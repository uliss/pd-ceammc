/* ------------------------------------------------------------
author: "Julian Parker, bug fixes by Till Bovermann"
copyright: "(c) Julian Parker 2013"
license: "GPL2+"
name: "GreyholeRaw"
version: "1.0"
Code generated with Faust 2.8.5 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __greyhole_H__
#define  __greyhole_H__

/************************************************************************
 ************************************************************************
    FAUST Architecture File
    Copyright (C) 2006-2011 Albert Graef <Dr.Graef@t-online.de>
    ---------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2.1 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with the GNU C Library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA.
 ************************************************************************
 ************************************************************************/

/* Pd architecture file, written by Albert Graef <Dr.Graef@t-online.de>.
   This was derived from minimal.cpp included in the Faust distribution.
   Please note that this is to be compiled as a shared library, which is
   then loaded dynamically by Pd as an external. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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


#include "ceammc_atomlist.h"
#include "ceammc_externals.h"
#include "m_pd.h"

/******************************************************************************
*******************************************************************************

                                   VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/

#ifdef FAUST_MACRO
// clang-format off
// clang-format on
#endif

#define sym(name) xsym(name)
#define xsym(name) #name

/***************************************************************************
   Pd UI interface
***************************************************************************/

// clang-format off
#ifndef FAUST_MACRO
struct greyhole : public dsp {
};
#endif
// clang-format on

#include "ceammc_faust.h"
using namespace ceammc::faust;

/******************************************************************************
*******************************************************************************

                FAUST DSP

*******************************************************************************
*******************************************************************************/

//----------------------------------------------------------------------------
//  FAUST generated signal processor
//----------------------------------------------------------------------------

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
#define FAUSTCLASS greyhole
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class greyhole : public dsp {
	
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
	float fRec14[2];
	float fRec18[2];
	float fRec22[2];
	float fRec26[2];
	float fRec30[2];
	float fRec34[2];
	float fRec41[2];
	FAUSTFLOAT fHslider4;
	float fVec3[2];
	int fSamplingFreq;
	float fConst0;
	FAUSTFLOAT fHslider5;
	float fRec42[2];
	float fRec43[2];
	float fRec44[2];
	float fRec45[2];
	float fConst1;
	FAUSTFLOAT fHslider6;
	float fVec4[2];
	float fConst2;
	FAUSTFLOAT fHslider7;
	float fVec5[2];
	float fRec46[2];
	float fRec47[2];
	int IOTA;
	float fVec6[131072];
	float fVec7[16384];
	float fRec40[2];
	float fRec38[2];
	float fVec8[131072];
	float fVec9[16384];
	float fRec49[2];
	float fRec48[2];
	float fRec39[2];
	float fVec10[16384];
	float fRec50[2];
	float fRec37[2];
	float fRec35[2];
	float fVec11[16384];
	float fRec52[2];
	float fRec51[2];
	float fRec36[2];
	float fVec12[16384];
	float fRec33[2];
	float fRec31[2];
	float fRec54[2];
	float fVec13[16384];
	float fRec53[2];
	float fRec32[2];
	float fVec14[16384];
	float fRec29[2];
	float fRec27[2];
	float fRec56[2];
	float fVec15[16384];
	float fRec55[2];
	float fRec28[2];
	float fVec16[16384];
	float fRec74[2];
	float fRec72[2];
	float fVec17[16384];
	float fRec75[2];
	float fRec73[2];
	float fVec18[16384];
	float fRec71[2];
	float fRec69[2];
	float fVec19[16384];
	float fRec76[2];
	float fRec70[2];
	float fVec20[16384];
	float fRec68[2];
	float fRec66[2];
	float fVec21[16384];
	float fRec77[2];
	float fRec67[2];
	float fVec22[16384];
	float fRec65[2];
	float fRec63[2];
	float fVec23[16384];
	float fRec78[2];
	float fRec64[2];
	float fVec24[16384];
	float fRec79[2];
	float fRec62[2];
	float fRec60[2];
	float fRec81[2];
	float fVec25[16384];
	float fRec80[2];
	float fRec61[2];
	float fVec26[16384];
	float fRec93[2];
	float fRec91[2];
	float fVec27[16384];
	float fRec94[2];
	float fRec92[2];
	float fVec28[16384];
	float fRec90[2];
	float fRec88[2];
	float fVec29[16384];
	float fRec95[2];
	float fRec89[2];
	float fVec30[16384];
	float fRec87[2];
	float fRec85[2];
	float fVec31[16384];
	float fRec96[2];
	float fRec86[2];
	float fVec32[16384];
	float fRec84[2];
	float fRec82[2];
	float fVec33[16384];
	float fRec97[2];
	float fRec83[2];
	float fVec34[16384];
	float fRec98[2];
	float fRec59[2];
	float fRec57[2];
	float fVec35[16384];
	float fRec100[2];
	float fRec99[2];
	float fRec58[2];
	float fVec36[16384];
	float fRec25[2];
	float fRec23[2];
	float fRec102[2];
	float fVec37[16384];
	float fRec101[2];
	float fRec24[2];
	float fVec38[16384];
	float fRec114[2];
	float fRec112[2];
	float fVec39[16384];
	float fRec115[2];
	float fRec113[2];
	float fVec40[16384];
	float fRec111[2];
	float fRec109[2];
	float fVec41[16384];
	float fRec116[2];
	float fRec110[2];
	float fVec42[16384];
	float fRec108[2];
	float fRec106[2];
	float fVec43[16384];
	float fRec117[2];
	float fRec107[2];
	float fVec44[16384];
	float fRec105[2];
	float fRec103[2];
	float fVec45[16384];
	float fRec118[2];
	float fRec104[2];
	float fVec46[16384];
	float fRec21[2];
	float fRec19[2];
	float fRec120[2];
	float fVec47[16384];
	float fRec119[2];
	float fRec20[2];
	float fVec48[16384];
	float fRec132[2];
	float fRec130[2];
	float fVec49[16384];
	float fRec133[2];
	float fRec131[2];
	float fVec50[16384];
	float fRec129[2];
	float fRec127[2];
	float fVec51[16384];
	float fRec134[2];
	float fRec128[2];
	float fVec52[16384];
	float fRec126[2];
	float fRec124[2];
	float fVec53[16384];
	float fRec135[2];
	float fRec125[2];
	float fVec54[16384];
	float fRec123[2];
	float fRec121[2];
	float fVec55[16384];
	float fRec136[2];
	float fRec122[2];
	float fVec56[16384];
	float fRec17[2];
	float fRec15[2];
	float fRec138[2];
	float fVec57[16384];
	float fRec137[2];
	float fRec16[2];
	float fVec58[16384];
	float fRec150[2];
	float fRec148[2];
	float fVec59[16384];
	float fRec151[2];
	float fRec149[2];
	float fVec60[16384];
	float fRec147[2];
	float fRec145[2];
	float fVec61[16384];
	float fRec152[2];
	float fRec146[2];
	float fVec62[16384];
	float fRec144[2];
	float fRec142[2];
	float fVec63[16384];
	float fRec153[2];
	float fRec143[2];
	float fVec64[16384];
	float fRec141[2];
	float fRec139[2];
	float fVec65[16384];
	float fRec154[2];
	float fRec140[2];
	float fVec66[16384];
	float fRec166[2];
	float fRec164[2];
	float fVec67[16384];
	float fRec167[2];
	float fRec165[2];
	float fVec68[16384];
	float fRec163[2];
	float fRec161[2];
	float fVec69[16384];
	float fRec168[2];
	float fRec162[2];
	float fVec70[16384];
	float fRec160[2];
	float fRec158[2];
	float fVec71[16384];
	float fRec169[2];
	float fRec159[2];
	float fVec72[16384];
	float fRec157[2];
	float fRec155[2];
	float fVec73[16384];
	float fRec170[2];
	float fRec156[2];
	float fVec74[16384];
	float fRec13[2];
	float fRec11[2];
	float fVec75[16384];
	float fRec172[2];
	float fRec171[2];
	float fRec12[2];
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
	float fRec200[2];
	float fRec198[2];
	float fVec85[16384];
	float fRec201[2];
	float fRec199[2];
	float fVec86[16384];
	float fRec197[2];
	float fRec195[2];
	float fVec87[16384];
	float fRec202[2];
	float fRec196[2];
	float fVec88[16384];
	float fRec194[2];
	float fRec192[2];
	float fVec89[16384];
	float fRec203[2];
	float fRec193[2];
	float fVec90[16384];
	float fRec191[2];
	float fRec189[2];
	float fVec91[16384];
	float fRec204[2];
	float fRec190[2];
	float fVec92[16384];
	float fRec9[2];
	float fRec7[2];
	float fRec206[2];
	float fVec93[16384];
	float fRec205[2];
	float fRec8[2];
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
	float fRec234[2];
	float fRec232[2];
	float fVec103[16384];
	float fRec235[2];
	float fRec233[2];
	float fVec104[16384];
	float fRec231[2];
	float fRec229[2];
	float fVec105[16384];
	float fRec236[2];
	float fRec230[2];
	float fVec106[16384];
	float fRec228[2];
	float fRec226[2];
	float fVec107[16384];
	float fRec237[2];
	float fRec227[2];
	float fVec108[16384];
	float fRec225[2];
	float fRec223[2];
	float fVec109[16384];
	float fRec238[2];
	float fRec224[2];
	float fVec110[16384];
	float fRec239[2];
	float fRec6[2];
	float fRec4[2];
	float fVec111[16384];
	float fRec241[2];
	float fRec240[2];
	float fRec5[2];
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
	float fVec120[16384];
	float fRec269[2];
	float fRec267[2];
	float fVec121[16384];
	float fRec270[2];
	float fRec268[2];
	float fVec122[16384];
	float fRec266[2];
	float fRec264[2];
	float fVec123[16384];
	float fRec271[2];
	float fRec265[2];
	float fVec124[16384];
	float fRec263[2];
	float fRec261[2];
	float fVec125[16384];
	float fRec272[2];
	float fRec262[2];
	float fVec126[16384];
	float fRec260[2];
	float fRec258[2];
	float fVec127[16384];
	float fRec273[2];
	float fRec259[2];
	float fRec3[2];
	float fRec1[1024];
	float fRec274[2];
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
			fRec14[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec18[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec22[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec26[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec30[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec34[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fRec41[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fVec3[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec42[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec43[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fRec44[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec45[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fVec4[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fVec5[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec46[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec47[l20] = 0.0f;
			
		}
		IOTA = 0;
		for (int l21 = 0; (l21 < 131072); l21 = (l21 + 1)) {
			fVec6[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 16384); l22 = (l22 + 1)) {
			fVec7[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec40[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec38[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 131072); l25 = (l25 + 1)) {
			fVec8[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 16384); l26 = (l26 + 1)) {
			fVec9[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec49[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec48[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec39[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 16384); l30 = (l30 + 1)) {
			fVec10[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec50[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec37[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec35[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 16384); l34 = (l34 + 1)) {
			fVec11[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec52[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec51[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec36[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 16384); l38 = (l38 + 1)) {
			fVec12[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec33[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec31[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec54[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 16384); l42 = (l42 + 1)) {
			fVec13[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec53[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec32[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 16384); l45 = (l45 + 1)) {
			fVec14[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec29[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec27[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec56[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 16384); l49 = (l49 + 1)) {
			fVec15[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec55[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec28[l51] = 0.0f;
			
		}
		for (int l52 = 0; (l52 < 16384); l52 = (l52 + 1)) {
			fVec16[l52] = 0.0f;
			
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec74[l53] = 0.0f;
			
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec72[l54] = 0.0f;
			
		}
		for (int l55 = 0; (l55 < 16384); l55 = (l55 + 1)) {
			fVec17[l55] = 0.0f;
			
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec75[l56] = 0.0f;
			
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec73[l57] = 0.0f;
			
		}
		for (int l58 = 0; (l58 < 16384); l58 = (l58 + 1)) {
			fVec18[l58] = 0.0f;
			
		}
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec71[l59] = 0.0f;
			
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec69[l60] = 0.0f;
			
		}
		for (int l61 = 0; (l61 < 16384); l61 = (l61 + 1)) {
			fVec19[l61] = 0.0f;
			
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec76[l62] = 0.0f;
			
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec70[l63] = 0.0f;
			
		}
		for (int l64 = 0; (l64 < 16384); l64 = (l64 + 1)) {
			fVec20[l64] = 0.0f;
			
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec68[l65] = 0.0f;
			
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec66[l66] = 0.0f;
			
		}
		for (int l67 = 0; (l67 < 16384); l67 = (l67 + 1)) {
			fVec21[l67] = 0.0f;
			
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec77[l68] = 0.0f;
			
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec67[l69] = 0.0f;
			
		}
		for (int l70 = 0; (l70 < 16384); l70 = (l70 + 1)) {
			fVec22[l70] = 0.0f;
			
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec65[l71] = 0.0f;
			
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec63[l72] = 0.0f;
			
		}
		for (int l73 = 0; (l73 < 16384); l73 = (l73 + 1)) {
			fVec23[l73] = 0.0f;
			
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec78[l74] = 0.0f;
			
		}
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec64[l75] = 0.0f;
			
		}
		for (int l76 = 0; (l76 < 16384); l76 = (l76 + 1)) {
			fVec24[l76] = 0.0f;
			
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec79[l77] = 0.0f;
			
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec62[l78] = 0.0f;
			
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec60[l79] = 0.0f;
			
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec81[l80] = 0.0f;
			
		}
		for (int l81 = 0; (l81 < 16384); l81 = (l81 + 1)) {
			fVec25[l81] = 0.0f;
			
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec80[l82] = 0.0f;
			
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec61[l83] = 0.0f;
			
		}
		for (int l84 = 0; (l84 < 16384); l84 = (l84 + 1)) {
			fVec26[l84] = 0.0f;
			
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fRec93[l85] = 0.0f;
			
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec91[l86] = 0.0f;
			
		}
		for (int l87 = 0; (l87 < 16384); l87 = (l87 + 1)) {
			fVec27[l87] = 0.0f;
			
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec94[l88] = 0.0f;
			
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec92[l89] = 0.0f;
			
		}
		for (int l90 = 0; (l90 < 16384); l90 = (l90 + 1)) {
			fVec28[l90] = 0.0f;
			
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec90[l91] = 0.0f;
			
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec88[l92] = 0.0f;
			
		}
		for (int l93 = 0; (l93 < 16384); l93 = (l93 + 1)) {
			fVec29[l93] = 0.0f;
			
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec95[l94] = 0.0f;
			
		}
		for (int l95 = 0; (l95 < 2); l95 = (l95 + 1)) {
			fRec89[l95] = 0.0f;
			
		}
		for (int l96 = 0; (l96 < 16384); l96 = (l96 + 1)) {
			fVec30[l96] = 0.0f;
			
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec87[l97] = 0.0f;
			
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec85[l98] = 0.0f;
			
		}
		for (int l99 = 0; (l99 < 16384); l99 = (l99 + 1)) {
			fVec31[l99] = 0.0f;
			
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			fRec96[l100] = 0.0f;
			
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec86[l101] = 0.0f;
			
		}
		for (int l102 = 0; (l102 < 16384); l102 = (l102 + 1)) {
			fVec32[l102] = 0.0f;
			
		}
		for (int l103 = 0; (l103 < 2); l103 = (l103 + 1)) {
			fRec84[l103] = 0.0f;
			
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec82[l104] = 0.0f;
			
		}
		for (int l105 = 0; (l105 < 16384); l105 = (l105 + 1)) {
			fVec33[l105] = 0.0f;
			
		}
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec97[l106] = 0.0f;
			
		}
		for (int l107 = 0; (l107 < 2); l107 = (l107 + 1)) {
			fRec83[l107] = 0.0f;
			
		}
		for (int l108 = 0; (l108 < 16384); l108 = (l108 + 1)) {
			fVec34[l108] = 0.0f;
			
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec98[l109] = 0.0f;
			
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			fRec59[l110] = 0.0f;
			
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec57[l111] = 0.0f;
			
		}
		for (int l112 = 0; (l112 < 16384); l112 = (l112 + 1)) {
			fVec35[l112] = 0.0f;
			
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fRec100[l113] = 0.0f;
			
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec99[l114] = 0.0f;
			
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec58[l115] = 0.0f;
			
		}
		for (int l116 = 0; (l116 < 16384); l116 = (l116 + 1)) {
			fVec36[l116] = 0.0f;
			
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fRec25[l117] = 0.0f;
			
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			fRec23[l118] = 0.0f;
			
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec102[l119] = 0.0f;
			
		}
		for (int l120 = 0; (l120 < 16384); l120 = (l120 + 1)) {
			fVec37[l120] = 0.0f;
			
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec101[l121] = 0.0f;
			
		}
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			fRec24[l122] = 0.0f;
			
		}
		for (int l123 = 0; (l123 < 16384); l123 = (l123 + 1)) {
			fVec38[l123] = 0.0f;
			
		}
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			fRec114[l124] = 0.0f;
			
		}
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			fRec112[l125] = 0.0f;
			
		}
		for (int l126 = 0; (l126 < 16384); l126 = (l126 + 1)) {
			fVec39[l126] = 0.0f;
			
		}
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			fRec115[l127] = 0.0f;
			
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			fRec113[l128] = 0.0f;
			
		}
		for (int l129 = 0; (l129 < 16384); l129 = (l129 + 1)) {
			fVec40[l129] = 0.0f;
			
		}
		for (int l130 = 0; (l130 < 2); l130 = (l130 + 1)) {
			fRec111[l130] = 0.0f;
			
		}
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			fRec109[l131] = 0.0f;
			
		}
		for (int l132 = 0; (l132 < 16384); l132 = (l132 + 1)) {
			fVec41[l132] = 0.0f;
			
		}
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			fRec116[l133] = 0.0f;
			
		}
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			fRec110[l134] = 0.0f;
			
		}
		for (int l135 = 0; (l135 < 16384); l135 = (l135 + 1)) {
			fVec42[l135] = 0.0f;
			
		}
		for (int l136 = 0; (l136 < 2); l136 = (l136 + 1)) {
			fRec108[l136] = 0.0f;
			
		}
		for (int l137 = 0; (l137 < 2); l137 = (l137 + 1)) {
			fRec106[l137] = 0.0f;
			
		}
		for (int l138 = 0; (l138 < 16384); l138 = (l138 + 1)) {
			fVec43[l138] = 0.0f;
			
		}
		for (int l139 = 0; (l139 < 2); l139 = (l139 + 1)) {
			fRec117[l139] = 0.0f;
			
		}
		for (int l140 = 0; (l140 < 2); l140 = (l140 + 1)) {
			fRec107[l140] = 0.0f;
			
		}
		for (int l141 = 0; (l141 < 16384); l141 = (l141 + 1)) {
			fVec44[l141] = 0.0f;
			
		}
		for (int l142 = 0; (l142 < 2); l142 = (l142 + 1)) {
			fRec105[l142] = 0.0f;
			
		}
		for (int l143 = 0; (l143 < 2); l143 = (l143 + 1)) {
			fRec103[l143] = 0.0f;
			
		}
		for (int l144 = 0; (l144 < 16384); l144 = (l144 + 1)) {
			fVec45[l144] = 0.0f;
			
		}
		for (int l145 = 0; (l145 < 2); l145 = (l145 + 1)) {
			fRec118[l145] = 0.0f;
			
		}
		for (int l146 = 0; (l146 < 2); l146 = (l146 + 1)) {
			fRec104[l146] = 0.0f;
			
		}
		for (int l147 = 0; (l147 < 16384); l147 = (l147 + 1)) {
			fVec46[l147] = 0.0f;
			
		}
		for (int l148 = 0; (l148 < 2); l148 = (l148 + 1)) {
			fRec21[l148] = 0.0f;
			
		}
		for (int l149 = 0; (l149 < 2); l149 = (l149 + 1)) {
			fRec19[l149] = 0.0f;
			
		}
		for (int l150 = 0; (l150 < 2); l150 = (l150 + 1)) {
			fRec120[l150] = 0.0f;
			
		}
		for (int l151 = 0; (l151 < 16384); l151 = (l151 + 1)) {
			fVec47[l151] = 0.0f;
			
		}
		for (int l152 = 0; (l152 < 2); l152 = (l152 + 1)) {
			fRec119[l152] = 0.0f;
			
		}
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			fRec20[l153] = 0.0f;
			
		}
		for (int l154 = 0; (l154 < 16384); l154 = (l154 + 1)) {
			fVec48[l154] = 0.0f;
			
		}
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fRec132[l155] = 0.0f;
			
		}
		for (int l156 = 0; (l156 < 2); l156 = (l156 + 1)) {
			fRec130[l156] = 0.0f;
			
		}
		for (int l157 = 0; (l157 < 16384); l157 = (l157 + 1)) {
			fVec49[l157] = 0.0f;
			
		}
		for (int l158 = 0; (l158 < 2); l158 = (l158 + 1)) {
			fRec133[l158] = 0.0f;
			
		}
		for (int l159 = 0; (l159 < 2); l159 = (l159 + 1)) {
			fRec131[l159] = 0.0f;
			
		}
		for (int l160 = 0; (l160 < 16384); l160 = (l160 + 1)) {
			fVec50[l160] = 0.0f;
			
		}
		for (int l161 = 0; (l161 < 2); l161 = (l161 + 1)) {
			fRec129[l161] = 0.0f;
			
		}
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			fRec127[l162] = 0.0f;
			
		}
		for (int l163 = 0; (l163 < 16384); l163 = (l163 + 1)) {
			fVec51[l163] = 0.0f;
			
		}
		for (int l164 = 0; (l164 < 2); l164 = (l164 + 1)) {
			fRec134[l164] = 0.0f;
			
		}
		for (int l165 = 0; (l165 < 2); l165 = (l165 + 1)) {
			fRec128[l165] = 0.0f;
			
		}
		for (int l166 = 0; (l166 < 16384); l166 = (l166 + 1)) {
			fVec52[l166] = 0.0f;
			
		}
		for (int l167 = 0; (l167 < 2); l167 = (l167 + 1)) {
			fRec126[l167] = 0.0f;
			
		}
		for (int l168 = 0; (l168 < 2); l168 = (l168 + 1)) {
			fRec124[l168] = 0.0f;
			
		}
		for (int l169 = 0; (l169 < 16384); l169 = (l169 + 1)) {
			fVec53[l169] = 0.0f;
			
		}
		for (int l170 = 0; (l170 < 2); l170 = (l170 + 1)) {
			fRec135[l170] = 0.0f;
			
		}
		for (int l171 = 0; (l171 < 2); l171 = (l171 + 1)) {
			fRec125[l171] = 0.0f;
			
		}
		for (int l172 = 0; (l172 < 16384); l172 = (l172 + 1)) {
			fVec54[l172] = 0.0f;
			
		}
		for (int l173 = 0; (l173 < 2); l173 = (l173 + 1)) {
			fRec123[l173] = 0.0f;
			
		}
		for (int l174 = 0; (l174 < 2); l174 = (l174 + 1)) {
			fRec121[l174] = 0.0f;
			
		}
		for (int l175 = 0; (l175 < 16384); l175 = (l175 + 1)) {
			fVec55[l175] = 0.0f;
			
		}
		for (int l176 = 0; (l176 < 2); l176 = (l176 + 1)) {
			fRec136[l176] = 0.0f;
			
		}
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fRec122[l177] = 0.0f;
			
		}
		for (int l178 = 0; (l178 < 16384); l178 = (l178 + 1)) {
			fVec56[l178] = 0.0f;
			
		}
		for (int l179 = 0; (l179 < 2); l179 = (l179 + 1)) {
			fRec17[l179] = 0.0f;
			
		}
		for (int l180 = 0; (l180 < 2); l180 = (l180 + 1)) {
			fRec15[l180] = 0.0f;
			
		}
		for (int l181 = 0; (l181 < 2); l181 = (l181 + 1)) {
			fRec138[l181] = 0.0f;
			
		}
		for (int l182 = 0; (l182 < 16384); l182 = (l182 + 1)) {
			fVec57[l182] = 0.0f;
			
		}
		for (int l183 = 0; (l183 < 2); l183 = (l183 + 1)) {
			fRec137[l183] = 0.0f;
			
		}
		for (int l184 = 0; (l184 < 2); l184 = (l184 + 1)) {
			fRec16[l184] = 0.0f;
			
		}
		for (int l185 = 0; (l185 < 16384); l185 = (l185 + 1)) {
			fVec58[l185] = 0.0f;
			
		}
		for (int l186 = 0; (l186 < 2); l186 = (l186 + 1)) {
			fRec150[l186] = 0.0f;
			
		}
		for (int l187 = 0; (l187 < 2); l187 = (l187 + 1)) {
			fRec148[l187] = 0.0f;
			
		}
		for (int l188 = 0; (l188 < 16384); l188 = (l188 + 1)) {
			fVec59[l188] = 0.0f;
			
		}
		for (int l189 = 0; (l189 < 2); l189 = (l189 + 1)) {
			fRec151[l189] = 0.0f;
			
		}
		for (int l190 = 0; (l190 < 2); l190 = (l190 + 1)) {
			fRec149[l190] = 0.0f;
			
		}
		for (int l191 = 0; (l191 < 16384); l191 = (l191 + 1)) {
			fVec60[l191] = 0.0f;
			
		}
		for (int l192 = 0; (l192 < 2); l192 = (l192 + 1)) {
			fRec147[l192] = 0.0f;
			
		}
		for (int l193 = 0; (l193 < 2); l193 = (l193 + 1)) {
			fRec145[l193] = 0.0f;
			
		}
		for (int l194 = 0; (l194 < 16384); l194 = (l194 + 1)) {
			fVec61[l194] = 0.0f;
			
		}
		for (int l195 = 0; (l195 < 2); l195 = (l195 + 1)) {
			fRec152[l195] = 0.0f;
			
		}
		for (int l196 = 0; (l196 < 2); l196 = (l196 + 1)) {
			fRec146[l196] = 0.0f;
			
		}
		for (int l197 = 0; (l197 < 16384); l197 = (l197 + 1)) {
			fVec62[l197] = 0.0f;
			
		}
		for (int l198 = 0; (l198 < 2); l198 = (l198 + 1)) {
			fRec144[l198] = 0.0f;
			
		}
		for (int l199 = 0; (l199 < 2); l199 = (l199 + 1)) {
			fRec142[l199] = 0.0f;
			
		}
		for (int l200 = 0; (l200 < 16384); l200 = (l200 + 1)) {
			fVec63[l200] = 0.0f;
			
		}
		for (int l201 = 0; (l201 < 2); l201 = (l201 + 1)) {
			fRec153[l201] = 0.0f;
			
		}
		for (int l202 = 0; (l202 < 2); l202 = (l202 + 1)) {
			fRec143[l202] = 0.0f;
			
		}
		for (int l203 = 0; (l203 < 16384); l203 = (l203 + 1)) {
			fVec64[l203] = 0.0f;
			
		}
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fRec141[l204] = 0.0f;
			
		}
		for (int l205 = 0; (l205 < 2); l205 = (l205 + 1)) {
			fRec139[l205] = 0.0f;
			
		}
		for (int l206 = 0; (l206 < 16384); l206 = (l206 + 1)) {
			fVec65[l206] = 0.0f;
			
		}
		for (int l207 = 0; (l207 < 2); l207 = (l207 + 1)) {
			fRec154[l207] = 0.0f;
			
		}
		for (int l208 = 0; (l208 < 2); l208 = (l208 + 1)) {
			fRec140[l208] = 0.0f;
			
		}
		for (int l209 = 0; (l209 < 16384); l209 = (l209 + 1)) {
			fVec66[l209] = 0.0f;
			
		}
		for (int l210 = 0; (l210 < 2); l210 = (l210 + 1)) {
			fRec166[l210] = 0.0f;
			
		}
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			fRec164[l211] = 0.0f;
			
		}
		for (int l212 = 0; (l212 < 16384); l212 = (l212 + 1)) {
			fVec67[l212] = 0.0f;
			
		}
		for (int l213 = 0; (l213 < 2); l213 = (l213 + 1)) {
			fRec167[l213] = 0.0f;
			
		}
		for (int l214 = 0; (l214 < 2); l214 = (l214 + 1)) {
			fRec165[l214] = 0.0f;
			
		}
		for (int l215 = 0; (l215 < 16384); l215 = (l215 + 1)) {
			fVec68[l215] = 0.0f;
			
		}
		for (int l216 = 0; (l216 < 2); l216 = (l216 + 1)) {
			fRec163[l216] = 0.0f;
			
		}
		for (int l217 = 0; (l217 < 2); l217 = (l217 + 1)) {
			fRec161[l217] = 0.0f;
			
		}
		for (int l218 = 0; (l218 < 16384); l218 = (l218 + 1)) {
			fVec69[l218] = 0.0f;
			
		}
		for (int l219 = 0; (l219 < 2); l219 = (l219 + 1)) {
			fRec168[l219] = 0.0f;
			
		}
		for (int l220 = 0; (l220 < 2); l220 = (l220 + 1)) {
			fRec162[l220] = 0.0f;
			
		}
		for (int l221 = 0; (l221 < 16384); l221 = (l221 + 1)) {
			fVec70[l221] = 0.0f;
			
		}
		for (int l222 = 0; (l222 < 2); l222 = (l222 + 1)) {
			fRec160[l222] = 0.0f;
			
		}
		for (int l223 = 0; (l223 < 2); l223 = (l223 + 1)) {
			fRec158[l223] = 0.0f;
			
		}
		for (int l224 = 0; (l224 < 16384); l224 = (l224 + 1)) {
			fVec71[l224] = 0.0f;
			
		}
		for (int l225 = 0; (l225 < 2); l225 = (l225 + 1)) {
			fRec169[l225] = 0.0f;
			
		}
		for (int l226 = 0; (l226 < 2); l226 = (l226 + 1)) {
			fRec159[l226] = 0.0f;
			
		}
		for (int l227 = 0; (l227 < 16384); l227 = (l227 + 1)) {
			fVec72[l227] = 0.0f;
			
		}
		for (int l228 = 0; (l228 < 2); l228 = (l228 + 1)) {
			fRec157[l228] = 0.0f;
			
		}
		for (int l229 = 0; (l229 < 2); l229 = (l229 + 1)) {
			fRec155[l229] = 0.0f;
			
		}
		for (int l230 = 0; (l230 < 16384); l230 = (l230 + 1)) {
			fVec73[l230] = 0.0f;
			
		}
		for (int l231 = 0; (l231 < 2); l231 = (l231 + 1)) {
			fRec170[l231] = 0.0f;
			
		}
		for (int l232 = 0; (l232 < 2); l232 = (l232 + 1)) {
			fRec156[l232] = 0.0f;
			
		}
		for (int l233 = 0; (l233 < 16384); l233 = (l233 + 1)) {
			fVec74[l233] = 0.0f;
			
		}
		for (int l234 = 0; (l234 < 2); l234 = (l234 + 1)) {
			fRec13[l234] = 0.0f;
			
		}
		for (int l235 = 0; (l235 < 2); l235 = (l235 + 1)) {
			fRec11[l235] = 0.0f;
			
		}
		for (int l236 = 0; (l236 < 16384); l236 = (l236 + 1)) {
			fVec75[l236] = 0.0f;
			
		}
		for (int l237 = 0; (l237 < 2); l237 = (l237 + 1)) {
			fRec172[l237] = 0.0f;
			
		}
		for (int l238 = 0; (l238 < 2); l238 = (l238 + 1)) {
			fRec171[l238] = 0.0f;
			
		}
		for (int l239 = 0; (l239 < 2); l239 = (l239 + 1)) {
			fRec12[l239] = 0.0f;
			
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
			fRec200[l265] = 0.0f;
			
		}
		for (int l266 = 0; (l266 < 2); l266 = (l266 + 1)) {
			fRec198[l266] = 0.0f;
			
		}
		for (int l267 = 0; (l267 < 16384); l267 = (l267 + 1)) {
			fVec85[l267] = 0.0f;
			
		}
		for (int l268 = 0; (l268 < 2); l268 = (l268 + 1)) {
			fRec201[l268] = 0.0f;
			
		}
		for (int l269 = 0; (l269 < 2); l269 = (l269 + 1)) {
			fRec199[l269] = 0.0f;
			
		}
		for (int l270 = 0; (l270 < 16384); l270 = (l270 + 1)) {
			fVec86[l270] = 0.0f;
			
		}
		for (int l271 = 0; (l271 < 2); l271 = (l271 + 1)) {
			fRec197[l271] = 0.0f;
			
		}
		for (int l272 = 0; (l272 < 2); l272 = (l272 + 1)) {
			fRec195[l272] = 0.0f;
			
		}
		for (int l273 = 0; (l273 < 16384); l273 = (l273 + 1)) {
			fVec87[l273] = 0.0f;
			
		}
		for (int l274 = 0; (l274 < 2); l274 = (l274 + 1)) {
			fRec202[l274] = 0.0f;
			
		}
		for (int l275 = 0; (l275 < 2); l275 = (l275 + 1)) {
			fRec196[l275] = 0.0f;
			
		}
		for (int l276 = 0; (l276 < 16384); l276 = (l276 + 1)) {
			fVec88[l276] = 0.0f;
			
		}
		for (int l277 = 0; (l277 < 2); l277 = (l277 + 1)) {
			fRec194[l277] = 0.0f;
			
		}
		for (int l278 = 0; (l278 < 2); l278 = (l278 + 1)) {
			fRec192[l278] = 0.0f;
			
		}
		for (int l279 = 0; (l279 < 16384); l279 = (l279 + 1)) {
			fVec89[l279] = 0.0f;
			
		}
		for (int l280 = 0; (l280 < 2); l280 = (l280 + 1)) {
			fRec203[l280] = 0.0f;
			
		}
		for (int l281 = 0; (l281 < 2); l281 = (l281 + 1)) {
			fRec193[l281] = 0.0f;
			
		}
		for (int l282 = 0; (l282 < 16384); l282 = (l282 + 1)) {
			fVec90[l282] = 0.0f;
			
		}
		for (int l283 = 0; (l283 < 2); l283 = (l283 + 1)) {
			fRec191[l283] = 0.0f;
			
		}
		for (int l284 = 0; (l284 < 2); l284 = (l284 + 1)) {
			fRec189[l284] = 0.0f;
			
		}
		for (int l285 = 0; (l285 < 16384); l285 = (l285 + 1)) {
			fVec91[l285] = 0.0f;
			
		}
		for (int l286 = 0; (l286 < 2); l286 = (l286 + 1)) {
			fRec204[l286] = 0.0f;
			
		}
		for (int l287 = 0; (l287 < 2); l287 = (l287 + 1)) {
			fRec190[l287] = 0.0f;
			
		}
		for (int l288 = 0; (l288 < 16384); l288 = (l288 + 1)) {
			fVec92[l288] = 0.0f;
			
		}
		for (int l289 = 0; (l289 < 2); l289 = (l289 + 1)) {
			fRec9[l289] = 0.0f;
			
		}
		for (int l290 = 0; (l290 < 2); l290 = (l290 + 1)) {
			fRec7[l290] = 0.0f;
			
		}
		for (int l291 = 0; (l291 < 2); l291 = (l291 + 1)) {
			fRec206[l291] = 0.0f;
			
		}
		for (int l292 = 0; (l292 < 16384); l292 = (l292 + 1)) {
			fVec93[l292] = 0.0f;
			
		}
		for (int l293 = 0; (l293 < 2); l293 = (l293 + 1)) {
			fRec205[l293] = 0.0f;
			
		}
		for (int l294 = 0; (l294 < 2); l294 = (l294 + 1)) {
			fRec8[l294] = 0.0f;
			
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
			fRec234[l320] = 0.0f;
			
		}
		for (int l321 = 0; (l321 < 2); l321 = (l321 + 1)) {
			fRec232[l321] = 0.0f;
			
		}
		for (int l322 = 0; (l322 < 16384); l322 = (l322 + 1)) {
			fVec103[l322] = 0.0f;
			
		}
		for (int l323 = 0; (l323 < 2); l323 = (l323 + 1)) {
			fRec235[l323] = 0.0f;
			
		}
		for (int l324 = 0; (l324 < 2); l324 = (l324 + 1)) {
			fRec233[l324] = 0.0f;
			
		}
		for (int l325 = 0; (l325 < 16384); l325 = (l325 + 1)) {
			fVec104[l325] = 0.0f;
			
		}
		for (int l326 = 0; (l326 < 2); l326 = (l326 + 1)) {
			fRec231[l326] = 0.0f;
			
		}
		for (int l327 = 0; (l327 < 2); l327 = (l327 + 1)) {
			fRec229[l327] = 0.0f;
			
		}
		for (int l328 = 0; (l328 < 16384); l328 = (l328 + 1)) {
			fVec105[l328] = 0.0f;
			
		}
		for (int l329 = 0; (l329 < 2); l329 = (l329 + 1)) {
			fRec236[l329] = 0.0f;
			
		}
		for (int l330 = 0; (l330 < 2); l330 = (l330 + 1)) {
			fRec230[l330] = 0.0f;
			
		}
		for (int l331 = 0; (l331 < 16384); l331 = (l331 + 1)) {
			fVec106[l331] = 0.0f;
			
		}
		for (int l332 = 0; (l332 < 2); l332 = (l332 + 1)) {
			fRec228[l332] = 0.0f;
			
		}
		for (int l333 = 0; (l333 < 2); l333 = (l333 + 1)) {
			fRec226[l333] = 0.0f;
			
		}
		for (int l334 = 0; (l334 < 16384); l334 = (l334 + 1)) {
			fVec107[l334] = 0.0f;
			
		}
		for (int l335 = 0; (l335 < 2); l335 = (l335 + 1)) {
			fRec237[l335] = 0.0f;
			
		}
		for (int l336 = 0; (l336 < 2); l336 = (l336 + 1)) {
			fRec227[l336] = 0.0f;
			
		}
		for (int l337 = 0; (l337 < 16384); l337 = (l337 + 1)) {
			fVec108[l337] = 0.0f;
			
		}
		for (int l338 = 0; (l338 < 2); l338 = (l338 + 1)) {
			fRec225[l338] = 0.0f;
			
		}
		for (int l339 = 0; (l339 < 2); l339 = (l339 + 1)) {
			fRec223[l339] = 0.0f;
			
		}
		for (int l340 = 0; (l340 < 16384); l340 = (l340 + 1)) {
			fVec109[l340] = 0.0f;
			
		}
		for (int l341 = 0; (l341 < 2); l341 = (l341 + 1)) {
			fRec238[l341] = 0.0f;
			
		}
		for (int l342 = 0; (l342 < 2); l342 = (l342 + 1)) {
			fRec224[l342] = 0.0f;
			
		}
		for (int l343 = 0; (l343 < 16384); l343 = (l343 + 1)) {
			fVec110[l343] = 0.0f;
			
		}
		for (int l344 = 0; (l344 < 2); l344 = (l344 + 1)) {
			fRec239[l344] = 0.0f;
			
		}
		for (int l345 = 0; (l345 < 2); l345 = (l345 + 1)) {
			fRec6[l345] = 0.0f;
			
		}
		for (int l346 = 0; (l346 < 2); l346 = (l346 + 1)) {
			fRec4[l346] = 0.0f;
			
		}
		for (int l347 = 0; (l347 < 16384); l347 = (l347 + 1)) {
			fVec111[l347] = 0.0f;
			
		}
		for (int l348 = 0; (l348 < 2); l348 = (l348 + 1)) {
			fRec241[l348] = 0.0f;
			
		}
		for (int l349 = 0; (l349 < 2); l349 = (l349 + 1)) {
			fRec240[l349] = 0.0f;
			
		}
		for (int l350 = 0; (l350 < 2); l350 = (l350 + 1)) {
			fRec5[l350] = 0.0f;
			
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
		for (int l375 = 0; (l375 < 16384); l375 = (l375 + 1)) {
			fVec120[l375] = 0.0f;
			
		}
		for (int l376 = 0; (l376 < 2); l376 = (l376 + 1)) {
			fRec269[l376] = 0.0f;
			
		}
		for (int l377 = 0; (l377 < 2); l377 = (l377 + 1)) {
			fRec267[l377] = 0.0f;
			
		}
		for (int l378 = 0; (l378 < 16384); l378 = (l378 + 1)) {
			fVec121[l378] = 0.0f;
			
		}
		for (int l379 = 0; (l379 < 2); l379 = (l379 + 1)) {
			fRec270[l379] = 0.0f;
			
		}
		for (int l380 = 0; (l380 < 2); l380 = (l380 + 1)) {
			fRec268[l380] = 0.0f;
			
		}
		for (int l381 = 0; (l381 < 16384); l381 = (l381 + 1)) {
			fVec122[l381] = 0.0f;
			
		}
		for (int l382 = 0; (l382 < 2); l382 = (l382 + 1)) {
			fRec266[l382] = 0.0f;
			
		}
		for (int l383 = 0; (l383 < 2); l383 = (l383 + 1)) {
			fRec264[l383] = 0.0f;
			
		}
		for (int l384 = 0; (l384 < 16384); l384 = (l384 + 1)) {
			fVec123[l384] = 0.0f;
			
		}
		for (int l385 = 0; (l385 < 2); l385 = (l385 + 1)) {
			fRec271[l385] = 0.0f;
			
		}
		for (int l386 = 0; (l386 < 2); l386 = (l386 + 1)) {
			fRec265[l386] = 0.0f;
			
		}
		for (int l387 = 0; (l387 < 16384); l387 = (l387 + 1)) {
			fVec124[l387] = 0.0f;
			
		}
		for (int l388 = 0; (l388 < 2); l388 = (l388 + 1)) {
			fRec263[l388] = 0.0f;
			
		}
		for (int l389 = 0; (l389 < 2); l389 = (l389 + 1)) {
			fRec261[l389] = 0.0f;
			
		}
		for (int l390 = 0; (l390 < 16384); l390 = (l390 + 1)) {
			fVec125[l390] = 0.0f;
			
		}
		for (int l391 = 0; (l391 < 2); l391 = (l391 + 1)) {
			fRec272[l391] = 0.0f;
			
		}
		for (int l392 = 0; (l392 < 2); l392 = (l392 + 1)) {
			fRec262[l392] = 0.0f;
			
		}
		for (int l393 = 0; (l393 < 16384); l393 = (l393 + 1)) {
			fVec126[l393] = 0.0f;
			
		}
		for (int l394 = 0; (l394 < 2); l394 = (l394 + 1)) {
			fRec260[l394] = 0.0f;
			
		}
		for (int l395 = 0; (l395 < 2); l395 = (l395 + 1)) {
			fRec258[l395] = 0.0f;
			
		}
		for (int l396 = 0; (l396 < 16384); l396 = (l396 + 1)) {
			fVec127[l396] = 0.0f;
			
		}
		for (int l397 = 0; (l397 < 2); l397 = (l397 + 1)) {
			fRec273[l397] = 0.0f;
			
		}
		for (int l398 = 0; (l398 < 2); l398 = (l398 + 1)) {
			fRec259[l398] = 0.0f;
			
		}
		for (int l399 = 0; (l399 < 2); l399 = (l399 + 1)) {
			fRec3[l399] = 0.0f;
			
		}
		for (int l400 = 0; (l400 < 1024); l400 = (l400 + 1)) {
			fRec1[l400] = 0.0f;
			
		}
		for (int l401 = 0; (l401 < 2); l401 = (l401 + 1)) {
			fRec274[l401] = 0.0f;
			
		}
		for (int l402 = 0; (l402 < 1024); l402 = (l402 + 1)) {
			fRec2[l402] = 0.0f;
			
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
	
	virtual greyhole* clone() {
		return new greyhole();
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
		float fSlow8 = (74.0f * fSlow4);
		int iSlow9 = int(primes(int(fSlow8)));
		float fSlow10 = (0.00100000005f * float(iSlow9));
		float fSlow11 = (87.0f * fSlow4);
		int iSlow12 = int(primes(int(fSlow11)));
		float fSlow13 = (9.99999975e-05f * float(iSlow12));
		float fSlow14 = (29.0f * fSlow4);
		int iSlow15 = int(primes(int(fSlow14)));
		float fSlow16 = (0.00100000005f * float(iSlow15));
		float fSlow17 = (42.0f * fSlow4);
		int iSlow18 = int(primes(int(fSlow17)));
		float fSlow19 = (0.00100000005f * float(iSlow18));
		int iSlow20 = int(primes(int((10.0f * fSlow4))));
		float fSlow21 = (0.00100000005f * float(iSlow20));
		float fSlow22 = (23.0f * fSlow4);
		int iSlow23 = int(primes(int(fSlow22)));
		float fSlow24 = (0.00100000005f * float(iSlow23));
		float fSlow25 = (49.0f * fSlow4);
		int iSlow26 = int(primes(int(fSlow25)));
		float fSlow27 = (9.99999975e-05f * float(iSlow26));
		float fSlow28 = float(fHslider4);
		float fSlow29 = std::floor(std::min(65533.0f, (fConst0 * float(fHslider5))));
		float fSlow30 = float(fHslider6);
		float fSlow31 = float(fHslider7);
		int iSlow32 = int(primes(int((fSlow25 + 10.0f))));
		float fSlow33 = (9.99999975e-05f * float(iSlow32));
		float fSlow34 = (36.0f * fSlow4);
		int iSlow35 = int(primes(int(fSlow34)));
		float fSlow36 = (0.00100000005f * float(iSlow35));
		int iSlow37 = int(primes(int((fSlow34 + 10.0f))));
		float fSlow38 = (0.00100000005f * float(iSlow37));
		int iSlow39 = int(primes(int((fSlow22 + 10.0f))));
		float fSlow40 = (0.00100000005f * float(iSlow39));
		int iSlow41 = int(primes(int((10.0f * (fSlow4 + 1.0f)))));
		float fSlow42 = (0.00100000005f * float(iSlow41));
		float fSlow43 = (68.0f * fSlow4);
		int iSlow44 = int(primes(int(fSlow43)));
		float fSlow45 = (9.99999975e-05f * float(iSlow44));
		int iSlow46 = int(primes(int((fSlow43 + 10.0f))));
		float fSlow47 = (9.99999975e-05f * float(iSlow46));
		float fSlow48 = (55.0f * fSlow4);
		int iSlow49 = int(primes(int(fSlow48)));
		float fSlow50 = (0.00100000005f * float(iSlow49));
		int iSlow51 = int(primes(int((fSlow48 + 10.0f))));
		float fSlow52 = (0.00100000005f * float(iSlow51));
		int iSlow53 = int(primes(int((fSlow17 + 10.0f))));
		float fSlow54 = (0.00100000005f * float(iSlow53));
		int iSlow55 = int(primes(int((fSlow14 + 10.0f))));
		float fSlow56 = (0.00100000005f * float(iSlow55));
		int iSlow57 = int(primes(int((fSlow11 + 10.0f))));
		float fSlow58 = (9.99999975e-05f * float(iSlow57));
		int iSlow59 = int(primes(int((fSlow8 + 10.0f))));
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
			fRec14[0] = (fSlow10 + (0.999000013f * (float((iSlow9 * iTemp7)) + fRec14[1])));
			float fTemp11 = (fRec14[0] + -1.49998999f);
			float fTemp12 = std::floor(fTemp11);
			float fTemp13 = (fTemp12 + (2.0f - fRec14[0]));
			fRec18[0] = (fSlow13 + (0.999899983f * (float((iSlow12 * iTemp7)) + fRec18[1])));
			float fTemp14 = (fRec18[0] + -1.49998999f);
			float fTemp15 = std::floor(fTemp14);
			float fTemp16 = (fTemp15 + (2.0f - fRec18[0]));
			float fTemp17 = std::sin(fTemp5);
			float fTemp18 = (0.0f - fTemp17);
			float fTemp19 = (fTemp18 * fRec12[1]);
			float fTemp20 = (fTemp18 * fRec8[1]);
			float fTemp21 = (fTemp18 * fRec5[1]);
			float fTemp22 = (0.0f - fTemp5);
			float fTemp23 = std::cos(fTemp22);
			fRec22[0] = (fSlow16 + (0.999000013f * (float((iSlow15 * iTemp7)) + fRec22[1])));
			float fTemp24 = (fRec22[0] + -1.49998999f);
			float fTemp25 = std::floor(fTemp24);
			float fTemp26 = (fTemp25 + (2.0f - fRec22[0]));
			fRec26[0] = (fSlow19 + (0.999000013f * (float((iSlow18 * iTemp7)) + fRec26[1])));
			float fTemp27 = (fRec26[0] + -1.49998999f);
			float fTemp28 = std::floor(fTemp27);
			float fTemp29 = (fTemp28 + (2.0f - fRec26[0]));
			float fTemp30 = std::sin(fTemp22);
			fRec30[0] = (fSlow21 + (0.999000013f * (float((iSlow20 * iTemp7)) + fRec30[1])));
			float fTemp31 = (fRec30[0] + -1.49998999f);
			float fTemp32 = std::floor(fTemp31);
			float fTemp33 = (fTemp32 + (2.0f - fRec30[0]));
			fRec34[0] = (fSlow24 + (0.999000013f * (float((iSlow23 * iTemp7)) + fRec34[1])));
			float fTemp34 = (fRec34[0] + -1.49998999f);
			float fTemp35 = std::floor(fTemp34);
			float fTemp36 = (fTemp35 + (2.0f - fRec34[0]));
			fRec41[0] = (fSlow27 + (0.999899983f * (float((iSlow26 * iTemp7)) + fRec41[1])));
			float fTemp37 = (fRec41[0] + -1.49998999f);
			float fTemp38 = std::floor(fTemp37);
			float fTemp39 = (fTemp38 + (2.0f - fRec41[0]));
			float fTemp40 = float(input1[i]);
			float fTemp41 = (iSlow0?0.0f:fTemp40);
			fVec3[0] = fSlow28;
			float fTemp42 = (fSlow28 + fVec3[1]);
			float fTemp43 = ((fRec42[1] != 0.0f)?(((fRec43[1] > 0.0f) & (fRec43[1] < 1.0f))?fRec42[1]:0.0f):(((fRec43[1] == 0.0f) & (fSlow29 != fRec44[1]))?4.53514731e-05f:(((fRec43[1] == 1.0f) & (fSlow29 != fRec45[1]))?-4.53514731e-05f:0.0f)));
			fRec42[0] = fTemp43;
			fRec43[0] = std::max(0.0f, std::min(1.0f, (fRec43[1] + fTemp43)));
			fRec44[0] = (((fRec43[1] >= 1.0f) & (fRec45[1] != fSlow29))?fSlow29:fRec44[1]);
			fRec45[0] = (((fRec43[1] <= 0.0f) & (fRec44[1] != fSlow29))?fSlow29:fRec45[1]);
			float fTemp44 = (1.0f - fRec43[0]);
			fVec4[0] = fSlow30;
			float fTemp45 = (fSlow30 + fVec4[1]);
			fVec5[0] = fSlow31;
			float fTemp46 = (fConst2 * (fSlow31 + fVec5[1]));
			float fTemp47 = std::sin(fTemp46);
			float fTemp48 = std::cos(fTemp46);
			fRec46[0] = ((fTemp47 * fRec47[1]) + (fTemp48 * fRec46[1]));
			fRec47[0] = ((((0.0f - fTemp47) * fRec46[1]) + (fTemp48 * fRec47[1])) + float(iTemp7));
			float fTemp49 = (fConst1 * (fTemp45 * (fRec46[0] + 1.0f)));
			float fTemp50 = (fTemp49 + 8.50000477f);
			int iTemp51 = int(fTemp50);
			float fTemp52 = std::floor(fTemp50);
			float fTemp53 = (fTemp49 + (8.0f - fTemp52));
			float fTemp54 = (fTemp49 + (7.0f - fTemp52));
			float fTemp55 = (fTemp49 + (6.0f - fTemp52));
			float fTemp56 = (((((fRec2[((IOTA - (std::min(512, std::max(0, iTemp51)) + 1)) & 1023)] * (0.0f - (0.5f * fTemp53))) * (fTemp52 + (-9.0f - fTemp49))) * (0.0f - (0.333333343f * fTemp54))) * (0.0f - (0.25f * fTemp55))) + ((fTemp49 + (10.0f - fTemp52)) * ((((fRec2[((IOTA - (std::min(512, std::max(0, (iTemp51 + 1))) + 1)) & 1023)] * (0.0f - (0.5f * fTemp54))) * (fTemp52 + (-8.0f - fTemp49))) * (0.0f - (0.333333343f * fTemp55))) + (((0.5f * ((fRec2[((IOTA - (std::min(512, std::max(0, (iTemp51 + 2))) + 1)) & 1023)] * (0.0f - (0.5f * fTemp55))) * (fTemp52 + (-7.0f - fTemp49)))) + (fTemp53 * ((0.0416666679f * (fTemp54 * fRec2[((IOTA - (std::min(512, std::max(0, (iTemp51 + 4))) + 1)) & 1023)])) + (0.166666672f * (fRec2[((IOTA - (std::min(512, std::max(0, (iTemp51 + 3))) + 1)) & 1023)] * (fTemp52 + (-6.0f - fTemp49))))))) * (fTemp49 + (9.0f - fTemp52))))));
			fVec6[(IOTA & 131071)] = fTemp56;
			int iTemp57 = int(std::min(65536.0f, std::max(0.0f, fRec44[0])));
			int iTemp58 = int(std::min(65536.0f, std::max(0.0f, fRec45[0])));
			float fTemp59 = (fTemp41 + (0.5f * (fTemp42 * ((fTemp44 * fVec6[((IOTA - iTemp57) & 131071)]) + (fRec43[0] * fVec6[((IOTA - iTemp58) & 131071)])))));
			float fTemp60 = (fTemp6 * fTemp59);
			float fTemp61 = ((fTemp18 * fRec28[1]) + fTemp60);
			float fTemp62 = ((fTemp18 * fRec32[1]) + (fTemp6 * fTemp61));
			float fTemp63 = ((fTemp18 * fRec36[1]) + (fTemp6 * fTemp62));
			float fTemp64 = (0.0f - ((fTemp18 * fRec39[1]) + (fTemp6 * fTemp63)));
			fVec7[(IOTA & 16383)] = fTemp64;
			int iTemp65 = std::min(8192, std::max(0, int(fTemp37)));
			float fTemp66 = (fRec41[0] - fTemp38);
			int iTemp67 = (iTemp65 + 1);
			float fTemp68 = (0.0f - (fTemp39 / fTemp66));
			fRec40[0] = ((((fTemp39 * fVec7[((IOTA - iTemp65) & 16383)]) / fTemp66) + fVec7[((IOTA - iTemp67) & 16383)]) + (fTemp68 * fRec40[1]));
			fRec38[0] = fRec40[0];
			float fTemp69 = (fConst1 * (fTemp45 * (fRec47[0] + 1.0f)));
			float fTemp70 = (fTemp69 + 8.50000477f);
			float fTemp71 = std::floor(fTemp70);
			float fTemp72 = (fTemp69 + (8.0f - fTemp71));
			float fTemp73 = (fTemp69 + (7.0f - fTemp71));
			float fTemp74 = (fTemp69 + (6.0f - fTemp71));
			int iTemp75 = int(fTemp70);
			float fTemp76 = ((((((0.0f - (0.5f * fTemp72)) * (fTemp71 + (-9.0f - fTemp69))) * (0.0f - (0.333333343f * fTemp73))) * (0.0f - (0.25f * fTemp74))) * fRec1[((IOTA - (std::min(512, std::max(0, iTemp75)) + 1)) & 1023)]) + ((fTemp69 + (10.0f - fTemp71)) * (((((0.0f - (0.5f * fTemp73)) * (fTemp71 + (-8.0f - fTemp69))) * (0.0f - (0.333333343f * fTemp74))) * fRec1[((IOTA - (std::min(512, std::max(0, (iTemp75 + 1))) + 1)) & 1023)]) + ((fTemp69 + (9.0f - fTemp71)) * ((0.5f * (((0.0f - (0.5f * fTemp74)) * (fTemp71 + (-7.0f - fTemp69))) * fRec1[((IOTA - (std::min(512, std::max(0, (iTemp75 + 2))) + 1)) & 1023)])) + (fTemp72 * ((0.166666672f * ((fTemp71 + (-6.0f - fTemp69)) * fRec1[((IOTA - (std::min(512, std::max(0, (iTemp75 + 3))) + 1)) & 1023)])) + (0.0416666679f * (fTemp73 * fRec1[((IOTA - (std::min(512, std::max(0, (iTemp75 + 4))) + 1)) & 1023)])))))))));
			fVec8[(IOTA & 131071)] = fTemp76;
			float fTemp77 = (fTemp1 + (0.5f * (fTemp42 * ((fTemp44 * fVec8[((IOTA - iTemp57) & 131071)]) + (fRec43[0] * fVec8[((IOTA - iTemp58) & 131071)])))));
			float fTemp78 = (fTemp6 * fTemp77);
			float fTemp79 = ((fTemp18 * fRec27[1]) + fTemp78);
			float fTemp80 = ((fTemp18 * fRec31[1]) + (fTemp6 * fTemp79));
			float fTemp81 = ((fTemp18 * fRec35[1]) + (fTemp6 * fTemp80));
			float fTemp82 = ((fTemp18 * fRec38[1]) + (fTemp6 * fTemp81));
			fVec9[(IOTA & 16383)] = fTemp82;
			fRec49[0] = (fSlow33 + (0.999899983f * (float((iSlow32 * iTemp7)) + fRec49[1])));
			float fTemp83 = (fRec49[0] + -1.49998999f);
			int iTemp84 = std::min(8192, std::max(0, int(fTemp83)));
			int iTemp85 = (iTemp84 + 1);
			float fTemp86 = std::floor(fTemp83);
			float fTemp87 = (fTemp86 + (2.0f - fRec49[0]));
			float fTemp88 = (fRec49[0] - fTemp86);
			float fTemp89 = (0.0f - (fTemp87 / fTemp88));
			fRec48[0] = (fVec9[((IOTA - iTemp85) & 16383)] + (((fTemp87 * fVec9[((IOTA - iTemp84) & 16383)]) / fTemp88) + (fTemp89 * fRec48[1])));
			fRec39[0] = fRec48[0];
			float fTemp90 = (0.0f - ((fTemp6 * fRec39[1]) + (fTemp17 * fTemp63)));
			fVec10[(IOTA & 16383)] = fTemp90;
			fRec50[0] = (fSlow36 + (0.999000013f * (float((iSlow35 * iTemp7)) + fRec50[1])));
			float fTemp91 = (fRec50[0] + -1.49998999f);
			int iTemp92 = std::min(8192, std::max(0, int(fTemp91)));
			int iTemp93 = (iTemp92 + 1);
			float fTemp94 = std::floor(fTemp91);
			float fTemp95 = (fTemp94 + (2.0f - fRec50[0]));
			float fTemp96 = (fRec50[0] - fTemp94);
			float fTemp97 = (0.0f - (fTemp95 / fTemp96));
			fRec37[0] = (fVec10[((IOTA - iTemp93) & 16383)] + (((fTemp95 * fVec10[((IOTA - iTemp92) & 16383)]) / fTemp96) + (fTemp97 * fRec37[1])));
			fRec35[0] = fRec37[0];
			float fTemp98 = ((fTemp6 * fRec38[1]) + (fTemp17 * fTemp81));
			fVec11[(IOTA & 16383)] = fTemp98;
			fRec52[0] = (fSlow38 + (0.999000013f * (float((iSlow37 * iTemp7)) + fRec52[1])));
			float fTemp99 = (fRec52[0] + -1.49998999f);
			int iTemp100 = std::min(8192, std::max(0, int(fTemp99)));
			int iTemp101 = (iTemp100 + 1);
			float fTemp102 = std::floor(fTemp99);
			float fTemp103 = (fTemp102 + (2.0f - fRec52[0]));
			float fTemp104 = (fRec52[0] - fTemp102);
			float fTemp105 = (0.0f - (fTemp103 / fTemp104));
			fRec51[0] = (fVec11[((IOTA - iTemp101) & 16383)] + (((fTemp103 * fVec11[((IOTA - iTemp100) & 16383)]) / fTemp104) + (fTemp105 * fRec51[1])));
			fRec36[0] = fRec51[0];
			float fTemp106 = (0.0f - ((fTemp6 * fRec36[1]) + (fTemp17 * fTemp62)));
			fVec12[(IOTA & 16383)] = fTemp106;
			int iTemp107 = std::min(8192, std::max(0, int(fTemp34)));
			float fTemp108 = (fRec34[0] - fTemp35);
			float fTemp109 = (0.0f - (fTemp36 / fTemp108));
			int iTemp110 = (iTemp107 + 1);
			fRec33[0] = (((fTemp36 * fVec12[((IOTA - iTemp107) & 16383)]) / fTemp108) + ((fRec33[1] * fTemp109) + fVec12[((IOTA - iTemp110) & 16383)]));
			fRec31[0] = fRec33[0];
			fRec54[0] = (fSlow40 + (0.999000013f * (float((iSlow39 * iTemp7)) + fRec54[1])));
			float fTemp111 = (fRec54[0] + -1.49998999f);
			float fTemp112 = std::floor(fTemp111);
			float fTemp113 = (fTemp112 + (2.0f - fRec54[0]));
			float fTemp114 = (fRec54[0] - fTemp112);
			float fTemp115 = (0.0f - (fTemp113 / fTemp114));
			float fTemp116 = ((fTemp6 * fRec35[1]) + (fTemp17 * fTemp80));
			fVec13[(IOTA & 16383)] = fTemp116;
			int iTemp117 = std::min(8192, std::max(0, int(fTemp111)));
			int iTemp118 = (iTemp117 + 1);
			fRec53[0] = (((fTemp115 * fRec53[1]) + ((fTemp113 * fVec13[((IOTA - iTemp117) & 16383)]) / fTemp114)) + fVec13[((IOTA - iTemp118) & 16383)]);
			fRec32[0] = fRec53[0];
			float fTemp119 = (0.0f - ((fTemp6 * fRec32[1]) + (fTemp17 * fTemp61)));
			fVec14[(IOTA & 16383)] = fTemp119;
			int iTemp120 = std::min(8192, std::max(0, int(fTemp31)));
			float fTemp121 = (fRec30[0] - fTemp32);
			float fTemp122 = (0.0f - (fTemp33 / fTemp121));
			int iTemp123 = (iTemp120 + 1);
			fRec29[0] = (((fTemp33 * fVec14[((IOTA - iTemp120) & 16383)]) / fTemp121) + ((fRec29[1] * fTemp122) + fVec14[((IOTA - iTemp123) & 16383)]));
			fRec27[0] = fRec29[0];
			fRec56[0] = (fSlow42 + (0.999000013f * (float((iSlow41 * iTemp7)) + fRec56[1])));
			float fTemp124 = (fRec56[0] + -1.49998999f);
			float fTemp125 = std::floor(fTemp124);
			float fTemp126 = (fTemp125 + (2.0f - fRec56[0]));
			float fTemp127 = ((fTemp6 * fRec31[1]) + (fTemp17 * fTemp79));
			fVec15[(IOTA & 16383)] = fTemp127;
			int iTemp128 = std::min(8192, std::max(0, int(fTemp124)));
			float fTemp129 = (fRec56[0] - fTemp125);
			int iTemp130 = (iTemp128 + 1);
			float fTemp131 = (0.0f - (fTemp126 / fTemp129));
			fRec55[0] = ((((fTemp126 * fVec15[((IOTA - iTemp128) & 16383)]) / fTemp129) + fVec15[((IOTA - iTemp130) & 16383)]) + (fTemp131 * fRec55[1]));
			fRec28[0] = fRec55[0];
			float fTemp132 = (fTemp17 * fTemp59);
			float fTemp133 = (fTemp23 * ((fTemp6 * fRec28[1]) + fTemp132));
			float fTemp134 = (0.0f - fTemp30);
			float fTemp135 = (fTemp134 * fRec20[1]);
			float fTemp136 = (fTemp134 * fRec24[1]);
			float fTemp137 = ((fTemp18 * fRec64[1]) + fTemp60);
			float fTemp138 = ((fTemp18 * fRec67[1]) + (fTemp6 * fTemp137));
			float fTemp139 = ((fTemp18 * fRec70[1]) + (fTemp6 * fTemp138));
			float fTemp140 = (0.0f - ((fTemp18 * fRec73[1]) + (fTemp6 * fTemp139)));
			fVec16[(IOTA & 16383)] = fTemp140;
			fRec74[0] = (((fRec74[1] * fTemp68) + fVec16[((IOTA - iTemp67) & 16383)]) + ((fTemp39 * fVec16[((IOTA - iTemp65) & 16383)]) / fTemp66));
			fRec72[0] = fRec74[0];
			float fTemp141 = ((fTemp18 * fRec63[1]) + fTemp78);
			float fTemp142 = ((fTemp18 * fRec66[1]) + (fTemp6 * fTemp141));
			float fTemp143 = ((fTemp18 * fRec69[1]) + (fTemp6 * fTemp142));
			float fTemp144 = ((fTemp18 * fRec72[1]) + (fTemp6 * fTemp143));
			fVec17[(IOTA & 16383)] = fTemp144;
			fRec75[0] = (((fRec75[1] * fTemp89) + fVec17[((IOTA - iTemp85) & 16383)]) + ((fTemp87 * fVec17[((IOTA - iTemp84) & 16383)]) / fTemp88));
			fRec73[0] = fRec75[0];
			float fTemp145 = (0.0f - ((fTemp6 * fRec73[1]) + (fTemp17 * fTemp139)));
			fVec18[(IOTA & 16383)] = fTemp145;
			fRec71[0] = (fVec18[((IOTA - iTemp93) & 16383)] + ((fRec71[1] * fTemp97) + ((fTemp95 * fVec18[((IOTA - iTemp92) & 16383)]) / fTemp96)));
			fRec69[0] = fRec71[0];
			float fTemp146 = ((fTemp6 * fRec72[1]) + (fTemp17 * fTemp143));
			fVec19[(IOTA & 16383)] = fTemp146;
			fRec76[0] = (fVec19[((IOTA - iTemp101) & 16383)] + ((fRec76[1] * fTemp105) + ((fTemp103 * fVec19[((IOTA - iTemp100) & 16383)]) / fTemp104)));
			fRec70[0] = fRec76[0];
			float fTemp147 = (0.0f - ((fTemp6 * fRec70[1]) + (fTemp17 * fTemp138)));
			fVec20[(IOTA & 16383)] = fTemp147;
			fRec68[0] = (((fRec68[1] * fTemp109) + ((fTemp36 * fVec20[((IOTA - iTemp107) & 16383)]) / fTemp108)) + fVec20[((IOTA - iTemp110) & 16383)]);
			fRec66[0] = fRec68[0];
			float fTemp148 = ((fTemp6 * fRec69[1]) + (fTemp17 * fTemp142));
			fVec21[(IOTA & 16383)] = fTemp148;
			fRec77[0] = (fVec21[((IOTA - iTemp118) & 16383)] + ((fRec77[1] * fTemp115) + ((fTemp113 * fVec21[((IOTA - iTemp117) & 16383)]) / fTemp114)));
			fRec67[0] = fRec77[0];
			float fTemp149 = (0.0f - ((fTemp6 * fRec67[1]) + (fTemp17 * fTemp137)));
			fVec22[(IOTA & 16383)] = fTemp149;
			fRec65[0] = (((fTemp33 * fVec22[((IOTA - iTemp120) & 16383)]) / fTemp121) + ((fRec65[1] * fTemp122) + fVec22[((IOTA - iTemp123) & 16383)]));
			fRec63[0] = fRec65[0];
			float fTemp150 = ((fTemp6 * fRec66[1]) + (fTemp17 * fTemp141));
			fVec23[(IOTA & 16383)] = fTemp150;
			fRec78[0] = (((fTemp126 * fVec23[((IOTA - iTemp128) & 16383)]) / fTemp129) + ((fRec78[1] * fTemp131) + fVec23[((IOTA - iTemp130) & 16383)]));
			fRec64[0] = fRec78[0];
			float fTemp151 = (fTemp134 * fRec58[1]);
			float fTemp152 = (0.0f - ((fTemp23 * ((fTemp23 * ((fTemp23 * ((fTemp23 * ((fTemp6 * fRec64[1]) + fTemp132)) + fTemp135)) + fTemp136)) + fTemp151)) + (fTemp134 * fRec61[1])));
			fVec24[(IOTA & 16383)] = fTemp152;
			fRec79[0] = (fSlow45 + (0.999899983f * (float((iSlow44 * iTemp7)) + fRec79[1])));
			float fTemp153 = (fRec79[0] + -1.49998999f);
			int iTemp154 = std::min(8192, std::max(0, int(fTemp153)));
			int iTemp155 = (iTemp154 + 1);
			float fTemp156 = std::floor(fTemp153);
			float fTemp157 = (fTemp156 + (2.0f - fRec79[0]));
			float fTemp158 = (fRec79[0] - fTemp156);
			float fTemp159 = (0.0f - (fTemp157 / fTemp158));
			fRec62[0] = (fVec24[((IOTA - iTemp155) & 16383)] + ((fRec62[1] * fTemp159) + ((fTemp157 * fVec24[((IOTA - iTemp154) & 16383)]) / fTemp158)));
			fRec60[0] = fRec62[0];
			fRec81[0] = (fSlow47 + (0.999899983f * (float((iSlow46 * iTemp7)) + fRec81[1])));
			float fTemp160 = (fRec81[0] + -1.49998999f);
			float fTemp161 = std::floor(fTemp160);
			float fTemp162 = (fTemp161 + (2.0f - fRec81[0]));
			float fTemp163 = (fTemp17 * fTemp77);
			float fTemp164 = (fTemp134 * fRec19[1]);
			float fTemp165 = (fTemp134 * fRec23[1]);
			float fTemp166 = (fTemp134 * fRec57[1]);
			float fTemp167 = ((fTemp23 * ((fTemp23 * ((fTemp23 * ((fTemp23 * ((fTemp6 * fRec63[1]) + fTemp163)) + fTemp164)) + fTemp165)) + fTemp166)) + (fTemp134 * fRec60[1]));
			fVec25[(IOTA & 16383)] = fTemp167;
			int iTemp168 = std::min(8192, std::max(0, int(fTemp160)));
			float fTemp169 = (fRec81[0] - fTemp161);
			float fTemp170 = (0.0f - (fTemp162 / fTemp169));
			int iTemp171 = (iTemp168 + 1);
			fRec80[0] = (((fTemp162 * fVec25[((IOTA - iTemp168) & 16383)]) / fTemp169) + ((fRec80[1] * fTemp170) + fVec25[((IOTA - iTemp171) & 16383)]));
			fRec61[0] = fRec80[0];
			float fTemp172 = ((fTemp18 * fRec83[1]) + fTemp60);
			float fTemp173 = ((fTemp18 * fRec86[1]) + (fTemp6 * fTemp172));
			float fTemp174 = ((fTemp18 * fRec89[1]) + (fTemp6 * fTemp173));
			float fTemp175 = (0.0f - ((fTemp18 * fRec92[1]) + (fTemp6 * fTemp174)));
			fVec26[(IOTA & 16383)] = fTemp175;
			fRec93[0] = (((fTemp39 * fVec26[((IOTA - iTemp65) & 16383)]) / fTemp66) + ((fTemp68 * fRec93[1]) + fVec26[((IOTA - iTemp67) & 16383)]));
			fRec91[0] = fRec93[0];
			float fTemp176 = ((fTemp18 * fRec82[1]) + fTemp78);
			float fTemp177 = ((fTemp18 * fRec85[1]) + (fTemp6 * fTemp176));
			float fTemp178 = ((fTemp18 * fRec88[1]) + (fTemp6 * fTemp177));
			float fTemp179 = ((fTemp18 * fRec91[1]) + (fTemp6 * fTemp178));
			fVec27[(IOTA & 16383)] = fTemp179;
			fRec94[0] = (((fTemp89 * fRec94[1]) + fVec27[((IOTA - iTemp85) & 16383)]) + ((fTemp87 * fVec27[((IOTA - iTemp84) & 16383)]) / fTemp88));
			fRec92[0] = fRec94[0];
			float fTemp180 = (0.0f - ((fTemp6 * fRec92[1]) + (fTemp17 * fTemp174)));
			fVec28[(IOTA & 16383)] = fTemp180;
			fRec90[0] = (fVec28[((IOTA - iTemp93) & 16383)] + (((fTemp95 * fVec28[((IOTA - iTemp92) & 16383)]) / fTemp96) + (fTemp97 * fRec90[1])));
			fRec88[0] = fRec90[0];
			float fTemp181 = ((fTemp6 * fRec91[1]) + (fTemp17 * fTemp178));
			fVec29[(IOTA & 16383)] = fTemp181;
			fRec95[0] = (fVec29[((IOTA - iTemp101) & 16383)] + (((fTemp103 * fVec29[((IOTA - iTemp100) & 16383)]) / fTemp104) + (fTemp105 * fRec95[1])));
			fRec89[0] = fRec95[0];
			float fTemp182 = (0.0f - ((fTemp6 * fRec89[1]) + (fTemp17 * fTemp173)));
			fVec30[(IOTA & 16383)] = fTemp182;
			fRec87[0] = (((fTemp36 * fVec30[((IOTA - iTemp107) & 16383)]) / fTemp108) + ((fTemp109 * fRec87[1]) + fVec30[((IOTA - iTemp110) & 16383)]));
			fRec85[0] = fRec87[0];
			float fTemp183 = ((fTemp6 * fRec88[1]) + (fTemp17 * fTemp177));
			fVec31[(IOTA & 16383)] = fTemp183;
			fRec96[0] = (((fTemp115 * fRec96[1]) + ((fTemp113 * fVec31[((IOTA - iTemp117) & 16383)]) / fTemp114)) + fVec31[((IOTA - iTemp118) & 16383)]);
			fRec86[0] = fRec96[0];
			float fTemp184 = (0.0f - ((fTemp6 * fRec86[1]) + (fTemp17 * fTemp172)));
			fVec32[(IOTA & 16383)] = fTemp184;
			fRec84[0] = (((fTemp33 * fVec32[((IOTA - iTemp120) & 16383)]) / fTemp121) + ((fTemp122 * fRec84[1]) + fVec32[((IOTA - iTemp123) & 16383)]));
			fRec82[0] = fRec84[0];
			float fTemp185 = ((fTemp6 * fRec85[1]) + (fTemp17 * fTemp176));
			fVec33[(IOTA & 16383)] = fTemp185;
			fRec97[0] = (fVec33[((IOTA - iTemp130) & 16383)] + (((fTemp126 * fVec33[((IOTA - iTemp128) & 16383)]) / fTemp129) + (fTemp131 * fRec97[1])));
			fRec83[0] = fRec97[0];
			float fTemp186 = (fTemp23 * ((fTemp6 * fRec83[1]) + fTemp132));
			float fTemp187 = (0.0f - ((fTemp23 * fRec61[1]) + (fTemp30 * (fTemp151 + (fTemp23 * (fTemp136 + (fTemp23 * (fTemp135 + fTemp186))))))));
			fVec34[(IOTA & 16383)] = fTemp187;
			fRec98[0] = (fSlow50 + (0.999000013f * (float((iSlow49 * iTemp7)) + fRec98[1])));
			float fTemp188 = (fRec98[0] + -1.49998999f);
			int iTemp189 = std::min(8192, std::max(0, int(fTemp188)));
			int iTemp190 = (iTemp189 + 1);
			float fTemp191 = std::floor(fTemp188);
			float fTemp192 = (fTemp191 + (2.0f - fRec98[0]));
			float fTemp193 = (fRec98[0] - fTemp191);
			float fTemp194 = (0.0f - (fTemp192 / fTemp193));
			fRec59[0] = (fVec34[((IOTA - iTemp190) & 16383)] + ((fRec59[1] * fTemp194) + ((fTemp192 * fVec34[((IOTA - iTemp189) & 16383)]) / fTemp193)));
			fRec57[0] = fRec59[0];
			float fTemp195 = (fTemp23 * ((fTemp6 * fRec82[1]) + fTemp163));
			float fTemp196 = ((fTemp23 * fRec60[1]) + (fTemp30 * (fTemp166 + (fTemp23 * (fTemp165 + (fTemp23 * (fTemp164 + fTemp195)))))));
			fVec35[(IOTA & 16383)] = fTemp196;
			fRec100[0] = (fSlow52 + (0.999000013f * (float((iSlow51 * iTemp7)) + fRec100[1])));
			float fTemp197 = (fRec100[0] + -1.49998999f);
			int iTemp198 = std::min(8192, std::max(0, int(fTemp197)));
			int iTemp199 = (iTemp198 + 1);
			float fTemp200 = std::floor(fTemp197);
			float fTemp201 = (fTemp200 + (2.0f - fRec100[0]));
			float fTemp202 = (fRec100[0] - fTemp200);
			float fTemp203 = (0.0f - (fTemp201 / fTemp202));
			fRec99[0] = (fVec35[((IOTA - iTemp199) & 16383)] + ((fRec99[1] * fTemp203) + ((fTemp201 * fVec35[((IOTA - iTemp198) & 16383)]) / fTemp202)));
			fRec58[0] = fRec99[0];
			float fTemp204 = (0.0f - ((fTemp30 * ((fTemp23 * (fTemp133 + fTemp135)) + fTemp136)) + (fTemp23 * fRec58[1])));
			fVec36[(IOTA & 16383)] = fTemp204;
			int iTemp205 = std::min(8192, std::max(0, int(fTemp27)));
			float fTemp206 = (fRec26[0] - fTemp28);
			float fTemp207 = (0.0f - (fTemp29 / fTemp206));
			int iTemp208 = (iTemp205 + 1);
			fRec25[0] = (((fTemp29 * fVec36[((IOTA - iTemp205) & 16383)]) / fTemp206) + ((fRec25[1] * fTemp207) + fVec36[((IOTA - iTemp208) & 16383)]));
			fRec23[0] = fRec25[0];
			fRec102[0] = (fSlow54 + (0.999000013f * (float((iSlow53 * iTemp7)) + fRec102[1])));
			float fTemp209 = (fRec102[0] + -1.49998999f);
			float fTemp210 = std::floor(fTemp209);
			float fTemp211 = (fTemp210 + (2.0f - fRec102[0]));
			float fTemp212 = (fTemp23 * ((fTemp6 * fRec27[1]) + fTemp163));
			float fTemp213 = ((fTemp23 * fRec57[1]) + (fTemp30 * ((fTemp23 * (fTemp212 + fTemp164)) + fTemp165)));
			fVec37[(IOTA & 16383)] = fTemp213;
			int iTemp214 = std::min(8192, std::max(0, int(fTemp209)));
			float fTemp215 = (fRec102[0] - fTemp210);
			float fTemp216 = (0.0f - (fTemp211 / fTemp215));
			int iTemp217 = (iTemp214 + 1);
			fRec101[0] = (((fTemp211 * fVec37[((IOTA - iTemp214) & 16383)]) / fTemp215) + ((fRec101[1] * fTemp216) + fVec37[((IOTA - iTemp217) & 16383)]));
			fRec24[0] = fRec101[0];
			float fTemp218 = ((fTemp18 * fRec104[1]) + fTemp60);
			float fTemp219 = ((fTemp18 * fRec107[1]) + (fTemp6 * fTemp218));
			float fTemp220 = ((fTemp18 * fRec110[1]) + (fTemp6 * fTemp219));
			float fTemp221 = (0.0f - ((fTemp18 * fRec113[1]) + (fTemp6 * fTemp220)));
			fVec38[(IOTA & 16383)] = fTemp221;
			fRec114[0] = (((fTemp68 * fRec114[1]) + ((fTemp39 * fVec38[((IOTA - iTemp65) & 16383)]) / fTemp66)) + fVec38[((IOTA - iTemp67) & 16383)]);
			fRec112[0] = fRec114[0];
			float fTemp222 = ((fTemp18 * fRec103[1]) + fTemp78);
			float fTemp223 = ((fTemp18 * fRec106[1]) + (fTemp6 * fTemp222));
			float fTemp224 = ((fTemp18 * fRec109[1]) + (fTemp6 * fTemp223));
			float fTemp225 = ((fTemp18 * fRec112[1]) + (fTemp6 * fTemp224));
			fVec39[(IOTA & 16383)] = fTemp225;
			fRec115[0] = (((fTemp89 * fRec115[1]) + fVec39[((IOTA - iTemp85) & 16383)]) + ((fTemp87 * fVec39[((IOTA - iTemp84) & 16383)]) / fTemp88));
			fRec113[0] = fRec115[0];
			float fTemp226 = (0.0f - ((fTemp6 * fRec113[1]) + (fTemp17 * fTemp220)));
			fVec40[(IOTA & 16383)] = fTemp226;
			fRec111[0] = (((fTemp97 * fRec111[1]) + fVec40[((IOTA - iTemp93) & 16383)]) + ((fTemp95 * fVec40[((IOTA - iTemp92) & 16383)]) / fTemp96));
			fRec109[0] = fRec111[0];
			float fTemp227 = ((fTemp6 * fRec112[1]) + (fTemp17 * fTemp224));
			fVec41[(IOTA & 16383)] = fTemp227;
			fRec116[0] = (((fTemp105 * fRec116[1]) + fVec41[((IOTA - iTemp101) & 16383)]) + ((fTemp103 * fVec41[((IOTA - iTemp100) & 16383)]) / fTemp104));
			fRec110[0] = fRec116[0];
			float fTemp228 = (0.0f - ((fTemp6 * fRec110[1]) + (fTemp17 * fTemp219)));
			fVec42[(IOTA & 16383)] = fTemp228;
			fRec108[0] = ((((fTemp36 * fVec42[((IOTA - iTemp107) & 16383)]) / fTemp108) + fVec42[((IOTA - iTemp110) & 16383)]) + (fTemp109 * fRec108[1]));
			fRec106[0] = fRec108[0];
			float fTemp229 = ((fTemp6 * fRec109[1]) + (fTemp17 * fTemp223));
			fVec43[(IOTA & 16383)] = fTemp229;
			fRec117[0] = ((((fTemp113 * fVec43[((IOTA - iTemp117) & 16383)]) / fTemp114) + fVec43[((IOTA - iTemp118) & 16383)]) + (fTemp115 * fRec117[1]));
			fRec107[0] = fRec117[0];
			float fTemp230 = (0.0f - ((fTemp6 * fRec107[1]) + (fTemp17 * fTemp218)));
			fVec44[(IOTA & 16383)] = fTemp230;
			fRec105[0] = (((fTemp33 * fVec44[((IOTA - iTemp120) & 16383)]) / fTemp121) + ((fTemp122 * fRec105[1]) + fVec44[((IOTA - iTemp123) & 16383)]));
			fRec103[0] = fRec105[0];
			float fTemp231 = ((fTemp17 * fTemp222) + (fTemp6 * fRec106[1]));
			fVec45[(IOTA & 16383)] = fTemp231;
			fRec118[0] = ((fVec45[((IOTA - iTemp130) & 16383)] + ((fTemp126 * fVec45[((IOTA - iTemp128) & 16383)]) / fTemp129)) + (fTemp131 * fRec118[1]));
			fRec104[0] = fRec118[0];
			float fTemp232 = (fTemp23 * ((fTemp6 * fRec104[1]) + fTemp132));
			float fTemp233 = (0.0f - ((fTemp23 * fRec24[1]) + (fTemp30 * (fTemp232 + fTemp135))));
			fVec46[(IOTA & 16383)] = fTemp233;
			int iTemp234 = std::min(8192, std::max(0, int(fTemp24)));
			float fTemp235 = (fRec22[0] - fTemp25);
			float fTemp236 = (0.0f - (fTemp26 / fTemp235));
			int iTemp237 = (iTemp234 + 1);
			fRec21[0] = (((fTemp26 * fVec46[((IOTA - iTemp234) & 16383)]) / fTemp235) + ((fRec21[1] * fTemp236) + fVec46[((IOTA - iTemp237) & 16383)]));
			fRec19[0] = fRec21[0];
			fRec120[0] = (fSlow56 + (0.999000013f * (float((iSlow55 * iTemp7)) + fRec120[1])));
			float fTemp238 = (fRec120[0] + -1.49998999f);
			float fTemp239 = std::floor(fTemp238);
			float fTemp240 = (fTemp239 + (2.0f - fRec120[0]));
			float fTemp241 = (fTemp23 * (fTemp163 + (fTemp6 * fRec103[1])));
			float fTemp242 = ((fTemp23 * fRec23[1]) + (fTemp30 * (fTemp164 + fTemp241)));
			fVec47[(IOTA & 16383)] = fTemp242;
			int iTemp243 = std::min(8192, std::max(0, int(fTemp238)));
			float fTemp244 = (fRec120[0] - fTemp239);
			float fTemp245 = (0.0f - (fTemp240 / fTemp244));
			int iTemp246 = (iTemp243 + 1);
			fRec119[0] = (((fTemp240 * fVec47[((IOTA - iTemp243) & 16383)]) / fTemp244) + ((fRec119[1] * fTemp245) + fVec47[((IOTA - iTemp246) & 16383)]));
			fRec20[0] = fRec119[0];
			float fTemp247 = ((fTemp18 * fRec122[1]) + fTemp60);
			float fTemp248 = ((fTemp18 * fRec125[1]) + (fTemp6 * fTemp247));
			float fTemp249 = ((fTemp18 * fRec128[1]) + (fTemp6 * fTemp248));
			float fTemp250 = (0.0f - ((fTemp18 * fRec131[1]) + (fTemp6 * fTemp249)));
			fVec48[(IOTA & 16383)] = fTemp250;
			fRec132[0] = (((fTemp68 * fRec132[1]) + ((fTemp39 * fVec48[((IOTA - iTemp65) & 16383)]) / fTemp66)) + fVec48[((IOTA - iTemp67) & 16383)]);
			fRec130[0] = fRec132[0];
			float fTemp251 = ((fTemp18 * fRec121[1]) + fTemp78);
			float fTemp252 = ((fTemp18 * fRec124[1]) + (fTemp6 * fTemp251));
			float fTemp253 = ((fTemp18 * fRec127[1]) + (fTemp6 * fTemp252));
			float fTemp254 = ((fTemp18 * fRec130[1]) + (fTemp6 * fTemp253));
			fVec49[(IOTA & 16383)] = fTemp254;
			fRec133[0] = (((fTemp89 * fRec133[1]) + fVec49[((IOTA - iTemp85) & 16383)]) + ((fTemp87 * fVec49[((IOTA - iTemp84) & 16383)]) / fTemp88));
			fRec131[0] = fRec133[0];
			float fTemp255 = (0.0f - ((fTemp6 * fRec131[1]) + (fTemp17 * fTemp249)));
			fVec50[(IOTA & 16383)] = fTemp255;
			fRec129[0] = (fVec50[((IOTA - iTemp93) & 16383)] + (((fTemp95 * fVec50[((IOTA - iTemp92) & 16383)]) / fTemp96) + (fTemp97 * fRec129[1])));
			fRec127[0] = fRec129[0];
			float fTemp256 = ((fTemp6 * fRec130[1]) + (fTemp17 * fTemp253));
			fVec51[(IOTA & 16383)] = fTemp256;
			fRec134[0] = (fVec51[((IOTA - iTemp101) & 16383)] + (((fTemp103 * fVec51[((IOTA - iTemp100) & 16383)]) / fTemp104) + (fTemp105 * fRec134[1])));
			fRec128[0] = fRec134[0];
			float fTemp257 = (0.0f - ((fTemp6 * fRec128[1]) + (fTemp17 * fTemp248)));
			fVec52[(IOTA & 16383)] = fTemp257;
			fRec126[0] = (((fTemp36 * fVec52[((IOTA - iTemp107) & 16383)]) / fTemp108) + ((fTemp109 * fRec126[1]) + fVec52[((IOTA - iTemp110) & 16383)]));
			fRec124[0] = fRec126[0];
			float fTemp258 = ((fTemp6 * fRec127[1]) + (fTemp17 * fTemp252));
			fVec53[(IOTA & 16383)] = fTemp258;
			fRec135[0] = (((fTemp113 * fVec53[((IOTA - iTemp117) & 16383)]) / fTemp114) + (fVec53[((IOTA - iTemp118) & 16383)] + (fTemp115 * fRec135[1])));
			fRec125[0] = fRec135[0];
			float fTemp259 = (0.0f - ((fTemp6 * fRec125[1]) + (fTemp17 * fTemp247)));
			fVec54[(IOTA & 16383)] = fTemp259;
			fRec123[0] = (((fTemp33 * fVec54[((IOTA - iTemp120) & 16383)]) / fTemp121) + ((fTemp122 * fRec123[1]) + fVec54[((IOTA - iTemp123) & 16383)]));
			fRec121[0] = fRec123[0];
			float fTemp260 = ((fTemp6 * fRec124[1]) + (fTemp17 * fTemp251));
			fVec55[(IOTA & 16383)] = fTemp260;
			fRec136[0] = (fVec55[((IOTA - iTemp130) & 16383)] + (((fTemp126 * fVec55[((IOTA - iTemp128) & 16383)]) / fTemp129) + (fTemp131 * fRec136[1])));
			fRec122[0] = fRec136[0];
			float fTemp261 = ((fTemp6 * fRec122[1]) + fTemp132);
			float fTemp262 = (0.0f - ((fTemp18 * fRec16[1]) + (fTemp6 * (fTemp19 + (fTemp6 * (fTemp20 + (fTemp6 * (fTemp21 + (fTemp6 * ((fTemp23 * fRec20[1]) + (fTemp30 * fTemp261)))))))))));
			fVec56[(IOTA & 16383)] = fTemp262;
			int iTemp263 = std::min(8192, std::max(0, int(fTemp14)));
			float fTemp264 = (fRec18[0] - fTemp15);
			fRec17[0] = (((fTemp16 * fVec56[((IOTA - iTemp263) & 16383)]) / fTemp264) + ((fRec17[1] * (0.0f - (fTemp16 / fTemp264))) + fVec56[((IOTA - (iTemp263 + 1)) & 16383)]));
			fRec15[0] = fRec17[0];
			fRec138[0] = (fSlow58 + (0.999899983f * (float((iSlow57 * iTemp7)) + fRec138[1])));
			float fTemp265 = (fRec138[0] + -1.49998999f);
			float fTemp266 = std::floor(fTemp265);
			float fTemp267 = (fTemp266 + (2.0f - fRec138[0]));
			float fTemp268 = (fTemp18 * fRec11[1]);
			float fTemp269 = (fTemp18 * fRec7[1]);
			float fTemp270 = (fTemp18 * fRec4[1]);
			float fTemp271 = ((fTemp6 * fRec121[1]) + fTemp163);
			float fTemp272 = ((fTemp18 * fRec15[1]) + (fTemp6 * (fTemp268 + (fTemp6 * (fTemp269 + (fTemp6 * (fTemp270 + (fTemp6 * ((fTemp23 * fRec19[1]) + (fTemp30 * fTemp271))))))))));
			fVec57[(IOTA & 16383)] = fTemp272;
			int iTemp273 = std::min(8192, std::max(0, int(fTemp265)));
			float fTemp274 = (fRec138[0] - fTemp266);
			fRec137[0] = (((fTemp267 * fVec57[((IOTA - iTemp273) & 16383)]) / fTemp274) + ((fRec137[1] * (0.0f - (fTemp267 / fTemp274))) + fVec57[((IOTA - (iTemp273 + 1)) & 16383)]));
			fRec16[0] = fRec137[0];
			float fTemp275 = (fTemp134 * fRec146[1]);
			float fTemp276 = (fTemp134 * fRec143[1]);
			float fTemp277 = (fTemp134 * fRec140[1]);
			float fTemp278 = (0.0f - ((fTemp134 * fRec149[1]) + (fTemp23 * (fTemp275 + (fTemp23 * (fTemp276 + (fTemp23 * (fTemp186 + fTemp277))))))));
			fVec58[(IOTA & 16383)] = fTemp278;
			fRec150[0] = (fVec58[((IOTA - iTemp155) & 16383)] + (((fTemp157 * fVec58[((IOTA - iTemp154) & 16383)]) / fTemp158) + (fTemp159 * fRec150[1])));
			fRec148[0] = fRec150[0];
			float fTemp279 = (fTemp134 * fRec145[1]);
			float fTemp280 = (fTemp134 * fRec142[1]);
			float fTemp281 = (fTemp134 * fRec139[1]);
			float fTemp282 = ((fTemp134 * fRec148[1]) + (fTemp23 * (fTemp279 + (fTemp23 * (fTemp280 + (fTemp23 * (fTemp195 + fTemp281)))))));
			fVec59[(IOTA & 16383)] = fTemp282;
			fRec151[0] = (fVec59[((IOTA - iTemp171) & 16383)] + (((fTemp162 * fVec59[((IOTA - iTemp168) & 16383)]) / fTemp169) + (fTemp170 * fRec151[1])));
			fRec149[0] = fRec151[0];
			float fTemp283 = (0.0f - ((fTemp23 * fRec149[1]) + (fTemp30 * (fTemp275 + (fTemp23 * (fTemp276 + (fTemp23 * (fTemp133 + fTemp277))))))));
			fVec60[(IOTA & 16383)] = fTemp283;
			fRec147[0] = (((fTemp192 * fVec60[((IOTA - iTemp189) & 16383)]) / fTemp193) + ((fTemp194 * fRec147[1]) + fVec60[((IOTA - iTemp190) & 16383)]));
			fRec145[0] = fRec147[0];
			float fTemp284 = ((fTemp23 * fRec148[1]) + (fTemp30 * (fTemp279 + (fTemp23 * (fTemp280 + (fTemp23 * (fTemp212 + fTemp281)))))));
			fVec61[(IOTA & 16383)] = fTemp284;
			fRec152[0] = (((fTemp201 * fVec61[((IOTA - iTemp198) & 16383)]) / fTemp202) + (fVec61[((IOTA - iTemp199) & 16383)] + (fTemp203 * fRec152[1])));
			fRec146[0] = fRec152[0];
			float fTemp285 = (0.0f - ((fTemp23 * fRec146[1]) + (fTemp30 * (fTemp276 + (fTemp23 * (fTemp232 + fTemp277))))));
			fVec62[(IOTA & 16383)] = fTemp285;
			fRec144[0] = (fVec62[((IOTA - iTemp208) & 16383)] + (((fTemp29 * fVec62[((IOTA - iTemp205) & 16383)]) / fTemp206) + (fTemp207 * fRec144[1])));
			fRec142[0] = fRec144[0];
			float fTemp286 = ((fTemp23 * fRec145[1]) + (fTemp30 * (fTemp280 + (fTemp23 * (fTemp241 + fTemp281)))));
			fVec63[(IOTA & 16383)] = fTemp286;
			fRec153[0] = (((fTemp211 * fVec63[((IOTA - iTemp214) & 16383)]) / fTemp215) + (fVec63[((IOTA - iTemp217) & 16383)] + (fTemp216 * fRec153[1])));
			fRec143[0] = fRec153[0];
			float fTemp287 = (fTemp23 * fTemp261);
			float fTemp288 = (0.0f - ((fTemp23 * fRec143[1]) + (fTemp30 * (fTemp277 + fTemp287))));
			fVec64[(IOTA & 16383)] = fTemp288;
			fRec141[0] = (((fTemp26 * fVec64[((IOTA - iTemp234) & 16383)]) / fTemp235) + ((fTemp236 * fRec141[1]) + fVec64[((IOTA - iTemp237) & 16383)]));
			fRec139[0] = fRec141[0];
			float fTemp289 = (fTemp23 * fTemp271);
			float fTemp290 = ((fTemp23 * fRec142[1]) + (fTemp30 * (fTemp281 + fTemp289)));
			fVec65[(IOTA & 16383)] = fTemp290;
			fRec154[0] = (((fTemp240 * fVec65[((IOTA - iTemp243) & 16383)]) / fTemp244) + (fVec65[((IOTA - iTemp246) & 16383)] + (fTemp245 * fRec154[1])));
			fRec140[0] = fRec154[0];
			float fTemp291 = ((fTemp18 * fRec156[1]) + fTemp60);
			float fTemp292 = ((fTemp18 * fRec159[1]) + (fTemp6 * fTemp291));
			float fTemp293 = ((fTemp18 * fRec162[1]) + (fTemp6 * fTemp292));
			float fTemp294 = (0.0f - ((fTemp18 * fRec165[1]) + (fTemp6 * fTemp293)));
			fVec66[(IOTA & 16383)] = fTemp294;
			fRec166[0] = (fVec66[((IOTA - iTemp67) & 16383)] + (((fTemp39 * fVec66[((IOTA - iTemp65) & 16383)]) / fTemp66) + (fTemp68 * fRec166[1])));
			fRec164[0] = fRec166[0];
			float fTemp295 = ((fTemp18 * fRec155[1]) + fTemp78);
			float fTemp296 = ((fTemp18 * fRec158[1]) + (fTemp6 * fTemp295));
			float fTemp297 = ((fTemp18 * fRec161[1]) + (fTemp6 * fTemp296));
			float fTemp298 = ((fTemp18 * fRec164[1]) + (fTemp6 * fTemp297));
			fVec67[(IOTA & 16383)] = fTemp298;
			fRec167[0] = (((fTemp87 * fVec67[((IOTA - iTemp84) & 16383)]) / fTemp88) + (fVec67[((IOTA - iTemp85) & 16383)] + (fTemp89 * fRec167[1])));
			fRec165[0] = fRec167[0];
			float fTemp299 = (0.0f - ((fTemp6 * fRec165[1]) + (fTemp17 * fTemp293)));
			fVec68[(IOTA & 16383)] = fTemp299;
			fRec163[0] = ((fVec68[((IOTA - iTemp93) & 16383)] + ((fTemp95 * fVec68[((IOTA - iTemp92) & 16383)]) / fTemp96)) + (fTemp97 * fRec163[1]));
			fRec161[0] = fRec163[0];
			float fTemp300 = ((fTemp6 * fRec164[1]) + (fTemp17 * fTemp297));
			fVec69[(IOTA & 16383)] = fTemp300;
			fRec168[0] = ((fVec69[((IOTA - iTemp101) & 16383)] + ((fTemp103 * fVec69[((IOTA - iTemp100) & 16383)]) / fTemp104)) + (fTemp105 * fRec168[1]));
			fRec162[0] = fRec168[0];
			float fTemp301 = (0.0f - ((fTemp6 * fRec162[1]) + (fTemp17 * fTemp292)));
			fVec70[(IOTA & 16383)] = fTemp301;
			fRec160[0] = ((((fTemp36 * fVec70[((IOTA - iTemp107) & 16383)]) / fTemp108) + fVec70[((IOTA - iTemp110) & 16383)]) + (fTemp109 * fRec160[1]));
			fRec158[0] = fRec160[0];
			float fTemp302 = ((fTemp6 * fRec161[1]) + (fTemp17 * fTemp296));
			fVec71[(IOTA & 16383)] = fTemp302;
			fRec169[0] = ((((fTemp113 * fVec71[((IOTA - iTemp117) & 16383)]) / fTemp114) + fVec71[((IOTA - iTemp118) & 16383)]) + (fTemp115 * fRec169[1]));
			fRec159[0] = fRec169[0];
			float fTemp303 = (0.0f - ((fTemp6 * fRec159[1]) + (fTemp17 * fTemp291)));
			fVec72[(IOTA & 16383)] = fTemp303;
			fRec157[0] = (fVec72[((IOTA - iTemp123) & 16383)] + (((fTemp33 * fVec72[((IOTA - iTemp120) & 16383)]) / fTemp121) + (fTemp122 * fRec157[1])));
			fRec155[0] = fRec157[0];
			float fTemp304 = ((fTemp17 * fTemp295) + (fTemp6 * fRec158[1]));
			fVec73[(IOTA & 16383)] = fTemp304;
			fRec170[0] = ((fVec73[((IOTA - iTemp130) & 16383)] + ((fTemp126 * fVec73[((IOTA - iTemp128) & 16383)]) / fTemp129)) + (fTemp131 * fRec170[1]));
			fRec156[0] = fRec170[0];
			float fTemp305 = (fTemp132 + (fTemp6 * fRec156[1]));
			float fTemp306 = (0.0f - ((fTemp6 * fRec16[1]) + (fTemp17 * (fTemp19 + (fTemp6 * (fTemp20 + (fTemp6 * (fTemp21 + (fTemp6 * ((fTemp23 * fRec140[1]) + (fTemp30 * fTemp305)))))))))));
			fVec74[(IOTA & 16383)] = fTemp306;
			int iTemp307 = std::min(8192, std::max(0, int(fTemp11)));
			float fTemp308 = (fRec14[0] - fTemp12);
			fRec13[0] = (((fTemp13 * fVec74[((IOTA - iTemp307) & 16383)]) / fTemp308) + ((fRec13[1] * (0.0f - (fTemp13 / fTemp308))) + fVec74[((IOTA - (iTemp307 + 1)) & 16383)]));
			fRec11[0] = fRec13[0];
			float fTemp309 = ((fTemp6 * fRec155[1]) + fTemp163);
			float fTemp310 = ((fTemp6 * fRec15[1]) + (fTemp17 * (fTemp268 + (fTemp6 * (fTemp269 + (fTemp6 * (fTemp270 + (fTemp6 * ((fTemp23 * fRec139[1]) + (fTemp30 * fTemp309))))))))));
			fVec75[(IOTA & 16383)] = fTemp310;
			fRec172[0] = (fSlow60 + (0.999000013f * (float((iSlow59 * iTemp7)) + fRec172[1])));
			float fTemp311 = (fRec172[0] + -1.49998999f);
			int iTemp312 = std::min(8192, std::max(0, int(fTemp311)));
			float fTemp313 = std::floor(fTemp311);
			float fTemp314 = (fTemp313 + (2.0f - fRec172[0]));
			float fTemp315 = (fRec172[0] - fTemp313);
			fRec171[0] = (fVec75[((IOTA - (iTemp312 + 1)) & 16383)] + ((fRec171[1] * (0.0f - (fTemp314 / fTemp315))) + ((fTemp314 * fVec75[((IOTA - iTemp312) & 16383)]) / fTemp315)));
			fRec12[0] = fRec171[0];
			float fTemp316 = (fTemp134 * fRec180[1]);
			float fTemp317 = (fTemp134 * fRec177[1]);
			float fTemp318 = (fTemp134 * fRec174[1]);
			float fTemp319 = (0.0f - ((fTemp134 * fRec183[1]) + (fTemp23 * (fTemp316 + (fTemp23 * (fTemp317 + (fTemp23 * (fTemp133 + fTemp318))))))));
			fVec76[(IOTA & 16383)] = fTemp319;
			fRec184[0] = (fVec76[((IOTA - iTemp155) & 16383)] + ((fRec184[1] * fTemp159) + ((fTemp157 * fVec76[((IOTA - iTemp154) & 16383)]) / fTemp158)));
			fRec182[0] = fRec184[0];
			float fTemp320 = (fTemp134 * fRec179[1]);
			float fTemp321 = (fTemp134 * fRec176[1]);
			float fTemp322 = (fTemp134 * fRec173[1]);
			float fTemp323 = ((fTemp134 * fRec182[1]) + (fTemp23 * (fTemp320 + (fTemp23 * (fTemp321 + (fTemp23 * (fTemp212 + fTemp322)))))));
			fVec77[(IOTA & 16383)] = fTemp323;
			fRec185[0] = (fVec77[((IOTA - iTemp171) & 16383)] + ((fRec185[1] * fTemp170) + ((fTemp162 * fVec77[((IOTA - iTemp168) & 16383)]) / fTemp169)));
			fRec183[0] = fRec185[0];
			float fTemp324 = (0.0f - ((fTemp23 * fRec183[1]) + (fTemp30 * (fTemp316 + (fTemp23 * (fTemp317 + (fTemp23 * (fTemp232 + fTemp318))))))));
			fVec78[(IOTA & 16383)] = fTemp324;
			fRec181[0] = (((fTemp192 * fVec78[((IOTA - iTemp189) & 16383)]) / fTemp193) + ((fTemp194 * fRec181[1]) + fVec78[((IOTA - iTemp190) & 16383)]));
			fRec179[0] = fRec181[0];
			float fTemp325 = ((fTemp23 * fRec182[1]) + (fTemp30 * (fTemp320 + (fTemp23 * (fTemp321 + (fTemp23 * (fTemp322 + fTemp241)))))));
			fVec79[(IOTA & 16383)] = fTemp325;
			fRec186[0] = (((fTemp201 * fVec79[((IOTA - iTemp198) & 16383)]) / fTemp202) + ((fRec186[1] * fTemp203) + fVec79[((IOTA - iTemp199) & 16383)]));
			fRec180[0] = fRec186[0];
			float fTemp326 = (0.0f - ((fTemp23 * fRec180[1]) + (fTemp30 * (fTemp317 + (fTemp23 * (fTemp318 + fTemp287))))));
			fVec80[(IOTA & 16383)] = fTemp326;
			fRec178[0] = (fVec80[((IOTA - iTemp208) & 16383)] + (((fTemp29 * fVec80[((IOTA - iTemp205) & 16383)]) / fTemp206) + (fTemp207 * fRec178[1])));
			fRec176[0] = fRec178[0];
			float fTemp327 = ((fTemp23 * fRec179[1]) + (fTemp30 * (fTemp321 + (fTemp23 * (fTemp322 + fTemp289)))));
			fVec81[(IOTA & 16383)] = fTemp327;
			fRec187[0] = (((fTemp211 * fVec81[((IOTA - iTemp214) & 16383)]) / fTemp215) + ((fRec187[1] * fTemp216) + fVec81[((IOTA - iTemp217) & 16383)]));
			fRec177[0] = fRec187[0];
			float fTemp328 = (fTemp23 * fTemp305);
			float fTemp329 = (0.0f - ((fTemp23 * fRec177[1]) + (fTemp30 * (fTemp318 + fTemp328))));
			fVec82[(IOTA & 16383)] = fTemp329;
			fRec175[0] = (((fTemp26 * fVec82[((IOTA - iTemp234) & 16383)]) / fTemp235) + ((fTemp236 * fRec175[1]) + fVec82[((IOTA - iTemp237) & 16383)]));
			fRec173[0] = fRec175[0];
			float fTemp330 = (fTemp23 * fTemp309);
			float fTemp331 = ((fTemp23 * fRec176[1]) + (fTemp30 * (fTemp322 + fTemp330)));
			fVec83[(IOTA & 16383)] = fTemp331;
			fRec188[0] = (((fTemp240 * fVec83[((IOTA - iTemp243) & 16383)]) / fTemp244) + ((fRec188[1] * fTemp245) + fVec83[((IOTA - iTemp246) & 16383)]));
			fRec174[0] = fRec188[0];
			float fTemp332 = ((fRec190[1] * fTemp18) + fTemp60);
			float fTemp333 = ((fTemp18 * fRec193[1]) + (fTemp6 * fTemp332));
			float fTemp334 = ((fTemp18 * fRec196[1]) + (fTemp6 * fTemp333));
			float fTemp335 = (0.0f - ((fTemp18 * fRec199[1]) + (fTemp6 * fTemp334)));
			fVec84[(IOTA & 16383)] = fTemp335;
			fRec200[0] = (((fTemp39 * fVec84[((IOTA - iTemp65) & 16383)]) / fTemp66) + (fVec84[((IOTA - iTemp67) & 16383)] + (fTemp68 * fRec200[1])));
			fRec198[0] = fRec200[0];
			float fTemp336 = ((fRec189[1] * fTemp18) + fTemp78);
			float fTemp337 = ((fTemp18 * fRec192[1]) + (fTemp6 * fTemp336));
			float fTemp338 = ((fTemp18 * fRec195[1]) + (fTemp6 * fTemp337));
			float fTemp339 = ((fTemp18 * fRec198[1]) + (fTemp6 * fTemp338));
			fVec85[(IOTA & 16383)] = fTemp339;
			fRec201[0] = (fVec85[((IOTA - iTemp85) & 16383)] + (((fTemp87 * fVec85[((IOTA - iTemp84) & 16383)]) / fTemp88) + (fTemp89 * fRec201[1])));
			fRec199[0] = fRec201[0];
			float fTemp340 = (0.0f - ((fTemp6 * fRec199[1]) + (fTemp17 * fTemp334)));
			fVec86[(IOTA & 16383)] = fTemp340;
			fRec197[0] = (fVec86[((IOTA - iTemp93) & 16383)] + (((fTemp95 * fVec86[((IOTA - iTemp92) & 16383)]) / fTemp96) + (fTemp97 * fRec197[1])));
			fRec195[0] = fRec197[0];
			float fTemp341 = ((fTemp6 * fRec198[1]) + (fTemp17 * fTemp338));
			fVec87[(IOTA & 16383)] = fTemp341;
			fRec202[0] = (fVec87[((IOTA - iTemp101) & 16383)] + (((fTemp103 * fVec87[((IOTA - iTemp100) & 16383)]) / fTemp104) + (fTemp105 * fRec202[1])));
			fRec196[0] = fRec202[0];
			float fTemp342 = (0.0f - ((fTemp6 * fRec196[1]) + (fTemp17 * fTemp333)));
			fVec88[(IOTA & 16383)] = fTemp342;
			fRec194[0] = (fVec88[((IOTA - iTemp110) & 16383)] + (((fTemp36 * fVec88[((IOTA - iTemp107) & 16383)]) / fTemp108) + (fTemp109 * fRec194[1])));
			fRec192[0] = fRec194[0];
			float fTemp343 = ((fTemp6 * fRec195[1]) + (fTemp17 * fTemp337));
			fVec89[(IOTA & 16383)] = fTemp343;
			fRec203[0] = (((fTemp113 * fVec89[((IOTA - iTemp117) & 16383)]) / fTemp114) + (fVec89[((IOTA - iTemp118) & 16383)] + (fTemp115 * fRec203[1])));
			fRec193[0] = fRec203[0];
			float fTemp344 = (0.0f - ((fTemp6 * fRec193[1]) + (fTemp17 * fTemp332)));
			fVec90[(IOTA & 16383)] = fTemp344;
			fRec191[0] = (fVec90[((IOTA - iTemp123) & 16383)] + ((fRec191[1] * fTemp122) + ((fTemp33 * fVec90[((IOTA - iTemp120) & 16383)]) / fTemp121)));
			fRec189[0] = fRec191[0];
			float fTemp345 = ((fTemp6 * fRec192[1]) + (fTemp17 * fTemp336));
			fVec91[(IOTA & 16383)] = fTemp345;
			fRec204[0] = (fVec91[((IOTA - iTemp130) & 16383)] + (((fTemp126 * fVec91[((IOTA - iTemp128) & 16383)]) / fTemp129) + (fTemp131 * fRec204[1])));
			fRec190[0] = fRec204[0];
			float fTemp346 = (fTemp132 + (fRec190[1] * fTemp6));
			float fTemp347 = (0.0f - ((fTemp6 * fRec12[1]) + (fTemp17 * (fTemp20 + (fTemp6 * (fTemp21 + (fTemp6 * ((fTemp23 * fRec174[1]) + (fTemp30 * fTemp346)))))))));
			fVec92[(IOTA & 16383)] = fTemp347;
			int iTemp348 = std::min(8192, std::max(0, int(fTemp8)));
			float fTemp349 = (fRec10[0] - fTemp9);
			fRec9[0] = (((fTemp10 * fVec92[((IOTA - iTemp348) & 16383)]) / fTemp349) + ((fRec9[1] * (0.0f - (fTemp10 / fTemp349))) + fVec92[((IOTA - (iTemp348 + 1)) & 16383)]));
			fRec7[0] = fRec9[0];
			fRec206[0] = (fSlow62 + (0.999000013f * (float((iSlow61 * iTemp7)) + fRec206[1])));
			float fTemp350 = (fRec206[0] + -1.49998999f);
			float fTemp351 = std::floor(fTemp350);
			float fTemp352 = (fTemp351 + (2.0f - fRec206[0]));
			float fTemp353 = (fTemp163 + (fRec189[1] * fTemp6));
			float fTemp354 = ((fTemp6 * fRec11[1]) + (fTemp17 * (fTemp269 + (fTemp6 * (fTemp270 + (fTemp6 * ((fTemp23 * fRec173[1]) + (fTemp30 * fTemp353))))))));
			fVec93[(IOTA & 16383)] = fTemp354;
			int iTemp355 = std::min(8192, std::max(0, int(fTemp350)));
			float fTemp356 = (fRec206[0] - fTemp351);
			fRec205[0] = (((fTemp352 * fVec93[((IOTA - iTemp355) & 16383)]) / fTemp356) + ((fRec205[1] * (0.0f - (fTemp352 / fTemp356))) + fVec93[((IOTA - (iTemp355 + 1)) & 16383)]));
			fRec8[0] = fRec205[0];
			float fTemp357 = (fTemp134 * fRec208[1]);
			float fTemp358 = (fTemp134 * fRec211[1]);
			float fTemp359 = (fTemp134 * fRec214[1]);
			float fTemp360 = (0.0f - ((fTemp23 * ((fTemp23 * ((fTemp23 * (fTemp232 + fTemp357)) + fTemp358)) + fTemp359)) + (fTemp134 * fRec217[1])));
			fVec94[(IOTA & 16383)] = fTemp360;
			fRec218[0] = ((((fTemp157 * fVec94[((IOTA - iTemp154) & 16383)]) / fTemp158) + fVec94[((IOTA - iTemp155) & 16383)]) + (fTemp159 * fRec218[1]));
			fRec216[0] = fRec218[0];
			float fTemp361 = (fTemp134 * fRec207[1]);
			float fTemp362 = (fTemp134 * fRec210[1]);
			float fTemp363 = (fTemp134 * fRec213[1]);
			float fTemp364 = ((fTemp23 * ((fTemp23 * ((fTemp23 * (fTemp241 + fTemp361)) + fTemp362)) + fTemp363)) + (fTemp134 * fRec216[1]));
			fVec95[(IOTA & 16383)] = fTemp364;
			fRec219[0] = ((((fTemp162 * fVec95[((IOTA - iTemp168) & 16383)]) / fTemp169) + fVec95[((IOTA - iTemp171) & 16383)]) + (fTemp170 * fRec219[1]));
			fRec217[0] = fRec219[0];
			float fTemp365 = (0.0f - ((fTemp23 * fRec217[1]) + (fTemp30 * (fTemp359 + (fTemp23 * (fTemp358 + (fTemp23 * (fTemp287 + fTemp357))))))));
			fVec96[(IOTA & 16383)] = fTemp365;
			fRec215[0] = ((fVec96[((IOTA - iTemp190) & 16383)] + ((fTemp192 * fVec96[((IOTA - iTemp189) & 16383)]) / fTemp193)) + (fTemp194 * fRec215[1]));
			fRec213[0] = fRec215[0];
			float fTemp366 = ((fTemp23 * fRec216[1]) + (fTemp30 * (fTemp363 + (fTemp23 * (fTemp362 + (fTemp23 * (fTemp289 + fTemp361)))))));
			fVec97[(IOTA & 16383)] = fTemp366;
			fRec220[0] = ((fVec97[((IOTA - iTemp199) & 16383)] + ((fTemp201 * fVec97[((IOTA - iTemp198) & 16383)]) / fTemp202)) + (fTemp203 * fRec220[1]));
			fRec214[0] = fRec220[0];
			float fTemp367 = (0.0f - ((fTemp23 * fRec214[1]) + (fTemp30 * (fTemp358 + (fTemp23 * (fTemp328 + fTemp357))))));
			fVec98[(IOTA & 16383)] = fTemp367;
			fRec212[0] = ((((fTemp29 * fVec98[((IOTA - iTemp205) & 16383)]) / fTemp206) + fVec98[((IOTA - iTemp208) & 16383)]) + (fTemp207 * fRec212[1]));
			fRec210[0] = fRec212[0];
			float fTemp368 = ((fTemp23 * fRec213[1]) + (fTemp30 * (fTemp362 + (fTemp23 * (fTemp330 + fTemp361)))));
			fVec99[(IOTA & 16383)] = fTemp368;
			fRec221[0] = (((fTemp211 * fVec99[((IOTA - iTemp214) & 16383)]) / fTemp215) + ((fRec221[1] * fTemp216) + fVec99[((IOTA - iTemp217) & 16383)]));
			fRec211[0] = fRec221[0];
			float fTemp369 = (fTemp23 * fTemp346);
			float fTemp370 = (0.0f - ((fTemp23 * fRec211[1]) + (fTemp30 * (fTemp357 + fTemp369))));
			fVec100[(IOTA & 16383)] = fTemp370;
			fRec209[0] = ((fVec100[((IOTA - iTemp237) & 16383)] + ((fTemp26 * fVec100[((IOTA - iTemp234) & 16383)]) / fTemp235)) + (fTemp236 * fRec209[1]));
			fRec207[0] = fRec209[0];
			float fTemp371 = (fTemp23 * fTemp353);
			float fTemp372 = ((fTemp23 * fRec210[1]) + (fTemp30 * (fTemp361 + fTemp371)));
			fVec101[(IOTA & 16383)] = fTemp372;
			fRec222[0] = (((fTemp240 * fVec101[((IOTA - iTemp243) & 16383)]) / fTemp244) + ((fRec222[1] * fTemp245) + fVec101[((IOTA - iTemp246) & 16383)]));
			fRec208[0] = fRec222[0];
			float fTemp373 = ((fTemp18 * fRec224[1]) + fTemp60);
			float fTemp374 = ((fTemp18 * fRec227[1]) + (fTemp6 * fTemp373));
			float fTemp375 = ((fTemp18 * fRec230[1]) + (fTemp6 * fTemp374));
			float fTemp376 = (0.0f - ((fTemp18 * fRec233[1]) + (fTemp6 * fTemp375)));
			fVec102[(IOTA & 16383)] = fTemp376;
			fRec234[0] = ((((fTemp39 * fVec102[((IOTA - iTemp65) & 16383)]) / fTemp66) + fVec102[((IOTA - iTemp67) & 16383)]) + (fTemp68 * fRec234[1]));
			fRec232[0] = fRec234[0];
			float fTemp377 = ((fTemp18 * fRec223[1]) + fTemp78);
			float fTemp378 = ((fTemp18 * fRec226[1]) + (fTemp6 * fTemp377));
			float fTemp379 = ((fTemp18 * fRec229[1]) + (fTemp6 * fTemp378));
			float fTemp380 = ((fTemp18 * fRec232[1]) + (fTemp6 * fTemp379));
			fVec103[(IOTA & 16383)] = fTemp380;
			fRec235[0] = (fVec103[((IOTA - iTemp85) & 16383)] + (((fTemp87 * fVec103[((IOTA - iTemp84) & 16383)]) / fTemp88) + (fTemp89 * fRec235[1])));
			fRec233[0] = fRec235[0];
			float fTemp381 = (0.0f - ((fTemp6 * fRec233[1]) + (fTemp17 * fTemp375)));
			fVec104[(IOTA & 16383)] = fTemp381;
			fRec231[0] = (fVec104[((IOTA - iTemp93) & 16383)] + (((fTemp95 * fVec104[((IOTA - iTemp92) & 16383)]) / fTemp96) + (fTemp97 * fRec231[1])));
			fRec229[0] = fRec231[0];
			float fTemp382 = ((fTemp6 * fRec232[1]) + (fTemp17 * fTemp379));
			fVec105[(IOTA & 16383)] = fTemp382;
			fRec236[0] = ((((fTemp103 * fVec105[((IOTA - iTemp100) & 16383)]) / fTemp104) + fVec105[((IOTA - iTemp101) & 16383)]) + (fTemp105 * fRec236[1]));
			fRec230[0] = fRec236[0];
			float fTemp383 = (0.0f - ((fTemp17 * fTemp374) + (fTemp6 * fRec230[1])));
			fVec106[(IOTA & 16383)] = fTemp383;
			fRec228[0] = ((((fTemp36 * fVec106[((IOTA - iTemp107) & 16383)]) / fTemp108) + fVec106[((IOTA - iTemp110) & 16383)]) + (fTemp109 * fRec228[1]));
			fRec226[0] = fRec228[0];
			float fTemp384 = ((fTemp6 * fRec229[1]) + (fTemp17 * fTemp378));
			fVec107[(IOTA & 16383)] = fTemp384;
			fRec237[0] = ((fVec107[((IOTA - iTemp118) & 16383)] + ((fTemp113 * fVec107[((IOTA - iTemp117) & 16383)]) / fTemp114)) + (fTemp115 * fRec237[1]));
			fRec227[0] = fRec237[0];
			float fTemp385 = (0.0f - ((fTemp6 * fRec227[1]) + (fTemp17 * fTemp373)));
			fVec108[(IOTA & 16383)] = fTemp385;
			fRec225[0] = (fVec108[((IOTA - iTemp123) & 16383)] + (((fTemp33 * fVec108[((IOTA - iTemp120) & 16383)]) / fTemp121) + (fTemp122 * fRec225[1])));
			fRec223[0] = fRec225[0];
			float fTemp386 = ((fTemp6 * fRec226[1]) + (fTemp17 * fTemp377));
			fVec109[(IOTA & 16383)] = fTemp386;
			fRec238[0] = (fVec109[((IOTA - iTemp130) & 16383)] + (((fTemp126 * fVec109[((IOTA - iTemp128) & 16383)]) / fTemp129) + (fTemp131 * fRec238[1])));
			fRec224[0] = fRec238[0];
			float fTemp387 = (fTemp132 + (fTemp6 * fRec224[1]));
			float fTemp388 = (0.0f - ((fTemp6 * fRec8[1]) + (fTemp17 * (fTemp21 + (fTemp6 * ((fTemp23 * fRec208[1]) + (fTemp30 * fTemp387)))))));
			fVec110[(IOTA & 16383)] = fTemp388;
			fRec239[0] = (fSlow65 + (0.999000013f * (float((iSlow64 * iTemp7)) + fRec239[1])));
			float fTemp389 = (fRec239[0] + -1.49998999f);
			int iTemp390 = std::min(8192, std::max(0, int(fTemp389)));
			float fTemp391 = std::floor(fTemp389);
			float fTemp392 = (fTemp391 + (2.0f - fRec239[0]));
			float fTemp393 = (fRec239[0] - fTemp391);
			fRec6[0] = (fVec110[((IOTA - (iTemp390 + 1)) & 16383)] + ((fRec6[1] * (0.0f - (fTemp392 / fTemp393))) + ((fTemp392 * fVec110[((IOTA - iTemp390) & 16383)]) / fTemp393)));
			fRec4[0] = fRec6[0];
			float fTemp394 = (fTemp163 + (fTemp6 * fRec223[1]));
			float fTemp395 = ((fTemp6 * fRec7[1]) + (fTemp17 * (fTemp270 + (fTemp6 * ((fTemp23 * fRec207[1]) + (fTemp30 * fTemp394))))));
			fVec111[(IOTA & 16383)] = fTemp395;
			fRec241[0] = (fSlow67 + (0.999000013f * (float((iSlow66 * iTemp7)) + fRec241[1])));
			float fTemp396 = (fRec241[0] + -1.49998999f);
			int iTemp397 = std::min(8192, std::max(0, int(fTemp396)));
			float fTemp398 = std::floor(fTemp396);
			float fTemp399 = (fTemp398 + (2.0f - fRec241[0]));
			float fTemp400 = (fRec241[0] - fTemp398);
			fRec240[0] = (fVec111[((IOTA - (iTemp397 + 1)) & 16383)] + ((fRec240[1] * (0.0f - (fTemp399 / fTemp400))) + ((fTemp399 * fVec111[((IOTA - iTemp397) & 16383)]) / fTemp400)));
			fRec5[0] = fRec240[0];
			float fTemp401 = (fTemp134 * fRec246[1]);
			float fTemp402 = (fTemp134 * fRec243[1]);
			float fTemp403 = (fTemp134 * fRec249[1]);
			float fTemp404 = (0.0f - ((fTemp134 * fRec252[1]) + (fTemp23 * ((fTemp23 * (fTemp401 + (fTemp23 * (fTemp287 + fTemp402)))) + fTemp403))));
			fVec112[(IOTA & 16383)] = fTemp404;
			fRec253[0] = (((fTemp157 * fVec112[((IOTA - iTemp154) & 16383)]) / fTemp158) + (fVec112[((IOTA - iTemp155) & 16383)] + (fTemp159 * fRec253[1])));
			fRec251[0] = fRec253[0];
			float fTemp405 = (fTemp134 * fRec248[1]);
			float fTemp406 = (fTemp134 * fRec242[1]);
			float fTemp407 = (fTemp134 * fRec245[1]);
			float fTemp408 = ((fTemp134 * fRec251[1]) + (fTemp23 * (fTemp405 + (fTemp23 * ((fTemp23 * (fTemp289 + fTemp406)) + fTemp407)))));
			fVec113[(IOTA & 16383)] = fTemp408;
			fRec254[0] = ((((fTemp162 * fVec113[((IOTA - iTemp168) & 16383)]) / fTemp169) + fVec113[((IOTA - iTemp171) & 16383)]) + (fTemp170 * fRec254[1]));
			fRec252[0] = fRec254[0];
			float fTemp409 = (0.0f - ((fTemp23 * fRec252[1]) + (fTemp30 * (fTemp403 + (fTemp23 * (fTemp401 + (fTemp23 * (fTemp328 + fTemp402))))))));
			fVec114[(IOTA & 16383)] = fTemp409;
			fRec250[0] = ((fVec114[((IOTA - iTemp190) & 16383)] + ((fTemp192 * fVec114[((IOTA - iTemp189) & 16383)]) / fTemp193)) + (fTemp194 * fRec250[1]));
			fRec248[0] = fRec250[0];
			float fTemp410 = ((fTemp23 * fRec251[1]) + (fTemp30 * (fTemp405 + (fTemp23 * (fTemp407 + (fTemp23 * (fTemp330 + fTemp406)))))));
			fVec115[(IOTA & 16383)] = fTemp410;
			fRec255[0] = ((fVec115[((IOTA - iTemp199) & 16383)] + ((fTemp201 * fVec115[((IOTA - iTemp198) & 16383)]) / fTemp202)) + (fTemp203 * fRec255[1]));
			fRec249[0] = fRec255[0];
			float fTemp411 = (0.0f - ((fTemp23 * fRec249[1]) + (fTemp30 * (fTemp401 + (fTemp23 * (fTemp369 + fTemp402))))));
			fVec116[(IOTA & 16383)] = fTemp411;
			fRec247[0] = (fVec116[((IOTA - iTemp208) & 16383)] + ((fRec247[1] * fTemp207) + ((fTemp29 * fVec116[((IOTA - iTemp205) & 16383)]) / fTemp206)));
			fRec245[0] = fRec247[0];
			float fTemp412 = ((fTemp23 * fRec248[1]) + (fTemp30 * (fTemp407 + (fTemp23 * (fTemp371 + fTemp406)))));
			fVec117[(IOTA & 16383)] = fTemp412;
			fRec256[0] = (((fTemp211 * fVec117[((IOTA - iTemp214) & 16383)]) / fTemp215) + ((fRec256[1] * fTemp216) + fVec117[((IOTA - iTemp217) & 16383)]));
			fRec246[0] = fRec256[0];
			float fTemp413 = (0.0f - ((fTemp23 * fRec246[1]) + (fTemp30 * (fTemp402 + (fTemp23 * fTemp387)))));
			fVec118[(IOTA & 16383)] = fTemp413;
			fRec244[0] = (fVec118[((IOTA - iTemp237) & 16383)] + ((fRec244[1] * fTemp236) + ((fTemp26 * fVec118[((IOTA - iTemp234) & 16383)]) / fTemp235)));
			fRec242[0] = fRec244[0];
			float fTemp414 = ((fTemp23 * fRec245[1]) + (fTemp30 * (fTemp406 + (fTemp23 * fTemp394))));
			fVec119[(IOTA & 16383)] = fTemp414;
			fRec257[0] = (fVec119[((IOTA - iTemp246) & 16383)] + ((fRec257[1] * fTemp245) + ((fTemp240 * fVec119[((IOTA - iTemp243) & 16383)]) / fTemp244)));
			fRec243[0] = fRec257[0];
			float fTemp415 = ((fTemp18 * fRec259[1]) + fTemp60);
			float fTemp416 = ((fTemp18 * fRec262[1]) + (fTemp6 * fTemp415));
			float fTemp417 = ((fTemp18 * fRec265[1]) + (fTemp6 * fTemp416));
			float fTemp418 = (0.0f - ((fTemp18 * fRec268[1]) + (fTemp6 * fTemp417)));
			fVec120[(IOTA & 16383)] = fTemp418;
			fRec269[0] = (((fTemp39 * fVec120[((IOTA - iTemp65) & 16383)]) / fTemp66) + ((fTemp68 * fRec269[1]) + fVec120[((IOTA - iTemp67) & 16383)]));
			fRec267[0] = fRec269[0];
			float fTemp419 = ((fTemp18 * fRec258[1]) + fTemp78);
			float fTemp420 = ((fTemp18 * fRec261[1]) + (fTemp6 * fTemp419));
			float fTemp421 = ((fTemp18 * fRec264[1]) + (fTemp6 * fTemp420));
			float fTemp422 = ((fTemp18 * fRec267[1]) + (fTemp6 * fTemp421));
			fVec121[(IOTA & 16383)] = fTemp422;
			fRec270[0] = (fVec121[((IOTA - iTemp85) & 16383)] + (((fTemp87 * fVec121[((IOTA - iTemp84) & 16383)]) / fTemp88) + (fTemp89 * fRec270[1])));
			fRec268[0] = fRec270[0];
			float fTemp423 = (0.0f - ((fTemp6 * fRec268[1]) + (fTemp17 * fTemp417)));
			fVec122[(IOTA & 16383)] = fTemp423;
			fRec266[0] = (fVec122[((IOTA - iTemp93) & 16383)] + (((fTemp95 * fVec122[((IOTA - iTemp92) & 16383)]) / fTemp96) + (fTemp97 * fRec266[1])));
			fRec264[0] = fRec266[0];
			float fTemp424 = ((fTemp6 * fRec267[1]) + (fTemp17 * fTemp421));
			fVec123[(IOTA & 16383)] = fTemp424;
			fRec271[0] = (((fTemp103 * fVec123[((IOTA - iTemp100) & 16383)]) / fTemp104) + (fVec123[((IOTA - iTemp101) & 16383)] + (fTemp105 * fRec271[1])));
			fRec265[0] = fRec271[0];
			float fTemp425 = (0.0f - ((fTemp6 * fRec265[1]) + (fTemp17 * fTemp416)));
			fVec124[(IOTA & 16383)] = fTemp425;
			fRec263[0] = (fVec124[((IOTA - iTemp110) & 16383)] + (((fTemp36 * fVec124[((IOTA - iTemp107) & 16383)]) / fTemp108) + (fTemp109 * fRec263[1])));
			fRec261[0] = fRec263[0];
			float fTemp426 = ((fTemp6 * fRec264[1]) + (fTemp17 * fTemp420));
			fVec125[(IOTA & 16383)] = fTemp426;
			fRec272[0] = (((fTemp113 * fVec125[((IOTA - iTemp117) & 16383)]) / fTemp114) + (fVec125[((IOTA - iTemp118) & 16383)] + (fTemp115 * fRec272[1])));
			fRec262[0] = fRec272[0];
			float fTemp427 = (0.0f - ((fTemp6 * fRec262[1]) + (fTemp17 * fTemp415)));
			fVec126[(IOTA & 16383)] = fTemp427;
			fRec260[0] = (fVec126[((IOTA - iTemp123) & 16383)] + (((fTemp33 * fVec126[((IOTA - iTemp120) & 16383)]) / fTemp121) + (fTemp122 * fRec260[1])));
			fRec258[0] = fRec260[0];
			float fTemp428 = ((fTemp6 * fRec261[1]) + (fTemp17 * fTemp419));
			fVec127[(IOTA & 16383)] = fTemp428;
			fRec273[0] = (fVec127[((IOTA - iTemp130) & 16383)] + (((fTemp126 * fVec127[((IOTA - iTemp128) & 16383)]) / fTemp129) + (fTemp131 * fRec273[1])));
			fRec259[0] = fRec273[0];
			fRec3[0] = ((0.5f * (fTemp3 * fRec3[1])) + (fTemp4 * ((fTemp6 * fRec4[1]) + (fTemp17 * ((fTemp23 * fRec242[1]) + (fTemp30 * ((fTemp6 * fRec258[1]) + fTemp163)))))));
			fRec1[(IOTA & 1023)] = fRec3[0];
			fRec274[0] = ((0.5f * (fTemp3 * fRec274[1])) + (fTemp4 * ((fTemp6 * fRec5[1]) + (fTemp17 * ((fTemp23 * fRec243[1]) + (fTemp30 * ((fTemp6 * fRec259[1]) + fTemp132)))))));
			fRec2[(IOTA & 1023)] = fRec274[0];
			output0[i] = FAUSTFLOAT((iSlow0?fTemp0:((fTemp1 * fTemp2) + (fRec0[0] * fRec1[((IOTA - 0) & 1023)]))));
			output1[i] = FAUSTFLOAT((iSlow0?fTemp40:((fTemp41 * fTemp2) + (fRec0[0] * fRec2[((IOTA - 0) & 1023)]))));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fRec10[1] = fRec10[0];
			fRec14[1] = fRec14[0];
			fRec18[1] = fRec18[0];
			fRec22[1] = fRec22[0];
			fRec26[1] = fRec26[0];
			fRec30[1] = fRec30[0];
			fRec34[1] = fRec34[0];
			fRec41[1] = fRec41[0];
			fVec3[1] = fVec3[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec46[1] = fRec46[0];
			fRec47[1] = fRec47[0];
			IOTA = (IOTA + 1);
			fRec40[1] = fRec40[0];
			fRec38[1] = fRec38[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec39[1] = fRec39[0];
			fRec50[1] = fRec50[0];
			fRec37[1] = fRec37[0];
			fRec35[1] = fRec35[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec36[1] = fRec36[0];
			fRec33[1] = fRec33[0];
			fRec31[1] = fRec31[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec32[1] = fRec32[0];
			fRec29[1] = fRec29[0];
			fRec27[1] = fRec27[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec28[1] = fRec28[0];
			fRec74[1] = fRec74[0];
			fRec72[1] = fRec72[0];
			fRec75[1] = fRec75[0];
			fRec73[1] = fRec73[0];
			fRec71[1] = fRec71[0];
			fRec69[1] = fRec69[0];
			fRec76[1] = fRec76[0];
			fRec70[1] = fRec70[0];
			fRec68[1] = fRec68[0];
			fRec66[1] = fRec66[0];
			fRec77[1] = fRec77[0];
			fRec67[1] = fRec67[0];
			fRec65[1] = fRec65[0];
			fRec63[1] = fRec63[0];
			fRec78[1] = fRec78[0];
			fRec64[1] = fRec64[0];
			fRec79[1] = fRec79[0];
			fRec62[1] = fRec62[0];
			fRec60[1] = fRec60[0];
			fRec81[1] = fRec81[0];
			fRec80[1] = fRec80[0];
			fRec61[1] = fRec61[0];
			fRec93[1] = fRec93[0];
			fRec91[1] = fRec91[0];
			fRec94[1] = fRec94[0];
			fRec92[1] = fRec92[0];
			fRec90[1] = fRec90[0];
			fRec88[1] = fRec88[0];
			fRec95[1] = fRec95[0];
			fRec89[1] = fRec89[0];
			fRec87[1] = fRec87[0];
			fRec85[1] = fRec85[0];
			fRec96[1] = fRec96[0];
			fRec86[1] = fRec86[0];
			fRec84[1] = fRec84[0];
			fRec82[1] = fRec82[0];
			fRec97[1] = fRec97[0];
			fRec83[1] = fRec83[0];
			fRec98[1] = fRec98[0];
			fRec59[1] = fRec59[0];
			fRec57[1] = fRec57[0];
			fRec100[1] = fRec100[0];
			fRec99[1] = fRec99[0];
			fRec58[1] = fRec58[0];
			fRec25[1] = fRec25[0];
			fRec23[1] = fRec23[0];
			fRec102[1] = fRec102[0];
			fRec101[1] = fRec101[0];
			fRec24[1] = fRec24[0];
			fRec114[1] = fRec114[0];
			fRec112[1] = fRec112[0];
			fRec115[1] = fRec115[0];
			fRec113[1] = fRec113[0];
			fRec111[1] = fRec111[0];
			fRec109[1] = fRec109[0];
			fRec116[1] = fRec116[0];
			fRec110[1] = fRec110[0];
			fRec108[1] = fRec108[0];
			fRec106[1] = fRec106[0];
			fRec117[1] = fRec117[0];
			fRec107[1] = fRec107[0];
			fRec105[1] = fRec105[0];
			fRec103[1] = fRec103[0];
			fRec118[1] = fRec118[0];
			fRec104[1] = fRec104[0];
			fRec21[1] = fRec21[0];
			fRec19[1] = fRec19[0];
			fRec120[1] = fRec120[0];
			fRec119[1] = fRec119[0];
			fRec20[1] = fRec20[0];
			fRec132[1] = fRec132[0];
			fRec130[1] = fRec130[0];
			fRec133[1] = fRec133[0];
			fRec131[1] = fRec131[0];
			fRec129[1] = fRec129[0];
			fRec127[1] = fRec127[0];
			fRec134[1] = fRec134[0];
			fRec128[1] = fRec128[0];
			fRec126[1] = fRec126[0];
			fRec124[1] = fRec124[0];
			fRec135[1] = fRec135[0];
			fRec125[1] = fRec125[0];
			fRec123[1] = fRec123[0];
			fRec121[1] = fRec121[0];
			fRec136[1] = fRec136[0];
			fRec122[1] = fRec122[0];
			fRec17[1] = fRec17[0];
			fRec15[1] = fRec15[0];
			fRec138[1] = fRec138[0];
			fRec137[1] = fRec137[0];
			fRec16[1] = fRec16[0];
			fRec150[1] = fRec150[0];
			fRec148[1] = fRec148[0];
			fRec151[1] = fRec151[0];
			fRec149[1] = fRec149[0];
			fRec147[1] = fRec147[0];
			fRec145[1] = fRec145[0];
			fRec152[1] = fRec152[0];
			fRec146[1] = fRec146[0];
			fRec144[1] = fRec144[0];
			fRec142[1] = fRec142[0];
			fRec153[1] = fRec153[0];
			fRec143[1] = fRec143[0];
			fRec141[1] = fRec141[0];
			fRec139[1] = fRec139[0];
			fRec154[1] = fRec154[0];
			fRec140[1] = fRec140[0];
			fRec166[1] = fRec166[0];
			fRec164[1] = fRec164[0];
			fRec167[1] = fRec167[0];
			fRec165[1] = fRec165[0];
			fRec163[1] = fRec163[0];
			fRec161[1] = fRec161[0];
			fRec168[1] = fRec168[0];
			fRec162[1] = fRec162[0];
			fRec160[1] = fRec160[0];
			fRec158[1] = fRec158[0];
			fRec169[1] = fRec169[0];
			fRec159[1] = fRec159[0];
			fRec157[1] = fRec157[0];
			fRec155[1] = fRec155[0];
			fRec170[1] = fRec170[0];
			fRec156[1] = fRec156[0];
			fRec13[1] = fRec13[0];
			fRec11[1] = fRec11[0];
			fRec172[1] = fRec172[0];
			fRec171[1] = fRec171[0];
			fRec12[1] = fRec12[0];
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
			fRec200[1] = fRec200[0];
			fRec198[1] = fRec198[0];
			fRec201[1] = fRec201[0];
			fRec199[1] = fRec199[0];
			fRec197[1] = fRec197[0];
			fRec195[1] = fRec195[0];
			fRec202[1] = fRec202[0];
			fRec196[1] = fRec196[0];
			fRec194[1] = fRec194[0];
			fRec192[1] = fRec192[0];
			fRec203[1] = fRec203[0];
			fRec193[1] = fRec193[0];
			fRec191[1] = fRec191[0];
			fRec189[1] = fRec189[0];
			fRec204[1] = fRec204[0];
			fRec190[1] = fRec190[0];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec206[1] = fRec206[0];
			fRec205[1] = fRec205[0];
			fRec8[1] = fRec8[0];
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
			fRec234[1] = fRec234[0];
			fRec232[1] = fRec232[0];
			fRec235[1] = fRec235[0];
			fRec233[1] = fRec233[0];
			fRec231[1] = fRec231[0];
			fRec229[1] = fRec229[0];
			fRec236[1] = fRec236[0];
			fRec230[1] = fRec230[0];
			fRec228[1] = fRec228[0];
			fRec226[1] = fRec226[0];
			fRec237[1] = fRec237[0];
			fRec227[1] = fRec227[0];
			fRec225[1] = fRec225[0];
			fRec223[1] = fRec223[0];
			fRec238[1] = fRec238[0];
			fRec224[1] = fRec224[0];
			fRec239[1] = fRec239[0];
			fRec6[1] = fRec6[0];
			fRec4[1] = fRec4[0];
			fRec241[1] = fRec241[0];
			fRec240[1] = fRec240[0];
			fRec5[1] = fRec5[0];
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
			fRec269[1] = fRec269[0];
			fRec267[1] = fRec267[0];
			fRec270[1] = fRec270[0];
			fRec268[1] = fRec268[0];
			fRec266[1] = fRec266[0];
			fRec264[1] = fRec264[0];
			fRec271[1] = fRec271[0];
			fRec265[1] = fRec265[0];
			fRec263[1] = fRec263[0];
			fRec261[1] = fRec261[0];
			fRec272[1] = fRec272[0];
			fRec262[1] = fRec262[0];
			fRec260[1] = fRec260[0];
			fRec258[1] = fRec258[0];
			fRec273[1] = fRec273[0];
			fRec259[1] = fRec259[0];
			fRec3[1] = fRec3[0];
			fRec274[1] = fRec274[0];
			
		}
		
	}

	
};
// clang-format on
#endif

// clang-format off
#define faust_setup(name) xfaust_setup(name)
#define xfaust_setup(name) name##_tilde_setup(void)
// time for "active" toggle xfades in secs
#define XFADE_TIME 0.1f
static t_class* greyhole_faust_class;
#define FAUST_EXT t_faust_greyhole
#define FAUST_EXT_CLASS greyhole_faust_class
// clang-format on

template <class T>
class _greyhole_UI : public UI {
};
typedef _greyhole_UI<greyhole> greyhole_UI;

struct t_faust_greyhole {
    t_object x_obj;
#ifdef __MINGW32__
    /* This seems to be necessary as some as yet undetermined Pd routine seems
     to write past the end of x_obj on Windows. */
    int fence; /* dummy field (not used) */
#endif
    greyhole* dsp;
    PdUI<greyhole_UI>* ui;
    int active, xfade, n_xfade, rate, n_in, n_out;
    t_sample **inputs, **outputs, **buf;
    t_outlet* out;
    t_sample f;
};

static inline void zero_samples(int k, int n, t_sample** out)
{
    for (int i = 0; i < k; i++)
#ifdef __STDC_IEC_559__
        /* IEC 559 a.k.a. IEEE 754 floats can be initialized faster like this */
        memset(out[i], 0, n * sizeof(t_sample));
#else
        for (int j = 0; j < n; j++)
            out[i][j] = 0.0f;
#endif
}

static inline void copy_samples(int k, int n, t_sample** out, t_sample** in)
{
    for (int i = 0; i < k; i++)
        memcpy(out[i], in[i], n * sizeof(t_sample));
}

static t_int* faust_perform(t_int* w)
{
    t_faust_greyhole* x = reinterpret_cast<t_faust_greyhole*>(w[1]);
    int n = static_cast<int>(w[2]);
    if (!x->dsp || !x->buf)
        return (w + 3);

    AVOIDDENORMALS;
    if (x->xfade > 0) {
        float d = 1.0f / x->n_xfade, f = (x->xfade--) * d;
        d = d / n;
        x->dsp->compute(n, x->inputs, x->buf);
        if (x->active) {
            if (x->n_in == x->n_out) {
                /* xfade inputs -> buf */
                for (int j = 0; j < n; j++, f -= d) {
                    for (int i = 0; i < x->n_out; i++)
                        x->outputs[i][j] = f * x->inputs[i][j] + (1.0f - f) * x->buf[i][j];
                }
            } else {
                /* xfade 0 -> buf */
                for (int j = 0; j < n; j++, f -= d) {
                    for (int i = 0; i < x->n_out; i++)
                        x->outputs[i][j] = (1.0f - f) * x->buf[i][j];
                }
            }
        } else if (x->n_in == x->n_out) {
            /* xfade buf -> inputs */
            for (int j = 0; j < n; j++, f -= d) {
                for (int i = 0; i < x->n_out; i++)
                    x->outputs[i][j] = f * x->buf[i][j] + (1.0f - f) * x->inputs[i][j];
            }
        } else {
            /* xfade buf -> 0 */
            for (int j = 0; j < n; j++, f -= d) {
                for (int i = 0; i < x->n_out; i++)
                    x->outputs[i][j] = f * x->buf[i][j];
            }
        }
    } else if (x->active) {
        x->dsp->compute(n, x->inputs, x->buf);
        copy_samples(x->n_out, n, x->outputs, x->buf);
    } else if (x->n_in == x->n_out) {
        copy_samples(x->n_out, n, x->buf, x->inputs);
        copy_samples(x->n_out, n, x->outputs, x->buf);
    } else
        zero_samples(x->n_out, n, x->outputs);

    return (w + 3);
}

static void greyhole_faust_dsp(t_faust_greyhole* x, t_signal** sp)
{
    const int n = sp[0]->s_n;
    const int sr = static_cast<int>(sp[0]->s_sr);

    if (x->rate <= 0) {
        /* default sample rate is whatever Pd tells us */
        PdUI<greyhole_UI>* ui = x->ui;
        std::vector<FAUSTFLOAT> z = ui->uiValues();
        /* set the proper sample rate; this requires reinitializing the dsp */
        x->rate = sr;
        x->dsp->init(sr);
        ui->setUIValues(z);
    }
    if (n > 0)
        x->n_xfade = static_cast<int>(x->rate * XFADE_TIME / n);

    dsp_add(faust_perform, 2, x, n);

    for (int i = 0; i < x->n_in; i++)
        x->inputs[i] = sp[i]->s_vec;

    for (int i = 0; i < x->n_out; i++)
        x->outputs[i] = sp[x->n_in + i]->s_vec;

    if (x->buf != NULL) {
        for (int i = 0; i < x->n_out; i++) {
            x->buf[i] = static_cast<t_sample*>(malloc(n * sizeof(t_sample)));
            if (x->buf[i] == NULL) {
                for (int j = 0; j < i; j++)
                    free(x->buf[j]);
                free(x->buf);
                x->buf = NULL;
                break;
            }
        }
    }
}

static void greyhole_dump_to_console(t_faust_greyhole* x)
{
    t_object* xobj = &x->x_obj;
    t_class* xc = xobj->te_pd;
    const char* name = class_getname(xc);

    // print xlets
    post("[%s] inlets: %i", name, x->dsp->getNumInputs());
    int info_outlet = (x->out == 0) ? 0 : 1;
    post("[%s] outlets: %i", name, x->dsp->getNumOutputs() + info_outlet);

    // print properties
    for (size_t i = 0; i < x->ui->uiCount(); i++) {
        UIElement* el = x->ui->uiAt(i);
        post("[%s] property: %s = %g", name, el->setPropertySym()->s_name, static_cast<double>(el->value()));
    }
}

static void greyhole_faust_any(t_faust_greyhole* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!x->dsp)
        return;

    PdUI<greyhole_UI>* ui = x->ui;
    if (s == &s_bang) {
        ui->dumpUI(x->out);
    } else if (isGetAllProperties(s)) {
        ui->outputAllProperties(x->out);
    } else if (isGetProperty(s)) {
        ui->outputProperty(s, x->out);
    } else if (isSetProperty(s)) {
        ui->setProperty(s, argc, argv);
    } else {
        const char* label = s->s_name;
        int count = 0;
        for (size_t i = 0; i < ui->uiCount(); i++) {
            if (ui->uiAt(i)->pathcmp(label)) {
                if (argc == 0) {
                    ui->uiAt(i)->outputValue(x->out);
                    ++count;
                } else if (argc == 1 && (argv[0].a_type == A_FLOAT || argv[0].a_type == A_DEFFLOAT)) {
                    float f = atom_getfloat(argv);
                    UIElement* el = ui->uiAt(i);
                    el->setValue(f);
                    ++count;
                } else
                    pd_error(x, "[ceammc] %s: bad control argument: %s", ui->fullName().c_str(), label);
            }
        }

        if (count == 0 && strcmp(label, "active") == 0) {
            if (argc == 0) {
                t_atom arg;
                SETFLOAT(&arg, (float)x->active);
                if (x->out) {
                    outlet_anything(x->out, gensym("active"), 1, &arg);
                }
            } else if (argc == 1 && (argv[0].a_type == A_FLOAT || argv[0].a_type == A_DEFFLOAT)) {
                float f = atom_getfloat(argv);
                x->active = (int)f;
                x->xfade = x->n_xfade;
            }
        }
    }
}

static void faust_free_dsp(t_faust_greyhole* x)
{
    delete x->dsp;
    x->dsp = NULL;
}

static void faust_free_ui(t_faust_greyhole* x)
{
    delete x->ui;
    x->ui = NULL;
}

static void faust_free_inputs(t_faust_greyhole* x)
{
    if (x->inputs)
        free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust_greyhole* x)
{
    if (x->outputs)
        free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust_greyhole* x)
{
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i])
                free(x->buf[i]);
        }

        free(x->buf);
    }
}

static void greyhole_faust_free(t_faust_greyhole* x)
{
    faust_free_dsp(x);
    faust_free_ui(x);
    faust_free_inputs(x);
    faust_free_outputs(x);
    faust_free_buf(x);
}

static bool faust_init_inputs(t_faust_greyhole* x)
{
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if (x->inputs == NULL) {
            error("[%s] faust_init_inputs failed", sym(greyhole));
            return false;
        }
    }

    // creating sound inlets (except the first one!)
    for (int i = 0; i < (x->n_in - 1); i++) {
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    return true;
}

static bool faust_init_outputs(t_faust_greyhole* x, bool info_outlet)
{
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->outputs == NULL) {
            error("[%s] faust_init_outputs failed", sym(greyhole));
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->buf == NULL) {
            error("[%s] faust_init_outputs failed", sym(greyhole));
            faust_free_outputs(x);
            return false;
        }

        for (int i = 0; i < x->n_out; i++)
            x->buf[i] = NULL;
    }

    // creating sound outlets
    for (int i = 0; i < x->n_out; i++) {
        outlet_new(&x->x_obj, &s_signal);
    }

    // control outlet
    if (info_outlet)
        x->out = outlet_new(&x->x_obj, 0);
    else
        x->out = 0;

    return true;
}

static bool faust_new_internal(t_faust_greyhole* x, const std::string& objId = "", bool info_outlet = true)
{
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new greyhole();
    x->ui = new PdUI<greyhole_UI>(sym(greyhole), objId);

    if (!faust_init_inputs(x)) {
        greyhole_faust_free(x);
        return false;
    }

    if (!faust_init_outputs(x, info_outlet)) {
        greyhole_faust_free(x);
        return false;
    }

    x->dsp->init(sr);
    x->dsp->buildUserInterface(x->ui);

    return true;
}

/**
 * find nth element that satisfies given predicate
 * @param first - first element of sequence
 * @param last - pointer behind last element of sequence
 * @param Nth - searched element index
 * @param pred - predicate
 * @return pointer to found element or pointer to last, if not found
 */
template <class InputIterator, class NthOccurence, class UnaryPredicate>
InputIterator find_nth_if(InputIterator first, InputIterator last, NthOccurence Nth, UnaryPredicate pred)
{
    if (Nth > 0) {
        while (first != last) {
            if (pred(*first))
                if (!--Nth)
                    return first;
            ++first;
        }
    }
    return last;
}

/**
 * @return true if given atom is a float
 */
static bool atom_is_float(const t_atom& a)
{
    switch (a.a_type) {
    case A_FLOAT:
    case A_DEFFLOAT:
        return true;
    default:
        return false;
    }
}

/**
 * @return true if given atom is a symbol
 */
static bool atom_is_symbol(const t_atom& a)
{
    switch (a.a_type) {
    case A_DEFSYMBOL:
    case A_SYMBOL:
        return true;
    default:
        return false;
    }
}

/**
 * @return true if given atom is a property
 */
static bool atom_is_property(const t_atom& a)
{
    switch (a.a_type) {
    case A_DEFSYMBOL:
    case A_SYMBOL:
        return a.a_w.w_symbol->s_name[0] == '@';
    default:
        return false;
    }
}

/**
 * @brief find nth float in argument list. (arguments can be mixed)
 * @param argc argument count
 * @param argv pointer to argument vector
 * @param nth find position. nth should be > 0!
 * @param dest destination to write value
 * @return true if argument at given position was found, otherwise false
 */
static bool get_nth_float_arg(int argc, t_atom* argv, int nth, t_float* dest)
{
    t_atom* last = argv + argc;
    t_atom* res = find_nth_if(argv, last, nth, atom_is_float);
    if (last == res)
        return false;

    *dest = atom_getfloat(res);
    return true;
}

/**
 * @brief find nth symbol in argument list. (arguments can be mixed)
 * @param argc argument count
 * @param argv pointer to argument vector
 * @param nth find position. nth should be > 0!
 * @param dest destination to write found argument value
 * @return true if argument at given position was found, otherwise false
 */
static bool get_nth_symbol_arg(int argc, t_atom* argv, int nth, const char** dest)
{
    t_atom* last = argv + argc;
    t_atom* res = find_nth_if(argv, last, nth, atom_is_symbol);
    if (last == res)
        return false;

    t_symbol* s = atom_getsymbol(res);
    *dest = s->s_name;
    return true;
}

class PdArgParser {
    t_faust_greyhole* x_;
    int argc_;
    t_atom* argv_;
    bool control_outlet_;

public:
    /**
     * @brief FaustArgParser
     * @param x pointer to faust class
     * @param argc arguments count
     * @param argv pointer to argument vector
     */
    PdArgParser(t_faust_greyhole* x, int argc, t_atom* argv, bool info_outlet = true)
        : x_(x)
        , argc_(0)
        , argv_(argv)
        , control_outlet_(info_outlet)
    {
        const char* id = NULL;
        std::string objId;

        int first_prop_idx = argc;
        for (int i = 0; i < argc; i++) {
            if (atom_is_property(argv[i]))
                first_prop_idx = i;
        }

        // store argument count (without properties)
        argc_ = first_prop_idx;

        if (get_nth_symbol_arg(argc_, argv_, 1, &id))
            objId = id;

        // init error
        if (!faust_new_internal(x, objId, control_outlet_)) {
            this->x_ = NULL;
        }

        // process properties
        std::deque<ceammc::AtomList> props = ceammc::AtomList(argc, argv).properties();
        for (size_t i = 0; i < props.size(); i++) {
            ceammc::AtomList& p = props[i];
            // skip empty property
            if (p.size() < 2)
                continue;

            t_atom* data = p.toPdData() + 1;
            this->x_->ui->setProperty(p[0].asSymbol(), p.size() - 1, data);
        }
    }

    /**
     * @brief initFloatArg
     * @param name argument name
     * @param pos argument position among of @bold float(!) arguments. Position starts from @bold 1(!).
     * to select first argument - pass 1.
     */
    void initFloatArg(const char* name, int pos)
    {
        // object was not created
        if (!this->x_)
            return;

        t_float v = 0.0;
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &v)) {
            UIElement* el = this->x_->ui->findElementByLabel(name);
            if (!el) {
                post("invalid UI element name: %s", name);
                return;
            }

            el->setValue(v, true);
        }
    }

    /**
     * @brief send creation argument to first signal inlet
     * @param name argument name
     * @param pos argument position among of @bold float(!) arguments. Position starts from @bold 1(!).
     * to select first argument - pass 1.
     */
    void signalFloatArg(const char* /*name*/, int pos)
    {
        // object was not created
        if (!this->x_)
            return;

        t_float arg = 0;
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &arg))
            pd_float(reinterpret_cast<t_pd*>(this->x_), arg);
    }

    t_faust_greyhole* pd_obj()
    {
        return this->x_;
    }
};

static void* greyhole_faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s, bool soundIn = true)
{
    greyhole_faust_class = class_new(s, reinterpret_cast<t_newmethod>(greyhole_faust_new),
        reinterpret_cast<t_method>(greyhole_faust_free),
        sizeof(t_faust_greyhole),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);

    if (soundIn) {
        class_addmethod(greyhole_faust_class, nullfn, &s_signal, A_NULL);
        CLASS_MAINSIGNALIN(greyhole_faust_class, t_faust_greyhole, f);
    }

    class_addmethod(greyhole_faust_class, reinterpret_cast<t_method>(greyhole_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(greyhole_faust_class, reinterpret_cast<t_method>(greyhole_dump_to_console), gensym("dump"), A_NULL);
    class_addanything(greyhole_faust_class, greyhole_faust_any);
    ceammc::register_faust_external(greyhole_faust_class);
}

#define EXTERNAL_NEW void* greyhole_faust_new(t_symbol*, int argc, t_atom* argv)

#define EXTERNAL_SIMPLE_NEW()                                                           \
    static void* greyhole_faust_new(t_symbol*, int argc, t_atom* argv)                     \
    {                                                                                   \
        t_faust_greyhole* x = reinterpret_cast<t_faust_greyhole*>(pd_new(greyhole_faust_class)); \
        PdArgParser p(x, argc, argv, false);                                            \
        return p.pd_obj();                                                              \
    }

#define EXTERNAL_SETUP(MOD)                        \
    extern "C" void setup_##MOD##0x2egreyhole_tilde() \
    {                                              \
        internal_setup(gensym(#MOD ".greyhole~"));    \
    }

#define EXTERNAL_SETUP_NO_IN(MOD)                      \
    extern "C" void setup_##MOD##0x2egreyhole_tilde()     \
    {                                                  \
        internal_setup(gensym(#MOD ".greyhole~"), false); \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);

#endif
