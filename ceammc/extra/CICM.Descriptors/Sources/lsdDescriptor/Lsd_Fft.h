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

#ifndef DEF_LSD_FFT
#define DEF_LSD_FFT

#include "Lsd_Window.h"

class Fft 
{
	
private:
	int						m_window_size;
	int						m_array_size;
	Window*					m_window;
	DFTI_DESCRIPTOR_HANDLE	m_handle;
	MKL_Complex16*			m_complex;
	double*					m_real;
	
public:
	Fft(int aWindowSize = 1024, std::string aWindowType = "hanning");
	void process(double* anInput, double* anOutput);
	~Fft();	
};

#endif