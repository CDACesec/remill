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

template <typename D, typename S1, typename S2>
DEF_SEM(SRA, D dst, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    auto lhs = Read(src2);
    WriteZExt(dst,(rhs,lhs));
    return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(ROTR, D dst, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    auto lhs = Read(src2);
    WriteZExt(dst,(rhs,lhs));
    return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SRAV, D dst, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    auto lhs = Read(src2);
    auto res = rhs >> lhs;
    Write(dst,res);
    return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SRL, D dst, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    auto lhs = Read(src2);
    WriteZExt(dst,(rhs,lhs));
    return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SLL, D dst, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    auto lhs = Read(src2);
    WriteZExt(dst,(rhs,lhs));
    return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SLLV, D dst, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    auto lhs = Read(src2);
    auto res = rhs << lhs;
    Write(dst,res);
    return memory;
}

template <typename D, typename S1, typename S2>
DEF_SEM(SRLV, D dst, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    auto lhs = Read(src2);
    auto res = rhs >> lhs;
    Write(dst,res);
    return memory;
}

} // namespace

DEF_ISEL(SRA_RIGHT_SHIFT) = SRA<R32W,R32,I32>;
DEF_ISEL(SRAV_RIGHT_ARITHMETIC_VARIABLE) = SRAV<R32W,R32,R32>;
DEF_ISEL(SRL_RIGHT_SHIFT_LOGICAL) = SRL<R32W,R32,I32>;
DEF_ISEL(SLL_LEFT_SHIFT_LOGICAL) = SLL<R32W,R32,I32>;
DEF_ISEL(SLLV_LEFT_LOGICAL_VARIABLE) = SLLV<R32W,R32,R32>;
DEF_ISEL(SRLV_RIGHT_LOGICAL_VARIABLE) = SRLV<R32W,R32,R32>;
DEF_ISEL(ROTR) = ROTR<R32W,R32,I32>;