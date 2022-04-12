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

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include <llvm/ADT/Triple.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#include "remill/Arch/MIPSEL/Decode.h"
#include "remill/Arch/Arch.h"
#include "remill/Arch/Instruction.h"
#include "remill/Arch/Name.h"
#include "remill/BC/Version.h"
#include "remill/OS/OS.h"

namespace remill {
namespace {

static constexpr int kInstructionSize = 4;  // In bytes.
static constexpr int kPCWidth = 32;  // In bits.              

template <uint32_t bit, typename T>
static inline T Select(T val) {
  return (val >> bit) & T(1);
}

Instruction::Category InstCategory(const mipsel::InstData &inst) {
  switch (inst.iclass) {
    case mipsel::InstName::INVALID:
      return Instruction::kCategoryInvalid;
                                                                              
    case mipsel::InstName::J:    
    case mipsel::InstName::JALX:
      return Instruction::kCategoryDirectJump;
      
    
    case mipsel::InstName::BEQ: 
    case mipsel::InstName::BNE: 
    case mipsel::InstName::BAL: 
    case mipsel::InstName::BC1F:
    case mipsel::InstName::BC1T:
    case mipsel::InstName::BGEZ:
    case mipsel::InstName::BLEZ:    
    case mipsel::InstName::BLTZ:
    case mipsel::InstName::BGTZ:
    case mipsel::InstName::BEQL:
    case mipsel::InstName::BGTZL:
    case mipsel::InstName::BGEZL:
    case mipsel::InstName::BLTZL:
    case mipsel::InstName::BNEL:
    case mipsel::InstName::BC1FL:
    case mipsel::InstName::BC1TL:
    case mipsel::InstName::BC2F:
    case mipsel::InstName::BC2T:
    case mipsel::InstName::BC2FL:
    case mipsel::InstName::BC2TL:
      return Instruction::kCategoryConditionalBranch;
    
    case mipsel::InstName::BGEZAL:
    case mipsel::InstName::BLTZAL:
      return Instruction::kCategoryConditionalCall;


    case mipsel::InstName::JAL:
      return Instruction::kCategoryDirectFunctionCall;

    case mipsel::InstName::JALR:
    case mipsel::InstName::JALR_HB:                        
      return Instruction::kCategoryIndirectFunctionCall; 

    case mipsel ::InstName::B:
      return Instruction::kCategoryPipelineBranch; 

    // case mipsel::InstName::SYSCALL:
      return Instruction::kCategoryAsyncHyperCall;

    case mipsel ::InstName :: NOP:                                        
    case mipsel ::InstName :: SSNOP: 
      return Instruction::kCategoryNoOp;


    case mipsel::InstName::JR:  
    case mipsel::InstName::JR_HB:                 
      return Instruction::kCategoryFunctionReturn;  


    default:
      return Instruction::kCategoryNormal;
  }
}

class MIPSELArch : public Arch {
 public:
  MIPSELArch(OSName os_name_, ArchName arch_name_);

  virtual ~MIPSELArch(void);

  // Decode an instruction.
  bool DecodeInstruction(
      uint64_t address, const std::string &instr_bytes,
      Instruction &inst) const final;

  // Maximum number of bytes in an instruction.
  uint64_t MaxInstructionSize(void) const final;

  llvm::Triple Triple(void) const final;
  llvm::DataLayout DataLayout(void) const final;

  // Default calling convention for this architecture.
  llvm::CallingConv::ID DefaultCallingConv(void) const final;

 private:
  MIPSELArch(void) = delete;
};

MIPSELArch::MIPSELArch(OSName os_name_, ArchName arch_name_)                           
    : Arch(os_name_, arch_name_) {}

MIPSELArch::~MIPSELArch(void) {}

// Default calling convention for this architecture.                                  
llvm::CallingConv::ID MIPSELArch::DefaultCallingConv(void) const {
  return llvm::CallingConv::C;
}

// Maximum number of bytes in an instruction for this particular architecture.
uint64_t MIPSELArch::MaxInstructionSize(void) const {
  return 4;
  
}

llvm::Triple MIPSELArch::Triple(void) const {
  auto triple = BasicTriple();
  switch (arch_name) {
    case kArchMips32: 
      triple.setArch(llvm::Triple::mipsel);
      break;

    default:
      LOG(FATAL)
          << "Cannot get triple for MIPSEL architecture "
          << GetArchName(arch_name);
      break;
  }
  return triple;
}                                                                             

llvm::DataLayout MIPSELArch::DataLayout(void) const {
  std::string dl;
  switch (arch_name) {
    case kArchMips32: 
      dl = "e-m:m-p:32:32-i8:8:32-i16:16:32-i64:64-n32-S64";      
      break;
                
    default:
      LOG(FATAL)
          << "Cannot get data layout for mips32 architecture "
          << GetArchName(arch_name);
      break;
  }
  return llvm::DataLayout(dl);
}

enum RegClass {                                                   
  kRegX,  // 64-bit int.
  kRegW,  // Word, 32-bit int.
  kRegB,  // Byte.
  kRegH,  // Half-word, 16-bit float.
  kRegS,  // Single-precision float.
  kRegD,  // Doubleword, Double precision float.
  kRegDW // Doubleword, coprocessor 2 registers.
  // kRegQ,  // Quadword.
  // kRegV,  // V reg containing Q, D, S, H, and B.
};

enum RegUsage {
  /*
    This is AArch64 specific stuff where X31 can be interpreted
    as 2 ways, so we need to provide the usage type as well
    No such thing is required in MIPS32, so this enum, `till now`,
    is useless for MIPS32
  */
  kUseAsAddress,  // Interpret X31 == SP and W32 == WSP.
  kUseAsValue  // Interpret X31 == XZR and W31 == WZR.
};

enum Action {
  kActionRead,
  kActionWrite,
  kActionReadWrite
};

// Immediate integer type.
enum ImmType {
  kUnsigned,
  kSigned
};

// Note: Order is significant; extracted bits may be casted to this type.
enum Extend : uint8_t {
  kExtendUXTB,  // 0b000
  kExtendUXTH,  // 0b001
  kExtendUXTW,  // 0b010
  kExtendUXTX,  // 0b011
  kExtendSXTB,  // 0b100
  kExtendSXTH,  // 0b101
  kExtendSXTW,  // 0b110
  kExtendSXTX  // 0b111
};

static uint64_t ExtractSizeInBits(Extend extend) {
  switch (extend) {
    case kExtendUXTB: return 8;
    case kExtendUXTH: return 16;
    case kExtendUXTW: return 32;
    case kExtendUXTX: return 64;
    case kExtendSXTB: return 8;
    case kExtendSXTH: return 16;
    case kExtendSXTW: return 32;
    case kExtendSXTX: return 64;
  }
  return 0;
}

static RegClass ExtendTypeToRegClass(Extend extend) {
  switch (extend) {
    case kExtendUXTB: return kRegW;
    case kExtendUXTH: return kRegW;
    case kExtendUXTW: return kRegW;
    case kExtendUXTX: return kRegX;
    case kExtendSXTB: return kRegW;
    case kExtendSXTH: return kRegW;
    case kExtendSXTW: return kRegW;
    case kExtendSXTX: return kRegX;
  }
}

static Operand::ShiftRegister::Extend ShiftRegExtendType(Extend extend) {
  switch (extend) {
    case kExtendUXTB:
    case kExtendUXTH:
    case kExtendUXTW:
    case kExtendUXTX:
      return Operand::ShiftRegister::kExtendUnsigned;
    case kExtendSXTB:
    case kExtendSXTH:
    case kExtendSXTW:
    case kExtendSXTX:
      return Operand::ShiftRegister::kExtendSigned;
  }
  return Operand::ShiftRegister::kExtendInvalid;
}

// Note: Order is significant; extracted bits may be casted to this type.
enum Shift : uint8_t {
  kShiftLSL,
  kShiftLSR,
  kShiftASR,
  kShiftROR
};

// Translate a shift encoding into an operand shift type used by the shift
// register class.
static Operand::ShiftRegister::Shift GetOperandShift(Shift s) {
  switch (s) {
    case kShiftLSL:
      return Operand::ShiftRegister::kShiftLeftWithZeroes;
    case kShiftLSR:
      return Operand::ShiftRegister::kShiftUnsignedRight;
    case kShiftASR:
      return Operand::ShiftRegister::kShiftSignedRight;
    case kShiftROR:
      return Operand::ShiftRegister::kShiftRightAround;
  }
  return Operand::ShiftRegister::kShiftInvalid;
}

// Get the name of an integer register.
static std::string RegNameXW(Action action, RegClass rclass, RegUsage rtype,
                             mipsel::RegNum number_) {
  auto number = static_cast<uint8_t>(number_);            
  CHECK_LE(number, 31U);

  std::stringstream ss;
  ss << "W" << static_cast<unsigned>(number);
  return ss.str();
}

// Get the name of a floating point register.
static std::string RegNameFP(Action action, RegClass rclass, RegUsage rtype,
                             mipsel::RegNum number_) {
  auto number = static_cast<uint8_t>(number_);
  CHECK_LE(number, 31U);

  std::stringstream ss;
  ss << (rclass == kRegS ? "F" : "D" );
  ss << static_cast<unsigned>(number);
  return ss.str();
}

static std::string RegNameDW(Action action, RegClass rclass, RegUsage rtype,
                             mipsel::RegNum number_) {
  auto number = static_cast<uint8_t>(number_);
  CHECK_LE(number, 31U); 

  std::stringstream ss;
  ss << "DW" << static_cast<unsigned>(number);
  return ss.str();
}


static std::string RegName(Action action, RegClass rclass, RegUsage rtype,
                           mipsel::RegNum number) {
  switch (rclass) {
    case kRegB:
    case kRegH:
    case kRegX:
    case kRegW:
      return RegNameXW(action, rclass, rtype, number); /* register is a GPR */
    case kRegS:
    case kRegD:
      return RegNameFP(action, rclass, rtype, number); /* register is a CP1 FPR */
    case kRegDW:
      return RegNameDW(action, rclass, rtype, number); /* copreocessor 2 (COP2) register*/  
  }
}


static uint32_t ReadRegSize(RegClass rclass) {
  switch (rclass) {
   case kRegX:
     return 64;
    case kRegW:
      return 32;
    case kRegB:
      return 8;
    case kRegH:
      return 16;
    case kRegS:
      return 32;
    case kRegD:
      return 64;
    case kRegDW:                  
      return 64;  
  }
  return 0;
}

static uint32_t WriteRegSize(RegClass rclass) {
  switch (rclass) {
    case kRegX:
    case kRegW:
    case kRegB:
    case kRegH:
    case kRegS:
      return 32;
    case kRegD:
      return 64;
    case kRegDW:            
      return 64;  
  }
  return 0;
}

// This gives us a register operand. If we have an operand like `<Xn|SP>`,
// then the usage is `kTypeUsage`, otherwise (i.e. `<Xn>`), the usage is
// a `kTypeValue`.
static Operand::Register Reg(Action action, RegClass rclass, RegUsage rtype,
                             mipsel::RegNum reg_num) {
                            
  Operand::Register reg;
  if (kActionWrite == action) {
    reg.name = RegName(action, rclass, rtype, reg_num);
    reg.size = WriteRegSize(rclass);              
  } 
  else if (kActionRead == action) {
    reg.name = RegName(action, rclass, rtype, reg_num);                 
    reg.size = ReadRegSize(rclass);                
  }
  else {
    LOG(FATAL)<< "Reg function only takes a simple read or write action.";
  }
  return reg;
  
}
                                                                       
static void AddRegOperand(Instruction &inst, Action action,
                          RegClass rclass, RegUsage rtype,
                          mipsel::RegNum reg_num) {
  Operand op;
  op.type = Operand::kTypeRegister;

  if (kActionWrite == action || kActionReadWrite == action) {                 
    op.reg = Reg(kActionWrite, rclass, rtype, reg_num);
    op.size = op.reg.size;
    op.action = Operand::kActionWrite;
    inst.operands.push_back(op);
  }

  if (kActionRead == action || kActionReadWrite == action) {               
    op.reg = Reg(kActionRead, rclass, rtype, reg_num);
    op.size = op.reg.size;
    op.action = Operand::kActionRead;
    inst.operands.push_back(op);
  }
}

static void AddShiftRegOperand(Instruction &inst, RegClass rclass,
                               RegUsage rtype, mipsel::RegNum reg_num,
                               Shift shift_type,
                               uint32_t shift_size) {
  if (!shift_size) {
    AddRegOperand(inst, kActionRead, rclass, rtype, reg_num);
  } else {
    Operand op;
    op.shift_reg.reg = Reg(kActionRead, rclass, rtype, reg_num);
    
    op.shift_reg.shift_op = GetOperandShift(shift_type);
    
    op.shift_reg.shift_size = shift_size;

    op.type = Operand::kTypeShiftRegister;
    op.size = op.shift_reg.reg.size;
    
    op.action = Operand::kActionRead;
    inst.operands.push_back(op);
  }
}

// Add an extend register operand, e.g. `(<Wm>|<Xm>){, <extend> {<amount>}}`.
//
// NOTE(pag): `rclass` is explicitly passed instead of inferred because some
//            instructions, e.g. `ADD_32_ADDSUB_EXT` specify `Wm` only.
static void AddExtendRegOperand(Instruction &inst, RegClass reg_class,
                                RegUsage rtype, mipsel::RegNum reg_num,
                                Extend extend_type, uint64_t output_size,
                                uint64_t shift_size=0) {
  Operand op;
  op.shift_reg.reg = Reg(kActionRead, reg_class, rtype, reg_num);
  op.shift_reg.extend_op = ShiftRegExtendType(extend_type);
  op.shift_reg.extract_size = ExtractSizeInBits(extend_type);

  // No extraction needs to be done, and zero extension already happens.
  if (Operand::ShiftRegister::kExtendUnsigned == op.shift_reg.extend_op &&
      op.shift_reg.extract_size == op.shift_reg.reg.size) {
    op.shift_reg.extend_op = Operand::ShiftRegister::kExtendInvalid;
    op.shift_reg.extract_size = 0;

  // Extracting a value that is wider than the register.
  } else if (op.shift_reg.extract_size > op.shift_reg.reg.size) {
    op.shift_reg.extend_op = Operand::ShiftRegister::kExtendInvalid;
    op.shift_reg.extract_size = 0;
  }

  if (shift_size) {
    op.shift_reg.shift_op = Operand::ShiftRegister::kShiftLeftWithZeroes;
    op.shift_reg.shift_size = shift_size;
  }

  op.type = Operand::kTypeShiftRegister;
  op.size = output_size;
  op.action = Operand::kActionRead;
  inst.operands.push_back(op);
}

static void AddImmOperand(Instruction &inst, uint32_t val,
                          ImmType signedness=kUnsigned,
                          unsigned size=32) {
  Operand op;
  op.type = Operand::kTypeImmediate;
  op.action = Operand::kActionRead;
  op.size = size;
  op.imm.is_signed = signedness == kUnsigned ? false : true;
  op.imm.val = val;
  inst.operands.push_back(op);
}

static void AddMonitorOperand(Instruction &inst) {
  Operand op;
  op.action = Operand::kActionWrite;
  op.reg.name = "MONITOR";
  op.reg.size = 32;
  op.size = 32;
  op.type = Operand::kTypeRegister;
  inst.operands.push_back(op);
}

static void AddPCRegOp(Instruction &inst, Operand::Action action, int32_t disp,
                       Operand::Address::Kind op_kind) {
  Operand op;
  op.type = Operand::kTypeAddress;
  op.size = 32;
  op.addr.address_size = 32;
  op.addr.base_reg.name = "PC";
  op.addr.base_reg.size = 32;
  op.addr.displacement = disp;
  op.addr.kind = op_kind;
  op.action = action;
  inst.operands.push_back(op);
}

// Emit a memory read or write operand of the form `[PC + disp]`.
static void AddPCRegMemOp(Instruction &inst, Action action, int64_t disp) {
  if (kActionRead == action) {
    AddPCRegOp(inst, Operand::kActionRead, disp, Operand::Address::kMemoryRead);
  } else if (kActionWrite == action) {
    AddPCRegOp(inst, Operand::kActionWrite, disp,
               Operand::Address::kMemoryWrite);
  } else {
    LOG(FATAL)<< __FUNCTION__ << " only accepts simple operand actions.";
  }
}

// Emit an address operand that computes `PC + disp`.
static void AddPCDisp(Instruction &inst, int32_t disp) {
  AddPCRegOp(inst, Operand::kActionRead, disp,
             Operand::Address::kAddressCalculation);
}

static void AddNextPC(Instruction &inst) {
  // add +4 as the PC displacement
  // emit an address computation operand
  AddPCDisp(inst, (kInstructionSize + 4) );
}
// We have something like this:
//    [<Xn|SP>, #<imm>]
//
// Which gets is:
//    addr = Xn + imm
//    ... deref addr and do stuff ...
static void AddBasePlusOffsetMemOp(Instruction &inst, Action action,
                                   uint32_t access_size,
                                   mipsel::RegNum base_reg, uint32_t disp) {
  Operand op;
  op.type = Operand::kTypeAddress;
  
  op.size = access_size;
  op.addr.address_size = 32;
  op.addr.base_reg = Reg(kActionRead, kRegW, kUseAsAddress, base_reg);
  op.addr.displacement = disp;

  if (kActionWrite == action || kActionReadWrite == action) {
    op.action = Operand::kActionWrite;
    op.addr.kind = Operand::Address::kMemoryWrite;
    inst.operands.push_back(op);
  }

  if (kActionRead == action || kActionReadWrite == action) {
    op.action = Operand::kActionRead;
    op.addr.kind = Operand::Address::kMemoryRead;
    inst.operands.push_back(op);
  }
}

static constexpr auto kInvalidReg = static_cast<mipsel::RegNum>(0xFF);

// Pre-index memory operands write back the result of the displaced address
// to the base register.
//
// We have something like this:
//    [<Xn|SP>, #<imm>]!
//
// Which gets us:
//    addr = Xn + imm
//    ... deref addr and do stuff ...
//    Xn = addr + imm
//
// So we add in two operands: one that is a register write operand for Xn,
// the other that is the value of (Xn + imm + imm).                            
static void AddPreIndexMemOp(Instruction &inst, Action action,
                             uint64_t access_size,
                             mipsel::RegNum base_reg, uint64_t disp,
                             mipsel::RegNum dest_reg1=kInvalidReg,
                             mipsel::RegNum dest_reg2=kInvalidReg) {
  AddBasePlusOffsetMemOp(inst, action, access_size, base_reg, disp);
  auto addr_op = inst.operands[inst.operands.size() - 1];

  Operand reg_op;
  reg_op.type = Operand::kTypeRegister;
  reg_op.action = Operand::kActionWrite;

  // We don't care about the case of `31` because then `base_reg` will be
  // `SP`, but `dest_reg1` or `dest_reg2` (if they are 31), will represent
  // one of `WZR` or `ZR`.
  if (static_cast<uint8_t>(base_reg) != 31 &&
      (dest_reg1 == base_reg || dest_reg2 == base_reg)) {
    reg_op.reg.name = "SUPPRESS_WRITEBACK";
    reg_op.reg.size = 32;
  } else {
    reg_op.reg = Reg(kActionWrite, kRegW, kUseAsAddress, base_reg);
  }

  reg_op.size = reg_op.reg.size;
  inst.operands.push_back(reg_op);

  addr_op.action = Operand::kActionRead;
  addr_op.addr.kind = Operand::Address::kAddressCalculation;
  inst.operands.push_back(addr_op);
}

// Post-index memory operands write back the result of the displaced address
// to the base register.
//
// We have something like this:
//    [<Xn|SP>], #<imm>
//
// Which gets us:
//    addr = Xn
//    ... deref addr and do stuff ...
//    Xn = addr + imm
//
// So we add in two operands: one that is a register write operand for Xn,
// the other that is the value of (Xn + imm).                               
static void AddPostIndexMemOp(Instruction &inst, Action action,
                              uint64_t access_size,
                              mipsel::RegNum base_reg, uint64_t disp,
                              mipsel::RegNum dest_reg1=kInvalidReg,
                              mipsel::RegNum dest_reg2=kInvalidReg) {
  AddBasePlusOffsetMemOp(inst, action, access_size, base_reg, 0);
  auto addr_op = inst.operands[inst.operands.size() - 1];

  Operand reg_op;
  reg_op.type = Operand::kTypeRegister;
  reg_op.action = Operand::kActionWrite;

  if (static_cast<uint8_t>(base_reg) != 31 &&
      (dest_reg1 == base_reg || dest_reg2 == base_reg)) {
    reg_op.reg.name = "SUPPRESS_WRITEBACK";
    reg_op.reg.size = 32;
  } else {
    reg_op.reg = Reg(kActionWrite, kRegW, kUseAsAddress, base_reg);
  }

  reg_op.size = reg_op.reg.size;
  inst.operands.push_back(reg_op);

  addr_op.size = 32;
  addr_op.action = Operand::kActionRead;
  addr_op.addr.kind = Operand::Address::kAddressCalculation;
  addr_op.addr.displacement = disp;
  inst.operands.push_back(addr_op);
}

// Post-index memory operands write back the result of the displaced address
// to the base register.
//
// We have something like this:
//    [<Xn|SP>], <Xm>
//
// Which gets us:
//    addr = Xn
//    ... deref addr and do stuff ...
//    Xn = addr + Xm
//
// So we add in two operands: one that is a register write operand for Xn,
// the other that is the value of (Xn + imm).                                    
static void AddPostIndexMemOp(Instruction &inst, Action action,
                              uint64_t access_size,
                              mipsel::RegNum base_reg,
                              mipsel::RegNum disp_reg,
                              mipsel::RegNum dest_reg1=kInvalidReg,
                              mipsel::RegNum dest_reg2=kInvalidReg) {
  AddBasePlusOffsetMemOp(inst, action, access_size, base_reg, 0);
  auto addr_op = inst.operands[inst.operands.size() - 1];

  Operand reg_op;
  reg_op.type = Operand::kTypeRegister;
  reg_op.action = Operand::kActionWrite;

  if (static_cast<uint8_t>(base_reg) != 31 &&
      (dest_reg1 == base_reg || dest_reg2 == base_reg)) {
    reg_op.reg.name = "SUPPRESS_WRITEBACK";
    reg_op.reg.size = 32;
  } else {
    reg_op.reg = Reg(kActionWrite, kRegW, kUseAsAddress, base_reg);
  }

  reg_op.size = reg_op.reg.size;
  inst.operands.push_back(reg_op);

  addr_op.size = 32;
  addr_op.action = Operand::kActionRead;
  addr_op.addr.kind = Operand::Address::kAddressCalculation;
  addr_op.addr.scale = 1;
  addr_op.addr.index_reg = Reg(kActionRead, kRegW, kUseAsAddress, disp_reg);
  inst.operands.push_back(addr_op);
}

static bool MostSignificantSetBit(uint64_t val, uint64_t *highest_out) {
#if __has_builtin(__builtin_clzll)
  if (val) {
    *highest_out = 63 - (__builtin_clzll(val) - (sizeof(unsigned long long) * 8 - 64));
    return true;
  } else {
    return false;
  }
#else
  auto found = false;
  for (uint64_t i = 0; i < 64; ++i) {
    if ((val >> i) & 1) {
      *highest_out = i;
      found = true;
    }
  }
  return found;
#endif
}

static bool LeastSignificantSetBit(uint64_t val, uint64_t *highest_out) {
#if __has_builtin(__builtin_ctzll)
  if (val) {
    *highest_out = __builtin_ctzll(val);
    return true;
  } else {
    return false;
  }
#else
  for (uint64_t i = 0; i < 64; ++i) {
    if ((val >> i) & 1) {
      *highest_out = i;
      return true;
    }
  }
  return false;
#endif  // __has_builtin(__builtin_ctzll)
}

static constexpr uint64_t kOne = static_cast<uint64_t>(1);

inline static uint64_t Ones(uint64_t val) {
  uint64_t out = 0;
  for (; val != 0; --val) {
    out <<= kOne;
    out |= kOne;
  }
  return out;
}

static uint64_t ROR(uint64_t val, uint64_t val_size, uint64_t rotate_amount) {
  for (uint64_t i = 0; i < rotate_amount; ++i) {
    val = ((val & kOne) << (val_size - kOne)) | (val >> kOne);
  }
  return val;
}

// Take a bit string `val` of length `val_size` bits, and concatenate it to
// itself until it occupies at least `goal_size` bits.
static uint64_t Replicate(uint64_t val, uint64_t val_size, uint64_t goal_size) {
  uint64_t replicated_val = 0;
  for (uint64_t i = 0; i < goal_size; i += val_size) {
    replicated_val = (replicated_val << val_size) | val;
  }
  return replicated_val;
}

// Decode bitfield and logical immediate masks. There is a nice piece of code
// here for producing all valid (64-bit) inputs:
//
//      https://stackoverflow.com/a/33265035/247591
//
// The gist of the format is that you have
static bool DecodeBitMasks(uint64_t N /* one bit */,
                           uint64_t imms /* six bits */,
                           uint64_t immr /* six bits */,
                           bool is_immediate,
                           uint64_t data_size,
                           uint64_t *wmask_out=nullptr,
                           uint64_t *tmask_out=nullptr) {
  uint64_t len = 0;
  if (!MostSignificantSetBit((N << 6ULL) | (~imms & 0x3fULL), &len)) {
    return false;
  }
  if (len < 1) {
    return false;
  }

  const uint64_t esize = kOne << len;
  if (esize > data_size) {
    return false;  // `len == 0` is a `ReservedValue()`.
  }

  const uint64_t levels = Ones(len);  // ZeroExtend(Ones(len), 6).
  const uint64_t R = immr & levels;
  const uint64_t S = imms & levels;

  if (is_immediate && S == levels) {
    return false;  // ReservedValue.
  }

  const uint64_t diff = (S - R) & static_cast<uint64_t>(0x3F);  // 6-bit sbb.
  const uint64_t d = diff & levels;  // `diff<len-1:0>`.
  const uint64_t welem = Ones(S + kOne);
  const uint64_t telem = Ones(d + kOne);
  const uint64_t wmask = Replicate(
      ROR(welem, esize, R), esize, data_size);
  const uint64_t tmask = Replicate(telem, esize, data_size);

  if (wmask_out) {
    *wmask_out = wmask;
  }

  if (tmask_out) {
    *tmask_out = tmask;
  }
  return true;
}
// Utility function for extracting [From, To] bits from a uint32_t.
static inline uint64_t Extract(uint64_t bits, unsigned from, unsigned to) {
  CHECK(from < 64 && to < 64 && from >= to);
  return (bits >> to) & ((1 << (from - to + 1)) - 1);
}

static uint64_t VFPExpandImmToFloat32(uint64_t imm) {
  uint64_t result = 0;
  uint64_t bit6 = Extract(imm, 6, 6);
  result |= Extract(imm, 7, 7) << 31;
  result |= Extract(imm, 5, 0) << 19;
  result |= bit6 ? (0x1FULL << 25) : (0x1ULL << 30);
  return result;
}

static uint64_t VFPExpandImmToFloat64(uint64_t imm) {
  uint64_t result = 0;
  uint64_t bit6 = Extract(imm, 6, 6);
  result |= Extract(imm, 7, 7) << 63;
  result |= Extract(imm, 5, 0) << 48;
  result |= bit6 ? (0xFFULL << 54) : (0x1ULL << 62);
  return result;
}

bool MIPSELArch::DecodeInstruction(  
    uint64_t address, const std::string &inst_bytes,
    Instruction &inst) const {
       
  mipsel::InstData dinst = {};                                                
  auto bytes = reinterpret_cast<const uint8_t *>(inst_bytes.data());
  inst.arch_name = arch_name;
  inst.pc = address;
  inst.next_pc = address + kInstructionSize;
  inst.category = Instruction::kCategoryInvalid;
                                        

  if (0 != (address % kInstructionSize)) {
    inst.category = Instruction::kCategoryInvalid;
    return false;
  } 
  else if (!mipsel::TryExtractMips(bytes, dinst)) {                           
    inst.category = Instruction::kCategoryInvalid;
    return false;
  }

  inst.bytes = inst_bytes.substr(0, kInstructionSize);
  inst.category = InstCategory(dinst);
  inst.function = mipsel::InstFormToString(dinst.iform);                         

  if (!mipsel::TryDecodeMips(dinst, inst)) {                                     
    inst.category = Instruction::kCategoryInvalid;
    return false;
  }
  return true;
}

}  // namespace


static void DecodeFallThroughPC(Instruction &inst) {
  Operand not_taken_op = {};
  not_taken_op.action = Operand::kActionRead;
  not_taken_op.type = Operand::kTypeAddress;
  not_taken_op.size = kPCWidth;
  not_taken_op.addr.address_size = kPCWidth;
  not_taken_op.addr.base_reg.name = "PC";
  not_taken_op.addr.base_reg.size = kPCWidth;
  not_taken_op.addr.displacement = kInstructionSize + 4;
  not_taken_op.addr.kind = Operand::Address::kControlFlowTarget;
  inst.operands.push_back(not_taken_op);

  inst.branch_not_taken_pc = inst.next_pc + 4; 
}

static void DecodeFallThroughPCLikely(Instruction &inst) {
  Operand not_taken_op = {};
  not_taken_op.action = Operand::kActionRead;
  not_taken_op.type = Operand::kTypeAddress;
  not_taken_op.size = kPCWidth;
  not_taken_op.addr.address_size = kPCWidth;
  not_taken_op.addr.base_reg.name = "PC";
  not_taken_op.addr.base_reg.size = kPCWidth;
  not_taken_op.addr.displacement = kInstructionSize + 4;
  not_taken_op.addr.kind = Operand::Address::kControlFlowTarget;
  inst.operands.push_back(not_taken_op);

  inst.branch_not_taken_pc = inst.next_pc; 
}

// Decode a relative branch target.
static void DecodeConditionalBranch(Instruction &inst, int32_t disp) {       

  // Condition variable
  Operand cond_op = {};
  cond_op.action = Operand::kActionWrite;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);

  // Taken branch.
  Operand taken_op = {};
  taken_op.action = Operand::kActionRead;
  taken_op.type = Operand::kTypeAddress;
  taken_op.size = kPCWidth;
  taken_op.addr.address_size = kPCWidth;
  taken_op.addr.base_reg.name = "PC";
  taken_op.addr.base_reg.size = kPCWidth;
  taken_op.addr.displacement = disp;
  taken_op.addr.kind = Operand::Address::kControlFlowTarget;
  inst.operands.push_back(taken_op);

  inst.branch_taken_pc = disp;    

  DecodeFallThroughPC(inst);
}

// Decode a relative branch target.
static void DecodeConditionalBranchLikely(Instruction &inst, int32_t disp) {

  // Condition variable
  Operand cond_op = {};
  cond_op.action = Operand::kActionWrite;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);

  // Taken branch.
  Operand taken_op = {};
  taken_op.action = Operand::kActionRead;
  taken_op.type = Operand::kTypeAddress;
  taken_op.size = kPCWidth;
  taken_op.addr.address_size = kPCWidth;
  taken_op.addr.base_reg.name = "PC";
  taken_op.addr.base_reg.size = kPCWidth;
  taken_op.addr.displacement = disp;
  taken_op.addr.kind = Operand::Address::kControlFlowTarget;
  inst.operands.push_back(taken_op);
  inst.branch_taken_pc = disp;    

  DecodeFallThroughPCLikely(inst);
}

static bool DecodeBranchRegLabel(const mipsel::InstData &data, Instruction &inst,
                                 RegClass reg_class) { 
                                                                                                
  DecodeConditionalBranch(inst,(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2)) );
  AddRegOperand(inst, kActionRead, reg_class, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, reg_class, kUseAsValue, data.rt);      
  
  return true;
}

static bool DecodeBranchLikely(const mipsel::InstData &data, Instruction &inst,
                                 RegClass reg_class) { 
                                                                                                
  DecodeConditionalBranchLikely(inst,(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2)) );
  AddRegOperand(inst, kActionRead, reg_class, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, reg_class, kUseAsValue, data.rt);      
  
  return true;
}

static const char*CondName(uint8_t cond) {
  if (cond == 16)
  {
    return "S";
  }
  return "D";
}

namespace mipsel {                                                       
//namespace {

static void SetConditionalFunctionName(const InstData &data, Instruction &inst) {
  uint8_t cond = 0;
  cond = data.fmt;

  std::stringstream ss;
  ss << inst.function << "_" << CondName(cond);
  inst.function = ss.str();
}

// ADD  <rd>,<rs>,<rt>                                                  
bool TryDecodeADD_word(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}


// ADDU  <rd>,<rs>,<rt>                                                  
bool TryDecodeADDU_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// ADDU  <rz>,<rx>,<ry>                                                  
bool TryDecodeADDU_16e_REG(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rz);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.ry);
  return true;
}

// ADDI  <rt>, <rs>,<immediate>                                                     
bool TryDecodeADDI_32_IMM(const InstData &data, Instruction &inst) {
  auto imm = data.SIGNED_IMM;                                   
  auto reg_val = data.rs;
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  if (inst.has_ref_info)
  {
    reg_val = 0;
  }
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, reg_val);
  AddImmOperand(inst, imm,kSigned,16);
  return true;
}


// ADDIU  <rt>, <rs>,<immediate>                                                     
bool TryDecodeADDIU_32_IMM(const InstData &data, Instruction &inst) {
  auto imm = data.SIGNED_IMM;                                   
  auto reg_val = data.rs;
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  if (inst.has_ref_info)
  {
    reg_val = 0;
  }
    AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, reg_val);
  AddImmOperand(inst, imm,kSigned,16);
  return true;
}

// ADDIU  <rx>, ,<immediate>                                                     
bool TryDecodeADDIU_16e_IMM(const InstData &data, Instruction &inst) {
  auto imm = data.SIGNED_16e_IMM;                                   
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rx);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);
  AddImmOperand(inst, imm,kSigned,8);
  return true;
}

// ADDIU  <rx>, ,<immediate>                                                     
bool TryDecodeADDIU_16e_IMM_SPRelative(const InstData &data, Instruction &inst) {
  auto imm = (data.SIGNED_16e_IMM << 2 );
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rx);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);
  AddImmOperand(inst, imm,kSigned,8);
  return true;
}


// ADDIU  <ry>, <rx>, ,<immediate>                                                     
bool TryDecodeADDIU_16e_IMM_Word(const InstData &data, Instruction &inst) {
  auto imm = data.SIGNED_16e_IMM;                                   
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.ry);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);
  AddImmOperand(inst, imm,kSigned,4);
  return true;
}

// B  <offset>                                                     
bool TryDecodeB_16e(const InstData &data, Instruction &inst) {
  AddImmOperand(inst, data.offset,kSigned,16);                        
  return true; 
}


// B  <offset> extended                                                     
bool TryDecodeB_16e_ext(const InstData &data, Instruction &inst) {
  AddImmOperand(inst, data.offset,kSigned,16);
  AddImmOperand(inst, data.offset_2,kSigned,16);
  AddImmOperand(inst, data.offset_1,kSigned,16);                      
  return true; 
}

// BEQZ <rx>  <offset>                                                     
bool TryDecodeBEQZ_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);  
  AddImmOperand(inst, data.offset,kSigned,16); 
  return true; 
}


// BEQZ <rx> <offset> extended                                                     
bool TryDecodeBEQZ_16e_ext(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx); 
  AddImmOperand(inst, data.offset,kSigned,16);
  AddImmOperand(inst, data.offset_2,kSigned,16);
  AddImmOperand(inst, data.offset_1,kSigned,16);                      
  return true; 
}

// BNEZ <rx>  <offset>                                                     
bool TryDecodeBNEZ_16e(const InstData &data, Instruction &inst) { 
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);  
  AddImmOperand(inst, data.offset,kSigned,16); 
  return true; 
}


// BNEZ <rx> <offset> extended                                                     
bool TryDecodeBNEZ_16e_ext(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx); 
  AddImmOperand(inst, data.offset,kSigned,16);
  AddImmOperand(inst, data.offset_2,kSigned,16);
  AddImmOperand(inst, data.offset_1,kSigned,16);                      
  return true; 
}


// BREAK  <offset> 
bool TryDecodeBREAK_16e(const InstData &data, Instruction &inst) { 
  AddImmOperand(inst, data.offset,kSigned,16);                      
  return true; 
}

// BTEQZ  <offset>                                                     
bool TryDecodeBTEQZ_16e(const InstData &data, Instruction &inst) {
  AddImmOperand(inst, data.offset,kSigned,16); 
  return true; 
}


// BTEQZ <offset> extended                                                     
bool TryDecodeBTEQZ_16e_ext(const InstData &data, Instruction &inst) {
  AddImmOperand(inst, data.offset,kSigned,16);
  AddImmOperand(inst, data.offset_2,kSigned,16);
  AddImmOperand(inst, data.offset_1,kSigned,16);                      
  return true; 
}

// BTNEZ  <offset>                                                     
bool TryDecodeBTNEZ_16e(const InstData &data, Instruction &inst) {
  AddImmOperand(inst, data.offset,kSigned,16); 
  return true; 
}


// BTNEZ <rx> <offset> extended                                                     
bool TryDecodeBTNEZ_16e_ext(const InstData &data, Instruction &inst) {
  AddImmOperand(inst, data.offset,kSigned,16);
  AddImmOperand(inst, data.offset_2,kSigned,16);
  AddImmOperand(inst, data.offset_1,kSigned,16);                      
  return true; 
}

// CMP rx, ry
bool TryDecodeCMP_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.ry);  
  return true; 
}
// CMPI rx, IMM
bool TryDecodeCMPI_16e_imm(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);  
  AddImmOperand(inst, data.offset,kSigned,16); 
  return true; 
}

// CMPI rx, IMM extended
bool TryDecodeCMPI_16e_imm_ext(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);  
  AddImmOperand(inst, data.offset,kSigned,16); 
  return true; 
}

// // DIV_16e rx, ry                                                  
bool TryDecodeDIV_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx); 
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.ry); 
  
  return true; 
}

// DIVU_16e rx, ry                                                    
bool TryDecodeDIVU_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);  
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.ry);  
  return true; 
}


// MFHI_16e rd
bool TryDecodeMFHI_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  return true;
}

// MFLO_16e rd
bool TryDecodeMFLO_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  return true;
}


// MOVE_16e r32, rz
bool TryDecodeMOVE_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rz);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  return true;
}

//MOVE_16e ry, r32
bool TryDecodeMOVE_16e_r32(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rz);
  return true;
}

// MULT_16e rx, ry
bool TryDecodeMULT_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  return true;
}

// MULTU_16e rx, ry
bool TryDecodeMULTU_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  return true;
}

// NEG_16e rx, ry
bool TryDecodeNEG_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  return true;
}


// NOT_16e rx, ry
bool TryDecodeNOT_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  return true;
}

// OR_16e rx, ry
bool TryDecodeOR_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  return true;
}

// RESTORE_16e rx, ry
bool TryDecodeRESTORE_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  return true;
}

// RESTORE_16e_ext rx, ry
bool TryDecodeRESTORE_16e_ext(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  return true;
}

// SAVE_16e rx, ry
bool TryDecodeSAVE_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  return true;
}

// SAVE_16e_ext rx, ry
bool TryDecodeSAVE_16e_ext(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  return true;
}

// SB_16e rx, ry
bool TryDecodeSB_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  AddImmOperand(inst, data.offset_1,kSigned,16);
  return true;
}

// SB_16e_ext rx, ry
bool TryDecodeSB_16e_ext(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  AddImmOperand(inst, data.offset_1,kSigned,16);
  return true;
}

// SDBBP_16e rx, ry
bool TryDecodeSDBBP_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.ry);
  AddImmOperand(inst, data.code,kSigned,16);
  return true;
}

// SEB_16e rx, ry
bool TryDecodeSEB_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rx);
  return true;
}

// CFC2  <rt>, <impl>                                                     
bool TryDecodeCFC2(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  return true;
}

// CTC2  <rt>, <impl>                                                     
bool TryDecodeCTC2(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// MFC2  <rt>, <impl>                                                     
bool TryDecodeMFC2(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  return true;
}

// MFHC2  <rt>, <impl>                                                     
bool TryDecodeMFHC2(const InstData &data, Instruction &inst) {
  auto impl = data.impl;        
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegDW, kUseAsAddress, data.impl);
  return true;
}

// MTC2  <rt>, <impl>                                                     
bool TryDecodeMTC2(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// MTHC2  <rt>, <impl>                                                     
bool TryDecodeMTHC2(const InstData &data, Instruction &inst) {
  auto impl = data.impl; 
  AddRegOperand(inst, kActionWrite, kRegDW, kUseAsAddress, data.impl);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// ADD.FMT  <fd>, <fs>,<ft>                                                     
bool TryDecodeADD_FMT(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegD;
  std::stringstream ss;
  if (data.fmt == 16)
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else {
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  }

  // SetConditionalFunctionName(data, inst);
  AddRegOperand(inst, kActionWrite, rclass, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.rt);
  return true;
}

// DIV rs, rt
bool TryDecodeDIV_32(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  return true;
}

// DIV_fmt rd, rs, rt
bool TryDecodeDIV_fmt(const InstData &data, Instruction &inst) {
  auto rclass = kRegS;
  std::stringstream ss; 
  if (data.fmt == 16){
    ss << inst.function << "_S";
    inst.function = ss.str(); 
  }else {
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  }
  AddRegOperand(inst, kActionWrite, rclass, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.rt);
  return true;
}


// DIVU rs, rt
bool TryDecodeDIVU(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  return true;
}

// MUL rd, rs, rt
bool TryDecodeMUL_32(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  return true;
}

// MUL_fmt rd, rs, rt
bool TryDecodeMUL_fmt(const InstData &data, Instruction &inst) {
  auto rclass = kRegS;
  std::stringstream ss; 
  if (data.fmt == 16){
    ss << inst.function << "_S";
    inst.function = ss.str(); 
  }else {
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  }
  AddRegOperand(inst, kActionWrite, rclass, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.rt);
  return true;
}

// MULT rs, rt
bool TryDecodeMULT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  return true;
}


// MULTU rs, rt
bool TryDecodeMULTU(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  return true;
}

// MADD rs, rt
bool TryDecodeMADD(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  return true;
}


// Mov_s rd, rs
bool TryDecodeMOV_fmt(const InstData &data, Instruction &inst) {
  auto rclass = kRegS;
  std::stringstream ss; 
  if (data.fmt == 16){
    ss << inst.function << "_S";
    inst.function = ss.str(); 
  }else {
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  }
  AddRegOperand(inst, kActionWrite, rclass, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.rs);
  return true;
}

// Movz rd, rs, rt
bool TryDecodeMOVZ(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  return true;
}

// Movn rd, rs, rt
bool TryDecodeMOVN(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  return true;
}

// ROTRV rd, rt, rs
bool TryDecodeROTRV(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  return true;
}


// Movf rd, rs, cc
bool TryDecodeMOVF(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  return true;
}

// Movt rd, rs, cc
bool TryDecodeMOVT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  return true;
}



// MFHI rd
bool TryDecodeMFHI(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  return true;
}

// MFLO rd
bool TryDecodeMFLO(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  return true;
}

// MTLO rs
bool TryDecodeMTLO(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  return true;
}

// TEQ rs, rt
bool TryDecodeTEQ_32(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);    
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  
  return true;
}

// LW  <rt>, <base>                                                  
bool TryDecodeLW_32_IMM(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  auto offset = static_cast<signed>(data.offset);
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));
  return true;
}

// LWL  <rt>, offset<base>                                                  
bool TryDecodeLWL_LOAD_WORD_LEFT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  auto offset = static_cast<signed>(data.offset);
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));
  return true;
}

// LWR  <rt>, offset<base>                                                  
bool TryDecodeLWR_LOAD_WORD_RIGHT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  auto offset = static_cast<signed>(data.offset);
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));
  return true;
}

// LL  <rt>, offset<base>                                                  
bool TryDecodeLL_LOAD_LINKED_WORD(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  auto offset = static_cast<signed>(data.offset);
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));
  return true;
}

// // LWC1 <ft>, offset(base)
bool TryDecodeLWC1_FLOATING_POINT(const InstData &data, Instruction &inst){
AddRegOperand(inst, kActionWrite, kRegS, kUseAsValue, data.ft);
AddBasePlusOffsetMemOp(inst,kActionRead, 32, data.base, static_cast<signed>(data.offset));
return true;
}

// LDC1  <ft>, offset(base)
bool TryDecodeLDC1_FLOATING_R(const InstData &data, Instruction &inst) {   
  AddRegOperand(inst, kActionWrite, kRegD, kUseAsValue, data.rt);   
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));                          
  return true; 
}

// LDC2  <rt>, offset(base)
bool TryDecodeLDC2(const InstData &data, Instruction &inst) {   
  AddRegOperand(inst, kActionWrite, kRegDW, kUseAsValue, data.rt);   
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));                          
  return true; 
}

// LWC2  <rt>, offset(base)
bool TryDecodeLWC2(const InstData &data, Instruction &inst) {   
  AddRegOperand(inst, kActionWrite, kRegDW, kUseAsValue, data.rt);   
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));                          
  return true; 
}

// SDC2  <rt>, offset(base)
bool TryDecodeSDC2(const InstData &data, Instruction &inst) {   
  AddBasePlusOffsetMemOp(inst, kActionWrite, 32, data.base,static_cast<signed>(data.offset)); 
  AddRegOperand(inst, kActionRead, kRegDW, kUseAsValue, data.rt);   
  return true; 
}

// SWC2  <rt>, offset(base)
bool TryDecodeSWC2(const InstData &data, Instruction &inst) {   
  AddBasePlusOffsetMemOp(inst, kActionWrite, 32, data.base,static_cast<signed>(data.offset)); 
  AddRegOperand(inst, kActionRead, kRegDW, kUseAsValue, data.rt);   
  return true; 
}

// LUI  <rt>, <Immediate>                                                   
bool TryDecodeLUI_IMM(const InstData &data, Instruction &inst) {
  auto imm =(data.IMM << 16) ;
  uint16_t lsb = 0;
  uint32_t result = (imm ) | (lsb & 0xFFFF);

  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  AddImmOperand(inst, result, kSigned, 32);
  return true;
}

// SLTIU  <rt>, <rs> , <Immediate>                                                  
bool TryDecodeSLTIU_IMM(const  InstData &data, Instruction &inst) {
  auto imm = data.SIGNED_IMM;       
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddImmOperand(inst, imm,kUnsigned,16);
  return true;
}

// SLTI  <rt>, <rs> , <Immediate>                                                    
bool TryDecodeSLTI_IMM(const InstData &data, Instruction &inst) {
  auto imm = data.SIGNED_IMM;      
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddImmOperand(inst, imm, kSigned,16);
  return true;
}

// SLT rd, rs , rt
bool TryDecodeSLT_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  
  return true;
}

// SLTU rd, rs , rt
bool TryDecodeSLTU_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  
  return true;
}

// EXT <rt>, <rs>, pos, size
bool TryDecodeEXT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddImmOperand(inst, data.lsb, kUnsigned, 32);
  AddImmOperand(inst, data.msbd , kUnsigned, 32);

  return true;
}

// SUB.FMT  <fd>, <fs>,<ft>                                                     
bool TryDecodeSUB_FMT(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegD;
  std::stringstream ss;
  if (data.fmt == 16)
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else {
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  }

  // SetConditionalFunctionName(data, inst);
  AddRegOperand(inst, kActionWrite, rclass, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.rt);
  return true;
}

// SUBU rd, rs , rt
bool TryDecodeSUBU_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// SUB rd, rs , rt
bool TryDecodeSUB_WORD(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

//SW <rt>,<offset(base)>
bool TryDecodeSW_32_IMM(const InstData &data, Instruction &inst) {
  AddBasePlusOffsetMemOp(inst, kActionWrite, 32, data.base, static_cast<signed>(data.offset));
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

//SC <rt>,<offset(base)>
bool TryDecodeSC_STORE_CONDITIONAL_WORD(const InstData &data, Instruction &inst) {
  AddBasePlusOffsetMemOp(inst, kActionWrite, 32, data.base, static_cast<signed>(data.offset));
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

//SWL <rt>,<offset(base)>
bool TryDecodeSWL(const InstData &data, Instruction &inst) {
  AddBasePlusOffsetMemOp(inst, kActionWrite, 32, data.base, static_cast<signed>(data.offset));
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}


// // SWC1 <ft> offset(base)
bool TryDecodeSWC1_FLOATING_POINT(const InstData &data, Instruction &inst) {
  AddBasePlusOffsetMemOp(inst, kActionWrite, 32, data.base, static_cast<signed>(data.offset));
  AddRegOperand(inst, kActionRead, kRegS, kUseAsAddress, data.rt);
  return true;
}

// //SDC1 <ft>,<offset(base)>
bool TryDecodeSDC1_FLOATING_R(const InstData &data, Instruction &inst) {
  AddBasePlusOffsetMemOp(inst, kActionWrite, 32, data.base, static_cast<signed>(data.offset));
  AddRegOperand(inst, kActionRead, kRegD, kUseAsAddress, data.rt);
  return true;
}

//SH <rt>,<offset(base)>
bool TryDecodeSH_16_MEM(const InstData &data, Instruction &inst) {
  AddBasePlusOffsetMemOp(inst, kActionWrite, 16, data.base, static_cast<signed>(data.offset));
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// SEH  <rd>, <rt>
bool TryDecodeSEH(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  
  return true;
}

//LH <rt>,<offset(base)>
bool TryDecodeLH(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base, static_cast<signed>(data.offset));
  return true;
}

//LBU <rt> ,<offset(base)>
bool TryDecodeLBU_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));
  return true;
}

//LB <rt> ,<offset(base)>
bool TryDecodeLB_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddBasePlusOffsetMemOp(inst, kActionRead, 32, data.base,static_cast<signed>(data.offset));
  return true;
}

//LHU <rt> ,<offset(base)>
bool TryDecodeLHU_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddBasePlusOffsetMemOp(inst, kActionRead, 16, data.base,static_cast<signed>(data.offset));
  return true;
}

// NOP
bool TryDecodeNOP(const InstData &data, Instruction &inst) {
  return true;
}

// OR rd, rs , rt
bool TryDecodeOR_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// ORI rt, rs , immediate
bool TryDecodeORI_32_IMM(const InstData &data, Instruction &inst) {
  auto imm = data.IMM;        
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddImmOperand(inst, imm, kSigned,16);
  return true;
}

// AND rd, rs , rt
bool TryDecodeAND_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// AND rx, ry 
bool TryDecodeAND_16e(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rx);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rx);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.ry);
  return true;
}

// ANDI rt, rs , immediate
bool TryDecodeANDI_32_IMM(const InstData &data, Instruction &inst) {
  auto imm = data.IMM;      
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddImmOperand(inst, imm, kSigned,16);
  return true;
}

// NOR rd, rs , rt
bool TryDecodeNOR_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// XOR rd, rs , rt
bool TryDecodeXOR_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// XORI rt, rs , immediate 
bool TryDecodeXORI_32_IMM(const InstData &data, Instruction &inst) {
  auto imm = data.IMM;      
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddImmOperand(inst, imm, kSigned,16);
  return true;
}

// SRA rd, rt , sa
bool TryDecodeSRA_RIGHT_SHIFT(const InstData &data, Instruction &inst) {
  auto shift_type = static_cast<Shift>(2);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  AddShiftRegOperand(inst, kRegW, kUseAsAddress, data.rt,
                     shift_type, data._sa);
  return true;
}

// ROTR rd, rt , sa
bool TryDecodeROTR(const InstData &data, Instruction &inst) {
  auto shift_type = static_cast<Shift>(3);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  AddShiftRegOperand(inst, kRegW, kUseAsAddress, data.rt,
                     shift_type, data._sa);
  return true;
}

// CFC1 rt , fs
bool TryDecodeCFC1_FLOATING_POINT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegS, kUseAsAddress, data.fs);
  return true;
}

// CTC1 rt , fs
bool TryDecodeCTC1(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionWrite, kRegS, kUseAsAddress, data.fs);
  return true;
}

// SRAV rd, rt, rs
bool TryDecodeSRAV_RIGHT_ARITHMETIC_VARIABLE(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  return true;
}

// SRL rd, rt , sa
bool TryDecodeSRL_RIGHT_SHIFT_LOGICAL(const InstData &data, Instruction &inst) {
  auto shift_type = static_cast<Shift>(1);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  AddShiftRegOperand(inst, kRegW, kUseAsAddress, data.rt, shift_type, data._sa);
  return true;
}

// SLL rd, rt , sa
bool TryDecodeSLL_LEFT_SHIFT_LOGICAL(const  InstData &data, Instruction &inst) {
  auto shift_type = static_cast<Shift>(0);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  AddShiftRegOperand(inst, kRegW, kUseAsAddress, data.rt,shift_type, data._sa);
  return true;
}

// SLLV rd, rt , rs
bool TryDecodeSLLV_LEFT_LOGICAL_VARIABLE(const  InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  return true;
}

// SRLV rd, rt , rs
bool TryDecodeSRLV_RIGHT_LOGICAL_VARIABLE(const  InstData &data, Instruction &inst) {
  auto shift_type = static_cast<Shift>(1);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  return true;
}

// BNE rt, rs , offset
bool TryDecodeBNE_BRANCH_NOT_EQUAL(const  InstData &data, Instruction &inst) {
  DecodeBranchRegLabel(data, inst, kRegW);
  return true ;
}

// BNEL rs, rt , offset
bool TryDecodeBNEL_BRANCH_LIKELY(const  InstData &data, Instruction &inst) {
int32_t disp = (data.offset << 2);
  Operand cond_op = {};
  cond_op.action = Operand::kActionWrite;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);

  Operand br_taken_op = {};
  br_taken_op.action = Operand::kActionRead;
  br_taken_op.type = Operand::kTypeAddress;
  br_taken_op.size = kPCWidth; // 32 bits
  br_taken_op.addr.address_size = kPCWidth; 
  br_taken_op.addr.base_reg.name = "PC";
  br_taken_op.addr.base_reg.size = kPCWidth;
  br_taken_op.addr.displacement = disp;
  br_taken_op.addr.kind = Operand::Address::kControlFlowTarget;
  inst.operands.push_back(br_taken_op);

  inst.branch_taken_pc = static_cast<int32_t>(static_cast<uint32_t>(inst.next_pc) + disp);

  Operand not_taken_op = {};
  not_taken_op.action = Operand::kActionRead;
  not_taken_op.type = Operand::kTypeAddress;
  not_taken_op.size = kPCWidth; // 32 bits
  not_taken_op.addr.address_size = kPCWidth; 
  not_taken_op.addr.base_reg.name = "PC";
  not_taken_op.addr.base_reg.size = kPCWidth;
  not_taken_op.addr.displacement = 8;
  not_taken_op.addr.kind = Operand::Address::kControlFlowTarget;
  inst.operands.push_back(not_taken_op);

  inst.branch_not_taken_pc = static_cast<int32_t>(static_cast<uint32_t>(inst.next_pc)); 

  //register operands
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true ;
}


// BEQ rs, rt , offset
bool TryDecodeBEQ_BRANCH_EQUAL(const  InstData &data, Instruction &inst) {
  DecodeBranchRegLabel(data, inst, kRegW);
  return true ;
}

// BEQL rs, rt , offset
bool TryDecodeBEQL_BRANCH_LIKELY(const  InstData &data, Instruction &inst) {
  DecodeBranchLikely(data, inst, kRegW);
  return true ;
}

// BGEZ rs, offset
bool TryDecodeBGEZ(const  InstData &data, Instruction &inst) {
  DecodeConditionalBranch(inst,(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2)) );
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);    
  return true;
}

// BGEZL rs, offset
bool TryDecodeBGEZL_BRANCH_LIKELY(const  InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);    
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address );                                                              
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.next_pc;      
  Operand cond_op = {};
  cond_op.action = Operand::kActionWrite;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;                                                         
  cond_op.size = 8;                                                             
  inst.operands.push_back(cond_op);

  return true;
}

// BGTZL rs, offset
bool TryDecodeBGTZL_BRANCH_LIKELY(const  InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);    
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address );                                                              
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.next_pc;      

  Operand cond_op = {};
  cond_op.action = Operand::kActionWrite;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;                                                         
  cond_op.size = 8;                                                             
  inst.operands.push_back(cond_op);

  return true;
}
  
// BLEZ rs, offset
bool TryDecodeBLEZ(const  InstData &data, Instruction &inst) {
  DecodeConditionalBranch(inst,(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2)) );
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  return true;
}

// BLTZ rs, offset
bool TryDecodeBLTZ(const  InstData &data, Instruction &inst) {
  DecodeConditionalBranch(inst,(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2)) );
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  return true;
}

// BGTZ rs, offset
bool TryDecodeBGTZ_32_IMM_COND(const  InstData &data, Instruction &inst) {
  DecodeConditionalBranch(inst,(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2)) );
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  return true;
}


// BLTZL rs, offset
bool TryDecodeBLTZL_BRANCH_LIKELY(const  InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);    
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address );                                                              
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.next_pc;      
  Operand cond_op = {};
  cond_op.action = Operand::kActionWrite;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;                                                         
  cond_op.size = 8;                                                             
  inst.operands.push_back(cond_op);

  return true;
}

// JR  rs
bool TryDecodeJR_32_R(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  return true;
}

// JR.HB  rs
bool TryDecodeJR_HB(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  return true;
}

// J  target
bool TryDecodeJ_Target(const InstData &data, Instruction &inst) {
  auto target_address_right = (inst.next_pc >> 28);
  auto target_address_left = (target_address_right << 28);    
  auto target_address =static_cast<uint32_t>(
      static_cast<uint32_t>(target_address_left ) | (data.instr_index << 2));     
  AddPCDisp(inst, target_address );
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<uint32_t>(target_address_left ) | (data.instr_index << 2));;
  return true;
}

// JAL  target
bool TryDecodeJAL_Target(const InstData &data, Instruction &inst) {
  auto target_address_right = (inst.next_pc >> 28);
  auto target_address_left = (target_address_right << 28);    
  auto target_address =static_cast<uint32_t>(
      static_cast<uint32_t>(target_address_left ) | (data.instr_index << 2));
   
  AddPCDisp(inst, target_address);
  AddNextPC(inst);     
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<uint32_t>(target_address_left ) | (data.instr_index << 2));;
  return true;
}

// JALR  rd , rs
bool TryDecodeJALR_32_R(const InstData &data, Instruction &inst) {
  AddNextPC(inst);                                       
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs); 
  return true;  
}

// JALR.HB  rd , rs
bool TryDecodeJALR_HB(const InstData &data, Instruction &inst) {
  AddNextPC(inst);                                           
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  return true;  
}

// B offset
bool TryDecodeB_32_offset(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.offset);
  return true;
}

// BAL offset
bool TryDecodeBAL_BRANCH_LINK(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.offset);
  return true;
}

// BGEZAL  rs,offset
bool TryDecodeBGEZAL_LINK(const InstData &data, Instruction &inst) {
  int32_t disp = (data.offset << 2);
  // Condition Variable
  Operand cond_op = {};
  cond_op.action = Operand::kActionWrite;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);

  // taken pc
  Operand br_taken_op = {};
  br_taken_op.action = Operand::kActionRead;
  br_taken_op.type = Operand::kTypeAddress;
  br_taken_op.size = kPCWidth; // 32 bits
  br_taken_op.addr.address_size = kPCWidth; 
  br_taken_op.addr.base_reg.name = "PC";
  br_taken_op.addr.base_reg.size = kPCWidth;
  br_taken_op.addr.displacement = disp;
  br_taken_op.addr.kind = Operand::Address::kControlFlowTarget;
  inst.operands.push_back(br_taken_op);

  inst.branch_taken_pc = static_cast<int32_t>(static_cast<uint32_t>(inst.next_pc) + disp); 

  //not taken pc || DecodeFallThroughPC
  Operand not_taken_op = {};
  not_taken_op.action = Operand::kActionRead;
  not_taken_op.type = Operand::kTypeAddress;
  not_taken_op.size = kPCWidth; // 32 bits
  not_taken_op.addr.address_size = kPCWidth;
  not_taken_op.addr.base_reg.name = "PC";
  not_taken_op.addr.base_reg.size = kPCWidth;
  not_taken_op.addr.displacement = kInstructionSize;
  not_taken_op.addr.kind = Operand::Address::kControlFlowTarget;
  inst.operands.push_back(not_taken_op);
  inst.branch_not_taken_pc = static_cast<int32_t>(static_cast<uint32_t>(inst.next_pc));

  //register operands
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);
  return true ;
}

// BLTZAL  rs,offset
bool TryDecodeBLTZAL(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rs);    
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
                         
  AddPCDisp(inst,target_address );
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.next_pc;      

  Operand cond_op = {};
  cond_op.action = Operand::kActionWrite;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;                                                         
  cond_op.size = 8;                                                             
  inst.operands.push_back(cond_op);

  return true;
}

// BC1F cc, offset
bool TryDecodeBC1F_FP_FALSE(const InstData &data, Instruction &inst) {
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address);
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.pc + 8;
  Operand cond_op = {};
  cond_op.action = Operand::kActionRead;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);
  return true;
}

// BC2F cc, offset
bool TryDecodeBC2F_FP_FALSE(const InstData &data, Instruction &inst) {
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address);
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.pc + 8;
  Operand cond_op = {};
  cond_op.action = Operand::kActionRead;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);
  return true;
}

// BC2FL cc, offset
bool TryDecodeBC2FL_FALSE_LIKELY(const InstData &data, Instruction &inst) {
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address);
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.pc + 4;                                                 
  Operand cond_op = {};
  cond_op.action = Operand::kActionRead;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);
  return true;
}

// BC1FL cc, offset
bool TryDecodeBC1FL_FP_FALSE_LIKELY(const InstData &data, Instruction &inst) {
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address);
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.pc + 8;
  Operand cond_op = {};
  cond_op.action = Operand::kActionRead;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);
  return true;
}

// BC1T cc, offset
bool TryDecodeBC1T_FP_TRUE(const InstData &data, Instruction &inst) {
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address);
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.pc + 8;
  Operand cond_op = {};
  cond_op.action = Operand::kActionRead;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);
  return true;
}

// BC2T cc, offset
bool TryDecodeBC2T_FP_TRUE(const InstData &data, Instruction &inst) {
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address);
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.pc + 8;
  Operand cond_op = {};
  cond_op.action = Operand::kActionRead;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);
  return true;
}

// BC2TL cc, offset
bool TryDecodeBC2TL_TRUE_LIKELY(const InstData &data, Instruction &inst) {
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address);
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.pc + 4;
  Operand cond_op = {};
  cond_op.action = Operand::kActionRead;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);
  return true;
}

// BC1TL cc, offset
bool TryDecodeBC1TL_FP_TRUE_LIKELY(const InstData &data, Instruction &inst) {
  auto target_address =(static_cast<int32_t>(inst.next_pc) + static_cast<signed>(data.offset << 2));
  AddPCDisp(inst,target_address);
  inst.branch_taken_pc = static_cast<uint32_t>(
      static_cast<int32_t>(inst.next_pc ) + static_cast<signed>(data.offset << 2));
  inst.branch_not_taken_pc = inst.pc + 8;                  
  Operand cond_op = {};
  cond_op.action = Operand::kActionRead;
  cond_op.type = Operand::kTypeRegister;
  cond_op.reg.name = "BRANCH_TAKEN";
  cond_op.reg.size = 8;
  cond_op.size = 8;
  inst.operands.push_back(cond_op);
  return true;
}

bool TryDecodeCMP_COND_FMT(const InstData &data, Instruction &inst) {
  
  auto rclass = data.fmt == 16? kRegS : kRegD;
  std::stringstream ss;
  if (data.fmt == 16)
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else { // fmt == 17
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  } 

  AddRegOperand(inst, kActionRead, kRegS, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionRead, kRegS, kUseAsValue, data.rt);
  AddImmOperand(inst, static_cast<uint32_t>(data.cond), kUnsigned,32U);  
  return true;
}

bool TryDecodeROUND_L_fmt(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegD;           
  std::stringstream ss;
  if (data.fmt == 16)
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else { // fmt == 17
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  } 

  AddRegOperand(inst, kActionWrite, kRegD, kUseAsValue, data.fd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.fs);
  return true;
}

bool TryDecodeROUND_W_fmt(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegD;          
  std::stringstream ss;
  if (data.fmt == 16)
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else { // fmt == 17
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  } 
  AddRegOperand(inst, kActionWrite, kRegS, kUseAsValue, data.fd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.fs);
  return true;
}

bool TryDecodeCEIL_L_fmt(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegD;         
  std::stringstream ss;
  if (data.fmt == 16)
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else { // fmt == 17
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  } 
  AddRegOperand(inst, kActionWrite, kRegD, kUseAsValue, data.fd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.fs);
  return true;
}

bool TryDecodeCEIL_W_fmt(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegD;            
  std::stringstream ss;
  if (data.fmt == 16)
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else { // fmt == 17
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  } 
  AddRegOperand(inst, kActionWrite, kRegS, kUseAsValue, data.fd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsValue, data.fs);
  return true;
}

bool TryDecodeCVT_D_FMT(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegW;
  std::stringstream ss;
  if (data.fmt == 16) 
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else if( data.fmt == 20) {
    rclass = kRegW;
    ss << inst.function << "_W";
    inst.function = ss.str();
  }
   else { 
    rclass = kRegD; 
    ss << inst.function << "_L";
    inst.function = ss.str();
  }

  AddRegOperand(inst, kActionWrite, kRegD, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsAddress, data.rs);
  return true;
}

bool TryDecodeCVT_S_FMT(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegW;
  std::stringstream ss;
  if (data.fmt == 17) 
  {
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  }
  else if( data.fmt == 20) { 
    rclass = kRegW;
    ss << inst.function << "_W";
    inst.function = ss.str();
  }
   else { 
    rclass = kRegD; 
    ss << inst.function << "_L";
    inst.function = ss.str();
  }

  AddRegOperand(inst, kActionWrite, kRegS, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsAddress, data.rs);
  return true;
}

// MFC1  <rt>,<fs>
bool TryDecodeMFC1_FLOATING_POINT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegS, kUseAsAddress, data.rs);
  return true;
}

// MFHC1  <rt>,<fs>
bool TryDecodeMFHC1_FLOATING_POINT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rt);
  AddRegOperand(inst, kActionRead, kRegD, kUseAsAddress, data.rs);
  return true;
}

// MTHC1  <rt>,<fs>                                                  
bool TryDecodeMTHC1_FLOATING_POINT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegD, kUseAsAddress, data.fs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// MTC1 <rt>, <fs>
bool TryDecodeMTC1_FLOATING_POINT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegS, kUseAsAddress, data.rs);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// NEG_FMT  <fd>, <fs>,<ft>                                                     
bool TryDecodeNEG_fmt(const InstData &data, Instruction &inst) {
  auto rclass = data.fmt == 16? kRegS : kRegD;            
  std::stringstream ss;
  if (data.fmt == 16)
  {
    rclass = kRegS;
    ss << inst.function << "_S";
    inst.function = ss.str();
  }
  else { // fmt == 17
    rclass = kRegD;
    ss << inst.function << "_D";
    inst.function = ss.str();
  }

  // SetConditionalFunctionName(data, inst);
  AddRegOperand(inst, kActionWrite, rclass, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, rclass, kUseAsAddress, data.rs);
  return true;
}

// SB <rt>, <offset(base)>
bool TryDecodeSB_IMM(const InstData &data, Instruction &inst) {
  AddBasePlusOffsetMemOp(inst, kActionWrite, 8, data.base,static_cast<signed>(data.offset));
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// INS <rt>, <rs>, pos, size
bool TryDecodeINS(const InstData &data, Instruction &inst) {
  
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rs);
  AddImmOperand(inst, data.lsb, kUnsigned, 32);
  AddImmOperand(inst, (data.msbd + 1 - data.lsb), kUnsigned, 32);
  return true;
}

// ASMACRO_16e select,p0,p1,p2,p3,p4
  bool TryDecodeASMACRO_16e(const InstData &data, Instruction &inst) {
    return true;
}

//SEB  <rd>, <rt>
bool TryDecodeSEB_EXTEND_BYTE(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsAddress, data.rd);
  AddRegOperand(inst, kActionRead, kRegW, kUseAsAddress, data.rt);
  return true;
}

// DMT_MT <rt>
bool TryDecodeDMT_MT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  return true;
}

// DVPE_MT <rt>
bool TryDecodeDVPE_MT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  return true;
}

// EMT_MT <rt>
bool TryDecodeEMT_MT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  return true;
}

// EVPE_MT <rt>
bool TryDecodeEVPE_MT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  return true;
}

// FORK_MT  rd, rs, rt 
bool TryDecodeFORK_MT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rs);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  return true;
}
// MFTR_MT rd, rt, u, sel, h
bool TryDecodeMFTR_MT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  return true;
}
// MTTR_MT rt, rd, u, sel, h
bool TryDecodeMTTR_MT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionRead, kRegW, kUseAsValue, data.rt);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  return true;
}

// YIELD_MT rd, rs
bool TryDecodeYIELD_MT(const InstData &data, Instruction &inst) {
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rt);
  AddRegOperand(inst, kActionWrite, kRegW, kUseAsValue, data.rd);
  return true;
}

} // namespace mipsel

// TODO(pag): We pretend that these are singletons, but they aren't really!
const Arch *Arch::GetMips(OSName os_name_, ArchName arch_name_) {
  return new MIPSELArch(os_name_, arch_name_);
}

}  // namespace remill
