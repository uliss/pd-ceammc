
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

#include "Components/revGigaverb.h"

extern "C" 
{
	#include "ext.h"
	#include "ext_obex.h"
	#include "z_dsp.h"
	#include "buffer.h"
	#include "ext_globalsymbol.h"
}


typedef struct _gigaverb
{
	t_pxobject		f_ob;	
	Gigaverb		*f_gigaverb;
	long			f_vectorSize;
	float			f_size;
	float			f_damp;
	float			f_time;
	float			f_bandwidth;
	float			f_earlygain;
	float			f_tailgain;
}
t_gigaverb ;

void *gigaverb_new(t_symbol *s, long argc, t_atom *argv);
void gigaverb_free(t_gigaverb *x);
void gigaverb_assist(t_gigaverb *x, void *b, long m, long a, char *s);

t_max_err size_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err damp_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err reverb_time_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err imput_bandwidth_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err early_gain_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err tail_gain_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv);

void gigaverb_dsp(t_gigaverb *x, t_signal **sp, short *count);
t_int *gigaverb_perform(t_int *w);

void gigaverb_dsp64(t_gigaverb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void gigaverb_perform64(t_gigaverb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

t_class *gigaverb_class;

int main(void)
{	
	t_class *c;
	
	c = class_new("rev.gigaverb~", (method)gigaverb_new, (method)gigaverb_free, (long)sizeof(t_gigaverb ), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)gigaverb_dsp,		"dsp",			A_CANT, 0);
	class_addmethod(c, (method)gigaverb_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)gigaverb_assist,		"assist",		A_CANT, 0);

	CLASS_ATTR_FLOAT			(c, "size", 0, t_gigaverb, f_size);
	CLASS_ATTR_CATEGORY			(c, "size", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "size", 0, "Room size");
	CLASS_ATTR_ORDER			(c, "size", 0, "1");
	CLASS_ATTR_ACCESSORS		(c, "size", NULL, size_set);
	CLASS_ATTR_DEFAULT			(c, "size", 0, "50.");
	CLASS_ATTR_SAVE				(c, "size", 1);

	CLASS_ATTR_FLOAT			(c, "damp", 0, t_gigaverb, f_damp);
	CLASS_ATTR_CATEGORY			(c, "damp", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "damp", 0, "Damping value");
	CLASS_ATTR_ORDER			(c, "damp", 0, "2");
	CLASS_ATTR_ACCESSORS		(c, "damp", NULL, damp_set);
	CLASS_ATTR_DEFAULT			(c, "damp", 0, "0.5");
	CLASS_ATTR_FILTER_MIN		(c, "damp", 0);
	CLASS_ATTR_SAVE				(c, "damp", 1);
	
	CLASS_ATTR_FLOAT			(c, "time", 0, t_gigaverb, f_time);
	CLASS_ATTR_CATEGORY			(c, "time", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "time", 0, "Reverberation time");
	CLASS_ATTR_ORDER			(c, "time", 0, "3");
	CLASS_ATTR_ACCESSORS		(c, "time", NULL, reverb_time_set);
	CLASS_ATTR_DEFAULT			(c, "time", 0, "7.");
	CLASS_ATTR_FILTER_MIN		(c, "time", 0);
	CLASS_ATTR_SAVE				(c, "time", 1);

	CLASS_ATTR_FLOAT			(c, "bandwidth", 0, t_gigaverb, f_bandwidth);
	CLASS_ATTR_CATEGORY			(c, "bandwidth", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "bandwidth", 0, "Input bandwidth");
	CLASS_ATTR_ORDER			(c, "bandwidth", 0, "4");
	CLASS_ATTR_ACCESSORS		(c, "bandwidth", NULL, imput_bandwidth_set);
	CLASS_ATTR_DEFAULT			(c, "bandwidth", 0, "0.5");
	CLASS_ATTR_FILTER_MIN		(c, "bandwidth", 0);
	CLASS_ATTR_SAVE				(c, "bandwidth", 1);

	CLASS_ATTR_FLOAT			(c, "egain", 0, t_gigaverb, f_earlygain);
	CLASS_ATTR_CATEGORY			(c, "egain", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "egain", 0, "Early gain");
	CLASS_ATTR_ORDER			(c, "egain", 0, "5");
	CLASS_ATTR_ACCESSORS		(c, "egain", NULL, early_gain_set);
	CLASS_ATTR_DEFAULT			(c, "egain", 0, "1.");
	CLASS_ATTR_FILTER_MIN		(c, "egain", 0);
	CLASS_ATTR_SAVE				(c, "egain", 1);

	CLASS_ATTR_FLOAT			(c, "tgain", 0, t_gigaverb, f_tailgain);
	CLASS_ATTR_CATEGORY			(c, "tgain", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "tgain", 0, "Tail gain");
	CLASS_ATTR_ORDER			(c, "tgain", 0, "6");
	CLASS_ATTR_ACCESSORS		(c, "tgain", NULL, tail_gain_set);
	CLASS_ATTR_DEFAULT			(c, "tgain", 0, "1.");
	CLASS_ATTR_FILTER_MIN		(c, "tgain", 0);
	CLASS_ATTR_SAVE				(c, "tgain", 1);

	class_dspinit(c);				
	class_register(CLASS_BOX, c);	
	gigaverb_class = c;
	
	class_findbyname(CLASS_BOX, gensym("rev.freeverb~"));
	
	return 0;
}

void *gigaverb_new(t_symbol *s, long argc, t_atom *argv)
{
	t_gigaverb *x = NULL;
	int aSide = 0;
    x = (t_gigaverb *)object_alloc(gigaverb_class);
	if (x)
	{	
		if(atom_gettype(argv) == A_LONG)
			aSide = atom_getlong(argv);
		else if(atom_gettype(argv) == A_FLOAT)
			aSide = atom_getfloat(argv);
		else if(atom_gettype(argv) == A_SYM)
		{
			if(atom_getsym(argv) == gensym("right"))
				aSide = 1;
		}

		 if(aSide <= 0)
			 aSide = 0;

		x->f_gigaverb = new Gigaverb(aSide);
		
		dsp_setup((t_pxobject *)x, 1);	
		outlet_new(x, "signal");
		attr_args_process(x, argc, argv);
	}
	return (x);
}

void gigaverb_dsp(t_gigaverb *x, t_signal **sp, short *count)
{
	x->f_vectorSize = sp[0]->s_n;
	dsp_add(gigaverb_perform, 3, x, sp[0]->s_vec, sp[1]->s_vec);
}

t_int *gigaverb_perform(t_int *w)
{
	t_gigaverb*	x	= (t_gigaverb *)(w[1]);
	float**		ins	= (float **)(w+2);
	float**		outs= (float **)(w+3);
	
	for(int i = 0; i < x->f_vectorSize; i++)
		outs[0][i] = x->f_gigaverb->perform(ins[0][i]);
	
	return (w + 4);
}

void gigaverb_dsp64(t_gigaverb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_vectorSize = maxvectorsize;
	object_method(dsp64, gensym("dsp_add64"), x, gigaverb_perform64, 0, NULL);
}

void gigaverb_perform64(t_gigaverb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	for(int i = 0; i < x->f_vectorSize; i++)
		outs[0][i] = x->f_gigaverb->perform(ins[0][i]);
}

void gigaverb_assist(t_gigaverb *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) 
	{
		sprintf(s,"(Signal) Input signal");
	} 
	else 
	{
		sprintf(s,"(Signal) Reverberated signal"); 			
	}
}

void gigaverb_free(t_gigaverb *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_gigaverb;
}


t_max_err size_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_gigaverb->setRoomSize(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_gigaverb->setRoomSize(atom_getfloat(argv));

	x->f_size = x->f_gigaverb->getRoomSize();
	return 0;
}

t_max_err damp_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_gigaverb->setDamping(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_gigaverb->setDamping(atom_getfloat(argv));

	x->f_damp = x->f_gigaverb->getDamping();
	return 0;
}

t_max_err reverb_time_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_gigaverb->setReverberationTime(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_gigaverb->setReverberationTime(atom_getfloat(argv));

	x->f_time = x->f_gigaverb->getReverberationTime();
	return 0;
}

t_max_err imput_bandwidth_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_gigaverb->setInputBandwidth(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_gigaverb->setInputBandwidth(atom_getfloat(argv));

	x->f_bandwidth= x->f_gigaverb->getInputBandwidth();
	return 0;
}

t_max_err early_gain_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_gigaverb->setEarlyLevel(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_gigaverb->setEarlyLevel(atom_getfloat(argv));

	x->f_earlygain = x->f_gigaverb->getEarlyLevel();
	return 0;
}

t_max_err tail_gain_set(t_gigaverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_gigaverb->setTailLevel(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_gigaverb->setTailLevel(atom_getfloat(argv));

	x->f_tailgain= x->f_gigaverb->getTailLevel();
	return 0;
}
