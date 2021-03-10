/* ------------------------------------------------------------
name: "spat.zita8"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __spat_zita8_H__
#define  __spat_zita8_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN spat_zita8_dsp.h **************************/
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

class spat_zita8_dsp {

    public:

        spat_zita8_dsp() {}
        virtual ~spat_zita8_dsp() {}

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
        virtual spat_zita8_dsp* clone() = 0;
    
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

class decorator_dsp : public spat_zita8_dsp {

    protected:

        spat_zita8_dsp* fDSP;

    public:

        decorator_dsp(spat_zita8_dsp* spat_zita8_dsp = nullptr):fDSP(spat_zita8_dsp) {}
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
    
        virtual spat_zita8_dsp* createDSPInstance() = 0;
    
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
/**************************  END  spat_zita8_dsp.h **************************/
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
 The base class of Meta handler to be used in spat_zita8_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct spat_zita8 : public spat_zita8_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include "m_pd.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

static float spat_zita8_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS spat_zita8
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class spat_zita8 : public spat_zita8_dsp {
	
 private:
	
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
	int IOTA;
	float fVec0[65536];
	int iConst6;
	float fVec1[4096];
	int iConst7;
	float fRec8[2];
	float fConst9;
	float fRec15[2];
	float fRec14[2];
	float fVec2[65536];
	int iConst11;
	float fVec3[8192];
	int iConst12;
	float fRec12[2];
	float fConst14;
	float fRec19[2];
	float fRec18[2];
	float fVec4[32768];
	int iConst16;
	float fVec5[8192];
	int iConst17;
	float fRec16[2];
	float fConst19;
	float fRec23[2];
	float fRec22[2];
	float fVec6[65536];
	int iConst21;
	float fVec7[8192];
	int iConst22;
	float fRec20[2];
	float fConst24;
	float fRec27[2];
	float fRec26[2];
	float fVec8[32768];
	int iConst26;
	float fVec9[16384];
	float fConst27;
	FAUSTFLOAT fVslider4;
	float fRec28[2];
	float fVec10[4096];
	int iConst28;
	float fRec24[2];
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
	float fVec14[8192];
	int iConst38;
	float fRec33[2];
	float fConst40;
	float fRec40[2];
	float fRec39[2];
	float fVec15[32768];
	int iConst42;
	float fVec16[4096];
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
	FAUSTFLOAT fVslider5;
	FAUSTFLOAT fVslider6;
	float fRec41[2];
	float fRec42[2];
	float fRec43[2];
	float fRec44[2];
	float fRec45[2];
	float fRec46[2];
	float fRec47[2];
	float fRec48[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "spat_zita8.dsp");
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
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "spat.zita8");
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
		return 8;
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
			case 1: {
				rate = 1;
				break;
			}
			case 2: {
				rate = 1;
				break;
			}
			case 3: {
				rate = 1;
				break;
			}
			case 4: {
				rate = 1;
				break;
			}
			case 5: {
				rate = 1;
				break;
			}
			case 6: {
				rate = 1;
				break;
			}
			case 7: {
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
		iConst7 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst5 + -1.0f))));
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
		fConst27 = (0.00100000005f * fConst0);
		iConst28 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst25 + -1.0f))));
		float fConst29 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst30 = ((0.0f - (6.90775537f * fConst29)) / fConst0);
		float fConst31 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst32 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst29 - fConst31))));
		iConst33 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst31 + -1.0f))));
		float fConst34 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst35 = ((0.0f - (6.90775537f * fConst34)) / fConst0);
		float fConst36 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst37 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst34 - fConst36))));
		iConst38 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst36 + -1.0f))));
		float fConst39 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst40 = ((0.0f - (6.90775537f * fConst39)) / fConst0);
		float fConst41 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst42 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst39 - fConst41))));
		iConst43 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst41 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(2.0f);
		fVslider1 = FAUSTFLOAT(6000.0f);
		fVslider2 = FAUSTFLOAT(3.0f);
		fVslider3 = FAUSTFLOAT(200.0f);
		fVslider4 = FAUSTFLOAT(60.0f);
		fVslider5 = FAUSTFLOAT(1.0f);
		fVslider6 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec11[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec10[l1] = 0.0f;
		}
		IOTA = 0;
		for (int l2 = 0; (l2 < 65536); l2 = (l2 + 1)) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 4096); l3 = (l3 + 1)) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec8[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec15[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec14[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 65536); l7 = (l7 + 1)) {
			fVec2[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 8192); l8 = (l8 + 1)) {
			fVec3[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec12[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec19[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec18[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 32768); l12 = (l12 + 1)) {
			fVec4[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 8192); l13 = (l13 + 1)) {
			fVec5[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec16[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec23[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec22[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 65536); l17 = (l17 + 1)) {
			fVec6[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 8192); l18 = (l18 + 1)) {
			fVec7[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec20[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec27[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec26[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 32768); l22 = (l22 + 1)) {
			fVec8[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 16384); l23 = (l23 + 1)) {
			fVec9[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec28[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 4096); l25 = (l25 + 1)) {
			fVec10[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec24[l26] = 0.0f;
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
		for (int l35 = 0; (l35 < 8192); l35 = (l35 + 1)) {
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
		for (int l40 = 0; (l40 < 4096); l40 = (l40 + 1)) {
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
			fRec41[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec42[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec43[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec44[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec45[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec46[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec47[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec48[l57] = 0.0f;
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
	
	virtual spat_zita8* clone() {
		return new spat_zita8();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("spat.zita8");
		ui_interface->addVerticalSlider("angle", &fVslider6, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low", &fVslider2, 3.0f, 1.0f, 60.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "scale", "log");
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid", &fVslider0, 2.0f, 1.0f, 60.0f, 0.100000001f);
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
		ui_interface->addVerticalSlider("radius", &fVslider5, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		FAUSTFLOAT* output2 = outputs[2];
		FAUSTFLOAT* output3 = outputs[3];
		FAUSTFLOAT* output4 = outputs[4];
		FAUSTFLOAT* output5 = outputs[5];
		FAUSTFLOAT* output6 = outputs[6];
		FAUSTFLOAT* output7 = outputs[7];
		float fSlow0 = float(fVslider0);
		float fSlow1 = std::exp((fConst2 / fSlow0));
		float fSlow2 = spat_zita8_faustpower2_f(fSlow1);
		float fSlow3 = std::cos((fConst3 * float(fVslider1)));
		float fSlow4 = (1.0f - (fSlow2 * fSlow3));
		float fSlow5 = (1.0f - fSlow2);
		float fSlow6 = (fSlow4 / fSlow5);
		float fSlow7 = std::sqrt(std::max<float>(0.0f, ((spat_zita8_faustpower2_f(fSlow4) / spat_zita8_faustpower2_f(fSlow5)) + -1.0f)));
		float fSlow8 = (fSlow6 - fSlow7);
		float fSlow9 = (fSlow1 * (fSlow7 + (1.0f - fSlow6)));
		float fSlow10 = float(fVslider2);
		float fSlow11 = ((std::exp((fConst2 / fSlow10)) / fSlow1) + -1.0f);
		float fSlow12 = (1.0f / std::tan((fConst4 * float(fVslider3))));
		float fSlow13 = (1.0f / (fSlow12 + 1.0f));
		float fSlow14 = (1.0f - fSlow12);
		float fSlow15 = std::exp((fConst9 / fSlow0));
		float fSlow16 = spat_zita8_faustpower2_f(fSlow15);
		float fSlow17 = (1.0f - (fSlow16 * fSlow3));
		float fSlow18 = (1.0f - fSlow16);
		float fSlow19 = (fSlow17 / fSlow18);
		float fSlow20 = std::sqrt(std::max<float>(0.0f, ((spat_zita8_faustpower2_f(fSlow17) / spat_zita8_faustpower2_f(fSlow18)) + -1.0f)));
		float fSlow21 = (fSlow19 - fSlow20);
		float fSlow22 = (fSlow15 * (fSlow20 + (1.0f - fSlow19)));
		float fSlow23 = ((std::exp((fConst9 / fSlow10)) / fSlow15) + -1.0f);
		float fSlow24 = std::exp((fConst14 / fSlow0));
		float fSlow25 = spat_zita8_faustpower2_f(fSlow24);
		float fSlow26 = (1.0f - (fSlow25 * fSlow3));
		float fSlow27 = (1.0f - fSlow25);
		float fSlow28 = (fSlow26 / fSlow27);
		float fSlow29 = std::sqrt(std::max<float>(0.0f, ((spat_zita8_faustpower2_f(fSlow26) / spat_zita8_faustpower2_f(fSlow27)) + -1.0f)));
		float fSlow30 = (fSlow28 - fSlow29);
		float fSlow31 = (fSlow24 * (fSlow29 + (1.0f - fSlow28)));
		float fSlow32 = ((std::exp((fConst14 / fSlow10)) / fSlow24) + -1.0f);
		float fSlow33 = std::exp((fConst19 / fSlow0));
		float fSlow34 = spat_zita8_faustpower2_f(fSlow33);
		float fSlow35 = (1.0f - (fSlow34 * fSlow3));
		float fSlow36 = (1.0f - fSlow34);
		float fSlow37 = (fSlow35 / fSlow36);
		float fSlow38 = std::sqrt(std::max<float>(0.0f, ((spat_zita8_faustpower2_f(fSlow35) / spat_zita8_faustpower2_f(fSlow36)) + -1.0f)));
		float fSlow39 = (fSlow37 - fSlow38);
		float fSlow40 = (fSlow33 * (fSlow38 + (1.0f - fSlow37)));
		float fSlow41 = ((std::exp((fConst19 / fSlow10)) / fSlow33) + -1.0f);
		float fSlow42 = std::exp((fConst24 / fSlow0));
		float fSlow43 = spat_zita8_faustpower2_f(fSlow42);
		float fSlow44 = (1.0f - (fSlow43 * fSlow3));
		float fSlow45 = (1.0f - fSlow43);
		float fSlow46 = (fSlow44 / fSlow45);
		float fSlow47 = std::sqrt(std::max<float>(0.0f, ((spat_zita8_faustpower2_f(fSlow44) / spat_zita8_faustpower2_f(fSlow45)) + -1.0f)));
		float fSlow48 = (fSlow46 - fSlow47);
		float fSlow49 = (fSlow42 * (fSlow47 + (1.0f - fSlow46)));
		float fSlow50 = ((std::exp((fConst24 / fSlow10)) / fSlow42) + -1.0f);
		float fSlow51 = (0.00100000005f * float(fVslider4));
		float fSlow52 = std::exp((fConst30 / fSlow0));
		float fSlow53 = spat_zita8_faustpower2_f(fSlow52);
		float fSlow54 = (1.0f - (fSlow53 * fSlow3));
		float fSlow55 = (1.0f - fSlow53);
		float fSlow56 = (fSlow54 / fSlow55);
		float fSlow57 = std::sqrt(std::max<float>(0.0f, ((spat_zita8_faustpower2_f(fSlow54) / spat_zita8_faustpower2_f(fSlow55)) + -1.0f)));
		float fSlow58 = (fSlow56 - fSlow57);
		float fSlow59 = (fSlow52 * (fSlow57 + (1.0f - fSlow56)));
		float fSlow60 = ((std::exp((fConst30 / fSlow10)) / fSlow52) + -1.0f);
		float fSlow61 = std::exp((fConst35 / fSlow0));
		float fSlow62 = spat_zita8_faustpower2_f(fSlow61);
		float fSlow63 = (1.0f - (fSlow62 * fSlow3));
		float fSlow64 = (1.0f - fSlow62);
		float fSlow65 = (fSlow63 / fSlow64);
		float fSlow66 = std::sqrt(std::max<float>(0.0f, ((spat_zita8_faustpower2_f(fSlow63) / spat_zita8_faustpower2_f(fSlow64)) + -1.0f)));
		float fSlow67 = (fSlow65 - fSlow66);
		float fSlow68 = (fSlow61 * (fSlow66 + (1.0f - fSlow65)));
		float fSlow69 = ((std::exp((fConst35 / fSlow10)) / fSlow61) + -1.0f);
		float fSlow70 = std::exp((fConst40 / fSlow0));
		float fSlow71 = spat_zita8_faustpower2_f(fSlow70);
		float fSlow72 = (1.0f - (fSlow71 * fSlow3));
		float fSlow73 = (1.0f - fSlow71);
		float fSlow74 = (fSlow72 / fSlow73);
		float fSlow75 = std::sqrt(std::max<float>(0.0f, ((spat_zita8_faustpower2_f(fSlow72) / spat_zita8_faustpower2_f(fSlow73)) + -1.0f)));
		float fSlow76 = (fSlow74 - fSlow75);
		float fSlow77 = (fSlow70 * (fSlow75 + (1.0f - fSlow74)));
		float fSlow78 = ((std::exp((fConst40 / fSlow10)) / fSlow70) + -1.0f);
		float fSlow79 = float(fVslider5);
		float fSlow80 = (fSlow79 + 1.0f);
		float fSlow81 = float(fVslider6);
		float fSlow82 = (4.99999987e-05f * (fSlow80 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow79 * std::fabs((std::fmod((fSlow81 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow83 = (4.99999987e-05f * (fSlow80 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow79 * std::fabs((std::fmod((fSlow81 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow84 = (4.99999987e-05f * (fSlow80 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow79 * std::fabs((std::fmod((fSlow81 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow85 = (4.99999987e-05f * (fSlow80 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow79 * std::fabs((std::fmod((fSlow81 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow86 = (4.99999987e-05f * (fSlow80 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow79 * std::fabs((std::fmod((fSlow81 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow87 = (4.99999987e-05f * (fSlow80 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow79 * std::fabs((std::fmod((fSlow81 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow88 = (4.99999987e-05f * (fSlow80 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow79 * std::fabs((std::fmod((fSlow81 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow89 = (4.99999987e-05f * (fSlow80 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow79 * std::fabs((std::fmod((fSlow81 + 0.625f), 1.0f) + -0.5f)))))))))));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec11[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec11[1]) - (fRec7[1] + fRec7[2]))));
			fRec10[0] = ((fSlow8 * fRec10[1]) + (fSlow9 * (fRec7[1] + (fSlow11 * fRec11[0]))));
			fVec0[(IOTA & 65535)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			float fTemp0 = ((0.600000024f * fRec8[1]) + fVec0[((IOTA - iConst6) & 65535)]);
			fVec1[(IOTA & 4095)] = fTemp0;
			fRec8[0] = fVec1[((IOTA - iConst7) & 4095)];
			float fRec9 = (0.0f - (0.600000024f * fTemp0));
			fRec15[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec15[1]) - (fRec3[1] + fRec3[2]))));
			fRec14[0] = ((fSlow21 * fRec14[1]) + (fSlow22 * (fRec3[1] + (fSlow23 * fRec15[0]))));
			fVec2[(IOTA & 65535)] = ((0.353553385f * fRec14[0]) + 9.99999968e-21f);
			float fTemp1 = ((0.600000024f * fRec12[1]) + fVec2[((IOTA - iConst11) & 65535)]);
			fVec3[(IOTA & 8191)] = fTemp1;
			fRec12[0] = fVec3[((IOTA - iConst12) & 8191)];
			float fRec13 = (0.0f - (0.600000024f * fTemp1));
			fRec19[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec19[1]) - (fRec5[1] + fRec5[2]))));
			fRec18[0] = ((fSlow30 * fRec18[1]) + (fSlow31 * (fRec5[1] + (fSlow32 * fRec19[0]))));
			fVec4[(IOTA & 32767)] = ((0.353553385f * fRec18[0]) + 9.99999968e-21f);
			float fTemp2 = ((0.600000024f * fRec16[1]) + fVec4[((IOTA - iConst16) & 32767)]);
			fVec5[(IOTA & 8191)] = fTemp2;
			fRec16[0] = fVec5[((IOTA - iConst17) & 8191)];
			float fRec17 = (0.0f - (0.600000024f * fTemp2));
			fRec23[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec23[1]) - (fRec1[1] + fRec1[2]))));
			fRec22[0] = ((fSlow39 * fRec22[1]) + (fSlow40 * (fRec1[1] + (fSlow41 * fRec23[0]))));
			fVec6[(IOTA & 65535)] = ((0.353553385f * fRec22[0]) + 9.99999968e-21f);
			float fTemp3 = ((0.600000024f * fRec20[1]) + fVec6[((IOTA - iConst21) & 65535)]);
			fVec7[(IOTA & 8191)] = fTemp3;
			fRec20[0] = fVec7[((IOTA - iConst22) & 8191)];
			float fRec21 = (0.0f - (0.600000024f * fTemp3));
			fRec27[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec27[1]) - (fRec6[1] + fRec6[2]))));
			fRec26[0] = ((fSlow48 * fRec26[1]) + (fSlow49 * (fRec6[1] + (fSlow50 * fRec27[0]))));
			fVec8[(IOTA & 32767)] = ((0.353553385f * fRec26[0]) + 9.99999968e-21f);
			fVec9[(IOTA & 16383)] = float(input0[i]);
			fRec28[0] = (fSlow51 + (0.999000013f * fRec28[1]));
			float fTemp4 = (0.300000012f * fVec9[((IOTA - int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst27 * fRec28[0]))))) & 16383)]);
			float fTemp5 = (fVec8[((IOTA - iConst26) & 32767)] - (fTemp4 + (0.600000024f * fRec24[1])));
			fVec10[(IOTA & 4095)] = fTemp5;
			fRec24[0] = fVec10[((IOTA - iConst28) & 4095)];
			float fRec25 = (0.600000024f * fTemp5);
			fRec32[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec32[1]) - (fRec2[1] + fRec2[2]))));
			fRec31[0] = ((fSlow58 * fRec31[1]) + (fSlow59 * (fRec2[1] + (fSlow60 * fRec32[0]))));
			fVec11[(IOTA & 32767)] = ((0.353553385f * fRec31[0]) + 9.99999968e-21f);
			float fTemp6 = (fVec11[((IOTA - iConst32) & 32767)] - (fTemp4 + (0.600000024f * fRec29[1])));
			fVec12[(IOTA & 8191)] = fTemp6;
			fRec29[0] = fVec12[((IOTA - iConst33) & 8191)];
			float fRec30 = (0.600000024f * fTemp6);
			fRec36[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec36[1]) - (fRec4[1] + fRec4[2]))));
			fRec35[0] = ((fSlow67 * fRec35[1]) + (fSlow68 * (fRec4[1] + (fSlow69 * fRec36[0]))));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec35[0]) + 9.99999968e-21f);
			float fTemp7 = ((fTemp4 + fVec13[((IOTA - iConst37) & 32767)]) - (0.600000024f * fRec33[1]));
			fVec14[(IOTA & 8191)] = fTemp7;
			fRec33[0] = fVec14[((IOTA - iConst38) & 8191)];
			float fRec34 = (0.600000024f * fTemp7);
			fRec40[0] = (0.0f - (fSlow13 * ((fSlow14 * fRec40[1]) - (fRec0[1] + fRec0[2]))));
			fRec39[0] = ((fSlow76 * fRec39[1]) + (fSlow77 * (fRec0[1] + (fSlow78 * fRec40[0]))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec39[0]) + 9.99999968e-21f);
			float fTemp8 = ((fVec15[((IOTA - iConst42) & 32767)] + fTemp4) - (0.600000024f * fRec37[1]));
			fVec16[(IOTA & 4095)] = fTemp8;
			fRec37[0] = fVec16[((IOTA - iConst43) & 4095)];
			float fRec38 = (0.600000024f * fTemp8);
			float fTemp9 = (fRec38 + fRec34);
			float fTemp10 = (fRec25 + (fRec30 + fTemp9));
			fRec0[0] = (fRec8[1] + (fRec12[1] + (fRec16[1] + (fRec20[1] + (fRec24[1] + (fRec29[1] + (fRec33[1] + (fRec37[1] + (fRec9 + (fRec13 + (fRec17 + (fRec21 + fTemp10))))))))))));
			fRec1[0] = ((fRec24[1] + (fRec29[1] + (fRec33[1] + (fRec37[1] + fTemp10)))) - (fRec8[1] + (fRec12[1] + (fRec16[1] + (fRec20[1] + (fRec9 + (fRec13 + (fRec21 + fRec17))))))));
			float fTemp11 = (fRec30 + fRec25);
			fRec2[0] = ((fRec16[1] + (fRec20[1] + (fRec33[1] + (fRec37[1] + (fRec17 + (fRec21 + fTemp9)))))) - (fRec8[1] + (fRec12[1] + (fRec24[1] + (fRec29[1] + (fRec9 + (fRec13 + fTemp11)))))));
			fRec3[0] = ((fRec8[1] + (fRec12[1] + (fRec33[1] + (fRec37[1] + (fRec9 + (fRec13 + fTemp9)))))) - (fRec16[1] + (fRec20[1] + (fRec24[1] + (fRec29[1] + (fRec17 + (fRec21 + fTemp11)))))));
			float fTemp12 = (fRec38 + fRec30);
			float fTemp13 = (fRec34 + fRec25);
			fRec4[0] = ((fRec12[1] + (fRec20[1] + (fRec29[1] + (fRec37[1] + (fRec13 + (fRec21 + fTemp12)))))) - (fRec8[1] + (fRec16[1] + (fRec24[1] + (fRec33[1] + (fRec9 + (fRec17 + fTemp13)))))));
			fRec5[0] = ((fRec8[1] + (fRec16[1] + (fRec29[1] + (fRec37[1] + (fRec9 + (fRec17 + fTemp12)))))) - (fRec12[1] + (fRec20[1] + (fRec24[1] + (fRec33[1] + (fRec13 + (fRec21 + fTemp13)))))));
			float fTemp14 = (fRec38 + fRec25);
			float fTemp15 = (fRec34 + fRec30);
			fRec6[0] = ((fRec8[1] + (fRec20[1] + (fRec24[1] + (fRec37[1] + (fRec9 + (fRec21 + fTemp14)))))) - (fRec12[1] + (fRec16[1] + (fRec29[1] + (fRec33[1] + (fRec13 + (fRec17 + fTemp15)))))));
			fRec7[0] = ((fRec12[1] + (fRec16[1] + (fRec24[1] + (fRec37[1] + (fRec13 + (fRec17 + fTemp14)))))) - (fRec8[1] + (fRec20[1] + (fRec29[1] + (fRec33[1] + (fRec9 + (fRec21 + fTemp15)))))));
			float fTemp16 = (fRec1[0] + fRec2[0]);
			fRec41[0] = (fSlow82 + (0.999899983f * fRec41[1]));
			output0[i] = FAUSTFLOAT((0.370000005f * (fTemp16 * fRec41[0])));
			fRec42[0] = (fSlow83 + (0.999899983f * fRec42[1]));
			output1[i] = FAUSTFLOAT((0.370000005f * (fTemp16 * fRec42[0])));
			fRec43[0] = (fSlow84 + (0.999899983f * fRec43[1]));
			output2[i] = FAUSTFLOAT((0.370000005f * (fTemp16 * fRec43[0])));
			fRec44[0] = (fSlow85 + (0.999899983f * fRec44[1]));
			output3[i] = FAUSTFLOAT((0.370000005f * (fTemp16 * fRec44[0])));
			fRec45[0] = (fSlow86 + (0.999899983f * fRec45[1]));
			output4[i] = FAUSTFLOAT((0.370000005f * (fTemp16 * fRec45[0])));
			fRec46[0] = (fSlow87 + (0.999899983f * fRec46[1]));
			output5[i] = FAUSTFLOAT((0.370000005f * (fTemp16 * fRec46[0])));
			fRec47[0] = (fSlow88 + (0.999899983f * fRec47[1]));
			output6[i] = FAUSTFLOAT((0.370000005f * (fTemp16 * fRec47[0])));
			fRec48[0] = (fSlow89 + (0.999899983f * fRec48[1]));
			output7[i] = FAUSTFLOAT((0.370000005f * (fTemp16 * fRec48[0])));
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA = (IOTA + 1);
			fRec8[1] = fRec8[0];
			fRec15[1] = fRec15[0];
			fRec14[1] = fRec14[0];
			fRec12[1] = fRec12[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec16[1] = fRec16[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec20[1] = fRec20[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec28[1] = fRec28[0];
			fRec24[1] = fRec24[0];
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
			fRec41[1] = fRec41[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fRec46[1] = fRec46[0];
			fRec47[1] = fRec47[0];
			fRec48[1] = fRec48[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _spat_zita8_UI : public UI {
    static std::string name;
};

template <class T>
std::string _spat_zita8_UI<T>::name(sym(spat_zita8));

typedef _spat_zita8_UI<spat_zita8> spat_zita8_UI;

class faust_spat_zita8_tilde : public FaustExternal<spat_zita8, spat_zita8_UI> {
public:
    faust_spat_zita8_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
