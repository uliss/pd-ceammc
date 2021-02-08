/* ------------------------------------------------------------
name: "fx.freeverb2"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_freeverb2_H__
#define  __fx_freeverb2_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_freeverb2_dsp.h **************************/
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

class fx_freeverb2_dsp {

    public:

        fx_freeverb2_dsp() {}
        virtual ~fx_freeverb2_dsp() {}

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
        virtual fx_freeverb2_dsp* clone() = 0;
    
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

class decorator_dsp : public fx_freeverb2_dsp {

    protected:

        fx_freeverb2_dsp* fDSP;

    public:

        decorator_dsp(fx_freeverb2_dsp* fx_freeverb2_dsp = nullptr):fDSP(fx_freeverb2_dsp) {}
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
    
        virtual fx_freeverb2_dsp* createDSPInstance() = 0;
    
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
/**************************  END  fx_freeverb2_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_freeverb2_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_freeverb2 : public fx_freeverb2_dsp {
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


#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_freeverb2
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_freeverb2 : public fx_freeverb2_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider2;
	float fRec12[2];
	FAUSTFLOAT fHslider3;
	float fRec11[2];
	float fRec10[2];
	FAUSTFLOAT fHslider4;
	float fRec14[2];
	FAUSTFLOAT fHslider5;
	float fRec13[2];
	float fRec15[2];
	int IOTA;
	float fVec0[8192];
	int iConst2;
	float fRec9[2];
	float fRec17[2];
	float fVec1[8192];
	int iConst3;
	float fRec16[2];
	float fRec19[2];
	float fVec2[8192];
	int iConst4;
	float fRec18[2];
	float fRec21[2];
	float fVec3[8192];
	int iConst5;
	float fRec20[2];
	float fRec23[2];
	float fVec4[8192];
	int iConst6;
	float fRec22[2];
	float fRec25[2];
	float fVec5[8192];
	int iConst7;
	float fRec24[2];
	float fRec27[2];
	float fVec6[8192];
	int iConst8;
	float fRec26[2];
	float fRec29[2];
	float fVec7[8192];
	int iConst9;
	float fRec28[2];
	float fVec8[2048];
	int iConst10;
	int iConst11;
	float fRec7[2];
	float fVec9[2048];
	int iConst12;
	int iConst13;
	float fRec5[2];
	float fVec10[2048];
	int iConst14;
	int iConst15;
	float fRec3[2];
	float fVec11[1024];
	int iConst16;
	int iConst17;
	float fRec1[2];
	float fRec39[2];
	float fVec12[8192];
	float fConst18;
	FAUSTFLOAT fHslider6;
	float fRec38[2];
	float fRec41[2];
	float fVec13[8192];
	float fRec40[2];
	float fRec43[2];
	float fVec14[8192];
	float fRec42[2];
	float fRec45[2];
	float fVec15[8192];
	float fRec44[2];
	float fRec47[2];
	float fVec16[8192];
	float fRec46[2];
	float fRec49[2];
	float fVec17[8192];
	float fRec48[2];
	float fRec51[2];
	float fVec18[8192];
	float fRec50[2];
	float fRec53[2];
	float fVec19[8192];
	float fRec52[2];
	float fVec20[2048];
	float fRec36[2];
	float fVec21[2048];
	float fRec34[2];
	float fVec22[2048];
	float fRec32[2];
	float fVec23[2048];
	float fRec30[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_freeverb2.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "fx.freeverb2");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
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
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (1.0f / fConst0);
		iConst2 = int((0.0253061224f * fConst0));
		iConst3 = int((0.0269387756f * fConst0));
		iConst4 = int((0.0289569162f * fConst0));
		iConst5 = int((0.0307482984f * fConst0));
		iConst6 = int((0.0322448984f * fConst0));
		iConst7 = int((0.033809524f * fConst0));
		iConst8 = int((0.0353061222f * fConst0));
		iConst9 = int((0.0366666652f * fConst0));
		iConst10 = int((0.0126077095f * fConst0));
		iConst11 = std::min<int>(1024, std::max<int>(0, (iConst10 + -1)));
		iConst12 = int((0.00999999978f * fConst0));
		iConst13 = std::min<int>(1024, std::max<int>(0, (iConst12 + -1)));
		iConst14 = int((0.00773242628f * fConst0));
		iConst15 = std::min<int>(1024, std::max<int>(0, (iConst14 + -1)));
		iConst16 = int((0.00510204071f * fConst0));
		iConst17 = std::min<int>(1024, std::max<int>(0, (iConst16 + -1)));
		fConst18 = (0.00104308384f * fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.33000000000000002f);
		fHslider1 = FAUSTFLOAT(100.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.0001f);
		fHslider4 = FAUSTFLOAT(0.5f);
		fHslider5 = FAUSTFLOAT(0.999f);
		fHslider6 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec12[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec11[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec10[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec14[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec13[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec15[l6] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 8192); l7 = (l7 + 1)) {
			fVec0[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec9[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec17[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 8192); l10 = (l10 + 1)) {
			fVec1[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec16[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec19[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 8192); l13 = (l13 + 1)) {
			fVec2[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec18[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec21[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 8192); l16 = (l16 + 1)) {
			fVec3[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec20[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec23[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 8192); l19 = (l19 + 1)) {
			fVec4[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec22[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec25[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 8192); l22 = (l22 + 1)) {
			fVec5[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec24[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec27[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 8192); l25 = (l25 + 1)) {
			fVec6[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec26[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec29[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 8192); l28 = (l28 + 1)) {
			fVec7[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec28[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2048); l30 = (l30 + 1)) {
			fVec8[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec7[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2048); l32 = (l32 + 1)) {
			fVec9[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec5[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2048); l34 = (l34 + 1)) {
			fVec10[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec3[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 1024); l36 = (l36 + 1)) {
			fVec11[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec1[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec39[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 8192); l39 = (l39 + 1)) {
			fVec12[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec38[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec41[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 8192); l42 = (l42 + 1)) {
			fVec13[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec40[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec43[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 8192); l45 = (l45 + 1)) {
			fVec14[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec42[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec45[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 8192); l48 = (l48 + 1)) {
			fVec15[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec44[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec47[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 8192); l51 = (l51 + 1)) {
			fVec16[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec46[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec49[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 8192); l54 = (l54 + 1)) {
			fVec17[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec48[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec51[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 8192); l57 = (l57 + 1)) {
			fVec18[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec50[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec53[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 8192); l60 = (l60 + 1)) {
			fVec19[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec52[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 2048); l62 = (l62 + 1)) {
			fVec20[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec36[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 2048); l64 = (l64 + 1)) {
			fVec21[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec34[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 2048); l66 = (l66 + 1)) {
			fVec22[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec32[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 2048); l68 = (l68 + 1)) {
			fVec23[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec30[l69] = 0.0f;
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
	
	virtual fx_freeverb2* clone() {
		return new fx_freeverb2();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.freeverb2");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addHorizontalSlider("damp", &fHslider2, 0.5f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, 0.330000013f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("fr_damp", &fHslider3, 9.99999975e-05f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->addHorizontalSlider("fr_room", &fHslider5, 0.999000013f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "ms");
		ui_interface->addHorizontalSlider("fr_time", &fHslider1, 100.0f, 0.0f, 1000.0f, 0.00100000005f);
		ui_interface->declare(&fCheckbox1, "type", "bool");
		ui_interface->addCheckButton("freeze", &fCheckbox1);
		ui_interface->addHorizontalSlider("room", &fHslider4, 0.5f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("spread", &fHslider6, 0.5f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = (0.00100000005f * float(fHslider0));
		float fSlow2 = (0.00100000005f * float(fHslider1));
		int iSlow3 = (std::fabs(fSlow2) < 1.1920929e-07f);
		float fSlow4 = (iSlow3 ? 0.0f : std::exp((0.0f - (fConst1 / (iSlow3 ? 1.0f : fSlow2)))));
		int iSlow5 = int(float(fCheckbox1));
		float fSlow6 = (0.00100000005f * float(fHslider2));
		float fSlow7 = float(fHslider3);
		float fSlow8 = (1.0f - fSlow4);
		float fSlow9 = (0.00100000005f * float(fHslider4));
		float fSlow10 = float(fHslider5);
		float fSlow11 = (float((1 - iSlow5)) * fSlow8);
		int iSlow12 = int((fConst18 * float(fHslider6)));
		int iSlow13 = (iConst9 + iSlow12);
		int iSlow14 = (iConst8 + iSlow12);
		int iSlow15 = (iConst7 + iSlow12);
		int iSlow16 = (iConst6 + iSlow12);
		int iSlow17 = (iConst2 + iSlow12);
		int iSlow18 = (iConst3 + iSlow12);
		int iSlow19 = (iConst4 + iSlow12);
		int iSlow20 = (iConst5 + iSlow12);
		int iSlow21 = (iSlow12 + -1);
		int iSlow22 = std::min<int>(1024, std::max<int>(0, (iConst10 + iSlow21)));
		int iSlow23 = std::min<int>(1024, std::max<int>(0, (iConst12 + iSlow21)));
		int iSlow24 = std::min<int>(1024, std::max<int>(0, (iConst14 + iSlow21)));
		int iSlow25 = std::min<int>(1024, std::max<int>(0, (iConst16 + iSlow21)));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			float fTemp1 = (iSlow0 ? 0.0f : fTemp0);
			fRec0[0] = (fSlow1 + (0.999000013f * fRec0[1]));
			float fTemp2 = (1.0f - fRec0[0]);
			fRec12[0] = (fSlow6 + (0.999000013f * fRec12[1]));
			fRec11[0] = ((fRec11[1] * fSlow4) + ((iSlow5 ? fSlow7 : fRec12[0]) * fSlow8));
			float fTemp3 = (1.0f - fRec11[0]);
			fRec10[0] = ((fRec11[0] * fRec10[1]) + (fTemp3 * fRec9[1]));
			fRec14[0] = (fSlow9 + (0.999000013f * fRec14[1]));
			fRec13[0] = ((fSlow4 * fRec13[1]) + ((iSlow5 ? fSlow10 : fRec14[0]) * fSlow8));
			fRec15[0] = ((fSlow4 * fRec15[1]) + fSlow11);
			float fTemp4 = float(input1[i]);
			float fTemp5 = (iSlow0 ? 0.0f : fTemp4);
			float fTemp6 = (fRec15[0] * (fTemp1 + fTemp5));
			fVec0[(IOTA & 8191)] = ((fRec10[0] * fRec13[0]) + fTemp6);
			fRec9[0] = fVec0[((IOTA - iConst2) & 8191)];
			fRec17[0] = ((fRec11[0] * fRec17[1]) + (fTemp3 * fRec16[1]));
			fVec1[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec17[0]));
			fRec16[0] = fVec1[((IOTA - iConst3) & 8191)];
			fRec19[0] = ((fRec11[0] * fRec19[1]) + (fTemp3 * fRec18[1]));
			fVec2[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec19[0]));
			fRec18[0] = fVec2[((IOTA - iConst4) & 8191)];
			fRec21[0] = ((fRec11[0] * fRec21[1]) + (fTemp3 * fRec20[1]));
			fVec3[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec21[0]));
			fRec20[0] = fVec3[((IOTA - iConst5) & 8191)];
			fRec23[0] = ((fRec11[0] * fRec23[1]) + (fTemp3 * fRec22[1]));
			fVec4[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec23[0]));
			fRec22[0] = fVec4[((IOTA - iConst6) & 8191)];
			fRec25[0] = ((fRec11[0] * fRec25[1]) + (fTemp3 * fRec24[1]));
			fVec5[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec25[0]));
			fRec24[0] = fVec5[((IOTA - iConst7) & 8191)];
			fRec27[0] = ((fRec11[0] * fRec27[1]) + (fTemp3 * fRec26[1]));
			fVec6[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec27[0]));
			fRec26[0] = fVec6[((IOTA - iConst8) & 8191)];
			fRec29[0] = ((fRec11[0] * fRec29[1]) + (fTemp3 * fRec28[1]));
			fVec7[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec29[0]));
			fRec28[0] = fVec7[((IOTA - iConst9) & 8191)];
			float fTemp7 = ((((((((fRec9[0] + fRec16[0]) + fRec18[0]) + fRec20[0]) + fRec22[0]) + fRec24[0]) + fRec26[0]) + fRec28[0]) + (0.5f * fRec7[1]));
			fVec8[(IOTA & 2047)] = fTemp7;
			fRec7[0] = fVec8[((IOTA - iConst11) & 2047)];
			float fRec8 = (0.0f - (0.5f * fTemp7));
			float fTemp8 = (fRec7[1] + (fRec8 + (0.5f * fRec5[1])));
			fVec9[(IOTA & 2047)] = fTemp8;
			fRec5[0] = fVec9[((IOTA - iConst13) & 2047)];
			float fRec6 = (0.0f - (0.5f * fTemp8));
			float fTemp9 = (fRec5[1] + (fRec6 + (0.5f * fRec3[1])));
			fVec10[(IOTA & 2047)] = fTemp9;
			fRec3[0] = fVec10[((IOTA - iConst15) & 2047)];
			float fRec4 = (0.0f - (0.5f * fTemp9));
			float fTemp10 = (fRec3[1] + (fRec4 + (0.5f * fRec1[1])));
			fVec11[(IOTA & 1023)] = fTemp10;
			fRec1[0] = fVec11[((IOTA - iConst17) & 1023)];
			float fRec2 = (0.0f - (0.5f * fTemp10));
			output0[i] = FAUSTFLOAT((iSlow0 ? fTemp0 : ((fTemp1 * fTemp2) + (fRec0[0] * (fRec2 + fRec1[1])))));
			fRec39[0] = ((fRec11[0] * fRec39[1]) + (fTemp3 * fRec38[1]));
			fVec12[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec39[0]));
			fRec38[0] = fVec12[((IOTA - iSlow13) & 8191)];
			fRec41[0] = ((fRec11[0] * fRec41[1]) + (fTemp3 * fRec40[1]));
			fVec13[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec41[0]));
			fRec40[0] = fVec13[((IOTA - iSlow14) & 8191)];
			fRec43[0] = ((fRec11[0] * fRec43[1]) + (fTemp3 * fRec42[1]));
			fVec14[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec43[0]));
			fRec42[0] = fVec14[((IOTA - iSlow15) & 8191)];
			fRec45[0] = ((fRec11[0] * fRec45[1]) + (fTemp3 * fRec44[1]));
			fVec15[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec45[0]));
			fRec44[0] = fVec15[((IOTA - iSlow16) & 8191)];
			fRec47[0] = ((fRec11[0] * fRec47[1]) + (fTemp3 * fRec46[1]));
			fVec16[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec47[0]));
			fRec46[0] = fVec16[((IOTA - iSlow17) & 8191)];
			fRec49[0] = ((fRec11[0] * fRec49[1]) + (fTemp3 * fRec48[1]));
			fVec17[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec49[0]));
			fRec48[0] = fVec17[((IOTA - iSlow18) & 8191)];
			fRec51[0] = ((fRec11[0] * fRec51[1]) + (fTemp3 * fRec50[1]));
			fVec18[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec51[0]));
			fRec50[0] = fVec18[((IOTA - iSlow19) & 8191)];
			fRec53[0] = ((fRec11[0] * fRec53[1]) + (fTemp3 * fRec52[1]));
			fVec19[(IOTA & 8191)] = (fTemp6 + (fRec13[0] * fRec53[0]));
			fRec52[0] = fVec19[((IOTA - iSlow20) & 8191)];
			float fTemp11 = (fRec38[0] + (fRec40[0] + (fRec42[0] + (fRec44[0] + ((((fRec46[0] + fRec48[0]) + fRec50[0]) + fRec52[0]) + (0.5f * fRec36[1]))))));
			fVec20[(IOTA & 2047)] = fTemp11;
			fRec36[0] = fVec20[((IOTA - iSlow22) & 2047)];
			float fRec37 = (0.0f - (0.5f * fTemp11));
			float fTemp12 = (fRec37 + ((0.5f * fRec34[1]) + fRec36[1]));
			fVec21[(IOTA & 2047)] = fTemp12;
			fRec34[0] = fVec21[((IOTA - iSlow23) & 2047)];
			float fRec35 = (0.0f - (0.5f * fTemp12));
			float fTemp13 = (fRec35 + ((0.5f * fRec32[1]) + fRec34[1]));
			fVec22[(IOTA & 2047)] = fTemp13;
			fRec32[0] = fVec22[((IOTA - iSlow24) & 2047)];
			float fRec33 = (0.0f - (0.5f * fTemp13));
			float fTemp14 = (fRec33 + ((0.5f * fRec30[1]) + fRec32[1]));
			fVec23[(IOTA & 2047)] = fTemp14;
			fRec30[0] = fVec23[((IOTA - iSlow25) & 2047)];
			float fRec31 = (0.0f - (0.5f * fTemp14));
			output1[i] = FAUSTFLOAT((iSlow0 ? fTemp4 : ((fTemp2 * fTemp5) + (fRec0[0] * (fRec31 + fRec30[1])))));
			fRec0[1] = fRec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec15[1] = fRec15[0];
			IOTA = (IOTA + 1);
			fRec9[1] = fRec9[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec7[1] = fRec7[0];
			fRec5[1] = fRec5[0];
			fRec3[1] = fRec3[0];
			fRec1[1] = fRec1[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec36[1] = fRec36[0];
			fRec34[1] = fRec34[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_freeverb2_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_freeverb2_UI<T>::name(sym(fx_freeverb2));

typedef _fx_freeverb2_UI<fx_freeverb2> fx_freeverb2_UI;

class faust_fx_freeverb2_tilde : public FaustExternal<fx_freeverb2, fx_freeverb2_UI> {
public:
    faust_fx_freeverb2_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
