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

#include "revNewverb.h"

Newverb::Newverb(int aSide)
{
	m_side = aSide;
	if(m_side == 0)
	{
		m_comb_filter[0].setBuffer(m_comb_buffer1,combtuningL1);
		m_comb_filter[1].setBuffer(m_comb_buffer2,combtuningL2);
		m_comb_filter[2].setBuffer(m_comb_buffer3,combtuningL3);
		m_comb_filter[3].setBuffer(m_comb_buffer4,combtuningL4);
		m_comb_filter[4].setBuffer(m_comb_buffer5,combtuningL5);
		m_comb_filter[5].setBuffer(m_comb_buffer6,combtuningL6);
		m_comb_filter[6].setBuffer(m_comb_buffer7,combtuningL7);
		m_comb_filter[7].setBuffer(m_comb_buffer8,combtuningL8);
		m_allpass_filter[0].setBuffer(m_allpass_buffer1,allpasstuningL1);
		m_allpass_filter[1].setBuffer(m_allpass_buffer2,allpasstuningL2);
		m_allpass_filter[2].setBuffer(m_allpass_buffer3,allpasstuningL3);
		m_allpass_filter[3].setBuffer(m_allpass_buffer4,allpasstuningL4);
	}
	else
	{
		m_comb_filter[0].setBuffer(m_comb_buffer1r,combtuningR1);
		m_comb_filter[1].setBuffer(m_comb_buffer2r,combtuningR2);
		m_comb_filter[2].setBuffer(m_comb_buffer3r,combtuningR3);
		m_comb_filter[3].setBuffer(m_comb_buffer4r,combtuningR4);
		m_comb_filter[4].setBuffer(m_comb_buffer5r,combtuningR5);
		m_comb_filter[5].setBuffer(m_comb_buffer6r,combtuningR6);
		m_comb_filter[6].setBuffer(m_comb_buffer7r,combtuningR7);
		m_comb_filter[7].setBuffer(m_comb_buffer8r,combtuningR8);
		m_allpass_filter[0].setBuffer(m_allpass_buffer1r,allpasstuningR1);
		m_allpass_filter[1].setBuffer(m_allpass_buffer2r,allpasstuningR2);
		m_allpass_filter[2].setBuffer(m_allpass_buffer3r,allpasstuningR3);
		m_allpass_filter[3].setBuffer(m_allpass_buffer4r,allpasstuningR4);
	}

	m_allpass_filter[0].setFeedback(0.5f);
	m_allpass_filter[1].setFeedback(0.5f);
	m_allpass_filter[2].setFeedback(0.5f);
	m_allpass_filter[3].setFeedback(0.5f);

	setroomsize(initialroom);
	setdamp(initialdamp);
	setmode(initialmode);
}

void Newverb::update()
{
	if (mode >= freezemode)
	{
		roomsize1 = 1;
		damp1 = 0;
		m_gain = muted;
	}
	else
	{
		roomsize1 = roomsize;
		damp1 = damp;
		m_gain = fixedgain;
	}

	for(int i = 0; i < numcombs; i++)
	{
		m_comb_filter[i].setFeedback(roomsize1);
		m_comb_filter[i].setDamp(damp1);
	}
}

void Newverb::setroomsize(double value)
{
	if(value <= 0.)
		value = 0.;
	else if (value >= 1.)
		value = 1;
	roomsize = (value*scaleroom) + offsetroom;
	update();
}

double Newverb::getroomsize()
{
	return (roomsize-offsetroom)/scaleroom;
}

void Newverb::setdamp(double value)
{
	if(value <= 0.)
		value = 0.;
	else if (value >= 1.)
		value = 1;
	damp = value*scaledamp;
	update();
}

double Newverb::getdamp()
{
	return damp/scaledamp;
}

void Newverb::setmode(double value)
{
	if(value <= 0.)
		value = 0.;
	else if (value >= 1.)
		value = 1;
	mode = value;
	update();
}

double Newverb::getmode()
{
	if (mode >= freezemode)
		return 1;
	else
		return 0;
}
