struct Recycle {
  struct Recycle *next;
};

struct pool {
  uint32_t  obj_sz;
  uint32_t  obj_id;
  int32_t   blk_id;
  uint32_t  nblk;
  struct Recycle  *next;
  uint8_t **data;
};

#define BLK 64
