/* ------------------------------------------------------------
name: "synth.eguitar"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_eguitar -scn synth_eguitar_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_eguitar_H__
#define  __synth_eguitar_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_eguitar_dsp.h ********************************
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

class FAUST_API synth_eguitar_dsp {

    public:

        synth_eguitar_dsp() {}
        virtual ~synth_eguitar_dsp() {}

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
        virtual synth_eguitar_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_eguitar_dsp {

    protected:

        synth_eguitar_dsp* fDSP;

    public:

        decorator_dsp(synth_eguitar_dsp* synth_eguitar_dsp = nullptr):fDSP(synth_eguitar_dsp) {}
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
    
        virtual synth_eguitar_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_eguitar_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_eguitar_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_eguitar : public synth_eguitar_dsp {
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
#define FAUSTCLASS synth_eguitar
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

static float synth_eguitar_faustpower2_f(float value) {
	return value * value;
}

class synth_eguitar : public synth_eguitar_dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	int IOTA0;
	float fConst2;
	float fRec25[2];
	float fRec29[2];
	float fRec31[4];
	float fRec32[2048];
	float fVec0[2];
	FAUSTFLOAT fHslider3;
	float fConst3;
	int iRec34[2];
	float fRec33[3];
	float fConst4;
	FAUSTFLOAT fButton0;
	float fVec1[2];
	int iRec35[2];
	float fVec2[3];
	float fRec30[2048];
	float fRec21[2];
	float fRec17[2];
	float fRec13[2048];
	float fRec15[2];
	float fRec11[4];
	float fRec6[2];
	float fRec2[2048];
	float fRec0[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_eguitar -scn synth_eguitar_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.2");
		m->declare("filename", "synth_eguitar.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "synth.eguitar");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.4");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/version", "0.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
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
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 0.0014705883f * fConst0;
		fConst2 = 0.00882353f * fConst0;
		fConst3 = 6911.504f / fConst0;
		fConst4 = 0.002f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(48.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(1.0f);
		fButton0 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec25[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec29[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec31[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2048; l3 = l3 + 1) {
			fRec32[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec0[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec34[l5] = 0;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec33[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec1[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			iRec35[l8] = 0;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fVec2[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2048; l10 = l10 + 1) {
			fRec30[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec21[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec17[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2048; l13 = l13 + 1) {
			fRec13[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec15[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 4; l15 = l15 + 1) {
			fRec11[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec6[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2048; l17 = l17 + 1) {
			fRec2[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec0[l18] = 0.0f;
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
	
	virtual synth_eguitar* clone() {
		return new synth_eguitar();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.eguitar");
		ui_interface->addHorizontalSlider("gain", &fHslider3, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("mute", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("pitch", &fHslider1, FAUSTFLOAT(48.0f), FAUSTFLOAT(36.0f), FAUSTFLOAT(84.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("pos", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = 1.0f - float(fHslider0);
		float fSlow1 = std::pow(2.0f, 0.083333336f * (float(fHslider1) + -69.0f));
		float fSlow2 = 0.77272725f / fSlow1 + -0.11f;
		float fSlow3 = float(fHslider2);
		float fSlow4 = fConst1 * fSlow2 * (1.0f - fSlow3);
		float fSlow5 = fSlow4 + -1.499995f;
		float fSlow6 = std::floor(fSlow5);
		float fSlow7 = fSlow4 + (-1.0f - fSlow6);
		float fSlow8 = fSlow4 + (-2.0f - fSlow6);
		float fSlow9 = fSlow4 + (-3.0f - fSlow6);
		float fSlow10 = fSlow4 + (-4.0f - fSlow6);
		float fSlow11 = (0.0f - fSlow7) * (0.0f - 0.5f * fSlow8) * (0.0f - 0.33333334f * fSlow9) * (0.0f - 0.25f * fSlow10);
		int iSlow12 = int(fSlow5);
		int iSlow13 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow12))));
		int iSlow14 = iSlow13 + 1;
		float fSlow15 = fSlow4 - fSlow6;
		float fSlow16 = (0.0f - fSlow8) * (0.0f - 0.5f * fSlow9) * (0.0f - 0.33333334f * fSlow10);
		int iSlow17 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow12 + 1))));
		int iSlow18 = iSlow17 + 1;
		float fSlow19 = 0.5f * fSlow7 * (0.0f - fSlow9) * (0.0f - 0.5f * fSlow10);
		int iSlow20 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow12 + 2))));
		int iSlow21 = iSlow20 + 1;
		float fSlow22 = fSlow7 * fSlow8;
		float fSlow23 = 0.16666667f * fSlow22 * (0.0f - fSlow10);
		int iSlow24 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow12 + 3))));
		int iSlow25 = iSlow24 + 1;
		float fSlow26 = 0.041666668f * fSlow22 * fSlow9;
		int iSlow27 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow12 + 4))));
		int iSlow28 = iSlow27 + 1;
		float fSlow29 = fConst1 * fSlow3 * fSlow2;
		float fSlow30 = fSlow29 + -1.499995f;
		float fSlow31 = std::floor(fSlow30);
		float fSlow32 = fSlow29 + (-1.0f - fSlow31);
		float fSlow33 = fSlow29 + (-2.0f - fSlow31);
		float fSlow34 = fSlow29 + (-3.0f - fSlow31);
		float fSlow35 = fSlow29 + (-4.0f - fSlow31);
		float fSlow36 = (0.0f - fSlow32) * (0.0f - 0.5f * fSlow33) * (0.0f - 0.33333334f * fSlow34) * (0.0f - 0.25f * fSlow35);
		int iSlow37 = int(fSlow30);
		int iSlow38 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow37))));
		int iSlow39 = iSlow38 + 2;
		float fSlow40 = fSlow29 - fSlow31;
		float fSlow41 = (0.0f - fSlow33) * (0.0f - 0.5f * fSlow34) * (0.0f - 0.33333334f * fSlow35);
		int iSlow42 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow37 + 1))));
		int iSlow43 = iSlow42 + 2;
		float fSlow44 = 0.5f * fSlow32 * (0.0f - fSlow34) * (0.0f - 0.5f * fSlow35);
		int iSlow45 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow37 + 2))));
		int iSlow46 = iSlow45 + 2;
		float fSlow47 = fSlow32 * fSlow33;
		float fSlow48 = 0.16666667f * fSlow47 * (0.0f - fSlow35);
		int iSlow49 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow37 + 3))));
		int iSlow50 = iSlow49 + 2;
		float fSlow51 = 0.041666668f * fSlow47 * fSlow34;
		int iSlow52 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow37 + 4))));
		int iSlow53 = iSlow52 + 2;
		float fSlow54 = std::tan(fConst3 * fSlow1);
		float fSlow55 = 1.0f / fSlow54;
		float fSlow56 = (fSlow55 + 1.4142135f) / fSlow54 + 1.0f;
		float fSlow57 = float(fHslider3) / fSlow56;
		float fSlow58 = 1.0f / fSlow56;
		float fSlow59 = (fSlow55 + -1.4142135f) / fSlow54 + 1.0f;
		float fSlow60 = 2.0f * (1.0f - 1.0f / synth_eguitar_faustpower2_f(fSlow54));
		float fSlow61 = 1.0f / std::max<float>(1.0f, fConst4 * synth_eguitar_faustpower2_f(1.0f - 0.22f * fSlow1));
		float fSlow62 = float(fButton0);
		int iSlow63 = iSlow38 + 1;
		int iSlow64 = iSlow42 + 1;
		int iSlow65 = iSlow45 + 1;
		int iSlow66 = iSlow49 + 1;
		int iSlow67 = iSlow52 + 1;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fRec10 = -1.0f * 0.9973053f * (0.9f * fRec11[2] + 0.05f * (fRec11[1] + fRec11[3]));
			fRec25[0] = fSlow11 * fRec2[(IOTA0 - iSlow14) & 2047] + fSlow15 * (fSlow16 * fRec2[(IOTA0 - iSlow18) & 2047] + fSlow19 * fRec2[(IOTA0 - iSlow21) & 2047] + fSlow23 * fRec2[(IOTA0 - iSlow25) & 2047] + fSlow26 * fRec2[(IOTA0 - iSlow28) & 2047]);
			fRec29[0] = 0.95f * fRec25[1] + 0.05f * fRec29[1];
			float fRec26 = fRec29[0];
			fRec31[0] = fRec0[1];
			fRec32[IOTA0 & 2047] = -1.0f * 0.9973053f * (0.9f * fRec31[2] + 0.05f * (fRec31[1] + fRec31[3]));
			fVec0[0] = fSlow36 * fRec32[(IOTA0 - iSlow39) & 2047] + fSlow40 * (fSlow41 * fRec32[(IOTA0 - iSlow43) & 2047] + fSlow44 * fRec32[(IOTA0 - iSlow46) & 2047] + fSlow48 * fRec32[(IOTA0 - iSlow50) & 2047] + fSlow51 * fRec32[(IOTA0 - iSlow53) & 2047]);
			iRec34[0] = 1103515245 * iRec34[1] + 12345;
			fRec33[0] = 4.656613e-10f * float(iRec34[0]) - fSlow58 * (fSlow59 * fRec33[2] + fSlow60 * fRec33[1]);
			fVec1[0] = fSlow62;
			iRec35[0] = (iRec35[1] + (iRec35[1] > 0)) * (fSlow62 <= fVec1[1]) + (fSlow62 > fVec1[1]);
			float fTemp0 = fSlow61 * float(iRec35[0]);
			float fTemp1 = fSlow57 * (fRec33[2] + fRec33[0] + 2.0f * fRec33[1]) * std::max<float>(0.0f, std::min<float>(fTemp0, 2.0f - fTemp0));
			float fTemp2 = fVec0[1] + fTemp1;
			fVec2[0] = fTemp2;
			fRec30[IOTA0 & 2047] = 0.95f * fVec2[2] + 0.05f * fRec30[(IOTA0 - 1) & 2047];
			float fRec27 = fSlow11 * fRec30[(IOTA0 - iSlow13) & 2047] + fSlow15 * (fSlow16 * fRec30[(IOTA0 - iSlow17) & 2047] + fSlow19 * fRec30[(IOTA0 - iSlow20) & 2047] + fSlow23 * fRec30[(IOTA0 - iSlow24) & 2047] + fSlow26 * fRec30[(IOTA0 - iSlow27) & 2047]);
			float fRec28 = fVec2[1] + fRec21[1];
			fRec21[0] = fRec26;
			float fRec22 = fRec21[1];
			float fRec23 = fRec27;
			float fRec24 = fRec28;
			fRec17[0] = fRec22;
			float fRec18 = fTemp1 + fRec17[1];
			float fRec19 = fRec23;
			float fRec20 = fRec24;
			fRec13[IOTA0 & 2047] = fRec18;
			float fRec14 = fSlow36 * fRec13[(IOTA0 - iSlow63) & 2047] + fSlow40 * (fSlow41 * fRec13[(IOTA0 - iSlow64) & 2047] + fSlow44 * fRec13[(IOTA0 - iSlow65) & 2047] + fSlow48 * fRec13[(IOTA0 - iSlow66) & 2047] + fSlow51 * fRec13[(IOTA0 - iSlow67) & 2047]);
			fRec15[0] = fRec19;
			float fRec16 = fRec20;
			fRec11[0] = fSlow0 * fRec15[1];
			float fRec12 = fRec16;
			fRec6[0] = fRec10;
			float fRec7 = fSlow0 * fRec6[1];
			float fRec8 = fRec11[0];
			float fRec9 = fRec12;
			fRec2[IOTA0 & 2047] = fRec7;
			float fRec3 = fRec14;
			float fRec4 = fRec8;
			float fRec5 = fRec9;
			fRec0[0] = fRec3;
			float fRec1 = fRec5;
			output0[i0] = FAUSTFLOAT(fRec1);
			IOTA0 = IOTA0 + 1;
			fRec25[1] = fRec25[0];
			fRec29[1] = fRec29[0];
			for (int j0 = 3; j0 > 0; j0 = j0 - 1) {
				fRec31[j0] = fRec31[j0 - 1];
			}
			fVec0[1] = fVec0[0];
			iRec34[1] = iRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fVec1[1] = fVec1[0];
			iRec35[1] = iRec35[0];
			fVec2[2] = fVec2[1];
			fVec2[1] = fVec2[0];
			fRec21[1] = fRec21[0];
			fRec17[1] = fRec17[0];
			fRec15[1] = fRec15[0];
			for (int j1 = 3; j1 > 0; j1 = j1 - 1) {
				fRec11[j1] = fRec11[j1 - 1];
			}
			fRec6[1] = fRec6[0];
			fRec0[1] = fRec0[0];
		}
	}

};
#endif

class faust_synth_eguitar_tilde : public FaustExternal<synth_eguitar> {
public:
    faust_synth_eguitar_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_eguitar))
    {
    }
};

#endif
