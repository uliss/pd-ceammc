/* ------------------------------------------------------------
author: "Mayank Sanganeria"
name: "fx.granulator"
version: "1.0"
Code generated with Faust 2.8.5 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_granulator_H__
#define  __fx_granulator_H__

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
struct fx_granulator : public dsp {
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


class fx_granulatorSIG0 {
	
  private:
	
	int iRec0[2];
	
  public:
	
	int getNumInputsfx_granulatorSIG0() {
		return 0;
		
	}
	int getNumOutputsfx_granulatorSIG0() {
		return 1;
		
	}
	int getInputRatefx_granulatorSIG0(int channel) {
		int rate;
		switch (channel) {
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	int getOutputRatefx_granulatorSIG0(int channel) {
		int rate;
		switch (channel) {
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
	
	void instanceInitfx_granulatorSIG0(int samplingFreq) {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iRec0[l0] = 0;
			
		}
		
	}
	
	void fillfx_granulatorSIG0(int count, float* output) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec0[0] = (iRec0[1] + 1);
			output[i] = (0.5f * (1.0f - std::cos((0.00614192104f * float((iRec0[0] + -1))))));
			iRec0[1] = iRec0[0];
			
		}
		
	}
};

fx_granulatorSIG0* newfx_granulatorSIG0() { return (fx_granulatorSIG0*)new fx_granulatorSIG0(); }
void deletefx_granulatorSIG0(fx_granulatorSIG0* dsp) { delete dsp; }


class fx_granulatorSIG1 {
	
  private:
	
	
  public:
	
	int getNumInputsfx_granulatorSIG1() {
		return 0;
		
	}
	int getNumOutputsfx_granulatorSIG1() {
		return 1;
		
	}
	int getInputRatefx_granulatorSIG1(int channel) {
		int rate;
		switch (channel) {
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	int getOutputRatefx_granulatorSIG1(int channel) {
		int rate;
		switch (channel) {
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
	
	void instanceInitfx_granulatorSIG1(int samplingFreq) {
		
	}
	
	void fillfx_granulatorSIG1(int count, float* output) {
		for (int i = 0; (i < count); i = (i + 1)) {
			output[i] = 0.0f;
			
		}
		
	}
};

fx_granulatorSIG1* newfx_granulatorSIG1() { return (fx_granulatorSIG1*)new fx_granulatorSIG1(); }
void deletefx_granulatorSIG1(fx_granulatorSIG1* dsp) { delete dsp; }

static float ftbl0fx_granulatorSIG0[1024];

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_granulator
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_granulator : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int iVec0[2];
	int fSamplingFreq;
	float fConst0;
	FAUSTFLOAT fHslider1;
	int iRec2[2];
	int iRec1[2];
	FAUSTFLOAT fHslider2;
	float fRec67[2];
	int iRec66[2];
	float ftbl1[480000];
	int iRec68[2];
	int iRec69[2];
	int iRec70[2];
	int iRec71[2];
	int iRec73[2];
	int iRec72[2];
	int iRec74[2];
	int iRec75[2];
	int iRec76[2];
	int iRec77[2];
	int iRec78[2];
	int iRec79[2];
	int iRec80[2];
	int iRec81[2];
	int iRec82[2];
	int iRec83[2];
	int iRec84[2];
	int iRec85[2];
	int iRec86[2];
	int iRec87[2];
	int iRec88[2];
	int iRec89[2];
	int iRec90[2];
	int iRec91[2];
	int iRec92[2];
	int iRec93[2];
	int iRec95[2];
	int iRec94[2];
	int iRec97[2];
	int iRec96[2];
	int iRec99[2];
	int iRec98[2];
	int iRec101[2];
	int iRec100[2];
	int iRec103[2];
	int iRec102[2];
	int iRec105[2];
	int iRec104[2];
	int iRec106[2];
	int iRec107[2];
	int iRec108[2];
	int iRec109[2];
	int iRec110[2];
	int iRec111[2];
	int iRec113[2];
	int iRec112[2];
	int iRec114[2];
	int iRec115[2];
	int iRec116[2];
	int iRec117[2];
	int iRec118[2];
	int iRec119[2];
	int iRec120[2];
	int iRec121[2];
	int iRec122[2];
	int iRec123[2];
	int iRec124[2];
	int iRec125[2];
	int iRec126[2];
	int iRec127[2];
	int iRec129[2];
	int iRec128[2];
	int iRec130[2];
	int iRec131[2];
	int iRec132[2];
	int iRec133[2];
	int iRec135[2];
	int iRec134[2];
	int iRec136[2];
	int iRec137[2];
	int iRec138[2];
	int iRec139[2];
	int iRec141[2];
	int iRec140[2];
	int iRec143[2];
	int iRec142[2];
	int iRec144[2];
	int iRec145[2];
	int iRec146[2];
	int iRec147[2];
	int iRec148[2];
	int iRec149[2];
	int iRec151[2];
	int iRec150[2];
	int iRec153[2];
	int iRec152[2];
	int iRec154[2];
	int iRec155[2];
	int iRec157[2];
	int iRec156[2];
	int iRec159[2];
	int iRec158[2];
	int iRec160[2];
	int iRec161[2];
	int iRec162[2];
	int iRec163[2];
	int iRec164[2];
	int iRec165[2];
	int iRec166[2];
	int iRec167[2];
	int iRec168[2];
	int iRec169[2];
	int iRec170[2];
	int iRec171[2];
	int iRec172[2];
	int iRec173[2];
	int iRec174[2];
	int iRec175[2];
	int iRec176[2];
	int iRec177[2];
	int iRec179[2];
	int iRec178[2];
	int iRec180[2];
	int iRec181[2];
	int iRec183[2];
	int iRec182[2];
	int iRec185[2];
	int iRec184[2];
	int iRec187[2];
	int iRec186[2];
	int iRec189[2];
	int iRec188[2];
	int iRec190[2];
	int iRec191[2];
	int iRec192[2];
	int iRec193[2];
	int iRec195[2];
	int iRec194[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Mayank Sanganeria");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.1");
		m->declare("filename", "fx_granulator");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "fx.granulator");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 1;
		
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
		fx_granulatorSIG0* sig0 = newfx_granulatorSIG0();
		sig0->instanceInitfx_granulatorSIG0(samplingFreq);
		sig0->fillfx_granulatorSIG0(1024, ftbl0fx_granulatorSIG0);
		deletefx_granulatorSIG0(sig0);
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = std::min(192000.0f, std::max(1.0f, float(fSamplingFreq)));
		fx_granulatorSIG1* sig1 = newfx_granulatorSIG1();
		sig1->instanceInitfx_granulatorSIG1(samplingFreq);
		sig1->fillfx_granulatorSIG1(480000, ftbl1);
		deletefx_granulatorSIG1(sig1);
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(10.0f);
		fHslider1 = FAUSTFLOAT(10.0f);
		fHslider2 = FAUSTFLOAT(100.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iVec0[l1] = 0;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec2[l2] = 0;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec1[l3] = 0;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec67[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec66[l5] = 0;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec68[l6] = 0;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec69[l7] = 0;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			iRec70[l8] = 0;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			iRec71[l9] = 0;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			iRec73[l10] = 0;
			
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			iRec72[l11] = 0;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			iRec74[l12] = 0;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			iRec75[l13] = 0;
			
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			iRec76[l14] = 0;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			iRec77[l15] = 0;
			
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			iRec78[l16] = 0;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			iRec79[l17] = 0;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			iRec80[l18] = 0;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			iRec81[l19] = 0;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			iRec82[l20] = 0;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			iRec83[l21] = 0;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			iRec84[l22] = 0;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			iRec85[l23] = 0;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			iRec86[l24] = 0;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			iRec87[l25] = 0;
			
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			iRec88[l26] = 0;
			
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			iRec89[l27] = 0;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			iRec90[l28] = 0;
			
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			iRec91[l29] = 0;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			iRec92[l30] = 0;
			
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			iRec93[l31] = 0;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			iRec95[l32] = 0;
			
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			iRec94[l33] = 0;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			iRec97[l34] = 0;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			iRec96[l35] = 0;
			
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			iRec99[l36] = 0;
			
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			iRec98[l37] = 0;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			iRec101[l38] = 0;
			
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			iRec100[l39] = 0;
			
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			iRec103[l40] = 0;
			
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			iRec102[l41] = 0;
			
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			iRec105[l42] = 0;
			
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			iRec104[l43] = 0;
			
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			iRec106[l44] = 0;
			
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			iRec107[l45] = 0;
			
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			iRec108[l46] = 0;
			
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			iRec109[l47] = 0;
			
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			iRec110[l48] = 0;
			
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			iRec111[l49] = 0;
			
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			iRec113[l50] = 0;
			
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			iRec112[l51] = 0;
			
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			iRec114[l52] = 0;
			
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			iRec115[l53] = 0;
			
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			iRec116[l54] = 0;
			
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			iRec117[l55] = 0;
			
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			iRec118[l56] = 0;
			
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			iRec119[l57] = 0;
			
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			iRec120[l58] = 0;
			
		}
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			iRec121[l59] = 0;
			
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			iRec122[l60] = 0;
			
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			iRec123[l61] = 0;
			
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			iRec124[l62] = 0;
			
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			iRec125[l63] = 0;
			
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			iRec126[l64] = 0;
			
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			iRec127[l65] = 0;
			
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			iRec129[l66] = 0;
			
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			iRec128[l67] = 0;
			
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			iRec130[l68] = 0;
			
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			iRec131[l69] = 0;
			
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			iRec132[l70] = 0;
			
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			iRec133[l71] = 0;
			
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			iRec135[l72] = 0;
			
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			iRec134[l73] = 0;
			
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			iRec136[l74] = 0;
			
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			iRec137[l75] = 0;
			
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			iRec138[l76] = 0;
			
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			iRec139[l77] = 0;
			
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			iRec141[l78] = 0;
			
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			iRec140[l79] = 0;
			
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			iRec143[l80] = 0;
			
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			iRec142[l81] = 0;
			
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			iRec144[l82] = 0;
			
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			iRec145[l83] = 0;
			
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			iRec146[l84] = 0;
			
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			iRec147[l85] = 0;
			
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			iRec148[l86] = 0;
			
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			iRec149[l87] = 0;
			
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			iRec151[l88] = 0;
			
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			iRec150[l89] = 0;
			
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			iRec153[l90] = 0;
			
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			iRec152[l91] = 0;
			
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			iRec154[l92] = 0;
			
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			iRec155[l93] = 0;
			
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			iRec157[l94] = 0;
			
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			iRec156[l95] = 0;
			
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			iRec159[l96] = 0;
			
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			iRec158[l97] = 0;
			
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			iRec160[l98] = 0;
			
		}
		for (int l99 = 0; (l99 < 2); l99 = (l99 + 1)) {
			iRec161[l99] = 0;
			
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			iRec162[l100] = 0;
			
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			iRec163[l101] = 0;
			
		}
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			iRec164[l102] = 0;
			
		}
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			iRec165[l103] = 0;
			
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			iRec166[l104] = 0;
			
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			iRec167[l105] = 0;
			
		}
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			iRec168[l106] = 0;
			
		}
		for (int l107 = 0; (l107 < 2); l107 = (l107 + 1)) {
			iRec169[l107] = 0;
			
		}
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			iRec170[l108] = 0;
			
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			iRec171[l109] = 0;
			
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			iRec172[l110] = 0;
			
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			iRec173[l111] = 0;
			
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			iRec174[l112] = 0;
			
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			iRec175[l113] = 0;
			
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			iRec176[l114] = 0;
			
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			iRec177[l115] = 0;
			
		}
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			iRec179[l116] = 0;
			
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			iRec178[l117] = 0;
			
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			iRec180[l118] = 0;
			
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			iRec181[l119] = 0;
			
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			iRec183[l120] = 0;
			
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			iRec182[l121] = 0;
			
		}
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			iRec185[l122] = 0;
			
		}
		for (int l123 = 0; (l123 < 2); l123 = (l123 + 1)) {
			iRec184[l123] = 0;
			
		}
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			iRec187[l124] = 0;
			
		}
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			iRec186[l125] = 0;
			
		}
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			iRec189[l126] = 0;
			
		}
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			iRec188[l127] = 0;
			
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			iRec190[l128] = 0;
			
		}
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			iRec191[l129] = 0;
			
		}
		for (int l130 = 0; (l130 < 2); l130 = (l130 + 1)) {
			iRec192[l130] = 0;
			
		}
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			iRec193[l131] = 0;
			
		}
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			iRec195[l132] = 0;
			
		}
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			iRec194[l133] = 0;
			
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
	
	virtual fx_granulator* clone() {
		return new fx_granulator();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.granulator");
		ui_interface->declare(&fHslider1, "unit", "sec");
		ui_interface->addHorizontalSlider("delay", &fHslider1, 10.0f, 0.5f, 10.0f, 0.100000001f);
		ui_interface->addHorizontalSlider("density", &fHslider0, 10.0f, 1.0f, 64.0f, 1.0f);
		ui_interface->declare(&fHslider2, "unit", "ms");
		ui_interface->addHorizontalSlider("length", &fHslider2, 100.0f, 10.0f, 500.0f, 0.00999999978f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (1.0f / fSlow0);
		int iSlow2 = (58.0f < fSlow0);
		int iSlow3 = int((fConst0 * float(fHslider1)));
		float fSlow4 = (0.5f * float(iSlow3));
		float fSlow5 = (9.99999997e-07f * float(fHslider2));
		int iSlow6 = (46.0f < fSlow0);
		int iSlow7 = (40.0f < fSlow0);
		int iSlow8 = (18.0f < fSlow0);
		int iSlow9 = (4.0f < fSlow0);
		int iSlow10 = (2.0f < fSlow0);
		int iSlow11 = (0.0f < fSlow0);
		int iSlow12 = (6.0f < fSlow0);
		int iSlow13 = (8.0f < fSlow0);
		int iSlow14 = (10.0f < fSlow0);
		int iSlow15 = (12.0f < fSlow0);
		int iSlow16 = (14.0f < fSlow0);
		int iSlow17 = (16.0f < fSlow0);
		int iSlow18 = (20.0f < fSlow0);
		int iSlow19 = (22.0f < fSlow0);
		int iSlow20 = (24.0f < fSlow0);
		int iSlow21 = (26.0f < fSlow0);
		int iSlow22 = (28.0f < fSlow0);
		int iSlow23 = (30.0f < fSlow0);
		int iSlow24 = (32.0f < fSlow0);
		int iSlow25 = (34.0f < fSlow0);
		int iSlow26 = (36.0f < fSlow0);
		int iSlow27 = (38.0f < fSlow0);
		int iSlow28 = (42.0f < fSlow0);
		int iSlow29 = (44.0f < fSlow0);
		int iSlow30 = (48.0f < fSlow0);
		int iSlow31 = (50.0f < fSlow0);
		int iSlow32 = (52.0f < fSlow0);
		int iSlow33 = (54.0f < fSlow0);
		int iSlow34 = (56.0f < fSlow0);
		int iSlow35 = (60.0f < fSlow0);
		int iSlow36 = (62.0f < fSlow0);
		int iSlow37 = (53.0f < fSlow0);
		int iSlow38 = (49.0f < fSlow0);
		int iSlow39 = (33.0f < fSlow0);
		int iSlow40 = (29.0f < fSlow0);
		int iSlow41 = (9.0f < fSlow0);
		int iSlow42 = (1.0f < fSlow0);
		int iSlow43 = (3.0f < fSlow0);
		int iSlow44 = (5.0f < fSlow0);
		int iSlow45 = (7.0f < fSlow0);
		int iSlow46 = (11.0f < fSlow0);
		int iSlow47 = (13.0f < fSlow0);
		int iSlow48 = (15.0f < fSlow0);
		int iSlow49 = (17.0f < fSlow0);
		int iSlow50 = (19.0f < fSlow0);
		int iSlow51 = (21.0f < fSlow0);
		int iSlow52 = (23.0f < fSlow0);
		int iSlow53 = (25.0f < fSlow0);
		int iSlow54 = (27.0f < fSlow0);
		int iSlow55 = (31.0f < fSlow0);
		int iSlow56 = (35.0f < fSlow0);
		int iSlow57 = (37.0f < fSlow0);
		int iSlow58 = (39.0f < fSlow0);
		int iSlow59 = (41.0f < fSlow0);
		int iSlow60 = (43.0f < fSlow0);
		int iSlow61 = (45.0f < fSlow0);
		int iSlow62 = (47.0f < fSlow0);
		int iSlow63 = (51.0f < fSlow0);
		int iSlow64 = (55.0f < fSlow0);
		int iSlow65 = (57.0f < fSlow0);
		int iSlow66 = (59.0f < fSlow0);
		int iSlow67 = (61.0f < fSlow0);
		int iSlow68 = (63.0f < fSlow0);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			int iTemp0 = (1 - iVec0[1]);
			int iTemp1 = (1103515245 * (iRec2[1] + 12345));
			int iTemp2 = (1103515245 * (iTemp1 + 12345));
			int iTemp3 = (1103515245 * (iTemp2 + 12345));
			int iTemp4 = (1103515245 * (iTemp3 + 12345));
			int iTemp5 = (1103515245 * (iTemp4 + 12345));
			int iTemp6 = (1103515245 * (iTemp5 + 12345));
			int iTemp7 = (1103515245 * (iTemp6 + 12345));
			int iTemp8 = (1103515245 * (iTemp7 + 12345));
			int iTemp9 = (1103515245 * (iTemp8 + 12345));
			int iTemp10 = (1103515245 * (iTemp9 + 12345));
			int iTemp11 = (1103515245 * (iTemp10 + 12345));
			int iTemp12 = (1103515245 * (iTemp11 + 12345));
			int iTemp13 = (1103515245 * (iTemp12 + 12345));
			int iTemp14 = (1103515245 * (iTemp13 + 12345));
			int iTemp15 = (1103515245 * (iTemp14 + 12345));
			int iTemp16 = (1103515245 * (iTemp15 + 12345));
			int iTemp17 = (1103515245 * (iTemp16 + 12345));
			int iTemp18 = (1103515245 * (iTemp17 + 12345));
			int iTemp19 = (1103515245 * (iTemp18 + 12345));
			int iTemp20 = (1103515245 * (iTemp19 + 12345));
			int iTemp21 = (1103515245 * (iTemp20 + 12345));
			int iTemp22 = (1103515245 * (iTemp21 + 12345));
			int iTemp23 = (1103515245 * (iTemp22 + 12345));
			int iTemp24 = (1103515245 * (iTemp23 + 12345));
			int iTemp25 = (1103515245 * (iTemp24 + 12345));
			int iTemp26 = (1103515245 * (iTemp25 + 12345));
			int iTemp27 = (1103515245 * (iTemp26 + 12345));
			int iTemp28 = (1103515245 * (iTemp27 + 12345));
			int iTemp29 = (1103515245 * (iTemp28 + 12345));
			int iTemp30 = (1103515245 * (iTemp29 + 12345));
			int iTemp31 = (1103515245 * (iTemp30 + 12345));
			int iTemp32 = (1103515245 * (iTemp31 + 12345));
			int iTemp33 = (1103515245 * (iTemp32 + 12345));
			int iTemp34 = (1103515245 * (iTemp33 + 12345));
			int iTemp35 = (1103515245 * (iTemp34 + 12345));
			int iTemp36 = (1103515245 * (iTemp35 + 12345));
			int iTemp37 = (1103515245 * (iTemp36 + 12345));
			int iTemp38 = (1103515245 * (iTemp37 + 12345));
			int iTemp39 = (1103515245 * (iTemp38 + 12345));
			int iTemp40 = (1103515245 * (iTemp39 + 12345));
			int iTemp41 = (1103515245 * (iTemp40 + 12345));
			int iTemp42 = (1103515245 * (iTemp41 + 12345));
			int iTemp43 = (1103515245 * (iTemp42 + 12345));
			int iTemp44 = (1103515245 * (iTemp43 + 12345));
			int iTemp45 = (1103515245 * (iTemp44 + 12345));
			int iTemp46 = (1103515245 * (iTemp45 + 12345));
			int iTemp47 = (1103515245 * (iTemp46 + 12345));
			int iTemp48 = (1103515245 * (iTemp47 + 12345));
			int iTemp49 = (1103515245 * (iTemp48 + 12345));
			int iTemp50 = (1103515245 * (iTemp49 + 12345));
			int iTemp51 = (1103515245 * (iTemp50 + 12345));
			int iTemp52 = (1103515245 * (iTemp51 + 12345));
			int iTemp53 = (1103515245 * (iTemp52 + 12345));
			int iTemp54 = (1103515245 * (iTemp53 + 12345));
			int iTemp55 = (1103515245 * (iTemp54 + 12345));
			int iTemp56 = (1103515245 * (iTemp55 + 12345));
			int iTemp57 = (1103515245 * (iTemp56 + 12345));
			int iTemp58 = (1103515245 * (iTemp57 + 12345));
			int iTemp59 = (1103515245 * (iTemp58 + 12345));
			int iTemp60 = (1103515245 * (iTemp59 + 12345));
			int iTemp61 = (1103515245 * (iTemp60 + 12345));
			int iTemp62 = (1103515245 * (iTemp61 + 12345));
			int iTemp63 = (1103515245 * (iTemp62 + 12345));
			int iTemp64 = (1103515245 * (iTemp63 + 12345));
			iRec2[0] = (1103515245 * (iTemp64 + 12345));
			int iRec3 = iTemp64;
			int iRec4 = iTemp63;
			int iRec5 = iTemp62;
			int iRec6 = iTemp61;
			int iRec7 = iTemp60;
			int iRec8 = iTemp59;
			int iRec9 = iTemp58;
			int iRec10 = iTemp57;
			int iRec11 = iTemp56;
			int iRec12 = iTemp55;
			int iRec13 = iTemp54;
			int iRec14 = iTemp53;
			int iRec15 = iTemp52;
			int iRec16 = iTemp51;
			int iRec17 = iTemp50;
			int iRec18 = iTemp49;
			int iRec19 = iTemp48;
			int iRec20 = iTemp47;
			int iRec21 = iTemp46;
			int iRec22 = iTemp45;
			int iRec23 = iTemp44;
			int iRec24 = iTemp43;
			int iRec25 = iTemp42;
			int iRec26 = iTemp41;
			int iRec27 = iTemp40;
			int iRec28 = iTemp39;
			int iRec29 = iTemp38;
			int iRec30 = iTemp37;
			int iRec31 = iTemp36;
			int iRec32 = iTemp35;
			int iRec33 = iTemp34;
			int iRec34 = iTemp33;
			int iRec35 = iTemp32;
			int iRec36 = iTemp31;
			int iRec37 = iTemp30;
			int iRec38 = iTemp29;
			int iRec39 = iTemp28;
			int iRec40 = iTemp27;
			int iRec41 = iTemp26;
			int iRec42 = iTemp25;
			int iRec43 = iTemp24;
			int iRec44 = iTemp23;
			int iRec45 = iTemp22;
			int iRec46 = iTemp21;
			int iRec47 = iTemp20;
			int iRec48 = iTemp19;
			int iRec49 = iTemp18;
			int iRec50 = iTemp17;
			int iRec51 = iTemp16;
			int iRec52 = iTemp15;
			int iRec53 = iTemp14;
			int iRec54 = iTemp13;
			int iRec55 = iTemp12;
			int iRec56 = iTemp11;
			int iRec57 = iTemp10;
			int iRec58 = iTemp9;
			int iRec59 = iTemp8;
			int iRec60 = iTemp7;
			int iRec61 = iTemp6;
			int iRec62 = iTemp5;
			int iRec63 = iTemp4;
			int iRec64 = iTemp3;
			int iRec65 = iTemp2;
			int iTemp65 = int((fSlow4 * ((2.32830644e-10f * float(iRec60)) + 1.0f)));
			iRec1[0] = ((iVec0[1] * iRec1[1]) + (iTemp0 * iTemp65));
			fRec67[0] = (fSlow5 + (0.999000013f * fRec67[1]));
			int iTemp66 = int((fConst0 * fRec67[0]));
			iRec66[0] = ((iRec66[1] + 1) % iTemp66);
			int iTemp67 = ((iRec1[0] + iRec66[0]) % iTemp66);
			int iTemp68 = (iTemp66 + -1);
			float fTemp69 = (float(iTemp67) / float(iTemp68));
			iRec68[0] = ((iRec68[1] + 1) % iSlow3);
			ftbl1[(iRec68[0] % iSlow3)] = float(input0[i]);
			int iTemp70 = int(fTemp69);
			iRec69[0] = ((iTemp65 * iTemp70) + (iRec69[1] * (1 - iTemp70)));
			int iTemp71 = int((fSlow4 * ((2.32830644e-10f * float(iRec48)) + 1.0f)));
			iRec70[0] = ((iVec0[1] * iRec70[1]) + (iTemp0 * iTemp71));
			int iTemp72 = ((iRec70[0] + iRec66[0]) % iTemp66);
			float fTemp73 = (float(iTemp72) / float(iTemp68));
			int iTemp74 = int(fTemp73);
			iRec71[0] = ((iTemp71 * iTemp74) + (iRec71[1] * (1 - iTemp74)));
			int iTemp75 = int((fSlow4 * ((2.32830644e-10f * float(iRec42)) + 1.0f)));
			iRec73[0] = ((iVec0[1] * iRec73[1]) + (iTemp0 * iTemp75));
			int iTemp76 = ((iRec73[0] + iRec66[0]) % iTemp66);
			float fTemp77 = (float(iTemp76) / float(iTemp68));
			int iTemp78 = int(fTemp77);
			iRec72[0] = ((iRec72[1] * (1 - iTemp78)) + (iTemp75 * iTemp78));
			int iTemp79 = int((fSlow4 * ((2.32830644e-10f * float(iRec20)) + 1.0f)));
			iRec74[0] = ((iVec0[1] * iRec74[1]) + (iTemp0 * iTemp79));
			int iTemp80 = ((iRec74[0] + iRec66[0]) % iTemp66);
			float fTemp81 = (float(iTemp80) / float(iTemp68));
			int iTemp82 = int(fTemp81);
			iRec75[0] = (((1 - iTemp82) * iRec75[1]) + (iTemp82 * iTemp79));
			int iTemp83 = int((fSlow4 * ((2.32830644e-10f * float(iRec6)) + 1.0f)));
			iRec76[0] = ((iVec0[1] * iRec76[1]) + (iTemp0 * iTemp83));
			int iTemp84 = ((iRec66[0] + iRec76[0]) % iTemp66);
			float fTemp85 = (float(iTemp84) / float(iTemp68));
			int iTemp86 = int(fTemp85);
			iRec77[0] = ((iRec77[1] * (1 - iTemp86)) + (iTemp83 * iTemp86));
			int iTemp87 = int((fSlow4 * ((2.32830644e-10f * float(iRec4)) + 1.0f)));
			iRec78[0] = ((iVec0[1] * iRec78[1]) + (iTemp0 * iTemp87));
			int iTemp88 = ((iRec66[0] + iRec78[0]) % iTemp66);
			float fTemp89 = (float(iTemp88) / float(iTemp68));
			int iTemp90 = int(fTemp89);
			iRec79[0] = ((iRec79[1] * (1 - iTemp90)) + (iTemp87 * iTemp90));
			int iTemp91 = int((fSlow4 * ((2.32830644e-10f * float(iRec2[0])) + 1.0f)));
			iRec80[0] = ((iVec0[1] * iRec80[1]) + (iTemp0 * iTemp91));
			int iTemp92 = ((iRec66[0] + iRec80[0]) % iTemp66);
			float fTemp93 = (float(iTemp92) / float(iTemp68));
			int iTemp94 = int(fTemp93);
			iRec81[0] = ((iTemp91 * iTemp94) + (iRec81[1] * (1 - iTemp94)));
			int iTemp95 = int((fSlow4 * ((2.32830644e-10f * float(iRec8)) + 1.0f)));
			iRec82[0] = ((iVec0[1] * iRec82[1]) + (iTemp0 * iTemp95));
			int iTemp96 = ((iRec66[0] + iRec82[0]) % iTemp66);
			float fTemp97 = (float(iTemp96) / float(iTemp68));
			int iTemp98 = int(fTemp97);
			iRec83[0] = ((iRec83[1] * (1 - iTemp98)) + (iTemp95 * iTemp98));
			int iTemp99 = int((fSlow4 * ((2.32830644e-10f * float(iRec10)) + 1.0f)));
			iRec84[0] = ((iVec0[1] * iRec84[1]) + (iTemp0 * iTemp99));
			int iTemp100 = ((iRec84[0] + iRec66[0]) % iTemp66);
			float fTemp101 = (float(iTemp100) / float(iTemp68));
			int iTemp102 = int(fTemp101);
			iRec85[0] = ((iRec85[1] * (1 - iTemp102)) + (iTemp99 * iTemp102));
			int iTemp103 = int((fSlow4 * ((2.32830644e-10f * float(iRec12)) + 1.0f)));
			iRec86[0] = ((iVec0[1] * iRec86[1]) + (iTemp0 * iTemp103));
			int iTemp104 = ((iRec86[0] + iRec66[0]) % iTemp66);
			float fTemp105 = (float(iTemp104) / float(iTemp68));
			int iTemp106 = int(fTemp105);
			iRec87[0] = ((iRec87[1] * (1 - iTemp106)) + (iTemp103 * iTemp106));
			int iTemp107 = int((fSlow4 * ((2.32830644e-10f * float(iRec14)) + 1.0f)));
			iRec88[0] = ((iVec0[1] * iRec88[1]) + (iTemp0 * iTemp107));
			int iTemp108 = ((iRec66[0] + iRec88[0]) % iTemp66);
			float fTemp109 = (float(iTemp108) / float(iTemp68));
			int iTemp110 = int(fTemp109);
			iRec89[0] = ((iRec89[1] * (1 - iTemp110)) + (iTemp107 * iTemp110));
			int iTemp111 = int((fSlow4 * ((2.32830644e-10f * float(iRec16)) + 1.0f)));
			iRec90[0] = ((iVec0[1] * iRec90[1]) + (iTemp0 * iTemp111));
			int iTemp112 = ((iRec66[0] + iRec90[0]) % iTemp66);
			float fTemp113 = (float(iTemp112) / float(iTemp68));
			int iTemp114 = int(fTemp113);
			iRec91[0] = ((iRec91[1] * (1 - iTemp114)) + (iTemp111 * iTemp114));
			int iTemp115 = int((fSlow4 * ((2.32830644e-10f * float(iRec18)) + 1.0f)));
			iRec92[0] = ((iVec0[1] * iRec92[1]) + (iTemp0 * iTemp115));
			int iTemp116 = ((iRec66[0] + iRec92[0]) % iTemp66);
			float fTemp117 = (float(iTemp116) / float(iTemp68));
			int iTemp118 = int(fTemp117);
			iRec93[0] = ((iRec93[1] * (1 - iTemp118)) + (iTemp115 * iTemp118));
			int iTemp119 = int((fSlow4 * ((2.32830644e-10f * float(iRec22)) + 1.0f)));
			iRec95[0] = ((iVec0[1] * iRec95[1]) + (iTemp0 * iTemp119));
			int iTemp120 = ((iRec95[0] + iRec66[0]) % iTemp66);
			float fTemp121 = (float(iTemp120) / float(iTemp68));
			int iTemp122 = int(fTemp121);
			iRec94[0] = ((iRec94[1] * (1 - iTemp122)) + (iTemp119 * iTemp122));
			int iTemp123 = int((fSlow4 * ((2.32830644e-10f * float(iRec24)) + 1.0f)));
			iRec97[0] = ((iVec0[1] * iRec97[1]) + (iTemp0 * iTemp123));
			int iTemp124 = ((iRec97[0] + iRec66[0]) % iTemp66);
			float fTemp125 = (float(iTemp124) / float(iTemp68));
			int iTemp126 = int(fTemp125);
			iRec96[0] = ((iRec96[1] * (1 - iTemp126)) + (iTemp123 * iTemp126));
			int iTemp127 = int((fSlow4 * ((2.32830644e-10f * float(iRec26)) + 1.0f)));
			iRec99[0] = ((iVec0[1] * iRec99[1]) + (iTemp0 * iTemp127));
			int iTemp128 = ((iRec99[0] + iRec66[0]) % iTemp66);
			float fTemp129 = (float(iTemp128) / float(iTemp68));
			int iTemp130 = int(fTemp129);
			iRec98[0] = ((iRec98[1] * (1 - iTemp130)) + (iTemp127 * iTemp130));
			int iTemp131 = int((fSlow4 * ((2.32830644e-10f * float(iRec28)) + 1.0f)));
			iRec101[0] = ((iVec0[1] * iRec101[1]) + (iTemp0 * iTemp131));
			int iTemp132 = ((iRec101[0] + iRec66[0]) % iTemp66);
			float fTemp133 = (float(iTemp132) / float(iTemp68));
			int iTemp134 = int(fTemp133);
			iRec100[0] = ((iRec100[1] * (1 - iTemp134)) + (iTemp131 * iTemp134));
			int iTemp135 = int((fSlow4 * ((2.32830644e-10f * float(iRec30)) + 1.0f)));
			iRec103[0] = ((iVec0[1] * iRec103[1]) + (iTemp0 * iTemp135));
			int iTemp136 = ((iRec103[0] + iRec66[0]) % iTemp66);
			float fTemp137 = (float(iTemp136) / float(iTemp68));
			int iTemp138 = int(fTemp137);
			iRec102[0] = ((iRec102[1] * (1 - iTemp138)) + (iTemp135 * iTemp138));
			int iTemp139 = int((fSlow4 * ((2.32830644e-10f * float(iRec32)) + 1.0f)));
			iRec105[0] = ((iVec0[1] * iRec105[1]) + (iTemp0 * iTemp139));
			int iTemp140 = ((iRec105[0] + iRec66[0]) % iTemp66);
			float fTemp141 = (float(iTemp140) / float(iTemp68));
			int iTemp142 = int(fTemp141);
			iRec104[0] = ((iTemp139 * iTemp142) + (iRec104[1] * (1 - iTemp142)));
			int iTemp143 = int((fSlow4 * ((2.32830644e-10f * float(iRec34)) + 1.0f)));
			iRec106[0] = ((iVec0[1] * iRec106[1]) + (iTemp0 * iTemp143));
			int iTemp144 = ((iRec66[0] + iRec106[0]) % iTemp66);
			float fTemp145 = (float(iTemp144) / float(iTemp68));
			int iTemp146 = int(fTemp145);
			iRec107[0] = ((iRec107[1] * (1 - iTemp146)) + (iTemp143 * iTemp146));
			int iTemp147 = int((fSlow4 * ((2.32830644e-10f * float(iRec36)) + 1.0f)));
			iRec108[0] = ((iVec0[1] * iRec108[1]) + (iTemp0 * iTemp147));
			int iTemp148 = ((iRec66[0] + iRec108[0]) % iTemp66);
			float fTemp149 = (float(iTemp148) / float(iTemp68));
			int iTemp150 = int(fTemp149);
			iRec109[0] = ((iRec109[1] * (1 - iTemp150)) + (iTemp147 * iTemp150));
			int iTemp151 = int((fSlow4 * ((2.32830644e-10f * float(iRec38)) + 1.0f)));
			iRec110[0] = ((iVec0[1] * iRec110[1]) + (iTemp0 * iTemp151));
			int iTemp152 = ((iRec66[0] + iRec110[0]) % iTemp66);
			float fTemp153 = (float(iTemp152) / float(iTemp68));
			int iTemp154 = int(fTemp153);
			iRec111[0] = ((iRec111[1] * (1 - iTemp154)) + (iTemp151 * iTemp154));
			int iTemp155 = int((fSlow4 * ((2.32830644e-10f * float(iRec40)) + 1.0f)));
			iRec113[0] = ((iTemp0 * iTemp155) + (iVec0[1] * iRec113[1]));
			int iTemp156 = ((iRec113[0] + iRec66[0]) % iTemp66);
			float fTemp157 = (float(iTemp156) / float(iTemp68));
			int iTemp158 = int(fTemp157);
			iRec112[0] = ((iRec112[1] * (1 - iTemp158)) + (iTemp155 * iTemp158));
			int iTemp159 = int((fSlow4 * ((2.32830644e-10f * float(iRec44)) + 1.0f)));
			iRec114[0] = ((iVec0[1] * iRec114[1]) + (iTemp0 * iTemp159));
			int iTemp160 = ((iRec114[0] + iRec66[0]) % iTemp66);
			float fTemp161 = (float(iTemp160) / float(iTemp68));
			int iTemp162 = int(fTemp161);
			iRec115[0] = ((iRec115[1] * (1 - iTemp162)) + (iTemp159 * iTemp162));
			int iTemp163 = int((fSlow4 * ((2.32830644e-10f * float(iRec46)) + 1.0f)));
			iRec116[0] = ((iVec0[1] * iRec116[1]) + (iTemp0 * iTemp163));
			int iTemp164 = ((iRec116[0] + iRec66[0]) % iTemp66);
			float fTemp165 = (float(iTemp164) / float(iTemp68));
			int iTemp166 = int(fTemp165);
			iRec117[0] = (((1 - iTemp166) * iRec117[1]) + (iTemp166 * iTemp163));
			int iTemp167 = int((fSlow4 * ((2.32830644e-10f * float(iRec50)) + 1.0f)));
			iRec118[0] = ((iVec0[1] * iRec118[1]) + (iTemp0 * iTemp167));
			int iTemp168 = ((iRec118[0] + iRec66[0]) % iTemp66);
			float fTemp169 = (float(iTemp168) / float(iTemp68));
			int iTemp170 = int(fTemp169);
			iRec119[0] = (((1 - iTemp170) * iRec119[1]) + (iTemp170 * iTemp167));
			int iTemp171 = int((fSlow4 * ((2.32830644e-10f * float(iRec52)) + 1.0f)));
			iRec120[0] = ((iVec0[1] * iRec120[1]) + (iTemp0 * iTemp171));
			int iTemp172 = ((iRec120[0] + iRec66[0]) % iTemp66);
			float fTemp173 = (float(iTemp172) / float(iTemp68));
			int iTemp174 = int(fTemp173);
			iRec121[0] = (((1 - iTemp174) * iRec121[1]) + (iTemp174 * iTemp171));
			int iTemp175 = int((fSlow4 * ((2.32830644e-10f * float(iRec54)) + 1.0f)));
			iRec122[0] = ((iVec0[1] * iRec122[1]) + (iTemp0 * iTemp175));
			int iTemp176 = ((iRec66[0] + iRec122[0]) % iTemp66);
			float fTemp177 = (float(iTemp176) / float(iTemp68));
			int iTemp178 = int(fTemp177);
			iRec123[0] = ((iRec123[1] * (1 - iTemp178)) + (iTemp175 * iTemp178));
			int iTemp179 = int((fSlow4 * ((2.32830644e-10f * float(iRec56)) + 1.0f)));
			iRec124[0] = ((iVec0[1] * iRec124[1]) + (iTemp0 * iTemp179));
			int iTemp180 = ((iRec124[0] + iRec66[0]) % iTemp66);
			float fTemp181 = (float(iTemp180) / float(iTemp68));
			int iTemp182 = int(fTemp181);
			iRec125[0] = ((iTemp179 * iTemp182) + (iRec125[1] * (1 - iTemp182)));
			int iTemp183 = int((fSlow4 * ((2.32830644e-10f * float(iRec58)) + 1.0f)));
			iRec126[0] = ((iVec0[1] * iRec126[1]) + (iTemp0 * iTemp183));
			int iTemp184 = ((iRec126[0] + iRec66[0]) % iTemp66);
			float fTemp185 = (float(iTemp184) / float(iTemp68));
			int iTemp186 = int(fTemp185);
			iRec127[0] = ((iRec127[1] * (1 - iTemp186)) + (iTemp183 * iTemp186));
			int iTemp187 = int((fSlow4 * ((2.32830644e-10f * float(iRec62)) + 1.0f)));
			iRec129[0] = ((iVec0[1] * iRec129[1]) + (iTemp0 * iTemp187));
			int iTemp188 = ((iRec129[0] + iRec66[0]) % iTemp66);
			float fTemp189 = (float(iTemp188) / float(iTemp68));
			int iTemp190 = int(fTemp189);
			iRec128[0] = (((1 - iTemp190) * iRec128[1]) + (iTemp187 * iTemp190));
			int iTemp191 = int((fSlow4 * ((2.32830644e-10f * float(iRec64)) + 1.0f)));
			iRec130[0] = ((iVec0[1] * iRec130[1]) + (iTemp0 * iTemp191));
			int iTemp192 = ((iRec130[0] + iRec66[0]) % iTemp66);
			float fTemp193 = (float(iTemp192) / float(iTemp68));
			int iTemp194 = int(fTemp193);
			iRec131[0] = ((iTemp191 * iTemp194) + (iRec131[1] * (1 - iTemp194)));
			output0[i] = FAUSTFLOAT((fSlow1 * ((((float(iSlow2) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp69))] * ftbl1[((iRec69[0] + iTemp67) % iSlow3)])) + (((((((float(iSlow6) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp73))] * ftbl1[((iRec71[0] + iTemp72) % iSlow3)])) + ((((float(iSlow7) * (ftbl1[((iRec72[0] + iTemp76) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp77))])) + ((((((((((((float(iSlow8) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp81))] * ftbl1[((iRec75[0] + iTemp80) % iSlow3)])) + ((((((((float(iSlow9) * (ftbl1[((iTemp84 + iRec77[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp85))])) + ((float(iSlow10) * (ftbl1[((iTemp88 + iRec79[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp89))])) + (float(iSlow11) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp93))] * ftbl1[((iTemp92 + iRec81[0]) % iSlow3)])))) + (float(iSlow12) * (ftbl1[((iTemp96 + iRec83[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp97))]))) + (float(iSlow13) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp101))] * ftbl1[((iRec85[0] + iTemp100) % iSlow3)]))) + (float(iSlow14) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp105))] * ftbl1[((iRec87[0] + iTemp104) % iSlow3)]))) + (float(iSlow15) * (ftbl1[((iTemp108 + iRec89[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp109))]))) + (float(iSlow16) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp113))] * ftbl1[((iTemp112 + iRec91[0]) % iSlow3)]))) + (float(iSlow17) * (ftbl1[((iTemp116 + iRec93[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp117))])))) + (float(iSlow18) * (ftbl1[((iRec94[0] + iTemp120) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp121))]))) + (float(iSlow19) * (ftbl1[((iRec96[0] + iTemp124) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp125))]))) + (float(iSlow20) * (ftbl1[((iRec98[0] + iTemp128) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp129))]))) + (float(iSlow21) * (ftbl1[((iRec100[0] + iTemp132) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp133))]))) + (float(iSlow22) * (ftbl1[((iRec102[0] + iTemp136) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp137))]))) + (float(iSlow23) * (ftbl1[((iRec104[0] + iTemp140) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp141))]))) + (float(iSlow24) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp145))] * ftbl1[((iTemp144 + iRec107[0]) % iSlow3)]))) + (float(iSlow25) * (ftbl1[((iTemp148 + iRec109[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp149))]))) + (float(iSlow26) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp153))] * ftbl1[((iTemp152 + iRec111[0]) % iSlow3)]))) + (float(iSlow27) * (ftbl1[((iRec112[0] + iTemp156) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp157))])))) + (float(iSlow28) * (ftbl1[((iTemp160 + iRec115[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp161))]))) + (float(iSlow29) * (ftbl1[((iTemp164 + iRec117[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp165))])))) + (float(iSlow30) * (ftbl1[((iTemp168 + iRec119[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp169))]))) + (float(iSlow31) * (ftbl1[((iTemp172 + iRec121[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp173))]))) + (float(iSlow32) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp177))] * ftbl1[((iRec123[0] + iTemp176) % iSlow3)]))) + (float(iSlow33) * (ftbl1[((iTemp180 + iRec125[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp181))]))) + (float(iSlow34) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp185))] * ftbl1[((iRec127[0] + iTemp184) % iSlow3)])))) + (float(iSlow35) * (ftbl1[((iRec128[0] + iTemp188) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp189))]))) + (float(iSlow36) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp193))] * ftbl1[((iTemp192 + iRec131[0]) % iSlow3)])))));
			int iTemp195 = int((fSlow4 * ((2.32830644e-10f * float(iRec55)) + 1.0f)));
			iRec132[0] = ((iVec0[1] * iRec132[1]) + (iTemp0 * iTemp195));
			int iTemp196 = ((iRec132[0] + iRec66[0]) % iTemp66);
			float fTemp197 = (float(iTemp196) / float(iTemp68));
			int iTemp198 = int(fTemp197);
			iRec133[0] = (((1 - iTemp198) * iRec133[1]) + (iTemp198 * iTemp195));
			int iTemp199 = int((fSlow4 * ((2.32830644e-10f * float(iRec51)) + 1.0f)));
			iRec135[0] = ((iVec0[1] * iRec135[1]) + (iTemp0 * iTemp199));
			int iTemp200 = ((iRec135[0] + iRec66[0]) % iTemp66);
			float fTemp201 = (float(iTemp200) / float(iTemp68));
			int iTemp202 = int(fTemp201);
			iRec134[0] = ((iTemp199 * iTemp202) + (iRec134[1] * (1 - iTemp202)));
			int iTemp203 = int((fSlow4 * ((2.32830644e-10f * float(iRec35)) + 1.0f)));
			iRec136[0] = ((iVec0[1] * iRec136[1]) + (iTemp0 * iTemp203));
			int iTemp204 = ((iRec66[0] + iRec136[0]) % iTemp66);
			float fTemp205 = (float(iTemp204) / float(iTemp68));
			int iTemp206 = int(fTemp205);
			iRec137[0] = ((iRec137[1] * (1 - iTemp206)) + (iTemp203 * iTemp206));
			int iTemp207 = int((fSlow4 * ((2.32830644e-10f * float(iRec31)) + 1.0f)));
			iRec138[0] = ((iVec0[1] * iRec138[1]) + (iTemp0 * iTemp207));
			int iTemp208 = ((iRec66[0] + iRec138[0]) % iTemp66);
			float fTemp209 = (float(iTemp208) / float(iTemp68));
			int iTemp210 = int(fTemp209);
			iRec139[0] = ((iTemp207 * iTemp210) + (iRec139[1] * (1 - iTemp210)));
			int iTemp211 = int((fSlow4 * ((2.32830644e-10f * float(iRec11)) + 1.0f)));
			iRec141[0] = ((iVec0[1] * iRec141[1]) + (iTemp0 * iTemp211));
			int iTemp212 = ((iRec141[0] + iRec66[0]) % iTemp66);
			float fTemp213 = (float(iTemp212) / float(iTemp68));
			int iTemp214 = int(fTemp213);
			iRec140[0] = ((iRec140[1] * (1 - iTemp214)) + (iTemp211 * iTemp214));
			int iTemp215 = int((fSlow4 * ((2.32830644e-10f * float(iRec3)) + 1.0f)));
			iRec143[0] = ((iVec0[1] * iRec143[1]) + (iTemp0 * iTemp215));
			int iTemp216 = ((iRec143[0] + iRec66[0]) % iTemp66);
			float fTemp217 = (float(iTemp216) / float(iTemp68));
			int iTemp218 = int(fTemp217);
			iRec142[0] = ((iRec142[1] * (1 - iTemp218)) + (iTemp215 * iTemp218));
			int iTemp219 = int((fSlow4 * ((2.32830644e-10f * float(iRec5)) + 1.0f)));
			iRec144[0] = ((iVec0[1] * iRec144[1]) + (iTemp0 * iTemp219));
			int iTemp220 = ((iRec144[0] + iRec66[0]) % iTemp66);
			float fTemp221 = (float(iTemp220) / float(iTemp68));
			int iTemp222 = int(fTemp221);
			iRec145[0] = (((1 - iTemp222) * iRec145[1]) + (iTemp222 * iTemp219));
			int iTemp223 = int((fSlow4 * ((2.32830644e-10f * float(iRec7)) + 1.0f)));
			iRec146[0] = ((iVec0[1] * iRec146[1]) + (iTemp0 * iTemp223));
			int iTemp224 = ((iRec146[0] + iRec66[0]) % iTemp66);
			float fTemp225 = (float(iTemp224) / float(iTemp68));
			int iTemp226 = int(fTemp225);
			iRec147[0] = ((iRec147[1] * (1 - iTemp226)) + (iTemp223 * iTemp226));
			int iTemp227 = int((fSlow4 * ((2.32830644e-10f * float(iRec9)) + 1.0f)));
			iRec148[0] = ((iVec0[1] * iRec148[1]) + (iTemp0 * iTemp227));
			int iTemp228 = ((iRec66[0] + iRec148[0]) % iTemp66);
			float fTemp229 = (float(iTemp228) / float(iTemp68));
			int iTemp230 = int(fTemp229);
			iRec149[0] = ((iRec149[1] * (1 - iTemp230)) + (iTemp227 * iTemp230));
			int iTemp231 = int((fSlow4 * ((2.32830644e-10f * float(iRec13)) + 1.0f)));
			iRec151[0] = ((iVec0[1] * iRec151[1]) + (iTemp0 * iTemp231));
			int iTemp232 = ((iRec151[0] + iRec66[0]) % iTemp66);
			float fTemp233 = (float(iTemp232) / float(iTemp68));
			int iTemp234 = int(fTemp233);
			iRec150[0] = ((iRec150[1] * (1 - iTemp234)) + (iTemp231 * iTemp234));
			int iTemp235 = int((fSlow4 * ((2.32830644e-10f * float(iRec15)) + 1.0f)));
			iRec153[0] = ((iVec0[1] * iRec153[1]) + (iTemp0 * iTemp235));
			int iTemp236 = ((iRec153[0] + iRec66[0]) % iTemp66);
			float fTemp237 = (float(iTemp236) / float(iTemp68));
			int iTemp238 = int(fTemp237);
			iRec152[0] = ((iRec152[1] * (1 - iTemp238)) + (iTemp235 * iTemp238));
			int iTemp239 = int((fSlow4 * ((2.32830644e-10f * float(iRec17)) + 1.0f)));
			iRec154[0] = ((iVec0[1] * iRec154[1]) + (iTemp0 * iTemp239));
			int iTemp240 = ((iRec154[0] + iRec66[0]) % iTemp66);
			float fTemp241 = (float(iTemp240) / float(iTemp68));
			int iTemp242 = int(fTemp241);
			iRec155[0] = ((iRec155[1] * (1 - iTemp242)) + (iTemp239 * iTemp242));
			int iTemp243 = int((fSlow4 * ((2.32830644e-10f * float(iRec19)) + 1.0f)));
			iRec157[0] = ((iVec0[1] * iRec157[1]) + (iTemp0 * iTemp243));
			int iTemp244 = ((iRec157[0] + iRec66[0]) % iTemp66);
			float fTemp245 = (float(iTemp244) / float(iTemp68));
			int iTemp246 = int(fTemp245);
			iRec156[0] = ((iRec156[1] * (1 - iTemp246)) + (iTemp243 * iTemp246));
			int iTemp247 = int((fSlow4 * ((2.32830644e-10f * float(iRec21)) + 1.0f)));
			iRec159[0] = ((iVec0[1] * iRec159[1]) + (iTemp0 * iTemp247));
			int iTemp248 = ((iRec159[0] + iRec66[0]) % iTemp66);
			float fTemp249 = (float(iTemp248) / float(iTemp68));
			int iTemp250 = int(fTemp249);
			iRec158[0] = ((iTemp247 * iTemp250) + (iRec158[1] * (1 - iTemp250)));
			int iTemp251 = int((fSlow4 * ((2.32830644e-10f * float(iRec23)) + 1.0f)));
			iRec160[0] = ((iVec0[1] * iRec160[1]) + (iTemp0 * iTemp251));
			int iTemp252 = ((iRec160[0] + iRec66[0]) % iTemp66);
			float fTemp253 = (float(iTemp252) / float(iTemp68));
			int iTemp254 = int(fTemp253);
			iRec161[0] = (((1 - iTemp254) * iRec161[1]) + (iTemp251 * iTemp254));
			int iTemp255 = int((fSlow4 * ((2.32830644e-10f * float(iRec25)) + 1.0f)));
			iRec162[0] = ((iVec0[1] * iRec162[1]) + (iTemp0 * iTemp255));
			int iTemp256 = ((iRec66[0] + iRec162[0]) % iTemp66);
			float fTemp257 = (float(iTemp256) / float(iTemp68));
			int iTemp258 = int(fTemp257);
			iRec163[0] = ((iRec163[1] * (1 - iTemp258)) + (iTemp255 * iTemp258));
			int iTemp259 = int((fSlow4 * ((2.32830644e-10f * float(iRec27)) + 1.0f)));
			iRec164[0] = ((iVec0[1] * iRec164[1]) + (iTemp0 * iTemp259));
			int iTemp260 = ((iRec66[0] + iRec164[0]) % iTemp66);
			float fTemp261 = (float(iTemp260) / float(iTemp68));
			int iTemp262 = int(fTemp261);
			iRec165[0] = ((iRec165[1] * (1 - iTemp262)) + (iTemp259 * iTemp262));
			int iTemp263 = int((fSlow4 * ((2.32830644e-10f * float(iRec29)) + 1.0f)));
			iRec166[0] = ((iVec0[1] * iRec166[1]) + (iTemp0 * iTemp263));
			int iTemp264 = ((iRec66[0] + iRec166[0]) % iTemp66);
			float fTemp265 = (float(iTemp264) / float(iTemp68));
			int iTemp266 = int(fTemp265);
			iRec167[0] = ((iRec167[1] * (1 - iTemp266)) + (iTemp263 * iTemp266));
			int iTemp267 = int((fSlow4 * ((2.32830644e-10f * float(iRec33)) + 1.0f)));
			iRec168[0] = ((iVec0[1] * iRec168[1]) + (iTemp0 * iTemp267));
			int iTemp268 = ((iRec66[0] + iRec168[0]) % iTemp66);
			float fTemp269 = (float(iTemp268) / float(iTemp68));
			int iTemp270 = int(fTemp269);
			iRec169[0] = ((iRec169[1] * (1 - iTemp270)) + (iTemp267 * iTemp270));
			int iTemp271 = int((fSlow4 * ((2.32830644e-10f * float(iRec37)) + 1.0f)));
			iRec170[0] = ((iVec0[1] * iRec170[1]) + (iTemp0 * iTemp271));
			int iTemp272 = ((iRec66[0] + iRec170[0]) % iTemp66);
			float fTemp273 = (float(iTemp272) / float(iTemp68));
			int iTemp274 = int(fTemp273);
			iRec171[0] = ((iRec171[1] * (1 - iTemp274)) + (iTemp271 * iTemp274));
			int iTemp275 = int((fSlow4 * ((2.32830644e-10f * float(iRec39)) + 1.0f)));
			iRec172[0] = ((iVec0[1] * iRec172[1]) + (iTemp0 * iTemp275));
			int iTemp276 = ((iRec66[0] + iRec172[0]) % iTemp66);
			float fTemp277 = (float(iTemp276) / float(iTemp68));
			int iTemp278 = int(fTemp277);
			iRec173[0] = ((iRec173[1] * (1 - iTemp278)) + (iTemp275 * iTemp278));
			int iTemp279 = int((fSlow4 * ((2.32830644e-10f * float(iRec41)) + 1.0f)));
			iRec174[0] = ((iVec0[1] * iRec174[1]) + (iTemp0 * iTemp279));
			int iTemp280 = ((iRec66[0] + iRec174[0]) % iTemp66);
			float fTemp281 = (float(iTemp280) / float(iTemp68));
			int iTemp282 = int(fTemp281);
			iRec175[0] = ((iRec175[1] * (1 - iTemp282)) + (iTemp279 * iTemp282));
			int iTemp283 = int((fSlow4 * ((2.32830644e-10f * float(iRec43)) + 1.0f)));
			iRec176[0] = ((iVec0[1] * iRec176[1]) + (iTemp0 * iTemp283));
			int iTemp284 = ((iRec66[0] + iRec176[0]) % iTemp66);
			float fTemp285 = (float(iTemp284) / float(iTemp68));
			int iTemp286 = int(fTemp285);
			iRec177[0] = ((iRec177[1] * (1 - iTemp286)) + (iTemp283 * iTemp286));
			int iTemp287 = int((fSlow4 * ((2.32830644e-10f * float(iRec45)) + 1.0f)));
			iRec179[0] = ((iVec0[1] * iRec179[1]) + (iTemp0 * iTemp287));
			int iTemp288 = ((iRec179[0] + iRec66[0]) % iTemp66);
			float fTemp289 = (float(iTemp288) / float(iTemp68));
			int iTemp290 = int(fTemp289);
			iRec178[0] = ((iRec178[1] * (1 - iTemp290)) + (iTemp287 * iTemp290));
			int iTemp291 = int((fSlow4 * ((2.32830644e-10f * float(iRec47)) + 1.0f)));
			iRec180[0] = ((iVec0[1] * iRec180[1]) + (iTemp0 * iTemp291));
			int iTemp292 = ((iRec180[0] + iRec66[0]) % iTemp66);
			float fTemp293 = (float(iTemp292) / float(iTemp68));
			int iTemp294 = int(fTemp293);
			iRec181[0] = ((iRec181[1] * (1 - iTemp294)) + (iTemp291 * iTemp294));
			int iTemp295 = int((fSlow4 * ((2.32830644e-10f * float(iRec49)) + 1.0f)));
			iRec183[0] = ((iVec0[1] * iRec183[1]) + (iTemp0 * iTemp295));
			int iTemp296 = ((iRec183[0] + iRec66[0]) % iTemp66);
			float fTemp297 = (float(iTemp296) / float(iTemp68));
			int iTemp298 = int(fTemp297);
			iRec182[0] = ((iRec182[1] * (1 - iTemp298)) + (iTemp295 * iTemp298));
			int iTemp299 = int((fSlow4 * ((2.32830644e-10f * float(iRec53)) + 1.0f)));
			iRec185[0] = ((iVec0[1] * iRec185[1]) + (iTemp0 * iTemp299));
			int iTemp300 = ((iRec185[0] + iRec66[0]) % iTemp66);
			float fTemp301 = (float(iTemp300) / float(iTemp68));
			int iTemp302 = int(fTemp301);
			iRec184[0] = ((iRec184[1] * (1 - iTemp302)) + (iTemp299 * iTemp302));
			int iTemp303 = int((fSlow4 * ((2.32830644e-10f * float(iRec57)) + 1.0f)));
			iRec187[0] = ((iVec0[1] * iRec187[1]) + (iTemp0 * iTemp303));
			int iTemp304 = ((iRec187[0] + iRec66[0]) % iTemp66);
			float fTemp305 = (float(iTemp304) / float(iTemp68));
			int iTemp306 = int(fTemp305);
			iRec186[0] = ((iRec186[1] * (1 - iTemp306)) + (iTemp303 * iTemp306));
			int iTemp307 = int((fSlow4 * ((2.32830644e-10f * float(iRec59)) + 1.0f)));
			iRec189[0] = ((iVec0[1] * iRec189[1]) + (iTemp0 * iTemp307));
			int iTemp308 = ((iRec189[0] + iRec66[0]) % iTemp66);
			float fTemp309 = (float(iTemp308) / float(iTemp68));
			int iTemp310 = int(fTemp309);
			iRec188[0] = ((iRec188[1] * (1 - iTemp310)) + (iTemp307 * iTemp310));
			int iTemp311 = int((fSlow4 * ((2.32830644e-10f * float(iRec61)) + 1.0f)));
			iRec190[0] = ((iVec0[1] * iRec190[1]) + (iTemp0 * iTemp311));
			int iTemp312 = ((iRec66[0] + iRec190[0]) % iTemp66);
			float fTemp313 = (float(iTemp312) / float(iTemp68));
			int iTemp314 = int(fTemp313);
			iRec191[0] = ((iRec191[1] * (1 - iTemp314)) + (iTemp311 * iTemp314));
			int iTemp315 = int((fSlow4 * ((2.32830644e-10f * float(iRec63)) + 1.0f)));
			iRec192[0] = ((iVec0[1] * iRec192[1]) + (iTemp0 * iTemp315));
			int iTemp316 = ((iRec66[0] + iRec192[0]) % iTemp66);
			float fTemp317 = (float(iTemp316) / float(iTemp68));
			int iTemp318 = int(fTemp317);
			iRec193[0] = ((iRec193[1] * (1 - iTemp318)) + (iTemp315 * iTemp318));
			int iTemp319 = int((fSlow4 * ((2.32830644e-10f * float(iRec65)) + 1.0f)));
			iRec195[0] = ((iVec0[1] * iRec195[1]) + (iTemp0 * iTemp319));
			int iTemp320 = ((iRec195[0] + iRec66[0]) % iTemp66);
			float fTemp321 = (float(iTemp320) / float(iTemp68));
			int iTemp322 = int(fTemp321);
			iRec194[0] = ((iRec194[1] * (1 - iTemp322)) + (iTemp319 * iTemp322));
			output1[i] = FAUSTFLOAT((fSlow1 * (((((((float(iSlow37) * (ftbl1[((iTemp196 + iRec133[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp197))])) + (((float(iSlow38) * (ftbl1[((iRec134[0] + iTemp200) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp201))])) + (((((((((float(iSlow39) * (ftbl1[((iTemp204 + iRec137[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp205))])) + (((float(iSlow40) * (ftbl1[((iTemp208 + iRec139[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp209))])) + (((((((((((float(iSlow41) * (ftbl1[((iRec140[0] + iTemp212) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp213))])) + ((((float(iSlow42) * (ftbl1[((iRec142[0] + iTemp216) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp217))])) + (float(iSlow43) * (ftbl1[((iTemp220 + iRec145[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp221))]))) + (float(iSlow44) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp225))] * ftbl1[((iRec147[0] + iTemp224) % iSlow3)]))) + (float(iSlow45) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp229))] * ftbl1[((iRec149[0] + iTemp228) % iSlow3)])))) + (float(iSlow46) * (ftbl1[((iRec150[0] + iTemp232) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp233))]))) + (float(iSlow47) * (ftbl1[((iRec152[0] + iTemp236) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp237))]))) + (float(iSlow48) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp241))] * ftbl1[((iRec155[0] + iTemp240) % iSlow3)]))) + (float(iSlow49) * (ftbl1[((iRec156[0] + iTemp244) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp245))]))) + (float(iSlow50) * (ftbl1[((iRec158[0] + iTemp248) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp249))]))) + (float(iSlow51) * (ftbl1[((iTemp252 + iRec161[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp253))]))) + (float(iSlow52) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp257))] * ftbl1[((iTemp256 + iRec163[0]) % iSlow3)]))) + (float(iSlow53) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp261))] * ftbl1[((iTemp260 + iRec165[0]) % iSlow3)]))) + (float(iSlow54) * (ftbl1[((iTemp264 + iRec167[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp265))])))) + (float(iSlow55) * (ftbl1[((iTemp268 + iRec169[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp269))])))) + (float(iSlow56) * (ftbl1[((iTemp272 + iRec171[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp273))]))) + (float(iSlow57) * (ftbl1[((iTemp276 + iRec173[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp277))]))) + (float(iSlow58) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp281))] * ftbl1[((iTemp280 + iRec175[0]) % iSlow3)]))) + (float(iSlow59) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp285))] * ftbl1[((iTemp284 + iRec177[0]) % iSlow3)]))) + (float(iSlow60) * (ftbl1[((iRec178[0] + iTemp288) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp289))]))) + (float(iSlow61) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp293))] * ftbl1[((iRec181[0] + iTemp292) % iSlow3)]))) + (float(iSlow62) * (ftbl1[((iRec182[0] + iTemp296) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp297))])))) + (float(iSlow63) * (ftbl1[((iRec184[0] + iTemp300) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp301))])))) + (float(iSlow64) * (ftbl1[((iRec186[0] + iTemp304) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp305))]))) + (float(iSlow65) * (ftbl1[((iRec188[0] + iTemp308) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp309))]))) + (float(iSlow66) * (ftbl0fx_granulatorSIG0[int((1024.0f * fTemp313))] * ftbl1[((iTemp312 + iRec191[0]) % iSlow3)]))) + (float(iSlow67) * (ftbl1[((iTemp316 + iRec193[0]) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp317))]))) + (float(iSlow68) * (ftbl1[((iRec194[0] + iTemp320) % iSlow3)] * ftbl0fx_granulatorSIG0[int((1024.0f * fTemp321))])))));
			iVec0[1] = iVec0[0];
			iRec2[1] = iRec2[0];
			iRec1[1] = iRec1[0];
			fRec67[1] = fRec67[0];
			iRec66[1] = iRec66[0];
			iRec68[1] = iRec68[0];
			iRec69[1] = iRec69[0];
			iRec70[1] = iRec70[0];
			iRec71[1] = iRec71[0];
			iRec73[1] = iRec73[0];
			iRec72[1] = iRec72[0];
			iRec74[1] = iRec74[0];
			iRec75[1] = iRec75[0];
			iRec76[1] = iRec76[0];
			iRec77[1] = iRec77[0];
			iRec78[1] = iRec78[0];
			iRec79[1] = iRec79[0];
			iRec80[1] = iRec80[0];
			iRec81[1] = iRec81[0];
			iRec82[1] = iRec82[0];
			iRec83[1] = iRec83[0];
			iRec84[1] = iRec84[0];
			iRec85[1] = iRec85[0];
			iRec86[1] = iRec86[0];
			iRec87[1] = iRec87[0];
			iRec88[1] = iRec88[0];
			iRec89[1] = iRec89[0];
			iRec90[1] = iRec90[0];
			iRec91[1] = iRec91[0];
			iRec92[1] = iRec92[0];
			iRec93[1] = iRec93[0];
			iRec95[1] = iRec95[0];
			iRec94[1] = iRec94[0];
			iRec97[1] = iRec97[0];
			iRec96[1] = iRec96[0];
			iRec99[1] = iRec99[0];
			iRec98[1] = iRec98[0];
			iRec101[1] = iRec101[0];
			iRec100[1] = iRec100[0];
			iRec103[1] = iRec103[0];
			iRec102[1] = iRec102[0];
			iRec105[1] = iRec105[0];
			iRec104[1] = iRec104[0];
			iRec106[1] = iRec106[0];
			iRec107[1] = iRec107[0];
			iRec108[1] = iRec108[0];
			iRec109[1] = iRec109[0];
			iRec110[1] = iRec110[0];
			iRec111[1] = iRec111[0];
			iRec113[1] = iRec113[0];
			iRec112[1] = iRec112[0];
			iRec114[1] = iRec114[0];
			iRec115[1] = iRec115[0];
			iRec116[1] = iRec116[0];
			iRec117[1] = iRec117[0];
			iRec118[1] = iRec118[0];
			iRec119[1] = iRec119[0];
			iRec120[1] = iRec120[0];
			iRec121[1] = iRec121[0];
			iRec122[1] = iRec122[0];
			iRec123[1] = iRec123[0];
			iRec124[1] = iRec124[0];
			iRec125[1] = iRec125[0];
			iRec126[1] = iRec126[0];
			iRec127[1] = iRec127[0];
			iRec129[1] = iRec129[0];
			iRec128[1] = iRec128[0];
			iRec130[1] = iRec130[0];
			iRec131[1] = iRec131[0];
			iRec132[1] = iRec132[0];
			iRec133[1] = iRec133[0];
			iRec135[1] = iRec135[0];
			iRec134[1] = iRec134[0];
			iRec136[1] = iRec136[0];
			iRec137[1] = iRec137[0];
			iRec138[1] = iRec138[0];
			iRec139[1] = iRec139[0];
			iRec141[1] = iRec141[0];
			iRec140[1] = iRec140[0];
			iRec143[1] = iRec143[0];
			iRec142[1] = iRec142[0];
			iRec144[1] = iRec144[0];
			iRec145[1] = iRec145[0];
			iRec146[1] = iRec146[0];
			iRec147[1] = iRec147[0];
			iRec148[1] = iRec148[0];
			iRec149[1] = iRec149[0];
			iRec151[1] = iRec151[0];
			iRec150[1] = iRec150[0];
			iRec153[1] = iRec153[0];
			iRec152[1] = iRec152[0];
			iRec154[1] = iRec154[0];
			iRec155[1] = iRec155[0];
			iRec157[1] = iRec157[0];
			iRec156[1] = iRec156[0];
			iRec159[1] = iRec159[0];
			iRec158[1] = iRec158[0];
			iRec160[1] = iRec160[0];
			iRec161[1] = iRec161[0];
			iRec162[1] = iRec162[0];
			iRec163[1] = iRec163[0];
			iRec164[1] = iRec164[0];
			iRec165[1] = iRec165[0];
			iRec166[1] = iRec166[0];
			iRec167[1] = iRec167[0];
			iRec168[1] = iRec168[0];
			iRec169[1] = iRec169[0];
			iRec170[1] = iRec170[0];
			iRec171[1] = iRec171[0];
			iRec172[1] = iRec172[0];
			iRec173[1] = iRec173[0];
			iRec174[1] = iRec174[0];
			iRec175[1] = iRec175[0];
			iRec176[1] = iRec176[0];
			iRec177[1] = iRec177[0];
			iRec179[1] = iRec179[0];
			iRec178[1] = iRec178[0];
			iRec180[1] = iRec180[0];
			iRec181[1] = iRec181[0];
			iRec183[1] = iRec183[0];
			iRec182[1] = iRec182[0];
			iRec185[1] = iRec185[0];
			iRec184[1] = iRec184[0];
			iRec187[1] = iRec187[0];
			iRec186[1] = iRec186[0];
			iRec189[1] = iRec189[0];
			iRec188[1] = iRec188[0];
			iRec190[1] = iRec190[0];
			iRec191[1] = iRec191[0];
			iRec192[1] = iRec192[0];
			iRec193[1] = iRec193[0];
			iRec195[1] = iRec195[0];
			iRec194[1] = iRec194[0];
			
		}
		
	}

	
};
// clang-format on
#endif

    template <class T>
    struct _fx_granulator_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_granulator_UI<T>::name(sym(fx_granulator));

typedef _fx_granulator_UI<fx_granulator> fx_granulator_UI;

class faust_fx_granulator_tilde : public FaustExternal<fx_granulator, fx_granulator_UI> {
public:
    faust_fx_granulator_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
