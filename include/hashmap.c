#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"

typedef struct HashMapValue_ HashMapValue;

typedef struct HashMap_ *HashMap;

ANN HashMap new_hashmap(MemPool p, const size_t key_size, const size_t val_size);

ANN bool hashmap_find(const HashMap hmap, m_bit* key, m_bit *ret_val);

ANN bool hashmap_remove(const HashMap hmap, m_bit *key, m_bit *ret_val);

ANN void hashmap_set(const HashMap hmap, m_bit *key, m_bit *val);
ANN void free_hashmap(MemPool p, HashMap a);
