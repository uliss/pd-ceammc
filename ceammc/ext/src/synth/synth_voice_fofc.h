/* ------------------------------------------------------------
name: "synth.voice_fofc"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_voice_fofc -scn synth_voice_fofc_dsp -es 1 -mcd 16 -single -ftz 0
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
    
        virtual synth_voice_fofc_dsp* createDSPInstance() = 0;
    
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
	
	int iVec0[2];
	int iRec2[2];
	
  public:
	
	int getNumInputssynth_voice_fofcSIG0() {
		return 0;
	}
	int getNumOutputssynth_voice_fofcSIG0() {
		return 1;
	}
	
	void instanceInitsynth_voice_fofcSIG0(int sample_rate) {
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iVec0[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec2[l3] = 0;
		}
	}
	
	void fillsynth_voice_fofcSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec2[0] = (iVec0[1] + iRec2[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec2[0]));
			iVec0[1] = iVec0[0];
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
	int fSampleRate;
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
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fRec4[2];
	float fRec7[2];
	float fRec6[2];
	float fVec1[3];
	float fRec8[2];
	float fRec9[2];
	float fRec5[3];
	float fRec10[2];
	float fVec2[3];
	float fRec12[2];
	float fRec13[2];
	float fRec11[3];
	float fRec14[2];
	float fVec3[3];
	float fRec16[2];
	float fRec17[2];
	float fRec15[3];
	float fRec18[2];
	float fVec4[3];
	float fRec20[2];
	float fRec21[2];
	float fRec19[3];
	float fRec22[2];
	float fVec5[3];
	float fRec24[2];
	float fRec25[2];
	float fRec23[3];
	float fRec26[2];
	float fVec6[3];
	float fRec28[2];
	float fRec29[2];
	float fRec27[3];
	float fRec30[2];
	float fVec7[3];
	float fRec32[2];
	float fRec33[2];
	float fRec31[3];
	float fRec34[2];
	float fVec8[3];
	float fRec36[2];
	float fRec37[2];
	float fRec35[3];
	float fRec38[2];
	float fVec9[3];
	float fRec40[2];
	float fRec41[2];
	float fRec39[3];
	float fRec42[2];
	float fVec10[3];
	float fRec44[2];
	float fRec45[2];
	float fRec43[3];
	float fRec46[2];
	float fVec11[3];
	float fRec48[2];
	float fRec49[2];
	float fRec47[3];
	float fRec50[2];
	float fVec12[3];
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
	float fConst4;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fCheckbox0;
	float fVec13[2];
	float fRec247[2];
	FAUSTFLOAT fHslider7;
	int iRec248[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/cycle:author", "Mike Olsen");
		m->declare("basics.lib/listInterp:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/pulse_countup_loop:author", "Vince");
		m->declare("basics.lib/resetCtr:author", "Mike Olsen");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_voice_fofc -scn synth_voice_fofc_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("envelopes.lib/asr:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.2");
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
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "synth.voice_fofc");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.4");
		m->declare("oscillators.lib/hs_oscsin:author", "Mike Olsen");
		m->declare("oscillators.lib/hs_phasor:author", "Mike Olsen, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.3");
		m->declare("physmodels.lib/SFFormantModel:author", "Mike Olsen");
		m->declare("physmodels.lib/autobendFreq:author", "Mike Olsen");
		m->declare("physmodels.lib/fof:author", "Mike Olsen");
		m->declare("physmodels.lib/fofCycle:author", "Mike Olsen");
		m->declare("physmodels.lib/fofSH:author", "Mike Olsen");
		m->declare("physmodels.lib/formantFilterbank:author", "Mike Olsen");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/skirtWidthMultiplier:author", "Mike Olsen");
		m->declare("physmodels.lib/version", "0.1");
		m->declare("physmodels.lib/vocalEffort:author", "Mike Olsen");
		m->declare("physmodels.lib/voiceGender:author", "Mike Olsen");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
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
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / fConst0;
		fConst4 = 0.001f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
		fVslider0 = FAUSTFLOAT(4.4e+02f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(6.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(1e+01f);
		fHslider6 = FAUSTFLOAT(3e+01f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(5e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec1[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec7[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fVec1[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec5[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec10[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = l13 + 1) {
			fVec2[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
			fRec11[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec14[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fVec3[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec15[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec18[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fVec4[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec20[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec21[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec19[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec22[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 3; l28 = l28 + 1) {
			fVec5[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec24[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec25[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 3; l31 = l31 + 1) {
			fRec23[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec26[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fVec6[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec28[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec29[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 3; l36 = l36 + 1) {
			fRec27[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec30[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 3; l38 = l38 + 1) {
			fVec7[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec32[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec33[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 3; l41 = l41 + 1) {
			fRec31[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec34[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fVec8[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec36[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec37[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 3; l46 = l46 + 1) {
			fRec35[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec38[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fVec9[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec40[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec41[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec39[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec42[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fVec10[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec44[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec45[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fRec43[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec46[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 3; l58 = l58 + 1) {
			fVec11[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec48[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec49[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 3; l61 = l61 + 1) {
			fRec47[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec50[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fVec12[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec52[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec53[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 3; l66 = l66 + 1) {
			fRec51[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec54[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec56[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fRec57[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fRec55[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec58[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec60[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec61[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 3; l74 = l74 + 1) {
			fRec59[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec62[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec64[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec65[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fRec63[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec66[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec68[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec69[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 3; l82 = l82 + 1) {
			fRec67[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec70[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fRec72[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec73[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec71[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec74[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec76[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec77[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 3; l90 = l90 + 1) {
			fRec75[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			fRec78[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec80[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec81[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 3; l94 = l94 + 1) {
			fRec79[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fRec82[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec84[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec85[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 3; l98 = l98 + 1) {
			fRec83[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			fRec86[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			fRec88[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec89[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 3; l102 = l102 + 1) {
			fRec87[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec90[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			fRec92[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fRec93[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 3; l106 = l106 + 1) {
			fRec91[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 2; l107 = l107 + 1) {
			fRec94[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec96[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			fRec97[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 3; l110 = l110 + 1) {
			fRec95[l110] = 0.0f;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			fRec98[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec100[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec101[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 3; l114 = l114 + 1) {
			fRec99[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fRec102[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			fRec104[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fRec105[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 3; l118 = l118 + 1) {
			fRec103[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 2; l119 = l119 + 1) {
			fRec106[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fRec108[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			fRec109[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 3; l122 = l122 + 1) {
			fRec107[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fRec110[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			fRec112[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec113[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 3; l126 = l126 + 1) {
			fRec111[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec114[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			fRec116[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fRec117[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 3; l130 = l130 + 1) {
			fRec115[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
			fRec118[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec120[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fRec121[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 3; l134 = l134 + 1) {
			fRec119[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fRec122[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
			fRec124[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 2; l137 = l137 + 1) {
			fRec125[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 3; l138 = l138 + 1) {
			fRec123[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
			fRec126[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
			fRec128[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 2; l141 = l141 + 1) {
			fRec129[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 3; l142 = l142 + 1) {
			fRec127[l142] = 0.0f;
		}
		for (int l143 = 0; l143 < 2; l143 = l143 + 1) {
			fRec130[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fRec132[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec133[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 3; l146 = l146 + 1) {
			fRec131[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 2; l147 = l147 + 1) {
			fRec134[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fRec136[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fRec137[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 3; l150 = l150 + 1) {
			fRec135[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			fRec138[l151] = 0.0f;
		}
		for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
			fRec140[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
			fRec141[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 3; l154 = l154 + 1) {
			fRec139[l154] = 0.0f;
		}
		for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
			fRec142[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec144[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
			fRec145[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 3; l158 = l158 + 1) {
			fRec143[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 2; l159 = l159 + 1) {
			fRec146[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fRec148[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 2; l161 = l161 + 1) {
			fRec149[l161] = 0.0f;
		}
		for (int l162 = 0; l162 < 3; l162 = l162 + 1) {
			fRec147[l162] = 0.0f;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec150[l163] = 0.0f;
		}
		for (int l164 = 0; l164 < 2; l164 = l164 + 1) {
			fRec152[l164] = 0.0f;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fRec153[l165] = 0.0f;
		}
		for (int l166 = 0; l166 < 3; l166 = l166 + 1) {
			fRec151[l166] = 0.0f;
		}
		for (int l167 = 0; l167 < 2; l167 = l167 + 1) {
			fRec154[l167] = 0.0f;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fRec156[l168] = 0.0f;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fRec157[l169] = 0.0f;
		}
		for (int l170 = 0; l170 < 3; l170 = l170 + 1) {
			fRec155[l170] = 0.0f;
		}
		for (int l171 = 0; l171 < 2; l171 = l171 + 1) {
			fRec158[l171] = 0.0f;
		}
		for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
			fRec160[l172] = 0.0f;
		}
		for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
			fRec161[l173] = 0.0f;
		}
		for (int l174 = 0; l174 < 3; l174 = l174 + 1) {
			fRec159[l174] = 0.0f;
		}
		for (int l175 = 0; l175 < 2; l175 = l175 + 1) {
			fRec162[l175] = 0.0f;
		}
		for (int l176 = 0; l176 < 2; l176 = l176 + 1) {
			fRec164[l176] = 0.0f;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			fRec165[l177] = 0.0f;
		}
		for (int l178 = 0; l178 < 3; l178 = l178 + 1) {
			fRec163[l178] = 0.0f;
		}
		for (int l179 = 0; l179 < 2; l179 = l179 + 1) {
			fRec166[l179] = 0.0f;
		}
		for (int l180 = 0; l180 < 2; l180 = l180 + 1) {
			fRec168[l180] = 0.0f;
		}
		for (int l181 = 0; l181 < 2; l181 = l181 + 1) {
			fRec169[l181] = 0.0f;
		}
		for (int l182 = 0; l182 < 3; l182 = l182 + 1) {
			fRec167[l182] = 0.0f;
		}
		for (int l183 = 0; l183 < 2; l183 = l183 + 1) {
			fRec170[l183] = 0.0f;
		}
		for (int l184 = 0; l184 < 2; l184 = l184 + 1) {
			fRec172[l184] = 0.0f;
		}
		for (int l185 = 0; l185 < 2; l185 = l185 + 1) {
			fRec173[l185] = 0.0f;
		}
		for (int l186 = 0; l186 < 3; l186 = l186 + 1) {
			fRec171[l186] = 0.0f;
		}
		for (int l187 = 0; l187 < 2; l187 = l187 + 1) {
			fRec174[l187] = 0.0f;
		}
		for (int l188 = 0; l188 < 2; l188 = l188 + 1) {
			fRec176[l188] = 0.0f;
		}
		for (int l189 = 0; l189 < 2; l189 = l189 + 1) {
			fRec177[l189] = 0.0f;
		}
		for (int l190 = 0; l190 < 3; l190 = l190 + 1) {
			fRec175[l190] = 0.0f;
		}
		for (int l191 = 0; l191 < 2; l191 = l191 + 1) {
			fRec178[l191] = 0.0f;
		}
		for (int l192 = 0; l192 < 2; l192 = l192 + 1) {
			fRec180[l192] = 0.0f;
		}
		for (int l193 = 0; l193 < 2; l193 = l193 + 1) {
			fRec181[l193] = 0.0f;
		}
		for (int l194 = 0; l194 < 3; l194 = l194 + 1) {
			fRec179[l194] = 0.0f;
		}
		for (int l195 = 0; l195 < 2; l195 = l195 + 1) {
			fRec182[l195] = 0.0f;
		}
		for (int l196 = 0; l196 < 2; l196 = l196 + 1) {
			fRec184[l196] = 0.0f;
		}
		for (int l197 = 0; l197 < 2; l197 = l197 + 1) {
			fRec185[l197] = 0.0f;
		}
		for (int l198 = 0; l198 < 3; l198 = l198 + 1) {
			fRec183[l198] = 0.0f;
		}
		for (int l199 = 0; l199 < 2; l199 = l199 + 1) {
			fRec186[l199] = 0.0f;
		}
		for (int l200 = 0; l200 < 2; l200 = l200 + 1) {
			fRec188[l200] = 0.0f;
		}
		for (int l201 = 0; l201 < 2; l201 = l201 + 1) {
			fRec189[l201] = 0.0f;
		}
		for (int l202 = 0; l202 < 3; l202 = l202 + 1) {
			fRec187[l202] = 0.0f;
		}
		for (int l203 = 0; l203 < 2; l203 = l203 + 1) {
			fRec190[l203] = 0.0f;
		}
		for (int l204 = 0; l204 < 2; l204 = l204 + 1) {
			fRec192[l204] = 0.0f;
		}
		for (int l205 = 0; l205 < 2; l205 = l205 + 1) {
			fRec193[l205] = 0.0f;
		}
		for (int l206 = 0; l206 < 3; l206 = l206 + 1) {
			fRec191[l206] = 0.0f;
		}
		for (int l207 = 0; l207 < 2; l207 = l207 + 1) {
			fRec194[l207] = 0.0f;
		}
		for (int l208 = 0; l208 < 2; l208 = l208 + 1) {
			fRec196[l208] = 0.0f;
		}
		for (int l209 = 0; l209 < 2; l209 = l209 + 1) {
			fRec197[l209] = 0.0f;
		}
		for (int l210 = 0; l210 < 3; l210 = l210 + 1) {
			fRec195[l210] = 0.0f;
		}
		for (int l211 = 0; l211 < 2; l211 = l211 + 1) {
			fRec198[l211] = 0.0f;
		}
		for (int l212 = 0; l212 < 2; l212 = l212 + 1) {
			fRec200[l212] = 0.0f;
		}
		for (int l213 = 0; l213 < 2; l213 = l213 + 1) {
			fRec201[l213] = 0.0f;
		}
		for (int l214 = 0; l214 < 3; l214 = l214 + 1) {
			fRec199[l214] = 0.0f;
		}
		for (int l215 = 0; l215 < 2; l215 = l215 + 1) {
			fRec202[l215] = 0.0f;
		}
		for (int l216 = 0; l216 < 2; l216 = l216 + 1) {
			fRec204[l216] = 0.0f;
		}
		for (int l217 = 0; l217 < 2; l217 = l217 + 1) {
			fRec205[l217] = 0.0f;
		}
		for (int l218 = 0; l218 < 3; l218 = l218 + 1) {
			fRec203[l218] = 0.0f;
		}
		for (int l219 = 0; l219 < 2; l219 = l219 + 1) {
			fRec206[l219] = 0.0f;
		}
		for (int l220 = 0; l220 < 2; l220 = l220 + 1) {
			fRec208[l220] = 0.0f;
		}
		for (int l221 = 0; l221 < 2; l221 = l221 + 1) {
			fRec209[l221] = 0.0f;
		}
		for (int l222 = 0; l222 < 3; l222 = l222 + 1) {
			fRec207[l222] = 0.0f;
		}
		for (int l223 = 0; l223 < 2; l223 = l223 + 1) {
			fRec210[l223] = 0.0f;
		}
		for (int l224 = 0; l224 < 2; l224 = l224 + 1) {
			fRec212[l224] = 0.0f;
		}
		for (int l225 = 0; l225 < 2; l225 = l225 + 1) {
			fRec213[l225] = 0.0f;
		}
		for (int l226 = 0; l226 < 3; l226 = l226 + 1) {
			fRec211[l226] = 0.0f;
		}
		for (int l227 = 0; l227 < 2; l227 = l227 + 1) {
			fRec214[l227] = 0.0f;
		}
		for (int l228 = 0; l228 < 2; l228 = l228 + 1) {
			fRec216[l228] = 0.0f;
		}
		for (int l229 = 0; l229 < 2; l229 = l229 + 1) {
			fRec217[l229] = 0.0f;
		}
		for (int l230 = 0; l230 < 3; l230 = l230 + 1) {
			fRec215[l230] = 0.0f;
		}
		for (int l231 = 0; l231 < 2; l231 = l231 + 1) {
			fRec218[l231] = 0.0f;
		}
		for (int l232 = 0; l232 < 2; l232 = l232 + 1) {
			fRec220[l232] = 0.0f;
		}
		for (int l233 = 0; l233 < 2; l233 = l233 + 1) {
			fRec221[l233] = 0.0f;
		}
		for (int l234 = 0; l234 < 3; l234 = l234 + 1) {
			fRec219[l234] = 0.0f;
		}
		for (int l235 = 0; l235 < 2; l235 = l235 + 1) {
			fRec222[l235] = 0.0f;
		}
		for (int l236 = 0; l236 < 2; l236 = l236 + 1) {
			fRec224[l236] = 0.0f;
		}
		for (int l237 = 0; l237 < 2; l237 = l237 + 1) {
			fRec225[l237] = 0.0f;
		}
		for (int l238 = 0; l238 < 3; l238 = l238 + 1) {
			fRec223[l238] = 0.0f;
		}
		for (int l239 = 0; l239 < 2; l239 = l239 + 1) {
			fRec226[l239] = 0.0f;
		}
		for (int l240 = 0; l240 < 2; l240 = l240 + 1) {
			fRec228[l240] = 0.0f;
		}
		for (int l241 = 0; l241 < 2; l241 = l241 + 1) {
			fRec229[l241] = 0.0f;
		}
		for (int l242 = 0; l242 < 3; l242 = l242 + 1) {
			fRec227[l242] = 0.0f;
		}
		for (int l243 = 0; l243 < 2; l243 = l243 + 1) {
			fRec230[l243] = 0.0f;
		}
		for (int l244 = 0; l244 < 2; l244 = l244 + 1) {
			fRec232[l244] = 0.0f;
		}
		for (int l245 = 0; l245 < 2; l245 = l245 + 1) {
			fRec233[l245] = 0.0f;
		}
		for (int l246 = 0; l246 < 3; l246 = l246 + 1) {
			fRec231[l246] = 0.0f;
		}
		for (int l247 = 0; l247 < 2; l247 = l247 + 1) {
			fRec234[l247] = 0.0f;
		}
		for (int l248 = 0; l248 < 2; l248 = l248 + 1) {
			fRec236[l248] = 0.0f;
		}
		for (int l249 = 0; l249 < 2; l249 = l249 + 1) {
			fRec237[l249] = 0.0f;
		}
		for (int l250 = 0; l250 < 3; l250 = l250 + 1) {
			fRec235[l250] = 0.0f;
		}
		for (int l251 = 0; l251 < 2; l251 = l251 + 1) {
			fRec238[l251] = 0.0f;
		}
		for (int l252 = 0; l252 < 2; l252 = l252 + 1) {
			fRec240[l252] = 0.0f;
		}
		for (int l253 = 0; l253 < 2; l253 = l253 + 1) {
			fRec241[l253] = 0.0f;
		}
		for (int l254 = 0; l254 < 3; l254 = l254 + 1) {
			fRec239[l254] = 0.0f;
		}
		for (int l255 = 0; l255 < 2; l255 = l255 + 1) {
			fRec242[l255] = 0.0f;
		}
		for (int l256 = 0; l256 < 2; l256 = l256 + 1) {
			fRec244[l256] = 0.0f;
		}
		for (int l257 = 0; l257 < 2; l257 = l257 + 1) {
			fRec245[l257] = 0.0f;
		}
		for (int l258 = 0; l258 < 3; l258 = l258 + 1) {
			fRec243[l258] = 0.0f;
		}
		for (int l259 = 0; l259 < 2; l259 = l259 + 1) {
			fRec246[l259] = 0.0f;
		}
		for (int l260 = 0; l260 < 2; l260 = l260 + 1) {
			fVec13[l260] = 0.0f;
		}
		for (int l261 = 0; l261 < 2; l261 = l261 + 1) {
			fRec247[l261] = 0.0f;
		}
		for (int l262 = 0; l262 < 2; l262 = l262 + 1) {
			iRec248[l262] = 0;
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
		int iSlow0 = int(float(fHslider0));
		int iSlow1 = ((iSlow0 == 0) ? 1 : ((iSlow0 == 3) ? 1 : 0));
		int iSlow2 = iSlow1 == 0;
		float fSlow3 = fConst1 * float(fVslider0);
		float fSlow4 = 0.001f * float(fHslider1);
		int iSlow5 = std::fabs(fSlow4) < 1.1920929e-07f;
		float fSlow6 = ((iSlow5) ? 0.0f : std::exp(0.0f - fConst3 / ((iSlow5) ? 1.0f : fSlow4)));
		float fSlow7 = 1.0f - fSlow6;
		float fSlow8 = 0.1f * float(fHslider2);
		float fSlow9 = fConst3 * float(fHslider3);
		float fSlow10 = float(5 * iSlow0);
		float fSlow11 = 0.001f * float(fHslider5);
		int iSlow12 = std::fabs(fSlow11) < 1.1920929e-07f;
		float fSlow13 = ((iSlow12) ? 0.0f : std::exp(0.0f - fConst3 / ((iSlow12) ? 1.0f : fSlow11)));
		float fSlow14 = float(fHslider4) * (1.0f - fSlow13);
		float fSlow15 = float(5 * iSlow1);
		int iSlow16 = iSlow1 >= 1;
		float fSlow17 = ((iSlow16) ? 174.61f : 82.41f);
		float fSlow18 = ((iSlow16) ? 1046.5f : 523.25f);
		float fSlow19 = fSlow18 - fSlow17;
		int iSlow20 = iSlow0 != 2;
		float fSlow21 = 1.0f / std::max<float>(1.0f, fConst4 * float(fHslider6));
		float fSlow22 = float(fCheckbox0);
		float fSlow23 = 1.0f / std::max<float>(1.0f, fConst4 * float(fHslider7));
		int iSlow24 = fSlow22 == 0.0f;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec1[0] = fSlow3 + fConst2 * fRec1[1];
			fRec0[0] = fRec1[0] * fSlow7 + fSlow6 * fRec0[1];
			fRec3[0] = fSlow9 + (fRec3[1] - std::floor(fSlow9 + fRec3[1]));
			float fTemp0 = fRec0[0] * (fSlow8 * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec3[0])] + 1.0f);
			float fTemp1 = 0.00084f * (1e+03f - fTemp0) + 0.8f;
			fRec4[0] = fSlow14 + fSlow13 * fRec4[1];
			float fTemp2 = fSlow10 + fRec4[0];
			int iTemp3 = fTemp2 < 23.0f;
			int iTemp4 = fTemp2 < 24.0f;
			float fTemp5 = fSlow10 + fRec4[0] + -23.0f;
			int iTemp6 = fTemp2 < 22.0f;
			float fTemp7 = fSlow10 + fRec4[0] + -22.0f;
			int iTemp8 = fTemp2 < 21.0f;
			float fTemp9 = fSlow10 + fRec4[0] + -21.0f;
			int iTemp10 = fTemp2 < 2e+01f;
			float fTemp11 = fSlow10 + fRec4[0] + -2e+01f;
			int iTemp12 = fTemp2 < 19.0f;
			float fTemp13 = fSlow10 + fRec4[0] + -19.0f;
			int iTemp14 = fTemp2 < 18.0f;
			float fTemp15 = fSlow10 + fRec4[0] + -18.0f;
			int iTemp16 = fTemp2 < 17.0f;
			float fTemp17 = fSlow10 + fRec4[0] + -17.0f;
			float fTemp18 = ((iTemp14) ? 0.029314f * fTemp17 + 0.050119f : 0.079433f);
			int iTemp19 = fTemp2 < 16.0f;
			float fTemp20 = fSlow10 + fRec4[0] + -16.0f;
			int iTemp21 = fTemp2 < 15.0f;
			float fTemp22 = fSlow10 + fRec4[0] + -15.0f;
			int iTemp23 = fTemp2 < 14.0f;
			float fTemp24 = fSlow10 + fRec4[0] + -14.0f;
			int iTemp25 = fTemp2 < 13.0f;
			float fTemp26 = fSlow10 + fRec4[0] + -13.0f;
			int iTemp27 = fTemp2 < 12.0f;
			float fTemp28 = fSlow10 + fRec4[0] + -12.0f;
			int iTemp29 = fTemp2 < 11.0f;
			float fTemp30 = fSlow10 + fRec4[0] + -11.0f;
			float fTemp31 = ((iTemp27) ? 0.1f - 0.084151f * fTemp30 : 0.015849f);
			int iTemp32 = fTemp2 < 1e+01f;
			float fTemp33 = fSlow10 + fRec4[0] + -1e+01f;
			int iTemp34 = fTemp2 < 9.0f;
			float fTemp35 = fSlow10 + fRec4[0] + -9.0f;
			int iTemp36 = fTemp2 < 8.0f;
			float fTemp37 = fSlow10 + fRec4[0] + -8.0f;
			int iTemp38 = fTemp2 < 7.0f;
			float fTemp39 = fSlow10 + fRec4[0] + -7.0f;
			int iTemp40 = fTemp2 < 6.0f;
			float fTemp41 = fSlow10 + fRec4[0] + -6.0f;
			int iTemp42 = fTemp2 < 5.0f;
			float fTemp43 = fSlow10 + fRec4[0] + -5.0f;
			int iTemp44 = fTemp2 < 4.0f;
			float fTemp45 = fSlow10 + fRec4[0] + -4.0f;
			int iTemp46 = fTemp2 < 3.0f;
			float fTemp47 = fSlow10 + fRec4[0] + -3.0f;
			int iTemp48 = fTemp2 < 2.0f;
			float fTemp49 = fSlow10 + fRec4[0] + -2.0f;
			int iTemp50 = fTemp2 < 1.0f;
			float fTemp51 = fSlow10 + fRec4[0] + -1.0f;
			int iTemp52 = fTemp2 < 0.0f;
			float fTemp53 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 0.015849f : ((iTemp50) ? 0.001934f * fTemp2 + 0.015849f : 0.017783f)) : ((iTemp48) ? 0.017783f - 0.001934f * fTemp51 : 0.015849f)) : ((iTemp46) ? 0.023962f * fTemp49 + 0.015849f : 0.039811f)) : ((iTemp44) ? 0.039811f - 0.029811f * fTemp47 : 0.01f)) : ((iTemp42) ? 0.344813f * fTemp45 + 0.01f : 0.354813f)) : ((iTemp40) ? 0.354813f - 0.103624f * fTemp43 : 0.251189f)) : ((iTemp38) ? 0.251189f - 0.171756f * fTemp41 : 0.079433f)) : ((iTemp36) ? 0.020567f * fTemp39 + 0.079433f : 0.1f)) : ((iTemp34) ? 0.1f - 0.060189f * fTemp37 : 0.039811f)) : ((iTemp32) ? 0.023285f * fTemp35 + 0.039811f : 0.063096f)) : ((iTemp29) ? 0.036904f * fTemp33 + 0.063096f : 0.1f)) : fTemp31) : ((iTemp25) ? 0.063584f * fTemp28 + 0.015849f : 0.079433f)) : ((iTemp23) ? 0.079433f - 0.04781f * fTemp26 : 0.031623f)) : ((iTemp21) ? 0.068377f * fTemp24 + 0.031623f : 0.1f)) : ((iTemp19) ? 0.1f - 0.09f * fTemp22 : 0.01f)) : ((iTemp16) ? 0.040119f * fTemp20 + 0.01f : 0.050119f)) : fTemp18) : ((iTemp12) ? 0.079433f - 0.069433f * fTemp15 : 0.01f)) : ((iTemp10) ? 0.388107f * fTemp13 + 0.01f : 0.398107f)) : ((iTemp8) ? 0.398107f - 0.198581f * fTemp11 : 0.199526f)) : ((iTemp6) ? 0.199526f - 0.099526f * fTemp9 : 0.1f)) : ((iTemp3) ? 0.151189f * fTemp7 + 0.1f : 0.251189f)) : ((iTemp4) ? 0.251189f - 0.051663f * fTemp5 : 0.199526f));
			float fTemp54 = 0.0036666666f * (4e+02f - fTemp0) + 3.0f;
			float fTemp55 = fRec7[1] + fConst3 * fTemp0;
			fRec7[0] = fTemp55 - std::floor(fTemp55);
			float fTemp56 = float((fRec7[0] - fRec7[1]) < 0.0f);
			fRec6[0] = fTemp56 + fRec6[1] * float(fRec6[1] <= 11.0f);
			float fTemp57 = float(fRec6[0] == 1.0f) * fTemp56;
			fVec1[0] = fTemp57;
			int iTemp58 = int(fTemp57);
			float fTemp59 = fRec4[0] + fSlow15;
			int iTemp60 = fTemp59 >= 5.0f;
			int iTemp61 = fTemp59 >= 3.0f;
			int iTemp62 = fTemp59 >= 2.0f;
			int iTemp63 = fTemp59 >= 1.0f;
			int iTemp64 = fTemp59 >= 4.0f;
			int iTemp65 = fTemp59 >= 8.0f;
			int iTemp66 = fTemp59 >= 7.0f;
			int iTemp67 = fTemp59 >= 6.0f;
			float fTemp68 = ((iTemp60) ? ((iTemp65) ? 2.0f : ((iTemp66) ? 3.0f : ((iTemp67) ? 3.0f : 2.0f))) : ((iTemp61) ? ((iTemp64) ? 1.5f : 1.0f) : ((iTemp62) ? 1.25f : ((iTemp63) ? 1.25f : 1.0f))));
			float fTemp69 = fTemp68 + (((iTemp60) ? ((iTemp65) ? 12.0f : ((iTemp66) ? 12.0f : ((iTemp67) ? 12.0f : 15.0f))) : ((iTemp61) ? ((iTemp64) ? 4.0f : 1e+01f) : ((iTemp62) ? 2.5f : ((iTemp63) ? 2.5f : 1e+01f)))) - fTemp68) * ((fTemp0 <= fSlow17) ? 0.0f : ((fTemp0 >= fSlow18) ? 1.0f : (fTemp0 - fSlow17) / fSlow19));
			float fTemp70 = 1e+01f * fTemp7;
			float fTemp71 = 1e+01f * fTemp11;
			float fTemp72 = 5e+01f * fTemp13;
			float fTemp73 = 2e+01f * fTemp22;
			float fTemp74 = 1e+01f * fTemp33;
			float fTemp75 = 1e+01f * fTemp35;
			float fTemp76 = 6e+01f * fTemp45;
			float fTemp77 = 2e+01f * fTemp49;
			float fTemp78 = 2e+01f * fTemp2;
			float fTemp79 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 1.3e+02f : ((iTemp50) ? fTemp78 + 1.3e+02f : 1.5e+02f)) : 1.5e+02f) : ((iTemp46) ? 1.5e+02f - fTemp77 : 1.3e+02f)) : ((iTemp44) ? 5e+01f * fTemp47 + 1.3e+02f : 1.8e+02f)) : ((iTemp42) ? 1.8e+02f - fTemp76 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp32) ? fTemp75 + 1.2e+02f : 1.3e+02f)) : ((iTemp29) ? 1.3e+02f - fTemp74 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp21) ? 1e+01f * fTemp24 + 1.2e+02f : 1.3e+02f)) : ((iTemp19) ? fTemp73 + 1.3e+02f : 1.5e+02f)) : ((iTemp16) ? 1.5e+02f - 3e+01f * fTemp20 : 1.2e+02f)) : 1.2e+02f) : ((iTemp12) ? 6e+01f * fTemp15 + 1.2e+02f : 1.8e+02f)) : ((iTemp10) ? 1.8e+02f - fTemp72 : 1.3e+02f)) : ((iTemp8) ? 1.3e+02f - fTemp71 : 1.2e+02f)) : 1.2e+02f) : ((iTemp3) ? fTemp70 + 1.2e+02f : 1.3e+02f)) : ((iTemp4) ? 1.3e+02f - 1e+01f * fTemp5 : 1.2e+02f));
			float fTemp80 = fTemp69 * fTemp79;
			fRec8[0] = ((iTemp58) ? fTemp80 : fRec8[1]);
			float fTemp81 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec8[0]));
			fRec9[0] = ((iTemp58) ? fTemp79 : fRec9[1]);
			float fTemp82 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec9[0]));
			fRec5[0] = fVec1[2] - (fRec5[1] * (0.0f - (fTemp81 + fTemp82)) + fTemp81 * fTemp82 * fRec5[2]);
			int iTemp83 = int(fVec1[1]);
			float fTemp84 = 1e+02f * fTemp5;
			float fTemp85 = 3.5e+02f * fTemp43;
			float fTemp86 = fConst3 * ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 3.5e+03f : ((iTemp50) ? 3.5e+03f - 2e+02f * fTemp2 : 3.3e+03f)) : ((iTemp48) ? 4e+02f * fTemp51 + 3.3e+03f : 3.7e+03f)) : ((iTemp46) ? 3.7e+03f - 2e+02f * fTemp49 : 3.5e+03f)) : 3.5e+03f) : ((iTemp42) ? 3.5e+03f - 1.05e+03f * fTemp45 : 2.45e+03f)) : ((iTemp40) ? fTemp85 + 2.45e+03f : 2.8e+03f)) : ((iTemp38) ? 2.5e+02f * fTemp41 + 2.8e+03f : 3.05e+03f)) : ((iTemp36) ? 3.05e+03f - 4.5e+02f * fTemp39 : 2.6e+03f)) : ((iTemp34) ? 75.0f * fTemp37 + 2.6e+03f : 2675.0f)) : ((iTemp32) ? 325.0f * fTemp35 + 2675.0f : 3e+03f)) : 3e+03f) : ((iTemp27) ? 3.5e+02f * fTemp30 + 3e+03f : 3.35e+03f)) : ((iTemp25) ? 3.35e+03f - 3.5e+02f * fTemp28 : 3e+03f)) : 3e+03f) : ((iTemp21) ? 9e+02f * fTemp24 + 3e+03f : 3.9e+03f)) : ((iTemp19) ? 3.9e+03f - 3e+02f * fTemp22 : 3.6e+03f)) : ((iTemp16) ? 3e+02f * fTemp20 + 3.6e+03f : 3.9e+03f)) : ((iTemp14) ? 3.9e+03f - 1e+02f * fTemp17 : 3.8e+03f)) : 3.8e+03f) : ((iTemp10) ? 3.8e+03f - 9e+02f * fTemp13 : 2.9e+03f)) : ((iTemp8) ? 3e+02f * fTemp11 + 2.9e+03f : 3.2e+03f)) : ((iTemp6) ? 5e+01f * fTemp9 + 3.2e+03f : 3.25e+03f)) : ((iTemp3) ? 3.25e+03f - 4.5e+02f * fTemp7 : 2.8e+03f)) : ((iTemp4) ? fTemp84 + 2.8e+03f : 2.9e+03f));
			float fTemp87 = ((iTemp83) ? 0.0f : fRec10[1] + fTemp86);
			fRec10[0] = fTemp87 - std::floor(fTemp87);
			float fTemp88 = float(fRec6[0] == 2.0f) * fTemp56;
			fVec2[0] = fTemp88;
			int iTemp89 = int(fTemp88);
			fRec12[0] = ((iTemp89) ? fTemp80 : fRec12[1]);
			float fTemp90 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec12[0]));
			fRec13[0] = ((iTemp89) ? fTemp79 : fRec13[1]);
			float fTemp91 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec13[0]));
			fRec11[0] = fVec2[2] - (fRec11[1] * (0.0f - (fTemp90 + fTemp91)) + fTemp90 * fTemp91 * fRec11[2]);
			int iTemp92 = int(fVec2[1]);
			float fTemp93 = ((iTemp92) ? 0.0f : fTemp86 + fRec14[1]);
			fRec14[0] = fTemp93 - std::floor(fTemp93);
			float fTemp94 = float(fRec6[0] == 3.0f) * fTemp56;
			fVec3[0] = fTemp94;
			int iTemp95 = int(fTemp94);
			fRec16[0] = ((iTemp95) ? fTemp80 : fRec16[1]);
			float fTemp96 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec16[0]));
			fRec17[0] = ((iTemp95) ? fTemp79 : fRec17[1]);
			float fTemp97 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec17[0]));
			fRec15[0] = fVec3[2] - (fRec15[1] * (0.0f - (fTemp96 + fTemp97)) + fTemp96 * fTemp97 * fRec15[2]);
			int iTemp98 = int(fVec3[1]);
			float fTemp99 = ((iTemp98) ? 0.0f : fTemp86 + fRec18[1]);
			fRec18[0] = fTemp99 - std::floor(fTemp99);
			float fTemp100 = float(fRec6[0] == 4.0f) * fTemp56;
			fVec4[0] = fTemp100;
			int iTemp101 = int(fTemp100);
			fRec20[0] = ((iTemp101) ? fTemp80 : fRec20[1]);
			float fTemp102 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec20[0]));
			fRec21[0] = ((iTemp101) ? fTemp79 : fRec21[1]);
			float fTemp103 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec21[0]));
			fRec19[0] = fVec4[2] - (fRec19[1] * (0.0f - (fTemp102 + fTemp103)) + fTemp102 * fTemp103 * fRec19[2]);
			int iTemp104 = int(fVec4[1]);
			float fTemp105 = ((iTemp104) ? 0.0f : fTemp86 + fRec22[1]);
			fRec22[0] = fTemp105 - std::floor(fTemp105);
			float fTemp106 = float(fRec6[0] == 5.0f) * fTemp56;
			fVec5[0] = fTemp106;
			int iTemp107 = int(fTemp106);
			fRec24[0] = ((iTemp107) ? fTemp80 : fRec24[1]);
			float fTemp108 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec24[0]));
			fRec25[0] = ((iTemp107) ? fTemp79 : fRec25[1]);
			float fTemp109 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec25[0]));
			fRec23[0] = fVec5[2] - (fRec23[1] * (0.0f - (fTemp108 + fTemp109)) + fTemp108 * fTemp109 * fRec23[2]);
			int iTemp110 = int(fVec5[1]);
			float fTemp111 = ((iTemp110) ? 0.0f : fTemp86 + fRec26[1]);
			fRec26[0] = fTemp111 - std::floor(fTemp111);
			float fTemp112 = float(fRec6[0] == 6.0f) * fTemp56;
			fVec6[0] = fTemp112;
			int iTemp113 = int(fTemp112);
			fRec28[0] = ((iTemp113) ? fTemp80 : fRec28[1]);
			float fTemp114 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec28[0]));
			fRec29[0] = ((iTemp113) ? fTemp79 : fRec29[1]);
			float fTemp115 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec29[0]));
			fRec27[0] = fVec6[2] - (fRec27[1] * (0.0f - (fTemp114 + fTemp115)) + fTemp114 * fTemp115 * fRec27[2]);
			int iTemp116 = int(fVec6[1]);
			float fTemp117 = ((iTemp116) ? 0.0f : fTemp86 + fRec30[1]);
			fRec30[0] = fTemp117 - std::floor(fTemp117);
			float fTemp118 = float(fRec6[0] == 7.0f) * fTemp56;
			fVec7[0] = fTemp118;
			int iTemp119 = int(fTemp118);
			fRec32[0] = ((iTemp119) ? fTemp80 : fRec32[1]);
			float fTemp120 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec32[0]));
			fRec33[0] = ((iTemp119) ? fTemp79 : fRec33[1]);
			float fTemp121 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec33[0]));
			fRec31[0] = fVec7[2] - (fRec31[1] * (0.0f - (fTemp120 + fTemp121)) + fTemp120 * fTemp121 * fRec31[2]);
			int iTemp122 = int(fVec7[1]);
			float fTemp123 = ((iTemp122) ? 0.0f : fTemp86 + fRec34[1]);
			fRec34[0] = fTemp123 - std::floor(fTemp123);
			float fTemp124 = float(fRec6[0] == 8.0f) * fTemp56;
			fVec8[0] = fTemp124;
			int iTemp125 = int(fTemp124);
			fRec36[0] = ((iTemp125) ? fTemp80 : fRec36[1]);
			float fTemp126 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec36[0]));
			fRec37[0] = ((iTemp125) ? fTemp79 : fRec37[1]);
			float fTemp127 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec37[0]));
			fRec35[0] = fVec8[2] - (fRec35[1] * (0.0f - (fTemp126 + fTemp127)) + fTemp126 * fTemp127 * fRec35[2]);
			int iTemp128 = int(fVec8[1]);
			float fTemp129 = ((iTemp128) ? 0.0f : fTemp86 + fRec38[1]);
			fRec38[0] = fTemp129 - std::floor(fTemp129);
			float fTemp130 = float(fRec6[0] == 9.0f) * fTemp56;
			fVec9[0] = fTemp130;
			int iTemp131 = int(fTemp130);
			fRec40[0] = ((iTemp131) ? fTemp80 : fRec40[1]);
			float fTemp132 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec40[0]));
			fRec41[0] = ((iTemp131) ? fTemp79 : fRec41[1]);
			float fTemp133 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec41[0]));
			fRec39[0] = fVec9[2] - (fRec39[1] * (0.0f - (fTemp132 + fTemp133)) + fTemp132 * fTemp133 * fRec39[2]);
			int iTemp134 = int(fVec9[1]);
			float fTemp135 = ((iTemp134) ? 0.0f : fTemp86 + fRec42[1]);
			fRec42[0] = fTemp135 - std::floor(fTemp135);
			float fTemp136 = float(fRec6[0] == 1e+01f) * fTemp56;
			fVec10[0] = fTemp136;
			int iTemp137 = int(fTemp136);
			fRec44[0] = ((iTemp137) ? fTemp80 : fRec44[1]);
			float fTemp138 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec44[0]));
			fRec45[0] = ((iTemp137) ? fTemp79 : fRec45[1]);
			float fTemp139 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec45[0]));
			fRec43[0] = fVec10[2] - (fRec43[1] * (0.0f - (fTemp138 + fTemp139)) + fTemp138 * fTemp139 * fRec43[2]);
			int iTemp140 = int(fVec10[1]);
			float fTemp141 = ((iTemp140) ? 0.0f : fTemp86 + fRec46[1]);
			fRec46[0] = fTemp141 - std::floor(fTemp141);
			float fTemp142 = float(fRec6[0] == 11.0f) * fTemp56;
			fVec11[0] = fTemp142;
			int iTemp143 = int(fTemp142);
			fRec48[0] = ((iTemp143) ? fTemp80 : fRec48[1]);
			float fTemp144 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec48[0]));
			fRec49[0] = ((iTemp143) ? fTemp79 : fRec49[1]);
			float fTemp145 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec49[0]));
			fRec47[0] = fVec11[2] - (fRec47[1] * (0.0f - (fTemp144 + fTemp145)) + fTemp144 * fTemp145 * fRec47[2]);
			int iTemp146 = int(fVec11[1]);
			float fTemp147 = ((iTemp146) ? 0.0f : fTemp86 + fRec50[1]);
			fRec50[0] = fTemp147 - std::floor(fTemp147);
			float fTemp148 = float(fRec6[0] == 12.0f) * fTemp56;
			fVec12[0] = fTemp148;
			int iTemp149 = int(fTemp148);
			fRec52[0] = ((iTemp149) ? fTemp80 : fRec52[1]);
			float fTemp150 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec52[0]));
			fRec53[0] = ((iTemp149) ? fTemp79 : fRec53[1]);
			float fTemp151 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec53[0]));
			fRec51[0] = fVec12[2] - (fRec51[1] * (0.0f - (fTemp150 + fTemp151)) + fTemp150 * fTemp151 * fRec51[2]);
			int iTemp152 = int(fVec12[1]);
			float fTemp153 = ((iTemp152) ? 0.0f : fTemp86 + fRec54[1]);
			fRec54[0] = fTemp153 - std::floor(fTemp153);
			float fTemp154 = 2e+01f * fTemp11;
			float fTemp155 = 1e+01f * fTemp45;
			float fTemp156 = 2e+01f * fTemp47;
			float fTemp157 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 8e+01f : ((iTemp50) ? 8e+01f - fTemp78 : 6e+01f)) : ((iTemp48) ? 6e+01f - 1e+01f * fTemp51 : 5e+01f)) : ((iTemp46) ? fTemp77 + 5e+01f : 7e+01f)) : ((iTemp44) ? 7e+01f - fTemp156 : 5e+01f)) : ((iTemp42) ? fTemp155 + 5e+01f : 6e+01f)) : ((iTemp40) ? 6e+01f - 2e+01f * fTemp43 : 4e+01f)) : ((iTemp38) ? 2e+01f * fTemp41 + 4e+01f : 6e+01f)) : ((iTemp36) ? 6e+01f - 2e+01f * fTemp39 : 4e+01f)) : 4e+01f) : ((iTemp32) ? 4e+01f * (fTemp35 + 1.0f) : 8e+01f)) : ((iTemp29) ? 8e+01f - fTemp74 : 7e+01f)) : ((iTemp27) ? 7e+01f - 3e+01f * fTemp30 : 4e+01f)) : 4e+01f) : 4e+01f) : ((iTemp21) ? 4e+01f * (fTemp24 + 1.0f) : 8e+01f)) : ((iTemp19) ? 8e+01f - fTemp73 : 6e+01f)) : 6e+01f) : ((iTemp14) ? 6e+01f - 2e+01f * fTemp17 : 4e+01f)) : ((iTemp12) ? 1e+01f * fTemp15 + 4e+01f : 5e+01f)) : 5e+01f) : ((iTemp8) ? fTemp154 + 5e+01f : 7e+01f)) : ((iTemp6) ? 7e+01f - 3e+01f * fTemp9 : 4e+01f)) : ((iTemp3) ? 3e+01f * fTemp7 + 4e+01f : 7e+01f)) : ((iTemp4) ? 7e+01f - 3e+01f * fTemp5 : 4e+01f));
			float fTemp158 = fTemp69 * fTemp157;
			fRec56[0] = ((iTemp89) ? fTemp158 : fRec56[1]);
			float fTemp159 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec56[0]));
			fRec57[0] = ((iTemp89) ? fTemp157 : fRec57[1]);
			float fTemp160 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec57[0]));
			fRec55[0] = fVec2[2] - (fRec55[1] * (0.0f - (fTemp159 + fTemp160)) + fTemp159 * fTemp160 * fRec55[2]);
			float fTemp161 = 8e+01f * fTemp20;
			float fTemp162 = 5e+01f * fTemp37;
			float fTemp163 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 8e+02f : ((iTemp50) ? 8e+02f - 4e+02f * fTemp2 : 4e+02f)) : ((iTemp48) ? 4e+02f - 5e+01f * fTemp51 : 3.5e+02f)) : ((iTemp46) ? 1e+02f * fTemp49 + 3.5e+02f : 4.5e+02f)) : ((iTemp44) ? 4.5e+02f - 125.0f * fTemp47 : 325.0f)) : ((iTemp42) ? 275.0f * fTemp45 + 325.0f : 6e+02f)) : ((iTemp40) ? 6e+02f - 2e+02f * fTemp43 : 4e+02f)) : ((iTemp38) ? 4e+02f - 1.5e+02f * fTemp41 : 2.5e+02f)) : ((iTemp36) ? 1.5e+02f * fTemp39 + 2.5e+02f : 4e+02f)) : ((iTemp34) ? 4e+02f - fTemp162 : 3.5e+02f)) : ((iTemp32) ? 3.1e+02f * fTemp35 + 3.5e+02f : 6.6e+02f)) : ((iTemp29) ? 6.6e+02f - 2.2e+02f * fTemp33 : 4.4e+02f)) : ((iTemp27) ? 4.4e+02f - 1.7e+02f * fTemp30 : 2.7e+02f)) : ((iTemp25) ? 1.6e+02f * fTemp28 + 2.7e+02f : 4.3e+02f)) : ((iTemp23) ? 4.3e+02f - 6e+01f * fTemp26 : 3.7e+02f)) : ((iTemp21) ? 4.3e+02f * fTemp24 + 3.7e+02f : 8e+02f)) : ((iTemp19) ? 8e+02f - 4.5e+02f * fTemp22 : 3.5e+02f)) : ((iTemp16) ? 3.5e+02f - fTemp161 : 2.7e+02f)) : ((iTemp14) ? 1.8e+02f * fTemp17 + 2.7e+02f : 4.5e+02f)) : ((iTemp12) ? 4.5e+02f - 125.0f * fTemp15 : 325.0f)) : ((iTemp10) ? 325.0f * (fTemp13 + 1.0f) : 6.5e+02f)) : ((iTemp8) ? 6.5e+02f - 2.5e+02f * fTemp11 : 4e+02f)) : ((iTemp6) ? 4e+02f - 1.1e+02f * fTemp9 : 2.9e+02f)) : ((iTemp3) ? 1.1e+02f * fTemp7 + 2.9e+02f : 4e+02f)) : ((iTemp4) ? 4e+02f - 5e+01f * fTemp5 : 3.5e+02f));
			float fTemp164 = fConst3 * ((fTemp163 <= fTemp0) ? fTemp0 : fTemp163);
			float fTemp165 = ((iTemp92) ? 0.0f : fTemp164 + fRec58[1]);
			fRec58[0] = fTemp165 - std::floor(fTemp165);
			fRec60[0] = ((iTemp58) ? fTemp158 : fRec60[1]);
			float fTemp166 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec60[0]));
			fRec61[0] = ((iTemp58) ? fTemp157 : fRec61[1]);
			float fTemp167 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec61[0]));
			fRec59[0] = fVec1[2] - (fRec59[1] * (0.0f - (fTemp166 + fTemp167)) + fTemp166 * fTemp167 * fRec59[2]);
			float fTemp168 = ((iTemp83) ? 0.0f : fRec62[1] + fTemp164);
			fRec62[0] = fTemp168 - std::floor(fTemp168);
			fRec64[0] = ((iTemp95) ? fTemp158 : fRec64[1]);
			float fTemp169 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec64[0]));
			fRec65[0] = ((iTemp95) ? fTemp157 : fRec65[1]);
			float fTemp170 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec65[0]));
			fRec63[0] = fVec3[2] - (fRec63[1] * (0.0f - (fTemp169 + fTemp170)) + fTemp169 * fTemp170 * fRec63[2]);
			float fTemp171 = ((iTemp98) ? 0.0f : fTemp164 + fRec66[1]);
			fRec66[0] = fTemp171 - std::floor(fTemp171);
			fRec68[0] = ((iTemp101) ? fTemp158 : fRec68[1]);
			float fTemp172 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec68[0]));
			fRec69[0] = ((iTemp101) ? fTemp157 : fRec69[1]);
			float fTemp173 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec69[0]));
			fRec67[0] = fVec4[2] - (fRec67[1] * (0.0f - (fTemp172 + fTemp173)) + fTemp172 * fTemp173 * fRec67[2]);
			float fTemp174 = ((iTemp104) ? 0.0f : fTemp164 + fRec70[1]);
			fRec70[0] = fTemp174 - std::floor(fTemp174);
			fRec72[0] = ((iTemp107) ? fTemp158 : fRec72[1]);
			float fTemp175 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec72[0]));
			fRec73[0] = ((iTemp107) ? fTemp157 : fRec73[1]);
			float fTemp176 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec73[0]));
			fRec71[0] = fVec5[2] - (fRec71[1] * (0.0f - (fTemp175 + fTemp176)) + fTemp175 * fTemp176 * fRec71[2]);
			float fTemp177 = ((iTemp110) ? 0.0f : fTemp164 + fRec74[1]);
			fRec74[0] = fTemp177 - std::floor(fTemp177);
			fRec76[0] = ((iTemp113) ? fTemp158 : fRec76[1]);
			float fTemp178 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec76[0]));
			fRec77[0] = ((iTemp113) ? fTemp157 : fRec77[1]);
			float fTemp179 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec77[0]));
			fRec75[0] = fVec6[2] - (fRec75[1] * (0.0f - (fTemp178 + fTemp179)) + fTemp178 * fTemp179 * fRec75[2]);
			float fTemp180 = ((iTemp116) ? 0.0f : fTemp164 + fRec78[1]);
			fRec78[0] = fTemp180 - std::floor(fTemp180);
			fRec80[0] = ((iTemp119) ? fTemp158 : fRec80[1]);
			float fTemp181 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec80[0]));
			fRec81[0] = ((iTemp119) ? fTemp157 : fRec81[1]);
			float fTemp182 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec81[0]));
			fRec79[0] = fVec7[2] - (fRec79[1] * (0.0f - (fTemp181 + fTemp182)) + fTemp181 * fTemp182 * fRec79[2]);
			float fTemp183 = ((iTemp122) ? 0.0f : fTemp164 + fRec82[1]);
			fRec82[0] = fTemp183 - std::floor(fTemp183);
			fRec84[0] = ((iTemp125) ? fTemp158 : fRec84[1]);
			float fTemp184 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec84[0]));
			fRec85[0] = ((iTemp125) ? fTemp157 : fRec85[1]);
			float fTemp185 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec85[0]));
			fRec83[0] = fVec8[2] - (fRec83[1] * (0.0f - (fTemp184 + fTemp185)) + fTemp184 * fTemp185 * fRec83[2]);
			float fTemp186 = ((iTemp128) ? 0.0f : fTemp164 + fRec86[1]);
			fRec86[0] = fTemp186 - std::floor(fTemp186);
			fRec88[0] = ((iTemp131) ? fTemp158 : fRec88[1]);
			float fTemp187 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec88[0]));
			fRec89[0] = ((iTemp131) ? fTemp157 : fRec89[1]);
			float fTemp188 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec89[0]));
			fRec87[0] = fVec9[2] - (fRec87[1] * (0.0f - (fTemp187 + fTemp188)) + fTemp187 * fTemp188 * fRec87[2]);
			float fTemp189 = ((iTemp134) ? 0.0f : fTemp164 + fRec90[1]);
			fRec90[0] = fTemp189 - std::floor(fTemp189);
			fRec92[0] = ((iTemp137) ? fTemp158 : fRec92[1]);
			float fTemp190 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec92[0]));
			fRec93[0] = ((iTemp137) ? fTemp157 : fRec93[1]);
			float fTemp191 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec93[0]));
			fRec91[0] = fVec10[2] - (fRec91[1] * (0.0f - (fTemp190 + fTemp191)) + fTemp190 * fTemp191 * fRec91[2]);
			float fTemp192 = ((iTemp140) ? 0.0f : fTemp164 + fRec94[1]);
			fRec94[0] = fTemp192 - std::floor(fTemp192);
			fRec96[0] = ((iTemp143) ? fTemp158 : fRec96[1]);
			float fTemp193 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec96[0]));
			fRec97[0] = ((iTemp143) ? fTemp157 : fRec97[1]);
			float fTemp194 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec97[0]));
			fRec95[0] = fVec11[2] - (fRec95[1] * (0.0f - (fTemp193 + fTemp194)) + fTemp193 * fTemp194 * fRec95[2]);
			float fTemp195 = ((iTemp146) ? 0.0f : fTemp164 + fRec98[1]);
			fRec98[0] = fTemp195 - std::floor(fTemp195);
			fRec100[0] = ((iTemp149) ? fTemp158 : fRec100[1]);
			float fTemp196 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec100[0]));
			fRec101[0] = ((iTemp149) ? fTemp157 : fRec101[1]);
			float fTemp197 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec101[0]));
			fRec99[0] = fVec12[2] - (fRec99[1] * (0.0f - (fTemp196 + fTemp197)) + fTemp196 * fTemp197 * fRec99[2]);
			float fTemp198 = ((iTemp152) ? 0.0f : fTemp164 + fRec102[1]);
			fRec102[0] = fTemp198 - std::floor(fTemp198);
			float fTemp199 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 0.630957f : ((iTemp50) ? 0.630957f - 0.567861f * fTemp2 : 0.063096f)) : ((iTemp48) ? 0.036904f * fTemp51 + 0.063096f : 0.1f)) : ((iTemp46) ? 0.254813f * fTemp49 + 0.1f : 0.354813f)) : ((iTemp44) ? 0.354813f - 0.103624f * fTemp47 : 0.251189f)) : ((iTemp42) ? 0.195495f * fTemp45 + 0.251189f : 0.446684f)) : ((iTemp40) ? 0.446684f - 0.195495f * fTemp43 : 0.251189f)) : ((iTemp38) ? 0.251189f - 0.219566f * fTemp41 : 0.031623f)) : ((iTemp36) ? 0.250215f * fTemp39 + 0.031623f : 0.281838f)) : ((iTemp34) ? 0.281838f - 0.181838f * fTemp37 : 0.1f)) : ((iTemp32) ? 0.401187f * fTemp35 + 0.1f : 0.501187f)) : ((iTemp29) ? 0.501187f - 0.301661f * fTemp33 : 0.199526f)) : ((iTemp27) ? 0.199526f - 0.13643f * fTemp30 : 0.063096f)) : ((iTemp25) ? 0.253132f * fTemp28 + 0.063096f : 0.316228f)) : ((iTemp23) ? 0.316228f - 0.216228f * fTemp26 : 0.1f)) : ((iTemp21) ? 0.401187f * fTemp24 + 0.1f : 0.501187f)) : ((iTemp19) ? 0.501187f - 0.401187f * fTemp22 : 0.1f)) : ((iTemp16) ? 0.151189f * fTemp20 + 0.1f : 0.251189f)) : ((iTemp14) ? 0.030649f * fTemp17 + 0.251189f : 0.281838f)) : ((iTemp12) ? 0.281838f - 0.123349f * fTemp15 : 0.158489f)) : ((iTemp10) ? 0.342698f * fTemp13 + 0.158489f : 0.501187f)) : ((iTemp8) ? 0.501187f - 0.301661f * fTemp11 : 0.199526f)) : ((iTemp6) ? 0.199526f - 0.021698f * fTemp9 : 0.177828f)) : ((iTemp3) ? 0.1384f * fTemp7 + 0.177828f : 0.316228f)) : ((iTemp4) ? 0.316228f - 0.216228f * fTemp5 : 0.1f));
			float fTemp200 = 1e+01f * fTemp43;
			float fTemp201 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 9e+01f : ((iTemp50) ? 9e+01f - 1e+01f * fTemp2 : 8e+01f)) : ((iTemp48) ? 2e+01f * fTemp51 + 8e+01f : 1e+02f)) : ((iTemp46) ? 1e+02f - fTemp77 : 8e+01f)) : ((iTemp44) ? 8e+01f - fTemp156 : 6e+01f)) : ((iTemp42) ? fTemp155 + 6e+01f : 7e+01f)) : ((iTemp40) ? fTemp200 + 7e+01f : 8e+01f)) : ((iTemp38) ? 1e+01f * fTemp41 + 8e+01f : 9e+01f)) : ((iTemp36) ? 9e+01f - 1e+01f * fTemp39 : 8e+01f)) : 8e+01f) : ((iTemp32) ? fTemp75 + 8e+01f : 9e+01f)) : ((iTemp29) ? 9e+01f - fTemp74 : 8e+01f)) : ((iTemp27) ? 1e+01f * fTemp30 + 8e+01f : 9e+01f)) : ((iTemp25) ? 9e+01f - 1e+01f * fTemp28 : 8e+01f)) : ((iTemp23) ? 8e+01f - 2e+01f * fTemp26 : 6e+01f)) : ((iTemp21) ? 3e+01f * fTemp24 + 6e+01f : 9e+01f)) : ((iTemp19) ? 1e+01f * fTemp22 + 9e+01f : 1e+02f)) : ((iTemp16) ? 1e+02f - 1e+01f * fTemp20 : 9e+01f)) : ((iTemp14) ? 9e+01f - 1e+01f * fTemp17 : 8e+01f)) : ((iTemp12) ? 8e+01f - 2e+01f * fTemp15 : 6e+01f)) : ((iTemp10) ? 3e+01f * fTemp13 + 6e+01f : 9e+01f)) : ((iTemp8) ? 9e+01f - fTemp71 : 8e+01f)) : ((iTemp6) ? 1e+01f * fTemp9 + 8e+01f : 9e+01f)) : ((iTemp3) ? 9e+01f - fTemp70 : 8e+01f)) : ((iTemp4) ? 8e+01f - 2e+01f * fTemp5 : 6e+01f));
			float fTemp202 = fTemp69 * fTemp201;
			fRec104[0] = ((iTemp58) ? fTemp202 : fRec104[1]);
			float fTemp203 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec104[0]));
			fRec105[0] = ((iTemp58) ? fTemp201 : fRec105[1]);
			float fTemp204 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec105[0]));
			fRec103[0] = fVec1[2] - (fRec103[1] * (0.0f - (fTemp203 + fTemp204)) + fTemp203 * fTemp204 * fRec103[2]);
			float fTemp205 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 1.15e+03f : ((iTemp50) ? 4.5e+02f * fTemp2 + 1.15e+03f : 1.6e+03f)) : ((iTemp48) ? 1e+02f * fTemp51 + 1.6e+03f : 1.7e+03f)) : ((iTemp46) ? 1.7e+03f - 9e+02f * fTemp49 : 8e+02f)) : ((iTemp44) ? 8e+02f - 1e+02f * fTemp47 : 7e+02f)) : ((iTemp42) ? 3.4e+02f * fTemp45 + 7e+02f : 1.04e+03f)) : ((iTemp40) ? 5.8e+02f * fTemp43 + 1.04e+03f : 1.62e+03f)) : ((iTemp38) ? 1.3e+02f * fTemp41 + 1.62e+03f : 1.75e+03f)) : ((iTemp36) ? 1.75e+03f - 1e+03f * fTemp39 : 7.5e+02f)) : ((iTemp34) ? 7.5e+02f - 1.5e+02f * fTemp37 : 6e+02f)) : ((iTemp32) ? 5.2e+02f * fTemp35 + 6e+02f : 1.12e+03f)) : ((iTemp29) ? 6.8e+02f * fTemp33 + 1.12e+03f : 1.8e+03f)) : ((iTemp27) ? 5e+01f * fTemp30 + 1.8e+03f : 1.85e+03f)) : ((iTemp25) ? 1.85e+03f - 1.03e+03f * fTemp28 : 8.2e+02f)) : ((iTemp23) ? 8.2e+02f - 1.9e+02f * fTemp26 : 6.3e+02f)) : ((iTemp21) ? 5.2e+02f * fTemp24 + 6.3e+02f : 1.15e+03f)) : ((iTemp19) ? 8.5e+02f * fTemp22 + 1.15e+03f : 2e+03f)) : ((iTemp16) ? 1.4e+02f * fTemp20 + 2e+03f : 2.14e+03f)) : ((iTemp14) ? 2.14e+03f - 1.34e+03f * fTemp17 : 8e+02f)) : ((iTemp12) ? 8e+02f - 1e+02f * fTemp15 : 7e+02f)) : ((iTemp10) ? 3.8e+02f * fTemp13 + 7e+02f : 1.08e+03f)) : ((iTemp8) ? 6.2e+02f * fTemp11 + 1.08e+03f : 1.7e+03f)) : ((iTemp6) ? 1.7e+02f * fTemp9 + 1.7e+03f : 1.87e+03f)) : ((iTemp3) ? 1.87e+03f - 1.07e+03f * fTemp7 : 8e+02f)) : ((iTemp4) ? 8e+02f - 2e+02f * fTemp5 : 6e+02f));
			float fTemp206 = 2.0f * fTemp0 + 3e+01f;
			float fTemp207 = fConst3 * ((iSlow20) ? (((fTemp205 >= 1.3e+03f) & (fTemp0 >= 2e+02f)) ? fTemp205 - 0.00095238094f * (fTemp0 + -2e+02f) * (fTemp205 + -1.3e+03f) : ((fTemp205 <= fTemp206) ? fTemp206 : fTemp205)) : fTemp205);
			float fTemp208 = ((iTemp83) ? 0.0f : fRec106[1] + fTemp207);
			fRec106[0] = fTemp208 - std::floor(fTemp208);
			fRec108[0] = ((iTemp89) ? fTemp202 : fRec108[1]);
			float fTemp209 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec108[0]));
			fRec109[0] = ((iTemp89) ? fTemp201 : fRec109[1]);
			float fTemp210 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec109[0]));
			fRec107[0] = fVec2[2] - (fRec107[1] * (0.0f - (fTemp209 + fTemp210)) + fTemp209 * fTemp210 * fRec107[2]);
			float fTemp211 = ((iTemp92) ? 0.0f : fTemp207 + fRec110[1]);
			fRec110[0] = fTemp211 - std::floor(fTemp211);
			fRec112[0] = ((iTemp95) ? fTemp202 : fRec112[1]);
			float fTemp212 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec112[0]));
			fRec113[0] = ((iTemp95) ? fTemp201 : fRec113[1]);
			float fTemp213 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec113[0]));
			fRec111[0] = fVec3[2] - (fRec111[1] * (0.0f - (fTemp212 + fTemp213)) + fTemp212 * fTemp213 * fRec111[2]);
			float fTemp214 = ((iTemp98) ? 0.0f : fTemp207 + fRec114[1]);
			fRec114[0] = fTemp214 - std::floor(fTemp214);
			fRec116[0] = ((iTemp101) ? fTemp202 : fRec116[1]);
			float fTemp215 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec116[0]));
			fRec117[0] = ((iTemp101) ? fTemp201 : fRec117[1]);
			float fTemp216 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec117[0]));
			fRec115[0] = fVec4[2] - (fRec115[1] * (0.0f - (fTemp215 + fTemp216)) + fTemp215 * fTemp216 * fRec115[2]);
			float fTemp217 = ((iTemp104) ? 0.0f : fTemp207 + fRec118[1]);
			fRec118[0] = fTemp217 - std::floor(fTemp217);
			fRec120[0] = ((iTemp107) ? fTemp202 : fRec120[1]);
			float fTemp218 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec120[0]));
			fRec121[0] = ((iTemp107) ? fTemp201 : fRec121[1]);
			float fTemp219 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec121[0]));
			fRec119[0] = fVec5[2] - (fRec119[1] * (0.0f - (fTemp218 + fTemp219)) + fTemp218 * fTemp219 * fRec119[2]);
			float fTemp220 = ((iTemp110) ? 0.0f : fTemp207 + fRec122[1]);
			fRec122[0] = fTemp220 - std::floor(fTemp220);
			fRec124[0] = ((iTemp113) ? fTemp202 : fRec124[1]);
			float fTemp221 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec124[0]));
			fRec125[0] = ((iTemp113) ? fTemp201 : fRec125[1]);
			float fTemp222 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec125[0]));
			fRec123[0] = fVec6[2] - (fRec123[1] * (0.0f - (fTemp221 + fTemp222)) + fTemp221 * fTemp222 * fRec123[2]);
			float fTemp223 = ((iTemp116) ? 0.0f : fTemp207 + fRec126[1]);
			fRec126[0] = fTemp223 - std::floor(fTemp223);
			fRec128[0] = ((iTemp119) ? fTemp202 : fRec128[1]);
			float fTemp224 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec128[0]));
			fRec129[0] = ((iTemp119) ? fTemp201 : fRec129[1]);
			float fTemp225 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec129[0]));
			fRec127[0] = fVec7[2] - (fRec127[1] * (0.0f - (fTemp224 + fTemp225)) + fTemp224 * fTemp225 * fRec127[2]);
			float fTemp226 = ((iTemp122) ? 0.0f : fTemp207 + fRec130[1]);
			fRec130[0] = fTemp226 - std::floor(fTemp226);
			fRec132[0] = ((iTemp125) ? fTemp202 : fRec132[1]);
			float fTemp227 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec132[0]));
			fRec133[0] = ((iTemp125) ? fTemp201 : fRec133[1]);
			float fTemp228 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec133[0]));
			fRec131[0] = fVec8[2] - (fRec131[1] * (0.0f - (fTemp227 + fTemp228)) + fTemp227 * fTemp228 * fRec131[2]);
			float fTemp229 = ((iTemp128) ? 0.0f : fTemp207 + fRec134[1]);
			fRec134[0] = fTemp229 - std::floor(fTemp229);
			fRec136[0] = ((iTemp131) ? fTemp202 : fRec136[1]);
			float fTemp230 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec136[0]));
			fRec137[0] = ((iTemp131) ? fTemp201 : fRec137[1]);
			float fTemp231 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec137[0]));
			fRec135[0] = fVec9[2] - (fRec135[1] * (0.0f - (fTemp230 + fTemp231)) + fTemp230 * fTemp231 * fRec135[2]);
			float fTemp232 = ((iTemp134) ? 0.0f : fTemp207 + fRec138[1]);
			fRec138[0] = fTemp232 - std::floor(fTemp232);
			fRec140[0] = ((iTemp137) ? fTemp202 : fRec140[1]);
			float fTemp233 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec140[0]));
			fRec141[0] = ((iTemp137) ? fTemp201 : fRec141[1]);
			float fTemp234 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec141[0]));
			fRec139[0] = fVec10[2] - (fRec139[1] * (0.0f - (fTemp233 + fTemp234)) + fTemp233 * fTemp234 * fRec139[2]);
			float fTemp235 = ((iTemp140) ? 0.0f : fTemp207 + fRec142[1]);
			fRec142[0] = fTemp235 - std::floor(fTemp235);
			fRec144[0] = ((iTemp143) ? fTemp202 : fRec144[1]);
			float fTemp236 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec144[0]));
			fRec145[0] = ((iTemp143) ? fTemp201 : fRec145[1]);
			float fTemp237 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec145[0]));
			fRec143[0] = fVec11[2] - (fRec143[1] * (0.0f - (fTemp236 + fTemp237)) + fTemp236 * fTemp237 * fRec143[2]);
			float fTemp238 = ((iTemp146) ? 0.0f : fTemp207 + fRec146[1]);
			fRec146[0] = fTemp238 - std::floor(fTemp238);
			fRec148[0] = ((iTemp149) ? fTemp202 : fRec148[1]);
			float fTemp239 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec148[0]));
			fRec149[0] = ((iTemp149) ? fTemp201 : fRec149[1]);
			float fTemp240 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec149[0]));
			fRec147[0] = fVec12[2] - (fRec147[1] * (0.0f - (fTemp239 + fTemp240)) + fTemp239 * fTemp240 * fRec147[2]);
			float fTemp241 = ((iTemp152) ? 0.0f : fTemp207 + fRec150[1]);
			fRec150[0] = fTemp241 - std::floor(fTemp241);
			float fTemp242 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 0.1f : ((iTemp50) ? 0.1f - 0.068377f * fTemp2 : 0.031623f)) : 0.031623f) : ((iTemp46) ? 0.126866f * fTemp49 + 0.031623f : 0.158489f)) : ((iTemp44) ? 0.158489f - 0.126866f * fTemp47 : 0.031623f)) : ((iTemp42) ? 0.32319f * fTemp45 + 0.031623f : 0.354813f)) : 0.354813f) : ((iTemp38) ? 0.354813f - 0.196324f * fTemp41 : 0.158489f)) : ((iTemp36) ? 0.158489f - 0.069364f * fTemp39 : 0.089125f)) : ((iTemp34) ? 0.089125f - 0.064006f * fTemp37 : 0.025119f)) : ((iTemp32) ? 0.045676f * fTemp35 + 0.025119f : 0.070795f)) : ((iTemp29) ? 0.055098f * fTemp33 + 0.070795f : 0.125893f)) : ((iTemp27) ? 0.125893f - 0.062797f * fTemp30 : 0.063096f)) : ((iTemp25) ? 0.063096f - 0.012977f * fTemp28 : 0.050119f)) : ((iTemp23) ? 0.020676f * fTemp26 + 0.050119f : 0.070795f)) : ((iTemp21) ? 0.070795f - 0.045676f * fTemp24 : 0.025119f)) : ((iTemp19) ? 0.152709f * fTemp22 + 0.025119f : 0.177828f)) : ((iTemp16) ? 0.177828f - 0.127709f * fTemp20 : 0.050119f)) : fTemp18) : ((iTemp12) ? 0.079433f - 0.06165f * fTemp15 : 0.017783f)) : ((iTemp10) ? 0.428901f * fTemp13 + 0.017783f : 0.446684f)) : ((iTemp8) ? 0.446684f - 0.195495f * fTemp11 : 0.251189f)) : ((iTemp6) ? 0.251189f - 0.125296f * fTemp9 : 0.125893f)) : ((iTemp3) ? 0.125296f * fTemp7 + 0.125893f : 0.251189f)) : ((iTemp4) ? 0.251189f - 0.109935f * fTemp5 : 0.141254f));
			float fTemp243 = 2e+01f * fTemp24;
			float fTemp244 = 2e+01f * fTemp33;
			float fTemp245 = 2e+01f * fTemp35;
			float fTemp246 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? 1.2e+02f : ((iTemp46) ? 1.2e+02f - fTemp77 : 1e+02f)) : ((iTemp44) ? 7e+01f * fTemp47 + 1e+02f : 1.7e+02f)) : ((iTemp42) ? 1.7e+02f - fTemp76 : 1.1e+02f)) : ((iTemp40) ? 1.1e+02f - fTemp200 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp32) ? fTemp245 + 1e+02f : 1.2e+02f)) : ((iTemp29) ? 1.2e+02f - fTemp244 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp21) ? fTemp243 + 1e+02f : 1.2e+02f)) : 1.2e+02f) : ((iTemp16) ? 1.2e+02f - 2e+01f * fTemp20 : 1e+02f)) : 1e+02f) : ((iTemp12) ? 7e+01f * fTemp15 + 1e+02f : 1.7e+02f)) : ((iTemp10) ? 1.7e+02f - fTemp72 : 1.2e+02f)) : ((iTemp8) ? 1.2e+02f - fTemp154 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f);
			float fTemp247 = fTemp69 * fTemp246;
			fRec152[0] = ((iTemp58) ? fTemp247 : fRec152[1]);
			float fTemp248 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec152[0]));
			fRec153[0] = ((iTemp58) ? fTemp246 : fRec153[1]);
			float fTemp249 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec153[0]));
			fRec151[0] = fVec1[2] - (fRec151[1] * (0.0f - (fTemp248 + fTemp249)) + fTemp248 * fTemp249 * fRec151[2]);
			float fTemp250 = 5e+01f * fTemp33;
			float fTemp251 = fConst3 * ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 2.8e+03f : ((iTemp50) ? 2.8e+03f - 1e+02f * fTemp2 : 2.7e+03f)) : 2.7e+03f) : ((iTemp46) ? 1.3e+02f * fTemp49 + 2.7e+03f : 2.83e+03f)) : ((iTemp44) ? 2.83e+03f - 3e+02f * fTemp47 : 2.53e+03f)) : ((iTemp42) ? 2.53e+03f - 2.8e+02f * fTemp45 : 2.25e+03f)) : ((iTemp40) ? 1.5e+02f * fTemp43 + 2.25e+03f : 2.4e+03f)) : ((iTemp38) ? 2e+02f * fTemp41 + 2.4e+03f : 2.6e+03f)) : ((iTemp36) ? 2.6e+03f - 2e+02f * fTemp39 : 2.4e+03f)) : 2.4e+03f) : ((iTemp32) ? 3.5e+02f * fTemp35 + 2.4e+03f : 2.75e+03f)) : ((iTemp29) ? 2.75e+03f - fTemp250 : 2.7e+03f)) : ((iTemp27) ? 2e+02f * fTemp30 + 2.7e+03f : 2.9e+03f)) : ((iTemp25) ? 2.9e+03f - 2e+02f * fTemp28 : 2.7e+03f)) : ((iTemp23) ? 5e+01f * fTemp26 + 2.7e+03f : 2.75e+03f)) : ((iTemp21) ? 1.5e+02f * fTemp24 + 2.75e+03f : 2.9e+03f)) : ((iTemp19) ? 2.9e+03f - 1e+02f * fTemp22 : 2.8e+03f)) : ((iTemp16) ? 1.5e+02f * fTemp20 + 2.8e+03f : 2.95e+03f)) : ((iTemp14) ? 2.95e+03f - 1.2e+02f * fTemp17 : 2.83e+03f)) : ((iTemp12) ? 2.83e+03f - 1.3e+02f * fTemp15 : 2.7e+03f)) : ((iTemp10) ? 2.7e+03f - fTemp72 : 2.65e+03f)) : ((iTemp8) ? 2.65e+03f - 5e+01f * fTemp11 : 2.6e+03f)) : ((iTemp6) ? 2e+02f * fTemp9 + 2.6e+03f : 2.8e+03f)) : ((iTemp3) ? 2.8e+03f - 2e+02f * fTemp7 : 2.6e+03f)) : ((iTemp4) ? fTemp84 + 2.6e+03f : 2.7e+03f));
			float fTemp252 = ((iTemp83) ? 0.0f : fRec154[1] + fTemp251);
			fRec154[0] = fTemp252 - std::floor(fTemp252);
			fRec156[0] = ((iTemp89) ? fTemp247 : fRec156[1]);
			float fTemp253 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec156[0]));
			fRec157[0] = ((iTemp89) ? fTemp246 : fRec157[1]);
			float fTemp254 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec157[0]));
			fRec155[0] = fVec2[2] - (fRec155[1] * (0.0f - (fTemp253 + fTemp254)) + fTemp253 * fTemp254 * fRec155[2]);
			float fTemp255 = ((iTemp92) ? 0.0f : fTemp251 + fRec158[1]);
			fRec158[0] = fTemp255 - std::floor(fTemp255);
			fRec160[0] = ((iTemp95) ? fTemp247 : fRec160[1]);
			float fTemp256 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec160[0]));
			fRec161[0] = ((iTemp95) ? fTemp246 : fRec161[1]);
			float fTemp257 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec161[0]));
			fRec159[0] = fVec3[2] - (fRec159[1] * (0.0f - (fTemp256 + fTemp257)) + fTemp256 * fTemp257 * fRec159[2]);
			float fTemp258 = ((iTemp98) ? 0.0f : fTemp251 + fRec162[1]);
			fRec162[0] = fTemp258 - std::floor(fTemp258);
			fRec164[0] = ((iTemp101) ? fTemp247 : fRec164[1]);
			float fTemp259 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec164[0]));
			fRec165[0] = ((iTemp101) ? fTemp246 : fRec165[1]);
			float fTemp260 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec165[0]));
			fRec163[0] = fVec4[2] - (fRec163[1] * (0.0f - (fTemp259 + fTemp260)) + fTemp259 * fTemp260 * fRec163[2]);
			float fTemp261 = ((iTemp104) ? 0.0f : fTemp251 + fRec166[1]);
			fRec166[0] = fTemp261 - std::floor(fTemp261);
			fRec168[0] = ((iTemp107) ? fTemp247 : fRec168[1]);
			float fTemp262 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec168[0]));
			fRec169[0] = ((iTemp107) ? fTemp246 : fRec169[1]);
			float fTemp263 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec169[0]));
			fRec167[0] = fVec5[2] - (fRec167[1] * (0.0f - (fTemp262 + fTemp263)) + fTemp262 * fTemp263 * fRec167[2]);
			float fTemp264 = ((iTemp110) ? 0.0f : fTemp251 + fRec170[1]);
			fRec170[0] = fTemp264 - std::floor(fTemp264);
			fRec172[0] = ((iTemp113) ? fTemp247 : fRec172[1]);
			float fTemp265 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec172[0]));
			fRec173[0] = ((iTemp113) ? fTemp246 : fRec173[1]);
			float fTemp266 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec173[0]));
			fRec171[0] = fVec6[2] - (fRec171[1] * (0.0f - (fTemp265 + fTemp266)) + fTemp265 * fTemp266 * fRec171[2]);
			float fTemp267 = ((iTemp116) ? 0.0f : fTemp251 + fRec174[1]);
			fRec174[0] = fTemp267 - std::floor(fTemp267);
			fRec176[0] = ((iTemp119) ? fTemp247 : fRec176[1]);
			float fTemp268 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec176[0]));
			fRec177[0] = ((iTemp119) ? fTemp246 : fRec177[1]);
			float fTemp269 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec177[0]));
			fRec175[0] = fVec7[2] - (fRec175[1] * (0.0f - (fTemp268 + fTemp269)) + fTemp268 * fTemp269 * fRec175[2]);
			float fTemp270 = ((iTemp122) ? 0.0f : fTemp251 + fRec178[1]);
			fRec178[0] = fTemp270 - std::floor(fTemp270);
			fRec180[0] = ((iTemp125) ? fTemp247 : fRec180[1]);
			float fTemp271 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec180[0]));
			fRec181[0] = ((iTemp125) ? fTemp246 : fRec181[1]);
			float fTemp272 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec181[0]));
			fRec179[0] = fVec8[2] - (fRec179[1] * (0.0f - (fTemp271 + fTemp272)) + fTemp271 * fTemp272 * fRec179[2]);
			float fTemp273 = ((iTemp128) ? 0.0f : fTemp251 + fRec182[1]);
			fRec182[0] = fTemp273 - std::floor(fTemp273);
			fRec184[0] = ((iTemp131) ? fTemp247 : fRec184[1]);
			float fTemp274 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec184[0]));
			fRec185[0] = ((iTemp131) ? fTemp246 : fRec185[1]);
			float fTemp275 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec185[0]));
			fRec183[0] = fVec9[2] - (fRec183[1] * (0.0f - (fTemp274 + fTemp275)) + fTemp274 * fTemp275 * fRec183[2]);
			float fTemp276 = ((iTemp134) ? 0.0f : fTemp251 + fRec186[1]);
			fRec186[0] = fTemp276 - std::floor(fTemp276);
			fRec188[0] = ((iTemp137) ? fTemp247 : fRec188[1]);
			float fTemp277 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec188[0]));
			fRec189[0] = ((iTemp137) ? fTemp246 : fRec189[1]);
			float fTemp278 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec189[0]));
			fRec187[0] = fVec10[2] - (fRec187[1] * (0.0f - (fTemp277 + fTemp278)) + fTemp277 * fTemp278 * fRec187[2]);
			float fTemp279 = ((iTemp140) ? 0.0f : fTemp251 + fRec190[1]);
			fRec190[0] = fTemp279 - std::floor(fTemp279);
			fRec192[0] = ((iTemp143) ? fTemp247 : fRec192[1]);
			float fTemp280 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec192[0]));
			fRec193[0] = ((iTemp143) ? fTemp246 : fRec193[1]);
			float fTemp281 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec193[0]));
			fRec191[0] = fVec11[2] - (fRec191[1] * (0.0f - (fTemp280 + fTemp281)) + fTemp280 * fTemp281 * fRec191[2]);
			float fTemp282 = ((iTemp146) ? 0.0f : fTemp251 + fRec194[1]);
			fRec194[0] = fTemp282 - std::floor(fTemp282);
			fRec196[0] = ((iTemp149) ? fTemp247 : fRec196[1]);
			float fTemp283 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec196[0]));
			fRec197[0] = ((iTemp149) ? fTemp246 : fRec197[1]);
			float fTemp284 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec197[0]));
			fRec195[0] = fVec12[2] - (fRec195[1] * (0.0f - (fTemp283 + fTemp284)) + fTemp283 * fTemp284 * fRec195[2]);
			float fTemp285 = ((iTemp152) ? 0.0f : fTemp251 + fRec198[1]);
			fRec198[0] = fTemp285 - std::floor(fTemp285);
			float fTemp286 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? 0.001f : ((iTemp46) ? 0.000778f * fTemp49 + 0.001f : 0.001778f)) : ((iTemp44) ? 0.001778f - 0.001147f * fTemp47 : 0.000631f)) : ((iTemp42) ? 0.099369f * fTemp45 + 0.000631f : 0.1f)) : ((iTemp40) ? 0.025893f * fTemp43 + 0.1f : 0.125893f)) : ((iTemp38) ? 0.125893f - 0.086082f * fTemp41 : 0.039811f)) : ((iTemp36) ? 0.039811f - 0.029811f * fTemp39 : 0.01f)) : ((iTemp34) ? 0.005849f * fTemp37 + 0.01f : 0.015849f)) : ((iTemp32) ? 0.015849f - 0.00326f * fTemp35 : 0.012589f)) : ((iTemp29) ? 0.087411f * fTemp33 + 0.012589f : 0.1f)) : fTemp31) : ((iTemp25) ? 0.004104f * fTemp28 + 0.015849f : 0.019953f)) : 0.019953f) : ((iTemp21) ? 0.019953f - 0.016791f * fTemp24 : 0.003162f)) : ((iTemp19) ? 0.003162f - 0.001577f * fTemp22 : 0.001585f)) : ((iTemp16) ? 0.004725f * fTemp20 + 0.001585f : 0.00631f)) : ((iTemp14) ? 0.00631f - 0.003148f * fTemp17 : 0.003162f)) : ((iTemp12) ? 0.003162f - 0.002162f * fTemp15 : 0.001f)) : ((iTemp10) ? 0.078433f * fTemp13 + 0.001f : 0.079433f)) : ((iTemp8) ? 0.020567f * fTemp11 + 0.079433f : 0.1f)) : ((iTemp6) ? 0.1f - 0.068377f * fTemp9 : 0.031623f)) : ((iTemp3) ? 0.018496f * fTemp7 + 0.031623f : 0.050119f)) : 0.050119f);
			float fTemp287 = ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 1.4e+02f : ((iTemp50) ? 6e+01f * fTemp2 + 1.4e+02f : 2e+02f)) : 2e+02f) : ((iTemp46) ? 2e+02f - 65.0f * fTemp49 : 135.0f)) : ((iTemp44) ? 65.0f * fTemp47 + 135.0f : 2e+02f)) : ((iTemp42) ? 2e+02f - 7e+01f * fTemp45 : 1.3e+02f)) : ((iTemp40) ? 1.3e+02f - fTemp200 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp32) ? fTemp245 + 1.2e+02f : 1.4e+02f)) : ((iTemp29) ? 1.4e+02f - fTemp244 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp21) ? fTemp243 + 1.2e+02f : 1.4e+02f)) : ((iTemp19) ? 6e+01f * fTemp22 + 1.4e+02f : 2e+02f)) : ((iTemp16) ? 2e+02f - fTemp161 : 1.2e+02f)) : 1.2e+02f) : ((iTemp12) ? 8e+01f * fTemp15 + 1.2e+02f : 2e+02f)) : ((iTemp10) ? 2e+02f - 6e+01f * fTemp13 : 1.4e+02f)) : ((iTemp8) ? 1.4e+02f - fTemp154 : 1.2e+02f)) : 1.2e+02f) : ((iTemp3) ? 15.0f * fTemp7 + 1.2e+02f : 135.0f)) : ((iTemp4) ? 135.0f - 15.0f * fTemp5 : 1.2e+02f));
			float fTemp288 = fTemp69 * fTemp287;
			fRec200[0] = ((iTemp58) ? fTemp288 : fRec200[1]);
			float fTemp289 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec200[0]));
			fRec201[0] = ((iTemp58) ? fTemp287 : fRec201[1]);
			float fTemp290 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec201[0]));
			fRec199[0] = fVec1[2] - (fRec199[1] * (0.0f - (fTemp289 + fTemp290)) + fTemp289 * fTemp290 * fRec199[2]);
			float fTemp291 = fConst3 * ((iTemp3) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iTemp25) ? ((iTemp27) ? ((iTemp29) ? ((iTemp32) ? ((iTemp34) ? ((iTemp36) ? ((iTemp38) ? ((iTemp40) ? ((iTemp42) ? ((iTemp44) ? 4.95e+03f : ((iTemp42) ? 4.95e+03f - 2.2e+03f * fTemp45 : 2.75e+03f)) : ((iTemp40) ? fTemp85 + 2.75e+03f : 3.1e+03f)) : ((iTemp38) ? 2.4e+02f * fTemp41 + 3.1e+03f : 3.34e+03f)) : ((iTemp36) ? 3.34e+03f - 4.4e+02f * fTemp39 : 2.9e+03f)) : ((iTemp34) ? fTemp162 + 2.9e+03f : 2.95e+03f)) : ((iTemp32) ? 4e+02f * fTemp35 + 2.95e+03f : 3.35e+03f)) : ((iTemp29) ? 3.35e+03f - fTemp250 : 3.3e+03f)) : ((iTemp27) ? 2.9e+02f * fTemp30 + 3.3e+03f : 3.59e+03f)) : ((iTemp25) ? 3.59e+03f - 2.9e+02f * fTemp28 : 3.3e+03f)) : ((iTemp23) ? 1e+02f * fTemp26 + 3.3e+03f : 3.4e+03f)) : ((iTemp21) ? 1.55e+03f * fTemp24 + 3.4e+03f : 4.95e+03f)) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : ((iTemp10) ? 4.95e+03f - 1.7e+03f * fTemp13 : 3.25e+03f)) : ((iTemp8) ? 3.3e+02f * fTemp11 + 3.25e+03f : 3.58e+03f)) : ((iTemp6) ? 3.58e+03f - 4e+01f * fTemp9 : 3.54e+03f)) : ((iTemp3) ? 3.54e+03f - 5.4e+02f * fTemp7 : 3e+03f)) : ((iTemp4) ? 3e+02f * fTemp5 + 3e+03f : 3.3e+03f));
			float fTemp292 = ((iTemp83) ? 0.0f : fRec202[1] + fTemp291);
			fRec202[0] = fTemp292 - std::floor(fTemp292);
			fRec204[0] = ((iTemp89) ? fTemp288 : fRec204[1]);
			float fTemp293 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec204[0]));
			fRec205[0] = ((iTemp89) ? fTemp287 : fRec205[1]);
			float fTemp294 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec205[0]));
			fRec203[0] = fVec2[2] - (fRec203[1] * (0.0f - (fTemp293 + fTemp294)) + fTemp293 * fTemp294 * fRec203[2]);
			float fTemp295 = ((iTemp92) ? 0.0f : fTemp291 + fRec206[1]);
			fRec206[0] = fTemp295 - std::floor(fTemp295);
			fRec208[0] = ((iTemp95) ? fTemp288 : fRec208[1]);
			float fTemp296 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec208[0]));
			fRec209[0] = ((iTemp95) ? fTemp287 : fRec209[1]);
			float fTemp297 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec209[0]));
			fRec207[0] = fVec3[2] - (fRec207[1] * (0.0f - (fTemp296 + fTemp297)) + fTemp296 * fTemp297 * fRec207[2]);
			float fTemp298 = ((iTemp98) ? 0.0f : fTemp291 + fRec210[1]);
			fRec210[0] = fTemp298 - std::floor(fTemp298);
			fRec212[0] = ((iTemp101) ? fTemp288 : fRec212[1]);
			float fTemp299 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec212[0]));
			fRec213[0] = ((iTemp101) ? fTemp287 : fRec213[1]);
			float fTemp300 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec213[0]));
			fRec211[0] = fVec4[2] - (fRec211[1] * (0.0f - (fTemp299 + fTemp300)) + fTemp299 * fTemp300 * fRec211[2]);
			float fTemp301 = ((iTemp104) ? 0.0f : fTemp291 + fRec214[1]);
			fRec214[0] = fTemp301 - std::floor(fTemp301);
			fRec216[0] = ((iTemp107) ? fTemp288 : fRec216[1]);
			float fTemp302 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec216[0]));
			fRec217[0] = ((iTemp107) ? fTemp287 : fRec217[1]);
			float fTemp303 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec217[0]));
			fRec215[0] = fVec5[2] - (fRec215[1] * (0.0f - (fTemp302 + fTemp303)) + fTemp302 * fTemp303 * fRec215[2]);
			float fTemp304 = ((iTemp110) ? 0.0f : fTemp291 + fRec218[1]);
			fRec218[0] = fTemp304 - std::floor(fTemp304);
			fRec220[0] = ((iTemp113) ? fTemp288 : fRec220[1]);
			float fTemp305 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec220[0]));
			fRec221[0] = ((iTemp113) ? fTemp287 : fRec221[1]);
			float fTemp306 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec221[0]));
			fRec219[0] = fVec6[2] - (fRec219[1] * (0.0f - (fTemp305 + fTemp306)) + fTemp305 * fTemp306 * fRec219[2]);
			float fTemp307 = ((iTemp116) ? 0.0f : fTemp291 + fRec222[1]);
			fRec222[0] = fTemp307 - std::floor(fTemp307);
			fRec224[0] = ((iTemp119) ? fTemp288 : fRec224[1]);
			float fTemp308 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec224[0]));
			fRec225[0] = ((iTemp119) ? fTemp287 : fRec225[1]);
			float fTemp309 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec225[0]));
			fRec223[0] = fVec7[2] - (fRec223[1] * (0.0f - (fTemp308 + fTemp309)) + fTemp308 * fTemp309 * fRec223[2]);
			float fTemp310 = ((iTemp122) ? 0.0f : fTemp291 + fRec226[1]);
			fRec226[0] = fTemp310 - std::floor(fTemp310);
			fRec228[0] = ((iTemp125) ? fTemp288 : fRec228[1]);
			float fTemp311 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec228[0]));
			fRec229[0] = ((iTemp125) ? fTemp287 : fRec229[1]);
			float fTemp312 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec229[0]));
			fRec227[0] = fVec8[2] - (fRec227[1] * (0.0f - (fTemp311 + fTemp312)) + fTemp311 * fTemp312 * fRec227[2]);
			float fTemp313 = ((iTemp128) ? 0.0f : fTemp291 + fRec230[1]);
			fRec230[0] = fTemp313 - std::floor(fTemp313);
			fRec232[0] = ((iTemp131) ? fTemp288 : fRec232[1]);
			float fTemp314 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec232[0]));
			fRec233[0] = ((iTemp131) ? fTemp287 : fRec233[1]);
			float fTemp315 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec233[0]));
			fRec231[0] = fVec9[2] - (fRec231[1] * (0.0f - (fTemp314 + fTemp315)) + fTemp314 * fTemp315 * fRec231[2]);
			float fTemp316 = ((iTemp134) ? 0.0f : fTemp291 + fRec234[1]);
			fRec234[0] = fTemp316 - std::floor(fTemp316);
			fRec236[0] = ((iTemp137) ? fTemp288 : fRec236[1]);
			float fTemp317 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec236[0]));
			fRec237[0] = ((iTemp137) ? fTemp287 : fRec237[1]);
			float fTemp318 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec237[0]));
			fRec235[0] = fVec10[2] - (fRec235[1] * (0.0f - (fTemp317 + fTemp318)) + fTemp317 * fTemp318 * fRec235[2]);
			float fTemp319 = ((iTemp140) ? 0.0f : fTemp291 + fRec238[1]);
			fRec238[0] = fTemp319 - std::floor(fTemp319);
			fRec240[0] = ((iTemp143) ? fTemp288 : fRec240[1]);
			float fTemp320 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec240[0]));
			fRec241[0] = ((iTemp143) ? fTemp287 : fRec241[1]);
			float fTemp321 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec241[0]));
			fRec239[0] = fVec11[2] - (fRec239[1] * (0.0f - (fTemp320 + fTemp321)) + fTemp320 * fTemp321 * fRec239[2]);
			float fTemp322 = ((iTemp146) ? 0.0f : fTemp291 + fRec242[1]);
			fRec242[0] = fTemp322 - std::floor(fTemp322);
			fRec244[0] = ((iTemp149) ? fTemp288 : fRec244[1]);
			float fTemp323 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec244[0]));
			fRec245[0] = ((iTemp149) ? fTemp287 : fRec245[1]);
			float fTemp324 = std::exp(fConst3 * (0.0f - 3.1415927f * fRec245[0]));
			fRec243[0] = fVec12[2] - (fRec243[1] * (0.0f - (fTemp323 + fTemp324)) + fTemp323 * fTemp324 * fRec243[2]);
			float fTemp325 = ((iTemp152) ? 0.0f : fTemp291 + fRec246[1]);
			fRec246[0] = fTemp325 - std::floor(fTemp325);
			fVec13[0] = fSlow22;
			fRec247[0] = fSlow22 + fRec247[1] * float(fVec13[1] >= fSlow22);
			iRec248[0] = iSlow24 * (iRec248[1] + 1);
			output0[i0] = FAUSTFLOAT(5e+01f * (((iSlow2) ? fTemp54 * fTemp53 : fTemp1 * fTemp53) * (fRec5[0] * (1.0f - (fTemp82 + fTemp81 * (1.0f - fTemp82))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec10[0])] + fRec11[0] * (1.0f - (fTemp91 + fTemp90 * (1.0f - fTemp91))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec14[0])] + fRec15[0] * (1.0f - (fTemp97 + fTemp96 * (1.0f - fTemp97))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec18[0])] + fRec19[0] * (1.0f - (fTemp103 + fTemp102 * (1.0f - fTemp103))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec22[0])] + fRec23[0] * (1.0f - (fTemp109 + fTemp108 * (1.0f - fTemp109))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec26[0])] + fRec27[0] * (1.0f - (fTemp115 + fTemp114 * (1.0f - fTemp115))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec30[0])] + fRec31[0] * (1.0f - (fTemp121 + fTemp120 * (1.0f - fTemp121))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec34[0])] + fRec35[0] * (1.0f - (fTemp127 + fTemp126 * (1.0f - fTemp127))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec38[0])] + fRec39[0] * (1.0f - (fTemp133 + fTemp132 * (1.0f - fTemp133))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec42[0])] + fRec43[0] * (1.0f - (fTemp139 + fTemp138 * (1.0f - fTemp139))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec46[0])] + fRec47[0] * (1.0f - (fTemp145 + fTemp144 * (1.0f - fTemp145))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec50[0])] + fRec51[0] * (1.0f - (fTemp151 + fTemp150 * (1.0f - fTemp151))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec54[0])]) + ((iSlow2) ? fTemp54 : fTemp1) * (fRec55[0] * (1.0f - (fTemp160 + fTemp159 * (1.0f - fTemp160))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec58[0])] + fRec59[0] * (1.0f - (fTemp167 + fTemp166 * (1.0f - fTemp167))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec62[0])] + fRec63[0] * (1.0f - (fTemp170 + fTemp169 * (1.0f - fTemp170))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec66[0])] + fRec67[0] * (1.0f - (fTemp173 + fTemp172 * (1.0f - fTemp173))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec70[0])] + fRec71[0] * (1.0f - (fTemp176 + fTemp175 * (1.0f - fTemp176))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec74[0])] + fRec75[0] * (1.0f - (fTemp179 + fTemp178 * (1.0f - fTemp179))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec78[0])] + fRec79[0] * (1.0f - (fTemp182 + fTemp181 * (1.0f - fTemp182))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec82[0])] + fRec83[0] * (1.0f - (fTemp185 + fTemp184 * (1.0f - fTemp185))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec86[0])] + fRec87[0] * (1.0f - (fTemp188 + fTemp187 * (1.0f - fTemp188))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec90[0])] + fRec91[0] * (1.0f - (fTemp191 + fTemp190 * (1.0f - fTemp191))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec94[0])] + fRec95[0] * (1.0f - (fTemp194 + fTemp193 * (1.0f - fTemp194))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec98[0])] + fRec99[0] * (1.0f - (fTemp197 + fTemp196 * (1.0f - fTemp197))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec102[0])]) + ((iSlow2) ? fTemp54 * fTemp199 : fTemp1 * fTemp199) * (fRec103[0] * (1.0f - (fTemp204 + fTemp203 * (1.0f - fTemp204))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec106[0])] + fRec107[0] * (1.0f - (fTemp210 + fTemp209 * (1.0f - fTemp210))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec110[0])] + fRec111[0] * (1.0f - (fTemp213 + fTemp212 * (1.0f - fTemp213))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec114[0])] + fRec115[0] * (1.0f - (fTemp216 + fTemp215 * (1.0f - fTemp216))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec118[0])] + fRec119[0] * (1.0f - (fTemp219 + fTemp218 * (1.0f - fTemp219))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec122[0])] + fRec123[0] * (1.0f - (fTemp222 + fTemp221 * (1.0f - fTemp222))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec126[0])] + fRec127[0] * (1.0f - (fTemp225 + fTemp224 * (1.0f - fTemp225))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec130[0])] + fRec131[0] * (1.0f - (fTemp227 + fTemp228 * (1.0f - fTemp227))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec134[0])] + fRec135[0] * (1.0f - (fTemp231 + fTemp230 * (1.0f - fTemp231))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec138[0])] + fRec139[0] * (1.0f - (fTemp234 + fTemp233 * (1.0f - fTemp234))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec142[0])] + fRec143[0] * (1.0f - (fTemp237 + fTemp236 * (1.0f - fTemp237))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec146[0])] + fRec147[0] * (1.0f - (fTemp240 + fTemp239 * (1.0f - fTemp240))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec150[0])]) + ((iSlow2) ? fTemp54 * fTemp242 : fTemp1 * fTemp242) * (fRec151[0] * (fTemp248 * (fTemp249 + -1.0f) + (1.0f - fTemp249)) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec154[0])] + fRec155[0] * (1.0f - (fTemp254 + fTemp253 * (1.0f - fTemp254))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec158[0])] + fRec159[0] * (1.0f - (fTemp257 + fTemp256 * (1.0f - fTemp257))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec162[0])] + fRec163[0] * (1.0f - (fTemp260 + fTemp259 * (1.0f - fTemp260))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec166[0])] + fRec167[0] * (1.0f - (fTemp263 + fTemp262 * (1.0f - fTemp263))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec170[0])] + fRec171[0] * (1.0f - (fTemp266 + fTemp265 * (1.0f - fTemp266))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec174[0])] + fRec175[0] * (1.0f - (fTemp269 + fTemp268 * (1.0f - fTemp269))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec178[0])] + fRec179[0] * (1.0f - (fTemp272 + fTemp271 * (1.0f - fTemp272))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec182[0])] + fRec183[0] * (1.0f - (fTemp275 + fTemp274 * (1.0f - fTemp275))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec186[0])] + fRec187[0] * (1.0f - (fTemp278 + fTemp277 * (1.0f - fTemp278))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec190[0])] + fRec191[0] * (1.0f - (fTemp281 + fTemp280 * (1.0f - fTemp281))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec194[0])] + fRec195[0] * (1.0f - (fTemp284 + fTemp283 * (1.0f - fTemp284))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec198[0])]) + ((iSlow2) ? fTemp54 * fTemp286 : fTemp1 * fTemp286) * (fRec199[0] * (1.0f - (fTemp290 + fTemp289 * (1.0f - fTemp290))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec202[0])] + fRec203[0] * (1.0f - (fTemp294 + fTemp293 * (1.0f - fTemp294))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec206[0])] + fRec207[0] * (1.0f - (fTemp297 + fTemp296 * (1.0f - fTemp297))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec210[0])] + fRec211[0] * (1.0f - (fTemp300 + fTemp299 * (1.0f - fTemp300))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec214[0])] + fRec215[0] * (1.0f - (fTemp303 + fTemp302 * (1.0f - fTemp303))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec218[0])] + fRec219[0] * (1.0f - (fTemp306 + fTemp305 * (1.0f - fTemp306))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec222[0])] + fRec223[0] * (1.0f - (fTemp308 + fTemp309 * (1.0f - fTemp308))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec226[0])] + fRec227[0] * (1.0f - (fTemp312 + fTemp311 * (1.0f - fTemp312))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec230[0])] + fRec231[0] * (1.0f - (fTemp315 + fTemp314 * (1.0f - fTemp315))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec234[0])] + fRec235[0] * (1.0f - (fTemp318 + fTemp317 * (1.0f - fTemp318))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec238[0])] + fRec239[0] * (1.0f - (fTemp321 + fTemp320 * (1.0f - fTemp321))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec242[0])] + fRec243[0] * (1.0f - (fTemp324 + fTemp323 * (1.0f - fTemp324))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec246[0])])) * std::max<float>(0.0f, std::min<float>(fSlow21 * fRec247[0], 1.0f) - fSlow23 * float(iRec248[0])));
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec7[1] = fRec7[0];
			fRec6[1] = fRec6[0];
			fVec1[2] = fVec1[1];
			fVec1[1] = fVec1[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec10[1] = fRec10[0];
			fVec2[2] = fVec2[1];
			fVec2[1] = fVec2[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec14[1] = fRec14[0];
			fVec3[2] = fVec3[1];
			fVec3[1] = fVec3[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec18[1] = fRec18[0];
			fVec4[2] = fVec4[1];
			fVec4[1] = fVec4[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec22[1] = fRec22[0];
			fVec5[2] = fVec5[1];
			fVec5[1] = fVec5[0];
			fRec24[1] = fRec24[0];
			fRec25[1] = fRec25[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec26[1] = fRec26[0];
			fVec6[2] = fVec6[1];
			fVec6[1] = fVec6[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec30[1] = fRec30[0];
			fVec7[2] = fVec7[1];
			fVec7[1] = fVec7[0];
			fRec32[1] = fRec32[0];
			fRec33[1] = fRec33[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec34[1] = fRec34[0];
			fVec8[2] = fVec8[1];
			fVec8[1] = fVec8[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec38[1] = fRec38[0];
			fVec9[2] = fVec9[1];
			fVec9[1] = fVec9[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec42[1] = fRec42[0];
			fVec10[2] = fVec10[1];
			fVec10[1] = fVec10[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec46[1] = fRec46[0];
			fVec11[2] = fVec11[1];
			fVec11[1] = fVec11[0];
			fRec48[1] = fRec48[0];
			fRec49[1] = fRec49[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec50[1] = fRec50[0];
			fVec12[2] = fVec12[1];
			fVec12[1] = fVec12[0];
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
			fVec13[1] = fVec13[0];
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
