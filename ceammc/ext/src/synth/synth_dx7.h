/* ------------------------------------------------------------
name: "synth.dx7"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_dx7 -scn synth_dx7_dsp -es 1 -mcd 16 -single -ftz 0
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
    
        virtual synth_dx7_dsp* createDSPInstance() = 0;
    
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
	
	int iVec0[2];
	int iRec3[2];
	
  public:
	
	int getNumInputssynth_dx7SIG0() {
		return 0;
	}
	int getNumOutputssynth_dx7SIG0() {
		return 1;
	}
	
	void instanceInitsynth_dx7SIG0(int sample_rate) {
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iVec0[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec3[l4] = 0;
		}
	}
	
	void fillsynth_dx7SIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec3[0] = (iVec0[1] + iRec3[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec3[0]));
			iVec0[1] = iVec0[0];
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
	int iTempPerm1;
	float fConst1;
	float fRec1[2];
	float fTempPerm2;
	float fTempPerm3;
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
	float fRec6[2];
	FAUSTFLOAT fEntry24;
	FAUSTFLOAT fEntry25;
	FAUSTFLOAT fEntry26;
	FAUSTFLOAT fEntry27;
	FAUSTFLOAT fEntry28;
	float fRec8[2];
	int iTempPerm4;
	int iTempPerm5;
	float fRec7[2];
	float fTempPerm6;
	float fTempPerm7;
	float fRec9[2];
	float fTempPerm8;
	FAUSTFLOAT fEntry29;
	float fRec5[2];
	float fTempPerm9;
	FAUSTFLOAT fEntry30;
	FAUSTFLOAT fEntry31;
	FAUSTFLOAT fEntry32;
	FAUSTFLOAT fEntry33;
	FAUSTFLOAT fEntry34;
	FAUSTFLOAT fEntry35;
	FAUSTFLOAT fEntry36;
	FAUSTFLOAT fEntry37;
	FAUSTFLOAT fEntry38;
	float fRec10[2];
	FAUSTFLOAT fEntry39;
	FAUSTFLOAT fEntry40;
	FAUSTFLOAT fEntry41;
	FAUSTFLOAT fEntry42;
	FAUSTFLOAT fEntry43;
	float fRec12[2];
	int iTempPerm10;
	int iTempPerm11;
	float fRec11[2];
	float fTempPerm12;
	float fTempPerm13;
	float fRec13[2];
	FAUSTFLOAT fEntry44;
	FAUSTFLOAT fEntry45;
	FAUSTFLOAT fEntry46;
	FAUSTFLOAT fEntry47;
	FAUSTFLOAT fEntry48;
	FAUSTFLOAT fEntry49;
	FAUSTFLOAT fEntry50;
	FAUSTFLOAT fEntry51;
	FAUSTFLOAT fEntry52;
	float fRec14[2];
	FAUSTFLOAT fEntry53;
	FAUSTFLOAT fEntry54;
	FAUSTFLOAT fEntry55;
	FAUSTFLOAT fEntry56;
	FAUSTFLOAT fEntry57;
	float fRec16[2];
	int iTempPerm14;
	int iTempPerm15;
	float fRec15[2];
	float fTempPerm16;
	float fTempPerm17;
	float fRec17[2];
	FAUSTFLOAT fEntry58;
	FAUSTFLOAT fEntry59;
	FAUSTFLOAT fEntry60;
	FAUSTFLOAT fEntry61;
	FAUSTFLOAT fEntry62;
	FAUSTFLOAT fEntry63;
	FAUSTFLOAT fEntry64;
	FAUSTFLOAT fEntry65;
	FAUSTFLOAT fEntry66;
	float fRec18[2];
	FAUSTFLOAT fEntry67;
	FAUSTFLOAT fEntry68;
	FAUSTFLOAT fEntry69;
	FAUSTFLOAT fEntry70;
	FAUSTFLOAT fEntry71;
	float fRec20[2];
	int iTempPerm18;
	int iTempPerm19;
	float fRec19[2];
	float fTempPerm20;
	float fTempPerm21;
	float fRec21[2];
	float fTempPerm22;
	float fTempPerm23;
	float fTempPerm24;
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
	int iTempPerm25;
	int iTempPerm26;
	float fRec23[2];
	float fTempPerm27;
	float fTempPerm28;
	float fRec25[2];
	float fTempPerm29;
	float fTempPerm30;
	float fTempPerm31;
	float fTempPerm32;
	float fTempPerm33;
	float fRec26[2];
	float fTempPerm34;
	float fRec27[2];
	float fTempPerm35;
	float fTempPerm36;
	float fRec28[2];
	float fTempPerm37;
	float fTempPerm38;
	float fTempPerm39;
	float fRec29[2];
	float fTempPerm40;
	float fRec30[2];
	float fTempPerm41;
	float fTempPerm42;
	float fTempPerm43;
	float fTempPerm44;
	float fRec31[2];
	float fTempPerm45;
	float fTempPerm46;
	float fRec32[2];
	float fTempPerm47;
	float fTempPerm48;
	float fTempPerm49;
	float fRec33[2];
	float fTempPerm50;
	float fTempPerm51;
	float fTempPerm52;
	float fTempPerm53;
	float fTempPerm54;
	float fTempPerm55;
	float fTempPerm56;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_dx7 -scn synth_dx7_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/dx7envelope:author", "Romain Michon");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.2");
		m->declare("filename", "synth_dx7.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "synth.dx7");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.3");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
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
		fEntry29 = FAUSTFLOAT(0.0f);
		fEntry30 = FAUSTFLOAT(95.0f);
		fEntry31 = FAUSTFLOAT(9e+01f);
		fEntry32 = FAUSTFLOAT(1.0f);
		fEntry33 = FAUSTFLOAT(0.0f);
		fEntry34 = FAUSTFLOAT(9e+01f);
		fEntry35 = FAUSTFLOAT(0.0f);
		fEntry36 = FAUSTFLOAT(1.0f);
		fEntry37 = FAUSTFLOAT(0.0f);
		fEntry38 = FAUSTFLOAT(1.0f);
		fEntry39 = FAUSTFLOAT(0.0f);
		fEntry40 = FAUSTFLOAT(9e+01f);
		fEntry41 = FAUSTFLOAT(9e+01f);
		fEntry42 = FAUSTFLOAT(9e+01f);
		fEntry43 = FAUSTFLOAT(9e+01f);
		fEntry44 = FAUSTFLOAT(95.0f);
		fEntry45 = FAUSTFLOAT(9e+01f);
		fEntry46 = FAUSTFLOAT(1.0f);
		fEntry47 = FAUSTFLOAT(0.0f);
		fEntry48 = FAUSTFLOAT(9e+01f);
		fEntry49 = FAUSTFLOAT(0.0f);
		fEntry50 = FAUSTFLOAT(1.0f);
		fEntry51 = FAUSTFLOAT(0.0f);
		fEntry52 = FAUSTFLOAT(1.0f);
		fEntry53 = FAUSTFLOAT(0.0f);
		fEntry54 = FAUSTFLOAT(9e+01f);
		fEntry55 = FAUSTFLOAT(9e+01f);
		fEntry56 = FAUSTFLOAT(9e+01f);
		fEntry57 = FAUSTFLOAT(9e+01f);
		fEntry58 = FAUSTFLOAT(95.0f);
		fEntry59 = FAUSTFLOAT(9e+01f);
		fEntry60 = FAUSTFLOAT(1.0f);
		fEntry61 = FAUSTFLOAT(0.0f);
		fEntry62 = FAUSTFLOAT(9e+01f);
		fEntry63 = FAUSTFLOAT(0.0f);
		fEntry64 = FAUSTFLOAT(1.0f);
		fEntry65 = FAUSTFLOAT(0.0f);
		fEntry66 = FAUSTFLOAT(1.0f);
		fEntry67 = FAUSTFLOAT(0.0f);
		fEntry68 = FAUSTFLOAT(9e+01f);
		fEntry69 = FAUSTFLOAT(9e+01f);
		fEntry70 = FAUSTFLOAT(9e+01f);
		fEntry71 = FAUSTFLOAT(9e+01f);
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
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec2[l1] = 0.0f;
		}
		iTempPerm0 = 0;
		iTempPerm1 = 0;
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		fTempPerm2 = 0.0f;
		fTempPerm3 = 0.0f;
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec6[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec8[l7] = 0.0f;
		}
		iTempPerm4 = 0;
		iTempPerm5 = 0;
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec7[l8] = 0.0f;
		}
		fTempPerm6 = 0.0f;
		fTempPerm7 = 0.0f;
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec9[l9] = 0.0f;
		}
		fTempPerm8 = 0.0f;
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec5[l10] = 0.0f;
		}
		fTempPerm9 = 0.0f;
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec10[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec12[l12] = 0.0f;
		}
		iTempPerm10 = 0;
		iTempPerm11 = 0;
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec11[l13] = 0.0f;
		}
		fTempPerm12 = 0.0f;
		fTempPerm13 = 0.0f;
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec13[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec14[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec16[l16] = 0.0f;
		}
		iTempPerm14 = 0;
		iTempPerm15 = 0;
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec15[l17] = 0.0f;
		}
		fTempPerm16 = 0.0f;
		fTempPerm17 = 0.0f;
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec20[l20] = 0.0f;
		}
		iTempPerm18 = 0;
		iTempPerm19 = 0;
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec19[l21] = 0.0f;
		}
		fTempPerm20 = 0.0f;
		fTempPerm21 = 0.0f;
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec21[l22] = 0.0f;
		}
		fTempPerm22 = 0.0f;
		fTempPerm23 = 0.0f;
		fTempPerm24 = 0.0f;
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec22[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec24[l24] = 0.0f;
		}
		iTempPerm25 = 0;
		iTempPerm26 = 0;
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec23[l25] = 0.0f;
		}
		fTempPerm27 = 0.0f;
		fTempPerm28 = 0.0f;
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec25[l26] = 0.0f;
		}
		fTempPerm29 = 0.0f;
		fTempPerm30 = 0.0f;
		fTempPerm31 = 0.0f;
		fTempPerm32 = 0.0f;
		fTempPerm33 = 0.0f;
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec26[l27] = 0.0f;
		}
		fTempPerm34 = 0.0f;
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec27[l28] = 0.0f;
		}
		fTempPerm35 = 0.0f;
		fTempPerm36 = 0.0f;
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec28[l29] = 0.0f;
		}
		fTempPerm37 = 0.0f;
		fTempPerm38 = 0.0f;
		fTempPerm39 = 0.0f;
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec29[l30] = 0.0f;
		}
		fTempPerm40 = 0.0f;
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec30[l31] = 0.0f;
		}
		fTempPerm41 = 0.0f;
		fTempPerm42 = 0.0f;
		fTempPerm43 = 0.0f;
		fTempPerm44 = 0.0f;
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec31[l32] = 0.0f;
		}
		fTempPerm45 = 0.0f;
		fTempPerm46 = 0.0f;
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec32[l33] = 0.0f;
		}
		fTempPerm47 = 0.0f;
		fTempPerm48 = 0.0f;
		fTempPerm49 = 0.0f;
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec33[l34] = 0.0f;
		}
		fTempPerm50 = 0.0f;
		fTempPerm51 = 0.0f;
		fTempPerm52 = 0.0f;
		fTempPerm53 = 0.0f;
		fTempPerm54 = 0.0f;
		fTempPerm55 = 0.0f;
		fTempPerm56 = 0.0f;
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
		ui_interface->addNumEntry("feedback", &fEntry29, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
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
		ui_interface->addNumEntry("op2:detune", &fEntry36, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egL1", &fEntry39, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egL2", &fEntry41, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egL3", &fEntry31, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egL4", &fEntry33, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egR1", &fEntry40, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egR2", &fEntry42, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egR3", &fEntry43, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:egR4", &fEntry34, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry38, "unit", "Hz");
		ui_interface->addNumEntry("op2:freq", &fEntry38, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op2:key_vel_sens", &fEntry32, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:level", &fEntry30, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:mode", &fEntry37, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op2:rate_scale", &fEntry35, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:detune", &fEntry78, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egL1", &fEntry81, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egL2", &fEntry83, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egL3", &fEntry73, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egL4", &fEntry75, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egR1", &fEntry82, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egR2", &fEntry84, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egR3", &fEntry85, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:egR4", &fEntry76, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry80, "unit", "Hz");
		ui_interface->addNumEntry("op3:freq", &fEntry80, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op3:key_vel_sens", &fEntry74, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:level", &fEntry72, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:mode", &fEntry79, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op3:rate_scale", &fEntry77, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:detune", &fEntry50, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egL1", &fEntry53, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egL2", &fEntry55, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egL3", &fEntry45, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egL4", &fEntry47, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egR1", &fEntry54, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egR2", &fEntry56, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egR3", &fEntry57, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:egR4", &fEntry48, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry52, "unit", "Hz");
		ui_interface->addNumEntry("op4:freq", &fEntry52, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op4:key_vel_sens", &fEntry46, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:level", &fEntry44, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:mode", &fEntry51, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op4:rate_scale", &fEntry49, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:detune", &fEntry64, FAUSTFLOAT(1.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egL1", &fEntry67, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egL2", &fEntry69, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egL3", &fEntry59, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egL4", &fEntry61, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egR1", &fEntry68, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egR2", &fEntry70, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egR3", &fEntry71, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:egR4", &fEntry62, FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry66, "unit", "Hz");
		ui_interface->addNumEntry("op5:freq", &fEntry66, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addNumEntry("op5:key_vel_sens", &fEntry60, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:level", &fEntry58, FAUSTFLOAT(95.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(99.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:mode", &fEntry65, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addNumEntry("op5:rate_scale", &fEntry63, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("pitch", &fHslider1, FAUSTFLOAT(48.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(84.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = int(float(fEntry0));
		int iSlow1 = float(iSlow0 == 8) != 0.0f;
		int iSlow2 = float(iSlow0 == 7) != 0.0f;
		int iSlow3 = float(iSlow0 == 6) != 0.0f;
		int iSlow4 = float(iSlow0 == 5) != 0.0f;
		int iSlow5 = float(iSlow0 == 4) != 0.0f;
		int iSlow6 = float(iSlow0 == 3) != 0.0f;
		int iSlow7 = float(iSlow0 == 2) != 0.0f;
		int iSlow8 = float(iSlow0 == 1) != 0.0f;
		int iSlow9 = float(iSlow0 == 0) != 0.0f;
		int iSlow10 = float(iSlow0 == 9) != 0.0f;
		int iSlow11 = float(iSlow0 == 10) != 0.0f;
		int iSlow12 = float(iSlow0 == 11) != 0.0f;
		int iSlow13 = float(iSlow0 == 12) != 0.0f;
		int iSlow14 = float(iSlow0 == 13) != 0.0f;
		int iSlow15 = float(iSlow0 == 14) != 0.0f;
		int iSlow16 = float(iSlow0 == 15) != 0.0f;
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
		float fSlow62 = 0.0f - fSlow61 * (fSlow49 - fSlow21);
		float fSlow63 = 0.0f - fConst0 * (fSlow55 - fSlow58);
		float fSlow64 = fConst0 * fSlow48;
		int iTemp1 = iTempPerm1;
		float fSlow65 = 0.0f - fSlow61 * (fSlow42 - fSlow49);
		float fSlow66 = 0.0f - fConst0 * (fSlow48 - fSlow55);
		float fSlow67 = fConst1 * (fSlow42 - fSlow26);
		float fTemp2 = fTempPerm2;
		float fSlow68 = fConst1 * fSlow31;
		float fTemp3 = fTempPerm3;
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
		float fSlow100 = fSlow93 + std::max<float>(0.001f, std::fabs(0.0f - ((iSlow96) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow97, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow97, 12.6f))) * (((iSlow96) ? fSlow92 : fSlow91) - ((iSlow96) ? fSlow99 : fSlow98))));
		int iSlow101 = fSlow73 > fSlow95;
		float fSlow102 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry28) + fSlow79));
		float fSlow103 = fSlow100 + std::max<float>(0.001f, std::fabs(((iSlow101) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow102, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow102, 12.6f))) * (((iSlow101) ? fSlow99 : fSlow98) - ((iSlow101) ? fSlow82 : fSlow81))));
		float fSlow104 = fConst0 * fSlow103;
		float fSlow105 = fConst0 * fSlow100;
		int iTemp4 = iTempPerm4;
		float fSlow106 = fSlow69 * fSlow72;
		float fSlow107 = fSlow70 - fSlow94;
		float fSlow108 = 0.0f - fConst0 * (fSlow100 - fSlow103);
		float fSlow109 = fConst0 * fSlow93;
		int iTemp5 = iTempPerm5;
		float fSlow110 = fConst1 * (0.0f - fSlow106 * (fSlow87 - fSlow94));
		float fSlow111 = fSlow100 - fSlow93;
		float fSlow112 = fConst1 * (0.0f - fSlow106 * (fSlow74 - fSlow87));
		float fTemp6 = fTempPerm6;
		float fSlow113 = fConst1 * fSlow78;
		float fTemp7 = fTempPerm7;
		float fTemp8 = fTempPerm8;
		float fSlow114 = float(fEntry29);
		float fSlow115 = 3650.6323f * fSlow114;
		float fTemp9 = fTempPerm9;
		float fSlow116 = float(fEntry30);
		float fSlow117 = float(fEntry31);
		float fSlow118 = float(fEntry32);
		float fSlow119 = 0.0012626263f * fSlow22 * fSlow118 + 0.01010101f * (1.0f - 0.125f * fSlow118);
		float fSlow120 = fSlow116 * fSlow117 * fSlow119;
		float fSlow121 = float(fEntry33);
		float fSlow122 = fSlow116 * fSlow121 * fSlow119;
		int iSlow123 = fSlow120 <= fSlow122;
		float fSlow124 = float(fEntry38);
		float fSlow125 = 0.25f * float(fEntry36) + ((int(float(fEntry37))) ? fSlow124 : 4.4e+02f * fSlow29 * fSlow124);
		float fSlow126 = 0.057142857f * float(fEntry35) * (17.31234f * std::log(0.0022727272f * fSlow125) + 48.0f);
		float fSlow127 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry34) + fSlow126));
		float fSlow128 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow120), 0.7f);
		float fSlow129 = std::pow(0.014705882f * float(fSlow120 >= 3e+01f) * std::min<float>(fSlow120 + -3e+01f, 68.0f), 1.8f);
		float fSlow130 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow122), 0.7f);
		float fSlow131 = std::pow(0.014705882f * float(fSlow122 >= 3e+01f) * std::min<float>(fSlow122 + -3e+01f, 68.0f), 1.8f);
		float fSlow132 = std::max<float>(0.001f, std::fabs(((iSlow123) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow127, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow127, 12.6f))) * (((iSlow123) ? fSlow129 : fSlow128) - ((iSlow123) ? fSlow131 : fSlow130))));
		float fSlow133 = fConst0 * fSlow132;
		float fSlow134 = float(fEntry39);
		float fSlow135 = fSlow116 * fSlow134 * fSlow119;
		int iSlow136 = fSlow135 > fSlow122;
		float fSlow137 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry40) + fSlow126));
		float fSlow138 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow135), 0.7f);
		float fSlow139 = std::pow(0.014705882f * float(fSlow135 >= 3e+01f) * std::min<float>(fSlow135 + -3e+01f, 68.0f), 1.8f);
		float fSlow140 = std::max<float>(0.001f, std::fabs(((iSlow136) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow137, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow137, 12.6f))) * (((iSlow136) ? fSlow131 : fSlow130) - ((iSlow136) ? fSlow139 : fSlow138))));
		float fSlow141 = float(fEntry41);
		float fSlow142 = fSlow116 * fSlow141 * fSlow119;
		int iSlow143 = fSlow142 > fSlow135;
		float fSlow144 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry42) + fSlow126));
		float fSlow145 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow142), 0.7f);
		float fSlow146 = std::pow(0.014705882f * float(fSlow142 >= 3e+01f) * std::min<float>(fSlow142 + -3e+01f, 68.0f), 1.8f);
		float fSlow147 = fSlow140 + std::max<float>(0.001f, std::fabs(((iSlow143) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow144, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow144, 12.6f))) * (((iSlow143) ? fSlow146 : fSlow145) - ((iSlow143) ? fSlow139 : fSlow138))));
		int iSlow148 = fSlow120 > fSlow142;
		float fSlow149 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry43) + fSlow126));
		float fSlow150 = fSlow147 + std::max<float>(0.001f, std::fabs(((iSlow148) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow149, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow149, 12.6f))) * (((iSlow148) ? fSlow146 : fSlow145) - ((iSlow148) ? fSlow129 : fSlow128))));
		float fSlow151 = fConst0 * fSlow150;
		float fSlow152 = fConst0 * fSlow147;
		int iTemp10 = iTempPerm10;
		float fSlow153 = fSlow116 * fSlow119;
		float fSlow154 = fSlow117 - fSlow141;
		float fSlow155 = 0.0f - fConst0 * (fSlow147 - fSlow150);
		float fSlow156 = fConst0 * fSlow140;
		int iTemp11 = iTempPerm11;
		float fSlow157 = 0.0f - fSlow153 * (fSlow134 - fSlow141);
		float fSlow158 = 0.0f - fConst0 * (fSlow140 - fSlow147);
		float fSlow159 = fConst1 * (0.0f - fSlow153 * (fSlow121 - fSlow134));
		float fTemp12 = fTempPerm12;
		float fSlow160 = fConst1 * fSlow125;
		float fTemp13 = fTempPerm13;
		float fSlow161 = float(fEntry44);
		float fSlow162 = float(fEntry45);
		float fSlow163 = float(fEntry46);
		float fSlow164 = 0.0012626263f * fSlow22 * fSlow163 + 0.01010101f * (1.0f - 0.125f * fSlow163);
		float fSlow165 = fSlow161 * fSlow162 * fSlow164;
		float fSlow166 = float(fEntry47);
		float fSlow167 = fSlow161 * fSlow166 * fSlow164;
		int iSlow168 = fSlow165 <= fSlow167;
		float fSlow169 = float(fEntry52);
		float fSlow170 = 0.25f * float(fEntry50) + ((int(float(fEntry51))) ? fSlow169 : 4.4e+02f * fSlow29 * fSlow169);
		float fSlow171 = 0.057142857f * float(fEntry49) * (17.31234f * std::log(0.0022727272f * fSlow170) + 48.0f);
		float fSlow172 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry48) + fSlow171));
		float fSlow173 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow165), 0.7f);
		float fSlow174 = std::pow(0.014705882f * float(fSlow165 >= 3e+01f) * std::min<float>(fSlow165 + -3e+01f, 68.0f), 1.8f);
		float fSlow175 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow167), 0.7f);
		float fSlow176 = std::pow(0.014705882f * float(fSlow167 >= 3e+01f) * std::min<float>(fSlow167 + -3e+01f, 68.0f), 1.8f);
		float fSlow177 = std::max<float>(0.001f, std::fabs(((iSlow168) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow172, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow172, 12.6f))) * (((iSlow168) ? fSlow174 : fSlow173) - ((iSlow168) ? fSlow176 : fSlow175))));
		float fSlow178 = fConst0 * fSlow177;
		float fSlow179 = float(fEntry53);
		float fSlow180 = fSlow161 * fSlow179 * fSlow164;
		int iSlow181 = fSlow180 > fSlow167;
		float fSlow182 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry54) + fSlow171));
		float fSlow183 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow180), 0.7f);
		float fSlow184 = std::pow(0.014705882f * float(fSlow180 >= 3e+01f) * std::min<float>(fSlow180 + -3e+01f, 68.0f), 1.8f);
		float fSlow185 = std::max<float>(0.001f, std::fabs(((iSlow181) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow182, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow182, 12.6f))) * (((iSlow181) ? fSlow176 : fSlow175) - ((iSlow181) ? fSlow184 : fSlow183))));
		float fSlow186 = float(fEntry55);
		float fSlow187 = fSlow161 * fSlow186 * fSlow164;
		int iSlow188 = fSlow187 > fSlow180;
		float fSlow189 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry56) + fSlow171));
		float fSlow190 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow187), 0.7f);
		float fSlow191 = std::pow(0.014705882f * float(fSlow187 >= 3e+01f) * std::min<float>(fSlow187 + -3e+01f, 68.0f), 1.8f);
		float fSlow192 = fSlow185 + std::max<float>(0.001f, std::fabs(((iSlow188) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow189, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow189, 12.6f))) * (((iSlow188) ? fSlow191 : fSlow190) - ((iSlow188) ? fSlow184 : fSlow183))));
		int iSlow193 = fSlow165 > fSlow187;
		float fSlow194 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry57) + fSlow171));
		float fSlow195 = fSlow192 + std::max<float>(0.001f, std::fabs(((iSlow193) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow194, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow194, 12.6f))) * (((iSlow193) ? fSlow191 : fSlow190) - ((iSlow193) ? fSlow174 : fSlow173))));
		float fSlow196 = fConst0 * fSlow195;
		float fSlow197 = fConst0 * fSlow192;
		int iTemp14 = iTempPerm14;
		float fSlow198 = fSlow161 * fSlow164;
		float fSlow199 = fSlow162 - fSlow186;
		float fSlow200 = 0.0f - fConst0 * (fSlow192 - fSlow195);
		float fSlow201 = fConst0 * fSlow185;
		int iTemp15 = iTempPerm15;
		float fSlow202 = fConst1 * (0.0f - fSlow198 * (fSlow179 - fSlow186));
		float fSlow203 = fSlow192 - fSlow185;
		float fSlow204 = fConst1 * (0.0f - fSlow198 * (fSlow166 - fSlow179));
		float fTemp16 = fTempPerm16;
		float fSlow205 = fConst1 * fSlow170;
		float fTemp17 = fTempPerm17;
		float fSlow206 = float(fEntry58);
		float fSlow207 = float(fEntry59);
		float fSlow208 = float(fEntry60);
		float fSlow209 = 0.0012626263f * fSlow22 * fSlow208 + 0.01010101f * (1.0f - 0.125f * fSlow208);
		float fSlow210 = fSlow206 * fSlow207 * fSlow209;
		float fSlow211 = float(fEntry61);
		float fSlow212 = fSlow206 * fSlow211 * fSlow209;
		int iSlow213 = fSlow210 <= fSlow212;
		float fSlow214 = float(fEntry66);
		float fSlow215 = 0.25f * float(fEntry64) + ((int(float(fEntry65))) ? fSlow214 : 4.4e+02f * fSlow29 * fSlow214);
		float fSlow216 = 0.057142857f * float(fEntry63) * (17.31234f * std::log(0.0022727272f * fSlow215) + 48.0f);
		float fSlow217 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry62) + fSlow216));
		float fSlow218 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow210), 0.7f);
		float fSlow219 = std::pow(0.014705882f * float(fSlow210 >= 3e+01f) * std::min<float>(fSlow210 + -3e+01f, 68.0f), 1.8f);
		float fSlow220 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow212), 0.7f);
		float fSlow221 = std::pow(0.014705882f * float(fSlow212 >= 3e+01f) * std::min<float>(fSlow212 + -3e+01f, 68.0f), 1.8f);
		float fSlow222 = std::max<float>(0.001f, std::fabs(((iSlow213) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow217, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow217, 12.6f))) * (((iSlow213) ? fSlow219 : fSlow218) - ((iSlow213) ? fSlow221 : fSlow220))));
		float fSlow223 = fConst0 * fSlow222;
		float fSlow224 = float(fEntry67);
		float fSlow225 = fSlow206 * fSlow224 * fSlow209;
		int iSlow226 = fSlow225 > fSlow212;
		float fSlow227 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry68) + fSlow216));
		float fSlow228 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow225), 0.7f);
		float fSlow229 = std::pow(0.014705882f * float(fSlow225 >= 3e+01f) * std::min<float>(fSlow225 + -3e+01f, 68.0f), 1.8f);
		float fSlow230 = std::max<float>(0.001f, std::fabs(((iSlow226) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow227, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow227, 12.6f))) * (((iSlow226) ? fSlow221 : fSlow220) - ((iSlow226) ? fSlow229 : fSlow228))));
		float fSlow231 = float(fEntry69);
		float fSlow232 = fSlow206 * fSlow231 * fSlow209;
		int iSlow233 = fSlow232 > fSlow225;
		float fSlow234 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry70) + fSlow216));
		float fSlow235 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow232), 0.7f);
		float fSlow236 = std::pow(0.014705882f * float(fSlow232 >= 3e+01f) * std::min<float>(fSlow232 + -3e+01f, 68.0f), 1.8f);
		float fSlow237 = fSlow230 + std::max<float>(0.001f, std::fabs(((iSlow233) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow234, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow234, 12.6f))) * (((iSlow233) ? fSlow236 : fSlow235) - ((iSlow233) ? fSlow229 : fSlow228))));
		int iSlow238 = fSlow210 > fSlow232;
		float fSlow239 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry71) + fSlow216));
		float fSlow240 = fSlow237 + std::max<float>(0.001f, std::fabs(((iSlow238) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow239, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow239, 12.6f))) * (((iSlow238) ? fSlow236 : fSlow235) - ((iSlow238) ? fSlow219 : fSlow218))));
		float fSlow241 = fConst0 * fSlow240;
		float fSlow242 = fConst0 * fSlow237;
		int iTemp18 = iTempPerm18;
		float fSlow243 = fSlow206 * fSlow209;
		float fSlow244 = fSlow207 - fSlow231;
		float fSlow245 = 0.0f - fConst0 * (fSlow237 - fSlow240);
		float fSlow246 = fConst0 * fSlow230;
		int iTemp19 = iTempPerm19;
		float fSlow247 = 0.0f - fSlow243 * (fSlow224 - fSlow231);
		float fSlow248 = 0.0f - fConst0 * (fSlow230 - fSlow237);
		float fSlow249 = fConst1 * (0.0f - fSlow243 * (fSlow211 - fSlow224));
		float fTemp20 = fTempPerm20;
		float fSlow250 = fConst1 * fSlow215;
		float fTemp21 = fTempPerm21;
		float fTemp22 = fTempPerm22;
		float fTemp23 = fTempPerm23;
		float fTemp24 = fTempPerm24;
		float fSlow251 = float(fEntry72);
		float fSlow252 = float(fEntry73);
		float fSlow253 = float(fEntry74);
		float fSlow254 = 0.0012626263f * fSlow22 * fSlow253 + 0.01010101f * (1.0f - 0.125f * fSlow253);
		float fSlow255 = fSlow251 * fSlow252 * fSlow254;
		float fSlow256 = float(fEntry75);
		float fSlow257 = fSlow251 * fSlow256 * fSlow254;
		int iSlow258 = fSlow255 <= fSlow257;
		float fSlow259 = float(fEntry80);
		float fSlow260 = 0.25f * float(fEntry78) + ((int(float(fEntry79))) ? fSlow259 : 4.4e+02f * fSlow29 * fSlow259);
		float fSlow261 = 0.057142857f * float(fEntry77) * (17.31234f * std::log(0.0022727272f * fSlow260) + 48.0f);
		float fSlow262 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry76) + fSlow261));
		float fSlow263 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow255), 0.7f);
		float fSlow264 = std::pow(0.014705882f * float(fSlow255 >= 3e+01f) * std::min<float>(fSlow255 + -3e+01f, 68.0f), 1.8f);
		float fSlow265 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow257), 0.7f);
		float fSlow266 = std::pow(0.014705882f * float(fSlow257 >= 3e+01f) * std::min<float>(fSlow257 + -3e+01f, 68.0f), 1.8f);
		float fSlow267 = std::max<float>(0.001f, std::fabs(((iSlow258) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow262, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow262, 12.6f))) * (((iSlow258) ? fSlow264 : fSlow263) - ((iSlow258) ? fSlow266 : fSlow265))));
		float fSlow268 = fConst0 * fSlow267;
		float fSlow269 = float(fEntry81);
		float fSlow270 = fSlow251 * fSlow269 * fSlow254;
		int iSlow271 = fSlow270 > fSlow257;
		float fSlow272 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry82) + fSlow261));
		float fSlow273 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow270), 0.7f);
		float fSlow274 = std::pow(0.014705882f * float(fSlow270 >= 3e+01f) * std::min<float>(fSlow270 + -3e+01f, 68.0f), 1.8f);
		float fSlow275 = std::max<float>(0.001f, std::fabs(((iSlow271) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow272, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow272, 12.6f))) * (((iSlow271) ? fSlow266 : fSlow265) - ((iSlow271) ? fSlow274 : fSlow273))));
		float fSlow276 = float(fEntry83);
		float fSlow277 = fSlow251 * fSlow276 * fSlow254;
		int iSlow278 = fSlow277 > fSlow270;
		float fSlow279 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry84) + fSlow261));
		float fSlow280 = std::pow(0.01010101f * std::min<float>(99.0f, fSlow277), 0.7f);
		float fSlow281 = std::pow(0.014705882f * float(fSlow277 >= 3e+01f) * std::min<float>(fSlow277 + -3e+01f, 68.0f), 1.8f);
		float fSlow282 = fSlow275 + std::max<float>(0.001f, std::fabs(((iSlow278) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow279, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow279, 12.6f))) * (((iSlow278) ? fSlow281 : fSlow280) - ((iSlow278) ? fSlow274 : fSlow273))));
		int iSlow283 = fSlow255 > fSlow277;
		float fSlow284 = 0.007874016f * (127.0f - std::min<float>(99.0f, float(fEntry85) + fSlow261));
		float fSlow285 = fSlow282 + std::max<float>(0.001f, std::fabs(((iSlow283) ? std::max<float>(0.003f, 38.0f * std::pow(fSlow284, 12.0f)) : std::max<float>(0.008f, 318.0f * std::pow(fSlow284, 12.6f))) * (((iSlow283) ? fSlow281 : fSlow280) - ((iSlow283) ? fSlow264 : fSlow263))));
		float fSlow286 = fConst0 * fSlow285;
		float fSlow287 = fConst0 * fSlow282;
		int iTemp25 = iTempPerm25;
		float fSlow288 = fSlow251 * fSlow254;
		float fSlow289 = fSlow252 - fSlow276;
		float fSlow290 = 0.0f - fConst0 * (fSlow282 - fSlow285);
		float fSlow291 = fConst0 * fSlow275;
		int iTemp26 = iTempPerm26;
		float fSlow292 = 0.0f - fSlow288 * (fSlow269 - fSlow276);
		float fSlow293 = 0.0f - fConst0 * (fSlow275 - fSlow282);
		float fSlow294 = fConst1 * (0.0f - fSlow288 * (fSlow256 - fSlow269));
		float fTemp27 = fTempPerm27;
		float fSlow295 = fConst1 * fSlow260;
		float fTemp28 = fTempPerm28;
		float fTemp29 = fTempPerm29;
		float fTemp30 = fTempPerm30;
		float fTemp31 = fTempPerm31;
		float fTemp32 = fTempPerm32;
		float fTemp33 = fTempPerm33;
		float fSlow296 = 912.6581f * fSlow114;
		float fTemp34 = fTempPerm34;
		float fSlow297 = 9126.581f * fSlow114;
		float fTemp35 = fTempPerm35;
		float fTemp36 = fTempPerm36;
		float fTemp37 = fTempPerm37;
		float fTemp38 = fTempPerm38;
		float fTemp39 = fTempPerm39;
		float fTemp40 = fTempPerm40;
		float fTemp41 = fTempPerm41;
		float fTemp42 = fTempPerm42;
		float fTemp43 = fTempPerm43;
		float fTemp44 = fTempPerm44;
		float fSlow298 = 1825.3162f * fSlow114;
		float fTemp45 = fTempPerm45;
		float fTemp46 = fTempPerm46;
		float fTemp47 = fTempPerm47;
		float fTemp48 = fTempPerm48;
		float fTemp49 = fTempPerm49;
		float fTemp50 = fTempPerm50;
		float fTemp51 = fTempPerm51;
		float fTemp52 = fTempPerm52;
		float fTemp53 = fTempPerm53;
		float fTemp54 = fTempPerm54;
		float fTemp55 = fTempPerm55;
		float fTemp56 = fTempPerm56;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			if ((iSlow1 | (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16)))))))))))))))) {
				fRec0[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow39, fRec0[1] + 1.0f));
				fRec2[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow59, fRec2[1] + 1.0f));
				iTemp0 = fRec2[0] < fSlow60;
				iTemp1 = fRec2[0] < fSlow64;
				fRec1[0] = ((iSlow18) ? fSlow40 * ((iTemp0) ? ((iTemp1) ? ((fRec2[0] < 0.0f) ? fSlow27 : ((iTemp1) ? fSlow61 * (fSlow26 + fSlow67 * (fRec2[0] / fSlow48)) : fSlow43)) : ((iTemp0) ? fSlow43 + fSlow65 * ((fRec2[0] - fSlow64) / fSlow66) : fSlow50)) : ((fRec2[0] < fSlow59) ? fSlow50 + fSlow62 * ((fRec2[0] - fSlow60) / fSlow63) : fSlow25)) : fRec1[1]);
				fTemp2 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec0[0] < 0.0f) ? fRec1[0] : ((fRec0[0] < fSlow39) ? fRec1[0] + fConst1 * (fRec0[0] * (fSlow27 - fRec1[0]) / fSlow38) : fSlow27))));
				fTemp3 = fRec4[1] + fSlow68;
				fRec4[0] = fTemp3 - std::floor(fTemp3);
				fRec6[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow86, fRec6[1] + 1.0f));
				fRec8[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow104, fRec8[1] + 1.0f));
				iTemp4 = fRec8[0] < fSlow105;
				iTemp5 = fRec8[0] < fSlow109;
				fRec7[0] = ((iSlow18) ? fSlow40 * ((iTemp4) ? ((iTemp5) ? ((fRec8[0] < 0.0f) ? fSlow75 : ((iTemp5) ? fSlow75 + fSlow112 * (fRec8[0] / fSlow93) : fSlow88)) : ((iTemp4) ? fSlow88 + fSlow110 * ((fRec8[0] - fSlow109) / fSlow111) : fSlow95)) : ((fRec8[0] < fSlow104) ? fSlow106 * (fSlow94 + fSlow107 * ((fRec8[0] - fSlow105) / fSlow108)) : fSlow73)) : fRec7[1]);
				fTemp6 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec6[0] < 0.0f) ? fRec7[0] : ((fRec6[0] < fSlow86) ? fRec7[0] + fConst1 * (fRec6[0] * (fSlow75 - fRec7[0]) / fSlow85) : fSlow75))));
				fTemp7 = fRec9[1] + fSlow113;
				fRec9[0] = fTemp7 - std::floor(fTemp7);
			}
			if ((iSlow1 | (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow8 | (iSlow9 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16)))))))))))) {
				fTemp8 = 65536.0f * fRec9[0];
			}
			if ((iSlow1 | iSlow15)) {
				fRec5[0] = fTemp6 * ftbl0synth_dx7SIG0[(int(fTemp8 + fSlow115 * fRec5[1]) % 65536 + 65536) % 65536];
				fTemp9 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec5[0])) % 65536 + 65536) % 65536];
			}
			if ((iSlow1 | (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16)))))))))))))))) {
				fRec10[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow133, fRec10[1] + 1.0f));
				fRec12[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow151, fRec12[1] + 1.0f));
				iTemp10 = fRec12[0] < fSlow152;
				iTemp11 = fRec12[0] < fSlow156;
				fRec11[0] = ((iSlow18) ? fSlow40 * ((iTemp10) ? ((iTemp11) ? ((fRec12[0] < 0.0f) ? fSlow122 : ((iTemp11) ? fSlow122 + fSlow159 * (fRec12[0] / fSlow140) : fSlow135)) : ((iTemp10) ? fSlow135 + fSlow157 * ((fRec12[0] - fSlow156) / fSlow158) : fSlow142)) : ((fRec12[0] < fSlow151) ? fSlow153 * (fSlow141 + fSlow154 * ((fRec12[0] - fSlow152) / fSlow155)) : fSlow120)) : fRec11[1]);
				fTemp12 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec10[0] < 0.0f) ? fRec11[0] : ((fRec10[0] < fSlow133) ? fRec11[0] + fConst1 * (fRec10[0] * (fSlow122 - fRec11[0]) / fSlow132) : fSlow122))));
				fTemp13 = fRec13[1] + fSlow160;
				fRec13[0] = fTemp13 - std::floor(fTemp13);
				fRec14[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow178, fRec14[1] + 1.0f));
				fRec16[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow196, fRec16[1] + 1.0f));
				iTemp14 = fRec16[0] < fSlow197;
				iTemp15 = fRec16[0] < fSlow201;
				fRec15[0] = ((iSlow18) ? fSlow40 * ((iTemp14) ? ((iTemp15) ? ((fRec16[0] < 0.0f) ? fSlow167 : ((iTemp15) ? fSlow167 + fSlow204 * (fRec16[0] / fSlow185) : fSlow180)) : ((iTemp14) ? fSlow180 + fSlow202 * ((fRec16[0] - fSlow201) / fSlow203) : fSlow187)) : ((fRec16[0] < fSlow196) ? fSlow198 * (fSlow186 + fSlow199 * ((fRec16[0] - fSlow197) / fSlow200)) : fSlow165)) : fRec15[1]);
				fTemp16 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec14[0] < 0.0f) ? fRec15[0] : ((fRec14[0] < fSlow178) ? fRec15[0] + fConst1 * (fRec14[0] * (fSlow167 - fRec15[0]) / fSlow177) : fSlow167))));
				fTemp17 = fRec17[1] + fSlow205;
				fRec17[0] = fTemp17 - std::floor(fTemp17);
				fRec18[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow223, fRec18[1] + 1.0f));
				fRec20[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow241, fRec20[1] + 1.0f));
				iTemp18 = fRec20[0] < fSlow242;
				iTemp19 = fRec20[0] < fSlow246;
				fRec19[0] = ((iSlow18) ? fSlow40 * ((iTemp18) ? ((iTemp19) ? ((fRec20[0] < 0.0f) ? fSlow212 : ((iTemp19) ? fSlow212 + fSlow249 * (fRec20[0] / fSlow230) : fSlow225)) : ((iTemp18) ? fSlow225 + fSlow247 * ((fRec20[0] - fSlow246) / fSlow248) : fSlow232)) : ((fRec20[0] < fSlow241) ? fSlow243 * (fSlow231 + fSlow244 * ((fRec20[0] - fSlow242) / fSlow245)) : fSlow210)) : fRec19[1]);
				fTemp20 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec18[0] < 0.0f) ? fRec19[0] : ((fRec18[0] < fSlow223) ? fRec19[0] + fConst1 * (fRec18[0] * (fSlow212 - fRec19[0]) / fSlow222) : fSlow212))));
				fTemp21 = fRec21[1] + fSlow250;
				fRec21[0] = fTemp21 - std::floor(fTemp21);
				fTemp22 = 65536.0f * fRec21[0];
			}
			if ((iSlow1 | (iSlow2 | (iSlow8 | (iSlow10 | (iSlow12 | iSlow15)))))) {
				fTemp23 = fTemp20 * ftbl0synth_dx7SIG0[(int(fTemp22) % 65536 + 65536) % 65536];
			}
			if ((iSlow1 | (iSlow2 | iSlow8))) {
				fTemp24 = fTemp16 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec17[0] + fTemp23)) % 65536 + 65536) % 65536];
			}
			if ((iSlow1 | (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16)))))))))))))))) {
				fRec22[0] = ((iSlow19) ? 0.0f : std::min<float>(fSlow268, fRec22[1] + 1.0f));
				fRec24[0] = ((iSlow41) ? 0.0f : std::min<float>(fSlow286, fRec24[1] + 1.0f));
				iTemp25 = fRec24[0] < fSlow287;
				iTemp26 = fRec24[0] < fSlow291;
				fRec23[0] = ((iSlow18) ? fSlow40 * ((iTemp25) ? ((iTemp26) ? ((fRec24[0] < 0.0f) ? fSlow257 : ((iTemp26) ? fSlow257 + fSlow294 * (fRec24[0] / fSlow275) : fSlow270)) : ((iTemp25) ? fSlow270 + fSlow292 * ((fRec24[0] - fSlow291) / fSlow293) : fSlow277)) : ((fRec24[0] < fSlow286) ? fSlow288 * (fSlow276 + fSlow289 * ((fRec24[0] - fSlow287) / fSlow290)) : fSlow255)) : fRec23[1]);
				fTemp27 = synth_dx7_faustpower8_f(0.010204081f * std::min<float>(98.0f, ((fRec22[0] < 0.0f) ? fRec23[0] : ((fRec22[0] < fSlow268) ? fRec23[0] + fConst1 * (fRec22[0] * (fSlow257 - fRec23[0]) / fSlow267) : fSlow257))));
				fTemp28 = fRec25[1] + fSlow295;
				fRec25[0] = fTemp28 - std::floor(fTemp28);
			}
			if ((iSlow1 | (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow12 | (iSlow13 | iSlow16)))))))) {
				fTemp29 = 65536.0f * fRec25[0];
			}
			if ((iSlow1 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow12 | (iSlow13 | iSlow16))))))) {
				fTemp30 = fTemp27 * ftbl0synth_dx7SIG0[(int(fTemp29) % 65536 + 65536) % 65536];
			}
			if ((iSlow1)) {
				fTemp31 = 2.08795f * (fTemp9 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp24 + fTemp30)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow9 | (iSlow13 | (iSlow14 | iSlow16)))))))) {
				fTemp32 = fTemp6 * ftbl0synth_dx7SIG0[(int(fTemp8) % 65536 + 65536) % 65536];
			}
			if ((iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow9 | (iSlow13 | iSlow14))))))) {
				fTemp33 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp32)) % 65536 + 65536) % 65536];
			}
			if ((iSlow2)) {
				fRec26[0] = fTemp27 * ftbl0synth_dx7SIG0[(int(fTemp29 + fSlow296 * fRec26[1]) % 65536 + 65536) % 65536];
				fTemp34 = 2.08795f * (fTemp33 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fRec26[0] + fTemp24)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow3 | (iSlow7 | (iSlow13 | (iSlow14 | iSlow16))))) {
				fRec27[0] = fTemp20 * ftbl0synth_dx7SIG0[(int(fTemp22 + fSlow297 * fRec27[1]) % 65536 + 65536) % 65536];
			}
			if ((iSlow3 | (iSlow7 | iSlow16))) {
				fTemp35 = fTemp16 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec17[0] + fRec27[0])) % 65536 + 65536) % 65536];
			}
			if ((iSlow3)) {
				fTemp36 = 2.08795f * (fTemp33 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp35 + fTemp30)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow4)) {
				fRec28[0] = 2.08795f * fTemp16 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec17[0] + fTemp20 * ftbl0synth_dx7SIG0[(int(fTemp22 + fSlow297 * fRec28[1]) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
			}
			if ((iSlow4 | (iSlow5 | iSlow16))) {
				fTemp37 = fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp30)) % 65536 + 65536) % 65536];
			}
			if ((iSlow4 | iSlow5)) {
				fTemp38 = fTemp33 + fTemp37;
			}
			if ((iSlow4)) {
				fTemp39 = fRec28[0] + 2.08795f * fTemp38;
			}
			if ((iSlow5)) {
				fRec29[0] = fTemp20 * ftbl0synth_dx7SIG0[(int(fTemp22 + fSlow296 * fRec29[1]) % 65536 + 65536) % 65536];
				fTemp40 = 2.08795f * (fTemp38 + fTemp16 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec17[0] + fRec29[0])) % 65536 + 65536) % 65536]);
			}
			if ((iSlow6)) {
				fRec30[0] = 2.08795f * fTemp27 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fTemp16 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec17[0] + fTemp20 * ftbl0synth_dx7SIG0[(int(fTemp22 + fSlow297 * fRec30[1]) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
			}
			if ((iSlow6 | (iSlow7 | (iSlow10 | iSlow11)))) {
				fTemp41 = 65536.0f * fRec13[0];
			}
			if ((iSlow6 | (iSlow7 | iSlow11))) {
				fTemp42 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp6 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec9[0] + fTemp12 * ftbl0synth_dx7SIG0[(int(fTemp41) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
			}
			if ((iSlow6)) {
				fTemp43 = fRec30[0] + 2.08795f * fTemp42;
			}
			if ((iSlow7)) {
				fTemp44 = 2.08795f * (fTemp42 + fTemp27 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fTemp35)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow8 | iSlow12)) {
				fRec31[0] = fTemp6 * ftbl0synth_dx7SIG0[(int(fTemp8 + fSlow298 * fRec31[1]) % 65536 + 65536) % 65536];
				fTemp45 = fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fRec31[0])) % 65536 + 65536) % 65536];
			}
			if ((iSlow8)) {
				fTemp46 = 2.08795f * (fTemp45 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp27 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fTemp24)) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if ((iSlow9 | iSlow11)) {
				fRec32[0] = fTemp20 * ftbl0synth_dx7SIG0[(int(fTemp22 + fSlow298 * fRec32[1]) % 65536 + 65536) % 65536];
			}
			if ((iSlow9)) {
				fTemp47 = 2.08795f * (fTemp33 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp27 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fTemp16 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec17[0] + fRec32[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if ((iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | iSlow15)))))) {
				fTemp48 = fTemp16 * ftbl0synth_dx7SIG0[(int(65536.0f * fRec17[0]) % 65536 + 65536) % 65536];
			}
			if ((iSlow10 | iSlow15)) {
				fTemp49 = fTemp27 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fTemp23 + fTemp48)) % 65536 + 65536) % 65536];
			}
			if ((iSlow10)) {
				fRec33[0] = fTemp12 * ftbl0synth_dx7SIG0[(int(fTemp41 + fSlow298 * fRec33[1]) % 65536 + 65536) % 65536];
				fTemp50 = 2.08795f * (fTemp49 + fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp6 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec9[0] + fRec33[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if ((iSlow11)) {
				fTemp51 = 2.08795f * (fTemp42 + fTemp27 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fRec32[0] + fTemp48)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow12)) {
				fTemp52 = 2.08795f * (fTemp45 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp48 + fTemp23 + fTemp30)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow13)) {
				fTemp53 = 2.08795f * (fTemp33 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp48 + fRec27[0] + fTemp30)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow14)) {
				fTemp54 = 2.08795f * (fTemp33 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp27 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec25[0] + fRec27[0] + fTemp48)) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if ((iSlow15)) {
				fTemp55 = 2.08795f * (fTemp9 + fTemp12 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec13[0] + fTemp49)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow16)) {
				fTemp56 = 2.08795f * fTemp2 * ftbl0synth_dx7SIG0[(int(65536.0f * (fRec4[0] + fTemp37 + fTemp32 + fTemp35)) % 65536 + 65536) % 65536];
			}
			output0[i0] = FAUSTFLOAT(fTemp31 + fTemp34 + fTemp36 + fTemp39 + fTemp40 + fTemp43 + fTemp44 + fTemp46 + fTemp47 + fTemp50 + fTemp51 + fTemp52 + fTemp53 + fTemp54 + fTemp55 + fTemp56);
			if ((iSlow1 | (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16)))))))))))))))) {
				fRec0[1] = fRec0[0];
				fRec2[1] = fRec2[0];
				fRec1[1] = fRec1[0];
				fRec4[1] = fRec4[0];
				fRec6[1] = fRec6[0];
				fRec8[1] = fRec8[0];
				fRec7[1] = fRec7[0];
				fRec9[1] = fRec9[0];
			}
			if ((iSlow1 | iSlow15)) {
				fRec5[1] = fRec5[0];
			}
			if ((iSlow1 | (iSlow2 | (iSlow3 | (iSlow4 | (iSlow5 | (iSlow6 | (iSlow7 | (iSlow8 | (iSlow9 | (iSlow10 | (iSlow11 | (iSlow12 | (iSlow13 | (iSlow14 | (iSlow15 | iSlow16)))))))))))))))) {
				fRec10[1] = fRec10[0];
				fRec12[1] = fRec12[0];
				fRec11[1] = fRec11[0];
				fRec13[1] = fRec13[0];
				fRec14[1] = fRec14[0];
				fRec16[1] = fRec16[0];
				fRec15[1] = fRec15[0];
				fRec17[1] = fRec17[0];
				fRec18[1] = fRec18[0];
				fRec20[1] = fRec20[0];
				fRec19[1] = fRec19[0];
				fRec21[1] = fRec21[0];
				fRec22[1] = fRec22[0];
				fRec24[1] = fRec24[0];
				fRec23[1] = fRec23[0];
				fRec25[1] = fRec25[0];
			}
			if ((iSlow2)) {
				fRec26[1] = fRec26[0];
			}
			if ((iSlow3 | (iSlow7 | (iSlow13 | (iSlow14 | iSlow16))))) {
				fRec27[1] = fRec27[0];
			}
			if ((iSlow4)) {
				fRec28[1] = fRec28[0];
			}
			if ((iSlow5)) {
				fRec29[1] = fRec29[0];
			}
			if ((iSlow6)) {
				fRec30[1] = fRec30[0];
			}
			if ((iSlow8 | iSlow12)) {
				fRec31[1] = fRec31[0];
			}
			if ((iSlow9 | iSlow11)) {
				fRec32[1] = fRec32[0];
			}
			if ((iSlow10)) {
				fRec33[1] = fRec33[0];
			}
		}
		iTempPerm0 = iTemp0;
		iTempPerm1 = iTemp1;
		fTempPerm2 = fTemp2;
		fTempPerm3 = fTemp3;
		iTempPerm4 = iTemp4;
		iTempPerm5 = iTemp5;
		fTempPerm6 = fTemp6;
		fTempPerm7 = fTemp7;
		fTempPerm8 = fTemp8;
		fTempPerm9 = fTemp9;
		iTempPerm10 = iTemp10;
		iTempPerm11 = iTemp11;
		fTempPerm12 = fTemp12;
		fTempPerm13 = fTemp13;
		iTempPerm14 = iTemp14;
		iTempPerm15 = iTemp15;
		fTempPerm16 = fTemp16;
		fTempPerm17 = fTemp17;
		iTempPerm18 = iTemp18;
		iTempPerm19 = iTemp19;
		fTempPerm20 = fTemp20;
		fTempPerm21 = fTemp21;
		fTempPerm22 = fTemp22;
		fTempPerm23 = fTemp23;
		fTempPerm24 = fTemp24;
		iTempPerm25 = iTemp25;
		iTempPerm26 = iTemp26;
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
