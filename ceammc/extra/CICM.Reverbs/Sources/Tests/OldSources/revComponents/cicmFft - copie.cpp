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

#include "revFftFilter.h"

FftFilter::FftFilter(long aWindowSize,  long aNumberOfInstances)
{
	m_window_size = Tools::clip_power_of_two(aWindowSize * 2);
	m_array_size = m_window_size / 2;
	m_number_of_instances = Tools::clip_min(aNumberOfInstances, (long)1);

	m_input_real_vector	= new double[m_window_size];
	m_input_comp_vector	= new double[m_window_size];
	m_handle_forward	= fftw_plan_r2r_1d(m_window_size, m_input_real_vector, m_input_comp_vector, FFTW_R2HC, FFTW_ESTIMATE);

	m_output_real_vectors	= new double*[m_number_of_instances];
	m_output_comp_vectors	= new double*[m_number_of_instances];
	m_ir_comp_vectors		= new double*[m_number_of_instances];
	m_handle_backward		= new  fftw_plan[m_number_of_instances];
	m_output_real_add		= new gsl_vector_view[m_number_of_instances];
	for(int i = 0; i < m_number_of_instances; i++)
	{
		m_output_comp_vectors[i]	= new double[m_window_size];
		m_output_real_vectors[i]	= new double[m_window_size];
		m_ir_comp_vectors[i]		= new double[m_window_size];
		m_handle_backward[i]		= fftw_plan_r2r_1d(m_window_size, m_output_comp_vectors[i], m_output_real_vectors[i], FFTW_HC2R, FFTW_ESTIMATE);
		for(int j = 0; j < m_window_size; j++)
		{
			m_output_real_vectors[i][j] = 0;
			m_output_comp_vectors[i][j] = 0;
			m_ir_comp_vectors[i][j] = 0;
		}
		m_output_real_add[i] = gsl_vector_view_array (m_output_real_vectors[i], m_window_size);
	}

	m_last			= gsl_vector_alloc(m_array_size);
	m_output_read	= gsl_vector_view_array (m_output_real_vectors[0], m_array_size);
	m_output_last	= gsl_vector_view_array (m_output_real_vectors[0] + m_array_size, m_array_size);
	
	gsl_vector_set_zero(&m_output_read.vector);
	gsl_vector_set_zero(&m_output_last.vector);
	gsl_vector_set_zero(m_last);
	for(int j = 0; j < m_window_size; j++)
	{
		m_input_real_vector[j] = 0;
		m_input_comp_vector[j] = 0;
	}


	m_ramp = 0;
}

void FftFilter::loadImpulseResponse(double* anImpulseResponse)
{
	double *datas = new double[m_window_size];
	for(int i = 0; i < m_number_of_instances; i++)
	{
		for(int j = 0; j < m_array_size; j++)
			datas[j] = anImpulseResponse[j+m_array_size*i] / m_window_size;
		for(int j = m_array_size; j < m_window_size; j++)
			datas[j] = 0.;

		fftw_plan handle = fftw_plan_r2r_1d(m_window_size, datas, m_ir_comp_vectors[i], FFTW_R2HC, FFTW_ESTIMATE);
		fftw_execute(handle);
		fftw_destroy_plan(handle);
	}
	free(datas);
}

double FftFilter::process(double anInput)
{
	m_input_real_vector[m_ramp] = anInput;
	double result = m_output_real_vectors[0][m_ramp];
	/* Convolution */
	double c, d;
	if(m_ramp != 0)
	{
		double a = m_input_comp_vector[m_ramp];
		double b = m_input_comp_vector[m_window_size - m_ramp];

		for(int i = 0; i < m_number_of_instances; i++)
		{
			c = m_ir_comp_vectors[i][m_ramp];
			d = m_ir_comp_vectors[i][m_window_size - m_ramp];
			m_output_comp_vectors[i][m_ramp] = (a*c-b*d);
			m_output_comp_vectors[i][m_window_size - m_ramp] = (a*d+b*c);
		}
	}
	
	/* FFT and iFFts */
	if(++m_ramp >= m_window_size / 2)
	{
		gsl_blas_dcopy(&m_output_last.vector, m_last);
		for(int i = 0; i < m_number_of_instances; i++)
		{
			fftw_execute(m_handle_backward[i]);
			if(i < m_number_of_instances - 1)
				gsl_vector_add(&m_output_real_add[i].vector, &m_output_real_add[i+1].vector);
		}
		gsl_vector_add(&m_output_read.vector, m_last);
		fftw_execute(m_handle_forward);
		for(int i = 0; i < m_number_of_instances; i++)
		{
			m_output_comp_vectors[i][0] = m_input_comp_vector[0] * m_ir_comp_vectors[i][0];
			m_output_comp_vectors[i][m_array_size] = m_input_comp_vector[m_array_size] * m_ir_comp_vectors[i][m_array_size];
		}
		m_ramp = 0;
	}
	
	return result;
}

FftFilter::~FftFilter()
{
	fftw_destroy_plan(m_handle_forward);
	free(m_input_comp_vector);
	free(m_input_real_vector);
	for(int i = 0; i < m_number_of_instances; i++)
	{
		fftw_destroy_plan(m_handle_backward[i]);
		free(m_output_comp_vectors[i]);
		free(m_ir_comp_vectors[i]);
		free(m_output_real_vectors[i]);
	}
	
	free(m_handle_backward);
	free(m_output_comp_vectors);
	free(m_ir_comp_vectors);
	free(m_output_real_vectors);
	free(m_output_real_add);
	gsl_vector_free(m_last);	
}