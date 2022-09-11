miscellany
==========

This is a collection of little snippets of code or small projects that don't quite deserve their own repo.

Most of these are toys or proofs of concept, don't judge my code too harshly!

Anything not otherwise labeled is licensed under the GPL; see LICENSE for details.

m90q-cx353
----------
FreeCAD 3D printable bracket to hold a Mellanox CX353A card in a Lenovo M90q Gen 2 SFF desktop.  Just pop it in.

softride-bracket
----------------
FreeCAD 3D printable bracket to hold onto the end of a Softride bike's beam, and has a gopro-style mount on the end.  Uses an M5 bolt and hex nut.  This is intended for mounting a rear bike light.

matias-ergo-pro-foot
--------------------
FreeCAD-designed 3D printable replacement foot for a Matias Ergo Pro keyboard (ideally, print in TPU or similar rubbery material).  Friction fit.

mac-metal-tray-scsi2sdv6
------------------------
FreeCAD-designed 3D printable adapter to mount a SCSI2SD v6 PCB onto the metal-style drive trays (mine is P/N 805-0425).  Uses the original standard drive screws to connect to the tray, and M3 screws for the SCSI2SD.

retro68-demos
-------------
These are some demos for doing networking on vintage Macs using the Retro68 toolchain.  Currently it's got demos for Open Transport (TCP and UDP) and MacTCP (UDP).  They've been tested on Mac OS 9.2, but should work on any version with Open Transport or MacTCP, respectively (note that Open Transport also emulates the MacTCP driver interface, for compatibility).

I'm publishing this since there's a real dearth of modern code for vintage macs, and hopefully it can give anyone interested an easy starting point.

moskva-knob
-----------
An OpenSCAD 3D printable replacement film advance knob for the Moskva 5 camera.  The original film advance is very low profile and has vicious knurling, and the mechanism requires a bizarre amount of force.  This design is much easier to grip and rotate.

industar-lens-cap
-----------------
OpenSCAD front lens cap for the Industar 69 half frame lens.

1key-pcbs
---------
KiCAD experiments in the design of ultra-minimal single-key PCBs for mechanical keyboards (see upcoming blog post for details), with a goal of minimizing construction labor and cost.

cherry-mx-lockout
-----------------
OpenSCAD 3D printable clip that sits on the top of Cherry MX switches, under a 1U keycap, to block key presses.

atreus-hacks
------------
A dump of the hardware and software used for my second generation Atreus keyboards.  Everything is ugly and not cleaned up to the standards I would prefer.  Many flaws: if you want (or need) to use these, definitely contact me before using them.  Some known issues:

* PCB switch placement for thumb keys doesn't exactly match OpenSCAD plate holes
* Can't run QMK
* PCB bottom corners don't fit into the OpenSCAD case, so I've included a hacked version of the OpenSCAD case generator here
* Screw holes in the OpenSCAD case 3D print out too small
* Pin choices for the PCB are bad, accidentally used an LED pin and the battery power detection pin for the matrix

