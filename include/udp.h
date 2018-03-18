#ifndef __UDP_H
#define __UDP_H
#include <netdb.h>

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

ANN void* udp_process(void* data);
ANN void udp_client(void* data);
ANN void udp_release(Udp* udp, const pthread_t t);
#endif
