/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.piano"
version: "1.0"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn synth_piano -scn synth_piano_dsp -es 1 -mcd 16 -single -ftz 0
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
        virtual synth_piano_dsp* clone() = 0;
    
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
    
        virtual synth_piano_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_piano_dsp.h **************************/
/************************** BEGIN UI.h *****************************
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
 ********************************************************************/

#ifndef __UI_H__
#define __UI_H__


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct FAUST_API UIReal {
    
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* /*zone*/, const char* /*key*/, const char* /*val*/) {}

    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct FAUST_API UI : public UIReal<FAUSTFLOAT> {
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
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
	
	int fSampleRate;
	float fConst3;
	FAUSTFLOAT fHslider0;
	float fConst4;
	float fConst5;
	float fConst7;
	float fConst8;
	float fRec11[2];
	float fRec10[2];
	int IOTA0;
	float fVec0[32768];
	int iConst10;
	FAUSTFLOAT fHslider1;
	float fConst11;
	FAUSTFLOAT fHslider2;
	float fConst12;
	float fRec12[2];
	float fConst14;
	FAUSTFLOAT fHslider3;
	float fConst15;
	FAUSTFLOAT fHslider4;
	float fConst16;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fButton0;
	float fRec20[2];
	FAUSTFLOAT fHslider6;
	int iRec26[2];
	int iRec28[2];
	float fConst17;
	float fRec27[2];
	float fConst18;
	float fConst19;
	FAUSTFLOAT fHslider7;
	float fConst20;
	float fConst21;
	float fRec29[2];
	float fRec25[2];
	float fRec24[2];
	float fRec23[2];
	float fRec22[2];
	float fRec21[2];
	float fVec1[2];
	float fRec19[2];
	float fRec18[2];
	float fRec17[8192];
	float fVec2[2];
	float fRec33[2];
	float fRec32[2];
	float fRec31[8192];
	float fVec3[2];
	float fRec30[2];
	float fRec14[2];
	float fRec15[2];
	float fConst22;
	float fConst23;
	float fConst24;
	float fRec13[3];
	float fRec46[2];
	float fRec45[2];
	float fRec44[2];
	float fRec43[2];
	float fRec42[2];
	float fVec4[2];
	float fRec41[2];
	float fRec40[2];
	float fRec39[8192];
	float fConst25;
	float fVec5[2];
	float fRec50[2];
	float fRec49[2];
	float fRec48[8192];
	float fVec6[2];
	float fRec53[2];
	float fRec52[2];
	float fRec51[8192];
	float fVec7[2];
	float fRec47[2];
	float fRec35[2];
	float fRec36[2];
	float fRec37[2];
	float fRec34[3];
	float fConst26;
	float fVec8[2];
	float fVec9[2];
	float fRec63[2];
	float fRec62[2];
	float fRec61[2];
	float fRec60[2];
	float fRec59[2];
	float fRec58[3];
	float fRec57[3];
	float fRec56[3];
	float fRec55[3];
	float fRec54[2];
	float fVec10[4096];
	float fConst27;
	FAUSTFLOAT fHslider8;
	float fVec11[4096];
	int iConst28;
	float fVec12[4096];
	int iConst29;
	float fRec8[2];
	float fConst32;
	float fConst33;
	float fRec67[2];
	float fRec66[2];
	float fVec13[16384];
	int iConst35;
	float fVec14[4096];
	float fVec15[2048];
	int iConst36;
	float fRec64[2];
	float fConst39;
	float fConst40;
	float fRec71[2];
	float fRec70[2];
	float fVec16[16384];
	int iConst42;
	float fVec17[4096];
	int iConst43;
	float fRec68[2];
	float fConst46;
	float fConst47;
	float fRec75[2];
	float fRec74[2];
	float fVec18[16384];
	int iConst49;
	float fVec19[4096];
	int iConst50;
	float fRec72[2];
	float fConst53;
	float fConst54;
	float fRec79[2];
	float fRec78[2];
	float fVec20[32768];
	int iConst56;
	float fVec21[4096];
	int iConst57;
	float fRec76[2];
	float fConst60;
	float fConst61;
	float fRec83[2];
	float fRec82[2];
	float fVec22[16384];
	int iConst63;
	float fVec23[4096];
	int iConst64;
	float fRec80[2];
	float fConst67;
	float fConst68;
	float fRec87[2];
	float fRec86[2];
	float fVec24[32768];
	int iConst70;
	float fVec25[2048];
	int iConst71;
	float fRec84[2];
	float fConst74;
	float fConst75;
	float fRec91[2];
	float fRec90[2];
	float fVec26[16384];
	int iConst77;
	float fVec27[2048];
	int iConst78;
	float fRec88[2];
	float fRec0[3];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("ceammc_instruments.lib/name", "CEAMMC replacement for intruments.lib");
		m->declare("ceammc_instruments.lib/version", "0.3");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn synth_piano -scn synth_piano_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("copyright", "Romain Michon");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("description", "WaveGuide Commuted Piano");
		m->declare("filename", "synth_piano.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
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
		m->declare("filters.lib/version", "0.3");
		m->declare("instruments.lib/author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("instruments.lib/copyright", "Romain Michon");
		m->declare("instruments.lib/licence", "STK-4.3");
		m->declare("instruments.lib/name", "Faust-STK Tools Library");
		m->declare("instruments.lib/version", "1.0");
		m->declare("licence", "STK-4.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "synth.piano");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.4");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
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
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::floor(0.210389f * fConst0 + 0.5f);
		float fConst2 = (0.0f - 6.9077554f * fConst1) / fConst0;
		fConst3 = 0.25f * fConst2;
		fConst4 = std::cos(37699.113f / fConst0);
		fConst5 = 0.16666667f * fConst2;
		float fConst6 = 1.0f / std::tan(628.31854f / fConst0);
		fConst7 = 1.0f / (fConst6 + 1.0f);
		fConst8 = 1.0f - fConst6;
		float fConst9 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst10 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst1 - fConst9)));
		fConst11 = 44.1f / fConst0;
		fConst12 = 1.0f - fConst11;
		float fConst13 = synth_piano_faustpower2_f(fConst0);
		fConst14 = 9.68e+04f / fConst13;
		fConst15 = 0.15915494f * fConst0;
		fConst16 = 6.2831855f / fConst0;
		fConst17 = 7.0f / fConst0;
		fConst18 = std::exp(0.0f - 5.0f / fConst0);
		fConst19 = 1e+01f / fConst0;
		fConst20 = 0.1f * fConst0;
		fConst21 = std::exp(0.0f - 0.5f / fConst0);
		fConst22 = 8.8e+02f / fConst0;
		fConst23 = 2764.6016f / fConst0;
		fConst24 = 1.936e+05f / fConst13;
		fConst25 = 0.0003617158f * fConst0;
		fConst26 = 0.05f / fConst0;
		fConst27 = 0.0011363636f * fConst0;
		iConst28 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.02f * fConst0)));
		iConst29 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst9 + -1.0f)));
		float fConst30 = std::floor(0.125f * fConst0 + 0.5f);
		float fConst31 = (0.0f - 6.9077554f * fConst30) / fConst0;
		fConst32 = 0.25f * fConst31;
		fConst33 = 0.16666667f * fConst31;
		float fConst34 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst35 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst30 - fConst34)));
		iConst36 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst34 + -1.0f)));
		float fConst37 = std::floor(0.127837f * fConst0 + 0.5f);
		float fConst38 = (0.0f - 6.9077554f * fConst37) / fConst0;
		fConst39 = 0.25f * fConst38;
		fConst40 = 0.16666667f * fConst38;
		float fConst41 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst42 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst37 - fConst41)));
		iConst43 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst41 + -1.0f)));
		float fConst44 = std::floor(0.174713f * fConst0 + 0.5f);
		float fConst45 = (0.0f - 6.9077554f * fConst44) / fConst0;
		fConst46 = 0.25f * fConst45;
		fConst47 = 0.16666667f * fConst45;
		float fConst48 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst49 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst44 - fConst48)));
		iConst50 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst48 + -1.0f)));
		float fConst51 = std::floor(0.256891f * fConst0 + 0.5f);
		float fConst52 = (0.0f - 6.9077554f * fConst51) / fConst0;
		fConst53 = 0.25f * fConst52;
		fConst54 = 0.16666667f * fConst52;
		float fConst55 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst56 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst51 - fConst55)));
		iConst57 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst55 + -1.0f)));
		float fConst58 = std::floor(0.192303f * fConst0 + 0.5f);
		float fConst59 = (0.0f - 6.9077554f * fConst58) / fConst0;
		fConst60 = 0.25f * fConst59;
		fConst61 = 0.16666667f * fConst59;
		float fConst62 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst63 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst58 - fConst62)));
		iConst64 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst62 + -1.0f)));
		float fConst65 = std::floor(0.219991f * fConst0 + 0.5f);
		float fConst66 = (0.0f - 6.9077554f * fConst65) / fConst0;
		fConst67 = 0.25f * fConst66;
		fConst68 = 0.16666667f * fConst66;
		float fConst69 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst70 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst65 - fConst69)));
		iConst71 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst69 + -1.0f)));
		float fConst72 = std::floor(0.153129f * fConst0 + 0.5f);
		float fConst73 = (0.0f - 6.9077554f * fConst72) / fConst0;
		fConst74 = 0.25f * fConst73;
		fConst75 = 0.16666667f * fConst73;
		float fConst76 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst77 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst72 - fConst76)));
		iConst78 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst76 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.36f);
		fHslider1 = FAUSTFLOAT(0.6f);
		fHslider2 = FAUSTFLOAT(0.137f);
		fHslider3 = FAUSTFLOAT(48.0f);
		fHslider4 = FAUSTFLOAT(0.28f);
		fHslider5 = FAUSTFLOAT(0.1f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(0.1f);
		fHslider8 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec11[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec10[l1] = 0.0f;
		}
		IOTA0 = 0;
		for (int l2 = 0; l2 < 32768; l2 = l2 + 1) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec12[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec20[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec26[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iRec28[l6] = 0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec27[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec29[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec25[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec24[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec23[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec22[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec21[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec1[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec19[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec18[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 8192; l17 = l17 + 1) {
			fRec17[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fVec2[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec33[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec32[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 8192; l21 = l21 + 1) {
			fRec31[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec3[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec30[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec14[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec15[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec13[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec46[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec45[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec44[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec43[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec42[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fVec4[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec41[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec40[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 8192; l35 = l35 + 1) {
			fRec39[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fVec5[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec50[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec49[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 8192; l39 = l39 + 1) {
			fRec48[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fVec6[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec53[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec52[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 8192; l43 = l43 + 1) {
			fRec51[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fVec7[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec47[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec35[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec36[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec37[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = l49 + 1) {
			fRec34[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fVec8[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fVec9[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec63[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec62[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec61[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec60[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec59[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fRec58[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 3; l58 = l58 + 1) {
			fRec57[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec56[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec55[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec54[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 4096; l62 = l62 + 1) {
			fVec10[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 4096; l63 = l63 + 1) {
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
		for (int l69 = 0; l69 < 4096; l69 = l69 + 1) {
			fVec14[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2048; l70 = l70 + 1) {
			fVec15[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec64[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec71[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec70[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 16384; l74 = l74 + 1) {
			fVec16[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 4096; l75 = l75 + 1) {
			fVec17[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec68[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec75[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec74[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 16384; l79 = l79 + 1) {
			fVec18[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 4096; l80 = l80 + 1) {
			fVec19[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec72[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec79[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec78[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 32768; l84 = l84 + 1) {
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
		for (int l95 = 0; l95 < 2048; l95 = l95 + 1) {
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
		for (int l99 = 0; l99 < 16384; l99 = l99 + 1) {
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
		ui_interface->addHorizontalSlider("brightness", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("detuning", &fHslider5, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider7, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pan", &fHslider1, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pitch", &fHslider3, FAUSTFLOAT(48.0f), FAUSTFLOAT(21.0f), FAUSTFLOAT(108.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, FAUSTFLOAT(0.137f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("room_size", &fHslider0, FAUSTFLOAT(0.36f), FAUSTFLOAT(0.005f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("stiffness", &fHslider4, FAUSTFLOAT(0.28f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("width", &fHslider8, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::exp(fConst3 / fSlow0);
		float fSlow2 = synth_piano_faustpower2_f(fSlow1);
		float fSlow3 = 1.0f - fConst4 * fSlow2;
		float fSlow4 = 1.0f - fSlow2;
		float fSlow5 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow3) / synth_piano_faustpower2_f(fSlow4) + -1.0f));
		float fSlow6 = fSlow3 / fSlow4;
		float fSlow7 = fSlow1 * (fSlow5 + (1.0f - fSlow6));
		float fSlow8 = std::exp(fConst5 / fSlow0) / fSlow1 + -1.0f;
		float fSlow9 = fSlow6 - fSlow5;
		float fSlow10 = float(fHslider1);
		float fSlow11 = 12.0f * fSlow10;
		float fSlow12 = fConst11 * float(fHslider2);
		float fSlow13 = float(fHslider3);
		float fSlow14 = std::pow(2.0f, 0.083333336f * (fSlow13 + -69.0f));
		float fSlow15 = getValueEQBandWidthFactor(fSlow13);
		float fSlow16 = synth_piano_faustpower2_f(fSlow14) * synth_piano_faustpower2_f(fSlow15);
		float fSlow17 = 0.5f - fConst14 * fSlow16;
		float fSlow18 = getValueEQGain(fSlow13);
		float fSlow19 = float(fHslider4);
		float fSlow20 = getValueStiffnessCoefficient(fSlow13);
		float fSlow21 = 13.69f * synth_piano_faustpower2_f(fSlow19) * synth_piano_faustpower2_f(fSlow20);
		float fSlow22 = fSlow21 + -1.0f;
		float fSlow23 = 4.4e+02f * fSlow14;
		float fSlow24 = 5.0f * getValueDetuningHz(fSlow13) * float(fHslider5);
		float fSlow25 = fSlow23 + fSlow24;
		float fSlow26 = fConst16 * fSlow25;
		float fSlow27 = std::sin(fSlow26);
		float fSlow28 = fSlow19 * fSlow20;
		float fSlow29 = 7.4f * fSlow28;
		float fSlow30 = fSlow21 + 1.0f;
		float fSlow31 = std::cos(fSlow26);
		float fSlow32 = 3.0f * std::atan2(fSlow22 * fSlow27, fSlow29 + fSlow30 * fSlow31);
		float fSlow33 = getValueSingleStringZero(fSlow13);
		float fSlow34 = getValueSingleStringPole(fSlow13);
		float fSlow35 = 1.0f - fSlow34;
		float fSlow36 = std::pow(1e+01f, 0.000113636364f * (getValueSingleStringDecayRate(fSlow13) / fSlow14));
		float fSlow37 = fSlow33 * fSlow35 * fSlow36;
		float fSlow38 = 1.0f - fSlow33;
		float fSlow39 = fSlow34 * fSlow38;
		float fSlow40 = 3.0f * fSlow39;
		float fSlow41 = fSlow37 - fSlow40;
		float fSlow42 = fSlow39 - fSlow37;
		float fSlow43 = 4.0f * fSlow42;
		float fSlow44 = fSlow41 + fSlow43;
		float fSlow45 = fSlow35 * fSlow36;
		float fSlow46 = 3.0f * fSlow38 - fSlow45;
		float fSlow47 = fSlow41 * fSlow31 / fSlow46 + 1.0f;
		float fSlow48 = fSlow33 + fSlow45 + -1.0f;
		float fSlow49 = 4.0f * fSlow48;
		float fSlow50 = (fSlow49 + fSlow44 * fSlow31) / fSlow46 + 1.0f;
		float fSlow51 = fSlow41 * fSlow44;
		float fSlow52 = synth_piano_faustpower2_f(fSlow27);
		float fSlow53 = synth_piano_faustpower2_f(fSlow46);
		float fSlow54 = fConst15 * ((fSlow32 + std::atan2((0.0f - fSlow44 * fSlow27 / fSlow46) * fSlow47 + fSlow41 * fSlow27 * fSlow50 / fSlow46, fSlow47 * fSlow50 + fSlow51 * fSlow52 / fSlow53) + 6.2831855f) / fSlow25);
		float fSlow55 = std::floor(fSlow54);
		float fSlow56 = fSlow54 - fSlow55;
		float fSlow57 = float(fButton0);
		int iSlow58 = fSlow57 > 0.0f;
		float fSlow59 = float(iSlow58);
		float fSlow60 = float(0 - (iSlow58 + -1));
		float fSlow61 = fConst11 * (0.9996f * fSlow59 + 0.9f * getValueReleaseLoopGain(fSlow13) * fSlow60);
		float fSlow62 = getValueDCBa1(fSlow13);
		float fSlow63 = 1.0f - fSlow62;
		float fSlow64 = 0.5f * fSlow63;
		float fSlow65 = 0.25f * float(fHslider6);
		float fSlow66 = getValueLoudPole(fSlow13);
		float fSlow67 = getValueLoudGain(fSlow13) * (fSlow65 + (0.98f - fSlow66));
		float fSlow68 = 1.6298145e-09f * fSlow67 * float(fSlow13 < 45.0f);
		int iSlow69 = iSlow58 > 0;
		float fSlow70 = std::exp(0.0f - fConst17 / (getValueDryTapAmpT60(fSlow13) * std::min<float>(1.0f, std::max<float>(0.0f, fSlow57))));
		int iSlow71 = iSlow58 < 1;
		float fSlow72 = fConst18 * fSlow60;
		float fSlow73 = float(fHslider7);
		float fSlow74 = fSlow59 * std::exp(0.0f - fConst19 / fSlow73);
		float fSlow75 = fConst20 * fSlow73;
		float fSlow76 = 0.2f * getValueSustainPedalLevel(fSlow13);
		float fSlow77 = fSlow66 + (0.02f - fSlow65);
		float fSlow78 = 0.0f - fSlow64;
		float fSlow79 = 3.7f * fSlow28;
		float fSlow80 = fSlow37 + fSlow43 - fSlow40;
		float fSlow81 = (fSlow49 + fSlow80 * fSlow31) / fSlow46 + 1.0f;
		float fSlow82 = fSlow41 * fSlow80;
		int iSlow83 = int(fConst15 * ((fSlow32 + std::atan2(0.0f - fSlow27 * (fSlow80 * fSlow47 - fSlow41 * fSlow81) / fSlow46, fSlow47 * fSlow81 + fSlow82 * fSlow52 / fSlow53) + 6.2831855f) / fSlow25));
		int iSlow84 = std::min<int>(4097, std::max<int>(0, iSlow83 + 1));
		float fSlow85 = 1.0f / fSlow46;
		float fSlow86 = fSlow23 - fSlow24;
		float fSlow87 = fConst16 * fSlow86;
		float fSlow88 = std::sin(fSlow87);
		float fSlow89 = std::cos(fSlow87);
		float fSlow90 = 3.0f * std::atan2(fSlow22 * fSlow88, fSlow29 + fSlow30 * fSlow89);
		float fSlow91 = fSlow89 * fSlow41 / fSlow46 + 1.0f;
		float fSlow92 = (fSlow89 * fSlow44 + fSlow49) / fSlow46 + 1.0f;
		float fSlow93 = synth_piano_faustpower2_f(fSlow88) * fSlow41;
		float fSlow94 = fConst15 * ((fSlow90 + std::atan2((0.0f - fSlow88 * fSlow44 / fSlow46) * fSlow91 + fSlow88 * fSlow41 * fSlow92 / fSlow46, fSlow91 * fSlow92 + fSlow93 * fSlow44 / fSlow53) + 6.2831855f) / fSlow86);
		float fSlow95 = std::floor(fSlow94);
		float fSlow96 = fSlow94 - fSlow95;
		float fSlow97 = (fSlow49 + fSlow89 * fSlow80) / fSlow46 + 1.0f;
		int iSlow98 = int(fConst15 * ((fSlow90 + std::atan2(0.0f - fSlow88 * (fSlow91 * fSlow80 - fSlow41 * fSlow97) / fSlow46, fSlow91 * fSlow97 + fSlow93 * fSlow80 / fSlow53) + 6.2831855f) / fSlow86));
		int iSlow99 = std::min<int>(4097, std::max<int>(0, iSlow98 + 1));
		float fSlow100 = fSlow95 + (1.0f - fSlow94);
		int iSlow101 = std::min<int>(4097, std::max<int>(0, iSlow98));
		float fSlow102 = fSlow55 + (1.0f - fSlow54);
		int iSlow103 = std::min<int>(4097, std::max<int>(0, iSlow83));
		float fSlow104 = (0.0f - fConst22 * fSlow14 * fSlow15) * std::cos(fConst23 * (fSlow14 / getValueStrikePosition(fSlow13)));
		float fSlow105 = fConst24 * fSlow16;
		float fSlow106 = fSlow14 + std::pow(2.0f, 0.083333336f * (fSlow13 + -129.0f));
		float fSlow107 = 4.4e+02f * fSlow106;
		float fSlow108 = fSlow24 + fSlow107;
		float fSlow109 = fConst16 * fSlow108;
		float fSlow110 = std::sin(fSlow109);
		float fSlow111 = std::cos(fSlow109);
		float fSlow112 = 3.0f * std::atan2(fSlow22 * fSlow110, fSlow29 + fSlow30 * fSlow111);
		float fSlow113 = fSlow41 * fSlow111 / fSlow46 + 1.0f;
		float fSlow114 = (fSlow49 + fSlow44 * fSlow111) / fSlow46 + 1.0f;
		float fSlow115 = synth_piano_faustpower2_f(fSlow110);
		float fSlow116 = fConst15 * ((fSlow112 + std::atan2((0.0f - fSlow44 * fSlow110 / fSlow46) * fSlow113 + fSlow41 * fSlow110 * fSlow114 / fSlow46, fSlow113 * fSlow114 + fSlow51 * fSlow115 / fSlow53) + 6.2831855f) / fSlow108);
		float fSlow117 = std::floor(fSlow116);
		float fSlow118 = fSlow116 - fSlow117;
		float fSlow119 = 1.1641532e-09f * fSlow67 * float((fSlow13 >= 45.0f) & (fSlow13 < 88.0f));
		float fSlow120 = (fSlow49 + fSlow80 * fSlow111) / fSlow46 + 1.0f;
		int iSlow121 = int(fConst15 * ((fSlow112 + std::atan2(0.0f - fSlow110 * (fSlow80 * fSlow113 - fSlow41 * fSlow120) / fSlow46, fSlow113 * fSlow120 + fSlow82 * fSlow115 / fSlow53) + 6.2831855f) / fSlow108));
		int iSlow122 = std::min<int>(4097, std::max<int>(0, iSlow121 + 1));
		float fSlow123 = fConst23 * fSlow106;
		float fSlow124 = std::sin(fSlow123);
		float fSlow125 = std::cos(fSlow123);
		float fSlow126 = 3.0f * std::atan2(fSlow22 * fSlow124, fSlow29 + fSlow30 * fSlow125);
		float fSlow127 = fSlow41 * fSlow125 / fSlow46 + 1.0f;
		float fSlow128 = (fSlow49 + fSlow44 * fSlow125) / fSlow46 + 1.0f;
		float fSlow129 = synth_piano_faustpower2_f(fSlow124);
		float fSlow130 = fConst25 * ((fSlow126 + std::atan2((0.0f - fSlow44 * fSlow124 / fSlow46) * fSlow127 + fSlow41 * fSlow124 * fSlow128 / fSlow46, fSlow127 * fSlow128 + fSlow51 * fSlow129 / fSlow53) + 6.2831855f) / fSlow106);
		float fSlow131 = std::floor(fSlow130);
		float fSlow132 = fSlow130 - fSlow131;
		float fSlow133 = (fSlow49 + fSlow80 * fSlow125) / fSlow46 + 1.0f;
		int iSlow134 = int(fConst25 * ((fSlow126 + std::atan2(0.0f - fSlow124 * (fSlow80 * fSlow127 - fSlow41 * fSlow133) / fSlow46, fSlow127 * fSlow133 + fSlow82 * fSlow129 / fSlow53) + 6.2831855f) / fSlow106));
		int iSlow135 = std::min<int>(4097, std::max<int>(0, iSlow134 + 1));
		float fSlow136 = fSlow131 + (1.0f - fSlow130);
		int iSlow137 = std::min<int>(4097, std::max<int>(0, iSlow134));
		float fSlow138 = fSlow107 - fSlow24;
		float fSlow139 = fConst16 * fSlow138;
		float fSlow140 = std::sin(fSlow139);
		float fSlow141 = std::cos(fSlow139);
		float fSlow142 = 3.0f * std::atan2(fSlow22 * fSlow140, fSlow29 + fSlow30 * fSlow141);
		float fSlow143 = fSlow41 * fSlow141 / fSlow46 + 1.0f;
		float fSlow144 = (fSlow49 + fSlow44 * fSlow141) / fSlow46 + 1.0f;
		float fSlow145 = synth_piano_faustpower2_f(fSlow140);
		float fSlow146 = fConst15 * ((fSlow142 + std::atan2((0.0f - fSlow44 * fSlow140 / fSlow46) * fSlow143 + fSlow41 * fSlow140 * fSlow144 / fSlow46, fSlow143 * fSlow144 + fSlow51 * fSlow145 / fSlow53) + 6.2831855f) / fSlow138);
		float fSlow147 = std::floor(fSlow146);
		float fSlow148 = fSlow147 + (1.0f - fSlow146);
		float fSlow149 = (fSlow49 + fSlow80 * fSlow141) / fSlow46 + 1.0f;
		int iSlow150 = int(fConst15 * ((fSlow142 + std::atan2(0.0f - fSlow140 * (fSlow80 * fSlow143 - fSlow41 * fSlow149) / fSlow46, fSlow143 * fSlow149 + fSlow82 * fSlow145 / fSlow53) + 6.2831855f) / fSlow138));
		int iSlow151 = std::min<int>(4097, std::max<int>(0, iSlow150));
		float fSlow152 = fSlow146 - fSlow147;
		int iSlow153 = std::min<int>(4097, std::max<int>(0, iSlow150 + 1));
		float fSlow154 = fSlow117 + (1.0f - fSlow116);
		int iSlow155 = std::min<int>(4097, std::max<int>(0, iSlow121));
		float fSlow156 = std::cos(fConst23 * fSlow14);
		float fSlow157 = std::pow(1e+01f, 0.05f * getValueSecondStageAmpRatio(fSlow13));
		float fSlow158 = std::pow(1e+01f, fConst26 * getValuer1_1db(fSlow13));
		float fSlow159 = 1.0f - fSlow157;
		float fSlow160 = std::pow(1e+01f, fConst26 * getValuer1_2db(fSlow13));
		float fSlow161 = 0.0f - 2.0f * (fSlow157 * fSlow158 + fSlow159 * fSlow160);
		float fSlow162 = getValueBq4_gEarBalled(fSlow13);
		float fSlow163 = 2.0f * fSlow162;
		float fSlow164 = float(fSlow13 >= 88.0f);
		float fSlow165 = 2.3283064e-10f * fSlow164;
		float fSlow166 = 1.1641532e-10f * fSlow164;
		float fSlow167 = std::pow(1e+01f, fConst26 * getValuer3db(fSlow13));
		float fSlow168 = (0.0f - 2.0f * fSlow167) * std::cos(fConst23 * fSlow14 * getValueThirdPartialFactor(fSlow13));
		float fSlow169 = synth_piano_faustpower2_f(fSlow167);
		float fSlow170 = std::pow(1e+01f, fConst26 * getValuer2db(fSlow13));
		float fSlow171 = (0.0f - 2.0f * fSlow170) * std::cos(fConst23 * fSlow14 * getValueSecondPartialFactor(fSlow13));
		float fSlow172 = synth_piano_faustpower2_f(fSlow170);
		float fSlow173 = (0.0f - 2.0f * fSlow158) * fSlow156;
		float fSlow174 = synth_piano_faustpower2_f(fSlow158);
		float fSlow175 = 0.0f - 2.0f * fSlow160;
		float fSlow176 = synth_piano_faustpower2_f(fSlow160);
		float fSlow177 = fSlow157 * fSlow174 + fSlow159 * fSlow176;
		int iSlow178 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst27 * (float(fHslider8) / fSlow14))));
		float fSlow179 = std::exp(fConst32 / fSlow0);
		float fSlow180 = synth_piano_faustpower2_f(fSlow179);
		float fSlow181 = 1.0f - fConst4 * fSlow180;
		float fSlow182 = 1.0f - fSlow180;
		float fSlow183 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow181) / synth_piano_faustpower2_f(fSlow182) + -1.0f));
		float fSlow184 = fSlow181 / fSlow182;
		float fSlow185 = fSlow179 * (fSlow183 + (1.0f - fSlow184));
		float fSlow186 = std::exp(fConst33 / fSlow0) / fSlow179 + -1.0f;
		float fSlow187 = fSlow184 - fSlow183;
		float fSlow188 = 12.0f * (1.0f - fSlow10);
		float fSlow189 = std::exp(fConst39 / fSlow0);
		float fSlow190 = synth_piano_faustpower2_f(fSlow189);
		float fSlow191 = 1.0f - fConst4 * fSlow190;
		float fSlow192 = 1.0f - fSlow190;
		float fSlow193 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow191) / synth_piano_faustpower2_f(fSlow192) + -1.0f));
		float fSlow194 = fSlow191 / fSlow192;
		float fSlow195 = fSlow189 * (fSlow193 + (1.0f - fSlow194));
		float fSlow196 = std::exp(fConst40 / fSlow0) / fSlow189 + -1.0f;
		float fSlow197 = fSlow194 - fSlow193;
		float fSlow198 = std::exp(fConst46 / fSlow0);
		float fSlow199 = synth_piano_faustpower2_f(fSlow198);
		float fSlow200 = 1.0f - fConst4 * fSlow199;
		float fSlow201 = 1.0f - fSlow199;
		float fSlow202 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow200) / synth_piano_faustpower2_f(fSlow201) + -1.0f));
		float fSlow203 = fSlow200 / fSlow201;
		float fSlow204 = fSlow198 * (fSlow202 + (1.0f - fSlow203));
		float fSlow205 = std::exp(fConst47 / fSlow0) / fSlow198 + -1.0f;
		float fSlow206 = fSlow203 - fSlow202;
		float fSlow207 = std::exp(fConst53 / fSlow0);
		float fSlow208 = synth_piano_faustpower2_f(fSlow207);
		float fSlow209 = 1.0f - fConst4 * fSlow208;
		float fSlow210 = 1.0f - fSlow208;
		float fSlow211 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow209) / synth_piano_faustpower2_f(fSlow210) + -1.0f));
		float fSlow212 = fSlow209 / fSlow210;
		float fSlow213 = fSlow207 * (fSlow211 + (1.0f - fSlow212));
		float fSlow214 = std::exp(fConst54 / fSlow0) / fSlow207 + -1.0f;
		float fSlow215 = fSlow212 - fSlow211;
		float fSlow216 = std::exp(fConst60 / fSlow0);
		float fSlow217 = synth_piano_faustpower2_f(fSlow216);
		float fSlow218 = 1.0f - fConst4 * fSlow217;
		float fSlow219 = 1.0f - fSlow217;
		float fSlow220 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow218) / synth_piano_faustpower2_f(fSlow219) + -1.0f));
		float fSlow221 = fSlow218 / fSlow219;
		float fSlow222 = fSlow216 * (fSlow220 + (1.0f - fSlow221));
		float fSlow223 = std::exp(fConst61 / fSlow0) / fSlow216 + -1.0f;
		float fSlow224 = fSlow221 - fSlow220;
		float fSlow225 = std::exp(fConst67 / fSlow0);
		float fSlow226 = synth_piano_faustpower2_f(fSlow225);
		float fSlow227 = 1.0f - fConst4 * fSlow226;
		float fSlow228 = 1.0f - fSlow226;
		float fSlow229 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow227) / synth_piano_faustpower2_f(fSlow228) + -1.0f));
		float fSlow230 = fSlow227 / fSlow228;
		float fSlow231 = fSlow225 * (fSlow229 + (1.0f - fSlow230));
		float fSlow232 = std::exp(fConst68 / fSlow0) / fSlow225 + -1.0f;
		float fSlow233 = fSlow230 - fSlow229;
		float fSlow234 = std::exp(fConst74 / fSlow0);
		float fSlow235 = synth_piano_faustpower2_f(fSlow234);
		float fSlow236 = 1.0f - fConst4 * fSlow235;
		float fSlow237 = 1.0f - fSlow235;
		float fSlow238 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow236) / synth_piano_faustpower2_f(fSlow237) + -1.0f));
		float fSlow239 = fSlow236 / fSlow237;
		float fSlow240 = fSlow234 * (fSlow238 + (1.0f - fSlow239));
		float fSlow241 = std::exp(fConst75 / fSlow0) / fSlow234 + -1.0f;
		float fSlow242 = fSlow239 - fSlow238;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec11[0] = 0.0f - fConst7 * (fConst8 * fRec11[1] - (fRec1[1] + fRec1[2]));
			fRec10[0] = fSlow7 * (fRec1[1] + fSlow8 * fRec11[0]) + fSlow9 * fRec10[1];
			fVec0[IOTA0 & 32767] = 0.35355338f * fRec10[0] + 1e-20f;
			fRec12[0] = fSlow12 + fConst12 * fRec12[1];
			fRec20[0] = fSlow61 + fConst12 * fRec20[1];
			iRec26[0] = 1103515245 * iRec26[1] + 12345;
			iRec28[0] = iSlow58 * iRec28[1] + 1;
			float fTemp0 = float(iRec28[0] + -1);
			float fTemp1 = float((fTemp0 < 2.0f) & iSlow69);
			float fTemp2 = 0.030197384f * fTemp1 + fSlow70 * float((fTemp0 >= 2.0f) | iSlow71);
			fRec27[0] = fRec27[1] * fTemp2 + 0.15f * fTemp1 * (1.0f - fTemp2);
			int iTemp3 = fTemp0 < fSlow75;
			float fTemp4 = fSlow74 * float(iTemp3) + fConst21 * float(iSlow58 * (fTemp0 >= fSlow75));
			fRec29[0] = fRec29[1] * (fSlow72 + fTemp4) + fSlow76 * (1.0f - fTemp4 - fSlow72) * float(iTemp3 & iSlow69);
			float fTemp5 = float(iRec26[0]) * (fRec27[0] + fRec29[0]);
			fRec25[0] = fSlow68 * fTemp5 + fSlow77 * fRec25[1];
			fRec24[0] = fSlow67 * fRec25[0] + fSlow77 * fRec24[1];
			fRec23[0] = fSlow67 * fRec24[0] + fSlow77 * fRec23[1];
			fRec22[0] = fSlow67 * fRec23[0] + fSlow77 * fRec22[1];
			fRec21[0] = fSlow64 * fRec22[0] + fSlow78 * fRec22[1] - fSlow62 * fRec21[1];
			float fTemp6 = fRec20[0] * (fRec21[0] + fRec14[1]);
			fVec1[0] = fTemp6;
			fRec19[0] = fVec1[1] - fSlow79 * (fRec19[1] - fTemp6);
			fRec18[0] = fRec19[1] + fSlow79 * (fRec19[0] - fRec18[1]);
			fRec17[IOTA0 & 8191] = fRec18[1] + fSlow79 * (fRec18[0] - fRec17[(IOTA0 - 1) & 8191]);
			float fTemp7 = fSlow56 * fRec17[(IOTA0 - iSlow84) & 8191];
			float fTemp8 = fRec21[0] + fRec20[0] * fRec15[1];
			fVec2[0] = fTemp8;
			fRec33[0] = fVec2[1] + fSlow79 * (fTemp8 - fRec33[1]);
			fRec32[0] = fRec33[1] - fSlow79 * (fRec32[1] - fRec33[0]);
			fRec31[IOTA0 & 8191] = fRec32[1] + fSlow79 * (fRec32[0] - fRec31[(IOTA0 - 1) & 8191]);
			float fTemp9 = fSlow96 * fRec31[(IOTA0 - iSlow99) & 8191];
			float fTemp10 = fSlow100 * fRec31[(IOTA0 - iSlow101) & 8191];
			float fTemp11 = fSlow102 * fRec17[(IOTA0 - iSlow103) & 8191];
			float fTemp12 = fTemp9 + fTemp10 + fTemp11 + fTemp7;
			fVec3[0] = fTemp12;
			fRec30[0] = fSlow85 * (2.0f * (fSlow48 * fTemp12 + fSlow42 * fVec3[1]) - fSlow41 * fRec30[1]);
			fRec14[0] = fTemp7 + fRec30[0] + fTemp11;
			fRec15[0] = fTemp9 + fRec30[0] + fTemp10;
			float fRec16 = fTemp12;
			fRec13[0] = fSlow18 * fRec16 - (fSlow104 * fRec13[1] + fSlow105 * fRec13[2]);
			fRec46[0] = fSlow119 * fTemp5 + fSlow77 * fRec46[1];
			fRec45[0] = fSlow67 * fRec46[0] + fSlow77 * fRec45[1];
			fRec44[0] = fSlow67 * fRec45[0] + fSlow77 * fRec44[1];
			fRec43[0] = fSlow67 * fRec44[0] + fSlow77 * fRec43[1];
			fRec42[0] = fSlow64 * fRec43[0] + fSlow78 * fRec43[1] - fSlow62 * fRec42[1];
			float fTemp13 = fRec20[0] * (fRec42[0] + fRec35[1]);
			fVec4[0] = fTemp13;
			fRec41[0] = fVec4[1] + fSlow79 * (fTemp13 - fRec41[1]);
			fRec40[0] = fRec41[1] + fSlow79 * (fRec41[0] - fRec40[1]);
			fRec39[IOTA0 & 8191] = fRec40[1] + fSlow79 * (fRec40[0] - fRec39[(IOTA0 - 1) & 8191]);
			float fTemp14 = fSlow118 * fRec39[(IOTA0 - iSlow122) & 8191];
			float fTemp15 = fRec42[0] + fRec20[0] * fRec36[1];
			fVec5[0] = fTemp15;
			fRec50[0] = fVec5[1] + fSlow79 * (fTemp15 - fRec50[1]);
			fRec49[0] = fRec50[1] + fSlow79 * (fRec50[0] - fRec49[1]);
			fRec48[IOTA0 & 8191] = fRec49[1] + fSlow79 * (fRec49[0] - fRec48[(IOTA0 - 1) & 8191]);
			float fTemp16 = fSlow132 * fRec48[(IOTA0 - iSlow135) & 8191];
			float fTemp17 = fSlow136 * fRec48[(IOTA0 - iSlow137) & 8191];
			float fTemp18 = fRec42[0] + fRec20[0] * fRec37[1];
			fVec6[0] = fTemp18;
			fRec53[0] = fVec6[1] + fSlow79 * (fTemp18 - fRec53[1]);
			fRec52[0] = fRec53[1] + fSlow79 * (fRec53[0] - fRec52[1]);
			fRec51[IOTA0 & 8191] = fRec52[1] + fSlow79 * (fRec52[0] - fRec51[(IOTA0 - 1) & 8191]);
			float fTemp19 = fSlow148 * fRec51[(IOTA0 - iSlow151) & 8191] + fSlow152 * fRec51[(IOTA0 - iSlow153) & 8191];
			float fTemp20 = fSlow154 * fRec39[(IOTA0 - iSlow155) & 8191];
			float fTemp21 = fTemp14 + fTemp16 + fTemp17 + fTemp19 + fTemp20;
			fVec7[0] = fTemp21;
			fRec47[0] = fSlow85 * (2.0f * (fSlow48 * fTemp21 + fSlow42 * fVec7[1]) - fSlow41 * fRec47[1]);
			fRec35[0] = fTemp14 + fRec47[0] + fTemp20;
			fRec36[0] = fTemp16 + fRec47[0] + fTemp17;
			fRec37[0] = fRec47[0] + fTemp19;
			float fRec38 = fTemp21;
			fRec34[0] = fSlow18 * fRec38 - (fSlow104 * fRec34[1] + fSlow105 * fRec34[2]);
			fVec8[0] = fSlow165 * fTemp5;
			float fTemp22 = 0.0f - (0.5f * fVec8[1] + fSlow166 * fTemp5);
			fVec9[0] = fTemp22;
			fRec63[0] = fSlow64 * fTemp22 + fSlow78 * fVec9[1] - fSlow62 * fRec63[1];
			fRec62[0] = fSlow67 * fRec63[0] + fSlow77 * fRec62[1];
			fRec61[0] = fSlow67 * fRec62[0] + fSlow77 * fRec61[1];
			fRec60[0] = fSlow67 * fRec61[0] + fSlow77 * fRec60[1];
			fRec59[0] = fSlow67 * fRec60[0] + fSlow77 * fRec59[1];
			fRec58[0] = fSlow162 * (fRec59[0] - fRec59[1]) - (fSlow168 * fRec58[1] + fSlow169 * fRec58[2]);
			fRec57[0] = fSlow163 * fRec58[0] - (fSlow171 * fRec57[1] + fSlow172 * fRec57[2]);
			fRec56[0] = fRec57[0] - (fSlow173 * fRec56[1] + fSlow174 * fRec56[2]);
			fRec55[0] = fSlow156 * (fSlow161 * fRec56[1] - fSlow175 * fRec55[1]) + fRec56[0] + fSlow177 * fRec56[2] - fSlow176 * fRec55[2];
			fRec54[0] = fSlow63 * fRec55[0] - fSlow62 * fRec54[1];
			float fTemp23 = fSlow17 * (fRec13[0] - fRec13[2] + (fRec34[0] - fRec34[2])) + fRec54[0] + fRec16 + fRec38;
			fVec10[IOTA0 & 4095] = fTemp23;
			float fTemp24 = fVec10[(IOTA0 - iSlow178) & 4095];
			fVec11[IOTA0 & 4095] = fSlow11 * fRec12[0] * fTemp24;
			float fTemp25 = 0.3f * fVec11[(IOTA0 - iConst28) & 4095];
			float fTemp26 = 0.6f * fRec8[1] + fVec0[(IOTA0 - iConst10) & 32767] + fTemp25;
			fVec12[IOTA0 & 4095] = fTemp26;
			fRec8[0] = fVec12[(IOTA0 - iConst29) & 4095];
			float fRec9 = 0.0f - 0.6f * fTemp26;
			fRec67[0] = 0.0f - fConst7 * (fConst8 * fRec67[1] - (fRec6[1] + fRec6[2]));
			fRec66[0] = fSlow185 * (fRec6[1] + fSlow186 * fRec67[0]) + fSlow187 * fRec66[1];
			fVec13[IOTA0 & 16383] = 0.35355338f * fRec66[0] + 1e-20f;
			fVec14[IOTA0 & 4095] = fSlow188 * fRec12[0] * fTemp23;
			float fTemp27 = 0.3f * fVec14[(IOTA0 - iConst28) & 4095];
			float fTemp28 = fVec13[(IOTA0 - iConst35) & 16383] - (fTemp27 + 0.6f * fRec64[1]);
			fVec15[IOTA0 & 2047] = fTemp28;
			fRec64[0] = fVec15[(IOTA0 - iConst36) & 2047];
			float fRec65 = 0.6f * fTemp28;
			fRec71[0] = 0.0f - fConst7 * (fConst8 * fRec71[1] - (fRec2[1] + fRec2[2]));
			fRec70[0] = fSlow195 * (fRec2[1] + fSlow196 * fRec71[0]) + fSlow197 * fRec70[1];
			fVec16[IOTA0 & 16383] = 0.35355338f * fRec70[0] + 1e-20f;
			float fTemp29 = fVec16[(IOTA0 - iConst42) & 16383] - (fTemp27 + 0.6f * fRec68[1]);
			fVec17[IOTA0 & 4095] = fTemp29;
			fRec68[0] = fVec17[(IOTA0 - iConst43) & 4095];
			float fRec69 = 0.6f * fTemp29;
			fRec75[0] = 0.0f - fConst7 * (fConst8 * fRec75[1] - (fRec4[1] + fRec4[2]));
			fRec74[0] = fSlow204 * (fRec4[1] + fSlow205 * fRec75[0]) + fSlow206 * fRec74[1];
			fVec18[IOTA0 & 16383] = 0.35355338f * fRec74[0] + 1e-20f;
			float fTemp30 = fTemp27 + fVec18[(IOTA0 - iConst49) & 16383] - 0.6f * fRec72[1];
			fVec19[IOTA0 & 4095] = fTemp30;
			fRec72[0] = fVec19[(IOTA0 - iConst50) & 4095];
			float fRec73 = 0.6f * fTemp30;
			fRec79[0] = 0.0f - fConst7 * (fConst8 * fRec79[1] - (fRec3[1] + fRec3[2]));
			fRec78[0] = fSlow213 * (fRec3[1] + fSlow214 * fRec79[0]) + fSlow215 * fRec78[1];
			fVec20[IOTA0 & 32767] = 0.35355338f * fRec78[0] + 1e-20f;
			float fTemp31 = 0.6f * fRec76[1] + fVec20[(IOTA0 - iConst56) & 32767] - fTemp25;
			fVec21[IOTA0 & 4095] = fTemp31;
			fRec76[0] = fVec21[(IOTA0 - iConst57) & 4095];
			float fRec77 = 0.0f - 0.6f * fTemp31;
			fRec83[0] = 0.0f - fConst7 * (fConst8 * fRec83[1] - (fRec5[1] + fRec5[2]));
			fRec82[0] = fSlow222 * (fRec5[1] + fSlow223 * fRec83[0]) + fSlow224 * fRec82[1];
			fVec22[IOTA0 & 16383] = 0.35355338f * fRec82[0] + 1e-20f;
			float fTemp32 = fVec22[(IOTA0 - iConst63) & 16383] + fTemp25 + 0.6f * fRec80[1];
			fVec23[IOTA0 & 4095] = fTemp32;
			fRec80[0] = fVec23[(IOTA0 - iConst64) & 4095];
			float fRec81 = 0.0f - 0.6f * fTemp32;
			fRec87[0] = 0.0f - fConst7 * (fConst8 * fRec87[1] - (fRec7[1] + fRec7[2]));
			fRec86[0] = fSlow231 * (fRec7[1] + fSlow232 * fRec87[0]) + fSlow233 * fRec86[1];
			fVec24[IOTA0 & 32767] = 0.35355338f * fRec86[0] + 1e-20f;
			float fTemp33 = 0.6f * fRec84[1] + fVec24[(IOTA0 - iConst70) & 32767] - fTemp25;
			fVec25[IOTA0 & 2047] = fTemp33;
			fRec84[0] = fVec25[(IOTA0 - iConst71) & 2047];
			float fRec85 = 0.0f - 0.6f * fTemp33;
			fRec91[0] = 0.0f - fConst7 * (fConst8 * fRec91[1] - (fRec0[1] + fRec0[2]));
			fRec90[0] = fSlow240 * (fRec0[1] + fSlow241 * fRec91[0]) + fSlow242 * fRec90[1];
			fVec26[IOTA0 & 16383] = 0.35355338f * fRec90[0] + 1e-20f;
			float fTemp34 = fVec26[(IOTA0 - iConst77) & 16383] + fTemp27 - 0.6f * fRec88[1];
			fVec27[IOTA0 & 2047] = fTemp34;
			fRec88[0] = fVec27[(IOTA0 - iConst78) & 2047];
			float fRec89 = 0.6f * fTemp34;
			float fTemp35 = fRec73 + fRec88[1];
			float fTemp36 = fRec69 + fTemp35;
			fRec0[0] = fRec8[1] + fRec64[1] + fRec68[1] + fRec72[1] + fRec76[1] + fRec80[1] + fRec85 + fRec77 + fRec81 + fRec9 + fRec65 + fRec89 + fTemp36 + fRec84[1];
			fRec1[0] = fRec64[1] + fRec68[1] + fRec72[1] + fRec65 + fRec89 + fTemp36 - (fRec8[1] + fRec76[1] + fRec80[1] + fRec85 + fRec77 + fRec81 + fRec9 + fRec84[1]);
			fRec2[0] = fRec8[1] + fRec72[1] + fRec80[1] + fRec81 + fRec9 + fRec89 + fTemp35 - (fRec64[1] + fRec68[1] + fRec76[1] + fRec85 + fRec77 + fRec65 + fRec69 + fRec84[1]);
			fRec3[0] = fRec72[1] + fRec76[1] + fRec85 + fRec77 + fRec89 + fTemp35 + fRec84[1] - (fRec8[1] + fRec64[1] + fRec68[1] + fRec80[1] + fRec81 + fRec9 + fRec69 + fRec65);
			float fTemp37 = fRec69 + fRec88[1];
			fRec4[0] = fRec8[1] + fRec68[1] + fRec76[1] + fRec77 + fRec9 + fRec89 + fTemp37 - (fRec64[1] + fRec72[1] + fRec80[1] + fRec85 + fRec81 + fRec65 + fRec73 + fRec84[1]);
			fRec5[0] = fRec68[1] + fRec80[1] + fRec85 + fRec81 + fRec89 + fRec84[1] + fTemp37 - (fRec8[1] + fRec64[1] + fRec72[1] + fRec76[1] + fRec77 + fRec9 + fRec73 + fRec65);
			float fTemp38 = fRec73 + fRec69;
			fRec6[0] = fRec8[1] + fRec64[1] + fRec85 + fRec9 + fRec65 + fRec89 + fRec88[1] + fRec84[1] - (fRec68[1] + fRec72[1] + fRec76[1] + fRec80[1] + fRec77 + fRec81 + fTemp38);
			fRec7[0] = fRec64[1] + fRec76[1] + fRec80[1] + fRec77 + fRec81 + fRec65 + fRec89 + fRec88[1] - (fRec8[1] + fRec68[1] + fRec72[1] + fRec85 + fRec9 + fRec84[1] + fTemp38);
			float fTemp39 = 1.0f - fRec12[0];
			output0[i0] = FAUSTFLOAT(0.37f * (fRec1[0] + fRec2[0]) + fSlow188 * fTemp39 * fTemp23);
			output1[i0] = FAUSTFLOAT(0.37f * (fRec1[0] - fRec2[0]) + fSlow11 * fTemp39 * fTemp24);
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			IOTA0 = IOTA0 + 1;
			fRec12[1] = fRec12[0];
			fRec20[1] = fRec20[0];
			iRec26[1] = iRec26[0];
			iRec28[1] = iRec28[0];
			fRec27[1] = fRec27[0];
			fRec29[1] = fRec29[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fVec1[1] = fVec1[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fVec2[1] = fVec2[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fVec3[1] = fVec3[0];
			fRec30[1] = fRec30[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fVec4[1] = fVec4[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fVec5[1] = fVec5[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fVec6[1] = fVec6[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fVec7[1] = fVec7[0];
			fRec47[1] = fRec47[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fVec8[1] = fVec8[0];
			fVec9[1] = fVec9[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
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
// clang-format on
#endif

    template <class T>
    struct _synth_piano_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_piano_UI<T>::name(sym(synth_piano));

typedef _synth_piano_UI<synth_piano> synth_piano_UI;

class faust_synth_piano_tilde : public FaustExternal<synth_piano, synth_piano_UI> {
public:
    faust_synth_piano_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
