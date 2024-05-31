/* ------------------------------------------------------------
author: "Romain Michon"
copyright: "Romain Michon (rmichon@ccrma.stanford.edu)"
name: "synth.glass_harm"
version: "1.0"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_glass_harm -scn synth_glass_harm_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_glass_harm_H__
#define  __synth_glass_harm_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_glass_harm_dsp.h ********************************
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

class FAUST_API synth_glass_harm_dsp {

    public:

        synth_glass_harm_dsp() {}
        virtual ~synth_glass_harm_dsp() {}

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
        virtual synth_glass_harm_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_glass_harm_dsp {

    protected:

        synth_glass_harm_dsp* fDSP;

    public:

        decorator_dsp(synth_glass_harm_dsp* synth_glass_harm_dsp = nullptr):fDSP(synth_glass_harm_dsp) {}
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
        virtual synth_glass_harm_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_glass_harm_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_glass_harm_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_glass_harm : public synth_glass_harm_dsp {
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
#define FAUSTCLASS synth_glass_harm
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

class synth_glass_harmSIG0 {
	
  private:
	
	int iVec9[2];
	int iRec35[2];
	
  public:
	
	int getNumInputssynth_glass_harmSIG0() {
		return 0;
	}
	int getNumOutputssynth_glass_harmSIG0() {
		return 1;
	}
	
	void instanceInitsynth_glass_harmSIG0(int sample_rate) {
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			iVec9[l33] = 0;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			iRec35[l34] = 0;
		}
	}
	
	void fillsynth_glass_harmSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec9[0] = 1;
			iRec35[0] = (iVec9[1] + iRec35[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec35[0]));
			iVec9[1] = iVec9[0];
			iRec35[1] = iRec35[0];
		}
	}

};

static synth_glass_harmSIG0* newsynth_glass_harmSIG0() { return (synth_glass_harmSIG0*)new synth_glass_harmSIG0(); }
static void deletesynth_glass_harmSIG0(synth_glass_harmSIG0* dsp) { delete dsp; }

static float synth_glass_harm_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0synth_glass_harmSIG0[65536];

class synth_glass_harm : public synth_glass_harm_dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fHslider0;
	float fConst4;
	int iVec0[2];
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fRec11[2];
	float fRec10[2];
	int IOTA0;
	float fVec1[16384];
	float fConst9;
	int iConst10;
	FAUSTFLOAT fHslider1;
	float fConst11;
	FAUSTFLOAT fHslider2;
	float fConst12;
	float fRec12[2];
	FAUSTFLOAT fEntry0;
	float fConst13;
	float fConst14;
	float fConst15;
	FAUSTFLOAT fEntry1;
	FAUSTFLOAT fCheckbox0;
	int iVec2[2];
	float fRec21[2];
	float fConst16;
	float fConst17;
	int iRec22[2];
	float fConst18;
	int iRec23[2];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fVec3[8192];
	float fConst19;
	FAUSTFLOAT fHslider6;
	float fConst20;
	float fConst21;
	float fRec20[3];
	float fRec19[2];
	float fRec13[2];
	float fVec4[4096];
	float fConst22;
	float fConst23;
	float fRec25[3];
	float fRec24[2];
	float fRec14[2];
	float fVec5[2048];
	float fConst24;
	float fConst25;
	float fRec27[3];
	float fRec26[2];
	float fRec15[2];
	float fVec6[1024];
	float fConst26;
	float fConst27;
	float fRec29[3];
	float fRec28[2];
	float fRec16[2];
	float fVec7[1024];
	float fConst28;
	float fConst29;
	float fRec31[3];
	float fRec30[2];
	float fRec17[2];
	float fConst30;
	float fRec33[3];
	float fRec18[2];
	float fVec8[2];
	FAUSTFLOAT fHslider7;
	float fRec34[2];
	float fConst31;
	FAUSTFLOAT fHslider8;
	float fRec37[2];
	float fRec36[2];
	float fRec43[2];
	float fRec42[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[2];
	float fRec38[2];
	float fRec49[2];
	float fRec48[2];
	float fRec47[2];
	float fRec46[2];
	float fRec45[2];
	float fRec44[2];
	float fVec10[4096];
	float fVec11[4096];
	int iConst32;
	float fVec12[2048];
	int iConst33;
	float fRec8[2];
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fRec53[2];
	float fRec52[2];
	float fVec13[32768];
	float fConst38;
	int iConst39;
	float fConst40;
	FAUSTFLOAT fHslider9;
	float fVec14[4096];
	float fVec15[4096];
	int iConst41;
	float fRec50[2];
	float fConst42;
	float fConst43;
	float fConst44;
	float fConst45;
	float fRec57[2];
	float fRec56[2];
	float fVec16[16384];
	float fConst46;
	int iConst47;
	float fVec17[4096];
	int iConst48;
	float fRec54[2];
	float fConst49;
	float fConst50;
	float fConst51;
	float fConst52;
	float fRec61[2];
	float fRec60[2];
	float fVec18[16384];
	float fConst53;
	int iConst54;
	float fVec19[4096];
	int iConst55;
	float fRec58[2];
	float fConst56;
	float fConst57;
	float fConst58;
	float fConst59;
	float fRec65[2];
	float fRec64[2];
	float fVec20[32768];
	float fConst60;
	int iConst61;
	float fVec21[2048];
	int iConst62;
	float fRec62[2];
	float fConst63;
	float fConst64;
	float fConst65;
	float fConst66;
	float fRec69[2];
	float fRec68[2];
	float fVec22[16384];
	float fConst67;
	int iConst68;
	float fVec23[4096];
	int iConst69;
	float fRec66[2];
	float fConst70;
	float fConst71;
	float fConst72;
	float fConst73;
	float fRec73[2];
	float fRec72[2];
	float fVec24[32768];
	float fConst74;
	int iConst75;
	float fVec25[4096];
	int iConst76;
	float fRec70[2];
	float fConst77;
	float fConst78;
	float fConst79;
	float fConst80;
	float fRec77[2];
	float fRec76[2];
	float fVec26[16384];
	float fConst81;
	int iConst82;
	float fVec27[2048];
	int iConst83;
	float fRec74[2];
	float fRec0[3];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fVec28[4096];
	
 public:
	synth_glass_harm() {
	}
	
	void metadata(Meta* m) { 
		m->declare("author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.17.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.4");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_glass_harm -scn synth_glass_harm_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("description", "Nonlinear Banded Waveguide Modeled Glass Harmonica");
		m->declare("envelopes.lib/adsr:author", "Yann Orlarey and Andrey Bundin");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "1.3.0");
		m->declare("filename", "synth_glass_harm.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/allpassnn:author", "Julius O. Smith III");
		m->declare("filters.lib/allpassnn:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpassnn:license", "MIT-style STK-4.3 license");
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
		m->declare("filters.lib/version", "1.3.0");
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
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "synth.glass_harm");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "1.3.0");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.5.0");
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
		synth_glass_harmSIG0* sig0 = newsynth_glass_harmSIG0();
		sig0->instanceInitsynth_glass_harmSIG0(sample_rate);
		sig0->fillsynth_glass_harmSIG0(65536, ftbl0synth_glass_harmSIG0);
		deletesynth_glass_harmSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::floor(0.125f * fConst0 + 0.5f);
		fConst2 = fConst1 / fConst0;
		fConst3 = 1.7269388f * fConst2;
		fConst4 = std::cos(37699.113f / fConst0);
		fConst5 = 1.1512926f * fConst2;
		fConst6 = 1.0f / std::tan(628.31854f / fConst0);
		fConst7 = 1.0f / (fConst6 + 1.0f);
		fConst8 = 1.0f - fConst6;
		fConst9 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst10 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst1 - fConst9)));
		fConst11 = 44.1f / fConst0;
		fConst12 = 1.0f - fConst11;
		fConst13 = 100.53097f / fConst0;
		fConst14 = synth_glass_harm_faustpower2_f(1.0f - fConst13);
		fConst15 = 0.5f * (1.0f - fConst14);
		fConst16 = 0.02f * fConst0;
		fConst17 = 1.0f / std::max<float>(1.0f, fConst16);
		fConst18 = 1.0f / std::max<float>(1.0f, 0.01f * fConst0);
		fConst19 = 0.0022727272f * fConst0;
		fConst20 = 2.0f * (fConst13 - 1.0f);
		fConst21 = 2764.6016f / fConst0;
		fConst22 = 0.0009796239f * fConst0;
		fConst23 = 6413.8755f / fConst0;
		fConst24 = 0.00053475937f * fConst0;
		fConst25 = 11749.557f / fConst0;
		fConst26 = 0.00034279446f * fConst0;
		fConst27 = 18329.309f / fConst0;
		fConst28 = 0.00024229502f * fConst0;
		fConst29 = 25931.963f / fConst0;
		fConst30 = 24881.414f / fConst0;
		fConst31 = 1.0f / fConst0;
		iConst32 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst16)));
		iConst33 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst9 + -1.0f)));
		fConst34 = std::floor(0.210389f * fConst0 + 0.5f);
		fConst35 = fConst34 / fConst0;
		fConst36 = 1.7269388f * fConst35;
		fConst37 = 1.1512926f * fConst35;
		fConst38 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst39 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst34 - fConst38)));
		fConst40 = 0.0011363636f * fConst0;
		iConst41 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst38 + -1.0f)));
		fConst42 = std::floor(0.174713f * fConst0 + 0.5f);
		fConst43 = fConst42 / fConst0;
		fConst44 = 1.7269388f * fConst43;
		fConst45 = 1.1512926f * fConst43;
		fConst46 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst47 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst42 - fConst46)));
		iConst48 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst46 + -1.0f)));
		fConst49 = std::floor(0.127837f * fConst0 + 0.5f);
		fConst50 = fConst49 / fConst0;
		fConst51 = 1.7269388f * fConst50;
		fConst52 = 1.1512926f * fConst50;
		fConst53 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst54 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst49 - fConst53)));
		iConst55 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst53 + -1.0f)));
		fConst56 = std::floor(0.219991f * fConst0 + 0.5f);
		fConst57 = fConst56 / fConst0;
		fConst58 = 1.7269388f * fConst57;
		fConst59 = 1.1512926f * fConst57;
		fConst60 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst61 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst56 - fConst60)));
		iConst62 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst60 + -1.0f)));
		fConst63 = std::floor(0.192303f * fConst0 + 0.5f);
		fConst64 = fConst63 / fConst0;
		fConst65 = 1.7269388f * fConst64;
		fConst66 = 1.1512926f * fConst64;
		fConst67 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst68 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst63 - fConst67)));
		iConst69 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst67 + -1.0f)));
		fConst70 = std::floor(0.256891f * fConst0 + 0.5f);
		fConst71 = fConst70 / fConst0;
		fConst72 = 1.7269388f * fConst71;
		fConst73 = 1.1512926f * fConst71;
		fConst74 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst75 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst70 - fConst74)));
		iConst76 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst74 + -1.0f)));
		fConst77 = std::floor(0.153129f * fConst0 + 0.5f);
		fConst78 = fConst77 / fConst0;
		fConst79 = 1.7269388f * fConst78;
		fConst80 = 1.1512926f * fConst78;
		fConst81 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst82 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst77 - fConst81)));
		iConst83 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst81 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.36f);
		fHslider1 = FAUSTFLOAT(0.6f);
		fHslider2 = FAUSTFLOAT(0.137f);
		fEntry0 = FAUSTFLOAT(0.0f);
		fEntry1 = FAUSTFLOAT(0.0f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(1.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.2f);
		fHslider6 = FAUSTFLOAT(48.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(2.2e+02f);
		fHslider9 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec11[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec10[l2] = 0.0f;
		}
		IOTA0 = 0;
		for (int l3 = 0; l3 < 16384; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec12[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iVec2[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec21[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iRec22[l7] = 0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			iRec23[l8] = 0;
		}
		for (int l9 = 0; l9 < 8192; l9 = l9 + 1) {
			fVec3[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec20[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec19[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec13[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 4096; l13 = l13 + 1) {
			fVec4[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec25[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec24[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2048; l17 = l17 + 1) {
			fVec5[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec27[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec26[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec15[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 1024; l21 = l21 + 1) {
			fVec6[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec29[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec28[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec16[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 1024; l25 = l25 + 1) {
			fVec7[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec31[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec30[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec17[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 3; l29 = l29 + 1) {
			fRec33[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec18[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fVec8[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec34[l32] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec37[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec36[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec43[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec42[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec41[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec40[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec39[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec38[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec49[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec48[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec47[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec46[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec45[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec44[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 4096; l49 = l49 + 1) {
			fVec10[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 4096; l50 = l50 + 1) {
			fVec11[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2048; l51 = l51 + 1) {
			fVec12[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec8[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec53[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec52[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 32768; l55 = l55 + 1) {
			fVec13[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 4096; l56 = l56 + 1) {
			fVec14[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 4096; l57 = l57 + 1) {
			fVec15[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec50[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec57[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec56[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 16384; l61 = l61 + 1) {
			fVec16[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 4096; l62 = l62 + 1) {
			fVec17[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec54[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec61[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec60[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 16384; l66 = l66 + 1) {
			fVec18[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 4096; l67 = l67 + 1) {
			fVec19[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec58[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fRec65[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec64[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 32768; l71 = l71 + 1) {
			fVec20[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2048; l72 = l72 + 1) {
			fVec21[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec62[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fRec69[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec68[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 16384; l76 = l76 + 1) {
			fVec22[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 4096; l77 = l77 + 1) {
			fVec23[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec66[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec73[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec72[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 32768; l81 = l81 + 1) {
			fVec24[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 4096; l82 = l82 + 1) {
			fVec25[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec70[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fRec77[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec76[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 16384; l86 = l86 + 1) {
			fVec26[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2048; l87 = l87 + 1) {
			fVec27[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec74[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 3; l89 = l89 + 1) {
			fRec0[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 3; l90 = l90 + 1) {
			fRec1[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
			fRec2[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 3; l92 = l92 + 1) {
			fRec3[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 3; l93 = l93 + 1) {
			fRec4[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 3; l94 = l94 + 1) {
			fRec5[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 3; l95 = l95 + 1) {
			fRec6[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 3; l96 = l96 + 1) {
			fRec7[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 4096; l97 = l97 + 1) {
			fVec28[l97] = 0.0f;
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
	
	virtual synth_glass_harm* clone() {
		return new synth_glass_harm();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.glass_harm");
		ui_interface->declare(&fHslider5, "tooltip", "Bow pressure on the instrument (Value between 0 and 1)");
		ui_interface->addHorizontalSlider("bow_press", &fHslider5, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "tooltip", "A value between 0 and 1");
		ui_interface->addHorizontalSlider("gain", &fHslider3, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fCheckbox0, "type", "float");
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->declare(&fHslider4, "tooltip", "A value between 0 and 1");
		ui_interface->addHorizontalSlider("integration", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider8, "tooltip", "Frequency of the sine wave for the modulation of theta (works if Modulation Type=3)");
		ui_interface->declare(&fHslider8, "unit", "Hz");
		ui_interface->addHorizontalSlider("modfreq", &fHslider8, FAUSTFLOAT(2.2e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fEntry0, "tooltip", "0=theta is modulated by the incoming signal; 1=theta is modulated by the averaged incoming signal; 2=theta is modulated by the squared incoming signal; 3=theta is modulated by a sine wave of frequency freqMod; 4=theta is modulated by a sine wave of frequency freq;");
		ui_interface->declare(&fEntry0, "type", "int");
		ui_interface->addNumEntry("modtype", &fEntry0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider7, "tooltip", "Nonlinearity factor (value between 0 and 1)");
		ui_interface->addHorizontalSlider("nonlin", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pan", &fHslider1, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pitch", &fHslider6, FAUSTFLOAT(48.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(96.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, FAUSTFLOAT(0.137f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("room_size", &fHslider0, FAUSTFLOAT(0.36f), FAUSTFLOAT(0.005f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fEntry1, "tooltip", "0=Bow; 1=Strike");
		ui_interface->declare(&fEntry1, "type", "int");
		ui_interface->addNumEntry("strike", &fEntry1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("width", &fHslider9, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::exp(-(fConst3 / fSlow0));
		float fSlow2 = synth_glass_harm_faustpower2_f(fSlow1);
		float fSlow3 = 1.0f - fConst4 * fSlow2;
		float fSlow4 = 1.0f - fSlow2;
		float fSlow5 = std::sqrt(std::max<float>(0.0f, synth_glass_harm_faustpower2_f(fSlow3) / synth_glass_harm_faustpower2_f(fSlow4) + -1.0f));
		float fSlow6 = fSlow3 / fSlow4;
		float fSlow7 = fSlow1 * (fSlow5 + (1.0f - fSlow6));
		float fSlow8 = std::exp(-(fConst5 / fSlow0)) / fSlow1 + -1.0f;
		float fSlow9 = fSlow6 - fSlow5;
		float fSlow10 = float(fHslider1);
		float fSlow11 = 1.0f - fSlow10;
		float fSlow12 = fConst11 * float(fHslider2);
		int iSlow13 = int(float(fEntry0));
		float fSlow14 = float(iSlow13 >= 3);
		float fSlow15 = float(fEntry1);
		float fSlow16 = float(fCheckbox0);
		int iSlow17 = fSlow16 > 0.0f;
		float fSlow18 = 0.2f * fSlow15 * float(iSlow17);
		float fSlow19 = fConst11 * std::min<float>(1.0f, std::max<float>(0.0f, fSlow16));
		float fSlow20 = 0.16666667f * (fSlow15 + -1.0f);
		int iSlow21 = iSlow17 == 0;
		float fSlow22 = 0.1f * float(fHslider3) + 0.9f;
		float fSlow23 = float(fHslider4);
		float fSlow24 = 1e+01f - 9.0f * float(fHslider5);
		float fSlow25 = std::pow(2.0f, 0.083333336f * (float(fHslider6) + -69.0f));
		int iSlow26 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst19 / fSlow25)));
		float fSlow27 = fConst20 * std::cos(fConst21 * fSlow25);
		int iSlow28 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst22 / fSlow25)));
		float fSlow29 = fConst20 * std::cos(fConst23 * fSlow25);
		int iSlow30 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst24 / fSlow25)));
		float fSlow31 = fConst20 * std::cos(fConst25 * fSlow25);
		int iSlow32 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst26 / fSlow25)));
		float fSlow33 = fConst20 * std::cos(fConst27 * fSlow25);
		int iSlow34 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst28 / fSlow25)));
		float fSlow35 = fConst20 * std::cos(fConst29 * fSlow25);
		float fSlow36 = fConst20 * std::cos(fConst30 * fSlow25);
		float fSlow37 = fConst11 * float(fHslider7);
		float fSlow38 = 4.4e+02f * fSlow25 * float(iSlow13 == 4);
		float fSlow39 = float(iSlow13 != 4);
		float fSlow40 = fConst11 * float(fHslider8);
		float fSlow41 = float(iSlow13 < 3);
		float fSlow42 = 12.566371f * float(iSlow13 == 0);
		float fSlow43 = 6.2831855f * float(iSlow13 == 1);
		float fSlow44 = 50.265484f * float(iSlow13 == 2);
		float fSlow45 = std::exp(-(fConst36 / fSlow0));
		float fSlow46 = synth_glass_harm_faustpower2_f(fSlow45);
		float fSlow47 = 1.0f - fConst4 * fSlow46;
		float fSlow48 = 1.0f - fSlow46;
		float fSlow49 = std::sqrt(std::max<float>(0.0f, synth_glass_harm_faustpower2_f(fSlow47) / synth_glass_harm_faustpower2_f(fSlow48) + -1.0f));
		float fSlow50 = fSlow47 / fSlow48;
		float fSlow51 = fSlow45 * (fSlow49 + (1.0f - fSlow50));
		float fSlow52 = std::exp(-(fConst37 / fSlow0)) / fSlow45 + -1.0f;
		float fSlow53 = fSlow50 - fSlow49;
		int iSlow54 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst40 * (float(fHslider9) / fSlow25))));
		float fSlow55 = std::exp(-(fConst44 / fSlow0));
		float fSlow56 = synth_glass_harm_faustpower2_f(fSlow55);
		float fSlow57 = 1.0f - fConst4 * fSlow56;
		float fSlow58 = 1.0f - fSlow56;
		float fSlow59 = std::sqrt(std::max<float>(0.0f, synth_glass_harm_faustpower2_f(fSlow57) / synth_glass_harm_faustpower2_f(fSlow58) + -1.0f));
		float fSlow60 = fSlow57 / fSlow58;
		float fSlow61 = fSlow55 * (fSlow59 + (1.0f - fSlow60));
		float fSlow62 = std::exp(-(fConst45 / fSlow0)) / fSlow55 + -1.0f;
		float fSlow63 = fSlow60 - fSlow59;
		float fSlow64 = std::exp(-(fConst51 / fSlow0));
		float fSlow65 = synth_glass_harm_faustpower2_f(fSlow64);
		float fSlow66 = 1.0f - fConst4 * fSlow65;
		float fSlow67 = 1.0f - fSlow65;
		float fSlow68 = std::sqrt(std::max<float>(0.0f, synth_glass_harm_faustpower2_f(fSlow66) / synth_glass_harm_faustpower2_f(fSlow67) + -1.0f));
		float fSlow69 = fSlow66 / fSlow67;
		float fSlow70 = fSlow64 * (fSlow68 + (1.0f - fSlow69));
		float fSlow71 = std::exp(-(fConst52 / fSlow0)) / fSlow64 + -1.0f;
		float fSlow72 = fSlow69 - fSlow68;
		float fSlow73 = std::exp(-(fConst58 / fSlow0));
		float fSlow74 = synth_glass_harm_faustpower2_f(fSlow73);
		float fSlow75 = 1.0f - fConst4 * fSlow74;
		float fSlow76 = 1.0f - fSlow74;
		float fSlow77 = std::sqrt(std::max<float>(0.0f, synth_glass_harm_faustpower2_f(fSlow75) / synth_glass_harm_faustpower2_f(fSlow76) + -1.0f));
		float fSlow78 = fSlow75 / fSlow76;
		float fSlow79 = fSlow73 * (fSlow77 + (1.0f - fSlow78));
		float fSlow80 = std::exp(-(fConst59 / fSlow0)) / fSlow73 + -1.0f;
		float fSlow81 = fSlow78 - fSlow77;
		float fSlow82 = std::exp(-(fConst65 / fSlow0));
		float fSlow83 = synth_glass_harm_faustpower2_f(fSlow82);
		float fSlow84 = 1.0f - fConst4 * fSlow83;
		float fSlow85 = 1.0f - fSlow83;
		float fSlow86 = std::sqrt(std::max<float>(0.0f, synth_glass_harm_faustpower2_f(fSlow84) / synth_glass_harm_faustpower2_f(fSlow85) + -1.0f));
		float fSlow87 = fSlow84 / fSlow85;
		float fSlow88 = fSlow82 * (fSlow86 + (1.0f - fSlow87));
		float fSlow89 = std::exp(-(fConst66 / fSlow0)) / fSlow82 + -1.0f;
		float fSlow90 = fSlow87 - fSlow86;
		float fSlow91 = std::exp(-(fConst72 / fSlow0));
		float fSlow92 = synth_glass_harm_faustpower2_f(fSlow91);
		float fSlow93 = 1.0f - fConst4 * fSlow92;
		float fSlow94 = 1.0f - fSlow92;
		float fSlow95 = std::sqrt(std::max<float>(0.0f, synth_glass_harm_faustpower2_f(fSlow93) / synth_glass_harm_faustpower2_f(fSlow94) + -1.0f));
		float fSlow96 = fSlow93 / fSlow94;
		float fSlow97 = fSlow91 * (fSlow95 + (1.0f - fSlow96));
		float fSlow98 = std::exp(-(fConst73 / fSlow0)) / fSlow91 + -1.0f;
		float fSlow99 = fSlow96 - fSlow95;
		float fSlow100 = std::exp(-(fConst79 / fSlow0));
		float fSlow101 = synth_glass_harm_faustpower2_f(fSlow100);
		float fSlow102 = 1.0f - fConst4 * fSlow101;
		float fSlow103 = 1.0f - fSlow101;
		float fSlow104 = std::sqrt(std::max<float>(0.0f, synth_glass_harm_faustpower2_f(fSlow102) / synth_glass_harm_faustpower2_f(fSlow103) + -1.0f));
		float fSlow105 = fSlow102 / fSlow103;
		float fSlow106 = fSlow100 * (fSlow104 + (1.0f - fSlow105));
		float fSlow107 = std::exp(-(fConst80 / fSlow0)) / fSlow100 + -1.0f;
		float fSlow108 = fSlow105 - fSlow104;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec11[0] = -(fConst7 * (fConst8 * fRec11[1] - (fRec6[1] + fRec6[2])));
			fRec10[0] = fSlow7 * (fRec6[1] + fSlow8 * fRec11[0]) + fSlow9 * fRec10[1];
			fVec1[IOTA0 & 16383] = 0.35355338f * fRec10[0] + 1e-20f;
			fRec12[0] = fSlow12 + fConst12 * fRec12[1];
			iVec2[0] = iSlow17;
			fRec21[0] = fSlow19 + fConst12 * fRec21[1];
			float fTemp0 = fSlow18 * fRec21[0];
			iRec22[0] = iSlow17 + iRec22[1] * (iVec2[1] >= iSlow17);
			iRec23[0] = iSlow21 * (iRec23[1] + 1);
			float fTemp1 = (0.1f * fRec21[0] + 0.03f) * std::max<float>(0.0f, std::min<float>(fConst17 * float(iRec22[0]), 1.0f) * (1.0f - fConst18 * float(iRec23[0]))) - fSlow22 * (fRec13[1] + fRec15[1] + fRec17[1] + fRec14[1] + fRec16[1] + fRec18[1]) - fSlow23;
			float fTemp2 = std::pow(std::fabs(fSlow24 * fTemp1) + 0.75f, -4.0f);
			float fTemp3 = fSlow20 * fTemp1 * (float(fTemp2 > 1.0f) + fTemp2 * float(fTemp2 <= 1.0f));
			fVec3[IOTA0 & 8191] = fRec19[1] + fTemp0 - fTemp3;
			fRec20[0] = 0.999f * fVec3[(IOTA0 - iSlow26) & 8191] - (fSlow27 * fRec20[1] + fConst14 * fRec20[2]);
			fRec19[0] = fConst15 * (fRec20[0] - fRec20[2]);
			fRec13[0] = fRec19[0];
			fVec4[IOTA0 & 4095] = fTemp0 + fRec24[1] - fTemp3;
			fRec25[0] = 0.998001f * fVec4[(IOTA0 - iSlow28) & 4095] - (fSlow29 * fRec25[1] + fConst14 * fRec25[2]);
			fRec24[0] = fConst15 * (fRec25[0] - fRec25[2]);
			fRec14[0] = fRec24[0];
			fVec5[IOTA0 & 2047] = fTemp0 + fRec26[1] - fTemp3;
			fRec27[0] = 0.997003f * fVec5[(IOTA0 - iSlow30) & 2047] - (fSlow31 * fRec27[1] + fConst14 * fRec27[2]);
			fRec26[0] = fConst15 * (fRec27[0] - fRec27[2]);
			fRec15[0] = fRec26[0];
			fVec6[IOTA0 & 1023] = fTemp0 + fRec28[1] - fTemp3;
			fRec29[0] = 0.996006f * fVec6[(IOTA0 - iSlow32) & 1023] - (fSlow33 * fRec29[1] + fConst14 * fRec29[2]);
			fRec28[0] = fConst15 * (fRec29[0] - fRec29[2]);
			fRec16[0] = fRec28[0];
			fVec7[IOTA0 & 1023] = fTemp0 + fRec30[1] - fTemp3;
			fRec31[0] = 0.99501f * fVec7[(IOTA0 - iSlow34) & 1023] - (fSlow35 * fRec31[1] + fConst14 * fRec31[2]);
			fRec30[0] = fConst15 * (fRec31[0] - fRec31[2]);
			fRec17[0] = fRec30[0];
			fRec33[0] = -(fSlow36 * fRec33[1] + fConst14 * fRec33[2]);
			float fRec32 = fConst15 * (fRec33[0] - fRec33[2]);
			fRec18[0] = fRec32;
			float fTemp4 = fRec18[0] + fRec16[0] + fRec13[0] + fRec15[0] + fRec17[0] + fRec14[0];
			fVec8[0] = fTemp4;
			fRec34[0] = fSlow37 + fConst12 * fRec34[1];
			fRec37[0] = fSlow40 + fConst12 * fRec37[1];
			float fTemp5 = ((1 - iVec0[1]) ? 0.0f : fRec36[1] + fConst31 * (fSlow38 + fSlow39 * fRec37[0]));
			fRec36[0] = fTemp5 - std::floor(fTemp5);
			float fTemp6 = 3.1415927f * fRec34[0] * ftbl0synth_glass_harmSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec36[0]), 65535))];
			float fTemp7 = std::sin(fTemp6);
			float fTemp8 = std::cos(fTemp6);
			float fTemp9 = 4.0f * fTemp4 * fTemp8 - fTemp7 * fRec38[1];
			float fTemp10 = fTemp8 * fTemp9 - fTemp7 * fRec39[1];
			float fTemp11 = fTemp8 * fTemp10 - fTemp7 * fRec40[1];
			float fTemp12 = fTemp8 * fTemp11 - fTemp7 * fRec41[1];
			float fTemp13 = fTemp8 * fTemp12 - fTemp7 * fRec42[1];
			fRec43[0] = fTemp8 * fTemp13 - fTemp7 * fRec43[1];
			fRec42[0] = fTemp7 * fTemp13 + fTemp8 * fRec43[1];
			fRec41[0] = fTemp7 * fTemp12 + fTemp8 * fRec42[1];
			fRec40[0] = fTemp7 * fTemp11 + fTemp8 * fRec41[1];
			fRec39[0] = fTemp7 * fTemp10 + fTemp8 * fRec40[1];
			fRec38[0] = fTemp7 * fTemp9 + fTemp8 * fRec39[1];
			float fTemp14 = fSlow14 * (4.0f * fTemp4 * fTemp7 + fRec38[1] * fTemp8);
			float fTemp15 = fSlow43 * (fTemp4 + fVec8[1]);
			float fTemp16 = fRec34[0] * (fSlow42 * fTemp4 + fTemp15 + fSlow44 * synth_glass_harm_faustpower2_f(fTemp4));
			float fTemp17 = std::sin(fTemp16);
			float fTemp18 = std::cos(fTemp16);
			float fTemp19 = 4.0f * fTemp4 * fTemp18 - fTemp17 * fRec44[1];
			float fTemp20 = fTemp18 * fTemp19 - fTemp17 * fRec45[1];
			float fTemp21 = fTemp18 * fTemp20 - fTemp17 * fRec46[1];
			float fTemp22 = fTemp18 * fTemp21 - fTemp17 * fRec47[1];
			float fTemp23 = fTemp18 * fTemp22 - fTemp17 * fRec48[1];
			fRec49[0] = fTemp18 * fTemp23 - fTemp17 * fRec49[1];
			fRec48[0] = fTemp17 * fTemp23 + fTemp18 * fRec49[1];
			fRec47[0] = fTemp17 * fTemp22 + fTemp18 * fRec48[1];
			fRec46[0] = fTemp17 * fTemp21 + fTemp18 * fRec47[1];
			fRec45[0] = fTemp17 * fTemp20 + fTemp18 * fRec46[1];
			fRec44[0] = fTemp17 * fTemp19 + fTemp18 * fRec45[1];
			float fTemp24 = 4.0f * (1.0f - fRec34[0]) * fTemp4;
			float fTemp25 = fTemp14 + fSlow41 * (fRec34[0] * (4.0f * fTemp4 * fTemp17 + fRec44[1] * fTemp18) + fTemp24);
			fVec10[IOTA0 & 4095] = fTemp25;
			fVec11[IOTA0 & 4095] = fSlow11 * fRec12[0] * fTemp25;
			float fTemp26 = 0.3f * fVec11[(IOTA0 - iConst32) & 4095];
			float fTemp27 = fVec1[(IOTA0 - iConst10) & 16383] - (fTemp26 + 0.6f * fRec8[1]);
			fVec12[IOTA0 & 2047] = fTemp27;
			fRec8[0] = fVec12[(IOTA0 - iConst33) & 2047];
			float fRec9 = 0.6f * fTemp27;
			fRec53[0] = -(fConst7 * (fConst8 * fRec53[1] - (fRec1[1] + fRec1[2])));
			fRec52[0] = fSlow51 * (fRec1[1] + fSlow52 * fRec53[0]) + fSlow53 * fRec52[1];
			fVec13[IOTA0 & 32767] = 0.35355338f * fRec52[0] + 1e-20f;
			fVec14[IOTA0 & 4095] = fSlow10 * fRec12[0] * fVec10[(IOTA0 - iSlow54) & 4095];
			float fTemp28 = 0.3f * fVec14[(IOTA0 - iConst32) & 4095];
			float fTemp29 = fVec13[(IOTA0 - iConst39) & 32767] + 0.6f * fRec50[1] + fTemp28;
			fVec15[IOTA0 & 4095] = fTemp29;
			fRec50[0] = fVec15[(IOTA0 - iConst41) & 4095];
			float fRec51 = -(0.6f * fTemp29);
			fRec57[0] = -(fConst7 * (fConst8 * fRec57[1] - (fRec4[1] + fRec4[2])));
			fRec56[0] = fSlow61 * (fRec4[1] + fSlow62 * fRec57[0]) + fSlow63 * fRec56[1];
			fVec16[IOTA0 & 16383] = 0.35355338f * fRec56[0] + 1e-20f;
			float fTemp30 = fTemp26 + fVec16[(IOTA0 - iConst47) & 16383] - 0.6f * fRec54[1];
			fVec17[IOTA0 & 4095] = fTemp30;
			fRec54[0] = fVec17[(IOTA0 - iConst48) & 4095];
			float fRec55 = 0.6f * fTemp30;
			fRec61[0] = -(fConst7 * (fConst8 * fRec61[1] - (fRec2[1] + fRec2[2])));
			fRec60[0] = fSlow70 * (fRec2[1] + fSlow71 * fRec61[0]) + fSlow72 * fRec60[1];
			fVec18[IOTA0 & 16383] = 0.35355338f * fRec60[0] + 1e-20f;
			float fTemp31 = fVec18[(IOTA0 - iConst54) & 16383] - (fTemp26 + 0.6f * fRec58[1]);
			fVec19[IOTA0 & 4095] = fTemp31;
			fRec58[0] = fVec19[(IOTA0 - iConst55) & 4095];
			float fRec59 = 0.6f * fTemp31;
			fRec65[0] = -(fConst7 * (fConst8 * fRec65[1] - (fRec7[1] + fRec7[2])));
			fRec64[0] = fSlow79 * (fRec7[1] + fSlow80 * fRec65[0]) + fSlow81 * fRec64[1];
			fVec20[IOTA0 & 32767] = 0.35355338f * fRec64[0] + 1e-20f;
			float fTemp32 = 0.6f * fRec62[1] + fVec20[(IOTA0 - iConst61) & 32767];
			fVec21[IOTA0 & 2047] = fTemp32 - fTemp28;
			fRec62[0] = fVec21[(IOTA0 - iConst62) & 2047];
			float fRec63 = 0.6f * (fTemp28 - fTemp32);
			fRec69[0] = -(fConst7 * (fConst8 * fRec69[1] - (fRec5[1] + fRec5[2])));
			fRec68[0] = fSlow88 * (fRec5[1] + fSlow89 * fRec69[0]) + fSlow90 * fRec68[1];
			fVec22[IOTA0 & 16383] = 0.35355338f * fRec68[0] + 1e-20f;
			float fTemp33 = fVec22[(IOTA0 - iConst68) & 16383] + fTemp28 + 0.6f * fRec66[1];
			fVec23[IOTA0 & 4095] = fTemp33;
			fRec66[0] = fVec23[(IOTA0 - iConst69) & 4095];
			float fRec67 = -(0.6f * fTemp33);
			fRec73[0] = -(fConst7 * (fConst8 * fRec73[1] - (fRec3[1] + fRec3[2])));
			fRec72[0] = fSlow97 * (fRec3[1] + fSlow98 * fRec73[0]) + fSlow99 * fRec72[1];
			fVec24[IOTA0 & 32767] = 0.35355338f * fRec72[0] + 1e-20f;
			float fTemp34 = 0.6f * fRec70[1] + fVec24[(IOTA0 - iConst75) & 32767];
			fVec25[IOTA0 & 4095] = fTemp34 - fTemp28;
			fRec70[0] = fVec25[(IOTA0 - iConst76) & 4095];
			float fRec71 = 0.6f * (fTemp28 - fTemp34);
			fRec77[0] = -(fConst7 * (fConst8 * fRec77[1] - (fRec0[1] + fRec0[2])));
			fRec76[0] = fSlow106 * (fRec0[1] + fSlow107 * fRec77[0]) + fSlow108 * fRec76[1];
			fVec26[IOTA0 & 16383] = 0.35355338f * fRec76[0] + 1e-20f;
			float fTemp35 = fVec26[(IOTA0 - iConst82) & 16383] + fTemp26 - 0.6f * fRec74[1];
			fVec27[IOTA0 & 2047] = fTemp35;
			fRec74[0] = fVec27[(IOTA0 - iConst83) & 2047];
			float fRec75 = 0.6f * fTemp35;
			float fTemp36 = fRec67 + fRec63;
			float fTemp37 = fRec71 + fTemp36;
			fRec0[0] = fRec8[1] + fRec50[1] + fRec54[1] + fRec58[1] + fRec62[1] + fRec66[1] + fRec70[1] + fRec74[1] + fRec59 + fRec55 + fRec51 + fRec9 + fRec75 + fTemp37;
			fRec1[0] = fRec8[1] + fRec54[1] + fRec58[1] + fRec74[1] + fRec59 + fRec55 + fRec75 + fRec9 - (fRec50[1] + fRec62[1] + fRec66[1] + fRec70[1] + fRec51 + fTemp37);
			float fTemp38 = fRec71 + fRec63;
			fRec2[0] = fRec50[1] + fRec54[1] + fRec66[1] + fRec74[1] + fRec55 + fRec51 + fRec75 + fRec67 - (fRec8[1] + fRec58[1] + fRec62[1] + fRec70[1] + fRec59 + fRec9 + fTemp38);
			fRec3[0] = fRec54[1] + fRec62[1] + fRec70[1] + fRec74[1] + fRec55 + fRec75 + fTemp38 - (fRec8[1] + fRec50[1] + fRec58[1] + fRec66[1] + fRec59 + fRec51 + fRec9 + fRec67);
			fRec4[0] = fRec50[1] + fRec58[1] + fRec70[1] + fRec74[1] + fRec59 + fRec51 + fRec75 + fRec71 - (fRec8[1] + fRec54[1] + fRec62[1] + fRec66[1] + fRec55 + fRec9 + fTemp36);
			fRec5[0] = fRec58[1] + fRec62[1] + fRec66[1] + fRec74[1] + fRec59 + fRec75 + fTemp36 - (fRec8[1] + fRec50[1] + fRec54[1] + fRec70[1] + fRec55 + fRec51 + fRec9 + fRec71);
			float fTemp39 = fRec67 + fRec71;
			fRec6[0] = fRec8[1] + fRec50[1] + fRec62[1] + fRec74[1] + fRec51 + fRec9 + fRec75 + fRec63 - (fRec54[1] + fRec58[1] + fRec66[1] + fRec70[1] + fRec59 + fRec55 + fTemp39);
			fRec7[0] = fRec8[1] + fRec66[1] + fRec70[1] + fRec74[1] + fRec9 + fRec75 + fTemp39 - (fRec50[1] + fRec54[1] + fRec58[1] + fRec62[1] + fRec59 + fRec55 + fRec51 + fRec63);
			float fTemp40 = 1.0f - fRec12[0];
			float fTemp41 = fRec34[0] * (fTemp15 + fTemp4 * (fSlow42 + fSlow44 * fTemp4));
			float fTemp42 = fTemp14 + fSlow41 * (fTemp24 + fRec34[0] * (4.0f * fTemp4 * std::sin(fTemp41) + fRec44[1] * std::cos(fTemp41)));
			fVec28[IOTA0 & 4095] = fTemp42;
			output0[i0] = FAUSTFLOAT(0.37f * (fRec1[0] + fRec2[0]) + fSlow11 * fTemp40 * fTemp42);
			output1[i0] = FAUSTFLOAT(0.37f * (fRec1[0] - fRec2[0]) + fSlow10 * fTemp40 * fVec28[(IOTA0 - iSlow54) & 4095]);
			iVec0[1] = iVec0[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA0 = IOTA0 + 1;
			fRec12[1] = fRec12[0];
			iVec2[1] = iVec2[0];
			fRec21[1] = fRec21[0];
			iRec22[1] = iRec22[0];
			iRec23[1] = iRec23[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec13[1] = fRec13[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec14[1] = fRec14[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec15[1] = fRec15[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec16[1] = fRec16[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec17[1] = fRec17[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec18[1] = fRec18[0];
			fVec8[1] = fVec8[0];
			fRec34[1] = fRec34[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec8[1] = fRec8[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec50[1] = fRec50[0];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fRec54[1] = fRec54[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec58[1] = fRec58[0];
			fRec65[1] = fRec65[0];
			fRec64[1] = fRec64[0];
			fRec62[1] = fRec62[0];
			fRec69[1] = fRec69[0];
			fRec68[1] = fRec68[0];
			fRec66[1] = fRec66[0];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec70[1] = fRec70[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec74[1] = fRec74[0];
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

class faust_synth_glass_harm_tilde : public FaustExternal<synth_glass_harm> {
public:
    faust_synth_glass_harm_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_glass_harm))
    {
    }
};

#endif
