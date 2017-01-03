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

#ifndef DEF_WIDTH
#define DEF_WIDTH

#include "cicmTools.h"

class StereoWidth
{
protected:
	double	m_width;
	long	m_vector_size;
public:

	StereoWidth()
	{
		setWidth(1.);
		setVectorSize(0.);
	}
	
	void setVectorSize(long aVectorSize)
	{
		m_vector_size = Tools::clip_power_of_two(aVectorSize);
	}
	
	void setWidth(double aWidthValue)
	{
		m_width = 0.5 * Tools::clip(aWidthValue, 0., 1.);
	}
	
	template<typename Type> inline void process(Type* anInputsVector, Type* anOutputsVector)
	{
		Type m = (anInputsVector[0] + anInputsVector[1]) * 0.5;
		Type s = (anInputsVector[1] - anInputsVector[0]) * m_width;
		
		anOutputsVector[0] = m - s;
		anOutputsVector[1] = m + s;
	}
	
	template<typename Type> inline void process(Type* anInputsVector, Type* anOutputsVector, Type aWidthValue)
	{
		setWidth(aWidthValue);
		Type m = (anInputsVector[0] + anInputsVector[1]) * 0.5;
		Type s = (anInputsVector[1] - anInputsVector[0]) * m_width;
		
		anOutputsVector[0] = m - s;
		anOutputsVector[1] = m + s;
	}
	
	template<typename Type> inline void process(Type** anInputsVector, Type** anOutputsVector)
	{
		for(int i = 0; i < m_vector_size; i++)
		{
			Type m = (anInputsVector[0][i] + anInputsVector[1][i]) * 0.5;
			Type s = (anInputsVector[1][i] - anInputsVector[0][i]) * m_width;
		
			anOutputsVector[0][i] = m - s;
			anOutputsVector[1][i] = m + s;
		}
	}
	
	template<typename Type> inline void process(Type** anInputsVector, Type** anOutputsVector, Type* aWidthValue)
	{
		for(int i = 0; i < m_vector_size; i++)
		{
			setWidth(aWidthValue[i]);
			Type m = (anInputsVector[0][i] + anInputsVector[1][i]) * 0.5;
			Type s = (anInputsVector[1][i] - anInputsVector[0][i]) * m_width;
			
			anOutputsVector[0][i] = m - s;
			anOutputsVector[1][i] = m + s;
		}
	}
	
	double getWidth()
	{
		return m_width * 2.;
	}
	
	long getVectorSize()
	{
		return m_vector_size;
	}
	~StereoWidth(){};

};



#endif