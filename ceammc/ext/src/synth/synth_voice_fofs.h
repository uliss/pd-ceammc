/* ------------------------------------------------------------
name: "synth.voice_fofs"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_voice_fofs -scn synth_voice_fofs_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_voice_fofs_H__
#define  __synth_voice_fofs_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_voice_fofs_dsp.h ********************************
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

class FAUST_API synth_voice_fofs_dsp {

    public:

        synth_voice_fofs_dsp() {}
        virtual ~synth_voice_fofs_dsp() {}

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
        virtual synth_voice_fofs_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_voice_fofs_dsp {

    protected:

        synth_voice_fofs_dsp* fDSP;

    public:

        decorator_dsp(synth_voice_fofs_dsp* synth_voice_fofs_dsp = nullptr):fDSP(synth_voice_fofs_dsp) {}
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
    
        virtual synth_voice_fofs_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_voice_fofs_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_voice_fofs_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_voice_fofs : public synth_voice_fofs_dsp {
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
#define FAUSTCLASS synth_voice_fofs
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

class synth_voice_fofsSIG0 {
	
  private:
	
	int iVec0[2];
	int iRec4[2];
	
  public:
	
	int getNumInputssynth_voice_fofsSIG0() {
		return 0;
	}
	int getNumOutputssynth_voice_fofsSIG0() {
		return 1;
	}
	
	void instanceInitsynth_voice_fofsSIG0(int sample_rate) {
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iVec0[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec4[l3] = 0;
		}
	}
	
	void fillsynth_voice_fofsSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec4[0] = (iVec0[1] + iRec4[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec4[0]));
			iVec0[1] = iVec0[0];
			iRec4[1] = iRec4[0];
		}
	}

};

static synth_voice_fofsSIG0* newsynth_voice_fofsSIG0() { return (synth_voice_fofsSIG0*)new synth_voice_fofsSIG0(); }
static void deletesynth_voice_fofsSIG0(synth_voice_fofsSIG0* dsp) { delete dsp; }

static float ftbl0synth_voice_fofsSIG0[65536];

class synth_voice_fofs : public synth_voice_fofs_dsp {
	
 private:
	
	int fSampleRate;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fVslider0;
	float fConst3;
	float fRec3[2];
	FAUSTFLOAT fHslider0;
	float fRec2[2];
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fRec5[2];
	float fRec1[2];
	float fVec1[2];
	float fConst4;
	float fConst5;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fRec7[2];
	FAUSTFLOAT fHslider5;
	float fRec6[2];
	float fRec8[2];
	float fRec0[3];
	float fRec9[2];
	float fRec11[2];
	float fRec12[2];
	float fRec10[3];
	float fRec13[2];
	float fRec15[2];
	float fRec16[2];
	float fRec14[3];
	float fRec17[2];
	float fRec19[2];
	float fRec20[2];
	float fRec18[3];
	float fRec21[2];
	float fRec23[2];
	float fRec24[2];
	float fRec22[3];
	float fRec25[2];
	float fConst6;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fCheckbox0;
	float fVec2[2];
	float fRec26[2];
	FAUSTFLOAT fHslider7;
	int iRec27[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/listInterp:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_voice_fofs -scn synth_voice_fofs_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("envelopes.lib/asr:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.2");
		m->declare("filename", "synth_voice_fofs.dsp");
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
		m->declare("name", "synth.voice_fofs");
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
		m->declare("physmodels.lib/fofSmooth:author", "Mike Olsen");
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
		synth_voice_fofsSIG0* sig0 = newsynth_voice_fofsSIG0();
		sig0->instanceInitsynth_voice_fofsSIG0(sample_rate);
		sig0->fillsynth_voice_fofsSIG0(65536, ftbl0synth_voice_fofsSIG0);
		deletesynth_voice_fofsSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = std::exp(0.0f - 1e+02f / fConst0);
		fConst5 = 1.0f - fConst4;
		fConst6 = 0.001f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(4.4e+02f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(6.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(1e+01f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(3e+01f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(5e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec3[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec2[l1] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec5[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec1[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fVec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec7[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec0[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec11[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec12[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec10[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec15[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec14[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec17[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec19[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec20[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec18[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec21[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec23[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec24[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec22[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec25[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fVec2[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec26[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			iRec27[l30] = 0;
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
	
	virtual synth_voice_fofs* clone() {
		return new synth_voice_fofs();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.voice_fofs");
		ui_interface->declare(&fHslider6, "unit", "ms");
		ui_interface->addHorizontalSlider("attack", &fHslider6, FAUSTFLOAT(3e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("freq", &fVslider0, FAUSTFLOAT(4.4e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("fvowel", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->addHorizontalSlider("ivoice", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider0, "unit", "ms");
		ui_interface->addHorizontalSlider("portamento", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4e+03f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider7, "unit", "ms");
		ui_interface->addHorizontalSlider("release", &fHslider7, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider2, "unit", "Hz");
		ui_interface->addHorizontalSlider("vibr_freq", &fHslider2, FAUSTFLOAT(6.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("vibr_gain", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider4, "unit", "ms");
		ui_interface->addHorizontalSlider("wsmooth", &fHslider4, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = fConst2 * float(fVslider0);
		float fSlow1 = 0.001f * float(fHslider0);
		int iSlow2 = std::fabs(fSlow1) < 1.1920929e-07f;
		float fSlow3 = ((iSlow2) ? 0.0f : std::exp(0.0f - fConst1 / ((iSlow2) ? 1.0f : fSlow1)));
		float fSlow4 = 1.0f - fSlow3;
		float fSlow5 = 0.1f * float(fHslider1);
		float fSlow6 = fConst1 * float(fHslider2);
		float fSlow7 = 0.001f * float(fHslider4);
		int iSlow8 = std::fabs(fSlow7) < 1.1920929e-07f;
		float fSlow9 = ((iSlow8) ? 0.0f : std::exp(0.0f - fConst1 / ((iSlow8) ? 1.0f : fSlow7)));
		float fSlow10 = float(fHslider3) * (1.0f - fSlow9);
		int iSlow11 = int(float(fHslider5));
		int iSlow12 = ((iSlow11 == 0) ? 1 : ((iSlow11 == 3) ? 1 : 0));
		float fSlow13 = float(5 * iSlow12);
		int iSlow14 = iSlow12 >= 1;
		float fSlow15 = ((iSlow14) ? 174.61f : 82.41f);
		float fSlow16 = ((iSlow14) ? 1046.5f : 523.25f);
		float fSlow17 = fSlow16 - fSlow15;
		float fSlow18 = float(5 * iSlow11);
		int iSlow19 = iSlow12 == 0;
		int iSlow20 = iSlow11 != 2;
		float fSlow21 = 1.0f / std::max<float>(1.0f, fConst6 * float(fHslider6));
		float fSlow22 = float(fCheckbox0);
		float fSlow23 = 1.0f / std::max<float>(1.0f, fConst6 * float(fHslider7));
		int iSlow24 = fSlow22 == 0.0f;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec3[0] = fSlow0 + fConst3 * fRec3[1];
			fRec2[0] = fRec3[0] * fSlow4 + fSlow3 * fRec2[1];
			fRec5[0] = fSlow6 + (fRec5[1] - std::floor(fSlow6 + fRec5[1]));
			float fTemp0 = fRec2[0] * (fSlow5 * ftbl0synth_voice_fofsSIG0[int(65536.0f * fRec5[0])] + 1.0f);
			float fTemp1 = fRec1[1] + fConst1 * fTemp0;
			fRec1[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = float((fRec1[0] - fRec1[1]) < 0.0f);
			fVec1[0] = fTemp2;
			fRec7[0] = fSlow10 + fSlow9 * fRec7[1];
			float fTemp3 = fRec7[0] + fSlow13;
			int iTemp4 = fTemp3 >= 5.0f;
			int iTemp5 = fTemp3 >= 3.0f;
			int iTemp6 = fTemp3 >= 2.0f;
			int iTemp7 = fTemp3 >= 1.0f;
			int iTemp8 = fTemp3 >= 4.0f;
			int iTemp9 = fTemp3 >= 8.0f;
			int iTemp10 = fTemp3 >= 7.0f;
			int iTemp11 = fTemp3 >= 6.0f;
			float fTemp12 = ((iTemp4) ? ((iTemp9) ? 2.0f : ((iTemp10) ? 3.0f : ((iTemp11) ? 3.0f : 2.0f))) : ((iTemp5) ? ((iTemp8) ? 1.5f : 1.0f) : ((iTemp6) ? 1.25f : ((iTemp7) ? 1.25f : 1.0f))));
			float fTemp13 = fTemp12 + (((iTemp4) ? ((iTemp9) ? 12.0f : ((iTemp10) ? 12.0f : ((iTemp11) ? 12.0f : 15.0f))) : ((iTemp5) ? ((iTemp8) ? 4.0f : 1e+01f) : ((iTemp6) ? 2.5f : ((iTemp7) ? 2.5f : 1e+01f)))) - fTemp12) * ((fTemp0 <= fSlow15) ? 0.0f : ((fTemp0 >= fSlow16) ? 1.0f : (fTemp0 - fSlow15) / fSlow17));
			float fTemp14 = fSlow18 + fRec7[0];
			int iTemp15 = fTemp14 < 23.0f;
			int iTemp16 = fTemp14 < 24.0f;
			float fTemp17 = fSlow18 + fRec7[0] + -23.0f;
			int iTemp18 = fTemp14 < 22.0f;
			float fTemp19 = fSlow18 + fRec7[0] + -22.0f;
			int iTemp20 = fTemp14 < 21.0f;
			float fTemp21 = fSlow18 + fRec7[0] + -21.0f;
			int iTemp22 = fTemp14 < 2e+01f;
			float fTemp23 = fSlow18 + fRec7[0] + -2e+01f;
			float fTemp24 = 2e+01f * fTemp23;
			int iTemp25 = fTemp14 < 19.0f;
			int iTemp26 = fTemp14 < 18.0f;
			float fTemp27 = fSlow18 + fRec7[0] + -18.0f;
			int iTemp28 = fTemp14 < 17.0f;
			float fTemp29 = fSlow18 + fRec7[0] + -17.0f;
			int iTemp30 = fTemp14 < 16.0f;
			int iTemp31 = fTemp14 < 15.0f;
			float fTemp32 = fSlow18 + fRec7[0] + -15.0f;
			float fTemp33 = 2e+01f * fTemp32;
			int iTemp34 = fTemp14 < 14.0f;
			float fTemp35 = fSlow18 + fRec7[0] + -14.0f;
			int iTemp36 = fTemp14 < 13.0f;
			int iTemp37 = fTemp14 < 12.0f;
			int iTemp38 = fTemp14 < 11.0f;
			float fTemp39 = fSlow18 + fRec7[0] + -11.0f;
			int iTemp40 = fTemp14 < 1e+01f;
			float fTemp41 = fSlow18 + fRec7[0] + -1e+01f;
			float fTemp42 = 1e+01f * fTemp41;
			int iTemp43 = fTemp14 < 9.0f;
			float fTemp44 = fSlow18 + fRec7[0] + -9.0f;
			int iTemp45 = fTemp14 < 8.0f;
			int iTemp46 = fTemp14 < 7.0f;
			float fTemp47 = fSlow18 + fRec7[0] + -7.0f;
			int iTemp48 = fTemp14 < 6.0f;
			float fTemp49 = fSlow18 + fRec7[0] + -6.0f;
			int iTemp50 = fTemp14 < 5.0f;
			float fTemp51 = fSlow18 + fRec7[0] + -5.0f;
			int iTemp52 = fTemp14 < 4.0f;
			float fTemp53 = fSlow18 + fRec7[0] + -4.0f;
			float fTemp54 = 1e+01f * fTemp53;
			int iTemp55 = fTemp14 < 3.0f;
			float fTemp56 = fSlow18 + fRec7[0] + -3.0f;
			float fTemp57 = 2e+01f * fTemp56;
			int iTemp58 = fTemp14 < 2.0f;
			float fTemp59 = fSlow18 + fRec7[0] + -2.0f;
			float fTemp60 = 2e+01f * fTemp59;
			int iTemp61 = fTemp14 < 1.0f;
			float fTemp62 = fSlow18 + fRec7[0] + -1.0f;
			int iTemp63 = fTemp14 < 0.0f;
			float fTemp64 = 2e+01f * fTemp14;
			float fTemp65 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 8e+01f : ((iTemp61) ? 8e+01f - fTemp64 : 6e+01f)) : ((iTemp58) ? 6e+01f - 1e+01f * fTemp62 : 5e+01f)) : ((iTemp55) ? fTemp60 + 5e+01f : 7e+01f)) : ((iTemp52) ? 7e+01f - fTemp57 : 5e+01f)) : ((iTemp50) ? fTemp54 + 5e+01f : 6e+01f)) : ((iTemp48) ? 6e+01f - 2e+01f * fTemp51 : 4e+01f)) : ((iTemp46) ? 2e+01f * fTemp49 + 4e+01f : 6e+01f)) : ((iTemp45) ? 6e+01f - 2e+01f * fTemp47 : 4e+01f)) : 4e+01f) : ((iTemp40) ? 4e+01f * (fTemp44 + 1.0f) : 8e+01f)) : ((iTemp38) ? 8e+01f - fTemp42 : 7e+01f)) : ((iTemp37) ? 7e+01f - 3e+01f * fTemp39 : 4e+01f)) : 4e+01f) : 4e+01f) : ((iTemp31) ? 4e+01f * (fTemp35 + 1.0f) : 8e+01f)) : ((iTemp30) ? 8e+01f - fTemp33 : 6e+01f)) : 6e+01f) : ((iTemp26) ? 6e+01f - 2e+01f * fTemp29 : 4e+01f)) : ((iTemp25) ? 1e+01f * fTemp27 + 4e+01f : 5e+01f)) : 5e+01f) : ((iTemp20) ? fTemp24 + 5e+01f : 7e+01f)) : ((iTemp18) ? 7e+01f - 3e+01f * fTemp21 : 4e+01f)) : ((iTemp15) ? 3e+01f * fTemp19 + 4e+01f : 7e+01f)) : ((iTemp16) ? 7e+01f - 3e+01f * fTemp17 : 4e+01f));
			fRec6[0] = fConst5 * fTemp13 * fTemp65 + fConst4 * fRec6[1];
			float fTemp66 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec6[0]));
			fRec8[0] = fConst5 * fTemp65 + fConst4 * fRec8[1];
			float fTemp67 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec8[0]));
			fRec0[0] = fVec1[1] - (fRec0[1] * (0.0f - (fTemp66 + fTemp67)) + fTemp66 * fTemp67 * fRec0[2]);
			float fTemp68 = 0.00084f * (1e+03f - fTemp0) + 0.8f;
			float fTemp69 = 0.0036666666f * (4e+02f - fTemp0) + 3.0f;
			int iTemp70 = int(fTemp2);
			float fTemp71 = fSlow18 + fRec7[0] + -19.0f;
			float fTemp72 = fSlow18 + fRec7[0] + -16.0f;
			float fTemp73 = 8e+01f * fTemp72;
			float fTemp74 = fSlow18 + fRec7[0] + -13.0f;
			float fTemp75 = fSlow18 + fRec7[0] + -12.0f;
			float fTemp76 = fSlow18 + fRec7[0] + -8.0f;
			float fTemp77 = 5e+01f * fTemp76;
			float fTemp78 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 8e+02f : ((iTemp61) ? 8e+02f - 4e+02f * fTemp14 : 4e+02f)) : ((iTemp58) ? 4e+02f - 5e+01f * fTemp62 : 3.5e+02f)) : ((iTemp55) ? 1e+02f * fTemp59 + 3.5e+02f : 4.5e+02f)) : ((iTemp52) ? 4.5e+02f - 125.0f * fTemp56 : 325.0f)) : ((iTemp50) ? 275.0f * fTemp53 + 325.0f : 6e+02f)) : ((iTemp48) ? 6e+02f - 2e+02f * fTemp51 : 4e+02f)) : ((iTemp46) ? 4e+02f - 1.5e+02f * fTemp49 : 2.5e+02f)) : ((iTemp45) ? 1.5e+02f * fTemp47 + 2.5e+02f : 4e+02f)) : ((iTemp43) ? 4e+02f - fTemp77 : 3.5e+02f)) : ((iTemp40) ? 3.1e+02f * fTemp44 + 3.5e+02f : 6.6e+02f)) : ((iTemp38) ? 6.6e+02f - 2.2e+02f * fTemp41 : 4.4e+02f)) : ((iTemp37) ? 4.4e+02f - 1.7e+02f * fTemp39 : 2.7e+02f)) : ((iTemp36) ? 1.6e+02f * fTemp75 + 2.7e+02f : 4.3e+02f)) : ((iTemp34) ? 4.3e+02f - 6e+01f * fTemp74 : 3.7e+02f)) : ((iTemp31) ? 4.3e+02f * fTemp35 + 3.7e+02f : 8e+02f)) : ((iTemp30) ? 8e+02f - 4.5e+02f * fTemp32 : 3.5e+02f)) : ((iTemp28) ? 3.5e+02f - fTemp73 : 2.7e+02f)) : ((iTemp26) ? 1.8e+02f * fTemp29 + 2.7e+02f : 4.5e+02f)) : ((iTemp25) ? 4.5e+02f - 125.0f * fTemp27 : 325.0f)) : ((iTemp22) ? 325.0f * (fTemp71 + 1.0f) : 6.5e+02f)) : ((iTemp20) ? 6.5e+02f - 2.5e+02f * fTemp23 : 4e+02f)) : ((iTemp18) ? 4e+02f - 1.1e+02f * fTemp21 : 2.9e+02f)) : ((iTemp15) ? 1.1e+02f * fTemp19 + 2.9e+02f : 4e+02f)) : ((iTemp16) ? 4e+02f - 5e+01f * fTemp17 : 3.5e+02f));
			float fTemp79 = ((iTemp70) ? 0.0f : fRec9[1] + fConst1 * ((fTemp78 <= fTemp0) ? fTemp0 : fTemp78));
			fRec9[0] = fTemp79 - std::floor(fTemp79);
			float fTemp80 = 1e+01f * fTemp19;
			float fTemp81 = 1e+01f * fTemp23;
			float fTemp82 = 1e+01f * fTemp44;
			float fTemp83 = 1e+01f * fTemp51;
			float fTemp84 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 9e+01f : ((iTemp61) ? 9e+01f - 1e+01f * fTemp14 : 8e+01f)) : ((iTemp58) ? 2e+01f * fTemp62 + 8e+01f : 1e+02f)) : ((iTemp55) ? 1e+02f - fTemp60 : 8e+01f)) : ((iTemp52) ? 8e+01f - fTemp57 : 6e+01f)) : ((iTemp50) ? fTemp54 + 6e+01f : 7e+01f)) : ((iTemp48) ? fTemp83 + 7e+01f : 8e+01f)) : ((iTemp46) ? 1e+01f * fTemp49 + 8e+01f : 9e+01f)) : ((iTemp45) ? 9e+01f - 1e+01f * fTemp47 : 8e+01f)) : 8e+01f) : ((iTemp40) ? fTemp82 + 8e+01f : 9e+01f)) : ((iTemp38) ? 9e+01f - fTemp42 : 8e+01f)) : ((iTemp37) ? 1e+01f * fTemp39 + 8e+01f : 9e+01f)) : ((iTemp36) ? 9e+01f - 1e+01f * fTemp75 : 8e+01f)) : ((iTemp34) ? 8e+01f - 2e+01f * fTemp74 : 6e+01f)) : ((iTemp31) ? 3e+01f * fTemp35 + 6e+01f : 9e+01f)) : ((iTemp30) ? 1e+01f * fTemp32 + 9e+01f : 1e+02f)) : ((iTemp28) ? 1e+02f - 1e+01f * fTemp72 : 9e+01f)) : ((iTemp26) ? 9e+01f - 1e+01f * fTemp29 : 8e+01f)) : ((iTemp25) ? 8e+01f - 2e+01f * fTemp27 : 6e+01f)) : ((iTemp22) ? 3e+01f * fTemp71 + 6e+01f : 9e+01f)) : ((iTemp20) ? 9e+01f - fTemp81 : 8e+01f)) : ((iTemp18) ? 1e+01f * fTemp21 + 8e+01f : 9e+01f)) : ((iTemp15) ? 9e+01f - fTemp80 : 8e+01f)) : ((iTemp16) ? 8e+01f - 2e+01f * fTemp17 : 6e+01f));
			fRec11[0] = fConst5 * fTemp13 * fTemp84 + fConst4 * fRec11[1];
			float fTemp85 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec11[0]));
			fRec12[0] = fConst5 * fTemp84 + fConst4 * fRec12[1];
			float fTemp86 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec12[0]));
			fRec10[0] = fVec1[1] - (fRec10[1] * (0.0f - (fTemp85 + fTemp86)) + fTemp85 * fTemp86 * fRec10[2]);
			float fTemp87 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 0.630957f : ((iTemp61) ? 0.630957f - 0.567861f * fTemp14 : 0.063096f)) : ((iTemp58) ? 0.036904f * fTemp62 + 0.063096f : 0.1f)) : ((iTemp55) ? 0.254813f * fTemp59 + 0.1f : 0.354813f)) : ((iTemp52) ? 0.354813f - 0.103624f * fTemp56 : 0.251189f)) : ((iTemp50) ? 0.195495f * fTemp53 + 0.251189f : 0.446684f)) : ((iTemp48) ? 0.446684f - 0.195495f * fTemp51 : 0.251189f)) : ((iTemp46) ? 0.251189f - 0.219566f * fTemp49 : 0.031623f)) : ((iTemp45) ? 0.250215f * fTemp47 + 0.031623f : 0.281838f)) : ((iTemp43) ? 0.281838f - 0.181838f * fTemp76 : 0.1f)) : ((iTemp40) ? 0.401187f * fTemp44 + 0.1f : 0.501187f)) : ((iTemp38) ? 0.501187f - 0.301661f * fTemp41 : 0.199526f)) : ((iTemp37) ? 0.199526f - 0.13643f * fTemp39 : 0.063096f)) : ((iTemp36) ? 0.253132f * fTemp75 + 0.063096f : 0.316228f)) : ((iTemp34) ? 0.316228f - 0.216228f * fTemp74 : 0.1f)) : ((iTemp31) ? 0.401187f * fTemp35 + 0.1f : 0.501187f)) : ((iTemp30) ? 0.501187f - 0.401187f * fTemp32 : 0.1f)) : ((iTemp28) ? 0.151189f * fTemp72 + 0.1f : 0.251189f)) : ((iTemp26) ? 0.030649f * fTemp29 + 0.251189f : 0.281838f)) : ((iTemp25) ? 0.281838f - 0.123349f * fTemp27 : 0.158489f)) : ((iTemp22) ? 0.342698f * fTemp71 + 0.158489f : 0.501187f)) : ((iTemp20) ? 0.501187f - 0.301661f * fTemp23 : 0.199526f)) : ((iTemp18) ? 0.199526f - 0.021698f * fTemp21 : 0.177828f)) : ((iTemp15) ? 0.1384f * fTemp19 + 0.177828f : 0.316228f)) : ((iTemp16) ? 0.316228f - 0.216228f * fTemp17 : 0.1f));
			float fTemp88 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 1.15e+03f : ((iTemp61) ? 4.5e+02f * fTemp14 + 1.15e+03f : 1.6e+03f)) : ((iTemp58) ? 1e+02f * fTemp62 + 1.6e+03f : 1.7e+03f)) : ((iTemp55) ? 1.7e+03f - 9e+02f * fTemp59 : 8e+02f)) : ((iTemp52) ? 8e+02f - 1e+02f * fTemp56 : 7e+02f)) : ((iTemp50) ? 3.4e+02f * fTemp53 + 7e+02f : 1.04e+03f)) : ((iTemp48) ? 5.8e+02f * fTemp51 + 1.04e+03f : 1.62e+03f)) : ((iTemp46) ? 1.3e+02f * fTemp49 + 1.62e+03f : 1.75e+03f)) : ((iTemp45) ? 1.75e+03f - 1e+03f * fTemp47 : 7.5e+02f)) : ((iTemp43) ? 7.5e+02f - 1.5e+02f * fTemp76 : 6e+02f)) : ((iTemp40) ? 5.2e+02f * fTemp44 + 6e+02f : 1.12e+03f)) : ((iTemp38) ? 6.8e+02f * fTemp41 + 1.12e+03f : 1.8e+03f)) : ((iTemp37) ? 5e+01f * fTemp39 + 1.8e+03f : 1.85e+03f)) : ((iTemp36) ? 1.85e+03f - 1.03e+03f * fTemp75 : 8.2e+02f)) : ((iTemp34) ? 8.2e+02f - 1.9e+02f * fTemp74 : 6.3e+02f)) : ((iTemp31) ? 5.2e+02f * fTemp35 + 6.3e+02f : 1.15e+03f)) : ((iTemp30) ? 8.5e+02f * fTemp32 + 1.15e+03f : 2e+03f)) : ((iTemp28) ? 1.4e+02f * fTemp72 + 2e+03f : 2.14e+03f)) : ((iTemp26) ? 2.14e+03f - 1.34e+03f * fTemp29 : 8e+02f)) : ((iTemp25) ? 8e+02f - 1e+02f * fTemp27 : 7e+02f)) : ((iTemp22) ? 3.8e+02f * fTemp71 + 7e+02f : 1.08e+03f)) : ((iTemp20) ? 6.2e+02f * fTemp23 + 1.08e+03f : 1.7e+03f)) : ((iTemp18) ? 1.7e+02f * fTemp21 + 1.7e+03f : 1.87e+03f)) : ((iTemp15) ? 1.87e+03f - 1.07e+03f * fTemp19 : 8e+02f)) : ((iTemp16) ? 8e+02f - 2e+02f * fTemp17 : 6e+02f));
			float fTemp89 = 2.0f * fTemp0 + 3e+01f;
			float fTemp90 = ((iTemp70) ? 0.0f : fRec13[1] + fConst1 * ((iSlow20) ? (((fTemp88 >= 1.3e+03f) & (fTemp0 >= 2e+02f)) ? fTemp88 - 0.00095238094f * (fTemp0 + -2e+02f) * (fTemp88 + -1.3e+03f) : ((fTemp88 <= fTemp89) ? fTemp89 : fTemp88)) : fTemp88));
			fRec13[0] = fTemp90 - std::floor(fTemp90);
			float fTemp91 = 5e+01f * fTemp71;
			float fTemp92 = 2e+01f * fTemp35;
			float fTemp93 = 2e+01f * fTemp41;
			float fTemp94 = 2e+01f * fTemp44;
			float fTemp95 = 6e+01f * fTemp53;
			float fTemp96 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? 1.2e+02f : ((iTemp55) ? 1.2e+02f - fTemp60 : 1e+02f)) : ((iTemp52) ? 7e+01f * fTemp56 + 1e+02f : 1.7e+02f)) : ((iTemp50) ? 1.7e+02f - fTemp95 : 1.1e+02f)) : ((iTemp48) ? 1.1e+02f - fTemp83 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp40) ? fTemp94 + 1e+02f : 1.2e+02f)) : ((iTemp38) ? 1.2e+02f - fTemp93 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp31) ? fTemp92 + 1e+02f : 1.2e+02f)) : 1.2e+02f) : ((iTemp28) ? 1.2e+02f - 2e+01f * fTemp72 : 1e+02f)) : 1e+02f) : ((iTemp25) ? 7e+01f * fTemp27 + 1e+02f : 1.7e+02f)) : ((iTemp22) ? 1.7e+02f - fTemp91 : 1.2e+02f)) : ((iTemp20) ? 1.2e+02f - fTemp24 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f);
			fRec15[0] = fConst5 * fTemp13 * fTemp96 + fConst4 * fRec15[1];
			float fTemp97 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec15[0]));
			fRec16[0] = fConst5 * fTemp96 + fConst4 * fRec16[1];
			float fTemp98 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec16[0]));
			fRec14[0] = fVec1[1] - (fRec14[1] * (0.0f - (fTemp97 + fTemp98)) + fTemp97 * fTemp98 * fRec14[2]);
			float fTemp99 = ((iTemp26) ? 0.029314f * fTemp29 + 0.050119f : 0.079433f);
			float fTemp100 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 0.1f : ((iTemp61) ? 0.1f - 0.068377f * fTemp14 : 0.031623f)) : 0.031623f) : ((iTemp55) ? 0.126866f * fTemp59 + 0.031623f : 0.158489f)) : ((iTemp52) ? 0.158489f - 0.126866f * fTemp56 : 0.031623f)) : ((iTemp50) ? 0.32319f * fTemp53 + 0.031623f : 0.354813f)) : 0.354813f) : ((iTemp46) ? 0.354813f - 0.196324f * fTemp49 : 0.158489f)) : ((iTemp45) ? 0.158489f - 0.069364f * fTemp47 : 0.089125f)) : ((iTemp43) ? 0.089125f - 0.064006f * fTemp76 : 0.025119f)) : ((iTemp40) ? 0.045676f * fTemp44 + 0.025119f : 0.070795f)) : ((iTemp38) ? 0.055098f * fTemp41 + 0.070795f : 0.125893f)) : ((iTemp37) ? 0.125893f - 0.062797f * fTemp39 : 0.063096f)) : ((iTemp36) ? 0.063096f - 0.012977f * fTemp75 : 0.050119f)) : ((iTemp34) ? 0.020676f * fTemp74 + 0.050119f : 0.070795f)) : ((iTemp31) ? 0.070795f - 0.045676f * fTemp35 : 0.025119f)) : ((iTemp30) ? 0.152709f * fTemp32 + 0.025119f : 0.177828f)) : ((iTemp28) ? 0.177828f - 0.127709f * fTemp72 : 0.050119f)) : fTemp99) : ((iTemp25) ? 0.079433f - 0.06165f * fTemp27 : 0.017783f)) : ((iTemp22) ? 0.428901f * fTemp71 + 0.017783f : 0.446684f)) : ((iTemp20) ? 0.446684f - 0.195495f * fTemp23 : 0.251189f)) : ((iTemp18) ? 0.251189f - 0.125296f * fTemp21 : 0.125893f)) : ((iTemp15) ? 0.125296f * fTemp19 + 0.125893f : 0.251189f)) : ((iTemp16) ? 0.251189f - 0.109935f * fTemp17 : 0.141254f));
			float fTemp101 = 1e+02f * fTemp17;
			float fTemp102 = 5e+01f * fTemp41;
			float fTemp103 = ((iTemp70) ? 0.0f : fRec17[1] + fConst1 * ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 2.8e+03f : ((iTemp61) ? 2.8e+03f - 1e+02f * fTemp14 : 2.7e+03f)) : 2.7e+03f) : ((iTemp55) ? 1.3e+02f * fTemp59 + 2.7e+03f : 2.83e+03f)) : ((iTemp52) ? 2.83e+03f - 3e+02f * fTemp56 : 2.53e+03f)) : ((iTemp50) ? 2.53e+03f - 2.8e+02f * fTemp53 : 2.25e+03f)) : ((iTemp48) ? 1.5e+02f * fTemp51 + 2.25e+03f : 2.4e+03f)) : ((iTemp46) ? 2e+02f * fTemp49 + 2.4e+03f : 2.6e+03f)) : ((iTemp45) ? 2.6e+03f - 2e+02f * fTemp47 : 2.4e+03f)) : 2.4e+03f) : ((iTemp40) ? 3.5e+02f * fTemp44 + 2.4e+03f : 2.75e+03f)) : ((iTemp38) ? 2.75e+03f - fTemp102 : 2.7e+03f)) : ((iTemp37) ? 2e+02f * fTemp39 + 2.7e+03f : 2.9e+03f)) : ((iTemp36) ? 2.9e+03f - 2e+02f * fTemp75 : 2.7e+03f)) : ((iTemp34) ? 5e+01f * fTemp74 + 2.7e+03f : 2.75e+03f)) : ((iTemp31) ? 1.5e+02f * fTemp35 + 2.75e+03f : 2.9e+03f)) : ((iTemp30) ? 2.9e+03f - 1e+02f * fTemp32 : 2.8e+03f)) : ((iTemp28) ? 1.5e+02f * fTemp72 + 2.8e+03f : 2.95e+03f)) : ((iTemp26) ? 2.95e+03f - 1.2e+02f * fTemp29 : 2.83e+03f)) : ((iTemp25) ? 2.83e+03f - 1.3e+02f * fTemp27 : 2.7e+03f)) : ((iTemp22) ? 2.7e+03f - fTemp91 : 2.65e+03f)) : ((iTemp20) ? 2.65e+03f - 5e+01f * fTemp23 : 2.6e+03f)) : ((iTemp18) ? 2e+02f * fTemp21 + 2.6e+03f : 2.8e+03f)) : ((iTemp15) ? 2.8e+03f - 2e+02f * fTemp19 : 2.6e+03f)) : ((iTemp16) ? fTemp101 + 2.6e+03f : 2.7e+03f)));
			fRec17[0] = fTemp103 - std::floor(fTemp103);
			float fTemp104 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 1.3e+02f : ((iTemp61) ? fTemp64 + 1.3e+02f : 1.5e+02f)) : 1.5e+02f) : ((iTemp55) ? 1.5e+02f - fTemp60 : 1.3e+02f)) : ((iTemp52) ? 5e+01f * fTemp56 + 1.3e+02f : 1.8e+02f)) : ((iTemp50) ? 1.8e+02f - fTemp95 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp40) ? fTemp82 + 1.2e+02f : 1.3e+02f)) : ((iTemp38) ? 1.3e+02f - fTemp42 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp31) ? 1e+01f * fTemp35 + 1.2e+02f : 1.3e+02f)) : ((iTemp30) ? fTemp33 + 1.3e+02f : 1.5e+02f)) : ((iTemp28) ? 1.5e+02f - 3e+01f * fTemp72 : 1.2e+02f)) : 1.2e+02f) : ((iTemp25) ? 6e+01f * fTemp27 + 1.2e+02f : 1.8e+02f)) : ((iTemp22) ? 1.8e+02f - fTemp91 : 1.3e+02f)) : ((iTemp20) ? 1.3e+02f - fTemp81 : 1.2e+02f)) : 1.2e+02f) : ((iTemp15) ? fTemp80 + 1.2e+02f : 1.3e+02f)) : ((iTemp16) ? 1.3e+02f - 1e+01f * fTemp17 : 1.2e+02f));
			fRec19[0] = fConst5 * fTemp13 * fTemp104 + fConst4 * fRec19[1];
			float fTemp105 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec19[0]));
			fRec20[0] = fConst5 * fTemp104 + fConst4 * fRec20[1];
			float fTemp106 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec20[0]));
			fRec18[0] = fVec1[1] - (fRec18[1] * (0.0f - (fTemp105 + fTemp106)) + fTemp105 * fTemp106 * fRec18[2]);
			float fTemp107 = ((iTemp37) ? 0.1f - 0.084151f * fTemp39 : 0.015849f);
			float fTemp108 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 0.015849f : ((iTemp61) ? 0.001934f * fTemp14 + 0.015849f : 0.017783f)) : ((iTemp58) ? 0.017783f - 0.001934f * fTemp62 : 0.015849f)) : ((iTemp55) ? 0.023962f * fTemp59 + 0.015849f : 0.039811f)) : ((iTemp52) ? 0.039811f - 0.029811f * fTemp56 : 0.01f)) : ((iTemp50) ? 0.344813f * fTemp53 + 0.01f : 0.354813f)) : ((iTemp48) ? 0.354813f - 0.103624f * fTemp51 : 0.251189f)) : ((iTemp46) ? 0.251189f - 0.171756f * fTemp49 : 0.079433f)) : ((iTemp45) ? 0.020567f * fTemp47 + 0.079433f : 0.1f)) : ((iTemp43) ? 0.1f - 0.060189f * fTemp76 : 0.039811f)) : ((iTemp40) ? 0.023285f * fTemp44 + 0.039811f : 0.063096f)) : ((iTemp38) ? 0.036904f * fTemp41 + 0.063096f : 0.1f)) : fTemp107) : ((iTemp36) ? 0.063584f * fTemp75 + 0.015849f : 0.079433f)) : ((iTemp34) ? 0.079433f - 0.04781f * fTemp74 : 0.031623f)) : ((iTemp31) ? 0.068377f * fTemp35 + 0.031623f : 0.1f)) : ((iTemp30) ? 0.1f - 0.09f * fTemp32 : 0.01f)) : ((iTemp28) ? 0.040119f * fTemp72 + 0.01f : 0.050119f)) : fTemp99) : ((iTemp25) ? 0.079433f - 0.069433f * fTemp27 : 0.01f)) : ((iTemp22) ? 0.388107f * fTemp71 + 0.01f : 0.398107f)) : ((iTemp20) ? 0.398107f - 0.198581f * fTemp23 : 0.199526f)) : ((iTemp18) ? 0.199526f - 0.099526f * fTemp21 : 0.1f)) : ((iTemp15) ? 0.151189f * fTemp19 + 0.1f : 0.251189f)) : ((iTemp16) ? 0.251189f - 0.051663f * fTemp17 : 0.199526f));
			float fTemp109 = 3.5e+02f * fTemp51;
			float fTemp110 = ((iTemp70) ? 0.0f : fRec21[1] + fConst1 * ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 3.5e+03f : ((iTemp61) ? 3.5e+03f - 2e+02f * fTemp14 : 3.3e+03f)) : ((iTemp58) ? 4e+02f * fTemp62 + 3.3e+03f : 3.7e+03f)) : ((iTemp55) ? 3.7e+03f - 2e+02f * fTemp59 : 3.5e+03f)) : 3.5e+03f) : ((iTemp50) ? 3.5e+03f - 1.05e+03f * fTemp53 : 2.45e+03f)) : ((iTemp48) ? fTemp109 + 2.45e+03f : 2.8e+03f)) : ((iTemp46) ? 2.5e+02f * fTemp49 + 2.8e+03f : 3.05e+03f)) : ((iTemp45) ? 3.05e+03f - 4.5e+02f * fTemp47 : 2.6e+03f)) : ((iTemp43) ? 75.0f * fTemp76 + 2.6e+03f : 2675.0f)) : ((iTemp40) ? 325.0f * fTemp44 + 2675.0f : 3e+03f)) : 3e+03f) : ((iTemp37) ? 3.5e+02f * fTemp39 + 3e+03f : 3.35e+03f)) : ((iTemp36) ? 3.35e+03f - 3.5e+02f * fTemp75 : 3e+03f)) : 3e+03f) : ((iTemp31) ? 9e+02f * fTemp35 + 3e+03f : 3.9e+03f)) : ((iTemp30) ? 3.9e+03f - 3e+02f * fTemp32 : 3.6e+03f)) : ((iTemp28) ? 3e+02f * fTemp72 + 3.6e+03f : 3.9e+03f)) : ((iTemp26) ? 3.9e+03f - 1e+02f * fTemp29 : 3.8e+03f)) : 3.8e+03f) : ((iTemp22) ? 3.8e+03f - 9e+02f * fTemp71 : 2.9e+03f)) : ((iTemp20) ? 3e+02f * fTemp23 + 2.9e+03f : 3.2e+03f)) : ((iTemp18) ? 5e+01f * fTemp21 + 3.2e+03f : 3.25e+03f)) : ((iTemp15) ? 3.25e+03f - 4.5e+02f * fTemp19 : 2.8e+03f)) : ((iTemp16) ? fTemp101 + 2.8e+03f : 2.9e+03f)));
			fRec21[0] = fTemp110 - std::floor(fTemp110);
			float fTemp111 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? ((iTemp61) ? ((iTemp63) ? 1.4e+02f : ((iTemp61) ? 6e+01f * fTemp14 + 1.4e+02f : 2e+02f)) : 2e+02f) : ((iTemp55) ? 2e+02f - 65.0f * fTemp59 : 135.0f)) : ((iTemp52) ? 65.0f * fTemp56 + 135.0f : 2e+02f)) : ((iTemp50) ? 2e+02f - 7e+01f * fTemp53 : 1.3e+02f)) : ((iTemp48) ? 1.3e+02f - fTemp83 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp40) ? fTemp94 + 1.2e+02f : 1.4e+02f)) : ((iTemp38) ? 1.4e+02f - fTemp93 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp31) ? fTemp92 + 1.2e+02f : 1.4e+02f)) : ((iTemp30) ? 6e+01f * fTemp32 + 1.4e+02f : 2e+02f)) : ((iTemp28) ? 2e+02f - fTemp73 : 1.2e+02f)) : 1.2e+02f) : ((iTemp25) ? 8e+01f * fTemp27 + 1.2e+02f : 2e+02f)) : ((iTemp22) ? 2e+02f - 6e+01f * fTemp71 : 1.4e+02f)) : ((iTemp20) ? 1.4e+02f - fTemp24 : 1.2e+02f)) : 1.2e+02f) : ((iTemp15) ? 15.0f * fTemp19 + 1.2e+02f : 135.0f)) : ((iTemp16) ? 135.0f - 15.0f * fTemp17 : 1.2e+02f));
			fRec23[0] = fConst5 * fTemp13 * fTemp111 + fConst4 * fRec23[1];
			float fTemp112 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec23[0]));
			fRec24[0] = fConst5 * fTemp111 + fConst4 * fRec24[1];
			float fTemp113 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec24[0]));
			fRec22[0] = fVec1[1] - (fRec22[1] * (0.0f - (fTemp112 + fTemp113)) + fTemp112 * fTemp113 * fRec22[2]);
			float fTemp114 = ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? ((iTemp55) ? ((iTemp58) ? 0.001f : ((iTemp55) ? 0.000778f * fTemp59 + 0.001f : 0.001778f)) : ((iTemp52) ? 0.001778f - 0.001147f * fTemp56 : 0.000631f)) : ((iTemp50) ? 0.099369f * fTemp53 + 0.000631f : 0.1f)) : ((iTemp48) ? 0.025893f * fTemp51 + 0.1f : 0.125893f)) : ((iTemp46) ? 0.125893f - 0.086082f * fTemp49 : 0.039811f)) : ((iTemp45) ? 0.039811f - 0.029811f * fTemp47 : 0.01f)) : ((iTemp43) ? 0.005849f * fTemp76 + 0.01f : 0.015849f)) : ((iTemp40) ? 0.015849f - 0.00326f * fTemp44 : 0.012589f)) : ((iTemp38) ? 0.087411f * fTemp41 + 0.012589f : 0.1f)) : fTemp107) : ((iTemp36) ? 0.004104f * fTemp75 + 0.015849f : 0.019953f)) : 0.019953f) : ((iTemp31) ? 0.019953f - 0.016791f * fTemp35 : 0.003162f)) : ((iTemp30) ? 0.003162f - 0.001577f * fTemp32 : 0.001585f)) : ((iTemp28) ? 0.004725f * fTemp72 + 0.001585f : 0.00631f)) : ((iTemp26) ? 0.00631f - 0.003148f * fTemp29 : 0.003162f)) : ((iTemp25) ? 0.003162f - 0.002162f * fTemp27 : 0.001f)) : ((iTemp22) ? 0.078433f * fTemp71 + 0.001f : 0.079433f)) : ((iTemp20) ? 0.020567f * fTemp23 + 0.079433f : 0.1f)) : ((iTemp18) ? 0.1f - 0.068377f * fTemp21 : 0.031623f)) : ((iTemp15) ? 0.018496f * fTemp19 + 0.031623f : 0.050119f)) : 0.050119f);
			float fTemp115 = ((iTemp70) ? 0.0f : fRec25[1] + fConst1 * ((iTemp15) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp25) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp31) ? ((iTemp34) ? ((iTemp36) ? ((iTemp37) ? ((iTemp38) ? ((iTemp40) ? ((iTemp43) ? ((iTemp45) ? ((iTemp46) ? ((iTemp48) ? ((iTemp50) ? ((iTemp52) ? 4.95e+03f : ((iTemp50) ? 4.95e+03f - 2.2e+03f * fTemp53 : 2.75e+03f)) : ((iTemp48) ? fTemp109 + 2.75e+03f : 3.1e+03f)) : ((iTemp46) ? 2.4e+02f * fTemp49 + 3.1e+03f : 3.34e+03f)) : ((iTemp45) ? 3.34e+03f - 4.4e+02f * fTemp47 : 2.9e+03f)) : ((iTemp43) ? fTemp77 + 2.9e+03f : 2.95e+03f)) : ((iTemp40) ? 4e+02f * fTemp44 + 2.95e+03f : 3.35e+03f)) : ((iTemp38) ? 3.35e+03f - fTemp102 : 3.3e+03f)) : ((iTemp37) ? 2.9e+02f * fTemp39 + 3.3e+03f : 3.59e+03f)) : ((iTemp36) ? 3.59e+03f - 2.9e+02f * fTemp75 : 3.3e+03f)) : ((iTemp34) ? 1e+02f * fTemp74 + 3.3e+03f : 3.4e+03f)) : ((iTemp31) ? 1.55e+03f * fTemp35 + 3.4e+03f : 4.95e+03f)) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : ((iTemp22) ? 4.95e+03f - 1.7e+03f * fTemp71 : 3.25e+03f)) : ((iTemp20) ? 3.3e+02f * fTemp23 + 3.25e+03f : 3.58e+03f)) : ((iTemp18) ? 3.58e+03f - 4e+01f * fTemp21 : 3.54e+03f)) : ((iTemp15) ? 3.54e+03f - 5.4e+02f * fTemp19 : 3e+03f)) : ((iTemp16) ? 3e+02f * fTemp17 + 3e+03f : 3.3e+03f)));
			fRec25[0] = fTemp115 - std::floor(fTemp115);
			fVec2[0] = fSlow22;
			fRec26[0] = fSlow22 + fRec26[1] * float(fVec2[1] >= fSlow22);
			iRec27[0] = iSlow24 * (iRec27[1] + 1);
			output0[i0] = FAUSTFLOAT(5e+01f * (fRec0[0] * ((iSlow19) ? fTemp69 : fTemp68) * (1.0f - (fTemp67 + fTemp66 * (1.0f - fTemp67))) * ftbl0synth_voice_fofsSIG0[int(65536.0f * fRec9[0])] + fRec10[0] * ((iSlow19) ? fTemp69 * fTemp87 : fTemp68 * fTemp87) * (1.0f - (fTemp86 + fTemp85 * (1.0f - fTemp86))) * ftbl0synth_voice_fofsSIG0[int(65536.0f * fRec13[0])] + fRec14[0] * ((iSlow19) ? fTemp69 * fTemp100 : fTemp68 * fTemp100) * (1.0f - (fTemp98 + fTemp97 * (1.0f - fTemp98))) * ftbl0synth_voice_fofsSIG0[int(65536.0f * fRec17[0])] + fRec18[0] * ((iSlow19) ? fTemp69 * fTemp108 : fTemp68 * fTemp108) * (1.0f - (fTemp106 + fTemp105 * (1.0f - fTemp106))) * ftbl0synth_voice_fofsSIG0[int(65536.0f * fRec21[0])] + fRec22[0] * ((iSlow19) ? fTemp69 * fTemp114 : fTemp68 * fTemp114) * (1.0f - (fTemp113 + fTemp112 * (1.0f - fTemp113))) * ftbl0synth_voice_fofsSIG0[int(65536.0f * fRec25[0])]) * std::max<float>(0.0f, std::min<float>(fSlow21 * fRec26[0], 1.0f) - fSlow23 * float(iRec27[0])));
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec5[1] = fRec5[0];
			fRec1[1] = fRec1[0];
			fVec1[1] = fVec1[0];
			fRec7[1] = fRec7[0];
			fRec6[1] = fRec6[0];
			fRec8[1] = fRec8[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec9[1] = fRec9[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec13[1] = fRec13[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec17[1] = fRec17[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec21[1] = fRec21[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec25[1] = fRec25[0];
			fVec2[1] = fVec2[0];
			fRec26[1] = fRec26[0];
			iRec27[1] = iRec27[0];
		}
	}

};
#endif

class faust_synth_voice_fofs_tilde : public FaustExternal<synth_voice_fofs> {
public:
    faust_synth_voice_fofs_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_voice_fofs))
    {
    }
};

#endif
