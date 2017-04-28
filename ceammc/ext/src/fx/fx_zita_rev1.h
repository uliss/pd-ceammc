//----------------------------------------------------------
// name: "fx_zita_rev1"
//
// Code generated with Faust 0.9.96 (http://faust.grame.fr)
//----------------------------------------------------------

/* link with  */
#include <math.h>
#ifndef FAUSTPOWER
#define FAUSTPOWER
#include <cmath>
template <int N> inline int faustpower(int x)              { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline int faustpower<0>(int x)            { return 1; }
template <> 	 inline int faustpower<1>(int x)            { return x; }
template <> 	 inline int faustpower<2>(int x)            { return x*x; }
template <int N> inline float faustpower(float x)            { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline float faustpower<0>(float x)          { return 1; }
template <> 	 inline float faustpower<1>(float x)          { return x; }
template <> 	 inline float faustpower<2>(float x)          { return x*x; }
#endif
/************************************************************************
 ************************************************************************
    FAUST Architecture File
    Copyright (C) 2006-2011 Albert Graef <Dr.Graef@t-online.de>
    ---------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2.1 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with the GNU C Library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA.
 ************************************************************************
 ************************************************************************/

/* Pd architecture file, written by Albert Graef <Dr.Graef@t-online.de>.
   This was derived from minimal.cpp included in the Faust distribution.
   Please note that this is to be compiled as a shared library, which is
   then loaded dynamically by Pd as an external. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/************************************************************************
    IMPORTANT NOTE : this file contains two clearly delimited sections :
    the ARCHITECTURE section (in two parts) and the USER section. Each section
    is governed by its own copyright and license. Please check individually
    each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
    Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; If not, see <http://www.gnu.org/licenses/>.

    EXCEPTION : As a special exception, you may create a larger work
    that contains this FAUST architecture section and distribute
    that work under terms of your choice, so long as this FAUST
    architecture section is not modified.

 ************************************************************************
 ************************************************************************/
 
/******************************************************************************
*******************************************************************************

								FAUST DSP

*******************************************************************************
*******************************************************************************/

#ifndef __dsp__
#define __dsp__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;
struct Meta;

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the UI* parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the UI* user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /** Global init, calls the following methods:
         * - static class 'classInit': static table initialisation
         * - 'instanceInit': constants and instance table initialisation
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void init(int samplingRate) = 0;
    
        /** Init instance state
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void instanceInit(int samplingRate) = 0;
    
        /** Init instance constant state
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void instanceConstants(int samplingRate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
    
        /**  
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value metadata).
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with sucessive in/out audio buffers.
         *
         * @param count - the nomber of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the nomber of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = 0):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int samplingRate) { fDSP->init(samplingRate); }
        virtual void instanceInit(int samplingRate) { fDSP->instanceInit(samplingRate); }
        virtual void instanceConstants(int samplingRate) { fDSP->instanceConstants(samplingRate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { return fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
       
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/************************************************************************
    FAUST Architecture File
    Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; If not, see <http://www.gnu.org/licenses/>.

    EXCEPTION : As a special exception, you may create a larger work
    that contains this FAUST architecture section and distribute
    that work under terms of your choice, so long as this FAUST
    architecture section is not modified.


 ************************************************************************
 ************************************************************************/

#ifndef FAUST_UI_H
#define FAUST_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

class UI
{

    public:

        UI() {}

        virtual ~UI() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

#endif
/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

 ************************************************************************
 ************************************************************************/
 
#ifndef __meta__
#define __meta__

struct Meta
{
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {};
};

#endif
/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

 ************************************************************************
 ************************************************************************/
 
#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <string.h>
#include <stdlib.h>


using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

inline int lsr(int x, int n)	{ return int(((unsigned int)x) >> n); }

inline int int2pow2(int x)		{ int r = 0; while ((1<<r) < x) r++; return r; }

inline long lopt(char* argv[], const char* name, long def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
	return def;
}

inline bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

inline const char* lopts(char* argv[], const char* name, const char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}

#endif


#include "ceammc_atomlist.h"
#include <m_pd.h>

/******************************************************************************
*******************************************************************************

                                   VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/

#ifdef FAUST_MACRO
// clang-format off
// clang-format on
#endif

#define sym(name) xsym(name)
#define xsym(name) #name

/***************************************************************************
   Pd UI interface
***************************************************************************/

// clang-format off
#ifndef FAUST_MACRO
struct zita_rev1 : public dsp {
};
#endif
// clang-format on

#include "ceammc_faust.h"
using namespace ceammc::faust;

/******************************************************************************
*******************************************************************************

                FAUST DSP

*******************************************************************************
*******************************************************************************/

//----------------------------------------------------------------------------
//  FAUST generated signal processor
//----------------------------------------------------------------------------

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  


#ifndef FAUSTCLASS 
#define FAUSTCLASS zita_rev1
#endif

class zita_rev1 : public dsp {
  private:
	FAUSTFLOAT 	fcheckbox0;
	int 	IOTA;
	float 	fVec0[8192];
	FAUSTFLOAT 	fslider0;
	float 	fRec0[2];
	float 	fConst0;
	float 	fConst1;
	FAUSTFLOAT 	fslider1;
	float 	fConst2;
	float 	fConst3;
	FAUSTFLOAT 	fslider2;
	FAUSTFLOAT 	fslider3;
	float 	fConst4;
	FAUSTFLOAT 	fslider4;
	float 	fRec12[2];
	float 	fRec11[2];
	float 	fVec1[16384];
	float 	fConst5;
	int 	iConst6;
	float 	fVec2[8192];
	float 	fConst7;
	FAUSTFLOAT 	fslider5;
	float 	fVec3[2048];
	int 	iConst8;
	float 	fRec9[2];
	float 	fConst9;
	float 	fConst10;
	float 	fRec16[2];
	float 	fRec15[2];
	float 	fVec4[8192];
	float 	fConst11;
	int 	iConst12;
	float 	fVec5[2048];
	int 	iConst13;
	float 	fRec13[2];
	float 	fConst14;
	float 	fConst15;
	float 	fRec20[2];
	float 	fRec19[2];
	float 	fVec6[16384];
	float 	fConst16;
	int 	iConst17;
	float 	fVec7[2048];
	int 	iConst18;
	float 	fRec17[2];
	float 	fConst19;
	float 	fConst20;
	float 	fRec24[2];
	float 	fRec23[2];
	float 	fVec8[8192];
	float 	fConst21;
	int 	iConst22;
	float 	fVec9[1024];
	int 	iConst23;
	float 	fRec21[2];
	float 	fConst24;
	float 	fConst25;
	float 	fRec28[2];
	float 	fRec27[2];
	float 	fVec10[16384];
	float 	fConst26;
	int 	iConst27;
	float 	fVec11[1024];
	int 	iConst28;
	float 	fRec25[2];
	float 	fConst29;
	float 	fConst30;
	float 	fRec32[2];
	float 	fRec31[2];
	float 	fVec12[8192];
	float 	fConst31;
	int 	iConst32;
	float 	fVec13[2048];
	int 	iConst33;
	float 	fRec29[2];
	float 	fConst34;
	float 	fConst35;
	float 	fRec36[2];
	float 	fRec35[2];
	float 	fVec14[8192];
	float 	fConst36;
	int 	iConst37;
	float 	fVec15[2048];
	int 	iConst38;
	float 	fRec33[2];
	float 	fConst39;
	float 	fConst40;
	float 	fRec40[2];
	float 	fRec39[2];
	float 	fVec16[8192];
	float 	fConst41;
	int 	iConst42;
	float 	fVec17[1024];
	int 	iConst43;
	float 	fRec37[2];
	float 	fRec1[3];
	float 	fRec2[3];
	float 	fRec3[3];
	float 	fRec4[3];
	float 	fRec5[3];
	float 	fRec6[3];
	float 	fRec7[3];
	float 	fRec8[3];
	int fSamplingFreq;

  public:
	virtual void metadata(Meta* m) { 
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.0");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("name", "fx_zita_rev1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.0");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1");
	}

	virtual int getNumInputs() { return 2; }
	virtual int getNumOutputs() { return 2; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = min(1.92e+05f, max(1.0f, (float)fSamplingFreq));
		fConst1 = (6.2831855f / float(fConst0));
		fConst2 = floorf(((0.210389f * fConst0) + 0.5f));
		fConst3 = ((0 - (6.9077554f * fConst2)) / fConst0);
		fConst4 = (3.1415927f / fConst0);
		fConst5 = floorf(((0.024421f * fConst0) + 0.5f));
		iConst6 = int((int((fConst2 - fConst5)) & 16383));
		fConst7 = (0.001f * fConst0);
		iConst8 = int((int((fConst5 + -1)) & 2047));
		fConst9 = floorf(((0.192303f * fConst0) + 0.5f));
		fConst10 = ((0 - (6.9077554f * fConst9)) / fConst0);
		fConst11 = floorf(((0.029291f * fConst0) + 0.5f));
		iConst12 = int((int((fConst9 - fConst11)) & 8191));
		iConst13 = int((int((fConst11 + -1)) & 2047));
		fConst14 = floorf(((0.256891f * fConst0) + 0.5f));
		fConst15 = ((0 - (6.9077554f * fConst14)) / fConst0);
		fConst16 = floorf(((0.027333f * fConst0) + 0.5f));
		iConst17 = int((int((fConst14 - fConst16)) & 16383));
		iConst18 = int((int((fConst16 + -1)) & 2047));
		fConst19 = floorf(((0.153129f * fConst0) + 0.5f));
		fConst20 = ((0 - (6.9077554f * fConst19)) / fConst0);
		fConst21 = floorf(((0.020346f * fConst0) + 0.5f));
		iConst22 = int((int((fConst19 - fConst21)) & 8191));
		iConst23 = int((int((fConst21 + -1)) & 1023));
		fConst24 = floorf(((0.219991f * fConst0) + 0.5f));
		fConst25 = ((0 - (6.9077554f * fConst24)) / fConst0);
		fConst26 = floorf(((0.019123f * fConst0) + 0.5f));
		iConst27 = int((int((fConst24 - fConst26)) & 16383));
		iConst28 = int((int((fConst26 + -1)) & 1023));
		fConst29 = floorf(((0.127837f * fConst0) + 0.5f));
		fConst30 = ((0 - (6.9077554f * fConst29)) / fConst0);
		fConst31 = floorf(((0.031604f * fConst0) + 0.5f));
		iConst32 = int((int((fConst29 - fConst31)) & 8191));
		iConst33 = int((int((fConst31 + -1)) & 2047));
		fConst34 = floorf(((0.174713f * fConst0) + 0.5f));
		fConst35 = ((0 - (6.9077554f * fConst34)) / fConst0);
		fConst36 = floorf(((0.022904f * fConst0) + 0.5f));
		iConst37 = int((int((fConst34 - fConst36)) & 8191));
		iConst38 = int((int((fConst36 + -1)) & 2047));
		fConst39 = floorf(((0.125f * fConst0) + 0.5f));
		fConst40 = ((0 - (6.9077554f * fConst39)) / fConst0);
		fConst41 = floorf(((0.013458f * fConst0) + 0.5f));
		iConst42 = int((int((fConst39 - fConst41)) & 8191));
		iConst43 = int((int((fConst41 + -1)) & 1023));
	}
	virtual void instanceResetUserInterface() {
		fcheckbox0 = 0.0;
		fslider0 = 1.0f;
		fslider1 = 6e+03f;
		fslider2 = 2.0f;
		fslider3 = 3.0f;
		fslider4 = 2e+02f;
		fslider5 = 6e+01f;
	}
	virtual void instanceClear() {
		IOTA = 0;
		for (int i=0; i<8192; i++) fVec0[i] = 0;
		for (int i=0; i<2; i++) fRec0[i] = 0;
		for (int i=0; i<2; i++) fRec12[i] = 0;
		for (int i=0; i<2; i++) fRec11[i] = 0;
		for (int i=0; i<16384; i++) fVec1[i] = 0;
		for (int i=0; i<8192; i++) fVec2[i] = 0;
		for (int i=0; i<2048; i++) fVec3[i] = 0;
		for (int i=0; i<2; i++) fRec9[i] = 0;
		for (int i=0; i<2; i++) fRec16[i] = 0;
		for (int i=0; i<2; i++) fRec15[i] = 0;
		for (int i=0; i<8192; i++) fVec4[i] = 0;
		for (int i=0; i<2048; i++) fVec5[i] = 0;
		for (int i=0; i<2; i++) fRec13[i] = 0;
		for (int i=0; i<2; i++) fRec20[i] = 0;
		for (int i=0; i<2; i++) fRec19[i] = 0;
		for (int i=0; i<16384; i++) fVec6[i] = 0;
		for (int i=0; i<2048; i++) fVec7[i] = 0;
		for (int i=0; i<2; i++) fRec17[i] = 0;
		for (int i=0; i<2; i++) fRec24[i] = 0;
		for (int i=0; i<2; i++) fRec23[i] = 0;
		for (int i=0; i<8192; i++) fVec8[i] = 0;
		for (int i=0; i<1024; i++) fVec9[i] = 0;
		for (int i=0; i<2; i++) fRec21[i] = 0;
		for (int i=0; i<2; i++) fRec28[i] = 0;
		for (int i=0; i<2; i++) fRec27[i] = 0;
		for (int i=0; i<16384; i++) fVec10[i] = 0;
		for (int i=0; i<1024; i++) fVec11[i] = 0;
		for (int i=0; i<2; i++) fRec25[i] = 0;
		for (int i=0; i<2; i++) fRec32[i] = 0;
		for (int i=0; i<2; i++) fRec31[i] = 0;
		for (int i=0; i<8192; i++) fVec12[i] = 0;
		for (int i=0; i<2048; i++) fVec13[i] = 0;
		for (int i=0; i<2; i++) fRec29[i] = 0;
		for (int i=0; i<2; i++) fRec36[i] = 0;
		for (int i=0; i<2; i++) fRec35[i] = 0;
		for (int i=0; i<8192; i++) fVec14[i] = 0;
		for (int i=0; i<2048; i++) fVec15[i] = 0;
		for (int i=0; i<2; i++) fRec33[i] = 0;
		for (int i=0; i<2; i++) fRec40[i] = 0;
		for (int i=0; i<2; i++) fRec39[i] = 0;
		for (int i=0; i<8192; i++) fVec16[i] = 0;
		for (int i=0; i<1024; i++) fVec17[i] = 0;
		for (int i=0; i<2; i++) fRec37[i] = 0;
		for (int i=0; i<3; i++) fRec1[i] = 0;
		for (int i=0; i<3; i++) fRec2[i] = 0;
		for (int i=0; i<3; i++) fRec3[i] = 0;
		for (int i=0; i<3; i++) fRec4[i] = 0;
		for (int i=0; i<3; i++) fRec5[i] = 0;
		for (int i=0; i<3; i++) fRec6[i] = 0;
		for (int i=0; i<3; i++) fRec7[i] = 0;
		for (int i=0; i<3; i++) fRec8[i] = 0;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	virtual zita_rev1* clone() {
		return new zita_rev1();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
	}
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("0x00");
		ui_interface->addCheckButton("bypass", &fcheckbox0);
		ui_interface->declare(&fslider3, "scale", "log");
		ui_interface->declare(&fslider3, "style", "knob");
		ui_interface->declare(&fslider3, "unit", "s");
		ui_interface->addVerticalSlider("decay_low", &fslider3, 3.0f, 1.0f, 25.0f, 0.1f);
		ui_interface->declare(&fslider2, "scale", "log");
		ui_interface->declare(&fslider2, "style", "knob");
		ui_interface->declare(&fslider2, "unit", "s");
		ui_interface->addVerticalSlider("decay_mid", &fslider2, 2.0f, 1.0f, 25.0f, 0.1f);
		ui_interface->declare(&fslider5, "style", "knob");
		ui_interface->declare(&fslider5, "unit", "ms");
		ui_interface->addVerticalSlider("delay", &fslider5, 6e+01f, 2e+01f, 1e+02f, 1.0f);
		ui_interface->declare(&fslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fslider0, 1.0f, 0.0f, 1.0f, 0.01f);
		ui_interface->declare(&fslider1, "scale", "log");
		ui_interface->declare(&fslider1, "style", "knob");
		ui_interface->declare(&fslider1, "unit", "Hz");
		ui_interface->addVerticalSlider("dump_hf", &fslider1, 6e+03f, 1.5e+03f, 2.352e+04f, 1.0f);
		ui_interface->declare(&fslider4, "scale", "log");
		ui_interface->declare(&fslider4, "style", "knob");
		ui_interface->declare(&fslider4, "unit", "Hz");
		ui_interface->addVerticalSlider("freq_low", &fslider4, 2e+02f, 5e+01f, 1e+03f, 1.0f);
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		int 	iSlow0 = int(float(fcheckbox0));
		float 	fSlow1 = (0.001f * float(fslider0));
		float 	fSlow2 = cosf((fConst1 * float(fslider1)));
		float 	fSlow3 = float(fslider2);
		float 	fSlow4 = expf((fConst3 / fSlow3));
		float 	fSlow5 = faustpower<2>(fSlow4);
		float 	fSlow6 = (1.0f - (fSlow2 * fSlow5));
		float 	fSlow7 = (1.0f - fSlow5);
		float 	fSlow8 = (fSlow6 / fSlow7);
		float 	fSlow9 = sqrtf(max((float)0, ((faustpower<2>(fSlow6) / faustpower<2>(fSlow7)) + -1.0f)));
		float 	fSlow10 = (fSlow8 - fSlow9);
		float 	fSlow11 = (fSlow4 * (fSlow9 + (1.0f - fSlow8)));
		float 	fSlow12 = float(fslider3);
		float 	fSlow13 = ((expf((fConst3 / fSlow12)) / fSlow4) + -1);
		float 	fSlow14 = (1.0f / tanf((fConst4 * float(fslider4))));
		float 	fSlow15 = (fSlow14 + 1);
		float 	fSlow16 = (0 - ((1 - fSlow14) / fSlow15));
		float 	fSlow17 = (1.0f / fSlow15);
		int 	iSlow18 = int((int((fConst7 * float(fslider5))) & 8191));
		float 	fSlow19 = expf((fConst10 / fSlow3));
		float 	fSlow20 = faustpower<2>(fSlow19);
		float 	fSlow21 = (1.0f - (fSlow20 * fSlow2));
		float 	fSlow22 = (1.0f - fSlow20);
		float 	fSlow23 = (fSlow21 / fSlow22);
		float 	fSlow24 = sqrtf(max((float)0, ((faustpower<2>(fSlow21) / faustpower<2>(fSlow22)) + -1.0f)));
		float 	fSlow25 = (fSlow23 - fSlow24);
		float 	fSlow26 = (fSlow19 * (fSlow24 + (1.0f - fSlow23)));
		float 	fSlow27 = ((expf((fConst10 / fSlow12)) / fSlow19) + -1);
		float 	fSlow28 = expf((fConst15 / fSlow3));
		float 	fSlow29 = faustpower<2>(fSlow28);
		float 	fSlow30 = (1.0f - (fSlow2 * fSlow29));
		float 	fSlow31 = (1.0f - fSlow29);
		float 	fSlow32 = (fSlow30 / fSlow31);
		float 	fSlow33 = sqrtf(max((float)0, ((faustpower<2>(fSlow30) / faustpower<2>(fSlow31)) + -1.0f)));
		float 	fSlow34 = (fSlow32 - fSlow33);
		float 	fSlow35 = (fSlow28 * (fSlow33 + (1.0f - fSlow32)));
		float 	fSlow36 = ((expf((fConst15 / fSlow12)) / fSlow28) + -1);
		float 	fSlow37 = expf((fConst20 / fSlow3));
		float 	fSlow38 = faustpower<2>(fSlow37);
		float 	fSlow39 = (1.0f - (fSlow2 * fSlow38));
		float 	fSlow40 = (1.0f - fSlow38);
		float 	fSlow41 = (fSlow39 / fSlow40);
		float 	fSlow42 = sqrtf(max((float)0, ((faustpower<2>(fSlow39) / faustpower<2>(fSlow40)) + -1.0f)));
		float 	fSlow43 = (fSlow41 - fSlow42);
		float 	fSlow44 = (fSlow37 * (fSlow42 + (1.0f - fSlow41)));
		float 	fSlow45 = ((expf((fConst20 / fSlow12)) / fSlow37) + -1);
		float 	fSlow46 = expf((fConst25 / fSlow3));
		float 	fSlow47 = faustpower<2>(fSlow46);
		float 	fSlow48 = (1.0f - (fSlow47 * fSlow2));
		float 	fSlow49 = (1.0f - fSlow47);
		float 	fSlow50 = (fSlow48 / fSlow49);
		float 	fSlow51 = sqrtf(max((float)0, ((faustpower<2>(fSlow48) / faustpower<2>(fSlow49)) + -1.0f)));
		float 	fSlow52 = (fSlow50 - fSlow51);
		float 	fSlow53 = (fSlow46 * (fSlow51 + (1.0f - fSlow50)));
		float 	fSlow54 = ((expf((fConst25 / fSlow12)) / fSlow46) + -1);
		float 	fSlow55 = expf((fConst30 / fSlow3));
		float 	fSlow56 = faustpower<2>(fSlow55);
		float 	fSlow57 = (1.0f - (fSlow56 * fSlow2));
		float 	fSlow58 = (1.0f - fSlow56);
		float 	fSlow59 = (fSlow57 / fSlow58);
		float 	fSlow60 = sqrtf(max((float)0, ((faustpower<2>(fSlow57) / faustpower<2>(fSlow58)) + -1.0f)));
		float 	fSlow61 = (fSlow59 - fSlow60);
		float 	fSlow62 = (fSlow55 * (fSlow60 + (1.0f - fSlow59)));
		float 	fSlow63 = ((expf((fConst30 / fSlow12)) / fSlow55) + -1);
		float 	fSlow64 = expf((fConst35 / fSlow3));
		float 	fSlow65 = faustpower<2>(fSlow64);
		float 	fSlow66 = (1.0f - (fSlow2 * fSlow65));
		float 	fSlow67 = (1.0f - fSlow65);
		float 	fSlow68 = (fSlow66 / fSlow67);
		float 	fSlow69 = sqrtf(max((float)0, ((faustpower<2>(fSlow66) / faustpower<2>(fSlow67)) + -1.0f)));
		float 	fSlow70 = (fSlow68 - fSlow69);
		float 	fSlow71 = (fSlow64 * (fSlow69 + (1.0f - fSlow68)));
		float 	fSlow72 = ((expf((fConst35 / fSlow12)) / fSlow64) + -1);
		float 	fSlow73 = expf((fConst40 / fSlow3));
		float 	fSlow74 = faustpower<2>(fSlow73);
		float 	fSlow75 = (1.0f - (fSlow74 * fSlow2));
		float 	fSlow76 = (1.0f - fSlow74);
		float 	fSlow77 = (fSlow75 / fSlow76);
		float 	fSlow78 = sqrtf(max((float)0, ((faustpower<2>(fSlow75) / faustpower<2>(fSlow76)) + -1.0f)));
		float 	fSlow79 = (fSlow77 - fSlow78);
		float 	fSlow80 = (fSlow73 * (fSlow78 + (1.0f - fSlow77)));
		float 	fSlow81 = ((expf((fConst40 / fSlow12)) / fSlow73) + -1);
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* input1 = input[1];
		FAUSTFLOAT* output0 = output[0];
		FAUSTFLOAT* output1 = output[1];
		for (int i=0; i<count; i++) {
			float fTemp0 = (float)input0[i];
			float fTemp1 = ((iSlow0)?0:fTemp0);
			fVec0[IOTA&8191] = fTemp1;
			fRec0[0] = (fSlow1 + (0.999f * fRec0[1]));
			float fTemp2 = (1 - fRec0[0]);
			fRec12[0] = ((fSlow16 * fRec12[1]) + (fSlow17 * (fRec2[2] + fRec2[1])));
			fRec11[0] = ((fSlow10 * fRec11[1]) + (fSlow11 * (fRec2[1] + (fSlow13 * fRec12[0]))));
			fVec1[IOTA&16383] = ((0.35355338f * fRec11[0]) + 1e-20f);
			float fTemp3 = (float)input1[i];
			float fTemp4 = ((iSlow0)?0:fTemp3);
			fVec2[IOTA&8191] = fTemp4;
			float fTemp5 = (0.3f * fVec2[(IOTA-iSlow18)&8191]);
			float fTemp6 = (fVec1[(IOTA-iConst6)&16383] + (fTemp5 + (0.6f * fRec9[1])));
			fVec3[IOTA&2047] = fTemp6;
			fRec9[0] = fVec3[(IOTA-iConst8)&2047];
			float 	fRec10 = (0 - (0.6f * fVec3[IOTA&2047]));
			fRec16[0] = ((fSlow16 * fRec16[1]) + (fSlow17 * (fRec6[1] + fRec6[2])));
			fRec15[0] = ((fSlow25 * fRec15[1]) + (fSlow26 * ((fSlow27 * fRec16[0]) + fRec6[1])));
			fVec4[IOTA&8191] = ((0.35355338f * fRec15[0]) + 1e-20f);
			float fTemp7 = ((fTemp5 + (0.6f * fRec13[1])) + fVec4[(IOTA-iConst12)&8191]);
			fVec5[IOTA&2047] = fTemp7;
			fRec13[0] = fVec5[(IOTA-iConst13)&2047];
			float 	fRec14 = (0 - (0.6f * fVec5[IOTA&2047]));
			fRec20[0] = ((fSlow16 * fRec20[1]) + (fSlow17 * (fRec4[1] + fRec4[2])));
			fRec19[0] = ((fSlow34 * fRec19[1]) + (fSlow35 * ((fSlow36 * fRec20[0]) + fRec4[1])));
			fVec6[IOTA&16383] = ((0.35355338f * fRec19[0]) + 1e-20f);
			float fTemp8 = (((0.6f * fRec17[1]) + fVec6[(IOTA-iConst17)&16383]) - fTemp5);
			fVec7[IOTA&2047] = fTemp8;
			fRec17[0] = fVec7[(IOTA-iConst18)&2047];
			float 	fRec18 = (0 - (0.6f * fVec7[IOTA&2047]));
			float fTemp9 = (0.3f * fVec0[(IOTA-iSlow18)&8191]);
			fRec24[0] = ((fSlow17 * (fRec1[1] + fRec1[2])) + (fSlow16 * fRec24[1]));
			fRec23[0] = ((fSlow43 * fRec23[1]) + (fSlow44 * (fRec1[1] + (fSlow45 * fRec24[0]))));
			fVec8[IOTA&8191] = ((0.35355338f * fRec23[0]) + 1e-20f);
			float fTemp10 = ((fTemp9 + fVec8[(IOTA-iConst22)&8191]) - (0.6f * fRec21[1]));
			fVec9[IOTA&1023] = fTemp10;
			fRec21[0] = fVec9[(IOTA-iConst23)&1023];
			float 	fRec22 = (0.6f * fVec9[IOTA&1023]);
			fRec28[0] = ((fSlow16 * fRec28[1]) + (fSlow17 * (fRec8[1] + fRec8[2])));
			fRec27[0] = ((fSlow52 * fRec27[1]) + (fSlow53 * ((fSlow54 * fRec28[0]) + fRec8[1])));
			fVec10[IOTA&16383] = ((0.35355338f * fRec27[0]) + 1e-20f);
			float fTemp11 = (((0.6f * fRec25[1]) + fVec10[(IOTA-iConst27)&16383]) - fTemp5);
			fVec11[IOTA&1023] = fTemp11;
			fRec25[0] = fVec11[(IOTA-iConst28)&1023];
			float 	fRec26 = (0 - (0.6f * fVec11[IOTA&1023]));
			float fTemp12 = (fRec25[1] + fRec26);
			float fTemp13 = (fRec22 + fTemp12);
			float fTemp14 = (fRec18 + (fRec17[1] + fTemp13));
			fRec32[0] = ((fSlow17 * (fRec3[1] + fRec3[2])) + (fSlow16 * fRec32[1]));
			fRec31[0] = ((fSlow61 * fRec31[1]) + (fSlow62 * (fRec3[1] + (fSlow63 * fRec32[0]))));
			fVec12[IOTA&8191] = ((0.35355338f * fRec31[0]) + 1e-20f);
			float fTemp15 = (fVec12[(IOTA-iConst32)&8191] - (fTemp9 + (0.6f * fRec29[1])));
			fVec13[IOTA&2047] = fTemp15;
			fRec29[0] = fVec13[(IOTA-iConst33)&2047];
			float 	fRec30 = (0.6f * fVec13[IOTA&2047]);
			fRec36[0] = ((fSlow17 * (fRec5[1] + fRec5[2])) + (fSlow16 * fRec36[1]));
			fRec35[0] = ((fSlow70 * fRec35[1]) + (fSlow71 * (fRec5[1] + (fSlow72 * fRec36[0]))));
			fVec14[IOTA&8191] = ((0.35355338f * fRec35[0]) + 1e-20f);
			float fTemp16 = ((fTemp9 + fVec14[(IOTA-iConst37)&8191]) - (0.6f * fRec33[1]));
			fVec15[IOTA&2047] = fTemp16;
			fRec33[0] = fVec15[(IOTA-iConst38)&2047];
			float 	fRec34 = (0.6f * fVec15[IOTA&2047]);
			fRec40[0] = ((fSlow17 * (fRec7[1] + fRec7[2])) + (fSlow16 * fRec40[1]));
			fRec39[0] = ((fSlow79 * fRec39[1]) + (fSlow80 * (fRec7[1] + (fSlow81 * fRec40[0]))));
			fVec16[IOTA&8191] = ((0.35355338f * fRec39[0]) + 1e-20f);
			float fTemp17 = (fVec16[(IOTA-iConst42)&8191] - (fTemp9 + (0.6f * fRec37[1])));
			fVec17[IOTA&1023] = fTemp17;
			fRec37[0] = fVec17[(IOTA-iConst43)&1023];
			float 	fRec38 = (0.6f * fVec17[IOTA&1023]);
			fRec1[0] = (((((((((fRec9[1] + (fRec14 + (fRec13[1] + fTemp14))) + fRec29[1]) + fRec30) + fRec33[1]) + fRec34) + fRec21[1]) + fRec10) + fRec37[1]) + fRec38);
			float fTemp18 = (fRec18 + (fRec17[1] + fTemp12));
			fRec2[0] = ((((((((fRec22 + fRec29[1]) + fRec30) + fRec33[1]) + fRec34) + fRec21[1]) + fRec37[1]) + fRec38) - ((fRec9[1] + (fRec14 + (fRec13[1] + fTemp18))) + fRec10));
			fRec3[0] = ((((((fRec9[1] + (fRec14 + (fRec22 + fRec13[1]))) + fRec33[1]) + fRec34) + fRec21[1]) + fRec10) - ((((fTemp18 + fRec29[1]) + fRec30) + fRec37[1]) + fRec38));
			fRec4[0] = ((((fTemp14 + fRec33[1]) + fRec34) + fRec21[1]) - (((((fRec29[1] + (fRec9[1] + (fRec13[1] + fRec14))) + fRec30) + fRec10) + fRec37[1]) + fRec38));
			float fTemp19 = (fRec18 + (fRec22 + fRec17[1]));
			fRec5[0] = ((((((fRec9[1] + fTemp19) + fRec29[1]) + fRec30) + fRec21[1]) + fRec10) - (((((fRec14 + (fRec13[1] + fTemp12)) + fRec33[1]) + fRec34) + fRec37[1]) + fRec38));
			float fTemp20 = (fRec17[1] + fRec18);
			fRec6[0] = (((((fRec14 + (fRec13[1] + fTemp13)) + fRec29[1]) + fRec30) + fRec21[1]) - ((((((fRec9[1] + fTemp20) + fRec33[1]) + fRec34) + fRec10) + fRec37[1]) + fRec38));
			fRec7[0] = ((((((fRec9[1] + fTemp13) + fRec21[1]) + fRec10) + fRec37[1]) + fRec38) - ((((fRec29[1] + (fRec14 + (fRec13[1] + fTemp20))) + fRec30) + fRec33[1]) + fRec34));
			fRec8[0] = ((fRec38 + (fRec37[1] + (fRec21[1] + (fRec14 + (fRec13[1] + fTemp19))))) - (fRec10 + (fRec34 + (fRec33[1] + (fRec30 + (fRec29[1] + (fRec9[1] + fTemp12)))))));
			output0[i] = (FAUSTFLOAT)((iSlow0)?fTemp0:((fVec0[IOTA&8191] * fTemp2) + (0.37f * (fRec0[0] * (fRec2[0] + fRec3[0])))));
			output1[i] = (FAUSTFLOAT)((iSlow0)?fTemp3:((fVec2[IOTA&8191] * fTemp2) + (0.37f * (fRec0[0] * (fRec2[0] - fRec3[0])))));
			// post processing
			fRec8[2] = fRec8[1]; fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1]; fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1]; fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1]; fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1]; fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1]; fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1]; fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1]; fRec1[1] = fRec1[0];
			fRec37[1] = fRec37[0];
			fRec39[1] = fRec39[0];
			fRec40[1] = fRec40[0];
			fRec33[1] = fRec33[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec29[1] = fRec29[0];
			fRec31[1] = fRec31[0];
			fRec32[1] = fRec32[0];
			fRec25[1] = fRec25[0];
			fRec27[1] = fRec27[0];
			fRec28[1] = fRec28[0];
			fRec21[1] = fRec21[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec17[1] = fRec17[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec13[1] = fRec13[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec9[1] = fRec9[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec0[1] = fRec0[0];
			IOTA = IOTA+1;
		}
	}
};


// clang-format on
#endif

// clang-format off
#define faust_setup(name) xfaust_setup(name)
#define xfaust_setup(name) name##_tilde_setup(void)
// time for "active" toggle xfades in secs
#define XFADE_TIME 0.1f
static t_class* zita_rev1_faust_class;
#define FAUST_EXT t_faust_zita_rev1
#define FAUST_EXT_CLASS zita_rev1_faust_class
// clang-format on

struct t_faust_zita_rev1 {
    t_object x_obj;
#ifdef __MINGW32__
    /* This seems to be necessary as some as yet undetermined Pd routine seems
     to write past the end of x_obj on Windows. */
    int fence; /* dummy field (not used) */
#endif
    zita_rev1* dsp;
    PdUI<UI>* ui;
    int active, xfade, n_xfade, rate, n_in, n_out;
    t_sample **inputs, **outputs, **buf;
    t_outlet* out;
    t_sample f;
};

static inline void zero_samples(int k, int n, t_sample** out)
{
    for (int i = 0; i < k; i++)
#ifdef __STDC_IEC_559__
        /* IEC 559 a.k.a. IEEE 754 floats can be initialized faster like this */
        memset(out[i], 0, n * sizeof(t_sample));
#else
        for (int j = 0; j < n; j++)
            out[i][j] = 0.0f;
#endif
}

static inline void copy_samples(int k, int n, t_sample** out, t_sample** in)
{
    for (int i = 0; i < k; i++)
        memcpy(out[i], in[i], n * sizeof(t_sample));
}

static t_int* faust_perform(t_int* w)
{
    t_faust_zita_rev1* x = reinterpret_cast<t_faust_zita_rev1*>(w[1]);
    int n = static_cast<int>(w[2]);
    if (!x->dsp || !x->buf)
        return (w + 3);

    AVOIDDENORMALS;
    if (x->xfade > 0) {
        float d = 1.0f / x->n_xfade, f = (x->xfade--) * d;
        d = d / n;
        x->dsp->compute(n, x->inputs, x->buf);
        if (x->active) {
            if (x->n_in == x->n_out) {
                /* xfade inputs -> buf */
                for (int j = 0; j < n; j++, f -= d) {
                    for (int i = 0; i < x->n_out; i++)
                        x->outputs[i][j] = f * x->inputs[i][j] + (1.0f - f) * x->buf[i][j];
                }
            } else {
                /* xfade 0 -> buf */
                for (int j = 0; j < n; j++, f -= d) {
                    for (int i = 0; i < x->n_out; i++)
                        x->outputs[i][j] = (1.0f - f) * x->buf[i][j];
                }
            }
        } else if (x->n_in == x->n_out) {
            /* xfade buf -> inputs */
            for (int j = 0; j < n; j++, f -= d) {
                for (int i = 0; i < x->n_out; i++)
                    x->outputs[i][j] = f * x->buf[i][j] + (1.0f - f) * x->inputs[i][j];
            }
        } else {
            /* xfade buf -> 0 */
            for (int j = 0; j < n; j++, f -= d) {
                for (int i = 0; i < x->n_out; i++)
                    x->outputs[i][j] = f * x->buf[i][j];
            }
        }
    } else if (x->active) {
        x->dsp->compute(n, x->inputs, x->buf);
        copy_samples(x->n_out, n, x->outputs, x->buf);
    } else if (x->n_in == x->n_out) {
        copy_samples(x->n_out, n, x->buf, x->inputs);
        copy_samples(x->n_out, n, x->outputs, x->buf);
    } else
        zero_samples(x->n_out, n, x->outputs);

    return (w + 3);
}

static void zita_rev1_faust_dsp(t_faust_zita_rev1* x, t_signal** sp)
{
    const int n = sp[0]->s_n;
    const int sr = static_cast<int>(sp[0]->s_sr);

    if (x->rate <= 0) {
        /* default sample rate is whatever Pd tells us */
        PdUI<UI>* ui = x->ui;
        std::vector<FAUSTFLOAT> z = ui->uiValues();
        /* set the proper sample rate; this requires reinitializing the dsp */
        x->rate = sr;
        x->dsp->init(sr);
        ui->setUIValues(z);
    }
    if (n > 0)
        x->n_xfade = static_cast<int>(x->rate * XFADE_TIME / n);

    dsp_add(faust_perform, 2, x, n);

    for (int i = 0; i < x->n_in; i++)
        x->inputs[i] = sp[i]->s_vec;

    for (int i = 0; i < x->n_out; i++)
        x->outputs[i] = sp[x->n_in + i]->s_vec;

    if (x->buf != NULL) {
        for (int i = 0; i < x->n_out; i++) {
            x->buf[i] = static_cast<t_sample*>(malloc(n * sizeof(t_sample)));
            if (x->buf[i] == NULL) {
                for (int j = 0; j < i; j++)
                    free(x->buf[j]);
                free(x->buf);
                x->buf = NULL;
                break;
            }
        }
    }
}

static void zita_rev1_dump_to_console(t_faust_zita_rev1* x)
{
    t_object* xobj = &x->x_obj;
    t_class* xc = xobj->te_pd;
    const char* name = class_getname(xc);

    // print xlets
    post("[%s] inlets: %i", name, x->dsp->getNumInputs());
    int info_outlet = (x->out == 0) ? 0 : 1;
    post("[%s] outlets: %i", name, x->dsp->getNumOutputs() + info_outlet);

    // print properties
    for (size_t i = 0; i < x->ui->uiCount(); i++) {
        UIElement* el = x->ui->uiAt(i);
        post("[%s] property: %s = %g", name, el->setPropertySym()->s_name, static_cast<double>(el->value()));
    }
}

static void zita_rev1_faust_any(t_faust_zita_rev1* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!x->dsp)
        return;

    PdUI<UI>* ui = x->ui;
    if (s == &s_bang) {
        ui->dumpUI(x->out);
    } else if (isGetAllProperties(s)) {
        ui->outputAllProperties(x->out);
    } else if (isGetProperty(s)) {
        ui->outputProperty(s, x->out);
    } else if (isSetProperty(s)) {
        ui->setProperty(s, argc, argv);
    } else {
        const char* label = s->s_name;
        int count = 0;
        for (size_t i = 0; i < ui->uiCount(); i++) {
            if (ui->uiAt(i)->pathcmp(label)) {
                if (argc == 0) {
                    ui->uiAt(i)->outputValue(x->out);
                    ++count;
                } else if (argc == 1 && (argv[0].a_type == A_FLOAT || argv[0].a_type == A_DEFFLOAT)) {
                    float f = atom_getfloat(argv);
                    UIElement* el = ui->uiAt(i);
                    el->setValue(f);
                    ++count;
                } else
                    pd_error(x, "[ceammc] %s: bad control argument: %s", ui->fullName().c_str(), label);
            }
        }

        if (count == 0 && strcmp(label, "active") == 0) {
            if (argc == 0) {
                t_atom arg;
                SETFLOAT(&arg, (float)x->active);
                if (x->out) {
                    outlet_anything(x->out, gensym("active"), 1, &arg);
                }
            } else if (argc == 1 && (argv[0].a_type == A_FLOAT || argv[0].a_type == A_DEFFLOAT)) {
                float f = atom_getfloat(argv);
                x->active = (int)f;
                x->xfade = x->n_xfade;
            }
        }
    }
}

static void faust_free_dsp(t_faust_zita_rev1* x)
{
    delete x->dsp;
    x->dsp = NULL;
}

static void faust_free_ui(t_faust_zita_rev1* x)
{
    delete x->ui;
    x->ui = NULL;
}

static void faust_free_inputs(t_faust_zita_rev1* x)
{
    if (x->inputs)
        free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust_zita_rev1* x)
{
    if (x->outputs)
        free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust_zita_rev1* x)
{
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i])
                free(x->buf[i]);
        }

        free(x->buf);
    }
}

static void zita_rev1_faust_free(t_faust_zita_rev1* x)
{
    faust_free_dsp(x);
    faust_free_ui(x);
    faust_free_inputs(x);
    faust_free_outputs(x);
    faust_free_buf(x);
}

static bool faust_init_inputs(t_faust_zita_rev1* x)
{
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if (x->inputs == NULL) {
            error("[%s] faust_init_inputs failed", sym(zita_rev1));
            return false;
        }
    }

    // creating sound inlets (except the first one!)
    for (int i = 0; i < (x->n_in - 1); i++) {
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    return true;
}

static bool faust_init_outputs(t_faust_zita_rev1* x, bool info_outlet)
{
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->outputs == NULL) {
            error("[%s] faust_init_outputs failed", sym(zita_rev1));
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->buf == NULL) {
            error("[%s] faust_init_outputs failed", sym(zita_rev1));
            faust_free_outputs(x);
            return false;
        }

        for (int i = 0; i < x->n_out; i++)
            x->buf[i] = NULL;
    }

    // creating sound outlets
    for (int i = 0; i < x->n_out; i++) {
        outlet_new(&x->x_obj, &s_signal);
    }

    // control outlet
    if (info_outlet)
        x->out = outlet_new(&x->x_obj, 0);
    else
        x->out = 0;

    return true;
}

static bool faust_new_internal(t_faust_zita_rev1* x, const std::string& objId = "", bool info_outlet = true)
{
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new zita_rev1();
    x->ui = new PdUI<UI>(sym(zita_rev1), objId);

    if (!faust_init_inputs(x)) {
        zita_rev1_faust_free(x);
        return false;
    }

    if (!faust_init_outputs(x, info_outlet)) {
        zita_rev1_faust_free(x);
        return false;
    }

    x->dsp->init(sr);
    x->dsp->buildUserInterface(x->ui);

    return true;
}

/**
 * find nth element that satisfies given predicate
 * @first - first element of sequence
 * @last - pointer behind last element of sequence
 * @Nth - searched element index
 * @pred - predicate
 * @return pointer to found element or pointer to @bold last, if not found
 */
template <class InputIterator, class NthOccurence, class UnaryPredicate>
InputIterator find_nth_if(InputIterator first, InputIterator last, NthOccurence Nth, UnaryPredicate pred)
{
    if (Nth > 0) {
        while (first != last) {
            if (pred(*first))
                if (!--Nth)
                    return first;
            ++first;
        }
    }
    return last;
}

/**
 * @return true if given atom is a float
 */
static bool atom_is_float(const t_atom& a)
{
    switch (a.a_type) {
    case A_FLOAT:
    case A_DEFFLOAT:
        return true;
    default:
        return false;
    }
}

/**
 * @return true if given atom is a symbol
 */
static bool atom_is_symbol(const t_atom& a)
{
    switch (a.a_type) {
    case A_DEFSYMBOL:
    case A_SYMBOL:
        return true;
    default:
        return false;
    }
}

/**
 * @brief find nth float in argument list. (arguments can be mixed)
 * @param argc argument count
 * @param argv pointer to argument vector
 * @param nth find position. nth should be > 0!
 * @param dest destination to write value
 * @return true if argument at given position was found, otherwise false
 */
static bool get_nth_float_arg(int argc, t_atom* argv, int nth, t_float* dest)
{
    t_atom* last = argv + argc;
    t_atom* res = find_nth_if(argv, last, nth, atom_is_float);
    if (last == res)
        return false;

    *dest = atom_getfloat(res);
    return true;
}

/**
 * @brief find nth symbol in argument list. (arguments can be mixed)
 * @param argc argument count
 * @param argv pointer to argument vector
 * @param nth find position. nth should be > 0!
 * @param dest destination to write found argument value
 * @return true if argument at given position was found, otherwise false
 */
static bool get_nth_symbol_arg(int argc, t_atom* argv, int nth, const char** dest)
{
    t_atom* last = argv + argc;
    t_atom* res = find_nth_if(argv, last, nth, atom_is_symbol);
    if (last == res)
        return false;

    t_symbol* s = atom_getsymbol(res);
    *dest = s->s_name;
    return true;
}

class PdArgParser {
    t_faust_zita_rev1* x_;
    int argc_;
    t_atom* argv_;
    bool control_outlet_;

public:
    /**
     * @brief FaustArgParser
     * @param x pointer to faust class
     * @param argc arguments count
     * @param argv pointer to argument vector
     */
    PdArgParser(t_faust_zita_rev1* x, int argc, t_atom* argv, bool info_outlet = true)
        : x_(x)
        , argc_(argc)
        , argv_(argv)
        , control_outlet_(info_outlet)
    {
        const char* id = NULL;
        std::string objId;
        if (get_nth_symbol_arg(argc_, argv_, 1, &id))
            objId = id;

        // init error
        if (!faust_new_internal(x, objId, control_outlet_)) {
            this->x_ = NULL;
        }
    }

    /**
     * @brief initFloatArg
     * @param name argument name
     * @param pos argument position among of @bold float(!) arguments. Position starts from @bold 1(!).
     * to select first argument - pass 1.
     */
    void initFloatArg(const char* name, int pos)
    {
        // object was not created
        if (!this->x_)
            return;

        t_float v = 0.0;
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &v)) {
            UIElement* el = this->x_->ui->findElementByLabel(name);
            if (!el) {
                post("invalid UI element name: %s", name);
                return;
            }

            el->setValue(v, true);
        }
    }

    /**
     * @brief send creation argument to first signal inlet
     * @param name argument name
     * @param pos argument position among of @bold float(!) arguments. Position starts from @bold 1(!).
     * to select first argument - pass 1.
     */
    void signalFloatArg(const char* name, int pos)
    {
        // object was not created
        if (!this->x_)
            return;

        t_float arg = 0;
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &arg))
            pd_float(reinterpret_cast<t_pd*>(this->x_), arg);
    }

    t_faust_zita_rev1* pd_obj()
    {
        return this->x_;
    }
};

static void* zita_rev1_faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s)
{
    zita_rev1_faust_class = class_new(s, reinterpret_cast<t_newmethod>(zita_rev1_faust_new),
        reinterpret_cast<t_method>(zita_rev1_faust_free),
        sizeof(t_faust_zita_rev1),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);
    class_addmethod(zita_rev1_faust_class, nullfn, &s_signal, A_NULL);
    class_addmethod(zita_rev1_faust_class, reinterpret_cast<t_method>(zita_rev1_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(zita_rev1_faust_class, reinterpret_cast<t_method>(zita_rev1_dump_to_console), gensym("dump"), A_NULL);
    CLASS_MAINSIGNALIN(zita_rev1_faust_class, t_faust_zita_rev1, f);
    class_addanything(zita_rev1_faust_class, zita_rev1_faust_any);
}

#define EXTERNAL_NEW void* zita_rev1_faust_new(t_symbol*, int argc, t_atom* argv)

#define EXTERNAL_SIMPLE_NEW()                                                           \
    static void* zita_rev1_faust_new(t_symbol*, int argc, t_atom* argv)                     \
    {                                                                                   \
        t_faust_zita_rev1* x = reinterpret_cast<t_faust_zita_rev1*>(pd_new(zita_rev1_faust_class)); \
        PdArgParser p(x, argc, argv, false);                                            \
        return p.pd_obj();                                                              \
    }

#define EXTERNAL_SETUP(MOD)                        \
    extern "C" void setup_##MOD##0x2ezita_rev1_tilde() \
    {                                              \
        internal_setup(gensym(#MOD ".zita_rev1~"));    \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);
