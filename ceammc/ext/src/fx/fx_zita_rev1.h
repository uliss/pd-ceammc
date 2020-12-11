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
	FAUSTFLOAT fHslider0;
	float fRec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fVslider0;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fHslider1;
	float fConst4;
	FAUSTFLOAT fVslider2;
	float fRec12[2];
	FAUSTFLOAT fVslider3;
	FAUSTFLOAT fHslider2;
	float fRec11[2];
	int IOTA;
	float fVec0[65536];
	float fConst5;
	int iConst6;
	FAUSTFLOAT fHslider3;
	float fConst7;
	float fRec13[2];
	float fVec1[16384];
	float fConst8;
	FAUSTFLOAT fVslider4;
	float fRec14[2];
	float fVec2[4096];
	int iConst9;
	float fRec9[2];
	float fConst10;
	float fConst11;
	float fRec18[2];
	float fRec17[2];
	float fVec3[65536];
	float fConst12;
	int iConst13;
	float fVec4[8192];
	int iConst14;
	float fRec15[2];
	float fConst15;
	float fConst16;
	float fRec22[2];
	float fRec21[2];
	float fVec5[32768];
	float fConst17;
	int iConst18;
	float fVec6[8192];
	int iConst19;
	float fRec19[2];
	float fConst20;
	float fConst21;
	float fRec26[2];
	float fRec25[2];
	float fVec7[65536];
	float fConst22;
	int iConst23;
	float fVec8[8192];
	int iConst24;
	float fRec23[2];
	float fConst25;
	float fConst26;
	float fRec30[2];
	float fRec29[2];
	float fVec9[32768];
	float fConst27;
	int iConst28;
	float fVec10[16384];
	float fVec11[4096];
	int iConst29;
	float fRec27[2];
	float fConst30;
	float fConst31;
	float fRec34[2];
	float fRec33[2];
	float fVec12[32768];
	float fConst32;
	int iConst33;
	float fVec13[4096];
	int iConst34;
	float fRec31[2];
	float fConst35;
	float fConst36;
	float fRec38[2];
	float fRec37[2];
	float fVec14[32768];
	float fConst37;
	int iConst38;
	float fVec15[8192];
	int iConst39;
	float fRec35[2];
	float fConst40;
	float fConst41;
	float fRec42[2];
	float fRec41[2];
	float fVec16[32768];
	float fConst42;
	int iConst43;
	float fVec17[8192];
	int iConst44;
	float fRec39[2];
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
		fConst1 = (6.28318548f / fConst0);
		fConst2 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst3 = ((0.0f - (6.90775537f * fConst2)) / fConst0);
		fConst4 = (3.14159274f / fConst0);
		fConst5 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst6 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst2 - fConst5))));
		fConst7 = (1.0f / fConst0);
		fConst8 = (0.00100000005f * fConst0);
		iConst9 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst5 + -1.0f))));
		fConst10 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst11 = ((0.0f - (6.90775537f * fConst10)) / fConst0);
		fConst12 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst13 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst10 - fConst12))));
		iConst14 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst12 + -1.0f))));
		fConst15 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst16 = ((0.0f - (6.90775537f * fConst15)) / fConst0);
		fConst17 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst18 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst15 - fConst17))));
		iConst19 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst17 + -1.0f))));
		fConst20 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst21 = ((0.0f - (6.90775537f * fConst20)) / fConst0);
		fConst22 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst23 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst20 - fConst22))));
		iConst24 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst22 + -1.0f))));
		fConst25 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst26 = ((0.0f - (6.90775537f * fConst25)) / fConst0);
		fConst27 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst28 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst25 - fConst27))));
		iConst29 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst27 + -1.0f))));
		fConst30 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst31 = ((0.0f - (6.90775537f * fConst30)) / fConst0);
		fConst32 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst33 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst30 - fConst32))));
		iConst34 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst32 + -1.0f))));
		fConst35 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst36 = ((0.0f - (6.90775537f * fConst35)) / fConst0);
		fConst37 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst38 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst35 - fConst37))));
		iConst39 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst37 + -1.0f))));
		fConst40 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst41 = ((0.0f - (6.90775537f * fConst40)) / fConst0);
		fConst42 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst43 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst40 - fConst42))));
		iConst44 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst42 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fVslider0 = FAUSTFLOAT(6000.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(2.0f);
		fHslider1 = FAUSTFLOAT(40.0f);
		fVslider2 = FAUSTFLOAT(200.0f);
		fVslider3 = FAUSTFLOAT(3.0f);
		fHslider2 = FAUSTFLOAT(40.0f);
		fHslider3 = FAUSTFLOAT(100.0f);
		fVslider4 = FAUSTFLOAT(60.0f);
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
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 65536); l3 = (l3 + 1)) {
			fVec0[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec13[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 16384); l5 = (l5 + 1)) {
			fVec1[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec14[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 4096); l7 = (l7 + 1)) {
			fVec2[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec9[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec18[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec17[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 65536); l11 = (l11 + 1)) {
			fVec3[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 8192); l12 = (l12 + 1)) {
			fVec4[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec15[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec22[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec21[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 32768); l16 = (l16 + 1)) {
			fVec5[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 8192); l17 = (l17 + 1)) {
			fVec6[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec19[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec26[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec25[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 65536); l21 = (l21 + 1)) {
			fVec7[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 8192); l22 = (l22 + 1)) {
			fVec8[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec23[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec30[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec29[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 32768); l26 = (l26 + 1)) {
			fVec9[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 16384); l27 = (l27 + 1)) {
			fVec10[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 4096); l28 = (l28 + 1)) {
			fVec11[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec27[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec34[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec33[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 32768); l32 = (l32 + 1)) {
			fVec12[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 4096); l33 = (l33 + 1)) {
			fVec13[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec31[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec38[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec37[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 32768); l37 = (l37 + 1)) {
			fVec14[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 8192); l38 = (l38 + 1)) {
			fVec15[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec35[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec42[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec41[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 32768); l42 = (l42 + 1)) {
			fVec16[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 8192); l43 = (l43 + 1)) {
			fVec17[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec39[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec1[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec2[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec3[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec4[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec5[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec6[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec7[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec8[l52] = 0.0f;
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
		ui_interface->declare(&fVslider3, "scale", "log");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low", &fVslider3, 3.0f, 1.0f, 60.0f, 0.100000001f);
		ui_interface->declare(&fVslider1, "scale", "log");
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid", &fVslider1, 2.0f, 1.0f, 60.0f, 0.100000001f);
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "unit", "ms");
		ui_interface->addVerticalSlider("delay", &fVslider4, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fVslider0, "scale", "log");
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("dump_hf", &fVslider0, 6000.0f, 1500.0f, 47040.0f, 1.0f);
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->declare(&fHslider2, "unit", "sec");
		ui_interface->addHorizontalSlider("fr_ldecay", &fHslider2, 40.0f, 0.0f, 60.0f, 0.00100000005f);
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "sec");
		ui_interface->addHorizontalSlider("fr_mdecay", &fHslider1, 40.0f, 0.0f, 60.0f, 0.00100000005f);
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->declare(&fHslider3, "unit", "ms");
		ui_interface->addHorizontalSlider("fr_time", &fHslider3, 100.0f, 0.0f, 1000.0f, 0.00100000005f);
		ui_interface->declare(&fCheckbox1, "type", "bool");
		ui_interface->addCheckButton("freeze", &fCheckbox1);
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "Hz");
		ui_interface->addVerticalSlider("freq_low", &fVslider2, 200.0f, 50.0f, 1000.0f, 1.0f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = (0.00100000005f * float(fHslider0));
		float fSlow2 = std::cos((fConst1 * float(fVslider0)));
		int iSlow3 = int(float(fCheckbox1));
		float fSlow4 = (iSlow3 ? float(fHslider1) : float(fVslider1));
		float fSlow5 = std::exp((fConst3 / fSlow4));
		float fSlow6 = fx_zita_rev1_faustpower2_f(fSlow5);
		float fSlow7 = (1.0f - (fSlow2 * fSlow6));
		float fSlow8 = (1.0f - fSlow6);
		float fSlow9 = (fSlow7 / fSlow8);
		float fSlow10 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow7) / fx_zita_rev1_faustpower2_f(fSlow8)) + -1.0f)));
		float fSlow11 = (fSlow9 - fSlow10);
		float fSlow12 = (1.0f / std::tan((fConst4 * float(fVslider2))));
		float fSlow13 = (1.0f / (fSlow12 + 1.0f));
		float fSlow14 = (1.0f - fSlow12);
		float fSlow15 = (iSlow3 ? float(fHslider2) : float(fVslider3));
		float fSlow16 = ((std::exp((fConst3 / fSlow15)) / fSlow5) + -1.0f);
		float fSlow17 = (fSlow10 + (1.0f - fSlow9));
		float fSlow18 = (0.00100000005f * float(fHslider3));
		int iSlow19 = (std::fabs(fSlow18) < 1.1920929e-07f);
		float fSlow20 = (iSlow19 ? 0.0f : std::exp((0.0f - (fConst7 / (iSlow19 ? 1.0f : fSlow18)))));
		float fSlow21 = (float((1 - iSlow3)) * (1.0f - fSlow20));
		float fSlow22 = (0.00100000005f * float(fVslider4));
		float fSlow23 = std::exp((fConst11 / fSlow4));
		float fSlow24 = fx_zita_rev1_faustpower2_f(fSlow23);
		float fSlow25 = (1.0f - (fSlow2 * fSlow24));
		float fSlow26 = (1.0f - fSlow24);
		float fSlow27 = (fSlow25 / fSlow26);
		float fSlow28 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow25) / fx_zita_rev1_faustpower2_f(fSlow26)) + -1.0f)));
		float fSlow29 = (fSlow27 - fSlow28);
		float fSlow30 = ((std::exp((fConst11 / fSlow15)) / fSlow23) + -1.0f);
		float fSlow31 = (fSlow28 + (1.0f - fSlow27));
		float fSlow32 = std::exp((fConst16 / fSlow4));
		float fSlow33 = fx_zita_rev1_faustpower2_f(fSlow32);
		float fSlow34 = (1.0f - (fSlow2 * fSlow33));
		float fSlow35 = (1.0f - fSlow33);
		float fSlow36 = (fSlow34 / fSlow35);
		float fSlow37 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow34) / fx_zita_rev1_faustpower2_f(fSlow35)) + -1.0f)));
		float fSlow38 = (fSlow36 - fSlow37);
		float fSlow39 = ((std::exp((fConst16 / fSlow15)) / fSlow32) + -1.0f);
		float fSlow40 = (fSlow37 + (1.0f - fSlow36));
		float fSlow41 = std::exp((fConst21 / fSlow4));
		float fSlow42 = fx_zita_rev1_faustpower2_f(fSlow41);
		float fSlow43 = (1.0f - (fSlow2 * fSlow42));
		float fSlow44 = (1.0f - fSlow42);
		float fSlow45 = (fSlow43 / fSlow44);
		float fSlow46 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow43) / fx_zita_rev1_faustpower2_f(fSlow44)) + -1.0f)));
		float fSlow47 = (fSlow45 - fSlow46);
		float fSlow48 = ((std::exp((fConst21 / fSlow15)) / fSlow41) + -1.0f);
		float fSlow49 = (fSlow46 + (1.0f - fSlow45));
		float fSlow50 = std::exp((fConst26 / fSlow4));
		float fSlow51 = fx_zita_rev1_faustpower2_f(fSlow50);
		float fSlow52 = (1.0f - (fSlow2 * fSlow51));
		float fSlow53 = (1.0f - fSlow51);
		float fSlow54 = (fSlow52 / fSlow53);
		float fSlow55 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow52) / fx_zita_rev1_faustpower2_f(fSlow53)) + -1.0f)));
		float fSlow56 = (fSlow54 - fSlow55);
		float fSlow57 = ((std::exp((fConst26 / fSlow15)) / fSlow50) + -1.0f);
		float fSlow58 = (fSlow55 + (1.0f - fSlow54));
		float fSlow59 = std::exp((fConst31 / fSlow4));
		float fSlow60 = fx_zita_rev1_faustpower2_f(fSlow59);
		float fSlow61 = (1.0f - (fSlow2 * fSlow60));
		float fSlow62 = (1.0f - fSlow60);
		float fSlow63 = (fSlow61 / fSlow62);
		float fSlow64 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow61) / fx_zita_rev1_faustpower2_f(fSlow62)) + -1.0f)));
		float fSlow65 = (fSlow63 - fSlow64);
		float fSlow66 = ((std::exp((fConst31 / fSlow15)) / fSlow59) + -1.0f);
		float fSlow67 = (fSlow64 + (1.0f - fSlow63));
		float fSlow68 = std::exp((fConst36 / fSlow4));
		float fSlow69 = fx_zita_rev1_faustpower2_f(fSlow68);
		float fSlow70 = (1.0f - (fSlow2 * fSlow69));
		float fSlow71 = (1.0f - fSlow69);
		float fSlow72 = (fSlow70 / fSlow71);
		float fSlow73 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow70) / fx_zita_rev1_faustpower2_f(fSlow71)) + -1.0f)));
		float fSlow74 = (fSlow72 - fSlow73);
		float fSlow75 = ((std::exp((fConst36 / fSlow15)) / fSlow68) + -1.0f);
		float fSlow76 = (fSlow73 + (1.0f - fSlow72));
		float fSlow77 = std::exp((fConst41 / fSlow4));
		float fSlow78 = fx_zita_rev1_faustpower2_f(fSlow77);
		float fSlow79 = (1.0f - (fSlow2 * fSlow78));
		float fSlow80 = (1.0f - fSlow78);
		float fSlow81 = (fSlow79 / fSlow80);
		float fSlow82 = std::sqrt(std::max<float>(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow79) / fx_zita_rev1_faustpower2_f(fSlow80)) + -1.0f)));
		float fSlow83 = (fSlow81 - fSlow82);
		float fSlow84 = ((std::exp((fConst41 / fSlow15)) / fSlow77) + -1.0f);
		float fSlow85 = (fSlow82 + (1.0f - fSlow81));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			float fTemp1 = (iSlow0 ? 0.0f : fTemp0);
			fRec0[0] = (fSlow1 + (0.999000013f * fRec0[1]));
			float fTemp2 = (1.0f - fRec0[0]);
			fRec12[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec12[1]) - (fRec8[1] + fRec8[2]))));
			fRec11[0] = ((fRec11[1] * fSlow11) + ((fSlow5 * (fRec8[1] + (fRec12[0] * fSlow16))) * fSlow17));
			fVec0[(IOTA & 65535)] = ((0.353553385f * fRec11[0]) + 9.99999968e-21f);
			fRec13[0] = ((fRec13[1] * fSlow20) + fSlow21);
			float fTemp3 = float(input1[i]);
			float fTemp4 = (iSlow0 ? 0.0f : fTemp3);
			fVec1[(IOTA & 16383)] = (fRec13[0] * fTemp4);
			fRec14[0] = (fSlow22 + (0.999000013f * fRec14[1]));
			int iTemp5 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst8 * fRec14[0]))));
			float fTemp6 = (0.300000012f * fVec1[((IOTA - iTemp5) & 16383)]);
			float fTemp7 = (((0.600000024f * fRec9[1]) + fVec0[((IOTA - iConst6) & 65535)]) - fTemp6);
			fVec2[(IOTA & 4095)] = fTemp7;
			fRec9[0] = fVec2[((IOTA - iConst9) & 4095)];
			float fRec10 = (0.0f - (0.600000024f * fTemp7));
			fRec18[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec18[1]) - (fRec4[1] + fRec4[2]))));
			fRec17[0] = ((fRec17[1] * fSlow29) + ((fSlow23 * (fRec4[1] + (fRec18[0] * fSlow30))) * fSlow31));
			fVec3[(IOTA & 65535)] = ((0.353553385f * fRec17[0]) + 9.99999968e-21f);
			float fTemp8 = (((0.600000024f * fRec15[1]) + fVec3[((IOTA - iConst13) & 65535)]) - fTemp6);
			fVec4[(IOTA & 8191)] = fTemp8;
			fRec15[0] = fVec4[((IOTA - iConst14) & 8191)];
			float fRec16 = (0.0f - (0.600000024f * fTemp8));
			fRec22[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec22[1]) - (fRec6[1] + fRec6[2]))));
			fRec21[0] = ((fRec21[1] * fSlow38) + ((fSlow32 * (fRec6[1] + (fRec22[0] * fSlow39))) * fSlow40));
			fVec5[(IOTA & 32767)] = ((0.353553385f * fRec21[0]) + 9.99999968e-21f);
			float fTemp9 = (fVec5[((IOTA - iConst18) & 32767)] + (fTemp6 + (0.600000024f * fRec19[1])));
			fVec6[(IOTA & 8191)] = fTemp9;
			fRec19[0] = fVec6[((IOTA - iConst19) & 8191)];
			float fRec20 = (0.0f - (0.600000024f * fTemp9));
			fRec26[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec26[1]) - (fRec2[1] + fRec2[2]))));
			fRec25[0] = ((fRec25[1] * fSlow47) + ((fSlow41 * (fRec2[1] + (fRec26[0] * fSlow48))) * fSlow49));
			fVec7[(IOTA & 65535)] = ((0.353553385f * fRec25[0]) + 9.99999968e-21f);
			float fTemp10 = (fTemp6 + ((0.600000024f * fRec23[1]) + fVec7[((IOTA - iConst23) & 65535)]));
			fVec8[(IOTA & 8191)] = fTemp10;
			fRec23[0] = fVec8[((IOTA - iConst24) & 8191)];
			float fRec24 = (0.0f - (0.600000024f * fTemp10));
			fRec30[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec30[1]) - (fRec1[1] + fRec1[2]))));
			fRec29[0] = ((fRec29[1] * fSlow56) + ((fSlow50 * (fRec1[1] + (fRec30[0] * fSlow57))) * fSlow58));
			fVec9[(IOTA & 32767)] = ((0.353553385f * fRec29[0]) + 9.99999968e-21f);
			fVec10[(IOTA & 16383)] = (fRec13[0] * fTemp1);
			float fTemp11 = (0.300000012f * fVec10[((IOTA - iTemp5) & 16383)]);
			float fTemp12 = ((fVec9[((IOTA - iConst28) & 32767)] + fTemp11) - (0.600000024f * fRec27[1]));
			fVec11[(IOTA & 4095)] = fTemp12;
			fRec27[0] = fVec11[((IOTA - iConst29) & 4095)];
			float fRec28 = (0.600000024f * fTemp12);
			fRec34[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec34[1]) - (fRec7[1] + fRec7[2]))));
			fRec33[0] = ((fRec33[1] * fSlow65) + ((fSlow59 * (fRec7[1] + (fRec34[0] * fSlow66))) * fSlow67));
			fVec12[(IOTA & 32767)] = ((0.353553385f * fRec33[0]) + 9.99999968e-21f);
			float fTemp13 = (fVec12[((IOTA - iConst33) & 32767)] - (fTemp11 + (0.600000024f * fRec31[1])));
			fVec13[(IOTA & 4095)] = fTemp13;
			fRec31[0] = fVec13[((IOTA - iConst34) & 4095)];
			float fRec32 = (0.600000024f * fTemp13);
			fRec38[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec38[1]) - (fRec3[1] + fRec3[2]))));
			fRec37[0] = ((fRec37[1] * fSlow74) + ((fSlow68 * (fRec3[1] + (fRec38[0] * fSlow75))) * fSlow76));
			fVec14[(IOTA & 32767)] = ((0.353553385f * fRec37[0]) + 9.99999968e-21f);
			float fTemp14 = (fVec14[((IOTA - iConst38) & 32767)] - (fTemp11 + (0.600000024f * fRec35[1])));
			fVec15[(IOTA & 8191)] = fTemp14;
			fRec35[0] = fVec15[((IOTA - iConst39) & 8191)];
			float fRec36 = (0.600000024f * fTemp14);
			fRec42[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec42[1]) - (fRec5[1] + fRec5[2]))));
			fRec41[0] = ((fRec41[1] * fSlow83) + ((fSlow77 * (fRec5[1] + (fRec42[0] * fSlow84))) * fSlow85));
			fVec16[(IOTA & 32767)] = ((0.353553385f * fRec41[0]) + 9.99999968e-21f);
			float fTemp15 = ((fTemp11 + fVec16[((IOTA - iConst43) & 32767)]) - (0.600000024f * fRec39[1]));
			fVec17[(IOTA & 8191)] = fTemp15;
			fRec39[0] = fVec17[((IOTA - iConst44) & 8191)];
			float fRec40 = (0.600000024f * fTemp15);
			float fTemp16 = (fRec39[1] + fRec35[1]);
			float fTemp17 = (fRec32 + (fRec36 + (fRec40 + (fRec28 + (fRec31[1] + fTemp16)))));
			fRec1[0] = (fRec9[1] + (fRec15[1] + (fRec19[1] + (fRec23[1] + (fRec27[1] + (fRec10 + (fRec16 + (fRec20 + (fRec24 + fTemp17)))))))));
			fRec2[0] = ((fRec27[1] + fTemp17) - (fRec9[1] + (fRec15[1] + (fRec19[1] + (fRec23[1] + (fRec10 + (fRec16 + (fRec24 + fRec20))))))));
			float fTemp18 = (fRec40 + (fRec28 + fRec39[1]));
			float fTemp19 = (fRec32 + (fRec36 + (fRec35[1] + fRec31[1])));
			fRec3[0] = ((fRec19[1] + (fRec23[1] + (fRec27[1] + (fRec20 + (fRec24 + fTemp18))))) - (fRec9[1] + (fRec15[1] + (fRec10 + (fRec16 + fTemp19)))));
			fRec4[0] = ((fRec9[1] + (fRec15[1] + (fRec27[1] + (fRec10 + (fRec16 + fTemp18))))) - (fRec19[1] + (fRec23[1] + (fRec20 + (fRec24 + fTemp19)))));
			float fTemp20 = (fRec36 + (fRec28 + fRec35[1]));
			float fTemp21 = (fRec32 + (fRec40 + (fRec39[1] + fRec31[1])));
			fRec5[0] = ((fRec15[1] + (fRec23[1] + (fRec27[1] + (fRec16 + (fRec24 + fTemp20))))) - (fRec9[1] + (fRec19[1] + (fRec10 + (fRec20 + fTemp21)))));
			fRec6[0] = ((fRec9[1] + (fRec19[1] + (fRec27[1] + (fRec10 + (fRec20 + fTemp20))))) - (fRec15[1] + (fRec23[1] + (fRec16 + (fRec24 + fTemp21)))));
			float fTemp22 = (fRec32 + (fRec28 + fRec31[1]));
			float fTemp23 = (fRec36 + (fRec40 + fTemp16));
			fRec7[0] = ((fRec9[1] + (fRec23[1] + (fRec27[1] + (fRec10 + (fRec24 + fTemp22))))) - (fRec15[1] + (fRec19[1] + (fRec16 + (fRec20 + fTemp23)))));
			fRec8[0] = ((fRec15[1] + (fRec19[1] + (fRec27[1] + (fRec16 + (fRec20 + fTemp22))))) - (fRec9[1] + (fRec23[1] + (fRec10 + (fRec24 + fTemp23)))));
			output0[i] = FAUSTFLOAT((iSlow0 ? fTemp0 : ((fTemp1 * fTemp2) + (0.370000005f * (fRec0[0] * (fRec2[0] + fRec3[0]))))));
			output1[i] = FAUSTFLOAT((iSlow0 ? fTemp3 : ((fTemp2 * fTemp4) + (0.370000005f * (fRec0[0] * (fRec2[0] - fRec3[0]))))));
			fRec0[1] = fRec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			IOTA = (IOTA + 1);
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec9[1] = fRec9[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec15[1] = fRec15[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec19[1] = fRec19[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec23[1] = fRec23[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fRec27[1] = fRec27[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec31[1] = fRec31[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec35[1] = fRec35[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec39[1] = fRec39[0];
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
