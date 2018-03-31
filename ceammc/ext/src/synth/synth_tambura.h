/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "Tambura"
version: "1.0"
Code generated with Faust 2.5.30 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __tambura_H__
#define  __tambura_H__

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
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
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
#include <m_pd.h>

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
struct tambura : public dsp {
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

#include <cmath>
#include <math.h>

float tambura_faustpower2_f(float value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS tambura
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class tambura : public dsp {
	
 private:
	
	int fSamplingFreq;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	FAUSTFLOAT fHslider0;
	float fRec13[2];
	float fVec0[2];
	float fRec12[2];
	float fRec14[2];
	float fConst21;
	float fRec11[3];
	float fConst22;
	float fConst23;
	float fConst24;
	float fConst25;
	float fRec10[3];
	float fConst26;
	float fConst27;
	float fConst28;
	float fVec1[2];
	float fRec17[2];
	float fRec18[2];
	float fRec16[3];
	float fRec15[3];
	float fVec2[2];
	float fRec21[2];
	float fRec22[2];
	float fRec20[3];
	float fRec19[3];
	int iRec24[2];
	float fRec23[4];
	FAUSTFLOAT fHslider1;
	float fRec25[2];
	FAUSTFLOAT fCheckbox0;
	float fConst29;
	FAUSTFLOAT fHslider2;
	float fRec30[2];
	FAUSTFLOAT fButton0;
	float fVec3[2];
	float fConst30;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fRec31[2];
	float fRec28[2];
	float fRec29[2];
	float fVec4[2];
	float fConst31;
	FAUSTFLOAT fHslider5;
	float fConst32;
	float fRec27[2];
	float fRec26[2];
	int IOTA;
	float fVec5[8192];
	FAUSTFLOAT fHslider6;
	float fRec32[2];
	FAUSTFLOAT fHslider7;
	float fRec33[2];
	float fConst33;
	FAUSTFLOAT fHslider8;
	float fRec34[2];
	float fRec35[2];
	FAUSTFLOAT fHslider9;
	float fConst34;
	FAUSTFLOAT fHslider10;
	float fRec36[2];
	FAUSTFLOAT fHslider11;
	float fVec6[2];
	float fConst35;
	FAUSTFLOAT fHslider12;
	float fRec37[2];
	float fRec38[2];
	float fConst36;
	FAUSTFLOAT fHslider13;
	float fRec39[2];
	float fVec7[8192];
	float fConst37;
	float fVec8[2];
	float fRec9[4];
	float fRec41[2];
	float fVec9[8192];
	float fVec10[2];
	float fRec40[4];
	float fRec43[2];
	float fVec11[8192];
	float fVec12[2];
	float fRec42[4];
	float fRec45[2];
	float fVec13[8192];
	float fVec14[2];
	float fRec44[4];
	float fRec47[2];
	float fVec15[8192];
	float fVec16[2];
	float fRec46[4];
	float fRec49[2];
	float fVec17[8192];
	float fVec18[2];
	float fRec48[4];
	float fRec51[2];
	float fVec19[8192];
	float fVec20[2];
	float fRec50[4];
	float fRec53[2];
	float fVec21[8192];
	float fVec22[2];
	float fRec52[4];
	float fRec55[2];
	float fVec23[8192];
	float fVec24[2];
	float fRec54[4];
	float fVec25[2];
	float fRec8[2];
	float fRec58[2];
	FAUSTFLOAT fHslider14;
	float fVec26[8192];
	float fVec27[2];
	float fRec57[4];
	float fRec60[2];
	float fVec28[8192];
	float fVec29[2];
	float fRec59[4];
	float fRec62[2];
	float fVec30[8192];
	float fVec31[2];
	float fRec61[4];
	float fRec64[2];
	float fVec32[8192];
	float fVec33[2];
	float fRec63[4];
	float fRec66[2];
	float fVec34[8192];
	float fVec35[2];
	float fRec65[4];
	float fRec68[2];
	float fVec36[8192];
	float fVec37[2];
	float fRec67[4];
	float fRec70[2];
	float fVec38[8192];
	float fVec39[2];
	float fRec69[4];
	float fRec72[2];
	float fVec40[8192];
	float fVec41[2];
	float fRec71[4];
	float fRec74[2];
	float fVec42[8192];
	float fVec43[2];
	float fRec73[4];
	float fVec44[2];
	float fRec56[2];
	float fRec1[2];
	float fVec45[2];
	float fRec79[2];
	float fRec80[2];
	float fRec78[3];
	float fRec77[3];
	FAUSTFLOAT fButton1;
	float fVec46[2];
	float fConst38;
	float fRec83[2];
	float fRec84[2];
	float fVec47[2];
	float fConst39;
	float fRec82[2];
	float fRec81[2];
	float fVec48[8192];
	float fRec85[2];
	float fRec86[2];
	float fVec49[2];
	float fRec87[2];
	float fRec88[2];
	float fVec50[8192];
	float fVec51[2];
	float fRec76[4];
	float fRec90[2];
	float fVec52[8192];
	float fVec53[2];
	float fRec89[4];
	float fRec92[2];
	float fVec54[8192];
	float fVec55[2];
	float fRec91[4];
	float fRec94[2];
	float fVec56[8192];
	float fVec57[2];
	float fRec93[4];
	float fRec96[2];
	float fVec58[8192];
	float fVec59[2];
	float fRec95[4];
	float fRec98[2];
	float fVec60[8192];
	float fVec61[2];
	float fRec97[4];
	float fRec100[2];
	float fVec62[8192];
	float fVec63[2];
	float fRec99[4];
	float fRec102[2];
	float fVec64[8192];
	float fVec65[2];
	float fRec101[4];
	float fRec104[2];
	float fVec66[8192];
	float fVec67[2];
	float fRec103[4];
	float fVec68[2];
	float fRec75[2];
	float fRec107[2];
	float fVec69[8192];
	float fVec70[2];
	float fRec106[4];
	float fRec109[2];
	float fVec71[8192];
	float fVec72[2];
	float fRec108[4];
	float fRec111[2];
	float fVec73[8192];
	float fVec74[2];
	float fRec110[4];
	float fRec113[2];
	float fVec75[8192];
	float fVec76[2];
	float fRec112[4];
	float fRec115[2];
	float fVec77[8192];
	float fVec78[2];
	float fRec114[4];
	float fRec117[2];
	float fVec79[8192];
	float fVec80[2];
	float fRec116[4];
	float fRec119[2];
	float fVec81[8192];
	float fVec82[2];
	float fRec118[4];
	float fRec121[2];
	float fVec83[8192];
	float fVec84[2];
	float fRec120[4];
	float fRec123[2];
	float fVec85[8192];
	float fVec86[2];
	float fRec122[4];
	float fVec87[2];
	float fRec105[2];
	float fRec3[2];
	FAUSTFLOAT fButton2;
	float fVec88[2];
	float fConst40;
	float fRec128[2];
	float fRec129[2];
	float fVec89[2];
	float fConst41;
	float fRec127[2];
	float fRec126[2];
	float fVec90[8192];
	float fRec130[2];
	float fRec131[2];
	float fVec91[2];
	float fRec132[2];
	float fRec133[2];
	float fVec92[8192];
	float fVec93[2];
	float fRec125[4];
	float fRec135[2];
	float fVec94[8192];
	float fVec95[2];
	float fRec134[4];
	float fRec137[2];
	float fVec96[8192];
	float fVec97[2];
	float fRec136[4];
	float fRec139[2];
	float fVec98[8192];
	float fVec99[2];
	float fRec138[4];
	float fRec141[2];
	float fVec100[8192];
	float fVec101[2];
	float fRec140[4];
	float fRec143[2];
	float fVec102[8192];
	float fVec103[2];
	float fRec142[4];
	float fRec145[2];
	float fVec104[8192];
	float fVec105[2];
	float fRec144[4];
	float fRec147[2];
	float fVec106[8192];
	float fVec107[2];
	float fRec146[4];
	float fRec149[2];
	float fVec108[8192];
	float fVec109[2];
	float fRec148[4];
	float fVec110[2];
	float fRec124[2];
	float fRec152[2];
	float fVec111[8192];
	float fVec112[2];
	float fRec151[4];
	float fRec154[2];
	float fVec113[8192];
	float fVec114[2];
	float fRec153[4];
	float fRec156[2];
	float fVec115[8192];
	float fVec116[2];
	float fRec155[4];
	float fRec158[2];
	float fVec117[8192];
	float fVec118[2];
	float fRec157[4];
	float fRec160[2];
	float fVec119[8192];
	float fVec120[2];
	float fRec159[4];
	float fRec162[2];
	float fVec121[8192];
	float fVec122[2];
	float fRec161[4];
	float fRec164[2];
	float fVec123[8192];
	float fVec124[2];
	float fRec163[4];
	float fRec166[2];
	float fVec125[8192];
	float fVec126[2];
	float fRec165[4];
	float fRec168[2];
	float fVec127[8192];
	float fVec128[2];
	float fRec167[4];
	float fVec129[2];
	float fRec150[2];
	float fRec5[2];
	FAUSTFLOAT fButton3;
	float fVec130[2];
	float fRec173[2];
	float fRec174[2];
	float fVec131[2];
	float fConst42;
	float fRec172[2];
	float fRec171[2];
	float fVec132[8192];
	float fRec175[2];
	float fRec176[2];
	float fVec133[2];
	float fRec177[2];
	float fRec178[2];
	float fVec134[8192];
	float fVec135[2];
	float fRec170[4];
	float fRec180[2];
	float fVec136[8192];
	float fVec137[2];
	float fRec179[4];
	float fRec182[2];
	float fVec138[8192];
	float fVec139[2];
	float fRec181[4];
	float fRec184[2];
	float fVec140[8192];
	float fVec141[2];
	float fRec183[4];
	float fRec186[2];
	float fVec142[8192];
	float fVec143[2];
	float fRec185[4];
	float fRec188[2];
	float fVec144[8192];
	float fVec145[2];
	float fRec187[4];
	float fRec190[2];
	float fVec146[8192];
	float fVec147[2];
	float fRec189[4];
	float fRec192[2];
	float fVec148[8192];
	float fVec149[2];
	float fRec191[4];
	float fRec194[2];
	float fVec150[8192];
	float fVec151[2];
	float fRec193[4];
	float fVec152[2];
	float fRec169[2];
	float fRec197[2];
	float fVec153[8192];
	float fVec154[2];
	float fRec196[4];
	float fRec199[2];
	float fVec155[8192];
	float fVec156[2];
	float fRec198[4];
	float fRec201[2];
	float fVec157[8192];
	float fVec158[2];
	float fRec200[4];
	float fRec203[2];
	float fVec159[8192];
	float fVec160[2];
	float fRec202[4];
	float fRec205[2];
	float fVec161[8192];
	float fVec162[2];
	float fRec204[4];
	float fRec207[2];
	float fVec163[8192];
	float fVec164[2];
	float fRec206[4];
	float fRec209[2];
	float fVec165[8192];
	float fVec166[2];
	float fRec208[4];
	float fRec211[2];
	float fVec167[8192];
	float fVec168[2];
	float fRec210[4];
	float fRec213[2];
	float fVec169[8192];
	float fVec170[2];
	float fRec212[4];
	float fVec171[2];
	float fRec195[2];
	float fRec7[2];
	FAUSTFLOAT fHslider15;
	float fRec214[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.0");
		m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1");
		m->declare("copyright", "Oliver Larkin");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("description", "Pseudo physical model of an Indian Tambura/Tanpura");
		m->declare("filename", "synth_tambura");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("licence", "GPL");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "Tambura");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 0;
		
	}
	virtual int getNumOutputs() {
		return 1;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
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
		fConst0 = min(192000.0f, max(1.0f, float(fSamplingFreq)));
		fConst1 = (1.0f / fConst0);
		fConst2 = (fConst1 / sinf((47123.8906f * fConst1)));
		fConst3 = (2042.03528f * fConst2);
		fConst4 = tanf((23561.9453f / fConst0));
		fConst5 = (1.0f / fConst4);
		fConst6 = (1.0f / (((fConst3 + fConst5) / fConst4) + 1.0f));
		fConst7 = (2.0f * (1.0f - (1.0f / tambura_faustpower2_f(fConst4))));
		fConst8 = tanf((7853.98145f / fConst0));
		fConst9 = (1.0f / fConst8);
		fConst10 = (fConst1 / sinf((15707.9629f * fConst1)));
		fConst11 = (1256.63708f * fConst10);
		fConst12 = (1.0f / (((fConst9 + fConst11) / fConst8) + 1.0f));
		fConst13 = (2.0f * (1.0f - (1.0f / tambura_faustpower2_f(fConst8))));
		fConst14 = (1.0f / tanf((15707.9629f / fConst0)));
		fConst15 = (fConst14 + 1.0f);
		fConst16 = (0.0f - ((1.0f - fConst14) / fConst15));
		fConst17 = (1.0f / fConst15);
		fConst18 = (0.0f - fConst14);
		fConst19 = expf((0.0f - (20.0f / fConst0)));
		fConst20 = (1.0f - fConst19);
		fConst21 = (((fConst9 - fConst11) / fConst8) + 1.0f);
		fConst22 = (6298.10449f * fConst10);
		fConst23 = (((fConst9 + fConst22) / fConst8) + 1.0f);
		fConst24 = (((fConst9 - fConst22) / fConst8) + 1.0f);
		fConst25 = (((fConst5 - fConst3) / fConst4) + 1.0f);
		fConst26 = (20420.3516f * fConst2);
		fConst27 = (((fConst26 + fConst5) / fConst4) + 1.0f);
		fConst28 = (((fConst5 - fConst26) / fConst4) + 1.0f);
		fConst29 = (1.0f / fConst0);
		fConst30 = (0.00151515147f * fConst0);
		fConst31 = (660.0f / fConst0);
		fConst32 = expf((0.0f - (200.0f / fConst0)));
		fConst33 = (0.100000001f * fConst20);
		fConst34 = (0.00100000005f * fConst20);
		fConst35 = (0.00100000005f * fConst0);
		fConst36 = (0.200000003f * fConst20);
		fConst37 = (0.0022727272f * fConst0);
		fConst38 = (0.0011363636f * fConst0);
		fConst39 = (880.0f / fConst0);
		fConst40 = (0.00113071012f * fConst0);
		fConst41 = (884.400024f / fConst0);
		fConst42 = (440.0f / fConst0);
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.10000000000000001f);
		fHslider1 = FAUSTFLOAT(0.13f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.10000000000000001f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(1.0f);
		fHslider4 = FAUSTFLOAT(36.0f);
		fHslider5 = FAUSTFLOAT(0.070000000000000007f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(0.25f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(1.0f);
		fHslider10 = FAUSTFLOAT(10000.0f);
		fHslider11 = FAUSTFLOAT(3.0f);
		fHslider12 = FAUSTFLOAT(10.0f);
		fHslider13 = FAUSTFLOAT(0.001f);
		fHslider14 = FAUSTFLOAT(1.0f);
		fButton1 = FAUSTFLOAT(0.0f);
		fButton2 = FAUSTFLOAT(0.0f);
		fButton3 = FAUSTFLOAT(0.0f);
		fHslider15 = FAUSTFLOAT(1.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec13[l0] = 0.0f;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fVec0[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec12[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec14[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 3); l4 = (l4 + 1)) {
			fRec11[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 3); l5 = (l5 + 1)) {
			fRec10[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fVec1[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec17[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec18[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 3); l9 = (l9 + 1)) {
			fRec16[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec15[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fVec2[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec21[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec22[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec20[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec19[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			iRec24[l16] = 0;
			
		}
		for (int l17 = 0; (l17 < 4); l17 = (l17 + 1)) {
			fRec23[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec25[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec30[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fVec3[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec31[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec28[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec29[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fVec4[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec27[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec26[l26] = 0.0f;
			
		}
		IOTA = 0;
		for (int l27 = 0; (l27 < 8192); l27 = (l27 + 1)) {
			fVec5[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec32[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec33[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec34[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec35[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec36[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fVec6[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec37[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec38[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec39[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 8192); l37 = (l37 + 1)) {
			fVec7[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fVec8[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 4); l39 = (l39 + 1)) {
			fRec9[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec41[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 8192); l41 = (l41 + 1)) {
			fVec9[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fVec10[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 4); l43 = (l43 + 1)) {
			fRec40[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec43[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 8192); l45 = (l45 + 1)) {
			fVec11[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fVec12[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 4); l47 = (l47 + 1)) {
			fRec42[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec45[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 8192); l49 = (l49 + 1)) {
			fVec13[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fVec14[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 4); l51 = (l51 + 1)) {
			fRec44[l51] = 0.0f;
			
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec47[l52] = 0.0f;
			
		}
		for (int l53 = 0; (l53 < 8192); l53 = (l53 + 1)) {
			fVec15[l53] = 0.0f;
			
		}
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fVec16[l54] = 0.0f;
			
		}
		for (int l55 = 0; (l55 < 4); l55 = (l55 + 1)) {
			fRec46[l55] = 0.0f;
			
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec49[l56] = 0.0f;
			
		}
		for (int l57 = 0; (l57 < 8192); l57 = (l57 + 1)) {
			fVec17[l57] = 0.0f;
			
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fVec18[l58] = 0.0f;
			
		}
		for (int l59 = 0; (l59 < 4); l59 = (l59 + 1)) {
			fRec48[l59] = 0.0f;
			
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec51[l60] = 0.0f;
			
		}
		for (int l61 = 0; (l61 < 8192); l61 = (l61 + 1)) {
			fVec19[l61] = 0.0f;
			
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fVec20[l62] = 0.0f;
			
		}
		for (int l63 = 0; (l63 < 4); l63 = (l63 + 1)) {
			fRec50[l63] = 0.0f;
			
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec53[l64] = 0.0f;
			
		}
		for (int l65 = 0; (l65 < 8192); l65 = (l65 + 1)) {
			fVec21[l65] = 0.0f;
			
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fVec22[l66] = 0.0f;
			
		}
		for (int l67 = 0; (l67 < 4); l67 = (l67 + 1)) {
			fRec52[l67] = 0.0f;
			
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec55[l68] = 0.0f;
			
		}
		for (int l69 = 0; (l69 < 8192); l69 = (l69 + 1)) {
			fVec23[l69] = 0.0f;
			
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fVec24[l70] = 0.0f;
			
		}
		for (int l71 = 0; (l71 < 4); l71 = (l71 + 1)) {
			fRec54[l71] = 0.0f;
			
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fVec25[l72] = 0.0f;
			
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec8[l73] = 0.0f;
			
		}
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec58[l74] = 0.0f;
			
		}
		for (int l75 = 0; (l75 < 8192); l75 = (l75 + 1)) {
			fVec26[l75] = 0.0f;
			
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fVec27[l76] = 0.0f;
			
		}
		for (int l77 = 0; (l77 < 4); l77 = (l77 + 1)) {
			fRec57[l77] = 0.0f;
			
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec60[l78] = 0.0f;
			
		}
		for (int l79 = 0; (l79 < 8192); l79 = (l79 + 1)) {
			fVec28[l79] = 0.0f;
			
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fVec29[l80] = 0.0f;
			
		}
		for (int l81 = 0; (l81 < 4); l81 = (l81 + 1)) {
			fRec59[l81] = 0.0f;
			
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec62[l82] = 0.0f;
			
		}
		for (int l83 = 0; (l83 < 8192); l83 = (l83 + 1)) {
			fVec30[l83] = 0.0f;
			
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fVec31[l84] = 0.0f;
			
		}
		for (int l85 = 0; (l85 < 4); l85 = (l85 + 1)) {
			fRec61[l85] = 0.0f;
			
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec64[l86] = 0.0f;
			
		}
		for (int l87 = 0; (l87 < 8192); l87 = (l87 + 1)) {
			fVec32[l87] = 0.0f;
			
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fVec33[l88] = 0.0f;
			
		}
		for (int l89 = 0; (l89 < 4); l89 = (l89 + 1)) {
			fRec63[l89] = 0.0f;
			
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec66[l90] = 0.0f;
			
		}
		for (int l91 = 0; (l91 < 8192); l91 = (l91 + 1)) {
			fVec34[l91] = 0.0f;
			
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fVec35[l92] = 0.0f;
			
		}
		for (int l93 = 0; (l93 < 4); l93 = (l93 + 1)) {
			fRec65[l93] = 0.0f;
			
		}
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec68[l94] = 0.0f;
			
		}
		for (int l95 = 0; (l95 < 8192); l95 = (l95 + 1)) {
			fVec36[l95] = 0.0f;
			
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fVec37[l96] = 0.0f;
			
		}
		for (int l97 = 0; (l97 < 4); l97 = (l97 + 1)) {
			fRec67[l97] = 0.0f;
			
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec70[l98] = 0.0f;
			
		}
		for (int l99 = 0; (l99 < 8192); l99 = (l99 + 1)) {
			fVec38[l99] = 0.0f;
			
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			fVec39[l100] = 0.0f;
			
		}
		for (int l101 = 0; (l101 < 4); l101 = (l101 + 1)) {
			fRec69[l101] = 0.0f;
			
		}
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			fRec72[l102] = 0.0f;
			
		}
		for (int l103 = 0; (l103 < 8192); l103 = (l103 + 1)) {
			fVec40[l103] = 0.0f;
			
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fVec41[l104] = 0.0f;
			
		}
		for (int l105 = 0; (l105 < 4); l105 = (l105 + 1)) {
			fRec71[l105] = 0.0f;
			
		}
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec74[l106] = 0.0f;
			
		}
		for (int l107 = 0; (l107 < 8192); l107 = (l107 + 1)) {
			fVec42[l107] = 0.0f;
			
		}
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			fVec43[l108] = 0.0f;
			
		}
		for (int l109 = 0; (l109 < 4); l109 = (l109 + 1)) {
			fRec73[l109] = 0.0f;
			
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			fVec44[l110] = 0.0f;
			
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec56[l111] = 0.0f;
			
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fRec1[l112] = 0.0f;
			
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fVec45[l113] = 0.0f;
			
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec79[l114] = 0.0f;
			
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec80[l115] = 0.0f;
			
		}
		for (int l116 = 0; (l116 < 3); l116 = (l116 + 1)) {
			fRec78[l116] = 0.0f;
			
		}
		for (int l117 = 0; (l117 < 3); l117 = (l117 + 1)) {
			fRec77[l117] = 0.0f;
			
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			fVec46[l118] = 0.0f;
			
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec83[l119] = 0.0f;
			
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec84[l120] = 0.0f;
			
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fVec47[l121] = 0.0f;
			
		}
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			fRec82[l122] = 0.0f;
			
		}
		for (int l123 = 0; (l123 < 2); l123 = (l123 + 1)) {
			fRec81[l123] = 0.0f;
			
		}
		for (int l124 = 0; (l124 < 8192); l124 = (l124 + 1)) {
			fVec48[l124] = 0.0f;
			
		}
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			fRec85[l125] = 0.0f;
			
		}
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			fRec86[l126] = 0.0f;
			
		}
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			fVec49[l127] = 0.0f;
			
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			fRec87[l128] = 0.0f;
			
		}
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			fRec88[l129] = 0.0f;
			
		}
		for (int l130 = 0; (l130 < 8192); l130 = (l130 + 1)) {
			fVec50[l130] = 0.0f;
			
		}
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			fVec51[l131] = 0.0f;
			
		}
		for (int l132 = 0; (l132 < 4); l132 = (l132 + 1)) {
			fRec76[l132] = 0.0f;
			
		}
		for (int l133 = 0; (l133 < 2); l133 = (l133 + 1)) {
			fRec90[l133] = 0.0f;
			
		}
		for (int l134 = 0; (l134 < 8192); l134 = (l134 + 1)) {
			fVec52[l134] = 0.0f;
			
		}
		for (int l135 = 0; (l135 < 2); l135 = (l135 + 1)) {
			fVec53[l135] = 0.0f;
			
		}
		for (int l136 = 0; (l136 < 4); l136 = (l136 + 1)) {
			fRec89[l136] = 0.0f;
			
		}
		for (int l137 = 0; (l137 < 2); l137 = (l137 + 1)) {
			fRec92[l137] = 0.0f;
			
		}
		for (int l138 = 0; (l138 < 8192); l138 = (l138 + 1)) {
			fVec54[l138] = 0.0f;
			
		}
		for (int l139 = 0; (l139 < 2); l139 = (l139 + 1)) {
			fVec55[l139] = 0.0f;
			
		}
		for (int l140 = 0; (l140 < 4); l140 = (l140 + 1)) {
			fRec91[l140] = 0.0f;
			
		}
		for (int l141 = 0; (l141 < 2); l141 = (l141 + 1)) {
			fRec94[l141] = 0.0f;
			
		}
		for (int l142 = 0; (l142 < 8192); l142 = (l142 + 1)) {
			fVec56[l142] = 0.0f;
			
		}
		for (int l143 = 0; (l143 < 2); l143 = (l143 + 1)) {
			fVec57[l143] = 0.0f;
			
		}
		for (int l144 = 0; (l144 < 4); l144 = (l144 + 1)) {
			fRec93[l144] = 0.0f;
			
		}
		for (int l145 = 0; (l145 < 2); l145 = (l145 + 1)) {
			fRec96[l145] = 0.0f;
			
		}
		for (int l146 = 0; (l146 < 8192); l146 = (l146 + 1)) {
			fVec58[l146] = 0.0f;
			
		}
		for (int l147 = 0; (l147 < 2); l147 = (l147 + 1)) {
			fVec59[l147] = 0.0f;
			
		}
		for (int l148 = 0; (l148 < 4); l148 = (l148 + 1)) {
			fRec95[l148] = 0.0f;
			
		}
		for (int l149 = 0; (l149 < 2); l149 = (l149 + 1)) {
			fRec98[l149] = 0.0f;
			
		}
		for (int l150 = 0; (l150 < 8192); l150 = (l150 + 1)) {
			fVec60[l150] = 0.0f;
			
		}
		for (int l151 = 0; (l151 < 2); l151 = (l151 + 1)) {
			fVec61[l151] = 0.0f;
			
		}
		for (int l152 = 0; (l152 < 4); l152 = (l152 + 1)) {
			fRec97[l152] = 0.0f;
			
		}
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			fRec100[l153] = 0.0f;
			
		}
		for (int l154 = 0; (l154 < 8192); l154 = (l154 + 1)) {
			fVec62[l154] = 0.0f;
			
		}
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fVec63[l155] = 0.0f;
			
		}
		for (int l156 = 0; (l156 < 4); l156 = (l156 + 1)) {
			fRec99[l156] = 0.0f;
			
		}
		for (int l157 = 0; (l157 < 2); l157 = (l157 + 1)) {
			fRec102[l157] = 0.0f;
			
		}
		for (int l158 = 0; (l158 < 8192); l158 = (l158 + 1)) {
			fVec64[l158] = 0.0f;
			
		}
		for (int l159 = 0; (l159 < 2); l159 = (l159 + 1)) {
			fVec65[l159] = 0.0f;
			
		}
		for (int l160 = 0; (l160 < 4); l160 = (l160 + 1)) {
			fRec101[l160] = 0.0f;
			
		}
		for (int l161 = 0; (l161 < 2); l161 = (l161 + 1)) {
			fRec104[l161] = 0.0f;
			
		}
		for (int l162 = 0; (l162 < 8192); l162 = (l162 + 1)) {
			fVec66[l162] = 0.0f;
			
		}
		for (int l163 = 0; (l163 < 2); l163 = (l163 + 1)) {
			fVec67[l163] = 0.0f;
			
		}
		for (int l164 = 0; (l164 < 4); l164 = (l164 + 1)) {
			fRec103[l164] = 0.0f;
			
		}
		for (int l165 = 0; (l165 < 2); l165 = (l165 + 1)) {
			fVec68[l165] = 0.0f;
			
		}
		for (int l166 = 0; (l166 < 2); l166 = (l166 + 1)) {
			fRec75[l166] = 0.0f;
			
		}
		for (int l167 = 0; (l167 < 2); l167 = (l167 + 1)) {
			fRec107[l167] = 0.0f;
			
		}
		for (int l168 = 0; (l168 < 8192); l168 = (l168 + 1)) {
			fVec69[l168] = 0.0f;
			
		}
		for (int l169 = 0; (l169 < 2); l169 = (l169 + 1)) {
			fVec70[l169] = 0.0f;
			
		}
		for (int l170 = 0; (l170 < 4); l170 = (l170 + 1)) {
			fRec106[l170] = 0.0f;
			
		}
		for (int l171 = 0; (l171 < 2); l171 = (l171 + 1)) {
			fRec109[l171] = 0.0f;
			
		}
		for (int l172 = 0; (l172 < 8192); l172 = (l172 + 1)) {
			fVec71[l172] = 0.0f;
			
		}
		for (int l173 = 0; (l173 < 2); l173 = (l173 + 1)) {
			fVec72[l173] = 0.0f;
			
		}
		for (int l174 = 0; (l174 < 4); l174 = (l174 + 1)) {
			fRec108[l174] = 0.0f;
			
		}
		for (int l175 = 0; (l175 < 2); l175 = (l175 + 1)) {
			fRec111[l175] = 0.0f;
			
		}
		for (int l176 = 0; (l176 < 8192); l176 = (l176 + 1)) {
			fVec73[l176] = 0.0f;
			
		}
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fVec74[l177] = 0.0f;
			
		}
		for (int l178 = 0; (l178 < 4); l178 = (l178 + 1)) {
			fRec110[l178] = 0.0f;
			
		}
		for (int l179 = 0; (l179 < 2); l179 = (l179 + 1)) {
			fRec113[l179] = 0.0f;
			
		}
		for (int l180 = 0; (l180 < 8192); l180 = (l180 + 1)) {
			fVec75[l180] = 0.0f;
			
		}
		for (int l181 = 0; (l181 < 2); l181 = (l181 + 1)) {
			fVec76[l181] = 0.0f;
			
		}
		for (int l182 = 0; (l182 < 4); l182 = (l182 + 1)) {
			fRec112[l182] = 0.0f;
			
		}
		for (int l183 = 0; (l183 < 2); l183 = (l183 + 1)) {
			fRec115[l183] = 0.0f;
			
		}
		for (int l184 = 0; (l184 < 8192); l184 = (l184 + 1)) {
			fVec77[l184] = 0.0f;
			
		}
		for (int l185 = 0; (l185 < 2); l185 = (l185 + 1)) {
			fVec78[l185] = 0.0f;
			
		}
		for (int l186 = 0; (l186 < 4); l186 = (l186 + 1)) {
			fRec114[l186] = 0.0f;
			
		}
		for (int l187 = 0; (l187 < 2); l187 = (l187 + 1)) {
			fRec117[l187] = 0.0f;
			
		}
		for (int l188 = 0; (l188 < 8192); l188 = (l188 + 1)) {
			fVec79[l188] = 0.0f;
			
		}
		for (int l189 = 0; (l189 < 2); l189 = (l189 + 1)) {
			fVec80[l189] = 0.0f;
			
		}
		for (int l190 = 0; (l190 < 4); l190 = (l190 + 1)) {
			fRec116[l190] = 0.0f;
			
		}
		for (int l191 = 0; (l191 < 2); l191 = (l191 + 1)) {
			fRec119[l191] = 0.0f;
			
		}
		for (int l192 = 0; (l192 < 8192); l192 = (l192 + 1)) {
			fVec81[l192] = 0.0f;
			
		}
		for (int l193 = 0; (l193 < 2); l193 = (l193 + 1)) {
			fVec82[l193] = 0.0f;
			
		}
		for (int l194 = 0; (l194 < 4); l194 = (l194 + 1)) {
			fRec118[l194] = 0.0f;
			
		}
		for (int l195 = 0; (l195 < 2); l195 = (l195 + 1)) {
			fRec121[l195] = 0.0f;
			
		}
		for (int l196 = 0; (l196 < 8192); l196 = (l196 + 1)) {
			fVec83[l196] = 0.0f;
			
		}
		for (int l197 = 0; (l197 < 2); l197 = (l197 + 1)) {
			fVec84[l197] = 0.0f;
			
		}
		for (int l198 = 0; (l198 < 4); l198 = (l198 + 1)) {
			fRec120[l198] = 0.0f;
			
		}
		for (int l199 = 0; (l199 < 2); l199 = (l199 + 1)) {
			fRec123[l199] = 0.0f;
			
		}
		for (int l200 = 0; (l200 < 8192); l200 = (l200 + 1)) {
			fVec85[l200] = 0.0f;
			
		}
		for (int l201 = 0; (l201 < 2); l201 = (l201 + 1)) {
			fVec86[l201] = 0.0f;
			
		}
		for (int l202 = 0; (l202 < 4); l202 = (l202 + 1)) {
			fRec122[l202] = 0.0f;
			
		}
		for (int l203 = 0; (l203 < 2); l203 = (l203 + 1)) {
			fVec87[l203] = 0.0f;
			
		}
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fRec105[l204] = 0.0f;
			
		}
		for (int l205 = 0; (l205 < 2); l205 = (l205 + 1)) {
			fRec3[l205] = 0.0f;
			
		}
		for (int l206 = 0; (l206 < 2); l206 = (l206 + 1)) {
			fVec88[l206] = 0.0f;
			
		}
		for (int l207 = 0; (l207 < 2); l207 = (l207 + 1)) {
			fRec128[l207] = 0.0f;
			
		}
		for (int l208 = 0; (l208 < 2); l208 = (l208 + 1)) {
			fRec129[l208] = 0.0f;
			
		}
		for (int l209 = 0; (l209 < 2); l209 = (l209 + 1)) {
			fVec89[l209] = 0.0f;
			
		}
		for (int l210 = 0; (l210 < 2); l210 = (l210 + 1)) {
			fRec127[l210] = 0.0f;
			
		}
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			fRec126[l211] = 0.0f;
			
		}
		for (int l212 = 0; (l212 < 8192); l212 = (l212 + 1)) {
			fVec90[l212] = 0.0f;
			
		}
		for (int l213 = 0; (l213 < 2); l213 = (l213 + 1)) {
			fRec130[l213] = 0.0f;
			
		}
		for (int l214 = 0; (l214 < 2); l214 = (l214 + 1)) {
			fRec131[l214] = 0.0f;
			
		}
		for (int l215 = 0; (l215 < 2); l215 = (l215 + 1)) {
			fVec91[l215] = 0.0f;
			
		}
		for (int l216 = 0; (l216 < 2); l216 = (l216 + 1)) {
			fRec132[l216] = 0.0f;
			
		}
		for (int l217 = 0; (l217 < 2); l217 = (l217 + 1)) {
			fRec133[l217] = 0.0f;
			
		}
		for (int l218 = 0; (l218 < 8192); l218 = (l218 + 1)) {
			fVec92[l218] = 0.0f;
			
		}
		for (int l219 = 0; (l219 < 2); l219 = (l219 + 1)) {
			fVec93[l219] = 0.0f;
			
		}
		for (int l220 = 0; (l220 < 4); l220 = (l220 + 1)) {
			fRec125[l220] = 0.0f;
			
		}
		for (int l221 = 0; (l221 < 2); l221 = (l221 + 1)) {
			fRec135[l221] = 0.0f;
			
		}
		for (int l222 = 0; (l222 < 8192); l222 = (l222 + 1)) {
			fVec94[l222] = 0.0f;
			
		}
		for (int l223 = 0; (l223 < 2); l223 = (l223 + 1)) {
			fVec95[l223] = 0.0f;
			
		}
		for (int l224 = 0; (l224 < 4); l224 = (l224 + 1)) {
			fRec134[l224] = 0.0f;
			
		}
		for (int l225 = 0; (l225 < 2); l225 = (l225 + 1)) {
			fRec137[l225] = 0.0f;
			
		}
		for (int l226 = 0; (l226 < 8192); l226 = (l226 + 1)) {
			fVec96[l226] = 0.0f;
			
		}
		for (int l227 = 0; (l227 < 2); l227 = (l227 + 1)) {
			fVec97[l227] = 0.0f;
			
		}
		for (int l228 = 0; (l228 < 4); l228 = (l228 + 1)) {
			fRec136[l228] = 0.0f;
			
		}
		for (int l229 = 0; (l229 < 2); l229 = (l229 + 1)) {
			fRec139[l229] = 0.0f;
			
		}
		for (int l230 = 0; (l230 < 8192); l230 = (l230 + 1)) {
			fVec98[l230] = 0.0f;
			
		}
		for (int l231 = 0; (l231 < 2); l231 = (l231 + 1)) {
			fVec99[l231] = 0.0f;
			
		}
		for (int l232 = 0; (l232 < 4); l232 = (l232 + 1)) {
			fRec138[l232] = 0.0f;
			
		}
		for (int l233 = 0; (l233 < 2); l233 = (l233 + 1)) {
			fRec141[l233] = 0.0f;
			
		}
		for (int l234 = 0; (l234 < 8192); l234 = (l234 + 1)) {
			fVec100[l234] = 0.0f;
			
		}
		for (int l235 = 0; (l235 < 2); l235 = (l235 + 1)) {
			fVec101[l235] = 0.0f;
			
		}
		for (int l236 = 0; (l236 < 4); l236 = (l236 + 1)) {
			fRec140[l236] = 0.0f;
			
		}
		for (int l237 = 0; (l237 < 2); l237 = (l237 + 1)) {
			fRec143[l237] = 0.0f;
			
		}
		for (int l238 = 0; (l238 < 8192); l238 = (l238 + 1)) {
			fVec102[l238] = 0.0f;
			
		}
		for (int l239 = 0; (l239 < 2); l239 = (l239 + 1)) {
			fVec103[l239] = 0.0f;
			
		}
		for (int l240 = 0; (l240 < 4); l240 = (l240 + 1)) {
			fRec142[l240] = 0.0f;
			
		}
		for (int l241 = 0; (l241 < 2); l241 = (l241 + 1)) {
			fRec145[l241] = 0.0f;
			
		}
		for (int l242 = 0; (l242 < 8192); l242 = (l242 + 1)) {
			fVec104[l242] = 0.0f;
			
		}
		for (int l243 = 0; (l243 < 2); l243 = (l243 + 1)) {
			fVec105[l243] = 0.0f;
			
		}
		for (int l244 = 0; (l244 < 4); l244 = (l244 + 1)) {
			fRec144[l244] = 0.0f;
			
		}
		for (int l245 = 0; (l245 < 2); l245 = (l245 + 1)) {
			fRec147[l245] = 0.0f;
			
		}
		for (int l246 = 0; (l246 < 8192); l246 = (l246 + 1)) {
			fVec106[l246] = 0.0f;
			
		}
		for (int l247 = 0; (l247 < 2); l247 = (l247 + 1)) {
			fVec107[l247] = 0.0f;
			
		}
		for (int l248 = 0; (l248 < 4); l248 = (l248 + 1)) {
			fRec146[l248] = 0.0f;
			
		}
		for (int l249 = 0; (l249 < 2); l249 = (l249 + 1)) {
			fRec149[l249] = 0.0f;
			
		}
		for (int l250 = 0; (l250 < 8192); l250 = (l250 + 1)) {
			fVec108[l250] = 0.0f;
			
		}
		for (int l251 = 0; (l251 < 2); l251 = (l251 + 1)) {
			fVec109[l251] = 0.0f;
			
		}
		for (int l252 = 0; (l252 < 4); l252 = (l252 + 1)) {
			fRec148[l252] = 0.0f;
			
		}
		for (int l253 = 0; (l253 < 2); l253 = (l253 + 1)) {
			fVec110[l253] = 0.0f;
			
		}
		for (int l254 = 0; (l254 < 2); l254 = (l254 + 1)) {
			fRec124[l254] = 0.0f;
			
		}
		for (int l255 = 0; (l255 < 2); l255 = (l255 + 1)) {
			fRec152[l255] = 0.0f;
			
		}
		for (int l256 = 0; (l256 < 8192); l256 = (l256 + 1)) {
			fVec111[l256] = 0.0f;
			
		}
		for (int l257 = 0; (l257 < 2); l257 = (l257 + 1)) {
			fVec112[l257] = 0.0f;
			
		}
		for (int l258 = 0; (l258 < 4); l258 = (l258 + 1)) {
			fRec151[l258] = 0.0f;
			
		}
		for (int l259 = 0; (l259 < 2); l259 = (l259 + 1)) {
			fRec154[l259] = 0.0f;
			
		}
		for (int l260 = 0; (l260 < 8192); l260 = (l260 + 1)) {
			fVec113[l260] = 0.0f;
			
		}
		for (int l261 = 0; (l261 < 2); l261 = (l261 + 1)) {
			fVec114[l261] = 0.0f;
			
		}
		for (int l262 = 0; (l262 < 4); l262 = (l262 + 1)) {
			fRec153[l262] = 0.0f;
			
		}
		for (int l263 = 0; (l263 < 2); l263 = (l263 + 1)) {
			fRec156[l263] = 0.0f;
			
		}
		for (int l264 = 0; (l264 < 8192); l264 = (l264 + 1)) {
			fVec115[l264] = 0.0f;
			
		}
		for (int l265 = 0; (l265 < 2); l265 = (l265 + 1)) {
			fVec116[l265] = 0.0f;
			
		}
		for (int l266 = 0; (l266 < 4); l266 = (l266 + 1)) {
			fRec155[l266] = 0.0f;
			
		}
		for (int l267 = 0; (l267 < 2); l267 = (l267 + 1)) {
			fRec158[l267] = 0.0f;
			
		}
		for (int l268 = 0; (l268 < 8192); l268 = (l268 + 1)) {
			fVec117[l268] = 0.0f;
			
		}
		for (int l269 = 0; (l269 < 2); l269 = (l269 + 1)) {
			fVec118[l269] = 0.0f;
			
		}
		for (int l270 = 0; (l270 < 4); l270 = (l270 + 1)) {
			fRec157[l270] = 0.0f;
			
		}
		for (int l271 = 0; (l271 < 2); l271 = (l271 + 1)) {
			fRec160[l271] = 0.0f;
			
		}
		for (int l272 = 0; (l272 < 8192); l272 = (l272 + 1)) {
			fVec119[l272] = 0.0f;
			
		}
		for (int l273 = 0; (l273 < 2); l273 = (l273 + 1)) {
			fVec120[l273] = 0.0f;
			
		}
		for (int l274 = 0; (l274 < 4); l274 = (l274 + 1)) {
			fRec159[l274] = 0.0f;
			
		}
		for (int l275 = 0; (l275 < 2); l275 = (l275 + 1)) {
			fRec162[l275] = 0.0f;
			
		}
		for (int l276 = 0; (l276 < 8192); l276 = (l276 + 1)) {
			fVec121[l276] = 0.0f;
			
		}
		for (int l277 = 0; (l277 < 2); l277 = (l277 + 1)) {
			fVec122[l277] = 0.0f;
			
		}
		for (int l278 = 0; (l278 < 4); l278 = (l278 + 1)) {
			fRec161[l278] = 0.0f;
			
		}
		for (int l279 = 0; (l279 < 2); l279 = (l279 + 1)) {
			fRec164[l279] = 0.0f;
			
		}
		for (int l280 = 0; (l280 < 8192); l280 = (l280 + 1)) {
			fVec123[l280] = 0.0f;
			
		}
		for (int l281 = 0; (l281 < 2); l281 = (l281 + 1)) {
			fVec124[l281] = 0.0f;
			
		}
		for (int l282 = 0; (l282 < 4); l282 = (l282 + 1)) {
			fRec163[l282] = 0.0f;
			
		}
		for (int l283 = 0; (l283 < 2); l283 = (l283 + 1)) {
			fRec166[l283] = 0.0f;
			
		}
		for (int l284 = 0; (l284 < 8192); l284 = (l284 + 1)) {
			fVec125[l284] = 0.0f;
			
		}
		for (int l285 = 0; (l285 < 2); l285 = (l285 + 1)) {
			fVec126[l285] = 0.0f;
			
		}
		for (int l286 = 0; (l286 < 4); l286 = (l286 + 1)) {
			fRec165[l286] = 0.0f;
			
		}
		for (int l287 = 0; (l287 < 2); l287 = (l287 + 1)) {
			fRec168[l287] = 0.0f;
			
		}
		for (int l288 = 0; (l288 < 8192); l288 = (l288 + 1)) {
			fVec127[l288] = 0.0f;
			
		}
		for (int l289 = 0; (l289 < 2); l289 = (l289 + 1)) {
			fVec128[l289] = 0.0f;
			
		}
		for (int l290 = 0; (l290 < 4); l290 = (l290 + 1)) {
			fRec167[l290] = 0.0f;
			
		}
		for (int l291 = 0; (l291 < 2); l291 = (l291 + 1)) {
			fVec129[l291] = 0.0f;
			
		}
		for (int l292 = 0; (l292 < 2); l292 = (l292 + 1)) {
			fRec150[l292] = 0.0f;
			
		}
		for (int l293 = 0; (l293 < 2); l293 = (l293 + 1)) {
			fRec5[l293] = 0.0f;
			
		}
		for (int l294 = 0; (l294 < 2); l294 = (l294 + 1)) {
			fVec130[l294] = 0.0f;
			
		}
		for (int l295 = 0; (l295 < 2); l295 = (l295 + 1)) {
			fRec173[l295] = 0.0f;
			
		}
		for (int l296 = 0; (l296 < 2); l296 = (l296 + 1)) {
			fRec174[l296] = 0.0f;
			
		}
		for (int l297 = 0; (l297 < 2); l297 = (l297 + 1)) {
			fVec131[l297] = 0.0f;
			
		}
		for (int l298 = 0; (l298 < 2); l298 = (l298 + 1)) {
			fRec172[l298] = 0.0f;
			
		}
		for (int l299 = 0; (l299 < 2); l299 = (l299 + 1)) {
			fRec171[l299] = 0.0f;
			
		}
		for (int l300 = 0; (l300 < 8192); l300 = (l300 + 1)) {
			fVec132[l300] = 0.0f;
			
		}
		for (int l301 = 0; (l301 < 2); l301 = (l301 + 1)) {
			fRec175[l301] = 0.0f;
			
		}
		for (int l302 = 0; (l302 < 2); l302 = (l302 + 1)) {
			fRec176[l302] = 0.0f;
			
		}
		for (int l303 = 0; (l303 < 2); l303 = (l303 + 1)) {
			fVec133[l303] = 0.0f;
			
		}
		for (int l304 = 0; (l304 < 2); l304 = (l304 + 1)) {
			fRec177[l304] = 0.0f;
			
		}
		for (int l305 = 0; (l305 < 2); l305 = (l305 + 1)) {
			fRec178[l305] = 0.0f;
			
		}
		for (int l306 = 0; (l306 < 8192); l306 = (l306 + 1)) {
			fVec134[l306] = 0.0f;
			
		}
		for (int l307 = 0; (l307 < 2); l307 = (l307 + 1)) {
			fVec135[l307] = 0.0f;
			
		}
		for (int l308 = 0; (l308 < 4); l308 = (l308 + 1)) {
			fRec170[l308] = 0.0f;
			
		}
		for (int l309 = 0; (l309 < 2); l309 = (l309 + 1)) {
			fRec180[l309] = 0.0f;
			
		}
		for (int l310 = 0; (l310 < 8192); l310 = (l310 + 1)) {
			fVec136[l310] = 0.0f;
			
		}
		for (int l311 = 0; (l311 < 2); l311 = (l311 + 1)) {
			fVec137[l311] = 0.0f;
			
		}
		for (int l312 = 0; (l312 < 4); l312 = (l312 + 1)) {
			fRec179[l312] = 0.0f;
			
		}
		for (int l313 = 0; (l313 < 2); l313 = (l313 + 1)) {
			fRec182[l313] = 0.0f;
			
		}
		for (int l314 = 0; (l314 < 8192); l314 = (l314 + 1)) {
			fVec138[l314] = 0.0f;
			
		}
		for (int l315 = 0; (l315 < 2); l315 = (l315 + 1)) {
			fVec139[l315] = 0.0f;
			
		}
		for (int l316 = 0; (l316 < 4); l316 = (l316 + 1)) {
			fRec181[l316] = 0.0f;
			
		}
		for (int l317 = 0; (l317 < 2); l317 = (l317 + 1)) {
			fRec184[l317] = 0.0f;
			
		}
		for (int l318 = 0; (l318 < 8192); l318 = (l318 + 1)) {
			fVec140[l318] = 0.0f;
			
		}
		for (int l319 = 0; (l319 < 2); l319 = (l319 + 1)) {
			fVec141[l319] = 0.0f;
			
		}
		for (int l320 = 0; (l320 < 4); l320 = (l320 + 1)) {
			fRec183[l320] = 0.0f;
			
		}
		for (int l321 = 0; (l321 < 2); l321 = (l321 + 1)) {
			fRec186[l321] = 0.0f;
			
		}
		for (int l322 = 0; (l322 < 8192); l322 = (l322 + 1)) {
			fVec142[l322] = 0.0f;
			
		}
		for (int l323 = 0; (l323 < 2); l323 = (l323 + 1)) {
			fVec143[l323] = 0.0f;
			
		}
		for (int l324 = 0; (l324 < 4); l324 = (l324 + 1)) {
			fRec185[l324] = 0.0f;
			
		}
		for (int l325 = 0; (l325 < 2); l325 = (l325 + 1)) {
			fRec188[l325] = 0.0f;
			
		}
		for (int l326 = 0; (l326 < 8192); l326 = (l326 + 1)) {
			fVec144[l326] = 0.0f;
			
		}
		for (int l327 = 0; (l327 < 2); l327 = (l327 + 1)) {
			fVec145[l327] = 0.0f;
			
		}
		for (int l328 = 0; (l328 < 4); l328 = (l328 + 1)) {
			fRec187[l328] = 0.0f;
			
		}
		for (int l329 = 0; (l329 < 2); l329 = (l329 + 1)) {
			fRec190[l329] = 0.0f;
			
		}
		for (int l330 = 0; (l330 < 8192); l330 = (l330 + 1)) {
			fVec146[l330] = 0.0f;
			
		}
		for (int l331 = 0; (l331 < 2); l331 = (l331 + 1)) {
			fVec147[l331] = 0.0f;
			
		}
		for (int l332 = 0; (l332 < 4); l332 = (l332 + 1)) {
			fRec189[l332] = 0.0f;
			
		}
		for (int l333 = 0; (l333 < 2); l333 = (l333 + 1)) {
			fRec192[l333] = 0.0f;
			
		}
		for (int l334 = 0; (l334 < 8192); l334 = (l334 + 1)) {
			fVec148[l334] = 0.0f;
			
		}
		for (int l335 = 0; (l335 < 2); l335 = (l335 + 1)) {
			fVec149[l335] = 0.0f;
			
		}
		for (int l336 = 0; (l336 < 4); l336 = (l336 + 1)) {
			fRec191[l336] = 0.0f;
			
		}
		for (int l337 = 0; (l337 < 2); l337 = (l337 + 1)) {
			fRec194[l337] = 0.0f;
			
		}
		for (int l338 = 0; (l338 < 8192); l338 = (l338 + 1)) {
			fVec150[l338] = 0.0f;
			
		}
		for (int l339 = 0; (l339 < 2); l339 = (l339 + 1)) {
			fVec151[l339] = 0.0f;
			
		}
		for (int l340 = 0; (l340 < 4); l340 = (l340 + 1)) {
			fRec193[l340] = 0.0f;
			
		}
		for (int l341 = 0; (l341 < 2); l341 = (l341 + 1)) {
			fVec152[l341] = 0.0f;
			
		}
		for (int l342 = 0; (l342 < 2); l342 = (l342 + 1)) {
			fRec169[l342] = 0.0f;
			
		}
		for (int l343 = 0; (l343 < 2); l343 = (l343 + 1)) {
			fRec197[l343] = 0.0f;
			
		}
		for (int l344 = 0; (l344 < 8192); l344 = (l344 + 1)) {
			fVec153[l344] = 0.0f;
			
		}
		for (int l345 = 0; (l345 < 2); l345 = (l345 + 1)) {
			fVec154[l345] = 0.0f;
			
		}
		for (int l346 = 0; (l346 < 4); l346 = (l346 + 1)) {
			fRec196[l346] = 0.0f;
			
		}
		for (int l347 = 0; (l347 < 2); l347 = (l347 + 1)) {
			fRec199[l347] = 0.0f;
			
		}
		for (int l348 = 0; (l348 < 8192); l348 = (l348 + 1)) {
			fVec155[l348] = 0.0f;
			
		}
		for (int l349 = 0; (l349 < 2); l349 = (l349 + 1)) {
			fVec156[l349] = 0.0f;
			
		}
		for (int l350 = 0; (l350 < 4); l350 = (l350 + 1)) {
			fRec198[l350] = 0.0f;
			
		}
		for (int l351 = 0; (l351 < 2); l351 = (l351 + 1)) {
			fRec201[l351] = 0.0f;
			
		}
		for (int l352 = 0; (l352 < 8192); l352 = (l352 + 1)) {
			fVec157[l352] = 0.0f;
			
		}
		for (int l353 = 0; (l353 < 2); l353 = (l353 + 1)) {
			fVec158[l353] = 0.0f;
			
		}
		for (int l354 = 0; (l354 < 4); l354 = (l354 + 1)) {
			fRec200[l354] = 0.0f;
			
		}
		for (int l355 = 0; (l355 < 2); l355 = (l355 + 1)) {
			fRec203[l355] = 0.0f;
			
		}
		for (int l356 = 0; (l356 < 8192); l356 = (l356 + 1)) {
			fVec159[l356] = 0.0f;
			
		}
		for (int l357 = 0; (l357 < 2); l357 = (l357 + 1)) {
			fVec160[l357] = 0.0f;
			
		}
		for (int l358 = 0; (l358 < 4); l358 = (l358 + 1)) {
			fRec202[l358] = 0.0f;
			
		}
		for (int l359 = 0; (l359 < 2); l359 = (l359 + 1)) {
			fRec205[l359] = 0.0f;
			
		}
		for (int l360 = 0; (l360 < 8192); l360 = (l360 + 1)) {
			fVec161[l360] = 0.0f;
			
		}
		for (int l361 = 0; (l361 < 2); l361 = (l361 + 1)) {
			fVec162[l361] = 0.0f;
			
		}
		for (int l362 = 0; (l362 < 4); l362 = (l362 + 1)) {
			fRec204[l362] = 0.0f;
			
		}
		for (int l363 = 0; (l363 < 2); l363 = (l363 + 1)) {
			fRec207[l363] = 0.0f;
			
		}
		for (int l364 = 0; (l364 < 8192); l364 = (l364 + 1)) {
			fVec163[l364] = 0.0f;
			
		}
		for (int l365 = 0; (l365 < 2); l365 = (l365 + 1)) {
			fVec164[l365] = 0.0f;
			
		}
		for (int l366 = 0; (l366 < 4); l366 = (l366 + 1)) {
			fRec206[l366] = 0.0f;
			
		}
		for (int l367 = 0; (l367 < 2); l367 = (l367 + 1)) {
			fRec209[l367] = 0.0f;
			
		}
		for (int l368 = 0; (l368 < 8192); l368 = (l368 + 1)) {
			fVec165[l368] = 0.0f;
			
		}
		for (int l369 = 0; (l369 < 2); l369 = (l369 + 1)) {
			fVec166[l369] = 0.0f;
			
		}
		for (int l370 = 0; (l370 < 4); l370 = (l370 + 1)) {
			fRec208[l370] = 0.0f;
			
		}
		for (int l371 = 0; (l371 < 2); l371 = (l371 + 1)) {
			fRec211[l371] = 0.0f;
			
		}
		for (int l372 = 0; (l372 < 8192); l372 = (l372 + 1)) {
			fVec167[l372] = 0.0f;
			
		}
		for (int l373 = 0; (l373 < 2); l373 = (l373 + 1)) {
			fVec168[l373] = 0.0f;
			
		}
		for (int l374 = 0; (l374 < 4); l374 = (l374 + 1)) {
			fRec210[l374] = 0.0f;
			
		}
		for (int l375 = 0; (l375 < 2); l375 = (l375 + 1)) {
			fRec213[l375] = 0.0f;
			
		}
		for (int l376 = 0; (l376 < 8192); l376 = (l376 + 1)) {
			fVec169[l376] = 0.0f;
			
		}
		for (int l377 = 0; (l377 < 2); l377 = (l377 + 1)) {
			fVec170[l377] = 0.0f;
			
		}
		for (int l378 = 0; (l378 < 4); l378 = (l378 + 1)) {
			fRec212[l378] = 0.0f;
			
		}
		for (int l379 = 0; (l379 < 2); l379 = (l379 + 1)) {
			fVec171[l379] = 0.0f;
			
		}
		for (int l380 = 0; (l380 < 2); l380 = (l380 + 1)) {
			fRec195[l380] = 0.0f;
			
		}
		for (int l381 = 0; (l381 < 2); l381 = (l381 + 1)) {
			fRec7[l381] = 0.0f;
			
		}
		for (int l382 = 0; (l382 < 2); l382 = (l382 + 1)) {
			fRec214[l382] = 0.0f;
			
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
	
	virtual tambura* clone() {
		return new tambura();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Tambura");
		ui_interface->declare(&fHslider5, "scale", "exp");
		ui_interface->addHorizontalSlider("attack_time", &fHslider5, 0.0700000003f, 0.0f, 0.5f, 0.00999999978f);
		ui_interface->addCheckButton("auto_pluck", &fCheckbox0);
		ui_interface->declare(&fHslider2, "unit", "hz");
		ui_interface->addHorizontalSlider("auto_pluck_rate", &fHslider2, 0.100000001f, 0.0f, 2.0f, 0.00100000005f);
		ui_interface->declare(&fHslider11, "unit", "st");
		ui_interface->addHorizontalSlider("bend_depth", &fHslider11, 3.0f, 0.0f, 12.0f, 0.00999999978f);
		ui_interface->declare(&fHslider12, "unit", "ms");
		ui_interface->addHorizontalSlider("bend_time", &fHslider12, 10.0f, 1.0f, 200.0f, 1.0f);
		ui_interface->addHorizontalSlider("decay_scale", &fHslider9, 1.0f, 0.100000001f, 1.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("decay_time", &fHslider3, 1.0f, 1.0f, 100.0f, 0.00999999978f);
		ui_interface->declare(&fHslider10, "unit", "ms");
		ui_interface->addHorizontalSlider("decay_time", &fHslider10, 10000.0f, 0.0f, 100000.0f, 0.100000001f);
		ui_interface->declare(&fHslider13, "scale", "exp");
		ui_interface->addHorizontalSlider("harmonic_motion", &fHslider13, 0.00100000005f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addHorizontalSlider("high_freq_loss", &fHslider6, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("jawari", &fHslider8, 0.0f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("material", &fHslider1, 0.129999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("note", &fHslider4, 36.0f, 24.0f, 72.0f, 1.0f);
		ui_interface->addButton("pluck0", &fButton0);
		ui_interface->addButton("pluck1", &fButton1);
		ui_interface->addButton("pluck2", &fButton2);
		ui_interface->addButton("pluck3", &fButton3);
		ui_interface->addHorizontalSlider("position", &fHslider7, 0.25f, 0.00999999978f, 0.5f, 0.00999999978f);
		ui_interface->addHorizontalSlider("string_spread", &fHslider15, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("sympathetic_coupling", &fHslider0, 0.100000001f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addHorizontalSlider("tune_scale", &fHslider14, 1.0f, 0.899999976f, 1.10000002f, 0.00100000005f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = (fConst20 * float(fHslider0));
		float fSlow1 = (fConst20 * float(fHslider1));
		float fSlow2 = float(fCheckbox0);
		float fSlow3 = (fConst29 * float(fHslider2));
		float fSlow4 = float(fButton0);
		float fSlow5 = float(fHslider3);
		float fSlow6 = (fConst30 * fSlow5);
		float fSlow7 = (fConst20 * float(fHslider4));
		float fSlow8 = float(fHslider5);
		float fSlow9 = (fConst31 / fSlow8);
		float fSlow10 = (fConst20 * (1.0f - float(fHslider6)));
		float fSlow11 = float(fHslider7);
		float fSlow12 = (fConst33 * float(fHslider8));
		float fSlow13 = float(fHslider9);
		float fSlow14 = (0.0022727272f / fSlow13);
		float fSlow15 = (fConst34 * float(fHslider10));
		float fSlow16 = float(fHslider11);
		float fSlow17 = (tambura_faustpower2_f(fSlow4) * fSlow16);
		float fSlow18 = (1.0f - fSlow4);
		float fSlow19 = (fConst35 * float(fHslider12));
		float fSlow20 = (fConst36 * float(fHslider13));
		float fSlow21 = float(fHslider14);
		float fSlow22 = (0.0022727272f / (fSlow21 * fSlow13));
		float fSlow23 = (fConst37 / fSlow21);
		float fSlow24 = float(fButton1);
		float fSlow25 = (fConst38 * fSlow5);
		float fSlow26 = (fConst39 / fSlow8);
		float fSlow27 = (tambura_faustpower2_f(fSlow24) * fSlow16);
		float fSlow28 = (1.0f - fSlow24);
		float fSlow29 = float(fButton2);
		float fSlow30 = (fConst40 * fSlow5);
		float fSlow31 = (fConst41 / fSlow8);
		float fSlow32 = (tambura_faustpower2_f(fSlow29) * fSlow16);
		float fSlow33 = (1.0f - fSlow29);
		float fSlow34 = float(fButton3);
		float fSlow35 = (fConst37 * fSlow5);
		float fSlow36 = (fConst42 / fSlow8);
		float fSlow37 = (tambura_faustpower2_f(fSlow34) * fSlow16);
		float fSlow38 = (1.0f - fSlow34);
		float fSlow39 = (fConst20 * float(fHslider15));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec13[0] = (fSlow0 + (fConst19 * fRec13[1]));
			float fTemp0 = (fRec7[1] * fRec13[0]);
			fVec0[0] = fTemp0;
			fRec12[0] = ((fConst16 * fRec12[1]) + (fConst17 * ((fConst18 * fVec0[1]) + (fConst14 * fTemp0))));
			fRec14[0] = ((fConst16 * fRec14[1]) + (fConst17 * (fTemp0 + fVec0[1])));
			float fTemp1 = (fConst13 * fRec11[1]);
			fRec11[0] = (((9.99999975e-06f * fRec12[0]) + fRec14[0]) - (fConst12 * (fTemp1 + (fConst21 * fRec11[2]))));
			float fTemp2 = (fConst7 * fRec10[1]);
			fRec10[0] = ((fConst12 * ((fTemp1 + (fConst23 * fRec11[0])) + (fConst24 * fRec11[2]))) - (fConst6 * (fTemp2 + (fConst25 * fRec10[2]))));
			float fTemp3 = ((fTemp2 + (fConst27 * fRec10[0])) + (fConst28 * fRec10[2]));
			float fTemp4 = (fRec3[1] * fRec13[0]);
			fVec1[0] = fTemp4;
			fRec17[0] = ((fConst16 * fRec17[1]) + (fConst17 * ((fConst18 * fVec1[1]) + (fConst14 * fTemp4))));
			fRec18[0] = ((fConst16 * fRec18[1]) + (fConst17 * (fTemp4 + fVec1[1])));
			float fTemp5 = (fConst13 * fRec16[1]);
			fRec16[0] = (((9.99999975e-06f * fRec17[0]) + fRec18[0]) - (fConst12 * ((fConst21 * fRec16[2]) + fTemp5)));
			float fTemp6 = (fConst7 * fRec15[1]);
			fRec15[0] = ((fConst12 * ((fTemp5 + (fConst23 * fRec16[0])) + (fConst24 * fRec16[2]))) - (fConst6 * ((fConst25 * fRec15[2]) + fTemp6)));
			float fTemp7 = ((fTemp6 + (fConst27 * fRec15[0])) + (fConst28 * fRec15[2]));
			float fTemp8 = (fRec5[1] * fRec13[0]);
			fVec2[0] = fTemp8;
			fRec21[0] = ((fConst16 * fRec21[1]) + (fConst17 * ((fConst18 * fVec2[1]) + (fConst14 * fTemp8))));
			fRec22[0] = ((fConst16 * fRec22[1]) + (fConst17 * (fTemp8 + fVec2[1])));
			float fTemp9 = (fConst13 * fRec20[1]);
			fRec20[0] = (((9.99999975e-06f * fRec21[0]) + fRec22[0]) - (fConst12 * (fTemp9 + (fConst21 * fRec20[2]))));
			float fTemp10 = (fConst7 * fRec19[1]);
			fRec19[0] = ((fConst12 * ((fTemp9 + (fConst23 * fRec20[0])) + (fConst24 * fRec20[2]))) - (fConst6 * (fTemp10 + (fConst25 * fRec19[2]))));
			float fTemp11 = ((fTemp10 + (fConst27 * fRec19[0])) + (fConst28 * fRec19[2]));
			iRec24[0] = ((1103515245 * iRec24[1]) + 12345);
			float fTemp12 = (4.65661287e-10f * float(iRec24[0]));
			fRec23[0] = ((fTemp12 + ((2.49495602f * fRec23[1]) + (0.522189379f * fRec23[3]))) - (2.0172658f * fRec23[2]));
			fRec25[0] = (fSlow1 + (fConst19 * fRec25[1]));
			float fTemp13 = ((((((0.0499220341f * fRec23[0]) + (0.0506126992f * fRec23[2])) - ((0.0959935337f * fRec23[1]) + (0.00440878607f * fRec23[3]))) + -1.0f) * fRec25[0]) + 1.0f);
			fRec30[0] = (fSlow3 + (fRec30[1] - floorf((fSlow3 + fRec30[1]))));
			float fTemp14 = (fSlow2 * float((fRec30[0] < 0.25f)));
			float fTemp15 = ((1.0f - fTemp14) - fSlow4);
			fVec3[0] = fTemp15;
			fRec31[0] = (fSlow7 + (fConst19 * fRec31[1]));
			float fTemp16 = powf(2.0f, (0.0833333358f * (fRec31[0] + -69.0f)));
			float fTemp17 = ((fTemp15 != fVec3[1])?(fSlow6 / fTemp16):(fRec28[1] + -1.0f));
			fRec28[0] = fTemp17;
			fRec29[0] = ((fTemp17 <= 0.0f)?fTemp15:(fRec29[1] + (((1.0f - (fTemp14 + fRec29[1])) - fSlow4) / fTemp17)));
			float fTemp18 = (fSlow4 + fTemp14);
			fVec4[0] = fTemp18;
			float fTemp19 = (fRec29[0] * fTemp18);
			float fTemp20 = ((fRec26[1] > fTemp19)?fConst32:expf((0.0f - (fSlow9 * fTemp16))));
			fRec27[0] = ((fRec27[1] * fTemp20) + (fTemp19 * (1.0f - fTemp20)));
			fRec26[0] = fRec27[0];
			float fTemp21 = (fTemp13 * fRec26[0]);
			fVec5[(IOTA & 8191)] = fTemp21;
			float fTemp22 = ((fConst6 * (fTemp3 + (fTemp7 + fTemp11))) + fTemp21);
			fRec32[0] = (fSlow10 + (fConst19 * fRec32[1]));
			int iTemp23 = ((fVec4[1] <= 0.0f) & (fTemp18 > 0.0f));
			float fTemp24 = fabsf(fTemp12);
			fRec33[0] = ((fRec33[1] * float((1 - iTemp23))) + (float(iTemp23) * fTemp24));
			float fTemp25 = (fRec32[0] * (fTemp22 - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]));
			float fTemp26 = (fTemp25 + 1.0f);
			float fTemp27 = (1.0f - fTemp25);
			float fTemp28 = ((0.5f * (fTemp26 * fRec9[2])) + (0.25f * (fTemp27 * (fRec9[1] + fRec9[3]))));
			fRec34[0] = (fSlow12 + (fConst19 * fRec34[1]));
			float fTemp29 = (3.14159274f * (fRec34[0] * fTemp28));
			float fTemp30 = sinf(fTemp29);
			float fTemp31 = cosf(fTemp29);
			fRec35[0] = ((fRec35[1] * (0.0f - fTemp30)) + (fTemp28 * fTemp31));
			fRec36[0] = (fSlow15 + (fConst19 * fRec36[1]));
			fVec6[0] = fSlow18;
			float fTemp32 = ((fSlow18 != fVec6[1])?fSlow19:(fRec37[1] + -1.0f));
			fRec37[0] = fTemp32;
			fRec38[0] = ((fTemp32 <= 0.0f)?fSlow18:(fRec38[1] + (((1.0f - fRec38[1]) - fSlow4) / fTemp32)));
			float fTemp33 = (fSlow17 * tambura_faustpower2_f(fRec38[0]));
			fRec39[0] = (fSlow20 + (fConst19 * fRec39[1]));
			float fTemp34 = (660.0f * fTemp16);
			float fTemp35 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp33 + (fRec39[0] + fTemp34))))));
			float fTemp36 = (fRec36[0] * fTemp35);
			float fTemp37 = ((fTemp22 + (((fTemp28 * fTemp30) + (fRec35[1] * fTemp31)) * powf(0.00100000005f, (fSlow14 / fTemp36)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec7[(IOTA & 8191)] = fTemp37;
			float fTemp38 = (fConst37 / fTemp35);
			float fTemp39 = (fTemp38 + -2.49999499f);
			int iTemp40 = min(4096, max(0, int(fTemp39)));
			float fTemp41 = floorf(fTemp39);
			float fTemp42 = ((fVec7[((IOTA - iTemp40) & 8191)] - fVec7[((IOTA - (iTemp40 + 1)) & 8191)]) * (fTemp41 + (2.0f - fTemp38)));
			fVec8[0] = fTemp42;
			fRec9[0] = ((0.5f * ((fTemp42 - fVec8[1]) * (fTemp41 + (3.0f - fTemp38)))) + (fVec7[((IOTA - iTemp40) & 8191)] + fTemp42));
			float fTemp43 = ((0.5f * (fTemp26 * fRec40[2])) + (0.25f * (fTemp27 * (fRec40[1] + fRec40[3]))));
			float fTemp44 = (3.14159274f * (fRec34[0] * fTemp43));
			float fTemp45 = sinf(fTemp44);
			float fTemp46 = cosf(fTemp44);
			fRec41[0] = ((fRec41[1] * (0.0f - fTemp45)) + (fTemp43 * fTemp46));
			float fTemp47 = (fTemp34 + fTemp33);
			float fTemp48 = powf(2.0f, (1.44269502f * logf((0.0022727272f * fTemp47))));
			float fTemp49 = (fRec36[0] * fTemp48);
			float fTemp50 = ((fTemp22 + (((fTemp43 * fTemp45) + (fRec41[1] * fTemp46)) * powf(0.00100000005f, (fSlow14 / fTemp49)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec9[(IOTA & 8191)] = fTemp50;
			float fTemp51 = (fConst37 / fTemp48);
			float fTemp52 = (fTemp51 + -2.49999499f);
			int iTemp53 = min(4096, max(0, int(fTemp52)));
			float fTemp54 = floorf(fTemp52);
			float fTemp55 = ((fVec9[((IOTA - iTemp53) & 8191)] - fVec9[((IOTA - (iTemp53 + 1)) & 8191)]) * (fTemp54 + (2.0f - fTemp51)));
			fVec10[0] = fTemp55;
			fRec40[0] = ((0.5f * ((fTemp55 - fVec10[1]) * (fTemp54 + (3.0f - fTemp51)))) + (fVec9[((IOTA - iTemp53) & 8191)] + fTemp55));
			float fTemp56 = ((0.5f * (fTemp26 * fRec42[2])) + (0.25f * (fTemp27 * (fRec42[1] + fRec42[3]))));
			float fTemp57 = (3.14159274f * (fRec34[0] * fTemp56));
			float fTemp58 = sinf(fTemp57);
			float fTemp59 = cosf(fTemp57);
			fRec43[0] = ((fRec43[1] * (0.0f - fTemp58)) + (fTemp56 * fTemp59));
			float fTemp60 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp47 - fRec39[0])))));
			float fTemp61 = (fRec36[0] * fTemp60);
			float fTemp62 = ((fTemp22 + (((fTemp56 * fTemp58) + (fRec43[1] * fTemp59)) * powf(0.00100000005f, (fSlow14 / fTemp61)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec11[(IOTA & 8191)] = fTemp62;
			float fTemp63 = (fConst37 / fTemp60);
			float fTemp64 = (fTemp63 + -2.49999499f);
			int iTemp65 = min(4096, max(0, int(fTemp64)));
			float fTemp66 = floorf(fTemp64);
			float fTemp67 = ((fVec11[((IOTA - iTemp65) & 8191)] - fVec11[((IOTA - (iTemp65 + 1)) & 8191)]) * (fTemp66 + (2.0f - fTemp63)));
			fVec12[0] = fTemp67;
			fRec42[0] = ((0.5f * ((fTemp67 - fVec12[1]) * (fTemp66 + (3.0f - fTemp63)))) + (fVec11[((IOTA - iTemp65) & 8191)] + fTemp67));
			float fTemp68 = ((0.5f * (fTemp26 * fRec44[2])) + (0.25f * (fTemp27 * (fRec44[1] + fRec44[3]))));
			float fTemp69 = (3.14159274f * (fRec34[0] * fTemp68));
			float fTemp70 = sinf(fTemp69);
			float fTemp71 = cosf(fTemp69);
			fRec45[0] = ((fRec45[1] * (0.0f - fTemp70)) + (fTemp68 * fTemp71));
			float fTemp72 = (2.0f * fRec39[0]);
			float fTemp73 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp47 - fTemp72)))));
			float fTemp74 = (fRec36[0] * fTemp73);
			float fTemp75 = ((fTemp22 + (((fTemp68 * fTemp70) + (fRec45[1] * fTemp71)) * powf(0.00100000005f, (fSlow14 / fTemp74)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec13[(IOTA & 8191)] = fTemp75;
			float fTemp76 = (fConst37 / fTemp73);
			float fTemp77 = (fTemp76 + -2.49999499f);
			int iTemp78 = min(4096, max(0, int(fTemp77)));
			float fTemp79 = floorf(fTemp77);
			float fTemp80 = ((fVec13[((IOTA - iTemp78) & 8191)] - fVec13[((IOTA - (iTemp78 + 1)) & 8191)]) * (fTemp79 + (2.0f - fTemp76)));
			fVec14[0] = fTemp80;
			fRec44[0] = ((0.5f * ((fTemp80 - fVec14[1]) * (fTemp79 + (3.0f - fTemp76)))) + (fVec13[((IOTA - iTemp78) & 8191)] + fTemp80));
			float fTemp81 = ((0.5f * (fTemp26 * fRec46[2])) + (0.25f * (fTemp27 * (fRec46[1] + fRec46[3]))));
			float fTemp82 = (3.14159274f * (fRec34[0] * fTemp81));
			float fTemp83 = sinf(fTemp82);
			float fTemp84 = cosf(fTemp82);
			fRec47[0] = ((fRec47[1] * (0.0f - fTemp83)) + (fTemp81 * fTemp84));
			float fTemp85 = (3.0f * fRec39[0]);
			float fTemp86 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp47 - fTemp85)))));
			float fTemp87 = (fRec36[0] * fTemp86);
			float fTemp88 = ((fTemp22 + (((fTemp81 * fTemp83) + (fRec47[1] * fTemp84)) * powf(0.00100000005f, (fSlow14 / fTemp87)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec15[(IOTA & 8191)] = fTemp88;
			float fTemp89 = (fConst37 / fTemp86);
			float fTemp90 = (fTemp89 + -2.49999499f);
			int iTemp91 = min(4096, max(0, int(fTemp90)));
			float fTemp92 = floorf(fTemp90);
			float fTemp93 = ((fVec15[((IOTA - iTemp91) & 8191)] - fVec15[((IOTA - (iTemp91 + 1)) & 8191)]) * (fTemp92 + (2.0f - fTemp89)));
			fVec16[0] = fTemp93;
			fRec46[0] = ((0.5f * ((fTemp93 - fVec16[1]) * (fTemp92 + (3.0f - fTemp89)))) + (fVec15[((IOTA - iTemp91) & 8191)] + fTemp93));
			float fTemp94 = ((0.5f * (fTemp26 * fRec48[2])) + (0.25f * (fTemp27 * (fRec48[1] + fRec48[3]))));
			float fTemp95 = (3.14159274f * (fRec34[0] * fTemp94));
			float fTemp96 = sinf(fTemp95);
			float fTemp97 = cosf(fTemp95);
			fRec49[0] = ((fRec49[1] * (0.0f - fTemp96)) + (fTemp94 * fTemp97));
			float fTemp98 = (4.0f * fRec39[0]);
			float fTemp99 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp47 - fTemp98)))));
			float fTemp100 = (fRec36[0] * fTemp99);
			float fTemp101 = ((fTemp22 + (((fTemp94 * fTemp96) + (fRec49[1] * fTemp97)) * powf(0.00100000005f, (fSlow14 / fTemp100)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec17[(IOTA & 8191)] = fTemp101;
			float fTemp102 = (fConst37 / fTemp99);
			float fTemp103 = (fTemp102 + -2.49999499f);
			int iTemp104 = min(4096, max(0, int(fTemp103)));
			float fTemp105 = floorf(fTemp103);
			float fTemp106 = ((fVec17[((IOTA - iTemp104) & 8191)] - fVec17[((IOTA - (iTemp104 + 1)) & 8191)]) * (fTemp105 + (2.0f - fTemp102)));
			fVec18[0] = fTemp106;
			fRec48[0] = ((0.5f * ((fTemp106 - fVec18[1]) * (fTemp105 + (3.0f - fTemp102)))) + (fVec17[((IOTA - iTemp104) & 8191)] + fTemp106));
			float fTemp107 = ((0.5f * (fTemp26 * fRec50[2])) + (0.25f * (fTemp27 * (fRec50[1] + fRec50[3]))));
			float fTemp108 = (3.14159274f * (fRec34[0] * fTemp107));
			float fTemp109 = sinf(fTemp108);
			float fTemp110 = cosf(fTemp108);
			fRec51[0] = ((fRec51[1] * (0.0f - fTemp109)) + (fTemp107 * fTemp110));
			float fTemp111 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp33 + (fTemp98 + fTemp34))))));
			float fTemp112 = (fRec36[0] * fTemp111);
			float fTemp113 = ((fTemp22 + (((fTemp107 * fTemp109) + (fRec51[1] * fTemp110)) * powf(0.00100000005f, (fSlow14 / fTemp112)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec19[(IOTA & 8191)] = fTemp113;
			float fTemp114 = (fConst37 / fTemp111);
			float fTemp115 = (fTemp114 + -2.49999499f);
			int iTemp116 = min(4096, max(0, int(fTemp115)));
			float fTemp117 = floorf(fTemp115);
			float fTemp118 = ((fVec19[((IOTA - iTemp116) & 8191)] - fVec19[((IOTA - (iTemp116 + 1)) & 8191)]) * (fTemp117 + (2.0f - fTemp114)));
			fVec20[0] = fTemp118;
			fRec50[0] = ((0.5f * ((fTemp118 - fVec20[1]) * (fTemp117 + (3.0f - fTemp114)))) + (fVec19[((IOTA - iTemp116) & 8191)] + fTemp118));
			float fTemp119 = ((0.5f * (fTemp26 * fRec52[2])) + (0.25f * (fTemp27 * (fRec52[1] + fRec52[3]))));
			float fTemp120 = (3.14159274f * (fRec34[0] * fTemp119));
			float fTemp121 = sinf(fTemp120);
			float fTemp122 = cosf(fTemp120);
			fRec53[0] = ((fRec53[1] * (0.0f - fTemp121)) + (fTemp119 * fTemp122));
			float fTemp123 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp33 + (fTemp72 + fTemp34))))));
			float fTemp124 = (fRec36[0] * fTemp123);
			float fTemp125 = ((fTemp22 + (((fTemp119 * fTemp121) + (fRec53[1] * fTemp122)) * powf(0.00100000005f, (fSlow14 / fTemp124)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec21[(IOTA & 8191)] = fTemp125;
			float fTemp126 = (fConst37 / fTemp123);
			float fTemp127 = (fTemp126 + -2.49999499f);
			int iTemp128 = min(4096, max(0, int(fTemp127)));
			float fTemp129 = floorf(fTemp127);
			float fTemp130 = ((fVec21[((IOTA - iTemp128) & 8191)] - fVec21[((IOTA - (iTemp128 + 1)) & 8191)]) * (fTemp129 + (2.0f - fTemp126)));
			fVec22[0] = fTemp130;
			fRec52[0] = ((0.5f * ((fTemp130 - fVec22[1]) * (fTemp129 + (3.0f - fTemp126)))) + (fVec21[((IOTA - iTemp128) & 8191)] + fTemp130));
			float fTemp131 = ((0.5f * (fTemp26 * fRec54[2])) + (0.25f * (fTemp27 * (fRec54[1] + fRec54[3]))));
			float fTemp132 = (3.14159274f * (fRec34[0] * fTemp131));
			float fTemp133 = sinf(fTemp132);
			float fTemp134 = cosf(fTemp132);
			fRec55[0] = ((fRec55[1] * (0.0f - fTemp133)) + (fTemp131 * fTemp134));
			float fTemp135 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp33 + (fTemp85 + fTemp34))))));
			float fTemp136 = (fRec36[0] * fTemp135);
			float fTemp137 = ((fTemp22 + (((fTemp131 * fTemp133) + (fRec55[1] * fTemp134)) * powf(0.00100000005f, (fSlow14 / fTemp136)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec23[(IOTA & 8191)] = fTemp137;
			float fTemp138 = (fConst37 / fTemp135);
			float fTemp139 = (fTemp138 + -2.49999499f);
			int iTemp140 = min(4096, max(0, int(fTemp139)));
			float fTemp141 = floorf(fTemp139);
			float fTemp142 = ((fVec23[((IOTA - iTemp140) & 8191)] - fVec23[((IOTA - (iTemp140 + 1)) & 8191)]) * (fTemp141 + (2.0f - fTemp138)));
			fVec24[0] = fTemp142;
			fRec54[0] = ((0.5f * ((fTemp142 - fVec24[1]) * (fTemp141 + (3.0f - fTemp138)))) + (fVec23[((IOTA - iTemp140) & 8191)] + fTemp142));
			float fTemp143 = (fRec50[0] + (fRec52[0] + fRec54[0]));
			fVec25[0] = (fRec9[0] + (fRec40[0] + (fRec42[0] + (fRec44[0] + (fRec46[0] + (fRec48[0] + fTemp143))))));
			fRec8[0] = ((fRec9[0] + (fRec40[0] + (fRec42[0] + (fRec44[0] + (fRec46[0] + (fRec48[0] + ((0.995000005f * fRec8[1]) + fTemp143))))))) - fVec25[1]);
			float fRec0 = (0.00999999978f * fRec8[0]);
			float fTemp144 = (fRec32[0] * (fTemp21 - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]));
			float fTemp145 = (fTemp144 + 1.0f);
			float fTemp146 = (1.0f - fTemp144);
			float fTemp147 = ((0.5f * (fTemp145 * fRec57[2])) + (0.25f * (fTemp146 * (fRec57[1] + fRec57[3]))));
			float fTemp148 = (3.14159274f * (fRec34[0] * fTemp147));
			float fTemp149 = sinf(fTemp148);
			float fTemp150 = cosf(fTemp148);
			fRec58[0] = ((fRec58[1] * (0.0f - fTemp149)) + (fTemp147 * fTemp150));
			float fTemp151 = ((fTemp21 + (((fTemp147 * fTemp149) + (fRec58[1] * fTemp150)) * powf(0.00100000005f, (fSlow22 / fTemp100)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec26[(IOTA & 8191)] = fTemp151;
			float fTemp152 = (fSlow23 / fTemp99);
			float fTemp153 = (fTemp152 + -2.49999499f);
			int iTemp154 = min(4096, max(0, int(fTemp153)));
			float fTemp155 = floorf(fTemp153);
			float fTemp156 = ((fVec26[((IOTA - iTemp154) & 8191)] - fVec26[((IOTA - (iTemp154 + 1)) & 8191)]) * (fTemp155 + (2.0f - fTemp152)));
			fVec27[0] = fTemp156;
			fRec57[0] = ((0.5f * ((fTemp156 - fVec27[1]) * (fTemp155 + (3.0f - fTemp152)))) + (fVec26[((IOTA - iTemp154) & 8191)] + fTemp156));
			float fTemp157 = ((0.5f * (fTemp145 * fRec59[2])) + (0.25f * (fTemp146 * (fRec59[1] + fRec59[3]))));
			float fTemp158 = (3.14159274f * (fRec34[0] * fTemp157));
			float fTemp159 = sinf(fTemp158);
			float fTemp160 = cosf(fTemp158);
			fRec60[0] = ((fRec60[1] * (0.0f - fTemp159)) + (fTemp157 * fTemp160));
			float fTemp161 = ((fTemp21 + (((fTemp157 * fTemp159) + (fRec60[1] * fTemp160)) * powf(0.00100000005f, (fSlow22 / fTemp87)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec28[(IOTA & 8191)] = fTemp161;
			float fTemp162 = (fSlow23 / fTemp86);
			float fTemp163 = (fTemp162 + -2.49999499f);
			int iTemp164 = min(4096, max(0, int(fTemp163)));
			float fTemp165 = floorf(fTemp163);
			float fTemp166 = ((fVec28[((IOTA - iTemp164) & 8191)] - fVec28[((IOTA - (iTemp164 + 1)) & 8191)]) * (fTemp165 + (2.0f - fTemp162)));
			fVec29[0] = fTemp166;
			fRec59[0] = ((0.5f * ((fTemp166 - fVec29[1]) * (fTemp165 + (3.0f - fTemp162)))) + (fVec28[((IOTA - iTemp164) & 8191)] + fTemp166));
			float fTemp167 = ((0.5f * (fTemp145 * fRec61[2])) + (0.25f * (fTemp146 * (fRec61[1] + fRec61[3]))));
			float fTemp168 = (3.14159274f * (fRec34[0] * fTemp167));
			float fTemp169 = sinf(fTemp168);
			float fTemp170 = cosf(fTemp168);
			fRec62[0] = ((fRec62[1] * (0.0f - fTemp169)) + (fTemp167 * fTemp170));
			float fTemp171 = ((fTemp21 + (((fTemp167 * fTemp169) + (fRec62[1] * fTemp170)) * powf(0.00100000005f, (fSlow22 / fTemp74)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec30[(IOTA & 8191)] = fTemp171;
			float fTemp172 = (fSlow23 / fTemp73);
			float fTemp173 = (fTemp172 + -2.49999499f);
			int iTemp174 = min(4096, max(0, int(fTemp173)));
			float fTemp175 = floorf(fTemp173);
			float fTemp176 = ((fVec30[((IOTA - iTemp174) & 8191)] - fVec30[((IOTA - (iTemp174 + 1)) & 8191)]) * (fTemp175 + (2.0f - fTemp172)));
			fVec31[0] = fTemp176;
			fRec61[0] = ((0.5f * ((fTemp176 - fVec31[1]) * (fTemp175 + (3.0f - fTemp172)))) + (fVec30[((IOTA - iTemp174) & 8191)] + fTemp176));
			float fTemp177 = ((0.5f * (fTemp145 * fRec63[2])) + (0.25f * (fTemp146 * (fRec63[1] + fRec63[3]))));
			float fTemp178 = (3.14159274f * (fRec34[0] * fTemp177));
			float fTemp179 = sinf(fTemp178);
			float fTemp180 = cosf(fTemp178);
			fRec64[0] = ((fRec64[1] * (0.0f - fTemp179)) + (fTemp177 * fTemp180));
			float fTemp181 = ((fTemp21 + (((fTemp177 * fTemp179) + (fRec64[1] * fTemp180)) * powf(0.00100000005f, (fSlow22 / fTemp61)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec32[(IOTA & 8191)] = fTemp181;
			float fTemp182 = (fSlow23 / fTemp60);
			float fTemp183 = (fTemp182 + -2.49999499f);
			int iTemp184 = min(4096, max(0, int(fTemp183)));
			float fTemp185 = floorf(fTemp183);
			float fTemp186 = ((fVec32[((IOTA - iTemp184) & 8191)] - fVec32[((IOTA - (iTemp184 + 1)) & 8191)]) * (fTemp185 + (2.0f - fTemp182)));
			fVec33[0] = fTemp186;
			fRec63[0] = ((0.5f * ((fTemp186 - fVec33[1]) * (fTemp185 + (3.0f - fTemp182)))) + (fVec32[((IOTA - iTemp184) & 8191)] + fTemp186));
			float fTemp187 = ((0.5f * (fTemp145 * fRec65[2])) + (0.25f * (fTemp146 * (fRec65[1] + fRec65[3]))));
			float fTemp188 = (3.14159274f * (fRec34[0] * fTemp187));
			float fTemp189 = sinf(fTemp188);
			float fTemp190 = cosf(fTemp188);
			fRec66[0] = ((fRec66[1] * (0.0f - fTemp189)) + (fTemp187 * fTemp190));
			float fTemp191 = ((fTemp21 + (((fTemp187 * fTemp189) + (fRec66[1] * fTemp190)) * powf(0.00100000005f, (fSlow22 / fTemp49)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec34[(IOTA & 8191)] = fTemp191;
			float fTemp192 = (fSlow23 / fTemp48);
			float fTemp193 = (fTemp192 + -2.49999499f);
			int iTemp194 = min(4096, max(0, int(fTemp193)));
			float fTemp195 = floorf(fTemp193);
			float fTemp196 = ((fVec34[((IOTA - iTemp194) & 8191)] - fVec34[((IOTA - (iTemp194 + 1)) & 8191)]) * (fTemp195 + (2.0f - fTemp192)));
			fVec35[0] = fTemp196;
			fRec65[0] = ((0.5f * ((fTemp196 - fVec35[1]) * (fTemp195 + (3.0f - fTemp192)))) + (fVec34[((IOTA - iTemp194) & 8191)] + fTemp196));
			float fTemp197 = ((0.5f * (fTemp145 * fRec67[2])) + (0.25f * (fTemp146 * (fRec67[1] + fRec67[3]))));
			float fTemp198 = (3.14159274f * (fRec34[0] * fTemp197));
			float fTemp199 = sinf(fTemp198);
			float fTemp200 = cosf(fTemp198);
			fRec68[0] = ((fRec68[1] * (0.0f - fTemp199)) + (fTemp197 * fTemp200));
			float fTemp201 = ((fTemp21 + (((fTemp197 * fTemp199) + (fRec68[1] * fTemp200)) * powf(0.00100000005f, (fSlow22 / fTemp36)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec36[(IOTA & 8191)] = fTemp201;
			float fTemp202 = (fSlow23 / fTemp35);
			float fTemp203 = (fTemp202 + -2.49999499f);
			int iTemp204 = min(4096, max(0, int(fTemp203)));
			float fTemp205 = floorf(fTemp203);
			float fTemp206 = ((fVec36[((IOTA - iTemp204) & 8191)] - fVec36[((IOTA - (iTemp204 + 1)) & 8191)]) * (fTemp205 + (2.0f - fTemp202)));
			fVec37[0] = fTemp206;
			fRec67[0] = ((0.5f * ((fTemp206 - fVec37[1]) * (fTemp205 + (3.0f - fTemp202)))) + (fVec36[((IOTA - iTemp204) & 8191)] + fTemp206));
			float fTemp207 = ((0.5f * (fTemp145 * fRec69[2])) + (0.25f * (fTemp146 * (fRec69[1] + fRec69[3]))));
			float fTemp208 = (3.14159274f * (fRec34[0] * fTemp207));
			float fTemp209 = sinf(fTemp208);
			float fTemp210 = cosf(fTemp208);
			fRec70[0] = ((fRec70[1] * (0.0f - fTemp209)) + (fTemp207 * fTemp210));
			float fTemp211 = ((fTemp21 + (((fTemp207 * fTemp209) + (fRec70[1] * fTemp210)) * powf(0.00100000005f, (fSlow22 / fTemp124)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec38[(IOTA & 8191)] = fTemp211;
			float fTemp212 = (fSlow23 / fTemp123);
			float fTemp213 = (fTemp212 + -2.49999499f);
			int iTemp214 = min(4096, max(0, int(fTemp213)));
			float fTemp215 = floorf(fTemp213);
			float fTemp216 = ((fVec38[((IOTA - iTemp214) & 8191)] - fVec38[((IOTA - (iTemp214 + 1)) & 8191)]) * (fTemp215 + (2.0f - fTemp212)));
			fVec39[0] = fTemp216;
			fRec69[0] = ((0.5f * ((fTemp216 - fVec39[1]) * (fTemp215 + (3.0f - fTemp212)))) + (fVec38[((IOTA - iTemp214) & 8191)] + fTemp216));
			float fTemp217 = ((0.5f * (fTemp145 * fRec71[2])) + (0.25f * (fTemp146 * (fRec71[1] + fRec71[3]))));
			float fTemp218 = (3.14159274f * (fRec34[0] * fTemp217));
			float fTemp219 = sinf(fTemp218);
			float fTemp220 = cosf(fTemp218);
			fRec72[0] = ((fRec72[1] * (0.0f - fTemp219)) + (fTemp217 * fTemp220));
			float fTemp221 = ((fTemp21 + (((fTemp217 * fTemp219) + (fRec72[1] * fTemp220)) * powf(0.00100000005f, (fSlow22 / fTemp136)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec40[(IOTA & 8191)] = fTemp221;
			float fTemp222 = (fSlow23 / fTemp135);
			float fTemp223 = (fTemp222 + -2.49999499f);
			int iTemp224 = min(4096, max(0, int(fTemp223)));
			float fTemp225 = floorf(fTemp223);
			float fTemp226 = ((fVec40[((IOTA - iTemp224) & 8191)] - fVec40[((IOTA - (iTemp224 + 1)) & 8191)]) * (fTemp225 + (2.0f - fTemp222)));
			fVec41[0] = fTemp226;
			fRec71[0] = ((0.5f * ((fTemp226 - fVec41[1]) * (fTemp225 + (3.0f - fTemp222)))) + (fVec40[((IOTA - iTemp224) & 8191)] + fTemp226));
			float fTemp227 = ((0.5f * (fTemp145 * fRec73[2])) + (0.25f * (fTemp146 * (fRec73[1] + fRec73[3]))));
			float fTemp228 = (3.14159274f * (fRec34[0] * fTemp227));
			float fTemp229 = sinf(fTemp228);
			float fTemp230 = cosf(fTemp228);
			fRec74[0] = ((fRec74[1] * (0.0f - fTemp229)) + (fTemp227 * fTemp230));
			float fTemp231 = ((fTemp21 + (((fTemp227 * fTemp229) + (fRec74[1] * fTemp230)) * powf(0.00100000005f, (fSlow22 / fTemp112)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp16)))))) & 8191)]);
			fVec42[(IOTA & 8191)] = fTemp231;
			float fTemp232 = (fSlow23 / fTemp111);
			float fTemp233 = (fTemp232 + -2.49999499f);
			int iTemp234 = min(4096, max(0, int(fTemp233)));
			float fTemp235 = floorf(fTemp233);
			float fTemp236 = ((fVec42[((IOTA - iTemp234) & 8191)] - fVec42[((IOTA - (iTemp234 + 1)) & 8191)]) * (fTemp235 + (2.0f - fTemp232)));
			fVec43[0] = fTemp236;
			fRec73[0] = ((fTemp236 + (0.5f * ((fTemp236 - fVec43[1]) * (fTemp235 + (3.0f - fTemp232))))) + fVec42[((IOTA - iTemp234) & 8191)]);
			float fTemp237 = ((((((((fRec57[0] + fRec59[0]) + fRec61[0]) + fRec63[0]) + fRec65[0]) + fRec67[0]) + fRec69[0]) + fRec71[0]) + fRec73[0]);
			fVec44[0] = fTemp237;
			fRec56[0] = ((fTemp237 + (0.995000005f * fRec56[1])) - fVec44[1]);
			fRec1[0] = (0.00999999978f * fRec56[0]);
			float fTemp238 = (fRec1[1] * fRec13[0]);
			fVec45[0] = fTemp238;
			fRec79[0] = ((fConst16 * fRec79[1]) + (fConst17 * (fTemp238 + fVec45[1])));
			fRec80[0] = ((fConst16 * fRec80[1]) + (fConst17 * ((fConst18 * fVec45[1]) + (fConst14 * fTemp238))));
			float fTemp239 = (fConst13 * fRec78[1]);
			fRec78[0] = ((fRec79[0] + (9.99999975e-06f * fRec80[0])) - (fConst12 * (fTemp239 + (fConst21 * fRec78[2]))));
			float fTemp240 = (fConst7 * fRec77[1]);
			fRec77[0] = ((fConst12 * ((fTemp239 + (fConst23 * fRec78[0])) + (fConst24 * fRec78[2]))) - (fConst6 * (fTemp240 + (fConst25 * fRec77[2]))));
			float fTemp241 = ((fTemp240 + (fConst27 * fRec77[0])) + (fConst28 * fRec77[2]));
			float fTemp242 = (fConst6 * (fTemp3 + (fTemp241 + fTemp11)));
			float fTemp243 = (fSlow2 * float(((fRec30[0] > 0.25f) & (fRec30[0] < 0.5f))));
			float fTemp244 = ((1.0f - fTemp243) - fSlow24);
			fVec46[0] = fTemp244;
			float fTemp245 = ((fTemp244 != fVec46[1])?(fSlow25 / fTemp16):(fRec83[1] + -1.0f));
			fRec83[0] = fTemp245;
			fRec84[0] = ((fTemp245 <= 0.0f)?fTemp244:(fRec84[1] + (((1.0f - (fTemp243 + fRec84[1])) - fSlow24) / fTemp245)));
			float fTemp246 = (fSlow24 + fTemp243);
			fVec47[0] = fTemp246;
			float fTemp247 = (fRec84[0] * fTemp246);
			float fTemp248 = ((fRec81[1] > fTemp247)?fConst32:expf((0.0f - (fSlow26 * fTemp16))));
			fRec82[0] = ((fRec82[1] * fTemp248) + (fTemp247 * (1.0f - fTemp248)));
			fRec81[0] = fRec82[0];
			float fTemp249 = (fTemp13 * fRec81[0]);
			fVec48[(IOTA & 8191)] = fTemp249;
			float fTemp250 = (fTemp242 + fTemp249);
			int iTemp251 = ((fVec47[1] <= 0.0f) & (fTemp246 > 0.0f));
			fRec85[0] = ((fRec85[1] * float((1 - iTemp251))) + (float(iTemp251) * fTemp24));
			float fTemp252 = (fRec32[0] * (fTemp250 - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]));
			float fTemp253 = (fTemp252 + 1.0f);
			float fTemp254 = (1.0f - fTemp252);
			float fTemp255 = ((0.5f * (fTemp253 * fRec76[2])) + (0.25f * (fTemp254 * (fRec76[1] + fRec76[3]))));
			float fTemp256 = (3.14159274f * (fRec34[0] * fTemp255));
			float fTemp257 = sinf(fTemp256);
			float fTemp258 = cosf(fTemp256);
			fRec86[0] = ((fRec86[1] * (0.0f - fTemp257)) + (fTemp255 * fTemp258));
			float fTemp259 = (880.0f * fTemp16);
			fVec49[0] = fSlow28;
			float fTemp260 = ((fSlow28 != fVec49[1])?fSlow19:(fRec87[1] + -1.0f));
			fRec87[0] = fTemp260;
			fRec88[0] = ((fTemp260 <= 0.0f)?fSlow28:(fRec88[1] + (((1.0f - fRec88[1]) - fSlow24) / fTemp260)));
			float fTemp261 = (fSlow27 * tambura_faustpower2_f(fRec88[0]));
			float fTemp262 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp259 + (fTemp98 + fTemp261))))));
			float fTemp263 = (fRec36[0] * fTemp262);
			float fTemp264 = ((fTemp250 + (((fTemp255 * fTemp257) + (fRec86[1] * fTemp258)) * powf(0.00100000005f, (fSlow14 / fTemp263)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec50[(IOTA & 8191)] = fTemp264;
			float fTemp265 = (fConst37 / fTemp262);
			float fTemp266 = (fTemp265 + -2.49999499f);
			int iTemp267 = min(4096, max(0, int(fTemp266)));
			float fTemp268 = floorf(fTemp266);
			float fTemp269 = ((fVec50[((IOTA - iTemp267) & 8191)] - fVec50[((IOTA - (iTemp267 + 1)) & 8191)]) * (fTemp268 + (2.0f - fTemp265)));
			fVec51[0] = fTemp269;
			fRec76[0] = ((0.5f * ((fTemp269 - fVec51[1]) * (fTemp268 + (3.0f - fTemp265)))) + (fVec50[((IOTA - iTemp267) & 8191)] + fTemp269));
			float fTemp270 = ((0.5f * (fTemp253 * fRec89[2])) + (0.25f * (fTemp254 * (fRec89[1] + fRec89[3]))));
			float fTemp271 = (3.14159274f * (fRec34[0] * fTemp270));
			float fTemp272 = sinf(fTemp271);
			float fTemp273 = cosf(fTemp271);
			fRec90[0] = ((fRec90[1] * (0.0f - fTemp272)) + (fTemp270 * fTemp273));
			float fTemp274 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp259 + (fTemp85 + fTemp261))))));
			float fTemp275 = (fRec36[0] * fTemp274);
			float fTemp276 = ((fTemp250 + (((fTemp270 * fTemp272) + (fRec90[1] * fTemp273)) * powf(0.00100000005f, (fSlow14 / fTemp275)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec52[(IOTA & 8191)] = fTemp276;
			float fTemp277 = (fConst37 / fTemp274);
			float fTemp278 = (fTemp277 + -2.49999499f);
			int iTemp279 = min(4096, max(0, int(fTemp278)));
			float fTemp280 = floorf(fTemp278);
			float fTemp281 = ((fVec52[((IOTA - iTemp279) & 8191)] - fVec52[((IOTA - (iTemp279 + 1)) & 8191)]) * (fTemp280 + (2.0f - fTemp277)));
			fVec53[0] = fTemp281;
			fRec89[0] = ((0.5f * ((fTemp281 - fVec53[1]) * (fTemp280 + (3.0f - fTemp277)))) + (fVec52[((IOTA - iTemp279) & 8191)] + fTemp281));
			float fTemp282 = ((0.5f * (fTemp253 * fRec91[2])) + (0.25f * (fTemp254 * (fRec91[1] + fRec91[3]))));
			float fTemp283 = (3.14159274f * (fRec34[0] * fTemp282));
			float fTemp284 = sinf(fTemp283);
			float fTemp285 = cosf(fTemp283);
			fRec92[0] = ((fRec92[1] * (0.0f - fTemp284)) + (fTemp282 * fTemp285));
			float fTemp286 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp259 + (fTemp72 + fTemp261))))));
			float fTemp287 = (fRec36[0] * fTemp286);
			float fTemp288 = ((fTemp250 + (((fTemp282 * fTemp284) + (fRec92[1] * fTemp285)) * powf(0.00100000005f, (fSlow14 / fTemp287)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec54[(IOTA & 8191)] = fTemp288;
			float fTemp289 = (fConst37 / fTemp286);
			float fTemp290 = (fTemp289 + -2.49999499f);
			int iTemp291 = min(4096, max(0, int(fTemp290)));
			float fTemp292 = floorf(fTemp290);
			float fTemp293 = ((fVec54[((IOTA - iTemp291) & 8191)] - fVec54[((IOTA - (iTemp291 + 1)) & 8191)]) * (fTemp292 + (2.0f - fTemp289)));
			fVec55[0] = fTemp293;
			fRec91[0] = ((0.5f * ((fTemp293 - fVec55[1]) * (fTemp292 + (3.0f - fTemp289)))) + (fVec54[((IOTA - iTemp291) & 8191)] + fTemp293));
			float fTemp294 = ((0.5f * (fRec93[2] * fTemp253)) + (0.25f * ((fRec93[1] + fRec93[3]) * fTemp254)));
			float fTemp295 = (3.14159274f * (fTemp294 * fRec34[0]));
			float fTemp296 = sinf(fTemp295);
			float fTemp297 = cosf(fTemp295);
			fRec94[0] = ((fRec94[1] * (0.0f - fTemp296)) + (fTemp294 * fTemp297));
			float fTemp298 = powf(2.0f, (1.44269502f * logf((0.0022727272f * ((fRec39[0] + fTemp261) + fTemp259)))));
			float fTemp299 = (fTemp298 * fRec36[0]);
			float fTemp300 = ((((((fTemp294 * fTemp296) + (fRec94[1] * fTemp297)) * powf(0.00100000005f, (fSlow14 / fTemp299))) + fTemp242) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec56[(IOTA & 8191)] = fTemp300;
			float fTemp301 = (fConst37 / fTemp298);
			float fTemp302 = (fTemp301 + -2.49999499f);
			int iTemp303 = min(4096, max(0, int(fTemp302)));
			float fTemp304 = floorf(fTemp302);
			float fTemp305 = ((fVec56[((IOTA - iTemp303) & 8191)] - fVec56[((IOTA - (iTemp303 + 1)) & 8191)]) * (fTemp304 + (2.0f - fTemp301)));
			fVec57[0] = fTemp305;
			fRec93[0] = ((0.5f * ((fTemp305 - fVec57[1]) * (fTemp304 + (3.0f - fTemp301)))) + (fVec56[((IOTA - iTemp303) & 8191)] + fTemp305));
			float fTemp306 = ((0.5f * (fRec95[2] * fTemp253)) + (0.25f * ((fRec95[1] + fRec95[3]) * fTemp254)));
			float fTemp307 = (3.14159274f * (fTemp306 * fRec34[0]));
			float fTemp308 = sinf(fTemp307);
			float fTemp309 = cosf(fTemp307);
			fRec96[0] = ((fRec96[1] * (0.0f - fTemp308)) + (fTemp306 * fTemp309));
			float fTemp310 = (fTemp259 + fTemp261);
			float fTemp311 = powf(2.0f, (1.44269502f * logf((0.0022727272f * fTemp310))));
			float fTemp312 = (fTemp311 * fRec36[0]);
			float fTemp313 = ((((((fTemp306 * fTemp308) + (fRec96[1] * fTemp309)) * powf(0.00100000005f, (fSlow14 / fTemp312))) + fTemp242) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec58[(IOTA & 8191)] = fTemp313;
			float fTemp314 = (fConst37 / fTemp311);
			float fTemp315 = (fTemp314 + -2.49999499f);
			int iTemp316 = min(4096, max(0, int(fTemp315)));
			float fTemp317 = floorf(fTemp315);
			float fTemp318 = ((fVec58[((IOTA - iTemp316) & 8191)] - fVec58[((IOTA - (iTemp316 + 1)) & 8191)]) * (fTemp317 + (2.0f - fTemp314)));
			fVec59[0] = fTemp318;
			fRec95[0] = ((0.5f * ((fTemp318 - fVec59[1]) * (fTemp317 + (3.0f - fTemp314)))) + (fVec58[((IOTA - iTemp316) & 8191)] + fTemp318));
			float fTemp319 = ((0.5f * (fRec97[2] * fTemp253)) + (0.25f * ((fRec97[1] + fRec97[3]) * fTemp254)));
			float fTemp320 = (3.14159274f * (fTemp319 * fRec34[0]));
			float fTemp321 = sinf(fTemp320);
			float fTemp322 = cosf(fTemp320);
			fRec98[0] = ((fRec98[1] * (0.0f - fTemp321)) + (fTemp319 * fTemp322));
			float fTemp323 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp310 - fRec39[0])))));
			float fTemp324 = (fTemp323 * fRec36[0]);
			float fTemp325 = ((((((fTemp319 * fTemp321) + (fRec98[1] * fTemp322)) * powf(0.00100000005f, (fSlow14 / fTemp324))) + fTemp242) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec60[(IOTA & 8191)] = fTemp325;
			float fTemp326 = (fConst37 / fTemp323);
			float fTemp327 = (fTemp326 + -2.49999499f);
			int iTemp328 = min(4096, max(0, int(fTemp327)));
			float fTemp329 = floorf(fTemp327);
			float fTemp330 = ((fVec60[((IOTA - iTemp328) & 8191)] - fVec60[((IOTA - (iTemp328 + 1)) & 8191)]) * (fTemp329 + (2.0f - fTemp326)));
			fVec61[0] = fTemp330;
			fRec97[0] = ((0.5f * ((fTemp330 - fVec61[1]) * (fTemp329 + (3.0f - fTemp326)))) + (fVec60[((IOTA - iTemp328) & 8191)] + fTemp330));
			float fTemp331 = ((0.5f * (fRec99[2] * fTemp253)) + (0.25f * ((fRec99[1] + fRec99[3]) * fTemp254)));
			float fTemp332 = (3.14159274f * (fTemp331 * fRec34[0]));
			float fTemp333 = sinf(fTemp332);
			float fTemp334 = cosf(fTemp332);
			fRec100[0] = ((fRec100[1] * (0.0f - fTemp333)) + (fTemp331 * fTemp334));
			float fTemp335 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp310 - fTemp72)))));
			float fTemp336 = (fTemp335 * fRec36[0]);
			float fTemp337 = ((((((fTemp331 * fTemp333) + (fRec100[1] * fTemp334)) * powf(0.00100000005f, (fSlow14 / fTemp336))) + fTemp242) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec62[(IOTA & 8191)] = fTemp337;
			float fTemp338 = (fConst37 / fTemp335);
			float fTemp339 = (fTemp338 + -2.49999499f);
			int iTemp340 = min(4096, max(0, int(fTemp339)));
			float fTemp341 = floorf(fTemp339);
			float fTemp342 = ((fVec62[((IOTA - iTemp340) & 8191)] - fVec62[((IOTA - (iTemp340 + 1)) & 8191)]) * (fTemp341 + (2.0f - fTemp338)));
			fVec63[0] = fTemp342;
			fRec99[0] = ((0.5f * ((fTemp342 - fVec63[1]) * (fTemp341 + (3.0f - fTemp338)))) + (fVec62[((IOTA - iTemp340) & 8191)] + fTemp342));
			float fTemp343 = ((0.5f * (fRec101[2] * fTemp253)) + (0.25f * ((fRec101[1] + fRec101[3]) * fTemp254)));
			float fTemp344 = (3.14159274f * (fTemp343 * fRec34[0]));
			float fTemp345 = sinf(fTemp344);
			float fTemp346 = cosf(fTemp344);
			fRec102[0] = ((fRec102[1] * (0.0f - fTemp345)) + (fTemp343 * fTemp346));
			float fTemp347 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp310 - fTemp85)))));
			float fTemp348 = (fTemp347 * fRec36[0]);
			float fTemp349 = ((((((fTemp343 * fTemp345) + (fRec102[1] * fTemp346)) * powf(0.00100000005f, (fSlow14 / fTemp348))) + fTemp242) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec64[(IOTA & 8191)] = fTemp349;
			float fTemp350 = (fConst37 / fTemp347);
			float fTemp351 = (fTemp350 + -2.49999499f);
			int iTemp352 = min(4096, max(0, int(fTemp351)));
			float fTemp353 = floorf(fTemp351);
			float fTemp354 = ((fVec64[((IOTA - iTemp352) & 8191)] - fVec64[((IOTA - (iTemp352 + 1)) & 8191)]) * (fTemp353 + (2.0f - fTemp350)));
			fVec65[0] = fTemp354;
			fRec101[0] = ((0.5f * ((fTemp354 - fVec65[1]) * (fTemp353 + (3.0f - fTemp350)))) + (fVec64[((IOTA - iTemp352) & 8191)] + fTemp354));
			float fTemp355 = ((0.5f * (fTemp253 * fRec103[2])) + (0.25f * (fTemp254 * (fRec103[1] + fRec103[3]))));
			float fTemp356 = (3.14159274f * (fRec34[0] * fTemp355));
			float fTemp357 = sinf(fTemp356);
			float fTemp358 = cosf(fTemp356);
			fRec104[0] = ((fRec104[1] * (0.0f - fTemp357)) + (fTemp355 * fTemp358));
			float fTemp359 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp310 - fTemp98)))));
			float fTemp360 = (fRec36[0] * fTemp359);
			float fTemp361 = ((fTemp250 + (((fTemp355 * fTemp357) + (fRec104[1] * fTemp358)) * powf(0.00100000005f, (fSlow14 / fTemp360)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec66[(IOTA & 8191)] = fTemp361;
			float fTemp362 = (fConst37 / fTemp359);
			float fTemp363 = (fTemp362 + -2.49999499f);
			int iTemp364 = min(4096, max(0, int(fTemp363)));
			float fTemp365 = floorf(fTemp363);
			float fTemp366 = ((fVec66[((IOTA - iTemp364) & 8191)] - fVec66[((IOTA - (iTemp364 + 1)) & 8191)]) * (fTemp365 + (2.0f - fTemp362)));
			fVec67[0] = fTemp366;
			fRec103[0] = ((0.5f * ((fTemp366 - fVec67[1]) * (fTemp365 + (3.0f - fTemp362)))) + (fVec66[((IOTA - iTemp364) & 8191)] + fTemp366));
			fVec68[0] = ((((((((fRec103[0] + fRec101[0]) + fRec99[0]) + fRec97[0]) + fRec95[0]) + fRec93[0]) + fRec91[0]) + fRec89[0]) + fRec76[0]);
			fRec75[0] = ((fRec76[0] + (fRec89[0] + (fRec91[0] + (fRec93[0] + (fRec95[0] + (fRec97[0] + (fRec99[0] + (fRec101[0] + ((0.995000005f * fRec75[1]) + fRec103[0]))))))))) - fVec68[1]);
			float fRec2 = (0.00999999978f * fRec75[0]);
			float fTemp367 = (fRec32[0] * (fTemp249 - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]));
			float fTemp368 = (fTemp367 + 1.0f);
			float fTemp369 = (1.0f - fTemp367);
			float fTemp370 = ((0.5f * (fTemp368 * fRec106[2])) + (0.25f * (fTemp369 * (fRec106[1] + fRec106[3]))));
			float fTemp371 = (3.14159274f * (fRec34[0] * fTemp370));
			float fTemp372 = sinf(fTemp371);
			float fTemp373 = cosf(fTemp371);
			fRec107[0] = ((fRec107[1] * (0.0f - fTemp372)) + (fTemp370 * fTemp373));
			float fTemp374 = ((fTemp249 + (((fTemp370 * fTemp372) + (fRec107[1] * fTemp373)) * powf(0.00100000005f, (fSlow22 / fTemp360)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec69[(IOTA & 8191)] = fTemp374;
			float fTemp375 = (fSlow23 / fTemp359);
			float fTemp376 = (fTemp375 + -2.49999499f);
			int iTemp377 = min(4096, max(0, int(fTemp376)));
			float fTemp378 = floorf(fTemp376);
			float fTemp379 = ((fVec69[((IOTA - iTemp377) & 8191)] - fVec69[((IOTA - (iTemp377 + 1)) & 8191)]) * (fTemp378 + (2.0f - fTemp375)));
			fVec70[0] = fTemp379;
			fRec106[0] = (((0.5f * ((fTemp379 - fVec70[1]) * (fTemp378 + (3.0f - fTemp375)))) + fVec69[((IOTA - iTemp377) & 8191)]) + fTemp379);
			float fTemp380 = ((0.5f * (fRec108[2] * fTemp368)) + (0.25f * ((fRec108[1] + fRec108[3]) * fTemp369)));
			float fTemp381 = (3.14159274f * (fTemp380 * fRec34[0]));
			float fTemp382 = sinf(fTemp381);
			float fTemp383 = cosf(fTemp381);
			fRec109[0] = ((fRec109[1] * (0.0f - fTemp382)) + (fTemp380 * fTemp383));
			float fTemp384 = (((((fTemp380 * fTemp382) + (fRec109[1] * fTemp383)) * powf(0.00100000005f, (fSlow22 / fTemp348))) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec71[(IOTA & 8191)] = fTemp384;
			float fTemp385 = (fSlow23 / fTemp347);
			float fTemp386 = (fTemp385 + -2.49999499f);
			int iTemp387 = min(4096, max(0, int(fTemp386)));
			float fTemp388 = floorf(fTemp386);
			float fTemp389 = ((fVec71[((IOTA - iTemp387) & 8191)] - fVec71[((IOTA - (iTemp387 + 1)) & 8191)]) * (fTemp388 + (2.0f - fTemp385)));
			fVec72[0] = fTemp389;
			fRec108[0] = ((0.5f * ((fTemp389 - fVec72[1]) * (fTemp388 + (3.0f - fTemp385)))) + (fVec71[((IOTA - iTemp387) & 8191)] + fTemp389));
			float fTemp390 = ((0.5f * (fRec110[2] * fTemp368)) + (0.25f * ((fRec110[1] + fRec110[3]) * fTemp369)));
			float fTemp391 = (3.14159274f * (fTemp390 * fRec34[0]));
			float fTemp392 = sinf(fTemp391);
			float fTemp393 = cosf(fTemp391);
			fRec111[0] = ((fRec111[1] * (0.0f - fTemp392)) + (fTemp390 * fTemp393));
			float fTemp394 = (((((fTemp390 * fTemp392) + (fRec111[1] * fTemp393)) * powf(0.00100000005f, (fSlow22 / fTemp336))) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec73[(IOTA & 8191)] = fTemp394;
			float fTemp395 = (fSlow23 / fTemp335);
			float fTemp396 = (fTemp395 + -2.49999499f);
			int iTemp397 = min(4096, max(0, int(fTemp396)));
			float fTemp398 = floorf(fTemp396);
			float fTemp399 = ((fVec73[((IOTA - iTemp397) & 8191)] - fVec73[((IOTA - (iTemp397 + 1)) & 8191)]) * (fTemp398 + (2.0f - fTemp395)));
			fVec74[0] = fTemp399;
			fRec110[0] = ((0.5f * ((fTemp399 - fVec74[1]) * (fTemp398 + (3.0f - fTemp395)))) + (fVec73[((IOTA - iTemp397) & 8191)] + fTemp399));
			float fTemp400 = ((0.5f * (fRec112[2] * fTemp368)) + (0.25f * ((fRec112[1] + fRec112[3]) * fTemp369)));
			float fTemp401 = (3.14159274f * (fTemp400 * fRec34[0]));
			float fTemp402 = sinf(fTemp401);
			float fTemp403 = cosf(fTemp401);
			fRec113[0] = ((fRec113[1] * (0.0f - fTemp402)) + (fTemp400 * fTemp403));
			float fTemp404 = (((((fTemp400 * fTemp402) + (fRec113[1] * fTemp403)) * powf(0.00100000005f, (fSlow22 / fTemp324))) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec75[(IOTA & 8191)] = fTemp404;
			float fTemp405 = (fSlow23 / fTemp323);
			float fTemp406 = (fTemp405 + -2.49999499f);
			int iTemp407 = min(4096, max(0, int(fTemp406)));
			float fTemp408 = floorf(fTemp406);
			float fTemp409 = ((fVec75[((IOTA - iTemp407) & 8191)] - fVec75[((IOTA - (iTemp407 + 1)) & 8191)]) * (fTemp408 + (2.0f - fTemp405)));
			fVec76[0] = fTemp409;
			fRec112[0] = ((0.5f * ((fTemp409 - fVec76[1]) * (fTemp408 + (3.0f - fTemp405)))) + (fVec75[((IOTA - iTemp407) & 8191)] + fTemp409));
			float fTemp410 = ((0.5f * (fRec114[2] * fTemp368)) + (0.25f * ((fRec114[1] + fRec114[3]) * fTemp369)));
			float fTemp411 = (3.14159274f * (fTemp410 * fRec34[0]));
			float fTemp412 = sinf(fTemp411);
			float fTemp413 = cosf(fTemp411);
			fRec115[0] = ((fRec115[1] * (0.0f - fTemp412)) + (fTemp410 * fTemp413));
			float fTemp414 = (((((fTemp410 * fTemp412) + (fRec115[1] * fTemp413)) * powf(0.00100000005f, (fSlow22 / fTemp312))) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec77[(IOTA & 8191)] = fTemp414;
			float fTemp415 = (fSlow23 / fTemp311);
			float fTemp416 = (fTemp415 + -2.49999499f);
			int iTemp417 = min(4096, max(0, int(fTemp416)));
			float fTemp418 = floorf(fTemp416);
			float fTemp419 = ((fVec77[((IOTA - iTemp417) & 8191)] - fVec77[((IOTA - (iTemp417 + 1)) & 8191)]) * (fTemp418 + (2.0f - fTemp415)));
			fVec78[0] = fTemp419;
			fRec114[0] = ((0.5f * ((fTemp419 - fVec78[1]) * (fTemp418 + (3.0f - fTemp415)))) + (fVec77[((IOTA - iTemp417) & 8191)] + fTemp419));
			float fTemp420 = ((0.5f * (fTemp368 * fRec116[2])) + (0.25f * (fTemp369 * (fRec116[1] + fRec116[3]))));
			float fTemp421 = (3.14159274f * (fRec34[0] * fTemp420));
			float fTemp422 = sinf(fTemp421);
			float fTemp423 = cosf(fTemp421);
			fRec117[0] = ((fRec117[1] * (0.0f - fTemp422)) + (fTemp420 * fTemp423));
			float fTemp424 = ((fTemp249 + (((fTemp420 * fTemp422) + (fRec117[1] * fTemp423)) * powf(0.00100000005f, (fSlow22 / fTemp299)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec79[(IOTA & 8191)] = fTemp424;
			float fTemp425 = (fSlow23 / fTemp298);
			float fTemp426 = (fTemp425 + -2.49999499f);
			int iTemp427 = min(4096, max(0, int(fTemp426)));
			float fTemp428 = floorf(fTemp426);
			float fTemp429 = ((fVec79[((IOTA - iTemp427) & 8191)] - fVec79[((IOTA - (iTemp427 + 1)) & 8191)]) * (fTemp428 + (2.0f - fTemp425)));
			fVec80[0] = fTemp429;
			fRec116[0] = ((0.5f * ((fTemp429 - fVec80[1]) * (fTemp428 + (3.0f - fTemp425)))) + (fVec79[((IOTA - iTemp427) & 8191)] + fTemp429));
			float fTemp430 = ((0.5f * (fRec118[2] * fTemp368)) + (0.25f * ((fRec118[1] + fRec118[3]) * fTemp369)));
			float fTemp431 = (3.14159274f * (fTemp430 * fRec34[0]));
			float fTemp432 = sinf(fTemp431);
			float fTemp433 = cosf(fTemp431);
			fRec119[0] = ((fRec119[1] * (0.0f - fTemp432)) + (fTemp430 * fTemp433));
			float fTemp434 = (((((fTemp430 * fTemp432) + (fRec119[1] * fTemp433)) * powf(0.00100000005f, (fSlow22 / fTemp287))) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec81[(IOTA & 8191)] = fTemp434;
			float fTemp435 = (fSlow23 / fTemp286);
			float fTemp436 = (fTemp435 + -2.49999499f);
			int iTemp437 = min(4096, max(0, int(fTemp436)));
			float fTemp438 = floorf(fTemp436);
			float fTemp439 = ((fVec81[((IOTA - iTemp437) & 8191)] - fVec81[((IOTA - (iTemp437 + 1)) & 8191)]) * (fTemp438 + (2.0f - fTemp435)));
			fVec82[0] = fTemp439;
			fRec118[0] = ((0.5f * ((fTemp439 - fVec82[1]) * (fTemp438 + (3.0f - fTemp435)))) + (fVec81[((IOTA - iTemp437) & 8191)] + fTemp439));
			float fTemp440 = ((0.5f * (fRec120[2] * fTemp368)) + (0.25f * ((fRec120[1] + fRec120[3]) * fTemp369)));
			float fTemp441 = (3.14159274f * (fTemp440 * fRec34[0]));
			float fTemp442 = sinf(fTemp441);
			float fTemp443 = cosf(fTemp441);
			fRec121[0] = ((fRec121[1] * (0.0f - fTemp442)) + (fTemp440 * fTemp443));
			float fTemp444 = (((((fTemp440 * fTemp442) + (fRec121[1] * fTemp443)) * powf(0.00100000005f, (fSlow22 / fTemp275))) + fTemp249) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec83[(IOTA & 8191)] = fTemp444;
			float fTemp445 = (fSlow23 / fTemp274);
			float fTemp446 = (fTemp445 + -2.49999499f);
			int iTemp447 = min(4096, max(0, int(fTemp446)));
			float fTemp448 = floorf(fTemp446);
			float fTemp449 = ((fVec83[((IOTA - iTemp447) & 8191)] - fVec83[((IOTA - (iTemp447 + 1)) & 8191)]) * (fTemp448 + (2.0f - fTemp445)));
			fVec84[0] = fTemp449;
			fRec120[0] = ((0.5f * ((fTemp449 - fVec84[1]) * (fTemp448 + (3.0f - fTemp445)))) + (fVec83[((IOTA - iTemp447) & 8191)] + fTemp449));
			float fTemp450 = ((0.5f * (fTemp368 * fRec122[2])) + (0.25f * (fTemp369 * (fRec122[1] + fRec122[3]))));
			float fTemp451 = (3.14159274f * (fRec34[0] * fTemp450));
			float fTemp452 = sinf(fTemp451);
			float fTemp453 = cosf(fTemp451);
			fRec123[0] = ((fRec123[1] * (0.0f - fTemp452)) + (fTemp450 * fTemp453));
			float fTemp454 = ((fTemp249 + (((fTemp450 * fTemp452) + (fRec123[1] * fTemp453)) * powf(0.00100000005f, (fSlow22 / fTemp263)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp16)))))) & 8191)]);
			fVec85[(IOTA & 8191)] = fTemp454;
			float fTemp455 = (fSlow23 / fTemp262);
			float fTemp456 = (fTemp455 + -2.49999499f);
			int iTemp457 = min(4096, max(0, int(fTemp456)));
			float fTemp458 = floorf(fTemp456);
			float fTemp459 = ((fVec85[((IOTA - iTemp457) & 8191)] - fVec85[((IOTA - (iTemp457 + 1)) & 8191)]) * (fTemp458 + (2.0f - fTemp455)));
			fVec86[0] = fTemp459;
			fRec122[0] = ((0.5f * ((fTemp459 - fVec86[1]) * (fTemp458 + (3.0f - fTemp455)))) + (fVec85[((IOTA - iTemp457) & 8191)] + fTemp459));
			float fTemp460 = ((((((((fRec106[0] + fRec108[0]) + fRec110[0]) + fRec112[0]) + fRec114[0]) + fRec116[0]) + fRec118[0]) + fRec120[0]) + fRec122[0]);
			fVec87[0] = fTemp460;
			fRec105[0] = ((fTemp460 + (0.995000005f * fRec105[1])) - fVec87[1]);
			fRec3[0] = (0.00999999978f * fRec105[0]);
			float fTemp461 = (fTemp7 + fTemp241);
			float fTemp462 = (fSlow2 * float(((fRec30[0] > 0.5f) & (fRec30[0] < 0.75f))));
			float fTemp463 = ((1.0f - fTemp462) - fSlow29);
			fVec88[0] = fTemp463;
			float fTemp464 = ((fTemp463 != fVec88[1])?(fSlow30 / fTemp16):(fRec128[1] + -1.0f));
			fRec128[0] = fTemp464;
			fRec129[0] = ((fTemp464 <= 0.0f)?fTemp463:(fRec129[1] + (((1.0f - (fTemp462 + fRec129[1])) - fSlow29) / fTemp464)));
			float fTemp465 = (fSlow29 + fTemp462);
			fVec89[0] = fTemp465;
			float fTemp466 = (fRec129[0] * fTemp465);
			float fTemp467 = ((fRec126[1] > fTemp466)?fConst32:expf((0.0f - (fSlow31 * fTemp16))));
			fRec127[0] = ((fRec127[1] * fTemp467) + (fTemp466 * (1.0f - fTemp467)));
			fRec126[0] = fRec127[0];
			float fTemp468 = (fTemp13 * fRec126[0]);
			fVec90[(IOTA & 8191)] = fTemp468;
			float fTemp469 = ((fConst6 * (fTemp3 + fTemp461)) + fTemp468);
			int iTemp470 = ((fVec89[1] <= 0.0f) & (fTemp465 > 0.0f));
			fRec130[0] = ((fRec130[1] * float((1 - iTemp470))) + (fTemp24 * float(iTemp470)));
			float fTemp471 = (fRec32[0] * (fTemp469 - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]));
			float fTemp472 = (fTemp471 + 1.0f);
			float fTemp473 = (1.0f - fTemp471);
			float fTemp474 = ((0.5f * (fTemp472 * fRec125[2])) + (0.25f * (fTemp473 * (fRec125[1] + fRec125[3]))));
			float fTemp475 = (3.14159274f * (fRec34[0] * fTemp474));
			float fTemp476 = sinf(fTemp475);
			float fTemp477 = cosf(fTemp475);
			fRec131[0] = ((fRec131[1] * (0.0f - fTemp476)) + (fTemp474 * fTemp477));
			float fTemp478 = (884.400024f * fTemp16);
			fVec91[0] = fSlow33;
			float fTemp479 = ((fSlow33 != fVec91[1])?fSlow19:(fRec132[1] + -1.0f));
			fRec132[0] = fTemp479;
			fRec133[0] = ((fTemp479 <= 0.0f)?fSlow33:(fRec133[1] + (((1.0f - fRec133[1]) - fSlow29) / fTemp479)));
			float fTemp480 = (fSlow32 * tambura_faustpower2_f(fRec133[0]));
			float fTemp481 = (fTemp478 + fTemp480);
			float fTemp482 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp481 - fTemp98)))));
			float fTemp483 = (fRec36[0] * fTemp482);
			float fTemp484 = ((fTemp469 + (((fTemp474 * fTemp476) + (fRec131[1] * fTemp477)) * powf(0.00100000005f, (fSlow14 / fTemp483)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec92[(IOTA & 8191)] = fTemp484;
			float fTemp485 = (fConst37 / fTemp482);
			float fTemp486 = (fTemp485 + -2.49999499f);
			int iTemp487 = min(4096, max(0, int(fTemp486)));
			float fTemp488 = floorf(fTemp486);
			float fTemp489 = ((fVec92[((IOTA - iTemp487) & 8191)] - fVec92[((IOTA - (iTemp487 + 1)) & 8191)]) * (fTemp488 + (2.0f - fTemp485)));
			fVec93[0] = fTemp489;
			fRec125[0] = ((0.5f * ((fTemp489 - fVec93[1]) * (fTemp488 + (3.0f - fTemp485)))) + (fVec92[((IOTA - iTemp487) & 8191)] + fTemp489));
			float fTemp490 = ((0.5f * (fTemp472 * fRec134[2])) + (0.25f * (fTemp473 * (fRec134[1] + fRec134[3]))));
			float fTemp491 = (3.14159274f * (fRec34[0] * fTemp490));
			float fTemp492 = sinf(fTemp491);
			float fTemp493 = cosf(fTemp491);
			fRec135[0] = ((fRec135[1] * (0.0f - fTemp492)) + (fTemp490 * fTemp493));
			float fTemp494 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp481 - fTemp85)))));
			float fTemp495 = (fRec36[0] * fTemp494);
			float fTemp496 = ((fTemp469 + (((fTemp490 * fTemp492) + (fRec135[1] * fTemp493)) * powf(0.00100000005f, (fSlow14 / fTemp495)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec94[(IOTA & 8191)] = fTemp496;
			float fTemp497 = (fConst37 / fTemp494);
			float fTemp498 = (fTemp497 + -2.49999499f);
			int iTemp499 = min(4096, max(0, int(fTemp498)));
			float fTemp500 = floorf(fTemp498);
			float fTemp501 = ((fVec94[((IOTA - iTemp499) & 8191)] - fVec94[((IOTA - (iTemp499 + 1)) & 8191)]) * (fTemp500 + (2.0f - fTemp497)));
			fVec95[0] = fTemp501;
			fRec134[0] = ((0.5f * ((fTemp501 - fVec95[1]) * (fTemp500 + (3.0f - fTemp497)))) + (fVec94[((IOTA - iTemp499) & 8191)] + fTemp501));
			float fTemp502 = ((0.5f * (fTemp472 * fRec136[2])) + (0.25f * (fTemp473 * (fRec136[1] + fRec136[3]))));
			float fTemp503 = (3.14159274f * (fRec34[0] * fTemp502));
			float fTemp504 = sinf(fTemp503);
			float fTemp505 = cosf(fTemp503);
			fRec137[0] = ((fRec137[1] * (0.0f - fTemp504)) + (fTemp502 * fTemp505));
			float fTemp506 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp481 - fTemp72)))));
			float fTemp507 = (fRec36[0] * fTemp506);
			float fTemp508 = ((fTemp469 + (((fTemp502 * fTemp504) + (fRec137[1] * fTemp505)) * powf(0.00100000005f, (fSlow14 / fTemp507)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec96[(IOTA & 8191)] = fTemp508;
			float fTemp509 = (fConst37 / fTemp506);
			float fTemp510 = (fTemp509 + -2.49999499f);
			int iTemp511 = min(4096, max(0, int(fTemp510)));
			float fTemp512 = floorf(fTemp510);
			float fTemp513 = ((fVec96[((IOTA - iTemp511) & 8191)] - fVec96[((IOTA - (iTemp511 + 1)) & 8191)]) * (fTemp512 + (2.0f - fTemp509)));
			fVec97[0] = fTemp513;
			fRec136[0] = ((0.5f * ((fTemp513 - fVec97[1]) * (fTemp512 + (3.0f - fTemp509)))) + (fVec96[((IOTA - iTemp511) & 8191)] + fTemp513));
			float fTemp514 = ((0.5f * (fTemp472 * fRec138[2])) + (0.25f * (fTemp473 * (fRec138[1] + fRec138[3]))));
			float fTemp515 = (3.14159274f * (fRec34[0] * fTemp514));
			float fTemp516 = sinf(fTemp515);
			float fTemp517 = cosf(fTemp515);
			fRec139[0] = ((fRec139[1] * (0.0f - fTemp516)) + (fTemp514 * fTemp517));
			float fTemp518 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp481 - fRec39[0])))));
			float fTemp519 = (fRec36[0] * fTemp518);
			float fTemp520 = ((fTemp469 + (((fTemp514 * fTemp516) + (fRec139[1] * fTemp517)) * powf(0.00100000005f, (fSlow14 / fTemp519)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec98[(IOTA & 8191)] = fTemp520;
			float fTemp521 = (fConst37 / fTemp518);
			float fTemp522 = (fTemp521 + -2.49999499f);
			int iTemp523 = min(4096, max(0, int(fTemp522)));
			float fTemp524 = floorf(fTemp522);
			float fTemp525 = ((fVec98[((IOTA - iTemp523) & 8191)] - fVec98[((IOTA - (iTemp523 + 1)) & 8191)]) * (fTemp524 + (2.0f - fTemp521)));
			fVec99[0] = fTemp525;
			fRec138[0] = ((0.5f * ((fTemp525 - fVec99[1]) * (fTemp524 + (3.0f - fTemp521)))) + (fVec98[((IOTA - iTemp523) & 8191)] + fTemp525));
			float fTemp526 = ((0.5f * (fTemp472 * fRec140[2])) + (0.25f * (fTemp473 * (fRec140[1] + fRec140[3]))));
			float fTemp527 = (3.14159274f * (fRec34[0] * fTemp526));
			float fTemp528 = sinf(fTemp527);
			float fTemp529 = cosf(fTemp527);
			fRec141[0] = ((fRec141[1] * (0.0f - fTemp528)) + (fTemp526 * fTemp529));
			float fTemp530 = powf(2.0f, (1.44269502f * logf((0.0022727272f * fTemp481))));
			float fTemp531 = (fRec36[0] * fTemp530);
			float fTemp532 = ((fTemp469 + (((fTemp526 * fTemp528) + (fRec141[1] * fTemp529)) * powf(0.00100000005f, (fSlow14 / fTemp531)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec100[(IOTA & 8191)] = fTemp532;
			float fTemp533 = (fConst37 / fTemp530);
			float fTemp534 = (fTemp533 + -2.49999499f);
			int iTemp535 = min(4096, max(0, int(fTemp534)));
			float fTemp536 = floorf(fTemp534);
			float fTemp537 = ((fVec100[((IOTA - iTemp535) & 8191)] - fVec100[((IOTA - (iTemp535 + 1)) & 8191)]) * (fTemp536 + (2.0f - fTemp533)));
			fVec101[0] = fTemp537;
			fRec140[0] = ((0.5f * ((fTemp537 - fVec101[1]) * (fTemp536 + (3.0f - fTemp533)))) + (fVec100[((IOTA - iTemp535) & 8191)] + fTemp537));
			float fTemp538 = ((0.5f * (fTemp472 * fRec142[2])) + (0.25f * (fTemp473 * (fRec142[1] + fRec142[3]))));
			float fTemp539 = (3.14159274f * (fRec34[0] * fTemp538));
			float fTemp540 = sinf(fTemp539);
			float fTemp541 = cosf(fTemp539);
			fRec143[0] = ((fRec143[1] * (0.0f - fTemp540)) + (fTemp538 * fTemp541));
			float fTemp542 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp480 + (fRec39[0] + fTemp478))))));
			float fTemp543 = (fRec36[0] * fTemp542);
			float fTemp544 = ((fTemp469 + (((fTemp538 * fTemp540) + (fRec143[1] * fTemp541)) * powf(0.00100000005f, (fSlow14 / fTemp543)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec102[(IOTA & 8191)] = fTemp544;
			float fTemp545 = (fConst37 / fTemp542);
			float fTemp546 = (fTemp545 + -2.49999499f);
			int iTemp547 = min(4096, max(0, int(fTemp546)));
			float fTemp548 = floorf(fTemp546);
			float fTemp549 = ((fVec102[((IOTA - iTemp547) & 8191)] - fVec102[((IOTA - (iTemp547 + 1)) & 8191)]) * (fTemp548 + (2.0f - fTemp545)));
			fVec103[0] = fTemp549;
			fRec142[0] = ((0.5f * ((fTemp549 - fVec103[1]) * (fTemp548 + (3.0f - fTemp545)))) + (fVec102[((IOTA - iTemp547) & 8191)] + fTemp549));
			float fTemp550 = ((0.5f * (fTemp472 * fRec144[2])) + (0.25f * (fTemp473 * (fRec144[1] + fRec144[3]))));
			float fTemp551 = (3.14159274f * (fRec34[0] * fTemp550));
			float fTemp552 = sinf(fTemp551);
			float fTemp553 = cosf(fTemp551);
			fRec145[0] = ((fRec145[1] * (0.0f - fTemp552)) + (fTemp550 * fTemp553));
			float fTemp554 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp480 + (fTemp72 + fTemp478))))));
			float fTemp555 = (fRec36[0] * fTemp554);
			float fTemp556 = ((fTemp469 + (((fTemp550 * fTemp552) + (fRec145[1] * fTemp553)) * powf(0.00100000005f, (fSlow14 / fTemp555)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec104[(IOTA & 8191)] = fTemp556;
			float fTemp557 = (fConst37 / fTemp554);
			float fTemp558 = (fTemp557 + -2.49999499f);
			int iTemp559 = min(4096, max(0, int(fTemp558)));
			float fTemp560 = floorf(fTemp558);
			float fTemp561 = ((fVec104[((IOTA - iTemp559) & 8191)] - fVec104[((IOTA - (iTemp559 + 1)) & 8191)]) * (fTemp560 + (2.0f - fTemp557)));
			fVec105[0] = fTemp561;
			fRec144[0] = ((0.5f * ((fTemp561 - fVec105[1]) * (fTemp560 + (3.0f - fTemp557)))) + (fVec104[((IOTA - iTemp559) & 8191)] + fTemp561));
			float fTemp562 = ((0.5f * (fTemp472 * fRec146[2])) + (0.25f * (fTemp473 * (fRec146[1] + fRec146[3]))));
			float fTemp563 = (3.14159274f * (fRec34[0] * fTemp562));
			float fTemp564 = sinf(fTemp563);
			float fTemp565 = cosf(fTemp563);
			fRec147[0] = ((fRec147[1] * (0.0f - fTemp564)) + (fTemp562 * fTemp565));
			float fTemp566 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp480 + (fTemp85 + fTemp478))))));
			float fTemp567 = (fRec36[0] * fTemp566);
			float fTemp568 = ((fTemp469 + (((fTemp562 * fTemp564) + (fRec147[1] * fTemp565)) * powf(0.00100000005f, (fSlow14 / fTemp567)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec106[(IOTA & 8191)] = fTemp568;
			float fTemp569 = (fConst37 / fTemp566);
			float fTemp570 = (fTemp569 + -2.49999499f);
			int iTemp571 = min(4096, max(0, int(fTemp570)));
			float fTemp572 = floorf(fTemp570);
			float fTemp573 = ((fVec106[((IOTA - iTemp571) & 8191)] - fVec106[((IOTA - (iTemp571 + 1)) & 8191)]) * (fTemp572 + (2.0f - fTemp569)));
			fVec107[0] = fTemp573;
			fRec146[0] = ((0.5f * ((fTemp573 - fVec107[1]) * (fTemp572 + (3.0f - fTemp569)))) + (fVec106[((IOTA - iTemp571) & 8191)] + fTemp573));
			float fTemp574 = ((0.5f * (fTemp472 * fRec148[2])) + (0.25f * (fTemp473 * (fRec148[1] + fRec148[3]))));
			float fTemp575 = (3.14159274f * (fRec34[0] * fTemp574));
			float fTemp576 = sinf(fTemp575);
			float fTemp577 = cosf(fTemp575);
			fRec149[0] = ((fRec149[1] * (0.0f - fTemp576)) + (fTemp574 * fTemp577));
			float fTemp578 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp480 + (fTemp98 + fTemp478))))));
			float fTemp579 = (fRec36[0] * fTemp578);
			float fTemp580 = ((fTemp469 + (((fTemp574 * fTemp576) + (fRec149[1] * fTemp577)) * powf(0.00100000005f, (fSlow14 / fTemp579)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec108[(IOTA & 8191)] = fTemp580;
			float fTemp581 = (fConst37 / fTemp578);
			float fTemp582 = (fTemp581 + -2.49999499f);
			int iTemp583 = min(4096, max(0, int(fTemp582)));
			float fTemp584 = floorf(fTemp582);
			float fTemp585 = ((fVec108[((IOTA - iTemp583) & 8191)] - fVec108[((IOTA - (iTemp583 + 1)) & 8191)]) * (fTemp584 + (2.0f - fTemp581)));
			fVec109[0] = fTemp585;
			fRec148[0] = (((0.5f * ((fTemp585 - fVec109[1]) * (fTemp584 + (3.0f - fTemp581)))) + fVec108[((IOTA - iTemp583) & 8191)]) + fTemp585);
			float fTemp586 = ((((((((fRec125[0] + fRec134[0]) + fRec136[0]) + fRec138[0]) + fRec140[0]) + fRec142[0]) + fRec144[0]) + fRec146[0]) + fRec148[0]);
			fVec110[0] = fTemp586;
			fRec124[0] = ((fTemp586 + (0.995000005f * fRec124[1])) - fVec110[1]);
			float fRec4 = (0.00999999978f * fRec124[0]);
			float fTemp587 = (fRec32[0] * (fTemp468 - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]));
			float fTemp588 = (fTemp587 + 1.0f);
			float fTemp589 = (1.0f - fTemp587);
			float fTemp590 = ((0.5f * (fTemp588 * fRec151[2])) + (0.25f * (fTemp589 * (fRec151[1] + fRec151[3]))));
			float fTemp591 = (3.14159274f * (fRec34[0] * fTemp590));
			float fTemp592 = sinf(fTemp591);
			float fTemp593 = cosf(fTemp591);
			fRec152[0] = ((fRec152[1] * (0.0f - fTemp592)) + (fTemp590 * fTemp593));
			float fTemp594 = ((fTemp468 + (((fTemp590 * fTemp592) + (fRec152[1] * fTemp593)) * powf(0.00100000005f, (fSlow22 / fTemp579)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec111[(IOTA & 8191)] = fTemp594;
			float fTemp595 = (fSlow23 / fTemp578);
			float fTemp596 = (fTemp595 + -2.49999499f);
			int iTemp597 = min(4096, max(0, int(fTemp596)));
			float fTemp598 = floorf(fTemp596);
			float fTemp599 = ((fVec111[((IOTA - iTemp597) & 8191)] - fVec111[((IOTA - (iTemp597 + 1)) & 8191)]) * (fTemp598 + (2.0f - fTemp595)));
			fVec112[0] = fTemp599;
			fRec151[0] = ((0.5f * ((fTemp599 - fVec112[1]) * (fTemp598 + (3.0f - fTemp595)))) + (fVec111[((IOTA - iTemp597) & 8191)] + fTemp599));
			float fTemp600 = ((0.5f * (fTemp588 * fRec153[2])) + (0.25f * (fTemp589 * (fRec153[1] + fRec153[3]))));
			float fTemp601 = (3.14159274f * (fRec34[0] * fTemp600));
			float fTemp602 = sinf(fTemp601);
			float fTemp603 = cosf(fTemp601);
			fRec154[0] = ((fRec154[1] * (0.0f - fTemp602)) + (fTemp600 * fTemp603));
			float fTemp604 = ((fTemp468 + (((fTemp600 * fTemp602) + (fRec154[1] * fTemp603)) * powf(0.00100000005f, (fSlow22 / fTemp567)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec113[(IOTA & 8191)] = fTemp604;
			float fTemp605 = (fSlow23 / fTemp566);
			float fTemp606 = (fTemp605 + -2.49999499f);
			int iTemp607 = min(4096, max(0, int(fTemp606)));
			float fTemp608 = floorf(fTemp606);
			float fTemp609 = ((fVec113[((IOTA - iTemp607) & 8191)] - fVec113[((IOTA - (iTemp607 + 1)) & 8191)]) * (fTemp608 + (2.0f - fTemp605)));
			fVec114[0] = fTemp609;
			fRec153[0] = ((0.5f * ((fTemp609 - fVec114[1]) * (fTemp608 + (3.0f - fTemp605)))) + (fVec113[((IOTA - iTemp607) & 8191)] + fTemp609));
			float fTemp610 = ((0.5f * (fTemp588 * fRec155[2])) + (0.25f * (fTemp589 * (fRec155[1] + fRec155[3]))));
			float fTemp611 = (3.14159274f * (fRec34[0] * fTemp610));
			float fTemp612 = sinf(fTemp611);
			float fTemp613 = cosf(fTemp611);
			fRec156[0] = ((fRec156[1] * (0.0f - fTemp612)) + (fTemp610 * fTemp613));
			float fTemp614 = ((fTemp468 + (((fTemp610 * fTemp612) + (fRec156[1] * fTemp613)) * powf(0.00100000005f, (fSlow22 / fTemp555)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec115[(IOTA & 8191)] = fTemp614;
			float fTemp615 = (fSlow23 / fTemp554);
			float fTemp616 = (fTemp615 + -2.49999499f);
			int iTemp617 = min(4096, max(0, int(fTemp616)));
			float fTemp618 = floorf(fTemp616);
			float fTemp619 = ((fVec115[((IOTA - iTemp617) & 8191)] - fVec115[((IOTA - (iTemp617 + 1)) & 8191)]) * (fTemp618 + (2.0f - fTemp615)));
			fVec116[0] = fTemp619;
			fRec155[0] = ((0.5f * ((fTemp619 - fVec116[1]) * (fTemp618 + (3.0f - fTemp615)))) + (fVec115[((IOTA - iTemp617) & 8191)] + fTemp619));
			float fTemp620 = ((0.5f * (fTemp588 * fRec157[2])) + (0.25f * (fTemp589 * (fRec157[1] + fRec157[3]))));
			float fTemp621 = (3.14159274f * (fRec34[0] * fTemp620));
			float fTemp622 = sinf(fTemp621);
			float fTemp623 = cosf(fTemp621);
			fRec158[0] = ((fRec158[1] * (0.0f - fTemp622)) + (fTemp620 * fTemp623));
			float fTemp624 = ((fTemp468 + (((fTemp620 * fTemp622) + (fRec158[1] * fTemp623)) * powf(0.00100000005f, (fSlow22 / fTemp543)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec117[(IOTA & 8191)] = fTemp624;
			float fTemp625 = (fSlow23 / fTemp542);
			float fTemp626 = (fTemp625 + -2.49999499f);
			int iTemp627 = min(4096, max(0, int(fTemp626)));
			float fTemp628 = floorf(fTemp626);
			float fTemp629 = ((fVec117[((IOTA - iTemp627) & 8191)] - fVec117[((IOTA - (iTemp627 + 1)) & 8191)]) * (fTemp628 + (2.0f - fTemp625)));
			fVec118[0] = fTemp629;
			fRec157[0] = ((0.5f * ((fTemp629 - fVec118[1]) * (fTemp628 + (3.0f - fTemp625)))) + (fVec117[((IOTA - iTemp627) & 8191)] + fTemp629));
			float fTemp630 = ((0.5f * (fTemp588 * fRec159[2])) + (0.25f * (fTemp589 * (fRec159[1] + fRec159[3]))));
			float fTemp631 = (3.14159274f * (fRec34[0] * fTemp630));
			float fTemp632 = sinf(fTemp631);
			float fTemp633 = cosf(fTemp631);
			fRec160[0] = ((fRec160[1] * (0.0f - fTemp632)) + (fTemp630 * fTemp633));
			float fTemp634 = ((fTemp468 + (((fTemp630 * fTemp632) + (fRec160[1] * fTemp633)) * powf(0.00100000005f, (fSlow22 / fTemp531)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec119[(IOTA & 8191)] = fTemp634;
			float fTemp635 = (fSlow23 / fTemp530);
			float fTemp636 = (fTemp635 + -2.49999499f);
			int iTemp637 = min(4096, max(0, int(fTemp636)));
			float fTemp638 = floorf(fTemp636);
			float fTemp639 = ((fVec119[((IOTA - iTemp637) & 8191)] - fVec119[((IOTA - (iTemp637 + 1)) & 8191)]) * (fTemp638 + (2.0f - fTemp635)));
			fVec120[0] = fTemp639;
			fRec159[0] = ((0.5f * ((fTemp639 - fVec120[1]) * (fTemp638 + (3.0f - fTemp635)))) + (fVec119[((IOTA - iTemp637) & 8191)] + fTemp639));
			float fTemp640 = ((0.5f * (fTemp588 * fRec161[2])) + (0.25f * (fTemp589 * (fRec161[1] + fRec161[3]))));
			float fTemp641 = (3.14159274f * (fRec34[0] * fTemp640));
			float fTemp642 = sinf(fTemp641);
			float fTemp643 = cosf(fTemp641);
			fRec162[0] = ((fRec162[1] * (0.0f - fTemp642)) + (fTemp640 * fTemp643));
			float fTemp644 = ((fTemp468 + (((fTemp640 * fTemp642) + (fRec162[1] * fTemp643)) * powf(0.00100000005f, (fSlow22 / fTemp519)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec121[(IOTA & 8191)] = fTemp644;
			float fTemp645 = (fSlow23 / fTemp518);
			float fTemp646 = (fTemp645 + -2.49999499f);
			int iTemp647 = min(4096, max(0, int(fTemp646)));
			float fTemp648 = floorf(fTemp646);
			float fTemp649 = ((fVec121[((IOTA - iTemp647) & 8191)] - fVec121[((IOTA - (iTemp647 + 1)) & 8191)]) * (fTemp648 + (2.0f - fTemp645)));
			fVec122[0] = fTemp649;
			fRec161[0] = ((0.5f * ((fTemp649 - fVec122[1]) * (fTemp648 + (3.0f - fTemp645)))) + (fVec121[((IOTA - iTemp647) & 8191)] + fTemp649));
			float fTemp650 = ((0.5f * (fTemp588 * fRec163[2])) + (0.25f * (fTemp589 * (fRec163[1] + fRec163[3]))));
			float fTemp651 = (3.14159274f * (fRec34[0] * fTemp650));
			float fTemp652 = sinf(fTemp651);
			float fTemp653 = cosf(fTemp651);
			fRec164[0] = ((fRec164[1] * (0.0f - fTemp652)) + (fTemp650 * fTemp653));
			float fTemp654 = ((fTemp468 + (((fTemp650 * fTemp652) + (fRec164[1] * fTemp653)) * powf(0.00100000005f, (fSlow22 / fTemp507)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec123[(IOTA & 8191)] = fTemp654;
			float fTemp655 = (fSlow23 / fTemp506);
			float fTemp656 = (fTemp655 + -2.49999499f);
			int iTemp657 = min(4096, max(0, int(fTemp656)));
			float fTemp658 = floorf(fTemp656);
			float fTemp659 = ((fVec123[((IOTA - iTemp657) & 8191)] - fVec123[((IOTA - (iTemp657 + 1)) & 8191)]) * (fTemp658 + (2.0f - fTemp655)));
			fVec124[0] = fTemp659;
			fRec163[0] = ((0.5f * ((fTemp659 - fVec124[1]) * (fTemp658 + (3.0f - fTemp655)))) + (fVec123[((IOTA - iTemp657) & 8191)] + fTemp659));
			float fTemp660 = ((0.5f * (fTemp588 * fRec165[2])) + (0.25f * (fTemp589 * (fRec165[1] + fRec165[3]))));
			float fTemp661 = (3.14159274f * (fRec34[0] * fTemp660));
			float fTemp662 = sinf(fTemp661);
			float fTemp663 = cosf(fTemp661);
			fRec166[0] = ((fRec166[1] * (0.0f - fTemp662)) + (fTemp660 * fTemp663));
			float fTemp664 = ((fTemp468 + (((fTemp660 * fTemp662) + (fRec166[1] * fTemp663)) * powf(0.00100000005f, (fSlow22 / fTemp495)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec125[(IOTA & 8191)] = fTemp664;
			float fTemp665 = (fSlow23 / fTemp494);
			float fTemp666 = (fTemp665 + -2.49999499f);
			int iTemp667 = min(4096, max(0, int(fTemp666)));
			float fTemp668 = floorf(fTemp666);
			float fTemp669 = ((fVec125[((IOTA - iTemp667) & 8191)] - fVec125[((IOTA - (iTemp667 + 1)) & 8191)]) * (fTemp668 + (2.0f - fTemp665)));
			fVec126[0] = fTemp669;
			fRec165[0] = ((0.5f * ((fTemp669 - fVec126[1]) * (fTemp668 + (3.0f - fTemp665)))) + (fVec125[((IOTA - iTemp667) & 8191)] + fTemp669));
			float fTemp670 = ((0.5f * (fTemp588 * fRec167[2])) + (0.25f * (fTemp589 * (fRec167[1] + fRec167[3]))));
			float fTemp671 = (3.14159274f * (fRec34[0] * fTemp670));
			float fTemp672 = sinf(fTemp671);
			float fTemp673 = cosf(fTemp671);
			fRec168[0] = ((fRec168[1] * (0.0f - fTemp672)) + (fTemp670 * fTemp673));
			float fTemp674 = ((fTemp468 + (((fTemp670 * fTemp672) + (fRec168[1] * fTemp673)) * powf(0.00100000005f, (fSlow22 / fTemp483)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp16)))))) & 8191)]);
			fVec127[(IOTA & 8191)] = fTemp674;
			float fTemp675 = (fSlow23 / fTemp482);
			float fTemp676 = (fTemp675 + -2.49999499f);
			int iTemp677 = min(4096, max(0, int(fTemp676)));
			float fTemp678 = floorf(fTemp676);
			float fTemp679 = ((fVec127[((IOTA - iTemp677) & 8191)] - fVec127[((IOTA - (iTemp677 + 1)) & 8191)]) * (fTemp678 + (2.0f - fTemp675)));
			fVec128[0] = fTemp679;
			fRec167[0] = ((0.5f * ((fTemp679 - fVec128[1]) * (fTemp678 + (3.0f - fTemp675)))) + (fVec127[((IOTA - iTemp677) & 8191)] + fTemp679));
			fVec129[0] = ((((((((fRec167[0] + fRec165[0]) + fRec163[0]) + fRec161[0]) + fRec159[0]) + fRec157[0]) + fRec155[0]) + fRec153[0]) + fRec151[0]);
			fRec150[0] = ((fRec151[0] + (fRec153[0] + (fRec155[0] + (fRec157[0] + (fRec159[0] + (fRec161[0] + (fRec163[0] + (fRec165[0] + (fRec167[0] + (0.995000005f * fRec150[1])))))))))) - fVec129[1]);
			fRec5[0] = (0.00999999978f * fRec150[0]);
			float fTemp680 = (fConst6 * (fTemp461 + fTemp11));
			float fTemp681 = (fSlow2 * float(((fRec30[0] > 0.75f) & (fRec30[0] < 1.0f))));
			float fTemp682 = ((1.0f - fTemp681) - fSlow34);
			fVec130[0] = fTemp682;
			float fTemp683 = ((fTemp682 != fVec130[1])?(fSlow35 / fTemp16):(fRec173[1] + -1.0f));
			fRec173[0] = fTemp683;
			fRec174[0] = ((fTemp683 <= 0.0f)?fTemp682:(fRec174[1] + (((1.0f - (fTemp681 + fRec174[1])) - fSlow34) / fTemp683)));
			float fTemp684 = (fSlow34 + fTemp681);
			fVec131[0] = fTemp684;
			float fTemp685 = (fRec174[0] * fTemp684);
			float fTemp686 = ((fRec171[1] > fTemp685)?fConst32:expf((0.0f - (fSlow36 * fTemp16))));
			fRec172[0] = ((fRec172[1] * fTemp686) + (fTemp685 * (1.0f - fTemp686)));
			fRec171[0] = fRec172[0];
			float fTemp687 = (fTemp13 * fRec171[0]);
			fVec132[(IOTA & 8191)] = fTemp687;
			float fTemp688 = (fTemp680 + fTemp687);
			int iTemp689 = ((fVec131[1] <= 0.0f) & (fTemp684 > 0.0f));
			fRec175[0] = ((fRec175[1] * float((1 - iTemp689))) + (fTemp24 * float(iTemp689)));
			float fTemp690 = (fRec32[0] * (fTemp688 - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]));
			float fTemp691 = (fTemp690 + 1.0f);
			float fTemp692 = (1.0f - fTemp690);
			float fTemp693 = ((0.5f * (fRec170[2] * fTemp691)) + (0.25f * ((fRec170[1] + fRec170[3]) * fTemp692)));
			float fTemp694 = (3.14159274f * (fRec34[0] * fTemp693));
			float fTemp695 = sinf(fTemp694);
			float fTemp696 = cosf(fTemp694);
			fRec176[0] = ((fRec176[1] * (0.0f - fTemp695)) + (fTemp693 * fTemp696));
			float fTemp697 = (440.0f * fTemp16);
			fVec133[0] = fSlow38;
			float fTemp698 = ((fSlow38 != fVec133[1])?fSlow19:(fRec177[1] + -1.0f));
			fRec177[0] = fTemp698;
			fRec178[0] = ((fTemp698 <= 0.0f)?fSlow38:(fRec178[1] + (((1.0f - fRec178[1]) - fSlow34) / fTemp698)));
			float fTemp699 = (fSlow37 * tambura_faustpower2_f(fRec178[0]));
			float fTemp700 = powf(2.0f, (1.44269502f * logf((0.0022727272f * ((fTemp85 + fTemp697) + fTemp699)))));
			float fTemp701 = (fRec36[0] * fTemp700);
			float fTemp702 = ((((((fTemp693 * fTemp695) + (fRec176[1] * fTemp696)) * powf(0.00100000005f, (fSlow14 / fTemp701))) + fTemp680) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec134[(IOTA & 8191)] = fTemp702;
			float fTemp703 = (fConst37 / fTemp700);
			float fTemp704 = (fTemp703 + -2.49999499f);
			int iTemp705 = min(4096, max(0, int(fTemp704)));
			float fTemp706 = floorf(fTemp704);
			float fTemp707 = ((fVec134[((IOTA - iTemp705) & 8191)] - fVec134[((IOTA - (iTemp705 + 1)) & 8191)]) * (fTemp706 + (2.0f - fTemp703)));
			fVec135[0] = fTemp707;
			fRec170[0] = ((0.5f * ((fTemp707 - fVec135[1]) * (fTemp706 + (3.0f - fTemp703)))) + (fVec134[((IOTA - iTemp705) & 8191)] + fTemp707));
			float fTemp708 = ((0.5f * (fRec179[2] * fTemp691)) + (0.25f * ((fRec179[1] + fRec179[3]) * fTemp692)));
			float fTemp709 = (3.14159274f * (fRec34[0] * fTemp708));
			float fTemp710 = sinf(fTemp709);
			float fTemp711 = cosf(fTemp709);
			fRec180[0] = ((fRec180[1] * (0.0f - fTemp710)) + (fTemp708 * fTemp711));
			float fTemp712 = powf(2.0f, (1.44269502f * logf((0.0022727272f * ((fTemp72 + fTemp697) + fTemp699)))));
			float fTemp713 = (fRec36[0] * fTemp712);
			float fTemp714 = ((((((fTemp708 * fTemp710) + (fRec180[1] * fTemp711)) * powf(0.00100000005f, (fSlow14 / fTemp713))) + fTemp680) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec136[(IOTA & 8191)] = fTemp714;
			float fTemp715 = (fConst37 / fTemp712);
			float fTemp716 = (fTemp715 + -2.49999499f);
			int iTemp717 = min(4096, max(0, int(fTemp716)));
			float fTemp718 = floorf(fTemp716);
			float fTemp719 = ((fVec136[((IOTA - iTemp717) & 8191)] - fVec136[((IOTA - (iTemp717 + 1)) & 8191)]) * (fTemp718 + (2.0f - fTemp715)));
			fVec137[0] = fTemp719;
			fRec179[0] = ((0.5f * ((fTemp719 - fVec137[1]) * (fTemp718 + (3.0f - fTemp715)))) + (fVec136[((IOTA - iTemp717) & 8191)] + fTemp719));
			float fTemp720 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp699 + (fRec39[0] + fTemp697))))));
			float fTemp721 = (fConst37 / fTemp720);
			float fTemp722 = (fTemp721 + -2.49999499f);
			float fTemp723 = floorf(fTemp722);
			float fTemp724 = ((0.5f * (fTemp691 * fRec181[2])) + (0.25f * (fTemp692 * (fRec181[1] + fRec181[3]))));
			float fTemp725 = (3.14159274f * (fRec34[0] * fTemp724));
			float fTemp726 = sinf(fTemp725);
			float fTemp727 = cosf(fTemp725);
			fRec182[0] = ((fRec182[1] * (0.0f - fTemp726)) + (fTemp724 * fTemp727));
			float fTemp728 = (fRec36[0] * fTemp720);
			float fTemp729 = ((fTemp688 + (((fTemp724 * fTemp726) + (fRec182[1] * fTemp727)) * powf(0.00100000005f, (fSlow14 / fTemp728)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec138[(IOTA & 8191)] = fTemp729;
			int iTemp730 = min(4096, max(0, int(fTemp722)));
			float fTemp731 = ((fTemp723 + (2.0f - fTemp721)) * (fVec138[((IOTA - iTemp730) & 8191)] - fVec138[((IOTA - (iTemp730 + 1)) & 8191)]));
			fVec139[0] = fTemp731;
			fRec181[0] = ((fTemp731 + (0.5f * ((fTemp731 - fVec139[1]) * (fTemp723 + (3.0f - fTemp721))))) + fVec138[((IOTA - iTemp730) & 8191)]);
			float fTemp732 = ((0.5f * (fTemp691 * fRec183[2])) + (0.25f * (fTemp692 * (fRec183[1] + fRec183[3]))));
			float fTemp733 = (3.14159274f * (fRec34[0] * fTemp732));
			float fTemp734 = sinf(fTemp733);
			float fTemp735 = cosf(fTemp733);
			fRec184[0] = ((fRec184[1] * (0.0f - fTemp734)) + (fTemp732 * fTemp735));
			float fTemp736 = (fTemp697 + fTemp699);
			float fTemp737 = powf(2.0f, (1.44269502f * logf((0.0022727272f * fTemp736))));
			float fTemp738 = (fRec36[0] * fTemp737);
			float fTemp739 = ((fTemp688 + (((fTemp732 * fTemp734) + (fRec184[1] * fTemp735)) * powf(0.00100000005f, (fSlow14 / fTemp738)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec140[(IOTA & 8191)] = fTemp739;
			float fTemp740 = (fConst37 / fTemp737);
			float fTemp741 = (fTemp740 + -2.49999499f);
			int iTemp742 = min(4096, max(0, int(fTemp741)));
			float fTemp743 = floorf(fTemp741);
			float fTemp744 = ((fVec140[((IOTA - iTemp742) & 8191)] - fVec140[((IOTA - (iTemp742 + 1)) & 8191)]) * (fTemp743 + (2.0f - fTemp740)));
			fVec141[0] = fTemp744;
			fRec183[0] = ((0.5f * ((fTemp744 - fVec141[1]) * (fTemp743 + (3.0f - fTemp740)))) + (fVec140[((IOTA - iTemp742) & 8191)] + fTemp744));
			float fTemp745 = ((0.5f * (fTemp691 * fRec185[2])) + (0.25f * (fTemp692 * (fRec185[1] + fRec185[3]))));
			float fTemp746 = (3.14159274f * (fRec34[0] * fTemp745));
			float fTemp747 = sinf(fTemp746);
			float fTemp748 = cosf(fTemp746);
			fRec186[0] = ((fRec186[1] * (0.0f - fTemp747)) + (fTemp745 * fTemp748));
			float fTemp749 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp736 - fRec39[0])))));
			float fTemp750 = (fRec36[0] * fTemp749);
			float fTemp751 = ((fTemp688 + (((fTemp745 * fTemp747) + (fRec186[1] * fTemp748)) * powf(0.00100000005f, (fSlow14 / fTemp750)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec142[(IOTA & 8191)] = fTemp751;
			float fTemp752 = (fConst37 / fTemp749);
			float fTemp753 = (fTemp752 + -2.49999499f);
			int iTemp754 = min(4096, max(0, int(fTemp753)));
			float fTemp755 = floorf(fTemp753);
			float fTemp756 = ((fVec142[((IOTA - iTemp754) & 8191)] - fVec142[((IOTA - (iTemp754 + 1)) & 8191)]) * (fTemp755 + (2.0f - fTemp752)));
			fVec143[0] = fTemp756;
			fRec185[0] = ((0.5f * ((fTemp756 - fVec143[1]) * (fTemp755 + (3.0f - fTemp752)))) + (fVec142[((IOTA - iTemp754) & 8191)] + fTemp756));
			float fTemp757 = ((0.5f * (fRec187[2] * fTemp691)) + (0.25f * ((fRec187[1] + fRec187[3]) * fTemp692)));
			float fTemp758 = (3.14159274f * (fRec34[0] * fTemp757));
			float fTemp759 = sinf(fTemp758);
			float fTemp760 = cosf(fTemp758);
			fRec188[0] = ((fRec188[1] * (0.0f - fTemp759)) + (fTemp757 * fTemp760));
			float fTemp761 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp736 - fTemp72)))));
			float fTemp762 = (fRec36[0] * fTemp761);
			float fTemp763 = ((fTemp688 + (((fTemp757 * fTemp759) + (fRec188[1] * fTemp760)) * powf(0.00100000005f, (fSlow14 / fTemp762)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec144[(IOTA & 8191)] = fTemp763;
			float fTemp764 = (fConst37 / fTemp761);
			float fTemp765 = (fTemp764 + -2.49999499f);
			int iTemp766 = min(4096, max(0, int(fTemp765)));
			float fTemp767 = floorf(fTemp765);
			float fTemp768 = ((fVec144[((IOTA - iTemp766) & 8191)] - fVec144[((IOTA - (iTemp766 + 1)) & 8191)]) * (fTemp767 + (2.0f - fTemp764)));
			fVec145[0] = fTemp768;
			fRec187[0] = ((0.5f * ((fTemp768 - fVec145[1]) * (fTemp767 + (3.0f - fTemp764)))) + (fVec144[((IOTA - iTemp766) & 8191)] + fTemp768));
			float fTemp769 = ((0.5f * (fRec189[2] * fTemp691)) + (0.25f * ((fRec189[1] + fRec189[3]) * fTemp692)));
			float fTemp770 = (3.14159274f * (fRec34[0] * fTemp769));
			float fTemp771 = sinf(fTemp770);
			float fTemp772 = cosf(fTemp770);
			fRec190[0] = ((fRec190[1] * (0.0f - fTemp771)) + (fTemp769 * fTemp772));
			float fTemp773 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp736 - fTemp85)))));
			float fTemp774 = (fRec36[0] * fTemp773);
			float fTemp775 = ((((((fTemp769 * fTemp771) + (fRec190[1] * fTemp772)) * powf(0.00100000005f, (fSlow14 / fTemp774))) + fTemp680) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec146[(IOTA & 8191)] = fTemp775;
			float fTemp776 = (fConst37 / fTemp773);
			float fTemp777 = (fTemp776 + -2.49999499f);
			int iTemp778 = min(4096, max(0, int(fTemp777)));
			float fTemp779 = floorf(fTemp777);
			float fTemp780 = ((fVec146[((IOTA - iTemp778) & 8191)] - fVec146[((IOTA - (iTemp778 + 1)) & 8191)]) * (fTemp779 + (2.0f - fTemp776)));
			fVec147[0] = fTemp780;
			fRec189[0] = ((0.5f * ((fTemp780 - fVec147[1]) * (fTemp779 + (3.0f - fTemp776)))) + (fVec146[((IOTA - iTemp778) & 8191)] + fTemp780));
			float fTemp781 = ((0.5f * (fRec191[2] * fTemp691)) + (0.25f * ((fRec191[1] + fRec191[3]) * fTemp692)));
			float fTemp782 = (3.14159274f * (fRec34[0] * fTemp781));
			float fTemp783 = sinf(fTemp782);
			float fTemp784 = cosf(fTemp782);
			fRec192[0] = ((fRec192[1] * (0.0f - fTemp783)) + (fTemp781 * fTemp784));
			float fTemp785 = powf(2.0f, (1.44269502f * logf((0.0022727272f * ((fTemp98 + fTemp697) + fTemp699)))));
			float fTemp786 = (fRec36[0] * fTemp785);
			float fTemp787 = ((((((fTemp781 * fTemp783) + (fRec192[1] * fTemp784)) * powf(0.00100000005f, (fSlow14 / fTemp786))) + fTemp680) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec148[(IOTA & 8191)] = fTemp787;
			float fTemp788 = (fConst37 / fTemp785);
			float fTemp789 = (fTemp788 + -2.49999499f);
			int iTemp790 = min(4096, max(0, int(fTemp789)));
			float fTemp791 = floorf(fTemp789);
			float fTemp792 = ((fVec148[((IOTA - iTemp790) & 8191)] - fVec148[((IOTA - (iTemp790 + 1)) & 8191)]) * (fTemp791 + (2.0f - fTemp788)));
			fVec149[0] = fTemp792;
			fRec191[0] = ((0.5f * ((fTemp792 - fVec149[1]) * (fTemp791 + (3.0f - fTemp788)))) + (fVec148[((IOTA - iTemp790) & 8191)] + fTemp792));
			float fTemp793 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp736 - fTemp98)))));
			float fTemp794 = (fRec36[0] * fTemp793);
			float fTemp795 = ((0.5f * (fTemp691 * fRec193[2])) + (0.25f * (fTemp692 * (fRec193[1] + fRec193[3]))));
			float fTemp796 = (3.14159274f * (fRec34[0] * fTemp795));
			float fTemp797 = sinf(fTemp796);
			float fTemp798 = cosf(fTemp796);
			fRec194[0] = ((fRec194[1] * (0.0f - fTemp797)) + (fTemp795 * fTemp798));
			float fTemp799 = ((((powf(0.00100000005f, (fSlow14 / fTemp794)) * ((fTemp795 * fTemp797) + (fRec194[1] * fTemp798))) + fTemp680) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec150[(IOTA & 8191)] = fTemp799;
			float fTemp800 = (fConst37 / fTemp793);
			float fTemp801 = (fTemp800 + -2.49999499f);
			int iTemp802 = min(4096, max(0, int(fTemp801)));
			float fTemp803 = floorf(fTemp801);
			float fTemp804 = ((fVec150[((IOTA - iTemp802) & 8191)] - fVec150[((IOTA - (iTemp802 + 1)) & 8191)]) * (fTemp803 + (2.0f - fTemp800)));
			fVec151[0] = fTemp804;
			fRec193[0] = ((0.5f * ((fTemp804 - fVec151[1]) * (fTemp803 + (3.0f - fTemp800)))) + (fVec150[((IOTA - iTemp802) & 8191)] + fTemp804));
			float fTemp805 = (fRec170[0] + (fRec179[0] + (fRec181[0] + (fRec183[0] + (fRec185[0] + (fRec187[0] + (fRec189[0] + (fRec191[0] + fRec193[0]))))))));
			fVec152[0] = fTemp805;
			fRec169[0] = ((fTemp805 + (0.995000005f * fRec169[1])) - fVec152[1]);
			float fRec6 = (0.00999999978f * fRec169[0]);
			float fTemp806 = (fRec32[0] * (fTemp687 - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]));
			float fTemp807 = (fTemp806 + 1.0f);
			float fTemp808 = (1.0f - fTemp806);
			float fTemp809 = ((0.5f * (fTemp807 * fRec196[2])) + (0.25f * (fTemp808 * (fRec196[1] + fRec196[3]))));
			float fTemp810 = (3.14159274f * (fRec34[0] * fTemp809));
			float fTemp811 = sinf(fTemp810);
			float fTemp812 = cosf(fTemp810);
			fRec197[0] = ((fRec197[1] * (0.0f - fTemp811)) + (fTemp809 * fTemp812));
			float fTemp813 = (((((fTemp809 * fTemp811) + (fRec197[1] * fTemp812)) * powf(0.00100000005f, (fSlow22 / fTemp794))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec153[(IOTA & 8191)] = fTemp813;
			float fTemp814 = (fSlow23 / fTemp793);
			float fTemp815 = (fTemp814 + -2.49999499f);
			int iTemp816 = min(4096, max(0, int(fTemp815)));
			float fTemp817 = floorf(fTemp815);
			float fTemp818 = ((fVec153[((IOTA - iTemp816) & 8191)] - fVec153[((IOTA - (iTemp816 + 1)) & 8191)]) * (fTemp817 + (2.0f - fTemp814)));
			fVec154[0] = fTemp818;
			fRec196[0] = ((0.5f * ((fTemp818 - fVec154[1]) * (fTemp817 + (3.0f - fTemp814)))) + (fVec153[((IOTA - iTemp816) & 8191)] + fTemp818));
			float fTemp819 = ((0.5f * (fTemp807 * fRec198[2])) + (0.25f * (fTemp808 * (fRec198[1] + fRec198[3]))));
			float fTemp820 = (3.14159274f * (fRec34[0] * fTemp819));
			float fTemp821 = sinf(fTemp820);
			float fTemp822 = cosf(fTemp820);
			fRec199[0] = ((fRec199[1] * (0.0f - fTemp821)) + (fTemp819 * fTemp822));
			float fTemp823 = (((((fTemp819 * fTemp821) + (fRec199[1] * fTemp822)) * powf(0.00100000005f, (fSlow22 / fTemp774))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec155[(IOTA & 8191)] = fTemp823;
			float fTemp824 = (fSlow23 / fTemp773);
			float fTemp825 = (fTemp824 + -2.49999499f);
			int iTemp826 = min(4096, max(0, int(fTemp825)));
			float fTemp827 = floorf(fTemp825);
			float fTemp828 = ((fVec155[((IOTA - iTemp826) & 8191)] - fVec155[((IOTA - (iTemp826 + 1)) & 8191)]) * (fTemp827 + (2.0f - fTemp824)));
			fVec156[0] = fTemp828;
			fRec198[0] = ((0.5f * ((fTemp828 - fVec156[1]) * (fTemp827 + (3.0f - fTemp824)))) + (fVec155[((IOTA - iTemp826) & 8191)] + fTemp828));
			float fTemp829 = ((0.5f * (fTemp807 * fRec200[2])) + (0.25f * (fTemp808 * (fRec200[1] + fRec200[3]))));
			float fTemp830 = (3.14159274f * (fRec34[0] * fTemp829));
			float fTemp831 = sinf(fTemp830);
			float fTemp832 = cosf(fTemp830);
			fRec201[0] = ((fRec201[1] * (0.0f - fTemp831)) + (fTemp829 * fTemp832));
			float fTemp833 = (((((fTemp829 * fTemp831) + (fRec201[1] * fTemp832)) * powf(0.00100000005f, (fSlow22 / fTemp762))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec157[(IOTA & 8191)] = fTemp833;
			float fTemp834 = (fSlow23 / fTemp761);
			float fTemp835 = (fTemp834 + -2.49999499f);
			int iTemp836 = min(4096, max(0, int(fTemp835)));
			float fTemp837 = floorf(fTemp835);
			float fTemp838 = ((fVec157[((IOTA - iTemp836) & 8191)] - fVec157[((IOTA - (iTemp836 + 1)) & 8191)]) * (fTemp837 + (2.0f - fTemp834)));
			fVec158[0] = fTemp838;
			fRec200[0] = ((0.5f * ((fTemp838 - fVec158[1]) * (fTemp837 + (3.0f - fTemp834)))) + (fVec157[((IOTA - iTemp836) & 8191)] + fTemp838));
			float fTemp839 = ((0.5f * (fTemp807 * fRec202[2])) + (0.25f * (fTemp808 * (fRec202[1] + fRec202[3]))));
			float fTemp840 = (3.14159274f * (fRec34[0] * fTemp839));
			float fTemp841 = sinf(fTemp840);
			float fTemp842 = cosf(fTemp840);
			fRec203[0] = ((fRec203[1] * (0.0f - fTemp841)) + (fTemp839 * fTemp842));
			float fTemp843 = (((((fTemp839 * fTemp841) + (fRec203[1] * fTemp842)) * powf(0.00100000005f, (fSlow22 / fTemp750))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec159[(IOTA & 8191)] = fTemp843;
			float fTemp844 = (fSlow23 / fTemp749);
			float fTemp845 = (fTemp844 + -2.49999499f);
			int iTemp846 = min(4096, max(0, int(fTemp845)));
			float fTemp847 = floorf(fTemp845);
			float fTemp848 = ((fVec159[((IOTA - iTemp846) & 8191)] - fVec159[((IOTA - (iTemp846 + 1)) & 8191)]) * (fTemp847 + (2.0f - fTemp844)));
			fVec160[0] = fTemp848;
			fRec202[0] = ((0.5f * ((fTemp848 - fVec160[1]) * (fTemp847 + (3.0f - fTemp844)))) + (fVec159[((IOTA - iTemp846) & 8191)] + fTemp848));
			float fTemp849 = ((0.5f * (fTemp807 * fRec204[2])) + (0.25f * (fTemp808 * (fRec204[1] + fRec204[3]))));
			float fTemp850 = (3.14159274f * (fRec34[0] * fTemp849));
			float fTemp851 = sinf(fTemp850);
			float fTemp852 = cosf(fTemp850);
			fRec205[0] = ((fRec205[1] * (0.0f - fTemp851)) + (fTemp849 * fTemp852));
			float fTemp853 = (((((fTemp849 * fTemp851) + (fRec205[1] * fTemp852)) * powf(0.00100000005f, (fSlow22 / fTemp738))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec161[(IOTA & 8191)] = fTemp853;
			float fTemp854 = (fSlow23 / fTemp737);
			float fTemp855 = (fTemp854 + -2.49999499f);
			int iTemp856 = min(4096, max(0, int(fTemp855)));
			float fTemp857 = floorf(fTemp855);
			float fTemp858 = ((fVec161[((IOTA - iTemp856) & 8191)] - fVec161[((IOTA - (iTemp856 + 1)) & 8191)]) * (fTemp857 + (2.0f - fTemp854)));
			fVec162[0] = fTemp858;
			fRec204[0] = ((0.5f * ((fTemp858 - fVec162[1]) * (fTemp857 + (3.0f - fTemp854)))) + (fVec161[((IOTA - iTemp856) & 8191)] + fTemp858));
			float fTemp859 = ((0.5f * (fTemp807 * fRec206[2])) + (0.25f * (fTemp808 * (fRec206[1] + fRec206[3]))));
			float fTemp860 = (3.14159274f * (fRec34[0] * fTemp859));
			float fTemp861 = sinf(fTemp860);
			float fTemp862 = cosf(fTemp860);
			fRec207[0] = ((fRec207[1] * (0.0f - fTemp861)) + (fTemp859 * fTemp862));
			float fTemp863 = (((((fTemp859 * fTemp861) + (fRec207[1] * fTemp862)) * powf(0.00100000005f, (fSlow22 / fTemp728))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec163[(IOTA & 8191)] = fTemp863;
			float fTemp864 = (fSlow23 / fTemp720);
			float fTemp865 = (fTemp864 + -2.49999499f);
			int iTemp866 = min(4096, max(0, int(fTemp865)));
			float fTemp867 = floorf(fTemp865);
			float fTemp868 = ((fVec163[((IOTA - iTemp866) & 8191)] - fVec163[((IOTA - (iTemp866 + 1)) & 8191)]) * (fTemp867 + (2.0f - fTemp864)));
			fVec164[0] = fTemp868;
			fRec206[0] = ((0.5f * ((fTemp868 - fVec164[1]) * (fTemp867 + (3.0f - fTemp864)))) + (fVec163[((IOTA - iTemp866) & 8191)] + fTemp868));
			float fTemp869 = ((0.5f * (fTemp807 * fRec208[2])) + (0.25f * (fTemp808 * (fRec208[1] + fRec208[3]))));
			float fTemp870 = (3.14159274f * (fRec34[0] * fTemp869));
			float fTemp871 = sinf(fTemp870);
			float fTemp872 = cosf(fTemp870);
			fRec209[0] = ((fRec209[1] * (0.0f - fTemp871)) + (fTemp869 * fTemp872));
			float fTemp873 = (((((fTemp869 * fTemp871) + (fRec209[1] * fTemp872)) * powf(0.00100000005f, (fSlow22 / fTemp713))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec165[(IOTA & 8191)] = fTemp873;
			float fTemp874 = (fSlow23 / fTemp712);
			float fTemp875 = (fTemp874 + -2.49999499f);
			int iTemp876 = min(4096, max(0, int(fTemp875)));
			float fTemp877 = floorf(fTemp875);
			float fTemp878 = ((fVec165[((IOTA - iTemp876) & 8191)] - fVec165[((IOTA - (iTemp876 + 1)) & 8191)]) * (fTemp877 + (2.0f - fTemp874)));
			fVec166[0] = fTemp878;
			fRec208[0] = ((0.5f * ((fTemp878 - fVec166[1]) * (fTemp877 + (3.0f - fTemp874)))) + (fVec165[((IOTA - iTemp876) & 8191)] + fTemp878));
			float fTemp879 = ((0.5f * (fTemp807 * fRec210[2])) + (0.25f * (fTemp808 * (fRec210[1] + fRec210[3]))));
			float fTemp880 = (3.14159274f * (fRec34[0] * fTemp879));
			float fTemp881 = sinf(fTemp880);
			float fTemp882 = cosf(fTemp880);
			fRec211[0] = ((fRec211[1] * (0.0f - fTemp881)) + (fTemp879 * fTemp882));
			float fTemp883 = (((((fTemp879 * fTemp881) + (fRec211[1] * fTemp882)) * powf(0.00100000005f, (fSlow22 / fTemp701))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec167[(IOTA & 8191)] = fTemp883;
			float fTemp884 = (fSlow23 / fTemp700);
			float fTemp885 = (fTemp884 + -2.49999499f);
			int iTemp886 = min(4096, max(0, int(fTemp885)));
			float fTemp887 = floorf(fTemp885);
			float fTemp888 = ((fVec167[((IOTA - iTemp886) & 8191)] - fVec167[((IOTA - (iTemp886 + 1)) & 8191)]) * (fTemp887 + (2.0f - fTemp884)));
			fVec168[0] = fTemp888;
			fRec210[0] = ((0.5f * ((fTemp888 - fVec168[1]) * (fTemp887 + (3.0f - fTemp884)))) + (fVec167[((IOTA - iTemp886) & 8191)] + fTemp888));
			float fTemp889 = ((0.5f * (fTemp807 * fRec212[2])) + (0.25f * (fTemp808 * (fRec212[1] + fRec212[3]))));
			float fTemp890 = (3.14159274f * (fRec34[0] * fTemp889));
			float fTemp891 = sinf(fTemp890);
			float fTemp892 = cosf(fTemp890);
			fRec213[0] = ((fRec213[1] * (0.0f - fTemp891)) + (fTemp889 * fTemp892));
			float fTemp893 = (((((fTemp889 * fTemp891) + (fRec213[1] * fTemp892)) * powf(0.00100000005f, (fSlow22 / fTemp786))) + fTemp687) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp16)))))) & 8191)]);
			fVec169[(IOTA & 8191)] = fTemp893;
			float fTemp894 = (fSlow23 / fTemp785);
			float fTemp895 = (fTemp894 + -2.49999499f);
			int iTemp896 = min(4096, max(0, int(fTemp895)));
			float fTemp897 = floorf(fTemp895);
			float fTemp898 = ((fVec169[((IOTA - iTemp896) & 8191)] - fVec169[((IOTA - (iTemp896 + 1)) & 8191)]) * (fTemp897 + (2.0f - fTemp894)));
			fVec170[0] = fTemp898;
			fRec212[0] = ((0.5f * ((fTemp898 - fVec170[1]) * (fTemp897 + (3.0f - fTemp894)))) + (fVec169[((IOTA - iTemp896) & 8191)] + fTemp898));
			float fTemp899 = ((((((((fRec196[0] + fRec198[0]) + fRec200[0]) + fRec202[0]) + fRec204[0]) + fRec206[0]) + fRec208[0]) + fRec210[0]) + fRec212[0]);
			fVec171[0] = fTemp899;
			fRec195[0] = ((fTemp899 + (0.995000005f * fRec195[1])) - fVec171[1]);
			fRec7[0] = (0.00999999978f * fRec195[0]);
			fRec214[0] = (fSlow39 + (fConst19 * fRec214[1]));
			float fTemp900 = (0.5f * (1.0f - fRec214[0]));
			float fTemp901 = (0.166666672f * fRec214[0]);
			float fTemp902 = (0.5f * (fRec214[0] + 1.0f));
			float fTemp903 = (0.166666672f * fRec214[0]);
			output0[i] = FAUSTFLOAT((((fRec0 + fRec1[0]) * (sqrtf((1.0f - fTemp900)) + sqrtf(fTemp900))) + (((fRec4 + fRec5[0]) * (sqrtf((0.5f - fTemp901)) + sqrtf((fTemp901 + 0.5f)))) + (((fRec6 + fRec7[0]) * (sqrtf((1.0f - fTemp902)) + sqrtf(fTemp902))) + ((fRec2 + fRec3[0]) * (sqrtf((fTemp903 + 0.5f)) + sqrtf((0.5f - fTemp903))))))));
			fRec13[1] = fRec13[0];
			fVec0[1] = fVec0[0];
			fRec12[1] = fRec12[0];
			fRec14[1] = fRec14[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fVec1[1] = fVec1[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fVec2[1] = fVec2[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			iRec24[1] = iRec24[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec23[j0] = fRec23[(j0 - 1)];
				
			}
			fRec25[1] = fRec25[0];
			fRec30[1] = fRec30[0];
			fVec3[1] = fVec3[0];
			fRec31[1] = fRec31[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fVec4[1] = fVec4[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			IOTA = (IOTA + 1);
			fRec32[1] = fRec32[0];
			fRec33[1] = fRec33[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fVec6[1] = fVec6[0];
			fRec37[1] = fRec37[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fVec8[1] = fVec8[0];
			for (int j1 = 3; (j1 > 0); j1 = (j1 - 1)) {
				fRec9[j1] = fRec9[(j1 - 1)];
				
			}
			fRec41[1] = fRec41[0];
			fVec10[1] = fVec10[0];
			for (int j2 = 3; (j2 > 0); j2 = (j2 - 1)) {
				fRec40[j2] = fRec40[(j2 - 1)];
				
			}
			fRec43[1] = fRec43[0];
			fVec12[1] = fVec12[0];
			for (int j3 = 3; (j3 > 0); j3 = (j3 - 1)) {
				fRec42[j3] = fRec42[(j3 - 1)];
				
			}
			fRec45[1] = fRec45[0];
			fVec14[1] = fVec14[0];
			for (int j4 = 3; (j4 > 0); j4 = (j4 - 1)) {
				fRec44[j4] = fRec44[(j4 - 1)];
				
			}
			fRec47[1] = fRec47[0];
			fVec16[1] = fVec16[0];
			for (int j5 = 3; (j5 > 0); j5 = (j5 - 1)) {
				fRec46[j5] = fRec46[(j5 - 1)];
				
			}
			fRec49[1] = fRec49[0];
			fVec18[1] = fVec18[0];
			for (int j6 = 3; (j6 > 0); j6 = (j6 - 1)) {
				fRec48[j6] = fRec48[(j6 - 1)];
				
			}
			fRec51[1] = fRec51[0];
			fVec20[1] = fVec20[0];
			for (int j7 = 3; (j7 > 0); j7 = (j7 - 1)) {
				fRec50[j7] = fRec50[(j7 - 1)];
				
			}
			fRec53[1] = fRec53[0];
			fVec22[1] = fVec22[0];
			for (int j8 = 3; (j8 > 0); j8 = (j8 - 1)) {
				fRec52[j8] = fRec52[(j8 - 1)];
				
			}
			fRec55[1] = fRec55[0];
			fVec24[1] = fVec24[0];
			for (int j9 = 3; (j9 > 0); j9 = (j9 - 1)) {
				fRec54[j9] = fRec54[(j9 - 1)];
				
			}
			fVec25[1] = fVec25[0];
			fRec8[1] = fRec8[0];
			fRec58[1] = fRec58[0];
			fVec27[1] = fVec27[0];
			for (int j10 = 3; (j10 > 0); j10 = (j10 - 1)) {
				fRec57[j10] = fRec57[(j10 - 1)];
				
			}
			fRec60[1] = fRec60[0];
			fVec29[1] = fVec29[0];
			for (int j11 = 3; (j11 > 0); j11 = (j11 - 1)) {
				fRec59[j11] = fRec59[(j11 - 1)];
				
			}
			fRec62[1] = fRec62[0];
			fVec31[1] = fVec31[0];
			for (int j12 = 3; (j12 > 0); j12 = (j12 - 1)) {
				fRec61[j12] = fRec61[(j12 - 1)];
				
			}
			fRec64[1] = fRec64[0];
			fVec33[1] = fVec33[0];
			for (int j13 = 3; (j13 > 0); j13 = (j13 - 1)) {
				fRec63[j13] = fRec63[(j13 - 1)];
				
			}
			fRec66[1] = fRec66[0];
			fVec35[1] = fVec35[0];
			for (int j14 = 3; (j14 > 0); j14 = (j14 - 1)) {
				fRec65[j14] = fRec65[(j14 - 1)];
				
			}
			fRec68[1] = fRec68[0];
			fVec37[1] = fVec37[0];
			for (int j15 = 3; (j15 > 0); j15 = (j15 - 1)) {
				fRec67[j15] = fRec67[(j15 - 1)];
				
			}
			fRec70[1] = fRec70[0];
			fVec39[1] = fVec39[0];
			for (int j16 = 3; (j16 > 0); j16 = (j16 - 1)) {
				fRec69[j16] = fRec69[(j16 - 1)];
				
			}
			fRec72[1] = fRec72[0];
			fVec41[1] = fVec41[0];
			for (int j17 = 3; (j17 > 0); j17 = (j17 - 1)) {
				fRec71[j17] = fRec71[(j17 - 1)];
				
			}
			fRec74[1] = fRec74[0];
			fVec43[1] = fVec43[0];
			for (int j18 = 3; (j18 > 0); j18 = (j18 - 1)) {
				fRec73[j18] = fRec73[(j18 - 1)];
				
			}
			fVec44[1] = fVec44[0];
			fRec56[1] = fRec56[0];
			fRec1[1] = fRec1[0];
			fVec45[1] = fVec45[0];
			fRec79[1] = fRec79[0];
			fRec80[1] = fRec80[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fVec46[1] = fVec46[0];
			fRec83[1] = fRec83[0];
			fRec84[1] = fRec84[0];
			fVec47[1] = fVec47[0];
			fRec82[1] = fRec82[0];
			fRec81[1] = fRec81[0];
			fRec85[1] = fRec85[0];
			fRec86[1] = fRec86[0];
			fVec49[1] = fVec49[0];
			fRec87[1] = fRec87[0];
			fRec88[1] = fRec88[0];
			fVec51[1] = fVec51[0];
			for (int j19 = 3; (j19 > 0); j19 = (j19 - 1)) {
				fRec76[j19] = fRec76[(j19 - 1)];
				
			}
			fRec90[1] = fRec90[0];
			fVec53[1] = fVec53[0];
			for (int j20 = 3; (j20 > 0); j20 = (j20 - 1)) {
				fRec89[j20] = fRec89[(j20 - 1)];
				
			}
			fRec92[1] = fRec92[0];
			fVec55[1] = fVec55[0];
			for (int j21 = 3; (j21 > 0); j21 = (j21 - 1)) {
				fRec91[j21] = fRec91[(j21 - 1)];
				
			}
			fRec94[1] = fRec94[0];
			fVec57[1] = fVec57[0];
			for (int j22 = 3; (j22 > 0); j22 = (j22 - 1)) {
				fRec93[j22] = fRec93[(j22 - 1)];
				
			}
			fRec96[1] = fRec96[0];
			fVec59[1] = fVec59[0];
			for (int j23 = 3; (j23 > 0); j23 = (j23 - 1)) {
				fRec95[j23] = fRec95[(j23 - 1)];
				
			}
			fRec98[1] = fRec98[0];
			fVec61[1] = fVec61[0];
			for (int j24 = 3; (j24 > 0); j24 = (j24 - 1)) {
				fRec97[j24] = fRec97[(j24 - 1)];
				
			}
			fRec100[1] = fRec100[0];
			fVec63[1] = fVec63[0];
			for (int j25 = 3; (j25 > 0); j25 = (j25 - 1)) {
				fRec99[j25] = fRec99[(j25 - 1)];
				
			}
			fRec102[1] = fRec102[0];
			fVec65[1] = fVec65[0];
			for (int j26 = 3; (j26 > 0); j26 = (j26 - 1)) {
				fRec101[j26] = fRec101[(j26 - 1)];
				
			}
			fRec104[1] = fRec104[0];
			fVec67[1] = fVec67[0];
			for (int j27 = 3; (j27 > 0); j27 = (j27 - 1)) {
				fRec103[j27] = fRec103[(j27 - 1)];
				
			}
			fVec68[1] = fVec68[0];
			fRec75[1] = fRec75[0];
			fRec107[1] = fRec107[0];
			fVec70[1] = fVec70[0];
			for (int j28 = 3; (j28 > 0); j28 = (j28 - 1)) {
				fRec106[j28] = fRec106[(j28 - 1)];
				
			}
			fRec109[1] = fRec109[0];
			fVec72[1] = fVec72[0];
			for (int j29 = 3; (j29 > 0); j29 = (j29 - 1)) {
				fRec108[j29] = fRec108[(j29 - 1)];
				
			}
			fRec111[1] = fRec111[0];
			fVec74[1] = fVec74[0];
			for (int j30 = 3; (j30 > 0); j30 = (j30 - 1)) {
				fRec110[j30] = fRec110[(j30 - 1)];
				
			}
			fRec113[1] = fRec113[0];
			fVec76[1] = fVec76[0];
			for (int j31 = 3; (j31 > 0); j31 = (j31 - 1)) {
				fRec112[j31] = fRec112[(j31 - 1)];
				
			}
			fRec115[1] = fRec115[0];
			fVec78[1] = fVec78[0];
			for (int j32 = 3; (j32 > 0); j32 = (j32 - 1)) {
				fRec114[j32] = fRec114[(j32 - 1)];
				
			}
			fRec117[1] = fRec117[0];
			fVec80[1] = fVec80[0];
			for (int j33 = 3; (j33 > 0); j33 = (j33 - 1)) {
				fRec116[j33] = fRec116[(j33 - 1)];
				
			}
			fRec119[1] = fRec119[0];
			fVec82[1] = fVec82[0];
			for (int j34 = 3; (j34 > 0); j34 = (j34 - 1)) {
				fRec118[j34] = fRec118[(j34 - 1)];
				
			}
			fRec121[1] = fRec121[0];
			fVec84[1] = fVec84[0];
			for (int j35 = 3; (j35 > 0); j35 = (j35 - 1)) {
				fRec120[j35] = fRec120[(j35 - 1)];
				
			}
			fRec123[1] = fRec123[0];
			fVec86[1] = fVec86[0];
			for (int j36 = 3; (j36 > 0); j36 = (j36 - 1)) {
				fRec122[j36] = fRec122[(j36 - 1)];
				
			}
			fVec87[1] = fVec87[0];
			fRec105[1] = fRec105[0];
			fRec3[1] = fRec3[0];
			fVec88[1] = fVec88[0];
			fRec128[1] = fRec128[0];
			fRec129[1] = fRec129[0];
			fVec89[1] = fVec89[0];
			fRec127[1] = fRec127[0];
			fRec126[1] = fRec126[0];
			fRec130[1] = fRec130[0];
			fRec131[1] = fRec131[0];
			fVec91[1] = fVec91[0];
			fRec132[1] = fRec132[0];
			fRec133[1] = fRec133[0];
			fVec93[1] = fVec93[0];
			for (int j37 = 3; (j37 > 0); j37 = (j37 - 1)) {
				fRec125[j37] = fRec125[(j37 - 1)];
				
			}
			fRec135[1] = fRec135[0];
			fVec95[1] = fVec95[0];
			for (int j38 = 3; (j38 > 0); j38 = (j38 - 1)) {
				fRec134[j38] = fRec134[(j38 - 1)];
				
			}
			fRec137[1] = fRec137[0];
			fVec97[1] = fVec97[0];
			for (int j39 = 3; (j39 > 0); j39 = (j39 - 1)) {
				fRec136[j39] = fRec136[(j39 - 1)];
				
			}
			fRec139[1] = fRec139[0];
			fVec99[1] = fVec99[0];
			for (int j40 = 3; (j40 > 0); j40 = (j40 - 1)) {
				fRec138[j40] = fRec138[(j40 - 1)];
				
			}
			fRec141[1] = fRec141[0];
			fVec101[1] = fVec101[0];
			for (int j41 = 3; (j41 > 0); j41 = (j41 - 1)) {
				fRec140[j41] = fRec140[(j41 - 1)];
				
			}
			fRec143[1] = fRec143[0];
			fVec103[1] = fVec103[0];
			for (int j42 = 3; (j42 > 0); j42 = (j42 - 1)) {
				fRec142[j42] = fRec142[(j42 - 1)];
				
			}
			fRec145[1] = fRec145[0];
			fVec105[1] = fVec105[0];
			for (int j43 = 3; (j43 > 0); j43 = (j43 - 1)) {
				fRec144[j43] = fRec144[(j43 - 1)];
				
			}
			fRec147[1] = fRec147[0];
			fVec107[1] = fVec107[0];
			for (int j44 = 3; (j44 > 0); j44 = (j44 - 1)) {
				fRec146[j44] = fRec146[(j44 - 1)];
				
			}
			fRec149[1] = fRec149[0];
			fVec109[1] = fVec109[0];
			for (int j45 = 3; (j45 > 0); j45 = (j45 - 1)) {
				fRec148[j45] = fRec148[(j45 - 1)];
				
			}
			fVec110[1] = fVec110[0];
			fRec124[1] = fRec124[0];
			fRec152[1] = fRec152[0];
			fVec112[1] = fVec112[0];
			for (int j46 = 3; (j46 > 0); j46 = (j46 - 1)) {
				fRec151[j46] = fRec151[(j46 - 1)];
				
			}
			fRec154[1] = fRec154[0];
			fVec114[1] = fVec114[0];
			for (int j47 = 3; (j47 > 0); j47 = (j47 - 1)) {
				fRec153[j47] = fRec153[(j47 - 1)];
				
			}
			fRec156[1] = fRec156[0];
			fVec116[1] = fVec116[0];
			for (int j48 = 3; (j48 > 0); j48 = (j48 - 1)) {
				fRec155[j48] = fRec155[(j48 - 1)];
				
			}
			fRec158[1] = fRec158[0];
			fVec118[1] = fVec118[0];
			for (int j49 = 3; (j49 > 0); j49 = (j49 - 1)) {
				fRec157[j49] = fRec157[(j49 - 1)];
				
			}
			fRec160[1] = fRec160[0];
			fVec120[1] = fVec120[0];
			for (int j50 = 3; (j50 > 0); j50 = (j50 - 1)) {
				fRec159[j50] = fRec159[(j50 - 1)];
				
			}
			fRec162[1] = fRec162[0];
			fVec122[1] = fVec122[0];
			for (int j51 = 3; (j51 > 0); j51 = (j51 - 1)) {
				fRec161[j51] = fRec161[(j51 - 1)];
				
			}
			fRec164[1] = fRec164[0];
			fVec124[1] = fVec124[0];
			for (int j52 = 3; (j52 > 0); j52 = (j52 - 1)) {
				fRec163[j52] = fRec163[(j52 - 1)];
				
			}
			fRec166[1] = fRec166[0];
			fVec126[1] = fVec126[0];
			for (int j53 = 3; (j53 > 0); j53 = (j53 - 1)) {
				fRec165[j53] = fRec165[(j53 - 1)];
				
			}
			fRec168[1] = fRec168[0];
			fVec128[1] = fVec128[0];
			for (int j54 = 3; (j54 > 0); j54 = (j54 - 1)) {
				fRec167[j54] = fRec167[(j54 - 1)];
				
			}
			fVec129[1] = fVec129[0];
			fRec150[1] = fRec150[0];
			fRec5[1] = fRec5[0];
			fVec130[1] = fVec130[0];
			fRec173[1] = fRec173[0];
			fRec174[1] = fRec174[0];
			fVec131[1] = fVec131[0];
			fRec172[1] = fRec172[0];
			fRec171[1] = fRec171[0];
			fRec175[1] = fRec175[0];
			fRec176[1] = fRec176[0];
			fVec133[1] = fVec133[0];
			fRec177[1] = fRec177[0];
			fRec178[1] = fRec178[0];
			fVec135[1] = fVec135[0];
			for (int j55 = 3; (j55 > 0); j55 = (j55 - 1)) {
				fRec170[j55] = fRec170[(j55 - 1)];
				
			}
			fRec180[1] = fRec180[0];
			fVec137[1] = fVec137[0];
			for (int j56 = 3; (j56 > 0); j56 = (j56 - 1)) {
				fRec179[j56] = fRec179[(j56 - 1)];
				
			}
			fRec182[1] = fRec182[0];
			fVec139[1] = fVec139[0];
			for (int j57 = 3; (j57 > 0); j57 = (j57 - 1)) {
				fRec181[j57] = fRec181[(j57 - 1)];
				
			}
			fRec184[1] = fRec184[0];
			fVec141[1] = fVec141[0];
			for (int j58 = 3; (j58 > 0); j58 = (j58 - 1)) {
				fRec183[j58] = fRec183[(j58 - 1)];
				
			}
			fRec186[1] = fRec186[0];
			fVec143[1] = fVec143[0];
			for (int j59 = 3; (j59 > 0); j59 = (j59 - 1)) {
				fRec185[j59] = fRec185[(j59 - 1)];
				
			}
			fRec188[1] = fRec188[0];
			fVec145[1] = fVec145[0];
			for (int j60 = 3; (j60 > 0); j60 = (j60 - 1)) {
				fRec187[j60] = fRec187[(j60 - 1)];
				
			}
			fRec190[1] = fRec190[0];
			fVec147[1] = fVec147[0];
			for (int j61 = 3; (j61 > 0); j61 = (j61 - 1)) {
				fRec189[j61] = fRec189[(j61 - 1)];
				
			}
			fRec192[1] = fRec192[0];
			fVec149[1] = fVec149[0];
			for (int j62 = 3; (j62 > 0); j62 = (j62 - 1)) {
				fRec191[j62] = fRec191[(j62 - 1)];
				
			}
			fRec194[1] = fRec194[0];
			fVec151[1] = fVec151[0];
			for (int j63 = 3; (j63 > 0); j63 = (j63 - 1)) {
				fRec193[j63] = fRec193[(j63 - 1)];
				
			}
			fVec152[1] = fVec152[0];
			fRec169[1] = fRec169[0];
			fRec197[1] = fRec197[0];
			fVec154[1] = fVec154[0];
			for (int j64 = 3; (j64 > 0); j64 = (j64 - 1)) {
				fRec196[j64] = fRec196[(j64 - 1)];
				
			}
			fRec199[1] = fRec199[0];
			fVec156[1] = fVec156[0];
			for (int j65 = 3; (j65 > 0); j65 = (j65 - 1)) {
				fRec198[j65] = fRec198[(j65 - 1)];
				
			}
			fRec201[1] = fRec201[0];
			fVec158[1] = fVec158[0];
			for (int j66 = 3; (j66 > 0); j66 = (j66 - 1)) {
				fRec200[j66] = fRec200[(j66 - 1)];
				
			}
			fRec203[1] = fRec203[0];
			fVec160[1] = fVec160[0];
			for (int j67 = 3; (j67 > 0); j67 = (j67 - 1)) {
				fRec202[j67] = fRec202[(j67 - 1)];
				
			}
			fRec205[1] = fRec205[0];
			fVec162[1] = fVec162[0];
			for (int j68 = 3; (j68 > 0); j68 = (j68 - 1)) {
				fRec204[j68] = fRec204[(j68 - 1)];
				
			}
			fRec207[1] = fRec207[0];
			fVec164[1] = fVec164[0];
			for (int j69 = 3; (j69 > 0); j69 = (j69 - 1)) {
				fRec206[j69] = fRec206[(j69 - 1)];
				
			}
			fRec209[1] = fRec209[0];
			fVec166[1] = fVec166[0];
			for (int j70 = 3; (j70 > 0); j70 = (j70 - 1)) {
				fRec208[j70] = fRec208[(j70 - 1)];
				
			}
			fRec211[1] = fRec211[0];
			fVec168[1] = fVec168[0];
			for (int j71 = 3; (j71 > 0); j71 = (j71 - 1)) {
				fRec210[j71] = fRec210[(j71 - 1)];
				
			}
			fRec213[1] = fRec213[0];
			fVec170[1] = fVec170[0];
			for (int j72 = 3; (j72 > 0); j72 = (j72 - 1)) {
				fRec212[j72] = fRec212[(j72 - 1)];
				
			}
			fVec171[1] = fVec171[0];
			fRec195[1] = fRec195[0];
			fRec7[1] = fRec7[0];
			fRec214[1] = fRec214[0];
			
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
static t_class* tambura_faust_class;
#define FAUST_EXT t_faust_tambura
#define FAUST_EXT_CLASS tambura_faust_class
// clang-format on

struct t_faust_tambura {
    t_object x_obj;
#ifdef __MINGW32__
    /* This seems to be necessary as some as yet undetermined Pd routine seems
     to write past the end of x_obj on Windows. */
    int fence; /* dummy field (not used) */
#endif
    tambura* dsp;
    PdUI<UI>* ui;
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
    t_faust_tambura* x = reinterpret_cast<t_faust_tambura*>(w[1]);
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

static void tambura_faust_dsp(t_faust_tambura* x, t_signal** sp)
{
    const int n = sp[0]->s_n;
    const int sr = static_cast<int>(sp[0]->s_sr);

    if (x->rate <= 0) {
        /* default sample rate is whatever Pd tells us */
        PdUI<UI>* ui = x->ui;
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

static void tambura_dump_to_console(t_faust_tambura* x)
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

static void tambura_faust_any(t_faust_tambura* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!x->dsp)
        return;

    PdUI<UI>* ui = x->ui;
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

static void faust_free_dsp(t_faust_tambura* x)
{
    delete x->dsp;
    x->dsp = NULL;
}

static void faust_free_ui(t_faust_tambura* x)
{
    delete x->ui;
    x->ui = NULL;
}

static void faust_free_inputs(t_faust_tambura* x)
{
    if (x->inputs)
        free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust_tambura* x)
{
    if (x->outputs)
        free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust_tambura* x)
{
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i])
                free(x->buf[i]);
        }

        free(x->buf);
    }
}

static void tambura_faust_free(t_faust_tambura* x)
{
    faust_free_dsp(x);
    faust_free_ui(x);
    faust_free_inputs(x);
    faust_free_outputs(x);
    faust_free_buf(x);
}

static bool faust_init_inputs(t_faust_tambura* x)
{
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if (x->inputs == NULL) {
            error("[%s] faust_init_inputs failed", sym(tambura));
            return false;
        }
    }

    // creating sound inlets (except the first one!)
    for (int i = 0; i < (x->n_in - 1); i++) {
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    return true;
}

static bool faust_init_outputs(t_faust_tambura* x, bool info_outlet)
{
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->outputs == NULL) {
            error("[%s] faust_init_outputs failed", sym(tambura));
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->buf == NULL) {
            error("[%s] faust_init_outputs failed", sym(tambura));
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

static bool faust_new_internal(t_faust_tambura* x, const std::string& objId = "", bool info_outlet = true)
{
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new tambura();
    x->ui = new PdUI<UI>(sym(tambura), objId);

    if (!faust_init_inputs(x)) {
        tambura_faust_free(x);
        return false;
    }

    if (!faust_init_outputs(x, info_outlet)) {
        tambura_faust_free(x);
        return false;
    }

    x->dsp->init(sr);
    x->dsp->buildUserInterface(x->ui);

    return true;
}

/**
 * find nth element that satisfies given predicate
 * @first - first element of sequence
 * @last - pointer behind last element of sequence
 * @Nth - searched element index
 * @pred - predicate
 * @return pointer to found element or pointer to @bold last, if not found
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
    t_faust_tambura* x_;
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
    PdArgParser(t_faust_tambura* x, int argc, t_atom* argv, bool info_outlet = true)
        : x_(x)
        , argc_(argc)
        , argv_(argv)
        , control_outlet_(info_outlet)
    {
        const char* id = NULL;
        std::string objId;
        if (get_nth_symbol_arg(argc_, argv_, 1, &id))
            objId = id;

        // init error
        if (!faust_new_internal(x, objId, control_outlet_)) {
            this->x_ = NULL;
        }

        std::deque<ceammc::AtomList> props = ceammc::AtomList(argc_, argv).properties();
        for (size_t i = 0; i < props.size(); i++) {
            ceammc::AtomList& p = props[i];
            // skip empty property
            if(p.size() < 2)
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
    void signalFloatArg(const char* name, int pos)
    {
        // object was not created
        if (!this->x_)
            return;

        t_float arg = 0;
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &arg))
            pd_float(reinterpret_cast<t_pd*>(this->x_), arg);
    }

    t_faust_tambura* pd_obj()
    {
        return this->x_;
    }
};

static void* tambura_faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s)
{
    tambura_faust_class = class_new(s, reinterpret_cast<t_newmethod>(tambura_faust_new),
        reinterpret_cast<t_method>(tambura_faust_free),
        sizeof(t_faust_tambura),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);
    class_addmethod(tambura_faust_class, nullfn, &s_signal, A_NULL);
    class_addmethod(tambura_faust_class, reinterpret_cast<t_method>(tambura_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(tambura_faust_class, reinterpret_cast<t_method>(tambura_dump_to_console), gensym("dump"), A_NULL);
    CLASS_MAINSIGNALIN(tambura_faust_class, t_faust_tambura, f);
    class_addanything(tambura_faust_class, tambura_faust_any);
}

#define EXTERNAL_NEW void* tambura_faust_new(t_symbol*, int argc, t_atom* argv)

#define EXTERNAL_SIMPLE_NEW()                                                           \
    static void* tambura_faust_new(t_symbol*, int argc, t_atom* argv)                     \
    {                                                                                   \
        t_faust_tambura* x = reinterpret_cast<t_faust_tambura*>(pd_new(tambura_faust_class)); \
        PdArgParser p(x, argc, argv, false);                                            \
        return p.pd_obj();                                                              \
    }

#define EXTERNAL_SETUP(MOD)                        \
    extern "C" void setup_##MOD##0x2etambura_tilde() \
    {                                              \
        internal_setup(gensym(#MOD ".tambura~"));    \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);

#endif
