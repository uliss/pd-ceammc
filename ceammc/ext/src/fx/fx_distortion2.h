/* ------------------------------------------------------------
author: "brummer"
copyright: "(c)brummer 2008"
license: "BSD"
name: "fx.distortion2"
version: "0.01"
Code generated with Faust 2.8.5 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_distortion2_H__
#define  __fx_distortion2_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
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


#include "ceammc_faust.h"

using namespace ceammc::faust;

#ifdef FAUST_MACRO
// clang-format off
// clang-format on
#endif

#define sym(name) xsym(name)
#define xsym(name) #name

// clang-format off
#ifndef FAUST_MACRO
struct fx_distortion2 : public dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <math.h>

static float fx_distortion2_faustpower2_f(float value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_distortion2
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_distortion2 : public dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSamplingFreq;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fVslider0;
	int iVec0[2];
	FAUSTFLOAT fVslider1;
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fEntry1;
	float fRec12[2];
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fVslider4;
	int IOTA;
	float fVec1[4096];
	FAUSTFLOAT fVslider5;
	float fRec13[2];
	float fVec2[2];
	float fRec11[2];
	float fRec10[2];
	float fVec3[2];
	float fRec9[2];
	float fVec4[2];
	float fRec8[2];
	float fRec7[3];
	float fRec6[3];
	float fConst10;
	float fConst11;
	float fRec5[3];
	float fConst12;
	float fConst13;
	FAUSTFLOAT fVslider6;
	float fRec14[2];
	float fRec4[3];
	float fVec5[2];
	float fRec3[2];
	float fVec6[2];
	float fRec2[2];
	float fRec1[3];
	float fRec0[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("HighShelf.dsp/id", "HighShelf");
		m->declare("author", "brummer");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("copyright", "(c)brummer 2008");
		m->declare("filename", "fx_distortion2");
		m->declare("filter.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("filter.lib/copyright", "Julius O. Smith III");
		m->declare("filter.lib/deprecated", "This library is deprecated and is not maintained anymore. It will be removed in August 2017.");
		m->declare("filter.lib/license", "STK-4.3");
		m->declare("filter.lib/name", "Faust Filter Library");
		m->declare("filter.lib/reference", "https://ccrma.stanford.edu/~jos/filters/");
		m->declare("filter.lib/version", "1.29");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("license", "BSD");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/deprecated", "This library is deprecated and is not maintained anymore. It will be removed in August 2017.");
		m->declare("math.lib/license", "LGPL with exception");
		m->declare("math.lib/name", "Math Library");
		m->declare("math.lib/version", "1.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("maxmsp.lib/author", "GRAME");
		m->declare("maxmsp.lib/copyright", "GRAME");
		m->declare("maxmsp.lib/license", "LGPL");
		m->declare("maxmsp.lib/name", "MaxMSP compatibility Library");
		m->declare("maxmsp.lib/version", "1.1");
		m->declare("misceffects.lib/name", "Faust Math Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("music.lib/author", "GRAME");
		m->declare("music.lib/copyright", "GRAME");
		m->declare("music.lib/deprecated", "This library is deprecated and is not maintained anymore. It will be removed in August 2017.");
		m->declare("music.lib/license", "LGPL with exception");
		m->declare("music.lib/name", "Music Library");
		m->declare("music.lib/version", "1.0");
		m->declare("name", "fx.distortion2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "0.01");
	}

	virtual int getNumInputs() {
		return 1;
		
	}
	virtual int getNumOutputs() {
		return 1;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
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
		fConst0 = std::min(192000.0f, std::max(1.0f, float(fSamplingFreq)));
		fConst1 = (3.14159274f / fConst0);
		fConst2 = (6.28318548f * (std::max(0.0f, ((0.5f * fConst0) + -100.0f)) / fConst0));
		fConst3 = std::cos(fConst2);
		fConst4 = (0.683772206f * fConst3);
		fConst5 = (0.00562341325f * std::sin(fConst2));
		fConst6 = (fConst4 + fConst5);
		fConst7 = (1.0f / (fConst6 + 1.31622779f));
		fConst8 = (1.31622779f * fConst3);
		fConst9 = (0.0f - (0.632455528f * (fConst8 + -0.683772206f)));
		fConst10 = (fConst4 + (1.31622779f - fConst5));
		fConst11 = (2.0f * (-0.683772206f - fConst8));
		fConst12 = (fConst5 + (1.31622779f - fConst4));
		fConst13 = (1.31622779f - fConst6);
		
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fVslider0 = FAUSTFLOAT(5000.0f);
		fVslider1 = FAUSTFLOAT(130.0f);
		fVslider2 = FAUSTFLOAT(0.01f);
		fVslider3 = FAUSTFLOAT(0.64000000000000001f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fEntry0 = FAUSTFLOAT(130.0f);
		fEntry1 = FAUSTFLOAT(5000.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fVslider4 = FAUSTFLOAT(0.12f);
		fVslider5 = FAUSTFLOAT(1.0f);
		fVslider6 = FAUSTFLOAT(2.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec12[l1] = 0.0f;
			
		}
		IOTA = 0;
		for (int l2 = 0; (l2 < 4096); l2 = (l2 + 1)) {
			fVec1[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec13[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec2[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec11[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec10[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fVec3[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec9[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fVec4[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec8[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec7[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec6[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec5[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec14[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec4[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fVec5[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec3[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fVec6[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec2[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec1[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec0[l21] = 0.0f;
			
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
	
	virtual fx_distortion2* clone() {
		return new fx_distortion2();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.distortion2");
		ui_interface->addVerticalSlider("drive", &fVslider3, 0.639999986f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fVslider6, "unit", "db");
		ui_interface->addVerticalSlider("gain", &fVslider6, 2.0f, -10.0f, 10.0f, 0.100000001f);
		ui_interface->addVerticalSlider("level", &fVslider2, 0.00999999978f, 0.0f, 0.5f, 0.00999999978f);
		ui_interface->openHorizontalBox("low_highcutoff");
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("sp_high_freq", &fVslider0, 5000.0f, 1000.0f, 12000.0f, 10.0f);
		ui_interface->declare(&fVslider1, "unit", "Hz");
		ui_interface->addVerticalSlider("sp_low_freq", &fVslider1, 130.0f, 20.0f, 1000.0f, 10.0f);
		ui_interface->addCheckButton("sp_on_off", &fCheckbox0);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("low_highpass");
		ui_interface->declare(&fEntry0, "unit", "Hz");
		ui_interface->addNumEntry("lh_high_freq", &fEntry0, 130.0f, 20.0f, 7040.0f, 10.0f);
		ui_interface->declare(&fEntry1, "unit", "Hz");
		ui_interface->addNumEntry("lh_low_freq", &fEntry1, 5000.0f, 20.0f, 12000.0f, 10.0f);
		ui_interface->addCheckButton("lh_on_off", &fCheckbox1);
		ui_interface->closeBox();
		ui_interface->addCheckButton("res_on_off", &fCheckbox2);
		ui_interface->addVerticalSlider("trigger", &fVslider4, 0.119999997f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addVerticalSlider("vibrato", &fVslider5, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = std::tan((fConst1 * float(fVslider0)));
		float fSlow2 = (1.0f / fSlow1);
		float fSlow3 = (1.0f / (((fSlow2 + 0.765366852f) / fSlow1) + 1.0f));
		float fSlow4 = (1.0f / (((fSlow2 + 1.84775901f) / fSlow1) + 1.0f));
		float fSlow5 = (fConst1 * float(fVslider1));
		float fSlow6 = (1.0f / (fSlow5 + 1.0f));
		float fSlow7 = (1.0f - fSlow5);
		float fSlow8 = float(fVslider2);
		float fSlow9 = (fConst7 * std::pow(10.0f, (2.0f * float(fVslider3))));
		int iSlow10 = int(float(fCheckbox1));
		float fSlow11 = (1.0f / std::tan((fConst1 * float(fEntry0))));
		float fSlow12 = (fSlow11 + 1.0f);
		float fSlow13 = (0.0f - ((1.0f - fSlow11) / fSlow12));
		float fSlow14 = (1.0f / fSlow12);
		float fSlow15 = (1.0f / std::tan((fConst1 * float(fEntry1))));
		float fSlow16 = (fSlow15 + 1.0f);
		float fSlow17 = (0.0f - ((1.0f - fSlow15) / fSlow16));
		float fSlow18 = (1.0f / fSlow16);
		int iSlow19 = int(float(fCheckbox2));
		float fSlow20 = (1.0f - float(fVslider4));
		float fSlow21 = float(fVslider5);
		int iSlow22 = (int((fSlow21 + -1.0f)) & 4095);
		int iSlow23 = (int(fSlow21) & 4095);
		float fSlow24 = (0.0f - fSlow11);
		float fSlow25 = (((fSlow2 + -1.84775901f) / fSlow1) + 1.0f);
		float fSlow26 = (2.0f * (1.0f - (1.0f / fx_distortion2_faustpower2_f(fSlow1))));
		float fSlow27 = (((fSlow2 + -0.765366852f) / fSlow1) + 1.0f);
		float fSlow28 = (0.00100000005f * std::pow(10.0f, (0.0500000007f * (float(fVslider6) + -10.0f))));
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec12[0] = ((9.99999968e-21f * float((1 - iVec0[1]))) - fRec12[1]);
			float fTemp0 = float(input0[i]);
			float fTemp1 = (fTemp0 + (fSlow20 * fRec13[1]));
			fVec1[(IOTA & 4095)] = fTemp1;
			fRec13[0] = (0.5f * (fVec1[((IOTA - iSlow22) & 4095)] + fVec1[((IOTA - iSlow23) & 4095)]));
			float fTemp2 = (fRec12[0] + (iSlow19?fRec13[0]:fTemp0));
			float fTemp3 = (fRec12[0] + (iSlow10?0.0f:fTemp2));
			fVec2[0] = fTemp3;
			fRec11[0] = ((fSlow17 * fRec11[1]) + (fSlow18 * (fTemp3 + fVec2[1])));
			fRec10[0] = ((fSlow13 * fRec10[1]) + (fSlow14 * ((fSlow11 * fRec11[0]) + (fSlow24 * fRec11[1]))));
			float fTemp4 = ((iSlow10?fTemp2:fRec10[0]) + 9.99999968e-21f);
			float fTemp5 = (iSlow0?0.0f:fTemp4);
			fVec3[0] = (fSlow6 * (fRec12[0] + fTemp5));
			fRec9[0] = ((fSlow6 * (fTemp5 + ((fSlow7 * fRec9[1]) + fRec12[0]))) - fVec3[1]);
			fVec4[0] = (fSlow6 * fRec9[0]);
			fRec8[0] = ((fSlow6 * ((fSlow7 * fRec8[1]) + fRec9[0])) - fVec4[1]);
			fRec7[0] = (fRec8[0] - (fSlow4 * ((fSlow25 * fRec7[2]) + (fSlow26 * fRec7[1]))));
			fRec6[0] = ((fSlow4 * (fRec7[0] + (fRec7[2] + (2.0f * fRec7[1])))) - (fSlow3 * ((fSlow26 * fRec6[1]) + (fSlow27 * fRec6[2]))));
			fRec5[0] = ((iSlow0?fTemp4:(fSlow3 * (fRec6[0] + (fRec6[2] + (2.0f * fRec6[1]))))) - (fConst7 * ((fConst10 * fRec5[2]) + (fConst11 * fRec5[1]))));
			float fTemp6 = std::max(-1.0f, std::min(1.0f, (fSlow8 + (fSlow9 * ((fConst9 * fRec5[1]) + (0.316227764f * ((fConst12 * fRec5[0]) + (fConst13 * fRec5[2]))))))));
			fRec14[0] = (fSlow28 + (0.999000013f * fRec14[1]));
			fRec4[0] = (((fTemp6 * (1.0f - (0.333333343f * fx_distortion2_faustpower2_f(fTemp6)))) * fRec14[0]) - (fConst7 * ((fConst10 * fRec4[2]) + (fConst11 * fRec4[1]))));
			float fTemp7 = (fConst7 * ((fConst9 * fRec4[1]) + (0.316227764f * ((fConst12 * fRec4[0]) + (fConst13 * fRec4[2])))));
			float fTemp8 = (iSlow0?0.0f:fTemp7);
			fVec5[0] = (fSlow6 * (fRec12[0] + fTemp8));
			fRec3[0] = ((fSlow6 * (fTemp8 + ((fSlow7 * fRec3[1]) + fRec12[0]))) - fVec5[1]);
			fVec6[0] = (fSlow6 * fRec3[0]);
			fRec2[0] = ((fSlow6 * ((fSlow7 * fRec2[1]) + fRec3[0])) - fVec6[1]);
			fRec1[0] = (fRec2[0] - (fSlow4 * ((fSlow25 * fRec1[2]) + (fSlow26 * fRec1[1]))));
			fRec0[0] = ((fSlow4 * (fRec1[0] + (fRec1[2] + (2.0f * fRec1[1])))) - (fSlow3 * ((fSlow26 * fRec0[1]) + (fSlow27 * fRec0[2]))));
			output0[i] = FAUSTFLOAT((iSlow0?fTemp7:(fSlow3 * (fRec0[2] + (fRec0[0] + (2.0f * fRec0[1]))))));
			iVec0[1] = iVec0[0];
			fRec12[1] = fRec12[0];
			IOTA = (IOTA + 1);
			fRec13[1] = fRec13[0];
			fVec2[1] = fVec2[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fVec3[1] = fVec3[0];
			fRec9[1] = fRec9[0];
			fVec4[1] = fVec4[0];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec14[1] = fRec14[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fVec5[1] = fVec5[0];
			fRec3[1] = fRec3[0];
			fVec6[1] = fVec6[0];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			
		}
		
	}

	
};
// clang-format on
#endif

    template <class T>
    struct _fx_distortion2_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_distortion2_UI<T>::name(sym(fx_distortion2));

typedef _fx_distortion2_UI<fx_distortion2> fx_distortion2_UI;

class faust_fx_distortion2_tilde : public FaustExternal<fx_distortion2, fx_distortion2_UI> {
public:
    faust_fx_distortion2_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
