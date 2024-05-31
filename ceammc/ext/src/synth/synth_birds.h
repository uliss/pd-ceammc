/* ------------------------------------------------------------
author: "Pierre Cochard"
name: "synth.birds"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_birds -scn synth_birds_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
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
        virtual synth_birds_dsp* clone() = 0;
    
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
        virtual synth_birds_dsp* createDSPInstance() = 0;
    
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
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	int iVec0[3];
	float fConst4;
	float fConst5;
	float fConst6;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fCheckbox1;
	float fConst7;
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
	float fConst8;
	float fRec13[4];
	float fRec12[2];
	int iRec3[2];
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
	float fConst25;
	float fRec2[2];
	float fConst26;
	float fRec1[2];
	float fConst27;
	float fRec17[4];
	float fRec16[2];
	float fConst28;
	float fConst29;
	float fConst30;
	int iRec22[2];
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fConst38;
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	float fVec4[2];
	float fVec5[2];
	int IOTA0;
	float fVec6[4096];
	float fConst39;
	float fRec18[2];
	float fConst40;
	int iRec25[2];
	float fConst41;
	float fConst42;
	float fConst43;
	float fConst44;
	float fRec24[2];
	float fRec23[2];
	float fConst45;
	int iRec28[2];
	float fConst46;
	float fConst47;
	float fRec30[4];
	float fRec29[2];
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
	float fConst67;
	float fRec27[2];
	float fRec26[2];
	float fRec15[2];
	float fVec7[2];
	float fRec0[2];
	
 public:
	synth_birds() {
	}
	
	void metadata(Meta* m) { 
		m->declare("author", "Pierre Cochard");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.17.1");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_birds -scn synth_birds_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "synth_birds.dsp");
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
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "synth.birds");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.1");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/sawN:author", "Julius O. Smith III");
		m->declare("oscillators.lib/sawN:license", "STK-4.3");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.5.0");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / std::tan(7853.9814f / fConst0);
		fConst2 = 1.0f / (fConst1 + 1.0f);
		fConst3 = 1.0f - fConst1;
		fConst4 = 1.0f / std::tan(9424.778f / fConst0);
		fConst5 = 1.0f / (fConst4 + 1.0f);
		fConst6 = 0.001f * fConst0;
		fConst7 = 6e+01f * fConst0;
		fConst8 = 0.00251f * fConst0;
		fConst9 = 0.002188755f * fConst0;
		fConst10 = 0.002088353f * fConst0;
		fConst11 = 0.001827309f * fConst0;
		fConst12 = 0.001746988f * fConst0;
		fConst13 = 0.001526105f * fConst0;
		fConst14 = 0.001345382f * fConst0;
		fConst15 = 0.001064257f * fConst0;
		fConst16 = 0.000983936f * fConst0;
		fConst17 = 0.000682731f * fConst0;
		fConst18 = 1659.999f / fConst0;
		fConst19 = 0.000582329f * fConst0;
		fConst20 = 4262.8633f / fConst0;
		fConst21 = 0.000461847f * fConst0;
		fConst22 = 7702.3955f / fConst0;
		fConst23 = 6.0241e-05f * fConst0;
		fConst24 = 2490.0027f / fConst0;
		fConst25 = 16599.99f / fConst0;
		fConst26 = 1.0f - fConst4;
		fConst27 = 1.0f / fConst0;
		fConst28 = 359.2512f / fConst0;
		fConst29 = 0.041666668f * synth_birds_faustpower2_f(fConst0);
		fConst30 = 0.0017f * fConst0;
		fConst31 = 0.001646586f * fConst0;
		fConst32 = 0.001305221f * fConst0;
		fConst33 = 2929.4158f / fConst0;
		fConst34 = 0.000865f * fConst0;
		fConst35 = 2271.5864f / fConst0;
		fConst36 = 0.000863454f * fConst0;
		fConst37 = 316946.97f / fConst0;
		fConst38 = 567.4882f / fConst0;
		fConst39 = 0.5f * fConst0;
		fConst40 = 0.00055f * fConst0;
		fConst41 = 0.000502068f * fConst0;
		fConst42 = 0.000240964f * fConst0;
		fConst43 = 2121.76f / fConst0;
		fConst44 = 2299.0986f / fConst0;
		fConst45 = 0.000943f * fConst0;
		fConst46 = 0.000829679f * fConst0;
		fConst47 = 0.000943333f * fConst0;
		fConst48 = 8.798635f / fConst0;
		fConst49 = 0.000806948f * fConst0;
		fConst50 = 43992.785f / fConst0;
		fConst51 = 0.000716024f * fConst0;
		fConst52 = 10998.196f / fConst0;
		fConst53 = 0.000659197f * fConst0;
		fConst54 = 17597.27f / fConst0;
		fConst55 = 0.000613735f * fConst0;
		fConst56 = 21996.393f / fConst0;
		fConst57 = 0.000488715f * fConst0;
		fConst58 = 7998.72f / fConst0;
		fConst59 = 0.000431888f * fConst0;
		fConst60 = 7637.215f / fConst0;
		fConst61 = 0.000318233f * fConst0;
		fConst62 = 8.798557f / fConst0;
		fConst63 = 0.000193213f * fConst0;
		fConst64 = 7.99872f / fConst0;
		fConst65 = 0.000147751f * fConst0;
		fConst66 = 21996.393f / fConst0;
		fConst67 = 6768.1436f / fConst0;
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
			iRec7[0] = (iVec0[1] + iRec7[1]) % int(fConst7 / fRec8[0]);
			iRec9[0] = 1103515245 * iRec9[1] + 12345;
			fRec10[0] = fSlow3 + 0.999f * fRec10[1];
			int iTemp0 = (iRec7[0] <= iRec7[1]) * (std::fabs(4.656613e-10f * float(iRec9[0])) <= fRec10[0]);
			iVec1[0] = iTemp0;
			fRec6[0] = fRec6[1] + float(float(iTemp0 - iVec1[1]) > 0.0f) - 0.020833334f * float(fRec6[1] > 0.0f);
			int iTemp1 = ((iSlow0) ? fRec6[0] > 0.0f : iSlow1);
			fRec11[0] = std::fmod(float(int(2994.2312f * (fRec11[2] + fRec11[3])) + 38125), 2.9e+03f);
			fRec5[0] = ((iTemp1) ? fRec11[0] + 1e+02f : fRec5[1]);
			int iTemp2 = int(fConst6 * std::fabs(fRec5[0] + -1.0f));
			iVec2[0] = iTemp2;
			iRec4[0] = (iTemp2 == iVec2[1]) * (iRec4[1] + 1);
			float fTemp3 = float(iRec4[0]);
			fRec13[0] = std::fmod(float(int(2994.2312f * (fRec13[2] + fRec13[3])) + 38125), 2e+03f);
			fRec12[0] = ((iTemp1) ? fRec13[0] : fRec12[1]);
			float fTemp4 = std::fabs(fRec12[0] + -1.0f);
			iRec3[0] = ((fTemp3 < (fConst8 * fTemp4)) ? iRec4[0] : iRec3[1]);
			float fTemp5 = float(iRec3[0]);
			int iTemp6 = fTemp5 < (fConst16 * fTemp4);
			float fTemp7 = fConst17 * fTemp4;
			int iTemp8 = fTemp5 < fTemp7;
			float fTemp9 = fConst19 * fTemp4;
			int iTemp10 = fTemp5 < fTemp9;
			float fTemp11 = fConst21 * fTemp4;
			int iTemp12 = fTemp5 < fTemp11;
			float fTemp13 = fConst23 * fTemp4;
			int iTemp14 = fTemp5 < fTemp13;
			fRec2[0] = 0.001f * ((fTemp5 < (fConst9 * fTemp4)) ? ((fTemp5 < (fConst10 * fTemp4)) ? ((fTemp5 < (fConst11 * fTemp4)) ? ((fTemp5 < (fConst12 * fTemp4)) ? ((fTemp5 < (fConst13 * fTemp4)) ? ((fTemp5 < (fConst14 * fTemp4)) ? ((fTemp5 < (fConst15 * fTemp4)) ? ((iTemp6) ? ((iTemp8) ? ((iTemp10) ? ((iTemp12) ? ((iTemp14) ? ((iRec3[0] < 0) ? 0.0f : ((iTemp14) ? fConst25 * (fTemp5 / fTemp4) : 1.0f)) : ((iTemp12) ? fConst24 * ((fTemp13 - fTemp5) / fTemp4) + 1.0f : 0.0f)) : ((iTemp10) ? fConst22 * ((fTemp5 - fTemp11) / fTemp4) : 0.928f)) : ((iTemp8) ? fConst20 * ((fTemp9 - fTemp5) / fTemp4) + 0.928f : 0.5f)) : ((iTemp6) ? fConst18 * ((fTemp7 - fTemp5) / fTemp4) + 0.5f : 0.0f)) : 0.0f) : 0.0f) : 0.0f) : 0.0f) : 0.0f) : 0.0f) : 0.0f) : 0.0f) + 0.999f * fRec2[1];
			fRec1[0] = fConst5 * (fRec2[0] + fRec2[1] - fConst26 * fRec1[1]);
			int iTemp15 = 1 - iVec0[1];
			fRec17[0] = std::fmod(float(int(2994.2312f * (fRec17[2] + fRec17[3])) + 38125), 22.0f);
			fRec16[0] = ((iTemp1) ? fRec17[0] + 72.0f : fRec16[1]);
			float fTemp16 = std::pow(2.0f, 0.083333336f * (std::fabs(fRec16[0] + -1.0f) + -69.0f));
			iRec22[0] = ((fTemp3 < (fConst30 * fTemp4)) ? iRec4[0] : iRec22[1]);
			float fTemp17 = float(iRec22[0]);
			int iTemp18 = fTemp17 < (fConst31 * fTemp4);
			float fTemp19 = fConst32 * fTemp4;
			int iTemp20 = fTemp17 < fTemp19;
			float fTemp21 = fConst34 * fTemp4;
			int iTemp22 = fTemp17 < fTemp21;
			float fTemp23 = fConst36 * fTemp4;
			int iTemp24 = fTemp17 < fTemp23;
			fRec21[0] = 0.001f * ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iRec22[0] < 0) ? 0.0f : ((iTemp24) ? fConst38 * (fTemp17 / fTemp4) : 0.49f)) : ((iTemp22) ? fConst37 * ((fTemp23 - fTemp17) / fTemp4) + 0.49f : 0.0f)) : ((iTemp20) ? fConst35 * ((fTemp17 - fTemp21) / fTemp4) : 1.0f)) : ((iTemp18) ? fConst33 * ((fTemp19 - fTemp17) / fTemp4) + 1.0f : 0.0f)) : 0.0f) + 0.999f * fRec21[1];
			fRec20[0] = -(fConst5 * (fConst26 * fRec20[1] - (fRec21[0] + fRec21[1])));
			float fTemp25 = fRec20[0] + 1.0f;
			float fTemp26 = std::max<float>(27.72f * fTemp16 * fTemp25, 23.44895f);
			float fTemp27 = std::max<float>(2e+01f, std::fabs(fTemp26));
			float fTemp28 = ((iTemp15) ? 0.0f : fRec19[1] + fConst27 * fTemp27);
			fRec19[0] = fTemp28 - std::floor(fTemp28);
			float fTemp29 = 2.0f * fRec19[0];
			float fTemp30 = synth_birds_faustpower3_f(fTemp29 + -1.0f);
			fVec4[0] = fTemp30 + (1.0f - fTemp29);
			float fTemp31 = (fTemp30 + (1.0f - (fTemp29 + fVec4[1]))) / fTemp27;
			fVec5[0] = fTemp31;
			float fTemp32 = float(iVec0[2]) * (fTemp31 - fVec5[1]) / fTemp27;
			fVec6[IOTA0 & 4095] = fTemp32;
			float fTemp33 = std::max<float>(0.0f, std::min<float>(2047.0f, fConst39 / fTemp26));
			int iTemp34 = int(fTemp33);
			float fTemp35 = std::floor(fTemp33);
			fRec18[0] = 0.999f * fRec18[1] + fConst29 * (fTemp32 - fVec6[(IOTA0 - iTemp34) & 4095] * (fTemp35 + (1.0f - fTemp33)) - (fTemp33 - fTemp35) * fVec6[(IOTA0 - (iTemp34 + 1)) & 4095]);
			iRec25[0] = ((fTemp3 < (fConst40 * fTemp4)) ? iRec4[0] : iRec25[1]);
			float fTemp36 = float(iRec25[0]);
			int iTemp37 = fTemp36 < (fConst41 * fTemp4);
			float fTemp38 = fConst42 * fTemp4;
			int iTemp39 = fTemp36 < fTemp38;
			fRec24[0] = 0.001f * ((iTemp37) ? ((iTemp39) ? ((iRec25[0] < 0) ? 0.0f : ((iTemp39) ? fConst44 * (fTemp36 / fTemp4) : 0.554f)) : ((iTemp37) ? fConst43 * ((fTemp38 - fTemp36) / fTemp4) + 0.554f : 0.0f)) : 0.0f) + 0.999f * fRec24[1];
			fRec23[0] = fConst5 * (fRec24[0] + fRec24[1] - fConst26 * fRec23[1]);
			iRec28[0] = ((fTemp3 < (fConst45 * fTemp4)) ? iRec4[0] : iRec28[1]);
			float fTemp40 = float(iRec28[0]);
			float fTemp41 = fConst46 * fTemp4;
			int iTemp42 = fTemp40 < fTemp41;
			fRec30[0] = std::fmod(float(int(2994.2312f * (fRec30[2] + fRec30[3])) + 38125), 1e+03f);
			fRec29[0] = ((iTemp1) ? fRec30[0] : fRec29[1]);
			float fTemp43 = std::fabs(fRec29[0] + -1.0f);
			float fTemp44 = fConst49 * fTemp4;
			int iTemp45 = fTemp40 < fTemp44;
			float fTemp46 = 0.001f * fTemp43;
			float fTemp47 = fTemp46 + -1.0f;
			float fTemp48 = fConst51 * fTemp4;
			int iTemp49 = fTemp40 < fTemp48;
			float fTemp50 = 1.0f - fTemp46;
			float fTemp51 = fConst53 * fTemp4;
			int iTemp52 = fTemp40 < fTemp51;
			float fTemp53 = fConst55 * fTemp4;
			int iTemp54 = fTemp40 < fTemp53;
			float fTemp55 = fConst57 * fTemp4;
			int iTemp56 = fTemp40 < fTemp55;
			float fTemp57 = fConst59 * fTemp4;
			int iTemp58 = fTemp40 < fTemp57;
			float fTemp59 = fConst61 * fTemp4;
			int iTemp60 = fTemp40 < fTemp59;
			float fTemp61 = fConst63 * fTemp4;
			int iTemp62 = fTemp40 < fTemp61;
			float fTemp63 = fConst65 * fTemp4;
			int iTemp64 = fTemp40 < fTemp63;
			fRec27[0] = 0.001f * ((iTemp42) ? ((iTemp45) ? ((iTemp49) ? ((iTemp52) ? ((iTemp54) ? ((iTemp56) ? ((iTemp58) ? ((iTemp60) ? ((iTemp62) ? ((iTemp64) ? ((iRec28[0] < 0) ? 0.0f : ((iTemp64) ? fConst67 * (fTemp40 / fTemp4) : 1.0f)) : ((iTemp62) ? fConst66 * ((fTemp63 - fTemp40) / fTemp4) + 1.0f : 0.0f)) : ((iTemp60) ? fConst64 * (fTemp43 * (fTemp40 - fTemp61) / fTemp4) : fTemp46)) : ((iTemp58) ? fTemp43 * (0.001f - fConst62 * ((fTemp40 - fTemp59) / fTemp4)) : 0.0f)) : ((iTemp56) ? fConst60 * ((fTemp40 - fTemp57) / fTemp4) : 0.434f)) : ((iTemp54) ? fConst58 * ((fTemp40 - fTemp55) * (fTemp46 + -0.434f) / fTemp4) + 0.434f : fTemp46)) : ((iTemp52) ? fTemp46 + fConst56 * (fTemp50 * (fTemp40 - fTemp53) / fTemp4) : 1.0f)) : ((iTemp49) ? fConst54 * (fTemp47 * (fTemp40 - fTemp51) / fTemp4) + 1.0f : fTemp46)) : ((iTemp45) ? fTemp46 + fConst52 * ((fTemp40 - fTemp48) * fTemp50 / fTemp4) : 1.0f)) : ((iTemp42) ? fConst50 * ((fTemp40 - fTemp44) * fTemp47 / fTemp4) + 1.0f : fTemp46)) : ((fTemp40 < (fConst47 * fTemp4)) ? fTemp43 * (0.001f - fConst48 * ((fTemp40 - fTemp41) / fTemp4)) : 0.0f)) + 0.999f * fRec27[1];
			fRec26[0] = -(fConst5 * (fConst26 * fRec26[1] - (fRec27[0] + fRec27[1])));
			float fTemp65 = ((iTemp15) ? 0.0f : fRec15[1] + fConst27 * fTemp16 * (fConst28 * fRec18[0] * fTemp25 * (fRec23[0] + 1.0f) + 4.4e+02f) * (fRec26[0] + 1.0f));
			fRec15[0] = fTemp65 - std::floor(fTemp65);
			float fTemp66 = 65536.0f * fRec15[0];
			int iTemp67 = int(fTemp66);
			float fTemp68 = ftbl0synth_birdsSIG0[std::max<int>(0, std::min<int>(iTemp67, 65536))];
			float fTemp69 = fRec1[0] * (fTemp68 + (fTemp66 - std::floor(fTemp66)) * (ftbl0synth_birdsSIG0[std::max<int>(0, std::min<int>(iTemp67 + 1, 65536))] - fTemp68));
			fVec7[0] = fTemp69;
			fRec0[0] = -(fConst2 * (fConst3 * fRec0[1] - (fTemp69 + fVec7[1])));
			float fTemp70 = 0.8f * fRec0[0];
			output0[i0] = FAUSTFLOAT(fTemp70);
			output1[i0] = FAUSTFLOAT(fTemp70);
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
