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
	float fVec0[16384];
	int fSampleRate;
	float fConst2;
	FAUSTFLOAT fVslider0;
	float fConst3;
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fVslider2;
	float fConst4;
	FAUSTFLOAT fVslider3;
	float fRec11[2];
	float fRec10[2];
	float fVec1[65536];
	int iConst6;
	float fConst7;
	FAUSTFLOAT fVslider4;
	float fRec12[2];
	float fVec2[4096];
	int iConst8;
	float fRec8[2];
	float fConst10;
	float fRec16[2];
	float fRec15[2];
	float fVec3[32768];
	int iConst12;
	float fVec4[8192];
	int iConst13;
	float fRec13[2];
	float fConst15;
	float fRec20[2];
	float fRec19[2];
	float fVec5[65536];
	int iConst17;
	float fVec6[8192];
	int iConst18;
	float fRec17[2];
	float fConst20;
	float fRec24[2];
	float fRec23[2];
	float fVec7[32768];
	int iConst22;
	float fVec8[4096];
	int iConst23;
	float fRec21[2];
	float fConst25;
	float fRec28[2];
	float fRec27[2];
	float fVec9[65536];
	int iConst27;
	float fVec10[8192];
	int iConst28;
	float fRec25[2];
	float fConst30;
	float fRec32[2];
	float fRec31[2];
	float fVec11[32768];
	int iConst32;
	float fVec12[8192];
	int iConst33;
	float fRec29[2];
	float fConst35;
	float fRec36[2];
	float fRec35[2];
	float fVec13[32768];
	int iConst37;
	float fVec14[4096];
	int iConst38;
	float fRec33[2];
	float fConst40;
	float fRec40[2];
	float fRec39[2];
	float fVec15[32768];
	int iConst42;
	float fVec16[8192];
	int iConst43;
	float fRec37[2];
	float fRec0[3];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fConst44;
	float fConst45;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fVslider5;
	float fConst46;
	FAUSTFLOAT fVslider6;
	float fRec43[2];
	float fRec42[2];
	float fRec41[2];
	
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
		fConst4 = (3.14159274f / fConst0);
		float fConst5 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst6 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst1 - fConst5))));
		fConst7 = (0.00100000005f * fConst0);
		iConst8 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst5 + -1.0f))));
		float fConst9 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst10 = ((0.0f - (6.90775537f * fConst9)) / fConst0);
		float fConst11 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst12 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst9 - fConst11))));
		iConst13 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst11 + -1.0f))));
		float fConst14 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst15 = ((0.0f - (6.90775537f * fConst14)) / fConst0);
		float fConst16 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst17 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst14 - fConst16))));
		iConst18 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst16 + -1.0f))));
		float fConst19 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst20 = ((0.0f - (6.90775537f * fConst19)) / fConst0);
		float fConst21 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst22 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst19 - fConst21))));
		iConst23 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst21 + -1.0f))));
		float fConst24 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst25 = ((0.0f - (6.90775537f * fConst24)) / fConst0);
		float fConst26 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst27 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst24 - fConst26))));
		iConst28 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst26 + -1.0f))));
		float fConst29 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst30 = ((0.0f - (6.90775537f * fConst29)) / fConst0);
		float fConst31 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst32 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst29 - fConst31))));
		iConst33 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst31 + -1.0f))));
		float fConst34 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst35 = ((0.0f - (6.90775537f * fConst34)) / fConst0);
		float fConst36 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst37 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst34 - fConst36))));
		iConst38 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst36 + -1.0f))));
		float fConst39 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst40 = ((0.0f - (6.90775537f * fConst39)) / fConst0);
		float fConst41 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst42 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst39 - fConst41))));
		iConst43 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst41 + -1.0f))));
		fConst44 = std::exp((0.0f - (10.0f / fConst0)));
		fConst45 = (1.0f - fConst44);
		fConst46 = (1.0f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(4.0f);
		fVslider1 = FAUSTFLOAT(6000.0f);
		fVslider2 = FAUSTFLOAT(4.0f);
		fVslider3 = FAUSTFLOAT(200.0f);
		fVslider4 = FAUSTFLOAT(60.0f);
		fHslider0 = FAUSTFLOAT(0.5f);
		fVslider5 = FAUSTFLOAT(100.0f);
		fVslider6 = FAUSTFLOAT(100.0f);
	}
	
	virtual void instanceClear() {
		IOTA = 0;
		for (int l0 = 0; (l0 < 16384); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec11[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec10[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 65536); l3 = (l3 + 1)) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec12[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 4096); l5 = (l5 + 1)) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec8[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec16[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec15[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 32768); l9 = (l9 + 1)) {
			fVec3[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 8192); l10 = (l10 + 1)) {
			fVec4[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec13[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec20[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec19[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 65536); l14 = (l14 + 1)) {
			fVec5[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 8192); l15 = (l15 + 1)) {
			fVec6[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec17[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec24[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec23[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 32768); l19 = (l19 + 1)) {
			fVec7[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 4096); l20 = (l20 + 1)) {
			fVec8[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec21[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec28[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec27[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 65536); l24 = (l24 + 1)) {
			fVec9[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 8192); l25 = (l25 + 1)) {
			fVec10[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec25[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec32[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec31[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 32768); l29 = (l29 + 1)) {
			fVec11[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 8192); l30 = (l30 + 1)) {
			fVec12[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec29[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec36[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec35[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 32768); l34 = (l34 + 1)) {
			fVec13[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 4096); l35 = (l35 + 1)) {
			fVec14[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec33[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec40[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec39[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 32768); l39 = (l39 + 1)) {
			fVec15[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 8192); l40 = (l40 + 1)) {
			fVec16[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec37[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 3); l42 = (l42 + 1)) {
			fRec0[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec1[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec2[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec3[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec4[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec5[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec6[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec7[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec43[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec42[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec41[l52] = 0.0f;
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
		ui_interface->declare(&fVslider5, "unit", "ms");
		ui_interface->addVerticalSlider("attack", &fVslider5, 100.0f, 20.0f, 500.0f, 0.100000001f);
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low", &fVslider2, 4.0f, 1.0f, 6.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "scale", "log");
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid", &fVslider0, 4.0f, 1.0f, 6.0f, 0.100000001f);
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "unit", "ms");
		ui_interface->addVerticalSlider("delay", &fVslider4, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider1, "scale", "log");
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "Hz");
		ui_interface->addVerticalSlider("dump_hf", &fVslider1, 6000.0f, 1500.0f, 47040.0f, 1.0f);
		ui_interface->declare(&fVslider3, "scale", "log");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "unit", "Hz");
		ui_interface->addVerticalSlider("freq_low", &fVslider3, 200.0f, 50.0f, 1000.0f, 1.0f);
		ui_interface->addHorizontalSlider("level", &fHslider0, 0.5f, 0.0f, 56.0f, 0.0500000007f);
		ui_interface->declare(&fVslider6, "unit", "ms");
		ui_interface->addVerticalSlider("release", &fVslider6, 100.0f, 20.0f, 2000.0f, 0.100000001f);
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
		float fSlow10 = float(fVslider2);
		float fSlow11 = ((std::exp((fConst2 / fSlow10)) / fSlow1) + -1.0f);
		float fSlow12 = (1.0f / std::tan((fConst4 * float(fVslider3))));
		float fSlow13 = (1.0f / (fSlow12 + 1.0f));
		float fSlow14 = (1.0f - fSlow12);
		float fSlow15 = (0.00100000005f * float(fVslider4));
		float fSlow16 = std::exp((fConst10 / fSlow0));
		float fSlow17 = fx_reverb_duck_faustpower2_f(fSlow16);
		float fSlow18 = (1.0f - (fSlow17 * fSlow3));
		float fSlow19 = (1.0f - fSlow17);
		float fSlow20 = (fSlow18 / fSlow19);
		float fSlow21 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow18) / fx_reverb_duck_faustpower2_f(fSlow19)) + -1.0f)));
		float fSlow22 = (fSlow20 - fSlow21);
		float fSlow23 = (fSlow16 * (fSlow21 + (1.0f - fSlow20)));
		float fSlow24 = ((std::exp((fConst10 / fSlow10)) / fSlow16) + -1.0f);
		float fSlow25 = std::exp((fConst15 / fSlow0));
		float fSlow26 = fx_reverb_duck_faustpower2_f(fSlow25);
		float fSlow27 = (1.0f - (fSlow26 * fSlow3));
		float fSlow28 = (1.0f - fSlow26);
		float fSlow29 = (fSlow27 / fSlow28);
		float fSlow30 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow27) / fx_reverb_duck_faustpower2_f(fSlow28)) + -1.0f)));
		float fSlow31 = (fSlow29 - fSlow30);
		float fSlow32 = (fSlow25 * (fSlow30 + (1.0f - fSlow29)));
		float fSlow33 = ((std::exp((fConst15 / fSlow10)) / fSlow25) + -1.0f);
		float fSlow34 = std::exp((fConst20 / fSlow0));
		float fSlow35 = fx_reverb_duck_faustpower2_f(fSlow34);
		float fSlow36 = (1.0f - (fSlow35 * fSlow3));
		float fSlow37 = (1.0f - fSlow35);
		float fSlow38 = (fSlow36 / fSlow37);
		float fSlow39 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow36) / fx_reverb_duck_faustpower2_f(fSlow37)) + -1.0f)));
		float fSlow40 = (fSlow38 - fSlow39);
		float fSlow41 = (fSlow34 * (fSlow39 + (1.0f - fSlow38)));
		float fSlow42 = ((std::exp((fConst20 / fSlow10)) / fSlow34) + -1.0f);
		float fSlow43 = std::exp((fConst25 / fSlow0));
		float fSlow44 = fx_reverb_duck_faustpower2_f(fSlow43);
		float fSlow45 = (1.0f - (fSlow44 * fSlow3));
		float fSlow46 = (1.0f - fSlow44);
		float fSlow47 = (fSlow45 / fSlow46);
		float fSlow48 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow45) / fx_reverb_duck_faustpower2_f(fSlow46)) + -1.0f)));
		float fSlow49 = (fSlow47 - fSlow48);
		float fSlow50 = (fSlow43 * (fSlow48 + (1.0f - fSlow47)));
		float fSlow51 = ((std::exp((fConst25 / fSlow10)) / fSlow43) + -1.0f);
		float fSlow52 = std::exp((fConst30 / fSlow0));
		float fSlow53 = fx_reverb_duck_faustpower2_f(fSlow52);
		float fSlow54 = (1.0f - (fSlow53 * fSlow3));
		float fSlow55 = (1.0f - fSlow53);
		float fSlow56 = (fSlow54 / fSlow55);
		float fSlow57 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow54) / fx_reverb_duck_faustpower2_f(fSlow55)) + -1.0f)));
		float fSlow58 = (fSlow56 - fSlow57);
		float fSlow59 = (fSlow52 * (fSlow57 + (1.0f - fSlow56)));
		float fSlow60 = ((std::exp((fConst30 / fSlow10)) / fSlow52) + -1.0f);
		float fSlow61 = std::exp((fConst35 / fSlow0));
		float fSlow62 = fx_reverb_duck_faustpower2_f(fSlow61);
		float fSlow63 = (1.0f - (fSlow62 * fSlow3));
		float fSlow64 = (1.0f - fSlow62);
		float fSlow65 = (fSlow63 / fSlow64);
		float fSlow66 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow63) / fx_reverb_duck_faustpower2_f(fSlow64)) + -1.0f)));
		float fSlow67 = (fSlow65 - fSlow66);
		float fSlow68 = (fSlow61 * (fSlow66 + (1.0f - fSlow65)));
		float fSlow69 = ((std::exp((fConst35 / fSlow10)) / fSlow61) + -1.0f);
		float fSlow70 = std::exp((fConst40 / fSlow0));
		float fSlow71 = fx_reverb_duck_faustpower2_f(fSlow70);
		float fSlow72 = (1.0f - (fSlow71 * fSlow3));
		float fSlow73 = (1.0f - fSlow71);
		float fSlow74 = (fSlow72 / fSlow73);
		float fSlow75 = std::sqrt(std::max<float>(0.0f, ((fx_reverb_duck_faustpower2_f(fSlow72) / fx_reverb_duck_faustpower2_f(fSlow73)) + -1.0f)));
		float fSlow76 = (fSlow74 - fSlow75);
		float fSlow77 = (fSlow70 * (fSlow75 + (1.0f - fSlow74)));
		float fSlow78 = ((std::exp((fConst40 / fSlow10)) / fSlow70) + -1.0f);
		float fSlow79 = std::pow(10.0f, (0.0500000007f * float(fHslider0)));
		float fSlow80 = (0.00100000005f * float(fVslider5));
		int iSlow81 = (std::fabs(fSlow80) < 1.1920929e-07f);
		float fSlow82 = (iSlow81 ? 0.0f : std::exp((0.0f - (fConst46 / (iSlow81 ? 1.0f : fSlow80)))));
		float fSlow83 = (0.00100000005f * float(fVslider6));
		int iSlow84 = (std::fabs(fSlow83) < 1.1920929e-07f);
		float fSlow85 = (iSlow84 ? 0.0f : std::exp((0.0f - (fConst46 / (iSlow84 ? 1.0f : fSlow83)))));
		float fSlow86 = (1.0f - fSlow85);
		float fSlow87 = (1.0f - fSlow82);
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			fVec0[(IOTA & 16383)] = fTemp0;
			fRec11[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec11[1]) - (fRec7[1] + fRec7[2]))));
			fRec10[0] = ((fSlow8 * fRec10[1]) + (fSlow9 * (fRec7[1] + (fSlow11 * fRec11[0]))));
			fVec1[(IOTA & 65535)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			fRec12[0] = (fSlow15 + (0.999000013f * fRec12[1]));
			float fTemp1 = (0.300000012f * fVec0[((IOTA - int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst7 * fRec12[0]))))) & 16383)]);
			float fTemp2 = (((0.600000024f * fRec8[1]) + fVec1[((IOTA - iConst6) & 65535)]) - fTemp1);
			fVec2[(IOTA & 4095)] = fTemp2;
			fRec8[0] = fVec2[((IOTA - iConst8) & 4095)];
			float fRec9 = (0.0f - (0.600000024f * fTemp2));
			fRec16[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec16[1]) - (fRec5[1] + fRec5[2]))));
			fRec15[0] = ((fSlow22 * fRec15[1]) + (fSlow23 * (fRec5[1] + (fSlow24 * fRec16[0]))));
			fVec3[(IOTA & 32767)] = ((0.353553385f * fRec15[0]) + 9.99999968e-21f);
			float fTemp3 = ((0.600000024f * fRec13[1]) + (fTemp1 + fVec3[((IOTA - iConst12) & 32767)]));
			fVec4[(IOTA & 8191)] = fTemp3;
			fRec13[0] = fVec4[((IOTA - iConst13) & 8191)];
			float fRec14 = (0.0f - (0.600000024f * fTemp3));
			fRec20[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec20[1]) - (fRec1[1] + fRec1[2]))));
			fRec19[0] = ((fSlow31 * fRec19[1]) + (fSlow32 * (fRec1[1] + (fSlow33 * fRec20[0]))));
			fVec5[(IOTA & 65535)] = ((0.353553385f * fRec19[0]) + 9.99999968e-21f);
			float fTemp4 = (fVec5[((IOTA - iConst17) & 65535)] + (fTemp1 + (0.600000024f * fRec17[1])));
			fVec6[(IOTA & 8191)] = fTemp4;
			fRec17[0] = fVec6[((IOTA - iConst18) & 8191)];
			float fRec18 = (0.0f - (0.600000024f * fTemp4));
			fRec24[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec24[1]) - (fRec6[1] + fRec6[2]))));
			fRec23[0] = ((fSlow40 * fRec23[1]) + (fSlow41 * (fRec6[1] + (fSlow42 * fRec24[0]))));
			fVec7[(IOTA & 32767)] = ((0.353553385f * fRec23[0]) + 9.99999968e-21f);
			float fTemp5 = (fVec7[((IOTA - iConst22) & 32767)] - (fTemp1 + (0.600000024f * fRec21[1])));
			fVec8[(IOTA & 4095)] = fTemp5;
			fRec21[0] = fVec8[((IOTA - iConst23) & 4095)];
			float fRec22 = (0.600000024f * fTemp5);
			fRec28[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec28[1]) - (fRec3[1] + fRec3[2]))));
			fRec27[0] = ((fSlow49 * fRec27[1]) + (fSlow50 * (fRec3[1] + (fSlow51 * fRec28[0]))));
			fVec9[(IOTA & 65535)] = ((0.353553385f * fRec27[0]) + 9.99999968e-21f);
			float fTemp6 = (((0.600000024f * fRec25[1]) + fVec9[((IOTA - iConst27) & 65535)]) - fTemp1);
			fVec10[(IOTA & 8191)] = fTemp6;
			fRec25[0] = fVec10[((IOTA - iConst28) & 8191)];
			float fRec26 = (0.0f - (0.600000024f * fTemp6));
			fRec32[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec32[1]) - (fRec4[1] + fRec4[2]))));
			fRec31[0] = ((fSlow58 * fRec31[1]) + (fSlow59 * (fRec4[1] + (fSlow60 * fRec32[0]))));
			fVec11[(IOTA & 32767)] = ((0.353553385f * fRec31[0]) + 9.99999968e-21f);
			float fTemp7 = ((fTemp1 + fVec11[((IOTA - iConst32) & 32767)]) - (0.600000024f * fRec29[1]));
			fVec12[(IOTA & 8191)] = fTemp7;
			fRec29[0] = fVec12[((IOTA - iConst33) & 8191)];
			float fRec30 = (0.600000024f * fTemp7);
			fRec36[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec36[1]) - (fRec0[1] + fRec0[2]))));
			fRec35[0] = ((fSlow67 * fRec35[1]) + (fSlow68 * (fRec0[1] + (fSlow69 * fRec36[0]))));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec35[0]) + 9.99999968e-21f);
			float fTemp8 = ((fVec13[((IOTA - iConst37) & 32767)] + fTemp1) - (0.600000024f * fRec33[1]));
			fVec14[(IOTA & 4095)] = fTemp8;
			fRec33[0] = fVec14[((IOTA - iConst38) & 4095)];
			float fRec34 = (0.600000024f * fTemp8);
			fRec40[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec40[1]) - (fRec2[1] + fRec2[2]))));
			fRec39[0] = ((fSlow76 * fRec39[1]) + (fSlow77 * (fRec2[1] + (fSlow78 * fRec40[0]))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec39[0]) + 9.99999968e-21f);
			float fTemp9 = (fVec15[((IOTA - iConst42) & 32767)] - (fTemp1 + (0.600000024f * fRec37[1])));
			fVec16[(IOTA & 8191)] = fTemp9;
			fRec37[0] = fVec16[((IOTA - iConst43) & 8191)];
			float fRec38 = (0.600000024f * fTemp9);
			float fTemp10 = (fRec34 + fRec9);
			float fTemp11 = (fRec30 + fTemp10);
			fRec0[0] = (fRec8[1] + (fRec13[1] + (fRec17[1] + (fRec21[1] + (fRec25[1] + (fRec29[1] + (fRec33[1] + (fRec37[1] + (fRec26 + (fRec14 + (fRec18 + (fRec22 + (fRec38 + fTemp11)))))))))))));
			float fTemp12 = (fRec34 + fRec30);
			fRec1[0] = ((fRec21[1] + (fRec29[1] + (fRec33[1] + (fRec37[1] + (fRec22 + (fRec38 + fTemp12)))))) - (fRec8[1] + (fRec13[1] + (fRec17[1] + (fRec25[1] + (fRec26 + (fRec14 + (fRec18 + fRec9))))))));
			fRec2[0] = ((fRec13[1] + (fRec17[1] + (fRec29[1] + (fRec33[1] + (fRec14 + (fRec18 + fTemp12)))))) - (fRec8[1] + (fRec21[1] + (fRec25[1] + (fRec37[1] + (fRec26 + (fRec22 + (fRec38 + fRec9))))))));
			fRec3[0] = ((fRec8[1] + (fRec25[1] + (fRec29[1] + (fRec33[1] + (fRec26 + fTemp11))))) - (fRec13[1] + (fRec17[1] + (fRec21[1] + (fRec37[1] + (fRec14 + (fRec18 + (fRec38 + fRec22))))))));
			float fTemp13 = (fRec30 + fRec9);
			fRec4[0] = ((fRec17[1] + (fRec25[1] + (fRec33[1] + (fRec37[1] + (fRec26 + (fRec18 + (fRec34 + fRec38))))))) - (fRec8[1] + (fRec13[1] + (fRec21[1] + (fRec29[1] + (fRec14 + (fRec22 + fTemp13)))))));
			fRec5[0] = ((fRec8[1] + (fRec13[1] + (fRec33[1] + (fRec37[1] + (fRec14 + (fRec38 + fTemp10)))))) - (fRec17[1] + (fRec21[1] + (fRec25[1] + (fRec29[1] + (fRec26 + (fRec18 + (fRec30 + fRec22))))))));
			fRec6[0] = ((fRec8[1] + (fRec17[1] + (fRec21[1] + (fRec33[1] + (fRec18 + (fRec22 + fTemp10)))))) - (fRec13[1] + (fRec25[1] + (fRec29[1] + (fRec37[1] + (fRec26 + (fRec14 + (fRec30 + fRec38))))))));
			fRec7[0] = ((fRec13[1] + (fRec21[1] + (fRec25[1] + (fRec33[1] + (fRec26 + (fRec14 + (fRec34 + fRec22))))))) - (fRec8[1] + (fRec17[1] + (fRec29[1] + (fRec37[1] + (fRec18 + (fRec38 + fTemp13)))))));
			float fTemp14 = std::fabs(fTemp0);
			fRec43[0] = std::max<float>(fTemp14, ((fRec43[1] * fSlow85) + (fTemp14 * fSlow86)));
			fRec42[0] = ((fRec42[1] * fSlow82) + (fRec43[0] * fSlow87));
			fRec41[0] = ((fConst44 * fRec41[1]) + (fConst45 * float((1 - ((fSlow79 * fRec42[0]) > 1.0f)))));
			output0[i] = FAUSTFLOAT((fTemp0 + (0.370000005f * ((fRec1[0] + fRec2[0]) * fRec41[0]))));
			IOTA = (IOTA + 1);
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec12[1] = fRec12[0];
			fRec8[1] = fRec8[0];
			fRec16[1] = fRec16[0];
			fRec15[1] = fRec15[0];
			fRec13[1] = fRec13[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec17[1] = fRec17[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec21[1] = fRec21[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec25[1] = fRec25[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec29[1] = fRec29[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec33[1] = fRec33[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec37[1] = fRec37[0];
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
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
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
