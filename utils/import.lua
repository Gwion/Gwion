function declare_c_param(param, offset)
  local type;
  local increment = "SZ_INT"
  if string.match(param.type, "int") then
    print("\tm_int "..param.name.." = *(m_int*)(shred->mem + gw_offset);")
  elseif string.match(param.type, "SPFLOAT$") then
    print("\tm_float "..param.name.." = *(m_float*)(shred->mem + gw_offset);")
    increment = "SZ_FLOAT"
  elseif string.match(param.type, "SPFLOAT*") then
    print("\tm_float "..param.name.." = *(m_float*)(shred->mem + gw_offset);")
    increment = "SZ_FLOAT"
  elseif string.match(param.type, "char%s*") then
    print("\tM_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);")
    print("\tm_str "..param.name.." = STRING("..param.name.."_obj);")
    print("\trelease("..param.name.."_obj, shred);")
  elseif string.match(param.type, "sp_ftbl%s%*%*") then
    print("\tM_Object "..param.name.."_ptr = *(M_Object*)(shred->mem + gw_offset);")
    print("\tm_uint "..param.name.."_iter;")
    print("\tsp_ftbl** "..param.name.." = (sp_ftbl**)xmalloc(m_vector_size(ARRAY("..param.name.."_ptr)) * SZ_INT);")
    print("\tfor("..param.name.."_iter = 0; "..param.name.."_iter < m_vector_size(ARRAY("..param.name.."_ptr)); "..param.name.."_iter++) {")
    print("\t\t\tM_Object "..param.name.."_ftl_obj;")
    print("\t\t\tm_vector_get(ARRAY("..param.name.."_ptr), "..param.name.."_iter, &"..param.name.."_ftl_obj);")
    print("\t\t\t"..param.name.."["..param.name.."_iter] = FTBL("..param.name.."_ftl_obj);\n\t}")
    print("\trelease("..param.name.."_ptr, shred);")
    print("\tREM_REF("..param.name.."_ptr->type_ref);")
  elseif string.match(param.type, "&sp_ftbl%s*") then
    print("\tM_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);")
    print("\tsp_ftbl** "..param.name.." = &FTBL("..param.name.."_obj);")
    print("\trelease("..param.name.."_obj, shred);")
  elseif string.match(param.type, "sp_ftbl%s*") then
    print("\tM_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);")
    print("\tsp_ftbl* "..param.name.." = FTBL("..param.name.."_obj);")
    print("\trelease("..param.name.."_obj, shred);")
  else
    print("unknown type:", param.type, ".")
    os.exit(1)
  end
  if offset == false then
    print("\tgw_offset +="..increment..";")
  end
end

function declare_gw_param(param)
  if string.match(param.type, "int") then
    print("\t\t gwi_func_arg(gwi, \"int\", \""..param.name.."\");")
  elseif string.match(param.type, "SPFLOAT") then
    print("\t\t gwi_func_arg(gwi, \"float\", \""..param.name.."\");")
  elseif string.match(param.type, "char*") then
    print("\t\t gwi_func_arg(gwi, \"string\", \""..param.name.."\");")
  elseif string.match(param.type, "sp_ftbl%s%*%*") then
    print("\t\t gwi_func_arg(gwi, \"ftbl[]\", \""..param.name.."\");")
  elseif string.match(param.type, "sp_ftbl%*%*") then
    print("\t\t gwi_func_arg(gwi, \"ftbl[]\", \""..param.name.."\");")
  elseif string.match(param.type, "sp_ftbl%s%*") then
    print("\t\t gwi_func_arg(gwi, \"ftbl\", \""..param.name.."\");")
  elseif string.match(param.type, "sp_ftbl%*") then
    print("\t\t gwi_func_arg(gwi, \"ftbl\", \""..param.name.."\");")
  else print("unknown "..param.type)
    os.exit(1);
  end
  -- make_doc("\t\targ", param)
end

function print_gen_func(name, func)
  print("MFUN(ftbl_"..name..") {")
  print("\tsp_ftbl* ftbl = FTBL(o);")
  if(func.params ~= nil) then
    print("\tm_uint gw_offset = SZ_INT*2;")
  end
  print("\tif(FTBL(o))\n    sp_ftbl_destroy(&ftbl);")
  print("\tm_int size = *(m_int*)(shred->mem + SZ_INT);")
  local args = "";
  if(func.params ~= nil) then
    for i, v in pairs(func.params) do
      --		while func.params[i]  do
      declare_c_param(v, i == #func.params)
      args =	string.format("%s, %s", args , v.name)
    end
  end
  print("\tCHECK_SIZE(size);")
  print("\tsp_ftbl_create(shred->vm_ref->sp, &ftbl, size);")
  print("\tsp_"..name.."(shred->vm_ref->sp, ftbl"..args..");")
  print("\tFTBL(o) = ftbl;")
  --	print("error:\n\tsp_ftbl_destroy(&ftbl);")
  print("}\n")
end


function print_mod_func(name, mod)
  if string.match(name, "^foo$") then
    return
  end
  local ntrig = 0
  local ninputs = mod.ninputs
  local nmandatory = 0
  local tbl = mod.params.mandatory
  if tbl then
    for _, v in pairs(tbl) do
      nmandatory = nmandatory + 1
    end
  end
  for i = 1, ninputs  do
    if string.match(mod.inputs[i].name, "trig") then
      ntrig = ntrig + 1
      ninputs  = ninputs - 1
    end
  end
  print("typedef struct {\n\tsp_data* sp;\n\tsp_"..name.."* osc;")
  if(nmandatory > 0) then
    print("\tm_bool is_init;")
    local tbl = mod.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("\tsp_ftbl** "..v.name..";\n")
        end
      end
    end
  end
  print("} GW_"..name..";\n")
  print("static TICK("..name.."_tick) {")
  print("\tconst GW_"..name.."* ug = (GW_"..name.."*)u->module.gen.data;")
  if(nmandatory > 0) then
    print("\tif(!ug->is_init) { // LCOV_EXCL_START\n\t\tu->out = 0;\n\t\treturn;\n\t} // LCOV_EXCL_STOP")
  end
  local args = ""
  if ninputs > 1 or mod.noutputs > 1 then
    for i = 1, ninputs do
      args = string.format("%s, &UGEN(u->connect.multi->channel["..(i - 1).."])->in", args)
    end
  elseif ninputs == 1 then
    args = string.format("%s, &u->in", args)
  elseif ntrig == 0 and not string.match(name, "voc") then
    args = ", NULL"
  end
  if ntrig > 0 then
    args = string.format("%s, &u->module.gen.trig->in", args)
  end
  if mod.noutputs > 1 then
    for i = 1, mod.noutputs do
      args = string.format("%s, &UGEN(u->connect.multi->channel["..(i - 1).."])->out", args)
    end
  elseif mod.noutputs == 1 then
    args = string.format("%s, &u->out", args)
  end
  print("\tsp_"..name.."_compute(ug->sp, ug->osc"..args..");")
  print("\n}\n")
  print("CTOR("..name.."_ctor) {\n\tGW_"..name.."* ug = (GW_"..name.."*)xcalloc(1, sizeof(GW_"..name.."));")
  print("\tug->sp = shred->vm_ref->sp;")
  if(nmandatory > 0) then
    print("\tug->is_init = 0;")
    print("\tug->osc = NULL;")
  else
    --[=====[
    -- check errors
    print("\tif(sp_"..name.."_create(&ug->osc) == SP_NOT_OK) {\n\t\tfree(ug);\n\t\tExcept(shred);\n\t}")
    print("\tif(sp_"..name.."_init(ug->sp, ug->osc) == SP_NOT_OK) {\n\t\tfree(ug);\n\t\tExcept(shred);\n\t}")
    --]=====]
    print("\tsp_"..name.."_create(&ug->osc);")
    print("\tsp_"..name.."_init(ug->sp, ug->osc);")
  end
  print("\tugen_ini(UGEN(o), "..mod.ninputs..", "..mod.noutputs..");")
  print("\tugen_gen(UGEN(o), "..name.."_tick, ug, "..ntrig..");")
  print("}\n")
  print("DTOR("..name.."_dtor) {\n\tGW_"..name.."* ug = UGEN(o)->module.gen.data;")
  if(nmandatory > 0) then
    print("\tif(ug->is_init) {\n")
    local  arg = mod.params.mandatory
    if arg then
      for _, v in pairs(arg) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("\t\tfree(ug->osc->"..v.name..");\n")
        end
      end
      print("\t\tsp_"..name.."_destroy(&ug->osc);\n\t}")
    end
  else
    print("\tsp_"..name.."_destroy(&ug->osc);")
  end
  print("\tfree(ug);");
  print("}\n")
  if nmandatory > 0 then
    print("MFUN("..name.."_init) {")
    print("\tm_uint gw_offset = SZ_INT;")
    print("\tGW_"..name.."* ug = (GW_"..name.."*)UGEN(o)->module.gen.data;")
    local args = ""
    local tbl = mod.params.mandatory
    print("\tif(ug->osc) {\n\t\tsp_"..name.."_destroy(&ug->osc);")
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("\t\tfree(ug->"..v.name..");")
        end
      end
    end
    print("\t\tug->osc = NULL;\n\t}");
    if tbl then
      for i, v in pairs(tbl) do
        declare_c_param(v, i == #tbl)
        if string.match(args, "^$") then
          args = v.name
        else
          args = string.format("%s, %s", args, v.name)
        end
      end
    end
    local tbl = mod.params.mandatory
    print("\tif(sp_"..name.."_create(&ug->osc) == SP_NOT_OK || sp_"..name.."_init(ug->sp, ug->osc, "..args..") == SP_NOT_OK) {")
    local tbl = mod.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("\t\tfree("..v.name.."); // LCOV_EXCL_LINE")
        end
      end
    end
    print("\t\tExcept(shred, \"UgenCreateException\") // LCOV_EXCL_LINE\n\t}")
    local tbl = mod.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("\tug->"..v.name.." = "..v.name..";")
        end
      end
    end
    print("\tug->is_init = 1;\n}\n")
--    print("#ifdef JIT\nJIT_MFUN("..name.."_init)\n#endif\n")
  end
  local opt = mod.params.optional
  if opt then
    for _, v in pairs(opt) do
      print("MFUN("..name.."_get_"..v.name..") {")
      print("\tGW_"..name.."* ug = (GW_"..name.."*)UGEN(o)->module.gen.data;")
      if string.match(v.type, "int") then
        print("\t*(m_uint*)RETURN = ug->osc->"..v.name..";")
      elseif string.match(v.type, "SPFLOAT$") then
        print("\t*(m_float*)RETURN = ug->osc->"..v.name..";")
      elseif string.gmatch(v.type, "SPFLOAT*") then
        print("\t*(m_float*)RETURN = *ug->osc->"..v.name..";")
      elseif string.gmatch(v.type, "char*") then
        print("\t*(m_uint*)RETURN = ug->osc->"..v.name.."_obj;")
      elseif string.gmatch(v.type, "sp_ftbl*") then
        print("\t*(m_uint*)RETURN = ug->osc->"..v.name.."_obj;")
      else
        print("unknown type "..v.type)
        os.exit(1);
      end
      print("}\n")
      print("MFUN("..name.."_set_"..v.name..") {")
      print("\tm_uint gw_offset = SZ_INT;")
      print("\tGW_"..name.."* ug = (GW_"..name.."*)UGEN(o)->module.gen.data;")
      declare_c_param(v, true)
      if string.match(v.type, "int") then
        print("\t*(m_uint*)RETURN = (ug->osc->"..v.name.." = "..v.name..");")
      elseif string.match(v.type, "SPFLOAT$") then
        print("\t*(m_float*)RETURN = (ug->osc->"..v.name.." = "..v.name..");")
      elseif string.gmatch(v.type, "SPFLOAT*") then
        print("\t*(m_float*)RETURN = (*ug->osc->"..v.name.." = "..v.name..");")
      elseif string.gmatch(v.type, "char*") then
        print("\t*(m_uint*)RETURN = (ug->osc->"..v.name.."_obj = "..v.name..");")
      elseif string.gmatch(v.type, "sp_ftbl*") then
        print("\t*(m_uint*)RETURN = (ug->osc->"..v.name.."_obj = "..v.name..");")
      end
      print("}\n")
    end
  end
end

sptbl = {}

if not arg[1] then
  print("this script takes a valid soudpipe data directory as argument")
  os.exit(1)
end

--local dir = io.popen("dir "..arg[1])
local dir = io.popen("ls "..arg[1])
if dir then
  for filename in string.gmatch(dir:read("*a"), '([^%s]+)') do
    if filename:gmatch(".lua$") then
      dofile(arg[1]..'/'..filename)
    end
  end
else
  print("failed to read soundpipe data directory.")
  os.exit(1);
end

a = {}
for n in pairs(sptbl) do table.insert(a, n) end
table.sort(a)

print('#include <stdlib.h>\
#include "vm.h"\
#include "type.h"\
#include "err_msg.h"\
#include "instr.h"\
#include "import.h"\
#include "ugen.h"\
#include "func.h"\
#include "lang.h"\
#ifdef JIT\
#include "ctrl.h"\
#endif')

print("m_uint o_ftbl_data;")
print("#define FTBL(o) *((sp_ftbl**)((M_Object)o)->data + o_ftbl_data)")
print("#define CHECK_SIZE(size)\tif(size <= 0){fprintf(stderr, \"'gen_ftbl' size argument must be more than 0\");return;}")
print("\nDTOR(ftbl_dtor) {")
print("\tif(FTBL(o))\n\t\tsp_ftbl_destroy(&FTBL(o));")
print("}\n")
for n in ipairs(a) do
  local name = a[n]
  local object = sptbl[name]
  if string.match(object.modtype, "gen") then
    print_gen_func(name, object)
  elseif not string.match(name, "foo") then
    print_mod_func(name, object)
  end
end

print("")
print("m_bool import_soundpipe(Gwi gwi) {\n")
print("\tType t_ftbl = gwi_mk_type(gwi, \"ftbl\", SZ_INT, t_object);")
print("\tCHECK_BB(gwi_class_ini(gwi, t_ftbl, NULL, ftbl_dtor))")
print("\tCHECK_BB(gwi_item_ini(gwi, \"int\", \"@ftbl\"))")
print("\to_ftbl_data = gwi_item_end(gwi, 0, NULL);")
for n in ipairs(a) do
  local gen_name = a[n]
  local object = sptbl[gen_name]
  if string.match(object.modtype, "gen") then
    print("\tgwi_func_ini(gwi, \"void\", \""..gen_name.."\", ftbl_"..gen_name..");")
    local i = 1; -- why this 'i' ?
    print("\tgwi_func_arg(gwi, \"int\", \"size\");")
    if(object.params ~= nil) then
      while object.params[i]  do
        declare_gw_param(object.params[i])
        i = i+1
      end
    end
    print("\tCHECK_BB(gwi_func_end(gwi, 0))")
  end
end
print("\tCHECK_BB(gwi_class_end(gwi))\n")

for n in ipairs(a) do
  local mod_name = a[n]
  local object = sptbl[mod_name]
  if not string.match(object.modtype, "gen") and not string.match(mod_name, "foo")then
    local title = string.format("%s%s", string.upper(mod_name:sub(1, 1)), string.sub(mod_name, 2))
    print("\tconst Type t_"..mod_name.." = gwi_mk_type(gwi, \""..title.."\", SZ_INT, t_ugen);")
    print("\tCHECK_BB(gwi_class_ini(gwi, t_"..mod_name..", "..mod_name.."_ctor, "..mod_name.."_dtor))")
    local nmandatory = 0
    local tbl = object.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        nmandatory = nmandatory + 1
      end
    end
    if nmandatory > 0 then
      print("\tgwi_func_ini(gwi, \"void\", \"init\", "..mod_name.."_init);")
      local tbl = object.params.mandatory
      if tbl then
        for _, v in pairs(tbl) do
          declare_gw_param(v)
        end
      end
      print("\tCHECK_BB(gwi_func_end(gwi, 0))")
    end
    local tbl = object.params.optional
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "int") then
          print("\tgwi_func_ini(gwi, \"int\", \""..v.name.."\", "..mod_name.."_get_"..v.name..");")
        elseif string.match(v.type, "SPFLOAT") then
          print("\tgwi_func_ini(gwi, \"float\", \""..v.name.."\", "..mod_name.."_get_"..v.name..");")
        elseif string.match(v.type, "char") then
          print("\tgwi_func_ini(gwi, \"string\", \""..v.name.."\", "..mod_name.."_get_"..v.name..");")
        elseif string.match(v.type, "sp_ftbl%s%*%*") then
          print("\tgwi_func_ini(gwi, \"ftbl[]\", \""..v.name.."\", "..mod_name.."_get_"..v.name..");")
        elseif string.match(v.type, "sp_ftbl%s%*") then
          print("\tgwi_func_ini(gwi, \"ftbl\", \""..v.name.."\", "..mod_name.."_get_"..v.name..");")
        end
        print("\tCHECK_BB(gwi_func_end(gwi, 0))")
        if string.match(v.type, "int") then
          print("\tgwi_func_ini(gwi, \"int\", \""..v.name.."\", "..mod_name.."_set_"..v.name..");")
        elseif string.match(v.type, "SPFLOAT") then
          print("\tgwi_func_ini(gwi, \"float\", \""..v.name.."\", "..mod_name.."_set_"..v.name..");")
        elseif string.match(v.type, "char") then
          print("\tgwi_func_ini(gwi, \"string\", \""..v.name.."\", "..mod_name.."_set_"..v.name..");")
        elseif string.match(v.type, "sp_ftbl%s%*%*") then
          print("\tgwi_func_ini(gwi, \"ftbl[]\", \""..v.name.."\", "..mod_name.."_set_"..v.name..");")
        elseif string.match(v.type, "sp_ftbl%s%*") then
          print("\tgwi_func_ini(gwi, \"ftbl\", \""..v.name.."\", "..mod_name.."_set_"..v.name..");")
        end
        declare_gw_param(v)
        print("\tCHECK_BB(gwi_func_end(gwi, 0))")
      end
    end
    print("\tCHECK_BB(gwi_class_end(gwi))\n")
  end
end
print("\treturn 1;\n}")
