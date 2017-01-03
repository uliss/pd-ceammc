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

#include "revConvolve.h"

GardnerConvolution::GardnerConvolution(long aMinimumSize, long aMaximumSize) 
{
	m_minimum_size = aMinimumSize;
	m_maximum_size = aMaximumSize;
	m_number_of_ffts = 0;
	m_number_of_first_filter = 0;
	m_offset_size = m_maximum_size * 2;

	m_fir = new FirFilter(m_minimum_size * 2);
	
	for(long windowSize = 0; windowSize < m_maximum_size; m_number_of_ffts++)
	{
		windowSize = m_minimum_size * pow(2., (double)m_number_of_ffts);
		m_fft.push_back(new FftConvolution(windowSize, 2));
	}
	 
}

void GardnerConvolution::setImpulseResponse(double* setImpulseResponse, long aSize, long anOffset)
{
	int aNumberOfFft = Tools::clip_min((int)((aSize - m_offset_size) / m_maximum_size), 0) + 1;
	if(aNumberOfFft > 10)
		aNumberOfFft = 10;
	int aIrSize = m_offset_size + aNumberOfFft * m_maximum_size;

	anOffset = Tools::clip(anOffset, (long)0, (long)aIrSize);

	Cicm_Signal* datas = new Cicm_Signal[aIrSize];
	for(int i = 0; i < aIrSize; i++)
	{
		if(i < aSize && i >= anOffset)
			datas[i] = (double)setImpulseResponse[i];
		else
			datas[i] = 0.;
	}

	m_fir->setImpulseResponse(datas);
	
	m_fft.pop_back();
	m_fft.push_back(new FftConvolution(m_maximum_size, aNumberOfFft));

	long offset = m_minimum_size * 2;
	
	m_number_of_first_filter = 0;
	for(int i = 0; i < m_number_of_ffts; i++)
	{
		if(anOffset >= offset)
			m_number_of_first_filter = i + 1;
		m_fft[i]->loadImpulseResponse(datas + offset, offset);
		offset *= 2;
	}
	
}

GardnerConvolution::~GardnerConvolution()
{
	delete m_fir;
	for(int i = 0; i < m_number_of_ffts; i++)
	{
		delete m_fft[i];
		m_fft[i] = 0;
	}
}
