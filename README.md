Forked from <https://github.com/lifting-bits/remill>

This fork of `remill` contains support for MIPS32 architecture

This fork is based on commit-id : `8146c3d3c662f9c6b7530dc9048b03495cee3fe`
of [remill](https://github.com/lifting-bits/remill)

## Steps for installation and usage
### Step 1. Install the required pre-requisites:

```
sudo apt-get update
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
```


#### Install the mips cross-compiler
``` sudo apt-get install gcc-mipsel-linux-gnu -y ```

For some reason, installing MIPS cross compiler removes `gcc-multilib`, so, a workaround for that is use the below command:

``` sudo ln -s /usr/include/asm-generic /usr/include/asm ```
### Step 2. Clone remill
```
git clone https://github.com/CDACesec/remill.git 
cd remill/tools
```

Using `remill` alone on it's own is not possible, so, clone and install
***our fork of mcsema*** as well.
 
Make sure to clone `Mcsema` into the `tools` folder of `remill`

```
git clone https://github.com/CDACesec/mcsema.git
cd ..
./scripts/build.sh
cd remill-build
sudo make install
```
Verify the installation by executing the following command
```
mcsema-disass
mcsema-lift-4.0
```

For more details. visit the [docs](docs) folder of this repo

Contact us at : `esuraksha@cdac.in`
