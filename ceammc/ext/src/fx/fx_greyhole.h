/* ------------------------------------------------------------
author: "Julian Parker, bug fixes by Till Bovermann"
copyright: "(c) Julian Parker 2013"
license: "GPL2+"
name: "GreyholeRaw"
version: "1.0"
Code generated with Faust 2.8.5 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_greyhole_H__
#define  __fx_greyhole_H__

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
struct fx_greyhole : public dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "primes" */
#include "jprev.h"
#include <algorithm>
#include <cmath>
#include <math.h>


#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_greyhole
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_greyhole : public dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int iVec0[2];
	FAUSTFLOAT fHslider0;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fVec1[2];
	FAUSTFLOAT fHslider2;
	float fVec2[2];
	FAUSTFLOAT fHslider3;
	float fRec10[2];
	FAUSTFLOAT fHslider4;
	float fVec3[2];
	int fSamplingFreq;
	float fConst0;
	FAUSTFLOAT fHslider5;
	float fRec17[2];
	float fRec18[2];
	float fRec19[2];
	float fRec20[2];
	float fConst1;
	FAUSTFLOAT fHslider6;
	float fVec4[2];
	float fConst2;
	FAUSTFLOAT fHslider7;
	float fVec5[2];
	float fRec21[2];
	float fRec22[2];
	int IOTA;
	float fVec6[131072];
	float fRec29[2];
	float fRec36[2];
	float fVec7[16384];
	float fRec35[2];
	float fRec33[2];
	float fRec38[2];
	float fVec8[131072];
	float fVec9[16384];
	float fRec37[2];
	float fRec34[2];
	float fVec10[16384];
	float fRec39[2];
	float fRec32[2];
	float fRec30[2];
	float fVec11[16384];
	float fRec41[2];
	float fRec40[2];
	float fRec31[2];
	float fVec12[16384];
	float fRec28[2];
	float fRec26[2];
	float fVec13[16384];
	float fRec43[2];
	float fRec42[2];
	float fRec27[2];
	float fVec14[16384];
	float fRec44[2];
	float fRec25[2];
	float fRec23[2];
	float fVec15[16384];
	float fRec46[2];
	float fRec45[2];
	float fRec24[2];
	float fVec16[16384];
	float fRec61[2];
	float fRec59[2];
	float fVec17[16384];
	float fRec62[2];
	float fRec60[2];
	float fVec18[16384];
	float fRec58[2];
	float fRec56[2];
	float fVec19[16384];
	float fRec63[2];
	float fRec57[2];
	float fVec20[16384];
	float fRec55[2];
	float fRec53[2];
	float fVec21[16384];
	float fRec64[2];
	float fRec54[2];
	float fVec22[16384];
	float fRec52[2];
	float fRec50[2];
	float fVec23[16384];
	float fRec65[2];
	float fRec51[2];
	float fRec72[2];
	float fVec24[16384];
	float fRec84[2];
	float fRec82[2];
	float fVec25[16384];
	float fRec85[2];
	float fRec83[2];
	float fVec26[16384];
	float fRec81[2];
	float fRec79[2];
	float fVec27[16384];
	float fRec86[2];
	float fRec80[2];
	float fVec28[16384];
	float fRec78[2];
	float fRec76[2];
	float fVec29[16384];
	float fRec87[2];
	float fRec77[2];
	float fVec30[16384];
	float fRec75[2];
	float fRec73[2];
	float fVec31[16384];
	float fRec88[2];
	float fRec74[2];
	float fRec92[2];
	float fVec32[16384];
	float fRec104[2];
	float fRec102[2];
	float fVec33[16384];
	float fRec105[2];
	float fRec103[2];
	float fVec34[16384];
	float fRec101[2];
	float fRec99[2];
	float fVec35[16384];
	float fRec106[2];
	float fRec100[2];
	float fVec36[16384];
	float fRec98[2];
	float fRec96[2];
	float fVec37[16384];
	float fRec107[2];
	float fRec97[2];
	float fVec38[16384];
	float fRec95[2];
	float fRec93[2];
	float fVec39[16384];
	float fRec108[2];
	float fRec94[2];
	float fVec40[16384];
	float fRec91[2];
	float fRec89[2];
	float fVec41[16384];
	float fRec110[2];
	float fRec109[2];
	float fRec90[2];
	float fVec42[16384];
	float fRec71[2];
	float fRec69[2];
	float fRec112[2];
	float fVec43[16384];
	float fRec111[2];
	float fRec70[2];
	float fVec44[16384];
	float fRec113[2];
	float fRec68[2];
	float fRec66[2];
	float fVec45[16384];
	float fRec115[2];
	float fRec114[2];
	float fRec67[2];
	float fVec46[16384];
	float fRec116[2];
	float fRec49[2];
	float fRec47[2];
	float fVec47[16384];
	float fRec118[2];
	float fRec117[2];
	float fRec48[2];
	float fVec48[16384];
	float fRec119[2];
	float fRec16[2];
	float fRec14[2];
	float fVec49[16384];
	float fRec121[2];
	float fRec120[2];
	float fRec15[2];
	float fVec50[16384];
	float fRec133[2];
	float fRec131[2];
	float fVec51[16384];
	float fRec134[2];
	float fRec132[2];
	float fVec52[16384];
	float fRec130[2];
	float fRec128[2];
	float fVec53[16384];
	float fRec135[2];
	float fRec129[2];
	float fVec54[16384];
	float fRec127[2];
	float fRec125[2];
	float fVec55[16384];
	float fRec136[2];
	float fRec126[2];
	float fVec56[16384];
	float fRec124[2];
	float fRec122[2];
	float fVec57[16384];
	float fRec137[2];
	float fRec123[2];
	float fVec58[16384];
	float fRec149[2];
	float fRec147[2];
	float fVec59[16384];
	float fRec150[2];
	float fRec148[2];
	float fVec60[16384];
	float fRec146[2];
	float fRec144[2];
	float fVec61[16384];
	float fRec151[2];
	float fRec145[2];
	float fVec62[16384];
	float fRec143[2];
	float fRec141[2];
	float fVec63[16384];
	float fRec152[2];
	float fRec142[2];
	float fVec64[16384];
	float fRec140[2];
	float fRec138[2];
	float fVec65[16384];
	float fRec153[2];
	float fRec139[2];
	float fVec66[16384];
	float fRec154[2];
	float fRec13[2];
	float fRec11[2];
	float fVec67[16384];
	float fRec156[2];
	float fRec155[2];
	float fRec12[2];
	float fVec68[16384];
	float fRec168[2];
	float fRec166[2];
	float fVec69[16384];
	float fRec169[2];
	float fRec167[2];
	float fVec70[16384];
	float fRec165[2];
	float fRec163[2];
	float fVec71[16384];
	float fRec170[2];
	float fRec164[2];
	float fVec72[16384];
	float fRec162[2];
	float fRec160[2];
	float fVec73[16384];
	float fRec171[2];
	float fRec161[2];
	float fVec74[16384];
	float fRec159[2];
	float fRec157[2];
	float fVec75[16384];
	float fRec172[2];
	float fRec158[2];
	float fVec76[16384];
	float fRec184[2];
	float fRec182[2];
	float fVec77[16384];
	float fRec185[2];
	float fRec183[2];
	float fVec78[16384];
	float fRec181[2];
	float fRec179[2];
	float fVec79[16384];
	float fRec186[2];
	float fRec180[2];
	float fVec80[16384];
	float fRec178[2];
	float fRec176[2];
	float fVec81[16384];
	float fRec187[2];
	float fRec177[2];
	float fVec82[16384];
	float fRec175[2];
	float fRec173[2];
	float fVec83[16384];
	float fRec188[2];
	float fRec174[2];
	float fVec84[16384];
	float fRec9[2];
	float fRec7[2];
	float fRec190[2];
	float fVec85[16384];
	float fRec189[2];
	float fRec8[2];
	float fVec86[16384];
	float fRec202[2];
	float fRec200[2];
	float fVec87[16384];
	float fRec203[2];
	float fRec201[2];
	float fVec88[16384];
	float fRec199[2];
	float fRec197[2];
	float fVec89[16384];
	float fRec204[2];
	float fRec198[2];
	float fVec90[16384];
	float fRec196[2];
	float fRec194[2];
	float fVec91[16384];
	float fRec205[2];
	float fRec195[2];
	float fVec92[16384];
	float fRec193[2];
	float fRec191[2];
	float fVec93[16384];
	float fRec206[2];
	float fRec192[2];
	float fVec94[16384];
	float fRec218[2];
	float fRec216[2];
	float fVec95[16384];
	float fRec219[2];
	float fRec217[2];
	float fVec96[16384];
	float fRec215[2];
	float fRec213[2];
	float fVec97[16384];
	float fRec220[2];
	float fRec214[2];
	float fVec98[16384];
	float fRec212[2];
	float fRec210[2];
	float fVec99[16384];
	float fRec221[2];
	float fRec211[2];
	float fVec100[16384];
	float fRec209[2];
	float fRec207[2];
	float fVec101[16384];
	float fRec222[2];
	float fRec208[2];
	float fVec102[16384];
	float fRec223[2];
	float fRec6[2];
	float fRec4[2];
	float fVec103[16384];
	float fRec225[2];
	float fRec224[2];
	float fRec5[2];
	float fVec104[16384];
	float fRec237[2];
	float fRec235[2];
	float fVec105[16384];
	float fRec238[2];
	float fRec236[2];
	float fVec106[16384];
	float fRec234[2];
	float fRec232[2];
	float fVec107[16384];
	float fRec239[2];
	float fRec233[2];
	float fVec108[16384];
	float fRec231[2];
	float fRec229[2];
	float fVec109[16384];
	float fRec240[2];
	float fRec230[2];
	float fVec110[16384];
	float fRec228[2];
	float fRec226[2];
	float fVec111[16384];
	float fRec241[2];
	float fRec227[2];
	float fVec112[16384];
	float fRec253[2];
	float fRec251[2];
	float fVec113[16384];
	float fRec254[2];
	float fRec252[2];
	float fVec114[16384];
	float fRec250[2];
	float fRec248[2];
	float fVec115[16384];
	float fRec255[2];
	float fRec249[2];
	float fVec116[16384];
	float fRec247[2];
	float fRec245[2];
	float fVec117[16384];
	float fRec256[2];
	float fRec246[2];
	float fVec118[16384];
	float fRec244[2];
	float fRec242[2];
	float fVec119[16384];
	float fRec257[2];
	float fRec243[2];
	float fRec3[2];
	float fRec1[1024];
	float fRec258[2];
	float fRec2[1024];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Julian Parker, bug fixes by Till Bovermann");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.1");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.1");
		m->declare("copyright", "(c) Julian Parker 2013");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("filename", "fx_greyhole");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("license", "GPL2+");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "GreyholeRaw");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
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
		fConst1 = (0.000566893432f * fConst0);
		fConst2 = (3.14159274f / fConst0);
		
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(1.0f);
		fHslider4 = FAUSTFLOAT(0.90000000000000002f);
		fHslider5 = FAUSTFLOAT(0.20000000000000001f);
		fHslider6 = FAUSTFLOAT(0.10000000000000001f);
		fHslider7 = FAUSTFLOAT(2.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec0[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fVec1[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fVec2[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec10[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec3[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec17[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec18[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec19[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec20[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fVec4[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fVec5[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec21[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec22[l13] = 0.0f;
			
		}
		IOTA = 0;
		for (int l14 = 0; (l14 < 131072); l14 = (l14 + 1)) {
			fVec6[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec29[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec36[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 16384); l17 = (l17 + 1)) {
			fVec7[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec35[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec33[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec38[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 131072); l21 = (l21 + 1)) {
			fVec8[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 16384); l22 = (l22 + 1)) {
			fVec9[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec37[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec34[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 16384); l25 = (l25 + 1)) {
			fVec10[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec39[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec32[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec30[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 16384); l29 = (l29 + 1)) {
			fVec11[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec41[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec40[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec31[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 16384); l33 = (l33 + 1)) {
			fVec12[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec28[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec26[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 16384); l36 = (l36 + 1)) {
			fVec13[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec43[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec42[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec27[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 16384); l40 = (l40 + 1)) {
			fVec14[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec44[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec25[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec23[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 16384); l44 = (l44 + 1)) {
			fVec15[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec46[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec45[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec24[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 16384); l48 = (l48 + 1)) {
			fVec16[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec61[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec59[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 16384); l51 = (l51 + 1)) {
			fVec17[l51] = 0.0f;
			
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec62[l52] = 0.0f;
			
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec60[l53] = 0.0f;
			
		}
		for (int l54 = 0; (l54 < 16384); l54 = (l54 + 1)) {
			fVec18[l54] = 0.0f;
			
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec58[l55] = 0.0f;
			
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec56[l56] = 0.0f;
			
		}
		for (int l57 = 0; (l57 < 16384); l57 = (l57 + 1)) {
			fVec19[l57] = 0.0f;
			
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec63[l58] = 0.0f;
			
		}
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec57[l59] = 0.0f;
			
		}
		for (int l60 = 0; (l60 < 16384); l60 = (l60 + 1)) {
			fVec20[l60] = 0.0f;
			
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec55[l61] = 0.0f;
			
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec53[l62] = 0.0f;
			
		}
		for (int l63 = 0; (l63 < 16384); l63 = (l63 + 1)) {
			fVec21[l63] = 0.0f;
			
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec64[l64] = 0.0f;
			
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec54[l65] = 0.0f;
			
		}
		for (int l66 = 0; (l66 < 16384); l66 = (l66 + 1)) {
			fVec22[l66] = 0.0f;
			
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec52[l67] = 0.0f;
			
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec50[l68] = 0.0f;
			
		}
		for (int l69 = 0; (l69 < 16384); l69 = (l69 + 1)) {
			fVec23[l69] = 0.0f;
			
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec65[l70] = 0.0f;
			
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec51[l71] = 0.0f;
			
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec72[l72] = 0.0f;
			
		}
		for (int l73 = 0; (l73 < 16384); l73 = (l73 + 1)) {
			fVec24[l73] = 0.0f;
			
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec84[l74] = 0.0f;
			
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec82[l75] = 0.0f;
			
		}
		for (int l76 = 0; (l76 < 16384); l76 = (l76 + 1)) {
			fVec25[l76] = 0.0f;
			
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec85[l77] = 0.0f;
			
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec83[l78] = 0.0f;
			
		}
		for (int l79 = 0; (l79 < 16384); l79 = (l79 + 1)) {
			fVec26[l79] = 0.0f;
			
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec81[l80] = 0.0f;
			
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec79[l81] = 0.0f;
			
		}
		for (int l82 = 0; (l82 < 16384); l82 = (l82 + 1)) {
			fVec27[l82] = 0.0f;
			
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec86[l83] = 0.0f;
			
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec80[l84] = 0.0f;
			
		}
		for (int l85 = 0; (l85 < 16384); l85 = (l85 + 1)) {
			fVec28[l85] = 0.0f;
			
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec78[l86] = 0.0f;
			
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec76[l87] = 0.0f;
			
		}
		for (int l88 = 0; (l88 < 16384); l88 = (l88 + 1)) {
			fVec29[l88] = 0.0f;
			
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec87[l89] = 0.0f;
			
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec77[l90] = 0.0f;
			
		}
		for (int l91 = 0; (l91 < 16384); l91 = (l91 + 1)) {
			fVec30[l91] = 0.0f;
			
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec75[l92] = 0.0f;
			
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec73[l93] = 0.0f;
			
		}
		for (int l94 = 0; (l94 < 16384); l94 = (l94 + 1)) {
			fVec31[l94] = 0.0f;
			
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec88[l95] = 0.0f;
			
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec74[l96] = 0.0f;
			
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec92[l97] = 0.0f;
			
		}
		for (int l98 = 0; (l98 < 16384); l98 = (l98 + 1)) {
			fVec32[l98] = 0.0f;
			
		}
		for (int l99 = 0; (l99 < 2); l99 = (l99 + 1)) {
			fRec104[l99] = 0.0f;
			
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			fRec102[l100] = 0.0f;
			
		}
		for (int l101 = 0; (l101 < 16384); l101 = (l101 + 1)) {
			fVec33[l101] = 0.0f;
			
		}
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			fRec105[l102] = 0.0f;
			
		}
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			fRec103[l103] = 0.0f;
			
		}
		for (int l104 = 0; (l104 < 16384); l104 = (l104 + 1)) {
			fVec34[l104] = 0.0f;
			
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec101[l105] = 0.0f;
			
		}
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec99[l106] = 0.0f;
			
		}
		for (int l107 = 0; (l107 < 16384); l107 = (l107 + 1)) {
			fVec35[l107] = 0.0f;
			
		}
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			fRec106[l108] = 0.0f;
			
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec100[l109] = 0.0f;
			
		}
		for (int l110 = 0; (l110 < 16384); l110 = (l110 + 1)) {
			fVec36[l110] = 0.0f;
			
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec98[l111] = 0.0f;
			
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fRec96[l112] = 0.0f;
			
		}
		for (int l113 = 0; (l113 < 16384); l113 = (l113 + 1)) {
			fVec37[l113] = 0.0f;
			
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec107[l114] = 0.0f;
			
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec97[l115] = 0.0f;
			
		}
		for (int l116 = 0; (l116 < 16384); l116 = (l116 + 1)) {
			fVec38[l116] = 0.0f;
			
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fRec95[l117] = 0.0f;
			
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			fRec93[l118] = 0.0f;
			
		}
		for (int l119 = 0; (l119 < 16384); l119 = (l119 + 1)) {
			fVec39[l119] = 0.0f;
			
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec108[l120] = 0.0f;
			
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec94[l121] = 0.0f;
			
		}
		for (int l122 = 0; (l122 < 16384); l122 = (l122 + 1)) {
			fVec40[l122] = 0.0f;
			
		}
		for (int l123 = 0; (l123 < 2); l123 = (l123 + 1)) {
			fRec91[l123] = 0.0f;
			
		}
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			fRec89[l124] = 0.0f;
			
		}
		for (int l125 = 0; (l125 < 16384); l125 = (l125 + 1)) {
			fVec41[l125] = 0.0f;
			
		}
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			fRec110[l126] = 0.0f;
			
		}
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			fRec109[l127] = 0.0f;
			
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			fRec90[l128] = 0.0f;
			
		}
		for (int l129 = 0; (l129 < 16384); l129 = (l129 + 1)) {
			fVec42[l129] = 0.0f;
			
		}
		for (int l130 = 0; (l130 < 2); l130 = (l130 + 1)) {
			fRec71[l130] = 0.0f;
			
		}
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			fRec69[l131] = 0.0f;
			
		}
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			fRec112[l132] = 0.0f;
			
		}
		for (int l133 = 0; (l133 < 16384); l133 = (l133 + 1)) {
			fVec43[l133] = 0.0f;
			
		}
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			fRec111[l134] = 0.0f;
			
		}
		for (int l135 = 0; (l135 < 2); l135 = (l135 + 1)) {
			fRec70[l135] = 0.0f;
			
		}
		for (int l136 = 0; (l136 < 16384); l136 = (l136 + 1)) {
			fVec44[l136] = 0.0f;
			
		}
		for (int l137 = 0; (l137 < 2); l137 = (l137 + 1)) {
			fRec113[l137] = 0.0f;
			
		}
		for (int l138 = 0; (l138 < 2); l138 = (l138 + 1)) {
			fRec68[l138] = 0.0f;
			
		}
		for (int l139 = 0; (l139 < 2); l139 = (l139 + 1)) {
			fRec66[l139] = 0.0f;
			
		}
		for (int l140 = 0; (l140 < 16384); l140 = (l140 + 1)) {
			fVec45[l140] = 0.0f;
			
		}
		for (int l141 = 0; (l141 < 2); l141 = (l141 + 1)) {
			fRec115[l141] = 0.0f;
			
		}
		for (int l142 = 0; (l142 < 2); l142 = (l142 + 1)) {
			fRec114[l142] = 0.0f;
			
		}
		for (int l143 = 0; (l143 < 2); l143 = (l143 + 1)) {
			fRec67[l143] = 0.0f;
			
		}
		for (int l144 = 0; (l144 < 16384); l144 = (l144 + 1)) {
			fVec46[l144] = 0.0f;
			
		}
		for (int l145 = 0; (l145 < 2); l145 = (l145 + 1)) {
			fRec116[l145] = 0.0f;
			
		}
		for (int l146 = 0; (l146 < 2); l146 = (l146 + 1)) {
			fRec49[l146] = 0.0f;
			
		}
		for (int l147 = 0; (l147 < 2); l147 = (l147 + 1)) {
			fRec47[l147] = 0.0f;
			
		}
		for (int l148 = 0; (l148 < 16384); l148 = (l148 + 1)) {
			fVec47[l148] = 0.0f;
			
		}
		for (int l149 = 0; (l149 < 2); l149 = (l149 + 1)) {
			fRec118[l149] = 0.0f;
			
		}
		for (int l150 = 0; (l150 < 2); l150 = (l150 + 1)) {
			fRec117[l150] = 0.0f;
			
		}
		for (int l151 = 0; (l151 < 2); l151 = (l151 + 1)) {
			fRec48[l151] = 0.0f;
			
		}
		for (int l152 = 0; (l152 < 16384); l152 = (l152 + 1)) {
			fVec48[l152] = 0.0f;
			
		}
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			fRec119[l153] = 0.0f;
			
		}
		for (int l154 = 0; (l154 < 2); l154 = (l154 + 1)) {
			fRec16[l154] = 0.0f;
			
		}
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fRec14[l155] = 0.0f;
			
		}
		for (int l156 = 0; (l156 < 16384); l156 = (l156 + 1)) {
			fVec49[l156] = 0.0f;
			
		}
		for (int l157 = 0; (l157 < 2); l157 = (l157 + 1)) {
			fRec121[l157] = 0.0f;
			
		}
		for (int l158 = 0; (l158 < 2); l158 = (l158 + 1)) {
			fRec120[l158] = 0.0f;
			
		}
		for (int l159 = 0; (l159 < 2); l159 = (l159 + 1)) {
			fRec15[l159] = 0.0f;
			
		}
		for (int l160 = 0; (l160 < 16384); l160 = (l160 + 1)) {
			fVec50[l160] = 0.0f;
			
		}
		for (int l161 = 0; (l161 < 2); l161 = (l161 + 1)) {
			fRec133[l161] = 0.0f;
			
		}
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			fRec131[l162] = 0.0f;
			
		}
		for (int l163 = 0; (l163 < 16384); l163 = (l163 + 1)) {
			fVec51[l163] = 0.0f;
			
		}
		for (int l164 = 0; (l164 < 2); l164 = (l164 + 1)) {
			fRec134[l164] = 0.0f;
			
		}
		for (int l165 = 0; (l165 < 2); l165 = (l165 + 1)) {
			fRec132[l165] = 0.0f;
			
		}
		for (int l166 = 0; (l166 < 16384); l166 = (l166 + 1)) {
			fVec52[l166] = 0.0f;
			
		}
		for (int l167 = 0; (l167 < 2); l167 = (l167 + 1)) {
			fRec130[l167] = 0.0f;
			
		}
		for (int l168 = 0; (l168 < 2); l168 = (l168 + 1)) {
			fRec128[l168] = 0.0f;
			
		}
		for (int l169 = 0; (l169 < 16384); l169 = (l169 + 1)) {
			fVec53[l169] = 0.0f;
			
		}
		for (int l170 = 0; (l170 < 2); l170 = (l170 + 1)) {
			fRec135[l170] = 0.0f;
			
		}
		for (int l171 = 0; (l171 < 2); l171 = (l171 + 1)) {
			fRec129[l171] = 0.0f;
			
		}
		for (int l172 = 0; (l172 < 16384); l172 = (l172 + 1)) {
			fVec54[l172] = 0.0f;
			
		}
		for (int l173 = 0; (l173 < 2); l173 = (l173 + 1)) {
			fRec127[l173] = 0.0f;
			
		}
		for (int l174 = 0; (l174 < 2); l174 = (l174 + 1)) {
			fRec125[l174] = 0.0f;
			
		}
		for (int l175 = 0; (l175 < 16384); l175 = (l175 + 1)) {
			fVec55[l175] = 0.0f;
			
		}
		for (int l176 = 0; (l176 < 2); l176 = (l176 + 1)) {
			fRec136[l176] = 0.0f;
			
		}
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fRec126[l177] = 0.0f;
			
		}
		for (int l178 = 0; (l178 < 16384); l178 = (l178 + 1)) {
			fVec56[l178] = 0.0f;
			
		}
		for (int l179 = 0; (l179 < 2); l179 = (l179 + 1)) {
			fRec124[l179] = 0.0f;
			
		}
		for (int l180 = 0; (l180 < 2); l180 = (l180 + 1)) {
			fRec122[l180] = 0.0f;
			
		}
		for (int l181 = 0; (l181 < 16384); l181 = (l181 + 1)) {
			fVec57[l181] = 0.0f;
			
		}
		for (int l182 = 0; (l182 < 2); l182 = (l182 + 1)) {
			fRec137[l182] = 0.0f;
			
		}
		for (int l183 = 0; (l183 < 2); l183 = (l183 + 1)) {
			fRec123[l183] = 0.0f;
			
		}
		for (int l184 = 0; (l184 < 16384); l184 = (l184 + 1)) {
			fVec58[l184] = 0.0f;
			
		}
		for (int l185 = 0; (l185 < 2); l185 = (l185 + 1)) {
			fRec149[l185] = 0.0f;
			
		}
		for (int l186 = 0; (l186 < 2); l186 = (l186 + 1)) {
			fRec147[l186] = 0.0f;
			
		}
		for (int l187 = 0; (l187 < 16384); l187 = (l187 + 1)) {
			fVec59[l187] = 0.0f;
			
		}
		for (int l188 = 0; (l188 < 2); l188 = (l188 + 1)) {
			fRec150[l188] = 0.0f;
			
		}
		for (int l189 = 0; (l189 < 2); l189 = (l189 + 1)) {
			fRec148[l189] = 0.0f;
			
		}
		for (int l190 = 0; (l190 < 16384); l190 = (l190 + 1)) {
			fVec60[l190] = 0.0f;
			
		}
		for (int l191 = 0; (l191 < 2); l191 = (l191 + 1)) {
			fRec146[l191] = 0.0f;
			
		}
		for (int l192 = 0; (l192 < 2); l192 = (l192 + 1)) {
			fRec144[l192] = 0.0f;
			
		}
		for (int l193 = 0; (l193 < 16384); l193 = (l193 + 1)) {
			fVec61[l193] = 0.0f;
			
		}
		for (int l194 = 0; (l194 < 2); l194 = (l194 + 1)) {
			fRec151[l194] = 0.0f;
			
		}
		for (int l195 = 0; (l195 < 2); l195 = (l195 + 1)) {
			fRec145[l195] = 0.0f;
			
		}
		for (int l196 = 0; (l196 < 16384); l196 = (l196 + 1)) {
			fVec62[l196] = 0.0f;
			
		}
		for (int l197 = 0; (l197 < 2); l197 = (l197 + 1)) {
			fRec143[l197] = 0.0f;
			
		}
		for (int l198 = 0; (l198 < 2); l198 = (l198 + 1)) {
			fRec141[l198] = 0.0f;
			
		}
		for (int l199 = 0; (l199 < 16384); l199 = (l199 + 1)) {
			fVec63[l199] = 0.0f;
			
		}
		for (int l200 = 0; (l200 < 2); l200 = (l200 + 1)) {
			fRec152[l200] = 0.0f;
			
		}
		for (int l201 = 0; (l201 < 2); l201 = (l201 + 1)) {
			fRec142[l201] = 0.0f;
			
		}
		for (int l202 = 0; (l202 < 16384); l202 = (l202 + 1)) {
			fVec64[l202] = 0.0f;
			
		}
		for (int l203 = 0; (l203 < 2); l203 = (l203 + 1)) {
			fRec140[l203] = 0.0f;
			
		}
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fRec138[l204] = 0.0f;
			
		}
		for (int l205 = 0; (l205 < 16384); l205 = (l205 + 1)) {
			fVec65[l205] = 0.0f;
			
		}
		for (int l206 = 0; (l206 < 2); l206 = (l206 + 1)) {
			fRec153[l206] = 0.0f;
			
		}
		for (int l207 = 0; (l207 < 2); l207 = (l207 + 1)) {
			fRec139[l207] = 0.0f;
			
		}
		for (int l208 = 0; (l208 < 16384); l208 = (l208 + 1)) {
			fVec66[l208] = 0.0f;
			
		}
		for (int l209 = 0; (l209 < 2); l209 = (l209 + 1)) {
			fRec154[l209] = 0.0f;
			
		}
		for (int l210 = 0; (l210 < 2); l210 = (l210 + 1)) {
			fRec13[l210] = 0.0f;
			
		}
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			fRec11[l211] = 0.0f;
			
		}
		for (int l212 = 0; (l212 < 16384); l212 = (l212 + 1)) {
			fVec67[l212] = 0.0f;
			
		}
		for (int l213 = 0; (l213 < 2); l213 = (l213 + 1)) {
			fRec156[l213] = 0.0f;
			
		}
		for (int l214 = 0; (l214 < 2); l214 = (l214 + 1)) {
			fRec155[l214] = 0.0f;
			
		}
		for (int l215 = 0; (l215 < 2); l215 = (l215 + 1)) {
			fRec12[l215] = 0.0f;
			
		}
		for (int l216 = 0; (l216 < 16384); l216 = (l216 + 1)) {
			fVec68[l216] = 0.0f;
			
		}
		for (int l217 = 0; (l217 < 2); l217 = (l217 + 1)) {
			fRec168[l217] = 0.0f;
			
		}
		for (int l218 = 0; (l218 < 2); l218 = (l218 + 1)) {
			fRec166[l218] = 0.0f;
			
		}
		for (int l219 = 0; (l219 < 16384); l219 = (l219 + 1)) {
			fVec69[l219] = 0.0f;
			
		}
		for (int l220 = 0; (l220 < 2); l220 = (l220 + 1)) {
			fRec169[l220] = 0.0f;
			
		}
		for (int l221 = 0; (l221 < 2); l221 = (l221 + 1)) {
			fRec167[l221] = 0.0f;
			
		}
		for (int l222 = 0; (l222 < 16384); l222 = (l222 + 1)) {
			fVec70[l222] = 0.0f;
			
		}
		for (int l223 = 0; (l223 < 2); l223 = (l223 + 1)) {
			fRec165[l223] = 0.0f;
			
		}
		for (int l224 = 0; (l224 < 2); l224 = (l224 + 1)) {
			fRec163[l224] = 0.0f;
			
		}
		for (int l225 = 0; (l225 < 16384); l225 = (l225 + 1)) {
			fVec71[l225] = 0.0f;
			
		}
		for (int l226 = 0; (l226 < 2); l226 = (l226 + 1)) {
			fRec170[l226] = 0.0f;
			
		}
		for (int l227 = 0; (l227 < 2); l227 = (l227 + 1)) {
			fRec164[l227] = 0.0f;
			
		}
		for (int l228 = 0; (l228 < 16384); l228 = (l228 + 1)) {
			fVec72[l228] = 0.0f;
			
		}
		for (int l229 = 0; (l229 < 2); l229 = (l229 + 1)) {
			fRec162[l229] = 0.0f;
			
		}
		for (int l230 = 0; (l230 < 2); l230 = (l230 + 1)) {
			fRec160[l230] = 0.0f;
			
		}
		for (int l231 = 0; (l231 < 16384); l231 = (l231 + 1)) {
			fVec73[l231] = 0.0f;
			
		}
		for (int l232 = 0; (l232 < 2); l232 = (l232 + 1)) {
			fRec171[l232] = 0.0f;
			
		}
		for (int l233 = 0; (l233 < 2); l233 = (l233 + 1)) {
			fRec161[l233] = 0.0f;
			
		}
		for (int l234 = 0; (l234 < 16384); l234 = (l234 + 1)) {
			fVec74[l234] = 0.0f;
			
		}
		for (int l235 = 0; (l235 < 2); l235 = (l235 + 1)) {
			fRec159[l235] = 0.0f;
			
		}
		for (int l236 = 0; (l236 < 2); l236 = (l236 + 1)) {
			fRec157[l236] = 0.0f;
			
		}
		for (int l237 = 0; (l237 < 16384); l237 = (l237 + 1)) {
			fVec75[l237] = 0.0f;
			
		}
		for (int l238 = 0; (l238 < 2); l238 = (l238 + 1)) {
			fRec172[l238] = 0.0f;
			
		}
		for (int l239 = 0; (l239 < 2); l239 = (l239 + 1)) {
			fRec158[l239] = 0.0f;
			
		}
		for (int l240 = 0; (l240 < 16384); l240 = (l240 + 1)) {
			fVec76[l240] = 0.0f;
			
		}
		for (int l241 = 0; (l241 < 2); l241 = (l241 + 1)) {
			fRec184[l241] = 0.0f;
			
		}
		for (int l242 = 0; (l242 < 2); l242 = (l242 + 1)) {
			fRec182[l242] = 0.0f;
			
		}
		for (int l243 = 0; (l243 < 16384); l243 = (l243 + 1)) {
			fVec77[l243] = 0.0f;
			
		}
		for (int l244 = 0; (l244 < 2); l244 = (l244 + 1)) {
			fRec185[l244] = 0.0f;
			
		}
		for (int l245 = 0; (l245 < 2); l245 = (l245 + 1)) {
			fRec183[l245] = 0.0f;
			
		}
		for (int l246 = 0; (l246 < 16384); l246 = (l246 + 1)) {
			fVec78[l246] = 0.0f;
			
		}
		for (int l247 = 0; (l247 < 2); l247 = (l247 + 1)) {
			fRec181[l247] = 0.0f;
			
		}
		for (int l248 = 0; (l248 < 2); l248 = (l248 + 1)) {
			fRec179[l248] = 0.0f;
			
		}
		for (int l249 = 0; (l249 < 16384); l249 = (l249 + 1)) {
			fVec79[l249] = 0.0f;
			
		}
		for (int l250 = 0; (l250 < 2); l250 = (l250 + 1)) {
			fRec186[l250] = 0.0f;
			
		}
		for (int l251 = 0; (l251 < 2); l251 = (l251 + 1)) {
			fRec180[l251] = 0.0f;
			
		}
		for (int l252 = 0; (l252 < 16384); l252 = (l252 + 1)) {
			fVec80[l252] = 0.0f;
			
		}
		for (int l253 = 0; (l253 < 2); l253 = (l253 + 1)) {
			fRec178[l253] = 0.0f;
			
		}
		for (int l254 = 0; (l254 < 2); l254 = (l254 + 1)) {
			fRec176[l254] = 0.0f;
			
		}
		for (int l255 = 0; (l255 < 16384); l255 = (l255 + 1)) {
			fVec81[l255] = 0.0f;
			
		}
		for (int l256 = 0; (l256 < 2); l256 = (l256 + 1)) {
			fRec187[l256] = 0.0f;
			
		}
		for (int l257 = 0; (l257 < 2); l257 = (l257 + 1)) {
			fRec177[l257] = 0.0f;
			
		}
		for (int l258 = 0; (l258 < 16384); l258 = (l258 + 1)) {
			fVec82[l258] = 0.0f;
			
		}
		for (int l259 = 0; (l259 < 2); l259 = (l259 + 1)) {
			fRec175[l259] = 0.0f;
			
		}
		for (int l260 = 0; (l260 < 2); l260 = (l260 + 1)) {
			fRec173[l260] = 0.0f;
			
		}
		for (int l261 = 0; (l261 < 16384); l261 = (l261 + 1)) {
			fVec83[l261] = 0.0f;
			
		}
		for (int l262 = 0; (l262 < 2); l262 = (l262 + 1)) {
			fRec188[l262] = 0.0f;
			
		}
		for (int l263 = 0; (l263 < 2); l263 = (l263 + 1)) {
			fRec174[l263] = 0.0f;
			
		}
		for (int l264 = 0; (l264 < 16384); l264 = (l264 + 1)) {
			fVec84[l264] = 0.0f;
			
		}
		for (int l265 = 0; (l265 < 2); l265 = (l265 + 1)) {
			fRec9[l265] = 0.0f;
			
		}
		for (int l266 = 0; (l266 < 2); l266 = (l266 + 1)) {
			fRec7[l266] = 0.0f;
			
		}
		for (int l267 = 0; (l267 < 2); l267 = (l267 + 1)) {
			fRec190[l267] = 0.0f;
			
		}
		for (int l268 = 0; (l268 < 16384); l268 = (l268 + 1)) {
			fVec85[l268] = 0.0f;
			
		}
		for (int l269 = 0; (l269 < 2); l269 = (l269 + 1)) {
			fRec189[l269] = 0.0f;
			
		}
		for (int l270 = 0; (l270 < 2); l270 = (l270 + 1)) {
			fRec8[l270] = 0.0f;
			
		}
		for (int l271 = 0; (l271 < 16384); l271 = (l271 + 1)) {
			fVec86[l271] = 0.0f;
			
		}
		for (int l272 = 0; (l272 < 2); l272 = (l272 + 1)) {
			fRec202[l272] = 0.0f;
			
		}
		for (int l273 = 0; (l273 < 2); l273 = (l273 + 1)) {
			fRec200[l273] = 0.0f;
			
		}
		for (int l274 = 0; (l274 < 16384); l274 = (l274 + 1)) {
			fVec87[l274] = 0.0f;
			
		}
		for (int l275 = 0; (l275 < 2); l275 = (l275 + 1)) {
			fRec203[l275] = 0.0f;
			
		}
		for (int l276 = 0; (l276 < 2); l276 = (l276 + 1)) {
			fRec201[l276] = 0.0f;
			
		}
		for (int l277 = 0; (l277 < 16384); l277 = (l277 + 1)) {
			fVec88[l277] = 0.0f;
			
		}
		for (int l278 = 0; (l278 < 2); l278 = (l278 + 1)) {
			fRec199[l278] = 0.0f;
			
		}
		for (int l279 = 0; (l279 < 2); l279 = (l279 + 1)) {
			fRec197[l279] = 0.0f;
			
		}
		for (int l280 = 0; (l280 < 16384); l280 = (l280 + 1)) {
			fVec89[l280] = 0.0f;
			
		}
		for (int l281 = 0; (l281 < 2); l281 = (l281 + 1)) {
			fRec204[l281] = 0.0f;
			
		}
		for (int l282 = 0; (l282 < 2); l282 = (l282 + 1)) {
			fRec198[l282] = 0.0f;
			
		}
		for (int l283 = 0; (l283 < 16384); l283 = (l283 + 1)) {
			fVec90[l283] = 0.0f;
			
		}
		for (int l284 = 0; (l284 < 2); l284 = (l284 + 1)) {
			fRec196[l284] = 0.0f;
			
		}
		for (int l285 = 0; (l285 < 2); l285 = (l285 + 1)) {
			fRec194[l285] = 0.0f;
			
		}
		for (int l286 = 0; (l286 < 16384); l286 = (l286 + 1)) {
			fVec91[l286] = 0.0f;
			
		}
		for (int l287 = 0; (l287 < 2); l287 = (l287 + 1)) {
			fRec205[l287] = 0.0f;
			
		}
		for (int l288 = 0; (l288 < 2); l288 = (l288 + 1)) {
			fRec195[l288] = 0.0f;
			
		}
		for (int l289 = 0; (l289 < 16384); l289 = (l289 + 1)) {
			fVec92[l289] = 0.0f;
			
		}
		for (int l290 = 0; (l290 < 2); l290 = (l290 + 1)) {
			fRec193[l290] = 0.0f;
			
		}
		for (int l291 = 0; (l291 < 2); l291 = (l291 + 1)) {
			fRec191[l291] = 0.0f;
			
		}
		for (int l292 = 0; (l292 < 16384); l292 = (l292 + 1)) {
			fVec93[l292] = 0.0f;
			
		}
		for (int l293 = 0; (l293 < 2); l293 = (l293 + 1)) {
			fRec206[l293] = 0.0f;
			
		}
		for (int l294 = 0; (l294 < 2); l294 = (l294 + 1)) {
			fRec192[l294] = 0.0f;
			
		}
		for (int l295 = 0; (l295 < 16384); l295 = (l295 + 1)) {
			fVec94[l295] = 0.0f;
			
		}
		for (int l296 = 0; (l296 < 2); l296 = (l296 + 1)) {
			fRec218[l296] = 0.0f;
			
		}
		for (int l297 = 0; (l297 < 2); l297 = (l297 + 1)) {
			fRec216[l297] = 0.0f;
			
		}
		for (int l298 = 0; (l298 < 16384); l298 = (l298 + 1)) {
			fVec95[l298] = 0.0f;
			
		}
		for (int l299 = 0; (l299 < 2); l299 = (l299 + 1)) {
			fRec219[l299] = 0.0f;
			
		}
		for (int l300 = 0; (l300 < 2); l300 = (l300 + 1)) {
			fRec217[l300] = 0.0f;
			
		}
		for (int l301 = 0; (l301 < 16384); l301 = (l301 + 1)) {
			fVec96[l301] = 0.0f;
			
		}
		for (int l302 = 0; (l302 < 2); l302 = (l302 + 1)) {
			fRec215[l302] = 0.0f;
			
		}
		for (int l303 = 0; (l303 < 2); l303 = (l303 + 1)) {
			fRec213[l303] = 0.0f;
			
		}
		for (int l304 = 0; (l304 < 16384); l304 = (l304 + 1)) {
			fVec97[l304] = 0.0f;
			
		}
		for (int l305 = 0; (l305 < 2); l305 = (l305 + 1)) {
			fRec220[l305] = 0.0f;
			
		}
		for (int l306 = 0; (l306 < 2); l306 = (l306 + 1)) {
			fRec214[l306] = 0.0f;
			
		}
		for (int l307 = 0; (l307 < 16384); l307 = (l307 + 1)) {
			fVec98[l307] = 0.0f;
			
		}
		for (int l308 = 0; (l308 < 2); l308 = (l308 + 1)) {
			fRec212[l308] = 0.0f;
			
		}
		for (int l309 = 0; (l309 < 2); l309 = (l309 + 1)) {
			fRec210[l309] = 0.0f;
			
		}
		for (int l310 = 0; (l310 < 16384); l310 = (l310 + 1)) {
			fVec99[l310] = 0.0f;
			
		}
		for (int l311 = 0; (l311 < 2); l311 = (l311 + 1)) {
			fRec221[l311] = 0.0f;
			
		}
		for (int l312 = 0; (l312 < 2); l312 = (l312 + 1)) {
			fRec211[l312] = 0.0f;
			
		}
		for (int l313 = 0; (l313 < 16384); l313 = (l313 + 1)) {
			fVec100[l313] = 0.0f;
			
		}
		for (int l314 = 0; (l314 < 2); l314 = (l314 + 1)) {
			fRec209[l314] = 0.0f;
			
		}
		for (int l315 = 0; (l315 < 2); l315 = (l315 + 1)) {
			fRec207[l315] = 0.0f;
			
		}
		for (int l316 = 0; (l316 < 16384); l316 = (l316 + 1)) {
			fVec101[l316] = 0.0f;
			
		}
		for (int l317 = 0; (l317 < 2); l317 = (l317 + 1)) {
			fRec222[l317] = 0.0f;
			
		}
		for (int l318 = 0; (l318 < 2); l318 = (l318 + 1)) {
			fRec208[l318] = 0.0f;
			
		}
		for (int l319 = 0; (l319 < 16384); l319 = (l319 + 1)) {
			fVec102[l319] = 0.0f;
			
		}
		for (int l320 = 0; (l320 < 2); l320 = (l320 + 1)) {
			fRec223[l320] = 0.0f;
			
		}
		for (int l321 = 0; (l321 < 2); l321 = (l321 + 1)) {
			fRec6[l321] = 0.0f;
			
		}
		for (int l322 = 0; (l322 < 2); l322 = (l322 + 1)) {
			fRec4[l322] = 0.0f;
			
		}
		for (int l323 = 0; (l323 < 16384); l323 = (l323 + 1)) {
			fVec103[l323] = 0.0f;
			
		}
		for (int l324 = 0; (l324 < 2); l324 = (l324 + 1)) {
			fRec225[l324] = 0.0f;
			
		}
		for (int l325 = 0; (l325 < 2); l325 = (l325 + 1)) {
			fRec224[l325] = 0.0f;
			
		}
		for (int l326 = 0; (l326 < 2); l326 = (l326 + 1)) {
			fRec5[l326] = 0.0f;
			
		}
		for (int l327 = 0; (l327 < 16384); l327 = (l327 + 1)) {
			fVec104[l327] = 0.0f;
			
		}
		for (int l328 = 0; (l328 < 2); l328 = (l328 + 1)) {
			fRec237[l328] = 0.0f;
			
		}
		for (int l329 = 0; (l329 < 2); l329 = (l329 + 1)) {
			fRec235[l329] = 0.0f;
			
		}
		for (int l330 = 0; (l330 < 16384); l330 = (l330 + 1)) {
			fVec105[l330] = 0.0f;
			
		}
		for (int l331 = 0; (l331 < 2); l331 = (l331 + 1)) {
			fRec238[l331] = 0.0f;
			
		}
		for (int l332 = 0; (l332 < 2); l332 = (l332 + 1)) {
			fRec236[l332] = 0.0f;
			
		}
		for (int l333 = 0; (l333 < 16384); l333 = (l333 + 1)) {
			fVec106[l333] = 0.0f;
			
		}
		for (int l334 = 0; (l334 < 2); l334 = (l334 + 1)) {
			fRec234[l334] = 0.0f;
			
		}
		for (int l335 = 0; (l335 < 2); l335 = (l335 + 1)) {
			fRec232[l335] = 0.0f;
			
		}
		for (int l336 = 0; (l336 < 16384); l336 = (l336 + 1)) {
			fVec107[l336] = 0.0f;
			
		}
		for (int l337 = 0; (l337 < 2); l337 = (l337 + 1)) {
			fRec239[l337] = 0.0f;
			
		}
		for (int l338 = 0; (l338 < 2); l338 = (l338 + 1)) {
			fRec233[l338] = 0.0f;
			
		}
		for (int l339 = 0; (l339 < 16384); l339 = (l339 + 1)) {
			fVec108[l339] = 0.0f;
			
		}
		for (int l340 = 0; (l340 < 2); l340 = (l340 + 1)) {
			fRec231[l340] = 0.0f;
			
		}
		for (int l341 = 0; (l341 < 2); l341 = (l341 + 1)) {
			fRec229[l341] = 0.0f;
			
		}
		for (int l342 = 0; (l342 < 16384); l342 = (l342 + 1)) {
			fVec109[l342] = 0.0f;
			
		}
		for (int l343 = 0; (l343 < 2); l343 = (l343 + 1)) {
			fRec240[l343] = 0.0f;
			
		}
		for (int l344 = 0; (l344 < 2); l344 = (l344 + 1)) {
			fRec230[l344] = 0.0f;
			
		}
		for (int l345 = 0; (l345 < 16384); l345 = (l345 + 1)) {
			fVec110[l345] = 0.0f;
			
		}
		for (int l346 = 0; (l346 < 2); l346 = (l346 + 1)) {
			fRec228[l346] = 0.0f;
			
		}
		for (int l347 = 0; (l347 < 2); l347 = (l347 + 1)) {
			fRec226[l347] = 0.0f;
			
		}
		for (int l348 = 0; (l348 < 16384); l348 = (l348 + 1)) {
			fVec111[l348] = 0.0f;
			
		}
		for (int l349 = 0; (l349 < 2); l349 = (l349 + 1)) {
			fRec241[l349] = 0.0f;
			
		}
		for (int l350 = 0; (l350 < 2); l350 = (l350 + 1)) {
			fRec227[l350] = 0.0f;
			
		}
		for (int l351 = 0; (l351 < 16384); l351 = (l351 + 1)) {
			fVec112[l351] = 0.0f;
			
		}
		for (int l352 = 0; (l352 < 2); l352 = (l352 + 1)) {
			fRec253[l352] = 0.0f;
			
		}
		for (int l353 = 0; (l353 < 2); l353 = (l353 + 1)) {
			fRec251[l353] = 0.0f;
			
		}
		for (int l354 = 0; (l354 < 16384); l354 = (l354 + 1)) {
			fVec113[l354] = 0.0f;
			
		}
		for (int l355 = 0; (l355 < 2); l355 = (l355 + 1)) {
			fRec254[l355] = 0.0f;
			
		}
		for (int l356 = 0; (l356 < 2); l356 = (l356 + 1)) {
			fRec252[l356] = 0.0f;
			
		}
		for (int l357 = 0; (l357 < 16384); l357 = (l357 + 1)) {
			fVec114[l357] = 0.0f;
			
		}
		for (int l358 = 0; (l358 < 2); l358 = (l358 + 1)) {
			fRec250[l358] = 0.0f;
			
		}
		for (int l359 = 0; (l359 < 2); l359 = (l359 + 1)) {
			fRec248[l359] = 0.0f;
			
		}
		for (int l360 = 0; (l360 < 16384); l360 = (l360 + 1)) {
			fVec115[l360] = 0.0f;
			
		}
		for (int l361 = 0; (l361 < 2); l361 = (l361 + 1)) {
			fRec255[l361] = 0.0f;
			
		}
		for (int l362 = 0; (l362 < 2); l362 = (l362 + 1)) {
			fRec249[l362] = 0.0f;
			
		}
		for (int l363 = 0; (l363 < 16384); l363 = (l363 + 1)) {
			fVec116[l363] = 0.0f;
			
		}
		for (int l364 = 0; (l364 < 2); l364 = (l364 + 1)) {
			fRec247[l364] = 0.0f;
			
		}
		for (int l365 = 0; (l365 < 2); l365 = (l365 + 1)) {
			fRec245[l365] = 0.0f;
			
		}
		for (int l366 = 0; (l366 < 16384); l366 = (l366 + 1)) {
			fVec117[l366] = 0.0f;
			
		}
		for (int l367 = 0; (l367 < 2); l367 = (l367 + 1)) {
			fRec256[l367] = 0.0f;
			
		}
		for (int l368 = 0; (l368 < 2); l368 = (l368 + 1)) {
			fRec246[l368] = 0.0f;
			
		}
		for (int l369 = 0; (l369 < 16384); l369 = (l369 + 1)) {
			fVec118[l369] = 0.0f;
			
		}
		for (int l370 = 0; (l370 < 2); l370 = (l370 + 1)) {
			fRec244[l370] = 0.0f;
			
		}
		for (int l371 = 0; (l371 < 2); l371 = (l371 + 1)) {
			fRec242[l371] = 0.0f;
			
		}
		for (int l372 = 0; (l372 < 16384); l372 = (l372 + 1)) {
			fVec119[l372] = 0.0f;
			
		}
		for (int l373 = 0; (l373 < 2); l373 = (l373 + 1)) {
			fRec257[l373] = 0.0f;
			
		}
		for (int l374 = 0; (l374 < 2); l374 = (l374 + 1)) {
			fRec243[l374] = 0.0f;
			
		}
		for (int l375 = 0; (l375 < 2); l375 = (l375 + 1)) {
			fRec3[l375] = 0.0f;
			
		}
		for (int l376 = 0; (l376 < 1024); l376 = (l376 + 1)) {
			fRec1[l376] = 0.0f;
			
		}
		for (int l377 = 0; (l377 < 2); l377 = (l377 + 1)) {
			fRec258[l377] = 0.0f;
			
		}
		for (int l378 = 0; (l378 < 1024); l378 = (l378 + 1)) {
			fRec2[l378] = 0.0f;
			
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
	
	virtual fx_greyhole* clone() {
		return new fx_greyhole();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("GreyholeRaw");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addHorizontalSlider("damping", &fHslider1, 0.0f, 0.0f, 0.99000001f, 0.00100000005f);
		ui_interface->addHorizontalSlider("delaytime", &fHslider5, 0.200000003f, 0.00100000005f, 1.45000005f, 9.99999975e-05f);
		ui_interface->addHorizontalSlider("diffusion", &fHslider2, 0.5f, 0.0f, 0.99000001f, 9.99999975e-05f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("feedback", &fHslider4, 0.899999976f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("moddepth", &fHslider6, 0.100000001f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("modfreq", &fHslider7, 2.0f, 0.0f, 10.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("size", &fHslider3, 1.0f, 0.5f, 3.0f, 9.99999975e-05f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = (0.00100000005f * float(fHslider0));
		float fSlow2 = float(fHslider1);
		float fSlow3 = float(fHslider2);
		float fSlow4 = float(fHslider3);
		float fSlow5 = (61.0f * fSlow4);
		int iSlow6 = int(primes(int(fSlow5)));
		float fSlow7 = (0.00100000005f * float(iSlow6));
		float fSlow8 = float(fHslider4);
		float fSlow9 = std::floor(std::min(65533.0f, (fConst0 * float(fHslider5))));
		float fSlow10 = float(fHslider6);
		float fSlow11 = float(fHslider7);
		float fSlow12 = (23.0f * fSlow4);
		int iSlow13 = int(primes(int(fSlow12)));
		float fSlow14 = (0.00100000005f * float(iSlow13));
		float fSlow15 = (49.0f * fSlow4);
		int iSlow16 = int(primes(int(fSlow15)));
		float fSlow17 = (9.99999975e-05f * float(iSlow16));
		int iSlow18 = int(primes(int((fSlow15 + 10.0f))));
		float fSlow19 = (9.99999975e-05f * float(iSlow18));
		float fSlow20 = (36.0f * fSlow4);
		int iSlow21 = int(primes(int(fSlow20)));
		float fSlow22 = (0.00100000005f * float(iSlow21));
		int iSlow23 = int(primes(int((fSlow20 + 10.0f))));
		float fSlow24 = (0.00100000005f * float(iSlow23));
		int iSlow25 = int(primes(int((fSlow12 + 10.0f))));
		float fSlow26 = (0.00100000005f * float(iSlow25));
		int iSlow27 = int(primes(int((10.0f * fSlow4))));
		float fSlow28 = (0.00100000005f * float(iSlow27));
		int iSlow29 = int(primes(int((10.0f * (fSlow4 + 1.0f)))));
		float fSlow30 = (0.00100000005f * float(iSlow29));
		float fSlow31 = (55.0f * fSlow4);
		int iSlow32 = int(primes(int(fSlow31)));
		float fSlow33 = (0.00100000005f * float(iSlow32));
		float fSlow34 = (68.0f * fSlow4);
		int iSlow35 = int(primes(int(fSlow34)));
		float fSlow36 = (9.99999975e-05f * float(iSlow35));
		int iSlow37 = int(primes(int((fSlow34 + 10.0f))));
		float fSlow38 = (9.99999975e-05f * float(iSlow37));
		int iSlow39 = int(primes(int((fSlow31 + 10.0f))));
		float fSlow40 = (0.00100000005f * float(iSlow39));
		float fSlow41 = (42.0f * fSlow4);
		int iSlow42 = int(primes(int(fSlow41)));
		float fSlow43 = (0.00100000005f * float(iSlow42));
		int iSlow44 = int(primes(int((fSlow41 + 10.0f))));
		float fSlow45 = (0.00100000005f * float(iSlow44));
		float fSlow46 = (29.0f * fSlow4);
		int iSlow47 = int(primes(int(fSlow46)));
		float fSlow48 = (0.00100000005f * float(iSlow47));
		int iSlow49 = int(primes(int((fSlow46 + 10.0f))));
		float fSlow50 = (0.00100000005f * float(iSlow49));
		float fSlow51 = (87.0f * fSlow4);
		int iSlow52 = int(primes(int(fSlow51)));
		float fSlow53 = (9.99999975e-05f * float(iSlow52));
		int iSlow54 = int(primes(int((fSlow51 + 10.0f))));
		float fSlow55 = (9.99999975e-05f * float(iSlow54));
		float fSlow56 = (74.0f * fSlow4);
		int iSlow57 = int(primes(int(fSlow56)));
		float fSlow58 = (0.00100000005f * float(iSlow57));
		int iSlow59 = int(primes(int((fSlow56 + 10.0f))));
		float fSlow60 = (0.00100000005f * float(iSlow59));
		int iSlow61 = int(primes(int((fSlow5 + 10.0f))));
		float fSlow62 = (0.00100000005f * float(iSlow61));
		float fSlow63 = (48.0f * fSlow4);
		int iSlow64 = int(primes(int(fSlow63)));
		float fSlow65 = (0.00100000005f * float(iSlow64));
		int iSlow66 = int(primes(int((fSlow63 + 10.0f))));
		float fSlow67 = (0.00100000005f * float(iSlow66));
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			float fTemp1 = (iSlow0?0.0f:fTemp0);
			iVec0[0] = 1;
			fRec0[0] = (fSlow1 + (0.999000013f * fRec0[1]));
			float fTemp2 = (1.0f - fRec0[0]);
			fVec1[0] = fSlow2;
			float fTemp3 = (fSlow2 + fVec1[1]);
			float fTemp4 = (1.0f - (0.5f * fTemp3));
			fVec2[0] = fSlow3;
			float fTemp5 = (0.5f * (fSlow3 + fVec2[1]));
			float fTemp6 = std::cos(fTemp5);
			int iTemp7 = (1 - iVec0[1]);
			fRec10[0] = (fSlow7 + (0.999000013f * (float((iSlow6 * iTemp7)) + fRec10[1])));
			float fTemp8 = (fRec10[0] + -1.49998999f);
			float fTemp9 = std::floor(fTemp8);
			float fTemp10 = (fTemp9 + (2.0f - fRec10[0]));
			float fTemp11 = std::sin(fTemp5);
			float fTemp12 = (0.0f - fTemp11);
			float fTemp13 = (fTemp12 * fRec5[1]);
			float fTemp14 = (0.0f - fTemp5);
			float fTemp15 = std::sin(fTemp14);
			float fTemp16 = float(input1[i]);
			float fTemp17 = (iSlow0?0.0f:fTemp16);
			fVec3[0] = fSlow8;
			float fTemp18 = (fSlow8 + fVec3[1]);
			float fTemp19 = ((fRec17[1] != 0.0f)?(((fRec18[1] > 0.0f) & (fRec18[1] < 1.0f))?fRec17[1]:0.0f):(((fRec18[1] == 0.0f) & (fSlow9 != fRec19[1]))?4.53514731e-05f:(((fRec18[1] == 1.0f) & (fSlow9 != fRec20[1]))?-4.53514731e-05f:0.0f)));
			fRec17[0] = fTemp19;
			fRec18[0] = std::max(0.0f, std::min(1.0f, (fRec18[1] + fTemp19)));
			fRec19[0] = (((fRec18[1] >= 1.0f) & (fRec20[1] != fSlow9))?fSlow9:fRec19[1]);
			fRec20[0] = (((fRec18[1] <= 0.0f) & (fRec19[1] != fSlow9))?fSlow9:fRec20[1]);
			float fTemp20 = (1.0f - fRec18[0]);
			fVec4[0] = fSlow10;
			float fTemp21 = (fSlow10 + fVec4[1]);
			fVec5[0] = fSlow11;
			float fTemp22 = (fConst2 * (fSlow11 + fVec5[1]));
			float fTemp23 = std::sin(fTemp22);
			float fTemp24 = std::cos(fTemp22);
			fRec21[0] = ((fTemp23 * fRec22[1]) + (fTemp24 * fRec21[1]));
			fRec22[0] = (((fTemp24 * fRec22[1]) + ((0.0f - fTemp23) * fRec21[1])) + float(iTemp7));
			float fTemp25 = (fConst1 * (fTemp21 * (fRec21[0] + 1.0f)));
			float fTemp26 = (fTemp25 + 8.50000477f);
			float fTemp27 = std::floor(fTemp26);
			float fTemp28 = (fTemp25 + (7.0f - fTemp27));
			float fTemp29 = (fTemp25 + (6.0f - fTemp27));
			int iTemp30 = int(fTemp26);
			float fTemp31 = (fTemp25 + (8.0f - fTemp27));
			float fTemp32 = (((fTemp25 + (10.0f - fTemp27)) * (((((0.0f - (0.5f * fTemp28)) * (fTemp27 + (-8.0f - fTemp25))) * (0.0f - (0.333333343f * fTemp29))) * fRec2[((IOTA - (std::min(512, std::max(0, (iTemp30 + 1))) + 1)) & 1023)]) + ((fTemp25 + (9.0f - fTemp27)) * ((0.5f * (((0.0f - (0.5f * fTemp29)) * (fTemp27 + (-7.0f - fTemp25))) * fRec2[((IOTA - (std::min(512, std::max(0, (iTemp30 + 2))) + 1)) & 1023)])) + (fTemp31 * ((0.166666672f * ((fTemp27 + (-6.0f - fTemp25)) * fRec2[((IOTA - (std::min(512, std::max(0, (iTemp30 + 3))) + 1)) & 1023)])) + (0.0416666679f * (fTemp28 * fRec2[((IOTA - (std::min(512, std::max(0, (iTemp30 + 4))) + 1)) & 1023)])))))))) + (((((0.0f - (0.5f * fTemp31)) * (fTemp27 + (-9.0f - fTemp25))) * (0.0f - (0.333333343f * fTemp28))) * (0.0f - (0.25f * fTemp29))) * fRec2[((IOTA - (std::min(512, std::max(0, iTemp30)) + 1)) & 1023)]));
			fVec6[(IOTA & 131071)] = fTemp32;
			int iTemp33 = int(std::min(65536.0f, std::max(0.0f, fRec19[0])));
			int iTemp34 = int(std::min(65536.0f, std::max(0.0f, fRec20[0])));
			float fTemp35 = (fTemp17 + (0.5f * (fTemp18 * ((fTemp20 * fVec6[((IOTA - iTemp33) & 131071)]) + (fRec18[0] * fVec6[((IOTA - iTemp34) & 131071)])))));
			float fTemp36 = (fTemp11 * fTemp35);
			float fTemp37 = (fTemp6 * fTemp35);
			float fTemp38 = (fTemp37 + (fTemp12 * fRec24[1]));
			fRec29[0] = (fSlow14 + (0.999000013f * (float((iSlow13 * iTemp7)) + fRec29[1])));
			float fTemp39 = (fRec29[0] + -1.49998999f);
			float fTemp40 = std::floor(fTemp39);
			float fTemp41 = (fTemp40 + (2.0f - fRec29[0]));
			float fTemp42 = (fRec29[0] - fTemp40);
			float fTemp43 = (0.0f - (fTemp41 / fTemp42));
			float fTemp44 = ((fTemp12 * fRec27[1]) + (fTemp6 * fTemp38));
			float fTemp45 = ((fTemp12 * fRec31[1]) + (fTemp6 * fTemp44));
			fRec36[0] = (fSlow17 + (0.999899983f * (float((iSlow16 * iTemp7)) + fRec36[1])));
			float fTemp46 = (fRec36[0] + -1.49998999f);
			float fTemp47 = std::floor(fTemp46);
			float fTemp48 = (fTemp47 + (2.0f - fRec36[0]));
			float fTemp49 = (fRec36[0] - fTemp47);
			float fTemp50 = (0.0f - (fTemp48 / fTemp49));
			float fTemp51 = (0.0f - ((fTemp12 * fRec34[1]) + (fTemp6 * fTemp45)));
			fVec7[(IOTA & 16383)] = fTemp51;
			int iTemp52 = std::min(8192, std::max(0, int(fTemp46)));
			int iTemp53 = (iTemp52 + 1);
			fRec35[0] = (((fTemp50 * fRec35[1]) + fVec7[((IOTA - iTemp53) & 16383)]) + ((fTemp48 * fVec7[((IOTA - iTemp52) & 16383)]) / fTemp49));
			fRec33[0] = fRec35[0];
			fRec38[0] = (fSlow19 + (0.999899983f * (float((iSlow18 * iTemp7)) + fRec38[1])));
			float fTemp54 = (fRec38[0] + -1.49998999f);
			float fTemp55 = std::floor(fTemp54);
			float fTemp56 = (fTemp55 + (2.0f - fRec38[0]));
			float fTemp57 = (fRec38[0] - fTemp55);
			float fTemp58 = (0.0f - (fTemp56 / fTemp57));
			float fTemp59 = (fConst1 * (fTemp21 * (fRec22[0] + 1.0f)));
			float fTemp60 = (fTemp59 + 8.50000477f);
			float fTemp61 = std::floor(fTemp60);
			float fTemp62 = (fTemp59 + (8.0f - fTemp61));
			float fTemp63 = (fTemp59 + (7.0f - fTemp61));
			float fTemp64 = (fTemp59 + (6.0f - fTemp61));
			int iTemp65 = int(fTemp60);
			float fTemp66 = ((((((0.0f - (0.5f * fTemp62)) * (fTemp61 + (-9.0f - fTemp59))) * (0.0f - (0.333333343f * fTemp63))) * (0.0f - (0.25f * fTemp64))) * fRec1[((IOTA - (std::min(512, std::max(0, iTemp65)) + 1)) & 1023)]) + ((fTemp59 + (10.0f - fTemp61)) * (((((0.0f - (0.5f * fTemp63)) * (fTemp61 + (-8.0f - fTemp59))) * (0.0f - (0.333333343f * fTemp64))) * fRec1[((IOTA - (std::min(512, std::max(0, (iTemp65 + 1))) + 1)) & 1023)]) + ((fTemp59 + (9.0f - fTemp61)) * ((0.5f * (((0.0f - (0.5f * fTemp64)) * (fTemp61 + (-7.0f - fTemp59))) * fRec1[((IOTA - (std::min(512, std::max(0, (iTemp65 + 2))) + 1)) & 1023)])) + (fTemp62 * ((0.166666672f * ((fTemp61 + (-6.0f - fTemp59)) * fRec1[((IOTA - (std::min(512, std::max(0, (iTemp65 + 3))) + 1)) & 1023)])) + (0.0416666679f * (fTemp63 * fRec1[((IOTA - (std::min(512, std::max(0, (iTemp65 + 4))) + 1)) & 1023)])))))))));
			fVec8[(IOTA & 131071)] = fTemp66;
			float fTemp67 = (fTemp1 + (0.5f * (fTemp18 * ((fTemp20 * fVec8[((IOTA - iTemp33) & 131071)]) + (fRec18[0] * fVec8[((IOTA - iTemp34) & 131071)])))));
			float fTemp68 = (fTemp6 * fTemp67);
			float fTemp69 = (fTemp68 + (fTemp12 * fRec23[1]));
			float fTemp70 = ((fTemp6 * fTemp69) + (fTemp12 * fRec26[1]));
			float fTemp71 = ((fTemp12 * fRec30[1]) + (fTemp6 * fTemp70));
			float fTemp72 = ((fTemp12 * fRec33[1]) + (fTemp6 * fTemp71));
			fVec9[(IOTA & 16383)] = fTemp72;
			int iTemp73 = std::min(8192, std::max(0, int(fTemp54)));
			int iTemp74 = (iTemp73 + 1);
			fRec37[0] = (((fTemp58 * fRec37[1]) + fVec9[((IOTA - iTemp74) & 16383)]) + ((fVec9[((IOTA - iTemp73) & 16383)] * fTemp56) / fTemp57));
			fRec34[0] = fRec37[0];
			float fTemp75 = (0.0f - ((fTemp11 * fTemp45) + (fTemp6 * fRec34[1])));
			fVec10[(IOTA & 16383)] = fTemp75;
			fRec39[0] = (fSlow22 + (0.999000013f * (float((iSlow21 * iTemp7)) + fRec39[1])));
			float fTemp76 = (fRec39[0] + -1.49998999f);
			int iTemp77 = std::min(8192, std::max(0, int(fTemp76)));
			int iTemp78 = (iTemp77 + 1);
			float fTemp79 = std::floor(fTemp76);
			float fTemp80 = (fTemp79 + (2.0f - fRec39[0]));
			float fTemp81 = (fRec39[0] - fTemp79);
			float fTemp82 = (0.0f - (fTemp80 / fTemp81));
			fRec32[0] = (fVec10[((IOTA - iTemp78) & 16383)] + (((fTemp80 * fVec10[((IOTA - iTemp77) & 16383)]) / fTemp81) + (fTemp82 * fRec32[1])));
			fRec30[0] = fRec32[0];
			float fTemp83 = ((fTemp11 * fTemp71) + (fTemp6 * fRec33[1]));
			fVec11[(IOTA & 16383)] = fTemp83;
			fRec41[0] = (fSlow24 + (0.999000013f * (float((iSlow23 * iTemp7)) + fRec41[1])));
			float fTemp84 = (fRec41[0] + -1.49998999f);
			int iTemp85 = std::min(8192, std::max(0, int(fTemp84)));
			int iTemp86 = (iTemp85 + 1);
			float fTemp87 = std::floor(fTemp84);
			float fTemp88 = (fTemp87 + (2.0f - fRec41[0]));
			float fTemp89 = (fRec41[0] - fTemp87);
			float fTemp90 = (0.0f - (fTemp88 / fTemp89));
			fRec40[0] = (fVec11[((IOTA - iTemp86) & 16383)] + (((fVec11[((IOTA - iTemp85) & 16383)] * fTemp88) / fTemp89) + (fTemp90 * fRec40[1])));
			fRec31[0] = fRec40[0];
			float fTemp91 = (0.0f - ((fTemp11 * fTemp44) + (fTemp6 * fRec31[1])));
			fVec12[(IOTA & 16383)] = fTemp91;
			int iTemp92 = std::min(8192, std::max(0, int(fTemp39)));
			int iTemp93 = (iTemp92 + 1);
			fRec28[0] = (((fTemp43 * fRec28[1]) + fVec12[((IOTA - iTemp93) & 16383)]) + ((fTemp41 * fVec12[((IOTA - iTemp92) & 16383)]) / fTemp42));
			fRec26[0] = fRec28[0];
			float fTemp94 = ((fTemp11 * fTemp70) + (fTemp6 * fRec30[1]));
			fVec13[(IOTA & 16383)] = fTemp94;
			fRec43[0] = (fSlow26 + (0.999000013f * (float((iSlow25 * iTemp7)) + fRec43[1])));
			float fTemp95 = (fRec43[0] + -1.49998999f);
			int iTemp96 = std::min(8192, std::max(0, int(fTemp95)));
			float fTemp97 = std::floor(fTemp95);
			float fTemp98 = (fTemp97 + (2.0f - fRec43[0]));
			float fTemp99 = (fRec43[0] - fTemp97);
			int iTemp100 = (iTemp96 + 1);
			float fTemp101 = (0.0f - (fTemp98 / fTemp99));
			fRec42[0] = (((fVec13[((IOTA - iTemp96) & 16383)] * fTemp98) / fTemp99) + (fVec13[((IOTA - iTemp100) & 16383)] + (fTemp101 * fRec42[1])));
			fRec27[0] = fRec42[0];
			float fTemp102 = (0.0f - ((fTemp11 * fTemp38) + (fTemp6 * fRec27[1])));
			fVec14[(IOTA & 16383)] = fTemp102;
			fRec44[0] = (fSlow28 + (0.999000013f * (float((iSlow27 * iTemp7)) + fRec44[1])));
			float fTemp103 = (fRec44[0] + -1.49998999f);
			int iTemp104 = std::min(8192, std::max(0, int(fTemp103)));
			int iTemp105 = (iTemp104 + 1);
			float fTemp106 = std::floor(fTemp103);
			float fTemp107 = (fTemp106 + (2.0f - fRec44[0]));
			float fTemp108 = (fRec44[0] - fTemp106);
			float fTemp109 = (0.0f - (fTemp107 / fTemp108));
			fRec25[0] = (fVec14[((IOTA - iTemp105) & 16383)] + (((fTemp107 * fVec14[((IOTA - iTemp104) & 16383)]) / fTemp108) + (fTemp109 * fRec25[1])));
			fRec23[0] = fRec25[0];
			float fTemp110 = ((fTemp11 * fTemp69) + (fTemp6 * fRec26[1]));
			fVec15[(IOTA & 16383)] = fTemp110;
			fRec46[0] = (fSlow30 + (0.999000013f * (float((iSlow29 * iTemp7)) + fRec46[1])));
			float fTemp111 = (fRec46[0] + -1.49998999f);
			int iTemp112 = std::min(8192, std::max(0, int(fTemp111)));
			int iTemp113 = (iTemp112 + 1);
			float fTemp114 = std::floor(fTemp111);
			float fTemp115 = (fTemp114 + (2.0f - fRec46[0]));
			float fTemp116 = (fRec46[0] - fTemp114);
			float fTemp117 = (0.0f - (fTemp115 / fTemp116));
			fRec45[0] = (fVec15[((IOTA - iTemp113) & 16383)] + (((fVec15[((IOTA - iTemp112) & 16383)] * fTemp115) / fTemp116) + (fTemp117 * fRec45[1])));
			fRec24[0] = fRec45[0];
			float fTemp118 = (fTemp36 + (fTemp6 * fRec24[1]));
			float fTemp119 = std::cos(fTemp14);
			float fTemp120 = (0.0f - fTemp15);
			float fTemp121 = (fTemp120 * fRec48[1]);
			float fTemp122 = ((fTemp12 * fRec51[1]) + fTemp37);
			float fTemp123 = ((fTemp6 * fTemp122) + (fTemp12 * fRec54[1]));
			float fTemp124 = ((fTemp12 * fRec57[1]) + (fTemp6 * fTemp123));
			float fTemp125 = (0.0f - ((fTemp12 * fRec60[1]) + (fTemp6 * fTemp124)));
			fVec16[(IOTA & 16383)] = fTemp125;
			fRec61[0] = (((fRec61[1] * fTemp50) + fVec16[((IOTA - iTemp53) & 16383)]) + ((fTemp48 * fVec16[((IOTA - iTemp52) & 16383)]) / fTemp49));
			fRec59[0] = fRec61[0];
			float fTemp126 = ((fTemp12 * fRec50[1]) + fTemp68);
			float fTemp127 = ((fTemp12 * fRec53[1]) + (fTemp6 * fTemp126));
			float fTemp128 = ((fTemp12 * fRec56[1]) + (fTemp6 * fTemp127));
			float fTemp129 = ((fTemp12 * fRec59[1]) + (fTemp6 * fTemp128));
			fVec17[(IOTA & 16383)] = fTemp129;
			fRec62[0] = (((fRec62[1] * fTemp58) + fVec17[((IOTA - iTemp74) & 16383)]) + ((fVec17[((IOTA - iTemp73) & 16383)] * fTemp56) / fTemp57));
			fRec60[0] = fRec62[0];
			float fTemp130 = (0.0f - ((fTemp6 * fRec60[1]) + (fTemp11 * fTemp124)));
			fVec18[(IOTA & 16383)] = fTemp130;
			fRec58[0] = (fVec18[((IOTA - iTemp78) & 16383)] + ((fRec58[1] * fTemp82) + ((fTemp80 * fVec18[((IOTA - iTemp77) & 16383)]) / fTemp81)));
			fRec56[0] = fRec58[0];
			float fTemp131 = ((fTemp11 * fTemp128) + (fTemp6 * fRec59[1]));
			fVec19[(IOTA & 16383)] = fTemp131;
			fRec63[0] = ((((fVec19[((IOTA - iTemp85) & 16383)] * fTemp88) / fTemp89) + fVec19[((IOTA - iTemp86) & 16383)]) + (fRec63[1] * fTemp90));
			fRec57[0] = fRec63[0];
			float fTemp132 = (0.0f - ((fTemp11 * fTemp123) + (fTemp6 * fRec57[1])));
			fVec20[(IOTA & 16383)] = fTemp132;
			fRec55[0] = (((fRec55[1] * fTemp43) + fVec20[((IOTA - iTemp93) & 16383)]) + ((fTemp41 * fVec20[((IOTA - iTemp92) & 16383)]) / fTemp42));
			fRec53[0] = fRec55[0];
			float fTemp133 = ((fTemp11 * fTemp127) + (fTemp6 * fRec56[1]));
			fVec21[(IOTA & 16383)] = fTemp133;
			fRec64[0] = (((fVec21[((IOTA - iTemp96) & 16383)] * fTemp98) / fTemp99) + (fVec21[((IOTA - iTemp100) & 16383)] + (fTemp101 * fRec64[1])));
			fRec54[0] = fRec64[0];
			float fTemp134 = (0.0f - ((fTemp11 * fTemp122) + (fTemp6 * fRec54[1])));
			fVec22[(IOTA & 16383)] = fTemp134;
			fRec52[0] = ((((fTemp107 * fVec22[((IOTA - iTemp104) & 16383)]) / fTemp108) + fVec22[((IOTA - iTemp105) & 16383)]) + (fRec52[1] * fTemp109));
			fRec50[0] = fRec52[0];
			float fTemp135 = ((fTemp11 * fTemp126) + (fTemp6 * fRec53[1]));
			fVec23[(IOTA & 16383)] = fTemp135;
			fRec65[0] = (fVec23[((IOTA - iTemp113) & 16383)] + (((fVec23[((IOTA - iTemp112) & 16383)] * fTemp115) / fTemp116) + (fTemp117 * fRec65[1])));
			fRec51[0] = fRec65[0];
			float fTemp136 = (fTemp119 * (fTemp36 + (fTemp6 * fRec51[1])));
			float fTemp137 = (fTemp121 + fTemp136);
			fRec72[0] = (fSlow33 + (0.999000013f * (float((iSlow32 * iTemp7)) + fRec72[1])));
			float fTemp138 = (fRec72[0] + -1.49998999f);
			float fTemp139 = std::floor(fTemp138);
			float fTemp140 = (fTemp139 + (2.0f - fRec72[0]));
			float fTemp141 = (fTemp120 * fRec67[1]);
			float fTemp142 = (fTemp37 + (fTemp12 * fRec74[1]));
			float fTemp143 = ((fTemp12 * fRec77[1]) + (fTemp6 * fTemp142));
			float fTemp144 = ((fTemp12 * fRec80[1]) + (fTemp6 * fTemp143));
			float fTemp145 = (0.0f - ((fTemp12 * fRec83[1]) + (fTemp6 * fTemp144)));
			fVec24[(IOTA & 16383)] = fTemp145;
			fRec84[0] = (((fRec84[1] * fTemp50) + fVec24[((IOTA - iTemp53) & 16383)]) + ((fTemp48 * fVec24[((IOTA - iTemp52) & 16383)]) / fTemp49));
			fRec82[0] = fRec84[0];
			float fTemp146 = (fTemp68 + (fTemp12 * fRec73[1]));
			float fTemp147 = ((fTemp6 * fTemp146) + (fTemp12 * fRec76[1]));
			float fTemp148 = ((fTemp12 * fRec79[1]) + (fTemp6 * fTemp147));
			float fTemp149 = ((fTemp12 * fRec82[1]) + (fTemp6 * fTemp148));
			fVec25[(IOTA & 16383)] = fTemp149;
			fRec85[0] = (((fRec85[1] * fTemp58) + fVec25[((IOTA - iTemp74) & 16383)]) + ((fVec25[((IOTA - iTemp73) & 16383)] * fTemp56) / fTemp57));
			fRec83[0] = fRec85[0];
			float fTemp150 = (0.0f - ((fTemp6 * fRec83[1]) + (fTemp11 * fTemp144)));
			fVec26[(IOTA & 16383)] = fTemp150;
			fRec81[0] = (((fTemp80 * fVec26[((IOTA - iTemp77) & 16383)]) / fTemp81) + ((fRec81[1] * fTemp82) + fVec26[((IOTA - iTemp78) & 16383)]));
			fRec79[0] = fRec81[0];
			float fTemp151 = ((fTemp6 * fRec82[1]) + (fTemp11 * fTemp148));
			fVec27[(IOTA & 16383)] = fTemp151;
			fRec86[0] = (((fVec27[((IOTA - iTemp85) & 16383)] * fTemp88) / fTemp89) + ((fRec86[1] * fTemp90) + fVec27[((IOTA - iTemp86) & 16383)]));
			fRec80[0] = fRec86[0];
			float fTemp152 = (0.0f - ((fTemp6 * fRec80[1]) + (fTemp11 * fTemp143)));
			fVec28[(IOTA & 16383)] = fTemp152;
			fRec78[0] = (((fTemp43 * fRec78[1]) + fVec28[((IOTA - iTemp93) & 16383)]) + ((fTemp41 * fVec28[((IOTA - iTemp92) & 16383)]) / fTemp42));
			fRec76[0] = fRec78[0];
			float fTemp153 = ((fTemp6 * fRec79[1]) + (fTemp11 * fTemp147));
			fVec29[(IOTA & 16383)] = fTemp153;
			fRec87[0] = (((fVec29[((IOTA - iTemp96) & 16383)] * fTemp98) / fTemp99) + ((fRec87[1] * fTemp101) + fVec29[((IOTA - iTemp100) & 16383)]));
			fRec77[0] = fRec87[0];
			float fTemp154 = (0.0f - ((fTemp6 * fRec77[1]) + (fTemp11 * fTemp142)));
			fVec30[(IOTA & 16383)] = fTemp154;
			fRec75[0] = (fVec30[((IOTA - iTemp105) & 16383)] + (((fTemp107 * fVec30[((IOTA - iTemp104) & 16383)]) / fTemp108) + (fTemp109 * fRec75[1])));
			fRec73[0] = fRec75[0];
			float fTemp155 = ((fTemp6 * fRec76[1]) + (fTemp11 * fTemp146));
			fVec31[(IOTA & 16383)] = fTemp155;
			fRec88[0] = (fVec31[((IOTA - iTemp113) & 16383)] + ((fRec88[1] * fTemp117) + ((fVec31[((IOTA - iTemp112) & 16383)] * fTemp115) / fTemp116)));
			fRec74[0] = fRec88[0];
			float fTemp156 = (fTemp119 * ((fTemp6 * fRec74[1]) + fTemp36));
			float fTemp157 = (fTemp120 * fRec70[1]);
			fRec92[0] = (fSlow36 + (0.999899983f * (float((iSlow35 * iTemp7)) + fRec92[1])));
			float fTemp158 = (fRec92[0] + -1.49998999f);
			float fTemp159 = std::floor(fTemp158);
			float fTemp160 = (fTemp159 + (2.0f - fRec92[0]));
			float fTemp161 = ((fTemp12 * fRec94[1]) + fTemp37);
			float fTemp162 = ((fTemp6 * fTemp161) + (fTemp12 * fRec97[1]));
			float fTemp163 = ((fTemp12 * fRec100[1]) + (fTemp6 * fTemp162));
			float fTemp164 = (0.0f - ((fTemp12 * fRec103[1]) + (fTemp6 * fTemp163)));
			fVec32[(IOTA & 16383)] = fTemp164;
			fRec104[0] = (((fRec104[1] * fTemp50) + fVec32[((IOTA - iTemp53) & 16383)]) + ((fTemp48 * fVec32[((IOTA - iTemp52) & 16383)]) / fTemp49));
			fRec102[0] = fRec104[0];
			float fTemp165 = ((fTemp12 * fRec93[1]) + fTemp68);
			float fTemp166 = ((fTemp12 * fRec96[1]) + (fTemp6 * fTemp165));
			float fTemp167 = ((fTemp12 * fRec99[1]) + (fTemp6 * fTemp166));
			float fTemp168 = ((fTemp12 * fRec102[1]) + (fTemp6 * fTemp167));
			fVec33[(IOTA & 16383)] = fTemp168;
			fRec105[0] = ((((fVec33[((IOTA - iTemp73) & 16383)] * fTemp56) / fTemp57) + (fRec105[1] * fTemp58)) + fVec33[((IOTA - iTemp74) & 16383)]);
			fRec103[0] = fRec105[0];
			float fTemp169 = (0.0f - ((fTemp11 * fTemp163) + (fTemp6 * fRec103[1])));
			fVec34[(IOTA & 16383)] = fTemp169;
			fRec101[0] = (((fRec101[1] * fTemp82) + fVec34[((IOTA - iTemp78) & 16383)]) + ((fTemp80 * fVec34[((IOTA - iTemp77) & 16383)]) / fTemp81));
			fRec99[0] = fRec101[0];
			float fTemp170 = ((fTemp11 * fTemp167) + (fTemp6 * fRec102[1]));
			fVec35[(IOTA & 16383)] = fTemp170;
			fRec106[0] = ((fVec35[((IOTA - iTemp86) & 16383)] + ((fVec35[((IOTA - iTemp85) & 16383)] * fTemp88) / fTemp89)) + (fRec106[1] * fTemp90));
			fRec100[0] = fRec106[0];
			float fTemp171 = (0.0f - ((fTemp11 * fTemp162) + (fTemp6 * fRec100[1])));
			fVec36[(IOTA & 16383)] = fTemp171;
			fRec98[0] = (((fRec98[1] * fTemp43) + fVec36[((IOTA - iTemp93) & 16383)]) + ((fTemp41 * fVec36[((IOTA - iTemp92) & 16383)]) / fTemp42));
			fRec96[0] = fRec98[0];
			float fTemp172 = ((fTemp11 * fTemp166) + (fTemp6 * fRec99[1]));
			fVec37[(IOTA & 16383)] = fTemp172;
			fRec107[0] = ((fVec37[((IOTA - iTemp100) & 16383)] + ((fVec37[((IOTA - iTemp96) & 16383)] * fTemp98) / fTemp99)) + (fRec107[1] * fTemp101));
			fRec97[0] = fRec107[0];
			float fTemp173 = (0.0f - ((fTemp11 * fTemp161) + (fTemp6 * fRec97[1])));
			fVec38[(IOTA & 16383)] = fTemp173;
			fRec95[0] = ((((fTemp107 * fVec38[((IOTA - iTemp104) & 16383)]) / fTemp108) + fVec38[((IOTA - iTemp105) & 16383)]) + (fRec95[1] * fTemp109));
			fRec93[0] = fRec95[0];
			float fTemp174 = ((fTemp11 * fTemp165) + (fTemp6 * fRec96[1]));
			fVec39[(IOTA & 16383)] = fTemp174;
			fRec108[0] = ((((fVec39[((IOTA - iTemp112) & 16383)] * fTemp115) / fTemp116) + fVec39[((IOTA - iTemp113) & 16383)]) + (fRec108[1] * fTemp117));
			fRec94[0] = fRec108[0];
			float fTemp175 = (0.0f - ((fTemp119 * ((fTemp119 * (fTemp141 + (fTemp119 * (fTemp121 + (fTemp119 * ((fTemp6 * fRec94[1]) + fTemp36)))))) + fTemp157)) + (fTemp120 * fRec90[1])));
			fVec40[(IOTA & 16383)] = fTemp175;
			int iTemp176 = std::min(8192, std::max(0, int(fTemp158)));
			float fTemp177 = (fRec92[0] - fTemp159);
			int iTemp178 = (iTemp176 + 1);
			float fTemp179 = (0.0f - (fTemp160 / fTemp177));
			fRec91[0] = ((((fTemp160 * fVec40[((IOTA - iTemp176) & 16383)]) / fTemp177) + fVec40[((IOTA - iTemp178) & 16383)]) + (fRec91[1] * fTemp179));
			fRec89[0] = fRec91[0];
			float fTemp180 = (fTemp120 * fRec69[1]);
			float fTemp181 = (fTemp120 * fRec66[1]);
			float fTemp182 = (fTemp120 * fRec47[1]);
			float fTemp183 = (fTemp11 * fTemp67);
			float fTemp184 = ((fTemp120 * fRec89[1]) + (fTemp119 * (fTemp180 + (fTemp119 * (fTemp181 + (fTemp119 * (fTemp182 + (fTemp119 * ((fTemp6 * fRec93[1]) + fTemp183)))))))));
			fVec41[(IOTA & 16383)] = fTemp184;
			fRec110[0] = (fSlow38 + (0.999899983f * (float((iSlow37 * iTemp7)) + fRec110[1])));
			float fTemp185 = (fRec110[0] + -1.49998999f);
			int iTemp186 = std::min(8192, std::max(0, int(fTemp185)));
			float fTemp187 = std::floor(fTemp185);
			float fTemp188 = (fTemp187 + (2.0f - fRec110[0]));
			float fTemp189 = (fRec110[0] - fTemp187);
			int iTemp190 = (iTemp186 + 1);
			float fTemp191 = (0.0f - (fTemp188 / fTemp189));
			fRec109[0] = ((((fVec41[((IOTA - iTemp186) & 16383)] * fTemp188) / fTemp189) + fVec41[((IOTA - iTemp190) & 16383)]) + (fRec109[1] * fTemp191));
			fRec90[0] = fRec109[0];
			float fTemp192 = (0.0f - ((fTemp15 * ((fTemp119 * (fTemp141 + (fTemp119 * (fTemp121 + fTemp156)))) + fTemp157)) + (fTemp119 * fRec90[1])));
			fVec42[(IOTA & 16383)] = fTemp192;
			int iTemp193 = std::min(8192, std::max(0, int(fTemp138)));
			float fTemp194 = (fRec72[0] - fTemp139);
			int iTemp195 = (iTemp193 + 1);
			float fTemp196 = (0.0f - (fTemp140 / fTemp194));
			fRec71[0] = (((fTemp140 * fVec42[((IOTA - iTemp193) & 16383)]) / fTemp194) + (fVec42[((IOTA - iTemp195) & 16383)] + (fTemp196 * fRec71[1])));
			fRec69[0] = fRec71[0];
			fRec112[0] = (fSlow40 + (0.999000013f * (float((iSlow39 * iTemp7)) + fRec112[1])));
			float fTemp197 = (fRec112[0] + -1.49998999f);
			float fTemp198 = std::floor(fTemp197);
			float fTemp199 = (fTemp198 + (2.0f - fRec112[0]));
			float fTemp200 = (fTemp119 * ((fTemp6 * fRec73[1]) + fTemp183));
			float fTemp201 = ((fTemp119 * fRec89[1]) + (fTemp15 * (fTemp180 + (fTemp119 * (fTemp181 + (fTemp119 * (fTemp182 + fTemp200)))))));
			fVec43[(IOTA & 16383)] = fTemp201;
			int iTemp202 = std::min(8192, std::max(0, int(fTemp197)));
			float fTemp203 = (fRec112[0] - fTemp198);
			float fTemp204 = (0.0f - (fTemp199 / fTemp203));
			int iTemp205 = (iTemp202 + 1);
			fRec111[0] = (((fTemp199 * fVec43[((IOTA - iTemp202) & 16383)]) / fTemp203) + ((fRec111[1] * fTemp204) + fVec43[((IOTA - iTemp205) & 16383)]));
			fRec70[0] = fRec111[0];
			float fTemp206 = (0.0f - ((fTemp119 * fRec70[1]) + (fTemp15 * (fTemp141 + (fTemp119 * fTemp137)))));
			fVec44[(IOTA & 16383)] = fTemp206;
			fRec113[0] = (fSlow43 + (0.999000013f * (float((iSlow42 * iTemp7)) + fRec113[1])));
			float fTemp207 = (fRec113[0] + -1.49998999f);
			int iTemp208 = std::min(8192, std::max(0, int(fTemp207)));
			float fTemp209 = std::floor(fTemp207);
			float fTemp210 = (fTemp209 + (2.0f - fRec113[0]));
			float fTemp211 = (fRec113[0] - fTemp209);
			int iTemp212 = (iTemp208 + 1);
			float fTemp213 = (0.0f - (fTemp210 / fTemp211));
			fRec68[0] = ((((fVec44[((IOTA - iTemp208) & 16383)] * fTemp210) / fTemp211) + fVec44[((IOTA - iTemp212) & 16383)]) + (fRec68[1] * fTemp213));
			fRec66[0] = fRec68[0];
			float fTemp214 = (fTemp119 * (fTemp183 + (fTemp6 * fRec50[1])));
			float fTemp215 = (fTemp182 + fTemp214);
			float fTemp216 = ((fTemp15 * (fTemp181 + (fTemp119 * fTemp215))) + (fTemp119 * fRec69[1]));
			fVec45[(IOTA & 16383)] = fTemp216;
			fRec115[0] = (fSlow45 + (0.999000013f * (float((iSlow44 * iTemp7)) + fRec115[1])));
			float fTemp217 = (fRec115[0] + -1.49998999f);
			int iTemp218 = std::min(8192, std::max(0, int(fTemp217)));
			float fTemp219 = std::floor(fTemp217);
			float fTemp220 = (fTemp219 + (2.0f - fRec115[0]));
			float fTemp221 = (fRec115[0] - fTemp219);
			int iTemp222 = (iTemp218 + 1);
			float fTemp223 = (0.0f - (fTemp220 / fTemp221));
			fRec114[0] = ((((fVec45[((IOTA - iTemp218) & 16383)] * fTemp220) / fTemp221) + fVec45[((IOTA - iTemp222) & 16383)]) + (fRec114[1] * fTemp223));
			fRec67[0] = fRec114[0];
			float fTemp224 = (0.0f - ((fTemp15 * fTemp137) + (fTemp119 * fRec67[1])));
			fVec46[(IOTA & 16383)] = fTemp224;
			fRec116[0] = (fSlow48 + (0.999000013f * (float((iSlow47 * iTemp7)) + fRec116[1])));
			float fTemp225 = (fRec116[0] + -1.49998999f);
			int iTemp226 = std::min(8192, std::max(0, int(fTemp225)));
			int iTemp227 = (iTemp226 + 1);
			float fTemp228 = std::floor(fTemp225);
			float fTemp229 = (fTemp228 + (2.0f - fRec116[0]));
			float fTemp230 = (fRec116[0] - fTemp228);
			float fTemp231 = (0.0f - (fTemp229 / fTemp230));
			fRec49[0] = ((fVec46[((IOTA - iTemp227) & 16383)] + ((fVec46[((IOTA - iTemp226) & 16383)] * fTemp229) / fTemp230)) + (fRec49[1] * fTemp231));
			fRec47[0] = fRec49[0];
			float fTemp232 = ((fTemp15 * fTemp215) + (fTemp119 * fRec66[1]));
			fVec47[(IOTA & 16383)] = fTemp232;
			fRec118[0] = (fSlow50 + (0.999000013f * (float((iSlow49 * iTemp7)) + fRec118[1])));
			float fTemp233 = (fRec118[0] + -1.49998999f);
			int iTemp234 = std::min(8192, std::max(0, int(fTemp233)));
			int iTemp235 = (iTemp234 + 1);
			float fTemp236 = std::floor(fTemp233);
			float fTemp237 = (fTemp236 + (2.0f - fRec118[0]));
			float fTemp238 = (fRec118[0] - fTemp236);
			float fTemp239 = (0.0f - (fTemp237 / fTemp238));
			fRec117[0] = ((fVec47[((IOTA - iTemp235) & 16383)] + ((fVec47[((IOTA - iTemp234) & 16383)] * fTemp237) / fTemp238)) + (fRec117[1] * fTemp239));
			fRec48[0] = fRec117[0];
			float fTemp240 = (fTemp12 * fRec8[1]);
			float fTemp241 = (fTemp12 * fRec12[1]);
			float fTemp242 = (0.0f - ((fTemp6 * ((fTemp6 * ((fTemp6 * (fTemp13 + (fTemp6 * ((fTemp15 * fTemp118) + (fTemp119 * fRec48[1]))))) + fTemp240)) + fTemp241)) + (fTemp12 * fRec15[1])));
			fVec48[(IOTA & 16383)] = fTemp242;
			fRec119[0] = (fSlow53 + (0.999899983f * (float((iSlow52 * iTemp7)) + fRec119[1])));
			float fTemp243 = (fRec119[0] + -1.49998999f);
			int iTemp244 = std::min(8192, std::max(0, int(fTemp243)));
			float fTemp245 = std::floor(fTemp243);
			float fTemp246 = (fTemp245 + (2.0f - fRec119[0]));
			float fTemp247 = (fRec119[0] - fTemp245);
			fRec16[0] = ((fVec48[((IOTA - (iTemp244 + 1)) & 16383)] + ((fVec48[((IOTA - iTemp244) & 16383)] * fTemp246) / fTemp247)) + (fRec16[1] * (0.0f - (fTemp246 / fTemp247))));
			fRec14[0] = fRec16[0];
			float fTemp248 = (fTemp12 * fRec11[1]);
			float fTemp249 = (fTemp12 * fRec7[1]);
			float fTemp250 = (fTemp12 * fRec4[1]);
			float fTemp251 = (fTemp183 + (fTemp6 * fRec23[1]));
			float fTemp252 = ((fTemp12 * fRec14[1]) + (fTemp6 * (fTemp248 + (fTemp6 * (fTemp249 + (fTemp6 * (fTemp250 + (fTemp6 * ((fTemp15 * fTemp251) + (fTemp119 * fRec47[1]))))))))));
			fVec49[(IOTA & 16383)] = fTemp252;
			fRec121[0] = (fSlow55 + (0.999899983f * (float((iSlow54 * iTemp7)) + fRec121[1])));
			float fTemp253 = (fRec121[0] + -1.49998999f);
			int iTemp254 = std::min(8192, std::max(0, int(fTemp253)));
			float fTemp255 = std::floor(fTemp253);
			float fTemp256 = (fTemp255 + (2.0f - fRec121[0]));
			float fTemp257 = (fRec121[0] - fTemp255);
			fRec120[0] = ((fVec49[((IOTA - (iTemp254 + 1)) & 16383)] + ((fVec49[((IOTA - iTemp254) & 16383)] * fTemp256) / fTemp257)) + (fRec120[1] * (0.0f - (fTemp256 / fTemp257))));
			fRec15[0] = fRec120[0];
			float fTemp258 = (fTemp120 * fRec129[1]);
			float fTemp259 = (fTemp120 * fRec123[1]);
			float fTemp260 = (fTemp120 * fRec126[1]);
			float fTemp261 = (0.0f - ((fTemp120 * fRec132[1]) + (fTemp119 * (fTemp258 + (fTemp119 * ((fTemp119 * (fTemp156 + fTemp259)) + fTemp260))))));
			fVec50[(IOTA & 16383)] = fTemp261;
			fRec133[0] = (fVec50[((IOTA - iTemp178) & 16383)] + (((fTemp160 * fVec50[((IOTA - iTemp176) & 16383)]) / fTemp177) + (fTemp179 * fRec133[1])));
			fRec131[0] = fRec133[0];
			float fTemp262 = (fTemp120 * fRec128[1]);
			float fTemp263 = (fTemp120 * fRec122[1]);
			float fTemp264 = (fTemp120 * fRec125[1]);
			float fTemp265 = ((fTemp120 * fRec131[1]) + (fTemp119 * (fTemp262 + (fTemp119 * ((fTemp119 * (fTemp200 + fTemp263)) + fTemp264)))));
			fVec51[(IOTA & 16383)] = fTemp265;
			fRec134[0] = (fVec51[((IOTA - iTemp190) & 16383)] + (((fTemp188 * fVec51[((IOTA - iTemp186) & 16383)]) / fTemp189) + (fTemp191 * fRec134[1])));
			fRec132[0] = fRec134[0];
			float fTemp266 = (fTemp260 + (fTemp119 * (fTemp136 + fTemp259)));
			float fTemp267 = (0.0f - ((fTemp119 * fRec132[1]) + (fTemp15 * (fTemp258 + (fTemp119 * fTemp266)))));
			fVec52[(IOTA & 16383)] = fTemp267;
			fRec130[0] = (fVec52[((IOTA - iTemp195) & 16383)] + (((fTemp140 * fVec52[((IOTA - iTemp193) & 16383)]) / fTemp194) + (fTemp196 * fRec130[1])));
			fRec128[0] = fRec130[0];
			float fTemp268 = (fTemp264 + (fTemp119 * (fTemp214 + fTemp263)));
			float fTemp269 = ((fTemp119 * fRec131[1]) + (fTemp15 * (fTemp262 + (fTemp119 * fTemp268))));
			fVec53[(IOTA & 16383)] = fTemp269;
			fRec135[0] = (fVec53[((IOTA - iTemp205) & 16383)] + (((fTemp199 * fVec53[((IOTA - iTemp202) & 16383)]) / fTemp203) + (fTemp204 * fRec135[1])));
			fRec129[0] = fRec135[0];
			float fTemp270 = (0.0f - ((fTemp119 * fRec129[1]) + (fTemp15 * fTemp266)));
			fVec54[(IOTA & 16383)] = fTemp270;
			fRec127[0] = (((fTemp210 * fVec54[((IOTA - iTemp208) & 16383)]) / fTemp211) + ((fTemp213 * fRec127[1]) + fVec54[((IOTA - iTemp212) & 16383)]));
			fRec125[0] = fRec127[0];
			float fTemp271 = ((fTemp119 * fRec128[1]) + (fTemp15 * fTemp268));
			fVec55[(IOTA & 16383)] = fTemp271;
			fRec136[0] = (((fTemp220 * fVec55[((IOTA - iTemp218) & 16383)]) / fTemp221) + (fVec55[((IOTA - iTemp222) & 16383)] + (fTemp223 * fRec136[1])));
			fRec126[0] = fRec136[0];
			float fTemp272 = (fTemp119 * fTemp118);
			float fTemp273 = (0.0f - ((fTemp119 * fRec126[1]) + (fTemp15 * (fTemp259 + fTemp272))));
			fVec56[(IOTA & 16383)] = fTemp273;
			fRec124[0] = (fVec56[((IOTA - iTemp227) & 16383)] + ((fRec124[1] * fTemp231) + ((fTemp229 * fVec56[((IOTA - iTemp226) & 16383)]) / fTemp230)));
			fRec122[0] = fRec124[0];
			float fTemp274 = (fTemp119 * fTemp251);
			float fTemp275 = ((fTemp119 * fRec125[1]) + (fTemp15 * (fTemp263 + fTemp274)));
			fVec57[(IOTA & 16383)] = fTemp275;
			fRec137[0] = (fVec57[((IOTA - iTemp235) & 16383)] + (((fTemp237 * fVec57[((IOTA - iTemp234) & 16383)]) / fTemp238) + (fTemp239 * fRec137[1])));
			fRec123[0] = fRec137[0];
			float fTemp276 = (fTemp37 + (fTemp12 * fRec139[1]));
			float fTemp277 = ((fTemp12 * fRec142[1]) + (fTemp6 * fTemp276));
			float fTemp278 = ((fTemp12 * fRec145[1]) + (fTemp6 * fTemp277));
			float fTemp279 = (0.0f - ((fTemp12 * fRec148[1]) + (fTemp6 * fTemp278)));
			fVec58[(IOTA & 16383)] = fTemp279;
			fRec149[0] = (((fTemp50 * fRec149[1]) + fVec58[((IOTA - iTemp53) & 16383)]) + ((fTemp48 * fVec58[((IOTA - iTemp52) & 16383)]) / fTemp49));
			fRec147[0] = fRec149[0];
			float fTemp280 = (fTemp68 + (fTemp12 * fRec138[1]));
			float fTemp281 = ((fTemp12 * fRec141[1]) + (fTemp6 * fTemp280));
			float fTemp282 = ((fTemp12 * fRec144[1]) + (fTemp6 * fTemp281));
			float fTemp283 = ((fTemp12 * fRec147[1]) + (fTemp6 * fTemp282));
			fVec59[(IOTA & 16383)] = fTemp283;
			fRec150[0] = (((fTemp56 * fVec59[((IOTA - iTemp73) & 16383)]) / fTemp57) + (fVec59[((IOTA - iTemp74) & 16383)] + (fTemp58 * fRec150[1])));
			fRec148[0] = fRec150[0];
			float fTemp284 = (0.0f - ((fTemp6 * fRec148[1]) + (fTemp11 * fTemp278)));
			fVec60[(IOTA & 16383)] = fTemp284;
			fRec146[0] = (fVec60[((IOTA - iTemp78) & 16383)] + (((fTemp80 * fVec60[((IOTA - iTemp77) & 16383)]) / fTemp81) + (fTemp82 * fRec146[1])));
			fRec144[0] = fRec146[0];
			float fTemp285 = ((fTemp6 * fRec147[1]) + (fTemp11 * fTemp282));
			fVec61[(IOTA & 16383)] = fTemp285;
			fRec151[0] = (fVec61[((IOTA - iTemp86) & 16383)] + (((fTemp88 * fVec61[((IOTA - iTemp85) & 16383)]) / fTemp89) + (fTemp90 * fRec151[1])));
			fRec145[0] = fRec151[0];
			float fTemp286 = (0.0f - ((fTemp6 * fRec145[1]) + (fTemp11 * fTemp277)));
			fVec62[(IOTA & 16383)] = fTemp286;
			fRec143[0] = (((fTemp43 * fRec143[1]) + fVec62[((IOTA - iTemp93) & 16383)]) + ((fTemp41 * fVec62[((IOTA - iTemp92) & 16383)]) / fTemp42));
			fRec141[0] = fRec143[0];
			float fTemp287 = ((fTemp6 * fRec144[1]) + (fTemp11 * fTemp281));
			fVec63[(IOTA & 16383)] = fTemp287;
			fRec152[0] = (((fTemp98 * fVec63[((IOTA - iTemp96) & 16383)]) / fTemp99) + (fVec63[((IOTA - iTemp100) & 16383)] + (fTemp101 * fRec152[1])));
			fRec142[0] = fRec152[0];
			float fTemp288 = (0.0f - ((fTemp6 * fRec142[1]) + (fTemp11 * fTemp276)));
			fVec64[(IOTA & 16383)] = fTemp288;
			fRec140[0] = (fVec64[((IOTA - iTemp105) & 16383)] + (((fTemp107 * fVec64[((IOTA - iTemp104) & 16383)]) / fTemp108) + (fTemp109 * fRec140[1])));
			fRec138[0] = fRec140[0];
			float fTemp289 = ((fTemp6 * fRec141[1]) + (fTemp11 * fTemp280));
			fVec65[(IOTA & 16383)] = fTemp289;
			fRec153[0] = (fVec65[((IOTA - iTemp113) & 16383)] + (((fTemp115 * fVec65[((IOTA - iTemp112) & 16383)]) / fTemp116) + (fTemp117 * fRec153[1])));
			fRec139[0] = fRec153[0];
			float fTemp290 = (fTemp36 + (fTemp6 * fRec139[1]));
			float fTemp291 = (0.0f - ((fTemp6 * fRec15[1]) + (fTemp11 * (fTemp241 + (fTemp6 * (fTemp240 + (fTemp6 * (fTemp13 + (fTemp6 * ((fTemp119 * fRec123[1]) + (fTemp15 * fTemp290)))))))))));
			fVec66[(IOTA & 16383)] = fTemp291;
			fRec154[0] = (fSlow58 + (0.999000013f * (float((iSlow57 * iTemp7)) + fRec154[1])));
			float fTemp292 = (fRec154[0] + -1.49998999f);
			int iTemp293 = std::min(8192, std::max(0, int(fTemp292)));
			float fTemp294 = std::floor(fTemp292);
			float fTemp295 = (fTemp294 + (2.0f - fRec154[0]));
			float fTemp296 = (fRec154[0] - fTemp294);
			fRec13[0] = (fVec66[((IOTA - (iTemp293 + 1)) & 16383)] + ((fRec13[1] * (0.0f - (fTemp295 / fTemp296))) + ((fTemp295 * fVec66[((IOTA - iTemp293) & 16383)]) / fTemp296)));
			fRec11[0] = fRec13[0];
			float fTemp297 = (fTemp183 + (fTemp6 * fRec138[1]));
			float fTemp298 = ((fTemp6 * fRec14[1]) + (fTemp11 * (fTemp248 + (fTemp6 * (fTemp249 + (fTemp6 * (fTemp250 + (fTemp6 * ((fTemp119 * fRec122[1]) + (fTemp15 * fTemp297))))))))));
			fVec67[(IOTA & 16383)] = fTemp298;
			fRec156[0] = (fSlow60 + (0.999000013f * (float((iSlow59 * iTemp7)) + fRec156[1])));
			float fTemp299 = (fRec156[0] + -1.49998999f);
			int iTemp300 = std::min(8192, std::max(0, int(fTemp299)));
			float fTemp301 = std::floor(fTemp299);
			float fTemp302 = (fTemp301 + (2.0f - fRec156[0]));
			float fTemp303 = (fRec156[0] - fTemp301);
			fRec155[0] = (fVec67[((IOTA - (iTemp300 + 1)) & 16383)] + ((fRec155[1] * (0.0f - (fTemp302 / fTemp303))) + ((fTemp302 * fVec67[((IOTA - iTemp300) & 16383)]) / fTemp303)));
			fRec12[0] = fRec155[0];
			float fTemp304 = (fTemp120 * fRec161[1]);
			float fTemp305 = (fTemp120 * fRec158[1]);
			float fTemp306 = ((fTemp120 * fRec164[1]) + (fTemp119 * (fTemp304 + (fTemp119 * (fTemp136 + fTemp305)))));
			float fTemp307 = (0.0f - ((fTemp120 * fRec167[1]) + (fTemp119 * fTemp306)));
			fVec68[(IOTA & 16383)] = fTemp307;
			fRec168[0] = (fVec68[((IOTA - iTemp178) & 16383)] + (((fTemp160 * fVec68[((IOTA - iTemp176) & 16383)]) / fTemp177) + (fTemp179 * fRec168[1])));
			fRec166[0] = fRec168[0];
			float fTemp308 = (fTemp120 * fRec160[1]);
			float fTemp309 = (fTemp120 * fRec157[1]);
			float fTemp310 = ((fTemp120 * fRec163[1]) + (fTemp119 * (fTemp308 + (fTemp119 * (fTemp214 + fTemp309)))));
			float fTemp311 = ((fTemp120 * fRec166[1]) + (fTemp119 * fTemp310));
			fVec69[(IOTA & 16383)] = fTemp311;
			fRec169[0] = (fVec69[((IOTA - iTemp190) & 16383)] + (((fTemp188 * fVec69[((IOTA - iTemp186) & 16383)]) / fTemp189) + (fTemp191 * fRec169[1])));
			fRec167[0] = fRec169[0];
			float fTemp312 = (0.0f - ((fTemp119 * fRec167[1]) + (fTemp15 * fTemp306)));
			fVec70[(IOTA & 16383)] = fTemp312;
			fRec165[0] = (fVec70[((IOTA - iTemp195) & 16383)] + (((fTemp140 * fVec70[((IOTA - iTemp193) & 16383)]) / fTemp194) + (fTemp196 * fRec165[1])));
			fRec163[0] = fRec165[0];
			float fTemp313 = ((fTemp119 * fRec166[1]) + (fTemp15 * fTemp310));
			fVec71[(IOTA & 16383)] = fTemp313;
			fRec170[0] = (fVec71[((IOTA - iTemp205) & 16383)] + (((fTemp199 * fVec71[((IOTA - iTemp202) & 16383)]) / fTemp203) + (fTemp204 * fRec170[1])));
			fRec164[0] = fRec170[0];
			float fTemp314 = (0.0f - ((fTemp119 * fRec164[1]) + (fTemp15 * (fTemp304 + (fTemp119 * (fTemp305 + fTemp272))))));
			fVec72[(IOTA & 16383)] = fTemp314;
			fRec162[0] = (((fTemp210 * fVec72[((IOTA - iTemp208) & 16383)]) / fTemp211) + ((fTemp213 * fRec162[1]) + fVec72[((IOTA - iTemp212) & 16383)]));
			fRec160[0] = fRec162[0];
			float fTemp315 = ((fTemp119 * fRec163[1]) + (fTemp15 * (fTemp308 + (fTemp119 * (fTemp309 + fTemp274)))));
			fVec73[(IOTA & 16383)] = fTemp315;
			fRec171[0] = (((fTemp220 * fVec73[((IOTA - iTemp218) & 16383)]) / fTemp221) + (fVec73[((IOTA - iTemp222) & 16383)] + (fTemp223 * fRec171[1])));
			fRec161[0] = fRec171[0];
			float fTemp316 = (fTemp119 * fTemp290);
			float fTemp317 = (0.0f - ((fTemp119 * fRec161[1]) + (fTemp15 * (fTemp305 + fTemp316))));
			fVec74[(IOTA & 16383)] = fTemp317;
			fRec159[0] = (fVec74[((IOTA - iTemp227) & 16383)] + (((fTemp229 * fVec74[((IOTA - iTemp226) & 16383)]) / fTemp230) + (fTemp231 * fRec159[1])));
			fRec157[0] = fRec159[0];
			float fTemp318 = (fTemp119 * fTemp297);
			float fTemp319 = ((fTemp119 * fRec160[1]) + (fTemp15 * (fTemp309 + fTemp318)));
			fVec75[(IOTA & 16383)] = fTemp319;
			fRec172[0] = (fVec75[((IOTA - iTemp235) & 16383)] + (((fTemp237 * fVec75[((IOTA - iTemp234) & 16383)]) / fTemp238) + (fTemp239 * fRec172[1])));
			fRec158[0] = fRec172[0];
			float fTemp320 = (fTemp37 + (fTemp12 * fRec174[1]));
			float fTemp321 = ((fTemp12 * fRec177[1]) + (fTemp6 * fTemp320));
			float fTemp322 = ((fTemp12 * fRec180[1]) + (fTemp6 * fTemp321));
			float fTemp323 = (0.0f - ((fTemp12 * fRec183[1]) + (fTemp6 * fTemp322)));
			fVec76[(IOTA & 16383)] = fTemp323;
			fRec184[0] = (((fTemp50 * fRec184[1]) + fVec76[((IOTA - iTemp53) & 16383)]) + ((fTemp48 * fVec76[((IOTA - iTemp52) & 16383)]) / fTemp49));
			fRec182[0] = fRec184[0];
			float fTemp324 = (fTemp68 + (fTemp12 * fRec173[1]));
			float fTemp325 = ((fTemp12 * fRec176[1]) + (fTemp6 * fTemp324));
			float fTemp326 = ((fTemp12 * fRec179[1]) + (fTemp6 * fTemp325));
			float fTemp327 = ((fTemp12 * fRec182[1]) + (fTemp6 * fTemp326));
			fVec77[(IOTA & 16383)] = fTemp327;
			fRec185[0] = (fVec77[((IOTA - iTemp74) & 16383)] + (((fTemp56 * fVec77[((IOTA - iTemp73) & 16383)]) / fTemp57) + (fTemp58 * fRec185[1])));
			fRec183[0] = fRec185[0];
			float fTemp328 = (0.0f - ((fTemp6 * fRec183[1]) + (fTemp11 * fTemp322)));
			fVec78[(IOTA & 16383)] = fTemp328;
			fRec181[0] = (fVec78[((IOTA - iTemp78) & 16383)] + (((fTemp80 * fVec78[((IOTA - iTemp77) & 16383)]) / fTemp81) + (fTemp82 * fRec181[1])));
			fRec179[0] = fRec181[0];
			float fTemp329 = ((fTemp6 * fRec182[1]) + (fTemp11 * fTemp326));
			fVec79[(IOTA & 16383)] = fTemp329;
			fRec186[0] = (fVec79[((IOTA - iTemp86) & 16383)] + (((fTemp88 * fVec79[((IOTA - iTemp85) & 16383)]) / fTemp89) + (fTemp90 * fRec186[1])));
			fRec180[0] = fRec186[0];
			float fTemp330 = (0.0f - ((fTemp6 * fRec180[1]) + (fTemp11 * fTemp321)));
			fVec80[(IOTA & 16383)] = fTemp330;
			fRec178[0] = (((fTemp43 * fRec178[1]) + fVec80[((IOTA - iTemp93) & 16383)]) + ((fTemp41 * fVec80[((IOTA - iTemp92) & 16383)]) / fTemp42));
			fRec176[0] = fRec178[0];
			float fTemp331 = ((fTemp6 * fRec179[1]) + (fTemp11 * fTemp325));
			fVec81[(IOTA & 16383)] = fTemp331;
			fRec187[0] = (((fTemp98 * fVec81[((IOTA - iTemp96) & 16383)]) / fTemp99) + (fVec81[((IOTA - iTemp100) & 16383)] + (fTemp101 * fRec187[1])));
			fRec177[0] = fRec187[0];
			float fTemp332 = (0.0f - ((fTemp6 * fRec177[1]) + (fTemp11 * fTemp320)));
			fVec82[(IOTA & 16383)] = fTemp332;
			fRec175[0] = (fVec82[((IOTA - iTemp105) & 16383)] + (((fTemp107 * fVec82[((IOTA - iTemp104) & 16383)]) / fTemp108) + (fTemp109 * fRec175[1])));
			fRec173[0] = fRec175[0];
			float fTemp333 = ((fTemp6 * fRec176[1]) + (fTemp11 * fTemp324));
			fVec83[(IOTA & 16383)] = fTemp333;
			fRec188[0] = (fVec83[((IOTA - iTemp113) & 16383)] + (((fTemp115 * fVec83[((IOTA - iTemp112) & 16383)]) / fTemp116) + (fTemp117 * fRec188[1])));
			fRec174[0] = fRec188[0];
			float fTemp334 = (fTemp36 + (fTemp6 * fRec174[1]));
			float fTemp335 = (0.0f - ((fTemp6 * fRec12[1]) + (fTemp11 * (fTemp240 + (fTemp6 * (fTemp13 + (fTemp6 * ((fTemp119 * fRec158[1]) + (fTemp15 * fTemp334)))))))));
			fVec84[(IOTA & 16383)] = fTemp335;
			int iTemp336 = std::min(8192, std::max(0, int(fTemp8)));
			float fTemp337 = (fRec10[0] - fTemp9);
			fRec9[0] = (((fTemp10 * fVec84[((IOTA - iTemp336) & 16383)]) / fTemp337) + ((fRec9[1] * (0.0f - (fTemp10 / fTemp337))) + fVec84[((IOTA - (iTemp336 + 1)) & 16383)]));
			fRec7[0] = fRec9[0];
			fRec190[0] = (fSlow62 + (0.999000013f * (float((iSlow61 * iTemp7)) + fRec190[1])));
			float fTemp338 = (fRec190[0] + -1.49998999f);
			float fTemp339 = std::floor(fTemp338);
			float fTemp340 = (fTemp339 + (2.0f - fRec190[0]));
			float fTemp341 = (fTemp183 + (fTemp6 * fRec173[1]));
			float fTemp342 = ((fTemp6 * fRec11[1]) + (fTemp11 * (fTemp249 + (fTemp6 * (fTemp250 + (fTemp6 * ((fTemp119 * fRec157[1]) + (fTemp15 * fTemp341))))))));
			fVec85[(IOTA & 16383)] = fTemp342;
			int iTemp343 = std::min(8192, std::max(0, int(fTemp338)));
			float fTemp344 = (fRec190[0] - fTemp339);
			fRec189[0] = (((fTemp340 * fVec85[((IOTA - iTemp343) & 16383)]) / fTemp344) + ((fRec189[1] * (0.0f - (fTemp340 / fTemp344))) + fVec85[((IOTA - (iTemp343 + 1)) & 16383)]));
			fRec8[0] = fRec189[0];
			float fTemp345 = (fTemp120 * fRec198[1]);
			float fTemp346 = (fTemp120 * fRec195[1]);
			float fTemp347 = (fTemp120 * fRec192[1]);
			float fTemp348 = (0.0f - ((fTemp119 * (fTemp345 + (fTemp119 * (fTemp346 + (fTemp119 * (fTemp136 + fTemp347)))))) + (fTemp120 * fRec201[1])));
			fVec86[(IOTA & 16383)] = fTemp348;
			fRec202[0] = ((((fTemp160 * fVec86[((IOTA - iTemp176) & 16383)]) / fTemp177) + fVec86[((IOTA - iTemp178) & 16383)]) + (fTemp179 * fRec202[1]));
			fRec200[0] = fRec202[0];
			float fTemp349 = (fTemp120 * fRec197[1]);
			float fTemp350 = (fTemp120 * fRec194[1]);
			float fTemp351 = (fTemp120 * fRec191[1]);
			float fTemp352 = ((fTemp119 * (fTemp349 + (fTemp119 * (fTemp350 + (fTemp119 * (fTemp214 + fTemp351)))))) + (fTemp120 * fRec200[1]));
			fVec87[(IOTA & 16383)] = fTemp352;
			fRec203[0] = ((((fTemp188 * fVec87[((IOTA - iTemp186) & 16383)]) / fTemp189) + fVec87[((IOTA - iTemp190) & 16383)]) + (fTemp191 * fRec203[1]));
			fRec201[0] = fRec203[0];
			float fTemp353 = (0.0f - ((fTemp119 * fRec201[1]) + (fTemp15 * (fTemp345 + (fTemp119 * (fTemp346 + (fTemp119 * (fTemp347 + fTemp272))))))));
			fVec88[(IOTA & 16383)] = fTemp353;
			fRec199[0] = ((fVec88[((IOTA - iTemp195) & 16383)] + ((fTemp140 * fVec88[((IOTA - iTemp193) & 16383)]) / fTemp194)) + (fTemp196 * fRec199[1]));
			fRec197[0] = fRec199[0];
			float fTemp354 = ((fTemp119 * fRec200[1]) + (fTemp15 * (fTemp349 + (fTemp119 * (fTemp350 + (fTemp119 * (fTemp351 + fTemp274)))))));
			fVec89[(IOTA & 16383)] = fTemp354;
			fRec204[0] = ((fVec89[((IOTA - iTemp205) & 16383)] + ((fTemp199 * fVec89[((IOTA - iTemp202) & 16383)]) / fTemp203)) + (fTemp204 * fRec204[1]));
			fRec198[0] = fRec204[0];
			float fTemp355 = (0.0f - ((fTemp119 * fRec198[1]) + (fTemp15 * (fTemp346 + (fTemp119 * (fTemp347 + fTemp316))))));
			fVec90[(IOTA & 16383)] = fTemp355;
			fRec196[0] = ((((fTemp210 * fVec90[((IOTA - iTemp208) & 16383)]) / fTemp211) + fVec90[((IOTA - iTemp212) & 16383)]) + (fTemp213 * fRec196[1]));
			fRec194[0] = fRec196[0];
			float fTemp356 = ((fTemp119 * fRec197[1]) + (fTemp15 * (fTemp350 + (fTemp119 * (fTemp318 + fTemp351)))));
			fVec91[(IOTA & 16383)] = fTemp356;
			fRec205[0] = ((((fTemp220 * fVec91[((IOTA - iTemp218) & 16383)]) / fTemp221) + fVec91[((IOTA - iTemp222) & 16383)]) + (fTemp223 * fRec205[1]));
			fRec195[0] = fRec205[0];
			float fTemp357 = (fTemp119 * fTemp334);
			float fTemp358 = (0.0f - ((fTemp119 * fRec195[1]) + (fTemp15 * (fTemp347 + fTemp357))));
			fVec92[(IOTA & 16383)] = fTemp358;
			fRec193[0] = ((fVec92[((IOTA - iTemp227) & 16383)] + ((fTemp229 * fVec92[((IOTA - iTemp226) & 16383)]) / fTemp230)) + (fTemp231 * fRec193[1]));
			fRec191[0] = fRec193[0];
			float fTemp359 = (fTemp119 * fTemp341);
			float fTemp360 = ((fTemp119 * fRec194[1]) + (fTemp15 * (fTemp351 + fTemp359)));
			fVec93[(IOTA & 16383)] = fTemp360;
			fRec206[0] = ((fVec93[((IOTA - iTemp235) & 16383)] + ((fTemp237 * fVec93[((IOTA - iTemp234) & 16383)]) / fTemp238)) + (fTemp239 * fRec206[1]));
			fRec192[0] = fRec206[0];
			float fTemp361 = (fTemp37 + (fTemp12 * fRec208[1]));
			float fTemp362 = ((fTemp12 * fRec211[1]) + (fTemp6 * fTemp361));
			float fTemp363 = ((fTemp12 * fRec214[1]) + (fTemp6 * fTemp362));
			float fTemp364 = (0.0f - ((fTemp12 * fRec217[1]) + (fTemp6 * fTemp363)));
			fVec94[(IOTA & 16383)] = fTemp364;
			fRec218[0] = (((fTemp50 * fRec218[1]) + fVec94[((IOTA - iTemp53) & 16383)]) + ((fTemp48 * fVec94[((IOTA - iTemp52) & 16383)]) / fTemp49));
			fRec216[0] = fRec218[0];
			float fTemp365 = (fTemp68 + (fTemp12 * fRec207[1]));
			float fTemp366 = ((fTemp12 * fRec210[1]) + (fTemp6 * fTemp365));
			float fTemp367 = ((fTemp12 * fRec213[1]) + (fTemp6 * fTemp366));
			float fTemp368 = ((fTemp12 * fRec216[1]) + (fTemp6 * fTemp367));
			fVec95[(IOTA & 16383)] = fTemp368;
			fRec219[0] = ((fVec95[((IOTA - iTemp74) & 16383)] + ((fTemp56 * fVec95[((IOTA - iTemp73) & 16383)]) / fTemp57)) + (fTemp58 * fRec219[1]));
			fRec217[0] = fRec219[0];
			float fTemp369 = (0.0f - ((fTemp11 * fTemp363) + (fTemp6 * fRec217[1])));
			fVec96[(IOTA & 16383)] = fTemp369;
			fRec215[0] = (fVec96[((IOTA - iTemp78) & 16383)] + (((fTemp80 * fVec96[((IOTA - iTemp77) & 16383)]) / fTemp81) + (fTemp82 * fRec215[1])));
			fRec213[0] = fRec215[0];
			float fTemp370 = ((fTemp6 * fRec216[1]) + (fTemp11 * fTemp367));
			fVec97[(IOTA & 16383)] = fTemp370;
			fRec220[0] = ((fVec97[((IOTA - iTemp86) & 16383)] + ((fTemp88 * fVec97[((IOTA - iTemp85) & 16383)]) / fTemp89)) + (fTemp90 * fRec220[1]));
			fRec214[0] = fRec220[0];
			float fTemp371 = (0.0f - ((fTemp11 * fTemp362) + (fTemp6 * fRec214[1])));
			fVec98[(IOTA & 16383)] = fTemp371;
			fRec212[0] = ((fVec98[((IOTA - iTemp93) & 16383)] + ((fTemp41 * fVec98[((IOTA - iTemp92) & 16383)]) / fTemp42)) + (fTemp43 * fRec212[1]));
			fRec210[0] = fRec212[0];
			float fTemp372 = ((fTemp6 * fRec213[1]) + (fTemp11 * fTemp366));
			fVec99[(IOTA & 16383)] = fTemp372;
			fRec221[0] = (((fTemp98 * fVec99[((IOTA - iTemp96) & 16383)]) / fTemp99) + (fVec99[((IOTA - iTemp100) & 16383)] + (fTemp101 * fRec221[1])));
			fRec211[0] = fRec221[0];
			float fTemp373 = (0.0f - ((fTemp6 * fRec211[1]) + (fTemp11 * fTemp361)));
			fVec100[(IOTA & 16383)] = fTemp373;
			fRec209[0] = (fVec100[((IOTA - iTemp105) & 16383)] + (((fTemp107 * fVec100[((IOTA - iTemp104) & 16383)]) / fTemp108) + (fTemp109 * fRec209[1])));
			fRec207[0] = fRec209[0];
			float fTemp374 = ((fTemp6 * fRec210[1]) + (fTemp11 * fTemp365));
			fVec101[(IOTA & 16383)] = fTemp374;
			fRec222[0] = (fVec101[((IOTA - iTemp113) & 16383)] + (((fTemp115 * fVec101[((IOTA - iTemp112) & 16383)]) / fTemp116) + (fTemp117 * fRec222[1])));
			fRec208[0] = fRec222[0];
			float fTemp375 = (fTemp36 + (fTemp6 * fRec208[1]));
			float fTemp376 = (0.0f - ((fTemp6 * fRec8[1]) + (fTemp11 * (fTemp13 + (fTemp6 * ((fTemp119 * fRec192[1]) + (fTemp15 * fTemp375)))))));
			fVec102[(IOTA & 16383)] = fTemp376;
			fRec223[0] = (fSlow65 + (0.999000013f * (float((iSlow64 * iTemp7)) + fRec223[1])));
			float fTemp377 = (fRec223[0] + -1.49998999f);
			int iTemp378 = std::min(8192, std::max(0, int(fTemp377)));
			float fTemp379 = std::floor(fTemp377);
			float fTemp380 = (fTemp379 + (2.0f - fRec223[0]));
			float fTemp381 = (fRec223[0] - fTemp379);
			fRec6[0] = (fVec102[((IOTA - (iTemp378 + 1)) & 16383)] + ((fRec6[1] * (0.0f - (fTemp380 / fTemp381))) + ((fTemp380 * fVec102[((IOTA - iTemp378) & 16383)]) / fTemp381)));
			fRec4[0] = fRec6[0];
			float fTemp382 = (fTemp183 + (fTemp6 * fRec207[1]));
			float fTemp383 = ((fTemp6 * fRec7[1]) + (fTemp11 * (fTemp250 + (fTemp6 * ((fTemp119 * fRec191[1]) + (fTemp15 * fTemp382))))));
			fVec103[(IOTA & 16383)] = fTemp383;
			fRec225[0] = (fSlow67 + (0.999000013f * (float((iSlow66 * iTemp7)) + fRec225[1])));
			float fTemp384 = (fRec225[0] + -1.49998999f);
			int iTemp385 = std::min(8192, std::max(0, int(fTemp384)));
			float fTemp386 = std::floor(fTemp384);
			float fTemp387 = (fTemp386 + (2.0f - fRec225[0]));
			float fTemp388 = (fRec225[0] - fTemp386);
			fRec224[0] = (fVec103[((IOTA - (iTemp385 + 1)) & 16383)] + ((fRec224[1] * (0.0f - (fTemp387 / fTemp388))) + ((fTemp387 * fVec103[((IOTA - iTemp385) & 16383)]) / fTemp388)));
			fRec5[0] = fRec224[0];
			float fTemp389 = (fTemp120 * fRec233[1]);
			float fTemp390 = (fTemp120 * fRec230[1]);
			float fTemp391 = (fTemp120 * fRec227[1]);
			float fTemp392 = (0.0f - ((fTemp119 * (fTemp389 + (fTemp119 * (fTemp390 + (fTemp119 * (fTemp391 + fTemp272)))))) + (fTemp120 * fRec236[1])));
			fVec104[(IOTA & 16383)] = fTemp392;
			fRec237[0] = ((((fTemp160 * fVec104[((IOTA - iTemp176) & 16383)]) / fTemp177) + fVec104[((IOTA - iTemp178) & 16383)]) + (fTemp179 * fRec237[1]));
			fRec235[0] = fRec237[0];
			float fTemp393 = (fTemp120 * fRec232[1]);
			float fTemp394 = (fTemp120 * fRec229[1]);
			float fTemp395 = (fTemp120 * fRec226[1]);
			float fTemp396 = ((fTemp119 * (fTemp393 + (fTemp119 * (fTemp394 + (fTemp119 * (fTemp395 + fTemp274)))))) + (fTemp120 * fRec235[1]));
			fVec105[(IOTA & 16383)] = fTemp396;
			fRec238[0] = ((((fTemp188 * fVec105[((IOTA - iTemp186) & 16383)]) / fTemp189) + fVec105[((IOTA - iTemp190) & 16383)]) + (fTemp191 * fRec238[1]));
			fRec236[0] = fRec238[0];
			float fTemp397 = (0.0f - ((fTemp119 * fRec236[1]) + (fTemp15 * (fTemp389 + (fTemp119 * (fTemp390 + (fTemp119 * (fTemp391 + fTemp316))))))));
			fVec106[(IOTA & 16383)] = fTemp397;
			fRec234[0] = ((fVec106[((IOTA - iTemp195) & 16383)] + ((fTemp140 * fVec106[((IOTA - iTemp193) & 16383)]) / fTemp194)) + (fTemp196 * fRec234[1]));
			fRec232[0] = fRec234[0];
			float fTemp398 = ((fTemp119 * fRec235[1]) + (fTemp15 * (fTemp393 + (fTemp119 * (fTemp394 + (fTemp119 * (fTemp318 + fTemp395)))))));
			fVec107[(IOTA & 16383)] = fTemp398;
			fRec239[0] = ((fVec107[((IOTA - iTemp205) & 16383)] + ((fTemp199 * fVec107[((IOTA - iTemp202) & 16383)]) / fTemp203)) + (fTemp204 * fRec239[1]));
			fRec233[0] = fRec239[0];
			float fTemp399 = (0.0f - ((fTemp119 * fRec233[1]) + (fTemp15 * (fTemp390 + (fTemp119 * (fTemp391 + fTemp357))))));
			fVec108[(IOTA & 16383)] = fTemp399;
			fRec231[0] = (fVec108[((IOTA - iTemp212) & 16383)] + ((fRec231[1] * fTemp213) + ((fTemp210 * fVec108[((IOTA - iTemp208) & 16383)]) / fTemp211)));
			fRec229[0] = fRec231[0];
			float fTemp400 = ((fTemp119 * fRec232[1]) + (fTemp15 * (fTemp394 + (fTemp119 * (fTemp359 + fTemp395)))));
			fVec109[(IOTA & 16383)] = fTemp400;
			fRec240[0] = (fVec109[((IOTA - iTemp222) & 16383)] + ((fRec240[1] * fTemp223) + ((fTemp220 * fVec109[((IOTA - iTemp218) & 16383)]) / fTemp221)));
			fRec230[0] = fRec240[0];
			float fTemp401 = (0.0f - ((fTemp119 * fRec230[1]) + (fTemp15 * (fTemp391 + (fTemp119 * fTemp375)))));
			fVec110[(IOTA & 16383)] = fTemp401;
			fRec228[0] = (((fTemp229 * fVec110[((IOTA - iTemp226) & 16383)]) / fTemp230) + ((fRec228[1] * fTemp231) + fVec110[((IOTA - iTemp227) & 16383)]));
			fRec226[0] = fRec228[0];
			float fTemp402 = ((fTemp119 * fRec229[1]) + (fTemp15 * (fTemp395 + (fTemp119 * fTemp382))));
			fVec111[(IOTA & 16383)] = fTemp402;
			fRec241[0] = (((fTemp237 * fVec111[((IOTA - iTemp234) & 16383)]) / fTemp238) + ((fRec241[1] * fTemp239) + fVec111[((IOTA - iTemp235) & 16383)]));
			fRec227[0] = fRec241[0];
			float fTemp403 = (fTemp37 + (fTemp12 * fRec243[1]));
			float fTemp404 = ((fTemp12 * fRec246[1]) + (fTemp6 * fTemp403));
			float fTemp405 = ((fTemp12 * fRec249[1]) + (fTemp6 * fTemp404));
			float fTemp406 = (0.0f - ((fTemp12 * fRec252[1]) + (fTemp6 * fTemp405)));
			fVec112[(IOTA & 16383)] = fTemp406;
			fRec253[0] = (((fTemp50 * fRec253[1]) + fVec112[((IOTA - iTemp53) & 16383)]) + ((fTemp48 * fVec112[((IOTA - iTemp52) & 16383)]) / fTemp49));
			fRec251[0] = fRec253[0];
			float fTemp407 = (fTemp68 + (fTemp12 * fRec242[1]));
			float fTemp408 = ((fTemp12 * fRec245[1]) + (fTemp6 * fTemp407));
			float fTemp409 = ((fTemp12 * fRec248[1]) + (fTemp6 * fTemp408));
			float fTemp410 = ((fTemp12 * fRec251[1]) + (fTemp6 * fTemp409));
			fVec113[(IOTA & 16383)] = fTemp410;
			fRec254[0] = (fVec113[((IOTA - iTemp74) & 16383)] + (((fTemp56 * fVec113[((IOTA - iTemp73) & 16383)]) / fTemp57) + (fTemp58 * fRec254[1])));
			fRec252[0] = fRec254[0];
			float fTemp411 = (0.0f - ((fTemp6 * fRec252[1]) + (fTemp11 * fTemp405)));
			fVec114[(IOTA & 16383)] = fTemp411;
			fRec250[0] = (fVec114[((IOTA - iTemp78) & 16383)] + (((fTemp80 * fVec114[((IOTA - iTemp77) & 16383)]) / fTemp81) + (fTemp82 * fRec250[1])));
			fRec248[0] = fRec250[0];
			float fTemp412 = ((fTemp6 * fRec251[1]) + (fTemp11 * fTemp409));
			fVec115[(IOTA & 16383)] = fTemp412;
			fRec255[0] = (fVec115[((IOTA - iTemp86) & 16383)] + (((fTemp88 * fVec115[((IOTA - iTemp85) & 16383)]) / fTemp89) + (fTemp90 * fRec255[1])));
			fRec249[0] = fRec255[0];
			float fTemp413 = (0.0f - ((fTemp6 * fRec249[1]) + (fTemp11 * fTemp404)));
			fVec116[(IOTA & 16383)] = fTemp413;
			fRec247[0] = (((fTemp41 * fVec116[((IOTA - iTemp92) & 16383)]) / fTemp42) + ((fTemp43 * fRec247[1]) + fVec116[((IOTA - iTemp93) & 16383)]));
			fRec245[0] = fRec247[0];
			float fTemp414 = ((fTemp6 * fRec248[1]) + (fTemp11 * fTemp408));
			fVec117[(IOTA & 16383)] = fTemp414;
			fRec256[0] = (((fTemp98 * fVec117[((IOTA - iTemp96) & 16383)]) / fTemp99) + (fVec117[((IOTA - iTemp100) & 16383)] + (fTemp101 * fRec256[1])));
			fRec246[0] = fRec256[0];
			float fTemp415 = (0.0f - ((fTemp6 * fRec246[1]) + (fTemp11 * fTemp403)));
			fVec118[(IOTA & 16383)] = fTemp415;
			fRec244[0] = (fVec118[((IOTA - iTemp105) & 16383)] + (((fTemp107 * fVec118[((IOTA - iTemp104) & 16383)]) / fTemp108) + (fTemp109 * fRec244[1])));
			fRec242[0] = fRec244[0];
			float fTemp416 = ((fTemp6 * fRec245[1]) + (fTemp11 * fTemp407));
			fVec119[(IOTA & 16383)] = fTemp416;
			fRec257[0] = (fVec119[((IOTA - iTemp113) & 16383)] + (((fTemp115 * fVec119[((IOTA - iTemp112) & 16383)]) / fTemp116) + (fTemp117 * fRec257[1])));
			fRec243[0] = fRec257[0];
			fRec3[0] = ((0.5f * (fTemp3 * fRec3[1])) + (fTemp4 * ((fTemp6 * fRec4[1]) + (fTemp11 * ((fTemp119 * fRec226[1]) + (fTemp15 * (fTemp183 + (fTemp6 * fRec242[1]))))))));
			fRec1[(IOTA & 1023)] = fRec3[0];
			fRec258[0] = ((0.5f * (fTemp3 * fRec258[1])) + (fTemp4 * ((fTemp6 * fRec5[1]) + (fTemp11 * ((fTemp119 * fRec227[1]) + (fTemp15 * (fTemp36 + (fTemp6 * fRec243[1]))))))));
			fRec2[(IOTA & 1023)] = fRec258[0];
			output0[i] = FAUSTFLOAT((iSlow0?fTemp0:((fTemp1 * fTemp2) + (fRec0[0] * fRec1[((IOTA - 0) & 1023)]))));
			output1[i] = FAUSTFLOAT((iSlow0?fTemp16:((fTemp17 * fTemp2) + (fRec0[0] * fRec2[((IOTA - 0) & 1023)]))));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fRec10[1] = fRec10[0];
			fVec3[1] = fVec3[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			IOTA = (IOTA + 1);
			fRec29[1] = fRec29[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec33[1] = fRec33[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec34[1] = fRec34[0];
			fRec39[1] = fRec39[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec31[1] = fRec31[0];
			fRec28[1] = fRec28[0];
			fRec26[1] = fRec26[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec27[1] = fRec27[0];
			fRec44[1] = fRec44[0];
			fRec25[1] = fRec25[0];
			fRec23[1] = fRec23[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec24[1] = fRec24[0];
			fRec61[1] = fRec61[0];
			fRec59[1] = fRec59[0];
			fRec62[1] = fRec62[0];
			fRec60[1] = fRec60[0];
			fRec58[1] = fRec58[0];
			fRec56[1] = fRec56[0];
			fRec63[1] = fRec63[0];
			fRec57[1] = fRec57[0];
			fRec55[1] = fRec55[0];
			fRec53[1] = fRec53[0];
			fRec64[1] = fRec64[0];
			fRec54[1] = fRec54[0];
			fRec52[1] = fRec52[0];
			fRec50[1] = fRec50[0];
			fRec65[1] = fRec65[0];
			fRec51[1] = fRec51[0];
			fRec72[1] = fRec72[0];
			fRec84[1] = fRec84[0];
			fRec82[1] = fRec82[0];
			fRec85[1] = fRec85[0];
			fRec83[1] = fRec83[0];
			fRec81[1] = fRec81[0];
			fRec79[1] = fRec79[0];
			fRec86[1] = fRec86[0];
			fRec80[1] = fRec80[0];
			fRec78[1] = fRec78[0];
			fRec76[1] = fRec76[0];
			fRec87[1] = fRec87[0];
			fRec77[1] = fRec77[0];
			fRec75[1] = fRec75[0];
			fRec73[1] = fRec73[0];
			fRec88[1] = fRec88[0];
			fRec74[1] = fRec74[0];
			fRec92[1] = fRec92[0];
			fRec104[1] = fRec104[0];
			fRec102[1] = fRec102[0];
			fRec105[1] = fRec105[0];
			fRec103[1] = fRec103[0];
			fRec101[1] = fRec101[0];
			fRec99[1] = fRec99[0];
			fRec106[1] = fRec106[0];
			fRec100[1] = fRec100[0];
			fRec98[1] = fRec98[0];
			fRec96[1] = fRec96[0];
			fRec107[1] = fRec107[0];
			fRec97[1] = fRec97[0];
			fRec95[1] = fRec95[0];
			fRec93[1] = fRec93[0];
			fRec108[1] = fRec108[0];
			fRec94[1] = fRec94[0];
			fRec91[1] = fRec91[0];
			fRec89[1] = fRec89[0];
			fRec110[1] = fRec110[0];
			fRec109[1] = fRec109[0];
			fRec90[1] = fRec90[0];
			fRec71[1] = fRec71[0];
			fRec69[1] = fRec69[0];
			fRec112[1] = fRec112[0];
			fRec111[1] = fRec111[0];
			fRec70[1] = fRec70[0];
			fRec113[1] = fRec113[0];
			fRec68[1] = fRec68[0];
			fRec66[1] = fRec66[0];
			fRec115[1] = fRec115[0];
			fRec114[1] = fRec114[0];
			fRec67[1] = fRec67[0];
			fRec116[1] = fRec116[0];
			fRec49[1] = fRec49[0];
			fRec47[1] = fRec47[0];
			fRec118[1] = fRec118[0];
			fRec117[1] = fRec117[0];
			fRec48[1] = fRec48[0];
			fRec119[1] = fRec119[0];
			fRec16[1] = fRec16[0];
			fRec14[1] = fRec14[0];
			fRec121[1] = fRec121[0];
			fRec120[1] = fRec120[0];
			fRec15[1] = fRec15[0];
			fRec133[1] = fRec133[0];
			fRec131[1] = fRec131[0];
			fRec134[1] = fRec134[0];
			fRec132[1] = fRec132[0];
			fRec130[1] = fRec130[0];
			fRec128[1] = fRec128[0];
			fRec135[1] = fRec135[0];
			fRec129[1] = fRec129[0];
			fRec127[1] = fRec127[0];
			fRec125[1] = fRec125[0];
			fRec136[1] = fRec136[0];
			fRec126[1] = fRec126[0];
			fRec124[1] = fRec124[0];
			fRec122[1] = fRec122[0];
			fRec137[1] = fRec137[0];
			fRec123[1] = fRec123[0];
			fRec149[1] = fRec149[0];
			fRec147[1] = fRec147[0];
			fRec150[1] = fRec150[0];
			fRec148[1] = fRec148[0];
			fRec146[1] = fRec146[0];
			fRec144[1] = fRec144[0];
			fRec151[1] = fRec151[0];
			fRec145[1] = fRec145[0];
			fRec143[1] = fRec143[0];
			fRec141[1] = fRec141[0];
			fRec152[1] = fRec152[0];
			fRec142[1] = fRec142[0];
			fRec140[1] = fRec140[0];
			fRec138[1] = fRec138[0];
			fRec153[1] = fRec153[0];
			fRec139[1] = fRec139[0];
			fRec154[1] = fRec154[0];
			fRec13[1] = fRec13[0];
			fRec11[1] = fRec11[0];
			fRec156[1] = fRec156[0];
			fRec155[1] = fRec155[0];
			fRec12[1] = fRec12[0];
			fRec168[1] = fRec168[0];
			fRec166[1] = fRec166[0];
			fRec169[1] = fRec169[0];
			fRec167[1] = fRec167[0];
			fRec165[1] = fRec165[0];
			fRec163[1] = fRec163[0];
			fRec170[1] = fRec170[0];
			fRec164[1] = fRec164[0];
			fRec162[1] = fRec162[0];
			fRec160[1] = fRec160[0];
			fRec171[1] = fRec171[0];
			fRec161[1] = fRec161[0];
			fRec159[1] = fRec159[0];
			fRec157[1] = fRec157[0];
			fRec172[1] = fRec172[0];
			fRec158[1] = fRec158[0];
			fRec184[1] = fRec184[0];
			fRec182[1] = fRec182[0];
			fRec185[1] = fRec185[0];
			fRec183[1] = fRec183[0];
			fRec181[1] = fRec181[0];
			fRec179[1] = fRec179[0];
			fRec186[1] = fRec186[0];
			fRec180[1] = fRec180[0];
			fRec178[1] = fRec178[0];
			fRec176[1] = fRec176[0];
			fRec187[1] = fRec187[0];
			fRec177[1] = fRec177[0];
			fRec175[1] = fRec175[0];
			fRec173[1] = fRec173[0];
			fRec188[1] = fRec188[0];
			fRec174[1] = fRec174[0];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec190[1] = fRec190[0];
			fRec189[1] = fRec189[0];
			fRec8[1] = fRec8[0];
			fRec202[1] = fRec202[0];
			fRec200[1] = fRec200[0];
			fRec203[1] = fRec203[0];
			fRec201[1] = fRec201[0];
			fRec199[1] = fRec199[0];
			fRec197[1] = fRec197[0];
			fRec204[1] = fRec204[0];
			fRec198[1] = fRec198[0];
			fRec196[1] = fRec196[0];
			fRec194[1] = fRec194[0];
			fRec205[1] = fRec205[0];
			fRec195[1] = fRec195[0];
			fRec193[1] = fRec193[0];
			fRec191[1] = fRec191[0];
			fRec206[1] = fRec206[0];
			fRec192[1] = fRec192[0];
			fRec218[1] = fRec218[0];
			fRec216[1] = fRec216[0];
			fRec219[1] = fRec219[0];
			fRec217[1] = fRec217[0];
			fRec215[1] = fRec215[0];
			fRec213[1] = fRec213[0];
			fRec220[1] = fRec220[0];
			fRec214[1] = fRec214[0];
			fRec212[1] = fRec212[0];
			fRec210[1] = fRec210[0];
			fRec221[1] = fRec221[0];
			fRec211[1] = fRec211[0];
			fRec209[1] = fRec209[0];
			fRec207[1] = fRec207[0];
			fRec222[1] = fRec222[0];
			fRec208[1] = fRec208[0];
			fRec223[1] = fRec223[0];
			fRec6[1] = fRec6[0];
			fRec4[1] = fRec4[0];
			fRec225[1] = fRec225[0];
			fRec224[1] = fRec224[0];
			fRec5[1] = fRec5[0];
			fRec237[1] = fRec237[0];
			fRec235[1] = fRec235[0];
			fRec238[1] = fRec238[0];
			fRec236[1] = fRec236[0];
			fRec234[1] = fRec234[0];
			fRec232[1] = fRec232[0];
			fRec239[1] = fRec239[0];
			fRec233[1] = fRec233[0];
			fRec231[1] = fRec231[0];
			fRec229[1] = fRec229[0];
			fRec240[1] = fRec240[0];
			fRec230[1] = fRec230[0];
			fRec228[1] = fRec228[0];
			fRec226[1] = fRec226[0];
			fRec241[1] = fRec241[0];
			fRec227[1] = fRec227[0];
			fRec253[1] = fRec253[0];
			fRec251[1] = fRec251[0];
			fRec254[1] = fRec254[0];
			fRec252[1] = fRec252[0];
			fRec250[1] = fRec250[0];
			fRec248[1] = fRec248[0];
			fRec255[1] = fRec255[0];
			fRec249[1] = fRec249[0];
			fRec247[1] = fRec247[0];
			fRec245[1] = fRec245[0];
			fRec256[1] = fRec256[0];
			fRec246[1] = fRec246[0];
			fRec244[1] = fRec244[0];
			fRec242[1] = fRec242[0];
			fRec257[1] = fRec257[0];
			fRec243[1] = fRec243[0];
			fRec3[1] = fRec3[0];
			fRec258[1] = fRec258[0];
			
		}
		
	}

	
};
// clang-format on
#endif

    template <class T>
    struct _fx_greyhole_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_greyhole_UI<T>::name(sym(fx_greyhole));

typedef _fx_greyhole_UI<fx_greyhole> fx_greyhole_UI;

class faust_fx_greyhole_tilde : public FaustExternal<fx_greyhole, fx_greyhole_UI> {
public:
    faust_fx_greyhole_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
