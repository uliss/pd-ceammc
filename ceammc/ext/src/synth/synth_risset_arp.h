/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "synth.risset_arp"
version: "0.1"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_risset_arp -scn synth_risset_arp_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_risset_arp_H__
#define  __synth_risset_arp_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_risset_arp_dsp.h ********************************
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

class FAUST_API synth_risset_arp_dsp {

    public:

        synth_risset_arp_dsp() {}
        virtual ~synth_risset_arp_dsp() {}

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
        virtual synth_risset_arp_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_risset_arp_dsp {

    protected:

        synth_risset_arp_dsp* fDSP;

    public:

        decorator_dsp(synth_risset_arp_dsp* synth_risset_arp_dsp = nullptr):fDSP(synth_risset_arp_dsp) {}
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
        virtual synth_risset_arp_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_risset_arp_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_risset_arp_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_risset_arp : public synth_risset_arp_dsp {
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
#define FAUSTCLASS synth_risset_arp
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

class synth_risset_arpSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec2[2];
	
  public:
	
	int getNumInputssynth_risset_arpSIG0() {
		return 0;
	}
	int getNumOutputssynth_risset_arpSIG0() {
		return 1;
	}
	
	void instanceInitsynth_risset_arpSIG0(int sample_rate) {
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iVec1[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec2[l4] = 0;
		}
	}
	
	void fillsynth_risset_arpSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[0] = 1;
			iRec2[0] = (iVec1[1] + iRec2[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec2[0]));
			iVec1[1] = iVec1[0];
			iRec2[1] = iRec2[0];
		}
	}

};

static synth_risset_arpSIG0* newsynth_risset_arpSIG0() { return (synth_risset_arpSIG0*)new synth_risset_arpSIG0(); }
static void deletesynth_risset_arpSIG0(synth_risset_arpSIG0* dsp) { delete dsp; }

static float ftbl0synth_risset_arpSIG0[65536];
static float synth_risset_arp_faustpower2_f(float value) {
	return value * value;
}

class synth_risset_arp : public synth_risset_arp_dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	int iVec0[2];
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fRec1[2];
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fRec4[2];
	float fConst4;
	FAUSTFLOAT fHslider3;
	float fRec5[2];
	float fRec3[2];
	FAUSTFLOAT fHslider4;
	float fRec6[2];
	FAUSTFLOAT fHslider5;
	float fRec7[2];
	FAUSTFLOAT fHslider6;
	float fRec8[2];
	FAUSTFLOAT fHslider7;
	float fRec9[2];
	FAUSTFLOAT fHslider8;
	float fRec10[2];
	FAUSTFLOAT fHslider9;
	float fRec11[2];
	FAUSTFLOAT fHslider10;
	float fRec12[2];
	float fRec13[2];
	float fRec14[2];
	float fRec15[2];
	float fRec16[2];
	float fRec17[2];
	float fRec18[2];
	float fRec19[2];
	
 public:
	synth_risset_arp() {
	}
	
	void metadata(Meta* m) { 
		m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.17.1");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_risset_arp -scn synth_risset_arp_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "Oliver Larkin");
		m->declare("description", "Jean Claude Risset's Harmonic Arpeggio Effect");
		m->declare("filename", "synth_risset_arp.dsp");
		m->declare("licence", "GPL");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "synth.risset_arp");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.5.0");
		m->declare("version", "0.1");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		synth_risset_arpSIG0* sig0 = newsynth_risset_arpSIG0();
		sig0->instanceInitsynth_risset_arpSIG0(sample_rate);
		sig0->fillsynth_risset_arpSIG0(65536, ftbl0synth_risset_arpSIG0);
		deletesynth_risset_arpSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / fConst0;
		fConst4 = 0.441f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(1.0f);
		fHslider2 = FAUSTFLOAT(1e+02f);
		fHslider3 = FAUSTFLOAT(5.0f);
		fHslider4 = FAUSTFLOAT(1.0f);
		fHslider5 = FAUSTFLOAT(1.0f);
		fHslider6 = FAUSTFLOAT(1.0f);
		fHslider7 = FAUSTFLOAT(1.0f);
		fHslider8 = FAUSTFLOAT(1.0f);
		fHslider9 = FAUSTFLOAT(1.0f);
		fHslider10 = FAUSTFLOAT(1.0f);
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
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec3[l7] = 0.0f;
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
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec16[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec17[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec18[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec19[l21] = 0.0f;
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
	
	virtual synth_risset_arp* clone() {
		return new synth_risset_arp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.risset_arp");
		ui_interface->addHorizontalSlider("detune", &fHslider3, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("freq", &fHslider2, FAUSTFLOAT(1e+02f), FAUSTFLOAT(4e+01f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("harmonic1", &fHslider5, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic2", &fHslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic3", &fHslider6, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic4", &fHslider9, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic5", &fHslider7, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic6", &fHslider10, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic7", &fHslider8, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic8", &fHslider4, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("spread", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = fConst1 * float(fHslider0);
		float fSlow1 = fConst1 * float(fHslider1);
		float fSlow2 = fConst1 * float(fHslider2);
		float fSlow3 = fConst4 * float(fHslider3);
		float fSlow4 = fConst1 * float(fHslider4);
		float fSlow5 = fConst1 * float(fHslider5);
		float fSlow6 = fConst1 * float(fHslider6);
		float fSlow7 = fConst1 * float(fHslider7);
		float fSlow8 = fConst1 * float(fHslider8);
		float fSlow9 = fConst1 * float(fHslider9);
		float fSlow10 = fConst1 * float(fHslider10);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec0[0] = fSlow0 + fConst2 * fRec0[1];
			float fTemp0 = 0.21428572f * fRec0[0];
			fRec1[0] = fSlow1 + fConst2 * fRec1[1];
			int iTemp1 = 1 - iVec0[1];
			fRec4[0] = fSlow2 + fConst2 * fRec4[1];
			fRec5[0] = fSlow3 + fConst2 * fRec5[1];
			float fTemp2 = 2.0f * fRec5[0];
			float fTemp3 = ((iTemp1) ? 0.0f : fRec3[1] + fConst3 * (fRec4[0] - fTemp2));
			fRec3[0] = fTemp3 - std::floor(fTemp3);
			float fTemp4 = ftbl0synth_risset_arpSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec3[0]), 65535))];
			float fTemp5 = synth_risset_arp_faustpower2_f(fTemp4);
			float fTemp6 = 2.0f * fTemp5 + -1.0f;
			fRec6[0] = fSlow4 + fConst2 * fRec6[1];
			float fTemp7 = 1.0f - fTemp6;
			float fTemp8 = 1.0f - 4.0f * fTemp5 * fTemp7;
			float fTemp9 = fTemp8 - fTemp6;
			float fTemp10 = fTemp7 + fTemp9;
			float fTemp11 = 2.0f * fTemp5 * (2.0f * fTemp10 + 1.0f) + -1.0f;
			float fTemp12 = fTemp9 - fTemp11;
			fRec7[0] = fSlow5 + fConst2 * fRec7[1];
			fRec8[0] = fSlow6 + fConst2 * fRec8[1];
			fRec9[0] = fSlow7 + fConst2 * fRec9[1];
			fRec10[0] = fSlow8 + fConst2 * fRec10[1];
			fRec11[0] = fSlow9 + fConst2 * fRec11[1];
			fRec12[0] = fSlow10 + fConst2 * fRec12[1];
			float fTemp13 = fRec1[0] * fTemp6 + fRec6[0] * (1.0f - 4.0f * fTemp5 * (fTemp10 + fTemp12 + 1.0f)) + fTemp4 * (fRec7[0] + fRec8[0] * (2.0f * fTemp6 + -1.0f) + fRec9[0] * (2.0f * fTemp9 + 1.0f) + fRec10[0] * (-1.0f - 2.0f * fTemp12)) + fRec11[0] * fTemp8 + fRec12[0] * fTemp11 + 1.0f;
			float fTemp14 = 0.35714287f * fRec0[0];
			float fTemp15 = 3.0f * fRec5[0];
			float fTemp16 = ((iTemp1) ? 0.0f : fRec13[1] + fConst3 * (fRec4[0] - fTemp15));
			fRec13[0] = fTemp16 - std::floor(fTemp16);
			float fTemp17 = ftbl0synth_risset_arpSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec13[0]), 65535))];
			float fTemp18 = synth_risset_arp_faustpower2_f(fTemp17);
			float fTemp19 = 2.0f * fTemp18 + -1.0f;
			float fTemp20 = 1.0f - fTemp19;
			float fTemp21 = 1.0f - 4.0f * fTemp18 * fTemp20;
			float fTemp22 = fTemp21 - fTemp19;
			float fTemp23 = fTemp20 + fTemp22;
			float fTemp24 = 2.0f * fTemp18 * (2.0f * fTemp23 + 1.0f) + -1.0f;
			float fTemp25 = fTemp24 - fTemp22;
			float fTemp26 = fRec1[0] * fTemp19 + fRec11[0] * fTemp21 + fRec12[0] * fTemp24 + fTemp17 * (fRec7[0] + fRec8[0] * (2.0f * fTemp19 + -1.0f) + fRec9[0] * (2.0f * fTemp22 + 1.0f) + fRec10[0] * (2.0f * fTemp25 + -1.0f)) + fRec6[0] * (1.0f - 4.0f * fTemp18 * (fTemp23 + (1.0f - fTemp25))) + 1.0f;
			float fTemp27 = 0.071428575f * fRec0[0];
			float fTemp28 = ((iTemp1) ? 0.0f : fRec14[1] + fConst3 * (fRec4[0] - fRec5[0]));
			fRec14[0] = fTemp28 - std::floor(fTemp28);
			float fTemp29 = ftbl0synth_risset_arpSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec14[0]), 65535))];
			float fTemp30 = synth_risset_arp_faustpower2_f(fTemp29);
			float fTemp31 = 2.0f * fTemp30 + -1.0f;
			float fTemp32 = 1.0f - fTemp31;
			float fTemp33 = 1.0f - 4.0f * fTemp30 * fTemp32;
			float fTemp34 = fTemp33 - fTemp31;
			float fTemp35 = fTemp32 + fTemp34;
			float fTemp36 = -1.0f - 2.0f * fTemp30 * (-1.0f - 2.0f * fTemp35);
			float fTemp37 = fTemp34 - fTemp36;
			float fTemp38 = fRec1[0] * fTemp31 + fRec12[0] * fTemp36 + fTemp29 * (fRec7[0] + fRec8[0] * (2.0f * fTemp31 + -1.0f) + fRec9[0] * (2.0f * fTemp34 + 1.0f) + fRec10[0] * (-1.0f - 2.0f * fTemp37)) + fRec11[0] * fTemp33 + fRec6[0] * (1.0f - 4.0f * fTemp30 * (fTemp35 + fTemp37 + 1.0f)) + 1.0f;
			float fTemp39 = 0.21428572f * fRec0[0];
			float fTemp40 = ((iTemp1) ? 0.0f : fRec15[1] + fConst3 * (fRec4[0] + fRec5[0]));
			fRec15[0] = fTemp40 - std::floor(fTemp40);
			float fTemp41 = ftbl0synth_risset_arpSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec15[0]), 65535))];
			float fTemp42 = synth_risset_arp_faustpower2_f(fTemp41);
			float fTemp43 = 2.0f * fTemp42 + -1.0f;
			float fTemp44 = 1.0f - fTemp43;
			float fTemp45 = 1.0f - 4.0f * fTemp42 * fTemp44;
			float fTemp46 = fTemp45 - fTemp43;
			float fTemp47 = fTemp44 + fTemp46;
			float fTemp48 = 2.0f * fTemp42 * (2.0f * fTemp47 + 1.0f) + -1.0f;
			float fTemp49 = fTemp46 - fTemp48;
			float fTemp50 = fRec1[0] * fTemp43 + fRec6[0] * (1.0f - 4.0f * fTemp42 * (fTemp47 + fTemp49 + 1.0f)) + fRec11[0] * fTemp45 + fTemp41 * (fRec7[0] + fRec8[0] * (2.0f * fTemp43 + -1.0f) + fRec9[0] * (2.0f * fTemp46 + 1.0f) + fRec10[0] * (-1.0f - 2.0f * fTemp49)) + fRec12[0] * fTemp48 + 1.0f;
			float fTemp51 = 0.071428575f * fRec0[0];
			float fTemp52 = ((iTemp1) ? 0.0f : fRec16[1] + fConst3 * fRec4[0]);
			fRec16[0] = fTemp52 - std::floor(fTemp52);
			float fTemp53 = ftbl0synth_risset_arpSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec16[0]), 65535))];
			float fTemp54 = synth_risset_arp_faustpower2_f(fTemp53);
			float fTemp55 = 2.0f * fTemp54 + -1.0f;
			float fTemp56 = 1.0f - fTemp55;
			float fTemp57 = 1.0f - 4.0f * fTemp54 * fTemp56;
			float fTemp58 = fTemp57 - fTemp55;
			float fTemp59 = fTemp56 + fTemp58;
			float fTemp60 = 2.0f * fTemp54 * (2.0f * fTemp59 + 1.0f) + -1.0f;
			float fTemp61 = fTemp58 - fTemp60;
			float fTemp62 = fRec1[0] * fTemp55 + fRec12[0] * fTemp60 + fRec11[0] * fTemp57 + fTemp53 * (fRec7[0] + fRec8[0] * (2.0f * fTemp55 + -1.0f) + fRec9[0] * (2.0f * fTemp58 + 1.0f) + fRec10[0] * (-1.0f - 2.0f * fTemp61)) + fRec6[0] * (1.0f - 4.0f * fTemp54 * (fTemp59 + fTemp61 + 1.0f)) + 1.0f;
			float fTemp63 = 0.35714287f * fRec0[0];
			float fTemp64 = ((iTemp1) ? 0.0f : fRec17[1] + fConst3 * (fRec4[0] + fTemp2));
			fRec17[0] = fTemp64 - std::floor(fTemp64);
			float fTemp65 = ftbl0synth_risset_arpSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec17[0]), 65535))];
			float fTemp66 = synth_risset_arp_faustpower2_f(fTemp65);
			float fTemp67 = 2.0f * fTemp66 + -1.0f;
			float fTemp68 = 1.0f - fTemp67;
			float fTemp69 = 1.0f - 4.0f * fTemp66 * fTemp68;
			float fTemp70 = fTemp69 - fTemp67;
			float fTemp71 = fTemp68 + fTemp70;
			float fTemp72 = 2.0f * fTemp66 * (2.0f * fTemp71 + 1.0f) + -1.0f;
			float fTemp73 = fTemp70 - fTemp72;
			float fTemp74 = fRec1[0] * fTemp67 + fRec11[0] * fTemp69 + fRec12[0] * fTemp72 + fTemp65 * (fRec7[0] + fRec8[0] * (2.0f * fTemp67 + -1.0f) + fRec9[0] * (2.0f * fTemp70 + 1.0f) + fRec10[0] * (-1.0f - 2.0f * fTemp73)) + fRec6[0] * (1.0f - 4.0f * fTemp66 * (fTemp71 + fTemp73 + 1.0f)) + 1.0f;
			float fTemp75 = 1.0f - fRec0[0];
			float fTemp76 = ((iTemp1) ? 0.0f : fRec18[1] + fConst3 * (fRec4[0] - 4.0f * fRec5[0]));
			fRec18[0] = fTemp76 - std::floor(fTemp76);
			float fTemp77 = ftbl0synth_risset_arpSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec18[0]), 65535))];
			float fTemp78 = synth_risset_arp_faustpower2_f(fTemp77);
			float fTemp79 = 2.0f * fTemp78 + -1.0f;
			float fTemp80 = 1.0f - fTemp79;
			float fTemp81 = 1.0f - 4.0f * fTemp78 * fTemp80;
			float fTemp82 = fTemp81 - fTemp79;
			float fTemp83 = fTemp80 + fTemp82;
			float fTemp84 = -1.0f - 2.0f * fTemp78 * (-1.0f - 2.0f * fTemp83);
			float fTemp85 = fTemp82 - fTemp84;
			float fTemp86 = fRec1[0] * fTemp79 + fRec6[0] * (1.0f - 4.0f * fTemp78 * (fTemp83 + fTemp85 + 1.0f)) + fRec11[0] * fTemp81 + fTemp77 * (fRec7[0] + fRec8[0] * (2.0f * fTemp79 + -1.0f) + fRec9[0] * (2.0f * fTemp82 + 1.0f) + fRec10[0] * (-1.0f - 2.0f * fTemp85)) + fRec12[0] * fTemp84 + 1.0f;
			float fTemp87 = fRec0[0] + 1.0f;
			float fTemp88 = ((iTemp1) ? 0.0f : fRec19[1] + fConst3 * (fRec4[0] + fTemp15));
			fRec19[0] = fTemp88 - std::floor(fTemp88);
			float fTemp89 = ftbl0synth_risset_arpSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec19[0]), 65535))];
			float fTemp90 = synth_risset_arp_faustpower2_f(fTemp89);
			float fTemp91 = 2.0f * fTemp90 + -1.0f;
			float fTemp92 = 1.0f - fTemp91;
			float fTemp93 = 1.0f - 4.0f * fTemp90 * fTemp92;
			float fTemp94 = fTemp93 - fTemp91;
			float fTemp95 = fTemp92 + fTemp94;
			float fTemp96 = 2.0f * fTemp90 * (2.0f * fTemp95 + 1.0f) + -1.0f;
			float fTemp97 = fTemp96 - fTemp94;
			float fTemp98 = fRec1[0] * fTemp91 + fRec11[0] * fTemp93 + fRec12[0] * fTemp96 + fTemp89 * (fRec7[0] + fRec8[0] * (2.0f * fTemp91 + -1.0f) + fRec9[0] * (2.0f * fTemp94 + 1.0f) + fRec10[0] * (2.0f * fTemp97 + -1.0f)) + fRec6[0] * (1.0f - 4.0f * fTemp90 * (fTemp95 + (1.0f - fTemp97))) + 1.0f;
			output0[i0] = FAUSTFLOAT(0.05f * ((0.5f - fTemp0) * fTemp13 + (0.5f - fTemp14) * fTemp26 + (0.5f - fTemp27) * fTemp38 + (fTemp39 + 0.5f) * fTemp50 + (fTemp51 + 0.5f) * fTemp62 + (fTemp63 + 0.5f) * fTemp74 + 0.5f * (fTemp75 * fTemp86 + fTemp87 * fTemp98)));
			output1[i0] = FAUSTFLOAT(0.05f * ((1.0f - 0.5f * fTemp75) * fTemp86 + (fTemp14 + 0.5f) * fTemp26 + (fTemp0 + 0.5f) * fTemp13 + (fTemp27 + 0.5f) * fTemp38 + (0.5f - fTemp51) * fTemp62 + (0.5f - fTemp39) * fTemp50 + (0.5f - fTemp63) * fTemp74 + (1.0f - 0.5f * fTemp87) * fTemp98));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec3[1] = fRec3[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
		}
	}

};
#endif

class faust_synth_risset_arp_tilde : public FaustExternal<synth_risset_arp> {
public:
    faust_synth_risset_arp_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_risset_arp))
    {
    }
};

#endif
