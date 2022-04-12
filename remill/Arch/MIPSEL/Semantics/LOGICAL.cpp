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

template <typename D, typename S1,typename S2>
DEF_SEM(OR, D dst, S1 src1,S2 src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  auto res = UOr(lhs, rhs);
  WriteZExt(dst, res);
  return memory;
}

template <typename D, typename S1,typename S2>
DEF_SEM(AND, D dst, S1 src1,S2 src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  auto res = UAnd(lhs, rhs);
  WriteZExt(dst, res);
  return memory;
}

template <typename D, typename S1,typename S2>
DEF_SEM(AND_16e, D dst, S1 src1,S2 src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  auto res = UAnd(lhs, rhs);
  WriteZExt(dst, res);
  return memory;
}

template <typename D, typename S1,typename S2>
DEF_SEM(ANDI, D dst, S1 src1,S2 src2) {
  uint32_t lhs = Read(src1);
  auto rhs = Read(src2);
  uint32_t temp = ZExt(rhs);
  uint32_t res = UAnd(lhs, temp);       
  WriteZExt(dst, res);
  return memory;
}
  
template <typename D, typename S1,typename S2>
DEF_SEM(NOR, D dst, S1 src1,S2 src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  auto res = UOr(lhs, rhs);
  auto result = UNot(res);
  WriteZExt(dst, result);
  return memory;
}


template <typename D, typename S1,typename S2>
DEF_SEM(XOR, D dst, S1 src1,S2 src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  auto res = UXor(lhs, rhs);
  WriteZExt(dst, res);
  return memory;
}

template <typename D, typename S1,typename S2>
DEF_SEM(XORI, D dst, S1 src1,S2 src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  auto temp = ZExt(rhs);
  auto res = UXor(lhs, temp);
  WriteZExt(dst, res);
  return memory;
}

template <typename D, typename S1,typename S2>
DEF_SEM(ORI, D dst, S1 src1,S2 src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  auto temp = ZExt(rhs);
  auto res = UOr(lhs, temp); 
  Write(dst, res);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(NEG_fmt, D dst, S1 src1) {
  uint32_t res = !(Read(src1));
  uint32_t f = res + (0x00000001);  
  Write(dst, f);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(NEG_16e, D dst, S1 src1) {
  uint32_t reg = (Read(src1));
  uint32_t res = 0 - !(reg);  
  Write(dst, res);
  return memory;
}

template <typename S1, typename S2>
DEF_SEM(NOT_16e, S1 src1, S2 src2) {
  uint32_t lhs = Read(src1);
  uint32_t rhs = Read(src2);
  uint32_t res = !(rhs);
  Write(rhs, res);
  return memory;
}

template <typename S1, typename S2>
DEF_SEM(OR_16e, S1 src1, S2 src2) {
  uint32_t lhs = Read(src1);
  uint32_t rhs = Read(src2);
  uint32_t res = (lhs || rhs);
  Write(lhs, res);
  return memory;
}


}  // namespace

DEF_ISEL(OR_32_R)  = OR<R32W,R32,R32>;
DEF_ISEL(AND_32_R) = AND<R32W,R32,R32>;
DEF_ISEL(ANDI_32_IMM) = ANDI<R32W,R32,I16>;
DEF_ISEL(NOR_32_R) = NOR<R32W,R32,R32>;
DEF_ISEL(XOR_32_R) = XOR<R32W,R32,R32>;
DEF_ISEL(XORI_32_IMM) = XORI<R32W,R32,I16>;
DEF_ISEL(ORI_32_IMM) = ORI<R32W,R32,I16>;
DEF_ISEL(NEG_fmt) = NEG_fmt<FP32W,FP32>;
DEF_ISEL(NEG_16e) = NEG_16e<R32W,R32>;
DEF_ISEL(AND_16e) = AND_16e<R32W,R32,R32>;

DEF_ISEL(NOT_16e) = NOT_16e<R32,R32>;
DEF_ISEL(OR_16e) = OR_16e<R32,R32>;