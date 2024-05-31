/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "synth.risset_tone"
version: "0.1"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_risset_tone -scn synth_risset_tone_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_risset_tone_H__
#define  __synth_risset_tone_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_risset_tone_dsp.h ********************************
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
#define FAUSTVERSION "2.74.5."

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 74
#define FAUSTPATCHVERSION 5.

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

class FAUST_API synth_risset_tone_dsp {

    public:

        synth_risset_tone_dsp() {}
        virtual ~synth_risset_tone_dsp() {}

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
         * Init instance state.
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state.
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
        virtual synth_risset_tone_dsp* clone() = 0;
    
        /**
         * Trigger the Meta* m parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * Read all controllers (buttons, sliders, etc.), and update the DSP state to be used by 'frame' or 'compute'.
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

class FAUST_API decorator_dsp : public synth_risset_tone_dsp {

    protected:

        synth_risset_tone_dsp* fDSP;

    public:

        decorator_dsp(synth_risset_tone_dsp* synth_risset_tone_dsp = nullptr):fDSP(synth_risset_tone_dsp) {}
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
        virtual synth_risset_tone_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_risset_tone_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_risset_tone_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_risset_tone : public synth_risset_tone_dsp {
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
#define FAUSTCLASS synth_risset_tone
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

class synth_risset_toneSIG0 {
	
  private:
	
	int iVec0[2];
	int iRec0[2];
	
  public:
	
	int getNumInputssynth_risset_toneSIG0() {
		return 0;
	}
	int getNumOutputssynth_risset_toneSIG0() {
		return 1;
	}
	
	void instanceInitsynth_risset_toneSIG0(int sample_rate) {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iRec0[l1] = 0;
		}
	}
	
	void fillsynth_risset_toneSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec0[0] = (iVec0[1] + iRec0[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec0[0]));
			iVec0[1] = iVec0[0];
			iRec0[1] = iRec0[0];
		}
	}

};

static synth_risset_toneSIG0* newsynth_risset_toneSIG0() { return (synth_risset_toneSIG0*)new synth_risset_toneSIG0(); }
static void deletesynth_risset_toneSIG0(synth_risset_toneSIG0* dsp) { delete dsp; }

class synth_risset_toneSIG1 {
	
  private:
	
	int iRec3[2];
	
  public:
	
	int getNumInputssynth_risset_toneSIG1() {
		return 0;
	}
	int getNumOutputssynth_risset_toneSIG1() {
		return 1;
	}
	
	void instanceInitsynth_risset_toneSIG1(int sample_rate) {
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec3[l5] = 0;
		}
	}
	
	void fillsynth_risset_toneSIG1(int count, float* table) {
		for (int i2 = 0; i2 < count; i2 = i2 + 1) {
			iRec3[0] = iRec3[1] + 1;
			table[i2] = std::exp(4.8283f * (std::cos(9.58738e-05f * (float(iRec3[0] + -1) + -32768.0f)) + -1.0f));
			iRec3[1] = iRec3[0];
		}
	}

};

static synth_risset_toneSIG1* newsynth_risset_toneSIG1() { return (synth_risset_toneSIG1*)new synth_risset_toneSIG1(); }
static void deletesynth_risset_toneSIG1(synth_risset_toneSIG1* dsp) { delete dsp; }

static float ftbl0synth_risset_toneSIG0[65536];
static float ftbl1synth_risset_toneSIG1[65537];

class synth_risset_tone : public synth_risset_tone_dsp {
	
 private:
	
	int iVec1[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	float fConst2;
	FAUSTFLOAT fHslider2;
	float fRec2[2];
	float fRec1[2];
	float fRec4[2];
	float fRec5[2];
	float fRec6[2];
	float fRec7[2];
	float fRec8[2];
	float fRec9[2];
	float fRec10[2];
	float fRec11[2];
	float fRec12[2];
	
 public:
	synth_risset_tone() {
	}
	
	void metadata(Meta* m) { 
		m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.17.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.4");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_risset_tone -scn synth_risset_tone_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "Oliver Larkin");
		m->declare("description", "Jean Claude Risset's endless glissando");
		m->declare("filename", "synth_risset_tone.dsp");
		m->declare("licence", "GPL");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "synth.risset_tone");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("version", "0.1");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		synth_risset_toneSIG0* sig0 = newsynth_risset_toneSIG0();
		sig0->instanceInitsynth_risset_toneSIG0(sample_rate);
		sig0->fillsynth_risset_toneSIG0(65536, ftbl0synth_risset_toneSIG0);
		synth_risset_toneSIG1* sig1 = newsynth_risset_toneSIG1();
		sig1->instanceInitsynth_risset_toneSIG1(sample_rate);
		sig1->fillsynth_risset_toneSIG1(65537, ftbl1synth_risset_toneSIG1);
		deletesynth_risset_toneSIG0(sig0);
		deletesynth_risset_toneSIG1(sig1);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 4.4e+02f / fConst0;
		fConst2 = 1.0f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(6e+01f);
		fHslider1 = FAUSTFLOAT(1.2e+02f);
		fHslider2 = FAUSTFLOAT(0.01f);
	}
	
	virtual void instanceClear() {
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iVec1[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec1[l4] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec4[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec5[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec7[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec10[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
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
	
	virtual synth_risset_tone* clone() {
		return new synth_risset_tone();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.risset_tone");
		ui_interface->addHorizontalSlider("centroid", &fHslider0, FAUSTFLOAT(6e+01f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("range", &fHslider1, FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("rate", &fHslider2, FAUSTFLOAT(0.01f), FAUSTFLOAT(-2.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = 0.5f * float(fHslider1);
		float fSlow2 = fConst2 * float(fHslider2);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec1[0] = 1;
			int iTemp0 = 1 - iVec1[1];
			fRec2[0] = fSlow2 + (fRec2[1] - std::floor(fSlow2 + fRec2[1]));
			float fTemp1 = std::fmod(fRec2[0] + 0.3f, 1.0f);
			float fTemp2 = ((iTemp0) ? 0.0f : fRec1[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp1 + -1.0f) + 7.0f)) + -69.0f)));
			fRec1[0] = fTemp2 - std::floor(fTemp2);
			float fTemp3 = 65536.0f * fTemp1;
			int iTemp4 = int(fTemp3);
			float fTemp5 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp4, 65536))];
			float fTemp6 = std::fmod(fRec2[0] + 0.1f, 1.0f);
			float fTemp7 = ((iTemp0) ? 0.0f : fRec4[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp6 + -1.0f) + 7.0f)) + -69.0f)));
			fRec4[0] = fTemp7 - std::floor(fTemp7);
			float fTemp8 = 65536.0f * fTemp6;
			int iTemp9 = int(fTemp8);
			float fTemp10 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp9, 65536))];
			float fTemp11 = std::fmod(fRec2[0] + 0.2f, 1.0f);
			float fTemp12 = ((iTemp0) ? 0.0f : fRec5[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp11 + -1.0f) + 7.0f)) + -69.0f)));
			fRec5[0] = fTemp12 - std::floor(fTemp12);
			float fTemp13 = 65536.0f * fTemp11;
			int iTemp14 = int(fTemp13);
			float fTemp15 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp14, 65536))];
			float fTemp16 = std::fmod(fRec2[0], 1.0f);
			float fTemp17 = ((iTemp0) ? 0.0f : fRec6[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp16 + -1.0f) + 7.0f)) + -69.0f)));
			fRec6[0] = fTemp17 - std::floor(fTemp17);
			float fTemp18 = 65536.0f * fTemp16;
			int iTemp19 = int(fTemp18);
			float fTemp20 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp19, 65536))];
			float fTemp21 = std::fmod(fRec2[0] + 0.6f, 1.0f);
			float fTemp22 = ((iTemp0) ? 0.0f : fRec7[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp21 + -1.0f) + 7.0f)) + -69.0f)));
			fRec7[0] = fTemp22 - std::floor(fTemp22);
			float fTemp23 = 65536.0f * fTemp21;
			int iTemp24 = int(fTemp23);
			float fTemp25 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp24, 65536))];
			float fTemp26 = std::fmod(fRec2[0] + 0.7f, 1.0f);
			float fTemp27 = ((iTemp0) ? 0.0f : fRec8[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp26 + -1.0f) + 7.0f)) + -69.0f)));
			fRec8[0] = fTemp27 - std::floor(fTemp27);
			float fTemp28 = 65536.0f * fTemp26;
			int iTemp29 = int(fTemp28);
			float fTemp30 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp29, 65536))];
			float fTemp31 = std::fmod(fRec2[0] + 0.4f, 1.0f);
			float fTemp32 = ((iTemp0) ? 0.0f : fRec9[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp31 + -1.0f) + 7.0f)) + -69.0f)));
			fRec9[0] = fTemp32 - std::floor(fTemp32);
			float fTemp33 = 65536.0f * fTemp31;
			int iTemp34 = int(fTemp33);
			float fTemp35 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp34, 65536))];
			float fTemp36 = std::fmod(fRec2[0] + 0.5f, 1.0f);
			float fTemp37 = ((iTemp0) ? 0.0f : fRec10[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp36 + -1.0f) + 7.0f)) + -69.0f)));
			fRec10[0] = fTemp37 - std::floor(fTemp37);
			float fTemp38 = 65536.0f * fTemp36;
			int iTemp39 = int(fTemp38);
			float fTemp40 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp39, 65536))];
			float fTemp41 = std::fmod(fRec2[0] + 0.8f, 1.0f);
			float fTemp42 = ((iTemp0) ? 0.0f : fRec11[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp41 + -1.0f) + 7.0f)) + -69.0f)));
			fRec11[0] = fTemp42 - std::floor(fTemp42);
			float fTemp43 = 65536.0f * fTemp41;
			int iTemp44 = int(fTemp43);
			float fTemp45 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp44, 65536))];
			float fTemp46 = std::fmod(fRec2[0] + 0.9f, 1.0f);
			float fTemp47 = ((iTemp0) ? 0.0f : fRec12[1] + fConst1 * std::pow(2.0f, 0.083333336f * (std::min<float>(1.2e+02f, std::max<float>(2e+01f, fSlow0 + fSlow1 * (2.0f * fTemp46 + -1.0f) + 7.0f)) + -69.0f)));
			fRec12[0] = fTemp47 - std::floor(fTemp47);
			float fTemp48 = 65536.0f * fTemp46;
			int iTemp49 = int(fTemp48);
			float fTemp50 = ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp49, 65536))];
			output0[i0] = FAUSTFLOAT(0.1f * (ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[0]), 65535))] * (fTemp5 + (fTemp3 - std::floor(fTemp3)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp4 + 1, 65536))] - fTemp5)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec4[0]), 65535))] * (fTemp10 + (fTemp8 - std::floor(fTemp8)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp9 + 1, 65536))] - fTemp10)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec5[0]), 65535))] * (fTemp15 + (fTemp13 - std::floor(fTemp13)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp14 + 1, 65536))] - fTemp15)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec6[0]), 65535))] * (fTemp20 + (fTemp18 - std::floor(fTemp18)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp19 + 1, 65536))] - fTemp20)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec7[0]), 65535))] * (fTemp25 + (fTemp23 - std::floor(fTemp23)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp24 + 1, 65536))] - fTemp25)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec8[0]), 65535))] * (fTemp30 + (fTemp28 - std::floor(fTemp28)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp29 + 1, 65536))] - fTemp30)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec9[0]), 65535))] * (fTemp35 + (fTemp33 - std::floor(fTemp33)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp34 + 1, 65536))] - fTemp35)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec10[0]), 65535))] * (fTemp40 + (fTemp38 - std::floor(fTemp38)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp39 + 1, 65536))] - fTemp40)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec11[0]), 65535))] * (fTemp45 + (fTemp43 - std::floor(fTemp43)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp44 + 1, 65536))] - fTemp45)) + ftbl0synth_risset_toneSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec12[0]), 65535))] * (fTemp50 + (fTemp48 - std::floor(fTemp48)) * (ftbl1synth_risset_toneSIG1[std::max<int>(0, std::min<int>(iTemp49 + 1, 65536))] - fTemp50))));
			iVec1[1] = iVec1[0];
			fRec2[1] = fRec2[0];
			fRec1[1] = fRec1[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
		}
	}

};
#endif

class faust_synth_risset_tone_tilde : public FaustExternal<synth_risset_tone> {
public:
    faust_synth_risset_tone_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_risset_tone))
    {
    }
};

#endif
