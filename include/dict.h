#ifndef __DICT
#define __DICT
typedef struct HState {
  bool set;
  bool deleted;
} HState;

typedef struct HMap {
  m_bit  *state;
  m_bit  *data;
  m_uint  capacity;
  m_uint  count;
} HMap;


typedef struct HMapInfo {
  Type key;
  Type val;
  m_uint sz;
} HMapInfo;

#endif
