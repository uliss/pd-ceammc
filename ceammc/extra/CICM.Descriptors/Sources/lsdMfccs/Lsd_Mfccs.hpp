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

#ifndef define DEF_MOMENT
#define DEF_MOMENT

#include "math.h"
#include <complex>
#include <stdio.h>
#include <vector>
#include "Lsd_Descriptor.hpp"

class Mfccs : public Descriptor
{
	
private:
	std::string m_mode;
	double	m_reinjectedSignal;
	double	m_highpassFactor;
	int		m_numberOfMelsBands;
	
public:
	Mfccs(int aWindowSize = 1024, int aNumberOfWindows = 1, std::string aWindowType = "hamming", int aVectorSize = 0, std::string aMode = "basic");
	void setSamplingRate(int aSamplingRate);
	void setMelFilterbands();
	void compute();
	void process();
	void process(double anInput, double& aMinimum, double& anAverage, double& aMaximum, double& aSum);
	void process(float anInput, float& aMinimum, float& anAverage, float& aMaximum, float& aSum);
	void process(double *anInput, double* aMinimum, double* anAverage, double* aMaximum, double* aSum);
	void process(float *anInput, float* aMinimum, float* anAverage, float* aMaximum, float* aSum);
	~Mfccs();
};

#endif