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

//SSNOP
bool TryDecodeSSNOP_NO_OPERATION(const InstData &data, Instruction &inst){
  return true;  
}

//PREF hint, offset(base)
bool TryDecodePREF(const InstData &data, Instruction &inst){
  return false;  
}

//SYNCI  offset(base)
bool TryDecodeSYNCI(const InstData &data, Instruction &inst){
  return false;  
}

//JALX <target>
bool TryDecodeJALX_JUMP_LINK(const InstData &data, Instruction &inst){
  return false;  
}

//COP2 func
bool TryDecodeCOP2_OPERATION(const InstData &data, Instruction &inst){
  return false;  
}

//CACHE op, offset(base)
bool TryDecodeCACHE_OPERATION(const InstData &data, Instruction &inst){
  return false;  
}


namespace {

static bool (* const kDecoder[])(const InstData &data, Instruction &inst) = {

  TryDecodeLUI_IMM,
  TryDecodeLW_32_IMM,
  TryDecodeLWC1_FLOATING_POINT,
  TryDecodeLDC1_FLOATING_R,
  TryDecodeADD_word,
  TryDecodeADDU_32_R,
  TryDecodeADDI_32_IMM,                  
  TryDecodeADDIU_32_IMM,
  TryDecodeADD_FMT,
  TryDecodeDIV_32,
  TryDecodeDIV_fmt,
  TryDecodeDIVU,
  TryDecodeMUL_32,
  TryDecodeMUL_fmt,
  TryDecodeMULT,
  TryDecodeMULTU,
  TryDecodeMADD,
  TryDecodeMOV_fmt,
  TryDecodeMOVZ,
  TryDecodeMOVN,
  TryDecodeMOVF,
  TryDecodeMOVT,
  TryDecodeMFHI,
  TryDecodeMFLO,
  TryDecodeMTLO,
  TryDecodeTEQ_32,
  TryDecodeBGTZ_32_IMM_COND,
  TryDecodeCMP_COND_FMT,
  TryDecodeCVT_D_FMT,
  TryDecodeCVT_S_FMT,
  TryDecodeMFC1_FLOATING_POINT,
  TryDecodeMFHC1_FLOATING_POINT,
  TryDecodeMTHC1_FLOATING_POINT,
  TryDecodeMTC1_FLOATING_POINT,
  TryDecodeNEG_fmt,
  TryDecodeSUBU_32_R,
  TryDecodeSW_32_IMM,
  TryDecodeSWL,
  TryDecodeSWC1_FLOATING_POINT,
  TryDecodeSDC1_FLOATING_R,
  TryDecodeSH_16_MEM,
  TryDecodeSEH,
  TryDecodeLH,
  TryDecodeLBU_32_R,
  TryDecodeLB_32_R,
  TryDecodeLHU_32_R,
  TryDecodeNOP,
  TryDecodeSLTIU_IMM,
  TryDecodeSLTI_IMM,
  TryDecodeSLT_32_R,
  TryDecodeSLTU_32_R,
  TryDecodeSUB_FMT,
  TryDecodeOR_32_R,
  TryDecodeORI_32_IMM,
  TryDecodeAND_32_R,
  TryDecodeANDI_32_IMM,
  TryDecodeNOR_32_R,
  TryDecodeXOR_32_R,
  TryDecodeXORI_32_IMM,
  TryDecodeJR_32_R,
  TryDecodeJ_Target,
  TryDecodeJALR_32_R,
  TryDecodeJAL_Target,
  TryDecodeJALX_JUMP_LINK,
  TryDecodeSRA_RIGHT_SHIFT,
  TryDecodeSRAV_RIGHT_ARITHMETIC_VARIABLE,
  TryDecodeSRL_RIGHT_SHIFT_LOGICAL,
  TryDecodeSLL_LEFT_SHIFT_LOGICAL,
  TryDecodeSLLV_LEFT_LOGICAL_VARIABLE,
  TryDecodeSRLV_RIGHT_LOGICAL_VARIABLE,
  TryDecodeBEQ_BRANCH_EQUAL,
  TryDecodeBNE_BRANCH_NOT_EQUAL,
  TryDecodeB_32_offset,
  TryDecodeBGEZAL_LINK,
  TryDecodeBC1F_FP_FALSE,
  TryDecodeBC1T_FP_TRUE,
  TryDecodeSB_IMM,
  TryDecodeSEB_EXTEND_BYTE,
  TryDecodeBAL_BRANCH_LINK,
  TryDecodeBGEZ,
  TryDecodeBLEZ,
  TryDecodeBLTZ,
  TryDecodeBEQL_BRANCH_LIKELY,
  TryDecodeBLTZAL,
  TryDecodeBGTZL_BRANCH_LIKELY,
  TryDecodeBGEZL_BRANCH_LIKELY,
  TryDecodeBLTZL_BRANCH_LIKELY,
  TryDecodeBNEL_BRANCH_LIKELY,
  TryDecodeLL_LOAD_LINKED_WORD,
  TryDecodeROTRV,
  TryDecodeROTR,
  TryDecodeCFC1_FLOATING_POINT,
  TryDecodeCTC1,
  TryDecodeSC_STORE_CONDITIONAL_WORD,
  TryDecodeSUB_WORD,
  TryDecodeJALR_HB,
  TryDecodeJR_HB,
  TryDecodeSSNOP_NO_OPERATION,
  TryDecodeLWL_LOAD_WORD_LEFT,
  TryDecodeLWR_LOAD_WORD_RIGHT,
  TryDecodePREF,
  TryDecodeSYNCI,
  TryDecodeBC1FL_FP_FALSE_LIKELY,
  TryDecodeBC1TL_FP_TRUE_LIKELY,
  TryDecodeROUND_L_fmt,
  TryDecodeROUND_W_fmt,
  TryDecodeCEIL_L_fmt,
  TryDecodeCEIL_W_fmt,
  TryDecodeBC2F_FP_FALSE,
  TryDecodeBC2T_FP_TRUE,
  TryDecodeLDC2,
  TryDecodeLWC2,
  TryDecodeSDC2,
  TryDecodeSWC2,
  TryDecodeCFC2,
  TryDecodeCTC2,
  TryDecodeMFC2,
  TryDecodeMFHC2,
  TryDecodeMTC2,
  TryDecodeMTHC2,
  TryDecodeBC2FL_FALSE_LIKELY,
  TryDecodeBC2TL_TRUE_LIKELY,
  TryDecodeCACHE_OPERATION,
  TryDecodeCOP2_OPERATION,
  TryDecodeEXT,
  TryDecodeADDIU_16e_IMM,
  TryDecodeADDIU_16e_IMM_Word,
  TryDecodeADDIU_16e_IMM_SPRelative,
  TryDecodeADDU_16e_REG,
  TryDecodeAND_16e,
  TryDecodeB_16e,
  TryDecodeB_16e_ext,
  TryDecodeBEQZ_16e,
  TryDecodeBEQZ_16e_ext,
  TryDecodeBNEZ_16e,
  TryDecodeBNEZ_16e_ext,
  TryDecodeBREAK_16e,
  TryDecodeBTEQZ_16e,
  TryDecodeBTEQZ_16e_ext,
  TryDecodeBTNEZ_16e,
  TryDecodeBTNEZ_16e_ext,
  TryDecodeCMP_16e,
  TryDecodeCMPI_16e_imm,
  TryDecodeCMPI_16e_imm_ext,
  TryDecodeDIV_16e,
  TryDecodeDIVU_16e,
  TryDecodeMFHI_16e,
  TryDecodeMFLO_16e,
  TryDecodeMOVE_16e,
  TryDecodeMOVE_16e_r32,
  TryDecodeMULT_16e,
  TryDecodeMULTU_16e,
  TryDecodeNEG_16e,
  TryDecodeNOT_16e,
  TryDecodeOR_16e,
  TryDecodeRESTORE_16e,
  TryDecodeRESTORE_16e_ext,
  TryDecodeSAVE_16e,
  TryDecodeSAVE_16e_ext,
  TryDecodeSB_16e,
  TryDecodeSB_16e_ext,
  TryDecodeSDBBP_16e,
  TryDecodeSEB_16e,
  TryDecodeINS,
  TryDecodeASMACRO_16e,
  TryDecodeDMT_MT,
  TryDecodeDMT_MT,
  TryDecodeDVPE_MT,
  TryDecodeEMT_MT,
  TryDecodeEVPE_MT,
  TryDecodeFORK_MT,
  TryDecodeMFTR_MT,
  TryDecodeMTTR_MT,
  TryDecodeYIELD_MT,
  
  };
}  // namespace

bool TryDecodeMips(const InstData &data, Instruction &inst) {
  auto iform_num = static_cast<unsigned>(data.iform);
  return kDecoder[iform_num - 1](data, inst);
        }

    }  // namespace mipsel
}  // namespace remill
