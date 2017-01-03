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

#ifndef DEF_CICM_CONVOLVE
#define DEF_CICM_CONVOLVE

#include "cicmTools.h"
#include "cicmFir.h"
#include "cicmFftConvolution.h"

class GardnerConvolution
{
protected:
	long		m_minimum_size;
	long		m_maximum_size;
	long		m_offset_size;
	long		m_number_of_ffts;
	long		m_number_of_first_filter;

	FirFilter*	m_fir;
	vector <FftConvolution*> m_fft;

	Cicm_Signal		m_result;

public:
	GardnerConvolution(long aMinimumSize = 64, long aMaximumSize = 8192);
	void	setImpulseResponse(double* anImpulResponse, long aSize, long anOffset = 0);
	inline Cicm_Signal process(Cicm_Signal anInput);
	~GardnerConvolution();
};

inline Cicm_Signal GardnerConvolution::process(Cicm_Signal anInput)
{
	if(!m_number_of_first_filter)
		m_result = m_fir->process(anInput);
	else 
		m_result = 0.;

	for(int i = m_number_of_ffts - 1; i > m_number_of_first_filter; i--)
	{
			m_result += m_fft[i]->process(anInput);
	}
	return  m_result;
}

#endif