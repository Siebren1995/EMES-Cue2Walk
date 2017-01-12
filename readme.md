Readme entry for EMES-Parkinson github repository.

The following steps are required before you can compile and run software for the nRF51822

Download eclipse CDT mars 2
https://eclipse.org/cdt/downloads.php
should create a folder : Eclipse

Download arm-gcc-toolchain (5_3_2016_q1)
https://launchpad.net/gcc-arm-embedded/+download
place in separate folder and add to path (google: mac path toolchain)

Download J-Link plugin for eclipse:
http://gnuarmeclipse.github.io/plugins/install/
Uncheck the STM template (might give errors otherwise)

https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF51822
download latest SDK

Eclipse->new->Project->project with existing makefile

go to file component->toolchain->gcc->Makefile.posix and add the path to the toolchain

rigth-click project, properties->c/c++ build->and set the build directory to your example +
the package (pca10028)
Bluetooth Soft version(110,120 or 130)
armgcc

Then check in the makefile in that folder whether the nRF(51422) or nRF(51822) is supported
our chip is 51822, but the 51422 is the same chip with ANT protocol.
enjoy
For more info:
www.github.com/wemaketotem

