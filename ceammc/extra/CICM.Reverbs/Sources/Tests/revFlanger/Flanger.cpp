
#include "Flanger.h"

Flanger::Flanger()
{
	m_sampling_rate = 44100.;
	setDelay(.5);
	setFeedback(.5);
	setMode(1);
	setFrequency(.5);
	setDepth(.5);

	for(int i = 0; i < BUFFERSIZE; i++)
		m_buffer[i] = 0.;
	
    m_index		= 0;
	m_offsetter = 0.;
	m_phase		= 0.;
}

void Flanger::setSamplingRate(double aSamplingRate)
{
	double delay = getDelay();
	double depth = getDepth();
	double frequency = getFrequency();
	
	m_sampling_rate = Tools::clip_min(aSamplingRate, 1.);
	
	setDelay(delay);
	setDepth(depth);
	setFrequency(frequency);
}

double Flanger::getSamplingRate()
{
	return m_sampling_rate;
}

void Flanger::setVectorSize(long aVectorSize)
{
	m_vector_size = Tools::clip_power_of_two(aVectorSize);
}

long Flanger::getVectorSize()
{
	return m_vector_size;
}

void Flanger::setFeedback(double aFeedbackValue)
{
	m_feedback = Tools::clip(aFeedbackValue, 0., 1.);
}

double Flanger::getFeedback()
{
	return m_feedback;
}

void Flanger::setDelay(double aDelayValue)
{
    m_delay = Tools::clip_min(aDelayValue, 0.) * (m_sampling_rate * .02);
}

double Flanger::getDelay()
{
	return m_delay / (m_sampling_rate * .02);
}

void Flanger::setMode(long aMode)
{
	m_mode = Tools::clip(aMode, (long)0, (long)1);
}

long Flanger::getMode()
{
	return m_mode;
}

void Flanger::setDepth(double aDepthValue)
{
	m_depth = aDepthValue * (m_sampling_rate * 0.01);
}

double Flanger::getDepth()
{
	return m_depth / (m_sampling_rate * 0.01);
}

void Flanger::setFrequency(double aFrequency)
{
	m_frequency = aFrequency * (CICM_2PI / (m_sampling_rate / 2.));
	m_step		= aFrequency * (3. / m_sampling_rate);
}

double Flanger::getFrequency()
{
	return m_frequency / (CICM_2PI / (m_sampling_rate / 2.));
}

