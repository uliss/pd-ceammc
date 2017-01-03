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

#ifndef DEF_LSD_DESCRIPTOR
#define DEF_LSD_DESCRIPTOR

#include "cicmFft.h"
#include "cicmTools.h"
#include "cicmWindow.h"

class Descriptor 
{
	
private:
	
protected:
	long            m_frequency_mode;
    long            m_amplitude_mode;
    
	long            m_number_of_instances;
	Cicm_Fft*       m_fft_instance;
    Window*         m_window;
    Cicm_Signal*    m_input;
	Cicm_Signal*    m_real;
	Cicm_Packed*    m_complexes;
    Cicm_Signal*    m_amplitude;
    Cicm_Signal*    m_phase;
    
    Cicm_Signal*    m_decibel_vector;
    Cicm_Signal*    m_log_vector;
    Cicm_Signal*    m_temp_vector;
    
	long            m_window_size;

	long            m_hope_size;
	long            m_real_size;
	long            m_array_size;
	long            m_ramp;
    
    long            m_vector_size;
	long            m_sampling_rate;
	
public:
	Descriptor(long aWindowSize = 1024, long aNumberOfWindows = 1, std::string aWindowType = "hanning", std::string aAmplitudeMode = "magnitude", std::string aFrequencyMode = "linear", long aVectorSize = 0);
	void	perform(double anInput);
	
	virtual void setVectorSize(long aVectorSize);
	virtual void setSamplingRate(long aSamplingRate);
    virtual void setWindowFunction(std::string aWindowType);
    virtual void setFrequencyMode(std::string aFrequencyMode);
	virtual void setAmplitudeMode(std::string aAmplitudeMode);
    
	virtual void process() = 0;
	virtual void compute() = 0;
	virtual ~Descriptor();
};

#endif