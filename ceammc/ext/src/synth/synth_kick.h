/* ------------------------------------------------------------
name: "synth.kick"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_kick_H__
#define  __synth_kick_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_kick_dsp.h **************************/
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

class synth_kick_dsp {

    public:

        synth_kick_dsp() {}
        virtual ~synth_kick_dsp() {}

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
        virtual synth_kick_dsp* clone() = 0;
    
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

class decorator_dsp : public synth_kick_dsp {

    protected:

        synth_kick_dsp* fDSP;

    public:

        decorator_dsp(synth_kick_dsp* synth_kick_dsp = nullptr):fDSP(synth_kick_dsp) {}
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
    
        virtual synth_kick_dsp* createDSPInstance() = 0;
    
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
/**************************  END  synth_kick_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_kick_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_kick : public synth_kick_dsp {
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

static float synth_kick_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_kick
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_kick : public synth_kick_dsp {
	
 private:
	
	int iVec0[2];
	FAUSTFLOAT fCheckbox0;
	float fVec1[2];
	int iVec2[2];
	float fRec0[2];
	int fSampleRate;
	float fConst3;
	float fConst4;
	FAUSTFLOAT fVslider0;
	float fRec3[2];
	int iRec1[2];
	float fConst5;
	FAUSTFLOAT fVslider1;
	float fRec2[2];
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst10;
	float fConst11;
	float fRec8[2];
	float fRec9[2];
	FAUSTFLOAT fVslider2;
	float fConst12;
	float fRec10[2];
	float fRec6[2];
	float fRec7[2];
	float fRec5[2];
	float fConst13;
	float fConst14;
	float fRec4[3];
	float fConst15;
	float fRec13[2];
	int iRec11[2];
	float fRec12[2];
	float fConst16;
	float fConst17;
	float fRec16[2];
	float fConst20;
	float fConst22;
	float fConst24;
	int iRec19[2];
	float fVec3[2];
	float fConst25;
	float fConst26;
	float fRec18[2];
	float fConst27;
	float fConst28;
	float fRec17[3];
	float fConst29;
	float fVec4[2];
	float fRec15[2];
	float fRec14[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("filename", "synth_kick.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "synth.kick");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
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
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::tan((3141.59277f / fConst0));
		float fConst2 = (1.0f / fConst1);
		fConst3 = (1.0f / (((fConst2 + 1.0f) / fConst1) + 1.0f));
		fConst4 = (0.000500000024f * fConst0);
		fConst5 = (0.00039999999f * fConst0);
		fConst6 = (1.0f / (fConst2 + 1.0f));
		fConst7 = (1.0f - fConst2);
		fConst8 = (6.28318548f / fConst0);
		float fConst9 = (31.415926f / fConst0);
		fConst10 = std::sin(fConst9);
		fConst11 = std::cos(fConst9);
		fConst12 = (1.0f / float(int((0.0199999996f * fConst0))));
		fConst13 = (((fConst2 + -1.0f) / fConst1) + 1.0f);
		fConst14 = (2.0f * (1.0f - (1.0f / synth_kick_faustpower2_f(fConst1))));
		fConst15 = (0.00100000005f * fConst0);
		fConst16 = (3.14159274f / fConst0);
		fConst17 = (1.0f / float(int((0.0299999993f * fConst0))));
		float fConst18 = std::tan((1570.79639f / fConst0));
		float fConst19 = (1.0f / fConst18);
		fConst20 = (1.0f / (((fConst19 + 1.0f) / fConst18) + 1.0f));
		float fConst21 = synth_kick_faustpower2_f(fConst18);
		fConst22 = (1.0f / fConst21);
		float fConst23 = (fConst19 + 1.0f);
		fConst24 = (1.0f / (fConst18 * fConst23));
		fConst25 = (0.0f - fConst24);
		fConst26 = ((1.0f - fConst19) / fConst23);
		fConst27 = (((fConst19 + -1.0f) / fConst18) + 1.0f);
		fConst28 = (2.0f * (1.0f - fConst22));
		fConst29 = (0.0f - (2.0f / fConst21));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fVslider0 = FAUSTFLOAT(10.0f);
		fVslider1 = FAUSTFLOAT(1000.0f);
		fVslider2 = FAUSTFLOAT(50.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fVec1[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iVec2[l2] = 0;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec0[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec1[l5] = 0;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec2[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec8[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec9[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec10[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec6[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec7[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec5[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec4[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec13[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			iRec11[l15] = 0;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec12[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			iRec19[l18] = 0;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fVec3[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec18[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec17[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fVec4[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec15[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 3); l24 = (l24 + 1)) {
			fRec14[l24] = 0.0f;
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
	
	virtual synth_kick* clone() {
		return new synth_kick();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.kick");
		ui_interface->declare(&fVslider0, "unit", "ms");
		ui_interface->addVerticalSlider("attack", &fVslider0, 10.0f, 1.0f, 50.0f, 0.100000001f);
		ui_interface->declare(&fVslider2, "unit", "hz");
		ui_interface->addVerticalSlider("freq", &fVslider2, 50.0f, 10.0f, 5000.0f, 0.100000001f);
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->declare(&fVslider1, "unit", "ms");
		ui_interface->addVerticalSlider("release", &fVslider1, 1000.0f, 10.0f, 8000.0f, 0.100000001f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fCheckbox0);
		float fSlow1 = std::min<float>(1.0f, std::max<float>(0.0f, fSlow0));
		float fSlow2 = float(fVslider0);
		float fSlow3 = (fConst4 * fSlow2);
		float fSlow4 = (1.0f / float(int(fSlow3)));
		float fSlow5 = (1.0f / (fSlow3 + float(((0.000500000024f * fSlow2) == 0.0f))));
		float fSlow6 = float(fVslider1);
		float fSlow7 = (1.0f - (1.0f / std::pow(1000.0f, (1.0f / ((fConst5 * fSlow6) + float(((0.00039999999f * fSlow6) == 0.0f)))))));
		float fSlow8 = float(fVslider2);
		float fSlow9 = (fConst15 * fSlow2);
		float fSlow10 = (1.0f / float(int(fSlow9)));
		float fSlow11 = (1.0f / (fSlow9 + float(((0.00100000005f * fSlow2) == 0.0f))));
		float fSlow12 = (1.0f - (1.0f / std::pow(1000.0f, (1.0f / ((fConst15 * fSlow6) + float(((0.00100000005f * fSlow6) == 0.0f)))))));
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fVec1[0] = fSlow0;
			int iTemp0 = (fSlow0 > fVec1[1]);
			iVec2[0] = iTemp0;
			int iTemp1 = ((iVec2[1] <= 0) & (iTemp0 > 0));
			fRec0[0] = ((fRec0[1] * float((1 - iTemp1))) + (fSlow1 * float(iTemp1)));
			float fTemp2 = float(iTemp0);
			fRec3[0] = ((fTemp2 + fRec3[1]) - (fSlow4 * float((fRec3[1] > 0.0f))));
			int iTemp3 = (fRec3[0] > 0.0f);
			int iTemp4 = (iTemp3 > 0);
			iRec1[0] = (iTemp4 & (iRec1[1] | (fRec2[1] >= 1.0f)));
			int iTemp5 = ((iTemp3 <= 0) & (fRec2[1] > 0.0f));
			fRec2[0] = (((fSlow5 * float((((iRec1[1] == 0) & iTemp4) & (fRec2[1] < 1.0f)))) + (fRec2[1] * (1.0f - (fSlow7 * float(iTemp5))))) * float(((iTemp5 == 0) | (fRec2[1] >= 9.99999997e-07f))));
			fRec8[0] = ((fConst10 * fRec9[1]) + (fConst11 * fRec8[1]));
			float fTemp6 = float((1 - iVec0[1]));
			fRec9[0] = ((fTemp6 + (fConst11 * fRec9[1])) - (fConst10 * fRec8[1]));
			fRec10[0] = ((fRec10[1] + fTemp2) - (fConst12 * float((fRec10[1] > 0.0f))));
			float fTemp7 = (fConst8 * ((19.2307701f * fRec8[0]) + (fSlow8 * (fRec10[0] + 1.0f))));
			float fTemp8 = std::sin(fTemp7);
			float fTemp9 = std::cos(fTemp7);
			fRec6[0] = ((fRec7[1] * fTemp8) + (fRec6[1] * fTemp9));
			fRec7[0] = ((fTemp6 + (fRec7[1] * fTemp9)) - (fTemp8 * fRec6[1]));
			fRec5[0] = (0.0f - (fConst6 * ((fConst7 * fRec5[1]) - (fRec6[0] + fRec6[1]))));
			fRec4[0] = (fRec5[0] - (fConst3 * ((fConst13 * fRec4[2]) + (fConst14 * fRec4[1]))));
			fRec13[0] = ((fTemp2 + fRec13[1]) - (fSlow10 * float((fRec13[1] > 0.0f))));
			int iTemp10 = (fRec13[0] > 0.0f);
			int iTemp11 = (iTemp10 > 0);
			iRec11[0] = (iTemp11 & (iRec11[1] | (fRec12[1] >= 1.0f)));
			int iTemp12 = ((iTemp10 <= 0) & (fRec12[1] > 0.0f));
			fRec12[0] = (((fSlow11 * float((((iRec11[1] == 0) & iTemp11) & (fRec12[1] < 1.0f)))) + (fRec12[1] * (1.0f - (fSlow12 * float(iTemp12))))) * float(((iTemp12 == 0) | (fRec12[1] >= 9.99999997e-07f))));
			fRec16[0] = ((fTemp2 + fRec16[1]) - (fConst17 * float((fRec16[1] > 0.0f))));
			float fTemp13 = std::tan((fConst16 * ((5500.0f * fRec16[0]) + 500.0f)));
			float fTemp14 = (1.0f / fTemp13);
			iRec19[0] = ((1103515245 * iRec19[1]) + 12345);
			float fTemp15 = float(iRec19[0]);
			fVec3[0] = fTemp15;
			fRec18[0] = ((1.39698383e-11f * ((fConst24 * fTemp15) + (fConst25 * fVec3[1]))) - (fConst26 * fRec18[1]));
			fRec17[0] = (fRec18[0] - (fConst20 * ((fConst27 * fRec17[2]) + (fConst28 * fRec17[1]))));
			float fTemp16 = (((fConst22 * fRec17[0]) + (fConst29 * fRec17[1])) + (fConst22 * fRec17[2]));
			fVec4[0] = fTemp16;
			fRec15[0] = (0.0f - (((fRec15[1] * (1.0f - fTemp14)) - (fConst20 * (fTemp16 + fVec4[1]))) / (fTemp14 + 1.0f)));
			float fTemp17 = (((fTemp14 + 1.0f) / fTemp13) + 1.0f);
			fRec14[0] = (fRec15[0] - (((fRec14[2] * (((fTemp14 + -1.0f) / fTemp13) + 1.0f)) + (2.0f * (fRec14[1] * (1.0f - (1.0f / synth_kick_faustpower2_f(fTemp13)))))) / fTemp17));
			output0[i] = FAUSTFLOAT((0.502377272f * (fRec0[0] * ((fConst3 * (fRec2[0] * (fRec4[2] + (fRec4[0] + (2.0f * fRec4[1]))))) + ((fRec12[0] * (fRec14[2] + (fRec14[0] + (2.0f * fRec14[1])))) / fTemp17)))));
			iVec0[1] = iVec0[0];
			fVec1[1] = fVec1[0];
			iVec2[1] = iVec2[0];
			fRec0[1] = fRec0[0];
			fRec3[1] = fRec3[0];
			iRec1[1] = iRec1[0];
			fRec2[1] = fRec2[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec13[1] = fRec13[0];
			iRec11[1] = iRec11[0];
			fRec12[1] = fRec12[0];
			fRec16[1] = fRec16[0];
			iRec19[1] = iRec19[0];
			fVec3[1] = fVec3[0];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fVec4[1] = fVec4[0];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _synth_kick_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_kick_UI<T>::name(sym(synth_kick));

typedef _synth_kick_UI<synth_kick> synth_kick_UI;

class faust_synth_kick_tilde : public FaustExternal<synth_kick, synth_kick_UI> {
public:
    faust_synth_kick_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
