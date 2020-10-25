/* ------------------------------------------------------------
name: "fx.zita_rev1"
Code generated with Faust 2.28.6 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_zita_rev1_H__
#define  __fx_zita_rev1_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_zita_rev1_dsp.h **************************/
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

class fx_zita_rev1_dsp {

    public:

        fx_zita_rev1_dsp() {}
        virtual ~fx_zita_rev1_dsp() {}

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
        virtual fx_zita_rev1_dsp* clone() = 0;
    
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

class decorator_dsp : public fx_zita_rev1_dsp {

    protected:

        fx_zita_rev1_dsp* fDSP;

    public:

        decorator_dsp(fx_zita_rev1_dsp* fx_zita_rev1_dsp = nullptr):fDSP(fx_zita_rev1_dsp) {}
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
    
        virtual fx_zita_rev1_dsp* createDSPInstance() = 0;
    
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
/**************************  END  fx_zita_rev1_dsp.h **************************/
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
struct fx_zita_rev1 : public fx_zita_rev1_dsp {
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

static float fx_zita_rev1_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_zita_rev1
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_zita_rev1 : public fx_zita_rev1_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int IOTA;
	float fVec0[16384];
	FAUSTFLOAT fHslider0;
	float fRec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fVslider0;
	float fConst3;
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fVslider2;
	float fConst4;
	FAUSTFLOAT fVslider3;
	float fRec12[2];
	float fRec11[2];
	float fVec1[65536];
	float fConst5;
	int iConst6;
	float fVec2[16384];
	float fConst7;
	FAUSTFLOAT fVslider4;
	float fRec13[2];
	float fVec3[4096];
	int iConst8;
	float fRec9[2];
	float fConst9;
	float fConst10;
	float fRec17[2];
	float fRec16[2];
	float fVec4[65536];
	float fConst11;
	int iConst12;
	float fVec5[8192];
	int iConst13;
	float fRec14[2];
	float fConst14;
	float fConst15;
	float fRec21[2];
	float fRec20[2];
	float fVec6[32768];
	float fConst16;
	int iConst17;
	float fVec7[8192];
	int iConst18;
	float fRec18[2];
	float fConst19;
	float fConst20;
	float fRec25[2];
	float fRec24[2];
	float fVec8[65536];
	float fConst21;
	int iConst22;
	float fVec9[8192];
	int iConst23;
	float fRec22[2];
	float fConst24;
	float fConst25;
	float fRec29[2];
	float fRec28[2];
	float fVec10[32768];
	float fConst26;
	int iConst27;
	float fVec11[8192];
	int iConst28;
	float fRec26[2];
	float fConst29;
	float fConst30;
	float fRec33[2];
	float fRec32[2];
	float fVec12[32768];
	float fConst31;
	int iConst32;
	float fVec13[4096];
	int iConst33;
	float fRec30[2];
	float fConst34;
	float fConst35;
	float fRec37[2];
	float fRec36[2];
	float fVec14[32768];
	float fConst36;
	int iConst37;
	float fVec15[4096];
	int iConst38;
	float fRec34[2];
	float fConst39;
	float fConst40;
	float fRec41[2];
	float fRec40[2];
	float fVec16[32768];
	float fConst41;
	int iConst42;
	float fVec17[8192];
	int iConst43;
	float fRec38[2];
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
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_zita_rev1.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
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
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "fx.zita_rev1");
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
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst2 = ((0.0f - (6.90775537f * fConst1)) / fConst0);
		fConst3 = (6.28318548f / fConst0);
		fConst4 = (3.14159274f / fConst0);
		fConst5 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst6 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst1 - fConst5))));
		fConst7 = (0.00100000005f * fConst0);
		iConst8 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst5 + -1.0f))));
		fConst9 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst10 = ((0.0f - (6.90775537f * fConst9)) / fConst0);
		fConst11 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst12 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst9 - fConst11))));
		iConst13 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst11 + -1.0f))));
		fConst14 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst15 = ((0.0f - (6.90775537f * fConst14)) / fConst0);
		fConst16 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst17 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst14 - fConst16))));
		iConst18 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst16 + -1.0f))));
		fConst19 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst20 = ((0.0f - (6.90775537f * fConst19)) / fConst0);
		fConst21 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst22 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst19 - fConst21))));
		iConst23 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst21 + -1.0f))));
		fConst24 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst25 = ((0.0f - (6.90775537f * fConst24)) / fConst0);
		fConst26 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst27 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst24 - fConst26))));
		iConst28 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst26 + -1.0f))));
		fConst29 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst30 = ((0.0f - (6.90775537f * fConst29)) / fConst0);
		fConst31 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst32 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst29 - fConst31))));
		iConst33 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst31 + -1.0f))));
		fConst34 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst35 = ((0.0f - (6.90775537f * fConst34)) / fConst0);
		fConst36 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst37 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst34 - fConst36))));
		iConst38 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst36 + -1.0f))));
		fConst39 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst40 = ((0.0f - (6.90775537f * fConst39)) / fConst0);
		fConst41 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst42 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst39 - fConst41))));
		iConst43 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst41 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fVslider0 = FAUSTFLOAT(2.0f);
		fVslider1 = FAUSTFLOAT(6000.0f);
		fVslider2 = FAUSTFLOAT(3.0f);
		fVslider3 = FAUSTFLOAT(200.0f);
		fVslider4 = FAUSTFLOAT(60.0f);
	}
	
	virtual void instanceClear() {
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 16384); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec0[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec12[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec11[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 65536); l4 = (l4 + 1)) {
			fVec1[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 16384); l5 = (l5 + 1)) {
			fVec2[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec13[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 4096); l7 = (l7 + 1)) {
			fVec3[l7] = 0.0f;
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
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec16[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 65536); l11 = (l11 + 1)) {
			fVec4[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 8192); l12 = (l12 + 1)) {
			fVec5[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec14[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec21[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec20[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 32768); l16 = (l16 + 1)) {
			fVec6[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 8192); l17 = (l17 + 1)) {
			fVec7[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec18[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec25[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec24[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 65536); l21 = (l21 + 1)) {
			fVec8[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 8192); l22 = (l22 + 1)) {
			fVec9[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec22[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec29[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec28[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 32768); l26 = (l26 + 1)) {
			fVec10[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 8192); l27 = (l27 + 1)) {
			fVec11[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec26[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec33[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec32[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 32768); l31 = (l31 + 1)) {
			fVec12[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 4096); l32 = (l32 + 1)) {
			fVec13[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec30[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec37[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec36[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 32768); l36 = (l36 + 1)) {
			fVec14[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 4096); l37 = (l37 + 1)) {
			fVec15[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec34[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec41[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec40[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 32768); l41 = (l41 + 1)) {
			fVec16[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 8192); l42 = (l42 + 1)) {
			fVec17[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec38[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec1[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec2[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec3[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec4[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec5[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec6[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec7[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec8[l51] = 0.0f;
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
	
	virtual fx_zita_rev1* clone() {
		return new fx_zita_rev1();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.zita_rev1");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "s");
		ui_interface->addVerticalSlider("decay_low", &fVslider2, 3.0f, 1.0f, 25.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "scale", "log");
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "unit", "s");
		ui_interface->addVerticalSlider("decay_mid", &fVslider0, 2.0f, 1.0f, 25.0f, 0.100000001f);
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "unit", "ms");
		ui_interface->addVerticalSlider("delay", &fVslider4, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fVslider1, "scale", "log");
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "Hz");
		ui_interface->addVerticalSlider("dump_hf", &fVslider1, 6000.0f, 1500.0f, 47040.0f, 1.0f);
		ui_interface->declare(&fVslider3, "scale", "log");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "unit", "Hz");
		ui_interface->addVerticalSlider("freq_low", &fVslider3, 200.0f, 50.0f, 1000.0f, 1.0f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = (0.00100000005f * float(fHslider0));
		float fSlow2 = float(fVslider0);
		float fSlow3 = std::exp((fConst2 / fSlow2));
		float fSlow4 = fx_zita_rev1_faustpower2_f(fSlow3);
		float fSlow5 = std::cos((fConst3 * float(fVslider1)));
		float fSlow6 = (1.0f - (fSlow4 * fSlow5));
		float fSlow7 = (1.0f - fSlow4);
		float fSlow8 = (fSlow6 / fSlow7);
		float fSlow9 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow6) / fx_zita_rev1_faustpower2_f(fSlow7)) + -1.0f)));
		float fSlow10 = (fSlow8 - fSlow9);
		float fSlow11 = (fSlow3 * (fSlow9 + (1.0f - fSlow8)));
		float fSlow12 = float(fVslider2);
		float fSlow13 = ((std::exp((fConst2 / fSlow12)) / fSlow3) + -1.0f);
		float fSlow14 = (1.0f / std::tan((fConst4 * float(fVslider3))));
		float fSlow15 = (1.0f / (fSlow14 + 1.0f));
		float fSlow16 = (1.0f - fSlow14);
		float fSlow17 = (0.00100000005f * float(fVslider4));
		float fSlow18 = std::exp((fConst10 / fSlow2));
		float fSlow19 = fx_zita_rev1_faustpower2_f(fSlow18);
		float fSlow20 = (1.0f - (fSlow19 * fSlow5));
		float fSlow21 = (1.0f - fSlow19);
		float fSlow22 = (fSlow20 / fSlow21);
		float fSlow23 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow20) / fx_zita_rev1_faustpower2_f(fSlow21)) + -1.0f)));
		float fSlow24 = (fSlow22 - fSlow23);
		float fSlow25 = (fSlow18 * (fSlow23 + (1.0f - fSlow22)));
		float fSlow26 = ((std::exp((fConst10 / fSlow12)) / fSlow18) + -1.0f);
		float fSlow27 = std::exp((fConst15 / fSlow2));
		float fSlow28 = fx_zita_rev1_faustpower2_f(fSlow27);
		float fSlow29 = (1.0f - (fSlow28 * fSlow5));
		float fSlow30 = (1.0f - fSlow28);
		float fSlow31 = (fSlow29 / fSlow30);
		float fSlow32 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow29) / fx_zita_rev1_faustpower2_f(fSlow30)) + -1.0f)));
		float fSlow33 = (fSlow31 - fSlow32);
		float fSlow34 = (fSlow27 * (fSlow32 + (1.0f - fSlow31)));
		float fSlow35 = ((std::exp((fConst15 / fSlow12)) / fSlow27) + -1.0f);
		float fSlow36 = std::exp((fConst20 / fSlow2));
		float fSlow37 = fx_zita_rev1_faustpower2_f(fSlow36);
		float fSlow38 = (1.0f - (fSlow37 * fSlow5));
		float fSlow39 = (1.0f - fSlow37);
		float fSlow40 = (fSlow38 / fSlow39);
		float fSlow41 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow38) / fx_zita_rev1_faustpower2_f(fSlow39)) + -1.0f)));
		float fSlow42 = (fSlow40 - fSlow41);
		float fSlow43 = (fSlow36 * (fSlow41 + (1.0f - fSlow40)));
		float fSlow44 = ((std::exp((fConst20 / fSlow12)) / fSlow36) + -1.0f);
		float fSlow45 = std::exp((fConst25 / fSlow2));
		float fSlow46 = fx_zita_rev1_faustpower2_f(fSlow45);
		float fSlow47 = (1.0f - (fSlow46 * fSlow5));
		float fSlow48 = (1.0f - fSlow46);
		float fSlow49 = (fSlow47 / fSlow48);
		float fSlow50 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow47) / fx_zita_rev1_faustpower2_f(fSlow48)) + -1.0f)));
		float fSlow51 = (fSlow49 - fSlow50);
		float fSlow52 = (fSlow45 * (fSlow50 + (1.0f - fSlow49)));
		float fSlow53 = ((std::exp((fConst25 / fSlow12)) / fSlow45) + -1.0f);
		float fSlow54 = std::exp((fConst30 / fSlow2));
		float fSlow55 = fx_zita_rev1_faustpower2_f(fSlow54);
		float fSlow56 = (1.0f - (fSlow55 * fSlow5));
		float fSlow57 = (1.0f - fSlow55);
		float fSlow58 = (fSlow56 / fSlow57);
		float fSlow59 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow56) / fx_zita_rev1_faustpower2_f(fSlow57)) + -1.0f)));
		float fSlow60 = (fSlow58 - fSlow59);
		float fSlow61 = (fSlow54 * (fSlow59 + (1.0f - fSlow58)));
		float fSlow62 = ((std::exp((fConst30 / fSlow12)) / fSlow54) + -1.0f);
		float fSlow63 = std::exp((fConst35 / fSlow2));
		float fSlow64 = fx_zita_rev1_faustpower2_f(fSlow63);
		float fSlow65 = (1.0f - (fSlow64 * fSlow5));
		float fSlow66 = (1.0f - fSlow64);
		float fSlow67 = (fSlow65 / fSlow66);
		float fSlow68 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow65) / fx_zita_rev1_faustpower2_f(fSlow66)) + -1.0f)));
		float fSlow69 = (fSlow67 - fSlow68);
		float fSlow70 = (fSlow63 * (fSlow68 + (1.0f - fSlow67)));
		float fSlow71 = ((std::exp((fConst35 / fSlow12)) / fSlow63) + -1.0f);
		float fSlow72 = std::exp((fConst40 / fSlow2));
		float fSlow73 = fx_zita_rev1_faustpower2_f(fSlow72);
		float fSlow74 = (1.0f - (fSlow73 * fSlow5));
		float fSlow75 = (1.0f - fSlow73);
		float fSlow76 = (fSlow74 / fSlow75);
		float fSlow77 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow74) / fx_zita_rev1_faustpower2_f(fSlow75)) + -1.0f)));
		float fSlow78 = (fSlow76 - fSlow77);
		float fSlow79 = (fSlow72 * (fSlow77 + (1.0f - fSlow76)));
		float fSlow80 = ((std::exp((fConst40 / fSlow12)) / fSlow72) + -1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			float fTemp1 = (iSlow0 ? 0.0f : fTemp0);
			fVec0[(IOTA & 16383)] = fTemp1;
			fRec0[0] = (fSlow1 + (0.999000013f * fRec0[1]));
			float fTemp2 = (1.0f - fRec0[0]);
			fRec12[0] = (0.0f - (fSlow15 * ((fSlow16 * fRec12[1]) - (fRec8[1] + fRec8[2]))));
			fRec11[0] = ((fSlow10 * fRec11[1]) + (fSlow11 * (fRec8[1] + (fSlow13 * fRec12[0]))));
			fVec1[(IOTA & 65535)] = ((0.353553385f * fRec11[0]) + 9.99999968e-21f);
			float fTemp3 = float(input1[i]);
			float fTemp4 = (iSlow0 ? 0.0f : fTemp3);
			fVec2[(IOTA & 16383)] = fTemp4;
			fRec13[0] = (fSlow17 + (0.999000013f * fRec13[1]));
			int iTemp5 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst7 * fRec13[0]))));
			float fTemp6 = (0.300000012f * fVec2[((IOTA - iTemp5) & 16383)]);
			float fTemp7 = (((0.600000024f * fRec9[1]) + fVec1[((IOTA - iConst6) & 65535)]) - fTemp6);
			fVec3[(IOTA & 4095)] = fTemp7;
			fRec9[0] = fVec3[((IOTA - iConst8) & 4095)];
			float fRec10 = (0.0f - (0.600000024f * fTemp7));
			fRec17[0] = (0.0f - (fSlow15 * ((fSlow16 * fRec17[1]) - (fRec4[1] + fRec4[2]))));
			fRec16[0] = ((fSlow24 * fRec16[1]) + (fSlow25 * (fRec4[1] + (fSlow26 * fRec17[0]))));
			fVec4[(IOTA & 65535)] = ((0.353553385f * fRec16[0]) + 9.99999968e-21f);
			float fTemp8 = (((0.600000024f * fRec14[1]) + fVec4[((IOTA - iConst12) & 65535)]) - fTemp6);
			fVec5[(IOTA & 8191)] = fTemp8;
			fRec14[0] = fVec5[((IOTA - iConst13) & 8191)];
			float fRec15 = (0.0f - (0.600000024f * fTemp8));
			fRec21[0] = (0.0f - (fSlow15 * ((fSlow16 * fRec21[1]) - (fRec6[1] + fRec6[2]))));
			fRec20[0] = ((fSlow33 * fRec20[1]) + (fSlow34 * (fRec6[1] + (fSlow35 * fRec21[0]))));
			fVec6[(IOTA & 32767)] = ((0.353553385f * fRec20[0]) + 9.99999968e-21f);
			float fTemp9 = (fVec6[((IOTA - iConst17) & 32767)] + (fTemp6 + (0.600000024f * fRec18[1])));
			fVec7[(IOTA & 8191)] = fTemp9;
			fRec18[0] = fVec7[((IOTA - iConst18) & 8191)];
			float fRec19 = (0.0f - (0.600000024f * fTemp9));
			fRec25[0] = (0.0f - (fSlow15 * ((fSlow16 * fRec25[1]) - (fRec2[1] + fRec2[2]))));
			fRec24[0] = ((fSlow42 * fRec24[1]) + (fSlow43 * (fRec2[1] + (fSlow44 * fRec25[0]))));
			fVec8[(IOTA & 65535)] = ((0.353553385f * fRec24[0]) + 9.99999968e-21f);
			float fTemp10 = (fTemp6 + ((0.600000024f * fRec22[1]) + fVec8[((IOTA - iConst22) & 65535)]));
			fVec9[(IOTA & 8191)] = fTemp10;
			fRec22[0] = fVec9[((IOTA - iConst23) & 8191)];
			float fRec23 = (0.0f - (0.600000024f * fTemp10));
			float fTemp11 = (0.300000012f * fVec0[((IOTA - iTemp5) & 16383)]);
			fRec29[0] = (0.0f - (fSlow15 * ((fSlow16 * fRec29[1]) - (fRec5[1] + fRec5[2]))));
			fRec28[0] = ((fSlow51 * fRec28[1]) + (fSlow52 * (fRec5[1] + (fSlow53 * fRec29[0]))));
			fVec10[(IOTA & 32767)] = ((0.353553385f * fRec28[0]) + 9.99999968e-21f);
			float fTemp12 = ((fTemp11 + fVec10[((IOTA - iConst27) & 32767)]) - (0.600000024f * fRec26[1]));
			fVec11[(IOTA & 8191)] = fTemp12;
			fRec26[0] = fVec11[((IOTA - iConst28) & 8191)];
			float fRec27 = (0.600000024f * fTemp12);
			fRec33[0] = (0.0f - (fSlow15 * ((fSlow16 * fRec33[1]) - (fRec1[1] + fRec1[2]))));
			fRec32[0] = ((fSlow60 * fRec32[1]) + (fSlow61 * (fRec1[1] + (fSlow62 * fRec33[0]))));
			fVec12[(IOTA & 32767)] = ((0.353553385f * fRec32[0]) + 9.99999968e-21f);
			float fTemp13 = ((fVec12[((IOTA - iConst32) & 32767)] + fTemp11) - (0.600000024f * fRec30[1]));
			fVec13[(IOTA & 4095)] = fTemp13;
			fRec30[0] = fVec13[((IOTA - iConst33) & 4095)];
			float fRec31 = (0.600000024f * fTemp13);
			fRec37[0] = (0.0f - (fSlow15 * ((fSlow16 * fRec37[1]) - (fRec7[1] + fRec7[2]))));
			fRec36[0] = ((fSlow69 * fRec36[1]) + (fSlow70 * (fRec7[1] + (fSlow71 * fRec37[0]))));
			fVec14[(IOTA & 32767)] = ((0.353553385f * fRec36[0]) + 9.99999968e-21f);
			float fTemp14 = (fVec14[((IOTA - iConst37) & 32767)] - (fTemp11 + (0.600000024f * fRec34[1])));
			fVec15[(IOTA & 4095)] = fTemp14;
			fRec34[0] = fVec15[((IOTA - iConst38) & 4095)];
			float fRec35 = (0.600000024f * fTemp14);
			fRec41[0] = (0.0f - (fSlow15 * ((fSlow16 * fRec41[1]) - (fRec3[1] + fRec3[2]))));
			fRec40[0] = ((fSlow78 * fRec40[1]) + (fSlow79 * (fRec3[1] + (fSlow80 * fRec41[0]))));
			fVec16[(IOTA & 32767)] = ((0.353553385f * fRec40[0]) + 9.99999968e-21f);
			float fTemp15 = (fVec16[((IOTA - iConst42) & 32767)] - (fTemp11 + (0.600000024f * fRec38[1])));
			fVec17[(IOTA & 8191)] = fTemp15;
			fRec38[0] = fVec17[((IOTA - iConst43) & 8191)];
			float fRec39 = (0.600000024f * fTemp15);
			float fTemp16 = (fRec31 + fRec27);
			float fTemp17 = (fRec35 + (fRec39 + fTemp16));
			fRec1[0] = (fRec9[1] + (fRec14[1] + (fRec18[1] + (fRec22[1] + (fRec26[1] + (fRec30[1] + (fRec34[1] + (fRec38[1] + (fRec10 + (fRec15 + (fRec19 + (fRec23 + fTemp17))))))))))));
			fRec2[0] = ((fRec26[1] + (fRec30[1] + (fRec34[1] + (fRec38[1] + fTemp17)))) - (fRec9[1] + (fRec14[1] + (fRec18[1] + (fRec22[1] + (fRec10 + (fRec15 + (fRec23 + fRec19))))))));
			float fTemp18 = (fRec39 + fRec35);
			fRec3[0] = ((fRec18[1] + (fRec22[1] + (fRec26[1] + (fRec30[1] + (fRec19 + (fRec23 + fTemp16)))))) - (fRec9[1] + (fRec14[1] + (fRec34[1] + (fRec38[1] + (fRec10 + (fRec15 + fTemp18)))))));
			fRec4[0] = ((fRec9[1] + (fRec14[1] + (fRec26[1] + (fRec30[1] + (fRec10 + (fRec15 + fTemp16)))))) - (fRec18[1] + (fRec22[1] + (fRec34[1] + (fRec38[1] + (fRec19 + (fRec23 + fTemp18)))))));
			float fTemp19 = (fRec31 + fRec39);
			float fTemp20 = (fRec27 + fRec35);
			fRec5[0] = ((fRec14[1] + (fRec22[1] + (fRec30[1] + (fRec38[1] + (fRec15 + (fRec23 + fTemp19)))))) - (fRec9[1] + (fRec18[1] + (fRec26[1] + (fRec34[1] + (fRec10 + (fRec19 + fTemp20)))))));
			fRec6[0] = ((fRec9[1] + (fRec18[1] + (fRec30[1] + (fRec38[1] + (fRec10 + (fRec19 + fTemp19)))))) - (fRec14[1] + (fRec22[1] + (fRec26[1] + (fRec34[1] + (fRec15 + (fRec23 + fTemp20)))))));
			float fTemp21 = (fRec31 + fRec35);
			float fTemp22 = (fRec27 + fRec39);
			fRec7[0] = ((fRec9[1] + (fRec22[1] + (fRec30[1] + (fRec34[1] + (fRec10 + (fRec23 + fTemp21)))))) - (fRec14[1] + (fRec18[1] + (fRec26[1] + (fRec38[1] + (fRec15 + (fRec19 + fTemp22)))))));
			fRec8[0] = ((fRec14[1] + (fRec18[1] + (fRec30[1] + (fRec34[1] + (fRec15 + (fRec19 + fTemp21)))))) - (fRec9[1] + (fRec22[1] + (fRec26[1] + (fRec38[1] + (fRec10 + (fRec23 + fTemp22)))))));
			output0[i] = FAUSTFLOAT((iSlow0 ? fTemp0 : ((fTemp1 * fTemp2) + (0.370000005f * (fRec0[0] * (fRec2[0] + fRec3[0]))))));
			output1[i] = FAUSTFLOAT((iSlow0 ? fTemp3 : ((fTemp2 * fTemp4) + (0.370000005f * (fRec0[0] * (fRec2[0] - fRec3[0]))))));
			IOTA = (IOTA + 1);
			fRec0[1] = fRec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec13[1] = fRec13[0];
			fRec9[1] = fRec9[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec14[1] = fRec14[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec18[1] = fRec18[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec26[1] = fRec26[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec34[1] = fRec34[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec38[1] = fRec38[0];
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
    struct _fx_zita_rev1_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_zita_rev1_UI<T>::name(sym(fx_zita_rev1));

typedef _fx_zita_rev1_UI<fx_zita_rev1> fx_zita_rev1_UI;

class faust_fx_zita_rev1_tilde : public FaustExternal<fx_zita_rev1, fx_zita_rev1_UI> {
public:
    faust_fx_zita_rev1_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
