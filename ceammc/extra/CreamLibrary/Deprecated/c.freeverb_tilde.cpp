/*
 * Cream Library
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CreamLibrary
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "../c.library.hpp"

const int	numcombs		= 8;
const int	numallpasses	= 4;
const float	muted           = 0;
const float	fixedgain       = 0.015;
const float scaledamp		= 0.4;
const float scaleroom		= 0.28;
const float offsetroom		= 0.7;
const float initialroom     = 0.5;
const float initialdamp     = 0.5;
const float initialmode     = 0;
const float freezemode		= 0.5;
const int	stereospread	= 23;

// These values assume 44.1KHz sample rate
// they will probably be OK for 48KHz sample rate
// but would need scaling for 96KHz (or other) sample rates.
// The values were obtained by listening tests.
const int combtuningL1		= 1116;
const int combtuningR1		= 1116+stereospread;
const int combtuningL2		= 1188;
const int combtuningR2		= 1188+stereospread;
const int combtuningL3		= 1277;
const int combtuningR3		= 1277+stereospread;
const int combtuningL4		= 1356;
const int combtuningR4		= 1356+stereospread;
const int combtuningL5		= 1422;
const int combtuningR5		= 1422+stereospread;
const int combtuningL6		= 1491;
const int combtuningR6		= 1491+stereospread;
const int combtuningL7		= 1557;
const int combtuningR7		= 1557+stereospread;
const int combtuningL8		= 1617;
const int combtuningR8		= 1617+stereospread;
const int allpasstuningL1	= 556;
const int allpasstuningR1	= 556+stereospread;
const int allpasstuningL2	= 441;
const int allpasstuningR2	= 441+stereospread;
const int allpasstuningL3	= 341;
const int allpasstuningR3	= 341+stereospread;
const int allpasstuningL4	= 225;
const int allpasstuningR4	= 225+stereospread;

/*******************************************************/
/** y(n) = a * x(n) + b * x(n-delay) + c * y(n-delay) **/
/*******************************************************/

class CombFilter
{
private:
	float	m_feedback;
	float	m_feedforward;
	float	m_gain;
	float	m_delay;
	
	float	m_filterstore;
	float	m_damp1;
	float	m_damp2;
    
	float* m_buffer;
	int	m_buffer_size;
	int	m_ramp;
    
	float	m_output;
public:
    CombFilter();
    void	setBuffer(float *aBuffer, int aBufferSize);
    int	getBufferSize() ;
	
    void	setDamp(float aDampValue);
    float	getDamp();
	
    void	setFeedback(float aFeedbackValue);
    float	getFeedback();
    void	setFeedforward(float aFeedforwardValue);
    float	getFeedforward();
    void	setDelay(float aValue);
    float	getDelay();
	
    inline  float process(float anInput);
    ~CombFilter(){};
};

inline float CombFilter::process(const float anInput)
{
	m_output = m_buffer[m_ramp];
	m_filterstore = (m_output * m_damp2) + (m_filterstore * m_damp1);
	m_buffer[m_ramp] = anInput + (m_filterstore * m_feedback);
	if(++m_ramp >= m_buffer_size)
		m_ramp = 0;
	
	return m_output;
}

class AllpassFilter
{
private:
	float	m_feedback;
    
	float*	m_buffer;
	int	m_buffer_size;
	int	m_ramp;
    
	float	m_output;
    
public:
    AllpassFilter();
    void	setBuffer(float *aBuffer, int aBufferSize);
    int	getBufferSize() ;
    void	setFeedback(float val);
    float	getFeedback();
    inline  float	process(float anInput);
    ~AllpassFilter(){};
};

inline float AllpassFilter::process(const float anInput)
{
	m_output = -anInput + m_buffer[m_ramp];
	m_buffer[m_ramp] = anInput + (m_buffer[m_ramp] * m_feedback);
	if(++m_ramp >= m_buffer_size)
		m_ramp = 0;
    
	return m_output;
}

class Freeverb
{
private:
	char    m_side;
	float	m_gain;
	float	roomsize,roomsize1;
	float	damp,damp1;
	float	mode;
    
	CombFilter		m_comb_filter[numcombs];
	AllpassFilter	m_allpass_filter[numallpasses];
    
	float	m_comb_buffer1[combtuningL1];
	float	m_comb_buffer2[combtuningL2];
	float	m_comb_buffer3[combtuningL3];
	float	m_comb_buffer4[combtuningL4];
	float	m_comb_buffer5[combtuningL5];
	float	m_comb_buffer6[combtuningL6];
	float	m_comb_buffer7[combtuningL7];
	float	m_comb_buffer8[combtuningL8];
    
	float	m_allpass_buffer1[allpasstuningL1];
	float	m_allpass_buffer2[allpasstuningL2];
	float	m_allpass_buffer3[allpasstuningL3];
	float	m_allpass_buffer4[allpasstuningL4];
    
	float	m_comb_buffer1r[combtuningR1];
	float	m_comb_buffer2r[combtuningR2];
	float	m_comb_buffer3r[combtuningR3];
	float	m_comb_buffer4r[combtuningR4];
	float	m_comb_buffer5r[combtuningR5];
	float	m_comb_buffer6r[combtuningR6];
	float	m_comb_buffer7r[combtuningR7];
	float	m_comb_buffer8r[combtuningR8];
    
	float	m_allpass_buffer1r[allpasstuningR1];
	float	m_allpass_buffer2r[allpasstuningR2];
	float	m_allpass_buffer3r[allpasstuningR3];
	float	m_allpass_buffer4r[allpasstuningR4];
    
	void	update();
    
public:
	Freeverb(char aSide);
 	inline float process(float anInput);
	void	setroomsize(float value);
	float	getroomsize();
	void	setdamp(float value);
	float	getdamp();
	void	setmode(float value);
	float	getmode();
	~Freeverb(){};
};


inline float Freeverb::process(float anInput)
{
	float out = 0.f, in;
    in = anInput * m_gain;
    for(int j = 0; j < numcombs; j++)
        out += m_comb_filter[j].process(in);
    
    for(int j = 0; j < numallpasses; j++)
        out = m_allpass_filter[j].process(out);
    
    return out;

}

typedef struct _freeverb
{
	t_edspobj   j_box;
    Freeverb*   f_freeverb;
} t_freeverb;

t_eclass *freeverb_class;

void *freeverb_new(t_symbol *s, int argc, t_atom *argv);
void freeverb_free(t_freeverb *x);
void freeverb_assist(t_freeverb *x, void *b, long m, long a, char *s);

void freeverb_dsp(t_freeverb *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void freeverb_perform(t_freeverb *x, t_object *d, t_sample **ins, long ni, t_sample **outs, long no, long sf, long f,void *up);
void freeverb_roomsize(t_freeverb *x, float f);
void freeverb_damp(t_freeverb *x, float f);
void freeverb_freeze(t_freeverb *x, float f);

extern "C"  void setup_c0x2efreeverb_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.freeverb~", (method)freeverb_new, (method)freeverb_free, (short)sizeof(t_freeverb), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);

    
    eclass_addmethod(c, (method) freeverb_dsp,             "dsp",              A_NULL, 0);
	eclass_addmethod(c, (method) freeverb_assist,          "assist",           A_NULL, 0);
    
    eclass_addmethod(c, (method) freeverb_roomsize,        "roomsize",         A_FLOAT, 0);
    eclass_addmethod(c, (method) freeverb_damp,            "damp",             A_FLOAT, 0);
    eclass_addmethod(c, (method) freeverb_freeze,          "freeze",           A_FLOAT, 0);
    eclass_register(CLASS_OBJ, c);
    freeverb_class = c;
}

void *freeverb_new(t_symbol *s, int argc, t_atom *argv)
{
	t_freeverb *x =  NULL;

	x = (t_freeverb *)eobj_new(freeverb_class);
	
    eobj_dspsetup((t_ebox *)x, 1, 1);
    
    if(argc && argv && atom_gettype(argv) == A_FLOAT)
    {
        if(!atom_getfloat(argv))
            x->f_freeverb = new Freeverb(0);
        else
            x->f_freeverb = new Freeverb(1);
    }
    else if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        if(atom_getsymbol(argv) == gensym("right"))
            x->f_freeverb = new Freeverb(1);
        else
            x->f_freeverb = new Freeverb(0);
    }
    else
        x->f_freeverb = new Freeverb(0);    
    
    x->f_freeverb->setdamp(initialdamp);
    x->f_freeverb->setroomsize(initialroom);
    
	return (x);
}

void freeverb_roomsize(t_freeverb *x, float f)
{
    x->f_freeverb->setroomsize(f);
}

void freeverb_damp(t_freeverb *x, float f)
{
    x->f_freeverb->setdamp(f);
}

void freeverb_freeze(t_freeverb *x, float f)
{
    x->f_freeverb->setmode(f);
}

void freeverb_free(t_freeverb *x)
{
	eobj_dspfree((t_ebox *)x);
    delete x->f_freeverb;
}

void freeverb_assist(t_freeverb *x, void *b, long m, long a, char *s)
{
	;
}

void freeverb_dsp(t_freeverb *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add"), x, (method)freeverb_perform, 0, NULL);
}

void freeverb_perform(t_freeverb *x, t_object *d, t_sample **ins, long ni, t_sample **outs, long no, long sampleframes, long f,void *up)
{
    for(int i = 0; i < sampleframes; i++)
    {
        outs[0][i] = x->f_freeverb->process(ins[0][i]);
    }
}


Freeverb::Freeverb(char aSide)
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
    
    setmode(initialmode);
	setroomsize(initialroom);
	setdamp(initialdamp);
}

void Freeverb::update()
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

void Freeverb::setroomsize(float value)
{
	if(value <= 0.)
		value = 0.;
	else if (value >= 1.)
		value = 1;
	roomsize = (value*scaleroom) + offsetroom;
	update();
}

float Freeverb::getroomsize()
{
	return (roomsize-offsetroom)/scaleroom;
}

void Freeverb::setdamp(float value)
{
	if(value <= 0.)
		value = 0.;
	else if (value >= 1.)
		value = 1;
	damp = value*scaledamp;
	update();
}

float Freeverb::getdamp()
{
	return damp/scaledamp;
}

void Freeverb::setmode(float value)
{
	if(value <= 0.)
		value = 0.;
	else if (value >= 1.)
		value = 1;
	mode = value;
	update();
}

float Freeverb::getmode()
{
	if (mode >= freezemode)
		return 1;
	else
		return 0;
}

CombFilter::CombFilter()
{
	m_filterstore = 0;
	m_ramp = 0;
}

void CombFilter::setBuffer(float *aBuffer, int aBufferSize)
{
	m_buffer = aBuffer;
	m_buffer_size = aBufferSize;
	for(int i = 0; i < m_buffer_size; i++)
		m_buffer[i] = 0.;
}

int CombFilter::getBufferSize()
{
	return m_buffer_size;
}

void CombFilter::setDamp(float aDampValue)
{
	m_damp1 = aDampValue;
	m_damp2 = 1. - aDampValue;
}

float CombFilter::getDamp()
{
	return m_damp1;
}

void CombFilter::setFeedback(float aFeedbackValue)
{
	m_feedback = aFeedbackValue;
}

float CombFilter::getFeedback()
{
	return m_feedback;
}

void CombFilter::setFeedforward(float aFeedforwardValue)
{
	m_feedforward = aFeedforwardValue;
}

AllpassFilter::AllpassFilter()
{
	m_ramp = 0;
}

void AllpassFilter::setBuffer(float *aBuffer, int aBufferSize)
{
	m_buffer = aBuffer;
	m_buffer_size = aBufferSize;
	for(int i = 0; i < m_buffer_size; i++)
		m_buffer[i] = 0.;
}

int AllpassFilter::getBufferSize()
{
	return m_buffer_size;
}

void AllpassFilter::setFeedback(float aFeedbackValue)
{
	m_feedback = aFeedbackValue;
}

float AllpassFilter::getFeedback()
{
	return m_feedback;
}



