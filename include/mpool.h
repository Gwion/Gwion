ANN struct pool* new_pool(const uint32_t elementSize);
ANN struct pool* mp_ini(const uint32_t elementSize);
ANN void mp_end(struct pool*);
ANEW ANN void *_mp_alloc2(struct pool*) __attribute__((hot));
ANEW ANN void *_mp_alloc(const m_uint) __attribute__((hot));
ANN void _mp_free(const m_uint, void*);
ANN void _mp_free2(struct pool*, void*);
#define mp_alloc(name) _mp_alloc(sizeof(struct name##_))
#define mp_free(name, a) _mp_free(sizeof(struct name##_), (a))
