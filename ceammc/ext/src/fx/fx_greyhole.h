/* ------------------------------------------------------------
author: "Julian Parker, bug fixes by Till Bovermann"
copyright: "(c) Julian Parker 2013"
license: "GPL2+"
name: "GreyholeRaw"
version: "1.0"
Code generated with Faust 2.5.31 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __greyhole_H__
#define  __greyhole_H__

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

#include <string>

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
         * @param samplingRate - the sampling rate in Hertz
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
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
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
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
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

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

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
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

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

/* link with : "primes" */
#include "jprev.h"
#include <cmath>
#include <math.h>


#ifndef FAUSTCLASS 
#define FAUSTCLASS greyhole
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class greyhole : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	float fVec0[2];
	int iVec1[2];
	FAUSTFLOAT fHslider1;
	float fRec15[2];
	FAUSTFLOAT fHslider2;
	float fVec2[2];
	FAUSTFLOAT fHslider3;
	float fVec3[2];
	int fSamplingFreq;
	float fConst0;
	FAUSTFLOAT fHslider4;
	float fRec40[2];
	float fRec41[2];
	float fRec42[2];
	float fRec43[2];
	float fConst1;
	FAUSTFLOAT fHslider5;
	float fVec4[2];
	float fConst2;
	FAUSTFLOAT fHslider6;
	float fVec5[2];
	float fRec44[2];
	float fRec45[2];
	int IOTA;
	float fVec6[131072];
	float fVec7[16384];
	float fRec46[2];
	float fRec39[2];
	float fRec37[2];
	float fVec8[131072];
	float fVec9[16384];
	float fRec48[2];
	float fRec47[2];
	float fRec38[2];
	float fVec10[16384];
	float fRec49[2];
	float fRec36[2];
	float fRec34[2];
	float fVec11[16384];
	float fRec51[2];
	float fRec50[2];
	float fRec35[2];
	float fVec12[16384];
	float fRec52[2];
	float fRec33[2];
	float fRec31[2];
	float fVec13[16384];
	float fRec54[2];
	float fRec53[2];
	float fRec32[2];
	float fVec14[16384];
	float fRec55[2];
	float fRec30[2];
	float fRec28[2];
	float fVec15[16384];
	float fRec57[2];
	float fRec56[2];
	float fRec29[2];
	float fVec16[16384];
	float fRec58[2];
	float fRec27[2];
	float fRec25[2];
	float fVec17[16384];
	float fRec60[2];
	float fRec59[2];
	float fRec26[2];
	float fVec18[16384];
	float fRec61[2];
	float fRec24[2];
	float fRec22[2];
	float fVec19[16384];
	float fRec63[2];
	float fRec62[2];
	float fRec23[2];
	float fVec20[16384];
	float fRec64[2];
	float fRec21[2];
	float fRec19[2];
	float fVec21[16384];
	float fRec66[2];
	float fRec65[2];
	float fRec20[2];
	float fVec22[16384];
	float fRec67[2];
	float fRec18[2];
	float fRec16[2];
	float fRec69[2];
	float fVec23[16384];
	float fRec68[2];
	float fRec17[2];
	float fVec24[16384];
	float fRec14[2];
	float fRec12[2];
	float fVec25[16384];
	float fRec71[2];
	float fRec70[2];
	float fRec13[2];
	float fVec26[16384];
	float fRec72[2];
	float fRec11[2];
	float fRec9[2];
	float fVec27[16384];
	float fRec74[2];
	float fRec73[2];
	float fRec10[2];
	float fVec28[16384];
	float fRec75[2];
	float fRec8[2];
	float fRec6[2];
	float fVec29[16384];
	float fRec77[2];
	float fRec76[2];
	float fRec7[2];
	float fVec30[16384];
	float fRec78[2];
	float fRec5[2];
	float fRec3[2];
	float fVec31[16384];
	float fRec80[2];
	float fRec79[2];
	float fRec4[2];
	float fRec2[2];
	float fRec0[1024];
	float fRec81[2];
	float fRec1[1024];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Julian Parker, bug fixes by Till Bovermann");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("copyright", "(c) Julian Parker 2013");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("filename", "fx_greyhole");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("license", "GPL2+");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "GreyholeRaw");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 2;
		
	}
	virtual int getNumOutputs() {
		return 2;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	
	static void classInit(int samplingFreq) {
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = min(192000.0f, max(1.0f, float(fSamplingFreq)));
		fConst1 = (0.000566893432f * fConst0);
		fConst2 = (3.14159274f / fConst0);
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(1.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.90000000000000002f);
		fHslider4 = FAUSTFLOAT(0.20000000000000001f);
		fHslider5 = FAUSTFLOAT(0.10000000000000001f);
		fHslider6 = FAUSTFLOAT(2.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iVec1[l1] = 0;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec15[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fVec2[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec3[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec40[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec41[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec42[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec43[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fVec4[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fVec5[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec44[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec45[l12] = 0.0f;
			
		}
		IOTA = 0;
		for (int l13 = 0; (l13 < 131072); l13 = (l13 + 1)) {
			fVec6[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 16384); l14 = (l14 + 1)) {
			fVec7[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec46[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec39[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec37[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 131072); l18 = (l18 + 1)) {
			fVec8[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 16384); l19 = (l19 + 1)) {
			fVec9[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec48[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec47[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec38[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 16384); l23 = (l23 + 1)) {
			fVec10[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec49[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec36[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec34[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 16384); l27 = (l27 + 1)) {
			fVec11[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec51[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec50[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec35[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 16384); l31 = (l31 + 1)) {
			fVec12[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec52[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec33[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec31[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 16384); l35 = (l35 + 1)) {
			fVec13[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec54[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec53[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec32[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 16384); l39 = (l39 + 1)) {
			fVec14[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec55[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec30[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec28[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 16384); l43 = (l43 + 1)) {
			fVec15[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec57[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec56[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec29[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 16384); l47 = (l47 + 1)) {
			fVec16[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec58[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec27[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec25[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 16384); l51 = (l51 + 1)) {
			fVec17[l51] = 0.0f;
			
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec60[l52] = 0.0f;
			
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec59[l53] = 0.0f;
			
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec26[l54] = 0.0f;
			
		}
		for (int l55 = 0; (l55 < 16384); l55 = (l55 + 1)) {
			fVec18[l55] = 0.0f;
			
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec61[l56] = 0.0f;
			
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec24[l57] = 0.0f;
			
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec22[l58] = 0.0f;
			
		}
		for (int l59 = 0; (l59 < 16384); l59 = (l59 + 1)) {
			fVec19[l59] = 0.0f;
			
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec63[l60] = 0.0f;
			
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec62[l61] = 0.0f;
			
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec23[l62] = 0.0f;
			
		}
		for (int l63 = 0; (l63 < 16384); l63 = (l63 + 1)) {
			fVec20[l63] = 0.0f;
			
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec64[l64] = 0.0f;
			
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec21[l65] = 0.0f;
			
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec19[l66] = 0.0f;
			
		}
		for (int l67 = 0; (l67 < 16384); l67 = (l67 + 1)) {
			fVec21[l67] = 0.0f;
			
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec66[l68] = 0.0f;
			
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec65[l69] = 0.0f;
			
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec20[l70] = 0.0f;
			
		}
		for (int l71 = 0; (l71 < 16384); l71 = (l71 + 1)) {
			fVec22[l71] = 0.0f;
			
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec67[l72] = 0.0f;
			
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec18[l73] = 0.0f;
			
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec16[l74] = 0.0f;
			
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec69[l75] = 0.0f;
			
		}
		for (int l76 = 0; (l76 < 16384); l76 = (l76 + 1)) {
			fVec23[l76] = 0.0f;
			
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec68[l77] = 0.0f;
			
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec17[l78] = 0.0f;
			
		}
		for (int l79 = 0; (l79 < 16384); l79 = (l79 + 1)) {
			fVec24[l79] = 0.0f;
			
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec14[l80] = 0.0f;
			
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec12[l81] = 0.0f;
			
		}
		for (int l82 = 0; (l82 < 16384); l82 = (l82 + 1)) {
			fVec25[l82] = 0.0f;
			
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec71[l83] = 0.0f;
			
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec70[l84] = 0.0f;
			
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fRec13[l85] = 0.0f;
			
		}
		for (int l86 = 0; (l86 < 16384); l86 = (l86 + 1)) {
			fVec26[l86] = 0.0f;
			
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec72[l87] = 0.0f;
			
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec11[l88] = 0.0f;
			
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec9[l89] = 0.0f;
			
		}
		for (int l90 = 0; (l90 < 16384); l90 = (l90 + 1)) {
			fVec27[l90] = 0.0f;
			
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec74[l91] = 0.0f;
			
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec73[l92] = 0.0f;
			
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec10[l93] = 0.0f;
			
		}
		for (int l94 = 0; (l94 < 16384); l94 = (l94 + 1)) {
			fVec28[l94] = 0.0f;
			
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec75[l95] = 0.0f;
			
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec8[l96] = 0.0f;
			
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec6[l97] = 0.0f;
			
		}
		for (int l98 = 0; (l98 < 16384); l98 = (l98 + 1)) {
			fVec29[l98] = 0.0f;
			
		}
		for (int l99 = 0; (l99 < 2); l99 = (l99 + 1)) {
			fRec77[l99] = 0.0f;
			
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			fRec76[l100] = 0.0f;
			
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec7[l101] = 0.0f;
			
		}
		for (int l102 = 0; (l102 < 16384); l102 = (l102 + 1)) {
			fVec30[l102] = 0.0f;
			
		}
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			fRec78[l103] = 0.0f;
			
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec5[l104] = 0.0f;
			
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec3[l105] = 0.0f;
			
		}
		for (int l106 = 0; (l106 < 16384); l106 = (l106 + 1)) {
			fVec31[l106] = 0.0f;
			
		}
		for (int l107 = 0; (l107 < 2); l107 = (l107 + 1)) {
			fRec80[l107] = 0.0f;
			
		}
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			fRec79[l108] = 0.0f;
			
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec4[l109] = 0.0f;
			
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			fRec2[l110] = 0.0f;
			
		}
		for (int l111 = 0; (l111 < 1024); l111 = (l111 + 1)) {
			fRec0[l111] = 0.0f;
			
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fRec81[l112] = 0.0f;
			
		}
		for (int l113 = 0; (l113 < 1024); l113 = (l113 + 1)) {
			fRec1[l113] = 0.0f;
			
		}
		
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
		ui_interface->addHorizontalSlider("damping", &fHslider0, 0.0f, 0.0f, 0.99000001f, 0.00100000005f);
		ui_interface->addHorizontalSlider("delaytime", &fHslider4, 0.200000003f, 0.00100000005f, 1.45000005f, 9.99999975e-05f);
		ui_interface->addHorizontalSlider("diffusion", &fHslider2, 0.5f, 0.0f, 0.99000001f, 9.99999975e-05f);
		ui_interface->addHorizontalSlider("feedback", &fHslider3, 0.899999976f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("moddepth", &fHslider5, 0.100000001f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("modfreq", &fHslider6, 2.0f, 0.0f, 10.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("size", &fHslider1, 1.0f, 0.5f, 3.0f, 9.99999975e-05f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = float(fHslider1);
		float fSlow2 = (87.0f * fSlow1);
		int iSlow3 = int(primes(int(fSlow2)));
		float fSlow4 = (9.99999975e-05f * float(iSlow3));
		float fSlow5 = float(fHslider2);
		float fSlow6 = float(fHslider3);
		float fSlow7 = floorf(min(65533.0f, (fConst0 * float(fHslider4))));
		float fSlow8 = float(fHslider5);
		float fSlow9 = float(fHslider6);
		float fSlow10 = (49.0f * fSlow1);
		int iSlow11 = int(primes(int(fSlow10)));
		float fSlow12 = (9.99999975e-05f * float(iSlow11));
		int iSlow13 = int(primes(int((fSlow10 + 10.0f))));
		float fSlow14 = (9.99999975e-05f * float(iSlow13));
		float fSlow15 = (36.0f * fSlow1);
		int iSlow16 = int(primes(int(fSlow15)));
		float fSlow17 = (0.00100000005f * float(iSlow16));
		int iSlow18 = int(primes(int((fSlow15 + 10.0f))));
		float fSlow19 = (0.00100000005f * float(iSlow18));
		float fSlow20 = (23.0f * fSlow1);
		int iSlow21 = int(primes(int(fSlow20)));
		float fSlow22 = (0.00100000005f * float(iSlow21));
		int iSlow23 = int(primes(int((fSlow20 + 10.0f))));
		float fSlow24 = (0.00100000005f * float(iSlow23));
		int iSlow25 = int(primes(int((10.0f * fSlow1))));
		float fSlow26 = (0.00100000005f * float(iSlow25));
		int iSlow27 = int(primes(int((10.0f * (fSlow1 + 1.0f)))));
		float fSlow28 = (0.00100000005f * float(iSlow27));
		float fSlow29 = (68.0f * fSlow1);
		int iSlow30 = int(primes(int(fSlow29)));
		float fSlow31 = (9.99999975e-05f * float(iSlow30));
		int iSlow32 = int(primes(int((fSlow29 + 10.0f))));
		float fSlow33 = (9.99999975e-05f * float(iSlow32));
		float fSlow34 = (55.0f * fSlow1);
		int iSlow35 = int(primes(int(fSlow34)));
		float fSlow36 = (0.00100000005f * float(iSlow35));
		int iSlow37 = int(primes(int((fSlow34 + 10.0f))));
		float fSlow38 = (0.00100000005f * float(iSlow37));
		float fSlow39 = (42.0f * fSlow1);
		int iSlow40 = int(primes(int(fSlow39)));
		float fSlow41 = (0.00100000005f * float(iSlow40));
		int iSlow42 = int(primes(int((fSlow39 + 10.0f))));
		float fSlow43 = (0.00100000005f * float(iSlow42));
		float fSlow44 = (29.0f * fSlow1);
		int iSlow45 = int(primes(int(fSlow44)));
		float fSlow46 = (0.00100000005f * float(iSlow45));
		int iSlow47 = int(primes(int((fSlow44 + 10.0f))));
		float fSlow48 = (0.00100000005f * float(iSlow47));
		int iSlow49 = int(primes(int((fSlow2 + 10.0f))));
		float fSlow50 = (9.99999975e-05f * float(iSlow49));
		float fSlow51 = (74.0f * fSlow1);
		int iSlow52 = int(primes(int(fSlow51)));
		float fSlow53 = (0.00100000005f * float(iSlow52));
		int iSlow54 = int(primes(int((fSlow51 + 10.0f))));
		float fSlow55 = (0.00100000005f * float(iSlow54));
		float fSlow56 = (61.0f * fSlow1);
		int iSlow57 = int(primes(int(fSlow56)));
		float fSlow58 = (0.00100000005f * float(iSlow57));
		int iSlow59 = int(primes(int((fSlow56 + 10.0f))));
		float fSlow60 = (0.00100000005f * float(iSlow59));
		float fSlow61 = (48.0f * fSlow1);
		int iSlow62 = int(primes(int(fSlow61)));
		float fSlow63 = (0.00100000005f * float(iSlow62));
		int iSlow64 = int(primes(int((fSlow61 + 10.0f))));
		float fSlow65 = (0.00100000005f * float(iSlow64));
		for (int i = 0; (i < count); i = (i + 1)) {
			fVec0[0] = fSlow0;
			iVec1[0] = 1;
			float fTemp0 = (fSlow0 + fVec0[1]);
			int iTemp1 = (1 - iVec1[1]);
			fRec15[0] = (fSlow4 + (0.999899983f * (float((iSlow3 * iTemp1)) + fRec15[1])));
			float fTemp2 = (fRec15[0] + -1.49998999f);
			float fTemp3 = floorf(fTemp2);
			float fTemp4 = (fTemp3 + (2.0f - fRec15[0]));
			float fTemp5 = (fRec15[0] - fTemp3);
			fVec2[0] = fSlow5;
			float fTemp6 = (0.5f * (fSlow5 + fVec2[1]));
			float fTemp7 = sinf(fTemp6);
			float fTemp8 = (0.0f - fTemp7);
			float fTemp9 = (0.0f - fTemp6);
			float fTemp10 = cosf(fTemp9);
			float fTemp11 = sinf(fTemp9);
			float fTemp12 = (0.0f - fTemp11);
			float fTemp13 = cosf(fTemp6);
			fVec3[0] = fSlow6;
			float fTemp14 = (fSlow6 + fVec3[1]);
			float fTemp15 = ((fRec40[1] != 0.0f)?(((fRec41[1] > 0.0f) & (fRec41[1] < 1.0f))?fRec40[1]:0.0f):(((fRec41[1] == 0.0f) & (fSlow7 != fRec42[1]))?4.53514731e-05f:(((fRec41[1] == 1.0f) & (fSlow7 != fRec43[1]))?-4.53514731e-05f:0.0f)));
			fRec40[0] = fTemp15;
			fRec41[0] = max(0.0f, min(1.0f, (fRec41[1] + fTemp15)));
			fRec42[0] = (((fRec41[1] >= 1.0f) & (fRec43[1] != fSlow7))?fSlow7:fRec42[1]);
			fRec43[0] = (((fRec41[1] <= 0.0f) & (fRec42[1] != fSlow7))?fSlow7:fRec43[1]);
			fVec4[0] = fSlow8;
			float fTemp16 = (fSlow8 + fVec4[1]);
			fVec5[0] = fSlow9;
			float fTemp17 = (fConst2 * (fSlow9 + fVec5[1]));
			float fTemp18 = sinf(fTemp17);
			float fTemp19 = cosf(fTemp17);
			fRec44[0] = ((fTemp18 * fRec45[1]) + (fTemp19 * fRec44[1]));
			fRec45[0] = (((fTemp19 * fRec45[1]) + ((0.0f - fTemp18) * fRec44[1])) + float(iTemp1));
			float fTemp20 = (fConst1 * (fTemp16 * (fRec44[0] + 1.0f)));
			float fTemp21 = (fTemp20 + 8.50000477f);
			int iTemp22 = int(fTemp21);
			float fTemp23 = floorf(fTemp21);
			float fTemp24 = (fTemp20 + (9.0f - fTemp23));
			float fTemp25 = (fTemp20 + (8.0f - fTemp23));
			float fTemp26 = (fTemp20 + (7.0f - fTemp23));
			float fTemp27 = (fTemp20 + (6.0f - fTemp23));
			float fTemp28 = (fTemp24 * fTemp25);
			float fTemp29 = (((((fRec1[((IOTA - (min(512, max(0, iTemp22)) + 1)) & 1023)] * (0.0f - fTemp24)) * (0.0f - (0.5f * fTemp25))) * (0.0f - (0.333333343f * fTemp26))) * (0.0f - (0.25f * fTemp27))) + ((fTemp20 + (10.0f - fTemp23)) * ((((0.5f * (((fTemp24 * fRec1[((IOTA - (min(512, max(0, (iTemp22 + 2))) + 1)) & 1023)]) * (0.0f - fTemp26)) * (0.0f - (0.5f * fTemp27)))) + (((fRec1[((IOTA - (min(512, max(0, (iTemp22 + 1))) + 1)) & 1023)] * (0.0f - fTemp25)) * (0.0f - (0.5f * fTemp26))) * (0.0f - (0.333333343f * fTemp27)))) + (0.166666672f * ((fTemp28 * fRec1[((IOTA - (min(512, max(0, (iTemp22 + 3))) + 1)) & 1023)]) * (0.0f - fTemp27)))) + (0.0416666679f * ((fTemp28 * fTemp26) * fRec1[((IOTA - (min(512, max(0, (iTemp22 + 4))) + 1)) & 1023)])))));
			fVec6[(IOTA & 131071)] = fTemp29;
			int iTemp30 = int(min(65536.0f, max(0.0f, fRec43[0])));
			float fTemp31 = (1.0f - fRec41[0]);
			int iTemp32 = int(min(65536.0f, max(0.0f, fRec42[0])));
			float fTemp33 = (float(input1[i]) + (0.5f * (fTemp14 * ((fRec41[0] * fVec6[((IOTA - iTemp30) & 131071)]) + (fTemp31 * fVec6[((IOTA - iTemp32) & 131071)])))));
			float fTemp34 = ((fTemp8 * fRec29[1]) + (fTemp13 * fTemp33));
			float fTemp35 = ((fTemp8 * fRec32[1]) + (fTemp13 * fTemp34));
			float fTemp36 = ((fTemp8 * fRec35[1]) + (fTemp13 * fTemp35));
			float fTemp37 = (0.0f - ((fTemp8 * fRec38[1]) + (fTemp13 * fTemp36)));
			fVec7[(IOTA & 16383)] = fTemp37;
			fRec46[0] = (fSlow12 + (0.999899983f * (float((iSlow11 * iTemp1)) + fRec46[1])));
			float fTemp38 = (fRec46[0] + -1.49998999f);
			int iTemp39 = min(8192, max(0, int(fTemp38)));
			float fTemp40 = floorf(fTemp38);
			float fTemp41 = (fTemp40 + (2.0f - fRec46[0]));
			float fTemp42 = (fRec46[0] - fTemp40);
			fRec39[0] = (fVec7[((IOTA - (iTemp39 + 1)) & 16383)] + ((fRec39[1] * (0.0f - (fTemp41 / fTemp42))) + ((fTemp41 * fVec7[((IOTA - iTemp39) & 16383)]) / fTemp42)));
			fRec37[0] = fRec39[0];
			float fTemp43 = (fConst1 * (fTemp16 * (fRec45[0] + 1.0f)));
			float fTemp44 = (fTemp43 + 8.50000477f);
			int iTemp45 = int(fTemp44);
			float fTemp46 = floorf(fTemp44);
			float fTemp47 = (fTemp43 + (9.0f - fTemp46));
			float fTemp48 = (fTemp43 + (8.0f - fTemp46));
			float fTemp49 = (fTemp43 + (7.0f - fTemp46));
			float fTemp50 = (fTemp43 + (6.0f - fTemp46));
			float fTemp51 = (fTemp47 * fTemp48);
			float fTemp52 = (((((fRec0[((IOTA - (min(512, max(0, iTemp45)) + 1)) & 1023)] * (0.0f - fTemp47)) * (0.0f - (0.5f * fTemp48))) * (0.0f - (0.333333343f * fTemp49))) * (0.0f - (0.25f * fTemp50))) + ((fTemp43 + (10.0f - fTemp46)) * ((((0.5f * (((fTemp47 * fRec0[((IOTA - (min(512, max(0, (iTemp45 + 2))) + 1)) & 1023)]) * (0.0f - fTemp49)) * (0.0f - (0.5f * fTemp50)))) + (((fRec0[((IOTA - (min(512, max(0, (iTemp45 + 1))) + 1)) & 1023)] * (0.0f - fTemp48)) * (0.0f - (0.5f * fTemp49))) * (0.0f - (0.333333343f * fTemp50)))) + (0.166666672f * ((fTemp51 * fRec0[((IOTA - (min(512, max(0, (iTemp45 + 3))) + 1)) & 1023)]) * (0.0f - fTemp50)))) + (0.0416666679f * ((fTemp51 * fTemp49) * fRec0[((IOTA - (min(512, max(0, (iTemp45 + 4))) + 1)) & 1023)])))));
			fVec8[(IOTA & 131071)] = fTemp52;
			float fTemp53 = (float(input0[i]) + (0.5f * (((fTemp31 * fVec8[((IOTA - iTemp32) & 131071)]) + (fRec41[0] * fVec8[((IOTA - iTemp30) & 131071)])) * fTemp14)));
			float fTemp54 = ((fTemp8 * fRec28[1]) + (fTemp13 * fTemp53));
			float fTemp55 = ((fTemp8 * fRec31[1]) + (fTemp13 * fTemp54));
			float fTemp56 = ((fTemp8 * fRec34[1]) + (fTemp13 * fTemp55));
			float fTemp57 = ((fTemp8 * fRec37[1]) + (fTemp13 * fTemp56));
			fVec9[(IOTA & 16383)] = fTemp57;
			fRec48[0] = (fSlow14 + (0.999899983f * (float((iSlow13 * iTemp1)) + fRec48[1])));
			float fTemp58 = (fRec48[0] + -1.49998999f);
			int iTemp59 = min(8192, max(0, int(fTemp58)));
			float fTemp60 = floorf(fTemp58);
			float fTemp61 = (fTemp60 + (2.0f - fRec48[0]));
			float fTemp62 = (fRec48[0] - fTemp60);
			fRec47[0] = (fVec9[((IOTA - (iTemp59 + 1)) & 16383)] + ((fRec47[1] * (0.0f - (fTemp61 / fTemp62))) + ((fTemp61 * fVec9[((IOTA - iTemp59) & 16383)]) / fTemp62)));
			fRec38[0] = fRec47[0];
			float fTemp63 = (0.0f - ((fTemp13 * fRec38[1]) + (fTemp7 * fTemp36)));
			fVec10[(IOTA & 16383)] = fTemp63;
			fRec49[0] = (fSlow17 + (0.999000013f * (float((iSlow16 * iTemp1)) + fRec49[1])));
			float fTemp64 = (fRec49[0] + -1.49998999f);
			int iTemp65 = min(8192, max(0, int(fTemp64)));
			float fTemp66 = floorf(fTemp64);
			float fTemp67 = (fTemp66 + (2.0f - fRec49[0]));
			float fTemp68 = (fRec49[0] - fTemp66);
			fRec36[0] = (fVec10[((IOTA - (iTemp65 + 1)) & 16383)] + ((fRec36[1] * (0.0f - (fTemp67 / fTemp68))) + ((fTemp67 * fVec10[((IOTA - iTemp65) & 16383)]) / fTemp68)));
			fRec34[0] = fRec36[0];
			float fTemp69 = ((fRec37[1] * fTemp13) + (fTemp7 * fTemp56));
			fVec11[(IOTA & 16383)] = fTemp69;
			fRec51[0] = (fSlow19 + (0.999000013f * (float((iSlow18 * iTemp1)) + fRec51[1])));
			float fTemp70 = (fRec51[0] + -1.49998999f);
			int iTemp71 = min(8192, max(0, int(fTemp70)));
			float fTemp72 = floorf(fTemp70);
			float fTemp73 = (fTemp72 + (2.0f - fRec51[0]));
			float fTemp74 = (fRec51[0] - fTemp72);
			fRec50[0] = (fVec11[((IOTA - (iTemp71 + 1)) & 16383)] + (((0.0f - (fTemp73 / fTemp74)) * fRec50[1]) + ((fTemp73 * fVec11[((IOTA - iTemp71) & 16383)]) / fTemp74)));
			fRec35[0] = fRec50[0];
			float fTemp75 = (0.0f - ((fTemp13 * fRec35[1]) + (fTemp7 * fTemp35)));
			fVec12[(IOTA & 16383)] = fTemp75;
			fRec52[0] = (fSlow22 + (0.999000013f * (float((iSlow21 * iTemp1)) + fRec52[1])));
			float fTemp76 = (fRec52[0] + -1.49998999f);
			int iTemp77 = min(8192, max(0, int(fTemp76)));
			float fTemp78 = floorf(fTemp76);
			float fTemp79 = (fTemp78 + (2.0f - fRec52[0]));
			float fTemp80 = (fRec52[0] - fTemp78);
			fRec33[0] = (fVec12[((IOTA - (iTemp77 + 1)) & 16383)] + ((fRec33[1] * (0.0f - (fTemp79 / fTemp80))) + ((fTemp79 * fVec12[((IOTA - iTemp77) & 16383)]) / fTemp80)));
			fRec31[0] = fRec33[0];
			float fTemp81 = ((fRec34[1] * fTemp13) + (fTemp7 * fTemp55));
			fVec13[(IOTA & 16383)] = fTemp81;
			fRec54[0] = (fSlow24 + (0.999000013f * (float((iSlow23 * iTemp1)) + fRec54[1])));
			float fTemp82 = (fRec54[0] + -1.49998999f);
			int iTemp83 = min(8192, max(0, int(fTemp82)));
			float fTemp84 = floorf(fTemp82);
			float fTemp85 = (fTemp84 + (2.0f - fRec54[0]));
			float fTemp86 = (fRec54[0] - fTemp84);
			fRec53[0] = (fVec13[((IOTA - (iTemp83 + 1)) & 16383)] + ((fRec53[1] * (0.0f - (fTemp85 / fTemp86))) + ((fTemp85 * fVec13[((IOTA - iTemp83) & 16383)]) / fTemp86)));
			fRec32[0] = fRec53[0];
			float fTemp87 = (0.0f - ((fTemp13 * fRec32[1]) + (fTemp7 * fTemp34)));
			fVec14[(IOTA & 16383)] = fTemp87;
			fRec55[0] = (fSlow26 + (0.999000013f * (float((iSlow25 * iTemp1)) + fRec55[1])));
			float fTemp88 = (fRec55[0] + -1.49998999f);
			int iTemp89 = min(8192, max(0, int(fTemp88)));
			float fTemp90 = floorf(fTemp88);
			float fTemp91 = (fTemp90 + (2.0f - fRec55[0]));
			float fTemp92 = (fRec55[0] - fTemp90);
			fRec30[0] = (fVec14[((IOTA - (iTemp89 + 1)) & 16383)] + ((fRec30[1] * (0.0f - (fTemp91 / fTemp92))) + ((fTemp91 * fVec14[((IOTA - iTemp89) & 16383)]) / fTemp92)));
			fRec28[0] = fRec30[0];
			float fTemp93 = ((fRec31[1] * fTemp13) + (fTemp7 * fTemp54));
			fVec15[(IOTA & 16383)] = fTemp93;
			fRec57[0] = (fSlow28 + (0.999000013f * (float((iSlow27 * iTemp1)) + fRec57[1])));
			float fTemp94 = (fRec57[0] + -1.49998999f);
			int iTemp95 = min(8192, max(0, int(fTemp94)));
			float fTemp96 = floorf(fTemp94);
			float fTemp97 = (fTemp96 + (2.0f - fRec57[0]));
			float fTemp98 = (fRec57[0] - fTemp96);
			fRec56[0] = (fVec15[((IOTA - (iTemp95 + 1)) & 16383)] + ((fRec56[1] * (0.0f - (fTemp97 / fTemp98))) + ((fTemp97 * fVec15[((IOTA - iTemp95) & 16383)]) / fTemp98)));
			fRec29[0] = fRec56[0];
			float fTemp99 = ((fTemp13 * fRec29[1]) + (fTemp7 * fTemp33));
			float fTemp100 = ((fTemp12 * fRec17[1]) + (fTemp10 * fTemp99));
			float fTemp101 = ((fTemp12 * fRec20[1]) + (fTemp10 * fTemp100));
			float fTemp102 = ((fTemp12 * fRec23[1]) + (fTemp10 * fTemp101));
			float fTemp103 = (0.0f - ((fTemp12 * fRec26[1]) + (fTemp10 * fTemp102)));
			fVec16[(IOTA & 16383)] = fTemp103;
			fRec58[0] = (fSlow31 + (0.999899983f * (float((iSlow30 * iTemp1)) + fRec58[1])));
			float fTemp104 = (fRec58[0] + -1.49998999f);
			int iTemp105 = min(8192, max(0, int(fTemp104)));
			float fTemp106 = floorf(fTemp104);
			float fTemp107 = (fTemp106 + (2.0f - fRec58[0]));
			float fTemp108 = (fRec58[0] - fTemp106);
			fRec27[0] = (fVec16[((IOTA - (iTemp105 + 1)) & 16383)] + ((fRec27[1] * (0.0f - (fTemp107 / fTemp108))) + ((fTemp107 * fVec16[((IOTA - iTemp105) & 16383)]) / fTemp108)));
			fRec25[0] = fRec27[0];
			float fTemp109 = ((fRec28[1] * fTemp13) + (fTemp7 * fTemp53));
			float fTemp110 = ((fTemp12 * fRec16[1]) + (fTemp109 * fTemp10));
			float fTemp111 = ((fTemp12 * fRec19[1]) + (fTemp10 * fTemp110));
			float fTemp112 = ((fTemp12 * fRec22[1]) + (fTemp10 * fTemp111));
			float fTemp113 = ((fRec25[1] * fTemp12) + (fTemp10 * fTemp112));
			fVec17[(IOTA & 16383)] = fTemp113;
			fRec60[0] = (fSlow33 + (0.999899983f * (float((iSlow32 * iTemp1)) + fRec60[1])));
			float fTemp114 = (fRec60[0] + -1.49998999f);
			int iTemp115 = min(8192, max(0, int(fTemp114)));
			float fTemp116 = floorf(fTemp114);
			float fTemp117 = (fTemp116 + (2.0f - fRec60[0]));
			float fTemp118 = (fRec60[0] - fTemp116);
			fRec59[0] = (fVec17[((IOTA - (iTemp115 + 1)) & 16383)] + ((fRec59[1] * (0.0f - (fTemp117 / fTemp118))) + ((fTemp117 * fVec17[((IOTA - iTemp115) & 16383)]) / fTemp118)));
			fRec26[0] = fRec59[0];
			float fTemp119 = (0.0f - ((fTemp10 * fRec26[1]) + (fTemp11 * fTemp102)));
			fVec18[(IOTA & 16383)] = fTemp119;
			fRec61[0] = (fSlow36 + (0.999000013f * (float((iSlow35 * iTemp1)) + fRec61[1])));
			float fTemp120 = (fRec61[0] + -1.49998999f);
			int iTemp121 = min(8192, max(0, int(fTemp120)));
			float fTemp122 = floorf(fTemp120);
			float fTemp123 = (fTemp122 + (2.0f - fRec61[0]));
			float fTemp124 = (fRec61[0] - fTemp122);
			fRec24[0] = (fVec18[((IOTA - (iTemp121 + 1)) & 16383)] + ((fRec24[1] * (0.0f - (fTemp123 / fTemp124))) + ((fTemp123 * fVec18[((IOTA - iTemp121) & 16383)]) / fTemp124)));
			fRec22[0] = fRec24[0];
			float fTemp125 = ((fRec25[1] * fTemp10) + (fTemp11 * fTemp112));
			fVec19[(IOTA & 16383)] = fTemp125;
			fRec63[0] = (fSlow38 + (0.999000013f * (float((iSlow37 * iTemp1)) + fRec63[1])));
			float fTemp126 = (fRec63[0] + -1.49998999f);
			int iTemp127 = min(8192, max(0, int(fTemp126)));
			float fTemp128 = floorf(fTemp126);
			float fTemp129 = (fTemp128 + (2.0f - fRec63[0]));
			float fTemp130 = (fRec63[0] - fTemp128);
			fRec62[0] = (fVec19[((IOTA - (iTemp127 + 1)) & 16383)] + ((fRec62[1] * (0.0f - (fTemp129 / fTemp130))) + ((fTemp129 * fVec19[((IOTA - iTemp127) & 16383)]) / fTemp130)));
			fRec23[0] = fRec62[0];
			float fTemp131 = (0.0f - ((fTemp10 * fRec23[1]) + (fTemp11 * fTemp101)));
			fVec20[(IOTA & 16383)] = fTemp131;
			fRec64[0] = (fSlow41 + (0.999000013f * (float((iSlow40 * iTemp1)) + fRec64[1])));
			float fTemp132 = (fRec64[0] + -1.49998999f);
			int iTemp133 = min(8192, max(0, int(fTemp132)));
			float fTemp134 = floorf(fTemp132);
			float fTemp135 = (fTemp134 + (2.0f - fRec64[0]));
			float fTemp136 = (fRec64[0] - fTemp134);
			fRec21[0] = (fVec20[((IOTA - (iTemp133 + 1)) & 16383)] + ((fRec21[1] * (0.0f - (fTemp135 / fTemp136))) + ((fTemp135 * fVec20[((IOTA - iTemp133) & 16383)]) / fTemp136)));
			fRec19[0] = fRec21[0];
			float fTemp137 = ((fRec22[1] * fTemp10) + (fTemp11 * fTemp111));
			fVec21[(IOTA & 16383)] = fTemp137;
			fRec66[0] = (fSlow43 + (0.999000013f * (float((iSlow42 * iTemp1)) + fRec66[1])));
			float fTemp138 = (fRec66[0] + -1.49998999f);
			int iTemp139 = min(8192, max(0, int(fTemp138)));
			float fTemp140 = floorf(fTemp138);
			float fTemp141 = (fTemp140 + (2.0f - fRec66[0]));
			float fTemp142 = (fRec66[0] - fTemp140);
			fRec65[0] = (fVec21[((IOTA - (iTemp139 + 1)) & 16383)] + ((fRec65[1] * (0.0f - (fTemp141 / fTemp142))) + ((fTemp141 * fVec21[((IOTA - iTemp139) & 16383)]) / fTemp142)));
			fRec20[0] = fRec65[0];
			float fTemp143 = (0.0f - ((fTemp10 * fRec20[1]) + (fTemp11 * fTemp100)));
			fVec22[(IOTA & 16383)] = fTemp143;
			fRec67[0] = (fSlow46 + (0.999000013f * (float((iSlow45 * iTemp1)) + fRec67[1])));
			float fTemp144 = (fRec67[0] + -1.49998999f);
			int iTemp145 = min(8192, max(0, int(fTemp144)));
			float fTemp146 = floorf(fTemp144);
			float fTemp147 = (fTemp146 + (2.0f - fRec67[0]));
			float fTemp148 = (fRec67[0] - fTemp146);
			fRec18[0] = (fVec22[((IOTA - (iTemp145 + 1)) & 16383)] + ((fRec18[1] * (0.0f - (fTemp147 / fTemp148))) + ((fTemp147 * fVec22[((IOTA - iTemp145) & 16383)]) / fTemp148)));
			fRec16[0] = fRec18[0];
			fRec69[0] = (fSlow48 + (0.999000013f * (float((iSlow47 * iTemp1)) + fRec69[1])));
			float fTemp149 = (fRec69[0] + -1.49998999f);
			float fTemp150 = floorf(fTemp149);
			float fTemp151 = (fTemp150 + (2.0f - fRec69[0]));
			float fTemp152 = ((fRec19[1] * fTemp10) + (fTemp11 * fTemp110));
			fVec23[(IOTA & 16383)] = fTemp152;
			int iTemp153 = min(8192, max(0, int(fTemp149)));
			float fTemp154 = (fRec69[0] - fTemp150);
			fRec68[0] = (((fTemp151 * fVec23[((IOTA - iTemp153) & 16383)]) / fTemp154) + (fVec23[((IOTA - (iTemp153 + 1)) & 16383)] + (fRec68[1] * (0.0f - (fTemp151 / fTemp154)))));
			fRec17[0] = fRec68[0];
			float fTemp155 = ((fTemp10 * fRec17[1]) + (fTemp11 * fTemp99));
			float fTemp156 = ((fTemp8 * fRec4[1]) + (fTemp155 * fTemp13));
			float fTemp157 = ((fTemp8 * fRec7[1]) + (fTemp156 * fTemp13));
			float fTemp158 = ((fTemp8 * fRec10[1]) + (fTemp157 * fTemp13));
			float fTemp159 = (0.0f - ((fTemp8 * fRec13[1]) + (fTemp158 * fTemp13)));
			fVec24[(IOTA & 16383)] = fTemp159;
			int iTemp160 = min(8192, max(0, int(fTemp2)));
			fRec14[0] = (((fRec14[1] * (0.0f - (fTemp4 / fTemp5))) + ((fTemp4 * fVec24[((IOTA - iTemp160) & 16383)]) / fTemp5)) + fVec24[((IOTA - (iTemp160 + 1)) & 16383)]);
			fRec12[0] = fRec14[0];
			float fTemp161 = ((fRec16[1] * fTemp10) + (fTemp11 * fTemp109));
			float fTemp162 = ((fTemp8 * fRec3[1]) + (fTemp161 * fTemp13));
			float fTemp163 = ((fTemp8 * fRec6[1]) + (fTemp162 * fTemp13));
			float fTemp164 = ((fTemp8 * fRec9[1]) + (fTemp163 * fTemp13));
			float fTemp165 = ((fRec12[1] * fTemp8) + (fTemp164 * fTemp13));
			fVec25[(IOTA & 16383)] = fTemp165;
			fRec71[0] = (fSlow50 + (0.999899983f * (float((iSlow49 * iTemp1)) + fRec71[1])));
			float fTemp166 = (fRec71[0] + -1.49998999f);
			int iTemp167 = min(8192, max(0, int(fTemp166)));
			float fTemp168 = floorf(fTemp166);
			float fTemp169 = (fTemp168 + (2.0f - fRec71[0]));
			float fTemp170 = (fRec71[0] - fTemp168);
			fRec70[0] = (fVec25[((IOTA - (iTemp167 + 1)) & 16383)] + ((fRec70[1] * (0.0f - (fTemp169 / fTemp170))) + ((fTemp169 * fVec25[((IOTA - iTemp167) & 16383)]) / fTemp170)));
			fRec13[0] = fRec70[0];
			float fTemp171 = (0.0f - ((fRec13[1] * fTemp13) + (fTemp7 * fTemp158)));
			fVec26[(IOTA & 16383)] = fTemp171;
			fRec72[0] = (fSlow53 + (0.999000013f * (float((iSlow52 * iTemp1)) + fRec72[1])));
			float fTemp172 = (fRec72[0] + -1.49998999f);
			int iTemp173 = min(8192, max(0, int(fTemp172)));
			float fTemp174 = floorf(fTemp172);
			float fTemp175 = (fTemp174 + (2.0f - fRec72[0]));
			float fTemp176 = (fRec72[0] - fTemp174);
			fRec11[0] = (fVec26[((IOTA - (iTemp173 + 1)) & 16383)] + ((fRec11[1] * (0.0f - (fTemp175 / fTemp176))) + ((fTemp175 * fVec26[((IOTA - iTemp173) & 16383)]) / fTemp176)));
			fRec9[0] = fRec11[0];
			float fTemp177 = ((fRec12[1] * fTemp13) + (fTemp7 * fTemp164));
			fVec27[(IOTA & 16383)] = fTemp177;
			fRec74[0] = (fSlow55 + (0.999000013f * (float((iSlow54 * iTemp1)) + fRec74[1])));
			float fTemp178 = (fRec74[0] + -1.49998999f);
			int iTemp179 = min(8192, max(0, int(fTemp178)));
			float fTemp180 = floorf(fTemp178);
			float fTemp181 = (fTemp180 + (2.0f - fRec74[0]));
			float fTemp182 = (fRec74[0] - fTemp180);
			fRec73[0] = (fVec27[((IOTA - (iTemp179 + 1)) & 16383)] + ((fRec73[1] * (0.0f - (fTemp181 / fTemp182))) + ((fTemp181 * fVec27[((IOTA - iTemp179) & 16383)]) / fTemp182)));
			fRec10[0] = fRec73[0];
			float fTemp183 = (0.0f - ((fRec10[1] * fTemp13) + (fTemp7 * fTemp157)));
			fVec28[(IOTA & 16383)] = fTemp183;
			fRec75[0] = (fSlow58 + (0.999000013f * (float((iSlow57 * iTemp1)) + fRec75[1])));
			float fTemp184 = (fRec75[0] + -1.49998999f);
			int iTemp185 = min(8192, max(0, int(fTemp184)));
			float fTemp186 = floorf(fTemp184);
			float fTemp187 = (fTemp186 + (2.0f - fRec75[0]));
			float fTemp188 = (fRec75[0] - fTemp186);
			fRec8[0] = (fVec28[((IOTA - (iTemp185 + 1)) & 16383)] + ((fRec8[1] * (0.0f - (fTemp187 / fTemp188))) + ((fTemp187 * fVec28[((IOTA - iTemp185) & 16383)]) / fTemp188)));
			fRec6[0] = fRec8[0];
			float fTemp189 = ((fRec9[1] * fTemp13) + (fTemp7 * fTemp163));
			fVec29[(IOTA & 16383)] = fTemp189;
			fRec77[0] = (fSlow60 + (0.999000013f * (float((iSlow59 * iTemp1)) + fRec77[1])));
			float fTemp190 = (fRec77[0] + -1.49998999f);
			int iTemp191 = min(8192, max(0, int(fTemp190)));
			float fTemp192 = floorf(fTemp190);
			float fTemp193 = (fTemp192 + (2.0f - fRec77[0]));
			float fTemp194 = (fRec77[0] - fTemp192);
			fRec76[0] = (fVec29[((IOTA - (iTemp191 + 1)) & 16383)] + ((fRec76[1] * (0.0f - (fTemp193 / fTemp194))) + ((fTemp193 * fVec29[((IOTA - iTemp191) & 16383)]) / fTemp194)));
			fRec7[0] = fRec76[0];
			float fTemp195 = (0.0f - ((fRec7[1] * fTemp13) + (fTemp7 * fTemp156)));
			fVec30[(IOTA & 16383)] = fTemp195;
			fRec78[0] = (fSlow63 + (0.999000013f * (float((iSlow62 * iTemp1)) + fRec78[1])));
			float fTemp196 = (fRec78[0] + -1.49998999f);
			int iTemp197 = min(8192, max(0, int(fTemp196)));
			float fTemp198 = floorf(fTemp196);
			float fTemp199 = (fTemp198 + (2.0f - fRec78[0]));
			float fTemp200 = (fRec78[0] - fTemp198);
			fRec5[0] = (fVec30[((IOTA - (iTemp197 + 1)) & 16383)] + ((fRec5[1] * (0.0f - (fTemp199 / fTemp200))) + ((fTemp199 * fVec30[((IOTA - iTemp197) & 16383)]) / fTemp200)));
			fRec3[0] = fRec5[0];
			float fTemp201 = ((fRec6[1] * fTemp13) + (fTemp7 * fTemp162));
			fVec31[(IOTA & 16383)] = fTemp201;
			fRec80[0] = (fSlow65 + (0.999000013f * (float((iSlow64 * iTemp1)) + fRec80[1])));
			float fTemp202 = (fRec80[0] + -1.49998999f);
			int iTemp203 = min(8192, max(0, int(fTemp202)));
			float fTemp204 = floorf(fTemp202);
			float fTemp205 = (fTemp204 + (2.0f - fRec80[0]));
			float fTemp206 = (fRec80[0] - fTemp204);
			fRec79[0] = (fVec31[((IOTA - (iTemp203 + 1)) & 16383)] + ((fRec79[1] * (0.0f - (fTemp205 / fTemp206))) + ((fTemp205 * fVec31[((IOTA - iTemp203) & 16383)]) / fTemp206)));
			fRec4[0] = fRec79[0];
			float fTemp207 = (1.0f - (0.5f * fTemp0));
			fRec2[0] = ((0.5f * (fTemp0 * fRec2[1])) + (((fRec3[1] * fTemp13) + (fTemp7 * fTemp161)) * fTemp207));
			fRec0[(IOTA & 1023)] = fRec2[0];
			fRec81[0] = ((0.5f * (fTemp0 * fRec81[1])) + (fTemp207 * ((fRec4[1] * fTemp13) + (fTemp7 * fTemp155))));
			fRec1[(IOTA & 1023)] = fRec81[0];
			output0[i] = FAUSTFLOAT(fRec0[((IOTA - 0) & 1023)]);
			output1[i] = FAUSTFLOAT(fRec1[((IOTA - 0) & 1023)]);
			fVec0[1] = fVec0[0];
			iVec1[1] = iVec1[0];
			fRec15[1] = fRec15[0];
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			IOTA = (IOTA + 1);
			fRec46[1] = fRec46[0];
			fRec39[1] = fRec39[0];
			fRec37[1] = fRec37[0];
			fRec48[1] = fRec48[0];
			fRec47[1] = fRec47[0];
			fRec38[1] = fRec38[0];
			fRec49[1] = fRec49[0];
			fRec36[1] = fRec36[0];
			fRec34[1] = fRec34[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fRec35[1] = fRec35[0];
			fRec52[1] = fRec52[0];
			fRec33[1] = fRec33[0];
			fRec31[1] = fRec31[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec32[1] = fRec32[0];
			fRec55[1] = fRec55[0];
			fRec30[1] = fRec30[0];
			fRec28[1] = fRec28[0];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fRec29[1] = fRec29[0];
			fRec58[1] = fRec58[0];
			fRec27[1] = fRec27[0];
			fRec25[1] = fRec25[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fRec26[1] = fRec26[0];
			fRec61[1] = fRec61[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec23[1] = fRec23[0];
			fRec64[1] = fRec64[0];
			fRec21[1] = fRec21[0];
			fRec19[1] = fRec19[0];
			fRec66[1] = fRec66[0];
			fRec65[1] = fRec65[0];
			fRec20[1] = fRec20[0];
			fRec67[1] = fRec67[0];
			fRec18[1] = fRec18[0];
			fRec16[1] = fRec16[0];
			fRec69[1] = fRec69[0];
			fRec68[1] = fRec68[0];
			fRec17[1] = fRec17[0];
			fRec14[1] = fRec14[0];
			fRec12[1] = fRec12[0];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec13[1] = fRec13[0];
			fRec72[1] = fRec72[0];
			fRec11[1] = fRec11[0];
			fRec9[1] = fRec9[0];
			fRec74[1] = fRec74[0];
			fRec73[1] = fRec73[0];
			fRec10[1] = fRec10[0];
			fRec75[1] = fRec75[0];
			fRec8[1] = fRec8[0];
			fRec6[1] = fRec6[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec7[1] = fRec7[0];
			fRec78[1] = fRec78[0];
			fRec5[1] = fRec5[0];
			fRec3[1] = fRec3[0];
			fRec80[1] = fRec80[0];
			fRec79[1] = fRec79[0];
			fRec4[1] = fRec4[0];
			fRec2[1] = fRec2[0];
			fRec81[1] = fRec81[0];
			
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
 * @param first - first element of sequence
 * @param last - pointer behind last element of sequence
 * @param Nth - searched element index
 * @param pred - predicate
 * @return pointer to found element or pointer to last, if not found
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
            if (p.size() < 2)
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
    void signalFloatArg(const char* /*name*/, int pos)
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

static void internal_setup(t_symbol* s, bool soundIn = true)
{
    greyhole_faust_class = class_new(s, reinterpret_cast<t_newmethod>(greyhole_faust_new),
        reinterpret_cast<t_method>(greyhole_faust_free),
        sizeof(t_faust_greyhole),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);

    if (soundIn) {
        class_addmethod(greyhole_faust_class, nullfn, &s_signal, A_NULL);
        CLASS_MAINSIGNALIN(greyhole_faust_class, t_faust_greyhole, f);
    }

    class_addmethod(greyhole_faust_class, reinterpret_cast<t_method>(greyhole_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(greyhole_faust_class, reinterpret_cast<t_method>(greyhole_dump_to_console), gensym("dump"), A_NULL);
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

#define EXTERNAL_SETUP_NO_IN(MOD)                      \
    extern "C" void setup_##MOD##0x2egreyhole_tilde()     \
    {                                                  \
        internal_setup(gensym(#MOD ".greyhole~"), false); \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);

#endif
