/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.piano"
version: "1.0"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_piano_H__
#define  __synth_piano_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_piano_dsp.h **************************/
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

class synth_piano_dsp {

    public:

        synth_piano_dsp() {}
        virtual ~synth_piano_dsp() {}

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
        virtual synth_piano_dsp* clone() = 0;
    
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

class decorator_dsp : public synth_piano_dsp {

    protected:

        synth_piano_dsp* fDSP;

    public:

        decorator_dsp(synth_piano_dsp* synth_piano_dsp = nullptr):fDSP(synth_piano_dsp) {}
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
    
        virtual synth_piano_dsp* createDSPInstance() = 0;
    
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
/**************************  END  synth_piano_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_piano_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_piano : public synth_piano_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include "piano.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

static float synth_piano_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_piano
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_piano : public synth_piano_dsp {
	
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
	float fVec0[32768];
	int iConst10;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fRec12[2];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	int iRec23[2];
	FAUSTFLOAT fButton0;
	int iRec25[2];
	float fConst11;
	float fRec24[2];
	float fConst12;
	float fConst13;
	FAUSTFLOAT fHslider5;
	float fConst14;
	float fConst15;
	float fRec26[2];
	float fVec1[2];
	float fVec2[2];
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	float fRec18[2];
	float fConst16;
	float fConst17;
	float fRec17[3];
	float fRec16[3];
	float fRec15[3];
	float fRec14[3];
	float fRec13[2];
	float fConst18;
	FAUSTFLOAT fHslider6;
	float fRec39[2];
	float fRec38[2];
	float fRec37[2];
	float fRec36[2];
	float fRec35[2];
	float fRec40[2];
	float fVec3[2];
	float fRec34[2];
	float fRec33[2];
	float fRec32[8192];
	float fConst19;
	float fConst20;
	FAUSTFLOAT fHslider7;
	float fVec4[2];
	float fRec43[2];
	float fRec42[2];
	float fRec41[8192];
	float fVec5[2];
	float fRec46[2];
	float fRec45[2];
	float fRec44[8192];
	float fVec6[2];
	float fRec31[2];
	float fRec27[2];
	float fRec28[2];
	float fRec29[2];
	float fRec57[2];
	float fRec56[2];
	float fRec55[2];
	float fRec54[2];
	float fRec53[2];
	float fVec7[2];
	float fRec52[2];
	float fRec51[2];
	float fRec50[8192];
	float fVec8[2];
	float fRec61[2];
	float fRec60[2];
	float fRec59[8192];
	float fVec9[2];
	float fRec58[2];
	float fRec47[2];
	float fRec48[2];
	float fConst22;
	float fConst23;
	float fConst24;
	float fRec62[3];
	float fRec63[3];
	float fVec10[4096];
	float fConst25;
	FAUSTFLOAT fHslider8;
	float fVec11[4096];
	int iConst26;
	float fVec12[2048];
	int iConst27;
	float fRec8[2];
	float fConst30;
	float fConst31;
	float fRec67[2];
	float fRec66[2];
	float fVec13[16384];
	int iConst33;
	float fVec14[4096];
	int iConst34;
	float fRec64[2];
	float fConst37;
	float fConst38;
	float fRec71[2];
	float fRec70[2];
	float fVec15[32768];
	int iConst40;
	float fVec16[4096];
	int iConst41;
	float fRec68[2];
	float fConst44;
	float fConst45;
	float fRec75[2];
	float fRec74[2];
	float fVec17[32768];
	int iConst47;
	float fVec18[4096];
	int iConst48;
	float fRec72[2];
	float fConst51;
	float fConst52;
	float fRec79[2];
	float fRec78[2];
	float fVec19[16384];
	int iConst54;
	float fVec20[4096];
	float fVec21[2048];
	int iConst55;
	float fRec76[2];
	float fConst58;
	float fConst59;
	float fRec83[2];
	float fRec82[2];
	float fVec22[16384];
	int iConst61;
	float fVec23[4096];
	int iConst62;
	float fRec80[2];
	float fConst65;
	float fConst66;
	float fRec87[2];
	float fRec86[2];
	float fVec24[16384];
	int iConst68;
	float fVec25[4096];
	int iConst69;
	float fRec84[2];
	float fConst72;
	float fConst73;
	float fRec91[2];
	float fRec90[2];
	float fVec26[16384];
	int iConst75;
	float fVec27[2048];
	int iConst76;
	float fRec88[2];
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
		m->declare("author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("copyright", "Romain Michon");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("description", "WaveGuide Commuted Piano");
		m->declare("filename", "synth_piano.dsp");
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
		m->declare("name", "synth.piano");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
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
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::cos((37699.1133f / fConst0));
		float fConst2 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst3 = ((0.0f - (6.90775537f * fConst2)) / fConst0);
		fConst4 = (0.25f * fConst3);
		fConst5 = (0.166666672f * fConst3);
		float fConst6 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst7 = (1.0f / (fConst6 + 1.0f));
		fConst8 = (1.0f - fConst6);
		float fConst9 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst10 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst2 - fConst9))));
		fConst11 = (7.0f / fConst0);
		fConst12 = std::exp((0.0f - (5.0f / fConst0)));
		fConst13 = (10.0f / fConst0);
		fConst14 = (0.100000001f * fConst0);
		fConst15 = std::exp((0.0f - (0.5f / fConst0)));
		fConst16 = (2764.60156f / fConst0);
		fConst17 = (0.0500000007f / fConst0);
		fConst18 = (0.000361715793f * fConst0);
		fConst19 = (0.159154937f * fConst0);
		fConst20 = (6.28318548f / fConst0);
		float fConst21 = synth_piano_faustpower2_f(fConst0);
		fConst22 = (96800.0f / fConst21);
		fConst23 = (880.0f / fConst0);
		fConst24 = (193600.0f / fConst21);
		fConst25 = (0.0011363636f * fConst0);
		iConst26 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		iConst27 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst9 + -1.0f))));
		float fConst28 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst29 = ((0.0f - (6.90775537f * fConst28)) / fConst0);
		fConst30 = (0.25f * fConst29);
		fConst31 = (0.166666672f * fConst29);
		float fConst32 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst33 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst28 - fConst32))));
		iConst34 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst32 + -1.0f))));
		float fConst35 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst36 = ((0.0f - (6.90775537f * fConst35)) / fConst0);
		fConst37 = (0.25f * fConst36);
		fConst38 = (0.166666672f * fConst36);
		float fConst39 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst40 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst35 - fConst39))));
		iConst41 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst39 + -1.0f))));
		float fConst42 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst43 = ((0.0f - (6.90775537f * fConst42)) / fConst0);
		fConst44 = (0.25f * fConst43);
		fConst45 = (0.166666672f * fConst43);
		float fConst46 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst47 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst42 - fConst46))));
		iConst48 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst46 + -1.0f))));
		float fConst49 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst50 = ((0.0f - (6.90775537f * fConst49)) / fConst0);
		fConst51 = (0.25f * fConst50);
		fConst52 = (0.166666672f * fConst50);
		float fConst53 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst54 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst49 - fConst53))));
		iConst55 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst53 + -1.0f))));
		float fConst56 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst57 = ((0.0f - (6.90775537f * fConst56)) / fConst0);
		fConst58 = (0.25f * fConst57);
		fConst59 = (0.166666672f * fConst57);
		float fConst60 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst61 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst56 - fConst60))));
		iConst62 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst60 + -1.0f))));
		float fConst63 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst64 = ((0.0f - (6.90775537f * fConst63)) / fConst0);
		fConst65 = (0.25f * fConst64);
		fConst66 = (0.166666672f * fConst64);
		float fConst67 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst68 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst63 - fConst67))));
		iConst69 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst67 + -1.0f))));
		float fConst70 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst71 = ((0.0f - (6.90775537f * fConst70)) / fConst0);
		fConst72 = (0.25f * fConst71);
		fConst73 = (0.166666672f * fConst71);
		float fConst74 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst75 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst70 - fConst74))));
		iConst76 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst74 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.35999999999999999f);
		fHslider1 = FAUSTFLOAT(0.59999999999999998f);
		fHslider2 = FAUSTFLOAT(0.13700000000000001f);
		fHslider3 = FAUSTFLOAT(48.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.10000000000000001f);
		fHslider6 = FAUSTFLOAT(0.28000000000000003f);
		fHslider7 = FAUSTFLOAT(0.10000000000000001f);
		fHslider8 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec11[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec10[l1] = 0.0f;
		}
		IOTA = 0;
		for (int l2 = 0; (l2 < 32768); l2 = (l2 + 1)) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec12[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec23[l4] = 0;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec25[l5] = 0;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec24[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec26[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec1[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fVec2[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec22[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec21[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec20[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec19[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec18[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec17[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec16[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 3); l17 = (l17 + 1)) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec14[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec13[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec39[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec38[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec37[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec36[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec35[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec40[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fVec3[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec34[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec33[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 8192); l29 = (l29 + 1)) {
			fRec32[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fVec4[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec43[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec42[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 8192); l33 = (l33 + 1)) {
			fRec41[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fVec5[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec46[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec45[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 8192); l37 = (l37 + 1)) {
			fRec44[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fVec6[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec31[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec27[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec28[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec29[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec57[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec56[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec55[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec54[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec53[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fVec7[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec52[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec51[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 8192); l51 = (l51 + 1)) {
			fRec50[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fVec8[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec61[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec60[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 8192); l55 = (l55 + 1)) {
			fRec59[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fVec9[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec58[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec47[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec48[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec62[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 3); l61 = (l61 + 1)) {
			fRec63[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 4096); l62 = (l62 + 1)) {
			fVec10[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 4096); l63 = (l63 + 1)) {
			fVec11[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2048); l64 = (l64 + 1)) {
			fVec12[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec8[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec67[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec66[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 16384); l68 = (l68 + 1)) {
			fVec13[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 4096); l69 = (l69 + 1)) {
			fVec14[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec64[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec71[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec70[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 32768); l73 = (l73 + 1)) {
			fVec15[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 4096); l74 = (l74 + 1)) {
			fVec16[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec68[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec75[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec74[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 32768); l78 = (l78 + 1)) {
			fVec17[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 4096); l79 = (l79 + 1)) {
			fVec18[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec72[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec79[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec78[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 16384); l83 = (l83 + 1)) {
			fVec19[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 4096); l84 = (l84 + 1)) {
			fVec20[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 2048); l85 = (l85 + 1)) {
			fVec21[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec76[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec83[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec82[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 16384); l89 = (l89 + 1)) {
			fVec22[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 4096); l90 = (l90 + 1)) {
			fVec23[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec80[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec87[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec86[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 16384); l94 = (l94 + 1)) {
			fVec24[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 4096); l95 = (l95 + 1)) {
			fVec25[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec84[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec91[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec90[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 16384); l99 = (l99 + 1)) {
			fVec26[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 2048); l100 = (l100 + 1)) {
			fVec27[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec88[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec0[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec1[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 3); l104 = (l104 + 1)) {
			fRec2[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 3); l105 = (l105 + 1)) {
			fRec3[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec4[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec5[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec6[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 3); l109 = (l109 + 1)) {
			fRec7[l109] = 0.0f;
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
	
	virtual synth_piano* clone() {
		return new synth_piano();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.piano");
		ui_interface->addHorizontalSlider("brightness", &fHslider4, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("detuning", &fHslider7, 0.100000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider5, 0.100000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pan", &fHslider1, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider3, 48.0f, 21.0f, 108.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider0, 0.360000014f, 0.00499999989f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("stiffness", &fHslider6, 0.280000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("width", &fHslider8, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::exp((fConst4 / fSlow0));
		float fSlow2 = synth_piano_faustpower2_f(fSlow1);
		float fSlow3 = (1.0f - (fConst1 * fSlow2));
		float fSlow4 = (1.0f - fSlow2);
		float fSlow5 = (fSlow3 / fSlow4);
		float fSlow6 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow3) / synth_piano_faustpower2_f(fSlow4)) + -1.0f)));
		float fSlow7 = (fSlow5 - fSlow6);
		float fSlow8 = (fSlow1 * (fSlow6 + (1.0f - fSlow5)));
		float fSlow9 = ((std::exp((fConst5 / fSlow0)) / fSlow1) + -1.0f);
		float fSlow10 = float(fHslider1);
		float fSlow11 = (12.0f * fSlow10);
		float fSlow12 = (0.00100000005f * float(fHslider2));
		float fSlow13 = float(fHslider3);
		float fSlow14 = float(getValueDCBa1(float(fSlow13)));
		float fSlow15 = (1.0f - fSlow14);
		float fSlow16 = (2.0f * float(getValueBq4_gEarBalled(float(fSlow13))));
		float fSlow17 = (0.25f * float(fHslider4));
		float fSlow18 = float(getValueLoudPole(float(fSlow13)));
		float fSlow19 = (float(getValueLoudGain(float(fSlow13))) * (fSlow17 + (0.980000019f - fSlow18)));
		float fSlow20 = (0.5f * fSlow15);
		float fSlow21 = float((fSlow13 >= 88.0f));
		float fSlow22 = (2.32830644e-10f * fSlow21);
		float fSlow23 = float(fButton0);
		int iSlow24 = (fSlow23 > 0.0f);
		int iSlow25 = (iSlow24 > 0);
		float fSlow26 = std::exp((0.0f - (fConst11 / (float(getValueDryTapAmpT60(float(fSlow13))) * std::min<float>(1.0f, std::max<float>(0.0f, fSlow23))))));
		int iSlow27 = (iSlow24 < 1);
		float fSlow28 = float((0 - (iSlow24 + -1)));
		float fSlow29 = (fConst12 * fSlow28);
		float fSlow30 = float(iSlow24);
		float fSlow31 = float(fHslider5);
		float fSlow32 = (fSlow30 * std::exp((0.0f - (fConst13 / fSlow31))));
		float fSlow33 = (fConst14 * fSlow31);
		float fSlow34 = (0.200000003f * float(getValueSustainPedalLevel(float(fSlow13))));
		float fSlow35 = (1.16415322e-10f * fSlow21);
		float fSlow36 = (0.0f - fSlow20);
		float fSlow37 = (fSlow18 + (0.0199999996f - fSlow17));
		float fSlow38 = std::pow(2.0f, (0.0833333358f * (fSlow13 + -69.0f)));
		float fSlow39 = std::pow(10.0f, (fConst17 * float(getValuer3db(float(fSlow13)))));
		float fSlow40 = (std::cos((fConst16 * (fSlow38 * float(getValueThirdPartialFactor(float(fSlow13)))))) * (0.0f - (2.0f * fSlow39)));
		float fSlow41 = synth_piano_faustpower2_f(fSlow39);
		float fSlow42 = std::pow(10.0f, (fConst17 * float(getValuer2db(float(fSlow13)))));
		float fSlow43 = (std::cos((fConst16 * (fSlow38 * float(getValueSecondPartialFactor(float(fSlow13)))))) * (0.0f - (2.0f * fSlow42)));
		float fSlow44 = synth_piano_faustpower2_f(fSlow42);
		float fSlow45 = std::cos((fConst16 * fSlow38));
		float fSlow46 = std::pow(10.0f, (fConst17 * float(getValuer1_1db(float(fSlow13)))));
		float fSlow47 = (fSlow45 * (0.0f - (2.0f * fSlow46)));
		float fSlow48 = synth_piano_faustpower2_f(fSlow46);
		float fSlow49 = std::pow(10.0f, (0.0500000007f * float(getValueSecondStageAmpRatio(float(fSlow13)))));
		float fSlow50 = std::pow(10.0f, (fConst17 * float(getValuer1_2db(float(fSlow13)))));
		float fSlow51 = synth_piano_faustpower2_f(fSlow50);
		float fSlow52 = (1.0f - fSlow49);
		float fSlow53 = ((fSlow49 * fSlow48) + (fSlow51 * fSlow52));
		float fSlow54 = (0.0f - (2.0f * ((fSlow49 * fSlow46) + (fSlow50 * fSlow52))));
		float fSlow55 = (0.0f - (2.0f * fSlow50));
		float fSlow56 = float(getValueSingleStringZero(float(fSlow13)));
		float fSlow57 = (1.0f - fSlow56);
		float fSlow58 = std::pow(10.0f, (0.000113636364f * (float(getValueSingleStringDecayRate(float(fSlow13))) / fSlow38)));
		float fSlow59 = float(getValueSingleStringPole(float(fSlow13)));
		float fSlow60 = (1.0f - fSlow59);
		float fSlow61 = (fSlow58 * fSlow60);
		float fSlow62 = ((3.0f * fSlow57) - fSlow61);
		float fSlow63 = (1.0f / fSlow62);
		float fSlow64 = ((fSlow56 + fSlow61) + -1.0f);
		float fSlow65 = float(fHslider6);
		float fSlow66 = float(getValueStiffnessCoefficient(float(fSlow13)));
		float fSlow67 = (13.6899996f * (synth_piano_faustpower2_f(fSlow65) * synth_piano_faustpower2_f(fSlow66)));
		float fSlow68 = (fSlow67 + -1.0f);
		float fSlow69 = (fSlow38 + std::pow(2.0f, (0.0833333358f * (fSlow13 + -129.0f))));
		float fSlow70 = (fConst16 * fSlow69);
		float fSlow71 = std::sin(fSlow70);
		float fSlow72 = (fSlow65 * fSlow66);
		float fSlow73 = (7.4000001f * fSlow72);
		float fSlow74 = (fSlow67 + 1.0f);
		float fSlow75 = std::cos(fSlow70);
		float fSlow76 = (3.0f * std::atan2((fSlow68 * fSlow71), (fSlow73 + (fSlow74 * fSlow75))));
		float fSlow77 = ((fSlow56 * fSlow58) * fSlow60);
		float fSlow78 = (fSlow59 * fSlow57);
		float fSlow79 = (3.0f * fSlow78);
		float fSlow80 = (fSlow77 - fSlow79);
		float fSlow81 = (fSlow78 - fSlow77);
		float fSlow82 = (4.0f * fSlow81);
		float fSlow83 = (fSlow80 + fSlow82);
		float fSlow84 = (((fSlow75 * fSlow80) / fSlow62) + 1.0f);
		float fSlow85 = (4.0f * fSlow64);
		float fSlow86 = (((fSlow85 + (fSlow75 * fSlow83)) / fSlow62) + 1.0f);
		float fSlow87 = (synth_piano_faustpower2_f(fSlow71) * fSlow80);
		float fSlow88 = synth_piano_faustpower2_f(fSlow62);
		float fSlow89 = (fConst18 * (((fSlow76 + std::atan2((((0.0f - ((fSlow71 * fSlow83) / fSlow62)) * fSlow84) + (((fSlow71 * fSlow80) * fSlow86) / fSlow62)), ((fSlow84 * fSlow86) + ((fSlow87 * fSlow83) / fSlow88)))) + 6.28318548f) / fSlow69));
		float fSlow90 = std::floor(fSlow89);
		float fSlow91 = (fSlow89 - fSlow90);
		float fSlow92 = (1.16415322e-09f * (fSlow19 * float(((fSlow13 >= 45.0f) & (fSlow13 < 88.0f)))));
		float fSlow93 = (0.00100000005f * ((0.999599993f * fSlow30) + (0.899999976f * (float(getValueReleaseLoopGain(float(fSlow13))) * fSlow28))));
		float fSlow94 = (3.70000005f * fSlow72);
		float fSlow95 = ((fSlow77 + fSlow82) - fSlow79);
		float fSlow96 = (((fSlow85 + (fSlow75 * fSlow95)) / fSlow62) + 1.0f);
		int iSlow97 = int((fConst18 * (((fSlow76 + std::atan2((0.0f - ((fSlow71 * ((fSlow95 * fSlow84) - (fSlow80 * fSlow96))) / fSlow62)), ((fSlow84 * fSlow96) + ((fSlow87 * fSlow95) / fSlow88)))) + 6.28318548f) / fSlow69)));
		int iSlow98 = std::min<int>(4097, std::max<int>(0, (iSlow97 + 1)));
		float fSlow99 = (fSlow90 + (1.0f - fSlow89));
		int iSlow100 = std::min<int>(4097, std::max<int>(0, iSlow97));
		float fSlow101 = (440.0f * fSlow69);
		float fSlow102 = (5.0f * (float(getValueDetuningHz(float(fSlow13))) * float(fHslider7)));
		float fSlow103 = (fSlow101 - fSlow102);
		float fSlow104 = (fConst20 * fSlow103);
		float fSlow105 = std::sin(fSlow104);
		float fSlow106 = std::cos(fSlow104);
		float fSlow107 = (3.0f * std::atan2((fSlow68 * fSlow105), (fSlow73 + (fSlow74 * fSlow106))));
		float fSlow108 = (((fSlow106 * fSlow80) / fSlow62) + 1.0f);
		float fSlow109 = (((fSlow85 + (fSlow106 * fSlow83)) / fSlow62) + 1.0f);
		float fSlow110 = (synth_piano_faustpower2_f(fSlow105) * fSlow80);
		float fSlow111 = (fConst19 * (((fSlow107 + std::atan2((((0.0f - ((fSlow105 * fSlow83) / fSlow62)) * fSlow108) + (((fSlow105 * fSlow80) * fSlow109) / fSlow62)), ((fSlow108 * fSlow109) + ((fSlow110 * fSlow83) / fSlow88)))) + 6.28318548f) / fSlow103));
		float fSlow112 = std::floor(fSlow111);
		float fSlow113 = (fSlow111 - fSlow112);
		float fSlow114 = (((fSlow85 + (fSlow106 * fSlow95)) / fSlow62) + 1.0f);
		int iSlow115 = int((fConst19 * (((fSlow107 + std::atan2((0.0f - ((fSlow105 * ((fSlow95 * fSlow108) - (fSlow80 * fSlow114))) / fSlow62)), ((fSlow108 * fSlow114) + ((fSlow110 * fSlow95) / fSlow88)))) + 6.28318548f) / fSlow103)));
		int iSlow116 = std::min<int>(4097, std::max<int>(0, (iSlow115 + 1)));
		float fSlow117 = (fSlow112 + (1.0f - fSlow111));
		int iSlow118 = std::min<int>(4097, std::max<int>(0, iSlow115));
		float fSlow119 = (fSlow102 + fSlow101);
		float fSlow120 = (fConst20 * fSlow119);
		float fSlow121 = std::sin(fSlow120);
		float fSlow122 = std::cos(fSlow120);
		float fSlow123 = (3.0f * std::atan2((fSlow68 * fSlow121), (fSlow73 + (fSlow74 * fSlow122))));
		float fSlow124 = (((fSlow122 * fSlow80) / fSlow62) + 1.0f);
		float fSlow125 = (((fSlow85 + (fSlow122 * fSlow83)) / fSlow62) + 1.0f);
		float fSlow126 = (synth_piano_faustpower2_f(fSlow121) * fSlow80);
		float fSlow127 = (fConst19 * (((fSlow123 + std::atan2((((0.0f - ((fSlow121 * fSlow83) / fSlow62)) * fSlow124) + (((fSlow121 * fSlow80) * fSlow125) / fSlow62)), ((fSlow124 * fSlow125) + ((fSlow126 * fSlow83) / fSlow88)))) + 6.28318548f) / fSlow119));
		float fSlow128 = std::floor(fSlow127);
		float fSlow129 = (fSlow128 + (1.0f - fSlow127));
		float fSlow130 = (((fSlow85 + (fSlow122 * fSlow95)) / fSlow62) + 1.0f);
		int iSlow131 = int((fConst19 * (((fSlow123 + std::atan2((0.0f - ((fSlow121 * ((fSlow95 * fSlow124) - (fSlow80 * fSlow130))) / fSlow62)), ((fSlow124 * fSlow130) + ((fSlow126 * fSlow95) / fSlow88)))) + 6.28318548f) / fSlow119)));
		int iSlow132 = std::min<int>(4097, std::max<int>(0, iSlow131));
		float fSlow133 = (fSlow127 - fSlow128);
		int iSlow134 = std::min<int>(4097, std::max<int>(0, (iSlow131 + 1)));
		float fSlow135 = (440.0f * fSlow38);
		float fSlow136 = (fSlow135 + fSlow102);
		float fSlow137 = (fConst20 * fSlow136);
		float fSlow138 = std::sin(fSlow137);
		float fSlow139 = std::cos(fSlow137);
		float fSlow140 = (3.0f * std::atan2((fSlow68 * fSlow138), (fSlow73 + (fSlow74 * fSlow139))));
		float fSlow141 = (((fSlow139 * fSlow80) / fSlow62) + 1.0f);
		float fSlow142 = (((fSlow85 + (fSlow139 * fSlow83)) / fSlow62) + 1.0f);
		float fSlow143 = (synth_piano_faustpower2_f(fSlow138) * fSlow80);
		float fSlow144 = (fConst19 * (((fSlow140 + std::atan2((((0.0f - ((fSlow138 * fSlow83) / fSlow62)) * fSlow141) + (((fSlow138 * fSlow80) * fSlow142) / fSlow62)), ((fSlow141 * fSlow142) + ((fSlow143 * fSlow83) / fSlow88)))) + 6.28318548f) / fSlow136));
		float fSlow145 = std::floor(fSlow144);
		float fSlow146 = (fSlow144 - fSlow145);
		float fSlow147 = (1.62981451e-09f * (fSlow19 * float((fSlow13 < 45.0f))));
		float fSlow148 = (((fSlow85 + (fSlow139 * fSlow95)) / fSlow62) + 1.0f);
		int iSlow149 = int((fConst19 * (((fSlow140 + std::atan2((0.0f - ((fSlow138 * ((fSlow95 * fSlow141) - (fSlow80 * fSlow148))) / fSlow62)), ((fSlow141 * fSlow148) + ((fSlow143 * fSlow95) / fSlow88)))) + 6.28318548f) / fSlow136)));
		int iSlow150 = std::min<int>(4097, std::max<int>(0, (iSlow149 + 1)));
		float fSlow151 = (fSlow135 - fSlow102);
		float fSlow152 = (fConst20 * fSlow151);
		float fSlow153 = std::sin(fSlow152);
		float fSlow154 = std::cos(fSlow152);
		float fSlow155 = (3.0f * std::atan2((fSlow68 * fSlow153), (fSlow73 + (fSlow74 * fSlow154))));
		float fSlow156 = (((fSlow154 * fSlow80) / fSlow62) + 1.0f);
		float fSlow157 = ((((fSlow154 * fSlow83) + fSlow85) / fSlow62) + 1.0f);
		float fSlow158 = (synth_piano_faustpower2_f(fSlow153) * fSlow80);
		float fSlow159 = (fConst19 * (((fSlow155 + std::atan2((((0.0f - ((fSlow153 * fSlow83) / fSlow62)) * fSlow156) + (((fSlow153 * fSlow80) * fSlow157) / fSlow62)), ((fSlow156 * fSlow157) + ((fSlow158 * fSlow83) / fSlow88)))) + 6.28318548f) / fSlow151));
		float fSlow160 = std::floor(fSlow159);
		float fSlow161 = (fSlow160 + (1.0f - fSlow159));
		float fSlow162 = (((fSlow85 + (fSlow154 * fSlow95)) / fSlow62) + 1.0f);
		int iSlow163 = int((fConst19 * (((fSlow155 + std::atan2((0.0f - ((fSlow153 * ((fSlow156 * fSlow95) - (fSlow80 * fSlow162))) / fSlow62)), ((fSlow156 * fSlow162) + ((fSlow158 * fSlow95) / fSlow88)))) + 6.28318548f) / fSlow151)));
		int iSlow164 = std::min<int>(4097, std::max<int>(0, iSlow163));
		float fSlow165 = (fSlow159 - fSlow160);
		int iSlow166 = std::min<int>(4097, std::max<int>(0, (iSlow163 + 1)));
		float fSlow167 = (fSlow145 + (1.0f - fSlow144));
		int iSlow168 = std::min<int>(4097, std::max<int>(0, iSlow149));
		float fSlow169 = float(getValueEQBandWidthFactor(float(fSlow13)));
		float fSlow170 = (synth_piano_faustpower2_f(fSlow38) * synth_piano_faustpower2_f(fSlow169));
		float fSlow171 = (0.5f - (fConst22 * fSlow170));
		float fSlow172 = float(getValueEQGain(float(fSlow13)));
		float fSlow173 = ((0.0f - (fConst23 * (fSlow38 * fSlow169))) * std::cos((fConst16 * (fSlow38 / float(getValueStrikePosition(float(fSlow13)))))));
		float fSlow174 = (fConst24 * fSlow170);
		int iSlow175 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst25 * (float(fHslider8) / fSlow38)))));
		float fSlow176 = std::exp((fConst30 / fSlow0));
		float fSlow177 = synth_piano_faustpower2_f(fSlow176);
		float fSlow178 = (1.0f - (fConst1 * fSlow177));
		float fSlow179 = (1.0f - fSlow177);
		float fSlow180 = (fSlow178 / fSlow179);
		float fSlow181 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow178) / synth_piano_faustpower2_f(fSlow179)) + -1.0f)));
		float fSlow182 = (fSlow180 - fSlow181);
		float fSlow183 = (fSlow176 * (fSlow181 + (1.0f - fSlow180)));
		float fSlow184 = ((std::exp((fConst31 / fSlow0)) / fSlow176) + -1.0f);
		float fSlow185 = std::exp((fConst37 / fSlow0));
		float fSlow186 = synth_piano_faustpower2_f(fSlow185);
		float fSlow187 = (1.0f - (fConst1 * fSlow186));
		float fSlow188 = (1.0f - fSlow186);
		float fSlow189 = (fSlow187 / fSlow188);
		float fSlow190 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow187) / synth_piano_faustpower2_f(fSlow188)) + -1.0f)));
		float fSlow191 = (fSlow189 - fSlow190);
		float fSlow192 = (fSlow185 * (fSlow190 + (1.0f - fSlow189)));
		float fSlow193 = ((std::exp((fConst38 / fSlow0)) / fSlow185) + -1.0f);
		float fSlow194 = std::exp((fConst44 / fSlow0));
		float fSlow195 = synth_piano_faustpower2_f(fSlow194);
		float fSlow196 = (1.0f - (fConst1 * fSlow195));
		float fSlow197 = (1.0f - fSlow195);
		float fSlow198 = (fSlow196 / fSlow197);
		float fSlow199 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow196) / synth_piano_faustpower2_f(fSlow197)) + -1.0f)));
		float fSlow200 = (fSlow198 - fSlow199);
		float fSlow201 = (fSlow194 * (fSlow199 + (1.0f - fSlow198)));
		float fSlow202 = ((std::exp((fConst45 / fSlow0)) / fSlow194) + -1.0f);
		float fSlow203 = std::exp((fConst51 / fSlow0));
		float fSlow204 = synth_piano_faustpower2_f(fSlow203);
		float fSlow205 = (1.0f - (fConst1 * fSlow204));
		float fSlow206 = (1.0f - fSlow204);
		float fSlow207 = (fSlow205 / fSlow206);
		float fSlow208 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow205) / synth_piano_faustpower2_f(fSlow206)) + -1.0f)));
		float fSlow209 = (fSlow207 - fSlow208);
		float fSlow210 = (fSlow203 * (fSlow208 + (1.0f - fSlow207)));
		float fSlow211 = ((std::exp((fConst52 / fSlow0)) / fSlow203) + -1.0f);
		float fSlow212 = (12.0f * (1.0f - fSlow10));
		float fSlow213 = std::exp((fConst58 / fSlow0));
		float fSlow214 = synth_piano_faustpower2_f(fSlow213);
		float fSlow215 = (1.0f - (fConst1 * fSlow214));
		float fSlow216 = (1.0f - fSlow214);
		float fSlow217 = (fSlow215 / fSlow216);
		float fSlow218 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow215) / synth_piano_faustpower2_f(fSlow216)) + -1.0f)));
		float fSlow219 = (fSlow217 - fSlow218);
		float fSlow220 = (fSlow213 * (fSlow218 + (1.0f - fSlow217)));
		float fSlow221 = ((std::exp((fConst59 / fSlow0)) / fSlow213) + -1.0f);
		float fSlow222 = std::exp((fConst65 / fSlow0));
		float fSlow223 = synth_piano_faustpower2_f(fSlow222);
		float fSlow224 = (1.0f - (fConst1 * fSlow223));
		float fSlow225 = (1.0f - fSlow223);
		float fSlow226 = (fSlow224 / fSlow225);
		float fSlow227 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow224) / synth_piano_faustpower2_f(fSlow225)) + -1.0f)));
		float fSlow228 = (fSlow226 - fSlow227);
		float fSlow229 = (fSlow222 * (fSlow227 + (1.0f - fSlow226)));
		float fSlow230 = ((std::exp((fConst66 / fSlow0)) / fSlow222) + -1.0f);
		float fSlow231 = std::exp((fConst72 / fSlow0));
		float fSlow232 = synth_piano_faustpower2_f(fSlow231);
		float fSlow233 = (1.0f - (fConst1 * fSlow232));
		float fSlow234 = (1.0f - fSlow232);
		float fSlow235 = (fSlow233 / fSlow234);
		float fSlow236 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow233) / synth_piano_faustpower2_f(fSlow234)) + -1.0f)));
		float fSlow237 = (fSlow235 - fSlow236);
		float fSlow238 = (fSlow231 * (fSlow236 + (1.0f - fSlow235)));
		float fSlow239 = ((std::exp((fConst73 / fSlow0)) / fSlow231) + -1.0f);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec11[0] = (0.0f - (fConst7 * ((fConst8 * fRec11[1]) - (fRec7[1] + fRec7[2]))));
			fRec10[0] = ((fSlow7 * fRec10[1]) + (fSlow8 * (fRec7[1] + (fSlow9 * fRec11[0]))));
			fVec0[(IOTA & 32767)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			fRec12[0] = (fSlow12 + (0.999000013f * fRec12[1]));
			iRec23[0] = ((1103515245 * iRec23[1]) + 12345);
			iRec25[0] = ((iSlow24 * iRec25[1]) + 1);
			float fTemp0 = float((iRec25[0] + -1));
			float fTemp1 = float(((fTemp0 < 2.0f) & iSlow25));
			float fTemp2 = ((0.0301973838f * fTemp1) + (fSlow26 * float(((fTemp0 >= 2.0f) | iSlow27))));
			fRec24[0] = ((fRec24[1] * fTemp2) + (0.150000006f * (fTemp1 * (1.0f - fTemp2))));
			int iTemp3 = (fTemp0 < fSlow33);
			float fTemp4 = ((fSlow32 * float(iTemp3)) + (fConst15 * float((iSlow24 * (fTemp0 >= fSlow33)))));
			fRec26[0] = ((fRec26[1] * (fSlow29 + fTemp4)) + (fSlow34 * (((1.0f - fTemp4) - fSlow29) * float((iTemp3 & iSlow25)))));
			float fTemp5 = (float(iRec23[0]) * (fRec24[0] + fRec26[0]));
			fVec1[0] = (fSlow22 * fTemp5);
			float fTemp6 = (0.0f - ((0.5f * fVec1[1]) + (fSlow35 * fTemp5)));
			fVec2[0] = fTemp6;
			fRec22[0] = (((fSlow20 * fTemp6) + (fSlow36 * fVec2[1])) - (fSlow14 * fRec22[1]));
			fRec21[0] = ((fSlow19 * fRec22[0]) + (fSlow37 * fRec21[1]));
			fRec20[0] = ((fSlow19 * fRec21[0]) + (fSlow37 * fRec20[1]));
			fRec19[0] = ((fSlow19 * fRec20[0]) + (fSlow37 * fRec19[1]));
			fRec18[0] = ((fSlow19 * fRec19[0]) + (fSlow37 * fRec18[1]));
			fRec17[0] = ((fSlow16 * (0.0f - (0.5f * (fRec18[1] - fRec18[0])))) - ((fSlow40 * fRec17[1]) + (fSlow41 * fRec17[2])));
			fRec16[0] = ((fSlow16 * fRec17[0]) - ((fSlow43 * fRec16[1]) + (fSlow44 * fRec16[2])));
			fRec15[0] = (fRec16[0] - ((fSlow47 * fRec15[1]) + (fSlow48 * fRec15[2])));
			fRec14[0] = ((fRec15[0] + ((fSlow53 * fRec15[2]) + (fSlow45 * ((fSlow54 * fRec15[1]) - (fSlow55 * fRec14[1]))))) - (fSlow51 * fRec14[2]));
			fRec13[0] = ((fSlow15 * fRec14[0]) - (fSlow14 * fRec13[1]));
			fRec39[0] = ((fSlow92 * fTemp5) + (fSlow37 * fRec39[1]));
			fRec38[0] = ((fSlow19 * fRec39[0]) + (fSlow37 * fRec38[1]));
			fRec37[0] = ((fSlow19 * fRec38[0]) + (fSlow37 * fRec37[1]));
			fRec36[0] = ((fSlow19 * fRec37[0]) + (fSlow37 * fRec36[1]));
			fRec35[0] = (((fSlow20 * fRec36[0]) + (fSlow36 * fRec36[1])) - (fSlow14 * fRec35[1]));
			fRec40[0] = (fSlow93 + (0.999000013f * fRec40[1]));
			float fTemp7 = (fRec35[0] + (fRec40[0] * fRec28[1]));
			fVec3[0] = fTemp7;
			fRec34[0] = (fVec3[1] + (fSlow94 * (fTemp7 - fRec34[1])));
			fRec33[0] = (fRec34[1] + (fSlow94 * (fRec34[0] - fRec33[1])));
			fRec32[(IOTA & 8191)] = (fRec33[1] + (fSlow94 * (fRec33[0] - fRec32[((IOTA - 1) & 8191)])));
			float fTemp8 = (fSlow91 * fRec32[((IOTA - iSlow98) & 8191)]);
			float fTemp9 = (fSlow99 * fRec32[((IOTA - iSlow100) & 8191)]);
			float fTemp10 = (fRec35[0] + (fRec40[0] * fRec29[1]));
			fVec4[0] = fTemp10;
			fRec43[0] = (fVec4[1] + (fSlow94 * (fTemp10 - fRec43[1])));
			fRec42[0] = (fRec43[1] + (fSlow94 * (fRec43[0] - fRec42[1])));
			fRec41[(IOTA & 8191)] = (fRec42[1] + (fSlow94 * (fRec42[0] - fRec41[((IOTA - 1) & 8191)])));
			float fTemp11 = (fSlow113 * fRec41[((IOTA - iSlow116) & 8191)]);
			float fTemp12 = (fSlow117 * fRec41[((IOTA - iSlow118) & 8191)]);
			float fTemp13 = (fRec40[0] * (fRec35[0] + fRec27[1]));
			fVec5[0] = fTemp13;
			fRec46[0] = (fVec5[1] + (fSlow94 * (fTemp13 - fRec46[1])));
			fRec45[0] = (fRec46[1] + (fSlow94 * (fRec46[0] - fRec45[1])));
			fRec44[(IOTA & 8191)] = (fRec45[1] + (fSlow94 * (fRec45[0] - fRec44[((IOTA - 1) & 8191)])));
			float fTemp14 = ((fSlow129 * fRec44[((IOTA - iSlow132) & 8191)]) + (fSlow133 * fRec44[((IOTA - iSlow134) & 8191)]));
			float fTemp15 = (fTemp8 + (fTemp9 + (fTemp11 + (fTemp12 + fTemp14))));
			fVec6[0] = fTemp15;
			fRec31[0] = (fSlow63 * ((2.0f * ((fSlow64 * fTemp15) + (fSlow81 * fVec6[1]))) - (fSlow80 * fRec31[1])));
			fRec27[0] = (fRec31[0] + fTemp14);
			fRec28[0] = (fTemp8 + (fRec31[0] + fTemp9));
			fRec29[0] = (fRec31[0] + (fTemp12 + fTemp11));
			float fRec30 = fTemp15;
			fRec57[0] = ((fSlow147 * fTemp5) + (fSlow37 * fRec57[1]));
			fRec56[0] = ((fSlow19 * fRec57[0]) + (fSlow37 * fRec56[1]));
			fRec55[0] = ((fSlow19 * fRec56[0]) + (fSlow37 * fRec55[1]));
			fRec54[0] = ((fSlow19 * fRec55[0]) + (fSlow37 * fRec54[1]));
			fRec53[0] = (((fSlow20 * fRec54[0]) + (fSlow36 * fRec54[1])) - (fSlow14 * fRec53[1]));
			float fTemp16 = (fRec40[0] * (fRec53[0] + fRec47[1]));
			fVec7[0] = fTemp16;
			fRec52[0] = (fVec7[1] + (fSlow94 * (fTemp16 - fRec52[1])));
			fRec51[0] = (fRec52[1] + (fSlow94 * (fRec52[0] - fRec51[1])));
			fRec50[(IOTA & 8191)] = (fRec51[1] + (fSlow94 * (fRec51[0] - fRec50[((IOTA - 1) & 8191)])));
			float fTemp17 = (fSlow146 * fRec50[((IOTA - iSlow150) & 8191)]);
			float fTemp18 = (fRec53[0] + (fRec40[0] * fRec48[1]));
			fVec8[0] = fTemp18;
			fRec61[0] = (fVec8[1] - (fSlow94 * (fRec61[1] - fTemp18)));
			fRec60[0] = (fRec61[1] - (fSlow94 * (fRec60[1] - fRec61[0])));
			fRec59[(IOTA & 8191)] = (fRec60[1] + (fSlow94 * (fRec60[0] - fRec59[((IOTA - 1) & 8191)])));
			float fTemp19 = (fSlow161 * fRec59[((IOTA - iSlow164) & 8191)]);
			float fTemp20 = (fSlow165 * fRec59[((IOTA - iSlow166) & 8191)]);
			float fTemp21 = (fSlow167 * fRec50[((IOTA - iSlow168) & 8191)]);
			float fTemp22 = (fTemp17 + ((fTemp19 + fTemp20) + fTemp21));
			fVec9[0] = fTemp22;
			fRec58[0] = (fSlow63 * ((2.0f * ((fSlow64 * fTemp22) + (fSlow81 * fVec9[1]))) - (fSlow80 * fRec58[1])));
			fRec47[0] = (fTemp17 + (fRec58[0] + fTemp21));
			fRec48[0] = (fTemp20 + (fRec58[0] + fTemp19));
			float fRec49 = fTemp22;
			fRec62[0] = ((fSlow172 * fRec49) - ((fSlow173 * fRec62[1]) + (fSlow174 * fRec62[2])));
			fRec63[0] = ((fSlow172 * fRec30) - ((fSlow173 * fRec63[1]) + (fSlow174 * fRec63[2])));
			float fTemp23 = (fRec13[0] + (fRec30 + (fRec49 + (fSlow171 * ((fRec62[0] - fRec62[2]) + (fRec63[0] - fRec63[2]))))));
			fVec10[(IOTA & 4095)] = fTemp23;
			float fTemp24 = fVec10[((IOTA - iSlow175) & 4095)];
			fVec11[(IOTA & 4095)] = (fSlow11 * (fRec12[0] * fTemp24));
			float fTemp25 = (0.300000012f * fVec11[((IOTA - iConst26) & 4095)]);
			float fTemp26 = (((0.600000024f * fRec8[1]) + fVec0[((IOTA - iConst10) & 32767)]) - fTemp25);
			fVec12[(IOTA & 2047)] = fTemp26;
			fRec8[0] = fVec12[((IOTA - iConst27) & 2047)];
			float fRec9 = (0.0f - (0.600000024f * fTemp26));
			fRec67[0] = (0.0f - (fConst7 * ((fConst8 * fRec67[1]) - (fRec5[1] + fRec5[2]))));
			fRec66[0] = ((fSlow182 * fRec66[1]) + (fSlow183 * (fRec5[1] + (fSlow184 * fRec67[0]))));
			fVec13[(IOTA & 16383)] = ((0.353553385f * fRec66[0]) + 9.99999968e-21f);
			float fTemp27 = ((0.600000024f * fRec64[1]) + (fTemp25 + fVec13[((IOTA - iConst33) & 16383)]));
			fVec14[(IOTA & 4095)] = fTemp27;
			fRec64[0] = fVec14[((IOTA - iConst34) & 4095)];
			float fRec65 = (0.0f - (0.600000024f * fTemp27));
			fRec71[0] = (0.0f - (fConst7 * ((fConst8 * fRec71[1]) - (fRec3[1] + fRec3[2]))));
			fRec70[0] = ((fSlow191 * fRec70[1]) + (fSlow192 * (fRec3[1] + (fSlow193 * fRec71[0]))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec70[0]) + 9.99999968e-21f);
			float fTemp28 = (((0.600000024f * fRec68[1]) + fVec15[((IOTA - iConst40) & 32767)]) - fTemp25);
			fVec16[(IOTA & 4095)] = fTemp28;
			fRec68[0] = fVec16[((IOTA - iConst41) & 4095)];
			float fRec69 = (0.0f - (0.600000024f * fTemp28));
			fRec75[0] = (0.0f - (fConst7 * ((fConst8 * fRec75[1]) - (fRec1[1] + fRec1[2]))));
			fRec74[0] = ((fSlow200 * fRec74[1]) + (fSlow201 * (fRec1[1] + (fSlow202 * fRec75[0]))));
			fVec17[(IOTA & 32767)] = ((0.353553385f * fRec74[0]) + 9.99999968e-21f);
			float fTemp29 = (fTemp25 + ((0.600000024f * fRec72[1]) + fVec17[((IOTA - iConst47) & 32767)]));
			fVec18[(IOTA & 4095)] = fTemp29;
			fRec72[0] = fVec18[((IOTA - iConst48) & 4095)];
			float fRec73 = (0.0f - (0.600000024f * fTemp29));
			fRec79[0] = (0.0f - (fConst7 * ((fConst8 * fRec79[1]) - (fRec6[1] + fRec6[2]))));
			fRec78[0] = ((fSlow209 * fRec78[1]) + (fSlow210 * (fRec6[1] + (fSlow211 * fRec79[0]))));
			fVec19[(IOTA & 16383)] = ((0.353553385f * fRec78[0]) + 9.99999968e-21f);
			fVec20[(IOTA & 4095)] = (fSlow212 * (fRec12[0] * fTemp23));
			float fTemp30 = (0.300000012f * fVec20[((IOTA - iConst26) & 4095)]);
			float fTemp31 = (fVec19[((IOTA - iConst54) & 16383)] - (fTemp30 + (0.600000024f * fRec76[1])));
			fVec21[(IOTA & 2047)] = fTemp31;
			fRec76[0] = fVec21[((IOTA - iConst55) & 2047)];
			float fRec77 = (0.600000024f * fTemp31);
			fRec83[0] = (0.0f - (fConst7 * ((fConst8 * fRec83[1]) - (fRec2[1] + fRec2[2]))));
			fRec82[0] = ((fSlow219 * fRec82[1]) + (fSlow220 * (fRec2[1] + (fSlow221 * fRec83[0]))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec82[0]) + 9.99999968e-21f);
			float fTemp32 = (fVec22[((IOTA - iConst61) & 16383)] - (fTemp30 + (0.600000024f * fRec80[1])));
			fVec23[(IOTA & 4095)] = fTemp32;
			fRec80[0] = fVec23[((IOTA - iConst62) & 4095)];
			float fRec81 = (0.600000024f * fTemp32);
			fRec87[0] = (0.0f - (fConst7 * ((fConst8 * fRec87[1]) - (fRec4[1] + fRec4[2]))));
			fRec86[0] = ((fSlow228 * fRec86[1]) + (fSlow229 * (fRec4[1] + (fSlow230 * fRec87[0]))));
			fVec24[(IOTA & 16383)] = ((0.353553385f * fRec86[0]) + 9.99999968e-21f);
			float fTemp33 = ((fTemp30 + fVec24[((IOTA - iConst68) & 16383)]) - (0.600000024f * fRec84[1]));
			fVec25[(IOTA & 4095)] = fTemp33;
			fRec84[0] = fVec25[((IOTA - iConst69) & 4095)];
			float fRec85 = (0.600000024f * fTemp33);
			fRec91[0] = (0.0f - (fConst7 * ((fConst8 * fRec91[1]) - (fRec0[1] + fRec0[2]))));
			fRec90[0] = ((fSlow237 * fRec90[1]) + (fSlow238 * (fRec0[1] + (fSlow239 * fRec91[0]))));
			fVec26[(IOTA & 16383)] = ((0.353553385f * fRec90[0]) + 9.99999968e-21f);
			float fTemp34 = ((fVec26[((IOTA - iConst75) & 16383)] + fTemp30) - (0.600000024f * fRec88[1]));
			fVec27[(IOTA & 2047)] = fTemp34;
			fRec88[0] = fVec27[((IOTA - iConst76) & 2047)];
			float fRec89 = (0.600000024f * fTemp34);
			float fTemp35 = (fRec84[1] + fRec80[1]);
			float fTemp36 = (fRec88[1] + (fRec76[1] + fTemp35));
			fRec0[0] = (fRec8[1] + (fRec64[1] + (fRec9 + (fRec69 + (fRec65 + (fRec73 + (fRec77 + (fRec81 + (fRec85 + (fRec89 + (fRec68[1] + (fTemp36 + fRec72[1]))))))))))));
			fRec1[0] = ((fRec77 + (fRec81 + (fRec85 + (fRec89 + fTemp36)))) - (fRec8[1] + (fRec64[1] + (fRec9 + (fRec69 + (fRec65 + (fRec73 + (fRec72[1] + fRec68[1]))))))));
			float fTemp37 = (fRec88[1] + fRec84[1]);
			float fTemp38 = (fRec80[1] + fRec76[1]);
			fRec2[0] = ((fRec64[1] + (fRec65 + (fRec73 + (fRec85 + (fRec89 + (fTemp37 + fRec72[1])))))) - (fRec8[1] + (fRec9 + (fRec69 + (fRec77 + (fRec81 + (fTemp38 + fRec68[1])))))));
			fRec3[0] = ((fRec8[1] + (fRec9 + (fRec69 + (fRec85 + (fRec89 + (fTemp37 + fRec68[1])))))) - (fRec64[1] + (fRec65 + (fRec73 + (fRec77 + (fRec81 + (fTemp38 + fRec72[1])))))));
			float fTemp39 = (fRec88[1] + fRec80[1]);
			float fTemp40 = (fRec84[1] + fRec76[1]);
			fRec4[0] = ((fRec69 + (fRec73 + (fRec81 + (fRec89 + (fRec68[1] + (fRec72[1] + fTemp39)))))) - (fRec8[1] + (fRec64[1] + (fRec9 + (fRec65 + (fRec77 + (fRec85 + fTemp40)))))));
			fRec5[0] = ((fRec8[1] + (fRec64[1] + (fRec9 + (fRec65 + (fRec81 + (fRec89 + fTemp39)))))) - (fRec69 + (fRec73 + (fRec77 + (fRec85 + (fRec68[1] + (fRec72[1] + fTemp40)))))));
			float fTemp41 = (fRec88[1] + fRec76[1]);
			fRec6[0] = ((fRec8[1] + (fRec9 + (fRec73 + (fRec77 + (fRec89 + (fRec72[1] + fTemp41)))))) - (fRec64[1] + (fRec69 + (fRec65 + (fRec81 + (fRec85 + (fTemp35 + fRec68[1])))))));
			fRec7[0] = ((fRec64[1] + (fRec69 + (fRec65 + (fRec77 + (fRec89 + (fRec68[1] + fTemp41)))))) - (fRec8[1] + (fRec9 + (fRec73 + (fRec81 + (fRec85 + (fTemp35 + fRec72[1])))))));
			float fTemp42 = (1.0f - fRec12[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow212 * (fTemp42 * fTemp23))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow11 * (fTemp42 * fTemp24))));
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA = (IOTA + 1);
			fRec12[1] = fRec12[0];
			iRec23[1] = iRec23[0];
			iRec25[1] = iRec25[0];
			fRec24[1] = fRec24[0];
			fRec26[1] = fRec26[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec40[1] = fRec40[0];
			fVec3[1] = fVec3[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fVec4[1] = fVec4[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fVec5[1] = fVec5[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fVec6[1] = fVec6[0];
			fRec31[1] = fRec31[0];
			fRec27[1] = fRec27[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fVec7[1] = fVec7[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fVec8[1] = fVec8[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fVec9[1] = fVec9[0];
			fRec58[1] = fRec58[0];
			fRec47[1] = fRec47[0];
			fRec48[1] = fRec48[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec8[1] = fRec8[0];
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			fRec64[1] = fRec64[0];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec68[1] = fRec68[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec72[1] = fRec72[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec76[1] = fRec76[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec80[1] = fRec80[0];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec84[1] = fRec84[0];
			fRec91[1] = fRec91[0];
			fRec90[1] = fRec90[0];
			fRec88[1] = fRec88[0];
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
    struct _synth_piano_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_piano_UI<T>::name(sym(synth_piano));

typedef _synth_piano_UI<synth_piano> synth_piano_UI;

class faust_synth_piano_tilde : public FaustExternal<synth_piano, synth_piano_UI> {
public:
    faust_synth_piano_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
