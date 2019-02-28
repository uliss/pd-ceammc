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
		
	}
	
	void fillfx_granulatorSIG0(int count, float* output) {
		for (int i = 0; (i < count); i = (i + 1)) {
			output[i] = 0.0f;
			
		}
		
	}
};

fx_granulatorSIG0* newfx_granulatorSIG0() { return (fx_granulatorSIG0*)new fx_granulatorSIG0(); }
void deletefx_granulatorSIG0(fx_granulatorSIG0* dsp) { delete dsp; }


class fx_granulatorSIG1 {
	
  private:
	
	int iRec70[2];
	
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
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			iRec70[l8] = 0;
			
		}
		
	}
	
	void fillfx_granulatorSIG1(int count, float* output) {
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec70[0] = (iRec70[1] + 1);
			output[i] = (0.5f * (1.0f - std::cos((0.00614192104f * float((iRec70[0] + -1))))));
			iRec70[1] = iRec70[0];
			
		}
		
	}
};

fx_granulatorSIG1* newfx_granulatorSIG1() { return (fx_granulatorSIG1*)new fx_granulatorSIG1(); }
void deletefx_granulatorSIG1(fx_granulatorSIG1* dsp) { delete dsp; }

static float ftbl1fx_granulatorSIG1[1024];

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
	float ftbl0[480000];
	int iVec0[2];
	int fSamplingFreq;
	float fConst0;
	FAUSTFLOAT fHslider1;
	float fRec1[2];
	int iRec0[2];
	FAUSTFLOAT fHslider2;
	float fRec3[2];
	int iRec2[2];
	int iRec5[2];
	int iRec4[2];
	int iRec69[2];
	int iRec71[2];
	int iRec72[2];
	int iRec73[2];
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
	int iRec94[2];
	int iRec95[2];
	int iRec96[2];
	int iRec97[2];
	int iRec98[2];
	int iRec99[2];
	int iRec100[2];
	int iRec101[2];
	int iRec102[2];
	int iRec103[2];
	int iRec104[2];
	int iRec105[2];
	int iRec106[2];
	int iRec107[2];
	int iRec108[2];
	int iRec109[2];
	int iRec110[2];
	int iRec111[2];
	int iRec112[2];
	int iRec113[2];
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
	int iRec128[2];
	int iRec129[2];
	int iRec130[2];
	int iRec131[2];
	int iRec132[2];
	int iRec134[2];
	int iRec133[2];
	int iRec135[2];
	int iRec136[2];
	int iRec137[2];
	int iRec138[2];
	int iRec140[2];
	int iRec139[2];
	int iRec142[2];
	int iRec141[2];
	int iRec144[2];
	int iRec143[2];
	int iRec146[2];
	int iRec145[2];
	int iRec147[2];
	int iRec148[2];
	int iRec150[2];
	int iRec149[2];
	int iRec152[2];
	int iRec151[2];
	int iRec154[2];
	int iRec153[2];
	int iRec155[2];
	int iRec156[2];
	int iRec157[2];
	int iRec158[2];
	int iRec159[2];
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
	int iRec178[2];
	int iRec179[2];
	int iRec180[2];
	int iRec181[2];
	int iRec182[2];
	int iRec183[2];
	int iRec184[2];
	int iRec185[2];
	int iRec186[2];
	int iRec188[2];
	int iRec187[2];
	int iRec190[2];
	int iRec189[2];
	int iRec192[2];
	int iRec191[2];
	int iRec194[2];
	int iRec193[2];
	int iRec195[2];
	int iRec196[2];
	
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
		fx_granulatorSIG1* sig1 = newfx_granulatorSIG1();
		sig1->instanceInitfx_granulatorSIG1(samplingFreq);
		sig1->fillfx_granulatorSIG1(1024, ftbl1fx_granulatorSIG1);
		deletefx_granulatorSIG1(sig1);
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fx_granulatorSIG0* sig0 = newfx_granulatorSIG0();
		sig0->instanceInitfx_granulatorSIG0(samplingFreq);
		sig0->fillfx_granulatorSIG0(480000, ftbl0);
		fConst0 = std::min(192000.0f, std::max(1.0f, float(fSamplingFreq)));
		deletefx_granulatorSIG0(sig0);
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(10.0f);
		fHslider1 = FAUSTFLOAT(10.0f);
		fHslider2 = FAUSTFLOAT(100.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec1[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec0[l2] = 0;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec3[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec2[l4] = 0;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec5[l5] = 0;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec4[l6] = 0;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			iRec69[l7] = 0;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			iRec71[l9] = 0;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			iRec72[l10] = 0;
			
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			iRec73[l11] = 0;
			
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
			iRec94[l32] = 0;
			
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			iRec95[l33] = 0;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			iRec96[l34] = 0;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			iRec97[l35] = 0;
			
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			iRec98[l36] = 0;
			
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			iRec99[l37] = 0;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			iRec100[l38] = 0;
			
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			iRec101[l39] = 0;
			
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			iRec102[l40] = 0;
			
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			iRec103[l41] = 0;
			
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			iRec104[l42] = 0;
			
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			iRec105[l43] = 0;
			
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
			iRec112[l50] = 0;
			
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			iRec113[l51] = 0;
			
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
			iRec128[l66] = 0;
			
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			iRec129[l67] = 0;
			
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
			iRec134[l71] = 0;
			
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			iRec133[l72] = 0;
			
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			iRec135[l73] = 0;
			
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
			iRec140[l77] = 0;
			
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			iRec139[l78] = 0;
			
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			iRec142[l79] = 0;
			
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			iRec141[l80] = 0;
			
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			iRec144[l81] = 0;
			
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			iRec143[l82] = 0;
			
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			iRec146[l83] = 0;
			
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			iRec145[l84] = 0;
			
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			iRec147[l85] = 0;
			
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			iRec148[l86] = 0;
			
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			iRec150[l87] = 0;
			
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			iRec149[l88] = 0;
			
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			iRec152[l89] = 0;
			
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			iRec151[l90] = 0;
			
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			iRec154[l91] = 0;
			
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			iRec153[l92] = 0;
			
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			iRec155[l93] = 0;
			
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			iRec156[l94] = 0;
			
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			iRec157[l95] = 0;
			
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			iRec158[l96] = 0;
			
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			iRec159[l97] = 0;
			
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
			iRec178[l116] = 0;
			
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			iRec179[l117] = 0;
			
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			iRec180[l118] = 0;
			
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			iRec181[l119] = 0;
			
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			iRec182[l120] = 0;
			
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			iRec183[l121] = 0;
			
		}
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			iRec184[l122] = 0;
			
		}
		for (int l123 = 0; (l123 < 2); l123 = (l123 + 1)) {
			iRec185[l123] = 0;
			
		}
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			iRec186[l124] = 0;
			
		}
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			iRec188[l125] = 0;
			
		}
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			iRec187[l126] = 0;
			
		}
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			iRec190[l127] = 0;
			
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			iRec189[l128] = 0;
			
		}
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			iRec192[l129] = 0;
			
		}
		for (int l130 = 0; (l130 < 2); l130 = (l130 + 1)) {
			iRec191[l130] = 0;
			
		}
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			iRec194[l131] = 0;
			
		}
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			iRec193[l132] = 0;
			
		}
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			iRec195[l133] = 0;
			
		}
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			iRec196[l134] = 0;
			
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
		float fSlow1 = (2.0f / fSlow0);
		int iSlow2 = (40.0f < fSlow0);
		float fSlow3 = (0.00100000005f * float(fHslider1));
		float fSlow4 = (9.99999997e-07f * float(fHslider2));
		int iSlow5 = (28.0f < fSlow0);
		int iSlow6 = (2.0f < fSlow0);
		int iSlow7 = (0.0f < fSlow0);
		int iSlow8 = (4.0f < fSlow0);
		int iSlow9 = (6.0f < fSlow0);
		int iSlow10 = (8.0f < fSlow0);
		int iSlow11 = (10.0f < fSlow0);
		int iSlow12 = (12.0f < fSlow0);
		int iSlow13 = (14.0f < fSlow0);
		int iSlow14 = (16.0f < fSlow0);
		int iSlow15 = (18.0f < fSlow0);
		int iSlow16 = (20.0f < fSlow0);
		int iSlow17 = (22.0f < fSlow0);
		int iSlow18 = (24.0f < fSlow0);
		int iSlow19 = (26.0f < fSlow0);
		int iSlow20 = (30.0f < fSlow0);
		int iSlow21 = (32.0f < fSlow0);
		int iSlow22 = (34.0f < fSlow0);
		int iSlow23 = (36.0f < fSlow0);
		int iSlow24 = (38.0f < fSlow0);
		int iSlow25 = (42.0f < fSlow0);
		int iSlow26 = (44.0f < fSlow0);
		int iSlow27 = (46.0f < fSlow0);
		int iSlow28 = (48.0f < fSlow0);
		int iSlow29 = (50.0f < fSlow0);
		int iSlow30 = (52.0f < fSlow0);
		int iSlow31 = (54.0f < fSlow0);
		int iSlow32 = (56.0f < fSlow0);
		int iSlow33 = (58.0f < fSlow0);
		int iSlow34 = (60.0f < fSlow0);
		int iSlow35 = (62.0f < fSlow0);
		int iSlow36 = (59.0f < fSlow0);
		int iSlow37 = (1.0f < fSlow0);
		int iSlow38 = (3.0f < fSlow0);
		int iSlow39 = (5.0f < fSlow0);
		int iSlow40 = (7.0f < fSlow0);
		int iSlow41 = (9.0f < fSlow0);
		int iSlow42 = (11.0f < fSlow0);
		int iSlow43 = (13.0f < fSlow0);
		int iSlow44 = (15.0f < fSlow0);
		int iSlow45 = (17.0f < fSlow0);
		int iSlow46 = (19.0f < fSlow0);
		int iSlow47 = (21.0f < fSlow0);
		int iSlow48 = (23.0f < fSlow0);
		int iSlow49 = (25.0f < fSlow0);
		int iSlow50 = (27.0f < fSlow0);
		int iSlow51 = (29.0f < fSlow0);
		int iSlow52 = (31.0f < fSlow0);
		int iSlow53 = (33.0f < fSlow0);
		int iSlow54 = (35.0f < fSlow0);
		int iSlow55 = (37.0f < fSlow0);
		int iSlow56 = (39.0f < fSlow0);
		int iSlow57 = (41.0f < fSlow0);
		int iSlow58 = (43.0f < fSlow0);
		int iSlow59 = (45.0f < fSlow0);
		int iSlow60 = (47.0f < fSlow0);
		int iSlow61 = (49.0f < fSlow0);
		int iSlow62 = (51.0f < fSlow0);
		int iSlow63 = (53.0f < fSlow0);
		int iSlow64 = (55.0f < fSlow0);
		int iSlow65 = (57.0f < fSlow0);
		int iSlow66 = (61.0f < fSlow0);
		int iSlow67 = (63.0f < fSlow0);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			fRec1[0] = (fSlow3 + (0.999000013f * fRec1[1]));
			int iTemp0 = int((fConst0 * fRec1[0]));
			iRec0[0] = ((iRec0[1] + 1) % iTemp0);
			ftbl0[(iRec0[0] % iTemp0)] = float(input0[i]);
			fRec3[0] = (fSlow4 + (0.999000013f * fRec3[1]));
			int iTemp1 = int((fConst0 * fRec3[0]));
			iRec2[0] = ((iRec2[1] + 1) % iTemp1);
			int iTemp2 = (1 - iVec0[1]);
			int iTemp3 = (1103515245 * (iRec5[1] + 12345));
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
			int iTemp65 = (1103515245 * (iTemp64 + 12345));
			int iTemp66 = (1103515245 * (iTemp65 + 12345));
			iRec5[0] = (1103515245 * (iTemp66 + 12345));
			int iRec6 = iTemp66;
			int iRec7 = iTemp65;
			int iRec8 = iTemp64;
			int iRec9 = iTemp63;
			int iRec10 = iTemp62;
			int iRec11 = iTemp61;
			int iRec12 = iTemp60;
			int iRec13 = iTemp59;
			int iRec14 = iTemp58;
			int iRec15 = iTemp57;
			int iRec16 = iTemp56;
			int iRec17 = iTemp55;
			int iRec18 = iTemp54;
			int iRec19 = iTemp53;
			int iRec20 = iTemp52;
			int iRec21 = iTemp51;
			int iRec22 = iTemp50;
			int iRec23 = iTemp49;
			int iRec24 = iTemp48;
			int iRec25 = iTemp47;
			int iRec26 = iTemp46;
			int iRec27 = iTemp45;
			int iRec28 = iTemp44;
			int iRec29 = iTemp43;
			int iRec30 = iTemp42;
			int iRec31 = iTemp41;
			int iRec32 = iTemp40;
			int iRec33 = iTemp39;
			int iRec34 = iTemp38;
			int iRec35 = iTemp37;
			int iRec36 = iTemp36;
			int iRec37 = iTemp35;
			int iRec38 = iTemp34;
			int iRec39 = iTemp33;
			int iRec40 = iTemp32;
			int iRec41 = iTemp31;
			int iRec42 = iTemp30;
			int iRec43 = iTemp29;
			int iRec44 = iTemp28;
			int iRec45 = iTemp27;
			int iRec46 = iTemp26;
			int iRec47 = iTemp25;
			int iRec48 = iTemp24;
			int iRec49 = iTemp23;
			int iRec50 = iTemp22;
			int iRec51 = iTemp21;
			int iRec52 = iTemp20;
			int iRec53 = iTemp19;
			int iRec54 = iTemp18;
			int iRec55 = iTemp17;
			int iRec56 = iTemp16;
			int iRec57 = iTemp15;
			int iRec58 = iTemp14;
			int iRec59 = iTemp13;
			int iRec60 = iTemp12;
			int iRec61 = iTemp11;
			int iRec62 = iTemp10;
			int iRec63 = iTemp9;
			int iRec64 = iTemp8;
			int iRec65 = iTemp7;
			int iRec66 = iTemp6;
			int iRec67 = iTemp5;
			int iRec68 = iTemp4;
			int iTemp67 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec45)) + 1.0f))));
			iRec4[0] = ((iVec0[1] * iRec4[1]) + (iTemp2 * iTemp67));
			int iTemp68 = ((iRec2[0] + iRec4[0]) % iTemp1);
			int iTemp69 = (iTemp1 + -1);
			float fTemp70 = (float(iTemp68) / float(iTemp69));
			int iTemp71 = int(fTemp70);
			iRec69[0] = ((iRec69[1] * (1 - iTemp71)) + (iTemp67 * iTemp71));
			int iTemp72 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec33)) + 1.0f))));
			iRec71[0] = ((iVec0[1] * iRec71[1]) + (iTemp2 * iTemp72));
			int iTemp73 = ((iRec2[0] + iRec71[0]) % iTemp1);
			float fTemp74 = (float(iTemp73) / float(iTemp69));
			int iTemp75 = int(fTemp74);
			iRec72[0] = ((iTemp72 * iTemp75) + (iRec72[1] * (1 - iTemp75)));
			int iTemp76 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec7)) + 1.0f))));
			iRec73[0] = ((iVec0[1] * iRec73[1]) + (iTemp2 * iTemp76));
			int iTemp77 = ((iRec2[0] + iRec73[0]) % iTemp1);
			float fTemp78 = (float(iTemp77) / float(iTemp69));
			int iTemp79 = int(fTemp78);
			iRec74[0] = ((iRec74[1] * (1 - iTemp79)) + (iTemp76 * iTemp79));
			int iTemp80 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec5[0])) + 1.0f))));
			iRec75[0] = ((iVec0[1] * iRec75[1]) + (iTemp2 * iTemp80));
			int iTemp81 = ((iRec2[0] + iRec75[0]) % iTemp1);
			float fTemp82 = (float(iTemp81) / float(iTemp69));
			int iTemp83 = int(fTemp82);
			iRec76[0] = ((iRec76[1] * (1 - iTemp83)) + (iTemp80 * iTemp83));
			int iTemp84 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec9)) + 1.0f))));
			iRec77[0] = ((iVec0[1] * iRec77[1]) + (iTemp2 * iTemp84));
			int iTemp85 = ((iRec2[0] + iRec77[0]) % iTemp1);
			float fTemp86 = (float(iTemp85) / float(iTemp69));
			int iTemp87 = int(fTemp86);
			iRec78[0] = ((iRec78[1] * (1 - iTemp87)) + (iTemp84 * iTemp87));
			int iTemp88 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec11)) + 1.0f))));
			iRec79[0] = ((iVec0[1] * iRec79[1]) + (iTemp2 * iTemp88));
			int iTemp89 = ((iRec2[0] + iRec79[0]) % iTemp1);
			float fTemp90 = (float(iTemp89) / float(iTemp69));
			int iTemp91 = int(fTemp90);
			iRec80[0] = ((iRec80[1] * (1 - iTemp91)) + (iTemp88 * iTemp91));
			int iTemp92 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec13)) + 1.0f))));
			iRec81[0] = ((iVec0[1] * iRec81[1]) + (iTemp2 * iTemp92));
			int iTemp93 = ((iRec2[0] + iRec81[0]) % iTemp1);
			float fTemp94 = (float(iTemp93) / float(iTemp69));
			int iTemp95 = int(fTemp94);
			iRec82[0] = ((iRec82[1] * (1 - iTemp95)) + (iTemp92 * iTemp95));
			int iTemp96 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec15)) + 1.0f))));
			iRec83[0] = ((iVec0[1] * iRec83[1]) + (iTemp2 * iTemp96));
			int iTemp97 = ((iRec2[0] + iRec83[0]) % iTemp1);
			float fTemp98 = (float(iTemp97) / float(iTemp69));
			int iTemp99 = int(fTemp98);
			iRec84[0] = ((iRec84[1] * (1 - iTemp99)) + (iTemp96 * iTemp99));
			int iTemp100 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec17)) + 1.0f))));
			iRec85[0] = ((iVec0[1] * iRec85[1]) + (iTemp2 * iTemp100));
			int iTemp101 = ((iRec2[0] + iRec85[0]) % iTemp1);
			float fTemp102 = (float(iTemp101) / float(iTemp69));
			int iTemp103 = int(fTemp102);
			iRec86[0] = ((iRec86[1] * (1 - iTemp103)) + (iTemp100 * iTemp103));
			int iTemp104 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec19)) + 1.0f))));
			iRec87[0] = ((iVec0[1] * iRec87[1]) + (iTemp2 * iTemp104));
			int iTemp105 = ((iRec2[0] + iRec87[0]) % iTemp1);
			float fTemp106 = (float(iTemp105) / float(iTemp69));
			int iTemp107 = int(fTemp106);
			iRec88[0] = ((iRec88[1] * (1 - iTemp107)) + (iTemp104 * iTemp107));
			int iTemp108 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec21)) + 1.0f))));
			iRec89[0] = ((iVec0[1] * iRec89[1]) + (iTemp2 * iTemp108));
			int iTemp109 = ((iRec2[0] + iRec89[0]) % iTemp1);
			float fTemp110 = (float(iTemp109) / float(iTemp69));
			int iTemp111 = int(fTemp110);
			iRec90[0] = ((iRec90[1] * (1 - iTemp111)) + (iTemp108 * iTemp111));
			int iTemp112 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec23)) + 1.0f))));
			iRec91[0] = ((iVec0[1] * iRec91[1]) + (iTemp2 * iTemp112));
			int iTemp113 = ((iRec2[0] + iRec91[0]) % iTemp1);
			float fTemp114 = (float(iTemp113) / float(iTemp69));
			int iTemp115 = int(fTemp114);
			iRec92[0] = ((iRec92[1] * (1 - iTemp115)) + (iTemp112 * iTemp115));
			int iTemp116 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec25)) + 1.0f))));
			iRec93[0] = ((iVec0[1] * iRec93[1]) + (iTemp2 * iTemp116));
			int iTemp117 = ((iRec2[0] + iRec93[0]) % iTemp1);
			float fTemp118 = (float(iTemp117) / float(iTemp69));
			int iTemp119 = int(fTemp118);
			iRec94[0] = ((iRec94[1] * (1 - iTemp119)) + (iTemp116 * iTemp119));
			int iTemp120 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec27)) + 1.0f))));
			iRec95[0] = ((iVec0[1] * iRec95[1]) + (iTemp2 * iTemp120));
			int iTemp121 = ((iRec2[0] + iRec95[0]) % iTemp1);
			float fTemp122 = (float(iTemp121) / float(iTemp69));
			int iTemp123 = int(fTemp122);
			iRec96[0] = ((iRec96[1] * (1 - iTemp123)) + (iTemp120 * iTemp123));
			int iTemp124 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec29)) + 1.0f))));
			iRec97[0] = ((iVec0[1] * iRec97[1]) + (iTemp2 * iTemp124));
			int iTemp125 = ((iRec2[0] + iRec97[0]) % iTemp1);
			float fTemp126 = (float(iTemp125) / float(iTemp69));
			int iTemp127 = int(fTemp126);
			iRec98[0] = ((iRec98[1] * (1 - iTemp127)) + (iTemp124 * iTemp127));
			int iTemp128 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec31)) + 1.0f))));
			iRec99[0] = ((iVec0[1] * iRec99[1]) + (iTemp2 * iTemp128));
			int iTemp129 = ((iRec2[0] + iRec99[0]) % iTemp1);
			float fTemp130 = (float(iTemp129) / float(iTemp69));
			int iTemp131 = int(fTemp130);
			iRec100[0] = ((iRec100[1] * (1 - iTemp131)) + (iTemp128 * iTemp131));
			int iTemp132 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec35)) + 1.0f))));
			iRec101[0] = ((iVec0[1] * iRec101[1]) + (iTemp2 * iTemp132));
			int iTemp133 = ((iRec2[0] + iRec101[0]) % iTemp1);
			float fTemp134 = (float(iTemp133) / float(iTemp69));
			int iTemp135 = int(fTemp134);
			iRec102[0] = ((iRec102[1] * (1 - iTemp135)) + (iTemp132 * iTemp135));
			int iTemp136 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec37)) + 1.0f))));
			iRec103[0] = ((iVec0[1] * iRec103[1]) + (iTemp2 * iTemp136));
			int iTemp137 = ((iRec2[0] + iRec103[0]) % iTemp1);
			float fTemp138 = (float(iTemp137) / float(iTemp69));
			int iTemp139 = int(fTemp138);
			iRec104[0] = ((iTemp136 * iTemp139) + (iRec104[1] * (1 - iTemp139)));
			int iTemp140 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec39)) + 1.0f))));
			iRec105[0] = ((iVec0[1] * iRec105[1]) + (iTemp2 * iTemp140));
			int iTemp141 = ((iRec2[0] + iRec105[0]) % iTemp1);
			float fTemp142 = (float(iTemp141) / float(iTemp69));
			int iTemp143 = int(fTemp142);
			iRec106[0] = ((iRec106[1] * (1 - iTemp143)) + (iTemp140 * iTemp143));
			int iTemp144 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec41)) + 1.0f))));
			iRec107[0] = ((iVec0[1] * iRec107[1]) + (iTemp2 * iTemp144));
			int iTemp145 = ((iRec2[0] + iRec107[0]) % iTemp1);
			float fTemp146 = (float(iTemp145) / float(iTemp69));
			int iTemp147 = int(fTemp146);
			iRec108[0] = ((iRec108[1] * (1 - iTemp147)) + (iTemp144 * iTemp147));
			int iTemp148 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec43)) + 1.0f))));
			iRec109[0] = ((iVec0[1] * iRec109[1]) + (iTemp2 * iTemp148));
			int iTemp149 = ((iRec2[0] + iRec109[0]) % iTemp1);
			float fTemp150 = (float(iTemp149) / float(iTemp69));
			int iTemp151 = int(fTemp150);
			iRec110[0] = ((iRec110[1] * (1 - iTemp151)) + (iTemp148 * iTemp151));
			int iTemp152 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec47)) + 1.0f))));
			iRec111[0] = ((iVec0[1] * iRec111[1]) + (iTemp2 * iTemp152));
			int iTemp153 = ((iRec2[0] + iRec111[0]) % iTemp1);
			float fTemp154 = (float(iTemp153) / float(iTemp69));
			int iTemp155 = int(fTemp154);
			iRec112[0] = ((iTemp152 * iTemp155) + (iRec112[1] * (1 - iTemp155)));
			int iTemp156 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec49)) + 1.0f))));
			iRec113[0] = ((iVec0[1] * iRec113[1]) + (iTemp2 * iTemp156));
			int iTemp157 = ((iRec2[0] + iRec113[0]) % iTemp1);
			float fTemp158 = (float(iTemp157) / float(iTemp69));
			int iTemp159 = int(fTemp158);
			iRec114[0] = ((iRec114[1] * (1 - iTemp159)) + (iTemp156 * iTemp159));
			int iTemp160 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec51)) + 1.0f))));
			iRec115[0] = ((iVec0[1] * iRec115[1]) + (iTemp2 * iTemp160));
			int iTemp161 = ((iRec2[0] + iRec115[0]) % iTemp1);
			float fTemp162 = (float(iTemp161) / float(iTemp69));
			int iTemp163 = int(fTemp162);
			iRec116[0] = ((iRec116[1] * (1 - iTemp163)) + (iTemp160 * iTemp163));
			int iTemp164 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec53)) + 1.0f))));
			iRec117[0] = ((iVec0[1] * iRec117[1]) + (iTemp2 * iTemp164));
			int iTemp165 = ((iRec2[0] + iRec117[0]) % iTemp1);
			float fTemp166 = (float(iTemp165) / float(iTemp69));
			int iTemp167 = int(fTemp166);
			iRec118[0] = ((iTemp164 * iTemp167) + (iRec118[1] * (1 - iTemp167)));
			int iTemp168 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec55)) + 1.0f))));
			iRec119[0] = ((iVec0[1] * iRec119[1]) + (iTemp2 * iTemp168));
			int iTemp169 = ((iRec2[0] + iRec119[0]) % iTemp1);
			float fTemp170 = (float(iTemp169) / float(iTemp69));
			int iTemp171 = int(fTemp170);
			iRec120[0] = ((iRec120[1] * (1 - iTemp171)) + (iTemp168 * iTemp171));
			int iTemp172 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec57)) + 1.0f))));
			iRec121[0] = ((iVec0[1] * iRec121[1]) + (iTemp2 * iTemp172));
			int iTemp173 = ((iRec2[0] + iRec121[0]) % iTemp1);
			float fTemp174 = (float(iTemp173) / float(iTemp69));
			int iTemp175 = int(fTemp174);
			iRec122[0] = ((iRec122[1] * (1 - iTemp175)) + (iTemp172 * iTemp175));
			int iTemp176 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec59)) + 1.0f))));
			iRec123[0] = ((iVec0[1] * iRec123[1]) + (iTemp2 * iTemp176));
			int iTemp177 = ((iRec2[0] + iRec123[0]) % iTemp1);
			float fTemp178 = (float(iTemp177) / float(iTemp69));
			int iTemp179 = int(fTemp178);
			iRec124[0] = ((iRec124[1] * (1 - iTemp179)) + (iTemp176 * iTemp179));
			int iTemp180 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec61)) + 1.0f))));
			iRec125[0] = ((iVec0[1] * iRec125[1]) + (iTemp2 * iTemp180));
			int iTemp181 = ((iRec2[0] + iRec125[0]) % iTemp1);
			float fTemp182 = (float(iTemp181) / float(iTemp69));
			int iTemp183 = int(fTemp182);
			iRec126[0] = ((iTemp180 * iTemp183) + (iRec126[1] * (1 - iTemp183)));
			int iTemp184 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec63)) + 1.0f))));
			iRec127[0] = ((iVec0[1] * iRec127[1]) + (iTemp2 * iTemp184));
			int iTemp185 = ((iRec2[0] + iRec127[0]) % iTemp1);
			float fTemp186 = (float(iTemp185) / float(iTemp69));
			int iTemp187 = int(fTemp186);
			iRec128[0] = ((iRec128[1] * (1 - iTemp187)) + (iTemp184 * iTemp187));
			int iTemp188 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec65)) + 1.0f))));
			iRec129[0] = ((iVec0[1] * iRec129[1]) + (iTemp2 * iTemp188));
			int iTemp189 = ((iRec2[0] + iRec129[0]) % iTemp1);
			float fTemp190 = (float(iTemp189) / float(iTemp69));
			int iTemp191 = int(fTemp190);
			iRec130[0] = (((1 - iTemp191) * iRec130[1]) + (iTemp191 * iTemp188));
			int iTemp192 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec67)) + 1.0f))));
			iRec131[0] = ((iVec0[1] * iRec131[1]) + (iTemp2 * iTemp192));
			int iTemp193 = ((iRec2[0] + iRec131[0]) % iTemp1);
			float fTemp194 = (float(iTemp193) / float(iTemp69));
			int iTemp195 = int(fTemp194);
			iRec132[0] = ((iRec132[1] * (1 - iTemp195)) + (iTemp192 * iTemp195));
			output0[i] = FAUSTFLOAT((fSlow1 * (((((((((((((float(iSlow2) * (ftbl0[((iTemp68 + iRec69[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp70))])) + (((((((float(iSlow5) * (ftbl0[((iTemp73 + iRec72[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp74))])) + ((((((((((((((float(iSlow6) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp78))] * ftbl0[((iTemp77 + iRec74[0]) % iTemp0)])) + (float(iSlow7) * (ftbl0[((iTemp81 + iRec76[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp82))]))) + (float(iSlow8) * (ftbl0[((iTemp85 + iRec78[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp86))]))) + (float(iSlow9) * (ftbl0[((iTemp89 + iRec80[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp90))]))) + (float(iSlow10) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp94))] * ftbl0[((iTemp93 + iRec82[0]) % iTemp0)]))) + (float(iSlow11) * (ftbl0[((iTemp97 + iRec84[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp98))]))) + (float(iSlow12) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp102))] * ftbl0[((iTemp101 + iRec86[0]) % iTemp0)]))) + (float(iSlow13) * (ftbl0[((iTemp105 + iRec88[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp106))]))) + (float(iSlow14) * (ftbl0[((iTemp109 + iRec90[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp110))]))) + (float(iSlow15) * (ftbl0[((iTemp113 + iRec92[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp114))]))) + (float(iSlow16) * (ftbl0[((iTemp117 + iRec94[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp118))]))) + (float(iSlow17) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp122))] * ftbl0[((iTemp121 + iRec96[0]) % iTemp0)]))) + (float(iSlow18) * (ftbl0[((iTemp125 + iRec98[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp126))]))) + (float(iSlow19) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp130))] * ftbl0[((iTemp129 + iRec100[0]) % iTemp0)])))) + (float(iSlow20) * (ftbl0[((iTemp133 + iRec102[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp134))]))) + (float(iSlow21) * (ftbl0[((iTemp137 + iRec104[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp138))]))) + (float(iSlow22) * (ftbl0[((iTemp141 + iRec106[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp142))]))) + (float(iSlow23) * (ftbl0[((iTemp145 + iRec108[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp146))]))) + (float(iSlow24) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp150))] * ftbl0[((iTemp149 + iRec110[0]) % iTemp0)])))) + (float(iSlow25) * (ftbl0[((iTemp153 + iRec112[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp154))]))) + (float(iSlow26) * (ftbl0[((iTemp157 + iRec114[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp158))]))) + (float(iSlow27) * (ftbl0[((iTemp161 + iRec116[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp162))]))) + (float(iSlow28) * (ftbl0[((iTemp165 + iRec118[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp166))]))) + (float(iSlow29) * (ftbl0[((iTemp169 + iRec120[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp170))]))) + (float(iSlow30) * (ftbl0[((iTemp173 + iRec122[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp174))]))) + (float(iSlow31) * (ftbl0[((iTemp177 + iRec124[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp178))]))) + (float(iSlow32) * (ftbl0[((iTemp181 + iRec126[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp182))]))) + (float(iSlow33) * (ftbl0[((iTemp185 + iRec128[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp186))]))) + (float(iSlow34) * (ftbl0[((iTemp189 + iRec130[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp190))]))) + (float(iSlow35) * (ftbl0[((iTemp193 + iRec132[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp194))])))));
			int iTemp196 = int((0.5f * (((2.32830644e-10f * float(iRec64)) + 1.0f) * float(iTemp0))));
			iRec134[0] = ((iVec0[1] * iRec134[1]) + (iTemp2 * iTemp196));
			int iTemp197 = ((iRec134[0] + iRec2[0]) % iTemp1);
			float fTemp198 = (float(iTemp197) / float(iTemp69));
			int iTemp199 = int(fTemp198);
			iRec133[0] = ((iRec133[1] * (1 - iTemp199)) + (iTemp196 * iTemp199));
			int iTemp200 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec6)) + 1.0f))));
			iRec135[0] = ((iVec0[1] * iRec135[1]) + (iTemp2 * iTemp200));
			int iTemp201 = ((iRec2[0] + iRec135[0]) % iTemp1);
			float fTemp202 = (float(iTemp201) / float(iTemp69));
			int iTemp203 = int(fTemp202);
			iRec136[0] = ((iRec136[1] * (1 - iTemp203)) + (iTemp200 * iTemp203));
			int iTemp204 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec8)) + 1.0f))));
			iRec137[0] = ((iVec0[1] * iRec137[1]) + (iTemp2 * iTemp204));
			int iTemp205 = ((iRec2[0] + iRec137[0]) % iTemp1);
			float fTemp206 = (float(iTemp205) / float(iTemp69));
			int iTemp207 = int(fTemp206);
			iRec138[0] = (((1 - iTemp207) * iRec138[1]) + (iTemp207 * iTemp204));
			int iTemp208 = int((0.5f * (((2.32830644e-10f * float(iRec10)) + 1.0f) * float(iTemp0))));
			iRec140[0] = ((iVec0[1] * iRec140[1]) + (iTemp2 * iTemp208));
			int iTemp209 = ((iRec140[0] + iRec2[0]) % iTemp1);
			float fTemp210 = (float(iTemp209) / float(iTemp69));
			int iTemp211 = int(fTemp210);
			iRec139[0] = ((iRec139[1] * (1 - iTemp211)) + (iTemp208 * iTemp211));
			int iTemp212 = int((0.5f * (((2.32830644e-10f * float(iRec12)) + 1.0f) * float(iTemp0))));
			iRec142[0] = ((iVec0[1] * iRec142[1]) + (iTemp2 * iTemp212));
			int iTemp213 = ((iRec142[0] + iRec2[0]) % iTemp1);
			float fTemp214 = (float(iTemp213) / float(iTemp69));
			int iTemp215 = int(fTemp214);
			iRec141[0] = ((iRec141[1] * (1 - iTemp215)) + (iTemp212 * iTemp215));
			int iTemp216 = int((0.5f * (((2.32830644e-10f * float(iRec14)) + 1.0f) * float(iTemp0))));
			iRec144[0] = ((iVec0[1] * iRec144[1]) + (iTemp2 * iTemp216));
			int iTemp217 = ((iRec144[0] + iRec2[0]) % iTemp1);
			float fTemp218 = (float(iTemp217) / float(iTemp69));
			int iTemp219 = int(fTemp218);
			iRec143[0] = ((iRec143[1] * (1 - iTemp219)) + (iTemp216 * iTemp219));
			int iTemp220 = int((0.5f * (((2.32830644e-10f * float(iRec16)) + 1.0f) * float(iTemp0))));
			iRec146[0] = ((iVec0[1] * iRec146[1]) + (iTemp2 * iTemp220));
			int iTemp221 = ((iRec146[0] + iRec2[0]) % iTemp1);
			float fTemp222 = (float(iTemp221) / float(iTemp69));
			int iTemp223 = int(fTemp222);
			iRec145[0] = ((iRec145[1] * (1 - iTemp223)) + (iTemp220 * iTemp223));
			int iTemp224 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec18)) + 1.0f))));
			iRec147[0] = ((iVec0[1] * iRec147[1]) + (iTemp2 * iTemp224));
			int iTemp225 = ((iRec2[0] + iRec147[0]) % iTemp1);
			float fTemp226 = (float(iTemp225) / float(iTemp69));
			int iTemp227 = int(fTemp226);
			iRec148[0] = ((iRec148[1] * (1 - iTemp227)) + (iTemp227 * iTemp224));
			int iTemp228 = int((0.5f * (((2.32830644e-10f * float(iRec20)) + 1.0f) * float(iTemp0))));
			iRec150[0] = ((iVec0[1] * iRec150[1]) + (iTemp2 * iTemp228));
			int iTemp229 = ((iRec150[0] + iRec2[0]) % iTemp1);
			float fTemp230 = (float(iTemp229) / float(iTemp69));
			int iTemp231 = int(fTemp230);
			iRec149[0] = ((iRec149[1] * (1 - iTemp231)) + (iTemp228 * iTemp231));
			int iTemp232 = int((0.5f * (((2.32830644e-10f * float(iRec22)) + 1.0f) * float(iTemp0))));
			iRec152[0] = ((iVec0[1] * iRec152[1]) + (iTemp2 * iTemp232));
			int iTemp233 = ((iRec152[0] + iRec2[0]) % iTemp1);
			float fTemp234 = (float(iTemp233) / float(iTemp69));
			int iTemp235 = int(fTemp234);
			iRec151[0] = ((iRec151[1] * (1 - iTemp235)) + (iTemp232 * iTemp235));
			int iTemp236 = int((0.5f * (((2.32830644e-10f * float(iRec24)) + 1.0f) * float(iTemp0))));
			iRec154[0] = ((iVec0[1] * iRec154[1]) + (iTemp2 * iTemp236));
			int iTemp237 = ((iRec154[0] + iRec2[0]) % iTemp1);
			float fTemp238 = (float(iTemp237) / float(iTemp69));
			int iTemp239 = int(fTemp238);
			iRec153[0] = ((iRec153[1] * (1 - iTemp239)) + (iTemp236 * iTemp239));
			int iTemp240 = int((0.5f * (((2.32830644e-10f * float(iRec26)) + 1.0f) * float(iTemp0))));
			iRec155[0] = ((iVec0[1] * iRec155[1]) + (iTemp2 * iTemp240));
			int iTemp241 = ((iRec2[0] + iRec155[0]) % iTemp1);
			float fTemp242 = (float(iTemp241) / float(iTemp69));
			int iTemp243 = int(fTemp242);
			iRec156[0] = ((iRec156[1] * (1 - iTemp243)) + (iTemp243 * iTemp240));
			int iTemp244 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec28)) + 1.0f))));
			iRec157[0] = ((iVec0[1] * iRec157[1]) + (iTemp2 * iTemp244));
			int iTemp245 = ((iRec2[0] + iRec157[0]) % iTemp1);
			float fTemp246 = (float(iTemp245) / float(iTemp69));
			int iTemp247 = int(fTemp246);
			iRec158[0] = ((iRec158[1] * (1 - iTemp247)) + (iTemp244 * iTemp247));
			int iTemp248 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec30)) + 1.0f))));
			iRec159[0] = ((iVec0[1] * iRec159[1]) + (iTemp2 * iTemp248));
			int iTemp249 = ((iRec2[0] + iRec159[0]) % iTemp1);
			float fTemp250 = (float(iTemp249) / float(iTemp69));
			int iTemp251 = int(fTemp250);
			iRec160[0] = ((iRec160[1] * (1 - iTemp251)) + (iTemp248 * iTemp251));
			int iTemp252 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec32)) + 1.0f))));
			iRec161[0] = ((iVec0[1] * iRec161[1]) + (iTemp2 * iTemp252));
			int iTemp253 = ((iRec2[0] + iRec161[0]) % iTemp1);
			float fTemp254 = (float(iTemp253) / float(iTemp69));
			int iTemp255 = int(fTemp254);
			iRec162[0] = ((iRec162[1] * (1 - iTemp255)) + (iTemp252 * iTemp255));
			int iTemp256 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec34)) + 1.0f))));
			iRec163[0] = ((iVec0[1] * iRec163[1]) + (iTemp2 * iTemp256));
			int iTemp257 = ((iRec2[0] + iRec163[0]) % iTemp1);
			float fTemp258 = (float(iTemp257) / float(iTemp69));
			int iTemp259 = int(fTemp258);
			iRec164[0] = ((iRec164[1] * (1 - iTemp259)) + (iTemp256 * iTemp259));
			int iTemp260 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec36)) + 1.0f))));
			iRec165[0] = ((iVec0[1] * iRec165[1]) + (iTemp2 * iTemp260));
			int iTemp261 = ((iRec2[0] + iRec165[0]) % iTemp1);
			float fTemp262 = (float(iTemp261) / float(iTemp69));
			int iTemp263 = int(fTemp262);
			iRec166[0] = ((iRec166[1] * (1 - iTemp263)) + (iTemp260 * iTemp263));
			int iTemp264 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec38)) + 1.0f))));
			iRec167[0] = ((iVec0[1] * iRec167[1]) + (iTemp2 * iTemp264));
			int iTemp265 = ((iRec2[0] + iRec167[0]) % iTemp1);
			float fTemp266 = (float(iTemp265) / float(iTemp69));
			int iTemp267 = int(fTemp266);
			iRec168[0] = ((iRec168[1] * (1 - iTemp267)) + (iTemp264 * iTemp267));
			int iTemp268 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec40)) + 1.0f))));
			iRec169[0] = ((iVec0[1] * iRec169[1]) + (iTemp2 * iTemp268));
			int iTemp269 = ((iRec2[0] + iRec169[0]) % iTemp1);
			float fTemp270 = (float(iTemp269) / float(iTemp69));
			int iTemp271 = int(fTemp270);
			iRec170[0] = ((iRec170[1] * (1 - iTemp271)) + (iTemp268 * iTemp271));
			int iTemp272 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec42)) + 1.0f))));
			iRec171[0] = ((iVec0[1] * iRec171[1]) + (iTemp2 * iTemp272));
			int iTemp273 = ((iRec2[0] + iRec171[0]) % iTemp1);
			float fTemp274 = (float(iTemp273) / float(iTemp69));
			int iTemp275 = int(fTemp274);
			iRec172[0] = ((iRec172[1] * (1 - iTemp275)) + (iTemp272 * iTemp275));
			int iTemp276 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec44)) + 1.0f))));
			iRec173[0] = ((iVec0[1] * iRec173[1]) + (iTemp2 * iTemp276));
			int iTemp277 = ((iRec2[0] + iRec173[0]) % iTemp1);
			float fTemp278 = (float(iTemp277) / float(iTemp69));
			int iTemp279 = int(fTemp278);
			iRec174[0] = ((iRec174[1] * (1 - iTemp279)) + (iTemp276 * iTemp279));
			int iTemp280 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec46)) + 1.0f))));
			iRec175[0] = ((iVec0[1] * iRec175[1]) + (iTemp2 * iTemp280));
			int iTemp281 = ((iRec2[0] + iRec175[0]) % iTemp1);
			float fTemp282 = (float(iTemp281) / float(iTemp69));
			int iTemp283 = int(fTemp282);
			iRec176[0] = ((iRec176[1] * (1 - iTemp283)) + (iTemp280 * iTemp283));
			int iTemp284 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec48)) + 1.0f))));
			iRec177[0] = ((iVec0[1] * iRec177[1]) + (iTemp2 * iTemp284));
			int iTemp285 = ((iRec2[0] + iRec177[0]) % iTemp1);
			float fTemp286 = (float(iTemp285) / float(iTemp69));
			int iTemp287 = int(fTemp286);
			iRec178[0] = ((iRec178[1] * (1 - iTemp287)) + (iTemp284 * iTemp287));
			int iTemp288 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec50)) + 1.0f))));
			iRec179[0] = ((iVec0[1] * iRec179[1]) + (iTemp2 * iTemp288));
			int iTemp289 = ((iRec2[0] + iRec179[0]) % iTemp1);
			float fTemp290 = (float(iTemp289) / float(iTemp69));
			int iTemp291 = int(fTemp290);
			iRec180[0] = ((iRec180[1] * (1 - iTemp291)) + (iTemp288 * iTemp291));
			int iTemp292 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec52)) + 1.0f))));
			iRec181[0] = ((iVec0[1] * iRec181[1]) + (iTemp2 * iTemp292));
			int iTemp293 = ((iRec2[0] + iRec181[0]) % iTemp1);
			float fTemp294 = (float(iTemp293) / float(iTemp69));
			int iTemp295 = int(fTemp294);
			iRec182[0] = ((iRec182[1] * (1 - iTemp295)) + (iTemp292 * iTemp295));
			int iTemp296 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec54)) + 1.0f))));
			iRec183[0] = ((iVec0[1] * iRec183[1]) + (iTemp2 * iTemp296));
			int iTemp297 = ((iRec2[0] + iRec183[0]) % iTemp1);
			float fTemp298 = (float(iTemp297) / float(iTemp69));
			int iTemp299 = int(fTemp298);
			iRec184[0] = ((iRec184[1] * (1 - iTemp299)) + (iTemp296 * iTemp299));
			int iTemp300 = int((0.5f * (float(iTemp0) * ((2.32830644e-10f * float(iRec56)) + 1.0f))));
			iRec185[0] = ((iVec0[1] * iRec185[1]) + (iTemp2 * iTemp300));
			int iTemp301 = ((iRec2[0] + iRec185[0]) % iTemp1);
			float fTemp302 = (float(iTemp301) / float(iTemp69));
			int iTemp303 = int(fTemp302);
			iRec186[0] = (((1 - iTemp303) * iRec186[1]) + (iTemp303 * iTemp300));
			int iTemp304 = int((0.5f * (((2.32830644e-10f * float(iRec58)) + 1.0f) * float(iTemp0))));
			iRec188[0] = ((iVec0[1] * iRec188[1]) + (iTemp2 * iTemp304));
			int iTemp305 = ((iRec188[0] + iRec2[0]) % iTemp1);
			float fTemp306 = (float(iTemp305) / float(iTemp69));
			int iTemp307 = int(fTemp306);
			iRec187[0] = ((iRec187[1] * (1 - iTemp307)) + (iTemp304 * iTemp307));
			int iTemp308 = int((0.5f * (((2.32830644e-10f * float(iRec60)) + 1.0f) * float(iTemp0))));
			iRec190[0] = ((iVec0[1] * iRec190[1]) + (iTemp2 * iTemp308));
			int iTemp309 = ((iRec190[0] + iRec2[0]) % iTemp1);
			float fTemp310 = (float(iTemp309) / float(iTemp69));
			int iTemp311 = int(fTemp310);
			iRec189[0] = ((iRec189[1] * (1 - iTemp311)) + (iTemp308 * iTemp311));
			int iTemp312 = int((0.5f * (((2.32830644e-10f * float(iRec62)) + 1.0f) * float(iTemp0))));
			iRec192[0] = ((iVec0[1] * iRec192[1]) + (iTemp2 * iTemp312));
			int iTemp313 = ((iRec192[0] + iRec2[0]) % iTemp1);
			float fTemp314 = (float(iTemp313) / float(iTemp69));
			int iTemp315 = int(fTemp314);
			iRec191[0] = ((iRec191[1] * (1 - iTemp315)) + (iTemp312 * iTemp315));
			int iTemp316 = int((0.5f * (((2.32830644e-10f * float(iRec66)) + 1.0f) * float(iTemp0))));
			iRec194[0] = ((iVec0[1] * iRec194[1]) + (iTemp2 * iTemp316));
			int iTemp317 = ((iRec194[0] + iRec2[0]) % iTemp1);
			float fTemp318 = (float(iTemp317) / float(iTemp69));
			int iTemp319 = int(fTemp318);
			iRec193[0] = ((iRec193[1] * (1 - iTemp319)) + (iTemp316 * iTemp319));
			int iTemp320 = int((0.5f * (((2.32830644e-10f * float(iRec68)) + 1.0f) * float(iTemp0))));
			iRec195[0] = ((iVec0[1] * iRec195[1]) + (iTemp2 * iTemp320));
			int iTemp321 = ((iRec195[0] + iRec2[0]) % iTemp1);
			float fTemp322 = (float(iTemp321) / float(iTemp69));
			int iTemp323 = int(fTemp322);
			iRec196[0] = ((iRec196[1] * (1 - iTemp323)) + (iTemp320 * iTemp323));
			output1[i] = FAUSTFLOAT((fSlow1 * ((((float(iSlow36) * (ftbl0[((iRec133[0] + iTemp197) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp198))])) + (((((((((((((((((((((((((((((float(iSlow37) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp202))] * ftbl0[((iTemp201 + iRec136[0]) % iTemp0)])) + (float(iSlow38) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp206))] * ftbl0[((iRec138[0] + iTemp205) % iTemp0)]))) + (float(iSlow39) * (ftbl0[((iRec139[0] + iTemp209) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp210))]))) + (float(iSlow40) * (ftbl0[((iRec141[0] + iTemp213) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp214))]))) + (float(iSlow41) * (ftbl0[((iRec143[0] + iTemp217) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp218))]))) + (float(iSlow42) * (ftbl0[((iRec145[0] + iTemp221) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp222))]))) + (float(iSlow43) * (ftbl0[((iTemp225 + iRec148[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp226))]))) + (float(iSlow44) * (ftbl0[((iRec149[0] + iTemp229) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp230))]))) + (float(iSlow45) * (ftbl0[((iRec151[0] + iTemp233) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp234))]))) + (float(iSlow46) * (ftbl0[((iRec153[0] + iTemp237) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp238))]))) + (float(iSlow47) * (ftbl0[((iTemp241 + iRec156[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp242))]))) + (float(iSlow48) * (ftbl0[((iTemp245 + iRec158[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp246))]))) + (float(iSlow49) * (ftbl0[((iTemp249 + iRec160[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp250))]))) + (float(iSlow50) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp254))] * ftbl0[((iTemp253 + iRec162[0]) % iTemp0)]))) + (float(iSlow51) * (ftbl0[((iTemp257 + iRec164[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp258))]))) + (float(iSlow52) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp262))] * ftbl0[((iTemp261 + iRec166[0]) % iTemp0)]))) + (float(iSlow53) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp266))] * ftbl0[((iTemp265 + iRec168[0]) % iTemp0)]))) + (float(iSlow54) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp270))] * ftbl0[((iTemp269 + iRec170[0]) % iTemp0)]))) + (float(iSlow55) * (ftbl0[((iTemp273 + iRec172[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp274))]))) + (float(iSlow56) * (ftbl0[((iTemp277 + iRec174[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp278))]))) + (float(iSlow57) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp282))] * ftbl0[((iTemp281 + iRec176[0]) % iTemp0)]))) + (float(iSlow58) * (ftbl0[((iTemp285 + iRec178[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp286))]))) + (float(iSlow59) * (ftbl0[((iTemp289 + iRec180[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp290))]))) + (float(iSlow60) * (ftbl0[((iTemp293 + iRec182[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp294))]))) + (float(iSlow61) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp298))] * ftbl0[((iTemp297 + iRec184[0]) % iTemp0)]))) + (float(iSlow62) * (ftbl1fx_granulatorSIG1[int((1024.0f * fTemp302))] * ftbl0[((iRec186[0] + iTemp301) % iTemp0)]))) + (float(iSlow63) * (ftbl0[((iRec187[0] + iTemp305) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp306))]))) + (float(iSlow64) * (ftbl0[((iRec189[0] + iTemp309) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp310))]))) + (float(iSlow65) * (ftbl0[((iRec191[0] + iTemp313) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp314))])))) + (float(iSlow66) * (ftbl0[((iRec193[0] + iTemp317) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp318))]))) + (float(iSlow67) * (ftbl0[((iTemp321 + iRec196[0]) % iTemp0)] * ftbl1fx_granulatorSIG1[int((1024.0f * fTemp322))])))));
			iVec0[1] = iVec0[0];
			fRec1[1] = fRec1[0];
			iRec0[1] = iRec0[0];
			fRec3[1] = fRec3[0];
			iRec2[1] = iRec2[0];
			iRec5[1] = iRec5[0];
			iRec4[1] = iRec4[0];
			iRec69[1] = iRec69[0];
			iRec71[1] = iRec71[0];
			iRec72[1] = iRec72[0];
			iRec73[1] = iRec73[0];
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
			iRec94[1] = iRec94[0];
			iRec95[1] = iRec95[0];
			iRec96[1] = iRec96[0];
			iRec97[1] = iRec97[0];
			iRec98[1] = iRec98[0];
			iRec99[1] = iRec99[0];
			iRec100[1] = iRec100[0];
			iRec101[1] = iRec101[0];
			iRec102[1] = iRec102[0];
			iRec103[1] = iRec103[0];
			iRec104[1] = iRec104[0];
			iRec105[1] = iRec105[0];
			iRec106[1] = iRec106[0];
			iRec107[1] = iRec107[0];
			iRec108[1] = iRec108[0];
			iRec109[1] = iRec109[0];
			iRec110[1] = iRec110[0];
			iRec111[1] = iRec111[0];
			iRec112[1] = iRec112[0];
			iRec113[1] = iRec113[0];
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
			iRec128[1] = iRec128[0];
			iRec129[1] = iRec129[0];
			iRec130[1] = iRec130[0];
			iRec131[1] = iRec131[0];
			iRec132[1] = iRec132[0];
			iRec134[1] = iRec134[0];
			iRec133[1] = iRec133[0];
			iRec135[1] = iRec135[0];
			iRec136[1] = iRec136[0];
			iRec137[1] = iRec137[0];
			iRec138[1] = iRec138[0];
			iRec140[1] = iRec140[0];
			iRec139[1] = iRec139[0];
			iRec142[1] = iRec142[0];
			iRec141[1] = iRec141[0];
			iRec144[1] = iRec144[0];
			iRec143[1] = iRec143[0];
			iRec146[1] = iRec146[0];
			iRec145[1] = iRec145[0];
			iRec147[1] = iRec147[0];
			iRec148[1] = iRec148[0];
			iRec150[1] = iRec150[0];
			iRec149[1] = iRec149[0];
			iRec152[1] = iRec152[0];
			iRec151[1] = iRec151[0];
			iRec154[1] = iRec154[0];
			iRec153[1] = iRec153[0];
			iRec155[1] = iRec155[0];
			iRec156[1] = iRec156[0];
			iRec157[1] = iRec157[0];
			iRec158[1] = iRec158[0];
			iRec159[1] = iRec159[0];
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
			iRec178[1] = iRec178[0];
			iRec179[1] = iRec179[0];
			iRec180[1] = iRec180[0];
			iRec181[1] = iRec181[0];
			iRec182[1] = iRec182[0];
			iRec183[1] = iRec183[0];
			iRec184[1] = iRec184[0];
			iRec185[1] = iRec185[0];
			iRec186[1] = iRec186[0];
			iRec188[1] = iRec188[0];
			iRec187[1] = iRec187[0];
			iRec190[1] = iRec190[0];
			iRec189[1] = iRec189[0];
			iRec192[1] = iRec192[0];
			iRec191[1] = iRec191[0];
			iRec194[1] = iRec194[0];
			iRec193[1] = iRec193[0];
			iRec195[1] = iRec195[0];
			iRec196[1] = iRec196[0];
			
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
