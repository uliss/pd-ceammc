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

#ifndef DEF_DIFFUSER
#define DEF_DIFFUSER

class Diffuser
{
private:
	long	m_size;
	double	m_Coefficient;
	long	m_ramp;
	double*	m_buffer;

public:
	inline Diffuser(long aSize, double aCoefficient);
	inline void flush();
	inline double process(double aValue);
	inline ~Diffuser();
};

inline Diffuser::Diffuser(long aSize, double aCoefficient)
{
	m_size			= aSize;
	m_Coefficient	= aCoefficient;
	m_ramp			= 0;
	m_buffer		= new double[m_size];
	flush();
}

inline double Diffuser::process(double aValue)
{
	double w = aValue - m_buffer[m_ramp] * m_Coefficient;
	double y = m_buffer[m_ramp] + w * m_Coefficient;
	m_buffer[m_ramp] = w;
	if (++m_ramp >= m_size) 
		m_ramp = 0;

	return y;
}

inline Diffuser :: ~Diffuser()
{
	delete[] m_buffer;
}

inline void Diffuser::flush()
{
	for(int i = 0; i < m_size; i++)
		m_buffer[i] = 0.;
}

#endif