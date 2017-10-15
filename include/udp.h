#ifndef __UDP_H
#define __UDP_H
#include <netdb.h>
#include <stdlib.h>

typedef struct {
  VM* vm;
  void* arg;
  int sock;
  struct sockaddr_in saddr;
  struct sockaddr_in caddr;
  struct Vector_ add;
  struct Vector_ rem;
  m_int state;
} Udp;

void* udp_thread(void* data);
void udp_client(void* data);
void udp_release(Udp* udp, pthread_t t);
#endif
