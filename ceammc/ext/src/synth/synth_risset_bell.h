/* ------------------------------------------------------------
author: "Alain Bonardi & Paul Goutmann"
name: "synth.risset_bell", "abc_rissetsbell"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_risset_bell_H__
#define  __synth_risset_bell_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_risset_bell_dsp.h **************************/
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

class synth_risset_bell_dsp {

    public:

        synth_risset_bell_dsp() {}
        virtual ~synth_risset_bell_dsp() {}

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
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual synth_risset_bell_dsp* clone() = 0;
    
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

class decorator_dsp : public synth_risset_bell_dsp {

    protected:

        synth_risset_bell_dsp* fDSP;

    public:

        decorator_dsp(synth_risset_bell_dsp* synth_risset_bell_dsp = nullptr):fDSP(synth_risset_bell_dsp) {}
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
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
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
    
        virtual synth_risset_bell_dsp* createDSPInstance() = 0;
    
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
/**************************  END  synth_risset_bell_dsp.h **************************/
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

/**
 The base class of Meta handler to be used in synth_risset_bell_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
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
struct synth_risset_bell : public synth_risset_bell_dsp {
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
#include <cstdint>
#include <math.h>

class synth_risset_bellSIG0 {
	
  private:
	
	int iRec1[2];
	
  public:
	
	int getNumInputssynth_risset_bellSIG0() {
		return 0;
	}
	int getNumOutputssynth_risset_bellSIG0() {
		return 1;
	}
	int getInputRatesynth_risset_bellSIG0(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatesynth_risset_bellSIG0(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 0;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	void instanceInitsynth_risset_bellSIG0(int sample_rate) {
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec1[l3] = 0;
		}
	}
	
	void fillsynth_risset_bellSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec1[0] = (iRec1[1] + 1);
			table[i] = std::cos((9.58738019e-05f * float((iRec1[0] + -1))));
			iRec1[1] = iRec1[0];
		}
	}

};

static synth_risset_bellSIG0* newsynth_risset_bellSIG0() { return (synth_risset_bellSIG0*)new synth_risset_bellSIG0(); }
static void deletesynth_risset_bellSIG0(synth_risset_bellSIG0* dsp) { delete dsp; }

static float ftbl0synth_risset_bellSIG0[65536];
static float synth_risset_bell_faustpower4_f(float value) {
	return (((value * value) * value) * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_risset_bell
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_risset_bell : public synth_risset_bell_dsp {
	
 private:
	
	FAUSTFLOAT fButton0;
	float fVec0[2];
	int iVec1[2];
	float fRec0[2];
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fRec2[2];
	FAUSTFLOAT fHslider1;
	float fRec3[2];
	float fRec4[2];
	float fConst2;
	float fConst3;
	int iRec5[2];
	float fConst4;
	FAUSTFLOAT fHslider2;
	float fConst5;
	float fRec6[2];
	float fRec7[2];
	float fConst6;
	float fConst7;
	float fRec8[2];
	float fRec9[2];
	float fConst8;
	float fRec10[2];
	float fRec11[2];
	float fConst9;
	float fConst10;
	float fRec12[2];
	float fRec13[2];
	float fConst11;
	float fConst12;
	float fRec14[2];
	float fRec15[2];
	float fConst13;
	float fConst14;
	float fRec16[2];
	float fRec17[2];
	float fConst15;
	float fConst16;
	float fRec18[2];
	float fRec19[2];
	float fConst17;
	float fConst18;
	float fRec20[2];
	float fRec21[2];
	float fConst19;
	float fConst20;
	float fRec22[2];
	float fRec23[2];
	float fConst21;
	float fConst22;
	float fRec24[2];
	float fRec25[2];
	float fConst23;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Alain Bonardi & Paul Goutmann");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "synth_risset_bell.dsp");
		m->declare("licence", "LGPLv3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "synth.risset_bell");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
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
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	static void classInit(int sample_rate) {
		synth_risset_bellSIG0* sig0 = newsynth_risset_bellSIG0();
		sig0->instanceInitsynth_risset_bellSIG0(sample_rate);
		sig0->fillsynth_risset_bellSIG0(65536, ftbl0synth_risset_bellSIG0);
		deletesynth_risset_bellSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (0.560000002f / fConst0);
		fConst2 = std::max<float>(1.0f, (0.00499999989f * fConst0));
		fConst3 = (1.0f / fConst2);
		fConst4 = (0.00100000005f * fConst0);
		fConst5 = (1.0f / fConst0);
		fConst6 = (0.000899999985f * fConst0);
		fConst7 = (0.920000017f / fConst0);
		fConst8 = (0.000650000002f * fConst0);
		fConst9 = (0.000549999997f * fConst0);
		fConst10 = (1.19000006f / fConst0);
		fConst11 = (0.000325000001f * fConst0);
		fConst12 = (1.70000005f / fConst0);
		fConst13 = (0.000349999988f * fConst0);
		fConst14 = (2.0f / fConst0);
		fConst15 = (0.000250000012f * fConst0);
		fConst16 = (2.74000001f / fConst0);
		fConst17 = (0.000199999995f * fConst0);
		fConst18 = (3.0f / fConst0);
		fConst19 = (0.000150000007f * fConst0);
		fConst20 = (3.75999999f / fConst0);
		fConst21 = (9.99999975e-05f * fConst0);
		fConst22 = (4.07000017f / fConst0);
		fConst23 = (7.50000036e-05f * fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(440.0f);
		fHslider1 = FAUSTFLOAT(1.0f);
		fHslider2 = FAUSTFLOAT(5000.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iVec1[l1] = 0;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec0[l2] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec2[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec3[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec4[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec5[l7] = 0;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec7[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec10[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec16[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec17[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec18[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec19[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec20[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec21[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec22[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec23[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec24[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec25[l27] = 0.0f;
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
	
	virtual synth_risset_bell* clone() {
		return new synth_risset_bell();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.risset_bell");
		ui_interface->declare(&fHslider2, "unit", "ms");
		ui_interface->addHorizontalSlider("dur", &fHslider2, 5000.0f, 10.0f, 600000.0f, 1.0f);
		ui_interface->declare(&fHslider0, "unit", "Hz");
		ui_interface->addHorizontalSlider("freq", &fHslider0, 440.0f, 1.0f, 10000.0f, 0.00999999978f);
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("pfreq", &fHslider1, 1.0f, 0.0f, 10.0f, 0.00100000005f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fButton0);
		float fSlow1 = std::min<float>(1.0f, std::max<float>(0.0f, fSlow0));
		float fSlow2 = float(fHslider0);
		float fSlow3 = (fConst1 * fSlow2);
		float fSlow4 = (0.00100000005f * float(fHslider1));
		float fSlow5 = float(fHslider2);
		float fSlow6 = (1.0f / std::max<float>(1.0f, (fConst4 * fSlow5)));
		float fSlow7 = (fConst5 * ((0.560000002f * fSlow2) + 1.0f));
		float fSlow8 = (1.0f / std::max<float>(1.0f, (fConst6 * fSlow5)));
		float fSlow9 = (fConst7 * fSlow2);
		float fSlow10 = (1.0f / std::max<float>(1.0f, (fConst8 * fSlow5)));
		float fSlow11 = (fConst5 * ((0.920000017f * fSlow2) + 1.70000005f));
		float fSlow12 = (1.0f / std::max<float>(1.0f, (fConst9 * fSlow5)));
		float fSlow13 = (fConst10 * fSlow2);
		float fSlow14 = (1.0f / std::max<float>(1.0f, (fConst11 * fSlow5)));
		float fSlow15 = (fConst12 * fSlow2);
		float fSlow16 = (1.0f / std::max<float>(1.0f, (fConst13 * fSlow5)));
		float fSlow17 = (fConst14 * fSlow2);
		float fSlow18 = (1.0f / std::max<float>(1.0f, (fConst15 * fSlow5)));
		float fSlow19 = (fConst16 * fSlow2);
		float fSlow20 = (1.0f / std::max<float>(1.0f, (fConst17 * fSlow5)));
		float fSlow21 = (fConst18 * fSlow2);
		float fSlow22 = (1.0f / std::max<float>(1.0f, (fConst19 * fSlow5)));
		float fSlow23 = (fConst20 * fSlow2);
		float fSlow24 = (1.0f / std::max<float>(1.0f, (fConst21 * fSlow5)));
		float fSlow25 = (fConst22 * fSlow2);
		float fSlow26 = (1.0f / std::max<float>(1.0f, (fConst23 * fSlow5)));
		for (int i = 0; (i < count); i = (i + 1)) {
			fVec0[0] = fSlow0;
			int iTemp0 = (fSlow0 > fVec0[1]);
			iVec1[0] = iTemp0;
			int iTemp1 = ((iVec1[1] <= 0) & (iTemp0 > 0));
			fRec0[0] = ((fRec0[1] * float((1 - iTemp1))) + (fSlow1 * float(iTemp1)));
			fRec2[0] = (fSlow3 + (fRec2[1] - std::floor((fSlow3 + fRec2[1]))));
			fRec3[0] = (fSlow4 + (0.999000013f * fRec3[1]));
			int iTemp2 = (fRec3[0] == 1.0f);
			float fTemp3 = float((iTemp2 + 1));
			float fTemp4 = float((1 - iTemp2));
			float fTemp5 = (fRec4[1] + (fSlow3 * fRec3[0]));
			fRec4[0] = (fTemp5 - std::floor(fTemp5));
			iRec5[0] = (iTemp0 + ((iRec5[1] + (iRec5[1] > 0)) * (fSlow0 <= fVec0[1])));
			float fTemp6 = float(iRec5[0]);
			float fTemp7 = (fConst3 * fTemp6);
			float fTemp8 = (fConst2 - fTemp6);
			fRec6[0] = (fSlow7 + (fRec6[1] - std::floor((fSlow7 + fRec6[1]))));
			float fTemp9 = (fRec7[1] + (fSlow7 * fRec3[0]));
			fRec7[0] = (fTemp9 - std::floor(fTemp9));
			fRec8[0] = (fSlow9 + (fRec8[1] - std::floor((fSlow9 + fRec8[1]))));
			float fTemp10 = (fRec9[1] + (fSlow9 * fRec3[0]));
			fRec9[0] = (fTemp10 - std::floor(fTemp10));
			fRec10[0] = (fSlow11 + (fRec10[1] - std::floor((fSlow11 + fRec10[1]))));
			float fTemp11 = (fRec11[1] + (fSlow11 * fRec3[0]));
			fRec11[0] = (fTemp11 - std::floor(fTemp11));
			fRec12[0] = (fSlow13 + (fRec12[1] - std::floor((fSlow13 + fRec12[1]))));
			float fTemp12 = (fRec13[1] + (fSlow13 * fRec3[0]));
			fRec13[0] = (fTemp12 - std::floor(fTemp12));
			fRec14[0] = (fSlow15 + (fRec14[1] - std::floor((fSlow15 + fRec14[1]))));
			float fTemp13 = (fRec15[1] + (fSlow15 * fRec3[0]));
			fRec15[0] = (fTemp13 - std::floor(fTemp13));
			fRec16[0] = (fSlow17 + (fRec16[1] - std::floor((fSlow17 + fRec16[1]))));
			float fTemp14 = (fRec17[1] + (fSlow17 * fRec3[0]));
			fRec17[0] = (fTemp14 - std::floor(fTemp14));
			fRec18[0] = (fSlow19 + (fRec18[1] - std::floor((fSlow19 + fRec18[1]))));
			float fTemp15 = (fRec19[1] + (fSlow19 * fRec3[0]));
			fRec19[0] = (fTemp15 - std::floor(fTemp15));
			fRec20[0] = (fSlow21 + (fRec20[1] - std::floor((fSlow21 + fRec20[1]))));
			float fTemp16 = (fRec21[1] + (fSlow21 * fRec3[0]));
			fRec21[0] = (fTemp16 - std::floor(fTemp16));
			fRec22[0] = (fSlow23 + (fRec22[1] - std::floor((fSlow23 + fRec22[1]))));
			float fTemp17 = (fRec23[1] + (fSlow23 * fRec3[0]));
			fRec23[0] = (fTemp17 - std::floor(fTemp17));
			fRec24[0] = (fSlow25 + (fRec24[1] - std::floor((fSlow25 + fRec24[1]))));
			float fTemp18 = (fRec25[1] + (fSlow25 * fRec3[0]));
			fRec25[0] = (fTemp18 - std::floor(fTemp18));
			output0[i] = FAUSTFLOAT((fRec0[0] * (((((((((((0.0500000007f * (((ftbl0synth_risset_bellSIG0[int((65536.0f * fRec2[0]))] * fTemp3) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec4[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow6 * fTemp8) + 1.0f)))))) + (0.0335000008f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec6[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec7[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow8 * fTemp8) + 1.0f))))))) + (0.0500000007f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec8[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec9[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow10 * fTemp8) + 1.0f))))))) + (0.0900000036f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec10[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec11[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow12 * fTemp8) + 1.0f))))))) + (0.133499995f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec12[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec13[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow14 * fTemp8) + 1.0f))))))) + (0.0834999979f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec14[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec15[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow16 * fTemp8) + 1.0f))))))) + (0.0729999989f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec16[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec17[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow18 * fTemp8) + 1.0f))))))) + (0.0665000007f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec18[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec19[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow20 * fTemp8) + 1.0f))))))) + (0.0665000007f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec20[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec21[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow22 * fTemp8) + 1.0f))))))) + (0.0500000007f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec22[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec23[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow24 * fTemp8) + 1.0f))))))) + (0.0665000007f * (((fTemp3 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec24[0]))]) + (fTemp4 * ftbl0synth_risset_bellSIG0[int((65536.0f * fRec25[0]))])) * synth_risset_bell_faustpower4_f(std::max<float>(0.0f, std::min<float>(fTemp7, ((fSlow26 * fTemp8) + 1.0f)))))))));
			fVec0[1] = fVec0[0];
			iVec1[1] = iVec1[0];
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			iRec5[1] = iRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec25[1] = fRec25[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _synth_risset_bell_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_risset_bell_UI<T>::name(sym(synth_risset_bell));

typedef _synth_risset_bell_UI<synth_risset_bell> synth_risset_bell_UI;

class faust_synth_risset_bell_tilde : public FaustExternal<synth_risset_bell, synth_risset_bell_UI> {
public:
    faust_synth_risset_bell_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
