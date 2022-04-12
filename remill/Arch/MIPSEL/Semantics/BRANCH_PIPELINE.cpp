/*
 * Copyright 2020 Centre for Development of Advanced Computing (C-DAC)
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
DEF_SEM(BEQ, R8W cond, PC br_taken, PC br_not_taken, S1 reg1, S2 reg2) {
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = UCmpEq(Read(reg1), Read(reg2));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc)); 
    return memory;
}

template <typename S1, typename S2> 
DEF_SEM(BEQL, R8W cond, PC br_taken, PC br_not_taken, S1 reg1, S2 reg2) {
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = UCmpEq(Read(reg1), Read(reg2));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc)); 
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BNE, R8W cond, PC br_taken, PC br_not_taken, S1 reg1, S2 reg2) {
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = UCmpNeq(Read(reg1), Read(reg2));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc)); 
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BNEL, R8W cond, PC br_taken, PC br_not_taken, S1 reg1, S2 reg2) {
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = UCmpNeq(Read(reg1), Read(reg2));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename D, typename S1 >
DEF_SEM(B, D dst, S1 src1) {
    auto rhs = Read(src1);
    auto lhs = (rhs || 0^2);
    WriteZExt(dst,lhs);
    return memory;
}

template <typename S1 >
DEF_SEM(B_16e, S1 src1) {
    auto rhs = Read(src1);
    auto res = SExt(rhs << 1); 
    uint32_t target_addr = res + REG_PC + 2;
    Write(REG_PC,target_addr);
    return memory;
}

template <typename S1, typename S2, typename S3>
DEF_SEM(B_16e_ext, S1 src1, S2 src2, S3 src3) {
    uint16_t first_offset = Read(src1);
    uint16_t second_offset = Read(src2);
    uint16_t third_offset = Read(src3);
    uint16_t res = first_offset + (second_offset << 5) + (third_offset << 11);
    uint16_t shift_res = SExt(res << 1); 
    auto target_addr = shift_res + REG_PC + 4;
    Write(REG_PC,target_addr);
    return memory;
}

template <typename S1, typename S2 >
DEF_SEM(BEQZ_16e, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    if(rhs == 0){
        uint8_t offset = Read(src2);
        uint8_t shifted_res = SExt(offset << 1);
        uint32_t target_addr = shifted_res + REG_PC + 2;
        Write(REG_PC,target_addr);
        return memory;
    }
}

template <typename S1, typename S2, typename S3, typename S4>
DEF_SEM(BEQZ_16e_ext, S1 src1, S2 src2, S3 src3, S4 src4) {
    auto reg = Read(src1);
    if(reg == 0){
    uint16_t first_offset = Read(src2);
    uint16_t second_offset = Read(src3);
    uint16_t third_offset = Read(src4);
    uint16_t res = first_offset + (second_offset << 5) + (third_offset << 11);
    uint16_t shift_res = SExt(res << 1); 
    auto target_addr = shift_res + REG_PC + 4;
    Write(REG_PC,target_addr);
    return memory;
    }
}

template <typename S1, typename S2 >
DEF_SEM(BNEZ_16e, S1 src1, S2 src2) {
    auto rhs = Read(src1);
    if(rhs != 0){
        uint8_t offset = Read(src2);
        uint8_t shifted_res = SExt(offset << 1);
        uint32_t target_addr = shifted_res + REG_PC + 2;
        Write(REG_PC,target_addr);
        return memory;
    }
}

template <typename S1, typename S2, typename S3, typename S4>
DEF_SEM(BNEZ_16e_ext, S1 src1, S2 src2, S3 src3, S4 src4) {
    auto reg = Read(src1);
    if(reg != 0){
    uint16_t first_offset = Read(src2);
    uint16_t second_offset = Read(src3);
    uint16_t third_offset = Read(src4);
    uint16_t res = first_offset + (second_offset << 5) + (third_offset << 11);
    uint16_t shift_res = SExt(res << 1); 
    auto target_addr = shift_res + REG_PC + 4;
    Write(REG_PC,target_addr);
    return memory;
    }
}
    

template <typename S1>
DEF_SEM(BREAK_16e, S1 src1) {
}

template <typename S1 >
DEF_SEM(BTEQZ_16e, S1 src1) {
    if(REG_T8 == 0){
        uint8_t offset = Read(src1);
        uint8_t shifted_res = SExt(offset << 1);
        uint32_t target_addr = shifted_res + REG_PC + 2;
        Write(REG_PC,target_addr);
        return memory;
    }
}

template <typename S1, typename S2, typename S3>
DEF_SEM(BTEQZ_16e_ext, S1 src1, S2 src2, S3 src3) {
    if(REG_T8 == 0){
        uint16_t first_offset = Read(src1);
        uint16_t second_offset = Read(src2);
        uint16_t third_offset = Read(src3);
        uint16_t res = first_offset + (second_offset << 5) + (third_offset << 11);
        uint16_t shift_res = SExt(res << 1); 
        auto target_addr = shift_res + REG_PC + 4;
        Write(REG_PC,target_addr);
        return memory;
        }
    }
    
template <typename S1>
DEF_SEM(BTNEZ_16e, S1 src1) {
    if(REG_T8 != 0){
        uint8_t offset = Read(src1);
        uint8_t shifted_res = SExt(offset << 1);
        uint32_t target_addr = shifted_res + REG_PC + 2;
        Write(REG_PC,target_addr);
        return memory;
    }
}

template <typename S1, typename S2, typename S3>
DEF_SEM(BTNEZ_16e_ext, S1 src1, S2 src2, S3 src3) {
    if(REG_T8 != 0){
        uint16_t first_offset = Read(src1);
        uint16_t second_offset = Read(src2);
        uint16_t third_offset = Read(src3);
        uint16_t res = first_offset + (second_offset << 5) + (third_offset << 11);
        uint16_t shift_res = SExt(res << 1); 
        auto target_addr = shift_res + REG_PC + 4;
        Write(REG_PC,target_addr);
        return memory;
    }
}


} // namespace 

DEF_ISEL(BEQ_BRANCH_EQUAL) = BEQ<R32,R32>; 
DEF_ISEL(BNE_BRANCH_NOT_EQUAL) = BNE<R32,R32>; 

DEF_ISEL(BNEL_BRANCH_LIKELY) = BNEL<R32,R32>;

DEF_ISEL(B_32_offset) = B<R32W,I32>;

DEF_ISEL(B_16e) = B_16e<I16>;

DEF_ISEL(B_16e_ext) = B_16e_ext<I16, I16, I16>;

DEF_ISEL(BEQZ_16e) = BEQZ_16e<R32W,I16>;

DEF_ISEL(BEQZ_16e_ext) = BEQZ_16e_ext<R32,I16, I16, I16>;

DEF_ISEL(BNEZ_16e) = BNEZ_16e<R32W,I16>;

DEF_ISEL(BNEZ_16e_ext) = BNEZ_16e_ext<R32,I16, I16, I16>;

DEF_ISEL(BREAK_16e) = BREAK_16e<I16>;

DEF_ISEL(BTEQZ_16e) = BTEQZ_16e<I16>;

DEF_ISEL(BTEQZ_16e_ext) = BTEQZ_16e_ext<I16, I16, I16>;

DEF_ISEL(BTNEZ_16e) = BTNEZ_16e<I16>;

DEF_ISEL(BTNEZ_16e_ext) = BTNEZ_16e_ext<I16, I16, I16>;

DEF_ISEL(BEQL_BRANCH_LIKELY) = BEQL<R32,R32>;