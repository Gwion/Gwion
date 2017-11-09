#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  unsigned int set;
  unsigned int ini;
  unsigned int end;
} Line; 
static int last, max_exec;
static Line* lines;

static void da(char* base) {
  int line_count = 64; 
  int curr_line;
  char c[8];
  char filename[strlen(base) + 3];
  sprintf(filename, "%sda", base);
  FILE* fpda = fopen(filename, "r");

  char * line = NULL;
  if(!fpda) // err msg
    exit(EXIT_FAILURE);
        while (1) {
        int ret = fscanf(fpda, "%i %s", &curr_line, &c);
        if(ret == 2) {
          if(curr_line >= line_count) {
            lines = realloc(lines, 2 * line_count * sizeof(Line)); // check
            memset(lines + line_count, 0, line_count);
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
            printf("No match.\n");
        }
    }
  fclose(fpda);
  if(line)
    free(line);
}

void cov(char* base) {
  char filename[strlen(base) + 4];
  int curr_line;
  char c[8];
  FILE* fp;
  
  sprintf(filename, "%scov", base);
  fp = fopen(filename, "r");
  if(!fp) // err msg
    exit(EXIT_FAILURE);
  while (1) {
    int ret = fscanf(fp, "%i %s", &curr_line, &c);
    if(ret == 2) {
      if(!strcmp(c, "ini")) {
        lines[curr_line].ini++;
        max_exec++;
      }
      else
        lines[curr_line].end++;
    }
    else if(errno != 0) {
      perror("scanf:");
      break;
    } 
    else if(ret == EOF) {
        break;
    } else {
        printf("No match.\n");
    }
  }
  fclose(fp);
}

void diagnostic(char* filename){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_count = 1;
    int max_line_digit = last ? floor(log10(last) + 1) : 1;
    int max_exec_digit = last ? floor(log10(max_exec) + 1) : 1;
    
        struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int line_size = w.ws_col - max_line_digit - max_exec_digit - 6;
    printf ("lines %d\n", w.ws_col);

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
      char* prefix = "";
      ssize_t line_len = read > line_size ? line_size : read - 1;
      char *stripped_line = read == 1 ? strdup("") : strndup(line, line_len);
      int num_digit = line_count ? floor(log10(line_count) + 1) : 1;
      if(lines[line_count].set) {
        prefix = lines[line_count].ini == lines[line_count].end ?
        "\033[32m" : "\033[31m";
    }
    printf("\033[2m%i", line_count);
    while(num_digit < max_line_digit) {
      num_digit++;
      printf(" ");
    }
    printf(":\033[0m %s%s\033[0m\033[2m", prefix, stripped_line);
    free(stripped_line);
    while(line_len < line_size) {
      line_len++;
      printf(" ");
    }
    if(lines[line_count].set)
    printf("| (%i)\n\033[0m", lines[line_count].end);
    else puts("|\033[0m");
    line_count++;
    }
    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
  argv++;
  argc--;
  while(argc) {
    lines = calloc(64, sizeof(Line));
    da(*argv);
    cov(*argv);
    diagnostic(*argv);
    free(lines);
    argc--;
    argv++;
  }
}
