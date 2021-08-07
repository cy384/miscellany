wombat-hacking
--------------
Folder for whatever bits of code I write related to the "WOMBAT" motherboard for classic macs (Centris 650, Quadra 650, Quadra 800).  Currently, a crude program ("ultra-wombat-hax-tool") for reading and displaying djmemc memory controller registers.  This is useful for overclocking-related experimentation.

build directions
----------------
Set up Retro68 as per its instructions.

* `mkdir build`
* `cd build`
* `cmake .. -DCMAKE_TOOLCHAIN_FILE=your/path/to/Retro68-build/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake`
* `make`

Grab the .bin or the .dsk and run (only run on it a real WOMBAT machine).

