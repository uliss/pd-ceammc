
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

#include "StereoRotate.h"

extern "C" 
{
	#include "ext.h"
	#include "ext_obex.h"
	#include "z_dsp.h"
	#include "buffer.h"
	#include "ext_globalsymbol.h"
}


typedef struct _stereoRotate
{
	t_pxobject		f_ob;	
	StereoRotate		*f_stereoRotate;
}
t_stereoRotate ;

void *stereoRotate_new(t_symbol *s, long argc, t_atom *argv);
void stereoRotate_free(t_stereoRotate *x);
void stereoRotate_assist(t_stereoRotate *x, void *b, long m, long a, char *s);

void stereoRotate_float(t_stereoRotate *x, double f);
void stereoRotate_int(t_stereoRotate *x, long n);

void stereoRotate_dsp(t_stereoRotate *x, t_signal **sp, short *count);
t_int *stereoRotate_perform(t_int *w);

void stereoRotate_dsp64(t_stereoRotate *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void stereoRotate_perform64(t_stereoRotate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void *stereoRotate_class;

int main(void)
{	
	t_class *c;
	
	c = class_new("rev.rotate~", (method)stereoRotate_new, (method)stereoRotate_free, (long)sizeof(t_stereoRotate ), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)stereoRotate_dsp,		"dsp",			A_CANT, 0);
	class_addmethod(c, (method)stereoRotate_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)stereoRotate_assist,		"assist",		A_CANT, 0);
	class_addmethod(c, (method)stereoRotate_float,		"float",		A_FLOAT,0);
	class_addmethod(c, (method)stereoRotate_int,		"int",			A_LONG, 0);

	class_dspinit(c);				
	class_register(CLASS_BOX, c);	
	stereoRotate_class = c;
	
	class_findbyname(CLASS_BOX, gensym("rev.freeverb~"));
	
	return 0;
}

void *stereoRotate_new(t_symbol *s, long argc, t_atom *argv)
{
	t_stereoRotate *x = NULL;
    x = (t_stereoRotate *)object_alloc((t_class*)stereoRotate_class);
	if (x) 
	{	
		x->f_stereoRotate = new StereoRotate();
		
		dsp_setup((t_pxobject *)x, 3);	
		outlet_new(x, "signal");
		outlet_new(x, "signal");
		attr_args_process(x, argc, argv);
	}
	return (x);
}

void stereoRotate_float(t_stereoRotate *x, double f)
{
	x->f_stereoRotate->setPhase(f);
}

void stereoRotate_int(t_stereoRotate *x, long n)
{
	x->f_stereoRotate->setPhase(n);
}

void stereoRotate_dsp(t_stereoRotate *x, t_signal **sp, short *count)
{
	x->f_stereoRotate->setVectorSize(sp[0]->s_n);
	long numberOfinlets = 0;
	for(int i = 0; i < 3; i++)
	{
		if (count[i])
			numberOfinlets++;
	}
	
	int i;
	int pointer_count;
	t_int **sigvec;
	
	pointer_count = 7;
		
	sigvec  = (t_int **)calloc(pointer_count, sizeof(t_int *));
	for(i = 0; i < pointer_count; i++)
		sigvec[i] = (t_int *)calloc(1, sizeof(t_int));
		
	sigvec[0] = (t_int *)x;
	sigvec[1] = (t_int *)numberOfinlets;
	for(i = 2; i < pointer_count; i++)
		sigvec[i] = (t_int *)sp[i - 2]->s_vec;
	
	dsp_addv(stereoRotate_perform, pointer_count, (void **)sigvec);
	free(sigvec);
}

t_int *stereoRotate_perform(t_int *w)
{
	t_stereoRotate*	x			= (t_stereoRotate *)(w[1]);
	long		numins			= (long)(w[2]);
	float**		ins				= (float **)(w+3);
	float**		outs			= (float **)(w+6);
	
	
	if(numins == 3)
		x->f_stereoRotate->process(ins, outs, ins[2]);
	else
		x->f_stereoRotate->process(ins, outs);

	return (w + 8);
}

void stereoRotate_dsp64(t_stereoRotate *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_stereoRotate->setVectorSize(maxvectorsize);
	object_method(dsp64, gensym("dsp_add64"), x, stereoRotate_perform64, 0, NULL);
}

void stereoRotate_perform64(t_stereoRotate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{

	if(numins == 3)
		x->f_stereoRotate->process(ins, outs, ins[2]);
	else
		x->f_stereoRotate->process(ins, outs);

}

void stereoRotate_assist(t_stereoRotate *x, void *b, long m, long a, char *s)
{
	if (a == 0) 
		sprintf(s,"(Signal) Left channel");
	else if(a == 1)
		sprintf(s,"(Signal) Right channel"); 			
	else
		sprintf(s,"(Signal, int or float) Width value");
}

void stereoRotate_free(t_stereoRotate *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_stereoRotate;
}
