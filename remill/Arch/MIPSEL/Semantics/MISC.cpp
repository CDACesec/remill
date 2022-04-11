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


template <typename S1, typename S2>
DEF_SEM(TEQ, S1 src1, S2 src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  if(lhs == rhs){
    std::abort();
  }
  return memory;
}


DEF_SEM(NOP) {
  return memory;
}



template <typename D, typename S1>
DEF_SEM(RESTORE_16e, D dst, S1 src, I32 size, I32 pos ) {
  auto msbd = Read(size) - 1;
  auto lsb = Read(pos);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(RESTORE_16e_ext, D dst, S1 src, I32 size, I32 pos ) {
  auto msbd = Read(size) - 1;
  auto lsb = Read(pos);

  return memory;
}

template <typename D, typename S1>
DEF_SEM(SAVE_16e, D dst, S1 src, I32 size, I32 pos ) {
  auto msbd = Read(size) - 1;
  auto lsb = Read(pos);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(SAVE_16e_ext, D dst, S1 src, I32 size, I32 pos ) {

  auto msbd = Read(size) - 1;
  auto lsb = Read(pos);

  return memory;
}

template <typename D, typename S1, typename S2, typename S3>
DEF_SEM(INS, D dst, S1 src, S2 size, S3 pos ) {
  auto SIZE = Read(size);
  auto POS = Read(pos);
  auto extracted = (0xffffffff >> ( 32 - SIZE ) ) & Read(src);
  auto dest = Read(dst);
  auto left_bits = ( dest >> (32 - POS + SIZE) ) << (32 - POS + SIZE);
  auto right_bits = ( 0xffffffff >> (32 - POS) &  dest);
  uint32_t result = left_bits + (extracted  << (POS + SIZE) ) + ZExt(right_bits);
  Write(dst, result);
  return memory;
}

template <typename D, typename S1>
DEF_SEM(ASMACRO_16e, D dst, S1 src, I32 size, I32 pos ) {

  auto msbd = Read(size) - 1;
  auto lsb = Read(pos);
  return memory;
}

template <typename D, typename S1, typename S2, typename S3>
DEF_SEM(EXT, D dst, S1 src, S2 pos, S3 msbd ) {
  
  auto original = Read(src);
  uint32_t op1 = original >> Read(pos);
  uint32_t size = Read(msbd) + 1; 
  uint32_t mask = 0xffffffff >> (32 - size);
  uint32_t res = op1 & mask;
  WriteZExt(dst, res);
  return memory;
  }

template<typename D>
DEF_SEM(DMT_MT, D dst) {
  auto temp = Read(REG_VPEC_R);
  Write(dst, temp);
  
  int bit_to_clr = 15; /* refer 6.5, VPEControl Register (Mips MT module,Volume IV-f:) */
  temp = temp & (~(1 << bit_to_clr));
  Write(REG_VPEC_R, temp);
  return memory;
}

template<typename D>
DEF_SEM(DVPE_MT, D dst) {
  auto temp = Read(REG_MVPC_R);
  Write(dst, temp);
  
  int bit_to_clr = 0;
  temp = temp & (~(1 << bit_to_clr)); 
  Write(REG_MVPC_R, temp);
  return memory;
}

template<typename D>
DEF_SEM(EMT_MT, D dst) {
  auto temp = Read(REG_VPEC_R);
  Write(dst, temp);
  
  int bit_to_set = 15; 
  temp = temp | (1 << bit_to_set) ;
  Write(REG_VPEC_R, temp);
  return memory;
}

template<typename D>
DEF_SEM(EVPE_MT, D dst) {
  auto temp = Read(REG_MVPC_R);
  Write(dst, temp);
  int bit_to_set = 0; 
  temp = temp | (1 << bit_to_set); 
  Write(REG_MVPC_R, temp);
  return memory;
}

template<typename D, typename S1, typename S2>
DEF_SEM(FORK_MT, D dst, S1 src1, S2 src2) {
  Write(dst,Read(src2));
  Write(REG_TCRESTART, Read(src1));
  return memory;
}

template<typename D, typename S1>
DEF_SEM(MFTR_MT, D dst, S1 src1) {
  uint8_t temp = Read(REG_VPEC_R);     
  WriteZExt(dst,temp);
  return memory;
}

template<typename D, typename S1>
DEF_SEM(MTTR_MT, D dst, S1 src1) {
  uint32_t temp = Read(src1);     
  Write(REG_VPEC_R,temp);
  return memory;
}


template<typename D, typename S1>
DEF_SEM(YIELD_MT, D dst, S1 src1) {
  return memory;
}

}  // namespace 
 

DEF_ISEL(NOP) = NOP;
DEF_ISEL(TEQ_32) = TEQ<R32, R32>;
DEF_ISEL(INS) = INS<R32W, R32, I32, I32>;
DEF_ISEL(EXT) = EXT<R32W, R32, I32, I32>;

DEF_ISEL(RESTORE_16e) = RESTORE_16e<R32, R32>;
DEF_ISEL(RESTORE_16e_ext) = RESTORE_16e_ext<R32, R32>;
DEF_ISEL(SAVE_16e) = SAVE_16e<R32, R32>;
DEF_ISEL(SAVE_16e_ext) = SAVE_16e_ext<R32, R32>;

DEF_ISEL(ASMACRO_16e) = ASMACRO_16e<R32, R32>;
DEF_ISEL(DMT_MT) = DMT_MT<R32W>;
DEF_ISEL(DVPE_MT) = DVPE_MT<R32W>;
DEF_ISEL(EMT_MT) = EMT_MT<R32W>;
DEF_ISEL(EVPE_MT) = EVPE_MT<R32W>;
DEF_ISEL(FORK_MT) = FORK_MT<R32W, R32, R32>;
DEF_ISEL(MFTR_MT) = MFTR_MT<R32W, R32>;
DEF_ISEL(MTTR_MT) = MTTR_MT<R32W,R32>;
DEF_ISEL(YIELD_MT) = YIELD_MT<R32W,R32>;
