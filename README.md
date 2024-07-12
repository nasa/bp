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

Note: These bash statements mimic the YAML statements in the .github Actions and Workflows scripts. Also, the .github Actions and Workflows specify Ubuntu 22.04 (22.04.4 LTS).

```sh
   cd $HOME
   sudo apt update
   sudo apt upgrade
   sudo apt install cmake pkg-config build-essential
   git clone git@github.com:nasa/cFS.git cfs-bundle 
   pushd cfs-bundle
   git submodule init
   git submodule update
   git fetch --no-tags --depth=1 https://github.com/jphickey/PSP.git techdev-iodriver:techdev-iodriver
   pushd psp
   # Does the branch techdev-iodriver exist?
   git checkout techdev-iodriver
   popd; popd
   git clone https://github.com/nasa/CF.git cfs-bundle/apps/cf
   git clone git@github.com:gskenned/bp.git src/bp
   git clone git@github.com:gskenned/bplib.git src/bplib
   mkdir -p ./bpxfer_defs
   cp  ./cfs-bundle/cfe/cmake/sample_defs/sample_perfids.h ./bpxfer_defs/cfe_perfids.h
   cp -v -t ./bpxfer_defs ./cfs-bundle/cfe/cmake/sample_defs/*{osconfig,custom,options}.cmake
   cp -v -t ./bpxfer_defs ./src/bp/.github/buildconfig/*.{cmake,scr}
   cp -rv -t ./bpxfer_defs ./src/bp/.github/buildconfig/{tx,rx,tables}
   cmake -DCMAKE_BUILD_TYPE=Release -DSIMULATION=native \
      -DCMAKE_INSTALL_PREFIX=/exe \
      -DMISSIONCONFIG=bpxfer -DMISSION_DEFS=$PWD/bpxfer_defs \
      -B build -S cfs-bundle/cfe
   pushd ./build
   make DESTDIR=.. -j2 mission-install
   pushd exe
   tar -cvf ../cfs-bpapp.tar .

exit # WIP
   mkdir build-bp
   cd build-bp
   cmake ../bp
```

#### Building

Build bplib by running __make__ in the build subdirectory:

```sh
   cd $HOME/build-bplib
   make
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


## WIP Notes:

**Current Build Log**

```
gskenned@ip-10-1-21-63:~/repos/forks/cfs-bundle
$ git fetch --no-tags --depth=1 https://github.com/jphickey/PSP.git techdev-iodriver:techdev-iodriver
warning: no common commits
remote: Enumerating objects: 308, done.
remote: Counting objects: 100% (308/308), done.
remote: Compressing objects: 100% (222/222), done.
remote: Total 308 (delta 133), reused 159 (delta 71), pack-reused 0
Receiving objects: 100% (308/308), 834.53 KiB | 6.42 MiB/s, done.
Resolving deltas: 100% (133/133), done.
From https://github.com/jphickey/PSP
 * [new branch]      techdev-iodriver -> techdev-iodriver
gskenned@ip-10-1-21-63:~/repos/forks/cfs-bundle
$ cd psp
gskenned@ip-10-1-21-63:~/repos/forks/cfs-bundle/psp
$ git checkout techdev-iodriver
error: pathspec 'techdev-iodriver' did not match any file(s) known to git
gskenned@ip-10-1-21-63:~/repos/forks/cfs-bundle/psp
$ git remote -v
origin  https://github.com/nasa/PSP.git (fetch)
origin  https://github.com/nasa/PSP.git (push)
```

