# Multi-Threaded DU

The purpose of this project is to provide a faster `du` command for usage in file systems storing massive amounts of data. 
The project is currently implemented using C++17 but the plan is to move to Rust or C over time. 

**Version:** 0.0.1 **STATUS:** ALPHA

I consider any version number below 1.y.z in *alpha* unless anything else is marked. 

## C++ Dependencies

Building this library requires you to have an up-to-date version of clang++ or g++ with `std::filesystem` moved from fully experimental.  

# Build

To build you need make and clang++ v10+.
Run `make`. 
This will create a folder `bin/` where you will find the binary `mdu`.

# LICENSE

This tool is available under the BSD 3 License, for more information see the include LICENSE file 

## Acknowledgements 

Currently I am using [ThreadPool](https://github.com/progschj/ThreadPool) by [Jakob Progsch](https://github.com/progschj).
Which is released under the [zlib license](https://github.com/progschj/ThreadPool/blob/master/COPYING). 
Many thanks to Jakob for this library. 

