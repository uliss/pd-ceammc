//----------------------------------------------------------
// name: "GreyholeRaw"
// version: "1.0"
// author: "Julian Parker, bug fixes by Till Bovermann"
// license: "GPL2+"
// copyright: "(c) Julian Parker 2013"
//
// Code generated with Faust 0.10.6 (http://faust.grame.fr)
//----------------------------------------------------------

/* link with : "primes" */
#include "jprev.h"
#include <math.h>
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
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

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
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /** 
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void init(int samplingRate) = 0;
    
        /** 
         * Init instance state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceInit(int samplingRate) = 0;
    
        /** 
         * Init instance constant state
         *
         * @param samplingRate - the sampling rate in Hertz
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
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
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
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
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
struct greyhole : public dsp {
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
#define FAUSTCLASS greyhole
#endif

class greyhole : public dsp {
  private:
	FAUSTFLOAT 	fslider0;
	float 	fVec0[2];
	int 	iVec1[2];
	FAUSTFLOAT 	fslider1;
	float 	fVec2[2];
	FAUSTFLOAT 	fslider2;
	float 	fVec3[2];
	float 	fConst0;
	FAUSTFLOAT 	fslider3;
	float 	fRec39[2];
	float 	fRec40[2];
	float 	fRec41[2];
	float 	fRec42[2];
	float 	fConst1;
	FAUSTFLOAT 	fslider4;
	float 	fVec4[2];
	float 	fConst2;
	FAUSTFLOAT 	fslider5;
	float 	fVec5[2];
	float 	fRec43[2];
	float 	fRec44[2];
	int 	IOTA;
	float 	fVec6[131072];
	float 	fVec7[16384];
	FAUSTFLOAT 	fslider6;
	float 	fRec45[2];
	float 	fRec38[2];
	float 	fRec36[2];
	float 	fVec8[131072];
	float 	fVec9[16384];
	float 	fRec47[2];
	float 	fRec46[2];
	float 	fRec37[2];
	float 	fVec10[16384];
	float 	fRec48[2];
	float 	fRec35[2];
	float 	fRec33[2];
	float 	fVec11[16384];
	float 	fRec50[2];
	float 	fRec49[2];
	float 	fRec34[2];
	float 	fVec12[16384];
	float 	fRec51[2];
	float 	fRec32[2];
	float 	fRec30[2];
	float 	fVec13[16384];
	float 	fRec53[2];
	float 	fRec52[2];
	float 	fRec31[2];
	float 	fVec14[16384];
	float 	fRec54[2];
	float 	fRec29[2];
	float 	fRec27[2];
	float 	fVec15[16384];
	float 	fRec56[2];
	float 	fRec55[2];
	float 	fRec28[2];
	float 	fVec16[16384];
	float 	fRec57[2];
	float 	fRec26[2];
	float 	fRec24[2];
	float 	fVec17[16384];
	float 	fRec59[2];
	float 	fRec58[2];
	float 	fRec25[2];
	float 	fVec18[16384];
	float 	fRec60[2];
	float 	fRec23[2];
	float 	fRec21[2];
	float 	fVec19[16384];
	float 	fRec62[2];
	float 	fRec61[2];
	float 	fRec22[2];
	float 	fVec20[16384];
	float 	fRec63[2];
	float 	fRec20[2];
	float 	fRec18[2];
	float 	fVec21[16384];
	float 	fRec65[2];
	float 	fRec64[2];
	float 	fRec19[2];
	float 	fVec22[16384];
	float 	fRec66[2];
	float 	fRec17[2];
	float 	fRec15[2];
	float 	fVec23[16384];
	float 	fRec68[2];
	float 	fRec67[2];
	float 	fRec16[2];
	float 	fVec24[16384];
	float 	fRec69[2];
	float 	fRec14[2];
	float 	fRec12[2];
	float 	fVec25[16384];
	float 	fRec71[2];
	float 	fRec70[2];
	float 	fRec13[2];
	float 	fVec26[16384];
	float 	fRec72[2];
	float 	fRec11[2];
	float 	fRec9[2];
	float 	fVec27[16384];
	float 	fRec74[2];
	float 	fRec73[2];
	float 	fRec10[2];
	float 	fVec28[16384];
	float 	fRec75[2];
	float 	fRec8[2];
	float 	fRec6[2];
	float 	fVec29[16384];
	float 	fRec77[2];
	float 	fRec76[2];
	float 	fRec7[2];
	float 	fVec30[16384];
	float 	fRec78[2];
	float 	fRec5[2];
	float 	fRec3[2];
	float 	fVec31[16384];
	float 	fRec80[2];
	float 	fRec79[2];
	float 	fRec4[2];
	float 	fRec2[2];
	float 	fRec0[1024];
	float 	fRec81[2];
	float 	fRec1[1024];
	int fSamplingFreq;

  public:
	virtual void metadata(Meta* m) { 
		m->declare("name", "GreyholeRaw");
		m->declare("version", "1.0");
		m->declare("author", "Julian Parker, bug fixes by Till Bovermann");
		m->declare("license", "GPL2+");
		m->declare("copyright", "(c) Julian Parker 2013");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.0");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
	}

	virtual int getNumInputs() { return 2; }
	virtual int getNumOutputs() { return 2; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = min(1.92e+05f, max(1e+03f, (float)fSamplingFreq));
		fConst1 = (0.00056689343f * fConst0);
		fConst2 = (3.1415927f / fConst0);
	}
	virtual void instanceResetUserInterface() {
		fslider0 = 0.0f;
		fslider1 = 0.5f;
		fslider2 = 0.9f;
		fslider3 = 0.2f;
		fslider4 = 0.1f;
		fslider5 = 2.0f;
		fslider6 = 1.0f;
	}
	virtual void instanceClear() {
		for (int i=0; i<2; i++) fVec0[i] = 0;
		for (int i=0; i<2; i++) iVec1[i] = 0;
		for (int i=0; i<2; i++) fVec2[i] = 0;
		for (int i=0; i<2; i++) fVec3[i] = 0;
		for (int i=0; i<2; i++) fRec39[i] = 0;
		for (int i=0; i<2; i++) fRec40[i] = 0;
		for (int i=0; i<2; i++) fRec41[i] = 0;
		for (int i=0; i<2; i++) fRec42[i] = 0;
		for (int i=0; i<2; i++) fVec4[i] = 0;
		for (int i=0; i<2; i++) fVec5[i] = 0;
		for (int i=0; i<2; i++) fRec43[i] = 0;
		for (int i=0; i<2; i++) fRec44[i] = 0;
		IOTA = 0;
		for (int i=0; i<131072; i++) fVec6[i] = 0;
		for (int i=0; i<16384; i++) fVec7[i] = 0;
		for (int i=0; i<2; i++) fRec45[i] = 0;
		for (int i=0; i<2; i++) fRec38[i] = 0;
		for (int i=0; i<2; i++) fRec36[i] = 0;
		for (int i=0; i<131072; i++) fVec8[i] = 0;
		for (int i=0; i<16384; i++) fVec9[i] = 0;
		for (int i=0; i<2; i++) fRec47[i] = 0;
		for (int i=0; i<2; i++) fRec46[i] = 0;
		for (int i=0; i<2; i++) fRec37[i] = 0;
		for (int i=0; i<16384; i++) fVec10[i] = 0;
		for (int i=0; i<2; i++) fRec48[i] = 0;
		for (int i=0; i<2; i++) fRec35[i] = 0;
		for (int i=0; i<2; i++) fRec33[i] = 0;
		for (int i=0; i<16384; i++) fVec11[i] = 0;
		for (int i=0; i<2; i++) fRec50[i] = 0;
		for (int i=0; i<2; i++) fRec49[i] = 0;
		for (int i=0; i<2; i++) fRec34[i] = 0;
		for (int i=0; i<16384; i++) fVec12[i] = 0;
		for (int i=0; i<2; i++) fRec51[i] = 0;
		for (int i=0; i<2; i++) fRec32[i] = 0;
		for (int i=0; i<2; i++) fRec30[i] = 0;
		for (int i=0; i<16384; i++) fVec13[i] = 0;
		for (int i=0; i<2; i++) fRec53[i] = 0;
		for (int i=0; i<2; i++) fRec52[i] = 0;
		for (int i=0; i<2; i++) fRec31[i] = 0;
		for (int i=0; i<16384; i++) fVec14[i] = 0;
		for (int i=0; i<2; i++) fRec54[i] = 0;
		for (int i=0; i<2; i++) fRec29[i] = 0;
		for (int i=0; i<2; i++) fRec27[i] = 0;
		for (int i=0; i<16384; i++) fVec15[i] = 0;
		for (int i=0; i<2; i++) fRec56[i] = 0;
		for (int i=0; i<2; i++) fRec55[i] = 0;
		for (int i=0; i<2; i++) fRec28[i] = 0;
		for (int i=0; i<16384; i++) fVec16[i] = 0;
		for (int i=0; i<2; i++) fRec57[i] = 0;
		for (int i=0; i<2; i++) fRec26[i] = 0;
		for (int i=0; i<2; i++) fRec24[i] = 0;
		for (int i=0; i<16384; i++) fVec17[i] = 0;
		for (int i=0; i<2; i++) fRec59[i] = 0;
		for (int i=0; i<2; i++) fRec58[i] = 0;
		for (int i=0; i<2; i++) fRec25[i] = 0;
		for (int i=0; i<16384; i++) fVec18[i] = 0;
		for (int i=0; i<2; i++) fRec60[i] = 0;
		for (int i=0; i<2; i++) fRec23[i] = 0;
		for (int i=0; i<2; i++) fRec21[i] = 0;
		for (int i=0; i<16384; i++) fVec19[i] = 0;
		for (int i=0; i<2; i++) fRec62[i] = 0;
		for (int i=0; i<2; i++) fRec61[i] = 0;
		for (int i=0; i<2; i++) fRec22[i] = 0;
		for (int i=0; i<16384; i++) fVec20[i] = 0;
		for (int i=0; i<2; i++) fRec63[i] = 0;
		for (int i=0; i<2; i++) fRec20[i] = 0;
		for (int i=0; i<2; i++) fRec18[i] = 0;
		for (int i=0; i<16384; i++) fVec21[i] = 0;
		for (int i=0; i<2; i++) fRec65[i] = 0;
		for (int i=0; i<2; i++) fRec64[i] = 0;
		for (int i=0; i<2; i++) fRec19[i] = 0;
		for (int i=0; i<16384; i++) fVec22[i] = 0;
		for (int i=0; i<2; i++) fRec66[i] = 0;
		for (int i=0; i<2; i++) fRec17[i] = 0;
		for (int i=0; i<2; i++) fRec15[i] = 0;
		for (int i=0; i<16384; i++) fVec23[i] = 0;
		for (int i=0; i<2; i++) fRec68[i] = 0;
		for (int i=0; i<2; i++) fRec67[i] = 0;
		for (int i=0; i<2; i++) fRec16[i] = 0;
		for (int i=0; i<16384; i++) fVec24[i] = 0;
		for (int i=0; i<2; i++) fRec69[i] = 0;
		for (int i=0; i<2; i++) fRec14[i] = 0;
		for (int i=0; i<2; i++) fRec12[i] = 0;
		for (int i=0; i<16384; i++) fVec25[i] = 0;
		for (int i=0; i<2; i++) fRec71[i] = 0;
		for (int i=0; i<2; i++) fRec70[i] = 0;
		for (int i=0; i<2; i++) fRec13[i] = 0;
		for (int i=0; i<16384; i++) fVec26[i] = 0;
		for (int i=0; i<2; i++) fRec72[i] = 0;
		for (int i=0; i<2; i++) fRec11[i] = 0;
		for (int i=0; i<2; i++) fRec9[i] = 0;
		for (int i=0; i<16384; i++) fVec27[i] = 0;
		for (int i=0; i<2; i++) fRec74[i] = 0;
		for (int i=0; i<2; i++) fRec73[i] = 0;
		for (int i=0; i<2; i++) fRec10[i] = 0;
		for (int i=0; i<16384; i++) fVec28[i] = 0;
		for (int i=0; i<2; i++) fRec75[i] = 0;
		for (int i=0; i<2; i++) fRec8[i] = 0;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		for (int i=0; i<16384; i++) fVec29[i] = 0;
		for (int i=0; i<2; i++) fRec77[i] = 0;
		for (int i=0; i<2; i++) fRec76[i] = 0;
		for (int i=0; i<2; i++) fRec7[i] = 0;
		for (int i=0; i<16384; i++) fVec30[i] = 0;
		for (int i=0; i<2; i++) fRec78[i] = 0;
		for (int i=0; i<2; i++) fRec5[i] = 0;
		for (int i=0; i<2; i++) fRec3[i] = 0;
		for (int i=0; i<16384; i++) fVec31[i] = 0;
		for (int i=0; i<2; i++) fRec80[i] = 0;
		for (int i=0; i<2; i++) fRec79[i] = 0;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		for (int i=0; i<1024; i++) fRec0[i] = 0;
		for (int i=0; i<2; i++) fRec81[i] = 0;
		for (int i=0; i<1024; i++) fRec1[i] = 0;
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
	virtual greyhole* clone() {
		return new greyhole();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
	}
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("GreyholeRaw");
		ui_interface->addHorizontalSlider("damping", &fslider0, 0.0f, 0.0f, 0.99f, 0.001f);
		ui_interface->addHorizontalSlider("delaytime", &fslider3, 0.2f, 0.001f, 1.45f, 0.0001f);
		ui_interface->addHorizontalSlider("diffusion", &fslider1, 0.5f, 0.0f, 0.99f, 0.0001f);
		ui_interface->addHorizontalSlider("feedback", &fslider2, 0.9f, 0.0f, 1.0f, 0.01f);
		ui_interface->addHorizontalSlider("moddepth", &fslider4, 0.1f, 0.0f, 1.0f, 0.001f);
		ui_interface->addHorizontalSlider("modfreq", &fslider5, 2.0f, 0.0f, 1e+01f, 0.01f);
		ui_interface->addHorizontalSlider("size", &fslider6, 1.0f, 0.5f, 3.0f, 0.0001f);
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fSlow0 = float(fslider0);
		float 	fSlow1 = float(fslider1);
		float 	fSlow2 = float(fslider2);
		float 	fSlow3 = floorf(min((float)65533, (fConst0 * float(fslider3))));
		float 	fSlow4 = float(fslider4);
		float 	fSlow5 = float(fslider5);
		float 	fSlow6 = float(fslider6);
		float 	fSlow7 = (49 * fSlow6);
		int 	iSlow8 = primes(fSlow7);
		float 	fSlow9 = (0.0001f * iSlow8);
		int 	iSlow10 = primes((fSlow7 + 10));
		float 	fSlow11 = (0.0001f * iSlow10);
		float 	fSlow12 = (36 * fSlow6);
		int 	iSlow13 = primes(fSlow12);
		float 	fSlow14 = (0.001f * iSlow13);
		int 	iSlow15 = primes((fSlow12 + 10));
		float 	fSlow16 = (0.001f * iSlow15);
		float 	fSlow17 = (23 * fSlow6);
		int 	iSlow18 = primes(fSlow17);
		float 	fSlow19 = (0.001f * iSlow18);
		int 	iSlow20 = primes((fSlow17 + 10));
		float 	fSlow21 = (0.001f * iSlow20);
		int 	iSlow22 = primes((10 * fSlow6));
		float 	fSlow23 = (0.001f * iSlow22);
		int 	iSlow24 = primes((10 * (fSlow6 + 1)));
		float 	fSlow25 = (0.001f * iSlow24);
		float 	fSlow26 = (68 * fSlow6);
		int 	iSlow27 = primes(fSlow26);
		float 	fSlow28 = (0.0001f * iSlow27);
		int 	iSlow29 = primes((fSlow26 + 10));
		float 	fSlow30 = (0.0001f * iSlow29);
		float 	fSlow31 = (55 * fSlow6);
		int 	iSlow32 = primes(fSlow31);
		float 	fSlow33 = (0.001f * iSlow32);
		int 	iSlow34 = primes((fSlow31 + 10));
		float 	fSlow35 = (0.001f * iSlow34);
		float 	fSlow36 = (42 * fSlow6);
		int 	iSlow37 = primes(fSlow36);
		float 	fSlow38 = (0.001f * iSlow37);
		int 	iSlow39 = primes((fSlow36 + 10));
		float 	fSlow40 = (0.001f * iSlow39);
		float 	fSlow41 = (29 * fSlow6);
		int 	iSlow42 = primes(fSlow41);
		float 	fSlow43 = (0.001f * iSlow42);
		int 	iSlow44 = primes((fSlow41 + 10));
		float 	fSlow45 = (0.001f * iSlow44);
		float 	fSlow46 = (87 * fSlow6);
		int 	iSlow47 = primes(fSlow46);
		float 	fSlow48 = (0.0001f * iSlow47);
		int 	iSlow49 = primes((fSlow46 + 10));
		float 	fSlow50 = (0.0001f * iSlow49);
		float 	fSlow51 = (74 * fSlow6);
		int 	iSlow52 = primes(fSlow51);
		float 	fSlow53 = (0.001f * iSlow52);
		int 	iSlow54 = primes((fSlow51 + 10));
		float 	fSlow55 = (0.001f * iSlow54);
		float 	fSlow56 = (61 * fSlow6);
		int 	iSlow57 = primes(fSlow56);
		float 	fSlow58 = (0.001f * iSlow57);
		int 	iSlow59 = primes((fSlow56 + 10));
		float 	fSlow60 = (0.001f * iSlow59);
		float 	fSlow61 = (48 * fSlow6);
		int 	iSlow62 = primes(fSlow61);
		float 	fSlow63 = (0.001f * iSlow62);
		int 	iSlow64 = primes((fSlow61 + 10));
		float 	fSlow65 = (0.001f * iSlow64);
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* input1 = input[1];
		FAUSTFLOAT* output0 = output[0];
		FAUSTFLOAT* output1 = output[1];
		for (int i=0; i<count; i++) {
			fVec0[0] = fSlow0;
			iVec1[0] = 1;
			float fTemp0 = (fSlow0 + fVec0[1]);
			fVec2[0] = fSlow1;
			float fTemp1 = (0.5f * (fSlow1 + fVec2[1]));
			float fTemp2 = cosf(fTemp1);
			float fTemp3 = sinf(fTemp1);
			float fTemp4 = (0 - fTemp3);
			float fTemp5 = (0 - fTemp1);
			float fTemp6 = cosf(fTemp5);
			float fTemp7 = sinf(fTemp5);
			float fTemp8 = (0 - fTemp7);
			fVec3[0] = fSlow2;
			float fTemp9 = (fSlow2 + fVec3[1]);
			float fTemp10 = ((int((fRec39[1] != 0.0f)))?((int(((fRec40[1] > 0.0f) & (fRec40[1] < 1.0f))))?fRec39[1]:0):((int(((fRec40[1] == 0.0f) & (fSlow3 != fRec41[1]))))?4.5351473e-05f:((int(((fRec40[1] == 1.0f) & (fSlow3 != fRec42[1]))))?-4.5351473e-05f:0)));
			fRec39[0] = fTemp10;
			fRec40[0] = max(0.0f, min(1.0f, (fRec40[1] + fTemp10)));
			fRec41[0] = ((int(((fRec40[1] >= 1.0f) & (fRec42[1] != fSlow3))))?fSlow3:fRec41[1]);
			fRec42[0] = ((int(((fRec40[1] <= 0.0f) & (fRec41[1] != fSlow3))))?fSlow3:fRec42[1]);
			fVec4[0] = fSlow4;
			float fTemp11 = (fSlow4 + fVec4[1]);
			fVec5[0] = fSlow5;
			float fTemp12 = (fConst2 * (fSlow5 + fVec5[1]));
			float fTemp13 = sinf(fTemp12);
			float fTemp14 = cosf(fTemp12);
			fRec43[0] = ((fTemp13 * fRec44[1]) + (fTemp14 * fRec43[1]));
			int iTemp15 = (1 - iVec1[1]);
			fRec44[0] = (((fTemp14 * fRec44[1]) + ((0 - fTemp13) * fRec43[1])) + iTemp15);
			float fTemp16 = (fConst1 * (fTemp11 * (fRec43[0] + 1.0f)));
			float fTemp17 = (fTemp16 + 8.500005f);
			int iTemp18 = int(fTemp17);
			float fTemp19 = floorf(fTemp17);
			float fTemp20 = (fTemp16 + (9.0f - fTemp19));
			float fTemp21 = (fTemp16 + (8.0f - fTemp19));
			float fTemp22 = (fTemp16 + (7.0f - fTemp19));
			float fTemp23 = (fTemp16 + (6.0f - fTemp19));
			float fTemp24 = (fTemp20 * fTemp21);
			float fTemp25 = (((((fRec1[(IOTA-int((int(min(512, max(0, iTemp18))) + 1)))&1023] * (0 - fTemp20)) * (0 - (0.5f * fTemp21))) * (0 - (0.33333334f * fTemp22))) * (0 - (0.25f * fTemp23))) + ((fTemp16 + (1e+01f - fTemp19)) * ((((((fRec1[(IOTA-int((int(min(512, max(0, (iTemp18 + 1)))) + 1)))&1023] * (0 - fTemp21)) * (0 - (0.5f * fTemp22))) * (0 - (0.33333334f * fTemp23))) + (0.5f * (((fTemp20 * fRec1[(IOTA-int((int(min(512, max(0, (iTemp18 + 2)))) + 1)))&1023]) * (0 - fTemp22)) * (0 - (0.5f * fTemp23))))) + (0.16666667f * ((fTemp24 * fRec1[(IOTA-int((int(min(512, max(0, (iTemp18 + 3)))) + 1)))&1023]) * (0 - fTemp23)))) + (0.041666668f * ((fTemp24 * fTemp22) * fRec1[(IOTA-int((int(min(512, max(0, (iTemp18 + 4)))) + 1)))&1023])))));
			fVec6[IOTA&131071] = fTemp25;
			int iTemp26 = int(min((float)65536, max((float)0, fRec42[0])));
			float fTemp27 = (1.0f - fRec40[0]);
			int iTemp28 = int(min((float)65536, max((float)0, fRec41[0])));
			float fTemp29 = ((float)input1[i] + (0.5f * (fTemp9 * ((fRec40[0] * fVec6[(IOTA-iTemp26)&131071]) + (fTemp27 * fVec6[(IOTA-iTemp28)&131071])))));
			float fTemp30 = ((fTemp4 * fRec28[1]) + (fTemp2 * fTemp29));
			float fTemp31 = ((fTemp4 * fRec31[1]) + (fTemp2 * fTemp30));
			float fTemp32 = ((fTemp4 * fRec34[1]) + (fTemp2 * fTemp31));
			float fTemp33 = (0 - ((fTemp4 * fRec37[1]) + (fTemp2 * fTemp32)));
			fVec7[IOTA&16383] = fTemp33;
			fRec45[0] = (fSlow9 + (0.9999f * ((iSlow8 * iTemp15) + fRec45[1])));
			float fTemp34 = (fRec45[0] + -1.49999f);
			int iTemp35 = int(min(8192, max(0, int(fTemp34))));
			float fTemp36 = floorf(fTemp34);
			float fTemp37 = (fTemp36 + (2.0f - fRec45[0]));
			float fTemp38 = (fRec45[0] - fTemp36);
			fRec38[0] = (fVec7[(IOTA-int((iTemp35 + 1)))&16383] + ((fRec38[1] * (0 - (fTemp37 / fTemp38))) + ((fTemp37 * fVec7[(IOTA-iTemp35)&16383]) / fTemp38)));
			fRec36[0] = fRec38[0];
			float fTemp39 = (fConst1 * (fTemp11 * (fRec44[0] + 1.0f)));
			float fTemp40 = (fTemp39 + 8.500005f);
			int iTemp41 = int(fTemp40);
			float fTemp42 = floorf(fTemp40);
			float fTemp43 = (fTemp39 + (9.0f - fTemp42));
			float fTemp44 = (fTemp39 + (8.0f - fTemp42));
			float fTemp45 = (fTemp39 + (7.0f - fTemp42));
			float fTemp46 = (fTemp39 + (6.0f - fTemp42));
			float fTemp47 = (fTemp43 * fTemp44);
			float fTemp48 = (((((fRec0[(IOTA-int((int(min(512, max(0, iTemp41))) + 1)))&1023] * (0 - fTemp43)) * (0 - (0.5f * fTemp44))) * (0 - (0.33333334f * fTemp45))) * (0 - (0.25f * fTemp46))) + ((fTemp39 + (1e+01f - fTemp42)) * ((((((fRec0[(IOTA-int((int(min(512, max(0, (iTemp41 + 1)))) + 1)))&1023] * (0 - fTemp44)) * (0 - (0.5f * fTemp45))) * (0 - (0.33333334f * fTemp46))) + (0.5f * (((fTemp43 * fRec0[(IOTA-int((int(min(512, max(0, (iTemp41 + 2)))) + 1)))&1023]) * (0 - fTemp45)) * (0 - (0.5f * fTemp46))))) + (0.16666667f * ((fTemp47 * fRec0[(IOTA-int((int(min(512, max(0, (iTemp41 + 3)))) + 1)))&1023]) * (0 - fTemp46)))) + (0.041666668f * ((fTemp47 * fTemp45) * fRec0[(IOTA-int((int(min(512, max(0, (iTemp41 + 4)))) + 1)))&1023])))));
			fVec8[IOTA&131071] = fTemp48;
			float fTemp49 = ((float)input0[i] + (0.5f * (((fTemp27 * fVec8[(IOTA-iTemp28)&131071]) + (fRec40[0] * fVec8[(IOTA-iTemp26)&131071])) * fTemp9)));
			float fTemp50 = ((fTemp4 * fRec27[1]) + (fTemp2 * fTemp49));
			float fTemp51 = ((fTemp4 * fRec30[1]) + (fTemp2 * fTemp50));
			float fTemp52 = ((fTemp4 * fRec33[1]) + (fTemp2 * fTemp51));
			float fTemp53 = ((fTemp4 * fRec36[1]) + (fTemp2 * fTemp52));
			fVec9[IOTA&16383] = fTemp53;
			fRec47[0] = (fSlow11 + (0.9999f * ((iSlow10 * iTemp15) + fRec47[1])));
			float fTemp54 = (fRec47[0] + -1.49999f);
			int iTemp55 = int(min(8192, max(0, int(fTemp54))));
			float fTemp56 = floorf(fTemp54);
			float fTemp57 = (fTemp56 + (2.0f - fRec47[0]));
			float fTemp58 = (fRec47[0] - fTemp56);
			fRec46[0] = (fVec9[(IOTA-int((iTemp55 + 1)))&16383] + ((fRec46[1] * (0 - (fTemp57 / fTemp58))) + ((fTemp57 * fVec9[(IOTA-iTemp55)&16383]) / fTemp58)));
			fRec37[0] = fRec46[0];
			float fTemp59 = (0 - ((fTemp2 * fRec37[1]) + (fTemp3 * fTemp32)));
			fVec10[IOTA&16383] = fTemp59;
			fRec48[0] = (fSlow14 + (0.999f * ((iSlow13 * iTemp15) + fRec48[1])));
			float fTemp60 = (fRec48[0] + -1.49999f);
			int iTemp61 = int(min(8192, max(0, int(fTemp60))));
			float fTemp62 = floorf(fTemp60);
			float fTemp63 = (fTemp62 + (2.0f - fRec48[0]));
			float fTemp64 = (fRec48[0] - fTemp62);
			fRec35[0] = (fVec10[(IOTA-int((iTemp61 + 1)))&16383] + ((fRec35[1] * (0 - (fTemp63 / fTemp64))) + ((fTemp63 * fVec10[(IOTA-iTemp61)&16383]) / fTemp64)));
			fRec33[0] = fRec35[0];
			float fTemp65 = ((fRec36[1] * fTemp2) + (fTemp3 * fTemp52));
			fVec11[IOTA&16383] = fTemp65;
			fRec50[0] = (fSlow16 + (0.999f * ((iSlow15 * iTemp15) + fRec50[1])));
			float fTemp66 = (fRec50[0] + -1.49999f);
			int iTemp67 = int(min(8192, max(0, int(fTemp66))));
			float fTemp68 = floorf(fTemp66);
			float fTemp69 = (fTemp68 + (2.0f - fRec50[0]));
			float fTemp70 = (fRec50[0] - fTemp68);
			fRec49[0] = (fVec11[(IOTA-int((iTemp67 + 1)))&16383] + ((fRec49[1] * (0 - (fTemp69 / fTemp70))) + ((fTemp69 * fVec11[(IOTA-iTemp67)&16383]) / fTemp70)));
			fRec34[0] = fRec49[0];
			float fTemp71 = (0 - ((fTemp2 * fRec34[1]) + (fTemp3 * fTemp31)));
			fVec12[IOTA&16383] = fTemp71;
			fRec51[0] = (fSlow19 + (0.999f * ((iSlow18 * iTemp15) + fRec51[1])));
			float fTemp72 = (fRec51[0] + -1.49999f);
			int iTemp73 = int(min(8192, max(0, int(fTemp72))));
			float fTemp74 = floorf(fTemp72);
			float fTemp75 = (fTemp74 + (2.0f - fRec51[0]));
			float fTemp76 = (fRec51[0] - fTemp74);
			fRec32[0] = (fVec12[(IOTA-int((iTemp73 + 1)))&16383] + ((fRec32[1] * (0 - (fTemp75 / fTemp76))) + ((fTemp75 * fVec12[(IOTA-iTemp73)&16383]) / fTemp76)));
			fRec30[0] = fRec32[0];
			float fTemp77 = ((fRec33[1] * fTemp2) + (fTemp3 * fTemp51));
			fVec13[IOTA&16383] = fTemp77;
			fRec53[0] = (fSlow21 + (0.999f * ((iSlow20 * iTemp15) + fRec53[1])));
			float fTemp78 = (fRec53[0] + -1.49999f);
			int iTemp79 = int(min(8192, max(0, int(fTemp78))));
			float fTemp80 = floorf(fTemp78);
			float fTemp81 = (fTemp80 + (2.0f - fRec53[0]));
			float fTemp82 = (fRec53[0] - fTemp80);
			fRec52[0] = (fVec13[(IOTA-int((iTemp79 + 1)))&16383] + ((fRec52[1] * (0 - (fTemp81 / fTemp82))) + ((fTemp81 * fVec13[(IOTA-iTemp79)&16383]) / fTemp82)));
			fRec31[0] = fRec52[0];
			float fTemp83 = (0 - ((fTemp2 * fRec31[1]) + (fTemp3 * fTemp30)));
			fVec14[IOTA&16383] = fTemp83;
			fRec54[0] = (fSlow23 + (0.999f * ((iSlow22 * iTemp15) + fRec54[1])));
			float fTemp84 = (fRec54[0] + -1.49999f);
			int iTemp85 = int(min(8192, max(0, int(fTemp84))));
			float fTemp86 = floorf(fTemp84);
			float fTemp87 = (fTemp86 + (2.0f - fRec54[0]));
			float fTemp88 = (fRec54[0] - fTemp86);
			fRec29[0] = (fVec14[(IOTA-int((iTemp85 + 1)))&16383] + ((fRec29[1] * (0 - (fTemp87 / fTemp88))) + ((fTemp87 * fVec14[(IOTA-iTemp85)&16383]) / fTemp88)));
			fRec27[0] = fRec29[0];
			float fTemp89 = ((fRec30[1] * fTemp2) + (fTemp3 * fTemp50));
			fVec15[IOTA&16383] = fTemp89;
			fRec56[0] = (fSlow25 + (0.999f * ((iSlow24 * iTemp15) + fRec56[1])));
			float fTemp90 = (fRec56[0] + -1.49999f);
			int iTemp91 = int(min(8192, max(0, int(fTemp90))));
			float fTemp92 = floorf(fTemp90);
			float fTemp93 = (fTemp92 + (2.0f - fRec56[0]));
			float fTemp94 = (fRec56[0] - fTemp92);
			fRec55[0] = (fVec15[(IOTA-int((iTemp91 + 1)))&16383] + ((fRec55[1] * (0 - (fTemp93 / fTemp94))) + ((fTemp93 * fVec15[(IOTA-iTemp91)&16383]) / fTemp94)));
			fRec28[0] = fRec55[0];
			float fTemp95 = ((fTemp2 * fRec28[1]) + (fTemp3 * fTemp29));
			float fTemp96 = ((fTemp8 * fRec16[1]) + (fTemp6 * fTemp95));
			float fTemp97 = ((fTemp8 * fRec19[1]) + (fTemp6 * fTemp96));
			float fTemp98 = ((fTemp8 * fRec22[1]) + (fTemp6 * fTemp97));
			float fTemp99 = (0 - ((fTemp8 * fRec25[1]) + (fTemp6 * fTemp98)));
			fVec16[IOTA&16383] = fTemp99;
			fRec57[0] = (fSlow28 + (0.9999f * ((iSlow27 * iTemp15) + fRec57[1])));
			float fTemp100 = (fRec57[0] + -1.49999f);
			int iTemp101 = int(min(8192, max(0, int(fTemp100))));
			float fTemp102 = floorf(fTemp100);
			float fTemp103 = (fTemp102 + (2.0f - fRec57[0]));
			float fTemp104 = (fRec57[0] - fTemp102);
			fRec26[0] = (fVec16[(IOTA-int((iTemp101 + 1)))&16383] + ((fRec26[1] * (0 - (fTemp103 / fTemp104))) + ((fTemp103 * fVec16[(IOTA-iTemp101)&16383]) / fTemp104)));
			fRec24[0] = fRec26[0];
			float fTemp105 = ((fRec27[1] * fTemp2) + (fTemp3 * fTemp49));
			float fTemp106 = ((fTemp8 * fRec15[1]) + (fTemp105 * fTemp6));
			float fTemp107 = ((fTemp8 * fRec18[1]) + (fTemp6 * fTemp106));
			float fTemp108 = ((fTemp8 * fRec21[1]) + (fTemp6 * fTemp107));
			float fTemp109 = ((fRec24[1] * fTemp8) + (fTemp6 * fTemp108));
			fVec17[IOTA&16383] = fTemp109;
			fRec59[0] = (fSlow30 + (0.9999f * ((iSlow29 * iTemp15) + fRec59[1])));
			float fTemp110 = (fRec59[0] + -1.49999f);
			int iTemp111 = int(min(8192, max(0, int(fTemp110))));
			float fTemp112 = floorf(fTemp110);
			float fTemp113 = (fTemp112 + (2.0f - fRec59[0]));
			float fTemp114 = (fRec59[0] - fTemp112);
			fRec58[0] = (fVec17[(IOTA-int((iTemp111 + 1)))&16383] + ((fRec58[1] * (0 - (fTemp113 / fTemp114))) + ((fTemp113 * fVec17[(IOTA-iTemp111)&16383]) / fTemp114)));
			fRec25[0] = fRec58[0];
			float fTemp115 = (0 - ((fTemp6 * fRec25[1]) + (fTemp7 * fTemp98)));
			fVec18[IOTA&16383] = fTemp115;
			fRec60[0] = (fSlow33 + (0.999f * ((iSlow32 * iTemp15) + fRec60[1])));
			float fTemp116 = (fRec60[0] + -1.49999f);
			int iTemp117 = int(min(8192, max(0, int(fTemp116))));
			float fTemp118 = floorf(fTemp116);
			float fTemp119 = (fTemp118 + (2.0f - fRec60[0]));
			float fTemp120 = (fRec60[0] - fTemp118);
			fRec23[0] = (fVec18[(IOTA-int((iTemp117 + 1)))&16383] + ((fRec23[1] * (0 - (fTemp119 / fTemp120))) + ((fTemp119 * fVec18[(IOTA-iTemp117)&16383]) / fTemp120)));
			fRec21[0] = fRec23[0];
			float fTemp121 = ((fRec24[1] * fTemp6) + (fTemp7 * fTemp108));
			fVec19[IOTA&16383] = fTemp121;
			fRec62[0] = (fSlow35 + (0.999f * ((iSlow34 * iTemp15) + fRec62[1])));
			float fTemp122 = (fRec62[0] + -1.49999f);
			int iTemp123 = int(min(8192, max(0, int(fTemp122))));
			float fTemp124 = floorf(fTemp122);
			float fTemp125 = (fTemp124 + (2.0f - fRec62[0]));
			float fTemp126 = (fRec62[0] - fTemp124);
			fRec61[0] = (fVec19[(IOTA-int((iTemp123 + 1)))&16383] + ((fRec61[1] * (0 - (fTemp125 / fTemp126))) + ((fTemp125 * fVec19[(IOTA-iTemp123)&16383]) / fTemp126)));
			fRec22[0] = fRec61[0];
			float fTemp127 = (0 - ((fTemp6 * fRec22[1]) + (fTemp7 * fTemp97)));
			fVec20[IOTA&16383] = fTemp127;
			fRec63[0] = (fSlow38 + (0.999f * ((iSlow37 * iTemp15) + fRec63[1])));
			float fTemp128 = (fRec63[0] + -1.49999f);
			int iTemp129 = int(min(8192, max(0, int(fTemp128))));
			float fTemp130 = floorf(fTemp128);
			float fTemp131 = (fTemp130 + (2.0f - fRec63[0]));
			float fTemp132 = (fRec63[0] - fTemp130);
			fRec20[0] = (fVec20[(IOTA-int((iTemp129 + 1)))&16383] + ((fRec20[1] * (0 - (fTemp131 / fTemp132))) + ((fTemp131 * fVec20[(IOTA-iTemp129)&16383]) / fTemp132)));
			fRec18[0] = fRec20[0];
			float fTemp133 = ((fRec21[1] * fTemp6) + (fTemp7 * fTemp107));
			fVec21[IOTA&16383] = fTemp133;
			fRec65[0] = (fSlow40 + (0.999f * ((iSlow39 * iTemp15) + fRec65[1])));
			float fTemp134 = (fRec65[0] + -1.49999f);
			int iTemp135 = int(min(8192, max(0, int(fTemp134))));
			float fTemp136 = floorf(fTemp134);
			float fTemp137 = (fTemp136 + (2.0f - fRec65[0]));
			float fTemp138 = (fRec65[0] - fTemp136);
			fRec64[0] = (fVec21[(IOTA-int((iTemp135 + 1)))&16383] + ((fRec64[1] * (0 - (fTemp137 / fTemp138))) + ((fTemp137 * fVec21[(IOTA-iTemp135)&16383]) / fTemp138)));
			fRec19[0] = fRec64[0];
			float fTemp139 = (0 - ((fTemp6 * fRec19[1]) + (fTemp7 * fTemp96)));
			fVec22[IOTA&16383] = fTemp139;
			fRec66[0] = (fSlow43 + (0.999f * ((iSlow42 * iTemp15) + fRec66[1])));
			float fTemp140 = (fRec66[0] + -1.49999f);
			int iTemp141 = int(min(8192, max(0, int(fTemp140))));
			float fTemp142 = floorf(fTemp140);
			float fTemp143 = (fTemp142 + (2.0f - fRec66[0]));
			float fTemp144 = (fRec66[0] - fTemp142);
			fRec17[0] = (fVec22[(IOTA-int((iTemp141 + 1)))&16383] + ((fRec17[1] * (0 - (fTemp143 / fTemp144))) + ((fTemp143 * fVec22[(IOTA-iTemp141)&16383]) / fTemp144)));
			fRec15[0] = fRec17[0];
			float fTemp145 = ((fRec18[1] * fTemp6) + (fTemp7 * fTemp106));
			fVec23[IOTA&16383] = fTemp145;
			fRec68[0] = (fSlow45 + (0.999f * ((iSlow44 * iTemp15) + fRec68[1])));
			float fTemp146 = (fRec68[0] + -1.49999f);
			int iTemp147 = int(min(8192, max(0, int(fTemp146))));
			float fTemp148 = floorf(fTemp146);
			float fTemp149 = (fTemp148 + (2.0f - fRec68[0]));
			float fTemp150 = (fRec68[0] - fTemp148);
			fRec67[0] = (fVec23[(IOTA-int((iTemp147 + 1)))&16383] + ((fRec67[1] * (0 - (fTemp149 / fTemp150))) + ((fTemp149 * fVec23[(IOTA-iTemp147)&16383]) / fTemp150)));
			fRec16[0] = fRec67[0];
			float fTemp151 = ((fTemp6 * fRec16[1]) + (fTemp7 * fTemp95));
			float fTemp152 = ((fTemp4 * fRec4[1]) + (fTemp2 * fTemp151));
			float fTemp153 = ((fTemp4 * fRec7[1]) + (fTemp2 * fTemp152));
			float fTemp154 = ((fTemp4 * fRec10[1]) + (fTemp2 * fTemp153));
			float fTemp155 = (0 - ((fTemp4 * fRec13[1]) + (fTemp2 * fTemp154)));
			fVec24[IOTA&16383] = fTemp155;
			fRec69[0] = (fSlow48 + (0.9999f * ((iSlow47 * iTemp15) + fRec69[1])));
			float fTemp156 = (fRec69[0] + -1.49999f);
			int iTemp157 = int(min(8192, max(0, int(fTemp156))));
			float fTemp158 = floorf(fTemp156);
			float fTemp159 = (fTemp158 + (2.0f - fRec69[0]));
			float fTemp160 = (fRec69[0] - fTemp158);
			fRec14[0] = (fVec24[(IOTA-int((iTemp157 + 1)))&16383] + ((fRec14[1] * (0 - (fTemp159 / fTemp160))) + ((fTemp159 * fVec24[(IOTA-iTemp157)&16383]) / fTemp160)));
			fRec12[0] = fRec14[0];
			float fTemp161 = ((fRec15[1] * fTemp6) + (fTemp7 * fTemp105));
			float fTemp162 = ((fTemp4 * fRec3[1]) + (fTemp2 * fTemp161));
			float fTemp163 = ((fTemp4 * fRec6[1]) + (fTemp2 * fTemp162));
			float fTemp164 = ((fTemp4 * fRec9[1]) + (fTemp2 * fTemp163));
			float fTemp165 = ((fRec12[1] * fTemp4) + (fTemp2 * fTemp164));
			fVec25[IOTA&16383] = fTemp165;
			fRec71[0] = (fSlow50 + (0.9999f * ((iSlow49 * iTemp15) + fRec71[1])));
			float fTemp166 = (fRec71[0] + -1.49999f);
			int iTemp167 = int(min(8192, max(0, int(fTemp166))));
			float fTemp168 = floorf(fTemp166);
			float fTemp169 = (fTemp168 + (2.0f - fRec71[0]));
			float fTemp170 = (fRec71[0] - fTemp168);
			fRec70[0] = (fVec25[(IOTA-int((iTemp167 + 1)))&16383] + ((fRec70[1] * (0 - (fTemp169 / fTemp170))) + ((fTemp169 * fVec25[(IOTA-iTemp167)&16383]) / fTemp170)));
			fRec13[0] = fRec70[0];
			float fTemp171 = (0 - ((fTemp2 * fRec13[1]) + (fTemp3 * fTemp154)));
			fVec26[IOTA&16383] = fTemp171;
			fRec72[0] = (fSlow53 + (0.999f * ((iSlow52 * iTemp15) + fRec72[1])));
			float fTemp172 = (fRec72[0] + -1.49999f);
			int iTemp173 = int(min(8192, max(0, int(fTemp172))));
			float fTemp174 = floorf(fTemp172);
			float fTemp175 = (fTemp174 + (2.0f - fRec72[0]));
			float fTemp176 = (fRec72[0] - fTemp174);
			fRec11[0] = (fVec26[(IOTA-int((iTemp173 + 1)))&16383] + ((fRec11[1] * (0 - (fTemp175 / fTemp176))) + ((fTemp175 * fVec26[(IOTA-iTemp173)&16383]) / fTemp176)));
			fRec9[0] = fRec11[0];
			float fTemp177 = ((fRec12[1] * fTemp2) + (fTemp3 * fTemp164));
			fVec27[IOTA&16383] = fTemp177;
			fRec74[0] = (fSlow55 + (0.999f * ((iSlow54 * iTemp15) + fRec74[1])));
			float fTemp178 = (fRec74[0] + -1.49999f);
			int iTemp179 = int(min(8192, max(0, int(fTemp178))));
			float fTemp180 = floorf(fTemp178);
			float fTemp181 = (fTemp180 + (2.0f - fRec74[0]));
			float fTemp182 = (fRec74[0] - fTemp180);
			fRec73[0] = (fVec27[(IOTA-int((iTemp179 + 1)))&16383] + ((fRec73[1] * (0 - (fTemp181 / fTemp182))) + ((fTemp181 * fVec27[(IOTA-iTemp179)&16383]) / fTemp182)));
			fRec10[0] = fRec73[0];
			float fTemp183 = (0 - ((fTemp2 * fRec10[1]) + (fTemp3 * fTemp153)));
			fVec28[IOTA&16383] = fTemp183;
			fRec75[0] = (fSlow58 + (0.999f * ((iSlow57 * iTemp15) + fRec75[1])));
			float fTemp184 = (fRec75[0] + -1.49999f);
			int iTemp185 = int(min(8192, max(0, int(fTemp184))));
			float fTemp186 = floorf(fTemp184);
			float fTemp187 = (fTemp186 + (2.0f - fRec75[0]));
			float fTemp188 = (fRec75[0] - fTemp186);
			fRec8[0] = (fVec28[(IOTA-int((iTemp185 + 1)))&16383] + ((fRec8[1] * (0 - (fTemp187 / fTemp188))) + ((fTemp187 * fVec28[(IOTA-iTemp185)&16383]) / fTemp188)));
			fRec6[0] = fRec8[0];
			float fTemp189 = ((fRec9[1] * fTemp2) + (fTemp3 * fTemp163));
			fVec29[IOTA&16383] = fTemp189;
			fRec77[0] = (fSlow60 + (0.999f * ((iSlow59 * iTemp15) + fRec77[1])));
			float fTemp190 = (fRec77[0] + -1.49999f);
			int iTemp191 = int(min(8192, max(0, int(fTemp190))));
			float fTemp192 = floorf(fTemp190);
			float fTemp193 = (fTemp192 + (2.0f - fRec77[0]));
			float fTemp194 = (fRec77[0] - fTemp192);
			fRec76[0] = (fVec29[(IOTA-int((iTemp191 + 1)))&16383] + ((fRec76[1] * (0 - (fTemp193 / fTemp194))) + ((fTemp193 * fVec29[(IOTA-iTemp191)&16383]) / fTemp194)));
			fRec7[0] = fRec76[0];
			float fTemp195 = (0 - ((fTemp2 * fRec7[1]) + (fTemp3 * fTemp152)));
			fVec30[IOTA&16383] = fTemp195;
			fRec78[0] = (fSlow63 + (0.999f * ((iSlow62 * iTemp15) + fRec78[1])));
			float fTemp196 = (fRec78[0] + -1.49999f);
			int iTemp197 = int(min(8192, max(0, int(fTemp196))));
			float fTemp198 = floorf(fTemp196);
			float fTemp199 = (fTemp198 + (2.0f - fRec78[0]));
			float fTemp200 = (fRec78[0] - fTemp198);
			fRec5[0] = (fVec30[(IOTA-int((iTemp197 + 1)))&16383] + ((fRec5[1] * (0 - (fTemp199 / fTemp200))) + ((fTemp199 * fVec30[(IOTA-iTemp197)&16383]) / fTemp200)));
			fRec3[0] = fRec5[0];
			float fTemp201 = ((fRec6[1] * fTemp2) + (fTemp3 * fTemp162));
			fVec31[IOTA&16383] = fTemp201;
			fRec80[0] = (fSlow65 + (0.999f * ((iSlow64 * iTemp15) + fRec80[1])));
			float fTemp202 = (fRec80[0] + -1.49999f);
			int iTemp203 = int(min(8192, max(0, int(fTemp202))));
			float fTemp204 = floorf(fTemp202);
			float fTemp205 = (fTemp204 + (2.0f - fRec80[0]));
			float fTemp206 = (fRec80[0] - fTemp204);
			fRec79[0] = (fVec31[(IOTA-int((iTemp203 + 1)))&16383] + ((fRec79[1] * (0 - (fTemp205 / fTemp206))) + ((fTemp205 * fVec31[(IOTA-iTemp203)&16383]) / fTemp206)));
			fRec4[0] = fRec79[0];
			float fTemp207 = (1.0f - (0.5f * fTemp0));
			fRec2[0] = ((0.5f * (fTemp0 * fRec2[1])) + (((fRec3[1] * fTemp2) + (fTemp3 * fTemp161)) * fTemp207));
			fRec0[IOTA&1023] = fRec2[0];
			fRec81[0] = ((0.5f * (fTemp0 * fRec81[1])) + (fTemp207 * ((fTemp2 * fRec4[1]) + (fTemp3 * fTemp151))));
			fRec1[IOTA&1023] = fRec81[0];
			output0[i] = (FAUSTFLOAT)fRec0[(IOTA-0)&1023];
			output1[i] = (FAUSTFLOAT)fRec1[(IOTA-0)&1023];
			// post processing
			fRec81[1] = fRec81[0];
			fRec2[1] = fRec2[0];
			fRec4[1] = fRec4[0];
			fRec79[1] = fRec79[0];
			fRec80[1] = fRec80[0];
			fRec3[1] = fRec3[0];
			fRec5[1] = fRec5[0];
			fRec78[1] = fRec78[0];
			fRec7[1] = fRec7[0];
			fRec76[1] = fRec76[0];
			fRec77[1] = fRec77[0];
			fRec6[1] = fRec6[0];
			fRec8[1] = fRec8[0];
			fRec75[1] = fRec75[0];
			fRec10[1] = fRec10[0];
			fRec73[1] = fRec73[0];
			fRec74[1] = fRec74[0];
			fRec9[1] = fRec9[0];
			fRec11[1] = fRec11[0];
			fRec72[1] = fRec72[0];
			fRec13[1] = fRec13[0];
			fRec70[1] = fRec70[0];
			fRec71[1] = fRec71[0];
			fRec12[1] = fRec12[0];
			fRec14[1] = fRec14[0];
			fRec69[1] = fRec69[0];
			fRec16[1] = fRec16[0];
			fRec67[1] = fRec67[0];
			fRec68[1] = fRec68[0];
			fRec15[1] = fRec15[0];
			fRec17[1] = fRec17[0];
			fRec66[1] = fRec66[0];
			fRec19[1] = fRec19[0];
			fRec64[1] = fRec64[0];
			fRec65[1] = fRec65[0];
			fRec18[1] = fRec18[0];
			fRec20[1] = fRec20[0];
			fRec63[1] = fRec63[0];
			fRec22[1] = fRec22[0];
			fRec61[1] = fRec61[0];
			fRec62[1] = fRec62[0];
			fRec21[1] = fRec21[0];
			fRec23[1] = fRec23[0];
			fRec60[1] = fRec60[0];
			fRec25[1] = fRec25[0];
			fRec58[1] = fRec58[0];
			fRec59[1] = fRec59[0];
			fRec24[1] = fRec24[0];
			fRec26[1] = fRec26[0];
			fRec57[1] = fRec57[0];
			fRec28[1] = fRec28[0];
			fRec55[1] = fRec55[0];
			fRec56[1] = fRec56[0];
			fRec27[1] = fRec27[0];
			fRec29[1] = fRec29[0];
			fRec54[1] = fRec54[0];
			fRec31[1] = fRec31[0];
			fRec52[1] = fRec52[0];
			fRec53[1] = fRec53[0];
			fRec30[1] = fRec30[0];
			fRec32[1] = fRec32[0];
			fRec51[1] = fRec51[0];
			fRec34[1] = fRec34[0];
			fRec49[1] = fRec49[0];
			fRec50[1] = fRec50[0];
			fRec33[1] = fRec33[0];
			fRec35[1] = fRec35[0];
			fRec48[1] = fRec48[0];
			fRec37[1] = fRec37[0];
			fRec46[1] = fRec46[0];
			fRec47[1] = fRec47[0];
			fRec36[1] = fRec36[0];
			fRec38[1] = fRec38[0];
			fRec45[1] = fRec45[0];
			IOTA = IOTA+1;
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fVec5[1] = fVec5[0];
			fVec4[1] = fVec4[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fVec3[1] = fVec3[0];
			fVec2[1] = fVec2[0];
			iVec1[1] = iVec1[0];
			fVec0[1] = fVec0[0];
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
static t_class* greyhole_faust_class;
#define FAUST_EXT t_faust_greyhole
#define FAUST_EXT_CLASS greyhole_faust_class
// clang-format on

struct t_faust_greyhole {
    t_object x_obj;
#ifdef __MINGW32__
    /* This seems to be necessary as some as yet undetermined Pd routine seems
     to write past the end of x_obj on Windows. */
    int fence; /* dummy field (not used) */
#endif
    greyhole* dsp;
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
    t_faust_greyhole* x = reinterpret_cast<t_faust_greyhole*>(w[1]);
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

static void greyhole_faust_dsp(t_faust_greyhole* x, t_signal** sp)
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

static void greyhole_dump_to_console(t_faust_greyhole* x)
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

static void greyhole_faust_any(t_faust_greyhole* x, t_symbol* s, int argc, t_atom* argv)
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

static void faust_free_dsp(t_faust_greyhole* x)
{
    delete x->dsp;
    x->dsp = NULL;
}

static void faust_free_ui(t_faust_greyhole* x)
{
    delete x->ui;
    x->ui = NULL;
}

static void faust_free_inputs(t_faust_greyhole* x)
{
    if (x->inputs)
        free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust_greyhole* x)
{
    if (x->outputs)
        free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust_greyhole* x)
{
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i])
                free(x->buf[i]);
        }

        free(x->buf);
    }
}

static void greyhole_faust_free(t_faust_greyhole* x)
{
    faust_free_dsp(x);
    faust_free_ui(x);
    faust_free_inputs(x);
    faust_free_outputs(x);
    faust_free_buf(x);
}

static bool faust_init_inputs(t_faust_greyhole* x)
{
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if (x->inputs == NULL) {
            error("[%s] faust_init_inputs failed", sym(greyhole));
            return false;
        }
    }

    // creating sound inlets (except the first one!)
    for (int i = 0; i < (x->n_in - 1); i++) {
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    return true;
}

static bool faust_init_outputs(t_faust_greyhole* x, bool info_outlet)
{
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->outputs == NULL) {
            error("[%s] faust_init_outputs failed", sym(greyhole));
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->buf == NULL) {
            error("[%s] faust_init_outputs failed", sym(greyhole));
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

static bool faust_new_internal(t_faust_greyhole* x, const std::string& objId = "", bool info_outlet = true)
{
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new greyhole();
    x->ui = new PdUI<UI>(sym(greyhole), objId);

    if (!faust_init_inputs(x)) {
        greyhole_faust_free(x);
        return false;
    }

    if (!faust_init_outputs(x, info_outlet)) {
        greyhole_faust_free(x);
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
    t_faust_greyhole* x_;
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
    PdArgParser(t_faust_greyhole* x, int argc, t_atom* argv, bool info_outlet = true)
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

        std::deque<ceammc::AtomList> props = ceammc::AtomList(argc_, argv).properties();
        for (size_t i = 0; i < props.size(); i++) {
            ceammc::AtomList& p = props[i];
            // skip empty property
            if(p.size() < 2)
                continue;

            t_atom* data = p.toPdData() + 1;
            this->x_->ui->setProperty(p[0].asSymbol(), p.size() - 1, data);
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

    t_faust_greyhole* pd_obj()
    {
        return this->x_;
    }
};

static void* greyhole_faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s)
{
    greyhole_faust_class = class_new(s, reinterpret_cast<t_newmethod>(greyhole_faust_new),
        reinterpret_cast<t_method>(greyhole_faust_free),
        sizeof(t_faust_greyhole),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);
    class_addmethod(greyhole_faust_class, nullfn, &s_signal, A_NULL);
    class_addmethod(greyhole_faust_class, reinterpret_cast<t_method>(greyhole_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(greyhole_faust_class, reinterpret_cast<t_method>(greyhole_dump_to_console), gensym("dump"), A_NULL);
    CLASS_MAINSIGNALIN(greyhole_faust_class, t_faust_greyhole, f);
    class_addanything(greyhole_faust_class, greyhole_faust_any);
}

#define EXTERNAL_NEW void* greyhole_faust_new(t_symbol*, int argc, t_atom* argv)

#define EXTERNAL_SIMPLE_NEW()                                                           \
    static void* greyhole_faust_new(t_symbol*, int argc, t_atom* argv)                     \
    {                                                                                   \
        t_faust_greyhole* x = reinterpret_cast<t_faust_greyhole*>(pd_new(greyhole_faust_class)); \
        PdArgParser p(x, argc, argv, false);                                            \
        return p.pd_obj();                                                              \
    }

#define EXTERNAL_SETUP(MOD)                        \
    extern "C" void setup_##MOD##0x2egreyhole_tilde() \
    {                                              \
        internal_setup(gensym(#MOD ".greyhole~"));    \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);
