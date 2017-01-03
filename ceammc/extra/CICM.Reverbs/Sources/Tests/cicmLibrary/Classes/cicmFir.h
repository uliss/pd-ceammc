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

#ifndef DEF_CICM_FIR
#define DEF_CICM_FIR

#include "cicmTools.h"

class FirFilter
{
protected:
	
	Cicm_Signal*	m_input_vector;
	Cicm_Signal*	m_fir_vector;
	long			m_fir_size;
	long			m_input_size;
	long			m_index;
	Cicm_Signal		m_result;

public:
	FirFilter(long anImpulseSize = 128);
	void	setImpulseResponse(Cicm_Signal* anImpulseResponse);
	inline Cicm_Signal process(Cicm_Signal anInput);
	~FirFilter();
};

inline Cicm_Signal FirFilter::process(Cicm_Signal anInput)
{
	m_input_vector[--m_index] = anInput;
	Cicm_signal_dot(m_input_vector+m_index, m_fir_vector, &m_result, m_fir_size);
	if(m_index <= 0)
	{
		m_index = m_fir_size;
		Cicm_signal_copy(m_input_vector, m_input_vector+m_fir_size, m_fir_size);
	}
	return m_result;
}

#endif