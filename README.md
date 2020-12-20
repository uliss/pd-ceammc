[![Build Status](https://travis-ci.org/uliss/pure-data.svg?branch=ceammc)](https://travis-ci.org/uliss/pure-data)
[![Build status](https://ci.appveyor.com/api/projects/status/mfc2ahj8ttexapsj?svg=true)](https://ci.appveyor.com/project/uliss/pure-data)
[![Coverage Status](https://coveralls.io/repos/github/uliss/pure-data/badge.svg?branch=ceammc)](https://coveralls.io/github/uliss/pure-data?branch=ceammc)
[![Snap Status](https://build.snapcraft.io/badge/uliss/pure-data.svg)](https://build.snapcraft.io/user/uliss/pure-data)

CEAMMC PureData distribution 2020.12
------------------------------------

General purpose Pd distribution and library used for work and education purposes in Centre of electoacoustic music of Moscow Conservatory (CEAMMC) and ZIL-electro studio.

This repository is an assembly of Puredata that includes the CEAMMC library.

Key concepts
------------

   - All the externals are grouped by their function. This is different from the traditional approach but this aims to simplify the learning process and makes the language itself more clear.
   - Higher level set of objects for education and handy use in work process: oscillators, filters etc.
   - Library adds several features to basic Pd API. It is written in C++ and covered with tests.
   - Inclusion of some modern techniques. Library v0.8 includes comprehensive set of objects that work with lists and include some concepts from functional programming.

ChangeLog:
----------

see ceammc/CHANGELOG.md for more details
[ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)


this work is based on / includes:
---------------------------------
   - Pd vanilla (this repository)
   - CICM Wrapper library (https://github.com/CICM/CicmWrapper)
   - A-Chaos library - https://github.com/s373/ofxAChaosLib
   - FAUSTDSP (http://faust.grame.fr)
   - pddoc (https://github.com/uliss/pddoc)
   - muparser (https://beltoforion.de/en/muparser/)
   - FluidSynth (https://www.fluidsynth.org/)
   - aubio (https://aubio.org/)
   - HoaLibrary (https://github.com/CICM/HoaLibrary-Light)
   - STK (https://ccrma.stanford.edu/software/stk/)
   - autotune object
   - comport object
   - FFTease
   - lyonpotpourri
   - re2
   - glitch
   - SoundTouch
   - SoxR
   - flext


### Authors
Serge Poltavsky
Alex Nadzharov

### License
GPL3

[Original README](README_ORIGINAL.md)
