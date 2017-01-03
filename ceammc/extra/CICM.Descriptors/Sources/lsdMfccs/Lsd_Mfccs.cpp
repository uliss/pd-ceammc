/*
 *
 * Copyright (C) 2012 Julien Colafrancesco & Pierre Guillot, Universite Paris 8
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

#include "Lsd_Mfccs.hpp"

Mfccs::Mfccs(int aWindowSize, int aNumberOfWindows, std::string aWindowType, int aVectorSize, std::string aMode) : Descriptor(aWindowSize, aNumberOfWindows, aWindowType, aVectorSize)
{	
	m_mode = aMode;
}

void Mfccs::setSamplingRate(int aSamplingRate)
{
	m_sampling_rate = aSamplingRate;
}

void Mfccs::setMelFilterbands();
{
	frequencyMaximum = m_sampling_rate / 2;
	
}

void Mfccs::compute()
{
	;	
}

void Mfccs::process()
{
	;
}
/* Process sample by sample */
void Mfccs::process(double anInput, double& aMinimum, double& anAverage, double& aMaximum, double& aSum)
{
	perform(anInput - m_reinjectedSignal * m_highpassFactor);
	m_reinjectedSignal = anInput;
}

void Mfccs::process(float anInput, float& aMinimum, float& anAverage, float& aMaximum, float& aSum)
{
	perform(anInput - m_reinjectedSignal * m_highpassFactor);
	m_reinjectedSignal = anInput;
}

/* Process by sample block */
void Mfccs::process(double *anInput, double* aMinimum, double* anAverage, double* aMaximum, double* aSum)
{
	for(int i = 0; i < m_vector_size; i++)
		process(anInput[i], aMinimum[i], anAverage[i], aMaximum[i], aSum[i]);
}

void Mfccs::process(float *anInput, float* aMinimum, float* anAverage, float* aMaximum, float* aSum)
{
	for(int i = 0; i < m_vector_size; i++)
		process(anInput[i], aMinimum[i], anAverage[i], aMaximum[i], aSum[i]);
}

Mfccs::~Mfccs()
{
	;
}

