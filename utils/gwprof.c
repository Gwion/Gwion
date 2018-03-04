#include <stdio.h>
#include <string.h>
#include <time.h>
#include <bsd/sys/time.h>

#define STRLEN 1 << 8
#define CALLEN 1 << 16

struct Call {
  char caller[STRLEN];
  char callee[STRLEN];
  int n;
  struct timespec ts;
  struct timespec sum;
  struct timespec max;
  struct timespec min;
};
static struct Call call[CALLEN];
static unsigned int n_call;

struct Caller {
  char name[STRLEN];
  int i;
  struct timespec ts;
  struct timespec sum;
  struct timespec max;
  struct timespec min;
};
struct Prof {
  char callee[STRLEN];
  struct Caller caller[CALLEN];
  int i;
  struct timespec ts;
};

static struct Prof prof[512];
//static int idx;

//static void find(Call* call, const char* callee, const char* caller) {
//}

int main() {
  FILE* f = fopen("examples/Gain.gwprof", "r");
  while (1) {
    int curr = -1;
    char callee[STRLEN];
    char caller[STRLEN];
    struct timespec ts;
    int ret = fscanf(f, "%s %s %lu %lu", &callee, &caller, &ts.tv_sec, &ts.tv_nsec);


    if(ret == EOF)
      break;
//call[n_call] = {&callee, &caller, &ts.tv_sec, &ts.tv_nsec };
//call[n_call] = {&callee, &caller, &ts.tv_sec, &ts.tv_nsec };
//call[n_call] = {&callee, &caller, &ts.tv_sec, &ts.tv_nsec };
  strncpy(call[n_call].callee, callee, STRLEN);
  strncpy(call[n_call].caller, caller, STRLEN);
  call[n_call].ts  = ts;
  n_call++;

    int jcurr = -1;
    unsigned i = 0;
    while(strlen(prof[i].callee)) {
      if(!strncmp(prof[i].callee, callee, STRLEN)) {
        unsigned int j = 0;
        while(strlen(prof[i].caller[j].name)) {
          if(!strncmp(prof[i].caller[j].name, caller, STRLEN)) {
            prof[i].caller[j].i++;
            timespecadd(&prof[i].caller[j].ts, &ts, &prof[i].caller[j].ts);
            timespecadd(&prof[i].caller[j].sum, &ts, &prof[i].caller[j].ts);

  if(timespeccmp(&ts, &prof[i].caller[j].max, >))
    prof[i].caller[j].max = ts;
//if(!timerisset(&prof[i].caller[j].max) || timespeccmp(&ts, &prof[i].caller[j].max, >)
            jcurr = i;
            break;
          }
          j++;
        }
        if(jcurr == -1) {
          strcpy(prof[i].caller[j].name, caller);
          timespecadd(&prof[i].caller[j].ts, &ts, &prof[i].caller[j].ts);
          timespecadd(&prof[i].caller[j].sum, &ts, &prof[i].caller[j].ts);
          prof[i].caller[j].i++;
        }
        curr = i;
        break;
      }
      i++;
    }
    if(curr == -1)
      strcpy(prof[i].callee, callee);
    prof[i].i++;
    prof[i].ts.tv_sec  += ts.tv_sec;
    prof[i].ts.tv_nsec += ts.tv_nsec;
  }
  fclose(f);
  {
    unsigned i = 0;
    while(strlen(prof[i].callee)) {
      fprintf(stdout, "%s %lu %lu %lu\n", prof[i].callee, prof[i].i, prof[i].ts.tv_sec, prof[i].ts.tv_nsec);
      unsigned j = 0;
      while(strlen(prof[i].caller[j].name)) {
        fprintf(stdout, "%s %lu %lu %lu\n", prof[i].caller[j].name, prof[i].caller[j].i,
          prof[i].caller[j].ts.tv_sec, prof[i].caller[j].ts.tv_nsec);
        j++;
      }
    i++;
    }
  }



  {
//    for(unsigned int i = 0; i <= n_call; i++) {
//      puts("call");

//    }

  }

  return 0;
}
