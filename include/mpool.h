#ifndef GWMPOOL
#define POOL_HANDLE(name, nelem)
#define mp_alloc(name) calloc(1, sizeof(struct name##_))
#define mp_free(p, d) free((d))
#else
#define POOL_HANDLE(name, nelem)\
static pool name##_pool;\
__attribute__((constructor))\
static void mp_ini_##name() { mp_ini(&name##_pool, sizeof(struct name##_), (nelem)); }\
__attribute__((destructor))\
static void mp_end_##name() {\
/*\
printf("%s %s %u %u %i requested: %u\n", __FILE__, __func__, \
name##_pool.obj_id, name##_pool.nblk, name##_pool.blk_id, name##_pool.blk_sz); \
*/\
mp_end(&name##_pool);}\

typedef struct {
	uint32_t  obj_sz;
	uint32_t  blk_sz;
	uint32_t  obj_id;
	int32_t   blk_id;
	uint32_t  nblk;
	struct Recycle  *next;
	uint8_t **data;
} pool;

ANN void mp_ini(pool *p, const uint32_t elementSize, const uint32_t blockSize);
ANN void mp_end(pool *p);
ANN void *mp_alloc(pool *p) __attribute__((hot));
ANN void mp_free(pool *p, void *ptr);
#define mp_alloc(name) mp_alloc(&name##_pool)
#define mp_free(name, a) mp_free(&name##_pool, (a))
#endif
