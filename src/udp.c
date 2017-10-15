#include <string.h>
#ifndef __linux__
#include <sys/select.h>
#endif
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "map.h"
#include "err_msg.h"
#include "vm.h"
#include "compile.h"
#include "udp.h"
#include "arg.h"

static void udp_send(Udp* udp, const char* c) {
  struct sockaddr_in addr = udp->saddr;
  if(sendto(udp->sock, c, strlen(c), 0,
            (struct sockaddr *) &addr, sizeof(addr)) < 1)
    err_msg(UDP, 0, "problem while sending"); // LCOV_EXCL_LINE
}

static m_bool udp_recv(Udp* udp, char* buf) {
  unsigned int addrlen = 0;
  struct sockaddr_in addr;

#ifndef __linux__
  fd_set read_flags, write_flags;
  struct timeval waitd = {10, 0};

  FD_ZERO(&read_flags);
  FD_ZERO(&write_flags);
  FD_SET(udp->sock, &read_flags);
  FD_SET(udp->sock, &write_flags);
#endif

  memset(buf, 0, 256);
  addr  = udp->caddr;

#ifndef __linux__
  if(select(udp->sock + 1, &read_flags, &write_flags, (fd_set*)0, &waitd) < 0)
    return -1;
  if(FD_ISSET(udp->sock, &read_flags)) {
    FD_CLR(udp->sock, &read_flags);
#endif
    ssize_t len;
    if((len = recvfrom(udp->sock, buf, 255, 0, (struct sockaddr*)&addr, &addrlen)) < 0)
      CHECK_BB(err_msg(UDP, 0, "recvfrom() failed")) // LCOV_EXCL_LINE
    buf[len] = '\0';
    return 1;
#ifndef __linux__
  }
#endif
  return -1;
}

static void send_vector(Udp* udp, Vector v, m_str prefix) {
  m_uint i;
  for(i = 0; i < vector_size(v); i++) {
    m_str file = (m_str)vector_at(v, i);
    m_uint size = strlen(prefix) + 1 + strlen(file) + 1;
    char name[size];
    memset(name, 0, size);
    strcpy(name, prefix);
    strcat(name, " ");
    strcat(name, file);
    udp_send(udp, name);
  }
}

#ifndef __linux__
static void set_nonblock(int socket) {
  int flags;
  flags = fcntl(socket, F_GETFL, 0);
  fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}
#endif

static m_bool server_init(Udp* udp) {
  struct hostent * host;
  UdpIf* udpif = ((Arg*)udp->arg)->udp;
  if((udp->sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    CHECK_BB(err_msg(UDP, 0, "can't create socket"))
#ifndef __linux__
  set_nonblock(udp->sock);
#endif

  memset(&udp->saddr, 0, sizeof(udp->saddr));
  udp->saddr.sin_family = AF_INET;
  host = gethostbyname(udpif->host);
  if(!host) {
    udp->saddr.sin_addr.s_addr = inet_addr(udpif->host);
    if(udp->saddr.sin_addr.s_addr == -1) {
      udp->saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      err_msg(UDP, 0, "%s not found. setting hostname to localhost",
          udpif->host);
      char** host = &udpif->host;
      *host = "localhost";
    }
  } else bcopy(host->h_addr_list[0], (char *)&udp->saddr.sin_addr, host->h_length);
  udp->saddr.sin_port = htons(udpif->port);
  if(bind(udp->sock, (struct sockaddr *) &udp->saddr, sizeof(udp->saddr)))
    CHECK_BB(err_msg(UDP, 0, "can't bind"))
  return 1;
}

void udp_client(void* data) {
  Udp* udp = (Udp*)data;
  Arg* arg= (Arg*)udp->arg;
  if(server_init(udp) == -1) {
    if(arg->quit)
      udp_send(udp, "quit");
    if(arg->loop > 0)
      udp_send(udp, "loop 1");
    else if(arg->loop < 0)
      udp_send(udp, "loop 0");
    send_vector(udp, &arg->rem, "-");
    send_vector(udp, &arg->add, "+");
    vector_release(&arg->add);
    vector_release(&arg->rem);
    vector_release(&arg->lib);
    exit(0);
  }
}

static void udp_run(Udp* udp) {
  VM* vm = udp->vm;
  m_uint i;
  if(!udp->state)
    return;
  if(udp->state == -1) {
    shreduler_set_loop(vm->shreduler, 0);
  } else if(udp->state == 1) {
    shreduler_set_loop(vm->shreduler, 1);
  }
  for(i = 0; i < vector_size(&udp->add); i++) {
    m_str filename = (m_str)vector_at(&udp->add, i);
    compile(vm, filename);
    free(filename);
  }
  for(i = 0; i < vector_size(&udp->rem); i++)
    shreduler_remove(vm->shreduler, (VM_Shred)vector_at(&udp->rem, i), 1);
  vector_clear(&udp->add);
  vector_clear(&udp->rem);
  udp->state = 0;
}

void* udp_thread(void* data) {
  Udp* udp = (Udp*)data;
  VM* vm = udp->vm;
  vector_init(&udp->add);
  vector_init(&udp->rem);
  while(vm->is_running) {
    char buf[256];
    if(udp_recv(udp, buf) < 0)
      continue; // LCOV_EXCL_LINE
    udp->state = 1;
    if(strncmp(buf, "quit", 4) == 0) {
      vm->is_running = 0;
      vm->wakeup();
    } else if(strncmp(buf, "-", 1) == 0) {
      m_uint i;
      VM_Shred shred = NULL;

      for(i = 0; i < vector_size(&vm->shred); i++) {
        shred = (VM_Shred)vector_at(&vm->shred, i);
        if(shred->xid == atoi(buf + 2) - 1)
          vector_add(&udp->rem, (vtype)shred);
      }
    } else if(strncmp(buf, "+", 1) == 0) {
      vector_add(&udp->add, (vtype)strdup(buf + 2));
    } else if(strncmp(buf, "loop", 4) == 0) {
      m_int i = atoi(buf + 5);
      if(i <= 0)
        udp->state = -1;
      else
        udp->state = 1;
    }
    pthread_mutex_lock(&vm->mutex);
    udp_run(udp);
    pthread_mutex_unlock(&vm->mutex);
  }
  return NULL;
}

void udp_release(Udp* udp, pthread_t t) {
#ifdef __linux__
#ifndef ANDROID
  pthread_cancel(t);
  pthread_join(t, NULL);
#endif
#endif
  shutdown(udp->sock, SHUT_RDWR);
  vector_release(&udp->add);
  vector_release(&udp->rem);
}
