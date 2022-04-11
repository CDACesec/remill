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

namespace {


template <typename D, typename S1, typename S2>
DEF_SEM(ADDI, D dst, S1 src1, S2 src2) {
  uint32_t temp = Read(src2);
  uint32_t final = ((!Read(src1)) + temp);
  if(final >> 31 == 1) {
     std::abort();
  }
  else 
    Write(dst, final);
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ADDIU, D dst, S1 src1, S2 src2) {
  uint32_t temp = Read(src2);
  WriteZExt(dst, UAdd(Read(src1), temp) );
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ADDIU_16e, D dst, S1 src1, S2 src2) {
  uint32_t temp = Read(src2);
  WriteZExt(dst, UAdd(Read(src1), temp) );
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ADDIU_16e_IMM, D dst, S1 src1, S2 src2) {
  uint32_t temp = Read(src2);
  WriteZExt(dst, UAdd(Read(src1), temp) );
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ADDIU_16e_IMM_SP, D dst, S1 src1, S2 src2) {
  uint32_t rhs = ZExt(Read(src2));
  uint32_t lhs = Read(REG_SP);
  WriteZExt(dst, (rhs + lhs) );
  return memory;
}


template <typename D, typename S1, typename S2>
DEF_SEM(ADD, D dst, S1 src1, S2 src2) {
  uint32_t res = (Read(src1) + Read(src2));

  if(res >> 31 == 1) {
     std::abort();
  }
  else 
    Write(dst, res);
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ADDU, D dst, S1 src1, S2 src2) {
  WriteZExt(dst, UAdd(Read(src1), Read(src2)));
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ADDU_16e, D dst, S1 src1, S2 src2) {
  WriteZExt(dst, UAdd(Read(src1), Read(src2)));
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ADD_FMT, D dst, S1 src1, S2 src2) {
  Write(dst, (Read(src1) + Read(src2)));
  return memory;
}

template <typename S1, typename S2>
DEF_SEM(DIV,S1 src1, S2 src2) {
  int32_t lhs = static_cast<signed>(Read(src1));
  int32_t rhs = static_cast<signed>(Read(src2));
  Write(REG_LO, SDiv(lhs, rhs));
  Write(REG_HI, SRem(lhs, rhs));
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(DIV_fmt,D dst, S1 src1, S2 src2) { 
  auto lhs = (Read(src1));
  auto rhs = (Read(src2));
  Write(dst, (lhs/rhs));
  return memory;
}

template <typename S1, typename S2>
DEF_SEM(DIVU,S1 src1, S2 src2) {
  int32_t lhs = static_cast<unsigned>(Read(src1));
  int32_t rhs = static_cast<unsigned>(Read(src2));
  Write(REG_LO, SDiv(lhs, rhs));
  Write(REG_HI, SRem(lhs, rhs));
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(MUL, D dst, S1 src1, S2 src2) {
  auto lhs = static_cast<signed> (Read(src1));
  auto rhs = static_cast<signed> (Read(src2));
  auto result = (lhs * rhs);
  WriteZExt(dst, (result & static_cast<signed>(0x00000000FFFFFFFF)));
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(MUL_fmt, D dst, S1 src1, S2 src2) {
  auto lhs = (Read(src1));
  auto rhs = (Read(src2));
  auto result = (lhs * rhs);
  Write(dst, result );
  return memory;
}

template <typename S1, typename S2>
DEF_SEM(MADD, S1 src1, S2 src2) {
  int32_t lhs = Read(src1);
  int32_t rhs = Read(src2);
  int64_t result1 = (lhs * rhs);

  int32_t lsb = Read(REG_LO);
  int32_t msb = Read(REG_HI);
  int64_t LSB = lsb;
  int64_t MSB = msb;
  int64_t result2 = (MSB + (LSB << 32));

  int64_t final = result1 + result2;
  int32_t least = final;
  int32_t most = final >> 32;
  Write(REG_LO,least);
  Write(REG_HI,most);
  
  return memory;
}

template <typename D, typename S1>
DEF_SEM(LW, D dst, S1 src1) { 
  Write(dst,(Read(src1)));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(LWL, D dst, S1 src1) { 
  auto rhs = Read(src1);
  auto rhs_low_bits = (rhs & 0x0000ffff);
  auto lhs = Read(dst);
  auto lhs_low_bits = (lhs & 0x0000ffff);
  auto result = std::to_string(rhs_low_bits) + std::to_string(lhs_low_bits);
  int32_t final_res = stoi(result); 
  Write(dst,final_res);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(LWR, D dst, S1 src1) { 
  auto rhs = Read(src1);
  auto rhs_low_bits = (rhs & 0xffffff00);
  auto lhs = Read(dst);
  auto lhs_low_bits = (lhs & 0xff000000);
  auto result = std::to_string(lhs_low_bits) + std::to_string(rhs_low_bits);
  int32_t final_res = stoi(result); 
  Write(dst,final_res);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(LL, D dst, S1 src1) { 
  Write(dst,(Read(src1)));
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ROTRV, D dst, S1 src1, S2 src2) {
  auto rhs = Read(src2);
  auto low_bit = (rhs & 0x0000001f);
  auto result = (Read(src1) << low_bit);  
  Write(dst,result);
  return memory;
}

template <typename S1, typename S2>
DEF_SEM(MULT, S1 src1, S2 src2) {
  int64_t lhs = static_cast<signed> (Read(src1));
  int64_t rhs = static_cast<signed> (Read(src2));
  int64_t result = lhs * rhs;
  int32_t lower = result ;
  int32_t higher = result >> 32;
  Write(REG_HI, higher);
  Write(REG_LO, lower);
  return memory;
}

template <typename S1, typename S2>
DEF_SEM(MULTU, S1 src1, S2 src2) {
  uint64_t lhs = static_cast<long> (Read(src1));
  uint64_t rhs = static_cast<long> (Read(src2));
  uint64_t result = lhs * rhs;
  uint32_t higher = result >> 32;
  uint32_t lower = (uint32_t)result ;
  Write(REG_HI, higher);
  Write(REG_LO, lower);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(LWC1, D dst, S1 src1) {
  Write(dst, Read(src1));
  return memory;
}

template <typename D, typename S1> 
DEF_SEM(LDC1, D dst, S1 src1) {   
  Write(dst,Read(src1));   
  return memory; 
}  

template <typename D, typename S1> 
DEF_SEM(LDC2, D dst, S1 src1) {   
  Write(dst,Read(src1));   
  return memory; 
}  

template <typename D, typename S1> 
DEF_SEM(SDC2, D dst, S1 src1) {   
  Write(dst,Read(src1));   
  return memory; 
}  

template <typename D, typename S1> 
DEF_SEM(SWC2, D dst, S1 src1) {
  auto rhs = Read(src1);
  int32_t result = (rhs & 0x00000000ffffffff);   
  Write(dst, result);   
  return memory; 
}  

template <typename D, typename S1> 
DEF_SEM(LWC2, D dst, S1 src1) {   
  WriteZExt(dst,Read(src1));   
  return memory; 
}  

template <typename D, typename S1>
DEF_SEM(LUI, D dst, S1 src1) {
  auto lhs = Read(src1); 
  WriteZExt(dst, lhs);
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SUB_FMT, D dst, S1 src1, S2 src2) {
  Write(dst, (Read(src1) - Read(src2)));
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SUBU, D dst, S1 src1, S2 src2) {
  WriteZExt(dst, (USub(Read(src1), Read(src2))));
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SUB, D dst, S1 src1, S2 src2) {
  int32_t res = (Read(src1) - Read(src2));
  if(res >> 32 == 1){
    std::abort();
  }  
  else
  Write(dst,res);  
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MOV_fmt, D dst, S1 src1) {
  Write(dst,(Read(src1)));
  return memory;
  
}
  
template <typename D, typename S1, typename S2>
DEF_SEM(MOVN, D dst, S1 src1, S2 src2) {
  auto result = Read(src2);
  if (result != 0)
      Write(dst, Read(src1));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MOVF, D dst, S1 src1) {
  if(REG_CC == 0) {
    uint32_t res = Read(src1);
    Write(dst,res);    
    }
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MOVT, D dst, S1 src1) {
  if(REG_CC == 1) {
    uint32_t res = Read(src1);
    Write(dst,res);    
    }
  return memory;
}

template <typename D>
DEF_SEM(MFHI, D dst) {
  WriteZExt(dst, (Read(REG_HI)));
  return memory;
}

template <typename D>
DEF_SEM(MFLO, D dst) {
  WriteZExt(dst, (Read(REG_LO)));
  return memory;
}

template <typename D>
DEF_SEM(MTLO, D dst) {
  WriteZExt(REG_LO, (Read(dst)));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(ROUND_L_fmt, D dst, S1 src1) {
  float64_t rhs =FRoundToNearestEven64(Read(src1));
  Write(dst, rhs);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(ROUND_W_fmt, D dst, S1 src1) {
  float32_t rhs = FRoundToNearestEven32(Read(src1));
  Write(dst, rhs);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(CEIL_L_fmt, D dst, S1 src1) {
  float64_t rhs = FRoundToPositiveInfinity64(Read(src1));
  Write(dst, rhs);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(CEIL_W_fmt, D dst, S1 src1) {
  float32_t rhs = FRoundToPositiveInfinity32(Read(src1));
  Write(dst, rhs);
  return memory;
}

template <typename S1, typename S2, typename S3>
DEF_SEM(MOVE_16e, S1 src1, S2 src2, S3 src3) {
  uint32_t reg_1 = Read(src1);
  uint32_t reg_2 = Read(src2);
  uint32_t reg_3 = Read(src3);

  uint32_t res = reg_2 + (2 << reg_3);
  Write(res,reg_1);;
  return memory;
}


template <typename S1, typename S2>
DEF_SEM(MOVE_16e_r32, S1 src1, S2 src2) {
  uint32_t reg_1 = Read(src1);
  uint32_t reg_2 = Read(src2);
  Write(reg_2,reg_1);;
  return memory;
}


template <typename D, typename S1, typename S2>
DEF_SEM(MOVZ, D dst, S1 src1, S2 src2) {
  auto result = Read(src2);
  if (result == 0)
      Write(dst, Read(src1));
  return memory;
    }

}  // namespace

DEF_ISEL(ADDI_32_IMM) = ADDI<R32W, R32, I32>;

DEF_ISEL(ADDIU_32_IMM) = ADDIU<R32W, R32, I32>;

DEF_ISEL(ADDIU_16e_IMM) = ADDIU_16e<R32W, R32, I32>;

DEF_ISEL(ADDIU_16e_IMM_Word) = ADDIU_16e_IMM<R32W, R32, I32>;

DEF_ISEL(ADDIU_16e_IMM_SPRelative) = ADDIU_16e_IMM_SP<R32W, R32, I32>;

DEF_ISEL(ADD_word) = ADD<R32W, R32, R32>;

DEF_ISEL(ADDU_32_R) = ADDU<R32W, R32, R32>;

DEF_ISEL(ADDU_16e_REG) = ADDU_16e<R32W, R32, R32>;

DEF_ISEL(DIV_32) = DIV<R32, R32>;

DEF_ISEL(DIV_16e) = DIV<R32, R32>;

DEF_ISEL(DIV_fmt_D) = DIV_fmt<FP64W,FP64, FP64>;
DEF_ISEL(DIV_fmt_S) = DIV_fmt<FP32W,FP32, FP32>;               

DEF_ISEL(DIVU) = DIVU<R32, R32>;

DEF_ISEL(MUL_32) = MUL<R32W, R32, R32>;

DEF_ISEL(MUL_fmt_D) = MUL_fmt<FP64W,FP64, FP64>;               
DEF_ISEL(MUL_fmt_S) = MUL_fmt<FP32W,FP32, FP32>;               

DEF_ISEL(MULT) = MULT<R32,R32>;

DEF_ISEL(MULTU) = MULTU<R32,R32>;

DEF_ISEL(MULT_16e) = MULTU<R32,R32>;

DEF_ISEL(MULTU_16e) = MULTU<R32,R32>;

DEF_ISEL(MADD) = MADD<R32, R32>;

DEF_ISEL(MOV_fmt_D) = MOV_fmt<FP64W,FP64>;
DEF_ISEL(MOV_fmt_S) = MOV_fmt<FP32W,FP32>;

DEF_ISEL(MOVZ) = MOVZ<R32W,R32,R32>;

DEF_ISEL(ROTRV) = ROTRV<R32W,R32,R32>;

DEF_ISEL(MOVN) = MOVN<R32W,R32,R32>;

DEF_ISEL(MOVE_16e) = MOVE_16e<R32W,R32,R32>;

DEF_ISEL(MOVE_16e_r32) = MOVE_16e_r32<R32W,R32>;

DEF_ISEL(MOVF) = MOVF<R32W,R32>;

DEF_ISEL(MOVT) = MOVT<R32W,R32>;

DEF_ISEL(MFHI) = MFHI<R32W>;
DEF_ISEL(MFLO) = MFLO<R32W>;

DEF_ISEL(MFHI_16e) = MFHI<R32W>;
DEF_ISEL(MFLO_16e) = MFLO<R32W>;

DEF_ISEL(MTLO) = MTLO<R32W>;

DEF_ISEL(ADD_FMT_D) = ADD_FMT<FP64W, FP64, FP64>;
DEF_ISEL(ADD_FMT_S) = ADD_FMT<FP32W, FP32, FP32>;

DEF_ISEL(LW_32_IMM) = LW<R32W,M32>; 

DEF_ISEL(LWL_LOAD_WORD_LEFT) = LWL<R32W,M32>; 

DEF_ISEL(LWR_LOAD_WORD_RIGHT) = LWR<R32W,M32>; 

DEF_ISEL(LL_LOAD_LINKED_WORD) = LL<R32W,M32>;

DEF_ISEL(LWC1_FLOATING_POINT) = LWC1<FP32W,M32>;

DEF_ISEL(LDC1_FLOATING_R) = LDC1<FP64W,M64>;

DEF_ISEL(LDC2) = LDC2<R64W,M64>;

DEF_ISEL(SDC2) = SDC2<M64W,R64>;

DEF_ISEL(LWC2) = LWC2<R64W,M32>;

DEF_ISEL(SWC2) = SWC2<M32W,R64>;

DEF_ISEL(LUI_IMM) = LUI<R32W,I32>;

DEF_ISEL(SUBU_32_R) = SUBU<R32W,R32,R32>;

DEF_ISEL(SUB_WORD) = SUB<R32W,R32,R32>;

DEF_ISEL(SUB_FMT_D) = SUB_FMT<FP64W, FP64, FP64>;
DEF_ISEL(SUB_FMT_S) = SUB_FMT<FP32W, FP32, FP32>;

DEF_ISEL(ROUND_L_fmt_D) = ROUND_L_fmt<FP64W, FP64>;
DEF_ISEL(ROUND_L_fmt_S) = ROUND_L_fmt<FP64W, FP32>;

DEF_ISEL(ROUND_W_fmt_D) = ROUND_W_fmt<FP32W, FP64>;
DEF_ISEL(ROUND_W_fmt_S) = ROUND_W_fmt<FP32W, FP32>;

DEF_ISEL(CEIL_L_fmt_D) = CEIL_L_fmt<FP64W, FP64>;
DEF_ISEL(CEIL_L_fmt_S) = CEIL_L_fmt<FP64W, FP32>;

DEF_ISEL(CEIL_W_fmt_D) = CEIL_W_fmt<FP32W, FP64>;
DEF_ISEL(CEIL_W_fmt_S) = CEIL_W_fmt<FP32W, FP32>;