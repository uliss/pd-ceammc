//----------------------------------------------------------
// name: "synth.dx7"
//
// Code generated with Faust 2.15.0 (https://faust.grame.fr)
//----------------------------------------------------------

/* link with  */
#include <math.h>
#ifndef FAUSTPOWER
#define FAUSTPOWER
#include <cmath>
template <int N> inline int faustpower(int x)              { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline int faustpower<0>(int x)            { return 1; }
template <> 	 inline int faustpower<1>(int x)            { return x; }
template <> 	 inline int faustpower<2>(int x)            { return x*x; }
template <int N> inline float faustpower(float x)            { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline float faustpower<0>(float x)          { return 1; }
template <> 	 inline float faustpower<1>(float x)          { return x; }
template <> 	 inline float faustpower<2>(float x)          { return x*x; }
#endif
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
#include <cstdlib>
#include <string.h>


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
	int	i;
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
	return def;
}

static bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

static const char* lopts(char* argv[], const char* name, const char* def)
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
struct synth_dx7 : public dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 


#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_dx7
#endif

class synth_dx7 : public dsp {
  private:
	class SIG0 {
	  private:
		int fSamplingFreq;
		int 	iRec0[2];
		float 	fTempPerm0;
	  public:
		int getNumInputs() { return 0; }
		int getNumOutputs() { return 1; }
		void init(int samplingFreq) {
			fSamplingFreq = samplingFreq;
			fTempPerm0 = 0;
			for (int i=0; i<2; i++) iRec0[i] = 0;
		}
		void fill (int count, float output[]) {
			for (int i=0; i<count; i++) {
				iRec0[0] = (iRec0[1] + 1);
				fTempPerm0 = sinf((9.5873799242852573e-05f * float((iRec0[0] + -1))));
				output[i] = fTempPerm0;
				// post processing
				iRec0[1] = iRec0[0];
			}
		}
	};


	FAUSTFLOAT 	fentry0;
	static float 	ftbl0[65536];
	FAUSTFLOAT 	fbutton0;
	float 	fConst0;
	FAUSTFLOAT 	fentry1;
	FAUSTFLOAT 	fentry2;
	FAUSTFLOAT 	fentry3;
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT 	fentry4;
	FAUSTFLOAT 	fentry5;
	FAUSTFLOAT 	fentry6;
	FAUSTFLOAT 	fentry7;
	FAUSTFLOAT 	fentry8;
	FAUSTFLOAT 	fentry9;
	FAUSTFLOAT 	fslider1;
	float 	fRec2[2];
	FAUSTFLOAT 	fentry10;
	FAUSTFLOAT 	fentry11;
	FAUSTFLOAT 	fentry12;
	FAUSTFLOAT 	fentry13;
	FAUSTFLOAT 	fentry14;
	float 	fRec4[2];
	int 	iTempPerm1;
	int 	iTempPerm2;
	float 	fConst1;
	float 	fRec3[2];
	float 	fTempPerm3;
	float 	fTempPerm4;
	float 	fRec5[2];
	float 	fTempPerm5;
	FAUSTFLOAT 	fentry15;
	float 	fRec1[2];
	FAUSTFLOAT 	fentry16;
	FAUSTFLOAT 	fentry17;
	FAUSTFLOAT 	fentry18;
	float 	fTempPerm6;
	float 	fRec6[2];
	FAUSTFLOAT 	fentry19;
	FAUSTFLOAT 	fentry20;
	FAUSTFLOAT 	fentry21;
	FAUSTFLOAT 	fentry22;
	FAUSTFLOAT 	fentry23;
	FAUSTFLOAT 	fentry24;
	float 	fRec7[2];
	FAUSTFLOAT 	fentry25;
	FAUSTFLOAT 	fentry26;
	FAUSTFLOAT 	fentry27;
	FAUSTFLOAT 	fentry28;
	FAUSTFLOAT 	fentry29;
	float 	fRec9[2];
	int 	iTempPerm7;
	int 	iTempPerm8;
	float 	fRec8[2];
	float 	fTempPerm9;
	FAUSTFLOAT 	fentry30;
	FAUSTFLOAT 	fentry31;
	FAUSTFLOAT 	fentry32;
	FAUSTFLOAT 	fentry33;
	FAUSTFLOAT 	fentry34;
	FAUSTFLOAT 	fentry35;
	FAUSTFLOAT 	fentry36;
	FAUSTFLOAT 	fentry37;
	FAUSTFLOAT 	fentry38;
	float 	fRec10[2];
	FAUSTFLOAT 	fentry39;
	FAUSTFLOAT 	fentry40;
	FAUSTFLOAT 	fentry41;
	FAUSTFLOAT 	fentry42;
	FAUSTFLOAT 	fentry43;
	float 	fRec12[2];
	int 	iTempPerm10;
	int 	iTempPerm11;
	float 	fRec11[2];
	float 	fTempPerm12;
	FAUSTFLOAT 	fentry44;
	FAUSTFLOAT 	fentry45;
	FAUSTFLOAT 	fentry46;
	FAUSTFLOAT 	fentry47;
	FAUSTFLOAT 	fentry48;
	FAUSTFLOAT 	fentry49;
	FAUSTFLOAT 	fentry50;
	FAUSTFLOAT 	fentry51;
	FAUSTFLOAT 	fentry52;
	float 	fRec13[2];
	FAUSTFLOAT 	fentry53;
	FAUSTFLOAT 	fentry54;
	FAUSTFLOAT 	fentry55;
	FAUSTFLOAT 	fentry56;
	FAUSTFLOAT 	fentry57;
	float 	fRec15[2];
	int 	iTempPerm13;
	int 	iTempPerm14;
	float 	fRec14[2];
	float 	fTempPerm15;
	FAUSTFLOAT 	fentry58;
	FAUSTFLOAT 	fentry59;
	FAUSTFLOAT 	fentry60;
	float 	fTempPerm16;
	float 	fRec16[2];
	float 	fTempPerm17;
	FAUSTFLOAT 	fentry61;
	FAUSTFLOAT 	fentry62;
	FAUSTFLOAT 	fentry63;
	FAUSTFLOAT 	fentry64;
	FAUSTFLOAT 	fentry65;
	FAUSTFLOAT 	fentry66;
	float 	fRec17[2];
	FAUSTFLOAT 	fentry67;
	FAUSTFLOAT 	fentry68;
	FAUSTFLOAT 	fentry69;
	FAUSTFLOAT 	fentry70;
	FAUSTFLOAT 	fentry71;
	float 	fRec19[2];
	int 	iTempPerm18;
	int 	iTempPerm19;
	float 	fRec18[2];
	float 	fTempPerm20;
	FAUSTFLOAT 	fentry72;
	FAUSTFLOAT 	fentry73;
	FAUSTFLOAT 	fentry74;
	float 	fTempPerm21;
	float 	fRec20[2];
	FAUSTFLOAT 	fentry75;
	FAUSTFLOAT 	fentry76;
	FAUSTFLOAT 	fentry77;
	FAUSTFLOAT 	fentry78;
	FAUSTFLOAT 	fentry79;
	FAUSTFLOAT 	fentry80;
	float 	fRec21[2];
	FAUSTFLOAT 	fentry81;
	FAUSTFLOAT 	fentry82;
	FAUSTFLOAT 	fentry83;
	FAUSTFLOAT 	fentry84;
	FAUSTFLOAT 	fentry85;
	float 	fRec23[2];
	int 	iTempPerm22;
	int 	iTempPerm23;
	float 	fRec22[2];
	float 	fTempPerm24;
	float 	fTempPerm25;
	float 	fRec24[2];
	float 	fTempPerm26;
	float 	fRec25[2];
	float 	fTempPerm27;
	float 	fRec26[2];
	float 	fTempPerm28;
	int fSamplingFreq;

  public:
	virtual void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.0");
		m->declare("filename", "synth_dx7");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "synth.dx7");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.0");
	}

	virtual int getNumInputs() { return 0; }
	virtual int getNumOutputs() { return 1; }
	static void classInit(int samplingFreq) {
		SIG0 sig0;
		sig0.init(samplingFreq);
		sig0.fill(65536,ftbl0);
	}
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = min(192000.0f, max(1.0f, (float)fSamplingFreq));
		iTempPerm1 = 0;
		iTempPerm2 = 0;
		fConst1 = (1.0f / fConst0);
		fTempPerm3 = 0;
		fTempPerm4 = 0;
		fTempPerm5 = 0;
		fTempPerm6 = 0;
		iTempPerm7 = 0;
		iTempPerm8 = 0;
		fTempPerm9 = 0;
		iTempPerm10 = 0;
		iTempPerm11 = 0;
		fTempPerm12 = 0;
		iTempPerm13 = 0;
		iTempPerm14 = 0;
		fTempPerm15 = 0;
		fTempPerm16 = 0;
		fTempPerm17 = 0;
		iTempPerm18 = 0;
		iTempPerm19 = 0;
		fTempPerm20 = 0;
		fTempPerm21 = 0;
		iTempPerm22 = 0;
		iTempPerm23 = 0;
		fTempPerm24 = 0;
		fTempPerm25 = 0;
		fTempPerm26 = 0;
		fTempPerm27 = 0;
		fTempPerm28 = 0;
	}
	virtual void instanceResetUserInterface() {
		fentry0 = 0.0f;
		fbutton0 = 0.0;
		fentry1 = 95.0f;
		fentry2 = 90.0f;
		fentry3 = 1.0f;
		fslider0 = 0.80000000000000004f;
		fentry4 = 0.0f;
		fentry5 = 90.0f;
		fentry6 = 0.0f;
		fentry7 = 1.0f;
		fentry8 = 0.0f;
		fentry9 = 1.0f;
		fslider1 = 400.0f;
		fentry10 = 0.0f;
		fentry11 = 90.0f;
		fentry12 = 90.0f;
		fentry13 = 90.0f;
		fentry14 = 90.0f;
		fentry15 = 0.0f;
		fentry16 = 1.0f;
		fentry17 = 0.0f;
		fentry18 = 1.0f;
		fentry19 = 95.0f;
		fentry20 = 90.0f;
		fentry21 = 1.0f;
		fentry22 = 0.0f;
		fentry23 = 90.0f;
		fentry24 = 0.0f;
		fentry25 = 90.0f;
		fentry26 = 0.0f;
		fentry27 = 90.0f;
		fentry28 = 90.0f;
		fentry29 = 90.0f;
		fentry30 = 95.0f;
		fentry31 = 90.0f;
		fentry32 = 1.0f;
		fentry33 = 0.0f;
		fentry34 = 90.0f;
		fentry35 = 0.0f;
		fentry36 = 1.0f;
		fentry37 = 0.0f;
		fentry38 = 1.0f;
		fentry39 = 0.0f;
		fentry40 = 90.0f;
		fentry41 = 90.0f;
		fentry42 = 90.0f;
		fentry43 = 90.0f;
		fentry44 = 95.0f;
		fentry45 = 90.0f;
		fentry46 = 1.0f;
		fentry47 = 0.0f;
		fentry48 = 90.0f;
		fentry49 = 0.0f;
		fentry50 = 1.0f;
		fentry51 = 0.0f;
		fentry52 = 1.0f;
		fentry53 = 0.0f;
		fentry54 = 90.0f;
		fentry55 = 90.0f;
		fentry56 = 90.0f;
		fentry57 = 90.0f;
		fentry58 = 1.0f;
		fentry59 = 0.0f;
		fentry60 = 1.0f;
		fentry61 = 95.0f;
		fentry62 = 90.0f;
		fentry63 = 1.0f;
		fentry64 = 0.0f;
		fentry65 = 90.0f;
		fentry66 = 0.0f;
		fentry67 = 0.0f;
		fentry68 = 90.0f;
		fentry69 = 90.0f;
		fentry70 = 90.0f;
		fentry71 = 90.0f;
		fentry72 = 1.0f;
		fentry73 = 0.0f;
		fentry74 = 1.0f;
		fentry75 = 95.0f;
		fentry76 = 90.0f;
		fentry77 = 1.0f;
		fentry78 = 0.0f;
		fentry79 = 90.0f;
		fentry80 = 0.0f;
		fentry81 = 0.0f;
		fentry82 = 90.0f;
		fentry83 = 90.0f;
		fentry84 = 90.0f;
		fentry85 = 90.0f;
	}
	virtual void instanceClear() {
		for (int i=0; i<2; i++) fRec2[i] = 0;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		for (int i=0; i<2; i++) fRec3[i] = 0;
		for (int i=0; i<2; i++) fRec5[i] = 0;
		for (int i=0; i<2; i++) fRec1[i] = 0;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		for (int i=0; i<2; i++) fRec7[i] = 0;
		for (int i=0; i<2; i++) fRec9[i] = 0;
		for (int i=0; i<2; i++) fRec8[i] = 0;
		for (int i=0; i<2; i++) fRec10[i] = 0;
		for (int i=0; i<2; i++) fRec12[i] = 0;
		for (int i=0; i<2; i++) fRec11[i] = 0;
		for (int i=0; i<2; i++) fRec13[i] = 0;
		for (int i=0; i<2; i++) fRec15[i] = 0;
		for (int i=0; i<2; i++) fRec14[i] = 0;
		for (int i=0; i<2; i++) fRec16[i] = 0;
		for (int i=0; i<2; i++) fRec17[i] = 0;
		for (int i=0; i<2; i++) fRec19[i] = 0;
		for (int i=0; i<2; i++) fRec18[i] = 0;
		for (int i=0; i<2; i++) fRec20[i] = 0;
		for (int i=0; i<2; i++) fRec21[i] = 0;
		for (int i=0; i<2; i++) fRec23[i] = 0;
		for (int i=0; i<2; i++) fRec22[i] = 0;
		for (int i=0; i<2; i++) fRec24[i] = 0;
		for (int i=0; i<2; i++) fRec25[i] = 0;
		for (int i=0; i<2; i++) fRec26[i] = 0;
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
	virtual synth_dx7* clone() {
		return new synth_dx7();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
	}
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.dx7");
		ui_interface->addNumEntry("algorithm", &fentry0, 0.0f, 0.0f, 31.0f, 1.0f);
		ui_interface->addNumEntry("feedback", &fentry15, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addHorizontalSlider("freq", &fslider1, 400.0f, 50.0f, 1000.0f, 0.01f);
		ui_interface->addHorizontalSlider("gain", &fslider0, 0.80000000000000004f, 0.0f, 1.0f, 0.01f);
		ui_interface->addButton("gate", &fbutton0);
		ui_interface->addNumEntry("op0:egL1", &fentry26, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL2", &fentry25, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL3", &fentry20, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL4", &fentry22, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR1", &fentry28, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR2", &fentry27, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR3", &fentry29, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR4", &fentry23, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:keyVelSens", &fentry21, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op0:level", &fentry19, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:opDetune", &fentry16, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op0:opFreq", &fentry18, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op0:opMode", &fentry17, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op0:opRateScale", &fentry24, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL1", &fentry10, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL2", &fentry12, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL3", &fentry2, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL4", &fentry4, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR1", &fentry11, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR2", &fentry13, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR3", &fentry14, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR4", &fentry5, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:keyVelSens", &fentry3, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op1:level", &fentry1, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:opDetune", &fentry7, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op1:opFreq", &fentry9, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op1:opMode", &fentry8, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op1:opRateScale", &fentry6, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL1", &fentry39, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL2", &fentry41, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL3", &fentry31, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL4", &fentry33, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR1", &fentry40, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR2", &fentry42, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR3", &fentry43, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR4", &fentry34, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:keyVelSens", &fentry32, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op2:level", &fentry30, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:opDetune", &fentry36, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op2:opFreq", &fentry38, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op2:opMode", &fentry37, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op2:opRateScale", &fentry35, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL1", &fentry53, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL2", &fentry55, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL3", &fentry45, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL4", &fentry47, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR1", &fentry54, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR2", &fentry56, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR3", &fentry57, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR4", &fentry48, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:keyVelSens", &fentry46, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op3:level", &fentry44, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:opDetune", &fentry50, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op3:opFreq", &fentry52, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op3:opMode", &fentry51, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op3:opRateScale", &fentry49, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL1", &fentry81, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL2", &fentry83, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL3", &fentry76, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL4", &fentry78, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR1", &fentry82, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR2", &fentry84, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR3", &fentry85, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR4", &fentry79, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:keyVelSens", &fentry77, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op4:level", &fentry75, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:opDetune", &fentry72, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op4:opFreq", &fentry74, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op4:opMode", &fentry73, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op4:opRateScale", &fentry80, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL1", &fentry67, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL2", &fentry69, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL3", &fentry62, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL4", &fentry64, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR1", &fentry68, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR2", &fentry70, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR3", &fentry71, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR4", &fentry65, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:keyVelSens", &fentry63, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op5:level", &fentry61, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:opDetune", &fentry58, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op5:opFreq", &fentry60, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op5:opMode", &fentry59, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op5:opRateScale", &fentry66, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		//zone1
		//zone2
		int 	iSlow0 = int(float(fentry0));
		int 	iSlow1 = (float((iSlow0 == 1)) != 0.0f);
		float 	fSlow2 = float(fbutton0);
		int 	iSlow3 = (fSlow2 > 0.0f);
		int 	iSlow4 = int((iSlow3 > 0));
		float 	fSlow5 = float(fentry1);
		float 	fSlow6 = float(fentry2);
		float 	fSlow7 = float(fentry3);
		float 	fSlow8 = float(fslider0);
		float 	fSlow9 = ((0.0012626262626262627f * (fSlow7 * fSlow8)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow7))));
		float 	fSlow10 = ((fSlow5 * fSlow6) * fSlow9);
		float 	fSlow11 = float(fentry4);
		float 	fSlow12 = ((fSlow5 * fSlow11) * fSlow9);
		int 	iSlow13 = int((fSlow10 <= fSlow12));
		float 	fSlow14 = float(fentry9);
		float 	fSlow15 = float(fslider1);
		float 	fSlow16 = ((0.25f * float(fentry7)) + ((int(float(fentry8)))?fSlow14:(fSlow14 * fSlow15)));
		float 	fSlow17 = ((0.057142857142857148f * float(fentry6)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow16))) + 48.0f));
		float 	fSlow18 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry5) + fSlow17))));
		float 	fSlow19 = powf((0.014705882352941176f * (float((fSlow10 >= 30.0f)) * min((fSlow10 + -30.0f), (float)68))),1.8f);
		float 	fSlow20 = powf((0.010101010101010102f * min((float)99, fSlow10)),0.69999999999999996f);
		float 	fSlow21 = powf((0.014705882352941176f * (float((fSlow12 >= 30.0f)) * min((fSlow12 + -30.0f), (float)68))),1.8f);
		float 	fSlow22 = powf((0.010101010101010102f * min((float)99, fSlow12)),0.69999999999999996f);
		float 	fSlow23 = max(0.001f, fabsf((((iSlow13)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow18))):max(0.0080000000000000002f, (318.0f * powf(fSlow18,12.6f)))) * (((iSlow13)?fSlow19:fSlow20) - ((iSlow13)?fSlow21:fSlow22)))));
		float 	fSlow24 = (fConst0 * fSlow23);
		int 	iSlow25 = (float((iSlow0 == 0)) != 0.0f);
		int 	iSlow26 = int(iSlow3);
		float 	fSlow27 = float(iSlow3);
		int 	iSlow28 = int(((fSlow2 == 0.0f) > 0));
		float 	fSlow29 = float(fentry10);
		float 	fSlow30 = ((fSlow5 * fSlow29) * fSlow9);
		int 	iSlow31 = int((fSlow30 > fSlow12));
		float 	fSlow32 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry11) + fSlow17))));
		float 	fSlow33 = powf((0.014705882352941176f * (float((fSlow30 >= 30.0f)) * min((fSlow30 + -30.0f), (float)68))),1.8f);
		float 	fSlow34 = powf((0.010101010101010102f * min((float)99, fSlow30)),0.69999999999999996f);
		float 	fSlow35 = max(0.001f, fabsf((((iSlow31)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow32))):max(0.0080000000000000002f, (318.0f * powf(fSlow32,12.6f)))) * (((iSlow31)?fSlow21:fSlow22) - ((iSlow31)?fSlow33:fSlow34)))));
		float 	fSlow36 = float(fentry12);
		float 	fSlow37 = ((fSlow5 * fSlow36) * fSlow9);
		int 	iSlow38 = int((fSlow37 > fSlow30));
		float 	fSlow39 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry13) + fSlow17))));
		float 	fSlow40 = powf((0.014705882352941176f * (float((fSlow37 >= 30.0f)) * min((fSlow37 + -30.0f), (float)68))),1.8f);
		float 	fSlow41 = powf((0.010101010101010102f * min((float)99, fSlow37)),0.69999999999999996f);
		float 	fSlow42 = (fSlow35 + max(0.001f, fabsf((((iSlow38)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow39))):max(0.0080000000000000002f, (318.0f * powf(fSlow39,12.6f)))) * (((iSlow38)?fSlow40:fSlow41) - ((iSlow38)?fSlow33:fSlow34))))));
		int 	iSlow43 = int((fSlow10 > fSlow37));
		float 	fSlow44 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry14) + fSlow17))));
		float 	fSlow45 = (fSlow42 + max(0.001f, fabsf((((iSlow43)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow44))):max(0.0080000000000000002f, (318.0f * powf(fSlow44,12.6f)))) * (((iSlow43)?fSlow40:fSlow41) - ((iSlow43)?fSlow19:fSlow20))))));
		float 	fSlow46 = (fConst0 * fSlow45);
		float 	fSlow47 = (fConst0 * fSlow42);
		float 	fSlow48 = (fConst0 * fSlow35);
		float 	fSlow49 = (fSlow5 * fSlow9);
		float 	fSlow50 = (fConst1 * (fSlow29 - fSlow11));
		float 	fSlow51 = (0 - ((fSlow5 * (fSlow29 - fSlow36)) * fSlow9));
		float 	fSlow52 = (0 - (fConst0 * (fSlow35 - fSlow42)));
		float 	fSlow53 = (0 - ((fSlow5 * (fSlow36 - fSlow6)) * fSlow9));
		float 	fSlow54 = (0 - (fConst0 * (fSlow42 - fSlow45)));
		float 	fSlow55 = (fConst1 * fSlow16);
		float 	fSlow56 = (1825.3162113323297f * float(fentry15));
		float 	fSlow57 = float(fentry18);
		float 	fSlow58 = ((0.25f * float(fentry16)) + ((int(float(fentry17)))?fSlow57:(fSlow15 * fSlow57)));
		float 	fSlow59 = (fConst1 * fSlow58);
		float 	fSlow60 = float(fentry19);
		float 	fSlow61 = float(fentry20);
		float 	fSlow62 = float(fentry21);
		float 	fSlow63 = ((0.0012626262626262627f * (fSlow8 * fSlow62)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow62))));
		float 	fSlow64 = ((fSlow60 * fSlow61) * fSlow63);
		float 	fSlow65 = float(fentry22);
		float 	fSlow66 = ((fSlow60 * fSlow65) * fSlow63);
		int 	iSlow67 = int((fSlow64 <= fSlow66));
		float 	fSlow68 = ((0.057142857142857148f * float(fentry24)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow58))) + 48.0f));
		float 	fSlow69 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry23) + fSlow68))));
		float 	fSlow70 = powf((0.014705882352941176f * (float((fSlow64 >= 30.0f)) * min((fSlow64 + -30.0f), (float)68))),1.8f);
		float 	fSlow71 = powf((0.010101010101010102f * min((float)99, fSlow64)),0.69999999999999996f);
		float 	fSlow72 = powf((0.014705882352941176f * (float((fSlow66 >= 30.0f)) * min((fSlow66 + -30.0f), (float)68))),1.8f);
		float 	fSlow73 = powf((0.010101010101010102f * min((float)99, fSlow66)),0.69999999999999996f);
		float 	fSlow74 = max(0.001f, fabsf((((iSlow67)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow69))):max(0.0080000000000000002f, (318.0f * powf(fSlow69,12.6f)))) * (((iSlow67)?fSlow70:fSlow71) - ((iSlow67)?fSlow72:fSlow73)))));
		float 	fSlow75 = (fConst0 * fSlow74);
		float 	fSlow76 = float(fentry25);
		float 	fSlow77 = ((fSlow60 * fSlow76) * fSlow63);
		float 	fSlow78 = float(fentry26);
		float 	fSlow79 = ((fSlow60 * fSlow78) * fSlow63);
		int 	iSlow80 = int((fSlow77 > fSlow79));
		float 	fSlow81 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry27) + fSlow68))));
		float 	fSlow82 = powf((0.014705882352941176f * (float((fSlow77 >= 30.0f)) * min((fSlow77 + -30.0f), (float)68))),1.8f);
		float 	fSlow83 = powf((0.010101010101010102f * min((float)99, fSlow77)),0.69999999999999996f);
		float 	fSlow84 = powf((0.014705882352941176f * (float((fSlow79 >= 30.0f)) * min((fSlow79 + -30.0f), (float)68))),1.8f);
		float 	fSlow85 = powf((0.010101010101010102f * min((float)99, fSlow79)),0.69999999999999996f);
		int 	iSlow86 = int((fSlow79 > fSlow66));
		float 	fSlow87 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry28) + fSlow68))));
		float 	fSlow88 = max(0.001f, fabsf((((iSlow86)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow87))):max(0.0080000000000000002f, (318.0f * powf(fSlow87,12.6f)))) * (((iSlow86)?fSlow72:fSlow73) - ((iSlow86)?fSlow84:fSlow85)))));
		float 	fSlow89 = (max(0.001f, fabsf((((iSlow80)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow81))):max(0.0080000000000000002f, (318.0f * powf(fSlow81,12.6f)))) * (((iSlow80)?fSlow82:fSlow83) - ((iSlow80)?fSlow84:fSlow85))))) + fSlow88);
		int 	iSlow90 = int((fSlow64 > fSlow77));
		float 	fSlow91 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry29) + fSlow68))));
		float 	fSlow92 = (fSlow89 + max(0.001f, fabsf((((iSlow90)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow91))):max(0.0080000000000000002f, (318.0f * powf(fSlow91,12.6f)))) * (((iSlow90)?fSlow82:fSlow83) - ((iSlow90)?fSlow70:fSlow71))))));
		float 	fSlow93 = (fConst0 * fSlow92);
		float 	fSlow94 = (fConst0 * fSlow89);
		float 	fSlow95 = (fConst0 * fSlow88);
		float 	fSlow96 = (fConst1 * (0 - ((fSlow60 * (fSlow65 - fSlow78)) * fSlow63)));
		float 	fSlow97 = (fConst1 * (0 - ((fSlow60 * (fSlow78 - fSlow76)) * fSlow63)));
		float 	fSlow98 = (fSlow89 - fSlow88);
		float 	fSlow99 = (fSlow60 * fSlow63);
		float 	fSlow100 = (fSlow61 - fSlow76);
		float 	fSlow101 = (0 - (fConst0 * (fSlow89 - fSlow92)));
		float 	fSlow102 = float(fentry30);
		float 	fSlow103 = float(fentry31);
		float 	fSlow104 = float(fentry32);
		float 	fSlow105 = ((0.0012626262626262627f * (fSlow8 * fSlow104)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow104))));
		float 	fSlow106 = ((fSlow102 * fSlow103) * fSlow105);
		float 	fSlow107 = float(fentry33);
		float 	fSlow108 = ((fSlow102 * fSlow107) * fSlow105);
		int 	iSlow109 = int((fSlow106 <= fSlow108));
		float 	fSlow110 = float(fentry38);
		float 	fSlow111 = ((0.25f * float(fentry36)) + ((int(float(fentry37)))?fSlow110:(fSlow15 * fSlow110)));
		float 	fSlow112 = ((0.057142857142857148f * float(fentry35)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow111))) + 48.0f));
		float 	fSlow113 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry34) + fSlow112))));
		float 	fSlow114 = powf((0.014705882352941176f * (float((fSlow106 >= 30.0f)) * min((fSlow106 + -30.0f), (float)68))),1.8f);
		float 	fSlow115 = powf((0.010101010101010102f * min((float)99, fSlow106)),0.69999999999999996f);
		float 	fSlow116 = powf((0.014705882352941176f * (float((fSlow108 >= 30.0f)) * min((fSlow108 + -30.0f), (float)68))),1.8f);
		float 	fSlow117 = powf((0.010101010101010102f * min((float)99, fSlow108)),0.69999999999999996f);
		float 	fSlow118 = max(0.001f, fabsf((((iSlow109)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow113))):max(0.0080000000000000002f, (318.0f * powf(fSlow113,12.6f)))) * (((iSlow109)?fSlow114:fSlow115) - ((iSlow109)?fSlow116:fSlow117)))));
		float 	fSlow119 = (fConst0 * fSlow118);
		float 	fSlow120 = float(fentry39);
		float 	fSlow121 = ((fSlow102 * fSlow120) * fSlow105);
		int 	iSlow122 = int((fSlow121 > fSlow108));
		float 	fSlow123 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry40) + fSlow112))));
		float 	fSlow124 = powf((0.014705882352941176f * (float((fSlow121 >= 30.0f)) * min((fSlow121 + -30.0f), (float)68))),1.8f);
		float 	fSlow125 = powf((0.010101010101010102f * min((float)99, fSlow121)),0.69999999999999996f);
		float 	fSlow126 = max(0.001f, fabsf((((iSlow122)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow123))):max(0.0080000000000000002f, (318.0f * powf(fSlow123,12.6f)))) * (((iSlow122)?fSlow116:fSlow117) - ((iSlow122)?fSlow124:fSlow125)))));
		float 	fSlow127 = float(fentry41);
		float 	fSlow128 = ((fSlow102 * fSlow127) * fSlow105);
		int 	iSlow129 = int((fSlow128 > fSlow121));
		float 	fSlow130 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry42) + fSlow112))));
		float 	fSlow131 = powf((0.014705882352941176f * (float((fSlow128 >= 30.0f)) * min((fSlow128 + -30.0f), (float)68))),1.8f);
		float 	fSlow132 = powf((0.010101010101010102f * min((float)99, fSlow128)),0.69999999999999996f);
		float 	fSlow133 = (fSlow126 + max(0.001f, fabsf((((iSlow129)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow130))):max(0.0080000000000000002f, (318.0f * powf(fSlow130,12.6f)))) * (((iSlow129)?fSlow131:fSlow132) - ((iSlow129)?fSlow124:fSlow125))))));
		int 	iSlow134 = int((fSlow106 > fSlow128));
		float 	fSlow135 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry43) + fSlow112))));
		float 	fSlow136 = (fSlow133 + max(0.001f, fabsf((((iSlow134)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow135))):max(0.0080000000000000002f, (318.0f * powf(fSlow135,12.6f)))) * (((iSlow134)?fSlow131:fSlow132) - ((iSlow134)?fSlow114:fSlow115))))));
		float 	fSlow137 = (fConst0 * fSlow136);
		float 	fSlow138 = (fConst0 * fSlow133);
		float 	fSlow139 = (fConst0 * fSlow126);
		float 	fSlow140 = (fSlow102 * fSlow105);
		float 	fSlow141 = (fConst1 * (0 - (fSlow140 * (fSlow107 - fSlow120))));
		float 	fSlow142 = (0 - (fSlow140 * (fSlow120 - fSlow127)));
		float 	fSlow143 = (0 - (fConst0 * (fSlow126 - fSlow133)));
		float 	fSlow144 = (fSlow103 - fSlow127);
		float 	fSlow145 = (0 - (fConst0 * (fSlow133 - fSlow136)));
		float 	fSlow146 = float(fentry44);
		float 	fSlow147 = float(fentry45);
		float 	fSlow148 = float(fentry46);
		float 	fSlow149 = ((0.0012626262626262627f * (fSlow8 * fSlow148)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow148))));
		float 	fSlow150 = ((fSlow146 * fSlow147) * fSlow149);
		float 	fSlow151 = float(fentry47);
		float 	fSlow152 = ((fSlow146 * fSlow151) * fSlow149);
		int 	iSlow153 = int((fSlow150 <= fSlow152));
		float 	fSlow154 = float(fentry52);
		float 	fSlow155 = ((0.25f * float(fentry50)) + ((int(float(fentry51)))?fSlow154:(fSlow15 * fSlow154)));
		float 	fSlow156 = ((0.057142857142857148f * float(fentry49)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow155))) + 48.0f));
		float 	fSlow157 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry48) + fSlow156))));
		float 	fSlow158 = powf((0.014705882352941176f * (float((fSlow150 >= 30.0f)) * min((fSlow150 + -30.0f), (float)68))),1.8f);
		float 	fSlow159 = powf((0.010101010101010102f * min((float)99, fSlow150)),0.69999999999999996f);
		float 	fSlow160 = powf((0.014705882352941176f * (float((fSlow152 >= 30.0f)) * min((fSlow152 + -30.0f), (float)68))),1.8f);
		float 	fSlow161 = powf((0.010101010101010102f * min((float)99, fSlow152)),0.69999999999999996f);
		float 	fSlow162 = max(0.001f, fabsf((((iSlow153)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow157))):max(0.0080000000000000002f, (318.0f * powf(fSlow157,12.6f)))) * (((iSlow153)?fSlow158:fSlow159) - ((iSlow153)?fSlow160:fSlow161)))));
		float 	fSlow163 = (fConst0 * fSlow162);
		float 	fSlow164 = float(fentry53);
		float 	fSlow165 = ((fSlow146 * fSlow164) * fSlow149);
		int 	iSlow166 = int((fSlow165 > fSlow152));
		float 	fSlow167 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry54) + fSlow156))));
		float 	fSlow168 = powf((0.014705882352941176f * (float((fSlow165 >= 30.0f)) * min((fSlow165 + -30.0f), (float)68))),1.8f);
		float 	fSlow169 = powf((0.010101010101010102f * min((float)99, fSlow165)),0.69999999999999996f);
		float 	fSlow170 = max(0.001f, fabsf((((iSlow166)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow167))):max(0.0080000000000000002f, (318.0f * powf(fSlow167,12.6f)))) * (((iSlow166)?fSlow160:fSlow161) - ((iSlow166)?fSlow168:fSlow169)))));
		float 	fSlow171 = float(fentry55);
		float 	fSlow172 = ((fSlow146 * fSlow171) * fSlow149);
		int 	iSlow173 = int((fSlow172 > fSlow165));
		float 	fSlow174 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry56) + fSlow156))));
		float 	fSlow175 = powf((0.014705882352941176f * (float((fSlow172 >= 30.0f)) * min((fSlow172 + -30.0f), (float)68))),1.8f);
		float 	fSlow176 = powf((0.010101010101010102f * min((float)99, fSlow172)),0.69999999999999996f);
		float 	fSlow177 = (fSlow170 + max(0.001f, fabsf((((iSlow173)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow174))):max(0.0080000000000000002f, (318.0f * powf(fSlow174,12.6f)))) * (((iSlow173)?fSlow175:fSlow176) - ((iSlow173)?fSlow168:fSlow169))))));
		int 	iSlow178 = int((fSlow150 > fSlow172));
		float 	fSlow179 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry57) + fSlow156))));
		float 	fSlow180 = (fSlow177 + max(0.001f, fabsf((((iSlow178)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow179))):max(0.0080000000000000002f, (318.0f * powf(fSlow179,12.6f)))) * (((iSlow178)?fSlow175:fSlow176) - ((iSlow178)?fSlow158:fSlow159))))));
		float 	fSlow181 = (fConst0 * fSlow180);
		float 	fSlow182 = (fConst0 * fSlow177);
		float 	fSlow183 = (fConst0 * fSlow170);
		float 	fSlow184 = (fSlow146 * fSlow149);
		float 	fSlow185 = (fConst1 * (0 - (fSlow184 * (fSlow151 - fSlow164))));
		float 	fSlow186 = (0 - (fSlow184 * (fSlow164 - fSlow171)));
		float 	fSlow187 = (0 - (fConst0 * (fSlow170 - fSlow177)));
		float 	fSlow188 = (fSlow147 - fSlow171);
		float 	fSlow189 = (0 - (fConst0 * (fSlow177 - fSlow180)));
		float 	fSlow190 = float(fentry60);
		float 	fSlow191 = ((0.25f * float(fentry58)) + ((int(float(fentry59)))?fSlow190:(fSlow15 * fSlow190)));
		float 	fSlow192 = (fConst1 * fSlow191);
		float 	fSlow193 = float(fentry61);
		float 	fSlow194 = float(fentry62);
		float 	fSlow195 = float(fentry63);
		float 	fSlow196 = ((0.0012626262626262627f * (fSlow8 * fSlow195)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow195))));
		float 	fSlow197 = ((fSlow193 * fSlow194) * fSlow196);
		float 	fSlow198 = float(fentry64);
		float 	fSlow199 = ((fSlow193 * fSlow198) * fSlow196);
		int 	iSlow200 = int((fSlow197 <= fSlow199));
		float 	fSlow201 = ((0.057142857142857148f * float(fentry66)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow191))) + 48.0f));
		float 	fSlow202 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry65) + fSlow201))));
		float 	fSlow203 = powf((0.014705882352941176f * (float((fSlow197 >= 30.0f)) * min((fSlow197 + -30.0f), (float)68))),1.8f);
		float 	fSlow204 = powf((0.010101010101010102f * min((float)99, fSlow197)),0.69999999999999996f);
		float 	fSlow205 = powf((0.014705882352941176f * (float((fSlow199 >= 30.0f)) * min((fSlow199 + -30.0f), (float)68))),1.8f);
		float 	fSlow206 = powf((0.010101010101010102f * min((float)99, fSlow199)),0.69999999999999996f);
		float 	fSlow207 = max(0.001f, fabsf((((iSlow200)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow202))):max(0.0080000000000000002f, (318.0f * powf(fSlow202,12.6f)))) * (((iSlow200)?fSlow203:fSlow204) - ((iSlow200)?fSlow205:fSlow206)))));
		float 	fSlow208 = (fConst0 * fSlow207);
		float 	fSlow209 = float(fentry67);
		float 	fSlow210 = ((fSlow193 * fSlow209) * fSlow196);
		int 	iSlow211 = int((fSlow210 > fSlow199));
		float 	fSlow212 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry68) + fSlow201))));
		float 	fSlow213 = powf((0.014705882352941176f * (float((fSlow210 >= 30.0f)) * min((fSlow210 + -30.0f), (float)68))),1.8f);
		float 	fSlow214 = powf((0.010101010101010102f * min((float)99, fSlow210)),0.69999999999999996f);
		float 	fSlow215 = max(0.001f, fabsf((((iSlow211)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow212))):max(0.0080000000000000002f, (318.0f * powf(fSlow212,12.6f)))) * (((iSlow211)?fSlow205:fSlow206) - ((iSlow211)?fSlow213:fSlow214)))));
		float 	fSlow216 = float(fentry69);
		float 	fSlow217 = ((fSlow193 * fSlow216) * fSlow196);
		int 	iSlow218 = int((fSlow217 > fSlow210));
		float 	fSlow219 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry70) + fSlow201))));
		float 	fSlow220 = powf((0.014705882352941176f * (float((fSlow217 >= 30.0f)) * min((fSlow217 + -30.0f), (float)68))),1.8f);
		float 	fSlow221 = powf((0.010101010101010102f * min((float)99, fSlow217)),0.69999999999999996f);
		float 	fSlow222 = (fSlow215 + max(0.001f, fabsf((((iSlow218)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow219))):max(0.0080000000000000002f, (318.0f * powf(fSlow219,12.6f)))) * (((iSlow218)?fSlow220:fSlow221) - ((iSlow218)?fSlow213:fSlow214))))));
		int 	iSlow223 = int((fSlow197 > fSlow217));
		float 	fSlow224 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry71) + fSlow201))));
		float 	fSlow225 = (fSlow222 + max(0.001f, fabsf((((iSlow223)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow224))):max(0.0080000000000000002f, (318.0f * powf(fSlow224,12.6f)))) * (((iSlow223)?fSlow220:fSlow221) - ((iSlow223)?fSlow203:fSlow204))))));
		float 	fSlow226 = (fConst0 * fSlow225);
		float 	fSlow227 = (fConst0 * fSlow222);
		float 	fSlow228 = (fConst0 * fSlow215);
		float 	fSlow229 = (fSlow193 * fSlow196);
		float 	fSlow230 = (fConst1 * (0 - (fSlow229 * (fSlow198 - fSlow209))));
		float 	fSlow231 = (0 - (fSlow229 * (fSlow209 - fSlow216)));
		float 	fSlow232 = (0 - (fConst0 * (fSlow215 - fSlow222)));
		float 	fSlow233 = (fSlow194 - fSlow216);
		float 	fSlow234 = (0 - (fConst0 * (fSlow222 - fSlow225)));
		float 	fSlow235 = float(fentry74);
		float 	fSlow236 = ((0.25f * float(fentry72)) + ((int(float(fentry73)))?fSlow235:(fSlow15 * fSlow235)));
		float 	fSlow237 = (fConst1 * fSlow236);
		float 	fSlow238 = float(fentry75);
		float 	fSlow239 = float(fentry76);
		float 	fSlow240 = float(fentry77);
		float 	fSlow241 = ((0.0012626262626262627f * (fSlow8 * fSlow240)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow240))));
		float 	fSlow242 = ((fSlow238 * fSlow239) * fSlow241);
		float 	fSlow243 = float(fentry78);
		float 	fSlow244 = ((fSlow238 * fSlow243) * fSlow241);
		int 	iSlow245 = int((fSlow242 <= fSlow244));
		float 	fSlow246 = ((0.057142857142857148f * float(fentry80)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow236))) + 48.0f));
		float 	fSlow247 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry79) + fSlow246))));
		float 	fSlow248 = powf((0.014705882352941176f * (float((fSlow242 >= 30.0f)) * min((fSlow242 + -30.0f), (float)68))),1.8f);
		float 	fSlow249 = powf((0.010101010101010102f * min((float)99, fSlow242)),0.69999999999999996f);
		float 	fSlow250 = powf((0.014705882352941176f * (float((fSlow244 >= 30.0f)) * min((fSlow244 + -30.0f), (float)68))),1.8f);
		float 	fSlow251 = powf((0.010101010101010102f * min((float)99, fSlow244)),0.69999999999999996f);
		float 	fSlow252 = max(0.001f, fabsf((((iSlow245)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow247))):max(0.0080000000000000002f, (318.0f * powf(fSlow247,12.6f)))) * (((iSlow245)?fSlow248:fSlow249) - ((iSlow245)?fSlow250:fSlow251)))));
		float 	fSlow253 = (fConst0 * fSlow252);
		float 	fSlow254 = float(fentry81);
		float 	fSlow255 = ((fSlow238 * fSlow254) * fSlow241);
		int 	iSlow256 = int((fSlow255 > fSlow244));
		float 	fSlow257 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry82) + fSlow246))));
		float 	fSlow258 = powf((0.014705882352941176f * (float((fSlow255 >= 30.0f)) * min((fSlow255 + -30.0f), (float)68))),1.8f);
		float 	fSlow259 = powf((0.010101010101010102f * min((float)99, fSlow255)),0.69999999999999996f);
		float 	fSlow260 = max(0.001f, fabsf((((iSlow256)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow257))):max(0.0080000000000000002f, (318.0f * powf(fSlow257,12.6f)))) * (((iSlow256)?fSlow250:fSlow251) - ((iSlow256)?fSlow258:fSlow259)))));
		float 	fSlow261 = float(fentry83);
		float 	fSlow262 = ((fSlow238 * fSlow261) * fSlow241);
		int 	iSlow263 = int((fSlow262 > fSlow255));
		float 	fSlow264 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry84) + fSlow246))));
		float 	fSlow265 = powf((0.014705882352941176f * (float((fSlow262 >= 30.0f)) * min((fSlow262 + -30.0f), (float)68))),1.8f);
		float 	fSlow266 = powf((0.010101010101010102f * min((float)99, fSlow262)),0.69999999999999996f);
		float 	fSlow267 = (fSlow260 + max(0.001f, fabsf((((iSlow263)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow264))):max(0.0080000000000000002f, (318.0f * powf(fSlow264,12.6f)))) * (((iSlow263)?fSlow265:fSlow266) - ((iSlow263)?fSlow258:fSlow259))))));
		int 	iSlow268 = int((fSlow242 > fSlow262));
		float 	fSlow269 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry85) + fSlow246))));
		float 	fSlow270 = (fSlow267 + max(0.001f, fabsf((((iSlow268)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow269))):max(0.0080000000000000002f, (318.0f * powf(fSlow269,12.6f)))) * (((iSlow268)?fSlow265:fSlow266) - ((iSlow268)?fSlow248:fSlow249))))));
		float 	fSlow271 = (fConst0 * fSlow270);
		float 	fSlow272 = (fConst0 * fSlow267);
		float 	fSlow273 = (fConst0 * fSlow260);
		float 	fSlow274 = (fSlow238 * fSlow241);
		float 	fSlow275 = (fConst1 * (0 - (fSlow274 * (fSlow243 - fSlow254))));
		float 	fSlow276 = (0 - (fSlow274 * (fSlow254 - fSlow261)));
		float 	fSlow277 = (0 - (fConst0 * (fSlow260 - fSlow267)));
		float 	fSlow278 = (fSlow239 - fSlow261);
		float 	fSlow279 = (0 - (fConst0 * (fSlow267 - fSlow270)));
		float 	fSlow280 = (fConst1 * fSlow155);
		float 	fSlow281 = (fConst1 * fSlow111);
		//zone2b
		//zone3
		FAUSTFLOAT* output0 = output[0];
		//LoopGraphScalar
		for (int i=0; i<count; i++) {
			if (iSlow1 || iSlow25) {
				fRec2[0] = ((iSlow4)?0.0f:min(fSlow24, (fRec2[1] + 1.0f)));
				fRec4[0] = ((iSlow28)?0.0f:min(fSlow46, (fRec4[1] + 1.0f)));
				iTempPerm1 = int((fRec4[0] < fSlow47));
				iTempPerm2 = int((fRec4[0] < fSlow48));
				fRec3[0] = ((iSlow26)?(fSlow27 * ((iTempPerm1)?((iTempPerm2)?((int((fRec4[0] < 0.0f)))?fSlow12:((iTempPerm2)?(fSlow49 * (fSlow11 + (fSlow50 * (fRec4[0] / fSlow35)))):fSlow30)):((iTempPerm1)?(fSlow30 + (fSlow51 * ((fRec4[0] - fSlow48) / fSlow52))):fSlow37)):((int((fRec4[0] < fSlow46)))?(fSlow37 + (fSlow53 * ((fRec4[0] - fSlow47) / fSlow54))):fSlow10))):fRec3[1]);
				fTempPerm3 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec2[0] < 0.0f)))?fRec3[0]:((int((fRec2[0] < fSlow24)))?((fConst1 * (((fSlow12 - fRec3[0]) * fRec2[0]) / fSlow23)) + fRec3[0]):fSlow12)))));
				fTempPerm4 = (fRec5[1] + fSlow55);
				fRec5[0] = (fTempPerm4 - floorf(fTempPerm4));
				fTempPerm5 = (65536.0f * fRec5[0]);
			}
			if (iSlow1) {
				fRec1[0] = (fTempPerm3 * ftbl0[(((int((fTempPerm5 + (fSlow56 * fRec1[1]))) % 65536) + 65536) % 65536)]);
			}
			if (iSlow1 || iSlow25) {
				fTempPerm6 = (fRec6[1] + fSlow59);
				fRec6[0] = (fTempPerm6 - floorf(fTempPerm6));
				fRec7[0] = ((iSlow4)?0.0f:min(fSlow75, (fRec7[1] + 1.0f)));
				fRec9[0] = ((iSlow28)?0.0f:min(fSlow93, (fRec9[1] + 1.0f)));
				iTempPerm7 = int((fRec9[0] < fSlow94));
				iTempPerm8 = int((fRec9[0] < fSlow95));
				fRec8[0] = ((iSlow26)?(fSlow27 * ((iTempPerm7)?((iTempPerm8)?((int((fRec9[0] < 0.0f)))?fSlow66:((iTempPerm8)?(fSlow66 + (fSlow96 * (fRec9[0] / fSlow88))):fSlow79)):((iTempPerm7)?(fSlow79 + (fSlow97 * ((fRec9[0] - fSlow95) / fSlow98))):fSlow77)):((int((fRec9[0] < fSlow93)))?(fSlow99 * (fSlow76 + (fSlow100 * ((fRec9[0] - fSlow94) / fSlow101)))):fSlow64))):fRec8[1]);
				fTempPerm9 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec7[0] < 0.0f)))?fRec8[0]:((int((fRec7[0] < fSlow75)))?((fConst1 * (((fSlow66 - fRec8[0]) * fRec7[0]) / fSlow74)) + fRec8[0]):fSlow66)))));
				fRec10[0] = ((iSlow4)?0.0f:min(fSlow119, (fRec10[1] + 1.0f)));
				fRec12[0] = ((iSlow28)?0.0f:min(fSlow137, (fRec12[1] + 1.0f)));
				iTempPerm10 = int((fRec12[0] < fSlow138));
				iTempPerm11 = int((fRec12[0] < fSlow139));
				fRec11[0] = ((iSlow26)?(fSlow27 * ((iTempPerm10)?((iTempPerm11)?((int((fRec12[0] < 0.0f)))?fSlow108:((iTempPerm11)?(fSlow108 + (fSlow141 * (fRec12[0] / fSlow126))):fSlow121)):((iTempPerm10)?(fSlow121 + (fSlow142 * ((fRec12[0] - fSlow139) / fSlow143))):fSlow128)):((int((fRec12[0] < fSlow137)))?(fSlow140 * (fSlow127 + (fSlow144 * ((fRec12[0] - fSlow138) / fSlow145)))):fSlow106))):fRec11[1]);
				fTempPerm12 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec10[0] < 0.0f)))?fRec11[0]:((int((fRec10[0] < fSlow119)))?((fConst1 * (((fSlow108 - fRec11[0]) * fRec10[0]) / fSlow118)) + fRec11[0]):fSlow108)))));
				fRec13[0] = ((iSlow4)?0.0f:min(fSlow163, (fRec13[1] + 1.0f)));
				fRec15[0] = ((iSlow28)?0.0f:min(fSlow181, (fRec15[1] + 1.0f)));
				iTempPerm13 = int((fRec15[0] < fSlow182));
				iTempPerm14 = int((fRec15[0] < fSlow183));
				fRec14[0] = ((iSlow26)?(fSlow27 * ((iTempPerm13)?((iTempPerm14)?((int((fRec15[0] < 0.0f)))?fSlow152:((iTempPerm14)?(fSlow152 + (fSlow185 * (fRec15[0] / fSlow170))):fSlow165)):((iTempPerm13)?(fSlow165 + (fSlow186 * ((fRec15[0] - fSlow183) / fSlow187))):fSlow172)):((int((fRec15[0] < fSlow181)))?(fSlow184 * (fSlow171 + (fSlow188 * ((fRec15[0] - fSlow182) / fSlow189)))):fSlow150))):fRec14[1]);
				fTempPerm15 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec13[0] < 0.0f)))?fRec14[0]:((int((fRec13[0] < fSlow163)))?((fConst1 * (((fSlow152 - fRec14[0]) * fRec13[0]) / fSlow162)) + fRec14[0]):fSlow152)))));
				fTempPerm16 = (fRec16[1] + fSlow192);
				fRec16[0] = (fTempPerm16 - floorf(fTempPerm16));
				fTempPerm17 = (65536.0f * fRec16[0]);
				fRec17[0] = ((iSlow4)?0.0f:min(fSlow208, (fRec17[1] + 1.0f)));
				fRec19[0] = ((iSlow28)?0.0f:min(fSlow226, (fRec19[1] + 1.0f)));
				iTempPerm18 = int((fRec19[0] < fSlow227));
				iTempPerm19 = int((fRec19[0] < fSlow228));
				fRec18[0] = ((iSlow26)?(fSlow27 * ((iTempPerm18)?((iTempPerm19)?((int((fRec19[0] < 0.0f)))?fSlow199:((iTempPerm19)?(fSlow199 + (fSlow230 * (fRec19[0] / fSlow215))):fSlow210)):((iTempPerm18)?(fSlow210 + (fSlow231 * ((fRec19[0] - fSlow228) / fSlow232))):fSlow217)):((int((fRec19[0] < fSlow226)))?(fSlow229 * (fSlow216 + (fSlow233 * ((fRec19[0] - fSlow227) / fSlow234)))):fSlow197))):fRec18[1]);
				fTempPerm20 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec17[0] < 0.0f)))?fRec18[0]:((int((fRec17[0] < fSlow208)))?((fConst1 * (((fSlow199 - fRec18[0]) * fRec17[0]) / fSlow207)) + fRec18[0]):fSlow199)))));
				fTempPerm21 = (fRec20[1] + fSlow237);
				fRec20[0] = (fTempPerm21 - floorf(fTempPerm21));
				fRec21[0] = ((iSlow4)?0.0f:min(fSlow253, (fRec21[1] + 1.0f)));
				fRec23[0] = ((iSlow28)?0.0f:min(fSlow271, (fRec23[1] + 1.0f)));
				iTempPerm22 = int((fRec23[0] < fSlow272));
				iTempPerm23 = int((fRec23[0] < fSlow273));
				fRec22[0] = ((iSlow26)?(fSlow27 * ((iTempPerm22)?((iTempPerm23)?((int((fRec23[0] < 0.0f)))?fSlow244:((iTempPerm23)?(fSlow244 + (fSlow275 * (fRec23[0] / fSlow260))):fSlow255)):((iTempPerm22)?(fSlow255 + (fSlow276 * ((fRec23[0] - fSlow273) / fSlow277))):fSlow262)):((int((fRec23[0] < fSlow271)))?(fSlow274 * (fSlow261 + (fSlow278 * ((fRec23[0] - fSlow272) / fSlow279)))):fSlow242))):fRec22[1]);
				fTempPerm24 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec21[0] < 0.0f)))?fRec22[0]:((int((fRec21[0] < fSlow253)))?((fConst1 * (((fSlow244 - fRec22[0]) * fRec21[0]) / fSlow252)) + fRec22[0]):fSlow244)))));
				fTempPerm25 = (fRec24[1] + fSlow280);
				fRec24[0] = (fTempPerm25 - floorf(fTempPerm25));
				fTempPerm26 = (fRec25[1] + fSlow281);
				fRec25[0] = (fTempPerm26 - floorf(fTempPerm26));
			}
			if (iSlow1) {
				fTempPerm27 = (2.0879500000000002f * ((ftbl0[(((int((65536.0f * (fRec1[0] + fRec6[0]))) % 65536) + 65536) % 65536)] * fTempPerm9) + (fTempPerm12 * ftbl0[(((int((65536.0f * ((fTempPerm15 * ftbl0[(((int((65536.0f * ((ftbl0[(((int((65536.0f * ((ftbl0[(((int(fTempPerm17) % 65536) + 65536) % 65536)] * fTempPerm20) + fRec20[0]))) % 65536) + 65536) % 65536)] * fTempPerm24) + fRec24[0]))) % 65536) + 65536) % 65536)]) + fRec25[0]))) % 65536) + 65536) % 65536)])));
			}
			if (iSlow25) {
				fRec26[0] = (fTempPerm20 * ftbl0[(((int((fTempPerm17 + (fSlow56 * fRec26[1]))) % 65536) + 65536) % 65536)]);
				fTempPerm28 = (2.0879500000000002f * ((ftbl0[(((int((65536.0f * ((fTempPerm3 * ftbl0[(((int(fTempPerm5) % 65536) + 65536) % 65536)]) + fRec6[0]))) % 65536) + 65536) % 65536)] * fTempPerm9) + (fTempPerm12 * ftbl0[(((int((65536.0f * ((fTempPerm15 * ftbl0[(((int((65536.0f * ((ftbl0[(((int((65536.0f * (fRec26[0] + fRec20[0]))) % 65536) + 65536) % 65536)] * fTempPerm24) + fRec24[0]))) % 65536) + 65536) % 65536)]) + fRec25[0]))) % 65536) + 65536) % 65536)])));
			}
			output0[i] = (FAUSTFLOAT)(fTempPerm27 + fTempPerm28);
			// post processing
			if (iSlow25) {
				fRec26[1] = fRec26[0];
			}
			if (iSlow1 || iSlow25) {
				fRec25[1] = fRec25[0];
				fRec24[1] = fRec24[0];
				fRec22[1] = fRec22[0];
				fRec23[1] = fRec23[0];
				fRec21[1] = fRec21[0];
				fRec20[1] = fRec20[0];
				fRec18[1] = fRec18[0];
				fRec19[1] = fRec19[0];
				fRec17[1] = fRec17[0];
				fRec16[1] = fRec16[0];
				fRec14[1] = fRec14[0];
				fRec15[1] = fRec15[0];
				fRec13[1] = fRec13[0];
				fRec11[1] = fRec11[0];
				fRec12[1] = fRec12[0];
				fRec10[1] = fRec10[0];
				fRec8[1] = fRec8[0];
				fRec9[1] = fRec9[0];
				fRec7[1] = fRec7[0];
				fRec6[1] = fRec6[0];
			}
			if (iSlow1) {
				fRec1[1] = fRec1[0];
			}
			if (iSlow1 || iSlow25) {
				fRec5[1] = fRec5[0];
				fRec3[1] = fRec3[0];
				fRec4[1] = fRec4[0];
				fRec2[1] = fRec2[0];
			}
		}
	}
};


float 	synth_dx7::ftbl0[65536];
// clang-format on
#endif

    template <class T>
    struct _synth_dx7_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_dx7_UI<T>::name(sym(synth_dx7));

typedef _synth_dx7_UI<synth_dx7> synth_dx7_UI;

class faust_synth_dx7_tilde : public FaustExternal<synth_dx7, synth_dx7_UI> {
public:
    faust_synth_dx7_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};
