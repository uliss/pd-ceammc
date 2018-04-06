/* ------------------------------------------------------------
author: "Mayank Sanganeria"
name: "Granulator"
version: "1.0"
Code generated with Faust 2.5.31 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __granulator_H__
#define  __granulator_H__

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
struct granulator : public dsp {
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


class granulatorSIG0 {
	
  private:
	
	
  public:
	
	int getNumInputsgranulatorSIG0() {
		return 0;
		
	}
	int getNumOutputsgranulatorSIG0() {
		return 1;
		
	}
	int getInputRategranulatorSIG0(int channel) {
		int rate;
		switch (channel) {
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	int getOutputRategranulatorSIG0(int channel) {
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
	
	void instanceInitgranulatorSIG0(int samplingFreq) {
		
	}
	
	void fillgranulatorSIG0(int count, float* output) {
		for (int i = 0; (i < count); i = (i + 1)) {
			output[i] = 0.0f;
			
		}
		
	}
};

granulatorSIG0* newgranulatorSIG0() { return (granulatorSIG0*)new granulatorSIG0(); }
void deletegranulatorSIG0(granulatorSIG0* dsp) { delete dsp; }


#ifndef FAUSTCLASS 
#define FAUSTCLASS granulator
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class granulator : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSamplingFreq;
	FAUSTFLOAT fHslider1;
	int iVec0[2];
	int iRec0[2];
	FAUSTFLOAT fHslider2;
	int iRec2[2];
	int iRec1[2];
	float ftbl0[480000];
	int iRec66[2];
	int iRec67[2];
	int iRec68[2];
	int iRec69[2];
	int iRec70[2];
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
	int iRec125[2];
	int iRec124[2];
	int iRec127[2];
	int iRec126[2];
	int iRec128[2];
	int iRec129[2];
	int iRec131[2];
	int iRec130[2];
	int iRec132[2];
	int iRec133[2];
	int iRec134[2];
	int iRec135[2];
	int iRec136[2];
	int iRec137[2];
	int iRec138[2];
	int iRec139[2];
	int iRec140[2];
	int iRec141[2];
	int iRec142[2];
	int iRec143[2];
	int iRec144[2];
	int iRec145[2];
	int iRec146[2];
	int iRec147[2];
	int iRec148[2];
	int iRec149[2];
	int iRec150[2];
	int iRec151[2];
	int iRec152[2];
	int iRec153[2];
	int iRec154[2];
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
	int iRec169[2];
	int iRec168[2];
	int iRec171[2];
	int iRec170[2];
	int iRec173[2];
	int iRec172[2];
	int iRec175[2];
	int iRec174[2];
	int iRec177[2];
	int iRec176[2];
	int iRec179[2];
	int iRec178[2];
	int iRec181[2];
	int iRec180[2];
	int iRec183[2];
	int iRec182[2];
	int iRec185[2];
	int iRec184[2];
	int iRec187[2];
	int iRec186[2];
	int iRec189[2];
	int iRec188[2];
	int iRec191[2];
	int iRec190[2];
	int iRec192[2];
	int iRec193[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Mayank Sanganeria");
		m->declare("filename", "fx_granulator");
		m->declare("name", "Granulator");
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
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		granulatorSIG0* sig0 = newgranulatorSIG0();
		sig0->instanceInitgranulatorSIG0(samplingFreq);
		sig0->fillgranulatorSIG0(480000, ftbl0);
		deletegranulatorSIG0(sig0);
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.10000000000000001f);
		fHslider2 = FAUSTFLOAT(10.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iVec0[l0] = 0;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iRec0[l1] = 0;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec2[l2] = 0;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec1[l3] = 0;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec66[l4] = 0;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec67[l5] = 0;
			
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
			iRec125[l62] = 0;
			
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			iRec124[l63] = 0;
			
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			iRec127[l64] = 0;
			
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			iRec126[l65] = 0;
			
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			iRec128[l66] = 0;
			
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			iRec129[l67] = 0;
			
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			iRec131[l68] = 0;
			
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			iRec130[l69] = 0;
			
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			iRec132[l70] = 0;
			
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			iRec133[l71] = 0;
			
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			iRec134[l72] = 0;
			
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
			iRec139[l77] = 0;
			
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			iRec140[l78] = 0;
			
		}
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			iRec141[l79] = 0;
			
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			iRec142[l80] = 0;
			
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			iRec143[l81] = 0;
			
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
			iRec150[l88] = 0;
			
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			iRec151[l89] = 0;
			
		}
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			iRec152[l90] = 0;
			
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			iRec153[l91] = 0;
			
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			iRec154[l92] = 0;
			
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
			iRec169[l106] = 0;
			
		}
		for (int l107 = 0; (l107 < 2); l107 = (l107 + 1)) {
			iRec168[l107] = 0;
			
		}
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			iRec171[l108] = 0;
			
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			iRec170[l109] = 0;
			
		}
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			iRec173[l110] = 0;
			
		}
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			iRec172[l111] = 0;
			
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			iRec175[l112] = 0;
			
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			iRec174[l113] = 0;
			
		}
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			iRec177[l114] = 0;
			
		}
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			iRec176[l115] = 0;
			
		}
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			iRec179[l116] = 0;
			
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			iRec178[l117] = 0;
			
		}
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			iRec181[l118] = 0;
			
		}
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			iRec180[l119] = 0;
			
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
			iRec191[l128] = 0;
			
		}
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			iRec190[l129] = 0;
			
		}
		for (int l130 = 0; (l130 < 2); l130 = (l130 + 1)) {
			iRec192[l130] = 0;
			
		}
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			iRec193[l131] = 0;
			
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
	
	virtual granulator* clone() {
		return new granulator();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Granulator");
		ui_interface->addHorizontalSlider("delay", &fHslider2, 10.0f, 0.5f, 10.0f, 0.100000001f);
		ui_interface->addHorizontalSlider("density", &fHslider0, 1.0f, 1.0f, 64.0f, 1.0f);
		ui_interface->addHorizontalSlider("length", &fHslider1, 0.100000001f, 0.00999999978f, 0.5f, 0.00999999978f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (1.0f / fSlow0);
		int iSlow2 = (22.0f < fSlow0);
		int iSlow3 = int((float(fSamplingFreq) * float(fHslider1)));
		int iSlow4 = (iSlow3 + -1);
		float fSlow5 = (6.28318024f / float(iSlow4));
		int iSlow6 = int((float(fSamplingFreq) * float(fHslider2)));
		float fSlow7 = (0.5f * float(iSlow6));
		float fSlow8 = (1.0f / float(iSlow4));
		int iSlow9 = (0.0f < fSlow0);
		int iSlow10 = (2.0f < fSlow0);
		int iSlow11 = (4.0f < fSlow0);
		int iSlow12 = (6.0f < fSlow0);
		int iSlow13 = (8.0f < fSlow0);
		int iSlow14 = (10.0f < fSlow0);
		int iSlow15 = (12.0f < fSlow0);
		int iSlow16 = (14.0f < fSlow0);
		int iSlow17 = (16.0f < fSlow0);
		int iSlow18 = (18.0f < fSlow0);
		int iSlow19 = (20.0f < fSlow0);
		int iSlow20 = (24.0f < fSlow0);
		int iSlow21 = (26.0f < fSlow0);
		int iSlow22 = (28.0f < fSlow0);
		int iSlow23 = (30.0f < fSlow0);
		int iSlow24 = (32.0f < fSlow0);
		int iSlow25 = (34.0f < fSlow0);
		int iSlow26 = (36.0f < fSlow0);
		int iSlow27 = (38.0f < fSlow0);
		int iSlow28 = (40.0f < fSlow0);
		int iSlow29 = (42.0f < fSlow0);
		int iSlow30 = (44.0f < fSlow0);
		int iSlow31 = (46.0f < fSlow0);
		int iSlow32 = (48.0f < fSlow0);
		int iSlow33 = (50.0f < fSlow0);
		int iSlow34 = (52.0f < fSlow0);
		int iSlow35 = (54.0f < fSlow0);
		int iSlow36 = (56.0f < fSlow0);
		int iSlow37 = (58.0f < fSlow0);
		int iSlow38 = (60.0f < fSlow0);
		int iSlow39 = (62.0f < fSlow0);
		int iSlow40 = (61.0f < fSlow0);
		int iSlow41 = (1.0f < fSlow0);
		int iSlow42 = (3.0f < fSlow0);
		int iSlow43 = (5.0f < fSlow0);
		int iSlow44 = (7.0f < fSlow0);
		int iSlow45 = (9.0f < fSlow0);
		int iSlow46 = (11.0f < fSlow0);
		int iSlow47 = (13.0f < fSlow0);
		int iSlow48 = (15.0f < fSlow0);
		int iSlow49 = (17.0f < fSlow0);
		int iSlow50 = (19.0f < fSlow0);
		int iSlow51 = (21.0f < fSlow0);
		int iSlow52 = (23.0f < fSlow0);
		int iSlow53 = (25.0f < fSlow0);
		int iSlow54 = (27.0f < fSlow0);
		int iSlow55 = (29.0f < fSlow0);
		int iSlow56 = (31.0f < fSlow0);
		int iSlow57 = (33.0f < fSlow0);
		int iSlow58 = (35.0f < fSlow0);
		int iSlow59 = (37.0f < fSlow0);
		int iSlow60 = (39.0f < fSlow0);
		int iSlow61 = (41.0f < fSlow0);
		int iSlow62 = (43.0f < fSlow0);
		int iSlow63 = (45.0f < fSlow0);
		int iSlow64 = (47.0f < fSlow0);
		int iSlow65 = (49.0f < fSlow0);
		int iSlow66 = (51.0f < fSlow0);
		int iSlow67 = (53.0f < fSlow0);
		int iSlow68 = (55.0f < fSlow0);
		int iSlow69 = (57.0f < fSlow0);
		int iSlow70 = (59.0f < fSlow0);
		int iSlow71 = (63.0f < fSlow0);
		for (int i = 0; (i < count); i = (i + 1)) {
			iVec0[0] = 1;
			iRec0[0] = ((iRec0[1] + 1) % iSlow3);
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
			int iTemp65 = int((fSlow7 * ((2.32830644e-10f * float(iRec24)) + 1.0f)));
			iRec1[0] = ((iVec0[1] * iRec1[1]) + (iTemp0 * iTemp65));
			int iTemp66 = ((iRec0[0] + iRec1[0]) % iSlow3);
			iRec66[0] = ((iRec66[1] + 1) % iSlow6);
			ftbl0[(iRec66[0] % iSlow6)] = float(input0[i]);
			int iTemp67 = int((fSlow8 * float(iTemp66)));
			iRec67[0] = (((1 - iTemp67) * iRec67[1]) + (iTemp67 * iTemp65));
			int iTemp68 = int((fSlow7 * ((2.32830644e-10f * float(iRec2[0])) + 1.0f)));
			iRec68[0] = ((iVec0[1] * iRec68[1]) + (iTemp0 * iTemp68));
			int iTemp69 = ((iRec0[0] + iRec68[0]) % iSlow3);
			int iTemp70 = int((fSlow8 * float(iTemp69)));
			iRec69[0] = ((iRec69[1] * (1 - iTemp70)) + (iTemp68 * iTemp70));
			int iTemp71 = int((fSlow7 * ((2.32830644e-10f * float(iRec4)) + 1.0f)));
			iRec70[0] = ((iVec0[1] * iRec70[1]) + (iTemp0 * iTemp71));
			int iTemp72 = ((iRec0[0] + iRec70[0]) % iSlow3);
			int iTemp73 = int((fSlow8 * float(iTemp72)));
			iRec71[0] = ((iRec71[1] * (1 - iTemp73)) + (iTemp71 * iTemp73));
			int iTemp74 = int((fSlow7 * ((2.32830644e-10f * float(iRec6)) + 1.0f)));
			iRec72[0] = ((iVec0[1] * iRec72[1]) + (iTemp0 * iTemp74));
			int iTemp75 = ((iRec0[0] + iRec72[0]) % iSlow3);
			int iTemp76 = int((fSlow8 * float(iTemp75)));
			iRec73[0] = ((iRec73[1] * (1 - iTemp76)) + (iTemp74 * iTemp76));
			int iTemp77 = int((fSlow7 * ((2.32830644e-10f * float(iRec8)) + 1.0f)));
			iRec74[0] = ((iVec0[1] * iRec74[1]) + (iTemp0 * iTemp77));
			int iTemp78 = ((iRec0[0] + iRec74[0]) % iSlow3);
			int iTemp79 = int((fSlow8 * float(iTemp78)));
			iRec75[0] = ((iRec75[1] * (1 - iTemp79)) + (iTemp77 * iTemp79));
			int iTemp80 = int((fSlow7 * ((2.32830644e-10f * float(iRec10)) + 1.0f)));
			iRec76[0] = ((iVec0[1] * iRec76[1]) + (iTemp0 * iTemp80));
			int iTemp81 = ((iRec0[0] + iRec76[0]) % iSlow3);
			int iTemp82 = int((fSlow8 * float(iTemp81)));
			iRec77[0] = ((iRec77[1] * (1 - iTemp82)) + (iTemp80 * iTemp82));
			int iTemp83 = int((fSlow7 * ((2.32830644e-10f * float(iRec12)) + 1.0f)));
			iRec78[0] = ((iVec0[1] * iRec78[1]) + (iTemp0 * iTemp83));
			int iTemp84 = ((iRec0[0] + iRec78[0]) % iSlow3);
			int iTemp85 = int((fSlow8 * float(iTemp84)));
			iRec79[0] = ((iRec79[1] * (1 - iTemp85)) + (iTemp83 * iTemp85));
			int iTemp86 = int((fSlow7 * ((2.32830644e-10f * float(iRec14)) + 1.0f)));
			iRec80[0] = ((iVec0[1] * iRec80[1]) + (iTemp0 * iTemp86));
			int iTemp87 = ((iRec0[0] + iRec80[0]) % iSlow3);
			int iTemp88 = int((fSlow8 * float(iTemp87)));
			iRec81[0] = ((iRec81[1] * (1 - iTemp88)) + (iTemp86 * iTemp88));
			int iTemp89 = int((fSlow7 * ((2.32830644e-10f * float(iRec16)) + 1.0f)));
			iRec82[0] = ((iVec0[1] * iRec82[1]) + (iTemp0 * iTemp89));
			int iTemp90 = ((iRec0[0] + iRec82[0]) % iSlow3);
			int iTemp91 = int((fSlow8 * float(iTemp90)));
			iRec83[0] = ((iRec83[1] * (1 - iTemp91)) + (iTemp89 * iTemp91));
			int iTemp92 = int((fSlow7 * ((2.32830644e-10f * float(iRec18)) + 1.0f)));
			iRec84[0] = ((iVec0[1] * iRec84[1]) + (iTemp0 * iTemp92));
			int iTemp93 = ((iRec0[0] + iRec84[0]) % iSlow3);
			int iTemp94 = int((fSlow8 * float(iTemp93)));
			iRec85[0] = ((iRec85[1] * (1 - iTemp94)) + (iTemp92 * iTemp94));
			int iTemp95 = int((fSlow7 * ((2.32830644e-10f * float(iRec20)) + 1.0f)));
			iRec86[0] = ((iVec0[1] * iRec86[1]) + (iTemp0 * iTemp95));
			int iTemp96 = ((iRec0[0] + iRec86[0]) % iSlow3);
			int iTemp97 = int((fSlow8 * float(iTemp96)));
			iRec87[0] = ((iRec87[1] * (1 - iTemp97)) + (iTemp95 * iTemp97));
			int iTemp98 = int((fSlow7 * ((2.32830644e-10f * float(iRec22)) + 1.0f)));
			iRec88[0] = ((iVec0[1] * iRec88[1]) + (iTemp0 * iTemp98));
			int iTemp99 = ((iRec0[0] + iRec88[0]) % iSlow3);
			int iTemp100 = int((fSlow8 * float(iTemp99)));
			iRec89[0] = ((iRec89[1] * (1 - iTemp100)) + (iTemp98 * iTemp100));
			int iTemp101 = int((fSlow7 * ((2.32830644e-10f * float(iRec26)) + 1.0f)));
			iRec90[0] = ((iVec0[1] * iRec90[1]) + (iTemp0 * iTemp101));
			int iTemp102 = ((iRec0[0] + iRec90[0]) % iSlow3);
			int iTemp103 = int((fSlow8 * float(iTemp102)));
			iRec91[0] = ((iRec91[1] * (1 - iTemp103)) + (iTemp101 * iTemp103));
			int iTemp104 = int((fSlow7 * ((2.32830644e-10f * float(iRec28)) + 1.0f)));
			iRec92[0] = ((iVec0[1] * iRec92[1]) + (iTemp0 * iTemp104));
			int iTemp105 = ((iRec0[0] + iRec92[0]) % iSlow3);
			int iTemp106 = int((fSlow8 * float(iTemp105)));
			iRec93[0] = ((iRec93[1] * (1 - iTemp106)) + (iTemp104 * iTemp106));
			int iTemp107 = int((fSlow7 * ((2.32830644e-10f * float(iRec30)) + 1.0f)));
			iRec94[0] = ((iVec0[1] * iRec94[1]) + (iTemp0 * iTemp107));
			int iTemp108 = ((iRec0[0] + iRec94[0]) % iSlow3);
			int iTemp109 = int((fSlow8 * float(iTemp108)));
			iRec95[0] = ((iRec95[1] * (1 - iTemp109)) + (iTemp107 * iTemp109));
			int iTemp110 = int((fSlow7 * ((2.32830644e-10f * float(iRec32)) + 1.0f)));
			iRec96[0] = ((iVec0[1] * iRec96[1]) + (iTemp0 * iTemp110));
			int iTemp111 = ((iRec0[0] + iRec96[0]) % iSlow3);
			int iTemp112 = int((fSlow8 * float(iTemp111)));
			iRec97[0] = ((iRec97[1] * (1 - iTemp112)) + (iTemp110 * iTemp112));
			int iTemp113 = int((fSlow7 * ((2.32830644e-10f * float(iRec34)) + 1.0f)));
			iRec98[0] = ((iVec0[1] * iRec98[1]) + (iTemp0 * iTemp113));
			int iTemp114 = ((iRec0[0] + iRec98[0]) % iSlow3);
			int iTemp115 = int((fSlow8 * float(iTemp114)));
			iRec99[0] = ((iRec99[1] * (1 - iTemp115)) + (iTemp113 * iTemp115));
			int iTemp116 = int((fSlow7 * ((2.32830644e-10f * float(iRec36)) + 1.0f)));
			iRec100[0] = ((iVec0[1] * iRec100[1]) + (iTemp0 * iTemp116));
			int iTemp117 = ((iRec0[0] + iRec100[0]) % iSlow3);
			int iTemp118 = int((fSlow8 * float(iTemp117)));
			iRec101[0] = ((iRec101[1] * (1 - iTemp118)) + (iTemp116 * iTemp118));
			int iTemp119 = int((fSlow7 * ((2.32830644e-10f * float(iRec38)) + 1.0f)));
			iRec102[0] = ((iVec0[1] * iRec102[1]) + (iTemp0 * iTemp119));
			int iTemp120 = ((iRec0[0] + iRec102[0]) % iSlow3);
			int iTemp121 = int((fSlow8 * float(iTemp120)));
			iRec103[0] = ((iRec103[1] * (1 - iTemp121)) + (iTemp119 * iTemp121));
			int iTemp122 = int((fSlow7 * ((2.32830644e-10f * float(iRec40)) + 1.0f)));
			iRec104[0] = ((iVec0[1] * iRec104[1]) + (iTemp0 * iTemp122));
			int iTemp123 = ((iRec0[0] + iRec104[0]) % iSlow3);
			int iTemp124 = int((fSlow8 * float(iTemp123)));
			iRec105[0] = ((iRec105[1] * (1 - iTemp124)) + (iTemp122 * iTemp124));
			int iTemp125 = int((fSlow7 * ((2.32830644e-10f * float(iRec42)) + 1.0f)));
			iRec106[0] = ((iVec0[1] * iRec106[1]) + (iTemp0 * iTemp125));
			int iTemp126 = ((iRec0[0] + iRec106[0]) % iSlow3);
			int iTemp127 = int((fSlow8 * float(iTemp126)));
			iRec107[0] = ((iRec107[1] * (1 - iTemp127)) + (iTemp125 * iTemp127));
			int iTemp128 = int((fSlow7 * ((2.32830644e-10f * float(iRec44)) + 1.0f)));
			iRec108[0] = ((iVec0[1] * iRec108[1]) + (iTemp0 * iTemp128));
			int iTemp129 = ((iRec0[0] + iRec108[0]) % iSlow3);
			int iTemp130 = int((fSlow8 * float(iTemp129)));
			iRec109[0] = ((iRec109[1] * (1 - iTemp130)) + (iTemp128 * iTemp130));
			int iTemp131 = int((fSlow7 * ((2.32830644e-10f * float(iRec46)) + 1.0f)));
			iRec110[0] = ((iVec0[1] * iRec110[1]) + (iTemp0 * iTemp131));
			int iTemp132 = ((iRec0[0] + iRec110[0]) % iSlow3);
			int iTemp133 = int((fSlow8 * float(iTemp132)));
			iRec111[0] = ((iRec111[1] * (1 - iTemp133)) + (iTemp131 * iTemp133));
			int iTemp134 = int((fSlow7 * ((2.32830644e-10f * float(iRec48)) + 1.0f)));
			iRec112[0] = ((iVec0[1] * iRec112[1]) + (iTemp0 * iTemp134));
			int iTemp135 = ((iRec0[0] + iRec112[0]) % iSlow3);
			int iTemp136 = int((fSlow8 * float(iTemp135)));
			iRec113[0] = ((iRec113[1] * (1 - iTemp136)) + (iTemp134 * iTemp136));
			int iTemp137 = int((fSlow7 * ((2.32830644e-10f * float(iRec50)) + 1.0f)));
			iRec114[0] = ((iVec0[1] * iRec114[1]) + (iTemp0 * iTemp137));
			int iTemp138 = ((iRec0[0] + iRec114[0]) % iSlow3);
			int iTemp139 = int((fSlow8 * float(iTemp138)));
			iRec115[0] = ((iRec115[1] * (1 - iTemp139)) + (iTemp137 * iTemp139));
			int iTemp140 = int((fSlow7 * ((2.32830644e-10f * float(iRec52)) + 1.0f)));
			iRec116[0] = ((iVec0[1] * iRec116[1]) + (iTemp0 * iTemp140));
			int iTemp141 = ((iRec0[0] + iRec116[0]) % iSlow3);
			int iTemp142 = int((fSlow8 * float(iTemp141)));
			iRec117[0] = ((iRec117[1] * (1 - iTemp142)) + (iTemp140 * iTemp142));
			int iTemp143 = int((fSlow7 * ((2.32830644e-10f * float(iRec54)) + 1.0f)));
			iRec118[0] = ((iVec0[1] * iRec118[1]) + (iTemp0 * iTemp143));
			int iTemp144 = ((iRec0[0] + iRec118[0]) % iSlow3);
			int iTemp145 = int((fSlow8 * float(iTemp144)));
			iRec119[0] = ((iRec119[1] * (1 - iTemp145)) + (iTemp143 * iTemp145));
			int iTemp146 = int((fSlow7 * ((2.32830644e-10f * float(iRec56)) + 1.0f)));
			iRec120[0] = ((iVec0[1] * iRec120[1]) + (iTemp0 * iTemp146));
			int iTemp147 = ((iRec0[0] + iRec120[0]) % iSlow3);
			int iTemp148 = int((fSlow8 * float(iTemp147)));
			iRec121[0] = ((iRec121[1] * (1 - iTemp148)) + (iTemp146 * iTemp148));
			int iTemp149 = int((fSlow7 * ((2.32830644e-10f * float(iRec58)) + 1.0f)));
			iRec122[0] = ((iVec0[1] * iRec122[1]) + (iTemp0 * iTemp149));
			int iTemp150 = ((iRec0[0] + iRec122[0]) % iSlow3);
			int iTemp151 = int((fSlow8 * float(iTemp150)));
			iRec123[0] = ((iRec123[1] * (1 - iTemp151)) + (iTemp149 * iTemp151));
			int iTemp152 = int((fSlow7 * ((2.32830644e-10f * float(iRec60)) + 1.0f)));
			iRec125[0] = ((iVec0[1] * iRec125[1]) + (iTemp0 * iTemp152));
			int iTemp153 = ((iRec0[0] + iRec125[0]) % iSlow3);
			int iTemp154 = int((fSlow8 * float(iTemp153)));
			iRec124[0] = ((iRec124[1] * (1 - iTemp154)) + (iTemp152 * iTemp154));
			int iTemp155 = int((fSlow7 * ((2.32830644e-10f * float(iRec62)) + 1.0f)));
			iRec127[0] = ((iVec0[1] * iRec127[1]) + (iTemp0 * iTemp155));
			int iTemp156 = ((iRec127[0] + iRec0[0]) % iSlow3);
			int iTemp157 = int((fSlow8 * float(iTemp156)));
			iRec126[0] = ((iRec126[1] * (1 - iTemp157)) + (iTemp155 * iTemp157));
			int iTemp158 = int((fSlow7 * ((2.32830644e-10f * float(iRec64)) + 1.0f)));
			iRec128[0] = ((iVec0[1] * iRec128[1]) + (iTemp0 * iTemp158));
			int iTemp159 = ((iRec0[0] + iRec128[0]) % iSlow3);
			int iTemp160 = int((fSlow8 * float(iTemp159)));
			iRec129[0] = ((iRec129[1] * (1 - iTemp160)) + (iTemp158 * iTemp160));
			output0[i] = FAUSTFLOAT((fSlow1 * ((((((((((((((((((((((float(iSlow2) * (sinf((fSlow5 * float(iTemp66))) * ftbl0[((iTemp66 + iRec67[0]) % iSlow6)])) + (((((((((((float(iSlow9) * (ftbl0[((iTemp69 + iRec69[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp69))))) + (float(iSlow10) * (ftbl0[((iTemp72 + iRec71[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp72)))))) + (float(iSlow11) * (ftbl0[((iTemp75 + iRec73[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp75)))))) + (float(iSlow12) * (ftbl0[((iTemp78 + iRec75[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp78)))))) + (float(iSlow13) * (ftbl0[((iTemp81 + iRec77[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp81)))))) + (float(iSlow14) * (ftbl0[((iTemp84 + iRec79[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp84)))))) + (float(iSlow15) * (ftbl0[((iTemp87 + iRec81[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp87)))))) + (float(iSlow16) * (ftbl0[((iTemp90 + iRec83[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp90)))))) + (float(iSlow17) * (ftbl0[((iTemp93 + iRec85[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp93)))))) + (float(iSlow18) * (ftbl0[((iTemp96 + iRec87[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp96)))))) + (float(iSlow19) * (ftbl0[((iTemp99 + iRec89[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp99))))))) + (float(iSlow20) * (ftbl0[((iTemp102 + iRec91[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp102)))))) + (float(iSlow21) * (ftbl0[((iTemp105 + iRec93[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp105)))))) + (float(iSlow22) * (ftbl0[((iTemp108 + iRec95[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp108)))))) + (float(iSlow23) * (ftbl0[((iTemp111 + iRec97[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp111)))))) + (float(iSlow24) * (ftbl0[((iTemp114 + iRec99[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp114)))))) + (float(iSlow25) * (ftbl0[((iTemp117 + iRec101[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp117)))))) + (float(iSlow26) * (ftbl0[((iTemp120 + iRec103[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp120)))))) + (float(iSlow27) * (ftbl0[((iTemp123 + iRec105[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp123)))))) + (float(iSlow28) * (ftbl0[((iTemp126 + iRec107[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp126)))))) + (float(iSlow29) * (ftbl0[((iTemp129 + iRec109[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp129)))))) + (float(iSlow30) * (ftbl0[((iTemp132 + iRec111[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp132)))))) + (float(iSlow31) * (ftbl0[((iTemp135 + iRec113[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp135)))))) + (float(iSlow32) * (ftbl0[((iTemp138 + iRec115[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp138)))))) + (float(iSlow33) * (ftbl0[((iTemp141 + iRec117[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp141)))))) + (float(iSlow34) * (ftbl0[((iTemp144 + iRec119[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp144)))))) + (float(iSlow35) * (ftbl0[((iTemp147 + iRec121[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp147)))))) + (float(iSlow36) * (ftbl0[((iTemp150 + iRec123[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp150)))))) + (float(iSlow37) * (ftbl0[((iRec124[0] + iTemp153) % iSlow6)] * sinf((fSlow5 * float(iTemp153)))))) + (float(iSlow38) * (ftbl0[((iRec126[0] + iTemp156) % iSlow6)] * sinf((fSlow5 * float(iTemp156)))))) + (float(iSlow39) * (ftbl0[((iTemp159 + iRec129[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp159))))))));
			int iTemp161 = int((fSlow7 * ((2.32830644e-10f * float(iRec63)) + 1.0f)));
			iRec131[0] = ((iVec0[1] * iRec131[1]) + (iTemp0 * iTemp161));
			int iTemp162 = ((iRec131[0] + iRec0[0]) % iSlow3);
			int iTemp163 = int((fSlow8 * float(iTemp162)));
			iRec130[0] = ((iRec130[1] * (1 - iTemp163)) + (iTemp161 * iTemp163));
			int iTemp164 = int((fSlow7 * ((2.32830644e-10f * float(iRec3)) + 1.0f)));
			iRec132[0] = ((iVec0[1] * iRec132[1]) + (iTemp0 * iTemp164));
			int iTemp165 = ((iRec0[0] + iRec132[0]) % iSlow3);
			int iTemp166 = int((fSlow8 * float(iTemp165)));
			iRec133[0] = ((iTemp164 * iTemp166) + (iRec133[1] * (1 - iTemp166)));
			int iTemp167 = int((fSlow7 * ((2.32830644e-10f * float(iRec5)) + 1.0f)));
			iRec134[0] = ((iVec0[1] * iRec134[1]) + (iTemp0 * iTemp167));
			int iTemp168 = ((iRec0[0] + iRec134[0]) % iSlow3);
			int iTemp169 = int((fSlow8 * float(iTemp168)));
			iRec135[0] = ((iRec135[1] * (1 - iTemp169)) + (iTemp167 * iTemp169));
			int iTemp170 = int((fSlow7 * ((2.32830644e-10f * float(iRec7)) + 1.0f)));
			iRec136[0] = ((iVec0[1] * iRec136[1]) + (iTemp0 * iTemp170));
			int iTemp171 = ((iRec0[0] + iRec136[0]) % iSlow3);
			int iTemp172 = int((fSlow8 * float(iTemp171)));
			iRec137[0] = ((iRec137[1] * (1 - iTemp172)) + (iTemp170 * iTemp172));
			int iTemp173 = int((fSlow7 * ((2.32830644e-10f * float(iRec9)) + 1.0f)));
			iRec138[0] = ((iVec0[1] * iRec138[1]) + (iTemp0 * iTemp173));
			int iTemp174 = ((iRec0[0] + iRec138[0]) % iSlow3);
			int iTemp175 = int((fSlow8 * float(iTemp174)));
			iRec139[0] = ((iRec139[1] * (1 - iTemp175)) + (iTemp173 * iTemp175));
			int iTemp176 = int((fSlow7 * ((2.32830644e-10f * float(iRec11)) + 1.0f)));
			iRec140[0] = ((iVec0[1] * iRec140[1]) + (iTemp0 * iTemp176));
			int iTemp177 = ((iRec0[0] + iRec140[0]) % iSlow3);
			int iTemp178 = int((fSlow8 * float(iTemp177)));
			iRec141[0] = ((iRec141[1] * (1 - iTemp178)) + (iTemp176 * iTemp178));
			int iTemp179 = int((fSlow7 * ((2.32830644e-10f * float(iRec13)) + 1.0f)));
			iRec142[0] = ((iVec0[1] * iRec142[1]) + (iTemp0 * iTemp179));
			int iTemp180 = ((iRec0[0] + iRec142[0]) % iSlow3);
			int iTemp181 = int((fSlow8 * float(iTemp180)));
			iRec143[0] = ((iRec143[1] * (1 - iTemp181)) + (iTemp179 * iTemp181));
			int iTemp182 = int((fSlow7 * ((2.32830644e-10f * float(iRec15)) + 1.0f)));
			iRec144[0] = ((iVec0[1] * iRec144[1]) + (iTemp0 * iTemp182));
			int iTemp183 = ((iRec0[0] + iRec144[0]) % iSlow3);
			int iTemp184 = int((fSlow8 * float(iTemp183)));
			iRec145[0] = ((iRec145[1] * (1 - iTemp184)) + (iTemp182 * iTemp184));
			int iTemp185 = int((fSlow7 * ((2.32830644e-10f * float(iRec17)) + 1.0f)));
			iRec146[0] = ((iVec0[1] * iRec146[1]) + (iTemp0 * iTemp185));
			int iTemp186 = ((iRec0[0] + iRec146[0]) % iSlow3);
			int iTemp187 = int((fSlow8 * float(iTemp186)));
			iRec147[0] = ((iRec147[1] * (1 - iTemp187)) + (iTemp185 * iTemp187));
			int iTemp188 = int((fSlow7 * ((2.32830644e-10f * float(iRec19)) + 1.0f)));
			iRec148[0] = ((iVec0[1] * iRec148[1]) + (iTemp0 * iTemp188));
			int iTemp189 = ((iRec0[0] + iRec148[0]) % iSlow3);
			int iTemp190 = int((fSlow8 * float(iTemp189)));
			iRec149[0] = ((iRec149[1] * (1 - iTemp190)) + (iTemp188 * iTemp190));
			int iTemp191 = int((fSlow7 * ((2.32830644e-10f * float(iRec21)) + 1.0f)));
			iRec150[0] = ((iVec0[1] * iRec150[1]) + (iTemp0 * iTemp191));
			int iTemp192 = ((iRec0[0] + iRec150[0]) % iSlow3);
			int iTemp193 = int((fSlow8 * float(iTemp192)));
			iRec151[0] = ((iRec151[1] * (1 - iTemp193)) + (iTemp191 * iTemp193));
			int iTemp194 = int((fSlow7 * ((2.32830644e-10f * float(iRec23)) + 1.0f)));
			iRec152[0] = ((iVec0[1] * iRec152[1]) + (iTemp0 * iTemp194));
			int iTemp195 = ((iRec0[0] + iRec152[0]) % iSlow3);
			int iTemp196 = int((fSlow8 * float(iTemp195)));
			iRec153[0] = ((iRec153[1] * (1 - iTemp196)) + (iTemp194 * iTemp196));
			int iTemp197 = int((fSlow7 * ((2.32830644e-10f * float(iRec25)) + 1.0f)));
			iRec154[0] = ((iVec0[1] * iRec154[1]) + (iTemp0 * iTemp197));
			int iTemp198 = ((iRec0[0] + iRec154[0]) % iSlow3);
			int iTemp199 = int((fSlow8 * float(iTemp198)));
			iRec155[0] = ((iRec155[1] * (1 - iTemp199)) + (iTemp197 * iTemp199));
			int iTemp200 = int((fSlow7 * ((2.32830644e-10f * float(iRec27)) + 1.0f)));
			iRec156[0] = ((iVec0[1] * iRec156[1]) + (iTemp0 * iTemp200));
			int iTemp201 = ((iRec0[0] + iRec156[0]) % iSlow3);
			int iTemp202 = int((fSlow8 * float(iTemp201)));
			iRec157[0] = ((iRec157[1] * (1 - iTemp202)) + (iTemp200 * iTemp202));
			int iTemp203 = int((fSlow7 * ((2.32830644e-10f * float(iRec29)) + 1.0f)));
			iRec158[0] = ((iVec0[1] * iRec158[1]) + (iTemp0 * iTemp203));
			int iTemp204 = ((iRec0[0] + iRec158[0]) % iSlow3);
			int iTemp205 = int((fSlow8 * float(iTemp204)));
			iRec159[0] = ((iRec159[1] * (1 - iTemp205)) + (iTemp203 * iTemp205));
			int iTemp206 = int((fSlow7 * ((2.32830644e-10f * float(iRec31)) + 1.0f)));
			iRec160[0] = ((iVec0[1] * iRec160[1]) + (iTemp0 * iTemp206));
			int iTemp207 = ((iRec0[0] + iRec160[0]) % iSlow3);
			int iTemp208 = int((fSlow8 * float(iTemp207)));
			iRec161[0] = ((iRec161[1] * (1 - iTemp208)) + (iTemp206 * iTemp208));
			int iTemp209 = int((fSlow7 * ((2.32830644e-10f * float(iRec33)) + 1.0f)));
			iRec162[0] = ((iVec0[1] * iRec162[1]) + (iTemp0 * iTemp209));
			int iTemp210 = ((iRec0[0] + iRec162[0]) % iSlow3);
			int iTemp211 = int((fSlow8 * float(iTemp210)));
			iRec163[0] = ((iRec163[1] * (1 - iTemp211)) + (iTemp209 * iTemp211));
			int iTemp212 = int((fSlow7 * ((2.32830644e-10f * float(iRec35)) + 1.0f)));
			iRec164[0] = ((iVec0[1] * iRec164[1]) + (iTemp0 * iTemp212));
			int iTemp213 = ((iRec0[0] + iRec164[0]) % iSlow3);
			int iTemp214 = int((fSlow8 * float(iTemp213)));
			iRec165[0] = ((iRec165[1] * (1 - iTemp214)) + (iTemp212 * iTemp214));
			int iTemp215 = int((fSlow7 * ((2.32830644e-10f * float(iRec37)) + 1.0f)));
			iRec166[0] = ((iVec0[1] * iRec166[1]) + (iTemp0 * iTemp215));
			int iTemp216 = ((iRec0[0] + iRec166[0]) % iSlow3);
			int iTemp217 = int((fSlow8 * float(iTemp216)));
			iRec167[0] = ((iRec167[1] * (1 - iTemp217)) + (iTemp215 * iTemp217));
			int iTemp218 = int((fSlow7 * ((2.32830644e-10f * float(iRec39)) + 1.0f)));
			iRec169[0] = ((iVec0[1] * iRec169[1]) + (iTemp0 * iTemp218));
			int iTemp219 = ((iRec0[0] + iRec169[0]) % iSlow3);
			int iTemp220 = int((fSlow8 * float(iTemp219)));
			iRec168[0] = ((iRec168[1] * (1 - iTemp220)) + (iTemp218 * iTemp220));
			int iTemp221 = int((fSlow7 * ((2.32830644e-10f * float(iRec41)) + 1.0f)));
			iRec171[0] = ((iVec0[1] * iRec171[1]) + (iTemp0 * iTemp221));
			int iTemp222 = ((iRec171[0] + iRec0[0]) % iSlow3);
			int iTemp223 = int((fSlow8 * float(iTemp222)));
			iRec170[0] = ((iRec170[1] * (1 - iTemp223)) + (iTemp221 * iTemp223));
			int iTemp224 = int((fSlow7 * ((2.32830644e-10f * float(iRec43)) + 1.0f)));
			iRec173[0] = ((iVec0[1] * iRec173[1]) + (iTemp0 * iTemp224));
			int iTemp225 = ((iRec173[0] + iRec0[0]) % iSlow3);
			int iTemp226 = int((fSlow8 * float(iTemp225)));
			iRec172[0] = ((iRec172[1] * (1 - iTemp226)) + (iTemp224 * iTemp226));
			int iTemp227 = int((fSlow7 * ((2.32830644e-10f * float(iRec45)) + 1.0f)));
			iRec175[0] = ((iVec0[1] * iRec175[1]) + (iTemp0 * iTemp227));
			int iTemp228 = ((iRec175[0] + iRec0[0]) % iSlow3);
			int iTemp229 = int((fSlow8 * float(iTemp228)));
			iRec174[0] = ((iRec174[1] * (1 - iTemp229)) + (iTemp227 * iTemp229));
			int iTemp230 = int((fSlow7 * ((2.32830644e-10f * float(iRec47)) + 1.0f)));
			iRec177[0] = ((iVec0[1] * iRec177[1]) + (iTemp0 * iTemp230));
			int iTemp231 = ((iRec177[0] + iRec0[0]) % iSlow3);
			int iTemp232 = int((fSlow8 * float(iTemp231)));
			iRec176[0] = ((iRec176[1] * (1 - iTemp232)) + (iTemp230 * iTemp232));
			int iTemp233 = int((fSlow7 * ((2.32830644e-10f * float(iRec49)) + 1.0f)));
			iRec179[0] = ((iVec0[1] * iRec179[1]) + (iTemp0 * iTemp233));
			int iTemp234 = ((iRec179[0] + iRec0[0]) % iSlow3);
			int iTemp235 = int((fSlow8 * float(iTemp234)));
			iRec178[0] = ((iRec178[1] * (1 - iTemp235)) + (iTemp233 * iTemp235));
			int iTemp236 = int((fSlow7 * ((2.32830644e-10f * float(iRec51)) + 1.0f)));
			iRec181[0] = ((iVec0[1] * iRec181[1]) + (iTemp0 * iTemp236));
			int iTemp237 = ((iRec181[0] + iRec0[0]) % iSlow3);
			int iTemp238 = int((fSlow8 * float(iTemp237)));
			iRec180[0] = ((iRec180[1] * (1 - iTemp238)) + (iTemp236 * iTemp238));
			int iTemp239 = int((fSlow7 * ((2.32830644e-10f * float(iRec53)) + 1.0f)));
			iRec183[0] = ((iVec0[1] * iRec183[1]) + (iTemp0 * iTemp239));
			int iTemp240 = ((iRec183[0] + iRec0[0]) % iSlow3);
			int iTemp241 = int((fSlow8 * float(iTemp240)));
			iRec182[0] = ((iRec182[1] * (1 - iTemp241)) + (iTemp239 * iTemp241));
			int iTemp242 = int((fSlow7 * ((2.32830644e-10f * float(iRec55)) + 1.0f)));
			iRec185[0] = ((iVec0[1] * iRec185[1]) + (iTemp0 * iTemp242));
			int iTemp243 = ((iRec0[0] + iRec185[0]) % iSlow3);
			int iTemp244 = int((fSlow8 * float(iTemp243)));
			iRec184[0] = ((iRec184[1] * (1 - iTemp244)) + (iTemp242 * iTemp244));
			int iTemp245 = int((fSlow7 * ((2.32830644e-10f * float(iRec57)) + 1.0f)));
			iRec187[0] = ((iVec0[1] * iRec187[1]) + (iTemp0 * iTemp245));
			int iTemp246 = ((iRec187[0] + iRec0[0]) % iSlow3);
			int iTemp247 = int((fSlow8 * float(iTemp246)));
			iRec186[0] = ((iRec186[1] * (1 - iTemp247)) + (iTemp245 * iTemp247));
			int iTemp248 = int((fSlow7 * ((2.32830644e-10f * float(iRec59)) + 1.0f)));
			iRec189[0] = ((iVec0[1] * iRec189[1]) + (iTemp0 * iTemp248));
			int iTemp249 = ((iRec189[0] + iRec0[0]) % iSlow3);
			int iTemp250 = int((fSlow8 * float(iTemp249)));
			iRec188[0] = ((iRec188[1] * (1 - iTemp250)) + (iTemp248 * iTemp250));
			int iTemp251 = int((fSlow7 * ((2.32830644e-10f * float(iRec61)) + 1.0f)));
			iRec191[0] = ((iVec0[1] * iRec191[1]) + (iTemp0 * iTemp251));
			int iTemp252 = ((iRec191[0] + iRec0[0]) % iSlow3);
			int iTemp253 = int((fSlow8 * float(iTemp252)));
			iRec190[0] = ((iRec190[1] * (1 - iTemp253)) + (iTemp251 * iTemp253));
			int iTemp254 = int((fSlow7 * ((2.32830644e-10f * float(iRec65)) + 1.0f)));
			iRec192[0] = ((iTemp0 * iTemp254) + (iVec0[1] * iRec192[1]));
			int iTemp255 = ((iRec192[0] + iRec0[0]) % iSlow3);
			int iTemp256 = int((fSlow8 * float(iTemp255)));
			iRec193[0] = (((1 - iTemp256) * iRec193[1]) + (iTemp256 * iTemp254));
			output1[i] = FAUSTFLOAT((fSlow1 * (((float(iSlow40) * (ftbl0[((iRec130[0] + iTemp162) % iSlow6)] * sinf((fSlow5 * float(iTemp162))))) + ((((((((((((((((((((((((((((((float(iSlow41) * (ftbl0[((iTemp165 + iRec133[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp165))))) + (float(iSlow42) * (ftbl0[((iTemp168 + iRec135[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp168)))))) + (float(iSlow43) * (ftbl0[((iTemp171 + iRec137[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp171)))))) + (float(iSlow44) * (ftbl0[((iTemp174 + iRec139[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp174)))))) + (float(iSlow45) * (ftbl0[((iTemp177 + iRec141[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp177)))))) + (float(iSlow46) * (ftbl0[((iTemp180 + iRec143[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp180)))))) + (float(iSlow47) * (ftbl0[((iTemp183 + iRec145[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp183)))))) + (float(iSlow48) * (ftbl0[((iTemp186 + iRec147[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp186)))))) + (float(iSlow49) * (ftbl0[((iTemp189 + iRec149[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp189)))))) + (float(iSlow50) * (ftbl0[((iTemp192 + iRec151[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp192)))))) + (float(iSlow51) * (ftbl0[((iTemp195 + iRec153[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp195)))))) + (float(iSlow52) * (ftbl0[((iTemp198 + iRec155[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp198)))))) + (float(iSlow53) * (ftbl0[((iTemp201 + iRec157[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp201)))))) + (float(iSlow54) * (ftbl0[((iTemp204 + iRec159[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp204)))))) + (float(iSlow55) * (ftbl0[((iTemp207 + iRec161[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp207)))))) + (float(iSlow56) * (ftbl0[((iTemp210 + iRec163[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp210)))))) + (float(iSlow57) * (ftbl0[((iTemp213 + iRec165[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp213)))))) + (float(iSlow58) * (ftbl0[((iTemp216 + iRec167[0]) % iSlow6)] * sinf((fSlow5 * float(iTemp216)))))) + (float(iSlow59) * (ftbl0[((iRec168[0] + iTemp219) % iSlow6)] * sinf((fSlow5 * float(iTemp219)))))) + (float(iSlow60) * (ftbl0[((iRec170[0] + iTemp222) % iSlow6)] * sinf((fSlow5 * float(iTemp222)))))) + (float(iSlow61) * (ftbl0[((iRec172[0] + iTemp225) % iSlow6)] * sinf((fSlow5 * float(iTemp225)))))) + (float(iSlow62) * (ftbl0[((iRec174[0] + iTemp228) % iSlow6)] * sinf((fSlow5 * float(iTemp228)))))) + (float(iSlow63) * (ftbl0[((iRec176[0] + iTemp231) % iSlow6)] * sinf((fSlow5 * float(iTemp231)))))) + (float(iSlow64) * (ftbl0[((iRec178[0] + iTemp234) % iSlow6)] * sinf((fSlow5 * float(iTemp234)))))) + (float(iSlow65) * (ftbl0[((iRec180[0] + iTemp237) % iSlow6)] * sinf((fSlow5 * float(iTemp237)))))) + (float(iSlow66) * (ftbl0[((iRec182[0] + iTemp240) % iSlow6)] * sinf((fSlow5 * float(iTemp240)))))) + (float(iSlow67) * (ftbl0[((iRec184[0] + iTemp243) % iSlow6)] * sinf((fSlow5 * float(iTemp243)))))) + (float(iSlow68) * (ftbl0[((iRec186[0] + iTemp246) % iSlow6)] * sinf((fSlow5 * float(iTemp246)))))) + (float(iSlow69) * (ftbl0[((iRec188[0] + iTemp249) % iSlow6)] * sinf((fSlow5 * float(iTemp249)))))) + (float(iSlow70) * (ftbl0[((iRec190[0] + iTemp252) % iSlow6)] * sinf((fSlow5 * float(iTemp252))))))) + (float(iSlow71) * (sinf((fSlow5 * float(iTemp255))) * ftbl0[((iRec193[0] + iTemp255) % iSlow6)])))));
			iVec0[1] = iVec0[0];
			iRec0[1] = iRec0[0];
			iRec2[1] = iRec2[0];
			iRec1[1] = iRec1[0];
			iRec66[1] = iRec66[0];
			iRec67[1] = iRec67[0];
			iRec68[1] = iRec68[0];
			iRec69[1] = iRec69[0];
			iRec70[1] = iRec70[0];
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
			iRec125[1] = iRec125[0];
			iRec124[1] = iRec124[0];
			iRec127[1] = iRec127[0];
			iRec126[1] = iRec126[0];
			iRec128[1] = iRec128[0];
			iRec129[1] = iRec129[0];
			iRec131[1] = iRec131[0];
			iRec130[1] = iRec130[0];
			iRec132[1] = iRec132[0];
			iRec133[1] = iRec133[0];
			iRec134[1] = iRec134[0];
			iRec135[1] = iRec135[0];
			iRec136[1] = iRec136[0];
			iRec137[1] = iRec137[0];
			iRec138[1] = iRec138[0];
			iRec139[1] = iRec139[0];
			iRec140[1] = iRec140[0];
			iRec141[1] = iRec141[0];
			iRec142[1] = iRec142[0];
			iRec143[1] = iRec143[0];
			iRec144[1] = iRec144[0];
			iRec145[1] = iRec145[0];
			iRec146[1] = iRec146[0];
			iRec147[1] = iRec147[0];
			iRec148[1] = iRec148[0];
			iRec149[1] = iRec149[0];
			iRec150[1] = iRec150[0];
			iRec151[1] = iRec151[0];
			iRec152[1] = iRec152[0];
			iRec153[1] = iRec153[0];
			iRec154[1] = iRec154[0];
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
			iRec169[1] = iRec169[0];
			iRec168[1] = iRec168[0];
			iRec171[1] = iRec171[0];
			iRec170[1] = iRec170[0];
			iRec173[1] = iRec173[0];
			iRec172[1] = iRec172[0];
			iRec175[1] = iRec175[0];
			iRec174[1] = iRec174[0];
			iRec177[1] = iRec177[0];
			iRec176[1] = iRec176[0];
			iRec179[1] = iRec179[0];
			iRec178[1] = iRec178[0];
			iRec181[1] = iRec181[0];
			iRec180[1] = iRec180[0];
			iRec183[1] = iRec183[0];
			iRec182[1] = iRec182[0];
			iRec185[1] = iRec185[0];
			iRec184[1] = iRec184[0];
			iRec187[1] = iRec187[0];
			iRec186[1] = iRec186[0];
			iRec189[1] = iRec189[0];
			iRec188[1] = iRec188[0];
			iRec191[1] = iRec191[0];
			iRec190[1] = iRec190[0];
			iRec192[1] = iRec192[0];
			iRec193[1] = iRec193[0];
			
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
static t_class* granulator_faust_class;
#define FAUST_EXT t_faust_granulator
#define FAUST_EXT_CLASS granulator_faust_class
// clang-format on

struct t_faust_granulator {
    t_object x_obj;
#ifdef __MINGW32__
    /* This seems to be necessary as some as yet undetermined Pd routine seems
     to write past the end of x_obj on Windows. */
    int fence; /* dummy field (not used) */
#endif
    granulator* dsp;
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
    t_faust_granulator* x = reinterpret_cast<t_faust_granulator*>(w[1]);
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

static void granulator_faust_dsp(t_faust_granulator* x, t_signal** sp)
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

static void granulator_dump_to_console(t_faust_granulator* x)
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

static void granulator_faust_any(t_faust_granulator* x, t_symbol* s, int argc, t_atom* argv)
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

static void faust_free_dsp(t_faust_granulator* x)
{
    delete x->dsp;
    x->dsp = NULL;
}

static void faust_free_ui(t_faust_granulator* x)
{
    delete x->ui;
    x->ui = NULL;
}

static void faust_free_inputs(t_faust_granulator* x)
{
    if (x->inputs)
        free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust_granulator* x)
{
    if (x->outputs)
        free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust_granulator* x)
{
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i])
                free(x->buf[i]);
        }

        free(x->buf);
    }
}

static void granulator_faust_free(t_faust_granulator* x)
{
    faust_free_dsp(x);
    faust_free_ui(x);
    faust_free_inputs(x);
    faust_free_outputs(x);
    faust_free_buf(x);
}

static bool faust_init_inputs(t_faust_granulator* x)
{
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if (x->inputs == NULL) {
            error("[%s] faust_init_inputs failed", sym(granulator));
            return false;
        }
    }

    // creating sound inlets (except the first one!)
    for (int i = 0; i < (x->n_in - 1); i++) {
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    return true;
}

static bool faust_init_outputs(t_faust_granulator* x, bool info_outlet)
{
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->outputs == NULL) {
            error("[%s] faust_init_outputs failed", sym(granulator));
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->buf == NULL) {
            error("[%s] faust_init_outputs failed", sym(granulator));
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

static bool faust_new_internal(t_faust_granulator* x, const std::string& objId = "", bool info_outlet = true)
{
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new granulator();
    x->ui = new PdUI<UI>(sym(granulator), objId);

    if (!faust_init_inputs(x)) {
        granulator_faust_free(x);
        return false;
    }

    if (!faust_init_outputs(x, info_outlet)) {
        granulator_faust_free(x);
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
    t_faust_granulator* x_;
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
    PdArgParser(t_faust_granulator* x, int argc, t_atom* argv, bool info_outlet = true)
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

    t_faust_granulator* pd_obj()
    {
        return this->x_;
    }
};

static void* granulator_faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s, bool soundIn = true)
{
    granulator_faust_class = class_new(s, reinterpret_cast<t_newmethod>(granulator_faust_new),
        reinterpret_cast<t_method>(granulator_faust_free),
        sizeof(t_faust_granulator),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);

    if (soundIn) {
        class_addmethod(granulator_faust_class, nullfn, &s_signal, A_NULL);
        CLASS_MAINSIGNALIN(granulator_faust_class, t_faust_granulator, f);
    }

    class_addmethod(granulator_faust_class, reinterpret_cast<t_method>(granulator_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(granulator_faust_class, reinterpret_cast<t_method>(granulator_dump_to_console), gensym("dump"), A_NULL);
    class_addanything(granulator_faust_class, granulator_faust_any);
}

#define EXTERNAL_NEW void* granulator_faust_new(t_symbol*, int argc, t_atom* argv)

#define EXTERNAL_SIMPLE_NEW()                                                           \
    static void* granulator_faust_new(t_symbol*, int argc, t_atom* argv)                     \
    {                                                                                   \
        t_faust_granulator* x = reinterpret_cast<t_faust_granulator*>(pd_new(granulator_faust_class)); \
        PdArgParser p(x, argc, argv, false);                                            \
        return p.pd_obj();                                                              \
    }

#define EXTERNAL_SETUP(MOD)                        \
    extern "C" void setup_##MOD##0x2egranulator_tilde() \
    {                                              \
        internal_setup(gensym(#MOD ".granulator~"));    \
    }

#define EXTERNAL_SETUP_NO_IN(MOD)                      \
    extern "C" void setup_##MOD##0x2egranulator_tilde()     \
    {                                                  \
        internal_setup(gensym(#MOD ".granulator~"), false); \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);

#endif
