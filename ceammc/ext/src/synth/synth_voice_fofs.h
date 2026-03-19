/* ------------------------------------------------------------
name: "synth.voice_fofs"
Code generated with Faust 2.85.5 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -cn synth_voice_fofs -scn synth_voice_fofs_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_voice_fofs_H__
#define  __synth_voice_fofs_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_voice_fofs_dsp.h ********************************
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

class FAUST_API synth_voice_fofs_dsp {

    public:

        synth_voice_fofs_dsp() = default;
        virtual ~synth_voice_fofs_dsp() = default;

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
        virtual ::synth_voice_fofs_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public ::synth_voice_fofs_dsp {

    protected:

        ::synth_voice_fofs_dsp* fDSP;

    public:

        decorator_dsp(::synth_voice_fofs_dsp* synth_voice_fofs_dsp = nullptr):fDSP(synth_voice_fofs_dsp) {}
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
        virtual ::synth_voice_fofs_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_voice_fofs_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_voice_fofs_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_voice_fofs : public synth_voice_fofs_dsp {
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
#define FAUSTCLASS synth_voice_fofs
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

class synth_voice_fofsSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec4[2];
	int fSampleRate;
	
  public:
	
	int getNumInputssynth_voice_fofsSIG0() {
		return 0;
	}
	int getNumOutputssynth_voice_fofsSIG0() {
		return 1;
	}
	
	void instanceInitsynth_voice_fofsSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iVec1[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec4[l4] = 0;
		}
	}
	
	void fillsynth_voice_fofsSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[0] = 1;
			iRec4[0] = (iVec1[1] + iRec4[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * static_cast<float>(iRec4[0]));
			iVec1[1] = iVec1[0];
			iRec4[1] = iRec4[0];
		}
	}

};

static synth_voice_fofsSIG0* newsynth_voice_fofsSIG0() { return (synth_voice_fofsSIG0*)new synth_voice_fofsSIG0(); }
static void deletesynth_voice_fofsSIG0(synth_voice_fofsSIG0* dsp) { delete dsp; }

static float ftbl0synth_voice_fofsSIG0[65536];

class synth_voice_fofs : public synth_voice_fofs_dsp {
	
 private:
	
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fVslider0;
	float fConst3;
	float fRec3[2];
	FAUSTFLOAT fHslider0;
	float fRec2[2];
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fRec5[2];
	float fRec1[2];
	float fVec2[2];
	float fConst4;
	float fConst5;
	float fConst6;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fRec7[2];
	FAUSTFLOAT fHslider5;
	float fRec6[2];
	float fRec8[2];
	float fRec0[3];
	float fRec9[2];
	float fRec11[2];
	float fRec12[2];
	float fRec10[3];
	float fRec13[2];
	float fRec15[2];
	float fRec16[2];
	float fRec14[3];
	float fRec17[2];
	float fRec19[2];
	float fRec20[2];
	float fRec18[3];
	float fRec21[2];
	float fRec23[2];
	float fRec24[2];
	float fRec22[3];
	float fRec25[2];
	float fConst7;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fCheckbox0;
	float fVec3[2];
	float fRec26[2];
	FAUSTFLOAT fHslider7;
	int iRec27[2];
	
 public:
	synth_voice_fofs() {
	}
	
	synth_voice_fofs(const synth_voice_fofs&) = default;
	
	virtual ~synth_voice_fofs() = default;
	
	synth_voice_fofs& operator=(const synth_voice_fofs&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/listInterp:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -cn synth_voice_fofs -scn synth_voice_fofs_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("envelopes.lib/asr:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "1.3.0");
		m->declare("filename", "synth_voice_fofs.dsp");
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
		m->declare("name", "synth.voice_fofs");
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
		m->declare("physmodels.lib/fofSmooth:author", "Mike Olsen");
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
		synth_voice_fofsSIG0* sig0 = newsynth_voice_fofsSIG0();
		sig0->instanceInitsynth_voice_fofsSIG0(sample_rate);
		sig0->fillsynth_voice_fofsSIG0(65536, ftbl0synth_voice_fofsSIG0);
		deletesynth_voice_fofsSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 3.1415927f / fConst0;
		fConst5 = std::exp(-(1e+02f / fConst0));
		fConst6 = 1.0f - fConst5;
		fConst7 = 0.001f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = static_cast<FAUSTFLOAT>(4.4e+02f);
		fHslider0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider2 = static_cast<FAUSTFLOAT>(6.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(1e+01f);
		fHslider5 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider6 = static_cast<FAUSTFLOAT>(3e+01f);
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider7 = static_cast<FAUSTFLOAT>(5e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec3[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec2[l2] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec5[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec2[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec6[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec0[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec9[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fRec10[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec13[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec16[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = l19 + 1) {
			fRec14[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec19[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec20[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec18[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec21[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec23[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec24[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec22[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec25[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fVec3[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec26[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			iRec27[l31] = 0;
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
	
	virtual synth_voice_fofs* clone() {
		return new synth_voice_fofs(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.voice_fofs");
		ui_interface->declare(&fHslider6, "unit", "ms");
		ui_interface->addHorizontalSlider("attack", &fHslider6, FAUSTFLOAT(3e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("freq", &fVslider0, FAUSTFLOAT(4.4e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("fvowel", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->addHorizontalSlider("ivoice", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider0, "unit", "ms");
		ui_interface->addHorizontalSlider("portamento", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4e+03f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider7, "unit", "ms");
		ui_interface->addHorizontalSlider("release", &fHslider7, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider2, "unit", "Hz");
		ui_interface->addHorizontalSlider("vibr_freq", &fHslider2, FAUSTFLOAT(6.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("vibr_gain", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider4, "unit", "ms");
		ui_interface->addHorizontalSlider("wsmooth", &fHslider4, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = fConst2 * static_cast<float>(fVslider0);
		float fSlow1 = 0.001f * static_cast<float>(fHslider0);
		int iSlow2 = std::fabs(fSlow1) < 1.1920929e-07f;
		float fSlow3 = ((iSlow2) ? 0.0f : std::exp(-(fConst1 / ((iSlow2) ? 1.0f : fSlow1))));
		float fSlow4 = 1.0f - fSlow3;
		float fSlow5 = 0.1f * static_cast<float>(fHslider1);
		float fSlow6 = fConst1 * static_cast<float>(fHslider2);
		float fSlow7 = 0.001f * static_cast<float>(fHslider4);
		int iSlow8 = std::fabs(fSlow7) < 1.1920929e-07f;
		float fSlow9 = ((iSlow8) ? 0.0f : std::exp(-(fConst1 / ((iSlow8) ? 1.0f : fSlow7))));
		float fSlow10 = static_cast<float>(fHslider3) * (1.0f - fSlow9);
		int iSlow11 = static_cast<int>(static_cast<float>(fHslider5));
		int iSlow12 = ((iSlow11 == 0) ? 1 : ((iSlow11 == 3) ? 1 : 0));
		float fSlow13 = static_cast<float>(5 * iSlow12);
		int iSlow14 = iSlow12 >= 1;
		float fSlow15 = ((iSlow14) ? 174.61f : 82.41f);
		float fSlow16 = ((iSlow14) ? 1046.5f : 523.25f);
		float fSlow17 = fSlow16 - fSlow15;
		float fSlow18 = static_cast<float>(5 * iSlow11);
		int iSlow19 = iSlow12 == 0;
		int iSlow20 = iSlow11 != 2;
		float fSlow21 = 1.0f / std::max<float>(1.0f, fConst7 * static_cast<float>(fHslider6));
		float fSlow22 = static_cast<float>(fCheckbox0);
		float fSlow23 = 1.0f / std::max<float>(1.0f, fConst7 * static_cast<float>(fHslider7));
		int iSlow24 = fSlow22 == 0.0f;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			int iTemp0 = 1 - iVec0[1];
			fRec3[0] = fSlow0 + fConst3 * fRec3[1];
			fRec2[0] = fRec3[0] * fSlow4 + fSlow3 * fRec2[1];
			float fTemp1 = ((iTemp0) ? 0.0f : fSlow6 + fRec5[1]);
			fRec5[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = fRec2[0] * (fSlow5 * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec5[0]), 65535))] + 1.0f);
			float fTemp3 = ((iTemp0) ? 0.0f : fRec1[1] + fConst1 * fTemp2);
			fRec1[0] = fTemp3 - std::floor(fTemp3);
			float fTemp4 = static_cast<float>((fRec1[0] - fRec1[1]) < 0.0f);
			fVec2[0] = fTemp4;
			fRec7[0] = fSlow10 + fSlow9 * fRec7[1];
			float fTemp5 = fRec7[0] + fSlow13;
			int iTemp6 = fTemp5 >= 5.0f;
			int iTemp7 = fTemp5 >= 3.0f;
			int iTemp8 = fTemp5 >= 2.0f;
			int iTemp9 = fTemp5 >= 1.0f;
			int iTemp10 = fTemp5 >= 4.0f;
			int iTemp11 = fTemp5 >= 8.0f;
			int iTemp12 = fTemp5 >= 7.0f;
			int iTemp13 = fTemp5 >= 6.0f;
			float fTemp14 = ((iTemp6) ? ((iTemp11) ? 2.0f : ((iTemp12) ? 3.0f : ((iTemp13) ? 3.0f : 2.0f))) : ((iTemp7) ? ((iTemp10) ? 1.5f : 1.0f) : ((iTemp8) ? 1.25f : ((iTemp9) ? 1.25f : 1.0f))));
			float fTemp15 = fTemp14 + (((iTemp6) ? ((iTemp11) ? 12.0f : ((iTemp12) ? 12.0f : ((iTemp13) ? 12.0f : 15.0f))) : ((iTemp7) ? ((iTemp10) ? 4.0f : 1e+01f) : ((iTemp8) ? 2.5f : ((iTemp9) ? 2.5f : 1e+01f)))) - fTemp14) * ((fTemp2 <= fSlow15) ? 0.0f : ((fTemp2 >= fSlow16) ? 1.0f : (fTemp2 - fSlow15) / fSlow17));
			float fTemp16 = fSlow18 + fRec7[0];
			int iTemp17 = fTemp16 < 23.0f;
			int iTemp18 = fTemp16 < 24.0f;
			float fTemp19 = fSlow18 + fRec7[0] + -23.0f;
			int iTemp20 = fTemp16 < 22.0f;
			float fTemp21 = fSlow18 + fRec7[0] + -22.0f;
			int iTemp22 = fTemp16 < 21.0f;
			float fTemp23 = fSlow18 + fRec7[0] + -21.0f;
			int iTemp24 = fTemp16 < 2e+01f;
			float fTemp25 = fSlow18 + fRec7[0] + -2e+01f;
			float fTemp26 = 2e+01f * fTemp25;
			int iTemp27 = fTemp16 < 19.0f;
			int iTemp28 = fTemp16 < 18.0f;
			float fTemp29 = fSlow18 + fRec7[0] + -18.0f;
			int iTemp30 = fTemp16 < 17.0f;
			float fTemp31 = fSlow18 + fRec7[0] + -17.0f;
			int iTemp32 = fTemp16 < 16.0f;
			int iTemp33 = fTemp16 < 15.0f;
			float fTemp34 = fSlow18 + fRec7[0] + -15.0f;
			float fTemp35 = 2e+01f * fTemp34;
			int iTemp36 = fTemp16 < 14.0f;
			float fTemp37 = fSlow18 + fRec7[0] + -14.0f;
			int iTemp38 = fTemp16 < 13.0f;
			int iTemp39 = fTemp16 < 12.0f;
			int iTemp40 = fTemp16 < 11.0f;
			float fTemp41 = fSlow18 + fRec7[0] + -11.0f;
			int iTemp42 = fTemp16 < 1e+01f;
			float fTemp43 = fSlow18 + fRec7[0] + -1e+01f;
			float fTemp44 = 1e+01f * fTemp43;
			int iTemp45 = fTemp16 < 9.0f;
			float fTemp46 = fSlow18 + fRec7[0] + -9.0f;
			int iTemp47 = fTemp16 < 8.0f;
			int iTemp48 = fTemp16 < 7.0f;
			float fTemp49 = fSlow18 + fRec7[0] + -7.0f;
			int iTemp50 = fTemp16 < 6.0f;
			float fTemp51 = fSlow18 + fRec7[0] + -6.0f;
			int iTemp52 = fTemp16 < 5.0f;
			float fTemp53 = fSlow18 + fRec7[0] + -5.0f;
			int iTemp54 = fTemp16 < 4.0f;
			float fTemp55 = fSlow18 + fRec7[0] + -4.0f;
			float fTemp56 = 1e+01f * fTemp55;
			int iTemp57 = fTemp16 < 3.0f;
			float fTemp58 = fSlow18 + fRec7[0] + -3.0f;
			float fTemp59 = 2e+01f * fTemp58;
			int iTemp60 = fTemp16 < 2.0f;
			float fTemp61 = fSlow18 + fRec7[0] + -2.0f;
			float fTemp62 = 2e+01f * fTemp61;
			int iTemp63 = fTemp16 < 1.0f;
			float fTemp64 = fSlow18 + fRec7[0] + -1.0f;
			int iTemp65 = fTemp16 < 0.0f;
			float fTemp66 = 2e+01f * fTemp16;
			float fTemp67 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 8e+01f : ((iTemp63) ? 8e+01f - fTemp66 : 6e+01f)) : ((iTemp60) ? 6e+01f - 1e+01f * fTemp64 : 5e+01f)) : ((iTemp57) ? fTemp62 + 5e+01f : 7e+01f)) : ((iTemp54) ? 7e+01f - fTemp59 : 5e+01f)) : ((iTemp52) ? fTemp56 + 5e+01f : 6e+01f)) : ((iTemp50) ? 6e+01f - 2e+01f * fTemp53 : 4e+01f)) : ((iTemp48) ? 2e+01f * fTemp51 + 4e+01f : 6e+01f)) : ((iTemp47) ? 6e+01f - 2e+01f * fTemp49 : 4e+01f)) : 4e+01f) : ((iTemp42) ? 4e+01f * (fTemp46 + 1.0f) : 8e+01f)) : ((iTemp40) ? 8e+01f - fTemp44 : 7e+01f)) : ((iTemp39) ? 7e+01f - 3e+01f * fTemp41 : 4e+01f)) : 4e+01f) : 4e+01f) : ((iTemp33) ? 4e+01f * (fTemp37 + 1.0f) : 8e+01f)) : ((iTemp32) ? 8e+01f - fTemp35 : 6e+01f)) : 6e+01f) : ((iTemp28) ? 6e+01f - 2e+01f * fTemp31 : 4e+01f)) : ((iTemp27) ? 1e+01f * fTemp29 + 4e+01f : 5e+01f)) : 5e+01f) : ((iTemp22) ? fTemp26 + 5e+01f : 7e+01f)) : ((iTemp20) ? 7e+01f - 3e+01f * fTemp23 : 4e+01f)) : ((iTemp17) ? 3e+01f * fTemp21 + 4e+01f : 7e+01f)) : ((iTemp18) ? 7e+01f - 3e+01f * fTemp19 : 4e+01f));
			fRec6[0] = fConst6 * fTemp15 * fTemp67 + fConst5 * fRec6[1];
			float fTemp68 = std::exp(-(fConst4 * fRec6[0]));
			fRec8[0] = fConst6 * fTemp67 + fConst5 * fRec8[1];
			float fTemp69 = std::exp(-(fConst4 * fRec8[0]));
			fRec0[0] = fVec2[1] + fRec0[1] * (fTemp68 + fTemp69) - fTemp68 * fTemp69 * fRec0[2];
			float fTemp70 = 0.00084f * (1e+03f - fTemp2) + 0.8f;
			float fTemp71 = 0.0036666666f * (4e+02f - fTemp2) + 3.0f;
			int iTemp72 = iTemp0 | static_cast<int>(fTemp4);
			float fTemp73 = fSlow18 + fRec7[0] + -19.0f;
			float fTemp74 = fSlow18 + fRec7[0] + -16.0f;
			float fTemp75 = 8e+01f * fTemp74;
			float fTemp76 = fSlow18 + fRec7[0] + -13.0f;
			float fTemp77 = fSlow18 + fRec7[0] + -12.0f;
			float fTemp78 = fSlow18 + fRec7[0] + -8.0f;
			float fTemp79 = 5e+01f * fTemp78;
			float fTemp80 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 8e+02f : ((iTemp63) ? 8e+02f - 4e+02f * fTemp16 : 4e+02f)) : ((iTemp60) ? 4e+02f - 5e+01f * fTemp64 : 3.5e+02f)) : ((iTemp57) ? 1e+02f * fTemp61 + 3.5e+02f : 4.5e+02f)) : ((iTemp54) ? 4.5e+02f - 125.0f * fTemp58 : 325.0f)) : ((iTemp52) ? 275.0f * fTemp55 + 325.0f : 6e+02f)) : ((iTemp50) ? 6e+02f - 2e+02f * fTemp53 : 4e+02f)) : ((iTemp48) ? 4e+02f - 1.5e+02f * fTemp51 : 2.5e+02f)) : ((iTemp47) ? 1.5e+02f * fTemp49 + 2.5e+02f : 4e+02f)) : ((iTemp45) ? 4e+02f - fTemp79 : 3.5e+02f)) : ((iTemp42) ? 3.1e+02f * fTemp46 + 3.5e+02f : 6.6e+02f)) : ((iTemp40) ? 6.6e+02f - 2.2e+02f * fTemp43 : 4.4e+02f)) : ((iTemp39) ? 4.4e+02f - 1.7e+02f * fTemp41 : 2.7e+02f)) : ((iTemp38) ? 1.6e+02f * fTemp77 + 2.7e+02f : 4.3e+02f)) : ((iTemp36) ? 4.3e+02f - 6e+01f * fTemp76 : 3.7e+02f)) : ((iTemp33) ? 4.3e+02f * fTemp37 + 3.7e+02f : 8e+02f)) : ((iTemp32) ? 8e+02f - 4.5e+02f * fTemp34 : 3.5e+02f)) : ((iTemp30) ? 3.5e+02f - fTemp75 : 2.7e+02f)) : ((iTemp28) ? 1.8e+02f * fTemp31 + 2.7e+02f : 4.5e+02f)) : ((iTemp27) ? 4.5e+02f - 125.0f * fTemp29 : 325.0f)) : ((iTemp24) ? 325.0f * (fTemp73 + 1.0f) : 6.5e+02f)) : ((iTemp22) ? 6.5e+02f - 2.5e+02f * fTemp25 : 4e+02f)) : ((iTemp20) ? 4e+02f - 1.1e+02f * fTemp23 : 2.9e+02f)) : ((iTemp17) ? 1.1e+02f * fTemp21 + 2.9e+02f : 4e+02f)) : ((iTemp18) ? 4e+02f - 5e+01f * fTemp19 : 3.5e+02f));
			float fTemp81 = ((iTemp72) ? 0.0f : fRec9[1] + fConst1 * ((fTemp80 <= fTemp2) ? fTemp2 : fTemp80));
			fRec9[0] = fTemp81 - std::floor(fTemp81);
			float fTemp82 = 1e+01f * fTemp21;
			float fTemp83 = 1e+01f * fTemp25;
			float fTemp84 = 1e+01f * fTemp46;
			float fTemp85 = 1e+01f * fTemp53;
			float fTemp86 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 9e+01f : ((iTemp63) ? 9e+01f - 1e+01f * fTemp16 : 8e+01f)) : ((iTemp60) ? 2e+01f * fTemp64 + 8e+01f : 1e+02f)) : ((iTemp57) ? 1e+02f - fTemp62 : 8e+01f)) : ((iTemp54) ? 8e+01f - fTemp59 : 6e+01f)) : ((iTemp52) ? fTemp56 + 6e+01f : 7e+01f)) : ((iTemp50) ? fTemp85 + 7e+01f : 8e+01f)) : ((iTemp48) ? 1e+01f * fTemp51 + 8e+01f : 9e+01f)) : ((iTemp47) ? 9e+01f - 1e+01f * fTemp49 : 8e+01f)) : 8e+01f) : ((iTemp42) ? fTemp84 + 8e+01f : 9e+01f)) : ((iTemp40) ? 9e+01f - fTemp44 : 8e+01f)) : ((iTemp39) ? 1e+01f * fTemp41 + 8e+01f : 9e+01f)) : ((iTemp38) ? 9e+01f - 1e+01f * fTemp77 : 8e+01f)) : ((iTemp36) ? 8e+01f - 2e+01f * fTemp76 : 6e+01f)) : ((iTemp33) ? 3e+01f * fTemp37 + 6e+01f : 9e+01f)) : ((iTemp32) ? 1e+01f * fTemp34 + 9e+01f : 1e+02f)) : ((iTemp30) ? 1e+02f - 1e+01f * fTemp74 : 9e+01f)) : ((iTemp28) ? 9e+01f - 1e+01f * fTemp31 : 8e+01f)) : ((iTemp27) ? 8e+01f - 2e+01f * fTemp29 : 6e+01f)) : ((iTemp24) ? 3e+01f * fTemp73 + 6e+01f : 9e+01f)) : ((iTemp22) ? 9e+01f - fTemp83 : 8e+01f)) : ((iTemp20) ? 1e+01f * fTemp23 + 8e+01f : 9e+01f)) : ((iTemp17) ? 9e+01f - fTemp82 : 8e+01f)) : ((iTemp18) ? 8e+01f - 2e+01f * fTemp19 : 6e+01f));
			fRec11[0] = fConst6 * fTemp15 * fTemp86 + fConst5 * fRec11[1];
			float fTemp87 = std::exp(-(fConst4 * fRec11[0]));
			fRec12[0] = fConst6 * fTemp86 + fConst5 * fRec12[1];
			float fTemp88 = std::exp(-(fConst4 * fRec12[0]));
			fRec10[0] = fVec2[1] + fRec10[1] * (fTemp87 + fTemp88) - fTemp87 * fTemp88 * fRec10[2];
			float fTemp89 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 0.630957f : ((iTemp63) ? 0.630957f - 0.567861f * fTemp16 : 0.063096f)) : ((iTemp60) ? 0.036904f * fTemp64 + 0.063096f : 0.1f)) : ((iTemp57) ? 0.254813f * fTemp61 + 0.1f : 0.354813f)) : ((iTemp54) ? 0.354813f - 0.103624f * fTemp58 : 0.251189f)) : ((iTemp52) ? 0.195495f * fTemp55 + 0.251189f : 0.446684f)) : ((iTemp50) ? 0.446684f - 0.195495f * fTemp53 : 0.251189f)) : ((iTemp48) ? 0.251189f - 0.219566f * fTemp51 : 0.031623f)) : ((iTemp47) ? 0.250215f * fTemp49 + 0.031623f : 0.281838f)) : ((iTemp45) ? 0.281838f - 0.181838f * fTemp78 : 0.1f)) : ((iTemp42) ? 0.401187f * fTemp46 + 0.1f : 0.501187f)) : ((iTemp40) ? 0.501187f - 0.301661f * fTemp43 : 0.199526f)) : ((iTemp39) ? 0.199526f - 0.13643f * fTemp41 : 0.063096f)) : ((iTemp38) ? 0.253132f * fTemp77 + 0.063096f : 0.316228f)) : ((iTemp36) ? 0.316228f - 0.216228f * fTemp76 : 0.1f)) : ((iTemp33) ? 0.401187f * fTemp37 + 0.1f : 0.501187f)) : ((iTemp32) ? 0.501187f - 0.401187f * fTemp34 : 0.1f)) : ((iTemp30) ? 0.151189f * fTemp74 + 0.1f : 0.251189f)) : ((iTemp28) ? 0.030649f * fTemp31 + 0.251189f : 0.281838f)) : ((iTemp27) ? 0.281838f - 0.123349f * fTemp29 : 0.158489f)) : ((iTemp24) ? 0.342698f * fTemp73 + 0.158489f : 0.501187f)) : ((iTemp22) ? 0.501187f - 0.301661f * fTemp25 : 0.199526f)) : ((iTemp20) ? 0.199526f - 0.021698f * fTemp23 : 0.177828f)) : ((iTemp17) ? 0.1384f * fTemp21 + 0.177828f : 0.316228f)) : ((iTemp18) ? 0.316228f - 0.216228f * fTemp19 : 0.1f));
			float fTemp90 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 1.15e+03f : ((iTemp63) ? 4.5e+02f * fTemp16 + 1.15e+03f : 1.6e+03f)) : ((iTemp60) ? 1e+02f * fTemp64 + 1.6e+03f : 1.7e+03f)) : ((iTemp57) ? 1.7e+03f - 9e+02f * fTemp61 : 8e+02f)) : ((iTemp54) ? 8e+02f - 1e+02f * fTemp58 : 7e+02f)) : ((iTemp52) ? 3.4e+02f * fTemp55 + 7e+02f : 1.04e+03f)) : ((iTemp50) ? 5.8e+02f * fTemp53 + 1.04e+03f : 1.62e+03f)) : ((iTemp48) ? 1.3e+02f * fTemp51 + 1.62e+03f : 1.75e+03f)) : ((iTemp47) ? 1.75e+03f - 1e+03f * fTemp49 : 7.5e+02f)) : ((iTemp45) ? 7.5e+02f - 1.5e+02f * fTemp78 : 6e+02f)) : ((iTemp42) ? 5.2e+02f * fTemp46 + 6e+02f : 1.12e+03f)) : ((iTemp40) ? 6.8e+02f * fTemp43 + 1.12e+03f : 1.8e+03f)) : ((iTemp39) ? 5e+01f * fTemp41 + 1.8e+03f : 1.85e+03f)) : ((iTemp38) ? 1.85e+03f - 1.03e+03f * fTemp77 : 8.2e+02f)) : ((iTemp36) ? 8.2e+02f - 1.9e+02f * fTemp76 : 6.3e+02f)) : ((iTemp33) ? 5.2e+02f * fTemp37 + 6.3e+02f : 1.15e+03f)) : ((iTemp32) ? 8.5e+02f * fTemp34 + 1.15e+03f : 2e+03f)) : ((iTemp30) ? 1.4e+02f * fTemp74 + 2e+03f : 2.14e+03f)) : ((iTemp28) ? 2.14e+03f - 1.34e+03f * fTemp31 : 8e+02f)) : ((iTemp27) ? 8e+02f - 1e+02f * fTemp29 : 7e+02f)) : ((iTemp24) ? 3.8e+02f * fTemp73 + 7e+02f : 1.08e+03f)) : ((iTemp22) ? 6.2e+02f * fTemp25 + 1.08e+03f : 1.7e+03f)) : ((iTemp20) ? 1.7e+02f * fTemp23 + 1.7e+03f : 1.87e+03f)) : ((iTemp17) ? 1.87e+03f - 1.07e+03f * fTemp21 : 8e+02f)) : ((iTemp18) ? 8e+02f - 2e+02f * fTemp19 : 6e+02f));
			float fTemp91 = 2.0f * fTemp2 + 3e+01f;
			float fTemp92 = ((iTemp72) ? 0.0f : fRec13[1] + fConst1 * ((iSlow20) ? (((fTemp90 >= 1.3e+03f) & (fTemp2 >= 2e+02f)) ? fTemp90 - 0.00095238094f * (fTemp2 + -2e+02f) * (fTemp90 + -1.3e+03f) : ((fTemp90 <= fTemp91) ? fTemp91 : fTemp90)) : fTemp90));
			fRec13[0] = fTemp92 - std::floor(fTemp92);
			float fTemp93 = 5e+01f * fTemp73;
			float fTemp94 = 2e+01f * fTemp37;
			float fTemp95 = 2e+01f * fTemp43;
			float fTemp96 = 2e+01f * fTemp46;
			float fTemp97 = 6e+01f * fTemp55;
			float fTemp98 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? 1.2e+02f : ((iTemp57) ? 1.2e+02f - fTemp62 : 1e+02f)) : ((iTemp54) ? 7e+01f * fTemp58 + 1e+02f : 1.7e+02f)) : ((iTemp52) ? 1.7e+02f - fTemp97 : 1.1e+02f)) : ((iTemp50) ? 1.1e+02f - fTemp85 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp42) ? fTemp96 + 1e+02f : 1.2e+02f)) : ((iTemp40) ? 1.2e+02f - fTemp95 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp33) ? fTemp94 + 1e+02f : 1.2e+02f)) : 1.2e+02f) : ((iTemp30) ? 1.2e+02f - 2e+01f * fTemp74 : 1e+02f)) : 1e+02f) : ((iTemp27) ? 7e+01f * fTemp29 + 1e+02f : 1.7e+02f)) : ((iTemp24) ? 1.7e+02f - fTemp93 : 1.2e+02f)) : ((iTemp22) ? 1.2e+02f - fTemp26 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f);
			fRec15[0] = fConst6 * fTemp15 * fTemp98 + fConst5 * fRec15[1];
			float fTemp99 = std::exp(-(fConst4 * fRec15[0]));
			fRec16[0] = fConst6 * fTemp98 + fConst5 * fRec16[1];
			float fTemp100 = std::exp(-(fConst4 * fRec16[0]));
			fRec14[0] = fVec2[1] + fRec14[1] * (fTemp99 + fTemp100) - fTemp99 * fTemp100 * fRec14[2];
			float fTemp101 = ((iTemp28) ? 0.029314f * fTemp31 + 0.050119f : 0.079433f);
			float fTemp102 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 0.1f : ((iTemp63) ? 0.1f - 0.068377f * fTemp16 : 0.031623f)) : 0.031623f) : ((iTemp57) ? 0.126866f * fTemp61 + 0.031623f : 0.158489f)) : ((iTemp54) ? 0.158489f - 0.126866f * fTemp58 : 0.031623f)) : ((iTemp52) ? 0.32319f * fTemp55 + 0.031623f : 0.354813f)) : 0.354813f) : ((iTemp48) ? 0.354813f - 0.196324f * fTemp51 : 0.158489f)) : ((iTemp47) ? 0.158489f - 0.069364f * fTemp49 : 0.089125f)) : ((iTemp45) ? 0.089125f - 0.064006f * fTemp78 : 0.025119f)) : ((iTemp42) ? 0.045676f * fTemp46 + 0.025119f : 0.070795f)) : ((iTemp40) ? 0.055098f * fTemp43 + 0.070795f : 0.125893f)) : ((iTemp39) ? 0.125893f - 0.062797f * fTemp41 : 0.063096f)) : ((iTemp38) ? 0.063096f - 0.012977f * fTemp77 : 0.050119f)) : ((iTemp36) ? 0.020676f * fTemp76 + 0.050119f : 0.070795f)) : ((iTemp33) ? 0.070795f - 0.045676f * fTemp37 : 0.025119f)) : ((iTemp32) ? 0.152709f * fTemp34 + 0.025119f : 0.177828f)) : ((iTemp30) ? 0.177828f - 0.127709f * fTemp74 : 0.050119f)) : fTemp101) : ((iTemp27) ? 0.079433f - 0.06165f * fTemp29 : 0.017783f)) : ((iTemp24) ? 0.428901f * fTemp73 + 0.017783f : 0.446684f)) : ((iTemp22) ? 0.446684f - 0.195495f * fTemp25 : 0.251189f)) : ((iTemp20) ? 0.251189f - 0.125296f * fTemp23 : 0.125893f)) : ((iTemp17) ? 0.125296f * fTemp21 + 0.125893f : 0.251189f)) : ((iTemp18) ? 0.251189f - 0.109935f * fTemp19 : 0.141254f));
			float fTemp103 = 1e+02f * fTemp19;
			float fTemp104 = 5e+01f * fTemp43;
			float fTemp105 = ((iTemp72) ? 0.0f : fRec17[1] + fConst1 * ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 2.8e+03f : ((iTemp63) ? 2.8e+03f - 1e+02f * fTemp16 : 2.7e+03f)) : 2.7e+03f) : ((iTemp57) ? 1.3e+02f * fTemp61 + 2.7e+03f : 2.83e+03f)) : ((iTemp54) ? 2.83e+03f - 3e+02f * fTemp58 : 2.53e+03f)) : ((iTemp52) ? 2.53e+03f - 2.8e+02f * fTemp55 : 2.25e+03f)) : ((iTemp50) ? 1.5e+02f * fTemp53 + 2.25e+03f : 2.4e+03f)) : ((iTemp48) ? 2e+02f * fTemp51 + 2.4e+03f : 2.6e+03f)) : ((iTemp47) ? 2.6e+03f - 2e+02f * fTemp49 : 2.4e+03f)) : 2.4e+03f) : ((iTemp42) ? 3.5e+02f * fTemp46 + 2.4e+03f : 2.75e+03f)) : ((iTemp40) ? 2.75e+03f - fTemp104 : 2.7e+03f)) : ((iTemp39) ? 2e+02f * fTemp41 + 2.7e+03f : 2.9e+03f)) : ((iTemp38) ? 2.9e+03f - 2e+02f * fTemp77 : 2.7e+03f)) : ((iTemp36) ? 5e+01f * fTemp76 + 2.7e+03f : 2.75e+03f)) : ((iTemp33) ? 1.5e+02f * fTemp37 + 2.75e+03f : 2.9e+03f)) : ((iTemp32) ? 2.9e+03f - 1e+02f * fTemp34 : 2.8e+03f)) : ((iTemp30) ? 1.5e+02f * fTemp74 + 2.8e+03f : 2.95e+03f)) : ((iTemp28) ? 2.95e+03f - 1.2e+02f * fTemp31 : 2.83e+03f)) : ((iTemp27) ? 2.83e+03f - 1.3e+02f * fTemp29 : 2.7e+03f)) : ((iTemp24) ? 2.7e+03f - fTemp93 : 2.65e+03f)) : ((iTemp22) ? 2.65e+03f - 5e+01f * fTemp25 : 2.6e+03f)) : ((iTemp20) ? 2e+02f * fTemp23 + 2.6e+03f : 2.8e+03f)) : ((iTemp17) ? 2.8e+03f - 2e+02f * fTemp21 : 2.6e+03f)) : ((iTemp18) ? fTemp103 + 2.6e+03f : 2.7e+03f)));
			fRec17[0] = fTemp105 - std::floor(fTemp105);
			float fTemp106 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 1.3e+02f : ((iTemp63) ? fTemp66 + 1.3e+02f : 1.5e+02f)) : 1.5e+02f) : ((iTemp57) ? 1.5e+02f - fTemp62 : 1.3e+02f)) : ((iTemp54) ? 5e+01f * fTemp58 + 1.3e+02f : 1.8e+02f)) : ((iTemp52) ? 1.8e+02f - fTemp97 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp42) ? fTemp84 + 1.2e+02f : 1.3e+02f)) : ((iTemp40) ? 1.3e+02f - fTemp44 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp33) ? 1e+01f * fTemp37 + 1.2e+02f : 1.3e+02f)) : ((iTemp32) ? fTemp35 + 1.3e+02f : 1.5e+02f)) : ((iTemp30) ? 1.5e+02f - 3e+01f * fTemp74 : 1.2e+02f)) : 1.2e+02f) : ((iTemp27) ? 6e+01f * fTemp29 + 1.2e+02f : 1.8e+02f)) : ((iTemp24) ? 1.8e+02f - fTemp93 : 1.3e+02f)) : ((iTemp22) ? 1.3e+02f - fTemp83 : 1.2e+02f)) : 1.2e+02f) : ((iTemp17) ? fTemp82 + 1.2e+02f : 1.3e+02f)) : ((iTemp18) ? 1.3e+02f - 1e+01f * fTemp19 : 1.2e+02f));
			fRec19[0] = fConst6 * fTemp15 * fTemp106 + fConst5 * fRec19[1];
			float fTemp107 = std::exp(-(fConst4 * fRec19[0]));
			fRec20[0] = fConst6 * fTemp106 + fConst5 * fRec20[1];
			float fTemp108 = std::exp(-(fConst4 * fRec20[0]));
			fRec18[0] = fVec2[1] + fRec18[1] * (fTemp107 + fTemp108) - fTemp107 * fTemp108 * fRec18[2];
			float fTemp109 = ((iTemp39) ? 0.1f - 0.084151f * fTemp41 : 0.015849f);
			float fTemp110 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 0.015849f : ((iTemp63) ? 0.001934f * fTemp16 + 0.015849f : 0.017783f)) : ((iTemp60) ? 0.017783f - 0.001934f * fTemp64 : 0.015849f)) : ((iTemp57) ? 0.023962f * fTemp61 + 0.015849f : 0.039811f)) : ((iTemp54) ? 0.039811f - 0.029811f * fTemp58 : 0.01f)) : ((iTemp52) ? 0.344813f * fTemp55 + 0.01f : 0.354813f)) : ((iTemp50) ? 0.354813f - 0.103624f * fTemp53 : 0.251189f)) : ((iTemp48) ? 0.251189f - 0.171756f * fTemp51 : 0.079433f)) : ((iTemp47) ? 0.020567f * fTemp49 + 0.079433f : 0.1f)) : ((iTemp45) ? 0.1f - 0.060189f * fTemp78 : 0.039811f)) : ((iTemp42) ? 0.023285f * fTemp46 + 0.039811f : 0.063096f)) : ((iTemp40) ? 0.036904f * fTemp43 + 0.063096f : 0.1f)) : fTemp109) : ((iTemp38) ? 0.063584f * fTemp77 + 0.015849f : 0.079433f)) : ((iTemp36) ? 0.079433f - 0.04781f * fTemp76 : 0.031623f)) : ((iTemp33) ? 0.068377f * fTemp37 + 0.031623f : 0.1f)) : ((iTemp32) ? 0.1f - 0.09f * fTemp34 : 0.01f)) : ((iTemp30) ? 0.040119f * fTemp74 + 0.01f : 0.050119f)) : fTemp101) : ((iTemp27) ? 0.079433f - 0.069433f * fTemp29 : 0.01f)) : ((iTemp24) ? 0.388107f * fTemp73 + 0.01f : 0.398107f)) : ((iTemp22) ? 0.398107f - 0.198581f * fTemp25 : 0.199526f)) : ((iTemp20) ? 0.199526f - 0.099526f * fTemp23 : 0.1f)) : ((iTemp17) ? 0.151189f * fTemp21 + 0.1f : 0.251189f)) : ((iTemp18) ? 0.251189f - 0.051663f * fTemp19 : 0.199526f));
			float fTemp111 = 3.5e+02f * fTemp53;
			float fTemp112 = ((iTemp72) ? 0.0f : fRec21[1] + fConst1 * ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 3.5e+03f : ((iTemp63) ? 3.5e+03f - 2e+02f * fTemp16 : 3.3e+03f)) : ((iTemp60) ? 4e+02f * fTemp64 + 3.3e+03f : 3.7e+03f)) : ((iTemp57) ? 3.7e+03f - 2e+02f * fTemp61 : 3.5e+03f)) : 3.5e+03f) : ((iTemp52) ? 3.5e+03f - 1.05e+03f * fTemp55 : 2.45e+03f)) : ((iTemp50) ? fTemp111 + 2.45e+03f : 2.8e+03f)) : ((iTemp48) ? 2.5e+02f * fTemp51 + 2.8e+03f : 3.05e+03f)) : ((iTemp47) ? 3.05e+03f - 4.5e+02f * fTemp49 : 2.6e+03f)) : ((iTemp45) ? 75.0f * fTemp78 + 2.6e+03f : 2675.0f)) : ((iTemp42) ? 325.0f * fTemp46 + 2675.0f : 3e+03f)) : 3e+03f) : ((iTemp39) ? 3.5e+02f * fTemp41 + 3e+03f : 3.35e+03f)) : ((iTemp38) ? 3.35e+03f - 3.5e+02f * fTemp77 : 3e+03f)) : 3e+03f) : ((iTemp33) ? 9e+02f * fTemp37 + 3e+03f : 3.9e+03f)) : ((iTemp32) ? 3.9e+03f - 3e+02f * fTemp34 : 3.6e+03f)) : ((iTemp30) ? 3e+02f * fTemp74 + 3.6e+03f : 3.9e+03f)) : ((iTemp28) ? 3.9e+03f - 1e+02f * fTemp31 : 3.8e+03f)) : 3.8e+03f) : ((iTemp24) ? 3.8e+03f - 9e+02f * fTemp73 : 2.9e+03f)) : ((iTemp22) ? 3e+02f * fTemp25 + 2.9e+03f : 3.2e+03f)) : ((iTemp20) ? 5e+01f * fTemp23 + 3.2e+03f : 3.25e+03f)) : ((iTemp17) ? 3.25e+03f - 4.5e+02f * fTemp21 : 2.8e+03f)) : ((iTemp18) ? fTemp103 + 2.8e+03f : 2.9e+03f)));
			fRec21[0] = fTemp112 - std::floor(fTemp112);
			float fTemp113 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? ((iTemp63) ? ((iTemp65) ? 1.4e+02f : ((iTemp63) ? 6e+01f * fTemp16 + 1.4e+02f : 2e+02f)) : 2e+02f) : ((iTemp57) ? 2e+02f - 65.0f * fTemp61 : 135.0f)) : ((iTemp54) ? 65.0f * fTemp58 + 135.0f : 2e+02f)) : ((iTemp52) ? 2e+02f - 7e+01f * fTemp55 : 1.3e+02f)) : ((iTemp50) ? 1.3e+02f - fTemp85 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp42) ? fTemp96 + 1.2e+02f : 1.4e+02f)) : ((iTemp40) ? 1.4e+02f - fTemp95 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp33) ? fTemp94 + 1.2e+02f : 1.4e+02f)) : ((iTemp32) ? 6e+01f * fTemp34 + 1.4e+02f : 2e+02f)) : ((iTemp30) ? 2e+02f - fTemp75 : 1.2e+02f)) : 1.2e+02f) : ((iTemp27) ? 8e+01f * fTemp29 + 1.2e+02f : 2e+02f)) : ((iTemp24) ? 2e+02f - 6e+01f * fTemp73 : 1.4e+02f)) : ((iTemp22) ? 1.4e+02f - fTemp26 : 1.2e+02f)) : 1.2e+02f) : ((iTemp17) ? 15.0f * fTemp21 + 1.2e+02f : 135.0f)) : ((iTemp18) ? 135.0f - 15.0f * fTemp19 : 1.2e+02f));
			fRec23[0] = fConst6 * fTemp15 * fTemp113 + fConst5 * fRec23[1];
			float fTemp114 = std::exp(-(fConst4 * fRec23[0]));
			fRec24[0] = fConst6 * fTemp113 + fConst5 * fRec24[1];
			float fTemp115 = std::exp(-(fConst4 * fRec24[0]));
			fRec22[0] = fVec2[1] + fRec22[1] * (fTemp114 + fTemp115) - fTemp114 * fTemp115 * fRec22[2];
			float fTemp116 = ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? ((iTemp57) ? ((iTemp60) ? 0.001f : ((iTemp57) ? 0.000778f * fTemp61 + 0.001f : 0.001778f)) : ((iTemp54) ? 0.001778f - 0.001147f * fTemp58 : 0.000631f)) : ((iTemp52) ? 0.099369f * fTemp55 + 0.000631f : 0.1f)) : ((iTemp50) ? 0.025893f * fTemp53 + 0.1f : 0.125893f)) : ((iTemp48) ? 0.125893f - 0.086082f * fTemp51 : 0.039811f)) : ((iTemp47) ? 0.039811f - 0.029811f * fTemp49 : 0.01f)) : ((iTemp45) ? 0.005849f * fTemp78 + 0.01f : 0.015849f)) : ((iTemp42) ? 0.015849f - 0.00326f * fTemp46 : 0.012589f)) : ((iTemp40) ? 0.087411f * fTemp43 + 0.012589f : 0.1f)) : fTemp109) : ((iTemp38) ? 0.004104f * fTemp77 + 0.015849f : 0.019953f)) : 0.019953f) : ((iTemp33) ? 0.019953f - 0.016791f * fTemp37 : 0.003162f)) : ((iTemp32) ? 0.003162f - 0.001577f * fTemp34 : 0.001585f)) : ((iTemp30) ? 0.004725f * fTemp74 + 0.001585f : 0.00631f)) : ((iTemp28) ? 0.00631f - 0.003148f * fTemp31 : 0.003162f)) : ((iTemp27) ? 0.003162f - 0.002162f * fTemp29 : 0.001f)) : ((iTemp24) ? 0.078433f * fTemp73 + 0.001f : 0.079433f)) : ((iTemp22) ? 0.020567f * fTemp25 + 0.079433f : 0.1f)) : ((iTemp20) ? 0.1f - 0.068377f * fTemp23 : 0.031623f)) : ((iTemp17) ? 0.018496f * fTemp21 + 0.031623f : 0.050119f)) : 0.050119f);
			float fTemp117 = ((iTemp72) ? 0.0f : fRec25[1] + fConst1 * ((iTemp17) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp33) ? ((iTemp36) ? ((iTemp38) ? ((iTemp39) ? ((iTemp40) ? ((iTemp42) ? ((iTemp45) ? ((iTemp47) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 4.95e+03f : ((iTemp52) ? 4.95e+03f - 2.2e+03f * fTemp55 : 2.75e+03f)) : ((iTemp50) ? fTemp111 + 2.75e+03f : 3.1e+03f)) : ((iTemp48) ? 2.4e+02f * fTemp51 + 3.1e+03f : 3.34e+03f)) : ((iTemp47) ? 3.34e+03f - 4.4e+02f * fTemp49 : 2.9e+03f)) : ((iTemp45) ? fTemp79 + 2.9e+03f : 2.95e+03f)) : ((iTemp42) ? 4e+02f * fTemp46 + 2.95e+03f : 3.35e+03f)) : ((iTemp40) ? 3.35e+03f - fTemp104 : 3.3e+03f)) : ((iTemp39) ? 2.9e+02f * fTemp41 + 3.3e+03f : 3.59e+03f)) : ((iTemp38) ? 3.59e+03f - 2.9e+02f * fTemp77 : 3.3e+03f)) : ((iTemp36) ? 1e+02f * fTemp76 + 3.3e+03f : 3.4e+03f)) : ((iTemp33) ? 1.55e+03f * fTemp37 + 3.4e+03f : 4.95e+03f)) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : ((iTemp24) ? 4.95e+03f - 1.7e+03f * fTemp73 : 3.25e+03f)) : ((iTemp22) ? 3.3e+02f * fTemp25 + 3.25e+03f : 3.58e+03f)) : ((iTemp20) ? 3.58e+03f - 4e+01f * fTemp23 : 3.54e+03f)) : ((iTemp17) ? 3.54e+03f - 5.4e+02f * fTemp21 : 3e+03f)) : ((iTemp18) ? 3e+02f * fTemp19 + 3e+03f : 3.3e+03f)));
			fRec25[0] = fTemp117 - std::floor(fTemp117);
			fVec3[0] = fSlow22;
			fRec26[0] = fSlow22 + fRec26[1] * static_cast<float>(fVec3[1] >= fSlow22);
			iRec27[0] = iSlow24 * (iRec27[1] + 1);
			output0[i0] = static_cast<FAUSTFLOAT>(5e+01f * (fRec0[0] * ((iSlow19) ? fTemp71 : fTemp70) * (1.0f - (fTemp69 + fTemp68 * (1.0f - fTemp69))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec9[0]), 65535))] + fRec10[0] * ((iSlow19) ? fTemp71 * fTemp89 : fTemp70 * fTemp89) * (1.0f - (fTemp88 + fTemp87 * (1.0f - fTemp88))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec13[0]), 65535))] + fRec14[0] * ((iSlow19) ? fTemp71 * fTemp102 : fTemp70 * fTemp102) * (1.0f - (fTemp100 + fTemp99 * (1.0f - fTemp100))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec17[0]), 65535))] + fRec18[0] * ((iSlow19) ? fTemp71 * fTemp110 : fTemp70 * fTemp110) * (1.0f - (fTemp108 + fTemp107 * (1.0f - fTemp108))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec21[0]), 65535))] + fRec22[0] * ((iSlow19) ? fTemp71 * fTemp116 : fTemp70 * fTemp116) * (1.0f - (fTemp115 + fTemp114 * (1.0f - fTemp115))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(static_cast<int>(65536.0f * fRec25[0]), 65535))]) * std::max<float>(0.0f, std::min<float>(fSlow21 * fRec26[0], 1.0f) - fSlow23 * static_cast<float>(iRec27[0])));
			iVec0[1] = iVec0[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec5[1] = fRec5[0];
			fRec1[1] = fRec1[0];
			fVec2[1] = fVec2[0];
			fRec7[1] = fRec7[0];
			fRec6[1] = fRec6[0];
			fRec8[1] = fRec8[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec9[1] = fRec9[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec13[1] = fRec13[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec17[1] = fRec17[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec21[1] = fRec21[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec25[1] = fRec25[0];
			fVec3[1] = fVec3[0];
			fRec26[1] = fRec26[0];
			iRec27[1] = iRec27[0];
		}
	}

};
#endif

class faust_synth_voice_fofs_tilde : public FaustExternal<synth_voice_fofs> {
public:
    faust_synth_voice_fofs_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_voice_fofs))
    {
    }
};

#endif
