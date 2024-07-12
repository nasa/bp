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
```

The `prep-bp-build` script prepares the build folder from a cFS repository cloned to the `cfs-bundle` folder. The script includes initializing the local cFS repository with all required submodules and running the __cmake__ command to create the `build` folder containing a `Makefile`.

`prep-bp-build`
```sh
   cd $HOME
   git clone git@github.com:nasa/cFS.git cfs-bundle 
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
   make DESTDIR=.. -j2 mission-install
   cd ../exe
   tar -cvf ../cfs-bpapp.tar .
   cd ..
   ls -l cfs-bpapp.tar
```

#### Example Application


----------------------------------------------------------------------
## 3. Application Design
----------------------------------------------------------------------

![Figure 1](doc/bp_api_architecture.png "BP Library API (Architecture)")


----------------------------------------------------------------------
## 4. Command Interface
----------------------------------------------------------------------

The appl

----------------------------------------------------------------------
## 5. Telemetry Interface
----------------------------------------------------------------------

The Telemetry Interface

