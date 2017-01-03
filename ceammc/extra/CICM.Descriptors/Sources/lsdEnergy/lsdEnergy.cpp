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

#include "lsdEnergy.h"

Energy::Energy(long aWindowSize, long aNumberOfWindows, std::string aWindowType, std::string aAmplitudeMode, std::string aFrequencyMode, long aVectorSize) : Descriptor(aWindowSize, aNumberOfWindows, aWindowType, aAmplitudeMode, aFrequencyMode, aVectorSize)
{	
	m_minimum = 0.;
	m_average = 0.;
	m_maximum = 0.;
	m_sum = 0.;     
}

void Energy::compute()
{
    Cicm_min(m_amplitude, m_array_size, &m_minimum);
    Cicm_mean(m_amplitude, m_array_size, &m_average);
    Cicm_max(m_amplitude, m_array_size, &m_maximum);
    Cicm_sum(m_amplitude, m_array_size, &m_sum);
    if(m_amp_mode)
    {
        m_minimum = 20 * log(m_minimum);
        m_average = 20 * log(m_average);
        m_maximum = 20 * log(m_maximum);
        m_sum = 20 * log(m_sum);
    }
}

void Energy::setAmplitudeMode(std::string aAmplitudeMode)
{
    if(aAmplitudeMode == "power")
    {
        m_amplitude_mode = 1;
        m_amp_mode = 0;
    }
    else if(aAmplitudeMode == "decibel")
    {
        m_amplitude_mode = 0;
        m_amp_mode = 1;
    }
    else
    {
        m_amplitude_mode = 0;
        m_amp_mode = 0;
    }
}

void Energy::process()
{
	;
}

Energy::~Energy()
{
	;
}

