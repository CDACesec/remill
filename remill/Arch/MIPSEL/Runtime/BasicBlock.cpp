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

#include <algorithm>
#include <bitset>
#include <cmath>

#include "remill/Arch/Float.h"
#include "remill/Arch/MIPSEL/Runtime/State.h"

extern "C" {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

// Instructions will be lifted into clones of this function.
[[gnu::used]] Memory *__remill_basic_block(State &state, addr_t curr_pc,
                                           Memory *memory) {
  bool branch_taken = false; // needed for conditional branches
  addr_t monitor = 0; 

  // Note: These variables MUST be defined for all architectures.
  auto &STATE = state;
  auto &MEMORY = *memory;
  auto &PC = state.gpr.pc.word;
  auto &BRANCH_TAKEN = branch_taken;

  // `PC` should already have the correct value, but it's nice to make sure
  // that `curr_pc` is used throughout, as it helps with certain downstream
  // uses to be able to depend on the optimizer not eliminating `curr_pc`.
  PC = curr_pc;
  auto &WPC = state.gpr.pc.word;

  // This is to support load-linked/store-conditional operations.
  auto &MONITOR = monitor;

  auto &W0 = state.gpr.zero.word;
  auto &W1 = state.gpr.at.word;
  auto &W2 = state.gpr.v0.word;
  auto &W3 = state.gpr.v1.word;

  auto &W4 = state.gpr.a0.word;
  auto &W5 = state.gpr.a1.word;
  auto &W6 = state.gpr.a2.word;
  auto &W7 = state.gpr.a3.word;

  auto &W8 = state.gpr.t0.word;
  auto &W9 = state.gpr.t1.word;
  auto &W10 = state.gpr.t2.word;
  auto &W11 = state.gpr.t3.word;

  auto &W12 = state.gpr.t4.word;
  auto &W13 = state.gpr.t5.word;
  auto &W14 = state.gpr.t6.word;
  auto &W15 = state.gpr.t7.word;

  auto &W16 = state.gpr.s0.word;
  auto &W17 = state.gpr.s1.word;
  auto &W18 = state.gpr.s2.word;
  auto &W19 = state.gpr.s3.word;

  auto &W20 = state.gpr.s4.word;
  auto &W21 = state.gpr.s5.word;
  auto &W22 = state.gpr.s6.word;
  auto &W23 = state.gpr.s7.word;

  auto &W24 = state.gpr.t8.word;
  auto &W25 = state.gpr.t9.word;
  auto &W26 = state.gpr.k0.word;
  auto &W27 = state.gpr.k1.word;

  auto &W28 = state.gpr.gp.word;
  
  auto &W29 = state.gpr.sp.word;
  auto &W30 = state.gpr.fp.word;
  auto &W31 = state.gpr.ra.word;
  auto &TLS_BASE = state.TLS_BASE;


  // coprocessor 1 single precision floating point register

  auto &SF0 = state.fpr.f0.word;
  auto &SF1 = state.fpr.f1.word;
  auto &SF2 = state.fpr.f2.word;                   
  auto &SF3 = state.fpr.f3.word;

  auto &SF4 = state.fpr.f4.word;
  auto &SF5 = state.fpr.f5.word;
  auto &SF6 = state.fpr.f6.word;
  auto &SF7 = state.fpr.f7.word;

  auto &SF8 = state.fpr.f8.word;
  auto &SF9 = state.fpr.f9.word;
  auto &SF10 = state.fpr.f10.word;
  auto &SF11 = state.fpr.f11.word;

  auto &SF12 = state.fpr.f12.word;
  auto &SF13 = state.fpr.f13.word;
  auto &SF14 = state.fpr.f14.word;
  auto &SF15 = state.fpr.f15.word;

  auto &SF16 = state.fpr.f16.word;
  auto &SF17 = state.fpr.f17.word;
  auto &SF18 = state.fpr.f18.word;
  auto &SF19 = state.fpr.f19.word;

  auto &SF20 = state.fpr.f20.word;
  auto &SF21 = state.fpr.f21.word;
  auto &SF22 = state.fpr.f22.word;
  auto &SF23 = state.fpr.f23.word;

  auto &SF24 = state.fpr.f24.word;
  auto &SF25 = state.fpr.f25.word;
  auto &SF26 = state.fpr.f26.word;
  auto &SF27 = state.fpr.f27.word;

  auto &SF28 = state.fpr.f28.word;
  
  auto &SF29 = state.fpr.f29.word;
  auto &SF30 = state.fpr.f30.word;
  auto &SF31 = state.fpr.f31.word;

 //coprocessor 2 registers

 auto &DW0 = state.cp2.zero.dword;
  
  auto &DW1 = state.cp2.at.dword;
  auto &DW2 = state.cp2.v0.dword;
  auto &DW3 = state.cp2.v1.dword;

  auto &DW4 = state.cp2.a0.dword;
  auto &DW5 = state.cp2.a1.dword;
  auto &DW6 = state.cp2.a2.dword;
  auto &DW7 = state.cp2.a3.dword;

  auto &DW8 = state.cp2.t0.dword;
  auto &DW9 = state.cp2.t1.dword;
  auto &DW10 = state.cp2.t2.dword;
  auto &DW11 = state.cp2.t3.dword;

  auto &DW12 = state.cp2.t4.dword;
  auto &DW13 = state.cp2.t5.dword;
  auto &DW14 = state.cp2.t6.dword;
  auto &DW15 = state.cp2.t7.dword;

  auto &DW16 = state.cp2.s0.dword;
  auto &DW17 = state.cp2.s1.dword;
  auto &DW18 = state.cp2.s2.dword;
  auto &DW19 = state.cp2.s3.dword;

  auto &DW20 = state.cp2.s4.dword;
  auto &DW21 = state.cp2.s5.dword;
  auto &DW22 = state.cp2.s6.dword;
  auto &DW23 = state.cp2.s7.dword;

  auto &DW24 = state.cp2.t8.dword;
  auto &DW25 = state.cp2.t9.dword;
  auto &DW26 = state.cp2.k0.dword;
  auto &DW27 = state.cp2.k1.dword;

  auto &DW28 = state.cp2.gp.dword;
  auto &DW29 = state.cp2.sp.dword;
  auto &DW30 = state.cp2.fp.dword;
  auto &DW31 = state.cp2.ra.dword; 

//  cp1 Floating point registers
  auto &F0 = state.cp1.D0.f0.word;
  auto &F1 = state.cp1.D0.f1.word;
  auto &F2 = state.cp1.D2.f2.word;
  auto &F3 = state.cp1.D2.f3.word;
  auto &F4 = state.cp1.D4.f4.word;
  auto &F5 = state.cp1.D4.f5.word;
  auto &F6 = state.cp1.D6.f6.word;
  auto &F7 = state.cp1.D6.f7.word;
  auto &F8 = state.cp1.D8.f8.word;
  auto &F9 = state.cp1.D8.f9.word;
  auto &F10 = state.cp1.D10.f10.word;
  auto &F11 = state.cp1.D10.f11.word;
  auto &F12 = state.cp1.D12.f12.word;
  auto &F13 = state.cp1.D12.f13.word;
  auto &F14 = state.cp1.D14.f14.word;
  auto &F15 = state.cp1.D14.f15.word;
  auto &F16 = state.cp1.D16.f16.word;
  auto &F17 = state.cp1.D16.f17.word;
  auto &F18 = state.cp1.D18.f18.word;
  auto &F19 = state.cp1.D18.f19.word;
  auto &F20 = state.cp1.D20.f20.word;
  auto &F21 = state.cp1.D20.f21.word;
  auto &F22 = state.cp1.D22.f22.word;
  auto &F23 = state.cp1.D22.f23.word;
  auto &F24 = state.cp1.D24.f24.word;
  auto &F25 = state.cp1.D24.f25.word;
  auto &F26 = state.cp1.D26.f26.word;
  auto &F27 = state.cp1.D26.f27.word;
  auto &F28 = state.cp1.D28.f28.word;
  auto &F29 = state.cp1.D28.f29.word;
  auto &F30 = state.cp1.D30.f30.word;
  auto &F31 = state.cp1.D30.f31.word;

  // double precision floating point numbers
  auto &D0 = state.cp1.D0.DReg;
  
  auto &D2 = state.cp1.D2.DReg;
  
  auto &D4 = state.cp1.D4.DReg;
  
  auto &D6 = state.cp1.D6.DReg;
  
  auto &D8 = state.cp1.D8.DReg;
  
  auto &D10 = state.cp1.D10.DReg;
  
  auto &D12 = state.cp1.D12.DReg;
  
  auto &D14 = state.cp1.D14.DReg;
  
  auto &D16 = state.cp1.D16.DReg;
  
  auto &D18 = state.cp1.D18.DReg;
  
  auto &D20 = state.cp1.D20.DReg;
  
  auto &D22 = state.cp1.D22.DReg;
  
  auto &D24 = state.cp1.D24.DReg;
  
  auto &D26 = state.cp1.D26.DReg;
  
  auto &D28 = state.cp1.D28.DReg;
  
  auto &D30 = state.cp1.D30.DReg;
  
  auto &CC = state.fcc.cc;       /* floating point condition code reg*/

  auto &COP2CC = state.cop2cc.cc;  // coprocessor 2 condition code 

  auto &CR = state.cop2cr.cr;   // coprocessor 2 control register

  auto &GR = state.cop2gr.gr;   // coprocessor 2 general register

  // Lifted code will be placed here in clones versions of this function.
  return memory;
  
}

#pragma clang diagnostic pop

}  // extern C

#include "remill/Arch/Runtime/Intrinsics.cpp"
