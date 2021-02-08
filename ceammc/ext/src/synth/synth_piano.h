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
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	float fRec10[2];
	FAUSTFLOAT fHslider2;
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider3;
	int iRec21[2];
	FAUSTFLOAT fButton0;
	int iRec23[2];
	float fConst3;
	float fRec22[2];
	float fConst4;
	float fConst5;
	FAUSTFLOAT fHslider4;
	float fConst6;
	float fConst7;
	float fRec24[2];
	float fVec0[2];
	float fVec1[2];
	float fRec20[2];
	float fRec19[2];
	float fRec18[2];
	float fRec17[2];
	float fRec16[2];
	float fRec15[3];
	float fRec14[3];
	float fRec13[3];
	float fRec12[3];
	float fRec11[2];
	float fConst8;
	FAUSTFLOAT fHslider5;
	float fConst9;
	FAUSTFLOAT fHslider6;
	float fRec32[2];
	float fRec37[2];
	float fRec36[2];
	float fRec35[2];
	float fRec34[2];
	float fRec33[2];
	float fVec2[2];
	float fRec31[2];
	float fRec30[2];
	int IOTA;
	float fRec29[8192];
	float fVec3[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[8192];
	float fConst10;
	float fVec4[2];
	float fRec44[2];
	float fRec43[2];
	float fRec42[8192];
	float fVec5[2];
	float fRec38[2];
	float fRec25[2];
	float fRec26[2];
	float fRec27[2];
	float fRec55[2];
	float fRec54[2];
	float fRec53[2];
	float fRec52[2];
	float fRec51[2];
	float fVec6[2];
	float fRec50[2];
	float fRec49[2];
	float fRec48[8192];
	float fVec7[2];
	float fRec59[2];
	float fRec58[2];
	float fRec57[8192];
	float fVec8[2];
	float fRec56[2];
	float fRec45[2];
	float fRec46[2];
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fRec60[3];
	float fRec61[3];
	float fVec9[4096];
	float fConst15;
	FAUSTFLOAT fHslider7;
	float fVec10[4096];
	int iConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	FAUSTFLOAT fHslider8;
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	float fRec63[2];
	float fRec62[2];
	float fVec11[16384];
	float fConst25;
	int iConst26;
	float fVec12[4096];
	int iConst27;
	float fRec8[2];
	float fConst28;
	float fConst29;
	float fConst30;
	float fConst31;
	float fRec67[2];
	float fRec66[2];
	float fVec13[32768];
	float fConst32;
	int iConst33;
	float fVec14[4096];
	int iConst34;
	float fRec64[2];
	float fConst35;
	float fConst36;
	float fConst37;
	float fConst38;
	float fRec71[2];
	float fRec70[2];
	float fVec15[16384];
	float fConst39;
	int iConst40;
	float fVec16[4096];
	float fVec17[2048];
	int iConst41;
	float fRec68[2];
	float fConst42;
	float fConst43;
	float fConst44;
	float fConst45;
	float fRec75[2];
	float fRec74[2];
	float fVec18[16384];
	float fConst46;
	int iConst47;
	float fVec19[4096];
	int iConst48;
	float fRec72[2];
	float fConst49;
	float fConst50;
	float fConst51;
	float fConst52;
	float fRec79[2];
	float fRec78[2];
	float fVec20[16384];
	float fConst53;
	int iConst54;
	float fVec21[4096];
	int iConst55;
	float fRec76[2];
	float fConst56;
	float fConst57;
	float fConst58;
	float fConst59;
	float fRec83[2];
	float fRec82[2];
	float fVec22[16384];
	float fConst60;
	int iConst61;
	float fVec23[2048];
	int iConst62;
	float fRec80[2];
	float fConst63;
	float fConst64;
	float fConst65;
	float fConst66;
	float fRec87[2];
	float fRec86[2];
	float fVec24[32768];
	float fConst67;
	int iConst68;
	float fVec25[2048];
	int iConst69;
	float fRec84[2];
	float fConst70;
	float fConst71;
	float fConst72;
	float fConst73;
	float fRec91[2];
	float fRec90[2];
	float fVec26[32768];
	float fConst74;
	int iConst75;
	float fVec27[4096];
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
		fConst1 = (2764.60156f / fConst0);
		fConst2 = (0.0500000007f / fConst0);
		fConst3 = (7.0f / fConst0);
		fConst4 = std::exp((0.0f - (5.0f / fConst0)));
		fConst5 = (10.0f / fConst0);
		fConst6 = (0.100000001f * fConst0);
		fConst7 = std::exp((0.0f - (0.5f / fConst0)));
		fConst8 = (0.159154937f * fConst0);
		fConst9 = (6.28318548f / fConst0);
		fConst10 = (0.000361715793f * fConst0);
		fConst11 = synth_piano_faustpower2_f(fConst0);
		fConst12 = (96800.0f / fConst11);
		fConst13 = (880.0f / fConst0);
		fConst14 = (193600.0f / fConst11);
		fConst15 = (0.0011363636f * fConst0);
		iConst16 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		fConst17 = std::cos((37699.1133f / fConst0));
		fConst18 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst19 = ((0.0f - (6.90775537f * fConst18)) / fConst0);
		fConst20 = (0.25f * fConst19);
		fConst21 = (0.166666672f * fConst19);
		fConst22 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst23 = (1.0f / (fConst22 + 1.0f));
		fConst24 = (1.0f - fConst22);
		fConst25 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst26 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst18 - fConst25))));
		iConst27 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst25 + -1.0f))));
		fConst28 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst29 = ((0.0f - (6.90775537f * fConst28)) / fConst0);
		fConst30 = (0.25f * fConst29);
		fConst31 = (0.166666672f * fConst29);
		fConst32 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst33 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst28 - fConst32))));
		iConst34 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst32 + -1.0f))));
		fConst35 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst36 = ((0.0f - (6.90775537f * fConst35)) / fConst0);
		fConst37 = (0.25f * fConst36);
		fConst38 = (0.166666672f * fConst36);
		fConst39 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst40 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst35 - fConst39))));
		iConst41 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst39 + -1.0f))));
		fConst42 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst43 = ((0.0f - (6.90775537f * fConst42)) / fConst0);
		fConst44 = (0.25f * fConst43);
		fConst45 = (0.166666672f * fConst43);
		fConst46 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst47 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst42 - fConst46))));
		iConst48 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst46 + -1.0f))));
		fConst49 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst50 = ((0.0f - (6.90775537f * fConst49)) / fConst0);
		fConst51 = (0.25f * fConst50);
		fConst52 = (0.166666672f * fConst50);
		fConst53 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst54 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst49 - fConst53))));
		iConst55 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst53 + -1.0f))));
		fConst56 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst57 = ((0.0f - (6.90775537f * fConst56)) / fConst0);
		fConst58 = (0.25f * fConst57);
		fConst59 = (0.166666672f * fConst57);
		fConst60 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst61 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst56 - fConst60))));
		iConst62 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst60 + -1.0f))));
		fConst63 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst64 = ((0.0f - (6.90775537f * fConst63)) / fConst0);
		fConst65 = (0.25f * fConst64);
		fConst66 = (0.166666672f * fConst64);
		fConst67 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst68 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst63 - fConst67))));
		iConst69 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst67 + -1.0f))));
		fConst70 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst71 = ((0.0f - (6.90775537f * fConst70)) / fConst0);
		fConst72 = (0.25f * fConst71);
		fConst73 = (0.166666672f * fConst71);
		fConst74 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst75 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst70 - fConst74))));
		iConst76 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst74 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.59999999999999998f);
		fHslider1 = FAUSTFLOAT(0.13700000000000001f);
		fHslider2 = FAUSTFLOAT(48.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.10000000000000001f);
		fHslider5 = FAUSTFLOAT(0.28000000000000003f);
		fHslider6 = FAUSTFLOAT(0.10000000000000001f);
		fHslider7 = FAUSTFLOAT(0.5f);
		fHslider8 = FAUSTFLOAT(0.35999999999999999f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec10[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iRec21[l1] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec23[l2] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec22[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec24[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec0[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fVec1[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec20[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec19[l8] = 0.0f;
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
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec16[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec15[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec14[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec13[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec12[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec11[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec32[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec37[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec36[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec35[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec34[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec33[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fVec2[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec31[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec30[l25] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 8192); l26 = (l26 + 1)) {
			fRec29[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fVec3[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec41[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec40[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 8192); l30 = (l30 + 1)) {
			fRec39[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fVec4[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec44[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec43[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 8192); l34 = (l34 + 1)) {
			fRec42[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fVec5[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec38[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec25[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec26[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec27[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec55[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec54[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec53[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec52[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec51[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fVec6[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec50[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec49[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 8192); l48 = (l48 + 1)) {
			fRec48[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fVec7[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec59[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec58[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 8192); l52 = (l52 + 1)) {
			fRec57[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fVec8[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec56[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec45[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec46[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec60[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec61[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 4096); l59 = (l59 + 1)) {
			fVec9[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 4096); l60 = (l60 + 1)) {
			fVec10[l60] = 0.0f;
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
			fVec11[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 4096); l64 = (l64 + 1)) {
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
		for (int l68 = 0; (l68 < 32768); l68 = (l68 + 1)) {
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
		for (int l73 = 0; (l73 < 16384); l73 = (l73 + 1)) {
			fVec15[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 4096); l74 = (l74 + 1)) {
			fVec16[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 2048); l75 = (l75 + 1)) {
			fVec17[l75] = 0.0f;
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
		for (int l79 = 0; (l79 < 16384); l79 = (l79 + 1)) {
			fVec18[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 4096); l80 = (l80 + 1)) {
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
		for (int l90 = 0; (l90 < 2048); l90 = (l90 + 1)) {
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
		for (int l95 = 0; (l95 < 2048); l95 = (l95 + 1)) {
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
		for (int l99 = 0; (l99 < 32768); l99 = (l99 + 1)) {
			fVec26[l99] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l100 = 0; (l100 < 4096); l100 = (l100 + 1)) {
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
		ui_interface->addHorizontalSlider("brightness", &fHslider3, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("detuning", &fHslider6, 0.100000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider4, 0.100000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pan", &fHslider0, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider2, 48.0f, 21.0f, 108.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider1, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider8, 0.360000014f, 0.00499999989f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("stiffness", &fHslider5, 0.280000001f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("width", &fHslider7, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (12.0f * fSlow0);
		float fSlow2 = (0.00100000005f * float(fHslider1));
		float fSlow3 = float(fHslider2);
		float fSlow4 = float(getValueDCBa1(float(fSlow3)));
		float fSlow5 = (1.0f - fSlow4);
		float fSlow6 = std::pow(2.0f, (0.0833333358f * (fSlow3 + -69.0f)));
		float fSlow7 = std::cos((fConst1 * fSlow6));
		float fSlow8 = std::pow(10.0f, (0.0500000007f * float(getValueSecondStageAmpRatio(float(fSlow3)))));
		float fSlow9 = std::pow(10.0f, (fConst2 * float(getValuer1_1db(float(fSlow3)))));
		float fSlow10 = std::pow(10.0f, (fConst2 * float(getValuer1_2db(float(fSlow3)))));
		float fSlow11 = (1.0f - fSlow8);
		float fSlow12 = (0.0f - (2.0f * ((fSlow8 * fSlow9) + (fSlow10 * fSlow11))));
		float fSlow13 = (2.0f * float(getValueBq4_gEarBalled(float(fSlow3))));
		float fSlow14 = (0.25f * float(fHslider3));
		float fSlow15 = float(getValueLoudPole(float(fSlow3)));
		float fSlow16 = (float(getValueLoudGain(float(fSlow3))) * (fSlow14 + (0.980000019f - fSlow15)));
		float fSlow17 = (0.5f * fSlow5);
		float fSlow18 = float((fSlow3 >= 88.0f));
		float fSlow19 = (2.32830644e-10f * fSlow18);
		float fSlow20 = float(fButton0);
		int iSlow21 = (fSlow20 > 0.0f);
		int iSlow22 = (iSlow21 > 0);
		float fSlow23 = std::exp((0.0f - (fConst3 / (float(getValueDryTapAmpT60(float(fSlow3))) * std::min<float>(1.0f, std::max<float>(0.0f, fSlow20))))));
		int iSlow24 = (iSlow21 < 1);
		float fSlow25 = float((0 - (iSlow21 + -1)));
		float fSlow26 = (fConst4 * fSlow25);
		float fSlow27 = float(iSlow21);
		float fSlow28 = float(fHslider4);
		float fSlow29 = (fSlow27 * std::exp((0.0f - (fConst5 / fSlow28))));
		float fSlow30 = (fConst6 * fSlow28);
		float fSlow31 = (0.200000003f * float(getValueSustainPedalLevel(float(fSlow3))));
		float fSlow32 = (1.16415322e-10f * fSlow18);
		float fSlow33 = (0.0f - fSlow17);
		float fSlow34 = (fSlow15 + (0.0199999996f - fSlow14));
		float fSlow35 = std::pow(10.0f, (fConst2 * float(getValuer3db(float(fSlow3)))));
		float fSlow36 = (std::cos((fConst1 * (fSlow6 * float(getValueThirdPartialFactor(float(fSlow3)))))) * (0.0f - (2.0f * fSlow35)));
		float fSlow37 = synth_piano_faustpower2_f(fSlow35);
		float fSlow38 = std::pow(10.0f, (fConst2 * float(getValuer2db(float(fSlow3)))));
		float fSlow39 = (std::cos((fConst1 * (fSlow6 * float(getValueSecondPartialFactor(float(fSlow3)))))) * (0.0f - (2.0f * fSlow38)));
		float fSlow40 = synth_piano_faustpower2_f(fSlow38);
		float fSlow41 = (fSlow7 * (0.0f - (2.0f * fSlow9)));
		float fSlow42 = synth_piano_faustpower2_f(fSlow9);
		float fSlow43 = (0.0f - (2.0f * fSlow10));
		float fSlow44 = synth_piano_faustpower2_f(fSlow10);
		float fSlow45 = ((fSlow8 * fSlow42) + (fSlow44 * fSlow11));
		float fSlow46 = float(fHslider5);
		float fSlow47 = float(getValueStiffnessCoefficient(float(fSlow3)));
		float fSlow48 = (13.6899996f * (synth_piano_faustpower2_f(fSlow46) * synth_piano_faustpower2_f(fSlow47)));
		float fSlow49 = (fSlow48 + -1.0f);
		float fSlow50 = (5.0f * (float(getValueDetuningHz(float(fSlow3))) * float(fHslider6)));
		float fSlow51 = (fSlow6 + std::pow(2.0f, (0.0833333358f * (fSlow3 + -129.0f))));
		float fSlow52 = (440.0f * fSlow51);
		float fSlow53 = (fSlow50 + fSlow52);
		float fSlow54 = (fConst9 * fSlow53);
		float fSlow55 = std::sin(fSlow54);
		float fSlow56 = (fSlow46 * fSlow47);
		float fSlow57 = (7.4000001f * fSlow56);
		float fSlow58 = (fSlow48 + 1.0f);
		float fSlow59 = std::cos(fSlow54);
		float fSlow60 = (3.0f * std::atan2((fSlow49 * fSlow55), (fSlow57 + (fSlow58 * fSlow59))));
		float fSlow61 = float(getValueSingleStringZero(float(fSlow3)));
		float fSlow62 = std::pow(10.0f, (0.000113636364f * (float(getValueSingleStringDecayRate(float(fSlow3))) / fSlow6)));
		float fSlow63 = float(getValueSingleStringPole(float(fSlow3)));
		float fSlow64 = (1.0f - fSlow63);
		float fSlow65 = ((fSlow61 * fSlow62) * fSlow64);
		float fSlow66 = (1.0f - fSlow61);
		float fSlow67 = (fSlow63 * fSlow66);
		float fSlow68 = (3.0f * fSlow67);
		float fSlow69 = (fSlow65 - fSlow68);
		float fSlow70 = (fSlow67 - fSlow65);
		float fSlow71 = (4.0f * fSlow70);
		float fSlow72 = (fSlow69 + fSlow71);
		float fSlow73 = (fSlow62 * fSlow64);
		float fSlow74 = ((3.0f * fSlow66) - fSlow73);
		float fSlow75 = (((fSlow59 * fSlow69) / fSlow74) + 1.0f);
		float fSlow76 = ((fSlow61 + fSlow73) + -1.0f);
		float fSlow77 = (4.0f * fSlow76);
		float fSlow78 = (((fSlow77 + (fSlow59 * fSlow72)) / fSlow74) + 1.0f);
		float fSlow79 = (synth_piano_faustpower2_f(fSlow55) * fSlow69);
		float fSlow80 = synth_piano_faustpower2_f(fSlow74);
		float fSlow81 = (fConst8 * (((fSlow60 + std::atan2((((0.0f - ((fSlow55 * fSlow72) / fSlow74)) * fSlow75) + (((fSlow55 * fSlow69) * fSlow78) / fSlow74)), ((fSlow75 * fSlow78) + ((fSlow79 * fSlow72) / fSlow80)))) + 6.28318548f) / fSlow53));
		float fSlow82 = std::floor(fSlow81);
		float fSlow83 = (fSlow81 - fSlow82);
		float fSlow84 = (0.00100000005f * ((0.999599993f * fSlow27) + (0.899999976f * (float(getValueReleaseLoopGain(float(fSlow3))) * fSlow25))));
		float fSlow85 = (1.16415322e-09f * (fSlow16 * float(((fSlow3 >= 45.0f) & (fSlow3 < 88.0f)))));
		float fSlow86 = (3.70000005f * fSlow56);
		float fSlow87 = ((fSlow65 + fSlow71) - fSlow68);
		float fSlow88 = (((fSlow77 + (fSlow59 * fSlow87)) / fSlow74) + 1.0f);
		int iSlow89 = int((fConst8 * (((fSlow60 + std::atan2((0.0f - ((fSlow55 * ((fSlow87 * fSlow75) - (fSlow69 * fSlow88))) / fSlow74)), ((fSlow75 * fSlow88) + ((fSlow79 * fSlow87) / fSlow80)))) + 6.28318548f) / fSlow53)));
		int iSlow90 = std::min<int>(4097, std::max<int>(0, (iSlow89 + 1)));
		float fSlow91 = (1.0f / fSlow74);
		float fSlow92 = (fSlow52 - fSlow50);
		float fSlow93 = (fConst9 * fSlow92);
		float fSlow94 = std::sin(fSlow93);
		float fSlow95 = std::cos(fSlow93);
		float fSlow96 = (3.0f * std::atan2((fSlow49 * fSlow94), (fSlow57 + (fSlow58 * fSlow95))));
		float fSlow97 = (((fSlow95 * fSlow69) / fSlow74) + 1.0f);
		float fSlow98 = (((fSlow77 + (fSlow95 * fSlow72)) / fSlow74) + 1.0f);
		float fSlow99 = (synth_piano_faustpower2_f(fSlow94) * fSlow69);
		float fSlow100 = (fConst8 * (((fSlow96 + std::atan2((((0.0f - ((fSlow94 * fSlow72) / fSlow74)) * fSlow97) + (((fSlow94 * fSlow69) * fSlow98) / fSlow74)), ((fSlow97 * fSlow98) + ((fSlow99 * fSlow72) / fSlow80)))) + 6.28318548f) / fSlow92));
		float fSlow101 = std::floor(fSlow100);
		float fSlow102 = (fSlow100 - fSlow101);
		float fSlow103 = (((fSlow77 + (fSlow95 * fSlow87)) / fSlow74) + 1.0f);
		int iSlow104 = int((fConst8 * (((fSlow96 + std::atan2((0.0f - ((fSlow94 * ((fSlow87 * fSlow97) - (fSlow69 * fSlow103))) / fSlow74)), ((fSlow97 * fSlow103) + ((fSlow99 * fSlow87) / fSlow80)))) + 6.28318548f) / fSlow92)));
		int iSlow105 = std::min<int>(4097, std::max<int>(0, (iSlow104 + 1)));
		float fSlow106 = (fConst1 * fSlow51);
		float fSlow107 = std::sin(fSlow106);
		float fSlow108 = std::cos(fSlow106);
		float fSlow109 = (3.0f * std::atan2((fSlow49 * fSlow107), (fSlow57 + (fSlow58 * fSlow108))));
		float fSlow110 = (((fSlow108 * fSlow69) / fSlow74) + 1.0f);
		float fSlow111 = (((fSlow77 + (fSlow108 * fSlow72)) / fSlow74) + 1.0f);
		float fSlow112 = (synth_piano_faustpower2_f(fSlow107) * fSlow69);
		float fSlow113 = (fConst10 * (((fSlow109 + std::atan2((((0.0f - ((fSlow107 * fSlow72) / fSlow74)) * fSlow110) + (((fSlow107 * fSlow69) * fSlow111) / fSlow74)), ((fSlow110 * fSlow111) + ((fSlow112 * fSlow72) / fSlow80)))) + 6.28318548f) / fSlow51));
		float fSlow114 = std::floor(fSlow113);
		float fSlow115 = (fSlow114 + (1.0f - fSlow113));
		float fSlow116 = (((fSlow77 + (fSlow108 * fSlow87)) / fSlow74) + 1.0f);
		int iSlow117 = int((fConst10 * (((fSlow109 + std::atan2((0.0f - ((fSlow107 * ((fSlow87 * fSlow110) - (fSlow69 * fSlow116))) / fSlow74)), ((fSlow110 * fSlow116) + ((fSlow112 * fSlow87) / fSlow80)))) + 6.28318548f) / fSlow51)));
		int iSlow118 = std::min<int>(4097, std::max<int>(0, iSlow117));
		float fSlow119 = (fSlow113 - fSlow114);
		int iSlow120 = std::min<int>(4097, std::max<int>(0, (iSlow117 + 1)));
		float fSlow121 = (fSlow101 + (1.0f - fSlow100));
		int iSlow122 = std::min<int>(4097, std::max<int>(0, iSlow104));
		float fSlow123 = (fSlow82 + (1.0f - fSlow81));
		int iSlow124 = std::min<int>(4097, std::max<int>(0, iSlow89));
		float fSlow125 = (440.0f * fSlow6);
		float fSlow126 = (fSlow125 + fSlow50);
		float fSlow127 = (fConst9 * fSlow126);
		float fSlow128 = std::sin(fSlow127);
		float fSlow129 = std::cos(fSlow127);
		float fSlow130 = (3.0f * std::atan2((fSlow49 * fSlow128), (fSlow57 + (fSlow58 * fSlow129))));
		float fSlow131 = (((fSlow129 * fSlow69) / fSlow74) + 1.0f);
		float fSlow132 = (((fSlow77 + (fSlow129 * fSlow72)) / fSlow74) + 1.0f);
		float fSlow133 = (synth_piano_faustpower2_f(fSlow128) * fSlow69);
		float fSlow134 = (fConst8 * (((fSlow130 + std::atan2((((0.0f - ((fSlow128 * fSlow72) / fSlow74)) * fSlow131) + (((fSlow128 * fSlow69) * fSlow132) / fSlow74)), ((fSlow131 * fSlow132) + ((fSlow133 * fSlow72) / fSlow80)))) + 6.28318548f) / fSlow126));
		float fSlow135 = std::floor(fSlow134);
		float fSlow136 = (fSlow134 - fSlow135);
		float fSlow137 = (1.62981451e-09f * (fSlow16 * float((fSlow3 < 45.0f))));
		float fSlow138 = (((fSlow77 + (fSlow129 * fSlow87)) / fSlow74) + 1.0f);
		int iSlow139 = int((fConst8 * (((fSlow130 + std::atan2((0.0f - ((fSlow128 * ((fSlow87 * fSlow131) - (fSlow69 * fSlow138))) / fSlow74)), ((fSlow131 * fSlow138) + ((fSlow133 * fSlow87) / fSlow80)))) + 6.28318548f) / fSlow126)));
		int iSlow140 = std::min<int>(4097, std::max<int>(0, (iSlow139 + 1)));
		float fSlow141 = (fSlow125 - fSlow50);
		float fSlow142 = (fConst9 * fSlow141);
		float fSlow143 = std::sin(fSlow142);
		float fSlow144 = std::cos(fSlow142);
		float fSlow145 = (3.0f * std::atan2((fSlow49 * fSlow143), (fSlow57 + (fSlow58 * fSlow144))));
		float fSlow146 = (((fSlow144 * fSlow69) / fSlow74) + 1.0f);
		float fSlow147 = ((((fSlow144 * fSlow72) + fSlow77) / fSlow74) + 1.0f);
		float fSlow148 = (synth_piano_faustpower2_f(fSlow143) * fSlow69);
		float fSlow149 = (fConst8 * (((fSlow145 + std::atan2((((0.0f - ((fSlow143 * fSlow72) / fSlow74)) * fSlow146) + (((fSlow143 * fSlow69) * fSlow147) / fSlow74)), ((fSlow146 * fSlow147) + ((fSlow148 * fSlow72) / fSlow80)))) + 6.28318548f) / fSlow141));
		float fSlow150 = std::floor(fSlow149);
		float fSlow151 = (fSlow149 - fSlow150);
		float fSlow152 = (((fSlow77 + (fSlow144 * fSlow87)) / fSlow74) + 1.0f);
		int iSlow153 = int((fConst8 * (((fSlow145 + std::atan2((0.0f - ((fSlow143 * ((fSlow146 * fSlow87) - (fSlow69 * fSlow152))) / fSlow74)), ((fSlow146 * fSlow152) + ((fSlow148 * fSlow87) / fSlow80)))) + 6.28318548f) / fSlow141)));
		int iSlow154 = std::min<int>(4097, std::max<int>(0, (iSlow153 + 1)));
		float fSlow155 = (fSlow150 + (1.0f - fSlow149));
		int iSlow156 = std::min<int>(4097, std::max<int>(0, iSlow153));
		float fSlow157 = (fSlow135 + (1.0f - fSlow134));
		int iSlow158 = std::min<int>(4097, std::max<int>(0, iSlow139));
		float fSlow159 = float(getValueEQBandWidthFactor(float(fSlow3)));
		float fSlow160 = (synth_piano_faustpower2_f(fSlow6) * synth_piano_faustpower2_f(fSlow159));
		float fSlow161 = (0.5f - (fConst12 * fSlow160));
		float fSlow162 = float(getValueEQGain(float(fSlow3)));
		float fSlow163 = ((0.0f - (fConst13 * (fSlow6 * fSlow159))) * std::cos((fConst1 * (fSlow6 / float(getValueStrikePosition(float(fSlow3)))))));
		float fSlow164 = (fConst14 * fSlow160);
		int iSlow165 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst15 * (float(fHslider7) / fSlow6)))));
		float fSlow166 = float(fHslider8);
		float fSlow167 = std::exp((fConst20 / fSlow166));
		float fSlow168 = synth_piano_faustpower2_f(fSlow167);
		float fSlow169 = (1.0f - (fConst17 * fSlow168));
		float fSlow170 = (1.0f - fSlow168);
		float fSlow171 = (fSlow169 / fSlow170);
		float fSlow172 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow169) / synth_piano_faustpower2_f(fSlow170)) + -1.0f)));
		float fSlow173 = (fSlow171 - fSlow172);
		float fSlow174 = (fSlow167 * (fSlow172 + (1.0f - fSlow171)));
		float fSlow175 = ((std::exp((fConst21 / fSlow166)) / fSlow167) + -1.0f);
		float fSlow176 = std::exp((fConst30 / fSlow166));
		float fSlow177 = synth_piano_faustpower2_f(fSlow176);
		float fSlow178 = (1.0f - (fConst17 * fSlow177));
		float fSlow179 = (1.0f - fSlow177);
		float fSlow180 = (fSlow178 / fSlow179);
		float fSlow181 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow178) / synth_piano_faustpower2_f(fSlow179)) + -1.0f)));
		float fSlow182 = (fSlow180 - fSlow181);
		float fSlow183 = (fSlow176 * (fSlow181 + (1.0f - fSlow180)));
		float fSlow184 = ((std::exp((fConst31 / fSlow166)) / fSlow176) + -1.0f);
		float fSlow185 = std::exp((fConst37 / fSlow166));
		float fSlow186 = synth_piano_faustpower2_f(fSlow185);
		float fSlow187 = (1.0f - (fConst17 * fSlow186));
		float fSlow188 = (1.0f - fSlow186);
		float fSlow189 = (fSlow187 / fSlow188);
		float fSlow190 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow187) / synth_piano_faustpower2_f(fSlow188)) + -1.0f)));
		float fSlow191 = (fSlow189 - fSlow190);
		float fSlow192 = (fSlow185 * (fSlow190 + (1.0f - fSlow189)));
		float fSlow193 = ((std::exp((fConst38 / fSlow166)) / fSlow185) + -1.0f);
		float fSlow194 = (12.0f * (1.0f - fSlow0));
		float fSlow195 = std::exp((fConst44 / fSlow166));
		float fSlow196 = synth_piano_faustpower2_f(fSlow195);
		float fSlow197 = (1.0f - (fConst17 * fSlow196));
		float fSlow198 = (1.0f - fSlow196);
		float fSlow199 = (fSlow197 / fSlow198);
		float fSlow200 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow197) / synth_piano_faustpower2_f(fSlow198)) + -1.0f)));
		float fSlow201 = (fSlow199 - fSlow200);
		float fSlow202 = (fSlow195 * (fSlow200 + (1.0f - fSlow199)));
		float fSlow203 = ((std::exp((fConst45 / fSlow166)) / fSlow195) + -1.0f);
		float fSlow204 = std::exp((fConst51 / fSlow166));
		float fSlow205 = synth_piano_faustpower2_f(fSlow204);
		float fSlow206 = (1.0f - (fConst17 * fSlow205));
		float fSlow207 = (1.0f - fSlow205);
		float fSlow208 = (fSlow206 / fSlow207);
		float fSlow209 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow206) / synth_piano_faustpower2_f(fSlow207)) + -1.0f)));
		float fSlow210 = (fSlow208 - fSlow209);
		float fSlow211 = (fSlow204 * (fSlow209 + (1.0f - fSlow208)));
		float fSlow212 = ((std::exp((fConst52 / fSlow166)) / fSlow204) + -1.0f);
		float fSlow213 = std::exp((fConst58 / fSlow166));
		float fSlow214 = synth_piano_faustpower2_f(fSlow213);
		float fSlow215 = (1.0f - (fConst17 * fSlow214));
		float fSlow216 = (1.0f - fSlow214);
		float fSlow217 = (fSlow215 / fSlow216);
		float fSlow218 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow215) / synth_piano_faustpower2_f(fSlow216)) + -1.0f)));
		float fSlow219 = (fSlow217 - fSlow218);
		float fSlow220 = (fSlow213 * (fSlow218 + (1.0f - fSlow217)));
		float fSlow221 = ((std::exp((fConst59 / fSlow166)) / fSlow213) + -1.0f);
		float fSlow222 = std::exp((fConst65 / fSlow166));
		float fSlow223 = synth_piano_faustpower2_f(fSlow222);
		float fSlow224 = (1.0f - (fConst17 * fSlow223));
		float fSlow225 = (1.0f - fSlow223);
		float fSlow226 = (fSlow224 / fSlow225);
		float fSlow227 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow224) / synth_piano_faustpower2_f(fSlow225)) + -1.0f)));
		float fSlow228 = (fSlow226 - fSlow227);
		float fSlow229 = (fSlow222 * (fSlow227 + (1.0f - fSlow226)));
		float fSlow230 = ((std::exp((fConst66 / fSlow166)) / fSlow222) + -1.0f);
		float fSlow231 = std::exp((fConst72 / fSlow166));
		float fSlow232 = synth_piano_faustpower2_f(fSlow231);
		float fSlow233 = (1.0f - (fConst17 * fSlow232));
		float fSlow234 = (1.0f - fSlow232);
		float fSlow235 = (fSlow233 / fSlow234);
		float fSlow236 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow233) / synth_piano_faustpower2_f(fSlow234)) + -1.0f)));
		float fSlow237 = (fSlow235 - fSlow236);
		float fSlow238 = (fSlow231 * (fSlow236 + (1.0f - fSlow235)));
		float fSlow239 = ((std::exp((fConst73 / fSlow166)) / fSlow231) + -1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec10[0] = (fSlow2 + (0.999000013f * fRec10[1]));
			iRec21[0] = ((1103515245 * iRec21[1]) + 12345);
			iRec23[0] = ((iSlow21 * iRec23[1]) + 1);
			float fTemp0 = float((iRec23[0] + -1));
			float fTemp1 = float(((fTemp0 < 2.0f) & iSlow22));
			float fTemp2 = ((0.0301973838f * fTemp1) + (fSlow23 * float(((fTemp0 >= 2.0f) | iSlow24))));
			fRec22[0] = ((fRec22[1] * fTemp2) + (0.150000006f * (fTemp1 * (1.0f - fTemp2))));
			int iTemp3 = (fTemp0 < fSlow30);
			float fTemp4 = ((fSlow29 * float(iTemp3)) + (fConst7 * float((iSlow21 * (fTemp0 >= fSlow30)))));
			fRec24[0] = ((fRec24[1] * (fSlow26 + fTemp4)) + (fSlow31 * (((1.0f - fTemp4) - fSlow26) * float((iTemp3 & iSlow22)))));
			float fTemp5 = (float(iRec21[0]) * (fRec22[0] + fRec24[0]));
			fVec0[0] = (fSlow19 * fTemp5);
			float fTemp6 = (0.0f - ((0.5f * fVec0[1]) + (fSlow32 * fTemp5)));
			fVec1[0] = fTemp6;
			fRec20[0] = (((fSlow17 * fTemp6) + (fSlow33 * fVec1[1])) - (fSlow4 * fRec20[1]));
			fRec19[0] = ((fSlow16 * fRec20[0]) + (fSlow34 * fRec19[1]));
			fRec18[0] = ((fSlow16 * fRec19[0]) + (fSlow34 * fRec18[1]));
			fRec17[0] = ((fSlow16 * fRec18[0]) + (fSlow34 * fRec17[1]));
			fRec16[0] = ((fSlow16 * fRec17[0]) + (fSlow34 * fRec16[1]));
			fRec15[0] = ((fSlow13 * (0.0f - (0.5f * (fRec16[1] - fRec16[0])))) - ((fSlow36 * fRec15[1]) + (fSlow37 * fRec15[2])));
			fRec14[0] = ((fSlow13 * fRec15[0]) - ((fSlow39 * fRec14[1]) + (fSlow40 * fRec14[2])));
			fRec13[0] = (fRec14[0] - ((fSlow41 * fRec13[1]) + (fSlow42 * fRec13[2])));
			fRec12[0] = (((fSlow7 * ((fSlow12 * fRec13[1]) - (fSlow43 * fRec12[1]))) + (fRec13[0] + (fSlow45 * fRec13[2]))) - (fSlow44 * fRec12[2]));
			fRec11[0] = ((fSlow5 * fRec12[0]) - (fSlow4 * fRec11[1]));
			fRec32[0] = (fSlow84 + (0.999000013f * fRec32[1]));
			fRec37[0] = ((fSlow85 * fTemp5) + (fSlow34 * fRec37[1]));
			fRec36[0] = ((fSlow16 * fRec37[0]) + (fSlow34 * fRec36[1]));
			fRec35[0] = ((fSlow16 * fRec36[0]) + (fSlow34 * fRec35[1]));
			fRec34[0] = ((fSlow16 * fRec35[0]) + (fSlow34 * fRec34[1]));
			fRec33[0] = (((fSlow17 * fRec34[0]) + (fSlow33 * fRec34[1])) - (fSlow4 * fRec33[1]));
			float fTemp7 = (fRec32[0] * (fRec33[0] + fRec25[1]));
			fVec2[0] = fTemp7;
			fRec31[0] = (fVec2[1] + (fSlow86 * (fTemp7 - fRec31[1])));
			fRec30[0] = (fRec31[1] + (fSlow86 * (fRec31[0] - fRec30[1])));
			fRec29[(IOTA & 8191)] = (fRec30[1] + (fSlow86 * (fRec30[0] - fRec29[((IOTA - 1) & 8191)])));
			float fTemp8 = (fSlow83 * fRec29[((IOTA - iSlow90) & 8191)]);
			float fTemp9 = (fRec33[0] + (fRec32[0] * fRec27[1]));
			fVec3[0] = fTemp9;
			fRec41[0] = (fVec3[1] + (fSlow86 * (fTemp9 - fRec41[1])));
			fRec40[0] = (fRec41[1] + (fSlow86 * (fRec41[0] - fRec40[1])));
			fRec39[(IOTA & 8191)] = (fRec40[1] + (fSlow86 * (fRec40[0] - fRec39[((IOTA - 1) & 8191)])));
			float fTemp10 = (fSlow102 * fRec39[((IOTA - iSlow105) & 8191)]);
			float fTemp11 = (fRec33[0] + (fRec32[0] * fRec26[1]));
			fVec4[0] = fTemp11;
			fRec44[0] = (fVec4[1] + (fSlow86 * (fTemp11 - fRec44[1])));
			fRec43[0] = (fRec44[1] + (fSlow86 * (fRec44[0] - fRec43[1])));
			fRec42[(IOTA & 8191)] = (fRec43[1] + (fSlow86 * (fRec43[0] - fRec42[((IOTA - 1) & 8191)])));
			float fTemp12 = ((fSlow115 * fRec42[((IOTA - iSlow118) & 8191)]) + (fSlow119 * fRec42[((IOTA - iSlow120) & 8191)]));
			float fTemp13 = (fSlow121 * fRec39[((IOTA - iSlow122) & 8191)]);
			float fTemp14 = (fSlow123 * fRec29[((IOTA - iSlow124) & 8191)]);
			float fTemp15 = (fTemp8 + ((fTemp10 + (fTemp12 + fTemp13)) + fTemp14));
			fVec5[0] = fTemp15;
			fRec38[0] = (fSlow91 * ((2.0f * ((fSlow76 * fTemp15) + (fSlow70 * fVec5[1]))) - (fSlow69 * fRec38[1])));
			fRec25[0] = (fTemp8 + (fRec38[0] + fTemp14));
			fRec26[0] = (fRec38[0] + fTemp12);
			fRec27[0] = (fRec38[0] + (fTemp13 + fTemp10));
			float fRec28 = fTemp15;
			fRec55[0] = ((fSlow137 * fTemp5) + (fSlow34 * fRec55[1]));
			fRec54[0] = ((fSlow16 * fRec55[0]) + (fSlow34 * fRec54[1]));
			fRec53[0] = ((fSlow16 * fRec54[0]) + (fSlow34 * fRec53[1]));
			fRec52[0] = ((fSlow16 * fRec53[0]) + (fSlow34 * fRec52[1]));
			fRec51[0] = (((fSlow17 * fRec52[0]) + (fSlow33 * fRec52[1])) - (fSlow4 * fRec51[1]));
			float fTemp16 = (fRec32[0] * (fRec51[0] + fRec45[1]));
			fVec6[0] = fTemp16;
			fRec50[0] = (fVec6[1] + (fSlow86 * (fTemp16 - fRec50[1])));
			fRec49[0] = (fRec50[1] + (fSlow86 * (fRec50[0] - fRec49[1])));
			fRec48[(IOTA & 8191)] = (fRec49[1] + (fSlow86 * (fRec49[0] - fRec48[((IOTA - 1) & 8191)])));
			float fTemp17 = (fSlow136 * fRec48[((IOTA - iSlow140) & 8191)]);
			float fTemp18 = (fRec51[0] + (fRec32[0] * fRec46[1]));
			fVec7[0] = fTemp18;
			fRec59[0] = (fVec7[1] + (fSlow86 * (fTemp18 - fRec59[1])));
			fRec58[0] = (fRec59[1] + (fSlow86 * (fRec59[0] - fRec58[1])));
			fRec57[(IOTA & 8191)] = (fRec58[1] + (fSlow86 * (fRec58[0] - fRec57[((IOTA - 1) & 8191)])));
			float fTemp19 = (fSlow151 * fRec57[((IOTA - iSlow154) & 8191)]);
			float fTemp20 = (fSlow155 * fRec57[((IOTA - iSlow156) & 8191)]);
			float fTemp21 = (fSlow157 * fRec48[((IOTA - iSlow158) & 8191)]);
			float fTemp22 = (fTemp19 + (fTemp20 + (fTemp21 + fTemp17)));
			fVec8[0] = fTemp22;
			fRec56[0] = (fSlow91 * ((2.0f * ((fSlow76 * fTemp22) + (fSlow70 * fVec8[1]))) - (fSlow69 * fRec56[1])));
			fRec45[0] = (fTemp17 + (fRec56[0] + fTemp21));
			fRec46[0] = (fTemp19 + (fRec56[0] + fTemp20));
			float fRec47 = fTemp22;
			fRec60[0] = ((fSlow162 * fRec47) - ((fSlow163 * fRec60[1]) + (fSlow164 * fRec60[2])));
			fRec61[0] = ((fSlow162 * fRec28) - ((fSlow163 * fRec61[1]) + (fSlow164 * fRec61[2])));
			float fTemp23 = (fRec11[0] + (fRec28 + (fRec47 + (fSlow161 * ((fRec60[0] - fRec60[2]) + (fRec61[0] - fRec61[2]))))));
			fVec9[(IOTA & 4095)] = fTemp23;
			float fTemp24 = fVec9[((IOTA - iSlow165) & 4095)];
			fVec10[(IOTA & 4095)] = (fSlow1 * (fRec10[0] * fTemp24));
			float fTemp25 = (0.300000012f * fVec10[((IOTA - iConst16) & 4095)]);
			fRec63[0] = (0.0f - (fConst23 * ((fConst24 * fRec63[1]) - (fRec5[1] + fRec5[2]))));
			fRec62[0] = ((fSlow173 * fRec62[1]) + (fSlow174 * (fRec5[1] + (fSlow175 * fRec63[0]))));
			fVec11[(IOTA & 16383)] = ((0.353553385f * fRec62[0]) + 9.99999968e-21f);
			float fTemp26 = ((0.600000024f * fRec8[1]) + (fTemp25 + fVec11[((IOTA - iConst26) & 16383)]));
			fVec12[(IOTA & 4095)] = fTemp26;
			fRec8[0] = fVec12[((IOTA - iConst27) & 4095)];
			float fRec9 = (0.0f - (0.600000024f * fTemp26));
			fRec67[0] = (0.0f - (fConst23 * ((fConst24 * fRec67[1]) - (fRec1[1] + fRec1[2]))));
			fRec66[0] = ((fSlow182 * fRec66[1]) + (fSlow183 * (fRec1[1] + (fSlow184 * fRec67[0]))));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec66[0]) + 9.99999968e-21f);
			float fTemp27 = (fTemp25 + ((0.600000024f * fRec64[1]) + fVec13[((IOTA - iConst33) & 32767)]));
			fVec14[(IOTA & 4095)] = fTemp27;
			fRec64[0] = fVec14[((IOTA - iConst34) & 4095)];
			float fRec65 = (0.0f - (0.600000024f * fTemp27));
			fRec71[0] = (0.0f - (fConst23 * ((fConst24 * fRec71[1]) - (fRec6[1] + fRec6[2]))));
			fRec70[0] = ((fSlow191 * fRec70[1]) + (fSlow192 * (fRec6[1] + (fSlow193 * fRec71[0]))));
			fVec15[(IOTA & 16383)] = ((0.353553385f * fRec70[0]) + 9.99999968e-21f);
			fVec16[(IOTA & 4095)] = (fSlow194 * (fRec10[0] * fTemp23));
			float fTemp28 = (0.300000012f * fVec16[((IOTA - iConst16) & 4095)]);
			float fTemp29 = (fVec15[((IOTA - iConst40) & 16383)] - (fTemp28 + (0.600000024f * fRec68[1])));
			fVec17[(IOTA & 2047)] = fTemp29;
			fRec68[0] = fVec17[((IOTA - iConst41) & 2047)];
			float fRec69 = (0.600000024f * fTemp29);
			fRec75[0] = (0.0f - (fConst23 * ((fConst24 * fRec75[1]) - (fRec2[1] + fRec2[2]))));
			fRec74[0] = ((fSlow201 * fRec74[1]) + (fSlow202 * (fRec2[1] + (fSlow203 * fRec75[0]))));
			fVec18[(IOTA & 16383)] = ((0.353553385f * fRec74[0]) + 9.99999968e-21f);
			float fTemp30 = (fVec18[((IOTA - iConst47) & 16383)] - (fTemp28 + (0.600000024f * fRec72[1])));
			fVec19[(IOTA & 4095)] = fTemp30;
			fRec72[0] = fVec19[((IOTA - iConst48) & 4095)];
			float fRec73 = (0.600000024f * fTemp30);
			fRec79[0] = (0.0f - (fConst23 * ((fConst24 * fRec79[1]) - (fRec4[1] + fRec4[2]))));
			fRec78[0] = ((fSlow210 * fRec78[1]) + (fSlow211 * (fRec4[1] + (fSlow212 * fRec79[0]))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec78[0]) + 9.99999968e-21f);
			float fTemp31 = ((fTemp28 + fVec20[((IOTA - iConst54) & 16383)]) - (0.600000024f * fRec76[1]));
			fVec21[(IOTA & 4095)] = fTemp31;
			fRec76[0] = fVec21[((IOTA - iConst55) & 4095)];
			float fRec77 = (0.600000024f * fTemp31);
			fRec83[0] = (0.0f - (fConst23 * ((fConst24 * fRec83[1]) - (fRec0[1] + fRec0[2]))));
			fRec82[0] = ((fSlow219 * fRec82[1]) + (fSlow220 * (fRec0[1] + (fSlow221 * fRec83[0]))));
			fVec22[(IOTA & 16383)] = ((0.353553385f * fRec82[0]) + 9.99999968e-21f);
			float fTemp32 = ((fVec22[((IOTA - iConst61) & 16383)] + fTemp28) - (0.600000024f * fRec80[1]));
			fVec23[(IOTA & 2047)] = fTemp32;
			fRec80[0] = fVec23[((IOTA - iConst62) & 2047)];
			float fRec81 = (0.600000024f * fTemp32);
			fRec87[0] = (0.0f - (fConst23 * ((fConst24 * fRec87[1]) - (fRec7[1] + fRec7[2]))));
			fRec86[0] = ((fSlow228 * fRec86[1]) + (fSlow229 * (fRec7[1] + (fSlow230 * fRec87[0]))));
			fVec24[(IOTA & 32767)] = ((0.353553385f * fRec86[0]) + 9.99999968e-21f);
			float fTemp33 = (((0.600000024f * fRec84[1]) + fVec24[((IOTA - iConst68) & 32767)]) - fTemp25);
			fVec25[(IOTA & 2047)] = fTemp33;
			fRec84[0] = fVec25[((IOTA - iConst69) & 2047)];
			float fRec85 = (0.0f - (0.600000024f * fTemp33));
			fRec91[0] = (0.0f - (fConst23 * ((fConst24 * fRec91[1]) - (fRec3[1] + fRec3[2]))));
			fRec90[0] = ((fSlow237 * fRec90[1]) + (fSlow238 * (fRec3[1] + (fSlow239 * fRec91[0]))));
			fVec26[(IOTA & 32767)] = ((0.353553385f * fRec90[0]) + 9.99999968e-21f);
			float fTemp34 = (((0.600000024f * fRec88[1]) + fVec26[((IOTA - iConst75) & 32767)]) - fTemp25);
			fVec27[(IOTA & 4095)] = fTemp34;
			fRec88[0] = fVec27[((IOTA - iConst76) & 4095)];
			float fRec89 = (0.0f - (0.600000024f * fTemp34));
			float fTemp35 = (fRec88[1] + fRec84[1]);
			float fTemp36 = (fRec77 + (fRec81 + fTemp35));
			fRec0[0] = (fRec8[1] + (fRec64[1] + (fRec68[1] + (fRec72[1] + (fRec76[1] + (fRec80[1] + (fRec85 + (fRec89 + (fRec9 + (fRec65 + (fRec69 + (fRec73 + fTemp36))))))))))));
			float fTemp37 = (fRec81 + fRec77);
			fRec1[0] = ((fRec68[1] + (fRec72[1] + (fRec76[1] + (fRec80[1] + (fRec69 + (fRec73 + fTemp37)))))) - (fRec8[1] + (fRec64[1] + (fRec85 + (fRec89 + (fRec9 + (fRec65 + fTemp35)))))));
			fRec2[0] = ((fRec8[1] + (fRec64[1] + (fRec76[1] + (fRec80[1] + (fRec9 + (fRec65 + fTemp37)))))) - (fRec68[1] + (fRec72[1] + (fRec85 + (fRec89 + (fRec69 + (fRec73 + fTemp35)))))));
			fRec3[0] = ((fRec76[1] + (fRec80[1] + (fRec85 + (fRec89 + fTemp36)))) - (fRec8[1] + (fRec64[1] + (fRec68[1] + (fRec72[1] + (fRec9 + (fRec65 + (fRec73 + fRec69))))))));
			float fTemp38 = (fRec81 + fRec88[1]);
			float fTemp39 = (fRec77 + fRec84[1]);
			fRec4[0] = ((fRec64[1] + (fRec72[1] + (fRec80[1] + (fRec89 + (fRec65 + (fRec73 + fTemp38)))))) - (fRec8[1] + (fRec68[1] + (fRec76[1] + (fRec85 + (fRec9 + (fRec69 + fTemp39)))))));
			float fTemp40 = (fRec81 + fRec84[1]);
			float fTemp41 = (fRec77 + fRec88[1]);
			fRec5[0] = ((fRec8[1] + (fRec72[1] + (fRec80[1] + (fRec85 + (fRec9 + (fRec73 + fTemp40)))))) - (fRec64[1] + (fRec68[1] + (fRec76[1] + (fRec89 + (fRec65 + (fRec69 + fTemp41)))))));
			fRec6[0] = ((fRec64[1] + (fRec68[1] + (fRec80[1] + (fRec85 + (fRec65 + (fRec69 + fTemp40)))))) - (fRec8[1] + (fRec72[1] + (fRec76[1] + (fRec89 + (fRec9 + (fRec73 + fTemp41)))))));
			fRec7[0] = ((fRec8[1] + (fRec68[1] + (fRec80[1] + (fRec89 + (fRec9 + (fRec69 + fTemp38)))))) - (fRec64[1] + (fRec72[1] + (fRec76[1] + (fRec85 + (fRec65 + (fRec73 + fTemp39)))))));
			float fTemp42 = (1.0f - fRec10[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow194 * (fTemp42 * fTemp23))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow1 * (fTemp42 * fTemp24))));
			fRec10[1] = fRec10[0];
			iRec21[1] = iRec21[0];
			iRec23[1] = iRec23[0];
			fRec22[1] = fRec22[0];
			fRec24[1] = fRec24[0];
			fVec0[1] = fVec0[0];
			fVec1[1] = fVec1[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec32[1] = fRec32[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fVec2[1] = fVec2[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			IOTA = (IOTA + 1);
			fVec3[1] = fVec3[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fVec4[1] = fVec4[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fVec5[1] = fVec5[0];
			fRec38[1] = fRec38[0];
			fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			fRec27[1] = fRec27[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fVec6[1] = fVec6[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fVec7[1] = fVec7[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fVec8[1] = fVec8[0];
			fRec56[1] = fRec56[0];
			fRec45[1] = fRec45[0];
			fRec46[1] = fRec46[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
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
