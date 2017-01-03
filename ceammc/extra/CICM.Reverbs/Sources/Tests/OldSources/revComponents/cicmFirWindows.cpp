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

#include "cicmFirWindows.h"

FirFilter::FirFilter(long anImpulseSize)
{
	m_fir_size		= anImpulseSize;
	m_input_size	= m_fir_size * 2 - 1;

	m_fir_vector	= new Cicm_Signal[m_fir_size];
	m_input_vector	= new Cicm_Signal[m_input_size];

	Cicm_signal_clear(m_fir_vector, 1, m_fir_size);
	Cicm_signal_clear(m_input_vector, 1, m_input_size);
	m_index = m_fir_size;
}

void FirFilter::setImpulseResponse(Cicm_Signal* anImpulseResponse)
{
	Cicm_signal_copy(anImpulseResponse, m_fir_vector, m_fir_size);
}

FirFilter::~FirFilter()
{
	free(m_fir_vector);
	free(m_input_vector);
}
