#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#ifndef __linux__
#include <sys/select.h>
#endif
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#define _XOPEN_SOURCE 700
#include <math.h>
#include <string.h>
#include "err_msg.h"
#include "absyn.h"
#include "context.h"
#include "type.h"
#include "compile.h"


static int sock;
static struct sockaddr_in saddr;
static struct sockaddr_in caddr;

static struct Vector_ add;
static struct Vector_ rem;
static m_int state;

void Send(const char* c, unsigned int i) {
  struct sockaddr_in addr = i ? saddr : caddr;
  if(sendto(sock, c, strlen(c), 0,
            (struct sockaddr *) &addr, sizeof(addr)) < 1)
    err_msg(UDP, 0, "problem while sending"); // LCOV_EXCL_LINE
}

static m_bool Recv(int i, char* buf) {
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
  addr  = i ? saddr : caddr;

#ifndef __linux__
  if(select(sock + 1, &read_flags, &write_flags, (fd_set*)0, &waitd) < 0)
    return -1;
  if(FD_ISSET(sock, &read_flags)) {
    FD_CLR(sock, &read_flags);
#endif
    ssize_t len;
    if((len = recvfrom(sock, buf, 255, 0, (struct sockaddr*)&addr, &addrlen)) < 0)
      err_msg(UDP, 0, "recvfrom() failed"); // LCOV_EXCL_LINE
    buf[len] = '\0';
    return 1;
#ifndef __linux__
  }
#endif
  return -1;
}

void* server_thread(void* data) {
  VM* vm = (VM*)data;
  vector_init(&add);
  vector_init(&rem);
  while(vm->is_running) {
    char buf[256];
    if(Recv(0, buf) < 0)
      continue; // LCOV_EXCL_LINE
    state = 1;
    if(strncmp(buf, "quit", 4) == 0) {
      vm->is_running = 0;
      vm->wakeup();
    } else if(strncmp(buf, "-", 1) == 0) {
      m_uint i;
      VM_Shred shred = NULL;

      for(i = 0; i < vector_size(&vm->shred); i++) {
        shred = (VM_Shred)vector_at(&vm->shred, i);
        if(shred->xid == atoi(buf + 2) - 1)
          vector_add(&rem, (vtype)shred);
      }
    } else if(strncmp(buf, "+", 1) == 0) {
      vector_add(&add, (vtype)strdup(buf + 2));
    } else if(strncmp(buf, "loop", 4) == 0) {
      m_int i = atoi(buf + 5);
      if(i <= 0)
        state = -1;
      else
        state = 1;
    }
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
  if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    err_msg(UDP, 0, "can't create socket"); // LCOV_EXCL_LINE
    return -1;                              // LCOV_EXCL_LINE
  }

#ifndef __linux__
  set_nonblock(sock);
#endif

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  host = gethostbyname(hostname);
  if(!host) {
    saddr.sin_addr.s_addr = inet_addr(hostname);
    if(saddr.sin_addr.s_addr == -1) {
      saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      err_msg(UDP, 0, "%s not found. setting hostname to localhost", hostname);
      char** host = &hostname;
      *host = "localhost";
    }
  } else bcopy(host->h_addr_list[0], (char *)&saddr.sin_addr, host->h_length);
  saddr.sin_port = htons(port);
  if(bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
    err_msg(UDP, 0, "can't bind");
    return -1;
  }
  return 1;
}

void server_destroy(pthread_t t) {
#ifdef __linux__
#ifndef ANDROID
  pthread_cancel(t);
  pthread_join(t, NULL);
#endif
#endif
  shutdown(sock, SHUT_RDWR);
  vector_release(&add);
  vector_release(&rem);
}

void udp_do(VM* vm) {
  m_uint i;
  if(!state)
    return;
  if(state == -1) {
    shreduler_set_loop(vm->shreduler, 0);
  } else if(state == 1) {
    shreduler_set_loop(vm->shreduler, 1);
  }
  for(i = 0; i < vector_size(&add); i++) {
    m_str filename = (m_str)vector_at(&add, i);
    compile(vm, filename);
    free(filename);
  }
  for(i = 0; i < vector_size(&rem); i++)
    shreduler_remove(vm->shreduler, (VM_Shred)vector_at(&rem, i), 1);
  vector_clear(&add);
  vector_clear(&rem);
  state = 0;
}

static const char* err_headers[] = { "PARSE", "SCAN0", "SCAN1", "SCAN2_", "CHECK", "EMIT!", "UTILS", "LEXER",
                                     "INSTR", " UDP ", "ALSA", " VM  ", " OP  "

};

char err_msg(a_header header, long int pos, const char* fmt, ...) {
  char msg[256];
  va_list arg;
  va_start(arg, fmt);
#ifdef COLOR
  fprintf(stderr, "\033[1m[\033[0m\033[31m%s\033[0m\033[1m]\033[0m", err_headers[header]);
#else
  fprintf(stderr, "%s", err_headers[header]);
#endif
  if(pos > 0)
#ifdef COLOR
    fprintf(stderr, " \033[4mline \033[1m%li\033[0m\t", pos);
#else
    fprintf(stderr, " line: %li\t", pos);
#endif
  vsnprintf(msg, 256, fmt, arg);
  fprintf(stderr, "\t%s\n", msg);
  va_end(arg);
  return -1;
}
#ifdef DEBUG
void debug_msg(const char* header, const char* fmt, ...) {
  char msg[256];
  va_list arg;
  va_start(arg, fmt);
#ifdef COLOR
  fprintf(stderr, "\033[1m[\033[0m\033[31m%s\033[0m\033[1m]\033[0m", header);
#else
  fprintf(stderr, "%s", header);
#endif
  memset(msg, 0, 256);
  vsnprintf(msg, 256, fmt, arg);
  fprintf(stderr, "\t%s\n", msg);
  va_end(arg);
}
#endif

#define SIZE 65347  /* should be prime */

struct S_Symbol_ {
  m_str name;
  S_Symbol next;
};

static S_Symbol hashtable[SIZE];

static S_Symbol mksymbol(const m_str name, S_Symbol next) {
  S_Symbol s = malloc(sizeof(*s));
  s->name = strdup(name);
  s->next = next;
  return s;
}

static void free_Symbol(S_Symbol s) {
  if(s->next)
    free_Symbol(s->next);
  free(s->name);
  free(s);
}

void free_Symbols() {
  int i;
  for(i = SIZE + 1; --i;) {
    S_Symbol s = hashtable[i - 1];
    if(s)
      free_Symbol(s);
  }
}

static unsigned int hash(const char *s0) {
  unsigned int h = 0;
  const char *s;
  for(s = s0; *s; s++)
    h = h * 65599 + *s;
  return h;
}

S_Symbol insert_symbol(const m_str name) {
  int index = hash(name) % SIZE;
  S_Symbol sym, syms = hashtable[index];

  for(sym = syms; sym; sym = sym->next)
    if(!strcmp(sym->name, (m_str)name))
      return sym;
  return hashtable[index] = mksymbol(name, syms);
}

m_str s_name(S_Symbol sym) {
  return sym->name;
}
m_uint num_digit(m_uint i) {
  return i ? (m_uint)floor(log10(i) + 1) : 1;
}

int verify_array(Array_Sub array) {
  if(array->err_num) {
    if(array->err_num == 1)
      CHECK_BB(err_msg(UTIL_, array->pos,
                       "invalid format for array init [...][...]..."))
      else if(array->err_num == 2)
        CHECK_BB(err_msg(UTIL_, array->pos,
                         "partially empty array init [...][]..."))
      }
  return 1;
}

int isa(Type var, Type parent) {
  return (var->xid == parent->xid) ? 1 : var->parent ? isa(var->parent, parent) : -1;
}

int isres(Env env, S_Symbol xid, int pos) {
  m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") || !name2op(s)) {
    err_msg(UTIL_, 0, "%s is reserved.", s_name(xid));
    return 1;
  }
  return -1;
}

int isprim(Type type) {
  return (type->array_depth || isa(type, &t_object) > 0) ? -1 : 1;
}

Type find_type(Env env, ID_List path) {
  S_Symbol xid = NULL;
  Nspc nspc;
  Type t = NULL;

  Type type = nspc_lookup_type(env->curr, path->xid, 1);
  if(!type)
    return NULL;
  nspc = type->info;
  path = path->next;

  while(path) {
    xid = path->xid;
    t = nspc_lookup_type(nspc, xid, 1);
    while(!t && type && type->parent) {
      t = nspc_lookup_type(type->parent->info, xid, -1);
      type = type->parent;
    }
    if(!t) {
      err_msg(UTIL_, path->pos,
              "...(cannot find class '%s' in nspc '%s')",
              s_name(xid), nspc->name);
      return NULL;
    }
    type = t;
    if(type)
      nspc = type->info;
    path = path->next;
  }
  return type;
}

m_bool env_add_value(Env env, m_str name, Type type, m_bool is_const, void* data) {
  Value v = new_value(type, name);
  ae_flag flag = ae_flag_checked | ae_flag_global | ae_flag_builtin | (is_const ? ae_flag_const : 0);
  v->flag = flag;
  v->ptr = data;
  v->owner = env->global_nspc;
  nspc_add_value(env->global_nspc, insert_symbol(name), v);
  return 1;
}

m_bool name_valid(m_str a) {
  m_uint i, len = strlen(a);
  for(i = 0; i < len; i++) {
    char c = a[i];
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c == '_') || (c >= '0' && c <= '9'))
      continue;
    else
      CHECK_BB(err_msg(UTIL_,  0, "illegal character '%c' in name '%s'...", a, a))
    }
  return 1;
}

Value find_value(Type type, S_Symbol xid) {
  Value value = NULL;
  if(!type || !type->info)
    return NULL;
  if((value = nspc_lookup_value(type->info, xid, -1)))
    return value;
  if(type->parent)
    return find_value(type->parent, xid);
  return NULL;
}

m_str type_path(ID_List list) {
  m_uint len = 0;
  ID_List path = list;
  while(path) {
    len += strlen(s_name(path->xid));
    if(path->next)
      len++;
    path = path->next;
  }
  char str[++len];
  memset(str, 0, len);
  path = list;
  while(path) {
    strcat(str, s_name(path->xid));
    if(path->next)
      strcat(str, ".");
    path = path->next;
  }
  return strdup(str);
}

Kindof kindof(Type type) {
  if(type->array_depth)
    return Kindof_Int;
  if(isa(type, &t_void) > 0)
    return Kindof_Void;
  else if(isa(type, &t_complex) > 0 || isa(type, &t_polar) > 0)
    return Kindof_Complex;
  if(isa(type, &t_int) > 0 || isa(type, &t_object) > 0)
    return Kindof_Int;
  else if(isa(type, &t_float) > 0 || isa(type, &t_time) > 0 || isa(type, &t_dur) > 0)
    return Kindof_Float;
  else if(isa(type, &t_vec3) > 0)
    return Kindof_Vec3;
  else if(isa(type, &t_vec4) > 0)
    return Kindof_Vec4;
  return Kindof_Int;
}


Type new_array_type(Env env, m_uint depth, Type base_type, Nspc owner_nspc) {
  Type t = new_type(te_array, base_type->name, &t_array);
  t->size = SZ_INT;
  t->array_depth = depth;
  t->d.array_type = base_type;
  t->info = t_array.info;
  ADD_REF(t->info);
  t->owner = owner_nspc;
  return t;
}

m_int str2char(const m_str c, m_int linepos) {
  if(c[0] == '\\') {
    switch(c[1]) {
      case '0':
        return '\0';
      case '\'':
        return '\'';
      case '\\':
        return '\\';
      case 'a':
        return '\a';
      case 'b':
        return '\b';
      case 'f':
        return '\f';
      case 'n':
        return '\n';
      case 'r':
        return '\r';
      case 't':
        return '\t';
      case 'v':
        return 'v';
      default:
        err_msg(UTIL_, linepos, "unrecognized escape sequence '\\%c'", c[1]);
        return -1;
    }
  } else
    return c[0];
}
