/*
 * Copyright (c) 2020 Centre for Development of Advanced Computing (C-DAC)
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
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "remill/Arch/MIPSEL/Decode.h"

namespace remill {
namespace mipsel {
namespace {

const char * const kIClassName[] = {
  nullptr,
  "LUI",                                                       
  "LW", 
  "LWC1",
  "LDC1",
  "ADD",
  "ADDU",
  "ADDI",
  "ADDIU",
  "ADD_FMT",
  "DIV",
  "DIV_fmt",
  "DIVU",
  "MUL",
  "MULT",
  "MULTU",
  "MUL_fmt",
  "MADD",
  "MOV_fmt",
  "MOVZ",
  "MOVN",
  "MOVF",
  "MOVT",
  "MFHI",
  "MFLO",
  "MTLO",
  "TEQ", 
  "BGTZ", 
  "CMP_COND_FMT",
  "CVT_D_FMT",
  "CVT_S_FMT",
  "MFC1",
  "MFHC1",
  "MTHC1",
  "MTC1",
  "NEG_fmt",
  "SUBU",
  "SW",
  "SWL",
  "SWC1",
  "SDC1",
  "SH",
  "SEH",
  "LH",
  "LBU",
  "LB",
  "LHU",
  "NOP",
  "SLTIU",
  "SLTI",
  "SLT",
  "SLTU",
  "SUB_FMT",
  "OR",
  "ORI",
  "AND",
  "ANDI",
  "NOR",
  "XOR",
  "XORI",
  "JR",
  "J",
  "JALR",
  "JAL",
  "JALX",
  "SRA",
  "SRAV",
  "SRL",
  "SLL",
  "SLLV",
  "SRLV",
  "BEQ",
  "BNE",
  "B",
  "BGEZAL",
  "BC1F",
  "BC1T",
  "SB",
  "SEB",
  "BAL",
  "BGEZ",
  "BLEZ",
  "BLTZ",
  "BEQL",
  "BLTZAL",
  "BGTZL",
  "BGEZL",
  "BLTZL",
  "BNEL",
  "LL",
  "ROTRV",
  "ROTR",
  "CFC1",
  "CTC1",
  "SC",
  "SUB",
  "JALR_HB",
  "JR_HB",
  "SSNOP",
  "LWL",
  "LWR",
  "PREF",
  "SYNCI",
  "BC1FL",
  "BC1TL",
  "ROUND_L_fmt",
  "ROUND_W_fmt",
  "CEIL_L_fmt",
  "CEIL_W_fmt",
  "BC2F",
  "BC2T",
  "LDC2",
  "LWC2",
  "SDC2",
  "SWC2",
  "CFC2",
  "CTC2",
  "MFC2",
  "MFHC2",
  "MTC2",
  "MTHC2",
  "BC2FL",
  "BC2TL",
  "CACHE",
  "COP2",
  "EXT",
  "ADDIU_16e",
  "ADDIU_16e_Word",
  "ADDIU_16e_IMM_SP",
  "ADDU_16e",
  "AND_16e",
  "B_16e",
  "B_16e_ext",
  "BEQZ_16e",
  "BEQZ_16e_ext",
  "BNEZ_16e",
  "BNEZ_16e_ext",
  "BREAK_16e",
  "BTEQZ_16e",
  "BTEQZ_16e_ext",
  "BTNEZ_16e",
  "BTNEZ_16e_ext",
  "CMP_16e",
  "CMPI_16e_imm",
  "CMPI_16e_imm_ext",
  "DIV_16e",
  "DIVU_16e",
  "MFHI_16e",
  "MFLO_16e",
  "MOVE_16e",
  "MOVE_16e_r32",
  "MULT_16e",
  "MULTU_16e",
  "NEG_16e",
  "NOT_16e",
  "OR_16e",
  "RESTORE_16e",
  "RESTORE_16e_ext",
  "SAVE_16e",
  "SAVE_16e_ext",
  "SB_16e",
  "SB_16e_ext",
  "SDBBP_16e",
  "SEB_16e",
  "INS",
  "ADDIU_16_TwoOperand",
  "ASMACRO_16e",
  "DMT_MT",
  "DVPE_MT",
  "EMT_MT",
  "EVPE_MT",       
  "FORK_MT",
  "MFTR_MT",
  "MTTR_MT",
  "YIELD",
};

const char * const kIFormName[] = {
  nullptr,
  "LUI_IMM",
  "LW_32_IMM",
  "LWC1_FLOATING_POINT",
  "LDC1_FLOATING_R",
  "ADD_word",
  "ADDU_32_R",
  "ADDI_32_IMM",
  "ADDIU_32_IMM",
  "ADD_FMT",
  "DIV_32",
  "DIV_fmt",
  "DIVU",
  "MUL_32",
  "MUL_fmt",
  "MULT",
  "MULTU",
  "MADD",
  "MOV_fmt",
  "MOVZ",
  "MOVN",
  "MOVF",
  "MOVT",
  "MFHI",
  "MFLO",
  "MTLO",
  "TEQ_32",
  "BGTZ_32_IMM_COND",
  "CMP_COND_FMT",
  "CVT_D_FMT",
  "CVT_S_FMT",
  "MFC1_FLOATING_POINT",
  "MFHC1_FLOATING_POINT",
  "MTHC1_FLOATING_POINT",
  "MTC1_FLOATING_POINT",
  "NEG_fmt",
  "SUBU_32_R",
  "SW_32_IMM",
  "SWL",
  "SWC1_FLOATING_POINT",
  "SDC1_FLOATING_R",
  "SH_16_MEM",
  "SEH",
  "LH",
  "LBU_32_R",
  "LB_32_R",
  "LHU_32_R",
  "NOP",
  "SLTIU_IMM",
  "SLTI_IMM",
  "SLT_32_R",
  "SLTU_32_R",
  "SUB_FMT",
  "OR_32_R",
  "ORI_32_IMM",
  "AND_32_R",
  "ANDI_32_IMM",
  "NOR_32_R",
  "XOR_32_R",
  "XORI_32_IMM",
  "JR_32_R",
  "J_Target",
  "JALR_32_R",
  "JAL_Target",
  "JALX_JUMP_LINK",
  "SRA_RIGHT_SHIFT",
  "SRAV_RIGHT_ARITHMETIC_VARIABLE", 
  "SRL_RIGHT_SHIFT_LOGICAL",
  "SLL_LEFT_SHIFT_LOGICAL",
  "SLLV_LEFT_LOGICAL_VARIABLE",
  "SRLV_RIGHT_LOGICAL_VARIABLE",
  "BEQ_BRANCH_EQUAL",
  "BNE_BRANCH_NOT_EQUAL",
  "B_32_offset",
  "BGEZAL_LINK",
  "BC1F_FP_FALSE",
  "BC1T_FP_TRUE",
  "SB_IMM",
  "SEB_EXTEND_BYTE",
  "BAL_BRANCH_LINK",
  "BGEZ",
  "BLEZ",
  "BLTZ",
  "BEQL_BRANCH_LIKELY",
  "BLTZAL",
  "BGTZL_BRANCH_LIKELY",
  "BGEZL_BRANCH_LIKELY",
  "BLTZL_BRANCH_LIKELY",
  "BNEL_BRANCH_LIKELY",
  "LL_LOAD_LINKED_WORD",
  "ROTRV",
  "ROTR",
  "CFC1_FLOATING_POINT",
  "CTC1",
  "SC_STORE_CONDITIONAL_WORD",
  "SUB_WORD",
  "JALR_HB",
  "JR_HB",
  "SSNOP_NO_OPERATION",
  "LWL_LOAD_WORD_LEFT",
  "LWR_LOAD_WORD_RIGHT",
  "PREF",
  "SYNCI",
  "BC1FL_FP_FALSE_LIKELY",
  "BC1TL_FP_TRUE_LIKELY",
  "ROUND_L_fmt",
  "ROUND_W_fmt",
  "CEIL_L_fmt",
  "CEIL_W_fmt",
  "BC2F_FP_FALSE",
  "BC2T_FP_TRUE",
  "LDC2",
  "LWC2",
  "SDC2",
  "SWC2",
  "CFC2",
  "CTC2",
  "MFC2",
  "MFHC2",
  "MTC2",
  "MTHC2",
  "BC2FL_FALSE_LIKELY",
  "BC2TL_TRUE_LIKELY",
  "CACHE_OPERATION",
  "COP2_OPERATION",
  "EXT",
  "ADDIU_16e_IMM",
  "ADDIU_16e_IMM_Word",
  "ADDIU_16e_IMM_SPRelative",
  "ADDU_16e_REG",
  "AND_16e",
  "B_16e",
  "B_16e_ext", 
  "BEQZ_16e",
  "BEQZ_16e_ext",
  "BNEZ_16e",
  "BNEZ_16e_ext",
  "BREAK_16e",
  "BTEQZ_16e",
  "BTEQZ_16e_ext",
  "BTNEZ_16e",
  "BTNEZ_16e_ext",
  "CMP_16e",
  "CMPI_16e_imm",
  "CMPI_16e_imm_ext",
  "DIV_16e",
  "DIVU_16e",
  "MFHI_16e",
  "MFLO_16e",
  "MOVE_16e",
  "MOVE_16e_r32",
  "MULT_16e",
  "MULTU_16e",
  "NEG_16e",
  "NOT_16e",
  "OR_16e",
  "RESTORE_16e",
  "RESTORE_16e_ext",
  "SAVE_16e",
  "SAVE_16e_ext",
  "SB_16e",
  "SB_16e_ext",
  "SDBBP_16e",
  "SEB_16e",
  "INS",
  "ASMACRO_16e",
  "DMT_MT",
  "DVPE_MT",
  "EMT_MT",
  "EVPE_MT",
  "FORK_MT",
  "MFTR_MT",
  "MTTR_MT",
  "YIELD",
                                             
};

static bool TryExtractLUI_IMM(InstData &inst, uint32_t bits);
static bool TryExtractLW_32_IMM(InstData &inst, uint32_t bits) ;
static bool TryExtractLWC1_FLOATING_POINT(InstData &inst, uint32_t bits);
static bool TryExtractLDC1_FLOATING_R(InstData &inst, uint32_t bits) ;
static bool TryExtractADD_word(InstData &inst, uint32_t bits);
static bool TryExtractADDU_32_R(InstData &inst, uint32_t bits);
static bool TryExtractADDI_32_IMM(InstData &inst, uint32_t bits);
static bool TryExtractADDIU_32_IMM(InstData &inst, uint32_t bits);
static bool TryExtractADD_FMT(InstData &inst, uint32_t bits);
static bool TryExtractDIV_32(InstData &inst, uint32_t bits);
static bool TryExtractDIV_fmt(InstData &inst, uint32_t bits);
static bool TryExtractDIVU(InstData &inst, uint32_t bits);
static bool TryExtractMUL_32(InstData &inst, uint32_t bits);
static bool TryExtractMUL_fmt(InstData &inst, uint32_t bits);
static bool TryExtractMULT(InstData &inst, uint32_t bits);
static bool TryExtractMULTU(InstData &inst, uint32_t bits);
static bool TryExtractMADD(InstData &inst, uint32_t bits);
static bool TryExtractMOV_fmt(InstData &inst, uint32_t bits);
static bool TryExtractMOVZ(InstData &inst, uint32_t bits);
static bool TryExtractMOVN(InstData &inst, uint32_t bits);
static bool TryExtractMOVF(InstData &inst, uint32_t bits);
static bool TryExtractMOVT(InstData &inst, uint32_t bits);
static bool TryExtractMFHI(InstData &inst, uint32_t bits);
static bool TryExtractMFLO(InstData &inst, uint32_t bits);
static bool TryExtractMTLO(InstData &inst, uint32_t bits);
static bool TryExtractTEQ_32(InstData &inst, uint32_t bits);
static bool TryExtractBGTZ_32_IMM_COND(InstData &inst, uint32_t bits);
static bool TryExtractCMP_COND_FMT(InstData &inst, uint32_t bits);
static bool TryExtractCVT_D_FMT(InstData &inst, uint32_t bits);
static bool TryExtractCVT_S_FMT(InstData &inst, uint32_t bits);
static bool TryExtractMFC1_FLOATING_POINT(InstData &inst, uint32_t bits);
static bool TryExtractMFHC1_FLOATING_POINT(InstData &inst, uint32_t bits);
static bool TryExtractMTHC1_FLOATING_POINT(InstData &inst, uint32_t bits);
static bool TryExtractMTC1_FLOATING_POINT(InstData &inst, uint32_t bits);
static bool TryExtractNEG_fmt(InstData &inst, uint32_t bits);
static bool TryExtractJALX_32_DIR_JUMP(InstData &inst, uint32_t bits);
static bool TryExtractSUBU_32_R(InstData &inst, uint32_t bits);
static bool TryExtractSW_32_IMM(InstData &inst, uint32_t bits);
static bool TryExtractSWL(InstData &inst, uint32_t bits);
static bool TryExtractSWC1_FLOATING_POINT(InstData &inst, uint32_t bits);
static bool TryExtractSDC1_FLOATING_R(InstData &inst, uint32_t bits);
static bool TryExtractSH_16_MEM(InstData &inst, uint32_t bits);
static bool TryExtractSEH(InstData &inst, uint32_t bits);
static bool TryExtractLH(InstData &inst, uint32_t bits);
static bool TryExtractLBU_32_R(InstData &inst, uint32_t bits);
static bool TryExtractLB_32_R(InstData &inst, uint32_t bits);
static bool TryExtractLHU_32_R(InstData &inst, uint32_t bits);
static bool TryExtractNOP(InstData &inst, uint32_t bits);
static bool TryExtractSLTIU_IMM(InstData &inst, uint32_t bits);
static bool TryExtractSLTI_IMM(InstData &inst, uint32_t bits);
static bool TryExtractSLT_32_R(InstData &inst, uint32_t bits);
static bool TryExtractSLTU_32_R(InstData &inst, uint32_t bits);
static bool TryExtractSUB_FMT(InstData &inst, uint32_t bits);
static bool TryExtractOR_32_R(InstData &inst, uint32_t bits);
static bool TryExtractORI_32_IMM(InstData &inst, uint32_t bits);
static bool TryExtractAND_32_R(InstData &inst, uint32_t bits);
static bool TryExtractANDI_32_IMM(InstData &inst, uint32_t bits);
static bool TryExtractNOR_32_R(InstData &inst, uint32_t bits);
static bool TryExtractXOR_32_R(InstData &inst, uint32_t bits);
static bool TryExtractXORI_32_IMM(InstData &inst, uint32_t bits);
static bool TryExtractJR_32_R(InstData &inst, uint32_t bits);
static bool TryExtractJ_Target(InstData &inst, uint32_t bits);
static bool TryExtractJALR_32_R(InstData &inst, uint32_t bits);
static bool TryExtractJAL_Target(InstData &inst, uint32_t bits);
static bool TryExtractJALX_JUMP_LINK(InstData &inst, uint32_t bits);
static bool TryExtractSRA_RIGHT_SHIFT(InstData &inst, uint32_t bits);
static bool TryExtractSRAV_RIGHT_ARITHMETIC_VARIABLE(InstData &inst, uint32_t bits);
static bool TryExtractSRL_RIGHT_SHIFT_LOGICAL(InstData &inst, uint32_t bits);
static bool TryExtractSLL_LEFT_SHIFT_LOGICAL(InstData &inst, uint32_t bits);
static bool TryExtractSLLV_LEFT_LOGICAL_VARIABLE(InstData &inst, uint32_t bits);
static bool TryExtractSRLV_RIGHT_LOGICAL_VARIABLE(InstData &inst, uint32_t bits);
static bool TryExtractSB_IMM(InstData &inst, uint32_t bits);
static bool TryExtractSEB_EXTEND_BYTE(InstData &inst, uint32_t bits);
static bool TryExtractBEQ_BRANCH_EQUAL(InstData &inst, uint32_t bits);
static bool TryExtractBNE_BRANCH_NOT_EQUAL(InstData &inst, uint32_t bits);
static bool TryExtractB_32_offset(InstData &inst, uint32_t bits);
static bool TryExtractBGEZAL_LINK(InstData &inst, uint32_t bits);
static bool TryExtractBC1F_FP_FALSE(InstData &inst, uint32_t bits);
static bool TryExtractBC1T_FP_TRUE(InstData &inst, uint32_t bits);
static bool TryExtractB_IMM(InstData &inst, uint32_t bits);
static bool TryExtractBAL_BRANCH_LINK(InstData &inst, uint32_t bits);
static bool TryExtractBGEZ(InstData &inst, uint32_t bits);
static bool TryExtractBLEZ(InstData &inst, uint32_t bits);
static bool TryExtractBLTZ(InstData &inst, uint32_t bits);
static bool TryExtractBEQL_BRANCH_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractBLTZAL(InstData &inst, uint32_t bits);
static bool TryExtractBGTZL_BRANCH_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractBGEZL_BRANCH_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractBLTZL_BRANCH_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractBNEL_BRANCH_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractLL_LOAD_LINKED_WORD(InstData &inst, uint32_t bits);
static bool TryExtractROTRV(InstData &inst, uint32_t bits);
static bool TryExtractROTR(InstData &inst, uint32_t bits);
static bool TryExtractCFC1_FLOATING_POINT(InstData &inst, uint32_t bits);
static bool TryExtractCTC1(InstData &inst, uint32_t bits);
static bool TryExtractSC_STORE_CONDITIONAL_WORD(InstData &inst, uint32_t bits);
static bool TryExtractSUB_WORD(InstData &inst, uint32_t bits);
static bool TryExtractJALR_HB(InstData &inst, uint32_t bits);
static bool TryExtractJR_HB(InstData &inst, uint32_t bits);
static bool TryExtractSSNOP_NO_OPERATION(InstData &inst, uint32_t bits);
static bool TryExtractLWL_LOAD_WORD_LEFT(InstData &inst, uint32_t bits);
static bool TryExtractLWR_LOAD_WORD_RIGHT(InstData &inst, uint32_t bits);
static bool TryExtractPREF(InstData &inst, uint32_t bits);
static bool TryExtractSYNCI(InstData &inst, uint32_t bits);
static bool TryExtractBC1FL_FP_FALSE_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractBC1TL_FP_TRUE_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractROUND_L_fmt(InstData &inst, uint32_t bits);
static bool TryExtractROUND_W_fmt(InstData &inst, uint32_t bits);
static bool TryExtractCEIL_L_fmt(InstData &inst, uint32_t bits);
static bool TryExtractCEIL_W_fmt(InstData &inst, uint32_t bits);
static bool TryExtractBC2F_FP_FALSE(InstData &inst, uint32_t bits);
static bool TryExtractBC2T_FP_TRUE(InstData &inst, uint32_t bits);
static bool TryExtractLDC2(InstData &inst, uint32_t bits);
static bool TryExtractLWC2(InstData &inst, uint32_t bits);
static bool TryExtractSDC2(InstData &inst, uint32_t bits);
static bool TryExtractSWC2(InstData &inst, uint32_t bits);
static bool TryExtractCFC2(InstData &inst, uint32_t bits);
static bool TryExtractCTC2(InstData &inst, uint32_t bits);
static bool TryExtractMFC2(InstData &inst, uint32_t bits);
static bool TryExtractMFHC2(InstData &inst, uint32_t bits);
static bool TryExtractMTC2(InstData &inst, uint32_t bits);
static bool TryExtractMTHC2(InstData &inst, uint32_t bits);
static bool TryExtractBC2FL_FALSE_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractBC2TL_TRUE_LIKELY(InstData &inst, uint32_t bits);
static bool TryExtractCACHE_OPERATION(InstData &inst, uint32_t bits);
static bool TryExtractCOP2_OPERATION(InstData &inst, uint32_t bits);
static bool TryExtractADDIU_16e_IMM(InstData &inst, uint32_t bits);
static bool TryExtractADDIU_16e_IMM_Word(InstData &inst, uint32_t bits);
static bool TryExtractADDIU_16e_IMM_SPRelative(InstData &inst, uint32_t bits);
static bool TryExtractADDU_16e_REG(InstData &inst, uint32_t bits);
static bool TryExtractAND_16e(InstData &inst, uint32_t bits);
static bool TryExtractB_16e(InstData &inst, uint32_t bits);
static bool TryExtractB_16e_ext(InstData &inst, uint32_t bits);
static bool TryExtractBEQZ_16e(InstData &inst, uint32_t bits);
static bool TryExtractBEQZ_16e_ext(InstData &inst, uint32_t bits);
static bool TryExtractBNEZ_16e(InstData &inst, uint32_t bits);
static bool TryExtractBNEZ_16e_ext(InstData &inst, uint32_t bits);
static bool TryExtractBREAK_16e(InstData &inst, uint32_t bits);
static bool TryExtractBTEQZ_16e(InstData &inst, uint32_t bits);
static bool TryExtractBTEQZ_16e_ext(InstData &inst, uint32_t bits);
static bool TryExtractBTNEZ_16e(InstData &inst, uint32_t bits);
static bool TryExtractBTNEZ_16e_ext(InstData &inst, uint32_t bits);
static bool TryExtractCMP_16e(InstData &inst, uint32_t bits);
static bool TryExtractCMPI_16e_imm(InstData &inst, uint32_t bits);
static bool TryExtractCMPI_16e_imm_ext(InstData &inst, uint32_t bits);
static bool TryExtractDIV_16e(InstData &inst, uint32_t bits);
static bool TryExtractDIVU_16e(InstData &inst, uint32_t bits);
static bool TryExtractMFLO_16e(InstData &inst, uint32_t bits);
static bool TryExtractMTLO_16e(InstData &inst, uint32_t bits);

static bool TryExtractMOVE_16e(InstData &inst, uint32_t bits);
static bool TryExtractMOVE_16e_r32(InstData &inst, uint32_t bits);
static bool TryExtractMULT_16e(InstData &inst, uint32_t bits);
static bool TryExtractMULTU_16e(InstData &inst, uint32_t bits);


static bool TryExtractNEG_16e(InstData &inst, uint32_t bits);
static bool TryExtractNOT_16e(InstData &inst, uint32_t bits);
static bool TryExtractOR_16e(InstData &inst, uint32_t bits);
static bool TryExtractRESTORE_16e(InstData &inst, uint32_t bits);
static bool TryExtractRESTORE_16e_ext(InstData &inst, uint32_t bits);
static bool TryExtractSAVE_16e(InstData &inst, uint32_t bits);
static bool TryExtractSAVE_16e_ext(InstData &inst, uint32_t bits);
static bool TryExtractSB_16e(InstData &inst, uint32_t bits);
static bool TryExtractSB_16e_ext(InstData &inst, uint32_t bits);
static bool TryExtractSDBBP_16e(InstData &inst, uint32_t bits);
static bool TryExtractSEB_16e(InstData &inst, uint32_t bits);

static bool TryExtractINS(InstData &inst, uint32_t bits);
static bool TryExtractEXT(InstData &inst, uint32_t bits);
static bool TryExtractASMACRO_16e(InstData &inst, uint32_t bits);
static bool TryExtractDMT_MT(InstData &inst, uint32_t bits);
static bool TryExtractDVPE_MT(InstData &inst, uint32_t bits);
static bool TryExtractEMT_MT(InstData &inst, uint32_t bits);
static bool TryExtractEVPE_MT(InstData &inst, uint32_t bits);

static bool TryExtractFORK_MT(InstData &inst, uint32_t bits);
static bool TryExtractMFTR_MT(InstData &inst, uint32_t bits);
static bool TryExtractMTTR_MT(InstData &inst, uint32_t bits);
static bool TryExtractYIELD_MT(InstData &inst, uint32_t bits);


static bool TryExtractLUI_IMM( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000000000
  //   --------------------------------
  //   00111100000000000000000000000000
  if ((bits & 0xffe00000U) != 0x3c000000U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t IMM:16;
      uint32_t rt:5;
      uint32_t rs:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.IMM = static_cast<uint16_t>(enc.IMM);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LUI_IMM;                        
  inst.iclass = InstName::LUI;
  return true;
}

static bool TryExtractLW_32_IMM( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10001100000000000000000000000000
  if ((bits & 0xfc000000U) != 0x8c000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LW_32_IMM;
  inst.iclass = InstName::LW;
  return true;
}  

static bool TryExtractLWL_LOAD_WORD_LEFT( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10001000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x88000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LWL_LOAD_WORD_LEFT;
  inst.iclass = InstName::LWL;
  return true;
}  

static bool TryExtractLWR_LOAD_WORD_RIGHT( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10011000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x98000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LWR_LOAD_WORD_RIGHT;
  inst.iclass = InstName::LWR;
  return true;
}  

static bool TryExtractLWC1_FLOATING_POINT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11000100000000000000000000000000
  if((bits & 0xfc000000U) != 0xc4000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t ft:5;
      uint32_t base:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.ft = static_cast<uint8_t>(enc.ft);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LWC1_FLOATING_POINT;
  inst.iclass = InstName::LWC1;
  return true;
}

static bool TryExtractLDC1_FLOATING_R( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11010100000000000000000000000000
  if ((bits & 0xfc000000U) != 0xd4000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t ft:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.ft);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LDC1_FLOATING_R;
  inst.iclass = InstName::LDC1;
  return true;
}  

static bool TryExtractLDC2( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11011000000000000000000000000000
  if ((bits & 0xfc000000U) != 0xd8000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LDC2;
  inst.iclass = InstName::LDC2;
  return true;
}  

static bool TryExtractSDC2( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11111000000000000000000000000000
  if ((bits & 0xfc000000U) != 0xf8000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SDC2;
  inst.iclass = InstName::SDC2;
  return true;
}  

static bool TryExtractSWC2( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11101000000000000000000000000000
  if ((bits & 0xfc000000U) != 0xe8000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SWC2;
  inst.iclass = InstName::SWC2;
  return true;
}  

static bool TryExtractLWC2( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11001000000000000000000000000000
  if ((bits & 0xfc000000U) != 0xc8000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LWC2;
  inst.iclass = InstName::LWC2;
  return true;
}  

static bool TryExtractPREF( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11001100000000000000000000000000
  if ((bits & 0xfc000000U) != 0xcc000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t hint:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.hint = static_cast<uint8_t>(enc.hint);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::PREF;
  inst.iclass = InstName::PREF;
  return true;
}  

static bool TryExtractSYNCI( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00000100000111110000000000000000
  if ((bits & 0xfc1f0000U) != 0x41f0000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t hint:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.hint = static_cast<uint8_t>(enc.hint);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SYNCI;
  inst.iclass = InstName::SYNCI;
  return true;
}  

static bool TryExtractSC_STORE_CONDITIONAL_WORD(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11100000000000000000000000000000
  if((bits & 0xfc000000U) != 0xe0000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt); 
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SC_STORE_CONDITIONAL_WORD;
  inst.iclass = InstName::SC;
  return true;
}

static bool TryExtractSW_32_IMM( InstData  &inst, uint32_t bits) {
  //   bits
  // &11111100000000000000000000000000 
  //   --------------------------------
  //  10101100000000000000000000000000 
  if ((bits & 0xfc000000U) != 0xac000000U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SW_32_IMM;                        
  inst.iclass = InstName::SW;
  return true;
}

static bool TryExtractSWL( InstData  &inst, uint32_t bits) {
  //   bits
  // &11111100000000000000000000000000 
  //   --------------------------------
  //  10101000000000000000000000000000 
  if ((bits & 0xfc000000U) != 0xa8000000U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SWL;                        
  inst.iclass = InstName::SWL;
  return true;
}

static bool TryExtractSWC1_FLOATING_POINT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   11100100000000000000000000000000
  if ((bits & 0xfc000000U) != 0xe4000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t ft:5;
      uint32_t base:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.ft); 
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SWC1_FLOATING_POINT;
  inst.iclass = InstName::SWC1;
  return true;
}

static bool TryExtractSDC1_FLOATING_R( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11110100000000000000000000000000
  if ((bits & 0xfc000000U) != 0xf4000000U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t offset:16;
      uint32_t ft:5;
      uint32_t base:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.ft);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SDC1_FLOATING_R;                        
  inst.iclass = InstName::SDC1;
  return true;
}

static bool TryExtractSH_16_MEM( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10100100000000000000000000000000
  if ((bits & 0xfc000000U) != 0xa4000000U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SH_16_MEM;                        
  inst.iclass = InstName::SH;
  return true;
}

static bool TryExtractSEH( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01111100000000000000011000100000
  
  if ((bits & 0xFFE007FFU) != 0x7C000620U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;
      uint32_t rt:5;
      uint32_t _0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SEH;                        
  inst.iclass = InstName::SEH;
  return true;
}

static bool TryExtractROUND_L_fmt( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01111100000000000000011000100000
  
  if ((bits & 0xFFE007FFU) != 0x7C000620U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t fd:5;
      uint32_t fs:5;
      uint32_t _0:5;
      uint32_t fmt:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.fd = static_cast<uint8_t>(enc.fd);
  inst.fs = static_cast<uint8_t>(enc.fs);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ROUND_L_fmt;                        
  inst.iclass = InstName::ROUND_L_fmt;
  return true;
}

static bool TryExtractROUND_W_fmt( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000111111
  //   --------------------------------
  //   01000100000000000000000000001100
  
  if ((bits & 0xfc1f003fU) != 0x4400000cU) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t fd:5;
      uint32_t fs:5;
      uint32_t _0:5;
      uint32_t fmt:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.fd = static_cast<uint8_t>(enc.fd);
  inst.fs = static_cast<uint8_t>(enc.fs);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ROUND_W_fmt;                        
  inst.iclass = InstName::ROUND_W_fmt;
  return true;
}

static bool TryExtractCEIL_L_fmt( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000111111
  //   --------------------------------
  //   01000100000000000000000000001010
  
  if ((bits & 0xfc1f003fU) != 0x4400000aU) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t fd:5;
      uint32_t fs:5;
      uint32_t _0:5;
      uint32_t fmt:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.fd = static_cast<uint8_t>(enc.fd);
  inst.fs = static_cast<uint8_t>(enc.fs);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CEIL_L_fmt;                        
  inst.iclass = InstName::CEIL_L_fmt;
  return true;
}

static bool TryExtractCEIL_W_fmt( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000111111
  //   --------------------------------
  //   01000100000000000000000000001110
  
  if ((bits & 0xfc1f003fU) != 0x4400000eU) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t fd:5;
      uint32_t fs:5;
      uint32_t _0:5;
      uint32_t fmt:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.fd = static_cast<uint8_t>(enc.fd);
  inst.fs = static_cast<uint8_t>(enc.fs);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CEIL_W_fmt;                        
  inst.iclass = InstName::CEIL_W_fmt;
  return true;
}

static bool TryExtractSUB_WORD( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000100010
  if ((bits & 0xFC0007FFU) != 0x22U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;
      uint32_t rt:5;
      uint32_t rs:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SUB_WORD;                        
  inst.iclass = InstName::SUB;
  return true;
}

static bool TryExtractCFC1_FLOATING_POINT( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01000100010000000000000000000000
  
  if ((bits & 0xFFE007FFU) != 0x44400000U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t cfc1_0:11;
      uint32_t fs:5;
      uint32_t rt:5;
      uint32_t cf_0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.cfc1_0 = static_cast<uint16_t>(enc.cfc1_0);
  inst.fs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.cfc1_0 = static_cast<uint8_t>(enc.cfc1_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CFC1_FLOATING_POINT;                        
  inst.iclass = InstName::CFC1;
  return true;
}

static bool TryExtractCFC2( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000000000
  //   --------------------------------
  //   01001000010000000000000000000000
  
  if ((bits & 0xFFE00000U) != 0x48400000U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t impl:16; 
      uint32_t rt:5;
      uint32_t cf_0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.impl = static_cast<uint16_t>(enc.impl);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.cf_0 = static_cast<uint8_t>(enc.cf_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CFC2;                        
  inst.iclass = InstName::CFC2;
  return true;
}

static bool TryExtractCTC2( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000000000
  //   --------------------------------
  //   01001000110000000000000000000000
  
  if ((bits & 0xFFE00000U) != 0x48c00000U) {
    return false;
  }
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t impl:16; 
      uint32_t rt:5;
      uint32_t cf_0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.impl = static_cast<uint16_t>(enc.impl);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.cf_0 = static_cast<uint8_t>(enc.cf_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CTC2;                        
  inst.iclass = InstName::CTC2;
  return true;
}

static bool TryExtractMFC2( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000000000
  //   --------------------------------
  //   01001000000000000000000000000000
  
  if ((bits & 0xFFE00000U) != 0x48000000U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t impl:16; 
      uint32_t rt:5;
      uint32_t cf_0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.impl = static_cast<uint16_t>(enc.impl);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.cf_0 = static_cast<uint8_t>(enc.cf_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFC2;                        
  inst.iclass = InstName::MFC2;
  return true;
}

static bool TryExtractMFHC2( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000000000
  //   --------------------------------
  //   01001000011000000000000000000000
  
  if ((bits & 0xFFE00000U) != 0x48600000U) {
    return false;
  }
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t impl:16; 
      uint32_t rt:5;
      uint32_t cf_0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.impl = static_cast<uint16_t>(enc.impl);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.cf_0 = static_cast<uint8_t>(enc.cf_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFHC2;                        
  inst.iclass = InstName::MFHC2;
  return true;
}

static bool TryExtractMTC2( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000000000
  //   --------------------------------
  //   01001000100000000000000000000000
  
  if ((bits & 0xFFE00000U) != 0x48800000U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t impl:16; 
      uint32_t rt:5;
      uint32_t cf_0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.impl = static_cast<uint16_t>(enc.impl);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.cf_0 = static_cast<uint8_t>(enc.cf_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MTC2;                        
  inst.iclass = InstName::MTC2;
  return true;
}

static bool TryExtractMTHC2( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000000000
  //   --------------------------------
  //   01001000111000000000000000000000
  
  if ((bits & 0xFFE00000U) != 0x48e00000U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t impl:16; 
      uint32_t rt:5;
      uint32_t cf_0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.impl = static_cast<uint16_t>(enc.impl);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.cf_0 = static_cast<uint8_t>(enc.cf_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MTHC2;                        
  inst.iclass = InstName::MTHC2;
  return true;
}

static bool TryExtractCTC1( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01000100110000000000000000000000 
  
  if ((bits & 0xFFE007FFU) != 0x44c00000U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t cfc1_0:11;
      uint32_t fs:5;
      uint32_t rt:5;
      uint32_t cf_0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.cfc1_0 = static_cast<uint16_t>(enc.cfc1_0);
  inst.fs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.cfc1_0 = static_cast<uint8_t>(enc.cfc1_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CTC1;                        
  inst.iclass = InstName::CTC1;
  return true;
}

static bool TryExtractROTRV( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000001000110
  
  if ((bits & 0xFC0007FFU) != 0x46U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t _R:1;
      uint32_t _0:4;
      uint32_t rd:5;
      uint32_t rt:5;
      uint32_t rs:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._R = static_cast<uint8_t>(enc._R);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ROTRV;                        
  inst.iclass = InstName::ROTRV;
  return true;
}

static bool TryExtractROTR( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000001000110
  
  if ((bits & 0xFC0007FFU) != 0x46U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t _sa:5;
      uint32_t rd:5;
      uint32_t rt:5; 
      uint32_t _R:1;
      uint32_t _0:4;
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._sa = static_cast<uint8_t>(enc._sa);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst._R = static_cast<uint8_t>(enc._R);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ROTR;                        
  inst.iclass = InstName::ROTR;
  return true;
}

static bool TryExtractLH( InstData  &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10000100000000000000000000000000
    if ((bits & 0xfc000000U) != 0x84000000U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LH;                        
  inst.iclass = InstName::LH;
  return true;
}

static bool TryExtractSB_IMM( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10100000000000000000000000000000
  if ((bits & 0xfc000000U) != 0xa0000000U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SB_IMM;                        
  inst.iclass = InstName::SB;
  return true;
}

static bool TryExtractSEB_EXTEND_BYTE( InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01111100000000000000010000100000
  if ((bits & 0xFFE007FFU) != 0x7C000420U) {
    return false;
  }

  union {
    uint32_t flat;                                                       
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;
      uint32_t rt:5;
      uint32_t _0:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SEB_EXTEND_BYTE;                        
  inst.iclass = InstName::SEB;
  return true;
}

static bool TryExtractLBU_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10010000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x90000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LBU_32_R;
  inst.iclass = InstName::LBU;
  return true;
}  

static bool TryExtractLB_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10000000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x80000000U) {
    return false;
  }
    union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LB_32_R;
  inst.iclass = InstName::LB;
  return true;
}  

static bool TryExtractLHU_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10010100000000000000000000000000
  if ((bits & 0xfc000000U) != 0x94000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LHU_32_R;
  inst.iclass = InstName::LHU;
  return true;
}  

static bool TryExtractSLTIU_IMM(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00101100000000000000000000000000
  if ((bits & 0xfc000000U) != 0x2c000000U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t SIGNED_IMM:16;
      uint32_t rt:5;
      uint32_t rs:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.SIGNED_IMM = static_cast<int16_t>(enc.SIGNED_IMM);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SLTIU_IMM;                        
  inst.iclass = InstName::SLTIU;
  return true;
}

static bool TryExtractSLTI_IMM(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00101000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x28000000U) {
    return false;
  }
  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t SIGNED_IMM:16;
      uint32_t rt:5;
      uint32_t rs:5;  
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.SIGNED_IMM = static_cast<int16_t>(enc.SIGNED_IMM);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SLTI_IMM;                        
  inst.iclass = InstName::SLTI;
  return true;
}

static bool TryExtractSLT_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000101010 
  if ((bits & 0xfc0007ffU) != 0x0000002aU) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SLT_32_R;
  inst.iclass = InstName::SLT;
  return true;
  
}

static bool TryExtractBEQ_BRANCH_EQUAL(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00010000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x10000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BEQ_BRANCH_EQUAL;
  inst.iclass = InstName::BEQ;
  return true;
}  

static bool TryExtractBAL_BRANCH_LINK(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111111110000000000000000
  //   --------------------------------
  //   00000100000100010000000000000000
  if ((bits & 0xffff0000U) != 0x4110000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t _0:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BAL_BRANCH_LINK;
  inst.iclass = InstName::BAL;
  return true;
}  

static bool TryExtractBNE_BRANCH_NOT_EQUAL(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00010100000000000000000000000000
  if ((bits & 0xfc000000U) != 0x14000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BNE_BRANCH_NOT_EQUAL;
  inst.iclass = InstName::BNE;
  return true;
}  

static bool TryExtractB_32_offset(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111111110000000000000000
  //   --------------------------------
  //   00010000000000000000000000000000
  if ((bits & 0xffff0000U) != 0x10000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::B_32_offset;
  inst.iclass = InstName::B;
  return true;
}  

static bool TryExtractBGEZAL_LINK(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00000100000100010000000000000000
  if ((bits & 0xfc1f0000U) != 0x4110000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BGEZAL_LINK;
  inst.iclass = InstName::BGEZAL;
  return true;
}  

static bool TryExtractBGEZ(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00000100000000010000000000000000
  if ((bits & 0xfc1f0000U) != 0x4010000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BGEZ;
  inst.iclass = InstName::BGEZ;
  return true;
}  

static bool TryExtractBGEZL_BRANCH_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00000100000000110000000000000000
  if ((bits & 0xfc1f0000U) != 0x4030000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BGEZL_BRANCH_LIKELY;
  inst.iclass = InstName::BGEZL;
  return true;
}  

static bool TryExtractBLEZ(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00011000000000000000000000000000
  if ((bits & 0xfc1f0000U) != 0x18000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BLEZ;
  inst.iclass = InstName::BLEZ;
  return true;
}  

static bool TryExtractBLTZ(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00000100000000000000000000000000
  if ((bits & 0xfc1f0000U) != 0x4000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BLTZ;
  inst.iclass = InstName::BLTZ;
  return true;
}  

static bool TryExtractBNEL_BRANCH_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   01010100000000000000000000000000
  if ((bits & 0xfc000000U) != 0x54000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BNEL_BRANCH_LIKELY;
  inst.iclass = InstName::BNEL;
  return true;
}  

static bool TryExtractBLTZL_BRANCH_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00000100000000100000000000000000
  if ((bits & 0xfc1f0000U) != 0x4020000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BLTZL_BRANCH_LIKELY;
  inst.iclass = InstName::BLTZL;
  return true;
}  

static bool TryExtractBEQL_BRANCH_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   01010000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x50000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BEQL_BRANCH_LIKELY;
  inst.iclass = InstName::BEQL;
  return true;
}  

static bool TryExtractBLTZAL(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00000100000100000000000000000000
  if ((bits & 0xfc1f0000U) != 0x4100000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BLTZAL;
  inst.iclass = InstName::BLTZAL;
  return true;
}  

static bool TryExtractBGTZL_BRANCH_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   01011100000000000000000000000000
  if ((bits & 0xfc1f0000U) != 0x5c000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BGTZL_BRANCH_LIKELY;
  inst.iclass = InstName::BGTZL;
  return true;
}  

static bool TryExtractBC1F_FP_FALSE(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000110000000000000000
  //   --------------------------------
  //   01000101000000000000000000000000
  if ((bits & 0xFFE30000U) != 0x45000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t tf:1;
      uint32_t nd:1;
      uint32_t cc:3;
      uint32_t bc:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst.nd = static_cast<uint8_t>(enc.nd);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.bc = static_cast<uint8_t>(enc.bc);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BC1F_FP_FALSE;
  inst.iclass = InstName::BC1F;
  return true;
}

static bool TryExtractBC2F_FP_FALSE(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000110000000000000000
  //   --------------------------------
  //   01001001000000000000000000000000
  if ((bits & 0xFFE30000U) != 0x49000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t tf:1;
      uint32_t nd:1;
      uint32_t cc:3;
      uint32_t bc:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst.nd = static_cast<uint8_t>(enc.nd);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.bc = static_cast<uint8_t>(enc.bc);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BC2F_FP_FALSE;
  inst.iclass = InstName::BC2F;
  return true;
}

static bool TryExtractBC2FL_FALSE_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000110000000000000000
  //   --------------------------------
  //   01001001000000100000000000000000
  if ((bits & 0xFFE30000U) != 0x49020000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t tf:1;
      uint32_t nd:1;
      uint32_t cc:3;
      uint32_t bc:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst.nd = static_cast<uint8_t>(enc.nd);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.bc = static_cast<uint8_t>(enc.bc);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BC2FL_FALSE_LIKELY;
  inst.iclass = InstName::BC2FL;
  return true;
}

static bool TryExtractBC1FL_FP_FALSE_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000110000000000000000
  //   --------------------------------
  //   01000101000000100000000000000000
  if ((bits & 0xFFE30000U) != 0x45020000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t tf:1;
      uint32_t nd:1;
      uint32_t cc:3;
      uint32_t bc:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst.nd = static_cast<uint8_t>(enc.nd);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.bc = static_cast<uint8_t>(enc.bc);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BC1FL_FP_FALSE_LIKELY;
  inst.iclass = InstName::BC1FL;
  return true;
}

static bool TryExtractBC1T_FP_TRUE(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000110000000000000000
  //   --------------------------------
  //   01000101000000010000000000000000
  if ((bits & 0xFFE30000U) != 0x45010000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t tf:1;
      uint32_t nd:1;
      uint32_t cc:3;
      uint32_t bc:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst.nd = static_cast<uint8_t>(enc.nd);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.bc = static_cast<uint8_t>(enc.bc);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BC1T_FP_TRUE;
  inst.iclass = InstName::BC1T;
  return true;
}

static bool TryExtractBC2T_FP_TRUE(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000110000000000000000
  //   --------------------------------
  //   01001001000000010000000000000000
  if ((bits & 0xFFE30000U) != 0x49010000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t tf:1;
      uint32_t nd:1;
      uint32_t cc:3;
      uint32_t bc:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst.nd = static_cast<uint8_t>(enc.nd);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.bc = static_cast<uint8_t>(enc.bc);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BC2T_FP_TRUE;
  inst.iclass = InstName::BC2T;
  return true;
}

static bool TryExtractBC2TL_TRUE_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000110000000000000000
  //   --------------------------------
  //   01001001000000110000000000000000
  if ((bits & 0xFFE30000U) != 0x49030000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t tf:1;
      uint32_t nd:1;
      uint32_t cc:3;
      uint32_t bc:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst.nd = static_cast<uint8_t>(enc.nd);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.bc = static_cast<uint8_t>(enc.bc);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BC2TL_TRUE_LIKELY;
  inst.iclass = InstName::BC2TL;
  return true;
}

static bool TryExtractBC1TL_FP_TRUE_LIKELY(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000110000000000000000
  //   --------------------------------
  //   01000101000000110000000000000000
  if ((bits & 0xFFE30000U) != 0x45030000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t tf:1;
      uint32_t nd:1;
      uint32_t cc:3;
      uint32_t bc:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst.nd = static_cast<uint8_t>(enc.nd);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.bc = static_cast<uint8_t>(enc.bc);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BC1TL_FP_TRUE_LIKELY;
  inst.iclass = InstName::BC1TL;
  return true;
}

static bool TryExtractSLTU_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000101011 
  if ((bits & 0xfc0007ffU) != 0x0000002bU) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SLTU_32_R;
  inst.iclass = InstName::SLTU;
  return true;
  
}

static bool TryExtractSUB_FMT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000111111
  //   --------------------------------
  //   01000100000000000000000000000001
  if ((bits & 0xfc00003fU) != 0x44000001U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t fd:5;
      uint32_t fs:5;  
      uint32_t ft:5;  
      uint32_t fmt:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rd = static_cast<uint8_t>(enc.fd);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.ft);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SUB_FMT;
  inst.iclass = InstName::SUB_FMT;
  return true;
}

static bool TryExtractOR_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //    00000000000000000000000000100101
  if ((bits & 0xfc0007ffU) != 0x00000025U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::OR_32_R;
  inst.iclass = InstName::OR;
  return true;
  
}

static bool TryExtractORI_32_IMM(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00110100000000000000000000000000
  if ((bits & 0xfc000000U) != 0x34000000U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t IMM:16;
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.IMM = static_cast<int16_t>(enc.IMM);  
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ORI_32_IMM;
  inst.iclass = InstName::ORI;
  return true;
  
}

static bool TryExtractNOR_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000100111
  if ((bits & 0xfc0007ffU) != 0x00000027U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::NOR_32_R;
  inst.iclass = InstName::NOR;
  return true;
  
}

static bool TryExtractAND_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //    00000000000000000000000000100100
  if ((bits & 0xfc0007ffU) != 0x00000024U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::AND_32_R;
  inst.iclass = InstName::AND;
  return true;
  
}

static bool TryExtractAND_16e(InstData &inst, uint32_t bits) {
  //   bits
  // &  1111100000011111
  //   -----------------
  //    1110100000001100
  if ((bits & 0xf81fU) != 0xe80cU) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t ry:3;  
      uint32_t rx:3;  
      uint32_t opcode:5;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.ry = static_cast<uint8_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::AND_16e;
  inst.iclass = InstName::AND_16e;
  return true;
 
}

static bool TryExtractANDI_32_IMM(InstData &inst, uint32_t bits){
//   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00110000000000000000000000000000 
  if ((bits & 0xfc000000U) != 0x30000000U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t IMM:16;
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.IMM = static_cast<uint16_t>(enc.IMM);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ANDI_32_IMM;
  inst.iclass = InstName::ANDI;
  return true;
}

static bool TryExtractXOR_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000100110
  if ((bits & 0xfc0007ffU) != 0x00000026U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::XOR_32_R;
  inst.iclass = InstName::XOR;
  return true;
  
}

static bool TryExtractXORI_32_IMM(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00111000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x38000000U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t IMM:16; 
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.IMM = static_cast<uint16_t>(enc.IMM);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::XORI_32_IMM;
  inst.iclass = InstName::XORI;
  return true;
  
}

static bool TryExtractADDU_32_R(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000100001
  if ((bits & 0xfc0007ffU) != 0x21U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADDU_32_R;
  inst.iclass = InstName::ADDU;
  return true;
}

static bool TryExtractADD_word(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000100000
  if ((bits & 0xfc0007ffU) != 0x00000020U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADD_word;
  inst.iclass = InstName::ADD;
  return true;
}

static bool TryExtractADD_FMT(InstData &inst, uint32_t bits) {
  // prevents the below listed instructions from being
  // decoded as add.fmt
  if(true && TryExtractMTC1_FLOATING_POINT(inst, bits)) return true;
  if(true && TryExtractMFC1_FLOATING_POINT(inst, bits)) return true;
  if(true && TryExtractMFHC1_FLOATING_POINT(inst, bits)) return true;
  if(true && TryExtractMTHC1_FLOATING_POINT(inst, bits)) return true;

  //   bits
  // & 11111100000000000000000000111111
  //   --------------------------------
  //   01000100000000000000000000000000
  if ((bits & 0xfc00003FU) != 0x44000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t fd:5;
      uint32_t fs:5;
      uint32_t ft:5;  
      uint32_t fmt:5;  
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.fmt = static_cast<uint8_t>(enc.fmt); 
  inst.rt = static_cast<uint8_t>(enc.ft);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst.rd = static_cast<uint8_t>(enc.fd);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADD_FMT;
  inst.iclass = InstName::ADD_FMT;
  return true;
}

static bool TryExtractDIV_32(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000001111111111111111
  //   --------------------------------
  //   00000000000000000000000000011010
  if ((bits & 0xFC00FFFFU) != 0x0000001AU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t div_0:10;  
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.div_0 = static_cast<uint16_t>(enc.div_0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::DIV_32;
  inst.iclass = InstName::DIV;
  return true;
}

static bool TryExtractDIV_fmt(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000000000111111
  //   --------------------------------
  //   01000100000000000000000000000011
  if ((bits & 0xFC00003FU) != 0x44000003U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t fd:5; 
      uint32_t fs:5; 
      uint32_t ft:5;
      uint32_t fmt:5;
      uint32_t opcode:6;
       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rd = static_cast<uint8_t>(enc.fd);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.ft);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::DIV_fmt;
  inst.iclass = InstName::DIV_fmt;
  return true;
}

static bool TryExtractDIVU(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000001111111111111111
  //   --------------------------------
  //   00000000000000000000000000011011
  if ((bits & 0xFC00FFFFU) != 0x0000001BU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t div_0:10;  
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.div_0 = static_cast<uint16_t>(enc.div_0);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::DIVU;
  inst.iclass = InstName::DIVU;
  return true;
}

static bool TryExtractLL_LOAD_LINKED_WORD(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   11000000000000000000000000000000
  if ((bits & 0xFC000000U) != 0xc0000000U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t rt:5;
      uint32_t base:5;
      uint32_t opcode:6;
       
       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::LL_LOAD_LINKED_WORD;
  inst.iclass = InstName::LL;
  return true;
}

static bool TryExtractMUL_32(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   01110000000000000000000000000010
  if ((bits & 0xFC0007FFU) != 0x70000002U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5; 
      uint32_t rd:5; 
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MUL_32;
  inst.iclass = InstName::MUL;
  return true;
}

static bool TryExtractMUL_fmt(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000000000111111
  //   --------------------------------
  //   01000100000000000000000000000010
  if ((bits & 0xFC00003FU) != 0x44000002U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t fd:5; 
      uint32_t fs:5; 
      uint32_t ft:5;
      uint32_t fmt:5;
      uint32_t opcode:6;
       
       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rd = static_cast<uint8_t>(enc.fd);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.ft);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MUL_fmt;
  inst.iclass = InstName::MUL_fmt;
  return true;
}

static bool TryExtractMULT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000001111111111111111
  //   --------------------------------
  //   00000000000000000000000000011000
  if ((bits & 0xFC00FFFFU) != 0x00000018U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t mult_0:10; 
      uint32_t rt:5; 
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.mult_0 = static_cast<uint16_t>(enc.mult_0);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MULT;
  inst.iclass = InstName::MULT;
  return true;
}

static bool TryExtractMULTU(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000001111111111111111
  //   --------------------------------
  //   00000000000000000000000000011001
  if ((bits & 0xFC00FFFFU) != 0x00000019U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t mult_0:10; 
      uint32_t rt:5; 
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.mult_0 = static_cast<uint16_t>(enc.mult_0);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MULTU;
  inst.iclass = InstName::MULTU;
  return true;
}

static bool TryExtractMADD(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000001111111111111111
  //   --------------------------------
  //   01110000000000000000000000000000
  
  if ((bits & 0xFC00FFFFU) != 0x70000000U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;
      uint32_t mt_0:5; 
      uint32_t rt:5; 
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.mt_0 = static_cast<uint8_t>(enc.mt_0);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MADD;
  inst.iclass = InstName::MADD;
  return true;
}

static bool TryExtractMOV_fmt(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000111110000000000111111
  //   --------------------------------
  //   01000100000000000000000000000110
  if ((bits & 0xFC1F003FU) != 0x44000006U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t fd:5; 
      uint32_t fs:5; 
      uint32_t _0:5;
      uint32_t fmt:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rd = static_cast<uint8_t>(enc.fd);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MOV_fmt;
  inst.iclass = InstName::MOV_fmt;
  return true;
}

static bool TryExtractMOVZ(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000001010
  if ((bits & 0xFC0007FFU) != 0x0000000AU) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;
      uint32_t rd:5; 
      uint32_t rt:5;
      uint32_t rs:5; 
      uint32_t opcode:6;  

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MOVZ;
  inst.iclass = InstName::MOVZ;
  return true;
}

static bool TryExtractMOVN(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000001011
  if ((bits & 0xFC0007FFU) != 0x0000000BU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;
      uint32_t rd:5; 
      uint32_t rt:5;
      uint32_t rs:5; 
      uint32_t opcode:6;  

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MOVN;
  inst.iclass = InstName::MOVN;
  return true;
}

static bool TryExtractMOVF(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000110000011111111111
  //   --------------------------------
  //   00000000000000000000000000000001
  if ((bits & 0xFC0307FFU) != 0x00000001U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;
      uint32_t rd:5;
      uint32_t tf:1;
      uint32_t _1:1;
      uint32_t cc:3;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int16_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst._1 = static_cast<uint8_t>(enc._1);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MOVF;
  inst.iclass = InstName::MOVF;
  return true;
}

static bool TryExtractMOVT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000110000011111111111
  //   --------------------------------
  //   00000000000000010000000000000001
  if ((bits & 0xFC0307FFU) != 0x00010001U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;
      uint32_t rd:5;
      uint32_t tf:1;
      uint32_t _1:1;
      uint32_t cc:3;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int16_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.tf = static_cast<uint8_t>(enc.tf);
  inst._1 = static_cast<uint8_t>(enc._1);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MOVT;
  inst.iclass = InstName::MOVT;
  return true;
}

static bool TryExtractMFHI(InstData &inst, uint32_t bits){
  //   bits
  // & 11111111111111110000011111111111
  //   --------------------------------
  //   00000000000000000000000000010000
  if ((bits & 0xFFFF07FFU) != 0x00000010U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5; 
      uint32_t rd:5; 
      uint32_t _1:10;
      uint32_t opcode:6;
       
       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst._1 = static_cast<uint16_t>(enc._1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFHI;
  inst.iclass = InstName::MFHI;
  return true;
}

static bool TryExtractMFLO(InstData &inst, uint32_t bits){
  //   bits
  // & 11111111111111110000011111111111
  //   --------------------------------
  //   00000000000000000000000000010010
  if ((bits & 0xFFFF07FFU) != 0x00000012U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5; 
      uint32_t rd:5; 
      uint32_t _1:10;
      uint32_t opcode:6;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst._1 = static_cast<uint16_t>(enc._1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFLO;
  inst.iclass = InstName::MFLO;
  return true;
}

static bool TryExtractMTLO(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000111111111111111111111
  //   --------------------------------
  //   00000000000000000000000000010011
  if ((bits & 0xFC1FFFFFU) != 0x00000013U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t mtlo_0:15; 
      uint32_t rs:5; 
      uint32_t opcode:6;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.mtlo_0 = static_cast<uint16_t>(enc.mtlo_0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MTLO;
  inst.iclass = InstName::MTLO;
  return true;
}

static bool TryExtractCACHE_OPERATION(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   10111100000000000000000000000000
  if ((bits & 0xFC000000U) != 0xbc000000U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t op:5; 
      uint32_t base:5; 
      uint32_t opcode:6;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst.op = static_cast<uint8_t>(enc.op);
  inst.base = static_cast<uint8_t>(enc.base);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CACHE_OPERATION;
  inst.iclass = InstName::CACHE;
  return true;
}

static bool TryExtractTEQ_32(InstData &inst, uint32_t bits) {
//   bits
  // & 11111100000000000000000000111111
  //   --------------------------------
  //   00000000000000000000000000110100
  if ((bits & 0xFC00003FU) != 0x00000034U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t code :10;
      uint32_t rt:5;
      uint32_t rs:5;   
      uint32_t opcode:6;
        
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.code = static_cast<uint16_t>(enc.code);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.iform = InstForm::TEQ_32;
  inst.iclass = InstName::TEQ;
  return true;
  
}

// SUBU rd , rs ,rt
static bool TryExtractSUBU_32_R(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000100011
  if ((bits & 0xfc0007ffU) != 0x00000023U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t shift:5;
      uint32_t rd:5;  
      uint32_t rt:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.shift = static_cast<uint8_t>(enc.shift);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SUBU_32_R;
  inst.iclass = InstName::SUBU;
  return true;
}

// JR  rs 
static bool TryExtractJR_32_R(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000111111111100000111111
  //   --------------------------------
  //   00000000000000000000000000001000
  if ((bits & 0xfc1ff83fU) != 0x00000008U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t hint:5;
      uint32_t jr_0:10;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.hint = static_cast<uint8_t>(enc.hint);
  inst.jr_0 = static_cast<uint16_t>(enc.jr_0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::JR_32_R;
  inst.iclass = InstName::JR;
  return true;
}

// JR.HB  rs 
static bool TryExtractJR_HB(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000111111111110000111111
  //   --------------------------------
  //   00000000000000000000010000001000                      // Need to check
  if ((bits & 0xfc1ffc3fU) != 0x408U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t hint:4;
      uint32_t jr_1:1;
      uint32_t jr_0:10;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.hint = static_cast<uint8_t>(enc.hint);
  inst.jr_1 = static_cast<uint8_t>(enc.jr_1);
  inst.jr_0 = static_cast<uint16_t>(enc.jr_0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::JR_HB;
  inst.iclass = InstName::JR_HB;
  return true;
}

// SRL  rd , rt , sa 
static bool TryExtractSRL_RIGHT_SHIFT_LOGICAL(InstData &inst, uint32_t bits){
  //   bits
  // & 11111111111000000000000000111111
  //   --------------------------------
  //   00000000000000000000000000000010
  if ((bits & 0xffe0003fU) != 0x2U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _sa:5;
      uint32_t rd:5;  
      uint32_t rt:5;
      uint32_t _R:1;
      uint32_t _0:4;    
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._sa = static_cast<uint8_t>(enc._sa);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst._R = static_cast<uint8_t>(enc._R);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SRL_RIGHT_SHIFT_LOGICAL;
  inst.iclass = InstName::SRL;
  return true;
}

// SLL  rd , rt , sa 
static bool TryExtractSLL_LEFT_SHIFT_LOGICAL(InstData &inst, uint32_t bits){
  //   bits
  // & 11111111111000000000000000111111
  //   --------------------------------
  //   00000000000000000000000000000000
  if ((bits & 0xffe0003fU) != 0x0U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _sa:5;
      uint32_t rd:5;  
      uint32_t rt:5;
      uint32_t _0:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._sa = static_cast<uint8_t>(enc._sa);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SLL_LEFT_SHIFT_LOGICAL;
  inst.iclass = InstName::SLL;
  return true;
}

// SRA  rd , rt , sa 
static bool TryExtractSRA_RIGHT_SHIFT(InstData &inst, uint32_t bits){
  //   bits
  // & 11111111111000000000000000111111
  //   --------------------------------
  //   00000000000000000000000000000011
  if ((bits & 0xffe0003fU) != 0x3U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _sa:5;
      uint32_t rd:5;  
      uint32_t rt:5;
      uint32_t _0:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._sa = static_cast<uint8_t>(enc._sa);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SRA_RIGHT_SHIFT;
  inst.iclass = InstName::SRA;
  return true;
}

// SRAV  rd , rt , sa 
static bool TryExtractSRAV_RIGHT_ARITHMETIC_VARIABLE(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000000111
  if ((bits & 0xFC0007FFU) != 0x00000007U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;
      uint32_t rd:5;  
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SRAV_RIGHT_ARITHMETIC_VARIABLE;
  inst.iclass = InstName::SRAV;
  return true;
}

// SLLV  rd , rt , rs 
static bool TryExtractSLLV_LEFT_LOGICAL_VARIABLE(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000000100
  if ((bits & 0xFC0007FFU) != 0x00000004U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;
      uint32_t rd:5;  
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SLLV_LEFT_LOGICAL_VARIABLE;
  inst.iclass = InstName::SLLV;
  return true;
}

// SRLV  rd , rt , rs 
static bool TryExtractSRLV_RIGHT_LOGICAL_VARIABLE(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   00000000000000000000000000000110
  if ((bits & 0xFC0007FFU) != 0x00000006U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _R:1;
      uint32_t _0:4;
      uint32_t rd:5;  
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._R = static_cast<uint8_t>(enc._R);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SRLV_RIGHT_LOGICAL_VARIABLE;
  inst.iclass = InstName::SRLV;
  return true;
}

// J target
static bool TryExtractJ_Target(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00001000000000000000000000000000
  if ((bits & 0xfc000000U) != 0x8000000U) {
    return false;
  }  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t instr_index :26;
      uint32_t opcode:6; 
      
   } __attribute__((packed));
  } __attribute__((packed)) enc;

  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.instr_index = (enc.instr_index);
  inst.opcode = static_cast<uint32_t>(enc.opcode);
  inst.iform = InstForm::J_Target;                        
  inst.iclass = InstName::J;
  return true;
}

// JALX target
static bool TryExtractJALX_JUMP_LINK(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   01110100000000000000000000000000
  if ((bits & 0xfc000000U) != 0x74000000U) {
    return false;
  }  
  union {
    uint32_t flat;                                                      
    struct {
      uint32_t instr_index :26;
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.instr_index = static_cast<uint32_t>(enc.instr_index);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::JALX_JUMP_LINK;                        
  inst.iclass = InstName::JALX;
  return true;
}

// JAL target
static bool TryExtractJAL_Target(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00001100000000000000000000000000
  if ((bits & 0xfc000000U) != 0xc000000U) {
    return false;
  }  
  union {
    uint32_t flat;                                                       
    struct {
      uint32_t instr_index :26;
      uint32_t opcode:6;       
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.instr_index = static_cast<uint32_t>(enc.instr_index);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::JAL_Target;                        
  inst.iclass = InstName::JAL;
  return true;
}

// JALR rd , rs 
static bool TryExtractJALR_32_R(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000111110000000000111111
  //   --------------------------------
  //   00000000000000000000000000001001
  if ((bits & 0xfc1f003fU) != 0x9U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t hint:5;
      uint32_t rd:5;  
      uint32_t _0:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.hint = static_cast<uint8_t>(enc.hint);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::JALR_32_R;
  inst.iclass = InstName::JALR;
  return true;
}

// JALR.HB rd , rs 
static bool TryExtractJALR_HB(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000111110000010000111111
  //   --------------------------------
  //   00000000000000000000010000001001            // Need to check
  if ((bits & 0xfc1f043fU) != 0x409U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t hint:4;
      uint32_t jalr_1:1;  
      uint32_t rd:5;  
      uint32_t _0:5;  
      uint32_t rs:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.hint = static_cast<uint8_t>(enc.hint);
  inst.jalr_1 = static_cast<uint8_t>(enc.jalr_1);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::JALR_HB;
  inst.iclass = InstName::JALR_HB;
  return true;
}

// NOP
static bool TryExtractNOP(InstData &inst, uint32_t bits){
//   bits
  // & 11111111111111111111111111111111 (mask is calculated on basis of values defined in the instruction def)
  //   --------------------------------
  //   00000000000000000000000000000000  (instruction bits )
  if ((bits & 0xffffffffU) != 0x00000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t SLL:6;
      uint32_t _0:5;
      uint32_t __0:5;
      uint32_t ___0:5;
      uint32_t ____0:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.SLL = static_cast<uint8_t>(enc.SLL);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.__0 = static_cast<uint8_t>(enc.__0);
  inst.___0= static_cast<uint8_t>(enc.___0);
  inst.____0 = static_cast<uint8_t>(enc.____0);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::NOP;
  inst.iclass = InstName::NOP;
  return true;
}

// SSNOP
static bool TryExtractSSNOP_NO_OPERATION(InstData &inst, uint32_t bits){
//   bits
  // & 11111111111111111111111111111111 
  //   --------------------------------
  //   00000000000000000000000001000000  
  if ((bits & 0xffffffffU) != 0x40U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t ssnop_1:5;
      uint32_t _01:5;
      uint32_t _02:5;
      uint32_t _03:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.ssnop_1 = static_cast<uint8_t>(enc.ssnop_1);
  inst._01 = static_cast<uint8_t>(enc._01);
  inst._02 = static_cast<uint8_t>(enc._02);
  inst._03 = static_cast<uint8_t>(enc._03);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SSNOP_NO_OPERATION;
  inst.iclass = InstName::SSNOP;
  return true;
}

static bool TryExtractADDI_32_IMM(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000000000
  //   --------------------------------
  //   00100000000000000000000000000000
  
  if ((bits & 0xFC000000U) != 0x20000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t SIGNED_IMM:16; //typed to 16
      uint32_t rt:5;
      uint32_t rs:5;  
      uint32_t opcode:6;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.SIGNED_IMM = static_cast<int16_t>(enc.SIGNED_IMM);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADDI_32_IMM;
  inst.iclass = InstName::ADDI;
  return true; 
}

// ADDIU rt, rs, immediate
static bool TryExtractADDIU_32_IMM(InstData &inst, uint32_t bits) {
  //   bits
  // & 1111110000000000000000000000000
  //   --------------------------------
  //   0010010000000000000000000000000
  if ((bits & 0xfc000000U) != 0x24000000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t SIGNED_IMM:16; 
      uint32_t rt:5;
      uint32_t rs:5;  
      uint32_t opcode:6;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.SIGNED_IMM = static_cast<int16_t>(enc.SIGNED_IMM);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADDIU_32_IMM;
  inst.iclass = InstName::ADDIU;
  return true; 
}

static bool TryExtractADDU_16e_REG(InstData &inst, uint32_t bits) {
  //   bits
  // & 1111100000000011
  //   --------------------------------
  //   1110000000000001
  if ((bits & 0xf803U) != 0xe001U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t  _0:2; 
      uint32_t rz:3;
      uint32_t ry:3;
      uint32_t rx:3;  
      uint32_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rz = static_cast<uint8_t>(enc.rz);
  inst.ry = static_cast<uint8_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADDU_16e_REG;
  inst.iclass = InstName::ADDU_16e;
  return true; 
}

static bool TryExtractADDIU_16e_IMM(InstData &inst, uint32_t bits) {
  //   bits
  // & 1111100000000000
  //   --------------------------------
  //   0100100000000000
  if ((bits & 0xf800U) != 0x4800U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t SIGNED_16e_IMM:8;
      uint32_t rx:3;  
      uint32_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.SIGNED_16e_IMM = static_cast<int8_t>(enc.SIGNED_16e_IMM);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADDIU_16e_IMM;
  inst.iclass = InstName::ADDIU_16e;
  return true; 
}

static bool TryExtractADDIU_16e_IMM_SPRelative(InstData &inst, uint32_t bits) {
  //   bits
  // & 1111100000000000
  //   --------------------------------
  //   0000000000000000

  if ((bits & 0xf800U) != 0x0000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t SIGNED_16e_IMM:8;
      uint32_t rx:3;  
      uint32_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.SIGNED_16e_IMM = static_cast<int8_t>(enc.SIGNED_16e_IMM);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADDIU_16e_IMM_SPRelative;
  inst.iclass = InstName::ADDIU_16e_IMM_SP;
  return true; 
}

static bool TryExtractADDIU_16e_IMM_Word(InstData &inst, uint32_t bits) {

  if(true && TryExtractINS(inst, bits)) return true;

  //   bits
  // & 1111100000010000
  //   --------------------------------
  //   0100000000000000
  if ((bits & 0xf810U) != 0x4000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t SIGNED_16e_IMM:4;
      uint32_t _0:1;
      uint32_t ry:3;
      uint32_t rx:3; 
      uint32_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.SIGNED_16e_IMM = static_cast<int8_t>(enc.SIGNED_16e_IMM);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.ry = static_cast<uint8_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ADDIU_16e_IMM_Word;
  inst.iclass = InstName::ADDIU_16e_Word;
  return true; 
}

static bool TryExtractB_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111100000000000
  //   --------------------------------
  //   0001000000000000
  if ((bits & 0xF800U) != 0x1000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t offset:11;
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::B_16e;
  inst.iclass = InstName::B_16e;
  return true; 
}

static bool TryExtractB_16e_ext(InstData &inst, uint32_t bits) {

  //   bits
  // & 11111000000000001111111111100000
  //   --------------------------------
  //   11110000000000000001000000000000
  if ((bits & 0xF800FFE0U) != 0xF0001000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t offset:5;
      uint8_t _0:6;
      uint8_t _B:5;
      uint8_t offset_2:5;
      uint8_t offset_1:6; 
      uint32_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst._B = static_cast<uint8_t>(enc._B);
  inst.offset = static_cast<uint8_t>(enc.offset);
  inst.offset_2 = static_cast<uint8_t>(enc.offset_2);
  inst.offset_1 = static_cast<uint8_t>(enc.offset_1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::B_16e_ext;
  inst.iclass = InstName::B_16e_ext;
  return true; 
}

static bool TryExtractBEQZ_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111100000000000
  //   --------------------------------
  //   0010000000000000
  if ((bits & 0xF800U) != 0x2000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t offset:8;
      uint8_t rx:3;
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst.rx = static_cast<int8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BEQZ_16e;
  inst.iclass = InstName::BEQZ_16e;
  return true; 
}

static bool TryExtractBEQZ_16e_ext(InstData &inst, uint32_t bits) {

  //   bits
  // & 11111000000000001111100011100000
  //   --------------------------------
  //   11110000000000000010000000000000
  if ((bits & 0xF800F8E0U) != 0xF0002000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t offset:5;
      uint8_t _0:3;
      uint8_t rx:3;
      uint8_t funct:5;
      uint8_t offset_2:5;
      uint8_t offset_1:6; 
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.offset_2 = static_cast<uint8_t>(enc.offset_2);
  inst.offset_1 = static_cast<uint8_t>(enc.offset_1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BEQZ_16e_ext;
  inst.iclass = InstName::BEQZ_16e_ext;
  return true; 
}


static bool TryExtractBNEZ_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111100000000000
  //   --------------------------------
  //   0010100000000000
  if ((bits & 0xF800U) != 0x2800U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t offset:8;
      uint8_t rx:3;
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst.rx = static_cast<int8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BNEZ_16e;
  inst.iclass = InstName::BNEZ_16e;
  return true; 
}

static bool TryExtractBNEZ_16e_ext(InstData &inst, uint32_t bits) {

  //   bits
  // & 11111000000000001111100011100000
  //   --------------------------------
  //   11110000000000000010100000000000
  if ((bits & 0xF800F8E0U) != 0xF0002800U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t offset:5;
      uint8_t _0:3;
      uint8_t rx:3;
      uint8_t funct:5;
      uint8_t offset_2:5;
      uint8_t offset_1:6; 
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.offset_2 = static_cast<uint8_t>(enc.offset_2);
  inst.offset_1 = static_cast<uint8_t>(enc.offset_1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BNEZ_16e_ext;
  inst.iclass = InstName::BNEZ_16e_ext;
  return true; 
}

//----------------------


static bool TryExtractBREAK_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000000101
  if ((bits & 0xF81FU) != 0xE805U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t funct:5;
      uint8_t code:6;
      uint8_t opcode:5;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.code = static_cast<uint8_t>(enc.code);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BREAK_16e;
  inst.iclass = InstName::BREAK_16e;
  return true; 
}

static bool TryExtractBTEQZ_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111111100000000
  //   --------------------------------
  //   0110000000000000
  if ((bits & 0xFF00U) != 0x6000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t offset:8;
      uint8_t funct:3;
      uint8_t I8:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.I8 = static_cast<uint8_t>(enc.I8);
  inst.iform = InstForm::BTEQZ_16e;
  inst.iclass = InstName::BTEQZ_16e;
  return true; 
}

static bool TryExtractBTEQZ_16e_ext(InstData &inst, uint32_t bits) {

  //   bits
  // & 11111000000000001111111111100000
  //   --------------------------------
  //   11110000000000000110000000000000
  if ((bits & 0xF800FFE0U) != 0xF0006000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t offset:5;
      uint8_t _0:3;
      uint8_t funct:3;
      uint8_t I8:5;
      uint8_t offset_2:5;
      uint8_t offset_1:6; 
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.I8 = static_cast<uint8_t>(enc.I8);
  inst.offset_2 = static_cast<uint8_t>(enc.offset_2);
  inst.offset_1 = static_cast<uint8_t>(enc.offset_1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BTEQZ_16e_ext;
  inst.iclass = InstName::BTEQZ_16e_ext;
  return true; 
}


static bool TryExtractBTNEZ_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111111100000000
  //   --------------------------------
  //   0110000100000000
   if ((bits & 0xFF00U) != 0x6100U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t offset:8;
      uint8_t funct:3;
      uint8_t I8:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.I8 = static_cast<uint8_t>(enc.I8);
  inst.iform = InstForm::BTNEZ_16e;
  inst.iclass = InstName::BTNEZ_16e;
  return true; 
}

static bool TryExtractBTNEZ_16e_ext(InstData &inst, uint32_t bits) {

  //   bits
  // & 11111000000000001111111111100000
  //   --------------------------------
  //   11110000000000000110000100000000
  if ((bits & 0xF800FFE0U) != 0xF0006100U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t offset:5;
      uint8_t _0:3;
      uint8_t funct:3;
      uint8_t I8:5;
      uint8_t offset_2:5;
      uint8_t offset_1:6; 
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.I8 = static_cast<uint8_t>(enc.I8);
  inst.offset_2 = static_cast<uint8_t>(enc.offset_2);
  inst.offset_1 = static_cast<uint8_t>(enc.offset_1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BTNEZ_16e_ext;
  inst.iclass = InstName::BTNEZ_16e_ext;
  return true; 
}


static bool TryExtractCMP_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111111100011111
  //   --------------------------------
  //   1110100000001010
   if ((bits & 0xFF1FU) != 0xE80AU) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t funct:5;
      uint8_t ry:3;
      uint8_t rx:3;
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.ry = static_cast<int8_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CMP_16e;
  inst.iclass = InstName::CMP_16e;
  return true; 
}


static bool TryExtractCMPI_16e_imm(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111100000000000
  //   --------------------------------
  //   0111000000000000
   if ((bits & 0xF800U) != 0x7000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t offset:8;
      uint8_t rx:3;
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst.rx = static_cast<int8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CMPI_16e_imm;
  inst.iclass = InstName::CMPI_16e_imm;
  return true; 
}

static bool TryExtractCMPI_16e_imm_ext(InstData &inst, uint32_t bits) {

  //   bits
  // & 11111000000000001111100011100000
  //   --------------------------------
  //   11110000000000000111000000000000
  if ((bits & 0xF800F8E0U) != 0xF0007000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint8_t offset:5;
      uint8_t _0:3;
      uint8_t rx:3;
      uint8_t funct:5;
      uint8_t offset_2:5;
      uint8_t offset_1:6; 
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int8_t>(enc.offset);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.offset_2 = static_cast<uint8_t>(enc.offset_2);
  inst.offset_1 = static_cast<uint8_t>(enc.offset_1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CMPI_16e_imm_ext;
  inst.iclass = InstName::CMPI_16e_imm_ext;
  return true; 
}
//---------------------
static bool TryExtractDIV_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000011010
   if ((bits & 0xF81FU) != 0xE81AU) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t funct:5;
      uint8_t rx:3;
      uint8_t ry:3;
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.rx = static_cast<int8_t>(enc.rx);
  inst.ry = static_cast<int8_t>(enc.ry);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::DIV_16e;
  inst.iclass = InstName::DIV_16e;
  return true; 
}
static bool TryExtractDIVU_16e(InstData &inst, uint32_t bits) {

  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000011011
   if ((bits & 0xF81FU) != 0xE81BU) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint16_t funct:5;
      uint8_t rx:3;
      uint8_t ry:3;
      uint8_t opcode:5;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<int8_t>(enc.funct);
  inst.rx = static_cast<int8_t>(enc.rx);
  inst.ry = static_cast<int8_t>(enc.ry);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::DIVU_16e;
  inst.iclass = InstName::DIVU_16e;
  return true; 
}


static bool TryExtractMFHI_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100011111111
  //   --------------------------------
  //   1110100000010000
  if ((bits & 0xF8FFU) != 0xE810U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t _0:3; 
      uint32_t rx:5; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFHI_16e;
  inst.iclass = InstName::MFHI_16e;
  return true;
}

static bool TryExtractMFLO_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100011111111
  //   --------------------------------
  //   1110100000010010
  if ((bits & 0xF8FFU) != 0xE812U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t _0:3; 
      uint32_t rx:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint16_t>(enc._0);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFLO_16e;
  inst.iclass = InstName::MFLO_16e;
  return true;
}

//--------------

static bool TryExtractMOVE_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111111100000000
  //   --------------------------------
  //   0110010100000000
  if ((bits & 0xFF00U) != 0x6500U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t rz:3;
      uint32_t ry:2; 
      uint32_t rx:3; 
      uint32_t funct:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.rz = static_cast<uint8_t>(enc.rz);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MOVE_16e;
  inst.iclass = InstName::MOVE_16e;
  return true;
}

static bool TryExtractMOVE_16e_r32(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100011111111
  //   --------------------------------
  //   1110100000010010
  if ((bits & 0xF8FFU) != 0xE812U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t rx:3; 
      uint32_t ry:5;
      uint32_t MOVR32:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.MOVR32 = static_cast<uint8_t>(enc.MOVR32);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MOVE_16e_r32;
  inst.iclass = InstName::MOVE_16e_r32;
  return true;
}

static bool TryExtractMULT_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000011000
  if ((bits & 0xF81FU) != 0xE818U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t ry:3; 
      uint32_t rx:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MULT_16e;
  inst.iclass = InstName::MULT_16e;
  return true;
}

static bool TryExtractMULTU_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000011001
  if ((bits & 0xF81FU) != 0xE819U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t ry:3; 
      uint32_t rx:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MULTU_16e;
  inst.iclass = InstName::MULTU_16e;
  return true;
}

static bool TryExtractNEG_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000001011
  if ((bits & 0xF81FU) != 0xE80BU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t ry:3; 
      uint32_t rx:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::NEG_16e;
  inst.iclass = InstName::NEG_16e;
  return true;
}

static bool TryExtractNOT_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000001111
  if ((bits & 0xF81FU) != 0xE80FU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t ry:2; 
      uint32_t rx:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::NOT_16e;
  inst.iclass = InstName::NOT_16e;
  return true;
}

static bool TryExtractOR_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000001101
  if ((bits & 0xF81FU) != 0xE80DU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t ry:2; 
      uint32_t rx:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::OR_16e;
  inst.iclass = InstName::OR_16e;
  return true;
}

static bool TryExtractRESTORE_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111111110000000
  //   --------------------------------
  //   0110010000000000
  if ((bits & 0xFF80U) != 0x6400DU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t framesize:4;
      uint32_t _s1:1;
      uint32_t _s0:1;
      uint32_t _ra:1;
      uint32_t _s:1; 
      uint32_t funct:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.framesize = static_cast<uint8_t>(enc.framesize);
  inst._s1 = static_cast<uint16_t>(enc._s1);
  inst._s0 = static_cast<uint16_t>(enc._s0);
  inst._ra = static_cast<uint16_t>(enc._ra);
  inst._s = static_cast<uint16_t>(enc._s);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::RESTORE_16e;
  inst.iclass = InstName::RESTORE_16e;
  return true;
}

static bool TryExtractRESTORE_16e_ext(InstData &inst, uint32_t bits){
  //   bits
  // & 11111000000000001111111110000000
  //   --------------------------------
  //   11110000000000000110010000000000
  if ((bits & 0xF800FF80U) != 0xF0006400U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t framesize:4;
      uint32_t _s1:1;
      uint32_t _s0:1;
      uint32_t _ra:1;
      uint32_t _s:1; 
      uint32_t funct:3; 
      uint32_t I8:5;
      uint32_t aregs:4;
      uint32_t framesize_2:4;
      uint32_t xsregs:3;
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.framesize = static_cast<uint8_t>(enc.framesize);
  inst._s1 = static_cast<uint16_t>(enc._s1);
  inst._s0 = static_cast<uint16_t>(enc._s0);
  inst._ra = static_cast<uint16_t>(enc._ra);
  inst._s = static_cast<uint16_t>(enc._s);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.I8 = static_cast<uint8_t>(enc.I8);
  inst.aregs = static_cast<uint8_t>(enc.aregs);
  inst.framesize_2 = static_cast<uint8_t>(enc.framesize_2);
  inst.xsregs = static_cast<uint8_t>(enc.xsregs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::RESTORE_16e_ext;
  inst.iclass = InstName::RESTORE_16e_ext;
  return true;
}


static bool TryExtractSAVE_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111111110000000
  //   --------------------------------
  //   0110010010000000
  if ((bits & 0xFF80U) != 0x6480DU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t framesize:4;
      uint32_t _s1:1;
      uint32_t _s0:1;
      uint32_t _ra:1;
      uint32_t _s:1; 
      uint32_t funct:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.framesize = static_cast<uint8_t>(enc.framesize);
  inst._s1 = static_cast<uint16_t>(enc._s1);
  inst._s0 = static_cast<uint16_t>(enc._s0);
  inst._ra = static_cast<uint16_t>(enc._ra);
  inst._s = static_cast<uint16_t>(enc._s);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SAVE_16e;
  inst.iclass = InstName::SAVE_16e;
  return true;
}

static bool TryExtractSAVE_16e_ext(InstData &inst, uint32_t bits){
  //   bits
  // & 11111000000000001111111110000000
  //   --------------------------------
  //   11110000000000000110010010000000
  if ((bits & 0xF800FF80U) != 0xF0006480U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t framesize:4;
      uint32_t _s1:1;
      uint32_t _s0:1;
      uint32_t _ra:1;
      uint32_t _s:1; 
      uint32_t funct:3; 
      uint32_t I8:5;
      uint32_t aregs:4;
      uint32_t framesize_2:4;
      uint32_t xsregs:3;
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.framesize = static_cast<uint8_t>(enc.framesize);
  inst._s1 = static_cast<uint16_t>(enc._s1);
  inst._s0 = static_cast<uint16_t>(enc._s0);
  inst._ra = static_cast<uint16_t>(enc._ra);
  inst._s = static_cast<uint16_t>(enc._s);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.I8 = static_cast<uint8_t>(enc.I8);
  inst.aregs = static_cast<uint8_t>(enc.aregs);
  inst.framesize_2 = static_cast<uint8_t>(enc.framesize_2);
  inst.xsregs = static_cast<uint8_t>(enc.xsregs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SAVE_16e_ext;
  inst.iclass = InstName::SAVE_16e_ext;
  return true;
}


static bool TryExtractSB_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100000000000
  //   --------------------------------
  //   1100000000000000
  if ((bits & 0xF800U) != 0xC000DU) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t ry:3;
      uint32_t rx:3; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.rx = static_cast<uint16_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SB_16e;
  inst.iclass = InstName::SB_16e;
  return true;
}

static bool TryExtractSB_16e_ext(InstData &inst, uint32_t bits){
  //   bits
  // & 11111000000000001111100000000000
  //   --------------------------------
  //   11110000000000001100000000000000
  if ((bits & 0xF800F800U) != 0xF000C000U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t offset:5;
      uint32_t ry:3;
      uint32_t rx:3;
      uint32_t funct:5;
      uint32_t offset_2:5;
      uint32_t offset_1:6; 
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<uint8_t>(enc.offset);
  inst.ry = static_cast<uint16_t>(enc.ry);
  inst.rx = static_cast<uint16_t>(enc.rx);
  inst.funct = static_cast<uint16_t>(enc.funct);
  inst.offset_2 = static_cast<uint16_t>(enc.offset_2);
  inst.offset_1 = static_cast<uint8_t>(enc.offset_1);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SB_16e_ext;
  inst.iclass = InstName::SB_16e_ext;
  return true;
}

static bool TryExtractSDBBP_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100000011111
  //   --------------------------------
  //   1110100000000001
  if ((bits & 0xF81FU) != 0xE801U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:5;
      uint32_t code:3;
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.code = static_cast<uint16_t>(enc.code);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SDBBP_16e;
  inst.iclass = InstName::SDBBP_16e;
  return true;
}

static bool TryExtractSEB_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 1111100011111111
  //   --------------------------------
  //   1110100010010001
  if ((bits & 0xF81FU) != 0xE801U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t code:5;
      uint32_t funct:3;
      uint32_t rx:3;
      uint32_t opcode:5;

    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.code = static_cast<uint16_t>(enc.code);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::SEB_16e;
  inst.iclass = InstName::SEB_16e;
  return true;
}


static bool TryExtractBGTZ_32_IMM_COND(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000111110000000000000000
  //   --------------------------------
  //   00011100000000000000000000000000
  if ((bits & 0xfc1f0000U) != 0x1c000000U) {
    return false;
    
  }
  union {
    uint32_t flat;
    struct {
      uint32_t offset:16;
      uint32_t _0:5;
      uint32_t rs:5;  
      uint32_t opcode:6;    
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.offset = static_cast<int16_t>(enc.offset);
  inst._0 = static_cast<int16_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::BGTZ_32_IMM_COND;
  inst.iclass = InstName::BGTZ;
  return true; 
}

static bool TryExtractCMP_COND_FMT(InstData &inst, uint32_t bits){
  //   bits
  // & 11111100000000000000000001110000
  //   --------------------------------
  //   01000100000000000000000000110000
  if ((bits & 0xFC000070U) != 0x44000030U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t cond:4;
      uint32_t FC:2;
      uint32_t A:1;
      uint32_t _0:1;
      uint32_t cc:3; 
      uint32_t fs:5;
      uint32_t ft:5;
      uint32_t fmt:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.cond= static_cast<uint8_t>(enc.cond);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.rt = static_cast<uint8_t>(enc.ft);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst.cc = static_cast<uint8_t>(enc.cc);
  inst._0 = enc._0; 
  inst.A = enc.A;
  inst.FC = static_cast<uint8_t>(enc.FC);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::CMP_COND_FMT;
  inst.iclass = InstName::CMP_COND_FMT;
  return true; 
}

static bool TryExtractCVT_D_FMT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000111111
  //   --------------------------------
  //   01000100000000000000000000100001
  if ((bits & 0xFC1F003FU) != 0x44000021U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t opcode:6;
      uint32_t fd:5;
      uint32_t fs:5;
      uint32_t _0:5;
      uint32_t fmt:5;
      uint32_t funct:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.rd = static_cast<uint8_t>(enc.fd);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst._0 = enc._0; 
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.iform = InstForm::CVT_D_FMT;
  inst.iclass = InstName::CVT_D_FMT;
  return true; 
}

static bool TryExtractCVT_S_FMT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000111111
  //   --------------------------------
  //   01000100000000000000000000100000
  if ((bits & 0xFC1F003FU) != 0x44000020U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t fd:5;
      uint32_t fs:5;
      uint32_t _0:5;
      uint32_t fmt:5;
      uint32_t opcode:6;
      
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.rd = static_cast<uint8_t>(enc.fd);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst._0 = enc._0; /* cast required? */
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.iform = InstForm::CVT_S_FMT;
  inst.iclass = InstName::CVT_S_FMT;
  return true; 
}

static bool TryExtractMFC1_FLOATING_POINT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01000100000000000000000000000000
  if ((bits & 0xffe007ffU) != 0x44000000U) {
    return false;
  }  
  union {
    uint32_t flat;
    struct {
      uint32_t _0:11;
      uint32_t fs:5;
      uint32_t rt:5;
      uint32_t mf:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.mf = static_cast<uint8_t>(enc.mf);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFC1_FLOATING_POINT;
  inst.iclass = InstName::MFC1;
  return true; 
}

static bool TryExtractEXT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000111111
  //   --------------------------------
  //   01111100000000000000000000000000
  if ((bits & 0xFC00003FU) != 0x7C000000U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t lsb:5;
      uint32_t msbd:5;
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.lsb = static_cast<uint8_t>(enc.lsb);
  inst.msbd = static_cast<uint8_t>(enc.msbd);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::EXT;
  inst.iclass = InstName::EXT;
  return true;
}

static bool TryExtractMFHC1_FLOATING_POINT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01000100011000000000000000000000
  if ((bits & 0xffe007ffU) != 0x44600000U) {
    return false;
  }  
      
  union {
    uint32_t flat;
    struct {
      uint32_t _0:11;
      uint32_t fs:5;
      uint32_t rt:5;
      uint32_t mfh:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.mfh = static_cast<uint8_t>(enc.mfh);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFHC1_FLOATING_POINT;
  inst.iclass = InstName::MFHC1;
  return true;
}

static bool TryExtractMTHC1_FLOATING_POINT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01000100111000000000000000000000
  if ((bits & 0xffe007ffU) != 0x44e00000U) {
    return false;
  }
  
  union {
    uint32_t flat;
    struct {
      uint32_t mt_0:11;
      uint32_t fs:5;  
      uint32_t rt:5;  
      uint32_t mth:5;
      uint32_t opcode:6;  
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.mt_0 = static_cast<uint8_t>(enc.mt_0);
  inst.fs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.mth = static_cast<uint8_t>(enc.mth);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MTHC1_FLOATING_POINT;
  inst.iclass = InstName::MTHC1;
  return true;
  
}

static bool TryExtractMTC1_FLOATING_POINT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000011111111111
  //   --------------------------------
  //   01000100100000000000000000000000
  if ((bits & 0xffe007ffU) != 0x44800000U) {
    return false;
  }  
  union {
    uint32_t flat;
    struct {
      uint32_t _0:11;
      uint32_t fs:5;
      uint32_t rt:5;
      uint32_t mt:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.mt = static_cast<uint8_t>(enc.mt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MTC1_FLOATING_POINT;
  inst.iclass = InstName::MTC1;
  return true;
}

static bool TryExtractNEG_fmt(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000000000111111
  //   --------------------------------
  //   01000100000000000000000000000111
  if ((bits & 0xFC1F003FU) != 0x44000007U) {
    return false;
  }  
  
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t fd:5;
      uint32_t fs:5;
      uint32_t _0:5;
      uint32_t fmt:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rd = static_cast<uint8_t>(enc.fd);
  inst.rs = static_cast<uint8_t>(enc.fs);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.fmt = static_cast<uint8_t>(enc.fmt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::NEG_fmt;
  inst.iclass = InstName::NEG_fmt;
  return true;
}

static bool TryExtractINS(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000000000111111
  //   --------------------------------
  //   01111100000000000000000000000100
  if ((bits & 0xFC00003FU) != 0x7C000004U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t lsb:5;
      uint32_t msbd:5;
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.lsb = static_cast<uint8_t>(enc.lsb);
  inst.msbd = static_cast<uint8_t>(enc.msbd);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::INS;
  inst.iclass = InstName::INS;
  return true;
}

static bool TryExtractCOP2_OPERATION(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111110000000000000000000000000
  //   --------------------------------
  //   01001010000000000000000000000000
  if ((bits & 0xfe000000U) != 0x4a000000U) {
    return false;
  } 
  
  union {
    uint32_t flat;
    struct {
      uint32_t cofun:25;
      uint32_t co:1;
      uint32_t opcode:6;
    } __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.cofun = static_cast<uint32_t>(enc.cofun);
  inst.co = static_cast<uint8_t>(enc.co);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::COP2_OPERATION;
  inst.iclass = InstName::COP2;
  return true;
}
static bool TryExtractASMACRO_16e(InstData &inst, uint32_t bits){
  //   bits
  // & 11111000000000001111100000000000
  //   --------------------------------
  //   11110000000000001110000000000000
  if ((bits & 0xF800F800U) != 0xF000E000U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t p0:5;
      uint32_t p1:3;
      uint32_t p2:3;
      uint32_t p3:5; 
      uint32_t p4:3;
      uint32_t select:3;
      uint32_t opcode:5;
    }
   __attribute__((packed));
  } __attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.p0= static_cast<uint8_t>(enc.p0); 
  inst.p1 = static_cast<uint8_t>(enc.p1);
  inst.p2 = static_cast<uint8_t>(enc.p2);
  inst.p3= static_cast<uint8_t>(enc.p3);
  inst.p4 = static_cast<uint8_t>(enc.p4);
  inst.select = static_cast<uint8_t>(enc.select);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::ASMACRO_16e;
  inst.iclass = InstName::ASMACRO_16e;
  return true; 
}
static bool TryExtractDMT_MT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000001111111111111111
  //   --------------------------------
  //   01000001011000000000101111000001
  if ((bits & 0xFFE0FFFFU) != 0x41600BC1U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t _0:3;
      uint32_t __0:2;  
      uint32_t sc:1;
      uint32_t ___0:5;
      uint32_t _12:5;
      uint32_t rt:5;
      uint32_t funct:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::DMT_MT;
  inst.iclass = InstName::DMT_MT;
  return true;
}

static bool TryExtractDVPE_MT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000001111111111111111
  //   --------------------------------
  //   01000001011000000000000000000001
  if ((bits & 0xFFE0FFFFU) != 0x41600001U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t _0:3;
      uint32_t __0:2;  
      uint32_t sc:1;
      uint32_t ___0:5;
      uint32_t _12:5;
      uint32_t rt:5;
      uint32_t funct:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::DVPE_MT;
  inst.iclass = InstName::DVPE_MT;
  return true;
}

static bool TryExtractEMT_MT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000001111111111111111
  //   --------------------------------
  //   01000001011000000000101111100001
  if ((bits & 0xFFE0FFFFU) != 0x41600BE1U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t _0:3;
      uint32_t __0:2;  
      uint32_t sc:1;
      uint32_t ___0:5;
      uint32_t _12:5;
      uint32_t rt:5;
      uint32_t funct:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::EMT_MT;
  inst.iclass = InstName::EMT_MT;
  return true;
}

static bool TryExtractEVPE_MT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000001111111111111111
  //   --------------------------------
  //   01000001011000000000000000100001
  if ((bits & 0xFFE0FFFFU) != 0x41600021U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t _0:3;
      uint32_t __0:2;  
      uint32_t sc:1;
      uint32_t ___0:5;
      uint32_t _12:5;
      uint32_t rt:5;
      uint32_t funct:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::EVPE_MT;
  inst.iclass = InstName::EVPE_MT;
  return true;
}

static bool TryExtractFORK_MT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000000000000011111111111
  //   --------------------------------
  //   01111100000000000000000000001000
  if ((bits & 0xFC0007FFU) != 0x7C000008U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;  
      uint32_t rd:5;
      uint32_t rt:5;
      uint32_t rs:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::FORK_MT;
  inst.iclass = InstName::FORK_MT;
  return true;
}

static bool TryExtractMFTR_MT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000001000
  //   --------------------------------
  //   01000001000000000000000000111000
  if ((bits & 0xFFE00008U) != 0x41000038U) {
    return false;
  }
  union {
    uint32_t flat;
    struct {
      uint32_t sel:3;
      uint32_t _0:1;  
      uint32_t h:1;
      uint32_t u:1;
      uint32_t rx:5;
      uint32_t rd:5;
      uint32_t rt:5;
      uint32_t funct:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.sel = static_cast<uint8_t>(enc.sel);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.h = static_cast<uint8_t>(enc.h);
  inst.u = static_cast<uint8_t>(enc.u);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MFTR_MT;
  inst.iclass = InstName::MFTR_MT;
  return true;
}

static bool TryExtractMTTR_MT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111111111000000000000000001000
  //   --------------------------------
  //   01000001100000000000000000001000
  if ((bits & 0xFFE00008U) != 0x41800008U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t sel:3;
      uint32_t _0:1;  
      uint32_t h:1;
      uint32_t u:1;
      uint32_t rx:5;
      uint32_t rd:5;
      uint32_t rt:5;
      uint32_t funct:5;
      uint32_t opcode:6;
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.sel = static_cast<uint8_t>(enc.sel);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.h = static_cast<uint8_t>(enc.h);
  inst.u = static_cast<uint8_t>(enc.u);
  inst.rx = static_cast<uint8_t>(enc.rx);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.rt = static_cast<uint8_t>(enc.rt);
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::MTTR_MT;
  inst.iclass = InstName::MTTR_MT;
  return true;
}

static bool TryExtractYIELD_MT(InstData &inst, uint32_t bits) {
  //   bits
  // & 11111100000111110000011111111111
  //   --------------------------------
  //   01111100000000000000000000001001
  if ((bits & 0xFC1F07FFU) != 0x7C000009U) {
    return false;
  }
    
  union {
    uint32_t flat;
    struct {
      uint32_t funct:6;
      uint32_t _0:5;  
      uint32_t rd:1;
      uint32_t __0:5;
      uint32_t rs:5;
      uint32_t opcode:5;
      
    }__attribute__((packed));
  }__attribute__((packed)) enc;
  static_assert(sizeof(enc) == 4, " ");
  enc.flat = bits;
  inst.funct = static_cast<uint8_t>(enc.funct);
  inst._0 = static_cast<uint8_t>(enc._0);
  inst.rd = static_cast<uint8_t>(enc.rd);
  inst.__0 = static_cast<uint8_t>(enc.__0);
  inst.rs = static_cast<uint8_t>(enc.rs);
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  
  inst.opcode = static_cast<uint8_t>(enc.opcode);
  inst.iform = InstForm::YIELD_MT;
  inst.iclass = InstName::YIELD_MT;
  return true;
}



}  // namespace

const char *InstNameToString(InstName iclass) {
  auto num = static_cast<uint16_t>(iclass);
  if (iclass == InstName::INVALID) {
    return nullptr;
  } 
  else {
    return kIClassName[num];
  }
}

const char *InstFormToString(InstForm iform) {
  auto num = static_cast<uint16_t>(iform);
  if (iform == InstForm::INVALID) {
    return nullptr;
  } 
  else {
    return kIFormName[num];
  }
}


bool TryExtractMips(const uint8_t *bytes, InstData &inst) {

  uint32_t bits = 0;

  bits = (bits << 8) | static_cast<uint32_t>(bytes[3]);
  bits = (bits << 8) | static_cast<uint32_t>(bytes[2]);
  bits = (bits << 8) | static_cast<uint32_t>(bytes[1]);
  bits = (bits << 8) | static_cast<uint32_t>(bytes[0]);

  uint32_t index = 0;

  index |= ((bits >> 28U) & 1U) << 2U;
  index |= ((bits >> 27U) & 1U) << 1U;
  index |= ((bits >> 26U) & 1U) << 0U;


  return false ||
         TryExtractLUI_IMM(inst, bits)||
         TryExtractLW_32_IMM(inst, bits)||
         TryExtractLWC1_FLOATING_POINT(inst, bits) ||
         TryExtractLDC1_FLOATING_R(inst, bits)||
         TryExtractADD_word(inst, bits)||
         TryExtractADDU_32_R(inst, bits)||
         TryExtractADDI_32_IMM( inst,  bits)||
         TryExtractADDIU_32_IMM( inst,  bits)||
         TryExtractADD_FMT(inst,  bits) ||
         TryExtractDIV_32(inst,  bits) ||
         TryExtractDIV_fmt(inst,  bits) ||
         TryExtractDIVU(inst,  bits) ||
         TryExtractMUL_32(inst,  bits) ||
         TryExtractMUL_fmt(inst,  bits) ||
         TryExtractMULT(inst,  bits) ||
         TryExtractMULTU(inst,  bits) ||
         TryExtractMADD(inst,  bits) ||
         TryExtractMOV_fmt(inst,  bits) ||
         TryExtractMOVZ(inst,  bits) ||
         TryExtractMOVN(inst,  bits) ||
         TryExtractMOVF(inst,  bits) ||
         TryExtractMOVT(inst,  bits) ||
         TryExtractMFHI( inst, bits ) ||
         TryExtractMFLO( inst, bits ) ||
         TryExtractMTLO( inst, bits) ||
         TryExtractTEQ_32( inst, bits ) ||
         TryExtractBGTZ_32_IMM_COND(inst,  bits)||
         TryExtractCMP_COND_FMT(inst, bits) ||
         TryExtractCVT_D_FMT(inst, bits) ||
         TryExtractCVT_S_FMT(inst, bits) ||
         TryExtractNEG_fmt(inst, bits)||
         TryExtractSUBU_32_R(inst, bits)||
         TryExtractSW_32_IMM(inst, bits)||
         TryExtractSWL(inst, bits)||
         TryExtractSWC1_FLOATING_POINT(inst, bits) ||
         TryExtractSDC1_FLOATING_R(inst, bits)||   
         TryExtractSH_16_MEM(inst, bits)||
         TryExtractSEH(inst, bits)||
         TryExtractLH(inst, bits) ||
         TryExtractLBU_32_R(inst, bits)||
         TryExtractLB_32_R(inst, bits)||
         TryExtractLHU_32_R(inst, bits)||
         TryExtractNOP(inst, bits)||
         TryExtractSLTIU_IMM(inst, bits)||
         TryExtractSLTI_IMM(inst , bits)||
         TryExtractSLT_32_R(inst , bits)||
         TryExtractSLTU_32_R(inst , bits)||
         TryExtractSUB_FMT(inst, bits) ||
         TryExtractOR_32_R(inst , bits)||
         TryExtractORI_32_IMM(inst , bits)||
         TryExtractAND_32_R(inst , bits)||
         TryExtractANDI_32_IMM(inst , bits)||
         TryExtractNOR_32_R(inst , bits)||
         TryExtractXOR_32_R(inst , bits)||
         TryExtractXORI_32_IMM(inst , bits)||
         TryExtractJR_32_R(inst , bits)||
         TryExtractJ_Target(inst , bits)||
         TryExtractJALR_32_R(inst , bits)||
         TryExtractJAL_Target(inst , bits)||
         TryExtractJALX_JUMP_LINK(inst , bits)||
         TryExtractSRA_RIGHT_SHIFT(inst , bits)||
         TryExtractSRAV_RIGHT_ARITHMETIC_VARIABLE(inst , bits)||
         TryExtractSRL_RIGHT_SHIFT_LOGICAL(inst , bits)||
         TryExtractSLL_LEFT_SHIFT_LOGICAL(inst , bits)||
         TryExtractSLLV_LEFT_LOGICAL_VARIABLE(inst , bits)||
         TryExtractSRLV_RIGHT_LOGICAL_VARIABLE(inst , bits)||
         TryExtractBEQ_BRANCH_EQUAL(inst , bits)||
         TryExtractBNE_BRANCH_NOT_EQUAL(inst , bits)||
         TryExtractB_32_offset(inst , bits)||
         TryExtractBGEZAL_LINK(inst , bits)||
         TryExtractBC1F_FP_FALSE(inst, bits) ||
         TryExtractBC1T_FP_TRUE(inst, bits) ||
         TryExtractSB_IMM(inst , bits)||
         TryExtractSEB_EXTEND_BYTE(inst , bits)||
         TryExtractBAL_BRANCH_LINK(inst , bits)||
         TryExtractBGEZ(inst , bits)||
         TryExtractBLEZ(inst , bits)||
         TryExtractBLTZ(inst , bits)||
         TryExtractBEQL_BRANCH_LIKELY(inst , bits)||
         TryExtractBLTZAL(inst , bits)||
         TryExtractBGTZL_BRANCH_LIKELY(inst , bits)||
         TryExtractBGEZL_BRANCH_LIKELY(inst , bits)||
         TryExtractBLTZL_BRANCH_LIKELY(inst , bits)||
         TryExtractBNEL_BRANCH_LIKELY(inst , bits)||
         TryExtractLL_LOAD_LINKED_WORD(inst , bits)||
         TryExtractROTRV(inst , bits)||
         TryExtractROTR(inst , bits)||
         TryExtractCFC1_FLOATING_POINT(inst , bits)||
         TryExtractCTC1(inst , bits)||
         TryExtractSC_STORE_CONDITIONAL_WORD(inst , bits)||
         TryExtractSUB_WORD(inst , bits)||
         TryExtractJALR_HB(inst , bits)||
         TryExtractJR_HB(inst , bits)||
         TryExtractSSNOP_NO_OPERATION(inst , bits)||
         TryExtractLWL_LOAD_WORD_LEFT(inst , bits)||
         TryExtractLWR_LOAD_WORD_RIGHT(inst , bits)||
         TryExtractPREF(inst , bits)||
         TryExtractSYNCI(inst , bits)||
         TryExtractBC1FL_FP_FALSE_LIKELY(inst , bits)||
         TryExtractBC1TL_FP_TRUE_LIKELY(inst , bits)||
         TryExtractROUND_L_fmt(inst , bits)||
         TryExtractROUND_W_fmt(inst , bits)||
         TryExtractCEIL_L_fmt(inst , bits)||
         TryExtractCEIL_W_fmt(inst , bits)||
         TryExtractBC2F_FP_FALSE(inst , bits)||
         TryExtractBC2T_FP_TRUE(inst , bits)||
         TryExtractLDC2(inst , bits)||
         TryExtractLWC2(inst , bits)||
         TryExtractSDC2(inst , bits)||
         TryExtractSWC2(inst , bits)||
         TryExtractCFC2(inst , bits)||
         TryExtractCTC2(inst , bits)||
         TryExtractMFC2(inst , bits)||
         TryExtractMFHC2(inst , bits)||
         TryExtractMTC2(inst , bits)||
         TryExtractMTHC2(inst , bits)||
         TryExtractBC2FL_FALSE_LIKELY(inst , bits)||
         TryExtractBC2TL_TRUE_LIKELY(inst , bits)||
         TryExtractCACHE_OPERATION(inst , bits)||
         TryExtractCOP2_OPERATION(inst , bits)||
         TryExtractEXT(inst, bits) ||
         TryExtractADDIU_16e_IMM(inst , bits)||
         TryExtractADDIU_16e_IMM_Word(inst , bits)||
         TryExtractADDIU_16e_IMM_SPRelative(inst , bits)||
         TryExtractADDU_16e_REG(inst , bits)||
         TryExtractAND_16e(inst , bits)||
         TryExtractB_16e(inst , bits)||
         TryExtractB_16e_ext(inst , bits)||
         TryExtractBEQZ_16e(inst , bits)||
         TryExtractBEQZ_16e_ext(inst , bits)||
         TryExtractBNEZ_16e(inst , bits)||
         TryExtractBNEZ_16e_ext(inst , bits)||
         TryExtractBREAK_16e(inst , bits)||
         TryExtractBTEQZ_16e(inst , bits)||
         TryExtractBTEQZ_16e_ext(inst , bits)||
         TryExtractBTNEZ_16e(inst , bits)||
         TryExtractBTNEZ_16e_ext(inst , bits)||
         TryExtractCMP_16e(inst , bits)||
         TryExtractCMPI_16e_imm(inst , bits)||
         TryExtractCMPI_16e_imm_ext(inst , bits)||
         TryExtractDIV_16e(inst , bits)||
         TryExtractDIVU_16e(inst , bits)||
         TryExtractMOVE_16e(inst , bits)||
         TryExtractMOVE_16e_r32(inst , bits)||
         TryExtractMULT_16e(inst , bits)||
         TryExtractMULTU_16e(inst , bits)||
         TryExtractNEG_16e(inst , bits)||
         TryExtractNOT_16e(inst , bits)||
         TryExtractOR_16e(inst , bits)||
         TryExtractRESTORE_16e(inst , bits)||
         TryExtractRESTORE_16e_ext(inst , bits)||
         TryExtractSAVE_16e(inst , bits)||
         TryExtractSAVE_16e_ext(inst , bits)||
         TryExtractSB_16e(inst , bits)||
         TryExtractSB_16e_ext(inst , bits)||
         TryExtractSDBBP_16e(inst , bits)||
         TryExtractSEB_16e(inst , bits)||
         TryExtractINS(inst, bits) ||
         TryExtractASMACRO_16e(inst , bits)||
         TryExtractDMT_MT(inst , bits)||
         TryExtractDVPE_MT(inst , bits)||
         TryExtractEMT_MT(inst , bits)||
         TryExtractEVPE_MT(inst , bits)||
         TryExtractFORK_MT(inst , bits)||
         TryExtractMFTR_MT(inst , bits)||
         TryExtractMTTR_MT(inst , bits)||
         TryExtractYIELD_MT(inst , bits);
    }

  }  // namespace mipsel
  
}  // namespace remill
