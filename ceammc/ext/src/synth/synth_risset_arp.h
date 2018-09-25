/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "Risset Arpeggio"
version: "0.1"
Code generated with Faust 2.8.5 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __risset_arp_H__
#define  __risset_arp_H__

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
#include <vector>

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
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
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
#include <cstdlib>


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
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
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
#include "ceammc_externals.h"
#include "m_pd.h"

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
struct risset_arp : public dsp {
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

#include <algorithm>
#include <cmath>
#include <math.h>

static float risset_arp_faustpower2_f(float value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS risset_arp
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class risset_arp : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int iVec0[2];
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fRec1[2];
	int fSamplingFreq;
	float fConst0;
	FAUSTFLOAT fHslider2;
	float fRec4[2];
	FAUSTFLOAT fHslider3;
	float fRec5[2];
	float fRec6[2];
	float fRec3[2];
	float fRec2[2];
	FAUSTFLOAT fHslider4;
	float fRec7[2];
	FAUSTFLOAT fHslider5;
	float fRec8[2];
	FAUSTFLOAT fHslider6;
	float fRec9[2];
	FAUSTFLOAT fHslider7;
	float fRec10[2];
	FAUSTFLOAT fHslider8;
	float fRec11[2];
	FAUSTFLOAT fHslider9;
	float fRec12[2];
	FAUSTFLOAT fHslider10;
	float fRec13[2];
	float fRec16[2];
	float fRec15[2];
	float fRec14[2];
	float fRec19[2];
	float fRec18[2];
	float fRec17[2];
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fRec25[2];
	float fRec24[2];
	float fRec23[2];
	float fRec28[2];
	float fRec27[2];
	float fRec26[2];
	float fRec31[2];
	float fRec30[2];
	float fRec29[2];
	float fRec34[2];
	float fRec33[2];
	float fRec32[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
		m->declare("copyright", "Oliver Larkin");
		m->declare("description", "Jean Claude Risset's Harmonic Arpeggio Effect");
		m->declare("filename", "synth_risset_arp");
		m->declare("licence", "GPL");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "Risset Arpeggio");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "0.1");
	}

	virtual int getNumInputs() {
		return 0;
		
	}
	virtual int getNumOutputs() {
		return 2;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
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
		fConst0 = (6.28318548f / std::min(192000.0f, std::max(1.0f, float(fSamplingFreq))));
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(1.0f);
		fHslider2 = FAUSTFLOAT(100.0f);
		fHslider3 = FAUSTFLOAT(5.0f);
		fHslider4 = FAUSTFLOAT(1.0f);
		fHslider5 = FAUSTFLOAT(1.0f);
		fHslider6 = FAUSTFLOAT(1.0f);
		fHslider7 = FAUSTFLOAT(1.0f);
		fHslider8 = FAUSTFLOAT(1.0f);
		fHslider9 = FAUSTFLOAT(1.0f);
		fHslider10 = FAUSTFLOAT(1.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec0[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec1[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec4[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec5[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec6[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec3[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec2[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec7[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec8[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec9[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec10[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec11[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec12[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec13[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec16[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec15[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec14[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec19[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec18[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec17[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec22[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec21[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec20[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec25[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec24[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec23[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec28[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec27[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec26[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec31[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec30[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec29[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec34[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec33[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec32[l35] = 0.0f;
			
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
	
	virtual risset_arp* clone() {
		return new risset_arp();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Risset Arpeggio");
		ui_interface->addHorizontalSlider("detune", &fHslider3, 5.0f, 0.0f, 1000.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("freq", &fHslider2, 100.0f, 40.0f, 500.0f, 1.0f);
		ui_interface->addHorizontalSlider("harmonic1", &fHslider0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("harmonic2", &fHslider8, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("harmonic3", &fHslider4, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("harmonic4", &fHslider7, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("harmonic5", &fHslider5, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("harmonic6", &fHslider9, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("harmonic7", &fHslider1, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("harmonic8", &fHslider6, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("spread", &fHslider10, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (0.00100000005f * float(fHslider0));
		float fSlow1 = (0.00100000005f * float(fHslider1));
		float fSlow2 = (0.00100000005f * float(fHslider2));
		float fSlow3 = (9.99999975e-06f * float(fHslider3));
		float fSlow4 = (0.00100000005f * float(fHslider4));
		float fSlow5 = (0.00100000005f * float(fHslider5));
		float fSlow6 = (0.00100000005f * float(fHslider6));
		float fSlow7 = (0.00100000005f * float(fHslider7));
		float fSlow8 = (0.00100000005f * float(fHslider8));
		float fSlow9 = (0.00100000005f * float(fHslider9));
		float fSlow10 = (0.00100000005f * float(fHslider10));
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec0[0] = (fSlow0 + (0.999000013f * fRec0[1]));
			fRec1[0] = (fSlow1 + (0.999000013f * fRec1[1]));
			fRec4[0] = (fSlow2 + (0.999000013f * fRec4[1]));
			fRec5[0] = (fSlow3 + (0.999000013f * fRec5[1]));
			float fTemp0 = (fRec4[0] + fRec5[0]);
			fRec6[0] = ((fConst0 * ((0.0f - fRec2[1]) * fTemp0)) + fRec6[1]);
			int iTemp1 = (1 - iVec0[1]);
			fRec3[0] = ((fRec3[1] + (fConst0 * (fTemp0 * fRec6[0]))) + float(iTemp1));
			fRec2[0] = fRec3[0];
			float fTemp2 = risset_arp_faustpower2_f(fRec2[0]);
			float fTemp3 = (1.0f - fTemp2);
			float fTemp4 = (8.0f * fTemp3);
			float fTemp5 = (fTemp2 * (fTemp4 + 2.0f));
			float fTemp6 = (fTemp5 + -2.0f);
			fRec7[0] = (fSlow4 + (0.999000013f * fRec7[1]));
			float fTemp7 = ((2.0f * fTemp2) + -1.0f);
			fRec8[0] = (fSlow5 + (0.999000013f * fRec8[1]));
			float fTemp8 = (1.0f - (2.0f * fTemp6));
			fRec9[0] = (fSlow6 + (0.999000013f * fRec9[1]));
			fRec10[0] = (fSlow7 + (0.999000013f * fRec10[1]));
			fRec11[0] = (fSlow8 + (0.999000013f * fRec11[1]));
			fRec12[0] = (fSlow9 + (0.999000013f * fRec12[1]));
			float fTemp9 = (((((((((fRec0[0] + (fRec1[0] * ((2.0f * ((((4.0f * (1.0f - fTemp6)) + (16.0f * fTemp3)) * fTemp2) + -3.0f)) + -1.0f))) + (fRec7[0] * ((2.0f * fTemp7) + -1.0f))) + (fRec8[0] * fTemp8)) * fRec2[0]) + (fRec9[0] * (((0.0f - ((4.0f * (6.0f - ((((24.0f * fTemp3) + (4.0f * (3.0f - fTemp5))) + 2.0f) * fTemp2))) + fTemp4)) * fTemp2) + 1.0f))) + (fRec10[0] * (1.0f - (8.0f * (fTemp2 * fTemp3))))) + (fRec11[0] * fTemp7)) + (fRec12[0] * ((fTemp2 * (fTemp4 + (2.0f * fTemp8))) + -1.0f))) + 1.0f);
			fRec13[0] = (fSlow10 + (0.999000013f * fRec13[1]));
			float fTemp10 = (0.214285716f * fRec13[0]);
			float fTemp11 = (2.0f * fRec5[0]);
			float fTemp12 = (fRec4[0] + fTemp11);
			fRec16[0] = ((fConst0 * ((0.0f - fRec14[1]) * fTemp12)) + fRec16[1]);
			fRec15[0] = ((fRec15[1] + (fConst0 * (fTemp12 * fRec16[0]))) + float(iTemp1));
			fRec14[0] = fRec15[0];
			float fTemp13 = risset_arp_faustpower2_f(fRec14[0]);
			float fTemp14 = ((2.0f * fTemp13) + -1.0f);
			float fTemp15 = (1.0f - fTemp13);
			float fTemp16 = (8.0f * fTemp15);
			float fTemp17 = (fTemp13 * (-2.0f - fTemp16));
			float fTemp18 = (fTemp17 + 2.0f);
			float fTemp19 = ((2.0f * fTemp18) + 1.0f);
			float fTemp20 = ((((((fRec11[0] * fTemp14) + ((((fRec0[0] + (fRec1[0] * (-1.0f - (2.0f * (((0.0f - ((4.0f * (fTemp18 + 1.0f)) + (16.0f * fTemp15))) * fTemp13) + 3.0f))))) + (fRec7[0] * ((2.0f * fTemp14) + -1.0f))) + (fRec8[0] * fTemp19)) * fRec14[0])) + (fRec10[0] * (1.0f - (8.0f * (fTemp13 * fTemp15))))) + (fRec9[0] * (((0.0f - ((4.0f * (((-2.0f - ((24.0f * fTemp15) + (4.0f * (fTemp17 + 3.0f)))) * fTemp13) + 6.0f)) + fTemp16)) * fTemp13) + 1.0f))) + (fRec12[0] * ((fTemp13 * (fTemp16 + (2.0f * fTemp19))) + -1.0f))) + 1.0f);
			float fTemp21 = (0.357142866f * fRec13[0]);
			float fTemp22 = (3.0f * fRec5[0]);
			float fTemp23 = (fRec4[0] + fTemp22);
			fRec19[0] = ((fConst0 * ((0.0f - fRec17[1]) * fTemp23)) + fRec19[1]);
			fRec18[0] = ((fRec18[1] + (fConst0 * (fTemp23 * fRec19[0]))) + float(iTemp1));
			fRec17[0] = fRec18[0];
			float fTemp24 = risset_arp_faustpower2_f(fRec17[0]);
			float fTemp25 = (1.0f - fTemp24);
			float fTemp26 = (8.0f * fTemp25);
			float fTemp27 = (fTemp24 * (fTemp26 + 2.0f));
			float fTemp28 = (fTemp27 + -2.0f);
			float fTemp29 = ((2.0f * fTemp24) + -1.0f);
			float fTemp30 = (1.0f - (2.0f * fTemp28));
			float fTemp31 = (((((((((fRec0[0] + (fRec1[0] * ((2.0f * ((((4.0f * (1.0f - fTemp28)) + (16.0f * fTemp25)) * fTemp24) + -3.0f)) + -1.0f))) + (fRec7[0] * ((2.0f * fTemp29) + -1.0f))) + (fRec8[0] * fTemp30)) * fRec17[0]) + (fRec9[0] * (((0.0f - ((4.0f * (6.0f - ((((24.0f * fTemp25) + (4.0f * (3.0f - fTemp27))) + 2.0f) * fTemp24))) + fTemp26)) * fTemp24) + 1.0f))) + (fRec10[0] * (1.0f - (8.0f * (fTemp24 * fTemp25))))) + (fRec11[0] * fTemp29)) + (fRec12[0] * ((fTemp24 * (fTemp26 + (2.0f * fTemp30))) + -1.0f))) + 1.0f);
			float fTemp32 = (fRec13[0] + 1.0f);
			float fTemp33 = (0.5f * (fRec13[0] + -1.0f));
			float fTemp34 = (fRec4[0] - (4.0f * fRec5[0]));
			fRec22[0] = (fRec22[1] + (fConst0 * (fTemp34 * (0.0f - fRec20[1]))));
			fRec21[0] = ((fRec21[1] + (fConst0 * (fTemp34 * fRec22[0]))) + float(iTemp1));
			fRec20[0] = fRec21[0];
			float fTemp35 = risset_arp_faustpower2_f(fRec20[0]);
			float fTemp36 = (1.0f - fTemp35);
			float fTemp37 = (8.0f * fTemp36);
			float fTemp38 = (fTemp35 * (fTemp37 + 2.0f));
			float fTemp39 = (fTemp38 + -2.0f);
			float fTemp40 = (1.0f - (2.0f * fTemp39));
			float fTemp41 = ((2.0f * fTemp35) + -1.0f);
			float fTemp42 = (((fRec12[0] * ((fTemp35 * (fTemp37 + (2.0f * fTemp40))) + -1.0f)) + ((fRec20[0] * ((fRec8[0] * fTemp40) + ((fRec1[0] * ((2.0f * ((fTemp35 * ((16.0f * fTemp36) + (4.0f * (1.0f - fTemp39)))) + -3.0f)) + -1.0f)) + (fRec0[0] + (((2.0f * fTemp41) + -1.0f) * fRec7[0]))))) + ((fRec9[0] * ((fTemp35 * (0.0f - (fTemp37 + (2.0f * (0.0f - (2.0f * ((fTemp35 * ((24.0f * fTemp36) + (2.0f * ((0.0f - (2.0f * (fTemp38 + -3.0f))) + 1.0f)))) + -6.0f))))))) + 1.0f)) + ((fRec10[0] * (1.0f - (8.0f * (fTemp35 * fTemp36)))) + (fTemp41 * fRec11[0]))))) + 1.0f);
			float fTemp43 = (0.357142866f * fRec13[0]);
			float fTemp44 = (fRec4[0] - fTemp22);
			fRec25[0] = (fRec25[1] + (fConst0 * ((0.0f - fRec23[1]) * fTemp44)));
			fRec24[0] = ((fRec24[1] + (fConst0 * (fTemp44 * fRec25[0]))) + float(iTemp1));
			fRec23[0] = fRec24[0];
			float fTemp45 = risset_arp_faustpower2_f(fRec23[0]);
			float fTemp46 = (fTemp45 + -1.0f);
			float fTemp47 = (4.0f * fTemp46);
			float fTemp48 = ((fTemp45 * (2.0f - fTemp47)) + -2.0f);
			float fTemp49 = ((2.0f * (fTemp45 * (1.0f - (4.0f * fTemp48)))) + -1.0f);
			float fTemp50 = ((2.0f * fTemp45) + -1.0f);
			float fTemp51 = (4.0f * ((fTemp45 * (1.0f - fTemp47)) + -1.0f));
			float fTemp52 = (((((fRec10[0] * (1.0f - (2.0f * (fTemp45 * (0.0f - fTemp47))))) + ((fRec12[0] * fTemp49) + (fRec11[0] * fTemp50))) + (fRec23[0] * ((fRec7[0] * ((2.0f * fTemp50) + -1.0f)) + ((fRec8[0] * (1.0f - fTemp51)) + (fRec0[0] + (fRec1[0] * ((fTemp51 + (2.0f * fTemp49)) + -1.0f))))))) + (fRec9[0] * (1.0f - (2.0f * (fTemp45 * (0.0f - (4.0f * ((fTemp45 * ((4.0f * (1.0f - fTemp48)) - (8.0f * fTemp46))) + -4.0f)))))))) + 1.0f);
			float fTemp53 = (0.214285716f * fRec13[0]);
			float fTemp54 = (fRec4[0] - fTemp11);
			fRec28[0] = (fRec28[1] + (fConst0 * ((0.0f - fRec26[1]) * fTemp54)));
			fRec27[0] = ((fRec27[1] + (fConst0 * (fTemp54 * fRec28[0]))) + float(iTemp1));
			fRec26[0] = fRec27[0];
			float fTemp55 = risset_arp_faustpower2_f(fRec26[0]);
			float fTemp56 = (fTemp55 + -1.0f);
			float fTemp57 = (4.0f * fTemp56);
			float fTemp58 = ((fTemp55 * (2.0f - fTemp57)) + -2.0f);
			float fTemp59 = ((2.0f * (fTemp55 * (1.0f - (4.0f * fTemp58)))) + -1.0f);
			float fTemp60 = ((2.0f * fTemp55) + -1.0f);
			float fTemp61 = (4.0f * ((fTemp55 * (1.0f - fTemp57)) + -1.0f));
			float fTemp62 = (((((fRec10[0] * (1.0f - (2.0f * (fTemp55 * (0.0f - fTemp57))))) + ((fRec12[0] * fTemp59) + (fRec11[0] * fTemp60))) + (fRec26[0] * ((fRec7[0] * ((2.0f * fTemp60) + -1.0f)) + ((fRec8[0] * (1.0f - fTemp61)) + (fRec0[0] + (fRec1[0] * ((fTemp61 + (2.0f * fTemp59)) + -1.0f))))))) + (fRec9[0] * (1.0f - (2.0f * (fTemp55 * (0.0f - (4.0f * ((fTemp55 * ((4.0f * (1.0f - fTemp58)) - (8.0f * fTemp56))) + -4.0f)))))))) + 1.0f);
			float fTemp63 = (0.0714285746f * fRec13[0]);
			float fTemp64 = (fRec4[0] - fRec5[0]);
			fRec31[0] = (fRec31[1] + (fConst0 * ((0.0f - fRec29[1]) * fTemp64)));
			fRec30[0] = ((fRec30[1] + (fConst0 * (fTemp64 * fRec31[0]))) + float(iTemp1));
			fRec29[0] = fRec30[0];
			float fTemp65 = risset_arp_faustpower2_f(fRec29[0]);
			float fTemp66 = (1.0f - fTemp65);
			float fTemp67 = (8.0f * fTemp66);
			float fTemp68 = (fTemp65 * (fTemp67 + 2.0f));
			float fTemp69 = (fTemp68 + -2.0f);
			float fTemp70 = (1.0f - (2.0f * fTemp69));
			float fTemp71 = ((2.0f * fTemp65) + -1.0f);
			float fTemp72 = (((((fRec12[0] * ((fTemp65 * (fTemp67 + (2.0f * fTemp70))) + -1.0f)) + ((fRec10[0] * (1.0f - (8.0f * (fTemp65 * fTemp66)))) + (fRec11[0] * fTemp71))) + (fRec29[0] * (((fRec0[0] + (fRec7[0] * ((2.0f * fTemp71) + -1.0f))) + (fRec8[0] * fTemp70)) + (fRec1[0] * ((2.0f * ((fTemp65 * ((16.0f * fTemp66) + (4.0f * (1.0f - fTemp69)))) + -3.0f)) + -1.0f))))) + (fRec9[0] * ((fTemp65 * (0.0f - (fTemp67 + (4.0f * (6.0f - (fTemp65 * (((4.0f * (3.0f - fTemp68)) + (24.0f * fTemp66)) + 2.0f))))))) + 1.0f))) + 1.0f);
			float fTemp73 = (0.0714285746f * fRec13[0]);
			fRec34[0] = (fRec34[1] + (fConst0 * (fRec4[0] * (0.0f - fRec32[1]))));
			fRec33[0] = ((fRec33[1] + (fConst0 * (fRec4[0] * fRec34[0]))) + float(iTemp1));
			fRec32[0] = fRec33[0];
			float fTemp74 = risset_arp_faustpower2_f(fRec32[0]);
			float fTemp75 = (1.0f - fTemp74);
			float fTemp76 = (8.0f * fTemp75);
			float fTemp77 = (fTemp74 * (fTemp76 + 2.0f));
			float fTemp78 = (fTemp77 + -2.0f);
			float fTemp79 = (1.0f - (2.0f * fTemp78));
			float fTemp80 = ((2.0f * fTemp74) + -1.0f);
			float fTemp81 = (((((fRec12[0] * ((fTemp74 * (fTemp76 + (2.0f * fTemp79))) + -1.0f)) + ((fRec10[0] * (1.0f - (8.0f * (fTemp74 * fTemp75)))) + (fRec11[0] * fTemp80))) + (fRec32[0] * (((fRec0[0] + (fRec7[0] * ((2.0f * fTemp80) + -1.0f))) + (fRec8[0] * fTemp79)) + (fRec1[0] * ((2.0f * ((fTemp74 * ((16.0f * fTemp75) + (4.0f * (1.0f - fTemp78)))) + -3.0f)) + -1.0f))))) + (fRec9[0] * ((fTemp74 * (0.0f - (fTemp76 + (4.0f * (6.0f - (fTemp74 * (((4.0f * (3.0f - fTemp77)) + (24.0f * fTemp75)) + 2.0f))))))) + 1.0f))) + 1.0f);
			output0[i] = FAUSTFLOAT((0.0500000007f * ((((((((fTemp9 * (fTemp10 + 0.5f)) + (fTemp20 * (fTemp21 + 0.5f))) + (0.5f * (fTemp31 * fTemp32))) + ((0.0f - fTemp33) * fTemp42)) + ((0.5f - fTemp43) * fTemp52)) + ((0.5f - fTemp53) * fTemp62)) + ((0.5f - fTemp63) * fTemp72)) + ((fTemp73 + 0.5f) * fTemp81))));
			output1[i] = FAUSTFLOAT((0.0500000007f * (((((((((fTemp33 + 1.0f) * fTemp42) + ((fTemp43 + 0.5f) * fTemp52)) + ((fTemp53 + 0.5f) * fTemp62)) + ((fTemp63 + 0.5f) * fTemp72)) + ((0.5f - fTemp73) * fTemp81)) + (fTemp9 * (0.5f - fTemp10))) + (fTemp20 * (0.5f - fTemp21))) + (fTemp31 * (1.0f - (0.5f * fTemp32))))));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec16[1] = fRec16[0];
			fRec15[1] = fRec15[0];
			fRec14[1] = fRec14[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			
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
static t_class* risset_arp_faust_class;
#define FAUST_EXT t_faust_risset_arp
#define FAUST_EXT_CLASS risset_arp_faust_class
// clang-format on

template <class T>
class _risset_arp_UI : public UI {
};
typedef _risset_arp_UI<risset_arp> risset_arp_UI;

struct t_faust_risset_arp {
    t_object x_obj;
#ifdef __MINGW32__
    /* This seems to be necessary as some as yet undetermined Pd routine seems
     to write past the end of x_obj on Windows. */
    int fence; /* dummy field (not used) */
#endif
    risset_arp* dsp;
    PdUI<risset_arp_UI>* ui;
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
    t_faust_risset_arp* x = reinterpret_cast<t_faust_risset_arp*>(w[1]);
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

static void risset_arp_faust_dsp(t_faust_risset_arp* x, t_signal** sp)
{
    const int n = sp[0]->s_n;
    const int sr = static_cast<int>(sp[0]->s_sr);

    if (x->rate <= 0) {
        /* default sample rate is whatever Pd tells us */
        PdUI<risset_arp_UI>* ui = x->ui;
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

static void risset_arp_dump_to_console(t_faust_risset_arp* x)
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

static void risset_arp_faust_any(t_faust_risset_arp* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!x->dsp)
        return;

    PdUI<risset_arp_UI>* ui = x->ui;
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

static void faust_free_dsp(t_faust_risset_arp* x)
{
    delete x->dsp;
    x->dsp = NULL;
}

static void faust_free_ui(t_faust_risset_arp* x)
{
    delete x->ui;
    x->ui = NULL;
}

static void faust_free_inputs(t_faust_risset_arp* x)
{
    if (x->inputs)
        free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust_risset_arp* x)
{
    if (x->outputs)
        free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust_risset_arp* x)
{
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i])
                free(x->buf[i]);
        }

        free(x->buf);
    }
}

static void risset_arp_faust_free(t_faust_risset_arp* x)
{
    faust_free_dsp(x);
    faust_free_ui(x);
    faust_free_inputs(x);
    faust_free_outputs(x);
    faust_free_buf(x);
}

static bool faust_init_inputs(t_faust_risset_arp* x)
{
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if (x->inputs == NULL) {
            error("[%s] faust_init_inputs failed", sym(risset_arp));
            return false;
        }
    }

    // creating sound inlets (except the first one!)
    for (int i = 0; i < (x->n_in - 1); i++) {
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    return true;
}

static bool faust_init_outputs(t_faust_risset_arp* x, bool info_outlet)
{
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->outputs == NULL) {
            error("[%s] faust_init_outputs failed", sym(risset_arp));
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->buf == NULL) {
            error("[%s] faust_init_outputs failed", sym(risset_arp));
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

static bool faust_new_internal(t_faust_risset_arp* x, const std::string& objId = "", bool info_outlet = true)
{
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new risset_arp();
    x->ui = new PdUI<risset_arp_UI>(sym(risset_arp), objId);

    if (!faust_init_inputs(x)) {
        risset_arp_faust_free(x);
        return false;
    }

    if (!faust_init_outputs(x, info_outlet)) {
        risset_arp_faust_free(x);
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
 * @return true if given atom is a property
 */
static bool atom_is_property(const t_atom& a)
{
    switch (a.a_type) {
    case A_DEFSYMBOL:
    case A_SYMBOL:
        return a.a_w.w_symbol->s_name[0] == '@';
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
    t_faust_risset_arp* x_;
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
    PdArgParser(t_faust_risset_arp* x, int argc, t_atom* argv, bool info_outlet = true)
        : x_(x)
        , argc_(0)
        , argv_(argv)
        , control_outlet_(info_outlet)
    {
        const char* id = NULL;
        std::string objId;

        int first_prop_idx = argc;
        for (int i = 0; i < argc; i++) {
            if (atom_is_property(argv[i]))
                first_prop_idx = i;
        }

        // store argument count (without properties)
        argc_ = first_prop_idx;

        if (get_nth_symbol_arg(argc_, argv_, 1, &id))
            objId = id;

        // init error
        if (!faust_new_internal(x, objId, control_outlet_)) {
            this->x_ = NULL;
        }

        // process properties
        std::deque<ceammc::AtomList> props = ceammc::AtomList(argc, argv).properties();
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

    t_faust_risset_arp* pd_obj()
    {
        return this->x_;
    }
};

static void* risset_arp_faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s, bool soundIn = true)
{
    risset_arp_faust_class = class_new(s, reinterpret_cast<t_newmethod>(risset_arp_faust_new),
        reinterpret_cast<t_method>(risset_arp_faust_free),
        sizeof(t_faust_risset_arp),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);

    if (soundIn) {
        class_addmethod(risset_arp_faust_class, nullfn, &s_signal, A_NULL);
        CLASS_MAINSIGNALIN(risset_arp_faust_class, t_faust_risset_arp, f);
    }

    class_addmethod(risset_arp_faust_class, reinterpret_cast<t_method>(risset_arp_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(risset_arp_faust_class, reinterpret_cast<t_method>(risset_arp_dump_to_console), gensym("dump"), A_NULL);
    class_addanything(risset_arp_faust_class, risset_arp_faust_any);
    ceammc::register_faust_external(risset_arp_faust_class);
}

#define EXTERNAL_NEW void* risset_arp_faust_new(t_symbol*, int argc, t_atom* argv)

#define EXTERNAL_SIMPLE_NEW()                                                           \
    static void* risset_arp_faust_new(t_symbol*, int argc, t_atom* argv)                     \
    {                                                                                   \
        t_faust_risset_arp* x = reinterpret_cast<t_faust_risset_arp*>(pd_new(risset_arp_faust_class)); \
        PdArgParser p(x, argc, argv, false);                                            \
        return p.pd_obj();                                                              \
    }

#define EXTERNAL_SETUP(MOD)                        \
    extern "C" void setup_##MOD##0x2erisset_arp_tilde() \
    {                                              \
        internal_setup(gensym(#MOD ".risset_arp~"));    \
    }

#define EXTERNAL_SETUP_NO_IN(MOD)                      \
    extern "C" void setup_##MOD##0x2erisset_arp_tilde()     \
    {                                                  \
        internal_setup(gensym(#MOD ".risset_arp~"), false); \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);

#endif
