/* ------------------------------------------------------------
author: "Mayank Sanganeria"
name: "fx.granulator"
version: "1.0"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_granulator -scn fx_granulator_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_granulator_H__
#define  __fx_granulator_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_granulator_dsp.h ********************************
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

class FAUST_API fx_granulator_dsp {

    public:

        fx_granulator_dsp() {}
        virtual ~fx_granulator_dsp() {}

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
        virtual fx_granulator_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_granulator_dsp {

    protected:

        fx_granulator_dsp* fDSP;

    public:

        decorator_dsp(fx_granulator_dsp* fx_granulator_dsp = nullptr):fDSP(fx_granulator_dsp) {}
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
    
        virtual fx_granulator_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_granulator_dsp.h **************************/
/************************** BEGIN UI.h *****************************
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
 ********************************************************************/

#ifndef __UI_H__
#define __UI_H__


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct FAUST_API UIReal {
    
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* /*zone*/, const char* /*key*/, const char* /*val*/) {}

    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct FAUST_API UI : public UIReal<FAUSTFLOAT> {
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
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
 The base class of Meta handler to be used in fx_granulator_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct FAUST_API Meta {
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/
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
struct fx_granulator : public fx_granulator_dsp {
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
#define FAUSTCLASS fx_granulator
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

class fx_granulatorSIG0 {
	
  private:
	
	
  public:
	
	int getNumInputsfx_granulatorSIG0() {
		return 0;
	}
	int getNumOutputsfx_granulatorSIG0() {
		return 1;
	}
	
	void instanceInitfx_granulatorSIG0(int sample_rate) {
	}
	
	void fillfx_granulatorSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			table[i1] = 0.0f;
		}
	}

};

static fx_granulatorSIG0* newfx_granulatorSIG0() { return (fx_granulatorSIG0*)new fx_granulatorSIG0(); }
static void deletefx_granulatorSIG0(fx_granulatorSIG0* dsp) { delete dsp; }

class fx_granulatorSIG1 {
	
  private:
	
	int iRec70[2];
	
  public:
	
	int getNumInputsfx_granulatorSIG1() {
		return 0;
	}
	int getNumOutputsfx_granulatorSIG1() {
		return 1;
	}
	
	void instanceInitfx_granulatorSIG1(int sample_rate) {
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			iRec70[l8] = 0;
		}
	}
	
	void fillfx_granulatorSIG1(int count, float* table) {
		for (int i2 = 0; i2 < count; i2 = i2 + 1) {
			iRec70[0] = iRec70[1] + 1;
			table[i2] = 0.5f * (1.0f - std::cos(0.006141921f * float(iRec70[0] + -1)));
			iRec70[1] = iRec70[0];
		}
	}

};

static fx_granulatorSIG1* newfx_granulatorSIG1() { return (fx_granulatorSIG1*)new fx_granulatorSIG1(); }
static void deletefx_granulatorSIG1(fx_granulatorSIG1* dsp) { delete dsp; }

static float ftbl1fx_granulatorSIG1[1024];

class fx_granulator : public fx_granulator_dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	float ftbl0[480000];
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider1;
	float fConst2;
	float fRec1[2];
	int iRec0[2];
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fRec4[2];
	int iRec3[2];
	int iRec6[2];
	int iRec5[2];
	int iRec2[2];
	int iRec72[2];
	int iRec71[2];
	int iRec74[2];
	int iRec73[2];
	int iRec76[2];
	int iRec75[2];
	int iRec78[2];
	int iRec77[2];
	int iRec80[2];
	int iRec79[2];
	int iRec82[2];
	int iRec81[2];
	int iRec84[2];
	int iRec83[2];
	int iRec86[2];
	int iRec85[2];
	int iRec88[2];
	int iRec87[2];
	int iRec90[2];
	int iRec89[2];
	int iRec92[2];
	int iRec91[2];
	int iRec94[2];
	int iRec93[2];
	int iRec96[2];
	int iRec95[2];
	int iRec98[2];
	int iRec97[2];
	int iRec100[2];
	int iRec99[2];
	int iRec102[2];
	int iRec101[2];
	int iRec104[2];
	int iRec103[2];
	int iRec106[2];
	int iRec105[2];
	int iRec108[2];
	int iRec107[2];
	int iRec110[2];
	int iRec109[2];
	int iRec112[2];
	int iRec111[2];
	int iRec114[2];
	int iRec113[2];
	int iRec116[2];
	int iRec115[2];
	int iRec118[2];
	int iRec117[2];
	int iRec120[2];
	int iRec119[2];
	int iRec122[2];
	int iRec121[2];
	int iRec124[2];
	int iRec123[2];
	int iRec126[2];
	int iRec125[2];
	int iRec128[2];
	int iRec127[2];
	int iRec130[2];
	int iRec129[2];
	int iRec132[2];
	int iRec131[2];
	int iRec134[2];
	int iRec133[2];
	int iRec136[2];
	int iRec135[2];
	int iRec138[2];
	int iRec137[2];
	int iRec140[2];
	int iRec139[2];
	int iRec142[2];
	int iRec141[2];
	int iRec144[2];
	int iRec143[2];
	int iRec146[2];
	int iRec145[2];
	int iRec148[2];
	int iRec147[2];
	int iRec150[2];
	int iRec149[2];
	int iRec152[2];
	int iRec151[2];
	int iRec154[2];
	int iRec153[2];
	int iRec156[2];
	int iRec155[2];
	int iRec158[2];
	int iRec157[2];
	int iRec160[2];
	int iRec159[2];
	int iRec162[2];
	int iRec161[2];
	int iRec164[2];
	int iRec163[2];
	int iRec166[2];
	int iRec165[2];
	int iRec168[2];
	int iRec167[2];
	int iRec170[2];
	int iRec169[2];
	int iRec172[2];
	int iRec171[2];
	int iRec174[2];
	int iRec173[2];
	int iRec176[2];
	int iRec175[2];
	int iRec178[2];
	int iRec177[2];
	int iRec180[2];
	int iRec179[2];
	int iRec182[2];
	int iRec181[2];
	int iRec184[2];
	int iRec183[2];
	int iRec186[2];
	int iRec185[2];
	int iRec188[2];
	int iRec187[2];
	int iRec190[2];
	int iRec189[2];
	int iRec192[2];
	int iRec191[2];
	int iRec194[2];
	int iRec193[2];
	int iRec196[2];
	int iRec195[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Mayank Sanganeria");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_granulator -scn fx_granulator_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "fx_granulator.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "fx.granulator");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		fx_granulatorSIG1* sig1 = newfx_granulatorSIG1();
		sig1->instanceInitfx_granulatorSIG1(sample_rate);
		sig1->fillfx_granulatorSIG1(1024, ftbl1fx_granulatorSIG1);
		deletefx_granulatorSIG1(sig1);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fx_granulatorSIG0* sig0 = newfx_granulatorSIG0();
		sig0->instanceInitfx_granulatorSIG0(sample_rate);
		sig0->fillfx_granulatorSIG0(480000, ftbl0);
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 0.0441f / fConst0;
		deletefx_granulatorSIG0(sig0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1e+01f);
		fHslider1 = FAUSTFLOAT(1e+01f);
		fHslider2 = FAUSTFLOAT(1e+02f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iRec0[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec3[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec6[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iRec5[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iRec2[l7] = 0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			iRec72[l9] = 0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			iRec71[l10] = 0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			iRec74[l11] = 0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			iRec73[l12] = 0;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			iRec76[l13] = 0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			iRec75[l14] = 0;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			iRec78[l15] = 0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			iRec77[l16] = 0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			iRec80[l17] = 0;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			iRec79[l18] = 0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			iRec82[l19] = 0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			iRec81[l20] = 0;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			iRec84[l21] = 0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			iRec83[l22] = 0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			iRec86[l23] = 0;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			iRec85[l24] = 0;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			iRec88[l25] = 0;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			iRec87[l26] = 0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			iRec90[l27] = 0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			iRec89[l28] = 0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			iRec92[l29] = 0;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			iRec91[l30] = 0;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			iRec94[l31] = 0;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			iRec93[l32] = 0;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			iRec96[l33] = 0;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			iRec95[l34] = 0;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			iRec98[l35] = 0;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			iRec97[l36] = 0;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			iRec100[l37] = 0;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			iRec99[l38] = 0;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			iRec102[l39] = 0;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			iRec101[l40] = 0;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			iRec104[l41] = 0;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			iRec103[l42] = 0;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			iRec106[l43] = 0;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			iRec105[l44] = 0;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			iRec108[l45] = 0;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			iRec107[l46] = 0;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			iRec110[l47] = 0;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			iRec109[l48] = 0;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			iRec112[l49] = 0;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			iRec111[l50] = 0;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			iRec114[l51] = 0;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			iRec113[l52] = 0;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			iRec116[l53] = 0;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			iRec115[l54] = 0;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			iRec118[l55] = 0;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			iRec117[l56] = 0;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			iRec120[l57] = 0;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			iRec119[l58] = 0;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			iRec122[l59] = 0;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			iRec121[l60] = 0;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			iRec124[l61] = 0;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			iRec123[l62] = 0;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			iRec126[l63] = 0;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			iRec125[l64] = 0;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			iRec128[l65] = 0;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			iRec127[l66] = 0;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			iRec130[l67] = 0;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			iRec129[l68] = 0;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			iRec132[l69] = 0;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			iRec131[l70] = 0;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			iRec134[l71] = 0;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			iRec133[l72] = 0;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			iRec136[l73] = 0;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			iRec135[l74] = 0;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			iRec138[l75] = 0;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			iRec137[l76] = 0;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			iRec140[l77] = 0;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			iRec139[l78] = 0;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			iRec142[l79] = 0;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			iRec141[l80] = 0;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			iRec144[l81] = 0;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			iRec143[l82] = 0;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			iRec146[l83] = 0;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			iRec145[l84] = 0;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			iRec148[l85] = 0;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			iRec147[l86] = 0;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			iRec150[l87] = 0;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			iRec149[l88] = 0;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			iRec152[l89] = 0;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			iRec151[l90] = 0;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			iRec154[l91] = 0;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			iRec153[l92] = 0;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			iRec156[l93] = 0;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			iRec155[l94] = 0;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			iRec158[l95] = 0;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			iRec157[l96] = 0;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			iRec160[l97] = 0;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			iRec159[l98] = 0;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			iRec162[l99] = 0;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			iRec161[l100] = 0;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			iRec164[l101] = 0;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			iRec163[l102] = 0;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			iRec166[l103] = 0;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			iRec165[l104] = 0;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			iRec168[l105] = 0;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			iRec167[l106] = 0;
		}
		for (int l107 = 0; l107 < 2; l107 = l107 + 1) {
			iRec170[l107] = 0;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			iRec169[l108] = 0;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			iRec172[l109] = 0;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			iRec171[l110] = 0;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			iRec174[l111] = 0;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			iRec173[l112] = 0;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			iRec176[l113] = 0;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			iRec175[l114] = 0;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			iRec178[l115] = 0;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			iRec177[l116] = 0;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			iRec180[l117] = 0;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			iRec179[l118] = 0;
		}
		for (int l119 = 0; l119 < 2; l119 = l119 + 1) {
			iRec182[l119] = 0;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			iRec181[l120] = 0;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			iRec184[l121] = 0;
		}
		for (int l122 = 0; l122 < 2; l122 = l122 + 1) {
			iRec183[l122] = 0;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			iRec186[l123] = 0;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			iRec185[l124] = 0;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			iRec188[l125] = 0;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			iRec187[l126] = 0;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			iRec190[l127] = 0;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			iRec189[l128] = 0;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			iRec192[l129] = 0;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			iRec191[l130] = 0;
		}
		for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
			iRec194[l131] = 0;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			iRec193[l132] = 0;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			iRec196[l133] = 0;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			iRec195[l134] = 0;
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
	
	virtual fx_granulator* clone() {
		return new fx_granulator();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.granulator");
		ui_interface->declare(&fHslider1, "unit", "sec");
		ui_interface->addHorizontalSlider("delay", &fHslider1, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.5f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider0, "type", "int");
		ui_interface->addHorizontalSlider("density", &fHslider0, FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f), FAUSTFLOAT(64.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider2, "unit", "ms");
		ui_interface->addHorizontalSlider("length", &fHslider2, FAUSTFLOAT(1e+02f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fHslider0));
		float fSlow1 = 2.0f / float(iSlow0);
		float fSlow2 = float(0 < iSlow0);
		float fSlow3 = fConst1 * float(fHslider1);
		float fSlow4 = fConst3 * float(fHslider2);
		float fSlow5 = float(2 < iSlow0);
		float fSlow6 = float(4 < iSlow0);
		float fSlow7 = float(6 < iSlow0);
		float fSlow8 = float(8 < iSlow0);
		float fSlow9 = float(10 < iSlow0);
		float fSlow10 = float(12 < iSlow0);
		float fSlow11 = float(14 < iSlow0);
		float fSlow12 = float(16 < iSlow0);
		float fSlow13 = float(18 < iSlow0);
		float fSlow14 = float(20 < iSlow0);
		float fSlow15 = float(22 < iSlow0);
		float fSlow16 = float(24 < iSlow0);
		float fSlow17 = float(26 < iSlow0);
		float fSlow18 = float(28 < iSlow0);
		float fSlow19 = float(30 < iSlow0);
		float fSlow20 = float(32 < iSlow0);
		float fSlow21 = float(34 < iSlow0);
		float fSlow22 = float(36 < iSlow0);
		float fSlow23 = float(38 < iSlow0);
		float fSlow24 = float(40 < iSlow0);
		float fSlow25 = float(42 < iSlow0);
		float fSlow26 = float(44 < iSlow0);
		float fSlow27 = float(46 < iSlow0);
		float fSlow28 = float(48 < iSlow0);
		float fSlow29 = float(50 < iSlow0);
		float fSlow30 = float(52 < iSlow0);
		float fSlow31 = float(54 < iSlow0);
		float fSlow32 = float(56 < iSlow0);
		float fSlow33 = float(58 < iSlow0);
		float fSlow34 = float(60 < iSlow0);
		float fSlow35 = float(62 < iSlow0);
		float fSlow36 = float(1 < iSlow0);
		float fSlow37 = float(3 < iSlow0);
		float fSlow38 = float(5 < iSlow0);
		float fSlow39 = float(7 < iSlow0);
		float fSlow40 = float(9 < iSlow0);
		float fSlow41 = float(11 < iSlow0);
		float fSlow42 = float(13 < iSlow0);
		float fSlow43 = float(15 < iSlow0);
		float fSlow44 = float(17 < iSlow0);
		float fSlow45 = float(19 < iSlow0);
		float fSlow46 = float(21 < iSlow0);
		float fSlow47 = float(23 < iSlow0);
		float fSlow48 = float(25 < iSlow0);
		float fSlow49 = float(27 < iSlow0);
		float fSlow50 = float(29 < iSlow0);
		float fSlow51 = float(31 < iSlow0);
		float fSlow52 = float(33 < iSlow0);
		float fSlow53 = float(35 < iSlow0);
		float fSlow54 = float(37 < iSlow0);
		float fSlow55 = float(39 < iSlow0);
		float fSlow56 = float(41 < iSlow0);
		float fSlow57 = float(43 < iSlow0);
		float fSlow58 = float(45 < iSlow0);
		float fSlow59 = float(47 < iSlow0);
		float fSlow60 = float(49 < iSlow0);
		float fSlow61 = float(51 < iSlow0);
		float fSlow62 = float(53 < iSlow0);
		float fSlow63 = float(55 < iSlow0);
		float fSlow64 = float(57 < iSlow0);
		float fSlow65 = float(59 < iSlow0);
		float fSlow66 = float(61 < iSlow0);
		float fSlow67 = float(63 < iSlow0);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec1[0] = fSlow3 + fConst2 * fRec1[1];
			int iTemp0 = int(fConst0 * fRec1[0]);
			iRec0[0] = (iRec0[1] + 1) % iTemp0;
			ftbl0[iRec0[0] % iTemp0] = float(input0[i0]);
			fRec4[0] = fSlow4 + fConst2 * fRec4[1];
			int iTemp1 = int(fConst0 * fRec4[0]);
			iRec3[0] = (iRec3[1] + 1) % iTemp1;
			int iTemp2 = 1 - iVec0[1];
			float fTemp3 = float(iTemp0);
			int iTemp4 = 1103515245 * (iRec6[1] + 12345);
			int iTemp5 = 1103515245 * (iTemp4 + 12345);
			int iTemp6 = 1103515245 * (iTemp5 + 12345);
			int iTemp7 = 1103515245 * (iTemp6 + 12345);
			int iTemp8 = 1103515245 * (iTemp7 + 12345);
			int iTemp9 = 1103515245 * (iTemp8 + 12345);
			int iTemp10 = 1103515245 * (iTemp9 + 12345);
			int iTemp11 = 1103515245 * (iTemp10 + 12345);
			int iTemp12 = 1103515245 * (iTemp11 + 12345);
			int iTemp13 = 1103515245 * (iTemp12 + 12345);
			int iTemp14 = 1103515245 * (iTemp13 + 12345);
			int iTemp15 = 1103515245 * (iTemp14 + 12345);
			int iTemp16 = 1103515245 * (iTemp15 + 12345);
			int iTemp17 = 1103515245 * (iTemp16 + 12345);
			int iTemp18 = 1103515245 * (iTemp17 + 12345);
			int iTemp19 = 1103515245 * (iTemp18 + 12345);
			int iTemp20 = 1103515245 * (iTemp19 + 12345);
			int iTemp21 = 1103515245 * (iTemp20 + 12345);
			int iTemp22 = 1103515245 * (iTemp21 + 12345);
			int iTemp23 = 1103515245 * (iTemp22 + 12345);
			int iTemp24 = 1103515245 * (iTemp23 + 12345);
			int iTemp25 = 1103515245 * (iTemp24 + 12345);
			int iTemp26 = 1103515245 * (iTemp25 + 12345);
			int iTemp27 = 1103515245 * (iTemp26 + 12345);
			int iTemp28 = 1103515245 * (iTemp27 + 12345);
			int iTemp29 = 1103515245 * (iTemp28 + 12345);
			int iTemp30 = 1103515245 * (iTemp29 + 12345);
			int iTemp31 = 1103515245 * (iTemp30 + 12345);
			int iTemp32 = 1103515245 * (iTemp31 + 12345);
			int iTemp33 = 1103515245 * (iTemp32 + 12345);
			int iTemp34 = 1103515245 * (iTemp33 + 12345);
			int iTemp35 = 1103515245 * (iTemp34 + 12345);
			int iTemp36 = 1103515245 * (iTemp35 + 12345);
			int iTemp37 = 1103515245 * (iTemp36 + 12345);
			int iTemp38 = 1103515245 * (iTemp37 + 12345);
			int iTemp39 = 1103515245 * (iTemp38 + 12345);
			int iTemp40 = 1103515245 * (iTemp39 + 12345);
			int iTemp41 = 1103515245 * (iTemp40 + 12345);
			int iTemp42 = 1103515245 * (iTemp41 + 12345);
			int iTemp43 = 1103515245 * (iTemp42 + 12345);
			int iTemp44 = 1103515245 * (iTemp43 + 12345);
			int iTemp45 = 1103515245 * (iTemp44 + 12345);
			int iTemp46 = 1103515245 * (iTemp45 + 12345);
			int iTemp47 = 1103515245 * (iTemp46 + 12345);
			int iTemp48 = 1103515245 * (iTemp47 + 12345);
			int iTemp49 = 1103515245 * (iTemp48 + 12345);
			int iTemp50 = 1103515245 * (iTemp49 + 12345);
			int iTemp51 = 1103515245 * (iTemp50 + 12345);
			int iTemp52 = 1103515245 * (iTemp51 + 12345);
			int iTemp53 = 1103515245 * (iTemp52 + 12345);
			int iTemp54 = 1103515245 * (iTemp53 + 12345);
			int iTemp55 = 1103515245 * (iTemp54 + 12345);
			int iTemp56 = 1103515245 * (iTemp55 + 12345);
			int iTemp57 = 1103515245 * (iTemp56 + 12345);
			int iTemp58 = 1103515245 * (iTemp57 + 12345);
			int iTemp59 = 1103515245 * (iTemp58 + 12345);
			int iTemp60 = 1103515245 * (iTemp59 + 12345);
			int iTemp61 = 1103515245 * (iTemp60 + 12345);
			int iTemp62 = 1103515245 * (iTemp61 + 12345);
			int iTemp63 = 1103515245 * (iTemp62 + 12345);
			int iTemp64 = 1103515245 * (iTemp63 + 12345);
			int iTemp65 = 1103515245 * (iTemp64 + 12345);
			int iTemp66 = 1103515245 * (iTemp65 + 12345);
			int iTemp67 = 1103515245 * (iTemp66 + 12345);
			iRec6[0] = 1103515245 * (iTemp67 + 12345);
			int iRec7 = iTemp67;
			int iRec8 = iTemp66;
			int iRec9 = iTemp65;
			int iRec10 = iTemp64;
			int iRec11 = iTemp63;
			int iRec12 = iTemp62;
			int iRec13 = iTemp61;
			int iRec14 = iTemp60;
			int iRec15 = iTemp59;
			int iRec16 = iTemp58;
			int iRec17 = iTemp57;
			int iRec18 = iTemp56;
			int iRec19 = iTemp55;
			int iRec20 = iTemp54;
			int iRec21 = iTemp53;
			int iRec22 = iTemp52;
			int iRec23 = iTemp51;
			int iRec24 = iTemp50;
			int iRec25 = iTemp49;
			int iRec26 = iTemp48;
			int iRec27 = iTemp47;
			int iRec28 = iTemp46;
			int iRec29 = iTemp45;
			int iRec30 = iTemp44;
			int iRec31 = iTemp43;
			int iRec32 = iTemp42;
			int iRec33 = iTemp41;
			int iRec34 = iTemp40;
			int iRec35 = iTemp39;
			int iRec36 = iTemp38;
			int iRec37 = iTemp37;
			int iRec38 = iTemp36;
			int iRec39 = iTemp35;
			int iRec40 = iTemp34;
			int iRec41 = iTemp33;
			int iRec42 = iTemp32;
			int iRec43 = iTemp31;
			int iRec44 = iTemp30;
			int iRec45 = iTemp29;
			int iRec46 = iTemp28;
			int iRec47 = iTemp27;
			int iRec48 = iTemp26;
			int iRec49 = iTemp25;
			int iRec50 = iTemp24;
			int iRec51 = iTemp23;
			int iRec52 = iTemp22;
			int iRec53 = iTemp21;
			int iRec54 = iTemp20;
			int iRec55 = iTemp19;
			int iRec56 = iTemp18;
			int iRec57 = iTemp17;
			int iRec58 = iTemp16;
			int iRec59 = iTemp15;
			int iRec60 = iTemp14;
			int iRec61 = iTemp13;
			int iRec62 = iTemp12;
			int iRec63 = iTemp11;
			int iRec64 = iTemp10;
			int iRec65 = iTemp9;
			int iRec66 = iTemp8;
			int iRec67 = iTemp7;
			int iRec68 = iTemp6;
			int iRec69 = iTemp5;
			int iTemp68 = int(0.5f * fTemp3 * (1.0f - float(iRec6[0])));
			iRec5[0] = iVec0[1] * iRec5[1] + iTemp2 * iTemp68;
			int iTemp69 = (iRec3[0] + iRec5[0]) % iTemp1;
			float fTemp70 = float(iTemp1 + -1);
			float fTemp71 = float(iTemp69) / fTemp70;
			int iTemp72 = int(fTemp71);
			iRec2[0] = iRec2[1] * (1 - iTemp72) + iTemp68 * iTemp72;
			int iTemp73 = int(0.5f * fTemp3 * (1.0f - float(iRec8)));
			iRec72[0] = iVec0[1] * iRec72[1] + iTemp2 * iTemp73;
			int iTemp74 = (iRec3[0] + iRec72[0]) % iTemp1;
			float fTemp75 = float(iTemp74) / fTemp70;
			int iTemp76 = int(fTemp75);
			iRec71[0] = iRec71[1] * (1 - iTemp76) + iTemp73 * iTemp76;
			int iTemp77 = int(0.5f * fTemp3 * (1.0f - float(iRec10)));
			iRec74[0] = iVec0[1] * iRec74[1] + iTemp2 * iTemp77;
			int iTemp78 = (iRec3[0] + iRec74[0]) % iTemp1;
			float fTemp79 = float(iTemp78) / fTemp70;
			int iTemp80 = int(fTemp79);
			iRec73[0] = iRec73[1] * (1 - iTemp80) + iTemp77 * iTemp80;
			int iTemp81 = int(0.5f * fTemp3 * (1.0f - float(iRec12)));
			iRec76[0] = iVec0[1] * iRec76[1] + iTemp2 * iTemp81;
			int iTemp82 = (iRec3[0] + iRec76[0]) % iTemp1;
			float fTemp83 = float(iTemp82) / fTemp70;
			int iTemp84 = int(fTemp83);
			iRec75[0] = iRec75[1] * (1 - iTemp84) + iTemp81 * iTemp84;
			int iTemp85 = int(0.5f * fTemp3 * (1.0f - float(iRec14)));
			iRec78[0] = iVec0[1] * iRec78[1] + iTemp2 * iTemp85;
			int iTemp86 = (iRec3[0] + iRec78[0]) % iTemp1;
			float fTemp87 = float(iTemp86) / fTemp70;
			int iTemp88 = int(fTemp87);
			iRec77[0] = iRec77[1] * (1 - iTemp88) + iTemp85 * iTemp88;
			int iTemp89 = int(0.5f * fTemp3 * (1.0f - float(iRec16)));
			iRec80[0] = iVec0[1] * iRec80[1] + iTemp2 * iTemp89;
			int iTemp90 = (iRec3[0] + iRec80[0]) % iTemp1;
			float fTemp91 = float(iTemp90) / fTemp70;
			int iTemp92 = int(fTemp91);
			iRec79[0] = iRec79[1] * (1 - iTemp92) + iTemp89 * iTemp92;
			int iTemp93 = int(0.5f * fTemp3 * (1.0f - float(iRec18)));
			iRec82[0] = iVec0[1] * iRec82[1] + iTemp2 * iTemp93;
			int iTemp94 = (iRec3[0] + iRec82[0]) % iTemp1;
			float fTemp95 = float(iTemp94) / fTemp70;
			int iTemp96 = int(fTemp95);
			iRec81[0] = iRec81[1] * (1 - iTemp96) + iTemp93 * iTemp96;
			int iTemp97 = int(0.5f * fTemp3 * (1.0f - float(iRec20)));
			iRec84[0] = iVec0[1] * iRec84[1] + iTemp2 * iTemp97;
			int iTemp98 = (iRec3[0] + iRec84[0]) % iTemp1;
			float fTemp99 = float(iTemp98) / fTemp70;
			int iTemp100 = int(fTemp99);
			iRec83[0] = iRec83[1] * (1 - iTemp100) + iTemp97 * iTemp100;
			int iTemp101 = int(0.5f * fTemp3 * (1.0f - float(iRec22)));
			iRec86[0] = iVec0[1] * iRec86[1] + iTemp2 * iTemp101;
			int iTemp102 = (iRec3[0] + iRec86[0]) % iTemp1;
			float fTemp103 = float(iTemp102) / fTemp70;
			int iTemp104 = int(fTemp103);
			iRec85[0] = iRec85[1] * (1 - iTemp104) + iTemp101 * iTemp104;
			int iTemp105 = int(0.5f * fTemp3 * (1.0f - float(iRec24)));
			iRec88[0] = iVec0[1] * iRec88[1] + iTemp2 * iTemp105;
			int iTemp106 = (iRec3[0] + iRec88[0]) % iTemp1;
			float fTemp107 = float(iTemp106) / fTemp70;
			int iTemp108 = int(fTemp107);
			iRec87[0] = iRec87[1] * (1 - iTemp108) + iTemp105 * iTemp108;
			int iTemp109 = int(0.5f * fTemp3 * (1.0f - float(iRec26)));
			iRec90[0] = iVec0[1] * iRec90[1] + iTemp2 * iTemp109;
			int iTemp110 = (iRec3[0] + iRec90[0]) % iTemp1;
			float fTemp111 = float(iTemp110) / fTemp70;
			int iTemp112 = int(fTemp111);
			iRec89[0] = iRec89[1] * (1 - iTemp112) + iTemp109 * iTemp112;
			int iTemp113 = int(0.5f * fTemp3 * (1.0f - float(iRec28)));
			iRec92[0] = iVec0[1] * iRec92[1] + iTemp2 * iTemp113;
			int iTemp114 = (iRec3[0] + iRec92[0]) % iTemp1;
			float fTemp115 = float(iTemp114) / fTemp70;
			int iTemp116 = int(fTemp115);
			iRec91[0] = iRec91[1] * (1 - iTemp116) + iTemp113 * iTemp116;
			int iTemp117 = int(0.5f * fTemp3 * (1.0f - float(iRec30)));
			iRec94[0] = iVec0[1] * iRec94[1] + iTemp2 * iTemp117;
			int iTemp118 = (iRec3[0] + iRec94[0]) % iTemp1;
			float fTemp119 = float(iTemp118) / fTemp70;
			int iTemp120 = int(fTemp119);
			iRec93[0] = iRec93[1] * (1 - iTemp120) + iTemp117 * iTemp120;
			int iTemp121 = int(0.5f * fTemp3 * (1.0f - float(iRec32)));
			iRec96[0] = iVec0[1] * iRec96[1] + iTemp2 * iTemp121;
			int iTemp122 = (iRec3[0] + iRec96[0]) % iTemp1;
			float fTemp123 = float(iTemp122) / fTemp70;
			int iTemp124 = int(fTemp123);
			iRec95[0] = iRec95[1] * (1 - iTemp124) + iTemp121 * iTemp124;
			int iTemp125 = int(0.5f * fTemp3 * (1.0f - float(iRec34)));
			iRec98[0] = iVec0[1] * iRec98[1] + iTemp2 * iTemp125;
			int iTemp126 = (iRec3[0] + iRec98[0]) % iTemp1;
			float fTemp127 = float(iTemp126) / fTemp70;
			int iTemp128 = int(fTemp127);
			iRec97[0] = iRec97[1] * (1 - iTemp128) + iTemp125 * iTemp128;
			int iTemp129 = int(0.5f * fTemp3 * (1.0f - float(iRec36)));
			iRec100[0] = iVec0[1] * iRec100[1] + iTemp2 * iTemp129;
			int iTemp130 = (iRec3[0] + iRec100[0]) % iTemp1;
			float fTemp131 = float(iTemp130) / fTemp70;
			int iTemp132 = int(fTemp131);
			iRec99[0] = iRec99[1] * (1 - iTemp132) + iTemp129 * iTemp132;
			int iTemp133 = int(0.5f * fTemp3 * (1.0f - float(iRec38)));
			iRec102[0] = iVec0[1] * iRec102[1] + iTemp2 * iTemp133;
			int iTemp134 = (iRec3[0] + iRec102[0]) % iTemp1;
			float fTemp135 = float(iTemp134) / fTemp70;
			int iTemp136 = int(fTemp135);
			iRec101[0] = iRec101[1] * (1 - iTemp136) + iTemp133 * iTemp136;
			int iTemp137 = int(0.5f * fTemp3 * (1.0f - float(iRec40)));
			iRec104[0] = iVec0[1] * iRec104[1] + iTemp2 * iTemp137;
			int iTemp138 = (iRec3[0] + iRec104[0]) % iTemp1;
			float fTemp139 = float(iTemp138) / fTemp70;
			int iTemp140 = int(fTemp139);
			iRec103[0] = iRec103[1] * (1 - iTemp140) + iTemp137 * iTemp140;
			int iTemp141 = int(0.5f * fTemp3 * (1.0f - float(iRec42)));
			iRec106[0] = iVec0[1] * iRec106[1] + iTemp2 * iTemp141;
			int iTemp142 = (iRec3[0] + iRec106[0]) % iTemp1;
			float fTemp143 = float(iTemp142) / fTemp70;
			int iTemp144 = int(fTemp143);
			iRec105[0] = iRec105[1] * (1 - iTemp144) + iTemp141 * iTemp144;
			int iTemp145 = int(0.5f * fTemp3 * (1.0f - float(iRec44)));
			iRec108[0] = iVec0[1] * iRec108[1] + iTemp2 * iTemp145;
			int iTemp146 = (iRec3[0] + iRec108[0]) % iTemp1;
			float fTemp147 = float(iTemp146) / fTemp70;
			int iTemp148 = int(fTemp147);
			iRec107[0] = iRec107[1] * (1 - iTemp148) + iTemp145 * iTemp148;
			int iTemp149 = int(0.5f * fTemp3 * (1.0f - float(iRec46)));
			iRec110[0] = iVec0[1] * iRec110[1] + iTemp2 * iTemp149;
			int iTemp150 = (iRec3[0] + iRec110[0]) % iTemp1;
			float fTemp151 = float(iTemp150) / fTemp70;
			int iTemp152 = int(fTemp151);
			iRec109[0] = iRec109[1] * (1 - iTemp152) + iTemp149 * iTemp152;
			int iTemp153 = int(0.5f * fTemp3 * (1.0f - float(iRec48)));
			iRec112[0] = iVec0[1] * iRec112[1] + iTemp2 * iTemp153;
			int iTemp154 = (iRec3[0] + iRec112[0]) % iTemp1;
			float fTemp155 = float(iTemp154) / fTemp70;
			int iTemp156 = int(fTemp155);
			iRec111[0] = iRec111[1] * (1 - iTemp156) + iTemp153 * iTemp156;
			int iTemp157 = int(0.5f * fTemp3 * (1.0f - float(iRec50)));
			iRec114[0] = iVec0[1] * iRec114[1] + iTemp2 * iTemp157;
			int iTemp158 = (iRec3[0] + iRec114[0]) % iTemp1;
			float fTemp159 = float(iTemp158) / fTemp70;
			int iTemp160 = int(fTemp159);
			iRec113[0] = iRec113[1] * (1 - iTemp160) + iTemp157 * iTemp160;
			int iTemp161 = int(0.5f * fTemp3 * (1.0f - float(iRec52)));
			iRec116[0] = iVec0[1] * iRec116[1] + iTemp2 * iTemp161;
			int iTemp162 = (iRec3[0] + iRec116[0]) % iTemp1;
			float fTemp163 = float(iTemp162) / fTemp70;
			int iTemp164 = int(fTemp163);
			iRec115[0] = iRec115[1] * (1 - iTemp164) + iTemp161 * iTemp164;
			int iTemp165 = int(0.5f * fTemp3 * (1.0f - float(iRec54)));
			iRec118[0] = iVec0[1] * iRec118[1] + iTemp2 * iTemp165;
			int iTemp166 = (iRec3[0] + iRec118[0]) % iTemp1;
			float fTemp167 = float(iTemp166) / fTemp70;
			int iTemp168 = int(fTemp167);
			iRec117[0] = iRec117[1] * (1 - iTemp168) + iTemp165 * iTemp168;
			int iTemp169 = int(0.5f * fTemp3 * (1.0f - float(iRec56)));
			iRec120[0] = iVec0[1] * iRec120[1] + iTemp2 * iTemp169;
			int iTemp170 = (iRec3[0] + iRec120[0]) % iTemp1;
			float fTemp171 = float(iTemp170) / fTemp70;
			int iTemp172 = int(fTemp171);
			iRec119[0] = iRec119[1] * (1 - iTemp172) + iTemp169 * iTemp172;
			int iTemp173 = int(0.5f * fTemp3 * (1.0f - float(iRec58)));
			iRec122[0] = iVec0[1] * iRec122[1] + iTemp2 * iTemp173;
			int iTemp174 = (iRec3[0] + iRec122[0]) % iTemp1;
			float fTemp175 = float(iTemp174) / fTemp70;
			int iTemp176 = int(fTemp175);
			iRec121[0] = iRec121[1] * (1 - iTemp176) + iTemp173 * iTemp176;
			int iTemp177 = int(0.5f * fTemp3 * (1.0f - float(iRec60)));
			iRec124[0] = iVec0[1] * iRec124[1] + iTemp2 * iTemp177;
			int iTemp178 = (iRec3[0] + iRec124[0]) % iTemp1;
			float fTemp179 = float(iTemp178) / fTemp70;
			int iTemp180 = int(fTemp179);
			iRec123[0] = iRec123[1] * (1 - iTemp180) + iTemp177 * iTemp180;
			int iTemp181 = int(0.5f * fTemp3 * (1.0f - float(iRec62)));
			iRec126[0] = iVec0[1] * iRec126[1] + iTemp2 * iTemp181;
			int iTemp182 = (iRec3[0] + iRec126[0]) % iTemp1;
			float fTemp183 = float(iTemp182) / fTemp70;
			int iTemp184 = int(fTemp183);
			iRec125[0] = iRec125[1] * (1 - iTemp184) + iTemp181 * iTemp184;
			int iTemp185 = int(0.5f * fTemp3 * (1.0f - float(iRec64)));
			iRec128[0] = iVec0[1] * iRec128[1] + iTemp2 * iTemp185;
			int iTemp186 = (iRec3[0] + iRec128[0]) % iTemp1;
			float fTemp187 = float(iTemp186) / fTemp70;
			int iTemp188 = int(fTemp187);
			iRec127[0] = iRec127[1] * (1 - iTemp188) + iTemp185 * iTemp188;
			int iTemp189 = int(0.5f * fTemp3 * (1.0f - float(iRec66)));
			iRec130[0] = iVec0[1] * iRec130[1] + iTemp2 * iTemp189;
			int iTemp190 = (iRec3[0] + iRec130[0]) % iTemp1;
			float fTemp191 = float(iTemp190) / fTemp70;
			int iTemp192 = int(fTemp191);
			iRec129[0] = iRec129[1] * (1 - iTemp192) + iTemp189 * iTemp192;
			int iTemp193 = int(0.5f * fTemp3 * (1.0f - float(iRec68)));
			iRec132[0] = iVec0[1] * iRec132[1] + iTemp2 * iTemp193;
			int iTemp194 = (iRec3[0] + iRec132[0]) % iTemp1;
			float fTemp195 = float(iTemp194) / fTemp70;
			int iTemp196 = int(fTemp195);
			iRec131[0] = iRec131[1] * (1 - iTemp196) + iTemp193 * iTemp196;
			output0[i0] = FAUSTFLOAT(fSlow1 * (fSlow2 * ftbl0[(iRec2[0] + iTemp69) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp71)] + fSlow5 * ftbl0[(iRec71[0] + iTemp74) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp75)] + fSlow6 * ftbl0[(iRec73[0] + iTemp78) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp79)] + fSlow7 * ftbl0[(iRec75[0] + iTemp82) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp83)] + fSlow8 * ftbl0[(iRec77[0] + iTemp86) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp87)] + fSlow9 * ftbl0[(iRec79[0] + iTemp90) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp91)] + fSlow10 * ftbl0[(iRec81[0] + iTemp94) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp95)] + fSlow11 * ftbl0[(iRec83[0] + iTemp98) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp99)] + fSlow12 * ftbl0[(iRec85[0] + iTemp102) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp103)] + fSlow13 * ftbl0[(iRec87[0] + iTemp106) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp107)] + fSlow14 * ftbl0[(iRec89[0] + iTemp110) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp111)] + fSlow15 * ftbl0[(iRec91[0] + iTemp114) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp115)] + fSlow16 * ftbl0[(iRec93[0] + iTemp118) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp119)] + fSlow17 * ftbl0[(iRec95[0] + iTemp122) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp123)] + fSlow18 * ftbl0[(iRec97[0] + iTemp126) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp127)] + fSlow19 * ftbl0[(iRec99[0] + iTemp130) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp131)] + fSlow20 * ftbl0[(iRec101[0] + iTemp134) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp135)] + fSlow21 * ftbl0[(iRec103[0] + iTemp138) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp139)] + fSlow22 * ftbl0[(iRec105[0] + iTemp142) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp143)] + fSlow23 * ftbl0[(iRec107[0] + iTemp146) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp147)] + fSlow24 * ftbl0[(iRec109[0] + iTemp150) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp151)] + fSlow25 * ftbl0[(iRec111[0] + iTemp154) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp155)] + fSlow26 * ftbl0[(iRec113[0] + iTemp158) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp159)] + fSlow27 * ftbl0[(iRec115[0] + iTemp162) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp163)] + fSlow28 * ftbl0[(iRec117[0] + iTemp166) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp167)] + fSlow29 * ftbl0[(iRec119[0] + iTemp170) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp171)] + fSlow30 * ftbl0[(iRec121[0] + iTemp174) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp175)] + fSlow31 * ftbl0[(iRec123[0] + iTemp178) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp179)] + fSlow32 * ftbl0[(iRec125[0] + iTemp182) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp183)] + fSlow33 * ftbl0[(iRec127[0] + iTemp186) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp187)] + fSlow34 * ftbl0[(iRec129[0] + iTemp190) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp191)] + fSlow35 * ftbl0[(iRec131[0] + iTemp194) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp195)]));
			int iTemp197 = int(0.5f * fTemp3 * (1.0f - float(iRec7)));
			iRec134[0] = iVec0[1] * iRec134[1] + iTemp2 * iTemp197;
			int iTemp198 = (iRec3[0] + iRec134[0]) % iTemp1;
			float fTemp199 = float(iTemp198) / fTemp70;
			int iTemp200 = int(fTemp199);
			iRec133[0] = iRec133[1] * (1 - iTemp200) + iTemp197 * iTemp200;
			int iTemp201 = int(0.5f * fTemp3 * (1.0f - float(iRec9)));
			iRec136[0] = iVec0[1] * iRec136[1] + iTemp2 * iTemp201;
			int iTemp202 = (iRec3[0] + iRec136[0]) % iTemp1;
			float fTemp203 = float(iTemp202) / fTemp70;
			int iTemp204 = int(fTemp203);
			iRec135[0] = iRec135[1] * (1 - iTemp204) + iTemp201 * iTemp204;
			int iTemp205 = int(0.5f * fTemp3 * (1.0f - float(iRec11)));
			iRec138[0] = iVec0[1] * iRec138[1] + iTemp2 * iTemp205;
			int iTemp206 = (iRec3[0] + iRec138[0]) % iTemp1;
			float fTemp207 = float(iTemp206) / fTemp70;
			int iTemp208 = int(fTemp207);
			iRec137[0] = iRec137[1] * (1 - iTemp208) + iTemp205 * iTemp208;
			int iTemp209 = int(0.5f * fTemp3 * (1.0f - float(iRec13)));
			iRec140[0] = iVec0[1] * iRec140[1] + iTemp2 * iTemp209;
			int iTemp210 = (iRec3[0] + iRec140[0]) % iTemp1;
			float fTemp211 = float(iTemp210) / fTemp70;
			int iTemp212 = int(fTemp211);
			iRec139[0] = iRec139[1] * (1 - iTemp212) + iTemp209 * iTemp212;
			int iTemp213 = int(0.5f * fTemp3 * (1.0f - float(iRec15)));
			iRec142[0] = iVec0[1] * iRec142[1] + iTemp2 * iTemp213;
			int iTemp214 = (iRec3[0] + iRec142[0]) % iTemp1;
			float fTemp215 = float(iTemp214) / fTemp70;
			int iTemp216 = int(fTemp215);
			iRec141[0] = iRec141[1] * (1 - iTemp216) + iTemp213 * iTemp216;
			int iTemp217 = int(0.5f * fTemp3 * (1.0f - float(iRec17)));
			iRec144[0] = iVec0[1] * iRec144[1] + iTemp2 * iTemp217;
			int iTemp218 = (iRec3[0] + iRec144[0]) % iTemp1;
			float fTemp219 = float(iTemp218) / fTemp70;
			int iTemp220 = int(fTemp219);
			iRec143[0] = iRec143[1] * (1 - iTemp220) + iTemp217 * iTemp220;
			int iTemp221 = int(0.5f * fTemp3 * (1.0f - float(iRec19)));
			iRec146[0] = iVec0[1] * iRec146[1] + iTemp2 * iTemp221;
			int iTemp222 = (iRec3[0] + iRec146[0]) % iTemp1;
			float fTemp223 = float(iTemp222) / fTemp70;
			int iTemp224 = int(fTemp223);
			iRec145[0] = iRec145[1] * (1 - iTemp224) + iTemp221 * iTemp224;
			int iTemp225 = int(0.5f * fTemp3 * (1.0f - float(iRec21)));
			iRec148[0] = iVec0[1] * iRec148[1] + iTemp2 * iTemp225;
			int iTemp226 = (iRec3[0] + iRec148[0]) % iTemp1;
			float fTemp227 = float(iTemp226) / fTemp70;
			int iTemp228 = int(fTemp227);
			iRec147[0] = iRec147[1] * (1 - iTemp228) + iTemp225 * iTemp228;
			int iTemp229 = int(0.5f * fTemp3 * (1.0f - float(iRec23)));
			iRec150[0] = iVec0[1] * iRec150[1] + iTemp2 * iTemp229;
			int iTemp230 = (iRec3[0] + iRec150[0]) % iTemp1;
			float fTemp231 = float(iTemp230) / fTemp70;
			int iTemp232 = int(fTemp231);
			iRec149[0] = iRec149[1] * (1 - iTemp232) + iTemp229 * iTemp232;
			int iTemp233 = int(0.5f * fTemp3 * (1.0f - float(iRec25)));
			iRec152[0] = iVec0[1] * iRec152[1] + iTemp2 * iTemp233;
			int iTemp234 = (iRec3[0] + iRec152[0]) % iTemp1;
			float fTemp235 = float(iTemp234) / fTemp70;
			int iTemp236 = int(fTemp235);
			iRec151[0] = iRec151[1] * (1 - iTemp236) + iTemp233 * iTemp236;
			int iTemp237 = int(0.5f * fTemp3 * (1.0f - float(iRec27)));
			iRec154[0] = iVec0[1] * iRec154[1] + iTemp2 * iTemp237;
			int iTemp238 = (iRec3[0] + iRec154[0]) % iTemp1;
			float fTemp239 = float(iTemp238) / fTemp70;
			int iTemp240 = int(fTemp239);
			iRec153[0] = iRec153[1] * (1 - iTemp240) + iTemp237 * iTemp240;
			int iTemp241 = int(0.5f * fTemp3 * (1.0f - float(iRec29)));
			iRec156[0] = iVec0[1] * iRec156[1] + iTemp2 * iTemp241;
			int iTemp242 = (iRec3[0] + iRec156[0]) % iTemp1;
			float fTemp243 = float(iTemp242) / fTemp70;
			int iTemp244 = int(fTemp243);
			iRec155[0] = iRec155[1] * (1 - iTemp244) + iTemp241 * iTemp244;
			int iTemp245 = int(0.5f * fTemp3 * (1.0f - float(iRec31)));
			iRec158[0] = iVec0[1] * iRec158[1] + iTemp2 * iTemp245;
			int iTemp246 = (iRec3[0] + iRec158[0]) % iTemp1;
			float fTemp247 = float(iTemp246) / fTemp70;
			int iTemp248 = int(fTemp247);
			iRec157[0] = iRec157[1] * (1 - iTemp248) + iTemp245 * iTemp248;
			int iTemp249 = int(0.5f * fTemp3 * (1.0f - float(iRec33)));
			iRec160[0] = iVec0[1] * iRec160[1] + iTemp2 * iTemp249;
			int iTemp250 = (iRec3[0] + iRec160[0]) % iTemp1;
			float fTemp251 = float(iTemp250) / fTemp70;
			int iTemp252 = int(fTemp251);
			iRec159[0] = iRec159[1] * (1 - iTemp252) + iTemp249 * iTemp252;
			int iTemp253 = int(0.5f * fTemp3 * (1.0f - float(iRec35)));
			iRec162[0] = iVec0[1] * iRec162[1] + iTemp2 * iTemp253;
			int iTemp254 = (iRec3[0] + iRec162[0]) % iTemp1;
			float fTemp255 = float(iTemp254) / fTemp70;
			int iTemp256 = int(fTemp255);
			iRec161[0] = iRec161[1] * (1 - iTemp256) + iTemp253 * iTemp256;
			int iTemp257 = int(0.5f * fTemp3 * (1.0f - float(iRec37)));
			iRec164[0] = iVec0[1] * iRec164[1] + iTemp2 * iTemp257;
			int iTemp258 = (iRec3[0] + iRec164[0]) % iTemp1;
			float fTemp259 = float(iTemp258) / fTemp70;
			int iTemp260 = int(fTemp259);
			iRec163[0] = iRec163[1] * (1 - iTemp260) + iTemp257 * iTemp260;
			int iTemp261 = int(0.5f * fTemp3 * (1.0f - float(iRec39)));
			iRec166[0] = iVec0[1] * iRec166[1] + iTemp2 * iTemp261;
			int iTemp262 = (iRec3[0] + iRec166[0]) % iTemp1;
			float fTemp263 = float(iTemp262) / fTemp70;
			int iTemp264 = int(fTemp263);
			iRec165[0] = iRec165[1] * (1 - iTemp264) + iTemp261 * iTemp264;
			int iTemp265 = int(0.5f * fTemp3 * (1.0f - float(iRec41)));
			iRec168[0] = iVec0[1] * iRec168[1] + iTemp2 * iTemp265;
			int iTemp266 = (iRec3[0] + iRec168[0]) % iTemp1;
			float fTemp267 = float(iTemp266) / fTemp70;
			int iTemp268 = int(fTemp267);
			iRec167[0] = iRec167[1] * (1 - iTemp268) + iTemp265 * iTemp268;
			int iTemp269 = int(0.5f * fTemp3 * (1.0f - float(iRec43)));
			iRec170[0] = iVec0[1] * iRec170[1] + iTemp2 * iTemp269;
			int iTemp270 = (iRec3[0] + iRec170[0]) % iTemp1;
			float fTemp271 = float(iTemp270) / fTemp70;
			int iTemp272 = int(fTemp271);
			iRec169[0] = iRec169[1] * (1 - iTemp272) + iTemp269 * iTemp272;
			int iTemp273 = int(0.5f * fTemp3 * (1.0f - float(iRec45)));
			iRec172[0] = iVec0[1] * iRec172[1] + iTemp2 * iTemp273;
			int iTemp274 = (iRec3[0] + iRec172[0]) % iTemp1;
			float fTemp275 = float(iTemp274) / fTemp70;
			int iTemp276 = int(fTemp275);
			iRec171[0] = iRec171[1] * (1 - iTemp276) + iTemp273 * iTemp276;
			int iTemp277 = int(0.5f * fTemp3 * (1.0f - float(iRec47)));
			iRec174[0] = iVec0[1] * iRec174[1] + iTemp2 * iTemp277;
			int iTemp278 = (iRec3[0] + iRec174[0]) % iTemp1;
			float fTemp279 = float(iTemp278) / fTemp70;
			int iTemp280 = int(fTemp279);
			iRec173[0] = iRec173[1] * (1 - iTemp280) + iTemp277 * iTemp280;
			int iTemp281 = int(0.5f * fTemp3 * (1.0f - float(iRec49)));
			iRec176[0] = iVec0[1] * iRec176[1] + iTemp2 * iTemp281;
			int iTemp282 = (iRec3[0] + iRec176[0]) % iTemp1;
			float fTemp283 = float(iTemp282) / fTemp70;
			int iTemp284 = int(fTemp283);
			iRec175[0] = iRec175[1] * (1 - iTemp284) + iTemp281 * iTemp284;
			int iTemp285 = int(0.5f * fTemp3 * (1.0f - float(iRec51)));
			iRec178[0] = iVec0[1] * iRec178[1] + iTemp2 * iTemp285;
			int iTemp286 = (iRec3[0] + iRec178[0]) % iTemp1;
			float fTemp287 = float(iTemp286) / fTemp70;
			int iTemp288 = int(fTemp287);
			iRec177[0] = iRec177[1] * (1 - iTemp288) + iTemp285 * iTemp288;
			int iTemp289 = int(0.5f * fTemp3 * (1.0f - float(iRec53)));
			iRec180[0] = iVec0[1] * iRec180[1] + iTemp2 * iTemp289;
			int iTemp290 = (iRec3[0] + iRec180[0]) % iTemp1;
			float fTemp291 = float(iTemp290) / fTemp70;
			int iTemp292 = int(fTemp291);
			iRec179[0] = iRec179[1] * (1 - iTemp292) + iTemp289 * iTemp292;
			int iTemp293 = int(0.5f * fTemp3 * (1.0f - float(iRec55)));
			iRec182[0] = iVec0[1] * iRec182[1] + iTemp2 * iTemp293;
			int iTemp294 = (iRec3[0] + iRec182[0]) % iTemp1;
			float fTemp295 = float(iTemp294) / fTemp70;
			int iTemp296 = int(fTemp295);
			iRec181[0] = iRec181[1] * (1 - iTemp296) + iTemp293 * iTemp296;
			int iTemp297 = int(0.5f * fTemp3 * (1.0f - float(iRec57)));
			iRec184[0] = iVec0[1] * iRec184[1] + iTemp2 * iTemp297;
			int iTemp298 = (iRec3[0] + iRec184[0]) % iTemp1;
			float fTemp299 = float(iTemp298) / fTemp70;
			int iTemp300 = int(fTemp299);
			iRec183[0] = iRec183[1] * (1 - iTemp300) + iTemp297 * iTemp300;
			int iTemp301 = int(0.5f * fTemp3 * (1.0f - float(iRec59)));
			iRec186[0] = iVec0[1] * iRec186[1] + iTemp2 * iTemp301;
			int iTemp302 = (iRec3[0] + iRec186[0]) % iTemp1;
			float fTemp303 = float(iTemp302) / fTemp70;
			int iTemp304 = int(fTemp303);
			iRec185[0] = iRec185[1] * (1 - iTemp304) + iTemp301 * iTemp304;
			int iTemp305 = int(0.5f * fTemp3 * (1.0f - float(iRec61)));
			iRec188[0] = iVec0[1] * iRec188[1] + iTemp2 * iTemp305;
			int iTemp306 = (iRec3[0] + iRec188[0]) % iTemp1;
			float fTemp307 = float(iTemp306) / fTemp70;
			int iTemp308 = int(fTemp307);
			iRec187[0] = iRec187[1] * (1 - iTemp308) + iTemp305 * iTemp308;
			int iTemp309 = int(0.5f * fTemp3 * (1.0f - float(iRec63)));
			iRec190[0] = iVec0[1] * iRec190[1] + iTemp2 * iTemp309;
			int iTemp310 = (iRec3[0] + iRec190[0]) % iTemp1;
			float fTemp311 = float(iTemp310) / fTemp70;
			int iTemp312 = int(fTemp311);
			iRec189[0] = iRec189[1] * (1 - iTemp312) + iTemp309 * iTemp312;
			int iTemp313 = int(0.5f * fTemp3 * (1.0f - float(iRec65)));
			iRec192[0] = iVec0[1] * iRec192[1] + iTemp2 * iTemp313;
			int iTemp314 = (iRec3[0] + iRec192[0]) % iTemp1;
			float fTemp315 = float(iTemp314) / fTemp70;
			int iTemp316 = int(fTemp315);
			iRec191[0] = iRec191[1] * (1 - iTemp316) + iTemp313 * iTemp316;
			int iTemp317 = int(0.5f * fTemp3 * (1.0f - float(iRec67)));
			iRec194[0] = iVec0[1] * iRec194[1] + iTemp2 * iTemp317;
			int iTemp318 = (iRec3[0] + iRec194[0]) % iTemp1;
			float fTemp319 = float(iTemp318) / fTemp70;
			int iTemp320 = int(fTemp319);
			iRec193[0] = iRec193[1] * (1 - iTemp320) + iTemp317 * iTemp320;
			int iTemp321 = int(0.5f * fTemp3 * (1.0f - float(iRec69)));
			iRec196[0] = iVec0[1] * iRec196[1] + iTemp2 * iTemp321;
			int iTemp322 = (iRec3[0] + iRec196[0]) % iTemp1;
			float fTemp323 = float(iTemp322) / fTemp70;
			int iTemp324 = int(fTemp323);
			iRec195[0] = iRec195[1] * (1 - iTemp324) + iTemp321 * iTemp324;
			output1[i0] = FAUSTFLOAT(fSlow1 * (fSlow36 * ftbl0[(iRec133[0] + iTemp198) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp199)] + fSlow37 * ftbl0[(iRec135[0] + iTemp202) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp203)] + fSlow38 * ftbl0[(iRec137[0] + iTemp206) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp207)] + fSlow39 * ftbl0[(iRec139[0] + iTemp210) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp211)] + fSlow40 * ftbl0[(iRec141[0] + iTemp214) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp215)] + fSlow41 * ftbl0[(iRec143[0] + iTemp218) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp219)] + fSlow42 * ftbl0[(iRec145[0] + iTemp222) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp223)] + fSlow43 * ftbl0[(iRec147[0] + iTemp226) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp227)] + fSlow44 * ftbl0[(iRec149[0] + iTemp230) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp231)] + fSlow45 * ftbl0[(iRec151[0] + iTemp234) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp235)] + fSlow46 * ftbl0[(iRec153[0] + iTemp238) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp239)] + fSlow47 * ftbl0[(iRec155[0] + iTemp242) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp243)] + fSlow48 * ftbl0[(iRec157[0] + iTemp246) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp247)] + fSlow49 * ftbl0[(iRec159[0] + iTemp250) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp251)] + fSlow50 * ftbl0[(iRec161[0] + iTemp254) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp255)] + fSlow51 * ftbl0[(iRec163[0] + iTemp258) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp259)] + fSlow52 * ftbl0[(iRec165[0] + iTemp262) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp263)] + fSlow53 * ftbl0[(iRec167[0] + iTemp266) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp267)] + fSlow54 * ftbl0[(iRec169[0] + iTemp270) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp271)] + fSlow55 * ftbl0[(iRec171[0] + iTemp274) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp275)] + fSlow56 * ftbl0[(iRec173[0] + iTemp278) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp279)] + fSlow57 * ftbl0[(iRec175[0] + iTemp282) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp283)] + fSlow58 * ftbl0[(iRec177[0] + iTemp286) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp287)] + fSlow59 * ftbl0[(iRec179[0] + iTemp290) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp291)] + fSlow60 * ftbl0[(iRec181[0] + iTemp294) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp295)] + fSlow61 * ftbl0[(iRec183[0] + iTemp298) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp299)] + fSlow62 * ftbl0[(iRec185[0] + iTemp302) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp303)] + fSlow63 * ftbl0[(iRec187[0] + iTemp306) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp307)] + fSlow64 * ftbl0[(iRec189[0] + iTemp310) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp311)] + fSlow65 * ftbl0[(iRec191[0] + iTemp314) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp315)] + fSlow66 * ftbl0[(iRec193[0] + iTemp318) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp319)] + fSlow67 * ftbl0[(iRec195[0] + iTemp322) % iTemp0] * ftbl1fx_granulatorSIG1[int(1024.0f * fTemp323)]));
			iVec0[1] = iVec0[0];
			fRec1[1] = fRec1[0];
			iRec0[1] = iRec0[0];
			fRec4[1] = fRec4[0];
			iRec3[1] = iRec3[0];
			iRec6[1] = iRec6[0];
			iRec5[1] = iRec5[0];
			iRec2[1] = iRec2[0];
			iRec72[1] = iRec72[0];
			iRec71[1] = iRec71[0];
			iRec74[1] = iRec74[0];
			iRec73[1] = iRec73[0];
			iRec76[1] = iRec76[0];
			iRec75[1] = iRec75[0];
			iRec78[1] = iRec78[0];
			iRec77[1] = iRec77[0];
			iRec80[1] = iRec80[0];
			iRec79[1] = iRec79[0];
			iRec82[1] = iRec82[0];
			iRec81[1] = iRec81[0];
			iRec84[1] = iRec84[0];
			iRec83[1] = iRec83[0];
			iRec86[1] = iRec86[0];
			iRec85[1] = iRec85[0];
			iRec88[1] = iRec88[0];
			iRec87[1] = iRec87[0];
			iRec90[1] = iRec90[0];
			iRec89[1] = iRec89[0];
			iRec92[1] = iRec92[0];
			iRec91[1] = iRec91[0];
			iRec94[1] = iRec94[0];
			iRec93[1] = iRec93[0];
			iRec96[1] = iRec96[0];
			iRec95[1] = iRec95[0];
			iRec98[1] = iRec98[0];
			iRec97[1] = iRec97[0];
			iRec100[1] = iRec100[0];
			iRec99[1] = iRec99[0];
			iRec102[1] = iRec102[0];
			iRec101[1] = iRec101[0];
			iRec104[1] = iRec104[0];
			iRec103[1] = iRec103[0];
			iRec106[1] = iRec106[0];
			iRec105[1] = iRec105[0];
			iRec108[1] = iRec108[0];
			iRec107[1] = iRec107[0];
			iRec110[1] = iRec110[0];
			iRec109[1] = iRec109[0];
			iRec112[1] = iRec112[0];
			iRec111[1] = iRec111[0];
			iRec114[1] = iRec114[0];
			iRec113[1] = iRec113[0];
			iRec116[1] = iRec116[0];
			iRec115[1] = iRec115[0];
			iRec118[1] = iRec118[0];
			iRec117[1] = iRec117[0];
			iRec120[1] = iRec120[0];
			iRec119[1] = iRec119[0];
			iRec122[1] = iRec122[0];
			iRec121[1] = iRec121[0];
			iRec124[1] = iRec124[0];
			iRec123[1] = iRec123[0];
			iRec126[1] = iRec126[0];
			iRec125[1] = iRec125[0];
			iRec128[1] = iRec128[0];
			iRec127[1] = iRec127[0];
			iRec130[1] = iRec130[0];
			iRec129[1] = iRec129[0];
			iRec132[1] = iRec132[0];
			iRec131[1] = iRec131[0];
			iRec134[1] = iRec134[0];
			iRec133[1] = iRec133[0];
			iRec136[1] = iRec136[0];
			iRec135[1] = iRec135[0];
			iRec138[1] = iRec138[0];
			iRec137[1] = iRec137[0];
			iRec140[1] = iRec140[0];
			iRec139[1] = iRec139[0];
			iRec142[1] = iRec142[0];
			iRec141[1] = iRec141[0];
			iRec144[1] = iRec144[0];
			iRec143[1] = iRec143[0];
			iRec146[1] = iRec146[0];
			iRec145[1] = iRec145[0];
			iRec148[1] = iRec148[0];
			iRec147[1] = iRec147[0];
			iRec150[1] = iRec150[0];
			iRec149[1] = iRec149[0];
			iRec152[1] = iRec152[0];
			iRec151[1] = iRec151[0];
			iRec154[1] = iRec154[0];
			iRec153[1] = iRec153[0];
			iRec156[1] = iRec156[0];
			iRec155[1] = iRec155[0];
			iRec158[1] = iRec158[0];
			iRec157[1] = iRec157[0];
			iRec160[1] = iRec160[0];
			iRec159[1] = iRec159[0];
			iRec162[1] = iRec162[0];
			iRec161[1] = iRec161[0];
			iRec164[1] = iRec164[0];
			iRec163[1] = iRec163[0];
			iRec166[1] = iRec166[0];
			iRec165[1] = iRec165[0];
			iRec168[1] = iRec168[0];
			iRec167[1] = iRec167[0];
			iRec170[1] = iRec170[0];
			iRec169[1] = iRec169[0];
			iRec172[1] = iRec172[0];
			iRec171[1] = iRec171[0];
			iRec174[1] = iRec174[0];
			iRec173[1] = iRec173[0];
			iRec176[1] = iRec176[0];
			iRec175[1] = iRec175[0];
			iRec178[1] = iRec178[0];
			iRec177[1] = iRec177[0];
			iRec180[1] = iRec180[0];
			iRec179[1] = iRec179[0];
			iRec182[1] = iRec182[0];
			iRec181[1] = iRec181[0];
			iRec184[1] = iRec184[0];
			iRec183[1] = iRec183[0];
			iRec186[1] = iRec186[0];
			iRec185[1] = iRec185[0];
			iRec188[1] = iRec188[0];
			iRec187[1] = iRec187[0];
			iRec190[1] = iRec190[0];
			iRec189[1] = iRec189[0];
			iRec192[1] = iRec192[0];
			iRec191[1] = iRec191[0];
			iRec194[1] = iRec194[0];
			iRec193[1] = iRec193[0];
			iRec196[1] = iRec196[0];
			iRec195[1] = iRec195[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_granulator_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_granulator_UI<T>::name(sym(fx_granulator));

typedef _fx_granulator_UI<fx_granulator> fx_granulator_UI;

class faust_fx_granulator_tilde : public FaustExternal<fx_granulator, fx_granulator_UI> {
public:
    faust_fx_granulator_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
