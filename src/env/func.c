#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "gwfmt.h"
#include "vm.h"
#include "gwion.h"
#include "clean.h"

ANN void free_func(Func a, Gwion gwion) {
  if (fflag(a, fflag_tmpl)) func_def_cleaner(gwion, a->def);
  if (a->code) vmcode_remref(a->code, gwion);
  if (a->_wait) free_valuelist(gwion->mp, a->_wait);
  mp_free(gwion->mp, Func, a);
}

ANN Func new_func(MemPool p, const m_str name, const Func_Def def) {
  Func func         = mp_calloc(p, Func);
  func->name        = name;
  func->def         = def;
  func->inline_mult = 1.0;
  func->ref         = 1;
  return func;
}

ANN2(1, 2)
Symbol func_symbol(const Env env, const m_str nspc, const m_str base,
                   const m_str tmpl, const m_uint i) {
  const size_t base_len = strlen(base);
  const size_t tmpl_len = !tmpl ? 0 : strlen(tmpl) + 4;
  const size_t nspc_len = strlen(nspc);
  const size_t idx_len  = num_digit(i);
  const size_t len      = base_len + tmpl_len + nspc_len + idx_len + 2;
  char         name[len + 1];
  if(sprintf(name, "%s%s%s%s@%" UINT_F "@%s", base, !tmpl ? "" : ":[",
                   !tmpl ? "" : tmpl, !tmpl ? "" : "]", i, nspc) < 1)
    return NULL;
  return insert_symbol(env->gwion->st, name);
}

ANN void builtin_func(const Gwion gwion, const Func f, void *func_ptr) {
  set_vflag(f->value_ref, vflag_builtin);
  f->code = new_vmcode(gwion->mp, NULL, NULL, f->name, f->def->stack_depth, true, false);
  f->code->native_func = (m_uint)func_ptr;
  f->code->ret_type = f->def->base->ret_type;
  if(f->def->base->tmpl) {
    const TmplArgList *tl = f->def->base->tmpl->call;
    if(!tl) return;
    const Specialized spec = specializedlist_at(f->def->base->tmpl->list, f->def->base->tmpl->list->len - 1);
    if(!strcmp(s_name(spec.tag.sym), "...")) {
      const uint32_t len = tmplarg_ntypes(tl);
      f->code->types = new_typelist(gwion->mp, len);
      uint32_t n = 0;
      for(uint32_t i = 0; i < tl->len; i++)  {
        const TmplArg arg = tmplarglist_at(tl, i);
        if(likely(arg.type == tmplarg_td))
          typelist_set(f->code->types, n++, known_type(gwion->env, arg.d.td));
      }
    }
  }
}

ANN void print_signature(const Gwion gwion, const Func f) {
  struct GwfmtState ls = {.minimize=true, .ppa = gwion->ppa};
  gwfmt_state_init(&ls);
  text_init(&ls.text, gwion->mp);
  Gwfmt gwfmter = {.mp = gwion->mp, .st = gwion->st, .ls = &ls, .last = cht_nl};
  pos_ini(&gwfmter.pos);
  gwfmt_func_def(&gwfmter, f->def);
  gwlog_related_from(ls.text.str, f->value_ref->from); 
  text_release(&ls.text);
}
