/* ------------------------------------------------------------
name: "fx_reverb_duck"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_reverb_duck_H__
#define  __fx_reverb_duck_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_reverb_duck_dsp.h **************************/
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

class fx_reverb_duck_dsp {

    public:

        fx_reverb_duck_dsp() {}
        virtual ~fx_reverb_duck_dsp() {}

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
        virtual fx_reverb_duck_dsp* clone() = 0;
    
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

class decorator_dsp : public fx_reverb_duck_dsp {

    protected:

        fx_reverb_duck_dsp* fDSP;

    public:

        decorator_dsp(fx_reverb_duck_dsp* fx_reverb_duck_dsp = nullptr):fDSP(fx_reverb_duck_dsp) {}
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
    
        virtual fx_reverb_duck_dsp* createDSPInstance() = 0;
    
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
/**************************  END  fx_reverb_duck_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_reverb_duck_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_reverb_duck : public fx_reverb_duck_dsp {
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

static float fx_reverb_duck_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_reverb_duck
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_reverb_duck : public fx_reverb_duck_dsp {
	
 private:
	
	int IOTA;
	float fVec0[4096];
	int fSampleRate;
	float fConst2;
	FAUSTFLOAT fVslider0;
	float fConst3;
	FAUSTFLOAT fVslider1;
	float fRec10[2];
	float fVec1[65536];
	int iConst5;
	int iConst6;
	float fVec2[4096];
	int iConst7;
	float fRec8[2];
	float fConst9;
	float fRec13[2];
	float fVec3[65536];
	int iConst11;
	float fVec4[8192];
	int iConst12;
	float fRec11[2];
	float fConst14;
	float fRec16[2];
	float fVec5[32768];
	int iConst16;
	float fVec6[8192];
	int iConst17;
	float fRec14[2];
	float fConst19;
	float fRec19[2];
	float fVec7[65536];
	int iConst21;
	float fVec8[8192];
	int iConst22;
	float fRec17[2];
	float fConst24;
	float fRec22[2];
	float fVec9[32768];
	int iConst26;
	float fVec10[4096];
	int iConst27;
	float fRec20[2];
	float fConst29;
	float fRec25[2];
	float fVec11[32768];
	int iConst31;
	float fVec12[8192];
	int iConst32;
	float fRec23[2];
	float fConst34;
	float fRec28[2];
	float fVec13[32768];
	int iConst36;
	float fVec14[8192];
	int iConst37;
	float fRec26[2];
	float fConst39;
	float fRec31[2];
	float fVec15[32768];
	int iConst41;
	float fVec16[4096];
	int iConst42;
	float fRec29[2];
	float fRec0[2];
	float fRec1[2];
	float fRec2[2];
	float fRec3[2];
	float fRec4[2];
	float fRec5[2];
	float fRec6[2];
	float fRec7[2];
	float fConst43;
	float fConst44;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fVslider2;
	float fConst45;
	FAUSTFLOAT fVslider3;
	float fRec34[2];
	float fRec33[2];
	float fRec32[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.1");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_reverb_duck.dsp");
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
		m->declare("id", "fx.reverb_duck");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "fx_reverb_duck");
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
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	virtual int getInputRate(int channel) {
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
		float fConst1 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst2 = ((0.0f - (6.90775537f * fConst1)) / fConst0);
		fConst3 = (6.28318548f / fConst0);
		float fConst4 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst5 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst1 - fConst4))));
		iConst6 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		iConst7 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst4 + -1.0f))));
		float fConst8 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst9 = ((0.0f - (6.90775537f * fConst8)) / fConst0);
		float fConst10 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst11 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst8 - fConst10))));
		iConst12 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst10 + -1.0f))));
		float fConst13 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst14 = ((0.0f - (6.90775537f * fConst13)) / fConst0);
		float fConst15 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst16 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst13 - fConst15))));
		iConst17 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst15 + -1.0f))));
		float fConst18 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst19 = ((0.0f - (6.90775537f * fConst18)) / fConst0);
		float fConst20 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst21 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst18 - fConst20))));
		iConst22 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst20 + -1.0f))));
		float fConst23 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst24 = ((0.0f - (6.90775537f * fConst23)) / fConst0);
		float fConst25 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst26 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst23 - fConst25))));
		iConst27 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst25 + -1.0f))));
		float fConst28 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst29 = ((0.0f - (6.90775537f * fConst28)) / fConst0);
		float fConst30 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst31 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst28 - fConst30))));
		iConst32 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst30 + -1.0f))));
		float fConst33 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst34 = ((0.0f - (6.90775537f * fConst33)) / fConst0);
		float fConst35 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst36 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst33 - fConst35))));
		iConst37 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst35 + -1.0f))));
		float fConst38 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst39 = ((0.0f - (6.90775537f * fConst38)) / fConst0);
		float fConst40 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst41 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst38 - fConst40))));
		iConst42 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst40 + -1.0f))));
		fConst43 = std::exp((0.0f - (10.0f / fConst0)));
		fConst44 = (1.0f - fConst43);
		fConst45 = (1.0f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(4.0f);
		fVslider1 = FAUSTFLOAT(6000.0f);
		fHslider0 = FAUSTFLOAT(0.5f);
		fVslider2 = FAUSTFLOAT(100.0f);
		fVslider3 = FAUSTFLOAT(100.0f);
	}
	
	virtual void instanceClear() {
		IOTA = 0;
		for (int l0 = 0; (l0 < 4096); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec10[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 65536); l2 = (l2 + 1)) {
			fVec1[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 4096); l3 = (l3 + 1)) {
			fVec2[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec8[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec13[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 65536); l6 = (l6 + 1)) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 8192); l7 = (l7 + 1)) {
			fVec4[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec11[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec16[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 32768); l10 = (l10 + 1)) {
			fVec5[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 8192); l11 = (l11 + 1)) {
			fVec6[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec14[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec19[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 65536); l14 = (l14 + 1)) {
			fVec7[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 8192); l15 = (l15 + 1)) {
			fVec8[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec17[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec22[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 32768); l18 = (l18 + 1)) {
			fVec9[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 4096); l19 = (l19 + 1)) {
			fVec10[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec20[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec25[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 32768); l22 = (l22 + 1)) {
			fVec11[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 8192); l23 = (l23 + 1)) {
			fVec12[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec23[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec28[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 32768); l26 = (l26 + 1)) {
			fVec13[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 8192); l27 = (l27 + 1)) {
			fVec14[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec26[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec31[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 32768); l30 = (l30 + 1)) {
			fVec15[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 4096); l31 = (l31 + 1)) {
			fVec16[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec29[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec0[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec1[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec2[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec3[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec4[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec5[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec6[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec7[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec34[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec33[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec32[l43] = 0.0f;
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
	
	virtual fx_reverb_duck* clone() {
		return new fx_reverb_duck();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx_reverb_duck");
		ui_interface->declare(&fVslider2, "unit", "ms");
		ui_interface->addVerticalSlider("attack", &fVslider2, 100.0f, 20.0f, 500.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "scale", "log");
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "unit", "sec");
		ui_interface->addVerticalSlider("decay", &fVslider0, 4.0f, 1.0f, 6.0f, 0.100000001f);
		ui_interface->declare(&fVslider1, "scale", "log");
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "Hz");
		ui_interface->addVerticalSlider("dump_hf", &fVslider1, 6000.0f, 1500.0f, 47040.0f, 1.0f);
		ui_interface->declare(&fHslider0, "unit", "db");
		ui_interface->addHorizontalSlider("level", &fHslider0, 0.5f, 0.0f, 56.0f, 0.0500000007f);
		ui_interface->declare(&fVslider3, "unit", "ms");
		ui_interface->addVerticalSlider("release", &fVslider3, 100.0f, 20.0f, 2000.0f, 0.100000001f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fVslider0);
		float fSlow1 = std::exp((fConst2 / fSlow0));
		float fSlow2 = fx_reverb_duck_faustpower2_f(fSlow1);
		float fSlow3 = std::cos((fConst3 * float(fVslider1)));
		float fSlow4 = (1.0f - (fSlow2 * fSlow3));
		float fSlow5 = (1.0f - fSlow2);
		float fSlow6 = (fSlow4 / fSlow5);
		float fSlow7 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow4) / fx_reverb_duck_faustpower2_f(fSlow5)) + -1.0f)));
		float fSlow8 = (fSlow6 - fSlow7);
		float fSlow9 = (fSlow1 * (fSlow7 + (1.0f - fSlow6)));
		float fSlow10 = std::exp((fConst9 / fSlow0));
		float fSlow11 = fx_reverb_duck_faustpower2_f(fSlow10);
		float fSlow12 = (1.0f - (fSlow11 * fSlow3));
		float fSlow13 = (1.0f - fSlow11);
		float fSlow14 = (fSlow12 / fSlow13);
		float fSlow15 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow12) / fx_reverb_duck_faustpower2_f(fSlow13)) + -1.0f)));
		float fSlow16 = (fSlow14 - fSlow15);
		float fSlow17 = (fSlow10 * (fSlow15 + (1.0f - fSlow14)));
		float fSlow18 = std::exp((fConst14 / fSlow0));
		float fSlow19 = fx_reverb_duck_faustpower2_f(fSlow18);
		float fSlow20 = (1.0f - (fSlow19 * fSlow3));
		float fSlow21 = (1.0f - fSlow19);
		float fSlow22 = (fSlow20 / fSlow21);
		float fSlow23 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow20) / fx_reverb_duck_faustpower2_f(fSlow21)) + -1.0f)));
		float fSlow24 = (fSlow22 - fSlow23);
		float fSlow25 = (fSlow18 * (fSlow23 + (1.0f - fSlow22)));
		float fSlow26 = std::exp((fConst19 / fSlow0));
		float fSlow27 = fx_reverb_duck_faustpower2_f(fSlow26);
		float fSlow28 = (1.0f - (fSlow27 * fSlow3));
		float fSlow29 = (1.0f - fSlow27);
		float fSlow30 = (fSlow28 / fSlow29);
		float fSlow31 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow28) / fx_reverb_duck_faustpower2_f(fSlow29)) + -1.0f)));
		float fSlow32 = (fSlow30 - fSlow31);
		float fSlow33 = (fSlow26 * (fSlow31 + (1.0f - fSlow30)));
		float fSlow34 = std::exp((fConst24 / fSlow0));
		float fSlow35 = fx_reverb_duck_faustpower2_f(fSlow34);
		float fSlow36 = (1.0f - (fSlow35 * fSlow3));
		float fSlow37 = (1.0f - fSlow35);
		float fSlow38 = (fSlow36 / fSlow37);
		float fSlow39 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow36) / fx_reverb_duck_faustpower2_f(fSlow37)) + -1.0f)));
		float fSlow40 = (fSlow38 - fSlow39);
		float fSlow41 = (fSlow34 * (fSlow39 + (1.0f - fSlow38)));
		float fSlow42 = std::exp((fConst29 / fSlow0));
		float fSlow43 = fx_reverb_duck_faustpower2_f(fSlow42);
		float fSlow44 = (1.0f - (fSlow43 * fSlow3));
		float fSlow45 = (1.0f - fSlow43);
		float fSlow46 = (fSlow44 / fSlow45);
		float fSlow47 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow44) / fx_reverb_duck_faustpower2_f(fSlow45)) + -1.0f)));
		float fSlow48 = (fSlow46 - fSlow47);
		float fSlow49 = (fSlow42 * (fSlow47 + (1.0f - fSlow46)));
		float fSlow50 = std::exp((fConst34 / fSlow0));
		float fSlow51 = fx_reverb_duck_faustpower2_f(fSlow50);
		float fSlow52 = (1.0f - (fSlow51 * fSlow3));
		float fSlow53 = (1.0f - fSlow51);
		float fSlow54 = (fSlow52 / fSlow53);
		float fSlow55 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow52) / fx_reverb_duck_faustpower2_f(fSlow53)) + -1.0f)));
		float fSlow56 = (fSlow54 - fSlow55);
		float fSlow57 = (fSlow50 * (fSlow55 + (1.0f - fSlow54)));
		float fSlow58 = std::exp((fConst39 / fSlow0));
		float fSlow59 = fx_reverb_duck_faustpower2_f(fSlow58);
		float fSlow60 = (1.0f - (fSlow59 * fSlow3));
		float fSlow61 = (1.0f - fSlow59);
		float fSlow62 = (fSlow60 / fSlow61);
		float fSlow63 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow60) / fx_reverb_duck_faustpower2_f(fSlow61)) + -1.0f)));
		float fSlow64 = (fSlow62 - fSlow63);
		float fSlow65 = (fSlow58 * (fSlow63 + (1.0f - fSlow62)));
		float fSlow66 = std::pow(10.0f, (0.0500000007f * float(fHslider0)));
		float fSlow67 = (0.00100000005f * float(fVslider2));
		int iSlow68 = (std::fabs(fSlow67) < 1.1920929e-07f);
		float fSlow69 = (iSlow68 ? 0.0f : std::exp((0.0f - (fConst45 / (iSlow68 ? 1.0f : fSlow67)))));
		float fSlow70 = (0.00100000005f * float(fVslider3));
		int iSlow71 = (std::fabs(fSlow70) < 1.1920929e-07f);
		float fSlow72 = (iSlow71 ? 0.0f : std::exp((0.0f - (fConst45 / (iSlow71 ? 1.0f : fSlow70)))));
		float fSlow73 = (1.0f - fSlow72);
		float fSlow74 = (1.0f - fSlow69);
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			fVec0[(IOTA & 4095)] = fTemp0;
			fRec10[0] = ((fSlow8 * fRec10[1]) + (fSlow9 * fRec7[1]));
			fVec1[(IOTA & 65535)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			float fTemp1 = (0.300000012f * fVec0[((IOTA - iConst6) & 4095)]);
			float fTemp2 = (((0.600000024f * fRec8[1]) + fVec1[((IOTA - iConst5) & 65535)]) - fTemp1);
			fVec2[(IOTA & 4095)] = fTemp2;
			fRec8[0] = fVec2[((IOTA - iConst7) & 4095)];
			float fRec9 = (0.0f - (0.600000024f * fTemp2));
			fRec13[0] = ((fSlow16 * fRec13[1]) + (fSlow17 * fRec3[1]));
			fVec3[(IOTA & 65535)] = ((0.353553385f * fRec13[0]) + 9.99999968e-21f);
			float fTemp3 = (((0.600000024f * fRec11[1]) + fVec3[((IOTA - iConst11) & 65535)]) - fTemp1);
			fVec4[(IOTA & 8191)] = fTemp3;
			fRec11[0] = fVec4[((IOTA - iConst12) & 8191)];
			float fRec12 = (0.0f - (0.600000024f * fTemp3));
			fRec16[0] = ((fSlow24 * fRec16[1]) + (fSlow25 * fRec5[1]));
			fVec5[(IOTA & 32767)] = ((0.353553385f * fRec16[0]) + 9.99999968e-21f);
			float fTemp4 = (fVec5[((IOTA - iConst16) & 32767)] + (fTemp1 + (0.600000024f * fRec14[1])));
			fVec6[(IOTA & 8191)] = fTemp4;
			fRec14[0] = fVec6[((IOTA - iConst17) & 8191)];
			float fRec15 = (0.0f - (0.600000024f * fTemp4));
			fRec19[0] = ((fSlow32 * fRec19[1]) + (fSlow33 * fRec1[1]));
			fVec7[(IOTA & 65535)] = ((0.353553385f * fRec19[0]) + 9.99999968e-21f);
			float fTemp5 = (fVec7[((IOTA - iConst21) & 65535)] + (fTemp1 + (0.600000024f * fRec17[1])));
			fVec8[(IOTA & 8191)] = fTemp5;
			fRec17[0] = fVec8[((IOTA - iConst22) & 8191)];
			float fRec18 = (0.0f - (0.600000024f * fTemp5));
			fRec22[0] = ((fSlow40 * fRec22[1]) + (fSlow41 * fRec6[1]));
			fVec9[(IOTA & 32767)] = ((0.353553385f * fRec22[0]) + 9.99999968e-21f);
			float fTemp6 = (fVec9[((IOTA - iConst26) & 32767)] - (fTemp1 + (0.600000024f * fRec20[1])));
			fVec10[(IOTA & 4095)] = fTemp6;
			fRec20[0] = fVec10[((IOTA - iConst27) & 4095)];
			float fRec21 = (0.600000024f * fTemp6);
			fRec25[0] = ((fSlow48 * fRec25[1]) + (fSlow49 * fRec2[1]));
			fVec11[(IOTA & 32767)] = ((0.353553385f * fRec25[0]) + 9.99999968e-21f);
			float fTemp7 = (fVec11[((IOTA - iConst31) & 32767)] - (fTemp1 + (0.600000024f * fRec23[1])));
			fVec12[(IOTA & 8191)] = fTemp7;
			fRec23[0] = fVec12[((IOTA - iConst32) & 8191)];
			float fRec24 = (0.600000024f * fTemp7);
			fRec28[0] = ((fSlow56 * fRec28[1]) + (fSlow57 * fRec4[1]));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec28[0]) + 9.99999968e-21f);
			float fTemp8 = ((fTemp1 + fVec13[((IOTA - iConst36) & 32767)]) - (0.600000024f * fRec26[1]));
			fVec14[(IOTA & 8191)] = fTemp8;
			fRec26[0] = fVec14[((IOTA - iConst37) & 8191)];
			float fRec27 = (0.600000024f * fTemp8);
			fRec31[0] = ((fSlow64 * fRec31[1]) + (fSlow65 * fRec0[1]));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec31[0]) + 9.99999968e-21f);
			float fTemp9 = ((fVec15[((IOTA - iConst41) & 32767)] + fTemp1) - (0.600000024f * fRec29[1]));
			fVec16[(IOTA & 4095)] = fTemp9;
			fRec29[0] = fVec16[((IOTA - iConst42) & 4095)];
			float fRec30 = (0.600000024f * fTemp9);
			float fTemp10 = (fRec30 + fRec27);
			float fTemp11 = (fRec21 + (fRec24 + fTemp10));
			fRec0[0] = (fRec8[1] + (fRec11[1] + (fRec14[1] + (fRec17[1] + (fRec20[1] + (fRec23[1] + (fRec26[1] + (fRec29[1] + (fRec9 + (fRec12 + (fRec15 + (fRec18 + fTemp11))))))))))));
			fRec1[0] = ((fRec20[1] + (fRec23[1] + (fRec26[1] + (fRec29[1] + fTemp11)))) - (fRec8[1] + (fRec11[1] + (fRec14[1] + (fRec17[1] + (fRec9 + (fRec12 + (fRec18 + fRec15))))))));
			float fTemp12 = (fRec24 + fRec21);
			fRec2[0] = ((fRec14[1] + (fRec17[1] + (fRec26[1] + (fRec29[1] + (fRec15 + (fRec18 + fTemp10)))))) - (fRec8[1] + (fRec11[1] + (fRec20[1] + (fRec23[1] + (fRec9 + (fRec12 + fTemp12)))))));
			fRec3[0] = ((fRec8[1] + (fRec11[1] + (fRec26[1] + (fRec29[1] + (fRec9 + (fRec12 + fTemp10)))))) - (fRec14[1] + (fRec17[1] + (fRec20[1] + (fRec23[1] + (fRec15 + (fRec18 + fTemp12)))))));
			float fTemp13 = (fRec30 + fRec24);
			float fTemp14 = (fRec27 + fRec21);
			fRec4[0] = ((fRec11[1] + (fRec17[1] + (fRec23[1] + (fRec29[1] + (fRec12 + (fRec18 + fTemp13)))))) - (fRec8[1] + (fRec14[1] + (fRec20[1] + (fRec26[1] + (fRec9 + (fRec15 + fTemp14)))))));
			fRec5[0] = ((fRec8[1] + (fRec14[1] + (fRec23[1] + (fRec29[1] + (fRec9 + (fRec15 + fTemp13)))))) - (fRec11[1] + (fRec17[1] + (fRec20[1] + (fRec26[1] + (fRec12 + (fRec18 + fTemp14)))))));
			float fTemp15 = (fRec30 + fRec21);
			float fTemp16 = (fRec27 + fRec24);
			fRec6[0] = ((fRec8[1] + (fRec17[1] + (fRec20[1] + (fRec29[1] + (fRec9 + (fRec18 + fTemp15)))))) - (fRec11[1] + (fRec14[1] + (fRec23[1] + (fRec26[1] + (fRec12 + (fRec15 + fTemp16)))))));
			fRec7[0] = ((fRec11[1] + (fRec14[1] + (fRec20[1] + (fRec29[1] + (fRec12 + (fRec15 + fTemp15)))))) - (fRec8[1] + (fRec17[1] + (fRec23[1] + (fRec26[1] + (fRec9 + (fRec18 + fTemp16)))))));
			float fTemp17 = std::fabs(fTemp0);
			fRec34[0] = std::max<float>(fTemp17, ((fRec34[1] * fSlow72) + (fTemp17 * fSlow73)));
			fRec33[0] = ((fRec33[1] * fSlow69) + (fRec34[0] * fSlow74));
			fRec32[0] = ((fConst43 * fRec32[1]) + (fConst44 * float((1 - ((fSlow66 * fRec33[0]) > 1.0f)))));
			output0[i] = FAUSTFLOAT((fTemp0 + (0.370000005f * ((fRec1[0] + fRec2[0]) * fRec32[0]))));
			IOTA = (IOTA + 1);
			fRec10[1] = fRec10[0];
			fRec8[1] = fRec8[0];
			fRec13[1] = fRec13[0];
			fRec11[1] = fRec11[0];
			fRec16[1] = fRec16[0];
			fRec14[1] = fRec14[0];
			fRec19[1] = fRec19[0];
			fRec17[1] = fRec17[0];
			fRec22[1] = fRec22[0];
			fRec20[1] = fRec20[0];
			fRec25[1] = fRec25[0];
			fRec23[1] = fRec23[0];
			fRec28[1] = fRec28[0];
			fRec26[1] = fRec26[0];
			fRec31[1] = fRec31[0];
			fRec29[1] = fRec29[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_reverb_duck_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_reverb_duck_UI<T>::name(sym(fx_reverb_duck));

typedef _fx_reverb_duck_UI<fx_reverb_duck> fx_reverb_duck_UI;

class faust_fx_reverb_duck_tilde : public FaustExternal<fx_reverb_duck, fx_reverb_duck_UI> {
public:
    faust_fx_reverb_duck_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
