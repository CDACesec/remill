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
#pragma once

namespace {

  // Read a register directly. Sometimes this is needed for suppressed operands.
  // ALWAYS_INLINE static uint32_t _Read(Memory *, Reg reg) {
  ALWAYS_INLINE static IF_64BIT_ELSE(uint64_t, uint32_t)

    _Read(Memory *, Reg reg) {
    return reg.word;
  }

  // Write directly to a register. This is sometimes needed for suppressed
  // register operands.
  ALWAYS_INLINE static void _Write(Memory *, Reg &reg, uint32_t val) {
    reg.word = val;
  }

}  // namespace
