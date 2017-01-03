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
#ifndef DEF_COMBFILTER
#define DEF_COMBFILTER

/*******************************************************/
/** y(n) = a * x(n) + b * x(n-delay) + c * y(n-delay) **/
/*******************************************************/

class CombFilter
{
private:
	double	m_feedback;
	double	m_feedforward;
	double	m_gain;
	double	m_delay;
	
	double	m_filterstore;
	double	m_damp1;
	double	m_damp2;

	double* m_buffer;
	long	m_buffer_size;
	long	m_ramp;

	double	m_output;
public:
		CombFilter();
		void	setBuffer(double *aBuffer, long aBufferSize);
		long	getBufferSize() ;
	
		void	setDamp(double aDampValue);
		double	getDamp();
	
		void	setFeedback(double aFeedbackValue);
		double	getFeedback();
		void	setFeedforward(double aFeedforwardValue);
		double	getFeedforward();
		void	setDelay(double aValue);
		double	getDelay();
	
		inline  double	process(double anInput);
		~CombFilter(){};
};


inline double CombFilter::process(double anInput)
{
	m_output = m_buffer[m_ramp];
	m_filterstore = (m_output * m_damp2) + (m_filterstore * m_damp1);
	m_buffer[m_ramp] = anInput + (m_filterstore * m_feedback);
	if(++m_ramp >= m_buffer_size) 
		m_ramp = 0;
	
	return m_output;
}
#endif
