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
	
	int iRec35[2];
	
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
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			iRec35[l32] = 0;
		}
	}
	
	void fillsynth_glass_harmSIG0(int count, float* table) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec35[0] = (iRec35[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec35[0] + -1))));
			iRec35[1] = iRec35[0];
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
	
	int fSampleRate;
	float fConst1;
	float fConst4;
	FAUSTFLOAT fHslider0;
	float fConst5;
	float fConst7;
	float fConst8;
	float fRec11[2];
	float fRec10[2];
	int IOTA;
	float fVec0[16384];
	int iConst10;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fRec12[2];
	FAUSTFLOAT fEntry0;
	float fConst12;
	float fConst13;
	FAUSTFLOAT fEntry1;
	FAUSTFLOAT fCheckbox0;
	float fRec21[2];
	float fConst15;
	int iVec1[2];
	int iRec22[2];
	float fConst16;
	int iRec23[2];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fVec2[8192];
	float fConst17;
	FAUSTFLOAT fHslider6;
	float fConst18;
	float fConst19;
	float fRec20[3];
	float fRec19[2];
	float fRec13[2];
	float fVec3[4096];
	float fConst20;
	float fConst21;
	float fRec25[3];
	float fRec24[2];
	float fRec14[2];
	float fVec4[2048];
	float fConst22;
	float fConst23;
	float fRec27[3];
	float fRec26[2];
	float fRec15[2];
	float fVec5[1024];
	float fConst24;
	float fConst25;
	float fRec29[3];
	float fRec28[2];
	float fRec16[2];
	float fVec6[1024];
	float fConst26;
	float fConst27;
	float fRec31[3];
	float fRec30[2];
	float fRec17[2];
	float fConst28;
	float fRec33[3];
	float fRec18[2];
	float fVec7[2];
	FAUSTFLOAT fHslider7;
	float fRec34[2];
	float fConst29;
	FAUSTFLOAT fHslider8;
	float fRec37[2];
	float fRec36[2];
	float fRec43[2];
	float fRec42[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[2];
	float fRec38[2];
	float fRec49[2];
	float fRec48[2];
	float fRec47[2];
	float fRec46[2];
	float fRec45[2];
	float fRec44[2];
	float fVec8[4096];
	float fVec9[4096];
	int iConst30;
	float fVec10[2048];
	int iConst31;
	float fRec8[2];
	float fConst34;
	float fConst35;
	float fRec53[2];
	float fRec52[2];
	float fVec11[16384];
	int iConst37;
	float fVec12[2048];
	int iConst38;
	float fRec50[2];
	float fConst41;
	float fConst42;
	float fRec57[2];
	float fRec56[2];
	float fVec13[16384];
	int iConst44;
	float fVec14[4096];
	int iConst45;
	float fRec54[2];
	float fConst48;
	float fConst49;
	float fRec61[2];
	float fRec60[2];
	float fVec15[32768];
	int iConst51;
	float fConst52;
	FAUSTFLOAT fHslider9;
	float fVec16[4096];
	float fVec17[2048];
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
	float fVec21[4096];
	int iConst67;
	float fRec66[2];
	float fConst70;
	float fConst71;
	float fRec73[2];
	float fRec72[2];
	float fVec22[32768];
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
		fConst1 = std::cos((37699.1133f / fConst0));
		float fConst2 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst3 = ((0.0f - (6.90775537f * fConst2)) / fConst0);
		fConst4 = (0.25f * fConst3);
		fConst5 = (0.166666672f * fConst3);
		float fConst6 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst7 = (1.0f / (fConst6 + 1.0f));
		fConst8 = (1.0f - fConst6);
		float fConst9 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst10 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst2 - fConst9))));
		float fConst11 = (1.0f - (100.530968f / fConst0));
		fConst12 = synth_glass_harm_faustpower2_f(fConst11);
		fConst13 = (0.5f * (1.0f - fConst12));
		float fConst14 = (0.0199999996f * fConst0);
		fConst15 = (1.0f / std::max<float>(1.0f, fConst14));
		fConst16 = (1.0f / std::max<float>(1.0f, (0.00999999978f * fConst0)));
		fConst17 = (0.0022727272f * fConst0);
		fConst18 = (0.0f - (2.0f * fConst11));
		fConst19 = (2764.60156f / fConst0);
		fConst20 = (0.000979623874f * fConst0);
		fConst21 = (6413.87549f / fConst0);
		fConst22 = (0.000534759369f * fConst0);
		fConst23 = (11749.5566f / fConst0);
		fConst24 = (0.000342794461f * fConst0);
		fConst25 = (18329.3086f / fConst0);
		fConst26 = (0.00024229502f * fConst0);
		fConst27 = (25931.9629f / fConst0);
		fConst28 = (24881.4141f / fConst0);
		fConst29 = (1.0f / fConst0);
		iConst30 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst14)));
		iConst31 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst9 + -1.0f))));
		float fConst32 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst33 = ((0.0f - (6.90775537f * fConst32)) / fConst0);
		fConst34 = (0.25f * fConst33);
		fConst35 = (0.166666672f * fConst33);
		float fConst36 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst37 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst32 - fConst36))));
		iConst38 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst36 + -1.0f))));
		float fConst39 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst40 = ((0.0f - (6.90775537f * fConst39)) / fConst0);
		fConst41 = (0.25f * fConst40);
		fConst42 = (0.166666672f * fConst40);
		float fConst43 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst44 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst39 - fConst43))));
		iConst45 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst43 + -1.0f))));
		float fConst46 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst47 = ((0.0f - (6.90775537f * fConst46)) / fConst0);
		fConst48 = (0.25f * fConst47);
		fConst49 = (0.166666672f * fConst47);
		float fConst50 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst51 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst46 - fConst50))));
		fConst52 = (0.0011363636f * fConst0);
		iConst53 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst50 + -1.0f))));
		float fConst54 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst55 = ((0.0f - (6.90775537f * fConst54)) / fConst0);
		fConst56 = (0.25f * fConst55);
		fConst57 = (0.166666672f * fConst55);
		float fConst58 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst59 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst54 - fConst58))));
		iConst60 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst58 + -1.0f))));
		float fConst61 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst62 = ((0.0f - (6.90775537f * fConst61)) / fConst0);
		fConst63 = (0.25f * fConst62);
		fConst64 = (0.166666672f * fConst62);
		float fConst65 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst66 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst61 - fConst65))));
		iConst67 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst65 + -1.0f))));
		float fConst68 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst69 = ((0.0f - (6.90775537f * fConst68)) / fConst0);
		fConst70 = (0.25f * fConst69);
		fConst71 = (0.166666672f * fConst69);
		float fConst72 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst73 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst68 - fConst72))));
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
		fHslider0 = FAUSTFLOAT(0.35999999999999999f);
		fHslider1 = FAUSTFLOAT(0.59999999999999998f);
		fHslider2 = FAUSTFLOAT(0.13700000000000001f);
		fEntry0 = FAUSTFLOAT(0.0f);
		fEntry1 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(1.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.20000000000000001f);
		fHslider6 = FAUSTFLOAT(48.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(220.0f);
		fHslider9 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec11[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec10[l1] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 16384); l2 = (l2 + 1)) {
			fVec0[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec12[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec21[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iVec1[l5] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec22[l6] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec23[l7] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 8192); l8 = (l8 + 1)) {
			fVec2[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 3); l9 = (l9 + 1)) {
			fRec20[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec19[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec13[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 4096); l12 = (l12 + 1)) {
			fVec3[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec25[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec24[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec14[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2048); l16 = (l16 + 1)) {
			fVec4[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 3); l17 = (l17 + 1)) {
			fRec27[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec26[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec15[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 1024); l20 = (l20 + 1)) {
			fVec5[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec29[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec28[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec16[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 1024); l24 = (l24 + 1)) {
			fVec6[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 3); l25 = (l25 + 1)) {
			fRec31[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec30[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec17[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec33[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec18[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fVec7[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec34[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec37[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec36[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec43[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec42[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec41[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec40[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec39[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec38[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec49[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec48[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec47[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec46[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec45[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec44[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 4096); l47 = (l47 + 1)) {
			fVec8[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 4096); l48 = (l48 + 1)) {
			fVec9[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2048); l49 = (l49 + 1)) {
			fVec10[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec8[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec53[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec52[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 16384); l53 = (l53 + 1)) {
			fVec11[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2048); l54 = (l54 + 1)) {
			fVec12[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec50[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec57[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec56[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 16384); l58 = (l58 + 1)) {
			fVec13[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 4096); l59 = (l59 + 1)) {
			fVec14[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec54[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec61[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec60[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 32768); l63 = (l63 + 1)) {
			fVec15[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 4096); l64 = (l64 + 1)) {
			fVec16[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2048); l65 = (l65 + 1)) {
			fVec17[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec58[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec65[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec64[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 32768); l69 = (l69 + 1)) {
			fVec18[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 4096); l70 = (l70 + 1)) {
			fVec19[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec62[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec69[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec68[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 16384); l74 = (l74 + 1)) {
			fVec20[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 4096); l75 = (l75 + 1)) {
			fVec21[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec66[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec73[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec72[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 32768); l79 = (l79 + 1)) {
			fVec22[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 4096); l80 = (l80 + 1)) {
			fVec23[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec70[l81] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec77[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec76[l83] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 16384); l84 = (l84 + 1)) {
			fVec24[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 4096); l85 = (l85 + 1)) {
			fVec25[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec74[l86] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l87 = 0; (l87 < 3); l87 = (l87 + 1)) {
			fRec0[l87] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l88 = 0; (l88 < 3); l88 = (l88 + 1)) {
			fRec1[l88] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l89 = 0; (l89 < 3); l89 = (l89 + 1)) {
			fRec2[l89] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l90 = 0; (l90 < 3); l90 = (l90 + 1)) {
			fRec3[l90] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l91 = 0; (l91 < 3); l91 = (l91 + 1)) {
			fRec4[l91] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l92 = 0; (l92 < 3); l92 = (l92 + 1)) {
			fRec5[l92] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l93 = 0; (l93 < 3); l93 = (l93 + 1)) {
			fRec6[l93] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
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
		ui_interface->declare(&fHslider5, "tooltip", "Bow pressure on the instrument (Value between 0 and 1)");
		ui_interface->addHorizontalSlider("bow_press", &fHslider5, 0.200000003f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider3, "tooltip", "A value between 0 and 1");
		ui_interface->addHorizontalSlider("gain", &fHslider3, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fCheckbox0, "type", "float");
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->declare(&fHslider4, "tooltip", "A value between 0 and 1");
		ui_interface->addHorizontalSlider("integration", &fHslider4, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider8, "tooltip", "Frequency of the sine wave for the modulation of theta (works if Modulation Type=3)");
		ui_interface->declare(&fHslider8, "unit", "Hz");
		ui_interface->addHorizontalSlider("modfreq", &fHslider8, 220.0f, 20.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fEntry0, "tooltip", "0=theta is modulated by the incoming signal; 1=theta is modulated by the averaged incoming signal; 2=theta is modulated by the squared incoming signal; 3=theta is modulated by a sine wave of frequency freqMod; 4=theta is modulated by a sine wave of frequency freq;");
		ui_interface->declare(&fEntry0, "type", "int");
		ui_interface->addNumEntry("modtype", &fEntry0, 0.0f, 0.0f, 4.0f, 1.0f);
		ui_interface->declare(&fHslider7, "tooltip", "Nonlinearity factor (value between 0 and 1)");
		ui_interface->addHorizontalSlider("nonlin", &fHslider7, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pan", &fHslider1, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider6, 48.0f, 24.0f, 96.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider0, 0.360000014f, 0.00499999989f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fEntry1, "tooltip", "0=Bow; 1=Strike");
		ui_interface->declare(&fEntry1, "type", "int");
		ui_interface->addNumEntry("strike", &fEntry1, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addHorizontalSlider("width", &fHslider9, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::exp((fConst4 / fSlow0));
		float fSlow2 = synth_glass_harm_faustpower2_f(fSlow1);
		float fSlow3 = (1.0f - (fConst1 * fSlow2));
		float fSlow4 = (1.0f - fSlow2);
		float fSlow5 = (fSlow3 / fSlow4);
		float fSlow6 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow3) / synth_glass_harm_faustpower2_f(fSlow4)) + -1.0f)));
		float fSlow7 = (fSlow5 - fSlow6);
		float fSlow8 = (fSlow1 * (fSlow6 + (1.0f - fSlow5)));
		float fSlow9 = ((std::exp((fConst5 / fSlow0)) / fSlow1) + -1.0f);
		float fSlow10 = float(fHslider1);
		float fSlow11 = (1.0f - fSlow10);
		float fSlow12 = (0.00100000005f * float(fHslider2));
		int iSlow13 = int(float(fEntry0));
		float fSlow14 = float((iSlow13 >= 3));
		float fSlow15 = float(fEntry1);
		float fSlow16 = (0.166666672f * (0.0f - (fSlow15 + -1.0f)));
		float fSlow17 = float(fCheckbox0);
		float fSlow18 = (0.00100000005f * std::min<float>(1.0f, std::max<float>(0.0f, fSlow17)));
		int iSlow19 = (fSlow17 > 0.0f);
		int iSlow20 = (iSlow19 == 0);
		float fSlow21 = ((0.100000001f * float(fHslider3)) + 0.899999976f);
		float fSlow22 = float(fHslider4);
		float fSlow23 = (10.0f - (9.0f * float(fHslider5)));
		float fSlow24 = (0.200000003f * (fSlow15 * float(iSlow19)));
		float fSlow25 = std::pow(2.0f, (0.0833333358f * (float(fHslider6) + -69.0f)));
		int iSlow26 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst17 / fSlow25))));
		float fSlow27 = (fConst18 * std::cos((fConst19 * fSlow25)));
		int iSlow28 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst20 / fSlow25))));
		float fSlow29 = (fConst18 * std::cos((fConst21 * fSlow25)));
		int iSlow30 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst22 / fSlow25))));
		float fSlow31 = (fConst18 * std::cos((fConst23 * fSlow25)));
		int iSlow32 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst24 / fSlow25))));
		float fSlow33 = (fConst18 * std::cos((fConst25 * fSlow25)));
		int iSlow34 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst26 / fSlow25))));
		float fSlow35 = (fConst18 * std::cos((fConst27 * fSlow25)));
		float fSlow36 = (fConst18 * std::cos((fConst28 * fSlow25)));
		float fSlow37 = (0.00100000005f * float(fHslider7));
		float fSlow38 = (440.0f * (fSlow25 * float((iSlow13 == 4))));
		float fSlow39 = float((iSlow13 != 4));
		float fSlow40 = (0.00100000005f * float(fHslider8));
		float fSlow41 = float((iSlow13 < 3));
		float fSlow42 = (12.566371f * float((iSlow13 == 0)));
		float fSlow43 = (6.28318548f * float((iSlow13 == 1)));
		float fSlow44 = (50.2654839f * float((iSlow13 == 2)));
		float fSlow45 = std::exp((fConst34 / fSlow0));
		float fSlow46 = synth_glass_harm_faustpower2_f(fSlow45);
		float fSlow47 = (1.0f - (fConst1 * fSlow46));
		float fSlow48 = (1.0f - fSlow46);
		float fSlow49 = (fSlow47 / fSlow48);
		float fSlow50 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow47) / synth_glass_harm_faustpower2_f(fSlow48)) + -1.0f)));
		float fSlow51 = (fSlow49 - fSlow50);
		float fSlow52 = (fSlow45 * (fSlow50 + (1.0f - fSlow49)));
		float fSlow53 = ((std::exp((fConst35 / fSlow0)) / fSlow45) + -1.0f);
		float fSlow54 = std::exp((fConst41 / fSlow0));
		float fSlow55 = synth_glass_harm_faustpower2_f(fSlow54);
		float fSlow56 = (1.0f - (fConst1 * fSlow55));
		float fSlow57 = (1.0f - fSlow55);
		float fSlow58 = (fSlow56 / fSlow57);
		float fSlow59 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow56) / synth_glass_harm_faustpower2_f(fSlow57)) + -1.0f)));
		float fSlow60 = (fSlow58 - fSlow59);
		float fSlow61 = (fSlow54 * (fSlow59 + (1.0f - fSlow58)));
		float fSlow62 = ((std::exp((fConst42 / fSlow0)) / fSlow54) + -1.0f);
		float fSlow63 = std::exp((fConst48 / fSlow0));
		float fSlow64 = synth_glass_harm_faustpower2_f(fSlow63);
		float fSlow65 = (1.0f - (fConst1 * fSlow64));
		float fSlow66 = (1.0f - fSlow64);
		float fSlow67 = (fSlow65 / fSlow66);
		float fSlow68 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow65) / synth_glass_harm_faustpower2_f(fSlow66)) + -1.0f)));
		float fSlow69 = (fSlow67 - fSlow68);
		float fSlow70 = (fSlow63 * (fSlow68 + (1.0f - fSlow67)));
		float fSlow71 = ((std::exp((fConst49 / fSlow0)) / fSlow63) + -1.0f);
		int iSlow72 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst52 * (float(fHslider9) / fSlow25)))));
		float fSlow73 = std::exp((fConst56 / fSlow0));
		float fSlow74 = synth_glass_harm_faustpower2_f(fSlow73);
		float fSlow75 = (1.0f - (fConst1 * fSlow74));
		float fSlow76 = (1.0f - fSlow74);
		float fSlow77 = (fSlow75 / fSlow76);
		float fSlow78 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow75) / synth_glass_harm_faustpower2_f(fSlow76)) + -1.0f)));
		float fSlow79 = (fSlow77 - fSlow78);
		float fSlow80 = (fSlow73 * (fSlow78 + (1.0f - fSlow77)));
		float fSlow81 = ((std::exp((fConst57 / fSlow0)) / fSlow73) + -1.0f);
		float fSlow82 = std::exp((fConst63 / fSlow0));
		float fSlow83 = synth_glass_harm_faustpower2_f(fSlow82);
		float fSlow84 = (1.0f - (fConst1 * fSlow83));
		float fSlow85 = (1.0f - fSlow83);
		float fSlow86 = (fSlow84 / fSlow85);
		float fSlow87 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow84) / synth_glass_harm_faustpower2_f(fSlow85)) + -1.0f)));
		float fSlow88 = (fSlow86 - fSlow87);
		float fSlow89 = (fSlow82 * (fSlow87 + (1.0f - fSlow86)));
		float fSlow90 = ((std::exp((fConst64 / fSlow0)) / fSlow82) + -1.0f);
		float fSlow91 = std::exp((fConst70 / fSlow0));
		float fSlow92 = synth_glass_harm_faustpower2_f(fSlow91);
		float fSlow93 = (1.0f - (fConst1 * fSlow92));
		float fSlow94 = (1.0f - fSlow92);
		float fSlow95 = (fSlow93 / fSlow94);
		float fSlow96 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow93) / synth_glass_harm_faustpower2_f(fSlow94)) + -1.0f)));
		float fSlow97 = (fSlow95 - fSlow96);
		float fSlow98 = (fSlow91 * (fSlow96 + (1.0f - fSlow95)));
		float fSlow99 = ((std::exp((fConst71 / fSlow0)) / fSlow91) + -1.0f);
		float fSlow100 = std::exp((fConst77 / fSlow0));
		float fSlow101 = synth_glass_harm_faustpower2_f(fSlow100);
		float fSlow102 = (1.0f - (fConst1 * fSlow101));
		float fSlow103 = (1.0f - fSlow101);
		float fSlow104 = (fSlow102 / fSlow103);
		float fSlow105 = std::sqrt(std::max<float>(0.0f, ((synth_glass_harm_faustpower2_f(fSlow102) / synth_glass_harm_faustpower2_f(fSlow103)) + -1.0f)));
		float fSlow106 = (fSlow104 - fSlow105);
		float fSlow107 = (fSlow100 * (fSlow105 + (1.0f - fSlow104)));
		float fSlow108 = ((std::exp((fConst78 / fSlow0)) / fSlow100) + -1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec11[0] = (0.0f - (fConst7 * ((fConst8 * fRec11[1]) - (fRec0[1] + fRec0[2]))));
			fRec10[0] = ((fSlow7 * fRec10[1]) + (fSlow8 * (fRec0[1] + (fSlow9 * fRec11[0]))));
			fVec0[(IOTA & 16383)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			fRec12[0] = (fSlow12 + (0.999000013f * fRec12[1]));
			fRec21[0] = (fSlow18 + (0.999000013f * fRec21[1]));
			iVec1[0] = iSlow19;
			iRec22[0] = (iSlow19 + (iRec22[1] * (iVec1[1] >= iSlow19)));
			iRec23[0] = (iSlow20 * (iRec23[1] + 1));
			float fTemp0 = (((((0.100000001f * fRec21[0]) + 0.0299999993f) * std::max<float>(0.0f, (std::min<float>((fConst15 * float(iRec22[0])), 1.0f) - (fConst16 * float(iRec23[0]))))) - (fSlow21 * (((fRec13[1] + fRec15[1]) + fRec17[1]) + ((fRec14[1] + fRec16[1]) + fRec18[1])))) - fSlow22);
			float fTemp1 = synth_glass_harm_faustpower4_f((std::fabs((fSlow23 * fTemp0)) + 0.75f));
			float fTemp2 = (1.0f / fTemp1);
			float fTemp3 = ((fSlow16 * (fTemp0 * (float((fTemp2 > 1.0f)) + (float((fTemp2 <= 1.0f)) / fTemp1)))) + (fSlow24 * fRec21[0]));
			fVec2[(IOTA & 8191)] = (fRec19[1] + fTemp3);
			fRec20[0] = ((0.999000013f * fVec2[((IOTA - iSlow26) & 8191)]) - ((fSlow27 * fRec20[1]) + (fConst12 * fRec20[2])));
			fRec19[0] = (fConst13 * (fRec20[0] - fRec20[2]));
			fRec13[0] = fRec19[0];
			fVec3[(IOTA & 4095)] = (fTemp3 + fRec24[1]);
			fRec25[0] = ((0.998000979f * fVec3[((IOTA - iSlow28) & 4095)]) - ((fSlow29 * fRec25[1]) + (fConst12 * fRec25[2])));
			fRec24[0] = (fConst13 * (fRec25[0] - fRec25[2]));
			fRec14[0] = fRec24[0];
			fVec4[(IOTA & 2047)] = (fTemp3 + fRec26[1]);
			fRec27[0] = ((0.997003019f * fVec4[((IOTA - iSlow30) & 2047)]) - ((fSlow31 * fRec27[1]) + (fConst12 * fRec27[2])));
			fRec26[0] = (fConst13 * (fRec27[0] - fRec27[2]));
			fRec15[0] = fRec26[0];
			fVec5[(IOTA & 1023)] = (fTemp3 + fRec28[1]);
			fRec29[0] = ((0.996006012f * fVec5[((IOTA - iSlow32) & 1023)]) - ((fSlow33 * fRec29[1]) + (fConst12 * fRec29[2])));
			fRec28[0] = (fConst13 * (fRec29[0] - fRec29[2]));
			fRec16[0] = fRec28[0];
			fVec6[(IOTA & 1023)] = (fTemp3 + fRec30[1]);
			fRec31[0] = ((0.995010018f * fVec6[((IOTA - iSlow34) & 1023)]) - ((fSlow35 * fRec31[1]) + (fConst12 * fRec31[2])));
			fRec30[0] = (fConst13 * (fRec31[0] - fRec31[2]));
			fRec17[0] = fRec30[0];
			fRec33[0] = (-1.0f * ((fSlow36 * fRec33[1]) + (fConst12 * fRec33[2])));
			float fRec32 = (fConst13 * (fRec33[0] - fRec33[2]));
			fRec18[0] = fRec32;
			float fTemp4 = (fRec18[0] + (fRec16[0] + (((fRec13[0] + fRec15[0]) + fRec17[0]) + fRec14[0])));
			fVec7[0] = fTemp4;
			fRec34[0] = (fSlow37 + (0.999000013f * fRec34[1]));
			fRec37[0] = (fSlow40 + (0.999000013f * fRec37[1]));
			float fTemp5 = (fRec36[1] + (fConst29 * (fSlow38 + (fSlow39 * fRec37[0]))));
			fRec36[0] = (fTemp5 - std::floor(fTemp5));
			float fTemp6 = (3.14159274f * (fRec34[0] * ftbl0synth_glass_harmSIG0[int((65536.0f * fRec36[0]))]));
			float fTemp7 = std::sin(fTemp6);
			float fTemp8 = std::cos(fTemp6);
			float fTemp9 = ((4.0f * (fTemp4 * fTemp8)) - (fTemp7 * fRec38[1]));
			float fTemp10 = ((fTemp8 * fTemp9) - (fTemp7 * fRec39[1]));
			float fTemp11 = ((fTemp8 * fTemp10) - (fTemp7 * fRec40[1]));
			float fTemp12 = ((fTemp8 * fTemp11) - (fTemp7 * fRec41[1]));
			float fTemp13 = ((fTemp8 * fTemp12) - (fTemp7 * fRec42[1]));
			fRec43[0] = ((fTemp8 * fTemp13) - (fTemp7 * fRec43[1]));
			fRec42[0] = ((fTemp7 * fTemp13) + (fTemp8 * fRec43[1]));
			fRec41[0] = ((fTemp7 * fTemp12) + (fTemp8 * fRec42[1]));
			fRec40[0] = ((fTemp7 * fTemp11) + (fTemp8 * fRec41[1]));
			fRec39[0] = ((fTemp7 * fTemp10) + (fTemp8 * fRec40[1]));
			fRec38[0] = ((fTemp7 * fTemp9) + (fTemp8 * fRec39[1]));
			float fTemp14 = (fRec34[0] * (((fSlow42 * fTemp4) + (fSlow43 * (fTemp4 + fVec7[1]))) + (fSlow44 * synth_glass_harm_faustpower2_f(fTemp4))));
			float fTemp15 = std::sin(fTemp14);
			float fTemp16 = std::cos(fTemp14);
			float fTemp17 = ((4.0f * (fTemp4 * fTemp16)) - (fTemp15 * fRec44[1]));
			float fTemp18 = ((fTemp16 * fTemp17) - (fTemp15 * fRec45[1]));
			float fTemp19 = ((fTemp16 * fTemp18) - (fTemp15 * fRec46[1]));
			float fTemp20 = ((fTemp16 * fTemp19) - (fTemp15 * fRec47[1]));
			float fTemp21 = ((fTemp16 * fTemp20) - (fTemp15 * fRec48[1]));
			fRec49[0] = ((fTemp16 * fTemp21) - (fTemp15 * fRec49[1]));
			fRec48[0] = ((fTemp15 * fTemp21) + (fTemp16 * fRec49[1]));
			fRec47[0] = ((fTemp15 * fTemp20) + (fTemp16 * fRec48[1]));
			fRec46[0] = ((fTemp15 * fTemp19) + (fTemp16 * fRec47[1]));
			fRec45[0] = ((fTemp15 * fTemp18) + (fTemp16 * fRec46[1]));
			fRec44[0] = ((fTemp15 * fTemp17) + (fTemp16 * fRec45[1]));
			float fTemp22 = ((fSlow14 * ((4.0f * (fTemp4 * fTemp7)) + (fRec38[1] * fTemp8))) + (fSlow41 * ((fRec34[0] * ((4.0f * (fTemp4 * fTemp15)) + (fRec44[1] * fTemp16))) + (4.0f * ((1.0f - fRec34[0]) * fTemp4)))));
			fVec8[(IOTA & 4095)] = fTemp22;
			fVec9[(IOTA & 4095)] = (fSlow11 * (fRec12[0] * fTemp22));
			float fTemp23 = (0.300000012f * fVec9[((IOTA - iConst30) & 4095)]);
			float fTemp24 = ((fVec0[((IOTA - iConst10) & 16383)] + fTemp23) - (0.600000024f * fRec8[1]));
			fVec10[(IOTA & 2047)] = fTemp24;
			fRec8[0] = fVec10[((IOTA - iConst31) & 2047)];
			float fRec9 = (0.600000024f * fTemp24);
			fRec53[0] = (0.0f - (fConst7 * ((fConst8 * fRec53[1]) - (fRec6[1] + fRec6[2]))));
			fRec52[0] = ((fSlow51 * fRec52[1]) + (fSlow52 * (fRec6[1] + (fSlow53 * fRec53[0]))));
			fVec11[(IOTA & 16383)] = ((0.353553385f * fRec52[0]) + 9.99999968e-21f);
			float fTemp25 = (fVec11[((IOTA - iConst37) & 16383)] - (fTemp23 + (0.600000024f * fRec50[1])));
			fVec12[(IOTA & 2047)] = fTemp25;
			fRec50[0] = fVec12[((IOTA - iConst38) & 2047)];
			float fRec51 = (0.600000024f * fTemp25);
			fRec57[0] = (fConst7 * ((fRec2[1] + fRec2[2]) - (fConst8 * fRec57[1])));
			fRec56[0] = ((fSlow60 * fRec56[1]) + (fSlow61 * (fRec2[1] + (fSlow62 * fRec57[0]))));
			fVec13[(IOTA & 16383)] = ((0.353553385f * fRec56[0]) + 9.99999968e-21f);
			float fTemp26 = (fVec13[((IOTA - iConst44) & 16383)] - (fTemp23 + (0.600000024f * fRec54[1])));
			fVec14[(IOTA & 4095)] = fTemp26;
			fRec54[0] = fVec14[((IOTA - iConst45) & 4095)];
			float fRec55 = (0.600000024f * fTemp26);
			fRec61[0] = (0.0f - (fConst7 * ((fConst8 * fRec61[1]) - (fRec7[1] + fRec7[2]))));
			fRec60[0] = ((fSlow69 * fRec60[1]) + (fSlow70 * (fRec7[1] + (fSlow71 * fRec61[0]))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec60[0]) + 9.99999968e-21f);
			float fTemp27 = fVec8[((IOTA - iSlow72) & 4095)];
			fVec16[(IOTA & 4095)] = (fSlow10 * (fRec12[0] * fTemp27));
			float fTemp28 = (0.300000012f * fVec16[((IOTA - iConst30) & 4095)]);
			float fTemp29 = (((0.600000024f * fRec58[1]) + fVec15[((IOTA - iConst51) & 32767)]) - fTemp28);
			fVec17[(IOTA & 2047)] = fTemp29;
			fRec58[0] = fVec17[((IOTA - iConst53) & 2047)];
			float fRec59 = (0.0f - (0.600000024f * fTemp29));
			fRec65[0] = (0.0f - (fConst7 * ((fConst8 * fRec65[1]) - (fRec3[1] + fRec3[2]))));
			fRec64[0] = ((fSlow79 * fRec64[1]) + (fSlow80 * (fRec3[1] + (fSlow81 * fRec65[0]))));
			fVec18[(IOTA & 32767)] = ((0.353553385f * fRec64[0]) + 9.99999968e-21f);
			float fTemp30 = (((0.600000024f * fRec62[1]) + fVec18[((IOTA - iConst59) & 32767)]) - fTemp28);
			fVec19[(IOTA & 4095)] = fTemp30;
			fRec62[0] = fVec19[((IOTA - iConst60) & 4095)];
			float fRec63 = (0.0f - (0.600000024f * fTemp30));
			fRec69[0] = (0.0f - (fConst7 * ((fConst8 * fRec69[1]) - (fRec5[1] + fRec5[2]))));
			fRec68[0] = ((fSlow88 * fRec68[1]) + (fSlow89 * (fRec5[1] + (fSlow90 * fRec69[0]))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec68[0]) + 9.99999968e-21f);
			float fTemp31 = (fVec20[((IOTA - iConst66) & 16383)] + (fTemp28 + (0.600000024f * fRec66[1])));
			fVec21[(IOTA & 4095)] = fTemp31;
			fRec66[0] = fVec21[((IOTA - iConst67) & 4095)];
			float fRec67 = (0.0f - (0.600000024f * fTemp31));
			fRec73[0] = (0.0f - (fConst7 * ((fConst8 * fRec73[1]) - (fRec1[1] + fRec1[2]))));
			fRec72[0] = ((fSlow97 * fRec72[1]) + (fSlow98 * (fRec1[1] + (fSlow99 * fRec73[0]))));
			fVec22[(IOTA & 32767)] = ((0.353553385f * fRec72[0]) + 9.99999968e-21f);
			float fTemp32 = (fTemp28 + ((0.600000024f * fRec70[1]) + fVec22[((IOTA - iConst73) & 32767)]));
			fVec23[(IOTA & 4095)] = fTemp32;
			fRec70[0] = fVec23[((IOTA - iConst74) & 4095)];
			float fRec71 = (0.0f - (0.600000024f * fTemp32));
			fRec77[0] = (0.0f - (fConst7 * ((fConst8 * fRec77[1]) - (fRec4[1] + fRec4[2]))));
			fRec76[0] = ((fSlow106 * fRec76[1]) + (fSlow107 * (fRec4[1] + (fSlow108 * fRec77[0]))));
			fVec24[(IOTA & 16383)] = ((0.353553385f * fRec76[0]) + 9.99999968e-21f);
			float fTemp33 = ((fTemp23 + fVec24[((IOTA - iConst80) & 16383)]) - (0.600000024f * fRec74[1]));
			fVec25[(IOTA & 4095)] = fTemp33;
			fRec74[0] = fVec25[((IOTA - iConst81) & 4095)];
			float fRec75 = (0.600000024f * fTemp33);
			float fTemp34 = (fRec9 + fRec75);
			float fTemp35 = (fRec51 + (fRec55 + fTemp34));
			fRec0[0] = (fRec8[1] + (fRec50[1] + (fRec54[1] + (fRec58[1] + (fRec62[1] + (fRec66[1] + (fRec70[1] + (fRec74[1] + (fRec59 + (fRec63 + (fRec67 + (fRec71 + fTemp35))))))))))));
			fRec1[0] = ((fRec8[1] + (fRec50[1] + (fRec54[1] + (fRec74[1] + fTemp35)))) - (fRec58[1] + (fRec62[1] + (fRec66[1] + (fRec70[1] + (fRec59 + (fRec63 + (fRec71 + fRec67))))))));
			float fTemp36 = (fRec55 + fRec51);
			fRec2[0] = ((fRec8[1] + (fRec66[1] + (fRec70[1] + (fRec74[1] + (fRec67 + (fRec71 + fTemp34)))))) - (fRec50[1] + (fRec54[1] + (fRec58[1] + (fRec62[1] + (fRec59 + (fRec63 + fTemp36)))))));
			fRec3[0] = ((fRec8[1] + (fRec58[1] + (fRec62[1] + (fRec74[1] + (fRec59 + (fRec63 + fTemp34)))))) - (fRec50[1] + (fRec54[1] + (fRec66[1] + (fRec70[1] + (fRec67 + (fRec71 + fTemp36)))))));
			float fTemp37 = (fRec9 + fRec55);
			float fTemp38 = (fRec75 + fRec51);
			fRec4[0] = ((fRec8[1] + (fRec54[1] + (fRec62[1] + (fRec70[1] + (fRec63 + (fRec71 + fTemp37)))))) - (fRec50[1] + (fRec58[1] + (fRec66[1] + (fRec74[1] + (fRec59 + (fRec67 + fTemp38)))))));
			fRec5[0] = ((fRec8[1] + (fRec54[1] + (fRec58[1] + (fRec66[1] + (fRec59 + (fRec67 + fTemp37)))))) - (fRec50[1] + (fRec62[1] + (fRec70[1] + (fRec74[1] + (fRec63 + (fRec71 + fTemp38)))))));
			float fTemp39 = (fRec9 + fRec51);
			float fTemp40 = (fRec75 + fRec55);
			fRec6[0] = ((fRec8[1] + (fRec50[1] + (fRec58[1] + (fRec70[1] + (fRec59 + (fRec71 + fTemp39)))))) - (fRec54[1] + (fRec62[1] + (fRec66[1] + (fRec74[1] + (fRec63 + (fRec67 + fTemp40)))))));
			fRec7[0] = ((fRec8[1] + (fRec50[1] + (fRec62[1] + (fRec66[1] + (fRec63 + (fRec67 + fTemp39)))))) - (fRec54[1] + (fRec58[1] + (fRec70[1] + (fRec74[1] + (fRec59 + (fRec71 + fTemp40)))))));
			float fTemp41 = (1.0f - fRec12[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow11 * (fTemp41 * fTemp22))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow10 * (fTemp41 * fTemp27))));
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA = (IOTA + 1);
			fRec12[1] = fRec12[0];
			fRec21[1] = fRec21[0];
			iVec1[1] = iVec1[0];
			iRec22[1] = iRec22[0];
			iRec23[1] = iRec23[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec13[1] = fRec13[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec14[1] = fRec14[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec15[1] = fRec15[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec16[1] = fRec16[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec17[1] = fRec17[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec18[1] = fRec18[0];
			fVec7[1] = fVec7[0];
			fRec34[1] = fRec34[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
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
