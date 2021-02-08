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
	
	int iRec17[2];
	
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
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec17[l6] = 0;
		}
	}
	
	void fillsynth_harpsichordSIG0(int count, float* table) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec17[0] = (iRec17[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec17[0] + -1))));
			iRec17[1] = iRec17[0];
		}
	}

};

static synth_harpsichordSIG0* newsynth_harpsichordSIG0() { return (synth_harpsichordSIG0*)new synth_harpsichordSIG0(); }
static void deletesynth_harpsichordSIG0(synth_harpsichordSIG0* dsp) { delete dsp; }

static float synth_harpsichord_faustpower2_f(float value) {
	return (value * value);
}
static float ftbl0synth_harpsichordSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_harpsichord
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_harpsichord : public synth_harpsichord_dsp {
	
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
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fCheckbox0;
	float fRec15[2];
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fHslider4;
	float fRec16[2];
	float fConst11;
	FAUSTFLOAT fHslider5;
	float fRec19[2];
	float fRec18[2];
	float fRec25[2];
	float fRec24[2];
	float fRec23[2];
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fRec31[2];
	float fRec30[2];
	float fRec29[2];
	float fRec28[2];
	float fRec27[2];
	float fRec26[2];
	int iRec33[2];
	float fConst12;
	float fRec34[2];
	float fRec32[2];
	int iRec35[2];
	float fVec1[8192];
	float fConst13;
	float fRec14[3];
	float fRec13[4096];
	float fConst14;
	FAUSTFLOAT fHslider6;
	float fVec2[4096];
	int iConst15;
	float fVec3[2048];
	int iConst16;
	float fRec8[2];
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fRec39[2];
	float fRec38[2];
	float fVec4[32768];
	float fConst21;
	int iConst22;
	float fVec5[4096];
	int iConst23;
	float fRec36[2];
	float fConst24;
	float fConst25;
	float fConst26;
	float fConst27;
	float fRec43[2];
	float fRec42[2];
	float fVec6[16384];
	float fConst28;
	int iConst29;
	float fVec7[4096];
	int iConst30;
	float fRec40[2];
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fRec47[2];
	float fRec46[2];
	float fVec8[16384];
	float fConst35;
	int iConst36;
	float fVec9[4096];
	float fVec10[4096];
	int iConst37;
	float fRec44[2];
	float fConst38;
	float fConst39;
	float fConst40;
	float fConst41;
	float fRec51[2];
	float fRec50[2];
	float fVec11[16384];
	float fConst42;
	int iConst43;
	float fVec12[4096];
	int iConst44;
	float fRec48[2];
	float fConst45;
	float fConst46;
	float fConst47;
	float fConst48;
	float fRec55[2];
	float fRec54[2];
	float fVec13[32768];
	float fConst49;
	int iConst50;
	float fVec14[4096];
	int iConst51;
	float fRec52[2];
	float fConst52;
	float fConst53;
	float fConst54;
	float fConst55;
	float fRec59[2];
	float fRec58[2];
	float fVec15[16384];
	float fConst56;
	int iConst57;
	float fVec16[2048];
	int iConst58;
	float fRec56[2];
	float fConst59;
	float fConst60;
	float fConst61;
	float fConst62;
	float fRec63[2];
	float fRec62[2];
	float fVec17[16384];
	float fConst63;
	int iConst64;
	float fVec18[2048];
	int iConst65;
	float fRec60[2];
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
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
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
		fConst11 = (1.0f / fConst0);
		fConst12 = (7.0f / fConst0);
		fConst13 = (0.0022727272f * fConst0);
		fConst14 = (0.0011363636f * fConst0);
		iConst15 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		iConst16 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst9 + -1.0f))));
		fConst17 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst18 = ((0.0f - (6.90775537f * fConst17)) / fConst0);
		fConst19 = (0.25f * fConst18);
		fConst20 = (0.166666672f * fConst18);
		fConst21 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst22 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst17 - fConst21))));
		iConst23 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst21 + -1.0f))));
		fConst24 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst25 = ((0.0f - (6.90775537f * fConst24)) / fConst0);
		fConst26 = (0.25f * fConst25);
		fConst27 = (0.166666672f * fConst25);
		fConst28 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst29 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst24 - fConst28))));
		iConst30 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst28 + -1.0f))));
		fConst31 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst32 = ((0.0f - (6.90775537f * fConst31)) / fConst0);
		fConst33 = (0.25f * fConst32);
		fConst34 = (0.166666672f * fConst32);
		fConst35 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst36 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst31 - fConst35))));
		iConst37 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst35 + -1.0f))));
		fConst38 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst39 = ((0.0f - (6.90775537f * fConst38)) / fConst0);
		fConst40 = (0.25f * fConst39);
		fConst41 = (0.166666672f * fConst39);
		fConst42 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst43 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst38 - fConst42))));
		iConst44 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst42 + -1.0f))));
		fConst45 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst46 = ((0.0f - (6.90775537f * fConst45)) / fConst0);
		fConst47 = (0.25f * fConst46);
		fConst48 = (0.166666672f * fConst46);
		fConst49 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst50 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst45 - fConst49))));
		iConst51 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst49 + -1.0f))));
		fConst52 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst53 = ((0.0f - (6.90775537f * fConst52)) / fConst0);
		fConst54 = (0.25f * fConst53);
		fConst55 = (0.166666672f * fConst53);
		fConst56 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst57 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst52 - fConst56))));
		iConst58 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst56 + -1.0f))));
		fConst59 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst60 = ((0.0f - (6.90775537f * fConst59)) / fConst0);
		fConst61 = (0.25f * fConst60);
		fConst62 = (0.166666672f * fConst60);
		fConst63 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst64 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst59 - fConst63))));
		iConst65 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst63 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.35999999999999999f);
		fHslider1 = FAUSTFLOAT(0.59999999999999998f);
		fHslider2 = FAUSTFLOAT(0.13700000000000001f);
		fHslider3 = FAUSTFLOAT(48.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fEntry0 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(220.0f);
		fHslider6 = FAUSTFLOAT(0.5f);
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
			fRec15[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec16[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec19[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec18[l8] = 0.0f;
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
			fRec20[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec31[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec30[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec29[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec28[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec27[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec26[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			iRec33[l21] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec34[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec32[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			iRec35[l24] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 8192); l25 = (l25 + 1)) {
			fVec1[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec14[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 4096); l27 = (l27 + 1)) {
			fRec13[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 4096); l28 = (l28 + 1)) {
			fVec2[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2048); l29 = (l29 + 1)) {
			fVec3[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec8[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec39[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec38[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 32768); l33 = (l33 + 1)) {
			fVec4[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 4096); l34 = (l34 + 1)) {
			fVec5[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec36[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec43[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec42[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 16384); l38 = (l38 + 1)) {
			fVec6[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 4096); l39 = (l39 + 1)) {
			fVec7[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec40[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec47[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec46[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 16384); l43 = (l43 + 1)) {
			fVec8[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 4096); l44 = (l44 + 1)) {
			fVec9[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 4096); l45 = (l45 + 1)) {
			fVec10[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec44[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec51[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec50[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 16384); l49 = (l49 + 1)) {
			fVec11[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 4096); l50 = (l50 + 1)) {
			fVec12[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec48[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec55[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec54[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 32768); l54 = (l54 + 1)) {
			fVec13[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 4096); l55 = (l55 + 1)) {
			fVec14[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec52[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec59[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec58[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 16384); l59 = (l59 + 1)) {
			fVec15[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2048); l60 = (l60 + 1)) {
			fVec16[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec56[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec63[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec62[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 16384); l64 = (l64 + 1)) {
			fVec17[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2048); l65 = (l65 + 1)) {
			fVec18[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec60[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 3); l67 = (l67 + 1)) {
			fRec0[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 3); l68 = (l68 + 1)) {
			fRec1[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 3); l69 = (l69 + 1)) {
			fRec2[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec3[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec4[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec5[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec6[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec7[l74] = 0.0f;
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
		ui_interface->declare(&fCheckbox0, "type", "float");
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->declare(&fHslider5, "tooltip", "Frequency of the sine wave for the modulation of theta (works if Modulation Type=3)");
		ui_interface->declare(&fHslider5, "unit", "Hz");
		ui_interface->addHorizontalSlider("modfreq", &fHslider5, 220.0f, 20.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fEntry0, "tooltip", "0=theta is modulated by the incoming signal; 1=theta is modulated by the averaged incoming signal; 2=theta is modulated by the squared incoming signal; 3=theta is modulated by a sine wave of frequency freqMod; 4=theta is modulated by a sine wave of frequency freq;");
		ui_interface->addNumEntry("modtype", &fEntry0, 0.0f, 0.0f, 4.0f, 1.0f);
		ui_interface->declare(&fHslider4, "tooltip", "Nonlinearity factor (value between 0 and 1)");
		ui_interface->addHorizontalSlider("nonlin", &fHslider4, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pan", &fHslider1, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider3, 48.0f, 24.0f, 96.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider0, 0.360000014f, 0.00499999989f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("width", &fHslider6, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::exp((fConst4 / fSlow0));
		float fSlow2 = synth_harpsichord_faustpower2_f(fSlow1);
		float fSlow3 = (1.0f - (fConst1 * fSlow2));
		float fSlow4 = (1.0f - fSlow2);
		float fSlow5 = (fSlow3 / fSlow4);
		float fSlow6 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow3) / synth_harpsichord_faustpower2_f(fSlow4)) + -1.0f)));
		float fSlow7 = (fSlow5 - fSlow6);
		float fSlow8 = (fSlow1 * (fSlow6 + (1.0f - fSlow5)));
		float fSlow9 = ((std::exp((fConst5 / fSlow0)) / fSlow1) + -1.0f);
		float fSlow10 = float(fHslider1);
		float fSlow11 = (0.00100000005f * float(fHslider2));
		float fSlow12 = std::pow(2.0f, (0.0833333358f * (float(fHslider3) + -69.0f)));
		float fSlow13 = (440.0f * fSlow12);
		float fSlow14 = float(getValueLoopFilterb0(float(fSlow13)));
		float fSlow15 = float(fCheckbox0);
		int iSlow16 = (fSlow15 > 0.0f);
		int iSlow17 = (iSlow16 < 1);
		float fSlow18 = (0.00100000005f * ((0.999599993f * float(iSlow16)) + (0.899999976f * (float(iSlow17) * float(getValueReleaseLoopGain(float(fSlow13)))))));
		int iSlow19 = int(float(fEntry0));
		float fSlow20 = float((iSlow19 >= 3));
		float fSlow21 = (0.00100000005f * float(fHslider4));
		float fSlow22 = (440.0f * (fSlow12 * float((iSlow19 == 4))));
		float fSlow23 = float((iSlow19 != 4));
		float fSlow24 = (0.00100000005f * float(fHslider5));
		float fSlow25 = float((iSlow19 < 3));
		float fSlow26 = (3.14159274f * float((iSlow19 == 0)));
		float fSlow27 = (1.57079637f * float((iSlow19 == 1)));
		float fSlow28 = (3.14159274f * float((iSlow19 == 2)));
		int iSlow29 = (iSlow16 > 0);
		float fSlow30 = (fConst12 / float(getValueDryTapAmpT60(float(fSlow13))));
		float fSlow31 = (0.00100000005f * std::min<float>(1.0f, std::max<float>(0.0f, fSlow15)));
		int iSlow32 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst13 / fSlow12))));
		float fSlow33 = float(getValueLoopFiltera1(float(fSlow13)));
		float fSlow34 = float(getValueLoopFiltera2(float(fSlow13)));
		float fSlow35 = float(getValueLoopFilterb1(float(fSlow13)));
		float fSlow36 = float(getValueLoopFilterb2(float(fSlow13)));
		int iSlow37 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst14 * (float(fHslider6) / fSlow12)))));
		float fSlow38 = std::exp((fConst19 / fSlow0));
		float fSlow39 = synth_harpsichord_faustpower2_f(fSlow38);
		float fSlow40 = (1.0f - (fConst1 * fSlow39));
		float fSlow41 = (1.0f - fSlow39);
		float fSlow42 = (fSlow40 / fSlow41);
		float fSlow43 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow40) / synth_harpsichord_faustpower2_f(fSlow41)) + -1.0f)));
		float fSlow44 = (fSlow42 - fSlow43);
		float fSlow45 = (fSlow38 * (fSlow43 + (1.0f - fSlow42)));
		float fSlow46 = ((std::exp((fConst20 / fSlow0)) / fSlow38) + -1.0f);
		float fSlow47 = std::exp((fConst26 / fSlow0));
		float fSlow48 = synth_harpsichord_faustpower2_f(fSlow47);
		float fSlow49 = (1.0f - (fConst1 * fSlow48));
		float fSlow50 = (1.0f - fSlow48);
		float fSlow51 = (fSlow49 / fSlow50);
		float fSlow52 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow49) / synth_harpsichord_faustpower2_f(fSlow50)) + -1.0f)));
		float fSlow53 = (fSlow51 - fSlow52);
		float fSlow54 = (fSlow47 * (fSlow52 + (1.0f - fSlow51)));
		float fSlow55 = ((std::exp((fConst27 / fSlow0)) / fSlow47) + -1.0f);
		float fSlow56 = std::exp((fConst33 / fSlow0));
		float fSlow57 = synth_harpsichord_faustpower2_f(fSlow56);
		float fSlow58 = (1.0f - (fConst1 * fSlow57));
		float fSlow59 = (1.0f - fSlow57);
		float fSlow60 = (fSlow58 / fSlow59);
		float fSlow61 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow58) / synth_harpsichord_faustpower2_f(fSlow59)) + -1.0f)));
		float fSlow62 = (fSlow60 - fSlow61);
		float fSlow63 = (fSlow56 * (fSlow61 + (1.0f - fSlow60)));
		float fSlow64 = ((std::exp((fConst34 / fSlow0)) / fSlow56) + -1.0f);
		float fSlow65 = (1.0f - fSlow10);
		float fSlow66 = std::exp((fConst40 / fSlow0));
		float fSlow67 = synth_harpsichord_faustpower2_f(fSlow66);
		float fSlow68 = (1.0f - (fConst1 * fSlow67));
		float fSlow69 = (1.0f - fSlow67);
		float fSlow70 = (fSlow68 / fSlow69);
		float fSlow71 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow68) / synth_harpsichord_faustpower2_f(fSlow69)) + -1.0f)));
		float fSlow72 = (fSlow70 - fSlow71);
		float fSlow73 = (fSlow66 * (fSlow71 + (1.0f - fSlow70)));
		float fSlow74 = ((std::exp((fConst41 / fSlow0)) / fSlow66) + -1.0f);
		float fSlow75 = std::exp((fConst47 / fSlow0));
		float fSlow76 = synth_harpsichord_faustpower2_f(fSlow75);
		float fSlow77 = (1.0f - (fConst1 * fSlow76));
		float fSlow78 = (1.0f - fSlow76);
		float fSlow79 = (fSlow77 / fSlow78);
		float fSlow80 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow77) / synth_harpsichord_faustpower2_f(fSlow78)) + -1.0f)));
		float fSlow81 = (fSlow79 - fSlow80);
		float fSlow82 = (fSlow75 * (fSlow80 + (1.0f - fSlow79)));
		float fSlow83 = ((std::exp((fConst48 / fSlow0)) / fSlow75) + -1.0f);
		float fSlow84 = std::exp((fConst54 / fSlow0));
		float fSlow85 = synth_harpsichord_faustpower2_f(fSlow84);
		float fSlow86 = (1.0f - (fConst1 * fSlow85));
		float fSlow87 = (1.0f - fSlow85);
		float fSlow88 = (fSlow86 / fSlow87);
		float fSlow89 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow86) / synth_harpsichord_faustpower2_f(fSlow87)) + -1.0f)));
		float fSlow90 = (fSlow88 - fSlow89);
		float fSlow91 = (fSlow84 * (fSlow89 + (1.0f - fSlow88)));
		float fSlow92 = ((std::exp((fConst55 / fSlow0)) / fSlow84) + -1.0f);
		float fSlow93 = std::exp((fConst61 / fSlow0));
		float fSlow94 = synth_harpsichord_faustpower2_f(fSlow93);
		float fSlow95 = (1.0f - (fConst1 * fSlow94));
		float fSlow96 = (1.0f - fSlow94);
		float fSlow97 = (fSlow95 / fSlow96);
		float fSlow98 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow95) / synth_harpsichord_faustpower2_f(fSlow96)) + -1.0f)));
		float fSlow99 = (fSlow97 - fSlow98);
		float fSlow100 = (fSlow93 * (fSlow98 + (1.0f - fSlow97)));
		float fSlow101 = ((std::exp((fConst62 / fSlow0)) / fSlow93) + -1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec11[0] = (0.0f - (fConst7 * ((fConst8 * fRec11[1]) - (fRec7[1] + fRec7[2]))));
			fRec10[0] = ((fSlow7 * fRec10[1]) + (fSlow8 * (fRec7[1] + (fSlow9 * fRec11[0]))));
			fVec0[(IOTA & 32767)] = ((0.353553385f * fRec10[0]) + 9.99999968e-21f);
			fRec12[0] = (fSlow11 + (0.999000013f * fRec12[1]));
			fRec15[0] = (fSlow18 + (0.999000013f * fRec15[1]));
			float fTemp0 = fRec13[((IOTA - 1) & 4095)];
			fRec16[0] = (fSlow21 + (0.999000013f * fRec16[1]));
			fRec19[0] = (fSlow24 + (0.999000013f * fRec19[1]));
			float fTemp1 = (fRec18[1] + (fConst11 * (fSlow22 + (fSlow23 * fRec19[0]))));
			fRec18[0] = (fTemp1 - std::floor(fTemp1));
			float fTemp2 = (3.14159274f * (fRec16[0] * ftbl0synth_harpsichordSIG0[int((65536.0f * fRec18[0]))]));
			float fTemp3 = std::sin(fTemp2);
			float fTemp4 = std::cos(fTemp2);
			float fTemp5 = ((fTemp0 * fTemp4) - (fTemp3 * fRec20[1]));
			float fTemp6 = ((fTemp4 * fTemp5) - (fTemp3 * fRec21[1]));
			float fTemp7 = ((fTemp4 * fTemp6) - (fTemp3 * fRec22[1]));
			float fTemp8 = ((fTemp4 * fTemp7) - (fTemp3 * fRec23[1]));
			float fTemp9 = ((fTemp4 * fTemp8) - (fTemp3 * fRec24[1]));
			fRec25[0] = ((fTemp4 * fTemp9) - (fTemp3 * fRec25[1]));
			fRec24[0] = ((fTemp3 * fTemp9) + (fTemp4 * fRec25[1]));
			fRec23[0] = ((fTemp3 * fTemp8) + (fTemp4 * fRec24[1]));
			fRec22[0] = ((fTemp3 * fTemp7) + (fTemp4 * fRec23[1]));
			fRec21[0] = ((fTemp3 * fTemp6) + (fTemp4 * fRec22[1]));
			fRec20[0] = ((fTemp3 * fTemp5) + (fTemp4 * fRec21[1]));
			float fTemp10 = (fRec16[0] * (((fSlow26 * fTemp0) + (fSlow27 * (fTemp0 + fRec13[((IOTA - 2) & 4095)]))) + (fSlow28 * synth_harpsichord_faustpower2_f(fTemp0))));
			float fTemp11 = std::sin(fTemp10);
			float fTemp12 = std::cos(fTemp10);
			float fTemp13 = ((fTemp0 * fTemp12) - (fTemp11 * fRec26[1]));
			float fTemp14 = ((fTemp12 * fTemp13) - (fTemp11 * fRec27[1]));
			float fTemp15 = ((fTemp12 * fTemp14) - (fTemp11 * fRec28[1]));
			float fTemp16 = ((fTemp12 * fTemp15) - (fTemp11 * fRec29[1]));
			float fTemp17 = ((fTemp12 * fTemp16) - (fTemp11 * fRec30[1]));
			fRec31[0] = ((fTemp12 * fTemp17) - (fTemp11 * fRec31[1]));
			fRec30[0] = ((fTemp11 * fTemp17) + (fTemp12 * fRec31[1]));
			fRec29[0] = ((fTemp11 * fTemp16) + (fTemp12 * fRec30[1]));
			fRec28[0] = ((fTemp11 * fTemp15) + (fTemp12 * fRec29[1]));
			fRec27[0] = ((fTemp11 * fTemp14) + (fTemp12 * fRec28[1]));
			fRec26[0] = ((fTemp11 * fTemp13) + (fTemp12 * fRec27[1]));
			iRec33[0] = ((iSlow16 * iRec33[1]) + 1);
			float fTemp18 = float((iRec33[0] + -1));
			float fTemp19 = float(((fTemp18 < 2.0f) & iSlow29));
			fRec34[0] = (fSlow31 + (0.999000013f * fRec34[1]));
			float fTemp20 = ((0.0301973838f * fTemp19) + (float(((fTemp18 >= 2.0f) | iSlow17)) * std::exp((0.0f - (fSlow30 / fRec34[0])))));
			fRec32[0] = ((fRec32[1] * fTemp20) + (0.150000006f * (fTemp19 * (1.0f - fTemp20))));
			iRec35[0] = ((1103515245 * iRec35[1]) + 12345);
			fVec1[(IOTA & 8191)] = ((fRec15[0] * ((fSlow20 * ((fTemp0 * fTemp3) + (fRec20[1] * fTemp4))) + (fSlow25 * ((fRec16[0] * ((fTemp0 * fTemp11) + (fRec26[1] * fTemp12))) + ((1.0f - fRec16[0]) * fTemp0))))) + (4.65661287e-10f * (fRec32[0] * float(iRec35[0]))));
			fRec14[0] = (fVec1[((IOTA - iSlow32) & 8191)] - ((fSlow33 * fRec14[1]) + (fSlow34 * fRec14[2])));
			fRec13[(IOTA & 4095)] = (((fSlow14 * fRec14[0]) + (fSlow35 * fRec14[1])) + (fSlow36 * fRec14[2]));
			float fTemp21 = fRec13[((IOTA - iSlow37) & 4095)];
			fVec2[(IOTA & 4095)] = (fSlow10 * (fRec12[0] * fTemp21));
			float fTemp22 = (0.300000012f * fVec2[((IOTA - iConst15) & 4095)]);
			float fTemp23 = (((0.600000024f * fRec8[1]) + fVec0[((IOTA - iConst10) & 32767)]) - fTemp22);
			fVec3[(IOTA & 2047)] = fTemp23;
			fRec8[0] = fVec3[((IOTA - iConst16) & 2047)];
			float fRec9 = (0.0f - (0.600000024f * fTemp23));
			fRec39[0] = (0.0f - (fConst7 * ((fConst8 * fRec39[1]) - (fRec3[1] + fRec3[2]))));
			fRec38[0] = ((fSlow44 * fRec38[1]) + (fSlow45 * (fRec3[1] + (fSlow46 * fRec39[0]))));
			fVec4[(IOTA & 32767)] = ((0.353553385f * fRec38[0]) + 9.99999968e-21f);
			float fTemp24 = (((0.600000024f * fRec36[1]) + fVec4[((IOTA - iConst22) & 32767)]) - fTemp22);
			fVec5[(IOTA & 4095)] = fTemp24;
			fRec36[0] = fVec5[((IOTA - iConst23) & 4095)];
			float fRec37 = (0.0f - (0.600000024f * fTemp24));
			fRec43[0] = (0.0f - (fConst7 * ((fConst8 * fRec43[1]) - (fRec5[1] + fRec5[2]))));
			fRec42[0] = ((fSlow53 * fRec42[1]) + (fSlow54 * (fRec5[1] + (fSlow55 * fRec43[0]))));
			fVec6[(IOTA & 16383)] = ((0.353553385f * fRec42[0]) + 9.99999968e-21f);
			float fTemp25 = (fVec6[((IOTA - iConst29) & 16383)] + (fTemp22 + (0.600000024f * fRec40[1])));
			fVec7[(IOTA & 4095)] = fTemp25;
			fRec40[0] = fVec7[((IOTA - iConst30) & 4095)];
			float fRec41 = (0.0f - (0.600000024f * fTemp25));
			fRec47[0] = (0.0f - (fConst7 * ((fConst8 * fRec47[1]) - (fRec2[1] + fRec2[2]))));
			fRec46[0] = ((fSlow62 * fRec46[1]) + (fSlow63 * (fRec2[1] + (fSlow64 * fRec47[0]))));
			fVec8[(IOTA & 16383)] = ((0.353553385f * fRec46[0]) + 9.99999968e-21f);
			float fTemp26 = fRec13[((IOTA - 0) & 4095)];
			fVec9[(IOTA & 4095)] = (fSlow65 * (fTemp26 * fRec12[0]));
			float fTemp27 = (0.300000012f * fVec9[((IOTA - iConst15) & 4095)]);
			float fTemp28 = (fVec8[((IOTA - iConst36) & 16383)] - (fTemp27 + (0.600000024f * fRec44[1])));
			fVec10[(IOTA & 4095)] = fTemp28;
			fRec44[0] = fVec10[((IOTA - iConst37) & 4095)];
			float fRec45 = (0.600000024f * fTemp28);
			fRec51[0] = (0.0f - (fConst7 * ((fConst8 * fRec51[1]) - (fRec4[1] + fRec4[2]))));
			fRec50[0] = ((fSlow72 * fRec50[1]) + (fSlow73 * (fRec4[1] + (fSlow74 * fRec51[0]))));
			fVec11[(IOTA & 16383)] = ((0.353553385f * fRec50[0]) + 9.99999968e-21f);
			float fTemp29 = ((fTemp27 + fVec11[((IOTA - iConst43) & 16383)]) - (0.600000024f * fRec48[1]));
			fVec12[(IOTA & 4095)] = fTemp29;
			fRec48[0] = fVec12[((IOTA - iConst44) & 4095)];
			float fRec49 = (0.600000024f * fTemp29);
			fRec55[0] = (0.0f - (fConst7 * ((fConst8 * fRec55[1]) - (fRec1[1] + fRec1[2]))));
			fRec54[0] = ((fSlow81 * fRec54[1]) + (fSlow82 * (fRec1[1] + (fSlow83 * fRec55[0]))));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec54[0]) + 9.99999968e-21f);
			float fTemp30 = (fTemp22 + ((0.600000024f * fRec52[1]) + fVec13[((IOTA - iConst50) & 32767)]));
			fVec14[(IOTA & 4095)] = fTemp30;
			fRec52[0] = fVec14[((IOTA - iConst51) & 4095)];
			float fRec53 = (0.0f - (0.600000024f * fTemp30));
			fRec59[0] = (0.0f - (fConst7 * ((fConst8 * fRec59[1]) - (fRec6[1] + fRec6[2]))));
			fRec58[0] = ((fSlow90 * fRec58[1]) + (fSlow91 * (fRec6[1] + (fSlow92 * fRec59[0]))));
			fVec15[(IOTA & 16383)] = ((0.353553385f * fRec58[0]) + 9.99999968e-21f);
			float fTemp31 = (fVec15[((IOTA - iConst57) & 16383)] - (fTemp27 + (0.600000024f * fRec56[1])));
			fVec16[(IOTA & 2047)] = fTemp31;
			fRec56[0] = fVec16[((IOTA - iConst58) & 2047)];
			float fRec57 = (0.600000024f * fTemp31);
			fRec63[0] = (0.0f - (fConst7 * ((fConst8 * fRec63[1]) - (fRec0[1] + fRec0[2]))));
			fRec62[0] = ((fSlow99 * fRec62[1]) + (fSlow100 * (fRec0[1] + (fSlow101 * fRec63[0]))));
			fVec17[(IOTA & 16383)] = ((0.353553385f * fRec62[0]) + 9.99999968e-21f);
			float fTemp32 = ((fVec17[((IOTA - iConst64) & 16383)] + fTemp27) - (0.600000024f * fRec60[1]));
			fVec18[(IOTA & 2047)] = fTemp32;
			fRec60[0] = fVec18[((IOTA - iConst65) & 2047)];
			float fRec61 = (0.600000024f * fTemp32);
			float fTemp33 = (fRec57 + fRec60[1]);
			float fTemp34 = (fRec53 + fTemp33);
			fRec0[0] = (fRec8[1] + (fRec36[1] + (fRec40[1] + (fRec44[1] + (fRec48[1] + (fRec52[1] + (fRec56[1] + (fRec45 + (fRec49 + (fRec61 + (fRec9 + (fRec37 + (fRec41 + fTemp34)))))))))))));
			fRec1[0] = ((fRec44[1] + (fRec48[1] + (fRec56[1] + (fRec45 + (fRec49 + (fRec61 + fTemp33)))))) - (fRec8[1] + (fRec36[1] + (fRec40[1] + (fRec52[1] + (fRec9 + (fRec37 + (fRec53 + fRec41))))))));
			float fTemp35 = (fRec53 + fRec60[1]);
			fRec2[0] = ((fRec40[1] + (fRec48[1] + (fRec52[1] + (fRec49 + (fRec61 + (fRec41 + fTemp35)))))) - (fRec8[1] + (fRec36[1] + (fRec44[1] + (fRec56[1] + (fRec45 + (fRec9 + (fRec57 + fRec37))))))));
			float fTemp36 = (fRec57 + fRec53);
			fRec3[0] = ((fRec8[1] + (fRec36[1] + (fRec48[1] + (fRec49 + (fRec61 + (fRec9 + (fRec37 + fRec60[1]))))))) - (fRec40[1] + (fRec44[1] + (fRec52[1] + (fRec56[1] + (fRec45 + (fRec41 + fTemp36)))))));
			fRec4[0] = ((fRec36[1] + (fRec44[1] + (fRec52[1] + (fRec45 + (fRec61 + (fRec37 + fTemp35)))))) - (fRec8[1] + (fRec40[1] + (fRec48[1] + (fRec56[1] + (fRec49 + (fRec9 + (fRec57 + fRec41))))))));
			fRec5[0] = ((fRec8[1] + (fRec40[1] + (fRec44[1] + (fRec45 + (fRec61 + (fRec9 + (fRec41 + fRec60[1]))))))) - (fRec36[1] + (fRec48[1] + (fRec52[1] + (fRec56[1] + (fRec49 + (fRec37 + fTemp36)))))));
			fRec6[0] = ((fRec8[1] + (fRec52[1] + (fRec56[1] + (fRec61 + (fRec9 + fTemp34))))) - (fRec36[1] + (fRec40[1] + (fRec44[1] + (fRec48[1] + (fRec45 + (fRec49 + (fRec41 + fRec37))))))));
			fRec7[0] = ((fRec36[1] + (fRec40[1] + (fRec56[1] + (fRec61 + (fRec37 + (fRec41 + fTemp33)))))) - (fRec8[1] + (fRec44[1] + (fRec48[1] + (fRec52[1] + (fRec45 + (fRec49 + (fRec53 + fRec9))))))));
			float fTemp37 = (1.0f - fRec12[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow65 * (fTemp26 * fTemp37))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow10 * (fTemp37 * fTemp21))));
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA = (IOTA + 1);
			fRec12[1] = fRec12[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			iRec33[1] = iRec33[0];
			fRec34[1] = fRec34[0];
			fRec32[1] = fRec32[0];
			iRec35[1] = iRec35[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec8[1] = fRec8[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec36[1] = fRec36[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec40[1] = fRec40[0];
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
