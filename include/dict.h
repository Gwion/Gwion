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

ANN static inline void dict_alloc(MemPool mp, const M_Object o, const m_uint sz, const m_uint capacity) {
  HMap *hmap = &*(struct HMap*)o->data;
  hmap->data  = (m_bit*)mp_calloc2(mp, sz * capacity);
  hmap->state = (m_bit*)mp_calloc2(mp, sizeof(HState) * capacity);
  hmap->capacity = capacity;
}
#endif
