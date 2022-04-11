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

template <typename D, typename S1>
DEF_SEM(SW, D dst, S1 src1) {
  WriteZExt(dst,(Read(src1)));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(SC, D dst, S1 src1) {
  WriteZExt(dst,(Read(src1)));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(SWL, D dst, S1 src1) {
  WriteZExt(dst,(Read(src1)));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(SWC1, D dst, S1 src1) {
  Write(dst, Read(src1));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(SDC1, D dst, S1 src1) {
  Write(dst, static_cast<float64_t>(Read(src1)));
  return memory;
}


template <typename D, typename S1>
DEF_SEM(LBU, D dst, S1 src1) {
  WriteZExt(dst,Read(src1));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(LB, D dst, S1 src1) {
  auto rhs = Read(src1);
  WriteZExt(dst, (static_cast<signed>(rhs)));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(LHU, D dst, S1 src1) {
  auto rhs = Read(src1);
  WriteZExt(dst, (rhs));
  return memory;
}

template <typename D, typename S1 >
DEF_SEM(SB, D dst, S1 src1 ) {
  auto lhs = Read(src1);
  Write(dst,lhs);
  return memory;
}

template <typename D, typename S1 >
DEF_SEM(CFC1, D dst, S1 src1 ) {
  auto lhs = Read(src1);
  Write(dst,lhs);
  return memory;
}

template <typename D >
DEF_SEM(CFC2, D dst ) {
  Write(dst,Read(REG_CR));
  return memory;
}

template < typename S1 >
DEF_SEM(CTC2, S1 src1 ) {
  uint32_t lhs = Read(src1);
  Write(REG_CR,lhs);
  return memory;
}

template <typename S1, typename D >
DEF_SEM(CTC1, S1 src1, D dst ) {
  auto lhs = Read(src1);
  Write(dst,lhs);
  return memory;
}

template <typename D, typename S1 >
DEF_SEM(SEB, D dst, S1 src1 ) {
  auto lhs = Read(src1);
  auto rhs = (lhs & 0x000000ff);
  int32_t result = SExt(rhs);
  Write(dst,result);
  return memory;
}


template <typename D, typename S1>
DEF_SEM(SH, D dst, S1 src1) {
  Write(dst,Read(src1));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(SEH, D dst, S1 src1) {
  uint32_t res = SExt(Read(src1) & 0x0000ffff);
  Write(dst,res);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(LH, D dst, S1 src1) {
  uint16_t rhs = Read(src1);
  WriteSExt(dst, (rhs));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MFC1, D dst, S1 src1) {
  Write(dst, (Read(src1))); 
  return memory;
}

template <typename D>
DEF_SEM(MFC2, D dst) {
  Write(dst, (Read(REG_GR))); 
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MFHC2, D dst, S1 src1) {
  auto  rhs = Read(src1);
  uint32_t result = (rhs & 0xffffffff00000000);
  Write(dst, result); 
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MFHC1, D dst, S1 src1) {
  float64_t rhs = Read(src1);
  uint32_t result =(float) (((int64_t)rhs) & 0xffffffff00000000); 
  WriteZExt(dst,result );
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MTHC1, D dst, S1 src1) {
  float64_t rhs = Read(src1);
  return memory;
}

template <typename S1>
DEF_SEM(MTC2, S1 src1) {
  Write(REG_GR, Read(src1));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MTC1, D dst, S1 src1) {
  Write(dst, Read(src1));
  return memory;
}

template <typename D, typename S1>
DEF_SEM(MTHC2, D dst, S1 src1) {
  int64_t rhs = Read(src1);
  int64_t result = ( rhs << 32 );
  Write(dst, result);
  return memory;
}

template <typename S1, typename S2, typename S3 >
DEF_SEM(SB_16e, S1 src1, S2 src2, S3 src3 ) {
  auto lhs = Read(src2);
  auto offset = Read(src3);
  auto rhs = Read(src1);
  auto res = offset + rhs;
  Write(rhs, res);
  return memory;
}

template <typename S1, typename S2, typename S3, typename S4, typename S5 >
DEF_SEM(SB_16e_ext, S1 src1, S2 src2, S3 src3, S4 src4, S5 src5 ) {
  uint32_t lhs = Read(src1);
  uint32_t rhs = Read(src2);
  uint32_t off_1 = Read(src3);
  uint32_t off_2 = Read(src4);
  uint32_t off_3 = Read(src5);
  uint32_t total_off = SExt(off_1 + (off_2 << 11) + (off_3 << 21));
  uint32_t res = total_off + rhs;
  Write(lhs,res);
  return memory;
}

template < typename S1 >
DEF_SEM(SEB_16e, S1 src1 ) {
  auto lhs = Read(src1);
  auto rhs = (lhs & 0x000000ff);
  int32_t result = SExt(rhs);
  Write(lhs,result);
  return memory;
}


template <typename S1>
DEF_SEM(SDBBP_16e, S1 src1) {
  uint32_t rhs = Read(src1);
  uint32_t result = ( rhs << 32 );
  Write(rhs, result);
  return memory;
}


} // namespace


DEF_ISEL(LBU_32_R)  = LBU<R32W,M8>;
DEF_ISEL(LB_32_R)  = LB<R32W,M8>;
DEF_ISEL(LHU_32_R)  = LHU<R32W,M16>;

DEF_ISEL(SB_IMM) = SB<M8W, R8 >;

DEF_ISEL(SEB_EXTEND_BYTE) = SEB<R32W, R32>; 

DEF_ISEL(SH_16_MEM) = SH<M16W, R16>;

DEF_ISEL(SEH) = SEH<R32W, R32>;

DEF_ISEL(LH) = LH<R32W, M16>;

DEF_ISEL(MFC1_FLOATING_POINT) = MFC1<R32W, FP32>;
DEF_ISEL(MFHC1_FLOATING_POINT) = MFHC1<R32W, FP64>;
DEF_ISEL(MTHC1_FLOATING_POINT) = MTHC1<FP64W, R32>;
DEF_ISEL(MTC1_FLOATING_POINT) = MTC1<FP32W, R32>;

DEF_ISEL(SW_32_IMM) = SW<M32W, R32>;

DEF_ISEL(SC_STORE_CONDITIONAL_WORD) = SC<M32W, R32>;

DEF_ISEL(SWL) = SWL<M32W, R32>;

DEF_ISEL(SWC1_FLOATING_POINT) = SWC1<FPM32W, FP32>;

DEF_ISEL(SDC1_FLOATING_R) = SDC1<FPM64W, FP64>;

DEF_ISEL(CFC1_FLOATING_POINT) = CFC1<R32W, FP32>;

DEF_ISEL(CFC2) = CFC2<R32W>;

DEF_ISEL(CTC2) = CTC2<R32>;

DEF_ISEL(CTC1) = CTC1<R32, FP32W>;

DEF_ISEL(MFC2) = MFC2<R32W>;

DEF_ISEL(MFHC2) = MFHC2<R32W, R64>;

DEF_ISEL(MTC2) = MTC2<R32>;

DEF_ISEL(MTHC2) = MTHC2<R64W, R32>;

DEF_ISEL(SB_16e) = SB_16e<M32W, R32, I32>;

DEF_ISEL(SB_16e_ext) = SB_16e_ext<M32W, R32, R32, I16, I16>;

DEF_ISEL(SEB_16e) = SEB_16e<R32>;

DEF_ISEL(SDBBP_16e) = SDBBP_16e<R32>;
