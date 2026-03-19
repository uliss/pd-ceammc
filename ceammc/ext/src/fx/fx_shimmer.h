/* ------------------------------------------------------------
author: "thedrgreenthumb"
license: "MIT"
name: "fx.shimmer"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -cn fx_shimmer -scn fx_shimmer_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_shimmer_H__
#define  __fx_shimmer_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_shimmer_dsp.h ********************************
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

class FAUST_API fx_shimmer_dsp {

    public:

        fx_shimmer_dsp() = default;
        virtual ~fx_shimmer_dsp() = default;

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
        virtual ::fx_shimmer_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public ::fx_shimmer_dsp {

    protected:

        ::fx_shimmer_dsp* fDSP;

    public:

        decorator_dsp(::fx_shimmer_dsp* fx_shimmer_dsp = nullptr):fDSP(fx_shimmer_dsp) {}
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
        virtual ::fx_shimmer_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_shimmer_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_shimmer_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_shimmer : public fx_shimmer_dsp {
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
#define FAUSTCLASS fx_shimmer
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

class fx_shimmerSIG0 {
	
  private:
	
	int iVec2[2];
	int iRec14[2];
	int fSampleRate;
	
  public:
	
	int getNumInputsfx_shimmerSIG0() {
		return 0;
	}
	int getNumOutputsfx_shimmerSIG0() {
		return 1;
	}
	
	void instanceInitfx_shimmerSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iVec2[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iRec14[l6] = 0;
		}
	}
	
	void fillfx_shimmerSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec2[0] = 1;
			iRec14[0] = (iVec2[1] + iRec14[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * static_cast<float>(iRec14[0]));
			iVec2[1] = iVec2[0];
			iRec14[1] = iRec14[0];
		}
	}

};

static fx_shimmerSIG0* newfx_shimmerSIG0() { return (fx_shimmerSIG0*)new fx_shimmerSIG0(); }
static void deletefx_shimmerSIG0(fx_shimmerSIG0* dsp) { delete dsp; }

static float fx_shimmer_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0fx_shimmerSIG0[65536];

class fx_shimmer : public fx_shimmer_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	int iVec0[2];
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fConst3;
	float fConst4;
	FAUSTFLOAT fHslider2;
	float fConst5;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fConst6;
	FAUSTFLOAT fHslider5;
	float fRec12[2];
	float fRec11[2];
	int IOTA0;
	float fVec1[131072];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHslider7;
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	float fConst7;
	FAUSTFLOAT fHslider10;
	float fRec15[2];
	FAUSTFLOAT fHslider11;
	float fRec16[2];
	float fRec13[2];
	float fVec3[65536];
	float fConst8;
	int iConst9;
	float fVec4[4096];
	int iConst10;
	float fRec9[2];
	float fConst11;
	float fConst12;
	float fRec20[2];
	float fRec19[2];
	float fVec5[131072];
	float fRec22[2];
	float fRec21[2];
	float fVec6[65536];
	float fConst13;
	int iConst14;
	float fVec7[8192];
	int iConst15;
	float fRec17[2];
	float fConst16;
	float fConst17;
	float fRec26[2];
	float fRec25[2];
	float fVec8[131072];
	float fRec28[2];
	float fRec27[2];
	float fVec9[32768];
	float fConst18;
	int iConst19;
	float fVec10[8192];
	int iConst20;
	float fRec23[2];
	float fConst21;
	float fConst22;
	float fRec32[2];
	float fRec31[2];
	float fVec11[131072];
	float fRec34[2];
	float fRec33[2];
	float fVec12[65536];
	float fConst23;
	int iConst24;
	float fVec13[8192];
	int iConst25;
	float fRec29[2];
	float fConst26;
	float fConst27;
	float fRec38[2];
	float fRec37[2];
	float fVec14[131072];
	float fRec40[2];
	float fRec39[2];
	float fVec15[32768];
	float fConst28;
	int iConst29;
	float fVec16[4096];
	int iConst30;
	float fRec35[2];
	float fConst31;
	float fConst32;
	float fRec44[2];
	float fRec43[2];
	float fVec17[131072];
	float fRec46[2];
	float fRec45[2];
	float fVec18[32768];
	float fConst33;
	int iConst34;
	float fVec19[8192];
	int iConst35;
	float fRec41[2];
	float fConst36;
	float fConst37;
	float fRec50[2];
	float fRec49[2];
	float fVec20[131072];
	float fRec52[2];
	float fRec51[2];
	float fVec21[32768];
	float fConst38;
	int iConst39;
	float fVec22[8192];
	int iConst40;
	float fRec47[2];
	float fConst41;
	float fConst42;
	float fRec56[2];
	float fRec55[2];
	float fVec23[131072];
	float fRec58[2];
	float fRec57[2];
	float fVec24[32768];
	float fConst43;
	int iConst44;
	float fVec25[4096];
	int iConst45;
	float fRec53[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	
 public:
	fx_shimmer() {
	}
	
	fx_shimmer(const fx_shimmer&) = default;
	
	virtual ~fx_shimmer() = default;
	
	fx_shimmer& operator=(const fx_shimmer&) = default;
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.3.0");
		m->declare("author", "thedrgreenthumb");
		m->declare("basics.lib/bypass2:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -cn fx_shimmer -scn fx_shimmer_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "fx_shimmer.dsp");
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
		m->declare("id", "fx.shimmer");
		m->declare("license", "MIT");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "fx.shimmer");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.7.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		fx_shimmerSIG0* sig0 = newfx_shimmerSIG0();
		sig0->instanceInitfx_shimmerSIG0(sample_rate);
		sig0->fillfx_shimmerSIG0(65536, ftbl0fx_shimmerSIG0);
		deletefx_shimmerSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = std::floor(0.219991f * fConst0 + 0.5f);
		fConst4 = 6.9077554f * (fConst3 / fConst0);
		fConst5 = 6.2831855f / fConst0;
		fConst6 = 3.1415927f / fConst0;
		fConst7 = 1.0f / fConst0;
		fConst8 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst9 = static_cast<int>(std::min<float>(32768.0f, std::max<float>(0.0f, fConst3 - fConst8)));
		iConst10 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst8 + -1.0f)));
		fConst11 = std::floor(0.256891f * fConst0 + 0.5f);
		fConst12 = 6.9077554f * (fConst11 / fConst0);
		fConst13 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst14 = static_cast<int>(std::min<float>(32768.0f, std::max<float>(0.0f, fConst11 - fConst13)));
		iConst15 = static_cast<int>(std::min<float>(4096.0f, std::max<float>(0.0f, fConst13 + -1.0f)));
		fConst16 = std::floor(0.192303f * fConst0 + 0.5f);
		fConst17 = 6.9077554f * (fConst16 / fConst0);
		fConst18 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst19 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst16 - fConst18)));
		iConst20 = static_cast<int>(std::min<float>(4096.0f, std::max<float>(0.0f, fConst18 + -1.0f)));
		fConst21 = std::floor(0.210389f * fConst0 + 0.5f);
		fConst22 = 6.9077554f * (fConst21 / fConst0);
		fConst23 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst24 = static_cast<int>(std::min<float>(32768.0f, std::max<float>(0.0f, fConst21 - fConst23)));
		iConst25 = static_cast<int>(std::min<float>(4096.0f, std::max<float>(0.0f, fConst23 + -1.0f)));
		fConst26 = std::floor(0.125f * fConst0 + 0.5f);
		fConst27 = 6.9077554f * (fConst26 / fConst0);
		fConst28 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst29 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst26 - fConst28)));
		iConst30 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst28 + -1.0f)));
		fConst31 = std::floor(0.127837f * fConst0 + 0.5f);
		fConst32 = 6.9077554f * (fConst31 / fConst0);
		fConst33 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst34 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst31 - fConst33)));
		iConst35 = static_cast<int>(std::min<float>(4096.0f, std::max<float>(0.0f, fConst33 + -1.0f)));
		fConst36 = std::floor(0.174713f * fConst0 + 0.5f);
		fConst37 = 6.9077554f * (fConst36 / fConst0);
		fConst38 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst39 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst36 - fConst38)));
		iConst40 = static_cast<int>(std::min<float>(4096.0f, std::max<float>(0.0f, fConst38 + -1.0f)));
		fConst41 = std::floor(0.153129f * fConst0 + 0.5f);
		fConst42 = 6.9077554f * (fConst41 / fConst0);
		fConst43 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst44 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst41 - fConst43)));
		iConst45 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst43 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider0 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider2 = static_cast<FAUSTFLOAT>(2.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(6e+03f);
		fHslider4 = static_cast<FAUSTFLOAT>(3.0f);
		fHslider5 = static_cast<FAUSTFLOAT>(2e+02f);
		fHslider6 = static_cast<FAUSTFLOAT>(12.0f);
		fHslider7 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider8 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider9 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider10 = static_cast<FAUSTFLOAT>(0.1f);
		fHslider11 = static_cast<FAUSTFLOAT>(1.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec12[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec11[l3] = 0.0f;
		}
		IOTA0 = 0;
		for (int l4 = 0; l4 < 131072; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec15[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec16[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec13[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 65536; l10 = l10 + 1) {
			fVec3[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4096; l11 = l11 + 1) {
			fVec4[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec9[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec20[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec19[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 131072; l15 = l15 + 1) {
			fVec5[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec22[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec21[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 65536; l18 = l18 + 1) {
			fVec6[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 8192; l19 = l19 + 1) {
			fVec7[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec26[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec25[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 131072; l23 = l23 + 1) {
			fVec8[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec28[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec27[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 32768; l26 = l26 + 1) {
			fVec9[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 8192; l27 = l27 + 1) {
			fVec10[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec23[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec32[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec31[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 131072; l31 = l31 + 1) {
			fVec11[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec34[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec33[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 65536; l34 = l34 + 1) {
			fVec12[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 8192; l35 = l35 + 1) {
			fVec13[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec29[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec38[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec37[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 131072; l39 = l39 + 1) {
			fVec14[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec40[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec39[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 32768; l42 = l42 + 1) {
			fVec15[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 4096; l43 = l43 + 1) {
			fVec16[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec35[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec44[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec43[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 131072; l47 = l47 + 1) {
			fVec17[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec46[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec45[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 32768; l50 = l50 + 1) {
			fVec18[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 8192; l51 = l51 + 1) {
			fVec19[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec41[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec50[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec49[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 131072; l55 = l55 + 1) {
			fVec20[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec52[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec51[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 32768; l58 = l58 + 1) {
			fVec21[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 8192; l59 = l59 + 1) {
			fVec22[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec47[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec56[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec55[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 131072; l63 = l63 + 1) {
			fVec23[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec58[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec57[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 32768; l66 = l66 + 1) {
			fVec24[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 4096; l67 = l67 + 1) {
			fVec25[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec53[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec1[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fRec2[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec3[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 3; l72 = l72 + 1) {
			fRec4[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 3; l73 = l73 + 1) {
			fRec5[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 3; l74 = l74 + 1) {
			fRec6[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec7[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fRec8[l76] = 0.0f;
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
	
	virtual fx_shimmer* clone() {
		return new fx_shimmer(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.shimmer");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addHorizontalSlider("control", &fHslider9, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.05f));
		ui_interface->declare(&fHslider3, "unit", "Hz");
		ui_interface->addHorizontalSlider("damp_hf", &fHslider3, FAUSTFLOAT(6e+03f), FAUSTFLOAT(1.5e+03f), FAUSTFLOAT(4.704e+04f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider4, "unit", "sec");
		ui_interface->addHorizontalSlider("decay_low", &fHslider4, FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider2, "unit", "sec");
		ui_interface->addHorizontalSlider("decay_mid", &fHslider2, FAUSTFLOAT(2.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("depth", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.05f));
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider11, "unit", "sec");
		ui_interface->addHorizontalSlider("envelope", &fHslider11, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(3.0f), FAUSTFLOAT(0.05f));
		ui_interface->declare(&fHslider5, "unit", "Hz");
		ui_interface->addHorizontalSlider("freq_low", &fHslider5, FAUSTFLOAT(2e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider7, "type", "int");
		ui_interface->addHorizontalSlider("mode", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(-3.0f), FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("ps_drywet", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.05f));
		ui_interface->declare(&fHslider6, "unit", "semitone");
		ui_interface->addHorizontalSlider("shift", &fHslider6, FAUSTFLOAT(12.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider10, "unit", "Hz");
		ui_interface->addHorizontalSlider("speed", &fHslider10, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.05f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = static_cast<int>(static_cast<float>(fCheckbox0));
		float fSlow1 = fConst1 * static_cast<float>(fHslider0);
		float fSlow2 = static_cast<float>(fHslider1);
		float fSlow3 = 1.0f - fSlow2;
		float fSlow4 = static_cast<float>(fHslider2);
		float fSlow5 = std::exp(-(fConst4 / fSlow4));
		float fSlow6 = std::cos(fConst5 * static_cast<float>(fHslider3));
		float fSlow7 = fx_shimmer_faustpower2_f(fSlow5);
		float fSlow8 = 1.0f - fSlow6 * fSlow7;
		float fSlow9 = 1.0f - fSlow7;
		float fSlow10 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow8) / fx_shimmer_faustpower2_f(fSlow9) + -1.0f));
		float fSlow11 = fSlow8 / fSlow9;
		float fSlow12 = fSlow5 * (fSlow10 + (1.0f - fSlow11));
		float fSlow13 = static_cast<float>(fHslider4);
		float fSlow14 = std::exp(-(fConst4 / fSlow13)) / fSlow5 + -1.0f;
		float fSlow15 = 1.0f / std::tan(fConst6 * static_cast<float>(fHslider5));
		float fSlow16 = 1.0f / (fSlow15 + 1.0f);
		float fSlow17 = 1.0f - fSlow15;
		float fSlow18 = fSlow11 - fSlow10;
		float fSlow19 = static_cast<float>(fHslider6);
		float fSlow20 = static_cast<float>(fHslider7);
		float fSlow21 = static_cast<float>(fSlow20 >= 0.0f);
		float fSlow22 = 0.6666667f * fSlow20;
		float fSlow23 = fSlow22 + 2.0f;
		float fSlow24 = fSlow21 * fSlow23;
		float fSlow25 = static_cast<float>(fSlow20 < 0.0f);
		float fSlow26 = fSlow20 + 2.0f;
		float fSlow27 = 0.083333336f * fSlow19 * (fSlow24 + fSlow25 * fSlow26);
		float fSlow28 = static_cast<float>(fHslider8);
		float fSlow29 = static_cast<float>(fHslider9);
		float fSlow30 = 0.5f * fSlow29;
		float fSlow31 = fConst7 * static_cast<float>(fHslider10);
		float fSlow32 = 6.0f * (1.0f - fSlow29);
		float fSlow33 = static_cast<float>(fHslider11);
		int iSlow34 = std::fabs(fSlow33) < 1.1920929e-07f;
		float fSlow35 = ((iSlow34) ? 0.0f : std::exp(-(fConst7 / ((iSlow34) ? 1.0f : fSlow33))));
		float fSlow36 = 1.0f - fSlow35;
		float fSlow37 = std::exp(-(fConst12 / fSlow4));
		float fSlow38 = fx_shimmer_faustpower2_f(fSlow37);
		float fSlow39 = 1.0f - fSlow6 * fSlow38;
		float fSlow40 = 1.0f - fSlow38;
		float fSlow41 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow39) / fx_shimmer_faustpower2_f(fSlow40) + -1.0f));
		float fSlow42 = fSlow39 / fSlow40;
		float fSlow43 = fSlow37 * (fSlow41 + (1.0f - fSlow42));
		float fSlow44 = std::exp(-(fConst12 / fSlow13)) / fSlow37 + -1.0f;
		float fSlow45 = fSlow42 - fSlow41;
		float fSlow46 = fSlow25 * (2.0f - fSlow20);
		float fSlow47 = 0.083333336f * fSlow19 * (fSlow46 + fSlow21 * (0.5f * fSlow20 + 2.0f));
		float fSlow48 = std::exp(-(fConst17 / fSlow4));
		float fSlow49 = fx_shimmer_faustpower2_f(fSlow48);
		float fSlow50 = 1.0f - fSlow6 * fSlow49;
		float fSlow51 = 1.0f - fSlow49;
		float fSlow52 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow50) / fx_shimmer_faustpower2_f(fSlow51) + -1.0f));
		float fSlow53 = fSlow50 / fSlow51;
		float fSlow54 = fSlow48 * (fSlow52 + (1.0f - fSlow53));
		float fSlow55 = std::exp(-(fConst17 / fSlow13)) / fSlow48 + -1.0f;
		float fSlow56 = fSlow53 - fSlow52;
		float fSlow57 = 0.083333336f * fSlow19 * (fSlow21 * (0.25f * fSlow20 + 2.0f) + fSlow25 * (2.0f - fSlow22));
		float fSlow58 = std::exp(-(fConst22 / fSlow4));
		float fSlow59 = fx_shimmer_faustpower2_f(fSlow58);
		float fSlow60 = 1.0f - fSlow6 * fSlow59;
		float fSlow61 = 1.0f - fSlow59;
		float fSlow62 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow60) / fx_shimmer_faustpower2_f(fSlow61) + -1.0f));
		float fSlow63 = fSlow60 / fSlow61;
		float fSlow64 = fSlow58 * (fSlow62 + (1.0f - fSlow63));
		float fSlow65 = std::exp(-(fConst22 / fSlow13)) / fSlow58 + -1.0f;
		float fSlow66 = fSlow63 - fSlow62;
		float fSlow67 = 0.083333336f * fSlow19 * (fSlow24 + fSlow46);
		float fSlow68 = std::exp(-(fConst27 / fSlow4));
		float fSlow69 = fx_shimmer_faustpower2_f(fSlow68);
		float fSlow70 = 1.0f - fSlow6 * fSlow69;
		float fSlow71 = 1.0f - fSlow69;
		float fSlow72 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow70) / fx_shimmer_faustpower2_f(fSlow71) + -1.0f));
		float fSlow73 = fSlow70 / fSlow71;
		float fSlow74 = fSlow68 * (fSlow72 + (1.0f - fSlow73));
		float fSlow75 = std::exp(-(fConst27 / fSlow13)) / fSlow68 + -1.0f;
		float fSlow76 = fSlow73 - fSlow72;
		float fSlow77 = fSlow21 * fSlow26;
		float fSlow78 = 0.083333336f * fSlow19 * (fSlow77 + fSlow25 * fSlow23);
		float fSlow79 = std::exp(-(fConst32 / fSlow4));
		float fSlow80 = fx_shimmer_faustpower2_f(fSlow79);
		float fSlow81 = 1.0f - fSlow6 * fSlow80;
		float fSlow82 = 1.0f - fSlow80;
		float fSlow83 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow81) / fx_shimmer_faustpower2_f(fSlow82) + -1.0f));
		float fSlow84 = fSlow81 / fSlow82;
		float fSlow85 = fSlow79 * (fSlow83 + (1.0f - fSlow84));
		float fSlow86 = std::exp(-(fConst32 / fSlow13)) / fSlow79 + -1.0f;
		float fSlow87 = fSlow84 - fSlow83;
		float fSlow88 = std::exp(-(fConst37 / fSlow4));
		float fSlow89 = fx_shimmer_faustpower2_f(fSlow88);
		float fSlow90 = 1.0f - fSlow6 * fSlow89;
		float fSlow91 = 1.0f - fSlow89;
		float fSlow92 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow90) / fx_shimmer_faustpower2_f(fSlow91) + -1.0f));
		float fSlow93 = fSlow90 / fSlow91;
		float fSlow94 = fSlow88 * (fSlow92 + (1.0f - fSlow93));
		float fSlow95 = std::exp(-(fConst37 / fSlow13)) / fSlow88 + -1.0f;
		float fSlow96 = fSlow93 - fSlow92;
		float fSlow97 = 0.083333336f * fSlow19 * (fSlow77 + fSlow25 * (0.33333334f * fSlow20 + 2.0f));
		float fSlow98 = std::exp(-(fConst42 / fSlow4));
		float fSlow99 = fx_shimmer_faustpower2_f(fSlow98);
		float fSlow100 = 1.0f - fSlow99 * fSlow6;
		float fSlow101 = 1.0f - fSlow99;
		float fSlow102 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow100) / fx_shimmer_faustpower2_f(fSlow101) + -1.0f));
		float fSlow103 = fSlow100 / fSlow101;
		float fSlow104 = fSlow98 * (fSlow102 + (1.0f - fSlow103));
		float fSlow105 = std::exp(-(fConst42 / fSlow13)) / fSlow98 + -1.0f;
		float fSlow106 = fSlow103 - fSlow102;
		float fSlow107 = 0.083333336f * fSlow19 * (fSlow77 + fSlow25 * (0.4f * fSlow20 + 2.0f));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec0[0] = fSlow1 + fConst2 * fRec0[1];
			float fTemp0 = 1.0f - fRec0[0];
			float fTemp1 = static_cast<float>(input0[i0]);
			float fTemp2 = ((iSlow0) ? 0.0f : fTemp1);
			fRec12[0] = -(fSlow16 * (fSlow17 * fRec12[1] - (fRec8[1] + fRec8[2])));
			fRec11[0] = fSlow12 * (fRec8[1] + fSlow14 * fRec12[0]) + fSlow18 * fRec11[1];
			float fTemp3 = 0.35355338f * fRec11[0] + 1e-20f;
			fVec1[IOTA0 & 131071] = fTemp3;
			float fTemp4 = ((1 - iVec0[1]) ? 0.0f : fSlow31 + fRec15[1]);
			fRec15[0] = fTemp4 - std::floor(fTemp4);
			float fTemp5 = fSlow30 * ftbl0fx_shimmerSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec15[0]), 65535))];
			float fTemp6 = std::fabs(fTemp3);
			fRec16[0] = std::max<float>(fTemp6, fSlow35 * fRec16[1] + fSlow36 * fTemp6);
			fRec13[0] = std::fmod(fRec13[1] + (2049.0f - std::pow(2.0f, fSlow27 * (fSlow28 * (fTemp5 + fSlow32 * fRec16[0]) + 0.5f))), 2048.0f);
			int iTemp7 = static_cast<int>(fRec13[0]);
			float fTemp8 = std::floor(fRec13[0]);
			float fTemp9 = std::min<float>(0.0009765625f * fRec13[0], 1.0f);
			float fTemp10 = fRec13[0] + 2048.0f;
			int iTemp11 = static_cast<int>(fTemp10);
			float fTemp12 = std::floor(fTemp10);
			fVec3[IOTA0 & 65535] = fSlow3 * fTemp3 + fSlow2 * ((fVec1[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp7))) & 131071] * (fTemp8 + (1.0f - fRec13[0])) + (fRec13[0] - fTemp8) * fVec1[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp7 + 1))) & 131071]) * fTemp9 + (fVec1[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp11))) & 131071] * (fTemp12 + (-2047.0f - fRec13[0])) + (fRec13[0] + (2048.0f - fTemp12)) * fVec1[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp11 + 1))) & 131071]) * (1.0f - fTemp9));
			float fTemp13 = static_cast<float>(input1[i0]);
			float fTemp14 = ((iSlow0) ? 0.0f : fTemp13);
			float fTemp15 = fVec3[(IOTA0 - iConst9) & 65535] + fTemp14 + 0.6f * fRec9[1];
			fVec4[IOTA0 & 4095] = fTemp15;
			fRec9[0] = fVec4[(IOTA0 - iConst10) & 4095];
			float fRec10 = -(0.6f * fTemp15);
			fRec20[0] = -(fSlow16 * (fSlow17 * fRec20[1] - (fRec4[1] + fRec4[2])));
			fRec19[0] = fSlow43 * (fRec4[1] + fSlow44 * fRec20[0]) + fSlow45 * fRec19[1];
			float fTemp16 = 0.35355338f * fRec19[0] + 1e-20f;
			fVec5[IOTA0 & 131071] = fTemp16;
			float fTemp17 = std::fabs(fTemp16);
			fRec22[0] = std::max<float>(fTemp17, fSlow35 * fRec22[1] + fSlow36 * fTemp17);
			fRec21[0] = std::fmod(fRec21[1] + (2049.0f - std::pow(2.0f, fSlow47 * (fSlow28 * (fTemp5 + fSlow32 * fRec22[0]) + 0.5f))), 2048.0f);
			int iTemp18 = static_cast<int>(fRec21[0]);
			float fTemp19 = std::floor(fRec21[0]);
			float fTemp20 = std::min<float>(0.0009765625f * fRec21[0], 1.0f);
			float fTemp21 = fRec21[0] + 2048.0f;
			int iTemp22 = static_cast<int>(fTemp21);
			float fTemp23 = std::floor(fTemp21);
			fVec6[IOTA0 & 65535] = fSlow3 * fTemp16 + fSlow2 * ((fVec5[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp18))) & 131071] * (fTemp19 + (1.0f - fRec21[0])) + (fRec21[0] - fTemp19) * fVec5[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp18 + 1))) & 131071]) * fTemp20 + (fVec5[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp22))) & 131071] * (fTemp23 + (-2047.0f - fRec21[0])) + (fRec21[0] + (2048.0f - fTemp23)) * fVec5[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp22 + 1))) & 131071]) * (1.0f - fTemp20));
			float fTemp24 = fVec6[(IOTA0 - iConst14) & 65535] + fTemp14 + 0.6f * fRec17[1];
			fVec7[IOTA0 & 8191] = fTemp24;
			fRec17[0] = fVec7[(IOTA0 - iConst15) & 8191];
			float fRec18 = -(0.6f * fTemp24);
			fRec26[0] = -(fSlow16 * (fSlow17 * fRec26[1] - (fRec6[1] + fRec6[2])));
			fRec25[0] = fSlow54 * (fRec6[1] + fSlow55 * fRec26[0]) + fSlow56 * fRec25[1];
			float fTemp25 = 0.35355338f * fRec25[0] + 1e-20f;
			fVec8[IOTA0 & 131071] = fTemp25;
			float fTemp26 = std::fabs(fTemp25);
			fRec28[0] = std::max<float>(fTemp26, fSlow35 * fRec28[1] + fSlow36 * fTemp26);
			fRec27[0] = std::fmod(fRec27[1] + (2049.0f - std::pow(2.0f, fSlow57 * (fSlow28 * (fTemp5 + fSlow32 * fRec28[0]) + 0.5f))), 2048.0f);
			int iTemp27 = static_cast<int>(fRec27[0]);
			float fTemp28 = std::floor(fRec27[0]);
			float fTemp29 = std::min<float>(0.0009765625f * fRec27[0], 1.0f);
			float fTemp30 = fRec27[0] + 2048.0f;
			int iTemp31 = static_cast<int>(fTemp30);
			float fTemp32 = std::floor(fTemp30);
			fVec9[IOTA0 & 32767] = fSlow3 * fTemp25 + fSlow2 * ((fVec8[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp27))) & 131071] * (fTemp28 + (1.0f - fRec27[0])) + (fRec27[0] - fTemp28) * fVec8[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp27 + 1))) & 131071]) * fTemp29 + (fVec8[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp31))) & 131071] * (fTemp32 + (-2047.0f - fRec27[0])) + (fRec27[0] + (2048.0f - fTemp32)) * fVec8[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp31 + 1))) & 131071]) * (1.0f - fTemp29));
			float fTemp33 = fVec9[(IOTA0 - iConst19) & 32767] + fTemp14 + 0.6f * fRec23[1];
			fVec10[IOTA0 & 8191] = fTemp33;
			fRec23[0] = fVec10[(IOTA0 - iConst20) & 8191];
			float fRec24 = -(0.6f * fTemp33);
			fRec32[0] = -(fSlow16 * (fSlow17 * fRec32[1] - (fRec2[1] + fRec2[2])));
			fRec31[0] = fSlow64 * (fRec2[1] + fSlow65 * fRec32[0]) + fSlow66 * fRec31[1];
			float fTemp34 = 0.35355338f * fRec31[0] + 1e-20f;
			fVec11[IOTA0 & 131071] = fTemp34;
			float fTemp35 = std::fabs(fTemp34);
			fRec34[0] = std::max<float>(fTemp35, fSlow35 * fRec34[1] + fSlow36 * fTemp35);
			fRec33[0] = std::fmod(fRec33[1] + (2049.0f - std::pow(2.0f, fSlow67 * (fSlow28 * (fTemp5 + fSlow32 * fRec34[0]) + 0.5f))), 2048.0f);
			int iTemp36 = static_cast<int>(fRec33[0]);
			float fTemp37 = std::floor(fRec33[0]);
			float fTemp38 = std::min<float>(0.0009765625f * fRec33[0], 1.0f);
			float fTemp39 = fRec33[0] + 2048.0f;
			int iTemp40 = static_cast<int>(fTemp39);
			float fTemp41 = std::floor(fTemp39);
			fVec12[IOTA0 & 65535] = fSlow3 * fTemp34 + fSlow2 * ((fVec11[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp36))) & 131071] * (fTemp37 + (1.0f - fRec33[0])) + (fRec33[0] - fTemp37) * fVec11[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp36 + 1))) & 131071]) * fTemp38 + (fVec11[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp40))) & 131071] * (fTemp41 + (-2047.0f - fRec33[0])) + (fRec33[0] + (2048.0f - fTemp41)) * fVec11[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp40 + 1))) & 131071]) * (1.0f - fTemp38));
			float fTemp42 = fVec12[(IOTA0 - iConst24) & 65535] + fTemp14 + 0.6f * fRec29[1];
			fVec13[IOTA0 & 8191] = fTemp42;
			fRec29[0] = fVec13[(IOTA0 - iConst25) & 8191];
			float fRec30 = -(0.6f * fTemp42);
			fRec38[0] = -(fSlow16 * (fSlow17 * fRec38[1] - (fRec7[1] + fRec7[2])));
			fRec37[0] = fSlow74 * (fRec7[1] + fSlow75 * fRec38[0]) + fSlow76 * fRec37[1];
			float fTemp43 = 0.35355338f * fRec37[0] + 1e-20f;
			fVec14[IOTA0 & 131071] = fTemp43;
			float fTemp44 = std::fabs(fTemp43);
			fRec40[0] = std::max<float>(fTemp44, fSlow35 * fRec40[1] + fSlow36 * fTemp44);
			fRec39[0] = std::fmod(fRec39[1] + (2049.0f - std::pow(2.0f, fSlow78 * (fSlow28 * (fTemp5 + fSlow32 * fRec40[0]) + 0.5f))), 2048.0f);
			int iTemp45 = static_cast<int>(fRec39[0]);
			float fTemp46 = std::floor(fRec39[0]);
			float fTemp47 = std::min<float>(0.0009765625f * fRec39[0], 1.0f);
			float fTemp48 = fRec39[0] + 2048.0f;
			int iTemp49 = static_cast<int>(fTemp48);
			float fTemp50 = std::floor(fTemp48);
			fVec15[IOTA0 & 32767] = fSlow3 * fTemp43 + fSlow2 * ((fVec14[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp45))) & 131071] * (fTemp46 + (1.0f - fRec39[0])) + (fRec39[0] - fTemp46) * fVec14[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp45 + 1))) & 131071]) * fTemp47 + (fVec14[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp49))) & 131071] * (fTemp50 + (-2047.0f - fRec39[0])) + (fRec39[0] + (2048.0f - fTemp50)) * fVec14[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp49 + 1))) & 131071]) * (1.0f - fTemp47));
			float fTemp51 = fTemp2 + fVec15[(IOTA0 - iConst29) & 32767] - 0.6f * fRec35[1];
			fVec16[IOTA0 & 4095] = fTemp51;
			fRec35[0] = fVec16[(IOTA0 - iConst30) & 4095];
			float fRec36 = 0.6f * fTemp51;
			fRec44[0] = -(fSlow16 * (fSlow17 * fRec44[1] - (fRec3[1] + fRec3[2])));
			fRec43[0] = fSlow85 * (fRec3[1] + fSlow86 * fRec44[0]) + fSlow87 * fRec43[1];
			float fTemp52 = 0.35355338f * fRec43[0] + 1e-20f;
			fVec17[IOTA0 & 131071] = fTemp52;
			float fTemp53 = std::fabs(fTemp52);
			fRec46[0] = std::max<float>(fTemp53, fSlow35 * fRec46[1] + fSlow36 * fTemp53);
			fRec45[0] = std::fmod(fRec45[1] + (2049.0f - std::pow(2.0f, fSlow78 * (fSlow28 * (fTemp5 + fSlow32 * fRec46[0]) + 0.5f))), 2048.0f);
			int iTemp54 = static_cast<int>(fRec45[0]);
			float fTemp55 = std::floor(fRec45[0]);
			float fTemp56 = std::min<float>(0.0009765625f * fRec45[0], 1.0f);
			float fTemp57 = fRec45[0] + 2048.0f;
			int iTemp58 = static_cast<int>(fTemp57);
			float fTemp59 = std::floor(fTemp57);
			fVec18[IOTA0 & 32767] = fSlow3 * fTemp52 + fSlow2 * ((fVec17[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp54))) & 131071] * (fTemp55 + (1.0f - fRec45[0])) + (fRec45[0] - fTemp55) * fVec17[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp54 + 1))) & 131071]) * fTemp56 + (fVec17[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp58))) & 131071] * (fTemp59 + (-2047.0f - fRec45[0])) + (fRec45[0] + (2048.0f - fTemp59)) * fVec17[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp58 + 1))) & 131071]) * (1.0f - fTemp56));
			float fTemp60 = fTemp2 + fVec18[(IOTA0 - iConst34) & 32767] - 0.6f * fRec41[1];
			fVec19[IOTA0 & 8191] = fTemp60;
			fRec41[0] = fVec19[(IOTA0 - iConst35) & 8191];
			float fRec42 = 0.6f * fTemp60;
			fRec50[0] = -(fSlow16 * (fSlow17 * fRec50[1] - (fRec5[1] + fRec5[2])));
			fRec49[0] = fSlow94 * (fRec5[1] + fSlow95 * fRec50[0]) + fSlow96 * fRec49[1];
			float fTemp61 = 0.35355338f * fRec49[0] + 1e-20f;
			fVec20[IOTA0 & 131071] = fTemp61;
			float fTemp62 = std::fabs(fTemp61);
			fRec52[0] = std::max<float>(fTemp62, fSlow35 * fRec52[1] + fSlow36 * fTemp62);
			fRec51[0] = std::fmod(fRec51[1] + (2049.0f - std::pow(2.0f, fSlow97 * (fSlow28 * (fTemp5 + fSlow32 * fRec52[0]) + 0.5f))), 2048.0f);
			int iTemp63 = static_cast<int>(fRec51[0]);
			float fTemp64 = std::floor(fRec51[0]);
			float fTemp65 = std::min<float>(0.0009765625f * fRec51[0], 1.0f);
			float fTemp66 = fRec51[0] + 2048.0f;
			int iTemp67 = static_cast<int>(fTemp66);
			float fTemp68 = std::floor(fTemp66);
			fVec21[IOTA0 & 32767] = fSlow3 * fTemp61 + fSlow2 * ((fVec20[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp63))) & 131071] * (fTemp64 + (1.0f - fRec51[0])) + (fRec51[0] - fTemp64) * fVec20[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp63 + 1))) & 131071]) * fTemp65 + (fVec20[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp67))) & 131071] * (fTemp68 + (-2047.0f - fRec51[0])) + (fRec51[0] + (2048.0f - fTemp68)) * fVec20[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp67 + 1))) & 131071]) * (1.0f - fTemp65));
			float fTemp69 = fTemp2 + fVec21[(IOTA0 - iConst39) & 32767] - 0.6f * fRec47[1];
			fVec22[IOTA0 & 8191] = fTemp69;
			fRec47[0] = fVec22[(IOTA0 - iConst40) & 8191];
			float fRec48 = 0.6f * fTemp69;
			fRec56[0] = -(fSlow16 * (fSlow17 * fRec56[1] - (fRec1[1] + fRec1[2])));
			fRec55[0] = fSlow104 * (fRec1[1] + fSlow105 * fRec56[0]) + fSlow106 * fRec55[1];
			float fTemp70 = 0.35355338f * fRec55[0] + 1e-20f;
			fVec23[IOTA0 & 131071] = fTemp70;
			float fTemp71 = std::fabs(fTemp70);
			fRec58[0] = std::max<float>(fTemp71, fRec58[1] * fSlow35 + fTemp71 * fSlow36);
			fRec57[0] = std::fmod(fRec57[1] + (2049.0f - std::pow(2.0f, fSlow107 * (fSlow28 * (fSlow32 * fRec58[0] + fTemp5) + 0.5f))), 2048.0f);
			int iTemp72 = static_cast<int>(fRec57[0]);
			float fTemp73 = std::floor(fRec57[0]);
			float fTemp74 = std::min<float>(0.0009765625f * fRec57[0], 1.0f);
			float fTemp75 = fRec57[0] + 2048.0f;
			int iTemp76 = static_cast<int>(fTemp75);
			float fTemp77 = std::floor(fTemp75);
			fVec24[IOTA0 & 32767] = fSlow3 * fTemp70 + fSlow2 * ((fVec23[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp72))) & 131071] * (fTemp73 + (1.0f - fRec57[0])) + (fRec57[0] - fTemp73) * fVec23[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp72 + 1))) & 131071]) * fTemp74 + (fVec23[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp76))) & 131071] * (fTemp77 + (-2047.0f - fRec57[0])) + (fRec57[0] + (2048.0f - fTemp77)) * fVec23[(IOTA0 - std::min<int>(65537, std::max<int>(0, iTemp76 + 1))) & 131071]) * (1.0f - fTemp74));
			float fTemp78 = fTemp2 + fVec24[(IOTA0 - iConst44) & 32767] - 0.6f * fRec53[1];
			fVec25[IOTA0 & 4095] = fTemp78;
			fRec53[0] = fVec25[(IOTA0 - iConst45) & 4095];
			float fRec54 = 0.6f * fTemp78;
			float fTemp79 = fRec54 + fRec48;
			float fTemp80 = fRec36 + fRec42 + fTemp79;
			fRec1[0] = fRec9[1] + fRec17[1] + fRec23[1] + fRec29[1] + fRec35[1] + fRec41[1] + fRec47[1] + fRec53[1] + fRec10 + fRec18 + fRec24 + fRec30 + fTemp80;
			fRec2[0] = fRec35[1] + fRec41[1] + fRec47[1] + fRec53[1] + fTemp80 - (fRec9[1] + fRec17[1] + fRec23[1] + fRec29[1] + fRec10 + fRec18 + fRec30 + fRec24);
			float fTemp81 = fRec42 + fRec36;
			fRec3[0] = fRec23[1] + fRec29[1] + fRec47[1] + fRec53[1] + fRec24 + fRec30 + fTemp79 - (fRec9[1] + fRec17[1] + fRec35[1] + fRec41[1] + fRec10 + fRec18 + fTemp81);
			fRec4[0] = fRec9[1] + fRec17[1] + fRec47[1] + fRec53[1] + fRec10 + fRec18 + fTemp79 - (fRec23[1] + fRec29[1] + fRec35[1] + fRec41[1] + fRec24 + fRec30 + fTemp81);
			float fTemp82 = fRec54 + fRec42;
			float fTemp83 = fRec48 + fRec36;
			fRec5[0] = fRec17[1] + fRec29[1] + fRec41[1] + fRec53[1] + fRec18 + fRec30 + fTemp82 - (fRec9[1] + fRec23[1] + fRec35[1] + fRec47[1] + fRec10 + fRec24 + fTemp83);
			fRec6[0] = fRec9[1] + fRec23[1] + fRec41[1] + fRec53[1] + fRec10 + fRec24 + fTemp82 - (fRec17[1] + fRec29[1] + fRec35[1] + fRec47[1] + fRec18 + fRec30 + fTemp83);
			float fTemp84 = fRec54 + fRec36;
			float fTemp85 = fRec48 + fRec42;
			fRec7[0] = fRec9[1] + fRec29[1] + fRec35[1] + fRec53[1] + fRec10 + fRec30 + fTemp84 - (fRec17[1] + fRec23[1] + fRec41[1] + fRec47[1] + fRec18 + fRec24 + fTemp85);
			fRec8[0] = fRec17[1] + fRec23[1] + fRec35[1] + fRec53[1] + fRec18 + fRec24 + fTemp84 - (fRec9[1] + fRec29[1] + fRec41[1] + fRec47[1] + fRec10 + fRec30 + fTemp85);
			output0[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp1 : fTemp0 * fTemp2 + fRec0[0] * (fRec1[0] + fRec3[0] + fRec5[0] + fRec7[0])));
			output1[i0] = static_cast<FAUSTFLOAT>(((iSlow0) ? fTemp13 : fTemp0 * fTemp14 + fRec0[0] * (fRec2[0] + fRec4[0] + fRec6[0] + fRec8[0])));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			IOTA0 = IOTA0 + 1;
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec13[1] = fRec13[0];
			fRec9[1] = fRec9[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec17[1] = fRec17[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec23[1] = fRec23[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec29[1] = fRec29[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec35[1] = fRec35[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec41[1] = fRec41[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec47[1] = fRec47[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec58[1] = fRec58[0];
			fRec57[1] = fRec57[0];
			fRec53[1] = fRec53[0];
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
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
		}
	}

};
#endif

class faust_fx_shimmer_tilde : public FaustExternal<fx_shimmer> {
public:
    faust_fx_shimmer_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(fx_shimmer))
    {
    }
};

#endif
