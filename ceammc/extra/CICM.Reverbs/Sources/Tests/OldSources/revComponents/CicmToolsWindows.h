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

//#define CICM_VDSP
//#define CICM_IPPS
#define CICM_FFTW_GSL
//#define CICM_FLOAT
#define CICM_DOUBLE

#ifndef DEF_CICM_TOOLS
#define DEF_CICM_TOOLS

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
using namespace std;
#include "cicmDefine.h"

#define TOOLS_PI 3.141592653589793238462643383279502884

class Tools
{
public:
	Tools();
	~Tools();
	template<typename Type> static Type clip(Type aValue, Type aMinimum = 0, Type aMaximum = 1)
	{
		if(aValue < aMinimum)
			aValue = aMinimum;
		else if(aValue > aMaximum)
			aValue = aMaximum;
		return aValue;
	}

	template<typename Type> static Type clip_min(Type aValue, Type aMinimum = 0)
	{
		if(aValue < aMinimum)
			aValue = aMinimum;
		return aValue;
	}

	template<typename Type> static Type clip_max(Type aValue, Type aMinimum = 0)
	{
		if(aValue < aMinimum)
			aValue = aMinimum;
		return aValue;
	}

	template<typename Type> static Type clip_power_of_two(Type aValue)
	{
		if(aValue < 2)
		aValue = 2;
		else if(aValue&(aValue-1) != 0)
		{
			while (aValue&(aValue-1) != 0) 
				aValue--;
		}
		return aValue;
	}

	template<typename Type> static Type log2(Type n)
	{
		return log((double)n) / log(2.);
	}
};

#endif