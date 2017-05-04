[![Build Status](https://travis-ci.org/uliss/pure-data.svg?branch=ceammc)](https://travis-ci.org/uliss/pure-data)
[![Coverage Status](https://coveralls.io/repos/github/uliss/pure-data/badge.svg?branch=ceammc)](https://coveralls.io/github/uliss/pure-data?branch=ceammc)

CEAMMC Puredata distribution 2017.05
-----------------

General purpose Pd distribution and library used for work and education purposes in Centre of electoacoustic music of Moscow Conservatory (CEAMMC) and ZIL-electro studio.

This repository is an assembly of Puredata that includes the CEAMMC library.

Key concepts
------------

   - All the externals are grouped by their function. This is different from the traditional approach but this aims to simplify the learning process and makes the language itself more clear.
   - Higher level set of objects for education and handy use in work process: oscillators, filters etc.
   - Library adds several features to basic Pd API. It is written in C++ and covered with tests.
   - Inclusion of some modern techniques. Library v0.1 includes comprehensive set of objects that work with lists and include some concepts from functional programming.


Features: v0.2
--------------

 * Core objects:
   - list.* objects: 32 objects that cover different techniques to work with lists
   - list.apply_to, list.reduce objects introduce basic functional approach to list processing.
   - math.* : 28 objects covering math functions with 1 argument
   - vector.* : basic vector operations
   - basic JSON handling
   - system.* object for various system features
   - flow control: pass, reject objects; predicates objects
   - random number generators
   - property concept: non-UI objects may have properties that can be accessed after object creation. This includes special message and prop.get object for handling
 * Audio objects:
   - dynamics: compressor, limiter, soft clip
   - envelope generators (ar, adsr) and follower
   - filters: eq peak and shelf filters, dc block
   - oscillators and LFO with different waveforms
   - filter coefficient calculations, biquad filter, lp/hp/bp 2- and 4-pole filters
   - some basic effects - pitch/frequency shift, flanger, distortion
   - snd.file - object for importing sound files
 * GUI objects:
   - 8 new objects: knob, multiple slider box, 2d slider, keyboard, message display, scope and oscilloscope
   - breakpoint function object, working with envelope objects
   - Basic timeline objects: graphical cues in a patch
   - Includes several objects from CICM Cream Library
 * Extensions:
   - New objects toolbar (also works as edit mode indicator)
   - Saving subpatch as new patch
   - Multiple undo (beta)
   - Grid / Align to grid (beta)


Roadmap v0.3+:
-------------

 * Core objects:
   - objects working with strings and string data type
   - math.* - add functions with multiple arguments
   - Audio filters: biquad coefficients generators
 * GUI
   - new completely rewritten Qt-based GUI with MVC architecture
   - simplified object set
   - Zoom, grid, extended property handling, extended patching/clipboard/undo etc
   - embedded Python; API for python


this work is based on / includes:
---------------------------------
   - Pd vanilla (this repository)
   - CICM Wrapper library (https://github.com/CICM/CicmWrapper)
   - Cream library (https://github.com/CICM/CreamLibrary)
   - hcs library (https://puredata.info/downloads/hcs)
   - comport object
   - A-Chaos library - https://github.com/s373/ofxAChaosLib
   - FAUSTDSP (http://faust.grame.fr)
   - pddoc (https://github.com/uliss/pddoc)


### Authors
Serge Poltavsky
Alex Nadzharov

### License
GPL3

[Original README](README_ORIGINAL.md)
