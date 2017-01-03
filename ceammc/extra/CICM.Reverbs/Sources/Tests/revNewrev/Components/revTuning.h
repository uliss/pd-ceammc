/*
 *
 * Copyright (C) 2012 Pierre Guillot, Universite Paris 8
 * 
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Library General Public License as published 
 * by the Free Software Foundation; either version 2 of the License.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License 
 * along with this library; if not, write to the Free Software Foundation, 
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#ifndef DEF_TUNING
#define DEF_TUNING

const long	numcombs		= 8;
const long	numallpasses	= 4;
const double	muted		= 0;
const double	fixedgain	= 0.015;
const double scalewet		= 3;
const double scaledry		= 2;
const double scaledamp		= 0.4;
const double scaleroom		= 0.28;
const double offsetroom		= 0.7;
const double initialroom	= 0.5;
const double initialdamp	= 0.5;
const double initialwet		= 1./scalewet;
const double initialdry		= 0;
const double initialwidth	= 1;
const double initialmode	= 0;
const double freezemode		= 0.5;
const long	stereospread	= 23;

// These values assume 44.1KHz sample rate
// they will probably be OK for 48KHz sample rate
// but would need scaling for 96KHz (or other) sample rates.
// The values were obtained by listening tests.
const long combtuningL1		= 1116;
const long combtuningR1		= 1116+stereospread;
const long combtuningL2		= 1188;
const long combtuningR2		= 1188+stereospread;
const long combtuningL3		= 1277;
const long combtuningR3		= 1277+stereospread;
const long combtuningL4		= 1356;
const long combtuningR4		= 1356+stereospread;
const long combtuningL5		= 1422;
const long combtuningR5		= 1422+stereospread;
const long combtuningL6		= 1491;
const long combtuningR6		= 1491+stereospread;
const long combtuningL7		= 1557;
const long combtuningR7		= 1557+stereospread;
const long combtuningL8		= 1617;
const long combtuningR8		= 1617+stereospread;
const long allpasstuningL1	= 556;
const long allpasstuningR1	= 556+stereospread;
const long allpasstuningL2	= 441;
const long allpasstuningR2	= 441+stereospread;
const long allpasstuningL3	= 341;
const long allpasstuningR3	= 341+stereospread;
const long allpasstuningL4	= 225;
const long allpasstuningR4	= 225+stereospread;

#endif


