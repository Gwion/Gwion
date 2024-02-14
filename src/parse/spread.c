#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "gwion.h"
#include "object.h"
#include "instr.h"
#include "operator.h"
#include "import.h"

ANN bool spread_ast(const Env env, const Spread_Def spread, const Tmpl *tmpl) {
  const m_str data = spread->data;
  char c[256];
  DECL_B(FILE *,f, = fmemopen(data, strlen(data), "r"));
  for(uint32_t i = tmpl->list->len - 1; i < tmpl->call->len; i++) {
    fseek(f, 0, SEEK_SET);
    const TmplArg targ = *mp_vector_at(tmpl->call, TmplArg, i);
    if(targ.type != tmplarg_td) {
      gwerr_basic("invalid const expression in variadic template", NULL, "can't use expression in spread", env->name, targ.d.exp->loc, 0);
      Specialized *spec = mp_vector_at(tmpl->list, Specialized, tmpl->list->len - 1);
      gwerr_secondary("spread starts here", env->name, spec->tag.loc);
      env_set_error(env, true);
      return false;
    }
    DECL_B(const Type, t, = known_type(env, targ.d.td));
    struct AstGetter_ arg =  {env->name, f, env->gwion->st, .ppa = env->gwion->ppa};
    const m_str type = type2str(env->gwion, t, targ.d.td->tag.loc);
    sprintf(c, "%s=%s", s_name(spread->tag.sym), type);
    free_mstr(env->gwion->mp, type);
    pparg_add(env->gwion->ppa, c);
    for(uint32_t j = 0; j < spread->list->len; j++) {
      const Symbol sym = *mp_vector_at(spread->list, Symbol, j);
      m_str name = s_name(sym);
      sprintf(c, "%s=%s%u", name, name, i);
      pparg_add(env->gwion->ppa, c);
    }
    Ast ast = parse_pos(&arg, spread->tag.loc.first);
    pparg_rem(env->gwion->ppa, s_name(spread->tag.sym));
    for(uint32_t j = 0; j < spread->list->len;j++) {
      const Symbol sym = *mp_vector_at(spread->list, Symbol, j);
      m_str name = s_name(sym);
      pparg_rem(env->gwion->ppa, name);
    }
    if(!ast) return false;
    CHECK_B(scan0_ast(env, &ast));
    for(uint32_t i = 0; i < ast->len; i++) {
      const Section section = *mp_vector_at(ast, Section, i);
      mp_vector_add(env->gwion->mp, &env->context->extend, Section, section);
    }
    free_mp_vector(env->gwion->mp, Section , ast);
  }
  fclose(f);
  mp_vector_add(env->gwion->mp, &env->context->extend, Section, MK_SECTION(stmt, stmt_list, NULL));
  return true;
}

ANN Ast spread_class(const Env env, const Ast body) {
  const Ast extend = env->context->extend;
  Ast new_body = new_mp_vector(env->gwion->mp, Section, 0);
  uint32_t offset = 0;
  for(uint32_t i = 0; i < body->len; i++) {
    const Section section = *mp_vector_at(body, Section, i);
    if(section.section_type == ae_section_stmt) {
      Stmt_List list = section.d.stmt_list;
      Stmt_List acc = NULL;
      for(uint32_t j = 0; j < list->len; j++) {
        const Stmt stmt = *mp_vector_at(list, Stmt, j);
        if(stmt.stmt_type == ae_stmt_spread) {
          if(acc) {
            mp_vector_add(env->gwion->mp, &new_body, Section, MK_SECTION(stmt, stmt_list, acc));
            acc = NULL;
          }
          const Ast extend = env->context->extend;
          for(; offset < extend->len; offset++) {
            const Section section = *mp_vector_at(extend, Section, offset);
            if(section.section_type == ae_section_stmt && !section.d.stmt_list)
            {
              ++offset;
              break;
            }
            mp_vector_add(env->gwion->mp, &new_body, Section, section);
          }
        } else {
          if(!acc)
            acc = new_mp_vector(env->gwion->mp, Stmt, 0);
          mp_vector_add(env->gwion->mp, &acc, Stmt, stmt);
        }
      }
      if(acc) {
        mp_vector_add(env->gwion->mp, &new_body, Section, MK_SECTION(stmt, stmt_list, acc));
      }
    }
  }
  free_mp_vector(env->gwion->mp, Section, body);
  free_mp_vector(env->gwion->mp, Section, extend);
  return new_body;
}

ANN Stmt_List spread_func(const Env env, const Stmt_List body) {
  const Ast extend = env->context->extend;
  Ast new_body = new_mp_vector(env->gwion->mp, Stmt, 0);
  uint32_t offset = 0;
  bool ok = true;
  for(uint32_t i = 0; i < body->len; i++) {
    Stmt stmt = *mp_vector_at(body, Stmt, i);
    if(stmt.stmt_type == ae_stmt_spread) {
      for(; offset < extend->len; offset++) {
        const Section section = *mp_vector_at(extend, Section, offset);
        if(section.section_type == ae_section_stmt && !section.d.stmt_list)
          break;
        if(section.section_type != ae_section_stmt)
          ERR_OK_NODE(ok, &stmt, stmt.loc, "invalid section in variadic func");
        const Stmt_List list = section.d.stmt_list;
        for(uint32_t j = 0; j < list->len; j++) {
          const Stmt stmt = *mp_vector_at(list, Stmt, j);
          mp_vector_add(env->gwion->mp, &new_body, Stmt, stmt);
        }
      }
    } else {
      mp_vector_add(env->gwion->mp, &new_body, Stmt, stmt);
    }
  }
  free_mp_vector(env->gwion->mp, Stmt_List, body);
  free_mp_vector(env->gwion->mp, Section, extend);
  if(ok) return new_body;
  return NULL;
}
