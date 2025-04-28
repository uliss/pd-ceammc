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
        virtual synth_dx7_dsp* clone() = 0;
    
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
	FAUSTFLOAT fEntry3;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fEntry4;
	FAUSTFLOAT fEntry5;
	FAUSTFLOAT fEntry6;
	FAUSTFLOAT fEntry7;
	FAUSTFLOAT fEntry8;
	FAUSTFLOAT fEntry9;
	FAUSTFLOAT fHslider1;
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
	int iTempPerm9;
	int iTempPerm10;
	float fRec10[2];
	float fTempPerm11;
	float fTempPerm12;
	float fRec12[2];
	float fTempPerm13;
	float fTempPerm14;
	float fTempPerm15;
	float fTempPerm16;
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
	int iTempPerm17;
	int iTempPerm18;
	float fRec14[2];
	float fTempPerm19;
	float fTempPerm20;
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
	float fRec18[2];
	FAUSTFLOAT fEntry66;
	FAUSTFLOAT fEntry67;
	FAUSTFLOAT fEntry68;
	FAUSTFLOAT fEntry69;
	FAUSTFLOAT fEntry70;
	float fRec20[2];
	int iTempPerm21;
	int iTempPerm22;
	float fRec19[2];
	float fTempPerm23;
	float fTempPerm24;
	float fRec21[2];
	float fTempPerm25;
	FAUSTFLOAT fEntry71;
	float fRec17[2];
	FAUSTFLOAT fEntry72;
	FAUSTFLOAT fEntry73;
	FAUSTFLOAT fEntry74;
	FAUSTFLOAT fEntry75;
	FAUSTFLOAT fEntry76;
	FAUSTFLOAT fEntry77;
	FAUSTFLOAT fEntry78;
	FAUSTFLOAT fEntry79;
	FAUSTFLOAT fEntry80;
	float fRec22[2];
	FAUSTFLOAT fEntry81;
	FAUSTFLOAT fEntry82;
	FAUSTFLOAT fEntry83;
	FAUSTFLOAT fEntry84;
	FAUSTFLOAT fEntry85;
	float fRec24[2];
	int iTempPerm26;
	int iTempPerm27;
	float fRec23[2];
	float fTempPerm28;
	float fTempPerm29;
	float fRec25[2];
	float fTempPerm30;
	float fRec26[2];
	float fTempPerm31;
	float fTempPerm32;
	float fTempPerm33;
	float fTempPerm34;
	float fTempPerm35;
	float fTempPerm36;
	float fRec27[2];
	float fTempPerm37;
	float fTempPerm38;
	float fTempPerm39;
	float fTempPerm40;
	float fRec28[2];
	float fTempPerm41;
	float fTempPerm42;
	float fTempPerm43;
	float fTempPerm44;
	float fTempPerm45;
	float fTempPerm46;
	float fTempPerm47;
	float fRec29[2];
	float fTempPerm48;
	float fTempPerm49;
	float fTempPerm50;
	float fTempPerm51;
	float fTempPerm52;
	float fRec30[2];
	float fTempPerm53;
	float fTempPerm54;
	float fTempPerm55;
	float fTempPerm56;
	float fTempPerm57;
	float fRec31[2];
	float fTempPerm58;
	float fTempPerm59;
	float fTempPerm60;
	float fTempPerm61;
	float fTempPerm62;
	float fTempPerm63;
	float fTempPerm64;
	float fTempPerm65;
	float fTempPerm66;
	float fTempPerm67;
	float fTempPerm68;
	float fTempPerm69;
	float fRec32[2];
	float fTempPerm70;
	float fTempPerm71;
	float fRec33[2];
	float fTempPerm72;
	float fRec34[2];
	float fTempPerm73;
	float fTempPerm74;
	float fRec35[2];
	float fTempPerm75;
	float fTempPerm76;
	float fRec36[2];
	float fTempPerm77;
	float fRec37[2];
	float fTempPerm78;
	float fRec38[2];
	float fTempPerm79;
	float fTempPerm80;
	float fTempPerm81;
	float fTempPerm82;
	float fTempPerm83;
	float fTempPerm84;
	float fTempPerm85;
	float fTempPerm86;
	float fTempPerm87;
	
 public:
	synth_dx7() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.16.0");
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
		fEntry3 = FAUSTFLOAT(1.0f);
		fHslider0 = FAUSTFLOAT(0.8f);
		fEntry4 = FAUSTFLOAT(0.0f);
		fEntry5 = FAUSTFLOAT(9e+01f);
		fEntry6 = FAUSTFLOAT(0.0f);
		fEntry7 = FAUSTFLOAT(1.0f);
		fEntry8 = FAUSTFLOAT(0.0f);
		fEntry9 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(48.0f);
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
		fEntry71 = FAUSTFLOAT(0.0f);
		fEntry72 = FAUSTFLOAT(95.0f);
		fEntry73 = FAUSTFLOAT(9e+01f);
		fEntry74 = FAUSTFLOAT(1.0f);
		fEntry75 = FAUSTFLOAT(0.0f);
		fEntry76 = FAUSTFLOAT(9e+01f);
		fEntry77 = FAUSTFLOAT(0.0f);
		fEntry78 = FAUSTFLOAT(1.0f);
		fEntry79 = FAUSTFLOAT(0.0f);
		fEntry80 = FAUSTFLOAT(1.0f);
		fEntry81 = FAUSTFLOAT(0.0f);
		fEntry82 = FAUSTFLOAT(9e+01f);
		fEntry83 = FAUSTFLOAT(9e+01f);
		fEntry84 = FAUSTFLOAT(9e+01f);
		fEntry85 = FAUSTFLOAT(9e+01f);
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
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec11[l12] = 0.0f;
		}
		iTempPerm9 = 0;
		iTempPerm10 = 0;
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec10[l13] = 0.0f;
		}
		fTempPerm11 = 0.0f;
		fTempPerm12 = 0.0f;
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
		}
		fTempPerm13 = 0.0f;
		fTempPerm14 = 0.0f;
		fTempPerm15 = 0.0f;
		fTempPerm16 = 0.0f;
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec15[l16] = 0.0f;
		}
		iTempPerm17 = 0;
		iTempPerm18 = 0;
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec14[l17] = 0.0f;
		}
		fTempPerm19 = 0.0f;
		fTempPerm20 = 0.0f;
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec16[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec20[l20] = 0.0f;
		}
		iTempPerm21 = 0;
		iTempPerm22 = 0;
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec19[l21] = 0.0f;
		}
		fTempPerm23 = 0.0f;
		fTempPerm24 = 0.0f;
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec21[l22] = 0.0f;
		}
		fTempPerm25 = 0.0f;
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec17[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec22[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec24[l25] = 0.0f;
		}
		iTempPerm26 = 0;
		iTempPerm27 = 0;
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec23[l26] = 0.0f;
		}
		fTempPerm28 = 0.0f;
		fTempPerm29 = 0.0f;
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec25[l27] = 0.0f;
		}
		fTempPerm30 = 0.0f;
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec26[l28] = 0.0f;
		}
		fTempPerm31 = 0.0f;
		fTempPerm32 = 0.0f;
		fTempPerm33 = 0.0f;
		fTempPerm34 = 0.0f;
		fTempPerm35 = 0.0f;
		fTempPerm36 = 0.0f;
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec27[l29] = 0.0f;
		}
		fTempPerm37 = 0.0f;
		fTempPerm38 = 0.0f;
		fTempPerm39 = 0.0f;
		fTempPerm40 = 0.0f;
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec28[l30] = 0.0f;
		}
		fTempPerm41 = 0.0f;
		fTempPerm42 = 0.0f;
		fTempPerm43 = 0.0f;
		fTempPerm44 = 0.0f;
		fTempPerm45 = 0.0f;
		fTempPerm46 = 0.0f;
		fTempPerm47 = 0.0f;
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec29[l31] = 0.0f;
		}
		fTempPerm48 = 0.0f;
		fTempPerm49 = 0.0f;
		fTempPerm50 = 0.0f;
		fTempPerm51 = 0.0f;
		fTempPerm52 = 0.0f;
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec30[l32] = 0.0f;
		}
		fTempPerm53 = 0.0f;
		fTempPerm54 = 0.0f;
		fTempPerm55 = 0.0f;
		fTempPerm56 = 0.0f;
		fTempPerm57 = 0.0f;
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec31[l33] = 0.0f;
		}
		fTempPerm58 = 0.0f;
		fTempPerm59 = 0.0f;
		fTempPerm60 = 0.0f;
		fTempPerm61 = 0.0f;
		fTempPerm62 = 0.0f;
		fTempPerm63 = 0.0f;
		fTempPerm64 = 0.0f;
		fTempPerm65 = 0.0f;
		fTempPerm66 = 0.0f;
		fTempPerm67 = 0.0f;
		fTempPerm68 = 0.0f;
		fTempPerm69 = 0.0f;
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec32[l34] = 0.0f;
		}
		fTempPerm70 = 0.0f;
		fTempPerm71 = 0.0f;
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec33[l35] = 0.0f;
		}
		fTempPerm72 = 0.0f;
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec34[l36] = 0.0f;
		}
		fTempPerm73 = 0.0f;
		fTempPerm74 = 0.0f;
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec35[l37] = 0.0f;
		}
		fTempPerm75 = 0.0f;
		fTempPerm76 = 0.0f;
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec36[l38] = 0.0f;
		}
		fTempPerm77 = 0.0f;
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec37[l39] = 0.0f;
		}
		fTempPerm78 = 0.0f;
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec38[l40] = 0.0f;
		}
		fTempPerm79 = 0.0f;
		fTempPerm80 = 0.0f;
		fTempPerm81 = 0.0f;
		fTempPerm82 = 0.0f;
		fTempPerm83 = 0.0f;
		fTempPerm84 = 0.0f;
		fTempPerm85 = 0.0f;
		fTempPerm86 = 0.0f;
		fTempPerm87 = 0.0f;
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
		ui_interface->addNumEntry("algorithm", &fEntry0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(31.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("feedback", &fEntry71, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("gain", &fHslider0, FAUSTFLOAT(0.8f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addNumEntry("op0:L1", &fEntry10, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:L2", &fEntry12, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:L3", &fEntry2, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:L4", &fEntry4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:R1", &fEntry11, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:R2", &fEntry13, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:R3", &fEntry14, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:R4", &fEntry5, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:detune", &fEntry7, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry9, "unit", "Hz");
		ui_interface->addNumEntry("op0:freq", &fEntry9, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(32.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op0:level", &fEntry1, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:opMode", &fEntry8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:rate", &fEntry6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op0:vel", &fEntry3, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:L1", &fEntry24, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:L2", &fEntry26, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:L3", &fEntry16, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:L4", &fEntry18, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:R1", &fEntry25, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:R2", &fEntry27, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:R3", &fEntry28, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:R4", &fEntry19, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:detune", &fEntry21, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry23, "unit", "Hz");
		ui_interface->addNumEntry("op1:freq", &fEntry23, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(32.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op1:level", &fEntry15, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:opMode", &fEntry22, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:rate", &fEntry20, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op1:vel", &fEntry17, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:L1", &fEntry38, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:L2", &fEntry40, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:L3", &fEntry30, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:L4", &fEntry32, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:R1", &fEntry39, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:R2", &fEntry41, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:R3", &fEntry42, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:R4", &fEntry33, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:detune", &fEntry35, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry37, "unit", "Hz");
		ui_interface->addNumEntry("op2:freq", &fEntry37, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(32.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op2:level", &fEntry29, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:opMode", &fEntry36, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:rate", &fEntry34, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:vel", &fEntry31, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:L1", &fEntry52, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:L2", &fEntry54, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:L3", &fEntry44, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:L4", &fEntry46, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:R1", &fEntry53, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:R2", &fEntry55, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:R3", &fEntry56, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:R4", &fEntry47, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:detune", &fEntry49, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry51, "unit", "Hz");
		ui_interface->addNumEntry("op3:freq", &fEntry51, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(32.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op3:level", &fEntry43, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:opMode", &fEntry50, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:rate", &fEntry48, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:vel", &fEntry45, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:L1", &fEntry81, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:L2", &fEntry83, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:L3", &fEntry73, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:L4", &fEntry75, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:R1", &fEntry82, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:R2", &fEntry84, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:R3", &fEntry85, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:R4", &fEntry76, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:detune", &fEntry78, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry80, "unit", "Hz");
		ui_interface->addNumEntry("op4:freq", &fEntry80, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(32.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op4:level", &fEntry72, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:opMode", &fEntry79, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:rate", &fEntry77, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:vel", &fEntry74, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:L1", &fEntry66, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:L2", &fEntry68, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:L3", &fEntry58, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:L4", &fEntry60, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:R1", &fEntry67, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:R2", &fEntry69, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:R3", &fEntry70, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:R4", &fEntry61, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:detune", &fEntry63, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry65, "unit", "Hz");
		ui_interface->addNumEntry("op5:freq", &fEntry65, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(32.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op5:level", &fEntry57, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:opMode", &fEntry64, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:rate", &fEntry62, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:vel", &fEntry59, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("pitch", &fHslider1, FAUSTFLOAT(48.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(84.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = int(float(fEntry0));
		int iSlow1 = float(iSlow0 == 18) != 0.0f;
		int iSlow2 = float(iSlow0 == 3) != 0.0f;
		int iSlow3 = float(iSlow0 == 31) != 0.0f;
		int iSlow4 = float(iSlow0 == 10) != 0.0f;
		int iSlow5 = float(iSlow0 == 11) != 0.0f;
		int iSlow6 = float(iSlow0 == 12) != 0.0f;
		int iSlow7 = float(iSlow0 == 5) != 0.0f;
		int iSlow8 = float(iSlow0 == 9) != 0.0f;
		int iSlow9 = float(iSlow0 == 6) != 0.0f;
		int iSlow10 = float(iSlow0 == 1) != 0.0f;
		int iSlow11 = float(iSlow0 == 0) != 0.0f;
		int iSlow12 = float(iSlow0 == 8) != 0.0f;
		int iSlow13 = float(iSlow0 == 7) != 0.0f;
		int iSlow14 = float(iSlow0 == 14) != 0.0f;
		int iSlow15 = float(iSlow0 == 13) != 0.0f;
		int iSlow16 = float(iSlow0 == 19) != 0.0f;
		int iSlow17 = float(iSlow0 == 20) != 0.0f;
		int iSlow18 = float(iSlow0 == 2) != 0.0f;
		int iSlow19 = float(iSlow0 == 23) != 0.0f;
		int iSlow20 = float(iSlow0 == 24) != 0.0f;
		int iSlow21 = float(iSlow0 == 25) != 0.0f;
		int iSlow22 = float(iSlow0 == 26) != 0.0f;
		int iSlow23 = float(iSlow0 == 29) != 0.0f;
		int iSlow24 = float(iSlow0 == 28) != 0.0f;
		int iSlow25 = float(iSlow0 == 27) != 0.0f;
		int iSlow26 = float(iSlow0 == 4) != 0.0f;
		int iSlow27 = float(iSlow0 == 21) != 0.0f;
		int iSlow28 = float(iSlow0 == 22) != 0.0f;
		int iSlow29 = float(iSlow0 == 17) != 0.0f;
		int iSlow30 = float(iSlow0 == 16) != 0.0f;
		int iSlow31 = float(iSlow0 == 15) != 0.0f;
		int iSlow32 = float(iSlow0 == 30) != 0.0f;
		float fSlow33 = float(fButton0);
		int iSlow34 = fSlow33 > 0.0f;
		int iSlow35 = iSlow34 > 0;
		float fSlow36 = float(fEntry1);
		float fSlow37 = float(fEntry2);
		float fSlow38 = float(fEntry3);
		float fSlow39 = float(fHslider0);
		float fSlow40 = 0.0012626263f * fSlow38 * fSlow39 + 0.01010101f * (1.0f - 0.125f * fSlow38);
		float fSlow41 = fSlow36 * fSlow37 * fSlow40;
		float fSlow42 = float(fEntry4);
		float fSlow43 = fSlow36 * fSlow42 * fSlow40;
		int iSlow44 = fSlow41 <= fSlow43;
		float fSlow45 = float(fEntry9);
		float fSlow46 = std::pow(2.0f, 0.083333336f * (float(fHslider1) + -69.0f));
		float fSlow47 = 0.25f * float(fEntry7) + ((int(float(fEntry8))) ? fSlow45 : 4.4e+02f * fSlow45 * fSlow46);
		float fSlow48 = 0.057142857f * float(fEntry6) * (17.31234f * std::log(0.0022727272f * fSlow47) + 48.0f);
		float fSlow49 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry5) + fSlow48));
		float fSlow50 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow41), 0.7f);
		float fSlow51 = std::pow(0.014705882f * float(fSlow41 >= 3e+01f) * std::min<float>(fSlow41 + -3e+01f, 68.0f), 1.8f);
		float fSlow52 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow43), 0.7f);
		float fSlow53 = std::pow(0.014705882f * float(fSlow43 >= 3e+01f) * std::min<float>(fSlow43 + -3e+01f, 68.0f), 1.8f);
		float fSlow54 = std::max<float>(0.001f, std::fabs(((iSlow44) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow49, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow49, 12.6f))) * (((iSlow44) ? fSlow51 : fSlow50) - ((iSlow44) ? fSlow53 : fSlow52))));
		float fSlow55 = fConst0 * fSlow54;
		float fSlow56 = float(iSlow34);
		int iSlow57 = (fSlow33 == 0.0f) > 0;
		float fSlow58 = float(fEntry10);
		float fSlow59 = fSlow36 * fSlow58 * fSlow40;
		int iSlow60 = fSlow59 > fSlow43;
		float fSlow61 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry11) + fSlow48));
		float fSlow62 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow59), 0.7f);
		float fSlow63 = std::pow(0.014705882f * float(fSlow59 >= 3e+01f) * std::min<float>(fSlow59 + -3e+01f, 68.0f), 1.8f);
		float fSlow64 = std::max<float>(0.001f, std::fabs(((iSlow60) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow61, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow61, 12.6f))) * (((iSlow60) ? fSlow53 : fSlow52) - ((iSlow60) ? fSlow63 : fSlow62))));
		float fSlow65 = float(fEntry12);
		float fSlow66 = fSlow36 * fSlow65 * fSlow40;
		int iSlow67 = fSlow66 > fSlow59;
		float fSlow68 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry13) + fSlow48));
		float fSlow69 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow66), 0.7f);
		float fSlow70 = std::pow(0.014705882f * float(fSlow66 >= 3e+01f) * std::min<float>(fSlow66 + -3e+01f, 68.0f), 1.8f);
		float fSlow71 = fSlow64 + std::max<float>(0.001f, std::fabs(((iSlow67) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow68, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow68, 12.6f))) * (((iSlow67) ? fSlow70 : fSlow69) - ((iSlow67) ? fSlow63 : fSlow62))));
		int iSlow72 = fSlow41 > fSlow66;
		float fSlow73 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry14) + fSlow48));
		float fSlow74 = fSlow71 + std::max<float>(0.001f, std::fabs(((iSlow72) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow73, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow73, 12.6f))) * (((iSlow72) ? fSlow70 : fSlow69) - ((iSlow72) ? fSlow51 : fSlow50))));
		float fSlow75 = fConst0 * fSlow74;
		float fSlow76 = fConst0 * fSlow71;
		int iTemp0 = iTempPerm0;
		float fSlow77 = fSlow36 * fSlow40;
		float fSlow78 = fConst1 * (fSlow65 - fSlow37);
		float fSlow79 = fSlow71 - fSlow74;
		float fSlow80 = fConst0 * fSlow64;
		int iTemp1 = iTempPerm1;
		float fSlow81 = fConst1 * (fSlow65 - fSlow58);
		float fSlow82 = fSlow71 - fSlow64;
		float fSlow83 = fConst1 * (fSlow42 - fSlow58);
		float fTemp2 = fTempPerm2;
		int iTemp3 = iTempPerm3;
		float fSlow84 = fConst1 * fSlow47;
		float fTemp4 = fTempPerm4;
		float fSlow85 = float(fEntry15);
		float fSlow86 = float(fEntry16);
		float fSlow87 = float(fEntry17);
		float fSlow88 = 0.0012626263f * fSlow39 * fSlow87 + 0.01010101f * (1.0f - 0.125f * fSlow87);
		float fSlow89 = fSlow85 * fSlow86 * fSlow88;
		float fSlow90 = float(fEntry18);
		float fSlow91 = fSlow85 * fSlow90 * fSlow88;
		int iSlow92 = fSlow89 <= fSlow91;
		float fSlow93 = float(fEntry23);
		float fSlow94 = 0.25f * float(fEntry21) + ((int(float(fEntry22))) ? fSlow93 : 4.4e+02f * fSlow46 * fSlow93);
		float fSlow95 = 0.057142857f * float(fEntry20) * (17.31234f * std::log(0.0022727272f * fSlow94) + 48.0f);
		float fSlow96 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry19) + fSlow95));
		float fSlow97 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow89), 0.7f);
		float fSlow98 = std::pow(0.014705882f * float(fSlow89 >= 3e+01f) * std::min<float>(fSlow89 + -3e+01f, 68.0f), 1.8f);
		float fSlow99 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow91), 0.7f);
		float fSlow100 = std::pow(0.014705882f * float(fSlow91 >= 3e+01f) * std::min<float>(fSlow91 + -3e+01f, 68.0f), 1.8f);
		float fSlow101 = std::max<float>(0.001f, std::fabs(((iSlow92) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow96, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow96, 12.6f))) * (((iSlow92) ? fSlow98 : fSlow97) - ((iSlow92) ? fSlow100 : fSlow99))));
		float fSlow102 = fConst0 * fSlow101;
		float fSlow103 = float(fEntry24);
		float fSlow104 = fSlow85 * fSlow103 * fSlow88;
		int iSlow105 = fSlow104 > fSlow91;
		float fSlow106 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry25) + fSlow95));
		float fSlow107 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow104), 0.7f);
		float fSlow108 = std::pow(0.014705882f * float(fSlow104 >= 3e+01f) * std::min<float>(fSlow104 + -3e+01f, 68.0f), 1.8f);
		float fSlow109 = std::max<float>(0.001f, std::fabs(((iSlow105) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow106, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow106, 12.6f))) * (((iSlow105) ? fSlow100 : fSlow99) - ((iSlow105) ? fSlow108 : fSlow107))));
		float fSlow110 = float(fEntry26);
		float fSlow111 = fSlow85 * fSlow110 * fSlow88;
		int iSlow112 = fSlow111 > fSlow104;
		float fSlow113 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry27) + fSlow95));
		float fSlow114 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow111), 0.7f);
		float fSlow115 = std::pow(0.014705882f * float(fSlow111 >= 3e+01f) * std::min<float>(fSlow111 + -3e+01f, 68.0f), 1.8f);
		float fSlow116 = fSlow109 + std::max<float>(0.001f, std::fabs(((iSlow112) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow113, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow113, 12.6f))) * (((iSlow112) ? fSlow115 : fSlow114) - ((iSlow112) ? fSlow108 : fSlow107))));
		int iSlow117 = fSlow89 > fSlow111;
		float fSlow118 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry28) + fSlow95));
		float fSlow119 = fSlow116 + std::max<float>(0.001f, std::fabs(((iSlow117) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow118, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow118, 12.6f))) * (((iSlow117) ? fSlow115 : fSlow114) - ((iSlow117) ? fSlow98 : fSlow97))));
		float fSlow120 = fConst0 * fSlow119;
		float fSlow121 = fConst0 * fSlow116;
		int iTemp5 = iTempPerm5;
		float fSlow122 = fSlow85 * fSlow88;
		float fSlow123 = fConst1 * (fSlow110 - fSlow86);
		float fSlow124 = fSlow116 - fSlow119;
		float fSlow125 = fConst0 * fSlow109;
		int iTemp6 = iTempPerm6;
		float fSlow126 = fConst1 * (fSlow103 - fSlow110);
		float fSlow127 = fSlow109 - fSlow116;
		float fSlow128 = fConst1 * (fSlow103 - fSlow90);
		float fTemp7 = fTempPerm7;
		float fSlow129 = fConst1 * fSlow94;
		float fTemp8 = fTempPerm8;
		float fSlow130 = float(fEntry29);
		float fSlow131 = float(fEntry30);
		float fSlow132 = float(fEntry31);
		float fSlow133 = 0.0012626263f * fSlow39 * fSlow132 + 0.01010101f * (1.0f - 0.125f * fSlow132);
		float fSlow134 = fSlow130 * fSlow131 * fSlow133;
		float fSlow135 = float(fEntry32);
		float fSlow136 = fSlow130 * fSlow135 * fSlow133;
		int iSlow137 = fSlow134 <= fSlow136;
		float fSlow138 = float(fEntry37);
		float fSlow139 = 0.25f * float(fEntry35) + ((int(float(fEntry36))) ? fSlow138 : 4.4e+02f * fSlow46 * fSlow138);
		float fSlow140 = 0.057142857f * float(fEntry34) * (17.31234f * std::log(0.0022727272f * fSlow139) + 48.0f);
		float fSlow141 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry33) + fSlow140));
		float fSlow142 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow134), 0.7f);
		float fSlow143 = std::pow(0.014705882f * float(fSlow134 >= 3e+01f) * std::min<float>(fSlow134 + -3e+01f, 68.0f), 1.8f);
		float fSlow144 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow136), 0.7f);
		float fSlow145 = std::pow(0.014705882f * float(fSlow136 >= 3e+01f) * std::min<float>(fSlow136 + -3e+01f, 68.0f), 1.8f);
		float fSlow146 = std::max<float>(0.001f, std::fabs(((iSlow137) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow141, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow141, 12.6f))) * (((iSlow137) ? fSlow143 : fSlow142) - ((iSlow137) ? fSlow145 : fSlow144))));
		float fSlow147 = fConst0 * fSlow146;
		float fSlow148 = float(fEntry38);
		float fSlow149 = fSlow130 * fSlow148 * fSlow133;
		int iSlow150 = fSlow149 > fSlow136;
		float fSlow151 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry39) + fSlow140));
		float fSlow152 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow149), 0.7f);
		float fSlow153 = std::pow(0.014705882f * float(fSlow149 >= 3e+01f) * std::min<float>(fSlow149 + -3e+01f, 68.0f), 1.8f);
		float fSlow154 = std::max<float>(0.001f, std::fabs(((iSlow150) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow151, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow151, 12.6f))) * (((iSlow150) ? fSlow145 : fSlow144) - ((iSlow150) ? fSlow153 : fSlow152))));
		float fSlow155 = float(fEntry40);
		float fSlow156 = fSlow130 * fSlow155 * fSlow133;
		int iSlow157 = fSlow156 > fSlow149;
		float fSlow158 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry41) + fSlow140));
		float fSlow159 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow156), 0.7f);
		float fSlow160 = std::pow(0.014705882f * float(fSlow156 >= 3e+01f) * std::min<float>(fSlow156 + -3e+01f, 68.0f), 1.8f);
		float fSlow161 = fSlow154 + std::max<float>(0.001f, std::fabs(((iSlow157) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow158, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow158, 12.6f))) * (((iSlow157) ? fSlow160 : fSlow159) - ((iSlow157) ? fSlow153 : fSlow152))));
		int iSlow162 = fSlow134 > fSlow156;
		float fSlow163 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry42) + fSlow140));
		float fSlow164 = fSlow161 + std::max<float>(0.001f, std::fabs(((iSlow162) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow163, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow163, 12.6f))) * (((iSlow162) ? fSlow160 : fSlow159) - ((iSlow162) ? fSlow143 : fSlow142))));
		float fSlow165 = fConst0 * fSlow164;
		float fSlow166 = fConst0 * fSlow161;
		int iTemp9 = iTempPerm9;
		float fSlow167 = fSlow130 * fSlow133;
		float fSlow168 = fConst1 * (fSlow131 - fSlow155);
		float fSlow169 = fSlow161 - fSlow164;
		float fSlow170 = fConst0 * fSlow154;
		int iTemp10 = iTempPerm10;
		float fSlow171 = fConst1 * (fSlow148 - fSlow155);
		float fSlow172 = fSlow154 - fSlow161;
		float fSlow173 = fConst1 * (fSlow135 - fSlow148);
		float fTemp11 = fTempPerm11;
		float fSlow174 = fConst1 * fSlow139;
		float fTemp12 = fTempPerm12;
		float fTemp13 = fTempPerm13;
		float fTemp14 = fTempPerm14;
		float fTemp15 = fTempPerm15;
		float fTemp16 = fTempPerm16;
		float fSlow175 = float(fEntry43);
		float fSlow176 = float(fEntry44);
		float fSlow177 = float(fEntry45);
		float fSlow178 = 0.0012626263f * fSlow39 * fSlow177 + 0.01010101f * (1.0f - 0.125f * fSlow177);
		float fSlow179 = fSlow175 * fSlow176 * fSlow178;
		float fSlow180 = float(fEntry46);
		float fSlow181 = fSlow175 * fSlow180 * fSlow178;
		int iSlow182 = fSlow179 <= fSlow181;
		float fSlow183 = float(fEntry51);
		float fSlow184 = 0.25f * float(fEntry49) + ((int(float(fEntry50))) ? fSlow183 : 4.4e+02f * fSlow46 * fSlow183);
		float fSlow185 = 0.057142857f * float(fEntry48) * (17.31234f * std::log(0.0022727272f * fSlow184) + 48.0f);
		float fSlow186 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry47) + fSlow185));
		float fSlow187 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow181), 0.7f);
		float fSlow188 = std::pow(0.014705882f * float(fSlow181 >= 3e+01f) * std::min<float>(fSlow181 + -3e+01f, 68.0f), 1.8f);
		float fSlow189 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow179), 0.7f);
		float fSlow190 = std::pow(0.014705882f * float(fSlow179 >= 3e+01f) * std::min<float>(fSlow179 + -3e+01f, 68.0f), 1.8f);
		float fSlow191 = std::max<float>(0.001f, std::fabs(-(((iSlow182) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow186, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow186, 12.6f))) * (((iSlow182) ? fSlow188 : fSlow187) - ((iSlow182) ? fSlow190 : fSlow189)))));
		float fSlow192 = fConst0 * fSlow191;
		float fSlow193 = float(fEntry52);
		float fSlow194 = fSlow175 * fSlow193 * fSlow178;
		int iSlow195 = fSlow194 > fSlow181;
		float fSlow196 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry53) + fSlow185));
		float fSlow197 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow194), 0.7f);
		float fSlow198 = std::pow(0.014705882f * float(fSlow194 >= 3e+01f) * std::min<float>(fSlow194 + -3e+01f, 68.0f), 1.8f);
		float fSlow199 = std::max<float>(0.001f, std::fabs(((iSlow195) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow196, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow196, 12.6f))) * (((iSlow195) ? fSlow188 : fSlow187) - ((iSlow195) ? fSlow198 : fSlow197))));
		float fSlow200 = float(fEntry54);
		float fSlow201 = fSlow175 * fSlow200 * fSlow178;
		int iSlow202 = fSlow201 > fSlow194;
		float fSlow203 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry55) + fSlow185));
		float fSlow204 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow201), 0.7f);
		float fSlow205 = std::pow(0.014705882f * float(fSlow201 >= 3e+01f) * std::min<float>(fSlow201 + -3e+01f, 68.0f), 1.8f);
		float fSlow206 = fSlow199 + std::max<float>(0.001f, std::fabs(((iSlow202) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow203, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow203, 12.6f))) * (((iSlow202) ? fSlow205 : fSlow204) - ((iSlow202) ? fSlow198 : fSlow197))));
		int iSlow207 = fSlow179 > fSlow201;
		float fSlow208 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry56) + fSlow185));
		float fSlow209 = fSlow206 + std::max<float>(0.001f, std::fabs(((iSlow207) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow208, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow208, 12.6f))) * (((iSlow207) ? fSlow205 : fSlow204) - ((iSlow207) ? fSlow190 : fSlow189))));
		float fSlow210 = fConst0 * fSlow209;
		float fSlow211 = fConst0 * fSlow206;
		int iTemp17 = iTempPerm17;
		float fSlow212 = fSlow175 * fSlow178;
		float fSlow213 = fConst1 * (fSlow200 - fSlow176);
		float fSlow214 = fSlow206 - fSlow209;
		float fSlow215 = fConst0 * fSlow199;
		int iTemp18 = iTempPerm18;
		float fSlow216 = fConst1 * (fSlow193 - fSlow200);
		float fSlow217 = fSlow199 - fSlow206;
		float fSlow218 = fConst1 * (fSlow193 - fSlow180);
		float fTemp19 = fTempPerm19;
		float fSlow219 = fConst1 * fSlow184;
		float fTemp20 = fTempPerm20;
		float fSlow220 = float(fEntry57);
		float fSlow221 = float(fEntry58);
		float fSlow222 = float(fEntry59);
		float fSlow223 = 0.0012626263f * fSlow39 * fSlow222 + 0.01010101f * (1.0f - 0.125f * fSlow222);
		float fSlow224 = fSlow220 * fSlow221 * fSlow223;
		float fSlow225 = float(fEntry60);
		float fSlow226 = fSlow220 * fSlow225 * fSlow223;
		int iSlow227 = fSlow224 <= fSlow226;
		float fSlow228 = float(fEntry65);
		float fSlow229 = 0.25f * float(fEntry63) + ((int(float(fEntry64))) ? fSlow228 : 4.4e+02f * fSlow46 * fSlow228);
		float fSlow230 = 0.057142857f * float(fEntry62) * (17.31234f * std::log(0.0022727272f * fSlow229) + 48.0f);
		float fSlow231 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry61) + fSlow230));
		float fSlow232 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow224), 0.7f);
		float fSlow233 = std::pow(0.014705882f * float(fSlow224 >= 3e+01f) * std::min<float>(fSlow224 + -3e+01f, 68.0f), 1.8f);
		float fSlow234 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow226), 0.7f);
		float fSlow235 = std::pow(0.014705882f * float(fSlow226 >= 3e+01f) * std::min<float>(fSlow226 + -3e+01f, 68.0f), 1.8f);
		float fSlow236 = std::max<float>(0.001f, std::fabs(((iSlow227) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow231, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow231, 12.6f))) * (((iSlow227) ? fSlow233 : fSlow232) - ((iSlow227) ? fSlow235 : fSlow234))));
		float fSlow237 = fConst0 * fSlow236;
		float fSlow238 = float(fEntry66);
		float fSlow239 = fSlow220 * fSlow238 * fSlow223;
		int iSlow240 = fSlow239 > fSlow226;
		float fSlow241 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry67) + fSlow230));
		float fSlow242 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow239), 0.7f);
		float fSlow243 = std::pow(0.014705882f * float(fSlow239 >= 3e+01f) * std::min<float>(fSlow239 + -3e+01f, 68.0f), 1.8f);
		float fSlow244 = std::max<float>(0.001f, std::fabs(-(((iSlow240) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow241, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow241, 12.6f))) * (((iSlow240) ? fSlow243 : fSlow242) - ((iSlow240) ? fSlow235 : fSlow234)))));
		float fSlow245 = float(fEntry68);
		float fSlow246 = fSlow220 * fSlow245 * fSlow223;
		int iSlow247 = fSlow246 > fSlow239;
		float fSlow248 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry69) + fSlow230));
		float fSlow249 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow246), 0.7f);
		float fSlow250 = std::pow(0.014705882f * float(fSlow246 >= 3e+01f) * std::min<float>(fSlow246 + -3e+01f, 68.0f), 1.8f);
		float fSlow251 = fSlow244 + std::max<float>(0.001f, std::fabs(((iSlow247) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow248, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow248, 12.6f))) * (((iSlow247) ? fSlow250 : fSlow249) - ((iSlow247) ? fSlow243 : fSlow242))));
		int iSlow252 = fSlow224 > fSlow246;
		float fSlow253 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry70) + fSlow230));
		float fSlow254 = fSlow251 + std::max<float>(0.001f, std::fabs(((iSlow252) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow253, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow253, 12.6f))) * (((iSlow252) ? fSlow250 : fSlow249) - ((iSlow252) ? fSlow233 : fSlow232))));
		float fSlow255 = fConst0 * fSlow254;
		float fSlow256 = fConst0 * fSlow251;
		int iTemp21 = iTempPerm21;
		float fSlow257 = fSlow220 * fSlow223;
		float fSlow258 = fConst1 * (fSlow245 - fSlow221);
		float fSlow259 = fSlow251 - fSlow254;
		float fSlow260 = fConst0 * fSlow244;
		int iTemp22 = iTempPerm22;
		float fSlow261 = fConst1 * (fSlow245 - fSlow238);
		float fSlow262 = fSlow244 - fSlow251;
		float fSlow263 = fConst1 * (fSlow225 - fSlow238);
		float fTemp23 = fTempPerm23;
		float fSlow264 = fConst1 * fSlow229;
		float fTemp24 = fTempPerm24;
		float fTemp25 = fTempPerm25;
		float fSlow265 = float(fEntry71);
		float fSlow266 = 3650.6323f * fSlow265;
		float fSlow267 = float(fEntry72);
		float fSlow268 = float(fEntry73);
		float fSlow269 = float(fEntry74);
		float fSlow270 = 0.0012626263f * fSlow39 * fSlow269 + 0.01010101f * (1.0f - 0.125f * fSlow269);
		float fSlow271 = fSlow267 * fSlow268 * fSlow270;
		float fSlow272 = float(fEntry75);
		float fSlow273 = fSlow267 * fSlow272 * fSlow270;
		int iSlow274 = fSlow271 <= fSlow273;
		float fSlow275 = float(fEntry80);
		float fSlow276 = 0.25f * float(fEntry78) + ((int(float(fEntry79))) ? fSlow275 : 4.4e+02f * fSlow46 * fSlow275);
		float fSlow277 = 0.057142857f * float(fEntry77) * (17.31234f * std::log(0.0022727272f * fSlow276) + 48.0f);
		float fSlow278 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry76) + fSlow277));
		float fSlow279 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow271), 0.7f);
		float fSlow280 = std::pow(0.014705882f * float(fSlow271 >= 3e+01f) * std::min<float>(fSlow271 + -3e+01f, 68.0f), 1.8f);
		float fSlow281 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow273), 0.7f);
		float fSlow282 = std::pow(0.014705882f * float(fSlow273 >= 3e+01f) * std::min<float>(fSlow273 + -3e+01f, 68.0f), 1.8f);
		float fSlow283 = std::max<float>(0.001f, std::fabs(((iSlow274) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow278, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow278, 12.6f))) * (((iSlow274) ? fSlow280 : fSlow279) - ((iSlow274) ? fSlow282 : fSlow281))));
		float fSlow284 = fConst0 * fSlow283;
		float fSlow285 = float(fEntry81);
		float fSlow286 = fSlow267 * fSlow285 * fSlow270;
		int iSlow287 = fSlow286 > fSlow273;
		float fSlow288 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry82) + fSlow277));
		float fSlow289 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow286), 0.7f);
		float fSlow290 = std::pow(0.014705882f * float(fSlow286 >= 3e+01f) * std::min<float>(fSlow286 + -3e+01f, 68.0f), 1.8f);
		float fSlow291 = std::max<float>(0.001f, std::fabs(((iSlow287) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow288, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow288, 12.6f))) * (((iSlow287) ? fSlow282 : fSlow281) - ((iSlow287) ? fSlow290 : fSlow289))));
		float fSlow292 = float(fEntry83);
		float fSlow293 = fSlow267 * fSlow292 * fSlow270;
		int iSlow294 = fSlow293 > fSlow286;
		float fSlow295 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry84) + fSlow277));
		float fSlow296 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow293), 0.7f);
		float fSlow297 = std::pow(0.014705882f * float(fSlow293 >= 3e+01f) * std::min<float>(fSlow293 + -3e+01f, 68.0f), 1.8f);
		float fSlow298 = fSlow291 + std::max<float>(0.001f, std::fabs(((iSlow294) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow295, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow295, 12.6f))) * (((iSlow294) ? fSlow297 : fSlow296) - ((iSlow294) ? fSlow290 : fSlow289))));
		int iSlow299 = fSlow271 > fSlow293;
		float fSlow300 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry85) + fSlow277));
		float fSlow301 = fSlow298 + std::max<float>(0.001f, std::fabs(((iSlow299) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow300, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow300, 12.6f))) * (((iSlow299) ? fSlow297 : fSlow296) - ((iSlow299) ? fSlow280 : fSlow279))));
		float fSlow302 = fConst0 * fSlow301;
		float fSlow303 = fConst0 * fSlow298;
		int iTemp26 = iTempPerm26;
		float fSlow304 = fSlow267 * fSlow270;
		float fSlow305 = fConst1 * (fSlow268 - fSlow292);
		float fSlow306 = fSlow298 - fSlow301;
		float fSlow307 = fConst0 * fSlow291;
		int iTemp27 = iTempPerm27;
		float fSlow308 = fConst1 * (fSlow285 - fSlow292);
		float fSlow309 = fSlow291 - fSlow298;
		float fSlow310 = fConst1 * (fSlow272 - fSlow285);
		float fTemp28 = fTempPerm28;
		float fSlow311 = fConst1 * fSlow276;
		float fTemp29 = fTempPerm29;
		float fTemp30 = fTempPerm30;
		float fSlow312 = 9126.581f * fSlow265;
		float fTemp31 = fTempPerm31;
		float fTemp32 = fTempPerm32;
		float fTemp33 = fTempPerm33;
		float fTemp34 = fTempPerm34;
		float fTemp35 = fTempPerm35;
		float fTemp36 = fTempPerm36;
		float fSlow313 = 4563.2905f * fSlow265;
		float fTemp37 = fTempPerm37;
		float fTemp38 = fTempPerm38;
		float fTemp39 = fTempPerm39;
		float fTemp40 = fTempPerm40;
		float fTemp41 = fTempPerm41;
		float fTemp42 = fTempPerm42;
		float fTemp43 = fTempPerm43;
		float fTemp44 = fTempPerm44;
		float fTemp45 = fTempPerm45;
		float fTemp46 = fTempPerm46;
		float fTemp47 = fTempPerm47;
		float fTemp48 = fTempPerm48;
		float fTemp49 = fTempPerm49;
		float fTemp50 = fTempPerm50;
		float fTemp51 = fTempPerm51;
		float fTemp52 = fTempPerm52;
		float fSlow314 = 912.6581f * fSlow265;
		float fTemp53 = fTempPerm53;
		float fTemp54 = fTempPerm54;
		float fTemp55 = fTempPerm55;
		float fTemp56 = fTempPerm56;
		float fTemp57 = fTempPerm57;
		float fSlow315 = 5475.9487f * fSlow265;
		float fTemp58 = fTempPerm58;
		float fTemp59 = fTempPerm59;
		float fTemp60 = fTempPerm60;
		float fTemp61 = fTempPerm61;
		float fTemp62 = fTempPerm62;
		float fTemp63 = fTempPerm63;
		float fTemp64 = fTempPerm64;
		float fTemp65 = fTempPerm65;
		float fTemp66 = fTempPerm66;
		float fTemp67 = fTempPerm67;
		float fTemp68 = fTempPerm68;
		float fTemp69 = fTempPerm69;
		float fSlow316 = 1825.3162f * fSlow265;
		float fTemp70 = fTempPerm70;
		float fTemp71 = fTempPerm71;
		float fTemp72 = fTempPerm72;
		float fTemp73 = fTempPerm73;
		float fTemp74 = fTempPerm74;
		float fTemp75 = fTempPerm75;
		float fTemp76 = fTempPerm76;
		float fTemp77 = fTempPerm77;
		float fTemp78 = fTempPerm78;
		float fTemp79 = fTempPerm79;
		float fTemp80 = fTempPerm80;
		float fTemp81 = fTempPerm81;
		float fTemp82 = fTempPerm82;
		float fTemp83 = fTempPerm83;
		float fTemp84 = fTempPerm84;
		float fTemp85 = fTempPerm85;
		float fTemp86 = fTempPerm86;
		float fTemp87 = fTempPerm87;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | (iSlow16 | (iSlow17 | (iSlow18 | (iSlow19 | (iSlow20 | (iSlow21 | (iSlow22 | (iSlow23 | (iSlow24 | (iSlow25 | (iSlow26 | (iSlow27 | (iSlow28 | (iSlow1 | (iSlow29 | (iSlow30 | (iSlow31 | iSlow32))))))))))))))))))))))))))))))) {
				fRec0[0] = ((iSlow35) ? 0.0f : std::min<float>(fSlow55, fRec0[1] + 1.0f));
				fRec2[0] = ((iSlow57) ? 0.0f : std::min<float>(fSlow75, fRec2[1] + 1.0f));
				iTemp0 = fRec2[0] < fSlow76;
				iTemp1 = fRec2[0] < fSlow80;
				fRec1[0] = ((iSlow34) ? fSlow56 * ((iTemp0) ? ((iTemp1) ? ((fRec2[0] < 0.0f) ? fSlow43 : ((iTemp1) ? -(fSlow77 * (fSlow83 * (fRec2[0] / fSlow64) - fSlow42)) : fSlow59)) : ((iTemp0) ? fSlow77 * (fSlow58 + fSlow81 * ((fRec2[0] - fSlow80) / fSlow82)) : fSlow66)) : ((fRec2[0] < fSlow75) ? fSlow77 * (fSlow65 + fSlow78 * ((fRec2[0] - fSlow76) / fSlow79)) : fSlow41)) : fRec1[1]);
				fTemp2 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec0[0] < 0.0f) ? fRec1[0] : ((fRec0[0] < fSlow55) ? fRec1[0] + fConst1 * (fRec0[0] * (fSlow43 - fRec1[0]) / fSlow54) : fSlow43))));
				iTemp3 = 1 - iVec0[1];
				fTemp4 = ((iTemp3) ? 0.0f : fRec4[1] + fSlow84);
				fRec4[0] = fTemp4 - std::floor(fTemp4);
				fRec5[0] = ((iSlow35) ? 0.0f : std::min<float>(fSlow102, fRec5[1] + 1.0f));
				fRec7[0] = ((iSlow57) ? 0.0f : std::min<float>(fSlow120, fRec7[1] + 1.0f));
				iTemp5 = fRec7[0] < fSlow121;
				iTemp6 = fRec7[0] < fSlow125;
				fRec6[0] = ((iSlow34) ? fSlow56 * ((iTemp5) ? ((iTemp6) ? ((fRec7[0] < 0.0f) ? fSlow91 : ((iTemp6) ? fSlow122 * (fSlow90 + fSlow128 * (fRec7[0] / fSlow109)) : fSlow104)) : ((iTemp5) ? fSlow122 * (fSlow103 + fSlow126 * ((fRec7[0] - fSlow125) / fSlow127)) : fSlow111)) : ((fRec7[0] < fSlow120) ? fSlow122 * (fSlow110 + fSlow123 * ((fRec7[0] - fSlow121) / fSlow124)) : fSlow89)) : fRec6[1]);
				fTemp7 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec5[0] < 0.0f) ? fRec6[0] : ((fRec5[0] < fSlow102) ? fRec6[0] + fConst1 * (fRec5[0] * (fSlow91 - fRec6[0]) / fSlow101) : fSlow91))));
				fTemp8 = ((iTemp3) ? 0.0f : fRec8[1] + fSlow129);
				fRec8[0] = fTemp8 - std::floor(fTemp8);
				fRec9[0] = ((iSlow35) ? 0.0f : std::min<float>(fSlow147, fRec9[1] + 1.0f));
				fRec11[0] = ((iSlow57) ? 0.0f : std::min<float>(fSlow165, fRec11[1] + 1.0f));
				iTemp9 = fRec11[0] < fSlow166;
				iTemp10 = fRec11[0] < fSlow170;
				fRec10[0] = ((iSlow34) ? fSlow56 * ((iTemp9) ? ((iTemp10) ? ((fRec11[0] < 0.0f) ? fSlow136 : ((iTemp10) ? fSlow167 * (fSlow135 - fSlow173 * (fRec11[0] / fSlow154)) : fSlow149)) : ((iTemp9) ? fSlow167 * (fSlow148 + fSlow171 * ((fRec11[0] - fSlow170) / fSlow172)) : fSlow156)) : ((fRec11[0] < fSlow165) ? fSlow167 * (fSlow155 - fSlow168 * ((fRec11[0] - fSlow166) / fSlow169)) : fSlow134)) : fRec10[1]);
				fTemp11 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec9[0] < 0.0f) ? fRec10[0] : ((fRec9[0] < fSlow147) ? fRec10[0] + fConst1 * (fRec9[0] * (fSlow136 - fRec10[0]) / fSlow146) : fSlow136))));
				fTemp12 = ((iTemp3) ? 0.0f : fRec12[1] + fSlow174);
				fRec12[0] = fTemp12 - std::floor(fTemp12);
			}
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow8 | (iSlow16 | (iSlow17 | (iSlow18 | (iSlow20 | (iSlow21 | (iSlow22 | (iSlow28 | (iSlow1 | (iSlow29 | iSlow32))))))))))))) {
				fTemp13 = 65536.0f * fRec12[0];
			}
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow18 | (iSlow20 | (iSlow21 | (iSlow28 | (iSlow1 | iSlow32)))))))) {
				fTemp14 = fTemp11 * ftbl0synth_dx7SIG0[(int(fTemp13) % 65536 + 65536) % 65536];
			}
			if (iSlow2 | (iSlow4 | (iSlow18 | (iSlow21 | (iSlow28 | iSlow1))))) {
				fTemp15 = fTemp7 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec8[0] + fTemp14)) % 65536 + 65536) % 65536];
			}
			if (iSlow2 | (iSlow4 | (iSlow18 | iSlow1))) {
				fTemp16 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp15)) % 65536 + 65536) % 65536];
			}
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | (iSlow16 | (iSlow17 | (iSlow18 | (iSlow19 | (iSlow20 | (iSlow21 | (iSlow22 | (iSlow23 | (iSlow24 | (iSlow25 | (iSlow26 | (iSlow27 | (iSlow28 | (iSlow1 | (iSlow29 | (iSlow30 | (iSlow31 | iSlow32))))))))))))))))))))))))))))))) {
				fRec13[0] = ((iSlow35) ? 0.0f : std::min<float>(fSlow192, fRec13[1] + 1.0f));
				fRec15[0] = ((iSlow57) ? 0.0f : std::min<float>(fSlow210, fRec15[1] + 1.0f));
				iTemp17 = fRec15[0] < fSlow211;
				iTemp18 = fRec15[0] < fSlow215;
				fRec14[0] = ((iSlow34) ? fSlow56 * ((iTemp17) ? ((iTemp18) ? ((fRec15[0] < 0.0f) ? fSlow181 : ((iTemp18) ? fSlow212 * (fSlow180 + fSlow218 * (fRec15[0] / fSlow199)) : fSlow194)) : ((iTemp17) ? fSlow212 * (fSlow193 + fSlow216 * ((fRec15[0] - fSlow215) / fSlow217)) : fSlow201)) : ((fRec15[0] < fSlow210) ? fSlow212 * (fSlow200 + fSlow213 * ((fRec15[0] - fSlow211) / fSlow214)) : fSlow179)) : fRec14[1]);
				fTemp19 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec13[0] < 0.0f) ? fRec14[0] : ((fRec13[0] < fSlow192) ? fRec14[0] + fConst1 * (fRec13[0] * (fSlow181 - fRec14[0]) / fSlow191) : fSlow181))));
				fTemp20 = ((iTemp3) ? 0.0f : fRec16[1] + fSlow219);
				fRec16[0] = fTemp20 - std::floor(fTemp20);
				fRec18[0] = ((iSlow35) ? 0.0f : std::min<float>(fSlow237, fRec18[1] + 1.0f));
				fRec20[0] = ((iSlow57) ? 0.0f : std::min<float>(fSlow255, fRec20[1] + 1.0f));
				iTemp21 = fRec20[0] < fSlow256;
				iTemp22 = fRec20[0] < fSlow260;
				fRec19[0] = ((iSlow34) ? fSlow56 * ((iTemp21) ? ((iTemp22) ? ((fRec20[0] < 0.0f) ? fSlow226 : ((iTemp22) ? -(fSlow257 * (fSlow263 * (fRec20[0] / fSlow244) - fSlow225)) : fSlow239)) : ((iTemp21) ? -(fSlow257 * (fSlow261 * ((fRec20[0] - fSlow260) / fSlow262) - fSlow238)) : fSlow246)) : ((fRec20[0] < fSlow255) ? fSlow257 * (fSlow245 + fSlow258 * ((fRec20[0] - fSlow256) / fSlow259)) : fSlow224)) : fRec19[1]);
				fTemp23 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec18[0] < 0.0f) ? fRec19[0] : ((fRec18[0] < fSlow237) ? fRec19[0] + fConst1 * (fRec18[0] * (fSlow226 - fRec19[0]) / fSlow236) : fSlow226))));
				fTemp24 = ((iTemp3) ? 0.0f : fRec21[1] + fSlow264);
				fRec21[0] = fTemp24 - std::floor(fTemp24);
				fTemp25 = 65536.0f * fRec21[0];
			}
			if (iSlow1) {
				fRec17[0] = fTemp23 * ftbl0synth_dx7SIG0[(int(fTemp25 + fSlow266 * fRec17[1]) % 65536 + 65536) % 65536];
			}
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | (iSlow16 | (iSlow17 | (iSlow18 | (iSlow19 | (iSlow20 | (iSlow21 | (iSlow22 | (iSlow23 | (iSlow24 | (iSlow25 | (iSlow26 | (iSlow27 | (iSlow28 | (iSlow1 | (iSlow29 | (iSlow30 | (iSlow31 | iSlow32))))))))))))))))))))))))))))))) {
				fRec22[0] = ((iSlow35) ? 0.0f : std::min<float>(fSlow284, fRec22[1] + 1.0f));
				fRec24[0] = ((iSlow57) ? 0.0f : std::min<float>(fSlow302, fRec24[1] + 1.0f));
				iTemp26 = fRec24[0] < fSlow303;
				iTemp27 = fRec24[0] < fSlow307;
				fRec23[0] = ((iSlow34) ? fSlow56 * ((iTemp26) ? ((iTemp27) ? ((fRec24[0] < 0.0f) ? fSlow273 : ((iTemp27) ? -(fSlow304 * (fSlow310 * (fRec24[0] / fSlow291) - fSlow272)) : fSlow286)) : ((iTemp26) ? fSlow304 * (fSlow285 + fSlow308 * ((fRec24[0] - fSlow307) / fSlow309)) : fSlow293)) : ((fRec24[0] < fSlow302) ? -(fSlow304 * (fSlow305 * ((fRec24[0] - fSlow303) / fSlow306) - fSlow292)) : fSlow271)) : fRec23[1]);
				fTemp28 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec22[0] < 0.0f) ? fRec23[0] : ((fRec22[0] < fSlow284) ? fRec23[0] + fConst1 * (fRec22[0] * (fSlow273 - fRec23[0]) / fSlow283) : fSlow273))));
				fTemp29 = ((iTemp3) ? 0.0f : fRec25[1] + fSlow311);
				fRec25[0] = fTemp29 - std::floor(fTemp29);
			}
			if (iSlow1) {
				fTemp30 = 2.08795f * (fTemp16 + fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec17[0])) % 65536 + 65536) % 65536] + fTemp28 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fRec17[0])) % 65536 + 65536) % 65536]);
			}
			if (iSlow16 | (iSlow17 | (iSlow22 | iSlow29))) {
				fRec26[0] = fTemp11 * ftbl0synth_dx7SIG0[(int(fTemp13 + fSlow312 * fRec26[1]) % 65536 + 65536) % 65536];
			}
			if (iSlow3 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | (iSlow19 | (iSlow20 | (iSlow23 | (iSlow24 | (iSlow25 | (iSlow26 | (iSlow27 | (iSlow29 | (iSlow30 | (iSlow31 | iSlow32))))))))))))))))))))) {
				fTemp31 = 65536.0f * fRec8[0];
			}
			if (iSlow3 | (iSlow6 | (iSlow7 | (iSlow9 | (iSlow11 | (iSlow13 | (iSlow15 | (iSlow19 | (iSlow20 | (iSlow23 | (iSlow24 | (iSlow25 | (iSlow26 | (iSlow27 | (iSlow29 | (iSlow31 | iSlow32)))))))))))))))) {
				fTemp32 = fTemp7 * ftbl0synth_dx7SIG0[(int(fTemp31) % 65536 + 65536) % 65536];
			}
			if (iSlow3 | (iSlow5 | (iSlow8 | (iSlow10 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow16 | (iSlow17 | (iSlow22 | (iSlow23 | (iSlow25 | (iSlow29 | iSlow30))))))))))))) {
				fTemp33 = fTemp23 * ftbl0synth_dx7SIG0[(int(fTemp25) % 65536 + 65536) % 65536];
			}
			if (iSlow10 | (iSlow12 | (iSlow13 | (iSlow17 | (iSlow29 | iSlow30))))) {
				fTemp34 = fTemp28 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fTemp33)) % 65536 + 65536) % 65536];
			}
			if (iSlow10 | iSlow29) {
				fTemp35 = fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp34)) % 65536 + 65536) % 65536];
			}
			if (iSlow29) {
				fTemp36 = 2.08795f * fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec26[0] + fTemp32 + fTemp35)) % 65536 + 65536) % 65536];
			}
			if (iSlow30) {
				fRec27[0] = fTemp7 * ftbl0synth_dx7SIG0[(int(fTemp31 + fSlow313 * fRec27[1]) % 65536 + 65536) % 65536];
			}
			if (iSlow3 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow9 | (iSlow12 | (iSlow13 | (iSlow24 | (iSlow26 | (iSlow30 | (iSlow31 | iSlow32))))))))))) {
				fTemp37 = 65536.0f * fRec16[0];
			}
			if (iSlow3 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow9 | (iSlow12 | (iSlow24 | (iSlow26 | (iSlow30 | (iSlow31 | iSlow32)))))))))) {
				fTemp38 = fTemp19 * ftbl0synth_dx7SIG0[(int(fTemp37) % 65536 + 65536) % 65536];
			}
			if (iSlow7 | (iSlow24 | (iSlow26 | (iSlow30 | iSlow31)))) {
				fTemp39 = fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp38)) % 65536 + 65536) % 65536];
			}
			if (iSlow30) {
				fTemp40 = 2.08795f * fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec27[0] + fTemp34 + fTemp39)) % 65536 + 65536) % 65536];
			}
			if (iSlow6 | (iSlow9 | (iSlow15 | (iSlow18 | (iSlow20 | (iSlow21 | (iSlow24 | (iSlow28 | (iSlow31 | iSlow32))))))))) {
				fRec28[0] = fTemp23 * ftbl0synth_dx7SIG0[(int(fTemp25 + fSlow312 * fRec28[1]) % 65536 + 65536) % 65536];
			}
			if (iSlow9 | (iSlow18 | (iSlow20 | (iSlow24 | (iSlow28 | (iSlow31 | iSlow32)))))) {
				fTemp41 = fTemp28 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fRec28[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow24 | iSlow31) {
				fTemp42 = fTemp41 + fTemp39;
			}
			if (iSlow31) {
				fTemp43 = 2.08795f * fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp32 + fTemp42)) % 65536 + 65536) % 65536];
			}
			if (iSlow3 | (iSlow19 | (iSlow20 | (iSlow21 | (iSlow22 | (iSlow23 | (iSlow24 | (iSlow27 | (iSlow28 | iSlow32))))))))) {
				fTemp44 = 65536.0f * fRec4[0];
			}
			if (iSlow3 | (iSlow19 | (iSlow20 | (iSlow21 | (iSlow22 | (iSlow23 | (iSlow24 | (iSlow28 | iSlow32)))))))) {
				fTemp45 = fTemp2 * ftbl0synth_dx7SIG0[(int(fTemp44) % 65536 + 65536) % 65536];
			}
			if (iSlow32) {
				fTemp46 = 2.08795f * (fTemp32 + fTemp45 + fTemp41 + fTemp14 + fTemp38);
			}
			if (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow8 | (iSlow14 | (iSlow15 | (iSlow16 | (iSlow21 | (iSlow22 | (iSlow23 | iSlow25))))))))))) {
				fTemp47 = 65536.0f * fRec25[0];
			}
			if (iSlow23 | iSlow25) {
				fRec29[0] = fTemp28 * ftbl0synth_dx7SIG0[(int(fTemp47 + fSlow312 * fRec29[1]) % 65536 + 65536) % 65536];
				fTemp48 = fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec29[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
			}
			if (iSlow23) {
				fTemp49 = 2.08795f * (fTemp32 + fTemp48 + fTemp33 + fTemp45);
			}
			if (iSlow24) {
				fTemp50 = 2.08795f * (fTemp32 + fTemp42 + fTemp45);
			}
			if (iSlow6 | (iSlow7 | (iSlow9 | (iSlow11 | (iSlow13 | (iSlow15 | (iSlow25 | iSlow26))))))) {
				fTemp51 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp32)) % 65536 + 65536) % 65536];
			}
			if (iSlow25) {
				fTemp52 = 2.08795f * (fTemp51 + fTemp33 + fTemp48);
			}
			if (iSlow26 | iSlow27) {
				fRec30[0] = fTemp23 * ftbl0synth_dx7SIG0[(int(fTemp25 + fSlow314 * fRec30[1]) % 65536 + 65536) % 65536];
				fTemp53 = fTemp28 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fRec30[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow26) {
				fTemp54 = 2.08795f * (fTemp51 + fTemp39 + fTemp53);
			}
			if (iSlow27) {
				fTemp55 = fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fRec30[0])) % 65536 + 65536) % 65536] + 2.08795f * (fTemp2 * ftbl0synth_dx7SIG0[(int(fTemp44 + 136835.89f * fTemp32) % 65536 + 65536) % 65536] + fTemp53 + fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec30[0])) % 65536 + 65536) % 65536]);
			}
			if (iSlow20 | iSlow28) {
				fTemp56 = fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec28[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow28) {
				fTemp57 = 2.08795f * (fTemp56 + fTemp45 + fTemp15 + fTemp41);
			}
			if (iSlow19) {
				fRec31[0] = fTemp23 * ftbl0synth_dx7SIG0[(int(fTemp25 + fSlow315 * fRec31[1]) % 65536 + 65536) % 65536];
				fTemp58 = 2.08795f * (fTemp32 + fTemp28 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fRec31[0])) % 65536 + 65536) % 65536] + fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec31[0])) % 65536 + 65536) % 65536] + fTemp45 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fRec31[0])) % 65536 + 65536) % 65536]);
			}
			if (iSlow20) {
				fTemp59 = 2.08795f * (fTemp32 + fTemp56 + fTemp45 + fTemp14 + fTemp41);
			}
			if (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow8 | (iSlow14 | (iSlow15 | (iSlow16 | (iSlow21 | iSlow22))))))))) {
				fTemp60 = fTemp28 * ftbl0synth_dx7SIG0[(int(fTemp47) % 65536 + 65536) % 65536];
			}
			if (iSlow15 | iSlow21) {
				fTemp61 = fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec28[0] + fTemp60)) % 65536 + 65536) % 65536];
			}
			if (iSlow21) {
				fTemp62 = 2.08795f * (fTemp45 + fTemp15 + fTemp61);
			}
			if (iSlow16 | (iSlow17 | iSlow22)) {
				fTemp63 = fTemp7 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec8[0] + fRec26[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow8 | (iSlow14 | (iSlow16 | iSlow22))) {
				fTemp64 = fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp33 + fTemp60)) % 65536 + 65536) % 65536];
			}
			if (iSlow22) {
				fTemp65 = 2.08795f * (fTemp63 + fTemp64 + fTemp45);
			}
			if (iSlow16 | iSlow17) {
				fTemp66 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec26[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow16) {
				fTemp67 = 2.08795f * (fTemp63 + fTemp64 + fTemp66);
			}
			if (iSlow17) {
				fTemp68 = 2.08795f * (fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp33)) % 65536 + 65536) % 65536] + fTemp63 + fTemp34 + fTemp66);
			}
			if (iSlow18) {
				fTemp69 = 2.08795f * (fTemp16 + fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp41)) % 65536 + 65536) % 65536]);
			}
			if (iSlow5 | iSlow10) {
				fRec32[0] = fTemp7 * ftbl0synth_dx7SIG0[(int(fTemp31 + fSlow316 * fRec32[1]) % 65536 + 65536) % 65536];
				fTemp70 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec32[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow10) {
				fTemp71 = 2.08795f * (fTemp70 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp35)) % 65536 + 65536) % 65536]);
			}
			if (iSlow4 | iSlow11) {
				fRec33[0] = fTemp23 * ftbl0synth_dx7SIG0[(int(fTemp25 + fSlow316 * fRec33[1]) % 65536 + 65536) % 65536];
			}
			if (iSlow11) {
				fTemp72 = 2.08795f * (fTemp51 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp28 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fRec33[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow12 | iSlow14) {
				fRec34[0] = fTemp7 * ftbl0synth_dx7SIG0[(int(fTemp31 + fSlow266 * fRec34[1]) % 65536 + 65536) % 65536];
				fTemp73 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec34[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow12) {
				fTemp74 = 2.08795f * (fTemp73 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp34 + fTemp38)) % 65536 + 65536) % 65536]);
			}
			if (iSlow13) {
				fRec35[0] = fTemp19 * ftbl0synth_dx7SIG0[(int(fTemp37 + fSlow314 * fRec35[1]) % 65536 + 65536) % 65536];
				fTemp75 = 2.08795f * (fTemp51 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fRec35[0] + fTemp34)) % 65536 + 65536) % 65536]);
			}
			if (iSlow9) {
				fTemp76 = 2.08795f * (fTemp51 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp41 + fTemp38)) % 65536 + 65536) % 65536]);
			}
			if (iSlow7) {
				fRec36[0] = 2.08795f * fTemp28 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fTemp23 * ftbl0synth_dx7SIG0[(int(fTemp25 + fSlow312 * fRec36[1]) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
				fTemp77 = fRec36[0] + 2.08795f * (fTemp51 + fTemp39);
			}
			if (iSlow2) {
				fRec37[0] = 2.08795f * fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fTemp28 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fTemp23 * ftbl0synth_dx7SIG0[(int(fTemp25 + fSlow312 * fRec37[1]) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
				fTemp78 = fRec37[0] + 2.08795f * fTemp16;
			}
			if (iSlow8) {
				fRec38[0] = fTemp11 * ftbl0synth_dx7SIG0[(int(fTemp13 + fSlow316 * fRec38[1]) % 65536 + 65536) % 65536];
				fTemp79 = 2.08795f * (fTemp64 + fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp7 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec8[0] + fRec38[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow4) {
				fTemp80 = 2.08795f * (fTemp16 + fTemp19 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec16[0] + fRec33[0] + fTemp60)) % 65536 + 65536) % 65536]);
			}
			if (iSlow3 | (iSlow5 | iSlow6)) {
				fTemp81 = fTemp38 + fTemp60;
			}
			if (iSlow3 | iSlow5) {
				fTemp82 = fTemp33 + fTemp81;
			}
			if (iSlow5) {
				fTemp83 = 2.08795f * (fTemp70 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp82)) % 65536 + 65536) % 65536]);
			}
			if (iSlow6) {
				fTemp84 = 2.08795f * (fTemp51 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fRec28[0] + fTemp81)) % 65536 + 65536) % 65536]);
			}
			if (iSlow15) {
				fTemp85 = 2.08795f * (fTemp51 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp61)) % 65536 + 65536) % 65536]);
			}
			if (iSlow14) {
				fTemp86 = 2.08795f * (fTemp73 + fTemp11 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec12[0] + fTemp64)) % 65536 + 65536) % 65536]);
			}
			if (iSlow3) {
				fTemp87 = 2.08795f * (fTemp32 + fTemp45 + fTemp14 + fTemp82);
			}
			output0[i0] = FAUSTFLOAT(fTemp30 + fTemp36 + fTemp40 + fTemp43 + fTemp46 + fTemp49 + fTemp50 + fTemp52 + fTemp54 + fTemp55 + fTemp57 + fTemp58 + fTemp59 + fTemp62 + fTemp65 + fTemp67 + fTemp68 + fTemp69 + fTemp71 + fTemp72 + fTemp74 + fTemp75 + fTemp76 + fTemp77 + fTemp78 + fTemp79 + fTemp80 + fTemp83 + fTemp84 + fTemp85 + fTemp86 + fTemp87);
			iVec0[1] = iVec0[0];
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | (iSlow16 | (iSlow17 | (iSlow18 | (iSlow19 | (iSlow20 | (iSlow21 | (iSlow22 | (iSlow23 | (iSlow24 | (iSlow25 | (iSlow26 | (iSlow27 | (iSlow28 | (iSlow1 | (iSlow29 | (iSlow30 | (iSlow31 | iSlow32))))))))))))))))))))))))))))))) {
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
				fRec18[1] = fRec18[0];
				fRec20[1] = fRec20[0];
				fRec19[1] = fRec19[0];
				fRec21[1] = fRec21[0];
			}
			if (iSlow1) {
				fRec17[1] = fRec17[0];
			}
			if (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | (iSlow16 | (iSlow17 | (iSlow18 | (iSlow19 | (iSlow20 | (iSlow21 | (iSlow22 | (iSlow23 | (iSlow24 | (iSlow25 | (iSlow26 | (iSlow27 | (iSlow28 | (iSlow1 | (iSlow29 | (iSlow30 | (iSlow31 | iSlow32))))))))))))))))))))))))))))))) {
				fRec22[1] = fRec22[0];
				fRec24[1] = fRec24[0];
				fRec23[1] = fRec23[0];
				fRec25[1] = fRec25[0];
			}
			if (iSlow16 | (iSlow17 | (iSlow22 | iSlow29))) {
				fRec26[1] = fRec26[0];
			}
			if (iSlow30) {
				fRec27[1] = fRec27[0];
			}
			if (iSlow6 | (iSlow9 | (iSlow15 | (iSlow18 | (iSlow20 | (iSlow21 | (iSlow24 | (iSlow28 | (iSlow31 | iSlow32))))))))) {
				fRec28[1] = fRec28[0];
			}
			if (iSlow23 | iSlow25) {
				fRec29[1] = fRec29[0];
			}
			if (iSlow26 | iSlow27) {
				fRec30[1] = fRec30[0];
			}
			if (iSlow19) {
				fRec31[1] = fRec31[0];
			}
			if (iSlow5 | iSlow10) {
				fRec32[1] = fRec32[0];
			}
			if (iSlow4 | iSlow11) {
				fRec33[1] = fRec33[0];
			}
			if (iSlow12 | iSlow14) {
				fRec34[1] = fRec34[0];
			}
			if (iSlow13) {
				fRec35[1] = fRec35[0];
			}
			if (iSlow7) {
				fRec36[1] = fRec36[0];
			}
			if (iSlow2) {
				fRec37[1] = fRec37[0];
			}
			if (iSlow8) {
				fRec38[1] = fRec38[0];
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
		iTempPerm9 = iTemp9;
		iTempPerm10 = iTemp10;
		fTempPerm11 = fTemp11;
		fTempPerm12 = fTemp12;
		fTempPerm13 = fTemp13;
		fTempPerm14 = fTemp14;
		fTempPerm15 = fTemp15;
		fTempPerm16 = fTemp16;
		iTempPerm17 = iTemp17;
		iTempPerm18 = iTemp18;
		fTempPerm19 = fTemp19;
		fTempPerm20 = fTemp20;
		iTempPerm21 = iTemp21;
		iTempPerm22 = iTemp22;
		fTempPerm23 = fTemp23;
		fTempPerm24 = fTemp24;
		fTempPerm25 = fTemp25;
		iTempPerm26 = iTemp26;
		iTempPerm27 = iTemp27;
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
		fTempPerm58 = fTemp58;
		fTempPerm59 = fTemp59;
		fTempPerm60 = fTemp60;
		fTempPerm61 = fTemp61;
		fTempPerm62 = fTemp62;
		fTempPerm63 = fTemp63;
		fTempPerm64 = fTemp64;
		fTempPerm65 = fTemp65;
		fTempPerm66 = fTemp66;
		fTempPerm67 = fTemp67;
		fTempPerm68 = fTemp68;
		fTempPerm69 = fTemp69;
		fTempPerm70 = fTemp70;
		fTempPerm71 = fTemp71;
		fTempPerm72 = fTemp72;
		fTempPerm73 = fTemp73;
		fTempPerm74 = fTemp74;
		fTempPerm75 = fTemp75;
		fTempPerm76 = fTemp76;
		fTempPerm77 = fTemp77;
		fTempPerm78 = fTemp78;
		fTempPerm79 = fTemp79;
		fTempPerm80 = fTemp80;
		fTempPerm81 = fTemp81;
		fTempPerm82 = fTemp82;
		fTempPerm83 = fTemp83;
		fTempPerm84 = fTemp84;
		fTempPerm85 = fTemp85;
		fTempPerm86 = fTemp86;
		fTempPerm87 = fTemp87;
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
