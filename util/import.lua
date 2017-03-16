function make_doc(prefix, o)
	local tmp = string.gsub(o.description, "\"", "\\\"")
	local doc = string.gsub(tmp, "\n", "")
	print(prefix.."->doc = \""..doc.."\";")
end

function declare_c_param(param)
	local type;
	if string.match(param.type, "int") then
		print("\tm_int "..param.name.." = *(m_int*)(shred->mem + gw_offset);\n\tgw_offset += SZ_INT;")
	elseif string.match(param.type, "SPFLOAT$") then
		print("\tm_float "..param.name.." = *(m_float*)(shred->mem + gw_offset);\n\tgw_offset += SZ_FLOAT;")
	elseif string.match(param.type, "SPFLOAT*") then
		print("\tm_float "..param.name.." = *(m_float*)(shred->mem + gw_offset);\n\tgw_offset += SZ_FLOAT;")
	elseif string.match(param.type, "char%s*") then
		print("\tM_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);\n\tgw_offset += SZ_INT;")
		print("\tm_str "..param.name.." = STRING("..param.name.."_obj);")
		print("\trelease("..param.name.."_obj, shred);")
	elseif string.match(param.type, "sp_ftbl%s%*%*") then
		print("\tM_Object "..param.name.."_ptr = *(M_Object*)(shred->mem + gw_offset);\n\tgw_offset += SZ_INT;")
		print("\tm_uint "..param.name.."_iter;")
		print("\tsp_ftbl* "..param.name.."[m_vector_size("..param.name.."_ptr->d.array)];")
		print("\tfor("..param.name.."_iter = 0; "..param.name.."_iter < m_vector_size("..param.name.."_ptr->d.array); "..param.name.."_iter++)")
		print("\t\t"..param.name.."["..param.name.."_iter] = FTBL((M_Object)i_vector_at("..param.name.."_ptr->d.array, "..param.name.."_iter));")
	elseif string.match(param.type, "&sp_ftbl%s*") then
		print("\tM_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);\n\tgw_offset+=SZ_INT;")
		print("\tsp_ftbl** "..param.name.." = &FTBL("..param.name.."_obj);")
	elseif string.match(param.type, "sp_ftbl%s*") then
		print("\tM_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);\n\tgw_offset+=SZ_INT;")
		print("\tsp_ftbl* "..param.name.." = FTBL("..param.name.."_obj);")
		print("\trelease("..param.name.."_obj, shred);")
		print("\tif(!"..param.name..") {\n\t\trelease(o, shred);\n\t\tExcept(shred)\n}")
	else
		print("unknown type:", param.type, ".")
		os.exit(1)
	end
--	print("gw_offset;")
end

function declare_gw_param(param)
	if string.match(param.type, "int") then
		print("\t\targ = dl_func_add_arg(fun, \"int\", \""..param.name.."\");")
	elseif string.match(param.type, "SPFLOAT") then
		print("\t\targ = dl_func_add_arg(fun, \"float\", \""..param.name.."\");")
	elseif string.match(param.type, "char*") then
		print("\t\targ = dl_func_add_arg(fun, \"string\", \""..param.name.."\");")
	elseif string.match(param.type, "sp_ftbl%s%*%*") then
		print("\t\targ = dl_func_add_arg(fun, \"ftbl[]\", \""..param.name.."\");")
	elseif string.match(param.type, "sp_ftbl%*%*") then
		print("\t\targ = dl_func_add_arg(fun, \"ftbl[]\", \""..param.name.."\");")
	elseif string.match(param.type, "sp_ftbl%s%*") then
		print("\t\targ = dl_func_add_arg(fun, \"ftbl\", \""..param.name.."\");")
	elseif string.match(param.type, "sp_ftbl%*") then
		print("\t\targ = dl_func_add_arg(fun, \"ftbl\", \""..param.name.."\");")
else print("unknown "..param.type)
os.exit(1);
	end
	make_doc("\t\targ", param)
end

function print_gen_func(name, func)
	print("MFUN(ftbl_"..name..")\n{")
	print("\tsp_ftbl* ftbl = FTBL(o);")
if(func.params ~= nil) then
	print("\tm_uint gw_offset = SZ_INT*2;")
end
	print("\tif(FTBL(o))\n    sp_ftbl_destroy(&ftbl);")
	print("\tm_int size = *(m_int*)(shred->mem + SZ_INT);")
	print("\tCHECK_SIZE(size);")
	print("\tsp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);")
	local i = 1;
	local args = "";
	if(func.params ~= nil) then
		while func.params[i]  do
			declare_c_param(func.params[i])
			args =	string.format("%s, %s", args , func.params[i].name)
			i = i+1
		end
	end
	print("\tsp_"..name.."(shred->vm_ref->bbq->sp, ftbl"..args..");")
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
	print("typedef struct\n{\n\tsp_data* sp;\n\tsp_"..name.."* osc;")
	if(nmandatory > 0) then
		print("\tm_bool is_init;")
	end
	print("} GW_"..name..";\n")
	print("TICK("..name.."_tick)\n{")
	print("\tGW_"..name.."* ug = (GW_"..name.."*)u->ug;")
  if(nmandatory > 0) then
		print("\tif(!ug->is_init)\n\t{\n\t\tu->out = 0;\n\t\treturn 1;\n\t}")
	end
	local args = ""
	if ninputs > 1 then
		for i = 1, ninputs do
			args = string.format("%s, &u->channel["..(i - 1).."]->ugen->in", args)
		end	
	elseif ninputs == 1 then
		args = string.format("%s, &u->in", args)
	elseif ntrig == 0 then
		args = ", NULL"
	end
	if ntrig > 0 then
		args = string.format("%s, &u->trig->ugen->out", args)
	end
	if mod.noutputs > 1 then
		for i = 1, mod.noutputs do
			args = string.format("%s, &u->channel["..(i - 1).."]->ugen->out", args)
		end	
	elseif mod.noutputs == 1 then
		args = string.format("%s, &u->out", args)
	end
	print("\tsp_"..name.."_compute(ug->sp, ug->osc"..args..");")
	print("\treturn 1;\n}\n")
	print("CTOR("..name.."_ctor)\n{\n\tGW_"..name.."* ug = malloc(sizeof(GW_"..name.."));")
	print("\tug->sp = shred->vm_ref->bbq->sp;")
    if(nmandatory > 0) then
		print("\tug->is_init = 0;")
		print("\tug->osc = NULL;")
	else
		print("\tsp_"..name.."_create(&ug->osc);")
		print("\tsp_"..name.."_init(ug->sp, ug->osc);")
	end
	print("\to->ugen->tick = "..name.."_tick;")
	print("\tassign_ugen(o->ugen, "..mod.ninputs..", "..mod.noutputs..", "..ntrig..", ug);")
	print("}\n")
	print("DTOR("..name.."_dtor)\n{\n\tGW_"..name.."* ug = o->ugen->ug;")
	if(nmandatory > 0) then
		print("\tif(ug->is_init)\n\t\tsp_"..name.."_destroy(&ug->osc);")
	else
		print("\tsp_"..name.."_destroy(&ug->osc);")
	end
	print("\tfree(ug);");
	print("}\n")
	if nmandatory > 0 then
		print("MFUN("..name.."_init)\n{")
		print("\tm_uint gw_offset = SZ_INT;")
		print("\tGW_"..name.."* ug = (GW_"..name.."*)o->ugen->ug;")
		local args = ""
		local tbl = mod.params.mandatory
		if tbl then
			for _, v in pairs(tbl) do
				declare_c_param(v)
				if string.match(args, "^$") then
					args = v.name
				else
					args = string.format("%s, %s", args, v.name)
				end
			end
		end
		print("\tif(ug->osc) {\n\t\tsp_"..name.."_destroy(&ug->osc);\n\t\tug->osc = NULL;\t}");
		print("\tsp_"..name.."_create(&ug->osc);")
		print("\tsp_"..name.."_init(ug->sp, ug->osc, "..args..");")
		print("\tug->is_init = 1;\n}\n")
	end
-- helper
--	if nmandatory == 1 then
--		local tbl = mod.params.mandatory
--		if tbl then
--			for _, v in pairs(tbl) do
--				print("MFUN("..name.."_"..v.name..")\n{")
--				print("\tm_uint gw_offset = SZ_INT;")
--				print("\tGW_"..name.."* ug = (GW_"..name.."*)o->ugen->ug;")
--				declare_c_param(v)
--				print("\tsp_"..name.."_init(ug->sp, ug->osc, "..v.name..");")
--				print("}\n")
--			end
--		end
--	end
	local opt = mod.params.optional
	if opt then
		for _, v in pairs(opt) do
			print("MFUN("..name.."_get_"..v.name..")\n{")
			print("\tGW_"..name.."* ug = (GW_"..name.."*)o->ugen->ug;")
			if string.match(v.type, "int") then
				print("\tRETURN->d.v_uint = ug->osc->"..v.name..";")
			elseif string.match(v.type, "SPFLOAT$") then
				print("\tRETURN->d.v_float = ug->osc->"..v.name..";")
			elseif string.gmatch(v.type, "SPFLOAT*") then
				print("\tRETURN->d.v_float = *ug->osc->"..v.name..";")
			elseif string.gmatch(v.type, "char*") then
				print("\tRETURN->d.v_uint = ug->osc->"..v.name.."_obj;")
			elseif string.gmatch(v.type, "sp_ftbl*") then
				print("\tRETURN->d.v_uint = ug->osc->"..v.name.."_obj;")
			else 
				print("unknown type "..v.type)
				os.exit(1);
			end
			print("}\n")
			print("MFUN("..name.."_set_"..v.name..")\n{")
			print("\tm_uint gw_offset = SZ_INT;")
			print("\tGW_"..name.."* ug = (GW_"..name.."*)o->ugen->ug;")
			declare_c_param(v)
			if string.match(v.type, "int") then
				print("\tRETURN->d.v_uint = (ug->osc->"..v.name.." = "..v.name..");")
			elseif string.match(v.type, "SPFLOAT$") then
				print("\tRETURN->d.v_float = (ug->osc->"..v.name.." = "..v.name..");")
			elseif string.gmatch(v.type, "SPFLOAT*") then
				print("\tRETURN->d.v_float = (*ug->osc->"..v.name.." = "..v.name..");")
			elseif string.gmatch(v.type, "char*") then
				print("\tRETURN->d.v_uint = (ug->osc->"..v.name.."_obj = "..v.name..");")
			elseif string.gmatch(v.type, "sp_ftbl*") then
				print("\tRETURN->d.v_uint = (ug->osc->"..v.name.."_obj = "..v.name..");")
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

print('#include "vm.h"\
#include "type.h"\
#include "dl.h"\
#include "err_msg.h"\
#include "import.h"\
#include "ugen.h"\
#include "bbq.h"\
#include "array.h"\
#include "func.h"\
#include "lang.h"\
#include "object.h"')

print("m_uint o_ftbl_data;")
print("#define FTBL(o) *((sp_ftbl**)((M_Object)o)->d.data + o_ftbl_data)")
print("#define CHECK_SIZE(size)\tif(size <= 0){fprintf(stderr, \"'gen_ftbl' size argument must be more than 0\");return;}")

print("\nDTOR(ftbl_dtor)\n{")
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

	print("struct Type_ t_ftbl = {\"ftbl\", SZ_INT, &t_object};")
for n in ipairs(a) do
	local name = a[n]
	local object = sptbl[name]
	local title = string.format("%s%s", string.upper(name:sub(1, 1)), string.sub(name, 2))
	if not string.match(object.modtype, "gen") and not string.match(name, "foo") then
		print("struct Type_ t_"..name.." = {\""..title.."\", SZ_INT, &t_ugen};")
	end
end
print("")
print("m_bool import_soundpipe(Env env)\n{\n\tDL_Func* fun;\n\tDL_Value* arg;\n\tFunc f;\n")
print("\tCHECK_BB(add_global_type(env, &t_ftbl))")
print("\tCHECK_OB(import_class_begin(env, &t_ftbl, env->global_nspc, NULL, ftbl_dtor))")
print("\to_ftbl_data = import_mvar(env, \"int\", \"@ftbl\", 1, 0, \"internal data for ftbl\");")
for n in ipairs(a) do
	local gen_name = a[n]
	local object = sptbl[gen_name]
	if string.match(object.modtype, "gen") then
		print("\tfun = new_DL_Func(\"void\", \""..gen_name.."\", (m_uint)ftbl_"..gen_name..");")
		local i = 1; -- why this 'i' ?
		print("\t\targ = dl_func_add_arg(fun, \"int\", \"size\");")
		if(object.params ~= nil) then
			while object.params[i]  do
				declare_gw_param(object.params[i])
				i = i+1
			end
		end
		print("\tCHECK_OB((f = import_mfun(env, fun)))")
		make_doc("\tf", object)
	end
end
print("env->class_def->doc = \"soudpipe float array type\";")
--			make_doc("f", mod_name)
print("\tCHECK_BB(import_class_end(env))\n")

for n in ipairs(a) do
	local mod_name = a[n]
	local object = sptbl[mod_name]
	if not string.match(object.modtype, "gen") and not string.match(mod_name, "foo")then
		print("\tCHECK_BB(add_global_type(env, &t_"..mod_name.."))")
		print("\tCHECK_OB(import_class_begin(env, &t_"..mod_name..", env->global_nspc, "..mod_name.."_ctor, "..mod_name.."_dtor))")
		local nmandatory = 0
		local tbl = object.params.mandatory
		if tbl then
			for _, v in pairs(tbl) do
				nmandatory = nmandatory + 1
			end
		end
-- helper.
--		if nmandatory == 1 then
--			for _, v in pairs(tbl) do
--				print("\tfun = new_DL_Func(\"void\", \""..v.name.."\", (m_uint)"..mod_name.."_init);")
--				declare_gw_param(v)
--			end
--			print("\tCHECK_OB((f = import_mfun(env, fun)))")
--			make_doc("\tf", object)
--		end
		if nmandatory > 0 then
				print("\tfun = new_DL_Func(\"void\", \"init\", (m_uint)"..mod_name.."_init);")
			local tbl = object.params.mandatory
			if tbl then
				for _, v in pairs(tbl) do
				declare_gw_param(v)
				end
			end	
			print("\tCHECK_OB((f = import_mfun(env, fun)))")
			make_doc("\tf", object)
		end
			local tbl = object.params.optional
			if tbl then
				for _, v in pairs(tbl) do
				if string.match(v.type, "int") then
					print("\tfun = new_DL_Func(\"int\", \""..v.name.."\", (m_uint)"..mod_name.."_get_"..v.name..");")
				elseif string.match(v.type, "SPFLOAT") then
					print("\tfun = new_DL_Func(\"float\", \""..v.name.."\", (m_uint)"..mod_name.."_get_"..v.name..");")
				elseif string.match(v.type, "char") then
					print("\tfun = new_DL_Func(\"string\", \""..v.name.."\", (m_uint)"..mod_name.."_get_"..v.name..");")
				elseif string.match(v.type, "sp_ftbl%s%*%*") then
					print("\tfun = new_DL_Func(\"ftbl[]\", \""..v.name.."\", (m_uint)"..mod_name.."_get_"..v.name..");")
				elseif string.match(v.type, "sp_ftbl%s%*") then
					print("\tfun = new_DL_Func(\"ftbl\", \""..v.name.."\", (m_uint)"..mod_name.."_get_"..v.name..");")
				end
				print("\tCHECK_OB((f = import_mfun(env, fun)))")
				make_doc("\tf", v)
				if string.match(v.type, "int") then
					print("\tfun = new_DL_Func(\"int\", \""..v.name.."\", (m_uint)"..mod_name.."_set_"..v.name..");")
				elseif string.match(v.type, "SPFLOAT") then
					print("\tfun = new_DL_Func(\"float\", \""..v.name.."\", (m_uint)"..mod_name.."_set_"..v.name..");")
				elseif string.match(v.type, "char") then
					print("\tfun = new_DL_Func(\"string\", \""..v.name.."\", (m_uint)"..mod_name.."_set_"..v.name..");")
				elseif string.match(v.type, "sp_ftbl%s%*%*") then
					print("\tfun = new_DL_Func(\"ftbl[]\", \""..v.name.."\", (m_uint)"..mod_name.."_set_"..v.name..");")
				elseif string.match(v.type, "sp_ftbl%s%*") then
					print("\tfun = new_DL_Func(\"ftbl\", \""..v.name.."\", (m_uint)"..mod_name.."_set_"..v.name..");")
				end
				declare_gw_param(v)
				print("\tCHECK_OB((f = import_mfun(env, fun)))")
				make_doc("\tf", v)
				end
			end	
		make_doc("\tenv->class_def", object)
		print("\tCHECK_BB(import_class_end(env))\n")
	end
end
print("\treturn 1;\n}")
