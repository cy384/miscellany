build directions
----------------
Set up Retro68 as per its instructions.

In CMakeLists.txt, comment/uncomment the lines to choose PPC vs. m68k.

* `mkdir build`
* `cd build`

* for m68k: `cmake .. -DCMAKE_TOOLCHAIN_FILE=your/path/to/Retro68-build/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake`

* for PPC: `cmake .. -DCMAKE_TOOLCHAIN_FILE=your/path/to/Retro68-build/toolchain/powerpc-apple-macos/cmake/retroppc.toolchain.cmake`

* `make`

The result will be the standard Retro68 pile of files, including the `tcp-demo.dsk` disk image, which can be dumped onto a real disk or given to an emulator.

