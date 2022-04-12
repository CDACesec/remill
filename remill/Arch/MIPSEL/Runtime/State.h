/*
 * Copyright (c) 2017 Trail of Bits, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Reviewed and Modified for use by Centre for Development of Advanced Computing (C-DAC)
 */

#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wpadded"

#include "remill/Arch/Runtime/State.h"
#include "remill/Arch/Runtime/Types.h"

struct Reg final {
  uint32_t word;
}__attribute__((packed));

static_assert(sizeof(uint32_t) == sizeof(Reg), "Invalid packing of `Reg`.");

static_assert(0 == __builtin_offsetof(Reg, word),
               "Invalid packing of `Reg::word`.");


union VPEControl_Register final {
  uint32_t flat;
  struct {
    uint32_t TargTc: 8;
    uint32_t _0: 7;
    uint32_t TE: 1; /* Threads Enabled */
    uint32_t EXCPT: 3; /*Exception sub-code*/
    uint32_t __0: 1;
    uint32_t GSI: 1; /*Gating Storage Scheduler Intercept*/
    uint32_t YSI: 1; /*YIELD Scheduler Intercept*/
    uint32_t ___0: 10;
  }__attribute__((packed));
}__attribute__((packed));

union MVPControl_Register final {
  uint32_t flat;
  struct {
    uint32_t EVP: 1;  /*Enable Virtual Processors.*/
    uint32_t VPC: 1;  /*VPE Configuration State*/
    uint32_t STLB: 1; /* Share TLBs.*/
    uint32_t CPA: 1;  /* Cache Partitioning Active*/
    uint32_t _0: 28;  /*Must be written as zero; return zero on read.*/
  }__attribute__((packed));
}__attribute__((packed));


struct FReg final {                                                     
  float32_t word;
}__attribute__((packed));

static_assert(sizeof(float32_t) == sizeof(FReg), "Invalid packing of `FReg`.");

static_assert(0 == __builtin_offsetof(FReg, word),
               "Invalid packing of `FReg::word`.");

struct DReg final {
  uint64_t dword;
}__attribute__((packed));

static_assert(sizeof(uint64_t) == sizeof(DReg), "Invalid packing of `Reg`.");

static_assert(0 == __builtin_offsetof(DReg, dword),
               "Invalid packing of `Reg::word`.");

struct FPReg final {
  float32_t word; /* single precision float */

  // double precision requires using pairs of
  // single precision, even numbered regs.
  // odd numbered regs are included automatically
}__attribute__((packed));

static_assert(sizeof(float32_t) == sizeof(FPReg), "Invalid packing of `Reg`.");
//add some checks for this


struct alignas(4) CP0 final { // System Control Coprocessor 0
  VPEControl_Register VPEC_R;
  MVPControl_Register MVPC_R;
  uint32_t TCRestart;
}__attribute__((packed));
static_assert(12 == sizeof(CP0), "Invalid packing of `CP0`.");

struct  CP1 final { // floating point Co-Processor 1
  //volatile uint32_t _0;
  union {
    float64_t DReg; /* double precision uses two regs together*/
    struct  {
      FPReg f0; /* for single precision */
      FPReg f1; /* for single precision*/
    };
  }D0;
  union {
    float64_t DReg;
    struct  {
      FPReg f2;
      FPReg f3;
    };
  }D2;
  union {
    float64_t DReg;
    struct  {
      FPReg f4;
      FPReg f5;
    };
  }D4;
  union {
    float64_t DReg;
    struct  {
      FPReg f6;
      FPReg f7;
    };
  }D6;
  union {
    float64_t DReg;
    struct  {
      FPReg f8;
      FPReg f9;
    };
  }D8;
  union {
    float64_t DReg;
    struct  {
      FPReg f10;
      FPReg f11;
    };
  }D10;
  union {
    float64_t DReg;
    struct  {
      FPReg f12;
      FPReg f13;
    };
  }D12;
  union {
    float64_t DReg;
    struct  {
      FPReg f14;
      FPReg f15;
    };
  }D14;
  union {
    float64_t DReg;
    struct  {
      FPReg f16;
      FPReg f17;
    };
  }D16;
  union {
    float64_t DReg;
    struct  {
      FPReg f18;
      FPReg f19;
    };
  }D18;
  union {
    float64_t DReg;
    struct  {
      FPReg f20;
      FPReg f21;
    };
  }D20;
  union {
    float64_t DReg;
    struct  {
      FPReg f22;
      FPReg f23;
    };
  }D22;
  union {
    float64_t DReg;
    struct  {
      FPReg f24;
      FPReg f25;
    };
  }D24;
  union {
    float64_t DReg;
    struct  {
      FPReg f26;
      FPReg f27;
    };
  }D26;
  union {
    float64_t DReg;
    struct  {
      FPReg f28;
      FPReg f29;
    };
  }D28;
  union {
    float64_t DReg;
    struct  {
      FPReg f30;
      FPReg f31;
    };
  }D30;
}__attribute__((packed));

struct CP2 final {
  DReg zero;
  DReg at;
  DReg v0;
  DReg v1;
  DReg a0;
  DReg a1;
  DReg a2;
  DReg a3;
  DReg t0;
  DReg t1;
  DReg t2;
  DReg t3;
  DReg t4;
  DReg t5;
  DReg t6;
  DReg t7;
  DReg s0;
  DReg s1;
  DReg s2;
  DReg s3;
  DReg s4;
  DReg s5;
  DReg s6;
  DReg s7;
  DReg t8;
  DReg t9;
  DReg k0;
  DReg k1;
  DReg gp;
  DReg sp;
  DReg fp;
  DReg ra;
  DReg pc;

}__attribute__((packed));


struct alignas(4) GPR final {
  Reg zero; //always equal to zero
  Reg at; // assembler temporary, used by the assembler
  Reg v0; //return value from a function call
  Reg v1; //return value from a function call
  Reg a0; // first four parameters of a function call
  Reg a1; // first four parameters of a function call
  Reg a2; // first four parameters of a function call
  Reg a3; // first four parameters of a function call
  Reg t0; // Temp Vars, need not be preserved
  Reg t1; // Temp Vars, need not be preserved
  Reg t2; // Temp Vars, need not be preserved
  Reg t3; // Temp Vars, need not be preserved
  Reg t4; // Temp Vars, need not be preserved
  Reg t5; // Temp Vars, need not be preserved
  Reg t6; // Temp Vars, need not be preserved
  Reg t7; // Temp Vars, need not be preserved
  Reg s0; //Function variables, must be preserved
  Reg s1; //Function variables, must be preserved
  Reg s2; //Function variables, must be preserved
  Reg s3; //Function variables, must be preserved
  Reg s4; //Function variables, must be preserved
  Reg s5; //Function variables, must be preserved
  Reg s6; //Function variables, must be preserved
  Reg s7; //Function variables, must be preserved
  Reg t8; //Two more temporary variables
  Reg t9; //Two more temporary variables
  Reg k0; // Kernel use regs, may change unexpectedly
  Reg k1; // Kernel use regs, may change unexpectedly
  Reg gp; // global pointer
  Reg sp; // Stack pointer
  Reg fp; //or s8. Stack frame pointer or subroutine variable.
  Reg ra;  // return address of the last subroutine call.
  Reg pc;  // Program counter of the CURRENT instruction!

}__attribute__((packed));

struct alignas(4) FPR final {          // coprocessor 1 floating point registers 

  
  FReg f0; 
  
  FReg f1; 
  
  FReg f2; 
  
  FReg f3; 

  FReg f4; 
  
  FReg f5; 
  
  FReg f6; 
 
  FReg f7; 
  
  FReg f8; 
  
  FReg f9; 
  
  FReg f10;
 
  FReg f11; 
  
  FReg f12; 
  
  FReg f13; 
  
  FReg f14; 
  
  FReg f15; 
  
  FReg f16; 
  
  FReg f17;
  
  FReg f18; 
 
  FReg f19; 
  
  FReg f20; 
  
  FReg f21; 
  
  FReg f22; 
  
  FReg f23; 
  
  FReg f24; 
  
  FReg f25;
  
  FReg f26; 
  
  FReg f27; 
 
  FReg f28; 
  
  FReg f29;
  
  FReg f30; 
                                                                         
  FReg f31;  
  

}__attribute__((packed));



struct alignas(4) SPR final {
  Reg HI;
  Reg LO;
}__attribute__((packed));

struct alignas(4) HINT {                     

   uint32_t load:1;
   uint32_t store:1;
   uint32_t reserved:2;
   uint32_t load_streamed:1;
   uint32_t store_streamed:1;
   uint32_t load_retained:1;
   uint32_t store_retained:1;
   uint32_t reserved_1:13;
   uint32_t implementation_dependent:4;
   uint32_t writeback_invalidate:1;
   uint32_t implementation_dependent_1:4;
   uint32_t prepareforstore:1;
   uint32_t implementation_dependent_2:1;
}__attribute__((packed));

struct alignas(4) FCC
{
   uint32_t cc;
}__attribute__((packed));

struct alignas(4) COP2CC               // coprocessor 2 condition code 
{
   uint32_t cc;
}__attribute__((packed));

struct alignas(4) COP2CR              // coprocessor 2 control register  
{
   uint32_t cr;
}__attribute__((packed));

struct alignas(4) COP2GR               // coprocessor 2 general register(32 bit)
{
   uint32_t gr;
}__attribute__((packed));


static_assert(8 == sizeof(SPR), "Invalid structure packing of `SPR`.");


struct alignas(16) State final : public ArchState { // ArchState is aligned to 16

  CP0 cp0;
  uint32_t _0;

  CP1 cp1;
  
  GPR gpr;

  FPR fpr;  

  SPR spr;
    
  CP2 cp2;
  uint32_t _1;
  uint32_t _2; 

  COP2CR cop2cr;
  uint32_t _3;
  uint32_t _4;
  uint32_t _5;

  COP2GR cop2gr;
  uint32_t _6;
  uint32_t _7;
  uint32_t _8;

  COP2CC cop2cc;               // COP2 condition code
  uint32_t _9;
  uint32_t _10;
  uint32_t _11;

  HINT hint;                 // Hint field for prefetch instruction
  uint32_t _12;
  uint32_t _13;
  uint32_t _14;

  FCC fcc;
  uint64_t _15; //alignment
  uint32_t _16; //alignment
  // uint32_t _3;
  uint32_t TLS_BASE;    
    
  // FCSR fcsr;
  // uint32_t _4;
  // uint64_t _5;

}__attribute__((packed));

using MIPSELState = State;

#pragma clang diagnostic pop
