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
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fRec11[2];
	float fRec10[2];
	int IOTA;
	float fVec0[16384];
	float fConst9;
	int iConst10;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fRec12[2];
	float fConst11;
	float fConst12;
	FAUSTFLOAT fHslider3;
	float fConst13;
	FAUSTFLOAT fHslider4;
	float fConst14;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fButton0;
	float fRec20[2];
	FAUSTFLOAT fHslider6;
	int iRec26[2];
	int iRec28[2];
	float fConst15;
	FAUSTFLOAT fEntry0;
	float fRec27[2];
	float fConst16;
	float fConst17;
	FAUSTFLOAT fHslider7;
	float fConst18;
	float fConst19;
	float fRec29[2];
	float fRec25[2];
	float fRec24[2];
	float fRec23[2];
	float fRec22[2];
	float fRec21[2];
	float fVec1[2];
	float fRec19[2];
	float fRec18[2];
	float fRec17[8192];
	float fVec2[2];
	float fRec33[2];
	float fRec32[2];
	float fRec31[8192];
	float fVec3[2];
	float fRec30[2];
	float fRec14[2];
	float fRec15[2];
	float fConst20;
	float fConst21;
	float fConst22;
	float fRec13[3];
	float fConst23;
	float fVec4[2];
	float fVec5[2];
	float fRec43[2];
	float fRec42[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[2];
	float fRec38[3];
	float fRec37[3];
	float fRec36[3];
	float fRec35[3];
	float fRec34[2];
	float fVec6[4096];
	float fVec7[4096];
	int iConst24;
	float fVec8[2048];
	int iConst25;
	float fRec8[2];
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fRec47[2];
	float fRec46[2];
	float fVec9[32768];
	float fConst30;
	int iConst31;
	float fConst32;
	FAUSTFLOAT fHslider8;
	float fVec10[4096];
	float fVec11[2048];
	int iConst33;
	float fRec44[2];
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fRec51[2];
	float fRec50[2];
	float fVec12[32768];
	float fConst38;
	int iConst39;
	float fVec13[4096];
	int iConst40;
	float fRec48[2];
	float fConst41;
	float fConst42;
	float fConst43;
	float fConst44;
	float fRec55[2];
	float fRec54[2];
	float fVec14[16384];
	float fConst45;
	int iConst46;
	float fVec15[4096];
	int iConst47;
	float fRec52[2];
	float fConst48;
	float fConst49;
	float fConst50;
	float fConst51;
	float fRec59[2];
	float fRec58[2];
	float fVec16[32768];
	float fConst52;
	int iConst53;
	float fVec17[4096];
	int iConst54;
	float fRec56[2];
	float fConst55;
	float fConst56;
	float fConst57;
	float fConst58;
	float fRec63[2];
	float fRec62[2];
	float fVec18[16384];
	float fConst59;
	int iConst60;
	float fVec19[2048];
	int iConst61;
	float fRec60[2];
	float fConst62;
	float fConst63;
	float fConst64;
	float fConst65;
	float fRec67[2];
	float fRec66[2];
	float fVec20[16384];
	float fConst66;
	int iConst67;
	float fVec21[4096];
	int iConst68;
	float fRec64[2];
	float fConst69;
	float fConst70;
	float fConst71;
	float fConst72;
	float fRec71[2];
	float fRec70[2];
	float fVec22[16384];
	float fConst73;
	int iConst74;
	float fVec23[4096];
	int iConst75;
	float fRec68[2];
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
		m->declare("ceammc_instruments.lib/version", "0.3");
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
		fConst2 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst3 = ((0.0f - (6.90775537f * fConst2)) / fConst0);
		fConst4 = (0.25f * fConst3);
		fConst5 = (0.166666672f * fConst3);
		fConst6 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst7 = (1.0f / (fConst6 + 1.0f));
		fConst8 = (1.0f - fConst6);
		fConst9 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst10 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst2 - fConst9))));
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
		iConst24 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		iConst25 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst9 + -1.0f))));
		fConst26 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst27 = ((0.0f - (6.90775537f * fConst26)) / fConst0);
		fConst28 = (0.25f * fConst27);
		fConst29 = (0.166666672f * fConst27);
		fConst30 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst31 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst26 - fConst30))));
		fConst32 = (0.0011363636f * fConst0);
		iConst33 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst30 + -1.0f))));
		fConst34 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst35 = ((0.0f - (6.90775537f * fConst34)) / fConst0);
		fConst36 = (0.25f * fConst35);
		fConst37 = (0.166666672f * fConst35);
		fConst38 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst39 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst34 - fConst38))));
		iConst40 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst38 + -1.0f))));
		fConst41 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst42 = ((0.0f - (6.90775537f * fConst41)) / fConst0);
		fConst43 = (0.25f * fConst42);
		fConst44 = (0.166666672f * fConst42);
		fConst45 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst46 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst41 - fConst45))));
		iConst47 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst45 + -1.0f))));
		fConst48 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst49 = ((0.0f - (6.90775537f * fConst48)) / fConst0);
		fConst50 = (0.25f * fConst49);
		fConst51 = (0.166666672f * fConst49);
		fConst52 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst53 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst48 - fConst52))));
		iConst54 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst52 + -1.0f))));
		fConst55 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst56 = ((0.0f - (6.90775537f * fConst55)) / fConst0);
		fConst57 = (0.25f * fConst56);
		fConst58 = (0.166666672f * fConst56);
		fConst59 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst60 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst55 - fConst59))));
		iConst61 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst59 + -1.0f))));
		fConst62 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst63 = ((0.0f - (6.90775537f * fConst62)) / fConst0);
		fConst64 = (0.25f * fConst63);
		fConst65 = (0.166666672f * fConst63);
		fConst66 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst67 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst62 - fConst66))));
		iConst68 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst66 + -1.0f))));
		fConst69 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst70 = ((0.0f - (6.90775537f * fConst69)) / fConst0);
		fConst71 = (0.25f * fConst70);
		fConst72 = (0.166666672f * fConst70);
		fConst73 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst74 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst69 - fConst73))));
		iConst75 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst73 + -1.0f))));
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
			fRec10[l1] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 16384); l2 = (l2 + 1)) {
			fVec0[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec12[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec20[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec26[l5] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec28[l6] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec27[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec29[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec25[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec24[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec23[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec22[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec21[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fVec1[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec19[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec18[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 8192); l17 = (l17 + 1)) {
			fRec17[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fVec2[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec33[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec32[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 8192); l21 = (l21 + 1)) {
			fRec31[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fVec3[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec30[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec14[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec15[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec13[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fVec4[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fVec5[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec43[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec42[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec41[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec40[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec39[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec38[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec37[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 3); l36 = (l36 + 1)) {
			fRec36[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 3); l37 = (l37 + 1)) {
			fRec35[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec34[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 4096); l39 = (l39 + 1)) {
			fVec6[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 4096); l40 = (l40 + 1)) {
			fVec7[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2048); l41 = (l41 + 1)) {
			fVec8[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec8[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec47[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec46[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 32768); l45 = (l45 + 1)) {
			fVec9[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 4096); l46 = (l46 + 1)) {
			fVec10[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 2048); l47 = (l47 + 1)) {
			fVec11[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec44[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec51[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec50[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 32768); l51 = (l51 + 1)) {
			fVec12[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 4096); l52 = (l52 + 1)) {
			fVec13[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec48[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec55[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec54[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 16384); l56 = (l56 + 1)) {
			fVec14[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 4096); l57 = (l57 + 1)) {
			fVec15[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec52[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec59[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec58[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 32768); l61 = (l61 + 1)) {
			fVec16[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 4096); l62 = (l62 + 1)) {
			fVec17[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec56[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec63[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec62[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 16384); l66 = (l66 + 1)) {
			fVec18[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 2048); l67 = (l67 + 1)) {
			fVec19[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec60[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec67[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec66[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 16384); l71 = (l71 + 1)) {
			fVec20[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 4096); l72 = (l72 + 1)) {
			fVec21[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec64[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec71[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec70[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 16384); l76 = (l76 + 1)) {
			fVec22[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 4096); l77 = (l77 + 1)) {
			fVec23[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec68[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 3); l79 = (l79 + 1)) {
			fRec0[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 3); l80 = (l80 + 1)) {
			fRec1[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 3); l81 = (l81 + 1)) {
			fRec2[l81] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 3); l82 = (l82 + 1)) {
			fRec3[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec4[l83] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec5[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec6[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec7[l86] = 0.0f;
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
		ui_interface->addHorizontalSlider("brightness", &fHslider6, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("detuning", &fHslider5, 0.100000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addNumEntry("gain", &fEntry0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider7, 0.100000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pan", &fHslider1, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider3, 48.0f, 24.0f, 96.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider0, 0.360000014f, 0.00499999989f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("stiffness", &fHslider4, 0.280000001f, 0.0f, 1.0f, 0.00999999978f);
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
		float fSlow11 = (12.0f * (1.0f - fSlow10));
		float fSlow12 = (0.00100000005f * float(fHslider2));
		float fSlow13 = std::pow(2.0f, (0.0833333358f * (float(fHslider3) + -69.0f)));
		float fSlow14 = (440.0f * fSlow13);
		int iSlow15 = int(((17.3123398f * (std::log(fSlow14) + -6.08677483f)) + 69.5f));
		float fSlow16 = float(getValueEQBandWidthFactor(float(iSlow15)));
		float fSlow17 = (synth_piano_faustpower2_f(fSlow13) * synth_piano_faustpower2_f(fSlow16));
		float fSlow18 = (0.5f - (fConst12 * fSlow17));
		float fSlow19 = float(getValueEQGain(float(iSlow15)));
		float fSlow20 = float(fHslider4);
		float fSlow21 = float(getValueStiffnessCoefficient(float(iSlow15)));
		float fSlow22 = (13.6899996f * (synth_piano_faustpower2_f(fSlow20) * synth_piano_faustpower2_f(fSlow21)));
		float fSlow23 = (fSlow22 + -1.0f);
		float fSlow24 = (5.0f * (float(fHslider5) * float(getValueDetuningHz(float(iSlow15)))));
		float fSlow25 = (fSlow14 + fSlow24);
		float fSlow26 = (fConst14 * fSlow25);
		float fSlow27 = std::sin(fSlow26);
		float fSlow28 = (fSlow20 * fSlow21);
		float fSlow29 = (7.4000001f * fSlow28);
		float fSlow30 = (fSlow22 + 1.0f);
		float fSlow31 = std::cos(fSlow26);
		float fSlow32 = (3.0f * std::atan2((fSlow23 * fSlow27), (fSlow29 + (fSlow30 * fSlow31))));
		float fSlow33 = std::pow(10.0f, (0.000113636364f * (float(getValueSingleStringDecayRate(float(iSlow15))) / fSlow13)));
		float fSlow34 = float(getValueSingleStringZero(float(iSlow15)));
		float fSlow35 = float(getValueSingleStringPole(float(iSlow15)));
		float fSlow36 = (1.0f - fSlow35);
		float fSlow37 = ((fSlow33 * fSlow34) * fSlow36);
		float fSlow38 = (1.0f - fSlow34);
		float fSlow39 = (fSlow35 * fSlow38);
		float fSlow40 = (3.0f * fSlow39);
		float fSlow41 = (fSlow37 - fSlow40);
		float fSlow42 = (fSlow39 - fSlow37);
		float fSlow43 = (4.0f * fSlow42);
		float fSlow44 = (fSlow41 + fSlow43);
		float fSlow45 = (fSlow33 * fSlow36);
		float fSlow46 = ((3.0f * fSlow38) - fSlow45);
		float fSlow47 = (((fSlow31 * fSlow41) / fSlow46) + 1.0f);
		float fSlow48 = ((fSlow34 + fSlow45) + -1.0f);
		float fSlow49 = (4.0f * fSlow48);
		float fSlow50 = (((fSlow49 + (fSlow31 * fSlow44)) / fSlow46) + 1.0f);
		float fSlow51 = (synth_piano_faustpower2_f(fSlow27) * fSlow41);
		float fSlow52 = synth_piano_faustpower2_f(fSlow46);
		float fSlow53 = (fConst13 * (((fSlow32 + std::atan2((((0.0f - ((fSlow27 * fSlow44) / fSlow46)) * fSlow47) + (((fSlow27 * fSlow41) * fSlow50) / fSlow46)), ((fSlow47 * fSlow50) + ((fSlow51 * fSlow44) / fSlow52)))) + 6.28318548f) / fSlow25));
		float fSlow54 = std::floor(fSlow53);
		float fSlow55 = (fSlow53 - fSlow54);
		int iSlow56 = (float(fButton0) > 0.0f);
		float fSlow57 = float(iSlow56);
		float fSlow58 = float((0 - (iSlow56 + -1)));
		float fSlow59 = (0.00100000005f * ((0.999599993f * fSlow57) + (0.899999976f * (fSlow58 * float(getValueReleaseLoopGain(float(iSlow15)))))));
		float fSlow60 = float(getValueDCBa1(float(iSlow15)));
		float fSlow61 = (1.0f - fSlow60);
		float fSlow62 = (0.5f * fSlow61);
		float fSlow63 = (0.25f * float(fHslider6));
		float fSlow64 = float(getValueLoudPole(float(iSlow15)));
		float fSlow65 = ((fSlow63 + (0.980000019f - fSlow64)) * float(getValueLoudGain(float(iSlow15))));
		float fSlow66 = (1.39698386e-09f * (fSlow65 * float((iSlow15 < 88))));
		int iSlow67 = (iSlow56 > 0);
		float fSlow68 = std::exp((0.0f - (fConst15 / (float(fEntry0) * float(getValueDryTapAmpT60(float(iSlow15)))))));
		int iSlow69 = (iSlow56 < 1);
		float fSlow70 = (fConst16 * fSlow58);
		float fSlow71 = float(fHslider7);
		float fSlow72 = (fSlow57 * std::exp((0.0f - (fConst17 / fSlow71))));
		float fSlow73 = (fConst18 * fSlow71);
		float fSlow74 = (0.200000003f * float(getValueSustainPedalLevel(float(iSlow15))));
		float fSlow75 = (fSlow64 + (0.0199999996f - fSlow63));
		float fSlow76 = (0.0f - fSlow62);
		float fSlow77 = (3.70000005f * fSlow28);
		float fSlow78 = ((fSlow37 + fSlow43) - fSlow40);
		float fSlow79 = (((fSlow49 + (fSlow31 * fSlow78)) / fSlow46) + 1.0f);
		int iSlow80 = int((fConst13 * (((fSlow32 + std::atan2((0.0f - ((fSlow27 * ((fSlow78 * fSlow47) - (fSlow41 * fSlow79))) / fSlow46)), ((fSlow47 * fSlow79) + ((fSlow51 * fSlow78) / fSlow52)))) + 6.28318548f) / fSlow25)));
		int iSlow81 = std::min<int>(4097, std::max<int>(0, (iSlow80 + 1)));
		float fSlow82 = (1.0f / fSlow46);
		float fSlow83 = (fSlow14 - fSlow24);
		float fSlow84 = (fConst14 * fSlow83);
		float fSlow85 = std::sin(fSlow84);
		float fSlow86 = std::cos(fSlow84);
		float fSlow87 = (3.0f * std::atan2((fSlow23 * fSlow85), (fSlow29 + (fSlow30 * fSlow86))));
		float fSlow88 = (((fSlow86 * fSlow41) / fSlow46) + 1.0f);
		float fSlow89 = ((((fSlow86 * fSlow44) + fSlow49) / fSlow46) + 1.0f);
		float fSlow90 = (synth_piano_faustpower2_f(fSlow85) * fSlow41);
		float fSlow91 = (fConst13 * (((fSlow87 + std::atan2((((0.0f - ((fSlow85 * fSlow44) / fSlow46)) * fSlow88) + (((fSlow85 * fSlow41) * fSlow89) / fSlow46)), ((fSlow88 * fSlow89) + ((fSlow90 * fSlow44) / fSlow52)))) + 6.28318548f) / fSlow83));
		float fSlow92 = std::floor(fSlow91);
		float fSlow93 = (fSlow92 + (1.0f - fSlow91));
		float fSlow94 = (((fSlow49 + (fSlow86 * fSlow78)) / fSlow46) + 1.0f);
		int iSlow95 = int((fConst13 * (((fSlow87 + std::atan2((0.0f - ((fSlow85 * ((fSlow88 * fSlow78) - (fSlow41 * fSlow94))) / fSlow46)), ((fSlow88 * fSlow94) + ((fSlow90 * fSlow78) / fSlow52)))) + 6.28318548f) / fSlow83)));
		int iSlow96 = std::min<int>(4097, std::max<int>(0, iSlow95));
		float fSlow97 = (fSlow91 - fSlow92);
		int iSlow98 = std::min<int>(4097, std::max<int>(0, (iSlow95 + 1)));
		float fSlow99 = (fSlow54 + (1.0f - fSlow53));
		int iSlow100 = std::min<int>(4097, std::max<int>(0, iSlow80));
		float fSlow101 = ((0.0f - (fConst20 * (fSlow13 * fSlow16))) * std::cos((fConst21 * (fSlow13 / float(getValueStrikePosition(float(iSlow15)))))));
		float fSlow102 = (fConst22 * fSlow17);
		float fSlow103 = std::cos((fConst21 * fSlow13));
		float fSlow104 = std::pow(10.0f, (0.0500000007f * float(getValueSecondStageAmpRatio(float(iSlow15)))));
		float fSlow105 = std::pow(10.0f, (fConst23 * float(getValuer1_1db(float(iSlow15)))));
		float fSlow106 = std::pow(10.0f, (fConst23 * float(getValuer1_2db(float(iSlow15)))));
		float fSlow107 = (1.0f - fSlow104);
		float fSlow108 = (0.0f - (2.0f * ((fSlow104 * fSlow105) + (fSlow106 * fSlow107))));
		float fSlow109 = float(getValueBq4_gEarBalled(float(iSlow15)));
		float fSlow110 = (2.0f * fSlow109);
		float fSlow111 = float((iSlow15 >= 88));
		float fSlow112 = (2.32830644e-10f * fSlow111);
		float fSlow113 = (1.16415322e-10f * fSlow111);
		float fSlow114 = std::pow(10.0f, (fConst23 * float(getValuer3db(float(iSlow15)))));
		float fSlow115 = (std::cos((fConst21 * (fSlow13 * float(getValueThirdPartialFactor(float(iSlow15)))))) * (0.0f - (2.0f * fSlow114)));
		float fSlow116 = synth_piano_faustpower2_f(fSlow114);
		float fSlow117 = std::pow(10.0f, (fConst23 * float(getValuer2db(float(iSlow15)))));
		float fSlow118 = (std::cos((fConst21 * (fSlow13 * float(getValueSecondPartialFactor(float(iSlow15)))))) * (0.0f - (2.0f * fSlow117)));
		float fSlow119 = synth_piano_faustpower2_f(fSlow117);
		float fSlow120 = (fSlow103 * (0.0f - (2.0f * fSlow105)));
		float fSlow121 = synth_piano_faustpower2_f(fSlow105);
		float fSlow122 = (0.0f - (2.0f * fSlow106));
		float fSlow123 = synth_piano_faustpower2_f(fSlow106);
		float fSlow124 = ((fSlow104 * fSlow121) + (fSlow123 * fSlow107));
		float fSlow125 = std::exp((fConst28 / fSlow0));
		float fSlow126 = synth_piano_faustpower2_f(fSlow125);
		float fSlow127 = (1.0f - (fConst1 * fSlow126));
		float fSlow128 = (1.0f - fSlow126);
		float fSlow129 = (fSlow127 / fSlow128);
		float fSlow130 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow127) / synth_piano_faustpower2_f(fSlow128)) + -1.0f)));
		float fSlow131 = (fSlow129 - fSlow130);
		float fSlow132 = (fSlow125 * (fSlow130 + (1.0f - fSlow129)));
		float fSlow133 = ((std::exp((fConst29 / fSlow0)) / fSlow125) + -1.0f);
		float fSlow134 = (12.0f * fSlow10);
		int iSlow135 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst32 * (float(fHslider8) / fSlow13)))));
		float fSlow136 = std::exp((fConst36 / fSlow0));
		float fSlow137 = synth_piano_faustpower2_f(fSlow136);
		float fSlow138 = (1.0f - (fConst1 * fSlow137));
		float fSlow139 = (1.0f - fSlow137);
		float fSlow140 = (fSlow138 / fSlow139);
		float fSlow141 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow138) / synth_piano_faustpower2_f(fSlow139)) + -1.0f)));
		float fSlow142 = (fSlow140 - fSlow141);
		float fSlow143 = (fSlow136 * (fSlow141 + (1.0f - fSlow140)));
		float fSlow144 = ((std::exp((fConst37 / fSlow0)) / fSlow136) + -1.0f);
		float fSlow145 = std::exp((fConst43 / fSlow0));
		float fSlow146 = synth_piano_faustpower2_f(fSlow145);
		float fSlow147 = (1.0f - (fConst1 * fSlow146));
		float fSlow148 = (1.0f - fSlow146);
		float fSlow149 = (fSlow147 / fSlow148);
		float fSlow150 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow147) / synth_piano_faustpower2_f(fSlow148)) + -1.0f)));
		float fSlow151 = (fSlow149 - fSlow150);
		float fSlow152 = (fSlow145 * (fSlow150 + (1.0f - fSlow149)));
		float fSlow153 = ((std::exp((fConst44 / fSlow0)) / fSlow145) + -1.0f);
		float fSlow154 = std::exp((fConst50 / fSlow0));
		float fSlow155 = synth_piano_faustpower2_f(fSlow154);
		float fSlow156 = (1.0f - (fConst1 * fSlow155));
		float fSlow157 = (1.0f - fSlow155);
		float fSlow158 = (fSlow156 / fSlow157);
		float fSlow159 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow156) / synth_piano_faustpower2_f(fSlow157)) + -1.0f)));
		float fSlow160 = (fSlow158 - fSlow159);
		float fSlow161 = (fSlow154 * (fSlow159 + (1.0f - fSlow158)));
		float fSlow162 = ((std::exp((fConst51 / fSlow0)) / fSlow154) + -1.0f);
		float fSlow163 = std::exp((fConst57 / fSlow0));
		float fSlow164 = synth_piano_faustpower2_f(fSlow163);
		float fSlow165 = (1.0f - (fConst1 * fSlow164));
		float fSlow166 = (1.0f - fSlow164);
		float fSlow167 = (fSlow165 / fSlow166);
		float fSlow168 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow165) / synth_piano_faustpower2_f(fSlow166)) + -1.0f)));
		float fSlow169 = (fSlow167 - fSlow168);
		float fSlow170 = (fSlow163 * (fSlow168 + (1.0f - fSlow167)));
		float fSlow171 = ((std::exp((fConst58 / fSlow0)) / fSlow163) + -1.0f);
		float fSlow172 = std::exp((fConst64 / fSlow0));
		float fSlow173 = synth_piano_faustpower2_f(fSlow172);
		float fSlow174 = (1.0f - (fConst1 * fSlow173));
		float fSlow175 = (1.0f - fSlow173);
		float fSlow176 = (fSlow174 / fSlow175);
		float fSlow177 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow174) / synth_piano_faustpower2_f(fSlow175)) + -1.0f)));
		float fSlow178 = (fSlow176 - fSlow177);
		float fSlow179 = (fSlow172 * (fSlow177 + (1.0f - fSlow176)));
		float fSlow180 = ((std::exp((fConst65 / fSlow0)) / fSlow172) + -1.0f);
		float fSlow181 = std::exp((fConst71 / fSlow0));
		float fSlow182 = synth_piano_faustpower2_f(fSlow181);
		float fSlow183 = (1.0f - (fConst1 * fSlow182));
		float fSlow184 = (1.0f - fSlow182);
		float fSlow185 = (fSlow183 / fSlow184);
		float fSlow186 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow183) / synth_piano_faustpower2_f(fSlow184)) + -1.0f)));
		float fSlow187 = (fSlow185 - fSlow186);
		float fSlow188 = (fSlow181 * (fSlow186 + (1.0f - fSlow185)));
		float fSlow189 = ((std::exp((fConst72 / fSlow0)) / fSlow181) + -1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec11[0] = (0.0f - (fConst7 * ((fConst8 * fRec11[1]) - (fRec0[1] + fRec0[2]))));
			fRec10[0] = ((fSlow7 * fRec10[1]) + (fSlow8 * (fRec0[1] + (fSlow9 * fRec11[0]))));
			fVec0[(IOTA & 16383)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			fRec12[0] = (fSlow12 + (0.999000013f * fRec12[1]));
			fRec20[0] = (fSlow59 + (0.999000013f * fRec20[1]));
			iRec26[0] = ((1103515245 * iRec26[1]) + 12345);
			iRec28[0] = ((iSlow56 * iRec28[1]) + 1);
			float fTemp0 = float((iRec28[0] + -1));
			float fTemp1 = float(((fTemp0 < 2.0f) & iSlow67));
			float fTemp2 = ((0.0301973838f * fTemp1) + (fSlow68 * float(((fTemp0 >= 2.0f) | iSlow69))));
			fRec27[0] = ((fRec27[1] * fTemp2) + (0.150000006f * (fTemp1 * (1.0f - fTemp2))));
			int iTemp3 = (fTemp0 < fSlow73);
			float fTemp4 = ((fSlow72 * float(iTemp3)) + (fConst19 * float((iSlow56 * (fTemp0 >= fSlow73)))));
			fRec29[0] = ((fRec29[1] * (fSlow70 + fTemp4)) + (fSlow74 * (((1.0f - fTemp4) - fSlow70) * float((iTemp3 & iSlow67)))));
			float fTemp5 = (float(iRec26[0]) * (fRec27[0] + fRec29[0]));
			fRec25[0] = ((fSlow66 * fTemp5) + (fSlow75 * fRec25[1]));
			fRec24[0] = ((fSlow65 * fRec25[0]) + (fSlow75 * fRec24[1]));
			fRec23[0] = ((fSlow65 * fRec24[0]) + (fSlow75 * fRec23[1]));
			fRec22[0] = ((fSlow65 * fRec23[0]) + (fSlow75 * fRec22[1]));
			fRec21[0] = (((fSlow62 * fRec22[0]) + (fSlow76 * fRec22[1])) - (fSlow60 * fRec21[1]));
			float fTemp6 = (fRec20[0] * (fRec21[0] + fRec14[1]));
			fVec1[0] = fTemp6;
			fRec19[0] = (fVec1[1] + (fSlow77 * (fTemp6 - fRec19[1])));
			fRec18[0] = (fRec19[1] + (fSlow77 * (fRec19[0] - fRec18[1])));
			fRec17[(IOTA & 8191)] = (fRec18[1] + (fSlow77 * (fRec18[0] - fRec17[((IOTA - 1) & 8191)])));
			float fTemp7 = (fSlow55 * fRec17[((IOTA - iSlow81) & 8191)]);
			float fTemp8 = (fRec21[0] + (fRec20[0] * fRec15[1]));
			fVec2[0] = fTemp8;
			fRec33[0] = (fVec2[1] + (fSlow77 * (fTemp8 - fRec33[1])));
			fRec32[0] = (fRec33[1] + (fSlow77 * (fRec33[0] - fRec32[1])));
			fRec31[(IOTA & 8191)] = (fRec32[1] + (fSlow77 * (fRec32[0] - fRec31[((IOTA - 1) & 8191)])));
			float fTemp9 = (fSlow93 * fRec31[((IOTA - iSlow96) & 8191)]);
			float fTemp10 = (fSlow97 * fRec31[((IOTA - iSlow98) & 8191)]);
			float fTemp11 = (fSlow99 * fRec17[((IOTA - iSlow100) & 8191)]);
			float fTemp12 = (fTemp7 + ((fTemp9 + fTemp10) + fTemp11));
			fVec3[0] = fTemp12;
			fRec30[0] = (fSlow82 * ((2.0f * ((fSlow48 * fTemp12) + (fSlow42 * fVec3[1]))) - (fSlow41 * fRec30[1])));
			fRec14[0] = (fTemp7 + (fRec30[0] + fTemp11));
			fRec15[0] = (fTemp10 + (fRec30[0] + fTemp9));
			float fRec16 = fTemp12;
			fRec13[0] = ((fSlow19 * fRec16) - ((fSlow101 * fRec13[1]) + (fSlow102 * fRec13[2])));
			fVec4[0] = (fSlow112 * fTemp5);
			float fTemp13 = (0.0f - ((0.5f * fVec4[1]) + (fSlow113 * fTemp5)));
			fVec5[0] = fTemp13;
			fRec43[0] = (((fSlow62 * fTemp13) + (fSlow76 * fVec5[1])) - (fSlow60 * fRec43[1]));
			fRec42[0] = ((fSlow65 * fRec43[0]) + (fSlow75 * fRec42[1]));
			fRec41[0] = ((fSlow65 * fRec42[0]) + (fSlow75 * fRec41[1]));
			fRec40[0] = ((fSlow65 * fRec41[0]) + (fSlow75 * fRec40[1]));
			fRec39[0] = ((fSlow65 * fRec40[0]) + (fSlow75 * fRec39[1]));
			fRec38[0] = ((fSlow109 * (fRec39[0] - fRec39[1])) - ((fSlow115 * fRec38[1]) + (fSlow116 * fRec38[2])));
			fRec37[0] = ((fSlow110 * fRec38[0]) - ((fSlow118 * fRec37[1]) + (fSlow119 * fRec37[2])));
			fRec36[0] = (fRec37[0] - ((fSlow120 * fRec36[1]) + (fSlow121 * fRec36[2])));
			fRec35[0] = (((fSlow103 * ((fSlow108 * fRec36[1]) - (fSlow122 * fRec35[1]))) + (fRec36[0] + (fSlow124 * fRec36[2]))) - (fSlow123 * fRec35[2]));
			fRec34[0] = ((fSlow61 * fRec35[0]) - (fSlow60 * fRec34[1]));
			float fTemp14 = ((fSlow18 * (fRec13[0] - fRec13[2])) + (fRec16 + fRec34[0]));
			fVec6[(IOTA & 4095)] = fTemp14;
			fVec7[(IOTA & 4095)] = (fSlow11 * (fRec12[0] * fTemp14));
			float fTemp15 = (0.300000012f * fVec7[((IOTA - iConst24) & 4095)]);
			float fTemp16 = ((fVec0[((IOTA - iConst10) & 16383)] + fTemp15) - (0.600000024f * fRec8[1]));
			fVec8[(IOTA & 2047)] = fTemp16;
			fRec8[0] = fVec8[((IOTA - iConst25) & 2047)];
			float fRec9 = (0.600000024f * fTemp16);
			fRec47[0] = (0.0f - (fConst7 * ((fConst8 * fRec47[1]) - (fRec7[1] + fRec7[2]))));
			fRec46[0] = ((fSlow131 * fRec46[1]) + (fSlow132 * (fRec7[1] + (fSlow133 * fRec47[0]))));
			fVec9[(IOTA & 32767)] = ((0.353553385f * fRec46[0]) + 9.99999968e-21f);
			float fTemp17 = fVec6[((IOTA - iSlow135) & 4095)];
			fVec10[(IOTA & 4095)] = (fSlow134 * (fRec12[0] * fTemp17));
			float fTemp18 = (0.300000012f * fVec10[((IOTA - iConst24) & 4095)]);
			float fTemp19 = (((0.600000024f * fRec44[1]) + fVec9[((IOTA - iConst31) & 32767)]) - fTemp18);
			fVec11[(IOTA & 2047)] = fTemp19;
			fRec44[0] = fVec11[((IOTA - iConst33) & 2047)];
			float fRec45 = (0.0f - (0.600000024f * fTemp19));
			fRec51[0] = (0.0f - (fConst7 * ((fConst8 * fRec51[1]) - (fRec3[1] + fRec3[2]))));
			fRec50[0] = ((fSlow142 * fRec50[1]) + (fSlow143 * (fRec3[1] + (fSlow144 * fRec51[0]))));
			fVec12[(IOTA & 32767)] = ((0.353553385f * fRec50[0]) + 9.99999968e-21f);
			float fTemp20 = (((0.600000024f * fRec48[1]) + fVec12[((IOTA - iConst39) & 32767)]) - fTemp18);
			fVec13[(IOTA & 4095)] = fTemp20;
			fRec48[0] = fVec13[((IOTA - iConst40) & 4095)];
			float fRec49 = (0.0f - (0.600000024f * fTemp20));
			fRec55[0] = (0.0f - (fConst7 * ((fConst8 * fRec55[1]) - (fRec5[1] + fRec5[2]))));
			fRec54[0] = ((fSlow151 * fRec54[1]) + (fSlow152 * (fRec5[1] + (fSlow153 * fRec55[0]))));
			fVec14[(IOTA & 16383)] = ((0.353553385f * fRec54[0]) + 9.99999968e-21f);
			float fTemp21 = (fVec14[((IOTA - iConst46) & 16383)] + (fTemp18 + (0.600000024f * fRec52[1])));
			fVec15[(IOTA & 4095)] = fTemp21;
			fRec52[0] = fVec15[((IOTA - iConst47) & 4095)];
			float fRec53 = (0.0f - (0.600000024f * fTemp21));
			fRec59[0] = (0.0f - (fConst7 * ((fConst8 * fRec59[1]) - (fRec1[1] + fRec1[2]))));
			fRec58[0] = ((fSlow160 * fRec58[1]) + (fSlow161 * (fRec1[1] + (fSlow162 * fRec59[0]))));
			fVec16[(IOTA & 32767)] = ((0.353553385f * fRec58[0]) + 9.99999968e-21f);
			float fTemp22 = (fTemp18 + ((0.600000024f * fRec56[1]) + fVec16[((IOTA - iConst53) & 32767)]));
			fVec17[(IOTA & 4095)] = fTemp22;
			fRec56[0] = fVec17[((IOTA - iConst54) & 4095)];
			float fRec57 = (0.0f - (0.600000024f * fTemp22));
			fRec63[0] = (0.0f - (fConst7 * ((fConst8 * fRec63[1]) - (fRec6[1] + fRec6[2]))));
			fRec62[0] = ((fSlow169 * fRec62[1]) + (fSlow170 * (fRec6[1] + (fSlow171 * fRec63[0]))));
			fVec18[(IOTA & 16383)] = ((0.353553385f * fRec62[0]) + 9.99999968e-21f);
			float fTemp23 = (fVec18[((IOTA - iConst60) & 16383)] - (fTemp15 + (0.600000024f * fRec60[1])));
			fVec19[(IOTA & 2047)] = fTemp23;
			fRec60[0] = fVec19[((IOTA - iConst61) & 2047)];
			float fRec61 = (0.600000024f * fTemp23);
			fRec67[0] = (0.0f - (fConst7 * ((fConst8 * fRec67[1]) - (fRec2[1] + fRec2[2]))));
			fRec66[0] = ((fSlow178 * fRec66[1]) + (fSlow179 * (fRec2[1] + (fSlow180 * fRec67[0]))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec66[0]) + 9.99999968e-21f);
			float fTemp24 = (fVec20[((IOTA - iConst67) & 16383)] - (fTemp15 + (0.600000024f * fRec64[1])));
			fVec21[(IOTA & 4095)] = fTemp24;
			fRec64[0] = fVec21[((IOTA - iConst68) & 4095)];
			float fRec65 = (0.600000024f * fTemp24);
			fRec71[0] = (0.0f - (fConst7 * ((fConst8 * fRec71[1]) - (fRec4[1] + fRec4[2]))));
			fRec70[0] = ((fSlow187 * fRec70[1]) + (fSlow188 * (fRec4[1] + (fSlow189 * fRec71[0]))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec70[0]) + 9.99999968e-21f);
			float fTemp25 = ((fTemp15 + fVec22[((IOTA - iConst74) & 16383)]) - (0.600000024f * fRec68[1]));
			fVec23[(IOTA & 4095)] = fTemp25;
			fRec68[0] = fVec23[((IOTA - iConst75) & 4095)];
			float fRec69 = (0.600000024f * fTemp25);
			float fTemp26 = (fRec9 + fRec69);
			float fTemp27 = (fRec61 + (fRec65 + fTemp26));
			fRec0[0] = (fRec8[1] + (fRec44[1] + (fRec48[1] + (fRec52[1] + (fRec56[1] + (fRec60[1] + (fRec64[1] + (fRec68[1] + (fRec45 + (fRec49 + (fRec53 + (fRec57 + fTemp27))))))))))));
			fRec1[0] = ((fRec8[1] + (fRec60[1] + (fRec64[1] + (fRec68[1] + fTemp27)))) - (fRec44[1] + (fRec48[1] + (fRec52[1] + (fRec56[1] + (fRec45 + (fRec49 + (fRec57 + fRec53))))))));
			float fTemp28 = (fRec65 + fRec61);
			fRec2[0] = ((fRec8[1] + (fRec52[1] + (fRec56[1] + (fRec68[1] + (fRec53 + (fRec57 + fTemp26)))))) - (fRec44[1] + (fRec48[1] + (fRec60[1] + (fRec64[1] + (fRec45 + (fRec49 + fTemp28)))))));
			fRec3[0] = ((fRec8[1] + (fRec44[1] + (fRec48[1] + (fRec68[1] + (fRec45 + (fRec49 + fTemp26)))))) - (fRec52[1] + (fRec56[1] + (fRec60[1] + (fRec64[1] + (fRec53 + (fRec57 + fTemp28)))))));
			float fTemp29 = (fRec9 + fRec65);
			float fTemp30 = (fRec69 + fRec61);
			fRec4[0] = ((fRec8[1] + (fRec48[1] + (fRec56[1] + (fRec64[1] + (fRec49 + (fRec57 + fTemp29)))))) - (fRec44[1] + (fRec52[1] + (fRec60[1] + (fRec68[1] + (fRec45 + (fRec53 + fTemp30)))))));
			fRec5[0] = ((fRec8[1] + (fRec44[1] + (fRec52[1] + (fRec64[1] + (fRec45 + (fRec53 + fTemp29)))))) - (fRec48[1] + (fRec56[1] + (fRec60[1] + (fRec68[1] + (fRec49 + (fRec57 + fTemp30)))))));
			float fTemp31 = (fRec9 + fRec61);
			float fTemp32 = (fRec69 + fRec65);
			fRec6[0] = ((fRec8[1] + (fRec44[1] + (fRec56[1] + (fRec60[1] + (fRec45 + (fRec57 + fTemp31)))))) - (fRec48[1] + (fRec52[1] + (fRec64[1] + (fRec68[1] + (fRec49 + (fRec53 + fTemp32)))))));
			fRec7[0] = ((fRec8[1] + (fRec48[1] + (fRec52[1] + (fRec60[1] + (fRec49 + (fRec53 + fTemp31)))))) - (fRec44[1] + (fRec56[1] + (fRec64[1] + (fRec68[1] + (fRec45 + (fRec57 + fTemp32)))))));
			float fTemp33 = (1.0f - fRec12[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow11 * (fTemp33 * fTemp14))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow134 * (fTemp33 * fTemp17))));
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA = (IOTA + 1);
			fRec12[1] = fRec12[0];
			fRec20[1] = fRec20[0];
			iRec26[1] = iRec26[0];
			iRec28[1] = iRec28[0];
			fRec27[1] = fRec27[0];
			fRec29[1] = fRec29[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fVec1[1] = fVec1[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fVec2[1] = fVec2[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fVec3[1] = fVec3[0];
			fRec30[1] = fRec30[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec8[1] = fRec8[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec44[1] = fRec44[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fRec48[1] = fRec48[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec52[1] = fRec52[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fRec56[1] = fRec56[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec60[1] = fRec60[0];
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			fRec64[1] = fRec64[0];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec68[1] = fRec68[0];
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
