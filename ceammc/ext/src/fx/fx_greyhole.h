/* ------------------------------------------------------------
author: "Julian Parker, bug fixes by Till Bovermann"
copyright: "(c) Julian Parker 2013"
license: "GPL2+"
name: "fx.greyhole"
version: "1.0"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn fx_greyhole -scn fx_greyhole_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_greyhole_H__
#define  __fx_greyhole_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_greyhole_dsp.h ********************************
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

class FAUST_API fx_greyhole_dsp {

    public:

        fx_greyhole_dsp() {}
        virtual ~fx_greyhole_dsp() {}

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
        virtual fx_greyhole_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_greyhole_dsp {

    protected:

        fx_greyhole_dsp* fDSP;

    public:

        decorator_dsp(fx_greyhole_dsp* fx_greyhole_dsp = nullptr):fDSP(fx_greyhole_dsp) {}
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
        virtual fx_greyhole_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_greyhole_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_greyhole_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_greyhole : public fx_greyhole_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "primes" */
#include "jprev.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_greyhole
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


class fx_greyhole : public fx_greyhole_dsp {
	
 private:
	
	int iVec0[2];
	FAUSTFLOAT fCheckbox0;
	float fRec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec1[2];
	FAUSTFLOAT fHslider1;
	float fVec1[2];
	FAUSTFLOAT fHslider2;
	float fRec8[2];
	FAUSTFLOAT fHslider3;
	float fVec2[2];
	float fRec12[2];
	float fRec16[2];
	float fRec23[2];
	float fRec27[2];
	float fRec31[2];
	float fRec38[2];
	float fRec42[2];
	float fRec46[2];
	FAUSTFLOAT fHslider4;
	float fVec3[2];
	float fConst3;
	FAUSTFLOAT fHslider5;
	float fVec4[2];
	float fConst4;
	FAUSTFLOAT fHslider6;
	float fVec5[2];
	float fRec50[2];
	float fRec51[2];
	int IOTA0;
	float fVec6[131072];
	FAUSTFLOAT fHslider7;
	float fRec52[2];
	float fRec53[2];
	float fRec54[2];
	float fRec55[2];
	float fVec7[16384];
	float fRec56[2];
	float fVec8[2];
	float fRec49[2];
	float fRec47[2];
	float fVec9[131072];
	float fVec10[16384];
	float fRec58[2];
	float fVec11[2];
	float fRec57[2];
	float fRec48[2];
	float fVec12[16384];
	float fVec13[2];
	float fRec45[2];
	float fRec43[2];
	float fVec14[16384];
	float fRec60[2];
	float fVec15[2];
	float fRec59[2];
	float fRec44[2];
	float fVec16[16384];
	float fVec17[2];
	float fRec41[2];
	float fRec39[2];
	float fVec18[16384];
	float fRec62[2];
	float fVec19[2];
	float fRec61[2];
	float fRec40[2];
	float fVec20[16384];
	float fVec21[2];
	float fRec37[2];
	float fRec35[2];
	float fVec22[16384];
	float fRec64[2];
	float fVec23[2];
	float fRec63[2];
	float fRec36[2];
	float fVec24[16384];
	float fRec65[2];
	float fVec25[2];
	float fRec34[2];
	float fRec32[2];
	float fVec26[16384];
	float fRec67[2];
	float fVec27[2];
	float fRec66[2];
	float fRec33[2];
	float fVec28[16384];
	float fVec29[2];
	float fRec30[2];
	float fRec28[2];
	float fVec30[16384];
	float fRec69[2];
	float fVec31[2];
	float fRec68[2];
	float fRec29[2];
	float fVec32[16384];
	float fVec33[2];
	float fRec26[2];
	float fRec24[2];
	float fVec34[16384];
	float fRec71[2];
	float fVec35[2];
	float fRec70[2];
	float fRec25[2];
	float fVec36[16384];
	float fVec37[2];
	float fRec22[2];
	float fRec20[2];
	float fVec38[16384];
	float fRec73[2];
	float fVec39[2];
	float fRec72[2];
	float fRec21[2];
	float fVec40[16384];
	float fRec74[2];
	float fVec41[2];
	float fRec19[2];
	float fRec17[2];
	float fVec42[16384];
	float fRec76[2];
	float fVec43[2];
	float fRec75[2];
	float fRec18[2];
	float fVec44[16384];
	float fVec45[2];
	float fRec15[2];
	float fRec13[2];
	float fVec46[16384];
	float fRec78[2];
	float fVec47[2];
	float fRec77[2];
	float fRec14[2];
	float fVec48[16384];
	float fVec49[2];
	float fRec11[2];
	float fRec9[2];
	float fVec50[16384];
	float fRec80[2];
	float fVec51[2];
	float fRec79[2];
	float fRec10[2];
	float fVec52[16384];
	float fVec53[2];
	float fRec7[2];
	float fRec5[2];
	float fVec54[16384];
	float fRec82[2];
	float fVec55[2];
	float fRec81[2];
	float fRec6[2];
	float fRec4[2];
	float fRec2[1024];
	float fRec83[2];
	float fRec3[1024];
	
 public:
	fx_greyhole() {
	}
	
	void metadata(Meta* m) { 
		m->declare("author", "Julian Parker, bug fixes by Till Bovermann");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.16.0");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.4");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn fx_greyhole -scn fx_greyhole_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "(c) Julian Parker 2013");
		m->declare("delays.lib/fdelay1a:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("filename", "fx_greyhole.dsp");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("license", "GPL2+");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "fx.greyhole");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.5.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 0.00056689343f * fConst0;
		fConst4 = 3.1415927f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(1.0f);
		fHslider3 = FAUSTFLOAT(0.5f);
		fHslider4 = FAUSTFLOAT(0.9f);
		fHslider5 = FAUSTFLOAT(0.1f);
		fHslider6 = FAUSTFLOAT(2.0f);
		fHslider7 = FAUSTFLOAT(0.2f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec8[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec12[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec16[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec23[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec27[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec31[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec38[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec42[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec46[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec3[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fVec4[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fVec5[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec50[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec51[l18] = 0.0f;
		}
		IOTA0 = 0;
		for (int l19 = 0; l19 < 131072; l19 = l19 + 1) {
			fVec6[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec52[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec53[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec54[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec55[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 16384; l24 = l24 + 1) {
			fVec7[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec56[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fVec8[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec49[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec47[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 131072; l29 = l29 + 1) {
			fVec9[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 16384; l30 = l30 + 1) {
			fVec10[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec58[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fVec11[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec57[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec48[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 16384; l35 = l35 + 1) {
			fVec12[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fVec13[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec45[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec43[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 16384; l39 = l39 + 1) {
			fVec14[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec60[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fVec15[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec59[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec44[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 16384; l44 = l44 + 1) {
			fVec16[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fVec17[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec41[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec39[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 16384; l48 = l48 + 1) {
			fVec18[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec62[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fVec19[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec61[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec40[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 16384; l53 = l53 + 1) {
			fVec20[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fVec21[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec37[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec35[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 16384; l57 = l57 + 1) {
			fVec22[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec64[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fVec23[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec63[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec36[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 16384; l62 = l62 + 1) {
			fVec24[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec65[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fVec25[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec34[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec32[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 16384; l67 = l67 + 1) {
			fVec26[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec67[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fVec27[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec66[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec33[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 16384; l72 = l72 + 1) {
			fVec28[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fVec29[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fRec30[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec28[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 16384; l76 = l76 + 1) {
			fVec30[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec69[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fVec31[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec68[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec29[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 16384; l81 = l81 + 1) {
			fVec32[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fVec33[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec26[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fRec24[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 16384; l85 = l85 + 1) {
			fVec34[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec71[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fVec35[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec70[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec25[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 16384; l90 = l90 + 1) {
			fVec36[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			fVec37[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec22[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec20[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 16384; l94 = l94 + 1) {
			fVec38[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fRec73[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fVec39[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec72[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec21[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 16384; l99 = l99 + 1) {
			fVec40[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			fRec74[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fVec41[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fRec19[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec17[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 16384; l104 = l104 + 1) {
			fVec42[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fRec76[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			fVec43[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 2; l107 = l107 + 1) {
			fRec75[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec18[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 16384; l109 = l109 + 1) {
			fVec44[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			fVec45[l110] = 0.0f;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			fRec15[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec13[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 16384; l113 = l113 + 1) {
			fVec46[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fRec78[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fVec47[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			fRec77[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fRec14[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 16384; l118 = l118 + 1) {
			fVec48[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 2; l119 = l119 + 1) {
			fVec49[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fRec11[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			fRec9[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 16384; l122 = l122 + 1) {
			fVec50[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fRec80[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			fVec51[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec79[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec10[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 16384; l127 = l127 + 1) {
			fVec52[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			fVec53[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fRec7[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			fRec5[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 16384; l131 = l131 + 1) {
			fVec54[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fRec82[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fVec55[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			fRec81[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fRec6[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
			fRec4[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 1024; l137 = l137 + 1) {
			fRec2[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 2; l138 = l138 + 1) {
			fRec83[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 1024; l139 = l139 + 1) {
			fRec3[l139] = 0.0f;
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
	
	virtual fx_greyhole* clone() {
		return new fx_greyhole();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.greyhole");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addHorizontalSlider("damping", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.99f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider7, "unit", "sec");
		ui_interface->addHorizontalSlider("delaytime", &fHslider7, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.001f), FAUSTFLOAT(1.45f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("diffusion", &fHslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.99f), FAUSTFLOAT(0.0001f));
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("feedback", &fHslider4, FAUSTFLOAT(0.9f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("moddepth", &fHslider5, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("modfreq", &fHslider6, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("size", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(float(fCheckbox0) >= 1.0f);
		float fSlow1 = fConst1 * float(fHslider0);
		float fSlow2 = float(fHslider1);
		float fSlow3 = float(fHslider2);
		float fSlow4 = 48.0f * fSlow3;
		int iSlow5 = primes(int(fSlow4));
		float fSlow6 = 0.001f * float(iSlow5);
		float fSlow7 = float(fHslider3);
		float fSlow8 = 61.0f * fSlow3;
		int iSlow9 = primes(int(fSlow8));
		float fSlow10 = 0.001f * float(iSlow9);
		float fSlow11 = 74.0f * fSlow3;
		int iSlow12 = primes(int(fSlow11));
		float fSlow13 = 0.001f * float(iSlow12);
		float fSlow14 = 29.0f * fSlow3;
		int iSlow15 = primes(int(fSlow14));
		float fSlow16 = 0.001f * float(iSlow15);
		float fSlow17 = 42.0f * fSlow3;
		int iSlow18 = primes(int(fSlow17));
		float fSlow19 = 0.001f * float(iSlow18);
		float fSlow20 = 55.0f * fSlow3;
		int iSlow21 = primes(int(fSlow20));
		float fSlow22 = 0.001f * float(iSlow21);
		int iSlow23 = primes(int(1e+01f * fSlow3));
		float fSlow24 = 0.001f * float(iSlow23);
		float fSlow25 = 23.0f * fSlow3;
		int iSlow26 = primes(int(fSlow25));
		float fSlow27 = 0.001f * float(iSlow26);
		float fSlow28 = 36.0f * fSlow3;
		int iSlow29 = primes(int(fSlow28));
		float fSlow30 = 0.001f * float(iSlow29);
		float fSlow31 = float(fHslider4);
		float fSlow32 = float(fHslider5);
		float fSlow33 = float(fHslider6);
		float fSlow34 = std::floor(std::min<float>(65533.0f, fConst0 * float(fHslider7)));
		float fSlow35 = 49.0f * fSlow3;
		int iSlow36 = primes(int(fSlow35));
		float fSlow37 = 0.0001f * float(iSlow36);
		int iSlow38 = primes(int(fSlow35 + 1e+01f));
		float fSlow39 = 0.0001f * float(iSlow38);
		int iSlow40 = primes(int(fSlow28 + 1e+01f));
		float fSlow41 = 0.001f * float(iSlow40);
		int iSlow42 = primes(int(fSlow25 + 1e+01f));
		float fSlow43 = 0.001f * float(iSlow42);
		int iSlow44 = primes(int(1e+01f * (fSlow3 + 1.0f)));
		float fSlow45 = 0.001f * float(iSlow44);
		float fSlow46 = 68.0f * fSlow3;
		int iSlow47 = primes(int(fSlow46));
		float fSlow48 = 0.0001f * float(iSlow47);
		int iSlow49 = primes(int(fSlow46 + 1e+01f));
		float fSlow50 = 0.0001f * float(iSlow49);
		int iSlow51 = primes(int(fSlow20 + 1e+01f));
		float fSlow52 = 0.001f * float(iSlow51);
		int iSlow53 = primes(int(fSlow17 + 1e+01f));
		float fSlow54 = 0.001f * float(iSlow53);
		int iSlow55 = primes(int(fSlow14 + 1e+01f));
		float fSlow56 = 0.001f * float(iSlow55);
		float fSlow57 = 87.0f * fSlow3;
		int iSlow58 = primes(int(fSlow57));
		float fSlow59 = 0.0001f * float(iSlow58);
		int iSlow60 = primes(int(fSlow57 + 1e+01f));
		float fSlow61 = 0.0001f * float(iSlow60);
		int iSlow62 = primes(int(fSlow11 + 1e+01f));
		float fSlow63 = 0.001f * float(iSlow62);
		int iSlow64 = primes(int(fSlow8 + 1e+01f));
		float fSlow65 = 0.001f * float(iSlow64);
		int iSlow66 = primes(int(fSlow4 + 1e+01f));
		float fSlow67 = 0.001f * float(iSlow66);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			iVec0[0] = 1;
			float fTemp1 = fRec0[1] + 0.0078125f;
			float fTemp2 = fRec0[1] + -0.0078125f;
			fRec0[0] = ((fTemp1 < fSlow0) ? fTemp1 : ((fTemp2 > fSlow0) ? fTemp2 : fSlow0));
			float fTemp3 = 1.0f - fRec0[0];
			fRec1[0] = fSlow1 + fConst2 * fRec1[1];
			float fTemp4 = 1.0f - fRec1[0];
			fVec1[0] = fSlow2;
			float fTemp5 = fSlow2 + fVec1[1];
			float fTemp6 = 1.0f - 0.5f * fTemp5;
			int iTemp7 = 1 - iVec0[1];
			fRec8[0] = fSlow6 + 0.999f * (fRec8[1] + float(iSlow5 * iTemp7));
			float fTemp8 = fRec8[0] + -1.49999f;
			float fTemp9 = std::floor(fTemp8);
			float fTemp10 = fTemp9 + (2.0f - fRec8[0]);
			float fTemp11 = fRec8[0] - fTemp9;
			fVec2[0] = fSlow7;
			float fTemp12 = fSlow7 + fVec2[1];
			float fTemp13 = 0.5f * fTemp12;
			float fTemp14 = std::cos(fTemp13);
			fRec12[0] = fSlow10 + 0.999f * (fRec12[1] + float(iSlow9 * iTemp7));
			float fTemp15 = fRec12[0] + -1.49999f;
			float fTemp16 = std::floor(fTemp15);
			float fTemp17 = fTemp16 + (2.0f - fRec12[0]);
			float fTemp18 = fRec12[0] - fTemp16;
			fRec16[0] = fSlow13 + 0.999f * (fRec16[1] + float(iSlow12 * iTemp7));
			float fTemp19 = fRec16[0] + -1.49999f;
			float fTemp20 = std::floor(fTemp19);
			float fTemp21 = fTemp20 + (2.0f - fRec16[0]);
			float fTemp22 = fRec16[0] - fTemp20;
			float fTemp23 = std::sin(fTemp13);
			float fTemp24 = -0.5f * fTemp12;
			float fTemp25 = std::cos(fTemp24);
			fRec23[0] = fSlow16 + 0.999f * (fRec23[1] + float(iSlow15 * iTemp7));
			float fTemp26 = fRec23[0] + -1.49999f;
			float fTemp27 = std::floor(fTemp26);
			float fTemp28 = fTemp27 + (2.0f - fRec23[0]);
			float fTemp29 = fRec23[0] - fTemp27;
			fRec27[0] = fSlow19 + 0.999f * (fRec27[1] + float(iSlow18 * iTemp7));
			float fTemp30 = fRec27[0] + -1.49999f;
			float fTemp31 = std::floor(fTemp30);
			float fTemp32 = fTemp31 + (2.0f - fRec27[0]);
			float fTemp33 = fRec27[0] - fTemp31;
			fRec31[0] = fSlow22 + 0.999f * (fRec31[1] + float(iSlow21 * iTemp7));
			float fTemp34 = fRec31[0] + -1.49999f;
			float fTemp35 = std::floor(fTemp34);
			float fTemp36 = fTemp35 + (2.0f - fRec31[0]);
			float fTemp37 = fRec31[0] - fTemp35;
			float fTemp38 = std::sin(fTemp24);
			fRec38[0] = fSlow24 + 0.999f * (fRec38[1] + float(iSlow23 * iTemp7));
			float fTemp39 = fRec38[0] + -1.49999f;
			float fTemp40 = std::floor(fTemp39);
			float fTemp41 = fTemp40 + (2.0f - fRec38[0]);
			float fTemp42 = fRec38[0] - fTemp40;
			fRec42[0] = fSlow27 + 0.999f * (fRec42[1] + float(iSlow26 * iTemp7));
			float fTemp43 = fRec42[0] + -1.49999f;
			float fTemp44 = std::floor(fTemp43);
			float fTemp45 = fTemp44 + (2.0f - fRec42[0]);
			float fTemp46 = fRec42[0] - fTemp44;
			fRec46[0] = fSlow30 + 0.999f * (fRec46[1] + float(iSlow29 * iTemp7));
			float fTemp47 = fRec46[0] + -1.49999f;
			float fTemp48 = std::floor(fTemp47);
			float fTemp49 = fTemp48 + (2.0f - fRec46[0]);
			float fTemp50 = fRec46[0] - fTemp48;
			float fTemp51 = float(input1[i0]);
			fVec3[0] = fSlow31;
			float fTemp52 = fSlow31 + fVec3[1];
			fVec4[0] = fSlow32;
			float fTemp53 = fSlow32 + fVec4[1];
			fVec5[0] = fSlow33;
			float fTemp54 = fConst4 * (fSlow33 + fVec5[1]);
			float fTemp55 = std::sin(fTemp54);
			float fTemp56 = std::cos(fTemp54);
			fRec50[0] = fRec51[1] * fTemp55 + fRec50[1] * fTemp56;
			fRec51[0] = float(iTemp7) + fRec51[1] * fTemp56 - fTemp55 * fRec50[1];
			float fTemp57 = fConst3 * fTemp53 * (fRec50[0] + 1.0f);
			float fTemp58 = fTemp57 + 8.500005f;
			float fTemp59 = std::floor(fTemp58);
			float fTemp60 = fTemp57 + (7.0f - fTemp59);
			float fTemp61 = fTemp57 + (8.0f - fTemp59);
			int iTemp62 = int(fTemp58);
			float fTemp63 = fTemp57 + (9.0f - fTemp59);
			float fTemp64 = fTemp57 + (1e+01f - fTemp59);
			float fTemp65 = fTemp64 * fTemp63;
			float fTemp66 = fTemp65 * fTemp61;
			float fTemp67 = (fTemp57 + (6.0f - fTemp59)) * (fTemp60 * (fTemp61 * (0.041666668f * fRec3[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp62)) + 1)) & 1023] * fTemp63 - 0.16666667f * fTemp64 * fRec3[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp62 + 1)) + 1)) & 1023]) + 0.25f * fTemp65 * fRec3[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp62 + 2)) + 1)) & 1023]) - 0.16666667f * fTemp66 * fRec3[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp62 + 3)) + 1)) & 1023]) + 0.041666668f * fTemp66 * fTemp60 * fRec3[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp62 + 4)) + 1)) & 1023];
			fVec6[IOTA0 & 131071] = fTemp67;
			float fTemp68 = ((fRec52[1] != 0.0f) ? (((fRec53[1] > 0.0f) & (fRec53[1] < 1.0f)) ? fRec52[1] : 0.0f) : (((fRec53[1] == 0.0f) & (fSlow34 != fRec54[1])) ? 4.5351473e-05f : (((fRec53[1] == 1.0f) & (fSlow34 != fRec55[1])) ? -4.5351473e-05f : 0.0f)));
			fRec52[0] = fTemp68;
			fRec53[0] = std::max<float>(0.0f, std::min<float>(1.0f, fRec53[1] + fTemp68));
			fRec54[0] = (((fRec53[1] >= 1.0f) & (fRec55[1] != fSlow34)) ? fSlow34 : fRec54[1]);
			fRec55[0] = (((fRec53[1] <= 0.0f) & (fRec54[1] != fSlow34)) ? fSlow34 : fRec55[1]);
			int iTemp69 = int(std::min<float>(65536.0f, std::max<float>(0.0f, fRec54[0])));
			float fTemp70 = fVec6[(IOTA0 - iTemp69) & 131071];
			int iTemp71 = int(std::min<float>(65536.0f, std::max<float>(0.0f, fRec55[0])));
			float fTemp72 = fTemp51 + 0.5f * fTemp52 * (fTemp70 + fRec53[0] * (fVec6[(IOTA0 - iTemp71) & 131071] - fTemp70));
			float fTemp73 = fTemp14 * fTemp72 - fTemp23 * fRec36[1];
			float fTemp74 = fTemp14 * fTemp73 - fTemp23 * fRec40[1];
			float fTemp75 = fTemp14 * fTemp74 - fTemp23 * fRec44[1];
			fVec7[IOTA0 & 16383] = fTemp23 * fRec48[1] - fTemp14 * fTemp75;
			fRec56[0] = fSlow37 + 0.9999f * (fRec56[1] + float(iSlow36 * iTemp7));
			float fTemp76 = fRec56[0] + -1.49999f;
			float fTemp77 = fVec7[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp76)))) & 16383];
			fVec8[0] = fTemp77;
			float fTemp78 = std::floor(fTemp76);
			fRec49[0] = fVec8[1] + (fTemp78 + (2.0f - fRec56[0])) * (fTemp77 - fRec49[1]) / (fRec56[0] - fTemp78);
			fRec47[0] = fRec49[0];
			float fTemp79 = fConst3 * fTemp53 * (fRec51[0] + 1.0f);
			float fTemp80 = fTemp79 + 8.500005f;
			float fTemp81 = std::floor(fTemp80);
			float fTemp82 = fTemp79 + (7.0f - fTemp81);
			float fTemp83 = fTemp79 + (8.0f - fTemp81);
			int iTemp84 = int(fTemp80);
			float fTemp85 = fTemp79 + (9.0f - fTemp81);
			float fTemp86 = fTemp79 + (1e+01f - fTemp81);
			float fTemp87 = fTemp86 * fTemp85;
			float fTemp88 = fTemp87 * fTemp83;
			float fTemp89 = (fTemp79 + (6.0f - fTemp81)) * (fTemp82 * (fTemp83 * (0.041666668f * fRec2[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp84)) + 1)) & 1023] * fTemp85 - 0.16666667f * fTemp86 * fRec2[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp84 + 1)) + 1)) & 1023]) + 0.25f * fTemp87 * fRec2[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp84 + 2)) + 1)) & 1023]) - 0.16666667f * fTemp88 * fRec2[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp84 + 3)) + 1)) & 1023]) + 0.041666668f * fTemp88 * fTemp82 * fRec2[(IOTA0 - (std::min<int>(512, std::max<int>(0, iTemp84 + 4)) + 1)) & 1023];
			fVec9[IOTA0 & 131071] = fTemp89;
			float fTemp90 = fVec9[(IOTA0 - iTemp69) & 131071];
			float fTemp91 = fTemp0 + 0.5f * (fTemp90 + fRec53[0] * (fVec9[(IOTA0 - iTemp71) & 131071] - fTemp90)) * fTemp52;
			float fTemp92 = fTemp91 * fTemp14 - fTemp23 * fRec35[1];
			float fTemp93 = fTemp14 * fTemp92 - fTemp23 * fRec39[1];
			float fTemp94 = fTemp14 * fTemp93 - fTemp23 * fRec43[1];
			fVec10[IOTA0 & 16383] = fTemp14 * fTemp94 - fTemp23 * fRec47[1];
			fRec58[0] = fSlow39 + 0.9999f * (fRec58[1] + float(iSlow38 * iTemp7));
			float fTemp95 = fRec58[0] + -1.49999f;
			float fTemp96 = fVec10[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp95)))) & 16383];
			fVec11[0] = fTemp96;
			float fTemp97 = std::floor(fTemp95);
			fRec57[0] = fVec11[1] - (fTemp97 + (2.0f - fRec58[0])) * (fRec57[1] - fTemp96) / (fRec58[0] - fTemp97);
			fRec48[0] = fRec57[0];
			fVec12[IOTA0 & 16383] = fTemp14 * fRec48[1] + fTemp23 * fTemp75;
			float fTemp98 = fVec12[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp47)))) & 16383];
			fVec13[0] = fTemp98;
			fRec45[0] = -(fRec45[1] * fTemp49 / fTemp50 + fTemp49 * fTemp98 / fTemp50 + fVec13[1]);
			fRec43[0] = fRec45[0];
			fVec14[IOTA0 & 16383] = fRec47[1] * fTemp14 + fTemp23 * fTemp94;
			fRec60[0] = fSlow41 + 0.999f * (fRec60[1] + float(iSlow40 * iTemp7));
			float fTemp99 = fRec60[0] + -1.49999f;
			float fTemp100 = fVec14[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp99)))) & 16383];
			fVec15[0] = fTemp100;
			float fTemp101 = std::floor(fTemp99);
			fRec59[0] = fVec15[1] - (fTemp101 + (2.0f - fRec60[0])) * (fRec59[1] - fTemp100) / (fRec60[0] - fTemp101);
			fRec44[0] = fRec59[0];
			fVec16[IOTA0 & 16383] = fTemp14 * fRec44[1] + fTemp23 * fTemp74;
			float fTemp102 = fVec16[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp43)))) & 16383];
			fVec17[0] = fTemp102;
			fRec41[0] = -(fRec41[1] * fTemp45 / fTemp46 + fTemp45 * fTemp102 / fTemp46 + fVec17[1]);
			fRec39[0] = fRec41[0];
			fVec18[IOTA0 & 16383] = fRec43[1] * fTemp14 + fTemp23 * fTemp93;
			fRec62[0] = fSlow43 + 0.999f * (fRec62[1] + float(iSlow42 * iTemp7));
			float fTemp103 = fRec62[0] + -1.49999f;
			float fTemp104 = fVec18[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp103)))) & 16383];
			fVec19[0] = fTemp104;
			float fTemp105 = std::floor(fTemp103);
			fRec61[0] = fVec19[1] - (fTemp105 + (2.0f - fRec62[0])) * (fRec61[1] - fTemp104) / (fRec62[0] - fTemp105);
			fRec40[0] = fRec61[0];
			fVec20[IOTA0 & 16383] = fTemp14 * fRec40[1] + fTemp23 * fTemp73;
			float fTemp106 = fVec20[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp39)))) & 16383];
			fVec21[0] = fTemp106;
			fRec37[0] = -(fRec37[1] * fTemp41 / fTemp42 + fTemp41 * fTemp106 / fTemp42 + fVec21[1]);
			fRec35[0] = fRec37[0];
			fVec22[IOTA0 & 16383] = fRec39[1] * fTemp14 + fTemp23 * fTemp92;
			fRec64[0] = fSlow45 + 0.999f * (fRec64[1] + float(iSlow44 * iTemp7));
			float fTemp107 = fRec64[0] + -1.49999f;
			float fTemp108 = fVec22[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp107)))) & 16383];
			fVec23[0] = fTemp108;
			float fTemp109 = std::floor(fTemp107);
			fRec63[0] = fVec23[1] - (fTemp109 + (2.0f - fRec64[0])) * (fRec63[1] - fTemp108) / (fRec64[0] - fTemp109);
			fRec36[0] = fRec63[0];
			float fTemp110 = fTemp14 * fRec36[1] + fTemp23 * fTemp72;
			float fTemp111 = fTemp25 * fTemp110 - fTemp38 * fRec21[1];
			float fTemp112 = fTemp25 * fTemp111 - fTemp38 * fRec25[1];
			float fTemp113 = fTemp25 * fTemp112 - fTemp38 * fRec29[1];
			fVec24[IOTA0 & 16383] = fTemp38 * fRec33[1] - fTemp25 * fTemp113;
			fRec65[0] = fSlow48 + 0.9999f * (fRec65[1] + float(iSlow47 * iTemp7));
			float fTemp114 = fRec65[0] + -1.49999f;
			float fTemp115 = fVec24[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp114)))) & 16383];
			fVec25[0] = fTemp115;
			float fTemp116 = std::floor(fTemp114);
			fRec34[0] = fVec25[1] + (fTemp116 + (2.0f - fRec65[0])) * (fTemp115 - fRec34[1]) / (fRec65[0] - fTemp116);
			fRec32[0] = fRec34[0];
			float fTemp117 = fRec35[1] * fTemp14 + fTemp23 * fTemp91;
			float fTemp118 = fTemp117 * fTemp25 - fTemp38 * fRec20[1];
			float fTemp119 = fTemp25 * fTemp118 - fTemp38 * fRec24[1];
			float fTemp120 = fTemp25 * fTemp119 - fTemp38 * fRec28[1];
			fVec26[IOTA0 & 16383] = fTemp25 * fTemp120 - fRec32[1] * fTemp38;
			fRec67[0] = fSlow50 + 0.9999f * (fRec67[1] + float(iSlow49 * iTemp7));
			float fTemp121 = fRec67[0] + -1.49999f;
			float fTemp122 = fVec26[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp121)))) & 16383];
			fVec27[0] = fTemp122;
			float fTemp123 = std::floor(fTemp121);
			fRec66[0] = fVec27[1] - (fTemp123 + (2.0f - fRec67[0])) * (fRec66[1] - fTemp122) / (fRec67[0] - fTemp123);
			fRec33[0] = fRec66[0];
			fVec28[IOTA0 & 16383] = fTemp25 * fRec33[1] + fTemp38 * fTemp113;
			float fTemp124 = fVec28[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp34)))) & 16383];
			fVec29[0] = fTemp124;
			fRec30[0] = -(fRec30[1] * fTemp36 / fTemp37 + fTemp36 * fTemp124 / fTemp37 + fVec29[1]);
			fRec28[0] = fRec30[0];
			fVec30[IOTA0 & 16383] = fRec32[1] * fTemp25 + fTemp38 * fTemp120;
			fRec69[0] = fSlow52 + 0.999f * (fRec69[1] + float(iSlow51 * iTemp7));
			float fTemp125 = fRec69[0] + -1.49999f;
			float fTemp126 = fVec30[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp125)))) & 16383];
			fVec31[0] = fTemp126;
			float fTemp127 = std::floor(fTemp125);
			fRec68[0] = fVec31[1] - (fTemp127 + (2.0f - fRec69[0])) * (fRec68[1] - fTemp126) / (fRec69[0] - fTemp127);
			fRec29[0] = fRec68[0];
			fVec32[IOTA0 & 16383] = fTemp25 * fRec29[1] + fTemp38 * fTemp112;
			float fTemp128 = fVec32[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp30)))) & 16383];
			fVec33[0] = fTemp128;
			fRec26[0] = -(fRec26[1] * fTemp32 / fTemp33 + fTemp32 * fTemp128 / fTemp33 + fVec33[1]);
			fRec24[0] = fRec26[0];
			fVec34[IOTA0 & 16383] = fRec28[1] * fTemp25 + fTemp38 * fTemp119;
			fRec71[0] = fSlow54 + 0.999f * (fRec71[1] + float(iSlow53 * iTemp7));
			float fTemp129 = fRec71[0] + -1.49999f;
			float fTemp130 = fVec34[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp129)))) & 16383];
			fVec35[0] = fTemp130;
			float fTemp131 = std::floor(fTemp129);
			fRec70[0] = fVec35[1] - (fTemp131 + (2.0f - fRec71[0])) * (fRec70[1] - fTemp130) / (fRec71[0] - fTemp131);
			fRec25[0] = fRec70[0];
			fVec36[IOTA0 & 16383] = fTemp25 * fRec25[1] + fTemp38 * fTemp111;
			float fTemp132 = fVec36[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp26)))) & 16383];
			fVec37[0] = fTemp132;
			fRec22[0] = -(fRec22[1] * fTemp28 / fTemp29 + fTemp28 * fTemp132 / fTemp29 + fVec37[1]);
			fRec20[0] = fRec22[0];
			fVec38[IOTA0 & 16383] = fRec24[1] * fTemp25 + fTemp38 * fTemp118;
			fRec73[0] = fSlow56 + 0.999f * (fRec73[1] + float(iSlow55 * iTemp7));
			float fTemp133 = fRec73[0] + -1.49999f;
			float fTemp134 = fVec38[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp133)))) & 16383];
			fVec39[0] = fTemp134;
			float fTemp135 = std::floor(fTemp133);
			fRec72[0] = fVec39[1] - (fTemp135 + (2.0f - fRec73[0])) * (fRec72[1] - fTemp134) / (fRec73[0] - fTemp135);
			fRec21[0] = fRec72[0];
			float fTemp136 = fTemp25 * fRec21[1] + fTemp38 * fTemp110;
			float fTemp137 = fTemp14 * fTemp136 - fTemp23 * fRec6[1];
			float fTemp138 = fTemp14 * fTemp137 - fTemp23 * fRec10[1];
			float fTemp139 = fTemp14 * fTemp138 - fTemp23 * fRec14[1];
			fVec40[IOTA0 & 16383] = fTemp23 * fRec18[1] - fTemp14 * fTemp139;
			fRec74[0] = fSlow59 + 0.9999f * (fRec74[1] + float(iSlow58 * iTemp7));
			float fTemp140 = fRec74[0] + -1.49999f;
			float fTemp141 = fVec40[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp140)))) & 16383];
			fVec41[0] = fTemp141;
			float fTemp142 = std::floor(fTemp140);
			fRec19[0] = fVec41[1] - (fTemp142 + (2.0f - fRec74[0])) * (fRec19[1] - fTemp141) / (fRec74[0] - fTemp142);
			fRec17[0] = fRec19[0];
			float fTemp143 = fRec20[1] * fTemp25 + fTemp38 * fTemp117;
			float fTemp144 = fTemp14 * fTemp143 - fTemp23 * fRec5[1];
			float fTemp145 = fTemp14 * fTemp144 - fTemp23 * fRec9[1];
			float fTemp146 = fTemp14 * fTemp145 - fTemp23 * fRec13[1];
			fVec42[IOTA0 & 16383] = fTemp14 * fTemp146 - fRec17[1] * fTemp23;
			fRec76[0] = fSlow61 + 0.9999f * (fRec76[1] + float(iSlow60 * iTemp7));
			float fTemp147 = fRec76[0] + -1.49999f;
			float fTemp148 = fVec42[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp147)))) & 16383];
			fVec43[0] = fTemp148;
			float fTemp149 = std::floor(fTemp147);
			fRec75[0] = fVec43[1] - (fTemp149 + (2.0f - fRec76[0])) * (fRec75[1] - fTemp148) / (fRec76[0] - fTemp149);
			fRec18[0] = fRec75[0];
			fVec44[IOTA0 & 16383] = fTemp14 * fRec18[1] + fTemp23 * fTemp139;
			float fTemp150 = fVec44[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp19)))) & 16383];
			fVec45[0] = fTemp150;
			fRec15[0] = -(fRec15[1] * fTemp21 / fTemp22 + fTemp21 * fTemp150 / fTemp22 + fVec45[1]);
			fRec13[0] = fRec15[0];
			fVec46[IOTA0 & 16383] = fRec17[1] * fTemp14 + fTemp23 * fTemp146;
			fRec78[0] = fSlow63 + 0.999f * (fRec78[1] + float(iSlow62 * iTemp7));
			float fTemp151 = fRec78[0] + -1.49999f;
			float fTemp152 = fVec46[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp151)))) & 16383];
			fVec47[0] = fTemp152;
			float fTemp153 = std::floor(fTemp151);
			fRec77[0] = fVec47[1] - (fTemp153 + (2.0f - fRec78[0])) * (fRec77[1] - fTemp152) / (fRec78[0] - fTemp153);
			fRec14[0] = fRec77[0];
			fVec48[IOTA0 & 16383] = fTemp14 * fRec14[1] + fTemp23 * fTemp138;
			float fTemp154 = fVec48[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp15)))) & 16383];
			fVec49[0] = fTemp154;
			fRec11[0] = -(fRec11[1] * fTemp17 / fTemp18 + fTemp17 * fTemp154 / fTemp18 + fVec49[1]);
			fRec9[0] = fRec11[0];
			fVec50[IOTA0 & 16383] = fRec13[1] * fTemp14 + fTemp23 * fTemp145;
			fRec80[0] = fSlow65 + 0.999f * (fRec80[1] + float(iSlow64 * iTemp7));
			float fTemp155 = fRec80[0] + -1.49999f;
			float fTemp156 = fVec50[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp155)))) & 16383];
			fVec51[0] = fTemp156;
			float fTemp157 = std::floor(fTemp155);
			fRec79[0] = fVec51[1] - (fTemp157 + (2.0f - fRec80[0])) * (fRec79[1] - fTemp156) / (fRec80[0] - fTemp157);
			fRec10[0] = fRec79[0];
			fVec52[IOTA0 & 16383] = fTemp14 * fRec10[1] + fTemp23 * fTemp137;
			float fTemp158 = fVec52[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp8)))) & 16383];
			fVec53[0] = fTemp158;
			fRec7[0] = -(fRec7[1] * fTemp10 / fTemp11 + fTemp10 * fTemp158 / fTemp11 + fVec53[1]);
			fRec5[0] = fRec7[0];
			fVec54[IOTA0 & 16383] = fRec9[1] * fTemp14 + fTemp23 * fTemp144;
			fRec82[0] = fSlow67 + 0.999f * (fRec82[1] + float(iSlow66 * iTemp7));
			float fTemp159 = fRec82[0] + -1.49999f;
			float fTemp160 = fVec54[(IOTA0 - std::min<int>(8192, std::max<int>(0, int(fTemp159)))) & 16383];
			fVec55[0] = fTemp160;
			float fTemp161 = std::floor(fTemp159);
			fRec81[0] = fVec55[1] - (fTemp161 + (2.0f - fRec82[0])) * (fRec81[1] - fTemp160) / (fRec82[0] - fTemp161);
			fRec6[0] = fRec81[0];
			fRec4[0] = fTemp6 * (fRec5[1] * fTemp14 + fTemp23 * fTemp143) + 0.5f * fTemp5 * fRec4[1];
			fRec2[IOTA0 & 1023] = fRec4[0];
			fRec83[0] = fTemp6 * (fTemp14 * fRec6[1] + fTemp23 * fTemp136) + 0.5f * fTemp5 * fRec83[1];
			fRec3[IOTA0 & 1023] = fRec83[0];
			output0[i0] = FAUSTFLOAT(fTemp0 * fRec0[0] + fTemp3 * (fTemp0 * fTemp4 + fRec1[0] * fRec2[IOTA0 & 1023]));
			output1[i0] = FAUSTFLOAT(fTemp51 * fRec0[0] + fTemp3 * (fTemp51 * fTemp4 + fRec1[0] * fRec3[IOTA0 & 1023]));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fVec1[1] = fVec1[0];
			fRec8[1] = fRec8[0];
			fVec2[1] = fVec2[0];
			fRec12[1] = fRec12[0];
			fRec16[1] = fRec16[0];
			fRec23[1] = fRec23[0];
			fRec27[1] = fRec27[0];
			fRec31[1] = fRec31[0];
			fRec38[1] = fRec38[0];
			fRec42[1] = fRec42[0];
			fRec46[1] = fRec46[0];
			fVec3[1] = fVec3[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec50[1] = fRec50[0];
			fRec51[1] = fRec51[0];
			IOTA0 = IOTA0 + 1;
			fRec52[1] = fRec52[0];
			fRec53[1] = fRec53[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			fRec56[1] = fRec56[0];
			fVec8[1] = fVec8[0];
			fRec49[1] = fRec49[0];
			fRec47[1] = fRec47[0];
			fRec58[1] = fRec58[0];
			fVec11[1] = fVec11[0];
			fRec57[1] = fRec57[0];
			fRec48[1] = fRec48[0];
			fVec13[1] = fVec13[0];
			fRec45[1] = fRec45[0];
			fRec43[1] = fRec43[0];
			fRec60[1] = fRec60[0];
			fVec15[1] = fVec15[0];
			fRec59[1] = fRec59[0];
			fRec44[1] = fRec44[0];
			fVec17[1] = fVec17[0];
			fRec41[1] = fRec41[0];
			fRec39[1] = fRec39[0];
			fRec62[1] = fRec62[0];
			fVec19[1] = fVec19[0];
			fRec61[1] = fRec61[0];
			fRec40[1] = fRec40[0];
			fVec21[1] = fVec21[0];
			fRec37[1] = fRec37[0];
			fRec35[1] = fRec35[0];
			fRec64[1] = fRec64[0];
			fVec23[1] = fVec23[0];
			fRec63[1] = fRec63[0];
			fRec36[1] = fRec36[0];
			fRec65[1] = fRec65[0];
			fVec25[1] = fVec25[0];
			fRec34[1] = fRec34[0];
			fRec32[1] = fRec32[0];
			fRec67[1] = fRec67[0];
			fVec27[1] = fVec27[0];
			fRec66[1] = fRec66[0];
			fRec33[1] = fRec33[0];
			fVec29[1] = fVec29[0];
			fRec30[1] = fRec30[0];
			fRec28[1] = fRec28[0];
			fRec69[1] = fRec69[0];
			fVec31[1] = fVec31[0];
			fRec68[1] = fRec68[0];
			fRec29[1] = fRec29[0];
			fVec33[1] = fVec33[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			fRec71[1] = fRec71[0];
			fVec35[1] = fVec35[0];
			fRec70[1] = fRec70[0];
			fRec25[1] = fRec25[0];
			fVec37[1] = fVec37[0];
			fRec22[1] = fRec22[0];
			fRec20[1] = fRec20[0];
			fRec73[1] = fRec73[0];
			fVec39[1] = fVec39[0];
			fRec72[1] = fRec72[0];
			fRec21[1] = fRec21[0];
			fRec74[1] = fRec74[0];
			fVec41[1] = fVec41[0];
			fRec19[1] = fRec19[0];
			fRec17[1] = fRec17[0];
			fRec76[1] = fRec76[0];
			fVec43[1] = fVec43[0];
			fRec75[1] = fRec75[0];
			fRec18[1] = fRec18[0];
			fVec45[1] = fVec45[0];
			fRec15[1] = fRec15[0];
			fRec13[1] = fRec13[0];
			fRec78[1] = fRec78[0];
			fVec47[1] = fVec47[0];
			fRec77[1] = fRec77[0];
			fRec14[1] = fRec14[0];
			fVec49[1] = fVec49[0];
			fRec11[1] = fRec11[0];
			fRec9[1] = fRec9[0];
			fRec80[1] = fRec80[0];
			fVec51[1] = fVec51[0];
			fRec79[1] = fRec79[0];
			fRec10[1] = fRec10[0];
			fVec53[1] = fVec53[0];
			fRec7[1] = fRec7[0];
			fRec5[1] = fRec5[0];
			fRec82[1] = fRec82[0];
			fVec55[1] = fVec55[0];
			fRec81[1] = fRec81[0];
			fRec6[1] = fRec6[0];
			fRec4[1] = fRec4[0];
			fRec83[1] = fRec83[0];
		}
	}

};
#endif

class faust_fx_greyhole_tilde : public FaustExternal<fx_greyhole> {
public:
    faust_fx_greyhole_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(fx_greyhole))
    {
    }
};

#endif
