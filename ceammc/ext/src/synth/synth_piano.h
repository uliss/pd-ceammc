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
	float fVec0[32768];
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
	float fRec46[2];
	float fRec45[2];
	float fRec44[2];
	float fRec43[2];
	float fRec42[2];
	float fVec4[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[8192];
	float fVec5[2];
	float fRec50[2];
	float fRec49[2];
	float fRec48[8192];
	float fConst23;
	float fConst24;
	float fVec6[2];
	float fRec53[2];
	float fRec52[2];
	float fRec51[8192];
	float fVec7[2];
	float fRec47[2];
	float fRec35[2];
	float fRec36[2];
	float fRec37[2];
	float fRec34[3];
	float fConst25;
	float fVec8[2];
	float fVec9[2];
	float fRec63[2];
	float fRec62[2];
	float fRec61[2];
	float fRec60[2];
	float fRec59[2];
	float fRec58[3];
	float fRec57[3];
	float fRec56[3];
	float fRec55[3];
	float fRec54[2];
	float fVec10[4096];
	float fConst26;
	FAUSTFLOAT fHslider8;
	float fVec11[4096];
	int iConst27;
	float fVec12[2048];
	int iConst28;
	float fRec8[2];
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst32;
	float fRec67[2];
	float fRec66[2];
	float fVec13[16384];
	float fConst33;
	int iConst34;
	float fVec14[4096];
	int iConst35;
	float fRec64[2];
	float fConst36;
	float fConst37;
	float fConst38;
	float fConst39;
	float fRec71[2];
	float fRec70[2];
	float fVec15[32768];
	float fConst40;
	int iConst41;
	float fVec16[4096];
	int iConst42;
	float fRec68[2];
	float fConst43;
	float fConst44;
	float fConst45;
	float fConst46;
	float fRec75[2];
	float fRec74[2];
	float fVec17[16384];
	float fConst47;
	int iConst48;
	float fVec18[4096];
	float fVec19[2048];
	int iConst49;
	float fRec72[2];
	float fConst50;
	float fConst51;
	float fConst52;
	float fConst53;
	float fRec79[2];
	float fRec78[2];
	float fVec20[16384];
	float fConst54;
	int iConst55;
	float fVec21[4096];
	int iConst56;
	float fRec76[2];
	float fConst57;
	float fConst58;
	float fConst59;
	float fConst60;
	float fRec83[2];
	float fRec82[2];
	float fVec22[16384];
	float fConst61;
	int iConst62;
	float fVec23[4096];
	int iConst63;
	float fRec80[2];
	float fConst64;
	float fConst65;
	float fConst66;
	float fConst67;
	float fRec87[2];
	float fRec86[2];
	float fVec24[32768];
	float fConst68;
	int iConst69;
	float fVec25[4096];
	int iConst70;
	float fRec84[2];
	float fConst71;
	float fConst72;
	float fConst73;
	float fConst74;
	float fRec91[2];
	float fRec90[2];
	float fVec26[16384];
	float fConst75;
	int iConst76;
	float fVec27[2048];
	int iConst77;
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
		fConst2 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst3 = ((0.0f - (6.90775537f * fConst2)) / fConst0);
		fConst4 = (0.25f * fConst3);
		fConst5 = (0.166666672f * fConst3);
		fConst6 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst7 = (1.0f / (fConst6 + 1.0f));
		fConst8 = (1.0f - fConst6);
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
		fConst23 = (0.0361715779f * fConst0);
		fConst24 = (27.6460152f / fConst0);
		fConst25 = (0.0500000007f / fConst0);
		fConst26 = (0.0011363636f * fConst0);
		iConst27 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		iConst28 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst9 + -1.0f))));
		fConst29 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst30 = ((0.0f - (6.90775537f * fConst29)) / fConst0);
		fConst31 = (0.25f * fConst30);
		fConst32 = (0.166666672f * fConst30);
		fConst33 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst34 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst29 - fConst33))));
		iConst35 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst33 + -1.0f))));
		fConst36 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst37 = ((0.0f - (6.90775537f * fConst36)) / fConst0);
		fConst38 = (0.25f * fConst37);
		fConst39 = (0.166666672f * fConst37);
		fConst40 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst41 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst36 - fConst40))));
		iConst42 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst40 + -1.0f))));
		fConst43 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst44 = ((0.0f - (6.90775537f * fConst43)) / fConst0);
		fConst45 = (0.25f * fConst44);
		fConst46 = (0.166666672f * fConst44);
		fConst47 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst48 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst43 - fConst47))));
		iConst49 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst47 + -1.0f))));
		fConst50 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst51 = ((0.0f - (6.90775537f * fConst50)) / fConst0);
		fConst52 = (0.25f * fConst51);
		fConst53 = (0.166666672f * fConst51);
		fConst54 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst55 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst50 - fConst54))));
		iConst56 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst54 + -1.0f))));
		fConst57 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst58 = ((0.0f - (6.90775537f * fConst57)) / fConst0);
		fConst59 = (0.25f * fConst58);
		fConst60 = (0.166666672f * fConst58);
		fConst61 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst62 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst57 - fConst61))));
		iConst63 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst61 + -1.0f))));
		fConst64 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst65 = ((0.0f - (6.90775537f * fConst64)) / fConst0);
		fConst66 = (0.25f * fConst65);
		fConst67 = (0.166666672f * fConst65);
		fConst68 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst69 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst64 - fConst68))));
		iConst70 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst68 + -1.0f))));
		fConst71 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst72 = ((0.0f - (6.90775537f * fConst71)) / fConst0);
		fConst73 = (0.25f * fConst72);
		fConst74 = (0.166666672f * fConst72);
		fConst75 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst76 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst71 - fConst75))));
		iConst77 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst75 + -1.0f))));
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
		for (int l2 = 0; (l2 < 32768); l2 = (l2 + 1)) {
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
			fRec46[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec45[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec44[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec43[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec42[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fVec4[l32] = 0.0f;
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
		for (int l35 = 0; (l35 < 8192); l35 = (l35 + 1)) {
			fRec39[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fVec5[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec50[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec49[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 8192); l39 = (l39 + 1)) {
			fRec48[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fVec6[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec53[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec52[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 8192); l43 = (l43 + 1)) {
			fRec51[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fVec7[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec47[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec35[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec36[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec37[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec34[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fVec8[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fVec9[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec63[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec62[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec61[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec60[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec59[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec58[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec57[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec56[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec55[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec54[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 4096); l62 = (l62 + 1)) {
			fVec10[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 4096); l63 = (l63 + 1)) {
			fVec11[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 2048); l64 = (l64 + 1)) {
			fVec12[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec8[l65] = 0.0f;
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
			fVec13[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 4096); l69 = (l69 + 1)) {
			fVec14[l69] = 0.0f;
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
			fVec15[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 4096); l74 = (l74 + 1)) {
			fVec16[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec68[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec75[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec74[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 16384); l78 = (l78 + 1)) {
			fVec17[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 4096); l79 = (l79 + 1)) {
			fVec18[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 2048); l80 = (l80 + 1)) {
			fVec19[l80] = 0.0f;
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
			fVec20[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 4096); l85 = (l85 + 1)) {
			fVec21[l85] = 0.0f;
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
		for (int l89 = 0; (l89 < 16384); l89 = (l89 + 1)) {
			fVec22[l89] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l90 = 0; (l90 < 4096); l90 = (l90 + 1)) {
			fVec23[l90] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec80[l91] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec87[l92] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec86[l93] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l94 = 0; (l94 < 32768); l94 = (l94 + 1)) {
			fVec24[l94] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l95 = 0; (l95 < 4096); l95 = (l95 + 1)) {
			fVec25[l95] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec84[l96] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec91[l97] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec90[l98] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l99 = 0; (l99 < 16384); l99 = (l99 + 1)) {
			fVec26[l99] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l100 = 0; (l100 < 2048); l100 = (l100 + 1)) {
			fVec27[l100] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec88[l101] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec0[l102] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec1[l103] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l104 = 0; (l104 < 3); l104 = (l104 + 1)) {
			fRec2[l104] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l105 = 0; (l105 < 3); l105 = (l105 + 1)) {
			fRec3[l105] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec4[l106] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec5[l107] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec6[l108] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
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
		float fSlow11 = (12.0f * fSlow10);
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
		float fSlow50 = (((fSlow49 + (fSlow31 * fSlow44)) / fSlow46) + 1.0f);
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
		float fSlow66 = (1.62981451e-09f * (fSlow65 * float((fSlow13 < 45.0f))));
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
		int iSlow80 = int((fConst13 * (((fSlow32 + std::atan2((0.0f - ((fSlow27 * ((fSlow78 * fSlow47) - (fSlow41 * fSlow79))) / fSlow46)), ((fSlow47 * fSlow79) + ((fSlow51 * fSlow78) / fSlow52)))) + 6.28318548f) / fSlow25)));
		int iSlow81 = std::min<int>(4097, std::max<int>(0, (iSlow80 + 1)));
		float fSlow82 = (1.0f / fSlow46);
		float fSlow83 = (fSlow23 - fSlow24);
		float fSlow84 = (fConst14 * fSlow83);
		float fSlow85 = std::sin(fSlow84);
		float fSlow86 = std::cos(fSlow84);
		float fSlow87 = (3.0f * std::atan2((fSlow22 * fSlow85), (fSlow29 + (fSlow30 * fSlow86))));
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
		float fSlow101 = ((0.0f - (fConst20 * (fSlow14 * fSlow15))) * std::cos((fConst21 * (fSlow14 / float(getValueStrikePosition(float(fSlow13)))))));
		float fSlow102 = (fConst22 * fSlow16);
		float fSlow103 = (4.4000001f * fSlow14);
		float fSlow104 = (fSlow24 + fSlow103);
		float fSlow105 = (fConst14 * fSlow104);
		float fSlow106 = std::sin(fSlow105);
		float fSlow107 = std::cos(fSlow105);
		float fSlow108 = (3.0f * std::atan2((fSlow22 * fSlow106), (fSlow29 + (fSlow30 * fSlow107))));
		float fSlow109 = (((fSlow107 * fSlow41) / fSlow46) + 1.0f);
		float fSlow110 = (((fSlow49 + (fSlow107 * fSlow44)) / fSlow46) + 1.0f);
		float fSlow111 = (synth_piano_faustpower2_f(fSlow106) * fSlow41);
		float fSlow112 = (fConst13 * (((fSlow108 + std::atan2((((0.0f - ((fSlow106 * fSlow44) / fSlow46)) * fSlow109) + (((fSlow106 * fSlow41) * fSlow110) / fSlow46)), ((fSlow109 * fSlow110) + ((fSlow111 * fSlow44) / fSlow52)))) + 6.28318548f) / fSlow104));
		float fSlow113 = std::floor(fSlow112);
		float fSlow114 = (fSlow112 - fSlow113);
		float fSlow115 = (1.16415322e-09f * (fSlow65 * float(((fSlow13 >= 45.0f) & (fSlow13 < 88.0f)))));
		float fSlow116 = (((fSlow49 + (fSlow107 * fSlow78)) / fSlow46) + 1.0f);
		int iSlow117 = int((fConst13 * (((fSlow108 + std::atan2((0.0f - ((fSlow106 * ((fSlow78 * fSlow109) - (fSlow41 * fSlow116))) / fSlow46)), ((fSlow109 * fSlow116) + ((fSlow111 * fSlow78) / fSlow52)))) + 6.28318548f) / fSlow104)));
		int iSlow118 = std::min<int>(4097, std::max<int>(0, (iSlow117 + 1)));
		float fSlow119 = (fSlow103 - fSlow24);
		float fSlow120 = (fConst14 * fSlow119);
		float fSlow121 = std::sin(fSlow120);
		float fSlow122 = std::cos(fSlow120);
		float fSlow123 = (3.0f * std::atan2((fSlow22 * fSlow121), (fSlow29 + (fSlow30 * fSlow122))));
		float fSlow124 = (((fSlow122 * fSlow41) / fSlow46) + 1.0f);
		float fSlow125 = (((fSlow49 + (fSlow122 * fSlow44)) / fSlow46) + 1.0f);
		float fSlow126 = (synth_piano_faustpower2_f(fSlow121) * fSlow41);
		float fSlow127 = (fConst13 * (((fSlow123 + std::atan2((((0.0f - ((fSlow121 * fSlow44) / fSlow46)) * fSlow124) + (((fSlow121 * fSlow41) * fSlow125) / fSlow46)), ((fSlow124 * fSlow125) + ((fSlow126 * fSlow44) / fSlow52)))) + 6.28318548f) / fSlow119));
		float fSlow128 = std::floor(fSlow127);
		float fSlow129 = (fSlow127 - fSlow128);
		float fSlow130 = (((fSlow49 + (fSlow122 * fSlow78)) / fSlow46) + 1.0f);
		int iSlow131 = int((fConst13 * (((fSlow123 + std::atan2((0.0f - ((fSlow121 * ((fSlow78 * fSlow124) - (fSlow41 * fSlow130))) / fSlow46)), ((fSlow124 * fSlow130) + ((fSlow126 * fSlow78) / fSlow52)))) + 6.28318548f) / fSlow119)));
		int iSlow132 = std::min<int>(4097, std::max<int>(0, (iSlow131 + 1)));
		float fSlow133 = (fConst24 * fSlow14);
		float fSlow134 = std::sin(fSlow133);
		float fSlow135 = std::cos(fSlow133);
		float fSlow136 = (3.0f * std::atan2((fSlow22 * fSlow134), (fSlow29 + (fSlow30 * fSlow135))));
		float fSlow137 = (((fSlow135 * fSlow41) / fSlow46) + 1.0f);
		float fSlow138 = (((fSlow49 + (fSlow135 * fSlow44)) / fSlow46) + 1.0f);
		float fSlow139 = (synth_piano_faustpower2_f(fSlow134) * fSlow41);
		float fSlow140 = (fConst23 * (((fSlow136 + std::atan2((((0.0f - ((fSlow134 * fSlow44) / fSlow46)) * fSlow137) + (((fSlow134 * fSlow41) * fSlow138) / fSlow46)), ((fSlow137 * fSlow138) + ((fSlow139 * fSlow44) / fSlow52)))) + 6.28318548f) / fSlow14));
		float fSlow141 = std::floor(fSlow140);
		float fSlow142 = (fSlow141 + (1.0f - fSlow140));
		float fSlow143 = (((fSlow49 + (fSlow135 * fSlow78)) / fSlow46) + 1.0f);
		int iSlow144 = int((fConst23 * (((fSlow136 + std::atan2((0.0f - ((fSlow134 * ((fSlow78 * fSlow137) - (fSlow41 * fSlow143))) / fSlow46)), ((fSlow137 * fSlow143) + ((fSlow139 * fSlow78) / fSlow52)))) + 6.28318548f) / fSlow14)));
		int iSlow145 = std::min<int>(4097, std::max<int>(0, iSlow144));
		float fSlow146 = (fSlow140 - fSlow141);
		int iSlow147 = std::min<int>(4097, std::max<int>(0, (iSlow144 + 1)));
		float fSlow148 = (fSlow128 + (1.0f - fSlow127));
		int iSlow149 = std::min<int>(4097, std::max<int>(0, iSlow131));
		float fSlow150 = (fSlow113 + (1.0f - fSlow112));
		int iSlow151 = std::min<int>(4097, std::max<int>(0, iSlow117));
		float fSlow152 = std::cos((fConst21 * fSlow14));
		float fSlow153 = std::pow(10.0f, (0.0500000007f * float(getValueSecondStageAmpRatio(float(fSlow13)))));
		float fSlow154 = std::pow(10.0f, (fConst25 * float(getValuer1_1db(float(fSlow13)))));
		float fSlow155 = std::pow(10.0f, (fConst25 * float(getValuer1_2db(float(fSlow13)))));
		float fSlow156 = (1.0f - fSlow153);
		float fSlow157 = (0.0f - (2.0f * ((fSlow153 * fSlow154) + (fSlow155 * fSlow156))));
		float fSlow158 = float(getValueBq4_gEarBalled(float(fSlow13)));
		float fSlow159 = (2.0f * fSlow158);
		float fSlow160 = float((fSlow13 >= 88.0f));
		float fSlow161 = (2.32830644e-10f * fSlow160);
		float fSlow162 = (1.16415322e-10f * fSlow160);
		float fSlow163 = std::pow(10.0f, (fConst25 * float(getValuer3db(float(fSlow13)))));
		float fSlow164 = (std::cos((fConst21 * (fSlow14 * float(getValueThirdPartialFactor(float(fSlow13)))))) * (0.0f - (2.0f * fSlow163)));
		float fSlow165 = synth_piano_faustpower2_f(fSlow163);
		float fSlow166 = std::pow(10.0f, (fConst25 * float(getValuer2db(float(fSlow13)))));
		float fSlow167 = (std::cos((fConst21 * (fSlow14 * float(getValueSecondPartialFactor(float(fSlow13)))))) * (0.0f - (2.0f * fSlow166)));
		float fSlow168 = synth_piano_faustpower2_f(fSlow166);
		float fSlow169 = (fSlow152 * (0.0f - (2.0f * fSlow154)));
		float fSlow170 = synth_piano_faustpower2_f(fSlow154);
		float fSlow171 = (0.0f - (2.0f * fSlow155));
		float fSlow172 = synth_piano_faustpower2_f(fSlow155);
		float fSlow173 = ((fSlow153 * fSlow170) + (fSlow172 * fSlow156));
		int iSlow174 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst26 * (float(fHslider8) / fSlow14)))));
		float fSlow175 = std::exp((fConst31 / fSlow0));
		float fSlow176 = synth_piano_faustpower2_f(fSlow175);
		float fSlow177 = (1.0f - (fConst1 * fSlow176));
		float fSlow178 = (1.0f - fSlow176);
		float fSlow179 = (fSlow177 / fSlow178);
		float fSlow180 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow177) / synth_piano_faustpower2_f(fSlow178)) + -1.0f)));
		float fSlow181 = (fSlow179 - fSlow180);
		float fSlow182 = (fSlow175 * (fSlow180 + (1.0f - fSlow179)));
		float fSlow183 = ((std::exp((fConst32 / fSlow0)) / fSlow175) + -1.0f);
		float fSlow184 = std::exp((fConst38 / fSlow0));
		float fSlow185 = synth_piano_faustpower2_f(fSlow184);
		float fSlow186 = (1.0f - (fConst1 * fSlow185));
		float fSlow187 = (1.0f - fSlow185);
		float fSlow188 = (fSlow186 / fSlow187);
		float fSlow189 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow186) / synth_piano_faustpower2_f(fSlow187)) + -1.0f)));
		float fSlow190 = (fSlow188 - fSlow189);
		float fSlow191 = (fSlow184 * (fSlow189 + (1.0f - fSlow188)));
		float fSlow192 = ((std::exp((fConst39 / fSlow0)) / fSlow184) + -1.0f);
		float fSlow193 = std::exp((fConst45 / fSlow0));
		float fSlow194 = synth_piano_faustpower2_f(fSlow193);
		float fSlow195 = (1.0f - (fConst1 * fSlow194));
		float fSlow196 = (1.0f - fSlow194);
		float fSlow197 = (fSlow195 / fSlow196);
		float fSlow198 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow195) / synth_piano_faustpower2_f(fSlow196)) + -1.0f)));
		float fSlow199 = (fSlow197 - fSlow198);
		float fSlow200 = (fSlow193 * (fSlow198 + (1.0f - fSlow197)));
		float fSlow201 = ((std::exp((fConst46 / fSlow0)) / fSlow193) + -1.0f);
		float fSlow202 = (12.0f * (1.0f - fSlow10));
		float fSlow203 = std::exp((fConst52 / fSlow0));
		float fSlow204 = synth_piano_faustpower2_f(fSlow203);
		float fSlow205 = (1.0f - (fConst1 * fSlow204));
		float fSlow206 = (1.0f - fSlow204);
		float fSlow207 = (fSlow205 / fSlow206);
		float fSlow208 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow205) / synth_piano_faustpower2_f(fSlow206)) + -1.0f)));
		float fSlow209 = (fSlow207 - fSlow208);
		float fSlow210 = (fSlow203 * (fSlow208 + (1.0f - fSlow207)));
		float fSlow211 = ((std::exp((fConst53 / fSlow0)) / fSlow203) + -1.0f);
		float fSlow212 = std::exp((fConst59 / fSlow0));
		float fSlow213 = synth_piano_faustpower2_f(fSlow212);
		float fSlow214 = (1.0f - (fConst1 * fSlow213));
		float fSlow215 = (1.0f - fSlow213);
		float fSlow216 = (fSlow214 / fSlow215);
		float fSlow217 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow214) / synth_piano_faustpower2_f(fSlow215)) + -1.0f)));
		float fSlow218 = (fSlow216 - fSlow217);
		float fSlow219 = (fSlow212 * (fSlow217 + (1.0f - fSlow216)));
		float fSlow220 = ((std::exp((fConst60 / fSlow0)) / fSlow212) + -1.0f);
		float fSlow221 = std::exp((fConst66 / fSlow0));
		float fSlow222 = synth_piano_faustpower2_f(fSlow221);
		float fSlow223 = (1.0f - (fConst1 * fSlow222));
		float fSlow224 = (1.0f - fSlow222);
		float fSlow225 = (fSlow223 / fSlow224);
		float fSlow226 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow223) / synth_piano_faustpower2_f(fSlow224)) + -1.0f)));
		float fSlow227 = (fSlow225 - fSlow226);
		float fSlow228 = (fSlow221 * (fSlow226 + (1.0f - fSlow225)));
		float fSlow229 = ((std::exp((fConst67 / fSlow0)) / fSlow221) + -1.0f);
		float fSlow230 = std::exp((fConst73 / fSlow0));
		float fSlow231 = synth_piano_faustpower2_f(fSlow230);
		float fSlow232 = (1.0f - (fConst1 * fSlow231));
		float fSlow233 = (1.0f - fSlow231);
		float fSlow234 = (fSlow232 / fSlow233);
		float fSlow235 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow232) / synth_piano_faustpower2_f(fSlow233)) + -1.0f)));
		float fSlow236 = (fSlow234 - fSlow235);
		float fSlow237 = (fSlow230 * (fSlow235 + (1.0f - fSlow234)));
		float fSlow238 = ((std::exp((fConst74 / fSlow0)) / fSlow230) + -1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec11[0] = (0.0f - (fConst7 * ((fConst8 * fRec11[1]) - (fRec7[1] + fRec7[2]))));
			fRec10[0] = ((fSlow7 * fRec10[1]) + (fSlow8 * (fRec7[1] + (fSlow9 * fRec11[0]))));
			fVec0[(IOTA & 32767)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
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
			fRec13[0] = ((fSlow18 * fRec16) - ((fSlow101 * fRec13[1]) + (fSlow102 * fRec13[2])));
			fRec46[0] = ((fSlow115 * fTemp5) + (fSlow75 * fRec46[1]));
			fRec45[0] = ((fSlow65 * fRec46[0]) + (fSlow75 * fRec45[1]));
			fRec44[0] = ((fSlow65 * fRec45[0]) + (fSlow75 * fRec44[1]));
			fRec43[0] = ((fSlow65 * fRec44[0]) + (fSlow75 * fRec43[1]));
			fRec42[0] = (((fSlow62 * fRec43[0]) + (fSlow76 * fRec43[1])) - (fSlow60 * fRec42[1]));
			float fTemp13 = (fRec20[0] * (fRec42[0] + fRec35[1]));
			fVec4[0] = fTemp13;
			fRec41[0] = (fVec4[1] + (fSlow77 * (fTemp13 - fRec41[1])));
			fRec40[0] = (fRec41[1] + (fSlow77 * (fRec41[0] - fRec40[1])));
			fRec39[(IOTA & 8191)] = (fRec40[1] + (fSlow77 * (fRec40[0] - fRec39[((IOTA - 1) & 8191)])));
			float fTemp14 = (fSlow114 * fRec39[((IOTA - iSlow118) & 8191)]);
			float fTemp15 = (fRec42[0] + (fRec20[0] * fRec37[1]));
			fVec5[0] = fTemp15;
			fRec50[0] = (fVec5[1] + (fSlow77 * (fTemp15 - fRec50[1])));
			fRec49[0] = (fRec50[1] + (fSlow77 * (fRec50[0] - fRec49[1])));
			fRec48[(IOTA & 8191)] = (fRec49[1] + (fSlow77 * (fRec49[0] - fRec48[((IOTA - 1) & 8191)])));
			float fTemp16 = (fSlow129 * fRec48[((IOTA - iSlow132) & 8191)]);
			float fTemp17 = (fRec42[0] + (fRec20[0] * fRec36[1]));
			fVec6[0] = fTemp17;
			fRec53[0] = (fVec6[1] + (fSlow77 * (fTemp17 - fRec53[1])));
			fRec52[0] = (fRec53[1] + (fSlow77 * (fRec53[0] - fRec52[1])));
			fRec51[(IOTA & 8191)] = (fRec52[1] + (fSlow77 * (fRec52[0] - fRec51[((IOTA - 1) & 8191)])));
			float fTemp18 = (fSlow142 * fRec51[((IOTA - iSlow145) & 8191)]);
			float fTemp19 = (fSlow146 * fRec51[((IOTA - iSlow147) & 8191)]);
			float fTemp20 = (fSlow148 * fRec48[((IOTA - iSlow149) & 8191)]);
			float fTemp21 = (fSlow150 * fRec39[((IOTA - iSlow151) & 8191)]);
			float fTemp22 = (fTemp14 + ((fTemp16 + ((fTemp18 + fTemp19) + fTemp20)) + fTemp21));
			fVec7[0] = fTemp22;
			fRec47[0] = (fSlow82 * ((2.0f * ((fSlow48 * fTemp22) + (fSlow42 * fVec7[1]))) - (fSlow41 * fRec47[1])));
			fRec35[0] = (fTemp14 + (fRec47[0] + fTemp21));
			fRec36[0] = (fTemp19 + (fRec47[0] + fTemp18));
			fRec37[0] = (fTemp16 + (fRec47[0] + fTemp20));
			float fRec38 = fTemp22;
			fRec34[0] = ((fSlow18 * fRec38) - ((fSlow101 * fRec34[1]) + (fSlow102 * fRec34[2])));
			fVec8[0] = (fSlow161 * fTemp5);
			float fTemp23 = (0.0f - ((0.5f * fVec8[1]) + (fSlow162 * fTemp5)));
			fVec9[0] = fTemp23;
			fRec63[0] = (((fSlow62 * fTemp23) + (fSlow76 * fVec9[1])) - (fSlow60 * fRec63[1]));
			fRec62[0] = ((fSlow65 * fRec63[0]) + (fSlow75 * fRec62[1]));
			fRec61[0] = ((fSlow65 * fRec62[0]) + (fSlow75 * fRec61[1]));
			fRec60[0] = ((fSlow65 * fRec61[0]) + (fSlow75 * fRec60[1]));
			fRec59[0] = ((fSlow65 * fRec60[0]) + (fSlow75 * fRec59[1]));
			fRec58[0] = ((fSlow158 * (fRec59[0] - fRec59[1])) - ((fSlow164 * fRec58[1]) + (fSlow165 * fRec58[2])));
			fRec57[0] = ((fSlow159 * fRec58[0]) - ((fSlow167 * fRec57[1]) + (fSlow168 * fRec57[2])));
			fRec56[0] = (fRec57[0] - ((fSlow169 * fRec56[1]) + (fSlow170 * fRec56[2])));
			fRec55[0] = (((fSlow152 * ((fSlow157 * fRec56[1]) - (fSlow171 * fRec55[1]))) + (fRec56[0] + (fSlow173 * fRec56[2]))) - (fSlow172 * fRec55[2]));
			fRec54[0] = ((fSlow61 * fRec55[0]) - (fSlow60 * fRec54[1]));
			float fTemp24 = ((fSlow17 * ((fRec13[0] - fRec13[2]) + (fRec34[0] - fRec34[2]))) + (fRec54[0] + (fRec16 + fRec38)));
			fVec10[(IOTA & 4095)] = fTemp24;
			float fTemp25 = fVec10[((IOTA - iSlow174) & 4095)];
			fVec11[(IOTA & 4095)] = (fSlow11 * (fRec12[0] * fTemp25));
			float fTemp26 = (0.300000012f * fVec11[((IOTA - iConst27) & 4095)]);
			float fTemp27 = (((0.600000024f * fRec8[1]) + fVec0[((IOTA - iConst10) & 32767)]) - fTemp26);
			fVec12[(IOTA & 2047)] = fTemp27;
			fRec8[0] = fVec12[((IOTA - iConst28) & 2047)];
			float fRec9 = (0.0f - (0.600000024f * fTemp27));
			fRec67[0] = (0.0f - (fConst7 * ((fConst8 * fRec67[1]) - (fRec5[1] + fRec5[2]))));
			fRec66[0] = ((fSlow181 * fRec66[1]) + (fSlow182 * (fRec5[1] + (fSlow183 * fRec67[0]))));
			fVec13[(IOTA & 16383)] = ((0.353553385f * fRec66[0]) + 9.99999968e-21f);
			float fTemp28 = ((0.600000024f * fRec64[1]) + (fTemp26 + fVec13[((IOTA - iConst34) & 16383)]));
			fVec14[(IOTA & 4095)] = fTemp28;
			fRec64[0] = fVec14[((IOTA - iConst35) & 4095)];
			float fRec65 = (0.0f - (0.600000024f * fTemp28));
			fRec71[0] = (0.0f - (fConst7 * ((fConst8 * fRec71[1]) - (fRec1[1] + fRec1[2]))));
			fRec70[0] = ((fSlow190 * fRec70[1]) + (fSlow191 * (fRec1[1] + (fSlow192 * fRec71[0]))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec70[0]) + 9.99999968e-21f);
			float fTemp29 = (fTemp26 + ((0.600000024f * fRec68[1]) + fVec15[((IOTA - iConst41) & 32767)]));
			fVec16[(IOTA & 4095)] = fTemp29;
			fRec68[0] = fVec16[((IOTA - iConst42) & 4095)];
			float fRec69 = (0.0f - (0.600000024f * fTemp29));
			fRec75[0] = (0.0f - (fConst7 * ((fConst8 * fRec75[1]) - (fRec6[1] + fRec6[2]))));
			fRec74[0] = ((fSlow199 * fRec74[1]) + (fSlow200 * (fRec6[1] + (fSlow201 * fRec75[0]))));
			fVec17[(IOTA & 16383)] = ((0.353553385f * fRec74[0]) + 9.99999968e-21f);
			fVec18[(IOTA & 4095)] = (fSlow202 * (fRec12[0] * fTemp24));
			float fTemp30 = (0.300000012f * fVec18[((IOTA - iConst27) & 4095)]);
			float fTemp31 = (fVec17[((IOTA - iConst48) & 16383)] - (fTemp30 + (0.600000024f * fRec72[1])));
			fVec19[(IOTA & 2047)] = fTemp31;
			fRec72[0] = fVec19[((IOTA - iConst49) & 2047)];
			float fRec73 = (0.600000024f * fTemp31);
			fRec79[0] = (0.0f - (fConst7 * ((fConst8 * fRec79[1]) - (fRec2[1] + fRec2[2]))));
			fRec78[0] = ((fSlow209 * fRec78[1]) + (fSlow210 * (fRec2[1] + (fSlow211 * fRec79[0]))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec78[0]) + 9.99999968e-21f);
			float fTemp32 = (fVec20[((IOTA - iConst55) & 16383)] - (fTemp30 + (0.600000024f * fRec76[1])));
			fVec21[(IOTA & 4095)] = fTemp32;
			fRec76[0] = fVec21[((IOTA - iConst56) & 4095)];
			float fRec77 = (0.600000024f * fTemp32);
			fRec83[0] = (0.0f - (fConst7 * ((fConst8 * fRec83[1]) - (fRec4[1] + fRec4[2]))));
			fRec82[0] = ((fSlow218 * fRec82[1]) + (fSlow219 * (fRec4[1] + (fSlow220 * fRec83[0]))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec82[0]) + 9.99999968e-21f);
			float fTemp33 = ((fTemp30 + fVec22[((IOTA - iConst62) & 16383)]) - (0.600000024f * fRec80[1]));
			fVec23[(IOTA & 4095)] = fTemp33;
			fRec80[0] = fVec23[((IOTA - iConst63) & 4095)];
			float fRec81 = (0.600000024f * fTemp33);
			fRec87[0] = (0.0f - (fConst7 * ((fConst8 * fRec87[1]) - (fRec3[1] + fRec3[2]))));
			fRec86[0] = ((fSlow227 * fRec86[1]) + (fSlow228 * (fRec3[1] + (fSlow229 * fRec87[0]))));
			fVec24[(IOTA & 32767)] = ((0.353553385f * fRec86[0]) + 9.99999968e-21f);
			float fTemp34 = (((0.600000024f * fRec84[1]) + fVec24[((IOTA - iConst69) & 32767)]) - fTemp26);
			fVec25[(IOTA & 4095)] = fTemp34;
			fRec84[0] = fVec25[((IOTA - iConst70) & 4095)];
			float fRec85 = (0.0f - (0.600000024f * fTemp34));
			fRec91[0] = (0.0f - (fConst7 * ((fConst8 * fRec91[1]) - (fRec0[1] + fRec0[2]))));
			fRec90[0] = ((fSlow236 * fRec90[1]) + (fSlow237 * (fRec0[1] + (fSlow238 * fRec91[0]))));
			fVec26[(IOTA & 16383)] = ((0.353553385f * fRec90[0]) + 9.99999968e-21f);
			float fTemp35 = ((fVec26[((IOTA - iConst76) & 16383)] + fTemp30) - (0.600000024f * fRec88[1]));
			fVec27[(IOTA & 2047)] = fTemp35;
			fRec88[0] = fVec27[((IOTA - iConst77) & 2047)];
			float fRec89 = (0.600000024f * fTemp35);
			float fTemp36 = (fRec89 + (fRec88[1] + fRec84[1]));
			float fTemp37 = (fRec81 + fTemp36);
			fRec0[0] = (fRec8[1] + (fRec64[1] + (fRec68[1] + (fRec72[1] + (fRec76[1] + (fRec80[1] + (fRec9 + (fRec85 + (fRec65 + (fRec69 + (fRec73 + (fRec77 + fTemp37))))))))))));
			float fTemp38 = (fRec89 + fRec88[1]);
			float fTemp39 = (fRec81 + fTemp38);
			fRec1[0] = ((fRec72[1] + (fRec76[1] + (fRec80[1] + (fRec73 + (fRec77 + fTemp39))))) - (fRec8[1] + (fRec64[1] + (fRec68[1] + (fRec9 + (fRec85 + (fRec65 + (fRec69 + fRec84[1]))))))));
			fRec2[0] = ((fRec64[1] + (fRec68[1] + (fRec80[1] + (fRec65 + (fRec69 + fTemp39))))) - (fRec8[1] + (fRec72[1] + (fRec76[1] + (fRec9 + (fRec85 + (fRec73 + (fRec77 + fRec84[1]))))))));
			fRec3[0] = ((fRec8[1] + (fRec80[1] + (fRec9 + (fRec85 + fTemp37)))) - (fRec64[1] + (fRec68[1] + (fRec72[1] + (fRec76[1] + (fRec65 + (fRec69 + (fRec77 + fRec73))))))));
			fRec4[0] = ((fRec68[1] + (fRec76[1] + (fRec85 + (fRec69 + (fRec77 + fTemp36))))) - (fRec8[1] + (fRec64[1] + (fRec72[1] + (fRec80[1] + (fRec9 + (fRec65 + (fRec81 + fRec73))))))));
			float fTemp40 = (fRec81 + fRec84[1]);
			fRec5[0] = ((fRec8[1] + (fRec64[1] + (fRec76[1] + (fRec9 + (fRec65 + (fRec77 + fTemp38)))))) - (fRec68[1] + (fRec72[1] + (fRec80[1] + (fRec85 + (fRec69 + (fRec73 + fTemp40)))))));
			fRec6[0] = ((fRec8[1] + (fRec68[1] + (fRec72[1] + (fRec9 + (fRec69 + (fRec73 + fTemp38)))))) - (fRec64[1] + (fRec76[1] + (fRec80[1] + (fRec85 + (fRec65 + (fRec77 + fTemp40)))))));
			fRec7[0] = ((fRec64[1] + (fRec72[1] + (fRec85 + (fRec65 + (fRec73 + fTemp36))))) - (fRec8[1] + (fRec68[1] + (fRec76[1] + (fRec80[1] + (fRec9 + (fRec69 + (fRec81 + fRec77))))))));
			float fTemp41 = (1.0f - fRec12[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow202 * (fTemp41 * fTemp24))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow11 * (fTemp41 * fTemp25))));
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
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fVec4[1] = fVec4[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fVec5[1] = fVec5[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fVec6[1] = fVec6[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fVec7[1] = fVec7[0];
			fRec47[1] = fRec47[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fVec8[1] = fVec8[0];
			fVec9[1] = fVec9[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
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
