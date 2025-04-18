/* ------------------------------------------------------------
name: "synth.dx7"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_dx7 -scn synth_dx7_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_dx7_H__
#define  __synth_dx7_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_dx7_dsp.h ********************************
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

class FAUST_API synth_dx7_dsp {

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
        virtual synth_dx7_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_dx7_dsp {

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
        virtual synth_dx7_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_dx7_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_dx7_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_dx7 : public synth_dx7_dsp {
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
#define FAUSTCLASS synth_dx7
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

class synth_dx7SIG0 {
	
  private:
	
	int iVec1[2];
	int iRec3[2];
	
  public:
	
	int getNumInputssynth_dx7SIG0() {
		return 0;
	}
	int getNumOutputssynth_dx7SIG0() {
		return 1;
	}
	
	void instanceInitsynth_dx7SIG0(int sample_rate) {
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iVec1[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec3[l5] = 0;
		}
	}
	
	void fillsynth_dx7SIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[0] = 1;
			iRec3[0] = (iVec1[1] + iRec3[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec3[0]));
			iVec1[1] = iVec1[0];
			iRec3[1] = iRec3[0];
		}
	}

};

static synth_dx7SIG0* newsynth_dx7SIG0() { return (synth_dx7SIG0*)new synth_dx7SIG0(); }
static void deletesynth_dx7SIG0(synth_dx7SIG0* dsp) { delete dsp; }

static float synth_dx7_faustpower8_f(float value) {
	return value * value * value * value * value * value * value * value;
}
static float ftbl0synth_dx7SIG0[65536];

class synth_dx7 : public synth_dx7_dsp {
	
 private:
	
	FAUSTFLOAT fEntry0;
	int iVec0[2];
	FAUSTFLOAT fButton0;
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fEntry1;
	FAUSTFLOAT fEntry2;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fEntry3;
	FAUSTFLOAT fEntry4;
	FAUSTFLOAT fEntry5;
	FAUSTFLOAT fEntry6;
	FAUSTFLOAT fEntry7;
	FAUSTFLOAT fEntry8;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fEntry9;
	float fRec0[2];
	FAUSTFLOAT fEntry10;
	FAUSTFLOAT fEntry11;
	FAUSTFLOAT fEntry12;
	FAUSTFLOAT fEntry13;
	FAUSTFLOAT fEntry14;
	float fRec2[2];
	int iTempPerm0;
	float fConst1;
	int iTempPerm1;
	float fRec1[2];
	float fTempPerm2;
	int iTempPerm3;
	float fTempPerm4;
	float fRec4[2];
	FAUSTFLOAT fEntry15;
	FAUSTFLOAT fEntry16;
	FAUSTFLOAT fEntry17;
	FAUSTFLOAT fEntry18;
	FAUSTFLOAT fEntry19;
	FAUSTFLOAT fEntry20;
	FAUSTFLOAT fEntry21;
	FAUSTFLOAT fEntry22;
	FAUSTFLOAT fEntry23;
	float fRec5[2];
	FAUSTFLOAT fEntry24;
	FAUSTFLOAT fEntry25;
	FAUSTFLOAT fEntry26;
	FAUSTFLOAT fEntry27;
	FAUSTFLOAT fEntry28;
	float fRec7[2];
	int iTempPerm5;
	int iTempPerm6;
	float fRec6[2];
	float fTempPerm7;
	float fTempPerm8;
	float fRec8[2];
	float fTempPerm9;
	float fTempPerm10;
	float fTempPerm11;
	FAUSTFLOAT fEntry29;
	FAUSTFLOAT fEntry30;
	FAUSTFLOAT fEntry31;
	FAUSTFLOAT fEntry32;
	FAUSTFLOAT fEntry33;
	FAUSTFLOAT fEntry34;
	FAUSTFLOAT fEntry35;
	FAUSTFLOAT fEntry36;
	FAUSTFLOAT fEntry37;
	float fRec9[2];
	FAUSTFLOAT fEntry38;
	FAUSTFLOAT fEntry39;
	FAUSTFLOAT fEntry40;
	FAUSTFLOAT fEntry41;
	FAUSTFLOAT fEntry42;
	float fRec11[2];
	int iTempPerm12;
	int iTempPerm13;
	float fRec10[2];
	float fTempPerm14;
	float fTempPerm15;
	float fRec12[2];
	FAUSTFLOAT fEntry43;
	FAUSTFLOAT fEntry44;
	FAUSTFLOAT fEntry45;
	FAUSTFLOAT fEntry46;
	FAUSTFLOAT fEntry47;
	FAUSTFLOAT fEntry48;
	FAUSTFLOAT fEntry49;
	FAUSTFLOAT fEntry50;
	FAUSTFLOAT fEntry51;
	float fRec13[2];
	FAUSTFLOAT fEntry52;
	FAUSTFLOAT fEntry53;
	FAUSTFLOAT fEntry54;
	FAUSTFLOAT fEntry55;
	FAUSTFLOAT fEntry56;
	float fRec15[2];
	int iTempPerm16;
	int iTempPerm17;
	float fRec14[2];
	float fTempPerm18;
	float fTempPerm19;
	float fRec16[2];
	FAUSTFLOAT fEntry57;
	FAUSTFLOAT fEntry58;
	FAUSTFLOAT fEntry59;
	FAUSTFLOAT fEntry60;
	FAUSTFLOAT fEntry61;
	FAUSTFLOAT fEntry62;
	FAUSTFLOAT fEntry63;
	FAUSTFLOAT fEntry64;
	FAUSTFLOAT fEntry65;
	float fRec17[2];
	FAUSTFLOAT fEntry66;
	FAUSTFLOAT fEntry67;
	FAUSTFLOAT fEntry68;
	FAUSTFLOAT fEntry69;
	FAUSTFLOAT fEntry70;
	float fRec19[2];
	int iTempPerm20;
	int iTempPerm21;
	float fRec18[2];
	float fTempPerm22;
	float fTempPerm23;
	float fRec20[2];
	FAUSTFLOAT fEntry71;
	FAUSTFLOAT fEntry72;
	FAUSTFLOAT fEntry73;
	FAUSTFLOAT fEntry74;
	FAUSTFLOAT fEntry75;
	FAUSTFLOAT fEntry76;
	FAUSTFLOAT fEntry77;
	FAUSTFLOAT fEntry78;
	FAUSTFLOAT fEntry79;
	float fRec22[2];
	FAUSTFLOAT fEntry80;
	FAUSTFLOAT fEntry81;
	FAUSTFLOAT fEntry82;
	FAUSTFLOAT fEntry83;
	FAUSTFLOAT fEntry84;
	float fRec24[2];
	int iTempPerm24;
	int iTempPerm25;
	float fRec23[2];
	float fTempPerm26;
	float fTempPerm27;
	float fRec25[2];
	float fTempPerm28;
	FAUSTFLOAT fEntry85;
	float fRec21[2];
	float fTempPerm29;
	float fTempPerm30;
	float fTempPerm31;
	float fRec26[2];
	float fTempPerm32;
	float fTempPerm33;
	float fRec27[2];
	float fTempPerm34;
	float fTempPerm35;
	float fTempPerm36;
	float fTempPerm37;
	float fRec28[2];
	float fTempPerm38;
	float fTempPerm39;
	float fRec29[2];
	float fTempPerm40;
	float fTempPerm41;
	float fTempPerm42;
	float fRec30[2];
	float fTempPerm43;
	float fTempPerm44;
	float fTempPerm45;
	float fTempPerm46;
	float fRec31[2];
	float fTempPerm47;
	float fRec32[2];
	float fTempPerm48;
	float fTempPerm49;
	float fTempPerm50;
	float fTempPerm51;
	float fTempPerm52;
	float fTempPerm53;
	float fTempPerm54;
	float fTempPerm55;
	float fTempPerm56;
	float fRec33[2];
	float fTempPerm57;
	
 public:
	synth_dx7() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.17.1");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_dx7 -scn synth_dx7_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("dx7.lib/version", "1.1.0");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/dx7envelope:author", "Romain Michon");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "1.3.0");
		m->declare("filename", "synth_dx7.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "synth.dx7");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("spn.lib/name", "Standart Pitch Notation constants");
		m->declare("spn.lib/version", "0.2");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		synth_dx7SIG0* sig0 = newsynth_dx7SIG0();
		sig0->instanceInitsynth_dx7SIG0(sample_rate);
		sig0->fillsynth_dx7SIG0(65536, ftbl0synth_dx7SIG0);
		deletesynth_dx7SIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fEntry0 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fEntry1 = FAUSTFLOAT(95.0f);
		fEntry2 = FAUSTFLOAT(9e+01f);
		fHslider0 = FAUSTFLOAT(0.8f);
		fEntry3 = FAUSTFLOAT(1.0f);
		fEntry4 = FAUSTFLOAT(0.0f);
		fEntry5 = FAUSTFLOAT(9e+01f);
		fEntry6 = FAUSTFLOAT(0.0f);
		fEntry7 = FAUSTFLOAT(1.0f);
		fEntry8 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(48.0f);
		fEntry9 = FAUSTFLOAT(1.0f);
		fEntry10 = FAUSTFLOAT(0.0f);
		fEntry11 = FAUSTFLOAT(9e+01f);
		fEntry12 = FAUSTFLOAT(9e+01f);
		fEntry13 = FAUSTFLOAT(9e+01f);
		fEntry14 = FAUSTFLOAT(9e+01f);
		fEntry15 = FAUSTFLOAT(95.0f);
		fEntry16 = FAUSTFLOAT(9e+01f);
		fEntry17 = FAUSTFLOAT(1.0f);
		fEntry18 = FAUSTFLOAT(0.0f);
		fEntry19 = FAUSTFLOAT(9e+01f);
		fEntry20 = FAUSTFLOAT(0.0f);
		fEntry21 = FAUSTFLOAT(1.0f);
		fEntry22 = FAUSTFLOAT(0.0f);
		fEntry23 = FAUSTFLOAT(1.0f);
		fEntry24 = FAUSTFLOAT(0.0f);
		fEntry25 = FAUSTFLOAT(9e+01f);
		fEntry26 = FAUSTFLOAT(9e+01f);
		fEntry27 = FAUSTFLOAT(9e+01f);
		fEntry28 = FAUSTFLOAT(9e+01f);
		fEntry29 = FAUSTFLOAT(95.0f);
		fEntry30 = FAUSTFLOAT(9e+01f);
		fEntry31 = FAUSTFLOAT(1.0f);
		fEntry32 = FAUSTFLOAT(0.0f);
		fEntry33 = FAUSTFLOAT(9e+01f);
		fEntry34 = FAUSTFLOAT(0.0f);
		fEntry35 = FAUSTFLOAT(1.0f);
		fEntry36 = FAUSTFLOAT(0.0f);
		fEntry37 = FAUSTFLOAT(1.0f);
		fEntry38 = FAUSTFLOAT(0.0f);
		fEntry39 = FAUSTFLOAT(9e+01f);
		fEntry40 = FAUSTFLOAT(9e+01f);
		fEntry41 = FAUSTFLOAT(9e+01f);
		fEntry42 = FAUSTFLOAT(9e+01f);
		fEntry43 = FAUSTFLOAT(95.0f);
		fEntry44 = FAUSTFLOAT(9e+01f);
		fEntry45 = FAUSTFLOAT(1.0f);
		fEntry46 = FAUSTFLOAT(0.0f);
		fEntry47 = FAUSTFLOAT(9e+01f);
		fEntry48 = FAUSTFLOAT(0.0f);
		fEntry49 = FAUSTFLOAT(1.0f);
		fEntry50 = FAUSTFLOAT(0.0f);
		fEntry51 = FAUSTFLOAT(1.0f);
		fEntry52 = FAUSTFLOAT(0.0f);
		fEntry53 = FAUSTFLOAT(9e+01f);
		fEntry54 = FAUSTFLOAT(9e+01f);
		fEntry55 = FAUSTFLOAT(9e+01f);
		fEntry56 = FAUSTFLOAT(9e+01f);
		fEntry57 = FAUSTFLOAT(95.0f);
		fEntry58 = FAUSTFLOAT(9e+01f);
		fEntry59 = FAUSTFLOAT(1.0f);
		fEntry60 = FAUSTFLOAT(0.0f);
		fEntry61 = FAUSTFLOAT(9e+01f);
		fEntry62 = FAUSTFLOAT(0.0f);
		fEntry63 = FAUSTFLOAT(1.0f);
		fEntry64 = FAUSTFLOAT(0.0f);
		fEntry65 = FAUSTFLOAT(1.0f);
		fEntry66 = FAUSTFLOAT(0.0f);
		fEntry67 = FAUSTFLOAT(9e+01f);
		fEntry68 = FAUSTFLOAT(9e+01f);
		fEntry69 = FAUSTFLOAT(9e+01f);
		fEntry70 = FAUSTFLOAT(9e+01f);
		fEntry71 = FAUSTFLOAT(95.0f);
		fEntry72 = FAUSTFLOAT(9e+01f);
		fEntry73 = FAUSTFLOAT(1.0f);
		fEntry74 = FAUSTFLOAT(0.0f);
		fEntry75 = FAUSTFLOAT(9e+01f);
		fEntry76 = FAUSTFLOAT(0.0f);
		fEntry77 = FAUSTFLOAT(1.0f);
		fEntry78 = FAUSTFLOAT(0.0f);
		fEntry79 = FAUSTFLOAT(1.0f);
		fEntry80 = FAUSTFLOAT(0.0f);
		fEntry81 = FAUSTFLOAT(9e+01f);
		fEntry82 = FAUSTFLOAT(9e+01f);
		fEntry83 = FAUSTFLOAT(9e+01f);
		fEntry84 = FAUSTFLOAT(9e+01f);
		fEntry85 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec2[l2] = 0.0f;
		}
		iTempPerm0 = 0;
		iTempPerm1 = 0;
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec1[l3] = 0.0f;
		}
		fTempPerm2 = 0.0f;
		iTempPerm3 = 0;
		fTempPerm4 = 0.0f;
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec4[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec5[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec7[l8] = 0.0f;
		}
		iTempPerm5 = 0;
		iTempPerm6 = 0;
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec6[l9] = 0.0f;
		}
		fTempPerm7 = 0.0f;
		fTempPerm8 = 0.0f;
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec8[l10] = 0.0f;
		}
		fTempPerm9 = 0.0f;
		fTempPerm10 = 0.0f;
		fTempPerm11 = 0.0f;
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec11[l12] = 0.0f;
		}
		iTempPerm12 = 0;
		iTempPerm13 = 0;
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec10[l13] = 0.0f;
		}
		fTempPerm14 = 0.0f;
		fTempPerm15 = 0.0f;
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec15[l16] = 0.0f;
		}
		iTempPerm16 = 0;
		iTempPerm17 = 0;
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec14[l17] = 0.0f;
		}
		fTempPerm18 = 0.0f;
		fTempPerm19 = 0.0f;
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec16[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec17[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec19[l20] = 0.0f;
		}
		iTempPerm20 = 0;
		iTempPerm21 = 0;
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec18[l21] = 0.0f;
		}
		fTempPerm22 = 0.0f;
		fTempPerm23 = 0.0f;
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec20[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec22[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec24[l24] = 0.0f;
		}
		iTempPerm24 = 0;
		iTempPerm25 = 0;
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec23[l25] = 0.0f;
		}
		fTempPerm26 = 0.0f;
		fTempPerm27 = 0.0f;
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec25[l26] = 0.0f;
		}
		fTempPerm28 = 0.0f;
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec21[l27] = 0.0f;
		}
		fTempPerm29 = 0.0f;
		fTempPerm30 = 0.0f;
		fTempPerm31 = 0.0f;
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec26[l28] = 0.0f;
		}
		fTempPerm32 = 0.0f;
		fTempPerm33 = 0.0f;
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec27[l29] = 0.0f;
		}
		fTempPerm34 = 0.0f;
		fTempPerm35 = 0.0f;
		fTempPerm36 = 0.0f;
		fTempPerm37 = 0.0f;
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec28[l30] = 0.0f;
		}
		fTempPerm38 = 0.0f;
		fTempPerm39 = 0.0f;
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec29[l31] = 0.0f;
		}
		fTempPerm40 = 0.0f;
		fTempPerm41 = 0.0f;
		fTempPerm42 = 0.0f;
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec30[l32] = 0.0f;
		}
		fTempPerm43 = 0.0f;
		fTempPerm44 = 0.0f;
		fTempPerm45 = 0.0f;
		fTempPerm46 = 0.0f;
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec31[l33] = 0.0f;
		}
		fTempPerm47 = 0.0f;
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec32[l34] = 0.0f;
		}
		fTempPerm48 = 0.0f;
		fTempPerm49 = 0.0f;
		fTempPerm50 = 0.0f;
		fTempPerm51 = 0.0f;
		fTempPerm52 = 0.0f;
		fTempPerm53 = 0.0f;
		fTempPerm54 = 0.0f;
		fTempPerm55 = 0.0f;
		fTempPerm56 = 0.0f;
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec33[l35] = 0.0f;
		}
		fTempPerm57 = 0.0f;
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
		ui_interface->declare(&fEntry0, "type", "int");
		ui_interface->addNumEntry("algorithm", &fEntry0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(15.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("feedback", &fEntry85, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("gain", &fHslider0, FAUSTFLOAT(0.8f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addNumEntry("op0:detune", &fEntry7, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:egL1", &fEntry10, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:egL2", &fEntry12, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:egL3", &fEntry2, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:egL4", &fEntry4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:egR1", &fEntry11, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:egR2", &fEntry13, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:egR3", &fEntry14, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:egR4", &fEntry5, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry9, "unit", "Hz");
		ui_interface->addNumEntry("op0:freq", &fEntry9, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op0:key_vel_sens", &fEntry3, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:level", &fEntry1, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:mode", &fEntry8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:rate_scale", &fEntry6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:detune", &fEntry21, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:egL1", &fEntry24, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:egL2", &fEntry26, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:egL3", &fEntry16, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:egL4", &fEntry18, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:egR1", &fEntry25, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:egR2", &fEntry27, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:egR3", &fEntry28, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:egR4", &fEntry19, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry23, "unit", "Hz");
		ui_interface->addNumEntry("op1:freq", &fEntry23, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op1:key_vel_sens", &fEntry17, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:level", &fEntry15, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:mode", &fEntry22, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:rate_scale", &fEntry20, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:detune", &fEntry35, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egL1", &fEntry38, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egL2", &fEntry40, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egL3", &fEntry30, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egL4", &fEntry32, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egR1", &fEntry39, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egR2", &fEntry41, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egR3", &fEntry42, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egR4", &fEntry33, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry37, "unit", "Hz");
		ui_interface->addNumEntry("op2:freq", &fEntry37, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op2:key_vel_sens", &fEntry31, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:level", &fEntry29, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:mode", &fEntry36, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:rate_scale", &fEntry34, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:detune", &fEntry49, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egL1", &fEntry52, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egL2", &fEntry54, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egL3", &fEntry44, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egL4", &fEntry46, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egR1", &fEntry53, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egR2", &fEntry55, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egR3", &fEntry56, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egR4", &fEntry47, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry51, "unit", "Hz");
		ui_interface->addNumEntry("op3:freq", &fEntry51, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op3:key_vel_sens", &fEntry45, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:level", &fEntry43, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:mode", &fEntry50, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:rate_scale", &fEntry48, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:detune", &fEntry63, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egL1", &fEntry66, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egL2", &fEntry68, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egL3", &fEntry58, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egL4", &fEntry60, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egR1", &fEntry67, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egR2", &fEntry69, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egR3", &fEntry70, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egR4", &fEntry61, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry65, "unit", "Hz");
		ui_interface->addNumEntry("op4:freq", &fEntry65, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op4:key_vel_sens", &fEntry59, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:level", &fEntry57, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:mode", &fEntry64, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:rate_scale", &fEntry62, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:detune", &fEntry77, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egL1", &fEntry80, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egL2", &fEntry82, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egL3", &fEntry72, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egL4", &fEntry74, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egR1", &fEntry81, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egR2", &fEntry83, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egR3", &fEntry84, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egR4", &fEntry75, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry79, "unit", "Hz");
		ui_interface->addNumEntry("op5:freq", &fEntry79, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op5:key_vel_sens", &fEntry73, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:level", &fEntry71, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:mode", &fEntry78, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:rate_scale", &fEntry76, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("pitch", &fHslider1, FAUSTFLOAT(48.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(84.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = int(float(fEntry0));
		int iSlow1 = float(iSlow0 == 0) != 0.0f;
		int iSlow2 = float(iSlow0 == 11) != 0.0f;
		int iSlow3 = float(iSlow0 == 13) != 0.0f;
		int iSlow4 = float(iSlow0 == 15) != 0.0f;
		int iSlow5 = float(iSlow0 == 2) != 0.0f;
		int iSlow6 = float(iSlow0 == 5) != 0.0f;
		int iSlow7 = float(iSlow0 == 12) != 0.0f;
		int iSlow8 = float(iSlow0 == 10) != 0.0f;
		int iSlow9 = float(iSlow0 == 9) != 0.0f;
		int iSlow10 = float(iSlow0 == 1) != 0.0f;
		int iSlow11 = float(iSlow0 == 3) != 0.0f;
		int iSlow12 = float(iSlow0 == 7) != 0.0f;
		int iSlow13 = float(iSlow0 == 6) != 0.0f;
		int iSlow14 = float(iSlow0 == 14) != 0.0f;
		int iSlow15 = float(iSlow0 == 4) != 0.0f;
		int iSlow16 = float(iSlow0 == 8) != 0.0f;
		float fSlow17 = float(fButton0);
		int iSlow18 = fSlow17 > 0.0f;
		int iSlow19 = iSlow18 > 0;
		float fSlow20 = float(fEntry1);
		float fSlow21 = float(fEntry2);
		float fSlow22 = float(fHslider0);
		float fSlow23 = float(fEntry3);
		float fSlow24 = 0.0012626263f * fSlow22 * fSlow23 + 0.01010101f * (1.0f - 0.125f * fSlow23);
		float fSlow25 = fSlow20 * fSlow21 * fSlow24;
		float fSlow26 = float(fEntry4);
		float fSlow27 = fSlow20 * fSlow26 * fSlow24;
		int iSlow28 = fSlow25 <= fSlow27;
		float fSlow29 = std::pow(2.0f, 0.083333336f * (float(fHslider1) + -69.0f));
		float fSlow30 = float(fEntry9);
		float fSlow31 = 0.25f * float(fEntry7) + ((int(float(fEntry8))) ? fSlow30 : 4.4e+02f * fSlow29 * fSlow30);
		float fSlow32 = 0.057142857f * float(fEntry6) * (17.31234f * std::log(0.0022727272f * fSlow31) + 48.0f);
		float fSlow33 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry5) + fSlow32));
		float fSlow34 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow25), 0.7f);
		float fSlow35 = std::pow(0.014705882f * float(fSlow25 >= 3e+01f) * std::min<float>(fSlow25 + -3e+01f, 68.0f), 1.8f);
		float fSlow36 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow27), 0.7f);
		float fSlow37 = std::pow(0.014705882f * float(fSlow27 >= 3e+01f) * std::min<float>(fSlow27 + -3e+01f, 68.0f), 1.8f);
		float fSlow38 = std::max<float>(0.001f, std::fabs(((iSlow28) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow33, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow33, 12.6f))) * (((iSlow28) ? fSlow35 : fSlow34) - ((iSlow28) ? fSlow37 : fSlow36))));
		float fSlow39 = fConst0 * fSlow38;
		float fSlow40 = float(iSlow18);
		int iSlow41 = (fSlow17 == 0.0f) > 0;
		float fSlow42 = float(fEntry10);
		float fSlow43 = fSlow20 * fSlow42 * fSlow24;
		int iSlow44 = fSlow43 > fSlow27;
		float fSlow45 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry11) + fSlow32));
		float fSlow46 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow43), 0.7f);
		float fSlow47 = std::pow(0.014705882f * float(fSlow43 >= 3e+01f) * std::min<float>(fSlow43 + -3e+01f, 68.0f), 1.8f);
		float fSlow48 = std::max<float>(0.001f, std::fabs(((iSlow44) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow45, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow45, 12.6f))) * (((iSlow44) ? fSlow37 : fSlow36) - ((iSlow44) ? fSlow47 : fSlow46))));
		float fSlow49 = float(fEntry12);
		float fSlow50 = fSlow20 * fSlow49 * fSlow24;
		int iSlow51 = fSlow50 > fSlow43;
		float fSlow52 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry13) + fSlow32));
		float fSlow53 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow50), 0.7f);
		float fSlow54 = std::pow(0.014705882f * float(fSlow50 >= 3e+01f) * std::min<float>(fSlow50 + -3e+01f, 68.0f), 1.8f);
		float fSlow55 = fSlow48 + std::max<float>(0.001f, std::fabs(((iSlow51) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow52, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow52, 12.6f))) * (((iSlow51) ? fSlow54 : fSlow53) - ((iSlow51) ? fSlow47 : fSlow46))));
		int iSlow56 = fSlow25 > fSlow50;
		float fSlow57 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry14) + fSlow32));
		float fSlow58 = fSlow55 + std::max<float>(0.001f, std::fabs(((iSlow56) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow57, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow57, 12.6f))) * (((iSlow56) ? fSlow54 : fSlow53) - ((iSlow56) ? fSlow35 : fSlow34))));
		float fSlow59 = fConst0 * fSlow58;
		float fSlow60 = fConst0 * fSlow55;
		int iTemp0 = iTempPerm0;
		float fSlow61 = fSlow20 * fSlow24;
		float fSlow62 = fConst1 * (fSlow49 - fSlow21);
		float fSlow63 = fSlow55 - fSlow58;
		float fSlow64 = fConst0 * fSlow48;
		int iTemp1 = iTempPerm1;
		float fSlow65 = fConst1 * (fSlow49 - fSlow42);
		float fSlow66 = fSlow48 - fSlow55;
		float fSlow67 = fConst1 * (fSlow26 - fSlow42);
		float fTemp2 = fTempPerm2;
		int iTemp3 = iTempPerm3;
		float fSlow68 = fConst1 * fSlow31;
		float fTemp4 = fTempPerm4;
		float fSlow69 = float(fEntry15);
		float fSlow70 = float(fEntry16);
		float fSlow71 = float(fEntry17);
		float fSlow72 = 0.0012626263f * fSlow71 * fSlow22 + 0.01010101f * (1.0f - 0.125f * fSlow71);
		float fSlow73 = fSlow69 * fSlow70 * fSlow72;
		float fSlow74 = float(fEntry18);
		float fSlow75 = fSlow69 * fSlow74 * fSlow72;
		int iSlow76 = fSlow73 <= fSlow75;
		float fSlow77 = float(fEntry23);
		float fSlow78 = 0.25f * float(fEntry21) + ((int(float(fEntry22))) ? fSlow77 : 4.4e+02f * fSlow77 * fSlow29);
		float fSlow79 = 0.057142857f * float(fEntry20) * (17.31234f * std::log(0.0022727272f * fSlow78) + 48.0f);
		float fSlow80 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry19) + fSlow79));
		float fSlow81 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow73), 0.7f);
		float fSlow82 = std::pow(0.014705882f * float(fSlow73 >= 3e+01f) * std::min<float>(fSlow73 + -3e+01f, 68.0f), 1.8f);
		float fSlow83 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow75), 0.7f);
		float fSlow84 = std::pow(0.014705882f * float(fSlow75 >= 3e+01f) * std::min<float>(fSlow75 + -3e+01f, 68.0f), 1.8f);
		float fSlow85 = std::max<float>(0.001f, std::fabs(((iSlow76) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow80, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow80, 12.6f))) * (((iSlow76) ? fSlow82 : fSlow81) - ((iSlow76) ? fSlow84 : fSlow83))));
		float fSlow86 = fConst0 * fSlow85;
		float fSlow87 = float(fEntry24);
		float fSlow88 = fSlow69 * fSlow87 * fSlow72;
		int iSlow89 = fSlow88 > fSlow75;
		float fSlow90 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry25) + fSlow79));
		float fSlow91 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow88), 0.7f);
		float fSlow92 = std::pow(0.014705882f * float(fSlow88 >= 3e+01f) * std::min<float>(fSlow88 + -3e+01f, 68.0f), 1.8f);
		float fSlow93 = std::max<float>(0.001f, std::fabs(((iSlow89) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow90, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow90, 12.6f))) * (((iSlow89) ? fSlow84 : fSlow83) - ((iSlow89) ? fSlow92 : fSlow91))));
		float fSlow94 = float(fEntry26);
		float fSlow95 = fSlow69 * fSlow94 * fSlow72;
		int iSlow96 = fSlow95 > fSlow88;
		float fSlow97 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry27) + fSlow79));
		float fSlow98 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow95), 0.7f);
		float fSlow99 = std::pow(0.014705882f * float(fSlow95 >= 3e+01f) * std::min<float>(fSlow95 + -3e+01f, 68.0f), 1.8f);
		float fSlow100 = fSlow93 + std::max<float>(0.001f, std::fabs(((iSlow96) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow97, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow97, 12.6f))) * (((iSlow96) ? fSlow99 : fSlow98) - ((iSlow96) ? fSlow92 : fSlow91))));
		int iSlow101 = fSlow73 > fSlow95;
		float fSlow102 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry28) + fSlow79));
		float fSlow103 = fSlow100 + std::max<float>(0.001f, std::fabs(((iSlow101) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow102, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow102, 12.6f))) * (((iSlow101) ? fSlow99 : fSlow98) - ((iSlow101) ? fSlow82 : fSlow81))));
		float fSlow104 = fConst0 * fSlow103;
		float fSlow105 = fConst0 * fSlow100;
		int iTemp5 = iTempPerm5;
		float fSlow106 = fSlow69 * fSlow72;
		float fSlow107 = fConst1 * (fSlow94 - fSlow70);
		float fSlow108 = fSlow100 - fSlow103;
		float fSlow109 = fConst0 * fSlow93;
		int iTemp6 = iTempPerm6;
		float fSlow110 = fConst1 * (fSlow94 - fSlow87);
		float fSlow111 = fSlow93 - fSlow100;
		float fSlow112 = fConst1 * (fSlow74 - fSlow87);
		float fTemp7 = fTempPerm7;
		float fSlow113 = fConst1 * fSlow78;
		float fTemp8 = fTempPerm8;
		float fTemp9 = fTempPerm9;
		float fTemp10 = fTempPerm10;
		float fTemp11 = fTempPerm11;
		float fSlow114 = float(fEntry29);
		float fSlow115 = float(fEntry30);
		float fSlow116 = float(fEntry31);
		float fSlow117 = 0.0012626263f * fSlow22 * fSlow116 + 0.01010101f * (1.0f - 0.125f * fSlow116);
		float fSlow118 = fSlow114 * fSlow115 * fSlow117;
		float fSlow119 = float(fEntry32);
		float fSlow120 = fSlow114 * fSlow119 * fSlow117;
		int iSlow121 = fSlow118 <= fSlow120;
		float fSlow122 = float(fEntry37);
		float fSlow123 = 0.25f * float(fEntry35) + ((int(float(fEntry36))) ? fSlow122 : 4.4e+02f * fSlow29 * fSlow122);
		float fSlow124 = 0.057142857f * float(fEntry34) * (17.31234f * std::log(0.0022727272f * fSlow123) + 48.0f);
		float fSlow125 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry33) + fSlow124));
		float fSlow126 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow118), 0.7f);
		float fSlow127 = std::pow(0.014705882f * float(fSlow118 >= 3e+01f) * std::min<float>(fSlow118 + -3e+01f, 68.0f), 1.8f);
		float fSlow128 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow120), 0.7f);
		float fSlow129 = std::pow(0.014705882f * float(fSlow120 >= 3e+01f) * std::min<float>(fSlow120 + -3e+01f, 68.0f), 1.8f);
		float fSlow130 = std::max<float>(0.001f, std::fabs(((iSlow121) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow125, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow125, 12.6f))) * (((iSlow121) ? fSlow127 : fSlow126) - ((iSlow121) ? fSlow129 : fSlow128))));
		float fSlow131 = fConst0 * fSlow130;
		float fSlow132 = float(fEntry38);
		float fSlow133 = fSlow114 * fSlow132 * fSlow117;
		int iSlow134 = fSlow133 > fSlow120;
		float fSlow135 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry39) + fSlow124));
		float fSlow136 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow133), 0.7f);
		float fSlow137 = std::pow(0.014705882f * float(fSlow133 >= 3e+01f) * std::min<float>(fSlow133 + -3e+01f, 68.0f), 1.8f);
		float fSlow138 = std::max<float>(0.001f, std::fabs(((iSlow134) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow135, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow135, 12.6f))) * (((iSlow134) ? fSlow129 : fSlow128) - ((iSlow134) ? fSlow137 : fSlow136))));
		float fSlow139 = float(fEntry40);
		float fSlow140 = fSlow114 * fSlow139 * fSlow117;
		int iSlow141 = fSlow140 > fSlow133;
		float fSlow142 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry41) + fSlow124));
		float fSlow143 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow140), 0.7f);
		float fSlow144 = std::pow(0.014705882f * float(fSlow140 >= 3e+01f) * std::min<float>(fSlow140 + -3e+01f, 68.0f), 1.8f);
		float fSlow145 = fSlow138 + std::max<float>(0.001f, std::fabs(((iSlow141) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow142, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow142, 12.6f))) * (((iSlow141) ? fSlow144 : fSlow143) - ((iSlow141) ? fSlow137 : fSlow136))));
		int iSlow146 = fSlow118 > fSlow140;
		float fSlow147 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry42) + fSlow124));
		float fSlow148 = fSlow145 + std::max<float>(0.001f, std::fabs(((iSlow146) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow147, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow147, 12.6f))) * (((iSlow146) ? fSlow144 : fSlow143) - ((iSlow146) ? fSlow127 : fSlow126))));
		float fSlow149 = fConst0 * fSlow148;
		float fSlow150 = fConst0 * fSlow145;
		int iTemp12 = iTempPerm12;
		float fSlow151 = fSlow114 * fSlow117;
		float fSlow152 = fConst1 * (fSlow139 - fSlow115);
		float fSlow153 = fSlow145 - fSlow148;
		float fSlow154 = fConst0 * fSlow138;
		int iTemp13 = iTempPerm13;
		float fSlow155 = fConst1 * (fSlow139 - fSlow132);
		float fSlow156 = fSlow138 - fSlow145;
		float fSlow157 = fConst1 * (fSlow119 - fSlow132);
		float fTemp14 = fTempPerm14;
		float fSlow158 = fConst1 * fSlow123;
		float fTemp15 = fTempPerm15;
		float fSlow159 = float(fEntry43);
		float fSlow160 = float(fEntry44);
		float fSlow161 = float(fEntry45);
		float fSlow162 = 0.0012626263f * fSlow22 * fSlow161 + 0.01010101f * (1.0f - 0.125f * fSlow161);
		float fSlow163 = fSlow159 * fSlow160 * fSlow162;
		float fSlow164 = float(fEntry46);
		float fSlow165 = fSlow159 * fSlow164 * fSlow162;
		int iSlow166 = fSlow163 <= fSlow165;
		float fSlow167 = float(fEntry51);
		float fSlow168 = 0.25f * float(fEntry49) + ((int(float(fEntry50))) ? fSlow167 : 4.4e+02f * fSlow29 * fSlow167);
		float fSlow169 = 0.057142857f * float(fEntry48) * (17.31234f * std::log(0.0022727272f * fSlow168) + 48.0f);
		float fSlow170 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry47) + fSlow169));
		float fSlow171 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow163), 0.7f);
		float fSlow172 = std::pow(0.014705882f * float(fSlow163 >= 3e+01f) * std::min<float>(fSlow163 + -3e+01f, 68.0f), 1.8f);
		float fSlow173 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow165), 0.7f);
		float fSlow174 = std::pow(0.014705882f * float(fSlow165 >= 3e+01f) * std::min<float>(fSlow165 + -3e+01f, 68.0f), 1.8f);
		float fSlow175 = std::max<float>(0.001f, std::fabs(((iSlow166) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow170, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow170, 12.6f))) * (((iSlow166) ? fSlow172 : fSlow171) - ((iSlow166) ? fSlow174 : fSlow173))));
		float fSlow176 = fConst0 * fSlow175;
		float fSlow177 = float(fEntry52);
		float fSlow178 = fSlow159 * fSlow177 * fSlow162;
		int iSlow179 = fSlow178 > fSlow165;
		float fSlow180 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry53) + fSlow169));
		float fSlow181 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow178), 0.7f);
		float fSlow182 = std::pow(0.014705882f * float(fSlow178 >= 3e+01f) * std::min<float>(fSlow178 + -3e+01f, 68.0f), 1.8f);
		float fSlow183 = std::max<float>(0.001f, std::fabs(((iSlow179) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow180, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow180, 12.6f))) * (((iSlow179) ? fSlow174 : fSlow173) - ((iSlow179) ? fSlow182 : fSlow181))));
		float fSlow184 = float(fEntry54);
		float fSlow185 = fSlow159 * fSlow184 * fSlow162;
		int iSlow186 = fSlow185 > fSlow178;
		float fSlow187 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry55) + fSlow169));
		float fSlow188 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow185), 0.7f);
		float fSlow189 = std::pow(0.014705882f * float(fSlow185 >= 3e+01f) * std::min<float>(fSlow185 + -3e+01f, 68.0f), 1.8f);
		float fSlow190 = fSlow183 + std::max<float>(0.001f, std::fabs(((iSlow186) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow187, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow187, 12.6f))) * (((iSlow186) ? fSlow189 : fSlow188) - ((iSlow186) ? fSlow182 : fSlow181))));
		int iSlow191 = fSlow163 > fSlow185;
		float fSlow192 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry56) + fSlow169));
		float fSlow193 = fSlow190 + std::max<float>(0.001f, std::fabs(((iSlow191) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow192, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow192, 12.6f))) * (((iSlow191) ? fSlow189 : fSlow188) - ((iSlow191) ? fSlow172 : fSlow171))));
		float fSlow194 = fConst0 * fSlow193;
		float fSlow195 = fConst0 * fSlow190;
		int iTemp16 = iTempPerm16;
		float fSlow196 = fSlow159 * fSlow162;
		float fSlow197 = fConst1 * (fSlow184 - fSlow160);
		float fSlow198 = fSlow190 - fSlow193;
		float fSlow199 = fConst0 * fSlow183;
		int iTemp17 = iTempPerm17;
		float fSlow200 = fConst1 * (fSlow184 - fSlow177);
		float fSlow201 = fSlow183 - fSlow190;
		float fSlow202 = fConst1 * (fSlow177 - fSlow164);
		float fTemp18 = fTempPerm18;
		float fSlow203 = fConst1 * fSlow168;
		float fTemp19 = fTempPerm19;
		float fSlow204 = float(fEntry57);
		float fSlow205 = float(fEntry58);
		float fSlow206 = float(fEntry59);
		float fSlow207 = 0.0012626263f * fSlow22 * fSlow206 + 0.01010101f * (1.0f - 0.125f * fSlow206);
		float fSlow208 = fSlow204 * fSlow205 * fSlow207;
		float fSlow209 = float(fEntry60);
		float fSlow210 = fSlow204 * fSlow209 * fSlow207;
		int iSlow211 = fSlow208 <= fSlow210;
		float fSlow212 = float(fEntry65);
		float fSlow213 = 0.25f * float(fEntry63) + ((int(float(fEntry64))) ? fSlow212 : 4.4e+02f * fSlow29 * fSlow212);
		float fSlow214 = 0.057142857f * float(fEntry62) * (17.31234f * std::log(0.0022727272f * fSlow213) + 48.0f);
		float fSlow215 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry61) + fSlow214));
		float fSlow216 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow208), 0.7f);
		float fSlow217 = std::pow(0.014705882f * float(fSlow208 >= 3e+01f) * std::min<float>(fSlow208 + -3e+01f, 68.0f), 1.8f);
		float fSlow218 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow210), 0.7f);
		float fSlow219 = std::pow(0.014705882f * float(fSlow210 >= 3e+01f) * std::min<float>(fSlow210 + -3e+01f, 68.0f), 1.8f);
		float fSlow220 = std::max<float>(0.001f, std::fabs(((iSlow211) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow215, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow215, 12.6f))) * (((iSlow211) ? fSlow217 : fSlow216) - ((iSlow211) ? fSlow219 : fSlow218))));
		float fSlow221 = fConst0 * fSlow220;
		float fSlow222 = float(fEntry66);
		float fSlow223 = fSlow204 * fSlow222 * fSlow207;
		int iSlow224 = fSlow223 > fSlow210;
		float fSlow225 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry67) + fSlow214));
		float fSlow226 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow223), 0.7f);
		float fSlow227 = std::pow(0.014705882f * float(fSlow223 >= 3e+01f) * std::min<float>(fSlow223 + -3e+01f, 68.0f), 1.8f);
		float fSlow228 = std::max<float>(0.001f, std::fabs(((iSlow224) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow225, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow225, 12.6f))) * (((iSlow224) ? fSlow219 : fSlow218) - ((iSlow224) ? fSlow227 : fSlow226))));
		float fSlow229 = float(fEntry68);
		float fSlow230 = fSlow204 * fSlow229 * fSlow207;
		int iSlow231 = fSlow230 > fSlow223;
		float fSlow232 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry69) + fSlow214));
		float fSlow233 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow230), 0.7f);
		float fSlow234 = std::pow(0.014705882f * float(fSlow230 >= 3e+01f) * std::min<float>(fSlow230 + -3e+01f, 68.0f), 1.8f);
		float fSlow235 = fSlow228 + std::max<float>(0.001f, std::fabs(((iSlow231) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow232, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow232, 12.6f))) * (((iSlow231) ? fSlow234 : fSlow233) - ((iSlow231) ? fSlow227 : fSlow226))));
		int iSlow236 = fSlow208 > fSlow230;
		float fSlow237 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry70) + fSlow214));
		float fSlow238 = fSlow235 + std::max<float>(0.001f, std::fabs(((iSlow236) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow237, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow237, 12.6f))) * (((iSlow236) ? fSlow234 : fSlow233) - ((iSlow236) ? fSlow217 : fSlow216))));
		float fSlow239 = fConst0 * fSlow238;
		float fSlow240 = fConst0 * fSlow235;
		int iTemp20 = iTempPerm20;
		float fSlow241 = fSlow204 * fSlow207;
		float fSlow242 = fConst1 * (fSlow229 - fSlow205);
		float fSlow243 = fSlow235 - fSlow238;
		float fSlow244 = fConst0 * fSlow228;
		int iTemp21 = iTempPerm21;
		float fSlow245 = fConst1 * (fSlow222 - fSlow229);
		float fSlow246 = fSlow235 - fSlow228;
		float fSlow247 = fConst1 * (fSlow222 - fSlow209);
		float fTemp22 = fTempPerm22;
		float fSlow248 = fConst1 * fSlow213;
		float fTemp23 = fTempPerm23;
		float fSlow249 = float(fEntry71);
		float fSlow250 = float(fEntry72);
		float fSlow251 = float(fEntry73);
		float fSlow252 = 0.0012626263f * fSlow22 * fSlow251 + 0.01010101f * (1.0f - 0.125f * fSlow251);
		float fSlow253 = fSlow249 * fSlow250 * fSlow252;
		float fSlow254 = float(fEntry74);
		float fSlow255 = fSlow249 * fSlow254 * fSlow252;
		int iSlow256 = fSlow253 <= fSlow255;
		float fSlow257 = float(fEntry79);
		float fSlow258 = 0.25f * float(fEntry77) + ((int(float(fEntry78))) ? fSlow257 : 4.4e+02f * fSlow29 * fSlow257);
		float fSlow259 = 0.057142857f * float(fEntry76) * (17.31234f * std::log(0.0022727272f * fSlow258) + 48.0f);
		float fSlow260 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry75) + fSlow259));
		float fSlow261 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow253), 0.7f);
		float fSlow262 = std::pow(0.014705882f * float(fSlow253 >= 3e+01f) * std::min<float>(fSlow253 + -3e+01f, 68.0f), 1.8f);
		float fSlow263 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow255), 0.7f);
		float fSlow264 = std::pow(0.014705882f * float(fSlow255 >= 3e+01f) * std::min<float>(fSlow255 + -3e+01f, 68.0f), 1.8f);
		float fSlow265 = std::max<float>(0.001f, std::fabs(((iSlow256) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow260, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow260, 12.6f))) * (((iSlow256) ? fSlow262 : fSlow261) - ((iSlow256) ? fSlow264 : fSlow263))));
		float fSlow266 = fConst0 * fSlow265;
		float fSlow267 = float(fEntry80);
		float fSlow268 = fSlow249 * fSlow267 * fSlow252;
		int iSlow269 = fSlow268 > fSlow255;
		float fSlow270 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry81) + fSlow259));
		float fSlow271 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow268), 0.7f);
		float fSlow272 = std::pow(0.014705882f * float(fSlow268 >= 3e+01f) * std::min<float>(fSlow268 + -3e+01f, 68.0f), 1.8f);
		float fSlow273 = std::max<float>(0.001f, std::fabs(((iSlow269) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow270, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow270, 12.6f))) * (((iSlow269) ? fSlow264 : fSlow263) - ((iSlow269) ? fSlow272 : fSlow271))));
		float fSlow274 = float(fEntry82);
		float fSlow275 = fSlow249 * fSlow274 * fSlow252;
		int iSlow276 = fSlow275 > fSlow268;
		float fSlow277 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry83) + fSlow259));
		float fSlow278 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow275), 0.7f);
		float fSlow279 = std::pow(0.014705882f * float(fSlow275 >= 3e+01f) * std::min<float>(fSlow275 + -3e+01f, 68.0f), 1.8f);
		float fSlow280 = fSlow273 + std::max<float>(0.001f, std::fabs(((iSlow276) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow277, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow277, 12.6f))) * (((iSlow276) ? fSlow279 : fSlow278) - ((iSlow276) ? fSlow272 : fSlow271))));
		int iSlow281 = fSlow253 > fSlow275;
		float fSlow282 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry84) + fSlow259));
		float fSlow283 = fSlow280 + std::max<float>(0.001f, std::fabs(((iSlow281) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow282, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow282, 12.6f))) * (((iSlow281) ? fSlow279 : fSlow278) - ((iSlow281) ? fSlow262 : fSlow261))));
		float fSlow284 = fConst0 * fSlow283;
		float fSlow285 = fConst0 * fSlow280;
		int iTemp24 = iTempPerm24;
		float fSlow286 = fSlow249 * fSlow252;
		float fSlow287 = fConst1 * (fSlow274 - fSlow250);
		float fSlow288 = fSlow280 - fSlow283;
		float fSlow289 = fConst0 * fSlow273;
		int iTemp25 = iTempPerm25;
		float fSlow290 = fConst1 * (fSlow267 - fSlow274);
		float fSlow291 = fSlow273 - fSlow280;
		float fSlow292 = fConst1 * (fSlow267 - fSlow254);
		float fTemp26 = fTempPerm26;
		float fSlow293 = fConst1 * fSlow258;
		float fTemp27 = fTempPerm27;
		float fTemp28 = fTempPerm28;
		float fSlow294 = float(fEntry85);
		float fSlow295 = 1825.3162f * fSlow294;
		float fTemp29 = fTempPerm29;
		float fTemp30 = fTempPerm30;
		float fTemp31 = fTempPerm31;
		float fSlow296 = 9126.581f * fSlow294;
		float fTemp32 = fTempPerm32;
		float fTemp33 = fTempPerm33;
		float fTemp34 = fTempPerm34;
		float fTemp35 = fTempPerm35;
		float fTemp36 = fTempPerm36;
		float fTemp37 = fTempPerm37;
		float fTemp38 = fTempPerm38;
		float fTemp39 = fTempPerm39;
		float fSlow297 = 912.6581f * fSlow294;
		float fTemp40 = fTempPerm40;
		float fTemp41 = fTempPerm41;
		float fTemp42 = fTempPerm42;
		float fTemp43 = fTempPerm43;
		float fTemp44 = fTempPerm44;
		float fTemp45 = fTempPerm45;
		float fTemp46 = fTempPerm46;
		float fTemp47 = fTempPerm47;
		float fSlow298 = 3650.6323f * fSlow294;
		float fTemp48 = fTempPerm48;
		float fTemp49 = fTempPerm49;
		float fTemp50 = fTempPerm50;
		float fTemp51 = fTempPerm51;
		float fTemp52 = fTempPerm52;
		float fTemp53 = fTempPerm53;
		float fTemp54 = fTempPerm54;
		float fTemp55 = fTempPerm55;
		float fTemp56 = fTempPerm56;
		float fTemp57 = fTempPerm57;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow1 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16))))))))))))))) {
				fRec0[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow39, fRec0[1] + 1.0f));
				fRec2[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow59, fRec2[1] + 1.0f));
				iTemp0 = fRec2[0] < fSlow60;
				iTemp1 = fRec2[0] < fSlow64;
				fRec1[0] = ((iSlow18) ? fSlow40 * ((iTemp0) ? ((iTemp1) ? ((fRec2[0] < 0.0f) ? fSlow27 : ((iTemp1) ? -(fSlow61 * (fSlow67 * (fRec2[0] / fSlow48) - fSlow26)) : fSlow43)) : ((iTemp0) ? -(fSlow61 * (fSlow65 * ((fRec2[0] - fSlow64) / fSlow66) - fSlow42)) : fSlow50)) : ((fRec2[0] < fSlow59) ? fSlow61 * (fSlow49 + fSlow62 * ((fRec2[0] - fSlow60) / fSlow63)) : fSlow25)) : fRec1[1]);
				fTemp2 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec0[0] < 0.0f) ? fRec1[0] : ((fRec0[0] < fSlow39) ? fRec1[0] + fConst1 * (fRec0[0] * (fSlow27 - fRec1[0]) / fSlow38) : fSlow27))));
				iTemp3 = 1 - iVec0[1];
				fTemp4 = ((iTemp3) ? 0.0f : fRec4[1] + fSlow68);
				fRec4[0] = fTemp4 - std::floor(fTemp4);
				fRec5[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow86, fRec5[1] + 1.0f));
				fRec7[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow104, fRec7[1] + 1.0f));
				iTemp5 = fRec7[0] < fSlow105;
				iTemp6 = fRec7[0] < fSlow109;
				fRec6[0] = ((iSlow18) ? fSlow40 * ((iTemp5) ? ((iTemp6) ? ((fRec7[0] < 0.0f) ? fSlow75 : ((iTemp6) ? -(fSlow106 * (fSlow112 * (fRec7[0] / fSlow93) - fSlow74)) : fSlow88)) : ((iTemp5) ? -(fSlow106 * (fSlow110 * ((fRec7[0] - fSlow109) / fSlow111) - fSlow87)) : fSlow95)) : ((fRec7[0] < fSlow104) ? fSlow106 * (fSlow94 + fSlow107 * ((fRec7[0] - fSlow105) / fSlow108)) : fSlow73)) : fRec6[1]);
				fTemp7 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec5[0] < 0.0f) ? fRec6[0] : ((fRec5[0] < fSlow86) ? fRec6[0] + fConst1 * (fRec5[0] * (fSlow75 - fRec6[0]) / fSlow85) : fSlow75))));
				fTemp8 = ((iTemp3) ? 0.0f : fRec8[1] + fSlow113);
				fRec8[0] = fTemp8 - std::floor(fTemp8);
			}
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow1 | (iSlow6 | (iSlow7 | (iSlow10 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16))))))))))) {
				fTemp9 = 65536.0f * fRec8[0];
			}
			if (iSlow3 | (iSlow4 | (iSlow1 | (iSlow6 | (iSlow7 | (iSlow12 | (iSlow13 | iSlow15))))))) {
				fTemp10 = fTemp7 * ftbl0synth_dx7SIG0[(int(fTemp9) % 65536 + 65536) % 65536];
			}
			if (iSlow3 | (iSlow1 | (iSlow6 | (iSlow7 | (iSlow12 | (iSlow13 | iSlow15)))))) {
				fTemp11 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp10)) % 65536 + 65536) % 65536];
			}
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow1 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16))))))))))))))) {
				fRec9[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow131, fRec9[1] + 1.0f));
				fRec11[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow149, fRec11[1] + 1.0f));
				iTemp12 = fRec11[0] < fSlow150;
				iTemp13 = fRec11[0] < fSlow154;
				fRec10[0] = ((iSlow18) ? fSlow40 * ((iTemp12) ? ((iTemp13) ? ((fRec11[0] < 0.0f) ? fSlow120 : ((iTemp13) ? -(fSlow151 * (fSlow157 * (fRec11[0] / fSlow138) - fSlow119)) : fSlow133)) : ((iTemp12) ? -(fSlow151 * (fSlow155 * ((fRec11[0] - fSlow154) / fSlow156) - fSlow132)) : fSlow140)) : ((fRec11[0] < fSlow149) ? fSlow151 * (fSlow139 + fSlow152 * ((fRec11[0] - fSlow150) / fSlow153)) : fSlow118)) : fRec10[1]);
				fTemp14 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec9[0] < 0.0f) ? fRec10[0] : ((fRec9[0] < fSlow131) ? fRec10[0] + fConst1 * (fRec9[0] * (fSlow120 - fRec10[0]) / fSlow130) : fSlow120))));
				fTemp15 = ((iTemp3) ? 0.0f : fRec12[1] + fSlow158);
				fRec12[0] = fTemp15 - std::floor(fTemp15);
				fRec13[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow176, fRec13[1] + 1.0f));
				fRec15[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow194, fRec15[1] + 1.0f));
				iTemp16 = fRec15[0] < fSlow195;
				iTemp17 = fRec15[0] < fSlow199;
				fRec14[0] = ((iSlow18) ? fSlow40 * ((iTemp16) ? ((iTemp17) ? ((fRec15[0] < 0.0f) ? fSlow165 : ((iTemp17) ? fSlow196 * (fSlow164 + fSlow202 * (fRec15[0] / fSlow183)) : fSlow178)) : ((iTemp16) ? -(fSlow196 * (fSlow200 * ((fRec15[0] - fSlow199) / fSlow201) - fSlow177)) : fSlow185)) : ((fRec15[0] < fSlow194) ? fSlow196 * (fSlow184 + fSlow197 * ((fRec15[0] - fSlow195) / fSlow198)) : fSlow163)) : fRec14[1]);
				fTemp18 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec13[0] < 0.0f) ? fRec14[0] : ((fRec13[0] < fSlow176) ? fRec14[0] + fConst1 * (fRec13[0] * (fSlow165 - fRec14[0]) / fSlow175) : fSlow165))));
				fTemp19 = ((iTemp3) ? 0.0f : fRec16[1] + fSlow203);
				fRec16[0] = fTemp19 - std::floor(fTemp19);
				fRec17[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow221, fRec17[1] + 1.0f));
				fRec19[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow239, fRec19[1] + 1.0f));
				iTemp20 = fRec19[0] < fSlow240;
				iTemp21 = fRec19[0] < fSlow244;
				fRec18[0] = ((iSlow18) ? fSlow40 * ((iTemp20) ? ((iTemp21) ? ((fRec19[0] < 0.0f) ? fSlow210 : ((iTemp21) ? fSlow241 * (fSlow209 + fSlow247 * (fRec19[0] / fSlow228)) : fSlow223)) : ((iTemp20) ? -(fSlow241 * (fSlow245 * ((fRec19[0] - fSlow244) / fSlow246) - fSlow222)) : fSlow230)) : ((fRec19[0] < fSlow239) ? fSlow241 * (fSlow229 + fSlow242 * ((fRec19[0] - fSlow240) / fSlow243)) : fSlow208)) : fRec18[1]);
				fTemp22 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec17[0] < 0.0f) ? fRec18[0] : ((fRec17[0] < fSlow221) ? fRec18[0] + fConst1 * (fRec17[0] * (fSlow210 - fRec18[0]) / fSlow220) : fSlow210))));
				fTemp23 = ((iTemp3) ? 0.0f : fRec20[1] + fSlow248);
				fRec20[0] = fTemp23 - std::floor(fTemp23);
				fRec22[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow266, fRec22[1] + 1.0f));
				fRec24[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow284, fRec24[1] + 1.0f));
				iTemp24 = fRec24[0] < fSlow285;
				iTemp25 = fRec24[0] < fSlow289;
				fRec23[0] = ((iSlow18) ? fSlow40 * ((iTemp24) ? ((iTemp25) ? ((fRec24[0] < 0.0f) ? fSlow255 : ((iTemp25) ? fSlow286 * (fSlow254 + fSlow292 * (fRec24[0] / fSlow273)) : fSlow268)) : ((iTemp24) ? fSlow286 * (fSlow267 + fSlow290 * ((fRec24[0] - fSlow289) / fSlow291)) : fSlow275)) : ((fRec24[0] < fSlow284) ? fSlow286 * (fSlow274 + fSlow287 * ((fRec24[0] - fSlow285) / fSlow288)) : fSlow253)) : fRec23[1]);
				fTemp26 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec22[0] < 0.0f) ? fRec23[0] : ((fRec22[0] < fSlow266) ? fRec23[0] + fConst1 * (fRec22[0] * (fSlow255 - fRec23[0]) / fSlow265) : fSlow255))));
				fTemp27 = ((iTemp3) ? 0.0f : fRec25[1] + fSlow293);
				fRec25[0] = fTemp27 - std::floor(fTemp27);
				fTemp28 = 65536.0f * fRec25[0];
			}
			if (iSlow1 | iSlow8) {
				fRec21[0] = fTemp26 * ftbl0synth_dx7SIG0[(int(fTemp28 + fSlow295 * fRec21[1]) % 65536 + 65536) % 65536];
			}
			if (iSlow1) {
				fTemp29 = 2.08795f * (fTemp11 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp18 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp22 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec20[0] + fRec21[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow5 | (iSlow8 | (iSlow9 | iSlow11))) {
				fTemp30 = 65536.0f * fRec12[0];
			}
			if (iSlow5 | (iSlow8 | iSlow11)) {
				fTemp31 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp7 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec8[0] + fTemp14 * ftbl0synth_dx7SIG0[(int(fTemp30) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
			}
			if (iSlow3 | (iSlow4 | (iSlow5 | (iSlow7 | iSlow13)))) {
				fRec26[0] = fTemp26 * ftbl0synth_dx7SIG0[(int(fTemp28 + fSlow296 * fRec26[1]) % 65536 + 65536) % 65536];
			}
			if (iSlow4 | (iSlow5 | iSlow13)) {
				fTemp32 = fTemp22 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec20[0] + fRec26[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow5) {
				fTemp33 = 2.08795f * (fTemp31 + fTemp18 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp32)) % 65536 + 65536) % 65536]);
			}
			if (iSlow2 | iSlow10) {
				fRec27[0] = fTemp7 * ftbl0synth_dx7SIG0[(int(fTemp9 + fSlow295 * fRec27[1]) % 65536 + 65536) % 65536];
				fTemp34 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec27[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow2 | (iSlow9 | (iSlow10 | (iSlow12 | (iSlow14 | iSlow16))))) {
				fTemp35 = fTemp26 * ftbl0synth_dx7SIG0[(int(fTemp28) % 65536 + 65536) % 65536];
			}
			if (iSlow10 | (iSlow12 | iSlow16)) {
				fTemp36 = fTemp22 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec20[0] + fTemp35)) % 65536 + 65536) % 65536];
			}
			if (iSlow10) {
				fTemp37 = 2.08795f * (fTemp34 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp18 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp36)) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow11) {
				fRec28[0] = 2.08795f * fTemp18 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp22 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec20[0] + fTemp26 * ftbl0synth_dx7SIG0[(int(fTemp28 + fSlow296 * fRec28[1]) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
				fTemp38 = fRec28[0] + 2.08795f * fTemp31;
			}
			if (iSlow2 | (iSlow4 | (iSlow6 | (iSlow7 | (iSlow12 | (iSlow13 | (iSlow15 | iSlow16))))))) {
				fTemp39 = 65536.0f * fRec16[0];
			}
			if (iSlow12) {
				fRec29[0] = fTemp18 * ftbl0synth_dx7SIG0[(int(fTemp39 + fSlow297 * fRec29[1]) % 65536 + 65536) % 65536];
				fTemp40 = 2.08795f * (fTemp11 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fRec29[0] + fTemp36)) % 65536 + 65536) % 65536]);
			}
			if (iSlow2 | (iSlow4 | (iSlow6 | (iSlow7 | (iSlow13 | (iSlow15 | iSlow16)))))) {
				fTemp41 = fTemp18 * ftbl0synth_dx7SIG0[(int(fTemp39) % 65536 + 65536) % 65536];
			}
			if (iSlow13) {
				fTemp42 = 2.08795f * (fTemp11 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp32 + fTemp41)) % 65536 + 65536) % 65536]);
			}
			if (iSlow6) {
				fRec30[0] = 2.08795f * fTemp22 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec20[0] + fTemp26 * ftbl0synth_dx7SIG0[(int(fTemp28 + fSlow296 * fRec30[1]) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
			}
			if (iSlow4 | (iSlow6 | iSlow15)) {
				fTemp43 = fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp41)) % 65536 + 65536) % 65536];
			}
			if (iSlow6) {
				fTemp44 = fRec30[0] + 2.08795f * (fTemp11 + fTemp43);
			}
			if (iSlow2 | (iSlow3 | (iSlow7 | (iSlow8 | (iSlow9 | iSlow14))))) {
				fTemp45 = fTemp22 * ftbl0synth_dx7SIG0[(int(65536.0f * fRec20[0]) % 65536 + 65536) % 65536];
			}
			if (iSlow9 | iSlow14) {
				fTemp46 = fTemp18 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp35 + fTemp45)) % 65536 + 65536) % 65536];
			}
			if (iSlow9) {
				fRec31[0] = fTemp14 * ftbl0synth_dx7SIG0[(int(fTemp30 + fSlow295 * fRec31[1]) % 65536 + 65536) % 65536];
				fTemp47 = 2.08795f * (fTemp46 + fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp7 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec8[0] + fRec31[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow14 | iSlow16) {
				fRec32[0] = fTemp7 * ftbl0synth_dx7SIG0[(int(fTemp9 + fSlow298 * fRec32[1]) % 65536 + 65536) % 65536];
				fTemp48 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec32[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow16) {
				fTemp49 = 2.08795f * (fTemp48 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp36 + fTemp41)) % 65536 + 65536) % 65536]);
			}
			if (iSlow2 | iSlow7) {
				fTemp50 = fTemp41 + fTemp45;
			}
			if (iSlow7) {
				fTemp51 = 2.08795f * (fTemp11 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fRec26[0] + fTemp50)) % 65536 + 65536) % 65536]);
			}
			if (iSlow2) {
				fTemp52 = 2.08795f * (fTemp34 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp35 + fTemp50)) % 65536 + 65536) % 65536]);
			}
			if (iSlow8) {
				fTemp53 = 2.08795f * (fTemp31 + fTemp18 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec21[0] + fTemp45)) % 65536 + 65536) % 65536]);
			}
			if (iSlow3) {
				fTemp54 = 2.08795f * (fTemp11 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp18 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec26[0] + fTemp45)) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow14) {
				fTemp55 = 2.08795f * (fTemp48 + fTemp14 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp46)) % 65536 + 65536) % 65536]);
			}
			if (iSlow4) {
				fTemp56 = 2.08795f * fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp10 + fTemp32 + fTemp43)) % 65536 + 65536) % 65536];
			}
			if (iSlow15) {
				fRec33[0] = fTemp26 * ftbl0synth_dx7SIG0[(int(fTemp28 + fSlow297 * fRec33[1]) % 65536 + 65536) % 65536];
				fTemp57 = 2.08795f * (fTemp11 + fTemp43 + fTemp22 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec20[0] + fRec33[0])) % 65536 + 65536) % 65536]);
			}
			output0[i0] = FAUSTFLOAT(fTemp29 + fTemp33 + fTemp37 + fTemp38 + fTemp40 + fTemp42 + fTemp44 + fTemp47 + fTemp49 + fTemp51 + fTemp52 + fTemp53 + fTemp54 + fTemp55 + fTemp56 + fTemp57);
			iVec0[1] = iVec0[0];
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow1 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16))))))))))))))) {
				fRec0[1] = fRec0[0];
				fRec2[1] = fRec2[0];
				fRec1[1] = fRec1[0];
				fRec4[1] = fRec4[0];
				fRec5[1] = fRec5[0];
				fRec7[1] = fRec7[0];
				fRec6[1] = fRec6[0];
				fRec8[1] = fRec8[0];
				fRec9[1] = fRec9[0];
				fRec11[1] = fRec11[0];
				fRec10[1] = fRec10[0];
				fRec12[1] = fRec12[0];
				fRec13[1] = fRec13[0];
				fRec15[1] = fRec15[0];
				fRec14[1] = fRec14[0];
				fRec16[1] = fRec16[0];
				fRec17[1] = fRec17[0];
				fRec19[1] = fRec19[0];
				fRec18[1] = fRec18[0];
				fRec20[1] = fRec20[0];
				fRec22[1] = fRec22[0];
				fRec24[1] = fRec24[0];
				fRec23[1] = fRec23[0];
				fRec25[1] = fRec25[0];
			}
			if (iSlow1 | iSlow8) {
				fRec21[1] = fRec21[0];
			}
			if (iSlow3 | (iSlow4 | (iSlow5 | (iSlow7 | iSlow13)))) {
				fRec26[1] = fRec26[0];
			}
			if (iSlow2 | iSlow10) {
				fRec27[1] = fRec27[0];
			}
			if (iSlow11) {
				fRec28[1] = fRec28[0];
			}
			if (iSlow12) {
				fRec29[1] = fRec29[0];
			}
			if (iSlow6) {
				fRec30[1] = fRec30[0];
			}
			if (iSlow9) {
				fRec31[1] = fRec31[0];
			}
			if (iSlow14 | iSlow16) {
				fRec32[1] = fRec32[0];
			}
			if (iSlow15) {
				fRec33[1] = fRec33[0];
			}
		}
		iTempPerm0 = iTemp0;
		iTempPerm1 = iTemp1;
		fTempPerm2 = fTemp2;
		iTempPerm3 = iTemp3;
		fTempPerm4 = fTemp4;
		iTempPerm5 = iTemp5;
		iTempPerm6 = iTemp6;
		fTempPerm7 = fTemp7;
		fTempPerm8 = fTemp8;
		fTempPerm9 = fTemp9;
		fTempPerm10 = fTemp10;
		fTempPerm11 = fTemp11;
		iTempPerm12 = iTemp12;
		iTempPerm13 = iTemp13;
		fTempPerm14 = fTemp14;
		fTempPerm15 = fTemp15;
		iTempPerm16 = iTemp16;
		iTempPerm17 = iTemp17;
		fTempPerm18 = fTemp18;
		fTempPerm19 = fTemp19;
		iTempPerm20 = iTemp20;
		iTempPerm21 = iTemp21;
		fTempPerm22 = fTemp22;
		fTempPerm23 = fTemp23;
		iTempPerm24 = iTemp24;
		iTempPerm25 = iTemp25;
		fTempPerm26 = fTemp26;
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
		fTempPerm41 = fTemp41;
		fTempPerm42 = fTemp42;
		fTempPerm43 = fTemp43;
		fTempPerm44 = fTemp44;
		fTempPerm45 = fTemp45;
		fTempPerm46 = fTemp46;
		fTempPerm47 = fTemp47;
		fTempPerm48 = fTemp48;
		fTempPerm49 = fTemp49;
		fTempPerm50 = fTemp50;
		fTempPerm51 = fTemp51;
		fTempPerm52 = fTemp52;
		fTempPerm53 = fTemp53;
		fTempPerm54 = fTemp54;
		fTempPerm55 = fTemp55;
		fTempPerm56 = fTemp56;
		fTempPerm57 = fTemp57;
	}

};
#endif

class faust_synth_dx7_tilde : public FaustExternal<synth_dx7> {
public:
    faust_synth_dx7_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_dx7))
    {
    }
};

#endif
