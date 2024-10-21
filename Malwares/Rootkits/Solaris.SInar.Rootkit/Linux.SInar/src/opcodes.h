/*
 * Copyright (c) 2004 by Archim
 * All rights reserved.
 *
 * For License information please see LICENSE (that was unexpected wasn't it!).
 *
 *
 */

#ifndef __i386
struct sethi_opcode
{
  unsigned op:2;
  unsigned regd:5;
  unsigned op2:3;
  unsigned imm:22;
};

typedef struct sethi_opcode sethi_t;

struct or_opcode
{

  unsigned op:2;
  unsigned regd:5;
  unsigned op3:6;
  unsigned rs1:5;
  unsigned i_fl:1;
  unsigned imm:13;

};

typedef struct or_opcode or_t;



struct nop_opcode
{
  unsigned nopc:32;
};

typedef struct nop_opcode nop_t;

struct jmp_opcode {
  unsigned start:2;
  unsigned regdest:5;
  unsigned op3:6;
  unsigned rs1:5;
  unsigned i_fl:1;
  unsigned simm13:13;
};

typedef struct jmp_opcode jmp_t;

  sethi_t sethop;
  or_t orop;
  jmp_t jop;
  nop_t nop;

#else
// come on I did some of the work for you.
struct jmpl_opcode{
unsigned offs:16; 
unsigned dest:32;
  unsigned sig:8;
};

struct nop_opcode{
  unsigned nopc:8;
};

typedef struct jmpl_opcode jmpl_t;
typedef struct nop_opcode nop_t;


jmpl_t jmpl_op;
nop_t nop_op;

#endif
