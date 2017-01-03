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

#include "pg.mel.h"

int main()
{
	t_class *c;

	c = class_new("pg.mel~", (method)mel_new, (method)mel_free, (short)sizeof(t_mel), 0L, A_GIMME, 0);
	class_addmethod(c, (method)mel_dsp64,		"dsp64",		A_CANT,	0);
	class_addmethod(c, (method)mel_dsp,			"dsp",			A_CANT,	0);
	class_addmethod(c, (method)mel_assist,		"assist",		A_CANT,	0);
	
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	mel_class = c;
	
	CLASS_ATTR_LONG				(c, "mode", 0, t_mel, f_mode);
	CLASS_ATTR_LABEL			(c, "mode", 0, "Mode");
	CLASS_ATTR_ENUMINDEX		(c, "mode", 0, "Mel \"Mfcc");
	CLASS_ATTR_DEFAULT			(c, "mode", 0, "0");
	CLASS_ATTR_SAVE				(c, "mode", 1);
	CLASS_ATTR_ORDER			(c, "mode", 0, "1");

	CLASS_ATTR_LONG				(c, "window", 0, t_mel, f_winMode);
	CLASS_ATTR_LABEL			(c, "window", 0, "Window type");
	CLASS_ATTR_ENUMINDEX		(c, "window", 0, "Square \" \"Hanning \" \"Hamming \" \"Tukey \" \"Cosinus \" \"Lanczos \" \"Triangular \" \"Gaussian \" \"Bartlett-Hann \" \"Blackman \" \"Kaiser \" \"Nuttall \" \"Blackman-Harris \" \"Blackman-Nuttall \" \"Flat-Top \" \"Poisson");
	CLASS_ATTR_ACCESSORS		(c, "window", NULL, mode_set);
	CLASS_ATTR_DEFAULT			(c, "window", 0, "0");
	CLASS_ATTR_SAVE				(c, "window", 1);
	CLASS_ATTR_FILTER_CLIP		(c, "window", 0, 10);
	CLASS_ATTR_ORDER			(c, "window", 0, "2");

	CLASS_ATTR_LONG				(c, "interval", 0, t_mel, f_interval);
	CLASS_ATTR_LABEL			(c, "interval", 0, "Polling interval");
	CLASS_ATTR_FILTER_CLIP		(c, "interval", 3, 5000);
	CLASS_ATTR_DEFAULT			(c, "interval", 0, "10");
	CLASS_ATTR_SAVE				(c, "interval", 1);
	CLASS_ATTR_ORDER			(c, "interval", 0, "3");
	
	CLASS_ATTR_FLOAT			(c, "highpass", 0, t_mel, f_hightpass);
	CLASS_ATTR_LABEL			(c, "highpass", 0, "Highpass filter");
	CLASS_ATTR_FILTER_CLIP		(c, "highpass", 0., 1.);
	CLASS_ATTR_DEFAULT			(c, "highpass", 0, "0.95");
	CLASS_ATTR_SAVE				(c, "highpass", 1);
	CLASS_ATTR_ORDER			(c, "highpass", 0, "5");

	post("pg.mel~ by Pierre Guillot",0);
	post("Copyright (C) 2012, Universite Paris 8");
	
	return 0;
}

void *mel_new(t_symbol *s, int argc, t_atom *argv)
{
	t_mel *x = NULL;
	int i;
	
	if (x = (t_mel *)object_alloc((t_class *)mel_class)) 
	{		
		/* Sampling Rate initialization **************/
		x->f_sr = sys_getsr();

		/* Windows Size Initialization **************/
		x->f_windowSize = 0; 
		if( argv[0].a_type == A_LONG ) x->f_windowSize = (int)(argv[0].a_w.w_long);
		if (x->f_windowSize%2 != 0)
		{
			x->f_windowSize = 1024;
			object_post((t_object*)x, "Wrong window size initialization, set to default : 1024");
		}
		x->f_arraySize = x->f_windowSize/2;

		/* Overlapping Initialization ***************/
		x->f_overlapping = 0; 
		if( argv[1].a_type == A_LONG ) x->f_overlapping = (int)(argv[1].a_w.w_long);
		if (x->f_overlapping < 1)
		{
			x->f_overlapping = 2;
			object_post((t_object*)x, "Wrong overlapping initialization, set to default : 2");
		}
		
		/* Initialization of the window  ********************************/
		if( argv[2].a_type == A_LONG ) x->f_winMode = argv[2].a_w.w_long;
		if (x->f_winMode < 0 || x->f_winMode > 15)
		{
			x->f_winMode = 1;
			object_post((t_object*)x, "Wrong window initialization, set to default : 1");
		}
		window_setup(&x->f_env, x->f_windowSize, x->f_winMode);

		/* Mel bands Initialization ***************/
		if( argv[3].a_type == A_LONG ) x->f_nBands = (int)(argv[3].a_w.w_long);
		if (x->f_nBands < 1)
		{
			x->f_nBands = 24;
			object_post((t_object*)x, "Wrong bands initialization, set to default : 24");
		}

		/* Mel Filter Initialization **************/
		x->f_melBandRef			= (long *)getbytes(x->f_arraySize * sizeof(long));
		x->f_filterParameters	= (double **)getbytes(x->f_nBands  * sizeof(double *));
		for(i = 0; i < x->f_nBands ; i++)
			x->f_filterParameters[i] = (double *)getbytes(x->f_arraySize  * sizeof(double));
		mel_filterParameter(x);
				
		x->f_interval = 5;		
		x->f_mode = 0;
		x->f_filter = 0.;
		x->f_hightpass = 0.95;
		x->f_result				= (double *)getbytes(x->f_nBands * sizeof(double));
		x->f_output				= (t_atom *)getbytes(x->f_nBands * sizeof(t_atom));

		/* Inlet and Outlet Initialization ******************************/
		dsp_setup((t_pxobject *)x, 1);
		x->out1 = listout(x);
		x->f_spew = 0;
		x->f_clock = clock_new(x, (method)mel_out);
		clock_delay(x->f_clock, 0L);

		attr_args_process (x, argc, argv);
	}
	return x;
}			

void mel_dsp64(t_mel *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	int i;
	x->f_sr = samplerate;
	x->f_rapportSize	= (double)(PI * (1. / (double)x->f_arraySize));

	/* FFt initialization ***********************/
	x->f_fft = (t_fft *)getbytes(x->f_overlapping  * sizeof(t_fft));
	for(i = 0; i < x->f_overlapping; i++)
	{
		fft_setup(&x->f_fft[i], x->f_windowSize, i, x->f_overlapping, x->f_nBands);
	}
	/* Clock Initialization ****************************************/
	
	object_method(dsp64, gensym("dsp_add64"), x, mel_perform64, 0, NULL);
}

void mel_perform64(t_mel *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{	
	long i, j, alpha;
	double	*in		= ins[0];
	double energy;
	
	for(i = 0; i < x->f_overlapping; i++)
	{
		for(j = 0; j < sampleframes; j++)
		{
			/* Lowpass filter for Mfccs */
			if (x->f_mode != 0)
			{
				x->f_fft[i].f_real[x->f_fft[i].f_ramp] = (in[j] - (x->f_hightpass * x->f_filter)) * x->f_env.f_envelope[x->f_fft[i].f_ramp];
				x->f_filter = in[j];
			}
			else
			{
				x->f_fft[i].f_real[x->f_fft[i].f_ramp] = in[j] * x->f_env.f_envelope[x->f_fft[i].f_ramp];
			}
			/* MFCC's perform */
			if (x->f_mode != 0)
			{
				/* First : Scale above the mel scale */
				if (x->f_fft[i].f_ramp >= 0 && x->f_fft[i].f_ramp <  x->f_arraySize )
				{
					alpha = x->f_melBandRef[x->f_fft[i].f_ramp];
					energy = (sqrt((x->f_fft[i].f_complex[x->f_fft[i].f_ramp][0]*x->f_fft[i].f_complex[x->f_fft[i].f_ramp][0])+(x->f_fft[i].f_complex[x->f_fft[i].f_ramp][1]*x->f_fft[i].f_complex[x->f_fft[i].f_ramp][1])));
					// Si la bande frequentielle appartient au flitre de la bande mel alpha, il appartient aussi au filtre de la bande alpha+1
					// sauf lorque j-1 car la bande j-1 existe pas, donc elle appartient seulement a la bande j+1.
					if(alpha != -1)
					{
						x->f_fft[i].f_melBand[alpha] += energy * x->f_filterParameters[alpha][x->f_fft[i].f_ramp];
					}
					// Sauf pour la dernire bande car la position +1 de la derniere n'existe pas, elle appartient seulement a la bande alpha.
					if(alpha != x->f_nBands-1)
					{
						x->f_fft[i].f_melBand[alpha+1] += energy * x->f_filterParameters[alpha+1][x->f_fft[i].f_ramp];
					}
					
				}
				/* Second : Take the logarithm */
				else if(x->f_fft[i].f_ramp >= x->f_arraySize && x->f_fft[i].f_ramp < x->f_arraySize+x->f_nBands)
				{
					alpha = x->f_fft[i].f_ramp - x->f_arraySize ;
					if(x->f_fft[i].f_melBand[alpha] != 0.)
					{
						x->f_fft[i].f_melBand[alpha] = log10(x->f_fft[i].f_melBand[alpha]);
					}
					else
					{
						x->f_fft[i].f_melBand[alpha] = -90.f;
					}
				}
				/* Third : Perfom the cosinus transform */
				else if(x->f_fft[i].f_ramp == x->f_arraySize + x->f_nBands)
				{
					fftw_execute(x->f_fft[i].f_planCos );
				}
				/* Fourth : Record the result */
				else if(x->f_fft[i].f_ramp >= x->f_arraySize + 1 + x->f_nBands && x->f_fft[i].f_ramp < x->f_arraySize + (2 * x->f_nBands) + 1)
				{
					alpha = x->f_fft[i].f_ramp - (x->f_arraySize + x->f_nBands + 1);
						
					x->f_result[alpha] = x->f_fft[i].f_mffcoeff[alpha];
					x->f_fft[i].f_melBand[alpha] = 0.f;
					if(alpha == x->f_nBands - 1)
					{
						x->f_spew = 1;
						x->f_fft[i].f_melBand[alpha] = 0.f;
					}
				}
			}
			/* Mel perform */
			else
			{
				if (x->f_fft[i].f_ramp >= 0 && x->f_fft[i].f_ramp <  x->f_arraySize )
				{

					alpha = x->f_melBandRef[x->f_fft[i].f_ramp];
					energy =  (sqrt((x->f_fft[i].f_complex[x->f_fft[i].f_ramp][0]*x->f_fft[i].f_complex[x->f_fft[i].f_ramp][0])+(x->f_fft[i].f_complex[x->f_fft[i].f_ramp][1]*x->f_fft[i].f_complex[x->f_fft[i].f_ramp][1])));
					
					if(alpha != -1)
					{
						x->f_fft[i].f_melBand[alpha] += energy * x->f_filterParameters[alpha][x->f_fft[i].f_ramp];
					}
					if(alpha != x->f_nBands-1)
					{
						x->f_fft[i].f_melBand[alpha+1] += energy * x->f_filterParameters[alpha+1][x->f_fft[i].f_ramp];
					}
					
				}
				else if(x->f_fft[i].f_ramp >= x->f_arraySize && x->f_fft[i].f_ramp < x->f_arraySize + x->f_nBands)
				{
					alpha = x->f_fft[i].f_ramp - x->f_arraySize;
	
					x->f_result[alpha] = x->f_fft[i].f_melBand[alpha];
					x->f_fft[i].f_melBand[alpha] = 0.f;
				}
				else if(x->f_fft[i].f_ramp == x->f_arraySize + x->f_nBands)
				{
					x->f_spew = 1;
				}
			}
			
			x->f_fft[i].f_ramp++;
			if (x->f_fft[i].f_ramp >= x->f_windowSize)
			{
				fftw_execute(x->f_fft[i].f_plan );
				x->f_fft[i].f_ramp = 0;
			}	
		}
	}
}

void mel_out(t_mel *x)
{
	int i, j;
	
	for(i = 0; i < x->f_overlapping; i++)
	{
		for(j = 0; j < x->f_nBands; j++)
		{
			SETFLOAT(x->f_output+j, x->f_result[j]);
		}
		if(x->f_mode != 0)
		{
			outlet_list(x->out1, 0L, (x->f_nBands-1), x->f_output+1);
		}
		else 
		{
			outlet_list(x->out1, 0L, x->f_nBands, x->f_output);
		}
	}
	clock_delay(x->f_clock, x->f_interval);
}

void mel_free(t_mel *x)
{
	int i;
	dsp_free((t_pxobject *)x);
	
	freebytes(x->f_result, x->f_nBands * sizeof(double));
	freebytes(x->f_output, x->f_nBands * sizeof(t_atom));
	freebytes(x->f_melBandRef , x->f_arraySize * sizeof(long));
	for(i = 0; i < x->f_nBands; i++)
		freebytes(x->f_filterParameters[i], x->f_arraySize * sizeof(double));

	freebytes(x->f_filterParameters, x->f_nBands * sizeof(double));
	window_free(&x->f_env);
	object_free(x->f_clock);

}

void mel_assist(t_mel *x, void *b, long m, long a, char *s)
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
				if (x->f_mode != 0)
				{
					sprintf(s,"(List) MFCCs");
				}
				else 
				{
					sprintf(s,"(List) Mel Bands");
				}

				break;
		}
	}
}


void mel_filterParameter(t_mel *x)
{
	int i, j, frequency;

	double maxMel;
	double melBandSize;
	double freqBandSize;
	double *melBandCenterMel;
	double *melBandCenterHerz;
	double twoMelBandSize;
	double *freqBandCenter;
	double *normalizeFactor;
	
	
	maxMel = 2595. * log10(((((double)(x->f_sr) / 2.) - 20.) / 700.) + 1.);
	melBandSize = maxMel / (double)x->f_nBands;

	melBandCenterMel = (double *)getbytes(x->f_nBands  * sizeof(double));
	for(i = 0; i < x->f_nBands; i++)
		melBandCenterMel[i] = (double)i * melBandSize  +  melBandSize / 2. ;

	melBandCenterHerz= (double *)getbytes((x->f_nBands+1)  * sizeof(double));
	for(i = 0; i < x->f_nBands; i++)
		melBandCenterHerz[i]	= (700. * (pow(10., (melBandCenterMel[i] / 2595.)) - 1.)) + 20.;

	melBandCenterHerz[x->f_nBands] = x->f_sr;
	freqBandSize = x->f_sr / (double)x->f_windowSize;

	freqBandCenter = (double *)getbytes(x->f_arraySize  * sizeof(double));
	for(i = 0; i < x->f_arraySize; i++)
		freqBandCenter[i] = ((double)i * freqBandSize) + (freqBandSize / 2.);
	
	for(i = 0; i < x->f_arraySize; i++)
	{
		frequency = (double)i * freqBandSize;
		for(j = 0; j < x->f_nBands; j++)
		{
			if( frequency < melBandCenterHerz[0])
			{
				x->f_melBandRef[i] = -1;
			}
			else if( frequency > melBandCenterHerz[j] && frequency < melBandCenterHerz[j+1])
			{
				x->f_melBandRef[i] = j;
			}
		}
	}
	
	normalizeFactor = (double *)getbytes(x->f_nBands  * sizeof(double));
	for(i = 0; i < x->f_nBands; i++)
	{
		normalizeFactor[i] = 0;
		for(j = 0; j < x->f_arraySize; j++)
		{
			if( x->f_melBandRef[j] == i || x->f_melBandRef[j] == i-1 )
			{
				normalizeFactor[i]++;
			}
		}
		if (normalizeFactor[i] <= 1.) 
		{
			normalizeFactor[i] = 1.;
		}
	}
	
	for(i = 0; i < x->f_nBands; i++)
	{
		for(j = 0; j < x->f_arraySize; j++)
		{
			if(i == 0) 
				twoMelBandSize = melBandCenterHerz[1];
			else if(i == x->f_nBands-1) 
				twoMelBandSize = x->f_sr/2 - melBandCenterHerz[i-1];
			else 
				twoMelBandSize = melBandCenterHerz[i+1] - melBandCenterHerz[i-1];

			x->f_filterParameters[i][j] = ((2. / twoMelBandSize)  * ((twoMelBandSize / 2.) - fabs((freqBandCenter[j] - melBandCenterHerz[i-1])  - ((twoMelBandSize - 1.) / 2.))) / normalizeFactor[i]) ;
			if (x->f_filterParameters[i][j] <= 0.f) 
				x->f_filterParameters[i][j] = 0.f;
			}
	}
	
	freebytes(melBandCenterMel, x->f_nBands * sizeof(double));
	freebytes(melBandCenterHerz, (x->f_nBands + 1) * sizeof(double));
	freebytes(freqBandCenter, x->f_arraySize * sizeof(double));
	freebytes(freqBandCenter, x->f_nBands * sizeof(double));
}

t_max_err mode_set(t_mel *x, t_object *attr, long argc, t_atom *argv)
{
	window_mode_set(&x->f_env, attr, argc, argv);
	return 0;
}

void mel_dsp(t_mel *x, t_signal **sp, short *count)
{
	int i;
	x->f_sr = (double)sp[0]->s_sr;
	x->f_rapportSize	= (double)(PI * (1. / (double)x->f_arraySize));
	
	/* FFt initialization ***********************/
	x->f_fft = (t_fft *)getbytes(x->f_overlapping  * sizeof(t_fft));
	for(i = 0; i < x->f_overlapping; i++)
	{
		fft_setup(&x->f_fft[i], x->f_windowSize, i, x->f_overlapping, x->f_nBands);
	}
	/* Clock Initialization ****************************************/
	
	dsp_add(mel_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *mel_perform(t_int *w)
{
    t_mel	*x		= (t_mel *)(w[1]);
	float	*in		= (float *)	(w[2]);
	int n			= (int)		(w[3]);
	long i, j, alpha;
	double energy;
	
	if (x->f_ob.z_disabled) return w + 4;
	
	for(i = 0; i < x->f_overlapping; i++)
	{
		for(j = 0; j < n; j++)
		{
			/* Lowpass filter for Mfccs */
			if (x->f_mode != 0)
			{
				x->f_fft[i].f_real[x->f_fft[i].f_ramp] = (in[j] - (x->f_hightpass * x->f_filter)) * x->f_env.f_envelope[x->f_fft[i].f_ramp];
				x->f_filter = in[j];
			}
			else
			{
				x->f_fft[i].f_real[x->f_fft[i].f_ramp] = in[j] * x->f_env.f_envelope[x->f_fft[i].f_ramp];
			}
			/* MFCC's perform */
			if (x->f_mode != 0)
			{
				/* First : Scale above the mel scale */
				if (x->f_fft[i].f_ramp >= 0 && x->f_fft[i].f_ramp <  x->f_arraySize )
				{
					alpha = x->f_melBandRef[x->f_fft[i].f_ramp];
					energy = (sqrt((x->f_fft[i].f_complex[x->f_fft[i].f_ramp][0]*x->f_fft[i].f_complex[x->f_fft[i].f_ramp][0])+(x->f_fft[i].f_complex[x->f_fft[i].f_ramp][1]*x->f_fft[i].f_complex[x->f_fft[i].f_ramp][1])));
					// Si la bande frequentielle appartient au flitre de la bande mel alpha, il appartient aussi au filtre de la bande alpha+1
					// sauf lorque j-1 car la bande j-1 existe pas, donc elle appartient seulement a la bande j+1.
					if(alpha != -1)
					{
						x->f_fft[i].f_melBand[alpha] += energy * x->f_filterParameters[alpha][x->f_fft[i].f_ramp];
					}
					// Sauf pour la dernire bande car la position +1 de la derniere n'existe pas, elle appartient seulement a la bande alpha.
					if(alpha != x->f_nBands-1)
					{
						x->f_fft[i].f_melBand[alpha+1] += energy * x->f_filterParameters[alpha+1][x->f_fft[i].f_ramp];
					}
					
				}
				/* Second : Take the logarithm */
				else if(x->f_fft[i].f_ramp >= x->f_arraySize && x->f_fft[i].f_ramp < x->f_arraySize+x->f_nBands)
				{
					alpha = x->f_fft[i].f_ramp - x->f_arraySize ;
					if(x->f_fft[i].f_melBand[alpha] != 0.)
					{
						x->f_fft[i].f_melBand[alpha] = log10(x->f_fft[i].f_melBand[alpha]);
					}
					else
					{
						x->f_fft[i].f_melBand[alpha] = -90.f;
					}
				}
				/* Third : Perfom the cosinus transform */
				else if(x->f_fft[i].f_ramp == x->f_arraySize + x->f_nBands)
				{
					fftw_execute(x->f_fft[i].f_planCos );
				}
				/* Fourth : Record the result */
				else if(x->f_fft[i].f_ramp >= x->f_arraySize + 1 + x->f_nBands && x->f_fft[i].f_ramp < x->f_arraySize + (2 * x->f_nBands) + 1)
				{
					alpha = x->f_fft[i].f_ramp - (x->f_arraySize + x->f_nBands + 1);
					
					x->f_result[alpha] = x->f_fft[i].f_mffcoeff[alpha];
					x->f_fft[i].f_melBand[alpha] = 0.f;
					if(alpha == x->f_nBands - 1)
					{
						x->f_spew = 1;
						x->f_fft[i].f_melBand[alpha] = 0.f;
					}
				}
			}
			/* Mel perform */
			else
			{
				if (x->f_fft[i].f_ramp >= 0 && x->f_fft[i].f_ramp <  x->f_arraySize )
				{
					
					alpha = x->f_melBandRef[x->f_fft[i].f_ramp];
					energy =  (sqrt((x->f_fft[i].f_complex[x->f_fft[i].f_ramp][0]*x->f_fft[i].f_complex[x->f_fft[i].f_ramp][0])+(x->f_fft[i].f_complex[x->f_fft[i].f_ramp][1]*x->f_fft[i].f_complex[x->f_fft[i].f_ramp][1])));
					
					if(alpha != -1)
					{
						x->f_fft[i].f_melBand[alpha] += energy * x->f_filterParameters[alpha][x->f_fft[i].f_ramp];
					}
					if(alpha != x->f_nBands-1)
					{
						x->f_fft[i].f_melBand[alpha+1] += energy * x->f_filterParameters[alpha+1][x->f_fft[i].f_ramp];
					}
					
				}
				else if(x->f_fft[i].f_ramp >= x->f_arraySize && x->f_fft[i].f_ramp < x->f_arraySize + x->f_nBands)
				{
					alpha = x->f_fft[i].f_ramp - x->f_arraySize;
					
					x->f_result[alpha] = x->f_fft[i].f_melBand[alpha];
					x->f_fft[i].f_melBand[alpha] = 0.f;
				}
				else if(x->f_fft[i].f_ramp == x->f_arraySize + x->f_nBands)
				{
					x->f_spew = 1;
				}
			}
			
			x->f_fft[i].f_ramp++;
			if (x->f_fft[i].f_ramp >= x->f_windowSize)
			{
				fftw_execute(x->f_fft[i].f_plan );
				x->f_fft[i].f_ramp = 0;
			}	
		}
	}
	return w + 4;
}