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

#include "revCombFilter.h"

CombFilter::CombFilter()
{
	m_filterstore = 0;
	m_ramp = 0;
}

void CombFilter::setBuffer(double *aBuffer, long aBufferSize) 
{
	m_buffer = aBuffer; 
	m_buffer_size = aBufferSize;
	for(int i = 0; i < m_buffer_size; i++)
		m_buffer[i] = 0.;
}

long CombFilter::getBufferSize() 
{
	return m_buffer_size;
}

void CombFilter::setDamp(double aDampValue) 
{
	m_damp1 = aDampValue; 
	m_damp2 = 1. - aDampValue;
}

double CombFilter::getDamp() 
{
	return m_damp1;
}

void CombFilter::setFeedback(double aFeedbackValue) 
{
	m_feedback = aFeedbackValue;
}

double CombFilter::getFeedback() 
{
	return m_feedback;
}
