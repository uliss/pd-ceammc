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

#include "cicmFftConvolution.h"

FftConvolution::FftConvolution(long aWindowSize, long aNumberOfInstances)
{
	m_fft_instance	= new Cicm_Fft(aWindowSize);
	m_window_size	= m_fft_instance->getWindowSize();
	m_array_size	= m_fft_instance->getArraySize();
	m_number_of_instances = Tools::clip_min(aNumberOfInstances, (long)1);
	m_hope_size		= m_array_size / m_number_of_instances;		
	m_ramp = 0;

	m_real_vector		= new Cicm_Signal*[m_number_of_instances];
	m_impul_complexes	= new Cicm_Signal*[m_number_of_instances];
	
	m_input_complexes	= (Cicm_Signal *)Cicm_signal_malloc(m_window_size);
	m_output_complexes	= (Cicm_Signal *)Cicm_signal_malloc(m_window_size);
	m_buffer			= (Cicm_Signal *)Cicm_signal_malloc(m_array_size);
	Cicm_signal_clear(m_input_complexes, m_window_size);
	Cicm_signal_clear(m_output_complexes, m_window_size);
	Cicm_signal_clear(m_buffer, m_array_size);

	for(int i = 0; i < m_number_of_instances; i++)
	{
		m_real_vector[i]		= (Cicm_Signal *)Cicm_signal_malloc(m_window_size);
		m_impul_complexes[i]	= (Cicm_Signal *)Cicm_signal_malloc(m_window_size);
		Cicm_signal_clear(m_real_vector[i], m_window_size);
		Cicm_signal_clear(m_impul_complexes[i], m_window_size);
	}
}

void FftConvolution::loadImpulseResponse(Cicm_Signal* anImpulseResponse)
{
	Cicm_Signal *datas = (Cicm_Signal *)Cicm_signal_malloc(m_window_size);
	for(int i = 0; i < m_number_of_instances; i++)
	{
		for(int j = 0; j < m_array_size; j++)
			datas[j] = anImpulseResponse[j + m_array_size * i] * m_fft_instance->getScale();
		for(int j = m_array_size; j < m_window_size; j++)
			datas[j] = 0.;
		
		m_fft_instance->forward(datas, m_impul_complexes[i]);
	}
	
	free(datas);
}

FftConvolution::~FftConvolution()
{
	Cicm_free(m_buffer);
	Cicm_free(m_input_complexes);
	Cicm_free(m_output_complexes);
	for(int i = 0; i < m_number_of_instances; i++)
	{
		Cicm_free(m_real_vector[i]);
		Cicm_free(m_impul_complexes[i]);
	}
	free(m_real_vector);
	free(m_impul_complexes);
	delete m_fft_instance;
}