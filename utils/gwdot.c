#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <obstack.h>
#include <graphviz/gvc.h>
#include <bsd/sys/time.h>

//#define SIZE 65347
#define SIZE 10007

#define MAP_CAP 4
#define OFFSET 2

#define obstack_chunk_alloc malloc
#define obstack_chunk_free free
struct obstack obs;

struct Vector_ {
  unsigned int* ptr;
};
typedef struct Vector_* Vector;

static void vector_init(Vector v) {
  v->ptr = malloc(MAP_CAP * sizeof(unsigned int));
  v->ptr[0] = 0;
  v->ptr[1] = MAP_CAP;
}

static void vector_release(Vector v) {
  free(v->ptr);
}

static inline unsigned int vector_size(const Vector v) {
  return v->ptr[0];
}

static inline unsigned int vector_at(Vector v, const unsigned int i) {
  return (i >= v->ptr[0]) ? 0 : v->ptr[i + OFFSET];
}

struct Entry {
  char* name;
  Agnode_t* node;
  unsigned int n;
  struct timespec sum;
  float mean_sec, mean_nsec;
  struct Caller* ht[SIZE];
  struct Entry* next;
};

struct Caller {
  struct Entry* e;
  struct Vector_ v;
  struct Caller* next;
};

Agraph_t *graph;

__attribute__((nonnull(1)))
static struct Entry* new_entry(const char* name, struct Entry* next) {
  struct Entry *e = obstack_alloc(&obs, sizeof(struct Entry));
  e->name = strdup(name);
  e->node = agnode(graph, e->name, 1);
  if(!strncmp(name, "spork~", 6)) {
    if(!strncmp(name + 6 , "exp", 3))
      agsafeset(e->node, "color", "yellow", "yellow");
    else
      agsafeset(e->node, "color", "green", "green");
  } else
    agsafeset(e->node, "color", "blue", "blue");
  memset(e->ht, 0, sizeof(e->ht));
  e->next = next;
  return e;
}

static void iter_entry(struct Entry** ht, void (*action)(struct Entry*)) {
  for(unsigned int i = SIZE + 1; --i;) {
    struct Entry* e = ht[i - 1];
    if(e) {
      action(e);
    }
  }
}

static void iter_caller(struct Caller** ht, struct Entry* data, void
(*action)(struct Caller*, struct Entry*)) {
  for(unsigned int i = SIZE + 1; --i;) {
    struct Caller* c = ht[i - 1];
    if(c)
      action(c, data);
  }
}

static void free_caller(struct Caller*, struct Entry*);
static void free_entry(struct Entry* e) {
  if(e->next)
    free_entry(e->next);
  free(e->name);
  iter_caller(e->ht, NULL, free_caller);
}

static inline const unsigned int num_digit(const unsigned int i) {
  return i ? (unsigned int)floor(log10(i) + 1) : 1;
}

static void pp_caller(struct Caller* c, struct Entry* e) {
  unsigned int sz = vector_size(&c->v)/4;
  Agedge_t* l = agedge(graph, c->e->node, e->node, e->name, 1);
  agset(e->node, "color", "red");
  agsafeset(e->node, "tooltip", "red", "red");
  struct timespec sum = {0,0};
  struct timespec min = {INT_MAX,0};
  struct timespec max = {0,0};
  struct timespec dur[sz];

  for(unsigned int i = 0; i < sz; i++) {
    struct timespec t  =  { sum.tv_sec, sum.tv_nsec };
    struct timespec ini = { vector_at(&c->v, i  ), vector_at(&c->v, ++i) };
    struct timespec end = { vector_at(&c->v, ++i), vector_at(&c->v, ++i) };
    timespecsub(&end, &ini, &dur[i]);
    timespecadd(&t, &dur[i], &sum);
    if(timespeccmp(&dur[i], &min, <))
      min = dur[i];
    if(timespeccmp(&dur[i], &max, >))
      max = dur[i];
  }
  long unsigned int ssum = sum.tv_sec * 1000000 + sum.tv_nsec;
  float mean = ssum / (float)sz;
  long unsigned int variance = 0;
  for(unsigned int i = 0; i < sz; i++) {
    long unsigned int var = dur[i].tv_sec * 1000000 + dur[i].tv_nsec;
    variance = variance + pow(var - mean, 2);
  }
  char label[258];
  sprintf(label, "call %u times.\nmean:(%f)\n"
    "min:(%lu %lu) max:(%lu %lu) sum(%lu %lu)\n"
    "deviation:%f",
  vector_size(&c->v) / 4,
  mean,
  min.tv_sec, min.tv_nsec,
  max.tv_sec, max.tv_nsec,
  sum.tv_sec, sum.tv_nsec,
  sqrt(variance/(float)sz));
  agsafeset(l, "tooltip", label, label);
  timespecadd(&sum, &e->sum, &e->sum);
  e->n += sz;
}

static void pp_entry(struct Entry* e) {
  iter_caller(e->ht, e, pp_caller);
  if(e->n) {
    char s[512];
    long unsigned int sum = e->sum.tv_nsec * 1000000 + e->sum.tv_nsec;
    float mean = sum / (float)e->n;
    mean /= 1000000;
    sprintf(s, "%u mean(%f) sum(%9lu %9lu)", e->n,
    mean,
    e->sum.tv_sec, e->sum.tv_nsec);
    agset(e->node, "tooltip", s);
  }
}

static unsigned int hash(const char *s0) {
  unsigned int h = 0;
  const char *s;
  for(s = s0; *s; s++)
    h = h * 65599 + *s;
  return h;
}

struct Entry* insert_entry(struct Entry** ht, const char* name) {
  int index = hash(name) % SIZE;
  struct Entry* e, *n = ht[index];

  for(e = n; e; e = e->next)
    if(!strcmp(e->name, (char*)name))
      return e;
  return ht[index] = new_entry(name, n);
}

void vector_add2ts(const Vector v,
    const struct timespec ini, const struct timespec end) {
  int sz;
  while((sz = (v->ptr[1] - v->ptr[0] - OFFSET - (3*sizeof(unsigned int)))) < 0)
    v->ptr = xrealloc(v->ptr, (v->ptr[1] *= 2) * sizeof(unsigned int));
  v->ptr[v->ptr[0]++ + OFFSET] = ini.tv_sec;
  v->ptr[v->ptr[0]++ + OFFSET] = ini.tv_nsec;
  v->ptr[v->ptr[0]++ + OFFSET] = end.tv_sec;
  v->ptr[v->ptr[0]++ + OFFSET] = end.tv_nsec;
}


static struct Caller* new_caller(struct Entry* e, struct Caller* next) {
  struct Caller *c = obstack_alloc(&obs, sizeof(struct Caller));
  c->e = e;
  vector_init(&c->v);
  c->next = next;
  return c;
}


static void free_caller(struct Caller* c, struct Entry* e) {
  vector_release(&c->v);
}

struct Caller* insert_caller(struct Caller** ht, struct Entry* entry,
    struct timespec ini, struct timespec end) {
  int index = hash(entry->name) % SIZE;
  struct Caller* c, *n = ht[index];
  for(c = n; c; c = c->next)
    if(c->e == entry) {
      goto set;
  }
   c = ht[index] = new_caller(entry, n);
set:
   vector_add2ts(&c->v, ini, end);
   return c;
}

static void gv_init(GVC_t **gvc, int argc, char** argv) {
    *gvc = gvContext();
    graph = agopen("Gwion call graph", Agdirected, NULL);
    gvParseArgs(*gvc, argc, argv);
}

static void gv_render(GVC_t *gvc, Agraph_t *graph) {
  gvLayoutJobs(gvc, graph);
  gvRenderJobs(gvc, graph);
}

static void gv_free(GVC_t *gvc, Agraph_t *graph) {
  gvFreeLayout(gvc, graph);
  agclose(graph);
  gvFreeContext(gvc);
}

static void parse(struct Entry** ht) {
  FILE* f = fopen("gwmon.out", "r");
  while(1) {
    char callee[512];
    char caller[512];
    struct timespec ini, end;
    int ret = fscanf(f, "%512s %512s %lu %lu %lu %lu",
         callee,      caller,
        &ini.tv_sec, &ini.tv_nsec,
        &end.tv_sec, &end.tv_nsec);
    if(ret == EOF)
      break;
    else {
      struct Entry* e = insert_entry(ht, callee);
      struct Entry* f = insert_entry(ht, caller);
      insert_caller(e->ht, f, ini, end);
    }
  }
  fclose(f);
}

int main(int argc, char** argv) {
  struct Entry* ht[SIZE];
  obstack_init(&obs);
  GVC_t *gvc;
  gv_init(&gvc, argc, argv);
  memset(ht, 0, sizeof(ht));
  parse(ht);
  iter_entry(ht, pp_entry);
  iter_entry(ht, free_entry);

  gv_render(gvc, graph);
  gv_free(gvc, graph);
  obstack_free(&obs, NULL);
}
