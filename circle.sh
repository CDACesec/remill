#!/bin/bash



# Lift, recompile and execute a mips32 binary

function Lift
{
  if [ $arch == "mipsl" ];then
    arch="mips32"
  fi
  remill-build/tools/mcsema/mcsema-lift-4.0 --os linux --arch $arch --cfg $bn.cfg --output $bn.bc
  return $?
}


read -p "path to mips32 binary: " bn
read -p "arch? [ X86 | AMD64 | AARCH64 | mipsl ]" arch

mcsema-disass --os 
mcsema-disass  --os linux --arch $arch --entrypoint main --output $bn.cfg --log_file $bn.log --binary $bn


echo "generating bitocode"

if ! ( Lift $1 ); then    
        echo "bitcode generation failed"
        exit -1
fi

echo "created .bc file"

echo Converting .bc into .ll file

llvm-dis-4.0 $bn.bc

echo "Recompiling..."

clang-4.0 -o $bn\_re -fuse-ld=lld --target="mipsel-pc-linux-gnu-elf" $bn.ll  remill/remill-build/tools/mcsema/mcsema/Arch/MIPSEL/Runtime/lib_mipsel.a

echo -e "\n\nExecuting...\n\n"

qemu-mipsel -L /usr/mipsel-linux-gnu/ $bn.new

exit 