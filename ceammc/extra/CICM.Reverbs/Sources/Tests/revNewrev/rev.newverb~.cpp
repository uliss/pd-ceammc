
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

#include "Components/revNewverb.h"

extern "C" 
{
	#include "ext.h"
	#include "ext_obex.h"
	#include "z_dsp.h"
	#include "buffer.h"
	#include "ext_globalsymbol.h"
}


typedef struct _newverb
{
	t_pxobject		f_ob;	
	Newverb			*f_newverb;
	long			f_vectorSize;
	long			f_freeze;
	float			f_size;
	float			f_damp;
}
t_newverb ;

void *newverb_new(t_symbol *s, long argc, t_atom *argv);
void newverb_free(t_newverb *x);
void newverb_assist(t_newverb *x, void *b, long m, long a, char *s);

t_max_err size_set(t_newverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err damp_set(t_newverb *x, t_object *attr, long argc, t_atom *argv);
t_max_err freeze_set(t_newverb *x, t_object *attr, long argc, t_atom *argv);

void newverb_dsp(t_newverb *x, t_signal **sp, short *count);
t_int *newverb_perform(t_int *w);

void newverb_dsp64(t_newverb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void newverb_perform64(t_newverb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void *newverb_class;

int main(void)
{	
	t_class *c;
	
	c = class_new("rev.newverb~", (method)newverb_new, (method)newverb_free, (long)sizeof(t_newverb ), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)newverb_dsp,		"dsp",			A_CANT, 0);
	class_addmethod(c, (method)newverb_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)newverb_assist,		"assist",		A_CANT, 0);

	CLASS_ATTR_FLOAT			(c, "size", 0, t_newverb, f_size);
	CLASS_ATTR_CATEGORY			(c, "size", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "size", 0, "Room size");
	CLASS_ATTR_ORDER			(c, "size", 0, "1");
	CLASS_ATTR_ACCESSORS		(c, "size", NULL, size_set);
	CLASS_ATTR_DEFAULT			(c, "size", 0, "0.5");
	CLASS_ATTR_FILTER_MIN		(c, "size", 0);
	CLASS_ATTR_SAVE				(c, "size", 1);

	CLASS_ATTR_FLOAT			(c, "damp", 0, t_newverb, f_damp);
	CLASS_ATTR_CATEGORY			(c, "damp", 0, "Parameters");
	CLASS_ATTR_LABEL			(c, "damp", 0, "Damping value");
	CLASS_ATTR_ORDER			(c, "damp", 0, "2");
	CLASS_ATTR_ACCESSORS		(c, "damp", NULL, damp_set);
	CLASS_ATTR_DEFAULT			(c, "damp", 0, "0.5");
	CLASS_ATTR_FILTER_MIN		(c, "damp", 0);
	CLASS_ATTR_SAVE				(c, "damp", 1);

	CLASS_ATTR_LONG				(c, "freeze", 0, t_newverb, f_freeze);
	CLASS_ATTR_CATEGORY			(c, "freeze", 0, "Behaviors");
	CLASS_ATTR_STYLE_LABEL		(c, "freeze", 0, "onoff", "Freeze");
	CLASS_ATTR_ACCESSORS		(c, "freeze", NULL, freeze_set);
	CLASS_ATTR_DEFAULT			(c, "freeze", 0, "0");
	CLASS_ATTR_SAVE				(c, "freeze", 1);

	class_dspinit(c);				
	class_register(CLASS_BOX, c);	
	newverb_class = c;
	
	post("rev.library by Pierre Guillot");
	post("Copyright (C) 2012, CICM / Universite Paris 8");
	
	return 0;
}

void *newverb_new(t_symbol *s, long argc, t_atom *argv)
{
	t_newverb *x = NULL;
	int aSide = 0;
	if (x = (t_newverb *)object_alloc((t_class*)newverb_class))
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

		x->f_newverb = new NNewveraSide);
		
		dsp_setup((t_pxobject *)x, 1);	
		outlet_new(x, "signal");
		attr_args_process(x, argc, argv);
	}
	return (x);
}

void newverb_dsp(t_newverb *x, t_signal **sp, short *count)
{
	x->f_vectorSize = sp[0]->s_n;
	dsp_add(newverb_perform, 3, x, sp[0]->s_vec, sp[1]->s_vec);
}

t_int *newverb_perform(t_int *w)
{
	t_newverb*	x	= (t_newverb *)(w[1]);
	float**		ins	= (float **)(w+2);
	float**		outs= (float **)(w+3);

	x->f_newverb->process(ins[0], outs[0],x->f_vectorSize);

	return (w + 4);
}

void newverb_dsp64(t_newverb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_vectorSize = maxvectorsize;
	object_method(dsp64, gensym("dsp_add64"), x, newverb_perform64, 0, NULL);
}

void newverb_perform64(t_newverb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->f_newverb->process(ins[0], outs[0], x->f_vectorSize);
}

void newverb_assist(t_newverb *x, void *b, long m, long a, char *s)
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

void newverb_free(t_newverb *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_newverb;
}


t_max_err size_set(t_newverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_newverb->setroomsize(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_newverb->setroomsize(atom_getfloat(argv));

	x->f_size = x->f_newverb->getroomsize();
	return 0;
}


t_max_err damp_set(t_newverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_newverb->setdamp(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_newverb->setdamp(atom_getfloat(argv));

	x->f_damp = x->f_newverb->getdamp();
	return 0;
}

t_max_err freeze_set(t_newverb *x, t_object *attr, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_LONG)
		x->f_newverb->setmode(atom_getlong(argv));
	else if(atom_gettype(argv) == A_FLOAT)
		x->f_newverb->setmode(atom_getfloat(argv));

	x->f_freeze = x->f_newverb->getmode();
	return 0;
}