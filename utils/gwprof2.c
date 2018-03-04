#include <stdio.h>
#include <string.h>
#include <time.h>
//#include <sys/time.h>
#include <bsd/sys/time.h>

#define STRLEN 1 << 8
#define CALLEN 1 << 8

struct Caller {
  struct timespec start;
  struct timespec end;
};

struct Callee {
  char callee[STRLEN];
  char caller[STRLEN];
  struct timespec start;
  struct timespec end;
};
int start_asc(const void *a, const void *b) {
  struct Callee* c = (struct Callee*)a;
  struct Callee* d = (struct Callee*)b;
  return timespeccmp(&c->start, &d->start, >);
}


void time_ascending(struct Callee* src, int n) {
  struct Callee tgt[n];
  memcpy(tgt, src, (n * sizeof(struct Callee)));
  qsort(tgt, n, sizeof(struct Callee), start_asc);
  for(int i = 0; i < n; i++)
    printf("%s %s %lu %lu\n", tgt[i].callee, tgt[i].caller,
           tgt[i].start.tv_sec, tgt[i].start.tv_nsec);
}

int main(int argc, char** argv) {
  int n = 0;
  struct Callee call[CALLEN];
  FILE* f = fopen("examples/Gain.gwprof", "r");
  while (1) {
    int state;//, curr = -1;
    char callee[STRLEN];
    char caller[STRLEN];
    struct timespec start, end;
    int ret = fscanf(f, "%i %s %s %ld %ld %ld %ld", &state, callee, caller,
              &start.tv_sec, &start.tv_nsec,
              &end.tv_sec, &end.tv_nsec); 
    if(ret == EOF)
      break;
//    for(int i = 0; i < n; i++) {
      strcpy(call[n].callee, callee);
      strcpy(call[n].caller, caller);
      call[n].start = start;
      call[n].end = end;

//      printf("%i %s %s %lu %lu %lu %lu\n", state, callee, caller,
//           start.tv_sec, start.tv_nsec,
//           end.tv_sec, end.tv_nsec);
//            timespecadd(&prof[i].caller[j].sum, &ts, &prof[i].caller[j].ts);
    n++;
  }
struct timespec sum = { 0, 0 };
  for(int i = 0; i < n; i++) {
    if(argc < 2 || !strcmp(argv[1], call[i].callee)) {
      if(argc < 3 || !strcmp(argv[2], call[i].caller)) {
        struct timespec ts = { 0, 0 };
        struct timespec ts2 = { 0, 0 };
        timespecsub(&call[i].end, &call[i].start, &ts);
        timespecsub(&call[i].start, &call[i].end, &ts);
        printf("%s %s %lu %lu %lu %lu\n\t%lu %lu %lu %lu %lu %lu\n", call[i].callee, call[i].caller,
           call[i].start.tv_sec, call[i].start.tv_nsec,
           call[i].end.tv_sec, call[i].end.tv_nsec, 
ts.tv_sec, ts.tv_nsec,
ts2.tv_sec, ts2.tv_nsec,
sum.tv_sec, sum.tv_nsec);
struct timespec _sum = sum;
           timespecadd(&_sum, &ts, &sum);
      }
    }
  }
  printf("[%i] sum %lu %lu\n", n, sum.tv_sec, sum.tv_nsec);
  time_ascending(call, n);

  if(f)
    fclose(f);
  return 0;
}
