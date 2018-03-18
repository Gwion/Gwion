#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <search.h>

typedef struct {
  char name[128];
  char dir[128];
} PlugInfo;

static PlugInfo info[128];
static size_t n, installed;
static const char *pdir, *path, **list;

static const char* get(const char* c) {
  const char *s = getenv(c);
  if(!s) {
    fprintf(stderr, "'%s' not set\n", c);
    exit(1);
  }
  return s;
}

static int compar(const void *a, const void *b) {
  return strncmp(((PlugInfo*)a)->name, ((PlugInfo*)b)->name, 128);
}

static int so_filter(const struct dirent* dir) {
  size_t len = strlen(dir->d_name);
  return len < 3 ? 0 : !strncmp(dir->d_name + len - 3, ".so", 3);
}

static void fill(const char* d, const char* c) {
  PlugInfo i;
  if(n == 128) {
    fprintf(stderr, "Max plugin number (%u) exceeded\n", 128);
    return;
  }
  strncpy(i.name, c, 128);
  strncpy(i.dir, "", 128);
  PlugInfo* p = lsearch(&i, info, &n, sizeof(PlugInfo), compar);
  if(p->dir[0] == '\0') {
    strncpy(p->dir, d, 128);
    strncpy(p->name, c, strlen(c) - 2);
  }
}

static void directory_handler(const char* dirname) {
  struct dirent **namelist;
  int n = scandir(dirname, &namelist, so_filter, alphasort);
  if(n > 0) {
    while(n--) {
      fill(dirname, namelist[n]->d_name);
      free(namelist[n]);
    }
    free(namelist);
  }
}

static void find(const char* c) {
  PlugInfo i;
  strcpy(i.name, c);
  strcat(i.name, ".so");
  PlugInfo* p = lfind(&i, info, &n, sizeof(PlugInfo), compar);
  char tgt[128*2];
  snprintf(tgt, 128*2, "%s/%s", pdir, p->name);
  if(p) {
    int state;
    if(!strncmp(p->dir, pdir, 128)) {
      --installed;
      state = unlink(tgt);
    } else {
      char src[128*2];
      ++installed;
      snprintf(src, 128*2, "%s/%s", p->dir, p->name);
      state = link(src, tgt);
    }
    if(state == -1)
      perror("there as been a problem");
  } else
    fprintf(stderr, "'%s', unknown plugin\n", c);
}


const char** strsplit(const char* s, unsigned int *n) {
  void* data;
  char* _s = (char*)s;
  const char** p;
  unsigned int sz,
    l = strlen(s);

  *n = 1;
  while(( _s = strchr(_s, ':'))) {
    ++_s;
    ++(*n);
  }
  sz = (*n + 1) * sizeof(char*);
  p = data = malloc(sz + l + 1);
  if(data) {
    *p = _s = strncpy(((char*)data) + sz, s, l + 1);
    if(*n > 1) {
      while((_s = strchr(_s, ':'))) {
        *_s = '\0';
        ++_s;
        *++p = _s;
      }
    }
    *++p = NULL;
  }
  return data;
}

static void print(unsigned int i, const unsigned max) {
  for(i; i < max; i++) {
    char s[128];
    snprintf(s, strlen(info[i].name) - 2, "%s", info[i].name);
    printf("\t%s\n", s);
  }
}

int main(int argc, char **argv) {
  unsigned int i;
  pdir = get("GWPLUG_DIR");
  path = get("GWPLUG_PATH");
  list = strsplit(path, &i);
  directory_handler(pdir);
  installed = n;
  while(i--)
    directory_handler(list[i]);
  while(++argv && *argv)
    find(*argv);
  puts("installed:");
  print(0, installed);
  puts("available:");
  print(installed, n);
  free(list);
}
