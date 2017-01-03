
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

#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "z_dsp.h"
#include <math.h>
#include "../pg.window/pg.window.h"
#include <fftw3.h>

typedef struct _fft
{
	double			*f_real; 
	fftw_complex	*f_complex;
	fftw_plan		f_plan;

	double			*f_melBand;
	double			*f_mffcoeff;
	fftw_plan		f_planCos;

	long			f_ramp;

	long			f_windowSize;
	long			f_arraySize;
	long			f_instance;
	long			f_nBands;
} t_fft;

typedef struct  _mel
{
	t_pxobject	f_ob;

	double	f_sr;
	void		*out1;

	long		f_windowSize;
	long		f_overlapping;
	long		f_arraySize;
	
	long		f_nBands;
	void 		*f_clock;
	t_fft		*f_fft;

	double		*f_result;
	t_atom		*f_output;
	long		f_interval;

	t_window	f_env;
	long		f_winMode;

	double	**f_filterParameters;
	
	long		*f_melBandRef;
	long		f_mode;

	double	f_filter;
	double	f_hightpass;
	double	f_rapportSize;

	int			f_spew;
} t_mel;

void *mel_class;

void *mel_new(t_symbol *s, int argc, t_atom *argv);
void mel_free(t_mel *x);
void mel_assist(t_mel *x, void *b, long m, long a, char *s);
void mel_out(t_mel *x);

void mel_dsp64(t_mel *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void mel_perform64(t_mel *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void mel_filterParameter(t_mel *x);
void mel_envSelector(t_mel *x);
t_max_err mode_set(t_mel *x, t_object *attr, long argc, t_atom *argv);


void fft_setup(t_fft *x, int windowSize, int instance, int nbIntance, int nBands);
void fft_free(t_fft *x);

void mel_dsp(t_mel *x, t_signal **sp, short *count);
t_int *mel_perform(t_int *w);
