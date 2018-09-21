#include <libgccjit.h>
struct JitCC_ {
  gcc_jit_context* ctx;
  gcc_jit_block* block;
  gcc_jit_rvalue* shred;
};
