# BP - The cFS Bundle Protocol Application

[1. Overview](#1-overview)  
[2. Build with CMake](#2-build-with-cmake)  
[3. Command Interface](#3-command-interface)  

----------------------------------------------------------------------
## 1. Overview
----------------------------------------------------------------------

The cFS Bundle Protocol Application (bp) is a core Flight System (cFS) application that integrates the Bundle Protocol (BP) library (bplib) with cFS to add RFC9171 Bundle Protocol services to embedded space flight systems.

With bp, cFS can participate in a Delay/Disruption Tolerant Network (DTN) as a DTN Node. cFS bp communicate with other DTN nodes via BP.

----------------------------------------------------------------------
## 2. Build with CMake
----------------------------------------------------------------------

#### Prerequisites

1. The build requires Ubuntu 22.04.4 LTS, the __cmake__ build system and a compiler toolchain (by default __gcc__).

Additionally, the __pkg-config__ tool is used to manage the flags required for dependencies.

The required packages on Ubuntu 22.04 are: cmake, pkg-config, and build-essential.

The versions used may change. The versions as of the last update of this document are:

- cmake version 3.22.1
- pkg-config 0.29.1
- gcc  (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0

2. Install tinycbor if it is not already installed.  
See https://github.com/intel/tinycbor.git.

- tinycbor version 0.6.0

#### Run CMake

3. Create a subdirectory for building a cFS bundle that includes bp (./cfs-bundle) and run CMake to set up the build tree (./build).


```
   git clone https://github.com/nasa/cFS.git cfs-bundle
   pushd cfs-bundle
   git submodule init
   git submodule update
   pushd psp
   git fetch --no-tags --depth=1 https://github.com/jphickey/PSP.git \
       techdev-iodriver:techdev-iodriver
   git checkout techdev-iodriver
   popd
   git clone https://github.com/nasa/CF.git apps/cf
   git clone https://github.com/nasa/bp.git apps/bp
   git clone https://github.com/gs/bplib.git libs/bplib
   popd
   # Copy the Mission Definitions to a temporary folder (./bpxfer_defs)
   mkdir -p ./bpxfer_defs
   cp  ./cfs-bundle/cfe/cmake/sample_defs/sample_perfids.h ./bpxfer_defs/cfe_perfids.h
   cp -v -t ./bpxfer_defs ./cfs-bundle/cfe/cmake/sample_defs/*{osconfig,custom,options}.cmake
   cp -v -t ./bpxfer_defs ./cfs-bundle/apps/bp/.github/buildconfig/*.{cmake,scr}
   cp -rv -t ./bpxfer_defs ./cfs-bundle/apps/bp/.github/buildconfig/{tx,rx,tables}
   cmake -DCMAKE_BUILD_TYPE=Release -DSIMULATION=native \
      -DCMAKE_INSTALL_PREFIX=/exe \
      -DMISSIONCONFIG=bpxfer -DMISSION_DEFS=$PWD/bpxfer_defs \
      -B build -S cfs-bundle/cfe
```

#### Run Make

4. Build BP

Run `make` in the build directory.

```
   cd ./build
   make DESTDIR=./cfs-build-folder -j2 mission-install
   cd ./cfs-build-folder/exe
   tar -cvf ./cfs-bpapp.tar .
```

#### Test

5. Test BP

- Create a bp-test subfolder. Untar the cfs-bpapp.tar file there.
- Transmit and receive random data.
- Verify the md5sums match for the transmitted and received data.

```sh
   mkdir ./bp-test
   pushd ./bp-test
   tar -xvf cfs-bpapp.tar
   pushd tx/cf
   dd if=/dev/urandom of=testdata.bin bs=1k count=1
   popd
   mkdir -p ./tx/storage ./rx/storage
   pushd tx
   ./core-tx > console_output.log 2>&1 &
   popd
   pushd rx
   ./core-rx > console_output.log 2>&1 &
   popd
   sleep 5 && grep -q 'CFE_ES_Main entering OPERATIONAL state' ./tx/console_output.log
   pushd ./exe/host
   # Enable Flows
   ./cmdUtil --port 1234 -ELE -I0x1812 -C7 -s8:CFDP
   ./cmdUtil --port 1235 -ELE -I0x1812 -C7 -s8:CFDP
   # Initiate Class 1 Transfer
   ./cmdUtil --port 1234 -ELE -I0x18B3 -C2 -b0 -b1 -b0 -b0 -o2 -s64:/cf/testdata.bin -s64:/cf/testdata.bin
   # Wait for Transfer
   sleep 30
   # Terminate cFS
   ./cmdUtil --port=1234 --endian=LE --pktid=0x1806 --cmdcode=2 --half=0x0002
   ./cmdUtil --port=1235 --endian=LE --pktid=0x1806 --cmdcode=2 --half=0x0002
   # Waith for Shutdown
   sleep 5
   # Dump Tx Log
   popd
   cat tx/console_output.log
   cat rx/console_output.log
   md5sum tx/cf/testdata.bin
   md5sum rx/cf/testdata.bin
```
