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

Cicm_Fft::Cicm_Fft(long aWindowSize)
{
	m_window_size	= Tools::clip_power_of_two(aWindowSize * 2);
	m_array_size	= m_window_size / 2;
	m_order			= Tools::log2(m_window_size);
	m_scale			= 1. / (double)(m_window_size * 2.);

#ifdef CICM_VDSP
	m_fft_handle = Cicm_fft_init_handle(m_order);
#endif
#ifdef CICM_IPPS
	Cicm_fft_get_size(m_order, &m_spec_size, &m_init_size, &m_buff_size);
	m_fft_buff = Cicm_buffer_malloc(m_buff_size);
	m_fft_init = Cicm_buffer_malloc(m_init_size);
	m_fft_spec = Cicm_buffer_malloc(m_spec_size);
	Cicm_fft_init_handle(&m_fft_handle, m_order, m_fft_spec, m_fft_init);
#endif
#ifdef CICM_FFTW_GSL
	Cicm_signal_malloc(m_real_vector, m_window_size);
	Cicm_packed_malloc(m_complex_vector, m_window_size);
	Cicm_signal_clear(m_real_vector, m_window_size);
	Cicm_signal_clear(m_complex_vector, m_window_size);
	m_handle_forward	= Cicm_fft_init_handle_forward(m_window_size, m_real_vector, m_complex_vector);
	m_handle_inverse	= Cicm_fft_init_handle_inverse(m_window_size, m_complex_vector, m_real_vector);
#endif
		
}

long	Cicm_Fft::getWindowSize()
{
	return m_window_size;
}

long	Cicm_Fft::getArraySize()
{
	return m_array_size;
}
long	Cicm_Fft::getOrder()
{
	return m_order;
}
double	Cicm_Fft::getScale()
{
	return m_scale;
}

Cicm_Fft::~Cicm_Fft()
{
#ifdef CICM_VDSP
	Cicm_fft_free_handle(m_fft_handle);
#endif
#ifdef CICM_IPPS
	Cicm_free(m_fft_buff);
	Cicm_free(m_fft_init);
	Cicm_free(m_fft_spec);
	Cicm_fft_free_handle(m_fft_handle);
#endif
#ifdef CICM_FFTW_GSL
	Cicm_fft_free_handle(m_handle_forward);
	Cicm_fft_free_handle(m_handle_inverse);
	Cicm_free(m_real_vector);
	Cicm_free(m_complex_vector);
#endif
}