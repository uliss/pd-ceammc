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

#ifdef CICM_WINDOWS

#include "revFirFilter.h"

FirFilter::FirFilter(long anImpulseSize)
{
	m_fir_size		= anImpulseSize;
	m_input_size	= m_fir_size * 2 - 1;

	m_fir_vector	= gsl_vector_alloc(m_fir_size);
	m_input_vector	= gsl_vector_alloc(m_input_size);

	gsl_vector_set_zero(m_fir_vector);
	gsl_vector_set_zero(m_input_vector);

	m_input_process_view = new gsl_vector_view[m_fir_size];
	for(int i = 0; i < m_fir_size; i++)
		m_input_process_view[i] = gsl_vector_subvector(m_input_vector, i, m_fir_size);

	m_input_begin_view	= gsl_vector_subvector(m_input_vector, 0, m_fir_size - 1);
	m_input_end_view	= gsl_vector_subvector(m_input_vector, m_fir_size, m_fir_size - 1);

	m_ramp = m_fir_size;
}


inline double process(double anInput)
{
	gsl_vector_set(m_input_vector, --m_ramp, (double)anInput);
	gsl_blas_ddot(&m_input_process_view[m_ramp].vector, m_fir_vector, &m_result);
	m_result /= m_fir_size;
	if(m_ramp <= 0)
	{
		m_ramp = m_fir_size;
		gsl_blas_dcopy(&m_input_begin_view.vector, &m_input_end_view.vector);
	}
	
	return m_result;
}

void FirFilter::setImpulseResponse(double* anImpulseResponse)
{
	for(int i = 0; i < m_fir_size; i++)
		gsl_vector_set(m_fir_vector, i, anImpulseResponse[i]);
}

void FirFilter::setImpulseSize(long anImpulseSize)
{
	if(m_fir_size != anImpulseSize && anImpulseSize > 0)
	{
		freeVectors();
	
		m_fir_size		= anImpulseSize;
		m_input_size	= m_fir_size * 2 - 1;
		m_fir_vector	= gsl_vector_alloc(m_fir_size);
		m_input_vector	= gsl_vector_alloc(m_input_size);
		gsl_vector_set_zero(m_input_vector);

		m_input_process_view = new gsl_vector_view[m_fir_size];
		for(int i = 0; i < m_fir_size; i++)
			m_input_process_view[i] = gsl_vector_subvector(m_input_vector, i, m_fir_size);

		m_input_begin_view	= gsl_vector_subvector(m_input_vector, 0, m_fir_size - 1);
		m_input_end_view	= gsl_vector_subvector(m_input_vector, m_fir_size, m_fir_size - 1);
		m_ramp = m_fir_size;
	}
}

void FirFilter::freeVectors()
{
	gsl_vector_free(m_fir_vector);
	gsl_vector_free(m_input_vector);
	free(m_input_process_view);
}

FirFilter::~FirFilter()
{
	freeVectors();
}

#endif