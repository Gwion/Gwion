#ifndef __UDP_H
#define __UDP_H
#include <netdb.h>
#include <stdlib.h>

typedef struct {
  int sock;
  struct sockaddr_in saddr;
  struct sockaddr_in caddr;

  struct Vector_ add;
  struct Vector_ rem;
  m_int state;
} Udp;

void Send(const char* c, unsigned int i);
void* udp_thread(void* data);
int server_init(char* hostname, int port);
void udp_release();
#endif
