/* ------------------------------------------------------------
name: "synth.voice_fofc"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -cn synth_voice_fofc -scn synth_voice_fofc_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_voice_fofc_H__
#define  __synth_voice_fofc_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_voice_fofc_dsp.h ********************************
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

class FAUST_API synth_voice_fofc_dsp {

    public:

        synth_voice_fofc_dsp() = default;
        virtual ~synth_voice_fofc_dsp() = default;

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
        virtual ::synth_voice_fofc_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public ::synth_voice_fofc_dsp {

    protected:

        ::synth_voice_fofc_dsp* fDSP;

    public:

        decorator_dsp(::synth_voice_fofc_dsp* synth_voice_fofc_dsp = nullptr):fDSP(synth_voice_fofc_dsp) {}
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
        virtual ::synth_voice_fofc_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_voice_fofc_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_voice_fofc_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_voice_fofc : public synth_voice_fofc_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_voice_fofc
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

class synth_voice_fofcSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec2[2];
	int fSampleRate;
	
  public:
	
	int getNumInputssynth_voice_fofcSIG0() {
		return 0;
	}
	int getNumOutputssynth_voice_fofcSIG0() {
		return 1;
	}
	
	void instanceInitsynth_voice_fofcSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iVec1[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec2[l4] = 0;
		}
	}
	
	void fillsynth_voice_fofcSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[0] = 1;
			iRec2[0] = (iVec1[1] + iRec2[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * static_cast<float>(iRec2[0]));
			iVec1[1] = iVec1[0];
			iRec2[1] = iRec2[0];
		}
	}

};

static synth_voice_fofcSIG0* newsynth_voice_fofcSIG0() { return (synth_voice_fofcSIG0*)new synth_voice_fofcSIG0(); }
static void deletesynth_voice_fofcSIG0(synth_voice_fofcSIG0* dsp) { delete dsp; }

static float ftbl0synth_voice_fofcSIG0[65536];

class synth_voice_fofc : public synth_voice_fofc_dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fVslider0;
	float fConst2;
	float fRec1[2];
	FAUSTFLOAT fHslider1;
	float fConst3;
	float fRec0[2];
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	float fRec3[2];
	float fRec6[2];
	float fRec5[2];
	float fVec2[3];
	float fConst4;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fRec8[2];
	float fRec7[2];
	float fRec9[2];
	float fRec4[3];
	float fRec10[2];
	float fVec3[3];
	float fRec12[2];
	float fRec13[2];
	float fRec11[3];
	float fRec14[2];
	float fVec4[3];
	float fRec16[2];
	float fRec17[2];
	float fRec15[3];
	float fRec18[2];
	float fVec5[3];
	float fRec20[2];
	float fRec21[2];
	float fRec19[3];
	float fRec22[2];
	float fVec6[3];
	float fRec24[2];
	float fRec25[2];
	float fRec23[3];
	float fRec26[2];
	float fVec7[3];
	float fRec28[2];
	float fRec29[2];
	float fRec27[3];
	float fRec30[2];
	float fVec8[3];
	float fRec32[2];
	float fRec33[2];
	float fRec31[3];
	float fRec34[2];
	float fVec9[3];
	float fRec36[2];
	float fRec37[2];
	float fRec35[3];
	float fRec38[2];
	float fVec10[3];
	float fRec40[2];
	float fRec41[2];
	float fRec39[3];
	float fRec42[2];
	float fVec11[3];
	float fRec44[2];
	float fRec45[2];
	float fRec43[3];
	float fRec46[2];
	float fVec12[3];
	float fRec48[2];
	float fRec49[2];
	float fRec47[3];
	float fRec50[2];
	float fVec13[3];
	float fRec52[2];
	float fRec53[2];
	float fRec51[3];
	float fRec54[2];
	float fRec56[2];
	float fRec57[2];
	float fRec55[3];
	float fRec58[2];
	float fRec60[2];
	float fRec61[2];
	float fRec59[3];
	float fRec62[2];
	float fRec64[2];
	float fRec65[2];
	float fRec63[3];
	float fRec66[2];
	float fRec68[2];
	float fRec69[2];
	float fRec67[3];
	float fRec70[2];
	float fRec72[2];
	float fRec73[2];
	float fRec71[3];
	float fRec74[2];
	float fRec76[2];
	float fRec77[2];
	float fRec75[3];
	float fRec78[2];
	float fRec80[2];
	float fRec81[2];
	float fRec79[3];
	float fRec82[2];
	float fRec84[2];
	float fRec85[2];
	float fRec83[3];
	float fRec86[2];
	float fRec88[2];
	float fRec89[2];
	float fRec87[3];
	float fRec90[2];
	float fRec92[2];
	float fRec93[2];
	float fRec91[3];
	float fRec94[2];
	float fRec96[2];
	float fRec97[2];
	float fRec95[3];
	float fRec98[2];
	float fRec100[2];
	float fRec101[2];
	float fRec99[3];
	float fRec102[2];
	float fRec104[2];
	float fRec105[2];
	float fRec103[3];
	float fRec106[2];
	float fRec108[2];
	float fRec109[2];
	float fRec107[3];
	float fRec110[2];
	float fRec112[2];
	float fRec113[2];
	float fRec111[3];
	float fRec114[2];
	float fRec116[2];
	float fRec117[2];
	float fRec115[3];
	float fRec118[2];
	float fRec120[2];
	float fRec121[2];
	float fRec119[3];
	float fRec122[2];
	float fRec124[2];
	float fRec125[2];
	float fRec123[3];
	float fRec126[2];
	float fRec128[2];
	float fRec129[2];
	float fRec127[3];
	float fRec130[2];
	float fRec132[2];
	float fRec133[2];
	float fRec131[3];
	float fRec134[2];
	float fRec136[2];
	float fRec137[2];
	float fRec135[3];
	float fRec138[2];
	float fRec140[2];
	float fRec141[2];
	float fRec139[3];
	float fRec142[2];
	float fRec144[2];
	float fRec145[2];
	float fRec143[3];
	float fRec146[2];
	float fRec148[2];
	float fRec149[2];
	float fRec147[3];
	float fRec150[2];
	float fRec152[2];
	float fRec153[2];
	float fRec151[3];
	float fRec154[2];
	float fRec156[2];
	float fRec157[2];
	float fRec155[3];
	float fRec158[2];
	float fRec160[2];
	float fRec161[2];
	float fRec159[3];
	float fRec162[2];
	float fRec164[2];
	float fRec165[2];
	float fRec163[3];
	float fRec166[2];
	float fRec168[2];
	float fRec169[2];
	float fRec167[3];
	float fRec170[2];
	float fRec172[2];
	float fRec173[2];
	float fRec171[3];
	float fRec174[2];
	float fRec176[2];
	float fRec177[2];
	float fRec175[3];
	float fRec178[2];
	float fRec180[2];
	float fRec181[2];
	float fRec179[3];
	float fRec182[2];
	float fRec184[2];
	float fRec185[2];
	float fRec183[3];
	float fRec186[2];
	float fRec188[2];
	float fRec189[2];
	float fRec187[3];
	float fRec190[2];
	float fRec192[2];
	float fRec193[2];
	float fRec191[3];
	float fRec194[2];
	float fRec196[2];
	float fRec197[2];
	float fRec195[3];
	float fRec198[2];
	float fRec200[2];
	float fRec201[2];
	float fRec199[3];
	float fRec202[2];
	float fRec204[2];
	float fRec205[2];
	float fRec203[3];
	float fRec206[2];
	float fRec208[2];
	float fRec209[2];
	float fRec207[3];
	float fRec210[2];
	float fRec212[2];
	float fRec213[2];
	float fRec211[3];
	float fRec214[2];
	float fRec216[2];
	float fRec217[2];
	float fRec215[3];
	float fRec218[2];
	float fRec220[2];
	float fRec221[2];
	float fRec219[3];
	float fRec222[2];
	float fRec224[2];
	float fRec225[2];
	float fRec223[3];
	float fRec226[2];
	float fRec228[2];
	float fRec229[2];
	float fRec227[3];
	float fRec230[2];
	float fRec232[2];
	float fRec233[2];
	float fRec231[3];
	float fRec234[2];
	float fRec236[2];
	float fRec237[2];
	float fRec235[3];
	float fRec238[2];
	float fRec240[2];
	float fRec241[2];
	float fRec239[3];
	float fRec242[2];
	float fRec244[2];
	float fRec245[2];
	float fRec243[3];
	float fRec246[2];
	float fConst5;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fCheckbox0;
	float fVec14[2];
	float fRec247[2];
	FAUSTFLOAT fHslider7;
	int iRec248[2];
	
 public:
	synth_voice_fofc() {
	}
	
	synth_voice_fofc(const synth_voice_fofc&) = default;
	
	virtual ~synth_voice_fofc() = default;
	
	synth_voice_fofc& operator=(const synth_voice_fofc&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/cycle:author", "Mike Olsen");
		m->declare("basics.lib/listInterp:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/pulse_countup_loop:author", "Vince");
		m->declare("basics.lib/resetCtr:author", "Mike Olsen");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -cn synth_voice_fofc -scn synth_voice_fofc_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("envelopes.lib/asr:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "1.3.0");
		m->declare("filename", "synth_voice_fofc.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "synth.voice_fofc");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.5.0");
		m->declare("oscillators.lib/hs_oscsin:author", "Mike Olsen");
		m->declare("oscillators.lib/hs_phasor:author", "Mike Olsen, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.7.0");
		m->declare("physmodels.lib/SFFormantModel:author", "Mike Olsen");
		m->declare("physmodels.lib/autobendFreq:author", "Mike Olsen");
		m->declare("physmodels.lib/fof:author", "Mike Olsen");
		m->declare("physmodels.lib/fofCycle:author", "Mike Olsen");
		m->declare("physmodels.lib/fofSH:author", "Mike Olsen");
		m->declare("physmodels.lib/formantFilterbank:author", "Mike Olsen");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/skirtWidthMultiplier:author", "Mike Olsen");
		m->declare("physmodels.lib/version", "1.2.0");
		m->declare("physmodels.lib/vocalEffort:author", "Mike Olsen");
		m->declare("physmodels.lib/voiceGender:author", "Mike Olsen");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		synth_voice_fofcSIG0* sig0 = newsynth_voice_fofcSIG0();
		sig0->instanceInitsynth_voice_fofcSIG0(sample_rate);
		sig0->fillsynth_voice_fofcSIG0(65536, ftbl0synth_voice_fofcSIG0);
		deletesynth_voice_fofcSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / fConst0;
		fConst4 = 3.1415927f / fConst0;
		fConst5 = 0.001f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.0f);
		fVslider0 = static_cast<FAUSTFLOAT>(4.4e+02f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider3 = static_cast<FAUSTFLOAT>(6.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider5 = static_cast<FAUSTFLOAT>(1e+01f);
		fHslider6 = static_cast<FAUSTFLOAT>(3e+01f);
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider7 = static_cast<FAUSTFLOAT>(5e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec0[l2] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec3[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec6[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec5[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fVec2[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec7[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec4[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec10[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fVec3[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec12[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec13[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec11[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec14[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = l19 + 1) {
			fVec4[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec16[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec17[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec15[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec18[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fVec5[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec20[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec21[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec19[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec22[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 3; l29 = l29 + 1) {
			fVec6[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec24[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec25[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 3; l32 = l32 + 1) {
			fRec23[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec26[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fVec7[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec28[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec29[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fRec27[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec30[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 3; l39 = l39 + 1) {
			fVec8[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec32[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec33[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec31[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec34[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fVec9[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec36[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec37[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec35[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec38[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = l49 + 1) {
			fVec10[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec40[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec41[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 3; l52 = l52 + 1) {
			fRec39[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec42[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 3; l54 = l54 + 1) {
			fVec11[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec44[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec45[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fRec43[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec46[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fVec12[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec48[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec49[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec47[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec50[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 3; l64 = l64 + 1) {
			fVec13[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec52[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec53[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 3; l67 = l67 + 1) {
			fRec51[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec54[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fRec56[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec57[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec55[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec58[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec60[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fRec61[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec59[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec62[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec64[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec65[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 3; l79 = l79 + 1) {
			fRec63[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec66[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec68[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec69[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 3; l83 = l83 + 1) {
			fRec67[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fRec70[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec72[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec73[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 3; l87 = l87 + 1) {
			fRec71[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec74[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec76[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec77[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
			fRec75[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec78[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec80[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec81[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 3; l95 = l95 + 1) {
			fRec79[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec82[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec84[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec85[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 3; l99 = l99 + 1) {
			fRec83[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			fRec86[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec88[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fRec89[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 3; l103 = l103 + 1) {
			fRec87[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			fRec90[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fRec92[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			fRec93[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 3; l107 = l107 + 1) {
			fRec91[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec94[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			fRec96[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			fRec97[l110] = 0.0f;
		}
		for (int l111 = 0; l111 < 3; l111 = l111 + 1) {
			fRec95[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec98[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec100[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fRec101[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 3; l115 = l115 + 1) {
			fRec99[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			fRec102[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fRec104[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			fRec105[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 3; l119 = l119 + 1) {
			fRec103[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fRec106[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			fRec108[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 2; l122 = l122 + 1) {
			fRec109[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 3; l123 = l123 + 1) {
			fRec107[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			fRec110[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec112[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec113[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 3; l127 = l127 + 1) {
			fRec111[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			fRec114[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fRec116[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			fRec117[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 3; l131 = l131 + 1) {
			fRec115[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec118[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fRec120[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			fRec121[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 3; l135 = l135 + 1) {
			fRec119[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
			fRec122[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 2; l137 = l137 + 1) {
			fRec124[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 2; l138 = l138 + 1) {
			fRec125[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 3; l139 = l139 + 1) {
			fRec123[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
			fRec126[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 2; l141 = l141 + 1) {
			fRec128[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 2; l142 = l142 + 1) {
			fRec129[l142] = 0.0f;
		}
		for (int l143 = 0; l143 < 3; l143 = l143 + 1) {
			fRec127[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fRec130[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec132[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 2; l146 = l146 + 1) {
			fRec133[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 3; l147 = l147 + 1) {
			fRec131[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fRec134[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fRec136[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 2; l150 = l150 + 1) {
			fRec137[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 3; l151 = l151 + 1) {
			fRec135[l151] = 0.0f;
		}
		for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
			fRec138[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
			fRec140[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 2; l154 = l154 + 1) {
			fRec141[l154] = 0.0f;
		}
		for (int l155 = 0; l155 < 3; l155 = l155 + 1) {
			fRec139[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec142[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
			fRec144[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 2; l158 = l158 + 1) {
			fRec145[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 3; l159 = l159 + 1) {
			fRec143[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fRec146[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 2; l161 = l161 + 1) {
			fRec148[l161] = 0.0f;
		}
		for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
			fRec149[l162] = 0.0f;
		}
		for (int l163 = 0; l163 < 3; l163 = l163 + 1) {
			fRec147[l163] = 0.0f;
		}
		for (int l164 = 0; l164 < 2; l164 = l164 + 1) {
			fRec150[l164] = 0.0f;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fRec152[l165] = 0.0f;
		}
		for (int l166 = 0; l166 < 2; l166 = l166 + 1) {
			fRec153[l166] = 0.0f;
		}
		for (int l167 = 0; l167 < 3; l167 = l167 + 1) {
			fRec151[l167] = 0.0f;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fRec154[l168] = 0.0f;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fRec156[l169] = 0.0f;
		}
		for (int l170 = 0; l170 < 2; l170 = l170 + 1) {
			fRec157[l170] = 0.0f;
		}
		for (int l171 = 0; l171 < 3; l171 = l171 + 1) {
			fRec155[l171] = 0.0f;
		}
		for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
			fRec158[l172] = 0.0f;
		}
		for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
			fRec160[l173] = 0.0f;
		}
		for (int l174 = 0; l174 < 2; l174 = l174 + 1) {
			fRec161[l174] = 0.0f;
		}
		for (int l175 = 0; l175 < 3; l175 = l175 + 1) {
			fRec159[l175] = 0.0f;
		}
		for (int l176 = 0; l176 < 2; l176 = l176 + 1) {
			fRec162[l176] = 0.0f;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			fRec164[l177] = 0.0f;
		}
		for (int l178 = 0; l178 < 2; l178 = l178 + 1) {
			fRec165[l178] = 0.0f;
		}
		for (int l179 = 0; l179 < 3; l179 = l179 + 1) {
			fRec163[l179] = 0.0f;
		}
		for (int l180 = 0; l180 < 2; l180 = l180 + 1) {
			fRec166[l180] = 0.0f;
		}
		for (int l181 = 0; l181 < 2; l181 = l181 + 1) {
			fRec168[l181] = 0.0f;
		}
		for (int l182 = 0; l182 < 2; l182 = l182 + 1) {
			fRec169[l182] = 0.0f;
		}
		for (int l183 = 0; l183 < 3; l183 = l183 + 1) {
			fRec167[l183] = 0.0f;
		}
		for (int l184 = 0; l184 < 2; l184 = l184 + 1) {
			fRec170[l184] = 0.0f;
		}
		for (int l185 = 0; l185 < 2; l185 = l185 + 1) {
			fRec172[l185] = 0.0f;
		}
		for (int l186 = 0; l186 < 2; l186 = l186 + 1) {
			fRec173[l186] = 0.0f;
		}
		for (int l187 = 0; l187 < 3; l187 = l187 + 1) {
			fRec171[l187] = 0.0f;
		}
		for (int l188 = 0; l188 < 2; l188 = l188 + 1) {
			fRec174[l188] = 0.0f;
		}
		for (int l189 = 0; l189 < 2; l189 = l189 + 1) {
			fRec176[l189] = 0.0f;
		}
		for (int l190 = 0; l190 < 2; l190 = l190 + 1) {
			fRec177[l190] = 0.0f;
		}
		for (int l191 = 0; l191 < 3; l191 = l191 + 1) {
			fRec175[l191] = 0.0f;
		}
		for (int l192 = 0; l192 < 2; l192 = l192 + 1) {
			fRec178[l192] = 0.0f;
		}
		for (int l193 = 0; l193 < 2; l193 = l193 + 1) {
			fRec180[l193] = 0.0f;
		}
		for (int l194 = 0; l194 < 2; l194 = l194 + 1) {
			fRec181[l194] = 0.0f;
		}
		for (int l195 = 0; l195 < 3; l195 = l195 + 1) {
			fRec179[l195] = 0.0f;
		}
		for (int l196 = 0; l196 < 2; l196 = l196 + 1) {
			fRec182[l196] = 0.0f;
		}
		for (int l197 = 0; l197 < 2; l197 = l197 + 1) {
			fRec184[l197] = 0.0f;
		}
		for (int l198 = 0; l198 < 2; l198 = l198 + 1) {
			fRec185[l198] = 0.0f;
		}
		for (int l199 = 0; l199 < 3; l199 = l199 + 1) {
			fRec183[l199] = 0.0f;
		}
		for (int l200 = 0; l200 < 2; l200 = l200 + 1) {
			fRec186[l200] = 0.0f;
		}
		for (int l201 = 0; l201 < 2; l201 = l201 + 1) {
			fRec188[l201] = 0.0f;
		}
		for (int l202 = 0; l202 < 2; l202 = l202 + 1) {
			fRec189[l202] = 0.0f;
		}
		for (int l203 = 0; l203 < 3; l203 = l203 + 1) {
			fRec187[l203] = 0.0f;
		}
		for (int l204 = 0; l204 < 2; l204 = l204 + 1) {
			fRec190[l204] = 0.0f;
		}
		for (int l205 = 0; l205 < 2; l205 = l205 + 1) {
			fRec192[l205] = 0.0f;
		}
		for (int l206 = 0; l206 < 2; l206 = l206 + 1) {
			fRec193[l206] = 0.0f;
		}
		for (int l207 = 0; l207 < 3; l207 = l207 + 1) {
			fRec191[l207] = 0.0f;
		}
		for (int l208 = 0; l208 < 2; l208 = l208 + 1) {
			fRec194[l208] = 0.0f;
		}
		for (int l209 = 0; l209 < 2; l209 = l209 + 1) {
			fRec196[l209] = 0.0f;
		}
		for (int l210 = 0; l210 < 2; l210 = l210 + 1) {
			fRec197[l210] = 0.0f;
		}
		for (int l211 = 0; l211 < 3; l211 = l211 + 1) {
			fRec195[l211] = 0.0f;
		}
		for (int l212 = 0; l212 < 2; l212 = l212 + 1) {
			fRec198[l212] = 0.0f;
		}
		for (int l213 = 0; l213 < 2; l213 = l213 + 1) {
			fRec200[l213] = 0.0f;
		}
		for (int l214 = 0; l214 < 2; l214 = l214 + 1) {
			fRec201[l214] = 0.0f;
		}
		for (int l215 = 0; l215 < 3; l215 = l215 + 1) {
			fRec199[l215] = 0.0f;
		}
		for (int l216 = 0; l216 < 2; l216 = l216 + 1) {
			fRec202[l216] = 0.0f;
		}
		for (int l217 = 0; l217 < 2; l217 = l217 + 1) {
			fRec204[l217] = 0.0f;
		}
		for (int l218 = 0; l218 < 2; l218 = l218 + 1) {
			fRec205[l218] = 0.0f;
		}
		for (int l219 = 0; l219 < 3; l219 = l219 + 1) {
			fRec203[l219] = 0.0f;
		}
		for (int l220 = 0; l220 < 2; l220 = l220 + 1) {
			fRec206[l220] = 0.0f;
		}
		for (int l221 = 0; l221 < 2; l221 = l221 + 1) {
			fRec208[l221] = 0.0f;
		}
		for (int l222 = 0; l222 < 2; l222 = l222 + 1) {
			fRec209[l222] = 0.0f;
		}
		for (int l223 = 0; l223 < 3; l223 = l223 + 1) {
			fRec207[l223] = 0.0f;
		}
		for (int l224 = 0; l224 < 2; l224 = l224 + 1) {
			fRec210[l224] = 0.0f;
		}
		for (int l225 = 0; l225 < 2; l225 = l225 + 1) {
			fRec212[l225] = 0.0f;
		}
		for (int l226 = 0; l226 < 2; l226 = l226 + 1) {
			fRec213[l226] = 0.0f;
		}
		for (int l227 = 0; l227 < 3; l227 = l227 + 1) {
			fRec211[l227] = 0.0f;
		}
		for (int l228 = 0; l228 < 2; l228 = l228 + 1) {
			fRec214[l228] = 0.0f;
		}
		for (int l229 = 0; l229 < 2; l229 = l229 + 1) {
			fRec216[l229] = 0.0f;
		}
		for (int l230 = 0; l230 < 2; l230 = l230 + 1) {
			fRec217[l230] = 0.0f;
		}
		for (int l231 = 0; l231 < 3; l231 = l231 + 1) {
			fRec215[l231] = 0.0f;
		}
		for (int l232 = 0; l232 < 2; l232 = l232 + 1) {
			fRec218[l232] = 0.0f;
		}
		for (int l233 = 0; l233 < 2; l233 = l233 + 1) {
			fRec220[l233] = 0.0f;
		}
		for (int l234 = 0; l234 < 2; l234 = l234 + 1) {
			fRec221[l234] = 0.0f;
		}
		for (int l235 = 0; l235 < 3; l235 = l235 + 1) {
			fRec219[l235] = 0.0f;
		}
		for (int l236 = 0; l236 < 2; l236 = l236 + 1) {
			fRec222[l236] = 0.0f;
		}
		for (int l237 = 0; l237 < 2; l237 = l237 + 1) {
			fRec224[l237] = 0.0f;
		}
		for (int l238 = 0; l238 < 2; l238 = l238 + 1) {
			fRec225[l238] = 0.0f;
		}
		for (int l239 = 0; l239 < 3; l239 = l239 + 1) {
			fRec223[l239] = 0.0f;
		}
		for (int l240 = 0; l240 < 2; l240 = l240 + 1) {
			fRec226[l240] = 0.0f;
		}
		for (int l241 = 0; l241 < 2; l241 = l241 + 1) {
			fRec228[l241] = 0.0f;
		}
		for (int l242 = 0; l242 < 2; l242 = l242 + 1) {
			fRec229[l242] = 0.0f;
		}
		for (int l243 = 0; l243 < 3; l243 = l243 + 1) {
			fRec227[l243] = 0.0f;
		}
		for (int l244 = 0; l244 < 2; l244 = l244 + 1) {
			fRec230[l244] = 0.0f;
		}
		for (int l245 = 0; l245 < 2; l245 = l245 + 1) {
			fRec232[l245] = 0.0f;
		}
		for (int l246 = 0; l246 < 2; l246 = l246 + 1) {
			fRec233[l246] = 0.0f;
		}
		for (int l247 = 0; l247 < 3; l247 = l247 + 1) {
			fRec231[l247] = 0.0f;
		}
		for (int l248 = 0; l248 < 2; l248 = l248 + 1) {
			fRec234[l248] = 0.0f;
		}
		for (int l249 = 0; l249 < 2; l249 = l249 + 1) {
			fRec236[l249] = 0.0f;
		}
		for (int l250 = 0; l250 < 2; l250 = l250 + 1) {
			fRec237[l250] = 0.0f;
		}
		for (int l251 = 0; l251 < 3; l251 = l251 + 1) {
			fRec235[l251] = 0.0f;
		}
		for (int l252 = 0; l252 < 2; l252 = l252 + 1) {
			fRec238[l252] = 0.0f;
		}
		for (int l253 = 0; l253 < 2; l253 = l253 + 1) {
			fRec240[l253] = 0.0f;
		}
		for (int l254 = 0; l254 < 2; l254 = l254 + 1) {
			fRec241[l254] = 0.0f;
		}
		for (int l255 = 0; l255 < 3; l255 = l255 + 1) {
			fRec239[l255] = 0.0f;
		}
		for (int l256 = 0; l256 < 2; l256 = l256 + 1) {
			fRec242[l256] = 0.0f;
		}
		for (int l257 = 0; l257 < 2; l257 = l257 + 1) {
			fRec244[l257] = 0.0f;
		}
		for (int l258 = 0; l258 < 2; l258 = l258 + 1) {
			fRec245[l258] = 0.0f;
		}
		for (int l259 = 0; l259 < 3; l259 = l259 + 1) {
			fRec243[l259] = 0.0f;
		}
		for (int l260 = 0; l260 < 2; l260 = l260 + 1) {
			fRec246[l260] = 0.0f;
		}
		for (int l261 = 0; l261 < 2; l261 = l261 + 1) {
			fVec14[l261] = 0.0f;
		}
		for (int l262 = 0; l262 < 2; l262 = l262 + 1) {
			fRec247[l262] = 0.0f;
		}
		for (int l263 = 0; l263 < 2; l263 = l263 + 1) {
			iRec248[l263] = 0;
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
	
	virtual synth_voice_fofc* clone() {
		return new synth_voice_fofc(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.voice_fofc");
		ui_interface->declare(&fHslider6, "unit", "ms");
		ui_interface->addHorizontalSlider("attack", &fHslider6, FAUSTFLOAT(3e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("freq", &fVslider0, FAUSTFLOAT(4.4e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("fvowel", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->addHorizontalSlider("ivoice", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider1, "unit", "ms");
		ui_interface->addHorizontalSlider("portamento", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4e+03f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider7, "unit", "ms");
		ui_interface->addHorizontalSlider("release", &fHslider7, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "unit", "Hz");
		ui_interface->addHorizontalSlider("vibr_freq", &fHslider3, FAUSTFLOAT(6.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("vibr_gain", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider5, "unit", "ms");
		ui_interface->addHorizontalSlider("wsmooth", &fHslider5, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = static_cast<int>(static_cast<float>(fHslider0));
		int iSlow1 = ((iSlow0 == 0) ? 1 : ((iSlow0 == 3) ? 1 : 0));
		int iSlow2 = iSlow1 == 0;
		float fSlow3 = fConst1 * static_cast<float>(fVslider0);
		float fSlow4 = 0.001f * static_cast<float>(fHslider1);
		int iSlow5 = std::fabs(fSlow4) < 1.1920929e-07f;
		float fSlow6 = ((iSlow5) ? 0.0f : std::exp(-(fConst3 / ((iSlow5) ? 1.0f : fSlow4))));
		float fSlow7 = 1.0f - fSlow6;
		float fSlow8 = 0.1f * static_cast<float>(fHslider2);
		float fSlow9 = fConst3 * static_cast<float>(fHslider3);
		float fSlow10 = 0.001f * static_cast<float>(fHslider5);
		int iSlow11 = std::fabs(fSlow10) < 1.1920929e-07f;
		float fSlow12 = ((iSlow11) ? 0.0f : std::exp(-(fConst3 / ((iSlow11) ? 1.0f : fSlow10))));
		float fSlow13 = static_cast<float>(fHslider4) * (1.0f - fSlow12);
		float fSlow14 = static_cast<float>(5 * iSlow1);
		int iSlow15 = iSlow1 >= 1;
		float fSlow16 = ((iSlow15) ? 174.61f : 82.41f);
		float fSlow17 = ((iSlow15) ? 1046.5f : 523.25f);
		float fSlow18 = fSlow17 - fSlow16;
		float fSlow19 = static_cast<float>(5 * iSlow0);
		int iSlow20 = iSlow0 != 2;
		float fSlow21 = 1.0f / std::max<float>(1.0f, fConst5 * static_cast<float>(fHslider6));
		float fSlow22 = static_cast<float>(fCheckbox0);
		float fSlow23 = 1.0f / std::max<float>(1.0f, fConst5 * static_cast<float>(fHslider7));
		int iSlow24 = fSlow22 == 0.0f;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec1[0] = fSlow3 + fConst2 * fRec1[1];
			fRec0[0] = fRec1[0] * fSlow7 + fSlow6 * fRec0[1];
			int iTemp0 = 1 - iVec0[1];
			float fTemp1 = ((iTemp0) ? 0.0f : fSlow9 + fRec3[1]);
			fRec3[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = fRec0[0] * (fSlow8 * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec3[0]), 65535))] + 1.0f);
			float fTemp3 = 0.00084f * (1e+03f - fTemp2) + 0.8f;
			float fTemp4 = 0.0036666666f * (4e+02f - fTemp2) + 3.0f;
			float fTemp5 = ((iTemp0) ? 0.0f : fRec6[1] + fConst3 * fTemp2);
			fRec6[0] = fTemp5 - std::floor(fTemp5);
			float fTemp6 = static_cast<float>((fRec6[0] - fRec6[1]) < 0.0f);
			fRec5[0] = fTemp6 + fRec5[1] * static_cast<float>(fRec5[1] <= 11.0f);
			float fTemp7 = static_cast<float>(fRec5[0] == 1.0f) * fTemp6;
			fVec2[0] = fTemp7;
			int iTemp8 = static_cast<int>(fTemp7);
			fRec8[0] = fSlow13 + fSlow12 * fRec8[1];
			float fTemp9 = fRec8[0] + fSlow14;
			int iTemp10 = fTemp9 >= 5.0f;
			int iTemp11 = fTemp9 >= 3.0f;
			int iTemp12 = fTemp9 >= 2.0f;
			int iTemp13 = fTemp9 >= 1.0f;
			int iTemp14 = fTemp9 >= 4.0f;
			int iTemp15 = fTemp9 >= 8.0f;
			int iTemp16 = fTemp9 >= 7.0f;
			int iTemp17 = fTemp9 >= 6.0f;
			float fTemp18 = ((iTemp10) ? ((iTemp15) ? 2.0f : ((iTemp16) ? 3.0f : ((iTemp17) ? 3.0f : 2.0f))) : ((iTemp11) ? ((iTemp14) ? 1.5f : 1.0f) : ((iTemp12) ? 1.25f : ((iTemp13) ? 1.25f : 1.0f))));
			float fTemp19 = fTemp18 + (((iTemp10) ? ((iTemp15) ? 12.0f : ((iTemp16) ? 12.0f : ((iTemp17) ? 12.0f : 15.0f))) : ((iTemp11) ? ((iTemp14) ? 4.0f : 1e+01f) : ((iTemp12) ? 2.5f : ((iTemp13) ? 2.5f : 1e+01f)))) - fTemp18) * ((fTemp2 <= fSlow16) ? 0.0f : ((fTemp2 >= fSlow17) ? 1.0f : (fTemp2 - fSlow16) / fSlow18));
			float fTemp20 = fSlow19 + fRec8[0];
			int iTemp21 = fTemp20 < 23.0f;
			int iTemp22 = fTemp20 < 24.0f;
			float fTemp23 = fSlow19 + fRec8[0] + -23.0f;
			int iTemp24 = fTemp20 < 22.0f;
			float fTemp25 = fSlow19 + fRec8[0] + -22.0f;
			int iTemp26 = fTemp20 < 21.0f;
			float fTemp27 = fSlow19 + fRec8[0] + -21.0f;
			int iTemp28 = fTemp20 < 2e+01f;
			float fTemp29 = fSlow19 + fRec8[0] + -2e+01f;
			float fTemp30 = 2e+01f * fTemp29;
			int iTemp31 = fTemp20 < 19.0f;
			int iTemp32 = fTemp20 < 18.0f;
			float fTemp33 = fSlow19 + fRec8[0] + -18.0f;
			int iTemp34 = fTemp20 < 17.0f;
			float fTemp35 = fSlow19 + fRec8[0] + -17.0f;
			int iTemp36 = fTemp20 < 16.0f;
			int iTemp37 = fTemp20 < 15.0f;
			float fTemp38 = fSlow19 + fRec8[0] + -15.0f;
			float fTemp39 = 2e+01f * fTemp38;
			int iTemp40 = fTemp20 < 14.0f;
			float fTemp41 = fSlow19 + fRec8[0] + -14.0f;
			int iTemp42 = fTemp20 < 13.0f;
			int iTemp43 = fTemp20 < 12.0f;
			int iTemp44 = fTemp20 < 11.0f;
			float fTemp45 = fSlow19 + fRec8[0] + -11.0f;
			int iTemp46 = fTemp20 < 1e+01f;
			float fTemp47 = fSlow19 + fRec8[0] + -1e+01f;
			float fTemp48 = 1e+01f * fTemp47;
			int iTemp49 = fTemp20 < 9.0f;
			float fTemp50 = fSlow19 + fRec8[0] + -9.0f;
			int iTemp51 = fTemp20 < 8.0f;
			int iTemp52 = fTemp20 < 7.0f;
			float fTemp53 = fSlow19 + fRec8[0] + -7.0f;
			int iTemp54 = fTemp20 < 6.0f;
			float fTemp55 = fSlow19 + fRec8[0] + -6.0f;
			int iTemp56 = fTemp20 < 5.0f;
			float fTemp57 = fSlow19 + fRec8[0] + -5.0f;
			int iTemp58 = fTemp20 < 4.0f;
			float fTemp59 = fSlow19 + fRec8[0] + -4.0f;
			float fTemp60 = 1e+01f * fTemp59;
			int iTemp61 = fTemp20 < 3.0f;
			float fTemp62 = fSlow19 + fRec8[0] + -3.0f;
			float fTemp63 = 2e+01f * fTemp62;
			int iTemp64 = fTemp20 < 2.0f;
			float fTemp65 = fSlow19 + fRec8[0] + -2.0f;
			float fTemp66 = 2e+01f * fTemp65;
			int iTemp67 = fTemp20 < 1.0f;
			float fTemp68 = fSlow19 + fRec8[0] + -1.0f;
			int iTemp69 = fTemp20 < 0.0f;
			float fTemp70 = 2e+01f * fTemp20;
			float fTemp71 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 8e+01f : ((iTemp67) ? 8e+01f - fTemp70 : 6e+01f)) : ((iTemp64) ? 6e+01f - 1e+01f * fTemp68 : 5e+01f)) : ((iTemp61) ? fTemp66 + 5e+01f : 7e+01f)) : ((iTemp58) ? 7e+01f - fTemp63 : 5e+01f)) : ((iTemp56) ? fTemp60 + 5e+01f : 6e+01f)) : ((iTemp54) ? 6e+01f - 2e+01f * fTemp57 : 4e+01f)) : ((iTemp52) ? 2e+01f * fTemp55 + 4e+01f : 6e+01f)) : ((iTemp51) ? 6e+01f - 2e+01f * fTemp53 : 4e+01f)) : 4e+01f) : ((iTemp46) ? 4e+01f * (fTemp50 + 1.0f) : 8e+01f)) : ((iTemp44) ? 8e+01f - fTemp48 : 7e+01f)) : ((iTemp43) ? 7e+01f - 3e+01f * fTemp45 : 4e+01f)) : 4e+01f) : 4e+01f) : ((iTemp37) ? 4e+01f * (fTemp41 + 1.0f) : 8e+01f)) : ((iTemp36) ? 8e+01f - fTemp39 : 6e+01f)) : 6e+01f) : ((iTemp32) ? 6e+01f - 2e+01f * fTemp35 : 4e+01f)) : ((iTemp31) ? 1e+01f * fTemp33 + 4e+01f : 5e+01f)) : 5e+01f) : ((iTemp26) ? fTemp30 + 5e+01f : 7e+01f)) : ((iTemp24) ? 7e+01f - 3e+01f * fTemp27 : 4e+01f)) : ((iTemp21) ? 3e+01f * fTemp25 + 4e+01f : 7e+01f)) : ((iTemp22) ? 7e+01f - 3e+01f * fTemp23 : 4e+01f));
			float fTemp72 = fTemp19 * fTemp71;
			fRec7[0] = ((iTemp8) ? fTemp72 : fRec7[1]);
			float fTemp73 = std::exp(-(fConst4 * fRec7[0]));
			fRec9[0] = ((iTemp8) ? fTemp71 : fRec9[1]);
			float fTemp74 = std::exp(-(fConst4 * fRec9[0]));
			fRec4[0] = fVec2[2] + fRec4[1] * (fTemp73 + fTemp74) - fTemp73 * fTemp74 * fRec4[2];
			int iTemp75 = iTemp0 | static_cast<int>(fVec2[1]);
			float fTemp76 = fSlow19 + fRec8[0] + -19.0f;
			float fTemp77 = fSlow19 + fRec8[0] + -16.0f;
			float fTemp78 = 8e+01f * fTemp77;
			float fTemp79 = fSlow19 + fRec8[0] + -13.0f;
			float fTemp80 = fSlow19 + fRec8[0] + -12.0f;
			float fTemp81 = fSlow19 + fRec8[0] + -8.0f;
			float fTemp82 = 5e+01f * fTemp81;
			float fTemp83 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 8e+02f : ((iTemp67) ? 8e+02f - 4e+02f * fTemp20 : 4e+02f)) : ((iTemp64) ? 4e+02f - 5e+01f * fTemp68 : 3.5e+02f)) : ((iTemp61) ? 1e+02f * fTemp65 + 3.5e+02f : 4.5e+02f)) : ((iTemp58) ? 4.5e+02f - 125.0f * fTemp62 : 325.0f)) : ((iTemp56) ? 275.0f * fTemp59 + 325.0f : 6e+02f)) : ((iTemp54) ? 6e+02f - 2e+02f * fTemp57 : 4e+02f)) : ((iTemp52) ? 4e+02f - 1.5e+02f * fTemp55 : 2.5e+02f)) : ((iTemp51) ? 1.5e+02f * fTemp53 + 2.5e+02f : 4e+02f)) : ((iTemp49) ? 4e+02f - fTemp82 : 3.5e+02f)) : ((iTemp46) ? 3.1e+02f * fTemp50 + 3.5e+02f : 6.6e+02f)) : ((iTemp44) ? 6.6e+02f - 2.2e+02f * fTemp47 : 4.4e+02f)) : ((iTemp43) ? 4.4e+02f - 1.7e+02f * fTemp45 : 2.7e+02f)) : ((iTemp42) ? 1.6e+02f * fTemp80 + 2.7e+02f : 4.3e+02f)) : ((iTemp40) ? 4.3e+02f - 6e+01f * fTemp79 : 3.7e+02f)) : ((iTemp37) ? 4.3e+02f * fTemp41 + 3.7e+02f : 8e+02f)) : ((iTemp36) ? 8e+02f - 4.5e+02f * fTemp38 : 3.5e+02f)) : ((iTemp34) ? 3.5e+02f - fTemp78 : 2.7e+02f)) : ((iTemp32) ? 1.8e+02f * fTemp35 + 2.7e+02f : 4.5e+02f)) : ((iTemp31) ? 4.5e+02f - 125.0f * fTemp33 : 325.0f)) : ((iTemp28) ? 325.0f * (fTemp76 + 1.0f) : 6.5e+02f)) : ((iTemp26) ? 6.5e+02f - 2.5e+02f * fTemp29 : 4e+02f)) : ((iTemp24) ? 4e+02f - 1.1e+02f * fTemp27 : 2.9e+02f)) : ((iTemp21) ? 1.1e+02f * fTemp25 + 2.9e+02f : 4e+02f)) : ((iTemp22) ? 4e+02f - 5e+01f * fTemp23 : 3.5e+02f));
			float fTemp84 = fConst3 * ((fTemp83 <= fTemp2) ? fTemp2 : fTemp83);
			float fTemp85 = ((iTemp75) ? 0.0f : fRec10[1] + fTemp84);
			fRec10[0] = fTemp85 - std::floor(fTemp85);
			float fTemp86 = static_cast<float>(fRec5[0] == 2.0f) * fTemp6;
			fVec3[0] = fTemp86;
			int iTemp87 = static_cast<int>(fTemp86);
			fRec12[0] = ((iTemp87) ? fTemp72 : fRec12[1]);
			float fTemp88 = std::exp(-(fConst4 * fRec12[0]));
			fRec13[0] = ((iTemp87) ? fTemp71 : fRec13[1]);
			float fTemp89 = std::exp(-(fConst4 * fRec13[0]));
			fRec11[0] = fVec3[2] + fRec11[1] * (fTemp88 + fTemp89) - fTemp88 * fTemp89 * fRec11[2];
			int iTemp90 = iTemp0 | static_cast<int>(fVec3[1]);
			float fTemp91 = ((iTemp90) ? 0.0f : fTemp84 + fRec14[1]);
			fRec14[0] = fTemp91 - std::floor(fTemp91);
			float fTemp92 = static_cast<float>(fRec5[0] == 3.0f) * fTemp6;
			fVec4[0] = fTemp92;
			int iTemp93 = static_cast<int>(fTemp92);
			fRec16[0] = ((iTemp93) ? fTemp72 : fRec16[1]);
			float fTemp94 = std::exp(-(fConst4 * fRec16[0]));
			fRec17[0] = ((iTemp93) ? fTemp71 : fRec17[1]);
			float fTemp95 = std::exp(-(fConst4 * fRec17[0]));
			fRec15[0] = fVec4[2] + fRec15[1] * (fTemp94 + fTemp95) - fTemp94 * fTemp95 * fRec15[2];
			int iTemp96 = iTemp0 | static_cast<int>(fVec4[1]);
			float fTemp97 = ((iTemp96) ? 0.0f : fTemp84 + fRec18[1]);
			fRec18[0] = fTemp97 - std::floor(fTemp97);
			float fTemp98 = static_cast<float>(fRec5[0] == 4.0f) * fTemp6;
			fVec5[0] = fTemp98;
			int iTemp99 = static_cast<int>(fTemp98);
			fRec20[0] = ((iTemp99) ? fTemp72 : fRec20[1]);
			float fTemp100 = std::exp(-(fConst4 * fRec20[0]));
			fRec21[0] = ((iTemp99) ? fTemp71 : fRec21[1]);
			float fTemp101 = std::exp(-(fConst4 * fRec21[0]));
			fRec19[0] = fVec5[2] + fRec19[1] * (fTemp100 + fTemp101) - fTemp100 * fTemp101 * fRec19[2];
			int iTemp102 = iTemp0 | static_cast<int>(fVec5[1]);
			float fTemp103 = ((iTemp102) ? 0.0f : fTemp84 + fRec22[1]);
			fRec22[0] = fTemp103 - std::floor(fTemp103);
			float fTemp104 = static_cast<float>(fRec5[0] == 5.0f) * fTemp6;
			fVec6[0] = fTemp104;
			int iTemp105 = static_cast<int>(fTemp104);
			fRec24[0] = ((iTemp105) ? fTemp72 : fRec24[1]);
			float fTemp106 = std::exp(-(fConst4 * fRec24[0]));
			fRec25[0] = ((iTemp105) ? fTemp71 : fRec25[1]);
			float fTemp107 = std::exp(-(fConst4 * fRec25[0]));
			fRec23[0] = fVec6[2] + fRec23[1] * (fTemp106 + fTemp107) - fTemp106 * fTemp107 * fRec23[2];
			int iTemp108 = iTemp0 | static_cast<int>(fVec6[1]);
			float fTemp109 = ((iTemp108) ? 0.0f : fTemp84 + fRec26[1]);
			fRec26[0] = fTemp109 - std::floor(fTemp109);
			float fTemp110 = static_cast<float>(fRec5[0] == 6.0f) * fTemp6;
			fVec7[0] = fTemp110;
			int iTemp111 = static_cast<int>(fTemp110);
			fRec28[0] = ((iTemp111) ? fTemp72 : fRec28[1]);
			float fTemp112 = std::exp(-(fConst4 * fRec28[0]));
			fRec29[0] = ((iTemp111) ? fTemp71 : fRec29[1]);
			float fTemp113 = std::exp(-(fConst4 * fRec29[0]));
			fRec27[0] = fVec7[2] + fRec27[1] * (fTemp112 + fTemp113) - fTemp112 * fTemp113 * fRec27[2];
			int iTemp114 = iTemp0 | static_cast<int>(fVec7[1]);
			float fTemp115 = ((iTemp114) ? 0.0f : fTemp84 + fRec30[1]);
			fRec30[0] = fTemp115 - std::floor(fTemp115);
			float fTemp116 = static_cast<float>(fRec5[0] == 7.0f) * fTemp6;
			fVec8[0] = fTemp116;
			int iTemp117 = static_cast<int>(fTemp116);
			fRec32[0] = ((iTemp117) ? fTemp72 : fRec32[1]);
			float fTemp118 = std::exp(-(fConst4 * fRec32[0]));
			fRec33[0] = ((iTemp117) ? fTemp71 : fRec33[1]);
			float fTemp119 = std::exp(-(fConst4 * fRec33[0]));
			fRec31[0] = fVec8[2] + fRec31[1] * (fTemp118 + fTemp119) - fTemp118 * fTemp119 * fRec31[2];
			int iTemp120 = iTemp0 | static_cast<int>(fVec8[1]);
			float fTemp121 = ((iTemp120) ? 0.0f : fTemp84 + fRec34[1]);
			fRec34[0] = fTemp121 - std::floor(fTemp121);
			float fTemp122 = static_cast<float>(fRec5[0] == 8.0f) * fTemp6;
			fVec9[0] = fTemp122;
			int iTemp123 = static_cast<int>(fTemp122);
			fRec36[0] = ((iTemp123) ? fTemp72 : fRec36[1]);
			float fTemp124 = std::exp(-(fConst4 * fRec36[0]));
			fRec37[0] = ((iTemp123) ? fTemp71 : fRec37[1]);
			float fTemp125 = std::exp(-(fConst4 * fRec37[0]));
			fRec35[0] = fVec9[2] + fRec35[1] * (fTemp124 + fTemp125) - fTemp124 * fTemp125 * fRec35[2];
			int iTemp126 = iTemp0 | static_cast<int>(fVec9[1]);
			float fTemp127 = ((iTemp126) ? 0.0f : fTemp84 + fRec38[1]);
			fRec38[0] = fTemp127 - std::floor(fTemp127);
			float fTemp128 = static_cast<float>(fRec5[0] == 9.0f) * fTemp6;
			fVec10[0] = fTemp128;
			int iTemp129 = static_cast<int>(fTemp128);
			fRec40[0] = ((iTemp129) ? fTemp72 : fRec40[1]);
			float fTemp130 = std::exp(-(fConst4 * fRec40[0]));
			fRec41[0] = ((iTemp129) ? fTemp71 : fRec41[1]);
			float fTemp131 = std::exp(-(fConst4 * fRec41[0]));
			fRec39[0] = fVec10[2] + fRec39[1] * (fTemp130 + fTemp131) - fTemp130 * fTemp131 * fRec39[2];
			int iTemp132 = iTemp0 | static_cast<int>(fVec10[1]);
			float fTemp133 = ((iTemp132) ? 0.0f : fTemp84 + fRec42[1]);
			fRec42[0] = fTemp133 - std::floor(fTemp133);
			float fTemp134 = static_cast<float>(fRec5[0] == 1e+01f) * fTemp6;
			fVec11[0] = fTemp134;
			int iTemp135 = static_cast<int>(fTemp134);
			fRec44[0] = ((iTemp135) ? fTemp72 : fRec44[1]);
			float fTemp136 = std::exp(-(fConst4 * fRec44[0]));
			fRec45[0] = ((iTemp135) ? fTemp71 : fRec45[1]);
			float fTemp137 = std::exp(-(fConst4 * fRec45[0]));
			fRec43[0] = fVec11[2] + fRec43[1] * (fTemp136 + fTemp137) - fTemp136 * fTemp137 * fRec43[2];
			int iTemp138 = iTemp0 | static_cast<int>(fVec11[1]);
			float fTemp139 = ((iTemp138) ? 0.0f : fTemp84 + fRec46[1]);
			fRec46[0] = fTemp139 - std::floor(fTemp139);
			float fTemp140 = static_cast<float>(fRec5[0] == 11.0f) * fTemp6;
			fVec12[0] = fTemp140;
			int iTemp141 = static_cast<int>(fTemp140);
			fRec48[0] = ((iTemp141) ? fTemp72 : fRec48[1]);
			float fTemp142 = std::exp(-(fConst4 * fRec48[0]));
			fRec49[0] = ((iTemp141) ? fTemp71 : fRec49[1]);
			float fTemp143 = std::exp(-(fConst4 * fRec49[0]));
			fRec47[0] = fVec12[2] + fRec47[1] * (fTemp142 + fTemp143) - fTemp142 * fTemp143 * fRec47[2];
			int iTemp144 = iTemp0 | static_cast<int>(fVec12[1]);
			float fTemp145 = ((iTemp144) ? 0.0f : fTemp84 + fRec50[1]);
			fRec50[0] = fTemp145 - std::floor(fTemp145);
			float fTemp146 = static_cast<float>(fRec5[0] == 12.0f) * fTemp6;
			fVec13[0] = fTemp146;
			int iTemp147 = static_cast<int>(fTemp146);
			fRec52[0] = ((iTemp147) ? fTemp72 : fRec52[1]);
			float fTemp148 = std::exp(-(fConst4 * fRec52[0]));
			fRec53[0] = ((iTemp147) ? fTemp71 : fRec53[1]);
			float fTemp149 = std::exp(-(fConst4 * fRec53[0]));
			fRec51[0] = fVec13[2] + fRec51[1] * (fTemp148 + fTemp149) - fTemp148 * fTemp149 * fRec51[2];
			int iTemp150 = iTemp0 | static_cast<int>(fVec13[1]);
			float fTemp151 = ((iTemp150) ? 0.0f : fTemp84 + fRec54[1]);
			fRec54[0] = fTemp151 - std::floor(fTemp151);
			float fTemp152 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 0.630957f : ((iTemp67) ? 0.630957f - 0.567861f * fTemp20 : 0.063096f)) : ((iTemp64) ? 0.036904f * fTemp68 + 0.063096f : 0.1f)) : ((iTemp61) ? 0.254813f * fTemp65 + 0.1f : 0.354813f)) : ((iTemp58) ? 0.354813f - 0.103624f * fTemp62 : 0.251189f)) : ((iTemp56) ? 0.195495f * fTemp59 + 0.251189f : 0.446684f)) : ((iTemp54) ? 0.446684f - 0.195495f * fTemp57 : 0.251189f)) : ((iTemp52) ? 0.251189f - 0.219566f * fTemp55 : 0.031623f)) : ((iTemp51) ? 0.250215f * fTemp53 + 0.031623f : 0.281838f)) : ((iTemp49) ? 0.281838f - 0.181838f * fTemp81 : 0.1f)) : ((iTemp46) ? 0.401187f * fTemp50 + 0.1f : 0.501187f)) : ((iTemp44) ? 0.501187f - 0.301661f * fTemp47 : 0.199526f)) : ((iTemp43) ? 0.199526f - 0.13643f * fTemp45 : 0.063096f)) : ((iTemp42) ? 0.253132f * fTemp80 + 0.063096f : 0.316228f)) : ((iTemp40) ? 0.316228f - 0.216228f * fTemp79 : 0.1f)) : ((iTemp37) ? 0.401187f * fTemp41 + 0.1f : 0.501187f)) : ((iTemp36) ? 0.501187f - 0.401187f * fTemp38 : 0.1f)) : ((iTemp34) ? 0.151189f * fTemp77 + 0.1f : 0.251189f)) : ((iTemp32) ? 0.030649f * fTemp35 + 0.251189f : 0.281838f)) : ((iTemp31) ? 0.281838f - 0.123349f * fTemp33 : 0.158489f)) : ((iTemp28) ? 0.342698f * fTemp76 + 0.158489f : 0.501187f)) : ((iTemp26) ? 0.501187f - 0.301661f * fTemp29 : 0.199526f)) : ((iTemp24) ? 0.199526f - 0.021698f * fTemp27 : 0.177828f)) : ((iTemp21) ? 0.1384f * fTemp25 + 0.177828f : 0.316228f)) : ((iTemp22) ? 0.316228f - 0.216228f * fTemp23 : 0.1f));
			float fTemp153 = 1e+01f * fTemp25;
			float fTemp154 = 1e+01f * fTemp29;
			float fTemp155 = 1e+01f * fTemp50;
			float fTemp156 = 1e+01f * fTemp57;
			float fTemp157 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 9e+01f : ((iTemp67) ? 9e+01f - 1e+01f * fTemp20 : 8e+01f)) : ((iTemp64) ? 2e+01f * fTemp68 + 8e+01f : 1e+02f)) : ((iTemp61) ? 1e+02f - fTemp66 : 8e+01f)) : ((iTemp58) ? 8e+01f - fTemp63 : 6e+01f)) : ((iTemp56) ? fTemp60 + 6e+01f : 7e+01f)) : ((iTemp54) ? fTemp156 + 7e+01f : 8e+01f)) : ((iTemp52) ? 1e+01f * fTemp55 + 8e+01f : 9e+01f)) : ((iTemp51) ? 9e+01f - 1e+01f * fTemp53 : 8e+01f)) : 8e+01f) : ((iTemp46) ? fTemp155 + 8e+01f : 9e+01f)) : ((iTemp44) ? 9e+01f - fTemp48 : 8e+01f)) : ((iTemp43) ? 1e+01f * fTemp45 + 8e+01f : 9e+01f)) : ((iTemp42) ? 9e+01f - 1e+01f * fTemp80 : 8e+01f)) : ((iTemp40) ? 8e+01f - 2e+01f * fTemp79 : 6e+01f)) : ((iTemp37) ? 3e+01f * fTemp41 + 6e+01f : 9e+01f)) : ((iTemp36) ? 1e+01f * fTemp38 + 9e+01f : 1e+02f)) : ((iTemp34) ? 1e+02f - 1e+01f * fTemp77 : 9e+01f)) : ((iTemp32) ? 9e+01f - 1e+01f * fTemp35 : 8e+01f)) : ((iTemp31) ? 8e+01f - 2e+01f * fTemp33 : 6e+01f)) : ((iTemp28) ? 3e+01f * fTemp76 + 6e+01f : 9e+01f)) : ((iTemp26) ? 9e+01f - fTemp154 : 8e+01f)) : ((iTemp24) ? 1e+01f * fTemp27 + 8e+01f : 9e+01f)) : ((iTemp21) ? 9e+01f - fTemp153 : 8e+01f)) : ((iTemp22) ? 8e+01f - 2e+01f * fTemp23 : 6e+01f));
			float fTemp158 = fTemp19 * fTemp157;
			fRec56[0] = ((iTemp8) ? fTemp158 : fRec56[1]);
			float fTemp159 = std::exp(-(fConst4 * fRec56[0]));
			fRec57[0] = ((iTemp8) ? fTemp157 : fRec57[1]);
			float fTemp160 = std::exp(-(fConst4 * fRec57[0]));
			fRec55[0] = fVec2[2] + fRec55[1] * (fTemp159 + fTemp160) - fTemp159 * fTemp160 * fRec55[2];
			float fTemp161 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 1.15e+03f : ((iTemp67) ? 4.5e+02f * fTemp20 + 1.15e+03f : 1.6e+03f)) : ((iTemp64) ? 1e+02f * fTemp68 + 1.6e+03f : 1.7e+03f)) : ((iTemp61) ? 1.7e+03f - 9e+02f * fTemp65 : 8e+02f)) : ((iTemp58) ? 8e+02f - 1e+02f * fTemp62 : 7e+02f)) : ((iTemp56) ? 3.4e+02f * fTemp59 + 7e+02f : 1.04e+03f)) : ((iTemp54) ? 5.8e+02f * fTemp57 + 1.04e+03f : 1.62e+03f)) : ((iTemp52) ? 1.3e+02f * fTemp55 + 1.62e+03f : 1.75e+03f)) : ((iTemp51) ? 1.75e+03f - 1e+03f * fTemp53 : 7.5e+02f)) : ((iTemp49) ? 7.5e+02f - 1.5e+02f * fTemp81 : 6e+02f)) : ((iTemp46) ? 5.2e+02f * fTemp50 + 6e+02f : 1.12e+03f)) : ((iTemp44) ? 6.8e+02f * fTemp47 + 1.12e+03f : 1.8e+03f)) : ((iTemp43) ? 5e+01f * fTemp45 + 1.8e+03f : 1.85e+03f)) : ((iTemp42) ? 1.85e+03f - 1.03e+03f * fTemp80 : 8.2e+02f)) : ((iTemp40) ? 8.2e+02f - 1.9e+02f * fTemp79 : 6.3e+02f)) : ((iTemp37) ? 5.2e+02f * fTemp41 + 6.3e+02f : 1.15e+03f)) : ((iTemp36) ? 8.5e+02f * fTemp38 + 1.15e+03f : 2e+03f)) : ((iTemp34) ? 1.4e+02f * fTemp77 + 2e+03f : 2.14e+03f)) : ((iTemp32) ? 2.14e+03f - 1.34e+03f * fTemp35 : 8e+02f)) : ((iTemp31) ? 8e+02f - 1e+02f * fTemp33 : 7e+02f)) : ((iTemp28) ? 3.8e+02f * fTemp76 + 7e+02f : 1.08e+03f)) : ((iTemp26) ? 6.2e+02f * fTemp29 + 1.08e+03f : 1.7e+03f)) : ((iTemp24) ? 1.7e+02f * fTemp27 + 1.7e+03f : 1.87e+03f)) : ((iTemp21) ? 1.87e+03f - 1.07e+03f * fTemp25 : 8e+02f)) : ((iTemp22) ? 8e+02f - 2e+02f * fTemp23 : 6e+02f));
			float fTemp162 = 2.0f * fTemp2 + 3e+01f;
			float fTemp163 = fConst3 * ((iSlow20) ? (((fTemp161 >= 1.3e+03f) & (fTemp2 >= 2e+02f)) ? fTemp161 - 0.00095238094f * (fTemp2 + -2e+02f) * (fTemp161 + -1.3e+03f) : ((fTemp161 <= fTemp162) ? fTemp162 : fTemp161)) : fTemp161);
			float fTemp164 = ((iTemp75) ? 0.0f : fRec58[1] + fTemp163);
			fRec58[0] = fTemp164 - std::floor(fTemp164);
			fRec60[0] = ((iTemp87) ? fTemp158 : fRec60[1]);
			float fTemp165 = std::exp(-(fConst4 * fRec60[0]));
			fRec61[0] = ((iTemp87) ? fTemp157 : fRec61[1]);
			float fTemp166 = std::exp(-(fConst4 * fRec61[0]));
			fRec59[0] = fVec3[2] + fRec59[1] * (fTemp165 + fTemp166) - fTemp165 * fTemp166 * fRec59[2];
			float fTemp167 = ((iTemp90) ? 0.0f : fTemp163 + fRec62[1]);
			fRec62[0] = fTemp167 - std::floor(fTemp167);
			fRec64[0] = ((iTemp93) ? fTemp158 : fRec64[1]);
			float fTemp168 = std::exp(-(fConst4 * fRec64[0]));
			fRec65[0] = ((iTemp93) ? fTemp157 : fRec65[1]);
			float fTemp169 = std::exp(-(fConst4 * fRec65[0]));
			fRec63[0] = fVec4[2] + fRec63[1] * (fTemp168 + fTemp169) - fTemp168 * fTemp169 * fRec63[2];
			float fTemp170 = ((iTemp96) ? 0.0f : fTemp163 + fRec66[1]);
			fRec66[0] = fTemp170 - std::floor(fTemp170);
			fRec68[0] = ((iTemp99) ? fTemp158 : fRec68[1]);
			float fTemp171 = std::exp(-(fConst4 * fRec68[0]));
			fRec69[0] = ((iTemp99) ? fTemp157 : fRec69[1]);
			float fTemp172 = std::exp(-(fConst4 * fRec69[0]));
			fRec67[0] = fVec5[2] + fRec67[1] * (fTemp171 + fTemp172) - fTemp171 * fTemp172 * fRec67[2];
			float fTemp173 = ((iTemp102) ? 0.0f : fTemp163 + fRec70[1]);
			fRec70[0] = fTemp173 - std::floor(fTemp173);
			fRec72[0] = ((iTemp105) ? fTemp158 : fRec72[1]);
			float fTemp174 = std::exp(-(fConst4 * fRec72[0]));
			fRec73[0] = ((iTemp105) ? fTemp157 : fRec73[1]);
			float fTemp175 = std::exp(-(fConst4 * fRec73[0]));
			fRec71[0] = fVec6[2] + fRec71[1] * (fTemp174 + fTemp175) - fTemp174 * fTemp175 * fRec71[2];
			float fTemp176 = ((iTemp108) ? 0.0f : fTemp163 + fRec74[1]);
			fRec74[0] = fTemp176 - std::floor(fTemp176);
			fRec76[0] = ((iTemp111) ? fTemp158 : fRec76[1]);
			float fTemp177 = std::exp(-(fConst4 * fRec76[0]));
			fRec77[0] = ((iTemp111) ? fTemp157 : fRec77[1]);
			float fTemp178 = std::exp(-(fConst4 * fRec77[0]));
			fRec75[0] = fVec7[2] + fRec75[1] * (fTemp177 + fTemp178) - fTemp177 * fTemp178 * fRec75[2];
			float fTemp179 = ((iTemp114) ? 0.0f : fTemp163 + fRec78[1]);
			fRec78[0] = fTemp179 - std::floor(fTemp179);
			fRec80[0] = ((iTemp117) ? fTemp158 : fRec80[1]);
			float fTemp180 = std::exp(-(fConst4 * fRec80[0]));
			fRec81[0] = ((iTemp117) ? fTemp157 : fRec81[1]);
			float fTemp181 = std::exp(-(fConst4 * fRec81[0]));
			fRec79[0] = fVec8[2] + fRec79[1] * (fTemp180 + fTemp181) - fTemp180 * fTemp181 * fRec79[2];
			float fTemp182 = ((iTemp120) ? 0.0f : fTemp163 + fRec82[1]);
			fRec82[0] = fTemp182 - std::floor(fTemp182);
			fRec84[0] = ((iTemp123) ? fTemp158 : fRec84[1]);
			float fTemp183 = std::exp(-(fConst4 * fRec84[0]));
			fRec85[0] = ((iTemp123) ? fTemp157 : fRec85[1]);
			float fTemp184 = std::exp(-(fConst4 * fRec85[0]));
			fRec83[0] = fVec9[2] + fRec83[1] * (fTemp183 + fTemp184) - fTemp183 * fTemp184 * fRec83[2];
			float fTemp185 = ((iTemp126) ? 0.0f : fTemp163 + fRec86[1]);
			fRec86[0] = fTemp185 - std::floor(fTemp185);
			fRec88[0] = ((iTemp129) ? fTemp158 : fRec88[1]);
			float fTemp186 = std::exp(-(fConst4 * fRec88[0]));
			fRec89[0] = ((iTemp129) ? fTemp157 : fRec89[1]);
			float fTemp187 = std::exp(-(fConst4 * fRec89[0]));
			fRec87[0] = fVec10[2] + fRec87[1] * (fTemp186 + fTemp187) - fTemp186 * fTemp187 * fRec87[2];
			float fTemp188 = ((iTemp132) ? 0.0f : fTemp163 + fRec90[1]);
			fRec90[0] = fTemp188 - std::floor(fTemp188);
			fRec92[0] = ((iTemp135) ? fTemp158 : fRec92[1]);
			float fTemp189 = std::exp(-(fConst4 * fRec92[0]));
			fRec93[0] = ((iTemp135) ? fTemp157 : fRec93[1]);
			float fTemp190 = std::exp(-(fConst4 * fRec93[0]));
			fRec91[0] = fVec11[2] + fRec91[1] * (fTemp189 + fTemp190) - fTemp189 * fTemp190 * fRec91[2];
			float fTemp191 = ((iTemp138) ? 0.0f : fTemp163 + fRec94[1]);
			fRec94[0] = fTemp191 - std::floor(fTemp191);
			fRec96[0] = ((iTemp141) ? fTemp158 : fRec96[1]);
			float fTemp192 = std::exp(-(fConst4 * fRec96[0]));
			fRec97[0] = ((iTemp141) ? fTemp157 : fRec97[1]);
			float fTemp193 = std::exp(-(fConst4 * fRec97[0]));
			fRec95[0] = fVec12[2] + fRec95[1] * (fTemp192 + fTemp193) - fTemp192 * fTemp193 * fRec95[2];
			float fTemp194 = ((iTemp144) ? 0.0f : fTemp163 + fRec98[1]);
			fRec98[0] = fTemp194 - std::floor(fTemp194);
			fRec100[0] = ((iTemp147) ? fTemp158 : fRec100[1]);
			float fTemp195 = std::exp(-(fConst4 * fRec100[0]));
			fRec101[0] = ((iTemp147) ? fTemp157 : fRec101[1]);
			float fTemp196 = std::exp(-(fConst4 * fRec101[0]));
			fRec99[0] = fVec13[2] + fRec99[1] * (fTemp195 + fTemp196) - fTemp195 * fTemp196 * fRec99[2];
			float fTemp197 = ((iTemp150) ? 0.0f : fTemp163 + fRec102[1]);
			fRec102[0] = fTemp197 - std::floor(fTemp197);
			float fTemp198 = ((iTemp32) ? 0.029314f * fTemp35 + 0.050119f : 0.079433f);
			float fTemp199 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 0.1f : ((iTemp67) ? 0.1f - 0.068377f * fTemp20 : 0.031623f)) : 0.031623f) : ((iTemp61) ? 0.126866f * fTemp65 + 0.031623f : 0.158489f)) : ((iTemp58) ? 0.158489f - 0.126866f * fTemp62 : 0.031623f)) : ((iTemp56) ? 0.32319f * fTemp59 + 0.031623f : 0.354813f)) : 0.354813f) : ((iTemp52) ? 0.354813f - 0.196324f * fTemp55 : 0.158489f)) : ((iTemp51) ? 0.158489f - 0.069364f * fTemp53 : 0.089125f)) : ((iTemp49) ? 0.089125f - 0.064006f * fTemp81 : 0.025119f)) : ((iTemp46) ? 0.045676f * fTemp50 + 0.025119f : 0.070795f)) : ((iTemp44) ? 0.055098f * fTemp47 + 0.070795f : 0.125893f)) : ((iTemp43) ? 0.125893f - 0.062797f * fTemp45 : 0.063096f)) : ((iTemp42) ? 0.063096f - 0.012977f * fTemp80 : 0.050119f)) : ((iTemp40) ? 0.020676f * fTemp79 + 0.050119f : 0.070795f)) : ((iTemp37) ? 0.070795f - 0.045676f * fTemp41 : 0.025119f)) : ((iTemp36) ? 0.152709f * fTemp38 + 0.025119f : 0.177828f)) : ((iTemp34) ? 0.177828f - 0.127709f * fTemp77 : 0.050119f)) : fTemp198) : ((iTemp31) ? 0.079433f - 0.06165f * fTemp33 : 0.017783f)) : ((iTemp28) ? 0.428901f * fTemp76 + 0.017783f : 0.446684f)) : ((iTemp26) ? 0.446684f - 0.195495f * fTemp29 : 0.251189f)) : ((iTemp24) ? 0.251189f - 0.125296f * fTemp27 : 0.125893f)) : ((iTemp21) ? 0.125296f * fTemp25 + 0.125893f : 0.251189f)) : ((iTemp22) ? 0.251189f - 0.109935f * fTemp23 : 0.141254f));
			float fTemp200 = 5e+01f * fTemp76;
			float fTemp201 = 2e+01f * fTemp41;
			float fTemp202 = 2e+01f * fTemp47;
			float fTemp203 = 2e+01f * fTemp50;
			float fTemp204 = 6e+01f * fTemp59;
			float fTemp205 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? 1.2e+02f : ((iTemp61) ? 1.2e+02f - fTemp66 : 1e+02f)) : ((iTemp58) ? 7e+01f * fTemp62 + 1e+02f : 1.7e+02f)) : ((iTemp56) ? 1.7e+02f - fTemp204 : 1.1e+02f)) : ((iTemp54) ? 1.1e+02f - fTemp156 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp46) ? fTemp203 + 1e+02f : 1.2e+02f)) : ((iTemp44) ? 1.2e+02f - fTemp202 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp37) ? fTemp201 + 1e+02f : 1.2e+02f)) : 1.2e+02f) : ((iTemp34) ? 1.2e+02f - 2e+01f * fTemp77 : 1e+02f)) : 1e+02f) : ((iTemp31) ? 7e+01f * fTemp33 + 1e+02f : 1.7e+02f)) : ((iTemp28) ? 1.7e+02f - fTemp200 : 1.2e+02f)) : ((iTemp26) ? 1.2e+02f - fTemp30 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f);
			float fTemp206 = fTemp19 * fTemp205;
			fRec104[0] = ((iTemp8) ? fTemp206 : fRec104[1]);
			float fTemp207 = std::exp(-(fConst4 * fRec104[0]));
			fRec105[0] = ((iTemp8) ? fTemp205 : fRec105[1]);
			float fTemp208 = std::exp(-(fConst4 * fRec105[0]));
			fRec103[0] = fVec2[2] + fRec103[1] * (fTemp207 + fTemp208) - fTemp207 * fTemp208 * fRec103[2];
			float fTemp209 = 1e+02f * fTemp23;
			float fTemp210 = 5e+01f * fTemp47;
			float fTemp211 = fConst3 * ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 2.8e+03f : ((iTemp67) ? 2.8e+03f - 1e+02f * fTemp20 : 2.7e+03f)) : 2.7e+03f) : ((iTemp61) ? 1.3e+02f * fTemp65 + 2.7e+03f : 2.83e+03f)) : ((iTemp58) ? 2.83e+03f - 3e+02f * fTemp62 : 2.53e+03f)) : ((iTemp56) ? 2.53e+03f - 2.8e+02f * fTemp59 : 2.25e+03f)) : ((iTemp54) ? 1.5e+02f * fTemp57 + 2.25e+03f : 2.4e+03f)) : ((iTemp52) ? 2e+02f * fTemp55 + 2.4e+03f : 2.6e+03f)) : ((iTemp51) ? 2.6e+03f - 2e+02f * fTemp53 : 2.4e+03f)) : 2.4e+03f) : ((iTemp46) ? 3.5e+02f * fTemp50 + 2.4e+03f : 2.75e+03f)) : ((iTemp44) ? 2.75e+03f - fTemp210 : 2.7e+03f)) : ((iTemp43) ? 2e+02f * fTemp45 + 2.7e+03f : 2.9e+03f)) : ((iTemp42) ? 2.9e+03f - 2e+02f * fTemp80 : 2.7e+03f)) : ((iTemp40) ? 5e+01f * fTemp79 + 2.7e+03f : 2.75e+03f)) : ((iTemp37) ? 1.5e+02f * fTemp41 + 2.75e+03f : 2.9e+03f)) : ((iTemp36) ? 2.9e+03f - 1e+02f * fTemp38 : 2.8e+03f)) : ((iTemp34) ? 1.5e+02f * fTemp77 + 2.8e+03f : 2.95e+03f)) : ((iTemp32) ? 2.95e+03f - 1.2e+02f * fTemp35 : 2.83e+03f)) : ((iTemp31) ? 2.83e+03f - 1.3e+02f * fTemp33 : 2.7e+03f)) : ((iTemp28) ? 2.7e+03f - fTemp200 : 2.65e+03f)) : ((iTemp26) ? 2.65e+03f - 5e+01f * fTemp29 : 2.6e+03f)) : ((iTemp24) ? 2e+02f * fTemp27 + 2.6e+03f : 2.8e+03f)) : ((iTemp21) ? 2.8e+03f - 2e+02f * fTemp25 : 2.6e+03f)) : ((iTemp22) ? fTemp209 + 2.6e+03f : 2.7e+03f));
			float fTemp212 = ((iTemp75) ? 0.0f : fRec106[1] + fTemp211);
			fRec106[0] = fTemp212 - std::floor(fTemp212);
			fRec108[0] = ((iTemp87) ? fTemp206 : fRec108[1]);
			float fTemp213 = std::exp(-(fConst4 * fRec108[0]));
			fRec109[0] = ((iTemp87) ? fTemp205 : fRec109[1]);
			float fTemp214 = std::exp(-(fConst4 * fRec109[0]));
			fRec107[0] = fVec3[2] + fRec107[1] * (fTemp213 + fTemp214) - fTemp213 * fTemp214 * fRec107[2];
			float fTemp215 = ((iTemp90) ? 0.0f : fTemp211 + fRec110[1]);
			fRec110[0] = fTemp215 - std::floor(fTemp215);
			fRec112[0] = ((iTemp93) ? fTemp206 : fRec112[1]);
			float fTemp216 = std::exp(-(fConst4 * fRec112[0]));
			fRec113[0] = ((iTemp93) ? fTemp205 : fRec113[1]);
			float fTemp217 = std::exp(-(fConst4 * fRec113[0]));
			fRec111[0] = fVec4[2] + fRec111[1] * (fTemp216 + fTemp217) - fTemp216 * fTemp217 * fRec111[2];
			float fTemp218 = ((iTemp96) ? 0.0f : fTemp211 + fRec114[1]);
			fRec114[0] = fTemp218 - std::floor(fTemp218);
			fRec116[0] = ((iTemp99) ? fTemp206 : fRec116[1]);
			float fTemp219 = std::exp(-(fConst4 * fRec116[0]));
			fRec117[0] = ((iTemp99) ? fTemp205 : fRec117[1]);
			float fTemp220 = std::exp(-(fConst4 * fRec117[0]));
			fRec115[0] = fVec5[2] + fRec115[1] * (fTemp219 + fTemp220) - fTemp219 * fTemp220 * fRec115[2];
			float fTemp221 = ((iTemp102) ? 0.0f : fTemp211 + fRec118[1]);
			fRec118[0] = fTemp221 - std::floor(fTemp221);
			fRec120[0] = ((iTemp105) ? fTemp206 : fRec120[1]);
			float fTemp222 = std::exp(-(fConst4 * fRec120[0]));
			fRec121[0] = ((iTemp105) ? fTemp205 : fRec121[1]);
			float fTemp223 = std::exp(-(fConst4 * fRec121[0]));
			fRec119[0] = fVec6[2] + fRec119[1] * (fTemp222 + fTemp223) - fTemp222 * fTemp223 * fRec119[2];
			float fTemp224 = ((iTemp108) ? 0.0f : fTemp211 + fRec122[1]);
			fRec122[0] = fTemp224 - std::floor(fTemp224);
			fRec124[0] = ((iTemp111) ? fTemp206 : fRec124[1]);
			float fTemp225 = std::exp(-(fConst4 * fRec124[0]));
			fRec125[0] = ((iTemp111) ? fTemp205 : fRec125[1]);
			float fTemp226 = std::exp(-(fConst4 * fRec125[0]));
			fRec123[0] = fVec7[2] + fRec123[1] * (fTemp225 + fTemp226) - fTemp225 * fTemp226 * fRec123[2];
			float fTemp227 = ((iTemp114) ? 0.0f : fTemp211 + fRec126[1]);
			fRec126[0] = fTemp227 - std::floor(fTemp227);
			fRec128[0] = ((iTemp117) ? fTemp206 : fRec128[1]);
			float fTemp228 = std::exp(-(fConst4 * fRec128[0]));
			fRec129[0] = ((iTemp117) ? fTemp205 : fRec129[1]);
			float fTemp229 = std::exp(-(fConst4 * fRec129[0]));
			fRec127[0] = fVec8[2] + fRec127[1] * (fTemp228 + fTemp229) - fTemp228 * fTemp229 * fRec127[2];
			float fTemp230 = ((iTemp120) ? 0.0f : fTemp211 + fRec130[1]);
			fRec130[0] = fTemp230 - std::floor(fTemp230);
			fRec132[0] = ((iTemp123) ? fTemp206 : fRec132[1]);
			float fTemp231 = std::exp(-(fConst4 * fRec132[0]));
			fRec133[0] = ((iTemp123) ? fTemp205 : fRec133[1]);
			float fTemp232 = std::exp(-(fConst4 * fRec133[0]));
			fRec131[0] = fVec9[2] + fRec131[1] * (fTemp231 + fTemp232) - fTemp231 * fTemp232 * fRec131[2];
			float fTemp233 = ((iTemp126) ? 0.0f : fTemp211 + fRec134[1]);
			fRec134[0] = fTemp233 - std::floor(fTemp233);
			fRec136[0] = ((iTemp129) ? fTemp206 : fRec136[1]);
			float fTemp234 = std::exp(-(fConst4 * fRec136[0]));
			fRec137[0] = ((iTemp129) ? fTemp205 : fRec137[1]);
			float fTemp235 = std::exp(-(fConst4 * fRec137[0]));
			fRec135[0] = fVec10[2] + fRec135[1] * (fTemp234 + fTemp235) - fTemp234 * fTemp235 * fRec135[2];
			float fTemp236 = ((iTemp132) ? 0.0f : fTemp211 + fRec138[1]);
			fRec138[0] = fTemp236 - std::floor(fTemp236);
			fRec140[0] = ((iTemp135) ? fTemp206 : fRec140[1]);
			float fTemp237 = std::exp(-(fConst4 * fRec140[0]));
			fRec141[0] = ((iTemp135) ? fTemp205 : fRec141[1]);
			float fTemp238 = std::exp(-(fConst4 * fRec141[0]));
			fRec139[0] = fVec11[2] + fRec139[1] * (fTemp237 + fTemp238) - fTemp237 * fTemp238 * fRec139[2];
			float fTemp239 = ((iTemp138) ? 0.0f : fTemp211 + fRec142[1]);
			fRec142[0] = fTemp239 - std::floor(fTemp239);
			fRec144[0] = ((iTemp141) ? fTemp206 : fRec144[1]);
			float fTemp240 = std::exp(-(fConst4 * fRec144[0]));
			fRec145[0] = ((iTemp141) ? fTemp205 : fRec145[1]);
			float fTemp241 = std::exp(-(fConst4 * fRec145[0]));
			fRec143[0] = fVec12[2] + fRec143[1] * (fTemp240 + fTemp241) - fTemp240 * fTemp241 * fRec143[2];
			float fTemp242 = ((iTemp144) ? 0.0f : fTemp211 + fRec146[1]);
			fRec146[0] = fTemp242 - std::floor(fTemp242);
			fRec148[0] = ((iTemp147) ? fTemp206 : fRec148[1]);
			float fTemp243 = std::exp(-(fConst4 * fRec148[0]));
			fRec149[0] = ((iTemp147) ? fTemp205 : fRec149[1]);
			float fTemp244 = std::exp(-(fConst4 * fRec149[0]));
			fRec147[0] = fVec13[2] + fRec147[1] * (fTemp243 + fTemp244) - fTemp243 * fTemp244 * fRec147[2];
			float fTemp245 = ((iTemp150) ? 0.0f : fTemp211 + fRec150[1]);
			fRec150[0] = fTemp245 - std::floor(fTemp245);
			float fTemp246 = ((iTemp43) ? 0.1f - 0.084151f * fTemp45 : 0.015849f);
			float fTemp247 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 0.015849f : ((iTemp67) ? 0.001934f * fTemp20 + 0.015849f : 0.017783f)) : ((iTemp64) ? 0.017783f - 0.001934f * fTemp68 : 0.015849f)) : ((iTemp61) ? 0.023962f * fTemp65 + 0.015849f : 0.039811f)) : ((iTemp58) ? 0.039811f - 0.029811f * fTemp62 : 0.01f)) : ((iTemp56) ? 0.344813f * fTemp59 + 0.01f : 0.354813f)) : ((iTemp54) ? 0.354813f - 0.103624f * fTemp57 : 0.251189f)) : ((iTemp52) ? 0.251189f - 0.171756f * fTemp55 : 0.079433f)) : ((iTemp51) ? 0.020567f * fTemp53 + 0.079433f : 0.1f)) : ((iTemp49) ? 0.1f - 0.060189f * fTemp81 : 0.039811f)) : ((iTemp46) ? 0.023285f * fTemp50 + 0.039811f : 0.063096f)) : ((iTemp44) ? 0.036904f * fTemp47 + 0.063096f : 0.1f)) : fTemp246) : ((iTemp42) ? 0.063584f * fTemp80 + 0.015849f : 0.079433f)) : ((iTemp40) ? 0.079433f - 0.04781f * fTemp79 : 0.031623f)) : ((iTemp37) ? 0.068377f * fTemp41 + 0.031623f : 0.1f)) : ((iTemp36) ? 0.1f - 0.09f * fTemp38 : 0.01f)) : ((iTemp34) ? 0.040119f * fTemp77 + 0.01f : 0.050119f)) : fTemp198) : ((iTemp31) ? 0.079433f - 0.069433f * fTemp33 : 0.01f)) : ((iTemp28) ? 0.388107f * fTemp76 + 0.01f : 0.398107f)) : ((iTemp26) ? 0.398107f - 0.198581f * fTemp29 : 0.199526f)) : ((iTemp24) ? 0.199526f - 0.099526f * fTemp27 : 0.1f)) : ((iTemp21) ? 0.151189f * fTemp25 + 0.1f : 0.251189f)) : ((iTemp22) ? 0.251189f - 0.051663f * fTemp23 : 0.199526f));
			float fTemp248 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 1.3e+02f : ((iTemp67) ? fTemp70 + 1.3e+02f : 1.5e+02f)) : 1.5e+02f) : ((iTemp61) ? 1.5e+02f - fTemp66 : 1.3e+02f)) : ((iTemp58) ? 5e+01f * fTemp62 + 1.3e+02f : 1.8e+02f)) : ((iTemp56) ? 1.8e+02f - fTemp204 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp46) ? fTemp155 + 1.2e+02f : 1.3e+02f)) : ((iTemp44) ? 1.3e+02f - fTemp48 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp37) ? 1e+01f * fTemp41 + 1.2e+02f : 1.3e+02f)) : ((iTemp36) ? fTemp39 + 1.3e+02f : 1.5e+02f)) : ((iTemp34) ? 1.5e+02f - 3e+01f * fTemp77 : 1.2e+02f)) : 1.2e+02f) : ((iTemp31) ? 6e+01f * fTemp33 + 1.2e+02f : 1.8e+02f)) : ((iTemp28) ? 1.8e+02f - fTemp200 : 1.3e+02f)) : ((iTemp26) ? 1.3e+02f - fTemp154 : 1.2e+02f)) : 1.2e+02f) : ((iTemp21) ? fTemp153 + 1.2e+02f : 1.3e+02f)) : ((iTemp22) ? 1.3e+02f - 1e+01f * fTemp23 : 1.2e+02f));
			float fTemp249 = fTemp19 * fTemp248;
			fRec152[0] = ((iTemp8) ? fTemp249 : fRec152[1]);
			float fTemp250 = std::exp(-(fConst4 * fRec152[0]));
			fRec153[0] = ((iTemp8) ? fTemp248 : fRec153[1]);
			float fTemp251 = std::exp(-(fConst4 * fRec153[0]));
			fRec151[0] = fVec2[2] + fRec151[1] * (fTemp250 + fTemp251) - fTemp250 * fTemp251 * fRec151[2];
			float fTemp252 = 3.5e+02f * fTemp57;
			float fTemp253 = fConst3 * ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 3.5e+03f : ((iTemp67) ? 3.5e+03f - 2e+02f * fTemp20 : 3.3e+03f)) : ((iTemp64) ? 4e+02f * fTemp68 + 3.3e+03f : 3.7e+03f)) : ((iTemp61) ? 3.7e+03f - 2e+02f * fTemp65 : 3.5e+03f)) : 3.5e+03f) : ((iTemp56) ? 3.5e+03f - 1.05e+03f * fTemp59 : 2.45e+03f)) : ((iTemp54) ? fTemp252 + 2.45e+03f : 2.8e+03f)) : ((iTemp52) ? 2.5e+02f * fTemp55 + 2.8e+03f : 3.05e+03f)) : ((iTemp51) ? 3.05e+03f - 4.5e+02f * fTemp53 : 2.6e+03f)) : ((iTemp49) ? 75.0f * fTemp81 + 2.6e+03f : 2675.0f)) : ((iTemp46) ? 325.0f * fTemp50 + 2675.0f : 3e+03f)) : 3e+03f) : ((iTemp43) ? 3.5e+02f * fTemp45 + 3e+03f : 3.35e+03f)) : ((iTemp42) ? 3.35e+03f - 3.5e+02f * fTemp80 : 3e+03f)) : 3e+03f) : ((iTemp37) ? 9e+02f * fTemp41 + 3e+03f : 3.9e+03f)) : ((iTemp36) ? 3.9e+03f - 3e+02f * fTemp38 : 3.6e+03f)) : ((iTemp34) ? 3e+02f * fTemp77 + 3.6e+03f : 3.9e+03f)) : ((iTemp32) ? 3.9e+03f - 1e+02f * fTemp35 : 3.8e+03f)) : 3.8e+03f) : ((iTemp28) ? 3.8e+03f - 9e+02f * fTemp76 : 2.9e+03f)) : ((iTemp26) ? 3e+02f * fTemp29 + 2.9e+03f : 3.2e+03f)) : ((iTemp24) ? 5e+01f * fTemp27 + 3.2e+03f : 3.25e+03f)) : ((iTemp21) ? 3.25e+03f - 4.5e+02f * fTemp25 : 2.8e+03f)) : ((iTemp22) ? fTemp209 + 2.8e+03f : 2.9e+03f));
			float fTemp254 = ((iTemp75) ? 0.0f : fRec154[1] + fTemp253);
			fRec154[0] = fTemp254 - std::floor(fTemp254);
			fRec156[0] = ((iTemp87) ? fTemp249 : fRec156[1]);
			float fTemp255 = std::exp(-(fConst4 * fRec156[0]));
			fRec157[0] = ((iTemp87) ? fTemp248 : fRec157[1]);
			float fTemp256 = std::exp(-(fConst4 * fRec157[0]));
			fRec155[0] = fVec3[2] + fRec155[1] * (fTemp255 + fTemp256) - fTemp255 * fTemp256 * fRec155[2];
			float fTemp257 = ((iTemp90) ? 0.0f : fTemp253 + fRec158[1]);
			fRec158[0] = fTemp257 - std::floor(fTemp257);
			fRec160[0] = ((iTemp93) ? fTemp249 : fRec160[1]);
			float fTemp258 = std::exp(-(fConst4 * fRec160[0]));
			fRec161[0] = ((iTemp93) ? fTemp248 : fRec161[1]);
			float fTemp259 = std::exp(-(fConst4 * fRec161[0]));
			fRec159[0] = fVec4[2] + fRec159[1] * (fTemp258 + fTemp259) - fTemp258 * fTemp259 * fRec159[2];
			float fTemp260 = ((iTemp96) ? 0.0f : fTemp253 + fRec162[1]);
			fRec162[0] = fTemp260 - std::floor(fTemp260);
			fRec164[0] = ((iTemp99) ? fTemp249 : fRec164[1]);
			float fTemp261 = std::exp(-(fConst4 * fRec164[0]));
			fRec165[0] = ((iTemp99) ? fTemp248 : fRec165[1]);
			float fTemp262 = std::exp(-(fConst4 * fRec165[0]));
			fRec163[0] = fVec5[2] + fRec163[1] * (fTemp261 + fTemp262) - fTemp261 * fTemp262 * fRec163[2];
			float fTemp263 = ((iTemp102) ? 0.0f : fTemp253 + fRec166[1]);
			fRec166[0] = fTemp263 - std::floor(fTemp263);
			fRec168[0] = ((iTemp105) ? fTemp249 : fRec168[1]);
			float fTemp264 = std::exp(-(fConst4 * fRec168[0]));
			fRec169[0] = ((iTemp105) ? fTemp248 : fRec169[1]);
			float fTemp265 = std::exp(-(fConst4 * fRec169[0]));
			fRec167[0] = fVec6[2] + fRec167[1] * (fTemp264 + fTemp265) - fTemp264 * fTemp265 * fRec167[2];
			float fTemp266 = ((iTemp108) ? 0.0f : fTemp253 + fRec170[1]);
			fRec170[0] = fTemp266 - std::floor(fTemp266);
			fRec172[0] = ((iTemp111) ? fTemp249 : fRec172[1]);
			float fTemp267 = std::exp(-(fConst4 * fRec172[0]));
			fRec173[0] = ((iTemp111) ? fTemp248 : fRec173[1]);
			float fTemp268 = std::exp(-(fConst4 * fRec173[0]));
			fRec171[0] = fVec7[2] + fRec171[1] * (fTemp267 + fTemp268) - fTemp267 * fTemp268 * fRec171[2];
			float fTemp269 = ((iTemp114) ? 0.0f : fTemp253 + fRec174[1]);
			fRec174[0] = fTemp269 - std::floor(fTemp269);
			fRec176[0] = ((iTemp117) ? fTemp249 : fRec176[1]);
			float fTemp270 = std::exp(-(fConst4 * fRec176[0]));
			fRec177[0] = ((iTemp117) ? fTemp248 : fRec177[1]);
			float fTemp271 = std::exp(-(fConst4 * fRec177[0]));
			fRec175[0] = fVec8[2] + fRec175[1] * (fTemp270 + fTemp271) - fTemp270 * fTemp271 * fRec175[2];
			float fTemp272 = ((iTemp120) ? 0.0f : fTemp253 + fRec178[1]);
			fRec178[0] = fTemp272 - std::floor(fTemp272);
			fRec180[0] = ((iTemp123) ? fTemp249 : fRec180[1]);
			float fTemp273 = std::exp(-(fConst4 * fRec180[0]));
			fRec181[0] = ((iTemp123) ? fTemp248 : fRec181[1]);
			float fTemp274 = std::exp(-(fConst4 * fRec181[0]));
			fRec179[0] = fVec9[2] + fRec179[1] * (fTemp273 + fTemp274) - fTemp273 * fTemp274 * fRec179[2];
			float fTemp275 = ((iTemp126) ? 0.0f : fTemp253 + fRec182[1]);
			fRec182[0] = fTemp275 - std::floor(fTemp275);
			fRec184[0] = ((iTemp129) ? fTemp249 : fRec184[1]);
			float fTemp276 = std::exp(-(fConst4 * fRec184[0]));
			fRec185[0] = ((iTemp129) ? fTemp248 : fRec185[1]);
			float fTemp277 = std::exp(-(fConst4 * fRec185[0]));
			fRec183[0] = fVec10[2] + fRec183[1] * (fTemp276 + fTemp277) - fTemp276 * fTemp277 * fRec183[2];
			float fTemp278 = ((iTemp132) ? 0.0f : fTemp253 + fRec186[1]);
			fRec186[0] = fTemp278 - std::floor(fTemp278);
			fRec188[0] = ((iTemp135) ? fTemp249 : fRec188[1]);
			float fTemp279 = std::exp(-(fConst4 * fRec188[0]));
			fRec189[0] = ((iTemp135) ? fTemp248 : fRec189[1]);
			float fTemp280 = std::exp(-(fConst4 * fRec189[0]));
			fRec187[0] = fVec11[2] + fRec187[1] * (fTemp279 + fTemp280) - fTemp279 * fTemp280 * fRec187[2];
			float fTemp281 = ((iTemp138) ? 0.0f : fTemp253 + fRec190[1]);
			fRec190[0] = fTemp281 - std::floor(fTemp281);
			fRec192[0] = ((iTemp141) ? fTemp249 : fRec192[1]);
			float fTemp282 = std::exp(-(fConst4 * fRec192[0]));
			fRec193[0] = ((iTemp141) ? fTemp248 : fRec193[1]);
			float fTemp283 = std::exp(-(fConst4 * fRec193[0]));
			fRec191[0] = fVec12[2] + fRec191[1] * (fTemp282 + fTemp283) - fTemp282 * fTemp283 * fRec191[2];
			float fTemp284 = ((iTemp144) ? 0.0f : fTemp253 + fRec194[1]);
			fRec194[0] = fTemp284 - std::floor(fTemp284);
			fRec196[0] = ((iTemp147) ? fTemp249 : fRec196[1]);
			float fTemp285 = std::exp(-(fConst4 * fRec196[0]));
			fRec197[0] = ((iTemp147) ? fTemp248 : fRec197[1]);
			float fTemp286 = std::exp(-(fConst4 * fRec197[0]));
			fRec195[0] = fVec13[2] + fRec195[1] * (fTemp285 + fTemp286) - fTemp285 * fTemp286 * fRec195[2];
			float fTemp287 = ((iTemp150) ? 0.0f : fTemp253 + fRec198[1]);
			fRec198[0] = fTemp287 - std::floor(fTemp287);
			float fTemp288 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? 0.001f : ((iTemp61) ? 0.000778f * fTemp65 + 0.001f : 0.001778f)) : ((iTemp58) ? 0.001778f - 0.001147f * fTemp62 : 0.000631f)) : ((iTemp56) ? 0.099369f * fTemp59 + 0.000631f : 0.1f)) : ((iTemp54) ? 0.025893f * fTemp57 + 0.1f : 0.125893f)) : ((iTemp52) ? 0.125893f - 0.086082f * fTemp55 : 0.039811f)) : ((iTemp51) ? 0.039811f - 0.029811f * fTemp53 : 0.01f)) : ((iTemp49) ? 0.005849f * fTemp81 + 0.01f : 0.015849f)) : ((iTemp46) ? 0.015849f - 0.00326f * fTemp50 : 0.012589f)) : ((iTemp44) ? 0.087411f * fTemp47 + 0.012589f : 0.1f)) : fTemp246) : ((iTemp42) ? 0.004104f * fTemp80 + 0.015849f : 0.019953f)) : 0.019953f) : ((iTemp37) ? 0.019953f - 0.016791f * fTemp41 : 0.003162f)) : ((iTemp36) ? 0.003162f - 0.001577f * fTemp38 : 0.001585f)) : ((iTemp34) ? 0.004725f * fTemp77 + 0.001585f : 0.00631f)) : ((iTemp32) ? 0.00631f - 0.003148f * fTemp35 : 0.003162f)) : ((iTemp31) ? 0.003162f - 0.002162f * fTemp33 : 0.001f)) : ((iTemp28) ? 0.078433f * fTemp76 + 0.001f : 0.079433f)) : ((iTemp26) ? 0.020567f * fTemp29 + 0.079433f : 0.1f)) : ((iTemp24) ? 0.1f - 0.068377f * fTemp27 : 0.031623f)) : ((iTemp21) ? 0.018496f * fTemp25 + 0.031623f : 0.050119f)) : 0.050119f);
			float fTemp289 = ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp61) ? ((iTemp64) ? ((iTemp67) ? ((iTemp69) ? 1.4e+02f : ((iTemp67) ? 6e+01f * fTemp20 + 1.4e+02f : 2e+02f)) : 2e+02f) : ((iTemp61) ? 2e+02f - 65.0f * fTemp65 : 135.0f)) : ((iTemp58) ? 65.0f * fTemp62 + 135.0f : 2e+02f)) : ((iTemp56) ? 2e+02f - 7e+01f * fTemp59 : 1.3e+02f)) : ((iTemp54) ? 1.3e+02f - fTemp156 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp46) ? fTemp203 + 1.2e+02f : 1.4e+02f)) : ((iTemp44) ? 1.4e+02f - fTemp202 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp37) ? fTemp201 + 1.2e+02f : 1.4e+02f)) : ((iTemp36) ? 6e+01f * fTemp38 + 1.4e+02f : 2e+02f)) : ((iTemp34) ? 2e+02f - fTemp78 : 1.2e+02f)) : 1.2e+02f) : ((iTemp31) ? 8e+01f * fTemp33 + 1.2e+02f : 2e+02f)) : ((iTemp28) ? 2e+02f - 6e+01f * fTemp76 : 1.4e+02f)) : ((iTemp26) ? 1.4e+02f - fTemp30 : 1.2e+02f)) : 1.2e+02f) : ((iTemp21) ? 15.0f * fTemp25 + 1.2e+02f : 135.0f)) : ((iTemp22) ? 135.0f - 15.0f * fTemp23 : 1.2e+02f));
			float fTemp290 = fTemp19 * fTemp289;
			fRec200[0] = ((iTemp8) ? fTemp290 : fRec200[1]);
			float fTemp291 = std::exp(-(fConst4 * fRec200[0]));
			fRec201[0] = ((iTemp8) ? fTemp289 : fRec201[1]);
			float fTemp292 = std::exp(-(fConst4 * fRec201[0]));
			fRec199[0] = fVec2[2] + fRec199[1] * (fTemp291 + fTemp292) - fTemp291 * fTemp292 * fRec199[2];
			float fTemp293 = fConst3 * ((iTemp21) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp31) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp40) ? ((iTemp42) ? ((iTemp43) ? ((iTemp44) ? ((iTemp46) ? ((iTemp49) ? ((iTemp51) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? 4.95e+03f : ((iTemp56) ? 4.95e+03f - 2.2e+03f * fTemp59 : 2.75e+03f)) : ((iTemp54) ? fTemp252 + 2.75e+03f : 3.1e+03f)) : ((iTemp52) ? 2.4e+02f * fTemp55 + 3.1e+03f : 3.34e+03f)) : ((iTemp51) ? 3.34e+03f - 4.4e+02f * fTemp53 : 2.9e+03f)) : ((iTemp49) ? fTemp82 + 2.9e+03f : 2.95e+03f)) : ((iTemp46) ? 4e+02f * fTemp50 + 2.95e+03f : 3.35e+03f)) : ((iTemp44) ? 3.35e+03f - fTemp210 : 3.3e+03f)) : ((iTemp43) ? 2.9e+02f * fTemp45 + 3.3e+03f : 3.59e+03f)) : ((iTemp42) ? 3.59e+03f - 2.9e+02f * fTemp80 : 3.3e+03f)) : ((iTemp40) ? 1e+02f * fTemp79 + 3.3e+03f : 3.4e+03f)) : ((iTemp37) ? 1.55e+03f * fTemp41 + 3.4e+03f : 4.95e+03f)) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : ((iTemp28) ? 4.95e+03f - 1.7e+03f * fTemp76 : 3.25e+03f)) : ((iTemp26) ? 3.3e+02f * fTemp29 + 3.25e+03f : 3.58e+03f)) : ((iTemp24) ? 3.58e+03f - 4e+01f * fTemp27 : 3.54e+03f)) : ((iTemp21) ? 3.54e+03f - 5.4e+02f * fTemp25 : 3e+03f)) : ((iTemp22) ? 3e+02f * fTemp23 + 3e+03f : 3.3e+03f));
			float fTemp294 = ((iTemp75) ? 0.0f : fRec202[1] + fTemp293);
			fRec202[0] = fTemp294 - std::floor(fTemp294);
			fRec204[0] = ((iTemp87) ? fTemp290 : fRec204[1]);
			float fTemp295 = std::exp(-(fConst4 * fRec204[0]));
			fRec205[0] = ((iTemp87) ? fTemp289 : fRec205[1]);
			float fTemp296 = std::exp(-(fConst4 * fRec205[0]));
			fRec203[0] = fVec3[2] + fRec203[1] * (fTemp295 + fTemp296) - fTemp295 * fTemp296 * fRec203[2];
			float fTemp297 = ((iTemp90) ? 0.0f : fTemp293 + fRec206[1]);
			fRec206[0] = fTemp297 - std::floor(fTemp297);
			fRec208[0] = ((iTemp93) ? fTemp290 : fRec208[1]);
			float fTemp298 = std::exp(-(fConst4 * fRec208[0]));
			fRec209[0] = ((iTemp93) ? fTemp289 : fRec209[1]);
			float fTemp299 = std::exp(-(fConst4 * fRec209[0]));
			fRec207[0] = fVec4[2] + fRec207[1] * (fTemp298 + fTemp299) - fTemp298 * fTemp299 * fRec207[2];
			float fTemp300 = ((iTemp96) ? 0.0f : fTemp293 + fRec210[1]);
			fRec210[0] = fTemp300 - std::floor(fTemp300);
			fRec212[0] = ((iTemp99) ? fTemp290 : fRec212[1]);
			float fTemp301 = std::exp(-(fConst4 * fRec212[0]));
			fRec213[0] = ((iTemp99) ? fTemp289 : fRec213[1]);
			float fTemp302 = std::exp(-(fConst4 * fRec213[0]));
			fRec211[0] = fVec5[2] + fRec211[1] * (fTemp301 + fTemp302) - fTemp301 * fTemp302 * fRec211[2];
			float fTemp303 = ((iTemp102) ? 0.0f : fTemp293 + fRec214[1]);
			fRec214[0] = fTemp303 - std::floor(fTemp303);
			fRec216[0] = ((iTemp105) ? fTemp290 : fRec216[1]);
			float fTemp304 = std::exp(-(fConst4 * fRec216[0]));
			fRec217[0] = ((iTemp105) ? fTemp289 : fRec217[1]);
			float fTemp305 = std::exp(-(fConst4 * fRec217[0]));
			fRec215[0] = fVec6[2] + fRec215[1] * (fTemp304 + fTemp305) - fTemp304 * fTemp305 * fRec215[2];
			float fTemp306 = ((iTemp108) ? 0.0f : fTemp293 + fRec218[1]);
			fRec218[0] = fTemp306 - std::floor(fTemp306);
			fRec220[0] = ((iTemp111) ? fTemp290 : fRec220[1]);
			float fTemp307 = std::exp(-(fConst4 * fRec220[0]));
			fRec221[0] = ((iTemp111) ? fTemp289 : fRec221[1]);
			float fTemp308 = std::exp(-(fConst4 * fRec221[0]));
			fRec219[0] = fVec7[2] + fRec219[1] * (fTemp307 + fTemp308) - fTemp307 * fTemp308 * fRec219[2];
			float fTemp309 = ((iTemp114) ? 0.0f : fTemp293 + fRec222[1]);
			fRec222[0] = fTemp309 - std::floor(fTemp309);
			fRec224[0] = ((iTemp117) ? fTemp290 : fRec224[1]);
			float fTemp310 = std::exp(-(fConst4 * fRec224[0]));
			fRec225[0] = ((iTemp117) ? fTemp289 : fRec225[1]);
			float fTemp311 = std::exp(-(fConst4 * fRec225[0]));
			fRec223[0] = fVec8[2] + fRec223[1] * (fTemp310 + fTemp311) - fTemp310 * fTemp311 * fRec223[2];
			float fTemp312 = ((iTemp120) ? 0.0f : fTemp293 + fRec226[1]);
			fRec226[0] = fTemp312 - std::floor(fTemp312);
			fRec228[0] = ((iTemp123) ? fTemp290 : fRec228[1]);
			float fTemp313 = std::exp(-(fConst4 * fRec228[0]));
			fRec229[0] = ((iTemp123) ? fTemp289 : fRec229[1]);
			float fTemp314 = std::exp(-(fConst4 * fRec229[0]));
			fRec227[0] = fVec9[2] + fRec227[1] * (fTemp313 + fTemp314) - fTemp313 * fTemp314 * fRec227[2];
			float fTemp315 = ((iTemp126) ? 0.0f : fTemp293 + fRec230[1]);
			fRec230[0] = fTemp315 - std::floor(fTemp315);
			fRec232[0] = ((iTemp129) ? fTemp290 : fRec232[1]);
			float fTemp316 = std::exp(-(fConst4 * fRec232[0]));
			fRec233[0] = ((iTemp129) ? fTemp289 : fRec233[1]);
			float fTemp317 = std::exp(-(fConst4 * fRec233[0]));
			fRec231[0] = fVec10[2] + fRec231[1] * (fTemp316 + fTemp317) - fTemp316 * fTemp317 * fRec231[2];
			float fTemp318 = ((iTemp132) ? 0.0f : fTemp293 + fRec234[1]);
			fRec234[0] = fTemp318 - std::floor(fTemp318);
			fRec236[0] = ((iTemp135) ? fTemp290 : fRec236[1]);
			float fTemp319 = std::exp(-(fConst4 * fRec236[0]));
			fRec237[0] = ((iTemp135) ? fTemp289 : fRec237[1]);
			float fTemp320 = std::exp(-(fConst4 * fRec237[0]));
			fRec235[0] = fVec11[2] + fRec235[1] * (fTemp319 + fTemp320) - fTemp319 * fTemp320 * fRec235[2];
			float fTemp321 = ((iTemp138) ? 0.0f : fTemp293 + fRec238[1]);
			fRec238[0] = fTemp321 - std::floor(fTemp321);
			fRec240[0] = ((iTemp141) ? fTemp290 : fRec240[1]);
			float fTemp322 = std::exp(-(fConst4 * fRec240[0]));
			fRec241[0] = ((iTemp141) ? fTemp289 : fRec241[1]);
			float fTemp323 = std::exp(-(fConst4 * fRec241[0]));
			fRec239[0] = fVec12[2] + fRec239[1] * (fTemp322 + fTemp323) - fTemp322 * fTemp323 * fRec239[2];
			float fTemp324 = ((iTemp144) ? 0.0f : fTemp293 + fRec242[1]);
			fRec242[0] = fTemp324 - std::floor(fTemp324);
			fRec244[0] = ((iTemp147) ? fTemp290 : fRec244[1]);
			float fTemp325 = std::exp(-(fConst4 * fRec244[0]));
			fRec245[0] = ((iTemp147) ? fTemp289 : fRec245[1]);
			float fTemp326 = std::exp(-(fConst4 * fRec245[0]));
			fRec243[0] = fVec13[2] + fRec243[1] * (fTemp325 + fTemp326) - fTemp325 * fTemp326 * fRec243[2];
			float fTemp327 = ((iTemp150) ? 0.0f : fTemp293 + fRec246[1]);
			fRec246[0] = fTemp327 - std::floor(fTemp327);
			fVec14[0] = fSlow22;
			fRec247[0] = fSlow22 + fRec247[1] * static_cast<float>(fVec14[1] >= fSlow22);
			iRec248[0] = iSlow24 * (iRec248[1] + 1);
			output0[i0] = static_cast<FAUSTFLOAT>(5e+01f * (((iSlow2) ? fTemp4 : fTemp3) * (fRec4[0] * (1.0f - (fTemp74 + fTemp73 * (1.0f - fTemp74))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec10[0]), 65535))] + fRec11[0] * (1.0f - (fTemp89 + fTemp88 * (1.0f - fTemp89))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec14[0]), 65535))] + fRec15[0] * (1.0f - (fTemp95 + fTemp94 * (1.0f - fTemp95))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec18[0]), 65535))] + fRec19[0] * (1.0f - (fTemp101 + fTemp100 * (1.0f - fTemp101))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec22[0]), 65535))] + fRec23[0] * (1.0f - (fTemp107 + fTemp106 * (1.0f - fTemp107))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec26[0]), 65535))] + fRec27[0] * (1.0f - (fTemp113 + fTemp112 * (1.0f - fTemp113))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec30[0]), 65535))] + fRec31[0] * (1.0f - (fTemp119 + fTemp118 * (1.0f - fTemp119))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec34[0]), 65535))] + fRec35[0] * (1.0f - (fTemp125 + fTemp124 * (1.0f - fTemp125))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec38[0]), 65535))] + fRec39[0] * (1.0f - (fTemp131 + fTemp130 * (1.0f - fTemp131))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec42[0]), 65535))] + fRec43[0] * (1.0f - (fTemp137 + fTemp136 * (1.0f - fTemp137))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec46[0]), 65535))] + fRec47[0] * (1.0f - (fTemp143 + fTemp142 * (1.0f - fTemp143))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec50[0]), 65535))] + fRec51[0] * (1.0f - (fTemp149 + fTemp148 * (1.0f - fTemp149))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec54[0]), 65535))]) + ((iSlow2) ? fTemp4 * fTemp152 : fTemp3 * fTemp152) * (fRec55[0] * (1.0f - (fTemp160 + fTemp159 * (1.0f - fTemp160))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec58[0]), 65535))] + fRec59[0] * (1.0f - (fTemp166 + fTemp165 * (1.0f - fTemp166))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec62[0]), 65535))] + fRec63[0] * (1.0f - (fTemp169 + fTemp168 * (1.0f - fTemp169))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec66[0]), 65535))] + fRec67[0] * (1.0f - (fTemp172 + fTemp171 * (1.0f - fTemp172))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec70[0]), 65535))] + fRec71[0] * (1.0f - (fTemp175 + fTemp174 * (1.0f - fTemp175))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec74[0]), 65535))] + fRec75[0] * (1.0f - (fTemp178 + fTemp177 * (1.0f - fTemp178))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec78[0]), 65535))] + fRec79[0] * (1.0f - (fTemp181 + fTemp180 * (1.0f - fTemp181))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec82[0]), 65535))] + fRec83[0] * (1.0f - (fTemp184 + fTemp183 * (1.0f - fTemp184))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec86[0]), 65535))] + fRec87[0] * (1.0f - (fTemp187 + fTemp186 * (1.0f - fTemp187))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec90[0]), 65535))] + fRec91[0] * (1.0f - (fTemp190 + fTemp189 * (1.0f - fTemp190))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec94[0]), 65535))] + fRec95[0] * (1.0f - (fTemp193 + fTemp192 * (1.0f - fTemp193))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec98[0]), 65535))] + fRec99[0] * (1.0f - (fTemp196 + fTemp195 * (1.0f - fTemp196))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec102[0]), 65535))]) + ((iSlow2) ? fTemp4 * fTemp199 : fTemp3 * fTemp199) * (fRec103[0] * (1.0f - (fTemp208 + fTemp207 * (1.0f - fTemp208))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec106[0]), 65535))] + fRec107[0] * (1.0f - (fTemp214 + fTemp213 * (1.0f - fTemp214))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec110[0]), 65535))] + fRec111[0] * (1.0f - (fTemp217 + fTemp216 * (1.0f - fTemp217))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec114[0]), 65535))] + fRec115[0] * (1.0f - (fTemp220 + fTemp219 * (1.0f - fTemp220))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec118[0]), 65535))] + fRec119[0] * (1.0f - (fTemp223 + fTemp222 * (1.0f - fTemp223))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec122[0]), 65535))] + fRec123[0] * (1.0f - (fTemp226 + fTemp225 * (1.0f - fTemp226))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec126[0]), 65535))] + fRec127[0] * (1.0f - (fTemp229 + fTemp228 * (1.0f - fTemp229))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec130[0]), 65535))] + fRec131[0] * (1.0f - (fTemp232 + fTemp231 * (1.0f - fTemp232))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec134[0]), 65535))] + fRec135[0] * (1.0f - (fTemp235 + fTemp234 * (1.0f - fTemp235))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec138[0]), 65535))] + fRec139[0] * (1.0f - (fTemp238 + fTemp237 * (1.0f - fTemp238))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec142[0]), 65535))] + fRec143[0] * (1.0f - (fTemp241 + fTemp240 * (1.0f - fTemp241))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec146[0]), 65535))] + fRec147[0] * (1.0f - (fTemp244 + fTemp243 * (1.0f - fTemp244))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec150[0]), 65535))]) + ((iSlow2) ? fTemp4 * fTemp247 : fTemp3 * fTemp247) * (fRec151[0] * (1.0f - (fTemp251 + fTemp250 * (1.0f - fTemp251))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec154[0]), 65535))] + fRec155[0] * (1.0f - (fTemp256 + fTemp255 * (1.0f - fTemp256))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec158[0]), 65535))] + fRec159[0] * (1.0f - (fTemp259 + fTemp258 * (1.0f - fTemp259))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec162[0]), 65535))] + fRec163[0] * (1.0f - (fTemp262 + fTemp261 * (1.0f - fTemp262))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec166[0]), 65535))] + fRec167[0] * (1.0f - (fTemp265 + fTemp264 * (1.0f - fTemp265))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec170[0]), 65535))] + fRec171[0] * (1.0f - (fTemp268 + fTemp267 * (1.0f - fTemp268))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec174[0]), 65535))] + fRec175[0] * (1.0f - (fTemp271 + fTemp270 * (1.0f - fTemp271))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec178[0]), 65535))] + fRec179[0] * (1.0f - (fTemp274 + fTemp273 * (1.0f - fTemp274))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec182[0]), 65535))] + fRec183[0] * (1.0f - (fTemp277 + fTemp276 * (1.0f - fTemp277))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec186[0]), 65535))] + fRec187[0] * (1.0f - (fTemp280 + fTemp279 * (1.0f - fTemp280))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec190[0]), 65535))] + fRec191[0] * (1.0f - (fTemp283 + fTemp282 * (1.0f - fTemp283))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec194[0]), 65535))] + fRec195[0] * (1.0f - (fTemp286 + fTemp285 * (1.0f - fTemp286))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec198[0]), 65535))]) + ((iSlow2) ? fTemp4 * fTemp288 : fTemp3 * fTemp288) * (fRec199[0] * (1.0f - (fTemp292 + fTemp291 * (1.0f - fTemp292))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec202[0]), 65535))] + fRec203[0] * (1.0f - (fTemp296 + fTemp295 * (1.0f - fTemp296))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec206[0]), 65535))] + fRec207[0] * (1.0f - (fTemp299 + fTemp298 * (1.0f - fTemp299))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec210[0]), 65535))] + fRec211[0] * (1.0f - (fTemp302 + fTemp301 * (1.0f - fTemp302))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec214[0]), 65535))] + fRec215[0] * (1.0f - (fTemp305 + fTemp304 * (1.0f - fTemp305))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec218[0]), 65535))] + fRec219[0] * (1.0f - (fTemp308 + fTemp307 * (1.0f - fTemp308))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec222[0]), 65535))] + fRec223[0] * (1.0f - (fTemp311 + fTemp310 * (1.0f - fTemp311))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec226[0]), 65535))] + fRec227[0] * (1.0f - (fTemp314 + fTemp313 * (1.0f - fTemp314))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec230[0]), 65535))] + fRec231[0] * (1.0f - (fTemp317 + fTemp316 * (1.0f - fTemp317))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec234[0]), 65535))] + fRec235[0] * (1.0f - (fTemp320 + fTemp319 * (1.0f - fTemp320))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec238[0]), 65535))] + fRec239[0] * (1.0f - (fTemp323 + fTemp322 * (1.0f - fTemp323))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec242[0]), 65535))] + fRec243[0] * (1.0f - (fTemp326 + fTemp325 * (1.0f - fTemp326))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec246[0]), 65535))])) * std::max<float>(0.0f, std::min<float>(fSlow21 * fRec247[0], 1.0f) - fSlow23 * static_cast<float>(iRec248[0])));
			iVec0[1] = iVec0[0];
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
			fRec3[1] = fRec3[0];
			fRec6[1] = fRec6[0];
			fRec5[1] = fRec5[0];
			fVec2[2] = fVec2[1];
			fVec2[1] = fVec2[0];
			fRec8[1] = fRec8[0];
			fRec7[1] = fRec7[0];
			fRec9[1] = fRec9[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec10[1] = fRec10[0];
			fVec3[2] = fVec3[1];
			fVec3[1] = fVec3[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec14[1] = fRec14[0];
			fVec4[2] = fVec4[1];
			fVec4[1] = fVec4[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec18[1] = fRec18[0];
			fVec5[2] = fVec5[1];
			fVec5[1] = fVec5[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec22[1] = fRec22[0];
			fVec6[2] = fVec6[1];
			fVec6[1] = fVec6[0];
			fRec24[1] = fRec24[0];
			fRec25[1] = fRec25[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec26[1] = fRec26[0];
			fVec7[2] = fVec7[1];
			fVec7[1] = fVec7[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec30[1] = fRec30[0];
			fVec8[2] = fVec8[1];
			fVec8[1] = fVec8[0];
			fRec32[1] = fRec32[0];
			fRec33[1] = fRec33[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec34[1] = fRec34[0];
			fVec9[2] = fVec9[1];
			fVec9[1] = fVec9[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec38[1] = fRec38[0];
			fVec10[2] = fVec10[1];
			fVec10[1] = fVec10[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec42[1] = fRec42[0];
			fVec11[2] = fVec11[1];
			fVec11[1] = fVec11[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec46[1] = fRec46[0];
			fVec12[2] = fVec12[1];
			fVec12[1] = fVec12[0];
			fRec48[1] = fRec48[0];
			fRec49[1] = fRec49[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec50[1] = fRec50[0];
			fVec13[2] = fVec13[1];
			fVec13[1] = fVec13[0];
			fRec52[1] = fRec52[0];
			fRec53[1] = fRec53[0];
			fRec51[2] = fRec51[1];
			fRec51[1] = fRec51[0];
			fRec54[1] = fRec54[0];
			fRec56[1] = fRec56[0];
			fRec57[1] = fRec57[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec58[1] = fRec58[0];
			fRec60[1] = fRec60[0];
			fRec61[1] = fRec61[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec62[1] = fRec62[0];
			fRec64[1] = fRec64[0];
			fRec65[1] = fRec65[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec66[1] = fRec66[0];
			fRec68[1] = fRec68[0];
			fRec69[1] = fRec69[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fRec70[1] = fRec70[0];
			fRec72[1] = fRec72[0];
			fRec73[1] = fRec73[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec74[1] = fRec74[0];
			fRec76[1] = fRec76[0];
			fRec77[1] = fRec77[0];
			fRec75[2] = fRec75[1];
			fRec75[1] = fRec75[0];
			fRec78[1] = fRec78[0];
			fRec80[1] = fRec80[0];
			fRec81[1] = fRec81[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec82[1] = fRec82[0];
			fRec84[1] = fRec84[0];
			fRec85[1] = fRec85[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec86[1] = fRec86[0];
			fRec88[1] = fRec88[0];
			fRec89[1] = fRec89[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec90[1] = fRec90[0];
			fRec92[1] = fRec92[0];
			fRec93[1] = fRec93[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fRec94[1] = fRec94[0];
			fRec96[1] = fRec96[0];
			fRec97[1] = fRec97[0];
			fRec95[2] = fRec95[1];
			fRec95[1] = fRec95[0];
			fRec98[1] = fRec98[0];
			fRec100[1] = fRec100[0];
			fRec101[1] = fRec101[0];
			fRec99[2] = fRec99[1];
			fRec99[1] = fRec99[0];
			fRec102[1] = fRec102[0];
			fRec104[1] = fRec104[0];
			fRec105[1] = fRec105[0];
			fRec103[2] = fRec103[1];
			fRec103[1] = fRec103[0];
			fRec106[1] = fRec106[0];
			fRec108[1] = fRec108[0];
			fRec109[1] = fRec109[0];
			fRec107[2] = fRec107[1];
			fRec107[1] = fRec107[0];
			fRec110[1] = fRec110[0];
			fRec112[1] = fRec112[0];
			fRec113[1] = fRec113[0];
			fRec111[2] = fRec111[1];
			fRec111[1] = fRec111[0];
			fRec114[1] = fRec114[0];
			fRec116[1] = fRec116[0];
			fRec117[1] = fRec117[0];
			fRec115[2] = fRec115[1];
			fRec115[1] = fRec115[0];
			fRec118[1] = fRec118[0];
			fRec120[1] = fRec120[0];
			fRec121[1] = fRec121[0];
			fRec119[2] = fRec119[1];
			fRec119[1] = fRec119[0];
			fRec122[1] = fRec122[0];
			fRec124[1] = fRec124[0];
			fRec125[1] = fRec125[0];
			fRec123[2] = fRec123[1];
			fRec123[1] = fRec123[0];
			fRec126[1] = fRec126[0];
			fRec128[1] = fRec128[0];
			fRec129[1] = fRec129[0];
			fRec127[2] = fRec127[1];
			fRec127[1] = fRec127[0];
			fRec130[1] = fRec130[0];
			fRec132[1] = fRec132[0];
			fRec133[1] = fRec133[0];
			fRec131[2] = fRec131[1];
			fRec131[1] = fRec131[0];
			fRec134[1] = fRec134[0];
			fRec136[1] = fRec136[0];
			fRec137[1] = fRec137[0];
			fRec135[2] = fRec135[1];
			fRec135[1] = fRec135[0];
			fRec138[1] = fRec138[0];
			fRec140[1] = fRec140[0];
			fRec141[1] = fRec141[0];
			fRec139[2] = fRec139[1];
			fRec139[1] = fRec139[0];
			fRec142[1] = fRec142[0];
			fRec144[1] = fRec144[0];
			fRec145[1] = fRec145[0];
			fRec143[2] = fRec143[1];
			fRec143[1] = fRec143[0];
			fRec146[1] = fRec146[0];
			fRec148[1] = fRec148[0];
			fRec149[1] = fRec149[0];
			fRec147[2] = fRec147[1];
			fRec147[1] = fRec147[0];
			fRec150[1] = fRec150[0];
			fRec152[1] = fRec152[0];
			fRec153[1] = fRec153[0];
			fRec151[2] = fRec151[1];
			fRec151[1] = fRec151[0];
			fRec154[1] = fRec154[0];
			fRec156[1] = fRec156[0];
			fRec157[1] = fRec157[0];
			fRec155[2] = fRec155[1];
			fRec155[1] = fRec155[0];
			fRec158[1] = fRec158[0];
			fRec160[1] = fRec160[0];
			fRec161[1] = fRec161[0];
			fRec159[2] = fRec159[1];
			fRec159[1] = fRec159[0];
			fRec162[1] = fRec162[0];
			fRec164[1] = fRec164[0];
			fRec165[1] = fRec165[0];
			fRec163[2] = fRec163[1];
			fRec163[1] = fRec163[0];
			fRec166[1] = fRec166[0];
			fRec168[1] = fRec168[0];
			fRec169[1] = fRec169[0];
			fRec167[2] = fRec167[1];
			fRec167[1] = fRec167[0];
			fRec170[1] = fRec170[0];
			fRec172[1] = fRec172[0];
			fRec173[1] = fRec173[0];
			fRec171[2] = fRec171[1];
			fRec171[1] = fRec171[0];
			fRec174[1] = fRec174[0];
			fRec176[1] = fRec176[0];
			fRec177[1] = fRec177[0];
			fRec175[2] = fRec175[1];
			fRec175[1] = fRec175[0];
			fRec178[1] = fRec178[0];
			fRec180[1] = fRec180[0];
			fRec181[1] = fRec181[0];
			fRec179[2] = fRec179[1];
			fRec179[1] = fRec179[0];
			fRec182[1] = fRec182[0];
			fRec184[1] = fRec184[0];
			fRec185[1] = fRec185[0];
			fRec183[2] = fRec183[1];
			fRec183[1] = fRec183[0];
			fRec186[1] = fRec186[0];
			fRec188[1] = fRec188[0];
			fRec189[1] = fRec189[0];
			fRec187[2] = fRec187[1];
			fRec187[1] = fRec187[0];
			fRec190[1] = fRec190[0];
			fRec192[1] = fRec192[0];
			fRec193[1] = fRec193[0];
			fRec191[2] = fRec191[1];
			fRec191[1] = fRec191[0];
			fRec194[1] = fRec194[0];
			fRec196[1] = fRec196[0];
			fRec197[1] = fRec197[0];
			fRec195[2] = fRec195[1];
			fRec195[1] = fRec195[0];
			fRec198[1] = fRec198[0];
			fRec200[1] = fRec200[0];
			fRec201[1] = fRec201[0];
			fRec199[2] = fRec199[1];
			fRec199[1] = fRec199[0];
			fRec202[1] = fRec202[0];
			fRec204[1] = fRec204[0];
			fRec205[1] = fRec205[0];
			fRec203[2] = fRec203[1];
			fRec203[1] = fRec203[0];
			fRec206[1] = fRec206[0];
			fRec208[1] = fRec208[0];
			fRec209[1] = fRec209[0];
			fRec207[2] = fRec207[1];
			fRec207[1] = fRec207[0];
			fRec210[1] = fRec210[0];
			fRec212[1] = fRec212[0];
			fRec213[1] = fRec213[0];
			fRec211[2] = fRec211[1];
			fRec211[1] = fRec211[0];
			fRec214[1] = fRec214[0];
			fRec216[1] = fRec216[0];
			fRec217[1] = fRec217[0];
			fRec215[2] = fRec215[1];
			fRec215[1] = fRec215[0];
			fRec218[1] = fRec218[0];
			fRec220[1] = fRec220[0];
			fRec221[1] = fRec221[0];
			fRec219[2] = fRec219[1];
			fRec219[1] = fRec219[0];
			fRec222[1] = fRec222[0];
			fRec224[1] = fRec224[0];
			fRec225[1] = fRec225[0];
			fRec223[2] = fRec223[1];
			fRec223[1] = fRec223[0];
			fRec226[1] = fRec226[0];
			fRec228[1] = fRec228[0];
			fRec229[1] = fRec229[0];
			fRec227[2] = fRec227[1];
			fRec227[1] = fRec227[0];
			fRec230[1] = fRec230[0];
			fRec232[1] = fRec232[0];
			fRec233[1] = fRec233[0];
			fRec231[2] = fRec231[1];
			fRec231[1] = fRec231[0];
			fRec234[1] = fRec234[0];
			fRec236[1] = fRec236[0];
			fRec237[1] = fRec237[0];
			fRec235[2] = fRec235[1];
			fRec235[1] = fRec235[0];
			fRec238[1] = fRec238[0];
			fRec240[1] = fRec240[0];
			fRec241[1] = fRec241[0];
			fRec239[2] = fRec239[1];
			fRec239[1] = fRec239[0];
			fRec242[1] = fRec242[0];
			fRec244[1] = fRec244[0];
			fRec245[1] = fRec245[0];
			fRec243[2] = fRec243[1];
			fRec243[1] = fRec243[0];
			fRec246[1] = fRec246[0];
			fVec14[1] = fVec14[0];
			fRec247[1] = fRec247[0];
			iRec248[1] = iRec248[0];
		}
	}

};
#endif

class faust_synth_voice_fofc_tilde : public FaustExternal<synth_voice_fofc> {
public:
    faust_synth_voice_fofc_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_voice_fofc))
    {
    }
};

#endif
