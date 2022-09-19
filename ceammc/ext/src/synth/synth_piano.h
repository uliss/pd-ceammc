/* ------------------------------------------------------------
author: "Romain Michon (rmichon@ccrma.stanford.edu)"
copyright: "Romain Michon"
name: "synth.piano"
version: "1.0"
Code generated with Faust 2.44.1 (https://faust.grame.fr)
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
 ************************************************************************
    FAUST compiler
    Copyright (C) 2003-2018 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 ************************************************************************
 ************************************************************************/

#ifndef __export__
#define __export__

#define FAUSTVERSION "2.44.1"

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
    virtual void begin(size_t count) {}
    
    /**
     * Give the Memory Manager information on a given memory zone.
     * @param size - the size in bytes of the memory zone
     * @param reads - the number of Read access to the zone used to compute one frame
     * @param writes - the number of Write access to the zone used to compute one frame
     */
    virtual void info(size_t size, size_t reads, size_t writes) {}
    
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
    
        intptr_t fpsr;
        
        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
           asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            _mm_setcsr(static_cast<uint32_t>(fpsr_aux));
        #endif
        }
        
        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined ( __SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }
    
    public:
    
        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #else
            #if defined(__SSE__)
            #if defined(__SSE2__)
                intptr_t mask = 0x8040;
            #else
                intptr_t mask = 0x8000;
            #endif
            #else
                intptr_t mask = 0x0000;
            #endif
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }
        
        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals();

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
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
    
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
	float fVec0[16384];
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
	float fVec5[2];
	float fRec50[2];
	float fRec49[2];
	float fRec48[8192];
	float fConst25;
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
	float fVec11[4096];
	int iConst27;
	float fVec12[2048];
	int iConst28;
	float fRec8[2];
	float fConst31;
	float fConst32;
	float fRec67[2];
	float fRec66[2];
	float fVec13[32768];
	int iConst34;
	float fConst35;
	FAUSTFLOAT fHslider8;
	float fVec14[4096];
	float fVec15[2048];
	int iConst36;
	float fRec64[2];
	float fConst39;
	float fConst40;
	float fRec71[2];
	float fRec70[2];
	float fVec16[32768];
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
	float fVec23[2048];
	int iConst64;
	float fRec80[2];
	float fConst67;
	float fConst68;
	float fRec87[2];
	float fRec86[2];
	float fVec24[16384];
	int iConst70;
	float fVec25[4096];
	int iConst71;
	float fRec84[2];
	float fConst74;
	float fConst75;
	float fRec91[2];
	float fRec90[2];
	float fVec26[16384];
	int iConst77;
	float fVec27[4096];
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
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::floor(0.153128996f * fConst0 + 0.5f);
		float fConst2 = (0.0f - 6.90775537f * fConst1) / fConst0;
		fConst3 = 0.25f * fConst2;
		fConst4 = std::cos(37699.1133f / fConst0);
		fConst5 = 0.166666672f * fConst2;
		float fConst6 = 1.0f / std::tan(628.318542f / fConst0);
		fConst7 = 1.0f / (fConst6 + 1.0f);
		fConst8 = 1.0f - fConst6;
		float fConst9 = std::floor(0.0203460008f * fConst0 + 0.5f);
		iConst10 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst1 - fConst9)));
		fConst11 = 44.0999985f / fConst0;
		fConst12 = 1.0f - fConst11;
		float fConst13 = synth_piano_faustpower2_f(fConst0);
		fConst14 = 96800.0f / fConst13;
		fConst15 = 0.159154937f * fConst0;
		fConst16 = 6.28318548f / fConst0;
		fConst17 = 7.0f / fConst0;
		fConst18 = std::exp(0.0f - 5.0f / fConst0);
		fConst19 = 10.0f / fConst0;
		fConst20 = 0.100000001f * fConst0;
		fConst21 = std::exp(0.0f - 0.5f / fConst0);
		fConst22 = 880.0f / fConst0;
		fConst23 = 2764.60156f / fConst0;
		fConst24 = 193600.0f / fConst13;
		fConst25 = 0.000361715793f * fConst0;
		fConst26 = 0.0500000007f / fConst0;
		iConst27 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0199999996f * fConst0)));
		iConst28 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst9 + -1.0f)));
		float fConst29 = std::floor(0.219990999f * fConst0 + 0.5f);
		float fConst30 = (0.0f - 6.90775537f * fConst29) / fConst0;
		fConst31 = 0.25f * fConst30;
		fConst32 = 0.166666672f * fConst30;
		float fConst33 = std::floor(0.0191229992f * fConst0 + 0.5f);
		iConst34 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst29 - fConst33)));
		fConst35 = 0.0011363636f * fConst0;
		iConst36 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst33 + -1.0f)));
		float fConst37 = std::floor(0.256891012f * fConst0 + 0.5f);
		float fConst38 = (0.0f - 6.90775537f * fConst37) / fConst0;
		fConst39 = 0.25f * fConst38;
		fConst40 = 0.166666672f * fConst38;
		float fConst41 = std::floor(0.0273330007f * fConst0 + 0.5f);
		iConst42 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst37 - fConst41)));
		iConst43 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst41 + -1.0f)));
		float fConst44 = std::floor(0.192303002f * fConst0 + 0.5f);
		float fConst45 = (0.0f - 6.90775537f * fConst44) / fConst0;
		fConst46 = 0.25f * fConst45;
		fConst47 = 0.166666672f * fConst45;
		float fConst48 = std::floor(0.0292910002f * fConst0 + 0.5f);
		iConst49 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst44 - fConst48)));
		iConst50 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst48 + -1.0f)));
		float fConst51 = std::floor(0.210389003f * fConst0 + 0.5f);
		float fConst52 = (0.0f - 6.90775537f * fConst51) / fConst0;
		fConst53 = 0.25f * fConst52;
		fConst54 = 0.166666672f * fConst52;
		float fConst55 = std::floor(0.0244210009f * fConst0 + 0.5f);
		iConst56 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst51 - fConst55)));
		iConst57 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst55 + -1.0f)));
		float fConst58 = std::floor(0.125f * fConst0 + 0.5f);
		float fConst59 = (0.0f - 6.90775537f * fConst58) / fConst0;
		fConst60 = 0.25f * fConst59;
		fConst61 = 0.166666672f * fConst59;
		float fConst62 = std::floor(0.0134579996f * fConst0 + 0.5f);
		iConst63 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst58 - fConst62)));
		iConst64 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst62 + -1.0f)));
		float fConst65 = std::floor(0.127837002f * fConst0 + 0.5f);
		float fConst66 = (0.0f - 6.90775537f * fConst65) / fConst0;
		fConst67 = 0.25f * fConst66;
		fConst68 = 0.166666672f * fConst66;
		float fConst69 = std::floor(0.0316039994f * fConst0 + 0.5f);
		iConst70 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst65 - fConst69)));
		iConst71 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst69 + -1.0f)));
		float fConst72 = std::floor(0.174713001f * fConst0 + 0.5f);
		float fConst73 = (0.0f - 6.90775537f * fConst72) / fConst0;
		fConst74 = 0.25f * fConst73;
		fConst75 = 0.166666672f * fConst73;
		float fConst76 = std::floor(0.0229039993f * fConst0 + 0.5f);
		iConst77 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst72 - fConst76)));
		iConst78 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst76 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.35999999999999999f);
		fHslider1 = FAUSTFLOAT(0.59999999999999998f);
		fHslider2 = FAUSTFLOAT(0.13700000000000001f);
		fHslider3 = FAUSTFLOAT(48.0f);
		fHslider4 = FAUSTFLOAT(0.28000000000000003f);
		fHslider5 = FAUSTFLOAT(0.10000000000000001f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(0.10000000000000001f);
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
		for (int l2 = 0; l2 < 16384; l2 = l2 + 1) {
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
		for (int l64 = 0; l64 < 2048; l64 = l64 + 1) {
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
		for (int l68 = 0; l68 < 32768; l68 = l68 + 1) {
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
		for (int l74 = 0; l74 < 32768; l74 = l74 + 1) {
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
		for (int l90 = 0; l90 < 2048; l90 = l90 + 1) {
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
		for (int l94 = 0; l94 < 16384; l94 = l94 + 1) {
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
		for (int l99 = 0; l99 < 16384; l99 = l99 + 1) {
			fVec26[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 4096; l100 = l100 + 1) {
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
		ui_interface->addHorizontalSlider("brightness", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("detuning", &fHslider5, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("hammer_hardness", &fHslider7, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("pan", &fHslider1, FAUSTFLOAT(0.600000024f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("pitch", &fHslider3, FAUSTFLOAT(48.0f), FAUSTFLOAT(21.0f), FAUSTFLOAT(108.0f), FAUSTFLOAT(0.00100000005f));
		ui_interface->addHorizontalSlider("reverb_gain", &fHslider2, FAUSTFLOAT(0.136999995f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("room_size", &fHslider0, FAUSTFLOAT(0.360000014f), FAUSTFLOAT(0.00499999989f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("stiffness", &fHslider4, FAUSTFLOAT(0.280000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("width", &fHslider8, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
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
		float fSlow7 = fSlow1 * (fSlow5 + 1.0f - fSlow6);
		float fSlow8 = std::exp(fConst5 / fSlow0) / fSlow1 + -1.0f;
		float fSlow9 = fSlow6 - fSlow5;
		float fSlow10 = float(fHslider1);
		float fSlow11 = 12.0f * (1.0f - fSlow10);
		float fSlow12 = fConst11 * float(fHslider2);
		float fSlow13 = float(fHslider3);
		float fSlow14 = std::pow(2.0f, 0.0833333358f * (fSlow13 + -69.0f));
		float fSlow15 = float(getValueEQBandWidthFactor(float(fSlow13)));
		float fSlow16 = synth_piano_faustpower2_f(fSlow14) * synth_piano_faustpower2_f(fSlow15);
		float fSlow17 = 0.5f - fConst14 * fSlow16;
		float fSlow18 = float(getValueEQGain(float(fSlow13)));
		float fSlow19 = float(fHslider4);
		float fSlow20 = float(getValueStiffnessCoefficient(float(fSlow13)));
		float fSlow21 = 13.6899996f * synth_piano_faustpower2_f(fSlow19) * synth_piano_faustpower2_f(fSlow20);
		float fSlow22 = fSlow21 + -1.0f;
		float fSlow23 = 440.0f * fSlow14;
		float fSlow24 = 5.0f * float(getValueDetuningHz(float(fSlow13))) * float(fHslider5);
		float fSlow25 = fSlow23 + fSlow24;
		float fSlow26 = fConst16 * fSlow25;
		float fSlow27 = std::sin(fSlow26);
		float fSlow28 = fSlow19 * fSlow20;
		float fSlow29 = 7.4000001f * fSlow28;
		float fSlow30 = fSlow21 + 1.0f;
		float fSlow31 = std::cos(fSlow26);
		float fSlow32 = 3.0f * std::atan2(fSlow22 * fSlow27, fSlow29 + fSlow30 * fSlow31);
		float fSlow33 = float(getValueSingleStringZero(float(fSlow13)));
		float fSlow34 = std::pow(10.0f, 0.000113636364f * float(getValueSingleStringDecayRate(float(fSlow13))) / fSlow14);
		float fSlow35 = float(getValueSingleStringPole(float(fSlow13)));
		float fSlow36 = 1.0f - fSlow35;
		float fSlow37 = fSlow33 * fSlow34 * fSlow36;
		float fSlow38 = 1.0f - fSlow33;
		float fSlow39 = fSlow35 * fSlow38;
		float fSlow40 = 3.0f * fSlow39;
		float fSlow41 = fSlow37 - fSlow40;
		float fSlow42 = fSlow39 - fSlow37;
		float fSlow43 = 4.0f * fSlow42;
		float fSlow44 = fSlow41 + fSlow43;
		float fSlow45 = fSlow34 * fSlow36;
		float fSlow46 = 3.0f * fSlow38 - fSlow45;
		float fSlow47 = (fSlow31 * fSlow41) / fSlow46 + 1.0f;
		float fSlow48 = fSlow33 + fSlow45 + -1.0f;
		float fSlow49 = 4.0f * fSlow48;
		float fSlow50 = (fSlow49 + fSlow31 * fSlow44) / fSlow46 + 1.0f;
		float fSlow51 = synth_piano_faustpower2_f(fSlow27) * fSlow41;
		float fSlow52 = synth_piano_faustpower2_f(fSlow46);
		float fSlow53 = fConst15 * (fSlow32 + std::atan2((0.0f - (fSlow27 * fSlow44) / fSlow46) * fSlow47 + (fSlow27 * fSlow41 * fSlow50) / fSlow46, fSlow47 * fSlow50 + (fSlow51 * fSlow44) / fSlow52) + 6.28318548f) / fSlow25;
		float fSlow54 = std::floor(fSlow53);
		float fSlow55 = fSlow53 - fSlow54;
		float fSlow56 = float(fButton0);
		int iSlow57 = fSlow56 > 0.0f;
		float fSlow58 = float(iSlow57);
		float fSlow59 = float(0 - (iSlow57 + -1));
		float fSlow60 = fConst11 * (0.999599993f * fSlow58 + 0.899999976f * float(getValueReleaseLoopGain(float(fSlow13))) * fSlow59);
		float fSlow61 = float(getValueDCBa1(float(fSlow13)));
		float fSlow62 = 1.0f - fSlow61;
		float fSlow63 = 0.5f * fSlow62;
		float fSlow64 = 0.25f * float(fHslider6);
		float fSlow65 = float(getValueLoudPole(float(fSlow13)));
		float fSlow66 = float(getValueLoudGain(float(fSlow13))) * (fSlow64 + 0.980000019f - fSlow65);
		float fSlow67 = 1.62981451e-09f * fSlow66 * float(fSlow13 < 45.0f);
		int iSlow68 = iSlow57 > 0;
		float fSlow69 = std::exp(0.0f - fConst17 / (float(getValueDryTapAmpT60(float(fSlow13))) * std::min<float>(1.0f, std::max<float>(0.0f, fSlow56))));
		int iSlow70 = iSlow57 < 1;
		float fSlow71 = fConst18 * fSlow59;
		float fSlow72 = float(fHslider7);
		float fSlow73 = fSlow58 * std::exp(0.0f - fConst19 / fSlow72);
		float fSlow74 = fConst20 * fSlow72;
		float fSlow75 = 0.200000003f * float(getValueSustainPedalLevel(float(fSlow13)));
		float fSlow76 = fSlow65 + 0.0199999996f - fSlow64;
		float fSlow77 = 0.0f - fSlow63;
		float fSlow78 = 3.70000005f * fSlow28;
		float fSlow79 = (fSlow37 + fSlow43) - fSlow40;
		float fSlow80 = (fSlow49 + fSlow31 * fSlow79) / fSlow46 + 1.0f;
		int iSlow81 = int(fConst15 * (fSlow32 + std::atan2(0.0f - (fSlow27 * (fSlow79 * fSlow47 - fSlow41 * fSlow80)) / fSlow46, fSlow47 * fSlow80 + (fSlow51 * fSlow79) / fSlow52) + 6.28318548f) / fSlow25);
		int iSlow82 = std::min<int>(4097, std::max<int>(0, iSlow81 + 1));
		float fSlow83 = 1.0f / fSlow46;
		float fSlow84 = fSlow23 - fSlow24;
		float fSlow85 = fConst16 * fSlow84;
		float fSlow86 = std::sin(fSlow85);
		float fSlow87 = std::cos(fSlow85);
		float fSlow88 = 3.0f * std::atan2(fSlow22 * fSlow86, fSlow29 + fSlow30 * fSlow87);
		float fSlow89 = (fSlow87 * fSlow41) / fSlow46 + 1.0f;
		float fSlow90 = (fSlow87 * fSlow44 + fSlow49) / fSlow46 + 1.0f;
		float fSlow91 = synth_piano_faustpower2_f(fSlow86) * fSlow41;
		float fSlow92 = fConst15 * (fSlow88 + std::atan2((0.0f - (fSlow86 * fSlow44) / fSlow46) * fSlow89 + (fSlow86 * fSlow41 * fSlow90) / fSlow46, fSlow89 * fSlow90 + (fSlow91 * fSlow44) / fSlow52) + 6.28318548f) / fSlow84;
		float fSlow93 = std::floor(fSlow92);
		float fSlow94 = fSlow93 + 1.0f - fSlow92;
		float fSlow95 = (fSlow49 + fSlow87 * fSlow79) / fSlow46 + 1.0f;
		int iSlow96 = int(fConst15 * (fSlow88 + std::atan2(0.0f - (fSlow86 * (fSlow89 * fSlow79 - fSlow41 * fSlow95)) / fSlow46, fSlow89 * fSlow95 + (fSlow91 * fSlow79) / fSlow52) + 6.28318548f) / fSlow84);
		int iSlow97 = std::min<int>(4097, std::max<int>(0, iSlow96));
		float fSlow98 = fSlow92 - fSlow93;
		int iSlow99 = std::min<int>(4097, std::max<int>(0, iSlow96 + 1));
		float fSlow100 = fSlow54 + 1.0f - fSlow53;
		int iSlow101 = std::min<int>(4097, std::max<int>(0, iSlow81));
		float fSlow102 = (0.0f - fConst22 * fSlow14 * fSlow15) * std::cos(fConst23 * fSlow14 / float(getValueStrikePosition(float(fSlow13))));
		float fSlow103 = fConst24 * fSlow16;
		float fSlow104 = fSlow14 + std::pow(2.0f, 0.0833333358f * (fSlow13 + -129.0f));
		float fSlow105 = 440.0f * fSlow104;
		float fSlow106 = fSlow24 + fSlow105;
		float fSlow107 = fConst16 * fSlow106;
		float fSlow108 = std::sin(fSlow107);
		float fSlow109 = std::cos(fSlow107);
		float fSlow110 = 3.0f * std::atan2(fSlow22 * fSlow108, fSlow29 + fSlow30 * fSlow109);
		float fSlow111 = (fSlow109 * fSlow41) / fSlow46 + 1.0f;
		float fSlow112 = (fSlow49 + fSlow109 * fSlow44) / fSlow46 + 1.0f;
		float fSlow113 = synth_piano_faustpower2_f(fSlow108) * fSlow41;
		float fSlow114 = fConst15 * (fSlow110 + std::atan2((0.0f - (fSlow108 * fSlow44) / fSlow46) * fSlow111 + (fSlow108 * fSlow41 * fSlow112) / fSlow46, fSlow111 * fSlow112 + (fSlow113 * fSlow44) / fSlow52) + 6.28318548f) / fSlow106;
		float fSlow115 = std::floor(fSlow114);
		float fSlow116 = fSlow114 - fSlow115;
		float fSlow117 = 1.16415322e-09f * fSlow66 * float((fSlow13 >= 45.0f) & (fSlow13 < 88.0f));
		float fSlow118 = (fSlow49 + fSlow109 * fSlow79) / fSlow46 + 1.0f;
		int iSlow119 = int(fConst15 * (fSlow110 + std::atan2(0.0f - (fSlow108 * (fSlow79 * fSlow111 - fSlow41 * fSlow118)) / fSlow46, fSlow111 * fSlow118 + (fSlow113 * fSlow79) / fSlow52) + 6.28318548f) / fSlow106);
		int iSlow120 = std::min<int>(4097, std::max<int>(0, iSlow119 + 1));
		float fSlow121 = fSlow105 - fSlow24;
		float fSlow122 = fConst16 * fSlow121;
		float fSlow123 = std::sin(fSlow122);
		float fSlow124 = std::cos(fSlow122);
		float fSlow125 = 3.0f * std::atan2(fSlow22 * fSlow123, fSlow29 + fSlow30 * fSlow124);
		float fSlow126 = (fSlow124 * fSlow41) / fSlow46 + 1.0f;
		float fSlow127 = (fSlow49 + fSlow124 * fSlow44) / fSlow46 + 1.0f;
		float fSlow128 = synth_piano_faustpower2_f(fSlow123) * fSlow41;
		float fSlow129 = fConst15 * (fSlow125 + std::atan2((0.0f - (fSlow123 * fSlow44) / fSlow46) * fSlow126 + (fSlow123 * fSlow41 * fSlow127) / fSlow46, fSlow126 * fSlow127 + (fSlow128 * fSlow44) / fSlow52) + 6.28318548f) / fSlow121;
		float fSlow130 = std::floor(fSlow129);
		float fSlow131 = fSlow129 - fSlow130;
		float fSlow132 = (fSlow49 + fSlow124 * fSlow79) / fSlow46 + 1.0f;
		int iSlow133 = int(fConst15 * (fSlow125 + std::atan2(0.0f - (fSlow123 * (fSlow79 * fSlow126 - fSlow41 * fSlow132)) / fSlow46, fSlow126 * fSlow132 + (fSlow128 * fSlow79) / fSlow52) + 6.28318548f) / fSlow121);
		int iSlow134 = std::min<int>(4097, std::max<int>(0, iSlow133 + 1));
		float fSlow135 = fConst23 * fSlow104;
		float fSlow136 = std::sin(fSlow135);
		float fSlow137 = std::cos(fSlow135);
		float fSlow138 = 3.0f * std::atan2(fSlow22 * fSlow136, fSlow29 + fSlow30 * fSlow137);
		float fSlow139 = (fSlow137 * fSlow41) / fSlow46 + 1.0f;
		float fSlow140 = (fSlow49 + fSlow137 * fSlow44) / fSlow46 + 1.0f;
		float fSlow141 = synth_piano_faustpower2_f(fSlow136) * fSlow41;
		float fSlow142 = fConst25 * (fSlow138 + std::atan2((0.0f - (fSlow136 * fSlow44) / fSlow46) * fSlow139 + (fSlow136 * fSlow41 * fSlow140) / fSlow46, fSlow139 * fSlow140 + (fSlow141 * fSlow44) / fSlow52) + 6.28318548f) / fSlow104;
		float fSlow143 = std::floor(fSlow142);
		float fSlow144 = fSlow143 + 1.0f - fSlow142;
		float fSlow145 = (fSlow49 + fSlow137 * fSlow79) / fSlow46 + 1.0f;
		int iSlow146 = int(fConst25 * (fSlow138 + std::atan2(0.0f - (fSlow136 * (fSlow79 * fSlow139 - fSlow41 * fSlow145)) / fSlow46, fSlow139 * fSlow145 + (fSlow141 * fSlow79) / fSlow52) + 6.28318548f) / fSlow104);
		int iSlow147 = std::min<int>(4097, std::max<int>(0, iSlow146));
		float fSlow148 = fSlow142 - fSlow143;
		int iSlow149 = std::min<int>(4097, std::max<int>(0, iSlow146 + 1));
		float fSlow150 = fSlow130 + 1.0f - fSlow129;
		int iSlow151 = std::min<int>(4097, std::max<int>(0, iSlow133));
		float fSlow152 = fSlow115 + 1.0f - fSlow114;
		int iSlow153 = std::min<int>(4097, std::max<int>(0, iSlow119));
		float fSlow154 = std::cos(fConst23 * fSlow14);
		float fSlow155 = std::pow(10.0f, 0.0500000007f * float(getValueSecondStageAmpRatio(float(fSlow13))));
		float fSlow156 = std::pow(10.0f, fConst26 * float(getValuer1_1db(float(fSlow13))));
		float fSlow157 = std::pow(10.0f, fConst26 * float(getValuer1_2db(float(fSlow13))));
		float fSlow158 = 1.0f - fSlow155;
		float fSlow159 = 0.0f - 2.0f * (fSlow155 * fSlow156 + fSlow157 * fSlow158);
		float fSlow160 = float(getValueBq4_gEarBalled(float(fSlow13)));
		float fSlow161 = 2.0f * fSlow160;
		float fSlow162 = float(fSlow13 >= 88.0f);
		float fSlow163 = 2.32830644e-10f * fSlow162;
		float fSlow164 = 1.16415322e-10f * fSlow162;
		float fSlow165 = std::pow(10.0f, fConst26 * float(getValuer3db(float(fSlow13))));
		float fSlow166 = std::cos(fConst23 * fSlow14 * float(getValueThirdPartialFactor(float(fSlow13)))) * (0.0f - 2.0f * fSlow165);
		float fSlow167 = synth_piano_faustpower2_f(fSlow165);
		float fSlow168 = std::pow(10.0f, fConst26 * float(getValuer2db(float(fSlow13))));
		float fSlow169 = std::cos(fConst23 * fSlow14 * float(getValueSecondPartialFactor(float(fSlow13)))) * (0.0f - 2.0f * fSlow168);
		float fSlow170 = synth_piano_faustpower2_f(fSlow168);
		float fSlow171 = fSlow154 * (0.0f - 2.0f * fSlow156);
		float fSlow172 = synth_piano_faustpower2_f(fSlow156);
		float fSlow173 = 0.0f - 2.0f * fSlow157;
		float fSlow174 = synth_piano_faustpower2_f(fSlow157);
		float fSlow175 = fSlow155 * fSlow172 + fSlow174 * fSlow158;
		float fSlow176 = std::exp(fConst31 / fSlow0);
		float fSlow177 = synth_piano_faustpower2_f(fSlow176);
		float fSlow178 = 1.0f - fConst4 * fSlow177;
		float fSlow179 = 1.0f - fSlow177;
		float fSlow180 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow178) / synth_piano_faustpower2_f(fSlow179) + -1.0f));
		float fSlow181 = fSlow178 / fSlow179;
		float fSlow182 = fSlow176 * (fSlow180 + 1.0f - fSlow181);
		float fSlow183 = std::exp(fConst32 / fSlow0) / fSlow176 + -1.0f;
		float fSlow184 = fSlow181 - fSlow180;
		float fSlow185 = 12.0f * fSlow10;
		int iSlow186 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst35 * float(fHslider8) / fSlow14)));
		float fSlow187 = std::exp(fConst39 / fSlow0);
		float fSlow188 = synth_piano_faustpower2_f(fSlow187);
		float fSlow189 = 1.0f - fConst4 * fSlow188;
		float fSlow190 = 1.0f - fSlow188;
		float fSlow191 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow189) / synth_piano_faustpower2_f(fSlow190) + -1.0f));
		float fSlow192 = fSlow189 / fSlow190;
		float fSlow193 = fSlow187 * (fSlow191 + 1.0f - fSlow192);
		float fSlow194 = std::exp(fConst40 / fSlow0) / fSlow187 + -1.0f;
		float fSlow195 = fSlow192 - fSlow191;
		float fSlow196 = std::exp(fConst46 / fSlow0);
		float fSlow197 = synth_piano_faustpower2_f(fSlow196);
		float fSlow198 = 1.0f - fConst4 * fSlow197;
		float fSlow199 = 1.0f - fSlow197;
		float fSlow200 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow198) / synth_piano_faustpower2_f(fSlow199) + -1.0f));
		float fSlow201 = fSlow198 / fSlow199;
		float fSlow202 = fSlow196 * (fSlow200 + 1.0f - fSlow201);
		float fSlow203 = std::exp(fConst47 / fSlow0) / fSlow196 + -1.0f;
		float fSlow204 = fSlow201 - fSlow200;
		float fSlow205 = std::exp(fConst53 / fSlow0);
		float fSlow206 = synth_piano_faustpower2_f(fSlow205);
		float fSlow207 = 1.0f - fConst4 * fSlow206;
		float fSlow208 = 1.0f - fSlow206;
		float fSlow209 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow207) / synth_piano_faustpower2_f(fSlow208) + -1.0f));
		float fSlow210 = fSlow207 / fSlow208;
		float fSlow211 = fSlow205 * (fSlow209 + 1.0f - fSlow210);
		float fSlow212 = std::exp(fConst54 / fSlow0) / fSlow205 + -1.0f;
		float fSlow213 = fSlow210 - fSlow209;
		float fSlow214 = std::exp(fConst60 / fSlow0);
		float fSlow215 = synth_piano_faustpower2_f(fSlow214);
		float fSlow216 = 1.0f - fConst4 * fSlow215;
		float fSlow217 = 1.0f - fSlow215;
		float fSlow218 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow216) / synth_piano_faustpower2_f(fSlow217) + -1.0f));
		float fSlow219 = fSlow216 / fSlow217;
		float fSlow220 = fSlow214 * (fSlow218 + 1.0f - fSlow219);
		float fSlow221 = std::exp(fConst61 / fSlow0) / fSlow214 + -1.0f;
		float fSlow222 = fSlow219 - fSlow218;
		float fSlow223 = std::exp(fConst67 / fSlow0);
		float fSlow224 = synth_piano_faustpower2_f(fSlow223);
		float fSlow225 = 1.0f - fConst4 * fSlow224;
		float fSlow226 = 1.0f - fSlow224;
		float fSlow227 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow225) / synth_piano_faustpower2_f(fSlow226) + -1.0f));
		float fSlow228 = fSlow225 / fSlow226;
		float fSlow229 = fSlow223 * (fSlow227 + 1.0f - fSlow228);
		float fSlow230 = std::exp(fConst68 / fSlow0) / fSlow223 + -1.0f;
		float fSlow231 = fSlow228 - fSlow227;
		float fSlow232 = std::exp(fConst74 / fSlow0);
		float fSlow233 = synth_piano_faustpower2_f(fSlow232);
		float fSlow234 = 1.0f - fConst4 * fSlow233;
		float fSlow235 = 1.0f - fSlow233;
		float fSlow236 = std::sqrt(std::max<float>(0.0f, synth_piano_faustpower2_f(fSlow234) / synth_piano_faustpower2_f(fSlow235) + -1.0f));
		float fSlow237 = fSlow234 / fSlow235;
		float fSlow238 = fSlow232 * (fSlow236 + 1.0f - fSlow237);
		float fSlow239 = std::exp(fConst75 / fSlow0) / fSlow232 + -1.0f;
		float fSlow240 = fSlow237 - fSlow236;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec11[0] = 0.0f - fConst7 * (fConst8 * fRec11[1] - (fRec0[1] + fRec0[2]));
			fRec10[0] = fSlow7 * (fRec0[1] + fSlow8 * fRec11[0]) + fSlow9 * fRec10[1];
			fVec0[IOTA0 & 16383] = 0.353553385f * fRec10[0] + 9.99999968e-21f;
			fRec12[0] = fSlow12 + fConst12 * fRec12[1];
			fRec20[0] = fSlow60 + fConst12 * fRec20[1];
			iRec26[0] = 1103515245 * iRec26[1] + 12345;
			iRec28[0] = iSlow57 * iRec28[1] + 1;
			float fTemp0 = float(iRec28[0] + -1);
			float fTemp1 = float((fTemp0 < 2.0f) & iSlow68);
			float fTemp2 = 0.0301973838f * fTemp1 + fSlow69 * float((fTemp0 >= 2.0f) | iSlow70);
			fRec27[0] = fRec27[1] * fTemp2 + 0.150000006f * fTemp1 * (1.0f - fTemp2);
			int iTemp3 = fTemp0 < fSlow74;
			float fTemp4 = fSlow73 * float(iTemp3) + fConst21 * float(iSlow57 * (fTemp0 >= fSlow74));
			fRec29[0] = fRec29[1] * (fSlow71 + fTemp4) + fSlow75 * (1.0f - fTemp4 - fSlow71) * float(iTemp3 & iSlow68);
			float fTemp5 = float(iRec26[0]) * (fRec27[0] + fRec29[0]);
			fRec25[0] = fSlow67 * fTemp5 + fSlow76 * fRec25[1];
			fRec24[0] = fSlow66 * fRec25[0] + fSlow76 * fRec24[1];
			fRec23[0] = fSlow66 * fRec24[0] + fSlow76 * fRec23[1];
			fRec22[0] = fSlow66 * fRec23[0] + fSlow76 * fRec22[1];
			fRec21[0] = (fSlow63 * fRec22[0] + fSlow77 * fRec22[1]) - fSlow61 * fRec21[1];
			float fTemp6 = fRec20[0] * (fRec21[0] + fRec14[1]);
			fVec1[0] = fTemp6;
			fRec19[0] = fVec1[1] + fSlow78 * (fTemp6 - fRec19[1]);
			fRec18[0] = fRec19[1] + fSlow78 * (fRec19[0] - fRec18[1]);
			fRec17[IOTA0 & 8191] = fRec18[1] + fSlow78 * (fRec18[0] - fRec17[(IOTA0 - 1) & 8191]);
			float fTemp7 = fSlow55 * fRec17[(IOTA0 - iSlow82) & 8191];
			float fTemp8 = fRec21[0] + fRec20[0] * fRec15[1];
			fVec2[0] = fTemp8;
			fRec33[0] = fVec2[1] + fSlow78 * (fTemp8 - fRec33[1]);
			fRec32[0] = fRec33[1] + fSlow78 * (fRec33[0] - fRec32[1]);
			fRec31[IOTA0 & 8191] = fRec32[1] + fSlow78 * (fRec32[0] - fRec31[(IOTA0 - 1) & 8191]);
			float fTemp9 = fSlow94 * fRec31[(IOTA0 - iSlow97) & 8191];
			float fTemp10 = fSlow98 * fRec31[(IOTA0 - iSlow99) & 8191];
			float fTemp11 = fSlow100 * fRec17[(IOTA0 - iSlow101) & 8191];
			float fTemp12 = fTemp7 + fTemp9 + fTemp10 + fTemp11;
			fVec3[0] = fTemp12;
			fRec30[0] = fSlow83 * (2.0f * (fSlow48 * fTemp12 + fSlow42 * fVec3[1]) - fSlow41 * fRec30[1]);
			fRec14[0] = fTemp7 + fRec30[0] + fTemp11;
			fRec15[0] = fTemp10 + fRec30[0] + fTemp9;
			float fRec16 = fTemp12;
			fRec13[0] = fSlow18 * fRec16 - (fSlow102 * fRec13[1] + fSlow103 * fRec13[2]);
			fRec46[0] = fSlow117 * fTemp5 + fSlow76 * fRec46[1];
			fRec45[0] = fSlow66 * fRec46[0] + fSlow76 * fRec45[1];
			fRec44[0] = fSlow66 * fRec45[0] + fSlow76 * fRec44[1];
			fRec43[0] = fSlow66 * fRec44[0] + fSlow76 * fRec43[1];
			fRec42[0] = (fSlow63 * fRec43[0] + fSlow77 * fRec43[1]) - fSlow61 * fRec42[1];
			float fTemp13 = fRec20[0] * (fRec42[0] + fRec35[1]);
			fVec4[0] = fTemp13;
			fRec41[0] = fVec4[1] + fSlow78 * (fTemp13 - fRec41[1]);
			fRec40[0] = fRec41[1] + fSlow78 * (fRec41[0] - fRec40[1]);
			fRec39[IOTA0 & 8191] = fRec40[1] + fSlow78 * (fRec40[0] - fRec39[(IOTA0 - 1) & 8191]);
			float fTemp14 = fSlow116 * fRec39[(IOTA0 - iSlow120) & 8191];
			float fTemp15 = fRec42[0] + fRec20[0] * fRec37[1];
			fVec5[0] = fTemp15;
			fRec50[0] = fVec5[1] + fSlow78 * (fTemp15 - fRec50[1]);
			fRec49[0] = fRec50[1] + fSlow78 * (fRec50[0] - fRec49[1]);
			fRec48[IOTA0 & 8191] = fRec49[1] + fSlow78 * (fRec49[0] - fRec48[(IOTA0 - 1) & 8191]);
			float fTemp16 = fSlow131 * fRec48[(IOTA0 - iSlow134) & 8191];
			float fTemp17 = fRec42[0] + fRec20[0] * fRec36[1];
			fVec6[0] = fTemp17;
			fRec53[0] = fVec6[1] + fSlow78 * (fTemp17 - fRec53[1]);
			fRec52[0] = fRec53[1] + fSlow78 * (fRec53[0] - fRec52[1]);
			fRec51[IOTA0 & 8191] = fRec52[1] + fSlow78 * (fRec52[0] - fRec51[(IOTA0 - 1) & 8191]);
			float fTemp18 = fSlow144 * fRec51[(IOTA0 - iSlow147) & 8191];
			float fTemp19 = fSlow148 * fRec51[(IOTA0 - iSlow149) & 8191];
			float fTemp20 = fSlow150 * fRec48[(IOTA0 - iSlow151) & 8191];
			float fTemp21 = fSlow152 * fRec39[(IOTA0 - iSlow153) & 8191];
			float fTemp22 = fTemp14 + fTemp16 + fTemp18 + fTemp19 + fTemp20 + fTemp21;
			fVec7[0] = fTemp22;
			fRec47[0] = fSlow83 * (2.0f * (fSlow48 * fTemp22 + fSlow42 * fVec7[1]) - fSlow41 * fRec47[1]);
			fRec35[0] = fTemp14 + fRec47[0] + fTemp21;
			fRec36[0] = fTemp19 + fRec47[0] + fTemp18;
			fRec37[0] = fTemp16 + fRec47[0] + fTemp20;
			float fRec38 = fTemp22;
			fRec34[0] = fSlow18 * fRec38 - (fSlow102 * fRec34[1] + fSlow103 * fRec34[2]);
			fVec8[0] = fSlow163 * fTemp5;
			float fTemp23 = 0.0f - (0.5f * fVec8[1] + fSlow164 * fTemp5);
			fVec9[0] = fTemp23;
			fRec63[0] = (fSlow63 * fTemp23 + fSlow77 * fVec9[1]) - fSlow61 * fRec63[1];
			fRec62[0] = fSlow66 * fRec63[0] + fSlow76 * fRec62[1];
			fRec61[0] = fSlow66 * fRec62[0] + fSlow76 * fRec61[1];
			fRec60[0] = fSlow66 * fRec61[0] + fSlow76 * fRec60[1];
			fRec59[0] = fSlow66 * fRec60[0] + fSlow76 * fRec59[1];
			fRec58[0] = fSlow160 * (fRec59[0] - fRec59[1]) - (fSlow166 * fRec58[1] + fSlow167 * fRec58[2]);
			fRec57[0] = fSlow161 * fRec58[0] - (fSlow169 * fRec57[1] + fSlow170 * fRec57[2]);
			fRec56[0] = fRec57[0] - (fSlow171 * fRec56[1] + fSlow172 * fRec56[2]);
			fRec55[0] = (fSlow154 * (fSlow159 * fRec56[1] - fSlow173 * fRec55[1]) + fRec56[0] + fSlow175 * fRec56[2]) - fSlow174 * fRec55[2];
			fRec54[0] = fSlow62 * fRec55[0] - fSlow61 * fRec54[1];
			float fTemp24 = fSlow17 * (fRec13[0] - fRec13[2] + fRec34[0] - fRec34[2]) + fRec54[0] + fRec16 + fRec38;
			fVec10[IOTA0 & 4095] = fTemp24;
			fVec11[IOTA0 & 4095] = fSlow11 * fRec12[0] * fTemp24;
			float fTemp25 = 0.300000012f * fVec11[(IOTA0 - iConst27) & 4095];
			float fTemp26 = (fVec0[(IOTA0 - iConst10) & 16383] + fTemp25) - 0.600000024f * fRec8[1];
			fVec12[IOTA0 & 2047] = fTemp26;
			fRec8[0] = fVec12[(IOTA0 - iConst28) & 2047];
			float fRec9 = 0.600000024f * fTemp26;
			fRec67[0] = 0.0f - fConst7 * (fConst8 * fRec67[1] - (fRec7[1] + fRec7[2]));
			fRec66[0] = fSlow182 * (fRec7[1] + fSlow183 * fRec67[0]) + fSlow184 * fRec66[1];
			fVec13[IOTA0 & 32767] = 0.353553385f * fRec66[0] + 9.99999968e-21f;
			float fTemp27 = fVec10[(IOTA0 - iSlow186) & 4095];
			fVec14[IOTA0 & 4095] = fSlow185 * fRec12[0] * fTemp27;
			float fTemp28 = 0.300000012f * fVec14[(IOTA0 - iConst27) & 4095];
			float fTemp29 = (0.600000024f * fRec64[1] + fVec13[(IOTA0 - iConst34) & 32767]) - fTemp28;
			fVec15[IOTA0 & 2047] = fTemp29;
			fRec64[0] = fVec15[(IOTA0 - iConst36) & 2047];
			float fRec65 = 0.0f - 0.600000024f * fTemp29;
			fRec71[0] = 0.0f - fConst7 * (fConst8 * fRec71[1] - (fRec3[1] + fRec3[2]));
			fRec70[0] = fSlow193 * (fRec3[1] + fSlow194 * fRec71[0]) + fSlow195 * fRec70[1];
			fVec16[IOTA0 & 32767] = 0.353553385f * fRec70[0] + 9.99999968e-21f;
			float fTemp30 = (0.600000024f * fRec68[1] + fVec16[(IOTA0 - iConst42) & 32767]) - fTemp28;
			fVec17[IOTA0 & 4095] = fTemp30;
			fRec68[0] = fVec17[(IOTA0 - iConst43) & 4095];
			float fRec69 = 0.0f - 0.600000024f * fTemp30;
			fRec75[0] = 0.0f - fConst7 * (fConst8 * fRec75[1] - (fRec5[1] + fRec5[2]));
			fRec74[0] = fSlow202 * (fRec5[1] + fSlow203 * fRec75[0]) + fSlow204 * fRec74[1];
			fVec18[IOTA0 & 16383] = 0.353553385f * fRec74[0] + 9.99999968e-21f;
			float fTemp31 = fVec18[(IOTA0 - iConst49) & 16383] + fTemp28 + 0.600000024f * fRec72[1];
			fVec19[IOTA0 & 4095] = fTemp31;
			fRec72[0] = fVec19[(IOTA0 - iConst50) & 4095];
			float fRec73 = 0.0f - 0.600000024f * fTemp31;
			fRec79[0] = 0.0f - fConst7 * (fConst8 * fRec79[1] - (fRec1[1] + fRec1[2]));
			fRec78[0] = fSlow211 * (fRec1[1] + fSlow212 * fRec79[0]) + fSlow213 * fRec78[1];
			fVec20[IOTA0 & 32767] = 0.353553385f * fRec78[0] + 9.99999968e-21f;
			float fTemp32 = fTemp28 + 0.600000024f * fRec76[1] + fVec20[(IOTA0 - iConst56) & 32767];
			fVec21[IOTA0 & 4095] = fTemp32;
			fRec76[0] = fVec21[(IOTA0 - iConst57) & 4095];
			float fRec77 = 0.0f - 0.600000024f * fTemp32;
			fRec83[0] = 0.0f - fConst7 * (fConst8 * fRec83[1] - (fRec6[1] + fRec6[2]));
			fRec82[0] = fSlow220 * (fRec6[1] + fSlow221 * fRec83[0]) + fSlow222 * fRec82[1];
			fVec22[IOTA0 & 16383] = 0.353553385f * fRec82[0] + 9.99999968e-21f;
			float fTemp33 = fVec22[(IOTA0 - iConst63) & 16383] - (fTemp25 + 0.600000024f * fRec80[1]);
			fVec23[IOTA0 & 2047] = fTemp33;
			fRec80[0] = fVec23[(IOTA0 - iConst64) & 2047];
			float fRec81 = 0.600000024f * fTemp33;
			fRec87[0] = 0.0f - fConst7 * (fConst8 * fRec87[1] - (fRec2[1] + fRec2[2]));
			fRec86[0] = fSlow229 * (fRec2[1] + fSlow230 * fRec87[0]) + fSlow231 * fRec86[1];
			fVec24[IOTA0 & 16383] = 0.353553385f * fRec86[0] + 9.99999968e-21f;
			float fTemp34 = fVec24[(IOTA0 - iConst70) & 16383] - (fTemp25 + 0.600000024f * fRec84[1]);
			fVec25[IOTA0 & 4095] = fTemp34;
			fRec84[0] = fVec25[(IOTA0 - iConst71) & 4095];
			float fRec85 = 0.600000024f * fTemp34;
			fRec91[0] = 0.0f - fConst7 * (fConst8 * fRec91[1] - (fRec4[1] + fRec4[2]));
			fRec90[0] = fSlow238 * (fRec4[1] + fSlow239 * fRec91[0]) + fSlow240 * fRec90[1];
			fVec26[IOTA0 & 16383] = 0.353553385f * fRec90[0] + 9.99999968e-21f;
			float fTemp35 = (fTemp25 + fVec26[(IOTA0 - iConst77) & 16383]) - 0.600000024f * fRec88[1];
			fVec27[IOTA0 & 4095] = fTemp35;
			fRec88[0] = fVec27[(IOTA0 - iConst78) & 4095];
			float fRec89 = 0.600000024f * fTemp35;
			float fTemp36 = fRec9 + fRec89;
			float fTemp37 = fRec81 + fRec85 + fTemp36;
			fRec0[0] = fRec8[1] + fRec64[1] + fRec68[1] + fRec72[1] + fRec76[1] + fRec80[1] + fRec84[1] + fRec88[1] + fRec65 + fRec69 + fRec73 + fRec77 + fTemp37;
			fRec1[0] = (fRec8[1] + fRec80[1] + fRec84[1] + fRec88[1] + fTemp37) - (fRec64[1] + fRec68[1] + fRec72[1] + fRec76[1] + fRec65 + fRec69 + fRec77 + fRec73);
			float fTemp38 = fRec85 + fRec81;
			fRec2[0] = (fRec8[1] + fRec72[1] + fRec76[1] + fRec88[1] + fRec73 + fRec77 + fTemp36) - (fRec64[1] + fRec68[1] + fRec80[1] + fRec84[1] + fRec65 + fRec69 + fTemp38);
			fRec3[0] = (fRec8[1] + fRec64[1] + fRec68[1] + fRec88[1] + fRec65 + fRec69 + fTemp36) - (fRec72[1] + fRec76[1] + fRec80[1] + fRec84[1] + fRec73 + fRec77 + fTemp38);
			float fTemp39 = fRec9 + fRec85;
			float fTemp40 = fRec89 + fRec81;
			fRec4[0] = (fRec8[1] + fRec68[1] + fRec76[1] + fRec84[1] + fRec69 + fRec77 + fTemp39) - (fRec64[1] + fRec72[1] + fRec80[1] + fRec88[1] + fRec65 + fRec73 + fTemp40);
			fRec5[0] = (fRec8[1] + fRec64[1] + fRec72[1] + fRec84[1] + fRec65 + fRec73 + fTemp39) - (fRec68[1] + fRec76[1] + fRec80[1] + fRec88[1] + fRec69 + fRec77 + fTemp40);
			float fTemp41 = fRec9 + fRec81;
			float fTemp42 = fRec89 + fRec85;
			fRec6[0] = (fRec8[1] + fRec64[1] + fRec76[1] + fRec80[1] + fRec65 + fRec77 + fTemp41) - (fRec68[1] + fRec72[1] + fRec84[1] + fRec88[1] + fRec69 + fRec73 + fTemp42);
			fRec7[0] = (fRec8[1] + fRec68[1] + fRec72[1] + fRec80[1] + fRec69 + fRec73 + fTemp41) - (fRec64[1] + fRec76[1] + fRec84[1] + fRec88[1] + fRec65 + fRec77 + fTemp42);
			float fTemp43 = 1.0f - fRec12[0];
			output0[i0] = FAUSTFLOAT(0.370000005f * (fRec1[0] + fRec2[0]) + fSlow11 * fTemp43 * fTemp24);
			output1[i0] = FAUSTFLOAT(0.370000005f * (fRec1[0] - fRec2[0]) + fSlow185 * fTemp43 * fTemp27);
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
