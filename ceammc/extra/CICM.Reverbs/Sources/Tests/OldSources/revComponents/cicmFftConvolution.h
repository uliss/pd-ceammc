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


#ifndef DEF_CICM_FFT
#define DEF_CICM_FFT

#include "CicmTools.h"

class FftFilter
{
	
private:
	
	long				m_window_size;
	long				m_array_size;
	long				m_log2_size;
	long				m_number_of_instances;
	long				m_ramp;
	long				m_offset;
	long				m_hope_size;
	Cicm_Signal			m_scale;
	
	Cicm_Fft_Handle			m_fft_setup;
	Cicm_Signal**			m_real_vector;
	Cicm_Signal*			m_buffer;
	Cicm_Split_Complex		m_input_complexes;
	Cicm_Split_Complex*		m_impul_complexes;
	Cicm_Split_Complex*		m_output_complexes;
	
	Cicm_Signal				m_result;
	
public:
	FftFilter(long aWindowSize = 1024, long aNumberOfInstances = 2);
	void loadImpulseResponse(Cicm_Signal* anImpulseResponse);
	inline Cicm_Signal process(Cicm_Signal anInput);
	~FftFilter();
};
	
inline Cicm_Signal FftFilter::process(Cicm_Signal anInput)
{
	m_result = m_real_vector[0][m_ramp];
	m_real_vector[0][m_ramp] = anInput;
	
	if(m_ramp % m_hope_size == 0 && m_ramp != 0)
	{
		int i = m_ramp / m_hope_size;
		if(i >= m_number_of_instances)
			i = m_number_of_instances - 1;
		/* CONVOLUTION */
		Cicm_zvmul(&m_input_complexes, 1, &m_impul_complexes[i], 1, &m_output_complexes[i], 1, m_array_size, 1);
		m_output_complexes[i].realp[0] = m_impul_complexes[i].realp[0] * m_input_complexes.realp[0]; 
		m_output_complexes[i].imagp[0] = m_impul_complexes[i].imagp[0] * m_input_complexes.imagp[0];
			
		/* FFT BACKWARD */
		Cicm_fft_zrip(m_fft_setup, &m_output_complexes[i], 1, m_log2_size, FFT_INVERSE); 
		Cicm_ztoc(&m_output_complexes[i], 1, (Cicm_Complex *)m_real_vector[i], 2, m_array_size);
			
		/* COPY OUTPUTS SIGNALS */
		if(i < m_number_of_instances - 1)
			Cicm_vadd(m_real_vector[i+1], 1, m_real_vector[i], 1, m_real_vector[i], 1, m_window_size);
	}
	
	if(++m_ramp >= m_array_size)
	{
		/* RECORD THE END OF THE REAL VECTOR INTO THE BUFFER AND ZEROPAD THE END OG THE REAL VECTOR */
		Cicm_mmov(m_real_vector[0]+m_array_size, m_buffer, m_array_size, 1, m_array_size, m_array_size);
		Cicm_signal_clear(m_real_vector[0]+m_array_size, 1, m_array_size);
		
		/* FFT FORWARD */
		Cicm_ctoz((Cicm_Complex *)m_real_vector[0], 2, &m_input_complexes, 1, m_array_size); 
		Cicm_fft_zrip(m_fft_setup, &m_input_complexes, 1, m_log2_size, FFT_FORWARD);
		
		/* CONVOLUTION */
		Cicm_zvmul(&m_input_complexes, 1, &m_impul_complexes[0], 1, &m_output_complexes[0], 1, m_array_size, 1);
		m_output_complexes[0].realp[0] = m_impul_complexes[0].realp[0] * m_input_complexes.realp[0]; 
		m_output_complexes[0].imagp[0] = m_impul_complexes[0].imagp[0] * m_input_complexes.imagp[0];
		
		/* FFT BACKWARD */
		Cicm_fft_zrip(m_fft_setup, &m_output_complexes[0], 1, m_log2_size, FFT_INVERSE); 
		Cicm_ztoc(&m_output_complexes[0], 1, (Cicm_Complex *)m_real_vector[0], 2, m_array_size);
		
		/* COPY OUTPUTS SIGNALS */
		if(m_number_of_instances > 1)
			Cicm_vadd(m_real_vector[1], 1, m_real_vector[0], 1, m_real_vector[0], 1, m_window_size);

		/* ADD THE BUFFER TO THE REAL VECTOR */ 
		Cicm_vadd(m_real_vector[0], 1, m_buffer, 1, m_real_vector[0], 1, m_array_size);
		m_ramp = 0;
	}
	
	return m_result;
}

#endif
