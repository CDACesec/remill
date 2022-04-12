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
#include "remill/Arch/Runtime/Intrinsics.h"
#include "remill/Arch/Runtime/Operators.h"
#include "remill/Arch/MIPSEL/Runtime/State.h"
#include "remill/Arch/MIPSEL/Runtime/Operators.h"
#include "remill/Arch/MIPSEL/Runtime/Types.h"


#define REG_PC state.gpr.pc.word                                  
#define REG_RA state.gpr.ra.word

#define REG_ZERO state.gpr.zero.word
#define REG_AT state.gpr.at.word
#define REG_V0 state.gpr.v0.word
#define REG_V1 state.gpr.v1.word

#define REG_A0 state.gpr.a0.word
#define REG_A1 state.gpr.a1.word
#define REG_A2 state.gpr.a2.word
#define REG_A3 state.gpr.a3.word

#define REG_T0 state.gpr.t0.word
#define REG_T1 state.gpr.t1.word
#define REG_T2 state.gpr.t2.word
#define REG_T3 state.gpr.t3.word

#define REG_T4 state.gpr.t4.word
#define REG_T5 state.gpr.t5.word
#define REG_T6 state.gpr.t6.word
#define REG_T7 state.gpr.t7.word

#define REG_S0 state.gpr.s0.word
#define REG_S1 state.gpr.s1.word
#define REG_S2 state.gpr.s2.word
#define REG_S3 state.gpr.s3.word

#define REG_S4 state.gpr.s4.word
#define REG_S5 state.gpr.s5.word
#define REG_S6 state.gpr.s6.word
#define REG_S7 state.gpr.s7.word

#define REG_T8 state.gpr.t8.word
#define REG_T9 state.gpr.t9.word
#define REG_K0 state.gpr.k0.word
#define REG_K1 state.gpr.k1.word

#define REG_GP state.gpr.gp.word
#define REG_SP state.gpr.sp.word
#define REG_FP state.gpr.fp.word
#define REG_LO state.spr.LO.word
#define REG_HI state.spr.HI.word
#define REG_CC state.fcc.cc
#define REG_CR state.cop2cr.cr
#define REG_GR state.cop2gr.gr
#define REG_COP2CC state.cop2cc.cc

// single precision floating point registers

#define REG_SF0 state.fpr.f0.word
#define REG_SF1 state.fpr.f1.word
#define REG_SF2 state.fpr.f2.word
#define REG_SF3 state.fpr.f3.word

#define REG_SF4 state.fpr.f4.word
#define REG_SF5 state.fpr.f5.word
#define REG_SF6 state.fpr.f6.word
#define REG_SF7 state.fpr.f7.word

#define REG_SF8 state.fpr.f8.word
#define REG_SF9 state.fpr.f9.word
#define REG_SF10 state.fpr.f10.word
#define REG_SF11 state.fpr.f11.word

#define REG_SF12 state.fpr.f12.word
#define REG_SF13 state.fpr.f13.word
#define REG_SF14 state.fpr.f14.word
#define REG_SF15 state.fpr.f15.word

#define REG_SF16 state.fpr.f16.word
#define REG_SF17 state.fpr.f17.word
#define REG_SF18 state.fpr.f18.word
#define REG_SF19 state.fpr.f19.word

#define REG_SF20 state.fpr.f20.word
#define REG_SF21 state.fpr.f21.word
#define REG_SF22 state.fpr.f22.word
#define REG_SF23 state.fpr.f23.word

#define REG_SF24 state.fpr.f24.word
#define REG_SF25 state.fpr.f25.word
#define REG_SF26 state.fpr.f26.word
#define REG_SF27 state.fpr.f27.word

#define REG_SF28 state.fpr.f28.word
#define REG_SF29 state.fpr.f29.word
#define REG_SF30 state.fpr.f30.word
#define REG_SF31 state.fpr.f31.word


// single precision floating point numbers

#define REG_F0 state.cp1.D0.f0.word
#define REG_F1 state.cp1.D0.f1.word
#define REG_F2 state.cp1.D2.f2.word
#define REG_F3  state.cp1.D2.f3.word
#define REG_F4  state.cp1.D4.f4.word
#define REG_F5  state.cp1.D4.f5.word
#define REG_F6  state.cp1.D6.f6.word
#define REG_F7  state.cp1.D6.f7.word
#define REG_F8  state.cp1.D8.f8.word
#define REG_F9  state.cp1.D8.f9.word
#define REG_F10  state.cp1.D10.f10.word
#define REG_F11  state.cp1.D10.f11.word
#define REG_F12  state.cp1.D12.f12.word
#define REG_F13  state.cp1.D12.f13.word
#define REG_F14  state.cp1.D14.f14.word
#define REG_F15  state.cp1.D14.f15.word
#define REG_F16  state.cp1.D16.f16.word
#define REG_F17  state.cp1.D16.f17.word
#define REG_F18  state.cp1.D18.f18.word
#define REG_F19  state.cp1.D18.f19.word
#define REG_F20  state.cp1.D20.f20.word
#define REG_F21  state.cp1.D20.f21.word
#define REG_F22  state.cp1.D22.f22.word
#define REG_F23  state.cp1.D22.f23.word
#define REG_F24  state.cp1.D24.f24.word
#define REG_F25  state.cp1.D24.f25.word
#define REG_F26  state.cp1.D26.f26.word
#define REG_F27  state.cp1.D26.f27.word
#define REG_F28  state.cp1.D28.f28.word
#define REG_F29  state.cp1.D28.f29.word
#define REG_F30  state.cp1.D30.f30.word
#define REG_F31  state.cp1.D30.f31.word

// double precision floating point numbers

#define REG_D0  state.cp1.D0.DReg  
#define REG_D2  state.cp1.D2.DReg 
#define REG_D4  state.cp1.D4.DReg  
#define REG_D6  state.cp1.D6.DReg  
#define REG_D8  state.cp1.D8.DReg  
#define REG_D10  state.cp1.D10.DReg 
#define REG_D12  state.cp1.D12.DReg  
#define REG_D14  state.cp1.D14.DReg 
#define REG_D16  state.cp1.D16.DReg  
#define REG_D18  state.cp1.D18.DReg  
#define REG_D20  state.cp1.D20.DReg  
#define REG_D22  state.cp1.D22.DReg  
#define REG_D24  state.cp1.D24.DReg  
#define REG_D26  state.cp1.D26.DReg  
#define REG_D28  state.cp1.D28.DReg 
#define REG_D30  state.cp1.D30.DReg

// coprocessor 0 (COP2) registers0

#define REG_VPEC_R state.cp0.VPEC_R.flat

#define REG_MVPC_R state.cp0.MVPC_R.flat

#define REG_TCRESTART state.cp0.TCRestart



// coprocessor 2 (COP2) registers

#define REG_DZERO state.cp2.zero.dword
#define REG_DAT state.cp2.at.dword
#define REG_DV0 state.cp2.v0.dword
#define REG_DV1 state.cp2.v1.dword

#define REG_DA0 state.cp2.a0.dword
#define REG_DA1 state.cp2.a1.dword
#define REG_DA2 state.cp2.a2.dword
#define REG_DA3 state.cp2.a3.dword

#define REG_DT0 state.cp2.t0.dword
#define REG_DT1 state.cp2.t1.dword
#define REG_DT2 state.cp2.t2.dword
#define REG_DT3 state.cp2.t3.dword

#define REG_DT4 state.cp2.t4.dword
#define REG_DT5 state.cp2.t5.dword
#define REG_DT6 state.cp2.t6.dword
#define REG_DT7 state.cp2.t7.dword

#define REG_DS0 state.cp2.s0.dword
#define REG_DS1 state.cp2.s1.dword
#define REG_DS2 state.cp2.s2.dword
#define REG_DS3 state.cp2.s3.dword

#define REG_DS4 state.cp2.s4.dword
#define REG_DS5 state.cp2.s5.dword
#define REG_DS6 state.cp2.s6.dword
#define REG_DS7 state.cp2.s7.dword

#define REG_DT8 state.cp2.t8.dword
#define REG_DT9 state.cp2.t9.dword
#define REG_DK0 state.cp2.k0.dword
#define REG_DK1 state.cp2.k1.dword

#define REG_DGP state.cp2.gp.dword
#define REG_DSP state.cp2.sp.dword
#define REG_DFP state.cp2.fp.dword

#define HYPER_CALL state.hyper_call

namespace {
    // Takes the place of an invalid instruction.
DEF_SEM(HandleInvalidInstruction) {
  HYPER_CALL = AsyncHyperCall::kInvalidInstruction;
  return memory;
}
}  // namespace

DEF_ISEL(INVALID_INSTRUCTION) = HandleInvalidInstruction;

#include "remill/Arch/MIPSEL/Semantics/BINARY.cpp"                        
#include "remill/Arch/MIPSEL/Semantics/DATAXFER.cpp"
#include "remill/Arch/MIPSEL/Semantics/MISC.cpp"
#include "remill/Arch/MIPSEL/Semantics/COND.cpp"
#include "remill/Arch/MIPSEL/Semantics/LOGICAL.cpp"
#include "remill/Arch/MIPSEL/Semantics/UNCOND.cpp"
#include "remill/Arch/MIPSEL/Semantics/SHIFT.cpp"
#include "remill/Arch/MIPSEL/Semantics/DIRECT_CALL.cpp"
#include "remill/Arch/MIPSEL/Semantics/DIRECT_JUMP.cpp"
#include "remill/Arch/MIPSEL/Semantics/BRANCH_PIPELINE.cpp"
#include "remill/Arch/MIPSEL/Semantics/COND_BRANCH.cpp"


