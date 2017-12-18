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
  unsigned int set;
  unsigned int ini;
  /*unsigned int end;*/
} Line;

static int last, max_exec;
static Line* lines;

static void err(const char* base, const char* file) {
  fprintf(stderr, "Unable to do coverage for '\033[1m%s\033[0m'.\n", base);
  fprintf(stderr, "Reason: '\033[1m%s\033[0m', no such file.\n", file);
  if(strcmp(base, file))
    fprintf(stderr, "Did you run '\033[32;1mgwion -k\033[0m"
        " \033[1m%s\033[0m' ?\n", base);
  exit(EXIT_FAILURE);

}

static void da(char* base) {
  int line_count = MIN_LINE;
  int curr_line;
  char c[8];
  char filename[strlen(base) + 3];
  sprintf(filename, "%sda", base);
  FILE* f = fopen(filename, "r");

  char * line = NULL;
  if(!f) // err msg
    err(base, filename);
  while (1) {
    int ret = fscanf(f, "%i %s", &curr_line, (char*)&c);
    if(ret == 2) {
      if(curr_line >= line_count) {
        lines = realloc(lines, 2 * line_count * sizeof(Line));
        memset(lines + line_count, 0, line_count * sizeof(Line));
        line_count *= 2;
      }
      lines[curr_line].set++;
      if(curr_line > last)
        last = curr_line;
    }
    else if(errno != 0) {
      perror("scanf:");
      break;
    }
    else if(ret == EOF) {
        break;
    } else {
        fprintf(stderr, "No match.\n");
    }
  }
  fclose(f);
  if(line)
    free(line);
}

void cov(char* base) {
  char filename[strlen(base) + 4];
  int curr_line;
  char c[8];
  FILE* f;

  sprintf(filename, "%scov", base);
  f = fopen(filename, "r");
  if(!f) // err msg
    err(base, filename);
  while (1) {
    int ret = fscanf(f, "%i %s", &curr_line, (char*)&c);
    if(ret == 2) {
      if(!strcmp(c, "ini")) {
        lines[curr_line].ini++;
        max_exec++;
      }
      /*else*/
      /*lines[curr_line].end++;*/
    }
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

static void fill(uint min, uint max) {
  while(min++ < max)
    fprintf(stdout, " ");
}

void diagnostic(char* filename){
  FILE * f;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int line_size, line_count = 1;
  int max_line_digit = last ? floor(log10(last) + 1) : 1;
  int max_exec_digit = last ? floor(log10(max_exec) + 1) : 1;
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  line_size = w.ws_col - max_line_digit - max_exec_digit - 6;

  if(!(f = fopen(filename, "r")))
    err(filename, filename);

  while((read = getline(&line, &len, f)) != -1) {
    uint too_long = 0;
    uint has_comment = 0;
    int num_digit = line_count ? floor(log10(line_count) + 1) : 1;
    ssize_t line_len = read > line_size ? line_size : read - 1;
    if(read >= line_size)
      too_long = 1;
    const char* prefix;
    char detabed[TABLEN*line_len];
    char *stripped_line = read == 1 ? strdup("") : strndup(line, line_len);
    detab(stripped_line, detabed, TABLEN*line_len, &has_comment);
    free(stripped_line);
    line_len = strlen(detabed);
    if(lines[line_count].set) {
      prefix = lines[line_count].ini ?
      "\033[32m" : "\033[31m";
    } else
      prefix = "";
    fprintf(stdout, "\033[2m%i", line_count);
    fill(num_digit, max_line_digit);
    fprintf(stdout, ":\033[0m %s%s\033[0m\033[2m", prefix, detabed);
    fill(line_len, line_size + (has_comment ? 8 : 0));
    if(lines[line_count].set)
      fprintf(stdout, "%s (%i)\n\033[0m",
          too_long ? "\b\b..." : "|", lines[line_count].ini);
    else
      puts("| ...\033[0m");
    line_count++;
  }
  fclose(f);
  if(line)
    free(line);
}

int main(int argc, char** argv) {
  argv++;
  argc--;
  while(argc) {
    lines = calloc(MIN_LINE, sizeof(Line));
    da(*argv);
    cov(*argv);
    diagnostic(*argv);
    free(lines);
    argc--;
    argv++;
  }
  exit(EXIT_SUCCESS);
}
