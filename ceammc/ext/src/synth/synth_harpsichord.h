/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.harpsichord"
version: "1.0"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
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

/**
 The base class of Meta handler to be used in synth_harpsichord_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
#include <cstdint>
#include <math.h>

class synth_harpsichordSIG0 {
	
  private:
	
	int iRec15[2];
	
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
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec15[l3] = 0;
		}
	}
	
	void fillsynth_harpsichordSIG0(int count, float* table) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec15[0] = (iRec15[1] + 1);
			table[i] = std::sin((9.58738019e-05f * float((iRec15[0] + -1))));
			iRec15[1] = iRec15[0];
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
	FAUSTFLOAT fHslider1;
	float fRec10[2];
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fCheckbox0;
	float fRec13[2];
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fHslider3;
	float fRec14[2];
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider4;
	float fRec17[2];
	float fRec16[2];
	float fRec23[2];
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	float fRec18[2];
	float fRec29[2];
	float fRec28[2];
	float fRec27[2];
	float fRec26[2];
	float fRec25[2];
	float fRec24[2];
	int iRec31[2];
	float fConst2;
	float fRec32[2];
	float fRec30[2];
	int iRec33[2];
	int IOTA;
	float fVec0[8192];
	float fConst3;
	float fRec12[3];
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fRec36[2];
	float fRec35[2];
	float fRec34[2];
	float fRec11[8192];
	float fConst8;
	FAUSTFLOAT fHslider5;
	float fVec1[4096];
	int iConst9;
	float fConst10;
	float fConst13;
	FAUSTFLOAT fHslider6;
	float fConst14;
	float fConst16;
	float fConst17;
	float fRec38[2];
	float fRec37[2];
	float fVec2[32768];
	int iConst19;
	float fVec3[4096];
	int iConst20;
	float fRec8[2];
	float fConst23;
	float fConst24;
	float fRec42[2];
	float fRec41[2];
	float fVec4[16384];
	int iConst26;
	float fVec5[4096];
	float fVec6[2048];
	int iConst27;
	float fRec39[2];
	float fConst30;
	float fConst31;
	float fRec46[2];
	float fRec45[2];
	float fVec7[16384];
	int iConst33;
	float fVec8[4096];
	int iConst34;
	float fRec43[2];
	float fConst37;
	float fConst38;
	float fRec50[2];
	float fRec49[2];
	float fVec9[16384];
	int iConst40;
	float fVec10[4096];
	int iConst41;
	float fRec47[2];
	float fConst44;
	float fConst45;
	float fRec54[2];
	float fRec53[2];
	float fVec11[32768];
	int iConst47;
	float fVec12[2048];
	int iConst48;
	float fRec51[2];
	float fConst51;
	float fConst52;
	float fRec58[2];
	float fRec57[2];
	float fVec13[16384];
	int iConst54;
	float fVec14[2048];
	int iConst55;
	float fRec55[2];
	float fConst58;
	float fConst59;
	float fRec62[2];
	float fRec61[2];
	float fVec15[32768];
	int iConst61;
	float fVec16[4096];
	int iConst62;
	float fRec59[2];
	float fConst65;
	float fConst66;
	float fRec66[2];
	float fRec65[2];
	float fVec17[16384];
	int iConst68;
	float fVec18[4096];
	int iConst69;
	float fRec63[2];
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
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.1");
		m->declare("author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("compressors.lib/compression_gain_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compression_gain_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compression_gain_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_lad_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_lad_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_lad_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/version", "0.1");
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
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (1.0f / fConst0);
		fConst2 = (7.0f / fConst0);
		fConst3 = (0.0022727272f * fConst0);
		fConst4 = std::exp((0.0f - (0.200000003f / fConst0)));
		fConst5 = (1.0f - fConst4);
		fConst6 = std::exp((0.0f - (0.100000001f / fConst0)));
		fConst7 = std::exp((0.0f - (0.0199999996f / fConst0)));
		fConst8 = (0.0011363636f * fConst0);
		iConst9 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		fConst10 = std::cos((37699.1133f / fConst0));
		float fConst11 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst12 = ((0.0f - (6.90775537f * fConst11)) / fConst0);
		fConst13 = (0.25f * fConst12);
		fConst14 = (0.166666672f * fConst12);
		float fConst15 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst16 = (1.0f / (fConst15 + 1.0f));
		fConst17 = (1.0f - fConst15);
		float fConst18 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst19 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst11 - fConst18))));
		iConst20 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst18 + -1.0f))));
		float fConst21 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst22 = ((0.0f - (6.90775537f * fConst21)) / fConst0);
		fConst23 = (0.25f * fConst22);
		fConst24 = (0.166666672f * fConst22);
		float fConst25 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst26 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst21 - fConst25))));
		iConst27 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst25 + -1.0f))));
		float fConst28 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst29 = ((0.0f - (6.90775537f * fConst28)) / fConst0);
		fConst30 = (0.25f * fConst29);
		fConst31 = (0.166666672f * fConst29);
		float fConst32 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst33 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst28 - fConst32))));
		iConst34 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst32 + -1.0f))));
		float fConst35 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst36 = ((0.0f - (6.90775537f * fConst35)) / fConst0);
		fConst37 = (0.25f * fConst36);
		fConst38 = (0.166666672f * fConst36);
		float fConst39 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst40 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst35 - fConst39))));
		iConst41 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst39 + -1.0f))));
		float fConst42 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst43 = ((0.0f - (6.90775537f * fConst42)) / fConst0);
		fConst44 = (0.25f * fConst43);
		fConst45 = (0.166666672f * fConst43);
		float fConst46 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst47 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst42 - fConst46))));
		iConst48 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst46 + -1.0f))));
		float fConst49 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst50 = ((0.0f - (6.90775537f * fConst49)) / fConst0);
		fConst51 = (0.25f * fConst50);
		fConst52 = (0.166666672f * fConst50);
		float fConst53 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst54 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst49 - fConst53))));
		iConst55 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst53 + -1.0f))));
		float fConst56 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst57 = ((0.0f - (6.90775537f * fConst56)) / fConst0);
		fConst58 = (0.25f * fConst57);
		fConst59 = (0.166666672f * fConst57);
		float fConst60 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst61 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst56 - fConst60))));
		iConst62 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst60 + -1.0f))));
		float fConst63 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst64 = ((0.0f - (6.90775537f * fConst63)) / fConst0);
		fConst65 = (0.25f * fConst64);
		fConst66 = (0.166666672f * fConst64);
		float fConst67 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst68 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst63 - fConst67))));
		iConst69 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst67 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.59999999999999998f);
		fHslider1 = FAUSTFLOAT(0.13700000000000001f);
		fHslider2 = FAUSTFLOAT(48.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fEntry0 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(220.0f);
		fHslider5 = FAUSTFLOAT(0.5f);
		fHslider6 = FAUSTFLOAT(0.35999999999999999f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec10[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec13[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec14[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec17[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec16[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec23[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec22[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec21[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec20[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec19[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec18[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec29[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec28[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec27[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec26[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec25[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec24[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			iRec31[l18] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec32[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec30[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			iRec33[l21] = 0;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 8192); l22 = (l22 + 1)) {
			fVec0[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec12[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec36[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec35[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec34[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 8192); l27 = (l27 + 1)) {
			fRec11[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 4096); l28 = (l28 + 1)) {
			fVec1[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec38[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec37[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 32768); l31 = (l31 + 1)) {
			fVec2[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 4096); l32 = (l32 + 1)) {
			fVec3[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec8[l33] = 0.0f;
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
		for (int l36 = 0; (l36 < 16384); l36 = (l36 + 1)) {
			fVec4[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 4096); l37 = (l37 + 1)) {
			fVec5[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2048); l38 = (l38 + 1)) {
			fVec6[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec39[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec46[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec45[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 16384); l42 = (l42 + 1)) {
			fVec7[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 4096); l43 = (l43 + 1)) {
			fVec8[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec43[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec50[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec49[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 16384); l47 = (l47 + 1)) {
			fVec9[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 4096); l48 = (l48 + 1)) {
			fVec10[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec47[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec54[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec53[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 32768); l52 = (l52 + 1)) {
			fVec11[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2048); l53 = (l53 + 1)) {
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
		for (int l57 = 0; (l57 < 16384); l57 = (l57 + 1)) {
			fVec13[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2048); l58 = (l58 + 1)) {
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
		for (int l63 = 0; (l63 < 4096); l63 = (l63 + 1)) {
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
		for (int l67 = 0; (l67 < 16384); l67 = (l67 + 1)) {
			fVec17[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 4096); l68 = (l68 + 1)) {
			fVec18[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec63[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec0[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec1[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec2[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec3[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec4[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec5[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 3); l76 = (l76 + 1)) {
			fRec6[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 3); l77 = (l77 + 1)) {
			fRec7[l77] = 0.0f;
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
		ui_interface->declare(&fHslider4, "tooltip", "Frequency of the sine wave for the modulation of theta (works if Modulation Type=3)");
		ui_interface->declare(&fHslider4, "unit", "Hz");
		ui_interface->addHorizontalSlider("modfreq", &fHslider4, 220.0f, 20.0f, 1000.0f, 0.100000001f);
		ui_interface->declare(&fEntry0, "tooltip", "0=theta is modulated by the incoming signal; 1=theta is modulated by the averaged incoming signal; 2=theta is modulated by the squared incoming signal; 3=theta is modulated by a sine wave of frequency freqMod; 4=theta is modulated by a sine wave of frequency freq;");
		ui_interface->declare(&fEntry0, "type", "int");
		ui_interface->addNumEntry("modtype", &fEntry0, 0.0f, 0.0f, 4.0f, 1.0f);
		ui_interface->declare(&fHslider3, "tooltip", "Nonlinearity factor (value between 0 and 1)");
		ui_interface->addHorizontalSlider("nonlin", &fHslider3, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pan", &fHslider0, 0.600000024f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider2, 48.0f, 12.0f, 96.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider1, 0.136999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("room_size", &fHslider6, 0.360000014f, 0.00499999989f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("width", &fHslider5, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (4.0f * fSlow0);
		float fSlow2 = (0.00100000005f * float(fHslider1));
		float fSlow3 = std::pow(2.0f, (0.0833333358f * (float(fHslider2) + -69.0f)));
		float fSlow4 = (440.0f * fSlow3);
		float fSlow5 = float(getValueLoopFilterb0(float(fSlow4)));
		float fSlow6 = float(fCheckbox0);
		int iSlow7 = (fSlow6 > 0.0f);
		int iSlow8 = (iSlow7 < 1);
		float fSlow9 = (0.00100000005f * ((0.999599993f * float(iSlow7)) + (0.899999976f * (float(iSlow8) * float(getValueReleaseLoopGain(float(fSlow4)))))));
		int iSlow10 = int(float(fEntry0));
		float fSlow11 = float((iSlow10 >= 3));
		float fSlow12 = (0.00100000005f * float(fHslider3));
		float fSlow13 = (440.0f * (fSlow3 * float((iSlow10 == 4))));
		float fSlow14 = float((iSlow10 != 4));
		float fSlow15 = (0.00100000005f * float(fHslider4));
		float fSlow16 = float((iSlow10 < 3));
		float fSlow17 = (3.14159274f * float((iSlow10 == 0)));
		float fSlow18 = (1.57079637f * float((iSlow10 == 1)));
		float fSlow19 = (3.14159274f * float((iSlow10 == 2)));
		int iSlow20 = (iSlow7 > 0);
		float fSlow21 = (fConst2 / float(getValueDryTapAmpT60(float(fSlow4))));
		float fSlow22 = (0.00100000005f * std::min<float>(1.0f, std::max<float>(0.0f, fSlow6)));
		int iSlow23 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst3 / fSlow3))));
		float fSlow24 = float(getValueLoopFiltera1(float(fSlow4)));
		float fSlow25 = float(getValueLoopFiltera2(float(fSlow4)));
		float fSlow26 = float(getValueLoopFilterb1(float(fSlow4)));
		float fSlow27 = float(getValueLoopFilterb2(float(fSlow4)));
		int iSlow28 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst8 * (float(fHslider5) / fSlow3)))));
		float fSlow29 = float(fHslider6);
		float fSlow30 = std::exp((fConst13 / fSlow29));
		float fSlow31 = synth_harpsichord_faustpower2_f(fSlow30);
		float fSlow32 = (1.0f - (fConst10 * fSlow31));
		float fSlow33 = (1.0f - fSlow31);
		float fSlow34 = (fSlow32 / fSlow33);
		float fSlow35 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow32) / synth_harpsichord_faustpower2_f(fSlow33)) + -1.0f)));
		float fSlow36 = (fSlow34 - fSlow35);
		float fSlow37 = (fSlow30 * (fSlow35 + (1.0f - fSlow34)));
		float fSlow38 = ((std::exp((fConst14 / fSlow29)) / fSlow30) + -1.0f);
		float fSlow39 = std::exp((fConst23 / fSlow29));
		float fSlow40 = synth_harpsichord_faustpower2_f(fSlow39);
		float fSlow41 = (1.0f - (fConst10 * fSlow40));
		float fSlow42 = (1.0f - fSlow40);
		float fSlow43 = (fSlow41 / fSlow42);
		float fSlow44 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow41) / synth_harpsichord_faustpower2_f(fSlow42)) + -1.0f)));
		float fSlow45 = (fSlow43 - fSlow44);
		float fSlow46 = (fSlow39 * (fSlow44 + (1.0f - fSlow43)));
		float fSlow47 = ((std::exp((fConst24 / fSlow29)) / fSlow39) + -1.0f);
		float fSlow48 = (4.0f * (1.0f - fSlow0));
		float fSlow49 = std::exp((fConst30 / fSlow29));
		float fSlow50 = synth_harpsichord_faustpower2_f(fSlow49);
		float fSlow51 = (1.0f - (fConst10 * fSlow50));
		float fSlow52 = (1.0f - fSlow50);
		float fSlow53 = (fSlow51 / fSlow52);
		float fSlow54 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow51) / synth_harpsichord_faustpower2_f(fSlow52)) + -1.0f)));
		float fSlow55 = (fSlow53 - fSlow54);
		float fSlow56 = (fSlow49 * (fSlow54 + (1.0f - fSlow53)));
		float fSlow57 = ((std::exp((fConst31 / fSlow29)) / fSlow49) + -1.0f);
		float fSlow58 = std::exp((fConst37 / fSlow29));
		float fSlow59 = synth_harpsichord_faustpower2_f(fSlow58);
		float fSlow60 = (1.0f - (fConst10 * fSlow59));
		float fSlow61 = (1.0f - fSlow59);
		float fSlow62 = (fSlow60 / fSlow61);
		float fSlow63 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow60) / synth_harpsichord_faustpower2_f(fSlow61)) + -1.0f)));
		float fSlow64 = (fSlow62 - fSlow63);
		float fSlow65 = (fSlow58 * (fSlow63 + (1.0f - fSlow62)));
		float fSlow66 = ((std::exp((fConst38 / fSlow29)) / fSlow58) + -1.0f);
		float fSlow67 = std::exp((fConst44 / fSlow29));
		float fSlow68 = synth_harpsichord_faustpower2_f(fSlow67);
		float fSlow69 = (1.0f - (fConst10 * fSlow68));
		float fSlow70 = (1.0f - fSlow68);
		float fSlow71 = (fSlow69 / fSlow70);
		float fSlow72 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow69) / synth_harpsichord_faustpower2_f(fSlow70)) + -1.0f)));
		float fSlow73 = (fSlow71 - fSlow72);
		float fSlow74 = (fSlow67 * (fSlow72 + (1.0f - fSlow71)));
		float fSlow75 = ((std::exp((fConst45 / fSlow29)) / fSlow67) + -1.0f);
		float fSlow76 = std::exp((fConst51 / fSlow29));
		float fSlow77 = synth_harpsichord_faustpower2_f(fSlow76);
		float fSlow78 = (1.0f - (fConst10 * fSlow77));
		float fSlow79 = (1.0f - fSlow77);
		float fSlow80 = (fSlow78 / fSlow79);
		float fSlow81 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow78) / synth_harpsichord_faustpower2_f(fSlow79)) + -1.0f)));
		float fSlow82 = (fSlow80 - fSlow81);
		float fSlow83 = (fSlow76 * (fSlow81 + (1.0f - fSlow80)));
		float fSlow84 = ((std::exp((fConst52 / fSlow29)) / fSlow76) + -1.0f);
		float fSlow85 = std::exp((fConst58 / fSlow29));
		float fSlow86 = synth_harpsichord_faustpower2_f(fSlow85);
		float fSlow87 = (1.0f - (fConst10 * fSlow86));
		float fSlow88 = (1.0f - fSlow86);
		float fSlow89 = (fSlow87 / fSlow88);
		float fSlow90 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow87) / synth_harpsichord_faustpower2_f(fSlow88)) + -1.0f)));
		float fSlow91 = (fSlow89 - fSlow90);
		float fSlow92 = (fSlow85 * (fSlow90 + (1.0f - fSlow89)));
		float fSlow93 = ((std::exp((fConst59 / fSlow29)) / fSlow85) + -1.0f);
		float fSlow94 = std::exp((fConst65 / fSlow29));
		float fSlow95 = synth_harpsichord_faustpower2_f(fSlow94);
		float fSlow96 = (1.0f - (fConst10 * fSlow95));
		float fSlow97 = (1.0f - fSlow95);
		float fSlow98 = (fSlow96 / fSlow97);
		float fSlow99 = std::sqrt(std::max<float>(0.0f, ((synth_harpsichord_faustpower2_f(fSlow96) / synth_harpsichord_faustpower2_f(fSlow97)) + -1.0f)));
		float fSlow100 = (fSlow98 - fSlow99);
		float fSlow101 = (fSlow94 * (fSlow99 + (1.0f - fSlow98)));
		float fSlow102 = ((std::exp((fConst66 / fSlow29)) / fSlow94) + -1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec10[0] = (fSlow2 + (0.999000013f * fRec10[1]));
			fRec13[0] = (fSlow9 + (0.999000013f * fRec13[1]));
			float fTemp0 = fRec11[((IOTA - 1) & 8191)];
			fRec14[0] = (fSlow12 + (0.999000013f * fRec14[1]));
			fRec17[0] = (fSlow15 + (0.999000013f * fRec17[1]));
			float fTemp1 = (fRec16[1] + (fConst1 * (fSlow13 + (fSlow14 * fRec17[0]))));
			fRec16[0] = (fTemp1 - std::floor(fTemp1));
			float fTemp2 = (3.14159274f * (fRec14[0] * ftbl0synth_harpsichordSIG0[int((65536.0f * fRec16[0]))]));
			float fTemp3 = std::sin(fTemp2);
			float fTemp4 = std::cos(fTemp2);
			float fTemp5 = ((fTemp0 * fTemp4) - (fTemp3 * fRec18[1]));
			float fTemp6 = ((fTemp4 * fTemp5) - (fTemp3 * fRec19[1]));
			float fTemp7 = ((fTemp4 * fTemp6) - (fTemp3 * fRec20[1]));
			float fTemp8 = ((fTemp4 * fTemp7) - (fTemp3 * fRec21[1]));
			float fTemp9 = ((fTemp4 * fTemp8) - (fTemp3 * fRec22[1]));
			fRec23[0] = ((fTemp4 * fTemp9) - (fTemp3 * fRec23[1]));
			fRec22[0] = ((fTemp3 * fTemp9) + (fTemp4 * fRec23[1]));
			fRec21[0] = ((fTemp3 * fTemp8) + (fTemp4 * fRec22[1]));
			fRec20[0] = ((fTemp3 * fTemp7) + (fTemp4 * fRec21[1]));
			fRec19[0] = ((fTemp3 * fTemp6) + (fTemp4 * fRec20[1]));
			fRec18[0] = ((fTemp3 * fTemp5) + (fTemp4 * fRec19[1]));
			float fTemp10 = (fRec14[0] * (((fSlow17 * fTemp0) + (fSlow18 * (fTemp0 + fRec11[((IOTA - 2) & 8191)]))) + (fSlow19 * synth_harpsichord_faustpower2_f(fTemp0))));
			float fTemp11 = std::sin(fTemp10);
			float fTemp12 = std::cos(fTemp10);
			float fTemp13 = ((fTemp0 * fTemp12) - (fTemp11 * fRec24[1]));
			float fTemp14 = ((fTemp12 * fTemp13) - (fTemp11 * fRec25[1]));
			float fTemp15 = ((fTemp12 * fTemp14) - (fTemp11 * fRec26[1]));
			float fTemp16 = ((fTemp12 * fTemp15) - (fTemp11 * fRec27[1]));
			float fTemp17 = ((fTemp12 * fTemp16) - (fTemp11 * fRec28[1]));
			fRec29[0] = ((fTemp12 * fTemp17) - (fTemp11 * fRec29[1]));
			fRec28[0] = ((fTemp11 * fTemp17) + (fTemp12 * fRec29[1]));
			fRec27[0] = ((fTemp11 * fTemp16) + (fTemp12 * fRec28[1]));
			fRec26[0] = ((fTemp11 * fTemp15) + (fTemp12 * fRec27[1]));
			fRec25[0] = ((fTemp11 * fTemp14) + (fTemp12 * fRec26[1]));
			fRec24[0] = ((fTemp11 * fTemp13) + (fTemp12 * fRec25[1]));
			iRec31[0] = ((iSlow7 * iRec31[1]) + 1);
			float fTemp18 = float((iRec31[0] + -1));
			float fTemp19 = float(((fTemp18 < 2.0f) & iSlow20));
			fRec32[0] = (fSlow22 + (0.999000013f * fRec32[1]));
			float fTemp20 = ((0.0301973838f * fTemp19) + (float(((fTemp18 >= 2.0f) | iSlow8)) * std::exp((0.0f - (fSlow21 / fRec32[0])))));
			fRec30[0] = ((fRec30[1] * fTemp20) + (0.150000006f * (fTemp19 * (1.0f - fTemp20))));
			iRec33[0] = ((1103515245 * iRec33[1]) + 12345);
			fVec0[(IOTA & 8191)] = ((fRec13[0] * ((fSlow11 * ((fTemp0 * fTemp3) + (fRec18[1] * fTemp4))) + (fSlow16 * ((fRec14[0] * ((fTemp0 * fTemp11) + (fRec24[1] * fTemp12))) + ((1.0f - fRec14[0]) * fTemp0))))) + (4.65661287e-10f * (fRec30[0] * float(iRec33[0]))));
			fRec12[0] = (fVec0[((IOTA - iSlow23) & 8191)] - ((fSlow24 * fRec12[1]) + (fSlow25 * fRec12[2])));
			float fTemp21 = (((fSlow5 * fRec12[0]) + (fSlow26 * fRec12[1])) + (fSlow27 * fRec12[2]));
			float fTemp22 = std::fabs(fTemp21);
			float fTemp23 = ((fRec35[1] > fTemp22) ? fConst7 : fConst6);
			fRec36[0] = ((fRec36[1] * fTemp23) + (fTemp22 * (1.0f - fTemp23)));
			fRec35[0] = fRec36[0];
			fRec34[0] = ((fConst4 * fRec34[1]) + (fConst5 * (0.0f - (0.5f * std::max<float>(((20.0f * std::log10(fRec35[0])) + -100.0f), 0.0f)))));
			fRec11[(IOTA & 8191)] = (fTemp21 * std::pow(10.0f, (0.0500000007f * fRec34[0])));
			float fTemp24 = fRec11[((IOTA - iSlow28) & 8191)];
			fVec1[(IOTA & 4095)] = (fSlow1 * (fRec10[0] * fTemp24));
			float fTemp25 = (0.300000012f * fVec1[((IOTA - iConst9) & 4095)]);
			fRec38[0] = (0.0f - (fConst16 * ((fConst17 * fRec38[1]) - (fRec1[1] + fRec1[2]))));
			fRec37[0] = ((fSlow36 * fRec37[1]) + (fSlow37 * (fRec1[1] + (fSlow38 * fRec38[0]))));
			fVec2[(IOTA & 32767)] = ((0.353553385f * fRec37[0]) + 9.99999968e-21f);
			float fTemp26 = (fTemp25 + ((0.600000024f * fRec8[1]) + fVec2[((IOTA - iConst19) & 32767)]));
			fVec3[(IOTA & 4095)] = fTemp26;
			fRec8[0] = fVec3[((IOTA - iConst20) & 4095)];
			float fRec9 = (0.0f - (0.600000024f * fTemp26));
			fRec42[0] = (0.0f - (fConst16 * ((fConst17 * fRec42[1]) - (fRec6[1] + fRec6[2]))));
			fRec41[0] = ((fSlow45 * fRec41[1]) + (fSlow46 * (fRec6[1] + (fSlow47 * fRec42[0]))));
			fVec4[(IOTA & 16383)] = ((0.353553385f * fRec41[0]) + 9.99999968e-21f);
			float fTemp27 = fRec11[((IOTA - 0) & 8191)];
			fVec5[(IOTA & 4095)] = (fSlow48 * (fTemp27 * fRec10[0]));
			float fTemp28 = (0.300000012f * fVec5[((IOTA - iConst9) & 4095)]);
			float fTemp29 = (fVec4[((IOTA - iConst26) & 16383)] - (fTemp28 + (0.600000024f * fRec39[1])));
			fVec6[(IOTA & 2047)] = fTemp29;
			fRec39[0] = fVec6[((IOTA - iConst27) & 2047)];
			float fRec40 = (0.600000024f * fTemp29);
			fRec46[0] = (0.0f - (fConst16 * ((fConst17 * fRec46[1]) - (fRec2[1] + fRec2[2]))));
			fRec45[0] = ((fSlow55 * fRec45[1]) + (fSlow56 * (fRec2[1] + (fSlow57 * fRec46[0]))));
			fVec7[(IOTA & 16383)] = ((0.353553385f * fRec45[0]) + 9.99999968e-21f);
			float fTemp30 = (fVec7[((IOTA - iConst33) & 16383)] - (fTemp28 + (0.600000024f * fRec43[1])));
			fVec8[(IOTA & 4095)] = fTemp30;
			fRec43[0] = fVec8[((IOTA - iConst34) & 4095)];
			float fRec44 = (0.600000024f * fTemp30);
			fRec50[0] = (0.0f - (fConst16 * ((fConst17 * fRec50[1]) - (fRec4[1] + fRec4[2]))));
			fRec49[0] = ((fSlow64 * fRec49[1]) + (fSlow65 * (fRec4[1] + (fSlow66 * fRec50[0]))));
			fVec9[(IOTA & 16383)] = ((0.353553385f * fRec49[0]) + 9.99999968e-21f);
			float fTemp31 = ((fTemp28 + fVec9[((IOTA - iConst40) & 16383)]) - (0.600000024f * fRec47[1]));
			fVec10[(IOTA & 4095)] = fTemp31;
			fRec47[0] = fVec10[((IOTA - iConst41) & 4095)];
			float fRec48 = (0.600000024f * fTemp31);
			fRec54[0] = (0.0f - (fConst16 * ((fConst17 * fRec54[1]) - (fRec7[1] + fRec7[2]))));
			fRec53[0] = ((fSlow73 * fRec53[1]) + (fSlow74 * (fRec7[1] + (fSlow75 * fRec54[0]))));
			fVec11[(IOTA & 32767)] = ((0.353553385f * fRec53[0]) + 9.99999968e-21f);
			float fTemp32 = (((0.600000024f * fRec51[1]) + fVec11[((IOTA - iConst47) & 32767)]) - fTemp25);
			fVec12[(IOTA & 2047)] = fTemp32;
			fRec51[0] = fVec12[((IOTA - iConst48) & 2047)];
			float fRec52 = (0.0f - (0.600000024f * fTemp32));
			fRec58[0] = (0.0f - (fConst16 * ((fConst17 * fRec58[1]) - (fRec0[1] + fRec0[2]))));
			fRec57[0] = ((fSlow82 * fRec57[1]) + (fSlow83 * (fRec0[1] + (fSlow84 * fRec58[0]))));
			fVec13[(IOTA & 16383)] = ((0.353553385f * fRec57[0]) + 9.99999968e-21f);
			float fTemp33 = ((fVec13[((IOTA - iConst54) & 16383)] + fTemp28) - (0.600000024f * fRec55[1]));
			fVec14[(IOTA & 2047)] = fTemp33;
			fRec55[0] = fVec14[((IOTA - iConst55) & 2047)];
			float fRec56 = (0.600000024f * fTemp33);
			fRec62[0] = (0.0f - (fConst16 * ((fConst17 * fRec62[1]) - (fRec3[1] + fRec3[2]))));
			fRec61[0] = ((fSlow91 * fRec61[1]) + (fSlow92 * (fRec3[1] + (fSlow93 * fRec62[0]))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec61[0]) + 9.99999968e-21f);
			float fTemp34 = (((0.600000024f * fRec59[1]) + fVec15[((IOTA - iConst61) & 32767)]) - fTemp25);
			fVec16[(IOTA & 4095)] = fTemp34;
			fRec59[0] = fVec16[((IOTA - iConst62) & 4095)];
			float fRec60 = (0.0f - (0.600000024f * fTemp34));
			fRec66[0] = (0.0f - (fConst16 * ((fConst17 * fRec66[1]) - (fRec5[1] + fRec5[2]))));
			fRec65[0] = ((fSlow100 * fRec65[1]) + (fSlow101 * (fRec5[1] + (fSlow102 * fRec66[0]))));
			fVec17[(IOTA & 16383)] = ((0.353553385f * fRec65[0]) + 9.99999968e-21f);
			float fTemp35 = (fVec17[((IOTA - iConst68) & 16383)] + (fTemp25 + (0.600000024f * fRec63[1])));
			fVec18[(IOTA & 4095)] = fTemp35;
			fRec63[0] = fVec18[((IOTA - iConst69) & 4095)];
			float fRec64 = (0.0f - (0.600000024f * fTemp35));
			float fTemp36 = (fRec64 + fRec60);
			float fTemp37 = (fRec52 + fTemp36);
			fRec0[0] = (fRec9 + (fRec40 + (fRec44 + (fRec48 + (fRec51[1] + (fRec56 + (fRec59[1] + (fRec63[1] + (fRec8[1] + (fRec39[1] + (fRec43[1] + (fRec47[1] + (fRec55[1] + fTemp37)))))))))))));
			fRec1[0] = ((fRec40 + (fRec44 + (fRec48 + (fRec56 + (fRec39[1] + ((fRec55[1] + fRec47[1]) + fRec43[1])))))) - (fRec9 + (fRec51[1] + (fRec59[1] + (fRec63[1] + (fRec8[1] + fTemp37))))));
			float fTemp38 = (fRec60 + fRec52);
			fRec2[0] = ((fRec9 + (fRec48 + (fRec56 + (fRec63[1] + (fRec8[1] + (fRec47[1] + (fRec64 + fRec55[1]))))))) - (fRec40 + (fRec44 + (fRec51[1] + (fRec59[1] + (fRec39[1] + (fRec43[1] + fTemp38)))))));
			fRec3[0] = ((fRec48 + (fRec51[1] + (fRec56 + (fRec59[1] + (fRec47[1] + (fRec55[1] + fTemp38)))))) - (fRec9 + (fRec40 + (fRec44 + (fRec63[1] + (fRec8[1] + (fRec39[1] + (fRec64 + fRec43[1]))))))));
			float fTemp39 = (fRec64 + fRec52);
			fRec4[0] = ((fRec9 + (fRec44 + (fRec56 + (fRec59[1] + (fRec8[1] + (fRec43[1] + (fRec60 + fRec55[1]))))))) - (fRec40 + (fRec48 + (fRec51[1] + (fRec63[1] + (fRec39[1] + (fRec47[1] + fTemp39)))))));
			fRec5[0] = ((fRec44 + (fRec51[1] + (fRec56 + (fRec63[1] + (fRec43[1] + (fRec55[1] + fTemp39)))))) - (fRec9 + (fRec40 + (fRec48 + (fRec59[1] + (fRec8[1] + (fRec39[1] + (fRec60 + fRec47[1]))))))));
			fRec6[0] = ((fRec9 + (fRec40 + (fRec51[1] + (fRec56 + (fRec8[1] + (fRec39[1] + (fRec52 + fRec55[1]))))))) - (fRec44 + (fRec48 + (fRec59[1] + (fRec63[1] + (fRec43[1] + (fRec47[1] + fTemp36)))))));
			fRec7[0] = ((fRec40 + (fRec56 + (fRec59[1] + (fRec63[1] + (fRec39[1] + (fRec55[1] + fTemp36)))))) - (fRec9 + (fRec44 + (fRec48 + (fRec51[1] + (fRec8[1] + (fRec43[1] + (fRec52 + fRec47[1]))))))));
			float fTemp40 = (1.0f - fRec10[0]);
			output0[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow48 * (fTemp27 * fTemp40))));
			output1[i] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow1 * (fTemp40 * fTemp24))));
			fRec10[1] = fRec10[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			iRec31[1] = iRec31[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			iRec33[1] = iRec33[0];
			IOTA = (IOTA + 1);
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec8[1] = fRec8[0];
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
