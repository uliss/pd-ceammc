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

#ifndef DEF_FLANGER
#define DEF_FLANGER

#include "cicmTools.h"
#define BUFFERSIZE 44100

class Flanger
{
	
private:
	double	m_buffer[BUFFERSIZE];
	
	double	m_sampling_rate;
	long	m_vector_size;
	
	double	m_feedback;
	double	m_delay;
	double	m_frequency;
	double	m_depth;
	long	m_mode;

	long	m_index;
	double	m_step;
	double	m_phase;
	double	m_offsetter;
	// double	m_delayed_index;
	
public:
	Flanger();
	
	template<typename Type> inline void process(Type *anInputsVector, Type *anOutputsVector)
	{
		Type fracpart, delayed_index, delay, delayedSample;
		
		for(int i = 0; i < m_vector_size; i++)
		{
			delay = fabs(m_offsetter * m_depth + m_delay);
			if(delay < 1.)
				delay = 1.;
			
			delayed_index	= m_index - delay;
			fracpart		= delayed_index - (int)delayed_index;
			delayedSample	= m_buffer[(int)(delayed_index + BUFFERSIZE) % BUFFERSIZE] * (1. - fracpart) + m_buffer[(int)(delayed_index + BUFFERSIZE + 1) % BUFFERSIZE] * fracpart;
			
			anOutputsVector[i]	= delayedSample;
			m_buffer[m_index]	= delayedSample * m_feedback + anInputsVector[i];
			
			m_index = (m_index + 1 + BUFFERSIZE) % BUFFERSIZE;
			
			if(!m_mode)
			{
				m_offsetter = sin(m_phase);
				m_phase += m_frequency;
				
				if(m_phase > (1000. * CICM_PI))
					m_phase = 0.;
			} 
			else 
			{
				m_offsetter += m_step;
				
				if(m_offsetter >= 1.)
					m_step *= -1.;
				else if(m_offsetter <= 0.)
					m_step *= -1.;
			}

		}
	}
	
	void setSamplingRate(double aSamplingRate);
	void setVectorSize(long aVectorSize);
	void setFeedback(double aFeedbackValue);
	void setDelay(double aDelayValue);
	void setFrequency(double aFrequency);
	void setDepth(double aDepthValue);
	void setMode(long aMode);
	
	double getSamplingRate();
	long   getVectorSize();
	double getFeedback();
	double getDelay();
	double getFrequency();
	double getDepth();
	long getMode();
	
	~Flanger(){};
};

#endif