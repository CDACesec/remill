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
 */

#pragma once
#include <cstdint>

namespace remill {

class Instruction;

namespace mipsel {

enum class InstName : uint16_t {
  INVALID,
  LUI,
  LW,
  LWC1,
  LDC1,
  ADD,
  ADDU,
  ADDI,
  ADDIU,
  ADD_FMT,
  DIV,
  DIV_fmt,
  DIVU,
  MUL,
  MUL_fmt,
  MULT,
  MULTU,
  MADD,
  MOV_fmt,
  MOVZ,
  MOVN,
  MOVF,
  MOVT,
  MFHI,
  MFLO,
  MTLO,
  TEQ,
  BGTZ,
  CMP_COND_FMT,
  CVT_D_FMT,
  CVT_S_FMT,
  MFC1,
  MFHC1,
  MTHC1,
  MTC1,
  NEG_fmt,
  SUBU,
  SW,
  SWL,
  SWC1,
  SDC1,
  SH,
  SEH,
  LH,
  LBU,
  LB,
  LHU,
  NOP,
  SLTIU,
  SLTI,
  SLT,
  SLTU,
  SUB_FMT,
  OR,
  ORI,
  AND,
  ANDI,
  NOR,
  XOR,
  XORI,
  JR,
  J,
  JALR,
  JAL,
  JALX,
  SRA,
  SRAV,
  SRL,
  SLL,
  SLLV,
  SRLV,
  BEQ,
  BNE,
  B,
  BGEZAL,
  BC1F,
  BC1T,
  SB,
  SEB,
  BAL,
  BGEZ,
  BLEZ,
  BLTZ,
  BEQL,
  BLTZAL,
  BGTZL,
  BGEZL,
  BLTZL,
  BNEL,
  LL,
  ROTRV,
  ROTR,
  CFC1,
  CTC1,
  SC,
  SUB,
  JALR_HB,
  JR_HB,
  SSNOP,
  LWL,
  LWR,
  PREF,
  SYNCI,
  BC1FL,
  BC1TL,
  ROUND_L_fmt,
  ROUND_W_fmt,
  CEIL_L_fmt,
  CEIL_W_fmt,
  BC2F,
  BC2T,
  LDC2,
  LWC2,
  SDC2,
  SWC2,
  CFC2,
  CTC2,
  MFC2,
  MFHC2,
  MTC2,
  MTHC2,
  BC2FL,
  BC2TL,
  CACHE,
  COP2,
  EXT,
  ADDIU_16e,
  ADDIU_16e_Word,
  ADDIU_16e_IMM_SP,
  ADDU_16e,
  AND_16e,
  B_16e,
  B_16e_ext,
  BEQZ_16e,
  BEQZ_16e_ext,
  BNEZ_16e,
  BNEZ_16e_ext,
  BREAK_16e,
  BTEQZ_16e,
  BTEQZ_16e_ext,
  BTNEZ_16e,
  BTNEZ_16e_ext,
  CMP_16e,
  CMPI_16e_imm,
  CMPI_16e_imm_ext,
  DIV_16e,
  DIVU_16e,
  MFHI_16e,
  MFLO_16e,
  MOVE_16e,
  MOVE_16e_r32,
  MULT_16e,
  MULTU_16e,
  NEG_16e,
  NOT_16e,
  OR_16e,
  RESTORE_16e,
  RESTORE_16e_ext,
  SAVE_16e,
  SAVE_16e_ext,
  SB_16e,
  SB_16e_ext,
  SDBBP_16e,
  SEB_16e,
  INS,
  ASMACRO_16e,
  DMT_MT,
  DVPE_MT,
  EMT_MT,
  EVPE_MT,
  FORK_MT,
  MFTR_MT,
  MTTR_MT,
  YIELD_MT,
   
   
};

enum class InstForm : uint16_t {
  INVALID,
  LUI_IMM,
  LW_32_IMM,
  LWC1_FLOATING_POINT,
  LDC1_FLOATING_R,
  ADD_word,
  ADDU_32_R,
  ADDI_32_IMM,
  ADDIU_32_IMM,
  ADD_FMT,
  DIV_32,
  DIV_fmt,
  DIVU,
  MUL_32,
  MUL_fmt,
  MULT,
  MADD,
  MULTU,
  MOV_fmt,
  MOVZ,
  MOVN,
  MOVF,
  MOVT,
  MFHI,
  MFLO,
  MTLO,
  TEQ_32,
  BGTZ_32_IMM_COND, 
  CMP_COND_FMT,
  CVT_D_FMT,
  CVT_S_FMT,
  MFC1_FLOATING_POINT,
  MFHC1_FLOATING_POINT,
  MTHC1_FLOATING_POINT,
  MTC1_FLOATING_POINT,
  NEG_fmt,
  SUBU_32_R,
  SW_32_IMM,
  SWL,
  SWC1_FLOATING_POINT,
  SDC1_FLOATING_R,
  SH_16_MEM,
  SEH,
  LH,
  LBU_32_R,
  LB_32_R,
  LHU_32_R,
  NOP,
  SLTIU_IMM,
  SLTI_IMM,
  SLT_32_R,
  SLTU_32_R,
  SUB_FMT,
  OR_32_R,
  ORI_32_IMM,
  AND_32_R,
  ANDI_32_IMM,
  NOR_32_R,
  XOR_32_R,
  XORI_32_IMM,
  JR_32_R,
  J_Target,
  JALR_32_R,
  JAL_Target,
  JALX_JUMP_LINK,
  SRA_RIGHT_SHIFT,
  SRAV_RIGHT_ARITHMETIC_VARIABLE,
  SRL_RIGHT_SHIFT_LOGICAL,
  SLL_LEFT_SHIFT_LOGICAL,
  SLLV_LEFT_LOGICAL_VARIABLE,
  SRLV_RIGHT_LOGICAL_VARIABLE,
  BEQ_BRANCH_EQUAL,
  BNE_BRANCH_NOT_EQUAL,
  B_32_offset,
  BGEZAL_LINK,
  BC1F_FP_FALSE,
  BC1T_FP_TRUE,
  SB_IMM,
  SEB_EXTEND_BYTE,
  BAL_BRANCH_LINK,
  BGEZ,
  BLEZ,
  BLTZ,
  BEQL_BRANCH_LIKELY,
  BLTZAL,
  BGTZL_BRANCH_LIKELY,
  BGEZL_BRANCH_LIKELY,
  BLTZL_BRANCH_LIKELY,
  BNEL_BRANCH_LIKELY,
  LL_LOAD_LINKED_WORD,
  ROTRV,
  ROTR,
  CFC1_FLOATING_POINT,
  CTC1,
  SC_STORE_CONDITIONAL_WORD,
  SUB_WORD,
  JALR_HB,
  JR_HB,
  SSNOP_NO_OPERATION,
  LWL_LOAD_WORD_LEFT,
  LWR_LOAD_WORD_RIGHT,
  PREF,
  SYNCI,
  BC1FL_FP_FALSE_LIKELY,
  BC1TL_FP_TRUE_LIKELY,
  ROUND_L_fmt,
  ROUND_W_fmt,
  CEIL_L_fmt,
  CEIL_W_fmt,
  BC2F_FP_FALSE,
  BC2T_FP_TRUE,
  LDC2,
  LWC2,
  SDC2,
  SWC2,
  CFC2,
  CTC2,
  MFC2,
  MFHC2,
  MTC2,
  MTHC2,
  BC2FL_FALSE_LIKELY,
  BC2TL_TRUE_LIKELY,
  CACHE_OPERATION,
  COP2_OPERATION,
  EXT,
  ADDIU_16e_IMM,
  ADDIU_16e_IMM_Word,
  ADDIU_16e_IMM_SPRelative,
  ADDU_16e_REG,
  AND_16e,
  B_16e,
  B_16e_ext,
  BEQZ_16e,
  BEQZ_16e_ext,
  BNEZ_16e,
  BNEZ_16e_ext,
  BREAK_16e,
  BTEQZ_16e,
  BTEQZ_16e_ext,
  BTNEZ_16e,
  BTNEZ_16e_ext,
  CMP_16e,
  CMPI_16e_imm,
  CMPI_16e_imm_ext,
  DIV_16e,
  DIVU_16e,
  MFHI_16e,
  MFLO_16e,
  MOVE_16e,
  MOVE_16e_r32,
  MULT_16e,
  MULTU_16e,
  NEG_16e,
  NOT_16e,
  OR_16e,
  RESTORE_16e,
  RESTORE_16e_ext,
  SAVE_16e,
  SAVE_16e_ext,
  SB_16e,
  SB_16e_ext,
  SDBBP_16e,
  SEB_16e,
  INS,
  ASMACRO_16e,
  DMT_MT,
  DVPE_MT,
  EMT_MT,
  EVPE_MT,
  FORK_MT,
  MFTR_MT,
  MTTR_MT,
  YIELD_MT,
    
};

union InstImm {
  uint64_t uimm;
  

} __attribute__((packed));
static_assert(sizeof(InstImm) == 8, "");

#ifdef REMILL_MIPS_STRICT_REGNUM
enum class RegNum : uint8_t;
#else
using RegNum = uint32_t; 
// using RegNum = uint8_t; 
#endif

struct InstData {
  InstForm iform;
  InstName iclass;
  uint16_t IMM;
  int16_t SIGNED_IMM;
  int8_t SIGNED_16e_IMM; 
  int16_t offset;
  int16_t offset_1;
  int16_t offset_2;                                   
  uint16_t impl;
  uint16_t cfc1_0;
  RegNum rt; 
  RegNum target;
  RegNum base; 
  RegNum rs; 
  RegNum fs;
  RegNum fd;
  RegNum ft;
  RegNum rd;
  RegNum rx;
  RegNum ry;
  RegNum rz;
  uint8_t lsb;
  uint8_t msbd;
  uint8_t cf_0; 
  uint32_t cofun;
  uint8_t  co; 
  uint8_t h;
  uint8_t u;
  uint8_t sel;
  uint8_t opcode;
  uint8_t p0;
  uint8_t p1;
  uint8_t p2;
  uint8_t p3;
  uint8_t p4;
  uint8_t select;
  uint8_t I8;
  uint8_t aregs;
  uint8_t xsregs;
  uint8_t framesize_2;
  uint8_t op;
  uint8_t shift;
  uint8_t hint;    
  uint8_t funct;
  uint8_t CNVT;
  uint16_t ____0;
  uint16_t ___0;
  uint8_t __0;
  uint16_t _0;
  uint16_t sc;
  uint16_t _1;
  uint16_t _2;
  uint16_t jr_0;
  uint16_t div_0;
  uint16_t mtlo_0;
  uint16_t code;
  uint8_t _R;
  uint8_t _B;
  uint8_t _BEQZ;
  uint8_t _BNEZ;
  uint8_t MOVR32;
  uint8_t jalr_1;
  uint8_t jr_1;
  uint8_t ssnop_1;
  uint8_t _01;
  uint8_t _02;
  uint8_t _03;
  uint8_t _sa;
  uint8_t _s0;
  uint8_t framesize;
  uint8_t _s1;
  uint8_t _ra;
  uint8_t _s;
  uint8_t mt_0;
  uint16_t mult_0;
  uint8_t SLL;
  uint8_t fmt; 
  uint8_t cc; 
  uint8_t A; 
  uint8_t FC; 
  uint8_t cond; 
  uint8_t mf; 
  uint8_t mt; 
  uint8_t mfh; 
  uint8_t mth; 
  uint8_t tf; 
  uint8_t nd; 
  uint8_t bc; 
  uint32_t instr_index;
  uint16_t temp;
};

// LUI  <rt>, <immediate>
bool TryDecodeLUI_IMM(const InstData &data, Instruction &inst);

// LW  <rt>, <offset(base)>
bool TryDecodeLW_32_IMM(const InstData &data, Instruction &inst);

// format todo
bool TryDecodeLWC1_FLOATING_POINT(const InstData &data, Instruction &inst);

// LDC1  <ft>, <offset(base)> 
bool TryDecodeLDC1_FLOATING_R(const InstData &data, Instruction &inst);

// ADDU  <rd>,<rs>,<rt>
bool TryDecodeADD_word(const InstData &data, Instruction &inst);

// ADDU  <rd>,<rs>,<rt>
bool TryDecodeADDU_32_R(const InstData &data, Instruction &inst);

// ADDI  <rt>, <rs>,<immediate>
bool TryDecodeADDI_32_IMM(const InstData &data, Instruction &inst);

// ADDIU  <rt>, <rs>,<immediate>
bool TryDecodeADDIU_32_IMM(const InstData &data, Instruction &inst);

// ADD.fmt  <fd>, <fs>, <ft>
bool TryDecodeADD_FMT(const InstData &data, Instruction &inst);

// DIV rs, rt
bool TryDecodeDIV_32(const InstData &data, Instruction &inst);

// DIV_fmt fd, fs ,ft
bool TryDecodeDIV_fmt(const InstData &data, Instruction &inst);

// DIVU rs, rt
bool TryDecodeDIVU(const InstData &data, Instruction &inst);

// MUL rd ,rs, rt
bool TryDecodeMUL_32(const InstData &data, Instruction &inst);

// MUL_fmt fd, fs ,ft
bool TryDecodeMUL_fmt(const InstData &data, Instruction &inst);

// MULT rs , rt
bool TryDecodeMULT(const InstData &data, Instruction &inst);

// MULTU rs , rt
bool TryDecodeMULTU(const InstData &data, Instruction &inst);

// MADD rs , rt
bool TryDecodeMADD(const InstData &data, Instruction &inst);

// MOV_fmt fd, fs 
bool TryDecodeMOV_fmt(const InstData &data, Instruction &inst);

// MOVZ rd, rs, rt 
bool TryDecodeMOVZ(const InstData &data, Instruction &inst);

// // MOVN rd, rs, rt 
bool TryDecodeMOVN(const InstData &data, Instruction &inst);

// MOVF rd, rs, cc 
bool TryDecodeMOVF(const InstData &data, Instruction &inst);

// // MOVT rd, rs, cc 
bool TryDecodeMOVT(const InstData &data, Instruction &inst);

// MFHI  rd
bool TryDecodeMFHI(const InstData &data, Instruction &inst);

// MFLO rd 
bool TryDecodeMFLO(const InstData &data, Instruction &inst);

// // MTLO rs 
bool TryDecodeMTLO(const InstData &data, Instruction &inst);

// TEQ rs , rt
bool TryDecodeTEQ_32(const InstData &data, Instruction &inst);

// BGTZ  <rs>,<immediate>
bool TryDecodeBGTZ_32_IMM_COND(const InstData &data, Instruction &inst);

// C.cond.fmt || currently c.le.s
bool TryDecodeCMP_COND_FMT(const InstData &data, Instruction &inst);

// cvt.d.fmt <fd> <fs> || currently cvt.d.s
bool TryDecodeCVT_D_FMT(const InstData &data, Instruction &inst);

// cvt.s.fmt <fd> <fs> 
bool TryDecodeCVT_S_FMT(const InstData &data, Instruction &inst);

// MFC1 <rt>, <fs>
bool TryDecodeMFC1_FLOATING_POINT(const InstData &data, Instruction &inst);

// MFHC1 <rt>, <fs>
bool TryDecodeMFHC1_FLOATING_POINT(const InstData &data, Instruction &inst);

// MTHC1 <rt>, <fs>
bool TryDecodeMTHC1_FLOATING_POINT(const InstData &data, Instruction &inst);

// MTC1 <rt>, <fs>
bool TryDecodeMTC1_FLOATING_POINT(const InstData &data, Instruction &inst);

// MTC1 <rt>, <fs>
bool TryDecodeNEG_fmt(const InstData &data, Instruction &inst);

// SUBU <rd>,<rs>,<rt>
bool TryDecodeSUBU_32_R(const InstData &data, Instruction &inst);

// SW <rt>,<offset(base)>
bool TryDecodeSW_32_IMM(const InstData &data, Instruction &inst);

// SWL <rt>,<offset(base)>
bool TryDecodeSWL(const InstData &data, Instruction &inst);

// SWC1 <ft>, offset(base)
bool TryDecodeSWC1_FLOATING_POINT(const InstData &data, Instruction &inst);

// SDC1 <ft>,<offset(base)> 
bool TryDecodeSDC1_FLOATING_R(const InstData &data, Instruction &inst);

// SH <rt>,<offset(base)>
bool TryDecodeSH_16_MEM(const InstData &data, Instruction &inst);

// SEH <rt>,<rt>
bool TryDecodeSEH(const InstData &data, Instruction &inst);

// LH <rt>,<offset(base)>
bool TryDecodeLH(const InstData &data, Instruction &inst);

//LBU <rt> ,<offset(base)>
bool TryDecodeLBU_32_R(const InstData &data, Instruction &inst);

//LB <rt> ,<offset(base)>
bool TryDecodeLB_32_R(const InstData &data, Instruction &inst);

//LHU <rt> ,<offset(base)>
bool TryDecodeLHU_32_R(const InstData &data, Instruction &inst);

//NOP
bool TryDecodeNOP(const InstData &data, Instruction &inst);

//SLTIU <rt> ,<rs>,<immediate>
bool TryDecodeSLTIU_IMM(const InstData &data, Instruction &inst);

//SLTI <rt> ,<rs>,<immediate>
bool TryDecodeSLTI_IMM(const InstData &data, Instruction &inst);

//SLT <rd> , <rs> , <rt>
bool TryDecodeSLT_32_R(const InstData &data, Instruction &inst);

//SLTU <rd> , <rs> , <rt> 
bool TryDecodeSLTU_32_R(const InstData &data, Instruction &inst);

// SUB.FMT <ft>, <fs>, <fd>
bool TryDecodeSUB_FMT(const InstData &data, Instruction &inst);

//OR <rd> ,<rs>,<rt>
bool TryDecodeOR_32_R(const InstData &data, Instruction &inst);

//ORI <rt> ,<rs>,<imm>
bool TryDecodeORI_32_IMM(const InstData &data, Instruction &inst);

//AND <rd> ,<rs>,<rt>
bool TryDecodeAND_32_R(const InstData &data, Instruction &inst);

//ANDI <rt> ,<rs>,<imm>
bool TryDecodeANDI_32_IMM(const InstData &data, Instruction &inst);

//NOR <rd> ,<rs>,<rt>
bool TryDecodeNOR_32_R(const InstData &data, Instruction &inst);

//XOR <rd> ,<rs>,<rt>
bool TryDecodeXOR_32_R(const InstData &data, Instruction &inst);

//XORI <rt> ,<rs>,<imm>
bool TryDecodeXORI_32_IMM(const InstData &data, Instruction &inst);

//JR  <rs>
bool TryDecodeJR_32_R(const InstData &data, Instruction &inst);

//J  <target>
bool TryDecodeJ_Target(const  InstData &data, Instruction &inst);

//JALR <rd>, <rs>
bool TryDecodeJALR_32_R(const InstData &data, Instruction &inst);

//JAL <target>
bool TryDecodeJAL_Target(const InstData &data, Instruction &inst);

//JALX <target>
bool TryDecodeJALX_JUMP_LINK(const InstData &data, Instruction &inst);

//SRA  rd , rt , sa
bool TryDecodeSRA_RIGHT_SHIFT(const InstData &data, Instruction &inst);

//SRAV  rd , rt , sa
bool TryDecodeSRAV_RIGHT_ARITHMETIC_VARIABLE(const InstData &data, Instruction &inst);

//SRL  rd , rt , sa
bool TryDecodeSRL_RIGHT_SHIFT_LOGICAL(const InstData &data, Instruction &inst);

//SLL  rd , rt , sa
bool TryDecodeSLL_LEFT_SHIFT_LOGICAL(const InstData &data, Instruction &inst);

//SLLV  rd , rt , rs
bool TryDecodeSLLV_LEFT_LOGICAL_VARIABLE(const InstData &data, Instruction &inst);

//SRLV  rd , rt , rs
bool TryDecodeSRLV_RIGHT_LOGICAL_VARIABLE(const InstData &data, Instruction &inst);

//BEQ  rt , rs ,offset
bool TryDecodeBEQ_BRANCH_EQUAL(const InstData &data, Instruction &inst);

//BNE  rt , rs ,offset
bool TryDecodeBNE_BRANCH_NOT_EQUAL(const InstData &data, Instruction &inst);

//B offset
bool TryDecodeB_32_offset(const InstData &data, Instruction &inst);

//BGEZAL rs, offset
bool TryDecodeBGEZAL_LINK(const InstData &data, Instruction &inst);

// BC1F cc, offset
bool TryDecodeBC1F_FP_FALSE(const InstData &data, Instruction &inst);

// BC1T cc, offset
bool TryDecodeBC1T_FP_TRUE(const InstData &data, Instruction &inst);

//SB rt, offset(base)
bool TryDecodeSB_IMM(const InstData &data, Instruction &inst);

//SEB rd, rt
bool TryDecodeSEB_EXTEND_BYTE(const InstData &data, Instruction &inst);

//BAL offset
bool TryDecodeBAL_BRANCH_LINK(const InstData &data, Instruction &inst);

//BGEZ rs, offset
bool TryDecodeBGEZ(const InstData &data, Instruction &inst);

//BLEZ rs, offset
bool TryDecodeBLEZ(const InstData &data, Instruction &inst);

//BLTZ rs, offset
bool TryDecodeBLTZ(const InstData &data, Instruction &inst);

//BEQL rs, rt, offset
bool TryDecodeBEQL_BRANCH_LIKELY(const InstData &data, Instruction &inst);

//BLTZAL rs,  offset
bool TryDecodeBLTZAL(const InstData &data, Instruction &inst);

//BGTZL rs,  offset
bool TryDecodeBGTZL_BRANCH_LIKELY(const InstData &data, Instruction &inst);

//BGEZL rs,  offset
bool TryDecodeBGEZL_BRANCH_LIKELY(const InstData &data, Instruction &inst);

//BLTZL rs,  offset
bool TryDecodeBLTZL_BRANCH_LIKELY(const InstData &data, Instruction &inst);

//BNEL rs,  offset
bool TryDecodeBNEL_BRANCH_LIKELY(const InstData &data, Instruction &inst);

//LL rs,  offset(base)
bool TryDecodeLL_LOAD_LINKED_WORD(const InstData &data, Instruction &inst);

//ROTRV rd, rt, rs
bool TryDecodeROTRV(const InstData &data, Instruction &inst);

//ROTR rd, rt, sa
bool TryDecodeROTR(const InstData &data, Instruction &inst);

//CFC1 rt, fs
bool TryDecodeCFC1_FLOATING_POINT(const InstData &data, Instruction &inst);

//CTC1 rt, fs
bool TryDecodeCTC1(const InstData &data, Instruction &inst);

//SC  rt, offset(base)
bool TryDecodeSC_STORE_CONDITIONAL_WORD(const InstData &data, Instruction &inst);

//SUB rd ,rs , rt
bool TryDecodeSUB_WORD(const InstData &data, Instruction &inst);

//JALR.HB rd ,rs
bool TryDecodeJALR_HB(const InstData &data, Instruction &inst);

//JR.HB rs
bool TryDecodeJR_HB(const InstData &data, Instruction &inst);

//SSNOP
bool TryDecodeSSNOP_NO_OPERATION(const InstData &data, Instruction &inst);

//LWL,rt , offset(base)
bool TryDecodeLWL_LOAD_WORD_LEFT(const InstData &data, Instruction &inst);

//LWR,rt , offset(base)
bool TryDecodeLWR_LOAD_WORD_RIGHT(const InstData &data, Instruction &inst);

//PREF hint , offset(base)
bool TryDecodePREF(const InstData &data, Instruction &inst);

//SYNCI  offset(base)
bool TryDecodeSYNCI(const InstData &data, Instruction &inst);

//BC1FL cc, offset
bool TryDecodeBC1FL_FP_FALSE_LIKELY(const InstData &data, Instruction &inst);

//BC1TL cc, offset
bool TryDecodeBC1TL_FP_TRUE_LIKELY(const InstData &data, Instruction &inst);

//ROUND.L.fmt 
bool TryDecodeROUND_L_fmt(const InstData &data, Instruction &inst);

//ROUND.W.fmt 
bool TryDecodeROUND_W_fmt(const InstData &data, Instruction &inst);

//CEIL.L.fmt 
bool TryDecodeCEIL_L_fmt(const InstData &data, Instruction &inst);

//CEIL.W.fmt 
bool TryDecodeCEIL_W_fmt(const InstData &data, Instruction &inst);

//BC2F cc, offset 
bool TryDecodeBC2F_FP_FALSE(const InstData &data, Instruction &inst);

//BC2T cc, offset 
bool TryDecodeBC2T_FP_TRUE(const InstData &data, Instruction &inst);

//LDC2 rt, offset(base) 
bool TryDecodeLDC2(const InstData &data, Instruction &inst);

//LWC2 rt, offset(base) 
bool TryDecodeLWC2(const InstData &data, Instruction &inst);

//SDC2 rt, offset(base) 
bool TryDecodeSDC2(const InstData &data, Instruction &inst);

//SWC2 rt, offset(base) 
bool TryDecodeSWC2(const InstData &data, Instruction &inst);

//CFC2 rt, impl 
bool TryDecodeCFC2(const InstData &data, Instruction &inst);

//CTC2 rt, impl 
bool TryDecodeCTC2(const InstData &data, Instruction &inst);

//MFC2 rt, impl 
bool TryDecodeMFC2(const InstData &data, Instruction &inst);

//MFHC2 rt, impl 
bool TryDecodeMFHC2(const InstData &data, Instruction &inst);

//MTC2 rt, impl 
bool TryDecodeMTC2(const InstData &data, Instruction &inst);

//MTHC2 rt, impl 
bool TryDecodeMTHC2(const InstData &data, Instruction &inst);

//BC2FL cc, offset 
bool TryDecodeBC2FL_FALSE_LIKELY(const InstData &data, Instruction &inst);

//BC2TL cc, offset 
bool TryDecodeBC2TL_TRUE_LIKELY(const InstData &data, Instruction &inst);

//CACHE op, offset 
bool TryDecodeCACHE_OPERATION(const InstData &data, Instruction &inst);

//COP2 func 
bool TryDecodeCOP2_OPERATION(const InstData &data, Instruction &inst);

// EXT <rt>, <rs>, pos, size
bool TryDecodeEXT(const InstData &data, Instruction &inst);


//ADDIU rx, immediate 
bool TryDecodeADDIU_16e_IMM(const InstData &data, Instruction &inst);

//ADDIU ry, rx, immediate 
bool TryDecodeADDIU_16e_IMM_Word(const InstData &data, Instruction &inst);

//ADDIU rx, immediate 
bool TryDecodeADDIU_16e_IMM_SPRelative(const InstData &data, Instruction &inst);

//ADDU rz, rx, ry 
bool TryDecodeADDU_16e_REG(const InstData &data, Instruction &inst);

//AND ry, rx
bool TryDecodeAND_16e(const InstData &data, Instruction &inst);

//B offset
bool TryDecodeB_16e(const InstData &data, Instruction &inst);

//B offset extended
bool TryDecodeB_16e_ext(const InstData &data, Instruction &inst);

// BEQZ rx, offset
bool TryDecodeBEQZ_16e(const InstData &data, Instruction &inst);

// BEQZ rx, offset(extended)
bool TryDecodeBEQZ_16e_ext(const InstData &data, Instruction &inst);

// BNEZ rx, offset
bool TryDecodeBNEZ_16e(const InstData &data, Instruction &inst);

// BNEZ rx, offset(extended)
bool TryDecodeBNEZ_16e_ext(const InstData &data, Instruction &inst);

//BREAK offset 
bool TryDecodeBREAK_16e(const InstData &data, Instruction &inst);

// BTEQZ offset
bool TryDecodeBTEQZ_16e(const InstData &data, Instruction &inst);

// BTEQZ offset(extended)
bool TryDecodeBTEQZ_16e_ext(const InstData &data, Instruction &inst);

// BTNEZ  offset
bool TryDecodeBTNEZ_16e(const InstData &data, Instruction &inst);

// BTNEZ offset(extended)
bool TryDecodeBTNEZ_16e_ext(const InstData &data, Instruction &inst);

// CMP_16e offset(extended)
bool TryDecodeCMP_16e(const InstData &data, Instruction &inst);

// CMPI_16e_imm offset(extended)
bool TryDecodeCMPI_16e_imm(const InstData &data, Instruction &inst);

// CMPI_16e_imm offset(extended)
bool TryDecodeCMPI_16e_imm_ext(const InstData &data, Instruction &inst);

// DIV_16e rx, ry
bool TryDecodeDIV_16e(const InstData &data, Instruction &inst);


// DIVU_16e rx, ry 
bool TryDecodeDIVU_16e(const InstData &data, Instruction &inst);

// MFHI_16e rx, ry 
bool TryDecodeMFHI_16e(const InstData &data, Instruction &inst);

// MFLO_16e rx, ry 
bool TryDecodeMFLO_16e(const InstData &data, Instruction &inst);

// MOVE_16e r32, rz 
bool TryDecodeMOVE_16e(const InstData &data, Instruction &inst);

// MOVE_16e_r32 ry, r32 
bool TryDecodeMOVE_16e_r32(const InstData &data, Instruction &inst);

// MULT_16e rx, ry 
bool TryDecodeMULT_16e(const InstData &data, Instruction &inst);

// MULTU_16e rx, ry 
bool TryDecodeMULTU_16e(const InstData &data, Instruction &inst);

// NEG_16e rx, ry 
bool TryDecodeNEG_16e(const InstData &data, Instruction &inst);

// NOP_16e 
// bool TryDecodeNOP_16e(const InstData &data, Instruction &inst);

// NOT_16e rx, ry 
bool TryDecodeNOT_16e(const InstData &data, Instruction &inst);

// OR_16e rx, ry 
bool TryDecodeOR_16e(const InstData &data, Instruction &inst);

// RESTORE_16e rx, ry 
bool TryDecodeRESTORE_16e(const InstData &data, Instruction &inst);

// RESTORE_16e_ext rx, ry 
bool TryDecodeRESTORE_16e_ext(const InstData &data, Instruction &inst);

// SAVE_16e rx, ry 
bool TryDecodeSAVE_16e(const InstData &data, Instruction &inst);

// SAVE_16e_ext rx, ry 
bool TryDecodeSAVE_16e_ext(const InstData &data, Instruction &inst);

// SB_16e rx, ry 
bool TryDecSB_16e(const InstData &data, Instruction &inst);

// SDBBP_16e rx, ry 
bool TryDecodeSDBBP_16e(const InstData &data, Instruction &inst);

// SB_16e rx, ry 
bool TryDecodeSB_16e(const InstData &data, Instruction &inst);

// SB_16e_ext rx, ry 
bool TryDecodeSB_16e_ext(const InstData &data, Instruction &inst);

// SEB_16e rx, ry 
bool TryDecodeSEB_16e(const InstData &data, Instruction &inst);

// ASMACRO_16e select,p0,p1,p2,p3,p4
bool TryDecodeASMACRO_16e(const InstData &data, Instruction &inst);

// INS <rt>, <rs>, pos, size
bool TryDecodeINS(const InstData &data, Instruction &inst);

// DMT_MT rt
bool TryDecodeDMT_MT(const InstData &data, Instruction &inst);

// DVPE_MT rt
bool TryDecodeDVPE_MT(const InstData &data, Instruction &inst);

// EMT_MT rt
bool TryDecodeEMT_MT(const InstData &data, Instruction &inst);

// EVPE_MT rt
bool TryDecodeEVPE_MT(const InstData &data, Instruction &inst);

// FORK_MT rd, rs, rt
bool TryDecodeFORK_MT(const InstData &data, Instruction &inst);

// MFTR_MT  rd, rt, u, sel, h
bool TryDecodeMFTR_MT(const InstData &data, Instruction &inst);

// MTTR_MT rd, rt, u, sel, h
bool TryDecodeMTTR_MT(const InstData &data, Instruction &inst);

// YIELD_MT rd, rs
bool TryDecodeYIELD_MT(const InstData &data, Instruction &inst);


const char *InstNameToString(InstName iclass);
const char *InstFormToString(InstForm iform);

bool TryExtractMips(const uint8_t *bytes, InstData &data);
bool TryDecodeMips(const InstData &data, Instruction &inst);

}  // namespace mipsel
}  // namespace remill
