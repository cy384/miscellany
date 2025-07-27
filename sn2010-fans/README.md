sn2010-fans
===========
FreeCAD 3D printable top for Mellanox SN2010 switches, allowing the use of 120mm fans instead of the extremely loud 40mm fans.  Will probably also fit the SN2100, but note that that model has a heatsink on the bottom of the main PCB and you'll need to run the fans much faster, or modify this design to get more air flow below.

Three fans is probably overkill, you could probably just do the front and rear fans.  The CPU doesn't run hot, but the switch chip and the power supplies need the cooling.

Parts needed:

* one 3D print of each file here.  Print the top pieces flat side down, no supports needed.  Pick either the fan or no-fan version of the middle part.
* fan controller (I'm using a "MEIRIYFA 3 Pin 4 Pin Fan Adapter PWM PC Chassis Cooling Fan Hub 8 Way Splitter 12V Speed Controller with 6 Pin Power Port" from Amazon)
* three 120mm fans

You'll need to power the fan controller.  I recommend cutting the cables off the stock fans and soldering those to the controller.  If you solder directly to the 12V output of one of the power supplies, note that the fans will only be on if that PSU is plugged in.  If you're bold you can wire the fan tachometer output, too.

I didn't nail down the pinout on the fans, you're on your own.

Caveat emptor!  The power supplies are not covered inside of these, you are responsible for your safety!  Consider printing these in a high temperature plastic!
