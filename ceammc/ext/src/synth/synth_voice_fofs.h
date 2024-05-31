/* ------------------------------------------------------------
name: "synth.voice_fofs"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_voice_fofs -scn synth_voice_fofs_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_voice_fofs_H__
#define  __synth_voice_fofs_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
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
#define FAUSTVERSION "2.74.5."

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 74
#define FAUSTPATCHVERSION 5.

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

class FAUST_API synth_voice_fofs_dsp {

    public:

        synth_voice_fofs_dsp() {}
        virtual ~synth_voice_fofs_dsp() {}

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
        virtual synth_voice_fofs_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_voice_fofs_dsp {

    protected:

        synth_voice_fofs_dsp* fDSP;

    public:

        decorator_dsp(synth_voice_fofs_dsp* synth_voice_fofs_dsp = nullptr):fDSP(synth_voice_fofs_dsp) {}
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
        virtual void control() { fDSP->control(); }
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) { fDSP->frame(inputs, outputs); }
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
    
        /* Create a new DSP instance, to be deleted with C++ 'delete' */
        virtual synth_voice_fofs_dsp* createDSPInstance() = 0;
    
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
	
  public:
	
	int getNumInputssynth_voice_fofsSIG0() {
		return 0;
	}
	int getNumOutputssynth_voice_fofsSIG0() {
		return 1;
	}
	
	void instanceInitsynth_voice_fofsSIG0(int sample_rate) {
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
			table[i1] = std::sin(9.58738e-05f * float(iRec4[0]));
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
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/listInterp:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.17.1");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_voice_fofs -scn synth_voice_fofs_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
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
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "synth.voice_fofs");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.1");
		m->declare("oscillators.lib/hs_oscsin:author", "Mike Olsen");
		m->declare("oscillators.lib/hs_phasor:author", "Mike Olsen, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("physmodels.lib/SFFormantModel:author", "Mike Olsen");
		m->declare("physmodels.lib/autobendFreq:author", "Mike Olsen");
		m->declare("physmodels.lib/fof:author", "Mike Olsen");
		m->declare("physmodels.lib/fofSmooth:author", "Mike Olsen");
		m->declare("physmodels.lib/formantFilterbank:author", "Mike Olsen");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/skirtWidthMultiplier:author", "Mike Olsen");
		m->declare("physmodels.lib/version", "1.1.0");
		m->declare("physmodels.lib/vocalEffort:author", "Mike Olsen");
		m->declare("physmodels.lib/voiceGender:author", "Mike Olsen");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.5.0");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 3.1415927f / fConst0;
		fConst5 = std::exp(-(1e+02f / fConst0));
		fConst6 = 1.0f - fConst5;
		fConst7 = 0.001f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(4.4e+02f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(6.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(1e+01f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(3e+01f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(5e+01f);
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
		return new synth_voice_fofs();
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
		float fSlow0 = fConst2 * float(fVslider0);
		float fSlow1 = 0.001f * float(fHslider0);
		int iSlow2 = std::fabs(fSlow1) < 1.1920929e-07f;
		float fSlow3 = ((iSlow2) ? 0.0f : std::exp(-(fConst1 / ((iSlow2) ? 1.0f : fSlow1))));
		float fSlow4 = 1.0f - fSlow3;
		float fSlow5 = 0.1f * float(fHslider1);
		float fSlow6 = fConst1 * float(fHslider2);
		float fSlow7 = 0.001f * float(fHslider4);
		int iSlow8 = std::fabs(fSlow7) < 1.1920929e-07f;
		float fSlow9 = ((iSlow8) ? 0.0f : std::exp(-(fConst1 / ((iSlow8) ? 1.0f : fSlow7))));
		float fSlow10 = float(fHslider3) * (1.0f - fSlow9);
		int iSlow11 = int(float(fHslider5));
		int iSlow12 = ((iSlow11 == 0) ? 1 : ((iSlow11 == 3) ? 1 : 0));
		float fSlow13 = float(5 * iSlow12);
		int iSlow14 = iSlow12 >= 1;
		float fSlow15 = ((iSlow14) ? 174.61f : 82.41f);
		float fSlow16 = ((iSlow14) ? 1046.5f : 523.25f);
		float fSlow17 = fSlow16 - fSlow15;
		float fSlow18 = float(5 * iSlow11);
		int iSlow19 = iSlow12 == 0;
		int iSlow20 = iSlow11 != 2;
		float fSlow21 = 1.0f / std::max<float>(1.0f, fConst7 * float(fHslider6));
		float fSlow22 = float(fCheckbox0);
		float fSlow23 = 1.0f / std::max<float>(1.0f, fConst7 * float(fHslider7));
		int iSlow24 = fSlow22 == 0.0f;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			int iTemp0 = 1 - iVec0[1];
			fRec3[0] = fSlow0 + fConst3 * fRec3[1];
			fRec2[0] = fRec3[0] * fSlow4 + fSlow3 * fRec2[1];
			float fTemp1 = ((iTemp0) ? 0.0f : fSlow6 + fRec5[1]);
			fRec5[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = fRec2[0] * (fSlow5 * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec5[0]), 65535))] + 1.0f);
			float fTemp3 = ((iTemp0) ? 0.0f : fRec1[1] + fConst1 * fTemp2);
			fRec1[0] = fTemp3 - std::floor(fTemp3);
			float fTemp4 = float((fRec1[0] - fRec1[1]) < 0.0f);
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
			int iTemp18 = fTemp16 < 22.0f;
			int iTemp19 = fTemp16 < 21.0f;
			int iTemp20 = fTemp16 < 2e+01f;
			float fTemp21 = fSlow18 + fRec7[0] + -2e+01f;
			float fTemp22 = 2e+01f * fTemp21;
			int iTemp23 = fTemp16 < 19.0f;
			float fTemp24 = fSlow18 + fRec7[0] + -19.0f;
			float fTemp25 = 5e+01f * fTemp24;
			int iTemp26 = fTemp16 < 18.0f;
			float fTemp27 = fSlow18 + fRec7[0] + -18.0f;
			int iTemp28 = fTemp16 < 17.0f;
			int iTemp29 = fTemp16 < 16.0f;
			float fTemp30 = fSlow18 + fRec7[0] + -16.0f;
			int iTemp31 = fTemp16 < 15.0f;
			int iTemp32 = fTemp16 < 14.0f;
			float fTemp33 = fSlow18 + fRec7[0] + -14.0f;
			float fTemp34 = 2e+01f * fTemp33;
			int iTemp35 = fTemp16 < 13.0f;
			int iTemp36 = fTemp16 < 12.0f;
			int iTemp37 = fTemp16 < 11.0f;
			int iTemp38 = fTemp16 < 1e+01f;
			float fTemp39 = fSlow18 + fRec7[0] + -1e+01f;
			float fTemp40 = 2e+01f * fTemp39;
			int iTemp41 = fTemp16 < 9.0f;
			float fTemp42 = fSlow18 + fRec7[0] + -9.0f;
			float fTemp43 = 2e+01f * fTemp42;
			int iTemp44 = fTemp16 < 8.0f;
			int iTemp45 = fTemp16 < 7.0f;
			int iTemp46 = fTemp16 < 6.0f;
			int iTemp47 = fTemp16 < 5.0f;
			float fTemp48 = fSlow18 + fRec7[0] + -5.0f;
			float fTemp49 = 1e+01f * fTemp48;
			int iTemp50 = fTemp16 < 4.0f;
			float fTemp51 = fSlow18 + fRec7[0] + -4.0f;
			float fTemp52 = 6e+01f * fTemp51;
			int iTemp53 = fTemp16 < 3.0f;
			float fTemp54 = fSlow18 + fRec7[0] + -3.0f;
			int iTemp55 = fTemp16 < 2.0f;
			float fTemp56 = fSlow18 + fRec7[0] + -2.0f;
			float fTemp57 = 2e+01f * fTemp56;
			float fTemp58 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? 1.2e+02f : ((iTemp53) ? 1.2e+02f - fTemp57 : 1e+02f)) : ((iTemp50) ? 7e+01f * fTemp54 + 1e+02f : 1.7e+02f)) : ((iTemp47) ? 1.7e+02f - fTemp52 : 1.1e+02f)) : ((iTemp46) ? 1.1e+02f - fTemp49 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp38) ? fTemp43 + 1e+02f : 1.2e+02f)) : ((iTemp37) ? 1.2e+02f - fTemp40 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp31) ? fTemp34 + 1e+02f : 1.2e+02f)) : 1.2e+02f) : ((iTemp28) ? 1.2e+02f - 2e+01f * fTemp30 : 1e+02f)) : 1e+02f) : ((iTemp23) ? 7e+01f * fTemp27 + 1e+02f : 1.7e+02f)) : ((iTemp20) ? 1.7e+02f - fTemp25 : 1.2e+02f)) : ((iTemp19) ? 1.2e+02f - fTemp22 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f);
			fRec6[0] = fConst6 * fTemp15 * fTemp58 + fConst5 * fRec6[1];
			float fTemp59 = std::exp(-(fConst4 * fRec6[0]));
			fRec8[0] = fConst6 * fTemp58 + fConst5 * fRec8[1];
			float fTemp60 = std::exp(-(fConst4 * fRec8[0]));
			fRec0[0] = fVec2[1] + fRec0[1] * (fTemp59 + fTemp60) - fTemp59 * fTemp60 * fRec0[2];
			float fTemp61 = 0.00084f * (1e+03f - fTemp2) + 0.8f;
			int iTemp62 = fTemp16 < 24.0f;
			float fTemp63 = fSlow18 + fRec7[0] + -23.0f;
			float fTemp64 = fSlow18 + fRec7[0] + -22.0f;
			float fTemp65 = fSlow18 + fRec7[0] + -21.0f;
			float fTemp66 = fSlow18 + fRec7[0] + -17.0f;
			float fTemp67 = ((iTemp26) ? 0.029314f * fTemp66 + 0.050119f : 0.079433f);
			float fTemp68 = fSlow18 + fRec7[0] + -15.0f;
			float fTemp69 = fSlow18 + fRec7[0] + -13.0f;
			float fTemp70 = fSlow18 + fRec7[0] + -12.0f;
			float fTemp71 = fSlow18 + fRec7[0] + -11.0f;
			float fTemp72 = fSlow18 + fRec7[0] + -8.0f;
			float fTemp73 = fSlow18 + fRec7[0] + -7.0f;
			float fTemp74 = fSlow18 + fRec7[0] + -6.0f;
			int iTemp75 = fTemp16 < 1.0f;
			int iTemp76 = fTemp16 < 0.0f;
			float fTemp77 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 0.1f : ((iTemp75) ? 0.1f - 0.068377f * fTemp16 : 0.031623f)) : 0.031623f) : ((iTemp53) ? 0.126866f * fTemp56 + 0.031623f : 0.158489f)) : ((iTemp50) ? 0.158489f - 0.126866f * fTemp54 : 0.031623f)) : ((iTemp47) ? 0.32319f * fTemp51 + 0.031623f : 0.354813f)) : 0.354813f) : ((iTemp45) ? 0.354813f - 0.196324f * fTemp74 : 0.158489f)) : ((iTemp44) ? 0.158489f - 0.069364f * fTemp73 : 0.089125f)) : ((iTemp41) ? 0.089125f - 0.064006f * fTemp72 : 0.025119f)) : ((iTemp38) ? 0.045676f * fTemp42 + 0.025119f : 0.070795f)) : ((iTemp37) ? 0.055098f * fTemp39 + 0.070795f : 0.125893f)) : ((iTemp36) ? 0.125893f - 0.062797f * fTemp71 : 0.063096f)) : ((iTemp35) ? 0.063096f - 0.012977f * fTemp70 : 0.050119f)) : ((iTemp32) ? 0.020676f * fTemp69 + 0.050119f : 0.070795f)) : ((iTemp31) ? 0.070795f - 0.045676f * fTemp33 : 0.025119f)) : ((iTemp29) ? 0.152709f * fTemp68 + 0.025119f : 0.177828f)) : ((iTemp28) ? 0.177828f - 0.127709f * fTemp30 : 0.050119f)) : fTemp67) : ((iTemp23) ? 0.079433f - 0.06165f * fTemp27 : 0.017783f)) : ((iTemp20) ? 0.428901f * fTemp24 + 0.017783f : 0.446684f)) : ((iTemp19) ? 0.446684f - 0.195495f * fTemp21 : 0.251189f)) : ((iTemp18) ? 0.251189f - 0.125296f * fTemp65 : 0.125893f)) : ((iTemp17) ? 0.125296f * fTemp64 + 0.125893f : 0.251189f)) : ((iTemp62) ? 0.251189f - 0.109935f * fTemp63 : 0.141254f));
			float fTemp78 = 0.0036666666f * (4e+02f - fTemp2) + 3.0f;
			int iTemp79 = iTemp0 | int(fTemp4);
			float fTemp80 = 1e+02f * fTemp63;
			float fTemp81 = 5e+01f * fTemp39;
			float fTemp82 = ((iTemp79) ? 0.0f : fRec9[1] + fConst1 * ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 2.8e+03f : ((iTemp75) ? 2.8e+03f - 1e+02f * fTemp16 : 2.7e+03f)) : 2.7e+03f) : ((iTemp53) ? 1.3e+02f * fTemp56 + 2.7e+03f : 2.83e+03f)) : ((iTemp50) ? 2.83e+03f - 3e+02f * fTemp54 : 2.53e+03f)) : ((iTemp47) ? 2.53e+03f - 2.8e+02f * fTemp51 : 2.25e+03f)) : ((iTemp46) ? 1.5e+02f * fTemp48 + 2.25e+03f : 2.4e+03f)) : ((iTemp45) ? 2e+02f * fTemp74 + 2.4e+03f : 2.6e+03f)) : ((iTemp44) ? 2.6e+03f - 2e+02f * fTemp73 : 2.4e+03f)) : 2.4e+03f) : ((iTemp38) ? 3.5e+02f * fTemp42 + 2.4e+03f : 2.75e+03f)) : ((iTemp37) ? 2.75e+03f - fTemp81 : 2.7e+03f)) : ((iTemp36) ? 2e+02f * fTemp71 + 2.7e+03f : 2.9e+03f)) : ((iTemp35) ? 2.9e+03f - 2e+02f * fTemp70 : 2.7e+03f)) : ((iTemp32) ? 5e+01f * fTemp69 + 2.7e+03f : 2.75e+03f)) : ((iTemp31) ? 1.5e+02f * fTemp33 + 2.75e+03f : 2.9e+03f)) : ((iTemp29) ? 2.9e+03f - 1e+02f * fTemp68 : 2.8e+03f)) : ((iTemp28) ? 1.5e+02f * fTemp30 + 2.8e+03f : 2.95e+03f)) : ((iTemp26) ? 2.95e+03f - 1.2e+02f * fTemp66 : 2.83e+03f)) : ((iTemp23) ? 2.83e+03f - 1.3e+02f * fTemp27 : 2.7e+03f)) : ((iTemp20) ? 2.7e+03f - fTemp25 : 2.65e+03f)) : ((iTemp19) ? 2.65e+03f - 5e+01f * fTemp21 : 2.6e+03f)) : ((iTemp18) ? 2e+02f * fTemp65 + 2.6e+03f : 2.8e+03f)) : ((iTemp17) ? 2.8e+03f - 2e+02f * fTemp64 : 2.6e+03f)) : ((iTemp62) ? fTemp80 + 2.6e+03f : 2.7e+03f)));
			fRec9[0] = fTemp82 - std::floor(fTemp82);
			float fTemp83 = 2e+01f * fTemp68;
			float fTemp84 = 1e+01f * fTemp39;
			float fTemp85 = 1e+01f * fTemp51;
			float fTemp86 = 2e+01f * fTemp54;
			float fTemp87 = fSlow18 + fRec7[0] + -1.0f;
			float fTemp88 = 2e+01f * fTemp16;
			float fTemp89 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 8e+01f : ((iTemp75) ? 8e+01f - fTemp88 : 6e+01f)) : ((iTemp55) ? 6e+01f - 1e+01f * fTemp87 : 5e+01f)) : ((iTemp53) ? fTemp57 + 5e+01f : 7e+01f)) : ((iTemp50) ? 7e+01f - fTemp86 : 5e+01f)) : ((iTemp47) ? fTemp85 + 5e+01f : 6e+01f)) : ((iTemp46) ? 6e+01f - 2e+01f * fTemp48 : 4e+01f)) : ((iTemp45) ? 2e+01f * fTemp74 + 4e+01f : 6e+01f)) : ((iTemp44) ? 6e+01f - 2e+01f * fTemp73 : 4e+01f)) : 4e+01f) : ((iTemp38) ? 4e+01f * (fTemp42 + 1.0f) : 8e+01f)) : ((iTemp37) ? 8e+01f - fTemp84 : 7e+01f)) : ((iTemp36) ? 7e+01f - 3e+01f * fTemp71 : 4e+01f)) : 4e+01f) : 4e+01f) : ((iTemp31) ? 4e+01f * (fTemp33 + 1.0f) : 8e+01f)) : ((iTemp29) ? 8e+01f - fTemp83 : 6e+01f)) : 6e+01f) : ((iTemp26) ? 6e+01f - 2e+01f * fTemp66 : 4e+01f)) : ((iTemp23) ? 1e+01f * fTemp27 + 4e+01f : 5e+01f)) : 5e+01f) : ((iTemp19) ? fTemp22 + 5e+01f : 7e+01f)) : ((iTemp18) ? 7e+01f - 3e+01f * fTemp65 : 4e+01f)) : ((iTemp17) ? 3e+01f * fTemp64 + 4e+01f : 7e+01f)) : ((iTemp62) ? 7e+01f - 3e+01f * fTemp63 : 4e+01f));
			fRec11[0] = fConst6 * fTemp15 * fTemp89 + fConst5 * fRec11[1];
			float fTemp90 = std::exp(-(fConst4 * fRec11[0]));
			fRec12[0] = fConst6 * fTemp89 + fConst5 * fRec12[1];
			float fTemp91 = std::exp(-(fConst4 * fRec12[0]));
			fRec10[0] = fVec2[1] + fRec10[1] * (fTemp90 + fTemp91) - fTemp90 * fTemp91 * fRec10[2];
			float fTemp92 = 8e+01f * fTemp30;
			float fTemp93 = 5e+01f * fTemp72;
			float fTemp94 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 8e+02f : ((iTemp75) ? 8e+02f - 4e+02f * fTemp16 : 4e+02f)) : ((iTemp55) ? 4e+02f - 5e+01f * fTemp87 : 3.5e+02f)) : ((iTemp53) ? 1e+02f * fTemp56 + 3.5e+02f : 4.5e+02f)) : ((iTemp50) ? 4.5e+02f - 125.0f * fTemp54 : 325.0f)) : ((iTemp47) ? 275.0f * fTemp51 + 325.0f : 6e+02f)) : ((iTemp46) ? 6e+02f - 2e+02f * fTemp48 : 4e+02f)) : ((iTemp45) ? 4e+02f - 1.5e+02f * fTemp74 : 2.5e+02f)) : ((iTemp44) ? 1.5e+02f * fTemp73 + 2.5e+02f : 4e+02f)) : ((iTemp41) ? 4e+02f - fTemp93 : 3.5e+02f)) : ((iTemp38) ? 3.1e+02f * fTemp42 + 3.5e+02f : 6.6e+02f)) : ((iTemp37) ? 6.6e+02f - 2.2e+02f * fTemp39 : 4.4e+02f)) : ((iTemp36) ? 4.4e+02f - 1.7e+02f * fTemp71 : 2.7e+02f)) : ((iTemp35) ? 1.6e+02f * fTemp70 + 2.7e+02f : 4.3e+02f)) : ((iTemp32) ? 4.3e+02f - 6e+01f * fTemp69 : 3.7e+02f)) : ((iTemp31) ? 4.3e+02f * fTemp33 + 3.7e+02f : 8e+02f)) : ((iTemp29) ? 8e+02f - 4.5e+02f * fTemp68 : 3.5e+02f)) : ((iTemp28) ? 3.5e+02f - fTemp92 : 2.7e+02f)) : ((iTemp26) ? 1.8e+02f * fTemp66 + 2.7e+02f : 4.5e+02f)) : ((iTemp23) ? 4.5e+02f - 125.0f * fTemp27 : 325.0f)) : ((iTemp20) ? 325.0f * (fTemp24 + 1.0f) : 6.5e+02f)) : ((iTemp19) ? 6.5e+02f - 2.5e+02f * fTemp21 : 4e+02f)) : ((iTemp18) ? 4e+02f - 1.1e+02f * fTemp65 : 2.9e+02f)) : ((iTemp17) ? 1.1e+02f * fTemp64 + 2.9e+02f : 4e+02f)) : ((iTemp62) ? 4e+02f - 5e+01f * fTemp63 : 3.5e+02f));
			float fTemp95 = ((iTemp79) ? 0.0f : fRec13[1] + fConst1 * ((fTemp94 <= fTemp2) ? fTemp2 : fTemp94));
			fRec13[0] = fTemp95 - std::floor(fTemp95);
			float fTemp96 = 1e+01f * fTemp64;
			float fTemp97 = 1e+01f * fTemp21;
			float fTemp98 = 1e+01f * fTemp42;
			float fTemp99 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 9e+01f : ((iTemp75) ? 9e+01f - 1e+01f * fTemp16 : 8e+01f)) : ((iTemp55) ? 2e+01f * fTemp87 + 8e+01f : 1e+02f)) : ((iTemp53) ? 1e+02f - fTemp57 : 8e+01f)) : ((iTemp50) ? 8e+01f - fTemp86 : 6e+01f)) : ((iTemp47) ? fTemp85 + 6e+01f : 7e+01f)) : ((iTemp46) ? fTemp49 + 7e+01f : 8e+01f)) : ((iTemp45) ? 1e+01f * fTemp74 + 8e+01f : 9e+01f)) : ((iTemp44) ? 9e+01f - 1e+01f * fTemp73 : 8e+01f)) : 8e+01f) : ((iTemp38) ? fTemp98 + 8e+01f : 9e+01f)) : ((iTemp37) ? 9e+01f - fTemp84 : 8e+01f)) : ((iTemp36) ? 1e+01f * fTemp71 + 8e+01f : 9e+01f)) : ((iTemp35) ? 9e+01f - 1e+01f * fTemp70 : 8e+01f)) : ((iTemp32) ? 8e+01f - 2e+01f * fTemp69 : 6e+01f)) : ((iTemp31) ? 3e+01f * fTemp33 + 6e+01f : 9e+01f)) : ((iTemp29) ? 1e+01f * fTemp68 + 9e+01f : 1e+02f)) : ((iTemp28) ? 1e+02f - 1e+01f * fTemp30 : 9e+01f)) : ((iTemp26) ? 9e+01f - 1e+01f * fTemp66 : 8e+01f)) : ((iTemp23) ? 8e+01f - 2e+01f * fTemp27 : 6e+01f)) : ((iTemp20) ? 3e+01f * fTemp24 + 6e+01f : 9e+01f)) : ((iTemp19) ? 9e+01f - fTemp97 : 8e+01f)) : ((iTemp18) ? 1e+01f * fTemp65 + 8e+01f : 9e+01f)) : ((iTemp17) ? 9e+01f - fTemp96 : 8e+01f)) : ((iTemp62) ? 8e+01f - 2e+01f * fTemp63 : 6e+01f));
			fRec15[0] = fConst6 * fTemp15 * fTemp99 + fConst5 * fRec15[1];
			float fTemp100 = std::exp(-(fConst4 * fRec15[0]));
			fRec16[0] = fConst6 * fTemp99 + fConst5 * fRec16[1];
			float fTemp101 = std::exp(-(fConst4 * fRec16[0]));
			fRec14[0] = fVec2[1] + fRec14[1] * (fTemp100 + fTemp101) - fTemp100 * fTemp101 * fRec14[2];
			float fTemp102 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 0.630957f : ((iTemp75) ? 0.630957f - 0.567861f * fTemp16 : 0.063096f)) : ((iTemp55) ? 0.036904f * fTemp87 + 0.063096f : 0.1f)) : ((iTemp53) ? 0.254813f * fTemp56 + 0.1f : 0.354813f)) : ((iTemp50) ? 0.354813f - 0.103624f * fTemp54 : 0.251189f)) : ((iTemp47) ? 0.195495f * fTemp51 + 0.251189f : 0.446684f)) : ((iTemp46) ? 0.446684f - 0.195495f * fTemp48 : 0.251189f)) : ((iTemp45) ? 0.251189f - 0.219566f * fTemp74 : 0.031623f)) : ((iTemp44) ? 0.250215f * fTemp73 + 0.031623f : 0.281838f)) : ((iTemp41) ? 0.281838f - 0.181838f * fTemp72 : 0.1f)) : ((iTemp38) ? 0.401187f * fTemp42 + 0.1f : 0.501187f)) : ((iTemp37) ? 0.501187f - 0.301661f * fTemp39 : 0.199526f)) : ((iTemp36) ? 0.199526f - 0.13643f * fTemp71 : 0.063096f)) : ((iTemp35) ? 0.253132f * fTemp70 + 0.063096f : 0.316228f)) : ((iTemp32) ? 0.316228f - 0.216228f * fTemp69 : 0.1f)) : ((iTemp31) ? 0.401187f * fTemp33 + 0.1f : 0.501187f)) : ((iTemp29) ? 0.501187f - 0.401187f * fTemp68 : 0.1f)) : ((iTemp28) ? 0.151189f * fTemp30 + 0.1f : 0.251189f)) : ((iTemp26) ? 0.030649f * fTemp66 + 0.251189f : 0.281838f)) : ((iTemp23) ? 0.281838f - 0.123349f * fTemp27 : 0.158489f)) : ((iTemp20) ? 0.342698f * fTemp24 + 0.158489f : 0.501187f)) : ((iTemp19) ? 0.501187f - 0.301661f * fTemp21 : 0.199526f)) : ((iTemp18) ? 0.199526f - 0.021698f * fTemp65 : 0.177828f)) : ((iTemp17) ? 0.1384f * fTemp64 + 0.177828f : 0.316228f)) : ((iTemp62) ? 0.316228f - 0.216228f * fTemp63 : 0.1f));
			float fTemp103 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 1.15e+03f : ((iTemp75) ? 4.5e+02f * fTemp16 + 1.15e+03f : 1.6e+03f)) : ((iTemp55) ? 1e+02f * fTemp87 + 1.6e+03f : 1.7e+03f)) : ((iTemp53) ? 1.7e+03f - 9e+02f * fTemp56 : 8e+02f)) : ((iTemp50) ? 8e+02f - 1e+02f * fTemp54 : 7e+02f)) : ((iTemp47) ? 3.4e+02f * fTemp51 + 7e+02f : 1.04e+03f)) : ((iTemp46) ? 5.8e+02f * fTemp48 + 1.04e+03f : 1.62e+03f)) : ((iTemp45) ? 1.3e+02f * fTemp74 + 1.62e+03f : 1.75e+03f)) : ((iTemp44) ? 1.75e+03f - 1e+03f * fTemp73 : 7.5e+02f)) : ((iTemp41) ? 7.5e+02f - 1.5e+02f * fTemp72 : 6e+02f)) : ((iTemp38) ? 5.2e+02f * fTemp42 + 6e+02f : 1.12e+03f)) : ((iTemp37) ? 6.8e+02f * fTemp39 + 1.12e+03f : 1.8e+03f)) : ((iTemp36) ? 5e+01f * fTemp71 + 1.8e+03f : 1.85e+03f)) : ((iTemp35) ? 1.85e+03f - 1.03e+03f * fTemp70 : 8.2e+02f)) : ((iTemp32) ? 8.2e+02f - 1.9e+02f * fTemp69 : 6.3e+02f)) : ((iTemp31) ? 5.2e+02f * fTemp33 + 6.3e+02f : 1.15e+03f)) : ((iTemp29) ? 8.5e+02f * fTemp68 + 1.15e+03f : 2e+03f)) : ((iTemp28) ? 1.4e+02f * fTemp30 + 2e+03f : 2.14e+03f)) : ((iTemp26) ? 2.14e+03f - 1.34e+03f * fTemp66 : 8e+02f)) : ((iTemp23) ? 8e+02f - 1e+02f * fTemp27 : 7e+02f)) : ((iTemp20) ? 3.8e+02f * fTemp24 + 7e+02f : 1.08e+03f)) : ((iTemp19) ? 6.2e+02f * fTemp21 + 1.08e+03f : 1.7e+03f)) : ((iTemp18) ? 1.7e+02f * fTemp65 + 1.7e+03f : 1.87e+03f)) : ((iTemp17) ? 1.87e+03f - 1.07e+03f * fTemp64 : 8e+02f)) : ((iTemp62) ? 8e+02f - 2e+02f * fTemp63 : 6e+02f));
			float fTemp104 = 2.0f * fTemp2 + 3e+01f;
			float fTemp105 = ((iTemp79) ? 0.0f : fRec17[1] + fConst1 * ((iSlow20) ? (((fTemp103 >= 1.3e+03f) & (fTemp2 >= 2e+02f)) ? fTemp103 - 0.00095238094f * (fTemp2 + -2e+02f) * (fTemp103 + -1.3e+03f) : ((fTemp103 <= fTemp104) ? fTemp104 : fTemp103)) : fTemp103));
			fRec17[0] = fTemp105 - std::floor(fTemp105);
			float fTemp106 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 1.3e+02f : ((iTemp75) ? fTemp88 + 1.3e+02f : 1.5e+02f)) : 1.5e+02f) : ((iTemp53) ? 1.5e+02f - fTemp57 : 1.3e+02f)) : ((iTemp50) ? 5e+01f * fTemp54 + 1.3e+02f : 1.8e+02f)) : ((iTemp47) ? 1.8e+02f - fTemp52 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp38) ? fTemp98 + 1.2e+02f : 1.3e+02f)) : ((iTemp37) ? 1.3e+02f - fTemp84 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp31) ? 1e+01f * fTemp33 + 1.2e+02f : 1.3e+02f)) : ((iTemp29) ? fTemp83 + 1.3e+02f : 1.5e+02f)) : ((iTemp28) ? 1.5e+02f - 3e+01f * fTemp30 : 1.2e+02f)) : 1.2e+02f) : ((iTemp23) ? 6e+01f * fTemp27 + 1.2e+02f : 1.8e+02f)) : ((iTemp20) ? 1.8e+02f - fTemp25 : 1.3e+02f)) : ((iTemp19) ? 1.3e+02f - fTemp97 : 1.2e+02f)) : 1.2e+02f) : ((iTemp17) ? fTemp96 + 1.2e+02f : 1.3e+02f)) : ((iTemp62) ? 1.3e+02f - 1e+01f * fTemp63 : 1.2e+02f));
			fRec19[0] = fConst6 * fTemp15 * fTemp106 + fConst5 * fRec19[1];
			float fTemp107 = std::exp(-(fConst4 * fRec19[0]));
			fRec20[0] = fConst6 * fTemp106 + fConst5 * fRec20[1];
			float fTemp108 = std::exp(-(fConst4 * fRec20[0]));
			fRec18[0] = fVec2[1] + fRec18[1] * (fTemp107 + fTemp108) - fTemp107 * fTemp108 * fRec18[2];
			float fTemp109 = ((iTemp36) ? 0.1f - 0.084151f * fTemp71 : 0.015849f);
			float fTemp110 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 0.015849f : ((iTemp75) ? 0.001934f * fTemp16 + 0.015849f : 0.017783f)) : ((iTemp55) ? 0.017783f - 0.001934f * fTemp87 : 0.015849f)) : ((iTemp53) ? 0.023962f * fTemp56 + 0.015849f : 0.039811f)) : ((iTemp50) ? 0.039811f - 0.029811f * fTemp54 : 0.01f)) : ((iTemp47) ? 0.344813f * fTemp51 + 0.01f : 0.354813f)) : ((iTemp46) ? 0.354813f - 0.103624f * fTemp48 : 0.251189f)) : ((iTemp45) ? 0.251189f - 0.171756f * fTemp74 : 0.079433f)) : ((iTemp44) ? 0.020567f * fTemp73 + 0.079433f : 0.1f)) : ((iTemp41) ? 0.1f - 0.060189f * fTemp72 : 0.039811f)) : ((iTemp38) ? 0.023285f * fTemp42 + 0.039811f : 0.063096f)) : ((iTemp37) ? 0.036904f * fTemp39 + 0.063096f : 0.1f)) : fTemp109) : ((iTemp35) ? 0.063584f * fTemp70 + 0.015849f : 0.079433f)) : ((iTemp32) ? 0.079433f - 0.04781f * fTemp69 : 0.031623f)) : ((iTemp31) ? 0.068377f * fTemp33 + 0.031623f : 0.1f)) : ((iTemp29) ? 0.1f - 0.09f * fTemp68 : 0.01f)) : ((iTemp28) ? 0.040119f * fTemp30 + 0.01f : 0.050119f)) : fTemp67) : ((iTemp23) ? 0.079433f - 0.069433f * fTemp27 : 0.01f)) : ((iTemp20) ? 0.388107f * fTemp24 + 0.01f : 0.398107f)) : ((iTemp19) ? 0.398107f - 0.198581f * fTemp21 : 0.199526f)) : ((iTemp18) ? 0.199526f - 0.099526f * fTemp65 : 0.1f)) : ((iTemp17) ? 0.151189f * fTemp64 + 0.1f : 0.251189f)) : ((iTemp62) ? 0.251189f - 0.051663f * fTemp63 : 0.199526f));
			float fTemp111 = 3.5e+02f * fTemp48;
			float fTemp112 = ((iTemp79) ? 0.0f : fRec21[1] + fConst1 * ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 3.5e+03f : ((iTemp75) ? 3.5e+03f - 2e+02f * fTemp16 : 3.3e+03f)) : ((iTemp55) ? 4e+02f * fTemp87 + 3.3e+03f : 3.7e+03f)) : ((iTemp53) ? 3.7e+03f - 2e+02f * fTemp56 : 3.5e+03f)) : 3.5e+03f) : ((iTemp47) ? 3.5e+03f - 1.05e+03f * fTemp51 : 2.45e+03f)) : ((iTemp46) ? fTemp111 + 2.45e+03f : 2.8e+03f)) : ((iTemp45) ? 2.5e+02f * fTemp74 + 2.8e+03f : 3.05e+03f)) : ((iTemp44) ? 3.05e+03f - 4.5e+02f * fTemp73 : 2.6e+03f)) : ((iTemp41) ? 75.0f * fTemp72 + 2.6e+03f : 2675.0f)) : ((iTemp38) ? 325.0f * fTemp42 + 2675.0f : 3e+03f)) : 3e+03f) : ((iTemp36) ? 3.5e+02f * fTemp71 + 3e+03f : 3.35e+03f)) : ((iTemp35) ? 3.35e+03f - 3.5e+02f * fTemp70 : 3e+03f)) : 3e+03f) : ((iTemp31) ? 9e+02f * fTemp33 + 3e+03f : 3.9e+03f)) : ((iTemp29) ? 3.9e+03f - 3e+02f * fTemp68 : 3.6e+03f)) : ((iTemp28) ? 3e+02f * fTemp30 + 3.6e+03f : 3.9e+03f)) : ((iTemp26) ? 3.9e+03f - 1e+02f * fTemp66 : 3.8e+03f)) : 3.8e+03f) : ((iTemp20) ? 3.8e+03f - 9e+02f * fTemp24 : 2.9e+03f)) : ((iTemp19) ? 3e+02f * fTemp21 + 2.9e+03f : 3.2e+03f)) : ((iTemp18) ? 5e+01f * fTemp65 + 3.2e+03f : 3.25e+03f)) : ((iTemp17) ? 3.25e+03f - 4.5e+02f * fTemp64 : 2.8e+03f)) : ((iTemp62) ? fTemp80 + 2.8e+03f : 2.9e+03f)));
			fRec21[0] = fTemp112 - std::floor(fTemp112);
			float fTemp113 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? ((iTemp75) ? ((iTemp76) ? 1.4e+02f : ((iTemp75) ? 6e+01f * fTemp16 + 1.4e+02f : 2e+02f)) : 2e+02f) : ((iTemp53) ? 2e+02f - 65.0f * fTemp56 : 135.0f)) : ((iTemp50) ? 65.0f * fTemp54 + 135.0f : 2e+02f)) : ((iTemp47) ? 2e+02f - 7e+01f * fTemp51 : 1.3e+02f)) : ((iTemp46) ? 1.3e+02f - fTemp49 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp38) ? fTemp43 + 1.2e+02f : 1.4e+02f)) : ((iTemp37) ? 1.4e+02f - fTemp40 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp31) ? fTemp34 + 1.2e+02f : 1.4e+02f)) : ((iTemp29) ? 6e+01f * fTemp68 + 1.4e+02f : 2e+02f)) : ((iTemp28) ? 2e+02f - fTemp92 : 1.2e+02f)) : 1.2e+02f) : ((iTemp23) ? 8e+01f * fTemp27 + 1.2e+02f : 2e+02f)) : ((iTemp20) ? 2e+02f - 6e+01f * fTemp24 : 1.4e+02f)) : ((iTemp19) ? 1.4e+02f - fTemp22 : 1.2e+02f)) : 1.2e+02f) : ((iTemp17) ? 15.0f * fTemp64 + 1.2e+02f : 135.0f)) : ((iTemp62) ? 135.0f - 15.0f * fTemp63 : 1.2e+02f));
			fRec23[0] = fConst6 * fTemp15 * fTemp113 + fConst5 * fRec23[1];
			float fTemp114 = std::exp(-(fConst4 * fRec23[0]));
			fRec24[0] = fConst6 * fTemp113 + fConst5 * fRec24[1];
			float fTemp115 = std::exp(-(fConst4 * fRec24[0]));
			fRec22[0] = fVec2[1] + fRec22[1] * (fTemp114 + fTemp115) - fTemp114 * fTemp115 * fRec22[2];
			float fTemp116 = ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? ((iTemp53) ? ((iTemp55) ? 0.001f : ((iTemp53) ? 0.000778f * fTemp56 + 0.001f : 0.001778f)) : ((iTemp50) ? 0.001778f - 0.001147f * fTemp54 : 0.000631f)) : ((iTemp47) ? 0.099369f * fTemp51 + 0.000631f : 0.1f)) : ((iTemp46) ? 0.025893f * fTemp48 + 0.1f : 0.125893f)) : ((iTemp45) ? 0.125893f - 0.086082f * fTemp74 : 0.039811f)) : ((iTemp44) ? 0.039811f - 0.029811f * fTemp73 : 0.01f)) : ((iTemp41) ? 0.005849f * fTemp72 + 0.01f : 0.015849f)) : ((iTemp38) ? 0.015849f - 0.00326f * fTemp42 : 0.012589f)) : ((iTemp37) ? 0.087411f * fTemp39 + 0.012589f : 0.1f)) : fTemp109) : ((iTemp35) ? 0.004104f * fTemp70 + 0.015849f : 0.019953f)) : 0.019953f) : ((iTemp31) ? 0.019953f - 0.016791f * fTemp33 : 0.003162f)) : ((iTemp29) ? 0.003162f - 0.001577f * fTemp68 : 0.001585f)) : ((iTemp28) ? 0.004725f * fTemp30 + 0.001585f : 0.00631f)) : ((iTemp26) ? 0.00631f - 0.003148f * fTemp66 : 0.003162f)) : ((iTemp23) ? 0.003162f - 0.002162f * fTemp27 : 0.001f)) : ((iTemp20) ? 0.078433f * fTemp24 + 0.001f : 0.079433f)) : ((iTemp19) ? 0.020567f * fTemp21 + 0.079433f : 0.1f)) : ((iTemp18) ? 0.1f - 0.068377f * fTemp65 : 0.031623f)) : ((iTemp17) ? 0.018496f * fTemp64 + 0.031623f : 0.050119f)) : 0.050119f);
			float fTemp117 = ((iTemp79) ? 0.0f : fRec25[1] + fConst1 * ((iTemp17) ? ((iTemp18) ? ((iTemp19) ? ((iTemp20) ? ((iTemp23) ? ((iTemp26) ? ((iTemp28) ? ((iTemp29) ? ((iTemp31) ? ((iTemp32) ? ((iTemp35) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp41) ? ((iTemp44) ? ((iTemp45) ? ((iTemp46) ? ((iTemp47) ? ((iTemp50) ? 4.95e+03f : ((iTemp47) ? 4.95e+03f - 2.2e+03f * fTemp51 : 2.75e+03f)) : ((iTemp46) ? fTemp111 + 2.75e+03f : 3.1e+03f)) : ((iTemp45) ? 2.4e+02f * fTemp74 + 3.1e+03f : 3.34e+03f)) : ((iTemp44) ? 3.34e+03f - 4.4e+02f * fTemp73 : 2.9e+03f)) : ((iTemp41) ? fTemp93 + 2.9e+03f : 2.95e+03f)) : ((iTemp38) ? 4e+02f * fTemp42 + 2.95e+03f : 3.35e+03f)) : ((iTemp37) ? 3.35e+03f - fTemp81 : 3.3e+03f)) : ((iTemp36) ? 2.9e+02f * fTemp71 + 3.3e+03f : 3.59e+03f)) : ((iTemp35) ? 3.59e+03f - 2.9e+02f * fTemp70 : 3.3e+03f)) : ((iTemp32) ? 1e+02f * fTemp69 + 3.3e+03f : 3.4e+03f)) : ((iTemp31) ? 1.55e+03f * fTemp33 + 3.4e+03f : 4.95e+03f)) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : ((iTemp20) ? 4.95e+03f - 1.7e+03f * fTemp24 : 3.25e+03f)) : ((iTemp19) ? 3.3e+02f * fTemp21 + 3.25e+03f : 3.58e+03f)) : ((iTemp18) ? 3.58e+03f - 4e+01f * fTemp65 : 3.54e+03f)) : ((iTemp17) ? 3.54e+03f - 5.4e+02f * fTemp64 : 3e+03f)) : ((iTemp62) ? 3e+02f * fTemp63 + 3e+03f : 3.3e+03f)));
			fRec25[0] = fTemp117 - std::floor(fTemp117);
			fVec3[0] = fSlow22;
			fRec26[0] = fSlow22 + fRec26[1] * float(fVec3[1] >= fSlow22);
			iRec27[0] = iSlow24 * (iRec27[1] + 1);
			output0[i0] = FAUSTFLOAT(5e+01f * (fRec0[0] * ((iSlow19) ? fTemp78 * fTemp77 : fTemp61 * fTemp77) * (1.0f - (fTemp60 + fTemp59 * (1.0f - fTemp60))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec9[0]), 65535))] + fRec10[0] * ((iSlow19) ? fTemp78 : fTemp61) * (1.0f - (fTemp91 + fTemp90 * (1.0f - fTemp91))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec13[0]), 65535))] + fRec14[0] * ((iSlow19) ? fTemp78 * fTemp102 : fTemp61 * fTemp102) * (1.0f - (fTemp101 + fTemp100 * (1.0f - fTemp101))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec17[0]), 65535))] + fRec18[0] * ((iSlow19) ? fTemp78 * fTemp110 : fTemp61 * fTemp110) * (1.0f - (fTemp108 + fTemp107 * (1.0f - fTemp108))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec21[0]), 65535))] + fRec22[0] * ((iSlow19) ? fTemp78 * fTemp116 : fTemp61 * fTemp116) * (1.0f - (fTemp115 + fTemp114 * (1.0f - fTemp115))) * ftbl0synth_voice_fofsSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec25[0]), 65535))]) * std::max<float>(0.0f, std::min<float>(fSlow21 * fRec26[0], 1.0f) - fSlow23 * float(iRec27[0])));
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
