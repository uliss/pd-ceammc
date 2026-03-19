/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.piano"
version: "1.0"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -cn synth_piano -scn synth_piano_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_piano_H__
#define  __synth_piano_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdint>
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
#include <cstdint>

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

// Version as a global string
#define FAUSTVERSION "2.85.5"

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 85
#define FAUSTPATCHVERSION 5

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
    
    enum MemType { kInt32, kInt32_ptr, kFloat, kFloat_ptr, kDouble, kDouble_ptr, kQuad, kQuad_ptr, kFixedPoint, kFixedPoint_ptr, kObj, kObj_ptr, kSound, kSound_ptr };

    virtual ~dsp_memory_manager() = default;
    
    /**
     * Inform the Memory Manager with the number of expected memory zones.
     * @param count - the number of expected memory zones
     */
    virtual void begin(size_t count) {}
    
    /**
     * Give the Memory Manager information on a given memory zone.
     * @param name - the memory zone name
     * @param type - the memory zone type (in MemType)
     * @param size - the size in unit of the memory type of the memory zone
     * @param size_bytes - the size in bytes of the memory zone
     * @param reads - the number of Read access to the zone used to compute one frame
     * @param writes - the number of Write access to the zone used to compute one frame
     */
    virtual void info(const char* name, MemType type, size_t size, size_t size_bytes, size_t reads, size_t writes) {}
  
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

        synth_piano_dsp() = default;
        virtual ~synth_piano_dsp() = default;

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
         * Init instance state.
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state.
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
        virtual ::synth_piano_dsp* clone() = 0;
    
        /**
         * Trigger the Meta* m parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;

    
        /**
         * Read all controllers (buttons, sliders, etc.), and update the DSP state to be used by 'frame' or 'compute'.
         * This method will be filled with the -ec (--external-control) option.
         */
        virtual void control() {}
    
        /**
         * DSP instance computation to process one single frame.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write frame(inputs, inputs).
         * The -inpl option can be used for that, but only in scalar mode for now.
         * This method will be filled with the -os (--one-sample) option.
         *
         * @param inputs - the input audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         */
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) {}
        
        /**
         * DSP instance computation to be called with successive in/out audio buffers.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write compute(count, inputs, inputs).
         * The -inpl compilation option can be used for that, but only in scalar mode for now.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * Alternative DSP instance computation method for use by subclasses, incorporating an additional `date_usec` parameter,
         * which specifies the timestamp of the first sample in the audio buffers.
         *
         * @param date_usec - the timestamp in microsec given by audio driver. By convention timestamp of -1 means 'no timestamp conversion',
         * events already have a timestamp expressed in frames.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class FAUST_API decorator_dsp : public ::synth_piano_dsp {

    protected:

        ::synth_piano_dsp* fDSP;

    public:

        decorator_dsp(::synth_piano_dsp* synth_piano_dsp = nullptr):fDSP(synth_piano_dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() override { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() override { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) override { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() override { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) override { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) override { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) override { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() override { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() override { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() override { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) override { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void control() override { fDSP->control(); }
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) override { fDSP->frame(inputs, outputs); }
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) override { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) override { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class FAUST_API dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() = default;
    
    public:
    
        /* Return factory name */
        virtual std::string getName() = 0;
    
        /* Return factory SHA key */
        virtual std::string getSHAKey() = 0;
    
        /* Return factory expanded DSP code */
        virtual std::string getDSPCode() = 0;
    
        /* Return factory compile options */
        virtual std::string getCompileOptions() = 0;
    
        /* Get the Faust DSP factory list of library dependancies */
        virtual std::vector<std::string> getLibraryList() = 0;
    
        /* Get the list of all used includes */
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        /* Get warning messages list for a given compilation */
        virtual std::vector<std::string> getWarningMessages() = 0;

        /* Return JSON description of the DSP (UI + metadata) */
        virtual std::string getJSON() = 0;
    
        /* Create a new DSP instance, to be deleted with C++ 'delete' */
        virtual ::synth_piano_dsp* createDSPInstance() = 0;
    
        /* Static tables initialization, possibly implemened in sub-classes*/
        virtual void classInit(int sample_rate) {};
    
        /* Set a custom memory manager to be used when creating instances */
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
    
        /* Return the currently set custom memory manager */
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
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <map>
#include <string.h>
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
    for (int i = 0; argv[i]; i++) {
        if (!strcmp(argv[i], name) && argv[i + 1]) {
            return std::strtol(argv[i + 1], nullptr, 10);
        }
    }
    return def;
}

static long lopt1(int argc, char* argv[], const char* longname, const char* shortname, long def)
{
    for (int i = 2; i < argc; i++) {
        if ((strcmp(argv[i - 1], shortname) == 0 || strcmp(argv[i - 1], longname) == 0) && argv[i]) {
            return std::strtol(argv[i], nullptr, 10);
        }
    }
    return def;
}

static const char* lopts(char* argv[], const char* name, const char* def)
{
    for (int i = 0; argv[i]; i++) {
        if (!strcmp(argv[i], name) && argv[i + 1]) {
            return argv[i + 1];
        }
    }
    return def;
}

static const char* lopts1(int argc, char* argv[], const char* longname, const char* shortname, const char* def)
{
    for (int i = 2; i < argc; i++) {
        if ((strcmp(argv[i - 1], shortname) == 0 || strcmp(argv[i - 1], longname) == 0) && argv[i]) {
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
    return (!file) ? "" : std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
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
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fHslider0;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fRec11[2];
	float fRec10[2];
	int IOTA0;
	float fVec0[32768];
	float fConst9;
	int iConst10;
	FAUSTFLOAT fHslider1;
	float fConst11;
	FAUSTFLOAT fHslider2;
	float fConst12;
	float fRec12[2];
	float fConst13;
	FAUSTFLOAT fHslider3;
	float fConst14;
	FAUSTFLOAT fHslider4;
	float fConst15;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fButton0;
	float fRec20[2];
	FAUSTFLOAT fHslider6;
	int iRec26[2];
	int iRec28[2];
	float fConst16;
	float fRec27[2];
	float fConst17;
	float fConst18;
	FAUSTFLOAT fHslider7;
	float fConst19;
	float fConst20;
	float fRec29[2];
	float fRec25[2];
	float fRec24[2];
	float fRec23[2];
	float fRec22[2];
	float fRec21[2];
	float fVec1[2];
	float fRec19[2];
	float fRec18[2];
	float fRec17[8192];
	float fVec2[2];
	float fRec33[2];
	float fRec32[2];
	float fRec31[8192];
	float fVec3[2];
	float fRec30[2];
	float fRec14[2];
	float fRec15[2];
	float fConst21;
	float fConst22;
	float fConst23;
	float fRec13[3];
	float fRec46[2];
	float fRec45[2];
	float fRec44[2];
	float fRec43[2];
	float fRec42[2];
	float fVec4[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[8192];
	float fVec5[2];
	float fRec50[2];
	float fRec49[2];
	float fRec48[8192];
	float fConst24;
	float fVec6[2];
	float fRec53[2];
	float fRec52[2];
	float fRec51[8192];
	float fVec7[2];
	float fRec47[2];
	float fRec35[2];
	float fRec36[2];
	float fRec37[2];
	float fRec34[3];
	float fVec8[2];
	float fVec9[2];
	float fRec63[2];
	float fRec62[2];
	float fRec61[2];
	float fRec60[2];
	float fRec59[2];
	float fConst25;
	float fRec58[3];
	float fRec57[3];
	float fRec56[3];
	float fRec55[3];
	float fRec54[2];
	float fVec10[4096];
	float fConst26;
	FAUSTFLOAT fHslider8;
	float fVec11[4096];
	int iConst27;
	float fVec12[2048];
	int iConst28;
	float fRec8[2];
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst32;
	float fRec67[2];
	float fRec66[2];
	float fVec13[32768];
	float fConst33;
	int iConst34;
	float fVec14[4096];
	int iConst35;
	float fRec64[2];
	float fConst36;
	float fConst37;
	float fConst38;
	float fConst39;
	float fRec71[2];
	float fRec70[2];
	float fVec15[16384];
	float fConst40;
	int iConst41;
	float fVec16[4096];
	int iConst42;
	float fRec68[2];
	float fConst43;
	float fConst44;
	float fConst45;
	float fConst46;
	float fRec75[2];
	float fRec74[2];
	float fVec17[32768];
	float fConst47;
	int iConst48;
	float fVec18[4096];
	int iConst49;
	float fRec72[2];
	float fConst50;
	float fConst51;
	float fConst52;
	float fConst53;
	float fRec79[2];
	float fRec78[2];
	float fVec19[16384];
	float fConst54;
	int iConst55;
	float fVec20[4096];
	float fVec21[2048];
	int iConst56;
	float fRec76[2];
	float fConst57;
	float fConst58;
	float fConst59;
	float fConst60;
	float fRec83[2];
	float fRec82[2];
	float fVec22[16384];
	float fConst61;
	int iConst62;
	float fVec23[4096];
	int iConst63;
	float fRec80[2];
	float fConst64;
	float fConst65;
	float fConst66;
	float fConst67;
	float fRec87[2];
	float fRec86[2];
	float fVec24[16384];
	float fConst68;
	int iConst69;
	float fVec25[4096];
	int iConst70;
	float fRec84[2];
	float fConst71;
	float fConst72;
	float fConst73;
	float fConst74;
	float fRec91[2];
	float fRec90[2];
	float fVec26[16384];
	float fConst75;
	int iConst76;
	float fVec27[2048];
	int iConst77;
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
	synth_piano() {
	}
	
	synth_piano(const synth_piano&) = default;
	
	virtual ~synth_piano() = default;
	
	synth_piano& operator=(const synth_piano&) = default;
	
	void metadata(Meta* m) { 
		m->declare("author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.4");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -cn synth_piano -scn synth_piano_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "Romain Michon");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
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
		m->declare("filters.lib/version", "1.7.1");
		m->declare("instruments.lib/author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("instruments.lib/copyright", "Romain Michon");
		m->declare("instruments.lib/licence", "STK-4.3");
		m->declare("instruments.lib/name", "Faust-STK Tools Library");
		m->declare("instruments.lib/version", "1.0.0");
		m->declare("licence", "STK-4.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "synth.piano");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.5.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "1.5.1");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.6.0");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = std::floor(0.219991f * fConst0 + 0.5f);
		fConst2 = fConst1 / fConst0;
		fConst3 = 1.7269388f * fConst2;
		fConst4 = std::cos(37699.113f / fConst0);
		fConst5 = 1.1512926f * fConst2;
		fConst6 = 1.0f / std::tan(628.31854f / fConst0);
		fConst7 = 1.0f / (fConst6 + 1.0f);
		fConst8 = 1.0f - fConst6;
		fConst9 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst10 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst1 - fConst9)));
		fConst11 = 44.1f / fConst0;
		fConst12 = 1.0f - fConst11;
		fConst13 = 9.68e+04f / synth_piano_faustpower2_f(fConst0);
		fConst14 = 0.15915494f * fConst0;
		fConst15 = 6.2831855f / fConst0;
		fConst16 = 7.0f / fConst0;
		fConst17 = std::exp(-(5.0f / fConst0));
		fConst18 = 1e+01f / fConst0;
		fConst19 = 0.1f * fConst0;
		fConst20 = std::exp(-(0.5f / fConst0));
		fConst21 = 1.0f / fConst0;
		fConst22 = 1.936e+05f / fConst0;
		fConst23 = 2764.6016f / fConst0;
		fConst24 = 0.0003617158f * fConst0;
		fConst25 = 0.05f / fConst0;
		fConst26 = 0.0011363636f * fConst0;
		iConst27 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, 0.02f * fConst0)));
		iConst28 = static_cast<int>(std::min<float>(1024.0f, std::max<float>(0.0f, fConst9 + -1.0f)));
		fConst29 = std::floor(0.256891f * fConst0 + 0.5f);
		fConst30 = fConst29 / fConst0;
		fConst31 = 1.7269388f * fConst30;
		fConst32 = 1.1512926f * fConst30;
		fConst33 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst34 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst29 - fConst33)));
		iConst35 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst33 + -1.0f)));
		fConst36 = std::floor(0.192303f * fConst0 + 0.5f);
		fConst37 = fConst36 / fConst0;
		fConst38 = 1.7269388f * fConst37;
		fConst39 = 1.1512926f * fConst37;
		fConst40 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst41 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst36 - fConst40)));
		iConst42 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst40 + -1.0f)));
		fConst43 = std::floor(0.210389f * fConst0 + 0.5f);
		fConst44 = fConst43 / fConst0;
		fConst45 = 1.7269388f * fConst44;
		fConst46 = 1.1512926f * fConst44;
		fConst47 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst48 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst43 - fConst47)));
		iConst49 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst47 + -1.0f)));
		fConst50 = std::floor(0.125f * fConst0 + 0.5f);
		fConst51 = fConst50 / fConst0;
		fConst52 = 1.7269388f * fConst51;
		fConst53 = 1.1512926f * fConst51;
		fConst54 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst55 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst50 - fConst54)));
		iConst56 = static_cast<int>(std::min<float>(1024.0f, std::max<float>(0.0f, fConst54 + -1.0f)));
		fConst57 = std::floor(0.127837f * fConst0 + 0.5f);
		fConst58 = fConst57 / fConst0;
		fConst59 = 1.7269388f * fConst58;
		fConst60 = 1.1512926f * fConst58;
		fConst61 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst62 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst57 - fConst61)));
		iConst63 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst61 + -1.0f)));
		fConst64 = std::floor(0.174713f * fConst0 + 0.5f);
		fConst65 = fConst64 / fConst0;
		fConst66 = 1.7269388f * fConst65;
		fConst67 = 1.1512926f * fConst65;
		fConst68 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst69 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst64 - fConst68)));
		iConst70 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst68 + -1.0f)));
		fConst71 = std::floor(0.153129f * fConst0 + 0.5f);
		fConst72 = fConst71 / fConst0;
		fConst73 = 1.7269388f * fConst72;
		fConst74 = 1.1512926f * fConst72;
		fConst75 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst76 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst71 - fConst75)));
		iConst77 = static_cast<int>(std::min<float>(1024.0f, std::max<float>(0.0f, fConst75 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.36f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.6f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.137f);
		fHslider3 = static_cast<FAUSTFLOAT>(48.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(0.28f);
		fHslider5 = static_cast<FAUSTFLOAT>(0.1f);
		fButton0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider6 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider7 = static_cast<FAUSTFLOAT>(0.1f);
		fHslider8 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec11[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec10[l1] = 0.0f;
		}
		IOTA0 = 0;
		for (int l2 = 0; l2 < 32768; l2 = l2 + 1) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec12[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec20[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec26[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iRec28[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec27[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec29[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec25[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec24[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec23[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec22[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec21[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec1[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec19[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec18[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 8192; l17 = l17 + 1) {
			fRec17[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fVec2[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec33[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec32[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 8192; l21 = l21 + 1) {
			fRec31[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec3[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec30[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec14[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec15[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec13[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec46[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec45[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec44[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec43[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec42[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fVec4[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec41[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec40[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 8192; l35 = l35 + 1) {
			fRec39[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fVec5[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec50[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec49[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 8192; l39 = l39 + 1) {
			fRec48[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fVec6[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec53[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec52[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 8192; l43 = l43 + 1) {
			fRec51[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fVec7[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec47[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec35[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec36[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec37[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = l49 + 1) {
			fRec34[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fVec8[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fVec9[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec63[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec62[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec61[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec60[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec59[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fRec58[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 3; l58 = l58 + 1) {
			fRec57[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec56[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec55[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec54[l61] = 0.0f;
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
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec64[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec71[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec70[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 16384; l73 = l73 + 1) {
			fVec15[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 4096; l74 = l74 + 1) {
			fVec16[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec68[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec75[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec74[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 32768; l78 = l78 + 1) {
			fVec17[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 4096; l79 = l79 + 1) {
			fVec18[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec72[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec79[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec78[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 16384; l83 = l83 + 1) {
			fVec19[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 4096; l84 = l84 + 1) {
			fVec20[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2048; l85 = l85 + 1) {
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
		for (int l99 = 0; l99 < 16384; l99 = l99 + 1) {
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
		return new synth_piano(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.piano");
		ui_interface->addHorizontalSlider("brightness", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("detuning", &fHslider5, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider7, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pan", &fHslider1, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pitch", &fHslider3, FAUSTFLOAT(48.0f), FAUSTFLOAT(21.0f), FAUSTFLOAT(108.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, FAUSTFLOAT(0.137f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("room_size", &fHslider0, FAUSTFLOAT(0.36f), FAUSTFLOAT(0.005f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("stiffness", &fHslider4, FAUSTFLOAT(0.28f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("width", &fHslider8, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = static_cast<float>(fHslider0);
		float fSlow1 = std::exp(-(fConst3 / fSlow0));
		float fSlow2 = synth_piano_faustpower2_f(fSlow1);
		float fSlow3 = 1.0f - fConst4 * fSlow2;
		float fSlow4 = 1.0f - fSlow2;
		float fSlow5 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow3) / synth_piano_faustpower2_f(fSlow4) + -1.0f));
		float fSlow6 = fSlow3 / fSlow4;
		float fSlow7 = fSlow1 * (fSlow5 + (1.0f - fSlow6));
		float fSlow8 = std::exp(-(fConst5 / fSlow0)) / fSlow1 + -1.0f;
		float fSlow9 = fSlow6 - fSlow5;
		float fSlow10 = static_cast<float>(fHslider1);
		float fSlow11 = 12.0f * fSlow10;
		float fSlow12 = fConst11 * static_cast<float>(fHslider2);
		float fSlow13 = static_cast<float>(fHslider3);
		float fSlow14 = std::pow(2.0f, 0.083333336f * (fSlow13 + -69.0f));
		float fSlow15 = getValueEQBandWidthFactor(fSlow13);
		float fSlow16 = 0.5f - fConst13 * synth_piano_faustpower2_f(fSlow14) * synth_piano_faustpower2_f(fSlow15);
		float fSlow17 = getValueEQGain(fSlow13);
		float fSlow18 = static_cast<float>(fHslider4);
		float fSlow19 = getValueStiffnessCoefficient(fSlow13);
		float fSlow20 = 13.69f * synth_piano_faustpower2_f(fSlow18) * synth_piano_faustpower2_f(fSlow19);
		float fSlow21 = fSlow20 + -1.0f;
		float fSlow22 = 4.4e+02f * fSlow14;
		float fSlow23 = 5.0f * getValueDetuningHz(fSlow13) * static_cast<float>(fHslider5);
		float fSlow24 = fSlow22 + fSlow23;
		float fSlow25 = fConst15 * fSlow24;
		float fSlow26 = std::sin(fSlow25);
		float fSlow27 = fSlow18 * fSlow19;
		float fSlow28 = 7.4f * fSlow27;
		float fSlow29 = fSlow20 + 1.0f;
		float fSlow30 = std::cos(fSlow25);
		float fSlow31 = 3.0f * std::atan2(fSlow21 * fSlow26, fSlow28 + fSlow29 * fSlow30);
		float fSlow32 = getValueSingleStringZero(fSlow13);
		float fSlow33 = getValueSingleStringPole(fSlow13);
		float fSlow34 = 1.0f - fSlow33;
		float fSlow35 = std::pow(1e+01f, 0.000113636364f * (getValueSingleStringDecayRate(fSlow13) / fSlow14));
		float fSlow36 = fSlow32 * fSlow34 * fSlow35;
		float fSlow37 = 1.0f - fSlow32;
		float fSlow38 = fSlow33 * fSlow37;
		float fSlow39 = 3.0f * fSlow38;
		float fSlow40 = fSlow36 - fSlow39;
		float fSlow41 = fSlow38 - fSlow36;
		float fSlow42 = 4.0f * fSlow41;
		float fSlow43 = fSlow40 + fSlow42;
		float fSlow44 = fSlow34 * fSlow35;
		float fSlow45 = 3.0f * fSlow37 - fSlow44;
		float fSlow46 = fSlow40 * fSlow30 / fSlow45 + 1.0f;
		float fSlow47 = fSlow32 + fSlow44 + -1.0f;
		float fSlow48 = 4.0f * fSlow47;
		float fSlow49 = (fSlow48 + fSlow43 * fSlow30) / fSlow45 + 1.0f;
		float fSlow50 = fSlow40 * fSlow43;
		float fSlow51 = synth_piano_faustpower2_f(fSlow26);
		float fSlow52 = synth_piano_faustpower2_f(fSlow45);
		float fSlow53 = fConst14 * ((fSlow31 + std::atan2(-(fSlow26 * (fSlow43 * fSlow46 - fSlow40 * fSlow49) / fSlow45), fSlow46 * fSlow49 + fSlow50 * fSlow51 / fSlow52) + 6.2831855f) / fSlow24);
		float fSlow54 = std::floor(fSlow53);
		float fSlow55 = fSlow53 - fSlow54;
		float fSlow56 = static_cast<float>(fButton0);
		int iSlow57 = fSlow56 > 0.0f;
		float fSlow58 = static_cast<float>(iSlow57);
		float fSlow59 = static_cast<float>(-(iSlow57 + -1));
		float fSlow60 = fConst11 * (0.9996f * fSlow58 + 0.9f * getValueReleaseLoopGain(fSlow13) * fSlow59);
		float fSlow61 = getValueDCBa1(fSlow13);
		float fSlow62 = 1.0f - fSlow61;
		float fSlow63 = 0.25f * static_cast<float>(fHslider6);
		float fSlow64 = getValueLoudPole(fSlow13);
		float fSlow65 = getValueLoudGain(fSlow13) * (fSlow63 + (0.98f - fSlow64));
		float fSlow66 = 1.6298145e-09f * fSlow65 * static_cast<float>(fSlow13 < 45.0f);
		int iSlow67 = iSlow57 > 0;
		float fSlow68 = std::exp(-(fConst16 / (getValueDryTapAmpT60(fSlow13) * std::min<float>(1.0f, std::max<float>(0.0f, fSlow56)))));
		int iSlow69 = iSlow57 < 1;
		float fSlow70 = fConst17 * fSlow59;
		float fSlow71 = static_cast<float>(fHslider7);
		float fSlow72 = fSlow58 * std::exp(-(fConst18 / fSlow71));
		float fSlow73 = fConst19 * fSlow71;
		float fSlow74 = 0.2f * getValueSustainPedalLevel(fSlow13);
		float fSlow75 = fSlow64 + (0.02f - fSlow63);
		float fSlow76 = fSlow61 - 1.0f;
		float fSlow77 = 3.7f * fSlow27;
		float fSlow78 = fSlow36 + fSlow42 - fSlow39;
		float fSlow79 = (fSlow48 + fSlow78 * fSlow30) / fSlow45 + 1.0f;
		float fSlow80 = fSlow40 * fSlow78;
		int iSlow81 = static_cast<int>(fConst14 * ((fSlow31 + std::atan2(-(fSlow26 * (fSlow78 * fSlow46 - fSlow40 * fSlow79) / fSlow45), fSlow46 * fSlow79 + fSlow80 * fSlow51 / fSlow52) + 6.2831855f) / fSlow24));
		int iSlow82 = std::min<int>(4097, std::max<int>(0, iSlow81 + 1));
		float fSlow83 = 1.0f / fSlow45;
		float fSlow84 = fSlow22 - fSlow23;
		float fSlow85 = fConst15 * fSlow84;
		float fSlow86 = std::sin(fSlow85);
		float fSlow87 = std::cos(fSlow85);
		float fSlow88 = 3.0f * std::atan2(fSlow21 * fSlow86, fSlow28 + fSlow29 * fSlow87);
		float fSlow89 = fSlow87 * fSlow40 / fSlow45 + 1.0f;
		float fSlow90 = (fSlow87 * fSlow43 + fSlow48) / fSlow45 + 1.0f;
		float fSlow91 = synth_piano_faustpower2_f(fSlow86) * fSlow40;
		float fSlow92 = fConst14 * ((fSlow88 + std::atan2(-(fSlow86 * (fSlow43 * fSlow89 - fSlow40 * fSlow90) / fSlow45), fSlow89 * fSlow90 + fSlow91 * fSlow43 / fSlow52) + 6.2831855f) / fSlow84);
		float fSlow93 = std::floor(fSlow92);
		float fSlow94 = fSlow93 + (1.0f - fSlow92);
		float fSlow95 = (fSlow48 + fSlow87 * fSlow78) / fSlow45 + 1.0f;
		int iSlow96 = static_cast<int>(fConst14 * ((fSlow88 + std::atan2(-(fSlow86 * (fSlow89 * fSlow78 - fSlow40 * fSlow95) / fSlow45), fSlow89 * fSlow95 + fSlow91 * fSlow78 / fSlow52) + 6.2831855f) / fSlow84));
		int iSlow97 = std::min<int>(4097, std::max<int>(0, iSlow96));
		float fSlow98 = fSlow92 - fSlow93;
		int iSlow99 = std::min<int>(4097, std::max<int>(0, iSlow96 + 1));
		float fSlow100 = fSlow54 + (1.0f - fSlow53);
		int iSlow101 = std::min<int>(4097, std::max<int>(0, iSlow81));
		float fSlow102 = fSlow14 * fSlow15;
		float fSlow103 = fConst21 * fSlow102;
		float fSlow104 = fConst22 * fSlow102;
		float fSlow105 = 8.8e+02f * std::cos(fConst23 * (fSlow14 / getValueStrikePosition(fSlow13)));
		float fSlow106 = fSlow14 + std::pow(2.0f, 0.083333336f * (fSlow13 + -129.0f));
		float fSlow107 = 4.4e+02f * fSlow106;
		float fSlow108 = fSlow23 + fSlow107;
		float fSlow109 = fConst15 * fSlow108;
		float fSlow110 = std::sin(fSlow109);
		float fSlow111 = std::cos(fSlow109);
		float fSlow112 = 3.0f * std::atan2(fSlow21 * fSlow110, fSlow28 + fSlow29 * fSlow111);
		float fSlow113 = fSlow40 * fSlow111 / fSlow45 + 1.0f;
		float fSlow114 = (fSlow48 + fSlow43 * fSlow111) / fSlow45 + 1.0f;
		float fSlow115 = synth_piano_faustpower2_f(fSlow110);
		float fSlow116 = fConst14 * ((fSlow112 + std::atan2(-(fSlow110 * (fSlow43 * fSlow113 - fSlow40 * fSlow114) / fSlow45), fSlow113 * fSlow114 + fSlow50 * fSlow115 / fSlow52) + 6.2831855f) / fSlow108);
		float fSlow117 = std::floor(fSlow116);
		float fSlow118 = fSlow116 - fSlow117;
		float fSlow119 = 1.1641532e-09f * fSlow65 * static_cast<float>((fSlow13 >= 45.0f) & (fSlow13 < 88.0f));
		float fSlow120 = (fSlow48 + fSlow78 * fSlow111) / fSlow45 + 1.0f;
		int iSlow121 = static_cast<int>(fConst14 * ((fSlow112 + std::atan2(-(fSlow110 * (fSlow78 * fSlow113 - fSlow40 * fSlow120) / fSlow45), fSlow113 * fSlow120 + fSlow80 * fSlow115 / fSlow52) + 6.2831855f) / fSlow108));
		int iSlow122 = std::min<int>(4097, std::max<int>(0, iSlow121 + 1));
		float fSlow123 = fSlow107 - fSlow23;
		float fSlow124 = fConst15 * fSlow123;
		float fSlow125 = std::sin(fSlow124);
		float fSlow126 = std::cos(fSlow124);
		float fSlow127 = 3.0f * std::atan2(fSlow21 * fSlow125, fSlow28 + fSlow29 * fSlow126);
		float fSlow128 = fSlow40 * fSlow126 / fSlow45 + 1.0f;
		float fSlow129 = (fSlow48 + fSlow43 * fSlow126) / fSlow45 + 1.0f;
		float fSlow130 = synth_piano_faustpower2_f(fSlow125);
		float fSlow131 = fConst14 * ((fSlow127 + std::atan2(-(fSlow125 * (fSlow43 * fSlow128 - fSlow40 * fSlow129) / fSlow45), fSlow128 * fSlow129 + fSlow50 * fSlow130 / fSlow52) + 6.2831855f) / fSlow123);
		float fSlow132 = std::floor(fSlow131);
		float fSlow133 = fSlow131 - fSlow132;
		float fSlow134 = (fSlow48 + fSlow78 * fSlow126) / fSlow45 + 1.0f;
		int iSlow135 = static_cast<int>(fConst14 * ((fSlow127 + std::atan2(-(fSlow125 * (fSlow78 * fSlow128 - fSlow40 * fSlow134) / fSlow45), fSlow128 * fSlow134 + fSlow80 * fSlow130 / fSlow52) + 6.2831855f) / fSlow123));
		int iSlow136 = std::min<int>(4097, std::max<int>(0, iSlow135 + 1));
		float fSlow137 = fConst23 * fSlow106;
		float fSlow138 = std::sin(fSlow137);
		float fSlow139 = std::cos(fSlow137);
		float fSlow140 = 3.0f * std::atan2(fSlow21 * fSlow138, fSlow28 + fSlow29 * fSlow139);
		float fSlow141 = fSlow40 * fSlow139 / fSlow45 + 1.0f;
		float fSlow142 = (fSlow48 + fSlow43 * fSlow139) / fSlow45 + 1.0f;
		float fSlow143 = synth_piano_faustpower2_f(fSlow138);
		float fSlow144 = fConst24 * ((fSlow140 + std::atan2(-(fSlow138 * (fSlow43 * fSlow141 - fSlow40 * fSlow142) / fSlow45), fSlow141 * fSlow142 + fSlow50 * fSlow143 / fSlow52) + 6.2831855f) / fSlow106);
		float fSlow145 = std::floor(fSlow144);
		float fSlow146 = fSlow145 + (1.0f - fSlow144);
		float fSlow147 = (fSlow48 + fSlow78 * fSlow139) / fSlow45 + 1.0f;
		int iSlow148 = static_cast<int>(fConst24 * ((fSlow140 + std::atan2(-(fSlow138 * (fSlow78 * fSlow141 - fSlow40 * fSlow147) / fSlow45), fSlow141 * fSlow147 + fSlow80 * fSlow143 / fSlow52) + 6.2831855f) / fSlow106));
		int iSlow149 = std::min<int>(4097, std::max<int>(0, iSlow148));
		float fSlow150 = fSlow144 - fSlow145;
		int iSlow151 = std::min<int>(4097, std::max<int>(0, iSlow148 + 1));
		float fSlow152 = fSlow132 + (1.0f - fSlow131);
		int iSlow153 = std::min<int>(4097, std::max<int>(0, iSlow135));
		float fSlow154 = fSlow117 + (1.0f - fSlow116);
		int iSlow155 = std::min<int>(4097, std::max<int>(0, iSlow121));
		float fSlow156 = getValueBq4_gEarBalled(fSlow13);
		float fSlow157 = 2.0f * fSlow156;
		float fSlow158 = static_cast<float>(fSlow13 >= 88.0f);
		float fSlow159 = 2.3283064e-10f * fSlow158;
		float fSlow160 = 1.1641532e-10f * fSlow158;
		float fSlow161 = std::pow(1e+01f, fConst25 * getValuer3db(fSlow13));
		float fSlow162 = 2.0f * std::cos(fConst23 * fSlow14 * getValueThirdPartialFactor(fSlow13));
		float fSlow163 = std::pow(1e+01f, fConst25 * getValuer2db(fSlow13));
		float fSlow164 = 2.0f * std::cos(fConst23 * fSlow14 * getValueSecondPartialFactor(fSlow13));
		float fSlow165 = std::pow(1e+01f, fConst25 * getValuer1_1db(fSlow13));
		float fSlow166 = std::cos(fConst23 * fSlow14);
		float fSlow167 = 2.0f * fSlow166;
		float fSlow168 = std::pow(1e+01f, 0.05f * getValueSecondStageAmpRatio(fSlow13));
		float fSlow169 = 1.0f - fSlow168;
		float fSlow170 = std::pow(1e+01f, fConst25 * getValuer1_2db(fSlow13));
		float fSlow171 = fSlow168 * synth_piano_faustpower2_f(fSlow165) + fSlow169 * synth_piano_faustpower2_f(fSlow170);
		float fSlow172 = 2.0f * fSlow166 * (fSlow168 * fSlow165 + fSlow169 * fSlow170);
		int iSlow173 = static_cast<int>(std::min<float>(4096.0f, std::max<float>(0.0f, fConst26 * (static_cast<float>(fHslider8) / fSlow14))));
		float fSlow174 = std::exp(-(fConst31 / fSlow0));
		float fSlow175 = synth_piano_faustpower2_f(fSlow174);
		float fSlow176 = 1.0f - fConst4 * fSlow175;
		float fSlow177 = 1.0f - fSlow175;
		float fSlow178 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow176) / synth_piano_faustpower2_f(fSlow177) + -1.0f));
		float fSlow179 = fSlow176 / fSlow177;
		float fSlow180 = fSlow174 * (fSlow178 + (1.0f - fSlow179));
		float fSlow181 = std::exp(-(fConst32 / fSlow0)) / fSlow174 + -1.0f;
		float fSlow182 = fSlow179 - fSlow178;
		float fSlow183 = std::exp(-(fConst38 / fSlow0));
		float fSlow184 = synth_piano_faustpower2_f(fSlow183);
		float fSlow185 = 1.0f - fConst4 * fSlow184;
		float fSlow186 = 1.0f - fSlow184;
		float fSlow187 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow185) / synth_piano_faustpower2_f(fSlow186) + -1.0f));
		float fSlow188 = fSlow185 / fSlow186;
		float fSlow189 = fSlow183 * (fSlow187 + (1.0f - fSlow188));
		float fSlow190 = std::exp(-(fConst39 / fSlow0)) / fSlow183 + -1.0f;
		float fSlow191 = fSlow188 - fSlow187;
		float fSlow192 = std::exp(-(fConst45 / fSlow0));
		float fSlow193 = synth_piano_faustpower2_f(fSlow192);
		float fSlow194 = 1.0f - fConst4 * fSlow193;
		float fSlow195 = 1.0f - fSlow193;
		float fSlow196 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow194) / synth_piano_faustpower2_f(fSlow195) + -1.0f));
		float fSlow197 = fSlow194 / fSlow195;
		float fSlow198 = fSlow192 * (fSlow196 + (1.0f - fSlow197));
		float fSlow199 = std::exp(-(fConst46 / fSlow0)) / fSlow192 + -1.0f;
		float fSlow200 = fSlow197 - fSlow196;
		float fSlow201 = std::exp(-(fConst52 / fSlow0));
		float fSlow202 = synth_piano_faustpower2_f(fSlow201);
		float fSlow203 = 1.0f - fConst4 * fSlow202;
		float fSlow204 = 1.0f - fSlow202;
		float fSlow205 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow203) / synth_piano_faustpower2_f(fSlow204) + -1.0f));
		float fSlow206 = fSlow203 / fSlow204;
		float fSlow207 = fSlow201 * (fSlow205 + (1.0f - fSlow206));
		float fSlow208 = std::exp(-(fConst53 / fSlow0)) / fSlow201 + -1.0f;
		float fSlow209 = fSlow206 - fSlow205;
		float fSlow210 = 12.0f * (1.0f - fSlow10);
		float fSlow211 = std::exp(-(fConst59 / fSlow0));
		float fSlow212 = synth_piano_faustpower2_f(fSlow211);
		float fSlow213 = 1.0f - fConst4 * fSlow212;
		float fSlow214 = 1.0f - fSlow212;
		float fSlow215 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow213) / synth_piano_faustpower2_f(fSlow214) + -1.0f));
		float fSlow216 = fSlow213 / fSlow214;
		float fSlow217 = fSlow211 * (fSlow215 + (1.0f - fSlow216));
		float fSlow218 = std::exp(-(fConst60 / fSlow0)) / fSlow211 + -1.0f;
		float fSlow219 = fSlow216 - fSlow215;
		float fSlow220 = std::exp(-(fConst66 / fSlow0));
		float fSlow221 = synth_piano_faustpower2_f(fSlow220);
		float fSlow222 = 1.0f - fConst4 * fSlow221;
		float fSlow223 = 1.0f - fSlow221;
		float fSlow224 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow222) / synth_piano_faustpower2_f(fSlow223) + -1.0f));
		float fSlow225 = fSlow222 / fSlow223;
		float fSlow226 = fSlow220 * (fSlow224 + (1.0f - fSlow225));
		float fSlow227 = std::exp(-(fConst67 / fSlow0)) / fSlow220 + -1.0f;
		float fSlow228 = fSlow225 - fSlow224;
		float fSlow229 = std::exp(-(fConst73 / fSlow0));
		float fSlow230 = synth_piano_faustpower2_f(fSlow229);
		float fSlow231 = 1.0f - fConst4 * fSlow230;
		float fSlow232 = 1.0f - fSlow230;
		float fSlow233 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow231) / synth_piano_faustpower2_f(fSlow232) + -1.0f));
		float fSlow234 = fSlow231 / fSlow232;
		float fSlow235 = fSlow229 * (fSlow233 + (1.0f - fSlow234));
		float fSlow236 = std::exp(-(fConst74 / fSlow0)) / fSlow229 + -1.0f;
		float fSlow237 = fSlow234 - fSlow233;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec11[0] = -(fConst7 * (fConst8 * fRec11[1] - (fRec7[1] + fRec7[2])));
			fRec10[0] = fSlow7 * (fRec7[1] + fSlow8 * fRec11[0]) + fSlow9 * fRec10[1];
			fVec0[IOTA0 & 32767] = 0.35355338f * fRec10[0] + 1e-20f;
			float fTemp0 = 0.6f * fRec8[1] + fVec0[(IOTA0 - iConst10) & 32767];
			fRec12[0] = fSlow12 + fConst12 * fRec12[1];
			fRec20[0] = fSlow60 + fConst12 * fRec20[1];
			iRec26[0] = 1103515245 * iRec26[1] + 12345;
			iRec28[0] = iSlow57 * iRec28[1] + 1;
			float fTemp1 = static_cast<float>(iRec28[0] + -1);
			float fTemp2 = static_cast<float>((fTemp1 < 2.0f) & iSlow67);
			float fTemp3 = 0.030197384f * fTemp2 + fSlow68 * static_cast<float>((fTemp1 >= 2.0f) | iSlow69);
			fRec27[0] = fRec27[1] * fTemp3 + 0.15f * fTemp2 * (1.0f - fTemp3);
			int iTemp4 = fTemp1 < fSlow73;
			float fTemp5 = fSlow72 * static_cast<float>(iTemp4) + fConst20 * static_cast<float>(iSlow57 * (fTemp1 >= fSlow73));
			fRec29[0] = fRec29[1] * (fSlow70 + fTemp5) + fSlow74 * (1.0f - fTemp5 - fSlow70) * static_cast<float>(iTemp4 & iSlow67);
			float fTemp6 = static_cast<float>(iRec26[0]) * (fRec27[0] + fRec29[0]);
			fRec25[0] = fSlow66 * fTemp6 + fSlow75 * fRec25[1];
			fRec24[0] = fSlow65 * fRec25[0] + fSlow75 * fRec24[1];
			fRec23[0] = fSlow65 * fRec24[0] + fSlow75 * fRec23[1];
			fRec22[0] = fSlow65 * fRec23[0] + fSlow75 * fRec22[1];
			fRec21[0] = 0.5f * (fSlow62 * fRec22[0] + fSlow76 * fRec22[1]) - fSlow61 * fRec21[1];
			float fTemp7 = fRec20[0] * (fRec21[0] + fRec14[1]);
			fVec1[0] = fTemp7;
			fRec19[0] = fVec1[1] + fSlow77 * (fTemp7 - fRec19[1]);
			fRec18[0] = fRec19[1] + fSlow77 * (fRec19[0] - fRec18[1]);
			fRec17[IOTA0 & 8191] = fRec18[1] + fSlow77 * (fRec18[0] - fRec17[(IOTA0 - 1) & 8191]);
			float fTemp8 = fSlow55 * fRec17[(IOTA0 - iSlow82) & 8191];
			float fTemp9 = fRec21[0] + fRec20[0] * fRec15[1];
			fVec2[0] = fTemp9;
			fRec33[0] = fVec2[1] + fSlow77 * (fTemp9 - fRec33[1]);
			fRec32[0] = fRec33[1] + fSlow77 * (fRec33[0] - fRec32[1]);
			fRec31[IOTA0 & 8191] = fRec32[1] + fSlow77 * (fRec32[0] - fRec31[(IOTA0 - 1) & 8191]);
			float fTemp10 = fSlow94 * fRec31[(IOTA0 - iSlow97) & 8191];
			float fTemp11 = fSlow98 * fRec31[(IOTA0 - iSlow99) & 8191];
			float fTemp12 = fSlow100 * fRec17[(IOTA0 - iSlow101) & 8191];
			float fTemp13 = fTemp8 + fTemp10 + fTemp11 + fTemp12;
			fVec3[0] = fTemp13;
			fRec30[0] = fSlow83 * (2.0f * (fSlow47 * fTemp13 + fSlow41 * fVec3[1]) - fSlow40 * fRec30[1]);
			fRec14[0] = fTemp8 + fRec30[0] + fTemp12;
			fRec15[0] = fTemp11 + fRec30[0] + fTemp10;
			float fRec16 = fTemp13;
			fRec13[0] = fSlow17 * fRec16 - fSlow103 * (fSlow104 * fRec13[2] - fSlow105 * fRec13[1]);
			fRec46[0] = fSlow119 * fTemp6 + fSlow75 * fRec46[1];
			fRec45[0] = fSlow65 * fRec46[0] + fSlow75 * fRec45[1];
			fRec44[0] = fSlow65 * fRec45[0] + fSlow75 * fRec44[1];
			fRec43[0] = fSlow65 * fRec44[0] + fSlow75 * fRec43[1];
			fRec42[0] = 0.5f * (fSlow62 * fRec43[0] + fSlow76 * fRec43[1]) - fSlow61 * fRec42[1];
			float fTemp14 = fRec20[0] * (fRec42[0] + fRec35[1]);
			fVec4[0] = fTemp14;
			fRec41[0] = fVec4[1] + fSlow77 * (fTemp14 - fRec41[1]);
			fRec40[0] = fRec41[1] + fSlow77 * (fRec41[0] - fRec40[1]);
			fRec39[IOTA0 & 8191] = fRec40[1] + fSlow77 * (fRec40[0] - fRec39[(IOTA0 - 1) & 8191]);
			float fTemp15 = fSlow118 * fRec39[(IOTA0 - iSlow122) & 8191];
			float fTemp16 = fRec42[0] + fRec20[0] * fRec37[1];
			fVec5[0] = fTemp16;
			fRec50[0] = fVec5[1] + fSlow77 * (fTemp16 - fRec50[1]);
			fRec49[0] = fRec50[1] + fSlow77 * (fRec50[0] - fRec49[1]);
			fRec48[IOTA0 & 8191] = fRec49[1] + fSlow77 * (fRec49[0] - fRec48[(IOTA0 - 1) & 8191]);
			float fTemp17 = fSlow133 * fRec48[(IOTA0 - iSlow136) & 8191];
			float fTemp18 = fRec42[0] + fRec20[0] * fRec36[1];
			fVec6[0] = fTemp18;
			fRec53[0] = fVec6[1] + fSlow77 * (fTemp18 - fRec53[1]);
			fRec52[0] = fRec53[1] + fSlow77 * (fRec53[0] - fRec52[1]);
			fRec51[IOTA0 & 8191] = fRec52[1] + fSlow77 * (fRec52[0] - fRec51[(IOTA0 - 1) & 8191]);
			float fTemp19 = fSlow146 * fRec51[(IOTA0 - iSlow149) & 8191];
			float fTemp20 = fSlow150 * fRec51[(IOTA0 - iSlow151) & 8191];
			float fTemp21 = fSlow152 * fRec48[(IOTA0 - iSlow153) & 8191];
			float fTemp22 = fSlow154 * fRec39[(IOTA0 - iSlow155) & 8191];
			float fTemp23 = fTemp15 + fTemp17 + fTemp19 + fTemp20 + fTemp21 + fTemp22;
			fVec7[0] = fTemp23;
			fRec47[0] = fSlow83 * (2.0f * (fSlow47 * fTemp23 + fSlow41 * fVec7[1]) - fSlow40 * fRec47[1]);
			fRec35[0] = fTemp15 + fRec47[0] + fTemp22;
			fRec36[0] = fTemp20 + fRec47[0] + fTemp19;
			fRec37[0] = fTemp17 + fRec47[0] + fTemp21;
			float fRec38 = fTemp23;
			fRec34[0] = fSlow17 * fRec38 - fSlow103 * (fSlow104 * fRec34[2] - fSlow105 * fRec34[1]);
			fVec8[0] = fSlow159 * fTemp6;
			float fTemp24 = 0.5f * fVec8[1] + fSlow160 * fTemp6;
			fVec9[0] = fTemp24;
			fRec63[0] = -(0.5f * (fSlow62 * fTemp24 + fSlow76 * fVec9[1]) + fSlow61 * fRec63[1]);
			fRec62[0] = fSlow65 * fRec63[0] + fSlow75 * fRec62[1];
			fRec61[0] = fSlow65 * fRec62[0] + fSlow75 * fRec61[1];
			fRec60[0] = fSlow65 * fRec61[0] + fSlow75 * fRec60[1];
			fRec59[0] = fSlow65 * fRec60[0] + fSlow75 * fRec59[1];
			fRec58[0] = fSlow156 * (fRec59[0] - fRec59[1]) - fSlow161 * (fSlow161 * fRec58[2] - fSlow162 * fRec58[1]);
			fRec57[0] = fSlow157 * fRec58[0] - fSlow163 * (fSlow163 * fRec57[2] - fSlow164 * fRec57[1]);
			fRec56[0] = fRec57[0] - fSlow165 * (fSlow165 * fRec56[2] - fSlow167 * fRec56[1]);
			fRec55[0] = fRec56[0] + fSlow171 * fRec56[2] - (fSlow172 * fRec56[1] + fSlow170 * (fSlow170 * fRec55[2] - fSlow167 * fRec55[1]));
			fRec54[0] = fSlow62 * fRec55[0] - fSlow61 * fRec54[1];
			float fTemp25 = fSlow16 * (fRec13[0] - fRec13[2] + (fRec34[0] - fRec34[2])) + fRec54[0] + fRec16 + fRec38;
			fVec10[IOTA0 & 4095] = fTemp25;
			float fTemp26 = fVec10[(IOTA0 - iSlow173) & 4095];
			fVec11[IOTA0 & 4095] = fSlow11 * fRec12[0] * fTemp26;
			float fTemp27 = 0.3f * fVec11[(IOTA0 - iConst27) & 4095];
			fVec12[IOTA0 & 2047] = fTemp0 - fTemp27;
			fRec8[0] = fVec12[(IOTA0 - iConst28) & 2047];
			float fRec9 = 0.6f * (fTemp27 - fTemp0);
			fRec67[0] = -(fConst7 * (fConst8 * fRec67[1] - (fRec3[1] + fRec3[2])));
			fRec66[0] = fSlow180 * (fRec3[1] + fSlow181 * fRec67[0]) + fSlow182 * fRec66[1];
			fVec13[IOTA0 & 32767] = 0.35355338f * fRec66[0] + 1e-20f;
			float fTemp28 = 0.6f * fRec64[1] + fVec13[(IOTA0 - iConst34) & 32767];
			fVec14[IOTA0 & 4095] = fTemp28 - fTemp27;
			fRec64[0] = fVec14[(IOTA0 - iConst35) & 4095];
			float fRec65 = 0.6f * (fTemp27 - fTemp28);
			fRec71[0] = -(fConst7 * (fConst8 * fRec71[1] - (fRec5[1] + fRec5[2])));
			fRec70[0] = fSlow189 * (fRec5[1] + fSlow190 * fRec71[0]) + fSlow191 * fRec70[1];
			fVec15[IOTA0 & 16383] = 0.35355338f * fRec70[0] + 1e-20f;
			float fTemp29 = fVec15[(IOTA0 - iConst41) & 16383] + fTemp27 + 0.6f * fRec68[1];
			fVec16[IOTA0 & 4095] = fTemp29;
			fRec68[0] = fVec16[(IOTA0 - iConst42) & 4095];
			float fRec69 = -(0.6f * fTemp29);
			fRec75[0] = -(fConst7 * (fConst8 * fRec75[1] - (fRec1[1] + fRec1[2])));
			fRec74[0] = fSlow198 * (fRec1[1] + fSlow199 * fRec75[0]) + fSlow200 * fRec74[1];
			fVec17[IOTA0 & 32767] = 0.35355338f * fRec74[0] + 1e-20f;
			float fTemp30 = fTemp27 + 0.6f * fRec72[1] + fVec17[(IOTA0 - iConst48) & 32767];
			fVec18[IOTA0 & 4095] = fTemp30;
			fRec72[0] = fVec18[(IOTA0 - iConst49) & 4095];
			float fRec73 = -(0.6f * fTemp30);
			fRec79[0] = -(fConst7 * (fConst8 * fRec79[1] - (fRec6[1] + fRec6[2])));
			fRec78[0] = fSlow207 * (fRec6[1] + fSlow208 * fRec79[0]) + fSlow209 * fRec78[1];
			fVec19[IOTA0 & 16383] = 0.35355338f * fRec78[0] + 1e-20f;
			fVec20[IOTA0 & 4095] = fSlow210 * fRec12[0] * fTemp25;
			float fTemp31 = 0.3f * fVec20[(IOTA0 - iConst27) & 4095];
			float fTemp32 = fVec19[(IOTA0 - iConst55) & 16383] - (fTemp31 + 0.6f * fRec76[1]);
			fVec21[IOTA0 & 2047] = fTemp32;
			fRec76[0] = fVec21[(IOTA0 - iConst56) & 2047];
			float fRec77 = 0.6f * fTemp32;
			fRec83[0] = -(fConst7 * (fConst8 * fRec83[1] - (fRec2[1] + fRec2[2])));
			fRec82[0] = fSlow217 * (fRec2[1] + fSlow218 * fRec83[0]) + fSlow219 * fRec82[1];
			fVec22[IOTA0 & 16383] = 0.35355338f * fRec82[0] + 1e-20f;
			float fTemp33 = fVec22[(IOTA0 - iConst62) & 16383] - (fTemp31 + 0.6f * fRec80[1]);
			fVec23[IOTA0 & 4095] = fTemp33;
			fRec80[0] = fVec23[(IOTA0 - iConst63) & 4095];
			float fRec81 = 0.6f * fTemp33;
			fRec87[0] = -(fConst7 * (fConst8 * fRec87[1] - (fRec4[1] + fRec4[2])));
			fRec86[0] = fSlow226 * (fRec4[1] + fSlow227 * fRec87[0]) + fSlow228 * fRec86[1];
			fVec24[IOTA0 & 16383] = 0.35355338f * fRec86[0] + 1e-20f;
			float fTemp34 = fTemp31 + fVec24[(IOTA0 - iConst69) & 16383] - 0.6f * fRec84[1];
			fVec25[IOTA0 & 4095] = fTemp34;
			fRec84[0] = fVec25[(IOTA0 - iConst70) & 4095];
			float fRec85 = 0.6f * fTemp34;
			fRec91[0] = -(fConst7 * (fConst8 * fRec91[1] - (fRec0[1] + fRec0[2])));
			fRec90[0] = fSlow235 * (fRec0[1] + fSlow236 * fRec91[0]) + fSlow237 * fRec90[1];
			fVec26[IOTA0 & 16383] = 0.35355338f * fRec90[0] + 1e-20f;
			float fTemp35 = fVec26[(IOTA0 - iConst76) & 16383] + fTemp31 - 0.6f * fRec88[1];
			fVec27[IOTA0 & 2047] = fTemp35;
			fRec88[0] = fVec27[(IOTA0 - iConst77) & 2047];
			float fRec89 = 0.6f * fTemp35;
			float fTemp36 = fRec89 + fRec85;
			float fTemp37 = fRec77 + fRec81 + fTemp36;
			fRec0[0] = fRec8[1] + fRec64[1] + fRec68[1] + fRec72[1] + fRec76[1] + fRec80[1] + fRec84[1] + fRec88[1] + fRec9 + fRec65 + fRec69 + fRec73 + fTemp37;
			fRec1[0] = fRec76[1] + fRec80[1] + fRec84[1] + fRec88[1] + fTemp37 - (fRec8[1] + fRec64[1] + fRec68[1] + fRec72[1] + fRec9 + fRec65 + fRec73 + fRec69);
			float fTemp38 = fRec81 + fRec77;
			fRec2[0] = fRec68[1] + fRec72[1] + fRec84[1] + fRec88[1] + fRec69 + fRec73 + fTemp36 - (fRec8[1] + fRec64[1] + fRec76[1] + fRec80[1] + fRec9 + fRec65 + fTemp38);
			fRec3[0] = fRec8[1] + fRec64[1] + fRec84[1] + fRec88[1] + fRec9 + fRec65 + fTemp36 - (fRec68[1] + fRec72[1] + fRec76[1] + fRec80[1] + fRec69 + fRec73 + fTemp38);
			float fTemp39 = fRec89 + fRec81;
			float fTemp40 = fRec85 + fRec77;
			fRec4[0] = fRec64[1] + fRec72[1] + fRec80[1] + fRec88[1] + fRec65 + fRec73 + fTemp39 - (fRec8[1] + fRec68[1] + fRec76[1] + fRec84[1] + fRec9 + fRec69 + fTemp40);
			fRec5[0] = fRec8[1] + fRec68[1] + fRec80[1] + fRec88[1] + fRec9 + fRec69 + fTemp39 - (fRec64[1] + fRec72[1] + fRec76[1] + fRec84[1] + fRec65 + fRec73 + fTemp40);
			float fTemp41 = fRec89 + fRec77;
			float fTemp42 = fRec85 + fRec81;
			fRec6[0] = fRec8[1] + fRec72[1] + fRec76[1] + fRec88[1] + fRec9 + fRec73 + fTemp41 - (fRec64[1] + fRec68[1] + fRec80[1] + fRec84[1] + fRec65 + fRec69 + fTemp42);
			fRec7[0] = fRec64[1] + fRec68[1] + fRec76[1] + fRec88[1] + fRec65 + fRec69 + fTemp41 - (fRec8[1] + fRec72[1] + fRec80[1] + fRec84[1] + fRec9 + fRec73 + fTemp42);
			float fTemp43 = 1.0f - fRec12[0];
			output0[i0] = static_cast<FAUSTFLOAT>(0.37f * (fRec1[0] + fRec2[0]) + fSlow210 * fTemp43 * fTemp25);
			output1[i0] = static_cast<FAUSTFLOAT>(0.37f * (fRec1[0] - fRec2[0]) + fSlow11 * fTemp43 * fTemp26);
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA0 = IOTA0 + 1;
			fRec12[1] = fRec12[0];
			fRec20[1] = fRec20[0];
			iRec26[1] = iRec26[0];
			iRec28[1] = iRec28[0];
			fRec27[1] = fRec27[0];
			fRec29[1] = fRec29[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fVec1[1] = fVec1[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fVec2[1] = fVec2[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fVec3[1] = fVec3[0];
			fRec30[1] = fRec30[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fVec4[1] = fVec4[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fVec5[1] = fVec5[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fVec6[1] = fVec6[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fVec7[1] = fVec7[0];
			fRec47[1] = fRec47[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fVec8[1] = fVec8[0];
			fVec9[1] = fVec9[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
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
