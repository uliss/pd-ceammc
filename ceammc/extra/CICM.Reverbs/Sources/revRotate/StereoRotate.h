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

#ifndef DEF_ROTATE
#define DEF_ROTATE

#include "cicmTools.h"

class StereoRotate
{
protected:
	double	m_phase;
	double	m_angle_cos;
	double	m_angle_sin;
	long	m_vector_size;
public:

	StereoRotate()
	{
		setPhase(0.);
		setVectorSize(0.);
	}
	
	void setVectorSize(long aVectorSize)
	{
		m_vector_size = Tools::clip_power_of_two(aVectorSize);
	}
	
	void setPhase(double aPhase)
	{
		m_phase = Tools::clip(aPhase, 0., 1.);
		m_angle_cos = cos(m_phase * CICM_PI2);
		m_angle_sin = sin(m_phase * CICM_PI2);
	}
	
	template<typename Type> inline void process(Type* anInputsVector, Type* anOutputsVector)
	{
		anOutputsVector[0] = anInputsVector[0] * m_angle_cos - anInputsVector[1] * m_angle_sin;
		anOutputsVector[1] = anInputsVector[0] * m_angle_sin + anInputsVector[1] * m_angle_cos;
	}
	
	template<typename Type> inline void process(Type* anInputsVector, Type* anOutputsVector, Type aPhase)
	{
		setPhase(aPhase);
		anOutputsVector[0] = anInputsVector[0] * m_angle_cos - anInputsVector[1] * m_angle_sin;
		anOutputsVector[1] = anInputsVector[0] * m_angle_sin + anInputsVector[1] * m_angle_cos;
	}
	
	template<typename Type> inline void process(Type** anInputsVector, Type** anOutputsVector)
	{
		for(int i = 0; i < m_vector_size; i++)
		{
			anOutputsVector[0][i] = anInputsVector[0][i] * m_angle_cos - anInputsVector[1][i] * m_angle_sin;
			anOutputsVector[1][i] = anInputsVector[0][i] * m_angle_sin + anInputsVector[1][i] * m_angle_cos;

		}
	}
	
	template<typename Type> inline void process(Type** anInputsVector, Type** anOutputsVector, Type* aPhase)
	{
		for(int i = 0; i < m_vector_size; i++)
		{
			setPhase(aPhase[i]);
			anOutputsVector[0][i] = anInputsVector[0][i] * m_angle_cos - anInputsVector[1][i] * m_angle_sin;
			anOutputsVector[1][i] = anInputsVector[0][i] * m_angle_sin + anInputsVector[1][i] * m_angle_cos;		
		}
	}
	
	double getPhase()
	{
		return m_phase;
	}
	
	long getVectorSize()
	{
		return m_vector_size;
	}
	~StereoRotate(){};

};



#endif