# BP - The cFS Bundle Protocol Application

[1. Overview](#1-overview)  
[2. Build with CMake](#2-build-with-cmake)  
[3. Application Design](#3-application-design)  
[4. Command Interface](#4-command-interface)  
[5. Telemetry Interface](#5-telemetry-interface)

----------------------------------------------------------------------
## 1. Overview
----------------------------------------------------------------------

The cFS Bundle Protocol Application (bp) is a core Flight System (cFS) application that integrates the Bundle Protocol (BP) library (bplib) with cFS to add RFC9171 Bundle Protocol services to embedded space flight systems.

With bp, cFS can participate in a Delay/Disruption Tolerant Network (DTN) as a DTN Node. cFS bp communicate with other DTN nodes via BP.

----------------------------------------------------------------------
## 2. Build with CMake
----------------------------------------------------------------------

#### Prerequisites

1. The build requires the __cmake__ build system and a compiler toolchain (by default __gcc__).

Additionally, the __pkg-config__ tool is used to manage the flags required for dependencies.

The bash statements in Section 2. include the `apt install` statement to install the required toolchain.

3. Create a subdirectory for building a cFS bundle that includes  bp and run CMake to set up the build tree.

Notes:
- These bash statements mimic the YAML statements in the .github Actions and Workflows scripts. Also, the .github Actions and Workflows specify Ubuntu 22.04 (22.04.4 LTS).
- Example bash scripts are in [docs/example_scripts](docs/example_scripts)
- Use of the `$HOME` environment variable is at your discretion. The example scripts use `$HOME` to distinguish the top level folder versus `$HOME/build`, the build folder.

The `install-toolchain` script installs cmake, pkg-config and build-essential (gcc toolchain). Run the script once, if required. There's no harm done by running the script again. It reports that the packages are already installed.

`install-toolchain`
```sh
   cd $HOME
   sudo apt update
   sudo apt upgrade
   sudo apt install cmake pkg-config build-essential
   git clone https://github.com/intel/tinycbor.git tinycbor-source
   mkdir tinycbor-staging
   pushd tinycbor-source
   make all && make DESTDIR=../tinycbor-staging install
   popd
   # Install from tinycbor-staging to /usr/local/include, lib, and bin
   sudo cp -rv -t / tinycbor-staging/*
```

The `prep-bp-build` script prepares the build folder from a cFS repository cloned to the `cfs-bundle` folder. The script includes initializing the local cFS repository with all required submodules and running the __cmake__ command to create the `build` folder containing a `Makefile`.

`prep-bp-build`
```sh
   cd $HOME
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
   git clone https://github.com/gskenned/bp.git apps/bp
   pushd apps/bp; git checkout fix-dtnn-132-bp-bplib-readmes; popd
   git clone https://github.com/gskenned/bplib.git libs/bplib
   popd
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

#### Building

The `build-bp` script builds bp by running __make__ in the build subdirectory:

`build-bp`
```sh
   cd $HOME/build
   make DESTDIR=$HOME/cfs-build-folder -j2 mission-install
   cd $HOME/cfs-build-folder/exe
   tar -cvf $HOME/cfs-bpapp.tar .
   cd $HOME
   ls -l cfs-bpapp.tar
```

#### BP Test

`bp-test`
```sh
   cd $HOME
   tar -xvf cfs-bpapp.tar
   pushd tx/cf
   dd if=/dev/urandom of=testdata.bin bs=1k count=1
   md5sum testdata.bin | tee -a $HOME/testdata.md5sum
   popd
   mkdir -p ./tx/storage ./rx/storage
   pushd tx
   ./core-tx > console_output.log 2>&1 &
   popd
   pushd rx
   ./core-rx > console_output.log 2>&1 &
   popd
   sleep 5 && grep -q 'CFE_ES_Main entering OPERATIONAL state' ./tx/console_output.log
   pushd $HOME/host
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

----------------------------------------------------------------------
## 3. BP Application Design
----------------------------------------------------------------------



----------------------------------------------------------------------
## 4. Command Interface
----------------------------------------------------------------------

```
cmdUtil -- Command Client.
  - General options:
    -v, --verbose: Extra output
    -?, --help: print options and exit
  - Destination options:
    -H, --host: Destination hostname or IP address (Default = 127.0.0.1)
    -P, --port: Destination port (default = 1234)
  - Packet format options:
    -E, --endian: Default endian for unnamed fields/payload: [BE|LE] (default = BE)
    -Q, --protocol: Sets allowed named fields and header layout (default = cfsv1)
             raw = no predefined fields/layout
        ccsdspri = CCSDS Pri header only
        ccsdsext = CCSDS Pri and Ext headers
           cfsv1 = CCSDS Pri and cFS Cmd Sec headers
           cfsv2 = CCSDS Pri, Ext, and cFS Cmd Sec headers
  - CCSDS Primary Header named fields (protocol=[ccsdspri|ccsdsext|cfsv1|cfsv2])
    -I, --pktid: macro for setting first 16 bits of CCSDS Primary header
    -V, --pktver: Packet version number (range=0-0x7)
    -T, --pkttype: !OVERRIDE! Packet type (default is cmd, 0=tlm, 1=cmd)
    -S, --pktsec: !OVERRIDE! Secondary header flag (default set from protocol, 0=absent, 1=present)
    -A, --pktapid: Application Process Identifier (range=0-0x7FF)
    -F, --pktseqflg: !OVERRIDE! Sequence Flags (default unsegmented, 0=continuation, 1=first, 2=last, 3=unsegmented)
    -G, --pktseqcnt, --pktname: Packet sequence count or Packet name (range=0-0x3FFF)
    -L, --pktlen: !OVERRIDE! Packet data length (default will calculate value, range=0-0xFFFF)
  - CCSDS Extended Header named fields (protocol=[ccsdsext|cfsv2])
    -D, --pktedsver: EDS version (range=0-0x1F)
    -J, --pktendian: !OVERRIDE! Endian (default uses endian, 0=big, 1=little(INTEL))
    -B, --pktpb: Playback field (0=original, 1=playback)
    -U, --pktsubsys: APID Qualifier Subsystem (range=0-0x1FF)
    -Y, --pktsys: APID Qualifier System (range=0-0xFFFF)
  - cFS Command Secondary Header named fields (protocol=[cfsv1|cfsv2])
    -C, --pktfc, --cmdcode: Command function code (range=0-0x7F)
    -R, --pktcksum: !OVERRIDE! Packet checksum (default will calculate value, range=0-0xFF)
  - Raw values converted to selected endian where applicable
    -b, --int8, --byte: Signed 8 bit int (range=-127-127)
    -m, --uint8: Unsigned 8 bit int (range=0-0xFF)
    -h, --int16, --half: Signed 16 bit int (range=-32767-32767)
    -n, --uint16: Unsigned 16 bit int (range=0-0xFFFF)
    -l, --int32, --long, --word: Signed 32 bit int (range=-2147483647-2147483647)
    -o, --uint32: Unsigned 32 bit int (range=0-0xFFFFFFFF)
    -q, --int64: Signed 64 bit int (range=-9223372036854775807-9223372036854775807)
    -p, --uint64: Unsigned 64 bit int (range=0-0xFFFFFFFFFFFFFFFF)
    -d, --double: Double format (caution - host format w/ converted endian, may not match target)
    -f, --float: Float format (caution - host format w/ converted endian, may not match target)
    -s, --string: Fixed length string, NNN:String where NNN is fixed length (0 padded)
  - Raw values always big endian (even if endian=LE)
    -i, --int16b: Big endian signed 16 bit int (range=-32767-32767)
    -j, --int32b: Big endian signed 32 bit int (range=-2147483647-2147483647)
    -k, --int64b: Big endian signed 64 bit int (range=-9223372036854775807-9223372036854775807)
    -w, --uint16b: Big endian unsigned 16 bit int (range=0-0xFFFF)
    -x, --uint32b: Big endian unsigned 32 bit int (range=0-0xFFFFFFFF)
    -y, --uint64b: Big endian unsigned 64 bit int (range=0-0xFFFFFFFFFFFFFFFF)
 
Examples:
  ./cmdUtil --host=localhost --port=1234 --pktid=0x1803 --pktfc=3 --int16=100 --string=16:ES_APP
  ./cmdUtil -ELE -C2 -A6 -n2 # Processor reset on little endian, using defaults
  ./cmdUtil --endian=LE --protocol=raw --uint64b=0x1806C000000302DD --uint16=2
  ./cmdUtil --pktver=1 --pkttype=0 --pktsec=0 --pktseqflg=2 --pktlen=0xABC --pktcksum=0
  ./cmdUtil -Qcfsv2 --pktedsver=0xA --pktendian=1 --pktpb=1 --pktsubsys=0x123 --pktsys=0x4321 --pktfc=0xB
```

----------------------------------------------------------------------
## 5. Telemetry Interface
----------------------------------------------------------------------


