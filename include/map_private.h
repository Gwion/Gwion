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

#endif
