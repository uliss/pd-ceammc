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

#ifndef DEF_ENERGY
#define DEF_ENERGY

#include "math.h"
#include <complex>
#include <stdio.h>
#include <vector>
#include "cicmDescriptor.h"

class Energy : public Descriptor
{
	
private:
	
	Cicm_Signal m_minimum;
	Cicm_Signal m_average;
	Cicm_Signal m_maximum;
	Cicm_Signal m_sum;
    
    long        m_amp_mode;
	
public:
	Energy(long aWindowSize = 1024, long aNumberOfWindows = 1, std::string aWindowType = "hanning", std::string aAmplitudeMode = "magnitude", std::string aFrequencyMode = "linear", long aVectorSize = 0);
	/* Virtual Function */
	void compute();
	void process();
    virtual void setAmplitudeMode(std::string aAmplitudeMode);
    
	template<typename Type> void process(Type *anInput, Type* aMinimum, Type* anAverage, Type* aMaximum, Type* aSum)
	{
		for(int i = 0; i < m_vector_size; i++)
		{
			perform(anInput[i]);
			aMinimum[i]	= (Type)m_minimum;
			anAverage[i]= (Type)m_average;
			aMaximum[i]	= (Type)m_maximum;
			aSum[i]		= (Type)m_sum;
		} 
	}
	~Energy();
};

#endif