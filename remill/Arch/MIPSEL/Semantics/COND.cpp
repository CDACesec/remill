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


namespace {


template <typename S1, typename S2, typename C>
DEF_SEM(CMP_COND_FMT, S1 src1, S2 src2, C cond) {
   uint32_t cond_kind = Read(cond);
   bool res;
   switch (cond_kind)
   {
   case 0: /*False*/
      res = false;
      break;

   case 2: /* oeq */
      res = (Read(src1) == Read(src2));
      break;

   case 12: /* lt */
      res = (Read(src1) < Read(src2));
      break;

   case 14: /* le */
      res = (Read(src1) <= Read(src2));
      break;

   default: /* not yet implemented */
      res = false;
      break;
   }
   WriteZExt(REG_CC, res);
   return memory;
}

template <typename S1, typename S2>
DEF_SEM(CVT_D_FMT, S1 dst, S2 src1) {
   Write(dst, static_cast<float64_t>(Read(src1)));
   return memory;
}

template <typename S1, typename S2>
DEF_SEM(CVT_S_FMT, S1 dst, S2 src1) {
   Write(dst, static_cast<float32_t>(Read(src1)));
   return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SLTIU, D dst, S1 src1, S2 src2) { 
  auto lhs = Read(src1);
  uint32_t rhs  = SExt(Signed(Read(src2))); 
  if (lhs < rhs)
     WriteZExt(dst,1) ;
  else 
     WriteZExt(dst,0);   
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SLTI, D dst, S1 src1, S2 src2) {
  int32_t lhs = Read(src1);
  int32_t rhs = SExt(Signed(Read(src2))); 
  if (lhs < rhs)
     WriteZExt(dst,1) ;
  else 
     WriteZExt(dst,0);   
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SLT, D dst, S1 src1, S2 src2) {
  auto lhs = static_cast<signed>(Read(src1));
  auto rhs = static_cast<signed>(Read(src2));
  if (lhs < rhs)
     WriteZExt(dst,1) ;
  else 
     WriteZExt(dst,0);   
  return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SLTU, D dst, S1 src1, S2 src2) {
  uint32_t lhs = Read(src1);
  uint32_t rhs = Read(src2); 
  if (lhs < rhs)
     WriteZExt(dst,1) ;
  else  
     WriteZExt(dst,0);   
  return memory;
}

template <typename S1, typename S2 >
DEF_SEM(CMP_16e, S1 src1, S2 src2) {
   uint32_t lhs = Read(src1);
   uint32_t rhs = Read(src2);
   uint32_t res = UXor(lhs,rhs);
   Write(REG_T8,res);
   return memory;
}

template <typename S1, typename S2>
DEF_SEM(CMPI_16e_imm, S1 src1, S2 src2) {
   uint32_t lhs = Read(src1);
   uint32_t rhs = ZExt(Read(src2));
   uint32_t res = UXor(lhs,rhs);
   Write(REG_T8,res);
   return memory;
}

template <typename S1, typename S2, typename S3, typename S4>
DEF_SEM(CMPI_16e_imm_ext, S1 src1, S2 src2, S3 src3, S4 src4) {
   uint32_t lhs = Read(src1); 
   uint32_t first_offset = Read(src1);
   uint32_t second_offset = Read(src2);
   uint32_t third_offset = Read(src3);
   uint32_t res = ZExt(first_offset + (second_offset << 5) + (third_offset << 11));
   uint32_t result = (res xor lhs); 
   Write(REG_T8,result);
   return memory;
}
    

}  // namespace

DEF_ISEL(CMP_COND_FMT_D) = CMP_COND_FMT<FP64, FP64, I32>;
DEF_ISEL(CMP_COND_FMT_S) = CMP_COND_FMT<FP32, FP32, I32>;
DEF_ISEL(CVT_D_FMT_S) = CVT_D_FMT<FP64W, FP32>;
DEF_ISEL(CVT_D_FMT_W) = CVT_D_FMT<FP64W, R32>;
DEF_ISEL(CVT_S_FMT_D) = CVT_S_FMT<FP32W, FP64>;
DEF_ISEL(CVT_S_FMT_W) = CVT_S_FMT<FP32W, R32>;

DEF_ISEL(SLTIU_IMM) = SLTIU<R32W,R32,I16>;
DEF_ISEL(SLTI_IMM) = SLTI<R32W,R32,I16>;
DEF_ISEL(SLT_32_R) = SLT<R32W,R32,R32>;
DEF_ISEL(SLTU_32_R) = SLTU<R32W,R32,R32>;

DEF_ISEL(CMP_16e) = CMP_16e<R32W,I16>;
DEF_ISEL(CMPI_16e_imm) = CMPI_16e_imm<R32W,I16>;
DEF_ISEL(CMPI_16e_imm_ext) = CMPI_16e_imm_ext<R32W,I16,I16,I16>;