/* ------------------------------------------------------------
author: "Lukas Hartmann & Luca Hilbrich"
name: "fx.granulator"
version: "2.8.1"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn fx_granulator -scn fx_granulator_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
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
        virtual fx_granulator_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_granulator_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_granulator_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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


class fx_granulator : public fx_granulator_dsp {
	
 private:
	
	FAUSTFLOAT fVslider0;
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fVslider1;
	int iVec0[2];
	FAUSTFLOAT fVslider2;
	float ftbl0[132300];
	FAUSTFLOAT fVslider3;
	float fRec1[2];
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fVslider5;
	float fRec2[2];
	FAUSTFLOAT fVslider6;
	int iRec4[2];
	int iRec3[2];
	int iRec20[2];
	int iRec21[2];
	int iRec22[2];
	int iRec23[2];
	int iRec24[2];
	int iRec25[2];
	int iRec26[2];
	int iRec27[2];
	int iRec28[2];
	int iRec29[2];
	int iRec30[2];
	int iRec31[2];
	int iRec32[2];
	int iRec33[2];
	int iRec34[2];
	int iRec35[2];
	int iRec36[2];
	int iRec37[2];
	int iRec38[2];
	int iRec39[2];
	int iRec40[2];
	int iRec41[2];
	int iRec42[2];
	int iRec43[2];
	int iRec44[2];
	int iRec45[2];
	int iRec46[2];
	int iRec47[2];
	int iRec48[2];
	int iRec49[2];
	int iRec50[2];
	float fVec1[2];
	float fRec0[2];
	FAUSTFLOAT fVslider7;
	FAUSTFLOAT fVslider8;
	FAUSTFLOAT fVslider9;
	float fRec52[2];
	int iRec53[2];
	int iRec54[2];
	int iRec55[2];
	int iRec56[2];
	int iRec57[2];
	int iRec58[2];
	int iRec59[2];
	int iRec60[2];
	int iRec61[2];
	int iRec62[2];
	int iRec63[2];
	int iRec64[2];
	int iRec65[2];
	int iRec66[2];
	int iRec67[2];
	int iRec68[2];
	float fVec2[2];
	float fRec51[2];
	FAUSTFLOAT fVslider10;
	float fVec3[2];
	float fRec69[2];
	FAUSTFLOAT fVslider11;
	float fVec4[2];
	float fRec70[2];
	
 public:
	fx_granulator() {
	}
	
	void metadata(Meta* m) { 
		m->declare("author", "Lukas Hartmann & Luca Hilbrich");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn fx_granulator -scn fx_granulator_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "fx_granulator.dsp");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "fx.granulator");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("version", "2.8.1");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = 3.1415927f / std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fx_granulatorSIG0* sig0 = newfx_granulatorSIG0();
		sig0->instanceInitfx_granulatorSIG0(sample_rate);
		sig0->fillfx_granulatorSIG0(132300, ftbl0);
		deletefx_granulatorSIG0(sig0);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.8f);
		fVslider1 = FAUSTFLOAT(1e+02f);
		fVslider2 = FAUSTFLOAT(1.0f);
		fVslider3 = FAUSTFLOAT(2.0f);
		fVslider4 = FAUSTFLOAT(1.0f);
		fVslider5 = FAUSTFLOAT(0.1f);
		fVslider6 = FAUSTFLOAT(1.0f);
		fVslider7 = FAUSTFLOAT(0.2f);
		fVslider8 = FAUSTFLOAT(1e+02f);
		fVslider9 = FAUSTFLOAT(1.0f);
		fVslider10 = FAUSTFLOAT(0.2f);
		fVslider11 = FAUSTFLOAT(0.8f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec4[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec3[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec20[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iRec21[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iRec22[l7] = 0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			iRec23[l8] = 0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			iRec24[l9] = 0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			iRec25[l10] = 0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			iRec26[l11] = 0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			iRec27[l12] = 0;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			iRec28[l13] = 0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			iRec29[l14] = 0;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			iRec30[l15] = 0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			iRec31[l16] = 0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			iRec32[l17] = 0;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			iRec33[l18] = 0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			iRec34[l19] = 0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			iRec35[l20] = 0;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			iRec36[l21] = 0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			iRec37[l22] = 0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			iRec38[l23] = 0;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			iRec39[l24] = 0;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			iRec40[l25] = 0;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			iRec41[l26] = 0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			iRec42[l27] = 0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			iRec43[l28] = 0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			iRec44[l29] = 0;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			iRec45[l30] = 0;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			iRec46[l31] = 0;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			iRec47[l32] = 0;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			iRec48[l33] = 0;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			iRec49[l34] = 0;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			iRec50[l35] = 0;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fVec1[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec0[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec52[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			iRec53[l39] = 0;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			iRec54[l40] = 0;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			iRec55[l41] = 0;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			iRec56[l42] = 0;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			iRec57[l43] = 0;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			iRec58[l44] = 0;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			iRec59[l45] = 0;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			iRec60[l46] = 0;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			iRec61[l47] = 0;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			iRec62[l48] = 0;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			iRec63[l49] = 0;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			iRec64[l50] = 0;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			iRec65[l51] = 0;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			iRec66[l52] = 0;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			iRec67[l53] = 0;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			iRec68[l54] = 0;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fVec2[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec51[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fVec3[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec69[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fVec4[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec70[l60] = 0.0f;
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
		ui_interface->addVerticalSlider("delay", &fVslider3, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("density", &fVslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(16.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->addVerticalSlider("gainAL", &fVslider0, FAUSTFLOAT(0.8f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider10, "style", "knob");
		ui_interface->addVerticalSlider("gainAR", &fVslider10, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider7, "style", "knob");
		ui_interface->addVerticalSlider("gainBL", &fVslider7, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider11, "style", "knob");
		ui_interface->addVerticalSlider("gainBR", &fVslider11, FAUSTFLOAT(0.8f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "hz");
		ui_interface->addVerticalSlider("hpfA", &fVslider1, FAUSTFLOAT(1e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+04f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fVslider8, "style", "knob");
		ui_interface->declare(&fVslider8, "unit", "hz");
		ui_interface->addVerticalSlider("hpfB", &fVslider8, FAUSTFLOAT(1e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+04f), FAUSTFLOAT(1.0f));
		ui_interface->addVerticalSlider("pitchA", &fVslider4, FAUSTFLOAT(1.0f), FAUSTFLOAT(-2.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("pitchB", &fVslider9, FAUSTFLOAT(1.0f), FAUSTFLOAT(-2.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("size", &fVslider5, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.01f), FAUSTFLOAT(0.5f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("spread", &fVslider6, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fVslider0);
		float fSlow1 = 1.0f / std::tan(fConst0 * float(fVslider1));
		float fSlow2 = 1.0f / (fSlow1 + 1.0f);
		float fSlow3 = 1.0f - fSlow1;
		float fSlow4 = float(fVslider2);
		float fSlow5 = 0.8f / fSlow4;
		float fSlow6 = float(0.0f < fSlow4);
		float fSlow7 = float(fVslider3);
		float fSlow8 = 4.41e+04f * fSlow7;
		float fSlow9 = float(fVslider4);
		float fSlow10 = 4.41e+04f * float(fVslider5);
		float fSlow11 = 2.205e+04f * fSlow7 * float(fVslider6);
		float fSlow12 = fSlow10 + -1.0f;
		float fSlow13 = 1.0f / fSlow12;
		float fSlow14 = 6.28318f / fSlow12;
		float fSlow15 = float(1.0f < fSlow4);
		float fSlow16 = float(2.0f < fSlow4);
		float fSlow17 = float(3.0f < fSlow4);
		float fSlow18 = float(4.0f < fSlow4);
		float fSlow19 = float(5.0f < fSlow4);
		float fSlow20 = float(6.0f < fSlow4);
		float fSlow21 = float(7.0f < fSlow4);
		float fSlow22 = float(8.0f < fSlow4);
		float fSlow23 = float(9.0f < fSlow4);
		float fSlow24 = float(1e+01f < fSlow4);
		float fSlow25 = float(11.0f < fSlow4);
		float fSlow26 = float(12.0f < fSlow4);
		float fSlow27 = float(13.0f < fSlow4);
		float fSlow28 = float(14.0f < fSlow4);
		float fSlow29 = float(15.0f < fSlow4);
		float fSlow30 = float(fVslider7);
		float fSlow31 = 1.0f / std::tan(fConst0 * float(fVslider8));
		float fSlow32 = 1.0f / (fSlow31 + 1.0f);
		float fSlow33 = 0.85f / fSlow4;
		float fSlow34 = float(fVslider9);
		float fSlow35 = 1.0f - fSlow31;
		float fSlow36 = float(fVslider10);
		float fSlow37 = 0.9f / fSlow4;
		float fSlow38 = float(fVslider11);
		float fSlow39 = 0.95f / fSlow4;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec1[0] = std::fmod(fRec1[1] + 1.0f, fSlow8);
			ftbl0[int(std::fmod(float(int(fRec1[0])), fSlow8))] = float(input0[i0]);
			fRec2[0] = std::fmod(fSlow9 + fRec2[1], fSlow10);
			int iTemp0 = 1 - iVec0[1];
			int iTemp1 = 1103515245 * iRec4[1] + 12345;
			int iTemp2 = 1103515245 * iTemp1 + 12345;
			int iTemp3 = 1103515245 * iTemp2 + 12345;
			int iTemp4 = 1103515245 * iTemp3 + 12345;
			int iTemp5 = 1103515245 * iTemp4 + 12345;
			int iTemp6 = 1103515245 * iTemp5 + 12345;
			int iTemp7 = 1103515245 * iTemp6 + 12345;
			int iTemp8 = 1103515245 * iTemp7 + 12345;
			int iTemp9 = 1103515245 * iTemp8 + 12345;
			int iTemp10 = 1103515245 * iTemp9 + 12345;
			int iTemp11 = 1103515245 * iTemp10 + 12345;
			int iTemp12 = 1103515245 * iTemp11 + 12345;
			int iTemp13 = 1103515245 * iTemp12 + 12345;
			int iTemp14 = 1103515245 * iTemp13 + 12345;
			int iTemp15 = 1103515245 * iTemp14 + 12345;
			int iTemp16 = 1103515245 * iTemp15 + 12345;
			iRec4[0] = 1103515245 * iTemp16 + 12345;
			int iRec5 = iTemp16;
			int iRec6 = iTemp15;
			int iRec7 = iTemp14;
			int iRec8 = iTemp13;
			int iRec9 = iTemp12;
			int iRec10 = iTemp11;
			int iRec11 = iTemp10;
			int iRec12 = iTemp9;
			int iRec13 = iTemp8;
			int iRec14 = iTemp7;
			int iRec15 = iTemp6;
			int iRec16 = iTemp5;
			int iRec17 = iTemp4;
			int iRec18 = iTemp3;
			int iRec19 = iTemp2;
			int iTemp17 = int(fSlow11 * (2.3283064e-10f * float(iRec4[0]) + 1.0f));
			iRec3[0] = iVec0[1] * iRec3[1] + iTemp0 * iTemp17;
			float fTemp18 = float(iRec3[0]);
			float fTemp19 = std::fmod(fRec2[0] + fTemp18, fSlow10);
			int iTemp20 = int(fSlow13 * fTemp19);
			iRec20[0] = iRec20[1] * (1 - iTemp20) + iTemp17 * iTemp20;
			int iTemp21 = int(fSlow11 * (2.3283064e-10f * float(iRec5) + 1.0f));
			iRec21[0] = iVec0[1] * iRec21[1] + iTemp0 * iTemp21;
			float fTemp22 = float(iRec21[0]);
			float fTemp23 = std::fmod(fRec2[0] + fTemp22, fSlow10);
			int iTemp24 = int(fSlow13 * fTemp23);
			iRec22[0] = iRec22[1] * (1 - iTemp24) + iTemp21 * iTemp24;
			int iTemp25 = int(fSlow11 * (2.3283064e-10f * float(iRec6) + 1.0f));
			iRec23[0] = iVec0[1] * iRec23[1] + iTemp0 * iTemp25;
			float fTemp26 = float(iRec23[0]);
			float fTemp27 = std::fmod(fRec2[0] + fTemp26, fSlow10);
			int iTemp28 = int(fSlow13 * fTemp27);
			iRec24[0] = iRec24[1] * (1 - iTemp28) + iTemp25 * iTemp28;
			int iTemp29 = int(fSlow11 * (2.3283064e-10f * float(iRec7) + 1.0f));
			iRec25[0] = iVec0[1] * iRec25[1] + iTemp0 * iTemp29;
			float fTemp30 = float(iRec25[0]);
			float fTemp31 = std::fmod(fRec2[0] + fTemp30, fSlow10);
			int iTemp32 = int(fSlow13 * fTemp31);
			iRec26[0] = iRec26[1] * (1 - iTemp32) + iTemp29 * iTemp32;
			int iTemp33 = int(fSlow11 * (2.3283064e-10f * float(iRec8) + 1.0f));
			iRec27[0] = iVec0[1] * iRec27[1] + iTemp0 * iTemp33;
			float fTemp34 = float(iRec27[0]);
			float fTemp35 = std::fmod(fRec2[0] + fTemp34, fSlow10);
			int iTemp36 = int(fSlow13 * fTemp35);
			iRec28[0] = iRec28[1] * (1 - iTemp36) + iTemp33 * iTemp36;
			int iTemp37 = int(fSlow11 * (2.3283064e-10f * float(iRec9) + 1.0f));
			iRec29[0] = iVec0[1] * iRec29[1] + iTemp0 * iTemp37;
			float fTemp38 = float(iRec29[0]);
			float fTemp39 = std::fmod(fRec2[0] + fTemp38, fSlow10);
			int iTemp40 = int(fSlow13 * fTemp39);
			iRec30[0] = iRec30[1] * (1 - iTemp40) + iTemp37 * iTemp40;
			int iTemp41 = int(fSlow11 * (2.3283064e-10f * float(iRec10) + 1.0f));
			iRec31[0] = iVec0[1] * iRec31[1] + iTemp0 * iTemp41;
			float fTemp42 = float(iRec31[0]);
			float fTemp43 = std::fmod(fRec2[0] + fTemp42, fSlow10);
			int iTemp44 = int(fSlow13 * fTemp43);
			iRec32[0] = iRec32[1] * (1 - iTemp44) + iTemp41 * iTemp44;
			int iTemp45 = int(fSlow11 * (2.3283064e-10f * float(iRec11) + 1.0f));
			iRec33[0] = iVec0[1] * iRec33[1] + iTemp0 * iTemp45;
			float fTemp46 = float(iRec33[0]);
			float fTemp47 = std::fmod(fRec2[0] + fTemp46, fSlow10);
			int iTemp48 = int(fSlow13 * fTemp47);
			iRec34[0] = iRec34[1] * (1 - iTemp48) + iTemp45 * iTemp48;
			int iTemp49 = int(fSlow11 * (2.3283064e-10f * float(iRec12) + 1.0f));
			iRec35[0] = iVec0[1] * iRec35[1] + iTemp0 * iTemp49;
			float fTemp50 = float(iRec35[0]);
			float fTemp51 = std::fmod(fRec2[0] + fTemp50, fSlow10);
			int iTemp52 = int(fSlow13 * fTemp51);
			iRec36[0] = iRec36[1] * (1 - iTemp52) + iTemp49 * iTemp52;
			int iTemp53 = int(fSlow11 * (2.3283064e-10f * float(iRec13) + 1.0f));
			iRec37[0] = iVec0[1] * iRec37[1] + iTemp0 * iTemp53;
			float fTemp54 = float(iRec37[0]);
			float fTemp55 = std::fmod(fRec2[0] + fTemp54, fSlow10);
			int iTemp56 = int(fSlow13 * fTemp55);
			iRec38[0] = iRec38[1] * (1 - iTemp56) + iTemp53 * iTemp56;
			int iTemp57 = int(fSlow11 * (2.3283064e-10f * float(iRec14) + 1.0f));
			iRec39[0] = iVec0[1] * iRec39[1] + iTemp0 * iTemp57;
			float fTemp58 = float(iRec39[0]);
			float fTemp59 = std::fmod(fRec2[0] + fTemp58, fSlow10);
			int iTemp60 = int(fSlow13 * fTemp59);
			iRec40[0] = iRec40[1] * (1 - iTemp60) + iTemp57 * iTemp60;
			int iTemp61 = int(fSlow11 * (2.3283064e-10f * float(iRec15) + 1.0f));
			iRec41[0] = iVec0[1] * iRec41[1] + iTemp0 * iTemp61;
			float fTemp62 = float(iRec41[0]);
			float fTemp63 = std::fmod(fRec2[0] + fTemp62, fSlow10);
			int iTemp64 = int(fSlow13 * fTemp63);
			iRec42[0] = iRec42[1] * (1 - iTemp64) + iTemp61 * iTemp64;
			int iTemp65 = int(fSlow11 * (2.3283064e-10f * float(iRec16) + 1.0f));
			iRec43[0] = iVec0[1] * iRec43[1] + iTemp0 * iTemp65;
			float fTemp66 = float(iRec43[0]);
			float fTemp67 = std::fmod(fRec2[0] + fTemp66, fSlow10);
			int iTemp68 = int(fSlow13 * fTemp67);
			iRec44[0] = iRec44[1] * (1 - iTemp68) + iTemp65 * iTemp68;
			int iTemp69 = int(fSlow11 * (2.3283064e-10f * float(iRec17) + 1.0f));
			iRec45[0] = iVec0[1] * iRec45[1] + iTemp0 * iTemp69;
			float fTemp70 = float(iRec45[0]);
			float fTemp71 = std::fmod(fRec2[0] + fTemp70, fSlow10);
			int iTemp72 = int(fSlow13 * fTemp71);
			iRec46[0] = iRec46[1] * (1 - iTemp72) + iTemp69 * iTemp72;
			int iTemp73 = int(fSlow11 * (2.3283064e-10f * float(iRec18) + 1.0f));
			iRec47[0] = iVec0[1] * iRec47[1] + iTemp0 * iTemp73;
			float fTemp74 = float(iRec47[0]);
			float fTemp75 = std::fmod(fRec2[0] + fTemp74, fSlow10);
			int iTemp76 = int(fSlow13 * fTemp75);
			iRec48[0] = iRec48[1] * (1 - iTemp76) + iTemp73 * iTemp76;
			int iTemp77 = int(fSlow11 * (2.3283064e-10f * float(iRec19) + 1.0f));
			iRec49[0] = iVec0[1] * iRec49[1] + iTemp0 * iTemp77;
			float fTemp78 = float(iRec49[0]);
			float fTemp79 = std::fmod(fRec2[0] + fTemp78, fSlow10);
			int iTemp80 = int(fSlow13 * fTemp79);
			iRec50[0] = iRec50[1] * (1 - iTemp80) + iTemp77 * iTemp80;
			float fTemp81 = fSlow6 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp19 + float(iRec20[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp19) + fSlow15 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp23 + float(iRec22[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp23) + fSlow16 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp27 + float(iRec24[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp27) + fSlow17 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp31 + float(iRec26[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp31) + fSlow18 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp35 + float(iRec28[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp35) + fSlow19 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp39 + float(iRec30[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp39) + fSlow20 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp43 + float(iRec32[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp43) + fSlow21 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp47 + float(iRec34[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp47) + fSlow22 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp51 + float(iRec36[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp51) + fSlow23 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp55 + float(iRec38[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp55) + fSlow24 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp59 + float(iRec40[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp59) + fSlow25 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp63 + float(iRec42[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp63) + fSlow26 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp67 + float(iRec44[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp67) + fSlow27 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp71 + float(iRec46[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp71) + fSlow28 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp75 + float(iRec48[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp75) + fSlow29 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp79 + float(iRec50[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp79);
			float fTemp82 = fSlow5 * fTemp81;
			fVec1[0] = fTemp82;
			fRec0[0] = -(fSlow2 * (fSlow3 * fRec0[1] - fSlow1 * (fTemp82 - fVec1[1])));
			fRec52[0] = std::fmod(fSlow34 + fRec52[1], fSlow10);
			float fTemp83 = std::fmod(fRec52[0] + fTemp18, fSlow10);
			int iTemp84 = int(fSlow13 * fTemp83);
			iRec53[0] = iRec53[1] * (1 - iTemp84) + iTemp17 * iTemp84;
			float fTemp85 = std::fmod(fRec52[0] + fTemp22, fSlow10);
			int iTemp86 = int(fSlow13 * fTemp85);
			iRec54[0] = iRec54[1] * (1 - iTemp86) + iTemp21 * iTemp86;
			float fTemp87 = std::fmod(fRec52[0] + fTemp26, fSlow10);
			int iTemp88 = int(fSlow13 * fTemp87);
			iRec55[0] = iRec55[1] * (1 - iTemp88) + iTemp25 * iTemp88;
			float fTemp89 = std::fmod(fRec52[0] + fTemp30, fSlow10);
			int iTemp90 = int(fSlow13 * fTemp89);
			iRec56[0] = iRec56[1] * (1 - iTemp90) + iTemp29 * iTemp90;
			float fTemp91 = std::fmod(fRec52[0] + fTemp34, fSlow10);
			int iTemp92 = int(fSlow13 * fTemp91);
			iRec57[0] = iRec57[1] * (1 - iTemp92) + iTemp33 * iTemp92;
			float fTemp93 = std::fmod(fRec52[0] + fTemp38, fSlow10);
			int iTemp94 = int(fSlow13 * fTemp93);
			iRec58[0] = iRec58[1] * (1 - iTemp94) + iTemp37 * iTemp94;
			float fTemp95 = std::fmod(fRec52[0] + fTemp42, fSlow10);
			int iTemp96 = int(fSlow13 * fTemp95);
			iRec59[0] = iRec59[1] * (1 - iTemp96) + iTemp41 * iTemp96;
			float fTemp97 = std::fmod(fRec52[0] + fTemp46, fSlow10);
			int iTemp98 = int(fSlow13 * fTemp97);
			iRec60[0] = iRec60[1] * (1 - iTemp98) + iTemp45 * iTemp98;
			float fTemp99 = std::fmod(fRec52[0] + fTemp50, fSlow10);
			int iTemp100 = int(fSlow13 * fTemp99);
			iRec61[0] = iRec61[1] * (1 - iTemp100) + iTemp49 * iTemp100;
			float fTemp101 = std::fmod(fRec52[0] + fTemp54, fSlow10);
			int iTemp102 = int(fSlow13 * fTemp101);
			iRec62[0] = iRec62[1] * (1 - iTemp102) + iTemp53 * iTemp102;
			float fTemp103 = std::fmod(fRec52[0] + fTemp58, fSlow10);
			int iTemp104 = int(fSlow13 * fTemp103);
			iRec63[0] = iRec63[1] * (1 - iTemp104) + iTemp57 * iTemp104;
			float fTemp105 = std::fmod(fRec52[0] + fTemp62, fSlow10);
			int iTemp106 = int(fSlow13 * fTemp105);
			iRec64[0] = iRec64[1] * (1 - iTemp106) + iTemp61 * iTemp106;
			float fTemp107 = std::fmod(fRec52[0] + fTemp66, fSlow10);
			int iTemp108 = int(fSlow13 * fTemp107);
			iRec65[0] = iRec65[1] * (1 - iTemp108) + iTemp65 * iTemp108;
			float fTemp109 = std::fmod(fRec52[0] + fTemp70, fSlow10);
			int iTemp110 = int(fSlow13 * fTemp109);
			iRec66[0] = iRec66[1] * (1 - iTemp110) + iTemp69 * iTemp110;
			float fTemp111 = std::fmod(fRec52[0] + fTemp74, fSlow10);
			int iTemp112 = int(fSlow13 * fTemp111);
			iRec67[0] = iRec67[1] * (1 - iTemp112) + iTemp73 * iTemp112;
			float fTemp113 = std::fmod(fRec52[0] + fTemp78, fSlow10);
			int iTemp114 = int(fSlow13 * fTemp113);
			iRec68[0] = iRec68[1] * (1 - iTemp114) + iTemp77 * iTemp114;
			float fTemp115 = fSlow6 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp83 + float(iRec53[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp83) + fSlow15 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp85 + float(iRec54[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp85) + fSlow16 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp87 + float(iRec55[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp87) + fSlow17 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp89 + float(iRec56[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp89) + fSlow18 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp91 + float(iRec57[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp91) + fSlow19 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp93 + float(iRec58[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp93) + fSlow20 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp95 + float(iRec59[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp95) + fSlow21 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp97 + float(iRec60[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp97) + fSlow22 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp99 + float(iRec61[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp99) + fSlow23 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp101 + float(iRec62[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp101) + fSlow24 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp103 + float(iRec63[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp103) + fSlow25 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp105 + float(iRec64[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp105) + fSlow26 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp107 + float(iRec65[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp107) + fSlow27 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp109 + float(iRec66[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp109) + fSlow28 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp111 + float(iRec67[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp111) + fSlow29 * ftbl0[std::max<int>(0, std::min<int>(int(std::fmod(float(int(fTemp113 + float(iRec68[0]))), fSlow8)), 132299))] * std::sin(fSlow14 * fTemp113);
			float fTemp116 = fSlow33 * fTemp115;
			fVec2[0] = fTemp116;
			fRec51[0] = fSlow32 * (fSlow31 * (fTemp116 - fVec2[1]) - fSlow35 * fRec51[1]);
			output0[i0] = FAUSTFLOAT(fSlow0 * fRec0[0] + fSlow30 * fRec51[0]);
			float fTemp117 = fSlow37 * fTemp81;
			fVec3[0] = fTemp117;
			fRec69[0] = -(fSlow2 * (fSlow3 * fRec69[1] - fSlow1 * (fTemp117 - fVec3[1])));
			float fTemp118 = fSlow39 * fTemp115;
			fVec4[0] = fTemp118;
			fRec70[0] = -(fSlow32 * (fSlow35 * fRec70[1] - fSlow31 * (fTemp118 - fVec4[1])));
			output1[i0] = FAUSTFLOAT(fSlow36 * fRec69[0] + fSlow38 * fRec70[0]);
			iVec0[1] = iVec0[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			iRec4[1] = iRec4[0];
			iRec3[1] = iRec3[0];
			iRec20[1] = iRec20[0];
			iRec21[1] = iRec21[0];
			iRec22[1] = iRec22[0];
			iRec23[1] = iRec23[0];
			iRec24[1] = iRec24[0];
			iRec25[1] = iRec25[0];
			iRec26[1] = iRec26[0];
			iRec27[1] = iRec27[0];
			iRec28[1] = iRec28[0];
			iRec29[1] = iRec29[0];
			iRec30[1] = iRec30[0];
			iRec31[1] = iRec31[0];
			iRec32[1] = iRec32[0];
			iRec33[1] = iRec33[0];
			iRec34[1] = iRec34[0];
			iRec35[1] = iRec35[0];
			iRec36[1] = iRec36[0];
			iRec37[1] = iRec37[0];
			iRec38[1] = iRec38[0];
			iRec39[1] = iRec39[0];
			iRec40[1] = iRec40[0];
			iRec41[1] = iRec41[0];
			iRec42[1] = iRec42[0];
			iRec43[1] = iRec43[0];
			iRec44[1] = iRec44[0];
			iRec45[1] = iRec45[0];
			iRec46[1] = iRec46[0];
			iRec47[1] = iRec47[0];
			iRec48[1] = iRec48[0];
			iRec49[1] = iRec49[0];
			iRec50[1] = iRec50[0];
			fVec1[1] = fVec1[0];
			fRec0[1] = fRec0[0];
			fRec52[1] = fRec52[0];
			iRec53[1] = iRec53[0];
			iRec54[1] = iRec54[0];
			iRec55[1] = iRec55[0];
			iRec56[1] = iRec56[0];
			iRec57[1] = iRec57[0];
			iRec58[1] = iRec58[0];
			iRec59[1] = iRec59[0];
			iRec60[1] = iRec60[0];
			iRec61[1] = iRec61[0];
			iRec62[1] = iRec62[0];
			iRec63[1] = iRec63[0];
			iRec64[1] = iRec64[0];
			iRec65[1] = iRec65[0];
			iRec66[1] = iRec66[0];
			iRec67[1] = iRec67[0];
			iRec68[1] = iRec68[0];
			fVec2[1] = fVec2[0];
			fRec51[1] = fRec51[0];
			fVec3[1] = fVec3[0];
			fRec69[1] = fRec69[0];
			fVec4[1] = fVec4[0];
			fRec70[1] = fRec70[0];
		}
	}

};
#endif

class faust_fx_granulator_tilde : public FaustExternal<fx_granulator> {
public:
    faust_fx_granulator_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(fx_granulator))
    {
    }
};

#endif
