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

#ifndef DEF_LSD_WINDOW
#define DEF_LSD_WINDOW

#include <cicmTools.h>

class Window
{
	
private:
	long            m_size;
	std::string     m_type;
	Cicm_Signal*    m_vector;

public:
	Window(long aWindowSize = 1024, std::string aWindowType = "hanning", double a0 = -1., double a1 = -1., double a2 = -1.);
	void setSize(long aWindowSize);
	void setFunction(std::string aWindowType, double a0 = -1., double a1 = -1., double a2 = -1.);

	void square();
	void hanning();
	void hamming();
	void tukey(double a0 = 0.5);
	void cosinus();
	void lanczos();
	void triangular();
	void bartlett();
	void gaussian(double a0 = 0.5);
	void bartlettHann(double a0 = 0.62, double a1 = 0.48, double a2 = 0.38);
	void blackman(double a0 = 0.16);
	void kaiser(double a0 = 3.);
	
    void normalize();
	double getIndexValue(long anIndex);
	void applyWindow(Cicm_Signal *anInput, Cicm_Signal *aResult);
	
	~Window();
};

#endif