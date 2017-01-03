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

#define CICM_VDSP
//#define CICM_FFTW_GSL
//#define CICM_IPPS
#define CICM_FLOAT
//#define CICM_DOUBLE

#ifndef DEF_CICM_DEFINE
#define DEF_CICM_DEFINE

#ifdef CICM_VDSP

#include <Accelerate/Accelerate.h>

#ifdef CICM_FLOAT

#define Cicm_Signal float
#define Cicm_Packed DSPSplitComplex
#define Cicm_Complex DSPComplex
#define Cicm_Fft_Handle FFTSetup

#define	Cicm_signal_malloc(pointeur, length) pointeur = (Cicm_Signal *)malloc(length * sizeof(Cicm_Signal))
#define	Cicm_packed_malloc(pointeur, length) pointeur = (Cicm_Packed *)malloc(sizeof(Cicm_Packed)); pointeur[0].realp = (Cicm_Signal *)malloc(length * sizeof(Cicm_Signal)); pointeur[0].imagp = (Cicm_Signal *)malloc(length * sizeof(Cicm_Signal)); 

#define Cicm_fft_init_handle(order) vDSP_create_fftsetup(order, FFT_RADIX2)
#define	Cicm_free free
#define Cicm_fft_free_handle vDSP_destroy_fftsetup

#define Cicm_signal_copy(source, dest, length) vDSP_mmov(source, dest, length, 1, length, length)
#define Cicm_signal_clear(source, length) vDSP_vclr(source, 1, length)
#define Cicm_signal_dot(source1, source2, dest, length) vDSP_dotpr(source1, 1, source2, 1, dest, length)
#define Cicm_signal_add(source1, source2, dest, length) vDSP_vadd(source1, 1, source2, 1, dest, 1, length)

#define Cicm_packed_mul(source1, source2, dest, length) zvmul(source1, 1, source2, 1, dest, 1, length, 1)
#define Cicm_packed_clear(source, length) vDSP_vclr(source[0].imagp, 1, length); vDSP_vclr(source[0].realp, 1, length);

#define Cicm_fft_forward(fft_handle, complex, order) fft_zrip(fft_handle, complex, 1, order, FFT_FORWARD)
#define Cicm_fft_inverse(fft_handle, complex, order) fft_zrip(fft_handle, complex, 1, order, FFT_INVERSE)

#endif

#ifdef CICM_DOUBLE

#define Cicm_Signal float
#define Cicm_Packed DSPSplitComplex
#define Cicm_Complex DSPComplex
#define Cicm_Fft_Handle FFTSetup

#define	Cicm_signal_malloc(pointeur, length) pointeur = (Cicm_Signal *)malloc(length * sizeof(Cicm_Signal))
#define	Cicm_packed_malloc(pointeur, length) pointeur.realp = (Cicm_Signal *)malloc(length * sizeof(Cicm_Signal)); pointeur.imagp = (Cicm_Signal *)malloc(length * sizeof(Cicm_Signal)); 

#define Cicm_fft_init_handle(order) vDSP_create_fftsetup(order, FFT_RADIX2)
#define	Cicm_free free
#define Cicm_fft_free_handle vDSP_destroy_fftsetup

#define Cicm_signal_copy(source, dest, length) vDSP_mmov(source, dest, length, 1, length, length)
#define Cicm_signal_clear(source, length) vDSP_vclr(source, 1, length)
#define Cicm_packed_mul(source1, source2, dest, length) zvmul(source1, 1, source2, 1, dest, 1, length, 1) // a revoir
#define Cicm_signal_dot(source1, source2, dest, length) vDSP_dotpr(source1, 1, source2, 1, dest, length)
#define Cicm_signal_add(source1, source2, dest, length) vDSP_vadd(source1, 1, source2, 1, dest, 1, length)
#define Cicm_fft_forward(fft_handle, complex, order) fft_zrip(fft_handle, complex, 1, order, FFT_FORWARD)
#define Cicm_fft_inverse(fft_handle, complex, order) fft_zrip(fft_handle, complex, 1, order, FFT_INVERSE)

#endif
#endif

#ifdef CICM_IPPS

#include <ipps.h>

#ifdef CICM_FLOAT

#define Cicm_Signal			Ipp32f
#define Cicm_Packed			Ipp32f
#define Cicm_Fft_Handle		IppsFFTSpec_R_32f
#define Cicm_Fft_Buffer		Ipp8u

#define	Cicm_signal_malloc(pointeur, length) pointeur = ippsMalloc_32f(length)
#define	Cicm_packed_malloc(pointeur, length) pointeur = ippsMalloc_32f(length)

#define	Cicm_buffer_malloc ippsMalloc_8u
#define	Cicm_free ippsFree

#define Cicm_signal_clear(source, length) ippsZero_32f(source, length)
#define Cicm_signal_add(source1, source2, dest, length) ippsAdd_32f(source1, source2, dest, length)
#define Cicm_signal_copy(source, dest, length) ippsCopy_32f(source, dest, length)
#define Cicm_signal_dot(source1, source2, dest, length) ippsDotProd_32f(source1, source2, length, dest)
#define Cicm_packed_mul(source1, source2, dest, length) ippsMulPack_32f(source1, source2, dest, length)

#define Cicm_fft_get_size(order, spectrumSize, initSize, bufferSize) ippsFFTGetSize_R_32f(order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast, spectrumSize, initSize, bufferSize)
#define Cicm_fft_init_handle(handle, order, spectBuffer, initBuffer) ippsFFTInit_R_32f(handle, order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast, spectBuffer, initBuffer)
#define Cicm_fft_forward(signal, complex, fft_handle, buffer) ippsFFTFwd_RToPack_32f(signal, complex, fft_handle, buffer)
#define Cicm_fft_inverse(complex, dest, fft_handle, buffer) ippsFFTInv_PackToR_32f(complex, dest, fft_handle, buffer)
#define Cicm_fft_free_handle ippsFFTFree_R_32f
#endif

#ifdef CICM_DOUBLE

#define Cicm_Signal			Ipp64f
#define Cicm_Packed			Ipp64f
#define Cicm_Fft_Handle		IppsFFTSpec_R_64f
#define Cicm_Fft_Buffer		Ipp8u

#define	Cicm_signal_malloc(pointeur, length) pointeur = ippsMalloc_64f(length)
#define	Cicm_packed_malloc(pointeur, length) pointeur = ippsMalloc_64f(length)

#define	Cicm_buffer_malloc ippsMalloc_8u
#define	Cicm_free ippsFree

#define Cicm_signal_clear(source, length) ippsZero_64f(source, length)
#define Cicm_signal_add(source1, source2, dest, length) ippsAdd_64f(source1, source2, dest, length)
#define Cicm_signal_copy(source, dest, length) ippsCopy_64f(source, dest, length)
#define Cicm_signal_dot(source1, source2, dest, length) ippsDotProd_64f(source1, source2, length, dest)
#define Cicm_packed_mul(source1, source2, dest, length) ippsMulPack_64f(source1, source2, dest, length)

#define Cicm_fft_get_size(order, spectrumSize, initSize, bufferSize) ippsFFTGetSize_R_64f(order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast, spectrumSize, initSize, bufferSize)
#define Cicm_fft_init_handle(handle, order, spectBuffer, initBuffer) ippsFFTInit_R_64f(handle, order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast, spectBuffer, initBuffer)
#define Cicm_fft_forward(signal, complex, fft_handle, buffer) ippsFFTFwd_RToPack_64f(signal, complex, fft_handle, buffer)
#define Cicm_fft_inverse(complex, dest, fft_handle, buffer) ippsFFTInv_PackToR_64f(complex, dest, fft_handle, buffer)
#define Cicm_fft_free_handle ippsFFTFree_R_64f
#endif
#endif

#ifdef CICM_FFTW_GSL

#include <fftw3.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>

#ifdef CICM_FLOAT
#include <fftw3.h>
static void packed_mul(float* x, float* y, float* out, long aLength)
{
	float a, b, c, d;
	long n = 2 * aLength;
	out[0] = x[0] * y[0];
	out[n/2] = x[n/2] * y[n/2];
	for (int j = 1; j < n/2; j++)
	{
	  a = x[j];
	  b = x[n - j];
	  c = y[j];
	  d = y[n - j];
	  out[j] = a*c-b*d;
	  out[n - j] = a*d+b*c;
	}
};

#define Cicm_Signal			float
#define Cicm_Packed			float
#define Cicm_Fft_Handle		fftwf_plan

#define	Cicm_signal_malloc(pointeur, length) pointeur = (Cicm_Signal *)malloc(length * sizeof(Cicm_Signal))
#define	Cicm_packed_malloc(pointeur, length) pointeur = (Cicm_Packed *)malloc(length * sizeof(Cicm_Packed))
#define	Cicm_free free

#define Cicm_signal_clear(source, length) gsl_vector_float_set_zero(&gsl_vector_float_view_array(source, length).vector)
#define Cicm_signal_add(source1, dest, none, length) gsl_vector_float_add(&gsl_vector_float_view_array(dest, length).vector,&gsl_vector_float_view_array(source1, length).vector)
#define Cicm_signal_copy(source, dest, length) gsl_vector_float_memcpy(&gsl_vector_float_view_array(dest, length).vector,&gsl_vector_float_view_array(source, length).vector)
#define Cicm_signal_dot(source1, source2, dest, length) double result; gsl_blas_dsdot(&gsl_vector_float_view_array(source1, length).vector, &gsl_vector_float_view_array(source2, length).vector, &result); *dest = result
#define Cicm_packed_mul(source1, source2, dest, length) packed_mul(source1, source2, dest, length)

#define Cicm_fft_init_handle_forward(length, real, packed) fftwf_plan_r2r_1d(length, real, packed, FFTW_R2HC, FFTW_ESTIMATE)
#define Cicm_fft_init_handle_inverse(length, packed, real) fftwf_plan_r2r_1d(length, packed, real, FFTW_HC2R, FFTW_ESTIMATE)
#define Cicm_fft_forward fftwf_execute
#define Cicm_fft_inverse fftwf_execute
#define Cicm_fft_free_handle fftwf_destroy_plan
#endif

#ifdef CICM_DOUBLE
static void packed_mul(double* x, double* y, double* out, long aLength)
{
	double a, b, c, d;
	long n = 2 * aLength;
	out[0] = x[0] * y[0];
	out[n/2] = x[n/2] * y[n/2];
	for (int j = 1; j < n/2; j++)
	{
	  a = x[j];
	  b = x[n - j];
	  c = y[j];
	  d = y[n - j];
	  out[j] = a*c-b*d;
	  out[n - j] = a*d+b*c;
	}
};

#define Cicm_Signal			double
#define Cicm_Packed			double
#define Cicm_Fft_Handle		fftw_plan

#define	Cicm_signal_malloc(pointeur, length) pointeur = (Cicm_Signal *)malloc(length * sizeof(Cicm_Signal))
#define	Cicm_packed_malloc(pointeur, length) pointeur = (Cicm_Packed *)malloc(length * sizeof(Cicm_Packed))
#define	Cicm_free free

#define Cicm_signal_clear(source, length) gsl_vector_set_zero(&gsl_vector_view_array(source, length).vector)
#define Cicm_signal_add(source1, dest, none, length) gsl_vector_add(&gsl_vector_view_array(dest, length).vector,&gsl_vector_view_array(source1, length).vector)
#define Cicm_signal_copy(source, dest, length) gsl_vector_memcpy(&gsl_vector_view_array(dest, length).vector,&gsl_vector_view_array(source, length).vector)
#define Cicm_signal_dot(source1, source2, dest, length) gsl_blas_ddot(&gsl_vector_view_array(source1, length).vector, &gsl_vector_view_array(source2, length).vector, dest)
#define Cicm_packed_mul(source1, source2, dest, length) packed_mul(source1, source2, dest, length)

#define Cicm_fft_init_handle_forward(length, real, packed) fftw_plan_r2r_1d(length, real, packed, FFTW_R2HC, FFTW_ESTIMATE)
#define Cicm_fft_init_handle_inverse(length, packed, real) fftw_plan_r2r_1d(length, packed, real, FFTW_HC2R, FFTW_ESTIMATE)
#define Cicm_fft_forward fftw_execute
#define Cicm_fft_inverse fftw_execute
#define Cicm_fft_free_handle fftw_destroy_plan
#endif
#endif

#endif