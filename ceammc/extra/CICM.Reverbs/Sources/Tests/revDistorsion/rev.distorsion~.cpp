
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

#include "Distorsion.h"

extern "C" 
{
	#include "ext.h"
	#include "ext_obex.h"
	#include "z_dsp.h"
	#include "buffer.h"
	#include "ext_globalsymbol.h"
}


typedef struct _distorsion
{
	t_pxobject		f_ob;	
	Distorsion	*f_distorsion;
}
t_distorsion ;

void *distorsion_new(t_symbol *s, long argc, t_atom *argv);
void distorsion_free(t_distorsion *x);
void distorsion_assist(t_distorsion *x, void *b, long m, long a, char *s);

void distorsion_float(t_distorsion *x, double f);
void distorsion_int(t_distorsion *x, long n);

void distorsion_dsp(t_distorsion *x, t_signal **sp, short *count);
t_int *distorsion_perform(t_int *w);

void distorsion_dsp64(t_distorsion *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void distorsion_perform64(t_distorsion *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void *distorsion_class;

int main(void)
{	
	t_class *c;
	
	c = class_new("rev.distorsion~", (method)distorsion_new, (method)distorsion_free, (long)sizeof(t_distorsion ), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)distorsion_dsp,			"dsp",			A_CANT, 0);
	class_addmethod(c, (method)distorsion_dsp64,		"dsp64",		A_CANT, 0);
	class_addmethod(c, (method)distorsion_assist,		"assist",		A_CANT, 0);

	class_dspinit(c);				
	class_register(CLASS_BOX, c);	
	distorsion_class = c;
	
	post("rev.library by Pierre Guillot");
	post("Copyright (C) 2012, CICM / Universite Paris 8");
	
	return 0;
}

void *distorsion_new(t_symbol *s, long argc, t_atom *argv)
{
	t_distorsion *x = NULL;
	if (x = (t_distorsion *)object_alloc((t_class*)distorsion_class)) 
	{	
		x->f_distorsion = new Distorsion();
		
		dsp_setup((t_pxobject *)x, 1);	
		outlet_new(x, "signal");
		attr_args_process(x, argc, argv);
	}
	return (x);
}

void distorsion_dsp(t_distorsion *x, t_signal **sp, short *count)
{
	x->f_distorsion->setVectorSize(sp[0]->s_n);
	long numberOfinlets = 0;
	for(int i = 0; i < 3; i++)
	{
		if (count[i])
			numberOfinlets++;
	}
	
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
	
	dsp_addv(distorsion_perform, pointer_count, (void **)sigvec);
	free(sigvec);
}

t_int *distorsion_perform(t_int *w)
{
	t_distorsion*	x			= (t_distorsion *)(w[1]);
	float**		ins				= (float **)(w+2);
	float**		outs			= (float **)(w+3);
	
	x->f_distorsion->process(ins[0], outs[0]);
	return (w + 4);
}

void distorsion_dsp64(t_distorsion *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_distorsion->setVectorSize(maxvectorsize);
	object_method(dsp64, gensym("dsp_add64"), x, distorsion_perform64, 0, NULL);
}

void distorsion_perform64(t_distorsion *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->f_distorsion->process(ins[0], outs[0]);
}

void distorsion_assist(t_distorsion *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) 
		sprintf(s,"(Signal) Clean signal");		
	else
		sprintf(s,"(Signal) Distorded signal");
}

void distorsion_free(t_distorsion *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_distorsion;
}
