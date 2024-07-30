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

1. The bp application has been tested on Ubuntu 22.04.4 LTS.

The build requires the __cmake__ build system and a compiler toolchain (by default __gcc__).

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
   export CFS_HOME="$(pwd)" # Use CFS_HOME at your discretion. 
   git clone https://github.com/nasa/cFS.git cfs-bundle
   cd cfs-bundle
   git submodule init
   git submodule update
   cd psp
   git fetch --no-tags --depth=1 https://github.com/jphickey/PSP.git \
       techdev-iodriver:techdev-iodriver
   git checkout techdev-iodriver
   cd "${CFS_HOME}"/cfs-bundle
   git clone https://github.com/nasa/CF.git apps/cf
   git clone https://github.com/nasa/bp.git apps/bp
   git clone https://github.com/nasa/bplib.git libs/bplib
   cd "${CFS_HOME}"
   # Copy the Mission Definitions to a temporary folder ("${CFS_HOME}"/bpxfer_defs)
   mkdir -p "${CFS_HOME}"/bpxfer_defs
   cp  "${CFS_HOME}"/cfs-bundle/cfe/cmake/sample_defs/sample_perfids.h "${CFS_HOME}"/bpxfer_defs/cfe_perfids.h
   cp -v -t "${CFS_HOME}"/bpxfer_defs "${CFS_HOME}"/cfs-bundle/cfe/cmake/sample_defs/*{osconfig,custom,options}.cmake
   cp -v -t "${CFS_HOME}"/bpxfer_defs "${CFS_HOME}"/cfs-bundle/apps/bp/.github/buildconfig/*.{cmake,scr}
   cp -rv -t "${CFS_HOME}"/bpxfer_defs "${CFS_HOME}"/cfs-bundle/apps/bp/.github/buildconfig/{tx,rx,tables}
   cmake -DCMAKE_BUILD_TYPE=Release -DSIMULATION=native \
      -DCMAKE_INSTALL_PREFIX=/exe \
      -DMISSIONCONFIG=bpxfer -DMISSION_DEFS="${CFS_HOME}"/bpxfer_defs \
      -B build -S cfs-bundle/cfe
```

#### Run Make

4. Build BP

Run `make` in the build directory.

```
   cd "${CFS_HOME}"/build
   make DESTDIR="${CFS_HOME}"/cfs-build-folder -j2 mission-install
   cd "${CFS_HOME}"/cfs-build-folder/exe
   tar -cvf "${CFS_HOME}"/cfs-bpapp.tar .
```

#### Test

5. Test BP

- Create a bp-test subfolder. Untar the cfs-bpapp.tar file there.
- Transmit and receive random data.
- Verify the md5sums match for the transmitted and received data.

```sh
   mkdir "${CFS_HOME}"/bp-test
   cd "${CFS_HOME}"/bp-test
   tar -xvf "${CFS_HOME}"/cfs-bpapp.tar
   cd "${CFS_HOME}"bp-test/tx/cf
   dd if=/dev/urandom of=testdata.bin bs=1k count=1
   cd "${CFS_HOME}"/bp-test
   mkdir -p "${CFS_HOME}"/bp-test/tx/storage "${CFS_HOME}"/bp-test/rx/storage
   cd "${CFS_HOME}"/bp-test/tx
   ./core-tx > console_output.log 2>&1 &
   cd "${CFS_HOME}"/bp-test/rx
   ./core-rx > console_output.log 2>&1 &
   sleep 5 && grep -q 'CFE_ES_Main entering OPERATIONAL state' "${CFS_HOME}"/bp-test/tx/console_output.log
   cd "${CFS_HOME}"/bp-test/host
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
   # Wait for Shutdown
   sleep 5
   # Dump Tx Log
   cd "${CFS_HOME}"
   cat "${CFS_HOME}"/bp-test/tx/console_output.log
   cat "${CFS_HOME}"/bp-test/rx/console_output.log
   md5sum "${CFS_HOME}"/bp-test/tx/cf/testdata.bin
   md5sum "${CFS_HOME}"/bp-test/rx/cf/testdata.bin
```
