#!/bin/sh
#use this to use defines

#this for INSTR
ARG=" *VM *\* *vm, *VM_Shred *shred, *Instr *instr *"
sed -i "/$ARG/ s/void /INSTR(/" $1
sed -i "/$ARG/ s/($ARG//" $1

#this for MFUN
ARG=" *M_Object *o,  *DL_Return *\* *RETURN, *VM_Shred *shred *"
sed -i "/$ARG/ s/void /MFUN(/" $1
sed -i "/$ARG/ s/($ARG//" $1

#this for SFUN
ARG=" *DL_Return *\* *RETURN, *VM_Shred *shred *"
sed -i "/$ARG/ s/void /SFUN(/" $1
sed -i "/$ARG/ s/($ARG//" $1

#this for TICK
#ARG=" *UGen *u *"
#sed -i "/$ARG/ s/m_bool /TICK(/" $1
#sed -i "/$ARG/ s/($ARG//" $1

#this for IMPORT
#ARG=" *Env *env *"
#sed -i "/$ARG/ s/m_bool /IMPORT(/" $1
#sed -i "/$ARG/ s/($ARG//" $1
sed -i "s/m_bool import(Env env)/IMPORT/" $1

#this for CTOR
#ARG=" *M_Object *o, *VM_Shred *shred *"
#sed -i "/ctor/ s/void /CTOR(/" $1
#sed -i "/$ARG/ s/($ARG//" $1

#this for DTOR
#ARG=" *M_Object *o, *VM_Shred *shred *"
#sed -i "/dtor/ s/void /DTOR(/" $1
#sed -i "/$ARG/ s/($ARG//" $1
