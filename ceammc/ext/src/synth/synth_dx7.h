//----------------------------------------------------------
// name: "synth.dx7"
//
// Code generated with Faust 2.28.6 (https://faust.grame.fr)
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

/************************** BEGIN synth_dx7_dsp.h **************************/
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

class synth_dx7_dsp {

    public:

        synth_dx7_dsp() {}
        virtual ~synth_dx7_dsp() {}

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
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual synth_dx7_dsp* clone() = 0;
    
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

class decorator_dsp : public synth_dx7_dsp {

    protected:

        synth_dx7_dsp* fDSP;

    public:

        decorator_dsp(synth_dx7_dsp* synth_dx7_dsp = nullptr):fDSP(synth_dx7_dsp) {}
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
    
        virtual synth_dx7_dsp* createDSPInstance() = 0;
    
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
/**************************  END  synth_dx7_dsp.h **************************/
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
struct synth_dx7 : public synth_dx7_dsp {
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

class synth_dx7 : public synth_dx7_dsp {
  private:
	class SIG0 {
	  private:
		int fSampleRate;
		int 	iRec3[2];
	  public:
		int getNumInputs() { return 0; }
		int getNumOutputs() { return 1; }
		void init(int sample_rate) {
			fSampleRate = sample_rate;
			for (int i=0; i<2; i++) iRec3[i] = 0;
		}
		void fill(int count, float output[]) {
			for (int i=0; i<count; i++) {
				iRec3[0] = (iRec3[1] + 1);
				output[i] = sinf((9.5873799242852573e-05f * float((iRec3[0] + -1))));
				// post processing
				iRec3[1] = iRec3[0];
			}
		}
	};


	FAUSTFLOAT 	fentry0;
	FAUSTFLOAT 	fbutton0;
	float 	fConst0;
	FAUSTFLOAT 	fentry1;
	FAUSTFLOAT 	fentry2;
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT 	fentry3;
	FAUSTFLOAT 	fentry4;
	FAUSTFLOAT 	fentry5;
	FAUSTFLOAT 	fentry6;
	FAUSTFLOAT 	fentry7;
	FAUSTFLOAT 	fentry8;
	FAUSTFLOAT 	fentry9;
	FAUSTFLOAT 	fslider1;
	float 	fRec0[2];
	FAUSTFLOAT 	fentry10;
	FAUSTFLOAT 	fentry11;
	FAUSTFLOAT 	fentry12;
	FAUSTFLOAT 	fentry13;
	FAUSTFLOAT 	fentry14;
	float 	fRec2[2];
	int 	iTempPerm0;
	int 	iTempPerm1;
	float 	fConst1;
	float 	fRec1[2];
	float 	fTempPerm2;
	static float 	ftbl0[65536];
	float 	fTempPerm3;
	float 	fRec4[2];
	FAUSTFLOAT 	fentry15;
	FAUSTFLOAT 	fentry16;
	FAUSTFLOAT 	fentry17;
	FAUSTFLOAT 	fentry18;
	FAUSTFLOAT 	fentry19;
	FAUSTFLOAT 	fentry20;
	FAUSTFLOAT 	fentry21;
	FAUSTFLOAT 	fentry22;
	FAUSTFLOAT 	fentry23;
	float 	fRec5[2];
	FAUSTFLOAT 	fentry24;
	FAUSTFLOAT 	fentry25;
	FAUSTFLOAT 	fentry26;
	FAUSTFLOAT 	fentry27;
	FAUSTFLOAT 	fentry28;
	float 	fRec7[2];
	int 	iTempPerm4;
	int 	iTempPerm5;
	float 	fRec6[2];
	float 	fTempPerm6;
	float 	fTempPerm7;
	float 	fRec8[2];
	float 	fTempPerm8;
	FAUSTFLOAT 	fentry29;
	FAUSTFLOAT 	fentry30;
	FAUSTFLOAT 	fentry31;
	FAUSTFLOAT 	fentry32;
	FAUSTFLOAT 	fentry33;
	FAUSTFLOAT 	fentry34;
	FAUSTFLOAT 	fentry35;
	FAUSTFLOAT 	fentry36;
	FAUSTFLOAT 	fentry37;
	float 	fRec9[2];
	FAUSTFLOAT 	fentry38;
	FAUSTFLOAT 	fentry39;
	FAUSTFLOAT 	fentry40;
	FAUSTFLOAT 	fentry41;
	FAUSTFLOAT 	fentry42;
	float 	fRec11[2];
	int 	iTempPerm9;
	int 	iTempPerm10;
	float 	fRec10[2];
	float 	fTempPerm11;
	float 	fTempPerm12;
	float 	fRec12[2];
	FAUSTFLOAT 	fentry43;
	FAUSTFLOAT 	fentry44;
	FAUSTFLOAT 	fentry45;
	FAUSTFLOAT 	fentry46;
	FAUSTFLOAT 	fentry47;
	FAUSTFLOAT 	fentry48;
	FAUSTFLOAT 	fentry49;
	FAUSTFLOAT 	fentry50;
	FAUSTFLOAT 	fentry51;
	float 	fRec13[2];
	FAUSTFLOAT 	fentry52;
	FAUSTFLOAT 	fentry53;
	FAUSTFLOAT 	fentry54;
	FAUSTFLOAT 	fentry55;
	FAUSTFLOAT 	fentry56;
	float 	fRec15[2];
	int 	iTempPerm13;
	int 	iTempPerm14;
	float 	fRec14[2];
	float 	fTempPerm15;
	float 	fTempPerm16;
	float 	fRec16[2];
	FAUSTFLOAT 	fentry57;
	FAUSTFLOAT 	fentry58;
	FAUSTFLOAT 	fentry59;
	FAUSTFLOAT 	fentry60;
	FAUSTFLOAT 	fentry61;
	FAUSTFLOAT 	fentry62;
	FAUSTFLOAT 	fentry63;
	FAUSTFLOAT 	fentry64;
	FAUSTFLOAT 	fentry65;
	float 	fRec17[2];
	FAUSTFLOAT 	fentry66;
	FAUSTFLOAT 	fentry67;
	FAUSTFLOAT 	fentry68;
	FAUSTFLOAT 	fentry69;
	FAUSTFLOAT 	fentry70;
	float 	fRec19[2];
	int 	iTempPerm17;
	int 	iTempPerm18;
	float 	fRec18[2];
	float 	fTempPerm19;
	float 	fTempPerm20;
	float 	fRec20[2];
	FAUSTFLOAT 	fentry71;
	FAUSTFLOAT 	fentry72;
	FAUSTFLOAT 	fentry73;
	FAUSTFLOAT 	fentry74;
	FAUSTFLOAT 	fentry75;
	FAUSTFLOAT 	fentry76;
	FAUSTFLOAT 	fentry77;
	FAUSTFLOAT 	fentry78;
	FAUSTFLOAT 	fentry79;
	float 	fRec22[2];
	FAUSTFLOAT 	fentry80;
	FAUSTFLOAT 	fentry81;
	FAUSTFLOAT 	fentry82;
	FAUSTFLOAT 	fentry83;
	FAUSTFLOAT 	fentry84;
	float 	fRec24[2];
	int 	iTempPerm21;
	int 	iTempPerm22;
	float 	fRec23[2];
	float 	fTempPerm23;
	float 	fTempPerm24;
	float 	fRec25[2];
	float 	fTempPerm25;
	FAUSTFLOAT 	fentry85;
	float 	fRec21[2];
	float 	fTempPerm26;
	float 	fRec26[2];
	float 	fTempPerm27;
	int fSampleRate;

  public:
	virtual void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "synth_dx7.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "synth.dx7");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
	}

	virtual int getNumInputs() { return 0; }
	virtual int getNumOutputs() { return 1; }
	static void classInit(int sample_rate) {
		SIG0 sig0;
		sig0.init(sample_rate);
		sig0.fill(65536,ftbl0);
	}
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = min(192000.0f, max(1.0f, (float)fSampleRate));
		iTempPerm0 = 0;
		iTempPerm1 = 0;
		fConst1 = (1.0f / fConst0);
		fTempPerm2 = 0;
		fTempPerm3 = 0;
		iTempPerm4 = 0;
		iTempPerm5 = 0;
		fTempPerm6 = 0;
		fTempPerm7 = 0;
		fTempPerm8 = 0;
		iTempPerm9 = 0;
		iTempPerm10 = 0;
		fTempPerm11 = 0;
		fTempPerm12 = 0;
		iTempPerm13 = 0;
		iTempPerm14 = 0;
		fTempPerm15 = 0;
		fTempPerm16 = 0;
		iTempPerm17 = 0;
		iTempPerm18 = 0;
		fTempPerm19 = 0;
		fTempPerm20 = 0;
		iTempPerm21 = 0;
		iTempPerm22 = 0;
		fTempPerm23 = 0;
		fTempPerm24 = 0;
		fTempPerm25 = 0;
		fTempPerm26 = 0;
		fTempPerm27 = 0;
	}
	virtual void instanceResetUserInterface() {
		fentry0 = 0.0f;
		fbutton0 = 0.0;
		fentry1 = 95.0f;
		fentry2 = 90.0f;
		fslider0 = 0.80000000000000004f;
		fentry3 = 1.0f;
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
		fentry15 = 95.0f;
		fentry16 = 90.0f;
		fentry17 = 1.0f;
		fentry18 = 0.0f;
		fentry19 = 90.0f;
		fentry20 = 0.0f;
		fentry21 = 1.0f;
		fentry22 = 0.0f;
		fentry23 = 1.0f;
		fentry24 = 0.0f;
		fentry25 = 90.0f;
		fentry26 = 90.0f;
		fentry27 = 90.0f;
		fentry28 = 90.0f;
		fentry29 = 95.0f;
		fentry30 = 90.0f;
		fentry31 = 1.0f;
		fentry32 = 0.0f;
		fentry33 = 90.0f;
		fentry34 = 0.0f;
		fentry35 = 1.0f;
		fentry36 = 0.0f;
		fentry37 = 1.0f;
		fentry38 = 0.0f;
		fentry39 = 90.0f;
		fentry40 = 90.0f;
		fentry41 = 90.0f;
		fentry42 = 90.0f;
		fentry43 = 95.0f;
		fentry44 = 90.0f;
		fentry45 = 1.0f;
		fentry46 = 0.0f;
		fentry47 = 90.0f;
		fentry48 = 0.0f;
		fentry49 = 1.0f;
		fentry50 = 0.0f;
		fentry51 = 1.0f;
		fentry52 = 0.0f;
		fentry53 = 90.0f;
		fentry54 = 90.0f;
		fentry55 = 90.0f;
		fentry56 = 90.0f;
		fentry57 = 95.0f;
		fentry58 = 90.0f;
		fentry59 = 1.0f;
		fentry60 = 0.0f;
		fentry61 = 90.0f;
		fentry62 = 0.0f;
		fentry63 = 1.0f;
		fentry64 = 0.0f;
		fentry65 = 1.0f;
		fentry66 = 0.0f;
		fentry67 = 90.0f;
		fentry68 = 90.0f;
		fentry69 = 90.0f;
		fentry70 = 90.0f;
		fentry71 = 95.0f;
		fentry72 = 90.0f;
		fentry73 = 1.0f;
		fentry74 = 0.0f;
		fentry75 = 90.0f;
		fentry76 = 0.0f;
		fentry77 = 1.0f;
		fentry78 = 0.0f;
		fentry79 = 1.0f;
		fentry80 = 0.0f;
		fentry81 = 90.0f;
		fentry82 = 90.0f;
		fentry83 = 90.0f;
		fentry84 = 90.0f;
		fentry85 = 0.0f;
	}
	virtual void instanceClear() {
		for (int i=0; i<2; i++) fRec0[i] = 0;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		for (int i=0; i<2; i++) fRec1[i] = 0;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		for (int i=0; i<2; i++) fRec5[i] = 0;
		for (int i=0; i<2; i++) fRec7[i] = 0;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		for (int i=0; i<2; i++) fRec8[i] = 0;
		for (int i=0; i<2; i++) fRec9[i] = 0;
		for (int i=0; i<2; i++) fRec11[i] = 0;
		for (int i=0; i<2; i++) fRec10[i] = 0;
		for (int i=0; i<2; i++) fRec12[i] = 0;
		for (int i=0; i<2; i++) fRec13[i] = 0;
		for (int i=0; i<2; i++) fRec15[i] = 0;
		for (int i=0; i<2; i++) fRec14[i] = 0;
		for (int i=0; i<2; i++) fRec16[i] = 0;
		for (int i=0; i<2; i++) fRec17[i] = 0;
		for (int i=0; i<2; i++) fRec19[i] = 0;
		for (int i=0; i<2; i++) fRec18[i] = 0;
		for (int i=0; i<2; i++) fRec20[i] = 0;
		for (int i=0; i<2; i++) fRec22[i] = 0;
		for (int i=0; i<2; i++) fRec24[i] = 0;
		for (int i=0; i<2; i++) fRec23[i] = 0;
		for (int i=0; i<2; i++) fRec25[i] = 0;
		for (int i=0; i<2; i++) fRec21[i] = 0;
		for (int i=0; i<2; i++) fRec26[i] = 0;
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
	virtual synth_dx7* clone() {
		return new synth_dx7();
	}
	virtual int getSampleRate() {
		return fSampleRate;
	}
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.dx7");
		ui_interface->addNumEntry("algorithm", &fentry0, 0.0f, 0.0f, 31.0f, 1.0f);
		ui_interface->addNumEntry("feedback", &fentry85, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addHorizontalSlider("freq", &fslider1, 400.0f, 50.0f, 1000.0f, 0.01f);
		ui_interface->addHorizontalSlider("gain", &fslider0, 0.80000000000000004f, 0.0f, 1.0f, 0.01f);
		ui_interface->addButton("gate", &fbutton0);
		ui_interface->addNumEntry("op0:egL1", &fentry10, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL2", &fentry12, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL3", &fentry2, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL4", &fentry4, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR1", &fentry11, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR2", &fentry13, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR3", &fentry14, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR4", &fentry5, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:keyVelSens", &fentry3, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op0:level", &fentry1, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:opDetune", &fentry7, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op0:opFreq", &fentry9, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op0:opMode", &fentry8, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op0:opRateScale", &fentry6, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL1", &fentry24, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL2", &fentry26, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL3", &fentry16, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL4", &fentry18, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR1", &fentry25, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR2", &fentry27, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR3", &fentry28, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR4", &fentry19, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:keyVelSens", &fentry17, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op1:level", &fentry15, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:opDetune", &fentry21, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op1:opFreq", &fentry23, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op1:opMode", &fentry22, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op1:opRateScale", &fentry20, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL1", &fentry38, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL2", &fentry40, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL3", &fentry30, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL4", &fentry32, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR1", &fentry39, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR2", &fentry41, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR3", &fentry42, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR4", &fentry33, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:keyVelSens", &fentry31, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op2:level", &fentry29, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:opDetune", &fentry35, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op2:opFreq", &fentry37, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op2:opMode", &fentry36, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op2:opRateScale", &fentry34, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL1", &fentry52, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL2", &fentry54, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL3", &fentry44, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL4", &fentry46, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR1", &fentry53, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR2", &fentry55, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR3", &fentry56, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR4", &fentry47, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:keyVelSens", &fentry45, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op3:level", &fentry43, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:opDetune", &fentry49, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op3:opFreq", &fentry51, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op3:opMode", &fentry50, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op3:opRateScale", &fentry48, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL1", &fentry66, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL2", &fentry68, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL3", &fentry58, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL4", &fentry60, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR1", &fentry67, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR2", &fentry69, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR3", &fentry70, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR4", &fentry61, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:keyVelSens", &fentry59, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op4:level", &fentry57, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:opDetune", &fentry63, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op4:opFreq", &fentry65, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op4:opMode", &fentry64, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op4:opRateScale", &fentry62, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL1", &fentry80, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL2", &fentry82, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL3", &fentry72, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL4", &fentry74, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR1", &fentry81, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR2", &fentry83, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR3", &fentry84, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR4", &fentry75, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:keyVelSens", &fentry73, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op5:level", &fentry71, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:opDetune", &fentry77, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op5:opFreq", &fentry79, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op5:opMode", &fentry78, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op5:opRateScale", &fentry76, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		//zone1
		//zone2
		int 	iSlow0 = int(float(fentry0));
		int 	iSlow1 = (float((iSlow0 == 0)) != 0.0f);
		float 	fSlow2 = float(fbutton0);
		int 	iSlow3 = (fSlow2 > 0.0f);
		int 	iSlow4 = int((iSlow3 > 0));
		float 	fSlow5 = float(fentry1);
		float 	fSlow6 = float(fentry2);
		float 	fSlow7 = float(fslider0);
		float 	fSlow8 = float(fentry3);
		float 	fSlow9 = ((0.0012626262626262627f * (fSlow7 * fSlow8)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow8))));
		float 	fSlow10 = ((fSlow5 * fSlow6) * fSlow9);
		float 	fSlow11 = float(fentry4);
		float 	fSlow12 = ((fSlow5 * fSlow11) * fSlow9);
		int 	iSlow13 = int((fSlow10 <= fSlow12));
		float 	fSlow14 = float(fentry9);
		float 	fSlow15 = float(fslider1);
		float 	fSlow16 = ((0.25f * float(fentry7)) + ((int(float(fentry8)))?fSlow14:(fSlow15 * fSlow14)));
		float 	fSlow17 = ((0.057142857142857148f * float(fentry6)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow16))) + 48.0f));
		float 	fSlow18 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry5) + fSlow17))));
		float 	fSlow19 = powf((0.014705882352941176f * (float((fSlow10 >= 30.0f)) * min((fSlow10 + -30.0f), (float)68))),1.8f);
		float 	fSlow20 = powf((0.010101010101010102f * min((float)99, fSlow10)),0.69999999999999996f);
		float 	fSlow21 = powf((0.014705882352941176f * (float((fSlow12 >= 30.0f)) * min((fSlow12 + -30.0f), (float)68))),1.8f);
		float 	fSlow22 = powf((0.010101010101010102f * min((float)99, fSlow12)),0.69999999999999996f);
		float 	fSlow23 = max(0.001f, fabsf((((iSlow13)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow18))):max(0.0080000000000000002f, (318.0f * powf(fSlow18,12.6f)))) * (((iSlow13)?fSlow19:fSlow20) - ((iSlow13)?fSlow21:fSlow22)))));
		float 	fSlow24 = (fConst0 * fSlow23);
		int 	iSlow25 = (float((iSlow0 == 1)) != 0.0f);
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
		int 	iTemp0 = iTempPerm0;
		float 	fSlow48 = (fConst0 * fSlow35);
		int 	iTemp1 = iTempPerm1;
		float 	fSlow49 = (fSlow5 * fSlow9);
		float 	fSlow50 = (fConst1 * (fSlow29 - fSlow11));
		float 	fSlow51 = (0 - (fSlow49 * (fSlow29 - fSlow36)));
		float 	fSlow52 = (0 - (fConst0 * (fSlow35 - fSlow42)));
		float 	fSlow53 = (0 - (fSlow49 * (fSlow36 - fSlow6)));
		float 	fSlow54 = (0 - (fConst0 * (fSlow42 - fSlow45)));
		float 	fTemp2 = fTempPerm2;
		float 	fSlow55 = (fConst1 * fSlow16);
		float 	fTemp3 = fTempPerm3;
		float 	fSlow56 = float(fentry15);
		float 	fSlow57 = float(fentry16);
		float 	fSlow58 = float(fentry17);
		float 	fSlow59 = ((0.0012626262626262627f * (fSlow58 * fSlow7)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow58))));
		float 	fSlow60 = ((fSlow56 * fSlow57) * fSlow59);
		float 	fSlow61 = float(fentry18);
		float 	fSlow62 = ((fSlow56 * fSlow61) * fSlow59);
		int 	iSlow63 = int((fSlow60 <= fSlow62));
		float 	fSlow64 = float(fentry23);
		float 	fSlow65 = ((0.25f * float(fentry21)) + ((int(float(fentry22)))?fSlow64:(fSlow64 * fSlow15)));
		float 	fSlow66 = ((0.057142857142857148f * float(fentry20)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow65))) + 48.0f));
		float 	fSlow67 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry19) + fSlow66))));
		float 	fSlow68 = powf((0.014705882352941176f * (float((fSlow60 >= 30.0f)) * min((fSlow60 + -30.0f), (float)68))),1.8f);
		float 	fSlow69 = powf((0.010101010101010102f * min((float)99, fSlow60)),0.69999999999999996f);
		float 	fSlow70 = powf((0.014705882352941176f * (float((fSlow62 >= 30.0f)) * min((fSlow62 + -30.0f), (float)68))),1.8f);
		float 	fSlow71 = powf((0.010101010101010102f * min((float)99, fSlow62)),0.69999999999999996f);
		float 	fSlow72 = max(0.001f, fabsf((((iSlow63)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow67))):max(0.0080000000000000002f, (318.0f * powf(fSlow67,12.6f)))) * (((iSlow63)?fSlow68:fSlow69) - ((iSlow63)?fSlow70:fSlow71)))));
		float 	fSlow73 = (fConst0 * fSlow72);
		float 	fSlow74 = float(fentry24);
		float 	fSlow75 = ((fSlow56 * fSlow74) * fSlow59);
		int 	iSlow76 = int((fSlow75 > fSlow62));
		float 	fSlow77 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry25) + fSlow66))));
		float 	fSlow78 = powf((0.014705882352941176f * (float((fSlow75 >= 30.0f)) * min((fSlow75 + -30.0f), (float)68))),1.8f);
		float 	fSlow79 = powf((0.010101010101010102f * min((float)99, fSlow75)),0.69999999999999996f);
		float 	fSlow80 = max(0.001f, fabsf((((iSlow76)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow77))):max(0.0080000000000000002f, (318.0f * powf(fSlow77,12.6f)))) * (((iSlow76)?fSlow70:fSlow71) - ((iSlow76)?fSlow78:fSlow79)))));
		float 	fSlow81 = float(fentry26);
		float 	fSlow82 = ((fSlow56 * fSlow81) * fSlow59);
		int 	iSlow83 = int((fSlow82 > fSlow75));
		float 	fSlow84 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry27) + fSlow66))));
		float 	fSlow85 = powf((0.014705882352941176f * (float((fSlow82 >= 30.0f)) * min((fSlow82 + -30.0f), (float)68))),1.8f);
		float 	fSlow86 = powf((0.010101010101010102f * min((float)99, fSlow82)),0.69999999999999996f);
		float 	fSlow87 = (fSlow80 + max(0.001f, fabsf((((iSlow83)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow84))):max(0.0080000000000000002f, (318.0f * powf(fSlow84,12.6f)))) * (((iSlow83)?fSlow85:fSlow86) - ((iSlow83)?fSlow78:fSlow79))))));
		int 	iSlow88 = int((fSlow60 > fSlow82));
		float 	fSlow89 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry28) + fSlow66))));
		float 	fSlow90 = (fSlow87 + max(0.001f, fabsf((((iSlow88)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow89))):max(0.0080000000000000002f, (318.0f * powf(fSlow89,12.6f)))) * (((iSlow88)?fSlow85:fSlow86) - ((iSlow88)?fSlow68:fSlow69))))));
		float 	fSlow91 = (fConst0 * fSlow90);
		float 	fSlow92 = (fConst0 * fSlow87);
		int 	iTemp4 = iTempPerm4;
		float 	fSlow93 = (fConst0 * fSlow80);
		int 	iTemp5 = iTempPerm5;
		float 	fSlow94 = (fSlow56 * fSlow59);
		float 	fSlow95 = (fConst1 * (fSlow74 - fSlow61));
		float 	fSlow96 = (0 - (fSlow94 * (fSlow74 - fSlow81)));
		float 	fSlow97 = (0 - (fConst0 * (fSlow80 - fSlow87)));
		float 	fSlow98 = (0 - (fSlow94 * (fSlow81 - fSlow57)));
		float 	fSlow99 = (0 - (fConst0 * (fSlow87 - fSlow90)));
		float 	fTemp6 = fTempPerm6;
		float 	fSlow100 = (fConst1 * fSlow65);
		float 	fTemp7 = fTempPerm7;
		float 	fTemp8 = fTempPerm8;
		float 	fSlow101 = float(fentry29);
		float 	fSlow102 = float(fentry30);
		float 	fSlow103 = float(fentry31);
		float 	fSlow104 = ((0.0012626262626262627f * (fSlow7 * fSlow103)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow103))));
		float 	fSlow105 = ((fSlow101 * fSlow102) * fSlow104);
		float 	fSlow106 = float(fentry32);
		float 	fSlow107 = ((fSlow101 * fSlow106) * fSlow104);
		int 	iSlow108 = int((fSlow105 <= fSlow107));
		float 	fSlow109 = float(fentry37);
		float 	fSlow110 = ((0.25f * float(fentry35)) + ((int(float(fentry36)))?fSlow109:(fSlow15 * fSlow109)));
		float 	fSlow111 = ((0.057142857142857148f * float(fentry34)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow110))) + 48.0f));
		float 	fSlow112 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry33) + fSlow111))));
		float 	fSlow113 = powf((0.014705882352941176f * (float((fSlow105 >= 30.0f)) * min((fSlow105 + -30.0f), (float)68))),1.8f);
		float 	fSlow114 = powf((0.010101010101010102f * min((float)99, fSlow105)),0.69999999999999996f);
		float 	fSlow115 = powf((0.014705882352941176f * (float((fSlow107 >= 30.0f)) * min((fSlow107 + -30.0f), (float)68))),1.8f);
		float 	fSlow116 = powf((0.010101010101010102f * min((float)99, fSlow107)),0.69999999999999996f);
		float 	fSlow117 = max(0.001f, fabsf((((iSlow108)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow112))):max(0.0080000000000000002f, (318.0f * powf(fSlow112,12.6f)))) * (((iSlow108)?fSlow113:fSlow114) - ((iSlow108)?fSlow115:fSlow116)))));
		float 	fSlow118 = (fConst0 * fSlow117);
		float 	fSlow119 = float(fentry38);
		float 	fSlow120 = ((fSlow101 * fSlow119) * fSlow104);
		int 	iSlow121 = int((fSlow120 > fSlow107));
		float 	fSlow122 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry39) + fSlow111))));
		float 	fSlow123 = powf((0.014705882352941176f * (float((fSlow120 >= 30.0f)) * min((fSlow120 + -30.0f), (float)68))),1.8f);
		float 	fSlow124 = powf((0.010101010101010102f * min((float)99, fSlow120)),0.69999999999999996f);
		float 	fSlow125 = max(0.001f, fabsf((((iSlow121)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow122))):max(0.0080000000000000002f, (318.0f * powf(fSlow122,12.6f)))) * (((iSlow121)?fSlow115:fSlow116) - ((iSlow121)?fSlow123:fSlow124)))));
		float 	fSlow126 = float(fentry40);
		float 	fSlow127 = ((fSlow101 * fSlow126) * fSlow104);
		int 	iSlow128 = int((fSlow127 > fSlow120));
		float 	fSlow129 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry41) + fSlow111))));
		float 	fSlow130 = powf((0.014705882352941176f * (float((fSlow127 >= 30.0f)) * min((fSlow127 + -30.0f), (float)68))),1.8f);
		float 	fSlow131 = powf((0.010101010101010102f * min((float)99, fSlow127)),0.69999999999999996f);
		float 	fSlow132 = (fSlow125 + max(0.001f, fabsf((((iSlow128)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow129))):max(0.0080000000000000002f, (318.0f * powf(fSlow129,12.6f)))) * (((iSlow128)?fSlow130:fSlow131) - ((iSlow128)?fSlow123:fSlow124))))));
		int 	iSlow133 = int((fSlow105 > fSlow127));
		float 	fSlow134 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry42) + fSlow111))));
		float 	fSlow135 = (fSlow132 + max(0.001f, fabsf((((iSlow133)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow134))):max(0.0080000000000000002f, (318.0f * powf(fSlow134,12.6f)))) * (((iSlow133)?fSlow130:fSlow131) - ((iSlow133)?fSlow113:fSlow114))))));
		float 	fSlow136 = (fConst0 * fSlow135);
		float 	fSlow137 = (fConst0 * fSlow132);
		int 	iTemp9 = iTempPerm9;
		float 	fSlow138 = (fConst0 * fSlow125);
		int 	iTemp10 = iTempPerm10;
		float 	fSlow139 = (fSlow101 * fSlow104);
		float 	fSlow140 = (fConst1 * (0 - (fSlow139 * (fSlow106 - fSlow119))));
		float 	fSlow141 = (0 - (fSlow139 * (fSlow119 - fSlow126)));
		float 	fSlow142 = (0 - (fConst0 * (fSlow125 - fSlow132)));
		float 	fSlow143 = (fSlow102 - fSlow126);
		float 	fSlow144 = (0 - (fConst0 * (fSlow132 - fSlow135)));
		float 	fTemp11 = fTempPerm11;
		float 	fSlow145 = (fConst1 * fSlow110);
		float 	fTemp12 = fTempPerm12;
		float 	fSlow146 = float(fentry43);
		float 	fSlow147 = float(fentry44);
		float 	fSlow148 = float(fentry45);
		float 	fSlow149 = ((0.0012626262626262627f * (fSlow7 * fSlow148)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow148))));
		float 	fSlow150 = ((fSlow146 * fSlow147) * fSlow149);
		float 	fSlow151 = float(fentry46);
		float 	fSlow152 = ((fSlow146 * fSlow151) * fSlow149);
		int 	iSlow153 = int((fSlow150 <= fSlow152));
		float 	fSlow154 = float(fentry51);
		float 	fSlow155 = ((0.25f * float(fentry49)) + ((int(float(fentry50)))?fSlow154:(fSlow15 * fSlow154)));
		float 	fSlow156 = ((0.057142857142857148f * float(fentry48)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow155))) + 48.0f));
		float 	fSlow157 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry47) + fSlow156))));
		float 	fSlow158 = powf((0.014705882352941176f * (float((fSlow150 >= 30.0f)) * min((fSlow150 + -30.0f), (float)68))),1.8f);
		float 	fSlow159 = powf((0.010101010101010102f * min((float)99, fSlow150)),0.69999999999999996f);
		float 	fSlow160 = powf((0.014705882352941176f * (float((fSlow152 >= 30.0f)) * min((fSlow152 + -30.0f), (float)68))),1.8f);
		float 	fSlow161 = powf((0.010101010101010102f * min((float)99, fSlow152)),0.69999999999999996f);
		float 	fSlow162 = max(0.001f, fabsf((((iSlow153)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow157))):max(0.0080000000000000002f, (318.0f * powf(fSlow157,12.6f)))) * (((iSlow153)?fSlow158:fSlow159) - ((iSlow153)?fSlow160:fSlow161)))));
		float 	fSlow163 = (fConst0 * fSlow162);
		float 	fSlow164 = float(fentry52);
		float 	fSlow165 = ((fSlow146 * fSlow164) * fSlow149);
		int 	iSlow166 = int((fSlow165 > fSlow152));
		float 	fSlow167 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry53) + fSlow156))));
		float 	fSlow168 = powf((0.014705882352941176f * (float((fSlow165 >= 30.0f)) * min((fSlow165 + -30.0f), (float)68))),1.8f);
		float 	fSlow169 = powf((0.010101010101010102f * min((float)99, fSlow165)),0.69999999999999996f);
		float 	fSlow170 = max(0.001f, fabsf((((iSlow166)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow167))):max(0.0080000000000000002f, (318.0f * powf(fSlow167,12.6f)))) * (((iSlow166)?fSlow160:fSlow161) - ((iSlow166)?fSlow168:fSlow169)))));
		float 	fSlow171 = float(fentry54);
		float 	fSlow172 = ((fSlow146 * fSlow171) * fSlow149);
		int 	iSlow173 = int((fSlow172 > fSlow165));
		float 	fSlow174 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry55) + fSlow156))));
		float 	fSlow175 = powf((0.014705882352941176f * (float((fSlow172 >= 30.0f)) * min((fSlow172 + -30.0f), (float)68))),1.8f);
		float 	fSlow176 = powf((0.010101010101010102f * min((float)99, fSlow172)),0.69999999999999996f);
		float 	fSlow177 = (fSlow170 + max(0.001f, fabsf((((iSlow173)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow174))):max(0.0080000000000000002f, (318.0f * powf(fSlow174,12.6f)))) * (((iSlow173)?fSlow175:fSlow176) - ((iSlow173)?fSlow168:fSlow169))))));
		int 	iSlow178 = int((fSlow150 > fSlow172));
		float 	fSlow179 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry56) + fSlow156))));
		float 	fSlow180 = (fSlow177 + max(0.001f, fabsf((((iSlow178)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow179))):max(0.0080000000000000002f, (318.0f * powf(fSlow179,12.6f)))) * (((iSlow178)?fSlow175:fSlow176) - ((iSlow178)?fSlow158:fSlow159))))));
		float 	fSlow181 = (fConst0 * fSlow180);
		float 	fSlow182 = (fConst0 * fSlow177);
		int 	iTemp13 = iTempPerm13;
		float 	fSlow183 = (fConst0 * fSlow170);
		int 	iTemp14 = iTempPerm14;
		float 	fSlow184 = (fSlow146 * fSlow149);
		float 	fSlow185 = (fConst1 * (0 - (fSlow184 * (fSlow151 - fSlow164))));
		float 	fSlow186 = (0 - (fSlow184 * (fSlow164 - fSlow171)));
		float 	fSlow187 = (0 - (fConst0 * (fSlow170 - fSlow177)));
		float 	fSlow188 = (fSlow147 - fSlow171);
		float 	fSlow189 = (0 - (fConst0 * (fSlow177 - fSlow180)));
		float 	fTemp15 = fTempPerm15;
		float 	fSlow190 = (fConst1 * fSlow155);
		float 	fTemp16 = fTempPerm16;
		float 	fSlow191 = float(fentry57);
		float 	fSlow192 = float(fentry58);
		float 	fSlow193 = float(fentry59);
		float 	fSlow194 = ((0.0012626262626262627f * (fSlow7 * fSlow193)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow193))));
		float 	fSlow195 = ((fSlow191 * fSlow192) * fSlow194);
		float 	fSlow196 = float(fentry60);
		float 	fSlow197 = ((fSlow191 * fSlow196) * fSlow194);
		int 	iSlow198 = int((fSlow195 <= fSlow197));
		float 	fSlow199 = float(fentry65);
		float 	fSlow200 = ((0.25f * float(fentry63)) + ((int(float(fentry64)))?fSlow199:(fSlow15 * fSlow199)));
		float 	fSlow201 = ((0.057142857142857148f * float(fentry62)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow200))) + 48.0f));
		float 	fSlow202 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry61) + fSlow201))));
		float 	fSlow203 = powf((0.014705882352941176f * (float((fSlow195 >= 30.0f)) * min((fSlow195 + -30.0f), (float)68))),1.8f);
		float 	fSlow204 = powf((0.010101010101010102f * min((float)99, fSlow195)),0.69999999999999996f);
		float 	fSlow205 = powf((0.014705882352941176f * (float((fSlow197 >= 30.0f)) * min((fSlow197 + -30.0f), (float)68))),1.8f);
		float 	fSlow206 = powf((0.010101010101010102f * min((float)99, fSlow197)),0.69999999999999996f);
		float 	fSlow207 = max(0.001f, fabsf((((iSlow198)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow202))):max(0.0080000000000000002f, (318.0f * powf(fSlow202,12.6f)))) * (((iSlow198)?fSlow203:fSlow204) - ((iSlow198)?fSlow205:fSlow206)))));
		float 	fSlow208 = (fConst0 * fSlow207);
		float 	fSlow209 = float(fentry66);
		float 	fSlow210 = ((fSlow191 * fSlow209) * fSlow194);
		int 	iSlow211 = int((fSlow210 > fSlow197));
		float 	fSlow212 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry67) + fSlow201))));
		float 	fSlow213 = powf((0.014705882352941176f * (float((fSlow210 >= 30.0f)) * min((fSlow210 + -30.0f), (float)68))),1.8f);
		float 	fSlow214 = powf((0.010101010101010102f * min((float)99, fSlow210)),0.69999999999999996f);
		float 	fSlow215 = max(0.001f, fabsf((((iSlow211)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow212))):max(0.0080000000000000002f, (318.0f * powf(fSlow212,12.6f)))) * (((iSlow211)?fSlow205:fSlow206) - ((iSlow211)?fSlow213:fSlow214)))));
		float 	fSlow216 = float(fentry68);
		float 	fSlow217 = ((fSlow191 * fSlow216) * fSlow194);
		int 	iSlow218 = int((fSlow217 > fSlow210));
		float 	fSlow219 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry69) + fSlow201))));
		float 	fSlow220 = powf((0.014705882352941176f * (float((fSlow217 >= 30.0f)) * min((fSlow217 + -30.0f), (float)68))),1.8f);
		float 	fSlow221 = powf((0.010101010101010102f * min((float)99, fSlow217)),0.69999999999999996f);
		float 	fSlow222 = (fSlow215 + max(0.001f, fabsf((((iSlow218)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow219))):max(0.0080000000000000002f, (318.0f * powf(fSlow219,12.6f)))) * (((iSlow218)?fSlow220:fSlow221) - ((iSlow218)?fSlow213:fSlow214))))));
		int 	iSlow223 = int((fSlow195 > fSlow217));
		float 	fSlow224 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry70) + fSlow201))));
		float 	fSlow225 = (fSlow222 + max(0.001f, fabsf((((iSlow223)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow224))):max(0.0080000000000000002f, (318.0f * powf(fSlow224,12.6f)))) * (((iSlow223)?fSlow220:fSlow221) - ((iSlow223)?fSlow203:fSlow204))))));
		float 	fSlow226 = (fConst0 * fSlow225);
		float 	fSlow227 = (fConst0 * fSlow222);
		int 	iTemp17 = iTempPerm17;
		float 	fSlow228 = (fConst0 * fSlow215);
		int 	iTemp18 = iTempPerm18;
		float 	fSlow229 = (fSlow191 * fSlow194);
		float 	fSlow230 = (fConst1 * (0 - (fSlow229 * (fSlow196 - fSlow209))));
		float 	fSlow231 = (0 - (fSlow229 * (fSlow209 - fSlow216)));
		float 	fSlow232 = (0 - (fConst0 * (fSlow215 - fSlow222)));
		float 	fSlow233 = (fSlow192 - fSlow216);
		float 	fSlow234 = (0 - (fConst0 * (fSlow222 - fSlow225)));
		float 	fTemp19 = fTempPerm19;
		float 	fSlow235 = (fConst1 * fSlow200);
		float 	fTemp20 = fTempPerm20;
		float 	fSlow236 = float(fentry71);
		float 	fSlow237 = float(fentry72);
		float 	fSlow238 = float(fentry73);
		float 	fSlow239 = ((0.0012626262626262627f * (fSlow7 * fSlow238)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow238))));
		float 	fSlow240 = ((fSlow236 * fSlow237) * fSlow239);
		float 	fSlow241 = float(fentry74);
		float 	fSlow242 = ((fSlow236 * fSlow241) * fSlow239);
		int 	iSlow243 = int((fSlow240 <= fSlow242));
		float 	fSlow244 = float(fentry79);
		float 	fSlow245 = ((0.25f * float(fentry77)) + ((int(float(fentry78)))?fSlow244:(fSlow15 * fSlow244)));
		float 	fSlow246 = ((0.057142857142857148f * float(fentry76)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow245))) + 48.0f));
		float 	fSlow247 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry75) + fSlow246))));
		float 	fSlow248 = powf((0.014705882352941176f * (float((fSlow242 >= 30.0f)) * min((fSlow242 + -30.0f), (float)68))),1.8f);
		float 	fSlow249 = powf((0.010101010101010102f * min((float)99, fSlow242)),0.69999999999999996f);
		float 	fSlow250 = powf((0.014705882352941176f * (float((fSlow240 >= 30.0f)) * min((fSlow240 + -30.0f), (float)68))),1.8f);
		float 	fSlow251 = powf((0.010101010101010102f * min((float)99, fSlow240)),0.69999999999999996f);
		float 	fSlow252 = max(0.001f, fabsf((0 - (((iSlow243)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow247))):max(0.0080000000000000002f, (318.0f * powf(fSlow247,12.6f)))) * (((iSlow243)?fSlow248:fSlow249) - ((iSlow243)?fSlow250:fSlow251))))));
		float 	fSlow253 = (fConst0 * fSlow252);
		float 	fSlow254 = float(fentry80);
		float 	fSlow255 = ((fSlow236 * fSlow254) * fSlow239);
		int 	iSlow256 = int((fSlow255 > fSlow242));
		float 	fSlow257 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry81) + fSlow246))));
		float 	fSlow258 = powf((0.014705882352941176f * (float((fSlow255 >= 30.0f)) * min((fSlow255 + -30.0f), (float)68))),1.8f);
		float 	fSlow259 = powf((0.010101010101010102f * min((float)99, fSlow255)),0.69999999999999996f);
		float 	fSlow260 = max(0.001f, fabsf((((iSlow256)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow257))):max(0.0080000000000000002f, (318.0f * powf(fSlow257,12.6f)))) * (((iSlow256)?fSlow248:fSlow249) - ((iSlow256)?fSlow258:fSlow259)))));
		float 	fSlow261 = float(fentry82);
		float 	fSlow262 = ((fSlow236 * fSlow261) * fSlow239);
		int 	iSlow263 = int((fSlow262 > fSlow255));
		float 	fSlow264 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry83) + fSlow246))));
		float 	fSlow265 = powf((0.014705882352941176f * (float((fSlow262 >= 30.0f)) * min((fSlow262 + -30.0f), (float)68))),1.8f);
		float 	fSlow266 = powf((0.010101010101010102f * min((float)99, fSlow262)),0.69999999999999996f);
		float 	fSlow267 = (fSlow260 + max(0.001f, fabsf((((iSlow263)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow264))):max(0.0080000000000000002f, (318.0f * powf(fSlow264,12.6f)))) * (((iSlow263)?fSlow265:fSlow266) - ((iSlow263)?fSlow258:fSlow259))))));
		int 	iSlow268 = int((fSlow240 > fSlow262));
		float 	fSlow269 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry84) + fSlow246))));
		float 	fSlow270 = (fSlow267 + max(0.001f, fabsf((((iSlow268)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow269))):max(0.0080000000000000002f, (318.0f * powf(fSlow269,12.6f)))) * (((iSlow268)?fSlow265:fSlow266) - ((iSlow268)?fSlow250:fSlow251))))));
		float 	fSlow271 = (fConst0 * fSlow270);
		float 	fSlow272 = (fConst0 * fSlow267);
		int 	iTemp21 = iTempPerm21;
		float 	fSlow273 = (fConst0 * fSlow260);
		int 	iTemp22 = iTempPerm22;
		float 	fSlow274 = (fSlow236 * fSlow239);
		float 	fSlow275 = (fConst1 * (0 - (fSlow274 * (fSlow241 - fSlow254))));
		float 	fSlow276 = (fSlow261 - fSlow254);
		float 	fSlow277 = (0 - (fConst0 * (fSlow260 - fSlow267)));
		float 	fSlow278 = (0 - (fSlow274 * (fSlow261 - fSlow237)));
		float 	fSlow279 = (0 - (fConst0 * (fSlow267 - fSlow270)));
		float 	fTemp23 = fTempPerm23;
		float 	fSlow280 = (fConst1 * fSlow245);
		float 	fTemp24 = fTempPerm24;
		float 	fTemp25 = fTempPerm25;
		float 	fSlow281 = (1825.3162113323297f * float(fentry85));
		float 	fTemp26 = fTempPerm26;
		float 	fTemp27 = fTempPerm27;
		//zone2b
		//zone3
		FAUSTFLOAT* output0 = output[0];
		//LoopGraphScalar
		for (int i=0; i<count; i++) {
			if ((iSlow25 || iSlow1)) {
				fRec0[0] = ((iSlow4)?0.0f:min(fSlow24, (fRec0[1] + 1.0f)));
				fRec2[0] = ((iSlow28)?0.0f:min(fSlow46, (fRec2[1] + 1.0f)));
				iTemp0 = int((fRec2[0] < fSlow47));
				iTemp1 = int((fRec2[0] < fSlow48));
				fRec1[0] = ((iSlow26)?(fSlow27 * ((iTemp0)?((iTemp1)?((int((fRec2[0] < 0.0f)))?fSlow12:((iTemp1)?(fSlow49 * (fSlow11 + (fSlow50 * (fRec2[0] / fSlow35)))):fSlow30)):((iTemp0)?(fSlow30 + (fSlow51 * ((fRec2[0] - fSlow48) / fSlow52))):fSlow37)):((int((fRec2[0] < fSlow46)))?(fSlow37 + (fSlow53 * ((fRec2[0] - fSlow47) / fSlow54))):fSlow10))):fRec1[1]);
				fTemp2 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec0[0] < 0.0f)))?fRec1[0]:((int((fRec0[0] < fSlow24)))?(fRec1[0] + (fConst1 * ((fRec0[0] * (fSlow12 - fRec1[0])) / fSlow23))):fSlow12)))));
				fTemp3 = (fRec4[1] + fSlow55);
				fRec4[0] = (fTemp3 - floorf(fTemp3));
				fRec5[0] = ((iSlow4)?0.0f:min(fSlow73, (fRec5[1] + 1.0f)));
				fRec7[0] = ((iSlow28)?0.0f:min(fSlow91, (fRec7[1] + 1.0f)));
				iTemp4 = int((fRec7[0] < fSlow92));
				iTemp5 = int((fRec7[0] < fSlow93));
				fRec6[0] = ((iSlow26)?(fSlow27 * ((iTemp4)?((iTemp5)?((int((fRec7[0] < 0.0f)))?fSlow62:((iTemp5)?(fSlow94 * (fSlow61 + (fSlow95 * (fRec7[0] / fSlow80)))):fSlow75)):((iTemp4)?(fSlow75 + (fSlow96 * ((fRec7[0] - fSlow93) / fSlow97))):fSlow82)):((int((fRec7[0] < fSlow91)))?(fSlow82 + (fSlow98 * ((fRec7[0] - fSlow92) / fSlow99))):fSlow60))):fRec6[1]);
				fTemp6 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec5[0] < 0.0f)))?fRec6[0]:((int((fRec5[0] < fSlow73)))?(fRec6[0] + (fConst1 * ((fRec5[0] * (fSlow62 - fRec6[0])) / fSlow72))):fSlow62)))));
				fTemp7 = (fRec8[1] + fSlow100);
				fRec8[0] = (fTemp7 - floorf(fTemp7));
				fTemp8 = (65536.0f * fRec8[0]);
				fRec9[0] = ((iSlow4)?0.0f:min(fSlow118, (fRec9[1] + 1.0f)));
				fRec11[0] = ((iSlow28)?0.0f:min(fSlow136, (fRec11[1] + 1.0f)));
				iTemp9 = int((fRec11[0] < fSlow137));
				iTemp10 = int((fRec11[0] < fSlow138));
				fRec10[0] = ((iSlow26)?(fSlow27 * ((iTemp9)?((iTemp10)?((int((fRec11[0] < 0.0f)))?fSlow107:((iTemp10)?(fSlow107 + (fSlow140 * (fRec11[0] / fSlow125))):fSlow120)):((iTemp9)?(fSlow120 + (fSlow141 * ((fRec11[0] - fSlow138) / fSlow142))):fSlow127)):((int((fRec11[0] < fSlow136)))?(fSlow139 * (fSlow126 + (fSlow143 * ((fRec11[0] - fSlow137) / fSlow144)))):fSlow105))):fRec10[1]);
				fTemp11 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec9[0] < 0.0f)))?fRec10[0]:((int((fRec9[0] < fSlow118)))?(fRec10[0] + (fConst1 * ((fRec9[0] * (fSlow107 - fRec10[0])) / fSlow117))):fSlow107)))));
				fTemp12 = (fRec12[1] + fSlow145);
				fRec12[0] = (fTemp12 - floorf(fTemp12));
				fRec13[0] = ((iSlow4)?0.0f:min(fSlow163, (fRec13[1] + 1.0f)));
				fRec15[0] = ((iSlow28)?0.0f:min(fSlow181, (fRec15[1] + 1.0f)));
				iTemp13 = int((fRec15[0] < fSlow182));
				iTemp14 = int((fRec15[0] < fSlow183));
				fRec14[0] = ((iSlow26)?(fSlow27 * ((iTemp13)?((iTemp14)?((int((fRec15[0] < 0.0f)))?fSlow152:((iTemp14)?(fSlow152 + (fSlow185 * (fRec15[0] / fSlow170))):fSlow165)):((iTemp13)?(fSlow165 + (fSlow186 * ((fRec15[0] - fSlow183) / fSlow187))):fSlow172)):((int((fRec15[0] < fSlow181)))?(fSlow184 * (fSlow171 + (fSlow188 * ((fRec15[0] - fSlow182) / fSlow189)))):fSlow150))):fRec14[1]);
				fTemp15 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec13[0] < 0.0f)))?fRec14[0]:((int((fRec13[0] < fSlow163)))?(fRec14[0] + (fConst1 * ((fRec13[0] * (fSlow152 - fRec14[0])) / fSlow162))):fSlow152)))));
				fTemp16 = (fRec16[1] + fSlow190);
				fRec16[0] = (fTemp16 - floorf(fTemp16));
				fRec17[0] = ((iSlow4)?0.0f:min(fSlow208, (fRec17[1] + 1.0f)));
				fRec19[0] = ((iSlow28)?0.0f:min(fSlow226, (fRec19[1] + 1.0f)));
				iTemp17 = int((fRec19[0] < fSlow227));
				iTemp18 = int((fRec19[0] < fSlow228));
				fRec18[0] = ((iSlow26)?(fSlow27 * ((iTemp17)?((iTemp18)?((int((fRec19[0] < 0.0f)))?fSlow197:((iTemp18)?(fSlow197 + (fSlow230 * (fRec19[0] / fSlow215))):fSlow210)):((iTemp17)?(fSlow210 + (fSlow231 * ((fRec19[0] - fSlow228) / fSlow232))):fSlow217)):((int((fRec19[0] < fSlow226)))?(fSlow229 * (fSlow216 + (fSlow233 * ((fRec19[0] - fSlow227) / fSlow234)))):fSlow195))):fRec18[1]);
				fTemp19 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec17[0] < 0.0f)))?fRec18[0]:((int((fRec17[0] < fSlow208)))?(fRec18[0] + (fConst1 * ((fRec17[0] * (fSlow197 - fRec18[0])) / fSlow207))):fSlow197)))));
				fTemp20 = (fRec20[1] + fSlow235);
				fRec20[0] = (fTemp20 - floorf(fTemp20));
				fRec22[0] = ((iSlow4)?0.0f:min(fSlow253, (fRec22[1] + 1.0f)));
				fRec24[0] = ((iSlow28)?0.0f:min(fSlow271, (fRec24[1] + 1.0f)));
				iTemp21 = int((fRec24[0] < fSlow272));
				iTemp22 = int((fRec24[0] < fSlow273));
				fRec23[0] = ((iSlow26)?(fSlow27 * ((iTemp21)?((iTemp22)?((int((fRec24[0] < 0.0f)))?fSlow242:((iTemp22)?(fSlow242 + (fSlow275 * (fRec24[0] / fSlow260))):fSlow255)):((iTemp21)?(fSlow274 * (fSlow254 + (fSlow276 * ((fRec24[0] - fSlow273) / fSlow277)))):fSlow262)):((int((fRec24[0] < fSlow271)))?(fSlow262 + (fSlow278 * ((fRec24[0] - fSlow272) / fSlow279))):fSlow240))):fRec23[1]);
				fTemp23 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec22[0] < 0.0f)))?fRec23[0]:((int((fRec22[0] < fSlow253)))?(fRec23[0] + (fConst1 * ((fRec22[0] * (fSlow242 - fRec23[0])) / fSlow252))):fSlow242)))));
				fTemp24 = (fRec25[1] + fSlow280);
				fRec25[0] = (fTemp24 - floorf(fTemp24));
				fTemp25 = (65536.0f * fRec25[0]);
			}
			if (iSlow1) {
				fRec21[0] = (fTemp23 * ftbl0[(((int((fTemp25 + (fSlow281 * fRec21[1]))) % 65536) + 65536) % 65536)]);
				fTemp26 = (2.0879500000000002f * ((fTemp2 * ftbl0[(((int((65536.0f * (fRec4[0] + (fTemp6 * ftbl0[(((int(fTemp8) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)]) + (fTemp11 * ftbl0[(((int((65536.0f * (fRec12[0] + (fTemp15 * ftbl0[(((int((65536.0f * (fRec16[0] + (fTemp19 * ftbl0[(((int((65536.0f * (fRec20[0] + fRec21[0]))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])));
			}
			if (iSlow25) {
				fRec26[0] = (fTemp6 * ftbl0[(((int((fTemp8 + (fSlow281 * fRec26[1]))) % 65536) + 65536) % 65536)]);
				fTemp27 = (2.0879500000000002f * ((fTemp2 * ftbl0[(((int((65536.0f * (fRec4[0] + fRec26[0]))) % 65536) + 65536) % 65536)]) + (fTemp11 * ftbl0[(((int((65536.0f * (fRec12[0] + (fTemp15 * ftbl0[(((int((65536.0f * (fRec16[0] + (fTemp19 * ftbl0[(((int((65536.0f * (fRec20[0] + (fTemp23 * ftbl0[(((int(fTemp25) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])));
			}
			output0[i] = (FAUSTFLOAT)(fTemp26 + fTemp27);
			// post processing
			if (iSlow25) {
				fRec26[1] = fRec26[0];
			}
			if (iSlow1) {
				fRec21[1] = fRec21[0];
			}
			if ((iSlow25 || iSlow1)) {
				fRec25[1] = fRec25[0];
				fRec23[1] = fRec23[0];
				fRec24[1] = fRec24[0];
				fRec22[1] = fRec22[0];
				fRec20[1] = fRec20[0];
				fRec18[1] = fRec18[0];
				fRec19[1] = fRec19[0];
				fRec17[1] = fRec17[0];
				fRec16[1] = fRec16[0];
				fRec14[1] = fRec14[0];
				fRec15[1] = fRec15[0];
				fRec13[1] = fRec13[0];
				fRec12[1] = fRec12[0];
				fRec10[1] = fRec10[0];
				fRec11[1] = fRec11[0];
				fRec9[1] = fRec9[0];
				fRec8[1] = fRec8[0];
				fRec6[1] = fRec6[0];
				fRec7[1] = fRec7[0];
				fRec5[1] = fRec5[0];
				fRec4[1] = fRec4[0];
				fRec1[1] = fRec1[0];
				fRec2[1] = fRec2[0];
				fRec0[1] = fRec0[0];
			}
		}
		iTempPerm0 = iTemp0;
		iTempPerm1 = iTemp1;
		fTempPerm2 = fTemp2;
		fTempPerm3 = fTemp3;
		iTempPerm4 = iTemp4;
		iTempPerm5 = iTemp5;
		fTempPerm6 = fTemp6;
		fTempPerm7 = fTemp7;
		fTempPerm8 = fTemp8;
		iTempPerm9 = iTemp9;
		iTempPerm10 = iTemp10;
		fTempPerm11 = fTemp11;
		fTempPerm12 = fTemp12;
		iTempPerm13 = iTemp13;
		iTempPerm14 = iTemp14;
		fTempPerm15 = fTemp15;
		fTempPerm16 = fTemp16;
		iTempPerm17 = iTemp17;
		iTempPerm18 = iTemp18;
		fTempPerm19 = fTemp19;
		fTempPerm20 = fTemp20;
		iTempPerm21 = iTemp21;
		iTempPerm22 = iTemp22;
		fTempPerm23 = fTemp23;
		fTempPerm24 = fTemp24;
		fTempPerm25 = fTemp25;
		fTempPerm26 = fTemp26;
		fTempPerm27 = fTemp27;
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
