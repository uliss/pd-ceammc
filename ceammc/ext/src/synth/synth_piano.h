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
	float fRec19[2];
	FAUSTFLOAT fHslider6;
	int iRec25[2];
	int iRec27[2];
	float fConst15;
	FAUSTFLOAT fEntry0;
	float fRec26[2];
	float fConst16;
	float fConst17;
	FAUSTFLOAT fHslider7;
	float fConst18;
	float fConst19;
	float fRec28[2];
	float fRec24[2];
	float fRec23[2];
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fVec1[2];
	float fRec18[2];
	float fRec17[2];
	float fRec16[8192];
	float fVec2[2];
	float fRec29[2];
	float fRec14[2];
	float fConst20;
	float fConst21;
	float fConst22;
	float fRec13[3];
	float fRec41[2];
	float fRec40[2];
	float fRec39[2];
	float fRec38[2];
	float fRec37[2];
	float fVec3[2];
	float fRec36[2];
	float fRec35[2];
	float fRec34[8192];
	float fVec4[2];
	float fRec45[2];
	float fRec44[2];
	float fRec43[8192];
	float fVec5[2];
	float fRec42[2];
	float fRec31[2];
	float fRec32[2];
	float fRec30[3];
	float fConst23;
	float fVec6[2];
	float fVec7[2];
	float fRec55[2];
	float fRec54[2];
	float fRec53[2];
	float fRec52[2];
	float fRec51[2];
	float fRec50[3];
	float fRec49[3];
	float fRec48[3];
	float fRec47[3];
	float fRec46[2];
	float fVec8[4096];
	float fVec9[4096];
	int iConst24;
	float fVec10[2048];
	int iConst25;
	float fRec8[2];
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fRec59[2];
	float fRec58[2];
	float fVec11[16384];
	float fConst30;
	int iConst31;
	float fVec12[4096];
	int iConst32;
	float fRec56[2];
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fRec63[2];
	float fRec62[2];
	float fVec13[16384];
	float fConst37;
	int iConst38;
	float fVec14[4096];
	int iConst39;
	float fRec60[2];
	float fConst40;
	float fConst41;
	float fConst42;
	float fConst43;
	float fRec67[2];
	float fRec66[2];
	float fVec15[16384];
	float fConst44;
	int iConst45;
	float fVec16[2048];
	int iConst46;
	float fRec64[2];
	float fConst47;
	float fConst48;
	float fConst49;
	float fConst50;
	float fRec71[2];
	float fRec70[2];
	float fVec17[32768];
	float fConst51;
	int iConst52;
	float fConst53;
	FAUSTFLOAT fHslider8;
	float fVec18[4096];
	float fVec19[2048];
	int iConst54;
	float fRec68[2];
	float fConst55;
	float fConst56;
	float fConst57;
	float fConst58;
	float fRec75[2];
	float fRec74[2];
	float fVec20[32768];
	float fConst59;
	int iConst60;
	float fVec21[4096];
	int iConst61;
	float fRec72[2];
	float fConst62;
	float fConst63;
	float fConst64;
	float fConst65;
	float fRec79[2];
	float fRec78[2];
	float fVec22[16384];
	float fConst66;
	int iConst67;
	float fVec23[4096];
	int iConst68;
	float fRec76[2];
	float fConst69;
	float fConst70;
	float fConst71;
	float fConst72;
	float fRec83[2];
	float fRec82[2];
	float fVec24[32768];
	float fConst73;
	int iConst74;
	float fVec25[4096];
	int iConst75;
	float fRec80[2];
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
		fConst2 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst3 = ((0.0f - (6.90775537f * fConst2)) / fConst0);
		fConst4 = (0.25f * fConst3);
		fConst5 = (0.166666672f * fConst3);
		fConst6 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst7 = (1.0f / (fConst6 + 1.0f));
		fConst8 = (1.0f - fConst6);
		fConst9 = std::floor(((0.0134579996f * fConst0) + 0.5f));
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
		fConst26 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst27 = ((0.0f - (6.90775537f * fConst26)) / fConst0);
		fConst28 = (0.25f * fConst27);
		fConst29 = (0.166666672f * fConst27);
		fConst30 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst31 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst26 - fConst30))));
		iConst32 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst30 + -1.0f))));
		fConst33 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst34 = ((0.0f - (6.90775537f * fConst33)) / fConst0);
		fConst35 = (0.25f * fConst34);
		fConst36 = (0.166666672f * fConst34);
		fConst37 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst38 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst33 - fConst37))));
		iConst39 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst37 + -1.0f))));
		fConst40 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst41 = ((0.0f - (6.90775537f * fConst40)) / fConst0);
		fConst42 = (0.25f * fConst41);
		fConst43 = (0.166666672f * fConst41);
		fConst44 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst45 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst40 - fConst44))));
		iConst46 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst44 + -1.0f))));
		fConst47 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst48 = ((0.0f - (6.90775537f * fConst47)) / fConst0);
		fConst49 = (0.25f * fConst48);
		fConst50 = (0.166666672f * fConst48);
		fConst51 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst52 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst47 - fConst51))));
		fConst53 = (0.0011363636f * fConst0);
		iConst54 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst51 + -1.0f))));
		fConst55 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst56 = ((0.0f - (6.90775537f * fConst55)) / fConst0);
		fConst57 = (0.25f * fConst56);
		fConst58 = (0.166666672f * fConst56);
		fConst59 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst60 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst55 - fConst59))));
		iConst61 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst59 + -1.0f))));
		fConst62 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst63 = ((0.0f - (6.90775537f * fConst62)) / fConst0);
		fConst64 = (0.25f * fConst63);
		fConst65 = (0.166666672f * fConst63);
		fConst66 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst67 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst62 - fConst66))));
		iConst68 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst66 + -1.0f))));
		fConst69 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst70 = ((0.0f - (6.90775537f * fConst69)) / fConst0);
		fConst71 = (0.25f * fConst70);
		fConst72 = (0.166666672f * fConst70);
		fConst73 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst74 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst69 - fConst73))));
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
			fRec19[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec25[l5] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec27[l6] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec26[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec28[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec24[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec23[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec22[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec21[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec20[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fVec1[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec18[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec17[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 8192); l17 = (l17 + 1)) {
			fRec16[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fVec2[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec29[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec14[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec13[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec41[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec40[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec39[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec38[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec37[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fVec3[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec36[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec35[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 8192); l30 = (l30 + 1)) {
			fRec34[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fVec4[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec45[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec44[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 8192); l34 = (l34 + 1)) {
			fRec43[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fVec5[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec42[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec31[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec32[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 3); l39 = (l39 + 1)) {
			fRec30[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fVec6[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fVec7[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec55[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec54[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec53[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec52[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec51[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec50[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec49[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec48[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec47[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec46[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 4096); l52 = (l52 + 1)) {
			fVec8[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 4096); l53 = (l53 + 1)) {
			fVec9[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2048); l54 = (l54 + 1)) {
			fVec10[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec8[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec59[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec58[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 16384); l58 = (l58 + 1)) {
			fVec11[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 4096); l59 = (l59 + 1)) {
			fVec12[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec56[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec63[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec62[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 16384); l63 = (l63 + 1)) {
			fVec13[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 4096); l64 = (l64 + 1)) {
			fVec14[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec60[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec67[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec66[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 16384); l68 = (l68 + 1)) {
			fVec15[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2048); l69 = (l69 + 1)) {
			fVec16[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec64[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec71[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec70[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 32768); l73 = (l73 + 1)) {
			fVec17[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 4096); l74 = (l74 + 1)) {
			fVec18[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 2048); l75 = (l75 + 1)) {
			fVec19[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec68[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec75[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec74[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 32768); l79 = (l79 + 1)) {
			fVec20[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 4096); l80 = (l80 + 1)) {
			fVec21[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec72[l81] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec79[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec78[l83] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 16384); l84 = (l84 + 1)) {
			fVec22[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 4096); l85 = (l85 + 1)) {
			fVec23[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec76[l86] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec83[l87] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec82[l88] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l89 = 0; (l89 < 32768); l89 = (l89 + 1)) {
			fVec24[l89] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l90 = 0; (l90 < 4096); l90 = (l90 + 1)) {
			fVec25[l90] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec80[l91] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l92 = 0; (l92 < 3); l92 = (l92 + 1)) {
			fRec0[l92] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l93 = 0; (l93 < 3); l93 = (l93 + 1)) {
			fRec1[l93] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l94 = 0; (l94 < 3); l94 = (l94 + 1)) {
			fRec2[l94] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l95 = 0; (l95 < 3); l95 = (l95 + 1)) {
			fRec3[l95] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l96 = 0; (l96 < 3); l96 = (l96 + 1)) {
			fRec4[l96] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l97 = 0; (l97 < 3); l97 = (l97 + 1)) {
			fRec5[l97] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec6[l98] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec7[l99] = 0.0f;
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
		ui_interface->addHorizontalSlider("pitch", &fHslider3, 48.0f, 21.0f, 108.0f, 0.00100000005f);
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
		float fSlow13 = float(fHslider3);
		float fSlow14 = std::pow(2.0f, (0.0833333358f * (fSlow13 + -69.0f)));
		float fSlow15 = float(getValueEQBandWidthFactor(float(fSlow13)));
		float fSlow16 = (synth_piano_faustpower2_f(fSlow14) * synth_piano_faustpower2_f(fSlow15));
		float fSlow17 = (0.5f - (fConst12 * fSlow16));
		float fSlow18 = float(getValueEQGain(float(fSlow13)));
		float fSlow19 = float(fHslider4);
		float fSlow20 = float(getValueStiffnessCoefficient(float(fSlow13)));
		float fSlow21 = (13.6899996f * (synth_piano_faustpower2_f(fSlow19) * synth_piano_faustpower2_f(fSlow20)));
		float fSlow22 = (fSlow21 + -1.0f);
		float fSlow23 = (440.0f * fSlow14);
		float fSlow24 = (5.0f * (float(getValueDetuningHz(float(fSlow13))) * float(fHslider5)));
		float fSlow25 = (fSlow23 + fSlow24);
		float fSlow26 = (fConst14 * fSlow25);
		float fSlow27 = std::sin(fSlow26);
		float fSlow28 = (fSlow19 * fSlow20);
		float fSlow29 = (7.4000001f * fSlow28);
		float fSlow30 = (fSlow21 + 1.0f);
		float fSlow31 = std::cos(fSlow26);
		float fSlow32 = (3.0f * std::atan2((fSlow22 * fSlow27), (fSlow29 + (fSlow30 * fSlow31))));
		float fSlow33 = float(getValueSingleStringZero(float(fSlow13)));
		float fSlow34 = std::pow(10.0f, (0.000113636364f * (float(getValueSingleStringDecayRate(float(fSlow13))) / fSlow14)));
		float fSlow35 = float(getValueSingleStringPole(float(fSlow13)));
		float fSlow36 = (1.0f - fSlow35);
		float fSlow37 = ((fSlow33 * fSlow34) * fSlow36);
		float fSlow38 = (1.0f - fSlow33);
		float fSlow39 = (fSlow35 * fSlow38);
		float fSlow40 = (3.0f * fSlow39);
		float fSlow41 = (fSlow37 - fSlow40);
		float fSlow42 = (fSlow39 - fSlow37);
		float fSlow43 = (4.0f * fSlow42);
		float fSlow44 = (fSlow41 + fSlow43);
		float fSlow45 = (fSlow34 * fSlow36);
		float fSlow46 = ((3.0f * fSlow38) - fSlow45);
		float fSlow47 = (((fSlow31 * fSlow41) / fSlow46) + 1.0f);
		float fSlow48 = ((fSlow33 + fSlow45) + -1.0f);
		float fSlow49 = (4.0f * fSlow48);
		float fSlow50 = ((((fSlow31 * fSlow44) + fSlow49) / fSlow46) + 1.0f);
		float fSlow51 = (synth_piano_faustpower2_f(fSlow27) * fSlow41);
		float fSlow52 = synth_piano_faustpower2_f(fSlow46);
		float fSlow53 = (fConst13 * (((fSlow32 + std::atan2((((0.0f - ((fSlow27 * fSlow44) / fSlow46)) * fSlow47) + (((fSlow27 * fSlow41) * fSlow50) / fSlow46)), ((fSlow47 * fSlow50) + ((fSlow51 * fSlow44) / fSlow52)))) + 6.28318548f) / fSlow25));
		float fSlow54 = std::floor(fSlow53);
		float fSlow55 = (fSlow53 - fSlow54);
		int iSlow56 = (float(fButton0) > 0.0f);
		float fSlow57 = float(iSlow56);
		float fSlow58 = float((0 - (iSlow56 + -1)));
		float fSlow59 = (0.00100000005f * ((0.999599993f * fSlow57) + (0.899999976f * (float(getValueReleaseLoopGain(float(fSlow13))) * fSlow58))));
		float fSlow60 = float(getValueDCBa1(float(fSlow13)));
		float fSlow61 = (1.0f - fSlow60);
		float fSlow62 = (0.5f * fSlow61);
		float fSlow63 = (0.25f * float(fHslider6));
		float fSlow64 = float(getValueLoudPole(float(fSlow13)));
		float fSlow65 = (float(getValueLoudGain(float(fSlow13))) * (fSlow63 + (0.980000019f - fSlow64)));
		float fSlow66 = (2.09547579e-09f * (fSlow65 * float((fSlow13 < 45.0f))));
		int iSlow67 = (iSlow56 > 0);
		float fSlow68 = std::exp((0.0f - (fConst15 / (float(getValueDryTapAmpT60(float(fSlow13))) * float(fEntry0)))));
		int iSlow69 = (iSlow56 < 1);
		float fSlow70 = (fConst16 * fSlow58);
		float fSlow71 = float(fHslider7);
		float fSlow72 = (fSlow57 * std::exp((0.0f - (fConst17 / fSlow71))));
		float fSlow73 = (fConst18 * fSlow71);
		float fSlow74 = (0.200000003f * float(getValueSustainPedalLevel(float(fSlow13))));
		float fSlow75 = (fSlow64 + (0.0199999996f - fSlow63));
		float fSlow76 = (0.0f - fSlow62);
		float fSlow77 = (3.70000005f * fSlow28);
		float fSlow78 = ((fSlow37 + fSlow43) - fSlow40);
		float fSlow79 = (((fSlow49 + (fSlow31 * fSlow78)) / fSlow46) + 1.0f);
		int iSlow80 = int((fConst13 * (((fSlow32 + std::atan2((0.0f - ((fSlow27 * ((fSlow47 * fSlow78) - (fSlow41 * fSlow79))) / fSlow46)), ((fSlow47 * fSlow79) + ((fSlow51 * fSlow78) / fSlow52)))) + 6.28318548f) / fSlow25)));
		int iSlow81 = std::min<int>(4097, std::max<int>(0, (iSlow80 + 1)));
		float fSlow82 = (1.0f / fSlow46);
		float fSlow83 = (fSlow54 + (1.0f - fSlow53));
		int iSlow84 = std::min<int>(4097, std::max<int>(0, iSlow80));
		float fSlow85 = ((0.0f - (fConst20 * (fSlow14 * fSlow15))) * std::cos((fConst21 * (fSlow14 / float(getValueStrikePosition(float(fSlow13)))))));
		float fSlow86 = (fConst22 * fSlow16);
		float fSlow87 = (1.39698386e-09f * (fSlow65 * float(((fSlow13 >= 45.0f) & (fSlow13 < 88.0f)))));
		float fSlow88 = (fSlow23 - fSlow24);
		float fSlow89 = (fConst14 * fSlow88);
		float fSlow90 = std::sin(fSlow89);
		float fSlow91 = std::cos(fSlow89);
		float fSlow92 = (3.0f * std::atan2((fSlow22 * fSlow90), (fSlow29 + (fSlow30 * fSlow91))));
		float fSlow93 = (((fSlow91 * fSlow41) / fSlow46) + 1.0f);
		float fSlow94 = (((fSlow49 + (fSlow91 * fSlow44)) / fSlow46) + 1.0f);
		float fSlow95 = (synth_piano_faustpower2_f(fSlow90) * fSlow41);
		float fSlow96 = (fConst13 * (((fSlow92 + std::atan2((((0.0f - ((fSlow90 * fSlow44) / fSlow46)) * fSlow93) + (((fSlow90 * fSlow41) * fSlow94) / fSlow46)), ((fSlow93 * fSlow94) + ((fSlow95 * fSlow44) / fSlow52)))) + 6.28318548f) / fSlow88));
		float fSlow97 = std::floor(fSlow96);
		float fSlow98 = (fSlow97 + (1.0f - fSlow96));
		float fSlow99 = (((fSlow49 + (fSlow91 * fSlow78)) / fSlow46) + 1.0f);
		int iSlow100 = int((fConst13 * (((fSlow92 + std::atan2((0.0f - ((fSlow90 * ((fSlow78 * fSlow93) - (fSlow41 * fSlow99))) / fSlow46)), ((fSlow93 * fSlow99) + ((fSlow95 * fSlow78) / fSlow52)))) + 6.28318548f) / fSlow88)));
		int iSlow101 = std::min<int>(4097, std::max<int>(0, iSlow100));
		float fSlow102 = (fSlow96 - fSlow97);
		int iSlow103 = std::min<int>(4097, std::max<int>(0, (iSlow100 + 1)));
		float fSlow104 = std::cos((fConst21 * fSlow14));
		float fSlow105 = std::pow(10.0f, (0.0500000007f * float(getValueSecondStageAmpRatio(float(fSlow13)))));
		float fSlow106 = std::pow(10.0f, (fConst23 * float(getValuer1_1db(float(fSlow13)))));
		float fSlow107 = std::pow(10.0f, (fConst23 * float(getValuer1_2db(float(fSlow13)))));
		float fSlow108 = (1.0f - fSlow105);
		float fSlow109 = (0.0f - (2.0f * ((fSlow105 * fSlow106) + (fSlow107 * fSlow108))));
		float fSlow110 = float(getValueBq4_gEarBalled(float(fSlow13)));
		float fSlow111 = (2.0f * fSlow110);
		float fSlow112 = float((fSlow13 >= 88.0f));
		float fSlow113 = (2.32830644e-10f * fSlow112);
		float fSlow114 = (1.16415322e-10f * fSlow112);
		float fSlow115 = std::pow(10.0f, (fConst23 * float(getValuer3db(float(fSlow13)))));
		float fSlow116 = (std::cos((fConst21 * (fSlow14 * float(getValueThirdPartialFactor(float(fSlow13)))))) * (0.0f - (2.0f * fSlow115)));
		float fSlow117 = synth_piano_faustpower2_f(fSlow115);
		float fSlow118 = std::pow(10.0f, (fConst23 * float(getValuer2db(float(fSlow13)))));
		float fSlow119 = (std::cos((fConst21 * (fSlow14 * float(getValueSecondPartialFactor(float(fSlow13)))))) * (0.0f - (2.0f * fSlow118)));
		float fSlow120 = synth_piano_faustpower2_f(fSlow118);
		float fSlow121 = (fSlow104 * (0.0f - (2.0f * fSlow106)));
		float fSlow122 = synth_piano_faustpower2_f(fSlow106);
		float fSlow123 = (0.0f - (2.0f * fSlow107));
		float fSlow124 = synth_piano_faustpower2_f(fSlow107);
		float fSlow125 = ((fSlow105 * fSlow122) + (fSlow124 * fSlow108));
		float fSlow126 = std::exp((fConst28 / fSlow0));
		float fSlow127 = synth_piano_faustpower2_f(fSlow126);
		float fSlow128 = (1.0f - (fConst1 * fSlow127));
		float fSlow129 = (1.0f - fSlow127);
		float fSlow130 = (fSlow128 / fSlow129);
		float fSlow131 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow128) / synth_piano_faustpower2_f(fSlow129)) + -1.0f)));
		float fSlow132 = (fSlow130 - fSlow131);
		float fSlow133 = (fSlow126 * (fSlow131 + (1.0f - fSlow130)));
		float fSlow134 = ((std::exp((fConst29 / fSlow0)) / fSlow126) + -1.0f);
		float fSlow135 = std::exp((fConst35 / fSlow0));
		float fSlow136 = synth_piano_faustpower2_f(fSlow135);
		float fSlow137 = (1.0f - (fConst1 * fSlow136));
		float fSlow138 = (1.0f - fSlow136);
		float fSlow139 = (fSlow137 / fSlow138);
		float fSlow140 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow137) / synth_piano_faustpower2_f(fSlow138)) + -1.0f)));
		float fSlow141 = (fSlow139 - fSlow140);
		float fSlow142 = (fSlow135 * (fSlow140 + (1.0f - fSlow139)));
		float fSlow143 = ((std::exp((fConst36 / fSlow0)) / fSlow135) + -1.0f);
		float fSlow144 = std::exp((fConst42 / fSlow0));
		float fSlow145 = synth_piano_faustpower2_f(fSlow144);
		float fSlow146 = (1.0f - (fConst1 * fSlow145));
		float fSlow147 = (1.0f - fSlow145);
		float fSlow148 = (fSlow146 / fSlow147);
		float fSlow149 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow146) / synth_piano_faustpower2_f(fSlow147)) + -1.0f)));
		float fSlow150 = (fSlow148 - fSlow149);
		float fSlow151 = (fSlow144 * (fSlow149 + (1.0f - fSlow148)));
		float fSlow152 = ((std::exp((fConst43 / fSlow0)) / fSlow144) + -1.0f);
		float fSlow153 = std::exp((fConst49 / fSlow0));
		float fSlow154 = synth_piano_faustpower2_f(fSlow153);
		float fSlow155 = (1.0f - (fConst1 * fSlow154));
		float fSlow156 = (1.0f - fSlow154);
		float fSlow157 = (fSlow155 / fSlow156);
		float fSlow158 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow155) / synth_piano_faustpower2_f(fSlow156)) + -1.0f)));
		float fSlow159 = (fSlow157 - fSlow158);
		float fSlow160 = (fSlow153 * (fSlow158 + (1.0f - fSlow157)));
		float fSlow161 = ((std::exp((fConst50 / fSlow0)) / fSlow153) + -1.0f);
		float fSlow162 = (12.0f * fSlow10);
		int iSlow163 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst53 * (float(fHslider8) / fSlow14)))));
		float fSlow164 = std::exp((fConst57 / fSlow0));
		float fSlow165 = synth_piano_faustpower2_f(fSlow164);
		float fSlow166 = (1.0f - (fConst1 * fSlow165));
		float fSlow167 = (1.0f - fSlow165);
		float fSlow168 = (fSlow166 / fSlow167);
		float fSlow169 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow166) / synth_piano_faustpower2_f(fSlow167)) + -1.0f)));
		float fSlow170 = (fSlow168 - fSlow169);
		float fSlow171 = (fSlow164 * (fSlow169 + (1.0f - fSlow168)));
		float fSlow172 = ((std::exp((fConst58 / fSlow0)) / fSlow164) + -1.0f);
		float fSlow173 = std::exp((fConst64 / fSlow0));
		float fSlow174 = synth_piano_faustpower2_f(fSlow173);
		float fSlow175 = (1.0f - (fConst1 * fSlow174));
		float fSlow176 = (1.0f - fSlow174);
		float fSlow177 = (fSlow175 / fSlow176);
		float fSlow178 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow175) / synth_piano_faustpower2_f(fSlow176)) + -1.0f)));
		float fSlow179 = (fSlow177 - fSlow178);
		float fSlow180 = (fSlow173 * (fSlow178 + (1.0f - fSlow177)));
		float fSlow181 = ((std::exp((fConst65 / fSlow0)) / fSlow173) + -1.0f);
		float fSlow182 = std::exp((fConst71 / fSlow0));
		float fSlow183 = synth_piano_faustpower2_f(fSlow182);
		float fSlow184 = (1.0f - (fConst1 * fSlow183));
		float fSlow185 = (1.0f - fSlow183);
		float fSlow186 = (fSlow184 / fSlow185);
		float fSlow187 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow184) / synth_piano_faustpower2_f(fSlow185)) + -1.0f)));
		float fSlow188 = (fSlow186 - fSlow187);
		float fSlow189 = (fSlow182 * (fSlow187 + (1.0f - fSlow186)));
		float fSlow190 = ((std::exp((fConst72 / fSlow0)) / fSlow182) + -1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec11[0] = (0.0f - (fConst7 * ((fConst8 * fRec11[1]) - (fRec6[1] + fRec6[2]))));
			fRec10[0] = ((fSlow7 * fRec10[1]) + (fSlow8 * (fRec6[1] + (fSlow9 * fRec11[0]))));
			fVec0[(IOTA & 16383)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			fRec12[0] = (fSlow12 + (0.999000013f * fRec12[1]));
			fRec19[0] = (fSlow59 + (0.999000013f * fRec19[1]));
			iRec25[0] = ((1103515245 * iRec25[1]) + 12345);
			iRec27[0] = ((iSlow56 * iRec27[1]) + 1);
			float fTemp0 = float((iRec27[0] + -1));
			float fTemp1 = float(((fTemp0 < 2.0f) & iSlow67));
			float fTemp2 = ((0.0301973838f * fTemp1) + (fSlow68 * float(((fTemp0 >= 2.0f) | iSlow69))));
			fRec26[0] = ((fRec26[1] * fTemp2) + (0.150000006f * (fTemp1 * (1.0f - fTemp2))));
			int iTemp3 = (fTemp0 < fSlow73);
			float fTemp4 = ((fSlow72 * float(iTemp3)) + (fConst19 * float((iSlow56 * (fTemp0 >= fSlow73)))));
			fRec28[0] = ((fRec28[1] * (fSlow70 + fTemp4)) + (fSlow74 * (((1.0f - fTemp4) - fSlow70) * float((iTemp3 & iSlow67)))));
			float fTemp5 = (float(iRec25[0]) * (fRec26[0] + fRec28[0]));
			fRec24[0] = ((fSlow66 * fTemp5) + (fSlow75 * fRec24[1]));
			fRec23[0] = ((fSlow65 * fRec24[0]) + (fSlow75 * fRec23[1]));
			fRec22[0] = ((fSlow65 * fRec23[0]) + (fSlow75 * fRec22[1]));
			fRec21[0] = ((fSlow65 * fRec22[0]) + (fSlow75 * fRec21[1]));
			fRec20[0] = (((fSlow62 * fRec21[0]) + (fSlow76 * fRec21[1])) - (fSlow60 * fRec20[1]));
			float fTemp6 = (fRec19[0] * (fRec20[0] + fRec14[1]));
			fVec1[0] = fTemp6;
			fRec18[0] = (fVec1[1] + (fSlow77 * (fTemp6 - fRec18[1])));
			fRec17[0] = (fRec18[1] + (fSlow77 * (fRec18[0] - fRec17[1])));
			fRec16[(IOTA & 8191)] = (fRec17[1] + (fSlow77 * (fRec17[0] - fRec16[((IOTA - 1) & 8191)])));
			float fTemp7 = (fSlow55 * fRec16[((IOTA - iSlow81) & 8191)]);
			float fTemp8 = (fSlow83 * fRec16[((IOTA - iSlow84) & 8191)]);
			float fTemp9 = (fTemp8 + fTemp7);
			fVec2[0] = fTemp9;
			fRec29[0] = (fSlow82 * ((2.0f * ((fSlow48 * fTemp9) + (fSlow42 * fVec2[1]))) - (fSlow41 * fRec29[1])));
			fRec14[0] = (fTemp7 + (fRec29[0] + fTemp8));
			float fRec15 = fTemp9;
			fRec13[0] = ((fSlow18 * fRec15) - ((fSlow85 * fRec13[1]) + (fSlow86 * fRec13[2])));
			fRec41[0] = ((fSlow87 * fTemp5) + (fSlow75 * fRec41[1]));
			fRec40[0] = ((fSlow65 * fRec41[0]) + (fSlow75 * fRec40[1]));
			fRec39[0] = ((fSlow65 * fRec40[0]) + (fSlow75 * fRec39[1]));
			fRec38[0] = ((fSlow65 * fRec39[0]) + (fSlow75 * fRec38[1]));
			fRec37[0] = (((fSlow62 * fRec38[0]) + (fSlow76 * fRec38[1])) - (fSlow60 * fRec37[1]));
			float fTemp10 = (fRec19[0] * (fRec37[0] + fRec31[1]));
			fVec3[0] = fTemp10;
			fRec36[0] = (fVec3[1] + (fSlow77 * (fTemp10 - fRec36[1])));
			fRec35[0] = (fRec36[1] + (fSlow77 * (fRec36[0] - fRec35[1])));
			fRec34[(IOTA & 8191)] = (fRec35[1] + (fSlow77 * (fRec35[0] - fRec34[((IOTA - 1) & 8191)])));
			float fTemp11 = (fSlow55 * fRec34[((IOTA - iSlow81) & 8191)]);
			float fTemp12 = (fRec37[0] + (fRec19[0] * fRec32[1]));
			fVec4[0] = fTemp12;
			fRec45[0] = (fVec4[1] + (fSlow77 * (fTemp12 - fRec45[1])));
			fRec44[0] = (fRec45[1] + (fSlow77 * (fRec45[0] - fRec44[1])));
			fRec43[(IOTA & 8191)] = (fRec44[1] + (fSlow77 * (fRec44[0] - fRec43[((IOTA - 1) & 8191)])));
			float fTemp13 = (fSlow98 * fRec43[((IOTA - iSlow101) & 8191)]);
			float fTemp14 = (fSlow102 * fRec43[((IOTA - iSlow103) & 8191)]);
			float fTemp15 = (fSlow83 * fRec34[((IOTA - iSlow84) & 8191)]);
			float fTemp16 = (fTemp11 + ((fTemp13 + fTemp14) + fTemp15));
			fVec5[0] = fTemp16;
			fRec42[0] = (fSlow82 * ((2.0f * ((fSlow48 * fTemp16) + (fSlow42 * fVec5[1]))) - (fSlow41 * fRec42[1])));
			fRec31[0] = (fTemp11 + (fRec42[0] + fTemp15));
			fRec32[0] = (fTemp14 + (fRec42[0] + fTemp13));
			float fRec33 = fTemp16;
			fRec30[0] = ((fSlow18 * fRec33) - ((fSlow85 * fRec30[1]) + (fSlow86 * fRec30[2])));
			fVec6[0] = (fSlow113 * fTemp5);
			float fTemp17 = (0.0f - ((0.5f * fVec6[1]) + (fSlow114 * fTemp5)));
			fVec7[0] = fTemp17;
			fRec55[0] = (((fSlow62 * fTemp17) + (fSlow76 * fVec7[1])) - (fSlow60 * fRec55[1]));
			fRec54[0] = ((fSlow65 * fRec55[0]) + (fSlow75 * fRec54[1]));
			fRec53[0] = ((fSlow65 * fRec54[0]) + (fSlow75 * fRec53[1]));
			fRec52[0] = ((fSlow65 * fRec53[0]) + (fSlow75 * fRec52[1]));
			fRec51[0] = ((fSlow65 * fRec52[0]) + (fSlow75 * fRec51[1]));
			fRec50[0] = ((fSlow110 * (fRec51[0] - fRec51[1])) - ((fSlow116 * fRec50[1]) + (fSlow117 * fRec50[2])));
			fRec49[0] = ((fSlow111 * fRec50[0]) - ((fSlow119 * fRec49[1]) + (fSlow120 * fRec49[2])));
			fRec48[0] = (fRec49[0] - ((fSlow121 * fRec48[1]) + (fSlow122 * fRec48[2])));
			fRec47[0] = (((fSlow104 * ((fSlow109 * fRec48[1]) - (fSlow123 * fRec47[1]))) + (fRec48[0] + (fSlow125 * fRec48[2]))) - (fSlow124 * fRec47[2]));
			fRec46[0] = ((fSlow61 * fRec47[0]) - (fSlow60 * fRec46[1]));
			float fTemp18 = ((fSlow17 * ((fRec13[0] - fRec13[2]) + (fRec30[0] - fRec30[2]))) + (fRec46[0] + (fRec15 + fRec33)));
			fVec8[(IOTA & 4095)] = fTemp18;
			fVec9[(IOTA & 4095)] = (fSlow11 * (fRec12[0] * fTemp18));
			float fTemp19 = (0.300000012f * fVec9[((IOTA - iConst24) & 4095)]);
			float fTemp20 = (fVec0[((IOTA - iConst10) & 16383)] - (fTemp19 + (0.600000024f * fRec8[1])));
			fVec10[(IOTA & 2047)] = fTemp20;
			fRec8[0] = fVec10[((IOTA - iConst25) & 2047)];
			float fRec9 = (0.600000024f * fTemp20);
			fRec59[0] = (0.0f - (fConst7 * ((fConst8 * fRec59[1]) - (fRec2[1] + fRec2[2]))));
			fRec58[0] = ((fSlow132 * fRec58[1]) + (fSlow133 * (fRec2[1] + (fSlow134 * fRec59[0]))));
			fVec11[(IOTA & 16383)] = ((0.353553385f * fRec58[0]) + 9.99999968e-21f);
			float fTemp21 = (fVec11[((IOTA - iConst31) & 16383)] - (fTemp19 + (0.600000024f * fRec56[1])));
			fVec12[(IOTA & 4095)] = fTemp21;
			fRec56[0] = fVec12[((IOTA - iConst32) & 4095)];
			float fRec57 = (0.600000024f * fTemp21);
			fRec63[0] = (0.0f - (fConst7 * ((fConst8 * fRec63[1]) - (fRec4[1] + fRec4[2]))));
			fRec62[0] = ((fSlow141 * fRec62[1]) + (fSlow142 * (fRec4[1] + (fSlow143 * fRec63[0]))));
			fVec13[(IOTA & 16383)] = ((0.353553385f * fRec62[0]) + 9.99999968e-21f);
			float fTemp22 = ((fTemp19 + fVec13[((IOTA - iConst38) & 16383)]) - (0.600000024f * fRec60[1]));
			fVec14[(IOTA & 4095)] = fTemp22;
			fRec60[0] = fVec14[((IOTA - iConst39) & 4095)];
			float fRec61 = (0.600000024f * fTemp22);
			fRec67[0] = (0.0f - (fConst7 * ((fConst8 * fRec67[1]) - (fRec0[1] + fRec0[2]))));
			fRec66[0] = ((fSlow150 * fRec66[1]) + (fSlow151 * (fRec0[1] + (fSlow152 * fRec67[0]))));
			fVec15[(IOTA & 16383)] = ((0.353553385f * fRec66[0]) + 9.99999968e-21f);
			float fTemp23 = ((fVec15[((IOTA - iConst45) & 16383)] + fTemp19) - (0.600000024f * fRec64[1]));
			fVec16[(IOTA & 2047)] = fTemp23;
			fRec64[0] = fVec16[((IOTA - iConst46) & 2047)];
			float fRec65 = (0.600000024f * fTemp23);
			fRec71[0] = (0.0f - (fConst7 * ((fConst8 * fRec71[1]) - (fRec7[1] + fRec7[2]))));
			fRec70[0] = ((fSlow159 * fRec70[1]) + (fSlow160 * (fRec7[1] + (fSlow161 * fRec71[0]))));
			fVec17[(IOTA & 32767)] = ((0.353553385f * fRec70[0]) + 9.99999968e-21f);
			float fTemp24 = fVec8[((IOTA - iSlow163) & 4095)];
			fVec18[(IOTA & 4095)] = (fSlow162 * (fRec12[0] * fTemp24));
			float fTemp25 = (0.300000012f * fVec18[((IOTA - iConst24) & 4095)]);
			float fTemp26 = (((0.600000024f * fRec68[1]) + fVec17[((IOTA - iConst52) & 32767)]) - fTemp25);
			fVec19[(IOTA & 2047)] = fTemp26;
			fRec68[0] = fVec19[((IOTA - iConst54) & 2047)];
			float fRec69 = (0.0f - (0.600000024f * fTemp26));
			fRec75[0] = (0.0f - (fConst7 * ((fConst8 * fRec75[1]) - (fRec3[1] + fRec3[2]))));
			fRec74[0] = ((fSlow170 * fRec74[1]) + (fSlow171 * (fRec3[1] + (fSlow172 * fRec75[0]))));
			fVec20[(IOTA & 32767)] = ((0.353553385f * fRec74[0]) + 9.99999968e-21f);
			float fTemp27 = (((0.600000024f * fRec72[1]) + fVec20[((IOTA - iConst60) & 32767)]) - fTemp25);
			fVec21[(IOTA & 4095)] = fTemp27;
			fRec72[0] = fVec21[((IOTA - iConst61) & 4095)];
			float fRec73 = (0.0f - (0.600000024f * fTemp27));
			fRec79[0] = (0.0f - (fConst7 * ((fConst8 * fRec79[1]) - (fRec5[1] + fRec5[2]))));
			fRec78[0] = ((fSlow179 * fRec78[1]) + (fSlow180 * (fRec5[1] + (fSlow181 * fRec79[0]))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec78[0]) + 9.99999968e-21f);
			float fTemp28 = (fVec22[((IOTA - iConst67) & 16383)] + (fTemp25 + (0.600000024f * fRec76[1])));
			fVec23[(IOTA & 4095)] = fTemp28;
			fRec76[0] = fVec23[((IOTA - iConst68) & 4095)];
			float fRec77 = (0.0f - (0.600000024f * fTemp28));
			fRec83[0] = (0.0f - (fConst7 * ((fConst8 * fRec83[1]) - (fRec1[1] + fRec1[2]))));
			fRec82[0] = ((fSlow188 * fRec82[1]) + (fSlow189 * (fRec1[1] + (fSlow190 * fRec83[0]))));
			fVec24[(IOTA & 32767)] = ((0.353553385f * fRec82[0]) + 9.99999968e-21f);
			float fTemp29 = (fTemp25 + ((0.600000024f * fRec80[1]) + fVec24[((IOTA - iConst74) & 32767)]));
			fVec25[(IOTA & 4095)] = fTemp29;
			fRec80[0] = fVec25[((IOTA - iConst75) & 4095)];
			float fRec81 = (0.0f - (0.600000024f * fTemp29));
			float fTemp30 = (fRec80[1] + fRec76[1]);
			float fTemp31 = (fRec68[1] + (fTemp30 + fRec72[1]));
			fRec0[0] = (fRec8[1] + (fRec56[1] + (fRec60[1] + (fRec64[1] + (fRec69 + (fRec73 + (fRec77 + (fRec81 + (fRec9 + (fRec57 + (fRec61 + (fRec65 + fTemp31))))))))))));
			fRec1[0] = ((fRec8[1] + (fRec56[1] + (fRec60[1] + (fRec64[1] + (fRec9 + (fRec57 + (fRec65 + fRec61))))))) - (fRec69 + (fRec73 + (fRec77 + (fRec81 + fTemp31)))));
			float fTemp32 = (fRec72[1] + fRec68[1]);
			fRec2[0] = ((fRec60[1] + (fRec64[1] + (fRec77 + (fRec81 + (fRec61 + (fRec65 + fTemp30)))))) - (fRec8[1] + (fRec56[1] + (fRec69 + (fRec73 + (fRec9 + (fRec57 + fTemp32)))))));
			fRec3[0] = ((fRec60[1] + (fRec64[1] + (fRec69 + (fRec73 + (fRec61 + (fRec65 + fTemp32)))))) - (fRec8[1] + (fRec56[1] + (fRec77 + (fRec81 + (fRec9 + (fRec57 + fTemp30)))))));
			float fTemp33 = (fRec80[1] + fRec72[1]);
			float fTemp34 = (fRec76[1] + fRec68[1]);
			fRec4[0] = ((fRec56[1] + (fRec64[1] + (fRec73 + (fRec81 + (fRec57 + (fRec65 + fTemp33)))))) - (fRec8[1] + (fRec60[1] + (fRec69 + (fRec77 + (fRec9 + (fRec61 + fTemp34)))))));
			fRec5[0] = ((fRec56[1] + (fRec64[1] + (fRec69 + (fRec77 + (fRec57 + (fRec65 + fTemp34)))))) - (fRec8[1] + (fRec60[1] + (fRec73 + (fRec81 + (fRec9 + (fRec61 + fTemp33)))))));
			float fTemp35 = (fRec80[1] + fRec68[1]);
			float fTemp36 = (fRec76[1] + fRec72[1]);
			fRec6[0] = ((fRec8[1] + (fRec64[1] + (fRec69 + (fRec81 + (fRec9 + (fRec65 + fTemp35)))))) - (fRec56[1] + (fRec60[1] + (fRec73 + (fRec77 + (fRec57 + (fRec61 + fTemp36)))))));
			fRec7[0] = ((fRec8[1] + (fRec64[1] + (fRec73 + (fRec77 + (fRec9 + (fRec65 + fTemp36)))))) - (fRec56[1] + (fRec60[1] + (fRec69 + (fRec81 + (fRec57 + (fRec61 + fTemp35)))))));
			float fTemp37 = (1.0f - fRec12[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow11 * (fTemp37 * fTemp18))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow162 * (fTemp37 * fTemp24))));
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA = (IOTA + 1);
			fRec12[1] = fRec12[0];
			fRec19[1] = fRec19[0];
			iRec25[1] = iRec25[0];
			iRec27[1] = iRec27[0];
			fRec26[1] = fRec26[0];
			fRec28[1] = fRec28[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fVec1[1] = fVec1[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fVec2[1] = fVec2[0];
			fRec29[1] = fRec29[0];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fVec3[1] = fVec3[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fVec4[1] = fVec4[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fVec5[1] = fVec5[0];
			fRec42[1] = fRec42[0];
			fRec31[1] = fRec31[0];
			fRec32[1] = fRec32[0];
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fVec6[1] = fVec6[0];
			fVec7[1] = fVec7[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec8[1] = fRec8[0];
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
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec72[1] = fRec72[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec76[1] = fRec76[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec80[1] = fRec80[0];
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
