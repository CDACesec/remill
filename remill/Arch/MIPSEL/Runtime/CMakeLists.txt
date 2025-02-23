# Copyright (c) 2017 Trail of Bits, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Reviewed and Modified for use by Centre for Development of Advanced Computing (C-DAC)


cmake_minimum_required(VERSION 3.2)
project(mipsel_runtime)

set(MIPS32RUNTIME_SOURCEFILES                                                                # changes added
  Instructions.cpp
  BasicBlock.cpp
)

set_source_files_properties(Instructions.cpp PROPERTIES COMPILE_FLAGS "-O3 -g0")
set_source_files_properties(BasicBlock.cpp PROPERTIES COMPILE_FLAGS "-O0 -g3")

if(DEFINED WIN32)
  set(install_folder "${CMAKE_INSTALL_PREFIX}/remill/${REMILL_LLVM_VERSION}/semantics")
else()
  set(install_folder "${CMAKE_INSTALL_PREFIX}/share/remill/${REMILL_LLVM_VERSION}/semantics")
endif()

function(add_runtime_helper target_name address_bit_size little_endian)
  message(" > Generating runtime target: ${target_name}")

  # Visual C++ requires C++14
  if(WIN32)
    set(required_cpp_standard "c++14")
  else()
    set(required_cpp_standard "c++11")
  endif()

  add_runtime(${target_name}
    SOURCES ${MIPS32RUNTIME_SOURCEFILES}                                                         # changes added
    ADDRESS_SIZE ${address_bit_size}
    DEFINITIONS "LITTLE_ENDIAN=${little_endian}"
    BCFLAGS "-std=${required_cpp_standard}"
    INCLUDEDIRECTORIES "${CMAKE_SOURCE_DIR}"
    INSTALLDESTINATION "${install_folder}"

    DEPENDENCIES
    
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/BINARY.cpp"                                # changes added for mips
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/DATAXFER.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/MISC.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/COND.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/LOGICAL.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/UNCOND.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/DIRECT_CALL.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/DIRECT_JUMP.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/SHIFT.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/BRANCH_PIPELINE.cpp"
    "${CMAKE_SOURCE_DIR}/remill/Arch/MIPSEL/Semantics/COND_BRANCH.cpp"
    
       
  )
endfunction()

#if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  #add_runtime_helper(aarch64be 64 0)
  add_runtime_helper(mips32 32 1 )                                                              # changes added (important)
#endif()
