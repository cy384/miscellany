retro68 demos
-------------
This is some sample code for vintage Mac development:

* `ot-tcp-demo.c`: uses OpenTransport to send a HTTP request over TCP
* `ot-udp-demo.c`: uses OpenTransport to send a UDP packet
* `mactcp-udp-demo.c`: uses MacTCP to send a UDP packet

build directions
----------------
Set up Retro68 as per its instructions.

Rename the file you want to build as `demo.c`

* `mkdir build`
* `cd build`

* for m68k: `cmake .. -DCMAKE_TOOLCHAIN_FILE=your/path/to/Retro68-build/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake`

* for PPC: `cmake .. -DCMAKE_TOOLCHAIN_FILE=your/path/to/Retro68-build/toolchain/powerpc-apple-macos/cmake/retroppc.toolchain.cmake`

* `make`

The result will be the standard Retro68 pile of files, including the `demo.dsk` disk image, which can be dumped onto a real disk or given to an emulator.

