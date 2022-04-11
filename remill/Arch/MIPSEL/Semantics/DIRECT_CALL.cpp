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

template <typename D>
DEF_SEM(JAL, D dst,PC ret_addr) {
  Write(REG_RA,Read(ret_addr));
  WriteZExt(REG_PC,Read(dst));
  return memory;
}

}  // namespace

DEF_ISEL(JAL_Target) = JAL<I32>;