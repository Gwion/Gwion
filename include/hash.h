typedef struct Args_* Args;
typedef struct Macro_* Macro;
typedef struct Hash_* Hash;
struct Hash_ {
  Macro       *table;
  unsigned int size;  /* should be prime */
};

typedef struct GWPP_* GWPP;
struct pp_info {
  int data[32];
  int idx;
};

enum gwpp_state {
  NORMAL,
  AFTER_ARG, // paste after arg, e.g. do not print sharps (find when)
  QUOTED,
  COMMENT,
  MACRO, // we need a append_check inline function

};

enum tok_type {
  TOK_NONE,
  TOK_ESCAPE,
  TOK_SHARP,
  TOK_PASTE,
  TOK_QUOTE,
};

struct GWPP_ {
  FILE* out;
  char* filename;
  struct Hash_ h;
  struct pp_info* ifdef;
  struct pp_info* lines;
  enum tok_type tok;
  enum gwpp_state state;
  int err;
};//__attribute__((packed));

struct Args_ {
  char* name;
  char* text;
  Args next;
};
Args new_args(const char*);
void clean_args(const Args);
struct Macro_ {
  char* name;
  char* text;
  Args  base;
  Args  args;
  Macro next;
  int line;
  int pos;
};

void  hini(const Hash, const unsigned int);
void  hdel(const Hash);
void  hend(const Hash);
Macro hadd(const Hash, const char*);
int   hrem(const Hash, const char*);
Macro hhas(const Hash, const char*);
