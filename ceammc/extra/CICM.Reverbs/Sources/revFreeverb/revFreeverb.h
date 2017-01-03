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

#ifndef DEF_FREEVERB
#define DEF_FREEVERB

#include "revCombFilter.h"
#include "revAllpassFilter.h"
#include "revTuning.h"

class Freeverb
{
private:
	int		m_side;
	double	m_gain;
	float	roomsize,roomsize1;
	float	damp,damp1;
	float	mode;

	CombFilter		m_comb_filter[numcombs];
	AllpassFilter	m_allpass_filter[numallpasses];

	double	m_comb_buffer1[combtuningL1];
	double	m_comb_buffer2[combtuningL2];
	double	m_comb_buffer3[combtuningL3];
	double	m_comb_buffer4[combtuningL4];
	double	m_comb_buffer5[combtuningL5];
	double	m_comb_buffer6[combtuningL6];
	double	m_comb_buffer7[combtuningL7];
	double	m_comb_buffer8[combtuningL8];

	double	m_allpass_buffer1[allpasstuningL1];
	double	m_allpass_buffer2[allpasstuningL2];
	double	m_allpass_buffer3[allpasstuningL3];
	double	m_allpass_buffer4[allpasstuningL4];

	double	m_comb_buffer1r[combtuningR1];
	double	m_comb_buffer2r[combtuningR2];
	double	m_comb_buffer3r[combtuningR3];
	double	m_comb_buffer4r[combtuningR4];
	double	m_comb_buffer5r[combtuningR5];
	double	m_comb_buffer6r[combtuningR6];
	double	m_comb_buffer7r[combtuningR7];
	double	m_comb_buffer8r[combtuningR8];

	double	m_allpass_buffer1r[allpasstuningR1];
	double	m_allpass_buffer2r[allpasstuningR2];
	double	m_allpass_buffer3r[allpasstuningR3];
	double	m_allpass_buffer4r[allpasstuningR4];

	void	update();

public:
	Freeverb(int aSide);
 	inline void	process(float *anInput, float *anOutput, int aVectorSize);
	inline void	process(double *anInput, double *anOutput, long aVectorSize);
	void	setroomsize(double value);
	double	getroomsize();
	void	setdamp(double value);
	double	getdamp();
	void	setmode(double value);
	double	getmode();
	~Freeverb(){};
};


inline void Freeverb::process(float *anInput, float *anOutput, int aVectorSize)
{
	for(int i = 0; i < aVectorSize; i++)
	{
		anInput[i] *= m_gain;
		anOutput[i] = 0.;
		for(int j = 0; j < numcombs; j++)
			anOutput[i] += m_comb_filter[j].process(anInput[i]);

		for(int j = 0; j < numallpasses; j++)
			anOutput[i] = m_allpass_filter[j].process(anOutput[i]);
	}
}

inline void Freeverb::process(double *anInput, double *anOutput, long aVectorSize)
{
	for(int i = 0; i < aVectorSize; i++)
	{
		anInput[i] *= m_gain;
		anOutput[i] = 0.;
		for(int j = 0; j < numcombs; j++)
			anOutput[i] += m_comb_filter[j].process(anInput[i]);
		
		for(int j = 0; j < numallpasses; j++)
			anOutput[i] = m_allpass_filter[j].process(anOutput[i]);
	}
}

#endif
