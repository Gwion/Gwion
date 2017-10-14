#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#ifndef __linux__
#include <sys/select.h>
#endif
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "defs.h"
#include "map.h"
#include "err_msg.h"
#include "vm.h"
#include "compile.h"
#include "udp.h"
#include "arg.h"

static Udp udp;

void udp_send(const char* c, unsigned int i) {
  struct sockaddr_in addr = i ? udp.saddr : udp.caddr;
  if(sendto(udp.sock, c, strlen(c), 0,
            (struct sockaddr *) &addr, sizeof(addr)) < 1)
    err_msg(UDP, 0, "problem while sending"); // LCOV_EXCL_LINE
}

static m_bool udp_recv(int i, char* buf) {
  unsigned int addrlen = 0;
  struct sockaddr_in addr;

#ifndef __linux__
  fd_set read_flags, write_flags;
  struct timeval waitd = {10, 0};

  FD_ZERO(&read_flags);
  FD_ZERO(&write_flags);
  FD_SET(sock, &read_flags);
  FD_SET(sock, &write_flags);
#endif

  memset(buf, 0, 256);
  addr  = i ? udp.saddr : udp.caddr;

#ifndef __linux__
  if(select(sock + 1, &read_flags, &write_flags, (fd_set*)0, &waitd) < 0)
    return -1;
  if(FD_ISSET(sock, &read_flags)) {
    FD_CLR(sock, &read_flags);
#endif
    ssize_t len;
    if((len = recvfrom(udp.sock, buf, 255, 0, (struct sockaddr*)&addr, &addrlen)) < 0)
      err_msg(UDP, 0, "recvfrom() failed"); // LCOV_EXCL_LINE
    buf[len] = '\0';
    return 1;
#ifndef __linux__
  }
#endif
  return -1;
}

static void send_vector(Vector v, m_str prefix) {
  m_uint i;
  for(i = 0; i < vector_size(v); i++) {
    m_str file = (m_str)vector_at(v, i);
    m_uint size = strlen(prefix) + 1 + strlen(file) + 1;
    char name[size];
    memset(name, 0, size);
    strcpy(name, prefix);
    strcpy(name, " ");
    strcat(name, file);
    udp_send(name, 1);
  }
}

void udp_client(void* data) {
  Arg* arg= (Arg*)data;
  if(server_init(arg->udp->host, arg->udp->port) == -1) {
    if(arg->quit)
      udp_send("quit", 1);
    if(arg->loop > 0)
      udp_send("loop 1", 1);
    else if(arg->loop < 0)
      udp_send("loop 0", 1);
    send_vector(&arg->rem, "- ");
    send_vector(&arg->add, "- ");
    vector_release(&arg->add);
    vector_release(&arg->rem);
    vector_release(&arg->lib);
    exit(0);
  }
}

static void udp_run(VM* vm) {
  m_uint i;
  if(!udp.state)
    return;
  if(udp.state == -1) {
    shreduler_set_loop(vm->shreduler, 0);
  } else if(udp.state == 1) {
    shreduler_set_loop(vm->shreduler, 1);
  }
  for(i = 0; i < vector_size(&udp.add); i++) {
    m_str filename = (m_str)vector_at(&udp.add, i);
    compile(vm, filename);
    free(filename);
  }
  for(i = 0; i < vector_size(&udp.rem); i++)
    shreduler_remove(vm->shreduler, (VM_Shred)vector_at(&udp.rem, i), 1);
  vector_clear(&udp.add);
  vector_clear(&udp.rem);
  udp.state = 0;
}

void* udp_thread(void* data) {
  VM* vm = (VM*)data;
  vector_init(&udp.add);
  vector_init(&udp.rem);
  while(vm->is_running) {
    char buf[256];
    if(udp_recv(0, buf) < 0)
      continue; // LCOV_EXCL_LINE
    udp.state = 1;
    if(strncmp(buf, "quit", 4) == 0) {
      vm->is_running = 0;
      vm->wakeup();
    } else if(strncmp(buf, "-", 1) == 0) {
      m_uint i;
      VM_Shred shred = NULL;

      for(i = 0; i < vector_size(&vm->shred); i++) {
        shred = (VM_Shred)vector_at(&vm->shred, i);
        if(shred->xid == atoi(buf + 2) - 1)
          vector_add(&udp.rem, (vtype)shred);
      }
    } else if(strncmp(buf, "+", 1) == 0) {
      vector_add(&udp.add, (vtype)strdup(buf + 2));
    } else if(strncmp(buf, "loop", 4) == 0) {
      m_int i = atoi(buf + 5);
      if(i <= 0)
        udp.state = -1;
      else
        udp.state = 1;
    }
    pthread_mutex_lock(&vm->mutex);
    udp_run(vm);
    pthread_mutex_unlock(&vm->mutex);
  }
  return NULL;
}

#ifndef __linux__
static void set_nonblock(int socket) {
  int flags;
  flags = fcntl(socket, F_GETFL, 0);
  fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}
#endif

int server_init(char* hostname, int port) {
  struct hostent * host;
  if((udp.sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    err_msg(UDP, 0, "can't create socket"); // LCOV_EXCL_LINE
    return -1;                              // LCOV_EXCL_LINE
  }

#ifndef __linux__
  set_nonblock(sock);
#endif

  memset(&udp.saddr, 0, sizeof(udp.saddr));
  udp.saddr.sin_family = AF_INET;
  host = gethostbyname(hostname);
  if(!host) {
    udp.saddr.sin_addr.s_addr = inet_addr(hostname);
    if(udp.saddr.sin_addr.s_addr == -1) {
      udp.saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      err_msg(UDP, 0, "%s not found. setting hostname to localhost", hostname);
      char** host = &hostname;
      *host = "localhost";
    }
  } else bcopy(host->h_addr_list[0], (char *)&udp.saddr.sin_addr, host->h_length);
  udp.saddr.sin_port = htons(port);
  if(bind(udp.sock, (struct sockaddr *) &udp.saddr, sizeof(udp.saddr)) < 0) {
    err_msg(UDP, 0, "can't bind");
    return -1;
  }
  return 1;
}

void udp_release(pthread_t t) {
#ifdef __linux__
#ifndef ANDROID
  pthread_cancel(t);
  pthread_join(t, NULL);
#endif
#endif
  shutdown(udp.sock, SHUT_RDWR);
  vector_release(&udp.add);
  vector_release(&udp.rem);
}
