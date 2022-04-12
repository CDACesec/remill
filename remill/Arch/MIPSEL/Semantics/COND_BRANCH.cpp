/*
 * Copyright (c) 2020 Centre for Development of Advanced Computing (C-DAC)
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
template <typename S >
DEF_SEM(BGEZAL, R8W cond, PC br_taken, PC br_not_taken, S reg) {
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = BOr(UCmpEq(Read(reg), 0), UCmpGt(Read(reg), 0));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc)); 
    return memory;
}

template <typename S1, typename S2 >
DEF_SEM(BLTZAL, S1 src1, S2 src2,R8W cond ) {
    auto rhs = Read(src1);
    addr_t taken_pc = Read(src2);
    auto ret_addr = Read(ZExtTo<addr_t>(UAdd(REG_PC ,static_cast<uint32_t>(4))));
    addr_t not_taken_pc =ret_addr;
    uint8_t take_branch = false;
    if (rhs < 0)
      take_branch = true;
    Write(REG_RA,Read(ret_addr));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));  
    return memory;
}

template <typename S1>
DEF_SEM(BGEZ, R8W cond, PC br_taken, PC br_not_taken, S1 reg1) {
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = static_cast<int32_t>(Read(reg1)) >= static_cast<int32_t>(Read(REG_ZERO));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));  
    return memory;
}


template <typename S1, typename S2 >
DEF_SEM(BLTZL, S1 src1, S2 src2,R8W cond ) {
    auto rhs = Read(src1);
    addr_t taken_pc = Read(src2);
    addr_t not_taken_pc = Read(ZExtTo<addr_t>(UAdd(REG_PC ,static_cast<uint32_t>(4))));
    uint8_t take_branch = false;
    if (rhs < 0)
      take_branch = true;
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));  
    return memory;
}

template <typename S1, typename S2 >
DEF_SEM(BGEZL, S1 src1, S2 src2,R8W cond) {
    auto rhs = Read(src1);
    addr_t taken_pc = Read(src2);
    addr_t not_taken_pc = Read(ZExtTo<addr_t>(UAdd(REG_PC ,static_cast<uint32_t>(4))));
    uint8_t take_branch = false;
    if (rhs >= 0)
      take_branch = true;
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));  
    return memory;
}

template <typename S1>
DEF_SEM(BLEZ, R8W cond, PC br_taken, PC br_not_taken, S1 reg1) {
    auto rhs = Read(reg1);
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = false;
    if (rhs <= 0)
      take_branch = true;
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));  
    return memory;
}

template <typename S1, typename S2 >
DEF_SEM(BGTZL, S1 src1, S2 src2,R8W cond ) {
    auto rhs = Read(src1);
    addr_t taken_pc = Read(src2);
    addr_t not_taken_pc = Read(ZExtTo<addr_t>(UAdd(REG_PC ,static_cast<uint32_t>(4))));
    uint8_t take_branch = false;
    if (rhs > 0)
      take_branch = true;
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));  
    return memory;
}

template <typename S>
DEF_SEM(BLTZ, R8W cond, PC br_taken, PC br_not_taken, S reg) {
    auto lhs = Read(reg);
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = static_cast<int32_t>(Read(reg)) < static_cast<int32_t>(Read(REG_ZERO));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));  
    return memory;
}

template <typename S1>
DEF_SEM(BGTZ, R8W cond, PC br_taken, PC br_not_taken, S1 reg1) {
    addr_t taken_pc = Read(br_taken);
    addr_t not_taken_pc = Read(br_not_taken);
    uint8_t take_branch = static_cast<int32_t>(Read(reg1)) > static_cast<int32_t>(Read(REG_ZERO));
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));  
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BC1F, S2 src, R8W cond) {
    addr_t taken_pc = Read(src);
    addr_t not_taken_pc = REG_PC + 8;
    bool take_branch = false;
    if(REG_CC == 0) {
        take_branch = true;
    }
    Write(cond, take_branch);                                          
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BC2F, S2 src, R8W cond) {
    addr_t taken_pc = Read(src);
    addr_t not_taken_pc = REG_PC + 8;
    bool take_branch = false;
    if(REG_COP2CC == 0) {                                              
        take_branch = true;
    }
    Write(cond, take_branch);                                            
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BC2FL, S2 src, R8W cond) {
    addr_t taken_pc = Read(src);
    addr_t not_taken_pc = REG_PC + 4;
    bool take_branch = false;
    if(REG_COP2CC == 0) {                                                
        take_branch = true;
    }
    Write(cond, take_branch);                                          
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BC1FL, S2 src, R8W cond) {
    addr_t taken_pc = Read(src);
    addr_t not_taken_pc = REG_PC + 8;                                  
    bool take_branch = false;
    if(REG_CC == 0) {
        take_branch = true;
    }
    Write(cond, take_branch);                                         
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BC1T, S2 src, R8W cond) {
    addr_t taken_pc = Read(src);
    addr_t not_taken_pc = REG_PC + 8;
    bool take_branch = false;
    if(REG_CC) {
        take_branch = true;
    }
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BC2T, S2 src, R8W cond) {
    addr_t taken_pc = Read(src);
    addr_t not_taken_pc = REG_PC + 8;
    bool take_branch = false;
    if(REG_COP2CC) {                                                
        take_branch = true;
    }
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BC2TL, S2 src, R8W cond) {
    addr_t taken_pc = Read(src);
    addr_t not_taken_pc = REG_PC + 4;
    bool take_branch = false;
    if(REG_COP2CC) {                                               
        take_branch = true;
    }
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename S1, typename S2>
DEF_SEM(BC1TL, S2 src, R8W cond) {
    addr_t taken_pc = Read(src);
    addr_t not_taken_pc = REG_PC + 8;                              
    bool take_branch = false;
    if(REG_CC) {
        take_branch = true;
    }
    Write(cond, take_branch);
    Write(REG_PC, Select<addr_t>(take_branch, taken_pc, not_taken_pc));
    return memory;
}

template <typename D, typename S1>
DEF_SEM(BAL, D dst, S1 src1) {
    auto rhs = Read(src1);
    WriteZExt(dst,rhs);
    return memory;
}


} // namespace

DEF_ISEL(BGEZAL_LINK) = BGEZAL<R32>;
DEF_ISEL(BLTZAL) = BLTZAL<R32,I32>;
DEF_ISEL(BLTZL_BRANCH_LIKELY) = BLTZL<R32,I32>;
DEF_ISEL(BGTZL_BRANCH_LIKELY) = BGTZL<R32,I32>;
DEF_ISEL(BGEZL_BRACH_LIKELY) = BGEZL<R32,I32>;
DEF_ISEL(BC1F_FP_FALSE) = BC1F<I32, I8>;
DEF_ISEL(BC2F_FP_FALSE) = BC2F<I32, I8>;
DEF_ISEL(BC2FL_FALSE_LIKELY) = BC2FL<I32, I8>;
DEF_ISEL(BC1FL_FP_FALSE_LIKELY) = BC1FL<I32, I8>;
DEF_ISEL(BC1T_FP_TRUE) = BC1T<I32, I8>;
DEF_ISEL(BC2T_FP_TRUE) = BC2T<I32, I8>;
DEF_ISEL(BC2TL_TRUE_LIKELY) = BC2TL<I32, I8>;
DEF_ISEL(BC1TL_FP_TRUE_LIKELY) = BC1TL<I32, I8>;

DEF_ISEL(BAL_BRANCH_LINK) = BAL<R32W,I32>;

DEF_ISEL(BGEZ) = BGEZ<R32>;
DEF_ISEL(BLEZ) = BLEZ<R32>;
DEF_ISEL(BLTZ) = BLTZ<R32>;
DEF_ISEL(BGTZ_32_IMM_COND) = BGTZ<R32>;