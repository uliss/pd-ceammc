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

#ifndef DEF_LSD_TOOLS
#define DEF_LSD_TOOLS

#include "math.h"
#include <complex>
#include <stdio.h>
#include <vector>

class Tools
{
public:
	Tools();
	template<typename Type> static Type clip(Type aValue, Type aMinimum = 0, Type aMaximum = 1)
	{
		if(aValue < aMinimum)
			aValue = aMinimum;
		else if(aValue > aMaximum)
			aValue = aMaximum;
		return aValue;
	}
};

#endif