#include <stdlib.h>
#include <stdio.h>
#include <string.h>
enum mode { NORMAL_MODE, FILE_MODE, PREFIX_MODE, CODE_MODE };
struct File {
  unsigned int n;
  char  c[128][128];
  FILE* f[128];
};

static void file_release(const struct File* file) {
  for(unsigned int i = 0; i < file->n; ++i) {
    if(file->f[i])
      fclose(file->f[i]);
  }
}

static FILE* file_get(struct File* file, const char* filename) {
  for(unsigned int i = file->n + 1; --i;) {
    if(!strcmp(filename, file->c[i-1]))
     return file->f[i-1];
  }
  if(file->n == 128) {
    fprintf(stderr, "maximum number of file (%i) exceeded\n", 128);
    return NULL;
  }
  strcpy(file->c[file->n], filename);
  return file->f[file->n++] = fopen(filename, "w");
}

int main() {
    struct File files = { 0 };
    char name[128] = "\0";
    FILE* f = NULL;
    enum mode m = NORMAL_MODE;
    int i = 0;

    for(int c, bt = 0; 1;) {
      if((c = getc(stdin)) == EOF)
        break;
      if(c == '`') {
        if(++bt == 3) {
          if(m == NORMAL_MODE) {
            m = FILE_MODE;
            putc(c, stdout);
          } else {
            f = NULL;
            m = NORMAL_MODE;
          }
        }
      } else {
        bt = 0;
        if(m == CODE_MODE && f)
            putc(c, f);
        else if(m == FILE_MODE || m == PREFIX_MODE) {
          if(c == ' ') continue;
          if(c == '\n') {
            name[i] = '\0';
            if(m!= PREFIX_MODE)
              fputs(name, stdout);
            f = file_get(&files, name);
            m = CODE_MODE;
            i = 0;
          } else {
            name[i++] = c;
            if(c == '.') {
              m = PREFIX_MODE;
              continue;
            }
          }
        }
      }
      if(m != FILE_MODE)
        putc(c, stdout);
    }

    file_release(&files);

}


