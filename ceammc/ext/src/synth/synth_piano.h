/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.piano"
version: "1.0"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_piano -scn synth_piano_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_piano_H__
#define  __synth_piano_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_piano_dsp.h ********************************
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

class FAUST_API synth_piano_dsp {

    public:

        synth_piano_dsp() {}
        virtual ~synth_piano_dsp() {}

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
        virtual synth_piano_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_piano_dsp {

    protected:

        synth_piano_dsp* fDSP;

    public:

        decorator_dsp(synth_piano_dsp* synth_piano_dsp = nullptr):fDSP(synth_piano_dsp) {}
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
        virtual synth_piano_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_piano_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_piano_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_piano : public synth_piano_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include "piano.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_piano
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

static float synth_piano_faustpower2_f(float value) {
	return value * value;
}

class synth_piano : public synth_piano_dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider1;
	float fConst2;
	float fRec10[2];
	FAUSTFLOAT fHslider2;
	float fConst3;
	FAUSTFLOAT fHslider3;
	float fConst4;
	FAUSTFLOAT fHslider4;
	int iRec24[2];
	FAUSTFLOAT fButton0;
	int iRec26[2];
	float fConst5;
	float fRec25[2];
	float fConst6;
	float fConst7;
	FAUSTFLOAT fHslider5;
	float fConst8;
	float fConst9;
	float fRec27[2];
	float fRec23[2];
	float fRec22[2];
	float fRec21[2];
	float fRec20[2];
	float fRec19[2];
	float fRec28[2];
	float fVec0[2];
	float fRec18[2];
	float fRec17[2];
	int IOTA0;
	float fRec16[8192];
	float fConst10;
	float fConst11;
	FAUSTFLOAT fHslider6;
	float fVec1[2];
	float fRec31[2];
	float fRec30[2];
	float fRec29[8192];
	float fVec2[2];
	float fRec34[2];
	float fRec33[2];
	float fRec32[8192];
	float fVec3[2];
	float fRec15[2];
	float fRec11[2];
	float fRec12[2];
	float fRec13[2];
	float fVec4[2];
	float fVec5[2];
	float fRec44[2];
	float fRec43[2];
	float fRec42[2];
	float fRec41[2];
	float fRec40[2];
	float fConst12;
	float fRec39[3];
	float fRec38[3];
	float fRec37[3];
	float fRec36[3];
	float fRec35[2];
	float fRec56[2];
	float fRec55[2];
	float fRec54[2];
	float fRec53[2];
	float fRec52[2];
	float fVec6[2];
	float fRec51[2];
	float fRec50[2];
	float fRec49[8192];
	float fVec7[2];
	float fRec59[2];
	float fRec58[2];
	float fRec57[8192];
	float fVec8[2];
	float fRec48[2];
	float fRec45[2];
	float fRec46[2];
	float fConst13;
	float fConst14;
	float fConst15;
	float fRec60[3];
	float fRec61[3];
	float fVec9[4096];
	float fVec10[4096];
	int iConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	FAUSTFLOAT fHslider7;
	float fConst20;
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	float fRec63[2];
	float fRec62[2];
	float fVec11[16384];
	float fConst25;
	int iConst26;
	float fVec12[4096];
	int iConst27;
	float fRec8[2];
	float fConst28;
	float fConst29;
	float fConst30;
	float fConst31;
	float fRec67[2];
	float fRec66[2];
	float fVec13[16384];
	float fConst32;
	int iConst33;
	float fVec14[2048];
	int iConst34;
	float fRec64[2];
	float fConst35;
	float fConst36;
	float fConst37;
	float fConst38;
	float fRec71[2];
	float fRec70[2];
	float fVec15[16384];
	float fConst39;
	int iConst40;
	float fVec16[2048];
	int iConst41;
	float fRec68[2];
	float fConst42;
	float fConst43;
	float fConst44;
	float fConst45;
	float fRec75[2];
	float fRec74[2];
	float fVec17[16384];
	float fConst46;
	int iConst47;
	float fVec18[4096];
	int iConst48;
	float fRec72[2];
	float fConst49;
	float fConst50;
	float fConst51;
	float fConst52;
	float fRec79[2];
	float fRec78[2];
	float fVec19[32768];
	float fConst53;
	int iConst54;
	float fConst55;
	FAUSTFLOAT fHslider8;
	float fVec20[4096];
	float fVec21[4096];
	int iConst56;
	float fRec76[2];
	float fConst57;
	float fConst58;
	float fConst59;
	float fConst60;
	float fRec83[2];
	float fRec82[2];
	float fVec22[16384];
	float fConst61;
	int iConst62;
	float fVec23[4096];
	int iConst63;
	float fRec80[2];
	float fConst64;
	float fConst65;
	float fConst66;
	float fConst67;
	float fRec87[2];
	float fRec86[2];
	float fVec24[32768];
	float fConst68;
	int iConst69;
	float fVec25[4096];
	int iConst70;
	float fRec84[2];
	float fConst71;
	float fConst72;
	float fConst73;
	float fConst74;
	float fRec91[2];
	float fRec90[2];
	float fVec26[32768];
	float fConst75;
	int iConst76;
	float fVec27[2048];
	int iConst77;
	float fRec88[2];
	float fRec0[3];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fVec28[4096];
	
 public:
	synth_piano() {
	}
	
	void metadata(Meta* m) { 
		m->declare("author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.17.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.4");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn synth_piano -scn synth_piano_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "Romain Michon");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("description", "WaveGuide Commuted Piano");
		m->declare("filename", "synth_piano.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("instruments.lib/author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("instruments.lib/copyright", "Romain Michon");
		m->declare("instruments.lib/licence", "STK-4.3");
		m->declare("instruments.lib/name", "Faust-STK Tools Library");
		m->declare("instruments.lib/version", "1.0.0");
		m->declare("licence", "STK-4.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "synth.piano");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "1.3.0");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.5.0");
		m->declare("spn.lib/name", "Standart Pitch Notation constants");
		m->declare("spn.lib/version", "0.2");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 0.0003617158f * fConst0;
		fConst4 = 2764.6016f / fConst0;
		fConst5 = 7.0f / fConst0;
		fConst6 = std::exp(-(5.0f / fConst0));
		fConst7 = 1e+01f / fConst0;
		fConst8 = 0.1f * fConst0;
		fConst9 = std::exp(-(0.5f / fConst0));
		fConst10 = 0.15915494f * fConst0;
		fConst11 = 6.2831855f / fConst0;
		fConst12 = 0.05f / fConst0;
		fConst13 = 9.68e+04f / synth_piano_faustpower2_f(fConst0);
		fConst14 = 1.0f / fConst0;
		fConst15 = 1.936e+05f / fConst0;
		iConst16 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.02f * fConst0)));
		fConst17 = std::floor(0.174713f * fConst0 + 0.5f);
		fConst18 = fConst17 / fConst0;
		fConst19 = 1.7269388f * fConst18;
		fConst20 = std::cos(37699.113f / fConst0);
		fConst21 = 1.1512926f * fConst18;
		fConst22 = 1.0f / std::tan(628.31854f / fConst0);
		fConst23 = 1.0f / (fConst22 + 1.0f);
		fConst24 = 1.0f - fConst22;
		fConst25 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst26 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst17 - fConst25)));
		iConst27 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst25 + -1.0f)));
		fConst28 = std::floor(0.153129f * fConst0 + 0.5f);
		fConst29 = fConst28 / fConst0;
		fConst30 = 1.7269388f * fConst29;
		fConst31 = 1.1512926f * fConst29;
		fConst32 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst33 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst28 - fConst32)));
		iConst34 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst32 + -1.0f)));
		fConst35 = std::floor(0.125f * fConst0 + 0.5f);
		fConst36 = fConst35 / fConst0;
		fConst37 = 1.7269388f * fConst36;
		fConst38 = 1.1512926f * fConst36;
		fConst39 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst40 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst35 - fConst39)));
		iConst41 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst39 + -1.0f)));
		fConst42 = std::floor(0.127837f * fConst0 + 0.5f);
		fConst43 = fConst42 / fConst0;
		fConst44 = 1.7269388f * fConst43;
		fConst45 = 1.1512926f * fConst43;
		fConst46 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst47 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst42 - fConst46)));
		iConst48 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst46 + -1.0f)));
		fConst49 = std::floor(0.256891f * fConst0 + 0.5f);
		fConst50 = fConst49 / fConst0;
		fConst51 = 1.7269388f * fConst50;
		fConst52 = 1.1512926f * fConst50;
		fConst53 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst54 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst49 - fConst53)));
		fConst55 = 0.0011363636f * fConst0;
		iConst56 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst53 + -1.0f)));
		fConst57 = std::floor(0.192303f * fConst0 + 0.5f);
		fConst58 = fConst57 / fConst0;
		fConst59 = 1.7269388f * fConst58;
		fConst60 = 1.1512926f * fConst58;
		fConst61 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst62 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst57 - fConst61)));
		iConst63 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst61 + -1.0f)));
		fConst64 = std::floor(0.210389f * fConst0 + 0.5f);
		fConst65 = fConst64 / fConst0;
		fConst66 = 1.7269388f * fConst65;
		fConst67 = 1.1512926f * fConst65;
		fConst68 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst69 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst64 - fConst68)));
		iConst70 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst68 + -1.0f)));
		fConst71 = std::floor(0.219991f * fConst0 + 0.5f);
		fConst72 = fConst71 / fConst0;
		fConst73 = 1.7269388f * fConst72;
		fConst74 = 1.1512926f * fConst72;
		fConst75 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst76 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst71 - fConst75)));
		iConst77 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst75 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.6f);
		fHslider1 = FAUSTFLOAT(0.137f);
		fHslider2 = FAUSTFLOAT(48.0f);
		fHslider3 = FAUSTFLOAT(0.28f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.1f);
		fHslider6 = FAUSTFLOAT(0.1f);
		fHslider7 = FAUSTFLOAT(0.36f);
		fHslider8 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec10[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iRec24[l1] = 0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iRec26[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec25[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec27[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec23[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec22[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec21[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec20[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec19[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec28[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fVec0[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec18[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec17[l13] = 0.0f;
		}
		IOTA0 = 0;
		for (int l14 = 0; l14 < 8192; l14 = l14 + 1) {
			fRec16[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fVec1[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec31[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec30[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 8192; l18 = l18 + 1) {
			fRec29[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fVec2[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec34[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec33[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 8192; l22 = l22 + 1) {
			fRec32[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fVec3[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec15[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec11[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec12[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec13[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fVec4[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fVec5[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec44[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec43[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec42[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec41[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec40[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fRec39[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 3; l36 = l36 + 1) {
			fRec38[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fRec37[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 3; l38 = l38 + 1) {
			fRec36[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec35[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec56[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec55[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec54[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec53[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec52[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fVec6[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec51[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec50[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 8192; l48 = l48 + 1) {
			fRec49[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fVec7[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec59[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec58[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 8192; l52 = l52 + 1) {
			fRec57[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fVec8[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec48[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec45[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec46[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fRec60[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 3; l58 = l58 + 1) {
			fRec61[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 4096; l59 = l59 + 1) {
			fVec9[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 4096; l60 = l60 + 1) {
			fVec10[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec63[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec62[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 16384; l63 = l63 + 1) {
			fVec11[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 4096; l64 = l64 + 1) {
			fVec12[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec8[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec67[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec66[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 16384; l68 = l68 + 1) {
			fVec13[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2048; l69 = l69 + 1) {
			fVec14[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec64[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec71[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec70[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 16384; l73 = l73 + 1) {
			fVec15[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2048; l74 = l74 + 1) {
			fVec16[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec68[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec75[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec74[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 16384; l78 = l78 + 1) {
			fVec17[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 4096; l79 = l79 + 1) {
			fVec18[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec72[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec79[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec78[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 32768; l83 = l83 + 1) {
			fVec19[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 4096; l84 = l84 + 1) {
			fVec20[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 4096; l85 = l85 + 1) {
			fVec21[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec76[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fRec83[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec82[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 16384; l89 = l89 + 1) {
			fVec22[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 4096; l90 = l90 + 1) {
			fVec23[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			fRec80[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec87[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec86[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 32768; l94 = l94 + 1) {
			fVec24[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 4096; l95 = l95 + 1) {
			fVec25[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec84[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec91[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec90[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 32768; l99 = l99 + 1) {
			fVec26[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2048; l100 = l100 + 1) {
			fVec27[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec88[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 3; l102 = l102 + 1) {
			fRec0[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 3; l103 = l103 + 1) {
			fRec1[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 3; l104 = l104 + 1) {
			fRec2[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 3; l105 = l105 + 1) {
			fRec3[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 3; l106 = l106 + 1) {
			fRec4[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 3; l107 = l107 + 1) {
			fRec5[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 3; l108 = l108 + 1) {
			fRec6[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 3; l109 = l109 + 1) {
			fRec7[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 4096; l110 = l110 + 1) {
			fVec28[l110] = 0.0f;
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
	
	virtual synth_piano* clone() {
		return new synth_piano();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.piano");
		ui_interface->addHorizontalSlider("brightness", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("detuning", &fHslider6, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider5, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pan", &fHslider0, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pitch", &fHslider2, FAUSTFLOAT(48.0f), FAUSTFLOAT(21.0f), FAUSTFLOAT(108.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider1, FAUSTFLOAT(0.137f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("room_size", &fHslider7, FAUSTFLOAT(0.36f), FAUSTFLOAT(0.005f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("stiffness", &fHslider3, FAUSTFLOAT(0.28f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("width", &fHslider8, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = 12.0f * (1.0f - fSlow0);
		float fSlow2 = fConst1 * float(fHslider1);
		float fSlow3 = float(fHslider2);
		float fSlow4 = getValueSingleStringZero(fSlow3);
		float fSlow5 = 1.0f - fSlow4;
		float fSlow6 = getValueSingleStringPole(fSlow3);
		float fSlow7 = 1.0f - fSlow6;
		float fSlow8 = std::pow(2.0f, 0.083333336f * (fSlow3 + -69.0f));
		float fSlow9 = std::pow(1e+01f, 0.000113636364f * (getValueSingleStringDecayRate(fSlow3) / fSlow8));
		float fSlow10 = fSlow7 * fSlow9;
		float fSlow11 = 3.0f * fSlow5 - fSlow10;
		float fSlow12 = 1.0f / fSlow11;
		float fSlow13 = fSlow4 + fSlow10 + -1.0f;
		float fSlow14 = float(fHslider3);
		float fSlow15 = getValueStiffnessCoefficient(fSlow3);
		float fSlow16 = 13.69f * synth_piano_faustpower2_f(fSlow14) * synth_piano_faustpower2_f(fSlow15);
		float fSlow17 = fSlow16 + -1.0f;
		float fSlow18 = fSlow8 + std::pow(2.0f, 0.083333336f * (fSlow3 + -129.0f));
		float fSlow19 = fConst4 * fSlow18;
		float fSlow20 = std::sin(fSlow19);
		float fSlow21 = fSlow14 * fSlow15;
		float fSlow22 = 7.4f * fSlow21;
		float fSlow23 = fSlow16 + 1.0f;
		float fSlow24 = std::cos(fSlow19);
		float fSlow25 = 3.0f * std::atan2(fSlow17 * fSlow20, fSlow22 + fSlow23 * fSlow24);
		float fSlow26 = fSlow4 * fSlow7 * fSlow9;
		float fSlow27 = fSlow6 * fSlow5;
		float fSlow28 = 3.0f * fSlow27;
		float fSlow29 = fSlow26 - fSlow28;
		float fSlow30 = fSlow27 - fSlow26;
		float fSlow31 = 4.0f * fSlow30;
		float fSlow32 = fSlow29 + fSlow31;
		float fSlow33 = fSlow29 * fSlow24 / fSlow11 + 1.0f;
		float fSlow34 = 4.0f * fSlow13;
		float fSlow35 = (fSlow34 + fSlow32 * fSlow24) / fSlow11 + 1.0f;
		float fSlow36 = fSlow29 * fSlow32;
		float fSlow37 = synth_piano_faustpower2_f(fSlow20);
		float fSlow38 = synth_piano_faustpower2_f(fSlow11);
		float fSlow39 = fConst3 * ((fSlow25 + std::atan2(-(fSlow20 * (fSlow32 * fSlow33 - fSlow29 * fSlow35) / fSlow11), fSlow33 * fSlow35 + fSlow36 * fSlow37 / fSlow38) + 6.2831855f) / fSlow18);
		float fSlow40 = std::floor(fSlow39);
		float fSlow41 = fSlow39 - fSlow40;
		float fSlow42 = getValueDCBa1(fSlow3);
		float fSlow43 = 1.0f - fSlow42;
		float fSlow44 = 0.25f * float(fHslider4);
		float fSlow45 = getValueLoudPole(fSlow3);
		float fSlow46 = getValueLoudGain(fSlow3) * (fSlow44 + (0.98f - fSlow45));
		float fSlow47 = 1.1641532e-09f * fSlow46 * float((fSlow3 >= 45.0f) & (fSlow3 < 88.0f));
		float fSlow48 = float(fButton0);
		int iSlow49 = fSlow48 > 0.0f;
		int iSlow50 = iSlow49 > 0;
		float fSlow51 = std::exp(-(fConst5 / (getValueDryTapAmpT60(fSlow3) * std::min<float>(1.0f, std::max<float>(0.0f, fSlow48)))));
		int iSlow52 = iSlow49 < 1;
		float fSlow53 = float(-(iSlow49 + -1));
		float fSlow54 = fConst6 * fSlow53;
		float fSlow55 = float(iSlow49);
		float fSlow56 = float(fHslider5);
		float fSlow57 = fSlow55 * std::exp(-(fConst7 / fSlow56));
		float fSlow58 = fConst8 * fSlow56;
		float fSlow59 = 0.2f * getValueSustainPedalLevel(fSlow3);
		float fSlow60 = fSlow45 + (0.02f - fSlow44);
		float fSlow61 = fSlow42 - 1.0f;
		float fSlow62 = fConst1 * (0.9996f * fSlow55 + 0.9f * getValueReleaseLoopGain(fSlow3) * fSlow53);
		float fSlow63 = 3.7f * fSlow21;
		float fSlow64 = fSlow26 + fSlow31 - fSlow28;
		float fSlow65 = (fSlow34 + fSlow64 * fSlow24) / fSlow11 + 1.0f;
		float fSlow66 = fSlow29 * fSlow64;
		int iSlow67 = int(fConst3 * ((fSlow25 + std::atan2(-(fSlow20 * (fSlow64 * fSlow33 - fSlow29 * fSlow65) / fSlow11), fSlow33 * fSlow65 + fSlow66 * fSlow37 / fSlow38) + 6.2831855f) / fSlow18));
		int iSlow68 = std::min<int>(4097, std::max<int>(0, iSlow67 + 1));
		float fSlow69 = fSlow40 + (1.0f - fSlow39);
		int iSlow70 = std::min<int>(4097, std::max<int>(0, iSlow67));
		float fSlow71 = 4.4e+02f * fSlow18;
		float fSlow72 = 5.0f * getValueDetuningHz(fSlow3) * float(fHslider6);
		float fSlow73 = fSlow71 - fSlow72;
		float fSlow74 = fConst11 * fSlow73;
		float fSlow75 = std::sin(fSlow74);
		float fSlow76 = std::cos(fSlow74);
		float fSlow77 = 3.0f * std::atan2(fSlow17 * fSlow75, fSlow22 + fSlow23 * fSlow76);
		float fSlow78 = (fSlow34 + fSlow32 * fSlow76) / fSlow11 + 1.0f;
		float fSlow79 = fSlow29 * fSlow76 / fSlow11 + 1.0f;
		float fSlow80 = synth_piano_faustpower2_f(fSlow75);
		float fSlow81 = fConst10 * ((fSlow77 + std::atan2(fSlow75 * (fSlow29 * fSlow78 - fSlow32 * fSlow79) / fSlow11, fSlow79 * fSlow78 + fSlow36 * fSlow80 / fSlow38) + 6.2831855f) / fSlow73);
		float fSlow82 = std::floor(fSlow81);
		float fSlow83 = fSlow81 - fSlow82;
		float fSlow84 = (fSlow34 + fSlow64 * fSlow76) / fSlow11 + 1.0f;
		int iSlow85 = int(fConst10 * ((fSlow77 + std::atan2(fSlow75 * (fSlow29 * fSlow84 - fSlow64 * fSlow79) / fSlow11, fSlow79 * fSlow84 + fSlow66 * fSlow80 / fSlow38) + 6.2831855f) / fSlow73));
		int iSlow86 = std::min<int>(4097, std::max<int>(0, iSlow85 + 1));
		float fSlow87 = fSlow82 + (1.0f - fSlow81);
		int iSlow88 = std::min<int>(4097, std::max<int>(0, iSlow85));
		float fSlow89 = fSlow72 + fSlow71;
		float fSlow90 = fConst11 * fSlow89;
		float fSlow91 = std::sin(fSlow90);
		float fSlow92 = std::cos(fSlow90);
		float fSlow93 = 3.0f * std::atan2(fSlow17 * fSlow91, fSlow22 + fSlow23 * fSlow92);
		float fSlow94 = fSlow29 * fSlow92 / fSlow11 + 1.0f;
		float fSlow95 = (fSlow34 + fSlow32 * fSlow92) / fSlow11 + 1.0f;
		float fSlow96 = synth_piano_faustpower2_f(fSlow91);
		float fSlow97 = fConst10 * ((fSlow93 + std::atan2(-(fSlow91 * (fSlow32 * fSlow94 - fSlow29 * fSlow95) / fSlow11), fSlow94 * fSlow95 + fSlow36 * fSlow96 / fSlow38) + 6.2831855f) / fSlow89);
		float fSlow98 = std::floor(fSlow97);
		float fSlow99 = fSlow98 + (1.0f - fSlow97);
		float fSlow100 = (fSlow34 + fSlow64 * fSlow92) / fSlow11 + 1.0f;
		int iSlow101 = int(fConst10 * ((fSlow93 + std::atan2(-(fSlow91 * (fSlow64 * fSlow94 - fSlow29 * fSlow100) / fSlow11), fSlow94 * fSlow100 + fSlow66 * fSlow96 / fSlow38) + 6.2831855f) / fSlow89));
		int iSlow102 = std::min<int>(4097, std::max<int>(0, iSlow101));
		float fSlow103 = fSlow97 - fSlow98;
		int iSlow104 = std::min<int>(4097, std::max<int>(0, iSlow101 + 1));
		float fSlow105 = getValueBq4_gEarBalled(fSlow3);
		float fSlow106 = 2.0f * fSlow105;
		float fSlow107 = float(fSlow3 >= 88.0f);
		float fSlow108 = 2.3283064e-10f * fSlow107;
		float fSlow109 = 1.1641532e-10f * fSlow107;
		float fSlow110 = std::pow(1e+01f, fConst12 * getValuer3db(fSlow3));
		float fSlow111 = 2.0f * std::cos(fConst4 * fSlow8 * getValueThirdPartialFactor(fSlow3));
		float fSlow112 = std::pow(1e+01f, fConst12 * getValuer2db(fSlow3));
		float fSlow113 = 2.0f * std::cos(fConst4 * fSlow8 * getValueSecondPartialFactor(fSlow3));
		float fSlow114 = std::pow(1e+01f, fConst12 * getValuer1_1db(fSlow3));
		float fSlow115 = std::cos(fConst4 * fSlow8);
		float fSlow116 = 2.0f * fSlow115;
		float fSlow117 = std::pow(1e+01f, 0.05f * getValueSecondStageAmpRatio(fSlow3));
		float fSlow118 = 1.0f - fSlow117;
		float fSlow119 = std::pow(1e+01f, fConst12 * getValuer1_2db(fSlow3));
		float fSlow120 = fSlow117 * synth_piano_faustpower2_f(fSlow114) + fSlow118 * synth_piano_faustpower2_f(fSlow119);
		float fSlow121 = 2.0f * fSlow115 * (fSlow117 * fSlow114 + fSlow118 * fSlow119);
		float fSlow122 = 4.4e+02f * fSlow8;
		float fSlow123 = fSlow122 - fSlow72;
		float fSlow124 = fConst11 * fSlow123;
		float fSlow125 = std::sin(fSlow124);
		float fSlow126 = std::cos(fSlow124);
		float fSlow127 = 3.0f * std::atan2(fSlow17 * fSlow125, fSlow22 + fSlow23 * fSlow126);
		float fSlow128 = fSlow126 * fSlow29 / fSlow11 + 1.0f;
		float fSlow129 = (fSlow126 * fSlow32 + fSlow34) / fSlow11 + 1.0f;
		float fSlow130 = synth_piano_faustpower2_f(fSlow125) * fSlow29;
		float fSlow131 = fConst10 * ((fSlow127 + std::atan2(-(fSlow125 * (fSlow32 * fSlow128 - fSlow29 * fSlow129) / fSlow11), fSlow128 * fSlow129 + fSlow130 * fSlow32 / fSlow38) + 6.2831855f) / fSlow123);
		float fSlow132 = std::floor(fSlow131);
		float fSlow133 = fSlow131 - fSlow132;
		float fSlow134 = 1.6298145e-09f * fSlow46 * float(fSlow3 < 45.0f);
		float fSlow135 = (fSlow34 + fSlow126 * fSlow64) / fSlow11 + 1.0f;
		int iSlow136 = int(fConst10 * ((fSlow127 + std::atan2(-(fSlow125 * (fSlow128 * fSlow64 - fSlow29 * fSlow135) / fSlow11), fSlow128 * fSlow135 + fSlow130 * fSlow64 / fSlow38) + 6.2831855f) / fSlow123));
		int iSlow137 = std::min<int>(4097, std::max<int>(0, iSlow136 + 1));
		float fSlow138 = fSlow132 + (1.0f - fSlow131);
		int iSlow139 = std::min<int>(4097, std::max<int>(0, iSlow136));
		float fSlow140 = fSlow122 + fSlow72;
		float fSlow141 = fConst11 * fSlow140;
		float fSlow142 = std::sin(fSlow141);
		float fSlow143 = std::cos(fSlow141);
		float fSlow144 = 3.0f * std::atan2(fSlow17 * fSlow142, fSlow22 + fSlow23 * fSlow143);
		float fSlow145 = fSlow29 * fSlow143 / fSlow11 + 1.0f;
		float fSlow146 = (fSlow34 + fSlow32 * fSlow143) / fSlow11 + 1.0f;
		float fSlow147 = synth_piano_faustpower2_f(fSlow142);
		float fSlow148 = fConst10 * ((fSlow144 + std::atan2(-(fSlow142 * (fSlow32 * fSlow145 - fSlow29 * fSlow146) / fSlow11), fSlow145 * fSlow146 + fSlow36 * fSlow147 / fSlow38) + 6.2831855f) / fSlow140);
		float fSlow149 = std::floor(fSlow148);
		float fSlow150 = fSlow149 + (1.0f - fSlow148);
		float fSlow151 = (fSlow34 + fSlow64 * fSlow143) / fSlow11 + 1.0f;
		int iSlow152 = int(fConst10 * ((fSlow144 + std::atan2(-(fSlow142 * (fSlow64 * fSlow145 - fSlow29 * fSlow151) / fSlow11), fSlow145 * fSlow151 + fSlow66 * fSlow147 / fSlow38) + 6.2831855f) / fSlow140));
		int iSlow153 = std::min<int>(4097, std::max<int>(0, iSlow152));
		float fSlow154 = fSlow148 - fSlow149;
		int iSlow155 = std::min<int>(4097, std::max<int>(0, iSlow152 + 1));
		float fSlow156 = getValueEQBandWidthFactor(fSlow3);
		float fSlow157 = 0.5f - fConst13 * synth_piano_faustpower2_f(fSlow8) * synth_piano_faustpower2_f(fSlow156);
		float fSlow158 = getValueEQGain(fSlow3);
		float fSlow159 = fSlow8 * fSlow156;
		float fSlow160 = fConst14 * fSlow159;
		float fSlow161 = fConst15 * fSlow159;
		float fSlow162 = 8.8e+02f * std::cos(fConst4 * (fSlow8 / getValueStrikePosition(fSlow3)));
		float fSlow163 = float(fHslider7);
		float fSlow164 = std::exp(-(fConst19 / fSlow163));
		float fSlow165 = synth_piano_faustpower2_f(fSlow164);
		float fSlow166 = 1.0f - fConst20 * fSlow165;
		float fSlow167 = 1.0f - fSlow165;
		float fSlow168 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow166) / synth_piano_faustpower2_f(fSlow167) + -1.0f));
		float fSlow169 = fSlow166 / fSlow167;
		float fSlow170 = fSlow164 * (fSlow168 + (1.0f - fSlow169));
		float fSlow171 = std::exp(-(fConst21 / fSlow163)) / fSlow164 + -1.0f;
		float fSlow172 = fSlow169 - fSlow168;
		float fSlow173 = std::exp(-(fConst30 / fSlow163));
		float fSlow174 = synth_piano_faustpower2_f(fSlow173);
		float fSlow175 = 1.0f - fConst20 * fSlow174;
		float fSlow176 = 1.0f - fSlow174;
		float fSlow177 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow175) / synth_piano_faustpower2_f(fSlow176) + -1.0f));
		float fSlow178 = fSlow175 / fSlow176;
		float fSlow179 = fSlow173 * (fSlow177 + (1.0f - fSlow178));
		float fSlow180 = std::exp(-(fConst31 / fSlow163)) / fSlow173 + -1.0f;
		float fSlow181 = fSlow178 - fSlow177;
		float fSlow182 = std::exp(-(fConst37 / fSlow163));
		float fSlow183 = synth_piano_faustpower2_f(fSlow182);
		float fSlow184 = 1.0f - fConst20 * fSlow183;
		float fSlow185 = 1.0f - fSlow183;
		float fSlow186 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow184) / synth_piano_faustpower2_f(fSlow185) + -1.0f));
		float fSlow187 = fSlow184 / fSlow185;
		float fSlow188 = fSlow182 * (fSlow186 + (1.0f - fSlow187));
		float fSlow189 = std::exp(-(fConst38 / fSlow163)) / fSlow182 + -1.0f;
		float fSlow190 = fSlow187 - fSlow186;
		float fSlow191 = std::exp(-(fConst44 / fSlow163));
		float fSlow192 = synth_piano_faustpower2_f(fSlow191);
		float fSlow193 = 1.0f - fConst20 * fSlow192;
		float fSlow194 = 1.0f - fSlow192;
		float fSlow195 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow193) / synth_piano_faustpower2_f(fSlow194) + -1.0f));
		float fSlow196 = fSlow193 / fSlow194;
		float fSlow197 = fSlow191 * (fSlow195 + (1.0f - fSlow196));
		float fSlow198 = std::exp(-(fConst45 / fSlow163)) / fSlow191 + -1.0f;
		float fSlow199 = fSlow196 - fSlow195;
		float fSlow200 = std::exp(-(fConst51 / fSlow163));
		float fSlow201 = synth_piano_faustpower2_f(fSlow200);
		float fSlow202 = 1.0f - fConst20 * fSlow201;
		float fSlow203 = 1.0f - fSlow201;
		float fSlow204 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow202) / synth_piano_faustpower2_f(fSlow203) + -1.0f));
		float fSlow205 = fSlow202 / fSlow203;
		float fSlow206 = fSlow200 * (fSlow204 + (1.0f - fSlow205));
		float fSlow207 = std::exp(-(fConst52 / fSlow163)) / fSlow200 + -1.0f;
		float fSlow208 = fSlow205 - fSlow204;
		float fSlow209 = 12.0f * fSlow0;
		int iSlow210 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst55 * (float(fHslider8) / fSlow8))));
		float fSlow211 = std::exp(-(fConst59 / fSlow163));
		float fSlow212 = synth_piano_faustpower2_f(fSlow211);
		float fSlow213 = 1.0f - fConst20 * fSlow212;
		float fSlow214 = 1.0f - fSlow212;
		float fSlow215 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow213) / synth_piano_faustpower2_f(fSlow214) + -1.0f));
		float fSlow216 = fSlow213 / fSlow214;
		float fSlow217 = fSlow211 * (fSlow215 + (1.0f - fSlow216));
		float fSlow218 = std::exp(-(fConst60 / fSlow163)) / fSlow211 + -1.0f;
		float fSlow219 = fSlow216 - fSlow215;
		float fSlow220 = std::exp(-(fConst66 / fSlow163));
		float fSlow221 = synth_piano_faustpower2_f(fSlow220);
		float fSlow222 = 1.0f - fConst20 * fSlow221;
		float fSlow223 = 1.0f - fSlow221;
		float fSlow224 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow222) / synth_piano_faustpower2_f(fSlow223) + -1.0f));
		float fSlow225 = fSlow222 / fSlow223;
		float fSlow226 = fSlow220 * (fSlow224 + (1.0f - fSlow225));
		float fSlow227 = std::exp(-(fConst67 / fSlow163)) / fSlow220 + -1.0f;
		float fSlow228 = fSlow225 - fSlow224;
		float fSlow229 = std::exp(-(fConst73 / fSlow163));
		float fSlow230 = synth_piano_faustpower2_f(fSlow229);
		float fSlow231 = 1.0f - fConst20 * fSlow230;
		float fSlow232 = 1.0f - fSlow230;
		float fSlow233 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow231) / synth_piano_faustpower2_f(fSlow232) + -1.0f));
		float fSlow234 = fSlow231 / fSlow232;
		float fSlow235 = fSlow229 * (fSlow233 + (1.0f - fSlow234));
		float fSlow236 = std::exp(-(fConst74 / fSlow163)) / fSlow229 + -1.0f;
		float fSlow237 = fSlow234 - fSlow233;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec10[0] = fSlow2 + fConst2 * fRec10[1];
			iRec24[0] = 1103515245 * iRec24[1] + 12345;
			iRec26[0] = iSlow49 * iRec26[1] + 1;
			float fTemp0 = float(iRec26[0] + -1);
			float fTemp1 = float((fTemp0 < 2.0f) & iSlow50);
			float fTemp2 = 0.030197384f * fTemp1 + fSlow51 * float((fTemp0 >= 2.0f) | iSlow52);
			fRec25[0] = fRec25[1] * fTemp2 + 0.15f * fTemp1 * (1.0f - fTemp2);
			int iTemp3 = fTemp0 < fSlow58;
			float fTemp4 = fSlow57 * float(iTemp3) + fConst9 * float(iSlow49 * (fTemp0 >= fSlow58));
			fRec27[0] = fRec27[1] * (fSlow54 + fTemp4) + fSlow59 * (1.0f - fTemp4 - fSlow54) * float(iTemp3 & iSlow50);
			float fTemp5 = float(iRec24[0]) * (fRec25[0] + fRec27[0]);
			fRec23[0] = fSlow47 * fTemp5 + fSlow60 * fRec23[1];
			fRec22[0] = fSlow46 * fRec23[0] + fSlow60 * fRec22[1];
			fRec21[0] = fSlow46 * fRec22[0] + fSlow60 * fRec21[1];
			fRec20[0] = fSlow46 * fRec21[0] + fSlow60 * fRec20[1];
			fRec19[0] = 0.5f * (fSlow43 * fRec20[0] + fSlow61 * fRec20[1]) - fSlow42 * fRec19[1];
			fRec28[0] = fSlow62 + fConst2 * fRec28[1];
			float fTemp6 = fRec19[0] + fRec28[0] * fRec12[1];
			fVec0[0] = fTemp6;
			fRec18[0] = fVec0[1] + fSlow63 * (fTemp6 - fRec18[1]);
			fRec17[0] = fRec18[1] + fSlow63 * (fRec18[0] - fRec17[1]);
			fRec16[IOTA0 & 8191] = fRec17[1] - fSlow63 * (fRec16[(IOTA0 - 1) & 8191] - fRec17[0]);
			float fTemp7 = fSlow41 * fRec16[(IOTA0 - iSlow68) & 8191];
			float fTemp8 = fSlow69 * fRec16[(IOTA0 - iSlow70) & 8191];
			float fTemp9 = fRec19[0] + fRec28[0] * fRec13[1];
			fVec1[0] = fTemp9;
			fRec31[0] = fVec1[1] + fSlow63 * (fTemp9 - fRec31[1]);
			fRec30[0] = fRec31[1] + fSlow63 * (fRec31[0] - fRec30[1]);
			fRec29[IOTA0 & 8191] = fRec30[1] + fSlow63 * (fRec30[0] - fRec29[(IOTA0 - 1) & 8191]);
			float fTemp10 = fSlow83 * fRec29[(IOTA0 - iSlow86) & 8191];
			float fTemp11 = fSlow87 * fRec29[(IOTA0 - iSlow88) & 8191];
			float fTemp12 = fRec28[0] * (fRec19[0] + fRec11[1]);
			fVec2[0] = fTemp12;
			fRec34[0] = fVec2[1] + fSlow63 * (fTemp12 - fRec34[1]);
			fRec33[0] = fRec34[1] + fSlow63 * (fRec34[0] - fRec33[1]);
			fRec32[IOTA0 & 8191] = fRec33[1] + fSlow63 * (fRec33[0] - fRec32[(IOTA0 - 1) & 8191]);
			float fTemp13 = fSlow99 * fRec32[(IOTA0 - iSlow102) & 8191] + fSlow103 * fRec32[(IOTA0 - iSlow104) & 8191];
			float fTemp14 = fTemp7 + fTemp8 + fTemp10 + fTemp11 + fTemp13;
			fVec3[0] = fTemp14;
			fRec15[0] = fSlow12 * (2.0f * (fSlow13 * fTemp14 + fSlow30 * fVec3[1]) - fSlow29 * fRec15[1]);
			fRec11[0] = fRec15[0] + fTemp13;
			fRec12[0] = fRec15[0] + fTemp8 + fTemp7;
			fRec13[0] = fRec15[0] + fTemp11 + fTemp10;
			float fRec14 = fTemp14;
			fVec4[0] = fSlow108 * fTemp5;
			float fTemp15 = 0.5f * fVec4[1] + fSlow109 * fTemp5;
			fVec5[0] = fTemp15;
			fRec44[0] = -(0.5f * (fSlow43 * fTemp15 + fSlow61 * fVec5[1]) + fSlow42 * fRec44[1]);
			fRec43[0] = fSlow46 * fRec44[0] + fSlow60 * fRec43[1];
			fRec42[0] = fSlow46 * fRec43[0] + fSlow60 * fRec42[1];
			fRec41[0] = fSlow46 * fRec42[0] + fSlow60 * fRec41[1];
			fRec40[0] = fSlow46 * fRec41[0] + fSlow60 * fRec40[1];
			fRec39[0] = -(fSlow105 * (fRec40[1] - fRec40[0]) + fSlow110 * (fSlow110 * fRec39[2] - fSlow111 * fRec39[1]));
			fRec38[0] = fSlow106 * fRec39[0] - fSlow112 * (fSlow112 * fRec38[2] - fSlow113 * fRec38[1]);
			fRec37[0] = fRec38[0] - fSlow114 * (fSlow114 * fRec37[2] - fSlow116 * fRec37[1]);
			fRec36[0] = fRec37[0] + fSlow120 * fRec37[2] - (fSlow121 * fRec37[1] + fSlow119 * (fSlow119 * fRec36[2] - fSlow116 * fRec36[1]));
			fRec35[0] = fSlow43 * fRec36[0] - fSlow42 * fRec35[1];
			fRec56[0] = fSlow134 * fTemp5 + fSlow60 * fRec56[1];
			fRec55[0] = fSlow46 * fRec56[0] + fSlow60 * fRec55[1];
			fRec54[0] = fSlow46 * fRec55[0] + fSlow60 * fRec54[1];
			fRec53[0] = fSlow46 * fRec54[0] + fSlow60 * fRec53[1];
			fRec52[0] = 0.5f * (fSlow43 * fRec53[0] + fSlow61 * fRec53[1]) - fSlow42 * fRec52[1];
			float fTemp16 = fRec52[0] + fRec28[0] * fRec46[1];
			fVec6[0] = fTemp16;
			fRec51[0] = fVec6[1] + fSlow63 * (fTemp16 - fRec51[1]);
			fRec50[0] = fRec51[1] + fSlow63 * (fRec51[0] - fRec50[1]);
			fRec49[IOTA0 & 8191] = fRec50[1] + fSlow63 * (fRec50[0] - fRec49[(IOTA0 - 1) & 8191]);
			float fTemp17 = fSlow133 * fRec49[(IOTA0 - iSlow137) & 8191];
			float fTemp18 = fSlow138 * fRec49[(IOTA0 - iSlow139) & 8191];
			float fTemp19 = fRec28[0] * (fRec52[0] + fRec45[1]);
			fVec7[0] = fTemp19;
			fRec59[0] = fVec7[1] + fSlow63 * (fTemp19 - fRec59[1]);
			fRec58[0] = fRec59[1] + fSlow63 * (fRec59[0] - fRec58[1]);
			fRec57[IOTA0 & 8191] = fRec58[1] + fSlow63 * (fRec58[0] - fRec57[(IOTA0 - 1) & 8191]);
			float fTemp20 = fSlow150 * fRec57[(IOTA0 - iSlow153) & 8191] + fSlow154 * fRec57[(IOTA0 - iSlow155) & 8191];
			float fTemp21 = fTemp17 + fTemp18 + fTemp20;
			fVec8[0] = fTemp21;
			fRec48[0] = fSlow12 * (2.0f * (fSlow13 * fTemp21 + fSlow30 * fVec8[1]) - fSlow29 * fRec48[1]);
			fRec45[0] = fRec48[0] + fTemp20;
			fRec46[0] = fRec48[0] + fTemp18 + fTemp17;
			float fRec47 = fTemp21;
			fRec60[0] = fSlow158 * fRec47 - fSlow160 * (fSlow161 * fRec60[2] - fSlow162 * fRec60[1]);
			fRec61[0] = fSlow158 * fRec14 - fSlow160 * (fSlow161 * fRec61[2] - fSlow162 * fRec61[1]);
			float fTemp22 = fRec14 + fRec35[0] + fRec47 + fSlow157 * (fRec60[0] - fRec60[2] + (fRec61[0] - fRec61[2]));
			fVec9[IOTA0 & 4095] = fTemp22;
			fVec10[IOTA0 & 4095] = fSlow1 * fRec10[0] * fTemp22;
			float fTemp23 = 0.3f * fVec10[(IOTA0 - iConst16) & 4095];
			fRec63[0] = -(fConst23 * (fConst24 * fRec63[1] - (fRec4[1] + fRec4[2])));
			fRec62[0] = fSlow170 * (fRec4[1] + fSlow171 * fRec63[0]) + fSlow172 * fRec62[1];
			fVec11[IOTA0 & 16383] = 0.35355338f * fRec62[0] + 1e-20f;
			float fTemp24 = fTemp23 + fVec11[(IOTA0 - iConst26) & 16383] - 0.6f * fRec8[1];
			fVec12[IOTA0 & 4095] = fTemp24;
			fRec8[0] = fVec12[(IOTA0 - iConst27) & 4095];
			float fRec9 = 0.6f * fTemp24;
			fRec67[0] = -(fConst23 * (fConst24 * fRec67[1] - (fRec0[1] + fRec0[2])));
			fRec66[0] = fSlow179 * (fRec0[1] + fSlow180 * fRec67[0]) + fSlow181 * fRec66[1];
			fVec13[IOTA0 & 16383] = 0.35355338f * fRec66[0] + 1e-20f;
			float fTemp25 = fVec13[(IOTA0 - iConst33) & 16383] + fTemp23 - 0.6f * fRec64[1];
			fVec14[IOTA0 & 2047] = fTemp25;
			fRec64[0] = fVec14[(IOTA0 - iConst34) & 2047];
			float fRec65 = 0.6f * fTemp25;
			fRec71[0] = -(fConst23 * (fConst24 * fRec71[1] - (fRec6[1] + fRec6[2])));
			fRec70[0] = fSlow188 * (fRec6[1] + fSlow189 * fRec71[0]) + fSlow190 * fRec70[1];
			fVec15[IOTA0 & 16383] = 0.35355338f * fRec70[0] + 1e-20f;
			float fTemp26 = fVec15[(IOTA0 - iConst40) & 16383] - (fTemp23 + 0.6f * fRec68[1]);
			fVec16[IOTA0 & 2047] = fTemp26;
			fRec68[0] = fVec16[(IOTA0 - iConst41) & 2047];
			float fRec69 = 0.6f * fTemp26;
			fRec75[0] = -(fConst23 * (fConst24 * fRec75[1] - (fRec2[1] + fRec2[2])));
			fRec74[0] = fSlow197 * (fRec2[1] + fSlow198 * fRec75[0]) + fSlow199 * fRec74[1];
			fVec17[IOTA0 & 16383] = 0.35355338f * fRec74[0] + 1e-20f;
			float fTemp27 = fVec17[(IOTA0 - iConst47) & 16383] - (fTemp23 + 0.6f * fRec72[1]);
			fVec18[IOTA0 & 4095] = fTemp27;
			fRec72[0] = fVec18[(IOTA0 - iConst48) & 4095];
			float fRec73 = 0.6f * fTemp27;
			fRec79[0] = -(fConst23 * (fConst24 * fRec79[1] - (fRec3[1] + fRec3[2])));
			fRec78[0] = fSlow206 * (fRec3[1] + fSlow207 * fRec79[0]) + fSlow208 * fRec78[1];
			fVec19[IOTA0 & 32767] = 0.35355338f * fRec78[0] + 1e-20f;
			float fTemp28 = 0.6f * fRec76[1] + fVec19[(IOTA0 - iConst54) & 32767];
			fVec20[IOTA0 & 4095] = fSlow209 * fRec10[0] * fVec9[(IOTA0 - iSlow210) & 4095];
			float fTemp29 = 0.3f * fVec20[(IOTA0 - iConst16) & 4095];
			fVec21[IOTA0 & 4095] = fTemp28 - fTemp29;
			fRec76[0] = fVec21[(IOTA0 - iConst56) & 4095];
			float fRec77 = 0.6f * (fTemp29 - fTemp28);
			fRec83[0] = -(fConst23 * (fConst24 * fRec83[1] - (fRec5[1] + fRec5[2])));
			fRec82[0] = fSlow217 * (fRec5[1] + fSlow218 * fRec83[0]) + fSlow219 * fRec82[1];
			fVec22[IOTA0 & 16383] = 0.35355338f * fRec82[0] + 1e-20f;
			float fTemp30 = 0.6f * fRec80[1] + fTemp29 + fVec22[(IOTA0 - iConst62) & 16383];
			fVec23[IOTA0 & 4095] = fTemp30;
			fRec80[0] = fVec23[(IOTA0 - iConst63) & 4095];
			float fRec81 = -(0.6f * fTemp30);
			fRec87[0] = -(fConst23 * (fConst24 * fRec87[1] - (fRec1[1] + fRec1[2])));
			fRec86[0] = fSlow226 * (fRec1[1] + fSlow227 * fRec87[0]) + fSlow228 * fRec86[1];
			fVec24[IOTA0 & 32767] = 0.35355338f * fRec86[0] + 1e-20f;
			float fTemp31 = 0.6f * fRec84[1] + fVec24[(IOTA0 - iConst69) & 32767] + fTemp29;
			fVec25[IOTA0 & 4095] = fTemp31;
			fRec84[0] = fVec25[(IOTA0 - iConst70) & 4095];
			float fRec85 = -(0.6f * fTemp31);
			fRec91[0] = -(fConst23 * (fConst24 * fRec91[1] - (fRec7[1] + fRec7[2])));
			fRec90[0] = fSlow235 * (fRec7[1] + fSlow236 * fRec91[0]) + fSlow237 * fRec90[1];
			fVec26[IOTA0 & 32767] = 0.35355338f * fRec90[0] + 1e-20f;
			float fTemp32 = 0.6f * fRec88[1] + fVec26[(IOTA0 - iConst76) & 32767];
			fVec27[IOTA0 & 2047] = fTemp32 - fTemp29;
			fRec88[0] = fVec27[(IOTA0 - iConst77) & 2047];
			float fRec89 = 0.6f * (fTemp29 - fTemp32);
			float fTemp33 = fRec64[1] + fRec72[1];
			float fTemp34 = fRec8[1] + fTemp33;
			fRec0[0] = fRec9 + fRec65 + fRec69 + fRec73 + fRec77 + fRec81 + fRec85 + fRec89 + fRec88[1] + fRec68[1] + fRec84[1] + fRec80[1] + fTemp34 + fRec76[1];
			fRec1[0] = fRec9 + fRec65 + fRec69 + fRec73 + fRec68[1] + fTemp34 - (fRec77 + fRec81 + fRec85 + fRec89 + fRec88[1] + fRec84[1] + fRec80[1] + fRec76[1]);
			float fTemp35 = fRec64[1] + fRec8[1];
			fRec2[0] = fRec9 + fRec65 + fRec81 + fRec85 + fRec84[1] + fTemp35 + fRec80[1] - (fRec69 + fRec73 + fRec77 + fRec89 + fRec88[1] + fRec68[1] + fRec72[1] + fRec76[1]);
			fRec3[0] = fRec9 + fRec65 + fRec77 + fRec89 + fRec88[1] + fTemp35 + fRec76[1] - (fRec69 + fRec73 + fRec81 + fRec85 + fRec68[1] + fRec84[1] + fRec72[1] + fRec80[1]);
			fRec4[0] = fRec65 + fRec73 + fRec77 + fRec85 + fRec84[1] + fTemp33 + fRec76[1] - (fRec9 + fRec69 + fRec81 + fRec89 + fRec88[1] + fRec68[1] + fRec8[1] + fRec80[1]);
			fRec5[0] = fRec65 + fRec73 + fRec81 + fRec89 + fRec88[1] + fTemp33 + fRec80[1] - (fRec9 + fRec69 + fRec77 + fRec85 + fRec68[1] + fRec84[1] + fRec8[1] + fRec76[1]);
			float fTemp36 = fRec8[1] + fRec72[1];
			fRec6[0] = fRec65 + fRec69 + fRec85 + fRec89 + fRec88[1] + fRec68[1] + fRec64[1] + fRec84[1] - (fRec9 + fRec73 + fRec77 + fRec81 + fRec80[1] + fRec76[1] + fTemp36);
			fRec7[0] = fRec65 + fRec69 + fRec77 + fRec81 + fRec68[1] + fRec80[1] + fRec64[1] + fRec76[1] - (fRec9 + fRec73 + fRec85 + fRec89 + fRec88[1] + fRec84[1] + fTemp36);
			float fTemp37 = 1.0f - fRec10[0];
			float fTemp38 = fRec14 + fRec35[0] + fRec47 + fSlow157 * (fRec60[0] + fRec61[0] - (fRec60[2] + fRec61[2]));
			fVec28[IOTA0 & 4095] = fTemp38;
			output0[i0] = FAUSTFLOAT(0.37f * (fRec1[0] + fRec2[0]) + fSlow1 * fTemp37 * fTemp38);
			output1[i0] = FAUSTFLOAT(0.37f * (fRec1[0] - fRec2[0]) + fSlow209 * fTemp37 * fVec28[(IOTA0 - iSlow210) & 4095]);
			fRec10[1] = fRec10[0];
			iRec24[1] = iRec24[0];
			iRec26[1] = iRec26[0];
			fRec25[1] = fRec25[0];
			fRec27[1] = fRec27[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec28[1] = fRec28[0];
			fVec0[1] = fVec0[0];
			fRec18[1] = fRec18[0];
			fRec17[1] = fRec17[0];
			IOTA0 = IOTA0 + 1;
			fVec1[1] = fVec1[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fVec2[1] = fVec2[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fVec3[1] = fVec3[0];
			fRec15[1] = fRec15[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fVec4[1] = fVec4[0];
			fVec5[1] = fVec5[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fVec6[1] = fVec6[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fVec7[1] = fVec7[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fVec8[1] = fVec8[0];
			fRec48[1] = fRec48[0];
			fRec45[1] = fRec45[0];
			fRec46[1] = fRec46[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec8[1] = fRec8[0];
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			fRec64[1] = fRec64[0];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec68[1] = fRec68[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec72[1] = fRec72[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec76[1] = fRec76[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec80[1] = fRec80[0];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec84[1] = fRec84[0];
			fRec91[1] = fRec91[0];
			fRec90[1] = fRec90[0];
			fRec88[1] = fRec88[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
		}
	}

};
#endif

class faust_synth_piano_tilde : public FaustExternal<synth_piano> {
public:
    faust_synth_piano_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(synth_piano))
    {
    }
};

#endif
