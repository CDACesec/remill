#!/bin/bash

# Tested on ubuntu 18.04 12-09-2019

# Implementation of Shell script to build Mcsema  

#-------------------------
#Assumptions: the script is executed in users home directory
#-------------------------

# Upgrading Packages
echo --------------------
echo Upgrading packages
echo --------------------

sudo apt-get update

# Download dependencies
echo --------------------
echo Installing the dependencies
echo --------------------
 sudo apt-get install \
             git \
             curl \
             cmake \
             python2.7 python-pip python-virtualenv \
             wget \
             build-essential \
             gcc-multilib g++-multilib \
             libtinfo-dev \
             lsb-release \
             realpath \
             zlib1g-dev \
             llvm-4.0 \
             clang-4.0 \
             lld-4.0 \
             qemu-user -y

# install the mips cross-compiler
echo --------------------
echo Installing the MIPS cross compiler
echo --------------------
sudo apt-get install gcc-mipsel-linux-gnu -y

# for some reason, installing MIPS cross compiler removes
# gcc multilib, so, a workaround for that is use the
# below command

sudo ln -s /usr/include/asm-generic /usr/include/asm




# Although remill ships with it's version of LLVM, that
# LLVM doesn't have backend for MIPS32, so we will instead utilize
# the precompiled binaries available with ubuntu's package manager


# Download Mcsema and Remill source code

echo --------------------
echo Cloning the repository of mcsema and remill
echo --------------------

required_remill_cid="8146c3d3c662f9c6b7530dc9048b03495cee3fe";
required_mcsema_cid="bb1cac3e5271fa7765109331590b8af6f4fc8c1a";

echo --------------------
echo \#1. Cloning the Mcsema into pwd
echo --------------------
    
git clone git@github.com:CDACesec/mcsema.git
export REMILL_VERSION=`cat ./mcsema/.remill_commit_id`

if [[ $REMILL_VERSION != required_remill_cid ]]; then
    # manually fix the correct commit hash for both
    # mcsema and remill
    export REMILL_VERSION=$required_remill_cid;
    git checkout -b temp $required_mcsema_cid;
fi


echo --------------------
echo \#2. Cloning the remill into pwd
echo --------------------


git clone git@github.com:CDACesec/remill.git
cd ~/remill
git checkout -b temp ${REMILL_VERSION}


# Move the Mcsema repository inside remill/tools folder

echo --------------------
echo Moving the mcsema into the remill-build/tools                       
echo --------------------


mv -f ../mcsema ~/remill/tools

echo --------------------
echo Building Mcsema
echo --------------------
# Build Mcsema


./scripts/build.sh

echo --------------------
echo Installation
echo --------------------
# Installing Mcsema


cd ~/remill/remill-build
sudo make install

# Verifying that Mcsema is installed properly or not 

echo --------------------
echo Try execution mcsema cmds: mcsema-disass \& mc-sema-lift-4.0
echo --------------------

 mcsema-disass
 mcsema-lift-4.0

# echo --------------------
# echo Remill Test Suite 
# echo --------------------
#  cd ~/remill/remill-build
#  make test_dependencies
#  make test


echo "Building finished, you may now run `circle.sh` to further check working of the installation";