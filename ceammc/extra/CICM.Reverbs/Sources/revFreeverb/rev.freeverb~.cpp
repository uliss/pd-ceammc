
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

#include "revFreeverb.h"

extern "C" 
{
	#include "ext.h"
	#include "ext_obex.h"
	#include "z_dsp.h"
	#include "buffer.h"
	#include "ext_globalsymbol.h"
}


typedef struct _freeverb
{
	t_pxobject		f_ob;	
	Freeverb		*f_freeverb;
	long			f_vectorSize;
	long			f_freeze;
	float			f_size;
	float			f_damp;
}
t_freeverb ;

void *freeverb_new(t_symbol *s, long argc, t_atom *argv);
void freeverb_free(t_freeverb *x);
void freeverb_assist(t_freeverb *x, void *b, long m, long a, char *s);

t_max_err size_set(t_freeverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err damp_set(t_freeverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err freeze_set(t_freeverb *x, t_object *attr, long argc, t_atom *argv);

void freeverb_dsp(t_freeverb *x, t_signal **sp, short *count);
t_int *freeverb_perform(t_int *w);

void freeverb_dsp64(t_freeverb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void freeverb_perform64(t_freeverb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

t_class *freeverb_class;

int main(void)
{	
	t_class *c;
	
	c = class_new("rev.freeverb~", (method)freeverb_new, (method)freeverb_free, (long)sizeof(t_freeverb ), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)freeverb_dsp,		"dsp",			A_CANT, 0);
	class_addmethod(c, (method)freeverb_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)freeverb_assist,		"assist",		A_CANT, 0);

	CLASS_ATTR_FLOAT			(c, "size", 0, t_freeverb, f_size);
	CLASS_ATTR_CATEGORY			(c, "size", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "size", 0, "Room size");
	CLASS_ATTR_ORDER			(c, "size", 0, "1");
	CLASS_ATTR_ACCESSORS		(c, "size", NULL, size_set);
	CLASS_ATTR_DEFAULT			(c, "size", 0, "0.5");
	CLASS_ATTR_FILTER_MIN		(c, "size", 0);
	CLASS_ATTR_SAVE				(c, "size", 1);

	CLASS_ATTR_FLOAT			(c, "damp", 0, t_freeverb, f_damp);
	CLASS_ATTR_CATEGORY			(c, "damp", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "damp", 0, "Damping value");
	CLASS_ATTR_ORDER			(c, "damp", 0, "2");
	CLASS_ATTR_ACCESSORS		(c, "damp", NULL, damp_set);
	CLASS_ATTR_DEFAULT			(c, "damp", 0, "0.5");
	CLASS_ATTR_FILTER_MIN		(c, "damp", 0);
	CLASS_ATTR_SAVE				(c, "damp", 1);

	CLASS_ATTR_LONG				(c, "freeze", 0, t_freeverb, f_freeze);
	CLASS_ATTR_CATEGORY			(c, "freeze", 0, "Behaviors");
	CLASS_ATTR_STYLE_LABEL		(c, "freeze", 0, "onoff", "Freeze");
	CLASS_ATTR_ACCESSORS		(c, "freeze", NULL, freeze_set);
	CLASS_ATTR_DEFAULT			(c, "freeze", 0, "0");
	CLASS_ATTR_SAVE				(c, "freeze", 1);

	class_dspinit(c);				
	class_register(CLASS_BOX, c);	
	freeverb_class = c;
	
	post("rev.library by Pierre Guillot");
	post("Copyright (C) 2012, CICM / Universite Paris 8");
	
	return 0;
}

void *freeverb_new(t_symbol *s, long argc, t_atom *argv)
{
	t_freeverb *x = NULL;
	int aSide = 0;
    x = ((t_freeverb *)object_alloc(freeverb_class));
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

		x->f_freeverb = new Freeverb(aSide);
		
		dsp_setup((t_pxobject *)x, 1);	
		outlet_new(x, "signal");
		attr_args_process(x, argc, argv);
	}
	return (x);
}

void freeverb_dsp(t_freeverb *x, t_signal **sp, short *count)
{
	x->f_vectorSize = sp[0]->s_n;
	dsp_add(freeverb_perform, 3, x, sp[0]->s_vec, sp[1]->s_vec);
}

t_int *freeverb_perform(t_int *w)
{
	t_freeverb*	x	= (t_freeverb *)(w[1]);
	float**		ins	= (float **)(w+2);
	float**		outs= (float **)(w+3);

	x->f_freeverb->process(ins[0], outs[0],x->f_vectorSize);

	return (w + 4);
}

void freeverb_dsp64(t_freeverb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_vectorSize = maxvectorsize;
	object_method(dsp64, gensym("dsp_add64"), x, freeverb_perform64, 0, NULL);
}

void freeverb_perform64(t_freeverb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->f_freeverb->process(ins[0], outs[0], x->f_vectorSize);
}

void freeverb_assist(t_freeverb *x, void *b, long m, long a, char *s)
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

void freeverb_free(t_freeverb *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_freeverb;
}


t_max_err size_set(t_freeverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_freeverb->setroomsize(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_freeverb->setroomsize(atom_getfloat(argv));

	x->f_size = x->f_freeverb->getroomsize();
	return 0;
}


t_max_err damp_set(t_freeverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_freeverb->setdamp(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_freeverb->setdamp(atom_getfloat(argv));

	x->f_damp = x->f_freeverb->getdamp();
	return 0;
}

t_max_err freeze_set(t_freeverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_freeverb->setmode(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_freeverb->setmode(atom_getfloat(argv));

	x->f_freeze = x->f_freeverb->getmode();
	return 0;
}