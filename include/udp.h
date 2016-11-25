#ifndef __UDP_H
#define __UDP_H
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>

//#include "shred.h"

int sock;
extern int port;
extern char* hostname;
struct sockaddr_in saddr;
struct sockaddr_in caddr;
pthread_t srv_thread;
void Send(const char* c, unsigned int i);
char* Recv(int i);

void* server_thread(void* data);
int server_init(char* hostname, int port);
void server_destroy();
#endif
