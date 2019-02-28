/* ------------------------------------------------------------
name: "fx.zita_rev1"
Code generated with Faust 2.8.5 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_zita_rev1_H__
#define  __fx_zita_rev1_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

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

class UI;
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

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
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
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void init(int samplingRate) = 0;

        /**
         * Init instance state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceInit(int samplingRate) = 0;

        /**
         * Init instance constant state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int samplingRate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
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
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = 0):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int samplingRate) { fDSP->init(samplingRate); }
        virtual void instanceInit(int samplingRate) { fDSP->instanceInit(samplingRate); }
        virtual void instanceConstants(int samplingRate) { fDSP->instanceConstants(samplingRate); }
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
    
        virtual dsp* createDSPInstance() = 0;
    
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

class UI
{

    public:

        UI() {}

        virtual ~UI() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

#endif
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
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {};
};

#endif
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
#include <string.h>
#include <stdlib.h>
#include <cstdlib>


using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

inline int lsr(int x, int n)	{ return int(((unsigned int)x) >> n); }

inline int int2pow2(int x)		{ int r = 0; while ((1<<r) < x) r++; return r; }

inline long lopt(char* argv[], const char* name, long def)
{
	int	i;
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
	return def;
}

inline bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

inline const char* lopts(char* argv[], const char* name, const char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}

#endif


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
struct fx_zita_rev1 : public dsp {
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
#include <math.h>

static float fx_zita_rev1_faustpower2_f(float value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_zita_rev1
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_zita_rev1 : public dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	float fRec0[2];
	int IOTA;
	float fVec0[16384];
	int fSamplingFreq;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fVslider0;
	float fRec11[2];
	float fConst2;
	FAUSTFLOAT fVslider1;
	float fConst3;
	float fConst4;
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	float fConst5;
	FAUSTFLOAT fVslider4;
	float fRec13[2];
	float fRec12[2];
	float fVec1[65536];
	float fConst6;
	int iConst7;
	float fVec2[8192];
	int iConst8;
	float fRec9[2];
	float fVec3[16384];
	float fConst9;
	float fConst10;
	float fRec17[2];
	float fRec16[2];
	float fVec4[32768];
	float fConst11;
	int iConst12;
	float fVec5[4096];
	int iConst13;
	float fRec14[2];
	float fConst14;
	float fConst15;
	float fRec21[2];
	float fRec20[2];
	float fVec6[32768];
	float fConst16;
	int iConst17;
	float fVec7[4096];
	int iConst18;
	float fRec18[2];
	float fConst19;
	float fConst20;
	float fRec25[2];
	float fRec24[2];
	float fVec8[65536];
	float fConst21;
	int iConst22;
	float fVec9[4096];
	int iConst23;
	float fRec22[2];
	float fConst24;
	float fConst25;
	float fRec29[2];
	float fRec28[2];
	float fVec10[32768];
	float fConst26;
	int iConst27;
	float fVec11[8192];
	int iConst28;
	float fRec26[2];
	float fConst29;
	float fConst30;
	float fRec33[2];
	float fRec32[2];
	float fVec12[32768];
	float fConst31;
	int iConst32;
	float fVec13[8192];
	int iConst33;
	float fRec30[2];
	float fConst34;
	float fConst35;
	float fRec37[2];
	float fRec36[2];
	float fVec14[65536];
	float fConst36;
	int iConst37;
	float fVec15[8192];
	int iConst38;
	float fRec34[2];
	float fConst39;
	float fConst40;
	float fRec41[2];
	float fRec40[2];
	float fVec16[32768];
	float fConst41;
	int iConst42;
	float fVec17[8192];
	int iConst43;
	float fRec38[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.1");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.1");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("filename", "fx_zita_rev1");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "fx.zita_rev1");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 2;
		
	}
	virtual int getNumOutputs() {
		return 2;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
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
	virtual int getOutputRate(int channel) {
		int rate;
		switch (channel) {
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
	
	static void classInit(int samplingFreq) {
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = std::min(192000.0f, std::max(1.0f, float(fSamplingFreq)));
		fConst1 = (0.00100000005f * fConst0);
		fConst2 = (6.28318548f / fConst0);
		fConst3 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst4 = ((0.0f - (6.90775537f * fConst3)) / fConst0);
		fConst5 = (3.14159274f / fConst0);
		fConst6 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst7 = int(std::min(32768.0f, std::max(0.0f, (fConst3 - fConst6))));
		iConst8 = int(std::min(4096.0f, std::max(0.0f, (fConst6 + -1.0f))));
		fConst9 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst10 = ((0.0f - (6.90775537f * fConst9)) / fConst0);
		fConst11 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst12 = int(std::min(16384.0f, std::max(0.0f, (fConst9 - fConst11))));
		iConst13 = int(std::min(2048.0f, std::max(0.0f, (fConst11 + -1.0f))));
		fConst14 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst15 = ((0.0f - (6.90775537f * fConst14)) / fConst0);
		fConst16 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst17 = int(std::min(16384.0f, std::max(0.0f, (fConst14 - fConst16))));
		iConst18 = int(std::min(2048.0f, std::max(0.0f, (fConst16 + -1.0f))));
		fConst19 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst20 = ((0.0f - (6.90775537f * fConst19)) / fConst0);
		fConst21 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst22 = int(std::min(32768.0f, std::max(0.0f, (fConst19 - fConst21))));
		iConst23 = int(std::min(2048.0f, std::max(0.0f, (fConst21 + -1.0f))));
		fConst24 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst25 = ((0.0f - (6.90775537f * fConst24)) / fConst0);
		fConst26 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst27 = int(std::min(16384.0f, std::max(0.0f, (fConst24 - fConst26))));
		iConst28 = int(std::min(4096.0f, std::max(0.0f, (fConst26 + -1.0f))));
		fConst29 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst30 = ((0.0f - (6.90775537f * fConst29)) / fConst0);
		fConst31 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst32 = int(std::min(16384.0f, std::max(0.0f, (fConst29 - fConst31))));
		iConst33 = int(std::min(4096.0f, std::max(0.0f, (fConst31 + -1.0f))));
		fConst34 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst35 = ((0.0f - (6.90775537f * fConst34)) / fConst0);
		fConst36 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst37 = int(std::min(32768.0f, std::max(0.0f, (fConst34 - fConst36))));
		iConst38 = int(std::min(4096.0f, std::max(0.0f, (fConst36 + -1.0f))));
		fConst39 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst40 = ((0.0f - (6.90775537f * fConst39)) / fConst0);
		fConst41 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst42 = int(std::min(16384.0f, std::max(0.0f, (fConst39 - fConst41))));
		iConst43 = int(std::min(4096.0f, std::max(0.0f, (fConst41 + -1.0f))));
		
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fVslider0 = FAUSTFLOAT(60.0f);
		fVslider1 = FAUSTFLOAT(6000.0f);
		fVslider2 = FAUSTFLOAT(2.0f);
		fVslider3 = FAUSTFLOAT(3.0f);
		fVslider4 = FAUSTFLOAT(200.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
			
		}
		IOTA = 0;
		for (int l1 = 0; (l1 < 16384); l1 = (l1 + 1)) {
			fVec0[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec11[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec13[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec12[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 65536); l5 = (l5 + 1)) {
			fVec1[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 8192); l6 = (l6 + 1)) {
			fVec2[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec9[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 16384); l8 = (l8 + 1)) {
			fVec3[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec17[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec16[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 32768); l11 = (l11 + 1)) {
			fVec4[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 4096); l12 = (l12 + 1)) {
			fVec5[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec14[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec21[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec20[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 32768); l16 = (l16 + 1)) {
			fVec6[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 4096); l17 = (l17 + 1)) {
			fVec7[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec18[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec25[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec24[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 65536); l21 = (l21 + 1)) {
			fVec8[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 4096); l22 = (l22 + 1)) {
			fVec9[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec22[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec29[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec28[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 32768); l26 = (l26 + 1)) {
			fVec10[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 8192); l27 = (l27 + 1)) {
			fVec11[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec26[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec33[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec32[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 32768); l31 = (l31 + 1)) {
			fVec12[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 8192); l32 = (l32 + 1)) {
			fVec13[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec30[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec37[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec36[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 65536); l36 = (l36 + 1)) {
			fVec14[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 8192); l37 = (l37 + 1)) {
			fVec15[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec34[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec41[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec40[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 32768); l41 = (l41 + 1)) {
			fVec16[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 8192); l42 = (l42 + 1)) {
			fVec17[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec38[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec1[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec2[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec3[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec4[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec5[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec6[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec7[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec8[l51] = 0.0f;
			
		}
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual fx_zita_rev1* clone() {
		return new fx_zita_rev1();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.zita_rev1");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->declare(&fVslider3, "scale", "log");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "unit", "s");
		ui_interface->addVerticalSlider("decay_low", &fVslider3, 3.0f, 1.0f, 25.0f, 0.100000001f);
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "s");
		ui_interface->addVerticalSlider("decay_mid", &fVslider2, 2.0f, 1.0f, 25.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "unit", "ms");
		ui_interface->addVerticalSlider("delay", &fVslider0, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fVslider1, "scale", "log");
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "Hz");
		ui_interface->addVerticalSlider("dump_hf", &fVslider1, 6000.0f, 1500.0f, 47040.0f, 1.0f);
		ui_interface->declare(&fVslider4, "scale", "log");
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "unit", "Hz");
		ui_interface->addVerticalSlider("freq_low", &fVslider4, 200.0f, 50.0f, 1000.0f, 1.0f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = (0.00100000005f * float(fHslider0));
		float fSlow2 = (0.00100000005f * float(fVslider0));
		float fSlow3 = std::cos((fConst2 * float(fVslider1)));
		float fSlow4 = float(fVslider2);
		float fSlow5 = std::exp((fConst4 / fSlow4));
		float fSlow6 = fx_zita_rev1_faustpower2_f(fSlow5);
		float fSlow7 = (1.0f - (fSlow3 * fSlow6));
		float fSlow8 = (1.0f - fSlow6);
		float fSlow9 = (fSlow7 / fSlow8);
		float fSlow10 = std::sqrt(std::max(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow7) / fx_zita_rev1_faustpower2_f(fSlow8)) + -1.0f)));
		float fSlow11 = (fSlow9 - fSlow10);
		float fSlow12 = (fSlow5 * (fSlow10 + (1.0f - fSlow9)));
		float fSlow13 = float(fVslider3);
		float fSlow14 = ((std::exp((fConst4 / fSlow13)) / fSlow5) + -1.0f);
		float fSlow15 = (1.0f / std::tan((fConst5 * float(fVslider4))));
		float fSlow16 = (fSlow15 + 1.0f);
		float fSlow17 = (1.0f / fSlow16);
		float fSlow18 = (0.0f - ((1.0f - fSlow15) / fSlow16));
		float fSlow19 = std::exp((fConst10 / fSlow4));
		float fSlow20 = fx_zita_rev1_faustpower2_f(fSlow19);
		float fSlow21 = (1.0f - (fSlow20 * fSlow3));
		float fSlow22 = (1.0f - fSlow20);
		float fSlow23 = (fSlow21 / fSlow22);
		float fSlow24 = std::sqrt(std::max(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow21) / fx_zita_rev1_faustpower2_f(fSlow22)) + -1.0f)));
		float fSlow25 = (fSlow23 - fSlow24);
		float fSlow26 = (fSlow19 * (fSlow24 + (1.0f - fSlow23)));
		float fSlow27 = ((std::exp((fConst10 / fSlow13)) / fSlow19) + -1.0f);
		float fSlow28 = std::exp((fConst15 / fSlow4));
		float fSlow29 = fx_zita_rev1_faustpower2_f(fSlow28);
		float fSlow30 = (1.0f - (fSlow3 * fSlow29));
		float fSlow31 = (1.0f - fSlow29);
		float fSlow32 = (fSlow30 / fSlow31);
		float fSlow33 = std::sqrt(std::max(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow30) / fx_zita_rev1_faustpower2_f(fSlow31)) + -1.0f)));
		float fSlow34 = (fSlow32 - fSlow33);
		float fSlow35 = (fSlow28 * (fSlow33 + (1.0f - fSlow32)));
		float fSlow36 = ((std::exp((fConst15 / fSlow13)) / fSlow28) + -1.0f);
		float fSlow37 = std::exp((fConst20 / fSlow4));
		float fSlow38 = fx_zita_rev1_faustpower2_f(fSlow37);
		float fSlow39 = (1.0f - (fSlow3 * fSlow38));
		float fSlow40 = (1.0f - fSlow38);
		float fSlow41 = (fSlow39 / fSlow40);
		float fSlow42 = std::sqrt(std::max(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow39) / fx_zita_rev1_faustpower2_f(fSlow40)) + -1.0f)));
		float fSlow43 = (fSlow41 - fSlow42);
		float fSlow44 = (fSlow37 * (fSlow42 + (1.0f - fSlow41)));
		float fSlow45 = ((std::exp((fConst20 / fSlow13)) / fSlow37) + -1.0f);
		float fSlow46 = std::exp((fConst25 / fSlow4));
		float fSlow47 = fx_zita_rev1_faustpower2_f(fSlow46);
		float fSlow48 = (1.0f - (fSlow3 * fSlow47));
		float fSlow49 = (1.0f - fSlow47);
		float fSlow50 = (fSlow48 / fSlow49);
		float fSlow51 = std::sqrt(std::max(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow48) / fx_zita_rev1_faustpower2_f(fSlow49)) + -1.0f)));
		float fSlow52 = (fSlow50 - fSlow51);
		float fSlow53 = (fSlow46 * (fSlow51 + (1.0f - fSlow50)));
		float fSlow54 = ((std::exp((fConst25 / fSlow13)) / fSlow46) + -1.0f);
		float fSlow55 = std::exp((fConst30 / fSlow4));
		float fSlow56 = fx_zita_rev1_faustpower2_f(fSlow55);
		float fSlow57 = (1.0f - (fSlow56 * fSlow3));
		float fSlow58 = (1.0f - fSlow56);
		float fSlow59 = (fSlow57 / fSlow58);
		float fSlow60 = std::sqrt(std::max(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow57) / fx_zita_rev1_faustpower2_f(fSlow58)) + -1.0f)));
		float fSlow61 = (fSlow59 - fSlow60);
		float fSlow62 = (fSlow55 * (fSlow60 + (1.0f - fSlow59)));
		float fSlow63 = ((std::exp((fConst30 / fSlow13)) / fSlow55) + -1.0f);
		float fSlow64 = std::exp((fConst35 / fSlow4));
		float fSlow65 = fx_zita_rev1_faustpower2_f(fSlow64);
		float fSlow66 = (1.0f - (fSlow3 * fSlow65));
		float fSlow67 = (1.0f - fSlow65);
		float fSlow68 = (fSlow66 / fSlow67);
		float fSlow69 = std::sqrt(std::max(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow66) / fx_zita_rev1_faustpower2_f(fSlow67)) + -1.0f)));
		float fSlow70 = (fSlow68 - fSlow69);
		float fSlow71 = (fSlow64 * (fSlow69 + (1.0f - fSlow68)));
		float fSlow72 = ((std::exp((fConst35 / fSlow13)) / fSlow64) + -1.0f);
		float fSlow73 = std::exp((fConst40 / fSlow4));
		float fSlow74 = fx_zita_rev1_faustpower2_f(fSlow73);
		float fSlow75 = (1.0f - (fSlow74 * fSlow3));
		float fSlow76 = (1.0f - fSlow74);
		float fSlow77 = (fSlow75 / fSlow76);
		float fSlow78 = std::sqrt(std::max(0.0f, ((fx_zita_rev1_faustpower2_f(fSlow75) / fx_zita_rev1_faustpower2_f(fSlow76)) + -1.0f)));
		float fSlow79 = (fSlow77 - fSlow78);
		float fSlow80 = (fSlow73 * (fSlow78 + (1.0f - fSlow77)));
		float fSlow81 = ((std::exp((fConst40 / fSlow13)) / fSlow73) + -1.0f);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec0[0] = (fSlow1 + (0.999000013f * fRec0[1]));
			float fTemp0 = float(input1[i]);
			float fTemp1 = (iSlow0?0.0f:fTemp0);
			fVec0[(IOTA & 16383)] = fTemp1;
			fRec11[0] = (fSlow2 + (0.999000013f * fRec11[1]));
			int iTemp2 = int(std::min(8192.0f, std::max(0.0f, (fConst1 * fRec11[0]))));
			float fTemp3 = (0.300000012f * fVec0[((IOTA - iTemp2) & 16383)]);
			fRec13[0] = ((fSlow17 * (fRec2[1] + fRec2[2])) + (fSlow18 * fRec13[1]));
			fRec12[0] = ((fSlow11 * fRec12[1]) + (fSlow12 * (fRec2[1] + (fSlow14 * fRec13[0]))));
			fVec1[(IOTA & 65535)] = ((0.353553385f * fRec12[0]) + 9.99999968e-21f);
			float fTemp4 = (fTemp3 + ((0.600000024f * fRec9[1]) + fVec1[((IOTA - iConst7) & 65535)]));
			fVec2[(IOTA & 8191)] = fTemp4;
			fRec9[0] = fVec2[((IOTA - iConst8) & 8191)];
			float fRec10 = (0.0f - (0.600000024f * fTemp4));
			float fTemp5 = (fRec9[1] + fRec10);
			float fTemp6 = float(input0[i]);
			float fTemp7 = (iSlow0?0.0f:fTemp6);
			fVec3[(IOTA & 16383)] = fTemp7;
			float fTemp8 = (0.300000012f * fVec3[((IOTA - iTemp2) & 16383)]);
			fRec17[0] = ((fSlow17 * (fRec1[1] + fRec1[2])) + (fSlow18 * fRec17[1]));
			fRec16[0] = ((fSlow25 * fRec16[1]) + (fSlow26 * (fRec1[1] + (fSlow27 * fRec17[0]))));
			fVec4[(IOTA & 32767)] = ((0.353553385f * fRec16[0]) + 9.99999968e-21f);
			float fTemp9 = ((fTemp8 + fVec4[((IOTA - iConst12) & 32767)]) - (0.600000024f * fRec14[1]));
			fVec5[(IOTA & 4095)] = fTemp9;
			fRec14[0] = fVec5[((IOTA - iConst13) & 4095)];
			float fRec15 = (0.600000024f * fTemp9);
			float fTemp10 = ((fTemp5 + fRec14[1]) + fRec15);
			fRec21[0] = ((fSlow17 * (fRec7[1] + fRec7[2])) + (fSlow18 * fRec21[1]));
			fRec20[0] = ((fSlow34 * fRec20[1]) + (fSlow35 * (fRec7[1] + (fSlow36 * fRec21[0]))));
			fVec6[(IOTA & 32767)] = ((0.353553385f * fRec20[0]) + 9.99999968e-21f);
			float fTemp11 = (fVec6[((IOTA - iConst17) & 32767)] - (fTemp8 + (0.600000024f * fRec18[1])));
			fVec7[(IOTA & 4095)] = fTemp11;
			fRec18[0] = fVec7[((IOTA - iConst18) & 4095)];
			float fRec19 = (0.600000024f * fTemp11);
			fRec25[0] = ((fSlow17 * (fRec8[1] + fRec8[2])) + (fSlow18 * fRec25[1]));
			fRec24[0] = ((fSlow43 * fRec24[1]) + (fSlow44 * (fRec8[1] + (fSlow45 * fRec25[0]))));
			fVec8[(IOTA & 65535)] = ((0.353553385f * fRec24[0]) + 9.99999968e-21f);
			float fTemp12 = (((0.600000024f * fRec22[1]) + fVec8[((IOTA - iConst22) & 65535)]) - fTemp3);
			fVec9[(IOTA & 4095)] = fTemp12;
			fRec22[0] = fVec9[((IOTA - iConst23) & 4095)];
			float fRec23 = (0.0f - (0.600000024f * fTemp12));
			float fTemp13 = ((((fTemp10 + fRec18[1]) + fRec22[1]) + fRec23) + fRec19);
			fRec29[0] = ((fSlow17 * (fRec6[1] + fRec6[2])) + (fSlow18 * fRec29[1]));
			fRec28[0] = ((fSlow52 * fRec28[1]) + (fSlow53 * (fRec6[1] + (fSlow54 * fRec29[0]))));
			fVec10[(IOTA & 32767)] = ((0.353553385f * fRec28[0]) + 9.99999968e-21f);
			float fTemp14 = (fVec10[((IOTA - iConst27) & 32767)] + (fTemp3 + (0.600000024f * fRec26[1])));
			fVec11[(IOTA & 8191)] = fTemp14;
			fRec26[0] = fVec11[((IOTA - iConst28) & 8191)];
			float fRec27 = (0.0f - (0.600000024f * fTemp14));
			fRec33[0] = ((fSlow17 * (fRec5[1] + fRec5[2])) + (fSlow18 * fRec33[1]));
			fRec32[0] = ((fSlow61 * fRec32[1]) + (fSlow62 * (fRec5[1] + (fSlow63 * fRec33[0]))));
			fVec12[(IOTA & 32767)] = ((0.353553385f * fRec32[0]) + 9.99999968e-21f);
			float fTemp15 = ((fTemp8 + fVec12[((IOTA - iConst32) & 32767)]) - (0.600000024f * fRec30[1]));
			fVec13[(IOTA & 8191)] = fTemp15;
			fRec30[0] = fVec13[((IOTA - iConst33) & 8191)];
			float fRec31 = (0.600000024f * fTemp15);
			fRec37[0] = ((fSlow17 * (fRec4[1] + fRec4[2])) + (fSlow18 * fRec37[1]));
			fRec36[0] = ((fSlow70 * fRec36[1]) + (fSlow71 * (fRec4[1] + (fSlow72 * fRec37[0]))));
			fVec14[(IOTA & 65535)] = ((0.353553385f * fRec36[0]) + 9.99999968e-21f);
			float fTemp16 = (((0.600000024f * fRec34[1]) + fVec14[((IOTA - iConst37) & 65535)]) - fTemp3);
			fVec15[(IOTA & 8191)] = fTemp16;
			fRec34[0] = fVec15[((IOTA - iConst38) & 8191)];
			float fRec35 = (0.0f - (0.600000024f * fTemp16));
			fRec41[0] = ((fSlow17 * (fRec3[1] + fRec3[2])) + (fSlow18 * fRec41[1]));
			fRec40[0] = ((fSlow79 * fRec40[1]) + (fSlow80 * (fRec3[1] + (fSlow81 * fRec41[0]))));
			fVec16[(IOTA & 32767)] = ((0.353553385f * fRec40[0]) + 9.99999968e-21f);
			float fTemp17 = (fVec16[((IOTA - iConst42) & 32767)] - (fTemp8 + (0.600000024f * fRec38[1])));
			fVec17[(IOTA & 8191)] = fTemp17;
			fRec38[0] = fVec17[((IOTA - iConst43) & 8191)];
			float fRec39 = (0.600000024f * fTemp17);
			fRec1[0] = ((((((((fTemp13 + fRec26[1]) + fRec27) + fRec30[1]) + fRec31) + fRec34[1]) + fRec38[1]) + fRec39) + fRec35);
			float fTemp18 = (fRec14[1] + fRec15);
			float fTemp19 = ((fTemp18 + fRec18[1]) + fRec19);
			float fTemp20 = ((fTemp5 + fRec22[1]) + fRec23);
			fRec2[0] = (((((fTemp19 + fRec30[1]) + fRec31) + fRec38[1]) + fRec39) - ((((fTemp20 + fRec26[1]) + fRec27) + fRec34[1]) + fRec35));
			float fTemp21 = (((fRec18[1] + fRec22[1]) + fRec23) + fRec19);
			fRec3[0] = (((((fTemp10 + fRec26[1]) + fRec27) + fRec30[1]) + fRec31) - ((((fTemp21 + fRec34[1]) + fRec38[1]) + fRec39) + fRec35));
			float fTemp22 = ((fTemp18 + fRec22[1]) + fRec23);
			float fTemp23 = ((fTemp5 + fRec18[1]) + fRec19);
			fRec4[0] = (((((fTemp22 + fRec30[1]) + fRec31) + fRec34[1]) + fRec35) - ((((fTemp23 + fRec26[1]) + fRec27) + fRec38[1]) + fRec39));
			fRec5[0] = (((((fTemp10 + fRec34[1]) + fRec38[1]) + fRec39) + fRec35) - ((((fTemp21 + fRec26[1]) + fRec27) + fRec30[1]) + fRec31));
			fRec6[0] = (((((fTemp22 + fRec26[1]) + fRec27) + fRec38[1]) + fRec39) - ((((fTemp23 + fRec30[1]) + fRec31) + fRec34[1]) + fRec35));
			fRec7[0] = (fTemp13 - (((((((fRec26[1] + fRec27) + fRec30[1]) + fRec31) + fRec34[1]) + fRec38[1]) + fRec39) + fRec35));
			fRec8[0] = (((((fTemp19 + fRec26[1]) + fRec27) + fRec34[1]) + fRec35) - ((((fTemp20 + fRec30[1]) + fRec31) + fRec38[1]) + fRec39));
			float fTemp24 = (1.0f - fRec0[0]);
			output0[i] = FAUSTFLOAT((iSlow0?fTemp6:((0.370000005f * (fRec0[0] * (fRec2[0] + fRec3[0]))) + (fTemp24 * fTemp7))));
			output1[i] = FAUSTFLOAT((iSlow0?fTemp0:((fTemp24 * fTemp1) + (0.370000005f * (fRec0[0] * (fRec2[0] - fRec3[0]))))));
			fRec0[1] = fRec0[0];
			IOTA = (IOTA + 1);
			fRec11[1] = fRec11[0];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fRec9[1] = fRec9[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec14[1] = fRec14[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec18[1] = fRec18[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec26[1] = fRec26[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec34[1] = fRec34[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec38[1] = fRec38[0];
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
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			
		}
		
	}

	
};
// clang-format on
#endif

    template <class T>
    struct _fx_zita_rev1_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_zita_rev1_UI<T>::name(sym(fx_zita_rev1));

typedef _fx_zita_rev1_UI<fx_zita_rev1> fx_zita_rev1_UI;

class faust_fx_zita_rev1_tilde : public FaustExternal<fx_zita_rev1, fx_zita_rev1_UI> {
public:
    faust_fx_zita_rev1_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
