#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#define TABLEN 2
#define MIN_LINE 64
typedef struct {
  uint set;
  uint ini;
} Line;

typedef struct {
  uint line_count;
  char* line;
  uint i;
  char c[8];
  ssize_t s;
} Data;

typedef struct {
  const char* base;
  FILE* out;
  uint last;
  uint max_exec;
  Line* lines;
  char* postfix;
} Cov;

typedef void (*cov_func)(Cov*,Data*);

static void err(const char* base, const char* file) {
  fprintf(stderr, "Unable to do coverage for '\033[1m%s\033[0m'.\n", base);
  fprintf(stderr, "Reason: '\033[1m%s\033[0m', no such file.\n", file);
  if(strcmp(base, file))
    fprintf(stderr, "Did you run '\033[32;1mgwion -k\033[0m"
        " \033[1m%s\033[0m' ?\n", base);
  exit(EXIT_FAILURE);

}

static void da(Cov* cov, Data* d) {
  if(d->i >= d->line_count) {
    cov->lines = realloc(cov->lines, 2 * d->line_count * sizeof(Line));
    memset(cov->lines + d->line_count, 0, d->line_count * sizeof(Line));
    d->line_count *= 2;
  }
  cov->lines[d->i].set++;
  if(d->i > cov->last)
    cov->last = d->i;
}

static void co(Cov *cov, Data* d) {
  if(!strcmp(d->c, "ini")) {
    cov->lines[d->i].ini++;
    cov->max_exec++;
  }
}

static void run(Cov *cov, void(*func)(Cov*, Data*)) {
  char filename[strlen(cov->base) + strlen(cov->postfix)+ 1];
  Data d;
  FILE* f;

  memset(&d, 0, sizeof(Data));
  d.line_count = MIN_LINE;

  sprintf(filename, "%s%s", cov->base, cov->postfix);
  if(!(f = fopen(filename, "r")))
    err(cov->base, filename);
  while (1) {
    int ret = fscanf(f, "%u %s", &d.i, (char*)&d.c);
    if(ret == 2)
      func(cov, &d);
    else if(errno != 0) {
      perror("scanf:");
      break;
    } else if(ret == EOF) {
      break;
    } else {
      fprintf(stderr, "No match.\n");
    }
  }
  fclose(f);
}

static void colorize(char* out, size_t* i, const char* color ) {
  size_t j, len = strlen(color);
  out[(*i)++] = '\033';
  out[(*i)++] = '[';
  for(j = 0; j < len; j++)
    out[(*i)++] = color[j];
  out[(*i)++] = 'm';
}

static void detab(char* in, char* out, size_t max_len, uint* has_comment) {
  size_t i = 0;
  while(*in && i < max_len - 1) {
    if(*in == '\t') {
      for(int j = 0; j < TABLEN; j++)
        out[i++] = ' ';
    } else if(!*has_comment && *in == '/' && *(in + 1) && *(in + 1) == '/') {
      *has_comment = 1;
      colorize(out, &i, "0");
      colorize(out, &i, "2");
      out[i++] = *in;
    } else
      out[i++] = *in;
    (void)*in++;
  }
  out[i] = 0;
}

static void fill(Cov* cov, uint min, uint max) {
  while(min++ < max)
    fprintf(cov->out, " ");
}

static void tty(Cov* cov, Data* d) {
  int max_line_digit = floor(log10(cov->last) + 1);
  int max_exec_digit = floor(log10(cov->max_exec) + 1);
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  uint line_size = w.ws_col - max_line_digit - max_exec_digit - 6;
  uint too_long = 0;
  uint has_comment = 0;
  uint num_digit = d->line_count ? floor(log10(d->line_count) + 1) : 1;
  ssize_t line_len = ((uint)d->s > line_size) ? line_size  - 1: d->s - 1;
  if((uint)d->s > line_size)
    too_long = 1;
  const char* prefix;
  char detabed[TABLEN*line_len];
  char *stripped_line = d->s == 1 ? strdup("") : strndup(d->line, line_len);
  detab(stripped_line, detabed, TABLEN*line_len, &has_comment);
  free(stripped_line);
  line_len = strlen(detabed);
  if(cov->lines[d->line_count].set) {
    prefix = cov->lines[d->line_count].ini ?
    "\033[32m" : "\033[31m";
  } else
    prefix = "";
  fprintf(cov->out, "\033[2m%i", d->line_count);
  fill(cov, num_digit, max_line_digit);
  fprintf(cov->out, ":\033[0m %s%s\033[0m\033[2m", prefix, detabed);
  fill(cov, line_len, line_size + (has_comment ? 8 : 0));
  if(cov->lines[d->line_count].set)
    fprintf(cov->out, "%s (%i)\n\033[0m",
        too_long ? "\b\b..." : "|", cov->lines[d->line_count].ini);
  else
    puts("| ...\033[0m");
}

static void terminal(Cov* cov, Data* d) {
  if(isatty(fileno(cov->out)))
    tty(cov, d);
  else
    fprintf(cov->out, "%i\n", cov->lines[d->line_count].ini);
}

void diagnostic(Cov* cov, cov_func func){
  FILE * f;
  Data d;
  size_t len = 0;

  memset(&d, 0, sizeof(Data));
  d.line_count = 1;
  if(!(f = fopen(cov->base, "r")))
    err(cov->base, cov->base);
  while((d.s = getline(&d.line, &len, f)) != -1) {
    func(cov, &d);
    d.line_count++;
  }
  fclose(f);
  if(d.line)
    free(d.line);
}

int main(int argc, char** argv) {
  int max_exec = 0;
  cov_func func = terminal;
  FILE* out = stdout;
  argv++;
  argc--;
  while(argc) {
    Cov c = { *argv, out, 1, max_exec, NULL, NULL };
    c.lines = calloc(MIN_LINE, sizeof(Line));
    c. postfix = "da";
    run(&c, da);
    c. postfix = "cov";
    run(&c, co);
    diagnostic(&c, func);
    free(c.lines);
    argc--;
    argv++;
    max_exec = c.max_exec;
  }
  exit(EXIT_SUCCESS);
}
