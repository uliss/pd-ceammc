/* ------------------------------------------------------------
name: "fx_shimmer"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_shimmer_H__
#define  __fx_shimmer_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_shimmer_dsp.h **************************/
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

class fx_shimmer_dsp {

    public:

        fx_shimmer_dsp() {}
        virtual ~fx_shimmer_dsp() {}

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
        virtual fx_shimmer_dsp* clone() = 0;
    
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

class decorator_dsp : public fx_shimmer_dsp {

    protected:

        fx_shimmer_dsp* fDSP;

    public:

        decorator_dsp(fx_shimmer_dsp* fx_shimmer_dsp = nullptr):fDSP(fx_shimmer_dsp) {}
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
    
        virtual fx_shimmer_dsp* createDSPInstance() = 0;
    
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
/**************************  END  fx_shimmer_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_shimmer_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_shimmer : public fx_shimmer_dsp {
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

class fx_shimmerSIG0 {
	
  private:
	
	int iRec14[2];
	
  public:
	
	int getNumInputsfx_shimmerSIG0() {
		return 0;
	}
	int getNumOutputsfx_shimmerSIG0() {
		return 1;
	}
	int getInputRatefx_shimmerSIG0(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatefx_shimmerSIG0(int channel) {
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
	
	void instanceInitfx_shimmerSIG0(int sample_rate) {
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec14[l4] = 0;
		}
	}
	
	void fillfx_shimmerSIG0(int count, float* table) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec14[0] = (iRec14[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec14[0] + -1))));
			iRec14[1] = iRec14[0];
		}
	}

};

static fx_shimmerSIG0* newfx_shimmerSIG0() { return (fx_shimmerSIG0*)new fx_shimmerSIG0(); }
static void deletefx_shimmerSIG0(fx_shimmerSIG0* dsp) { delete dsp; }

static float fx_shimmer_faustpower2_f(float value) {
	return (value * value);
}
static float ftbl0fx_shimmerSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_shimmer
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_shimmer : public fx_shimmer_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	int fSampleRate;
	float fConst2;
	FAUSTFLOAT fHslider2;
	float fConst3;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fConst4;
	FAUSTFLOAT fHslider5;
	float fRec12[2];
	float fRec11[2];
	int IOTA;
	float fVec0[131072];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHslider7;
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	float fConst5;
	FAUSTFLOAT fHslider10;
	float fRec15[2];
	FAUSTFLOAT fHslider11;
	float fRec16[2];
	float fRec13[2];
	float fVec1[65536];
	int iConst7;
	float fVec2[4096];
	int iConst8;
	float fRec9[2];
	float fConst10;
	float fRec20[2];
	float fRec19[2];
	float fVec3[131072];
	float fRec22[2];
	float fRec21[2];
	float fVec4[32768];
	int iConst12;
	float fVec5[8192];
	int iConst13;
	float fRec17[2];
	float fConst15;
	float fRec26[2];
	float fRec25[2];
	float fVec6[131072];
	float fRec28[2];
	float fRec27[2];
	float fVec7[32768];
	int iConst17;
	float fVec8[8192];
	int iConst18;
	float fRec23[2];
	float fConst20;
	float fRec32[2];
	float fRec31[2];
	float fVec9[131072];
	float fRec34[2];
	float fRec33[2];
	float fVec10[65536];
	int iConst22;
	float fVec11[8192];
	int iConst23;
	float fRec29[2];
	float fConst25;
	float fRec38[2];
	float fRec37[2];
	float fVec12[131072];
	float fRec40[2];
	float fRec39[2];
	float fVec13[32768];
	int iConst27;
	float fVec14[4096];
	int iConst28;
	float fRec35[2];
	float fConst30;
	float fRec44[2];
	float fRec43[2];
	float fVec15[131072];
	float fRec46[2];
	float fRec45[2];
	float fVec16[65536];
	int iConst32;
	float fVec17[8192];
	int iConst33;
	float fRec41[2];
	float fConst35;
	float fRec50[2];
	float fRec49[2];
	float fVec18[131072];
	float fRec52[2];
	float fRec51[2];
	float fVec19[32768];
	int iConst37;
	float fVec20[8192];
	int iConst38;
	float fRec47[2];
	float fConst40;
	float fRec56[2];
	float fRec55[2];
	float fVec21[131072];
	float fRec58[2];
	float fRec57[2];
	float fVec22[32768];
	int iConst42;
	float fVec23[4096];
	int iConst43;
	float fRec53[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.1");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_shimmer.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
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
		m->declare("id", "fx.shimmer");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "fx_shimmer");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
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
		fx_shimmerSIG0* sig0 = newfx_shimmerSIG0();
		sig0->instanceInitfx_shimmerSIG0(sample_rate);
		sig0->fillfx_shimmerSIG0(65536, ftbl0fx_shimmerSIG0);
		deletefx_shimmerSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst2 = ((0.0f - (6.90775537f * fConst1)) / fConst0);
		fConst3 = (6.28318548f / fConst0);
		fConst4 = (3.14159274f / fConst0);
		fConst5 = (1.0f / fConst0);
		float fConst6 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst7 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst1 - fConst6))));
		iConst8 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst6 + -1.0f))));
		float fConst9 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst10 = ((0.0f - (6.90775537f * fConst9)) / fConst0);
		float fConst11 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst12 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst9 - fConst11))));
		iConst13 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst11 + -1.0f))));
		float fConst14 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst15 = ((0.0f - (6.90775537f * fConst14)) / fConst0);
		float fConst16 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst17 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst14 - fConst16))));
		iConst18 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst16 + -1.0f))));
		float fConst19 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst20 = ((0.0f - (6.90775537f * fConst19)) / fConst0);
		float fConst21 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst22 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst19 - fConst21))));
		iConst23 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst21 + -1.0f))));
		float fConst24 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst25 = ((0.0f - (6.90775537f * fConst24)) / fConst0);
		float fConst26 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst27 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst24 - fConst26))));
		iConst28 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst26 + -1.0f))));
		float fConst29 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst30 = ((0.0f - (6.90775537f * fConst29)) / fConst0);
		float fConst31 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst32 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst29 - fConst31))));
		iConst33 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst31 + -1.0f))));
		float fConst34 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst35 = ((0.0f - (6.90775537f * fConst34)) / fConst0);
		float fConst36 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst37 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst34 - fConst36))));
		iConst38 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst36 + -1.0f))));
		float fConst39 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst40 = ((0.0f - (6.90775537f * fConst39)) / fConst0);
		float fConst41 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst42 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst39 - fConst41))));
		iConst43 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst41 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(2.0f);
		fHslider3 = FAUSTFLOAT(6000.0f);
		fHslider4 = FAUSTFLOAT(3.0f);
		fHslider5 = FAUSTFLOAT(200.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.5f);
		fHslider10 = FAUSTFLOAT(0.10000000000000001f);
		fHslider11 = FAUSTFLOAT(1.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec12[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec11[l2] = 0.0f;
		}
		IOTA = 0;
		for (int l3 = 0; (l3 < 131072); l3 = (l3 + 1)) {
			fVec0[l3] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec15[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec16[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec13[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 65536); l8 = (l8 + 1)) {
			fVec1[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 4096); l9 = (l9 + 1)) {
			fVec2[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec20[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec19[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 131072); l13 = (l13 + 1)) {
			fVec3[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec22[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec21[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 32768); l16 = (l16 + 1)) {
			fVec4[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 8192); l17 = (l17 + 1)) {
			fVec5[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec26[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec25[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 131072); l21 = (l21 + 1)) {
			fVec6[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec28[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec27[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 32768); l24 = (l24 + 1)) {
			fVec7[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 8192); l25 = (l25 + 1)) {
			fVec8[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec23[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec32[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec31[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 131072); l29 = (l29 + 1)) {
			fVec9[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec34[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec33[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 65536); l32 = (l32 + 1)) {
			fVec10[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 8192); l33 = (l33 + 1)) {
			fVec11[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec29[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec38[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec37[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 131072); l37 = (l37 + 1)) {
			fVec12[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec40[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec39[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 32768); l40 = (l40 + 1)) {
			fVec13[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 4096); l41 = (l41 + 1)) {
			fVec14[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec35[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec44[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec43[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 131072); l45 = (l45 + 1)) {
			fVec15[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec46[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec45[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 65536); l48 = (l48 + 1)) {
			fVec16[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 8192); l49 = (l49 + 1)) {
			fVec17[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec41[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec50[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec49[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 131072); l53 = (l53 + 1)) {
			fVec18[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec52[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec51[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 32768); l56 = (l56 + 1)) {
			fVec19[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 8192); l57 = (l57 + 1)) {
			fVec20[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec47[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec56[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec55[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 131072); l61 = (l61 + 1)) {
			fVec21[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec58[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec57[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 32768); l64 = (l64 + 1)) {
			fVec22[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 4096); l65 = (l65 + 1)) {
			fVec23[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec53[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 3); l67 = (l67 + 1)) {
			fRec1[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 3); l68 = (l68 + 1)) {
			fRec2[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 3); l69 = (l69 + 1)) {
			fRec3[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec4[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec5[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec6[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec7[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec8[l74] = 0.0f;
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
	
	virtual fx_shimmer* clone() {
		return new fx_shimmer();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx_shimmer");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addHorizontalSlider("control", &fHslider9, 0.5f, 0.0f, 1.0f, 0.0500000007f);
		ui_interface->declare(&fHslider3, "unit", "Hz");
		ui_interface->addHorizontalSlider("damp_hf", &fHslider3, 6000.0f, 1500.0f, 47040.0f, 1.0f);
		ui_interface->declare(&fHslider4, "unit", "sec");
		ui_interface->addHorizontalSlider("decay_low", &fHslider4, 3.0f, 1.0f, 8.0f, 0.100000001f);
		ui_interface->declare(&fHslider2, "unit", "sec");
		ui_interface->addHorizontalSlider("decay_mid", &fHslider2, 2.0f, 1.0f, 8.0f, 0.100000001f);
		ui_interface->addHorizontalSlider("depth", &fHslider8, 0.0f, 0.0f, 1.0f, 0.0500000007f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider11, "unit", "sec");
		ui_interface->addHorizontalSlider("envelope", &fHslider11, 1.0f, 0.100000001f, 3.0f, 0.0500000007f);
		ui_interface->declare(&fHslider5, "unit", "Hz");
		ui_interface->addHorizontalSlider("freq_low", &fHslider5, 200.0f, 50.0f, 1000.0f, 1.0f);
		ui_interface->declare(&fHslider7, "type", "int");
		ui_interface->addHorizontalSlider("mode", &fHslider7, 0.0f, -3.0f, 3.0f, 0.0500000007f);
		ui_interface->addHorizontalSlider("ps_drywet", &fHslider1, 0.5f, 0.0f, 1.0f, 0.0500000007f);
		ui_interface->declare(&fHslider6, "unit", "semitone");
		ui_interface->addHorizontalSlider("shift", &fHslider6, 0.0f, -12.0f, 12.0f, 0.100000001f);
		ui_interface->declare(&fHslider10, "unit", "Hz");
		ui_interface->addHorizontalSlider("speed", &fHslider10, 0.100000001f, 0.100000001f, 10.0f, 0.0500000007f);
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
		float fSlow3 = (1.0f - fSlow2);
		float fSlow4 = float(fHslider2);
		float fSlow5 = std::exp((fConst2 / fSlow4));
		float fSlow6 = fx_shimmer_faustpower2_f(fSlow5);
		float fSlow7 = std::cos((fConst3 * float(fHslider3)));
		float fSlow8 = (1.0f - (fSlow6 * fSlow7));
		float fSlow9 = (1.0f - fSlow6);
		float fSlow10 = (fSlow8 / fSlow9);
		float fSlow11 = std::sqrt(std::max<float>(0.0f, ((fx_shimmer_faustpower2_f(fSlow8) / fx_shimmer_faustpower2_f(fSlow9)) + -1.0f)));
		float fSlow12 = (fSlow10 - fSlow11);
		float fSlow13 = (fSlow5 * (fSlow11 + (1.0f - fSlow10)));
		float fSlow14 = float(fHslider4);
		float fSlow15 = ((std::exp((fConst2 / fSlow14)) / fSlow5) + -1.0f);
		float fSlow16 = (1.0f / std::tan((fConst4 * float(fHslider5))));
		float fSlow17 = (1.0f / (fSlow16 + 1.0f));
		float fSlow18 = (1.0f - fSlow16);
		float fSlow19 = float(fHslider6);
		float fSlow20 = float(fHslider7);
		float fSlow21 = float((fSlow20 >= 0.0f));
		float fSlow22 = (0.666666687f * fSlow20);
		float fSlow23 = (fSlow22 + 2.0f);
		float fSlow24 = (fSlow21 * fSlow23);
		float fSlow25 = float((fSlow20 < 0.0f));
		float fSlow26 = (fSlow20 + 2.0f);
		float fSlow27 = (0.0833333358f * (fSlow19 * (fSlow24 + (fSlow25 * fSlow26))));
		float fSlow28 = float(fHslider8);
		float fSlow29 = float(fHslider9);
		float fSlow30 = (0.5f * fSlow29);
		float fSlow31 = (fConst5 * float(fHslider10));
		float fSlow32 = (6.0f * (1.0f - fSlow29));
		float fSlow33 = float(fHslider11);
		int iSlow34 = (std::fabs(fSlow33) < 1.1920929e-07f);
		float fSlow35 = (iSlow34 ? 0.0f : std::exp((0.0f - (fConst5 / (iSlow34 ? 1.0f : fSlow33)))));
		float fSlow36 = (1.0f - fSlow35);
		float fSlow37 = std::exp((fConst10 / fSlow4));
		float fSlow38 = fx_shimmer_faustpower2_f(fSlow37);
		float fSlow39 = (1.0f - (fSlow38 * fSlow7));
		float fSlow40 = (1.0f - fSlow38);
		float fSlow41 = (fSlow39 / fSlow40);
		float fSlow42 = std::sqrt(std::max<float>(0.0f, ((fx_shimmer_faustpower2_f(fSlow39) / fx_shimmer_faustpower2_f(fSlow40)) + -1.0f)));
		float fSlow43 = (fSlow41 - fSlow42);
		float fSlow44 = (fSlow37 * (fSlow42 + (1.0f - fSlow41)));
		float fSlow45 = ((std::exp((fConst10 / fSlow14)) / fSlow37) + -1.0f);
		float fSlow46 = (0.0833333358f * (fSlow19 * ((fSlow21 * ((0.25f * fSlow20) + 2.0f)) + (fSlow25 * (2.0f - fSlow22)))));
		float fSlow47 = std::exp((fConst15 / fSlow4));
		float fSlow48 = fx_shimmer_faustpower2_f(fSlow47);
		float fSlow49 = (1.0f - (fSlow48 * fSlow7));
		float fSlow50 = (1.0f - fSlow48);
		float fSlow51 = (fSlow49 / fSlow50);
		float fSlow52 = std::sqrt(std::max<float>(0.0f, ((fx_shimmer_faustpower2_f(fSlow49) / fx_shimmer_faustpower2_f(fSlow50)) + -1.0f)));
		float fSlow53 = (fSlow51 - fSlow52);
		float fSlow54 = (fSlow47 * (fSlow52 + (1.0f - fSlow51)));
		float fSlow55 = ((std::exp((fConst15 / fSlow14)) / fSlow47) + -1.0f);
		float fSlow56 = (fSlow21 * fSlow26);
		float fSlow57 = (0.0833333358f * (fSlow19 * (fSlow56 + (fSlow25 * ((0.333333343f * fSlow20) + 2.0f)))));
		float fSlow58 = std::exp((fConst20 / fSlow4));
		float fSlow59 = fx_shimmer_faustpower2_f(fSlow58);
		float fSlow60 = (1.0f - (fSlow59 * fSlow7));
		float fSlow61 = (1.0f - fSlow59);
		float fSlow62 = (fSlow60 / fSlow61);
		float fSlow63 = std::sqrt(std::max<float>(0.0f, ((fx_shimmer_faustpower2_f(fSlow60) / fx_shimmer_faustpower2_f(fSlow61)) + -1.0f)));
		float fSlow64 = (fSlow62 - fSlow63);
		float fSlow65 = (fSlow58 * (fSlow63 + (1.0f - fSlow62)));
		float fSlow66 = ((std::exp((fConst20 / fSlow14)) / fSlow58) + -1.0f);
		float fSlow67 = (fSlow25 * (2.0f - fSlow20));
		float fSlow68 = (0.0833333358f * (fSlow19 * (fSlow24 + fSlow67)));
		float fSlow69 = std::exp((fConst25 / fSlow4));
		float fSlow70 = fx_shimmer_faustpower2_f(fSlow69);
		float fSlow71 = (1.0f - (fSlow70 * fSlow7));
		float fSlow72 = (1.0f - fSlow70);
		float fSlow73 = (fSlow71 / fSlow72);
		float fSlow74 = std::sqrt(std::max<float>(0.0f, ((fx_shimmer_faustpower2_f(fSlow71) / fx_shimmer_faustpower2_f(fSlow72)) + -1.0f)));
		float fSlow75 = (fSlow73 - fSlow74);
		float fSlow76 = (fSlow69 * (fSlow74 + (1.0f - fSlow73)));
		float fSlow77 = ((std::exp((fConst25 / fSlow14)) / fSlow69) + -1.0f);
		float fSlow78 = (0.0833333358f * (fSlow19 * (fSlow56 + (fSlow25 * fSlow23))));
		float fSlow79 = std::exp((fConst30 / fSlow4));
		float fSlow80 = fx_shimmer_faustpower2_f(fSlow79);
		float fSlow81 = (1.0f - (fSlow80 * fSlow7));
		float fSlow82 = (1.0f - fSlow80);
		float fSlow83 = (fSlow81 / fSlow82);
		float fSlow84 = std::sqrt(std::max<float>(0.0f, ((fx_shimmer_faustpower2_f(fSlow81) / fx_shimmer_faustpower2_f(fSlow82)) + -1.0f)));
		float fSlow85 = (fSlow83 - fSlow84);
		float fSlow86 = (fSlow79 * (fSlow84 + (1.0f - fSlow83)));
		float fSlow87 = ((std::exp((fConst30 / fSlow14)) / fSlow79) + -1.0f);
		float fSlow88 = (0.0833333358f * (fSlow19 * (fSlow67 + (fSlow21 * ((0.5f * fSlow20) + 2.0f)))));
		float fSlow89 = std::exp((fConst35 / fSlow4));
		float fSlow90 = fx_shimmer_faustpower2_f(fSlow89);
		float fSlow91 = (1.0f - (fSlow90 * fSlow7));
		float fSlow92 = (1.0f - fSlow90);
		float fSlow93 = (fSlow91 / fSlow92);
		float fSlow94 = std::sqrt(std::max<float>(0.0f, ((fx_shimmer_faustpower2_f(fSlow91) / fx_shimmer_faustpower2_f(fSlow92)) + -1.0f)));
		float fSlow95 = (fSlow93 - fSlow94);
		float fSlow96 = (fSlow89 * (fSlow94 + (1.0f - fSlow93)));
		float fSlow97 = ((std::exp((fConst35 / fSlow14)) / fSlow89) + -1.0f);
		float fSlow98 = std::exp((fConst40 / fSlow4));
		float fSlow99 = fx_shimmer_faustpower2_f(fSlow98);
		float fSlow100 = (1.0f - (fSlow99 * fSlow7));
		float fSlow101 = (1.0f - fSlow99);
		float fSlow102 = (fSlow100 / fSlow101);
		float fSlow103 = std::sqrt(std::max<float>(0.0f, ((fx_shimmer_faustpower2_f(fSlow100) / fx_shimmer_faustpower2_f(fSlow101)) + -1.0f)));
		float fSlow104 = (fSlow102 - fSlow103);
		float fSlow105 = (fSlow98 * (fSlow103 + (1.0f - fSlow102)));
		float fSlow106 = ((std::exp((fConst40 / fSlow14)) / fSlow98) + -1.0f);
		float fSlow107 = (0.0833333358f * (fSlow19 * (fSlow56 + (fSlow25 * ((0.400000006f * fSlow20) + 2.0f)))));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec0[0] = (fSlow1 + (0.999000013f * fRec0[1]));
			float fTemp0 = float(input1[i]);
			float fTemp1 = (iSlow0 ? 0.0f : fTemp0);
			fRec12[0] = (0.0f - (fSlow17 * ((fSlow18 * fRec12[1]) - (fRec8[1] + fRec8[2]))));
			fRec11[0] = ((fSlow12 * fRec11[1]) + (fSlow13 * (fRec8[1] + (fSlow15 * fRec12[0]))));
			float fTemp2 = ((0.353553385f * fRec11[0]) + 9.99999968e-21f);
			fVec0[(IOTA & 131071)] = fTemp2;
			fRec15[0] = (fSlow31 + (fRec15[1] - std::floor((fSlow31 + fRec15[1]))));
			float fTemp3 = (fSlow30 * ftbl0fx_shimmerSIG0[int((65536.0f * fRec15[0]))]);
			float fTemp4 = std::fabs(fTemp2);
			fRec16[0] = std::max<float>(fTemp4, ((fSlow35 * fRec16[1]) + (fSlow36 * fTemp4)));
			fRec13[0] = std::fmod((fRec13[1] + (2049.0f - std::pow(2.0f, (fSlow27 * ((fSlow28 * (fTemp3 + (fSlow32 * fRec16[0]))) + 0.5f))))), 2048.0f);
			int iTemp5 = int(fRec13[0]);
			float fTemp6 = std::floor(fRec13[0]);
			float fTemp7 = std::min<float>((0.0009765625f * fRec13[0]), 1.0f);
			float fTemp8 = (fRec13[0] + 2048.0f);
			int iTemp9 = int(fTemp8);
			float fTemp10 = std::floor(fTemp8);
			fVec1[(IOTA & 65535)] = ((fSlow3 * fTemp2) + (fSlow2 * ((((fVec0[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp5))))) & 131071)] * (fTemp6 + (1.0f - fRec13[0]))) + ((fRec13[0] - fTemp6) * fVec0[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp5 + 1)))))) & 131071)])) * fTemp7) + (((fVec0[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp9))))) & 131071)] * (fTemp10 + (-2047.0f - fRec13[0]))) + ((fRec13[0] + (2048.0f - fTemp10)) * fVec0[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp9 + 1)))))) & 131071)])) * (1.0f - fTemp7)))));
			float fTemp11 = ((0.600000024f * fRec9[1]) + (fTemp1 + fVec1[((IOTA - iConst7) & 65535)]));
			fVec2[(IOTA & 4095)] = fTemp11;
			fRec9[0] = fVec2[((IOTA - iConst8) & 4095)];
			float fRec10 = (0.0f - (0.600000024f * fTemp11));
			fRec20[0] = (0.0f - (fSlow17 * ((fSlow18 * fRec20[1]) - (fRec6[1] + fRec6[2]))));
			fRec19[0] = ((fSlow43 * fRec19[1]) + (fSlow44 * (fRec6[1] + (fSlow45 * fRec20[0]))));
			float fTemp12 = ((0.353553385f * fRec19[0]) + 9.99999968e-21f);
			fVec3[(IOTA & 131071)] = fTemp12;
			float fTemp13 = std::fabs(fTemp12);
			fRec22[0] = std::max<float>(fTemp13, ((fSlow35 * fRec22[1]) + (fSlow36 * fTemp13)));
			fRec21[0] = std::fmod((fRec21[1] + (2049.0f - std::pow(2.0f, (fSlow46 * ((fSlow28 * (fTemp3 + (fSlow32 * fRec22[0]))) + 0.5f))))), 2048.0f);
			int iTemp14 = int(fRec21[0]);
			float fTemp15 = std::floor(fRec21[0]);
			float fTemp16 = std::min<float>((0.0009765625f * fRec21[0]), 1.0f);
			float fTemp17 = (fRec21[0] + 2048.0f);
			int iTemp18 = int(fTemp17);
			float fTemp19 = std::floor(fTemp17);
			fVec4[(IOTA & 32767)] = ((fSlow3 * fTemp12) + (fSlow2 * ((((fVec3[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp14))))) & 131071)] * (fTemp15 + (1.0f - fRec21[0]))) + ((fRec21[0] - fTemp15) * fVec3[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp14 + 1)))))) & 131071)])) * fTemp16) + (((fVec3[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp18))))) & 131071)] * (fTemp19 + (-2047.0f - fRec21[0]))) + ((fRec21[0] + (2048.0f - fTemp19)) * fVec3[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp18 + 1)))))) & 131071)])) * (1.0f - fTemp16)))));
			float fTemp20 = ((0.600000024f * fRec17[1]) + (fTemp1 + fVec4[((IOTA - iConst12) & 32767)]));
			fVec5[(IOTA & 8191)] = fTemp20;
			fRec17[0] = fVec5[((IOTA - iConst13) & 8191)];
			float fRec18 = (0.0f - (0.600000024f * fTemp20));
			float fTemp21 = float(input0[i]);
			float fTemp22 = (iSlow0 ? 0.0f : fTemp21);
			fRec26[0] = (0.0f - (fSlow17 * ((fSlow18 * fRec26[1]) - (fRec5[1] + fRec5[2]))));
			fRec25[0] = ((fSlow53 * fRec25[1]) + (fSlow54 * (fRec5[1] + (fSlow55 * fRec26[0]))));
			float fTemp23 = ((0.353553385f * fRec25[0]) + 9.99999968e-21f);
			fVec6[(IOTA & 131071)] = fTemp23;
			float fTemp24 = std::fabs(fTemp23);
			fRec28[0] = std::max<float>(fTemp24, ((fSlow35 * fRec28[1]) + (fSlow36 * fTemp24)));
			fRec27[0] = std::fmod((fRec27[1] + (2049.0f - std::pow(2.0f, (fSlow57 * ((fSlow28 * (fTemp3 + (fSlow32 * fRec28[0]))) + 0.5f))))), 2048.0f);
			int iTemp25 = int(fRec27[0]);
			float fTemp26 = std::floor(fRec27[0]);
			float fTemp27 = std::min<float>((0.0009765625f * fRec27[0]), 1.0f);
			float fTemp28 = (fRec27[0] + 2048.0f);
			int iTemp29 = int(fTemp28);
			float fTemp30 = std::floor(fTemp28);
			fVec7[(IOTA & 32767)] = ((fSlow3 * fTemp23) + (fSlow2 * ((((fVec6[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp25))))) & 131071)] * (fTemp26 + (1.0f - fRec27[0]))) + ((fRec27[0] - fTemp26) * fVec6[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp25 + 1)))))) & 131071)])) * fTemp27) + (((fVec6[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp29))))) & 131071)] * (fTemp30 + (-2047.0f - fRec27[0]))) + ((fRec27[0] + (2048.0f - fTemp30)) * fVec6[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp29 + 1)))))) & 131071)])) * (1.0f - fTemp27)))));
			float fTemp31 = ((fTemp22 + fVec7[((IOTA - iConst17) & 32767)]) - (0.600000024f * fRec23[1]));
			fVec8[(IOTA & 8191)] = fTemp31;
			fRec23[0] = fVec8[((IOTA - iConst18) & 8191)];
			float fRec24 = (0.600000024f * fTemp31);
			fRec32[0] = (0.0f - (fSlow17 * ((fSlow18 * fRec32[1]) - (fRec2[1] + fRec2[2]))));
			fRec31[0] = ((fSlow64 * fRec31[1]) + (fSlow65 * (fRec2[1] + (fSlow66 * fRec32[0]))));
			float fTemp32 = ((0.353553385f * fRec31[0]) + 9.99999968e-21f);
			fVec9[(IOTA & 131071)] = fTemp32;
			float fTemp33 = std::fabs(fTemp32);
			fRec34[0] = std::max<float>(fTemp33, ((fSlow35 * fRec34[1]) + (fSlow36 * fTemp33)));
			fRec33[0] = std::fmod((fRec33[1] + (2049.0f - std::pow(2.0f, (fSlow68 * ((fSlow28 * (fTemp3 + (fSlow32 * fRec34[0]))) + 0.5f))))), 2048.0f);
			int iTemp34 = int(fRec33[0]);
			float fTemp35 = std::floor(fRec33[0]);
			float fTemp36 = std::min<float>((0.0009765625f * fRec33[0]), 1.0f);
			float fTemp37 = (fRec33[0] + 2048.0f);
			int iTemp38 = int(fTemp37);
			float fTemp39 = std::floor(fTemp37);
			fVec10[(IOTA & 65535)] = ((fSlow3 * fTemp32) + (fSlow2 * ((((fVec9[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp34))))) & 131071)] * (fTemp35 + (1.0f - fRec33[0]))) + ((fRec33[0] - fTemp35) * fVec9[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp34 + 1)))))) & 131071)])) * fTemp36) + (((fVec9[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp38))))) & 131071)] * (fTemp39 + (-2047.0f - fRec33[0]))) + ((fRec33[0] + (2048.0f - fTemp39)) * fVec9[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp38 + 1)))))) & 131071)])) * (1.0f - fTemp36)))));
			float fTemp40 = (fVec10[((IOTA - iConst22) & 65535)] + (fTemp1 + (0.600000024f * fRec29[1])));
			fVec11[(IOTA & 8191)] = fTemp40;
			fRec29[0] = fVec11[((IOTA - iConst23) & 8191)];
			float fRec30 = (0.0f - (0.600000024f * fTemp40));
			fRec38[0] = (0.0f - (fSlow17 * ((fSlow18 * fRec38[1]) - (fRec7[1] + fRec7[2]))));
			fRec37[0] = ((fSlow75 * fRec37[1]) + (fSlow76 * (fRec7[1] + (fSlow77 * fRec38[0]))));
			float fTemp41 = ((0.353553385f * fRec37[0]) + 9.99999968e-21f);
			fVec12[(IOTA & 131071)] = fTemp41;
			float fTemp42 = std::fabs(fTemp41);
			fRec40[0] = std::max<float>(fTemp42, ((fSlow35 * fRec40[1]) + (fSlow36 * fTemp42)));
			fRec39[0] = std::fmod((fRec39[1] + (2049.0f - std::pow(2.0f, (fSlow78 * ((fSlow28 * (fTemp3 + (fSlow32 * fRec40[0]))) + 0.5f))))), 2048.0f);
			int iTemp43 = int(fRec39[0]);
			float fTemp44 = std::floor(fRec39[0]);
			float fTemp45 = std::min<float>((0.0009765625f * fRec39[0]), 1.0f);
			float fTemp46 = (fRec39[0] + 2048.0f);
			int iTemp47 = int(fTemp46);
			float fTemp48 = std::floor(fTemp46);
			fVec13[(IOTA & 32767)] = ((fSlow3 * fTemp41) + (fSlow2 * ((((fVec12[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp43))))) & 131071)] * (fTemp44 + (1.0f - fRec39[0]))) + ((fRec39[0] - fTemp44) * fVec12[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp43 + 1)))))) & 131071)])) * fTemp45) + (((fVec12[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp47))))) & 131071)] * (fTemp48 + (-2047.0f - fRec39[0]))) + ((fRec39[0] + (2048.0f - fTemp48)) * fVec12[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp47 + 1)))))) & 131071)])) * (1.0f - fTemp45)))));
			float fTemp49 = ((fTemp22 + fVec13[((IOTA - iConst27) & 32767)]) - (0.600000024f * fRec35[1]));
			fVec14[(IOTA & 4095)] = fTemp49;
			fRec35[0] = fVec14[((IOTA - iConst28) & 4095)];
			float fRec36 = (0.600000024f * fTemp49);
			fRec44[0] = (0.0f - (fSlow17 * ((fSlow18 * fRec44[1]) - (fRec4[1] + fRec4[2]))));
			fRec43[0] = ((fSlow85 * fRec43[1]) + (fSlow86 * (fRec4[1] + (fSlow87 * fRec44[0]))));
			float fTemp50 = ((0.353553385f * fRec43[0]) + 9.99999968e-21f);
			fVec15[(IOTA & 131071)] = fTemp50;
			float fTemp51 = std::fabs(fTemp50);
			fRec46[0] = std::max<float>(fTemp51, ((fSlow35 * fRec46[1]) + (fSlow36 * fTemp51)));
			fRec45[0] = std::fmod((fRec45[1] + (2049.0f - std::pow(2.0f, (fSlow88 * ((fSlow28 * (fTemp3 + (fSlow32 * fRec46[0]))) + 0.5f))))), 2048.0f);
			int iTemp52 = int(fRec45[0]);
			float fTemp53 = std::floor(fRec45[0]);
			float fTemp54 = std::min<float>((0.0009765625f * fRec45[0]), 1.0f);
			float fTemp55 = (fRec45[0] + 2048.0f);
			int iTemp56 = int(fTemp55);
			float fTemp57 = std::floor(fTemp55);
			fVec16[(IOTA & 65535)] = ((fSlow3 * fTemp50) + (fSlow2 * ((((fVec15[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp52))))) & 131071)] * (fTemp53 + (1.0f - fRec45[0]))) + ((fRec45[0] - fTemp53) * fVec15[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp52 + 1)))))) & 131071)])) * fTemp54) + (((fVec15[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp56))))) & 131071)] * (fTemp57 + (-2047.0f - fRec45[0]))) + ((fRec45[0] + (2048.0f - fTemp57)) * fVec15[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp56 + 1)))))) & 131071)])) * (1.0f - fTemp54)))));
			float fTemp58 = (fVec16[((IOTA - iConst32) & 65535)] + (fTemp1 + (0.600000024f * fRec41[1])));
			fVec17[(IOTA & 8191)] = fTemp58;
			fRec41[0] = fVec17[((IOTA - iConst33) & 8191)];
			float fRec42 = (0.0f - (0.600000024f * fTemp58));
			fRec50[0] = (0.0f - (fSlow17 * ((fSlow18 * fRec50[1]) - (fRec3[1] + fRec3[2]))));
			fRec49[0] = ((fSlow95 * fRec49[1]) + (fSlow96 * (fRec3[1] + (fSlow97 * fRec50[0]))));
			float fTemp59 = ((0.353553385f * fRec49[0]) + 9.99999968e-21f);
			fVec18[(IOTA & 131071)] = fTemp59;
			float fTemp60 = std::fabs(fTemp59);
			fRec52[0] = std::max<float>(fTemp60, ((fSlow35 * fRec52[1]) + (fSlow36 * fTemp60)));
			fRec51[0] = std::fmod((fRec51[1] + (2049.0f - std::pow(2.0f, (fSlow78 * ((fSlow28 * (fTemp3 + (fSlow32 * fRec52[0]))) + 0.5f))))), 2048.0f);
			int iTemp61 = int(fRec51[0]);
			float fTemp62 = std::floor(fRec51[0]);
			float fTemp63 = std::min<float>((0.0009765625f * fRec51[0]), 1.0f);
			float fTemp64 = (fRec51[0] + 2048.0f);
			int iTemp65 = int(fTemp64);
			float fTemp66 = std::floor(fTemp64);
			fVec19[(IOTA & 32767)] = ((fSlow3 * fTemp59) + (fSlow2 * ((((fVec18[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp61))))) & 131071)] * (fTemp62 + (1.0f - fRec51[0]))) + ((fRec51[0] - fTemp62) * fVec18[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp61 + 1)))))) & 131071)])) * fTemp63) + (((fVec18[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp65))))) & 131071)] * (fTemp66 + (-2047.0f - fRec51[0]))) + ((fRec51[0] + (2048.0f - fTemp66)) * fVec18[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp65 + 1)))))) & 131071)])) * (1.0f - fTemp63)))));
			float fTemp67 = ((fTemp22 + fVec19[((IOTA - iConst37) & 32767)]) - (0.600000024f * fRec47[1]));
			fVec20[(IOTA & 8191)] = fTemp67;
			fRec47[0] = fVec20[((IOTA - iConst38) & 8191)];
			float fRec48 = (0.600000024f * fTemp67);
			fRec56[0] = (0.0f - (fSlow17 * ((fSlow18 * fRec56[1]) - (fRec1[1] + fRec1[2]))));
			fRec55[0] = ((fSlow104 * fRec55[1]) + (fSlow105 * (fRec1[1] + (fSlow106 * fRec56[0]))));
			float fTemp68 = ((0.353553385f * fRec55[0]) + 9.99999968e-21f);
			fVec21[(IOTA & 131071)] = fTemp68;
			float fTemp69 = std::fabs(fTemp68);
			fRec58[0] = std::max<float>(fTemp69, ((fRec58[1] * fSlow35) + (fTemp69 * fSlow36)));
			fRec57[0] = std::fmod((fRec57[1] + (2049.0f - std::pow(2.0f, (fSlow107 * ((fSlow28 * ((fSlow32 * fRec58[0]) + fTemp3)) + 0.5f))))), 2048.0f);
			int iTemp70 = int(fRec57[0]);
			float fTemp71 = std::floor(fRec57[0]);
			float fTemp72 = std::min<float>((0.0009765625f * fRec57[0]), 1.0f);
			float fTemp73 = (fRec57[0] + 2048.0f);
			int iTemp74 = int(fTemp73);
			float fTemp75 = std::floor(fTemp73);
			fVec22[(IOTA & 32767)] = ((fSlow3 * fTemp68) + (fSlow2 * ((((fVec21[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp70))))) & 131071)] * (fTemp71 + (1.0f - fRec57[0]))) + ((fRec57[0] - fTemp71) * fVec21[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp70 + 1)))))) & 131071)])) * fTemp72) + (((fVec21[((IOTA - std::min<int>(65537, int(std::max<int>(0, int(iTemp74))))) & 131071)] * (fTemp75 + (-2047.0f - fRec57[0]))) + ((fRec57[0] + (2048.0f - fTemp75)) * fVec21[((IOTA - std::min<int>(65537, int(std::max<int>(0, int((iTemp74 + 1)))))) & 131071)])) * (1.0f - fTemp72)))));
			float fTemp76 = ((fTemp22 + fVec22[((IOTA - iConst42) & 32767)]) - (0.600000024f * fRec53[1]));
			fVec23[(IOTA & 4095)] = fTemp76;
			fRec53[0] = fVec23[((IOTA - iConst43) & 4095)];
			float fRec54 = (0.600000024f * fTemp76);
			float fTemp77 = (fRec48 + fRec53[1]);
			float fTemp78 = (fRec54 + (fTemp77 + fRec29[1]));
			fRec1[0] = (fRec9[1] + (fRec17[1] + (fRec23[1] + (fRec30 + (fRec36 + (fRec41[1] + (fRec35[1] + (fRec47[1] + (fRec10 + (fRec42 + (fRec18 + (fRec24 + fTemp78))))))))))));
			float fTemp79 = (fRec54 + fTemp77);
			fRec2[0] = ((fRec23[1] + (fRec36 + (fRec35[1] + (fRec47[1] + (fRec24 + fTemp79))))) - (fRec9[1] + (fRec17[1] + (fRec30 + (fRec41[1] + (fRec10 + (fRec42 + (fRec18 + fRec29[1]))))))));
			float fTemp80 = (fRec54 + (fRec53[1] + fRec29[1]));
			fRec3[0] = ((fRec17[1] + (fRec23[1] + (fRec30 + (fRec18 + (fRec24 + fTemp80))))) - (fRec9[1] + (fRec36 + (fRec41[1] + (fRec35[1] + (fRec47[1] + (fRec10 + (fRec48 + fRec42))))))));
			float fTemp81 = (fRec54 + fRec53[1]);
			float fTemp82 = (fRec48 + fRec29[1]);
			fRec4[0] = ((fRec9[1] + (fRec23[1] + (fRec41[1] + (fRec10 + (fRec42 + (fRec24 + fTemp81)))))) - (fRec17[1] + (fRec30 + (fRec36 + (fRec35[1] + (fRec47[1] + (fRec18 + fTemp82)))))));
			fRec5[0] = ((fRec30 + (fRec41[1] + (fRec47[1] + (fRec42 + fTemp78)))) - (fRec9[1] + (fRec17[1] + (fRec23[1] + (fRec36 + (fRec35[1] + (fRec10 + (fRec24 + fRec18))))))));
			fRec6[0] = ((fRec9[1] + (fRec17[1] + (fRec47[1] + (fRec10 + (fRec18 + fTemp79))))) - (fRec23[1] + (fRec30 + (fRec36 + (fRec41[1] + (fRec35[1] + (fRec42 + (fRec24 + fRec29[1]))))))));
			fRec7[0] = ((fRec9[1] + (fRec30 + (fRec36 + (fRec35[1] + (fRec10 + fTemp80))))) - (fRec17[1] + (fRec23[1] + (fRec41[1] + (fRec47[1] + (fRec42 + (fRec18 + (fRec24 + fRec48))))))));
			fRec8[0] = ((fRec17[1] + (fRec36 + (fRec41[1] + (fRec35[1] + (fRec42 + (fRec18 + fTemp81)))))) - (fRec9[1] + (fRec23[1] + (fRec30 + (fRec47[1] + (fRec10 + (fRec24 + fTemp82)))))));
			float fTemp83 = (1.0f - fRec0[0]);
			output0[i] = FAUSTFLOAT((iSlow0 ? fTemp21 : ((fRec0[0] * (((fRec1[0] + fRec3[0]) + fRec5[0]) + fRec7[0])) + (fTemp83 * fTemp22))));
			output1[i] = FAUSTFLOAT((iSlow0 ? fTemp0 : ((fTemp83 * fTemp1) + (fRec0[0] * (((fRec2[0] + fRec4[0]) + fRec6[0]) + fRec8[0])))));
			fRec0[1] = fRec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			IOTA = (IOTA + 1);
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec13[1] = fRec13[0];
			fRec9[1] = fRec9[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec17[1] = fRec17[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec23[1] = fRec23[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec29[1] = fRec29[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec35[1] = fRec35[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec41[1] = fRec41[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec47[1] = fRec47[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec58[1] = fRec58[0];
			fRec57[1] = fRec57[0];
			fRec53[1] = fRec53[0];
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
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_shimmer_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_shimmer_UI<T>::name(sym(fx_shimmer));

typedef _fx_shimmer_UI<fx_shimmer> fx_shimmer_UI;

class faust_fx_shimmer_tilde : public FaustExternal<fx_shimmer, fx_shimmer_UI> {
public:
    faust_fx_shimmer_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
