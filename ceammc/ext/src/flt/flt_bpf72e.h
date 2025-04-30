/* ------------------------------------------------------------
name: "flt.bpf72e"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn flt_bpf72e -scn flt_bpf72e_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0
------------------------------------------------------------ */

#ifndef  __flt_bpf72e_H__
#define  __flt_bpf72e_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN flt_bpf72e_dsp.h ********************************
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

class FAUST_API flt_bpf72e_dsp {

    public:

        flt_bpf72e_dsp() {}
        virtual ~flt_bpf72e_dsp() {}

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
        virtual flt_bpf72e_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public flt_bpf72e_dsp {

    protected:

        flt_bpf72e_dsp* fDSP;

    public:

        decorator_dsp(flt_bpf72e_dsp* flt_bpf72e_dsp = nullptr):fDSP(flt_bpf72e_dsp) {}
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
        virtual flt_bpf72e_dsp* createDSPInstance() = 0;
    
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

/************************** END flt_bpf72e_dsp.h **************************/
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
 The base class of Meta handler to be used in flt_bpf72e_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct flt_bpf72e : public flt_bpf72e_dsp {
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
#define FAUSTCLASS flt_bpf72e
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

static double flt_bpf72e_faustpower2_f(double value) {
	return value * value;
}
static double flt_bpf72e_faustpower3_f(double value) {
	return value * value * value;
}
static double flt_bpf72e_faustpower4_f(double value) {
	return value * value * value * value;
}

class flt_bpf72e : public flt_bpf72e_dsp {
	
 private:
	
	int fSampleRate;
	double fConst0;
	double fConst1;
	double fConst2;
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fHslider0;
	double fConst3;
	double fConst4;
	double fConst5;
	double fConst6;
	double fConst7;
	double fConst8;
	double fConst9;
	double fConst10;
	double fConst11;
	double fConst12;
	double fConst13;
	double fConst14;
	double fConst15;
	double fConst16;
	double fConst17;
	double fConst18;
	double fConst19;
	double fRec2[5];
	double fConst20;
	double fConst21;
	double fConst22;
	double fConst23;
	double fConst24;
	double fRec1[5];
	double fConst25;
	double fConst26;
	double fConst27;
	double fRec0[5];
	
 public:
	flt_bpf72e() {
	}
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn flt_bpf72e -scn flt_bpf72e_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0");
		m->declare("filename", "flt_bpf72e.dsp");
		m->declare("filters.lib/bandpass12e:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass12e:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass12e:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf2sb:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2sb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2sb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "flt.bpf72e");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
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
		fConst0 = std::min<double>(1.92e+05, std::max<double>(1.0, double(fSampleRate)));
		fConst1 = 2.0 * fConst0;
		fConst2 = 3.141592653589793 / fConst0;
		fConst3 = 0.499 * fConst0;
		fConst4 = 0.5 / fConst0;
		fConst5 = 4.0 * flt_bpf72e_faustpower2_f(fConst0);
		fConst6 = 1.347238968908712 / fConst0;
		fConst7 = 1.0 / fConst0;
		fConst8 = flt_bpf72e_faustpower3_f(fConst7);
		fConst9 = 0.336809742227178 * fConst8;
		fConst10 = flt_bpf72e_faustpower4_f(fConst7);
		fConst11 = flt_bpf72e_faustpower2_f(fConst7);
		fConst12 = 4.0 * fConst10;
		fConst13 = 4.099829135113128 / fConst0;
		fConst14 = 1.024957283778282 * fConst8;
		fConst15 = 6.25930437457316 / fConst0;
		fConst16 = 1.56482609364329 * fConst8;
		fConst17 = 3.12965218728658 * fConst8;
		fConst18 = 12.51860874914632 / fConst0;
		fConst19 = 6.0 * fConst10;
		fConst20 = 0.00039999998822 * fConst10;
		fConst21 = 0.0005999999823299999 * fConst10;
		fConst22 = 0.00019999999411 * fConst10;
		fConst23 = 2.049914567556564 * fConst8;
		fConst24 = 8.199658270226257 / fConst0;
		fConst25 = 2.0 * fConst10;
		fConst26 = 0.673619484454356 * fConst8;
		fConst27 = 2.694477937817424 / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(1e+03);
		fHslider0 = FAUSTFLOAT(2.0);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 5; l0 = l0 + 1) {
			fRec2[l0] = 0.0;
		}
		for (int l1 = 0; l1 < 5; l1 = l1 + 1) {
			fRec1[l1] = 0.0;
		}
		for (int l2 = 0; l2 < 5; l2 = l2 + 1) {
			fRec0[l2] = 0.0;
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
	
	virtual flt_bpf72e* clone() {
		return new flt_bpf72e();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("flt.bpf72e");
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("freq", &fVslider0, FAUSTFLOAT(1e+03), FAUSTFLOAT(2e+01), FAUSTFLOAT(2e+04), FAUSTFLOAT(0.1));
		ui_interface->addHorizontalSlider("q", &fHslider0, FAUSTFLOAT(2.0), FAUSTFLOAT(0.25), FAUSTFLOAT(32.0), FAUSTFLOAT(0.001));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		double fSlow0 = double(fVslider0);
		double fSlow1 = 0.5 / double(fHslider0);
		double fSlow2 = std::tan(fConst2 * std::min<double>(fSlow0 * (fSlow1 + 1.0), fConst3));
		double fSlow3 = std::sqrt(fConst5 * std::tan(fConst2 * std::max<double>(fSlow0 * (1.0 - fSlow1), 2e+01)) * fSlow2);
		double fSlow4 = flt_bpf72e_faustpower2_f(fSlow3);
		double fSlow5 = fConst1 * fSlow2 - fConst4 * (fSlow4 / fSlow2);
		double fSlow6 = fSlow5 * (fConst6 + fConst9 * fSlow4);
		double fSlow7 = flt_bpf72e_faustpower4_f(fSlow3);
		double fSlow8 = fConst10 * fSlow7;
		double fSlow9 = 8.0 * fSlow4;
		double fSlow10 = flt_bpf72e_faustpower2_f(fSlow5);
		double fSlow11 = fSlow8 + fConst11 * (fSlow9 + 4.277433630829248 * fSlow10);
		double fSlow12 = 1.0 / (fSlow6 + fSlow11 + 16.0);
		double fSlow13 = fConst12 * fSlow7;
		double fSlow14 = fSlow13 + -64.0;
		double fSlow15 = fSlow5 * (fConst13 + fConst14 * fSlow4);
		double fSlow16 = fSlow8 + fConst11 * (fSlow9 + 2.7584854579387 * fSlow10);
		double fSlow17 = 1.0 / (fSlow15 + fSlow16 + 16.0);
		double fSlow18 = fConst11 * (fSlow9 + 30.486925195482414 * fSlow10);
		double fSlow19 = fSlow18 + 16.0;
		double fSlow20 = fSlow5 * (fConst15 + fConst16 * fSlow4);
		double fSlow21 = fSlow8 + fConst11 * (0.98116603482464 * fSlow10 + fSlow9);
		double fSlow22 = 1.0 / (fSlow20 + fSlow21 + 16.0);
		double fSlow23 = fConst11 * (0.00173290880222 * fSlow10 + 0.00079999997644 * fSlow4);
		double fSlow24 = fSlow23 + 0.00159999995288;
		double fSlow25 = fConst17 * fSlow4;
		double fSlow26 = fSlow13 + fSlow5 * (fSlow25 - fConst18) + -64.0;
		double fSlow27 = fConst19 * fSlow7;
		double fSlow28 = 16.0 * fSlow4;
		double fSlow29 = fSlow27 + (96.0 - fConst11 * (1.96233206964928 * fSlow10 + fSlow28));
		double fSlow30 = fSlow13 + fSlow5 * (fConst18 - fSlow25) + -64.0;
		double fSlow31 = fSlow21 + (16.0 - fSlow20);
		double fSlow32 = fConst20 * fSlow7 + -0.00639999981152;
		double fSlow33 = fConst21 * fSlow7 + (0.009599999717279999 - fConst11 * (0.00346581760444 * fSlow10 + 0.00159999995288 * fSlow4));
		double fSlow34 = fSlow23 + fConst22 * fSlow7 + 0.00159999995288;
		double fSlow35 = fConst23 * fSlow4;
		double fSlow36 = fSlow13 + fSlow5 * (fSlow35 - fConst24) + -64.0;
		double fSlow37 = fSlow27 + (96.0 - fConst11 * (fSlow28 + 5.5169709158774 * fSlow10));
		double fSlow38 = fSlow13 + fSlow5 * (fConst24 - fSlow35) + -64.0;
		double fSlow39 = fSlow16 + (16.0 - fSlow15);
		double fSlow40 = fSlow27 + (96.0 - fConst11 * (fSlow28 + 60.97385039096483 * fSlow10));
		double fSlow41 = fConst25 * fSlow7;
		double fSlow42 = fSlow18 + fSlow41 + 16.0;
		double fSlow43 = fConst26 * fSlow4;
		double fSlow44 = fSlow13 + fSlow5 * (fSlow43 - fConst27) + -64.0;
		double fSlow45 = fSlow27 + (96.0 - fConst11 * (fSlow28 + 8.554867261658496 * fSlow10));
		double fSlow46 = fSlow13 + fSlow5 * (fConst27 - fSlow43) + -64.0;
		double fSlow47 = fSlow11 + (16.0 - fSlow6);
		double fSlow48 = fConst11 * (fSlow9 + 214.1446118182269 * fSlow10);
		double fSlow49 = fSlow41 + fSlow48 + 16.0;
		double fSlow50 = fSlow48 + 16.0;
		double fSlow51 = fSlow27 + (96.0 - fConst11 * (fSlow28 + 428.2892236364538 * fSlow10));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec2[0] = double(input0[i0]) - fSlow22 * (fSlow26 * fRec2[1] + fSlow29 * fRec2[2] + fSlow30 * fRec2[3] + fSlow31 * fRec2[4]);
			fRec1[0] = fSlow22 * (fSlow24 * fRec2[0] + fSlow32 * fRec2[1] + fSlow33 * fRec2[2] + fSlow32 * fRec2[3] + fSlow34 * fRec2[4]) - fSlow17 * (fSlow36 * fRec1[1] + fSlow37 * fRec1[2] + fSlow38 * fRec1[3] + fSlow39 * fRec1[4]);
			fRec0[0] = fSlow17 * (fSlow19 * fRec1[0] + fSlow14 * fRec1[1] + fSlow40 * fRec1[2] + fSlow14 * fRec1[3] + fSlow42 * fRec1[4]) - fSlow12 * (fSlow44 * fRec0[1] + fSlow45 * fRec0[2] + fSlow46 * fRec0[3] + fSlow47 * fRec0[4]);
			output0[i0] = FAUSTFLOAT(fSlow12 * (fSlow14 * (fRec0[1] + fRec0[3]) + fSlow49 * fRec0[4] + fSlow50 * fRec0[0] + fSlow51 * fRec0[2]));
			for (int j0 = 4; j0 > 0; j0 = j0 - 1) {
				fRec2[j0] = fRec2[j0 - 1];
			}
			for (int j1 = 4; j1 > 0; j1 = j1 - 1) {
				fRec1[j1] = fRec1[j1 - 1];
			}
			for (int j2 = 4; j2 > 0; j2 = j2 - 1) {
				fRec0[j2] = fRec0[j2 - 1];
			}
		}
	}

};
#endif

class faust_flt_bpf72e_tilde : public FaustExternal<flt_bpf72e> {
public:
    faust_flt_bpf72e_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(flt_bpf72e))
    {
    }
};

#endif
