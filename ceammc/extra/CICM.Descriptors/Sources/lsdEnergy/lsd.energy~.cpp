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
#include "lsdEnergy.h"

typedef struct  _energy 
{
	t_pxobject	f_ob;
	Energy*		f_energy;
    
    long    f_amplitude_mode;
    long    f_frequency_mode;
    long    f_window_mode;
	
} t_energy;

t_class *energy_class;

void *energy_new(t_symbol *s, long argc, t_atom *argv);
void energy_free(t_energy *x);
void energy_assist(t_energy *x, void *b, long m, long a, char *s);

void energy_dsp64(t_energy *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void energy_perform64(t_energy *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void energy_dsp(t_energy *x, t_signal **sp, short *count);
t_int *energy_perform(t_int *w);


t_max_err window_mode_set(t_energy *x, t_object *attr, long argc, t_atom *argv);
t_max_err amplitude_mode_set(t_energy *x, t_object *attr, long argc, t_atom *argv);
t_max_err frequency_mode_set(t_energy *x, t_object *attr, long argc, t_atom *argv);

int main()
{
	t_class *c = class_new("lsd.energy~", (method)energy_new, (method)dsp_free, (long)sizeof(t_energy), 0L, A_GIMME, 0);
	class_addmethod(c, (method)energy_dsp64,		"dsp64",		A_CANT,	0);
	class_addmethod(c, (method)energy_dsp,			"dsp",			A_CANT,	0);
	class_addmethod(c, (method)energy_assist,		"assist",		A_CANT,	0);
    
    CLASS_ATTR_LONG             (c, "window", 0, t_energy, f_window_mode);
    CLASS_ATTR_ACCESSORS		(c, "window", NULL, window_mode_set);
	CLASS_ATTR_LABEL			(c, "window", 0, "Window function");
	CLASS_ATTR_ENUMINDEX		(c, "window", 0, "Square \" \"Hanning \" \"Hamming \" \"Tukey \" \"Cosinus \" \"Lanczos \" \"Triangular \" \"Bartlett \" \"Gaussian \" \"BartlettHann \" \"Blackman \" \"Kaiser");
	CLASS_ATTR_DEFAULT			(c, "window", 0, "0");
	CLASS_ATTR_SAVE				(c, "window", 1);
    
    CLASS_ATTR_LONG             (c, "magnitude", 0, t_energy, f_amplitude_mode);
    CLASS_ATTR_ACCESSORS		(c, "magnitude", NULL, amplitude_mode_set);
	CLASS_ATTR_LABEL			(c, "magnitude", 0, "Magnitude Scale");
	CLASS_ATTR_ENUMINDEX		(c, "magnitude", 0, "Amplitude \" \"Power \" \"Decibel");
	CLASS_ATTR_DEFAULT			(c, "magnitude", 0, "0");
	CLASS_ATTR_SAVE				(c, "magnitude", 1);
    
    CLASS_ATTR_LONG             (c, "frequency", 0, t_energy, f_frequency_mode);
    CLASS_ATTR_ACCESSORS		(c, "frequency", NULL, frequency_mode_set);
	CLASS_ATTR_LABEL			(c, "frequency", 0, "Magnitude Scale");
	CLASS_ATTR_ENUMINDEX		(c, "frequency", 0, "Linear \" \"Logarithmic");
	CLASS_ATTR_DEFAULT			(c, "frequency", 0, "0");
	CLASS_ATTR_SAVE				(c, "frequency", 1);
	
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	energy_class = c;

	post("lsd.library by Pierre Guillot",0);
	post("Copyright (C) 2012, Universite Paris 8");
	
	return 0;
}

void *energy_new(t_symbol *s, long argc, t_atom *argv)
{
	t_energy *x = NULL;
	int			windowSize		= 1024;
	int			numberOfWindow	= 2;
	
    x = (t_energy *)object_alloc(energy_class);
	if(x)
	{
		/* Sampling Rate initialization **************/
		
		if(atom_gettype(argv) == A_LONG)
			windowSize	= atom_getlong(argv);
		if(atom_gettype(argv+1) == A_LONG)
			numberOfWindow = atom_getlong(argv+1);
		
		x->f_energy = new Energy(windowSize, numberOfWindow);
		dsp_setup((t_pxobject *)x, 1);
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");

		attr_args_process(x, argc, argv);
	}
	return (x);
}			

void energy_dsp64(t_energy *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_energy->setVectorSize(maxvectorsize);
	object_method(dsp64, gensym("dsp_add64"), x, energy_perform64, 0, NULL);
}

void energy_perform64(t_energy *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->f_energy->process(ins[0], outs[0], outs[1], outs[2], outs[3]);
}


void energy_dsp(t_energy *x, t_signal **sp, short *count)
{
	x->f_energy->setVectorSize(sp[0]->s_n);
	dsp_add(energy_perform, 6, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec);
}


t_int *energy_perform(t_int *w)
{	
	t_energy*x	= (t_energy *)(w[1]);
	float	**ins	= (float **)	(w+2);
	float	**outs	= (float **)(w+3);

	x->f_energy->process(ins[0], outs[0], outs[1], outs[2], outs[3]);
	return (w + 7);	
}
	
void energy_assist(t_energy *x, void *b, long m, long a, char *s)
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
	
void energy_free(t_energy *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_energy;
}


t_max_err frequency_mode_set(t_energy *x, t_object *attr, long argc, t_atom *argv)
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
        x->f_energy->setFrequencyMode("linear");
    else if(x->f_frequency_mode == 1)
        x->f_energy->setFrequencyMode("logarithmic");
    
	return 0;
}

t_max_err amplitude_mode_set(t_energy *x, t_object *attr, long argc, t_atom *argv)
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
        x->f_energy->setAmplitudeMode("amplitude");
    else if(x->f_amplitude_mode == 1)
        x->f_energy->setAmplitudeMode("power");
    else if(x->f_amplitude_mode == 2)
        x->f_energy->setAmplitudeMode("decibel");
    
	return 0;
}

t_max_err window_mode_set(t_energy *x, t_object *attr, long argc, t_atom *argv)
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
        x->f_energy->setWindowFunction("square");
    else if(x->f_window_mode == 1)
        x->f_energy->setWindowFunction("hanning");
    else if(x->f_window_mode == 2)
        x->f_energy->setWindowFunction("hamming");
    else if(x->f_window_mode == 3)
        x->f_energy->setWindowFunction("tukey");
    else if(x->f_window_mode == 4)
        x->f_energy->setWindowFunction("cosinus");
    else if(x->f_window_mode == 5)
        x->f_energy->setWindowFunction("lanczos");
    else if(x->f_window_mode == 6)
        x->f_energy->setWindowFunction("triangular");
    else if(x->f_window_mode == 7)
        x->f_energy->setWindowFunction("bartlett");
    else if(x->f_window_mode == 8)
        x->f_energy->setWindowFunction("gaussian");
    else if(x->f_window_mode == 9)
        x->f_energy->setWindowFunction("bartlettHann");
    else if(x->f_window_mode == 10)
        x->f_energy->setWindowFunction("blackman");
    else if(x->f_window_mode == 11)
        x->f_energy->setWindowFunction("kaiser");
    
	return 0;
}
