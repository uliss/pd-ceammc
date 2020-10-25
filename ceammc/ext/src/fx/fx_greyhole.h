/* ------------------------------------------------------------
author: "Julian Parker, bug fixes by Till Bovermann"
copyright: "(c) Julian Parker 2013"
license: "GPL2+"
name: "fx.greyhole"
version: "1.0"
Code generated with Faust 2.28.6 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_greyhole_H__
#define  __fx_greyhole_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_greyhole_dsp.h **************************/
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

struct UI;
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

class fx_greyhole_dsp {

    public:

        fx_greyhole_dsp() {}
        virtual ~fx_greyhole_dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
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
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual fx_greyhole_dsp* clone() = 0;
    
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
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public fx_greyhole_dsp {

    protected:

        fx_greyhole_dsp* fDSP;

    public:

        decorator_dsp(fx_greyhole_dsp* fx_greyhole_dsp = nullptr):fDSP(fx_greyhole_dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
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
    
        virtual fx_greyhole_dsp* createDSPInstance() = 0;
    
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
/**************************  END  fx_greyhole_dsp.h **************************/
/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
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

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN meta.h **************************/
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
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
    
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN misc.h **************************/
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
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <string>


using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

static int lsr(int x, int n) { return int(((unsigned int)x) >> n); }

static int int2pow2(int x) { int r = 0; while ((1<<r) < x) r++; return r; }

static long lopt(char* argv[], const char* name, long def)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
    return def;
}

static long lopt1(int argc, char* argv[], const char* longname, const char* shortname, long def)
{
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i-1], shortname) == 0 || strcmp(argv[i-1], longname) == 0) {
            return atoi(argv[i]);
        }
    }
    return def;
}

static const char* lopts(char* argv[], const char* name, const char* def)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
    return def;
}

static const char* lopts1(int argc, char* argv[], const char* longname, const char* shortname, const char* def)
{
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i-1], shortname) == 0 || strcmp(argv[i-1], longname) == 0) {
            return argv[i];
        }
    }
    return def;
}

static bool isopt(char* argv[], const char* name)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
    return false;
}

static std::string pathToContent(const std::string& path)
{
    std::ifstream file(path.c_str(), std::ifstream::binary);
    
    file.seekg(0, file.end);
    int size = int(file.tellg());
    file.seekg(0, file.beg);
    
    // And allocate buffer to that a single line can be read...
    char* buffer = new char[size + 1];
    file.read(buffer, size);
    
    // Terminate the string
    buffer[size] = 0;
    std::string result = buffer;
    file.close();
    delete [] buffer;
    return result;
}

#endif

/**************************  END  misc.h **************************/

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
struct fx_greyhole : public fx_greyhole_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "primes" */
#include "jprev.h"
#include <algorithm>
#include <cmath>
#include <math.h>


#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_greyhole
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_greyhole : public fx_greyhole_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	int iVec0[2];
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fVec1[2];
	FAUSTFLOAT fHslider2;
	float fVec2[2];
	FAUSTFLOAT fHslider3;
	float fVec3[2];
	int fSampleRate;
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
	FAUSTFLOAT fHslider7;
	float fRec46[2];
	float fVec8[2];
	float fRec39[2];
	float fRec37[2];
	float fVec9[131072];
	float fVec10[16384];
	float fRec48[2];
	float fVec11[2];
	float fRec47[2];
	float fRec38[2];
	float fVec12[16384];
	float fRec49[2];
	float fVec13[2];
	float fRec36[2];
	float fRec34[2];
	float fVec14[16384];
	float fRec51[2];
	float fVec15[2];
	float fRec50[2];
	float fRec35[2];
	float fVec16[16384];
	float fRec52[2];
	float fVec17[2];
	float fRec33[2];
	float fRec31[2];
	float fVec18[16384];
	float fRec54[2];
	float fVec19[2];
	float fRec53[2];
	float fRec32[2];
	float fVec20[16384];
	float fRec55[2];
	float fVec21[2];
	float fRec30[2];
	float fRec28[2];
	float fVec22[16384];
	float fRec57[2];
	float fVec23[2];
	float fRec56[2];
	float fRec29[2];
	float fVec24[16384];
	float fRec58[2];
	float fVec25[2];
	float fRec27[2];
	float fRec25[2];
	float fVec26[16384];
	float fRec60[2];
	float fVec27[2];
	float fRec59[2];
	float fRec26[2];
	float fVec28[16384];
	float fRec61[2];
	float fVec29[2];
	float fRec24[2];
	float fRec22[2];
	float fVec30[16384];
	float fRec63[2];
	float fVec31[2];
	float fRec62[2];
	float fRec23[2];
	float fVec32[16384];
	float fRec64[2];
	float fVec33[2];
	float fRec21[2];
	float fRec19[2];
	float fVec34[16384];
	float fRec66[2];
	float fVec35[2];
	float fRec65[2];
	float fRec20[2];
	float fVec36[16384];
	float fRec67[2];
	float fVec37[2];
	float fRec18[2];
	float fRec16[2];
	float fVec38[16384];
	float fRec69[2];
	float fVec39[2];
	float fRec68[2];
	float fRec17[2];
	float fVec40[16384];
	float fRec70[2];
	float fVec41[2];
	float fRec15[2];
	float fRec13[2];
	float fVec42[16384];
	float fRec72[2];
	float fVec43[2];
	float fRec71[2];
	float fRec14[2];
	float fVec44[16384];
	float fRec73[2];
	float fVec45[2];
	float fRec12[2];
	float fRec10[2];
	float fVec46[16384];
	float fRec75[2];
	float fVec47[2];
	float fRec74[2];
	float fRec11[2];
	float fVec48[16384];
	float fRec76[2];
	float fVec49[2];
	float fRec9[2];
	float fRec7[2];
	float fVec50[16384];
	float fRec78[2];
	float fVec51[2];
	float fRec77[2];
	float fRec8[2];
	float fVec52[16384];
	float fRec79[2];
	float fVec53[2];
	float fRec6[2];
	float fRec4[2];
	float fVec54[16384];
	float fRec81[2];
	float fVec55[2];
	float fRec80[2];
	float fRec5[2];
	float fRec3[2];
	float fRec1[1024];
	float fRec82[2];
	float fRec2[1024];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Julian Parker, bug fixes by Till Bovermann");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.1");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("copyright", "(c) Julian Parker 2013");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_greyhole.dsp");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("license", "GPL2+");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "fx.greyhole");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
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
		switch ((channel)) {
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
		switch ((channel)) {
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
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (0.000566893432f * fConst0);
		fConst2 = (3.14159274f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.90000000000000002f);
		fHslider4 = FAUSTFLOAT(0.20000000000000001f);
		fHslider5 = FAUSTFLOAT(0.10000000000000001f);
		fHslider6 = FAUSTFLOAT(2.0f);
		fHslider7 = FAUSTFLOAT(1.0f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec0[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fVec1[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fVec2[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec3[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec40[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec41[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec42[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec43[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fVec4[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fVec5[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec44[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec45[l12] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 131072); l13 = (l13 + 1)) {
			fVec6[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 16384); l14 = (l14 + 1)) {
			fVec7[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec46[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fVec8[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec39[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec37[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 131072); l19 = (l19 + 1)) {
			fVec9[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 16384); l20 = (l20 + 1)) {
			fVec10[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec48[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fVec11[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec47[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec38[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 16384); l25 = (l25 + 1)) {
			fVec12[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec49[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fVec13[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec36[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec34[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 16384); l30 = (l30 + 1)) {
			fVec14[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec51[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fVec15[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec50[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec35[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 16384); l35 = (l35 + 1)) {
			fVec16[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec52[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec17[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec33[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec31[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 16384); l40 = (l40 + 1)) {
			fVec18[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec54[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fVec19[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec53[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec32[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 16384); l45 = (l45 + 1)) {
			fVec20[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec55[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fVec21[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec30[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec28[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 16384); l50 = (l50 + 1)) {
			fVec22[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec57[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fVec23[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec56[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec29[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 16384); l55 = (l55 + 1)) {
			fVec24[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec58[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fVec25[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec27[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec25[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 16384); l60 = (l60 + 1)) {
			fVec26[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec60[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fVec27[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec59[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec26[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 16384); l65 = (l65 + 1)) {
			fVec28[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec61[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fVec29[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec24[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec22[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 16384); l70 = (l70 + 1)) {
			fVec30[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec63[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fVec31[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec62[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec23[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 16384); l75 = (l75 + 1)) {
			fVec32[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec64[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fVec33[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec21[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec19[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 16384); l80 = (l80 + 1)) {
			fVec34[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec66[l81] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fVec35[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec65[l83] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec20[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 16384); l85 = (l85 + 1)) {
			fVec36[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec67[l86] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fVec37[l87] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec18[l88] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec16[l89] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l90 = 0; (l90 < 16384); l90 = (l90 + 1)) {
			fVec38[l90] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec69[l91] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fVec39[l92] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec68[l93] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec17[l94] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l95 = 0; (l95 < 16384); l95 = (l95 + 1)) {
			fVec40[l95] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec70[l96] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fVec41[l97] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec15[l98] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l99 = 0; (l99 < 2); l99 = (l99 + 1)) {
			fRec13[l99] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l100 = 0; (l100 < 16384); l100 = (l100 + 1)) {
			fVec42[l100] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec72[l101] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			fVec43[l102] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			fRec71[l103] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec14[l104] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l105 = 0; (l105 < 16384); l105 = (l105 + 1)) {
			fVec44[l105] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec73[l106] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l107 = 0; (l107 < 2); l107 = (l107 + 1)) {
			fVec45[l107] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			fRec12[l108] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec10[l109] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l110 = 0; (l110 < 16384); l110 = (l110 + 1)) {
			fVec46[l110] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec75[l111] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fVec47[l112] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fRec74[l113] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec11[l114] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l115 = 0; (l115 < 16384); l115 = (l115 + 1)) {
			fVec48[l115] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			fRec76[l116] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fVec49[l117] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			fRec9[l118] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec7[l119] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l120 = 0; (l120 < 16384); l120 = (l120 + 1)) {
			fVec50[l120] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec78[l121] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			fVec51[l122] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l123 = 0; (l123 < 2); l123 = (l123 + 1)) {
			fRec77[l123] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			fRec8[l124] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l125 = 0; (l125 < 16384); l125 = (l125 + 1)) {
			fVec52[l125] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			fRec79[l126] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			fVec53[l127] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			fRec6[l128] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			fRec4[l129] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l130 = 0; (l130 < 16384); l130 = (l130 + 1)) {
			fVec54[l130] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			fRec81[l131] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			fVec55[l132] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			fRec80[l133] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			fRec5[l134] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l135 = 0; (l135 < 2); l135 = (l135 + 1)) {
			fRec3[l135] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l136 = 0; (l136 < 1024); l136 = (l136 + 1)) {
			fRec1[l136] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l137 = 0; (l137 < 2); l137 = (l137 + 1)) {
			fRec82[l137] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l138 = 0; (l138 < 1024); l138 = (l138 + 1)) {
			fRec2[l138] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual fx_greyhole* clone() {
		return new fx_greyhole();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.greyhole");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addHorizontalSlider("damping", &fHslider1, 0.0f, 0.0f, 0.99000001f, 0.00100000005f);
		ui_interface->addHorizontalSlider("delaytime", &fHslider4, 0.200000003f, 0.00100000005f, 1.45000005f, 9.99999975e-05f);
		ui_interface->addHorizontalSlider("diffusion", &fHslider2, 0.5f, 0.0f, 0.99000001f, 9.99999975e-05f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("feedback", &fHslider3, 0.899999976f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("moddepth", &fHslider5, 0.100000001f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("modfreq", &fHslider6, 2.0f, 0.0f, 10.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("size", &fHslider7, 1.0f, 0.5f, 3.0f, 9.99999975e-05f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = (0.00100000005f * float(fHslider0));
		float fSlow2 = float(fHslider1);
		float fSlow3 = float(fHslider2);
		float fSlow4 = float(fHslider3);
		float fSlow5 = std::floor(std::min<float>(65533.0f, (fConst0 * float(fHslider4))));
		float fSlow6 = float(fHslider5);
		float fSlow7 = float(fHslider6);
		float fSlow8 = float(fHslider7);
		float fSlow9 = (49.0f * fSlow8);
		int iSlow10 = int(primes(int(fSlow9)));
		float fSlow11 = (9.99999975e-05f * float(iSlow10));
		int iSlow12 = int(primes(int((fSlow9 + 10.0f))));
		float fSlow13 = (9.99999975e-05f * float(iSlow12));
		float fSlow14 = (36.0f * fSlow8);
		int iSlow15 = int(primes(int(fSlow14)));
		float fSlow16 = (0.00100000005f * float(iSlow15));
		int iSlow17 = int(primes(int((fSlow14 + 10.0f))));
		float fSlow18 = (0.00100000005f * float(iSlow17));
		float fSlow19 = (23.0f * fSlow8);
		int iSlow20 = int(primes(int(fSlow19)));
		float fSlow21 = (0.00100000005f * float(iSlow20));
		int iSlow22 = int(primes(int((fSlow19 + 10.0f))));
		float fSlow23 = (0.00100000005f * float(iSlow22));
		int iSlow24 = int(primes(int((10.0f * fSlow8))));
		float fSlow25 = (0.00100000005f * float(iSlow24));
		int iSlow26 = int(primes(int((10.0f * (fSlow8 + 1.0f)))));
		float fSlow27 = (0.00100000005f * float(iSlow26));
		float fSlow28 = (68.0f * fSlow8);
		int iSlow29 = int(primes(int(fSlow28)));
		float fSlow30 = (9.99999975e-05f * float(iSlow29));
		int iSlow31 = int(primes(int((fSlow28 + 10.0f))));
		float fSlow32 = (9.99999975e-05f * float(iSlow31));
		float fSlow33 = (55.0f * fSlow8);
		int iSlow34 = int(primes(int(fSlow33)));
		float fSlow35 = (0.00100000005f * float(iSlow34));
		int iSlow36 = int(primes(int((fSlow33 + 10.0f))));
		float fSlow37 = (0.00100000005f * float(iSlow36));
		float fSlow38 = (42.0f * fSlow8);
		int iSlow39 = int(primes(int(fSlow38)));
		float fSlow40 = (0.00100000005f * float(iSlow39));
		int iSlow41 = int(primes(int((fSlow38 + 10.0f))));
		float fSlow42 = (0.00100000005f * float(iSlow41));
		float fSlow43 = (29.0f * fSlow8);
		int iSlow44 = int(primes(int(fSlow43)));
		float fSlow45 = (0.00100000005f * float(iSlow44));
		int iSlow46 = int(primes(int((fSlow43 + 10.0f))));
		float fSlow47 = (0.00100000005f * float(iSlow46));
		float fSlow48 = (87.0f * fSlow8);
		int iSlow49 = int(primes(int(fSlow48)));
		float fSlow50 = (9.99999975e-05f * float(iSlow49));
		int iSlow51 = int(primes(int((fSlow48 + 10.0f))));
		float fSlow52 = (9.99999975e-05f * float(iSlow51));
		float fSlow53 = (74.0f * fSlow8);
		int iSlow54 = int(primes(int(fSlow53)));
		float fSlow55 = (0.00100000005f * float(iSlow54));
		int iSlow56 = int(primes(int((fSlow53 + 10.0f))));
		float fSlow57 = (0.00100000005f * float(iSlow56));
		float fSlow58 = (61.0f * fSlow8);
		int iSlow59 = int(primes(int(fSlow58)));
		float fSlow60 = (0.00100000005f * float(iSlow59));
		int iSlow61 = int(primes(int((fSlow58 + 10.0f))));
		float fSlow62 = (0.00100000005f * float(iSlow61));
		float fSlow63 = (48.0f * fSlow8);
		int iSlow64 = int(primes(int(fSlow63)));
		float fSlow65 = (0.00100000005f * float(iSlow64));
		int iSlow66 = int(primes(int((fSlow63 + 10.0f))));
		float fSlow67 = (0.00100000005f * float(iSlow66));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			float fTemp1 = (iSlow0 ? 0.0f : fTemp0);
			iVec0[0] = 1;
			fRec0[0] = (fSlow1 + (0.999000013f * fRec0[1]));
			float fTemp2 = (1.0f - fRec0[0]);
			fVec1[0] = fSlow2;
			float fTemp3 = (fSlow2 + fVec1[1]);
			fVec2[0] = fSlow3;
			float fTemp4 = (0.5f * (fSlow3 + fVec2[1]));
			float fTemp5 = std::cos(fTemp4);
			float fTemp6 = (0.0f - fTemp4);
			float fTemp7 = std::cos(fTemp6);
			float fTemp8 = float(input1[i]);
			float fTemp9 = (iSlow0 ? 0.0f : fTemp8);
			fVec3[0] = fSlow4;
			float fTemp10 = (fSlow4 + fVec3[1]);
			float fTemp11 = ((fRec40[1] != 0.0f) ? (((fRec41[1] > 0.0f) & (fRec41[1] < 1.0f)) ? fRec40[1] : 0.0f) : (((fRec41[1] == 0.0f) & (fSlow5 != fRec42[1])) ? 4.53514731e-05f : (((fRec41[1] == 1.0f) & (fSlow5 != fRec43[1])) ? -4.53514731e-05f : 0.0f)));
			fRec40[0] = fTemp11;
			fRec41[0] = std::max<float>(0.0f, std::min<float>(1.0f, (fRec41[1] + fTemp11)));
			fRec42[0] = (((fRec41[1] >= 1.0f) & (fRec43[1] != fSlow5)) ? fSlow5 : fRec42[1]);
			fRec43[0] = (((fRec41[1] <= 0.0f) & (fRec42[1] != fSlow5)) ? fSlow5 : fRec43[1]);
			float fTemp12 = (1.0f - fRec41[0]);
			fVec4[0] = fSlow6;
			float fTemp13 = (fSlow6 + fVec4[1]);
			fVec5[0] = fSlow7;
			float fTemp14 = (fConst2 * (fSlow7 + fVec5[1]));
			float fTemp15 = std::sin(fTemp14);
			float fTemp16 = std::cos(fTemp14);
			fRec44[0] = ((fTemp15 * fRec45[1]) + (fTemp16 * fRec44[1]));
			int iTemp17 = (1 - iVec0[1]);
			fRec45[0] = ((float(iTemp17) + (fTemp16 * fRec45[1])) - (fTemp15 * fRec44[1]));
			float fTemp18 = (fConst1 * (fTemp13 * (fRec44[0] + 1.0f)));
			float fTemp19 = (fTemp18 + 8.50000477f);
			int iTemp20 = int(fTemp19);
			float fTemp21 = std::floor(fTemp19);
			float fTemp22 = (fTemp18 + (9.0f - fTemp21));
			float fTemp23 = (fTemp18 + (8.0f - fTemp21));
			float fTemp24 = (fTemp18 + (7.0f - fTemp21));
			float fTemp25 = (fTemp18 + (6.0f - fTemp21));
			float fTemp26 = (fTemp22 * fTemp23);
			float fTemp27 = (((((fRec2[((IOTA - (std::min<int>(512, std::max<int>(0, iTemp20)) + 1)) & 1023)] * (0.0f - fTemp22)) * (0.0f - (0.5f * fTemp23))) * (0.0f - (0.333333343f * fTemp24))) * (0.0f - (0.25f * fTemp25))) + ((fTemp18 + (10.0f - fTemp21)) * ((((((fRec2[((IOTA - (std::min<int>(512, std::max<int>(0, (iTemp20 + 1))) + 1)) & 1023)] * (0.0f - fTemp23)) * (0.0f - (0.5f * fTemp24))) * (0.0f - (0.333333343f * fTemp25))) + (0.5f * (((fTemp22 * fRec2[((IOTA - (std::min<int>(512, std::max<int>(0, (iTemp20 + 2))) + 1)) & 1023)]) * (0.0f - fTemp24)) * (0.0f - (0.5f * fTemp25))))) + (0.166666672f * ((fTemp26 * fRec2[((IOTA - (std::min<int>(512, std::max<int>(0, (iTemp20 + 3))) + 1)) & 1023)]) * (0.0f - fTemp25)))) + (0.0416666679f * ((fTemp26 * fTemp24) * fRec2[((IOTA - (std::min<int>(512, std::max<int>(0, (iTemp20 + 4))) + 1)) & 1023)])))));
			fVec6[(IOTA & 131071)] = fTemp27;
			int iTemp28 = int(std::min<float>(65536.0f, std::max<float>(0.0f, fRec42[0])));
			int iTemp29 = int(std::min<float>(65536.0f, std::max<float>(0.0f, fRec43[0])));
			float fTemp30 = (fTemp9 + (0.5f * (fTemp10 * ((fTemp12 * fVec6[((IOTA - iTemp28) & 131071)]) + (fRec41[0] * fVec6[((IOTA - iTemp29) & 131071)])))));
			float fTemp31 = std::sin(fTemp4);
			float fTemp32 = ((fTemp5 * fTemp30) - (fTemp31 * fRec29[1]));
			float fTemp33 = ((fTemp5 * fTemp32) - (fTemp31 * fRec32[1]));
			float fTemp34 = ((fTemp5 * fTemp33) - (fTemp31 * fRec35[1]));
			fVec7[(IOTA & 16383)] = (0.0f - ((fTemp5 * fTemp34) - (fTemp31 * fRec38[1])));
			fRec46[0] = (fSlow11 + (0.999899983f * (fRec46[1] + float((iSlow10 * iTemp17)))));
			float fTemp35 = (fRec46[0] + -1.49998999f);
			float fTemp36 = fVec7[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp35)))) & 16383)];
			fVec8[0] = fTemp36;
			float fTemp37 = std::floor(fTemp35);
			fRec39[0] = (fVec8[1] - (((fTemp37 + (2.0f - fRec46[0])) * (fRec39[1] - fTemp36)) / (fRec46[0] - fTemp37)));
			fRec37[0] = fRec39[0];
			float fTemp38 = (fConst1 * (fTemp13 * (fRec45[0] + 1.0f)));
			float fTemp39 = (fTemp38 + 8.50000477f);
			int iTemp40 = int(fTemp39);
			float fTemp41 = std::floor(fTemp39);
			float fTemp42 = (fTemp38 + (9.0f - fTemp41));
			float fTemp43 = (fTemp38 + (8.0f - fTemp41));
			float fTemp44 = (fTemp38 + (7.0f - fTemp41));
			float fTemp45 = (fTemp38 + (6.0f - fTemp41));
			float fTemp46 = (fTemp42 * fTemp43);
			float fTemp47 = (((((fRec1[((IOTA - (std::min<int>(512, std::max<int>(0, iTemp40)) + 1)) & 1023)] * (0.0f - fTemp42)) * (0.0f - (0.5f * fTemp43))) * (0.0f - (0.333333343f * fTemp44))) * (0.0f - (0.25f * fTemp45))) + ((fTemp38 + (10.0f - fTemp41)) * ((((((fRec1[((IOTA - (std::min<int>(512, std::max<int>(0, (iTemp40 + 1))) + 1)) & 1023)] * (0.0f - fTemp43)) * (0.0f - (0.5f * fTemp44))) * (0.0f - (0.333333343f * fTemp45))) + (0.5f * (((fTemp42 * fRec1[((IOTA - (std::min<int>(512, std::max<int>(0, (iTemp40 + 2))) + 1)) & 1023)]) * (0.0f - fTemp44)) * (0.0f - (0.5f * fTemp45))))) + (0.166666672f * ((fTemp46 * fRec1[((IOTA - (std::min<int>(512, std::max<int>(0, (iTemp40 + 3))) + 1)) & 1023)]) * (0.0f - fTemp45)))) + (0.0416666679f * ((fTemp46 * fTemp44) * fRec1[((IOTA - (std::min<int>(512, std::max<int>(0, (iTemp40 + 4))) + 1)) & 1023)])))));
			fVec9[(IOTA & 131071)] = fTemp47;
			float fTemp48 = (fTemp1 + (0.5f * (((fTemp12 * fVec9[((IOTA - iTemp28) & 131071)]) + (fRec41[0] * fVec9[((IOTA - iTemp29) & 131071)])) * fTemp10)));
			float fTemp49 = ((fTemp48 * fTemp5) - (fTemp31 * fRec28[1]));
			float fTemp50 = ((fTemp5 * fTemp49) - (fTemp31 * fRec31[1]));
			float fTemp51 = ((fTemp5 * fTemp50) - (fTemp31 * fRec34[1]));
			fVec10[(IOTA & 16383)] = ((fTemp5 * fTemp51) - (fTemp31 * fRec37[1]));
			fRec48[0] = (fSlow13 + (0.999899983f * (fRec48[1] + float((iSlow12 * iTemp17)))));
			float fTemp52 = (fRec48[0] + -1.49998999f);
			float fTemp53 = fVec10[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp52)))) & 16383)];
			fVec11[0] = fTemp53;
			float fTemp54 = std::floor(fTemp52);
			fRec47[0] = (fVec11[1] - (((fTemp54 + (2.0f - fRec48[0])) * (fRec47[1] - fTemp53)) / (fRec48[0] - fTemp54)));
			fRec38[0] = fRec47[0];
			fVec12[(IOTA & 16383)] = (0.0f - ((fTemp5 * fRec38[1]) + (fTemp31 * fTemp34)));
			fRec49[0] = (fSlow16 + (0.999000013f * (fRec49[1] + float((iSlow15 * iTemp17)))));
			float fTemp55 = (fRec49[0] + -1.49998999f);
			float fTemp56 = fVec12[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp55)))) & 16383)];
			fVec13[0] = fTemp56;
			float fTemp57 = std::floor(fTemp55);
			fRec36[0] = (fVec13[1] - (((fTemp57 + (2.0f - fRec49[0])) * (fRec36[1] - fTemp56)) / (fRec49[0] - fTemp57)));
			fRec34[0] = fRec36[0];
			fVec14[(IOTA & 16383)] = ((fRec37[1] * fTemp5) + (fTemp31 * fTemp51));
			fRec51[0] = (fSlow18 + (0.999000013f * (fRec51[1] + float((iSlow17 * iTemp17)))));
			float fTemp58 = (fRec51[0] + -1.49998999f);
			float fTemp59 = fVec14[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp58)))) & 16383)];
			fVec15[0] = fTemp59;
			float fTemp60 = std::floor(fTemp58);
			fRec50[0] = (fVec15[1] - (((fTemp60 + (2.0f - fRec51[0])) * (fRec50[1] - fTemp59)) / (fRec51[0] - fTemp60)));
			fRec35[0] = fRec50[0];
			fVec16[(IOTA & 16383)] = (0.0f - ((fTemp5 * fRec35[1]) + (fTemp31 * fTemp33)));
			fRec52[0] = (fSlow21 + (0.999000013f * (fRec52[1] + float((iSlow20 * iTemp17)))));
			float fTemp61 = (fRec52[0] + -1.49998999f);
			float fTemp62 = fVec16[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp61)))) & 16383)];
			fVec17[0] = fTemp62;
			float fTemp63 = std::floor(fTemp61);
			fRec33[0] = (fVec17[1] - (((fTemp63 + (2.0f - fRec52[0])) * (fRec33[1] - fTemp62)) / (fRec52[0] - fTemp63)));
			fRec31[0] = fRec33[0];
			fVec18[(IOTA & 16383)] = ((fRec34[1] * fTemp5) + (fTemp31 * fTemp50));
			fRec54[0] = (fSlow23 + (0.999000013f * (fRec54[1] + float((iSlow22 * iTemp17)))));
			float fTemp64 = (fRec54[0] + -1.49998999f);
			float fTemp65 = fVec18[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp64)))) & 16383)];
			fVec19[0] = fTemp65;
			float fTemp66 = std::floor(fTemp64);
			fRec53[0] = (fVec19[1] - (((fTemp66 + (2.0f - fRec54[0])) * (fRec53[1] - fTemp65)) / (fRec54[0] - fTemp66)));
			fRec32[0] = fRec53[0];
			fVec20[(IOTA & 16383)] = (0.0f - ((fTemp5 * fRec32[1]) + (fTemp31 * fTemp32)));
			fRec55[0] = (fSlow25 + (0.999000013f * (fRec55[1] + float((iSlow24 * iTemp17)))));
			float fTemp67 = (fRec55[0] + -1.49998999f);
			float fTemp68 = fVec20[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp67)))) & 16383)];
			fVec21[0] = fTemp68;
			float fTemp69 = std::floor(fTemp67);
			fRec30[0] = (fVec21[1] - (((fTemp69 + (2.0f - fRec55[0])) * (fRec30[1] - fTemp68)) / (fRec55[0] - fTemp69)));
			fRec28[0] = fRec30[0];
			fVec22[(IOTA & 16383)] = ((fRec31[1] * fTemp5) + (fTemp31 * fTemp49));
			fRec57[0] = (fSlow27 + (0.999000013f * (fRec57[1] + float((iSlow26 * iTemp17)))));
			float fTemp70 = (fRec57[0] + -1.49998999f);
			float fTemp71 = fVec22[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp70)))) & 16383)];
			fVec23[0] = fTemp71;
			float fTemp72 = std::floor(fTemp70);
			fRec56[0] = (fVec23[1] - (((fTemp72 + (2.0f - fRec57[0])) * (fRec56[1] - fTemp71)) / (fRec57[0] - fTemp72)));
			fRec29[0] = fRec56[0];
			float fTemp73 = ((fTemp5 * fRec29[1]) + (fTemp31 * fTemp30));
			float fTemp74 = std::sin(fTemp6);
			float fTemp75 = ((fTemp7 * fTemp73) - (fTemp74 * fRec17[1]));
			float fTemp76 = ((fTemp7 * fTemp75) - (fTemp74 * fRec20[1]));
			float fTemp77 = ((fTemp7 * fTemp76) - (fTemp74 * fRec23[1]));
			fVec24[(IOTA & 16383)] = (0.0f - ((fTemp7 * fTemp77) - (fTemp74 * fRec26[1])));
			fRec58[0] = (fSlow30 + (0.999899983f * (fRec58[1] + float((iSlow29 * iTemp17)))));
			float fTemp78 = (fRec58[0] + -1.49998999f);
			float fTemp79 = fVec24[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp78)))) & 16383)];
			fVec25[0] = fTemp79;
			float fTemp80 = std::floor(fTemp78);
			fRec27[0] = (fVec25[1] - (((fTemp80 + (2.0f - fRec58[0])) * (fRec27[1] - fTemp79)) / (fRec58[0] - fTemp80)));
			fRec25[0] = fRec27[0];
			float fTemp81 = ((fRec28[1] * fTemp5) + (fTemp31 * fTemp48));
			float fTemp82 = ((fTemp81 * fTemp7) - (fTemp74 * fRec16[1]));
			float fTemp83 = ((fTemp7 * fTemp82) - (fTemp74 * fRec19[1]));
			float fTemp84 = ((fTemp7 * fTemp83) - (fTemp74 * fRec22[1]));
			fVec26[(IOTA & 16383)] = ((fTemp7 * fTemp84) - (fRec25[1] * fTemp74));
			fRec60[0] = (fSlow32 + (0.999899983f * (fRec60[1] + float((iSlow31 * iTemp17)))));
			float fTemp85 = (fRec60[0] + -1.49998999f);
			float fTemp86 = fVec26[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp85)))) & 16383)];
			fVec27[0] = fTemp86;
			float fTemp87 = std::floor(fTemp85);
			fRec59[0] = (fVec27[1] - (((fTemp87 + (2.0f - fRec60[0])) * (fRec59[1] - fTemp86)) / (fRec60[0] - fTemp87)));
			fRec26[0] = fRec59[0];
			fVec28[(IOTA & 16383)] = (0.0f - ((fTemp7 * fRec26[1]) + (fTemp74 * fTemp77)));
			fRec61[0] = (fSlow35 + (0.999000013f * (fRec61[1] + float((iSlow34 * iTemp17)))));
			float fTemp88 = (fRec61[0] + -1.49998999f);
			float fTemp89 = fVec28[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp88)))) & 16383)];
			fVec29[0] = fTemp89;
			float fTemp90 = std::floor(fTemp88);
			fRec24[0] = (fVec29[1] - (((fTemp90 + (2.0f - fRec61[0])) * (fRec24[1] - fTemp89)) / (fRec61[0] - fTemp90)));
			fRec22[0] = fRec24[0];
			fVec30[(IOTA & 16383)] = ((fRec25[1] * fTemp7) + (fTemp74 * fTemp84));
			fRec63[0] = (fSlow37 + (0.999000013f * (fRec63[1] + float((iSlow36 * iTemp17)))));
			float fTemp91 = (fRec63[0] + -1.49998999f);
			float fTemp92 = fVec30[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp91)))) & 16383)];
			fVec31[0] = fTemp92;
			float fTemp93 = std::floor(fTemp91);
			fRec62[0] = (fVec31[1] - (((fTemp93 + (2.0f - fRec63[0])) * (fRec62[1] - fTemp92)) / (fRec63[0] - fTemp93)));
			fRec23[0] = fRec62[0];
			fVec32[(IOTA & 16383)] = (0.0f - ((fTemp7 * fRec23[1]) + (fTemp74 * fTemp76)));
			fRec64[0] = (fSlow40 + (0.999000013f * (fRec64[1] + float((iSlow39 * iTemp17)))));
			float fTemp94 = (fRec64[0] + -1.49998999f);
			float fTemp95 = fVec32[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp94)))) & 16383)];
			fVec33[0] = fTemp95;
			float fTemp96 = std::floor(fTemp94);
			fRec21[0] = (fVec33[1] - (((fTemp96 + (2.0f - fRec64[0])) * (fRec21[1] - fTemp95)) / (fRec64[0] - fTemp96)));
			fRec19[0] = fRec21[0];
			fVec34[(IOTA & 16383)] = ((fRec22[1] * fTemp7) + (fTemp74 * fTemp83));
			fRec66[0] = (fSlow42 + (0.999000013f * (fRec66[1] + float((iSlow41 * iTemp17)))));
			float fTemp97 = (fRec66[0] + -1.49998999f);
			float fTemp98 = fVec34[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp97)))) & 16383)];
			fVec35[0] = fTemp98;
			float fTemp99 = std::floor(fTemp97);
			fRec65[0] = (fVec35[1] - (((fTemp99 + (2.0f - fRec66[0])) * (fRec65[1] - fTemp98)) / (fRec66[0] - fTemp99)));
			fRec20[0] = fRec65[0];
			fVec36[(IOTA & 16383)] = (0.0f - ((fTemp7 * fRec20[1]) + (fTemp74 * fTemp75)));
			fRec67[0] = (fSlow45 + (0.999000013f * (fRec67[1] + float((iSlow44 * iTemp17)))));
			float fTemp100 = (fRec67[0] + -1.49998999f);
			float fTemp101 = fVec36[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp100)))) & 16383)];
			fVec37[0] = fTemp101;
			float fTemp102 = std::floor(fTemp100);
			fRec18[0] = (fVec37[1] - (((fTemp102 + (2.0f - fRec67[0])) * (fRec18[1] - fTemp101)) / (fRec67[0] - fTemp102)));
			fRec16[0] = fRec18[0];
			fVec38[(IOTA & 16383)] = ((fRec19[1] * fTemp7) + (fTemp74 * fTemp82));
			fRec69[0] = (fSlow47 + (0.999000013f * (fRec69[1] + float((iSlow46 * iTemp17)))));
			float fTemp103 = (fRec69[0] + -1.49998999f);
			float fTemp104 = fVec38[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp103)))) & 16383)];
			fVec39[0] = fTemp104;
			float fTemp105 = std::floor(fTemp103);
			fRec68[0] = (fVec39[1] - (((fTemp105 + (2.0f - fRec69[0])) * (fRec68[1] - fTemp104)) / (fRec69[0] - fTemp105)));
			fRec17[0] = fRec68[0];
			float fTemp106 = ((fTemp7 * fRec17[1]) + (fTemp74 * fTemp73));
			float fTemp107 = ((fTemp5 * fTemp106) - (fTemp31 * fRec5[1]));
			float fTemp108 = ((fTemp5 * fTemp107) - (fTemp31 * fRec8[1]));
			float fTemp109 = ((fTemp5 * fTemp108) - (fTemp31 * fRec11[1]));
			fVec40[(IOTA & 16383)] = (0.0f - ((fTemp5 * fTemp109) - (fTemp31 * fRec14[1])));
			fRec70[0] = (fSlow50 + (0.999899983f * (fRec70[1] + float((iSlow49 * iTemp17)))));
			float fTemp110 = (fRec70[0] + -1.49998999f);
			float fTemp111 = fVec40[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp110)))) & 16383)];
			fVec41[0] = fTemp111;
			float fTemp112 = std::floor(fTemp110);
			fRec15[0] = (fVec41[1] - (((fTemp112 + (2.0f - fRec70[0])) * (fRec15[1] - fTemp111)) / (fRec70[0] - fTemp112)));
			fRec13[0] = fRec15[0];
			float fTemp113 = ((fRec16[1] * fTemp7) + (fTemp74 * fTemp81));
			float fTemp114 = ((fTemp5 * fTemp113) - (fTemp31 * fRec4[1]));
			float fTemp115 = ((fTemp5 * fTemp114) - (fTemp31 * fRec7[1]));
			float fTemp116 = ((fTemp5 * fTemp115) - (fTemp31 * fRec10[1]));
			fVec42[(IOTA & 16383)] = ((fTemp5 * fTemp116) - (fRec13[1] * fTemp31));
			fRec72[0] = (fSlow52 + (0.999899983f * (fRec72[1] + float((iSlow51 * iTemp17)))));
			float fTemp117 = (fRec72[0] + -1.49998999f);
			float fTemp118 = fVec42[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp117)))) & 16383)];
			fVec43[0] = fTemp118;
			float fTemp119 = std::floor(fTemp117);
			fRec71[0] = (fVec43[1] - (((fTemp119 + (2.0f - fRec72[0])) * (fRec71[1] - fTemp118)) / (fRec72[0] - fTemp119)));
			fRec14[0] = fRec71[0];
			fVec44[(IOTA & 16383)] = (0.0f - ((fTemp5 * fRec14[1]) + (fTemp31 * fTemp109)));
			fRec73[0] = (fSlow55 + (0.999000013f * (fRec73[1] + float((iSlow54 * iTemp17)))));
			float fTemp120 = (fRec73[0] + -1.49998999f);
			float fTemp121 = fVec44[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp120)))) & 16383)];
			fVec45[0] = fTemp121;
			float fTemp122 = std::floor(fTemp120);
			fRec12[0] = (fVec45[1] - (((fTemp122 + (2.0f - fRec73[0])) * (fRec12[1] - fTemp121)) / (fRec73[0] - fTemp122)));
			fRec10[0] = fRec12[0];
			fVec46[(IOTA & 16383)] = ((fRec13[1] * fTemp5) + (fTemp31 * fTemp116));
			fRec75[0] = (fSlow57 + (0.999000013f * (fRec75[1] + float((iSlow56 * iTemp17)))));
			float fTemp123 = (fRec75[0] + -1.49998999f);
			float fTemp124 = fVec46[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp123)))) & 16383)];
			fVec47[0] = fTemp124;
			float fTemp125 = std::floor(fTemp123);
			fRec74[0] = (fVec47[1] - (((fTemp125 + (2.0f - fRec75[0])) * (fRec74[1] - fTemp124)) / (fRec75[0] - fTemp125)));
			fRec11[0] = fRec74[0];
			fVec48[(IOTA & 16383)] = (0.0f - ((fTemp5 * fRec11[1]) + (fTemp31 * fTemp108)));
			fRec76[0] = (fSlow60 + (0.999000013f * (fRec76[1] + float((iSlow59 * iTemp17)))));
			float fTemp126 = (fRec76[0] + -1.49998999f);
			float fTemp127 = fVec48[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp126)))) & 16383)];
			fVec49[0] = fTemp127;
			float fTemp128 = std::floor(fTemp126);
			fRec9[0] = (fVec49[1] - (((fTemp128 + (2.0f - fRec76[0])) * (fRec9[1] - fTemp127)) / (fRec76[0] - fTemp128)));
			fRec7[0] = fRec9[0];
			fVec50[(IOTA & 16383)] = ((fRec10[1] * fTemp5) + (fTemp31 * fTemp115));
			fRec78[0] = (fSlow62 + (0.999000013f * (fRec78[1] + float((iSlow61 * iTemp17)))));
			float fTemp129 = (fRec78[0] + -1.49998999f);
			float fTemp130 = fVec50[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp129)))) & 16383)];
			fVec51[0] = fTemp130;
			float fTemp131 = std::floor(fTemp129);
			fRec77[0] = (fVec51[1] - (((fTemp131 + (2.0f - fRec78[0])) * (fRec77[1] - fTemp130)) / (fRec78[0] - fTemp131)));
			fRec8[0] = fRec77[0];
			fVec52[(IOTA & 16383)] = (0.0f - ((fTemp5 * fRec8[1]) + (fTemp31 * fTemp107)));
			fRec79[0] = (fSlow65 + (0.999000013f * (fRec79[1] + float((iSlow64 * iTemp17)))));
			float fTemp132 = (fRec79[0] + -1.49998999f);
			float fTemp133 = fVec52[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp132)))) & 16383)];
			fVec53[0] = fTemp133;
			float fTemp134 = std::floor(fTemp132);
			fRec6[0] = (fVec53[1] - (((fTemp134 + (2.0f - fRec79[0])) * (fRec6[1] - fTemp133)) / (fRec79[0] - fTemp134)));
			fRec4[0] = fRec6[0];
			fVec54[(IOTA & 16383)] = ((fRec7[1] * fTemp5) + (fTemp31 * fTemp114));
			fRec81[0] = (fSlow67 + (0.999000013f * (fRec81[1] + float((iSlow66 * iTemp17)))));
			float fTemp135 = (fRec81[0] + -1.49998999f);
			float fTemp136 = fVec54[((IOTA - std::min<int>(8192, std::max<int>(0, int(fTemp135)))) & 16383)];
			fVec55[0] = fTemp136;
			float fTemp137 = std::floor(fTemp135);
			fRec80[0] = (fVec55[1] - (((fTemp137 + (2.0f - fRec81[0])) * (fRec80[1] - fTemp136)) / (fRec81[0] - fTemp137)));
			fRec5[0] = fRec80[0];
			float fTemp138 = (1.0f - (0.5f * fTemp3));
			fRec3[0] = ((0.5f * (fTemp3 * fRec3[1])) + (((fRec4[1] * fTemp5) + (fTemp31 * fTemp113)) * fTemp138));
			fRec1[(IOTA & 1023)] = fRec3[0];
			fRec82[0] = ((0.5f * (fTemp3 * fRec82[1])) + (fTemp138 * ((fTemp5 * fRec5[1]) + (fTemp31 * fTemp106))));
			fRec2[(IOTA & 1023)] = fRec82[0];
			output0[i] = FAUSTFLOAT((iSlow0 ? fTemp0 : ((fTemp1 * fTemp2) + (fRec0[0] * fRec1[((IOTA - 0) & 1023)]))));
			output1[i] = FAUSTFLOAT((iSlow0 ? fTemp8 : ((fTemp2 * fTemp9) + (fRec0[0] * fRec2[((IOTA - 0) & 1023)]))));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fVec1[1] = fVec1[0];
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
			fVec8[1] = fVec8[0];
			fRec39[1] = fRec39[0];
			fRec37[1] = fRec37[0];
			fRec48[1] = fRec48[0];
			fVec11[1] = fVec11[0];
			fRec47[1] = fRec47[0];
			fRec38[1] = fRec38[0];
			fRec49[1] = fRec49[0];
			fVec13[1] = fVec13[0];
			fRec36[1] = fRec36[0];
			fRec34[1] = fRec34[0];
			fRec51[1] = fRec51[0];
			fVec15[1] = fVec15[0];
			fRec50[1] = fRec50[0];
			fRec35[1] = fRec35[0];
			fRec52[1] = fRec52[0];
			fVec17[1] = fVec17[0];
			fRec33[1] = fRec33[0];
			fRec31[1] = fRec31[0];
			fRec54[1] = fRec54[0];
			fVec19[1] = fVec19[0];
			fRec53[1] = fRec53[0];
			fRec32[1] = fRec32[0];
			fRec55[1] = fRec55[0];
			fVec21[1] = fVec21[0];
			fRec30[1] = fRec30[0];
			fRec28[1] = fRec28[0];
			fRec57[1] = fRec57[0];
			fVec23[1] = fVec23[0];
			fRec56[1] = fRec56[0];
			fRec29[1] = fRec29[0];
			fRec58[1] = fRec58[0];
			fVec25[1] = fVec25[0];
			fRec27[1] = fRec27[0];
			fRec25[1] = fRec25[0];
			fRec60[1] = fRec60[0];
			fVec27[1] = fVec27[0];
			fRec59[1] = fRec59[0];
			fRec26[1] = fRec26[0];
			fRec61[1] = fRec61[0];
			fVec29[1] = fVec29[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec63[1] = fRec63[0];
			fVec31[1] = fVec31[0];
			fRec62[1] = fRec62[0];
			fRec23[1] = fRec23[0];
			fRec64[1] = fRec64[0];
			fVec33[1] = fVec33[0];
			fRec21[1] = fRec21[0];
			fRec19[1] = fRec19[0];
			fRec66[1] = fRec66[0];
			fVec35[1] = fVec35[0];
			fRec65[1] = fRec65[0];
			fRec20[1] = fRec20[0];
			fRec67[1] = fRec67[0];
			fVec37[1] = fVec37[0];
			fRec18[1] = fRec18[0];
			fRec16[1] = fRec16[0];
			fRec69[1] = fRec69[0];
			fVec39[1] = fVec39[0];
			fRec68[1] = fRec68[0];
			fRec17[1] = fRec17[0];
			fRec70[1] = fRec70[0];
			fVec41[1] = fVec41[0];
			fRec15[1] = fRec15[0];
			fRec13[1] = fRec13[0];
			fRec72[1] = fRec72[0];
			fVec43[1] = fVec43[0];
			fRec71[1] = fRec71[0];
			fRec14[1] = fRec14[0];
			fRec73[1] = fRec73[0];
			fVec45[1] = fVec45[0];
			fRec12[1] = fRec12[0];
			fRec10[1] = fRec10[0];
			fRec75[1] = fRec75[0];
			fVec47[1] = fVec47[0];
			fRec74[1] = fRec74[0];
			fRec11[1] = fRec11[0];
			fRec76[1] = fRec76[0];
			fVec49[1] = fVec49[0];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec78[1] = fRec78[0];
			fVec51[1] = fVec51[0];
			fRec77[1] = fRec77[0];
			fRec8[1] = fRec8[0];
			fRec79[1] = fRec79[0];
			fVec53[1] = fVec53[0];
			fRec6[1] = fRec6[0];
			fRec4[1] = fRec4[0];
			fRec81[1] = fRec81[0];
			fVec55[1] = fVec55[0];
			fRec80[1] = fRec80[0];
			fRec5[1] = fRec5[0];
			fRec3[1] = fRec3[0];
			fRec82[1] = fRec82[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_greyhole_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_greyhole_UI<T>::name(sym(fx_greyhole));

typedef _fx_greyhole_UI<fx_greyhole> fx_greyhole_UI;

class faust_fx_greyhole_tilde : public FaustExternal<fx_greyhole, fx_greyhole_UI> {
public:
    faust_fx_greyhole_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
