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

#include "LsdMoment.h"

Moment::Moment(long aWindowSize, long aNumberOfWindows, std::string aWindowType, std::string aAmplitudeMode, std::string aFrequencyMode, long aVectorSize) : Descriptor(aWindowSize, aNumberOfWindows, aWindowType, aAmplitudeMode, aFrequencyMode, aVectorSize)
{	
	m_weights			= new Cicm_Signal[m_array_size];
	m_centroidWeights	= new Cicm_Signal[m_array_size];
	m_spreadWeights		= new Cicm_Signal[m_array_size];
	m_skewnessWeights	= new Cicm_Signal[m_array_size];
	m_kurtosisWeights	= new Cicm_Signal[m_array_size];
	m_centroid = 0.;
	m_deviation= 0.;
	m_skewness = 0.;
	m_kurtosis = 0.;
}

void Moment::setSamplingRate(long aSamplingRate)
{
	m_sampling_rate = aSamplingRate;
    
	for(int i = 0; i < m_array_size; i++)
		m_weights[i] = ((double)(m_sampling_rate) / (double)m_window_size) * (double)i;
    
    double temp;
    for(int i = 0; i < m_array_size; i++)
    {
        temp = i;
        temp /= m_array_size;
        temp *= m_sampling_rate * 0.0002;
        m_log_vector[i] = exp2(temp);
        m_log_vector[i] /= exp2(m_sampling_rate * 0.0002);
        m_log_vector[i] *= m_array_size;
    }
}

void Moment::compute()
{
	Cicm_Signal sum = 0., spread = 0., skewness = 0., kurtosis = 0.;
    Cicm_sum(m_amplitude, m_array_size, &sum);
	if(sum != 0.)
	{
		/* Centroid */
        Cicm_signal_mul(m_amplitude, m_weights, m_centroidWeights, m_array_size);
        Cicm_sum(m_centroidWeights, m_array_size, &m_centroid);
		m_centroid /= sum;
		
		/* Spread */
		for(int i = 0; i < m_array_size; i++)
			m_kurtosisWeights[i] = m_weights[i] - m_centroid;
		
        Cicm_signal_mul(m_kurtosisWeights, m_kurtosisWeights, m_kurtosisWeights, m_array_size);
        Cicm_signal_mul(m_kurtosisWeights, m_amplitude, m_spreadWeights, m_array_size);
        Cicm_sum(m_spreadWeights, m_array_size, &spread);
		spread /= sum;
		m_deviation = sqrt(spread);
	
		/* Skewness */
        Cicm_signal_mul(m_kurtosisWeights, m_kurtosisWeights, m_kurtosisWeights, m_array_size);
        Cicm_signal_mul(m_amplitude, m_kurtosisWeights, m_kurtosisWeights, m_array_size);
        Cicm_sum(m_skewnessWeights, m_array_size, &skewness);
		m_skewness = skewness / (sum * m_deviation * spread);
	
        /* Kurtosis */
        Cicm_signal_mul(m_kurtosisWeights, m_kurtosisWeights, m_kurtosisWeights, m_array_size);
        Cicm_signal_mul(m_amplitude, m_kurtosisWeights, m_kurtosisWeights, m_array_size);
        Cicm_sum(m_skewnessWeights, m_array_size, &kurtosis);
		m_kurtosis = kurtosis / (sum * spread * spread);
        
	}
	else 
	{
		m_centroid = 0.;
		m_deviation = 0.;
		m_skewness = 0.;
		m_kurtosis = 0.;
	}

		
}

void Moment::process()
{
	;
}


Moment::~Moment()
{
	Cicm_free(m_weights);
	Cicm_free(m_centroidWeights);
	Cicm_free(m_spreadWeights);
	Cicm_free(m_skewnessWeights);
	Cicm_free(m_kurtosisWeights);
}

