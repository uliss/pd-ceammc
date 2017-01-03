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

#include "cicmDescriptor.h"

Descriptor::Descriptor(long aWindowSize, long aNumberOfInstances, std::string aWindowType, std::string aAmplitudeMode, std::string aFrequencyMode, long aVectorSize)
{	
    m_fft_instance	= new Cicm_Fft(aWindowSize);
	m_window_size	= m_fft_instance->getWindowSize();
	m_array_size	= m_fft_instance->getArraySize();
    
	m_number_of_instances = Tools::clip_min(aNumberOfInstances, (long)1);
	m_hope_size		= m_array_size / m_number_of_instances;
    m_real_size     = m_window_size + (aNumberOfInstances - 1) * m_window_size;
    Cicm_signal_malloc(m_input, m_real_size);
    Cicm_signal_malloc(m_real, m_window_size);
    Cicm_signal_malloc(m_amplitude, m_real_size);
    Cicm_packed_malloc(m_complexes, m_real_size);
    
    Cicm_signal_malloc(m_decibel_vector, m_array_size);
    for(int i = 0; i < m_array_size; i++)
        m_decibel_vector[i] =1;
    
    Cicm_signal_malloc(m_log_vector, m_array_size);
    Cicm_signal_malloc(m_temp_vector, m_array_size);
    
    m_window = new Window(m_window_size, aWindowType);
    setFrequencyMode(aFrequencyMode);
    setAmplitudeMode(aAmplitudeMode);   
    setSamplingRate(44100.);
    m_ramp = 0;
}


void Descriptor::setWindowFunction(std::string aWindowType)
{
    m_window->setFunction(aWindowType);
}

void Descriptor::setFrequencyMode(std::string aFrequencyMode)
{
    if(aFrequencyMode == "logarithmic")
        m_frequency_mode = 1;
    else
        m_frequency_mode = 0;
}

void Descriptor::setAmplitudeMode(std::string aAmplitudeMode)
{
    if(aAmplitudeMode == "power")
        m_amplitude_mode = 1;
    else if(aAmplitudeMode == "decibel")
        m_amplitude_mode = 2;
    else
        m_amplitude_mode = 0;    
}

void Descriptor::setVectorSize(long aVectorSize)
{
	m_vector_size = aVectorSize;
}

void Descriptor::setSamplingRate(long aSamplingRate)
{
	m_sampling_rate = aSamplingRate;
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

void Descriptor::perform(double anInput)
{
	m_input[m_ramp] = anInput;
	if(++m_ramp >= m_window_size && m_ramp % m_hope_size == 0)
	{
        m_window->applyWindow(m_input +m_ramp - m_window_size , m_real);
		m_fft_instance->forward(m_real, m_complexes);
        
        Cicm_fft_magnitude(m_complexes, m_amplitude, m_array_size);        
        if(m_amplitude_mode == 1)
            Cicm_fft_power(m_amplitude, m_array_size);
        else if (m_amplitude_mode == 2)
            Cicm_fft_decibel(m_amplitude, m_decibel_vector, m_array_size);
        
        if(m_frequency_mode == 1)
        {
            for(int i = 1; i < m_array_size; i++)
            {
                m_temp_vector[i] = 0.;
                float temp = m_log_vector[i] - m_log_vector[i - 1];
                
                temp++;
                for(int j = 0; j < temp; j++)
                    m_temp_vector[i] += m_amplitude[(int)m_log_vector[i-1]+j];
                
                
                m_temp_vector[i] /= (int)temp;
            }
            for(int i = 1; i < m_array_size; i++)
            {
                m_amplitude[i] = m_temp_vector[i];
            }
        }
        
		compute();
		if(m_ramp == m_real_size)
			m_ramp = 0;
	}
}

Descriptor::~Descriptor()
{
	delete m_fft_instance;
	Cicm_free(m_amplitude);
	Cicm_free(m_real);
    Cicm_free_packed(m_complexes);
    Cicm_free(m_input);
    Cicm_free(m_decibel_vector);
    Cicm_free(m_temp_vector);
}