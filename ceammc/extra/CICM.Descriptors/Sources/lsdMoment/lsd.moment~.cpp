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
#include "LsdMoment.h"

typedef struct  _moment 
{
	t_pxobject	f_ob;
	Moment*		f_moment;
	
    long    f_amplitude_mode;
    long    f_frequency_mode;
    long    f_window_mode;
} t_moment;

t_class *Moment_class;

void *Moment_new(t_symbol *s, long argc, t_atom *argv);
void Moment_free(t_moment *x);
void Moment_assist(t_moment *x, void *b, long m, long a, char *s);

void Moment_dsp64(t_moment *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void Moment_perform64(t_moment *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void Moment_dsp(t_moment *x, t_signal **sp, short *count);
t_int *Moment_perform(t_int *w);

t_max_err window_mode_set(t_moment *x, t_object *attr, long argc, t_atom *argv);
t_max_err amplitude_mode_set(t_moment *x, t_object *attr, long argc, t_atom *argv);
t_max_err frequency_mode_set(t_moment *x, t_object *attr, long argc, t_atom *argv);

int main()
{
	t_class *c = class_new("lsd.moment~", (method)Moment_new, (method)dsp_free, (long)sizeof(t_moment), 0L, A_GIMME, 0);
	class_addmethod(c, (method)Moment_dsp64,		"dsp64",		A_CANT,	0);
	class_addmethod(c, (method)Moment_dsp,			"dsp",			A_CANT,	0);
	class_addmethod(c, (method)Moment_assist,		"assist",		A_CANT,	0);
	
    CLASS_ATTR_LONG             (c, "window", 0, t_moment, f_window_mode);
    CLASS_ATTR_ACCESSORS		(c, "window", NULL, window_mode_set);
	CLASS_ATTR_LABEL			(c, "window", 0, "Window function");
	CLASS_ATTR_ENUMINDEX		(c, "window", 0, "Square \" \"Hanning \" \"Hamming \" \"Tukey \" \"Cosinus \" \"Lanczos \" \"Triangular \" \"Bartlett \" \"Gaussian \" \"BartlettHann \" \"Blackman \" \"Kaiser");
	CLASS_ATTR_DEFAULT			(c, "window", 0, "0");
	CLASS_ATTR_SAVE				(c, "window", 1);
    
    CLASS_ATTR_LONG             (c, "magnitude", 0, t_moment, f_amplitude_mode);
    CLASS_ATTR_ACCESSORS		(c, "magnitude", NULL, amplitude_mode_set);
	CLASS_ATTR_LABEL			(c, "magnitude", 0, "Magnitude Scale");
	CLASS_ATTR_ENUMINDEX		(c, "magnitude", 0, "Amplitude \" \"Power \" \"Decibel");
	CLASS_ATTR_DEFAULT			(c, "magnitude", 0, "0");
	CLASS_ATTR_SAVE				(c, "magnitude", 1);
    
    CLASS_ATTR_LONG             (c, "frequency", 0, t_moment, f_frequency_mode);
    CLASS_ATTR_ACCESSORS		(c, "frequency", NULL, frequency_mode_set);
	CLASS_ATTR_LABEL			(c, "frequency", 0, "Magnitude Scale");
	CLASS_ATTR_ENUMINDEX		(c, "frequency", 0, "Linear \" \"Logarithmic");
	CLASS_ATTR_DEFAULT			(c, "frequency", 0, "0");
	CLASS_ATTR_SAVE				(c, "frequency", 1);
    
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	Moment_class = c;
	
	return 0;
}

void *Moment_new(t_symbol *s, long argc, t_atom *argv)
{
	t_moment *x = NULL;
	int			windowSize		= 1024;
	int			numberOfWindow	= 2;
	
    x = (t_moment *)object_alloc(Moment_class);
	if(x)
	{
		/* Sampling Rate initialization **************/
		
		if(atom_gettype(argv) == A_LONG)
			windowSize	= atom_getlong(argv);
		if(atom_gettype(argv+1) == A_LONG)
			numberOfWindow = atom_getlong(argv+1);
		
		x->f_moment = new Moment(windowSize, numberOfWindow); 
		dsp_setup((t_pxobject *)x, 1);
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");

		attr_args_process(x, argc, argv);
	}
	return (x);
}			

void Moment_dsp64(t_moment *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_moment->setVectorSize(maxvectorsize);
    x->f_moment->setSamplingRate(samplerate);
	object_method(dsp64, gensym("dsp_add64"), x, Moment_perform64, 0, NULL);
}

void Moment_perform64(t_moment *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->f_moment->process(ins[0], outs[0], outs[1], outs[2], outs[3]);
}


void Moment_dsp(t_moment *x, t_signal **sp, short *count)
{
	x->f_moment->setSamplingRate(sp[0]->s_sr);
	x->f_moment->setVectorSize(sp[0]->s_n);
	dsp_add(Moment_perform, 6, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec);
}


t_int *Moment_perform(t_int *w)
{	
	t_moment	*x		= (t_moment *)(w[1]);
	float	*in		= (float *)	(w[2]);
	float	*min	= (float *)	(w[3]);
	float	*ave	= (float *)	(w[4]);
	float	*max	= (float *)	(w[5]);
	float	*sum	= (float *)	(w[6]);

	x->f_moment->process(in, min, ave, max, sum);
	return (w + 7);	
}
	
void Moment_assist(t_moment *x, void *b, long m, long a, char *s)
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
				sprintf(s,"(Signal) Minimum");
				break;
			case 1:
				sprintf(s,"(Signal) Average");
				break;
			case 2:
				sprintf(s,"(Signal) Maximum");
				break;
			case 3:
				sprintf(s,"(Signal) Sum");
				break;
		}
	}
}
	
void Moment_free(t_moment *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_moment;
}

t_max_err frequency_mode_set(t_moment *x, t_object *attr, long argc, t_atom *argv)
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
        x->f_moment->setFrequencyMode("linear");
    else if(x->f_frequency_mode == 1)
        x->f_moment->setFrequencyMode("logarithmic");
    
	return 0;
}

t_max_err amplitude_mode_set(t_moment *x, t_object *attr, long argc, t_atom *argv)
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
        x->f_moment->setAmplitudeMode("amplitude");
    else if(x->f_amplitude_mode == 1)
        x->f_moment->setAmplitudeMode("power");
    else if(x->f_amplitude_mode == 2)
        x->f_moment->setAmplitudeMode("decibel");
    
	return 0;
}

t_max_err window_mode_set(t_moment *x, t_object *attr, long argc, t_atom *argv)
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
        x->f_moment->setWindowFunction("square");
    else if(x->f_window_mode == 1)
        x->f_moment->setWindowFunction("hanning");
    else if(x->f_window_mode == 2)
        x->f_moment->setWindowFunction("hamming");
    else if(x->f_window_mode == 3)
        x->f_moment->setWindowFunction("tukey");
    else if(x->f_window_mode == 4)
        x->f_moment->setWindowFunction("cosinus");
    else if(x->f_window_mode == 5)
        x->f_moment->setWindowFunction("lanczos");
    else if(x->f_window_mode == 6)
        x->f_moment->setWindowFunction("triangular");
    else if(x->f_window_mode == 7)
        x->f_moment->setWindowFunction("bartlett");
    else if(x->f_window_mode == 8)
        x->f_moment->setWindowFunction("gaussian");
    else if(x->f_window_mode == 9)
        x->f_moment->setWindowFunction("bartlettHann");
    else if(x->f_window_mode == 10)
        x->f_moment->setWindowFunction("blackman");
    else if(x->f_window_mode == 11)
        x->f_moment->setWindowFunction("kaiser");
    
	return 0;
}
