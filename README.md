# BP - The cFS Bundle Protocol Application

[1. Overview](#1-overview)
[2. Build with CMake](#2-build-with-cmake)
[3. Application Design](#3-application-design)
[4. Command Interface](#4-command-interface)
[5. Telemetry Interface](#5-telemetry-interface)

----------------------------------------------------------------------
## 1. Overview
----------------------------------------------------------------------

The Bundle Protocol Application (bp) is a core Flight System (cFS) application that integrates the Bundle Protocol library (bplib) with cFS to add RFC9171 Bundle Protocol services to embedded space flight applications.

With bp, cFS can act as a Bundle Protocol (BP) node within a  node in a Delay/Disruption Tolerant Network (DTN), a "DTN Node". cFS BP nodes communicate with other DTN nodes via BP.

----------------------------------------------------------------------
## 2. Build with CMake
----------------------------------------------------------------------

#### Prerequisites

1. The build requires the __cmake__ build system and a compiler toolchain (by default __gcc__).

Additionally, the __pkg-config__ tool is used to manage the flags required for dependencies.

These can typically be installed via the built-in package management system on most Linux
distributions.

On Debian/Ubuntu and derivatives:

```sh
    sudo apt-get install cmake pkg-config build-essential
```

2. For BPv7 this uses the TinyCBOR library at https://github.com/intel/tinycbor.  As any
distribution-packaged version may be outdated, it is recommended to compile this from source.
As of this writing, the library uses a simple Makefile that will install into `/usr/local`.
This installation prefix can be changed by editing the Makefile before building.  Otherwise,
to install into the default location, steps are as follows:

```sh
    git clone https://github.com/intel/tinycbor
    cd tinycbor
    make
    sudo make install
```

Note that "sudo" is only required for installation into a system directory.  If installing into a
user-writable home directory, "sudo" is not necessary.

3. Create a subdirectory for building bplib and run CMake to set up the build tree.

```sh
   cd $HOME
   mkdir build-bplib
   cd build-bplib
   cmake ../bplib
```

Note that "sudo" is only required for installation into a system directory.  If installing into a
user-writable home directory, "sudo" is not necessary.

3. Create a subdirectory for building bp and run CMake to set up the build tree.

Note: These bash statements mimic the YAML statements in the .github Actions and Workflows scripts.

```sh
   cd $HOME
   git clone git@github.com:gskenned/bp.git src/bp
   git clone git@github.com:gskenned/bplib.git src/bplib
   git clone git@github.com:nasa/cFS.git cfs-bundle 
   mkdir -p ./bpxfer_defs
   cp  ./cfs-bundle/cfe/cmake/sample_defs/sample_perfids.h ./bpxfer_defs/cfe_perfids.h
   cp -v -t ./bpxfer_defs ./cfs-bundle/cfe/cmake/sample_defs/*{osconfig,custom,options}.cmake
   cp -v -t ./bpxfer_defs ./src/bp/.github/buildconfig/*.{cmake,scr}
   cp -rv -t ./bpxfer_defs ./src/bp/.github/buildconfig/{tx,rx,tables}
   cmake -DCMAKE_BUILD_TYPE=Release -DSIMULATION=native -DCMAKE_INSTALL_PREFIX=/exe \
   -DMISSIONCONFIG=bpxfer -DMISSION_DEFS=$PWD/bpxfer_defs -B build -S cfs-bundle/cfe
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


WIP Notes:

Build log so far:

```
gskenned@ip-10-1-21-63:~/repos/forks
$   cmake -DCMAKE_BUILD_TYPE=Release -DSIMULATION=native -DCMAKE_INSTALL_PREFIX=/exe \
>    -DMISSIONCONFIG=bpxfer -DMISSION_DEFS=$PWD/bpxfer_defs -B build -S cfs-bundle/cfe
-- The C compiler identification is GNU 9.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- OMIT_DEPRECATED=false: Deprecated elements included in build
-- Search path for modules: apps;libs;psp/fsw/modules;cfe/modules
-- Module 'core_api' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/core_api
-- Module 'core_private' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/core_private
-- Module 'cfe_assert' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/cfe_assert
** Module cf NOT found **
** Module bp NOT found **
** Module bplib NOT found **
-- Module 'ci_lab' found at /home/gskenned/repos/forks/cfs-bundle/apps/ci_lab
-- Module 'to_lab' found at /home/gskenned/repos/forks/cfs-bundle/apps/to_lab
-- Module 'sch_lab' found at /home/gskenned/repos/forks/cfs-bundle/apps/sch_lab
-- Module 'es' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/es
-- Module 'evs' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/evs
-- Module 'fs' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/fs
-- Module 'sb' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/sb
-- Module 'tbl' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/tbl
-- Module 'time' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/time
-- Module 'osal' found at /home/gskenned/repos/forks/cfs-bundle/osal
-- Module 'psp' found at /home/gskenned/repos/forks/cfs-bundle/psp
-- Module 'msg' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/msg
-- Module 'sbr' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/sbr
-- Module 'resourceid' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/resourceid
-- Module 'config' found at /home/gskenned/repos/forks/cfs-bundle/cfe/modules/config
** Module unsock_intf NOT found **
CMake Error at cmake/mission_build.cmake:354 (message):
  Target build incomplete, source for 4 module(s) not found.
Call Stack (most recent call first):
  CMakeLists.txt:135 (prepare)


-- Configuring incomplete, errors occurred!
See also "/home/gskenned/repos/forks/build/CMakeFiles/CMakeOutput.log".

CMakeOutput.log:

gskenned@ip-10-1-21-63:~/repos/forks
$ cat build/CMakeFiles/CMakeOutput.log 
The system is: Linux - 5.4.0-1021-aws-fips - x86_64
Compiling the C compiler identification source file "CMakeCCompilerId.c" succeeded.
Compiler: /usr/bin/cc 
Build flags: 
Id flags:  

The output was:
0


Compilation of the C compiler identification source "CMakeCCompilerId.c" produced "a.out"

The C compiler identification is GNU, found in "/home/gskenned/repos/forks/build/CMakeFiles/3.16.3/CompilerIdC/a.out"

Determining if the C compiler works passed with the following output:
Change Dir: /home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp

Run Build Command(s):/usr/bin/make cmTC_aa1c3/fast && /usr/bin/make -f CMakeFiles/cmTC_aa1c3.dir/build.make CMakeFiles/cmTC_aa1c3.dir/build
make[1]: Entering directory '/home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp'
Building C object CMakeFiles/cmTC_aa1c3.dir/testCCompiler.c.o
/usr/bin/cc    -o CMakeFiles/cmTC_aa1c3.dir/testCCompiler.c.o   -c /home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp/testCCompiler.c
Linking C executable cmTC_aa1c3
/usr/bin/cmake -E cmake_link_script CMakeFiles/cmTC_aa1c3.dir/link.txt --verbose=1
/usr/bin/cc      CMakeFiles/cmTC_aa1c3.dir/testCCompiler.c.o  -o cmTC_aa1c3 
make[1]: Leaving directory '/home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp'



Detecting C compiler ABI info compiled with the following output:
Change Dir: /home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp

Run Build Command(s):/usr/bin/make cmTC_4cacc/fast && /usr/bin/make -f CMakeFiles/cmTC_4cacc.dir/build.make CMakeFiles/cmTC_4cacc.dir/build
make[1]: Entering directory '/home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp'
Building C object CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o
/usr/bin/cc   -v -o CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o   -c /usr/share/cmake-3.16/Modules/CMakeCCompilerABI.c
Using built-in specs.
COLLECT_GCC=/usr/bin/cc
OFFLOAD_TARGET_NAMES=nvptx-none:hsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.4.0-1ubuntu1~20.04.2' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-9QDOt0/gcc-9-9.4.0/debian/tmp-nvptx/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.2) 
COLLECT_GCC_OPTIONS='-v' '-o' 'CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o' '-c' '-mtune=generic' '-march=x86-64'
 /usr/lib/gcc/x86_64-linux-gnu/9/cc1 -quiet -v -imultiarch x86_64-linux-gnu /usr/share/cmake-3.16/Modules/CMakeCCompilerABI.c -quiet -dumpbase CMakeCCompilerABI.c -mtune=generic -march=x86-64 -auxbase-strip CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o -version -fasynchronous-unwind-tables -fstack-protector-strong -Wformat -Wformat-security -fstack-clash-protection -fcf-protection -o /tmp/ccn8Dixg.s
GNU C17 (Ubuntu 9.4.0-1ubuntu1~20.04.2) version 9.4.0 (x86_64-linux-gnu)
        compiled by GNU C version 9.4.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
ignoring nonexistent directory "/usr/local/include/x86_64-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/9/include-fixed"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/9/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
GNU C17 (Ubuntu 9.4.0-1ubuntu1~20.04.2) version 9.4.0 (x86_64-linux-gnu)
        compiled by GNU C version 9.4.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
Compiler executable checksum: 01da938ff5dc2163489aa33cb3b747a7
COLLECT_GCC_OPTIONS='-v' '-o' 'CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o' '-c' '-mtune=generic' '-march=x86-64'
 as -v --64 -o CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o /tmp/ccn8Dixg.s
GNU assembler version 2.34 (x86_64-linux-gnu) using BFD version (GNU Binutils for Ubuntu) 2.34
COMPILER_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/
LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-v' '-o' 'CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o' '-c' '-mtune=generic' '-march=x86-64'
Linking C executable cmTC_4cacc
/usr/bin/cmake -E cmake_link_script CMakeFiles/cmTC_4cacc.dir/link.txt --verbose=1
/usr/bin/cc     -v CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o  -o cmTC_4cacc 
Using built-in specs.
COLLECT_GCC=/usr/bin/cc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:hsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.4.0-1ubuntu1~20.04.2' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-9QDOt0/gcc-9-9.4.0/debian/tmp-nvptx/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.2) 
COMPILER_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/
LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-v' '-o' 'cmTC_4cacc' '-mtune=generic' '-march=x86-64'
 /usr/lib/gcc/x86_64-linux-gnu/9/collect2 -plugin /usr/lib/gcc/x86_64-linux-gnu/9/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper -plugin-opt=-fresolution=/tmp/ccw75NfV.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s --build-id --eh-frame-hdr -m elf_x86_64 --hash-style=gnu --as-needed -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie -z now -z relro -o cmTC_4cacc /usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/Scrt1.o /usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/9/crtbeginS.o -L/usr/lib/gcc/x86_64-linux-gnu/9 -L/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/9/../../.. CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o -lgcc --push-state --as-needed -lgcc_s --pop-state -lc -lgcc --push-state --as-needed -lgcc_s --pop-state /usr/lib/gcc/x86_64-linux-gnu/9/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crtn.o
COLLECT_GCC_OPTIONS='-v' '-o' 'cmTC_4cacc' '-mtune=generic' '-march=x86-64'
make[1]: Leaving directory '/home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp'



Parsed C implicit include dir info from above output: rv=done
  found start of include info
  found start of implicit include info
    add: [/usr/lib/gcc/x86_64-linux-gnu/9/include]
    add: [/usr/local/include]
    add: [/usr/include/x86_64-linux-gnu]
    add: [/usr/include]
  end of search list found
  collapse include dir [/usr/lib/gcc/x86_64-linux-gnu/9/include] ==> [/usr/lib/gcc/x86_64-linux-gnu/9/include]
  collapse include dir [/usr/local/include] ==> [/usr/local/include]
  collapse include dir [/usr/include/x86_64-linux-gnu] ==> [/usr/include/x86_64-linux-gnu]
  collapse include dir [/usr/include] ==> [/usr/include]
  implicit include dirs: [/usr/lib/gcc/x86_64-linux-gnu/9/include;/usr/local/include;/usr/include/x86_64-linux-gnu;/usr/include]


Parsed C implicit link information from above output:
  link line regex: [^( *|.*[/\])(ld|CMAKE_LINK_STARTFILE-NOTFOUND|([^/\]+-)?ld|collect2)[^/\]*( |$)]
  ignore line: [Change Dir: /home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp]
  ignore line: []
  ignore line: [Run Build Command(s):/usr/bin/make cmTC_4cacc/fast && /usr/bin/make -f CMakeFiles/cmTC_4cacc.dir/build.make CMakeFiles/cmTC_4cacc.dir/build]
  ignore line: [make[1]: Entering directory '/home/gskenned/repos/forks/build/CMakeFiles/CMakeTmp']
  ignore line: [Building C object CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o]
  ignore line: [/usr/bin/cc   -v -o CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o   -c /usr/share/cmake-3.16/Modules/CMakeCCompilerABI.c]
  ignore line: [Using built-in specs.]
  ignore line: [COLLECT_GCC=/usr/bin/cc]
  ignore line: [OFFLOAD_TARGET_NAMES=nvptx-none:hsa]
  ignore line: [OFFLOAD_TARGET_DEFAULT=1]
  ignore line: [Target: x86_64-linux-gnu]
  ignore line: [Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.4.0-1ubuntu1~20.04.2' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c ada c++ go brig d fortran objc obj-c++ gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32 m64 mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-9QDOt0/gcc-9-9.4.0/debian/tmp-nvptx/usr hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu]
  ignore line: [Thread model: posix]
  ignore line: [gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.2) ]
  ignore line: [COLLECT_GCC_OPTIONS='-v' '-o' 'CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o' '-c' '-mtune=generic' '-march=x86-64']
  ignore line: [ /usr/lib/gcc/x86_64-linux-gnu/9/cc1 -quiet -v -imultiarch x86_64-linux-gnu /usr/share/cmake-3.16/Modules/CMakeCCompilerABI.c -quiet -dumpbase CMakeCCompilerABI.c -mtune=generic -march=x86-64 -auxbase-strip CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o -version -fasynchronous-unwind-tables -fstack-protector-strong -Wformat -Wformat-security -fstack-clash-protection -fcf-protection -o /tmp/ccn8Dixg.s]
  ignore line: [GNU C17 (Ubuntu 9.4.0-1ubuntu1~20.04.2) version 9.4.0 (x86_64-linux-gnu)]
  ignore line: [        compiled by GNU C version 9.4.0  GMP version 6.2.0  MPFR version 4.0.2  MPC version 1.1.0  isl version isl-0.22.1-GMP]
  ignore line: []
  ignore line: [GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072]
  ignore line: [ignoring nonexistent directory "/usr/local/include/x86_64-linux-gnu"]
  ignore line: [ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/9/include-fixed"]
  ignore line: [ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include"]
  ignore line: [#include "..." search starts here:]
  ignore line: [#include <...> search starts here:]
  ignore line: [ /usr/lib/gcc/x86_64-linux-gnu/9/include]
  ignore line: [ /usr/local/include]
  ignore line: [ /usr/include/x86_64-linux-gnu]
  ignore line: [ /usr/include]
  ignore line: [End of search list.]
  ignore line: [GNU C17 (Ubuntu 9.4.0-1ubuntu1~20.04.2) version 9.4.0 (x86_64-linux-gnu)]
  ignore line: [        compiled by GNU C version 9.4.0  GMP version 6.2.0  MPFR version 4.0.2  MPC version 1.1.0  isl version isl-0.22.1-GMP]
  ignore line: []
  ignore line: [GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072]
  ignore line: [Compiler executable checksum: 01da938ff5dc2163489aa33cb3b747a7]
  ignore line: [COLLECT_GCC_OPTIONS='-v' '-o' 'CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o' '-c' '-mtune=generic' '-march=x86-64']
  ignore line: [ as -v --64 -o CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o /tmp/ccn8Dixg.s]
  ignore line: [GNU assembler version 2.34 (x86_64-linux-gnu) using BFD version (GNU Binutils for Ubuntu) 2.34]
  ignore line: [COMPILER_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/]
  ignore line: [LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../:/lib/:/usr/lib/]
  ignore line: [COLLECT_GCC_OPTIONS='-v' '-o' 'CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o' '-c' '-mtune=generic' '-march=x86-64']
  ignore line: [Linking C executable cmTC_4cacc]
  ignore line: [/usr/bin/cmake -E cmake_link_script CMakeFiles/cmTC_4cacc.dir/link.txt --verbose=1]
  ignore line: [/usr/bin/cc     -v CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o  -o cmTC_4cacc ]
  ignore line: [Using built-in specs.]
  ignore line: [COLLECT_GCC=/usr/bin/cc]
  ignore line: [COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper]
  ignore line: [OFFLOAD_TARGET_NAMES=nvptx-none:hsa]
  ignore line: [OFFLOAD_TARGET_DEFAULT=1]
  ignore line: [Target: x86_64-linux-gnu]
  ignore line: [Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.4.0-1ubuntu1~20.04.2' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c ada c++ go brig d fortran objc obj-c++ gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32 m64 mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-9QDOt0/gcc-9-9.4.0/debian/tmp-nvptx/usr hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu]
  ignore line: [Thread model: posix]
  ignore line: [gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.2) ]
  ignore line: [COMPILER_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/]
  ignore line: [LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/9/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/9/../../../:/lib/:/usr/lib/]
  ignore line: [COLLECT_GCC_OPTIONS='-v' '-o' 'cmTC_4cacc' '-mtune=generic' '-march=x86-64']
  link line: [ /usr/lib/gcc/x86_64-linux-gnu/9/collect2 -plugin /usr/lib/gcc/x86_64-linux-gnu/9/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper -plugin-opt=-fresolution=/tmp/ccw75NfV.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s --build-id --eh-frame-hdr -m elf_x86_64 --hash-style=gnu --as-needed -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie -z now -z relro -o cmTC_4cacc /usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/Scrt1.o /usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/9/crtbeginS.o -L/usr/lib/gcc/x86_64-linux-gnu/9 -L/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/9/../../.. CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o -lgcc --push-state --as-needed -lgcc_s --pop-state -lc -lgcc --push-state --as-needed -lgcc_s --pop-state /usr/lib/gcc/x86_64-linux-gnu/9/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crtn.o]
    arg [/usr/lib/gcc/x86_64-linux-gnu/9/collect2] ==> ignore
    arg [-plugin] ==> ignore
    arg [/usr/lib/gcc/x86_64-linux-gnu/9/liblto_plugin.so] ==> ignore
    arg [-plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper] ==> ignore
    arg [-plugin-opt=-fresolution=/tmp/ccw75NfV.res] ==> ignore
    arg [-plugin-opt=-pass-through=-lgcc] ==> ignore
    arg [-plugin-opt=-pass-through=-lgcc_s] ==> ignore
    arg [-plugin-opt=-pass-through=-lc] ==> ignore
    arg [-plugin-opt=-pass-through=-lgcc] ==> ignore
    arg [-plugin-opt=-pass-through=-lgcc_s] ==> ignore
    arg [--build-id] ==> ignore
    arg [--eh-frame-hdr] ==> ignore
    arg [-m] ==> ignore
    arg [elf_x86_64] ==> ignore
    arg [--hash-style=gnu] ==> ignore
    arg [--as-needed] ==> ignore
    arg [-dynamic-linker] ==> ignore
    arg [/lib64/ld-linux-x86-64.so.2] ==> ignore
    arg [-pie] ==> ignore
    arg [-znow] ==> ignore
    arg [-zrelro] ==> ignore
    arg [-o] ==> ignore
    arg [cmTC_4cacc] ==> ignore
    arg [/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/Scrt1.o] ==> ignore
    arg [/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crti.o] ==> ignore
    arg [/usr/lib/gcc/x86_64-linux-gnu/9/crtbeginS.o] ==> ignore
    arg [-L/usr/lib/gcc/x86_64-linux-gnu/9] ==> dir [/usr/lib/gcc/x86_64-linux-gnu/9]
    arg [-L/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu] ==> dir [/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu]
    arg [-L/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib] ==> dir [/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib]
    arg [-L/lib/x86_64-linux-gnu] ==> dir [/lib/x86_64-linux-gnu]
    arg [-L/lib/../lib] ==> dir [/lib/../lib]
    arg [-L/usr/lib/x86_64-linux-gnu] ==> dir [/usr/lib/x86_64-linux-gnu]
    arg [-L/usr/lib/../lib] ==> dir [/usr/lib/../lib]
    arg [-L/usr/lib/gcc/x86_64-linux-gnu/9/../../..] ==> dir [/usr/lib/gcc/x86_64-linux-gnu/9/../../..]
    arg [CMakeFiles/cmTC_4cacc.dir/CMakeCCompilerABI.c.o] ==> ignore
    arg [-lgcc] ==> lib [gcc]
    arg [--push-state] ==> ignore
    arg [--as-needed] ==> ignore
    arg [-lgcc_s] ==> lib [gcc_s]
    arg [--pop-state] ==> ignore
    arg [-lc] ==> lib [c]
    arg [-lgcc] ==> lib [gcc]
    arg [--push-state] ==> ignore
    arg [--as-needed] ==> ignore
    arg [-lgcc_s] ==> lib [gcc_s]
    arg [--pop-state] ==> ignore
    arg [/usr/lib/gcc/x86_64-linux-gnu/9/crtendS.o] ==> ignore
    arg [/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crtn.o] ==> ignore
  collapse library dir [/usr/lib/gcc/x86_64-linux-gnu/9] ==> [/usr/lib/gcc/x86_64-linux-gnu/9]
  collapse library dir [/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu] ==> [/usr/lib/x86_64-linux-gnu]
  collapse library dir [/usr/lib/gcc/x86_64-linux-gnu/9/../../../../lib] ==> [/usr/lib]
  collapse library dir [/lib/x86_64-linux-gnu] ==> [/lib/x86_64-linux-gnu]
  collapse library dir [/lib/../lib] ==> [/lib]
  collapse library dir [/usr/lib/x86_64-linux-gnu] ==> [/usr/lib/x86_64-linux-gnu]
  collapse library dir [/usr/lib/../lib] ==> [/usr/lib]
  collapse library dir [/usr/lib/gcc/x86_64-linux-gnu/9/../../..] ==> [/usr/lib]
  implicit libs: [gcc;gcc_s;c;gcc;gcc_s]
  implicit dirs: [/usr/lib/gcc/x86_64-linux-gnu/9;/usr/lib/x86_64-linux-gnu;/usr/lib;/lib/x86_64-linux-gnu;/lib]
  implicit fwks: []
```
