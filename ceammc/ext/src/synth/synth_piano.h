/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.piano"
version: "1.0"
Code generated with Faust 2.28.6 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
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
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	FAUSTFLOAT fHslider0;
	float fRec11[2];
	float fConst5;
	float fConst6;
	float fConst7;
	float fRec12[2];
	float fConst8;
	float fRec10[2];
	int IOTA;
	float fVec0[32768];
	float fConst9;
	int iConst10;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fRec13[2];
	float fConst11;
	float fConst12;
	FAUSTFLOAT fHslider3;
	float fConst13;
	FAUSTFLOAT fHslider4;
	float fConst14;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fButton0;
	float fRec21[2];
	FAUSTFLOAT fHslider6;
	int iRec27[2];
	int iRec29[2];
	float fConst15;
	FAUSTFLOAT fEntry0;
	float fRec28[2];
	float fConst16;
	float fConst17;
	FAUSTFLOAT fHslider7;
	float fConst18;
	float fConst19;
	float fRec30[2];
	float fRec26[2];
	float fRec25[2];
	float fRec24[2];
	float fRec23[2];
	float fRec22[2];
	float fVec1[2];
	float fRec20[2];
	float fRec19[2];
	float fRec18[8192];
	float fVec2[2];
	float fRec34[2];
	float fRec33[2];
	float fRec32[8192];
	float fVec3[2];
	float fRec31[2];
	float fRec15[2];
	float fRec16[2];
	float fConst20;
	float fConst21;
	float fConst22;
	float fRec14[3];
	float fConst23;
	float fVec4[2];
	float fVec5[2];
	float fRec44[2];
	float fRec43[2];
	float fRec42[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[3];
	float fRec38[3];
	float fRec37[3];
	float fRec36[3];
	float fRec35[2];
	float fVec6[4096];
	float fConst24;
	FAUSTFLOAT fHslider8;
	float fVec7[4096];
	int iConst25;
	float fVec8[2048];
	int iConst26;
	float fRec8[2];
	float fConst27;
	float fConst28;
	float fConst29;
	float fRec48[2];
	float fConst30;
	float fRec47[2];
	float fVec9[32768];
	float fConst31;
	int iConst32;
	float fVec10[4096];
	int iConst33;
	float fRec45[2];
	float fConst34;
	float fConst35;
	float fConst36;
	float fRec52[2];
	float fConst37;
	float fRec51[2];
	float fVec11[16384];
	float fConst38;
	int iConst39;
	float fVec12[4096];
	int iConst40;
	float fRec49[2];
	float fConst41;
	float fConst42;
	float fConst43;
	float fRec56[2];
	float fConst44;
	float fRec55[2];
	float fVec13[32768];
	float fConst45;
	int iConst46;
	float fVec14[4096];
	int iConst47;
	float fRec53[2];
	float fConst48;
	float fConst49;
	float fConst50;
	float fRec60[2];
	float fConst51;
	float fRec59[2];
	float fVec15[16384];
	float fConst52;
	int iConst53;
	float fVec16[4096];
	float fVec17[2048];
	int iConst54;
	float fRec57[2];
	float fConst55;
	float fConst56;
	float fConst57;
	float fRec64[2];
	float fConst58;
	float fRec63[2];
	float fVec18[16384];
	float fConst59;
	int iConst60;
	float fVec19[4096];
	int iConst61;
	float fRec61[2];
	float fConst62;
	float fConst63;
	float fConst64;
	float fRec68[2];
	float fConst65;
	float fRec67[2];
	float fVec20[16384];
	float fConst66;
	int iConst67;
	float fVec21[4096];
	int iConst68;
	float fRec65[2];
	float fConst69;
	float fConst70;
	float fConst71;
	float fRec72[2];
	float fConst72;
	float fRec71[2];
	float fVec22[16384];
	float fConst73;
	int iConst74;
	float fVec23[2048];
	int iConst75;
	float fRec69[2];
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
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.1");
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
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::cos((37699.1133f / fConst0));
		fConst2 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst3 = ((0.0f - (6.90775537f * fConst2)) / fConst0);
		fConst4 = (0.5f * fConst3);
		fConst5 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst6 = (1.0f / (fConst5 + 1.0f));
		fConst7 = (1.0f - fConst5);
		fConst8 = (0.333333343f * fConst3);
		fConst9 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst10 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst2 - fConst9))));
		fConst11 = synth_piano_faustpower2_f(fConst0);
		fConst12 = (96800.0f / fConst11);
		fConst13 = (0.159154937f * fConst0);
		fConst14 = (6.28318548f / fConst0);
		fConst15 = (7.0f / fConst0);
		fConst16 = std::exp((0.0f - (5.0f / fConst0)));
		fConst17 = (10.0f / fConst0);
		fConst18 = (0.100000001f * fConst0);
		fConst19 = std::exp((0.0f - (0.5f / fConst0)));
		fConst20 = (880.0f / fConst0);
		fConst21 = (2764.60156f / fConst0);
		fConst22 = (193600.0f / fConst11);
		fConst23 = (0.0500000007f / fConst0);
		fConst24 = (0.0011363636f * fConst0);
		iConst25 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		iConst26 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst9 + -1.0f))));
		fConst27 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst28 = ((0.0f - (6.90775537f * fConst27)) / fConst0);
		fConst29 = (0.5f * fConst28);
		fConst30 = (0.333333343f * fConst28);
		fConst31 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst32 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst27 - fConst31))));
		iConst33 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst31 + -1.0f))));
		fConst34 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst35 = ((0.0f - (6.90775537f * fConst34)) / fConst0);
		fConst36 = (0.5f * fConst35);
		fConst37 = (0.333333343f * fConst35);
		fConst38 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst39 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst34 - fConst38))));
		iConst40 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst38 + -1.0f))));
		fConst41 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst42 = ((0.0f - (6.90775537f * fConst41)) / fConst0);
		fConst43 = (0.5f * fConst42);
		fConst44 = (0.333333343f * fConst42);
		fConst45 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst46 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst41 - fConst45))));
		iConst47 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst45 + -1.0f))));
		fConst48 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst49 = ((0.0f - (6.90775537f * fConst48)) / fConst0);
		fConst50 = (0.5f * fConst49);
		fConst51 = (0.333333343f * fConst49);
		fConst52 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst53 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst48 - fConst52))));
		iConst54 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst52 + -1.0f))));
		fConst55 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst56 = ((0.0f - (6.90775537f * fConst55)) / fConst0);
		fConst57 = (0.5f * fConst56);
		fConst58 = (0.333333343f * fConst56);
		fConst59 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst60 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst55 - fConst59))));
		iConst61 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst59 + -1.0f))));
		fConst62 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst63 = ((0.0f - (6.90775537f * fConst62)) / fConst0);
		fConst64 = (0.5f * fConst63);
		fConst65 = (0.333333343f * fConst63);
		fConst66 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst67 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst62 - fConst66))));
		iConst68 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst66 + -1.0f))));
		fConst69 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst70 = ((0.0f - (6.90775537f * fConst69)) / fConst0);
		fConst71 = (0.5f * fConst70);
		fConst72 = (0.333333343f * fConst70);
		fConst73 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst74 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst69 - fConst73))));
		iConst75 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst73 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.35999999999999999f);
		fHslider1 = FAUSTFLOAT(0.59999999999999998f);
		fHslider2 = FAUSTFLOAT(0.13700000000000001f);
		fHslider3 = FAUSTFLOAT(48.0f);
		fHslider4 = FAUSTFLOAT(0.28000000000000003f);
		fHslider5 = FAUSTFLOAT(0.10000000000000001f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fEntry0 = FAUSTFLOAT(1.0f);
		fHslider7 = FAUSTFLOAT(0.10000000000000001f);
		fHslider8 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec11[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec12[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec10[l2] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 32768); l3 = (l3 + 1)) {
			fVec0[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec13[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec21[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec27[l6] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec29[l7] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec28[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec30[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec26[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec25[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec24[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec23[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec22[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fVec1[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec20[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec19[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 8192); l18 = (l18 + 1)) {
			fRec18[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fVec2[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec34[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec33[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 8192); l22 = (l22 + 1)) {
			fRec32[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fVec3[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec31[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec15[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec16[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec14[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fVec4[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fVec5[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec44[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec43[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec42[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec41[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec40[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec39[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 3); l36 = (l36 + 1)) {
			fRec38[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 3); l37 = (l37 + 1)) {
			fRec37[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 3); l38 = (l38 + 1)) {
			fRec36[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec35[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 4096); l40 = (l40 + 1)) {
			fVec6[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 4096); l41 = (l41 + 1)) {
			fVec7[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2048); l42 = (l42 + 1)) {
			fVec8[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec8[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec48[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec47[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 32768); l46 = (l46 + 1)) {
			fVec9[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 4096); l47 = (l47 + 1)) {
			fVec10[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec45[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec52[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec51[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 16384); l51 = (l51 + 1)) {
			fVec11[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 4096); l52 = (l52 + 1)) {
			fVec12[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec49[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec56[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec55[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 32768); l56 = (l56 + 1)) {
			fVec13[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 4096); l57 = (l57 + 1)) {
			fVec14[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec53[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec60[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec59[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 16384); l61 = (l61 + 1)) {
			fVec15[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 4096); l62 = (l62 + 1)) {
			fVec16[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 2048); l63 = (l63 + 1)) {
			fVec17[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec57[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec64[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec63[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 16384); l67 = (l67 + 1)) {
			fVec18[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 4096); l68 = (l68 + 1)) {
			fVec19[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec61[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec68[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec67[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 16384); l72 = (l72 + 1)) {
			fVec20[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 4096); l73 = (l73 + 1)) {
			fVec21[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec65[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec72[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec71[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 16384); l77 = (l77 + 1)) {
			fVec22[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 2048); l78 = (l78 + 1)) {
			fVec23[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec69[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 3); l80 = (l80 + 1)) {
			fRec0[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 3); l81 = (l81 + 1)) {
			fRec1[l81] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 3); l82 = (l82 + 1)) {
			fRec2[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec3[l83] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec4[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec5[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec6[l86] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l87 = 0; (l87 < 3); l87 = (l87 + 1)) {
			fRec7[l87] = 0.0f;
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
		ui_interface->openVerticalBox("Spat");
		ui_interface->addHorizontalSlider("pan angle", &fHslider1, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("spatial width", &fHslider8, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
		ui_interface->addHorizontalSlider("brightness", &fHslider6, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("detuning", &fHslider5, 0.100000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addNumEntry("gain", &fEntry0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider7, 0.100000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider3, 48.0f, 24.0f, 96.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider0, 0.360000014f, 0.00499999989f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("stiffness", &fHslider4, 0.280000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (0.00200000009f * float(fHslider0));
		float fSlow1 = float(fHslider1);
		float fSlow2 = (12.0f * fSlow1);
		float fSlow3 = (0.00100000005f * float(fHslider2));
		float fSlow4 = std::pow(2.0f, (0.0833333358f * (float(fHslider3) + -69.0f)));
		float fSlow5 = (440.0f * fSlow4);
		int iSlow6 = int(((17.3123398f * (std::log(fSlow5) + -6.08677483f)) + 69.5f));
		float fSlow7 = float(getValueEQBandWidthFactor(float(iSlow6)));
		float fSlow8 = (synth_piano_faustpower2_f(fSlow4) * synth_piano_faustpower2_f(fSlow7));
		float fSlow9 = (0.5f - (fConst12 * fSlow8));
		float fSlow10 = float(getValueEQGain(float(iSlow6)));
		float fSlow11 = float(fHslider4);
		float fSlow12 = float(getValueStiffnessCoefficient(float(iSlow6)));
		float fSlow13 = (13.6899996f * (synth_piano_faustpower2_f(fSlow11) * synth_piano_faustpower2_f(fSlow12)));
		float fSlow14 = (fSlow13 + -1.0f);
		float fSlow15 = (5.0f * (float(fHslider5) * float(getValueDetuningHz(float(iSlow6)))));
		float fSlow16 = (fSlow5 + fSlow15);
		float fSlow17 = (fConst14 * fSlow16);
		float fSlow18 = std::sin(fSlow17);
		float fSlow19 = (fSlow11 * fSlow12);
		float fSlow20 = (7.4000001f * fSlow19);
		float fSlow21 = (fSlow13 + 1.0f);
		float fSlow22 = std::cos(fSlow17);
		float fSlow23 = (3.0f * std::atan2((fSlow14 * fSlow18), (fSlow20 + (fSlow21 * fSlow22))));
		float fSlow24 = std::pow(10.0f, (0.000113636364f * (float(getValueSingleStringDecayRate(float(iSlow6))) / fSlow4)));
		float fSlow25 = float(getValueSingleStringZero(float(iSlow6)));
		float fSlow26 = float(getValueSingleStringPole(float(iSlow6)));
		float fSlow27 = (1.0f - fSlow26);
		float fSlow28 = ((fSlow24 * fSlow25) * fSlow27);
		float fSlow29 = (1.0f - fSlow25);
		float fSlow30 = (fSlow26 * fSlow29);
		float fSlow31 = (3.0f * fSlow30);
		float fSlow32 = (fSlow28 - fSlow31);
		float fSlow33 = (fSlow30 - fSlow28);
		float fSlow34 = (4.0f * fSlow33);
		float fSlow35 = (fSlow32 + fSlow34);
		float fSlow36 = (fSlow24 * fSlow27);
		float fSlow37 = ((3.0f * fSlow29) - fSlow36);
		float fSlow38 = (((fSlow22 * fSlow32) / fSlow37) + 1.0f);
		float fSlow39 = ((fSlow25 + fSlow36) + -1.0f);
		float fSlow40 = (4.0f * fSlow39);
		float fSlow41 = (((fSlow40 + (fSlow22 * fSlow35)) / fSlow37) + 1.0f);
		float fSlow42 = (synth_piano_faustpower2_f(fSlow18) * fSlow32);
		float fSlow43 = synth_piano_faustpower2_f(fSlow37);
		float fSlow44 = (fConst13 * (((fSlow23 + std::atan2((((0.0f - ((fSlow18 * fSlow35) / fSlow37)) * fSlow38) + (((fSlow18 * fSlow32) * fSlow41) / fSlow37)), ((fSlow38 * fSlow41) + ((fSlow42 * fSlow35) / fSlow43)))) + 6.28318548f) / fSlow16));
		float fSlow45 = std::floor(fSlow44);
		float fSlow46 = (fSlow44 - fSlow45);
		int iSlow47 = (float(fButton0) > 0.0f);
		float fSlow48 = float(iSlow47);
		float fSlow49 = float((0 - (iSlow47 + -1)));
		float fSlow50 = (0.00100000005f * ((0.999599993f * fSlow48) + (0.899999976f * (fSlow49 * float(getValueReleaseLoopGain(float(iSlow6)))))));
		float fSlow51 = float(getValueDCBa1(float(iSlow6)));
		float fSlow52 = (1.0f - fSlow51);
		float fSlow53 = (0.5f * fSlow52);
		float fSlow54 = (0.25f * float(fHslider6));
		float fSlow55 = float(getValueLoudPole(float(iSlow6)));
		float fSlow56 = ((fSlow54 + (0.980000019f - fSlow55)) * float(getValueLoudGain(float(iSlow6))));
		float fSlow57 = (1.39698386e-09f * (fSlow56 * float((iSlow6 < 88))));
		int iSlow58 = (iSlow47 > 0);
		float fSlow59 = std::exp((0.0f - (fConst15 / (float(fEntry0) * float(getValueDryTapAmpT60(float(iSlow6)))))));
		int iSlow60 = (iSlow47 < 1);
		float fSlow61 = (fConst16 * fSlow49);
		float fSlow62 = float(fHslider7);
		float fSlow63 = (fSlow48 * std::exp((0.0f - (fConst17 / fSlow62))));
		float fSlow64 = (fConst18 * fSlow62);
		float fSlow65 = (0.200000003f * float(getValueSustainPedalLevel(float(iSlow6))));
		float fSlow66 = (fSlow55 + (0.0199999996f - fSlow54));
		float fSlow67 = (0.0f - fSlow53);
		float fSlow68 = (3.70000005f * fSlow19);
		float fSlow69 = ((fSlow28 + fSlow34) - fSlow31);
		float fSlow70 = (((fSlow40 + (fSlow22 * fSlow69)) / fSlow37) + 1.0f);
		int iSlow71 = int((fConst13 * (((fSlow23 + std::atan2((0.0f - ((fSlow18 * ((fSlow69 * fSlow38) - (fSlow32 * fSlow70))) / fSlow37)), ((fSlow38 * fSlow70) + ((fSlow42 * fSlow69) / fSlow43)))) + 6.28318548f) / fSlow16)));
		int iSlow72 = std::min<int>(4097, std::max<int>(0, (iSlow71 + 1)));
		float fSlow73 = (1.0f / fSlow37);
		float fSlow74 = (fSlow5 - fSlow15);
		float fSlow75 = (fConst14 * fSlow74);
		float fSlow76 = std::sin(fSlow75);
		float fSlow77 = std::cos(fSlow75);
		float fSlow78 = (3.0f * std::atan2((fSlow14 * fSlow76), (fSlow20 + (fSlow21 * fSlow77))));
		float fSlow79 = (((fSlow77 * fSlow32) / fSlow37) + 1.0f);
		float fSlow80 = ((((fSlow77 * fSlow35) + fSlow40) / fSlow37) + 1.0f);
		float fSlow81 = (synth_piano_faustpower2_f(fSlow76) * fSlow32);
		float fSlow82 = (fConst13 * (((fSlow78 + std::atan2((((0.0f - ((fSlow76 * fSlow35) / fSlow37)) * fSlow79) + (((fSlow76 * fSlow32) * fSlow80) / fSlow37)), ((fSlow79 * fSlow80) + ((fSlow81 * fSlow35) / fSlow43)))) + 6.28318548f) / fSlow74));
		float fSlow83 = std::floor(fSlow82);
		float fSlow84 = (fSlow83 + (1.0f - fSlow82));
		float fSlow85 = (((fSlow40 + (fSlow77 * fSlow69)) / fSlow37) + 1.0f);
		int iSlow86 = int((fConst13 * (((fSlow78 + std::atan2((0.0f - ((fSlow76 * ((fSlow79 * fSlow69) - (fSlow32 * fSlow85))) / fSlow37)), ((fSlow79 * fSlow85) + ((fSlow81 * fSlow69) / fSlow43)))) + 6.28318548f) / fSlow74)));
		int iSlow87 = std::min<int>(4097, std::max<int>(0, iSlow86));
		float fSlow88 = (fSlow82 - fSlow83);
		int iSlow89 = std::min<int>(4097, std::max<int>(0, (iSlow86 + 1)));
		float fSlow90 = (fSlow45 + (1.0f - fSlow44));
		int iSlow91 = std::min<int>(4097, std::max<int>(0, iSlow71));
		float fSlow92 = ((0.0f - (fConst20 * (fSlow4 * fSlow7))) * std::cos((fConst21 * (fSlow4 / float(getValueStrikePosition(float(iSlow6)))))));
		float fSlow93 = (fConst22 * fSlow8);
		float fSlow94 = std::cos((fConst21 * fSlow4));
		float fSlow95 = std::pow(10.0f, (0.0500000007f * float(getValueSecondStageAmpRatio(float(iSlow6)))));
		float fSlow96 = std::pow(10.0f, (fConst23 * float(getValuer1_1db(float(iSlow6)))));
		float fSlow97 = std::pow(10.0f, (fConst23 * float(getValuer1_2db(float(iSlow6)))));
		float fSlow98 = (1.0f - fSlow95);
		float fSlow99 = (0.0f - (2.0f * ((fSlow95 * fSlow96) + (fSlow97 * fSlow98))));
		float fSlow100 = float(getValueBq4_gEarBalled(float(iSlow6)));
		float fSlow101 = (2.0f * fSlow100);
		float fSlow102 = float((iSlow6 >= 88));
		float fSlow103 = (2.32830644e-10f * fSlow102);
		float fSlow104 = (1.16415322e-10f * fSlow102);
		float fSlow105 = std::pow(10.0f, (fConst23 * float(getValuer3db(float(iSlow6)))));
		float fSlow106 = (std::cos((fConst21 * (fSlow4 * float(getValueThirdPartialFactor(float(iSlow6)))))) * (0.0f - (2.0f * fSlow105)));
		float fSlow107 = synth_piano_faustpower2_f(fSlow105);
		float fSlow108 = std::pow(10.0f, (fConst23 * float(getValuer2db(float(iSlow6)))));
		float fSlow109 = (std::cos((fConst21 * (fSlow4 * float(getValueSecondPartialFactor(float(iSlow6)))))) * (0.0f - (2.0f * fSlow108)));
		float fSlow110 = synth_piano_faustpower2_f(fSlow108);
		float fSlow111 = (fSlow94 * (0.0f - (2.0f * fSlow96)));
		float fSlow112 = synth_piano_faustpower2_f(fSlow96);
		float fSlow113 = (0.0f - (2.0f * fSlow97));
		float fSlow114 = synth_piano_faustpower2_f(fSlow97);
		float fSlow115 = ((fSlow95 * fSlow112) + (fSlow114 * fSlow98));
		int iSlow116 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst24 * (float(fHslider8) / fSlow4)))));
		float fSlow117 = (12.0f * (1.0f - fSlow1));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec11[0] = (fSlow0 + (0.999000013f * fRec11[1]));
			float fTemp0 = std::exp((fConst4 / fRec11[0]));
			float fTemp1 = synth_piano_faustpower2_f(fTemp0);
			float fTemp2 = (1.0f - (fConst1 * fTemp1));
			float fTemp3 = (1.0f - fTemp1);
			float fTemp4 = (fTemp2 / fTemp3);
			float fTemp5 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fTemp2) / synth_piano_faustpower2_f(fTemp3)) + -1.0f)));
			fRec12[0] = (0.0f - (fConst6 * ((fConst7 * fRec12[1]) - (fRec7[1] + fRec7[2]))));
			fRec10[0] = ((fRec10[1] * (fTemp4 - fTemp5)) + ((fTemp0 * (fRec7[1] + (fRec12[0] * ((std::exp((fConst8 / fRec11[0])) / fTemp0) + -1.0f)))) * (fTemp5 + (1.0f - fTemp4))));
			fVec0[(IOTA & 32767)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			fRec13[0] = (fSlow3 + (0.999000013f * fRec13[1]));
			fRec21[0] = (fSlow50 + (0.999000013f * fRec21[1]));
			iRec27[0] = ((1103515245 * iRec27[1]) + 12345);
			iRec29[0] = ((iSlow47 * iRec29[1]) + 1);
			float fTemp6 = float((iRec29[0] + -1));
			float fTemp7 = float(((fTemp6 < 2.0f) & iSlow58));
			float fTemp8 = ((0.0301973838f * fTemp7) + (fSlow59 * float(((fTemp6 >= 2.0f) | iSlow60))));
			fRec28[0] = ((fRec28[1] * fTemp8) + (0.150000006f * (fTemp7 * (1.0f - fTemp8))));
			int iTemp9 = (fTemp6 < fSlow64);
			float fTemp10 = ((fSlow63 * float(iTemp9)) + (fConst19 * float((iSlow47 * (fTemp6 >= fSlow64)))));
			fRec30[0] = ((fRec30[1] * (fSlow61 + fTemp10)) + (fSlow65 * (((1.0f - fTemp10) - fSlow61) * float((iTemp9 & iSlow58)))));
			float fTemp11 = (float(iRec27[0]) * (fRec28[0] + fRec30[0]));
			fRec26[0] = ((fSlow57 * fTemp11) + (fSlow66 * fRec26[1]));
			fRec25[0] = ((fSlow56 * fRec26[0]) + (fSlow66 * fRec25[1]));
			fRec24[0] = ((fSlow56 * fRec25[0]) + (fSlow66 * fRec24[1]));
			fRec23[0] = ((fSlow56 * fRec24[0]) + (fSlow66 * fRec23[1]));
			fRec22[0] = (((fSlow53 * fRec23[0]) + (fSlow67 * fRec23[1])) - (fSlow51 * fRec22[1]));
			float fTemp12 = (fRec21[0] * (fRec22[0] + fRec15[1]));
			fVec1[0] = fTemp12;
			fRec20[0] = (fVec1[1] + (fSlow68 * (fTemp12 - fRec20[1])));
			fRec19[0] = (fRec20[1] + (fSlow68 * (fRec20[0] - fRec19[1])));
			fRec18[(IOTA & 8191)] = (fRec19[1] + (fSlow68 * (fRec19[0] - fRec18[((IOTA - 1) & 8191)])));
			float fTemp13 = (fSlow46 * fRec18[((IOTA - iSlow72) & 8191)]);
			float fTemp14 = (fRec22[0] + (fRec21[0] * fRec16[1]));
			fVec2[0] = fTemp14;
			fRec34[0] = (fVec2[1] + (fSlow68 * (fTemp14 - fRec34[1])));
			fRec33[0] = (fRec34[1] + (fSlow68 * (fRec34[0] - fRec33[1])));
			fRec32[(IOTA & 8191)] = (fRec33[1] + (fSlow68 * (fRec33[0] - fRec32[((IOTA - 1) & 8191)])));
			float fTemp15 = (fSlow84 * fRec32[((IOTA - iSlow87) & 8191)]);
			float fTemp16 = (fSlow88 * fRec32[((IOTA - iSlow89) & 8191)]);
			float fTemp17 = (fSlow90 * fRec18[((IOTA - iSlow91) & 8191)]);
			float fTemp18 = (fTemp13 + ((fTemp15 + fTemp16) + fTemp17));
			fVec3[0] = fTemp18;
			fRec31[0] = (fSlow73 * ((2.0f * ((fSlow39 * fTemp18) + (fSlow33 * fVec3[1]))) - (fSlow32 * fRec31[1])));
			fRec15[0] = (fTemp13 + (fRec31[0] + fTemp17));
			fRec16[0] = (fTemp16 + (fRec31[0] + fTemp15));
			float fRec17 = fTemp18;
			fRec14[0] = ((fSlow10 * fRec17) - ((fSlow92 * fRec14[1]) + (fSlow93 * fRec14[2])));
			fVec4[0] = (fSlow103 * fTemp11);
			float fTemp19 = (0.0f - ((0.5f * fVec4[1]) + (fSlow104 * fTemp11)));
			fVec5[0] = fTemp19;
			fRec44[0] = (((fSlow53 * fTemp19) + (fSlow67 * fVec5[1])) - (fSlow51 * fRec44[1]));
			fRec43[0] = ((fSlow56 * fRec44[0]) + (fSlow66 * fRec43[1]));
			fRec42[0] = ((fSlow56 * fRec43[0]) + (fSlow66 * fRec42[1]));
			fRec41[0] = ((fSlow56 * fRec42[0]) + (fSlow66 * fRec41[1]));
			fRec40[0] = ((fSlow56 * fRec41[0]) + (fSlow66 * fRec40[1]));
			fRec39[0] = ((fSlow100 * (fRec40[0] - fRec40[1])) - ((fSlow106 * fRec39[1]) + (fSlow107 * fRec39[2])));
			fRec38[0] = ((fSlow101 * fRec39[0]) - ((fSlow109 * fRec38[1]) + (fSlow110 * fRec38[2])));
			fRec37[0] = (fRec38[0] - ((fSlow111 * fRec37[1]) + (fSlow112 * fRec37[2])));
			fRec36[0] = (((fSlow94 * ((fSlow99 * fRec37[1]) - (fSlow113 * fRec36[1]))) + (fRec37[0] + (fSlow115 * fRec37[2]))) - (fSlow114 * fRec36[2]));
			fRec35[0] = ((fSlow52 * fRec36[0]) - (fSlow51 * fRec35[1]));
			float fTemp20 = ((fSlow9 * (fRec14[0] - fRec14[2])) + (fRec17 + fRec35[0]));
			fVec6[(IOTA & 4095)] = fTemp20;
			float fTemp21 = fVec6[((IOTA - iSlow116) & 4095)];
			fVec7[(IOTA & 4095)] = (fSlow2 * (fRec13[0] * fTemp21));
			float fTemp22 = (0.300000012f * fVec7[((IOTA - iConst25) & 4095)]);
			float fTemp23 = (((0.600000024f * fRec8[1]) + fVec0[((IOTA - iConst10) & 32767)]) - fTemp22);
			fVec8[(IOTA & 2047)] = fTemp23;
			fRec8[0] = fVec8[((IOTA - iConst26) & 2047)];
			float fRec9 = (0.0f - (0.600000024f * fTemp23));
			float fTemp24 = std::exp((fConst29 / fRec11[0]));
			float fTemp25 = synth_piano_faustpower2_f(fTemp24);
			float fTemp26 = (1.0f - (fConst1 * fTemp25));
			float fTemp27 = (1.0f - fTemp25);
			float fTemp28 = (fTemp26 / fTemp27);
			float fTemp29 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fTemp26) / synth_piano_faustpower2_f(fTemp27)) + -1.0f)));
			fRec48[0] = (0.0f - (fConst6 * ((fConst7 * fRec48[1]) - (fRec3[1] + fRec3[2]))));
			fRec47[0] = ((fRec47[1] * (fTemp28 - fTemp29)) + ((fTemp24 * (fRec3[1] + (fRec48[0] * ((std::exp((fConst30 / fRec11[0])) / fTemp24) + -1.0f)))) * (fTemp29 + (1.0f - fTemp28))));
			fVec9[(IOTA & 32767)] = ((0.353553385f * fRec47[0]) + 9.99999968e-21f);
			float fTemp30 = (((0.600000024f * fRec45[1]) + fVec9[((IOTA - iConst32) & 32767)]) - fTemp22);
			fVec10[(IOTA & 4095)] = fTemp30;
			fRec45[0] = fVec10[((IOTA - iConst33) & 4095)];
			float fRec46 = (0.0f - (0.600000024f * fTemp30));
			float fTemp31 = std::exp((fConst36 / fRec11[0]));
			float fTemp32 = synth_piano_faustpower2_f(fTemp31);
			float fTemp33 = (1.0f - (fConst1 * fTemp32));
			float fTemp34 = (1.0f - fTemp32);
			float fTemp35 = (fTemp33 / fTemp34);
			float fTemp36 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fTemp33) / synth_piano_faustpower2_f(fTemp34)) + -1.0f)));
			fRec52[0] = (0.0f - (fConst6 * ((fConst7 * fRec52[1]) - (fRec5[1] + fRec5[2]))));
			fRec51[0] = ((fRec51[1] * (fTemp35 - fTemp36)) + ((fTemp31 * (fRec5[1] + (fRec52[0] * ((std::exp((fConst37 / fRec11[0])) / fTemp31) + -1.0f)))) * (fTemp36 + (1.0f - fTemp35))));
			fVec11[(IOTA & 16383)] = ((0.353553385f * fRec51[0]) + 9.99999968e-21f);
			float fTemp37 = (fVec11[((IOTA - iConst39) & 16383)] + (fTemp22 + (0.600000024f * fRec49[1])));
			fVec12[(IOTA & 4095)] = fTemp37;
			fRec49[0] = fVec12[((IOTA - iConst40) & 4095)];
			float fRec50 = (0.0f - (0.600000024f * fTemp37));
			float fTemp38 = std::exp((fConst43 / fRec11[0]));
			float fTemp39 = synth_piano_faustpower2_f(fTemp38);
			float fTemp40 = (1.0f - (fConst1 * fTemp39));
			float fTemp41 = (1.0f - fTemp39);
			float fTemp42 = (fTemp40 / fTemp41);
			float fTemp43 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fTemp40) / synth_piano_faustpower2_f(fTemp41)) + -1.0f)));
			fRec56[0] = (0.0f - (fConst6 * ((fConst7 * fRec56[1]) - (fRec1[1] + fRec1[2]))));
			fRec55[0] = ((fRec55[1] * (fTemp42 - fTemp43)) + ((fTemp38 * (fRec1[1] + (fRec56[0] * ((std::exp((fConst44 / fRec11[0])) / fTemp38) + -1.0f)))) * (fTemp43 + (1.0f - fTemp42))));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec55[0]) + 9.99999968e-21f);
			float fTemp44 = (fTemp22 + ((0.600000024f * fRec53[1]) + fVec13[((IOTA - iConst46) & 32767)]));
			fVec14[(IOTA & 4095)] = fTemp44;
			fRec53[0] = fVec14[((IOTA - iConst47) & 4095)];
			float fRec54 = (0.0f - (0.600000024f * fTemp44));
			float fTemp45 = std::exp((fConst50 / fRec11[0]));
			float fTemp46 = synth_piano_faustpower2_f(fTemp45);
			float fTemp47 = (1.0f - (fConst1 * fTemp46));
			float fTemp48 = (1.0f - fTemp46);
			float fTemp49 = (fTemp47 / fTemp48);
			float fTemp50 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fTemp47) / synth_piano_faustpower2_f(fTemp48)) + -1.0f)));
			fRec60[0] = (0.0f - (fConst6 * ((fConst7 * fRec60[1]) - (fRec6[1] + fRec6[2]))));
			fRec59[0] = ((fRec59[1] * (fTemp49 - fTemp50)) + ((fTemp45 * (fRec6[1] + (fRec60[0] * ((std::exp((fConst51 / fRec11[0])) / fTemp45) + -1.0f)))) * (fTemp50 + (1.0f - fTemp49))));
			fVec15[(IOTA & 16383)] = ((0.353553385f * fRec59[0]) + 9.99999968e-21f);
			fVec16[(IOTA & 4095)] = (fSlow117 * (fRec13[0] * fTemp20));
			float fTemp51 = (0.300000012f * fVec16[((IOTA - iConst25) & 4095)]);
			float fTemp52 = (fVec15[((IOTA - iConst53) & 16383)] - (fTemp51 + (0.600000024f * fRec57[1])));
			fVec17[(IOTA & 2047)] = fTemp52;
			fRec57[0] = fVec17[((IOTA - iConst54) & 2047)];
			float fRec58 = (0.600000024f * fTemp52);
			float fTemp53 = std::exp((fConst57 / fRec11[0]));
			float fTemp54 = synth_piano_faustpower2_f(fTemp53);
			float fTemp55 = (1.0f - (fConst1 * fTemp54));
			float fTemp56 = (1.0f - fTemp54);
			float fTemp57 = (fTemp55 / fTemp56);
			float fTemp58 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fTemp55) / synth_piano_faustpower2_f(fTemp56)) + -1.0f)));
			fRec64[0] = (0.0f - (fConst6 * ((fConst7 * fRec64[1]) - (fRec2[1] + fRec2[2]))));
			fRec63[0] = ((fRec63[1] * (fTemp57 - fTemp58)) + ((fTemp53 * (fRec2[1] + (fRec64[0] * ((std::exp((fConst58 / fRec11[0])) / fTemp53) + -1.0f)))) * (fTemp58 + (1.0f - fTemp57))));
			fVec18[(IOTA & 16383)] = ((0.353553385f * fRec63[0]) + 9.99999968e-21f);
			float fTemp59 = (fVec18[((IOTA - iConst60) & 16383)] - (fTemp51 + (0.600000024f * fRec61[1])));
			fVec19[(IOTA & 4095)] = fTemp59;
			fRec61[0] = fVec19[((IOTA - iConst61) & 4095)];
			float fRec62 = (0.600000024f * fTemp59);
			float fTemp60 = std::exp((fConst64 / fRec11[0]));
			float fTemp61 = synth_piano_faustpower2_f(fTemp60);
			float fTemp62 = (1.0f - (fConst1 * fTemp61));
			float fTemp63 = (1.0f - fTemp61);
			float fTemp64 = (fTemp62 / fTemp63);
			float fTemp65 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fTemp62) / synth_piano_faustpower2_f(fTemp63)) + -1.0f)));
			fRec68[0] = (0.0f - (fConst6 * ((fConst7 * fRec68[1]) - (fRec4[1] + fRec4[2]))));
			fRec67[0] = ((fRec67[1] * (fTemp64 - fTemp65)) + ((fTemp60 * (fRec4[1] + (fRec68[0] * ((std::exp((fConst65 / fRec11[0])) / fTemp60) + -1.0f)))) * (fTemp65 + (1.0f - fTemp64))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec67[0]) + 9.99999968e-21f);
			float fTemp66 = ((fTemp51 + fVec20[((IOTA - iConst67) & 16383)]) - (0.600000024f * fRec65[1]));
			fVec21[(IOTA & 4095)] = fTemp66;
			fRec65[0] = fVec21[((IOTA - iConst68) & 4095)];
			float fRec66 = (0.600000024f * fTemp66);
			float fTemp67 = std::exp((fConst71 / fRec11[0]));
			float fTemp68 = synth_piano_faustpower2_f(fTemp67);
			float fTemp69 = (1.0f - (fConst1 * fTemp68));
			float fTemp70 = (1.0f - fTemp68);
			float fTemp71 = (fTemp69 / fTemp70);
			float fTemp72 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fTemp69) / synth_piano_faustpower2_f(fTemp70)) + -1.0f)));
			fRec72[0] = (0.0f - (fConst6 * ((fConst7 * fRec72[1]) - (fRec0[1] + fRec0[2]))));
			fRec71[0] = ((fRec71[1] * (fTemp71 - fTemp72)) + ((fTemp67 * (fRec0[1] + (fRec72[0] * ((std::exp((fConst72 / fRec11[0])) / fTemp67) + -1.0f)))) * (fTemp72 + (1.0f - fTemp71))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec71[0]) + 9.99999968e-21f);
			float fTemp73 = ((fVec22[((IOTA - iConst74) & 16383)] + fTemp51) - (0.600000024f * fRec69[1]));
			fVec23[(IOTA & 2047)] = fTemp73;
			fRec69[0] = fVec23[((IOTA - iConst75) & 2047)];
			float fRec70 = (0.600000024f * fTemp73);
			float fTemp74 = (fRec70 + fRec69[1]);
			float fTemp75 = (fRec66 + fTemp74);
			float fTemp76 = (fRec58 + (fRec62 + fTemp75));
			fRec0[0] = (fRec8[1] + (fRec45[1] + (fRec49[1] + (fRec53[1] + (fRec57[1] + (fRec61[1] + (fRec65[1] + (fRec9 + (fRec46 + (fRec50 + (fRec54 + fTemp76)))))))))));
			fRec1[0] = ((fRec57[1] + (fRec61[1] + (fRec65[1] + fTemp76))) - (fRec8[1] + (fRec45[1] + (fRec49[1] + (fRec53[1] + (fRec9 + (fRec46 + (fRec54 + fRec50))))))));
			float fTemp77 = (fRec62 + fRec58);
			fRec2[0] = ((fRec49[1] + (fRec53[1] + (fRec65[1] + (fRec50 + (fRec54 + fTemp75))))) - (fRec8[1] + (fRec45[1] + (fRec57[1] + (fRec61[1] + (fRec9 + (fRec46 + fTemp77)))))));
			fRec3[0] = ((fRec8[1] + (fRec45[1] + (fRec65[1] + (fRec9 + (fRec46 + fTemp75))))) - (fRec49[1] + (fRec53[1] + (fRec57[1] + (fRec61[1] + (fRec50 + (fRec54 + fTemp77)))))));
			float fTemp78 = (fRec62 + fTemp74);
			float fTemp79 = (fRec66 + fRec58);
			fRec4[0] = ((fRec45[1] + (fRec53[1] + (fRec61[1] + (fRec46 + (fRec54 + fTemp78))))) - (fRec8[1] + (fRec49[1] + (fRec57[1] + (fRec65[1] + (fRec9 + (fRec50 + fTemp79)))))));
			fRec5[0] = ((fRec8[1] + (fRec49[1] + (fRec61[1] + (fRec9 + (fRec50 + fTemp78))))) - (fRec45[1] + (fRec53[1] + (fRec57[1] + (fRec65[1] + (fRec46 + (fRec54 + fTemp79)))))));
			float fTemp80 = (fRec58 + fTemp74);
			float fTemp81 = (fRec66 + fRec62);
			fRec6[0] = ((fRec8[1] + (fRec53[1] + (fRec57[1] + (fRec9 + (fRec54 + fTemp80))))) - (fRec45[1] + (fRec49[1] + (fRec61[1] + (fRec65[1] + (fRec46 + (fRec50 + fTemp81)))))));
			fRec7[0] = ((fRec45[1] + (fRec49[1] + (fRec57[1] + (fRec46 + (fRec50 + fTemp80))))) - (fRec8[1] + (fRec53[1] + (fRec61[1] + (fRec65[1] + (fRec9 + (fRec54 + fTemp81)))))));
			float fTemp82 = (1.0f - fRec13[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow117 * (fTemp82 * fTemp20))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow2 * (fTemp82 * fTemp21))));
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec10[1] = fRec10[0];
			IOTA = (IOTA + 1);
			fRec13[1] = fRec13[0];
			fRec21[1] = fRec21[0];
			iRec27[1] = iRec27[0];
			iRec29[1] = iRec29[0];
			fRec28[1] = fRec28[0];
			fRec30[1] = fRec30[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fVec1[1] = fVec1[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fVec2[1] = fVec2[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fVec3[1] = fVec3[0];
			fRec31[1] = fRec31[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec8[1] = fRec8[0];
			fRec48[1] = fRec48[0];
			fRec47[1] = fRec47[0];
			fRec45[1] = fRec45[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec49[1] = fRec49[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec53[1] = fRec53[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fRec57[1] = fRec57[0];
			fRec64[1] = fRec64[0];
			fRec63[1] = fRec63[0];
			fRec61[1] = fRec61[0];
			fRec68[1] = fRec68[0];
			fRec67[1] = fRec67[0];
			fRec65[1] = fRec65[0];
			fRec72[1] = fRec72[0];
			fRec71[1] = fRec71[0];
			fRec69[1] = fRec69[0];
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
