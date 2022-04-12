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

template <typename D>
DEF_SEM(JR, D dst) {
  WriteZExt(REG_PC,(Read(dst)));
  return memory;
}

template <typename D>
DEF_SEM(JR_HB, D dst) {
  WriteZExt(REG_PC,(Read(dst)));
  return memory;
}

template <typename D>
DEF_SEM(J, D target_pc) {
  WriteZExt(REG_PC,(Read(target_pc)));
  return memory;
}


template <typename T>
DEF_SEM(JALR, PC ret_addr , T target ) {
  Write(REG_RA, Read(ret_addr));
  Write(REG_PC, Read(target));
  return memory;
}

template <typename T>
DEF_SEM(JALR_HB, PC ret_addr , T target ) {
  Write(REG_RA, Read(ret_addr));
  Write(REG_PC, Read(target));
  return memory;
}

} // namespace

DEF_ISEL(JR_32_R) = JR<R32>;
DEF_ISEL(JR_HB) = JR_HB<R32>;
DEF_ISEL(J_Target) = J<I32>;
DEF_ISEL(JALR_32_R) = JALR<R32>;
DEF_ISEL(JALR_HB) = JALR_HB<R32>;