
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

#include "Flanger.h"

extern "C" 
{
	#include "ext.h"
	#include "ext_obex.h"
	#include "z_dsp.h"
	#include "buffer.h"
	#include "ext_globalsymbol.h"
}


typedef struct _flanger
{
	t_pxobject		f_ob;	
	Flanger		*f_flanger;
}
t_flanger ;

void *flanger_new(t_symbol *s, long argc, t_atom *argv);
void flanger_free(t_flanger *x);
void flanger_assist(t_flanger *x, void *b, long m, long a, char *s);

void flanger_float(t_flanger *x, double f);
void flanger_int(t_flanger *x, long n);

void flanger_dsp(t_flanger *x, t_signal **sp, short *count);
t_int *flanger_perform(t_int *w);

void flanger_dsp64(t_flanger *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void flanger_perform64(t_flanger *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void *flanger_class;

int main(void)
{	
	t_class *c;
	
	c = class_new("rev.flanger~", (method)flanger_new, (method)flanger_free, (long)sizeof(t_flanger ), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)flanger_dsp,			"dsp",			A_CANT, 0);
	class_addmethod(c, (method)flanger_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)flanger_assist,		"assist",		A_CANT, 0);
	class_addmethod(c, (method)flanger_float,		"float",		A_FLOAT,0);
	class_addmethod(c, (method)flanger_int,			"int",			A_LONG, 0);
	
	class_dspinit(c);				
	class_register(CLASS_BOX, c);	
	flanger_class = c;
	
	post("rev.library by Pierre Guillot");
	post("Copyright (C) 2012, CICM / Universite Paris 8");
	
	return 0;
}

void *flanger_new(t_symbol *s, long argc, t_atom *argv)
{
	t_flanger *x = NULL;
    
    x = (t_flanger *)object_alloc((t_class*)flanger_class);
	if (x)
	{	
		x->f_flanger = new Flanger();
		
		dsp_setup((t_pxobject *)x, 1);	
		outlet_new(x, "signal");
		attr_args_process(x, argc, argv);
	}
	return (x);
}

void flanger_float(t_flanger *x, double f)
{
	;
}

void flanger_int(t_flanger *x, long n)
{
	;
}

void flanger_dsp(t_flanger *x, t_signal **sp, short *count)
{
	x->f_flanger->setVectorSize(sp[0]->s_n);
	x->f_flanger->setSamplingRate(sp[0]->s_sr);
	int i;
	int pointer_count;
	t_int **sigvec;
	
	pointer_count = 3;
		
	sigvec  = (t_int **)calloc(pointer_count, sizeof(t_int *));
	for(i = 0; i < pointer_count; i++)
		sigvec[i] = (t_int *)calloc(1, sizeof(t_int));
		
	sigvec[0] = (t_int *)x;
	for(i = 1; i < pointer_count; i++)
		sigvec[i] = (t_int *)sp[i - 1]->s_vec;
	
	dsp_addv(flanger_perform, pointer_count, (void **)sigvec);
	free(sigvec);
}

t_int *flanger_perform(t_int *w)
{
	t_flanger*	x			= (t_flanger *)(w[1]);
	float**		ins				= (float **)(w+2);
	float**		outs			= (float **)(w+3);

	x->f_flanger->process(ins[0], outs[0]);

	return (w + 4);
}

void flanger_dsp64(t_flanger *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_flanger->setVectorSize(maxvectorsize);
	x->f_flanger->setSamplingRate(samplerate);

	object_method(dsp64, gensym("dsp_add64"), x, flanger_perform64, 0, NULL);
}

void flanger_perform64(t_flanger *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->f_flanger->process(ins[0], outs[0]);
}

void flanger_assist(t_flanger *x, void *b, long m, long a, char *s)
{
	if (a == 0) 
		sprintf(s,"(Signal) Left channel");
	else if(a == 1)
		sprintf(s,"(Signal) Right channel"); 			
	else
		sprintf(s,"(Signal, int or float) Width value");
}

void flanger_free(t_flanger *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_flanger;
}
