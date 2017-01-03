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

#include "Lsd_Fft.hpp"

Fft::Fft(int aWindowSize, std::string aWindowType)
{	
	m_window_size = aWindowSize;
	m_array_size  = m_window_size / 2 + 1;
	
	m_real			= new double[m_window_size];
	m_complex		= new MKL_Complex16[m_array_size];
	
	m_window = new Window(aWindowSize, aWindowType);
	
	DftiCreateDescriptor(&m_handle, DFTI_DOUBLE, DFTI_REAL, 1, (MKL_LONG)m_window_size);
	DftiSetValue(m_handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	DftiSetValue(m_handle, DFTI_CONJUGATE_EVEN_STORAGE, DFTI_COMPLEX_COMPLEX);
	DftiCommitDescriptor(m_handle);
}

void Fft::process(double* anInput, double* anOutput)
{
	m_window->applyWindow(anInput, m_real);
	DftiComputeForward(m_handle, m_real, m_complex);
	vzAbs(m_array_size, m_complex, anOutput);
}

Fft::~Fft()
{
	DftiFreeDescriptor(&m_handle);
	delete m_window;
	delete m_real;
	delete m_complex;
}