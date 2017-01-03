
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

#include "pg.flux.h"

void fft_setup(t_fft *x, int windowSize, int instance, int nbIntance)
{
	x->f_windowSize = windowSize;
	x->f_arraySize = x->f_windowSize / 2;
	x->f_instance = instance;

	x->f_ramp = (x->f_windowSize * x->f_instance) / nbIntance;

	x->f_real		= (double *)fftw_malloc(x->f_windowSize * sizeof(double));
	x->f_complex	= (fftw_complex *)fftw_malloc(x->f_windowSize * sizeof(fftw_complex));
	x->f_plan		= fftw_plan_dft_r2c_1d(x->f_windowSize, x->f_real, x->f_complex, FFTW_ESTIMATE);

	x->f_sumAmpCar	= 0.;
	x->f_sumAmpMul	= 0.;
	x->f_sumAmpDel	= 0.;
}

void fft_free(t_fft *x)
{
	fftw_destroy_plan(x->f_plan);
	fftw_free(x->f_real);
	fftw_free(x->f_complex);
}