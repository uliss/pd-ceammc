/*
 *
 * Copyright (C) 2012 Julien Colafrancesco & Pierre Guillot, Universite Paris 8
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

#ifndef DEF_MOMENT
#define DEF_MOMENT

#include "math.h"
#include <complex>
#include <stdio.h>
#include <vector>
#include "cicmDescriptor.h"

class Moment : public Descriptor
{
	
private:

	Cicm_Signal* m_weights;
	Cicm_Signal* m_centroidWeights;
	Cicm_Signal* m_spreadWeights;
	Cicm_Signal* m_skewnessWeights;
	Cicm_Signal* m_kurtosisWeights;
	Cicm_Signal m_centroid;
	Cicm_Signal m_deviation;
	Cicm_Signal m_skewness;
	Cicm_Signal m_kurtosis;
	
public:
	Moment(long aWindowSize = 1024, long aNumberOfWindows = 1, std::string aWindowType = "hanning", std::string aAmplitudeMode = "magnitude", std::string aFrequencyMode = "linear", long aVectorSize = 0);
    virtual void setSamplingRate(long aSamplingRate);

	void compute();
	void process();
    
	template<typename Type> void process(Type *anInput, Type* aCentroid, Type* aDeviation, Type* aSkewness, Type* aKurtosis)
	{
		for(int i = 0; i < m_vector_size; i++)
		{
			perform(anInput[i]);
			aCentroid[i]	= (Type)m_centroid;
			aDeviation[i]   = (Type)m_deviation;
			aSkewness[i]	= (Type)m_skewness;
			aKurtosis[i]	= (Type)m_kurtosis;
		}
	}
    
	~Moment();
};

#endif