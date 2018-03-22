## CICM WRAPPER

### Presentation

The CICM wrapper is a C interface for [Tcl/Tk](https://www.tcl.tk/ "Tcl/Tk") and [Pure Data](http://msp.ucsd.edu/software.html "PD") that aims to facilitate the creation of objects for Pure Data. One of the main axis is the creation of graphical user interfaces. It also facilitates the implementation of dynamic multisignal processing and it improves the compatibility with Max.

- Graphics :
	- Layers system.
	- Matrix translation and rotation.
	- Bezier curves.
	- Multi shapes
	- Text layout
	- Clipping of the drawing.
	- Colors hsl/rgba and hexadecimal
	- etc.
- Interractions :
	- Full mouse notification
	- Full keyboard notification
	- Cross platform modifiers
	- etc.
- Object :
	- Proxy inlets.
	- Attributes
	- Automatic properties window
	- Click and drag to resize the objects
	- Special methods for background, border and size
	- Dynamic inlets/outlets
	- Dsp method simplified
	- Perform method simplified
	- etc.  

Macros to convert code (event GUI) from Max to Pd and from PD to Max and a lot of other stuff, I can't enumerate everything !

### Repository :

[GitHub](https://github.com/CICM/CicmWrapper "GitHub")

### Documentation :

[Documentation and examples](http://cicm.github.io/CicmWrapper "Documentation")

### Compilation :

	./autogen.sh (if needed)
	./configure (useful options --with-pd=</path/to/pd>)
	make
	make install (optional)

### Projects :

- [Cream Library](https://github.com/CICM/CreamLibrary "Cream").
- [Hoa Library](https://github.com/CICM/HoaLibrary-Light "Hoa").

### Author :

Pierre Guillot

### Thanks :

Antoine Villeret for Autotools and feedbacks.  
Thomas Le Meur for the corrections.  
Miller Puckette for Pure Data.

### Licence : 

Copyright (C) 2013-2015 Pierre Guillot - CICM - Universite Paris 8  
The Cream Library in under the [BSD2 License](http://opensource.org/licenses/BSD-2-Clause "BSD2").


