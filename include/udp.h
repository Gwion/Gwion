#ifndef __UDP_H
#define __UDP_H
#include <netdb.h>
#include <pthread.h>
/*#include <arpa/inet.h>*/
#include <stdlib.h>

typedef struct {
  int sock;
  struct sockaddr_in saddr;
  struct sockaddr_in caddr;

  struct Vector_ add;
  struct Vector_ rem;
  m_int state;
  pthread_t srv_thread;
} Udp;

void Send(const char* c, unsigned int i);
void* server_thread(void* data);
int server_init(char* hostname, int port);
void server_destroy();
#endif
