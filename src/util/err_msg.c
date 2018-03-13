#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"

#ifdef CURSES
#include <stdlib.h>
#include <ncurses.h>
#include <regex.h>
#include "absyn.h"
#include "operator.h"
#include "vm.h"
#include "instr.h"

static m_uint height, width;

enum dbg_t{
  DBG_PTR,
  DBG_INT,
  DBG_FLOAT,
};

struct BP {
  char    c[256];
  regex_t r;
};
#define DBG_SZ SIZEOF_MEM/SZ_FLOAT
struct ShredInfo {
  enum dbg_t t;
  char func[256];
  m_uint index;
  m_uint offset;
  WINDOW* pad;
  int pos;
};


static struct ShredInfo* curr;
static WINDOW* w;
static WINDOW* wout;
static WINDOW* werr;
static WINDOW* wexe;
static m_uint hexe, hout, herr;
static Vector shreds;
static Vector infos;
static Vector breaks;

static void size() {
  int y, x;
  getmaxyx(w, y, x);
  height = (y-1)/2;
  width = x - 1;
}

static void _init() {
  w = initscr();
  start_color();
  use_default_colors();
  shreds = new_vector();
  infos = new_vector();
  breaks = new_vector();
  init_pair(1, COLOR_CYAN, -1);
  init_pair(2, COLOR_MAGENTA, -1);
  init_pair(3, COLOR_GREEN, -1);
  init_pair(4, COLOR_RED, -1);
  init_pair(5, COLOR_BLUE, -1);
  noecho();
  curs_set(0);
  size();
  keypad(w, TRUE);
}

static void cross() {
  for(int i = 0; i < width/2; i++) {
    mvwaddch(w, height, i, ACS_HLINE | COLOR_PAIR(3));
    mvwaddch(w, height, i+width/2, ACS_HLINE | COLOR_PAIR(5));
  }
  for(int i = 0; i < height; i++) {
    mvwaddch(w, i, width/2, ACS_VLINE | COLOR_PAIR(1));
    mvwaddch(w, i+height+1, width/2, ACS_VLINE | COLOR_PAIR(4));
  }
  mvwaddch(w, height, width/2, ACS_PLUS);
}

__attribute__((constructor))
void init_curses() {
_init();
  wexe = newpad(10000, 256);
  scrollok(wexe, 1);

  wout = newpad(10000, 256);
  scrollok(wout, 1);

  werr = newpad(10000, 256);
  scrollok(werr, 1);

  cross();
  refresh();
}

__attribute__((destructor))
void end_curses() {
  delwin(w);
  endwin();
  free_vector(shreds);
  free_vector(infos);
  for(m_uint i = 0; i < vector_size(breaks); i++) {
    struct BP* bp = (struct BP*)vector_at(breaks, i);
    regfree(&bp->r);
    free(bp);
  }
  free_vector(breaks);
}
#undef stderr
#define fprintf wprintw
#define vfprintf vwprintw
#undef stderr
#define stderr  werr
#undef stdout
#define stdout  wout
m_bool gw_exe(const m_str func, char* fmt, ...) {
  wprintw(wexe, "%s ", func);
  if(curr)
    sprintf(curr->func, "%s", func);
  va_list arg;
  va_start(arg, fmt);
  vfprintf(wexe, fmt, arg);
  va_end(arg);

  prefresh(wexe, hexe++, 0, 0, width/2 + 1, height - 1, width);
  return -1;
}

static void do_stack(struct ShredInfo* info, m_uint i, m_uint w, char* data) {
  if(info->t == DBG_FLOAT) {
    mvwprintw(info->pad, i, w, "% 20.4f", *(m_float*)(data+i*SZ_FLOAT));
  } else if(info->t == DBG_INT) {
    mvwprintw(info->pad, i, w, "% 20i", *(m_int*)(data+i*SZ_FLOAT));
  } else
    wprintw(info->pad, "% 20p", *(m_uint*)(data+i*SZ_FLOAT));
 waddch(info->pad, ' ' | A_BOLD);
 waddch(info->pad, ' ' | A_BOLD);
}

static void display(VM_Shred shred, struct ShredInfo* info) {
  m_int mpos = (shred->mem - shred->_mem)/SZ_FLOAT;
  m_int rpos = (shred->reg - shred->_reg)/SZ_FLOAT;

  for(m_uint i = 0; i < height; i++) {
    mvwprintw(info->pad, i, 0, "% 5lu", (info->offset+i)*SZ_FLOAT);
    mvwaddch(info->pad, i, 5, ':');
    do_stack(info, info->offset + i, 6, shred->_mem);
    wprintw(info->pad, " ");
    do_stack(info, info->offset + i, (width-10)/4, shred->_reg);
  }
  mvwchgat(info->pad, (info->index % height), 0, -1, A_BOLD, 0, NULL);
  if(mpos >= info->offset)
    mvwaddch(info->pad, (mpos% height), width/4+6, '<' | A_BOLD | COLOR_PAIR(2));
  if(rpos >= info->offset)
    mvwaddch(info->pad, (rpos% height), width/2, '<' | A_BOLD | COLOR_PAIR(2));
  prefresh(info->pad, 0, 0, 0, 0, height - 1, width/2 -1);
  prefresh(wexe, hexe, 0, 0, width/2 + 1, height - 1, width);
  prefresh(wout, hout, 0, height + 1, 0, height*2, width/2 - 1);
  prefresh(werr, herr, 0, height + 1, width/2 + 1, height*2, width);
}

static void bp_add() {
  char s[256];
  mvwin(w, height*2, 0);
  deleteln();
  mvwprintw(w, height*2, 0, "new breakpoint: ");
  refresh();
  echo();
  getnstr(s, 256);
  noecho();
  struct BP* bp = malloc(sizeof(struct BP));
  strcpy(bp->c, s);
  regcomp(&bp->r, s, 0);
  vector_add(breaks, (vtype)bp);
}

static void bp_rem() {
  char s[256];
  mvwin(w, height*2, 0);
  deleteln();
  mvwprintw(w, height*2, 0, "remove breakpoint: ");
  refresh();
  echo();
  getnstr(s, 256);
  noecho();
  for(m_uint i = 0; i < vector_size(breaks); i++) {
    struct BP* bp = (struct BP*)vector_at(breaks, i);
    if(!strcmp(bp->c, s)) {
      vector_rem(breaks, i);
      regfree(&bp->r);
      free(bp);
      break;
    }
  }
}

static void handle(VM_Shred shred, struct ShredInfo* info) {
  int key = getch();
  switch(key) {
    case KEY_RESIZE:
  {
    endwin();
    refresh();
    clear();
    size();
    cross();
    refresh();
  }
break;
    case KEY_NPAGE:
      if(info->index >= DBG_SZ)break;
      info->index += height;
      if(info->index >= info->offset + height)
        info->offset += height;
      break;
    case KEY_UP:
      if(info->index >= DBG_SZ)break;
      info->index++;
      if(info->index >= info->offset + height)
        info->offset += height;
      break;
    case KEY_DOWN:
      if(!info->index)
        break;
      if(--info->index < info->offset)
        info->offset -= height;
      break;
    case KEY_PPAGE:
      if(!info->index)
        break;
      if((info->index -= height) < info->offset)
        info->offset -= height;
      break;
    case KEY_F(1):
      if(!hexe)
        break;
        prefresh(wexe, --hexe, 0, 0, width/2 + 1, height - 1, width);
      break;
    case KEY_F(13):
        prefresh(wexe, ++hexe, 0, 0, width/2 + 1, height - 1, width);
      break;
    case KEY_F(2):
      if(!hout)
        break;
      prefresh(wout, --hout, 0, height + 1, 0, height*2, width/2 - 1);
      break;
    case KEY_F(14):
      prefresh(wout, ++hout, 0, height + 1, 0, height*2, width/2 - 1);
      break;
    case KEY_F(3):
      if(!herr)
        break;
      prefresh(werr, --herr, 0, height + 1, width/2 + 1, height*2, width);
      break;
    case KEY_F(15):
      prefresh(werr, ++herr, 0, height + 1, width/2 + 1, height*2, width);
      break;
    case 'b':
      bp_add();
      break;
    case 'r':
      bp_rem();
      break;
    case 'f':
      info->t = DBG_FLOAT;
      break;
    case 'i':
      info->t = DBG_INT;
      break;
    case 'p':
      info->t = DBG_PTR;
      break;
    case 'x':
      exit(1);
    default:
      return;
  }
  display(shred, info);
  handle(shred, info);
}

void gw_shred(VM_Shred shred) {
  if(!shred) {
    wattron(w, COLOR_PAIR(2));
    mvwprintw(w, height*2, width-14, "shred finished!");
    wattroff(w, COLOR_PAIR(2));
    wrefresh(w);
    getch();
    m_int index = vector_find(infos, (vtype)curr);
    vector_rem(infos, index);
    vector_rem(shreds, index);
    free(curr);
    return;
  } else
    mvwprintw(w, height*2, width-14, "               ");
  wattron(w, A_BOLD);
  wattroff(w, A_BOLD);
  m_int index = vector_find(shreds, (vtype)shred);
  if(index == -1) {
    vector_add(shreds, (vtype)shred);
    curr = calloc(1, sizeof(struct ShredInfo));
    vector_add(infos, (vtype)curr);
    curr->pad = newpad(height, width/2);
  } else
    curr = (struct ShredInfo*)vector_at(infos, index);
  mvwin(w, height*2, 0);
  deleteln();
  mvwprintw(w, height*2, 0, "%s [%" INT_F"] [%" INT_F"->%"INT_F"] %i", shred->name, shred->xid,
    shred->pc, shred->next_pc, curr->pos);
  display(shred, curr);
  if(vector_size(breaks)) {
    for(m_uint i = 0; i < vector_size(breaks); i++) {
      struct BP* bp = (struct BP*)vector_at(breaks, i);
      if(!regexec(&bp->r, curr->func, 0, NULL, 0)) {
        handle(shred, curr);
        break;
      }
    }
  } else
  handle(shred, curr);
}

void gw_pos(int pos) {
  if(curr)
    curr->pos = pos;
}
#endif

static const char* err_headers[] = { "PARSE", "SCAN0", "SCAN1", "SCAN2_", "CHECK", "EMIT!", "UTILS", "LEXER",
                                     "INSTR", " UDP ", "ALSA", " VM  ", " OP  "

};

m_bool err_msg(a_header header, m_uint pos, const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
#ifdef COLOR
  fprintf(stderr, "\033[1m[\033[0m\033[31m%s\033[0m\033[1m]\033[0m", err_headers[header]);
#else
  fprintf(stderr, "%s", err_headers[header]);
#endif
  if(pos > 0)
#ifdef COLOR
    fprintf(stderr, " \033[4mline \033[1m%" INT_F "\033[0m\t", pos);
#else
    fprintf(stderr, " line: %" INT_F "\t", pos);
#endif
  else
    fprintf(stderr, "\t");
  vfprintf(stderr, fmt, arg);
  fprintf(stderr, "\n");
  va_end(arg);
#ifdef CURSES
  prefresh(werr, herr, 0, height + 1, width/2 + 1, height*2, width);
  getch();
#endif
  return -1;
}

m_bool gw_err(const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  vfprintf(stderr, fmt, arg);
  va_end(arg);
#ifdef CURSES
  prefresh(werr, herr, 0, height + 1, width/2 + 1, height*2, width);
  getch();
#endif
  return -1;
}

m_bool gw_out(const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  vfprintf(stdout, fmt, arg);
  va_end(arg);
#ifdef CURSES
  prefresh(wout, hout, 0, height + 1, 0, height*2, width/2 - 1);
  getch();
#endif
  return -1;
}
