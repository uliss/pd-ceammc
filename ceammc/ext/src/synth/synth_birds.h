/* ------------------------------------------------------------
author: "Pierre Cochard"
name: "synth.birds"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_birds -scn synth_birds_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_birds_H__
#define  __synth_birds_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_birds_dsp.h ********************************
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

class FAUST_API synth_birds_dsp {

    public:

        synth_birds_dsp() {}
        virtual ~synth_birds_dsp() {}

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
        virtual synth_birds_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_birds_dsp {

    protected:

        synth_birds_dsp* fDSP;

    public:

        decorator_dsp(synth_birds_dsp* synth_birds_dsp = nullptr):fDSP(synth_birds_dsp) {}
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
    
        virtual synth_birds_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_birds_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_birds_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_birds : public synth_birds_dsp {
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
#define FAUSTCLASS synth_birds
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

class synth_birdsSIG0 {
	
  private:
	
	int iVec3[2];
	int iRec14[2];
	
  public:
	
	int getNumInputssynth_birdsSIG0() {
		return 0;
	}
	int getNumOutputssynth_birdsSIG0() {
		return 1;
	}
	
	void instanceInitsynth_birdsSIG0(int sample_rate) {
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			iVec3[l16] = 0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			iRec14[l17] = 0;
		}
	}
	
	void fillsynth_birdsSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec3[0] = 1;
			iRec14[0] = (iVec3[1] + iRec14[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec14[0]));
			iVec3[1] = iVec3[0];
			iRec14[1] = iRec14[0];
		}
	}

};

static synth_birdsSIG0* newsynth_birdsSIG0() { return (synth_birdsSIG0*)new synth_birdsSIG0(); }
static void deletesynth_birdsSIG0(synth_birdsSIG0* dsp) { delete dsp; }

static float ftbl0synth_birdsSIG0[65537];
static float synth_birds_faustpower2_f(float value) {
	return value * value;
}
static float synth_birds_faustpower3_f(float value) {
	return value * value * value;
}

class synth_birds : public synth_birds_dsp {
	
 private:
	
	int fSampleRate;
	float fConst2;
	float fConst4;
	float fConst5;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fCheckbox1;
	int iVec0[3];
	float fConst6;
	FAUSTFLOAT fHslider0;
	float fRec8[2];
	int iRec7[2];
	int iRec9[2];
	FAUSTFLOAT fHslider1;
	float fRec10[2];
	int iVec1[2];
	float fRec6[2];
	float fRec11[4];
	float fRec5[2];
	int iVec2[2];
	int iRec4[2];
	float fConst7;
	float fRec13[4];
	float fRec12[2];
	int iRec3[2];
	float fConst8;
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	float fRec2[2];
	float fConst25;
	float fRec1[2];
	float fConst26;
	float fRec17[4];
	float fRec16[2];
	float fConst27;
	float fConst28;
	float fConst29;
	int iRec22[2];
	float fConst30;
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	float fVec4[2];
	float fVec5[2];
	int IOTA0;
	float fVec6[4096];
	float fConst38;
	float fRec18[2];
	float fConst39;
	int iRec25[2];
	float fConst40;
	float fConst41;
	float fConst42;
	float fConst43;
	float fRec24[2];
	float fRec23[2];
	float fConst44;
	int iRec28[2];
	float fConst45;
	float fConst46;
	float fRec30[4];
	float fRec29[2];
	float fConst47;
	float fConst48;
	float fConst49;
	float fConst50;
	float fConst51;
	float fConst52;
	float fConst53;
	float fConst54;
	float fConst55;
	float fConst56;
	float fConst57;
	float fConst58;
	float fConst59;
	float fConst60;
	float fConst61;
	float fConst62;
	float fConst63;
	float fConst64;
	float fConst65;
	float fConst66;
	float fRec27[2];
	float fRec26[2];
	float fRec15[2];
	float fVec7[2];
	float fConst67;
	float fRec0[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Pierre Cochard");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn synth_birds -scn synth_birds_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "synth_birds.dsp");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
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
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "synth.birds");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.4");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.3");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		synth_birdsSIG0* sig0 = newsynth_birdsSIG0();
		sig0->instanceInitsynth_birdsSIG0(sample_rate);
		sig0->fillsynth_birdsSIG0(65537, ftbl0synth_birdsSIG0);
		deletesynth_birdsSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = 1.0f / std::tan(7853.9814f / fConst0);
		fConst2 = 1.0f / (fConst1 + 1.0f);
		float fConst3 = 1.0f / std::tan(9424.778f / fConst0);
		fConst4 = 1.0f / (fConst3 + 1.0f);
		fConst5 = 0.001f * fConst0;
		fConst6 = 6e+01f * fConst0;
		fConst7 = 0.00251f * fConst0;
		fConst8 = 0.002188755f * fConst0;
		fConst9 = 0.002088353f * fConst0;
		fConst10 = 0.001827309f * fConst0;
		fConst11 = 0.001746988f * fConst0;
		fConst12 = 0.001526105f * fConst0;
		fConst13 = 0.001345382f * fConst0;
		fConst14 = 0.001064257f * fConst0;
		fConst15 = 0.000983936f * fConst0;
		fConst16 = 0.000682731f * fConst0;
		fConst17 = 3319.998f / fConst0;
		fConst18 = 0.000582329f * fConst0;
		fConst19 = 9959.961f / fConst0;
		fConst20 = 0.000461847f * fConst0;
		fConst21 = 7702.3955f / fConst0;
		fConst22 = 6.0241e-05f * fConst0;
		fConst23 = 2490.0027f / fConst0;
		fConst24 = 16599.99f / fConst0;
		fConst25 = 1.0f - fConst3;
		fConst26 = 1.0f / fConst0;
		fConst27 = 359.2512f / fConst0;
		fConst28 = 0.041666668f * synth_birds_faustpower2_f(fConst0);
		fConst29 = 0.0017f * fConst0;
		fConst30 = 0.001646586f * fConst0;
		fConst31 = 0.001305221f * fConst0;
		fConst32 = 2929.4158f / fConst0;
		fConst33 = 0.000865f * fConst0;
		fConst34 = 2271.5864f / fConst0;
		fConst35 = 0.000863454f * fConst0;
		fConst36 = 646830.5f / fConst0;
		fConst37 = 567.4882f / fConst0;
		fConst38 = 0.5f * fConst0;
		fConst39 = 0.00055f * fConst0;
		fConst40 = 0.000502068f * fConst0;
		fConst41 = 0.000240964f * fConst0;
		fConst42 = 3829.8916f / fConst0;
		fConst43 = 2299.0986f / fConst0;
		fConst44 = 0.000943f * fConst0;
		fConst45 = 0.000829679f * fConst0;
		fConst46 = 0.000943333f * fConst0;
		fConst47 = 8798.635f / fConst0;
		fConst48 = 0.000806948f * fConst0;
		fConst49 = 43992.785f / fConst0;
		fConst50 = 0.000716024f * fConst0;
		fConst51 = 10998.196f / fConst0;
		fConst52 = 0.000659197f * fConst0;
		fConst53 = 17597.27f / fConst0;
		fConst54 = 0.000613735f * fConst0;
		fConst55 = 21996.393f / fConst0;
		fConst56 = 0.000488715f * fConst0;
		fConst57 = 7998.72f / fConst0;
		fConst58 = 0.000431888f * fConst0;
		fConst59 = 7637.215f / fConst0;
		fConst60 = 0.000318233f * fConst0;
		fConst61 = 8798.557f / fConst0;
		fConst62 = 0.000193213f * fConst0;
		fConst63 = 7.99872f / fConst0;
		fConst64 = 0.000147751f * fConst0;
		fConst65 = 21996.393f / fConst0;
		fConst66 = 6768.1436f / fConst0;
		fConst67 = 1.0f - fConst1;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(2.4e+02f);
		fHslider1 = FAUSTFLOAT(5e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 3; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec8[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iRec7[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec9[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec10[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iVec1[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec6[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec11[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec5[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			iVec2[l9] = 0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			iRec4[l10] = 0;
		}
		for (int l11 = 0; l11 < 4; l11 = l11 + 1) {
			fRec13[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec12[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			iRec3[l13] = 0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec2[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec1[l15] = 0.0f;
		}
		for (int l18 = 0; l18 < 4; l18 = l18 + 1) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			iRec22[l20] = 0;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec21[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec20[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec19[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fVec4[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fVec5[l25] = 0.0f;
		}
		IOTA0 = 0;
		for (int l26 = 0; l26 < 4096; l26 = l26 + 1) {
			fVec6[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec18[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			iRec25[l28] = 0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec24[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec23[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			iRec28[l31] = 0;
		}
		for (int l32 = 0; l32 < 4; l32 = l32 + 1) {
			fRec30[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec29[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec27[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec26[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec15[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fVec7[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec0[l38] = 0.0f;
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
	
	virtual synth_birds* clone() {
		return new synth_birds();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.birds");
		ui_interface->declare(&fCheckbox0, "type", "bool");
		ui_interface->addCheckButton("auto", &fCheckbox0);
		ui_interface->declare(&fCheckbox1, "type", "bool");
		ui_interface->addCheckButton("gate", &fCheckbox1);
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "%");
		ui_interface->addHorizontalSlider("probability", &fHslider1, FAUSTFLOAT(5e+01f), FAUSTFLOAT(25.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("speed", &fHslider0, FAUSTFLOAT(2.4e+02f), FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(4.8e+02f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		int iSlow1 = int(float(fCheckbox1));
		float fSlow2 = 0.001f * float(fHslider0);
		float fSlow3 = 1e-05f * float(fHslider1);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec8[0] = fSlow2 + 0.999f * fRec8[1];
			iRec7[0] = (iVec0[1] + iRec7[1]) % int(fConst6 / fRec8[0]);
			iRec9[0] = 1103515245 * iRec9[1] + 12345;
			fRec10[0] = fSlow3 + 0.999f * fRec10[1];
			int iTemp0 = (iRec7[0] <= iRec7[1]) * (std::fabs(4.656613e-10f * float(iRec9[0])) <= fRec10[0]);
			iVec1[0] = iTemp0;
			fRec6[0] = fRec6[1] + float(float(iTemp0 - iVec1[1]) > 0.0f) - 0.020833334f * float(fRec6[1] > 0.0f);
			int iTemp1 = ((iSlow0) ? fRec6[0] > 0.0f : iSlow1);
			fRec11[0] = std::fmod(float(int(2994.2312f * (fRec11[2] + fRec11[3])) + 38125), 2.9e+03f);
			fRec5[0] = ((iTemp1) ? fRec11[0] + 1e+02f : fRec5[1]);
			int iTemp2 = int(fConst5 * std::fabs(fRec5[0] + -1.0f));
			iVec2[0] = iTemp2;
			iRec4[0] = (iTemp2 == iVec2[1]) * (iRec4[1] + 1);
			float fTemp3 = float(iRec4[0]);
			fRec13[0] = std::fmod(float(int(2994.2312f * (fRec13[2] + fRec13[3])) + 38125), 2e+03f);
			fRec12[0] = ((iTemp1) ? fRec13[0] : fRec12[1]);
			float fTemp4 = std::fabs(fRec12[0] + -1.0f);
			iRec3[0] = ((fTemp3 < (fConst7 * fTemp4)) ? iRec4[0] : iRec3[1]);
			float fTemp5 = float(iRec3[0]);
			int iTemp6 = fTemp5 < (fConst15 * fTemp4);
			float fTemp7 = fConst16 * fTemp4;
			int iTemp8 = fTemp5 < fTemp7;
			float fTemp9 = fConst18 * fTemp4;
			int iTemp10 = fTemp5 < fTemp9;
			float fTemp11 = fConst20 * fTemp4;
			int iTemp12 = fTemp5 < fTemp11;
			float fTemp13 = fConst22 * fTemp4;
			int iTemp14 = fTemp5 < fTemp13;
			fRec2[0] = 0.001f * ((fTemp5 < (fConst8 * fTemp4)) ? ((fTemp5 < (fConst9 * fTemp4)) ? ((fTemp5 < (fConst10 * fTemp4)) ? ((fTemp5 < (fConst11 * fTemp4)) ? ((fTemp5 < (fConst12 * fTemp4)) ? ((fTemp5 < (fConst13 * fTemp4)) ? ((fTemp5 < (fConst14 * fTemp4)) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iRec3[0] < 0) ? 0.0f : ((iTemp14) ? fConst24 * (fTemp5 / fTemp4) : 1.0f)) : ((iTemp12) ? fConst23 * ((0.0f - (fTemp5 - fTemp13)) / fTemp4) + 1.0f : 0.0f)) : ((iTemp10) ? fConst21 * ((fTemp5 - fTemp11) / fTemp4) : 0.928f)) : ((iTemp8) ? fConst19 * ((0.0f - 0.428f * (fTemp5 - fTemp9)) / fTemp4) + 0.928f : 0.5f)) : ((iTemp6) ? fConst17 * ((0.0f - 0.5f * (fTemp5 - fTemp7)) / fTemp4) + 0.5f : 0.0f)) : 0.0f) : 0.0f) : 0.0f) : 0.0f) : 0.0f) : 0.0f) : 0.0f) : 0.0f) + 0.999f * fRec2[1];
			fRec1[0] = fConst4 * (fRec2[0] + fRec2[1] - fConst25 * fRec1[1]);
			fRec17[0] = std::fmod(float(int(2994.2312f * (fRec17[2] + fRec17[3])) + 38125), 22.0f);
			fRec16[0] = ((iTemp1) ? fRec17[0] + 72.0f : fRec16[1]);
			float fTemp15 = std::pow(2.0f, 0.083333336f * (std::fabs(fRec16[0] + -1.0f) + -69.0f));
			iRec22[0] = ((fTemp3 < (fConst29 * fTemp4)) ? iRec4[0] : iRec22[1]);
			float fTemp16 = float(iRec22[0]);
			int iTemp17 = fTemp16 < (fConst30 * fTemp4);
			float fTemp18 = fConst31 * fTemp4;
			int iTemp19 = fTemp16 < fTemp18;
			float fTemp20 = fConst33 * fTemp4;
			int iTemp21 = fTemp16 < fTemp20;
			float fTemp22 = fConst35 * fTemp4;
			int iTemp23 = fTemp16 < fTemp22;
			fRec21[0] = 0.001f * ((iTemp17) ? ((iTemp19) ? ((iTemp21) ? ((iTemp23) ? ((iRec22[0] < 0) ? 0.0f : ((iTemp23) ? fConst37 * (fTemp16 / fTemp4) : 0.49f)) : ((iTemp21) ? fConst36 * ((0.0f - 0.49f * (fTemp16 - fTemp22)) / fTemp4) + 0.49f : 0.0f)) : ((iTemp19) ? fConst34 * ((fTemp16 - fTemp20) / fTemp4) : 1.0f)) : ((iTemp17) ? fConst32 * ((0.0f - (fTemp16 - fTemp18)) / fTemp4) + 1.0f : 0.0f)) : 0.0f) + 0.999f * fRec21[1];
			fRec20[0] = fConst4 * (fRec21[0] + fRec21[1] - fConst25 * fRec20[1]);
			float fTemp24 = fRec20[0] + 1.0f;
			float fTemp25 = std::max<float>(27.72f * fTemp15 * fTemp24, 23.44895f);
			float fTemp26 = std::max<float>(2e+01f, std::fabs(fTemp25));
			float fTemp27 = fRec19[1] + fConst26 * fTemp26;
			fRec19[0] = fTemp27 - std::floor(fTemp27);
			float fTemp28 = 2.0f * fRec19[0];
			float fTemp29 = synth_birds_faustpower3_f(fTemp28 + -1.0f);
			fVec4[0] = fTemp29 + (1.0f - fTemp28);
			float fTemp30 = (fTemp29 + (1.0f - (fTemp28 + fVec4[1]))) / fTemp26;
			fVec5[0] = fTemp30;
			float fTemp31 = float(iVec0[2]) * (fTemp30 - fVec5[1]) / fTemp26;
			fVec6[IOTA0 & 4095] = fTemp31;
			float fTemp32 = std::max<float>(0.0f, std::min<float>(2047.0f, fConst38 / fTemp25));
			int iTemp33 = int(fTemp32);
			float fTemp34 = std::floor(fTemp32);
			fRec18[0] = 0.999f * fRec18[1] - fConst28 * (fVec6[(IOTA0 - iTemp33) & 4095] * (fTemp34 + (1.0f - fTemp32)) - fTemp31 + (fTemp32 - fTemp34) * fVec6[(IOTA0 - (iTemp33 + 1)) & 4095]);
			iRec25[0] = ((fTemp3 < (fConst39 * fTemp4)) ? iRec4[0] : iRec25[1]);
			float fTemp35 = float(iRec25[0]);
			int iTemp36 = fTemp35 < (fConst40 * fTemp4);
			float fTemp37 = fConst41 * fTemp4;
			int iTemp38 = fTemp35 < fTemp37;
			fRec24[0] = 0.001f * ((iTemp36) ? ((iTemp38) ? ((iRec25[0] < 0) ? 0.0f : ((iTemp38) ? fConst43 * (fTemp35 / fTemp4) : 0.554f)) : ((iTemp36) ? fConst42 * ((0.0f - 0.554f * (fTemp35 - fTemp37)) / fTemp4) + 0.554f : 0.0f)) : 0.0f) + 0.999f * fRec24[1];
			fRec23[0] = fConst4 * (fRec24[0] + fRec24[1] - fConst25 * fRec23[1]);
			iRec28[0] = ((fTemp3 < (fConst44 * fTemp4)) ? iRec4[0] : iRec28[1]);
			float fTemp39 = float(iRec28[0]);
			float fTemp40 = fConst45 * fTemp4;
			int iTemp41 = fTemp39 < fTemp40;
			fRec30[0] = std::fmod(float(int(2994.2312f * (fRec30[2] + fRec30[3])) + 38125), 1e+03f);
			fRec29[0] = ((iTemp1) ? fRec30[0] : fRec29[1]);
			float fTemp42 = std::fabs(fRec29[0] + -1.0f);
			float fTemp43 = 0.001f * fTemp42;
			float fTemp44 = fConst48 * fTemp4;
			int iTemp45 = fTemp39 < fTemp44;
			float fTemp46 = fTemp43 + -1.0f;
			float fTemp47 = fConst50 * fTemp4;
			int iTemp48 = fTemp39 < fTemp47;
			float fTemp49 = 1.0f - fTemp43;
			float fTemp50 = fConst52 * fTemp4;
			int iTemp51 = fTemp39 < fTemp50;
			float fTemp52 = fConst54 * fTemp4;
			int iTemp53 = fTemp39 < fTemp52;
			float fTemp54 = fConst56 * fTemp4;
			int iTemp55 = fTemp39 < fTemp54;
			float fTemp56 = fConst58 * fTemp4;
			int iTemp57 = fTemp39 < fTemp56;
			float fTemp58 = fConst60 * fTemp4;
			int iTemp59 = fTemp39 < fTemp58;
			float fTemp60 = fConst62 * fTemp4;
			int iTemp61 = fTemp39 < fTemp60;
			float fTemp62 = fConst64 * fTemp4;
			int iTemp63 = fTemp39 < fTemp62;
			fRec27[0] = 0.001f * ((iTemp41) ? ((iTemp45) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? ((iTemp61) ? ((iTemp63) ? ((iRec28[0] < 0) ? 0.0f : ((iTemp63) ? fConst66 * (fTemp39 / fTemp4) : 1.0f)) : ((iTemp61) ? fConst65 * ((0.0f - (fTemp39 - fTemp62)) / fTemp4) + 1.0f : 0.0f)) : ((iTemp59) ? fConst63 * (fTemp42 * (fTemp39 - fTemp60) / fTemp4) : fTemp43)) : ((iTemp57) ? fTemp43 + fConst61 * ((0.0f - 0.001f * fTemp42 * (fTemp39 - fTemp58)) / fTemp4) : 0.0f)) : ((iTemp55) ? fConst59 * ((fTemp39 - fTemp56) / fTemp4) : 0.434f)) : ((iTemp53) ? fConst57 * ((fTemp39 - fTemp54) * (fTemp43 + -0.434f) / fTemp4) + 0.434f : fTemp43)) : ((iTemp51) ? fTemp43 + fConst55 * (fTemp49 * (fTemp39 - fTemp52) / fTemp4) : 1.0f)) : ((iTemp48) ? fConst53 * (fTemp46 * (fTemp39 - fTemp50) / fTemp4) + 1.0f : fTemp43)) : ((iTemp45) ? fTemp43 + fConst51 * ((fTemp39 - fTemp47) * fTemp49 / fTemp4) : 1.0f)) : ((iTemp41) ? fConst49 * ((fTemp39 - fTemp44) * fTemp46 / fTemp4) + 1.0f : fTemp43)) : ((fTemp39 < (fConst46 * fTemp4)) ? fTemp43 + fConst47 * ((0.0f - 0.001f * fTemp42 * (fTemp39 - fTemp40)) / fTemp4) : 0.0f)) + 0.999f * fRec27[1];
			fRec26[0] = 0.0f - fConst4 * (fConst25 * fRec26[1] - (fRec27[0] + fRec27[1]));
			float fTemp64 = fRec15[1] + fConst26 * fTemp15 * (fConst27 * fRec18[0] * fTemp24 * (fRec23[0] + 1.0f) + 4.4e+02f) * (fRec26[0] + 1.0f);
			fRec15[0] = fTemp64 - std::floor(fTemp64);
			float fTemp65 = 65536.0f * fRec15[0];
			int iTemp66 = int(fTemp65);
			float fTemp67 = ftbl0synth_birdsSIG0[iTemp66];
			float fTemp68 = fRec1[0] * (fTemp67 + (fTemp65 - std::floor(fTemp65)) * (ftbl0synth_birdsSIG0[iTemp66 + 1] - fTemp67));
			fVec7[0] = fTemp68;
			fRec0[0] = fConst2 * (fTemp68 + fVec7[1] - fConst67 * fRec0[1]);
			float fTemp69 = 0.8f * fRec0[0];
			output0[i0] = FAUSTFLOAT(fTemp69);
			output1[i0] = FAUSTFLOAT(fTemp69);
			iVec0[2] = iVec0[1];
			iVec0[1] = iVec0[0];
			fRec8[1] = fRec8[0];
			iRec7[1] = iRec7[0];
			iRec9[1] = iRec9[0];
			fRec10[1] = fRec10[0];
			iVec1[1] = iVec1[0];
			fRec6[1] = fRec6[0];
			for (int j0 = 3; j0 > 0; j0 = j0 - 1) {
				fRec11[j0] = fRec11[j0 - 1];
			}
			fRec5[1] = fRec5[0];
			iVec2[1] = iVec2[0];
			iRec4[1] = iRec4[0];
			for (int j1 = 3; j1 > 0; j1 = j1 - 1) {
				fRec13[j1] = fRec13[j1 - 1];
			}
			fRec12[1] = fRec12[0];
			iRec3[1] = iRec3[0];
			fRec2[1] = fRec2[0];
			fRec1[1] = fRec1[0];
			for (int j2 = 3; j2 > 0; j2 = j2 - 1) {
				fRec17[j2] = fRec17[j2 - 1];
			}
			fRec16[1] = fRec16[0];
			iRec22[1] = iRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			IOTA0 = IOTA0 + 1;
			fRec18[1] = fRec18[0];
			iRec25[1] = iRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			iRec28[1] = iRec28[0];
			for (int j3 = 3; j3 > 0; j3 = j3 - 1) {
				fRec30[j3] = fRec30[j3 - 1];
			}
			fRec29[1] = fRec29[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec15[1] = fRec15[0];
			fVec7[1] = fVec7[0];
			fRec0[1] = fRec0[0];
		}
	}

};
#endif

class faust_synth_birds_tilde : public FaustExternal<synth_birds> {
public:
    faust_synth_birds_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_birds))
    {
    }
};

#endif
