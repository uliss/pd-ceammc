/*
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
* guillotpierre6@gmail.com
*/

extern "C"
{
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
}
#include "lsdSpectrum.h"

typedef struct  _spectrum 
{
	t_pxobject	f_ob;
	Spectrum*	f_spectrum;
	void*		f_out;
	
	void*		f_clock;
	int			f_startclock;
	int			f_interval;
	
	int			f_arraySize;
    long    f_amplitude_mode;
    long    f_frequency_mode;
    long    f_window_mode;
    
} t_spectrum;

t_class *spectrum_class;

void *spectrum_new(t_symbol *s, long argc, t_atom *argv);
void spectrum_free(t_spectrum *x);
void spectrum_assist(t_spectrum *x, void *b, long m, long a, char *s);
void spectrum_tick(t_spectrum *x);

void spectrum_dsp64(t_spectrum *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void spectrum_perform64(t_spectrum *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void spectrum_dsp(t_spectrum *x, t_signal **sp, short *count);
t_int *spectrum_perform(t_int *w);

t_max_err window_mode_set(t_spectrum *x, t_object *attr, long argc, t_atom *argv);
t_max_err amplitude_mode_set(t_spectrum *x, t_object *attr, long argc, t_atom *argv);
t_max_err frequency_mode_set(t_spectrum *x, t_object *attr, long argc, t_atom *argv);

int main()
{
	t_class *c = class_new("lsd.spectrum~", (method)spectrum_new, (method)dsp_free, (long)sizeof(t_spectrum), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)spectrum_dsp64,		"dsp64",		A_CANT,	0);
	class_addmethod(c, (method)spectrum_dsp,		"dsp",			A_CANT,	0);
	class_addmethod(c, (method)spectrum_assist,		"assist",		A_CANT,	0);
    
    CLASS_ATTR_LONG             (c, "window", 0, t_spectrum, f_window_mode);
    CLASS_ATTR_ACCESSORS		(c, "window", NULL, window_mode_set);
	CLASS_ATTR_LABEL			(c, "window", 0, "Window function");
	CLASS_ATTR_ENUMINDEX		(c, "window", 0, "Square \" \"Hanning \" \"Hamming \" \"Tukey \" \"Cosinus \" \"Lanczos \" \"Triangular \" \"Bartlett \" \"Gaussian \" \"BartlettHann \" \"Blackman \" \"Kaiser");
	CLASS_ATTR_DEFAULT			(c, "window", 0, "0");
	CLASS_ATTR_SAVE				(c, "window", 1);
    
    CLASS_ATTR_LONG             (c, "magnitude", 0, t_spectrum, f_amplitude_mode);
    CLASS_ATTR_ACCESSORS		(c, "magnitude", NULL, amplitude_mode_set);
	CLASS_ATTR_LABEL			(c, "magnitude", 0, "Magnitude Scale");
	CLASS_ATTR_ENUMINDEX		(c, "magnitude", 0, "Amplitude \" \"Power \" \"Decibel");
	CLASS_ATTR_DEFAULT			(c, "magnitude", 0, "0");
	CLASS_ATTR_SAVE				(c, "magnitude", 1);
    
    CLASS_ATTR_LONG             (c, "frequency", 0, t_spectrum, f_frequency_mode);
    CLASS_ATTR_ACCESSORS		(c, "frequency", NULL, frequency_mode_set);
	CLASS_ATTR_LABEL			(c, "frequency", 0, "Magnitude Scale");
	CLASS_ATTR_ENUMINDEX		(c, "frequency", 0, "Linear \" \"Logarithmic");
	CLASS_ATTR_DEFAULT			(c, "frequency", 0, "0");
	CLASS_ATTR_SAVE				(c, "frequency", 1);
	
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	spectrum_class = c;

	post("lsd.library by Pierre Guillot",0);
	post("Copyright (C) 2012, Universite Paris 8");
	
	return 0;
}

void *spectrum_new(t_symbol *s, long argc, t_atom *argv)
{
	t_spectrum *x = NULL;
	int			windowSize		= 1024;
	int			numberOfWindow	= 2;
	
    x = (t_spectrum *)object_alloc(spectrum_class);
	if(x)
	{
		/* Sampling Rate initialization **************/
        if(atom_gettype(argv) == A_LONG)
			windowSize	= atom_getlong(argv);
		if(atom_gettype(argv+1) == A_LONG)
			numberOfWindow = atom_getlong(argv+1);
		if(atom_gettype(argv+2) == A_SYM)
            
		x->f_spectrum = new Spectrum(windowSize, numberOfWindow);
		dsp_setup((t_pxobject *)x, 1);
		x->f_out = listout(x);
		attr_args_process(x, argc, argv);
		x->f_arraySize = 0;
		x->f_clock = clock_new(x,(method)spectrum_tick);
		x->f_startclock = 0;
		x->f_interval = 100;
        
        attr_args_process(x, argc, argv);
		
	}
	return (x);
}			

void spectrum_dsp64(t_spectrum *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_spectrum->setVectorSize(maxvectorsize);
    x->f_spectrum->setSamplingRate(samplerate);
	object_method(dsp64, gensym("dsp_add64"), x, spectrum_perform64, 0, NULL);
	x->f_startclock = 1;
}

void spectrum_perform64(t_spectrum *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->f_spectrum->process(ins[0]);
	if (x->f_startclock) 
	{
		x->f_startclock = 0;
		clock_delay(x->f_clock,0);
	}
}


void spectrum_dsp(t_spectrum *x, t_signal **sp, short *count)
{
	x->f_spectrum->setVectorSize(sp[0]->s_n);
    x->f_spectrum->setSamplingRate(sp[0]->s_sr);
	dsp_add(spectrum_perform, 2, x, sp[0]->s_vec);
	x->f_startclock = 1;
}


t_int *spectrum_perform(t_int *w)
{	
	t_spectrum	*x		= (t_spectrum *)(w[1]);
	float		*ins	= (float *)(w[2]);

	x->f_spectrum->process(ins);
	if (x->f_startclock) 
	{
		x->f_startclock = 0;
		clock_delay(x->f_clock,0);
	}
	return (w + 3);	
}


void spectrum_tick(t_spectrum *x)
{
	if (sys_getdspstate())
	{
		t_atom list[x->f_spectrum->getArraySize()];
		for(int i = 0; i < x->f_spectrum->getArraySize(); i++)
			atom_setfloat(list+i, x->f_spectrum->getAmplitude(i));
		
		outlet_list(x->f_out, NULL, x->f_spectrum->getArraySize(), list);
		clock_fdelay(x->f_clock, x->f_interval);
	}
}

void spectrum_assist(t_spectrum *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) 
	{
		switch (a) 
		{	
			case 0:
				sprintf(s,"(Signal) Input");
				break;
		}
	}
	else 
	{
		switch (a) 
		{	
			case 0:
				sprintf(s,"(List) Spectrum");
				break;
		}
	}
}
	
void spectrum_free(t_spectrum *x)
{
	dsp_free((t_pxobject *)x);
    freeobject((t_object *)x->f_clock);
	delete x->f_spectrum;
}

t_max_err frequency_mode_set(t_spectrum *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG) x->f_frequency_mode = atom_getlong(argv);
    
	else if (atom_gettype(argv) == A_FLOAT) x->f_frequency_mode = (int)atom_getfloat(argv);
    
	else if (atom_gettype(argv) == A_SYM)
    {
        if(atom_getsym(argv) == gensym("logarithmic"))
            x->f_frequency_mode = 1;
        else
            x->f_frequency_mode = 0;
    }
    if(x->f_frequency_mode > 1)
        x->f_frequency_mode = 1;
    else if(x->f_frequency_mode < 0)
        x->f_frequency_mode = 0;
    
    if(x->f_frequency_mode == 0)
        x->f_spectrum->setFrequencyMode("linear");
    else if(x->f_frequency_mode == 1)
        x->f_spectrum->setFrequencyMode("logarithmic");
    
	return 0;
}

t_max_err amplitude_mode_set(t_spectrum *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG) x->f_amplitude_mode = atom_getlong(argv);
    
	else if (atom_gettype(argv) == A_FLOAT) x->f_amplitude_mode = (int)atom_getfloat(argv);
    
	else if (atom_gettype(argv) == A_SYM)
    {
		if(atom_getsym(argv) == gensym("power"))
            x->f_amplitude_mode = 1;
        else if(atom_getsym(argv) == gensym("decibel"))
            x->f_amplitude_mode = 2;
        else
            x->f_amplitude_mode = 0;
    }
    if(x->f_amplitude_mode > 2)
        x->f_amplitude_mode = 2;
    else if(x->f_amplitude_mode < 0)
        x->f_amplitude_mode = 0;

    if(x->f_amplitude_mode == 0)
        x->f_spectrum->setAmplitudeMode("amplitude");
    else if(x->f_amplitude_mode == 1)
        x->f_spectrum->setAmplitudeMode("power");
    else if(x->f_amplitude_mode == 2)
        x->f_spectrum->setAmplitudeMode("decibel");
        
	return 0;
}

t_max_err window_mode_set(t_spectrum *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG) x->f_window_mode = atom_getlong(argv);
    
	else if (atom_gettype(argv) == A_FLOAT) x->f_window_mode = (int)atom_getfloat(argv);
    
	else if (atom_gettype(argv) == A_SYM)
    {
        if(atom_getsym(argv) == gensym("square"))
            x->f_window_mode = 0;
        else if(atom_getsym(argv) == gensym("hanning"))
            x->f_window_mode = 1;
        else if(atom_getsym(argv) == gensym("hamming"))
            x->f_window_mode = 2;
        else if(atom_getsym(argv) == gensym("tukey"))
            x->f_window_mode = 3;
        else if(atom_getsym(argv) == gensym("cosinus"))
            x->f_window_mode = 4;
        else if(atom_getsym(argv) == gensym("lanczos"))
            x->f_window_mode = 5;
        else if(atom_getsym(argv) == gensym("triangular"))
            x->f_window_mode = 6;
        else if(atom_getsym(argv) == gensym("bartlett"))
            x->f_window_mode = 7;
        else if(atom_getsym(argv) == gensym("gaussian"))
            x->f_window_mode = 8;
        else if(atom_getsym(argv) == gensym("bartlettHann"))
            x->f_window_mode = 9;
        else if(atom_getsym(argv) == gensym("blackman"))
            x->f_window_mode = 10;
        else if(atom_getsym(argv) == gensym("kaiser"))
            x->f_window_mode = 11;
        else
            x->f_window_mode = 1;
    }
    if(x->f_window_mode > 11)
        x->f_window_mode = 11;
    else if(x->f_window_mode < 0)
        x->f_window_mode = 0;
    
    if(x->f_window_mode == 0)
        x->f_spectrum->setWindowFunction("square");
    else if(x->f_window_mode == 1)
        x->f_spectrum->setWindowFunction("hanning");
    else if(x->f_window_mode == 2)
        x->f_spectrum->setWindowFunction("hamming");
    else if(x->f_window_mode == 3)
        x->f_spectrum->setWindowFunction("tukey");
    else if(x->f_window_mode == 4)
        x->f_spectrum->setWindowFunction("cosinus");
    else if(x->f_window_mode == 5)
        x->f_spectrum->setWindowFunction("lanczos");
    else if(x->f_window_mode == 6)
        x->f_spectrum->setWindowFunction("triangular");
    else if(x->f_window_mode == 7)
        x->f_spectrum->setWindowFunction("bartlett");
    else if(x->f_window_mode == 8)
        x->f_spectrum->setWindowFunction("gaussian");
    else if(x->f_window_mode == 9)
        x->f_spectrum->setWindowFunction("bartlettHann");
    else if(x->f_window_mode == 10)
        x->f_spectrum->setWindowFunction("blackman");
    else if(x->f_window_mode == 11)
        x->f_spectrum->setWindowFunction("kaiser");    
    
	return 0;
}