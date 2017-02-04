#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "defs.h"
#include "vm.h"
#include "compile.h"
#include "err_msg.h"
#include "udp.h"
#include "shreduler.h"
unsigned int loop;

int sock;
//int port = 8888;
//char* hostname = "localhost";
struct sockaddr_in saddr;
struct sockaddr_in caddr;
pthread_t srv_thread;

void Send(const char* c, unsigned int i)
{
  struct sockaddr_in addr = i ? saddr : caddr;
  /* don't send if no port */
  if(!addr.sin_port)
    return;
  if(sendto(sock, c, strlen(c), 0,
            (struct sockaddr *) &addr, sizeof(addr)) < 1)
    err_msg(UDP, 0, "problem while sending");
}

char* Recv(int i)
{
  char buf[255];
  ssize_t len;
  unsigned int addrlen = 0;
  struct sockaddr_in addr;

  fd_set read_flags,write_flags;
  struct timeval waitd = {10, 0};

  FD_ZERO(&read_flags);
  FD_ZERO(&write_flags);
  FD_SET(sock, &read_flags);
  FD_SET(sock, &write_flags);
//	bzero(buf, 255);

  memset(buf, 0, 255);
  addr  = i ? saddr : caddr;


  if(select(sock+1, &read_flags, &write_flags, (fd_set*)0, &waitd) < 0)
	return NULL;
if(FD_ISSET(sock, &read_flags)) {
  FD_CLR(sock, &read_flags);
  if ((len = recvfrom(sock, buf, 255, 0,
                      (struct sockaddr *) &addr, &addrlen)) < 0)
    err_msg(UDP, 0, "recvfrom() failed");

  return strndup(buf, strlen(buf));
}
return NULL;
}

void* server_thread(void* data)
{
  VM* vm = (VM*)data;
  while(vm->is_running) {
    const char* buf;
    int index;

    buf = Recv(0);
    if(!buf)
      continue;
    if(strncmp(buf, "bonjour", 7) == 0);
    else if( strncmp(buf, "quit", 4) == 0) {
      vm->is_running = 0;
      vm->wakeup();
    }
    /* remove */
    else if( strncmp(buf, "-", 1) == 0) {
      buf += 2;
      shreduler_remove(vm->shreduler, (VM_Shred)vector_at(vm->shred, atoi(buf)), 1);
    }
//      check_remove_shred(buf);
    /* status */
//		else if( strncmp(buf, "^", 1) == 0)
//			status(1);
    else if( strncmp(buf, "+", 1) == 0) {
      buf += 2;
      /*      buf++;*/
      compile(data, (m_str)buf);
    } else if( strncmp(buf, "loop", 4) == 0) {
      strsep((char**)&buf, " ");
      index = atoi(buf);
      shreduler_set_loop(vm->shreduler, index);
    }
    /* else it might be a file */
    else
      compile(data, (m_str)buf);
  }
//pthread_exit(NULL);
  return NULL;
}

static void set_nonblock(int socket) {
    int flags;
    flags = fcntl(socket,F_GETFL,0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

int server_init(char* hostname, int port)
{
  struct hostent * host;
  sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(sock < 0) {
    err_msg(UDP, 0, "can't create socket");
    return -1;
  }

  //set non blocking
  set_nonblock(sock);

  /* Construct local address structure */
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  host = gethostbyname(hostname);
  if(!host) {
    saddr.sin_addr.s_addr = inet_addr(hostname);
    if( saddr.sin_addr.s_addr == -1 ) {
      saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      err_msg(UDP, 0, "%s not found. setting hostname to localhost", hostname);
//			hostname = strdup("localhost");
      char** host = &hostname;
      *host = "localhost";
//			hostname = "localhost";
    }
  } else bcopy( host->h_addr_list[0], (char *)&saddr.sin_addr, host->h_length );
  saddr.sin_port = htons(port);
  /* Bind to the local address */
  if (bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
    err_msg(UDP, 0, "can't bind");
    return -1;
  }
  return 1;
}

void server_destroy(pthread_t t)
{
//  pthread_cancel(t);
  pthread_join(t, NULL);
  shutdown(sock, SHUT_RDWR);
}
