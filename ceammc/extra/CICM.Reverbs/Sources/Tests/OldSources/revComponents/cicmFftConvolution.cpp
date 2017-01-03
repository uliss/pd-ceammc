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

#include "cicmFft.h"

FftFilter::FftFilter(long aWindowSize, long aNumberOfInstances)
{
	m_window_size	= Tools::clip_power_of_two(aWindowSize * 2);
	m_array_size	= m_window_size / 2;
	m_log2_size		= Tools::log2(m_window_size);
	m_number_of_instances = Tools::clip_min(aNumberOfInstances, (long)1);
	m_hope_size		= m_array_size / m_number_of_instances;
	
	m_fft_setup = Cicm_create_Fft_setup(m_log2_size, FFT_RADIX2);
	
	m_input_complexes.imagp		= (Cicm_Signal *)Cicm_malloc_signal(m_array_size);
	m_input_complexes.realp		= (Cicm_Signal *)Cicm_malloc_signal(m_array_size);
	Cicm_signal_clear(m_input_complexes.imagp, m_array_size);
	Cicm_signal_clear(m_input_complexes.realp, m_array_size);
		
	m_real_vector				= new Cicm_Signal*[m_number_of_instances];
	m_impul_complexes			= new Cicm_Split_Complex[m_number_of_instances];
	m_output_complexes			= new Cicm_Split_Complex[m_number_of_instances];
	
	for(int i = 0; i < m_number_of_instances; i++)
	{
		m_real_vector[i]				= (Cicm_Signal *)malloc(m_window_size * sizeof(Cicm_Signal));
		Cicm_signal_clear(m_real_vector[i], 1, m_window_size);
		m_impul_complexes[i].imagp		= (Cicm_Signal *)malloc(m_array_size * sizeof(Cicm_Signal));
		m_impul_complexes[i].realp		= (Cicm_Signal *)malloc(m_array_size * sizeof(Cicm_Signal));
		Cicm_signal_clear(m_impul_complexes[i].imagp, 1,m_array_size);
		Cicm_signal_clear(m_impul_complexes[i].realp, 1,m_array_size);
		m_output_complexes[i].imagp	= (Cicm_Signal *)malloc(m_array_size * sizeof(Cicm_Signal));
		m_output_complexes[i].realp	= (Cicm_Signal *)malloc(m_array_size * sizeof(Cicm_Signal));
		Cicm_signal_clear(m_output_complexes[i].imagp, 1,m_array_size);
		Cicm_signal_clear(m_output_complexes[i].realp, 1,m_array_size);
		
	}
	m_buffer					= (Cicm_Signal *)malloc(m_array_size * sizeof(Cicm_Signal));
	Cicm_signal_clear(m_buffer, 1, m_array_size);
	
	m_scale = 1. / (Cicm_Signal)(m_window_size * 2.);
	m_ramp = 0;
}

void FftFilter::loadImpulseResponse(Cicm_Signal* anImpulseResponse)
{
	Cicm_Signal *datas = new Cicm_Signal[m_window_size];
	for(int i = 0; i < m_number_of_instances; i++)
	{
		for(int j = 0; j < m_array_size; j++)
			datas[j] = anImpulseResponse[j + m_array_size * i] * m_scale;
		for(int j = m_array_size; j < m_window_size; j++)
			datas[j] = 0.;
		
		Cicm_ctoz((Cicm_Complex *)datas, 2, &m_impul_complexes[i], 1, m_array_size);
		Cicm_fft_zrip(m_fft_setup, &m_impul_complexes[i], 1, m_log2_size, FFT_FORWARD);
	}
	
	free(datas);
}

FftFilter::~FftFilter()
{
	Cicm_destroy_Fft_setup(m_fft_setup);
	free(m_buffer);
	free(m_input_complexes.imagp);
	free(m_input_complexes.realp);
	
	for(int i = 0; i < m_number_of_instances; i++)
	{
		free(m_real_vector[i]);
		free(m_impul_complexes[i].imagp);
		free(m_impul_complexes[i].realp);
		free(m_output_complexes[i].imagp);
		free(m_output_complexes[i].realp);
	}
	free(m_real_vector);
	free(m_impul_complexes);	
	free(m_output_complexes);
}