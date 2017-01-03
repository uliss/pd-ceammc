/*
 * Copyright (C) 2012 Pierre Guillot, CICM & Universite Paris 8
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

#ifndef DEF_CICM_FILTER
#define DEF_CICM_FILTER

#include "cicmTools.h"

class CicmFilter
{
protected:
	
	Cicm_Signal*	m_buffer;
	long			m_buffer_size;
	long			m_index;

	Cicm_Signal		m_result;

public:
	CicmFilter(long aBufferSize = 128);
	virtual inline Cicm_Signal process();
	long	getBufferSize() ;
	~CicmFilter();
};

CicmFilter::CicmFilter(long aBufferSize)
{
	m_buffer_size = Tools::clip_min(aBufferSize, long(1));
	Cicm_signal_malloc(m_buffer, m_buffer_size);
	m_result = 0.;
	m_index = 0;
}

long CicmFilter::getBufferSize() 
{
	return m_buffer_size;
}

CicmFilter::~CicmFilter()
{
	Cicm_free(m_buffer);
}

#endif