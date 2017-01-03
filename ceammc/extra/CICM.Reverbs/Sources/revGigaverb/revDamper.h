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

#ifndef DEF_DAMPER
#define DEF_DAMPER

class Damper
{
private:
	double m_damping;
	double m_delay;

public:
	inline Damper(double aDamping);
	inline void flush();
	inline void set(double aDamping);
	inline double process(double aValue);
	inline ~Damper(){};
};

inline Damper::Damper(double aDamping)
{
	m_damping = aDamping;
	flush();
}

inline void Damper::flush()
{
	m_delay = 0.0f;
}

inline void Damper::set(double aDamping)
{ 
	m_damping = aDamping;
} 
  
inline double Damper::process(double aValue)
{ 
	m_delay = aValue * (1. - m_damping) + m_delay * m_damping;
	return(m_delay);
}



#endif