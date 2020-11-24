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
	float 	fTempPerm9;
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
	int 	iTempPerm10;
	int 	iTempPerm11;
	float 	fRec10[2];
	float 	fTempPerm12;
	float 	fTempPerm13;
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
	int 	iTempPerm14;
	int 	iTempPerm15;
	float 	fRec14[2];
	float 	fTempPerm16;
	float 	fTempPerm17;
	float 	fRec16[2];
	float 	fTempPerm18;
	float 	fTempPerm19;
	float 	fTempPerm20;
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
	int 	iTempPerm21;
	int 	iTempPerm22;
	float 	fRec18[2];
	float 	fTempPerm23;
	float 	fTempPerm24;
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
	int 	iTempPerm25;
	int 	iTempPerm26;
	float 	fRec23[2];
	float 	fTempPerm27;
	float 	fTempPerm28;
	float 	fRec25[2];
	float 	fTempPerm29;
	FAUSTFLOAT 	fentry85;
	float 	fRec21[2];
	float 	fTempPerm30;
	float 	fRec26[2];
	float 	fTempPerm31;
	float 	fTempPerm32;
	float 	fRec27[2];
	float 	fTempPerm33;
	float 	fTempPerm34;
	float 	fRec28[2];
	float 	fTempPerm35;
	float 	fTempPerm36;
	float 	fRec29[2];
	float 	fTempPerm37;
	float 	fRec30[2];
	float 	fTempPerm38;
	float 	fTempPerm39;
	float 	fRec31[2];
	float 	fTempPerm40;
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
		m->declare("spn.lib/name", "Standart Pitch Notation constants");
		m->declare("spn.lib/version", "0.1");
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
		fTempPerm9 = 0;
		iTempPerm10 = 0;
		iTempPerm11 = 0;
		fTempPerm12 = 0;
		fTempPerm13 = 0;
		iTempPerm14 = 0;
		iTempPerm15 = 0;
		fTempPerm16 = 0;
		fTempPerm17 = 0;
		fTempPerm18 = 0;
		fTempPerm19 = 0;
		fTempPerm20 = 0;
		iTempPerm21 = 0;
		iTempPerm22 = 0;
		fTempPerm23 = 0;
		fTempPerm24 = 0;
		iTempPerm25 = 0;
		iTempPerm26 = 0;
		fTempPerm27 = 0;
		fTempPerm28 = 0;
		fTempPerm29 = 0;
		fTempPerm30 = 0;
		fTempPerm31 = 0;
		fTempPerm32 = 0;
		fTempPerm33 = 0;
		fTempPerm34 = 0;
		fTempPerm35 = 0;
		fTempPerm36 = 0;
		fTempPerm37 = 0;
		fTempPerm38 = 0;
		fTempPerm39 = 0;
		fTempPerm40 = 0;
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
		fslider1 = 48.0f;
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
		for (int i=0; i<2; i++) fRec27[i] = 0;
		for (int i=0; i<2; i++) fRec28[i] = 0;
		for (int i=0; i<2; i++) fRec29[i] = 0;
		for (int i=0; i<2; i++) fRec30[i] = 0;
		for (int i=0; i<2; i++) fRec31[i] = 0;
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
		ui_interface->declare(&fentry0, "type", "int");
		ui_interface->addNumEntry("algorithm", &fentry0, 0.0f, 0.0f, 7.0f, 1.0f);
		ui_interface->addNumEntry("feedback", &fentry85, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addHorizontalSlider("gain", &fslider0, 0.80000000000000004f, 0.0f, 1.0f, 0.01f);
		ui_interface->addButton("gate", &fbutton0);
		ui_interface->addNumEntry("op0:detune", &fentry7, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL1", &fentry10, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL2", &fentry12, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL3", &fentry2, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL4", &fentry4, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR1", &fentry11, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR2", &fentry13, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR3", &fentry14, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR4", &fentry5, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry9, "unit", "Hz");
		ui_interface->addNumEntry("op0:freq", &fentry9, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op0:key_vel_sens", &fentry3, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op0:level", &fentry1, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:mode", &fentry8, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op0:rate_scale", &fentry6, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op1:detune", &fentry21, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL1", &fentry24, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL2", &fentry26, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL3", &fentry16, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL4", &fentry18, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR1", &fentry25, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR2", &fentry27, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR3", &fentry28, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR4", &fentry19, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry23, "unit", "Hz");
		ui_interface->addNumEntry("op1:freq", &fentry23, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op1:key_vel_sens", &fentry17, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op1:level", &fentry15, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:mode", &fentry22, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op1:rate_scale", &fentry20, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op2:detune", &fentry35, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL1", &fentry38, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL2", &fentry40, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL3", &fentry30, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL4", &fentry32, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR1", &fentry39, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR2", &fentry41, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR3", &fentry42, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR4", &fentry33, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry37, "unit", "Hz");
		ui_interface->addNumEntry("op2:freq", &fentry37, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op2:key_vel_sens", &fentry31, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op2:level", &fentry29, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:mode", &fentry36, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op2:rate_scale", &fentry34, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op3:detune", &fentry49, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL1", &fentry52, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL2", &fentry54, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL3", &fentry44, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL4", &fentry46, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR1", &fentry53, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR2", &fentry55, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR3", &fentry56, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR4", &fentry47, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry51, "unit", "Hz");
		ui_interface->addNumEntry("op3:freq", &fentry51, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op3:key_vel_sens", &fentry45, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op3:level", &fentry43, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:mode", &fentry50, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op3:rate_scale", &fentry48, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op4:detune", &fentry63, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL1", &fentry66, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL2", &fentry68, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL3", &fentry58, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL4", &fentry60, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR1", &fentry67, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR2", &fentry69, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR3", &fentry70, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR4", &fentry61, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry65, "unit", "Hz");
		ui_interface->addNumEntry("op4:freq", &fentry65, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op4:key_vel_sens", &fentry59, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op4:level", &fentry57, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:mode", &fentry64, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op4:rate_scale", &fentry62, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op5:detune", &fentry77, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL1", &fentry80, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL2", &fentry82, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL3", &fentry72, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL4", &fentry74, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR1", &fentry81, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR2", &fentry83, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR3", &fentry84, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR4", &fentry75, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry79, "unit", "Hz");
		ui_interface->addNumEntry("op5:freq", &fentry79, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op5:key_vel_sens", &fentry73, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op5:level", &fentry71, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:mode", &fentry78, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op5:rate_scale", &fentry76, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addHorizontalSlider("pitch", &fslider1, 48.0f, 24.0f, 84.0f, 0.001f);
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		//zone1
		//zone2
		int 	iSlow0 = int(float(fentry0));
		int 	iSlow1 = (float((iSlow0 == 4)) != 0.0f);
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
		float 	fSlow15 = powf(2.0f,(0.083333333333333329f * (float(fslider1) + -69.0f)));
		float 	fSlow16 = ((0.25f * float(fentry7)) + ((int(float(fentry8)))?fSlow14:(440.0f * (fSlow15 * fSlow14))));
		float 	fSlow17 = ((0.057142857142857148f * float(fentry6)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow16))) + 48.0f));
		float 	fSlow18 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry5) + fSlow17))));
		float 	fSlow19 = powf((0.014705882352941176f * (float((fSlow10 >= 30.0f)) * min((fSlow10 + -30.0f), (float)68))),1.8f);
		float 	fSlow20 = powf((0.010101010101010102f * min((float)99, fSlow10)),0.69999999999999996f);
		float 	fSlow21 = powf((0.014705882352941176f * (float((fSlow12 >= 30.0f)) * min((fSlow12 + -30.0f), (float)68))),1.8f);
		float 	fSlow22 = powf((0.010101010101010102f * min((float)99, fSlow12)),0.69999999999999996f);
		float 	fSlow23 = max(0.001f, fabsf((((iSlow13)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow18))):max(0.0080000000000000002f, (318.0f * powf(fSlow18,12.6f)))) * (((iSlow13)?fSlow19:fSlow20) - ((iSlow13)?fSlow21:fSlow22)))));
		float 	fSlow24 = (fConst0 * fSlow23);
		int 	iSlow25 = (float((iSlow0 == 3)) != 0.0f);
		int 	iSlow26 = (float((iSlow0 == 1)) != 0.0f);
		int 	iSlow27 = (float((iSlow0 == 0)) != 0.0f);
		int 	iSlow28 = (float((iSlow0 == 5)) != 0.0f);
		int 	iSlow29 = (float((iSlow0 == 2)) != 0.0f);
		int 	iSlow30 = (float((iSlow0 == 6)) != 0.0f);
		int 	iSlow31 = (float((iSlow0 == 7)) != 0.0f);
		int 	iSlow32 = int(iSlow3);
		float 	fSlow33 = float(iSlow3);
		int 	iSlow34 = int(((fSlow2 == 0.0f) > 0));
		float 	fSlow35 = float(fentry10);
		float 	fSlow36 = ((fSlow5 * fSlow35) * fSlow9);
		int 	iSlow37 = int((fSlow36 > fSlow12));
		float 	fSlow38 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry11) + fSlow17))));
		float 	fSlow39 = powf((0.014705882352941176f * (float((fSlow36 >= 30.0f)) * min((fSlow36 + -30.0f), (float)68))),1.8f);
		float 	fSlow40 = powf((0.010101010101010102f * min((float)99, fSlow36)),0.69999999999999996f);
		float 	fSlow41 = max(0.001f, fabsf((((iSlow37)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow38))):max(0.0080000000000000002f, (318.0f * powf(fSlow38,12.6f)))) * (((iSlow37)?fSlow21:fSlow22) - ((iSlow37)?fSlow39:fSlow40)))));
		float 	fSlow42 = float(fentry12);
		float 	fSlow43 = ((fSlow5 * fSlow42) * fSlow9);
		int 	iSlow44 = int((fSlow43 > fSlow36));
		float 	fSlow45 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry13) + fSlow17))));
		float 	fSlow46 = powf((0.014705882352941176f * (float((fSlow43 >= 30.0f)) * min((fSlow43 + -30.0f), (float)68))),1.8f);
		float 	fSlow47 = powf((0.010101010101010102f * min((float)99, fSlow43)),0.69999999999999996f);
		float 	fSlow48 = (fSlow41 + max(0.001f, fabsf((((iSlow44)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow45))):max(0.0080000000000000002f, (318.0f * powf(fSlow45,12.6f)))) * (((iSlow44)?fSlow46:fSlow47) - ((iSlow44)?fSlow39:fSlow40))))));
		int 	iSlow49 = int((fSlow10 > fSlow43));
		float 	fSlow50 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry14) + fSlow17))));
		float 	fSlow51 = (fSlow48 + max(0.001f, fabsf((((iSlow49)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow50))):max(0.0080000000000000002f, (318.0f * powf(fSlow50,12.6f)))) * (((iSlow49)?fSlow46:fSlow47) - ((iSlow49)?fSlow19:fSlow20))))));
		float 	fSlow52 = (fConst0 * fSlow51);
		float 	fSlow53 = (fConst0 * fSlow48);
		int 	iTemp0 = iTempPerm0;
		float 	fSlow54 = (fConst0 * fSlow41);
		int 	iTemp1 = iTempPerm1;
		float 	fSlow55 = (fSlow5 * fSlow9);
		float 	fSlow56 = (fConst1 * (0 - (fSlow55 * (fSlow11 - fSlow35))));
		float 	fSlow57 = (fConst1 * (fSlow42 - fSlow35));
		float 	fSlow58 = (fSlow48 - fSlow41);
		float 	fSlow59 = (0 - (fSlow55 * (fSlow42 - fSlow6)));
		float 	fSlow60 = (0 - (fConst0 * (fSlow48 - fSlow51)));
		float 	fTemp2 = fTempPerm2;
		float 	fSlow61 = (fConst1 * fSlow16);
		float 	fTemp3 = fTempPerm3;
		float 	fSlow62 = float(fentry15);
		float 	fSlow63 = float(fentry16);
		float 	fSlow64 = float(fentry17);
		float 	fSlow65 = ((0.0012626262626262627f * (fSlow64 * fSlow7)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow64))));
		float 	fSlow66 = ((fSlow62 * fSlow63) * fSlow65);
		float 	fSlow67 = float(fentry18);
		float 	fSlow68 = ((fSlow62 * fSlow67) * fSlow65);
		int 	iSlow69 = int((fSlow66 <= fSlow68));
		float 	fSlow70 = float(fentry23);
		float 	fSlow71 = ((0.25f * float(fentry21)) + ((int(float(fentry22)))?fSlow70:(440.0f * (fSlow70 * fSlow15))));
		float 	fSlow72 = ((0.057142857142857148f * float(fentry20)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow71))) + 48.0f));
		float 	fSlow73 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry19) + fSlow72))));
		float 	fSlow74 = powf((0.014705882352941176f * (float((fSlow66 >= 30.0f)) * min((fSlow66 + -30.0f), (float)68))),1.8f);
		float 	fSlow75 = powf((0.010101010101010102f * min((float)99, fSlow66)),0.69999999999999996f);
		float 	fSlow76 = powf((0.014705882352941176f * (float((fSlow68 >= 30.0f)) * min((fSlow68 + -30.0f), (float)68))),1.8f);
		float 	fSlow77 = powf((0.010101010101010102f * min((float)99, fSlow68)),0.69999999999999996f);
		float 	fSlow78 = max(0.001f, fabsf((((iSlow69)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow73))):max(0.0080000000000000002f, (318.0f * powf(fSlow73,12.6f)))) * (((iSlow69)?fSlow74:fSlow75) - ((iSlow69)?fSlow76:fSlow77)))));
		float 	fSlow79 = (fConst0 * fSlow78);
		float 	fSlow80 = float(fentry24);
		float 	fSlow81 = ((fSlow62 * fSlow80) * fSlow65);
		int 	iSlow82 = int((fSlow81 > fSlow68));
		float 	fSlow83 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry25) + fSlow72))));
		float 	fSlow84 = powf((0.014705882352941176f * (float((fSlow81 >= 30.0f)) * min((fSlow81 + -30.0f), (float)68))),1.8f);
		float 	fSlow85 = powf((0.010101010101010102f * min((float)99, fSlow81)),0.69999999999999996f);
		float 	fSlow86 = max(0.001f, fabsf((((iSlow82)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow83))):max(0.0080000000000000002f, (318.0f * powf(fSlow83,12.6f)))) * (((iSlow82)?fSlow76:fSlow77) - ((iSlow82)?fSlow84:fSlow85)))));
		float 	fSlow87 = float(fentry26);
		float 	fSlow88 = ((fSlow62 * fSlow87) * fSlow65);
		int 	iSlow89 = int((fSlow88 > fSlow81));
		float 	fSlow90 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry27) + fSlow72))));
		float 	fSlow91 = powf((0.014705882352941176f * (float((fSlow88 >= 30.0f)) * min((fSlow88 + -30.0f), (float)68))),1.8f);
		float 	fSlow92 = powf((0.010101010101010102f * min((float)99, fSlow88)),0.69999999999999996f);
		float 	fSlow93 = (fSlow86 + max(0.001f, fabsf((((iSlow89)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow90))):max(0.0080000000000000002f, (318.0f * powf(fSlow90,12.6f)))) * (((iSlow89)?fSlow91:fSlow92) - ((iSlow89)?fSlow84:fSlow85))))));
		int 	iSlow94 = int((fSlow66 > fSlow88));
		float 	fSlow95 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry28) + fSlow72))));
		float 	fSlow96 = (fSlow93 + max(0.001f, fabsf((((iSlow94)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow95))):max(0.0080000000000000002f, (318.0f * powf(fSlow95,12.6f)))) * (((iSlow94)?fSlow91:fSlow92) - ((iSlow94)?fSlow74:fSlow75))))));
		float 	fSlow97 = (fConst0 * fSlow96);
		float 	fSlow98 = (fConst0 * fSlow93);
		int 	iTemp4 = iTempPerm4;
		float 	fSlow99 = (fConst0 * fSlow86);
		int 	iTemp5 = iTempPerm5;
		float 	fSlow100 = (fSlow62 * fSlow65);
		float 	fSlow101 = (fConst1 * (0 - (fSlow100 * (fSlow67 - fSlow80))));
		float 	fSlow102 = (0 - (fSlow100 * (fSlow80 - fSlow87)));
		float 	fSlow103 = (0 - (fConst0 * (fSlow86 - fSlow93)));
		float 	fSlow104 = (fSlow63 - fSlow87);
		float 	fSlow105 = (0 - (fConst0 * (fSlow93 - fSlow96)));
		float 	fTemp6 = fTempPerm6;
		float 	fSlow106 = (fConst1 * fSlow71);
		float 	fTemp7 = fTempPerm7;
		float 	fTemp8 = fTempPerm8;
		float 	fTemp9 = fTempPerm9;
		float 	fSlow107 = float(fentry29);
		float 	fSlow108 = float(fentry30);
		float 	fSlow109 = float(fentry31);
		float 	fSlow110 = ((0.0012626262626262627f * (fSlow7 * fSlow109)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow109))));
		float 	fSlow111 = ((fSlow107 * fSlow108) * fSlow110);
		float 	fSlow112 = float(fentry32);
		float 	fSlow113 = ((fSlow107 * fSlow112) * fSlow110);
		int 	iSlow114 = int((fSlow111 <= fSlow113));
		float 	fSlow115 = float(fentry37);
		float 	fSlow116 = ((0.25f * float(fentry35)) + ((int(float(fentry36)))?fSlow115:(440.0f * (fSlow15 * fSlow115))));
		float 	fSlow117 = ((0.057142857142857148f * float(fentry34)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow116))) + 48.0f));
		float 	fSlow118 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry33) + fSlow117))));
		float 	fSlow119 = powf((0.014705882352941176f * (float((fSlow113 >= 30.0f)) * min((fSlow113 + -30.0f), (float)68))),1.8f);
		float 	fSlow120 = powf((0.010101010101010102f * min((float)99, fSlow113)),0.69999999999999996f);
		float 	fSlow121 = powf((0.014705882352941176f * (float((fSlow111 >= 30.0f)) * min((fSlow111 + -30.0f), (float)68))),1.8f);
		float 	fSlow122 = powf((0.010101010101010102f * min((float)99, fSlow111)),0.69999999999999996f);
		float 	fSlow123 = max(0.001f, fabsf((0 - (((iSlow114)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow118))):max(0.0080000000000000002f, (318.0f * powf(fSlow118,12.6f)))) * (((iSlow114)?fSlow119:fSlow120) - ((iSlow114)?fSlow121:fSlow122))))));
		float 	fSlow124 = (fConst0 * fSlow123);
		float 	fSlow125 = float(fentry38);
		float 	fSlow126 = ((fSlow107 * fSlow125) * fSlow110);
		int 	iSlow127 = int((fSlow126 > fSlow113));
		float 	fSlow128 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry39) + fSlow117))));
		float 	fSlow129 = powf((0.014705882352941176f * (float((fSlow126 >= 30.0f)) * min((fSlow126 + -30.0f), (float)68))),1.8f);
		float 	fSlow130 = powf((0.010101010101010102f * min((float)99, fSlow126)),0.69999999999999996f);
		float 	fSlow131 = max(0.001f, fabsf((((iSlow127)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow128))):max(0.0080000000000000002f, (318.0f * powf(fSlow128,12.6f)))) * (((iSlow127)?fSlow119:fSlow120) - ((iSlow127)?fSlow129:fSlow130)))));
		float 	fSlow132 = float(fentry40);
		float 	fSlow133 = ((fSlow107 * fSlow132) * fSlow110);
		int 	iSlow134 = int((fSlow133 > fSlow126));
		float 	fSlow135 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry41) + fSlow117))));
		float 	fSlow136 = powf((0.014705882352941176f * (float((fSlow133 >= 30.0f)) * min((fSlow133 + -30.0f), (float)68))),1.8f);
		float 	fSlow137 = powf((0.010101010101010102f * min((float)99, fSlow133)),0.69999999999999996f);
		float 	fSlow138 = (fSlow131 + max(0.001f, fabsf((((iSlow134)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow135))):max(0.0080000000000000002f, (318.0f * powf(fSlow135,12.6f)))) * (((iSlow134)?fSlow136:fSlow137) - ((iSlow134)?fSlow129:fSlow130))))));
		int 	iSlow139 = int((fSlow111 > fSlow133));
		float 	fSlow140 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry42) + fSlow117))));
		float 	fSlow141 = (fSlow138 + max(0.001f, fabsf((0 - (((iSlow139)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow140))):max(0.0080000000000000002f, (318.0f * powf(fSlow140,12.6f)))) * (((iSlow139)?fSlow121:fSlow122) - ((iSlow139)?fSlow136:fSlow137)))))));
		float 	fSlow142 = (fConst0 * fSlow141);
		float 	fSlow143 = (fConst0 * fSlow138);
		int 	iTemp10 = iTempPerm10;
		float 	fSlow144 = (fConst0 * fSlow131);
		int 	iTemp11 = iTempPerm11;
		float 	fSlow145 = (fSlow107 * fSlow110);
		float 	fSlow146 = (fConst1 * (0 - (fSlow145 * (fSlow112 - fSlow125))));
		float 	fSlow147 = (fConst1 * (0 - (fSlow145 * (fSlow125 - fSlow132))));
		float 	fSlow148 = (fSlow138 - fSlow131);
		float 	fSlow149 = (fConst1 * (fSlow108 - fSlow132));
		float 	fSlow150 = (fSlow141 - fSlow138);
		float 	fTemp12 = fTempPerm12;
		float 	fSlow151 = (fConst1 * fSlow116);
		float 	fTemp13 = fTempPerm13;
		float 	fSlow152 = float(fentry43);
		float 	fSlow153 = float(fentry44);
		float 	fSlow154 = float(fentry45);
		float 	fSlow155 = ((0.0012626262626262627f * (fSlow7 * fSlow154)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow154))));
		float 	fSlow156 = ((fSlow152 * fSlow153) * fSlow155);
		float 	fSlow157 = float(fentry46);
		float 	fSlow158 = ((fSlow152 * fSlow157) * fSlow155);
		int 	iSlow159 = int((fSlow156 <= fSlow158));
		float 	fSlow160 = float(fentry51);
		float 	fSlow161 = ((0.25f * float(fentry49)) + ((int(float(fentry50)))?fSlow160:(440.0f * (fSlow15 * fSlow160))));
		float 	fSlow162 = ((0.057142857142857148f * float(fentry48)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow161))) + 48.0f));
		float 	fSlow163 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry47) + fSlow162))));
		float 	fSlow164 = powf((0.014705882352941176f * (float((fSlow156 >= 30.0f)) * min((fSlow156 + -30.0f), (float)68))),1.8f);
		float 	fSlow165 = powf((0.010101010101010102f * min((float)99, fSlow156)),0.69999999999999996f);
		float 	fSlow166 = powf((0.014705882352941176f * (float((fSlow158 >= 30.0f)) * min((fSlow158 + -30.0f), (float)68))),1.8f);
		float 	fSlow167 = powf((0.010101010101010102f * min((float)99, fSlow158)),0.69999999999999996f);
		float 	fSlow168 = max(0.001f, fabsf((((iSlow159)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow163))):max(0.0080000000000000002f, (318.0f * powf(fSlow163,12.6f)))) * (((iSlow159)?fSlow164:fSlow165) - ((iSlow159)?fSlow166:fSlow167)))));
		float 	fSlow169 = (fConst0 * fSlow168);
		float 	fSlow170 = float(fentry52);
		float 	fSlow171 = ((fSlow152 * fSlow170) * fSlow155);
		int 	iSlow172 = int((fSlow171 > fSlow158));
		float 	fSlow173 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry53) + fSlow162))));
		float 	fSlow174 = powf((0.014705882352941176f * (float((fSlow171 >= 30.0f)) * min((fSlow171 + -30.0f), (float)68))),1.8f);
		float 	fSlow175 = powf((0.010101010101010102f * min((float)99, fSlow171)),0.69999999999999996f);
		float 	fSlow176 = max(0.001f, fabsf((((iSlow172)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow173))):max(0.0080000000000000002f, (318.0f * powf(fSlow173,12.6f)))) * (((iSlow172)?fSlow166:fSlow167) - ((iSlow172)?fSlow174:fSlow175)))));
		float 	fSlow177 = float(fentry54);
		float 	fSlow178 = ((fSlow152 * fSlow177) * fSlow155);
		int 	iSlow179 = int((fSlow178 > fSlow171));
		float 	fSlow180 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry55) + fSlow162))));
		float 	fSlow181 = powf((0.014705882352941176f * (float((fSlow178 >= 30.0f)) * min((fSlow178 + -30.0f), (float)68))),1.8f);
		float 	fSlow182 = powf((0.010101010101010102f * min((float)99, fSlow178)),0.69999999999999996f);
		float 	fSlow183 = (fSlow176 + max(0.001f, fabsf((0 - (((iSlow179)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow180))):max(0.0080000000000000002f, (318.0f * powf(fSlow180,12.6f)))) * (((iSlow179)?fSlow174:fSlow175) - ((iSlow179)?fSlow181:fSlow182)))))));
		int 	iSlow184 = int((fSlow156 > fSlow178));
		float 	fSlow185 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry56) + fSlow162))));
		float 	fSlow186 = (fSlow183 + max(0.001f, fabsf((((iSlow184)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow185))):max(0.0080000000000000002f, (318.0f * powf(fSlow185,12.6f)))) * (((iSlow184)?fSlow181:fSlow182) - ((iSlow184)?fSlow164:fSlow165))))));
		float 	fSlow187 = (fConst0 * fSlow186);
		float 	fSlow188 = (fConst0 * fSlow183);
		int 	iTemp14 = iTempPerm14;
		float 	fSlow189 = (fConst0 * fSlow176);
		int 	iTemp15 = iTempPerm15;
		float 	fSlow190 = (fSlow152 * fSlow155);
		float 	fSlow191 = (fConst1 * (0 - (fSlow190 * (fSlow157 - fSlow170))));
		float 	fSlow192 = (fConst1 * (0 - (fSlow190 * (fSlow170 - fSlow177))));
		float 	fSlow193 = (fSlow183 - fSlow176);
		float 	fSlow194 = (fSlow153 - fSlow177);
		float 	fSlow195 = (0 - (fConst0 * (fSlow183 - fSlow186)));
		float 	fTemp16 = fTempPerm16;
		float 	fSlow196 = (fConst1 * fSlow161);
		float 	fTemp17 = fTempPerm17;
		float 	fTemp18 = fTempPerm18;
		float 	fTemp19 = fTempPerm19;
		float 	fTemp20 = fTempPerm20;
		float 	fSlow197 = float(fentry57);
		float 	fSlow198 = float(fentry58);
		float 	fSlow199 = float(fentry59);
		float 	fSlow200 = ((0.0012626262626262627f * (fSlow7 * fSlow199)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow199))));
		float 	fSlow201 = ((fSlow197 * fSlow198) * fSlow200);
		float 	fSlow202 = float(fentry60);
		float 	fSlow203 = ((fSlow197 * fSlow202) * fSlow200);
		int 	iSlow204 = int((fSlow201 <= fSlow203));
		float 	fSlow205 = float(fentry65);
		float 	fSlow206 = ((0.25f * float(fentry63)) + ((int(float(fentry64)))?fSlow205:(440.0f * (fSlow15 * fSlow205))));
		float 	fSlow207 = ((0.057142857142857148f * float(fentry62)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow206))) + 48.0f));
		float 	fSlow208 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry61) + fSlow207))));
		float 	fSlow209 = powf((0.014705882352941176f * (float((fSlow201 >= 30.0f)) * min((fSlow201 + -30.0f), (float)68))),1.8f);
		float 	fSlow210 = powf((0.010101010101010102f * min((float)99, fSlow201)),0.69999999999999996f);
		float 	fSlow211 = powf((0.014705882352941176f * (float((fSlow203 >= 30.0f)) * min((fSlow203 + -30.0f), (float)68))),1.8f);
		float 	fSlow212 = powf((0.010101010101010102f * min((float)99, fSlow203)),0.69999999999999996f);
		float 	fSlow213 = max(0.001f, fabsf((((iSlow204)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow208))):max(0.0080000000000000002f, (318.0f * powf(fSlow208,12.6f)))) * (((iSlow204)?fSlow209:fSlow210) - ((iSlow204)?fSlow211:fSlow212)))));
		float 	fSlow214 = (fConst0 * fSlow213);
		float 	fSlow215 = float(fentry66);
		float 	fSlow216 = ((fSlow197 * fSlow215) * fSlow200);
		int 	iSlow217 = int((fSlow216 > fSlow203));
		float 	fSlow218 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry67) + fSlow207))));
		float 	fSlow219 = powf((0.014705882352941176f * (float((fSlow216 >= 30.0f)) * min((fSlow216 + -30.0f), (float)68))),1.8f);
		float 	fSlow220 = powf((0.010101010101010102f * min((float)99, fSlow216)),0.69999999999999996f);
		float 	fSlow221 = max(0.001f, fabsf((((iSlow217)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow218))):max(0.0080000000000000002f, (318.0f * powf(fSlow218,12.6f)))) * (((iSlow217)?fSlow211:fSlow212) - ((iSlow217)?fSlow219:fSlow220)))));
		float 	fSlow222 = float(fentry68);
		float 	fSlow223 = ((fSlow197 * fSlow222) * fSlow200);
		int 	iSlow224 = int((fSlow223 > fSlow216));
		float 	fSlow225 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry69) + fSlow207))));
		float 	fSlow226 = powf((0.014705882352941176f * (float((fSlow223 >= 30.0f)) * min((fSlow223 + -30.0f), (float)68))),1.8f);
		float 	fSlow227 = powf((0.010101010101010102f * min((float)99, fSlow223)),0.69999999999999996f);
		float 	fSlow228 = (fSlow221 + max(0.001f, fabsf((((iSlow224)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow225))):max(0.0080000000000000002f, (318.0f * powf(fSlow225,12.6f)))) * (((iSlow224)?fSlow226:fSlow227) - ((iSlow224)?fSlow219:fSlow220))))));
		int 	iSlow229 = int((fSlow201 > fSlow223));
		float 	fSlow230 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry70) + fSlow207))));
		float 	fSlow231 = (fSlow228 + max(0.001f, fabsf((((iSlow229)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow230))):max(0.0080000000000000002f, (318.0f * powf(fSlow230,12.6f)))) * (((iSlow229)?fSlow226:fSlow227) - ((iSlow229)?fSlow209:fSlow210))))));
		float 	fSlow232 = (fConst0 * fSlow231);
		float 	fSlow233 = (fConst0 * fSlow228);
		int 	iTemp21 = iTempPerm21;
		float 	fSlow234 = (fConst0 * fSlow221);
		int 	iTemp22 = iTempPerm22;
		float 	fSlow235 = (fSlow197 * fSlow200);
		float 	fSlow236 = (fConst1 * (0 - (fSlow235 * (fSlow202 - fSlow215))));
		float 	fSlow237 = (0 - (fSlow235 * (fSlow215 - fSlow222)));
		float 	fSlow238 = (0 - (fConst0 * (fSlow221 - fSlow228)));
		float 	fSlow239 = (fSlow198 - fSlow222);
		float 	fSlow240 = (0 - (fConst0 * (fSlow228 - fSlow231)));
		float 	fTemp23 = fTempPerm23;
		float 	fSlow241 = (fConst1 * fSlow206);
		float 	fTemp24 = fTempPerm24;
		float 	fSlow242 = float(fentry71);
		float 	fSlow243 = float(fentry72);
		float 	fSlow244 = float(fentry73);
		float 	fSlow245 = ((0.0012626262626262627f * (fSlow7 * fSlow244)) + (0.010101010101010102f * (1.0f - (0.125f * fSlow244))));
		float 	fSlow246 = ((fSlow242 * fSlow243) * fSlow245);
		float 	fSlow247 = float(fentry74);
		float 	fSlow248 = ((fSlow242 * fSlow247) * fSlow245);
		int 	iSlow249 = int((fSlow246 <= fSlow248));
		float 	fSlow250 = float(fentry79);
		float 	fSlow251 = ((0.25f * float(fentry77)) + ((int(float(fentry78)))?fSlow250:(440.0f * (fSlow15 * fSlow250))));
		float 	fSlow252 = ((0.057142857142857148f * float(fentry76)) * ((17.312340490667559f * logf((0.0022727272727272726f * fSlow251))) + 48.0f));
		float 	fSlow253 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry75) + fSlow252))));
		float 	fSlow254 = powf((0.014705882352941176f * (float((fSlow246 >= 30.0f)) * min((fSlow246 + -30.0f), (float)68))),1.8f);
		float 	fSlow255 = powf((0.010101010101010102f * min((float)99, fSlow246)),0.69999999999999996f);
		float 	fSlow256 = powf((0.014705882352941176f * (float((fSlow248 >= 30.0f)) * min((fSlow248 + -30.0f), (float)68))),1.8f);
		float 	fSlow257 = powf((0.010101010101010102f * min((float)99, fSlow248)),0.69999999999999996f);
		float 	fSlow258 = max(0.001f, fabsf((((iSlow249)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow253))):max(0.0080000000000000002f, (318.0f * powf(fSlow253,12.6f)))) * (((iSlow249)?fSlow254:fSlow255) - ((iSlow249)?fSlow256:fSlow257)))));
		float 	fSlow259 = (fConst0 * fSlow258);
		float 	fSlow260 = float(fentry80);
		float 	fSlow261 = ((fSlow242 * fSlow260) * fSlow245);
		int 	iSlow262 = int((fSlow261 > fSlow248));
		float 	fSlow263 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry81) + fSlow252))));
		float 	fSlow264 = powf((0.014705882352941176f * (float((fSlow261 >= 30.0f)) * min((fSlow261 + -30.0f), (float)68))),1.8f);
		float 	fSlow265 = powf((0.010101010101010102f * min((float)99, fSlow261)),0.69999999999999996f);
		float 	fSlow266 = max(0.001f, fabsf((((iSlow262)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow263))):max(0.0080000000000000002f, (318.0f * powf(fSlow263,12.6f)))) * (((iSlow262)?fSlow256:fSlow257) - ((iSlow262)?fSlow264:fSlow265)))));
		float 	fSlow267 = float(fentry82);
		float 	fSlow268 = ((fSlow242 * fSlow267) * fSlow245);
		int 	iSlow269 = int((fSlow268 > fSlow261));
		float 	fSlow270 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry83) + fSlow252))));
		float 	fSlow271 = powf((0.014705882352941176f * (float((fSlow268 >= 30.0f)) * min((fSlow268 + -30.0f), (float)68))),1.8f);
		float 	fSlow272 = powf((0.010101010101010102f * min((float)99, fSlow268)),0.69999999999999996f);
		float 	fSlow273 = (fSlow266 + max(0.001f, fabsf((((iSlow269)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow270))):max(0.0080000000000000002f, (318.0f * powf(fSlow270,12.6f)))) * (((iSlow269)?fSlow271:fSlow272) - ((iSlow269)?fSlow264:fSlow265))))));
		int 	iSlow274 = int((fSlow246 > fSlow268));
		float 	fSlow275 = (0.007874015748031496f * (127.0f - min((float)99, (float(fentry84) + fSlow252))));
		float 	fSlow276 = (fSlow273 + max(0.001f, fabsf((((iSlow274)?max(0.0030000000000000001f, (38.0f * faustpower<12>(fSlow275))):max(0.0080000000000000002f, (318.0f * powf(fSlow275,12.6f)))) * (((iSlow274)?fSlow271:fSlow272) - ((iSlow274)?fSlow254:fSlow255))))));
		float 	fSlow277 = (fConst0 * fSlow276);
		float 	fSlow278 = (fConst0 * fSlow273);
		int 	iTemp25 = iTempPerm25;
		float 	fSlow279 = (fConst0 * fSlow266);
		int 	iTemp26 = iTempPerm26;
		float 	fSlow280 = (fSlow242 * fSlow245);
		float 	fSlow281 = (fConst1 * (0 - (fSlow280 * (fSlow247 - fSlow260))));
		float 	fSlow282 = (0 - (fSlow280 * (fSlow260 - fSlow267)));
		float 	fSlow283 = (0 - (fConst0 * (fSlow266 - fSlow273)));
		float 	fSlow284 = (fSlow243 - fSlow267);
		float 	fSlow285 = (0 - (fConst0 * (fSlow273 - fSlow276)));
		float 	fTemp27 = fTempPerm27;
		float 	fSlow286 = (fConst1 * fSlow251);
		float 	fTemp28 = fTempPerm28;
		float 	fTemp29 = fTempPerm29;
		float 	fSlow287 = float(fentry85);
		float 	fSlow288 = (912.65810566616483f * fSlow287);
		float 	fTemp30 = fTempPerm30;
		float 	fSlow289 = (9126.5810566616474f * fSlow287);
		float 	fTemp31 = fTempPerm31;
		float 	fTemp32 = fTempPerm32;
		float 	fTemp33 = fTempPerm33;
		float 	fTemp34 = fTempPerm34;
		float 	fSlow290 = (1825.3162113323297f * fSlow287);
		float 	fTemp35 = fTempPerm35;
		float 	fTemp36 = fTempPerm36;
		float 	fTemp37 = fTempPerm37;
		float 	fTemp38 = fTempPerm38;
		float 	fTemp39 = fTempPerm39;
		float 	fTemp40 = fTempPerm40;
		//zone2b
		//zone3
		FAUSTFLOAT* output0 = output[0];
		//LoopGraphScalar
		for (int i=0; i<count; i++) {
			if ((iSlow25 || (iSlow1 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || iSlow31)))))))) {
				fRec0[0] = ((iSlow4)?0.0f:min(fSlow24, (fRec0[1] + 1.0f)));
				fRec2[0] = ((iSlow34)?0.0f:min(fSlow52, (fRec2[1] + 1.0f)));
				iTemp0 = int((fRec2[0] < fSlow53));
				iTemp1 = int((fRec2[0] < fSlow54));
				fRec1[0] = ((iSlow32)?(fSlow33 * ((iTemp0)?((iTemp1)?((int((fRec2[0] < 0.0f)))?fSlow12:((iTemp1)?(fSlow12 + (fSlow56 * (fRec2[0] / fSlow41))):fSlow36)):((iTemp0)?(fSlow55 * (fSlow35 + (fSlow57 * ((fRec2[0] - fSlow54) / fSlow58)))):fSlow43)):((int((fRec2[0] < fSlow52)))?(fSlow43 + (fSlow59 * ((fRec2[0] - fSlow53) / fSlow60))):fSlow10))):fRec1[1]);
				fTemp2 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec0[0] < 0.0f)))?fRec1[0]:((int((fRec0[0] < fSlow24)))?(fRec1[0] + (fConst1 * ((fRec0[0] * (fSlow12 - fRec1[0])) / fSlow23))):fSlow12)))));
				fTemp3 = (fRec4[1] + fSlow61);
				fRec4[0] = (fTemp3 - floorf(fTemp3));
				fRec5[0] = ((iSlow4)?0.0f:min(fSlow79, (fRec5[1] + 1.0f)));
				fRec7[0] = ((iSlow34)?0.0f:min(fSlow97, (fRec7[1] + 1.0f)));
				iTemp4 = int((fRec7[0] < fSlow98));
				iTemp5 = int((fRec7[0] < fSlow99));
				fRec6[0] = ((iSlow32)?(fSlow33 * ((iTemp4)?((iTemp5)?((int((fRec7[0] < 0.0f)))?fSlow68:((iTemp5)?(fSlow68 + (fSlow101 * (fRec7[0] / fSlow86))):fSlow81)):((iTemp4)?(fSlow81 + (fSlow102 * ((fRec7[0] - fSlow99) / fSlow103))):fSlow88)):((int((fRec7[0] < fSlow97)))?(fSlow100 * (fSlow87 + (fSlow104 * ((fRec7[0] - fSlow98) / fSlow105)))):fSlow66))):fRec6[1]);
				fTemp6 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec5[0] < 0.0f)))?fRec6[0]:((int((fRec5[0] < fSlow79)))?(fRec6[0] + (fConst1 * ((fRec5[0] * (fSlow68 - fRec6[0])) / fSlow78))):fSlow68)))));
				fTemp7 = (fRec8[1] + fSlow106);
				fRec8[0] = (fTemp7 - floorf(fTemp7));
			}
			if ((iSlow1 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow30 || iSlow31)))))) {
				fTemp8 = (65536.0f * fRec8[0]);
			}
			if ((iSlow1 || (iSlow27 || (iSlow28 || (iSlow30 || iSlow31))))) {
				fTemp9 = (fTemp2 * ftbl0[(((int((65536.0f * (fRec4[0] + (fTemp6 * ftbl0[(((int(fTemp8) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)]);
			}
			if ((iSlow25 || (iSlow1 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || iSlow31)))))))) {
				fRec9[0] = ((iSlow4)?0.0f:min(fSlow124, (fRec9[1] + 1.0f)));
				fRec11[0] = ((iSlow34)?0.0f:min(fSlow142, (fRec11[1] + 1.0f)));
				iTemp10 = int((fRec11[0] < fSlow143));
				iTemp11 = int((fRec11[0] < fSlow144));
				fRec10[0] = ((iSlow32)?(fSlow33 * ((iTemp10)?((iTemp11)?((int((fRec11[0] < 0.0f)))?fSlow113:((iTemp11)?(fSlow113 + (fSlow146 * (fRec11[0] / fSlow131))):fSlow126)):((iTemp10)?(fSlow126 + (fSlow147 * ((fRec11[0] - fSlow144) / fSlow148))):fSlow133)):((int((fRec11[0] < fSlow142)))?(fSlow145 * (fSlow132 + (fSlow149 * ((fRec11[0] - fSlow143) / fSlow150)))):fSlow111))):fRec10[1]);
				fTemp12 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec9[0] < 0.0f)))?fRec10[0]:((int((fRec9[0] < fSlow124)))?(fRec10[0] + (fConst1 * ((fRec9[0] * (fSlow113 - fRec10[0])) / fSlow123))):fSlow113)))));
				fTemp13 = (fRec12[1] + fSlow151);
				fRec12[0] = (fTemp13 - floorf(fTemp13));
				fRec13[0] = ((iSlow4)?0.0f:min(fSlow169, (fRec13[1] + 1.0f)));
				fRec15[0] = ((iSlow34)?0.0f:min(fSlow187, (fRec15[1] + 1.0f)));
				iTemp14 = int((fRec15[0] < fSlow188));
				iTemp15 = int((fRec15[0] < fSlow189));
				fRec14[0] = ((iSlow32)?(fSlow33 * ((iTemp14)?((iTemp15)?((int((fRec15[0] < 0.0f)))?fSlow158:((iTemp15)?(fSlow158 + (fSlow191 * (fRec15[0] / fSlow176))):fSlow171)):((iTemp14)?(fSlow171 + (fSlow192 * ((fRec15[0] - fSlow189) / fSlow193))):fSlow178)):((int((fRec15[0] < fSlow187)))?(fSlow190 * (fSlow177 + (fSlow194 * ((fRec15[0] - fSlow188) / fSlow195)))):fSlow156))):fRec14[1]);
				fTemp16 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec13[0] < 0.0f)))?fRec14[0]:((int((fRec13[0] < fSlow169)))?(fRec14[0] + (fConst1 * ((fRec13[0] * (fSlow158 - fRec14[0])) / fSlow168))):fSlow158)))));
				fTemp17 = (fRec16[1] + fSlow196);
				fRec16[0] = (fTemp17 - floorf(fTemp17));
			}
			if ((iSlow1 || (iSlow28 || (iSlow30 || iSlow31)))) {
				fTemp18 = (65536.0f * fRec16[0]);
			}
			if ((iSlow1 || (iSlow28 || iSlow30))) {
				fTemp19 = (fTemp16 * ftbl0[(((int(fTemp18) % 65536) + 65536) % 65536)]);
			}
			if ((iSlow1 || iSlow28)) {
				fTemp20 = (fTemp9 + (fTemp12 * ftbl0[(((int((65536.0f * (fRec12[0] + fTemp19))) % 65536) + 65536) % 65536)]));
			}
			if ((iSlow25 || (iSlow1 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || iSlow31)))))))) {
				fRec17[0] = ((iSlow4)?0.0f:min(fSlow214, (fRec17[1] + 1.0f)));
				fRec19[0] = ((iSlow34)?0.0f:min(fSlow232, (fRec19[1] + 1.0f)));
				iTemp21 = int((fRec19[0] < fSlow233));
				iTemp22 = int((fRec19[0] < fSlow234));
				fRec18[0] = ((iSlow32)?(fSlow33 * ((iTemp21)?((iTemp22)?((int((fRec19[0] < 0.0f)))?fSlow203:((iTemp22)?(fSlow203 + (fSlow236 * (fRec19[0] / fSlow221))):fSlow216)):((iTemp21)?(fSlow216 + (fSlow237 * ((fRec19[0] - fSlow234) / fSlow238))):fSlow223)):((int((fRec19[0] < fSlow232)))?(fSlow235 * (fSlow222 + (fSlow239 * ((fRec19[0] - fSlow233) / fSlow240)))):fSlow201))):fRec18[1]);
				fTemp23 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec17[0] < 0.0f)))?fRec18[0]:((int((fRec17[0] < fSlow214)))?(fRec18[0] + (fConst1 * ((fRec17[0] * (fSlow203 - fRec18[0])) / fSlow213))):fSlow203)))));
				fTemp24 = (fRec20[1] + fSlow241);
				fRec20[0] = (fTemp24 - floorf(fTemp24));
				fRec22[0] = ((iSlow4)?0.0f:min(fSlow259, (fRec22[1] + 1.0f)));
				fRec24[0] = ((iSlow34)?0.0f:min(fSlow277, (fRec24[1] + 1.0f)));
				iTemp25 = int((fRec24[0] < fSlow278));
				iTemp26 = int((fRec24[0] < fSlow279));
				fRec23[0] = ((iSlow32)?(fSlow33 * ((iTemp25)?((iTemp26)?((int((fRec24[0] < 0.0f)))?fSlow248:((iTemp26)?(fSlow248 + (fSlow281 * (fRec24[0] / fSlow266))):fSlow261)):((iTemp25)?(fSlow261 + (fSlow282 * ((fRec24[0] - fSlow279) / fSlow283))):fSlow268)):((int((fRec24[0] < fSlow277)))?(fSlow280 * (fSlow267 + (fSlow284 * ((fRec24[0] - fSlow278) / fSlow285)))):fSlow246))):fRec23[1]);
				fTemp27 = faustpower<8>((0.01020408163265306f * min((float)98, ((int((fRec22[0] < 0.0f)))?fRec23[0]:((int((fRec22[0] < fSlow259)))?(fRec23[0] + (fConst1 * ((fRec22[0] * (fSlow248 - fRec23[0])) / fSlow258))):fSlow248)))));
				fTemp28 = (fRec25[1] + fSlow286);
				fRec25[0] = (fTemp28 - floorf(fTemp28));
				fTemp29 = (65536.0f * fRec25[0]);
			}
			if (iSlow1) {
				fRec21[0] = (fTemp27 * ftbl0[(((int((fTemp29 + (fSlow288 * fRec21[1]))) % 65536) + 65536) % 65536)]);
				fTemp30 = (2.0879500000000002f * (fTemp20 + (fTemp23 * ftbl0[(((int((65536.0f * (fRec20[0] + fRec21[0]))) % 65536) + 65536) % 65536)])));
			}
			if (iSlow25) {
				fRec26[0] = (2.0879500000000002f * (fTemp16 * ftbl0[(((int((65536.0f * (fRec16[0] + (fTemp23 * ftbl0[(((int((65536.0f * (fRec20[0] + (fTemp27 * ftbl0[(((int((fTemp29 + (fSlow289 * fRec26[1]))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)]));
			}
			if ((iSlow25 || iSlow29)) {
				fTemp31 = (fTemp2 * ftbl0[(((int((65536.0f * (fRec4[0] + (fTemp6 * ftbl0[(((int((65536.0f * (fRec8[0] + (fTemp12 * ftbl0[(((int((65536.0f * fRec12[0])) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)]);
			}
			if (iSlow25) {
				fTemp32 = (fRec26[0] + (2.0879500000000002f * fTemp31));
			}
			if ((iSlow29 || iSlow30)) {
				fRec27[0] = (fTemp27 * ftbl0[(((int((fTemp29 + (fSlow289 * fRec27[1]))) % 65536) + 65536) % 65536)]);
				fTemp33 = (fTemp23 * ftbl0[(((int((65536.0f * (fRec20[0] + fRec27[0]))) % 65536) + 65536) % 65536)]);
			}
			if (iSlow29) {
				fTemp34 = (2.0879500000000002f * (fTemp31 + (fTemp16 * ftbl0[(((int((65536.0f * (fRec16[0] + fTemp33))) % 65536) + 65536) % 65536)])));
			}
			if (iSlow26) {
				fRec28[0] = (fTemp6 * ftbl0[(((int((fTemp8 + (fSlow290 * fRec28[1]))) % 65536) + 65536) % 65536)]);
			}
			if ((iSlow26 || iSlow31)) {
				fTemp35 = (fTemp23 * ftbl0[(((int((65536.0f * (fRec20[0] + (fTemp27 * ftbl0[(((int(fTemp29) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)]);
			}
			if (iSlow26) {
				fTemp36 = (2.0879500000000002f * ((fTemp2 * ftbl0[(((int((65536.0f * (fRec4[0] + fRec28[0]))) % 65536) + 65536) % 65536)]) + (fTemp12 * ftbl0[(((int((65536.0f * (fRec12[0] + (fTemp16 * ftbl0[(((int((65536.0f * (fRec16[0] + fTemp35))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])));
			}
			if (iSlow27) {
				fRec29[0] = (fTemp27 * ftbl0[(((int((fTemp29 + (fSlow290 * fRec29[1]))) % 65536) + 65536) % 65536)]);
				fTemp37 = (2.0879500000000002f * (fTemp9 + (fTemp12 * ftbl0[(((int((65536.0f * (fRec12[0] + (fTemp16 * ftbl0[(((int((65536.0f * (fRec16[0] + (fTemp23 * ftbl0[(((int((65536.0f * (fRec20[0] + fRec29[0]))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)])));
			}
			if (iSlow28) {
				fRec30[0] = (2.0879500000000002f * (fTemp23 * ftbl0[(((int((65536.0f * (fRec20[0] + (fTemp27 * ftbl0[(((int((fTemp29 + (fSlow289 * fRec30[1]))) % 65536) + 65536) % 65536)])))) % 65536) + 65536) % 65536)]));
				fTemp38 = (fRec30[0] + (2.0879500000000002f * fTemp20));
			}
			if (iSlow30) {
				fTemp39 = (2.0879500000000002f * (fTemp9 + (fTemp12 * ftbl0[(((int((65536.0f * (fRec12[0] + (fTemp33 + fTemp19)))) % 65536) + 65536) % 65536)])));
			}
			if (iSlow31) {
				fRec31[0] = (fTemp16 * ftbl0[(((int((fTemp18 + (fSlow288 * fRec31[1]))) % 65536) + 65536) % 65536)]);
				fTemp40 = (2.0879500000000002f * (fTemp9 + (fTemp12 * ftbl0[(((int((65536.0f * (fRec12[0] + (fRec31[0] + fTemp35)))) % 65536) + 65536) % 65536)])));
			}
			output0[i] = (FAUSTFLOAT)(fTemp30 + (fTemp32 + (fTemp34 + (fTemp36 + (fTemp37 + ((fTemp38 + fTemp39) + fTemp40))))));
			// post processing
			if (iSlow31) {
				fRec31[1] = fRec31[0];
			}
			if (iSlow28) {
				fRec30[1] = fRec30[0];
			}
			if (iSlow27) {
				fRec29[1] = fRec29[0];
			}
			if (iSlow26) {
				fRec28[1] = fRec28[0];
			}
			if ((iSlow29 || iSlow30)) {
				fRec27[1] = fRec27[0];
			}
			if (iSlow25) {
				fRec26[1] = fRec26[0];
			}
			if (iSlow1) {
				fRec21[1] = fRec21[0];
			}
			if ((iSlow25 || (iSlow1 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || iSlow31)))))))) {
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
		fTempPerm9 = fTemp9;
		iTempPerm10 = iTemp10;
		iTempPerm11 = iTemp11;
		fTempPerm12 = fTemp12;
		fTempPerm13 = fTemp13;
		iTempPerm14 = iTemp14;
		iTempPerm15 = iTemp15;
		fTempPerm16 = fTemp16;
		fTempPerm17 = fTemp17;
		fTempPerm18 = fTemp18;
		fTempPerm19 = fTemp19;
		fTempPerm20 = fTemp20;
		iTempPerm21 = iTemp21;
		iTempPerm22 = iTemp22;
		fTempPerm23 = fTemp23;
		fTempPerm24 = fTemp24;
		iTempPerm25 = iTemp25;
		iTempPerm26 = iTemp26;
		fTempPerm27 = fTemp27;
		fTempPerm28 = fTemp28;
		fTempPerm29 = fTemp29;
		fTempPerm30 = fTemp30;
		fTempPerm31 = fTemp31;
		fTempPerm32 = fTemp32;
		fTempPerm33 = fTemp33;
		fTempPerm34 = fTemp34;
		fTempPerm35 = fTemp35;
		fTempPerm36 = fTemp36;
		fTempPerm37 = fTemp37;
		fTempPerm38 = fTemp38;
		fTempPerm39 = fTemp39;
		fTempPerm40 = fTemp40;
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
