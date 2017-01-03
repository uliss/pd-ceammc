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
#include "Lsd_Mfccs.hpp"

typedef struct  _mfcc 
{
	t_pxobject	f_ob;
	Mfccs*		f_mfcc;
	
} t_mfcc;

t_class *Mfcc_class;

void *Mfcc_new(t_symbol *s, long argc, t_atom *argv);
void Mfcc_free(t_mfcc *x);
void Mfcc_assist(t_mfcc *x, void *b, long m, long a, char *s);

void Mfcc_dsp64(t_mfcc *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void Mfcc_perform64(t_mfcc *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void Mfcc_dsp(t_mfcc *x, t_signal **sp, short *count);
t_int *Mfcc_perform(t_int *w);

int main()
{
	t_class *c = class_new("lsd.mfcc~", (method)Mfcc_new, (method)dsp_free, (long)sizeof(t_mfcc), 0L, A_GIMME, 0);
	class_addmethod(c, (method)Mfcc_dsp64,		"dsp64",		A_CANT,	0);
	class_addmethod(c, (method)Mfcc_dsp,			"dsp",			A_CANT,	0);
	class_addmethod(c, (method)Mfcc_assist,		"assist",		A_CANT,	0);
	
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	Mfcc_class = c;

	post("lsd.library by Pierre Guillot",0);
	post("Copyright (C) 2012, Universite Paris 8");
	
	return 0;
}

void *Mfcc_new(t_symbol *s, long argc, t_atom *argv)
{
	t_mfcc *x = NULL;
	int			windowSize		= 1024;
	int			numberOfWindow	= 2;
	std::string function		= "hanning";
	std::string mode			= "hanning";
	
	if(x = (t_mfcc *)object_alloc(Mfcc_class))
	{
		/* Sampling Rate initialization **************/
		
		if(atom_gettype(argv) == A_LONG)
			windowSize	= atom_getlong(argv);
		if(atom_gettype(argv+1) == A_LONG)
			numberOfWindow = atom_getlong(argv+1);
		if(atom_gettype(argv+2) == A_SYM)
			function = atom_getsym(argv+2)->s_name;
		if(atom_gettype(argv+3) == A_SYM)
			mode = atom_getsym(argv+3)->s_name;
		
		x->f_mfcc = new Mfccs(windowSize, numberOfWindow); 
		dsp_setup((t_pxobject *)x, 1);
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");
		outlet_new((t_pxobject *)x, "signal");

		attr_args_process(x, argc, argv);
	}
	return (x);
}			

void Mfcc_dsp64(t_mfcc *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->f_mfcc->setVectorSize(maxvectorsize);
	object_method(dsp64, gensym("dsp_add64"), x, Mfcc_perform64, 0, NULL);
}

void Mfcc_perform64(t_mfcc *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	;
}


void Mfcc_dsp(t_mfcc *x, t_signal **sp, short *count)
{
	x->f_mfcc->setSamplingRate(sp[0]->s_sr);
	x->f_mfcc->setVectorSize(sp[0]->s_n);
	dsp_add(Mfcc_perform, 6, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec);
}


t_int *Mfcc_perform(t_int *w)
{	
	t_mfcc	*x		= (t_mfcc *)(w[1]);
	float	*in		= (float *)	(w[2]);
	float	*min	= (float *)	(w[3]);
	float	*ave	= (float *)	(w[4]);
	float	*max	= (float *)	(w[5]);
	float	*sum	= (float *)	(w[6]);

	x->f_mfcc->process(in, min, ave, max, sum);
	return (w + 7);	
}
	
void Mfcc_assist(t_mfcc *x, void *b, long m, long a, char *s)
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
	
void Mfcc_free(t_mfcc *x)
{
	dsp_free((t_pxobject *)x);
	delete x->f_mfcc;
}
