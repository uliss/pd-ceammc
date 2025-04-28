/* ------------------------------------------------------------
name: "synth.voice_fofc"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_voice_fofc -scn synth_voice_fofc_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_voice_fofc_H__
#define  __synth_voice_fofc_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
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
#define FAUSTVERSION "2.74.3"

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 74
#define FAUSTPATCHVERSION 3

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

class FAUST_API synth_voice_fofc_dsp {

    public:

        synth_voice_fofc_dsp() {}
        virtual ~synth_voice_fofc_dsp() {}

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
        virtual synth_voice_fofc_dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * Read all controllers (buttons, sliders..etc), and update the DSP state to be used by 'frame' or 'compute'.
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

class FAUST_API decorator_dsp : public synth_voice_fofc_dsp {

    protected:

        synth_voice_fofc_dsp* fDSP;

    public:

        decorator_dsp(synth_voice_fofc_dsp* synth_voice_fofc_dsp = nullptr):fDSP(synth_voice_fofc_dsp) {}
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
        virtual synth_voice_fofc_dsp* createDSPInstance() = 0;
    
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
	
	int iVec2[2];
	int iRec4[2];
	
  public:
	
	int getNumInputssynth_voice_fofcSIG0() {
		return 0;
	}
	int getNumOutputssynth_voice_fofcSIG0() {
		return 1;
	}
	
	void instanceInitsynth_voice_fofcSIG0(int sample_rate) {
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iVec2[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iRec4[l7] = 0;
		}
	}
	
	void fillsynth_voice_fofcSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec2[0] = 1;
			iRec4[0] = (iVec2[1] + iRec4[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec4[0]));
			iVec2[1] = iVec2[0];
			iRec4[1] = iRec4[0];
		}
	}

};

static synth_voice_fofcSIG0* newsynth_voice_fofcSIG0() { return (synth_voice_fofcSIG0*)new synth_voice_fofcSIG0(); }
static void deletesynth_voice_fofcSIG0(synth_voice_fofcSIG0* dsp) { delete dsp; }

static float ftbl0synth_voice_fofcSIG0[65536];

class synth_voice_fofc : public synth_voice_fofc_dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fCheckbox0;
	float fVec0[2];
	int iVec1[2];
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	int iRec1[2];
	FAUSTFLOAT fHslider2;
	float fConst2;
	FAUSTFLOAT fVslider0;
	float fConst3;
	float fRec3[2];
	FAUSTFLOAT fHslider3;
	float fConst4;
	float fRec2[2];
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fRec5[2];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHslider7;
	float fRec6[2];
	float fRec9[2];
	float fRec8[2];
	float fVec3[3];
	float fConst5;
	float fRec10[2];
	float fRec11[2];
	float fRec7[3];
	float fRec12[2];
	float fVec4[3];
	float fRec14[2];
	float fRec15[2];
	float fRec13[3];
	float fRec16[2];
	float fVec5[3];
	float fRec18[2];
	float fRec19[2];
	float fRec17[3];
	float fRec20[2];
	float fVec6[3];
	float fRec22[2];
	float fRec23[2];
	float fRec21[3];
	float fRec24[2];
	float fVec7[3];
	float fRec26[2];
	float fRec27[2];
	float fRec25[3];
	float fRec28[2];
	float fVec8[3];
	float fRec30[2];
	float fRec31[2];
	float fRec29[3];
	float fRec32[2];
	float fVec9[3];
	float fRec34[2];
	float fRec35[2];
	float fRec33[3];
	float fRec36[2];
	float fVec10[3];
	float fRec38[2];
	float fRec39[2];
	float fRec37[3];
	float fRec40[2];
	float fVec11[3];
	float fRec42[2];
	float fRec43[2];
	float fRec41[3];
	float fRec44[2];
	float fVec12[3];
	float fRec46[2];
	float fRec47[2];
	float fRec45[3];
	float fRec48[2];
	float fVec13[3];
	float fRec50[2];
	float fRec51[2];
	float fRec49[3];
	float fRec52[2];
	float fVec14[3];
	float fRec54[2];
	float fRec55[2];
	float fRec53[3];
	float fRec56[2];
	float fRec58[2];
	float fRec59[2];
	float fRec57[3];
	float fRec60[2];
	float fRec62[2];
	float fRec63[2];
	float fRec61[3];
	float fRec64[2];
	float fRec66[2];
	float fRec67[2];
	float fRec65[3];
	float fRec68[2];
	float fRec70[2];
	float fRec71[2];
	float fRec69[3];
	float fRec72[2];
	float fRec74[2];
	float fRec75[2];
	float fRec73[3];
	float fRec76[2];
	float fRec78[2];
	float fRec79[2];
	float fRec77[3];
	float fRec80[2];
	float fRec82[2];
	float fRec83[2];
	float fRec81[3];
	float fRec84[2];
	float fRec86[2];
	float fRec87[2];
	float fRec85[3];
	float fRec88[2];
	float fRec90[2];
	float fRec91[2];
	float fRec89[3];
	float fRec92[2];
	float fRec94[2];
	float fRec95[2];
	float fRec93[3];
	float fRec96[2];
	float fRec98[2];
	float fRec99[2];
	float fRec97[3];
	float fRec100[2];
	float fRec102[2];
	float fRec103[2];
	float fRec101[3];
	float fRec104[2];
	float fRec106[2];
	float fRec107[2];
	float fRec105[3];
	float fRec108[2];
	float fRec110[2];
	float fRec111[2];
	float fRec109[3];
	float fRec112[2];
	float fRec114[2];
	float fRec115[2];
	float fRec113[3];
	float fRec116[2];
	float fRec118[2];
	float fRec119[2];
	float fRec117[3];
	float fRec120[2];
	float fRec122[2];
	float fRec123[2];
	float fRec121[3];
	float fRec124[2];
	float fRec126[2];
	float fRec127[2];
	float fRec125[3];
	float fRec128[2];
	float fRec130[2];
	float fRec131[2];
	float fRec129[3];
	float fRec132[2];
	float fRec134[2];
	float fRec135[2];
	float fRec133[3];
	float fRec136[2];
	float fRec138[2];
	float fRec139[2];
	float fRec137[3];
	float fRec140[2];
	float fRec142[2];
	float fRec143[2];
	float fRec141[3];
	float fRec144[2];
	float fRec146[2];
	float fRec147[2];
	float fRec145[3];
	float fRec148[2];
	float fRec150[2];
	float fRec151[2];
	float fRec149[3];
	float fRec152[2];
	float fRec154[2];
	float fRec155[2];
	float fRec153[3];
	float fRec156[2];
	float fRec158[2];
	float fRec159[2];
	float fRec157[3];
	float fRec160[2];
	float fRec162[2];
	float fRec163[2];
	float fRec161[3];
	float fRec164[2];
	float fRec166[2];
	float fRec167[2];
	float fRec165[3];
	float fRec168[2];
	float fRec170[2];
	float fRec171[2];
	float fRec169[3];
	float fRec172[2];
	float fRec174[2];
	float fRec175[2];
	float fRec173[3];
	float fRec176[2];
	float fRec178[2];
	float fRec179[2];
	float fRec177[3];
	float fRec180[2];
	float fRec182[2];
	float fRec183[2];
	float fRec181[3];
	float fRec184[2];
	float fRec186[2];
	float fRec187[2];
	float fRec185[3];
	float fRec188[2];
	float fRec190[2];
	float fRec191[2];
	float fRec189[3];
	float fRec192[2];
	float fRec194[2];
	float fRec195[2];
	float fRec193[3];
	float fRec196[2];
	float fRec198[2];
	float fRec199[2];
	float fRec197[3];
	float fRec200[2];
	float fRec202[2];
	float fRec203[2];
	float fRec201[3];
	float fRec204[2];
	float fRec206[2];
	float fRec207[2];
	float fRec205[3];
	float fRec208[2];
	float fRec210[2];
	float fRec211[2];
	float fRec209[3];
	float fRec212[2];
	float fRec214[2];
	float fRec215[2];
	float fRec213[3];
	float fRec216[2];
	float fRec218[2];
	float fRec219[2];
	float fRec217[3];
	float fRec220[2];
	float fRec222[2];
	float fRec223[2];
	float fRec221[3];
	float fRec224[2];
	float fRec226[2];
	float fRec227[2];
	float fRec225[3];
	float fRec228[2];
	float fRec230[2];
	float fRec231[2];
	float fRec229[3];
	float fRec232[2];
	float fRec234[2];
	float fRec235[2];
	float fRec233[3];
	float fRec236[2];
	float fRec238[2];
	float fRec239[2];
	float fRec237[3];
	float fRec240[2];
	float fRec242[2];
	float fRec243[2];
	float fRec241[3];
	float fRec244[2];
	float fRec246[2];
	float fRec247[2];
	float fRec245[3];
	float fRec248[2];
	
 public:
	synth_voice_fofc() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/cycle:author", "Mike Olsen");
		m->declare("basics.lib/listInterp:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/pulse_countup_loop:author", "Vince");
		m->declare("basics.lib/resetCtr:author", "Mike Olsen");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.16.0");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_voice_fofc -scn synth_voice_fofc_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
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
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "synth.voice_fofc");
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
		m->declare("physmodels.lib/fofCycle:author", "Mike Olsen");
		m->declare("physmodels.lib/fofSH:author", "Mike Olsen");
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
		synth_voice_fofcSIG0* sig0 = newsynth_voice_fofcSIG0();
		sig0->instanceInitsynth_voice_fofcSIG0(sample_rate);
		sig0->fillsynth_voice_fofcSIG0(65536, ftbl0synth_voice_fofcSIG0);
		deletesynth_voice_fofcSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 0.001f * fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 1.0f / fConst0;
		fConst5 = 3.1415927f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(3e+01f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(5e+01f);
		fHslider2 = FAUSTFLOAT(0.0f);
		fVslider0 = FAUSTFLOAT(4.4e+02f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.5f);
		fHslider5 = FAUSTFLOAT(6.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(1e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iVec1[l1] = 0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec1[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec2[l5] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec5[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec6[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec8[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fVec3[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec10[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fRec7[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec12[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fVec4[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec14[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec15[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fRec13[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec16[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fVec5[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec18[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec19[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = l25 + 1) {
			fRec17[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec20[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fVec6[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec22[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec23[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec21[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec24[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 3; l32 = l32 + 1) {
			fVec7[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec26[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec27[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fRec25[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec28[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fVec8[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec30[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec31[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 3; l40 = l40 + 1) {
			fRec29[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec32[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fVec9[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec34[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec35[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec33[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec36[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fVec10[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec38[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec39[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = l50 + 1) {
			fRec37[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec40[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 3; l52 = l52 + 1) {
			fVec11[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec42[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec43[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 3; l55 = l55 + 1) {
			fRec41[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec44[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fVec12[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec46[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec47[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec45[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec48[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fVec13[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec50[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec51[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 3; l65 = l65 + 1) {
			fRec49[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec52[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 3; l67 = l67 + 1) {
			fVec14[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec54[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fRec55[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fRec53[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec56[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec58[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec59[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 3; l74 = l74 + 1) {
			fRec57[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec60[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec62[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec63[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fRec61[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec64[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec66[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec67[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 3; l82 = l82 + 1) {
			fRec65[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec68[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fRec70[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec71[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec69[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec72[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec74[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec75[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 3; l90 = l90 + 1) {
			fRec73[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			fRec76[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec78[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec79[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 3; l94 = l94 + 1) {
			fRec77[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fRec80[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec82[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec83[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 3; l98 = l98 + 1) {
			fRec81[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			fRec84[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			fRec86[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec87[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 3; l102 = l102 + 1) {
			fRec85[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec88[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			fRec90[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fRec91[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 3; l106 = l106 + 1) {
			fRec89[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 2; l107 = l107 + 1) {
			fRec92[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec94[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			fRec95[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 3; l110 = l110 + 1) {
			fRec93[l110] = 0.0f;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			fRec96[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec98[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec99[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 3; l114 = l114 + 1) {
			fRec97[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fRec100[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			fRec102[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fRec103[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 3; l118 = l118 + 1) {
			fRec101[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 2; l119 = l119 + 1) {
			fRec104[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fRec106[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			fRec107[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 3; l122 = l122 + 1) {
			fRec105[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fRec108[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			fRec110[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec111[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 3; l126 = l126 + 1) {
			fRec109[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec112[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			fRec114[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fRec115[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 3; l130 = l130 + 1) {
			fRec113[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
			fRec116[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec118[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fRec119[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 3; l134 = l134 + 1) {
			fRec117[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fRec120[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
			fRec122[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 2; l137 = l137 + 1) {
			fRec123[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 3; l138 = l138 + 1) {
			fRec121[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
			fRec124[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
			fRec126[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 2; l141 = l141 + 1) {
			fRec127[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 3; l142 = l142 + 1) {
			fRec125[l142] = 0.0f;
		}
		for (int l143 = 0; l143 < 2; l143 = l143 + 1) {
			fRec128[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fRec130[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec131[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 3; l146 = l146 + 1) {
			fRec129[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 2; l147 = l147 + 1) {
			fRec132[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fRec134[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fRec135[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 3; l150 = l150 + 1) {
			fRec133[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			fRec136[l151] = 0.0f;
		}
		for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
			fRec138[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
			fRec139[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 3; l154 = l154 + 1) {
			fRec137[l154] = 0.0f;
		}
		for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
			fRec140[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec142[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
			fRec143[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 3; l158 = l158 + 1) {
			fRec141[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 2; l159 = l159 + 1) {
			fRec144[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fRec146[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 2; l161 = l161 + 1) {
			fRec147[l161] = 0.0f;
		}
		for (int l162 = 0; l162 < 3; l162 = l162 + 1) {
			fRec145[l162] = 0.0f;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec148[l163] = 0.0f;
		}
		for (int l164 = 0; l164 < 2; l164 = l164 + 1) {
			fRec150[l164] = 0.0f;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fRec151[l165] = 0.0f;
		}
		for (int l166 = 0; l166 < 3; l166 = l166 + 1) {
			fRec149[l166] = 0.0f;
		}
		for (int l167 = 0; l167 < 2; l167 = l167 + 1) {
			fRec152[l167] = 0.0f;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fRec154[l168] = 0.0f;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fRec155[l169] = 0.0f;
		}
		for (int l170 = 0; l170 < 3; l170 = l170 + 1) {
			fRec153[l170] = 0.0f;
		}
		for (int l171 = 0; l171 < 2; l171 = l171 + 1) {
			fRec156[l171] = 0.0f;
		}
		for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
			fRec158[l172] = 0.0f;
		}
		for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
			fRec159[l173] = 0.0f;
		}
		for (int l174 = 0; l174 < 3; l174 = l174 + 1) {
			fRec157[l174] = 0.0f;
		}
		for (int l175 = 0; l175 < 2; l175 = l175 + 1) {
			fRec160[l175] = 0.0f;
		}
		for (int l176 = 0; l176 < 2; l176 = l176 + 1) {
			fRec162[l176] = 0.0f;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			fRec163[l177] = 0.0f;
		}
		for (int l178 = 0; l178 < 3; l178 = l178 + 1) {
			fRec161[l178] = 0.0f;
		}
		for (int l179 = 0; l179 < 2; l179 = l179 + 1) {
			fRec164[l179] = 0.0f;
		}
		for (int l180 = 0; l180 < 2; l180 = l180 + 1) {
			fRec166[l180] = 0.0f;
		}
		for (int l181 = 0; l181 < 2; l181 = l181 + 1) {
			fRec167[l181] = 0.0f;
		}
		for (int l182 = 0; l182 < 3; l182 = l182 + 1) {
			fRec165[l182] = 0.0f;
		}
		for (int l183 = 0; l183 < 2; l183 = l183 + 1) {
			fRec168[l183] = 0.0f;
		}
		for (int l184 = 0; l184 < 2; l184 = l184 + 1) {
			fRec170[l184] = 0.0f;
		}
		for (int l185 = 0; l185 < 2; l185 = l185 + 1) {
			fRec171[l185] = 0.0f;
		}
		for (int l186 = 0; l186 < 3; l186 = l186 + 1) {
			fRec169[l186] = 0.0f;
		}
		for (int l187 = 0; l187 < 2; l187 = l187 + 1) {
			fRec172[l187] = 0.0f;
		}
		for (int l188 = 0; l188 < 2; l188 = l188 + 1) {
			fRec174[l188] = 0.0f;
		}
		for (int l189 = 0; l189 < 2; l189 = l189 + 1) {
			fRec175[l189] = 0.0f;
		}
		for (int l190 = 0; l190 < 3; l190 = l190 + 1) {
			fRec173[l190] = 0.0f;
		}
		for (int l191 = 0; l191 < 2; l191 = l191 + 1) {
			fRec176[l191] = 0.0f;
		}
		for (int l192 = 0; l192 < 2; l192 = l192 + 1) {
			fRec178[l192] = 0.0f;
		}
		for (int l193 = 0; l193 < 2; l193 = l193 + 1) {
			fRec179[l193] = 0.0f;
		}
		for (int l194 = 0; l194 < 3; l194 = l194 + 1) {
			fRec177[l194] = 0.0f;
		}
		for (int l195 = 0; l195 < 2; l195 = l195 + 1) {
			fRec180[l195] = 0.0f;
		}
		for (int l196 = 0; l196 < 2; l196 = l196 + 1) {
			fRec182[l196] = 0.0f;
		}
		for (int l197 = 0; l197 < 2; l197 = l197 + 1) {
			fRec183[l197] = 0.0f;
		}
		for (int l198 = 0; l198 < 3; l198 = l198 + 1) {
			fRec181[l198] = 0.0f;
		}
		for (int l199 = 0; l199 < 2; l199 = l199 + 1) {
			fRec184[l199] = 0.0f;
		}
		for (int l200 = 0; l200 < 2; l200 = l200 + 1) {
			fRec186[l200] = 0.0f;
		}
		for (int l201 = 0; l201 < 2; l201 = l201 + 1) {
			fRec187[l201] = 0.0f;
		}
		for (int l202 = 0; l202 < 3; l202 = l202 + 1) {
			fRec185[l202] = 0.0f;
		}
		for (int l203 = 0; l203 < 2; l203 = l203 + 1) {
			fRec188[l203] = 0.0f;
		}
		for (int l204 = 0; l204 < 2; l204 = l204 + 1) {
			fRec190[l204] = 0.0f;
		}
		for (int l205 = 0; l205 < 2; l205 = l205 + 1) {
			fRec191[l205] = 0.0f;
		}
		for (int l206 = 0; l206 < 3; l206 = l206 + 1) {
			fRec189[l206] = 0.0f;
		}
		for (int l207 = 0; l207 < 2; l207 = l207 + 1) {
			fRec192[l207] = 0.0f;
		}
		for (int l208 = 0; l208 < 2; l208 = l208 + 1) {
			fRec194[l208] = 0.0f;
		}
		for (int l209 = 0; l209 < 2; l209 = l209 + 1) {
			fRec195[l209] = 0.0f;
		}
		for (int l210 = 0; l210 < 3; l210 = l210 + 1) {
			fRec193[l210] = 0.0f;
		}
		for (int l211 = 0; l211 < 2; l211 = l211 + 1) {
			fRec196[l211] = 0.0f;
		}
		for (int l212 = 0; l212 < 2; l212 = l212 + 1) {
			fRec198[l212] = 0.0f;
		}
		for (int l213 = 0; l213 < 2; l213 = l213 + 1) {
			fRec199[l213] = 0.0f;
		}
		for (int l214 = 0; l214 < 3; l214 = l214 + 1) {
			fRec197[l214] = 0.0f;
		}
		for (int l215 = 0; l215 < 2; l215 = l215 + 1) {
			fRec200[l215] = 0.0f;
		}
		for (int l216 = 0; l216 < 2; l216 = l216 + 1) {
			fRec202[l216] = 0.0f;
		}
		for (int l217 = 0; l217 < 2; l217 = l217 + 1) {
			fRec203[l217] = 0.0f;
		}
		for (int l218 = 0; l218 < 3; l218 = l218 + 1) {
			fRec201[l218] = 0.0f;
		}
		for (int l219 = 0; l219 < 2; l219 = l219 + 1) {
			fRec204[l219] = 0.0f;
		}
		for (int l220 = 0; l220 < 2; l220 = l220 + 1) {
			fRec206[l220] = 0.0f;
		}
		for (int l221 = 0; l221 < 2; l221 = l221 + 1) {
			fRec207[l221] = 0.0f;
		}
		for (int l222 = 0; l222 < 3; l222 = l222 + 1) {
			fRec205[l222] = 0.0f;
		}
		for (int l223 = 0; l223 < 2; l223 = l223 + 1) {
			fRec208[l223] = 0.0f;
		}
		for (int l224 = 0; l224 < 2; l224 = l224 + 1) {
			fRec210[l224] = 0.0f;
		}
		for (int l225 = 0; l225 < 2; l225 = l225 + 1) {
			fRec211[l225] = 0.0f;
		}
		for (int l226 = 0; l226 < 3; l226 = l226 + 1) {
			fRec209[l226] = 0.0f;
		}
		for (int l227 = 0; l227 < 2; l227 = l227 + 1) {
			fRec212[l227] = 0.0f;
		}
		for (int l228 = 0; l228 < 2; l228 = l228 + 1) {
			fRec214[l228] = 0.0f;
		}
		for (int l229 = 0; l229 < 2; l229 = l229 + 1) {
			fRec215[l229] = 0.0f;
		}
		for (int l230 = 0; l230 < 3; l230 = l230 + 1) {
			fRec213[l230] = 0.0f;
		}
		for (int l231 = 0; l231 < 2; l231 = l231 + 1) {
			fRec216[l231] = 0.0f;
		}
		for (int l232 = 0; l232 < 2; l232 = l232 + 1) {
			fRec218[l232] = 0.0f;
		}
		for (int l233 = 0; l233 < 2; l233 = l233 + 1) {
			fRec219[l233] = 0.0f;
		}
		for (int l234 = 0; l234 < 3; l234 = l234 + 1) {
			fRec217[l234] = 0.0f;
		}
		for (int l235 = 0; l235 < 2; l235 = l235 + 1) {
			fRec220[l235] = 0.0f;
		}
		for (int l236 = 0; l236 < 2; l236 = l236 + 1) {
			fRec222[l236] = 0.0f;
		}
		for (int l237 = 0; l237 < 2; l237 = l237 + 1) {
			fRec223[l237] = 0.0f;
		}
		for (int l238 = 0; l238 < 3; l238 = l238 + 1) {
			fRec221[l238] = 0.0f;
		}
		for (int l239 = 0; l239 < 2; l239 = l239 + 1) {
			fRec224[l239] = 0.0f;
		}
		for (int l240 = 0; l240 < 2; l240 = l240 + 1) {
			fRec226[l240] = 0.0f;
		}
		for (int l241 = 0; l241 < 2; l241 = l241 + 1) {
			fRec227[l241] = 0.0f;
		}
		for (int l242 = 0; l242 < 3; l242 = l242 + 1) {
			fRec225[l242] = 0.0f;
		}
		for (int l243 = 0; l243 < 2; l243 = l243 + 1) {
			fRec228[l243] = 0.0f;
		}
		for (int l244 = 0; l244 < 2; l244 = l244 + 1) {
			fRec230[l244] = 0.0f;
		}
		for (int l245 = 0; l245 < 2; l245 = l245 + 1) {
			fRec231[l245] = 0.0f;
		}
		for (int l246 = 0; l246 < 3; l246 = l246 + 1) {
			fRec229[l246] = 0.0f;
		}
		for (int l247 = 0; l247 < 2; l247 = l247 + 1) {
			fRec232[l247] = 0.0f;
		}
		for (int l248 = 0; l248 < 2; l248 = l248 + 1) {
			fRec234[l248] = 0.0f;
		}
		for (int l249 = 0; l249 < 2; l249 = l249 + 1) {
			fRec235[l249] = 0.0f;
		}
		for (int l250 = 0; l250 < 3; l250 = l250 + 1) {
			fRec233[l250] = 0.0f;
		}
		for (int l251 = 0; l251 < 2; l251 = l251 + 1) {
			fRec236[l251] = 0.0f;
		}
		for (int l252 = 0; l252 < 2; l252 = l252 + 1) {
			fRec238[l252] = 0.0f;
		}
		for (int l253 = 0; l253 < 2; l253 = l253 + 1) {
			fRec239[l253] = 0.0f;
		}
		for (int l254 = 0; l254 < 3; l254 = l254 + 1) {
			fRec237[l254] = 0.0f;
		}
		for (int l255 = 0; l255 < 2; l255 = l255 + 1) {
			fRec240[l255] = 0.0f;
		}
		for (int l256 = 0; l256 < 2; l256 = l256 + 1) {
			fRec242[l256] = 0.0f;
		}
		for (int l257 = 0; l257 < 2; l257 = l257 + 1) {
			fRec243[l257] = 0.0f;
		}
		for (int l258 = 0; l258 < 3; l258 = l258 + 1) {
			fRec241[l258] = 0.0f;
		}
		for (int l259 = 0; l259 < 2; l259 = l259 + 1) {
			fRec244[l259] = 0.0f;
		}
		for (int l260 = 0; l260 < 2; l260 = l260 + 1) {
			fRec246[l260] = 0.0f;
		}
		for (int l261 = 0; l261 < 2; l261 = l261 + 1) {
			fRec247[l261] = 0.0f;
		}
		for (int l262 = 0; l262 < 3; l262 = l262 + 1) {
			fRec245[l262] = 0.0f;
		}
		for (int l263 = 0; l263 < 2; l263 = l263 + 1) {
			fRec248[l263] = 0.0f;
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
		return new synth_voice_fofc();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.voice_fofc");
		ui_interface->declare(&fHslider0, "unit", "ms");
		ui_interface->addHorizontalSlider("attack", &fHslider0, FAUSTFLOAT(3e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("freq", &fVslider0, FAUSTFLOAT(4.4e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("fvowel", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->addHorizontalSlider("ivoice", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider3, "unit", "ms");
		ui_interface->addHorizontalSlider("portamento", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4e+03f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider1, "unit", "ms");
		ui_interface->addHorizontalSlider("release", &fHslider1, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider5, "unit", "Hz");
		ui_interface->addHorizontalSlider("vibr_freq", &fHslider5, FAUSTFLOAT(6.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("vibr_gain", &fHslider4, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider7, "unit", "ms");
		ui_interface->addHorizontalSlider("wsmooth", &fHslider7, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = 1.0f / std::max<float>(1.0f, fConst1 * float(fHslider0));
		float fSlow1 = float(fCheckbox0);
		float fSlow2 = 1.0f / std::max<float>(1.0f, fConst1 * float(fHslider1));
		int iSlow3 = fSlow1 == 0.0f;
		int iSlow4 = int(float(fHslider2));
		int iSlow5 = ((iSlow4 == 0) ? 1 : ((iSlow4 == 3) ? 1 : 0));
		int iSlow6 = iSlow5 == 0;
		float fSlow7 = fConst2 * float(fVslider0);
		float fSlow8 = 0.001f * float(fHslider3);
		int iSlow9 = std::fabs(fSlow8) < 1.1920929e-07f;
		float fSlow10 = ((iSlow9) ? 0.0f : std::exp(-(fConst4 / ((iSlow9) ? 1.0f : fSlow8))));
		float fSlow11 = 1.0f - fSlow10;
		float fSlow12 = 0.1f * float(fHslider4);
		float fSlow13 = fConst4 * float(fHslider5);
		float fSlow14 = float(5 * iSlow4);
		float fSlow15 = 0.001f * float(fHslider7);
		int iSlow16 = std::fabs(fSlow15) < 1.1920929e-07f;
		float fSlow17 = ((iSlow16) ? 0.0f : std::exp(-(fConst4 / ((iSlow16) ? 1.0f : fSlow15))));
		float fSlow18 = float(fHslider6) * (1.0f - fSlow17);
		float fSlow19 = float(5 * iSlow5);
		int iSlow20 = iSlow5 >= 1;
		float fSlow21 = ((iSlow20) ? 174.61f : 82.41f);
		float fSlow22 = ((iSlow20) ? 1046.5f : 523.25f);
		float fSlow23 = fSlow22 - fSlow21;
		int iSlow24 = iSlow4 != 2;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fVec0[0] = fSlow1;
			iVec1[0] = 1;
			fRec0[0] = fSlow1 + fRec0[1] * float(fVec0[1] >= fSlow1);
			iRec1[0] = iSlow3 * (iRec1[1] + 1);
			fRec3[0] = fSlow7 + fConst3 * fRec3[1];
			fRec2[0] = fRec3[0] * fSlow11 + fSlow10 * fRec2[1];
			int iTemp0 = 1 - iVec1[1];
			float fTemp1 = ((iTemp0) ? 0.0f : fSlow13 + fRec5[1]);
			fRec5[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = fRec2[0] * (fSlow12 * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec5[0]), 65535))] + 1.0f);
			float fTemp3 = 0.00084f * (1e+03f - fTemp2) + 0.8f;
			fRec6[0] = fSlow18 + fSlow17 * fRec6[1];
			float fTemp4 = fSlow14 + fRec6[0];
			int iTemp5 = fTemp4 < 23.0f;
			int iTemp6 = fTemp4 < 24.0f;
			float fTemp7 = fSlow14 + fRec6[0] + -23.0f;
			int iTemp8 = fTemp4 < 22.0f;
			float fTemp9 = fSlow14 + fRec6[0] + -22.0f;
			int iTemp10 = fTemp4 < 21.0f;
			float fTemp11 = fSlow14 + fRec6[0] + -21.0f;
			int iTemp12 = fTemp4 < 2e+01f;
			float fTemp13 = fSlow14 + fRec6[0] + -2e+01f;
			int iTemp14 = fTemp4 < 19.0f;
			float fTemp15 = fSlow14 + fRec6[0] + -19.0f;
			int iTemp16 = fTemp4 < 18.0f;
			float fTemp17 = fSlow14 + fRec6[0] + -18.0f;
			int iTemp18 = fTemp4 < 17.0f;
			float fTemp19 = fSlow14 + fRec6[0] + -17.0f;
			float fTemp20 = ((iTemp16) ? 0.029314f * fTemp19 + 0.050119f : 0.079433f);
			int iTemp21 = fTemp4 < 16.0f;
			float fTemp22 = fSlow14 + fRec6[0] + -16.0f;
			int iTemp23 = fTemp4 < 15.0f;
			float fTemp24 = fSlow14 + fRec6[0] + -15.0f;
			int iTemp25 = fTemp4 < 14.0f;
			float fTemp26 = fSlow14 + fRec6[0] + -14.0f;
			int iTemp27 = fTemp4 < 13.0f;
			float fTemp28 = fSlow14 + fRec6[0] + -13.0f;
			int iTemp29 = fTemp4 < 12.0f;
			float fTemp30 = fSlow14 + fRec6[0] + -12.0f;
			int iTemp31 = fTemp4 < 11.0f;
			float fTemp32 = fSlow14 + fRec6[0] + -11.0f;
			float fTemp33 = ((iTemp29) ? 0.1f - 0.084151f * fTemp32 : 0.015849f);
			int iTemp34 = fTemp4 < 1e+01f;
			float fTemp35 = fSlow14 + fRec6[0] + -1e+01f;
			int iTemp36 = fTemp4 < 9.0f;
			float fTemp37 = fSlow14 + fRec6[0] + -9.0f;
			int iTemp38 = fTemp4 < 8.0f;
			float fTemp39 = fSlow14 + fRec6[0] + -8.0f;
			int iTemp40 = fTemp4 < 7.0f;
			float fTemp41 = fSlow14 + fRec6[0] + -7.0f;
			int iTemp42 = fTemp4 < 6.0f;
			float fTemp43 = fSlow14 + fRec6[0] + -6.0f;
			int iTemp44 = fTemp4 < 5.0f;
			float fTemp45 = fSlow14 + fRec6[0] + -5.0f;
			int iTemp46 = fTemp4 < 4.0f;
			float fTemp47 = fSlow14 + fRec6[0] + -4.0f;
			int iTemp48 = fTemp4 < 3.0f;
			float fTemp49 = fSlow14 + fRec6[0] + -3.0f;
			int iTemp50 = fTemp4 < 2.0f;
			float fTemp51 = fSlow14 + fRec6[0] + -2.0f;
			int iTemp52 = fTemp4 < 1.0f;
			float fTemp53 = fSlow14 + fRec6[0] + -1.0f;
			int iTemp54 = fTemp4 < 0.0f;
			float fTemp55 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 0.015849f : ((iTemp52) ? 0.001934f * fTemp4 + 0.015849f : 0.017783f)) : ((iTemp50) ? 0.017783f - 0.001934f * fTemp53 : 0.015849f)) : ((iTemp48) ? 0.023962f * fTemp51 + 0.015849f : 0.039811f)) : ((iTemp46) ? 0.039811f - 0.029811f * fTemp49 : 0.01f)) : ((iTemp44) ? 0.344813f * fTemp47 + 0.01f : 0.354813f)) : ((iTemp42) ? 0.354813f - 0.103624f * fTemp45 : 0.251189f)) : ((iTemp40) ? 0.251189f - 0.171756f * fTemp43 : 0.079433f)) : ((iTemp38) ? 0.020567f * fTemp41 + 0.079433f : 0.1f)) : ((iTemp36) ? 0.1f - 0.060189f * fTemp39 : 0.039811f)) : ((iTemp34) ? 0.023285f * fTemp37 + 0.039811f : 0.063096f)) : ((iTemp31) ? 0.036904f * fTemp35 + 0.063096f : 0.1f)) : fTemp33) : ((iTemp27) ? 0.063584f * fTemp30 + 0.015849f : 0.079433f)) : ((iTemp25) ? 0.079433f - 0.04781f * fTemp28 : 0.031623f)) : ((iTemp23) ? 0.068377f * fTemp26 + 0.031623f : 0.1f)) : ((iTemp21) ? 0.1f - 0.09f * fTemp24 : 0.01f)) : ((iTemp18) ? 0.040119f * fTemp22 + 0.01f : 0.050119f)) : fTemp20) : ((iTemp14) ? 0.079433f - 0.069433f * fTemp17 : 0.01f)) : ((iTemp12) ? 0.388107f * fTemp15 + 0.01f : 0.398107f)) : ((iTemp10) ? 0.398107f - 0.198581f * fTemp13 : 0.199526f)) : ((iTemp8) ? 0.199526f - 0.099526f * fTemp11 : 0.1f)) : ((iTemp5) ? 0.151189f * fTemp9 + 0.1f : 0.251189f)) : ((iTemp6) ? 0.251189f - 0.051663f * fTemp7 : 0.199526f));
			float fTemp56 = 0.0036666666f * (4e+02f - fTemp2) + 3.0f;
			float fTemp57 = ((iTemp0) ? 0.0f : fRec9[1] + fConst4 * fTemp2);
			fRec9[0] = fTemp57 - std::floor(fTemp57);
			float fTemp58 = float((fRec9[0] - fRec9[1]) < 0.0f);
			fRec8[0] = fTemp58 + fRec8[1] * float(fRec8[1] <= 11.0f);
			float fTemp59 = float(fRec8[0] == 9.0f) * fTemp58;
			fVec3[0] = fTemp59;
			int iTemp60 = int(fTemp59);
			float fTemp61 = fRec6[0] + fSlow19;
			int iTemp62 = fTemp61 >= 5.0f;
			int iTemp63 = fTemp61 >= 3.0f;
			int iTemp64 = fTemp61 >= 2.0f;
			int iTemp65 = fTemp61 >= 1.0f;
			int iTemp66 = fTemp61 >= 4.0f;
			int iTemp67 = fTemp61 >= 8.0f;
			int iTemp68 = fTemp61 >= 7.0f;
			int iTemp69 = fTemp61 >= 6.0f;
			float fTemp70 = ((iTemp62) ? ((iTemp67) ? 2.0f : ((iTemp68) ? 3.0f : ((iTemp69) ? 3.0f : 2.0f))) : ((iTemp63) ? ((iTemp66) ? 1.5f : 1.0f) : ((iTemp64) ? 1.25f : ((iTemp65) ? 1.25f : 1.0f))));
			float fTemp71 = fTemp70 + (((iTemp62) ? ((iTemp67) ? 12.0f : ((iTemp68) ? 12.0f : ((iTemp69) ? 12.0f : 15.0f))) : ((iTemp63) ? ((iTemp66) ? 4.0f : 1e+01f) : ((iTemp64) ? 2.5f : ((iTemp65) ? 2.5f : 1e+01f)))) - fTemp70) * ((fTemp2 <= fSlow21) ? 0.0f : ((fTemp2 >= fSlow22) ? 1.0f : (fTemp2 - fSlow21) / fSlow23));
			float fTemp72 = 1e+01f * fTemp9;
			float fTemp73 = 1e+01f * fTemp13;
			float fTemp74 = 5e+01f * fTemp15;
			float fTemp75 = 2e+01f * fTemp24;
			float fTemp76 = 1e+01f * fTemp35;
			float fTemp77 = 1e+01f * fTemp37;
			float fTemp78 = 6e+01f * fTemp47;
			float fTemp79 = 2e+01f * fTemp51;
			float fTemp80 = 2e+01f * fTemp4;
			float fTemp81 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 1.3e+02f : ((iTemp52) ? fTemp80 + 1.3e+02f : 1.5e+02f)) : 1.5e+02f) : ((iTemp48) ? 1.5e+02f - fTemp79 : 1.3e+02f)) : ((iTemp46) ? 5e+01f * fTemp49 + 1.3e+02f : 1.8e+02f)) : ((iTemp44) ? 1.8e+02f - fTemp78 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp34) ? fTemp77 + 1.2e+02f : 1.3e+02f)) : ((iTemp31) ? 1.3e+02f - fTemp76 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp23) ? 1e+01f * fTemp26 + 1.2e+02f : 1.3e+02f)) : ((iTemp21) ? fTemp75 + 1.3e+02f : 1.5e+02f)) : ((iTemp18) ? 1.5e+02f - 3e+01f * fTemp22 : 1.2e+02f)) : 1.2e+02f) : ((iTemp14) ? 6e+01f * fTemp17 + 1.2e+02f : 1.8e+02f)) : ((iTemp12) ? 1.8e+02f - fTemp74 : 1.3e+02f)) : ((iTemp10) ? 1.3e+02f - fTemp73 : 1.2e+02f)) : 1.2e+02f) : ((iTemp5) ? fTemp72 + 1.2e+02f : 1.3e+02f)) : ((iTemp6) ? 1.3e+02f - 1e+01f * fTemp7 : 1.2e+02f));
			float fTemp82 = fTemp71 * fTemp81;
			fRec10[0] = ((iTemp60) ? fTemp82 : fRec10[1]);
			float fTemp83 = std::exp(-(fConst5 * fRec10[0]));
			fRec11[0] = ((iTemp60) ? fTemp81 : fRec11[1]);
			float fTemp84 = std::exp(-(fConst5 * fRec11[0]));
			fRec7[0] = fVec3[2] + fRec7[1] * (fTemp83 + fTemp84) - fTemp83 * fTemp84 * fRec7[2];
			int iTemp85 = iTemp0 | int(fVec3[1]);
			float fTemp86 = 1e+02f * fTemp7;
			float fTemp87 = 3.5e+02f * fTemp45;
			float fTemp88 = fConst4 * ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 3.5e+03f : ((iTemp52) ? 3.5e+03f - 2e+02f * fTemp4 : 3.3e+03f)) : ((iTemp50) ? 4e+02f * fTemp53 + 3.3e+03f : 3.7e+03f)) : ((iTemp48) ? 3.7e+03f - 2e+02f * fTemp51 : 3.5e+03f)) : 3.5e+03f) : ((iTemp44) ? 3.5e+03f - 1.05e+03f * fTemp47 : 2.45e+03f)) : ((iTemp42) ? fTemp87 + 2.45e+03f : 2.8e+03f)) : ((iTemp40) ? 2.5e+02f * fTemp43 + 2.8e+03f : 3.05e+03f)) : ((iTemp38) ? 3.05e+03f - 4.5e+02f * fTemp41 : 2.6e+03f)) : ((iTemp36) ? 75.0f * fTemp39 + 2.6e+03f : 2675.0f)) : ((iTemp34) ? 325.0f * fTemp37 + 2675.0f : 3e+03f)) : 3e+03f) : ((iTemp29) ? 3.5e+02f * fTemp32 + 3e+03f : 3.35e+03f)) : ((iTemp27) ? 3.35e+03f - 3.5e+02f * fTemp30 : 3e+03f)) : 3e+03f) : ((iTemp23) ? 9e+02f * fTemp26 + 3e+03f : 3.9e+03f)) : ((iTemp21) ? 3.9e+03f - 3e+02f * fTemp24 : 3.6e+03f)) : ((iTemp18) ? 3e+02f * fTemp22 + 3.6e+03f : 3.9e+03f)) : ((iTemp16) ? 3.9e+03f - 1e+02f * fTemp19 : 3.8e+03f)) : 3.8e+03f) : ((iTemp12) ? 3.8e+03f - 9e+02f * fTemp15 : 2.9e+03f)) : ((iTemp10) ? 3e+02f * fTemp13 + 2.9e+03f : 3.2e+03f)) : ((iTemp8) ? 5e+01f * fTemp11 + 3.2e+03f : 3.25e+03f)) : ((iTemp5) ? 3.25e+03f - 4.5e+02f * fTemp9 : 2.8e+03f)) : ((iTemp6) ? fTemp86 + 2.8e+03f : 2.9e+03f));
			float fTemp89 = ((iTemp85) ? 0.0f : fTemp88 + fRec12[1]);
			fRec12[0] = fTemp89 - std::floor(fTemp89);
			float fTemp90 = float(fRec8[0] == 8.0f) * fTemp58;
			fVec4[0] = fTemp90;
			int iTemp91 = int(fTemp90);
			fRec14[0] = ((iTemp91) ? fTemp82 : fRec14[1]);
			float fTemp92 = std::exp(-(fConst5 * fRec14[0]));
			fRec15[0] = ((iTemp91) ? fTemp81 : fRec15[1]);
			float fTemp93 = std::exp(-(fConst5 * fRec15[0]));
			fRec13[0] = fVec4[2] + fRec13[1] * (fTemp92 + fTemp93) - fTemp92 * fTemp93 * fRec13[2];
			int iTemp94 = iTemp0 | int(fVec4[1]);
			float fTemp95 = ((iTemp94) ? 0.0f : fTemp88 + fRec16[1]);
			fRec16[0] = fTemp95 - std::floor(fTemp95);
			float fTemp96 = float(fRec8[0] == 7.0f) * fTemp58;
			fVec5[0] = fTemp96;
			int iTemp97 = int(fTemp96);
			fRec18[0] = ((iTemp97) ? fTemp82 : fRec18[1]);
			float fTemp98 = std::exp(-(fConst5 * fRec18[0]));
			fRec19[0] = ((iTemp97) ? fTemp81 : fRec19[1]);
			float fTemp99 = std::exp(-(fConst5 * fRec19[0]));
			fRec17[0] = fVec5[2] + fRec17[1] * (fTemp98 + fTemp99) - fTemp98 * fTemp99 * fRec17[2];
			int iTemp100 = iTemp0 | int(fVec5[1]);
			float fTemp101 = ((iTemp100) ? 0.0f : fTemp88 + fRec20[1]);
			fRec20[0] = fTemp101 - std::floor(fTemp101);
			float fTemp102 = float(fRec8[0] == 6.0f) * fTemp58;
			fVec6[0] = fTemp102;
			int iTemp103 = int(fTemp102);
			fRec22[0] = ((iTemp103) ? fTemp82 : fRec22[1]);
			float fTemp104 = std::exp(-(fConst5 * fRec22[0]));
			fRec23[0] = ((iTemp103) ? fTemp81 : fRec23[1]);
			float fTemp105 = std::exp(-(fConst5 * fRec23[0]));
			fRec21[0] = fVec6[2] + fRec21[1] * (fTemp104 + fTemp105) - fTemp104 * fTemp105 * fRec21[2];
			int iTemp106 = iTemp0 | int(fVec6[1]);
			float fTemp107 = ((iTemp106) ? 0.0f : fTemp88 + fRec24[1]);
			fRec24[0] = fTemp107 - std::floor(fTemp107);
			float fTemp108 = float(fRec8[0] == 5.0f) * fTemp58;
			fVec7[0] = fTemp108;
			int iTemp109 = int(fTemp108);
			fRec26[0] = ((iTemp109) ? fTemp82 : fRec26[1]);
			float fTemp110 = std::exp(-(fConst5 * fRec26[0]));
			fRec27[0] = ((iTemp109) ? fTemp81 : fRec27[1]);
			float fTemp111 = std::exp(-(fConst5 * fRec27[0]));
			fRec25[0] = fVec7[2] + fRec25[1] * (fTemp110 + fTemp111) - fTemp110 * fTemp111 * fRec25[2];
			int iTemp112 = iTemp0 | int(fVec7[1]);
			float fTemp113 = ((iTemp112) ? 0.0f : fTemp88 + fRec28[1]);
			fRec28[0] = fTemp113 - std::floor(fTemp113);
			float fTemp114 = float(fRec8[0] == 4.0f) * fTemp58;
			fVec8[0] = fTemp114;
			int iTemp115 = int(fTemp114);
			fRec30[0] = ((iTemp115) ? fTemp82 : fRec30[1]);
			float fTemp116 = std::exp(-(fConst5 * fRec30[0]));
			fRec31[0] = ((iTemp115) ? fTemp81 : fRec31[1]);
			float fTemp117 = std::exp(-(fConst5 * fRec31[0]));
			fRec29[0] = fVec8[2] + fRec29[1] * (fTemp116 + fTemp117) - fTemp116 * fTemp117 * fRec29[2];
			int iTemp118 = iTemp0 | int(fVec8[1]);
			float fTemp119 = ((iTemp118) ? 0.0f : fTemp88 + fRec32[1]);
			fRec32[0] = fTemp119 - std::floor(fTemp119);
			float fTemp120 = float(fRec8[0] == 12.0f) * fTemp58;
			fVec9[0] = fTemp120;
			int iTemp121 = int(fTemp120);
			fRec34[0] = ((iTemp121) ? fTemp82 : fRec34[1]);
			float fTemp122 = std::exp(-(fConst5 * fRec34[0]));
			fRec35[0] = ((iTemp121) ? fTemp81 : fRec35[1]);
			float fTemp123 = std::exp(-(fConst5 * fRec35[0]));
			fRec33[0] = fVec9[2] + fRec33[1] * (fTemp122 + fTemp123) - fTemp122 * fTemp123 * fRec33[2];
			int iTemp124 = iTemp0 | int(fVec9[1]);
			float fTemp125 = ((iTemp124) ? 0.0f : fTemp88 + fRec36[1]);
			fRec36[0] = fTemp125 - std::floor(fTemp125);
			float fTemp126 = float(fRec8[0] == 11.0f) * fTemp58;
			fVec10[0] = fTemp126;
			int iTemp127 = int(fTemp126);
			fRec38[0] = ((iTemp127) ? fTemp82 : fRec38[1]);
			float fTemp128 = std::exp(-(fConst5 * fRec38[0]));
			fRec39[0] = ((iTemp127) ? fTemp81 : fRec39[1]);
			float fTemp129 = std::exp(-(fConst5 * fRec39[0]));
			fRec37[0] = fVec10[2] + fRec37[1] * (fTemp128 + fTemp129) - fTemp128 * fTemp129 * fRec37[2];
			int iTemp130 = iTemp0 | int(fVec10[1]);
			float fTemp131 = ((iTemp130) ? 0.0f : fTemp88 + fRec40[1]);
			fRec40[0] = fTemp131 - std::floor(fTemp131);
			float fTemp132 = float(fRec8[0] == 1.0f) * fTemp58;
			fVec11[0] = fTemp132;
			int iTemp133 = int(fTemp132);
			fRec42[0] = ((iTemp133) ? fTemp82 : fRec42[1]);
			float fTemp134 = std::exp(-(fConst5 * fRec42[0]));
			fRec43[0] = ((iTemp133) ? fTemp81 : fRec43[1]);
			float fTemp135 = std::exp(-(fConst5 * fRec43[0]));
			fRec41[0] = fVec11[2] + fRec41[1] * (fTemp134 + fTemp135) - fTemp134 * fTemp135 * fRec41[2];
			int iTemp136 = iTemp0 | int(fVec11[1]);
			float fTemp137 = ((iTemp136) ? 0.0f : fRec44[1] + fTemp88);
			fRec44[0] = fTemp137 - std::floor(fTemp137);
			float fTemp138 = float(fRec8[0] == 2.0f) * fTemp58;
			fVec12[0] = fTemp138;
			int iTemp139 = int(fTemp138);
			fRec46[0] = ((iTemp139) ? fTemp82 : fRec46[1]);
			float fTemp140 = std::exp(-(fConst5 * fRec46[0]));
			fRec47[0] = ((iTemp139) ? fTemp81 : fRec47[1]);
			float fTemp141 = std::exp(-(fConst5 * fRec47[0]));
			fRec45[0] = fVec12[2] + fRec45[1] * (fTemp140 + fTemp141) - fTemp140 * fTemp141 * fRec45[2];
			int iTemp142 = iTemp0 | int(fVec12[1]);
			float fTemp143 = ((iTemp142) ? 0.0f : fTemp88 + fRec48[1]);
			fRec48[0] = fTemp143 - std::floor(fTemp143);
			float fTemp144 = float(fRec8[0] == 3.0f) * fTemp58;
			fVec13[0] = fTemp144;
			int iTemp145 = int(fTemp144);
			fRec50[0] = ((iTemp145) ? fTemp82 : fRec50[1]);
			float fTemp146 = std::exp(-(fConst5 * fRec50[0]));
			fRec51[0] = ((iTemp145) ? fTemp81 : fRec51[1]);
			float fTemp147 = std::exp(-(fConst5 * fRec51[0]));
			fRec49[0] = fVec13[2] + fRec49[1] * (fTemp146 + fTemp147) - fTemp146 * fTemp147 * fRec49[2];
			int iTemp148 = iTemp0 | int(fVec13[1]);
			float fTemp149 = ((iTemp148) ? 0.0f : fTemp88 + fRec52[1]);
			fRec52[0] = fTemp149 - std::floor(fTemp149);
			float fTemp150 = float(fRec8[0] == 1e+01f) * fTemp58;
			fVec14[0] = fTemp150;
			int iTemp151 = int(fTemp150);
			fRec54[0] = ((iTemp151) ? fTemp82 : fRec54[1]);
			float fTemp152 = std::exp(-(fConst5 * fRec54[0]));
			fRec55[0] = ((iTemp151) ? fTemp81 : fRec55[1]);
			float fTemp153 = std::exp(-(fConst5 * fRec55[0]));
			fRec53[0] = fVec14[2] + fRec53[1] * (fTemp152 + fTemp153) - fTemp152 * fTemp153 * fRec53[2];
			int iTemp154 = iTemp0 | int(fVec14[1]);
			float fTemp155 = ((iTemp154) ? 0.0f : fTemp88 + fRec56[1]);
			fRec56[0] = fTemp155 - std::floor(fTemp155);
			float fTemp156 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 0.1f : ((iTemp52) ? 0.1f - 0.068377f * fTemp4 : 0.031623f)) : 0.031623f) : ((iTemp48) ? 0.126866f * fTemp51 + 0.031623f : 0.158489f)) : ((iTemp46) ? 0.158489f - 0.126866f * fTemp49 : 0.031623f)) : ((iTemp44) ? 0.32319f * fTemp47 + 0.031623f : 0.354813f)) : 0.354813f) : ((iTemp40) ? 0.354813f - 0.196324f * fTemp43 : 0.158489f)) : ((iTemp38) ? 0.158489f - 0.069364f * fTemp41 : 0.089125f)) : ((iTemp36) ? 0.089125f - 0.064006f * fTemp39 : 0.025119f)) : ((iTemp34) ? 0.045676f * fTemp37 + 0.025119f : 0.070795f)) : ((iTemp31) ? 0.055098f * fTemp35 + 0.070795f : 0.125893f)) : ((iTemp29) ? 0.125893f - 0.062797f * fTemp32 : 0.063096f)) : ((iTemp27) ? 0.063096f - 0.012977f * fTemp30 : 0.050119f)) : ((iTemp25) ? 0.020676f * fTemp28 + 0.050119f : 0.070795f)) : ((iTemp23) ? 0.070795f - 0.045676f * fTemp26 : 0.025119f)) : ((iTemp21) ? 0.152709f * fTemp24 + 0.025119f : 0.177828f)) : ((iTemp18) ? 0.177828f - 0.127709f * fTemp22 : 0.050119f)) : fTemp20) : ((iTemp14) ? 0.079433f - 0.06165f * fTemp17 : 0.017783f)) : ((iTemp12) ? 0.428901f * fTemp15 + 0.017783f : 0.446684f)) : ((iTemp10) ? 0.446684f - 0.195495f * fTemp13 : 0.251189f)) : ((iTemp8) ? 0.251189f - 0.125296f * fTemp11 : 0.125893f)) : ((iTemp5) ? 0.125296f * fTemp9 + 0.125893f : 0.251189f)) : ((iTemp6) ? 0.251189f - 0.109935f * fTemp7 : 0.141254f));
			float fTemp157 = 2e+01f * fTemp13;
			float fTemp158 = 2e+01f * fTemp26;
			float fTemp159 = 2e+01f * fTemp35;
			float fTemp160 = 2e+01f * fTemp37;
			float fTemp161 = 1e+01f * fTemp45;
			float fTemp162 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? 1.2e+02f : ((iTemp48) ? 1.2e+02f - fTemp79 : 1e+02f)) : ((iTemp46) ? 7e+01f * fTemp49 + 1e+02f : 1.7e+02f)) : ((iTemp44) ? 1.7e+02f - fTemp78 : 1.1e+02f)) : ((iTemp42) ? 1.1e+02f - fTemp161 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp34) ? fTemp160 + 1e+02f : 1.2e+02f)) : ((iTemp31) ? 1.2e+02f - fTemp159 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp23) ? fTemp158 + 1e+02f : 1.2e+02f)) : 1.2e+02f) : ((iTemp18) ? 1.2e+02f - 2e+01f * fTemp22 : 1e+02f)) : 1e+02f) : ((iTemp14) ? 7e+01f * fTemp17 + 1e+02f : 1.7e+02f)) : ((iTemp12) ? 1.7e+02f - fTemp74 : 1.2e+02f)) : ((iTemp10) ? 1.2e+02f - fTemp157 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f);
			float fTemp163 = fTemp71 * fTemp162;
			fRec58[0] = ((iTemp145) ? fTemp163 : fRec58[1]);
			float fTemp164 = std::exp(-(fConst5 * fRec58[0]));
			fRec59[0] = ((iTemp145) ? fTemp162 : fRec59[1]);
			float fTemp165 = std::exp(-(fConst5 * fRec59[0]));
			fRec57[0] = fVec13[2] + fRec57[1] * (fTemp164 + fTemp165) - fTemp164 * fTemp165 * fRec57[2];
			float fTemp166 = 5e+01f * fTemp35;
			float fTemp167 = fConst4 * ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 2.8e+03f : ((iTemp52) ? 2.8e+03f - 1e+02f * fTemp4 : 2.7e+03f)) : 2.7e+03f) : ((iTemp48) ? 1.3e+02f * fTemp51 + 2.7e+03f : 2.83e+03f)) : ((iTemp46) ? 2.83e+03f - 3e+02f * fTemp49 : 2.53e+03f)) : ((iTemp44) ? 2.53e+03f - 2.8e+02f * fTemp47 : 2.25e+03f)) : ((iTemp42) ? 1.5e+02f * fTemp45 + 2.25e+03f : 2.4e+03f)) : ((iTemp40) ? 2e+02f * fTemp43 + 2.4e+03f : 2.6e+03f)) : ((iTemp38) ? 2.6e+03f - 2e+02f * fTemp41 : 2.4e+03f)) : 2.4e+03f) : ((iTemp34) ? 3.5e+02f * fTemp37 + 2.4e+03f : 2.75e+03f)) : ((iTemp31) ? 2.75e+03f - fTemp166 : 2.7e+03f)) : ((iTemp29) ? 2e+02f * fTemp32 + 2.7e+03f : 2.9e+03f)) : ((iTemp27) ? 2.9e+03f - 2e+02f * fTemp30 : 2.7e+03f)) : ((iTemp25) ? 5e+01f * fTemp28 + 2.7e+03f : 2.75e+03f)) : ((iTemp23) ? 1.5e+02f * fTemp26 + 2.75e+03f : 2.9e+03f)) : ((iTemp21) ? 2.9e+03f - 1e+02f * fTemp24 : 2.8e+03f)) : ((iTemp18) ? 1.5e+02f * fTemp22 + 2.8e+03f : 2.95e+03f)) : ((iTemp16) ? 2.95e+03f - 1.2e+02f * fTemp19 : 2.83e+03f)) : ((iTemp14) ? 2.83e+03f - 1.3e+02f * fTemp17 : 2.7e+03f)) : ((iTemp12) ? 2.7e+03f - fTemp74 : 2.65e+03f)) : ((iTemp10) ? 2.65e+03f - 5e+01f * fTemp13 : 2.6e+03f)) : ((iTemp8) ? 2e+02f * fTemp11 + 2.6e+03f : 2.8e+03f)) : ((iTemp5) ? 2.8e+03f - 2e+02f * fTemp9 : 2.6e+03f)) : ((iTemp6) ? fTemp86 + 2.6e+03f : 2.7e+03f));
			float fTemp168 = ((iTemp148) ? 0.0f : fTemp167 + fRec60[1]);
			fRec60[0] = fTemp168 - std::floor(fTemp168);
			fRec62[0] = ((iTemp139) ? fTemp163 : fRec62[1]);
			float fTemp169 = std::exp(-(fConst5 * fRec62[0]));
			fRec63[0] = ((iTemp139) ? fTemp162 : fRec63[1]);
			float fTemp170 = std::exp(-(fConst5 * fRec63[0]));
			fRec61[0] = fVec12[2] + fRec61[1] * (fTemp169 + fTemp170) - fTemp169 * fTemp170 * fRec61[2];
			float fTemp171 = ((iTemp142) ? 0.0f : fTemp167 + fRec64[1]);
			fRec64[0] = fTemp171 - std::floor(fTemp171);
			fRec66[0] = ((iTemp133) ? fTemp163 : fRec66[1]);
			float fTemp172 = std::exp(-(fConst5 * fRec66[0]));
			fRec67[0] = ((iTemp133) ? fTemp162 : fRec67[1]);
			float fTemp173 = std::exp(-(fConst5 * fRec67[0]));
			fRec65[0] = fVec11[2] + fRec65[1] * (fTemp172 + fTemp173) - fTemp172 * fTemp173 * fRec65[2];
			float fTemp174 = ((iTemp136) ? 0.0f : fRec68[1] + fTemp167);
			fRec68[0] = fTemp174 - std::floor(fTemp174);
			fRec70[0] = ((iTemp60) ? fTemp163 : fRec70[1]);
			float fTemp175 = std::exp(-(fConst5 * fRec70[0]));
			fRec71[0] = ((iTemp60) ? fTemp162 : fRec71[1]);
			float fTemp176 = std::exp(-(fConst5 * fRec71[0]));
			fRec69[0] = fVec3[2] + fRec69[1] * (fTemp175 + fTemp176) - fTemp175 * fTemp176 * fRec69[2];
			float fTemp177 = ((iTemp85) ? 0.0f : fTemp167 + fRec72[1]);
			fRec72[0] = fTemp177 - std::floor(fTemp177);
			fRec74[0] = ((iTemp91) ? fTemp163 : fRec74[1]);
			float fTemp178 = std::exp(-(fConst5 * fRec74[0]));
			fRec75[0] = ((iTemp91) ? fTemp162 : fRec75[1]);
			float fTemp179 = std::exp(-(fConst5 * fRec75[0]));
			fRec73[0] = fVec4[2] + fRec73[1] * (fTemp178 + fTemp179) - fTemp178 * fTemp179 * fRec73[2];
			float fTemp180 = ((iTemp94) ? 0.0f : fTemp167 + fRec76[1]);
			fRec76[0] = fTemp180 - std::floor(fTemp180);
			fRec78[0] = ((iTemp97) ? fTemp163 : fRec78[1]);
			float fTemp181 = std::exp(-(fConst5 * fRec78[0]));
			fRec79[0] = ((iTemp97) ? fTemp162 : fRec79[1]);
			float fTemp182 = std::exp(-(fConst5 * fRec79[0]));
			fRec77[0] = fVec5[2] + fRec77[1] * (fTemp181 + fTemp182) - fTemp181 * fTemp182 * fRec77[2];
			float fTemp183 = ((iTemp100) ? 0.0f : fTemp167 + fRec80[1]);
			fRec80[0] = fTemp183 - std::floor(fTemp183);
			fRec82[0] = ((iTemp103) ? fTemp163 : fRec82[1]);
			float fTemp184 = std::exp(-(fConst5 * fRec82[0]));
			fRec83[0] = ((iTemp103) ? fTemp162 : fRec83[1]);
			float fTemp185 = std::exp(-(fConst5 * fRec83[0]));
			fRec81[0] = fVec6[2] + fRec81[1] * (fTemp184 + fTemp185) - fTemp184 * fTemp185 * fRec81[2];
			float fTemp186 = ((iTemp106) ? 0.0f : fTemp167 + fRec84[1]);
			fRec84[0] = fTemp186 - std::floor(fTemp186);
			fRec86[0] = ((iTemp109) ? fTemp163 : fRec86[1]);
			float fTemp187 = std::exp(-(fConst5 * fRec86[0]));
			fRec87[0] = ((iTemp109) ? fTemp162 : fRec87[1]);
			float fTemp188 = std::exp(-(fConst5 * fRec87[0]));
			fRec85[0] = fVec7[2] + fRec85[1] * (fTemp187 + fTemp188) - fTemp187 * fTemp188 * fRec85[2];
			float fTemp189 = ((iTemp112) ? 0.0f : fTemp167 + fRec88[1]);
			fRec88[0] = fTemp189 - std::floor(fTemp189);
			fRec90[0] = ((iTemp115) ? fTemp163 : fRec90[1]);
			float fTemp190 = std::exp(-(fConst5 * fRec90[0]));
			fRec91[0] = ((iTemp115) ? fTemp162 : fRec91[1]);
			float fTemp191 = std::exp(-(fConst5 * fRec91[0]));
			fRec89[0] = fVec8[2] + fRec89[1] * (fTemp190 + fTemp191) - fTemp190 * fTemp191 * fRec89[2];
			float fTemp192 = ((iTemp118) ? 0.0f : fTemp167 + fRec92[1]);
			fRec92[0] = fTemp192 - std::floor(fTemp192);
			fRec94[0] = ((iTemp121) ? fTemp163 : fRec94[1]);
			float fTemp193 = std::exp(-(fConst5 * fRec94[0]));
			fRec95[0] = ((iTemp121) ? fTemp162 : fRec95[1]);
			float fTemp194 = std::exp(-(fConst5 * fRec95[0]));
			fRec93[0] = fVec9[2] + fRec93[1] * (fTemp193 + fTemp194) - fTemp193 * fTemp194 * fRec93[2];
			float fTemp195 = ((iTemp124) ? 0.0f : fTemp167 + fRec96[1]);
			fRec96[0] = fTemp195 - std::floor(fTemp195);
			fRec98[0] = ((iTemp151) ? fTemp163 : fRec98[1]);
			float fTemp196 = std::exp(-(fConst5 * fRec98[0]));
			fRec99[0] = ((iTemp151) ? fTemp162 : fRec99[1]);
			float fTemp197 = std::exp(-(fConst5 * fRec99[0]));
			fRec97[0] = fVec14[2] + fRec97[1] * (fTemp196 + fTemp197) - fTemp196 * fTemp197 * fRec97[2];
			float fTemp198 = ((iTemp154) ? 0.0f : fTemp167 + fRec100[1]);
			fRec100[0] = fTemp198 - std::floor(fTemp198);
			fRec102[0] = ((iTemp127) ? fTemp163 : fRec102[1]);
			float fTemp199 = std::exp(-(fConst5 * fRec102[0]));
			fRec103[0] = ((iTemp127) ? fTemp162 : fRec103[1]);
			float fTemp200 = std::exp(-(fConst5 * fRec103[0]));
			fRec101[0] = fVec10[2] + fRec101[1] * (fTemp199 + fTemp200) - fTemp199 * fTemp200 * fRec101[2];
			float fTemp201 = ((iTemp130) ? 0.0f : fTemp167 + fRec104[1]);
			fRec104[0] = fTemp201 - std::floor(fTemp201);
			float fTemp202 = 1e+01f * fTemp47;
			float fTemp203 = 2e+01f * fTemp49;
			float fTemp204 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 8e+01f : ((iTemp52) ? 8e+01f - fTemp80 : 6e+01f)) : ((iTemp50) ? 6e+01f - 1e+01f * fTemp53 : 5e+01f)) : ((iTemp48) ? fTemp79 + 5e+01f : 7e+01f)) : ((iTemp46) ? 7e+01f - fTemp203 : 5e+01f)) : ((iTemp44) ? fTemp202 + 5e+01f : 6e+01f)) : ((iTemp42) ? 6e+01f - 2e+01f * fTemp45 : 4e+01f)) : ((iTemp40) ? 2e+01f * fTemp43 + 4e+01f : 6e+01f)) : ((iTemp38) ? 6e+01f - 2e+01f * fTemp41 : 4e+01f)) : 4e+01f) : ((iTemp34) ? 4e+01f * (fTemp37 + 1.0f) : 8e+01f)) : ((iTemp31) ? 8e+01f - fTemp76 : 7e+01f)) : ((iTemp29) ? 7e+01f - 3e+01f * fTemp32 : 4e+01f)) : 4e+01f) : 4e+01f) : ((iTemp23) ? 4e+01f * (fTemp26 + 1.0f) : 8e+01f)) : ((iTemp21) ? 8e+01f - fTemp75 : 6e+01f)) : 6e+01f) : ((iTemp16) ? 6e+01f - 2e+01f * fTemp19 : 4e+01f)) : ((iTemp14) ? 1e+01f * fTemp17 + 4e+01f : 5e+01f)) : 5e+01f) : ((iTemp10) ? fTemp157 + 5e+01f : 7e+01f)) : ((iTemp8) ? 7e+01f - 3e+01f * fTemp11 : 4e+01f)) : ((iTemp5) ? 3e+01f * fTemp9 + 4e+01f : 7e+01f)) : ((iTemp6) ? 7e+01f - 3e+01f * fTemp7 : 4e+01f));
			float fTemp205 = fTemp71 * fTemp204;
			fRec106[0] = ((iTemp121) ? fTemp205 : fRec106[1]);
			float fTemp206 = std::exp(-(fConst5 * fRec106[0]));
			fRec107[0] = ((iTemp121) ? fTemp204 : fRec107[1]);
			float fTemp207 = std::exp(-(fConst5 * fRec107[0]));
			fRec105[0] = fVec9[2] + fRec105[1] * (fTemp206 + fTemp207) - fTemp206 * fTemp207 * fRec105[2];
			float fTemp208 = 8e+01f * fTemp22;
			float fTemp209 = 5e+01f * fTemp39;
			float fTemp210 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 8e+02f : ((iTemp52) ? 8e+02f - 4e+02f * fTemp4 : 4e+02f)) : ((iTemp50) ? 4e+02f - 5e+01f * fTemp53 : 3.5e+02f)) : ((iTemp48) ? 1e+02f * fTemp51 + 3.5e+02f : 4.5e+02f)) : ((iTemp46) ? 4.5e+02f - 125.0f * fTemp49 : 325.0f)) : ((iTemp44) ? 275.0f * fTemp47 + 325.0f : 6e+02f)) : ((iTemp42) ? 6e+02f - 2e+02f * fTemp45 : 4e+02f)) : ((iTemp40) ? 4e+02f - 1.5e+02f * fTemp43 : 2.5e+02f)) : ((iTemp38) ? 1.5e+02f * fTemp41 + 2.5e+02f : 4e+02f)) : ((iTemp36) ? 4e+02f - fTemp209 : 3.5e+02f)) : ((iTemp34) ? 3.1e+02f * fTemp37 + 3.5e+02f : 6.6e+02f)) : ((iTemp31) ? 6.6e+02f - 2.2e+02f * fTemp35 : 4.4e+02f)) : ((iTemp29) ? 4.4e+02f - 1.7e+02f * fTemp32 : 2.7e+02f)) : ((iTemp27) ? 1.6e+02f * fTemp30 + 2.7e+02f : 4.3e+02f)) : ((iTemp25) ? 4.3e+02f - 6e+01f * fTemp28 : 3.7e+02f)) : ((iTemp23) ? 4.3e+02f * fTemp26 + 3.7e+02f : 8e+02f)) : ((iTemp21) ? 8e+02f - 4.5e+02f * fTemp24 : 3.5e+02f)) : ((iTemp18) ? 3.5e+02f - fTemp208 : 2.7e+02f)) : ((iTemp16) ? 1.8e+02f * fTemp19 + 2.7e+02f : 4.5e+02f)) : ((iTemp14) ? 4.5e+02f - 125.0f * fTemp17 : 325.0f)) : ((iTemp12) ? 325.0f * (fTemp15 + 1.0f) : 6.5e+02f)) : ((iTemp10) ? 6.5e+02f - 2.5e+02f * fTemp13 : 4e+02f)) : ((iTemp8) ? 4e+02f - 1.1e+02f * fTemp11 : 2.9e+02f)) : ((iTemp5) ? 1.1e+02f * fTemp9 + 2.9e+02f : 4e+02f)) : ((iTemp6) ? 4e+02f - 5e+01f * fTemp7 : 3.5e+02f));
			float fTemp211 = fConst4 * ((fTemp210 <= fTemp2) ? fTemp2 : fTemp210);
			float fTemp212 = ((iTemp124) ? 0.0f : fTemp211 + fRec108[1]);
			fRec108[0] = fTemp212 - std::floor(fTemp212);
			fRec110[0] = ((iTemp127) ? fTemp205 : fRec110[1]);
			float fTemp213 = std::exp(-(fConst5 * fRec110[0]));
			fRec111[0] = ((iTemp127) ? fTemp204 : fRec111[1]);
			float fTemp214 = std::exp(-(fConst5 * fRec111[0]));
			fRec109[0] = fVec10[2] + fRec109[1] * (fTemp213 + fTemp214) - fTemp213 * fTemp214 * fRec109[2];
			float fTemp215 = ((iTemp130) ? 0.0f : fTemp211 + fRec112[1]);
			fRec112[0] = fTemp215 - std::floor(fTemp215);
			fRec114[0] = ((iTemp109) ? fTemp205 : fRec114[1]);
			float fTemp216 = std::exp(-(fConst5 * fRec114[0]));
			fRec115[0] = ((iTemp109) ? fTemp204 : fRec115[1]);
			float fTemp217 = std::exp(-(fConst5 * fRec115[0]));
			fRec113[0] = fVec7[2] + fRec113[1] * (fTemp216 + fTemp217) - fTemp216 * fTemp217 * fRec113[2];
			float fTemp218 = ((iTemp112) ? 0.0f : fTemp211 + fRec116[1]);
			fRec116[0] = fTemp218 - std::floor(fTemp218);
			fRec118[0] = ((iTemp115) ? fTemp205 : fRec118[1]);
			float fTemp219 = std::exp(-(fConst5 * fRec118[0]));
			fRec119[0] = ((iTemp115) ? fTemp204 : fRec119[1]);
			float fTemp220 = std::exp(-(fConst5 * fRec119[0]));
			fRec117[0] = fVec8[2] + fRec117[1] * (fTemp219 + fTemp220) - fTemp219 * fTemp220 * fRec117[2];
			float fTemp221 = ((iTemp118) ? 0.0f : fTemp211 + fRec120[1]);
			fRec120[0] = fTemp221 - std::floor(fTemp221);
			fRec122[0] = ((iTemp145) ? fTemp205 : fRec122[1]);
			float fTemp222 = std::exp(-(fConst5 * fRec122[0]));
			fRec123[0] = ((iTemp145) ? fTemp204 : fRec123[1]);
			float fTemp223 = std::exp(-(fConst5 * fRec123[0]));
			fRec121[0] = fVec13[2] + fRec121[1] * (fTemp222 + fTemp223) - fTemp222 * fTemp223 * fRec121[2];
			float fTemp224 = ((iTemp148) ? 0.0f : fTemp211 + fRec124[1]);
			fRec124[0] = fTemp224 - std::floor(fTemp224);
			fRec126[0] = ((iTemp139) ? fTemp205 : fRec126[1]);
			float fTemp225 = std::exp(-(fConst5 * fRec126[0]));
			fRec127[0] = ((iTemp139) ? fTemp204 : fRec127[1]);
			float fTemp226 = std::exp(-(fConst5 * fRec127[0]));
			fRec125[0] = fVec12[2] + fRec125[1] * (fTemp225 + fTemp226) - fTemp225 * fTemp226 * fRec125[2];
			float fTemp227 = ((iTemp142) ? 0.0f : fTemp211 + fRec128[1]);
			fRec128[0] = fTemp227 - std::floor(fTemp227);
			fRec130[0] = ((iTemp133) ? fTemp205 : fRec130[1]);
			float fTemp228 = std::exp(-(fConst5 * fRec130[0]));
			fRec131[0] = ((iTemp133) ? fTemp204 : fRec131[1]);
			float fTemp229 = std::exp(-(fConst5 * fRec131[0]));
			fRec129[0] = fVec11[2] + fRec129[1] * (fTemp228 + fTemp229) - fTemp228 * fTemp229 * fRec129[2];
			float fTemp230 = ((iTemp136) ? 0.0f : fRec132[1] + fTemp211);
			fRec132[0] = fTemp230 - std::floor(fTemp230);
			fRec134[0] = ((iTemp151) ? fTemp205 : fRec134[1]);
			float fTemp231 = std::exp(-(fConst5 * fRec134[0]));
			fRec135[0] = ((iTemp151) ? fTemp204 : fRec135[1]);
			float fTemp232 = std::exp(-(fConst5 * fRec135[0]));
			fRec133[0] = fVec14[2] + fRec133[1] * (fTemp231 + fTemp232) - fTemp231 * fTemp232 * fRec133[2];
			float fTemp233 = ((iTemp154) ? 0.0f : fTemp211 + fRec136[1]);
			fRec136[0] = fTemp233 - std::floor(fTemp233);
			fRec138[0] = ((iTemp60) ? fTemp205 : fRec138[1]);
			float fTemp234 = std::exp(-(fConst5 * fRec138[0]));
			fRec139[0] = ((iTemp60) ? fTemp204 : fRec139[1]);
			float fTemp235 = std::exp(-(fConst5 * fRec139[0]));
			fRec137[0] = fVec3[2] + fRec137[1] * (fTemp234 + fTemp235) - fTemp234 * fTemp235 * fRec137[2];
			float fTemp236 = ((iTemp85) ? 0.0f : fTemp211 + fRec140[1]);
			fRec140[0] = fTemp236 - std::floor(fTemp236);
			fRec142[0] = ((iTemp91) ? fTemp205 : fRec142[1]);
			float fTemp237 = std::exp(-(fConst5 * fRec142[0]));
			fRec143[0] = ((iTemp91) ? fTemp204 : fRec143[1]);
			float fTemp238 = std::exp(-(fConst5 * fRec143[0]));
			fRec141[0] = fVec4[2] + fRec141[1] * (fTemp237 + fTemp238) - fTemp237 * fTemp238 * fRec141[2];
			float fTemp239 = ((iTemp94) ? 0.0f : fTemp211 + fRec144[1]);
			fRec144[0] = fTemp239 - std::floor(fTemp239);
			fRec146[0] = ((iTemp103) ? fTemp205 : fRec146[1]);
			float fTemp240 = std::exp(-(fConst5 * fRec146[0]));
			fRec147[0] = ((iTemp103) ? fTemp204 : fRec147[1]);
			float fTemp241 = std::exp(-(fConst5 * fRec147[0]));
			fRec145[0] = fVec6[2] + fRec145[1] * (fTemp240 + fTemp241) - fTemp240 * fTemp241 * fRec145[2];
			float fTemp242 = ((iTemp106) ? 0.0f : fTemp211 + fRec148[1]);
			fRec148[0] = fTemp242 - std::floor(fTemp242);
			fRec150[0] = ((iTemp97) ? fTemp205 : fRec150[1]);
			float fTemp243 = std::exp(-(fConst5 * fRec150[0]));
			fRec151[0] = ((iTemp97) ? fTemp204 : fRec151[1]);
			float fTemp244 = std::exp(-(fConst5 * fRec151[0]));
			fRec149[0] = fVec5[2] + fRec149[1] * (fTemp243 + fTemp244) - fTemp243 * fTemp244 * fRec149[2];
			float fTemp245 = ((iTemp100) ? 0.0f : fTemp211 + fRec152[1]);
			fRec152[0] = fTemp245 - std::floor(fTemp245);
			float fTemp246 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 0.630957f : ((iTemp52) ? 0.630957f - 0.567861f * fTemp4 : 0.063096f)) : ((iTemp50) ? 0.036904f * fTemp53 + 0.063096f : 0.1f)) : ((iTemp48) ? 0.254813f * fTemp51 + 0.1f : 0.354813f)) : ((iTemp46) ? 0.354813f - 0.103624f * fTemp49 : 0.251189f)) : ((iTemp44) ? 0.195495f * fTemp47 + 0.251189f : 0.446684f)) : ((iTemp42) ? 0.446684f - 0.195495f * fTemp45 : 0.251189f)) : ((iTemp40) ? 0.251189f - 0.219566f * fTemp43 : 0.031623f)) : ((iTemp38) ? 0.250215f * fTemp41 + 0.031623f : 0.281838f)) : ((iTemp36) ? 0.281838f - 0.181838f * fTemp39 : 0.1f)) : ((iTemp34) ? 0.401187f * fTemp37 + 0.1f : 0.501187f)) : ((iTemp31) ? 0.501187f - 0.301661f * fTemp35 : 0.199526f)) : ((iTemp29) ? 0.199526f - 0.13643f * fTemp32 : 0.063096f)) : ((iTemp27) ? 0.253132f * fTemp30 + 0.063096f : 0.316228f)) : ((iTemp25) ? 0.316228f - 0.216228f * fTemp28 : 0.1f)) : ((iTemp23) ? 0.401187f * fTemp26 + 0.1f : 0.501187f)) : ((iTemp21) ? 0.501187f - 0.401187f * fTemp24 : 0.1f)) : ((iTemp18) ? 0.151189f * fTemp22 + 0.1f : 0.251189f)) : ((iTemp16) ? 0.030649f * fTemp19 + 0.251189f : 0.281838f)) : ((iTemp14) ? 0.281838f - 0.123349f * fTemp17 : 0.158489f)) : ((iTemp12) ? 0.342698f * fTemp15 + 0.158489f : 0.501187f)) : ((iTemp10) ? 0.501187f - 0.301661f * fTemp13 : 0.199526f)) : ((iTemp8) ? 0.199526f - 0.021698f * fTemp11 : 0.177828f)) : ((iTemp5) ? 0.1384f * fTemp9 + 0.177828f : 0.316228f)) : ((iTemp6) ? 0.316228f - 0.216228f * fTemp7 : 0.1f));
			float fTemp247 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 9e+01f : ((iTemp52) ? 9e+01f - 1e+01f * fTemp4 : 8e+01f)) : ((iTemp50) ? 2e+01f * fTemp53 + 8e+01f : 1e+02f)) : ((iTemp48) ? 1e+02f - fTemp79 : 8e+01f)) : ((iTemp46) ? 8e+01f - fTemp203 : 6e+01f)) : ((iTemp44) ? fTemp202 + 6e+01f : 7e+01f)) : ((iTemp42) ? fTemp161 + 7e+01f : 8e+01f)) : ((iTemp40) ? 1e+01f * fTemp43 + 8e+01f : 9e+01f)) : ((iTemp38) ? 9e+01f - 1e+01f * fTemp41 : 8e+01f)) : 8e+01f) : ((iTemp34) ? fTemp77 + 8e+01f : 9e+01f)) : ((iTemp31) ? 9e+01f - fTemp76 : 8e+01f)) : ((iTemp29) ? 1e+01f * fTemp32 + 8e+01f : 9e+01f)) : ((iTemp27) ? 9e+01f - 1e+01f * fTemp30 : 8e+01f)) : ((iTemp25) ? 8e+01f - 2e+01f * fTemp28 : 6e+01f)) : ((iTemp23) ? 3e+01f * fTemp26 + 6e+01f : 9e+01f)) : ((iTemp21) ? 1e+01f * fTemp24 + 9e+01f : 1e+02f)) : ((iTemp18) ? 1e+02f - 1e+01f * fTemp22 : 9e+01f)) : ((iTemp16) ? 9e+01f - 1e+01f * fTemp19 : 8e+01f)) : ((iTemp14) ? 8e+01f - 2e+01f * fTemp17 : 6e+01f)) : ((iTemp12) ? 3e+01f * fTemp15 + 6e+01f : 9e+01f)) : ((iTemp10) ? 9e+01f - fTemp73 : 8e+01f)) : ((iTemp8) ? 1e+01f * fTemp11 + 8e+01f : 9e+01f)) : ((iTemp5) ? 9e+01f - fTemp72 : 8e+01f)) : ((iTemp6) ? 8e+01f - 2e+01f * fTemp7 : 6e+01f));
			float fTemp248 = fTemp71 * fTemp247;
			fRec154[0] = ((iTemp145) ? fTemp248 : fRec154[1]);
			float fTemp249 = std::exp(-(fConst5 * fRec154[0]));
			fRec155[0] = ((iTemp145) ? fTemp247 : fRec155[1]);
			float fTemp250 = std::exp(-(fConst5 * fRec155[0]));
			fRec153[0] = fVec13[2] + fRec153[1] * (fTemp249 + fTemp250) - fTemp249 * fTemp250 * fRec153[2];
			float fTemp251 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 1.15e+03f : ((iTemp52) ? 4.5e+02f * fTemp4 + 1.15e+03f : 1.6e+03f)) : ((iTemp50) ? 1e+02f * fTemp53 + 1.6e+03f : 1.7e+03f)) : ((iTemp48) ? 1.7e+03f - 9e+02f * fTemp51 : 8e+02f)) : ((iTemp46) ? 8e+02f - 1e+02f * fTemp49 : 7e+02f)) : ((iTemp44) ? 3.4e+02f * fTemp47 + 7e+02f : 1.04e+03f)) : ((iTemp42) ? 5.8e+02f * fTemp45 + 1.04e+03f : 1.62e+03f)) : ((iTemp40) ? 1.3e+02f * fTemp43 + 1.62e+03f : 1.75e+03f)) : ((iTemp38) ? 1.75e+03f - 1e+03f * fTemp41 : 7.5e+02f)) : ((iTemp36) ? 7.5e+02f - 1.5e+02f * fTemp39 : 6e+02f)) : ((iTemp34) ? 5.2e+02f * fTemp37 + 6e+02f : 1.12e+03f)) : ((iTemp31) ? 6.8e+02f * fTemp35 + 1.12e+03f : 1.8e+03f)) : ((iTemp29) ? 5e+01f * fTemp32 + 1.8e+03f : 1.85e+03f)) : ((iTemp27) ? 1.85e+03f - 1.03e+03f * fTemp30 : 8.2e+02f)) : ((iTemp25) ? 8.2e+02f - 1.9e+02f * fTemp28 : 6.3e+02f)) : ((iTemp23) ? 5.2e+02f * fTemp26 + 6.3e+02f : 1.15e+03f)) : ((iTemp21) ? 8.5e+02f * fTemp24 + 1.15e+03f : 2e+03f)) : ((iTemp18) ? 1.4e+02f * fTemp22 + 2e+03f : 2.14e+03f)) : ((iTemp16) ? 2.14e+03f - 1.34e+03f * fTemp19 : 8e+02f)) : ((iTemp14) ? 8e+02f - 1e+02f * fTemp17 : 7e+02f)) : ((iTemp12) ? 3.8e+02f * fTemp15 + 7e+02f : 1.08e+03f)) : ((iTemp10) ? 6.2e+02f * fTemp13 + 1.08e+03f : 1.7e+03f)) : ((iTemp8) ? 1.7e+02f * fTemp11 + 1.7e+03f : 1.87e+03f)) : ((iTemp5) ? 1.87e+03f - 1.07e+03f * fTemp9 : 8e+02f)) : ((iTemp6) ? 8e+02f - 2e+02f * fTemp7 : 6e+02f));
			float fTemp252 = 2.0f * fTemp2 + 3e+01f;
			float fTemp253 = fConst4 * ((iSlow24) ? (((fTemp251 >= 1.3e+03f) & (fTemp2 >= 2e+02f)) ? fTemp251 - 0.00095238094f * (fTemp2 + -2e+02f) * (fTemp251 + -1.3e+03f) : ((fTemp251 <= fTemp252) ? fTemp252 : fTemp251)) : fTemp251);
			float fTemp254 = ((iTemp148) ? 0.0f : fTemp253 + fRec156[1]);
			fRec156[0] = fTemp254 - std::floor(fTemp254);
			fRec158[0] = ((iTemp139) ? fTemp248 : fRec158[1]);
			float fTemp255 = std::exp(-(fConst5 * fRec158[0]));
			fRec159[0] = ((iTemp139) ? fTemp247 : fRec159[1]);
			float fTemp256 = std::exp(-(fConst5 * fRec159[0]));
			fRec157[0] = fVec12[2] + fRec157[1] * (fTemp255 + fTemp256) - fTemp255 * fTemp256 * fRec157[2];
			float fTemp257 = ((iTemp142) ? 0.0f : fTemp253 + fRec160[1]);
			fRec160[0] = fTemp257 - std::floor(fTemp257);
			fRec162[0] = ((iTemp133) ? fTemp248 : fRec162[1]);
			float fTemp258 = std::exp(-(fConst5 * fRec162[0]));
			fRec163[0] = ((iTemp133) ? fTemp247 : fRec163[1]);
			float fTemp259 = std::exp(-(fConst5 * fRec163[0]));
			fRec161[0] = fVec11[2] + fRec161[1] * (fTemp258 + fTemp259) - fTemp258 * fTemp259 * fRec161[2];
			float fTemp260 = ((iTemp136) ? 0.0f : fRec164[1] + fTemp253);
			fRec164[0] = fTemp260 - std::floor(fTemp260);
			fRec166[0] = ((iTemp60) ? fTemp248 : fRec166[1]);
			float fTemp261 = std::exp(-(fConst5 * fRec166[0]));
			fRec167[0] = ((iTemp60) ? fTemp247 : fRec167[1]);
			float fTemp262 = std::exp(-(fConst5 * fRec167[0]));
			fRec165[0] = fVec3[2] + fRec165[1] * (fTemp261 + fTemp262) - fTemp261 * fTemp262 * fRec165[2];
			float fTemp263 = ((iTemp85) ? 0.0f : fTemp253 + fRec168[1]);
			fRec168[0] = fTemp263 - std::floor(fTemp263);
			fRec170[0] = ((iTemp91) ? fTemp248 : fRec170[1]);
			float fTemp264 = std::exp(-(fConst5 * fRec170[0]));
			fRec171[0] = ((iTemp91) ? fTemp247 : fRec171[1]);
			float fTemp265 = std::exp(-(fConst5 * fRec171[0]));
			fRec169[0] = fVec4[2] + fRec169[1] * (fTemp264 + fTemp265) - fTemp264 * fTemp265 * fRec169[2];
			float fTemp266 = ((iTemp94) ? 0.0f : fTemp253 + fRec172[1]);
			fRec172[0] = fTemp266 - std::floor(fTemp266);
			fRec174[0] = ((iTemp97) ? fTemp248 : fRec174[1]);
			float fTemp267 = std::exp(-(fConst5 * fRec174[0]));
			fRec175[0] = ((iTemp97) ? fTemp247 : fRec175[1]);
			float fTemp268 = std::exp(-(fConst5 * fRec175[0]));
			fRec173[0] = fVec5[2] + fRec173[1] * (fTemp267 + fTemp268) - fTemp267 * fTemp268 * fRec173[2];
			float fTemp269 = ((iTemp100) ? 0.0f : fTemp253 + fRec176[1]);
			fRec176[0] = fTemp269 - std::floor(fTemp269);
			fRec178[0] = ((iTemp103) ? fTemp248 : fRec178[1]);
			float fTemp270 = std::exp(-(fConst5 * fRec178[0]));
			fRec179[0] = ((iTemp103) ? fTemp247 : fRec179[1]);
			float fTemp271 = std::exp(-(fConst5 * fRec179[0]));
			fRec177[0] = fVec6[2] + fRec177[1] * (fTemp270 + fTemp271) - fTemp270 * fTemp271 * fRec177[2];
			float fTemp272 = ((iTemp106) ? 0.0f : fTemp253 + fRec180[1]);
			fRec180[0] = fTemp272 - std::floor(fTemp272);
			fRec182[0] = ((iTemp109) ? fTemp248 : fRec182[1]);
			float fTemp273 = std::exp(-(fConst5 * fRec182[0]));
			fRec183[0] = ((iTemp109) ? fTemp247 : fRec183[1]);
			float fTemp274 = std::exp(-(fConst5 * fRec183[0]));
			fRec181[0] = fVec7[2] + fRec181[1] * (fTemp273 + fTemp274) - fTemp273 * fTemp274 * fRec181[2];
			float fTemp275 = ((iTemp112) ? 0.0f : fTemp253 + fRec184[1]);
			fRec184[0] = fTemp275 - std::floor(fTemp275);
			fRec186[0] = ((iTemp115) ? fTemp248 : fRec186[1]);
			float fTemp276 = std::exp(-(fConst5 * fRec186[0]));
			fRec187[0] = ((iTemp115) ? fTemp247 : fRec187[1]);
			float fTemp277 = std::exp(-(fConst5 * fRec187[0]));
			fRec185[0] = fVec8[2] + fRec185[1] * (fTemp276 + fTemp277) - fTemp276 * fTemp277 * fRec185[2];
			float fTemp278 = ((iTemp118) ? 0.0f : fTemp253 + fRec188[1]);
			fRec188[0] = fTemp278 - std::floor(fTemp278);
			fRec190[0] = ((iTemp121) ? fTemp248 : fRec190[1]);
			float fTemp279 = std::exp(-(fConst5 * fRec190[0]));
			fRec191[0] = ((iTemp121) ? fTemp247 : fRec191[1]);
			float fTemp280 = std::exp(-(fConst5 * fRec191[0]));
			fRec189[0] = fVec9[2] + fRec189[1] * (fTemp279 + fTemp280) - fTemp279 * fTemp280 * fRec189[2];
			float fTemp281 = ((iTemp124) ? 0.0f : fTemp253 + fRec192[1]);
			fRec192[0] = fTemp281 - std::floor(fTemp281);
			fRec194[0] = ((iTemp151) ? fTemp248 : fRec194[1]);
			float fTemp282 = std::exp(-(fConst5 * fRec194[0]));
			fRec195[0] = ((iTemp151) ? fTemp247 : fRec195[1]);
			float fTemp283 = std::exp(-(fConst5 * fRec195[0]));
			fRec193[0] = fVec14[2] + fRec193[1] * (fTemp282 + fTemp283) - fTemp282 * fTemp283 * fRec193[2];
			float fTemp284 = ((iTemp154) ? 0.0f : fTemp253 + fRec196[1]);
			fRec196[0] = fTemp284 - std::floor(fTemp284);
			fRec198[0] = ((iTemp127) ? fTemp248 : fRec198[1]);
			float fTemp285 = std::exp(-(fConst5 * fRec198[0]));
			fRec199[0] = ((iTemp127) ? fTemp247 : fRec199[1]);
			float fTemp286 = std::exp(-(fConst5 * fRec199[0]));
			fRec197[0] = fVec10[2] + fRec197[1] * (fTemp285 + fTemp286) - fTemp285 * fTemp286 * fRec197[2];
			float fTemp287 = ((iTemp130) ? 0.0f : fTemp253 + fRec200[1]);
			fRec200[0] = fTemp287 - std::floor(fTemp287);
			float fTemp288 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? 0.001f : ((iTemp48) ? 0.000778f * fTemp51 + 0.001f : 0.001778f)) : ((iTemp46) ? 0.001778f - 0.001147f * fTemp49 : 0.000631f)) : ((iTemp44) ? 0.099369f * fTemp47 + 0.000631f : 0.1f)) : ((iTemp42) ? 0.025893f * fTemp45 + 0.1f : 0.125893f)) : ((iTemp40) ? 0.125893f - 0.086082f * fTemp43 : 0.039811f)) : ((iTemp38) ? 0.039811f - 0.029811f * fTemp41 : 0.01f)) : ((iTemp36) ? 0.005849f * fTemp39 + 0.01f : 0.015849f)) : ((iTemp34) ? 0.015849f - 0.00326f * fTemp37 : 0.012589f)) : ((iTemp31) ? 0.087411f * fTemp35 + 0.012589f : 0.1f)) : fTemp33) : ((iTemp27) ? 0.004104f * fTemp30 + 0.015849f : 0.019953f)) : 0.019953f) : ((iTemp23) ? 0.019953f - 0.016791f * fTemp26 : 0.003162f)) : ((iTemp21) ? 0.003162f - 0.001577f * fTemp24 : 0.001585f)) : ((iTemp18) ? 0.004725f * fTemp22 + 0.001585f : 0.00631f)) : ((iTemp16) ? 0.00631f - 0.003148f * fTemp19 : 0.003162f)) : ((iTemp14) ? 0.003162f - 0.002162f * fTemp17 : 0.001f)) : ((iTemp12) ? 0.078433f * fTemp15 + 0.001f : 0.079433f)) : ((iTemp10) ? 0.020567f * fTemp13 + 0.079433f : 0.1f)) : ((iTemp8) ? 0.1f - 0.068377f * fTemp11 : 0.031623f)) : ((iTemp5) ? 0.018496f * fTemp9 + 0.031623f : 0.050119f)) : 0.050119f);
			float fTemp289 = ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp54) ? 1.4e+02f : ((iTemp52) ? 6e+01f * fTemp4 + 1.4e+02f : 2e+02f)) : 2e+02f) : ((iTemp48) ? 2e+02f - 65.0f * fTemp51 : 135.0f)) : ((iTemp46) ? 65.0f * fTemp49 + 135.0f : 2e+02f)) : ((iTemp44) ? 2e+02f - 7e+01f * fTemp47 : 1.3e+02f)) : ((iTemp42) ? 1.3e+02f - fTemp161 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp34) ? fTemp160 + 1.2e+02f : 1.4e+02f)) : ((iTemp31) ? 1.4e+02f - fTemp159 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp23) ? fTemp158 + 1.2e+02f : 1.4e+02f)) : ((iTemp21) ? 6e+01f * fTemp24 + 1.4e+02f : 2e+02f)) : ((iTemp18) ? 2e+02f - fTemp208 : 1.2e+02f)) : 1.2e+02f) : ((iTemp14) ? 8e+01f * fTemp17 + 1.2e+02f : 2e+02f)) : ((iTemp12) ? 2e+02f - 6e+01f * fTemp15 : 1.4e+02f)) : ((iTemp10) ? 1.4e+02f - fTemp157 : 1.2e+02f)) : 1.2e+02f) : ((iTemp5) ? 15.0f * fTemp9 + 1.2e+02f : 135.0f)) : ((iTemp6) ? 135.0f - 15.0f * fTemp7 : 1.2e+02f));
			float fTemp290 = fTemp71 * fTemp289;
			fRec202[0] = ((iTemp121) ? fTemp290 : fRec202[1]);
			float fTemp291 = std::exp(-(fConst5 * fRec202[0]));
			fRec203[0] = ((iTemp121) ? fTemp289 : fRec203[1]);
			float fTemp292 = std::exp(-(fConst5 * fRec203[0]));
			fRec201[0] = fVec9[2] + fRec201[1] * (fTemp291 + fTemp292) - fTemp291 * fTemp292 * fRec201[2];
			float fTemp293 = fConst4 * ((iTemp5) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? 4.95e+03f : ((iTemp44) ? 4.95e+03f - 2.2e+03f * fTemp47 : 2.75e+03f)) : ((iTemp42) ? fTemp87 + 2.75e+03f : 3.1e+03f)) : ((iTemp40) ? 2.4e+02f * fTemp43 + 3.1e+03f : 3.34e+03f)) : ((iTemp38) ? 3.34e+03f - 4.4e+02f * fTemp41 : 2.9e+03f)) : ((iTemp36) ? fTemp209 + 2.9e+03f : 2.95e+03f)) : ((iTemp34) ? 4e+02f * fTemp37 + 2.95e+03f : 3.35e+03f)) : ((iTemp31) ? 3.35e+03f - fTemp166 : 3.3e+03f)) : ((iTemp29) ? 2.9e+02f * fTemp32 + 3.3e+03f : 3.59e+03f)) : ((iTemp27) ? 3.59e+03f - 2.9e+02f * fTemp30 : 3.3e+03f)) : ((iTemp25) ? 1e+02f * fTemp28 + 3.3e+03f : 3.4e+03f)) : ((iTemp23) ? 1.55e+03f * fTemp26 + 3.4e+03f : 4.95e+03f)) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : ((iTemp12) ? 4.95e+03f - 1.7e+03f * fTemp15 : 3.25e+03f)) : ((iTemp10) ? 3.3e+02f * fTemp13 + 3.25e+03f : 3.58e+03f)) : ((iTemp8) ? 3.58e+03f - 4e+01f * fTemp11 : 3.54e+03f)) : ((iTemp5) ? 3.54e+03f - 5.4e+02f * fTemp9 : 3e+03f)) : ((iTemp6) ? 3e+02f * fTemp7 + 3e+03f : 3.3e+03f));
			float fTemp294 = ((iTemp124) ? 0.0f : fTemp293 + fRec204[1]);
			fRec204[0] = fTemp294 - std::floor(fTemp294);
			fRec206[0] = ((iTemp127) ? fTemp290 : fRec206[1]);
			float fTemp295 = std::exp(-(fConst5 * fRec206[0]));
			fRec207[0] = ((iTemp127) ? fTemp289 : fRec207[1]);
			float fTemp296 = std::exp(-(fConst5 * fRec207[0]));
			fRec205[0] = fVec10[2] + fRec205[1] * (fTemp295 + fTemp296) - fTemp295 * fTemp296 * fRec205[2];
			float fTemp297 = ((iTemp130) ? 0.0f : fTemp293 + fRec208[1]);
			fRec208[0] = fTemp297 - std::floor(fTemp297);
			fRec210[0] = ((iTemp115) ? fTemp290 : fRec210[1]);
			float fTemp298 = std::exp(-(fConst5 * fRec210[0]));
			fRec211[0] = ((iTemp115) ? fTemp289 : fRec211[1]);
			float fTemp299 = std::exp(-(fConst5 * fRec211[0]));
			fRec209[0] = fVec8[2] + fRec209[1] * (fTemp298 + fTemp299) - fTemp298 * fTemp299 * fRec209[2];
			float fTemp300 = ((iTemp118) ? 0.0f : fTemp293 + fRec212[1]);
			fRec212[0] = fTemp300 - std::floor(fTemp300);
			fRec214[0] = ((iTemp145) ? fTemp290 : fRec214[1]);
			float fTemp301 = std::exp(-(fConst5 * fRec214[0]));
			fRec215[0] = ((iTemp145) ? fTemp289 : fRec215[1]);
			float fTemp302 = std::exp(-(fConst5 * fRec215[0]));
			fRec213[0] = fVec13[2] + fRec213[1] * (fTemp301 + fTemp302) - fTemp301 * fTemp302 * fRec213[2];
			float fTemp303 = ((iTemp148) ? 0.0f : fTemp293 + fRec216[1]);
			fRec216[0] = fTemp303 - std::floor(fTemp303);
			fRec218[0] = ((iTemp139) ? fTemp290 : fRec218[1]);
			float fTemp304 = std::exp(-(fConst5 * fRec218[0]));
			fRec219[0] = ((iTemp139) ? fTemp289 : fRec219[1]);
			float fTemp305 = std::exp(-(fConst5 * fRec219[0]));
			fRec217[0] = fVec12[2] + fRec217[1] * (fTemp304 + fTemp305) - fTemp304 * fTemp305 * fRec217[2];
			float fTemp306 = ((iTemp142) ? 0.0f : fTemp293 + fRec220[1]);
			fRec220[0] = fTemp306 - std::floor(fTemp306);
			fRec222[0] = ((iTemp133) ? fTemp290 : fRec222[1]);
			float fTemp307 = std::exp(-(fConst5 * fRec222[0]));
			fRec223[0] = ((iTemp133) ? fTemp289 : fRec223[1]);
			float fTemp308 = std::exp(-(fConst5 * fRec223[0]));
			fRec221[0] = fVec11[2] + fRec221[1] * (fTemp307 + fTemp308) - fTemp307 * fTemp308 * fRec221[2];
			float fTemp309 = ((iTemp136) ? 0.0f : fRec224[1] + fTemp293);
			fRec224[0] = fTemp309 - std::floor(fTemp309);
			fRec226[0] = ((iTemp151) ? fTemp290 : fRec226[1]);
			float fTemp310 = std::exp(-(fConst5 * fRec226[0]));
			fRec227[0] = ((iTemp151) ? fTemp289 : fRec227[1]);
			float fTemp311 = std::exp(-(fConst5 * fRec227[0]));
			fRec225[0] = fVec14[2] + fRec225[1] * (fTemp310 + fTemp311) - fTemp310 * fTemp311 * fRec225[2];
			float fTemp312 = ((iTemp154) ? 0.0f : fTemp293 + fRec228[1]);
			fRec228[0] = fTemp312 - std::floor(fTemp312);
			fRec230[0] = ((iTemp60) ? fTemp290 : fRec230[1]);
			float fTemp313 = std::exp(-(fConst5 * fRec230[0]));
			fRec231[0] = ((iTemp60) ? fTemp289 : fRec231[1]);
			float fTemp314 = std::exp(-(fConst5 * fRec231[0]));
			fRec229[0] = fVec3[2] + fRec229[1] * (fTemp313 + fTemp314) - fTemp313 * fTemp314 * fRec229[2];
			float fTemp315 = ((iTemp85) ? 0.0f : fTemp293 + fRec232[1]);
			fRec232[0] = fTemp315 - std::floor(fTemp315);
			fRec234[0] = ((iTemp91) ? fTemp290 : fRec234[1]);
			float fTemp316 = std::exp(-(fConst5 * fRec234[0]));
			fRec235[0] = ((iTemp91) ? fTemp289 : fRec235[1]);
			float fTemp317 = std::exp(-(fConst5 * fRec235[0]));
			fRec233[0] = fVec4[2] + fRec233[1] * (fTemp316 + fTemp317) - fTemp316 * fTemp317 * fRec233[2];
			float fTemp318 = ((iTemp94) ? 0.0f : fTemp293 + fRec236[1]);
			fRec236[0] = fTemp318 - std::floor(fTemp318);
			fRec238[0] = ((iTemp97) ? fTemp290 : fRec238[1]);
			float fTemp319 = std::exp(-(fConst5 * fRec238[0]));
			fRec239[0] = ((iTemp97) ? fTemp289 : fRec239[1]);
			float fTemp320 = std::exp(-(fConst5 * fRec239[0]));
			fRec237[0] = fVec5[2] + fRec237[1] * (fTemp319 + fTemp320) - fTemp319 * fTemp320 * fRec237[2];
			float fTemp321 = ((iTemp100) ? 0.0f : fTemp293 + fRec240[1]);
			fRec240[0] = fTemp321 - std::floor(fTemp321);
			fRec242[0] = ((iTemp109) ? fTemp290 : fRec242[1]);
			float fTemp322 = std::exp(-(fConst5 * fRec242[0]));
			fRec243[0] = ((iTemp109) ? fTemp289 : fRec243[1]);
			float fTemp323 = std::exp(-(fConst5 * fRec243[0]));
			fRec241[0] = fVec7[2] + fRec241[1] * (fTemp322 + fTemp323) - fTemp322 * fTemp323 * fRec241[2];
			float fTemp324 = ((iTemp112) ? 0.0f : fTemp293 + fRec244[1]);
			fRec244[0] = fTemp324 - std::floor(fTemp324);
			fRec246[0] = ((iTemp103) ? fTemp290 : fRec246[1]);
			float fTemp325 = std::exp(-(fConst5 * fRec246[0]));
			fRec247[0] = ((iTemp103) ? fTemp289 : fRec247[1]);
			float fTemp326 = std::exp(-(fConst5 * fRec247[0]));
			fRec245[0] = fVec6[2] + fRec245[1] * (fTemp325 + fTemp326) - fTemp325 * fTemp326 * fRec245[2];
			float fTemp327 = ((iTemp106) ? 0.0f : fTemp293 + fRec248[1]);
			fRec248[0] = fTemp327 - std::floor(fTemp327);
			output0[i0] = FAUSTFLOAT(5e+01f * std::max<float>(0.0f, std::min<float>(fSlow0 * fRec0[0], 1.0f) - fSlow2 * float(iRec1[0])) * (((iSlow6) ? fTemp56 * fTemp55 : fTemp3 * fTemp55) * (fRec7[0] * (1.0f - (fTemp84 + fTemp83 * (1.0f - fTemp84))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec12[0]), 65535))] + fRec13[0] * (1.0f - (fTemp93 + fTemp92 * (1.0f - fTemp93))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec16[0]), 65535))] + fRec17[0] * (1.0f - (fTemp99 + fTemp98 * (1.0f - fTemp99))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec20[0]), 65535))] + fRec21[0] * (1.0f - (fTemp105 + fTemp104 * (1.0f - fTemp105))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec24[0]), 65535))] + fRec25[0] * (1.0f - (fTemp111 + fTemp110 * (1.0f - fTemp111))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec28[0]), 65535))] + fRec29[0] * (1.0f - (fTemp117 + fTemp116 * (1.0f - fTemp117))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec32[0]), 65535))] + fRec33[0] * (1.0f - (fTemp123 + fTemp122 * (1.0f - fTemp123))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec36[0]), 65535))] + fRec37[0] * (1.0f - (fTemp129 + fTemp128 * (1.0f - fTemp129))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec40[0]), 65535))] + fRec41[0] * (1.0f - (fTemp135 + fTemp134 * (1.0f - fTemp135))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec44[0]), 65535))] + fRec45[0] * (1.0f - (fTemp141 + fTemp140 * (1.0f - fTemp141))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec48[0]), 65535))] + fRec49[0] * (1.0f - (fTemp147 + fTemp146 * (1.0f - fTemp147))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec52[0]), 65535))] + fRec53[0] * (1.0f - (fTemp152 + fTemp153 * (1.0f - fTemp152))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec56[0]), 65535))]) + ((iSlow6) ? fTemp56 * fTemp156 : fTemp3 * fTemp156) * (fRec57[0] * (1.0f - (fTemp165 + fTemp164 * (1.0f - fTemp165))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec60[0]), 65535))] + fRec61[0] * (1.0f - (fTemp170 + fTemp169 * (1.0f - fTemp170))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec64[0]), 65535))] + fRec65[0] * (1.0f - (fTemp173 + fTemp172 * (1.0f - fTemp173))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec68[0]), 65535))] + fRec69[0] * (1.0f - (fTemp176 + fTemp175 * (1.0f - fTemp176))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec72[0]), 65535))] + fRec73[0] * (1.0f - (fTemp179 + fTemp178 * (1.0f - fTemp179))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec76[0]), 65535))] + fRec77[0] * (1.0f - (fTemp182 + fTemp181 * (1.0f - fTemp182))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec80[0]), 65535))] + fRec81[0] * (1.0f - (fTemp185 + fTemp184 * (1.0f - fTemp185))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec84[0]), 65535))] + fRec85[0] * (1.0f - (fTemp188 + fTemp187 * (1.0f - fTemp188))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec88[0]), 65535))] + fRec89[0] * (1.0f - (fTemp190 + fTemp191 * (1.0f - fTemp190))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec92[0]), 65535))] + fRec93[0] * (1.0f - (fTemp194 + fTemp193 * (1.0f - fTemp194))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec96[0]), 65535))] + fRec97[0] * (1.0f - (fTemp197 + fTemp196 * (1.0f - fTemp197))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec100[0]), 65535))] + fRec101[0] * (1.0f - (fTemp200 + fTemp199 * (1.0f - fTemp200))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec104[0]), 65535))]) + ((iSlow6) ? fTemp56 : fTemp3) * (fRec105[0] * (1.0f - (fTemp207 + fTemp206 * (1.0f - fTemp207))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec108[0]), 65535))] + fRec109[0] * (1.0f - (fTemp214 + fTemp213 * (1.0f - fTemp214))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec112[0]), 65535))] + fRec113[0] * (1.0f - (fTemp217 + fTemp216 * (1.0f - fTemp217))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec116[0]), 65535))] + fRec117[0] * (1.0f - (fTemp220 + fTemp219 * (1.0f - fTemp220))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec120[0]), 65535))] + fRec121[0] * (1.0f - (fTemp223 + fTemp222 * (1.0f - fTemp223))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec124[0]), 65535))] + fRec125[0] * (1.0f - (fTemp226 + fTemp225 * (1.0f - fTemp226))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec128[0]), 65535))] + fRec129[0] * (1.0f - (fTemp229 + fTemp228 * (1.0f - fTemp229))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec132[0]), 65535))] + fRec133[0] * (1.0f - (fTemp232 + fTemp231 * (1.0f - fTemp232))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec136[0]), 65535))] + fRec137[0] * (1.0f - (fTemp235 + fTemp234 * (1.0f - fTemp235))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec140[0]), 65535))] + fRec141[0] * (1.0f - (fTemp238 + fTemp237 * (1.0f - fTemp238))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec144[0]), 65535))] + fRec145[0] * (fTemp240 * (fTemp241 + -1.0f) + (1.0f - fTemp241)) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec148[0]), 65535))] + fRec149[0] * (1.0f - (fTemp244 + fTemp243 * (1.0f - fTemp244))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec152[0]), 65535))]) + ((iSlow6) ? fTemp56 * fTemp246 : fTemp3 * fTemp246) * (fRec153[0] * (1.0f - (fTemp250 + fTemp249 * (1.0f - fTemp250))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec156[0]), 65535))] + fRec157[0] * (1.0f - (fTemp256 + fTemp255 * (1.0f - fTemp256))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec160[0]), 65535))] + fRec161[0] * (1.0f - (fTemp259 + fTemp258 * (1.0f - fTemp259))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec164[0]), 65535))] + fRec165[0] * (1.0f - (fTemp262 + fTemp261 * (1.0f - fTemp262))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec168[0]), 65535))] + fRec169[0] * (1.0f - (fTemp265 + fTemp264 * (1.0f - fTemp265))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec172[0]), 65535))] + fRec173[0] * (1.0f - (fTemp268 + fTemp267 * (1.0f - fTemp268))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec176[0]), 65535))] + fRec177[0] * (1.0f - (fTemp271 + fTemp270 * (1.0f - fTemp271))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec180[0]), 65535))] + fRec181[0] * (1.0f - (fTemp274 + fTemp273 * (1.0f - fTemp274))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec184[0]), 65535))] + fRec185[0] * (1.0f - (fTemp276 + fTemp277 * (1.0f - fTemp276))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec188[0]), 65535))] + fRec189[0] * (1.0f - (fTemp280 + fTemp279 * (1.0f - fTemp280))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec192[0]), 65535))] + fRec193[0] * (1.0f - (fTemp282 + fTemp283 * (1.0f - fTemp282))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec196[0]), 65535))] + fRec197[0] * (1.0f - (fTemp286 + fTemp285 * (1.0f - fTemp286))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec200[0]), 65535))]) + ((iSlow6) ? fTemp56 * fTemp288 : fTemp3 * fTemp288) * (fRec201[0] * (1.0f - (fTemp292 + fTemp291 * (1.0f - fTemp292))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec204[0]), 65535))] + fRec205[0] * (1.0f - (fTemp296 + fTemp295 * (1.0f - fTemp296))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec208[0]), 65535))] + fRec209[0] * (1.0f - (fTemp299 + fTemp298 * (1.0f - fTemp299))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec212[0]), 65535))] + fRec213[0] * (1.0f - (fTemp302 + fTemp301 * (1.0f - fTemp302))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec216[0]), 65535))] + fRec217[0] * (1.0f - (fTemp305 + fTemp304 * (1.0f - fTemp305))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec220[0]), 65535))] + fRec221[0] * (1.0f - (fTemp308 + fTemp307 * (1.0f - fTemp308))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec224[0]), 65535))] + fRec225[0] * (1.0f - (fTemp311 + fTemp310 * (1.0f - fTemp311))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec228[0]), 65535))] + fRec229[0] * (1.0f - (fTemp314 + fTemp313 * (1.0f - fTemp314))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec232[0]), 65535))] + fRec233[0] * (1.0f - (fTemp317 + fTemp316 * (1.0f - fTemp317))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec236[0]), 65535))] + fRec237[0] * (1.0f - (fTemp320 + fTemp319 * (1.0f - fTemp320))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec240[0]), 65535))] + fRec241[0] * (1.0f - (fTemp323 + fTemp322 * (1.0f - fTemp323))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec244[0]), 65535))] + fRec245[0] * (1.0f - (fTemp326 + fTemp325 * (1.0f - fTemp326))) * ftbl0synth_voice_fofcSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec248[0]), 65535))])));
			fVec0[1] = fVec0[0];
			iVec1[1] = iVec1[0];
			fRec0[1] = fRec0[0];
			iRec1[1] = iRec1[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			fVec3[2] = fVec3[1];
			fVec3[1] = fVec3[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec12[1] = fRec12[0];
			fVec4[2] = fVec4[1];
			fVec4[1] = fVec4[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec16[1] = fRec16[0];
			fVec5[2] = fVec5[1];
			fVec5[1] = fVec5[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec20[1] = fRec20[0];
			fVec6[2] = fVec6[1];
			fVec6[1] = fVec6[0];
			fRec22[1] = fRec22[0];
			fRec23[1] = fRec23[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec24[1] = fRec24[0];
			fVec7[2] = fVec7[1];
			fVec7[1] = fVec7[0];
			fRec26[1] = fRec26[0];
			fRec27[1] = fRec27[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec28[1] = fRec28[0];
			fVec8[2] = fVec8[1];
			fVec8[1] = fVec8[0];
			fRec30[1] = fRec30[0];
			fRec31[1] = fRec31[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec32[1] = fRec32[0];
			fVec9[2] = fVec9[1];
			fVec9[1] = fVec9[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec36[1] = fRec36[0];
			fVec10[2] = fVec10[1];
			fVec10[1] = fVec10[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec40[1] = fRec40[0];
			fVec11[2] = fVec11[1];
			fVec11[1] = fVec11[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec44[1] = fRec44[0];
			fVec12[2] = fVec12[1];
			fVec12[1] = fVec12[0];
			fRec46[1] = fRec46[0];
			fRec47[1] = fRec47[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec48[1] = fRec48[0];
			fVec13[2] = fVec13[1];
			fVec13[1] = fVec13[0];
			fRec50[1] = fRec50[0];
			fRec51[1] = fRec51[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec52[1] = fRec52[0];
			fVec14[2] = fVec14[1];
			fVec14[1] = fVec14[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec56[1] = fRec56[0];
			fRec58[1] = fRec58[0];
			fRec59[1] = fRec59[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec60[1] = fRec60[0];
			fRec62[1] = fRec62[0];
			fRec63[1] = fRec63[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec64[1] = fRec64[0];
			fRec66[1] = fRec66[0];
			fRec67[1] = fRec67[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec68[1] = fRec68[0];
			fRec70[1] = fRec70[0];
			fRec71[1] = fRec71[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec72[1] = fRec72[0];
			fRec74[1] = fRec74[0];
			fRec75[1] = fRec75[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec76[1] = fRec76[0];
			fRec78[1] = fRec78[0];
			fRec79[1] = fRec79[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec80[1] = fRec80[0];
			fRec82[1] = fRec82[0];
			fRec83[1] = fRec83[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec84[1] = fRec84[0];
			fRec86[1] = fRec86[0];
			fRec87[1] = fRec87[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fRec88[1] = fRec88[0];
			fRec90[1] = fRec90[0];
			fRec91[1] = fRec91[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec92[1] = fRec92[0];
			fRec94[1] = fRec94[0];
			fRec95[1] = fRec95[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec96[1] = fRec96[0];
			fRec98[1] = fRec98[0];
			fRec99[1] = fRec99[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec100[1] = fRec100[0];
			fRec102[1] = fRec102[0];
			fRec103[1] = fRec103[0];
			fRec101[2] = fRec101[1];
			fRec101[1] = fRec101[0];
			fRec104[1] = fRec104[0];
			fRec106[1] = fRec106[0];
			fRec107[1] = fRec107[0];
			fRec105[2] = fRec105[1];
			fRec105[1] = fRec105[0];
			fRec108[1] = fRec108[0];
			fRec110[1] = fRec110[0];
			fRec111[1] = fRec111[0];
			fRec109[2] = fRec109[1];
			fRec109[1] = fRec109[0];
			fRec112[1] = fRec112[0];
			fRec114[1] = fRec114[0];
			fRec115[1] = fRec115[0];
			fRec113[2] = fRec113[1];
			fRec113[1] = fRec113[0];
			fRec116[1] = fRec116[0];
			fRec118[1] = fRec118[0];
			fRec119[1] = fRec119[0];
			fRec117[2] = fRec117[1];
			fRec117[1] = fRec117[0];
			fRec120[1] = fRec120[0];
			fRec122[1] = fRec122[0];
			fRec123[1] = fRec123[0];
			fRec121[2] = fRec121[1];
			fRec121[1] = fRec121[0];
			fRec124[1] = fRec124[0];
			fRec126[1] = fRec126[0];
			fRec127[1] = fRec127[0];
			fRec125[2] = fRec125[1];
			fRec125[1] = fRec125[0];
			fRec128[1] = fRec128[0];
			fRec130[1] = fRec130[0];
			fRec131[1] = fRec131[0];
			fRec129[2] = fRec129[1];
			fRec129[1] = fRec129[0];
			fRec132[1] = fRec132[0];
			fRec134[1] = fRec134[0];
			fRec135[1] = fRec135[0];
			fRec133[2] = fRec133[1];
			fRec133[1] = fRec133[0];
			fRec136[1] = fRec136[0];
			fRec138[1] = fRec138[0];
			fRec139[1] = fRec139[0];
			fRec137[2] = fRec137[1];
			fRec137[1] = fRec137[0];
			fRec140[1] = fRec140[0];
			fRec142[1] = fRec142[0];
			fRec143[1] = fRec143[0];
			fRec141[2] = fRec141[1];
			fRec141[1] = fRec141[0];
			fRec144[1] = fRec144[0];
			fRec146[1] = fRec146[0];
			fRec147[1] = fRec147[0];
			fRec145[2] = fRec145[1];
			fRec145[1] = fRec145[0];
			fRec148[1] = fRec148[0];
			fRec150[1] = fRec150[0];
			fRec151[1] = fRec151[0];
			fRec149[2] = fRec149[1];
			fRec149[1] = fRec149[0];
			fRec152[1] = fRec152[0];
			fRec154[1] = fRec154[0];
			fRec155[1] = fRec155[0];
			fRec153[2] = fRec153[1];
			fRec153[1] = fRec153[0];
			fRec156[1] = fRec156[0];
			fRec158[1] = fRec158[0];
			fRec159[1] = fRec159[0];
			fRec157[2] = fRec157[1];
			fRec157[1] = fRec157[0];
			fRec160[1] = fRec160[0];
			fRec162[1] = fRec162[0];
			fRec163[1] = fRec163[0];
			fRec161[2] = fRec161[1];
			fRec161[1] = fRec161[0];
			fRec164[1] = fRec164[0];
			fRec166[1] = fRec166[0];
			fRec167[1] = fRec167[0];
			fRec165[2] = fRec165[1];
			fRec165[1] = fRec165[0];
			fRec168[1] = fRec168[0];
			fRec170[1] = fRec170[0];
			fRec171[1] = fRec171[0];
			fRec169[2] = fRec169[1];
			fRec169[1] = fRec169[0];
			fRec172[1] = fRec172[0];
			fRec174[1] = fRec174[0];
			fRec175[1] = fRec175[0];
			fRec173[2] = fRec173[1];
			fRec173[1] = fRec173[0];
			fRec176[1] = fRec176[0];
			fRec178[1] = fRec178[0];
			fRec179[1] = fRec179[0];
			fRec177[2] = fRec177[1];
			fRec177[1] = fRec177[0];
			fRec180[1] = fRec180[0];
			fRec182[1] = fRec182[0];
			fRec183[1] = fRec183[0];
			fRec181[2] = fRec181[1];
			fRec181[1] = fRec181[0];
			fRec184[1] = fRec184[0];
			fRec186[1] = fRec186[0];
			fRec187[1] = fRec187[0];
			fRec185[2] = fRec185[1];
			fRec185[1] = fRec185[0];
			fRec188[1] = fRec188[0];
			fRec190[1] = fRec190[0];
			fRec191[1] = fRec191[0];
			fRec189[2] = fRec189[1];
			fRec189[1] = fRec189[0];
			fRec192[1] = fRec192[0];
			fRec194[1] = fRec194[0];
			fRec195[1] = fRec195[0];
			fRec193[2] = fRec193[1];
			fRec193[1] = fRec193[0];
			fRec196[1] = fRec196[0];
			fRec198[1] = fRec198[0];
			fRec199[1] = fRec199[0];
			fRec197[2] = fRec197[1];
			fRec197[1] = fRec197[0];
			fRec200[1] = fRec200[0];
			fRec202[1] = fRec202[0];
			fRec203[1] = fRec203[0];
			fRec201[2] = fRec201[1];
			fRec201[1] = fRec201[0];
			fRec204[1] = fRec204[0];
			fRec206[1] = fRec206[0];
			fRec207[1] = fRec207[0];
			fRec205[2] = fRec205[1];
			fRec205[1] = fRec205[0];
			fRec208[1] = fRec208[0];
			fRec210[1] = fRec210[0];
			fRec211[1] = fRec211[0];
			fRec209[2] = fRec209[1];
			fRec209[1] = fRec209[0];
			fRec212[1] = fRec212[0];
			fRec214[1] = fRec214[0];
			fRec215[1] = fRec215[0];
			fRec213[2] = fRec213[1];
			fRec213[1] = fRec213[0];
			fRec216[1] = fRec216[0];
			fRec218[1] = fRec218[0];
			fRec219[1] = fRec219[0];
			fRec217[2] = fRec217[1];
			fRec217[1] = fRec217[0];
			fRec220[1] = fRec220[0];
			fRec222[1] = fRec222[0];
			fRec223[1] = fRec223[0];
			fRec221[2] = fRec221[1];
			fRec221[1] = fRec221[0];
			fRec224[1] = fRec224[0];
			fRec226[1] = fRec226[0];
			fRec227[1] = fRec227[0];
			fRec225[2] = fRec225[1];
			fRec225[1] = fRec225[0];
			fRec228[1] = fRec228[0];
			fRec230[1] = fRec230[0];
			fRec231[1] = fRec231[0];
			fRec229[2] = fRec229[1];
			fRec229[1] = fRec229[0];
			fRec232[1] = fRec232[0];
			fRec234[1] = fRec234[0];
			fRec235[1] = fRec235[0];
			fRec233[2] = fRec233[1];
			fRec233[1] = fRec233[0];
			fRec236[1] = fRec236[0];
			fRec238[1] = fRec238[0];
			fRec239[1] = fRec239[0];
			fRec237[2] = fRec237[1];
			fRec237[1] = fRec237[0];
			fRec240[1] = fRec240[0];
			fRec242[1] = fRec242[0];
			fRec243[1] = fRec243[0];
			fRec241[2] = fRec241[1];
			fRec241[1] = fRec241[0];
			fRec244[1] = fRec244[0];
			fRec246[1] = fRec246[0];
			fRec247[1] = fRec247[0];
			fRec245[2] = fRec245[1];
			fRec245[1] = fRec245[0];
			fRec248[1] = fRec248[0];
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
