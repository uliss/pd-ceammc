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

#include "lsdSpectrum.h"

Spectrum::Spectrum(long aWindowSize, long aNumberOfWindows, std::string aWindowType, std::string aAmplitudeMode, std::string aFrequencyMode, long aVectorSize) : Descriptor(aWindowSize, aNumberOfWindows, aWindowType, aAmplitudeMode, aFrequencyMode, aVectorSize)
{	
	;
}

void Spectrum::compute()
{
	;
}

void Spectrum::process()
{
	;
}

/* Process sample by sample */
void Spectrum::process(double anInput)
{
	perform(anInput);
} 

/* Process by sample block */
void Spectrum::process(double *anInput)
{
	for(int i = 0; i < m_vector_size; i++)
		process(anInput[i]);
}

void Spectrum::process(float *anInput)
{
	for(int i = 0; i < m_vector_size; i++)
		process(anInput[i]);
}

double Spectrum::getAmplitude(int anIndex)
{
	anIndex = Tools::clip(anIndex, (int)0, (int)m_array_size);
	return m_amplitude[anIndex];
}

int Spectrum::getArraySize()
{
	return m_array_size;
}

Spectrum::~Spectrum()
{
	;
}

