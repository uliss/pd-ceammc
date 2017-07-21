[soundtouch~] is a Pure Data implementation of Olli Parviainen's SoundTouch library, ported by Katja Vetter. The class is implemented as a real-time pitch shifter, converting an audio stream to different pitch.

[soundtouch~] version 0.9, Aug 2011

for SoundTouch version 1.6 and Pure Data version 0.42


---------------------------------------------------------------------------------------------------
contents of this package:

- soundtouch~-help.pd (help patch)
- soundtouch~.dll (Windows binary 32 bit Intel)
- soundtouch~.pd_darwin (MacOSX fat binary: PPC, Intel32/64)
- soundtouch~.pd_linux (Linux 32 bit binary)
- source codes and makefile to build soundtouch~ binary


---------------------------------------------------------------------------------------------------
how to use:

Copy directory 'soundtouch~' to a convenient location on your computer. Start your Pd or Pd-extended and load patch soundtouch~-help.pd to learn about it's use and settings. 

You can copy or move the helpfile plus the executable for your platform into a directory in Pd's searchpath, or alternatively you can add the soundtouch~ directory to Pd's path using the preferences menu. 


---------------------------------------------------------------------------------------------------
how to build with make command:

To build soundtouch~ from command line, first edit the makefile in the src directory to your needs and save. On the command line, cd to the src directory and type 'make'. This builds the executable and creates a copy in directory soundtouch~, where you can test it with the helpfile. 

Makefile builds were tested on Debian Squeeze 32 bit, MinGW on Windows XP, and OSX 10.5. For 64 bit builds, option -fpic must probably be added to the Cflags.

The SoundTouch C++ library offers a preprocessor definition to prevent clicks when changing pitch factor. It is called PREVENT_CLICK_AT_RATE_CROSSOVER and lives in file STTypes.h. If [soundtouch~] is compiled with #define PREVENT_CLICK_AT_RATE_CROSSOVER 1, clicks are gone, but the latency will double. For binary distributions of [soundtouch~] I have opted for the clicks rather than the increased latency. The SoundTouch API does not offer a way to avoid them both.


---------------------------------------------------------------------------------------------------
tests, builds and bug reports:

Notice that clicks and cracks during pitch-factor changes are inherent to the way soundtouch~ binaries were built for this distribution, and a compile-time option exists to trade these artifacts for a longer latency time (see section 'how to build'). Please do not send me bug reports about this, but instead recompile the class according to your preference.

I have built and tested soundtouch~ on the following systems:

- Debian Squeeze 32 bit on Panasonic Toughbook CF-74 Intel Core Duo 1.87 GHz
- Windows XP professional on Panasonic Toughbook CF-74 Intel Core Duo 1.87 GHz
- OSX 10.5 on MacBook Intel Core 2 Duo 2 GHz

If you find a bug or flaw in soundtouch~ for Pure Data, please send me a detailed notice, including OS/hardware info. If you have a working executable for a platform not yet included in this distribution, please send it to me so I can add it.

katjavetter@gmail.com


---------------------------------------------------------------------------------------------------
license info:

SoundTouch library is written/copyrighted by Olli Parviainen and distributed under LGPL. From this library, soundtouch~ for Pure Data uses the following files included in this distribution:

AAFilter.cpp
AAFilter.h
cpu_detect_x86_gcc.cpp
cpu_detect.h
FIFOSampleBuffer.cpp
FIFOSampleBuffer.h
FIFOSamplePipe.h
FIRFilter.cpp
FIRFilter.h
mmx_optimized.cpp
RateTransposer.cpp
RateTransposer.h
SoundTouch.cpp
SoundTouch.h
sse_optimized.cpp
SSTypes.h
TDStretch.cpp
TDStretch.h


The Pure Data port is written/copyrighted by Katja Vetter 2010/2011 and distributed under BSD 3-clause license. The port is implemented in the following files:

soundtouch~.cpp and 
soundtouch~.h 


Pure Data is the work of Miller Puckette and others. The Pure Data core code and API are distributed under BSD license. The API header file is included in this distribution:

m_pd.h


---------------------------------------------------------------------------------------------------
more info:

Olli Parviainen's pages for the SoundTouch library, it's method, and pitch shifting in general:

http://www.surina.net/soundtouch/
http://www.surina.net/article/time-and-pitch-scaling.html

My pages on pitch shifting, and on soundtouch~ for Pure Data:

http://www.katjaas.nl/pitchshift/pitchshift.html
http://www.katjaas.nl/pitchshift/soundtouch%7E.html


---------------------------------------------------------------------------------------------------
Katja Vetter, August 2011
katjavetter@gmail.com
www.katjaas.nl



