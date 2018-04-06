/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "Tambura"
version: "1.0"
Code generated with Faust 2.5.31 (https://faust.grame.fr)
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
	float fConst21;
	float fRec14[2];
	float fConst22;
	float fConst23;
	float fRec11[3];
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
	FAUSTFLOAT fCheckbox0;
	float fConst29;
	FAUSTFLOAT fHslider1;
	float fRec27[2];
	FAUSTFLOAT fButton0;
	float fVec3[2];
	float fConst30;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	float fRec28[2];
	float fRec25[2];
	float fRec26[2];
	float fVec4[2];
	float fConst31;
	FAUSTFLOAT fHslider4;
	float fConst32;
	float fRec24[2];
	float fRec23[2];
	FAUSTFLOAT fHslider5;
	float fRec29[2];
	int iRec31[2];
	float fRec30[4];
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
	float fVec47[2];
	float fConst38;
	float fRec83[2];
	float fRec84[2];
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
		fConst8 = (fConst1 / sinf((15707.9629f * fConst1)));
		fConst9 = (1256.63708f * fConst8);
		fConst10 = tanf((7853.98145f / fConst0));
		fConst11 = (1.0f / fConst10);
		fConst12 = (1.0f / (((fConst9 + fConst11) / fConst10) + 1.0f));
		fConst13 = (6298.10449f * fConst8);
		fConst14 = (((fConst13 + fConst11) / fConst10) + 1.0f);
		fConst15 = (1.0f / tanf((15707.9629f / fConst0)));
		fConst16 = (fConst15 + 1.0f);
		fConst17 = (0.0f - ((1.0f - fConst15) / fConst16));
		fConst18 = (1.0f / fConst16);
		fConst19 = expf((0.0f - (20.0f / fConst0)));
		fConst20 = (1.0f - fConst19);
		fConst21 = (0.0f - fConst15);
		fConst22 = (2.0f * (1.0f - (1.0f / tambura_faustpower2_f(fConst10))));
		fConst23 = (((fConst11 - fConst9) / fConst10) + 1.0f);
		fConst24 = (((fConst11 - fConst13) / fConst10) + 1.0f);
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
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.10000000000000001f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(1.0f);
		fHslider3 = FAUSTFLOAT(36.0f);
		fHslider4 = FAUSTFLOAT(0.070000000000000007f);
		fHslider5 = FAUSTFLOAT(0.13f);
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
			fRec27[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fVec3[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec28[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec25[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec26[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fVec4[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec24[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec23[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec29[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			iRec31[l25] = 0;
			
		}
		for (int l26 = 0; (l26 < 4); l26 = (l26 + 1)) {
			fRec30[l26] = 0.0f;
			
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
			fVec47[l119] = 0.0f;
			
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec83[l120] = 0.0f;
			
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec84[l121] = 0.0f;
			
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
		ui_interface->declare(&fHslider4, "scale", "exp");
		ui_interface->addHorizontalSlider("attack_time", &fHslider4, 0.0700000003f, 0.0f, 0.5f, 0.00999999978f);
		ui_interface->addCheckButton("auto_pluck", &fCheckbox0);
		ui_interface->declare(&fHslider1, "unit", "hz");
		ui_interface->addHorizontalSlider("auto_pluck_rate", &fHslider1, 0.100000001f, 0.0f, 2.0f, 0.00100000005f);
		ui_interface->declare(&fHslider11, "unit", "st");
		ui_interface->addHorizontalSlider("bend_depth", &fHslider11, 3.0f, 0.0f, 12.0f, 0.00999999978f);
		ui_interface->declare(&fHslider12, "unit", "ms");
		ui_interface->addHorizontalSlider("bend_time", &fHslider12, 10.0f, 1.0f, 200.0f, 1.0f);
		ui_interface->addHorizontalSlider("decay_scale", &fHslider9, 1.0f, 0.100000001f, 1.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("decay_time", &fHslider2, 1.0f, 1.0f, 100.0f, 0.00999999978f);
		ui_interface->declare(&fHslider10, "unit", "ms");
		ui_interface->addHorizontalSlider("decay_time", &fHslider10, 10000.0f, 0.0f, 100000.0f, 0.100000001f);
		ui_interface->declare(&fHslider13, "scale", "exp");
		ui_interface->addHorizontalSlider("harmonic_motion", &fHslider13, 0.00100000005f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addHorizontalSlider("high_freq_loss", &fHslider6, 0.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("jawari", &fHslider8, 0.0f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("material", &fHslider5, 0.129999995f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("pitch", &fHslider3, 36.0f, 24.0f, 72.0f, 1.0f);
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
		float fSlow1 = float(fCheckbox0);
		float fSlow2 = (fConst29 * float(fHslider1));
		float fSlow3 = float(fButton0);
		float fSlow4 = float(fHslider2);
		float fSlow5 = (fConst30 * fSlow4);
		float fSlow6 = (fConst20 * float(fHslider3));
		float fSlow7 = float(fHslider4);
		float fSlow8 = (fConst31 / fSlow7);
		float fSlow9 = (fConst20 * float(fHslider5));
		float fSlow10 = (fConst20 * (1.0f - float(fHslider6)));
		float fSlow11 = float(fHslider7);
		float fSlow12 = (fConst33 * float(fHslider8));
		float fSlow13 = float(fHslider9);
		float fSlow14 = (0.0022727272f / fSlow13);
		float fSlow15 = (fConst34 * float(fHslider10));
		float fSlow16 = float(fHslider11);
		float fSlow17 = (tambura_faustpower2_f(fSlow3) * fSlow16);
		float fSlow18 = (1.0f - fSlow3);
		float fSlow19 = (fConst35 * float(fHslider12));
		float fSlow20 = (fConst36 * float(fHslider13));
		float fSlow21 = float(fHslider14);
		float fSlow22 = (0.0022727272f / (fSlow21 * fSlow13));
		float fSlow23 = (fConst37 / fSlow21);
		float fSlow24 = float(fButton1);
		float fSlow25 = (fConst38 * fSlow4);
		float fSlow26 = (fConst39 / fSlow7);
		float fSlow27 = (tambura_faustpower2_f(fSlow24) * fSlow16);
		float fSlow28 = (1.0f - fSlow24);
		float fSlow29 = float(fButton2);
		float fSlow30 = (fConst40 * fSlow4);
		float fSlow31 = (fConst41 / fSlow7);
		float fSlow32 = (tambura_faustpower2_f(fSlow29) * fSlow16);
		float fSlow33 = (1.0f - fSlow29);
		float fSlow34 = float(fButton3);
		float fSlow35 = (fConst37 * fSlow4);
		float fSlow36 = (fConst42 / fSlow7);
		float fSlow37 = (tambura_faustpower2_f(fSlow34) * fSlow16);
		float fSlow38 = (1.0f - fSlow34);
		float fSlow39 = (fConst20 * float(fHslider15));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec13[0] = (fSlow0 + (fConst19 * fRec13[1]));
			float fTemp0 = (fRec13[0] * fRec5[1]);
			fVec0[0] = fTemp0;
			fRec12[0] = ((fConst17 * fRec12[1]) + (fConst18 * (fTemp0 + fVec0[1])));
			fRec14[0] = ((fConst17 * fRec14[1]) + (fConst18 * ((fConst21 * fVec0[1]) + (fConst15 * fTemp0))));
			float fTemp1 = (fConst22 * fRec11[1]);
			fRec11[0] = ((fRec12[0] + (9.99999975e-06f * fRec14[0])) - (fConst12 * (fTemp1 + (fConst23 * fRec11[2]))));
			float fTemp2 = (fConst7 * fRec10[1]);
			fRec10[0] = ((fConst12 * (((fConst14 * fRec11[0]) + fTemp1) + (fConst24 * fRec11[2]))) - (fConst6 * (fTemp2 + (fConst25 * fRec10[2]))));
			float fTemp3 = ((fTemp2 + (fConst27 * fRec10[0])) + (fConst28 * fRec10[2]));
			float fTemp4 = (fRec13[0] * fRec3[1]);
			fVec1[0] = fTemp4;
			fRec17[0] = ((fConst18 * (fTemp4 + fVec1[1])) + (fConst17 * fRec17[1]));
			fRec18[0] = ((fConst17 * fRec18[1]) + (fConst18 * ((fConst21 * fVec1[1]) + (fConst15 * fTemp4))));
			float fTemp5 = (fConst22 * fRec16[1]);
			fRec16[0] = ((fRec17[0] + (9.99999975e-06f * fRec18[0])) - (fConst12 * ((fConst23 * fRec16[2]) + fTemp5)));
			float fTemp6 = (fConst7 * fRec15[1]);
			fRec15[0] = ((fConst12 * ((fTemp5 + (fConst14 * fRec16[0])) + (fConst24 * fRec16[2]))) - (fConst6 * ((fConst25 * fRec15[2]) + fTemp6)));
			float fTemp7 = ((fTemp6 + (fConst27 * fRec15[0])) + (fConst28 * fRec15[2]));
			float fTemp8 = (fRec13[0] * fRec7[1]);
			fVec2[0] = fTemp8;
			fRec21[0] = ((fConst17 * fRec21[1]) + (fConst18 * (fTemp8 + fVec2[1])));
			fRec22[0] = ((fConst18 * ((fConst21 * fVec2[1]) + (fConst15 * fTemp8))) + (fConst17 * fRec22[1]));
			float fTemp9 = (fConst22 * fRec20[1]);
			fRec20[0] = ((fRec21[0] + (9.99999975e-06f * fRec22[0])) - (fConst12 * (fTemp9 + (fConst23 * fRec20[2]))));
			float fTemp10 = (fConst7 * fRec19[1]);
			fRec19[0] = ((fConst12 * ((fTemp9 + (fConst14 * fRec20[0])) + (fConst24 * fRec20[2]))) - (fConst6 * (fTemp10 + (fConst25 * fRec19[2]))));
			float fTemp11 = ((fTemp10 + (fConst27 * fRec19[0])) + (fConst28 * fRec19[2]));
			float fTemp12 = (fConst6 * ((fTemp3 + fTemp7) + fTemp11));
			fRec27[0] = (fSlow2 + (fRec27[1] - floorf((fSlow2 + fRec27[1]))));
			float fTemp13 = (fSlow1 * float((fRec27[0] < 0.25f)));
			float fTemp14 = ((1.0f - fTemp13) - fSlow3);
			fVec3[0] = fTemp14;
			fRec28[0] = (fSlow6 + (fConst19 * fRec28[1]));
			float fTemp15 = powf(2.0f, (0.0833333358f * (fRec28[0] + -69.0f)));
			float fTemp16 = ((fTemp14 != fVec3[1])?(fSlow5 / fTemp15):(fRec25[1] + -1.0f));
			fRec25[0] = fTemp16;
			fRec26[0] = ((fTemp16 <= 0.0f)?fTemp14:(fRec26[1] + (((1.0f - (fTemp13 + fRec26[1])) - fSlow3) / fTemp16)));
			float fTemp17 = (fSlow3 + fTemp13);
			fVec4[0] = fTemp17;
			float fTemp18 = (fRec26[0] * fTemp17);
			float fTemp19 = ((fRec23[1] > fTemp18)?fConst32:expf((0.0f - (fSlow8 * fTemp15))));
			fRec24[0] = ((fRec24[1] * fTemp19) + (fTemp18 * (1.0f - fTemp19)));
			fRec23[0] = fRec24[0];
			fRec29[0] = (fSlow9 + (fConst19 * fRec29[1]));
			iRec31[0] = ((1103515245 * iRec31[1]) + 12345);
			float fTemp20 = (4.65661287e-10f * float(iRec31[0]));
			fRec30[0] = (((0.522189379f * fRec30[3]) + (fTemp20 + (2.49495602f * fRec30[1]))) - (2.0172658f * fRec30[2]));
			float fTemp21 = (1.0f - (fRec29[0] * (1.0f - (((0.0499220341f * fRec30[0]) + (0.0506126992f * fRec30[2])) - ((0.0959935337f * fRec30[1]) + (0.00440878607f * fRec30[3]))))));
			float fTemp22 = (fRec23[0] * fTemp21);
			fVec5[(IOTA & 8191)] = fTemp22;
			float fTemp23 = (fTemp12 + fTemp22);
			fRec32[0] = (fSlow10 + (fConst19 * fRec32[1]));
			int iTemp24 = ((fVec4[1] <= 0.0f) & (fTemp17 > 0.0f));
			float fTemp25 = fabsf(fTemp20);
			fRec33[0] = ((fRec33[1] * float((1 - iTemp24))) + (float(iTemp24) * fTemp25));
			float fTemp26 = (fRec32[0] * (fTemp23 - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]));
			float fTemp27 = (fTemp26 + 1.0f);
			float fTemp28 = (1.0f - fTemp26);
			float fTemp29 = ((0.5f * (fTemp27 * fRec9[2])) + (0.25f * (fTemp28 * (fRec9[1] + fRec9[3]))));
			fRec34[0] = (fSlow12 + (fConst19 * fRec34[1]));
			float fTemp30 = (3.14159274f * (fRec34[0] * fTemp29));
			float fTemp31 = sinf(fTemp30);
			float fTemp32 = cosf(fTemp30);
			fRec35[0] = ((fRec35[1] * (0.0f - fTemp31)) + (fTemp29 * fTemp32));
			fRec36[0] = (fSlow15 + (fConst19 * fRec36[1]));
			fVec6[0] = fSlow18;
			float fTemp33 = ((fSlow18 != fVec6[1])?fSlow19:(fRec37[1] + -1.0f));
			fRec37[0] = fTemp33;
			fRec38[0] = ((fTemp33 <= 0.0f)?fSlow18:(fRec38[1] + (((1.0f - fRec38[1]) - fSlow3) / fTemp33)));
			float fTemp34 = (fSlow17 * tambura_faustpower2_f(fRec38[0]));
			float fTemp35 = (660.0f * fTemp15);
			fRec39[0] = (fSlow20 + (fConst19 * fRec39[1]));
			float fTemp36 = (4.0f * fRec39[0]);
			float fTemp37 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp34 + (fTemp35 + fTemp36))))));
			float fTemp38 = (fRec36[0] * fTemp37);
			float fTemp39 = ((fTemp23 + (((fTemp29 * fTemp31) + (fRec35[1] * fTemp32)) * powf(0.00100000005f, (fSlow14 / fTemp38)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec7[(IOTA & 8191)] = fTemp39;
			float fTemp40 = (fConst37 / fTemp37);
			float fTemp41 = (fTemp40 + -2.49999499f);
			int iTemp42 = min(4096, max(0, int(fTemp41)));
			float fTemp43 = floorf(fTemp41);
			float fTemp44 = ((fVec7[((IOTA - iTemp42) & 8191)] - fVec7[((IOTA - (iTemp42 + 1)) & 8191)]) * (fTemp43 + (2.0f - fTemp40)));
			fVec8[0] = fTemp44;
			fRec9[0] = ((0.5f * ((fTemp44 - fVec8[1]) * (fTemp43 + (3.0f - fTemp40)))) + (fVec7[((IOTA - iTemp42) & 8191)] + fTemp44));
			float fTemp45 = ((0.5f * (fTemp27 * fRec40[2])) + (0.25f * (fTemp28 * (fRec40[1] + fRec40[3]))));
			float fTemp46 = (3.14159274f * (fRec34[0] * fTemp45));
			float fTemp47 = sinf(fTemp46);
			float fTemp48 = cosf(fTemp46);
			fRec41[0] = ((fRec41[1] * (0.0f - fTemp47)) + (fTemp45 * fTemp48));
			float fTemp49 = (3.0f * fRec39[0]);
			float fTemp50 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp34 + (fTemp35 + fTemp49))))));
			float fTemp51 = (fRec36[0] * fTemp50);
			float fTemp52 = ((fTemp23 + (((fTemp45 * fTemp47) + (fRec41[1] * fTemp48)) * powf(0.00100000005f, (fSlow14 / fTemp51)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec9[(IOTA & 8191)] = fTemp52;
			float fTemp53 = (fConst37 / fTemp50);
			float fTemp54 = (fTemp53 + -2.49999499f);
			int iTemp55 = min(4096, max(0, int(fTemp54)));
			float fTemp56 = floorf(fTemp54);
			float fTemp57 = ((fVec9[((IOTA - iTemp55) & 8191)] - fVec9[((IOTA - (iTemp55 + 1)) & 8191)]) * (fTemp56 + (2.0f - fTemp53)));
			fVec10[0] = fTemp57;
			fRec40[0] = ((0.5f * ((fTemp57 - fVec10[1]) * (fTemp56 + (3.0f - fTemp53)))) + (fVec9[((IOTA - iTemp55) & 8191)] + fTemp57));
			float fTemp58 = ((0.5f * (fTemp27 * fRec42[2])) + (0.25f * (fTemp28 * (fRec42[1] + fRec42[3]))));
			float fTemp59 = (3.14159274f * (fRec34[0] * fTemp58));
			float fTemp60 = sinf(fTemp59);
			float fTemp61 = cosf(fTemp59);
			fRec43[0] = ((fRec43[1] * (0.0f - fTemp60)) + (fTemp58 * fTemp61));
			float fTemp62 = (2.0f * fRec39[0]);
			float fTemp63 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp34 + (fTemp35 + fTemp62))))));
			float fTemp64 = (fRec36[0] * fTemp63);
			float fTemp65 = ((fTemp23 + (((fTemp58 * fTemp60) + (fRec43[1] * fTemp61)) * powf(0.00100000005f, (fSlow14 / fTemp64)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec11[(IOTA & 8191)] = fTemp65;
			float fTemp66 = (fConst37 / fTemp63);
			float fTemp67 = (fTemp66 + -2.49999499f);
			int iTemp68 = min(4096, max(0, int(fTemp67)));
			float fTemp69 = floorf(fTemp67);
			float fTemp70 = ((fVec11[((IOTA - iTemp68) & 8191)] - fVec11[((IOTA - (iTemp68 + 1)) & 8191)]) * (fTemp69 + (2.0f - fTemp66)));
			fVec12[0] = fTemp70;
			fRec42[0] = ((0.5f * ((fTemp70 - fVec12[1]) * (fTemp69 + (3.0f - fTemp66)))) + (fVec11[((IOTA - iTemp68) & 8191)] + fTemp70));
			float fTemp71 = ((0.5f * (fTemp27 * fRec44[2])) + (0.25f * (fTemp28 * (fRec44[1] + fRec44[3]))));
			float fTemp72 = (3.14159274f * (fRec34[0] * fTemp71));
			float fTemp73 = sinf(fTemp72);
			float fTemp74 = cosf(fTemp72);
			fRec45[0] = ((fRec45[1] * (0.0f - fTemp73)) + (fTemp71 * fTemp74));
			float fTemp75 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp34 + (fRec39[0] + fTemp35))))));
			float fTemp76 = (fRec36[0] * fTemp75);
			float fTemp77 = ((fTemp23 + (((fTemp71 * fTemp73) + (fRec45[1] * fTemp74)) * powf(0.00100000005f, (fSlow14 / fTemp76)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec13[(IOTA & 8191)] = fTemp77;
			float fTemp78 = (fConst37 / fTemp75);
			float fTemp79 = (fTemp78 + -2.49999499f);
			int iTemp80 = min(4096, max(0, int(fTemp79)));
			float fTemp81 = floorf(fTemp79);
			float fTemp82 = ((fVec13[((IOTA - iTemp80) & 8191)] - fVec13[((IOTA - (iTemp80 + 1)) & 8191)]) * (fTemp81 + (2.0f - fTemp78)));
			fVec14[0] = fTemp82;
			fRec44[0] = ((0.5f * ((fTemp82 - fVec14[1]) * (fTemp81 + (3.0f - fTemp78)))) + (fVec13[((IOTA - iTemp80) & 8191)] + fTemp82));
			float fTemp83 = ((0.5f * (fTemp27 * fRec46[2])) + (0.25f * (fTemp28 * (fRec46[1] + fRec46[3]))));
			float fTemp84 = (3.14159274f * (fRec34[0] * fTemp83));
			float fTemp85 = sinf(fTemp84);
			float fTemp86 = cosf(fTemp84);
			fRec47[0] = ((fRec47[1] * (0.0f - fTemp85)) + (fTemp83 * fTemp86));
			float fTemp87 = (fTemp35 + fTemp34);
			float fTemp88 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp87 - fTemp36)))));
			float fTemp89 = (fRec36[0] * fTemp88);
			float fTemp90 = ((fTemp23 + (((fTemp83 * fTemp85) + (fRec47[1] * fTemp86)) * powf(0.00100000005f, (fSlow14 / fTemp89)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec15[(IOTA & 8191)] = fTemp90;
			float fTemp91 = (fConst37 / fTemp88);
			float fTemp92 = (fTemp91 + -2.49999499f);
			int iTemp93 = min(4096, max(0, int(fTemp92)));
			float fTemp94 = floorf(fTemp92);
			float fTemp95 = ((fVec15[((IOTA - iTemp93) & 8191)] - fVec15[((IOTA - (iTemp93 + 1)) & 8191)]) * (fTemp94 + (2.0f - fTemp91)));
			fVec16[0] = fTemp95;
			fRec46[0] = ((0.5f * ((fTemp95 - fVec16[1]) * (fTemp94 + (3.0f - fTemp91)))) + (fVec15[((IOTA - iTemp93) & 8191)] + fTemp95));
			float fTemp96 = ((0.5f * (fTemp27 * fRec48[2])) + (0.25f * (fTemp28 * (fRec48[1] + fRec48[3]))));
			float fTemp97 = (3.14159274f * (fRec34[0] * fTemp96));
			float fTemp98 = sinf(fTemp97);
			float fTemp99 = cosf(fTemp97);
			fRec49[0] = ((fRec49[1] * (0.0f - fTemp98)) + (fTemp96 * fTemp99));
			float fTemp100 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp87 - fTemp49)))));
			float fTemp101 = (fRec36[0] * fTemp100);
			float fTemp102 = ((fTemp23 + (((fTemp96 * fTemp98) + (fRec49[1] * fTemp99)) * powf(0.00100000005f, (fSlow14 / fTemp101)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec17[(IOTA & 8191)] = fTemp102;
			float fTemp103 = (fConst37 / fTemp100);
			float fTemp104 = (fTemp103 + -2.49999499f);
			int iTemp105 = min(4096, max(0, int(fTemp104)));
			float fTemp106 = floorf(fTemp104);
			float fTemp107 = ((fVec17[((IOTA - iTemp105) & 8191)] - fVec17[((IOTA - (iTemp105 + 1)) & 8191)]) * (fTemp106 + (2.0f - fTemp103)));
			fVec18[0] = fTemp107;
			fRec48[0] = ((0.5f * ((fTemp107 - fVec18[1]) * (fTemp106 + (3.0f - fTemp103)))) + (fVec17[((IOTA - iTemp105) & 8191)] + fTemp107));
			float fTemp108 = ((0.5f * (fTemp27 * fRec50[2])) + (0.25f * (fTemp28 * (fRec50[1] + fRec50[3]))));
			float fTemp109 = (3.14159274f * (fRec34[0] * fTemp108));
			float fTemp110 = sinf(fTemp109);
			float fTemp111 = cosf(fTemp109);
			fRec51[0] = ((fRec51[1] * (0.0f - fTemp110)) + (fTemp108 * fTemp111));
			float fTemp112 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp87 - fTemp62)))));
			float fTemp113 = (fRec36[0] * fTemp112);
			float fTemp114 = ((fTemp23 + (((fTemp108 * fTemp110) + (fRec51[1] * fTemp111)) * powf(0.00100000005f, (fSlow14 / fTemp113)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec19[(IOTA & 8191)] = fTemp114;
			float fTemp115 = (fConst37 / fTemp112);
			float fTemp116 = (fTemp115 + -2.49999499f);
			int iTemp117 = min(4096, max(0, int(fTemp116)));
			float fTemp118 = floorf(fTemp116);
			float fTemp119 = ((fVec19[((IOTA - iTemp117) & 8191)] - fVec19[((IOTA - (iTemp117 + 1)) & 8191)]) * (fTemp118 + (2.0f - fTemp115)));
			fVec20[0] = fTemp119;
			fRec50[0] = ((0.5f * ((fTemp119 - fVec20[1]) * (fTemp118 + (3.0f - fTemp115)))) + (fVec19[((IOTA - iTemp117) & 8191)] + fTemp119));
			float fTemp120 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp87 - fRec39[0])))));
			float fTemp121 = (fRec36[0] * fTemp120);
			float fTemp122 = ((0.5f * (fTemp27 * fRec52[2])) + (0.25f * (fTemp28 * (fRec52[1] + fRec52[3]))));
			float fTemp123 = (3.14159274f * (fRec34[0] * fTemp122));
			float fTemp124 = sinf(fTemp123);
			float fTemp125 = cosf(fTemp123);
			fRec53[0] = ((fRec53[1] * (0.0f - fTemp124)) + (fTemp122 * fTemp125));
			float fTemp126 = ((((powf(0.00100000005f, (fSlow14 / fTemp121)) * ((fTemp122 * fTemp124) + (fRec53[1] * fTemp125))) + fTemp12) + fTemp22) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec21[(IOTA & 8191)] = fTemp126;
			float fTemp127 = (fConst37 / fTemp120);
			float fTemp128 = (fTemp127 + -2.49999499f);
			int iTemp129 = min(4096, max(0, int(fTemp128)));
			float fTemp130 = floorf(fTemp128);
			float fTemp131 = ((fVec21[((IOTA - iTemp129) & 8191)] - fVec21[((IOTA - (iTemp129 + 1)) & 8191)]) * (fTemp130 + (2.0f - fTemp127)));
			fVec22[0] = fTemp131;
			fRec52[0] = ((0.5f * ((fTemp131 - fVec22[1]) * (fTemp130 + (3.0f - fTemp127)))) + (fVec21[((IOTA - iTemp129) & 8191)] + fTemp131));
			float fTemp132 = ((0.5f * (fRec54[2] * fTemp27)) + (0.25f * ((fRec54[1] + fRec54[3]) * fTemp28)));
			float fTemp133 = (3.14159274f * (fRec34[0] * fTemp132));
			float fTemp134 = sinf(fTemp133);
			float fTemp135 = cosf(fTemp133);
			fRec55[0] = ((fRec55[1] * (0.0f - fTemp134)) + (fTemp132 * fTemp135));
			float fTemp136 = powf(2.0f, (1.44269502f * logf((0.0022727272f * fTemp87))));
			float fTemp137 = (fRec36[0] * fTemp136);
			float fTemp138 = ((fTemp23 + (((fTemp132 * fTemp134) + (fRec55[1] * fTemp135)) * powf(0.00100000005f, (fSlow14 / fTemp137)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec23[(IOTA & 8191)] = fTemp138;
			float fTemp139 = (fConst37 / fTemp136);
			float fTemp140 = (fTemp139 + -2.49999499f);
			int iTemp141 = min(4096, max(0, int(fTemp140)));
			float fTemp142 = floorf(fTemp140);
			float fTemp143 = ((fVec23[((IOTA - iTemp141) & 8191)] - fVec23[((IOTA - (iTemp141 + 1)) & 8191)]) * (fTemp142 + (2.0f - fTemp139)));
			fVec24[0] = fTemp143;
			fRec54[0] = ((0.5f * ((fTemp143 - fVec24[1]) * (fTemp142 + (3.0f - fTemp139)))) + (fVec23[((IOTA - iTemp141) & 8191)] + fTemp143));
			float fTemp144 = ((((fRec46[0] + fRec48[0]) + fRec50[0]) + fRec52[0]) + fRec54[0]);
			fVec25[0] = ((((fTemp144 + fRec44[0]) + fRec42[0]) + fRec40[0]) + fRec9[0]);
			fRec8[0] = ((fRec9[0] + (fRec40[0] + (fRec42[0] + (fRec44[0] + (fTemp144 + (0.995000005f * fRec8[1])))))) - fVec25[1]);
			float fRec0 = (0.00999999978f * fRec8[0]);
			float fTemp145 = (fRec32[0] * (fTemp22 - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]));
			float fTemp146 = (fTemp145 + 1.0f);
			float fTemp147 = (1.0f - fTemp145);
			float fTemp148 = ((0.5f * (fTemp146 * fRec57[2])) + (0.25f * (fTemp147 * (fRec57[1] + fRec57[3]))));
			float fTemp149 = (3.14159274f * (fRec34[0] * fTemp148));
			float fTemp150 = sinf(fTemp149);
			float fTemp151 = cosf(fTemp149);
			fRec58[0] = ((fRec58[1] * (0.0f - fTemp150)) + (fTemp148 * fTemp151));
			float fTemp152 = ((fTemp22 + (((fTemp148 * fTemp150) + (fRec58[1] * fTemp151)) * powf(0.00100000005f, (fSlow22 / fTemp89)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec26[(IOTA & 8191)] = fTemp152;
			float fTemp153 = (fSlow23 / fTemp88);
			float fTemp154 = (fTemp153 + -2.49999499f);
			int iTemp155 = min(4096, max(0, int(fTemp154)));
			float fTemp156 = floorf(fTemp154);
			float fTemp157 = ((fVec26[((IOTA - iTemp155) & 8191)] - fVec26[((IOTA - (iTemp155 + 1)) & 8191)]) * (fTemp156 + (2.0f - fTemp153)));
			fVec27[0] = fTemp157;
			fRec57[0] = ((0.5f * ((fTemp157 - fVec27[1]) * (fTemp156 + (3.0f - fTemp153)))) + (fVec26[((IOTA - iTemp155) & 8191)] + fTemp157));
			float fTemp158 = ((0.5f * (fTemp146 * fRec59[2])) + (0.25f * (fTemp147 * (fRec59[1] + fRec59[3]))));
			float fTemp159 = (3.14159274f * (fRec34[0] * fTemp158));
			float fTemp160 = sinf(fTemp159);
			float fTemp161 = cosf(fTemp159);
			fRec60[0] = ((fRec60[1] * (0.0f - fTemp160)) + (fTemp158 * fTemp161));
			float fTemp162 = ((fTemp22 + (((fTemp158 * fTemp160) + (fRec60[1] * fTemp161)) * powf(0.00100000005f, (fSlow22 / fTemp101)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec28[(IOTA & 8191)] = fTemp162;
			float fTemp163 = (fSlow23 / fTemp100);
			float fTemp164 = (fTemp163 + -2.49999499f);
			int iTemp165 = min(4096, max(0, int(fTemp164)));
			float fTemp166 = floorf(fTemp164);
			float fTemp167 = ((fVec28[((IOTA - iTemp165) & 8191)] - fVec28[((IOTA - (iTemp165 + 1)) & 8191)]) * (fTemp166 + (2.0f - fTemp163)));
			fVec29[0] = fTemp167;
			fRec59[0] = ((0.5f * ((fTemp167 - fVec29[1]) * (fTemp166 + (3.0f - fTemp163)))) + (fVec28[((IOTA - iTemp165) & 8191)] + fTemp167));
			float fTemp168 = ((0.5f * (fRec61[2] * fTemp146)) + (0.25f * ((fRec61[1] + fRec61[3]) * fTemp147)));
			float fTemp169 = (3.14159274f * (fRec34[0] * fTemp168));
			float fTemp170 = sinf(fTemp169);
			float fTemp171 = cosf(fTemp169);
			fRec62[0] = ((fRec62[1] * (0.0f - fTemp170)) + (fTemp168 * fTemp171));
			float fTemp172 = ((fTemp22 + (((fTemp168 * fTemp170) + (fRec62[1] * fTemp171)) * powf(0.00100000005f, (fSlow22 / fTemp113)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec30[(IOTA & 8191)] = fTemp172;
			float fTemp173 = (fSlow23 / fTemp112);
			float fTemp174 = (fTemp173 + -2.49999499f);
			int iTemp175 = min(4096, max(0, int(fTemp174)));
			float fTemp176 = floorf(fTemp174);
			float fTemp177 = ((fVec30[((IOTA - iTemp175) & 8191)] - fVec30[((IOTA - (iTemp175 + 1)) & 8191)]) * (fTemp176 + (2.0f - fTemp173)));
			fVec31[0] = fTemp177;
			fRec61[0] = ((fTemp177 + (0.5f * ((fTemp177 - fVec31[1]) * (fTemp176 + (3.0f - fTemp173))))) + fVec30[((IOTA - iTemp175) & 8191)]);
			float fTemp178 = ((0.5f * (fRec63[2] * fTemp146)) + (0.25f * ((fRec63[1] + fRec63[3]) * fTemp147)));
			float fTemp179 = (3.14159274f * (fRec34[0] * fTemp178));
			float fTemp180 = sinf(fTemp179);
			float fTemp181 = cosf(fTemp179);
			fRec64[0] = ((fRec64[1] * (0.0f - fTemp180)) + (fTemp178 * fTemp181));
			float fTemp182 = ((fTemp22 + (((fTemp178 * fTemp180) + (fRec64[1] * fTemp181)) * powf(0.00100000005f, (fSlow22 / fTemp121)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec32[(IOTA & 8191)] = fTemp182;
			float fTemp183 = (fSlow23 / fTemp120);
			float fTemp184 = (fTemp183 + -2.49999499f);
			int iTemp185 = min(4096, max(0, int(fTemp184)));
			float fTemp186 = floorf(fTemp184);
			float fTemp187 = ((fVec32[((IOTA - iTemp185) & 8191)] - fVec32[((IOTA - (iTemp185 + 1)) & 8191)]) * (fTemp186 + (2.0f - fTemp183)));
			fVec33[0] = fTemp187;
			fRec63[0] = ((0.5f * ((fTemp187 - fVec33[1]) * (fTemp186 + (3.0f - fTemp183)))) + (fVec32[((IOTA - iTemp185) & 8191)] + fTemp187));
			float fTemp188 = ((0.5f * (fRec65[2] * fTemp146)) + (0.25f * ((fRec65[1] + fRec65[3]) * fTemp147)));
			float fTemp189 = (3.14159274f * (fRec34[0] * fTemp188));
			float fTemp190 = sinf(fTemp189);
			float fTemp191 = cosf(fTemp189);
			fRec66[0] = ((fRec66[1] * (0.0f - fTemp190)) + (fTemp188 * fTemp191));
			float fTemp192 = ((fTemp22 + (((fTemp188 * fTemp190) + (fRec66[1] * fTemp191)) * powf(0.00100000005f, (fSlow22 / fTemp137)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec34[(IOTA & 8191)] = fTemp192;
			float fTemp193 = (fSlow23 / fTemp136);
			float fTemp194 = (fTemp193 + -2.49999499f);
			int iTemp195 = min(4096, max(0, int(fTemp194)));
			float fTemp196 = floorf(fTemp194);
			float fTemp197 = ((fVec34[((IOTA - iTemp195) & 8191)] - fVec34[((IOTA - (iTemp195 + 1)) & 8191)]) * (fTemp196 + (2.0f - fTemp193)));
			fVec35[0] = fTemp197;
			fRec65[0] = ((0.5f * ((fTemp197 - fVec35[1]) * (fTemp196 + (3.0f - fTemp193)))) + (fVec34[((IOTA - iTemp195) & 8191)] + fTemp197));
			float fTemp198 = ((0.5f * (fRec67[2] * fTemp146)) + (0.25f * ((fRec67[1] + fRec67[3]) * fTemp147)));
			float fTemp199 = (3.14159274f * (fRec34[0] * fTemp198));
			float fTemp200 = sinf(fTemp199);
			float fTemp201 = cosf(fTemp199);
			fRec68[0] = ((fRec68[1] * (0.0f - fTemp200)) + (fTemp198 * fTemp201));
			float fTemp202 = ((fTemp22 + (((fTemp198 * fTemp200) + (fRec68[1] * fTemp201)) * powf(0.00100000005f, (fSlow22 / fTemp76)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec36[(IOTA & 8191)] = fTemp202;
			float fTemp203 = (fSlow23 / fTemp75);
			float fTemp204 = (fTemp203 + -2.49999499f);
			int iTemp205 = min(4096, max(0, int(fTemp204)));
			float fTemp206 = floorf(fTemp204);
			float fTemp207 = ((fVec36[((IOTA - iTemp205) & 8191)] - fVec36[((IOTA - (iTemp205 + 1)) & 8191)]) * (fTemp206 + (2.0f - fTemp203)));
			fVec37[0] = fTemp207;
			fRec67[0] = ((0.5f * ((fTemp207 - fVec37[1]) * (fTemp206 + (3.0f - fTemp203)))) + (fVec36[((IOTA - iTemp205) & 8191)] + fTemp207));
			float fTemp208 = ((0.5f * (fRec69[2] * fTemp146)) + (0.25f * ((fRec69[1] + fRec69[3]) * fTemp147)));
			float fTemp209 = (3.14159274f * (fRec34[0] * fTemp208));
			float fTemp210 = sinf(fTemp209);
			float fTemp211 = cosf(fTemp209);
			fRec70[0] = ((fRec70[1] * (0.0f - fTemp210)) + (fTemp208 * fTemp211));
			float fTemp212 = ((fTemp22 + (((fTemp208 * fTemp210) + (fRec70[1] * fTemp211)) * powf(0.00100000005f, (fSlow22 / fTemp64)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec38[(IOTA & 8191)] = fTemp212;
			float fTemp213 = (fSlow23 / fTemp63);
			float fTemp214 = (fTemp213 + -2.49999499f);
			int iTemp215 = min(4096, max(0, int(fTemp214)));
			float fTemp216 = floorf(fTemp214);
			float fTemp217 = ((fVec38[((IOTA - iTemp215) & 8191)] - fVec38[((IOTA - (iTemp215 + 1)) & 8191)]) * (fTemp216 + (2.0f - fTemp213)));
			fVec39[0] = fTemp217;
			fRec69[0] = ((0.5f * ((fTemp217 - fVec39[1]) * (fTemp216 + (3.0f - fTemp213)))) + (fVec38[((IOTA - iTemp215) & 8191)] + fTemp217));
			float fTemp218 = ((0.5f * (fRec71[2] * fTemp146)) + (0.25f * ((fRec71[1] + fRec71[3]) * fTemp147)));
			float fTemp219 = (3.14159274f * (fRec34[0] * fTemp218));
			float fTemp220 = sinf(fTemp219);
			float fTemp221 = cosf(fTemp219);
			fRec72[0] = ((fRec72[1] * (0.0f - fTemp220)) + (fTemp218 * fTemp221));
			float fTemp222 = ((fTemp22 + (((fTemp218 * fTemp220) + (fRec72[1] * fTemp221)) * powf(0.00100000005f, (fSlow22 / fTemp51)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec40[(IOTA & 8191)] = fTemp222;
			float fTemp223 = (fSlow23 / fTemp50);
			float fTemp224 = (fTemp223 + -2.49999499f);
			int iTemp225 = min(4096, max(0, int(fTemp224)));
			float fTemp226 = floorf(fTemp224);
			float fTemp227 = ((fVec40[((IOTA - iTemp225) & 8191)] - fVec40[((IOTA - (iTemp225 + 1)) & 8191)]) * (fTemp226 + (2.0f - fTemp223)));
			fVec41[0] = fTemp227;
			fRec71[0] = ((0.5f * ((fTemp227 - fVec41[1]) * (fTemp226 + (3.0f - fTemp223)))) + (fVec40[((IOTA - iTemp225) & 8191)] + fTemp227));
			float fTemp228 = ((0.5f * (fRec73[2] * fTemp146)) + (0.25f * ((fRec73[1] + fRec73[3]) * fTemp147)));
			float fTemp229 = (3.14159274f * (fRec34[0] * fTemp228));
			float fTemp230 = sinf(fTemp229);
			float fTemp231 = cosf(fTemp229);
			fRec74[0] = ((fRec74[1] * (0.0f - fTemp230)) + (fTemp228 * fTemp231));
			float fTemp232 = ((fTemp22 + (((fTemp228 * fTemp230) + (fRec74[1] * fTemp231)) * powf(0.00100000005f, (fSlow22 / fTemp38)))) - fVec5[((IOTA - int(min(4096.0f, max(0.0f, (fConst30 * ((fSlow11 + (0.200000003f * fRec33[0])) / fTemp15)))))) & 8191)]);
			fVec42[(IOTA & 8191)] = fTemp232;
			float fTemp233 = (fSlow23 / fTemp37);
			float fTemp234 = (fTemp233 + -2.49999499f);
			int iTemp235 = min(4096, max(0, int(fTemp234)));
			float fTemp236 = floorf(fTemp234);
			float fTemp237 = ((fVec42[((IOTA - iTemp235) & 8191)] - fVec42[((IOTA - (iTemp235 + 1)) & 8191)]) * (fTemp236 + (2.0f - fTemp233)));
			fVec43[0] = fTemp237;
			fRec73[0] = ((0.5f * ((fTemp237 - fVec43[1]) * (fTemp236 + (3.0f - fTemp233)))) + (fVec42[((IOTA - iTemp235) & 8191)] + fTemp237));
			float fTemp238 = ((((((((fRec57[0] + fRec59[0]) + fRec61[0]) + fRec63[0]) + fRec65[0]) + fRec67[0]) + fRec69[0]) + fRec71[0]) + fRec73[0]);
			fVec44[0] = fTemp238;
			fRec56[0] = ((fTemp238 + (0.995000005f * fRec56[1])) - fVec44[1]);
			fRec1[0] = (0.00999999978f * fRec56[0]);
			float fTemp239 = (fRec13[0] * fRec1[1]);
			fVec45[0] = fTemp239;
			fRec79[0] = ((fConst17 * fRec79[1]) + (fConst18 * ((fConst21 * fVec45[1]) + (fConst15 * fTemp239))));
			fRec80[0] = ((fConst17 * fRec80[1]) + (fConst18 * (fTemp239 + fVec45[1])));
			float fTemp240 = (fConst22 * fRec78[1]);
			fRec78[0] = (((9.99999975e-06f * fRec79[0]) + fRec80[0]) - (fConst12 * (fTemp240 + (fConst23 * fRec78[2]))));
			float fTemp241 = (fConst7 * fRec77[1]);
			fRec77[0] = ((fConst12 * ((fTemp240 + (fConst14 * fRec78[0])) + (fConst24 * fRec78[2]))) - (fConst6 * (fTemp241 + (fConst25 * fRec77[2]))));
			float fTemp242 = ((fTemp241 + (fConst27 * fRec77[0])) + (fConst28 * fRec77[2]));
			float fTemp243 = (fSlow1 * float(((fRec27[0] > 0.25f) & (fRec27[0] < 0.5f))));
			float fTemp244 = (fSlow24 + fTemp243);
			fVec46[0] = fTemp244;
			float fTemp245 = ((1.0f - fTemp243) - fSlow24);
			fVec47[0] = fTemp245;
			float fTemp246 = ((fTemp245 != fVec47[1])?(fSlow25 / fTemp15):(fRec83[1] + -1.0f));
			fRec83[0] = fTemp246;
			fRec84[0] = ((fTemp246 <= 0.0f)?fTemp245:(fRec84[1] + (((1.0f - (fTemp243 + fRec84[1])) - fSlow24) / fTemp246)));
			float fTemp247 = ((fRec81[1] > (fTemp244 * fRec84[0]))?fConst32:expf((0.0f - (fSlow26 * fTemp15))));
			fRec82[0] = ((fRec82[1] * fTemp247) + ((fTemp244 * (1.0f - fTemp247)) * fRec84[0]));
			fRec81[0] = fRec82[0];
			float fTemp248 = (fTemp21 * fRec81[0]);
			fVec48[(IOTA & 8191)] = fTemp248;
			float fTemp249 = ((fConst6 * (fTemp11 + (fTemp3 + fTemp242))) + fTemp248);
			int iTemp250 = ((fVec46[1] <= 0.0f) & (fTemp244 > 0.0f));
			fRec85[0] = ((fRec85[1] * float((1 - iTemp250))) + (fTemp25 * float(iTemp250)));
			float fTemp251 = (fRec32[0] * (fTemp249 - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]));
			float fTemp252 = (fTemp251 + 1.0f);
			float fTemp253 = (1.0f - fTemp251);
			float fTemp254 = ((0.5f * (fTemp252 * fRec76[2])) + (0.25f * (fTemp253 * (fRec76[1] + fRec76[3]))));
			float fTemp255 = (3.14159274f * (fRec34[0] * fTemp254));
			float fTemp256 = sinf(fTemp255);
			float fTemp257 = cosf(fTemp255);
			fRec86[0] = ((fRec86[1] * (0.0f - fTemp256)) + (fTemp254 * fTemp257));
			float fTemp258 = (880.0f * fTemp15);
			fVec49[0] = fSlow28;
			float fTemp259 = ((fSlow28 != fVec49[1])?fSlow19:(fRec87[1] + -1.0f));
			fRec87[0] = fTemp259;
			fRec88[0] = ((fTemp259 <= 0.0f)?fSlow28:(fRec88[1] + (((1.0f - fRec88[1]) - fSlow24) / fTemp259)));
			float fTemp260 = (fSlow27 * tambura_faustpower2_f(fRec88[0]));
			float fTemp261 = (fTemp258 + fTemp260);
			float fTemp262 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp261 - fTemp36)))));
			float fTemp263 = (fRec36[0] * fTemp262);
			float fTemp264 = ((fTemp249 + (((fTemp254 * fTemp256) + (fRec86[1] * fTemp257)) * powf(0.00100000005f, (fSlow14 / fTemp263)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec50[(IOTA & 8191)] = fTemp264;
			float fTemp265 = (fConst37 / fTemp262);
			float fTemp266 = (fTemp265 + -2.49999499f);
			int iTemp267 = min(4096, max(0, int(fTemp266)));
			float fTemp268 = floorf(fTemp266);
			float fTemp269 = ((fVec50[((IOTA - iTemp267) & 8191)] - fVec50[((IOTA - (iTemp267 + 1)) & 8191)]) * (fTemp268 + (2.0f - fTemp265)));
			fVec51[0] = fTemp269;
			fRec76[0] = ((fTemp269 + (0.5f * ((fTemp269 - fVec51[1]) * (fTemp268 + (3.0f - fTemp265))))) + fVec50[((IOTA - iTemp267) & 8191)]);
			float fTemp270 = ((0.5f * (fTemp252 * fRec89[2])) + (0.25f * (fTemp253 * (fRec89[1] + fRec89[3]))));
			float fTemp271 = (3.14159274f * (fRec34[0] * fTemp270));
			float fTemp272 = sinf(fTemp271);
			float fTemp273 = cosf(fTemp271);
			fRec90[0] = ((fRec90[1] * (0.0f - fTemp272)) + (fTemp270 * fTemp273));
			float fTemp274 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp261 - fTemp49)))));
			float fTemp275 = (fRec36[0] * fTemp274);
			float fTemp276 = ((fTemp249 + (((fTemp270 * fTemp272) + (fRec90[1] * fTemp273)) * powf(0.00100000005f, (fSlow14 / fTemp275)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec52[(IOTA & 8191)] = fTemp276;
			float fTemp277 = (fConst37 / fTemp274);
			float fTemp278 = (fTemp277 + -2.49999499f);
			int iTemp279 = min(4096, max(0, int(fTemp278)));
			float fTemp280 = floorf(fTemp278);
			float fTemp281 = ((fVec52[((IOTA - iTemp279) & 8191)] - fVec52[((IOTA - (iTemp279 + 1)) & 8191)]) * (fTemp280 + (2.0f - fTemp277)));
			fVec53[0] = fTemp281;
			fRec89[0] = ((0.5f * ((fTemp281 - fVec53[1]) * (fTemp280 + (3.0f - fTemp277)))) + (fVec52[((IOTA - iTemp279) & 8191)] + fTemp281));
			float fTemp282 = ((0.5f * (fTemp252 * fRec91[2])) + (0.25f * (fTemp253 * (fRec91[1] + fRec91[3]))));
			float fTemp283 = (3.14159274f * (fRec34[0] * fTemp282));
			float fTemp284 = sinf(fTemp283);
			float fTemp285 = cosf(fTemp283);
			fRec92[0] = ((fRec92[1] * (0.0f - fTemp284)) + (fTemp282 * fTemp285));
			float fTemp286 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp261 - fTemp62)))));
			float fTemp287 = (fRec36[0] * fTemp286);
			float fTemp288 = ((fTemp249 + (((fTemp282 * fTemp284) + (fRec92[1] * fTemp285)) * powf(0.00100000005f, (fSlow14 / fTemp287)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec54[(IOTA & 8191)] = fTemp288;
			float fTemp289 = (fConst37 / fTemp286);
			float fTemp290 = (fTemp289 + -2.49999499f);
			int iTemp291 = min(4096, max(0, int(fTemp290)));
			float fTemp292 = floorf(fTemp290);
			float fTemp293 = ((fVec54[((IOTA - iTemp291) & 8191)] - fVec54[((IOTA - (iTemp291 + 1)) & 8191)]) * (fTemp292 + (2.0f - fTemp289)));
			fVec55[0] = fTemp293;
			fRec91[0] = ((0.5f * ((fTemp293 - fVec55[1]) * (fTemp292 + (3.0f - fTemp289)))) + (fVec54[((IOTA - iTemp291) & 8191)] + fTemp293));
			float fTemp294 = ((0.5f * (fTemp252 * fRec93[2])) + (0.25f * (fTemp253 * (fRec93[1] + fRec93[3]))));
			float fTemp295 = (3.14159274f * (fRec34[0] * fTemp294));
			float fTemp296 = sinf(fTemp295);
			float fTemp297 = cosf(fTemp295);
			fRec94[0] = ((fRec94[1] * (0.0f - fTemp296)) + (fTemp294 * fTemp297));
			float fTemp298 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp261 - fRec39[0])))));
			float fTemp299 = (fRec36[0] * fTemp298);
			float fTemp300 = ((fTemp249 + (((fTemp294 * fTemp296) + (fRec94[1] * fTemp297)) * powf(0.00100000005f, (fSlow14 / fTemp299)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec56[(IOTA & 8191)] = fTemp300;
			float fTemp301 = (fConst37 / fTemp298);
			float fTemp302 = (fTemp301 + -2.49999499f);
			int iTemp303 = min(4096, max(0, int(fTemp302)));
			float fTemp304 = floorf(fTemp302);
			float fTemp305 = ((fVec56[((IOTA - iTemp303) & 8191)] - fVec56[((IOTA - (iTemp303 + 1)) & 8191)]) * (fTemp304 + (2.0f - fTemp301)));
			fVec57[0] = fTemp305;
			fRec93[0] = ((0.5f * ((fTemp305 - fVec57[1]) * (fTemp304 + (3.0f - fTemp301)))) + (fVec56[((IOTA - iTemp303) & 8191)] + fTemp305));
			float fTemp306 = ((0.5f * (fTemp252 * fRec95[2])) + (0.25f * (fTemp253 * (fRec95[1] + fRec95[3]))));
			float fTemp307 = (3.14159274f * (fRec34[0] * fTemp306));
			float fTemp308 = sinf(fTemp307);
			float fTemp309 = cosf(fTemp307);
			fRec96[0] = ((fRec96[1] * (0.0f - fTemp308)) + (fTemp306 * fTemp309));
			float fTemp310 = powf(2.0f, (1.44269502f * logf((0.0022727272f * fTemp261))));
			float fTemp311 = (fRec36[0] * fTemp310);
			float fTemp312 = ((fTemp249 + (((fTemp306 * fTemp308) + (fRec96[1] * fTemp309)) * powf(0.00100000005f, (fSlow14 / fTemp311)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec58[(IOTA & 8191)] = fTemp312;
			float fTemp313 = (fConst37 / fTemp310);
			float fTemp314 = (fTemp313 + -2.49999499f);
			int iTemp315 = min(4096, max(0, int(fTemp314)));
			float fTemp316 = floorf(fTemp314);
			float fTemp317 = ((fVec58[((IOTA - iTemp315) & 8191)] - fVec58[((IOTA - (iTemp315 + 1)) & 8191)]) * (fTemp316 + (2.0f - fTemp313)));
			fVec59[0] = fTemp317;
			fRec95[0] = ((0.5f * ((fTemp317 - fVec59[1]) * (fTemp316 + (3.0f - fTemp313)))) + (fVec58[((IOTA - iTemp315) & 8191)] + fTemp317));
			float fTemp318 = ((0.5f * (fTemp252 * fRec97[2])) + (0.25f * (fTemp253 * (fRec97[1] + fRec97[3]))));
			float fTemp319 = (3.14159274f * (fRec34[0] * fTemp318));
			float fTemp320 = sinf(fTemp319);
			float fTemp321 = cosf(fTemp319);
			fRec98[0] = ((fRec98[1] * (0.0f - fTemp320)) + (fTemp318 * fTemp321));
			float fTemp322 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp260 + (fRec39[0] + fTemp258))))));
			float fTemp323 = (fRec36[0] * fTemp322);
			float fTemp324 = ((fTemp249 + (((fTemp318 * fTemp320) + (fRec98[1] * fTemp321)) * powf(0.00100000005f, (fSlow14 / fTemp323)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec60[(IOTA & 8191)] = fTemp324;
			float fTemp325 = (fConst37 / fTemp322);
			float fTemp326 = (fTemp325 + -2.49999499f);
			int iTemp327 = min(4096, max(0, int(fTemp326)));
			float fTemp328 = floorf(fTemp326);
			float fTemp329 = ((fVec60[((IOTA - iTemp327) & 8191)] - fVec60[((IOTA - (iTemp327 + 1)) & 8191)]) * (fTemp328 + (2.0f - fTemp325)));
			fVec61[0] = fTemp329;
			fRec97[0] = ((0.5f * ((fTemp329 - fVec61[1]) * (fTemp328 + (3.0f - fTemp325)))) + (fVec60[((IOTA - iTemp327) & 8191)] + fTemp329));
			float fTemp330 = ((0.5f * (fTemp252 * fRec99[2])) + (0.25f * (fTemp253 * (fRec99[1] + fRec99[3]))));
			float fTemp331 = (3.14159274f * (fRec34[0] * fTemp330));
			float fTemp332 = sinf(fTemp331);
			float fTemp333 = cosf(fTemp331);
			fRec100[0] = ((fRec100[1] * (0.0f - fTemp332)) + (fTemp330 * fTemp333));
			float fTemp334 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp260 + (fTemp258 + fTemp62))))));
			float fTemp335 = (fRec36[0] * fTemp334);
			float fTemp336 = ((fTemp249 + (((fTemp330 * fTemp332) + (fRec100[1] * fTemp333)) * powf(0.00100000005f, (fSlow14 / fTemp335)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec62[(IOTA & 8191)] = fTemp336;
			float fTemp337 = (fConst37 / fTemp334);
			float fTemp338 = (fTemp337 + -2.49999499f);
			int iTemp339 = min(4096, max(0, int(fTemp338)));
			float fTemp340 = floorf(fTemp338);
			float fTemp341 = ((fVec62[((IOTA - iTemp339) & 8191)] - fVec62[((IOTA - (iTemp339 + 1)) & 8191)]) * (fTemp340 + (2.0f - fTemp337)));
			fVec63[0] = fTemp341;
			fRec99[0] = ((0.5f * ((fTemp341 - fVec63[1]) * (fTemp340 + (3.0f - fTemp337)))) + (fVec62[((IOTA - iTemp339) & 8191)] + fTemp341));
			float fTemp342 = ((0.5f * (fTemp252 * fRec101[2])) + (0.25f * (fTemp253 * (fRec101[1] + fRec101[3]))));
			float fTemp343 = (3.14159274f * (fRec34[0] * fTemp342));
			float fTemp344 = sinf(fTemp343);
			float fTemp345 = cosf(fTemp343);
			fRec102[0] = ((fRec102[1] * (0.0f - fTemp344)) + (fTemp342 * fTemp345));
			float fTemp346 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp260 + (fTemp258 + fTemp49))))));
			float fTemp347 = (fRec36[0] * fTemp346);
			float fTemp348 = ((fTemp249 + (((fTemp342 * fTemp344) + (fRec102[1] * fTemp345)) * powf(0.00100000005f, (fSlow14 / fTemp347)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec64[(IOTA & 8191)] = fTemp348;
			float fTemp349 = (fConst37 / fTemp346);
			float fTemp350 = (fTemp349 + -2.49999499f);
			int iTemp351 = min(4096, max(0, int(fTemp350)));
			float fTemp352 = floorf(fTemp350);
			float fTemp353 = ((fVec64[((IOTA - iTemp351) & 8191)] - fVec64[((IOTA - (iTemp351 + 1)) & 8191)]) * (fTemp352 + (2.0f - fTemp349)));
			fVec65[0] = fTemp353;
			fRec101[0] = ((0.5f * ((fTemp353 - fVec65[1]) * (fTemp352 + (3.0f - fTemp349)))) + (fVec64[((IOTA - iTemp351) & 8191)] + fTemp353));
			float fTemp354 = ((0.5f * (fTemp252 * fRec103[2])) + (0.25f * (fTemp253 * (fRec103[1] + fRec103[3]))));
			float fTemp355 = (3.14159274f * (fRec34[0] * fTemp354));
			float fTemp356 = sinf(fTemp355);
			float fTemp357 = cosf(fTemp355);
			fRec104[0] = ((fRec104[1] * (0.0f - fTemp356)) + (fTemp354 * fTemp357));
			float fTemp358 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp260 + (fTemp258 + fTemp36))))));
			float fTemp359 = (fRec36[0] * fTemp358);
			float fTemp360 = ((fTemp249 + (((fTemp354 * fTemp356) + (fRec104[1] * fTemp357)) * powf(0.00100000005f, (fSlow14 / fTemp359)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec66[(IOTA & 8191)] = fTemp360;
			float fTemp361 = (fConst37 / fTemp358);
			float fTemp362 = (fTemp361 + -2.49999499f);
			int iTemp363 = min(4096, max(0, int(fTemp362)));
			float fTemp364 = floorf(fTemp362);
			float fTemp365 = ((fVec66[((IOTA - iTemp363) & 8191)] - fVec66[((IOTA - (iTemp363 + 1)) & 8191)]) * (fTemp364 + (2.0f - fTemp361)));
			fVec67[0] = fTemp365;
			fRec103[0] = ((0.5f * ((fTemp365 - fVec67[1]) * (fTemp364 + (3.0f - fTemp361)))) + (fVec66[((IOTA - iTemp363) & 8191)] + fTemp365));
			fVec68[0] = ((((((((fRec76[0] + fRec89[0]) + fRec91[0]) + fRec93[0]) + fRec95[0]) + fRec97[0]) + fRec99[0]) + fRec101[0]) + fRec103[0]);
			fRec75[0] = (((((((((((0.995000005f * fRec75[1]) + fRec76[0]) + fRec89[0]) + fRec91[0]) + fRec93[0]) + fRec95[0]) + fRec97[0]) + fRec99[0]) + fRec101[0]) + fRec103[0]) - fVec68[1]);
			float fRec2 = (0.00999999978f * fRec75[0]);
			float fTemp366 = (fRec32[0] * (fTemp248 - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]));
			float fTemp367 = (fTemp366 + 1.0f);
			float fTemp368 = (1.0f - fTemp366);
			float fTemp369 = ((0.5f * (fTemp367 * fRec106[2])) + (0.25f * (fTemp368 * (fRec106[1] + fRec106[3]))));
			float fTemp370 = (3.14159274f * (fRec34[0] * fTemp369));
			float fTemp371 = sinf(fTemp370);
			float fTemp372 = cosf(fTemp370);
			fRec107[0] = ((fRec107[1] * (0.0f - fTemp371)) + (fTemp369 * fTemp372));
			float fTemp373 = ((fTemp248 + (((fTemp369 * fTemp371) + (fRec107[1] * fTemp372)) * powf(0.00100000005f, (fSlow22 / fTemp359)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec69[(IOTA & 8191)] = fTemp373;
			float fTemp374 = (fSlow23 / fTemp358);
			float fTemp375 = (fTemp374 + -2.49999499f);
			int iTemp376 = min(4096, max(0, int(fTemp375)));
			float fTemp377 = floorf(fTemp375);
			float fTemp378 = ((fVec69[((IOTA - iTemp376) & 8191)] - fVec69[((IOTA - (iTemp376 + 1)) & 8191)]) * (fTemp377 + (2.0f - fTemp374)));
			fVec70[0] = fTemp378;
			fRec106[0] = ((0.5f * ((fTemp378 - fVec70[1]) * (fTemp377 + (3.0f - fTemp374)))) + (fVec69[((IOTA - iTemp376) & 8191)] + fTemp378));
			float fTemp379 = ((0.5f * (fTemp367 * fRec108[2])) + (0.25f * (fTemp368 * (fRec108[1] + fRec108[3]))));
			float fTemp380 = (3.14159274f * (fRec34[0] * fTemp379));
			float fTemp381 = sinf(fTemp380);
			float fTemp382 = cosf(fTemp380);
			fRec109[0] = ((fRec109[1] * (0.0f - fTemp381)) + (fTemp379 * fTemp382));
			float fTemp383 = ((fTemp248 + (((fTemp379 * fTemp381) + (fRec109[1] * fTemp382)) * powf(0.00100000005f, (fSlow22 / fTemp347)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec71[(IOTA & 8191)] = fTemp383;
			float fTemp384 = (fSlow23 / fTemp346);
			float fTemp385 = (fTemp384 + -2.49999499f);
			int iTemp386 = min(4096, max(0, int(fTemp385)));
			float fTemp387 = floorf(fTemp385);
			float fTemp388 = ((fVec71[((IOTA - iTemp386) & 8191)] - fVec71[((IOTA - (iTemp386 + 1)) & 8191)]) * (fTemp387 + (2.0f - fTemp384)));
			fVec72[0] = fTemp388;
			fRec108[0] = ((0.5f * ((fTemp388 - fVec72[1]) * (fTemp387 + (3.0f - fTemp384)))) + (fVec71[((IOTA - iTemp386) & 8191)] + fTemp388));
			float fTemp389 = ((0.5f * (fTemp367 * fRec110[2])) + (0.25f * (fTemp368 * (fRec110[1] + fRec110[3]))));
			float fTemp390 = (3.14159274f * (fRec34[0] * fTemp389));
			float fTemp391 = sinf(fTemp390);
			float fTemp392 = cosf(fTemp390);
			fRec111[0] = ((fRec111[1] * (0.0f - fTemp391)) + (fTemp389 * fTemp392));
			float fTemp393 = ((fTemp248 + (((fTemp389 * fTemp391) + (fRec111[1] * fTemp392)) * powf(0.00100000005f, (fSlow22 / fTemp335)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec73[(IOTA & 8191)] = fTemp393;
			float fTemp394 = (fSlow23 / fTemp334);
			float fTemp395 = (fTemp394 + -2.49999499f);
			int iTemp396 = min(4096, max(0, int(fTemp395)));
			float fTemp397 = floorf(fTemp395);
			float fTemp398 = ((fVec73[((IOTA - iTemp396) & 8191)] - fVec73[((IOTA - (iTemp396 + 1)) & 8191)]) * (fTemp397 + (2.0f - fTemp394)));
			fVec74[0] = fTemp398;
			fRec110[0] = ((0.5f * ((fTemp398 - fVec74[1]) * (fTemp397 + (3.0f - fTemp394)))) + (fVec73[((IOTA - iTemp396) & 8191)] + fTemp398));
			float fTemp399 = ((0.5f * (fTemp367 * fRec112[2])) + (0.25f * (fTemp368 * (fRec112[1] + fRec112[3]))));
			float fTemp400 = (3.14159274f * (fRec34[0] * fTemp399));
			float fTemp401 = sinf(fTemp400);
			float fTemp402 = cosf(fTemp400);
			fRec113[0] = ((fRec113[1] * (0.0f - fTemp401)) + (fTemp399 * fTemp402));
			float fTemp403 = ((fTemp248 + (((fTemp399 * fTemp401) + (fRec113[1] * fTemp402)) * powf(0.00100000005f, (fSlow22 / fTemp323)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec75[(IOTA & 8191)] = fTemp403;
			float fTemp404 = (fSlow23 / fTemp322);
			float fTemp405 = (fTemp404 + -2.49999499f);
			int iTemp406 = min(4096, max(0, int(fTemp405)));
			float fTemp407 = floorf(fTemp405);
			float fTemp408 = ((fVec75[((IOTA - iTemp406) & 8191)] - fVec75[((IOTA - (iTemp406 + 1)) & 8191)]) * (fTemp407 + (2.0f - fTemp404)));
			fVec76[0] = fTemp408;
			fRec112[0] = ((0.5f * ((fTemp408 - fVec76[1]) * (fTemp407 + (3.0f - fTemp404)))) + (fVec75[((IOTA - iTemp406) & 8191)] + fTemp408));
			float fTemp409 = ((0.5f * (fTemp367 * fRec114[2])) + (0.25f * (fTemp368 * (fRec114[1] + fRec114[3]))));
			float fTemp410 = (3.14159274f * (fRec34[0] * fTemp409));
			float fTemp411 = sinf(fTemp410);
			float fTemp412 = cosf(fTemp410);
			fRec115[0] = ((fRec115[1] * (0.0f - fTemp411)) + (fTemp409 * fTemp412));
			float fTemp413 = ((fTemp248 + (((fTemp409 * fTemp411) + (fRec115[1] * fTemp412)) * powf(0.00100000005f, (fSlow22 / fTemp311)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec77[(IOTA & 8191)] = fTemp413;
			float fTemp414 = (fSlow23 / fTemp310);
			float fTemp415 = (fTemp414 + -2.49999499f);
			int iTemp416 = min(4096, max(0, int(fTemp415)));
			float fTemp417 = floorf(fTemp415);
			float fTemp418 = ((fVec77[((IOTA - iTemp416) & 8191)] - fVec77[((IOTA - (iTemp416 + 1)) & 8191)]) * (fTemp417 + (2.0f - fTemp414)));
			fVec78[0] = fTemp418;
			fRec114[0] = ((0.5f * ((fTemp418 - fVec78[1]) * (fTemp417 + (3.0f - fTemp414)))) + (fVec77[((IOTA - iTemp416) & 8191)] + fTemp418));
			float fTemp419 = ((0.5f * (fTemp367 * fRec116[2])) + (0.25f * (fTemp368 * (fRec116[1] + fRec116[3]))));
			float fTemp420 = (3.14159274f * (fRec34[0] * fTemp419));
			float fTemp421 = sinf(fTemp420);
			float fTemp422 = cosf(fTemp420);
			fRec117[0] = ((fRec117[1] * (0.0f - fTemp421)) + (fTemp419 * fTemp422));
			float fTemp423 = ((fTemp248 + (((fTemp419 * fTemp421) + (fRec117[1] * fTemp422)) * powf(0.00100000005f, (fSlow22 / fTemp299)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec79[(IOTA & 8191)] = fTemp423;
			float fTemp424 = (fSlow23 / fTemp298);
			float fTemp425 = (fTemp424 + -2.49999499f);
			int iTemp426 = min(4096, max(0, int(fTemp425)));
			float fTemp427 = floorf(fTemp425);
			float fTemp428 = ((fVec79[((IOTA - iTemp426) & 8191)] - fVec79[((IOTA - (iTemp426 + 1)) & 8191)]) * (fTemp427 + (2.0f - fTemp424)));
			fVec80[0] = fTemp428;
			fRec116[0] = ((0.5f * ((fTemp428 - fVec80[1]) * (fTemp427 + (3.0f - fTemp424)))) + (fVec79[((IOTA - iTemp426) & 8191)] + fTemp428));
			float fTemp429 = ((0.5f * (fTemp367 * fRec118[2])) + (0.25f * (fTemp368 * (fRec118[1] + fRec118[3]))));
			float fTemp430 = (3.14159274f * (fRec34[0] * fTemp429));
			float fTemp431 = sinf(fTemp430);
			float fTemp432 = cosf(fTemp430);
			fRec119[0] = ((fTemp429 * fTemp432) + (fRec119[1] * (0.0f - fTemp431)));
			float fTemp433 = ((fTemp248 + (((fTemp429 * fTemp431) + (fRec119[1] * fTemp432)) * powf(0.00100000005f, (fSlow22 / fTemp287)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec81[(IOTA & 8191)] = fTemp433;
			float fTemp434 = (fSlow23 / fTemp286);
			float fTemp435 = (fTemp434 + -2.49999499f);
			int iTemp436 = min(4096, max(0, int(fTemp435)));
			float fTemp437 = floorf(fTemp435);
			float fTemp438 = ((fVec81[((IOTA - iTemp436) & 8191)] - fVec81[((IOTA - (iTemp436 + 1)) & 8191)]) * (fTemp437 + (2.0f - fTemp434)));
			fVec82[0] = fTemp438;
			fRec118[0] = ((0.5f * ((fTemp438 - fVec82[1]) * (fTemp437 + (3.0f - fTemp434)))) + (fVec81[((IOTA - iTemp436) & 8191)] + fTemp438));
			float fTemp439 = ((0.25f * ((fRec120[3] + fRec120[1]) * fTemp368)) + (0.5f * (fTemp367 * fRec120[2])));
			float fTemp440 = (3.14159274f * (fRec34[0] * fTemp439));
			float fTemp441 = sinf(fTemp440);
			float fTemp442 = cosf(fTemp440);
			fRec121[0] = ((fRec121[1] * (0.0f - fTemp441)) + (fTemp439 * fTemp442));
			float fTemp443 = ((fTemp248 + (((fTemp439 * fTemp441) + (fRec121[1] * fTemp442)) * powf(0.00100000005f, (fSlow22 / fTemp275)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec83[(IOTA & 8191)] = fTemp443;
			float fTemp444 = (fSlow23 / fTemp274);
			float fTemp445 = (fTemp444 + -2.49999499f);
			int iTemp446 = min(4096, max(0, int(fTemp445)));
			float fTemp447 = floorf(fTemp445);
			float fTemp448 = ((fVec83[((IOTA - iTemp446) & 8191)] - fVec83[((IOTA - (iTemp446 + 1)) & 8191)]) * (fTemp447 + (2.0f - fTemp444)));
			fVec84[0] = fTemp448;
			fRec120[0] = (((0.5f * ((fTemp448 - fVec84[1]) * (fTemp447 + (3.0f - fTemp444)))) + fVec83[((IOTA - iTemp446) & 8191)]) + fTemp448);
			float fTemp449 = ((0.5f * (fTemp367 * fRec122[2])) + (0.25f * (fTemp368 * (fRec122[1] + fRec122[3]))));
			float fTemp450 = (3.14159274f * (fRec34[0] * fTemp449));
			float fTemp451 = sinf(fTemp450);
			float fTemp452 = cosf(fTemp450);
			fRec123[0] = ((fRec123[1] * (0.0f - fTemp451)) + (fTemp449 * fTemp452));
			float fTemp453 = ((fTemp248 + (((fTemp449 * fTemp451) + (fRec123[1] * fTemp452)) * powf(0.00100000005f, (fSlow22 / fTemp263)))) - fVec48[((IOTA - int(min(4096.0f, max(0.0f, (fConst38 * ((fSlow11 + (0.200000003f * fRec85[0])) / fTemp15)))))) & 8191)]);
			fVec85[(IOTA & 8191)] = fTemp453;
			float fTemp454 = (fSlow23 / fTemp262);
			float fTemp455 = (fTemp454 + -2.49999499f);
			int iTemp456 = min(4096, max(0, int(fTemp455)));
			float fTemp457 = floorf(fTemp455);
			float fTemp458 = ((fVec85[((IOTA - iTemp456) & 8191)] - fVec85[((IOTA - (iTemp456 + 1)) & 8191)]) * (fTemp457 + (2.0f - fTemp454)));
			fVec86[0] = fTemp458;
			fRec122[0] = ((0.5f * ((fTemp458 - fVec86[1]) * (fTemp457 + (3.0f - fTemp454)))) + (fVec85[((IOTA - iTemp456) & 8191)] + fTemp458));
			fVec87[0] = ((((((((fRec122[0] + fRec120[0]) + fRec118[0]) + fRec116[0]) + fRec114[0]) + fRec112[0]) + fRec110[0]) + fRec108[0]) + fRec106[0]);
			fRec105[0] = ((fRec106[0] + (fRec108[0] + (fRec110[0] + (fRec112[0] + (fRec114[0] + (fRec116[0] + (fRec118[0] + (fRec120[0] + ((0.995000005f * fRec105[1]) + fRec122[0]))))))))) - fVec87[1]);
			fRec3[0] = (0.00999999978f * fRec105[0]);
			float fTemp459 = (fTemp7 + fTemp242);
			float fTemp460 = (fSlow1 * float(((fRec27[0] > 0.5f) & (fRec27[0] < 0.75f))));
			float fTemp461 = ((1.0f - fTemp460) - fSlow29);
			fVec88[0] = fTemp461;
			float fTemp462 = ((fTemp461 != fVec88[1])?(fSlow30 / fTemp15):(fRec128[1] + -1.0f));
			fRec128[0] = fTemp462;
			fRec129[0] = ((fTemp462 <= 0.0f)?fTemp461:(fRec129[1] + (((1.0f - (fTemp460 + fRec129[1])) - fSlow29) / fTemp462)));
			float fTemp463 = (fSlow29 + fTemp460);
			fVec89[0] = fTemp463;
			float fTemp464 = (fRec129[0] * fTemp463);
			float fTemp465 = ((fRec126[1] > fTemp464)?fConst32:expf((0.0f - (fSlow31 * fTemp15))));
			fRec127[0] = ((fRec127[1] * fTemp465) + (fTemp464 * (1.0f - fTemp465)));
			fRec126[0] = fRec127[0];
			float fTemp466 = (fTemp21 * fRec126[0]);
			fVec90[(IOTA & 8191)] = fTemp466;
			float fTemp467 = ((fConst6 * (fTemp11 + fTemp459)) + fTemp466);
			int iTemp468 = ((fVec89[1] <= 0.0f) & (fTemp463 > 0.0f));
			fRec130[0] = ((fRec130[1] * float((1 - iTemp468))) + (fTemp25 * float(iTemp468)));
			float fTemp469 = (fRec32[0] * (fTemp467 - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]));
			float fTemp470 = (fTemp469 + 1.0f);
			float fTemp471 = (1.0f - fTemp469);
			float fTemp472 = ((0.5f * (fTemp470 * fRec125[2])) + (0.25f * (fTemp471 * (fRec125[1] + fRec125[3]))));
			float fTemp473 = (3.14159274f * (fRec34[0] * fTemp472));
			float fTemp474 = sinf(fTemp473);
			float fTemp475 = cosf(fTemp473);
			fRec131[0] = ((fRec131[1] * (0.0f - fTemp474)) + (fTemp472 * fTemp475));
			fVec91[0] = fSlow33;
			float fTemp476 = ((fSlow33 != fVec91[1])?fSlow19:(fRec132[1] + -1.0f));
			fRec132[0] = fTemp476;
			fRec133[0] = ((fTemp476 <= 0.0f)?fSlow33:(fRec133[1] + (((1.0f - fRec133[1]) - fSlow29) / fTemp476)));
			float fTemp477 = (fSlow32 * tambura_faustpower2_f(fRec133[0]));
			float fTemp478 = (884.400024f * fTemp15);
			float fTemp479 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp477 + (fRec39[0] + fTemp478))))));
			float fTemp480 = (fRec36[0] * fTemp479);
			float fTemp481 = ((fTemp467 + (((fTemp472 * fTemp474) + (fRec131[1] * fTemp475)) * powf(0.00100000005f, (fSlow14 / fTemp480)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec92[(IOTA & 8191)] = fTemp481;
			float fTemp482 = (fConst37 / fTemp479);
			float fTemp483 = (fTemp482 + -2.49999499f);
			int iTemp484 = min(4096, max(0, int(fTemp483)));
			float fTemp485 = floorf(fTemp483);
			float fTemp486 = ((fVec92[((IOTA - iTemp484) & 8191)] - fVec92[((IOTA - (iTemp484 + 1)) & 8191)]) * (fTemp485 + (2.0f - fTemp482)));
			fVec93[0] = fTemp486;
			fRec125[0] = ((0.5f * ((fTemp486 - fVec93[1]) * (fTemp485 + (3.0f - fTemp482)))) + (fVec92[((IOTA - iTemp484) & 8191)] + fTemp486));
			float fTemp487 = ((0.5f * (fTemp470 * fRec134[2])) + (0.25f * (fTemp471 * (fRec134[1] + fRec134[3]))));
			float fTemp488 = (3.14159274f * (fRec34[0] * fTemp487));
			float fTemp489 = sinf(fTemp488);
			float fTemp490 = cosf(fTemp488);
			fRec135[0] = ((fRec135[1] * (0.0f - fTemp489)) + (fTemp487 * fTemp490));
			float fTemp491 = (fTemp478 + fTemp477);
			float fTemp492 = powf(2.0f, (1.44269502f * logf((0.0022727272f * fTemp491))));
			float fTemp493 = (fRec36[0] * fTemp492);
			float fTemp494 = ((fTemp467 + (((fTemp487 * fTemp489) + (fRec135[1] * fTemp490)) * powf(0.00100000005f, (fSlow14 / fTemp493)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec94[(IOTA & 8191)] = fTemp494;
			float fTemp495 = (fConst37 / fTemp492);
			float fTemp496 = (fTemp495 + -2.49999499f);
			int iTemp497 = min(4096, max(0, int(fTemp496)));
			float fTemp498 = floorf(fTemp496);
			float fTemp499 = ((fVec94[((IOTA - iTemp497) & 8191)] - fVec94[((IOTA - (iTemp497 + 1)) & 8191)]) * (fTemp498 + (2.0f - fTemp495)));
			fVec95[0] = fTemp499;
			fRec134[0] = ((0.5f * ((fTemp499 - fVec95[1]) * (fTemp498 + (3.0f - fTemp495)))) + (fVec94[((IOTA - iTemp497) & 8191)] + fTemp499));
			float fTemp500 = ((0.5f * (fTemp470 * fRec136[2])) + (0.25f * (fTemp471 * (fRec136[1] + fRec136[3]))));
			float fTemp501 = (3.14159274f * (fRec34[0] * fTemp500));
			float fTemp502 = sinf(fTemp501);
			float fTemp503 = cosf(fTemp501);
			fRec137[0] = ((fRec137[1] * (0.0f - fTemp502)) + (fTemp500 * fTemp503));
			float fTemp504 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp491 - fRec39[0])))));
			float fTemp505 = (fRec36[0] * fTemp504);
			float fTemp506 = ((fTemp467 + (((fTemp500 * fTemp502) + (fRec137[1] * fTemp503)) * powf(0.00100000005f, (fSlow14 / fTemp505)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec96[(IOTA & 8191)] = fTemp506;
			float fTemp507 = (fConst37 / fTemp504);
			float fTemp508 = (fTemp507 + -2.49999499f);
			int iTemp509 = min(4096, max(0, int(fTemp508)));
			float fTemp510 = floorf(fTemp508);
			float fTemp511 = ((fVec96[((IOTA - iTemp509) & 8191)] - fVec96[((IOTA - (iTemp509 + 1)) & 8191)]) * (fTemp510 + (2.0f - fTemp507)));
			fVec97[0] = fTemp511;
			fRec136[0] = ((0.5f * ((fTemp511 - fVec97[1]) * (fTemp510 + (3.0f - fTemp507)))) + (fVec96[((IOTA - iTemp509) & 8191)] + fTemp511));
			float fTemp512 = ((0.5f * (fTemp470 * fRec138[2])) + (0.25f * (fTemp471 * (fRec138[1] + fRec138[3]))));
			float fTemp513 = (3.14159274f * (fRec34[0] * fTemp512));
			float fTemp514 = sinf(fTemp513);
			float fTemp515 = cosf(fTemp513);
			fRec139[0] = ((fRec139[1] * (0.0f - fTemp514)) + (fTemp512 * fTemp515));
			float fTemp516 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp491 - fTemp62)))));
			float fTemp517 = (fRec36[0] * fTemp516);
			float fTemp518 = ((fTemp467 + (((fTemp512 * fTemp514) + (fRec139[1] * fTemp515)) * powf(0.00100000005f, (fSlow14 / fTemp517)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec98[(IOTA & 8191)] = fTemp518;
			float fTemp519 = (fConst37 / fTemp516);
			float fTemp520 = (fTemp519 + -2.49999499f);
			int iTemp521 = min(4096, max(0, int(fTemp520)));
			float fTemp522 = floorf(fTemp520);
			float fTemp523 = ((fVec98[((IOTA - iTemp521) & 8191)] - fVec98[((IOTA - (iTemp521 + 1)) & 8191)]) * (fTemp522 + (2.0f - fTemp519)));
			fVec99[0] = fTemp523;
			fRec138[0] = ((0.5f * ((fTemp523 - fVec99[1]) * (fTemp522 + (3.0f - fTemp519)))) + (fVec98[((IOTA - iTemp521) & 8191)] + fTemp523));
			float fTemp524 = ((0.5f * (fTemp470 * fRec140[2])) + (0.25f * (fTemp471 * (fRec140[1] + fRec140[3]))));
			float fTemp525 = (3.14159274f * (fRec34[0] * fTemp524));
			float fTemp526 = sinf(fTemp525);
			float fTemp527 = cosf(fTemp525);
			fRec141[0] = ((fRec141[1] * (0.0f - fTemp526)) + (fTemp524 * fTemp527));
			float fTemp528 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp491 - fTemp49)))));
			float fTemp529 = (fRec36[0] * fTemp528);
			float fTemp530 = ((fTemp467 + (((fTemp524 * fTemp526) + (fRec141[1] * fTemp527)) * powf(0.00100000005f, (fSlow14 / fTemp529)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec100[(IOTA & 8191)] = fTemp530;
			float fTemp531 = (fConst37 / fTemp528);
			float fTemp532 = (fTemp531 + -2.49999499f);
			int iTemp533 = min(4096, max(0, int(fTemp532)));
			float fTemp534 = floorf(fTemp532);
			float fTemp535 = ((fVec100[((IOTA - iTemp533) & 8191)] - fVec100[((IOTA - (iTemp533 + 1)) & 8191)]) * (fTemp534 + (2.0f - fTemp531)));
			fVec101[0] = fTemp535;
			fRec140[0] = ((0.5f * ((fTemp535 - fVec101[1]) * (fTemp534 + (3.0f - fTemp531)))) + (fVec100[((IOTA - iTemp533) & 8191)] + fTemp535));
			float fTemp536 = ((0.5f * (fTemp470 * fRec142[2])) + (0.25f * (fTemp471 * (fRec142[1] + fRec142[3]))));
			float fTemp537 = (3.14159274f * (fRec34[0] * fTemp536));
			float fTemp538 = sinf(fTemp537);
			float fTemp539 = cosf(fTemp537);
			fRec143[0] = ((fRec143[1] * (0.0f - fTemp538)) + (fTemp536 * fTemp539));
			float fTemp540 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp491 - fTemp36)))));
			float fTemp541 = (fRec36[0] * fTemp540);
			float fTemp542 = ((fTemp467 + (((fTemp536 * fTemp538) + (fRec143[1] * fTemp539)) * powf(0.00100000005f, (fSlow14 / fTemp541)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec102[(IOTA & 8191)] = fTemp542;
			float fTemp543 = (fConst37 / fTemp540);
			float fTemp544 = (fTemp543 + -2.49999499f);
			int iTemp545 = min(4096, max(0, int(fTemp544)));
			float fTemp546 = floorf(fTemp544);
			float fTemp547 = ((fVec102[((IOTA - iTemp545) & 8191)] - fVec102[((IOTA - (iTemp545 + 1)) & 8191)]) * (fTemp546 + (2.0f - fTemp543)));
			fVec103[0] = fTemp547;
			fRec142[0] = ((0.5f * ((fTemp547 - fVec103[1]) * (fTemp546 + (3.0f - fTemp543)))) + (fVec102[((IOTA - iTemp545) & 8191)] + fTemp547));
			float fTemp548 = ((0.5f * (fTemp470 * fRec144[2])) + (0.25f * (fTemp471 * (fRec144[1] + fRec144[3]))));
			float fTemp549 = (3.14159274f * (fRec34[0] * fTemp548));
			float fTemp550 = sinf(fTemp549);
			float fTemp551 = cosf(fTemp549);
			fRec145[0] = ((fRec145[1] * (0.0f - fTemp550)) + (fTemp548 * fTemp551));
			float fTemp552 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp477 + (fTemp36 + fTemp478))))));
			float fTemp553 = (fRec36[0] * fTemp552);
			float fTemp554 = ((fTemp467 + (((fTemp548 * fTemp550) + (fRec145[1] * fTemp551)) * powf(0.00100000005f, (fSlow14 / fTemp553)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec104[(IOTA & 8191)] = fTemp554;
			float fTemp555 = (fConst37 / fTemp552);
			float fTemp556 = (fTemp555 + -2.49999499f);
			int iTemp557 = min(4096, max(0, int(fTemp556)));
			float fTemp558 = floorf(fTemp556);
			float fTemp559 = ((fVec104[((IOTA - iTemp557) & 8191)] - fVec104[((IOTA - (iTemp557 + 1)) & 8191)]) * (fTemp558 + (2.0f - fTemp555)));
			fVec105[0] = fTemp559;
			fRec144[0] = ((0.5f * ((fTemp559 - fVec105[1]) * (fTemp558 + (3.0f - fTemp555)))) + (fVec104[((IOTA - iTemp557) & 8191)] + fTemp559));
			float fTemp560 = ((0.5f * (fTemp470 * fRec146[2])) + (0.25f * (fTemp471 * (fRec146[1] + fRec146[3]))));
			float fTemp561 = (3.14159274f * (fRec34[0] * fTemp560));
			float fTemp562 = sinf(fTemp561);
			float fTemp563 = cosf(fTemp561);
			fRec147[0] = ((fRec147[1] * (0.0f - fTemp562)) + (fTemp560 * fTemp563));
			float fTemp564 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp477 + (fTemp62 + fTemp478))))));
			float fTemp565 = (fRec36[0] * fTemp564);
			float fTemp566 = ((fTemp467 + (((fTemp560 * fTemp562) + (fRec147[1] * fTemp563)) * powf(0.00100000005f, (fSlow14 / fTemp565)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec106[(IOTA & 8191)] = fTemp566;
			float fTemp567 = (fConst37 / fTemp564);
			float fTemp568 = (fTemp567 + -2.49999499f);
			int iTemp569 = min(4096, max(0, int(fTemp568)));
			float fTemp570 = floorf(fTemp568);
			float fTemp571 = ((fVec106[((IOTA - iTemp569) & 8191)] - fVec106[((IOTA - (iTemp569 + 1)) & 8191)]) * (fTemp570 + (2.0f - fTemp567)));
			fVec107[0] = fTemp571;
			fRec146[0] = ((0.5f * ((fTemp571 - fVec107[1]) * (fTemp570 + (3.0f - fTemp567)))) + (fVec106[((IOTA - iTemp569) & 8191)] + fTemp571));
			float fTemp572 = ((0.5f * (fTemp470 * fRec148[2])) + (0.25f * (fTemp471 * (fRec148[1] + fRec148[3]))));
			float fTemp573 = (3.14159274f * (fRec34[0] * fTemp572));
			float fTemp574 = sinf(fTemp573);
			float fTemp575 = cosf(fTemp573);
			fRec149[0] = ((fRec149[1] * (0.0f - fTemp574)) + (fTemp572 * fTemp575));
			float fTemp576 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp477 + (fTemp49 + fTemp478))))));
			float fTemp577 = (fRec36[0] * fTemp576);
			float fTemp578 = ((fTemp467 + (((fTemp572 * fTemp574) + (fRec149[1] * fTemp575)) * powf(0.00100000005f, (fSlow14 / fTemp577)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec108[(IOTA & 8191)] = fTemp578;
			float fTemp579 = (fConst37 / fTemp576);
			float fTemp580 = (fTemp579 + -2.49999499f);
			int iTemp581 = min(4096, max(0, int(fTemp580)));
			float fTemp582 = floorf(fTemp580);
			float fTemp583 = ((fVec108[((IOTA - iTemp581) & 8191)] - fVec108[((IOTA - (iTemp581 + 1)) & 8191)]) * (fTemp582 + (2.0f - fTemp579)));
			fVec109[0] = fTemp583;
			fRec148[0] = ((0.5f * ((fTemp583 - fVec109[1]) * (fTemp582 + (3.0f - fTemp579)))) + (fVec108[((IOTA - iTemp581) & 8191)] + fTemp583));
			float fTemp584 = (fRec125[0] + (fRec134[0] + (fRec136[0] + (fRec138[0] + (fRec140[0] + (fRec142[0] + (fRec144[0] + (fRec146[0] + fRec148[0]))))))));
			fVec110[0] = fTemp584;
			fRec124[0] = (((0.995000005f * fRec124[1]) + fTemp584) - fVec110[1]);
			float fRec4 = (0.00999999978f * fRec124[0]);
			float fTemp585 = (fRec32[0] * (fTemp466 - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]));
			float fTemp586 = (fTemp585 + 1.0f);
			float fTemp587 = (1.0f - fTemp585);
			float fTemp588 = ((0.5f * (fTemp586 * fRec151[2])) + (0.25f * (fTemp587 * (fRec151[1] + fRec151[3]))));
			float fTemp589 = (3.14159274f * (fRec34[0] * fTemp588));
			float fTemp590 = sinf(fTemp589);
			float fTemp591 = cosf(fTemp589);
			fRec152[0] = ((fRec152[1] * (0.0f - fTemp590)) + (fTemp588 * fTemp591));
			float fTemp592 = ((fTemp466 + (((fTemp588 * fTemp590) + (fRec152[1] * fTemp591)) * powf(0.00100000005f, (fSlow22 / fTemp541)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec111[(IOTA & 8191)] = fTemp592;
			float fTemp593 = (fSlow23 / fTemp540);
			float fTemp594 = (fTemp593 + -2.49999499f);
			int iTemp595 = min(4096, max(0, int(fTemp594)));
			float fTemp596 = floorf(fTemp594);
			float fTemp597 = ((fVec111[((IOTA - iTemp595) & 8191)] - fVec111[((IOTA - (iTemp595 + 1)) & 8191)]) * (fTemp596 + (2.0f - fTemp593)));
			fVec112[0] = fTemp597;
			fRec151[0] = ((0.5f * ((fTemp597 - fVec112[1]) * (fTemp596 + (3.0f - fTemp593)))) + (fVec111[((IOTA - iTemp595) & 8191)] + fTemp597));
			float fTemp598 = ((0.5f * (fTemp586 * fRec153[2])) + (0.25f * (fTemp587 * (fRec153[1] + fRec153[3]))));
			float fTemp599 = (3.14159274f * (fRec34[0] * fTemp598));
			float fTemp600 = sinf(fTemp599);
			float fTemp601 = cosf(fTemp599);
			fRec154[0] = ((fRec154[1] * (0.0f - fTemp600)) + (fTemp598 * fTemp601));
			float fTemp602 = ((fTemp466 + (((fTemp598 * fTemp600) + (fRec154[1] * fTemp601)) * powf(0.00100000005f, (fSlow22 / fTemp529)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec113[(IOTA & 8191)] = fTemp602;
			float fTemp603 = (fSlow23 / fTemp528);
			float fTemp604 = (fTemp603 + -2.49999499f);
			int iTemp605 = min(4096, max(0, int(fTemp604)));
			float fTemp606 = floorf(fTemp604);
			float fTemp607 = ((fVec113[((IOTA - iTemp605) & 8191)] - fVec113[((IOTA - (iTemp605 + 1)) & 8191)]) * (fTemp606 + (2.0f - fTemp603)));
			fVec114[0] = fTemp607;
			fRec153[0] = ((0.5f * ((fTemp607 - fVec114[1]) * (fTemp606 + (3.0f - fTemp603)))) + (fVec113[((IOTA - iTemp605) & 8191)] + fTemp607));
			float fTemp608 = ((0.5f * (fTemp586 * fRec155[2])) + (0.25f * (fTemp587 * (fRec155[1] + fRec155[3]))));
			float fTemp609 = (3.14159274f * (fRec34[0] * fTemp608));
			float fTemp610 = sinf(fTemp609);
			float fTemp611 = cosf(fTemp609);
			fRec156[0] = ((fRec156[1] * (0.0f - fTemp610)) + (fTemp608 * fTemp611));
			float fTemp612 = ((fTemp466 + (((fTemp608 * fTemp610) + (fRec156[1] * fTemp611)) * powf(0.00100000005f, (fSlow22 / fTemp517)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec115[(IOTA & 8191)] = fTemp612;
			float fTemp613 = (fSlow23 / fTemp516);
			float fTemp614 = (fTemp613 + -2.49999499f);
			int iTemp615 = min(4096, max(0, int(fTemp614)));
			float fTemp616 = floorf(fTemp614);
			float fTemp617 = ((fVec115[((IOTA - iTemp615) & 8191)] - fVec115[((IOTA - (iTemp615 + 1)) & 8191)]) * (fTemp616 + (2.0f - fTemp613)));
			fVec116[0] = fTemp617;
			fRec155[0] = ((0.5f * ((fTemp617 - fVec116[1]) * (fTemp616 + (3.0f - fTemp613)))) + (fVec115[((IOTA - iTemp615) & 8191)] + fTemp617));
			float fTemp618 = ((0.5f * (fTemp586 * fRec157[2])) + (0.25f * (fTemp587 * (fRec157[1] + fRec157[3]))));
			float fTemp619 = (3.14159274f * (fRec34[0] * fTemp618));
			float fTemp620 = sinf(fTemp619);
			float fTemp621 = cosf(fTemp619);
			fRec158[0] = ((fRec158[1] * (0.0f - fTemp620)) + (fTemp618 * fTemp621));
			float fTemp622 = ((fTemp466 + (((fTemp618 * fTemp620) + (fRec158[1] * fTemp621)) * powf(0.00100000005f, (fSlow22 / fTemp505)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec117[(IOTA & 8191)] = fTemp622;
			float fTemp623 = (fSlow23 / fTemp504);
			float fTemp624 = (fTemp623 + -2.49999499f);
			int iTemp625 = min(4096, max(0, int(fTemp624)));
			float fTemp626 = floorf(fTemp624);
			float fTemp627 = ((fVec117[((IOTA - iTemp625) & 8191)] - fVec117[((IOTA - (iTemp625 + 1)) & 8191)]) * (fTemp626 + (2.0f - fTemp623)));
			fVec118[0] = fTemp627;
			fRec157[0] = ((0.5f * ((fTemp627 - fVec118[1]) * (fTemp626 + (3.0f - fTemp623)))) + (fVec117[((IOTA - iTemp625) & 8191)] + fTemp627));
			float fTemp628 = ((0.5f * (fTemp586 * fRec159[2])) + (0.25f * (fTemp587 * (fRec159[1] + fRec159[3]))));
			float fTemp629 = (3.14159274f * (fRec34[0] * fTemp628));
			float fTemp630 = sinf(fTemp629);
			float fTemp631 = cosf(fTemp629);
			fRec160[0] = ((fRec160[1] * (0.0f - fTemp630)) + (fTemp628 * fTemp631));
			float fTemp632 = ((fTemp466 + (((fTemp628 * fTemp630) + (fRec160[1] * fTemp631)) * powf(0.00100000005f, (fSlow22 / fTemp493)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec119[(IOTA & 8191)] = fTemp632;
			float fTemp633 = (fSlow23 / fTemp492);
			float fTemp634 = (fTemp633 + -2.49999499f);
			int iTemp635 = min(4096, max(0, int(fTemp634)));
			float fTemp636 = floorf(fTemp634);
			float fTemp637 = ((fVec119[((IOTA - iTemp635) & 8191)] - fVec119[((IOTA - (iTemp635 + 1)) & 8191)]) * (fTemp636 + (2.0f - fTemp633)));
			fVec120[0] = fTemp637;
			fRec159[0] = ((0.5f * ((fTemp637 - fVec120[1]) * (fTemp636 + (3.0f - fTemp633)))) + (fVec119[((IOTA - iTemp635) & 8191)] + fTemp637));
			float fTemp638 = (fSlow23 / fTemp479);
			float fTemp639 = (fTemp638 + -2.49999499f);
			float fTemp640 = floorf(fTemp639);
			float fTemp641 = ((0.5f * (fTemp586 * fRec161[2])) + (0.25f * (fTemp587 * (fRec161[1] + fRec161[3]))));
			float fTemp642 = (3.14159274f * (fRec34[0] * fTemp641));
			float fTemp643 = sinf(fTemp642);
			float fTemp644 = cosf(fTemp642);
			fRec162[0] = ((fRec162[1] * (0.0f - fTemp643)) + (fTemp641 * fTemp644));
			float fTemp645 = ((fTemp466 + (((fTemp641 * fTemp643) + (fRec162[1] * fTemp644)) * powf(0.00100000005f, (fSlow22 / fTemp480)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec121[(IOTA & 8191)] = fTemp645;
			int iTemp646 = min(4096, max(0, int(fTemp639)));
			float fTemp647 = ((fTemp640 + (2.0f - fTemp638)) * (fVec121[((IOTA - iTemp646) & 8191)] - fVec121[((IOTA - (iTemp646 + 1)) & 8191)]));
			fVec122[0] = fTemp647;
			fRec161[0] = ((fTemp647 + (0.5f * ((fTemp647 - fVec122[1]) * (fTemp640 + (3.0f - fTemp638))))) + fVec121[((IOTA - iTemp646) & 8191)]);
			float fTemp648 = ((0.5f * (fTemp586 * fRec163[2])) + (0.25f * (fTemp587 * (fRec163[1] + fRec163[3]))));
			float fTemp649 = (3.14159274f * (fRec34[0] * fTemp648));
			float fTemp650 = sinf(fTemp649);
			float fTemp651 = cosf(fTemp649);
			fRec164[0] = ((fRec164[1] * (0.0f - fTemp650)) + (fTemp648 * fTemp651));
			float fTemp652 = ((fTemp466 + (((fRec164[1] * fTemp651) + (fTemp648 * fTemp650)) * powf(0.00100000005f, (fSlow22 / fTemp565)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec123[(IOTA & 8191)] = fTemp652;
			float fTemp653 = (fSlow23 / fTemp564);
			float fTemp654 = (fTemp653 + -2.49999499f);
			int iTemp655 = min(4096, max(0, int(fTemp654)));
			float fTemp656 = floorf(fTemp654);
			float fTemp657 = ((fVec123[((IOTA - iTemp655) & 8191)] - fVec123[((IOTA - (iTemp655 + 1)) & 8191)]) * (fTemp656 + (2.0f - fTemp653)));
			fVec124[0] = fTemp657;
			fRec163[0] = ((0.5f * ((fTemp657 - fVec124[1]) * (fTemp656 + (3.0f - fTemp653)))) + (fVec123[((IOTA - iTemp655) & 8191)] + fTemp657));
			float fTemp658 = ((0.5f * (fTemp586 * fRec165[2])) + (0.25f * (fTemp587 * (fRec165[1] + fRec165[3]))));
			float fTemp659 = (3.14159274f * (fRec34[0] * fTemp658));
			float fTemp660 = sinf(fTemp659);
			float fTemp661 = cosf(fTemp659);
			fRec166[0] = ((fRec166[1] * (0.0f - fTemp660)) + (fTemp658 * fTemp661));
			float fTemp662 = ((fTemp466 + (((fTemp658 * fTemp660) + (fRec166[1] * fTemp661)) * powf(0.00100000005f, (fSlow22 / fTemp577)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec125[(IOTA & 8191)] = fTemp662;
			float fTemp663 = (fSlow23 / fTemp576);
			float fTemp664 = (fTemp663 + -2.49999499f);
			int iTemp665 = min(4096, max(0, int(fTemp664)));
			float fTemp666 = floorf(fTemp664);
			float fTemp667 = ((fVec125[((IOTA - iTemp665) & 8191)] - fVec125[((IOTA - (iTemp665 + 1)) & 8191)]) * (fTemp666 + (2.0f - fTemp663)));
			fVec126[0] = fTemp667;
			fRec165[0] = ((0.5f * ((fTemp667 - fVec126[1]) * (fTemp666 + (3.0f - fTemp663)))) + (fVec125[((IOTA - iTemp665) & 8191)] + fTemp667));
			float fTemp668 = ((0.5f * (fTemp586 * fRec167[2])) + (0.25f * (fTemp587 * (fRec167[1] + fRec167[3]))));
			float fTemp669 = (3.14159274f * (fRec34[0] * fTemp668));
			float fTemp670 = sinf(fTemp669);
			float fTemp671 = cosf(fTemp669);
			fRec168[0] = ((fRec168[1] * (0.0f - fTemp670)) + (fTemp668 * fTemp671));
			float fTemp672 = ((fTemp466 + (((fTemp668 * fTemp670) + (fRec168[1] * fTemp671)) * powf(0.00100000005f, (fSlow22 / fTemp553)))) - fVec90[((IOTA - int(min(4096.0f, max(0.0f, (fConst40 * ((fSlow11 + (0.200000003f * fRec130[0])) / fTemp15)))))) & 8191)]);
			fVec127[(IOTA & 8191)] = fTemp672;
			float fTemp673 = (fSlow23 / fTemp552);
			float fTemp674 = (fTemp673 + -2.49999499f);
			int iTemp675 = min(4096, max(0, int(fTemp674)));
			float fTemp676 = floorf(fTemp674);
			float fTemp677 = ((fVec127[((IOTA - iTemp675) & 8191)] - fVec127[((IOTA - (iTemp675 + 1)) & 8191)]) * (fTemp676 + (2.0f - fTemp673)));
			fVec128[0] = fTemp677;
			fRec167[0] = ((0.5f * ((fTemp677 - fVec128[1]) * (fTemp676 + (3.0f - fTemp673)))) + (fVec127[((IOTA - iTemp675) & 8191)] + fTemp677));
			float fTemp678 = ((((((((fRec151[0] + fRec153[0]) + fRec155[0]) + fRec157[0]) + fRec159[0]) + fRec161[0]) + fRec163[0]) + fRec165[0]) + fRec167[0]);
			fVec129[0] = fTemp678;
			fRec150[0] = ((fTemp678 + (0.995000005f * fRec150[1])) - fVec129[1]);
			fRec5[0] = (0.00999999978f * fRec150[0]);
			float fTemp679 = (fConst6 * (fTemp3 + fTemp459));
			float fTemp680 = (fSlow1 * float(((fRec27[0] > 0.75f) & (fRec27[0] < 1.0f))));
			float fTemp681 = ((1.0f - fTemp680) - fSlow34);
			fVec130[0] = fTemp681;
			float fTemp682 = ((fTemp681 != fVec130[1])?(fSlow35 / fTemp15):(fRec173[1] + -1.0f));
			fRec173[0] = fTemp682;
			fRec174[0] = ((fTemp682 <= 0.0f)?fTemp681:(fRec174[1] + (((1.0f - (fTemp680 + fRec174[1])) - fSlow34) / fTemp682)));
			float fTemp683 = (fSlow34 + fTemp680);
			fVec131[0] = fTemp683;
			float fTemp684 = (fRec174[0] * fTemp683);
			float fTemp685 = ((fRec171[1] > fTemp684)?fConst32:expf((0.0f - (fSlow36 * fTemp15))));
			fRec172[0] = ((fRec172[1] * fTemp685) + (fTemp684 * (1.0f - fTemp685)));
			fRec171[0] = fRec172[0];
			float fTemp686 = (fRec171[0] * fTemp21);
			fVec132[(IOTA & 8191)] = fTemp686;
			float fTemp687 = (fTemp679 + fTemp686);
			int iTemp688 = ((fVec131[1] <= 0.0f) & (fTemp683 > 0.0f));
			fRec175[0] = ((fRec175[1] * float((1 - iTemp688))) + (fTemp25 * float(iTemp688)));
			float fTemp689 = (fRec32[0] * (fTemp687 - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]));
			float fTemp690 = (fTemp689 + 1.0f);
			float fTemp691 = (1.0f - fTemp689);
			float fTemp692 = ((0.5f * (fTemp690 * fRec170[2])) + (0.25f * (fTemp691 * (fRec170[1] + fRec170[3]))));
			float fTemp693 = (3.14159274f * (fRec34[0] * fTemp692));
			float fTemp694 = sinf(fTemp693);
			float fTemp695 = cosf(fTemp693);
			fRec176[0] = ((fRec176[1] * (0.0f - fTemp694)) + (fTemp692 * fTemp695));
			float fTemp696 = (440.0f * fTemp15);
			fVec133[0] = fSlow38;
			float fTemp697 = ((fSlow38 != fVec133[1])?fSlow19:(fRec177[1] + -1.0f));
			fRec177[0] = fTemp697;
			fRec178[0] = ((fTemp697 <= 0.0f)?fSlow38:(fRec178[1] + (((1.0f - fRec178[1]) - fSlow34) / fTemp697)));
			float fTemp698 = (fSlow37 * tambura_faustpower2_f(fRec178[0]));
			float fTemp699 = (fTemp696 + fTemp698);
			float fTemp700 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp699 - fRec39[0])))));
			float fTemp701 = (fRec36[0] * fTemp700);
			float fTemp702 = ((fTemp687 + (((fTemp692 * fTemp694) + (fRec176[1] * fTemp695)) * powf(0.00100000005f, (fSlow14 / fTemp701)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec134[(IOTA & 8191)] = fTemp702;
			float fTemp703 = (fConst37 / fTemp700);
			float fTemp704 = (fTemp703 + -2.49999499f);
			int iTemp705 = min(4096, max(0, int(fTemp704)));
			float fTemp706 = floorf(fTemp704);
			float fTemp707 = ((fVec134[((IOTA - iTemp705) & 8191)] - fVec134[((IOTA - (iTemp705 + 1)) & 8191)]) * (fTemp706 + (2.0f - fTemp703)));
			fVec135[0] = fTemp707;
			fRec170[0] = ((0.5f * ((fTemp707 - fVec135[1]) * (fTemp706 + (3.0f - fTemp703)))) + (fVec134[((IOTA - iTemp705) & 8191)] + fTemp707));
			float fTemp708 = ((0.5f * (fTemp690 * fRec179[2])) + (0.25f * (fTemp691 * (fRec179[1] + fRec179[3]))));
			float fTemp709 = (3.14159274f * (fRec34[0] * fTemp708));
			float fTemp710 = sinf(fTemp709);
			float fTemp711 = cosf(fTemp709);
			fRec180[0] = ((fRec180[1] * (0.0f - fTemp710)) + (fTemp708 * fTemp711));
			float fTemp712 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp699 - fTemp62)))));
			float fTemp713 = (fRec36[0] * fTemp712);
			float fTemp714 = ((fTemp687 + (((fTemp708 * fTemp710) + (fRec180[1] * fTemp711)) * powf(0.00100000005f, (fSlow14 / fTemp713)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec136[(IOTA & 8191)] = fTemp714;
			float fTemp715 = (fConst37 / fTemp712);
			float fTemp716 = (fTemp715 + -2.49999499f);
			int iTemp717 = min(4096, max(0, int(fTemp716)));
			float fTemp718 = floorf(fTemp716);
			float fTemp719 = ((fVec136[((IOTA - iTemp717) & 8191)] - fVec136[((IOTA - (iTemp717 + 1)) & 8191)]) * (fTemp718 + (2.0f - fTemp715)));
			fVec137[0] = fTemp719;
			fRec179[0] = ((0.5f * ((fTemp719 - fVec137[1]) * (fTemp718 + (3.0f - fTemp715)))) + (fVec136[((IOTA - iTemp717) & 8191)] + fTemp719));
			float fTemp720 = powf(2.0f, (1.44269502f * logf((0.0022727272f * ((fTemp696 + fTemp36) + fTemp698)))));
			float fTemp721 = (fRec36[0] * fTemp720);
			float fTemp722 = ((0.5f * (fTemp690 * fRec181[2])) + (0.25f * (fTemp691 * (fRec181[1] + fRec181[3]))));
			float fTemp723 = (3.14159274f * (fRec34[0] * fTemp722));
			float fTemp724 = sinf(fTemp723);
			float fTemp725 = cosf(fTemp723);
			fRec182[0] = ((fRec182[1] * (0.0f - fTemp724)) + (fTemp722 * fTemp725));
			float fTemp726 = ((((powf(0.00100000005f, (fSlow14 / fTemp721)) * ((fTemp722 * fTemp724) + (fRec182[1] * fTemp725))) + fTemp679) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec138[(IOTA & 8191)] = fTemp726;
			float fTemp727 = (fConst37 / fTemp720);
			float fTemp728 = (fTemp727 + -2.49999499f);
			int iTemp729 = min(4096, max(0, int(fTemp728)));
			float fTemp730 = floorf(fTemp728);
			float fTemp731 = ((fVec138[((IOTA - iTemp729) & 8191)] - fVec138[((IOTA - (iTemp729 + 1)) & 8191)]) * (fTemp730 + (2.0f - fTemp727)));
			fVec139[0] = fTemp731;
			fRec181[0] = ((0.5f * ((fTemp731 - fVec139[1]) * (fTemp730 + (3.0f - fTemp727)))) + (fVec138[((IOTA - iTemp729) & 8191)] + fTemp731));
			float fTemp732 = ((0.5f * (fTemp690 * fRec183[2])) + (0.25f * (fTemp691 * (fRec183[1] + fRec183[3]))));
			float fTemp733 = (3.14159274f * (fRec34[0] * fTemp732));
			float fTemp734 = sinf(fTemp733);
			float fTemp735 = cosf(fTemp733);
			fRec184[0] = ((fRec184[1] * (0.0f - fTemp734)) + (fTemp732 * fTemp735));
			float fTemp736 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp698 + (fTemp696 + fTemp49))))));
			float fTemp737 = (fRec36[0] * fTemp736);
			float fTemp738 = ((fTemp687 + (((fTemp732 * fTemp734) + (fRec184[1] * fTemp735)) * powf(0.00100000005f, (fSlow14 / fTemp737)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec140[(IOTA & 8191)] = fTemp738;
			float fTemp739 = (fConst37 / fTemp736);
			float fTemp740 = (fTemp739 + -2.49999499f);
			int iTemp741 = min(4096, max(0, int(fTemp740)));
			float fTemp742 = floorf(fTemp740);
			float fTemp743 = ((fVec140[((IOTA - iTemp741) & 8191)] - fVec140[((IOTA - (iTemp741 + 1)) & 8191)]) * (fTemp742 + (2.0f - fTemp739)));
			fVec141[0] = fTemp743;
			fRec183[0] = ((0.5f * ((fTemp743 - fVec141[1]) * (fTemp742 + (3.0f - fTemp739)))) + (fVec140[((IOTA - iTemp741) & 8191)] + fTemp743));
			float fTemp744 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp698 + (fTemp696 + fTemp62))))));
			float fTemp745 = (fRec36[0] * fTemp744);
			float fTemp746 = ((0.5f * (fTemp690 * fRec185[2])) + (0.25f * (fTemp691 * (fRec185[1] + fRec185[3]))));
			float fTemp747 = (3.14159274f * (fRec34[0] * fTemp746));
			float fTemp748 = sinf(fTemp747);
			float fTemp749 = cosf(fTemp747);
			fRec186[0] = ((fRec186[1] * (0.0f - fTemp748)) + (fTemp746 * fTemp749));
			float fTemp750 = ((((powf(0.00100000005f, (fSlow14 / fTemp745)) * ((fTemp746 * fTemp748) + (fRec186[1] * fTemp749))) + fTemp679) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec142[(IOTA & 8191)] = fTemp750;
			float fTemp751 = (fConst37 / fTemp744);
			float fTemp752 = (fTemp751 + -2.49999499f);
			int iTemp753 = min(4096, max(0, int(fTemp752)));
			float fTemp754 = floorf(fTemp752);
			float fTemp755 = ((fVec142[((IOTA - iTemp753) & 8191)] - fVec142[((IOTA - (iTemp753 + 1)) & 8191)]) * (fTemp754 + (2.0f - fTemp751)));
			fVec143[0] = fTemp755;
			fRec185[0] = ((0.5f * ((fTemp755 - fVec143[1]) * (fTemp754 + (3.0f - fTemp751)))) + (fVec142[((IOTA - iTemp753) & 8191)] + fTemp755));
			float fTemp756 = ((0.5f * (fTemp690 * fRec187[2])) + (0.25f * (fTemp691 * (fRec187[1] + fRec187[3]))));
			float fTemp757 = (3.14159274f * (fRec34[0] * fTemp756));
			float fTemp758 = sinf(fTemp757);
			float fTemp759 = cosf(fTemp757);
			fRec188[0] = ((fRec188[1] * (0.0f - fTemp758)) + (fTemp756 * fTemp759));
			float fTemp760 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp698 + (fRec39[0] + fTemp696))))));
			float fTemp761 = (fRec36[0] * fTemp760);
			float fTemp762 = ((fTemp687 + (((fTemp756 * fTemp758) + (fRec188[1] * fTemp759)) * powf(0.00100000005f, (fSlow14 / fTemp761)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec144[(IOTA & 8191)] = fTemp762;
			float fTemp763 = (fConst37 / fTemp760);
			float fTemp764 = (fTemp763 + -2.49999499f);
			int iTemp765 = min(4096, max(0, int(fTemp764)));
			float fTemp766 = floorf(fTemp764);
			float fTemp767 = ((fVec144[((IOTA - iTemp765) & 8191)] - fVec144[((IOTA - (iTemp765 + 1)) & 8191)]) * (fTemp766 + (2.0f - fTemp763)));
			fVec145[0] = fTemp767;
			fRec187[0] = ((0.5f * ((fTemp767 - fVec145[1]) * (fTemp766 + (3.0f - fTemp763)))) + (fVec144[((IOTA - iTemp765) & 8191)] + fTemp767));
			float fTemp768 = ((0.5f * (fTemp690 * fRec189[2])) + (0.25f * (fTemp691 * (fRec189[1] + fRec189[3]))));
			float fTemp769 = (3.14159274f * (fRec34[0] * fTemp768));
			float fTemp770 = sinf(fTemp769);
			float fTemp771 = cosf(fTemp769);
			fRec190[0] = ((fRec190[1] * (0.0f - fTemp770)) + (fTemp768 * fTemp771));
			float fTemp772 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp699 - fTemp36)))));
			float fTemp773 = (fRec36[0] * fTemp772);
			float fTemp774 = ((fTemp687 + (((fTemp768 * fTemp770) + (fRec190[1] * fTemp771)) * powf(0.00100000005f, (fSlow14 / fTemp773)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec146[(IOTA & 8191)] = fTemp774;
			float fTemp775 = (fConst37 / fTemp772);
			float fTemp776 = (fTemp775 + -2.49999499f);
			int iTemp777 = min(4096, max(0, int(fTemp776)));
			float fTemp778 = floorf(fTemp776);
			float fTemp779 = ((fVec146[((IOTA - iTemp777) & 8191)] - fVec146[((IOTA - (iTemp777 + 1)) & 8191)]) * (fTemp778 + (2.0f - fTemp775)));
			fVec147[0] = fTemp779;
			fRec189[0] = ((0.5f * ((fTemp779 - fVec147[1]) * (fTemp778 + (3.0f - fTemp775)))) + (fVec146[((IOTA - iTemp777) & 8191)] + fTemp779));
			float fTemp780 = ((0.5f * (fTemp690 * fRec191[2])) + (0.25f * (fTemp691 * (fRec191[1] + fRec191[3]))));
			float fTemp781 = (3.14159274f * (fRec34[0] * fTemp780));
			float fTemp782 = sinf(fTemp781);
			float fTemp783 = cosf(fTemp781);
			fRec192[0] = ((fRec192[1] * (0.0f - fTemp782)) + (fTemp780 * fTemp783));
			float fTemp784 = powf(2.0f, (1.44269502f * logf((0.0022727272f * (fTemp699 - fTemp49)))));
			float fTemp785 = (fRec36[0] * fTemp784);
			float fTemp786 = ((fTemp687 + (((fTemp780 * fTemp782) + (fRec192[1] * fTemp783)) * powf(0.00100000005f, (fSlow14 / fTemp785)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec148[(IOTA & 8191)] = fTemp786;
			float fTemp787 = (fConst37 / fTemp784);
			float fTemp788 = (fTemp787 + -2.49999499f);
			int iTemp789 = min(4096, max(0, int(fTemp788)));
			float fTemp790 = floorf(fTemp788);
			float fTemp791 = ((fVec148[((IOTA - iTemp789) & 8191)] - fVec148[((IOTA - (iTemp789 + 1)) & 8191)]) * (fTemp790 + (2.0f - fTemp787)));
			fVec149[0] = fTemp791;
			fRec191[0] = ((0.5f * ((fTemp791 - fVec149[1]) * (fTemp790 + (3.0f - fTemp787)))) + (fVec148[((IOTA - iTemp789) & 8191)] + fTemp791));
			float fTemp792 = ((0.5f * (fTemp690 * fRec193[2])) + (0.25f * (fTemp691 * (fRec193[1] + fRec193[3]))));
			float fTemp793 = (3.14159274f * (fRec34[0] * fTemp792));
			float fTemp794 = sinf(fTemp793);
			float fTemp795 = cosf(fTemp793);
			fRec194[0] = ((fRec194[1] * (0.0f - fTemp794)) + (fTemp792 * fTemp795));
			float fTemp796 = powf(2.0f, (1.44269502f * logf((0.0022727272f * fTemp699))));
			float fTemp797 = (fRec36[0] * fTemp796);
			float fTemp798 = ((fTemp687 + (((fTemp792 * fTemp794) + (fRec194[1] * fTemp795)) * powf(0.00100000005f, (fSlow14 / fTemp797)))) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec150[(IOTA & 8191)] = fTemp798;
			float fTemp799 = (fConst37 / fTemp796);
			float fTemp800 = (fTemp799 + -2.49999499f);
			int iTemp801 = min(4096, max(0, int(fTemp800)));
			float fTemp802 = floorf(fTemp800);
			float fTemp803 = ((fVec150[((IOTA - iTemp801) & 8191)] - fVec150[((IOTA - (iTemp801 + 1)) & 8191)]) * (fTemp802 + (2.0f - fTemp799)));
			fVec151[0] = fTemp803;
			fRec193[0] = ((0.5f * ((fTemp803 - fVec151[1]) * (fTemp802 + (3.0f - fTemp799)))) + (fVec150[((IOTA - iTemp801) & 8191)] + fTemp803));
			float fTemp804 = (fRec170[0] + (fRec179[0] + (fRec181[0] + (fRec183[0] + (fRec185[0] + (fRec187[0] + ((fRec189[0] + fRec191[0]) + fRec193[0])))))));
			fVec152[0] = fTemp804;
			fRec169[0] = ((fTemp804 + (0.995000005f * fRec169[1])) - fVec152[1]);
			float fRec6 = (0.00999999978f * fRec169[0]);
			float fTemp805 = (fRec32[0] * (fTemp686 - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]));
			float fTemp806 = (fTemp805 + 1.0f);
			float fTemp807 = (1.0f - fTemp805);
			float fTemp808 = ((0.5f * (fTemp806 * fRec196[2])) + (0.25f * (fTemp807 * (fRec196[1] + fRec196[3]))));
			float fTemp809 = (3.14159274f * (fRec34[0] * fTemp808));
			float fTemp810 = sinf(fTemp809);
			float fTemp811 = cosf(fTemp809);
			fRec197[0] = ((fRec197[1] * (0.0f - fTemp810)) + (fTemp808 * fTemp811));
			float fTemp812 = (((((fTemp808 * fTemp810) + (fRec197[1] * fTemp811)) * powf(0.00100000005f, (fSlow22 / fTemp773))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec153[(IOTA & 8191)] = fTemp812;
			float fTemp813 = (fSlow23 / fTemp772);
			float fTemp814 = (fTemp813 + -2.49999499f);
			int iTemp815 = min(4096, max(0, int(fTemp814)));
			float fTemp816 = floorf(fTemp814);
			float fTemp817 = ((fVec153[((IOTA - iTemp815) & 8191)] - fVec153[((IOTA - (iTemp815 + 1)) & 8191)]) * (fTemp816 + (2.0f - fTemp813)));
			fVec154[0] = fTemp817;
			fRec196[0] = ((0.5f * ((fTemp817 - fVec154[1]) * (fTemp816 + (3.0f - fTemp813)))) + (fVec153[((IOTA - iTemp815) & 8191)] + fTemp817));
			float fTemp818 = ((0.5f * (fTemp806 * fRec198[2])) + (0.25f * (fTemp807 * (fRec198[1] + fRec198[3]))));
			float fTemp819 = (3.14159274f * (fRec34[0] * fTemp818));
			float fTemp820 = sinf(fTemp819);
			float fTemp821 = cosf(fTemp819);
			fRec199[0] = ((fRec199[1] * (0.0f - fTemp820)) + (fTemp818 * fTemp821));
			float fTemp822 = (((((fTemp818 * fTemp820) + (fRec199[1] * fTemp821)) * powf(0.00100000005f, (fSlow22 / fTemp785))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec155[(IOTA & 8191)] = fTemp822;
			float fTemp823 = (fSlow23 / fTemp784);
			float fTemp824 = (fTemp823 + -2.49999499f);
			int iTemp825 = min(4096, max(0, int(fTemp824)));
			float fTemp826 = floorf(fTemp824);
			float fTemp827 = ((fVec155[((IOTA - iTemp825) & 8191)] - fVec155[((IOTA - (iTemp825 + 1)) & 8191)]) * (fTemp826 + (2.0f - fTemp823)));
			fVec156[0] = fTemp827;
			fRec198[0] = ((0.5f * ((fTemp827 - fVec156[1]) * (fTemp826 + (3.0f - fTemp823)))) + (fVec155[((IOTA - iTemp825) & 8191)] + fTemp827));
			float fTemp828 = ((0.5f * (fTemp806 * fRec200[2])) + (0.25f * (fTemp807 * (fRec200[1] + fRec200[3]))));
			float fTemp829 = (3.14159274f * (fRec34[0] * fTemp828));
			float fTemp830 = sinf(fTemp829);
			float fTemp831 = cosf(fTemp829);
			fRec201[0] = ((fRec201[1] * (0.0f - fTemp830)) + (fTemp828 * fTemp831));
			float fTemp832 = (((((fTemp828 * fTemp830) + (fRec201[1] * fTemp831)) * powf(0.00100000005f, (fSlow22 / fTemp713))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec157[(IOTA & 8191)] = fTemp832;
			float fTemp833 = (fSlow23 / fTemp712);
			float fTemp834 = (fTemp833 + -2.49999499f);
			int iTemp835 = min(4096, max(0, int(fTemp834)));
			float fTemp836 = floorf(fTemp834);
			float fTemp837 = ((fVec157[((IOTA - iTemp835) & 8191)] - fVec157[((IOTA - (iTemp835 + 1)) & 8191)]) * (fTemp836 + (2.0f - fTemp833)));
			fVec158[0] = fTemp837;
			fRec200[0] = ((0.5f * ((fTemp837 - fVec158[1]) * (fTemp836 + (3.0f - fTemp833)))) + (fVec157[((IOTA - iTemp835) & 8191)] + fTemp837));
			float fTemp838 = ((0.5f * (fTemp806 * fRec202[2])) + (0.25f * (fTemp807 * (fRec202[1] + fRec202[3]))));
			float fTemp839 = (3.14159274f * (fRec34[0] * fTemp838));
			float fTemp840 = sinf(fTemp839);
			float fTemp841 = cosf(fTemp839);
			fRec203[0] = ((fRec203[1] * (0.0f - fTemp840)) + (fTemp838 * fTemp841));
			float fTemp842 = (((((fTemp838 * fTemp840) + (fRec203[1] * fTemp841)) * powf(0.00100000005f, (fSlow22 / fTemp701))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec159[(IOTA & 8191)] = fTemp842;
			float fTemp843 = (fSlow23 / fTemp700);
			float fTemp844 = (fTemp843 + -2.49999499f);
			int iTemp845 = min(4096, max(0, int(fTemp844)));
			float fTemp846 = floorf(fTemp844);
			float fTemp847 = ((fVec159[((IOTA - iTemp845) & 8191)] - fVec159[((IOTA - (iTemp845 + 1)) & 8191)]) * (fTemp846 + (2.0f - fTemp843)));
			fVec160[0] = fTemp847;
			fRec202[0] = ((0.5f * ((fTemp847 - fVec160[1]) * (fTemp846 + (3.0f - fTemp843)))) + (fVec159[((IOTA - iTemp845) & 8191)] + fTemp847));
			float fTemp848 = ((0.5f * (fTemp806 * fRec204[2])) + (0.25f * (fTemp807 * (fRec204[1] + fRec204[3]))));
			float fTemp849 = (3.14159274f * (fRec34[0] * fTemp848));
			float fTemp850 = sinf(fTemp849);
			float fTemp851 = cosf(fTemp849);
			fRec205[0] = ((fRec205[1] * (0.0f - fTemp850)) + (fTemp848 * fTemp851));
			float fTemp852 = (((((fTemp848 * fTemp850) + (fRec205[1] * fTemp851)) * powf(0.00100000005f, (fSlow22 / fTemp797))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec161[(IOTA & 8191)] = fTemp852;
			float fTemp853 = (fSlow23 / fTemp796);
			float fTemp854 = (fTemp853 + -2.49999499f);
			int iTemp855 = min(4096, max(0, int(fTemp854)));
			float fTemp856 = floorf(fTemp854);
			float fTemp857 = ((fVec161[((IOTA - iTemp855) & 8191)] - fVec161[((IOTA - (iTemp855 + 1)) & 8191)]) * (fTemp856 + (2.0f - fTemp853)));
			fVec162[0] = fTemp857;
			fRec204[0] = ((0.5f * ((fTemp857 - fVec162[1]) * (fTemp856 + (3.0f - fTemp853)))) + (fVec161[((IOTA - iTemp855) & 8191)] + fTemp857));
			float fTemp858 = (fSlow23 / fTemp760);
			float fTemp859 = (fTemp858 + -2.49999499f);
			float fTemp860 = floorf(fTemp859);
			float fTemp861 = ((0.5f * (fTemp806 * fRec206[2])) + (0.25f * (fTemp807 * (fRec206[1] + fRec206[3]))));
			float fTemp862 = (3.14159274f * (fRec34[0] * fTemp861));
			float fTemp863 = sinf(fTemp862);
			float fTemp864 = cosf(fTemp862);
			fRec207[0] = ((fRec207[1] * (0.0f - fTemp863)) + (fTemp861 * fTemp864));
			float fTemp865 = (((((fTemp861 * fTemp863) + (fRec207[1] * fTemp864)) * powf(0.00100000005f, (fSlow22 / fTemp761))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec163[(IOTA & 8191)] = fTemp865;
			int iTemp866 = min(4096, max(0, int(fTemp859)));
			float fTemp867 = ((fTemp860 + (2.0f - fTemp858)) * (fVec163[((IOTA - iTemp866) & 8191)] - fVec163[((IOTA - (iTemp866 + 1)) & 8191)]));
			fVec164[0] = fTemp867;
			fRec206[0] = ((fTemp867 + (0.5f * ((fTemp867 - fVec164[1]) * (fTemp860 + (3.0f - fTemp858))))) + fVec163[((IOTA - iTemp866) & 8191)]);
			float fTemp868 = ((0.5f * (fRec208[2] * fTemp806)) + (0.25f * ((fRec208[1] + fRec208[3]) * fTemp807)));
			float fTemp869 = (3.14159274f * (fRec34[0] * fTemp868));
			float fTemp870 = sinf(fTemp869);
			float fTemp871 = cosf(fTemp869);
			fRec209[0] = ((fRec209[1] * (0.0f - fTemp870)) + (fTemp868 * fTemp871));
			float fTemp872 = (((((fTemp868 * fTemp870) + (fRec209[1] * fTemp871)) * powf(0.00100000005f, (fSlow22 / fTemp745))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec165[(IOTA & 8191)] = fTemp872;
			float fTemp873 = (fSlow23 / fTemp744);
			float fTemp874 = (fTemp873 + -2.49999499f);
			int iTemp875 = min(4096, max(0, int(fTemp874)));
			float fTemp876 = floorf(fTemp874);
			float fTemp877 = ((fVec165[((IOTA - iTemp875) & 8191)] - fVec165[((IOTA - (iTemp875 + 1)) & 8191)]) * (fTemp876 + (2.0f - fTemp873)));
			fVec166[0] = fTemp877;
			fRec208[0] = ((0.5f * ((fTemp877 - fVec166[1]) * (fTemp876 + (3.0f - fTemp873)))) + (fVec165[((IOTA - iTemp875) & 8191)] + fTemp877));
			float fTemp878 = ((0.5f * (fTemp806 * fRec210[2])) + (0.25f * (fTemp807 * (fRec210[1] + fRec210[3]))));
			float fTemp879 = (3.14159274f * (fRec34[0] * fTemp878));
			float fTemp880 = sinf(fTemp879);
			float fTemp881 = cosf(fTemp879);
			fRec211[0] = ((fRec211[1] * (0.0f - fTemp880)) + (fTemp878 * fTemp881));
			float fTemp882 = (((((fTemp878 * fTemp880) + (fRec211[1] * fTemp881)) * powf(0.00100000005f, (fSlow22 / fTemp737))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec167[(IOTA & 8191)] = fTemp882;
			float fTemp883 = (fSlow23 / fTemp736);
			float fTemp884 = (fTemp883 + -2.49999499f);
			int iTemp885 = min(4096, max(0, int(fTemp884)));
			float fTemp886 = floorf(fTemp884);
			float fTemp887 = ((fVec167[((IOTA - iTemp885) & 8191)] - fVec167[((IOTA - (iTemp885 + 1)) & 8191)]) * (fTemp886 + (2.0f - fTemp883)));
			fVec168[0] = fTemp887;
			fRec210[0] = ((0.5f * ((fTemp887 - fVec168[1]) * (fTemp886 + (3.0f - fTemp883)))) + (fVec167[((IOTA - iTemp885) & 8191)] + fTemp887));
			float fTemp888 = ((0.5f * (fTemp806 * fRec212[2])) + (0.25f * (fTemp807 * (fRec212[1] + fRec212[3]))));
			float fTemp889 = (3.14159274f * (fRec34[0] * fTemp888));
			float fTemp890 = sinf(fTemp889);
			float fTemp891 = cosf(fTemp889);
			fRec213[0] = ((fRec213[1] * (0.0f - fTemp890)) + (fTemp888 * fTemp891));
			float fTemp892 = (((((fTemp888 * fTemp890) + (fRec213[1] * fTemp891)) * powf(0.00100000005f, (fSlow22 / fTemp721))) + fTemp686) - fVec132[((IOTA - int(min(4096.0f, max(0.0f, (fConst37 * ((fSlow11 + (0.200000003f * fRec175[0])) / fTemp15)))))) & 8191)]);
			fVec169[(IOTA & 8191)] = fTemp892;
			float fTemp893 = (fSlow23 / fTemp720);
			float fTemp894 = (fTemp893 + -2.49999499f);
			int iTemp895 = min(4096, max(0, int(fTemp894)));
			float fTemp896 = floorf(fTemp894);
			float fTemp897 = ((fVec169[((IOTA - iTemp895) & 8191)] - fVec169[((IOTA - (iTemp895 + 1)) & 8191)]) * (fTemp896 + (2.0f - fTemp893)));
			fVec170[0] = fTemp897;
			fRec212[0] = ((0.5f * ((fTemp897 - fVec170[1]) * (fTemp896 + (3.0f - fTemp893)))) + (fVec169[((IOTA - iTemp895) & 8191)] + fTemp897));
			float fTemp898 = ((((((((fRec196[0] + fRec198[0]) + fRec200[0]) + fRec202[0]) + fRec204[0]) + fRec206[0]) + fRec208[0]) + fRec210[0]) + fRec212[0]);
			fVec171[0] = fTemp898;
			fRec195[0] = ((fTemp898 + (0.995000005f * fRec195[1])) - fVec171[1]);
			fRec7[0] = (0.00999999978f * fRec195[0]);
			fRec214[0] = (fSlow39 + (fConst19 * fRec214[1]));
			float fTemp899 = (0.5f * (1.0f - fRec214[0]));
			float fTemp900 = (0.166666672f * fRec214[0]);
			float fTemp901 = (0.5f * (fRec214[0] + 1.0f));
			float fTemp902 = (0.166666672f * fRec214[0]);
			output0[i] = FAUSTFLOAT((((fRec0 + fRec1[0]) * (sqrtf((1.0f - fTemp899)) + sqrtf(fTemp899))) + (((fRec4 + fRec5[0]) * (sqrtf((0.5f - fTemp900)) + sqrtf((fTemp900 + 0.5f)))) + (((fRec6 + fRec7[0]) * (sqrtf((1.0f - fTemp901)) + sqrtf(fTemp901))) + ((fRec2 + fRec3[0]) * (sqrtf((fTemp902 + 0.5f)) + sqrtf((0.5f - fTemp902))))))));
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
			fRec27[1] = fRec27[0];
			fVec3[1] = fVec3[0];
			fRec28[1] = fRec28[0];
			fRec25[1] = fRec25[0];
			fRec26[1] = fRec26[0];
			fVec4[1] = fVec4[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec29[1] = fRec29[0];
			iRec31[1] = iRec31[0];
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec30[j0] = fRec30[(j0 - 1)];
				
			}
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
			fVec47[1] = fVec47[0];
			fRec83[1] = fRec83[0];
			fRec84[1] = fRec84[0];
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

    t_faust_tambura* pd_obj()
    {
        return this->x_;
    }
};

static void* tambura_faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s, bool soundIn = true)
{
    tambura_faust_class = class_new(s, reinterpret_cast<t_newmethod>(tambura_faust_new),
        reinterpret_cast<t_method>(tambura_faust_free),
        sizeof(t_faust_tambura),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);

    if (soundIn) {
        class_addmethod(tambura_faust_class, nullfn, &s_signal, A_NULL);
        CLASS_MAINSIGNALIN(tambura_faust_class, t_faust_tambura, f);
    }

    class_addmethod(tambura_faust_class, reinterpret_cast<t_method>(tambura_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(tambura_faust_class, reinterpret_cast<t_method>(tambura_dump_to_console), gensym("dump"), A_NULL);
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

#define EXTERNAL_SETUP_NO_IN(MOD)                      \
    extern "C" void setup_##MOD##0x2etambura_tilde()     \
    {                                                  \
        internal_setup(gensym(#MOD ".tambura~"), false); \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);

#endif
