/* ------------------------------------------------------------
author: "brummer"
copyright: "(c)brummer 2008"
license: "BSD"
name: "fx.distortion1"
version: "0.01"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn fx_distortion1 -scn fx_distortion1_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_distortion1_H__
#define  __fx_distortion1_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_distortion1_dsp.h ********************************
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

class FAUST_API fx_distortion1_dsp {

    public:

        fx_distortion1_dsp() {}
        virtual ~fx_distortion1_dsp() {}

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
        virtual fx_distortion1_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_distortion1_dsp {

    protected:

        fx_distortion1_dsp* fDSP;

    public:

        decorator_dsp(fx_distortion1_dsp* fx_distortion1_dsp = nullptr):fDSP(fx_distortion1_dsp) {}
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
    
        virtual fx_distortion1_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_distortion1_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_distortion1_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_distortion1 : public fx_distortion1_dsp {
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
#define FAUSTCLASS fx_distortion1
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

static float fx_distortion1_faustpower2_f(float value) {
	return value * value;
}

class fx_distortion1 : public fx_distortion1_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fVslider1;
	int iVec0[2];
	float fRec4[2];
	float fConst7;
	float fConst9;
	FAUSTFLOAT fVslider2;
	float fRec6[2];
	FAUSTFLOAT fVslider3;
	float fRec7[2];
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fVslider5;
	FAUSTFLOAT fVslider6;
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fEntry1;
	FAUSTFLOAT fEntry2;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fEntry3;
	FAUSTFLOAT fEntry4;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fVslider7;
	float fVec1[2];
	FAUSTFLOAT fVslider8;
	float fRec21[2];
	float fVec2[2];
	float fRec20[2];
	float fRec19[2];
	float fVec3[2];
	float fRec18[2];
	float fVec4[2];
	float fRec17[2];
	float fRec16[3];
	float fRec15[3];
	float fConst10;
	float fConst11;
	float fRec14[3];
	float fConst12;
	float fConst13;
	float fVec5[2];
	float fRec13[2];
	float fRec12[3];
	float fVec6[2];
	float fRec11[2];
	float fRec10[3];
	float fVec7[2];
	float fRec9[2];
	float fRec8[3];
	FAUSTFLOAT fVslider9;
	float fRec22[2];
	FAUSTFLOAT fVslider10;
	float fRec24[2];
	float fRec23[3];
	FAUSTFLOAT fVslider11;
	float fRec25[2];
	FAUSTFLOAT fVslider12;
	float fRec27[2];
	float fRec26[3];
	FAUSTFLOAT fVslider13;
	float fRec28[2];
	FAUSTFLOAT fVslider14;
	float fRec30[2];
	float fRec29[3];
	float fRec5[3];
	float fVec8[2];
	float fRec3[2];
	float fVec9[2];
	float fRec2[2];
	float fRec1[3];
	float fRec0[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("HighShelf.dsp/id", "HighShelf");
		m->declare("author", "brummer");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn fx_distortion1 -scn fx_distortion1_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("copyright", "(c)brummer 2008");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_distortion1.dsp");
		m->declare("filters.lib/dcblockerat:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblockerat:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblockerat:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
		m->declare("license", "BSD");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("maxmsp.lib/author", "GRAME");
		m->declare("maxmsp.lib/copyright", "GRAME");
		m->declare("maxmsp.lib/license", "LGPL with exception");
		m->declare("maxmsp.lib/name", "MaxMSP compatibility Library");
		m->declare("maxmsp.lib/version", "1.1");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("name", "fx.distortion1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("version", "0.01");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 3.1415927f / fConst0;
		float fConst2 = 6.2831855f * (std::max<float>(0.0f, 0.5f * fConst0 + -1e+02f) / fConst0);
		float fConst3 = std::cos(fConst2);
		float fConst4 = 0.6837722f * fConst3;
		float fConst5 = 0.0056234132f * std::sin(fConst2);
		float fConst6 = fConst4 + fConst5;
		fConst7 = 1.0f / (fConst6 + 1.3162278f);
		float fConst8 = 1.3162278f * fConst3;
		fConst9 = 0.0f - 0.6324555f * (fConst8 + -0.6837722f);
		fConst10 = fConst4 + (1.3162278f - fConst5);
		fConst11 = 2.0f * (-0.6837722f - fConst8);
		fConst12 = 0.31622776f * (fConst5 + (1.3162278f - fConst4));
		fConst13 = 0.31622776f * (1.3162278f - fConst6);
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fVslider0 = FAUSTFLOAT(5e+03f);
		fVslider1 = FAUSTFLOAT(1.3e+02f);
		fVslider2 = FAUSTFLOAT(2.0f);
		fVslider3 = FAUSTFLOAT(1e+01f);
		fVslider4 = FAUSTFLOAT(0.01f);
		fVslider5 = FAUSTFLOAT(0.64f);
		fVslider6 = FAUSTFLOAT(1.0f);
		fEntry0 = FAUSTFLOAT(2.5e+02f);
		fEntry1 = FAUSTFLOAT(6.5e+02f);
		fEntry2 = FAUSTFLOAT(1.25e+03f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fEntry3 = FAUSTFLOAT(1.3e+02f);
		fEntry4 = FAUSTFLOAT(5e+03f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fVslider7 = FAUSTFLOAT(0.12f);
		fVslider8 = FAUSTFLOAT(1.0f);
		fVslider9 = FAUSTFLOAT(1e+01f);
		fVslider10 = FAUSTFLOAT(1.0f);
		fVslider11 = FAUSTFLOAT(1e+01f);
		fVslider12 = FAUSTFLOAT(1.0f);
		fVslider13 = FAUSTFLOAT(1e+01f);
		fVslider14 = FAUSTFLOAT(1.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec4[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec6[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec7[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec21[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fVec2[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec20[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec19[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fVec3[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec18[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fVec4[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec17[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = l13 + 1) {
			fRec16[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec15[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fRec14[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fVec5[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec13[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec12[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fVec6[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec11[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec10[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec7[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec9[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec8[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec22[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec24[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec23[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec25[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec27[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec26[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec28[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec30[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fRec29[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fRec5[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fVec8[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec3[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fVec9[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec2[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 3; l39 = l39 + 1) {
			fRec1[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 3; l40 = l40 + 1) {
			fRec0[l40] = 0.0f;
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
	
	virtual fx_distortion1* clone() {
		return new fx_distortion1();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.distortion1");
		ui_interface->addVerticalSlider("drive", &fVslider5, FAUSTFLOAT(0.64f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider2, "unit", "db");
		ui_interface->addVerticalSlider("gain", &fVslider2, FAUSTFLOAT(2.0f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("high_drive", &fVslider10, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider9, "unit", "db");
		ui_interface->addVerticalSlider("high_gain", &fVslider9, FAUSTFLOAT(1e+01f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("level", &fVslider4, FAUSTFLOAT(0.01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("low_drive", &fVslider14, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider13, "unit", "db");
		ui_interface->addVerticalSlider("low_gain", &fVslider13, FAUSTFLOAT(1e+01f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->openHorizontalBox("low_highcutoff");
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("sp_high_freq", &fVslider0, FAUSTFLOAT(5e+03f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.2e+04f), FAUSTFLOAT(1e+01f));
		ui_interface->declare(&fVslider1, "unit", "Hz");
		ui_interface->addVerticalSlider("sp_low_freq", &fVslider1, FAUSTFLOAT(1.3e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1e+01f));
		ui_interface->addCheckButton("sp_on_off", &fCheckbox0);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("low_highpass");
		ui_interface->addCheckButton("flt_on_off", &fCheckbox1);
		ui_interface->declare(&fEntry3, "unit", "Hz");
		ui_interface->addNumEntry("hp_freq", &fEntry3, FAUSTFLOAT(1.3e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(7.04e+03f), FAUSTFLOAT(1e+01f));
		ui_interface->declare(&fEntry4, "unit", "Hz");
		ui_interface->addNumEntry("lp_freq", &fEntry4, FAUSTFLOAT(5e+03f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(1.2e+04f), FAUSTFLOAT(1e+01f));
		ui_interface->closeBox();
		ui_interface->addVerticalSlider("middle_h_drive", &fVslider12, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider11, "unit", "db");
		ui_interface->addVerticalSlider("middle_h_gain", &fVslider11, FAUSTFLOAT(1e+01f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("middle_l_drive", &fVslider6, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider3, "unit", "db");
		ui_interface->addVerticalSlider("middle_l_gain", &fVslider3, FAUSTFLOAT(1e+01f), FAUSTFLOAT(-1e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addCheckButton("res_on_off", &fCheckbox2);
		ui_interface->declare(&fEntry2, "unit", "Hz");
		ui_interface->addNumEntry("split_high_freq", &fEntry2, FAUSTFLOAT(1.25e+03f), FAUSTFLOAT(1.25e+03f), FAUSTFLOAT(1.2e+04f), FAUSTFLOAT(1e+01f));
		ui_interface->declare(&fEntry0, "unit", "Hz");
		ui_interface->addNumEntry("split_low_freq", &fEntry0, FAUSTFLOAT(2.5e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(6e+02f), FAUSTFLOAT(1e+01f));
		ui_interface->declare(&fEntry1, "unit", "Hz");
		ui_interface->addNumEntry("split_middle_freq", &fEntry1, FAUSTFLOAT(6.5e+02f), FAUSTFLOAT(6e+02f), FAUSTFLOAT(1.25e+03f), FAUSTFLOAT(1e+01f));
		ui_interface->addVerticalSlider("trigger", &fVslider7, FAUSTFLOAT(0.12f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("vibrato", &fVslider8, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = std::tan(fConst1 * float(fVslider0));
		float fSlow2 = 1.0f / fSlow1;
		float fSlow3 = 1.0f / ((fSlow2 + 0.76536685f) / fSlow1 + 1.0f);
		float fSlow4 = 1.0f / ((fSlow2 + 1.847759f) / fSlow1 + 1.0f);
		float fSlow5 = fConst1 * float(fVslider1);
		float fSlow6 = 1.0f / (fSlow5 + 1.0f);
		float fSlow7 = 0.001f * std::pow(1e+01f, 0.05f * (float(fVslider2) + -1e+01f));
		float fSlow8 = 0.001f * std::pow(1e+01f, 0.05f * (float(fVslider3) + -1e+01f));
		float fSlow9 = float(fVslider4);
		float fSlow10 = float(fVslider5);
		float fSlow11 = std::tan(fConst1 * float(fEntry0));
		float fSlow12 = 1.0f / fSlow11;
		float fSlow13 = (fSlow12 + 1.0f) / fSlow11 + 1.0f;
		float fSlow14 = std::pow(1e+01f, 2.0f * fSlow10 * float(fVslider6)) / fSlow13;
		float fSlow15 = fx_distortion1_faustpower2_f(fSlow11);
		float fSlow16 = 1.0f / fSlow15;
		float fSlow17 = fSlow12 + 1.0f;
		float fSlow18 = 0.0f - 1.0f / (fSlow11 * fSlow17);
		float fSlow19 = std::tan(fConst1 * float(fEntry1));
		float fSlow20 = 1.0f / fSlow19;
		float fSlow21 = (fSlow20 + 1.0f) / fSlow19 + 1.0f;
		float fSlow22 = 1.0f / fSlow21;
		float fSlow23 = fSlow20 + 1.0f;
		float fSlow24 = 1.0f / fSlow23;
		float fSlow25 = std::tan(fConst1 * float(fEntry2));
		float fSlow26 = 1.0f / fSlow25;
		float fSlow27 = (fSlow26 + 1.0f) / fSlow25 + 1.0f;
		float fSlow28 = 1.0f / fSlow27;
		float fSlow29 = fSlow26 + 1.0f;
		float fSlow30 = 1.0f / fSlow29;
		float fSlow31 = 1.0f - fSlow26;
		int iSlow32 = int(float(fCheckbox1));
		float fSlow33 = std::tan(fConst1 * float(fEntry3));
		float fSlow34 = 1.0f / fSlow33;
		float fSlow35 = fSlow34 + 1.0f;
		float fSlow36 = 0.0f - 1.0f / (fSlow33 * fSlow35);
		float fSlow37 = 1.0f / std::tan(fConst1 * float(fEntry4));
		float fSlow38 = 1.0f / (fSlow37 + 1.0f);
		float fSlow39 = 1.0f - fSlow37;
		int iSlow40 = int(float(fCheckbox2));
		float fSlow41 = 1.0f - float(fVslider7);
		float fSlow42 = float(fVslider8);
		int iSlow43 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fSlow42 + -1.0f)));
		int iSlow44 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fSlow42)));
		float fSlow45 = 1.0f / fSlow35;
		float fSlow46 = 1.0f - fSlow34;
		float fSlow47 = 1.0f - fSlow5;
		float fSlow48 = (fSlow2 + -1.847759f) / fSlow1 + 1.0f;
		float fSlow49 = 2.0f * (1.0f - 1.0f / fx_distortion1_faustpower2_f(fSlow1));
		float fSlow50 = (fSlow2 + -0.76536685f) / fSlow1 + 1.0f;
		float fSlow51 = (fSlow26 + -1.0f) / fSlow25 + 1.0f;
		float fSlow52 = fx_distortion1_faustpower2_f(fSlow25);
		float fSlow53 = 1.0f / fSlow52;
		float fSlow54 = 2.0f * (1.0f - fSlow53);
		float fSlow55 = 1.0f - fSlow20;
		float fSlow56 = (fSlow20 + -1.0f) / fSlow19 + 1.0f;
		float fSlow57 = fx_distortion1_faustpower2_f(fSlow19);
		float fSlow58 = 1.0f / fSlow57;
		float fSlow59 = 2.0f * (1.0f - fSlow58);
		float fSlow60 = 1.0f / fSlow17;
		float fSlow61 = 1.0f / (fSlow21 * fSlow11);
		float fSlow62 = 1.0f - fSlow12;
		float fSlow63 = 1.0f / fSlow13;
		float fSlow64 = (fSlow12 + -1.0f) / fSlow11 + 1.0f;
		float fSlow65 = 2.0f * (1.0f - fSlow16);
		float fSlow66 = 0.0f - 2.0f / fSlow15;
		float fSlow67 = 0.001f * std::pow(1e+01f, 0.05f * (float(fVslider9) + -1e+01f));
		float fSlow68 = std::pow(1e+01f, 2.0f * float(fVslider10) * fSlow10) / fSlow27;
		float fSlow69 = 1.0f / (fSlow25 * fSlow29);
		float fSlow70 = 0.0f - fSlow69;
		float fSlow71 = fSlow31 / fSlow29;
		float fSlow72 = 0.0f - 2.0f / fSlow52;
		float fSlow73 = 0.001f * std::pow(1e+01f, 0.05f * (float(fVslider11) + -1e+01f));
		float fSlow74 = std::pow(1e+01f, 2.0f * fSlow10 * float(fVslider12)) / fSlow21;
		float fSlow75 = 0.0f - 1.0f / (fSlow19 * fSlow23);
		float fSlow76 = 1.0f / (fSlow27 * fSlow19);
		float fSlow77 = 0.0f - 2.0f / fSlow57;
		float fSlow78 = 0.001f * std::pow(1e+01f, 0.05f * (float(fVslider13) + -1e+01f));
		float fSlow79 = std::pow(1e+01f, 2.0f * fSlow10 * float(fVslider14)) / fSlow13;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec4[0] = 1e-20f * float(1 - iVec0[1]) - fRec4[1];
			fRec6[0] = fSlow7 + 0.999f * fRec6[1];
			fRec7[0] = fSlow8 + 0.999f * fRec7[1];
			float fTemp0 = float(input0[i0]);
			float fTemp1 = fTemp0 + fSlow41 * fRec21[1];
			fVec1[0] = fTemp1;
			fRec21[0] = 0.5f * (fVec1[iSlow43] + fVec1[iSlow44]);
			float fTemp2 = fRec4[0] + ((iSlow40) ? fRec21[0] : fTemp0);
			float fTemp3 = fRec4[0] + ((iSlow32) ? 0.0f : fTemp2);
			fVec2[0] = fTemp3;
			fRec20[0] = 0.0f - fSlow38 * (fSlow39 * fRec20[1] - (fTemp3 + fVec2[1]));
			fRec19[0] = fSlow36 * fRec20[1] - fSlow45 * (fSlow46 * fRec19[1] - fSlow34 * fRec20[0]);
			float fTemp4 = ((iSlow32) ? fTemp2 : fRec19[0]);
			float fTemp5 = fRec4[0] + ((iSlow0) ? 0.0f : fTemp4);
			fVec3[0] = fSlow6 * fTemp5;
			fRec18[0] = fSlow6 * (fTemp5 + fSlow47 * fRec18[1]) - fVec3[1];
			fVec4[0] = fSlow6 * fRec18[0];
			fRec17[0] = fSlow6 * (fRec18[0] + fSlow47 * fRec17[1]) - fVec4[1];
			fRec16[0] = fRec17[0] - fSlow4 * (fSlow48 * fRec16[2] + fSlow49 * fRec16[1]);
			fRec15[0] = fSlow4 * (fRec16[2] + fRec16[0] + 2.0f * fRec16[1]) - fSlow3 * (fSlow50 * fRec15[2] + fSlow49 * fRec15[1]);
			fRec14[0] = ((iSlow0) ? fTemp4 : fSlow3 * (fRec15[2] + fRec15[0] + 2.0f * fRec15[1])) - fConst7 * (fConst10 * fRec14[2] + fConst11 * fRec14[1]);
			float fTemp6 = fConst9 * fRec14[1] + fConst12 * fRec14[0] + fConst13 * fRec14[2];
			fVec5[0] = fTemp6;
			fRec13[0] = 0.0f - fSlow30 * (fSlow31 * fRec13[1] - fConst7 * (fTemp6 + fVec5[1]));
			fRec12[0] = fRec13[0] - fSlow28 * (fSlow51 * fRec12[2] + fSlow54 * fRec12[1]);
			float fTemp7 = fRec12[2] + fRec12[0] + 2.0f * fRec12[1];
			float fTemp8 = fSlow28 * fTemp7;
			fVec6[0] = fTemp8;
			fRec11[0] = fSlow24 * (fTemp8 + fVec6[1] - fSlow55 * fRec11[1]);
			fRec10[0] = fRec11[0] - fSlow22 * (fSlow56 * fRec10[2] + fSlow59 * fRec10[1]);
			float fTemp9 = fRec10[2] + fRec10[0] + 2.0f * fRec10[1];
			float fTemp10 = fSlow22 * fTemp9;
			fVec7[0] = fTemp10;
			fRec9[0] = fSlow18 * fVec7[1] + fSlow60 * (fSlow61 * fTemp9 - fSlow62 * fRec9[1]);
			fRec8[0] = fRec9[0] - fSlow63 * (fSlow64 * fRec8[2] + fSlow65 * fRec8[1]);
			float fTemp11 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow9 + fSlow14 * (fSlow16 * fRec8[0] + fSlow66 * fRec8[1] + fSlow16 * fRec8[2])));
			fRec22[0] = fSlow67 + 0.999f * fRec22[1];
			fRec24[0] = fConst7 * (fSlow69 * fTemp6 + fSlow70 * fVec5[1]) - fSlow71 * fRec24[1];
			fRec23[0] = fRec24[0] - fSlow28 * (fSlow51 * fRec23[2] + fSlow54 * fRec23[1]);
			float fTemp12 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow9 + fSlow68 * (fSlow53 * fRec23[0] + fSlow72 * fRec23[1] + fSlow53 * fRec23[2])));
			fRec25[0] = fSlow73 + 0.999f * fRec25[1];
			fRec27[0] = fSlow75 * fVec6[1] - fSlow24 * (fSlow55 * fRec27[1] - fSlow76 * fTemp7);
			fRec26[0] = fRec27[0] - fSlow22 * (fSlow56 * fRec26[2] + fSlow59 * fRec26[1]);
			float fTemp13 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow9 + fSlow74 * (fSlow58 * fRec26[0] + fSlow77 * fRec26[1] + fSlow58 * fRec26[2])));
			fRec28[0] = fSlow78 + 0.999f * fRec28[1];
			fRec30[0] = 0.0f - fSlow60 * (fSlow62 * fRec30[1] - (fTemp10 + fVec7[1]));
			fRec29[0] = fRec30[0] - fSlow63 * (fSlow64 * fRec29[2] + fSlow65 * fRec29[1]);
			float fTemp14 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow9 + fSlow79 * (fRec29[2] + fRec29[0] + 2.0f * fRec29[1])));
			fRec5[0] = fRec6[0] * (fRec7[0] * fTemp11 * (1.0f - 0.33333334f * fx_distortion1_faustpower2_f(fTemp11)) + fRec22[0] * fTemp12 * (1.0f - 0.33333334f * fx_distortion1_faustpower2_f(fTemp12)) + fRec25[0] * fTemp13 * (1.0f - 0.33333334f * fx_distortion1_faustpower2_f(fTemp13)) + fRec28[0] * fTemp14 * (1.0f - 0.33333334f * fx_distortion1_faustpower2_f(fTemp14))) - fConst7 * (fConst10 * fRec5[2] + fConst11 * fRec5[1]);
			float fTemp15 = fConst7 * (fConst9 * fRec5[1] + fConst12 * fRec5[0] + fConst13 * fRec5[2]);
			float fTemp16 = fRec4[0] + ((iSlow0) ? 0.0f : fTemp15);
			fVec8[0] = fSlow6 * fTemp16;
			fRec3[0] = fSlow6 * (fTemp16 + fSlow47 * fRec3[1]) - fVec8[1];
			fVec9[0] = fSlow6 * fRec3[0];
			fRec2[0] = fSlow6 * (fRec3[0] + fSlow47 * fRec2[1]) - fVec9[1];
			fRec1[0] = fRec2[0] - fSlow4 * (fSlow48 * fRec1[2] + fSlow49 * fRec1[1]);
			fRec0[0] = fSlow4 * (fRec1[2] + fRec1[0] + 2.0f * fRec1[1]) - fSlow3 * (fSlow50 * fRec0[2] + fSlow49 * fRec0[1]);
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp15 : fSlow3 * (fRec0[2] + fRec0[0] + 2.0f * fRec0[1])));
			iVec0[1] = iVec0[0];
			fRec4[1] = fRec4[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fVec1[1] = fVec1[0];
			fRec21[1] = fRec21[0];
			fVec2[1] = fVec2[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fVec3[1] = fVec3[0];
			fRec18[1] = fRec18[0];
			fVec4[1] = fVec4[0];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fVec5[1] = fVec5[0];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fVec6[1] = fVec6[0];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fVec7[1] = fVec7[0];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec22[1] = fRec22[0];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec25[1] = fRec25[0];
			fRec27[1] = fRec27[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec28[1] = fRec28[0];
			fRec30[1] = fRec30[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec8[1] = fVec8[0];
			fRec3[1] = fRec3[0];
			fVec9[1] = fVec9[0];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
		}
	}

};
#endif

class faust_fx_distortion1_tilde : public FaustExternal<fx_distortion1> {
public:
    faust_fx_distortion1_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(fx_distortion1))
    {
    }
};

#endif
