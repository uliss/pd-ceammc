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

#include "cicmWindow.h"

Window::Window(long aWindowSize, std::string aWindowType, double a0, double a1, double a2)
{
	m_size = aWindowSize;
	Cicm_signal_malloc(m_vector, m_size);
	setFunction(aWindowType);
}

void Window::setSize(long aWindowSize)
{
	if(aWindowSize != m_size)
	{
		Cicm_free(m_vector);
		m_size = aWindowSize;
		Cicm_signal_malloc(m_vector, m_size);
		setFunction(m_type);
	}
}

void Window::setFunction(std::string aWindowType, double a0, double a1, double a2)
{
	m_type = aWindowType;
	if(aWindowType == "square")
		square();
	else if(aWindowType == "hanning")
		hanning();
	else if(aWindowType == "hamming")
		hamming();
	else if(aWindowType == "tukey")
	{
		if (a0 > 0.) 
			tukey(a0);
		else
			tukey();
	}
	else if(aWindowType == "cosinus")
		cosinus();
	else if(aWindowType == "lanczos")
		lanczos();
	else if(aWindowType == "triangular")
		triangular();
	else if(aWindowType == "bartlett")
		bartlett();
	else if(aWindowType == "gaussian")
	{
		if (a0 > 0.) 
			gaussian(a0);
		else
			gaussian();
	}
	else if(aWindowType == "bartlettHann")
	{
		if (a0 > 0. && a1 > 0. && a2 >  0.)
			bartlettHann(a0, a1, a2);
		else
			bartlettHann();
	}
	else if(aWindowType == "blackman")
	{
		if (a0 > 0.)
			blackman(a0);
		else
			blackman();
	}
	else if(aWindowType == "kaiser")
	{
		if (a0 > 0.)
			kaiser(a0);
		else
			kaiser();
	}
	else
	{
		hanning();
		aWindowType = "hanning";
	}
    normalize();
}

void Window::square()
{
	for(int i = 0; i < m_size; i++)
		m_vector[i] = 1.;
}

void Window::hanning()
{
	for(int i = 0; i < m_size; i++)
		m_vector[i] = 0.5 * (1.0 - cos(2.0 * CICM_PI * (double)i / (double)(m_size - 1)));
}

void Window::hamming()
{
	for(int i = 0; i < m_size; i++)
		m_vector[i] = 0.54 - (0.46 * cos(2.0 * CICM_PI * (double)i / (double)(m_size - 1)));
}

void Window::tukey(double a0)
{
	a0 = Tools::clip(a0);
	for(int i = 0; i < m_size; i++)
    {
		if(i <= ((a0 * (double)(m_size - 1)) / 2.))
			m_vector[i] = 0.5 * ( 1. + cos(CICM_PI * ( ((2 * (double)i) / (a0 * (double)(m_size - 1))) - 1.)));
		else if(i > ((a0 * (double)(m_size - 1)) / 2.) && i <= ((double)(m_size - 1) * (1. - ( a0 / 2.))))
			m_vector[i] = 1.;
		else 
			m_vector[i] = 0.5 * ( 1. + cos(CICM_PI * ( ((2 *(double)i) / (a0 * (double)(m_size - 1))) + 1. - (2. / a0))));
    }
}

void Window::cosinus()
{
	for(int i = 0; i < m_size; i++)
		m_vector[i] = sin((CICM_PI * (double)i) / (double)(m_size - 1));
}

void Window::lanczos()
{
	for(int i = 0; i < m_size; i++)
		m_vector[i] = sin(CICM_PI * (((2. * (double)i) / (double)(m_size - 1)) - 1.)) / (CICM_PI * (((2. * (double)i) / (double)(m_size - 1)) - 1.));
}

void Window::triangular()
{
	for(int i = 0; i < m_size; i++)
		m_vector[i] = (2. / (double)(m_size + 1)) * (((double)(m_size + 1) / 2.) - fabs((double)i - (double)(m_size - 1) / 2.));
}

void Window::bartlett()
{
	for(int i = 0; i < m_size; i++)
		m_vector[i] = (2. / (double)(m_size - 1)) * (((double)(m_size - 1) / 2.) - fabs((double)i - ((double)(m_size - 1) / 2.)));
}


void Window::gaussian(double a0)
{
	a0 = Tools::clip(a0, 0., 0.5);
	for(int i = 0; i < m_size; i++)
		m_vector[i] = exp((i - (double)(m_size - 1) / 2.)/(a0 * (double)(m_size - 1) / 2.) * (i - (double)(m_size - 1) / 2.)/(a0 * (double)(m_size - 1) / 2.) * -0.5);
}

void Window::bartlettHann(double a0, double a1, double a2)
{
	a0 = Tools::clip(a0);
	a1 = Tools::clip(a1);
	a2 = Tools::clip(a2);
	for(int i = 0; i < m_size; i++)
		m_vector[i] = a0 - a1 * fabs(((double)i / (double)(m_size - 1)) - 0.5) - a2 * cos((2.0 * CICM_PI *(double)i) / (double)(m_size - 1));
}

void Window::blackman(double a0)
{
	a0 = Tools::clip(a0);
	for(int i = 0; i < m_size; i++)
		m_vector[i] = ((1. - a0) / 2.) - (0.5 * cos(2.0 * CICM_PI * (double)i / (double)(m_size - 1))) + ((a0 / 2.) * cos(4.0 * CICM_PI * (double)i / (double)(m_size - 1)));
}

void Window::kaiser(double a0)
{
	a0 = Tools::clip(a0, 0., 10.);
    
	for(int i = 0; i < m_size; i++)
		m_vector[i] = gsl_sf_bessel_I0(CICM_PI * a0 * sqrt((1. - pow(((2. * (double)i) / (double)(m_size - 1)) - 1., 2.)))) / gsl_sf_bessel_I0(CICM_PI * a0);
    
}

void Window::normalize()
{
    for(int i = 0; i < m_size; i++)
        m_vector[i] /= (double)m_size * CICM_2PI;
}

double Window::getIndexValue(long anIndex)
{
	anIndex = Tools::clip(anIndex, (long)0, m_size);
	return m_vector[anIndex];
}

void Window::applyWindow(Cicm_Signal *anInput, Cicm_Signal *aResult)
{
    Cicm_signal_mul(anInput, m_vector, aResult, m_size);
}

Window::~Window()
{
	free(m_vector);
}