#ifndef GWMPOOL
#define POOL_HANDLE(name, nelem)
#define mp_alloc(name) calloc(1, sizeof(struct name##_))
#define mp_free(p, d) free((d))
#else
//#define POOL_HANDLE(name, nelem)
//#define mp_alloc(name) _mp_alloc(sizeof(struct name##_))
//#define mp_free(name, a) _mp_free(sizeof(struct name##_), (a))
//ANEW ANN void *_mp_alloc(const size_t) __attribute__((hot));
//ANN void _mp_free(const size_t, void*);

#define POOL_HANDLE(name, nelem)\
static struct pool* name##_pool;\
__attribute__((constructor(900)))\
static void mp_ini_##name() { name##_pool = mp_ini(sizeof(struct name##_), (nelem)); }

ANN struct pool* mp_ini(const uint32_t elementSize, const uint32_t blockSize);
ANN void mp_end(struct pool*);
ANEW ANN void *mp_alloc(struct pool*) __attribute__((hot));
ANN void mp_free(struct pool*, void*);
#define mp_alloc(name) mp_alloc(name##_pool)
#define mp_free(name, a) mp_free(name##_pool, (a))
#endif
