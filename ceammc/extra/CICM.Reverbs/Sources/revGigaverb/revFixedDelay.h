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

#ifndef DEF_FIXEDDELAY
#define DEF_FIXEDDELAY

class FixedDelay
{
private :
	long	m_size;
	long	m_ramp;
	double* m_buffer;

public:
	inline FixedDelay(long aSize);
	inline void flush();
	inline double read(long anIndex);
	inline void write(double aValue);
	inline ~FixedDelay();
};

inline FixedDelay::FixedDelay(long aSize)
{
	m_size = aSize;
	m_ramp = 0;
	m_buffer = new double[m_size];
	flush();
}

inline double FixedDelay::read(long anIndex)
{
	return m_buffer[(m_ramp - anIndex + m_size) % m_size];
}

inline void FixedDelay::write(double aValue)
{
	m_buffer[m_ramp] = aValue;
	if (++m_ramp >= m_size) 
		m_ramp = 0;
}


inline FixedDelay::~FixedDelay()
{
	delete[] m_buffer;
}

inline void FixedDelay :: flush()
{
	for(int i = 0; i < m_size; i++)
		m_buffer[i] = 0.;
}

#endif