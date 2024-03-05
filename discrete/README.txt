### HOW TO BUILD ####

The programs are build from 'C' source files using the CMake tool.
Make sure, you have a version of this tool installed that is equal or newer than version 3.14.

On the root of the source tree, run these two command

(1) Create the files that will call your local build system and 'C' compiler:
  $ cmake -S cprog -B build -DCMAKE_BUILD_TYPE=Release

(2) Compile the code:
  $ cmake --build build


After these steps, the executable files will be found in the directory ./build/apps/


To create executables that support debugging, you can create a separate build area and use it for compilation

(3) Create the 'Debug' version of files that will call your local build system and 'C' compiler:
  $ cmake -S cprog -B build-debug -DCMAKE_BUILD_TYPE=Debug
