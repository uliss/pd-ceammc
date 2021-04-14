/* ------------------------------------------------------------
name: "lfo.mosc"
Code generated with Faust 2.30.12 (https://faust.grame.fr)
Compilation options: -lang cpp -es 1 -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __lfo_mosc_H__
#define  __lfo_mosc_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN lfo_mosc_dsp.h **************************/
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

class lfo_mosc_dsp {

    public:

        lfo_mosc_dsp() {}
        virtual ~lfo_mosc_dsp() {}

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
        virtual lfo_mosc_dsp* clone() = 0;
    
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

class decorator_dsp : public lfo_mosc_dsp {

    protected:

        lfo_mosc_dsp* fDSP;

    public:

        decorator_dsp(lfo_mosc_dsp* lfo_mosc_dsp = nullptr):fDSP(lfo_mosc_dsp) {}
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
    
        virtual lfo_mosc_dsp* createDSPInstance() = 0;
    
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
/**************************  END  lfo_mosc_dsp.h **************************/
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
 The base class of Meta handler to be used in lfo_mosc_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct lfo_mosc : public lfo_mosc_dsp {
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


#ifndef FAUSTCLASS 
#define FAUSTCLASS lfo_mosc
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class lfo_mosc : public lfo_mosc_dsp {
	
 private:
	
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fEntry0;
	float fRec0[2];
	float fRec1[2];
	float fRec2[2];
	float fRec3[2];
	FAUSTFLOAT fHslider0;
	float fVec0[2];
	float fConst2;
	FAUSTFLOAT fCheckbox0;
	float fRec4[2];
	float fRec5[2];
	FAUSTFLOAT fHslider1;
	float fRec6[2];
	float fRec7[2];
	float fRec8[2];
	float fRec9[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc_osc.lib/name", "CEAMMC faust oscillators");
		m->declare("ceammc_osc.lib/version", "0.1");
		m->declare("compile_options", "-lang cpp -es 1 -scal -ftz 0");
		m->declare("filename", "lfo_mosc.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "lfo.mosc");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
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
		fConst1 = (20.0f / fConst0);
		fConst2 = (1.0f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fEntry0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec0[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec9[l10] = 0.0f;
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
	
	virtual lfo_mosc* clone() {
		return new lfo_mosc();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("lfo.mosc");
		ui_interface->addHorizontalSlider("duty", &fHslider1, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addCheckButton("pause", &fCheckbox0);
		ui_interface->addHorizontalSlider("phase", &fHslider0, 0.0f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->addNumEntry("windex", &fEntry0, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fEntry0);
		float fSlow1 = float((fSlow0 >= 5.0f));
		float fSlow2 = float((fSlow0 >= 3.0f));
		float fSlow3 = float((fSlow0 >= 2.0f));
		float fSlow4 = float((fSlow0 >= 1.0f));
		float fSlow5 = float(fHslider0);
		float fSlow6 = (fConst2 * float(((float(fCheckbox0) > 0.5f) == 0)));
		float fSlow7 = float((fSlow0 >= 4.0f));
		float fSlow8 = float(fHslider1);
		float fSlow9 = float((fSlow0 >= 8.0f));
		float fSlow10 = float((fSlow0 >= 7.0f));
		float fSlow11 = float((fSlow0 >= 6.0f));
		float fSlow12 = float((fSlow0 >= 9.0f));
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = (fConst1 + fRec0[1]);
			float fTemp1 = (fRec0[1] - fConst1);
			fRec0[0] = ((fTemp0 < fSlow1) ? fTemp0 : ((fTemp1 > fSlow1) ? fTemp1 : fSlow1));
			float fTemp2 = (fConst1 + fRec1[1]);
			float fTemp3 = (fRec1[1] - fConst1);
			fRec1[0] = ((fTemp2 < fSlow2) ? fTemp2 : ((fTemp3 > fSlow2) ? fTemp3 : fSlow2));
			float fTemp4 = (fConst1 + fRec2[1]);
			float fTemp5 = (fRec2[1] - fConst1);
			fRec2[0] = ((fTemp4 < fSlow3) ? fTemp4 : ((fTemp5 > fSlow3) ? fTemp5 : fSlow3));
			float fTemp6 = (fConst1 + fRec3[1]);
			float fTemp7 = (fRec3[1] - fConst1);
			fRec3[0] = ((fTemp6 < fSlow4) ? fTemp6 : ((fTemp7 > fSlow4) ? fTemp7 : fSlow4));
			fVec0[0] = fSlow5;
			float fTemp8 = (fRec4[1] + (fSlow6 * float(input0[i])));
			fRec4[0] = (fSlow5 + (fTemp8 - (fVec0[1] + std::floor((fSlow5 + (fTemp8 - fVec0[1]))))));
			float fTemp9 = std::sin((6.28318548f * fRec4[0]));
			float fTemp10 = ((2.0f * fRec4[0]) + -1.0f);
			float fTemp11 = (1.0f - std::fabs(fTemp10));
			float fTemp12 = (fConst1 + fRec5[1]);
			float fTemp13 = (fRec5[1] - fConst1);
			fRec5[0] = ((fTemp12 < fSlow7) ? fTemp12 : ((fTemp13 > fSlow7) ? fTemp13 : fSlow7));
			float fTemp14 = float((fRec4[0] <= 0.5f));
			float fTemp15 = float((fRec4[0] <= fSlow8));
			float fTemp16 = (fConst1 + fRec6[1]);
			float fTemp17 = (fRec6[1] - fConst1);
			fRec6[0] = ((fTemp16 < fSlow9) ? fTemp16 : ((fTemp17 > fSlow9) ? fTemp17 : fSlow9));
			float fTemp18 = (fConst1 + fRec7[1]);
			float fTemp19 = (fRec7[1] - fConst1);
			fRec7[0] = ((fTemp18 < fSlow10) ? fTemp18 : ((fTemp19 > fSlow10) ? fTemp19 : fSlow10));
			float fTemp20 = (fConst1 + fRec8[1]);
			float fTemp21 = (fRec8[1] - fConst1);
			fRec8[0] = ((fTemp20 < fSlow11) ? fTemp20 : ((fTemp21 > fSlow11) ? fTemp21 : fSlow11));
			float fTemp22 = (fConst1 + fRec9[1]);
			float fTemp23 = (fRec9[1] - fConst1);
			fRec9[0] = ((fTemp22 < fSlow12) ? fTemp22 : ((fTemp23 > fSlow12) ? fTemp23 : fSlow12));
			output0[i] = FAUSTFLOAT((((1.0f - fRec0[0]) * (((1.0f - fRec1[0]) * (((1.0f - fRec2[0]) * (((1.0f - fRec3[0]) * fTemp9) + (fRec3[0] * fTemp10))) + (fRec2[0] * ((2.0f * fTemp11) + -1.0f)))) + (fRec1[0] * (((1.0f - fRec5[0]) * ((2.0f * fTemp14) + -1.0f)) + (fRec5[0] * ((2.0f * fTemp15) + -1.0f)))))) + (fRec0[0] * (((1.0f - fRec6[0]) * (((1.0f - fRec7[0]) * ((0.5f * ((1.0f - fRec8[0]) * (fTemp9 + 1.0f))) + (fRec4[0] * fRec8[0]))) + (fRec7[0] * fTemp11))) + (fRec6[0] * ((fTemp14 * (1.0f - fRec9[0])) + (fTemp15 * fRec9[0])))))));
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fVec0[1] = fVec0[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _lfo_mosc_UI : public UI {
    static std::string name;
};

template <class T>
std::string _lfo_mosc_UI<T>::name(sym(lfo_mosc));

typedef _lfo_mosc_UI<lfo_mosc> lfo_mosc_UI;

class faust_lfo_mosc_tilde : public FaustExternal<lfo_mosc, lfo_mosc_UI> {
public:
    faust_lfo_mosc_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
