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
#define CICM_IPP_FLOAT

#ifndef DEF_CICM_TOOLS
#define DEF_CICM_TOOLS

#include<iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
using namespace std;

#ifdef CICM_IPP_FLOAT
#include <ipps.h>
#define Cicm_Signal			Ipp32f
#define Cicm_Complex		Ipp32fc
#define Cicm_Split_Complex	DSPSplitComplex
#define Cicm_Fft_Handle		IppsFFTSpec_R_32f

#define	Cicm_malloc_signal ippsMalloc_32f
#define	Cicm_malloc_complex ippsMalloc_32fc

#define Cicm_mmov vDSP_mmov
#define Cicm_signal_clear(signal, size) ippsZero_32f(signal, size)
#define Cicm_complex_mul(signal, ir, length) ippsMulPack_32f(signal, ir, signal, length)
#define Cicm_dotpr vDSP_dotpr
#define Cicm_vadd vDSP_vadd
#define Cicm_ctoz vDSP_ctoz
#define Cicm_ztoc vDSP_ztoc

#define Cicm_fft_forward(signal, complex, fft_handle) ippsFFTFwd_RToPack_32f(signal, complex, fft_handle, 0)
#define Cicm_fft_inverse(complex, signal, fft_handle) ippsFFTInv_PackToR_32f(complex, signal, fft_handle, 0)

#endif
#ifdef CICM_IPP_DOUBLE
#include <ipps.h>
#define Cicm_Signal Ipp64f
#define Cicm_Complex Ipp64fc
#define	Cicm_malloc_real ippsMalloc_64f
#define	Cicm_malloc_complex ippsMalloc_64fc

#endif



#ifdef CICM_FLOAT
#include <Accelerate/Accelerate.h>
#define Cicm_Signal float
#define Cicm_Split_Complex DSPSplitComplex
#define Cicm_Complex DSPComplex
#define Cicm_Fft_setup FFTSetup

#define	Cicm_malloc_signal(size) malloc(size * sizeof(Cicm_Signal))
#define	Cicm_malloc_complex(size) malloc(size * sizeof(Cicm_Signal))

#define Cicm_create_Fft_setup vDSP_create_fftsetup
#define Cicm_destroy_Fft_setup vDSP_destroy_fftsetup
#define Cicm_mmov vDSP_mmov
#define Cicm_signal_clear(signal, size) vDSP_vclr(signal, 1, m_array_size)
#define Cicm_zvmul vDSP_zvmul
#define Cicm_dotpr vDSP_dotpr
#define Cicm_vadd vDSP_vadd
#define Cicm_ctoz vDSP_ctoz
#define Cicm_ztoc vDSP_ztoc
#define Cicm_fft_zrip vDSP_fft_zrip

#endif
#ifdef CICM_DOUBLE
#include <Accelerate/Accelerate.h>
#define Cicm_Signal double
#define Cicm_Split_Complex DSPDoubleSplitComplex
#define Cicm_Complex DSPDoubleComplex
#define Cicm_Fft_setup FFTSetupD

#define Cicm_create_Fft_setup vDSP_create_fftsetupD
#define Cicm_destroy_Fft_setup vDSP_destroy_fftsetupD
#define Cicm_mmov vDSP_mmovD
#define Cicm_vclr vDSP_vclrD
#define Cicm_zvmul vDSP_zvmulD
#define Cicm_dotpr vDSP_dotprD
#define Cicm_vadd vDSP_vaddD
#define Cicm_ctoz vDSP_ctozD
#define Cicm_ztoc vDSP_ztocD
#define Cicm_fft_zrip vDSP_fft_zripD

#endif

#ifdef CICM_FFTW_FLOAT
#include <ipps.h>
#define Cicm_Signal Ipp64f
#define Cicm_Complex Ipp64fc
#define	Cicm_malloc_real ippsMalloc_64f
#define	Cicm_malloc_complex ippsMalloc_64fc

#endif
#ifdef CICM_FFTW_DOUBLE
#include <fftw3.h>
#define Cicm_Signal		double
#define Cicm_Complex	Ipp64fc
#define	Cicm_malloc_real ippsMalloc_64f
#define	Cicm_malloc_complex ippsMalloc_64fc

#endif

#define TOOLS_PI 3.141592653589793238462643383279502884

class Tools
{
public:
	Tools();
	~Tools();
	template<typename Type> static Type clip(Type aValue, Type aMinimum = 0, Type aMaximum = 1)
	{
		if(aValue < aMinimum)
			aValue = aMinimum;
		else if(aValue > aMaximum)
			aValue = aMaximum;
		return aValue;
	}

	template<typename Type> static Type clip_min(Type aValue, Type aMinimum = 0)
	{
		if(aValue < aMinimum)
			aValue = aMinimum;
		return aValue;
	}

	template<typename Type> static Type clip_max(Type aValue, Type aMinimum = 0)
	{
		if(aValue < aMinimum)
			aValue = aMinimum;
		return aValue;
	}

	template<typename Type> static Type clip_power_of_two(Type aValue)
	{
		if(aValue < 2)
		aValue = 2;
		else if(aValue&(aValue-1) != 0)
		{
			while (aValue&(aValue-1) != 0) 
				aValue--;
		}
		return aValue;
	}

	template<typename Type> static Type log2(Type n)
	{
		return log(n) / log(2.);
	}
};

#endif