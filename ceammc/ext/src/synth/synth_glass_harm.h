/* ------------------------------------------------------------
author: "Romain Michon"
copyright: "Romain Michon (rmichon@ccrma.stanford.edu)"
name: "synth.glass_harm"
version: "1.0"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_glass_harm_H__
#define  __synth_glass_harm_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_glass_harm_dsp.h **************************/
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

class synth_glass_harm_dsp {

    public:

        synth_glass_harm_dsp() {}
        virtual ~synth_glass_harm_dsp() {}

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
        virtual synth_glass_harm_dsp* clone() = 0;
    
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

class decorator_dsp : public synth_glass_harm_dsp {

    protected:

        synth_glass_harm_dsp* fDSP;

    public:

        decorator_dsp(synth_glass_harm_dsp* synth_glass_harm_dsp = nullptr):fDSP(synth_glass_harm_dsp) {}
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
    
        virtual synth_glass_harm_dsp* createDSPInstance() = 0;
    
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
/**************************  END  synth_glass_harm_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_glass_harm_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_glass_harm : public synth_glass_harm_dsp {
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

class synth_glass_harmSIG0 {
	
  private:
	
	int iRec33[2];
	
  public:
	
	int getNumInputssynth_glass_harmSIG0() {
		return 0;
	}
	int getNumOutputssynth_glass_harmSIG0() {
		return 1;
	}
	int getInputRatesynth_glass_harmSIG0(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatesynth_glass_harmSIG0(int channel) {
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
	
	void instanceInitsynth_glass_harmSIG0(int sample_rate) {
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			iRec33[l29] = 0;
		}
	}
	
	void fillsynth_glass_harmSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec33[0] = (iRec33[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec33[0] + -1))));
			iRec33[1] = iRec33[0];
		}
	}

};

static synth_glass_harmSIG0* newsynth_glass_harmSIG0() { return (synth_glass_harmSIG0*)new synth_glass_harmSIG0(); }
static void deletesynth_glass_harmSIG0(synth_glass_harmSIG0* dsp) { delete dsp; }

static float synth_glass_harm_faustpower2_f(float value) {
	return (value * value);
}
static float synth_glass_harm_faustpower4_f(float value) {
	return (((value * value) * value) * value);
}
static float ftbl0synth_glass_harmSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_glass_harm
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_glass_harm : public synth_glass_harm_dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	float fRec10[2];
	FAUSTFLOAT fEntry0;
	int fSampleRate;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fEntry1;
	FAUSTFLOAT fCheckbox0;
	float fRec19[2];
	float fConst5;
	int iVec0[2];
	int iRec20[2];
	float fConst6;
	int iRec21[2];
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	int IOTA;
	float fVec1[8192];
	float fConst7;
	FAUSTFLOAT fHslider5;
	float fConst8;
	float fConst9;
	float fRec18[3];
	float fRec17[2];
	float fRec11[2];
	float fVec2[4096];
	float fConst10;
	float fConst11;
	float fRec23[3];
	float fRec22[2];
	float fRec12[2];
	float fVec3[2048];
	float fConst12;
	float fConst13;
	float fRec25[3];
	float fRec24[2];
	float fRec13[2];
	float fVec4[1024];
	float fConst14;
	float fConst15;
	float fRec27[3];
	float fRec26[2];
	float fRec14[2];
	float fVec5[1024];
	float fConst16;
	float fConst17;
	float fRec29[3];
	float fRec28[2];
	float fRec15[2];
	float fConst18;
	float fRec31[3];
	float fRec16[2];
	float fVec6[2];
	FAUSTFLOAT fHslider6;
	float fRec32[2];
	float fConst19;
	FAUSTFLOAT fHslider7;
	float fRec35[2];
	float fRec34[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[2];
	float fRec38[2];
	float fRec37[2];
	float fRec36[2];
	float fRec47[2];
	float fRec46[2];
	float fRec45[2];
	float fRec44[2];
	float fRec43[2];
	float fRec42[2];
	float fVec7[4096];
	float fConst20;
	FAUSTFLOAT fHslider8;
	float fVec8[4096];
	int iConst21;
	float fConst22;
	float fConst25;
	FAUSTFLOAT fHslider9;
	float fConst26;
	float fConst28;
	float fConst29;
	float fRec49[2];
	float fRec48[2];
	float fVec9[16384];
	int iConst31;
	float fVec10[4096];
	int iConst32;
	float fRec8[2];
	float fConst35;
	float fConst36;
	float fRec53[2];
	float fRec52[2];
	float fVec11[16384];
	int iConst38;
	float fVec12[4096];
	float fVec13[2048];
	int iConst39;
	float fRec50[2];
	float fConst42;
	float fConst43;
	float fRec57[2];
	float fRec56[2];
	float fVec14[32768];
	int iConst45;
	float fVec15[2048];
	int iConst46;
	float fRec54[2];
	float fConst49;
	float fConst50;
	float fRec61[2];
	float fRec60[2];
	float fVec16[32768];
	int iConst52;
	float fVec17[4096];
	int iConst53;
	float fRec58[2];
	float fConst56;
	float fConst57;
	float fRec65[2];
	float fRec64[2];
	float fVec18[32768];
	int iConst59;
	float fVec19[4096];
	int iConst60;
	float fRec62[2];
	float fConst63;
	float fConst64;
	float fRec69[2];
	float fRec68[2];
	float fVec20[16384];
	int iConst66;
	float fVec21[2048];
	int iConst67;
	float fRec66[2];
	float fConst70;
	float fConst71;
	float fRec73[2];
	float fRec72[2];
	float fVec22[16384];
	int iConst73;
	float fVec23[4096];
	int iConst74;
	float fRec70[2];
	float fConst77;
	float fConst78;
	float fRec77[2];
	float fRec76[2];
	float fVec24[16384];
	int iConst80;
	float fVec25[4096];
	int iConst81;
	float fRec74[2];
	float fRec0[3];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("copyright", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("description", "Nonlinear Banded Waveguide Modeled Glass Harmonica");
		m->declare("envelopes.lib/adsr:author", "Yann Orlarey");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "synth_glass_harm.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/allpassnn:author", "Julius O. Smith III");
		m->declare("filters.lib/allpassnn:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpassnn:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("instruments.lib/author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("instruments.lib/copyright", "Romain Michon");
		m->declare("instruments.lib/licence", "STK-4.3");
		m->declare("instruments.lib/name", "Faust-STK Tools Library");
		m->declare("instruments.lib/version", "1.0");
		m->declare("licence", "STK-4.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "synth.glass_harm");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("spn.lib/name", "Standart Pitch Notation constants");
		m->declare("spn.lib/version", "0.2");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
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
		synth_glass_harmSIG0* sig0 = newsynth_glass_harmSIG0();
		sig0->instanceInitsynth_glass_harmSIG0(sample_rate);
		sig0->fillsynth_glass_harmSIG0(65536, ftbl0synth_glass_harmSIG0);
		deletesynth_glass_harmSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = (1.0f - (100.530968f / fConst0));
		fConst2 = synth_glass_harm_faustpower2_f(fConst1);
		fConst3 = (0.5f * (1.0f - fConst2));
		float fConst4 = (0.0199999996f * fConst0);
		fConst5 = (1.0f / std::max<float>(1.0f, fConst4));
		fConst6 = (1.0f / std::max<float>(1.0f, (0.00999999978f * fConst0)));
		fConst7 = (0.0022727272f * fConst0);
		fConst8 = (0.0f - (2.0f * fConst1));
		fConst9 = (2764.60156f / fConst0);
		fConst10 = (0.000979623874f * fConst0);
		fConst11 = (6413.87549f / fConst0);
		fConst12 = (0.000534759369f * fConst0);
		fConst13 = (11749.5566f / fConst0);
		fConst14 = (0.000342794461f * fConst0);
		fConst15 = (18329.3086f / fConst0);
		fConst16 = (0.00024229502f * fConst0);
		fConst17 = (25931.9629f / fConst0);
		fConst18 = (24881.4141f / fConst0);
		fConst19 = (1.0f / fConst0);
		fConst20 = (0.0011363636f * fConst0);
		iConst21 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst4)));
		fConst22 = std::cos((37699.1133f / fConst0));
		float fConst23 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst24 = ((0.0f - (6.90775537f * fConst23)) / fConst0);
		fConst25 = (0.25f * fConst24);
		fConst26 = (0.166666672f * fConst24);
		float fConst27 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst28 = (1.0f / (fConst27 + 1.0f));
		fConst29 = (1.0f - fConst27);
		float fConst30 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst31 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst23 - fConst30))));
		iConst32 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst30 + -1.0f))));
		float fConst33 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst34 = ((0.0f - (6.90775537f * fConst33)) / fConst0);
		fConst35 = (0.25f * fConst34);
		fConst36 = (0.166666672f * fConst34);
		float fConst37 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst38 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst33 - fConst37))));
		iConst39 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst37 + -1.0f))));
		float fConst40 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst41 = ((0.0f - (6.90775537f * fConst40)) / fConst0);
		fConst42 = (0.25f * fConst41);
		fConst43 = (0.166666672f * fConst41);
		float fConst44 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst45 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst40 - fConst44))));
		iConst46 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst44 + -1.0f))));
		float fConst47 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst48 = ((0.0f - (6.90775537f * fConst47)) / fConst0);
		fConst49 = (0.25f * fConst48);
		fConst50 = (0.166666672f * fConst48);
		float fConst51 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst52 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst47 - fConst51))));
		iConst53 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst51 + -1.0f))));
		float fConst54 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst55 = ((0.0f - (6.90775537f * fConst54)) / fConst0);
		fConst56 = (0.25f * fConst55);
		fConst57 = (0.166666672f * fConst55);
		float fConst58 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst59 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst54 - fConst58))));
		iConst60 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst58 + -1.0f))));
		float fConst61 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst62 = ((0.0f - (6.90775537f * fConst61)) / fConst0);
		fConst63 = (0.25f * fConst62);
		fConst64 = (0.166666672f * fConst62);
		float fConst65 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst66 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst61 - fConst65))));
		iConst67 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst65 + -1.0f))));
		float fConst68 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst69 = ((0.0f - (6.90775537f * fConst68)) / fConst0);
		fConst70 = (0.25f * fConst69);
		fConst71 = (0.166666672f * fConst69);
		float fConst72 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst73 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst68 - fConst72))));
		iConst74 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst72 + -1.0f))));
		float fConst75 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst76 = ((0.0f - (6.90775537f * fConst75)) / fConst0);
		fConst77 = (0.25f * fConst76);
		fConst78 = (0.166666672f * fConst76);
		float fConst79 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst80 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst75 - fConst79))));
		iConst81 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst79 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.59999999999999998f);
		fHslider1 = FAUSTFLOAT(0.13700000000000001f);
		fEntry0 = FAUSTFLOAT(0.0f);
		fEntry1 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(1.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.20000000000000001f);
		fHslider5 = FAUSTFLOAT(48.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(220.0f);
		fHslider8 = FAUSTFLOAT(0.5f);
		fHslider9 = FAUSTFLOAT(0.35999999999999999f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec10[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec19[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iVec0[l2] = 0;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec20[l3] = 0;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec21[l4] = 0;
		}
		IOTA = 0;
		for (int l5 = 0; (l5 < 8192); l5 = (l5 + 1)) {
			fVec1[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 3); l6 = (l6 + 1)) {
			fRec18[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec17[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec11[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 4096); l9 = (l9 + 1)) {
			fVec2[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec23[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec22[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec12[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2048); l13 = (l13 + 1)) {
			fVec3[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec25[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec24[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec13[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 1024); l17 = (l17 + 1)) {
			fVec4[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec27[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec26[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec14[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 1024); l21 = (l21 + 1)) {
			fVec5[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec29[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec28[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec15[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 3); l25 = (l25 + 1)) {
			fRec31[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec16[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fVec6[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec32[l28] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec35[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec34[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec41[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec40[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec39[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec38[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec37[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec36[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec47[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec46[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec45[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec44[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec43[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec42[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 4096); l44 = (l44 + 1)) {
			fVec7[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 4096); l45 = (l45 + 1)) {
			fVec8[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec49[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec48[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 16384); l48 = (l48 + 1)) {
			fVec9[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 4096); l49 = (l49 + 1)) {
			fVec10[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec8[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec53[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec52[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 16384); l53 = (l53 + 1)) {
			fVec11[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 4096); l54 = (l54 + 1)) {
			fVec12[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 2048); l55 = (l55 + 1)) {
			fVec13[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec50[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec57[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec56[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 32768); l59 = (l59 + 1)) {
			fVec14[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 2048); l60 = (l60 + 1)) {
			fVec15[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec54[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec61[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec60[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 32768); l64 = (l64 + 1)) {
			fVec16[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 4096); l65 = (l65 + 1)) {
			fVec17[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec58[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec65[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec64[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 32768); l69 = (l69 + 1)) {
			fVec18[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 4096); l70 = (l70 + 1)) {
			fVec19[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec62[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec69[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec68[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 16384); l74 = (l74 + 1)) {
			fVec20[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 2048); l75 = (l75 + 1)) {
			fVec21[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec66[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec73[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec72[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 16384); l79 = (l79 + 1)) {
			fVec22[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 4096); l80 = (l80 + 1)) {
			fVec23[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec70[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec77[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec76[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 16384); l84 = (l84 + 1)) {
			fVec24[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 4096); l85 = (l85 + 1)) {
			fVec25[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec74[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 3); l87 = (l87 + 1)) {
			fRec0[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 3); l88 = (l88 + 1)) {
			fRec1[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 3); l89 = (l89 + 1)) {
			fRec2[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 3); l90 = (l90 + 1)) {
			fRec3[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 3); l91 = (l91 + 1)) {
			fRec4[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 3); l92 = (l92 + 1)) {
			fRec5[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 3); l93 = (l93 + 1)) {
			fRec6[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 3); l94 = (l94 + 1)) {
			fRec7[l94] = 0.0f;
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
	
	virtual synth_glass_harm* clone() {
		return new synth_glass_harm();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.glass_harm");
		ui_interface->declare(&fHslider4, "tooltip", "Bow pressure on the instrument (Value between 0 and 1)");
		ui_interface->addHorizontalSlider("bow_press", &fHslider4, 0.200000003f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider2, "tooltip", "A value between 0 and 1");
		ui_interface->addHorizontalSlider("gain", &fHslider2, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fCheckbox0, "type", "float");
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->declare(&fHslider3, "tooltip", "A value between 0 and 1");
		ui_interface->addHorizontalSlider("integration", &fHslider3, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider7, "tooltip", "Frequency of the sine wave for the modulation of theta (works if Modulation Type=3)");
		ui_interface->declare(&fHslider7, "unit", "Hz");
		ui_interface->addHorizontalSlider("modfreq", &fHslider7, 220.0f, 20.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fEntry0, "tooltip", "0=theta is modulated by the incoming signal; 1=theta is modulated by the averaged incoming signal; 2=theta is modulated by the squared incoming signal; 3=theta is modulated by a sine wave of frequency freqMod; 4=theta is modulated by a sine wave of frequency freq;");
		ui_interface->declare(&fEntry0, "type", "int");
		ui_interface->addNumEntry("modtype", &fEntry0, 0.0f, 0.0f, 4.0f, 1.0f);
		ui_interface->declare(&fHslider6, "tooltip", "Nonlinearity factor (value between 0 and 1)");
		ui_interface->addHorizontalSlider("nonlin", &fHslider6, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pan", &fHslider0, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider5, 48.0f, 24.0f, 96.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider1, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider9, 0.360000014f, 0.00499999989f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fEntry1, "tooltip", "0=Bow; 1=Strike");
		ui_interface->declare(&fEntry1, "type", "int");
		ui_interface->addNumEntry("strike", &fEntry1, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addHorizontalSlider("width", &fHslider8, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (0.00100000005f * float(fHslider1));
		int iSlow2 = int(float(fEntry0));
		float fSlow3 = float((iSlow2 >= 3));
		float fSlow4 = float(fEntry1);
		float fSlow5 = (0.166666672f * (0.0f - (fSlow4 + -1.0f)));
		float fSlow6 = float(fCheckbox0);
		float fSlow7 = (0.00100000005f * std::min<float>(1.0f, std::max<float>(0.0f, fSlow6)));
		int iSlow8 = (fSlow6 > 0.0f);
		int iSlow9 = (iSlow8 == 0);
		float fSlow10 = ((0.100000001f * float(fHslider2)) + 0.899999976f);
		float fSlow11 = float(fHslider3);
		float fSlow12 = (10.0f - (9.0f * float(fHslider4)));
		float fSlow13 = (0.200000003f * (fSlow4 * float(iSlow8)));
		float fSlow14 = std::pow(2.0f, (0.0833333358f * (float(fHslider5) + -69.0f)));
		int iSlow15 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst7 / fSlow14))));
		float fSlow16 = (fConst8 * std::cos((fConst9 * fSlow14)));
		int iSlow17 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst10 / fSlow14))));
		float fSlow18 = (fConst8 * std::cos((fConst11 * fSlow14)));
		int iSlow19 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst12 / fSlow14))));
		float fSlow20 = (fConst8 * std::cos((fConst13 * fSlow14)));
		int iSlow21 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst14 / fSlow14))));
		float fSlow22 = (fConst8 * std::cos((fConst15 * fSlow14)));
		int iSlow23 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst16 / fSlow14))));
		float fSlow24 = (fConst8 * std::cos((fConst17 * fSlow14)));
		float fSlow25 = (fConst8 * std::cos((fConst18 * fSlow14)));
		float fSlow26 = (0.00100000005f * float(fHslider6));
		float fSlow27 = (440.0f * (fSlow14 * float((iSlow2 == 4))));
		float fSlow28 = float((iSlow2 != 4));
		float fSlow29 = (0.00100000005f * float(fHslider7));
		float fSlow30 = float((iSlow2 < 3));
		float fSlow31 = (12.566371f * float((iSlow2 == 0)));
		float fSlow32 = (6.28318548f * float((iSlow2 == 1)));
		float fSlow33 = (50.2654839f * float((iSlow2 == 2)));
		int iSlow34 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst20 * (float(fHslider8) / fSlow14)))));
		float fSlow35 = float(fHslider9);
		float fSlow36 = std::exp((fConst25 / fSlow35));
		float fSlow37 = synth_glass_harm_faustpower2_f(fSlow36);
		float fSlow38 = (1.0f - (fConst22 * fSlow37));
		float fSlow39 = (1.0f - fSlow37);
		float fSlow40 = (fSlow38 / fSlow39);
		float fSlow41 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow38) / synth_glass_harm_faustpower2_f(fSlow39)) + -1.0f)));
		float fSlow42 = (fSlow40 - fSlow41);
		float fSlow43 = (fSlow36 * (fSlow41 + (1.0f - fSlow40)));
		float fSlow44 = ((std::exp((fConst26 / fSlow35)) / fSlow36) + -1.0f);
		float fSlow45 = std::exp((fConst35 / fSlow35));
		float fSlow46 = synth_glass_harm_faustpower2_f(fSlow45);
		float fSlow47 = (1.0f - (fConst22 * fSlow46));
		float fSlow48 = (1.0f - fSlow46);
		float fSlow49 = (fSlow47 / fSlow48);
		float fSlow50 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow47) / synth_glass_harm_faustpower2_f(fSlow48)) + -1.0f)));
		float fSlow51 = (fSlow49 - fSlow50);
		float fSlow52 = (fSlow45 * (fSlow50 + (1.0f - fSlow49)));
		float fSlow53 = ((std::exp((fConst36 / fSlow35)) / fSlow45) + -1.0f);
		float fSlow54 = (1.0f - fSlow0);
		float fSlow55 = std::exp((fConst42 / fSlow35));
		float fSlow56 = synth_glass_harm_faustpower2_f(fSlow55);
		float fSlow57 = (1.0f - (fConst22 * fSlow56));
		float fSlow58 = (1.0f - fSlow56);
		float fSlow59 = (fSlow57 / fSlow58);
		float fSlow60 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow57) / synth_glass_harm_faustpower2_f(fSlow58)) + -1.0f)));
		float fSlow61 = (fSlow59 - fSlow60);
		float fSlow62 = (fSlow55 * (fSlow60 + (1.0f - fSlow59)));
		float fSlow63 = ((std::exp((fConst43 / fSlow35)) / fSlow55) + -1.0f);
		float fSlow64 = std::exp((fConst49 / fSlow35));
		float fSlow65 = synth_glass_harm_faustpower2_f(fSlow64);
		float fSlow66 = (1.0f - (fConst22 * fSlow65));
		float fSlow67 = (1.0f - fSlow65);
		float fSlow68 = (fSlow66 / fSlow67);
		float fSlow69 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow66) / synth_glass_harm_faustpower2_f(fSlow67)) + -1.0f)));
		float fSlow70 = (fSlow68 - fSlow69);
		float fSlow71 = (fSlow64 * (fSlow69 + (1.0f - fSlow68)));
		float fSlow72 = ((std::exp((fConst50 / fSlow35)) / fSlow64) + -1.0f);
		float fSlow73 = std::exp((fConst56 / fSlow35));
		float fSlow74 = synth_glass_harm_faustpower2_f(fSlow73);
		float fSlow75 = (1.0f - (fConst22 * fSlow74));
		float fSlow76 = (1.0f - fSlow74);
		float fSlow77 = (fSlow75 / fSlow76);
		float fSlow78 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow75) / synth_glass_harm_faustpower2_f(fSlow76)) + -1.0f)));
		float fSlow79 = (fSlow77 - fSlow78);
		float fSlow80 = (fSlow73 * (fSlow78 + (1.0f - fSlow77)));
		float fSlow81 = ((std::exp((fConst57 / fSlow35)) / fSlow73) + -1.0f);
		float fSlow82 = std::exp((fConst63 / fSlow35));
		float fSlow83 = synth_glass_harm_faustpower2_f(fSlow82);
		float fSlow84 = (1.0f - (fConst22 * fSlow83));
		float fSlow85 = (1.0f - fSlow83);
		float fSlow86 = (fSlow84 / fSlow85);
		float fSlow87 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow84) / synth_glass_harm_faustpower2_f(fSlow85)) + -1.0f)));
		float fSlow88 = (fSlow86 - fSlow87);
		float fSlow89 = (fSlow82 * (fSlow87 + (1.0f - fSlow86)));
		float fSlow90 = ((std::exp((fConst64 / fSlow35)) / fSlow82) + -1.0f);
		float fSlow91 = std::exp((fConst70 / fSlow35));
		float fSlow92 = synth_glass_harm_faustpower2_f(fSlow91);
		float fSlow93 = (1.0f - (fConst22 * fSlow92));
		float fSlow94 = (1.0f - fSlow92);
		float fSlow95 = (fSlow93 / fSlow94);
		float fSlow96 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow93) / synth_glass_harm_faustpower2_f(fSlow94)) + -1.0f)));
		float fSlow97 = (fSlow95 - fSlow96);
		float fSlow98 = (fSlow91 * (fSlow96 + (1.0f - fSlow95)));
		float fSlow99 = ((std::exp((fConst71 / fSlow35)) / fSlow91) + -1.0f);
		float fSlow100 = std::exp((fConst77 / fSlow35));
		float fSlow101 = synth_glass_harm_faustpower2_f(fSlow100);
		float fSlow102 = (1.0f - (fConst22 * fSlow101));
		float fSlow103 = (1.0f - fSlow101);
		float fSlow104 = (fSlow102 / fSlow103);
		float fSlow105 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow102) / synth_glass_harm_faustpower2_f(fSlow103)) + -1.0f)));
		float fSlow106 = (fSlow104 - fSlow105);
		float fSlow107 = (fSlow100 * (fSlow105 + (1.0f - fSlow104)));
		float fSlow108 = ((std::exp((fConst78 / fSlow35)) / fSlow100) + -1.0f);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec10[0] = (fSlow1 + (0.999000013f * fRec10[1]));
			fRec19[0] = (fSlow7 + (0.999000013f * fRec19[1]));
			iVec0[0] = iSlow8;
			iRec20[0] = (iSlow8 + (iRec20[1] * (iVec0[1] >= iSlow8)));
			iRec21[0] = (iSlow9 * (iRec21[1] + 1));
			float fTemp0 = (((((0.100000001f * fRec19[0]) + 0.0299999993f) * std::max<float>(0.0f, (std::min<float>((fConst5 * float(iRec20[0])), 1.0f) - (fConst6 * float(iRec21[0]))))) - (fSlow10 * (((fRec11[1] + fRec13[1]) + fRec15[1]) + ((fRec12[1] + fRec14[1]) + fRec16[1])))) - fSlow11);
			float fTemp1 = synth_glass_harm_faustpower4_f((std::fabs((fSlow12 * fTemp0)) + 0.75f));
			float fTemp2 = (1.0f / fTemp1);
			float fTemp3 = (fSlow5 * (fTemp0 * (float((fTemp2 > 1.0f)) + (float((fTemp2 <= 1.0f)) / fTemp1))));
			float fTemp4 = (fSlow13 * fRec19[0]);
			fVec1[(IOTA & 8191)] = ((fRec17[1] + fTemp3) + fTemp4);
			fRec18[0] = ((0.999000013f * fVec1[((IOTA - iSlow15) & 8191)]) - ((fSlow16 * fRec18[1]) + (fConst2 * fRec18[2])));
			fRec17[0] = (fConst3 * (fRec18[0] - fRec18[2]));
			fRec11[0] = fRec17[0];
			float fTemp5 = (fTemp3 + fTemp4);
			fVec2[(IOTA & 4095)] = (fRec22[1] + fTemp5);
			fRec23[0] = ((0.998000979f * fVec2[((IOTA - iSlow17) & 4095)]) - ((fSlow18 * fRec23[1]) + (fConst2 * fRec23[2])));
			fRec22[0] = (fConst3 * (fRec23[0] - fRec23[2]));
			fRec12[0] = fRec22[0];
			fVec3[(IOTA & 2047)] = (fTemp5 + fRec24[1]);
			fRec25[0] = ((0.997003019f * fVec3[((IOTA - iSlow19) & 2047)]) - ((fSlow20 * fRec25[1]) + (fConst2 * fRec25[2])));
			fRec24[0] = (fConst3 * (fRec25[0] - fRec25[2]));
			fRec13[0] = fRec24[0];
			fVec4[(IOTA & 1023)] = (fTemp5 + fRec26[1]);
			fRec27[0] = ((0.996006012f * fVec4[((IOTA - iSlow21) & 1023)]) - ((fSlow22 * fRec27[1]) + (fConst2 * fRec27[2])));
			fRec26[0] = (fConst3 * (fRec27[0] - fRec27[2]));
			fRec14[0] = fRec26[0];
			fVec5[(IOTA & 1023)] = (fTemp5 + fRec28[1]);
			fRec29[0] = ((0.995010018f * fVec5[((IOTA - iSlow23) & 1023)]) - ((fSlow24 * fRec29[1]) + (fConst2 * fRec29[2])));
			fRec28[0] = (fConst3 * (fRec29[0] - fRec29[2]));
			fRec15[0] = fRec28[0];
			fRec31[0] = (-1.0f * ((fSlow25 * fRec31[1]) + (fConst2 * fRec31[2])));
			float fRec30 = (fConst3 * (fRec31[0] - fRec31[2]));
			fRec16[0] = fRec30;
			float fTemp6 = (fRec16[0] + (fRec14[0] + (((fRec11[0] + fRec13[0]) + fRec15[0]) + fRec12[0])));
			fVec6[0] = fTemp6;
			fRec32[0] = (fSlow26 + (0.999000013f * fRec32[1]));
			fRec35[0] = (fSlow29 + (0.999000013f * fRec35[1]));
			float fTemp7 = (fRec34[1] + (fConst19 * (fSlow27 + (fSlow28 * fRec35[0]))));
			fRec34[0] = (fTemp7 - std::floor(fTemp7));
			float fTemp8 = (3.14159274f * (fRec32[0] * ftbl0synth_glass_harmSIG0[int((65536.0f * fRec34[0]))]));
			float fTemp9 = std::sin(fTemp8);
			float fTemp10 = std::cos(fTemp8);
			float fTemp11 = ((4.0f * (fTemp6 * fTemp10)) - (fTemp9 * fRec36[1]));
			float fTemp12 = ((fTemp10 * fTemp11) - (fTemp9 * fRec37[1]));
			float fTemp13 = ((fTemp10 * fTemp12) - (fTemp9 * fRec38[1]));
			float fTemp14 = ((fTemp10 * fTemp13) - (fTemp9 * fRec39[1]));
			float fTemp15 = ((fTemp10 * fTemp14) - (fTemp9 * fRec40[1]));
			fRec41[0] = ((fTemp10 * fTemp15) - (fTemp9 * fRec41[1]));
			fRec40[0] = ((fTemp9 * fTemp15) + (fTemp10 * fRec41[1]));
			fRec39[0] = ((fTemp9 * fTemp14) + (fTemp10 * fRec40[1]));
			fRec38[0] = ((fTemp9 * fTemp13) + (fTemp10 * fRec39[1]));
			fRec37[0] = ((fTemp9 * fTemp12) + (fTemp10 * fRec38[1]));
			fRec36[0] = ((fTemp9 * fTemp11) + (fTemp10 * fRec37[1]));
			float fTemp16 = (fRec32[0] * (((fSlow31 * fTemp6) + (fSlow32 * (fTemp6 + fVec6[1]))) + (fSlow33 * synth_glass_harm_faustpower2_f(fTemp6))));
			float fTemp17 = std::sin(fTemp16);
			float fTemp18 = std::cos(fTemp16);
			float fTemp19 = ((4.0f * (fTemp6 * fTemp18)) - (fTemp17 * fRec42[1]));
			float fTemp20 = ((fTemp18 * fTemp19) - (fTemp17 * fRec43[1]));
			float fTemp21 = ((fTemp18 * fTemp20) - (fTemp17 * fRec44[1]));
			float fTemp22 = ((fTemp18 * fTemp21) - (fTemp17 * fRec45[1]));
			float fTemp23 = ((fTemp18 * fTemp22) - (fTemp17 * fRec46[1]));
			fRec47[0] = ((fTemp18 * fTemp23) - (fTemp17 * fRec47[1]));
			fRec46[0] = ((fTemp17 * fTemp23) + (fTemp18 * fRec47[1]));
			fRec45[0] = ((fTemp17 * fTemp22) + (fTemp18 * fRec46[1]));
			fRec44[0] = ((fTemp17 * fTemp21) + (fTemp18 * fRec45[1]));
			fRec43[0] = ((fTemp17 * fTemp20) + (fTemp18 * fRec44[1]));
			fRec42[0] = ((fTemp17 * fTemp19) + (fTemp18 * fRec43[1]));
			float fTemp24 = ((fSlow3 * ((4.0f * (fTemp6 * fTemp9)) + (fRec36[1] * fTemp10))) + (fSlow30 * ((fRec32[0] * ((4.0f * (fTemp6 * fTemp17)) + (fRec42[1] * fTemp18))) + (4.0f * ((1.0f - fRec32[0]) * fTemp6)))));
			fVec7[(IOTA & 4095)] = fTemp24;
			float fTemp25 = fVec7[((IOTA - iSlow34) & 4095)];
			fVec8[(IOTA & 4095)] = (fSlow0 * (fRec10[0] * fTemp25));
			float fTemp26 = (0.300000012f * fVec8[((IOTA - iConst21) & 4095)]);
			fRec49[0] = (0.0f - (fConst28 * ((fConst29 * fRec49[1]) - (fRec5[1] + fRec5[2]))));
			fRec48[0] = ((fSlow42 * fRec48[1]) + (fSlow43 * (fRec5[1] + (fSlow44 * fRec49[0]))));
			fVec9[(IOTA & 16383)] = ((0.353553385f * fRec48[0]) + 9.99999968e-21f);
			float fTemp27 = ((0.600000024f * fRec8[1]) + (fTemp26 + fVec9[((IOTA - iConst31) & 16383)]));
			fVec10[(IOTA & 4095)] = fTemp27;
			fRec8[0] = fVec10[((IOTA - iConst32) & 4095)];
			float fRec9 = (0.0f - (0.600000024f * fTemp27));
			fRec53[0] = (0.0f - (fConst28 * ((fConst29 * fRec53[1]) - (fRec0[1] + fRec0[2]))));
			fRec52[0] = ((fSlow51 * fRec52[1]) + (fSlow52 * (fRec0[1] + (fSlow53 * fRec53[0]))));
			fVec11[(IOTA & 16383)] = ((0.353553385f * fRec52[0]) + 9.99999968e-21f);
			fVec12[(IOTA & 4095)] = (fSlow54 * (fRec10[0] * fTemp24));
			float fTemp28 = (0.300000012f * fVec12[((IOTA - iConst21) & 4095)]);
			float fTemp29 = ((fVec11[((IOTA - iConst38) & 16383)] + fTemp28) - (0.600000024f * fRec50[1]));
			fVec13[(IOTA & 2047)] = fTemp29;
			fRec50[0] = fVec13[((IOTA - iConst39) & 2047)];
			float fRec51 = (0.600000024f * fTemp29);
			fRec57[0] = (0.0f - (fConst28 * ((fConst29 * fRec57[1]) - (fRec7[1] + fRec7[2]))));
			fRec56[0] = ((fSlow61 * fRec56[1]) + (fSlow62 * (fRec7[1] + (fSlow63 * fRec57[0]))));
			fVec14[(IOTA & 32767)] = ((0.353553385f * fRec56[0]) + 9.99999968e-21f);
			float fTemp30 = (((0.600000024f * fRec54[1]) + fVec14[((IOTA - iConst45) & 32767)]) - fTemp26);
			fVec15[(IOTA & 2047)] = fTemp30;
			fRec54[0] = fVec15[((IOTA - iConst46) & 2047)];
			float fRec55 = (0.0f - (0.600000024f * fTemp30));
			fRec61[0] = (0.0f - (fConst28 * ((fConst29 * fRec61[1]) - (fRec3[1] + fRec3[2]))));
			fRec60[0] = ((fSlow70 * fRec60[1]) + (fSlow71 * (fRec3[1] + (fSlow72 * fRec61[0]))));
			fVec16[(IOTA & 32767)] = ((0.353553385f * fRec60[0]) + 9.99999968e-21f);
			float fTemp31 = (((0.600000024f * fRec58[1]) + fVec16[((IOTA - iConst52) & 32767)]) - fTemp26);
			fVec17[(IOTA & 4095)] = fTemp31;
			fRec58[0] = fVec17[((IOTA - iConst53) & 4095)];
			float fRec59 = (0.0f - (0.600000024f * fTemp31));
			fRec65[0] = (0.0f - (fConst28 * ((fConst29 * fRec65[1]) - (fRec1[1] + fRec1[2]))));
			fRec64[0] = ((fSlow79 * fRec64[1]) + (fSlow80 * (fRec1[1] + (fSlow81 * fRec65[0]))));
			fVec18[(IOTA & 32767)] = ((0.353553385f * fRec64[0]) + 9.99999968e-21f);
			float fTemp32 = (fTemp26 + ((0.600000024f * fRec62[1]) + fVec18[((IOTA - iConst59) & 32767)]));
			fVec19[(IOTA & 4095)] = fTemp32;
			fRec62[0] = fVec19[((IOTA - iConst60) & 4095)];
			float fRec63 = (0.0f - (0.600000024f * fTemp32));
			fRec69[0] = (0.0f - (fConst28 * ((fConst29 * fRec69[1]) - (fRec6[1] + fRec6[2]))));
			fRec68[0] = ((fSlow88 * fRec68[1]) + (fSlow89 * (fRec6[1] + (fSlow90 * fRec69[0]))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec68[0]) + 9.99999968e-21f);
			float fTemp33 = (fVec20[((IOTA - iConst66) & 16383)] - (fTemp28 + (0.600000024f * fRec66[1])));
			fVec21[(IOTA & 2047)] = fTemp33;
			fRec66[0] = fVec21[((IOTA - iConst67) & 2047)];
			float fRec67 = (0.600000024f * fTemp33);
			fRec73[0] = (0.0f - (fConst28 * ((fConst29 * fRec73[1]) - (fRec2[1] + fRec2[2]))));
			fRec72[0] = ((fSlow97 * fRec72[1]) + (fSlow98 * (fRec2[1] + (fSlow99 * fRec73[0]))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec72[0]) + 9.99999968e-21f);
			float fTemp34 = (fVec22[((IOTA - iConst73) & 16383)] - (fTemp28 + (0.600000024f * fRec70[1])));
			fVec23[(IOTA & 4095)] = fTemp34;
			fRec70[0] = fVec23[((IOTA - iConst74) & 4095)];
			float fRec71 = (0.600000024f * fTemp34);
			fRec77[0] = (0.0f - (fConst28 * ((fConst29 * fRec77[1]) - (fRec4[1] + fRec4[2]))));
			fRec76[0] = ((fSlow106 * fRec76[1]) + (fSlow107 * (fRec4[1] + (fSlow108 * fRec77[0]))));
			fVec24[(IOTA & 16383)] = ((0.353553385f * fRec76[0]) + 9.99999968e-21f);
			float fTemp35 = ((fTemp28 + fVec24[((IOTA - iConst80) & 16383)]) - (0.600000024f * fRec74[1]));
			fVec25[(IOTA & 4095)] = fTemp35;
			fRec74[0] = fVec25[((IOTA - iConst81) & 4095)];
			float fRec75 = (0.600000024f * fTemp35);
			float fTemp36 = (fRec74[1] + fRec70[1]);
			float fTemp37 = (fRec66[1] + fTemp36);
			fRec0[0] = (fRec8[1] + (fRec50[1] + (fRec55 + (fRec59 + (fRec9 + (fRec63 + (fRec67 + (fRec71 + (fRec75 + (fRec51 + (fRec54[1] + (fRec58[1] + (fTemp37 + fRec62[1])))))))))))));
			fRec1[0] = ((fRec50[1] + (fRec67 + (fRec71 + (fRec75 + (fRec51 + fTemp37))))) - (fRec8[1] + (fRec55 + (fRec59 + (fRec9 + (fRec63 + (fRec54[1] + (fRec62[1] + fRec58[1]))))))));
			float fTemp38 = (fRec70[1] + fRec66[1]);
			fRec2[0] = ((fRec8[1] + (fRec50[1] + (fRec9 + (fRec63 + (fRec75 + (fRec51 + (fRec74[1] + fRec62[1]))))))) - (fRec55 + (fRec59 + (fRec67 + (fRec71 + (fRec54[1] + (fTemp38 + fRec58[1])))))));
			fRec3[0] = ((fRec50[1] + (fRec55 + (fRec59 + (fRec75 + (fRec51 + (fRec54[1] + (fRec74[1] + fRec58[1]))))))) - (fRec8[1] + (fRec9 + (fRec63 + (fRec67 + (fRec71 + (fTemp38 + fRec62[1])))))));
			float fTemp39 = (fRec74[1] + fRec66[1]);
			fRec4[0] = ((fRec50[1] + (fRec59 + (fRec63 + (fRec71 + (fRec51 + (fRec58[1] + (fRec70[1] + fRec62[1]))))))) - (fRec8[1] + (fRec55 + (fRec9 + (fRec67 + (fRec75 + (fRec54[1] + fTemp39)))))));
			fRec5[0] = ((fRec8[1] + (fRec50[1] + (fRec55 + (fRec9 + (fRec71 + (fRec51 + (fRec70[1] + fRec54[1]))))))) - (fRec59 + (fRec63 + (fRec67 + (fRec75 + (fRec58[1] + (fRec62[1] + fTemp39)))))));
			fRec6[0] = ((fRec50[1] + (fRec55 + (fRec63 + (fRec67 + (fRec51 + (fRec54[1] + (fRec66[1] + fRec62[1]))))))) - (fRec8[1] + (fRec59 + (fRec9 + (fRec71 + (fRec75 + (fTemp36 + fRec58[1])))))));
			fRec7[0] = ((fRec8[1] + (fRec50[1] + (fRec59 + (fRec9 + (fRec67 + (fRec51 + (fRec66[1] + fRec58[1]))))))) - (fRec55 + (fRec63 + (fRec71 + (fRec75 + (fRec54[1] + (fTemp36 + fRec62[1])))))));
			float fTemp40 = (1.0f - fRec10[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow54 * (fTemp40 * fTemp24))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow0 * (fTemp40 * fTemp25))));
			fRec10[1] = fRec10[0];
			fRec19[1] = fRec19[0];
			iVec0[1] = iVec0[0];
			iRec20[1] = iRec20[0];
			iRec21[1] = iRec21[0];
			IOTA = (IOTA + 1);
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec11[1] = fRec11[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec12[1] = fRec12[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec13[1] = fRec13[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec14[1] = fRec14[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec15[1] = fRec15[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec16[1] = fRec16[0];
			fVec6[1] = fVec6[0];
			fRec32[1] = fRec32[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec8[1] = fRec8[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec50[1] = fRec50[0];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fRec54[1] = fRec54[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec58[1] = fRec58[0];
			fRec65[1] = fRec65[0];
			fRec64[1] = fRec64[0];
			fRec62[1] = fRec62[0];
			fRec69[1] = fRec69[0];
			fRec68[1] = fRec68[0];
			fRec66[1] = fRec66[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec70[1] = fRec70[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec74[1] = fRec74[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _synth_glass_harm_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_glass_harm_UI<T>::name(sym(synth_glass_harm));

typedef _synth_glass_harm_UI<synth_glass_harm> synth_glass_harm_UI;

class faust_synth_glass_harm_tilde : public FaustExternal<synth_glass_harm, synth_glass_harm_UI> {
public:
    faust_synth_glass_harm_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
