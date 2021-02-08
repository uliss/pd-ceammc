/* ------------------------------------------------------------
author: "brummer"
copyright: "(c)brummer 2008"
license: "BSD"
name: "fx.distortion2"
version: "0.01"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_distortion2_H__
#define  __fx_distortion2_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_distortion2_dsp.h **************************/
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

class fx_distortion2_dsp {

    public:

        fx_distortion2_dsp() {}
        virtual ~fx_distortion2_dsp() {}

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
        virtual fx_distortion2_dsp* clone() = 0;
    
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

class decorator_dsp : public fx_distortion2_dsp {

    protected:

        fx_distortion2_dsp* fDSP;

    public:

        decorator_dsp(fx_distortion2_dsp* fx_distortion2_dsp = nullptr):fDSP(fx_distortion2_dsp) {}
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
    
        virtual fx_distortion2_dsp* createDSPInstance() = 0;
    
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
/**************************  END  fx_distortion2_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_distortion2_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_distortion2 : public fx_distortion2_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

static float fx_distortion2_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_distortion2
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_distortion2 : public fx_distortion2_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fVslider1;
	int iVec0[2];
	int iRec4[2];
	float fConst7;
	float fConst9;
	FAUSTFLOAT fVslider2;
	float fRec6[2];
	FAUSTFLOAT fVslider3;
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fEntry1;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fVslider5;
	float fVec1[2];
	FAUSTFLOAT fVslider6;
	float fRec14[2];
	float fVec2[2];
	float fRec13[2];
	float fRec12[2];
	float fVec3[2];
	float fRec11[2];
	float fVec4[2];
	float fRec10[2];
	float fRec9[3];
	float fRec8[3];
	float fConst10;
	float fConst11;
	float fRec7[3];
	float fConst12;
	float fConst13;
	float fRec5[3];
	float fVec5[2];
	float fRec3[2];
	float fVec6[2];
	float fRec2[2];
	float fRec1[3];
	float fRec0[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("HighShelf.dsp/id", "HighShelf");
		m->declare("author", "brummer");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("copyright", "(c)brummer 2008");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_distortion2.dsp");
		m->declare("filters.lib/dcblockerat:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblockerat:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblockerat:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
		m->declare("license", "BSD");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("maxmsp.lib/author", "GRAME");
		m->declare("maxmsp.lib/copyright", "GRAME");
		m->declare("maxmsp.lib/license", "LGPL with exception");
		m->declare("maxmsp.lib/name", "MaxMSP compatibility Library");
		m->declare("maxmsp.lib/version", "1.1");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("name", "fx.distortion2");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "0.01");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
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
		fConst1 = (3.14159274f / fConst0);
		float fConst2 = (6.28318548f * (std::max<float>(0.0f, ((0.5f * fConst0) + -100.0f)) / fConst0));
		float fConst3 = std::cos(fConst2);
		float fConst4 = (0.683772206f * fConst3);
		float fConst5 = (0.00562341325f * std::sin(fConst2));
		float fConst6 = (fConst4 + fConst5);
		fConst7 = (1.0f / (fConst6 + 1.31622779f));
		float fConst8 = (1.31622779f * fConst3);
		fConst9 = (0.0f - (0.632455528f * (fConst8 + -0.683772206f)));
		fConst10 = (fConst4 + (1.31622779f - fConst5));
		fConst11 = (2.0f * (-0.683772206f - fConst8));
		fConst12 = (0.316227764f * (fConst5 + (1.31622779f - fConst4)));
		fConst13 = (0.316227764f * (1.31622779f - fConst6));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fVslider0 = FAUSTFLOAT(5000.0f);
		fVslider1 = FAUSTFLOAT(130.0f);
		fVslider2 = FAUSTFLOAT(2.0f);
		fVslider3 = FAUSTFLOAT(0.01f);
		fVslider4 = FAUSTFLOAT(0.64000000000000001f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fEntry0 = FAUSTFLOAT(130.0f);
		fEntry1 = FAUSTFLOAT(5000.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fVslider5 = FAUSTFLOAT(0.12f);
		fVslider6 = FAUSTFLOAT(1.0f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iRec4[l1] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec6[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fVec1[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec14[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec2[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec13[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec12[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec3[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec11[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fVec4[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec10[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec9[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec8[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec7[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec5[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fVec5[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec3[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fVec6[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec2[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec1[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec0[l21] = 0.0f;
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
	
	virtual fx_distortion2* clone() {
		return new fx_distortion2();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.distortion2");
		ui_interface->addVerticalSlider("drive", &fVslider4, 0.639999986f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fVslider2, "unit", "db");
		ui_interface->addVerticalSlider("gain", &fVslider2, 2.0f, -10.0f, 10.0f, 0.100000001f);
		ui_interface->addVerticalSlider("level", &fVslider3, 0.00999999978f, 0.0f, 0.5f, 0.00999999978f);
		ui_interface->openHorizontalBox("low_highcutoff");
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("sp_high_freq", &fVslider0, 5000.0f, 1000.0f, 12000.0f, 10.0f);
		ui_interface->declare(&fVslider1, "unit", "Hz");
		ui_interface->addVerticalSlider("sp_low_freq", &fVslider1, 130.0f, 20.0f, 1000.0f, 10.0f);
		ui_interface->addCheckButton("sp_on_off", &fCheckbox0);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("low_highpass");
		ui_interface->addCheckButton("flt_on_off", &fCheckbox1);
		ui_interface->declare(&fEntry0, "unit", "Hz");
		ui_interface->addNumEntry("hp_freq", &fEntry0, 130.0f, 20.0f, 7040.0f, 10.0f);
		ui_interface->declare(&fEntry1, "unit", "Hz");
		ui_interface->addNumEntry("lp_freq", &fEntry1, 5000.0f, 20.0f, 12000.0f, 10.0f);
		ui_interface->closeBox();
		ui_interface->addCheckButton("res_on_off", &fCheckbox2);
		ui_interface->addVerticalSlider("trigger", &fVslider5, 0.119999997f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addVerticalSlider("vibrato", &fVslider6, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = std::tan((fConst1 * float(fVslider0)));
		float fSlow2 = (1.0f / fSlow1);
		float fSlow3 = (1.0f / (((fSlow2 + 0.765366852f) / fSlow1) + 1.0f));
		float fSlow4 = (1.0f / (((fSlow2 + 1.84775901f) / fSlow1) + 1.0f));
		float fSlow5 = (fConst1 * float(fVslider1));
		float fSlow6 = (1.0f / (fSlow5 + 1.0f));
		float fSlow7 = (0.00100000005f * std::pow(10.0f, (0.0500000007f * (float(fVslider2) + -10.0f))));
		float fSlow8 = float(fVslider3);
		float fSlow9 = (fConst7 * std::pow(10.0f, (2.0f * float(fVslider4))));
		int iSlow10 = int(float(fCheckbox1));
		float fSlow11 = std::tan((fConst1 * float(fEntry0)));
		float fSlow12 = (1.0f / fSlow11);
		float fSlow13 = (fSlow12 + 1.0f);
		float fSlow14 = (0.0f - (1.0f / (fSlow11 * fSlow13)));
		float fSlow15 = (1.0f / std::tan((fConst1 * float(fEntry1))));
		float fSlow16 = (1.0f / (fSlow15 + 1.0f));
		float fSlow17 = (1.0f - fSlow15);
		int iSlow18 = int(float(fCheckbox2));
		float fSlow19 = (1.0f - float(fVslider5));
		float fSlow20 = float(fVslider6);
		int iSlow21 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fSlow20 + -1.0f))));
		int iSlow22 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fSlow20)));
		float fSlow23 = (1.0f / fSlow13);
		float fSlow24 = (1.0f - fSlow12);
		float fSlow25 = (1.0f - fSlow5);
		float fSlow26 = (((fSlow2 + -1.84775901f) / fSlow1) + 1.0f);
		float fSlow27 = (2.0f * (1.0f - (1.0f / fx_distortion2_faustpower2_f(fSlow1))));
		float fSlow28 = (((fSlow2 + -0.765366852f) / fSlow1) + 1.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			iRec4[0] = ((1661992960 * (1 - iVec0[1])) - iRec4[1]);
			float fTemp0 = float(iRec4[0]);
			fRec6[0] = (fSlow7 + (0.999000013f * fRec6[1]));
			float fTemp1 = float(input0[i]);
			float fTemp2 = (fTemp1 + (fSlow19 * fRec14[1]));
			fVec1[0] = fTemp2;
			fRec14[0] = (0.5f * (fVec1[iSlow21] + fVec1[iSlow22]));
			float fTemp3 = ((iSlow18 ? fRec14[0] : fTemp1) + fTemp0);
			float fTemp4 = (fTemp0 + (iSlow10 ? 0.0f : fTemp3));
			fVec2[0] = fTemp4;
			fRec13[0] = (0.0f - (fSlow16 * ((fSlow17 * fRec13[1]) - (fTemp4 + fVec2[1]))));
			fRec12[0] = ((fSlow14 * fRec13[1]) - (fSlow23 * ((fSlow24 * fRec12[1]) - (fSlow12 * fRec13[0]))));
			float fTemp5 = ((iSlow10 ? fTemp3 : fRec12[0]) + 1.66199296e+09f);
			float fTemp6 = (fTemp0 + (iSlow0 ? 0.0f : fTemp5));
			fVec3[0] = (fSlow6 * fTemp6);
			fRec11[0] = ((fSlow6 * (fTemp6 + (fSlow25 * fRec11[1]))) - fVec3[1]);
			fVec4[0] = (fSlow6 * fRec11[0]);
			fRec10[0] = ((fSlow6 * (fRec11[0] + (fSlow25 * fRec10[1]))) - fVec4[1]);
			fRec9[0] = (fRec10[0] - (fSlow4 * ((fSlow26 * fRec9[2]) + (fSlow27 * fRec9[1]))));
			fRec8[0] = ((fSlow4 * (fRec9[2] + (fRec9[0] + (2.0f * fRec9[1])))) - (fSlow3 * ((fSlow28 * fRec8[2]) + (fSlow27 * fRec8[1]))));
			fRec7[0] = ((iSlow0 ? fTemp5 : (fSlow3 * (fRec8[2] + (fRec8[0] + (2.0f * fRec8[1]))))) - (fConst7 * ((fConst10 * fRec7[2]) + (fConst11 * fRec7[1]))));
			float fTemp7 = std::max<float>(-1.0f, std::min<float>(1.0f, (fSlow8 + (fSlow9 * (((fConst9 * fRec7[1]) + (fConst12 * fRec7[0])) + (fConst13 * fRec7[2]))))));
			fRec5[0] = (((fRec6[0] * fTemp7) * (1.0f - (0.333333343f * fx_distortion2_faustpower2_f(fTemp7)))) - (fConst7 * ((fConst10 * fRec5[2]) + (fConst11 * fRec5[1]))));
			float fTemp8 = (fConst7 * (((fConst9 * fRec5[1]) + (fConst12 * fRec5[0])) + (fConst13 * fRec5[2])));
			float fTemp9 = (fTemp0 + (iSlow0 ? 0.0f : fTemp8));
			fVec5[0] = (fSlow6 * fTemp9);
			fRec3[0] = ((fSlow6 * (fTemp9 + (fSlow25 * fRec3[1]))) - fVec5[1]);
			fVec6[0] = (fSlow6 * fRec3[0]);
			fRec2[0] = ((fSlow6 * (fRec3[0] + (fSlow25 * fRec2[1]))) - fVec6[1]);
			fRec1[0] = (fRec2[0] - (fSlow4 * ((fSlow26 * fRec1[2]) + (fSlow27 * fRec1[1]))));
			fRec0[0] = ((fSlow4 * (fRec1[2] + (fRec1[0] + (2.0f * fRec1[1])))) - (fSlow3 * ((fSlow28 * fRec0[2]) + (fSlow27 * fRec0[1]))));
			output0[i] = FAUSTFLOAT((iSlow0 ? fTemp8 : (fSlow3 * (fRec0[2] + (fRec0[0] + (2.0f * fRec0[1]))))));
			iVec0[1] = iVec0[0];
			iRec4[1] = iRec4[0];
			fRec6[1] = fRec6[0];
			fVec1[1] = fVec1[0];
			fRec14[1] = fRec14[0];
			fVec2[1] = fVec2[0];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fVec3[1] = fVec3[0];
			fRec11[1] = fRec11[0];
			fVec4[1] = fVec4[0];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec5[1] = fVec5[0];
			fRec3[1] = fRec3[0];
			fVec6[1] = fVec6[0];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_distortion2_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_distortion2_UI<T>::name(sym(fx_distortion2));

typedef _fx_distortion2_UI<fx_distortion2> fx_distortion2_UI;

class faust_fx_distortion2_tilde : public FaustExternal<fx_distortion2, fx_distortion2_UI> {
public:
    faust_fx_distortion2_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
