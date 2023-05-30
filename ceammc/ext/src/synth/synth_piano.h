/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.piano"
version: "1.0"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_piano -scn synth_piano_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_piano_H__
#define  __synth_piano_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_piano_dsp.h ********************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***************************************************************************/

#ifndef __export__
#define __export__

#define FAUSTVERSION "2.53.1"

// Use FAUST_API for code that is part of the external API but is also compiled in faust and libfaust
// Use LIBFAUST_API for code that is compiled in faust and libfaust

#ifdef _WIN32
    #pragma warning (disable: 4251)
    #ifdef FAUST_EXE
        #define FAUST_API
        #define LIBFAUST_API
    #elif FAUST_LIB
        #define FAUST_API __declspec(dllexport)
        #define LIBFAUST_API __declspec(dllexport)
    #else
        #define FAUST_API
        #define LIBFAUST_API 
    #endif
#else
    #ifdef FAUST_EXE
        #define FAUST_API
        #define LIBFAUST_API
    #else
        #define FAUST_API __attribute__((visibility("default")))
        #define LIBFAUST_API __attribute__((visibility("default")))
    #endif
#endif

#endif

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct FAUST_API UI;
struct FAUST_API Meta;

/**
 * DSP memory manager.
 */

struct FAUST_API dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    /**
     * Inform the Memory Manager with the number of expected memory zones.
     * @param count - the number of expected memory zones
     */
    virtual void begin(size_t /*count*/) {}
    
    /**
     * Give the Memory Manager information on a given memory zone.
     * @param size - the size in bytes of the memory zone
     * @param reads - the number of Read access to the zone used to compute one frame
     * @param writes - the number of Write access to the zone used to compute one frame
     */
    virtual void info(size_t /*size*/, size_t /*reads*/, size_t /*writes*/) {}

    /**
     * Inform the Memory Manager that all memory zones have been described,
     * to possibly start a 'compute the best allocation strategy' step.
     */
    virtual void end() {}
    
    /**
     * Allocate a memory zone.
     * @param size - the memory zone size in bytes
     */
    virtual void* allocate(size_t size) = 0;
    
    /**
     * Destroy a memory zone.
     * @param ptr - the memory zone pointer to be deallocated
     */
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class FAUST_API synth_piano_dsp {

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

class FAUST_API decorator_dsp : public synth_piano_dsp {

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

class FAUST_API dsp_factory {
    
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

class FAUST_API ScopedNoDenormals {
    
    private:
    
        intptr_t fpsr = 0;
        
        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            // The volatile keyword here is needed to workaround a bug in AppleClang 13.0
            // which aggressively optimises away the variable otherwise
            volatile uint32_t fpsr_w = static_cast<uint32_t>(fpsr_aux);
            _mm_setcsr(fpsr_w);
        #endif
        }
        
        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined (__SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }
    
    public:
    
        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #elif defined (__SSE__)
        #if defined (__SSE2__)
            intptr_t mask = 0x8040;
        #else
            intptr_t mask = 0x8000;
        #endif
        #else
            intptr_t mask = 0x0000;
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }
        
        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals ftz_scope;

#endif

/************************** END synth_piano_dsp.h **************************/
/************************** BEGIN misc.h *******************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
***************************************************************************/

#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <string>

/************************** BEGIN meta.h *******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
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
struct FAUST_API Meta {
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/

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

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_piano
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

static float synth_piano_faustpower2_f(float value) {
	return value * value;
}

class synth_piano : public synth_piano_dsp {
	
 private:
	
	int fSampleRate;
	float fConst3;
	FAUSTFLOAT fHslider0;
	float fConst4;
	float fConst5;
	float fConst7;
	float fConst8;
	float fRec11[2];
	float fRec10[2];
	int IOTA0;
	float fVec0[16384];
	int iConst10;
	FAUSTFLOAT fHslider1;
	float fConst11;
	FAUSTFLOAT fHslider2;
	float fConst12;
	float fRec12[2];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	int iRec23[2];
	FAUSTFLOAT fButton0;
	int iRec25[2];
	float fConst13;
	float fRec24[2];
	float fConst14;
	float fConst15;
	FAUSTFLOAT fHslider5;
	float fConst16;
	float fConst17;
	float fRec26[2];
	float fVec1[2];
	float fVec2[2];
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	float fRec18[2];
	float fConst18;
	float fConst19;
	float fRec17[3];
	float fRec16[3];
	float fRec15[3];
	float fRec14[3];
	float fRec13[2];
	float fConst20;
	FAUSTFLOAT fHslider6;
	float fConst21;
	FAUSTFLOAT fHslider7;
	float fRec38[2];
	float fRec37[2];
	float fRec36[2];
	float fRec35[2];
	float fRec34[2];
	float fRec39[2];
	float fVec3[2];
	float fRec33[2];
	float fRec32[2];
	float fRec31[8192];
	float fVec4[2];
	float fRec42[2];
	float fRec41[2];
	float fRec40[8192];
	float fVec5[2];
	float fRec30[2];
	float fRec27[2];
	float fRec28[2];
	float fConst22;
	float fRec55[2];
	float fRec54[2];
	float fRec53[2];
	float fRec52[2];
	float fRec51[2];
	float fVec6[2];
	float fRec50[2];
	float fRec49[2];
	float fRec48[8192];
	float fVec7[2];
	float fRec58[2];
	float fRec57[2];
	float fRec56[8192];
	float fVec8[2];
	float fRec61[2];
	float fRec60[2];
	float fRec59[8192];
	float fVec9[2];
	float fRec47[2];
	float fRec43[2];
	float fRec44[2];
	float fRec45[2];
	float fConst24;
	float fConst25;
	float fConst26;
	float fRec62[3];
	float fRec63[3];
	float fVec10[4096];
	float fVec11[4096];
	int iConst27;
	float fVec12[2048];
	int iConst28;
	float fRec8[2];
	float fConst31;
	float fConst32;
	float fRec67[2];
	float fRec66[2];
	float fVec13[32768];
	int iConst34;
	float fConst35;
	FAUSTFLOAT fHslider8;
	float fVec14[4096];
	float fVec15[4096];
	int iConst36;
	float fRec64[2];
	float fConst39;
	float fConst40;
	float fRec71[2];
	float fRec70[2];
	float fVec16[16384];
	int iConst42;
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
	float fVec22[16384];
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
	float fVec25[4096];
	int iConst71;
	float fRec84[2];
	float fConst74;
	float fConst75;
	float fRec91[2];
	float fRec90[2];
	float fVec26[32768];
	int iConst77;
	float fVec27[2048];
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
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.4");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_piano -scn synth_piano_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("copyright", "Romain Michon");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("description", "WaveGuide Commuted Piano");
		m->declare("filename", "synth_piano.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
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
		m->declare("noises.lib/version", "0.4");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
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
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::floor(0.153129f * fConst0 + 0.5f);
		float fConst2 = (0.0f - 6.9077554f * fConst1) / fConst0;
		fConst3 = 0.25f * fConst2;
		fConst4 = std::cos(37699.113f / fConst0);
		fConst5 = 0.16666667f * fConst2;
		float fConst6 = 1.0f / std::tan(628.31854f / fConst0);
		fConst7 = 1.0f / (fConst6 + 1.0f);
		fConst8 = 1.0f - fConst6;
		float fConst9 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst10 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst1 - fConst9)));
		fConst11 = 44.1f / fConst0;
		fConst12 = 1.0f - fConst11;
		fConst13 = 7.0f / fConst0;
		fConst14 = std::exp(0.0f - 5.0f / fConst0);
		fConst15 = 1e+01f / fConst0;
		fConst16 = 0.1f * fConst0;
		fConst17 = std::exp(0.0f - 0.5f / fConst0);
		fConst18 = 0.05f / fConst0;
		fConst19 = 2764.6016f / fConst0;
		fConst20 = 0.15915494f * fConst0;
		fConst21 = 6.2831855f / fConst0;
		fConst22 = 0.0003617158f * fConst0;
		float fConst23 = synth_piano_faustpower2_f(fConst0);
		fConst24 = 9.68e+04f / fConst23;
		fConst25 = 8.8e+02f / fConst0;
		fConst26 = 1.936e+05f / fConst23;
		iConst27 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.02f * fConst0)));
		iConst28 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst9 + -1.0f)));
		float fConst29 = std::floor(0.210389f * fConst0 + 0.5f);
		float fConst30 = (0.0f - 6.9077554f * fConst29) / fConst0;
		fConst31 = 0.25f * fConst30;
		fConst32 = 0.16666667f * fConst30;
		float fConst33 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst34 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst29 - fConst33)));
		fConst35 = 0.0011363636f * fConst0;
		iConst36 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst33 + -1.0f)));
		float fConst37 = std::floor(0.125f * fConst0 + 0.5f);
		float fConst38 = (0.0f - 6.9077554f * fConst37) / fConst0;
		fConst39 = 0.25f * fConst38;
		fConst40 = 0.16666667f * fConst38;
		float fConst41 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst42 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst37 - fConst41)));
		iConst43 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst41 + -1.0f)));
		float fConst44 = std::floor(0.256891f * fConst0 + 0.5f);
		float fConst45 = (0.0f - 6.9077554f * fConst44) / fConst0;
		fConst46 = 0.25f * fConst45;
		fConst47 = 0.16666667f * fConst45;
		float fConst48 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst49 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst44 - fConst48)));
		iConst50 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst48 + -1.0f)));
		float fConst51 = std::floor(0.192303f * fConst0 + 0.5f);
		float fConst52 = (0.0f - 6.9077554f * fConst51) / fConst0;
		fConst53 = 0.25f * fConst52;
		fConst54 = 0.16666667f * fConst52;
		float fConst55 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst56 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst51 - fConst55)));
		iConst57 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst55 + -1.0f)));
		float fConst58 = std::floor(0.127837f * fConst0 + 0.5f);
		float fConst59 = (0.0f - 6.9077554f * fConst58) / fConst0;
		fConst60 = 0.25f * fConst59;
		fConst61 = 0.16666667f * fConst59;
		float fConst62 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst63 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst58 - fConst62)));
		iConst64 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst62 + -1.0f)));
		float fConst65 = std::floor(0.174713f * fConst0 + 0.5f);
		float fConst66 = (0.0f - 6.9077554f * fConst65) / fConst0;
		fConst67 = 0.25f * fConst66;
		fConst68 = 0.16666667f * fConst66;
		float fConst69 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst70 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst65 - fConst69)));
		iConst71 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst69 + -1.0f)));
		float fConst72 = std::floor(0.219991f * fConst0 + 0.5f);
		float fConst73 = (0.0f - 6.9077554f * fConst72) / fConst0;
		fConst74 = 0.25f * fConst73;
		fConst75 = 0.16666667f * fConst73;
		float fConst76 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst77 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst72 - fConst76)));
		iConst78 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst76 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.36f);
		fHslider1 = FAUSTFLOAT(0.6f);
		fHslider2 = FAUSTFLOAT(0.137f);
		fHslider3 = FAUSTFLOAT(48.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.1f);
		fHslider6 = FAUSTFLOAT(0.28f);
		fHslider7 = FAUSTFLOAT(0.1f);
		fHslider8 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec11[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec10[l1] = 0.0f;
		}
		IOTA0 = 0;
		for (int l2 = 0; l2 < 16384; l2 = l2 + 1) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec12[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec23[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec25[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec24[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec26[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fVec1[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fVec2[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec22[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec21[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec20[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec19[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec18[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fRec17[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
			fRec16[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec14[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec13[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec38[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec37[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec36[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec35[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec34[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec39[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fVec3[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec33[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec32[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 8192; l29 = l29 + 1) {
			fRec31[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fVec4[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec42[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec41[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 8192; l33 = l33 + 1) {
			fRec40[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fVec5[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec30[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec27[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec28[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec55[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec54[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec53[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec52[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec51[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fVec6[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec50[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec49[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 8192; l46 = l46 + 1) {
			fRec48[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fVec7[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec58[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec57[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 8192; l50 = l50 + 1) {
			fRec56[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fVec8[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec61[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec60[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 8192; l54 = l54 + 1) {
			fRec59[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fVec9[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec47[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec43[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec44[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec45[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec62[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 3; l61 = l61 + 1) {
			fRec63[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 4096; l62 = l62 + 1) {
			fVec10[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 4096; l63 = l63 + 1) {
			fVec11[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2048; l64 = l64 + 1) {
			fVec12[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec8[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec67[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec66[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 32768; l68 = l68 + 1) {
			fVec13[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 4096; l69 = l69 + 1) {
			fVec14[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 4096; l70 = l70 + 1) {
			fVec15[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec64[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec71[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec70[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 16384; l74 = l74 + 1) {
			fVec16[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2048; l75 = l75 + 1) {
			fVec17[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec68[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec75[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec74[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 32768; l79 = l79 + 1) {
			fVec18[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 4096; l80 = l80 + 1) {
			fVec19[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec72[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec79[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec78[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 16384; l84 = l84 + 1) {
			fVec20[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 4096; l85 = l85 + 1) {
			fVec21[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec76[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec83[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec82[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 16384; l89 = l89 + 1) {
			fVec22[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 4096; l90 = l90 + 1) {
			fVec23[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			fRec80[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec87[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec86[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 16384; l94 = l94 + 1) {
			fVec24[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 4096; l95 = l95 + 1) {
			fVec25[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec84[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec91[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec90[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 32768; l99 = l99 + 1) {
			fVec26[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2048; l100 = l100 + 1) {
			fVec27[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec88[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 3; l102 = l102 + 1) {
			fRec0[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 3; l103 = l103 + 1) {
			fRec1[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 3; l104 = l104 + 1) {
			fRec2[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 3; l105 = l105 + 1) {
			fRec3[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 3; l106 = l106 + 1) {
			fRec4[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 3; l107 = l107 + 1) {
			fRec5[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 3; l108 = l108 + 1) {
			fRec6[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 3; l109 = l109 + 1) {
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
		ui_interface->addHorizontalSlider("brightness", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("detuning", &fHslider7, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider5, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pan", &fHslider1, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pitch", &fHslider3, FAUSTFLOAT(48.0f), FAUSTFLOAT(21.0f), FAUSTFLOAT(108.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, FAUSTFLOAT(0.137f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("room_size", &fHslider0, FAUSTFLOAT(0.36f), FAUSTFLOAT(0.005f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("stiffness", &fHslider6, FAUSTFLOAT(0.28f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("width", &fHslider8, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::exp(fConst3 / fSlow0);
		float fSlow2 = synth_piano_faustpower2_f(fSlow1);
		float fSlow3 = 1.0f - fConst4 * fSlow2;
		float fSlow4 = 1.0f - fSlow2;
		float fSlow5 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow3) / synth_piano_faustpower2_f(fSlow4) + -1.0f));
		float fSlow6 = fSlow3 / fSlow4;
		float fSlow7 = fSlow1 * (fSlow5 + (1.0f - fSlow6));
		float fSlow8 = std::exp(fConst5 / fSlow0) / fSlow1 + -1.0f;
		float fSlow9 = fSlow6 - fSlow5;
		float fSlow10 = float(fHslider1);
		float fSlow11 = 12.0f * (1.0f - fSlow10);
		float fSlow12 = fConst11 * float(fHslider2);
		float fSlow13 = float(fHslider3);
		float fSlow14 = getValueDCBa1(fSlow13);
		float fSlow15 = 1.0f - fSlow14;
		float fSlow16 = 2.0f * getValueBq4_gEarBalled(fSlow13);
		float fSlow17 = 0.25f * float(fHslider4);
		float fSlow18 = getValueLoudPole(fSlow13);
		float fSlow19 = getValueLoudGain(fSlow13) * (fSlow17 + (0.98f - fSlow18));
		float fSlow20 = 0.5f * fSlow15;
		float fSlow21 = float(fSlow13 >= 88.0f);
		float fSlow22 = 2.3283064e-10f * fSlow21;
		float fSlow23 = float(fButton0);
		int iSlow24 = fSlow23 > 0.0f;
		int iSlow25 = iSlow24 > 0;
		float fSlow26 = std::exp(0.0f - fConst13 / (getValueDryTapAmpT60(fSlow13) * std::min<float>(1.0f, std::max<float>(0.0f, fSlow23))));
		int iSlow27 = iSlow24 < 1;
		float fSlow28 = float(0 - (iSlow24 + -1));
		float fSlow29 = fConst14 * fSlow28;
		float fSlow30 = float(iSlow24);
		float fSlow31 = float(fHslider5);
		float fSlow32 = fSlow30 * std::exp(0.0f - fConst15 / fSlow31);
		float fSlow33 = fConst16 * fSlow31;
		float fSlow34 = 0.2f * getValueSustainPedalLevel(fSlow13);
		float fSlow35 = 1.1641532e-10f * fSlow21;
		float fSlow36 = 0.0f - fSlow20;
		float fSlow37 = fSlow18 + (0.02f - fSlow17);
		float fSlow38 = std::pow(1e+01f, fConst18 * getValuer3db(fSlow13));
		float fSlow39 = std::pow(2.0f, 0.083333336f * (fSlow13 + -69.0f));
		float fSlow40 = (0.0f - 2.0f * fSlow38) * std::cos(fConst19 * fSlow39 * getValueThirdPartialFactor(fSlow13));
		float fSlow41 = synth_piano_faustpower2_f(fSlow38);
		float fSlow42 = std::pow(1e+01f, fConst18 * getValuer2db(fSlow13));
		float fSlow43 = (0.0f - 2.0f * fSlow42) * std::cos(fConst19 * fSlow39 * getValueSecondPartialFactor(fSlow13));
		float fSlow44 = synth_piano_faustpower2_f(fSlow42);
		float fSlow45 = std::pow(1e+01f, fConst18 * getValuer1_1db(fSlow13));
		float fSlow46 = std::cos(fConst19 * fSlow39);
		float fSlow47 = (0.0f - 2.0f * fSlow45) * fSlow46;
		float fSlow48 = synth_piano_faustpower2_f(fSlow45);
		float fSlow49 = std::pow(1e+01f, 0.05f * getValueSecondStageAmpRatio(fSlow13));
		float fSlow50 = 1.0f - fSlow49;
		float fSlow51 = std::pow(1e+01f, fConst18 * getValuer1_2db(fSlow13));
		float fSlow52 = synth_piano_faustpower2_f(fSlow51);
		float fSlow53 = fSlow49 * fSlow48 + fSlow50 * fSlow52;
		float fSlow54 = 0.0f - 2.0f * (fSlow49 * fSlow45 + fSlow50 * fSlow51);
		float fSlow55 = 0.0f - 2.0f * fSlow51;
		float fSlow56 = getValueSingleStringZero(fSlow13);
		float fSlow57 = 1.0f - fSlow56;
		float fSlow58 = getValueSingleStringPole(fSlow13);
		float fSlow59 = 1.0f - fSlow58;
		float fSlow60 = std::pow(1e+01f, 0.000113636364f * (getValueSingleStringDecayRate(fSlow13) / fSlow39));
		float fSlow61 = fSlow59 * fSlow60;
		float fSlow62 = 3.0f * fSlow57 - fSlow61;
		float fSlow63 = 1.0f / fSlow62;
		float fSlow64 = fSlow56 + fSlow61 + -1.0f;
		float fSlow65 = float(fHslider6);
		float fSlow66 = getValueStiffnessCoefficient(fSlow13);
		float fSlow67 = 13.69f * synth_piano_faustpower2_f(fSlow65) * synth_piano_faustpower2_f(fSlow66);
		float fSlow68 = fSlow67 + -1.0f;
		float fSlow69 = 4.4e+02f * fSlow39;
		float fSlow70 = 5.0f * getValueDetuningHz(fSlow13) * float(fHslider7);
		float fSlow71 = fSlow69 - fSlow70;
		float fSlow72 = fConst21 * fSlow71;
		float fSlow73 = std::sin(fSlow72);
		float fSlow74 = fSlow65 * fSlow66;
		float fSlow75 = 7.4f * fSlow74;
		float fSlow76 = fSlow67 + 1.0f;
		float fSlow77 = std::cos(fSlow72);
		float fSlow78 = 3.0f * std::atan2(fSlow68 * fSlow73, fSlow75 + fSlow76 * fSlow77);
		float fSlow79 = fSlow56 * fSlow59 * fSlow60;
		float fSlow80 = fSlow58 * fSlow57;
		float fSlow81 = 3.0f * fSlow80;
		float fSlow82 = fSlow79 - fSlow81;
		float fSlow83 = fSlow80 - fSlow79;
		float fSlow84 = 4.0f * fSlow83;
		float fSlow85 = fSlow82 + fSlow84;
		float fSlow86 = fSlow77 * fSlow82 / fSlow62 + 1.0f;
		float fSlow87 = 4.0f * fSlow64;
		float fSlow88 = (fSlow77 * fSlow85 + fSlow87) / fSlow62 + 1.0f;
		float fSlow89 = synth_piano_faustpower2_f(fSlow73) * fSlow82;
		float fSlow90 = synth_piano_faustpower2_f(fSlow62);
		float fSlow91 = fConst20 * ((fSlow78 + std::atan2((0.0f - fSlow73 * fSlow85 / fSlow62) * fSlow86 + fSlow73 * fSlow82 * fSlow88 / fSlow62, fSlow86 * fSlow88 + fSlow89 * fSlow85 / fSlow90) + 6.2831855f) / fSlow71);
		float fSlow92 = std::floor(fSlow91);
		float fSlow93 = fSlow91 - fSlow92;
		float fSlow94 = 1.6298145e-09f * fSlow19 * float(fSlow13 < 45.0f);
		float fSlow95 = fConst11 * (0.9996f * fSlow30 + 0.9f * getValueReleaseLoopGain(fSlow13) * fSlow28);
		float fSlow96 = 3.7f * fSlow74;
		float fSlow97 = fSlow79 + fSlow84 - fSlow81;
		float fSlow98 = (fSlow87 + fSlow77 * fSlow97) / fSlow62 + 1.0f;
		int iSlow99 = int(fConst20 * ((fSlow78 + std::atan2(0.0f - fSlow73 * (fSlow86 * fSlow97 - fSlow82 * fSlow98) / fSlow62, fSlow86 * fSlow98 + fSlow89 * fSlow97 / fSlow90) + 6.2831855f) / fSlow71));
		int iSlow100 = std::min<int>(4097, std::max<int>(0, iSlow99 + 1));
		float fSlow101 = fSlow92 + (1.0f - fSlow91);
		int iSlow102 = std::min<int>(4097, std::max<int>(0, iSlow99));
		float fSlow103 = fSlow69 + fSlow70;
		float fSlow104 = fConst21 * fSlow103;
		float fSlow105 = std::sin(fSlow104);
		float fSlow106 = std::cos(fSlow104);
		float fSlow107 = 3.0f * std::atan2(fSlow68 * fSlow105, fSlow75 + fSlow76 * fSlow106);
		float fSlow108 = fSlow82 * fSlow106 / fSlow62 + 1.0f;
		float fSlow109 = (fSlow87 + fSlow85 * fSlow106) / fSlow62 + 1.0f;
		float fSlow110 = fSlow82 * fSlow85;
		float fSlow111 = synth_piano_faustpower2_f(fSlow105);
		float fSlow112 = fConst20 * ((fSlow107 + std::atan2((0.0f - fSlow85 * fSlow105 / fSlow62) * fSlow108 + fSlow82 * fSlow105 * fSlow109 / fSlow62, fSlow108 * fSlow109 + fSlow110 * fSlow111 / fSlow90) + 6.2831855f) / fSlow103);
		float fSlow113 = std::floor(fSlow112);
		float fSlow114 = fSlow113 + (1.0f - fSlow112);
		float fSlow115 = (fSlow87 + fSlow97 * fSlow106) / fSlow62 + 1.0f;
		float fSlow116 = fSlow82 * fSlow97;
		int iSlow117 = int(fConst20 * ((fSlow107 + std::atan2(0.0f - fSlow105 * (fSlow97 * fSlow108 - fSlow82 * fSlow115) / fSlow62, fSlow108 * fSlow115 + fSlow116 * fSlow111 / fSlow90) + 6.2831855f) / fSlow103));
		int iSlow118 = std::min<int>(4097, std::max<int>(0, iSlow117));
		float fSlow119 = fSlow112 - fSlow113;
		int iSlow120 = std::min<int>(4097, std::max<int>(0, iSlow117 + 1));
		float fSlow121 = fSlow39 + std::pow(2.0f, 0.083333336f * (fSlow13 + -129.0f));
		float fSlow122 = fConst19 * fSlow121;
		float fSlow123 = std::sin(fSlow122);
		float fSlow124 = std::cos(fSlow122);
		float fSlow125 = 3.0f * std::atan2(fSlow68 * fSlow123, fSlow75 + fSlow76 * fSlow124);
		float fSlow126 = fSlow82 * fSlow124 / fSlow62 + 1.0f;
		float fSlow127 = (fSlow87 + fSlow85 * fSlow124) / fSlow62 + 1.0f;
		float fSlow128 = synth_piano_faustpower2_f(fSlow123);
		float fSlow129 = fConst22 * ((fSlow125 + std::atan2((0.0f - fSlow85 * fSlow123 / fSlow62) * fSlow126 + fSlow82 * fSlow123 * fSlow127 / fSlow62, fSlow126 * fSlow127 + fSlow110 * fSlow128 / fSlow90) + 6.2831855f) / fSlow121);
		float fSlow130 = std::floor(fSlow129);
		float fSlow131 = fSlow129 - fSlow130;
		float fSlow132 = 1.1641532e-09f * fSlow19 * float((fSlow13 >= 45.0f) & (fSlow13 < 88.0f));
		float fSlow133 = (fSlow87 + fSlow97 * fSlow124) / fSlow62 + 1.0f;
		int iSlow134 = int(fConst22 * ((fSlow125 + std::atan2(0.0f - fSlow123 * (fSlow97 * fSlow126 - fSlow82 * fSlow133) / fSlow62, fSlow126 * fSlow133 + fSlow116 * fSlow128 / fSlow90) + 6.2831855f) / fSlow121));
		int iSlow135 = std::min<int>(4097, std::max<int>(0, iSlow134 + 1));
		float fSlow136 = fSlow130 + (1.0f - fSlow129);
		int iSlow137 = std::min<int>(4097, std::max<int>(0, iSlow134));
		float fSlow138 = 4.4e+02f * fSlow121;
		float fSlow139 = fSlow70 + fSlow138;
		float fSlow140 = fConst21 * fSlow139;
		float fSlow141 = std::sin(fSlow140);
		float fSlow142 = std::cos(fSlow140);
		float fSlow143 = 3.0f * std::atan2(fSlow68 * fSlow141, fSlow75 + fSlow76 * fSlow142);
		float fSlow144 = fSlow82 * fSlow142 / fSlow62 + 1.0f;
		float fSlow145 = (fSlow87 + fSlow85 * fSlow142) / fSlow62 + 1.0f;
		float fSlow146 = synth_piano_faustpower2_f(fSlow141);
		float fSlow147 = fConst20 * ((fSlow143 + std::atan2((0.0f - fSlow85 * fSlow141 / fSlow62) * fSlow144 + fSlow82 * fSlow141 * fSlow145 / fSlow62, fSlow144 * fSlow145 + fSlow110 * fSlow146 / fSlow90) + 6.2831855f) / fSlow139);
		float fSlow148 = std::floor(fSlow147);
		float fSlow149 = fSlow147 - fSlow148;
		float fSlow150 = (fSlow87 + fSlow97 * fSlow142) / fSlow62 + 1.0f;
		int iSlow151 = int(fConst20 * ((fSlow143 + std::atan2(0.0f - fSlow141 * (fSlow97 * fSlow144 - fSlow82 * fSlow150) / fSlow62, fSlow144 * fSlow150 + fSlow116 * fSlow146 / fSlow90) + 6.2831855f) / fSlow139));
		int iSlow152 = std::min<int>(4097, std::max<int>(0, iSlow151 + 1));
		float fSlow153 = fSlow138 - fSlow70;
		float fSlow154 = fConst21 * fSlow153;
		float fSlow155 = std::sin(fSlow154);
		float fSlow156 = std::cos(fSlow154);
		float fSlow157 = 3.0f * std::atan2(fSlow68 * fSlow155, fSlow75 + fSlow76 * fSlow156);
		float fSlow158 = fSlow82 * fSlow156 / fSlow62 + 1.0f;
		float fSlow159 = (fSlow87 + fSlow85 * fSlow156) / fSlow62 + 1.0f;
		float fSlow160 = synth_piano_faustpower2_f(fSlow155);
		float fSlow161 = fConst20 * ((fSlow157 + std::atan2((0.0f - fSlow85 * fSlow155 / fSlow62) * fSlow158 + fSlow82 * fSlow155 * fSlow159 / fSlow62, fSlow158 * fSlow159 + fSlow110 * fSlow160 / fSlow90) + 6.2831855f) / fSlow153);
		float fSlow162 = std::floor(fSlow161);
		float fSlow163 = fSlow162 + (1.0f - fSlow161);
		float fSlow164 = (fSlow87 + fSlow97 * fSlow156) / fSlow62 + 1.0f;
		int iSlow165 = int(fConst20 * ((fSlow157 + std::atan2(0.0f - fSlow155 * (fSlow97 * fSlow158 - fSlow82 * fSlow164) / fSlow62, fSlow158 * fSlow164 + fSlow116 * fSlow160 / fSlow90) + 6.2831855f) / fSlow153));
		int iSlow166 = std::min<int>(4097, std::max<int>(0, iSlow165));
		float fSlow167 = fSlow161 - fSlow162;
		int iSlow168 = std::min<int>(4097, std::max<int>(0, iSlow165 + 1));
		float fSlow169 = fSlow148 + (1.0f - fSlow147);
		int iSlow170 = std::min<int>(4097, std::max<int>(0, iSlow151));
		float fSlow171 = getValueEQBandWidthFactor(fSlow13);
		float fSlow172 = synth_piano_faustpower2_f(fSlow39) * synth_piano_faustpower2_f(fSlow171);
		float fSlow173 = 0.5f - fConst24 * fSlow172;
		float fSlow174 = getValueEQGain(fSlow13);
		float fSlow175 = (0.0f - fConst25 * fSlow39 * fSlow171) * std::cos(fConst19 * (fSlow39 / getValueStrikePosition(fSlow13)));
		float fSlow176 = fConst26 * fSlow172;
		float fSlow177 = std::exp(fConst31 / fSlow0);
		float fSlow178 = synth_piano_faustpower2_f(fSlow177);
		float fSlow179 = 1.0f - fConst4 * fSlow178;
		float fSlow180 = 1.0f - fSlow178;
		float fSlow181 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow179) / synth_piano_faustpower2_f(fSlow180) + -1.0f));
		float fSlow182 = fSlow179 / fSlow180;
		float fSlow183 = fSlow177 * (fSlow181 + (1.0f - fSlow182));
		float fSlow184 = std::exp(fConst32 / fSlow0) / fSlow177 + -1.0f;
		float fSlow185 = fSlow182 - fSlow181;
		float fSlow186 = 12.0f * fSlow10;
		int iSlow187 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst35 * (float(fHslider8) / fSlow39))));
		float fSlow188 = std::exp(fConst39 / fSlow0);
		float fSlow189 = synth_piano_faustpower2_f(fSlow188);
		float fSlow190 = 1.0f - fConst4 * fSlow189;
		float fSlow191 = 1.0f - fSlow189;
		float fSlow192 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow190) / synth_piano_faustpower2_f(fSlow191) + -1.0f));
		float fSlow193 = fSlow190 / fSlow191;
		float fSlow194 = fSlow188 * (fSlow192 + (1.0f - fSlow193));
		float fSlow195 = std::exp(fConst40 / fSlow0) / fSlow188 + -1.0f;
		float fSlow196 = fSlow193 - fSlow192;
		float fSlow197 = std::exp(fConst46 / fSlow0);
		float fSlow198 = synth_piano_faustpower2_f(fSlow197);
		float fSlow199 = 1.0f - fConst4 * fSlow198;
		float fSlow200 = 1.0f - fSlow198;
		float fSlow201 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow199) / synth_piano_faustpower2_f(fSlow200) + -1.0f));
		float fSlow202 = fSlow199 / fSlow200;
		float fSlow203 = fSlow197 * (fSlow201 + (1.0f - fSlow202));
		float fSlow204 = std::exp(fConst47 / fSlow0) / fSlow197 + -1.0f;
		float fSlow205 = fSlow202 - fSlow201;
		float fSlow206 = std::exp(fConst53 / fSlow0);
		float fSlow207 = synth_piano_faustpower2_f(fSlow206);
		float fSlow208 = 1.0f - fConst4 * fSlow207;
		float fSlow209 = 1.0f - fSlow207;
		float fSlow210 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow208) / synth_piano_faustpower2_f(fSlow209) + -1.0f));
		float fSlow211 = fSlow208 / fSlow209;
		float fSlow212 = fSlow206 * (fSlow210 + (1.0f - fSlow211));
		float fSlow213 = std::exp(fConst54 / fSlow0) / fSlow206 + -1.0f;
		float fSlow214 = fSlow211 - fSlow210;
		float fSlow215 = std::exp(fConst60 / fSlow0);
		float fSlow216 = synth_piano_faustpower2_f(fSlow215);
		float fSlow217 = 1.0f - fConst4 * fSlow216;
		float fSlow218 = 1.0f - fSlow216;
		float fSlow219 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow217) / synth_piano_faustpower2_f(fSlow218) + -1.0f));
		float fSlow220 = fSlow217 / fSlow218;
		float fSlow221 = fSlow215 * (fSlow219 + (1.0f - fSlow220));
		float fSlow222 = std::exp(fConst61 / fSlow0) / fSlow215 + -1.0f;
		float fSlow223 = fSlow220 - fSlow219;
		float fSlow224 = std::exp(fConst67 / fSlow0);
		float fSlow225 = synth_piano_faustpower2_f(fSlow224);
		float fSlow226 = 1.0f - fConst4 * fSlow225;
		float fSlow227 = 1.0f - fSlow225;
		float fSlow228 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow226) / synth_piano_faustpower2_f(fSlow227) + -1.0f));
		float fSlow229 = fSlow226 / fSlow227;
		float fSlow230 = fSlow224 * (fSlow228 + (1.0f - fSlow229));
		float fSlow231 = std::exp(fConst68 / fSlow0) / fSlow224 + -1.0f;
		float fSlow232 = fSlow229 - fSlow228;
		float fSlow233 = std::exp(fConst74 / fSlow0);
		float fSlow234 = synth_piano_faustpower2_f(fSlow233);
		float fSlow235 = 1.0f - fConst4 * fSlow234;
		float fSlow236 = 1.0f - fSlow234;
		float fSlow237 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow235) / synth_piano_faustpower2_f(fSlow236) + -1.0f));
		float fSlow238 = fSlow235 / fSlow236;
		float fSlow239 = fSlow233 * (fSlow237 + (1.0f - fSlow238));
		float fSlow240 = std::exp(fConst75 / fSlow0) / fSlow233 + -1.0f;
		float fSlow241 = fSlow238 - fSlow237;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec11[0] = 0.0f - fConst7 * (fConst8 * fRec11[1] - (fRec0[1] + fRec0[2]));
			fRec10[0] = fSlow7 * (fRec0[1] + fSlow8 * fRec11[0]) + fSlow9 * fRec10[1];
			fVec0[IOTA0 & 16383] = 0.35355338f * fRec10[0] + 1e-20f;
			fRec12[0] = fSlow12 + fConst12 * fRec12[1];
			iRec23[0] = 1103515245 * iRec23[1] + 12345;
			iRec25[0] = iSlow24 * iRec25[1] + 1;
			float fTemp0 = float(iRec25[0] + -1);
			float fTemp1 = float((fTemp0 < 2.0f) & iSlow25);
			float fTemp2 = 0.030197384f * fTemp1 + fSlow26 * float((fTemp0 >= 2.0f) | iSlow27);
			fRec24[0] = fRec24[1] * fTemp2 + 0.15f * fTemp1 * (1.0f - fTemp2);
			int iTemp3 = fTemp0 < fSlow33;
			float fTemp4 = fSlow32 * float(iTemp3) + fConst17 * float(iSlow24 * (fTemp0 >= fSlow33));
			fRec26[0] = fRec26[1] * (fSlow29 + fTemp4) + fSlow34 * (1.0f - fTemp4 - fSlow29) * float(iTemp3 & iSlow25);
			float fTemp5 = float(iRec23[0]) * (fRec24[0] + fRec26[0]);
			fVec1[0] = fSlow22 * fTemp5;
			float fTemp6 = 0.0f - (0.5f * fVec1[1] + fSlow35 * fTemp5);
			fVec2[0] = fTemp6;
			fRec22[0] = fSlow20 * fTemp6 + fSlow36 * fVec2[1] - fSlow14 * fRec22[1];
			fRec21[0] = fSlow19 * fRec22[0] + fSlow37 * fRec21[1];
			fRec20[0] = fSlow19 * fRec21[0] + fSlow37 * fRec20[1];
			fRec19[0] = fSlow19 * fRec20[0] + fSlow37 * fRec19[1];
			fRec18[0] = fSlow19 * fRec19[0] + fSlow37 * fRec18[1];
			fRec17[0] = fSlow16 * (0.0f - 0.5f * (fRec18[1] - fRec18[0])) - (fSlow40 * fRec17[1] + fSlow41 * fRec17[2]);
			fRec16[0] = fSlow16 * fRec17[0] - (fSlow43 * fRec16[1] + fSlow44 * fRec16[2]);
			fRec15[0] = fRec16[0] - (fSlow47 * fRec15[1] + fSlow48 * fRec15[2]);
			fRec14[0] = fRec15[0] + fSlow53 * fRec15[2] + fSlow46 * (fSlow54 * fRec15[1] - fSlow55 * fRec14[1]) - fSlow52 * fRec14[2];
			fRec13[0] = fSlow15 * fRec14[0] - fSlow14 * fRec13[1];
			fRec38[0] = fSlow94 * fTemp5 + fSlow37 * fRec38[1];
			fRec37[0] = fSlow19 * fRec38[0] + fSlow37 * fRec37[1];
			fRec36[0] = fSlow19 * fRec37[0] + fSlow37 * fRec36[1];
			fRec35[0] = fSlow19 * fRec36[0] + fSlow37 * fRec35[1];
			fRec34[0] = fSlow20 * fRec35[0] + fSlow36 * fRec35[1] - fSlow14 * fRec34[1];
			fRec39[0] = fSlow95 + fConst12 * fRec39[1];
			float fTemp7 = fRec34[0] + fRec39[0] * fRec28[1];
			fVec3[0] = fTemp7;
			fRec33[0] = fVec3[1] + fSlow96 * (fTemp7 - fRec33[1]);
			fRec32[0] = fRec33[1] + fSlow96 * (fRec33[0] - fRec32[1]);
			fRec31[IOTA0 & 8191] = fRec32[1] + fSlow96 * (fRec32[0] - fRec31[(IOTA0 - 1) & 8191]);
			float fTemp8 = fSlow93 * fRec31[(IOTA0 - iSlow100) & 8191];
			float fTemp9 = fSlow101 * fRec31[(IOTA0 - iSlow102) & 8191];
			float fTemp10 = fRec39[0] * (fRec34[0] + fRec27[1]);
			fVec4[0] = fTemp10;
			fRec42[0] = fVec4[1] + fSlow96 * (fTemp10 - fRec42[1]);
			fRec41[0] = fRec42[1] + fSlow96 * (fRec42[0] - fRec41[1]);
			fRec40[IOTA0 & 8191] = fRec41[1] + fSlow96 * (fRec41[0] - fRec40[(IOTA0 - 1) & 8191]);
			float fTemp11 = fSlow114 * fRec40[(IOTA0 - iSlow118) & 8191] + fSlow119 * fRec40[(IOTA0 - iSlow120) & 8191];
			float fTemp12 = fTemp8 + fTemp9 + fTemp11;
			fVec5[0] = fTemp12;
			fRec30[0] = fSlow63 * (2.0f * (fSlow64 * fTemp12 + fSlow83 * fVec5[1]) - fSlow82 * fRec30[1]);
			fRec27[0] = fRec30[0] + fTemp11;
			fRec28[0] = fRec30[0] + fTemp9 + fTemp8;
			float fRec29 = fTemp12;
			fRec55[0] = fSlow132 * fTemp5 + fSlow37 * fRec55[1];
			fRec54[0] = fSlow19 * fRec55[0] + fSlow37 * fRec54[1];
			fRec53[0] = fSlow19 * fRec54[0] + fSlow37 * fRec53[1];
			fRec52[0] = fSlow19 * fRec53[0] + fSlow37 * fRec52[1];
			fRec51[0] = fSlow20 * fRec52[0] + fSlow36 * fRec52[1] - fSlow14 * fRec51[1];
			float fTemp13 = fRec51[0] + fRec39[0] * fRec44[1];
			fVec6[0] = fTemp13;
			fRec50[0] = fVec6[1] + fSlow96 * (fTemp13 - fRec50[1]);
			fRec49[0] = fRec50[1] + fSlow96 * (fRec50[0] - fRec49[1]);
			fRec48[IOTA0 & 8191] = fRec49[1] + fSlow96 * (fRec49[0] - fRec48[(IOTA0 - 1) & 8191]);
			float fTemp14 = fSlow131 * fRec48[(IOTA0 - iSlow135) & 8191];
			float fTemp15 = fSlow136 * fRec48[(IOTA0 - iSlow137) & 8191];
			float fTemp16 = fRec39[0] * (fRec51[0] + fRec43[1]);
			fVec7[0] = fTemp16;
			fRec58[0] = fVec7[1] + fSlow96 * (fTemp16 - fRec58[1]);
			fRec57[0] = fRec58[1] + fSlow96 * (fRec58[0] - fRec57[1]);
			fRec56[IOTA0 & 8191] = fRec57[1] + fSlow96 * (fRec57[0] - fRec56[(IOTA0 - 1) & 8191]);
			float fTemp17 = fSlow149 * fRec56[(IOTA0 - iSlow152) & 8191];
			float fTemp18 = fRec51[0] + fRec39[0] * fRec45[1];
			fVec8[0] = fTemp18;
			fRec61[0] = fVec8[1] + fSlow96 * (fTemp18 - fRec61[1]);
			fRec60[0] = fRec61[1] + fSlow96 * (fRec61[0] - fRec60[1]);
			fRec59[IOTA0 & 8191] = fRec60[1] + fSlow96 * (fRec60[0] - fRec59[(IOTA0 - 1) & 8191]);
			float fTemp19 = fSlow163 * fRec59[(IOTA0 - iSlow166) & 8191] + fSlow167 * fRec59[(IOTA0 - iSlow168) & 8191];
			float fTemp20 = fSlow169 * fRec56[(IOTA0 - iSlow170) & 8191];
			float fTemp21 = fTemp14 + fTemp15 + fTemp17 + fTemp19 + fTemp20;
			fVec9[0] = fTemp21;
			fRec47[0] = fSlow63 * (2.0f * (fSlow64 * fTemp21 + fSlow83 * fVec9[1]) - fSlow82 * fRec47[1]);
			fRec43[0] = fRec47[0] + fTemp20 + fTemp17;
			fRec44[0] = fRec47[0] + fTemp15 + fTemp14;
			fRec45[0] = fRec47[0] + fTemp19;
			float fRec46 = fTemp21;
			fRec62[0] = fSlow174 * fRec46 - (fSlow175 * fRec62[1] + fSlow176 * fRec62[2]);
			fRec63[0] = fSlow174 * fRec29 - (fSlow175 * fRec63[1] + fSlow176 * fRec63[2]);
			float fTemp22 = fRec13[0] + fRec29 + fRec46 - fSlow173 * (fRec62[2] - fRec62[0] - (fRec63[0] - fRec63[2]));
			fVec10[IOTA0 & 4095] = fTemp22;
			fVec11[IOTA0 & 4095] = fSlow11 * fRec12[0] * fTemp22;
			float fTemp23 = 0.3f * fVec11[(IOTA0 - iConst27) & 4095];
			float fTemp24 = fVec0[(IOTA0 - iConst10) & 16383] + fTemp23 - 0.6f * fRec8[1];
			fVec12[IOTA0 & 2047] = fTemp24;
			fRec8[0] = fVec12[(IOTA0 - iConst28) & 2047];
			float fRec9 = 0.6f * fTemp24;
			fRec67[0] = 0.0f - fConst7 * (fConst8 * fRec67[1] - (fRec1[1] + fRec1[2]));
			fRec66[0] = fSlow183 * (fRec1[1] + fSlow184 * fRec67[0]) + fSlow185 * fRec66[1];
			fVec13[IOTA0 & 32767] = 0.35355338f * fRec66[0] + 1e-20f;
			float fTemp25 = fVec10[(IOTA0 - iSlow187) & 4095];
			fVec14[IOTA0 & 4095] = fSlow186 * fRec12[0] * fTemp25;
			float fTemp26 = 0.3f * fVec14[(IOTA0 - iConst27) & 4095];
			float fTemp27 = fVec13[(IOTA0 - iConst34) & 32767] + 0.6f * fRec64[1] + fTemp26;
			fVec15[IOTA0 & 4095] = fTemp27;
			fRec64[0] = fVec15[(IOTA0 - iConst36) & 4095];
			float fRec65 = 0.0f - 0.6f * fTemp27;
			fRec71[0] = 0.0f - fConst7 * (fConst8 * fRec71[1] - (fRec6[1] + fRec6[2]));
			fRec70[0] = fSlow194 * (fRec6[1] + fSlow195 * fRec71[0]) + fSlow196 * fRec70[1];
			fVec16[IOTA0 & 16383] = 0.35355338f * fRec70[0] + 1e-20f;
			float fTemp28 = fVec16[(IOTA0 - iConst42) & 16383] - (fTemp23 + 0.6f * fRec68[1]);
			fVec17[IOTA0 & 2047] = fTemp28;
			fRec68[0] = fVec17[(IOTA0 - iConst43) & 2047];
			float fRec69 = 0.6f * fTemp28;
			fRec75[0] = 0.0f - fConst7 * (fConst8 * fRec75[1] - (fRec3[1] + fRec3[2]));
			fRec74[0] = fSlow203 * (fRec3[1] + fSlow204 * fRec75[0]) + fSlow205 * fRec74[1];
			fVec18[IOTA0 & 32767] = 0.35355338f * fRec74[0] + 1e-20f;
			float fTemp29 = 0.6f * fRec72[1] + fVec18[(IOTA0 - iConst49) & 32767] - fTemp26;
			fVec19[IOTA0 & 4095] = fTemp29;
			fRec72[0] = fVec19[(IOTA0 - iConst50) & 4095];
			float fRec73 = 0.0f - 0.6f * fTemp29;
			fRec79[0] = 0.0f - fConst7 * (fConst8 * fRec79[1] - (fRec5[1] + fRec5[2]));
			fRec78[0] = fSlow212 * (fRec5[1] + fSlow213 * fRec79[0]) + fSlow214 * fRec78[1];
			fVec20[IOTA0 & 16383] = 0.35355338f * fRec78[0] + 1e-20f;
			float fTemp30 = fVec20[(IOTA0 - iConst56) & 16383] + fTemp26 + 0.6f * fRec76[1];
			fVec21[IOTA0 & 4095] = fTemp30;
			fRec76[0] = fVec21[(IOTA0 - iConst57) & 4095];
			float fRec77 = 0.0f - 0.6f * fTemp30;
			fRec83[0] = 0.0f - fConst7 * (fConst8 * fRec83[1] - (fRec2[1] + fRec2[2]));
			fRec82[0] = fSlow221 * (fRec2[1] + fSlow222 * fRec83[0]) + fSlow223 * fRec82[1];
			fVec22[IOTA0 & 16383] = 0.35355338f * fRec82[0] + 1e-20f;
			float fTemp31 = fVec22[(IOTA0 - iConst63) & 16383] - (fTemp23 + 0.6f * fRec80[1]);
			fVec23[IOTA0 & 4095] = fTemp31;
			fRec80[0] = fVec23[(IOTA0 - iConst64) & 4095];
			float fRec81 = 0.6f * fTemp31;
			fRec87[0] = 0.0f - fConst7 * (fConst8 * fRec87[1] - (fRec4[1] + fRec4[2]));
			fRec86[0] = fSlow230 * (fRec4[1] + fSlow231 * fRec87[0]) + fSlow232 * fRec86[1];
			fVec24[IOTA0 & 16383] = 0.35355338f * fRec86[0] + 1e-20f;
			float fTemp32 = fTemp23 + fVec24[(IOTA0 - iConst70) & 16383] - 0.6f * fRec84[1];
			fVec25[IOTA0 & 4095] = fTemp32;
			fRec84[0] = fVec25[(IOTA0 - iConst71) & 4095];
			float fRec85 = 0.6f * fTemp32;
			fRec91[0] = 0.0f - fConst7 * (fConst8 * fRec91[1] - (fRec7[1] + fRec7[2]));
			fRec90[0] = fSlow239 * (fRec7[1] + fSlow240 * fRec91[0]) + fSlow241 * fRec90[1];
			fVec26[IOTA0 & 32767] = 0.35355338f * fRec90[0] + 1e-20f;
			float fTemp33 = 0.6f * fRec88[1] + fVec26[(IOTA0 - iConst77) & 32767] - fTemp26;
			fVec27[IOTA0 & 2047] = fTemp33;
			fRec88[0] = fVec27[(IOTA0 - iConst78) & 2047];
			float fRec89 = 0.0f - 0.6f * fTemp33;
			float fTemp34 = fRec84[1] + fRec68[1];
			float fTemp35 = fRec80[1] + fTemp34;
			fRec0[0] = fRec8[1] + fRec65 + fRec69 + fRec73 + fRec77 + fRec81 + fRec85 + fRec9 + fRec89 + fRec72[1] + fRec88[1] + fRec64[1] + fTemp35 + fRec76[1];
			fRec1[0] = fRec8[1] + fRec69 + fRec81 + fRec85 + fRec9 + fTemp35 - (fRec65 + fRec73 + fRec77 + fRec89 + fRec72[1] + fRec64[1] + fRec76[1] + fRec88[1]);
			float fTemp36 = fRec80[1] + fRec68[1];
			fRec2[0] = fRec8[1] + fRec65 + fRec77 + fRec85 + fRec9 + fRec64[1] + fRec84[1] + fRec76[1] - (fRec69 + fRec73 + fRec81 + fRec89 + fRec72[1] + fTemp36 + fRec88[1]);
			fRec3[0] = fRec8[1] + fRec73 + fRec85 + fRec9 + fRec89 + fRec72[1] + fRec84[1] + fRec88[1] - (fRec65 + fRec69 + fRec77 + fRec81 + fRec64[1] + fTemp36 + fRec76[1]);
			fRec4[0] = fRec8[1] + fRec65 + fRec73 + fRec81 + fRec9 + fRec72[1] + fRec80[1] + fRec64[1] - (fRec69 + fRec77 + fRec85 + fRec89 + fRec88[1] + fTemp34 + fRec76[1]);
			fRec5[0] = fRec8[1] + fRec77 + fRec81 + fRec9 + fRec89 + fRec88[1] + fRec80[1] + fRec76[1] - (fRec65 + fRec69 + fRec73 + fRec85 + fRec72[1] + fTemp34 + fRec64[1]);
			float fTemp37 = fRec84[1] + fRec80[1];
			fRec6[0] = fRec8[1] + fRec65 + fRec69 + fRec9 + fRec89 + fRec88[1] + fRec68[1] + fRec64[1] - (fRec73 + fRec77 + fRec81 + fRec85 + fRec72[1] + fRec76[1] + fTemp37);
			fRec7[0] = fRec8[1] + fRec69 + fRec73 + fRec77 + fRec9 + fRec72[1] + fRec68[1] + fRec76[1] - (fRec65 + fRec81 + fRec85 + fRec89 + fRec88[1] + fRec64[1] + fTemp37);
			float fTemp38 = 1.0f - fRec12[0];
			output0[i0] = FAUSTFLOAT(0.37f * (fRec1[0] + fRec2[0]) + fSlow11 * fTemp38 * fTemp22);
			output1[i0] = FAUSTFLOAT(0.37f * (fRec1[0] - fRec2[0]) + fSlow186 * fTemp38 * fTemp25);
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA0 = IOTA0 + 1;
			fRec12[1] = fRec12[0];
			iRec23[1] = iRec23[0];
			iRec25[1] = iRec25[0];
			fRec24[1] = fRec24[0];
			fRec26[1] = fRec26[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec39[1] = fRec39[0];
			fVec3[1] = fVec3[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fVec4[1] = fVec4[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fVec5[1] = fVec5[0];
			fRec30[1] = fRec30[0];
			fRec27[1] = fRec27[0];
			fRec28[1] = fRec28[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fVec6[1] = fVec6[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fVec7[1] = fVec7[0];
			fRec58[1] = fRec58[0];
			fRec57[1] = fRec57[0];
			fVec8[1] = fVec8[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fVec9[1] = fVec9[0];
			fRec47[1] = fRec47[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
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
#endif

class faust_synth_piano_tilde : public FaustExternal<synth_piano> {
public:
    faust_synth_piano_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_piano))
    {
    }
};

#endif
