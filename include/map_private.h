#ifndef _MAP_PRIVATE
#define _MAP_PRIVATE

struct Vector_ {
  vtype* ptr;
};

struct Map_ {
  vtype* ptr;
};

struct Scope_ {
  struct Map_    commit_map;
  struct Vector_ vector;
};

#define MAP_CAP 4
#define OFFSET 2
#define VLEN(v)    (v)->ptr[0]
#define VCAP(v)    (v)->ptr[1]
#define VPTR(v, i)    (v)->ptr[OFFSET + (i)]
#define VKEY(v, i) (v)->ptr[OFFSET + (i) * 2]
#define VVAL(v, i) (v)->ptr[OFFSET + (i) * 2 + 1]
#endif
