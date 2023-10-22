/* ------------------------------------------------------------
name: "fx.freeverb2"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn fx_freeverb2 -scn fx_freeverb2_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_freeverb2_H__
#define  __fx_freeverb2_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_freeverb2_dsp.h ********************************
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

class FAUST_API fx_freeverb2_dsp {

    public:

        fx_freeverb2_dsp() {}
        virtual ~fx_freeverb2_dsp() {}

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
        virtual fx_freeverb2_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_freeverb2_dsp {

    protected:

        fx_freeverb2_dsp* fDSP;

    public:

        decorator_dsp(fx_freeverb2_dsp* fx_freeverb2_dsp = nullptr):fDSP(fx_freeverb2_dsp) {}
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
    
        virtual fx_freeverb2_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_freeverb2_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_freeverb2_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_freeverb2 : public fx_freeverb2_dsp {
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
#define FAUSTCLASS fx_freeverb2
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


class fx_freeverb2 : public fx_freeverb2_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	float fRec0[2];
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec1[2];
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider1;
	float fConst3;
	float fRec11[2];
	FAUSTFLOAT fHslider2;
	float fRec13[2];
	FAUSTFLOAT fHslider3;
	float fRec12[2];
	FAUSTFLOAT fHslider4;
	float fRec16[2];
	FAUSTFLOAT fHslider5;
	float fRec15[2];
	float fRec14[2];
	int IOTA0;
	float fVec0[8192];
	int iConst4;
	float fRec10[2];
	float fRec18[2];
	float fVec1[8192];
	int iConst5;
	float fRec17[2];
	float fRec20[2];
	float fVec2[8192];
	int iConst6;
	float fRec19[2];
	float fRec22[2];
	float fVec3[8192];
	int iConst7;
	float fRec21[2];
	float fRec24[2];
	float fVec4[8192];
	int iConst8;
	float fRec23[2];
	float fRec26[2];
	float fVec5[8192];
	int iConst9;
	float fRec25[2];
	float fRec28[2];
	float fVec6[8192];
	int iConst10;
	float fRec27[2];
	float fRec30[2];
	float fVec7[8192];
	int iConst11;
	float fRec29[2];
	float fVec8[2048];
	int iConst12;
	int iConst13;
	float fRec8[2];
	float fVec9[2048];
	int iConst14;
	int iConst15;
	float fRec6[2];
	float fVec10[2048];
	int iConst16;
	int iConst17;
	float fRec4[2];
	float fVec11[1024];
	int iConst18;
	int iConst19;
	float fRec2[2];
	float fRec40[2];
	float fVec12[8192];
	float fConst20;
	FAUSTFLOAT fHslider6;
	float fRec39[2];
	float fRec42[2];
	float fVec13[8192];
	float fRec41[2];
	float fRec44[2];
	float fVec14[8192];
	float fRec43[2];
	float fRec46[2];
	float fVec15[8192];
	float fRec45[2];
	float fRec48[2];
	float fVec16[8192];
	float fRec47[2];
	float fRec50[2];
	float fVec17[8192];
	float fRec49[2];
	float fRec52[2];
	float fVec18[8192];
	float fRec51[2];
	float fRec54[2];
	float fVec19[8192];
	float fRec53[2];
	float fVec20[2048];
	float fRec37[2];
	float fVec21[2048];
	float fRec35[2];
	float fVec22[2048];
	float fRec33[2];
	float fVec23[2048];
	float fRec31[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.4");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn fx_freeverb2 -scn fx_freeverb2_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_freeverb2.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "fx.freeverb2");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("reverbs.lib/mono_freeverb:author", "Romain Michon");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/stereo_freeverb:author", "Romain Michon");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
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
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / fConst0;
		iConst4 = int(0.036666665f * fConst0);
		iConst5 = int(0.035306122f * fConst0);
		iConst6 = int(0.033809524f * fConst0);
		iConst7 = int(0.0322449f * fConst0);
		iConst8 = int(0.025306122f * fConst0);
		iConst9 = int(0.030748298f * fConst0);
		iConst10 = int(0.026938776f * fConst0);
		iConst11 = int(0.028956916f * fConst0);
		iConst12 = int(0.0126077095f * fConst0);
		iConst13 = std::min<int>(1024, std::max<int>(0, iConst12 + -1));
		iConst14 = int(0.01f * fConst0);
		iConst15 = std::min<int>(1024, std::max<int>(0, iConst14 + -1));
		iConst16 = int(0.0077324263f * fConst0);
		iConst17 = std::min<int>(1024, std::max<int>(0, iConst16 + -1));
		iConst18 = int(0.0051020407f * fConst0);
		iConst19 = std::min<int>(1024, std::max<int>(0, iConst18 + -1));
		fConst20 = 0.0010430838f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.33f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(1e+02f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.999f);
		fHslider4 = FAUSTFLOAT(0.5f);
		fHslider5 = FAUSTFLOAT(0.0001f);
		fHslider6 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec11[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec13[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec12[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec16[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec15[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec14[l7] = 0.0f;
		}
		IOTA0 = 0;
		for (int l8 = 0; l8 < 8192; l8 = l8 + 1) {
			fVec0[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec10[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec18[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 8192; l11 = l11 + 1) {
			fVec1[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec17[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec20[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 8192; l14 = l14 + 1) {
			fVec2[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec19[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec22[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 8192; l17 = l17 + 1) {
			fVec3[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec21[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec24[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 8192; l20 = l20 + 1) {
			fVec4[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec23[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec26[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 8192; l23 = l23 + 1) {
			fVec5[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec25[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec28[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 8192; l26 = l26 + 1) {
			fVec6[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec27[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec30[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 8192; l29 = l29 + 1) {
			fVec7[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec29[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2048; l31 = l31 + 1) {
			fVec8[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec8[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2048; l33 = l33 + 1) {
			fVec9[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec6[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2048; l35 = l35 + 1) {
			fVec10[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec4[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 1024; l37 = l37 + 1) {
			fVec11[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec2[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec40[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 8192; l40 = l40 + 1) {
			fVec12[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec39[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec42[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 8192; l43 = l43 + 1) {
			fVec13[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec41[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec44[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 8192; l46 = l46 + 1) {
			fVec14[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec43[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec46[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 8192; l49 = l49 + 1) {
			fVec15[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec45[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec48[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 8192; l52 = l52 + 1) {
			fVec16[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec47[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec50[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 8192; l55 = l55 + 1) {
			fVec17[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec49[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec52[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 8192; l58 = l58 + 1) {
			fVec18[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec51[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec54[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 8192; l61 = l61 + 1) {
			fVec19[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec53[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2048; l63 = l63 + 1) {
			fVec20[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec37[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2048; l65 = l65 + 1) {
			fVec21[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec35[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2048; l67 = l67 + 1) {
			fVec22[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec33[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2048; l69 = l69 + 1) {
			fVec23[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec31[l70] = 0.0f;
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
	
	virtual fx_freeverb2* clone() {
		return new fx_freeverb2();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.freeverb2");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addHorizontalSlider("damp", &fHslider4, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, FAUSTFLOAT(0.33f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->addHorizontalSlider("fr_damp", &fHslider5, FAUSTFLOAT(0.0001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("fr_room", &fHslider3, FAUSTFLOAT(0.999f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "ms");
		ui_interface->addHorizontalSlider("fr_time", &fHslider1, FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fCheckbox1, "type", "bool");
		ui_interface->addCheckButton("freeze", &fCheckbox1);
		ui_interface->addHorizontalSlider("room", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("spread", &fHslider6, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(float(fCheckbox0) >= 1.0f);
		float fSlow1 = fConst1 * float(fHslider0);
		int iSlow2 = int(float(fCheckbox1));
		float fSlow3 = 0.001f * float(fHslider1);
		int iSlow4 = std::fabs(fSlow3) < 1.1920929e-07f;
		float fSlow5 = ((iSlow4) ? 0.0f : std::exp(0.0f - fConst3 / ((iSlow4) ? 1.0f : fSlow3)));
		float fSlow6 = 1.0f - fSlow5;
		float fSlow7 = float(1 - iSlow2) * fSlow6;
		float fSlow8 = fConst1 * float(fHslider2);
		float fSlow9 = float(fHslider3);
		float fSlow10 = fConst1 * float(fHslider4);
		float fSlow11 = float(fHslider5);
		int iSlow12 = int(fConst20 * float(fHslider6));
		int iSlow13 = iConst11 + iSlow12;
		int iSlow14 = iConst10 + iSlow12;
		int iSlow15 = iConst8 + iSlow12;
		int iSlow16 = iConst6 + iSlow12;
		int iSlow17 = iConst7 + iSlow12;
		int iSlow18 = iConst9 + iSlow12;
		int iSlow19 = iConst4 + iSlow12;
		int iSlow20 = iConst5 + iSlow12;
		int iSlow21 = iSlow12 + -1;
		int iSlow22 = std::min<int>(1024, std::max<int>(0, iConst12 + iSlow21));
		int iSlow23 = std::min<int>(1024, std::max<int>(0, iConst14 + iSlow21));
		int iSlow24 = std::min<int>(1024, std::max<int>(0, iConst16 + iSlow21));
		int iSlow25 = std::min<int>(1024, std::max<int>(0, iConst18 + iSlow21));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = fRec0[1] + 0.0078125f;
			float fTemp1 = fRec0[1] + -0.0078125f;
			fRec0[0] = ((fTemp0 < fSlow0) ? fTemp0 : ((fTemp1 > fSlow0) ? fTemp1 : fSlow0));
			float fTemp2 = 1.0f - fRec0[0];
			float fTemp3 = float(input0[i0]);
			fRec1[0] = fSlow1 + fConst2 * fRec1[1];
			float fTemp4 = 1.0f - fRec1[0];
			fRec11[0] = fSlow7 + fSlow5 * fRec11[1];
			float fTemp5 = float(input1[i0]);
			float fTemp6 = fRec11[0] * (fTemp3 + fTemp5);
			fRec13[0] = fSlow8 + fConst2 * fRec13[1];
			fRec12[0] = ((iSlow2) ? fSlow9 : fRec13[0]) * fSlow6 + fSlow5 * fRec12[1];
			fRec16[0] = fSlow10 + fConst2 * fRec16[1];
			fRec15[0] = ((iSlow2) ? fSlow11 : fRec16[0]) * fSlow6 + fSlow5 * fRec15[1];
			float fTemp7 = 1.0f - fRec15[0];
			fRec14[0] = fRec15[0] * fRec14[1] + fTemp7 * fRec10[1];
			fVec0[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec14[0];
			fRec10[0] = fVec0[(IOTA0 - iConst4) & 8191];
			fRec18[0] = fRec15[0] * fRec18[1] + fTemp7 * fRec17[1];
			fVec1[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec18[0];
			fRec17[0] = fVec1[(IOTA0 - iConst5) & 8191];
			fRec20[0] = fRec15[0] * fRec20[1] + fTemp7 * fRec19[1];
			fVec2[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec20[0];
			fRec19[0] = fVec2[(IOTA0 - iConst6) & 8191];
			fRec22[0] = fRec15[0] * fRec22[1] + fTemp7 * fRec21[1];
			fVec3[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec22[0];
			fRec21[0] = fVec3[(IOTA0 - iConst7) & 8191];
			fRec24[0] = fRec15[0] * fRec24[1] + fTemp7 * fRec23[1];
			fVec4[IOTA0 & 8191] = fRec24[0] * fRec12[0] + fTemp6;
			fRec23[0] = fVec4[(IOTA0 - iConst8) & 8191];
			fRec26[0] = fRec15[0] * fRec26[1] + fTemp7 * fRec25[1];
			fVec5[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec26[0];
			fRec25[0] = fVec5[(IOTA0 - iConst9) & 8191];
			fRec28[0] = fRec15[0] * fRec28[1] + fTemp7 * fRec27[1];
			fVec6[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec28[0];
			fRec27[0] = fVec6[(IOTA0 - iConst10) & 8191];
			fRec30[0] = fRec15[0] * fRec30[1] + fTemp7 * fRec29[1];
			fVec7[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec30[0];
			fRec29[0] = fVec7[(IOTA0 - iConst11) & 8191];
			float fTemp8 = fRec10[0] + fRec17[0] + fRec19[0] + fRec21[0] + 0.5f * fRec8[1] + fRec23[0] + fRec25[0] + fRec27[0] + fRec29[0];
			fVec8[IOTA0 & 2047] = fTemp8;
			fRec8[0] = fVec8[(IOTA0 - iConst13) & 2047];
			float fRec9 = 0.0f - 0.5f * fTemp8;
			float fTemp9 = fRec9 + 0.5f * fRec6[1] + fRec8[1];
			fVec9[IOTA0 & 2047] = fTemp9;
			fRec6[0] = fVec9[(IOTA0 - iConst15) & 2047];
			float fRec7 = 0.0f - 0.5f * fTemp9;
			float fTemp10 = fRec7 + 0.5f * fRec4[1] + fRec6[1];
			fVec10[IOTA0 & 2047] = fTemp10;
			fRec4[0] = fVec10[(IOTA0 - iConst17) & 2047];
			float fRec5 = 0.0f - 0.5f * fTemp10;
			float fTemp11 = fRec5 + 0.5f * fRec2[1] + fRec4[1];
			fVec11[IOTA0 & 1023] = fTemp11;
			fRec2[0] = fVec11[(IOTA0 - iConst19) & 1023];
			float fRec3 = 0.0f - 0.5f * fTemp11;
			output0[i0] = FAUSTFLOAT(fTemp2 * (fTemp3 * fTemp4 + fRec1[0] * (fRec3 + fRec2[1])) + fTemp3 * fRec0[0]);
			fRec40[0] = fRec15[0] * fRec40[1] + fTemp7 * fRec39[1];
			fVec12[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec40[0];
			fRec39[0] = fVec12[(IOTA0 - iSlow13) & 8191];
			fRec42[0] = fRec15[0] * fRec42[1] + fTemp7 * fRec41[1];
			fVec13[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec42[0];
			fRec41[0] = fVec13[(IOTA0 - iSlow14) & 8191];
			fRec44[0] = fRec15[0] * fRec44[1] + fTemp7 * fRec43[1];
			fVec14[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec44[0];
			fRec43[0] = fVec14[(IOTA0 - iSlow15) & 8191];
			fRec46[0] = fRec15[0] * fRec46[1] + fTemp7 * fRec45[1];
			fVec15[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec46[0];
			fRec45[0] = fVec15[(IOTA0 - iSlow16) & 8191];
			fRec48[0] = fRec15[0] * fRec48[1] + fTemp7 * fRec47[1];
			fVec16[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec48[0];
			fRec47[0] = fVec16[(IOTA0 - iSlow17) & 8191];
			fRec50[0] = fRec15[0] * fRec50[1] + fTemp7 * fRec49[1];
			fVec17[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec50[0];
			fRec49[0] = fVec17[(IOTA0 - iSlow18) & 8191];
			fRec52[0] = fRec15[0] * fRec52[1] + fTemp7 * fRec51[1];
			fVec18[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec52[0];
			fRec51[0] = fVec18[(IOTA0 - iSlow19) & 8191];
			fRec54[0] = fRec15[0] * fRec54[1] + fTemp7 * fRec53[1];
			fVec19[IOTA0 & 8191] = fTemp6 + fRec12[0] * fRec54[0];
			fRec53[0] = fVec19[(IOTA0 - iSlow20) & 8191];
			float fTemp12 = fRec39[0] + fRec41[0] + fRec43[0] + fRec45[0] + fRec47[0] + fRec49[0] + fRec51[0] + fRec53[0] + 0.5f * fRec37[1];
			fVec20[IOTA0 & 2047] = fTemp12;
			fRec37[0] = fVec20[(IOTA0 - iSlow22) & 2047];
			float fRec38 = 0.0f - 0.5f * fTemp12;
			float fTemp13 = fRec38 + 0.5f * fRec35[1] + fRec37[1];
			fVec21[IOTA0 & 2047] = fTemp13;
			fRec35[0] = fVec21[(IOTA0 - iSlow23) & 2047];
			float fRec36 = 0.0f - 0.5f * fTemp13;
			float fTemp14 = fRec36 + 0.5f * fRec33[1] + fRec35[1];
			fVec22[IOTA0 & 2047] = fTemp14;
			fRec33[0] = fVec22[(IOTA0 - iSlow24) & 2047];
			float fRec34 = 0.0f - 0.5f * fTemp14;
			float fTemp15 = fRec34 + 0.5f * fRec31[1] + fRec33[1];
			fVec23[IOTA0 & 2047] = fTemp15;
			fRec31[0] = fVec23[(IOTA0 - iSlow25) & 2047];
			float fRec32 = 0.0f - 0.5f * fTemp15;
			output1[i0] = FAUSTFLOAT(fTemp2 * (fTemp5 * fTemp4 + fRec1[0] * (fRec32 + fRec31[1])) + fTemp5 * fRec0[0]);
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec11[1] = fRec11[0];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fRec16[1] = fRec16[0];
			fRec15[1] = fRec15[0];
			fRec14[1] = fRec14[0];
			IOTA0 = IOTA0 + 1;
			fRec10[1] = fRec10[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec30[1] = fRec30[0];
			fRec29[1] = fRec29[0];
			fRec8[1] = fRec8[0];
			fRec6[1] = fRec6[0];
			fRec4[1] = fRec4[0];
			fRec2[1] = fRec2[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec48[1] = fRec48[0];
			fRec47[1] = fRec47[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec37[1] = fRec37[0];
			fRec35[1] = fRec35[0];
			fRec33[1] = fRec33[0];
			fRec31[1] = fRec31[0];
		}
	}

};
#endif

class faust_fx_freeverb2_tilde : public FaustExternal<fx_freeverb2> {
public:
    faust_fx_freeverb2_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(fx_freeverb2))
    {
    }
};

#endif
