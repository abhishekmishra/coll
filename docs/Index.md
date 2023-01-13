# Coll Documentation                {#mainpage}

[TOC]

# Introduction
**NOTE: This library is still under development. The public API is unstable till the library reaches beta status. There will be breaking changes in public API if needed.**

**COLL** is a C collections library. It provides two data structures at the moment:
1. ArrayList: Documented in the header [coll_arraylist.h](@ref coll_arraylist.h) 
2. Naive Map/Table based on ArrayList: Documented in the header [coll_arraylist_map.h](@ref coll_arraylist_map.h) 

# Getting Started

## Build the package using CMake
The Makefile in the source folder contains targets for useful CMake commands.
You can use the genbuild and build targets to create the CMake build folder,
and then run the build for your platform.

```bash
$ make genbuild build
```
## Use the library in your code.
A library named coll (with appropriate prefix/suffix for your platform) will
be generated in the ./build folder.

Use this library and add the src folder to your include folders during
compilation.

## Usage
See the test programs under the `./test` folder in the source code.