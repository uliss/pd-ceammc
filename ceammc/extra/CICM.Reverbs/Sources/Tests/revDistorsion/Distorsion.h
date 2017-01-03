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

#ifndef DEF_DISTORSION
#define DEF_DISTORSION

#include "cicmTools.h"

class Distorsion
{
protected:
	long	m_vector_size;
public:

	Distorsion()
	{
		setVectorSize(0.);
	}
	
	void setVectorSize(long aVectorSize)
	{
		m_vector_size = Tools::clip_power_of_two(aVectorSize);
	}
	
	template<typename Type> inline Type process(Type anInput)
	{
		double value = Tools::clip(anInput, (Type)-1., (Type)1.);
		return 1.5 * value - 0.5 * value * value * value;
	}
	
	template<typename Type> inline void process(Type* anInputsVector, Type* anOutputsVector)
	{
		for(int i = 0; i < m_vector_size; i++)
		{
			double x = anInputsVector[i] * 0.686306;
			double a = 1 + exp (sqrt (fabs (x)) * -0.75);
			anOutputsVector[i] = (exp (x) - exp (-x * a)) / (exp (x) + exp (-x));

		}
	}

	
	long getVectorSize()
	{
		return m_vector_size;
	}
	~Distorsion(){};

};



#endif