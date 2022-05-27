/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.piano"
version: "1.0"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_piano_H__
#define  __synth_piano_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_piano_dsp.h **************************/
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

class synth_piano_dsp {

    public:

        synth_piano_dsp() {}
        virtual ~synth_piano_dsp() {}

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
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual synth_piano_dsp* clone() = 0;
    
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

class decorator_dsp : public synth_piano_dsp {

    protected:

        synth_piano_dsp* fDSP;

    public:

        decorator_dsp(synth_piano_dsp* synth_piano_dsp = nullptr):fDSP(synth_piano_dsp) {}
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
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
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
    
        virtual synth_piano_dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

// Denormal handling

#if defined (__SSE__)
#include <xmmintrin.h>
#endif

class ScopedNoDenormals
{
    private:
    
        intptr_t fpsr;
        
        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
           asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            _mm_setcsr(static_cast<uint32_t>(fpsr_aux));
        #endif
        }
        
        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined ( __SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }
    
    public:
    
        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #else
            #if defined(__SSE__)
            #if defined(__SSE2__)
                intptr_t mask = 0x8040;
            #else
                intptr_t mask = 0x8000;
            #endif
            #else
                intptr_t mask = 0x0000;
            #endif
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }
        
        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals();

#endif

/************************** END synth_piano_dsp.h **************************/
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
    
    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
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

/**
 The base class of Meta handler to be used in synth_piano_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
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
struct synth_piano : public synth_piano_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include "piano.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

static float synth_piano_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_piano
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_piano : public synth_piano_dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider1;
	float fConst2;
	float fRec10[2];
	float fConst4;
	FAUSTFLOAT fHslider2;
	float fConst5;
	FAUSTFLOAT fHslider3;
	float fConst6;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fButton0;
	float fRec18[2];
	FAUSTFLOAT fHslider5;
	int iRec24[2];
	int iRec26[2];
	float fConst7;
	float fRec25[2];
	float fConst8;
	float fConst9;
	FAUSTFLOAT fHslider6;
	float fConst10;
	float fConst11;
	float fRec27[2];
	float fRec23[2];
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	float fVec0[2];
	float fRec17[2];
	float fRec16[2];
	int IOTA;
	float fRec15[8192];
	float fVec1[2];
	float fRec31[2];
	float fRec30[2];
	float fRec29[8192];
	float fVec2[2];
	float fRec28[2];
	float fRec12[2];
	float fRec13[2];
	float fConst12;
	float fConst13;
	float fConst14;
	float fRec11[3];
	float fRec44[2];
	float fRec43[2];
	float fRec42[2];
	float fRec41[2];
	float fRec40[2];
	float fVec3[2];
	float fRec39[2];
	float fRec38[2];
	float fRec37[8192];
	float fVec4[2];
	float fRec48[2];
	float fRec47[2];
	float fRec46[8192];
	float fConst15;
	float fVec5[2];
	float fRec51[2];
	float fRec50[2];
	float fRec49[8192];
	float fVec6[2];
	float fRec45[2];
	float fRec33[2];
	float fRec34[2];
	float fRec35[2];
	float fRec32[3];
	float fConst16;
	float fVec7[2];
	float fVec8[2];
	float fRec61[2];
	float fRec60[2];
	float fRec59[2];
	float fRec58[2];
	float fRec57[2];
	float fRec56[3];
	float fRec55[3];
	float fRec54[3];
	float fRec53[3];
	float fRec52[2];
	float fVec9[4096];
	float fVec10[4096];
	int iConst17;
	float fConst18;
	float fConst21;
	FAUSTFLOAT fHslider7;
	float fConst22;
	float fConst24;
	float fConst25;
	float fRec63[2];
	float fRec62[2];
	float fVec11[16384];
	int iConst27;
	float fVec12[4096];
	int iConst28;
	float fRec8[2];
	float fConst31;
	float fConst32;
	float fRec67[2];
	float fRec66[2];
	float fVec13[16384];
	int iConst34;
	float fVec14[2048];
	int iConst35;
	float fRec64[2];
	float fConst38;
	float fConst39;
	float fRec71[2];
	float fRec70[2];
	float fVec15[32768];
	int iConst41;
	float fConst42;
	FAUSTFLOAT fHslider8;
	float fVec16[4096];
	float fVec17[2048];
	int iConst43;
	float fRec68[2];
	float fConst46;
	float fConst47;
	float fRec75[2];
	float fRec74[2];
	float fVec18[32768];
	int iConst49;
	float fVec19[4096];
	int iConst50;
	float fRec72[2];
	float fConst53;
	float fConst54;
	float fRec79[2];
	float fRec78[2];
	float fVec20[16384];
	int iConst56;
	float fVec21[4096];
	int iConst57;
	float fRec76[2];
	float fConst60;
	float fConst61;
	float fRec83[2];
	float fRec82[2];
	float fVec22[32768];
	int iConst63;
	float fVec23[4096];
	int iConst64;
	float fRec80[2];
	float fConst67;
	float fConst68;
	float fRec87[2];
	float fRec86[2];
	float fVec24[16384];
	int iConst70;
	float fVec25[2048];
	int iConst71;
	float fRec84[2];
	float fConst74;
	float fConst75;
	float fRec91[2];
	float fRec90[2];
	float fVec26[16384];
	int iConst77;
	float fVec27[4096];
	int iConst78;
	float fRec88[2];
	float fRec0[3];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -es 1 -single -ftz 0");
		m->declare("copyright", "Romain Michon");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("description", "WaveGuide Commuted Piano");
		m->declare("filename", "synth_piano.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("instruments.lib/author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("instruments.lib/copyright", "Romain Michon");
		m->declare("instruments.lib/licence", "STK-4.3");
		m->declare("instruments.lib/name", "Faust-STK Tools Library");
		m->declare("instruments.lib/version", "1.0");
		m->declare("licence", "STK-4.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "synth.piano");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.1");
		m->declare("spn.lib/name", "Standart Pitch Notation constants");
		m->declare("spn.lib/version", "0.2");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (44.0999985f / fConst0);
		fConst2 = (1.0f - fConst1);
		float fConst3 = synth_piano_faustpower2_f(fConst0);
		fConst4 = (96800.0f / fConst3);
		fConst5 = (0.159154937f * fConst0);
		fConst6 = (6.28318548f / fConst0);
		fConst7 = (7.0f / fConst0);
		fConst8 = std::exp((0.0f - (5.0f / fConst0)));
		fConst9 = (10.0f / fConst0);
		fConst10 = (0.100000001f * fConst0);
		fConst11 = std::exp((0.0f - (0.5f / fConst0)));
		fConst12 = (880.0f / fConst0);
		fConst13 = (2764.60156f / fConst0);
		fConst14 = (193600.0f / fConst3);
		fConst15 = (0.000361715793f * fConst0);
		fConst16 = (0.0500000007f / fConst0);
		iConst17 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (0.0199999996f * fConst0))));
		fConst18 = std::cos((37699.1133f / fConst0));
		float fConst19 = std::floor(((0.174713001f * fConst0) + 0.5f));
		float fConst20 = ((0.0f - (6.90775537f * fConst19)) / fConst0);
		fConst21 = (0.25f * fConst20);
		fConst22 = (0.166666672f * fConst20);
		float fConst23 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst24 = (1.0f / (fConst23 + 1.0f));
		fConst25 = (1.0f - fConst23);
		float fConst26 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst27 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst19 - fConst26))));
		iConst28 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst26 + -1.0f))));
		float fConst29 = std::floor(((0.153128996f * fConst0) + 0.5f));
		float fConst30 = ((0.0f - (6.90775537f * fConst29)) / fConst0);
		fConst31 = (0.25f * fConst30);
		fConst32 = (0.166666672f * fConst30);
		float fConst33 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst34 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst29 - fConst33))));
		iConst35 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst33 + -1.0f))));
		float fConst36 = std::floor(((0.219990999f * fConst0) + 0.5f));
		float fConst37 = ((0.0f - (6.90775537f * fConst36)) / fConst0);
		fConst38 = (0.25f * fConst37);
		fConst39 = (0.166666672f * fConst37);
		float fConst40 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst41 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst36 - fConst40))));
		fConst42 = (0.0011363636f * fConst0);
		iConst43 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst40 + -1.0f))));
		float fConst44 = std::floor(((0.256891012f * fConst0) + 0.5f));
		float fConst45 = ((0.0f - (6.90775537f * fConst44)) / fConst0);
		fConst46 = (0.25f * fConst45);
		fConst47 = (0.166666672f * fConst45);
		float fConst48 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst49 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst44 - fConst48))));
		iConst50 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst48 + -1.0f))));
		float fConst51 = std::floor(((0.192303002f * fConst0) + 0.5f));
		float fConst52 = ((0.0f - (6.90775537f * fConst51)) / fConst0);
		fConst53 = (0.25f * fConst52);
		fConst54 = (0.166666672f * fConst52);
		float fConst55 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst56 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst51 - fConst55))));
		iConst57 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst55 + -1.0f))));
		float fConst58 = std::floor(((0.210389003f * fConst0) + 0.5f));
		float fConst59 = ((0.0f - (6.90775537f * fConst58)) / fConst0);
		fConst60 = (0.25f * fConst59);
		fConst61 = (0.166666672f * fConst59);
		float fConst62 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst63 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst58 - fConst62))));
		iConst64 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst62 + -1.0f))));
		float fConst65 = std::floor(((0.125f * fConst0) + 0.5f));
		float fConst66 = ((0.0f - (6.90775537f * fConst65)) / fConst0);
		fConst67 = (0.25f * fConst66);
		fConst68 = (0.166666672f * fConst66);
		float fConst69 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst70 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst65 - fConst69))));
		iConst71 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst69 + -1.0f))));
		float fConst72 = std::floor(((0.127837002f * fConst0) + 0.5f));
		float fConst73 = ((0.0f - (6.90775537f * fConst72)) / fConst0);
		fConst74 = (0.25f * fConst73);
		fConst75 = (0.166666672f * fConst73);
		float fConst76 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst77 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst72 - fConst76))));
		iConst78 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst76 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.59999999999999998f);
		fHslider1 = FAUSTFLOAT(0.13700000000000001f);
		fHslider2 = FAUSTFLOAT(48.0f);
		fHslider3 = FAUSTFLOAT(0.28000000000000003f);
		fHslider4 = FAUSTFLOAT(0.10000000000000001f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.10000000000000001f);
		fHslider7 = FAUSTFLOAT(0.35999999999999999f);
		fHslider8 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec10[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec18[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			iRec24[l2] = 0;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			iRec26[l3] = 0;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec25[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec27[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec23[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec22[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec21[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec20[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec19[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			fVec0[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec17[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec16[l13] = 0.0f;
		}
		IOTA = 0;
		for (int l14 = 0; (l14 < 8192); l14 = (l14 + 1)) {
			fRec15[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 2); l15 = (l15 + 1)) {
			fVec1[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec31[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec30[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 8192); l18 = (l18 + 1)) {
			fRec29[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fVec2[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec28[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec12[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec13[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec11[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec44[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec43[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec42[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec41[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec40[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fVec3[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec39[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec38[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 8192); l32 = (l32 + 1)) {
			fRec37[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fVec4[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec48[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec47[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 8192); l36 = (l36 + 1)) {
			fRec46[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec5[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec51[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec50[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 8192); l40 = (l40 + 1)) {
			fRec49[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fVec6[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec45[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec33[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec34[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec35[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec32[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fVec7[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fVec8[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec61[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec60[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec59[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec58[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec57[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec56[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec55[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 3); l56 = (l56 + 1)) {
			fRec54[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec53[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec52[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 4096); l59 = (l59 + 1)) {
			fVec9[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 4096); l60 = (l60 + 1)) {
			fVec10[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec63[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec62[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 16384); l63 = (l63 + 1)) {
			fVec11[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 4096); l64 = (l64 + 1)) {
			fVec12[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec8[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec67[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec66[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 16384); l68 = (l68 + 1)) {
			fVec13[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2048); l69 = (l69 + 1)) {
			fVec14[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec64[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec71[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec70[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 32768); l73 = (l73 + 1)) {
			fVec15[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 4096); l74 = (l74 + 1)) {
			fVec16[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 2048); l75 = (l75 + 1)) {
			fVec17[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec68[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec75[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec74[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 32768); l79 = (l79 + 1)) {
			fVec18[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 4096); l80 = (l80 + 1)) {
			fVec19[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec72[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec79[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec78[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 16384); l84 = (l84 + 1)) {
			fVec20[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 4096); l85 = (l85 + 1)) {
			fVec21[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec76[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec83[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec82[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 32768); l89 = (l89 + 1)) {
			fVec22[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 4096); l90 = (l90 + 1)) {
			fVec23[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fRec80[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec87[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec86[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 16384); l94 = (l94 + 1)) {
			fVec24[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 2048); l95 = (l95 + 1)) {
			fVec25[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec84[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec91[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec90[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 16384); l99 = (l99 + 1)) {
			fVec26[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 4096); l100 = (l100 + 1)) {
			fVec27[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec88[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec0[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec1[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 3); l104 = (l104 + 1)) {
			fRec2[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 3); l105 = (l105 + 1)) {
			fRec3[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec4[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec5[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec6[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 3); l109 = (l109 + 1)) {
			fRec7[l109] = 0.0f;
		}
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
	
	virtual synth_piano* clone() {
		return new synth_piano();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.piano");
		ui_interface->addHorizontalSlider("brightness", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("detuning", &fHslider4, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider6, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("pan", &fHslider0, FAUSTFLOAT(0.600000024f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("pitch", &fHslider2, FAUSTFLOAT(48.0f), FAUSTFLOAT(21.0f), FAUSTFLOAT(108.0f), FAUSTFLOAT(0.00100000005f));
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider1, FAUSTFLOAT(0.136999995f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("room_size", &fHslider7, FAUSTFLOAT(0.360000014f), FAUSTFLOAT(0.00499999989f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("stiffness", &fHslider3, FAUSTFLOAT(0.280000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("width", &fHslider8, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (12.0f * (1.0f - fSlow0));
		float fSlow2 = (fConst1 * float(fHslider1));
		float fSlow3 = float(fHslider2);
		float fSlow4 = std::pow(2.0f, (0.0833333358f * (fSlow3 + -69.0f)));
		float fSlow5 = float(getValueEQBandWidthFactor(float(fSlow3)));
		float fSlow6 = (synth_piano_faustpower2_f(fSlow4) * synth_piano_faustpower2_f(fSlow5));
		float fSlow7 = (0.5f - (fConst4 * fSlow6));
		float fSlow8 = float(getValueEQGain(float(fSlow3)));
		float fSlow9 = float(fHslider3);
		float fSlow10 = float(getValueStiffnessCoefficient(float(fSlow3)));
		float fSlow11 = (13.6899996f * (synth_piano_faustpower2_f(fSlow9) * synth_piano_faustpower2_f(fSlow10)));
		float fSlow12 = (fSlow11 + -1.0f);
		float fSlow13 = (440.0f * fSlow4);
		float fSlow14 = (5.0f * (float(getValueDetuningHz(float(fSlow3))) * float(fHslider4)));
		float fSlow15 = (fSlow13 + fSlow14);
		float fSlow16 = (fConst6 * fSlow15);
		float fSlow17 = std::sin(fSlow16);
		float fSlow18 = (fSlow9 * fSlow10);
		float fSlow19 = (7.4000001f * fSlow18);
		float fSlow20 = (fSlow11 + 1.0f);
		float fSlow21 = std::cos(fSlow16);
		float fSlow22 = (3.0f * std::atan2((fSlow12 * fSlow17), (fSlow19 + (fSlow20 * fSlow21))));
		float fSlow23 = float(getValueSingleStringZero(float(fSlow3)));
		float fSlow24 = std::pow(10.0f, (0.000113636364f * (float(getValueSingleStringDecayRate(float(fSlow3))) / fSlow4)));
		float fSlow25 = float(getValueSingleStringPole(float(fSlow3)));
		float fSlow26 = (1.0f - fSlow25);
		float fSlow27 = ((fSlow23 * fSlow24) * fSlow26);
		float fSlow28 = (1.0f - fSlow23);
		float fSlow29 = (fSlow25 * fSlow28);
		float fSlow30 = (3.0f * fSlow29);
		float fSlow31 = (fSlow27 - fSlow30);
		float fSlow32 = (fSlow29 - fSlow27);
		float fSlow33 = (4.0f * fSlow32);
		float fSlow34 = (fSlow31 + fSlow33);
		float fSlow35 = (fSlow24 * fSlow26);
		float fSlow36 = ((3.0f * fSlow28) - fSlow35);
		float fSlow37 = (((fSlow21 * fSlow31) / fSlow36) + 1.0f);
		float fSlow38 = ((fSlow23 + fSlow35) + -1.0f);
		float fSlow39 = (4.0f * fSlow38);
		float fSlow40 = (((fSlow39 + (fSlow21 * fSlow34)) / fSlow36) + 1.0f);
		float fSlow41 = (synth_piano_faustpower2_f(fSlow17) * fSlow31);
		float fSlow42 = synth_piano_faustpower2_f(fSlow36);
		float fSlow43 = (fConst5 * (((fSlow22 + std::atan2((((0.0f - ((fSlow17 * fSlow34) / fSlow36)) * fSlow37) + (((fSlow17 * fSlow31) * fSlow40) / fSlow36)), ((fSlow37 * fSlow40) + ((fSlow41 * fSlow34) / fSlow42)))) + 6.28318548f) / fSlow15));
		float fSlow44 = std::floor(fSlow43);
		float fSlow45 = (fSlow43 - fSlow44);
		float fSlow46 = float(fButton0);
		int iSlow47 = (fSlow46 > 0.0f);
		float fSlow48 = float(iSlow47);
		float fSlow49 = float((0 - (iSlow47 + -1)));
		float fSlow50 = (fConst1 * ((0.999599993f * fSlow48) + (0.899999976f * (float(getValueReleaseLoopGain(float(fSlow3))) * fSlow49))));
		float fSlow51 = float(getValueDCBa1(float(fSlow3)));
		float fSlow52 = (1.0f - fSlow51);
		float fSlow53 = (0.5f * fSlow52);
		float fSlow54 = (0.25f * float(fHslider5));
		float fSlow55 = float(getValueLoudPole(float(fSlow3)));
		float fSlow56 = (float(getValueLoudGain(float(fSlow3))) * (fSlow54 + (0.980000019f - fSlow55)));
		float fSlow57 = (1.62981451e-09f * (fSlow56 * float((fSlow3 < 45.0f))));
		int iSlow58 = (iSlow47 > 0);
		float fSlow59 = std::exp((0.0f - (fConst7 / (float(getValueDryTapAmpT60(float(fSlow3))) * std::min<float>(1.0f, std::max<float>(0.0f, fSlow46))))));
		int iSlow60 = (iSlow47 < 1);
		float fSlow61 = (fConst8 * fSlow49);
		float fSlow62 = float(fHslider6);
		float fSlow63 = (fSlow48 * std::exp((0.0f - (fConst9 / fSlow62))));
		float fSlow64 = (fConst10 * fSlow62);
		float fSlow65 = (0.200000003f * float(getValueSustainPedalLevel(float(fSlow3))));
		float fSlow66 = (fSlow55 + (0.0199999996f - fSlow54));
		float fSlow67 = (0.0f - fSlow53);
		float fSlow68 = (3.70000005f * fSlow18);
		float fSlow69 = ((fSlow27 + fSlow33) - fSlow30);
		float fSlow70 = (((fSlow39 + (fSlow21 * fSlow69)) / fSlow36) + 1.0f);
		int iSlow71 = int((fConst5 * (((fSlow22 + std::atan2((0.0f - ((fSlow17 * ((fSlow69 * fSlow37) - (fSlow31 * fSlow70))) / fSlow36)), ((fSlow37 * fSlow70) + ((fSlow41 * fSlow69) / fSlow42)))) + 6.28318548f) / fSlow15)));
		int iSlow72 = std::min<int>(4097, std::max<int>(0, (iSlow71 + 1)));
		float fSlow73 = (1.0f / fSlow36);
		float fSlow74 = (fSlow13 - fSlow14);
		float fSlow75 = (fConst6 * fSlow74);
		float fSlow76 = std::sin(fSlow75);
		float fSlow77 = std::cos(fSlow75);
		float fSlow78 = (3.0f * std::atan2((fSlow12 * fSlow76), (fSlow19 + (fSlow20 * fSlow77))));
		float fSlow79 = (((fSlow77 * fSlow31) / fSlow36) + 1.0f);
		float fSlow80 = ((((fSlow77 * fSlow34) + fSlow39) / fSlow36) + 1.0f);
		float fSlow81 = (synth_piano_faustpower2_f(fSlow76) * fSlow31);
		float fSlow82 = (fConst5 * (((fSlow78 + std::atan2((((0.0f - ((fSlow76 * fSlow34) / fSlow36)) * fSlow79) + (((fSlow76 * fSlow31) * fSlow80) / fSlow36)), ((fSlow79 * fSlow80) + ((fSlow81 * fSlow34) / fSlow42)))) + 6.28318548f) / fSlow74));
		float fSlow83 = std::floor(fSlow82);
		float fSlow84 = (fSlow83 + (1.0f - fSlow82));
		float fSlow85 = (((fSlow39 + (fSlow77 * fSlow69)) / fSlow36) + 1.0f);
		int iSlow86 = int((fConst5 * (((fSlow78 + std::atan2((0.0f - ((fSlow76 * ((fSlow79 * fSlow69) - (fSlow31 * fSlow85))) / fSlow36)), ((fSlow79 * fSlow85) + ((fSlow81 * fSlow69) / fSlow42)))) + 6.28318548f) / fSlow74)));
		int iSlow87 = std::min<int>(4097, std::max<int>(0, iSlow86));
		float fSlow88 = (fSlow82 - fSlow83);
		int iSlow89 = std::min<int>(4097, std::max<int>(0, (iSlow86 + 1)));
		float fSlow90 = (fSlow44 + (1.0f - fSlow43));
		int iSlow91 = std::min<int>(4097, std::max<int>(0, iSlow71));
		float fSlow92 = ((0.0f - (fConst12 * (fSlow4 * fSlow5))) * std::cos((fConst13 * (fSlow4 / float(getValueStrikePosition(float(fSlow3)))))));
		float fSlow93 = (fConst14 * fSlow6);
		float fSlow94 = (fSlow4 + std::pow(2.0f, (0.0833333358f * (fSlow3 + -129.0f))));
		float fSlow95 = (440.0f * fSlow94);
		float fSlow96 = (fSlow14 + fSlow95);
		float fSlow97 = (fConst6 * fSlow96);
		float fSlow98 = std::sin(fSlow97);
		float fSlow99 = std::cos(fSlow97);
		float fSlow100 = (3.0f * std::atan2((fSlow12 * fSlow98), (fSlow19 + (fSlow20 * fSlow99))));
		float fSlow101 = (((fSlow99 * fSlow31) / fSlow36) + 1.0f);
		float fSlow102 = (((fSlow39 + (fSlow99 * fSlow34)) / fSlow36) + 1.0f);
		float fSlow103 = (synth_piano_faustpower2_f(fSlow98) * fSlow31);
		float fSlow104 = (fConst5 * (((fSlow100 + std::atan2((((0.0f - ((fSlow98 * fSlow34) / fSlow36)) * fSlow101) + (((fSlow98 * fSlow31) * fSlow102) / fSlow36)), ((fSlow101 * fSlow102) + ((fSlow103 * fSlow34) / fSlow42)))) + 6.28318548f) / fSlow96));
		float fSlow105 = std::floor(fSlow104);
		float fSlow106 = (fSlow104 - fSlow105);
		float fSlow107 = (1.16415322e-09f * (fSlow56 * float(((fSlow3 >= 45.0f) & (fSlow3 < 88.0f)))));
		float fSlow108 = (((fSlow39 + (fSlow99 * fSlow69)) / fSlow36) + 1.0f);
		int iSlow109 = int((fConst5 * (((fSlow100 + std::atan2((0.0f - ((fSlow98 * ((fSlow69 * fSlow101) - (fSlow31 * fSlow108))) / fSlow36)), ((fSlow101 * fSlow108) + ((fSlow103 * fSlow69) / fSlow42)))) + 6.28318548f) / fSlow96)));
		int iSlow110 = std::min<int>(4097, std::max<int>(0, (iSlow109 + 1)));
		float fSlow111 = (fSlow95 - fSlow14);
		float fSlow112 = (fConst6 * fSlow111);
		float fSlow113 = std::sin(fSlow112);
		float fSlow114 = std::cos(fSlow112);
		float fSlow115 = (3.0f * std::atan2((fSlow12 * fSlow113), (fSlow19 + (fSlow20 * fSlow114))));
		float fSlow116 = (((fSlow114 * fSlow31) / fSlow36) + 1.0f);
		float fSlow117 = (((fSlow39 + (fSlow114 * fSlow34)) / fSlow36) + 1.0f);
		float fSlow118 = (synth_piano_faustpower2_f(fSlow113) * fSlow31);
		float fSlow119 = (fConst5 * (((fSlow115 + std::atan2((((0.0f - ((fSlow113 * fSlow34) / fSlow36)) * fSlow116) + (((fSlow113 * fSlow31) * fSlow117) / fSlow36)), ((fSlow116 * fSlow117) + ((fSlow118 * fSlow34) / fSlow42)))) + 6.28318548f) / fSlow111));
		float fSlow120 = std::floor(fSlow119);
		float fSlow121 = (fSlow119 - fSlow120);
		float fSlow122 = (((fSlow39 + (fSlow114 * fSlow69)) / fSlow36) + 1.0f);
		int iSlow123 = int((fConst5 * (((fSlow115 + std::atan2((0.0f - ((fSlow113 * ((fSlow69 * fSlow116) - (fSlow31 * fSlow122))) / fSlow36)), ((fSlow116 * fSlow122) + ((fSlow118 * fSlow69) / fSlow42)))) + 6.28318548f) / fSlow111)));
		int iSlow124 = std::min<int>(4097, std::max<int>(0, (iSlow123 + 1)));
		float fSlow125 = (fConst13 * fSlow94);
		float fSlow126 = std::sin(fSlow125);
		float fSlow127 = std::cos(fSlow125);
		float fSlow128 = (3.0f * std::atan2((fSlow12 * fSlow126), (fSlow19 + (fSlow20 * fSlow127))));
		float fSlow129 = (((fSlow127 * fSlow31) / fSlow36) + 1.0f);
		float fSlow130 = (((fSlow39 + (fSlow127 * fSlow34)) / fSlow36) + 1.0f);
		float fSlow131 = (synth_piano_faustpower2_f(fSlow126) * fSlow31);
		float fSlow132 = (fConst15 * (((fSlow128 + std::atan2((((0.0f - ((fSlow126 * fSlow34) / fSlow36)) * fSlow129) + (((fSlow126 * fSlow31) * fSlow130) / fSlow36)), ((fSlow129 * fSlow130) + ((fSlow131 * fSlow34) / fSlow42)))) + 6.28318548f) / fSlow94));
		float fSlow133 = std::floor(fSlow132);
		float fSlow134 = (fSlow133 + (1.0f - fSlow132));
		float fSlow135 = (((fSlow39 + (fSlow127 * fSlow69)) / fSlow36) + 1.0f);
		int iSlow136 = int((fConst15 * (((fSlow128 + std::atan2((0.0f - ((fSlow126 * ((fSlow69 * fSlow129) - (fSlow31 * fSlow135))) / fSlow36)), ((fSlow129 * fSlow135) + ((fSlow131 * fSlow69) / fSlow42)))) + 6.28318548f) / fSlow94)));
		int iSlow137 = std::min<int>(4097, std::max<int>(0, iSlow136));
		float fSlow138 = (fSlow132 - fSlow133);
		int iSlow139 = std::min<int>(4097, std::max<int>(0, (iSlow136 + 1)));
		float fSlow140 = (fSlow120 + (1.0f - fSlow119));
		int iSlow141 = std::min<int>(4097, std::max<int>(0, iSlow123));
		float fSlow142 = (fSlow105 + (1.0f - fSlow104));
		int iSlow143 = std::min<int>(4097, std::max<int>(0, iSlow109));
		float fSlow144 = std::cos((fConst13 * fSlow4));
		float fSlow145 = std::pow(10.0f, (0.0500000007f * float(getValueSecondStageAmpRatio(float(fSlow3)))));
		float fSlow146 = std::pow(10.0f, (fConst16 * float(getValuer1_1db(float(fSlow3)))));
		float fSlow147 = std::pow(10.0f, (fConst16 * float(getValuer1_2db(float(fSlow3)))));
		float fSlow148 = (1.0f - fSlow145);
		float fSlow149 = (0.0f - (2.0f * ((fSlow145 * fSlow146) + (fSlow147 * fSlow148))));
		float fSlow150 = float(getValueBq4_gEarBalled(float(fSlow3)));
		float fSlow151 = (2.0f * fSlow150);
		float fSlow152 = float((fSlow3 >= 88.0f));
		float fSlow153 = (2.32830644e-10f * fSlow152);
		float fSlow154 = (1.16415322e-10f * fSlow152);
		float fSlow155 = std::pow(10.0f, (fConst16 * float(getValuer3db(float(fSlow3)))));
		float fSlow156 = (std::cos((fConst13 * (fSlow4 * float(getValueThirdPartialFactor(float(fSlow3)))))) * (0.0f - (2.0f * fSlow155)));
		float fSlow157 = synth_piano_faustpower2_f(fSlow155);
		float fSlow158 = std::pow(10.0f, (fConst16 * float(getValuer2db(float(fSlow3)))));
		float fSlow159 = (std::cos((fConst13 * (fSlow4 * float(getValueSecondPartialFactor(float(fSlow3)))))) * (0.0f - (2.0f * fSlow158)));
		float fSlow160 = synth_piano_faustpower2_f(fSlow158);
		float fSlow161 = (fSlow144 * (0.0f - (2.0f * fSlow146)));
		float fSlow162 = synth_piano_faustpower2_f(fSlow146);
		float fSlow163 = (0.0f - (2.0f * fSlow147));
		float fSlow164 = synth_piano_faustpower2_f(fSlow147);
		float fSlow165 = ((fSlow145 * fSlow162) + (fSlow164 * fSlow148));
		float fSlow166 = float(fHslider7);
		float fSlow167 = std::exp((fConst21 / fSlow166));
		float fSlow168 = synth_piano_faustpower2_f(fSlow167);
		float fSlow169 = (1.0f - (fConst18 * fSlow168));
		float fSlow170 = (1.0f - fSlow168);
		float fSlow171 = (fSlow169 / fSlow170);
		float fSlow172 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow169) / synth_piano_faustpower2_f(fSlow170)) + -1.0f)));
		float fSlow173 = (fSlow171 - fSlow172);
		float fSlow174 = (fSlow167 * (fSlow172 + (1.0f - fSlow171)));
		float fSlow175 = ((std::exp((fConst22 / fSlow166)) / fSlow167) + -1.0f);
		float fSlow176 = std::exp((fConst31 / fSlow166));
		float fSlow177 = synth_piano_faustpower2_f(fSlow176);
		float fSlow178 = (1.0f - (fConst18 * fSlow177));
		float fSlow179 = (1.0f - fSlow177);
		float fSlow180 = (fSlow178 / fSlow179);
		float fSlow181 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow178) / synth_piano_faustpower2_f(fSlow179)) + -1.0f)));
		float fSlow182 = (fSlow180 - fSlow181);
		float fSlow183 = (fSlow176 * (fSlow181 + (1.0f - fSlow180)));
		float fSlow184 = ((std::exp((fConst32 / fSlow166)) / fSlow176) + -1.0f);
		float fSlow185 = std::exp((fConst38 / fSlow166));
		float fSlow186 = synth_piano_faustpower2_f(fSlow185);
		float fSlow187 = (1.0f - (fConst18 * fSlow186));
		float fSlow188 = (1.0f - fSlow186);
		float fSlow189 = (fSlow187 / fSlow188);
		float fSlow190 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow187) / synth_piano_faustpower2_f(fSlow188)) + -1.0f)));
		float fSlow191 = (fSlow189 - fSlow190);
		float fSlow192 = (fSlow185 * (fSlow190 + (1.0f - fSlow189)));
		float fSlow193 = ((std::exp((fConst39 / fSlow166)) / fSlow185) + -1.0f);
		float fSlow194 = (12.0f * fSlow0);
		int iSlow195 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst42 * (float(fHslider8) / fSlow4)))));
		float fSlow196 = std::exp((fConst46 / fSlow166));
		float fSlow197 = synth_piano_faustpower2_f(fSlow196);
		float fSlow198 = (1.0f - (fConst18 * fSlow197));
		float fSlow199 = (1.0f - fSlow197);
		float fSlow200 = (fSlow198 / fSlow199);
		float fSlow201 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow198) / synth_piano_faustpower2_f(fSlow199)) + -1.0f)));
		float fSlow202 = (fSlow200 - fSlow201);
		float fSlow203 = (fSlow196 * (fSlow201 + (1.0f - fSlow200)));
		float fSlow204 = ((std::exp((fConst47 / fSlow166)) / fSlow196) + -1.0f);
		float fSlow205 = std::exp((fConst53 / fSlow166));
		float fSlow206 = synth_piano_faustpower2_f(fSlow205);
		float fSlow207 = (1.0f - (fConst18 * fSlow206));
		float fSlow208 = (1.0f - fSlow206);
		float fSlow209 = (fSlow207 / fSlow208);
		float fSlow210 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow207) / synth_piano_faustpower2_f(fSlow208)) + -1.0f)));
		float fSlow211 = (fSlow209 - fSlow210);
		float fSlow212 = (fSlow205 * (fSlow210 + (1.0f - fSlow209)));
		float fSlow213 = ((std::exp((fConst54 / fSlow166)) / fSlow205) + -1.0f);
		float fSlow214 = std::exp((fConst60 / fSlow166));
		float fSlow215 = synth_piano_faustpower2_f(fSlow214);
		float fSlow216 = (1.0f - (fConst18 * fSlow215));
		float fSlow217 = (1.0f - fSlow215);
		float fSlow218 = (fSlow216 / fSlow217);
		float fSlow219 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow216) / synth_piano_faustpower2_f(fSlow217)) + -1.0f)));
		float fSlow220 = (fSlow218 - fSlow219);
		float fSlow221 = (fSlow214 * (fSlow219 + (1.0f - fSlow218)));
		float fSlow222 = ((std::exp((fConst61 / fSlow166)) / fSlow214) + -1.0f);
		float fSlow223 = std::exp((fConst67 / fSlow166));
		float fSlow224 = synth_piano_faustpower2_f(fSlow223);
		float fSlow225 = (1.0f - (fConst18 * fSlow224));
		float fSlow226 = (1.0f - fSlow224);
		float fSlow227 = (fSlow225 / fSlow226);
		float fSlow228 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow225) / synth_piano_faustpower2_f(fSlow226)) + -1.0f)));
		float fSlow229 = (fSlow227 - fSlow228);
		float fSlow230 = (fSlow223 * (fSlow228 + (1.0f - fSlow227)));
		float fSlow231 = ((std::exp((fConst68 / fSlow166)) / fSlow223) + -1.0f);
		float fSlow232 = std::exp((fConst74 / fSlow166));
		float fSlow233 = synth_piano_faustpower2_f(fSlow232);
		float fSlow234 = (1.0f - (fConst18 * fSlow233));
		float fSlow235 = (1.0f - fSlow233);
		float fSlow236 = (fSlow234 / fSlow235);
		float fSlow237 = std::sqrt(std::max<float>(0.0f, ((synth_piano_faustpower2_f(fSlow234) / synth_piano_faustpower2_f(fSlow235)) + -1.0f)));
		float fSlow238 = (fSlow236 - fSlow237);
		float fSlow239 = (fSlow232 * (fSlow237 + (1.0f - fSlow236)));
		float fSlow240 = ((std::exp((fConst75 / fSlow166)) / fSlow232) + -1.0f);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			fRec10[0] = (fSlow2 + (fConst2 * fRec10[1]));
			fRec18[0] = (fSlow50 + (fConst2 * fRec18[1]));
			iRec24[0] = ((1103515245 * iRec24[1]) + 12345);
			iRec26[0] = ((iSlow47 * iRec26[1]) + 1);
			float fTemp0 = float((iRec26[0] + -1));
			float fTemp1 = float(((fTemp0 < 2.0f) & iSlow58));
			float fTemp2 = ((0.0301973838f * fTemp1) + (fSlow59 * float(((fTemp0 >= 2.0f) | iSlow60))));
			fRec25[0] = ((fRec25[1] * fTemp2) + (0.150000006f * (fTemp1 * (1.0f - fTemp2))));
			int iTemp3 = (fTemp0 < fSlow64);
			float fTemp4 = ((fSlow63 * float(iTemp3)) + (fConst11 * float((iSlow47 * (fTemp0 >= fSlow64)))));
			fRec27[0] = ((fRec27[1] * (fSlow61 + fTemp4)) + (fSlow65 * (((1.0f - fTemp4) - fSlow61) * float((iTemp3 & iSlow58)))));
			float fTemp5 = (float(iRec24[0]) * (fRec25[0] + fRec27[0]));
			fRec23[0] = ((fSlow57 * fTemp5) + (fSlow66 * fRec23[1]));
			fRec22[0] = ((fSlow56 * fRec23[0]) + (fSlow66 * fRec22[1]));
			fRec21[0] = ((fSlow56 * fRec22[0]) + (fSlow66 * fRec21[1]));
			fRec20[0] = ((fSlow56 * fRec21[0]) + (fSlow66 * fRec20[1]));
			fRec19[0] = (((fSlow53 * fRec20[0]) + (fSlow67 * fRec20[1])) - (fSlow51 * fRec19[1]));
			float fTemp6 = (fRec18[0] * (fRec19[0] + fRec12[1]));
			fVec0[0] = fTemp6;
			fRec17[0] = (fVec0[1] + (fSlow68 * (fTemp6 - fRec17[1])));
			fRec16[0] = (fRec17[1] + (fSlow68 * (fRec17[0] - fRec16[1])));
			fRec15[(IOTA & 8191)] = (fRec16[1] + (fSlow68 * (fRec16[0] - fRec15[((IOTA - 1) & 8191)])));
			float fTemp7 = (fSlow45 * fRec15[((IOTA - iSlow72) & 8191)]);
			float fTemp8 = (fRec19[0] + (fRec18[0] * fRec13[1]));
			fVec1[0] = fTemp8;
			fRec31[0] = (fVec1[1] + (fSlow68 * (fTemp8 - fRec31[1])));
			fRec30[0] = (fRec31[1] + (fSlow68 * (fRec31[0] - fRec30[1])));
			fRec29[(IOTA & 8191)] = (fRec30[1] + (fSlow68 * (fRec30[0] - fRec29[((IOTA - 1) & 8191)])));
			float fTemp9 = (fSlow84 * fRec29[((IOTA - iSlow87) & 8191)]);
			float fTemp10 = (fSlow88 * fRec29[((IOTA - iSlow89) & 8191)]);
			float fTemp11 = (fSlow90 * fRec15[((IOTA - iSlow91) & 8191)]);
			float fTemp12 = (fTemp7 + ((fTemp9 + fTemp10) + fTemp11));
			fVec2[0] = fTemp12;
			fRec28[0] = (fSlow73 * ((2.0f * ((fSlow38 * fTemp12) + (fSlow32 * fVec2[1]))) - (fSlow31 * fRec28[1])));
			fRec12[0] = (fTemp7 + (fRec28[0] + fTemp11));
			fRec13[0] = (fTemp10 + (fRec28[0] + fTemp9));
			float fRec14 = fTemp12;
			fRec11[0] = ((fSlow8 * fRec14) - ((fSlow92 * fRec11[1]) + (fSlow93 * fRec11[2])));
			fRec44[0] = ((fSlow107 * fTemp5) + (fSlow66 * fRec44[1]));
			fRec43[0] = ((fSlow56 * fRec44[0]) + (fSlow66 * fRec43[1]));
			fRec42[0] = ((fSlow56 * fRec43[0]) + (fSlow66 * fRec42[1]));
			fRec41[0] = ((fSlow56 * fRec42[0]) + (fSlow66 * fRec41[1]));
			fRec40[0] = (((fSlow53 * fRec41[0]) + (fSlow67 * fRec41[1])) - (fSlow51 * fRec40[1]));
			float fTemp13 = (fRec18[0] * (fRec40[0] + fRec33[1]));
			fVec3[0] = fTemp13;
			fRec39[0] = (fVec3[1] + (fSlow68 * (fTemp13 - fRec39[1])));
			fRec38[0] = (fRec39[1] + (fSlow68 * (fRec39[0] - fRec38[1])));
			fRec37[(IOTA & 8191)] = (fRec38[1] + (fSlow68 * (fRec38[0] - fRec37[((IOTA - 1) & 8191)])));
			float fTemp14 = (fSlow106 * fRec37[((IOTA - iSlow110) & 8191)]);
			float fTemp15 = (fRec40[0] + (fRec18[0] * fRec35[1]));
			fVec4[0] = fTemp15;
			fRec48[0] = (fVec4[1] + (fSlow68 * (fTemp15 - fRec48[1])));
			fRec47[0] = (fRec48[1] + (fSlow68 * (fRec48[0] - fRec47[1])));
			fRec46[(IOTA & 8191)] = (fRec47[1] + (fSlow68 * (fRec47[0] - fRec46[((IOTA - 1) & 8191)])));
			float fTemp16 = (fSlow121 * fRec46[((IOTA - iSlow124) & 8191)]);
			float fTemp17 = (fRec40[0] + (fRec18[0] * fRec34[1]));
			fVec5[0] = fTemp17;
			fRec51[0] = (fVec5[1] + (fSlow68 * (fTemp17 - fRec51[1])));
			fRec50[0] = (fRec51[1] + (fSlow68 * (fRec51[0] - fRec50[1])));
			fRec49[(IOTA & 8191)] = (fRec50[1] + (fSlow68 * (fRec50[0] - fRec49[((IOTA - 1) & 8191)])));
			float fTemp18 = (fSlow134 * fRec49[((IOTA - iSlow137) & 8191)]);
			float fTemp19 = (fSlow138 * fRec49[((IOTA - iSlow139) & 8191)]);
			float fTemp20 = (fSlow140 * fRec46[((IOTA - iSlow141) & 8191)]);
			float fTemp21 = (fSlow142 * fRec37[((IOTA - iSlow143) & 8191)]);
			float fTemp22 = (fTemp14 + ((fTemp16 + ((fTemp18 + fTemp19) + fTemp20)) + fTemp21));
			fVec6[0] = fTemp22;
			fRec45[0] = (fSlow73 * ((2.0f * ((fSlow38 * fTemp22) + (fSlow32 * fVec6[1]))) - (fSlow31 * fRec45[1])));
			fRec33[0] = (fTemp14 + (fRec45[0] + fTemp21));
			fRec34[0] = (fTemp19 + (fRec45[0] + fTemp18));
			fRec35[0] = (fTemp16 + (fRec45[0] + fTemp20));
			float fRec36 = fTemp22;
			fRec32[0] = ((fSlow8 * fRec36) - ((fSlow92 * fRec32[1]) + (fSlow93 * fRec32[2])));
			fVec7[0] = (fSlow153 * fTemp5);
			float fTemp23 = (0.0f - ((0.5f * fVec7[1]) + (fSlow154 * fTemp5)));
			fVec8[0] = fTemp23;
			fRec61[0] = (((fSlow53 * fTemp23) + (fSlow67 * fVec8[1])) - (fSlow51 * fRec61[1]));
			fRec60[0] = ((fSlow56 * fRec61[0]) + (fSlow66 * fRec60[1]));
			fRec59[0] = ((fSlow56 * fRec60[0]) + (fSlow66 * fRec59[1]));
			fRec58[0] = ((fSlow56 * fRec59[0]) + (fSlow66 * fRec58[1]));
			fRec57[0] = ((fSlow56 * fRec58[0]) + (fSlow66 * fRec57[1]));
			fRec56[0] = ((fSlow150 * (fRec57[0] - fRec57[1])) - ((fSlow156 * fRec56[1]) + (fSlow157 * fRec56[2])));
			fRec55[0] = ((fSlow151 * fRec56[0]) - ((fSlow159 * fRec55[1]) + (fSlow160 * fRec55[2])));
			fRec54[0] = (fRec55[0] - ((fSlow161 * fRec54[1]) + (fSlow162 * fRec54[2])));
			fRec53[0] = (((fSlow144 * ((fSlow149 * fRec54[1]) - (fSlow163 * fRec53[1]))) + (fRec54[0] + (fSlow165 * fRec54[2]))) - (fSlow164 * fRec53[2]));
			fRec52[0] = ((fSlow52 * fRec53[0]) - (fSlow51 * fRec52[1]));
			float fTemp24 = ((fSlow7 * ((fRec11[0] - fRec11[2]) + (fRec32[0] - fRec32[2]))) + (fRec52[0] + (fRec14 + fRec36)));
			fVec9[(IOTA & 4095)] = fTemp24;
			fVec10[(IOTA & 4095)] = (fSlow1 * (fRec10[0] * fTemp24));
			float fTemp25 = (0.300000012f * fVec10[((IOTA - iConst17) & 4095)]);
			fRec63[0] = (0.0f - (fConst24 * ((fConst25 * fRec63[1]) - (fRec4[1] + fRec4[2]))));
			fRec62[0] = ((fSlow173 * fRec62[1]) + (fSlow174 * (fRec4[1] + (fSlow175 * fRec63[0]))));
			fVec11[(IOTA & 16383)] = ((0.353553385f * fRec62[0]) + 9.99999968e-21f);
			float fTemp26 = ((fTemp25 + fVec11[((IOTA - iConst27) & 16383)]) - (0.600000024f * fRec8[1]));
			fVec12[(IOTA & 4095)] = fTemp26;
			fRec8[0] = fVec12[((IOTA - iConst28) & 4095)];
			float fRec9 = (0.600000024f * fTemp26);
			fRec67[0] = (0.0f - (fConst24 * ((fConst25 * fRec67[1]) - (fRec0[1] + fRec0[2]))));
			fRec66[0] = ((fSlow182 * fRec66[1]) + (fSlow183 * (fRec0[1] + (fSlow184 * fRec67[0]))));
			fVec13[(IOTA & 16383)] = ((0.353553385f * fRec66[0]) + 9.99999968e-21f);
			float fTemp27 = ((fVec13[((IOTA - iConst34) & 16383)] + fTemp25) - (0.600000024f * fRec64[1]));
			fVec14[(IOTA & 2047)] = fTemp27;
			fRec64[0] = fVec14[((IOTA - iConst35) & 2047)];
			float fRec65 = (0.600000024f * fTemp27);
			fRec71[0] = (0.0f - (fConst24 * ((fConst25 * fRec71[1]) - (fRec7[1] + fRec7[2]))));
			fRec70[0] = ((fSlow191 * fRec70[1]) + (fSlow192 * (fRec7[1] + (fSlow193 * fRec71[0]))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec70[0]) + 9.99999968e-21f);
			float fTemp28 = fVec9[((IOTA - iSlow195) & 4095)];
			fVec16[(IOTA & 4095)] = (fSlow194 * (fRec10[0] * fTemp28));
			float fTemp29 = (0.300000012f * fVec16[((IOTA - iConst17) & 4095)]);
			float fTemp30 = (((0.600000024f * fRec68[1]) + fVec15[((IOTA - iConst41) & 32767)]) - fTemp29);
			fVec17[(IOTA & 2047)] = fTemp30;
			fRec68[0] = fVec17[((IOTA - iConst43) & 2047)];
			float fRec69 = (0.0f - (0.600000024f * fTemp30));
			fRec75[0] = (0.0f - (fConst24 * ((fConst25 * fRec75[1]) - (fRec3[1] + fRec3[2]))));
			fRec74[0] = ((fSlow202 * fRec74[1]) + (fSlow203 * (fRec3[1] + (fSlow204 * fRec75[0]))));
			fVec18[(IOTA & 32767)] = ((0.353553385f * fRec74[0]) + 9.99999968e-21f);
			float fTemp31 = (((0.600000024f * fRec72[1]) + fVec18[((IOTA - iConst49) & 32767)]) - fTemp29);
			fVec19[(IOTA & 4095)] = fTemp31;
			fRec72[0] = fVec19[((IOTA - iConst50) & 4095)];
			float fRec73 = (0.0f - (0.600000024f * fTemp31));
			fRec79[0] = (0.0f - (fConst24 * ((fConst25 * fRec79[1]) - (fRec5[1] + fRec5[2]))));
			fRec78[0] = ((fSlow211 * fRec78[1]) + (fSlow212 * (fRec5[1] + (fSlow213 * fRec79[0]))));
			fVec20[(IOTA & 16383)] = ((0.353553385f * fRec78[0]) + 9.99999968e-21f);
			float fTemp32 = (fVec20[((IOTA - iConst56) & 16383)] + (fTemp29 + (0.600000024f * fRec76[1])));
			fVec21[(IOTA & 4095)] = fTemp32;
			fRec76[0] = fVec21[((IOTA - iConst57) & 4095)];
			float fRec77 = (0.0f - (0.600000024f * fTemp32));
			fRec83[0] = (0.0f - (fConst24 * ((fConst25 * fRec83[1]) - (fRec1[1] + fRec1[2]))));
			fRec82[0] = ((fSlow220 * fRec82[1]) + (fSlow221 * (fRec1[1] + (fSlow222 * fRec83[0]))));
			fVec22[(IOTA & 32767)] = ((0.353553385f * fRec82[0]) + 9.99999968e-21f);
			float fTemp33 = (fTemp29 + ((0.600000024f * fRec80[1]) + fVec22[((IOTA - iConst63) & 32767)]));
			fVec23[(IOTA & 4095)] = fTemp33;
			fRec80[0] = fVec23[((IOTA - iConst64) & 4095)];
			float fRec81 = (0.0f - (0.600000024f * fTemp33));
			fRec87[0] = (0.0f - (fConst24 * ((fConst25 * fRec87[1]) - (fRec6[1] + fRec6[2]))));
			fRec86[0] = ((fSlow229 * fRec86[1]) + (fSlow230 * (fRec6[1] + (fSlow231 * fRec87[0]))));
			fVec24[(IOTA & 16383)] = ((0.353553385f * fRec86[0]) + 9.99999968e-21f);
			float fTemp34 = (fVec24[((IOTA - iConst70) & 16383)] - (fTemp25 + (0.600000024f * fRec84[1])));
			fVec25[(IOTA & 2047)] = fTemp34;
			fRec84[0] = fVec25[((IOTA - iConst71) & 2047)];
			float fRec85 = (0.600000024f * fTemp34);
			fRec91[0] = (0.0f - (fConst24 * ((fConst25 * fRec91[1]) - (fRec2[1] + fRec2[2]))));
			fRec90[0] = ((fSlow238 * fRec90[1]) + (fSlow239 * (fRec2[1] + (fSlow240 * fRec91[0]))));
			fVec26[(IOTA & 16383)] = ((0.353553385f * fRec90[0]) + 9.99999968e-21f);
			float fTemp35 = (fVec26[((IOTA - iConst77) & 16383)] - (fTemp25 + (0.600000024f * fRec88[1])));
			fVec27[(IOTA & 4095)] = fTemp35;
			fRec88[0] = fVec27[((IOTA - iConst78) & 4095)];
			float fRec89 = (0.600000024f * fTemp35);
			float fTemp36 = (fRec65 + fRec9);
			float fTemp37 = (fRec85 + (fRec89 + fTemp36));
			fRec0[0] = (fRec8[1] + (fRec64[1] + (fRec68[1] + (fRec72[1] + (fRec76[1] + (fRec80[1] + (fRec84[1] + (fRec88[1] + (fRec69 + (fRec73 + (fRec77 + (fRec81 + fTemp37))))))))))));
			fRec1[0] = ((fRec8[1] + (fRec64[1] + (fRec84[1] + (fRec88[1] + fTemp37)))) - (fRec68[1] + (fRec72[1] + (fRec76[1] + (fRec80[1] + (fRec69 + (fRec73 + (fRec81 + fRec77))))))));
			float fTemp38 = (fRec89 + fRec85);
			fRec2[0] = ((fRec8[1] + (fRec64[1] + (fRec76[1] + (fRec80[1] + (fRec77 + (fRec81 + fTemp36)))))) - (fRec68[1] + (fRec72[1] + (fRec84[1] + (fRec88[1] + (fRec69 + (fRec73 + fTemp38)))))));
			fRec3[0] = ((fRec8[1] + (fRec64[1] + (fRec68[1] + (fRec72[1] + (fRec69 + (fRec73 + fTemp36)))))) - (fRec76[1] + (fRec80[1] + (fRec84[1] + (fRec88[1] + (fRec77 + (fRec81 + fTemp38)))))));
			float fTemp39 = (fRec65 + fRec89);
			float fTemp40 = (fRec9 + fRec85);
			fRec4[0] = ((fRec64[1] + (fRec72[1] + (fRec80[1] + (fRec88[1] + (fRec73 + (fRec81 + fTemp39)))))) - (fRec8[1] + (fRec68[1] + (fRec76[1] + (fRec84[1] + (fRec69 + (fRec77 + fTemp40)))))));
			fRec5[0] = ((fRec64[1] + (fRec68[1] + (fRec76[1] + (fRec88[1] + (fRec69 + (fRec77 + fTemp39)))))) - (fRec8[1] + (fRec72[1] + (fRec80[1] + (fRec84[1] + (fRec73 + (fRec81 + fTemp40)))))));
			float fTemp41 = (fRec65 + fRec85);
			float fTemp42 = (fRec9 + fRec89);
			fRec6[0] = ((fRec64[1] + (fRec68[1] + (fRec80[1] + (fRec84[1] + (fRec69 + (fRec81 + fTemp41)))))) - (fRec8[1] + (fRec72[1] + (fRec76[1] + (fRec88[1] + (fRec73 + (fRec77 + fTemp42)))))));
			fRec7[0] = ((fRec64[1] + (fRec72[1] + (fRec76[1] + (fRec84[1] + (fRec73 + (fRec77 + fTemp41)))))) - (fRec8[1] + (fRec68[1] + (fRec80[1] + (fRec88[1] + (fRec69 + (fRec81 + fTemp42)))))));
			float fTemp43 = (1.0f - fRec10[0]);
			output0[i0] = FAUSTFLOAT(((0.370000005f * (fRec1[0] + fRec2[0])) + (fSlow1 * (fTemp43 * fTemp24))));
			output1[i0] = FAUSTFLOAT(((0.370000005f * (fRec1[0] - fRec2[0])) + (fSlow194 * (fTemp43 * fTemp28))));
			fRec10[1] = fRec10[0];
			fRec18[1] = fRec18[0];
			iRec24[1] = iRec24[0];
			iRec26[1] = iRec26[0];
			fRec25[1] = fRec25[0];
			fRec27[1] = fRec27[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fVec0[1] = fVec0[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			IOTA = (IOTA + 1);
			fVec1[1] = fVec1[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fVec2[1] = fVec2[0];
			fRec28[1] = fRec28[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fVec3[1] = fVec3[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fVec4[1] = fVec4[0];
			fRec48[1] = fRec48[0];
			fRec47[1] = fRec47[0];
			fVec5[1] = fVec5[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fVec6[1] = fVec6[0];
			fRec45[1] = fRec45[0];
			fRec33[1] = fRec33[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fVec7[1] = fVec7[0];
			fVec8[1] = fVec8[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec8[1] = fRec8[0];
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			fRec64[1] = fRec64[0];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec68[1] = fRec68[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec72[1] = fRec72[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec76[1] = fRec76[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec80[1] = fRec80[0];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec84[1] = fRec84[0];
			fRec91[1] = fRec91[0];
			fRec90[1] = fRec90[0];
			fRec88[1] = fRec88[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _synth_piano_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_piano_UI<T>::name(sym(synth_piano));

typedef _synth_piano_UI<synth_piano> synth_piano_UI;

class faust_synth_piano_tilde : public FaustExternal<synth_piano, synth_piano_UI> {
public:
    faust_synth_piano_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
