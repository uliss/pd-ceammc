/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.harpsichord"
version: "1.0"
Code generated with Faust 2.28.6 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_harpsichord_H__
#define  __synth_harpsichord_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_harpsichord_dsp.h **************************/
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

class synth_harpsichord_dsp {

    public:

        synth_harpsichord_dsp() {}
        virtual ~synth_harpsichord_dsp() {}

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
        virtual synth_harpsichord_dsp* clone() = 0;
    
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

class decorator_dsp : public synth_harpsichord_dsp {

    protected:

        synth_harpsichord_dsp* fDSP;

    public:

        decorator_dsp(synth_harpsichord_dsp* synth_harpsichord_dsp = nullptr):fDSP(synth_harpsichord_dsp) {}
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
    
        virtual synth_harpsichord_dsp* createDSPInstance() = 0;
    
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
/**************************  END  synth_harpsichord_dsp.h **************************/
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
struct synth_harpsichord : public synth_harpsichord_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include "harpsichord.h"
#include <algorithm>
#include <cmath>
#include <math.h>

class synth_harpsichordSIG0 {
	
  private:
	
	int iRec4[2];
	
  public:
	
	int getNumInputssynth_harpsichordSIG0() {
		return 0;
	}
	int getNumOutputssynth_harpsichordSIG0() {
		return 1;
	}
	int getInputRatesynth_harpsichordSIG0(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatesynth_harpsichordSIG0(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 0;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	void instanceInitsynth_harpsichordSIG0(int sample_rate) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec4[l2] = 0;
		}
	}
	
	void fillsynth_harpsichordSIG0(int count, float* table) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec4[0] = (iRec4[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec4[0] + -1))));
			iRec4[1] = iRec4[0];
		}
	}

};

static synth_harpsichordSIG0* newsynth_harpsichordSIG0() { return (synth_harpsichordSIG0*)new synth_harpsichordSIG0(); }
static void deletesynth_harpsichordSIG0(synth_harpsichordSIG0* dsp) { delete dsp; }

static float ftbl0synth_harpsichordSIG0[65536];
static float synth_harpsichord_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_harpsichord
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_harpsichord : public synth_harpsichord_dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fButton0;
	float fRec2[2];
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fHslider1;
	float fRec3[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider2;
	float fRec6[2];
	float fRec5[2];
	float fRec12[2];
	float fRec11[2];
	float fRec10[2];
	float fRec9[2];
	float fRec8[2];
	float fRec7[2];
	float fRec18[2];
	float fRec17[2];
	float fRec16[2];
	float fRec15[2];
	float fRec14[2];
	float fRec13[2];
	int iRec20[2];
	float fConst2;
	FAUSTFLOAT fEntry1;
	float fRec21[2];
	float fRec19[2];
	int iRec22[2];
	int IOTA;
	float fVec0[8192];
	float fConst3;
	float fRec1[3];
	float fRec0[8192];
	FAUSTFLOAT fHslider3;
	float fRec23[2];
	FAUSTFLOAT fHslider4;
	float fRec24[2];
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	FAUSTFLOAT fHslider5;
	float fRec36[2];
	float fConst8;
	float fConst9;
	float fConst10;
	float fRec37[2];
	float fConst11;
	float fRec35[2];
	float fVec1[65536];
	float fConst12;
	int iConst13;
	float fConst14;
	FAUSTFLOAT fHslider6;
	float fRec38[2];
	float fVec2[4096];
	int iConst15;
	float fVec3[4096];
	int iConst16;
	float fRec33[2];
	float fConst17;
	float fConst18;
	float fConst19;
	float fRec42[2];
	float fConst20;
	float fRec41[2];
	float fVec4[65536];
	float fConst21;
	int iConst22;
	float fVec5[8192];
	int iConst23;
	float fRec39[2];
	float fConst24;
	float fConst25;
	float fConst26;
	float fRec46[2];
	float fConst27;
	float fRec45[2];
	float fVec6[32768];
	float fConst28;
	int iConst29;
	float fVec7[8192];
	int iConst30;
	float fRec43[2];
	float fConst31;
	float fConst32;
	float fConst33;
	float fRec50[2];
	float fConst34;
	float fRec49[2];
	float fVec8[65536];
	float fConst35;
	int iConst36;
	float fVec9[8192];
	int iConst37;
	float fRec47[2];
	float fConst38;
	float fConst39;
	float fConst40;
	float fRec54[2];
	float fConst41;
	float fRec53[2];
	float fVec10[32768];
	float fConst42;
	int iConst43;
	float fVec11[4096];
	float fVec12[4096];
	int iConst44;
	float fRec51[2];
	float fConst45;
	float fConst46;
	float fConst47;
	float fRec58[2];
	float fConst48;
	float fRec57[2];
	float fVec13[32768];
	float fConst49;
	int iConst50;
	float fVec14[4096];
	int iConst51;
	float fRec55[2];
	float fConst52;
	float fConst53;
	float fConst54;
	float fRec62[2];
	float fConst55;
	float fRec61[2];
	float fVec15[32768];
	float fConst56;
	int iConst57;
	float fVec16[8192];
	int iConst58;
	float fRec59[2];
	float fConst59;
	float fConst60;
	float fConst61;
	float fRec66[2];
	float fConst62;
	float fRec65[2];
	float fVec17[32768];
	float fConst63;
	int iConst64;
	float fVec18[8192];
	int iConst65;
	float fRec63[2];
	float fRec25[3];
	float fRec26[3];
	float fRec27[3];
	float fRec28[3];
	float fRec29[3];
	float fRec30[3];
	float fRec31[3];
	float fRec32[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("copyright", "Romain Michon");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("description", "Nonlinear WaveGuide Commuted Harpsichord");
		m->declare("filename", "synth_harpsichord.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/allpassnn:author", "Julius O. Smith III");
		m->declare("filters.lib/allpassnn:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpassnn:license", "MIT-style STK-4.3 license");
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
		m->declare("name", "synth.harpsichord");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
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
		synth_harpsichordSIG0* sig0 = newsynth_harpsichordSIG0();
		sig0->instanceInitsynth_harpsichordSIG0(sample_rate);
		sig0->fillsynth_harpsichordSIG0(65536, ftbl0synth_harpsichordSIG0);
		deletesynth_harpsichordSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (1.0f / fConst0);
		fConst2 = (7.0f / fConst0);
		fConst3 = (0.0022727272f * fConst0);
		fConst4 = std::cos((37699.1133f / fConst0));
		fConst5 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst6 = ((0.0f - (6.90775537f * fConst5)) / fConst0);
		fConst7 = (0.5f * fConst6);
		fConst8 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst9 = (1.0f / (fConst8 + 1.0f));
		fConst10 = (1.0f - fConst8);
		fConst11 = (0.333333343f * fConst6);
		fConst12 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst13 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst5 - fConst12))));
		fConst14 = (0.0011363636f * fConst0);
		iConst15 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		iConst16 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst12 + -1.0f))));
		fConst17 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst18 = ((0.0f - (6.90775537f * fConst17)) / fConst0);
		fConst19 = (0.5f * fConst18);
		fConst20 = (0.333333343f * fConst18);
		fConst21 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst22 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst17 - fConst21))));
		iConst23 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst21 + -1.0f))));
		fConst24 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst25 = ((0.0f - (6.90775537f * fConst24)) / fConst0);
		fConst26 = (0.5f * fConst25);
		fConst27 = (0.333333343f * fConst25);
		fConst28 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst29 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst24 - fConst28))));
		iConst30 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst28 + -1.0f))));
		fConst31 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst32 = ((0.0f - (6.90775537f * fConst31)) / fConst0);
		fConst33 = (0.5f * fConst32);
		fConst34 = (0.333333343f * fConst32);
		fConst35 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst36 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst31 - fConst35))));
		iConst37 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst35 + -1.0f))));
		fConst38 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst39 = ((0.0f - (6.90775537f * fConst38)) / fConst0);
		fConst40 = (0.5f * fConst39);
		fConst41 = (0.333333343f * fConst39);
		fConst42 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst43 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst38 - fConst42))));
		iConst44 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst42 + -1.0f))));
		fConst45 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst46 = ((0.0f - (6.90775537f * fConst45)) / fConst0);
		fConst47 = (0.5f * fConst46);
		fConst48 = (0.333333343f * fConst46);
		fConst49 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst50 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst45 - fConst49))));
		iConst51 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst49 + -1.0f))));
		fConst52 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst53 = ((0.0f - (6.90775537f * fConst52)) / fConst0);
		fConst54 = (0.5f * fConst53);
		fConst55 = (0.333333343f * fConst53);
		fConst56 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst57 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst52 - fConst56))));
		iConst58 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst56 + -1.0f))));
		fConst59 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst60 = ((0.0f - (6.90775537f * fConst59)) / fConst0);
		fConst61 = (0.5f * fConst60);
		fConst62 = (0.333333343f * fConst60);
		fConst63 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst64 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst59 - fConst63))));
		iConst65 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst63 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(48.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fEntry0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(220.0f);
		fEntry1 = FAUSTFLOAT(0.80000000000000004f);
		fHslider3 = FAUSTFLOAT(0.59999999999999998f);
		fHslider4 = FAUSTFLOAT(0.13700000000000001f);
		fHslider5 = FAUSTFLOAT(0.71999999999999997f);
		fHslider6 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec2[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec3[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec6[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec5[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec12[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec11[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec10[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec9[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec8[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec7[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec18[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec17[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec16[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec15[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec14[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec13[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			iRec20[l17] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec21[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec19[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			iRec22[l20] = 0;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 8192); l21 = (l21 + 1)) {
			fVec0[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec1[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 8192); l23 = (l23 + 1)) {
			fRec0[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec23[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec24[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec36[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec37[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec35[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 65536); l29 = (l29 + 1)) {
			fVec1[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec38[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 4096); l31 = (l31 + 1)) {
			fVec2[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 4096); l32 = (l32 + 1)) {
			fVec3[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec33[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec42[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec41[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 65536); l36 = (l36 + 1)) {
			fVec4[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 8192); l37 = (l37 + 1)) {
			fVec5[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec39[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec46[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec45[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 32768); l41 = (l41 + 1)) {
			fVec6[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 8192); l42 = (l42 + 1)) {
			fVec7[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec43[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec50[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec49[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 65536); l46 = (l46 + 1)) {
			fVec8[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 8192); l47 = (l47 + 1)) {
			fVec9[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec47[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec54[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec53[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 32768); l51 = (l51 + 1)) {
			fVec10[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 4096); l52 = (l52 + 1)) {
			fVec11[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 4096); l53 = (l53 + 1)) {
			fVec12[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec51[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec58[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec57[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 32768); l57 = (l57 + 1)) {
			fVec13[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 4096); l58 = (l58 + 1)) {
			fVec14[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec55[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec62[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec61[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 32768); l62 = (l62 + 1)) {
			fVec15[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 8192); l63 = (l63 + 1)) {
			fVec16[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec59[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec66[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec65[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 32768); l67 = (l67 + 1)) {
			fVec17[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 8192); l68 = (l68 + 1)) {
			fVec18[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec63[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec25[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec26[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec27[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec28[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec29[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec30[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 3); l76 = (l76 + 1)) {
			fRec31[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 3); l77 = (l77 + 1)) {
			fRec32[l77] = 0.0f;
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
	
	virtual synth_harpsichord* clone() {
		return new synth_harpsichord();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.harpsichord");
		ui_interface->declare(&fEntry1, "tooltip", "Gain (value between 0 and 1)");
		ui_interface->addNumEntry("gain", &fEntry1, 0.800000012f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addButton("gate", &fButton0);
		ui_interface->declare(&fHslider2, "tooltip", "Frequency of the sine wave for the modulation of theta (works if Modulation Type=3)");
		ui_interface->declare(&fHslider2, "unit", "Hz");
		ui_interface->addHorizontalSlider("modfreq", &fHslider2, 220.0f, 20.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fEntry0, "tooltip", "0=theta is modulated by the incoming signal; 1=theta is modulated by the averaged incoming signal; 2=theta is modulated by the squared incoming signal; 3=theta is modulated by a sine wave of frequency freqMod; 4=theta is modulated by a sine wave of frequency freq;");
		ui_interface->addNumEntry("modtype", &fEntry0, 0.0f, 0.0f, 4.0f, 1.0f);
		ui_interface->declare(&fHslider1, "tooltip", "Nonlinearity factor (value between 0 and 1)");
		ui_interface->addHorizontalSlider("nonlin", &fHslider1, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pan", &fHslider3, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider0, 48.0f, 24.0f, 96.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider4, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider5, 0.720000029f, 0.00999999978f, 2.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("width", &fHslider6, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = std::pow(2.0f, (0.0833333358f * (float(fHslider0) + -69.0f)));
		float fSlow1 = (440.0f * fSlow0);
		float fSlow2 = float(getValueLoopFilterb0(float(fSlow1)));
		int iSlow3 = int(float(fButton0));
		int iSlow4 = (iSlow3 < 1);
		float fSlow5 = (0.00100000005f * ((0.999599993f * float(iSlow3)) + (0.899999976f * (float(iSlow4) * float(getValueReleaseLoopGain(float(fSlow1)))))));
		float fSlow6 = float(fEntry0);
		float fSlow7 = float((fSlow6 >= 3.0f));
		float fSlow8 = (0.00100000005f * float(fHslider1));
		float fSlow9 = (440.0f * (fSlow0 * float((fSlow6 == 4.0f))));
		float fSlow10 = float((fSlow6 != 4.0f));
		float fSlow11 = (0.00100000005f * float(fHslider2));
		float fSlow12 = float((fSlow6 < 3.0f));
		float fSlow13 = (3.14159274f * float((fSlow6 == 0.0f)));
		float fSlow14 = (1.57079637f * float((fSlow6 == 1.0f)));
		float fSlow15 = (3.14159274f * float((fSlow6 == 2.0f)));
		int iSlow16 = (iSlow3 > 0);
		float fSlow17 = (fConst2 / float(getValueDryTapAmpT60(float(fSlow1))));
		float fSlow18 = (0.00100000005f * float(fEntry1));
		int iSlow19 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst3 / fSlow0))));
		float fSlow20 = float(getValueLoopFiltera1(float(fSlow1)));
		float fSlow21 = float(getValueLoopFiltera2(float(fSlow1)));
		float fSlow22 = float(getValueLoopFilterb1(float(fSlow1)));
		float fSlow23 = float(getValueLoopFilterb2(float(fSlow1)));
		float fSlow24 = (0.00100000005f * float(fHslider3));
		float fSlow25 = (0.00100000005f * float(fHslider4));
		float fSlow26 = (0.00100000005f * float(fHslider5));
		float fSlow27 = (fConst14 / fSlow0);
		float fSlow28 = (0.00100000005f * float(fHslider6));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec2[0] = (fSlow5 + (0.999000013f * fRec2[1]));
			float fTemp0 = fRec0[((IOTA - 1) & 8191)];
			fRec3[0] = (fSlow8 + (0.999000013f * fRec3[1]));
			fRec6[0] = (fSlow11 + (0.999000013f * fRec6[1]));
			float fTemp1 = (fRec5[1] + (fConst1 * (fSlow9 + (fSlow10 * fRec6[0]))));
			fRec5[0] = (fTemp1 - std::floor(fTemp1));
			float fTemp2 = (3.14159274f * (fRec3[0] * ftbl0synth_harpsichordSIG0[int((65536.0f * fRec5[0]))]));
			float fTemp3 = std::sin(fTemp2);
			float fTemp4 = std::cos(fTemp2);
			float fTemp5 = ((fTemp0 * fTemp4) - (fTemp3 * fRec7[1]));
			float fTemp6 = ((fTemp4 * fTemp5) - (fTemp3 * fRec8[1]));
			float fTemp7 = ((fTemp4 * fTemp6) - (fTemp3 * fRec9[1]));
			float fTemp8 = ((fTemp4 * fTemp7) - (fTemp3 * fRec10[1]));
			float fTemp9 = ((fTemp4 * fTemp8) - (fTemp3 * fRec11[1]));
			fRec12[0] = ((fTemp4 * fTemp9) - (fTemp3 * fRec12[1]));
			fRec11[0] = ((fTemp3 * fTemp9) + (fTemp4 * fRec12[1]));
			fRec10[0] = ((fTemp3 * fTemp8) + (fTemp4 * fRec11[1]));
			fRec9[0] = ((fTemp3 * fTemp7) + (fTemp4 * fRec10[1]));
			fRec8[0] = ((fTemp3 * fTemp6) + (fTemp4 * fRec9[1]));
			fRec7[0] = ((fTemp3 * fTemp5) + (fTemp4 * fRec8[1]));
			float fTemp10 = (fRec3[0] * (((fSlow13 * fTemp0) + (fSlow14 * (fTemp0 + fRec0[((IOTA - 2) & 8191)]))) + (fSlow15 * synth_harpsichord_faustpower2_f(fTemp0))));
			float fTemp11 = std::sin(fTemp10);
			float fTemp12 = std::cos(fTemp10);
			float fTemp13 = ((fTemp0 * fTemp12) - (fTemp11 * fRec13[1]));
			float fTemp14 = ((fTemp12 * fTemp13) - (fTemp11 * fRec14[1]));
			float fTemp15 = ((fTemp12 * fTemp14) - (fTemp11 * fRec15[1]));
			float fTemp16 = ((fTemp12 * fTemp15) - (fTemp11 * fRec16[1]));
			float fTemp17 = ((fTemp12 * fTemp16) - (fTemp11 * fRec17[1]));
			fRec18[0] = ((fTemp12 * fTemp17) - (fTemp11 * fRec18[1]));
			fRec17[0] = ((fTemp11 * fTemp17) + (fTemp12 * fRec18[1]));
			fRec16[0] = ((fTemp11 * fTemp16) + (fTemp12 * fRec17[1]));
			fRec15[0] = ((fTemp11 * fTemp15) + (fTemp12 * fRec16[1]));
			fRec14[0] = ((fTemp11 * fTemp14) + (fTemp12 * fRec15[1]));
			fRec13[0] = ((fTemp11 * fTemp13) + (fTemp12 * fRec14[1]));
			iRec20[0] = ((iSlow3 * iRec20[1]) + 1);
			float fTemp18 = float((iRec20[0] + -1));
			float fTemp19 = float(((fTemp18 < 2.0f) & iSlow16));
			fRec21[0] = (fSlow18 + (0.999000013f * fRec21[1]));
			float fTemp20 = ((0.0301973838f * fTemp19) + (float(((fTemp18 >= 2.0f) | iSlow4)) * std::exp((0.0f - (fSlow17 / fRec21[0])))));
			fRec19[0] = ((fRec19[1] * fTemp20) + (0.150000006f * (fTemp19 * (1.0f - fTemp20))));
			iRec22[0] = ((1103515245 * iRec22[1]) + 12345);
			fVec0[(IOTA & 8191)] = ((fRec2[0] * ((fSlow7 * ((fTemp0 * fTemp3) + (fRec7[1] * fTemp4))) + (fSlow12 * ((fRec3[0] * ((fTemp0 * fTemp11) + (fRec13[1] * fTemp12))) + ((1.0f - fRec3[0]) * fTemp0))))) + (4.65661287e-10f * (fRec19[0] * float(iRec22[0]))));
			fRec1[0] = (fVec0[((IOTA - iSlow19) & 8191)] - ((fSlow20 * fRec1[1]) + (fSlow21 * fRec1[2])));
			fRec0[(IOTA & 8191)] = (((fSlow2 * fRec1[0]) + (fSlow22 * fRec1[1])) + (fSlow23 * fRec1[2]));
			fRec23[0] = (fSlow24 + (0.999000013f * fRec23[1]));
			float fTemp21 = (fRec0[((IOTA - 0) & 8191)] * (1.0f - fRec23[0]));
			fRec24[0] = (fSlow25 + (0.999000013f * fRec24[1]));
			float fTemp22 = (1.0f - fRec24[0]);
			fRec36[0] = (fSlow26 + (0.999000013f * fRec36[1]));
			float fTemp23 = std::exp((fConst7 / fRec36[0]));
			float fTemp24 = synth_harpsichord_faustpower2_f(fTemp23);
			float fTemp25 = (1.0f - (fConst4 * fTemp24));
			float fTemp26 = (1.0f - fTemp24);
			float fTemp27 = (fTemp25 / fTemp26);
			float fTemp28 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fTemp25) / synth_harpsichord_faustpower2_f(fTemp26)) + -1.0f)));
			fRec37[0] = (0.0f - (fConst9 * ((fConst10 * fRec37[1]) - (fRec32[1] + fRec32[2]))));
			fRec35[0] = ((fRec35[1] * (fTemp27 - fTemp28)) + ((fTemp23 * (fRec32[1] + (fRec37[0] * ((std::exp((fConst11 / fRec36[0])) / fTemp23) + -1.0f)))) * (fTemp28 + (1.0f - fTemp27))));
			fVec1[(IOTA & 65535)] = ((0.353553385f * fRec35[0]) + 9.99999968e-21f);
			fRec38[0] = (fSlow28 + (0.999000013f * fRec38[1]));
			float fTemp29 = fRec0[((IOTA - int(std::min<float>(4096.0f, std::max<float>(0.0f, (fSlow27 * fRec38[0]))))) & 8191)];
			fVec2[(IOTA & 4095)] = ((fRec23[0] * fRec24[0]) * fTemp29);
			float fTemp30 = (0.300000012f * fVec2[((IOTA - iConst15) & 4095)]);
			float fTemp31 = (((0.600000024f * fRec33[1]) + fVec1[((IOTA - iConst13) & 65535)]) - fTemp30);
			fVec3[(IOTA & 4095)] = fTemp31;
			fRec33[0] = fVec3[((IOTA - iConst16) & 4095)];
			float fRec34 = (0.0f - (0.600000024f * fTemp31));
			float fTemp32 = std::exp((fConst19 / fRec36[0]));
			float fTemp33 = synth_harpsichord_faustpower2_f(fTemp32);
			float fTemp34 = (1.0f - (fConst4 * fTemp33));
			float fTemp35 = (1.0f - fTemp33);
			float fTemp36 = (fTemp34 / fTemp35);
			float fTemp37 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fTemp34) / synth_harpsichord_faustpower2_f(fTemp35)) + -1.0f)));
			fRec42[0] = (0.0f - (fConst9 * ((fConst10 * fRec42[1]) - (fRec28[1] + fRec28[2]))));
			fRec41[0] = ((fRec41[1] * (fTemp36 - fTemp37)) + ((fTemp32 * (fRec28[1] + (fRec42[0] * ((std::exp((fConst20 / fRec36[0])) / fTemp32) + -1.0f)))) * (fTemp37 + (1.0f - fTemp36))));
			fVec4[(IOTA & 65535)] = ((0.353553385f * fRec41[0]) + 9.99999968e-21f);
			float fTemp38 = (((0.600000024f * fRec39[1]) + fVec4[((IOTA - iConst22) & 65535)]) - fTemp30);
			fVec5[(IOTA & 8191)] = fTemp38;
			fRec39[0] = fVec5[((IOTA - iConst23) & 8191)];
			float fRec40 = (0.0f - (0.600000024f * fTemp38));
			float fTemp39 = std::exp((fConst26 / fRec36[0]));
			float fTemp40 = synth_harpsichord_faustpower2_f(fTemp39);
			float fTemp41 = (1.0f - (fConst4 * fTemp40));
			float fTemp42 = (1.0f - fTemp40);
			float fTemp43 = (fTemp41 / fTemp42);
			float fTemp44 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fTemp41) / synth_harpsichord_faustpower2_f(fTemp42)) + -1.0f)));
			fRec46[0] = (0.0f - (fConst9 * ((fConst10 * fRec46[1]) - (fRec30[1] + fRec30[2]))));
			fRec45[0] = ((fRec45[1] * (fTemp43 - fTemp44)) + ((fTemp39 * (fRec30[1] + (fRec46[0] * ((std::exp((fConst27 / fRec36[0])) / fTemp39) + -1.0f)))) * (fTemp44 + (1.0f - fTemp43))));
			fVec6[(IOTA & 32767)] = ((0.353553385f * fRec45[0]) + 9.99999968e-21f);
			float fTemp45 = (fVec6[((IOTA - iConst29) & 32767)] + (fTemp30 + (0.600000024f * fRec43[1])));
			fVec7[(IOTA & 8191)] = fTemp45;
			fRec43[0] = fVec7[((IOTA - iConst30) & 8191)];
			float fRec44 = (0.0f - (0.600000024f * fTemp45));
			float fTemp46 = std::exp((fConst33 / fRec36[0]));
			float fTemp47 = synth_harpsichord_faustpower2_f(fTemp46);
			float fTemp48 = (1.0f - (fConst4 * fTemp47));
			float fTemp49 = (1.0f - fTemp47);
			float fTemp50 = (fTemp48 / fTemp49);
			float fTemp51 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fTemp48) / synth_harpsichord_faustpower2_f(fTemp49)) + -1.0f)));
			fRec50[0] = (0.0f - (fConst9 * ((fConst10 * fRec50[1]) - (fRec26[1] + fRec26[2]))));
			fRec49[0] = ((fRec49[1] * (fTemp50 - fTemp51)) + ((fTemp46 * (fRec26[1] + (fRec50[0] * ((std::exp((fConst34 / fRec36[0])) / fTemp46) + -1.0f)))) * (fTemp51 + (1.0f - fTemp50))));
			fVec8[(IOTA & 65535)] = ((0.353553385f * fRec49[0]) + 9.99999968e-21f);
			float fTemp52 = (fTemp30 + ((0.600000024f * fRec47[1]) + fVec8[((IOTA - iConst36) & 65535)]));
			fVec9[(IOTA & 8191)] = fTemp52;
			fRec47[0] = fVec9[((IOTA - iConst37) & 8191)];
			float fRec48 = (0.0f - (0.600000024f * fTemp52));
			float fTemp53 = std::exp((fConst40 / fRec36[0]));
			float fTemp54 = synth_harpsichord_faustpower2_f(fTemp53);
			float fTemp55 = (1.0f - (fConst4 * fTemp54));
			float fTemp56 = (1.0f - fTemp54);
			float fTemp57 = (fTemp55 / fTemp56);
			float fTemp58 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fTemp55) / synth_harpsichord_faustpower2_f(fTemp56)) + -1.0f)));
			fRec54[0] = (0.0f - (fConst9 * ((fConst10 * fRec54[1]) - (fRec25[1] + fRec25[2]))));
			fRec53[0] = ((fRec53[1] * (fTemp57 - fTemp58)) + ((fTemp53 * (fRec25[1] + (fRec54[0] * ((std::exp((fConst41 / fRec36[0])) / fTemp53) + -1.0f)))) * (fTemp58 + (1.0f - fTemp57))));
			fVec10[(IOTA & 32767)] = ((0.353553385f * fRec53[0]) + 9.99999968e-21f);
			fVec11[(IOTA & 4095)] = (fTemp21 * fRec24[0]);
			float fTemp59 = (0.300000012f * fVec11[((IOTA - iConst15) & 4095)]);
			float fTemp60 = ((fVec10[((IOTA - iConst43) & 32767)] + fTemp59) - (0.600000024f * fRec51[1]));
			fVec12[(IOTA & 4095)] = fTemp60;
			fRec51[0] = fVec12[((IOTA - iConst44) & 4095)];
			float fRec52 = (0.600000024f * fTemp60);
			float fTemp61 = std::exp((fConst47 / fRec36[0]));
			float fTemp62 = synth_harpsichord_faustpower2_f(fTemp61);
			float fTemp63 = (1.0f - (fConst4 * fTemp62));
			float fTemp64 = (1.0f - fTemp62);
			float fTemp65 = (fTemp63 / fTemp64);
			float fTemp66 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fTemp63) / synth_harpsichord_faustpower2_f(fTemp64)) + -1.0f)));
			fRec58[0] = (0.0f - (fConst9 * ((fConst10 * fRec58[1]) - (fRec31[1] + fRec31[2]))));
			fRec57[0] = ((fRec57[1] * (fTemp65 - fTemp66)) + ((fTemp61 * (fRec31[1] + (fRec58[0] * ((std::exp((fConst48 / fRec36[0])) / fTemp61) + -1.0f)))) * (fTemp66 + (1.0f - fTemp65))));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec57[0]) + 9.99999968e-21f);
			float fTemp67 = (fVec13[((IOTA - iConst50) & 32767)] - (fTemp59 + (0.600000024f * fRec55[1])));
			fVec14[(IOTA & 4095)] = fTemp67;
			fRec55[0] = fVec14[((IOTA - iConst51) & 4095)];
			float fRec56 = (0.600000024f * fTemp67);
			float fTemp68 = std::exp((fConst54 / fRec36[0]));
			float fTemp69 = synth_harpsichord_faustpower2_f(fTemp68);
			float fTemp70 = (1.0f - (fConst4 * fTemp69));
			float fTemp71 = (1.0f - fTemp69);
			float fTemp72 = (fTemp70 / fTemp71);
			float fTemp73 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fTemp70) / synth_harpsichord_faustpower2_f(fTemp71)) + -1.0f)));
			fRec62[0] = (0.0f - (fConst9 * ((fConst10 * fRec62[1]) - (fRec27[1] + fRec27[2]))));
			fRec61[0] = ((fRec61[1] * (fTemp72 - fTemp73)) + ((fTemp68 * (fRec27[1] + (fRec62[0] * ((std::exp((fConst55 / fRec36[0])) / fTemp68) + -1.0f)))) * (fTemp73 + (1.0f - fTemp72))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec61[0]) + 9.99999968e-21f);
			float fTemp74 = (fVec15[((IOTA - iConst57) & 32767)] - (fTemp59 + (0.600000024f * fRec59[1])));
			fVec16[(IOTA & 8191)] = fTemp74;
			fRec59[0] = fVec16[((IOTA - iConst58) & 8191)];
			float fRec60 = (0.600000024f * fTemp74);
			float fTemp75 = std::exp((fConst61 / fRec36[0]));
			float fTemp76 = synth_harpsichord_faustpower2_f(fTemp75);
			float fTemp77 = (1.0f - (fConst4 * fTemp76));
			float fTemp78 = (1.0f - fTemp76);
			float fTemp79 = (fTemp77 / fTemp78);
			float fTemp80 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fTemp77) / synth_harpsichord_faustpower2_f(fTemp78)) + -1.0f)));
			fRec66[0] = (0.0f - (fConst9 * ((fConst10 * fRec66[1]) - (fRec29[1] + fRec29[2]))));
			fRec65[0] = ((fRec65[1] * (fTemp79 - fTemp80)) + ((fTemp75 * (fRec29[1] + (fRec66[0] * ((std::exp((fConst62 / fRec36[0])) / fTemp75) + -1.0f)))) * (fTemp80 + (1.0f - fTemp79))));
			fVec17[(IOTA & 32767)] = ((0.353553385f * fRec65[0]) + 9.99999968e-21f);
			float fTemp81 = ((fTemp59 + fVec17[((IOTA - iConst64) & 32767)]) - (0.600000024f * fRec63[1]));
			fVec18[(IOTA & 8191)] = fTemp81;
			fRec63[0] = fVec18[((IOTA - iConst65) & 8191)];
			float fRec64 = (0.600000024f * fTemp81);
			float fTemp82 = (fRec63[1] + fRec59[1]);
			float fTemp83 = (fRec56 + (fRec60 + (fRec64 + (fRec52 + (fRec55[1] + fTemp82)))));
			fRec25[0] = (fRec33[1] + (fRec39[1] + (fRec43[1] + (fRec47[1] + (fRec51[1] + (fRec34 + (fRec40 + (fRec44 + (fRec48 + fTemp83)))))))));
			fRec26[0] = ((fRec51[1] + fTemp83) - (fRec33[1] + (fRec39[1] + (fRec43[1] + (fRec47[1] + (fRec34 + (fRec40 + (fRec48 + fRec44))))))));
			float fTemp84 = (fRec64 + (fRec52 + fRec63[1]));
			float fTemp85 = (fRec56 + (fRec60 + (fRec59[1] + fRec55[1])));
			fRec27[0] = ((fRec43[1] + (fRec47[1] + (fRec51[1] + (fRec44 + (fRec48 + fTemp84))))) - (fRec33[1] + (fRec39[1] + (fRec34 + (fRec40 + fTemp85)))));
			fRec28[0] = ((fRec33[1] + (fRec39[1] + (fRec51[1] + (fRec34 + (fRec40 + fTemp84))))) - (fRec43[1] + (fRec47[1] + (fRec44 + (fRec48 + fTemp85)))));
			float fTemp86 = (fRec60 + (fRec52 + fRec59[1]));
			float fTemp87 = (fRec56 + (fRec64 + (fRec63[1] + fRec55[1])));
			fRec29[0] = ((fRec39[1] + (fRec47[1] + (fRec51[1] + (fRec40 + (fRec48 + fTemp86))))) - (fRec33[1] + (fRec43[1] + (fRec34 + (fRec44 + fTemp87)))));
			fRec30[0] = ((fRec33[1] + (fRec43[1] + (fRec51[1] + (fRec34 + (fRec44 + fTemp86))))) - (fRec39[1] + (fRec47[1] + (fRec40 + (fRec48 + fTemp87)))));
			float fTemp88 = (fRec56 + (fRec52 + fRec55[1]));
			float fTemp89 = (fRec60 + (fRec64 + fTemp82));
			fRec31[0] = ((fRec33[1] + (fRec47[1] + (fRec51[1] + (fRec34 + (fRec48 + fTemp88))))) - (fRec39[1] + (fRec43[1] + (fRec40 + (fRec44 + fTemp89)))));
			fRec32[0] = ((fRec39[1] + (fRec43[1] + (fRec51[1] + (fRec40 + (fRec44 + fTemp88))))) - (fRec33[1] + (fRec47[1] + (fRec34 + (fRec48 + fTemp89)))));
			output0[i] = FAUSTFLOAT(((fTemp21 * fTemp22) + (0.370000005f * (fRec26[0] + fRec27[0]))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec26[0] - fRec27[0])) + ((fRec23[0] * fTemp22) * fTemp29)));
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec6[1] = fRec6[0];
			fRec5[1] = fRec5[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			fRec7[1] = fRec7[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec15[1] = fRec15[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			iRec20[1] = iRec20[0];
			fRec21[1] = fRec21[0];
			fRec19[1] = fRec19[0];
			iRec22[1] = iRec22[0];
			IOTA = (IOTA + 1);
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec35[1] = fRec35[0];
			fRec38[1] = fRec38[0];
			fRec33[1] = fRec33[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec39[1] = fRec39[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec43[1] = fRec43[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec47[1] = fRec47[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec51[1] = fRec51[0];
			fRec58[1] = fRec58[0];
			fRec57[1] = fRec57[0];
			fRec55[1] = fRec55[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec59[1] = fRec59[0];
			fRec66[1] = fRec66[0];
			fRec65[1] = fRec65[0];
			fRec63[1] = fRec63[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _synth_harpsichord_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_harpsichord_UI<T>::name(sym(synth_harpsichord));

typedef _synth_harpsichord_UI<synth_harpsichord> synth_harpsichord_UI;

class faust_synth_harpsichord_tilde : public FaustExternal<synth_harpsichord, synth_harpsichord_UI> {
public:
    faust_synth_harpsichord_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
