/* ------------------------------------------------------------
name: "spat.zita8"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn spat_zita6x8 -scn spat_zita6x8_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __spat_zita6x8_H__
#define  __spat_zita6x8_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN spat_zita6x8_dsp.h ********************************
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

class FAUST_API spat_zita6x8_dsp {

    public:

        spat_zita6x8_dsp() {}
        virtual ~spat_zita6x8_dsp() {}

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
        virtual spat_zita6x8_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public spat_zita6x8_dsp {

    protected:

        spat_zita6x8_dsp* fDSP;

    public:

        decorator_dsp(spat_zita6x8_dsp* spat_zita6x8_dsp = nullptr):fDSP(spat_zita6x8_dsp) {}
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
    
        virtual spat_zita6x8_dsp* createDSPInstance() = 0;
    
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

/************************** END spat_zita6x8_dsp.h **************************/
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
 The base class of Meta handler to be used in spat_zita6x8_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct spat_zita6x8 : public spat_zita6x8_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include "m_pd.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS spat_zita6x8
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

static float spat_zita6x8_faustpower2_f(float value) {
	return value * value;
}

class spat_zita6x8 : public spat_zita6x8_dsp {
	
 private:
	
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fVslider1;
	float fRec0[2];
	FAUSTFLOAT fVslider2;
	int fSampleRate;
	float fConst2;
	FAUSTFLOAT fVslider3;
	float fConst3;
	float fRec11[2];
	int IOTA0;
	float fVec0[32768];
	int iConst5;
	float fVec1[2048];
	int iConst6;
	float fRec9[2];
	float fConst8;
	float fRec14[2];
	float fVec2[16384];
	int iConst10;
	float fVec3[4096];
	int iConst11;
	float fRec12[2];
	float fConst13;
	float fRec17[2];
	float fVec4[32768];
	int iConst15;
	float fVec5[4096];
	int iConst16;
	float fRec15[2];
	float fConst18;
	float fRec20[2];
	float fVec6[16384];
	int iConst20;
	float fVec7[8192];
	int iConst21;
	float fVec8[4096];
	int iConst22;
	float fRec18[2];
	float fConst24;
	float fRec23[2];
	float fVec9[16384];
	int iConst26;
	float fVec10[4096];
	int iConst27;
	float fRec21[2];
	float fConst29;
	float fRec26[2];
	float fVec11[32768];
	int iConst31;
	float fVec12[4096];
	int iConst32;
	float fRec24[2];
	float fConst34;
	float fRec29[2];
	float fVec13[16384];
	int iConst36;
	float fVec14[2048];
	int iConst37;
	float fRec27[2];
	float fConst39;
	float fRec32[2];
	float fVec15[16384];
	int iConst41;
	float fVec16[2048];
	int iConst42;
	float fRec30[2];
	float fRec1[2];
	float fRec2[2];
	float fRec3[2];
	float fRec4[2];
	float fRec5[2];
	float fRec6[2];
	float fRec7[2];
	float fRec8[2];
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fVslider5;
	FAUSTFLOAT fVslider6;
	float fRec33[2];
	FAUSTFLOAT fVslider7;
	FAUSTFLOAT fVslider8;
	float fRec44[2];
	float fVec17[32768];
	float fVec18[2048];
	float fRec42[2];
	float fRec47[2];
	float fVec19[32768];
	float fVec20[4096];
	float fRec45[2];
	float fRec50[2];
	float fVec21[16384];
	float fVec22[4096];
	float fRec48[2];
	float fRec53[2];
	float fVec23[32768];
	float fVec24[4096];
	float fRec51[2];
	float fRec56[2];
	float fVec25[16384];
	float fVec26[8192];
	float fVec27[2048];
	float fRec54[2];
	float fRec59[2];
	float fVec28[16384];
	float fVec29[4096];
	float fRec57[2];
	float fRec62[2];
	float fVec30[16384];
	float fVec31[4096];
	float fRec60[2];
	float fRec65[2];
	float fVec32[16384];
	float fVec33[2048];
	float fRec63[2];
	float fRec34[2];
	float fRec35[2];
	float fRec36[2];
	float fRec37[2];
	float fRec38[2];
	float fRec39[2];
	float fRec40[2];
	float fRec41[2];
	FAUSTFLOAT fVslider9;
	FAUSTFLOAT fVslider10;
	FAUSTFLOAT fVslider11;
	float fRec66[2];
	FAUSTFLOAT fVslider12;
	FAUSTFLOAT fVslider13;
	float fRec77[2];
	float fVec34[16384];
	float fVec35[8192];
	float fVec36[2048];
	float fRec75[2];
	float fRec80[2];
	float fVec37[32768];
	float fVec38[4096];
	float fRec78[2];
	float fRec83[2];
	float fVec39[16384];
	float fVec40[4096];
	float fRec81[2];
	float fRec86[2];
	float fVec41[32768];
	float fVec42[4096];
	float fRec84[2];
	float fRec89[2];
	float fVec43[16384];
	float fVec44[2048];
	float fRec87[2];
	float fRec92[2];
	float fVec45[16384];
	float fVec46[4096];
	float fRec90[2];
	float fRec95[2];
	float fVec47[16384];
	float fVec48[4096];
	float fRec93[2];
	float fRec98[2];
	float fVec49[32768];
	float fVec50[2048];
	float fRec96[2];
	float fRec67[2];
	float fRec68[2];
	float fRec69[2];
	float fRec70[2];
	float fRec71[2];
	float fRec72[2];
	float fRec73[2];
	float fRec74[2];
	FAUSTFLOAT fVslider14;
	FAUSTFLOAT fVslider15;
	FAUSTFLOAT fVslider16;
	float fRec99[2];
	FAUSTFLOAT fVslider17;
	FAUSTFLOAT fVslider18;
	float fRec110[2];
	float fVec51[16384];
	float fVec52[4096];
	float fRec108[2];
	float fRec113[2];
	float fVec53[32768];
	float fVec54[4096];
	float fRec111[2];
	float fRec116[2];
	float fVec55[16384];
	float fVec56[8192];
	float fVec57[2048];
	float fRec114[2];
	float fRec119[2];
	float fVec58[16384];
	float fVec59[4096];
	float fRec117[2];
	float fRec122[2];
	float fVec60[32768];
	float fVec61[4096];
	float fRec120[2];
	float fRec125[2];
	float fVec62[16384];
	float fVec63[4096];
	float fRec123[2];
	float fRec128[2];
	float fVec64[16384];
	float fVec65[2048];
	float fRec126[2];
	float fRec131[2];
	float fVec66[32768];
	float fVec67[2048];
	float fRec129[2];
	float fRec100[2];
	float fRec101[2];
	float fRec102[2];
	float fRec103[2];
	float fRec104[2];
	float fRec105[2];
	float fRec106[2];
	float fRec107[2];
	FAUSTFLOAT fVslider19;
	FAUSTFLOAT fVslider20;
	FAUSTFLOAT fVslider21;
	float fRec132[2];
	FAUSTFLOAT fVslider22;
	FAUSTFLOAT fVslider23;
	float fRec143[2];
	float fVec68[32768];
	float fVec69[4096];
	float fRec141[2];
	float fRec146[2];
	float fVec70[32768];
	float fVec71[2048];
	float fRec144[2];
	float fRec149[2];
	float fVec72[32768];
	float fVec73[4096];
	float fRec147[2];
	float fRec152[2];
	float fVec74[16384];
	float fVec75[8192];
	float fVec76[2048];
	float fRec150[2];
	float fRec155[2];
	float fVec77[16384];
	float fVec78[4096];
	float fRec153[2];
	float fRec158[2];
	float fVec79[16384];
	float fVec80[4096];
	float fRec156[2];
	float fRec161[2];
	float fVec81[16384];
	float fVec82[4096];
	float fRec159[2];
	float fRec164[2];
	float fVec83[16384];
	float fVec84[2048];
	float fRec162[2];
	float fRec133[2];
	float fRec134[2];
	float fRec135[2];
	float fRec136[2];
	float fRec137[2];
	float fRec138[2];
	float fRec139[2];
	float fRec140[2];
	FAUSTFLOAT fVslider24;
	FAUSTFLOAT fVslider25;
	FAUSTFLOAT fVslider26;
	float fRec165[2];
	FAUSTFLOAT fVslider27;
	FAUSTFLOAT fVslider28;
	float fRec176[2];
	float fVec85[32768];
	float fVec86[2048];
	float fRec174[2];
	float fRec179[2];
	float fVec87[32768];
	float fVec88[4096];
	float fRec177[2];
	float fRec182[2];
	float fVec89[16384];
	float fVec90[4096];
	float fRec180[2];
	float fRec185[2];
	float fVec91[32768];
	float fVec92[4096];
	float fRec183[2];
	float fRec188[2];
	float fVec93[16384];
	float fVec94[8192];
	float fVec95[2048];
	float fRec186[2];
	float fRec191[2];
	float fVec96[16384];
	float fVec97[4096];
	float fRec189[2];
	float fRec194[2];
	float fVec98[16384];
	float fVec99[4096];
	float fRec192[2];
	float fRec197[2];
	float fVec100[16384];
	float fVec101[2048];
	float fRec195[2];
	float fRec166[2];
	float fRec167[2];
	float fRec168[2];
	float fRec169[2];
	float fRec170[2];
	float fRec171[2];
	float fRec172[2];
	float fRec173[2];
	FAUSTFLOAT fVslider29;
	float fRec198[2];
	float fRec199[2];
	float fRec200[2];
	float fRec201[2];
	float fRec202[2];
	float fRec203[2];
	float fRec204[2];
	float fRec205[2];
	float fRec206[2];
	float fRec207[2];
	float fRec208[2];
	float fRec209[2];
	float fRec210[2];
	float fRec211[2];
	float fRec212[2];
	float fRec213[2];
	float fRec214[2];
	float fRec215[2];
	float fRec216[2];
	float fRec217[2];
	float fRec218[2];
	float fRec219[2];
	float fRec220[2];
	float fRec221[2];
	float fRec222[2];
	float fRec223[2];
	float fRec224[2];
	float fRec225[2];
	float fRec226[2];
	float fRec227[2];
	float fRec228[2];
	float fRec229[2];
	float fRec230[2];
	float fRec231[2];
	float fRec232[2];
	float fRec233[2];
	float fRec234[2];
	float fRec235[2];
	float fRec236[2];
	float fRec237[2];
	float fRec238[2];
	float fRec239[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn spat_zita6x8 -scn spat_zita6x8_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "spat_zita6x8.dsp");
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
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "spat.zita8");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
	}

	virtual int getNumInputs() {
		return 6;
	}
	virtual int getNumOutputs() {
		return 8;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::floor(0.219991f * fConst0 + 0.5f);
		fConst2 = (0.0f - 6.9077554f * fConst1) / fConst0;
		fConst3 = std::cos(37699.113f / fConst0);
		float fConst4 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst5 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst1 - fConst4)));
		iConst6 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst4 + -1.0f)));
		float fConst7 = std::floor(0.192303f * fConst0 + 0.5f);
		fConst8 = (0.0f - 6.9077554f * fConst7) / fConst0;
		float fConst9 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst10 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst7 - fConst9)));
		iConst11 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst9 + -1.0f)));
		float fConst12 = std::floor(0.210389f * fConst0 + 0.5f);
		fConst13 = (0.0f - 6.9077554f * fConst12) / fConst0;
		float fConst14 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst15 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst12 - fConst14)));
		iConst16 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst14 + -1.0f)));
		float fConst17 = std::floor(0.127837f * fConst0 + 0.5f);
		fConst18 = (0.0f - 6.9077554f * fConst17) / fConst0;
		float fConst19 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst20 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst17 - fConst19)));
		iConst21 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.04f * fConst0)));
		iConst22 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst19 + -1.0f)));
		float fConst23 = std::floor(0.174713f * fConst0 + 0.5f);
		fConst24 = (0.0f - 6.9077554f * fConst23) / fConst0;
		float fConst25 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst26 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst23 - fConst25)));
		iConst27 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst25 + -1.0f)));
		float fConst28 = std::floor(0.256891f * fConst0 + 0.5f);
		fConst29 = (0.0f - 6.9077554f * fConst28) / fConst0;
		float fConst30 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst31 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst28 - fConst30)));
		iConst32 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst30 + -1.0f)));
		float fConst33 = std::floor(0.125f * fConst0 + 0.5f);
		fConst34 = (0.0f - 6.9077554f * fConst33) / fConst0;
		float fConst35 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst36 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst33 - fConst35)));
		iConst37 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst35 + -1.0f)));
		float fConst38 = std::floor(0.153129f * fConst0 + 0.5f);
		fConst39 = (0.0f - 6.9077554f * fConst38) / fConst0;
		float fConst40 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst41 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst38 - fConst40)));
		iConst42 = int(std::min<float>(1024.0f, std::max<float>(0.0f, fConst40 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(1.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fVslider2 = FAUSTFLOAT(-8.0f);
		fVslider3 = FAUSTFLOAT(2.0f);
		fVslider4 = FAUSTFLOAT(-3.0f);
		fVslider5 = FAUSTFLOAT(1.0f);
		fVslider6 = FAUSTFLOAT(0.0f);
		fVslider7 = FAUSTFLOAT(-8.0f);
		fVslider8 = FAUSTFLOAT(2.0f);
		fVslider9 = FAUSTFLOAT(-3.0f);
		fVslider10 = FAUSTFLOAT(1.0f);
		fVslider11 = FAUSTFLOAT(0.0f);
		fVslider12 = FAUSTFLOAT(-8.0f);
		fVslider13 = FAUSTFLOAT(2.0f);
		fVslider14 = FAUSTFLOAT(-3.0f);
		fVslider15 = FAUSTFLOAT(1.0f);
		fVslider16 = FAUSTFLOAT(0.0f);
		fVslider17 = FAUSTFLOAT(-8.0f);
		fVslider18 = FAUSTFLOAT(2.0f);
		fVslider19 = FAUSTFLOAT(-3.0f);
		fVslider20 = FAUSTFLOAT(1.0f);
		fVslider21 = FAUSTFLOAT(0.0f);
		fVslider22 = FAUSTFLOAT(-8.0f);
		fVslider23 = FAUSTFLOAT(2.0f);
		fVslider24 = FAUSTFLOAT(-3.0f);
		fVslider25 = FAUSTFLOAT(1.0f);
		fVslider26 = FAUSTFLOAT(0.0f);
		fVslider27 = FAUSTFLOAT(-8.0f);
		fVslider28 = FAUSTFLOAT(2.0f);
		fVslider29 = FAUSTFLOAT(-3.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec11[l1] = 0.0f;
		}
		IOTA0 = 0;
		for (int l2 = 0; l2 < 32768; l2 = l2 + 1) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2048; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec9[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec14[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 16384; l6 = l6 + 1) {
			fVec2[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 4096; l7 = l7 + 1) {
			fVec3[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec12[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec17[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 32768; l10 = l10 + 1) {
			fVec4[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4096; l11 = l11 + 1) {
			fVec5[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec15[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec20[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 16384; l14 = l14 + 1) {
			fVec6[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 8192; l15 = l15 + 1) {
			fVec7[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 4096; l16 = l16 + 1) {
			fVec8[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec18[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec23[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 16384; l19 = l19 + 1) {
			fVec9[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 4096; l20 = l20 + 1) {
			fVec10[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec21[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec26[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 32768; l23 = l23 + 1) {
			fVec11[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 4096; l24 = l24 + 1) {
			fVec12[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec24[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec29[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 16384; l27 = l27 + 1) {
			fVec13[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2048; l28 = l28 + 1) {
			fVec14[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec27[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec32[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 16384; l31 = l31 + 1) {
			fVec15[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2048; l32 = l32 + 1) {
			fVec16[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec30[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec1[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec2[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec3[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec4[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec5[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec6[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec7[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec8[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec33[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec44[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 32768; l44 = l44 + 1) {
			fVec17[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2048; l45 = l45 + 1) {
			fVec18[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec42[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec47[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 32768; l48 = l48 + 1) {
			fVec19[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 4096; l49 = l49 + 1) {
			fVec20[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec45[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec50[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 16384; l52 = l52 + 1) {
			fVec21[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 4096; l53 = l53 + 1) {
			fVec22[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec48[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec53[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 32768; l56 = l56 + 1) {
			fVec23[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 4096; l57 = l57 + 1) {
			fVec24[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec51[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec56[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 16384; l60 = l60 + 1) {
			fVec25[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 8192; l61 = l61 + 1) {
			fVec26[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2048; l62 = l62 + 1) {
			fVec27[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec54[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec59[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 16384; l65 = l65 + 1) {
			fVec28[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 4096; l66 = l66 + 1) {
			fVec29[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec57[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec62[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 16384; l69 = l69 + 1) {
			fVec30[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 4096; l70 = l70 + 1) {
			fVec31[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec60[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec65[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 16384; l73 = l73 + 1) {
			fVec32[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2048; l74 = l74 + 1) {
			fVec33[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec63[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec34[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec35[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fRec36[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec37[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec38[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec39[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec40[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec41[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fRec66[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec77[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 16384; l86 = l86 + 1) {
			fVec34[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 8192; l87 = l87 + 1) {
			fVec35[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2048; l88 = l88 + 1) {
			fVec36[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec75[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec80[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 32768; l91 = l91 + 1) {
			fVec37[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 4096; l92 = l92 + 1) {
			fVec38[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fRec78[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec83[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 16384; l95 = l95 + 1) {
			fVec39[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 4096; l96 = l96 + 1) {
			fVec40[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec81[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec86[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 32768; l99 = l99 + 1) {
			fVec41[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 4096; l100 = l100 + 1) {
			fVec42[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec84[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fRec89[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 16384; l103 = l103 + 1) {
			fVec43[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 2048; l104 = l104 + 1) {
			fVec44[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fRec87[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			fRec92[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 16384; l107 = l107 + 1) {
			fVec45[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 4096; l108 = l108 + 1) {
			fVec46[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			fRec90[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			fRec95[l110] = 0.0f;
		}
		for (int l111 = 0; l111 < 16384; l111 = l111 + 1) {
			fVec47[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 4096; l112 = l112 + 1) {
			fVec48[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec93[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fRec98[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 32768; l115 = l115 + 1) {
			fVec49[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 2048; l116 = l116 + 1) {
			fVec50[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fRec96[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			fRec67[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 2; l119 = l119 + 1) {
			fRec68[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fRec69[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			fRec70[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 2; l122 = l122 + 1) {
			fRec71[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fRec72[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			fRec73[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec74[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fRec99[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec110[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 16384; l128 = l128 + 1) {
			fVec51[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 4096; l129 = l129 + 1) {
			fVec52[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			fRec108[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
			fRec113[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 32768; l132 = l132 + 1) {
			fVec53[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 4096; l133 = l133 + 1) {
			fVec54[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			fRec111[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fRec116[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 16384; l136 = l136 + 1) {
			fVec55[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 8192; l137 = l137 + 1) {
			fVec56[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 2048; l138 = l138 + 1) {
			fVec57[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
			fRec114[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
			fRec119[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 16384; l141 = l141 + 1) {
			fVec58[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 4096; l142 = l142 + 1) {
			fVec59[l142] = 0.0f;
		}
		for (int l143 = 0; l143 < 2; l143 = l143 + 1) {
			fRec117[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fRec122[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 32768; l145 = l145 + 1) {
			fVec60[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 4096; l146 = l146 + 1) {
			fVec61[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 2; l147 = l147 + 1) {
			fRec120[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fRec125[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 16384; l149 = l149 + 1) {
			fVec62[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 4096; l150 = l150 + 1) {
			fVec63[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			fRec123[l151] = 0.0f;
		}
		for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
			fRec128[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 16384; l153 = l153 + 1) {
			fVec64[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 2048; l154 = l154 + 1) {
			fVec65[l154] = 0.0f;
		}
		for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
			fRec126[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fRec131[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 32768; l157 = l157 + 1) {
			fVec66[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 2048; l158 = l158 + 1) {
			fVec67[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 2; l159 = l159 + 1) {
			fRec129[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fRec100[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 2; l161 = l161 + 1) {
			fRec101[l161] = 0.0f;
		}
		for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
			fRec102[l162] = 0.0f;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec103[l163] = 0.0f;
		}
		for (int l164 = 0; l164 < 2; l164 = l164 + 1) {
			fRec104[l164] = 0.0f;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fRec105[l165] = 0.0f;
		}
		for (int l166 = 0; l166 < 2; l166 = l166 + 1) {
			fRec106[l166] = 0.0f;
		}
		for (int l167 = 0; l167 < 2; l167 = l167 + 1) {
			fRec107[l167] = 0.0f;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fRec132[l168] = 0.0f;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fRec143[l169] = 0.0f;
		}
		for (int l170 = 0; l170 < 32768; l170 = l170 + 1) {
			fVec68[l170] = 0.0f;
		}
		for (int l171 = 0; l171 < 4096; l171 = l171 + 1) {
			fVec69[l171] = 0.0f;
		}
		for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
			fRec141[l172] = 0.0f;
		}
		for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
			fRec146[l173] = 0.0f;
		}
		for (int l174 = 0; l174 < 32768; l174 = l174 + 1) {
			fVec70[l174] = 0.0f;
		}
		for (int l175 = 0; l175 < 2048; l175 = l175 + 1) {
			fVec71[l175] = 0.0f;
		}
		for (int l176 = 0; l176 < 2; l176 = l176 + 1) {
			fRec144[l176] = 0.0f;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			fRec149[l177] = 0.0f;
		}
		for (int l178 = 0; l178 < 32768; l178 = l178 + 1) {
			fVec72[l178] = 0.0f;
		}
		for (int l179 = 0; l179 < 4096; l179 = l179 + 1) {
			fVec73[l179] = 0.0f;
		}
		for (int l180 = 0; l180 < 2; l180 = l180 + 1) {
			fRec147[l180] = 0.0f;
		}
		for (int l181 = 0; l181 < 2; l181 = l181 + 1) {
			fRec152[l181] = 0.0f;
		}
		for (int l182 = 0; l182 < 16384; l182 = l182 + 1) {
			fVec74[l182] = 0.0f;
		}
		for (int l183 = 0; l183 < 8192; l183 = l183 + 1) {
			fVec75[l183] = 0.0f;
		}
		for (int l184 = 0; l184 < 2048; l184 = l184 + 1) {
			fVec76[l184] = 0.0f;
		}
		for (int l185 = 0; l185 < 2; l185 = l185 + 1) {
			fRec150[l185] = 0.0f;
		}
		for (int l186 = 0; l186 < 2; l186 = l186 + 1) {
			fRec155[l186] = 0.0f;
		}
		for (int l187 = 0; l187 < 16384; l187 = l187 + 1) {
			fVec77[l187] = 0.0f;
		}
		for (int l188 = 0; l188 < 4096; l188 = l188 + 1) {
			fVec78[l188] = 0.0f;
		}
		for (int l189 = 0; l189 < 2; l189 = l189 + 1) {
			fRec153[l189] = 0.0f;
		}
		for (int l190 = 0; l190 < 2; l190 = l190 + 1) {
			fRec158[l190] = 0.0f;
		}
		for (int l191 = 0; l191 < 16384; l191 = l191 + 1) {
			fVec79[l191] = 0.0f;
		}
		for (int l192 = 0; l192 < 4096; l192 = l192 + 1) {
			fVec80[l192] = 0.0f;
		}
		for (int l193 = 0; l193 < 2; l193 = l193 + 1) {
			fRec156[l193] = 0.0f;
		}
		for (int l194 = 0; l194 < 2; l194 = l194 + 1) {
			fRec161[l194] = 0.0f;
		}
		for (int l195 = 0; l195 < 16384; l195 = l195 + 1) {
			fVec81[l195] = 0.0f;
		}
		for (int l196 = 0; l196 < 4096; l196 = l196 + 1) {
			fVec82[l196] = 0.0f;
		}
		for (int l197 = 0; l197 < 2; l197 = l197 + 1) {
			fRec159[l197] = 0.0f;
		}
		for (int l198 = 0; l198 < 2; l198 = l198 + 1) {
			fRec164[l198] = 0.0f;
		}
		for (int l199 = 0; l199 < 16384; l199 = l199 + 1) {
			fVec83[l199] = 0.0f;
		}
		for (int l200 = 0; l200 < 2048; l200 = l200 + 1) {
			fVec84[l200] = 0.0f;
		}
		for (int l201 = 0; l201 < 2; l201 = l201 + 1) {
			fRec162[l201] = 0.0f;
		}
		for (int l202 = 0; l202 < 2; l202 = l202 + 1) {
			fRec133[l202] = 0.0f;
		}
		for (int l203 = 0; l203 < 2; l203 = l203 + 1) {
			fRec134[l203] = 0.0f;
		}
		for (int l204 = 0; l204 < 2; l204 = l204 + 1) {
			fRec135[l204] = 0.0f;
		}
		for (int l205 = 0; l205 < 2; l205 = l205 + 1) {
			fRec136[l205] = 0.0f;
		}
		for (int l206 = 0; l206 < 2; l206 = l206 + 1) {
			fRec137[l206] = 0.0f;
		}
		for (int l207 = 0; l207 < 2; l207 = l207 + 1) {
			fRec138[l207] = 0.0f;
		}
		for (int l208 = 0; l208 < 2; l208 = l208 + 1) {
			fRec139[l208] = 0.0f;
		}
		for (int l209 = 0; l209 < 2; l209 = l209 + 1) {
			fRec140[l209] = 0.0f;
		}
		for (int l210 = 0; l210 < 2; l210 = l210 + 1) {
			fRec165[l210] = 0.0f;
		}
		for (int l211 = 0; l211 < 2; l211 = l211 + 1) {
			fRec176[l211] = 0.0f;
		}
		for (int l212 = 0; l212 < 32768; l212 = l212 + 1) {
			fVec85[l212] = 0.0f;
		}
		for (int l213 = 0; l213 < 2048; l213 = l213 + 1) {
			fVec86[l213] = 0.0f;
		}
		for (int l214 = 0; l214 < 2; l214 = l214 + 1) {
			fRec174[l214] = 0.0f;
		}
		for (int l215 = 0; l215 < 2; l215 = l215 + 1) {
			fRec179[l215] = 0.0f;
		}
		for (int l216 = 0; l216 < 32768; l216 = l216 + 1) {
			fVec87[l216] = 0.0f;
		}
		for (int l217 = 0; l217 < 4096; l217 = l217 + 1) {
			fVec88[l217] = 0.0f;
		}
		for (int l218 = 0; l218 < 2; l218 = l218 + 1) {
			fRec177[l218] = 0.0f;
		}
		for (int l219 = 0; l219 < 2; l219 = l219 + 1) {
			fRec182[l219] = 0.0f;
		}
		for (int l220 = 0; l220 < 16384; l220 = l220 + 1) {
			fVec89[l220] = 0.0f;
		}
		for (int l221 = 0; l221 < 4096; l221 = l221 + 1) {
			fVec90[l221] = 0.0f;
		}
		for (int l222 = 0; l222 < 2; l222 = l222 + 1) {
			fRec180[l222] = 0.0f;
		}
		for (int l223 = 0; l223 < 2; l223 = l223 + 1) {
			fRec185[l223] = 0.0f;
		}
		for (int l224 = 0; l224 < 32768; l224 = l224 + 1) {
			fVec91[l224] = 0.0f;
		}
		for (int l225 = 0; l225 < 4096; l225 = l225 + 1) {
			fVec92[l225] = 0.0f;
		}
		for (int l226 = 0; l226 < 2; l226 = l226 + 1) {
			fRec183[l226] = 0.0f;
		}
		for (int l227 = 0; l227 < 2; l227 = l227 + 1) {
			fRec188[l227] = 0.0f;
		}
		for (int l228 = 0; l228 < 16384; l228 = l228 + 1) {
			fVec93[l228] = 0.0f;
		}
		for (int l229 = 0; l229 < 8192; l229 = l229 + 1) {
			fVec94[l229] = 0.0f;
		}
		for (int l230 = 0; l230 < 2048; l230 = l230 + 1) {
			fVec95[l230] = 0.0f;
		}
		for (int l231 = 0; l231 < 2; l231 = l231 + 1) {
			fRec186[l231] = 0.0f;
		}
		for (int l232 = 0; l232 < 2; l232 = l232 + 1) {
			fRec191[l232] = 0.0f;
		}
		for (int l233 = 0; l233 < 16384; l233 = l233 + 1) {
			fVec96[l233] = 0.0f;
		}
		for (int l234 = 0; l234 < 4096; l234 = l234 + 1) {
			fVec97[l234] = 0.0f;
		}
		for (int l235 = 0; l235 < 2; l235 = l235 + 1) {
			fRec189[l235] = 0.0f;
		}
		for (int l236 = 0; l236 < 2; l236 = l236 + 1) {
			fRec194[l236] = 0.0f;
		}
		for (int l237 = 0; l237 < 16384; l237 = l237 + 1) {
			fVec98[l237] = 0.0f;
		}
		for (int l238 = 0; l238 < 4096; l238 = l238 + 1) {
			fVec99[l238] = 0.0f;
		}
		for (int l239 = 0; l239 < 2; l239 = l239 + 1) {
			fRec192[l239] = 0.0f;
		}
		for (int l240 = 0; l240 < 2; l240 = l240 + 1) {
			fRec197[l240] = 0.0f;
		}
		for (int l241 = 0; l241 < 16384; l241 = l241 + 1) {
			fVec100[l241] = 0.0f;
		}
		for (int l242 = 0; l242 < 2048; l242 = l242 + 1) {
			fVec101[l242] = 0.0f;
		}
		for (int l243 = 0; l243 < 2; l243 = l243 + 1) {
			fRec195[l243] = 0.0f;
		}
		for (int l244 = 0; l244 < 2; l244 = l244 + 1) {
			fRec166[l244] = 0.0f;
		}
		for (int l245 = 0; l245 < 2; l245 = l245 + 1) {
			fRec167[l245] = 0.0f;
		}
		for (int l246 = 0; l246 < 2; l246 = l246 + 1) {
			fRec168[l246] = 0.0f;
		}
		for (int l247 = 0; l247 < 2; l247 = l247 + 1) {
			fRec169[l247] = 0.0f;
		}
		for (int l248 = 0; l248 < 2; l248 = l248 + 1) {
			fRec170[l248] = 0.0f;
		}
		for (int l249 = 0; l249 < 2; l249 = l249 + 1) {
			fRec171[l249] = 0.0f;
		}
		for (int l250 = 0; l250 < 2; l250 = l250 + 1) {
			fRec172[l250] = 0.0f;
		}
		for (int l251 = 0; l251 < 2; l251 = l251 + 1) {
			fRec173[l251] = 0.0f;
		}
		for (int l252 = 0; l252 < 2; l252 = l252 + 1) {
			fRec198[l252] = 0.0f;
		}
		for (int l253 = 0; l253 < 2; l253 = l253 + 1) {
			fRec199[l253] = 0.0f;
		}
		for (int l254 = 0; l254 < 2; l254 = l254 + 1) {
			fRec200[l254] = 0.0f;
		}
		for (int l255 = 0; l255 < 2; l255 = l255 + 1) {
			fRec201[l255] = 0.0f;
		}
		for (int l256 = 0; l256 < 2; l256 = l256 + 1) {
			fRec202[l256] = 0.0f;
		}
		for (int l257 = 0; l257 < 2; l257 = l257 + 1) {
			fRec203[l257] = 0.0f;
		}
		for (int l258 = 0; l258 < 2; l258 = l258 + 1) {
			fRec204[l258] = 0.0f;
		}
		for (int l259 = 0; l259 < 2; l259 = l259 + 1) {
			fRec205[l259] = 0.0f;
		}
		for (int l260 = 0; l260 < 2; l260 = l260 + 1) {
			fRec206[l260] = 0.0f;
		}
		for (int l261 = 0; l261 < 2; l261 = l261 + 1) {
			fRec207[l261] = 0.0f;
		}
		for (int l262 = 0; l262 < 2; l262 = l262 + 1) {
			fRec208[l262] = 0.0f;
		}
		for (int l263 = 0; l263 < 2; l263 = l263 + 1) {
			fRec209[l263] = 0.0f;
		}
		for (int l264 = 0; l264 < 2; l264 = l264 + 1) {
			fRec210[l264] = 0.0f;
		}
		for (int l265 = 0; l265 < 2; l265 = l265 + 1) {
			fRec211[l265] = 0.0f;
		}
		for (int l266 = 0; l266 < 2; l266 = l266 + 1) {
			fRec212[l266] = 0.0f;
		}
		for (int l267 = 0; l267 < 2; l267 = l267 + 1) {
			fRec213[l267] = 0.0f;
		}
		for (int l268 = 0; l268 < 2; l268 = l268 + 1) {
			fRec214[l268] = 0.0f;
		}
		for (int l269 = 0; l269 < 2; l269 = l269 + 1) {
			fRec215[l269] = 0.0f;
		}
		for (int l270 = 0; l270 < 2; l270 = l270 + 1) {
			fRec216[l270] = 0.0f;
		}
		for (int l271 = 0; l271 < 2; l271 = l271 + 1) {
			fRec217[l271] = 0.0f;
		}
		for (int l272 = 0; l272 < 2; l272 = l272 + 1) {
			fRec218[l272] = 0.0f;
		}
		for (int l273 = 0; l273 < 2; l273 = l273 + 1) {
			fRec219[l273] = 0.0f;
		}
		for (int l274 = 0; l274 < 2; l274 = l274 + 1) {
			fRec220[l274] = 0.0f;
		}
		for (int l275 = 0; l275 < 2; l275 = l275 + 1) {
			fRec221[l275] = 0.0f;
		}
		for (int l276 = 0; l276 < 2; l276 = l276 + 1) {
			fRec222[l276] = 0.0f;
		}
		for (int l277 = 0; l277 < 2; l277 = l277 + 1) {
			fRec223[l277] = 0.0f;
		}
		for (int l278 = 0; l278 < 2; l278 = l278 + 1) {
			fRec224[l278] = 0.0f;
		}
		for (int l279 = 0; l279 < 2; l279 = l279 + 1) {
			fRec225[l279] = 0.0f;
		}
		for (int l280 = 0; l280 < 2; l280 = l280 + 1) {
			fRec226[l280] = 0.0f;
		}
		for (int l281 = 0; l281 < 2; l281 = l281 + 1) {
			fRec227[l281] = 0.0f;
		}
		for (int l282 = 0; l282 < 2; l282 = l282 + 1) {
			fRec228[l282] = 0.0f;
		}
		for (int l283 = 0; l283 < 2; l283 = l283 + 1) {
			fRec229[l283] = 0.0f;
		}
		for (int l284 = 0; l284 < 2; l284 = l284 + 1) {
			fRec230[l284] = 0.0f;
		}
		for (int l285 = 0; l285 < 2; l285 = l285 + 1) {
			fRec231[l285] = 0.0f;
		}
		for (int l286 = 0; l286 < 2; l286 = l286 + 1) {
			fRec232[l286] = 0.0f;
		}
		for (int l287 = 0; l287 < 2; l287 = l287 + 1) {
			fRec233[l287] = 0.0f;
		}
		for (int l288 = 0; l288 < 2; l288 = l288 + 1) {
			fRec234[l288] = 0.0f;
		}
		for (int l289 = 0; l289 < 2; l289 = l289 + 1) {
			fRec235[l289] = 0.0f;
		}
		for (int l290 = 0; l290 < 2; l290 = l290 + 1) {
			fRec236[l290] = 0.0f;
		}
		for (int l291 = 0; l291 < 2; l291 = l291 + 1) {
			fRec237[l291] = 0.0f;
		}
		for (int l292 = 0; l292 < 2; l292 = l292 + 1) {
			fRec238[l292] = 0.0f;
		}
		for (int l293 = 0; l293 < 2; l293 = l293 + 1) {
			fRec239[l293] = 0.0f;
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
	
	virtual spat_zita6x8* clone() {
		return new spat_zita6x8();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("spat.zita8");
		ui_interface->addVerticalSlider("a0", &fVslider11, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("a1", &fVslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("a2", &fVslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("a3", &fVslider16, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("a4", &fVslider21, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("a5", &fVslider26, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->declare(&fVslider14, "scale", "log");
		ui_interface->declare(&fVslider14, "style", "knob");
		ui_interface->declare(&fVslider14, "unit", "db");
		ui_interface->addVerticalSlider("dry0", &fVslider14, FAUSTFLOAT(-3.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider4, "scale", "log");
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "unit", "db");
		ui_interface->addVerticalSlider("dry1", &fVslider4, FAUSTFLOAT(-3.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider9, "scale", "log");
		ui_interface->declare(&fVslider9, "style", "knob");
		ui_interface->declare(&fVslider9, "unit", "db");
		ui_interface->addVerticalSlider("dry2", &fVslider9, FAUSTFLOAT(-3.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider19, "scale", "log");
		ui_interface->declare(&fVslider19, "style", "knob");
		ui_interface->declare(&fVslider19, "unit", "db");
		ui_interface->addVerticalSlider("dry3", &fVslider19, FAUSTFLOAT(-3.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider24, "scale", "log");
		ui_interface->declare(&fVslider24, "style", "knob");
		ui_interface->declare(&fVslider24, "unit", "db");
		ui_interface->addVerticalSlider("dry4", &fVslider24, FAUSTFLOAT(-3.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider29, "scale", "log");
		ui_interface->declare(&fVslider29, "style", "knob");
		ui_interface->declare(&fVslider29, "unit", "db");
		ui_interface->addVerticalSlider("dry5", &fVslider29, FAUSTFLOAT(-3.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("r0", &fVslider10, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("r1", &fVslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("r2", &fVslider5, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("r3", &fVslider15, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("r4", &fVslider20, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("r5", &fVslider25, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->declare(&fVslider13, "scale", "log");
		ui_interface->declare(&fVslider13, "style", "knob");
		ui_interface->declare(&fVslider13, "unit", "sec");
		ui_interface->addVerticalSlider("t0", &fVslider13, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider3, "scale", "log");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "unit", "sec");
		ui_interface->addVerticalSlider("t1", &fVslider3, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider8, "scale", "log");
		ui_interface->declare(&fVslider8, "style", "knob");
		ui_interface->declare(&fVslider8, "unit", "sec");
		ui_interface->addVerticalSlider("t2", &fVslider8, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider18, "scale", "log");
		ui_interface->declare(&fVslider18, "style", "knob");
		ui_interface->declare(&fVslider18, "unit", "sec");
		ui_interface->addVerticalSlider("t3", &fVslider18, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider23, "scale", "log");
		ui_interface->declare(&fVslider23, "style", "knob");
		ui_interface->declare(&fVslider23, "unit", "sec");
		ui_interface->addVerticalSlider("t4", &fVslider23, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider28, "scale", "log");
		ui_interface->declare(&fVslider28, "style", "knob");
		ui_interface->declare(&fVslider28, "unit", "sec");
		ui_interface->addVerticalSlider("t5", &fVslider28, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider12, "scale", "log");
		ui_interface->declare(&fVslider12, "style", "knob");
		ui_interface->declare(&fVslider12, "unit", "db");
		ui_interface->addVerticalSlider("wet0", &fVslider12, FAUSTFLOAT(-8.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "db");
		ui_interface->addVerticalSlider("wet1", &fVslider2, FAUSTFLOAT(-8.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider7, "scale", "log");
		ui_interface->declare(&fVslider7, "style", "knob");
		ui_interface->declare(&fVslider7, "unit", "db");
		ui_interface->addVerticalSlider("wet2", &fVslider7, FAUSTFLOAT(-8.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider17, "scale", "log");
		ui_interface->declare(&fVslider17, "style", "knob");
		ui_interface->declare(&fVslider17, "unit", "db");
		ui_interface->addVerticalSlider("wet3", &fVslider17, FAUSTFLOAT(-8.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider22, "scale", "log");
		ui_interface->declare(&fVslider22, "style", "knob");
		ui_interface->declare(&fVslider22, "unit", "db");
		ui_interface->addVerticalSlider("wet4", &fVslider22, FAUSTFLOAT(-8.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider27, "scale", "log");
		ui_interface->declare(&fVslider27, "style", "knob");
		ui_interface->declare(&fVslider27, "unit", "db");
		ui_interface->addVerticalSlider("wet5", &fVslider27, FAUSTFLOAT(-8.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* input2 = inputs[2];
		FAUSTFLOAT* input3 = inputs[3];
		FAUSTFLOAT* input4 = inputs[4];
		FAUSTFLOAT* input5 = inputs[5];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		FAUSTFLOAT* output2 = outputs[2];
		FAUSTFLOAT* output3 = outputs[3];
		FAUSTFLOAT* output4 = outputs[4];
		FAUSTFLOAT* output5 = outputs[5];
		FAUSTFLOAT* output6 = outputs[6];
		FAUSTFLOAT* output7 = outputs[7];
		float fSlow0 = float(fVslider0);
		float fSlow1 = fSlow0 + 1.0f;
		float fSlow2 = float(fVslider1);
		float fSlow3 = 0.0005f * fSlow1 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow0 * std::fabs(std::fmod(fSlow2 + 1.5f, 1.0f) + -0.5f)));
		float fSlow4 = 0.37f * std::pow(1e+01f, 0.05f * float(fVslider2));
		float fSlow5 = float(fVslider3);
		float fSlow6 = std::exp(fConst2 / fSlow5);
		float fSlow7 = spat_zita6x8_faustpower2_f(fSlow6);
		float fSlow8 = 1.0f - fConst3 * fSlow7;
		float fSlow9 = 1.0f - fSlow7;
		float fSlow10 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow8) / spat_zita6x8_faustpower2_f(fSlow9) + -1.0f));
		float fSlow11 = fSlow8 / fSlow9;
		float fSlow12 = fSlow6 * (fSlow10 + (1.0f - fSlow11));
		float fSlow13 = fSlow11 - fSlow10;
		float fSlow14 = std::exp(fConst8 / fSlow5);
		float fSlow15 = spat_zita6x8_faustpower2_f(fSlow14);
		float fSlow16 = 1.0f - fConst3 * fSlow15;
		float fSlow17 = 1.0f - fSlow15;
		float fSlow18 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow16) / spat_zita6x8_faustpower2_f(fSlow17) + -1.0f));
		float fSlow19 = fSlow16 / fSlow17;
		float fSlow20 = fSlow14 * (fSlow18 + (1.0f - fSlow19));
		float fSlow21 = fSlow19 - fSlow18;
		float fSlow22 = std::exp(fConst13 / fSlow5);
		float fSlow23 = spat_zita6x8_faustpower2_f(fSlow22);
		float fSlow24 = 1.0f - fConst3 * fSlow23;
		float fSlow25 = 1.0f - fSlow23;
		float fSlow26 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow24) / spat_zita6x8_faustpower2_f(fSlow25) + -1.0f));
		float fSlow27 = fSlow24 / fSlow25;
		float fSlow28 = fSlow22 * (fSlow26 + (1.0f - fSlow27));
		float fSlow29 = fSlow27 - fSlow26;
		float fSlow30 = std::exp(fConst18 / fSlow5);
		float fSlow31 = spat_zita6x8_faustpower2_f(fSlow30);
		float fSlow32 = 1.0f - fConst3 * fSlow31;
		float fSlow33 = 1.0f - fSlow31;
		float fSlow34 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow32) / spat_zita6x8_faustpower2_f(fSlow33) + -1.0f));
		float fSlow35 = fSlow32 / fSlow33;
		float fSlow36 = fSlow30 * (fSlow34 + (1.0f - fSlow35));
		float fSlow37 = fSlow35 - fSlow34;
		float fSlow38 = std::exp(fConst24 / fSlow5);
		float fSlow39 = spat_zita6x8_faustpower2_f(fSlow38);
		float fSlow40 = 1.0f - fConst3 * fSlow39;
		float fSlow41 = 1.0f - fSlow39;
		float fSlow42 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow40) / spat_zita6x8_faustpower2_f(fSlow41) + -1.0f));
		float fSlow43 = fSlow40 / fSlow41;
		float fSlow44 = fSlow38 * (fSlow42 + (1.0f - fSlow43));
		float fSlow45 = fSlow43 - fSlow42;
		float fSlow46 = std::exp(fConst29 / fSlow5);
		float fSlow47 = spat_zita6x8_faustpower2_f(fSlow46);
		float fSlow48 = 1.0f - fConst3 * fSlow47;
		float fSlow49 = 1.0f - fSlow47;
		float fSlow50 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow48) / spat_zita6x8_faustpower2_f(fSlow49) + -1.0f));
		float fSlow51 = fSlow48 / fSlow49;
		float fSlow52 = fSlow46 * (fSlow50 + (1.0f - fSlow51));
		float fSlow53 = fSlow51 - fSlow50;
		float fSlow54 = std::exp(fConst34 / fSlow5);
		float fSlow55 = spat_zita6x8_faustpower2_f(fSlow54);
		float fSlow56 = 1.0f - fConst3 * fSlow55;
		float fSlow57 = 1.0f - fSlow55;
		float fSlow58 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow56) / spat_zita6x8_faustpower2_f(fSlow57) + -1.0f));
		float fSlow59 = fSlow56 / fSlow57;
		float fSlow60 = fSlow54 * (fSlow58 + (1.0f - fSlow59));
		float fSlow61 = fSlow59 - fSlow58;
		float fSlow62 = std::exp(fConst39 / fSlow5);
		float fSlow63 = spat_zita6x8_faustpower2_f(fSlow62);
		float fSlow64 = 1.0f - fConst3 * fSlow63;
		float fSlow65 = 1.0f - fSlow63;
		float fSlow66 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow64) / spat_zita6x8_faustpower2_f(fSlow65) + -1.0f));
		float fSlow67 = fSlow64 / fSlow65;
		float fSlow68 = fSlow62 * (fSlow66 + (1.0f - fSlow67));
		float fSlow69 = fSlow67 - fSlow66;
		float fSlow70 = std::pow(1e+01f, 0.05f * float(fVslider4));
		float fSlow71 = float(fVslider5);
		float fSlow72 = fSlow71 + 1.0f;
		float fSlow73 = float(fVslider6);
		float fSlow74 = 0.0005f * fSlow72 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow71 * std::fabs(std::fmod(fSlow73 + 1.5f, 1.0f) + -0.5f)));
		float fSlow75 = 0.37f * std::pow(1e+01f, 0.05f * float(fVslider7));
		float fSlow76 = float(fVslider8);
		float fSlow77 = std::exp(fConst2 / fSlow76);
		float fSlow78 = spat_zita6x8_faustpower2_f(fSlow77);
		float fSlow79 = 1.0f - fConst3 * fSlow78;
		float fSlow80 = 1.0f - fSlow78;
		float fSlow81 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow79) / spat_zita6x8_faustpower2_f(fSlow80) + -1.0f));
		float fSlow82 = fSlow79 / fSlow80;
		float fSlow83 = fSlow77 * (fSlow81 + (1.0f - fSlow82));
		float fSlow84 = fSlow82 - fSlow81;
		float fSlow85 = std::exp(fConst29 / fSlow76);
		float fSlow86 = spat_zita6x8_faustpower2_f(fSlow85);
		float fSlow87 = 1.0f - fConst3 * fSlow86;
		float fSlow88 = 1.0f - fSlow86;
		float fSlow89 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow87) / spat_zita6x8_faustpower2_f(fSlow88) + -1.0f));
		float fSlow90 = fSlow87 / fSlow88;
		float fSlow91 = fSlow85 * (fSlow89 + (1.0f - fSlow90));
		float fSlow92 = fSlow90 - fSlow89;
		float fSlow93 = std::exp(fConst8 / fSlow76);
		float fSlow94 = spat_zita6x8_faustpower2_f(fSlow93);
		float fSlow95 = 1.0f - fConst3 * fSlow94;
		float fSlow96 = 1.0f - fSlow94;
		float fSlow97 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow95) / spat_zita6x8_faustpower2_f(fSlow96) + -1.0f));
		float fSlow98 = fSlow95 / fSlow96;
		float fSlow99 = fSlow93 * (fSlow97 + (1.0f - fSlow98));
		float fSlow100 = fSlow98 - fSlow97;
		float fSlow101 = std::exp(fConst13 / fSlow76);
		float fSlow102 = spat_zita6x8_faustpower2_f(fSlow101);
		float fSlow103 = 1.0f - fConst3 * fSlow102;
		float fSlow104 = 1.0f - fSlow102;
		float fSlow105 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow103) / spat_zita6x8_faustpower2_f(fSlow104) + -1.0f));
		float fSlow106 = fSlow103 / fSlow104;
		float fSlow107 = fSlow101 * (fSlow105 + (1.0f - fSlow106));
		float fSlow108 = fSlow106 - fSlow105;
		float fSlow109 = std::exp(fConst34 / fSlow76);
		float fSlow110 = spat_zita6x8_faustpower2_f(fSlow109);
		float fSlow111 = 1.0f - fConst3 * fSlow110;
		float fSlow112 = 1.0f - fSlow110;
		float fSlow113 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow111) / spat_zita6x8_faustpower2_f(fSlow112) + -1.0f));
		float fSlow114 = fSlow111 / fSlow112;
		float fSlow115 = fSlow109 * (fSlow113 + (1.0f - fSlow114));
		float fSlow116 = fSlow114 - fSlow113;
		float fSlow117 = std::exp(fConst18 / fSlow76);
		float fSlow118 = spat_zita6x8_faustpower2_f(fSlow117);
		float fSlow119 = 1.0f - fConst3 * fSlow118;
		float fSlow120 = 1.0f - fSlow118;
		float fSlow121 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow119) / spat_zita6x8_faustpower2_f(fSlow120) + -1.0f));
		float fSlow122 = fSlow119 / fSlow120;
		float fSlow123 = fSlow117 * (fSlow121 + (1.0f - fSlow122));
		float fSlow124 = fSlow122 - fSlow121;
		float fSlow125 = std::exp(fConst24 / fSlow76);
		float fSlow126 = spat_zita6x8_faustpower2_f(fSlow125);
		float fSlow127 = 1.0f - fConst3 * fSlow126;
		float fSlow128 = 1.0f - fSlow126;
		float fSlow129 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow127) / spat_zita6x8_faustpower2_f(fSlow128) + -1.0f));
		float fSlow130 = fSlow127 / fSlow128;
		float fSlow131 = fSlow125 * (fSlow129 + (1.0f - fSlow130));
		float fSlow132 = fSlow130 - fSlow129;
		float fSlow133 = std::exp(fConst39 / fSlow76);
		float fSlow134 = spat_zita6x8_faustpower2_f(fSlow133);
		float fSlow135 = 1.0f - fConst3 * fSlow134;
		float fSlow136 = 1.0f - fSlow134;
		float fSlow137 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow135) / spat_zita6x8_faustpower2_f(fSlow136) + -1.0f));
		float fSlow138 = fSlow135 / fSlow136;
		float fSlow139 = fSlow133 * (fSlow137 + (1.0f - fSlow138));
		float fSlow140 = fSlow138 - fSlow137;
		float fSlow141 = std::pow(1e+01f, 0.05f * float(fVslider9));
		float fSlow142 = float(fVslider10);
		float fSlow143 = fSlow142 + 1.0f;
		float fSlow144 = float(fVslider11);
		float fSlow145 = 0.0005f * fSlow143 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow142 * std::fabs(std::fmod(fSlow144 + 1.5f, 1.0f) + -0.5f)));
		float fSlow146 = 0.37f * std::pow(1e+01f, 0.05f * float(fVslider12));
		float fSlow147 = float(fVslider13);
		float fSlow148 = std::exp(fConst39 / fSlow147);
		float fSlow149 = spat_zita6x8_faustpower2_f(fSlow148);
		float fSlow150 = 1.0f - fConst3 * fSlow149;
		float fSlow151 = 1.0f - fSlow149;
		float fSlow152 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow150) / spat_zita6x8_faustpower2_f(fSlow151) + -1.0f));
		float fSlow153 = fSlow150 / fSlow151;
		float fSlow154 = fSlow148 * (fSlow152 + (1.0f - fSlow153));
		float fSlow155 = fSlow153 - fSlow152;
		float fSlow156 = std::exp(fConst29 / fSlow147);
		float fSlow157 = spat_zita6x8_faustpower2_f(fSlow156);
		float fSlow158 = 1.0f - fConst3 * fSlow157;
		float fSlow159 = 1.0f - fSlow157;
		float fSlow160 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow158) / spat_zita6x8_faustpower2_f(fSlow159) + -1.0f));
		float fSlow161 = fSlow158 / fSlow159;
		float fSlow162 = fSlow156 * (fSlow160 + (1.0f - fSlow161));
		float fSlow163 = fSlow161 - fSlow160;
		float fSlow164 = std::exp(fConst8 / fSlow147);
		float fSlow165 = spat_zita6x8_faustpower2_f(fSlow164);
		float fSlow166 = 1.0f - fConst3 * fSlow165;
		float fSlow167 = 1.0f - fSlow165;
		float fSlow168 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow166) / spat_zita6x8_faustpower2_f(fSlow167) + -1.0f));
		float fSlow169 = fSlow166 / fSlow167;
		float fSlow170 = fSlow164 * (fSlow168 + (1.0f - fSlow169));
		float fSlow171 = fSlow169 - fSlow168;
		float fSlow172 = std::exp(fConst13 / fSlow147);
		float fSlow173 = spat_zita6x8_faustpower2_f(fSlow172);
		float fSlow174 = 1.0f - fConst3 * fSlow173;
		float fSlow175 = 1.0f - fSlow173;
		float fSlow176 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow174) / spat_zita6x8_faustpower2_f(fSlow175) + -1.0f));
		float fSlow177 = fSlow174 / fSlow175;
		float fSlow178 = fSlow172 * (fSlow176 + (1.0f - fSlow177));
		float fSlow179 = fSlow177 - fSlow176;
		float fSlow180 = std::exp(fConst34 / fSlow147);
		float fSlow181 = spat_zita6x8_faustpower2_f(fSlow180);
		float fSlow182 = 1.0f - fConst3 * fSlow181;
		float fSlow183 = 1.0f - fSlow181;
		float fSlow184 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow182) / spat_zita6x8_faustpower2_f(fSlow183) + -1.0f));
		float fSlow185 = fSlow182 / fSlow183;
		float fSlow186 = fSlow180 * (fSlow184 + (1.0f - fSlow185));
		float fSlow187 = fSlow185 - fSlow184;
		float fSlow188 = std::exp(fConst18 / fSlow147);
		float fSlow189 = spat_zita6x8_faustpower2_f(fSlow188);
		float fSlow190 = 1.0f - fConst3 * fSlow189;
		float fSlow191 = 1.0f - fSlow189;
		float fSlow192 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow190) / spat_zita6x8_faustpower2_f(fSlow191) + -1.0f));
		float fSlow193 = fSlow190 / fSlow191;
		float fSlow194 = fSlow188 * (fSlow192 + (1.0f - fSlow193));
		float fSlow195 = fSlow193 - fSlow192;
		float fSlow196 = std::exp(fConst24 / fSlow147);
		float fSlow197 = spat_zita6x8_faustpower2_f(fSlow196);
		float fSlow198 = 1.0f - fConst3 * fSlow197;
		float fSlow199 = 1.0f - fSlow197;
		float fSlow200 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow198) / spat_zita6x8_faustpower2_f(fSlow199) + -1.0f));
		float fSlow201 = fSlow198 / fSlow199;
		float fSlow202 = fSlow196 * (fSlow200 + (1.0f - fSlow201));
		float fSlow203 = fSlow201 - fSlow200;
		float fSlow204 = std::exp(fConst2 / fSlow147);
		float fSlow205 = spat_zita6x8_faustpower2_f(fSlow204);
		float fSlow206 = 1.0f - fConst3 * fSlow205;
		float fSlow207 = 1.0f - fSlow205;
		float fSlow208 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow206) / spat_zita6x8_faustpower2_f(fSlow207) + -1.0f));
		float fSlow209 = fSlow206 / fSlow207;
		float fSlow210 = fSlow204 * (fSlow208 + (1.0f - fSlow209));
		float fSlow211 = fSlow209 - fSlow208;
		float fSlow212 = std::pow(1e+01f, 0.05f * float(fVslider14));
		float fSlow213 = float(fVslider15);
		float fSlow214 = fSlow213 + 1.0f;
		float fSlow215 = float(fVslider16);
		float fSlow216 = 0.0005f * fSlow214 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow213 * std::fabs(std::fmod(fSlow215 + 1.5f, 1.0f) + -0.5f)));
		float fSlow217 = 0.37f * std::pow(1e+01f, 0.05f * float(fVslider17));
		float fSlow218 = float(fVslider18);
		float fSlow219 = std::exp(fConst8 / fSlow218);
		float fSlow220 = spat_zita6x8_faustpower2_f(fSlow219);
		float fSlow221 = 1.0f - fConst3 * fSlow220;
		float fSlow222 = 1.0f - fSlow220;
		float fSlow223 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow221) / spat_zita6x8_faustpower2_f(fSlow222) + -1.0f));
		float fSlow224 = fSlow221 / fSlow222;
		float fSlow225 = fSlow219 * (fSlow223 + (1.0f - fSlow224));
		float fSlow226 = fSlow224 - fSlow223;
		float fSlow227 = std::exp(fConst13 / fSlow218);
		float fSlow228 = spat_zita6x8_faustpower2_f(fSlow227);
		float fSlow229 = 1.0f - fConst3 * fSlow228;
		float fSlow230 = 1.0f - fSlow228;
		float fSlow231 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow229) / spat_zita6x8_faustpower2_f(fSlow230) + -1.0f));
		float fSlow232 = fSlow229 / fSlow230;
		float fSlow233 = fSlow227 * (fSlow231 + (1.0f - fSlow232));
		float fSlow234 = fSlow232 - fSlow231;
		float fSlow235 = std::exp(fConst34 / fSlow218);
		float fSlow236 = spat_zita6x8_faustpower2_f(fSlow235);
		float fSlow237 = 1.0f - fConst3 * fSlow236;
		float fSlow238 = 1.0f - fSlow236;
		float fSlow239 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow237) / spat_zita6x8_faustpower2_f(fSlow238) + -1.0f));
		float fSlow240 = fSlow237 / fSlow238;
		float fSlow241 = fSlow235 * (fSlow239 + (1.0f - fSlow240));
		float fSlow242 = fSlow240 - fSlow239;
		float fSlow243 = std::exp(fConst18 / fSlow218);
		float fSlow244 = spat_zita6x8_faustpower2_f(fSlow243);
		float fSlow245 = 1.0f - fConst3 * fSlow244;
		float fSlow246 = 1.0f - fSlow244;
		float fSlow247 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow245) / spat_zita6x8_faustpower2_f(fSlow246) + -1.0f));
		float fSlow248 = fSlow245 / fSlow246;
		float fSlow249 = fSlow243 * (fSlow247 + (1.0f - fSlow248));
		float fSlow250 = fSlow248 - fSlow247;
		float fSlow251 = std::exp(fConst29 / fSlow218);
		float fSlow252 = spat_zita6x8_faustpower2_f(fSlow251);
		float fSlow253 = 1.0f - fConst3 * fSlow252;
		float fSlow254 = 1.0f - fSlow252;
		float fSlow255 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow253) / spat_zita6x8_faustpower2_f(fSlow254) + -1.0f));
		float fSlow256 = fSlow253 / fSlow254;
		float fSlow257 = fSlow251 * (fSlow255 + (1.0f - fSlow256));
		float fSlow258 = fSlow256 - fSlow255;
		float fSlow259 = std::exp(fConst24 / fSlow218);
		float fSlow260 = spat_zita6x8_faustpower2_f(fSlow259);
		float fSlow261 = 1.0f - fConst3 * fSlow260;
		float fSlow262 = 1.0f - fSlow260;
		float fSlow263 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow261) / spat_zita6x8_faustpower2_f(fSlow262) + -1.0f));
		float fSlow264 = fSlow261 / fSlow262;
		float fSlow265 = fSlow259 * (fSlow263 + (1.0f - fSlow264));
		float fSlow266 = fSlow264 - fSlow263;
		float fSlow267 = std::exp(fConst39 / fSlow218);
		float fSlow268 = spat_zita6x8_faustpower2_f(fSlow267);
		float fSlow269 = 1.0f - fConst3 * fSlow268;
		float fSlow270 = 1.0f - fSlow268;
		float fSlow271 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow269) / spat_zita6x8_faustpower2_f(fSlow270) + -1.0f));
		float fSlow272 = fSlow269 / fSlow270;
		float fSlow273 = fSlow267 * (fSlow271 + (1.0f - fSlow272));
		float fSlow274 = fSlow272 - fSlow271;
		float fSlow275 = std::exp(fConst2 / fSlow218);
		float fSlow276 = spat_zita6x8_faustpower2_f(fSlow275);
		float fSlow277 = 1.0f - fConst3 * fSlow276;
		float fSlow278 = 1.0f - fSlow276;
		float fSlow279 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow277) / spat_zita6x8_faustpower2_f(fSlow278) + -1.0f));
		float fSlow280 = fSlow277 / fSlow278;
		float fSlow281 = fSlow275 * (fSlow279 + (1.0f - fSlow280));
		float fSlow282 = fSlow280 - fSlow279;
		float fSlow283 = std::pow(1e+01f, 0.05f * float(fVslider19));
		float fSlow284 = float(fVslider20);
		float fSlow285 = fSlow284 + 1.0f;
		float fSlow286 = float(fVslider21);
		float fSlow287 = 0.0005f * fSlow285 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow284 * std::fabs(std::fmod(fSlow286 + 1.5f, 1.0f) + -0.5f)));
		float fSlow288 = 0.37f * std::pow(1e+01f, 0.05f * float(fVslider22));
		float fSlow289 = float(fVslider23);
		float fSlow290 = std::exp(fConst13 / fSlow289);
		float fSlow291 = spat_zita6x8_faustpower2_f(fSlow290);
		float fSlow292 = 1.0f - fConst3 * fSlow291;
		float fSlow293 = 1.0f - fSlow291;
		float fSlow294 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow292) / spat_zita6x8_faustpower2_f(fSlow293) + -1.0f));
		float fSlow295 = fSlow292 / fSlow293;
		float fSlow296 = fSlow290 * (fSlow294 + (1.0f - fSlow295));
		float fSlow297 = fSlow295 - fSlow294;
		float fSlow298 = std::exp(fConst2 / fSlow289);
		float fSlow299 = spat_zita6x8_faustpower2_f(fSlow298);
		float fSlow300 = 1.0f - fConst3 * fSlow299;
		float fSlow301 = 1.0f - fSlow299;
		float fSlow302 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow300) / spat_zita6x8_faustpower2_f(fSlow301) + -1.0f));
		float fSlow303 = fSlow300 / fSlow301;
		float fSlow304 = fSlow298 * (fSlow302 + (1.0f - fSlow303));
		float fSlow305 = fSlow303 - fSlow302;
		float fSlow306 = std::exp(fConst29 / fSlow289);
		float fSlow307 = spat_zita6x8_faustpower2_f(fSlow306);
		float fSlow308 = 1.0f - fConst3 * fSlow307;
		float fSlow309 = 1.0f - fSlow307;
		float fSlow310 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow308) / spat_zita6x8_faustpower2_f(fSlow309) + -1.0f));
		float fSlow311 = fSlow308 / fSlow309;
		float fSlow312 = fSlow306 * (fSlow310 + (1.0f - fSlow311));
		float fSlow313 = fSlow311 - fSlow310;
		float fSlow314 = std::exp(fConst34 / fSlow289);
		float fSlow315 = spat_zita6x8_faustpower2_f(fSlow314);
		float fSlow316 = 1.0f - fConst3 * fSlow315;
		float fSlow317 = 1.0f - fSlow315;
		float fSlow318 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow316) / spat_zita6x8_faustpower2_f(fSlow317) + -1.0f));
		float fSlow319 = fSlow316 / fSlow317;
		float fSlow320 = fSlow314 * (fSlow318 + (1.0f - fSlow319));
		float fSlow321 = fSlow319 - fSlow318;
		float fSlow322 = std::exp(fConst8 / fSlow289);
		float fSlow323 = spat_zita6x8_faustpower2_f(fSlow322);
		float fSlow324 = 1.0f - fConst3 * fSlow323;
		float fSlow325 = 1.0f - fSlow323;
		float fSlow326 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow324) / spat_zita6x8_faustpower2_f(fSlow325) + -1.0f));
		float fSlow327 = fSlow324 / fSlow325;
		float fSlow328 = fSlow322 * (fSlow326 + (1.0f - fSlow327));
		float fSlow329 = fSlow327 - fSlow326;
		float fSlow330 = std::exp(fConst18 / fSlow289);
		float fSlow331 = spat_zita6x8_faustpower2_f(fSlow330);
		float fSlow332 = 1.0f - fConst3 * fSlow331;
		float fSlow333 = 1.0f - fSlow331;
		float fSlow334 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow332) / spat_zita6x8_faustpower2_f(fSlow333) + -1.0f));
		float fSlow335 = fSlow332 / fSlow333;
		float fSlow336 = fSlow330 * (fSlow334 + (1.0f - fSlow335));
		float fSlow337 = fSlow335 - fSlow334;
		float fSlow338 = std::exp(fConst24 / fSlow289);
		float fSlow339 = spat_zita6x8_faustpower2_f(fSlow338);
		float fSlow340 = 1.0f - fConst3 * fSlow339;
		float fSlow341 = 1.0f - fSlow339;
		float fSlow342 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow340) / spat_zita6x8_faustpower2_f(fSlow341) + -1.0f));
		float fSlow343 = fSlow340 / fSlow341;
		float fSlow344 = fSlow338 * (fSlow342 + (1.0f - fSlow343));
		float fSlow345 = fSlow343 - fSlow342;
		float fSlow346 = std::exp(fConst39 / fSlow289);
		float fSlow347 = spat_zita6x8_faustpower2_f(fSlow346);
		float fSlow348 = 1.0f - fConst3 * fSlow347;
		float fSlow349 = 1.0f - fSlow347;
		float fSlow350 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow348) / spat_zita6x8_faustpower2_f(fSlow349) + -1.0f));
		float fSlow351 = fSlow348 / fSlow349;
		float fSlow352 = fSlow346 * (fSlow350 + (1.0f - fSlow351));
		float fSlow353 = fSlow351 - fSlow350;
		float fSlow354 = std::pow(1e+01f, 0.05f * float(fVslider24));
		float fSlow355 = float(fVslider25);
		float fSlow356 = fSlow355 + 1.0f;
		float fSlow357 = float(fVslider26);
		float fSlow358 = 0.0005f * fSlow356 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow355 * std::fabs(std::fmod(fSlow357 + 1.5f, 1.0f) + -0.5f)));
		float fSlow359 = 0.37f * std::pow(1e+01f, 0.05f * float(fVslider27));
		float fSlow360 = float(fVslider28);
		float fSlow361 = std::exp(fConst2 / fSlow360);
		float fSlow362 = spat_zita6x8_faustpower2_f(fSlow361);
		float fSlow363 = 1.0f - fConst3 * fSlow362;
		float fSlow364 = 1.0f - fSlow362;
		float fSlow365 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow363) / spat_zita6x8_faustpower2_f(fSlow364) + -1.0f));
		float fSlow366 = fSlow363 / fSlow364;
		float fSlow367 = fSlow361 * (fSlow365 + (1.0f - fSlow366));
		float fSlow368 = fSlow366 - fSlow365;
		float fSlow369 = std::exp(fConst29 / fSlow360);
		float fSlow370 = spat_zita6x8_faustpower2_f(fSlow369);
		float fSlow371 = 1.0f - fConst3 * fSlow370;
		float fSlow372 = 1.0f - fSlow370;
		float fSlow373 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow371) / spat_zita6x8_faustpower2_f(fSlow372) + -1.0f));
		float fSlow374 = fSlow371 / fSlow372;
		float fSlow375 = fSlow369 * (fSlow373 + (1.0f - fSlow374));
		float fSlow376 = fSlow374 - fSlow373;
		float fSlow377 = std::exp(fConst8 / fSlow360);
		float fSlow378 = spat_zita6x8_faustpower2_f(fSlow377);
		float fSlow379 = 1.0f - fConst3 * fSlow378;
		float fSlow380 = 1.0f - fSlow378;
		float fSlow381 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow379) / spat_zita6x8_faustpower2_f(fSlow380) + -1.0f));
		float fSlow382 = fSlow379 / fSlow380;
		float fSlow383 = fSlow377 * (fSlow381 + (1.0f - fSlow382));
		float fSlow384 = fSlow382 - fSlow381;
		float fSlow385 = std::exp(fConst13 / fSlow360);
		float fSlow386 = spat_zita6x8_faustpower2_f(fSlow385);
		float fSlow387 = 1.0f - fConst3 * fSlow386;
		float fSlow388 = 1.0f - fSlow386;
		float fSlow389 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow387) / spat_zita6x8_faustpower2_f(fSlow388) + -1.0f));
		float fSlow390 = fSlow387 / fSlow388;
		float fSlow391 = fSlow385 * (fSlow389 + (1.0f - fSlow390));
		float fSlow392 = fSlow390 - fSlow389;
		float fSlow393 = std::exp(fConst34 / fSlow360);
		float fSlow394 = spat_zita6x8_faustpower2_f(fSlow393);
		float fSlow395 = 1.0f - fConst3 * fSlow394;
		float fSlow396 = 1.0f - fSlow394;
		float fSlow397 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow395) / spat_zita6x8_faustpower2_f(fSlow396) + -1.0f));
		float fSlow398 = fSlow395 / fSlow396;
		float fSlow399 = fSlow393 * (fSlow397 + (1.0f - fSlow398));
		float fSlow400 = fSlow398 - fSlow397;
		float fSlow401 = std::exp(fConst18 / fSlow360);
		float fSlow402 = spat_zita6x8_faustpower2_f(fSlow401);
		float fSlow403 = 1.0f - fConst3 * fSlow402;
		float fSlow404 = 1.0f - fSlow402;
		float fSlow405 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow403) / spat_zita6x8_faustpower2_f(fSlow404) + -1.0f));
		float fSlow406 = fSlow403 / fSlow404;
		float fSlow407 = fSlow401 * (fSlow405 + (1.0f - fSlow406));
		float fSlow408 = fSlow406 - fSlow405;
		float fSlow409 = std::exp(fConst24 / fSlow360);
		float fSlow410 = spat_zita6x8_faustpower2_f(fSlow409);
		float fSlow411 = 1.0f - fConst3 * fSlow410;
		float fSlow412 = 1.0f - fSlow410;
		float fSlow413 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow411) / spat_zita6x8_faustpower2_f(fSlow412) + -1.0f));
		float fSlow414 = fSlow411 / fSlow412;
		float fSlow415 = fSlow409 * (fSlow413 + (1.0f - fSlow414));
		float fSlow416 = fSlow414 - fSlow413;
		float fSlow417 = std::exp(fConst39 / fSlow360);
		float fSlow418 = spat_zita6x8_faustpower2_f(fSlow417);
		float fSlow419 = 1.0f - fConst3 * fSlow418;
		float fSlow420 = 1.0f - fSlow418;
		float fSlow421 = std::sqrt(std::max<float>(0.0f, spat_zita6x8_faustpower2_f(fSlow419) / spat_zita6x8_faustpower2_f(fSlow420) + -1.0f));
		float fSlow422 = fSlow419 / fSlow420;
		float fSlow423 = fSlow417 * (fSlow421 + (1.0f - fSlow422));
		float fSlow424 = fSlow422 - fSlow421;
		float fSlow425 = std::pow(1e+01f, 0.05f * float(fVslider29));
		float fSlow426 = 0.0005f * fSlow72 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow71 * std::fabs(std::fmod(fSlow73 + 1.375f, 1.0f) + -0.5f)));
		float fSlow427 = 0.0005f * fSlow1 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow0 * std::fabs(std::fmod(fSlow2 + 1.375f, 1.0f) + -0.5f)));
		float fSlow428 = 0.0005f * fSlow143 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow142 * std::fabs(std::fmod(fSlow144 + 1.375f, 1.0f) + -0.5f)));
		float fSlow429 = 0.0005f * fSlow214 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow213 * std::fabs(std::fmod(fSlow215 + 1.375f, 1.0f) + -0.5f)));
		float fSlow430 = 0.0005f * fSlow285 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow284 * std::fabs(std::fmod(fSlow286 + 1.375f, 1.0f) + -0.5f)));
		float fSlow431 = 0.0005f * fSlow356 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow355 * std::fabs(std::fmod(fSlow357 + 1.375f, 1.0f) + -0.5f)));
		float fSlow432 = 0.0005f * fSlow143 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow142 * std::fabs(std::fmod(fSlow144 + 1.25f, 1.0f) + -0.5f)));
		float fSlow433 = 0.0005f * fSlow1 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow0 * std::fabs(std::fmod(fSlow2 + 1.25f, 1.0f) + -0.5f)));
		float fSlow434 = 0.0005f * fSlow72 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow71 * std::fabs(std::fmod(fSlow73 + 1.25f, 1.0f) + -0.5f)));
		float fSlow435 = 0.0005f * fSlow214 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow213 * std::fabs(std::fmod(fSlow215 + 1.25f, 1.0f) + -0.5f)));
		float fSlow436 = 0.0005f * fSlow285 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow284 * std::fabs(std::fmod(fSlow286 + 1.25f, 1.0f) + -0.5f)));
		float fSlow437 = 0.0005f * fSlow356 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow355 * std::fabs(std::fmod(fSlow357 + 1.25f, 1.0f) + -0.5f)));
		float fSlow438 = 0.0005f * fSlow214 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow213 * std::fabs(std::fmod(fSlow215 + 1.125f, 1.0f) + -0.5f)));
		float fSlow439 = 0.0005f * fSlow72 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow71 * std::fabs(std::fmod(fSlow73 + 1.125f, 1.0f) + -0.5f)));
		float fSlow440 = 0.0005f * fSlow143 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow142 * std::fabs(std::fmod(fSlow144 + 1.125f, 1.0f) + -0.5f)));
		float fSlow441 = 0.0005f * fSlow1 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow0 * std::fabs(std::fmod(fSlow2 + 1.125f, 1.0f) + -0.5f)));
		float fSlow442 = 0.0005f * fSlow285 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow284 * std::fabs(std::fmod(fSlow286 + 1.125f, 1.0f) + -0.5f)));
		float fSlow443 = 0.0005f * fSlow356 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow355 * std::fabs(std::fmod(fSlow357 + 1.125f, 1.0f) + -0.5f)));
		float fSlow444 = 0.0005f * fSlow214 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow213 * std::fabs(std::fmod(fSlow215 + 1.0f, 1.0f) + -0.5f)));
		float fSlow445 = 0.0005f * fSlow72 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow71 * std::fabs(std::fmod(fSlow73 + 1.0f, 1.0f) + -0.5f)));
		float fSlow446 = 0.0005f * fSlow1 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow0 * std::fabs(std::fmod(fSlow2 + 1.0f, 1.0f) + -0.5f)));
		float fSlow447 = 0.0005f * fSlow143 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow142 * std::fabs(std::fmod(fSlow144 + 1.0f, 1.0f) + -0.5f)));
		float fSlow448 = 0.0005f * fSlow285 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow284 * std::fabs(std::fmod(fSlow286 + 1.0f, 1.0f) + -0.5f)));
		float fSlow449 = 0.0005f * fSlow356 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow355 * std::fabs(std::fmod(fSlow357 + 1.0f, 1.0f) + -0.5f)));
		float fSlow450 = 0.0005f * fSlow143 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow142 * std::fabs(std::fmod(fSlow144 + 0.875f, 1.0f) + -0.5f)));
		float fSlow451 = 0.0005f * fSlow1 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow0 * std::fabs(std::fmod(fSlow2 + 0.875f, 1.0f) + -0.5f)));
		float fSlow452 = 0.0005f * fSlow72 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow71 * std::fabs(std::fmod(fSlow73 + 0.875f, 1.0f) + -0.5f)));
		float fSlow453 = 0.0005f * fSlow214 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow213 * std::fabs(std::fmod(fSlow215 + 0.875f, 1.0f) + -0.5f)));
		float fSlow454 = 0.0005f * fSlow285 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow284 * std::fabs(std::fmod(fSlow286 + 0.875f, 1.0f) + -0.5f)));
		float fSlow455 = 0.0005f * fSlow356 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow355 * std::fabs(std::fmod(fSlow357 + 0.875f, 1.0f) + -0.5f)));
		float fSlow456 = 0.0005f * fSlow214 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow213 * std::fabs(std::fmod(fSlow215 + 0.75f, 1.0f) + -0.5f)));
		float fSlow457 = 0.0005f * fSlow72 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow71 * std::fabs(std::fmod(fSlow73 + 0.75f, 1.0f) + -0.5f)));
		float fSlow458 = 0.0005f * fSlow1 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow0 * std::fabs(std::fmod(fSlow2 + 0.75f, 1.0f) + -0.5f)));
		float fSlow459 = 0.0005f * fSlow143 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow142 * std::fabs(std::fmod(fSlow144 + 0.75f, 1.0f) + -0.5f)));
		float fSlow460 = 0.0005f * fSlow285 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow284 * std::fabs(std::fmod(fSlow286 + 0.75f, 1.0f) + -0.5f)));
		float fSlow461 = 0.0005f * fSlow356 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow355 * std::fabs(std::fmod(fSlow357 + 0.75f, 1.0f) + -0.5f)));
		float fSlow462 = 0.0005f * fSlow143 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow142 * std::fabs(std::fmod(fSlow144 + 0.625f, 1.0f) + -0.5f)));
		float fSlow463 = 0.0005f * fSlow1 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow0 * std::fabs(std::fmod(fSlow2 + 0.625f, 1.0f) + -0.5f)));
		float fSlow464 = 0.0005f * fSlow72 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow71 * std::fabs(std::fmod(fSlow73 + 0.625f, 1.0f) + -0.5f)));
		float fSlow465 = 0.0005f * fSlow214 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow213 * std::fabs(std::fmod(fSlow215 + 0.625f, 1.0f) + -0.5f)));
		float fSlow466 = 0.0005f * fSlow285 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow284 * std::fabs(std::fmod(fSlow286 + 0.625f, 1.0f) + -0.5f)));
		float fSlow467 = 0.0005f * fSlow356 * q8_sqrt(std::max<float>(0.0f, 1.0f - 8.0f * fSlow355 * std::fabs(std::fmod(fSlow357 + 0.625f, 1.0f) + -0.5f)));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow3 + 0.999f * fRec0[1];
			fRec11[0] = fSlow12 * fRec8[1] + fSlow13 * fRec11[1];
			fVec0[IOTA0 & 32767] = 0.35355338f * fRec11[0] + 1e-20f;
			float fTemp0 = 0.6f * fRec9[1] + fVec0[(IOTA0 - iConst5) & 32767];
			fVec1[IOTA0 & 2047] = fTemp0;
			fRec9[0] = fVec1[(IOTA0 - iConst6) & 2047];
			float fRec10 = 0.0f - 0.6f * fTemp0;
			fRec14[0] = fSlow20 * fRec6[1] + fSlow21 * fRec14[1];
			fVec2[IOTA0 & 16383] = 0.35355338f * fRec14[0] + 1e-20f;
			float fTemp1 = 0.6f * fRec12[1] + fVec2[(IOTA0 - iConst10) & 16383];
			fVec3[IOTA0 & 4095] = fTemp1;
			fRec12[0] = fVec3[(IOTA0 - iConst11) & 4095];
			float fRec13 = 0.0f - 0.6f * fTemp1;
			fRec17[0] = fSlow28 * fRec2[1] + fSlow29 * fRec17[1];
			fVec4[IOTA0 & 32767] = 0.35355338f * fRec17[0] + 1e-20f;
			float fTemp2 = 0.6f * fRec15[1] + fVec4[(IOTA0 - iConst15) & 32767];
			fVec5[IOTA0 & 4095] = fTemp2;
			fRec15[0] = fVec5[(IOTA0 - iConst16) & 4095];
			float fRec16 = 0.0f - 0.6f * fTemp2;
			fRec20[0] = fSlow36 * fRec3[1] + fSlow37 * fRec20[1];
			fVec6[IOTA0 & 16383] = 0.35355338f * fRec20[0] + 1e-20f;
			float fTemp3 = float(input1[i0]);
			fVec7[IOTA0 & 8191] = fTemp3;
			float fTemp4 = 0.3f * fVec7[(IOTA0 - iConst21) & 8191];
			float fTemp5 = fVec6[(IOTA0 - iConst20) & 16383] - (fTemp4 + 0.6f * fRec18[1]);
			fVec8[IOTA0 & 4095] = fTemp5;
			fRec18[0] = fVec8[(IOTA0 - iConst22) & 4095];
			float fRec19 = 0.6f * fTemp5;
			fRec23[0] = fSlow44 * fRec5[1] + fSlow45 * fRec23[1];
			fVec9[IOTA0 & 16383] = 0.35355338f * fRec23[0] + 1e-20f;
			float fTemp6 = fTemp4 + fVec9[(IOTA0 - iConst26) & 16383] - 0.6f * fRec21[1];
			fVec10[IOTA0 & 4095] = fTemp6;
			fRec21[0] = fVec10[(IOTA0 - iConst27) & 4095];
			float fRec22 = 0.6f * fTemp6;
			fRec26[0] = fSlow52 * fRec4[1] + fSlow53 * fRec26[1];
			fVec11[IOTA0 & 32767] = 0.35355338f * fRec26[0] + 1e-20f;
			float fTemp7 = 0.6f * fRec24[1] + fVec11[(IOTA0 - iConst31) & 32767];
			fVec12[IOTA0 & 4095] = fTemp7;
			fRec24[0] = fVec12[(IOTA0 - iConst32) & 4095];
			float fRec25 = 0.0f - 0.6f * fTemp7;
			fRec29[0] = fSlow60 * fRec7[1] + fSlow61 * fRec29[1];
			fVec13[IOTA0 & 16383] = 0.35355338f * fRec29[0] + 1e-20f;
			float fTemp8 = fVec13[(IOTA0 - iConst36) & 16383] - (fTemp4 + 0.6f * fRec27[1]);
			fVec14[IOTA0 & 2047] = fTemp8;
			fRec27[0] = fVec14[(IOTA0 - iConst37) & 2047];
			float fRec28 = 0.6f * fTemp8;
			fRec32[0] = fSlow68 * fRec1[1] + fSlow69 * fRec32[1];
			fVec15[IOTA0 & 16383] = 0.35355338f * fRec32[0] + 1e-20f;
			float fTemp9 = fVec15[(IOTA0 - iConst41) & 16383] + fTemp4 - 0.6f * fRec30[1];
			fVec16[IOTA0 & 2047] = fTemp9;
			fRec30[0] = fVec16[(IOTA0 - iConst42) & 2047];
			float fRec31 = 0.6f * fTemp9;
			float fTemp10 = fRec31 + fRec22;
			float fTemp11 = fRec28 + fRec19 + fTemp10;
			fRec1[0] = fRec9[1] + fRec12[1] + fRec15[1] + fRec18[1] + fRec21[1] + fRec24[1] + fRec27[1] + fRec30[1] + fRec10 + fRec25 + fRec13 + fRec16 + fTemp11;
			fRec2[0] = fRec18[1] + fRec21[1] + fRec27[1] + fRec30[1] + fTemp11 - (fRec9[1] + fRec12[1] + fRec15[1] + fRec24[1] + fRec10 + fRec25 + fRec16 + fRec13);
			float fTemp12 = fRec19 + fRec28;
			fRec3[0] = fRec12[1] + fRec15[1] + fRec21[1] + fRec30[1] + fRec13 + fRec16 + fTemp10 - (fRec9[1] + fRec18[1] + fRec24[1] + fRec27[1] + fRec10 + fRec25 + fTemp12);
			fRec4[0] = fRec9[1] + fRec21[1] + fRec24[1] + fRec30[1] + fRec10 + fRec25 + fTemp10 - (fRec12[1] + fRec15[1] + fRec18[1] + fRec27[1] + fRec13 + fRec16 + fTemp12);
			float fTemp13 = fRec31 + fRec19;
			float fTemp14 = fRec22 + fRec28;
			fRec5[0] = fRec15[1] + fRec18[1] + fRec24[1] + fRec30[1] + fRec25 + fRec16 + fTemp13 - (fRec9[1] + fRec12[1] + fRec21[1] + fRec27[1] + fRec10 + fRec13 + fTemp14);
			fRec6[0] = fRec9[1] + fRec12[1] + fRec18[1] + fRec30[1] + fRec10 + fRec13 + fTemp13 - (fRec15[1] + fRec21[1] + fRec24[1] + fRec27[1] + fRec25 + fRec16 + fTemp14);
			float fTemp15 = fRec31 + fRec28;
			float fTemp16 = fRec22 + fRec19;
			fRec7[0] = fRec9[1] + fRec15[1] + fRec27[1] + fRec30[1] + fRec10 + fRec16 + fTemp15 - (fRec12[1] + fRec18[1] + fRec21[1] + fRec24[1] + fRec25 + fRec13 + fTemp16);
			fRec8[0] = fRec12[1] + fRec24[1] + fRec27[1] + fRec30[1] + fRec25 + fRec13 + fTemp15 - (fRec9[1] + fRec15[1] + fRec18[1] + fRec21[1] + fRec10 + fRec16 + fTemp16);
			float fTemp17 = fSlow4 * (fRec2[0] + fRec3[0]) + fSlow70 * fTemp3;
			fRec33[0] = fSlow74 + 0.999f * fRec33[1];
			fRec44[0] = fSlow83 * fRec41[1] + fSlow84 * fRec44[1];
			fVec17[IOTA0 & 32767] = 0.35355338f * fRec44[0] + 1e-20f;
			float fTemp18 = 0.6f * fRec42[1] + fVec17[(IOTA0 - iConst5) & 32767];
			fVec18[IOTA0 & 2047] = fTemp18;
			fRec42[0] = fVec18[(IOTA0 - iConst6) & 2047];
			float fRec43 = 0.0f - 0.6f * fTemp18;
			fRec47[0] = fSlow91 * fRec37[1] + fSlow92 * fRec47[1];
			fVec19[IOTA0 & 32767] = 0.35355338f * fRec47[0] + 1e-20f;
			float fTemp19 = 0.6f * fRec45[1] + fVec19[(IOTA0 - iConst31) & 32767];
			fVec20[IOTA0 & 4095] = fTemp19;
			fRec45[0] = fVec20[(IOTA0 - iConst32) & 4095];
			float fRec46 = 0.0f - 0.6f * fTemp19;
			fRec50[0] = fSlow99 * fRec39[1] + fSlow100 * fRec50[1];
			fVec21[IOTA0 & 16383] = 0.35355338f * fRec50[0] + 1e-20f;
			float fTemp20 = 0.6f * fRec48[1] + fVec21[(IOTA0 - iConst10) & 16383];
			fVec22[IOTA0 & 4095] = fTemp20;
			fRec48[0] = fVec22[(IOTA0 - iConst11) & 4095];
			float fRec49 = 0.0f - 0.6f * fTemp20;
			fRec53[0] = fSlow107 * fRec35[1] + fSlow108 * fRec53[1];
			fVec23[IOTA0 & 32767] = 0.35355338f * fRec53[0] + 1e-20f;
			float fTemp21 = 0.6f * fRec51[1] + fVec23[(IOTA0 - iConst15) & 32767];
			fVec24[IOTA0 & 4095] = fTemp21;
			fRec51[0] = fVec24[(IOTA0 - iConst16) & 4095];
			float fRec52 = 0.0f - 0.6f * fTemp21;
			fRec56[0] = fSlow115 * fRec40[1] + fSlow116 * fRec56[1];
			fVec25[IOTA0 & 16383] = 0.35355338f * fRec56[0] + 1e-20f;
			float fTemp22 = float(input2[i0]);
			fVec26[IOTA0 & 8191] = fTemp22;
			float fTemp23 = 0.3f * fVec26[(IOTA0 - iConst21) & 8191];
			float fTemp24 = fVec25[(IOTA0 - iConst36) & 16383] - (fTemp23 + 0.6f * fRec54[1]);
			fVec27[IOTA0 & 2047] = fTemp24;
			fRec54[0] = fVec27[(IOTA0 - iConst37) & 2047];
			float fRec55 = 0.6f * fTemp24;
			fRec59[0] = fSlow123 * fRec36[1] + fSlow124 * fRec59[1];
			fVec28[IOTA0 & 16383] = 0.35355338f * fRec59[0] + 1e-20f;
			float fTemp25 = fVec28[(IOTA0 - iConst20) & 16383] - (fTemp23 + 0.6f * fRec57[1]);
			fVec29[IOTA0 & 4095] = fTemp25;
			fRec57[0] = fVec29[(IOTA0 - iConst22) & 4095];
			float fRec58 = 0.6f * fTemp25;
			fRec62[0] = fSlow131 * fRec38[1] + fSlow132 * fRec62[1];
			fVec30[IOTA0 & 16383] = 0.35355338f * fRec62[0] + 1e-20f;
			float fTemp26 = fTemp23 + fVec30[(IOTA0 - iConst26) & 16383] - 0.6f * fRec60[1];
			fVec31[IOTA0 & 4095] = fTemp26;
			fRec60[0] = fVec31[(IOTA0 - iConst27) & 4095];
			float fRec61 = 0.6f * fTemp26;
			fRec65[0] = fSlow139 * fRec34[1] + fSlow140 * fRec65[1];
			fVec32[IOTA0 & 16383] = 0.35355338f * fRec65[0] + 1e-20f;
			float fTemp27 = fVec32[(IOTA0 - iConst41) & 16383] + fTemp23 - 0.6f * fRec63[1];
			fVec33[IOTA0 & 2047] = fTemp27;
			fRec63[0] = fVec33[(IOTA0 - iConst42) & 2047];
			float fRec64 = 0.6f * fTemp27;
			float fTemp28 = fRec46 + fRec43;
			float fTemp29 = fRec49 + fRec52 + fTemp28;
			fRec34[0] = fRec42[1] + fRec45[1] + fRec48[1] + fRec51[1] + fRec54[1] + fRec57[1] + fRec60[1] + fRec63[1] + fRec55 + fRec58 + fRec61 + fRec64 + fTemp29;
			fRec35[0] = fRec54[1] + fRec57[1] + fRec60[1] + fRec63[1] + fRec55 + fRec58 + fRec64 + fRec61 - (fRec42[1] + fRec45[1] + fRec48[1] + fRec51[1] + fTemp29);
			float fTemp30 = fRec52 + fRec49;
			fRec36[0] = fRec48[1] + fRec51[1] + fRec60[1] + fRec63[1] + fRec61 + fRec64 + fTemp30 - (fRec42[1] + fRec45[1] + fRec54[1] + fRec57[1] + fRec55 + fRec58 + fTemp28);
			fRec37[0] = fRec42[1] + fRec45[1] + fRec60[1] + fRec63[1] + fRec61 + fRec64 + fTemp28 - (fRec48[1] + fRec51[1] + fRec54[1] + fRec57[1] + fRec55 + fRec58 + fTemp30);
			float fTemp31 = fRec52 + fRec46;
			float fTemp32 = fRec49 + fRec43;
			fRec38[0] = fRec45[1] + fRec51[1] + fRec57[1] + fRec63[1] + fRec58 + fRec64 + fTemp31 - (fRec42[1] + fRec48[1] + fRec54[1] + fRec60[1] + fRec55 + fRec61 + fTemp32);
			fRec39[0] = fRec42[1] + fRec48[1] + fRec57[1] + fRec63[1] + fRec58 + fRec64 + fTemp32 - (fRec45[1] + fRec51[1] + fRec54[1] + fRec60[1] + fRec55 + fRec61 + fTemp31);
			float fTemp33 = fRec52 + fRec43;
			float fTemp34 = fRec49 + fRec46;
			fRec40[0] = fRec42[1] + fRec51[1] + fRec54[1] + fRec63[1] + fRec55 + fRec64 + fTemp33 - (fRec45[1] + fRec48[1] + fRec57[1] + fRec60[1] + fRec58 + fRec61 + fTemp34);
			fRec41[0] = fRec45[1] + fRec48[1] + fRec54[1] + fRec63[1] + fRec55 + fRec64 + fTemp34 - (fRec42[1] + fRec51[1] + fRec57[1] + fRec60[1] + fRec58 + fRec61 + fTemp33);
			float fTemp35 = fSlow75 * (fRec35[0] + fRec36[0]) + fSlow141 * fTemp22;
			fRec66[0] = fSlow145 + 0.999f * fRec66[1];
			fRec77[0] = fSlow154 * fRec67[1] + fSlow155 * fRec77[1];
			fVec34[IOTA0 & 16383] = 0.35355338f * fRec77[0] + 1e-20f;
			float fTemp36 = float(input0[i0]);
			fVec35[IOTA0 & 8191] = fTemp36;
			float fTemp37 = 0.3f * fVec35[(IOTA0 - iConst21) & 8191];
			float fTemp38 = fVec34[(IOTA0 - iConst41) & 16383] + fTemp37 - 0.6f * fRec75[1];
			fVec36[IOTA0 & 2047] = fTemp38;
			fRec75[0] = fVec36[(IOTA0 - iConst42) & 2047];
			float fRec76 = 0.6f * fTemp38;
			fRec80[0] = fSlow162 * fRec70[1] + fSlow163 * fRec80[1];
			fVec37[IOTA0 & 32767] = 0.35355338f * fRec80[0] + 1e-20f;
			float fTemp39 = 0.6f * fRec78[1] + fVec37[(IOTA0 - iConst31) & 32767];
			fVec38[IOTA0 & 4095] = fTemp39;
			fRec78[0] = fVec38[(IOTA0 - iConst32) & 4095];
			float fRec79 = 0.0f - 0.6f * fTemp39;
			fRec83[0] = fSlow170 * fRec72[1] + fSlow171 * fRec83[1];
			fVec39[IOTA0 & 16383] = 0.35355338f * fRec83[0] + 1e-20f;
			float fTemp40 = 0.6f * fRec81[1] + fVec39[(IOTA0 - iConst10) & 16383];
			fVec40[IOTA0 & 4095] = fTemp40;
			fRec81[0] = fVec40[(IOTA0 - iConst11) & 4095];
			float fRec82 = 0.0f - 0.6f * fTemp40;
			fRec86[0] = fSlow178 * fRec68[1] + fSlow179 * fRec86[1];
			fVec41[IOTA0 & 32767] = 0.35355338f * fRec86[0] + 1e-20f;
			float fTemp41 = 0.6f * fRec84[1] + fVec41[(IOTA0 - iConst15) & 32767];
			fVec42[IOTA0 & 4095] = fTemp41;
			fRec84[0] = fVec42[(IOTA0 - iConst16) & 4095];
			float fRec85 = 0.0f - 0.6f * fTemp41;
			fRec89[0] = fSlow186 * fRec73[1] + fSlow187 * fRec89[1];
			fVec43[IOTA0 & 16383] = 0.35355338f * fRec89[0] + 1e-20f;
			float fTemp42 = fVec43[(IOTA0 - iConst36) & 16383] - (fTemp37 + 0.6f * fRec87[1]);
			fVec44[IOTA0 & 2047] = fTemp42;
			fRec87[0] = fVec44[(IOTA0 - iConst37) & 2047];
			float fRec88 = 0.6f * fTemp42;
			fRec92[0] = fSlow194 * fRec69[1] + fSlow195 * fRec92[1];
			fVec45[IOTA0 & 16383] = 0.35355338f * fRec92[0] + 1e-20f;
			float fTemp43 = fVec45[(IOTA0 - iConst20) & 16383] - (fTemp37 + 0.6f * fRec90[1]);
			fVec46[IOTA0 & 4095] = fTemp43;
			fRec90[0] = fVec46[(IOTA0 - iConst22) & 4095];
			float fRec91 = 0.6f * fTemp43;
			fRec95[0] = fSlow202 * fRec71[1] + fSlow203 * fRec95[1];
			fVec47[IOTA0 & 16383] = 0.35355338f * fRec95[0] + 1e-20f;
			float fTemp44 = fTemp37 + fVec47[(IOTA0 - iConst26) & 16383] - 0.6f * fRec93[1];
			fVec48[IOTA0 & 4095] = fTemp44;
			fRec93[0] = fVec48[(IOTA0 - iConst27) & 4095];
			float fRec94 = 0.6f * fTemp44;
			fRec98[0] = fSlow210 * fRec74[1] + fSlow211 * fRec98[1];
			fVec49[IOTA0 & 32767] = 0.35355338f * fRec98[0] + 1e-20f;
			float fTemp45 = 0.6f * fRec96[1] + fVec49[(IOTA0 - iConst5) & 32767];
			fVec50[IOTA0 & 2047] = fTemp45;
			fRec96[0] = fVec50[(IOTA0 - iConst6) & 2047];
			float fRec97 = 0.0f - 0.6f * fTemp45;
			float fTemp46 = fRec97 + fRec96[1];
			float fTemp47 = fRec76 + fTemp46;
			float fTemp48 = fRec94 + fTemp47;
			fRec67[0] = fRec75[1] + fRec78[1] + fRec81[1] + fRec84[1] + fRec87[1] + fRec90[1] + fRec93[1] + fRec79 + fRec82 + fRec85 + fRec88 + fRec91 + fTemp48;
			float fTemp49 = fRec76 + fRec94;
			fRec68[0] = fRec75[1] + fRec87[1] + fRec90[1] + fRec93[1] + fRec88 + fRec91 + fTemp49 - (fRec78[1] + fRec81[1] + fRec84[1] + fRec79 + fRec82 + fRec85 + fTemp46);
			fRec69[0] = fRec75[1] + fRec81[1] + fRec84[1] + fRec93[1] + fRec82 + fRec85 + fTemp49 - (fRec78[1] + fRec87[1] + fRec90[1] + fRec79 + fRec88 + fRec91 + fTemp46);
			fRec70[0] = fRec75[1] + fRec78[1] + fRec93[1] + fRec79 + fTemp48 - (fRec81[1] + fRec84[1] + fRec87[1] + fRec90[1] + fRec82 + fRec85 + fRec91 + fRec88);
			float fTemp50 = fRec94 + fTemp46;
			fRec71[0] = fRec75[1] + fRec78[1] + fRec84[1] + fRec90[1] + fRec79 + fRec85 + fRec76 + fRec91 - (fRec81[1] + fRec87[1] + fRec93[1] + fRec82 + fRec88 + fTemp50);
			fRec72[0] = fRec75[1] + fRec81[1] + fRec90[1] + fRec82 + fRec91 + fTemp47 - (fRec78[1] + fRec84[1] + fRec87[1] + fRec93[1] + fRec79 + fRec85 + fRec94 + fRec88);
			fRec73[0] = fRec75[1] + fRec84[1] + fRec87[1] + fRec85 + fRec88 + fTemp47 - (fRec78[1] + fRec81[1] + fRec90[1] + fRec93[1] + fRec79 + fRec82 + fRec94 + fRec91);
			fRec74[0] = fRec75[1] + fRec78[1] + fRec81[1] + fRec87[1] + fRec79 + fRec82 + fRec76 + fRec88 - (fRec84[1] + fRec90[1] + fRec93[1] + fRec85 + fRec91 + fTemp50);
			float fTemp51 = fSlow146 * (fRec68[0] + fRec69[0]) + fSlow212 * fTemp36;
			fRec99[0] = fSlow216 + 0.999f * fRec99[1];
			fRec110[0] = fSlow225 * fRec105[1] + fSlow226 * fRec110[1];
			fVec51[IOTA0 & 16383] = 0.35355338f * fRec110[0] + 1e-20f;
			float fTemp52 = 0.6f * fRec108[1] + fVec51[(IOTA0 - iConst10) & 16383];
			fVec52[IOTA0 & 4095] = fTemp52;
			fRec108[0] = fVec52[(IOTA0 - iConst11) & 4095];
			float fRec109 = 0.0f - 0.6f * fTemp52;
			fRec113[0] = fSlow233 * fRec101[1] + fSlow234 * fRec113[1];
			fVec53[IOTA0 & 32767] = 0.35355338f * fRec113[0] + 1e-20f;
			float fTemp53 = 0.6f * fRec111[1] + fVec53[(IOTA0 - iConst15) & 32767];
			fVec54[IOTA0 & 4095] = fTemp53;
			fRec111[0] = fVec54[(IOTA0 - iConst16) & 4095];
			float fRec112 = 0.0f - 0.6f * fTemp53;
			fRec116[0] = fSlow241 * fRec106[1] + fSlow242 * fRec116[1];
			fVec55[IOTA0 & 16383] = 0.35355338f * fRec116[0] + 1e-20f;
			float fTemp54 = float(input3[i0]);
			fVec56[IOTA0 & 8191] = fTemp54;
			float fTemp55 = 0.3f * fVec56[(IOTA0 - iConst21) & 8191];
			float fTemp56 = fVec55[(IOTA0 - iConst36) & 16383] - (fTemp55 + 0.6f * fRec114[1]);
			fVec57[IOTA0 & 2047] = fTemp56;
			fRec114[0] = fVec57[(IOTA0 - iConst37) & 2047];
			float fRec115 = 0.6f * fTemp56;
			fRec119[0] = fSlow249 * fRec102[1] + fSlow250 * fRec119[1];
			fVec58[IOTA0 & 16383] = 0.35355338f * fRec119[0] + 1e-20f;
			float fTemp57 = fVec58[(IOTA0 - iConst20) & 16383] - (fTemp55 + 0.6f * fRec117[1]);
			fVec59[IOTA0 & 4095] = fTemp57;
			fRec117[0] = fVec59[(IOTA0 - iConst22) & 4095];
			float fRec118 = 0.6f * fTemp57;
			fRec122[0] = fSlow257 * fRec103[1] + fSlow258 * fRec122[1];
			fVec60[IOTA0 & 32767] = 0.35355338f * fRec122[0] + 1e-20f;
			float fTemp58 = 0.6f * fRec120[1] + fVec60[(IOTA0 - iConst31) & 32767];
			fVec61[IOTA0 & 4095] = fTemp58;
			fRec120[0] = fVec61[(IOTA0 - iConst32) & 4095];
			float fRec121 = 0.0f - 0.6f * fTemp58;
			fRec125[0] = fSlow265 * fRec104[1] + fSlow266 * fRec125[1];
			fVec62[IOTA0 & 16383] = 0.35355338f * fRec125[0] + 1e-20f;
			float fTemp59 = fTemp55 + fVec62[(IOTA0 - iConst26) & 16383] - 0.6f * fRec123[1];
			fVec63[IOTA0 & 4095] = fTemp59;
			fRec123[0] = fVec63[(IOTA0 - iConst27) & 4095];
			float fRec124 = 0.6f * fTemp59;
			fRec128[0] = fSlow273 * fRec100[1] + fSlow274 * fRec128[1];
			fVec64[IOTA0 & 16383] = 0.35355338f * fRec128[0] + 1e-20f;
			float fTemp60 = fVec64[(IOTA0 - iConst41) & 16383] + fTemp55 - 0.6f * fRec126[1];
			fVec65[IOTA0 & 2047] = fTemp60;
			fRec126[0] = fVec65[(IOTA0 - iConst42) & 2047];
			float fRec127 = 0.6f * fTemp60;
			fRec131[0] = fSlow281 * fRec107[1] + fSlow282 * fRec131[1];
			fVec66[IOTA0 & 32767] = 0.35355338f * fRec131[0] + 1e-20f;
			float fTemp61 = 0.6f * fRec129[1] + fVec66[(IOTA0 - iConst5) & 32767];
			fVec67[IOTA0 & 2047] = fTemp61;
			fRec129[0] = fVec67[(IOTA0 - iConst6) & 2047];
			float fRec130 = 0.0f - 0.6f * fTemp61;
			float fTemp62 = fRec127 + fRec130;
			float fTemp63 = fRec124 + fTemp62;
			fRec100[0] = fRec108[1] + fRec111[1] + fRec114[1] + fRec117[1] + fRec120[1] + fRec123[1] + fRec126[1] + fRec129[1] + fRec121 + fRec109 + fRec112 + fRec115 + fRec118 + fTemp63;
			float fTemp64 = fRec127 + fRec124;
			fRec101[0] = fRec114[1] + fRec117[1] + fRec123[1] + fRec126[1] + fRec115 + fRec118 + fTemp64 - (fRec108[1] + fRec111[1] + fRec120[1] + fRec129[1] + fRec121 + fRec109 + fRec112 + fRec130);
			fRec102[0] = fRec108[1] + fRec111[1] + fRec123[1] + fRec126[1] + fRec109 + fRec112 + fTemp64 - (fRec114[1] + fRec117[1] + fRec120[1] + fRec129[1] + fRec121 + fRec115 + fRec118 + fRec130);
			fRec103[0] = fRec120[1] + fRec123[1] + fRec126[1] + fRec129[1] + fRec121 + fTemp63 - (fRec108[1] + fRec111[1] + fRec114[1] + fRec117[1] + fRec109 + fRec112 + fRec118 + fRec115);
			float fTemp65 = fRec124 + fRec130;
			fRec104[0] = fRec111[1] + fRec117[1] + fRec120[1] + fRec126[1] + fRec121 + fRec112 + fRec127 + fRec118 - (fRec108[1] + fRec114[1] + fRec123[1] + fRec129[1] + fRec109 + fRec115 + fTemp65);
			fRec105[0] = fRec108[1] + fRec117[1] + fRec126[1] + fRec129[1] + fRec109 + fRec118 + fTemp62 - (fRec111[1] + fRec114[1] + fRec120[1] + fRec123[1] + fRec121 + fRec112 + fRec124 + fRec115);
			fRec106[0] = fRec111[1] + fRec114[1] + fRec126[1] + fRec129[1] + fRec112 + fRec115 + fTemp62 - (fRec108[1] + fRec117[1] + fRec120[1] + fRec123[1] + fRec121 + fRec109 + fRec124 + fRec118);
			fRec107[0] = fRec108[1] + fRec114[1] + fRec120[1] + fRec126[1] + fRec121 + fRec109 + fRec127 + fRec115 - (fRec111[1] + fRec117[1] + fRec123[1] + fRec129[1] + fRec112 + fRec118 + fTemp65);
			float fTemp66 = fSlow217 * (fRec101[0] + fRec102[0]) + fSlow283 * fTemp54;
			fRec132[0] = fSlow287 + 0.999f * fRec132[1];
			fRec143[0] = fSlow296 * fRec134[1] + fSlow297 * fRec143[1];
			fVec68[IOTA0 & 32767] = 0.35355338f * fRec143[0] + 1e-20f;
			float fTemp67 = 0.6f * fRec141[1] + fVec68[(IOTA0 - iConst15) & 32767];
			fVec69[IOTA0 & 4095] = fTemp67;
			fRec141[0] = fVec69[(IOTA0 - iConst16) & 4095];
			float fRec142 = 0.0f - 0.6f * fTemp67;
			fRec146[0] = fSlow304 * fRec140[1] + fSlow305 * fRec146[1];
			fVec70[IOTA0 & 32767] = 0.35355338f * fRec146[0] + 1e-20f;
			float fTemp68 = 0.6f * fRec144[1] + fVec70[(IOTA0 - iConst5) & 32767];
			fVec71[IOTA0 & 2047] = fTemp68;
			fRec144[0] = fVec71[(IOTA0 - iConst6) & 2047];
			float fRec145 = 0.0f - 0.6f * fTemp68;
			fRec149[0] = fSlow312 * fRec136[1] + fSlow313 * fRec149[1];
			fVec72[IOTA0 & 32767] = 0.35355338f * fRec149[0] + 1e-20f;
			float fTemp69 = 0.6f * fRec147[1] + fVec72[(IOTA0 - iConst31) & 32767];
			fVec73[IOTA0 & 4095] = fTemp69;
			fRec147[0] = fVec73[(IOTA0 - iConst32) & 4095];
			float fRec148 = 0.0f - 0.6f * fTemp69;
			fRec152[0] = fSlow320 * fRec139[1] + fSlow321 * fRec152[1];
			fVec74[IOTA0 & 16383] = 0.35355338f * fRec152[0] + 1e-20f;
			float fTemp70 = float(input4[i0]);
			fVec75[IOTA0 & 8191] = fTemp70;
			float fTemp71 = 0.3f * fVec75[(IOTA0 - iConst21) & 8191];
			float fTemp72 = fVec74[(IOTA0 - iConst36) & 16383] - (fTemp71 + 0.6f * fRec150[1]);
			fVec76[IOTA0 & 2047] = fTemp72;
			fRec150[0] = fVec76[(IOTA0 - iConst37) & 2047];
			float fRec151 = 0.6f * fTemp72;
			fRec155[0] = fSlow328 * fRec138[1] + fSlow329 * fRec155[1];
			fVec77[IOTA0 & 16383] = 0.35355338f * fRec155[0] + 1e-20f;
			float fTemp73 = 0.6f * fRec153[1] + fVec77[(IOTA0 - iConst10) & 16383];
			fVec78[IOTA0 & 4095] = fTemp73;
			fRec153[0] = fVec78[(IOTA0 - iConst11) & 4095];
			float fRec154 = 0.0f - 0.6f * fTemp73;
			fRec158[0] = fSlow336 * fRec135[1] + fSlow337 * fRec158[1];
			fVec79[IOTA0 & 16383] = 0.35355338f * fRec158[0] + 1e-20f;
			float fTemp74 = fVec79[(IOTA0 - iConst20) & 16383] - (fTemp71 + 0.6f * fRec156[1]);
			fVec80[IOTA0 & 4095] = fTemp74;
			fRec156[0] = fVec80[(IOTA0 - iConst22) & 4095];
			float fRec157 = 0.6f * fTemp74;
			fRec161[0] = fSlow344 * fRec137[1] + fSlow345 * fRec161[1];
			fVec81[IOTA0 & 16383] = 0.35355338f * fRec161[0] + 1e-20f;
			float fTemp75 = fTemp71 + fVec81[(IOTA0 - iConst26) & 16383] - 0.6f * fRec159[1];
			fVec82[IOTA0 & 4095] = fTemp75;
			fRec159[0] = fVec82[(IOTA0 - iConst27) & 4095];
			float fRec160 = 0.6f * fTemp75;
			fRec164[0] = fSlow352 * fRec133[1] + fSlow353 * fRec164[1];
			fVec83[IOTA0 & 16383] = 0.35355338f * fRec164[0] + 1e-20f;
			float fTemp76 = fVec83[(IOTA0 - iConst41) & 16383] + fTemp71 - 0.6f * fRec162[1];
			fVec84[IOTA0 & 2047] = fTemp76;
			fRec162[0] = fVec84[(IOTA0 - iConst42) & 2047];
			float fRec163 = 0.6f * fTemp76;
			float fTemp77 = fRec160 + fRec157;
			float fTemp78 = fRec163 + fRec151 + fTemp77;
			fRec133[0] = fRec141[1] + fRec144[1] + fRec147[1] + fRec150[1] + fRec153[1] + fRec156[1] + fRec159[1] + fRec162[1] + fRec142 + fRec145 + fRec148 + fRec154 + fTemp78;
			fRec134[0] = fRec150[1] + fRec156[1] + fRec159[1] + fRec162[1] + fTemp78 - (fRec141[1] + fRec144[1] + fRec147[1] + fRec153[1] + fRec142 + fRec145 + fRec154 + fRec148);
			float fTemp79 = fRec163 + fRec160;
			float fTemp80 = fRec157 + fRec151;
			fRec135[0] = fRec141[1] + fRec153[1] + fRec159[1] + fRec162[1] + fRec142 + fRec154 + fTemp79 - (fRec144[1] + fRec147[1] + fRec150[1] + fRec156[1] + fRec145 + fRec148 + fTemp80);
			fRec136[0] = fRec144[1] + fRec147[1] + fRec159[1] + fRec162[1] + fRec145 + fRec148 + fTemp79 - (fRec141[1] + fRec150[1] + fRec153[1] + fRec156[1] + fRec142 + fRec154 + fTemp80);
			float fTemp81 = fRec163 + fRec157;
			float fTemp82 = fRec160 + fRec151;
			fRec137[0] = fRec141[1] + fRec147[1] + fRec156[1] + fRec162[1] + fRec142 + fRec148 + fTemp81 - (fRec144[1] + fRec150[1] + fRec153[1] + fRec159[1] + fRec145 + fRec154 + fTemp82);
			fRec138[0] = fRec144[1] + fRec153[1] + fRec156[1] + fRec162[1] + fRec145 + fRec154 + fTemp81 - (fRec141[1] + fRec147[1] + fRec150[1] + fRec159[1] + fRec142 + fRec148 + fTemp82);
			float fTemp83 = fRec163 + fRec151;
			fRec139[0] = fRec141[1] + fRec144[1] + fRec150[1] + fRec162[1] + fRec142 + fRec145 + fTemp83 - (fRec147[1] + fRec153[1] + fRec156[1] + fRec159[1] + fRec148 + fRec154 + fTemp77);
			fRec140[0] = fRec147[1] + fRec150[1] + fRec153[1] + fRec162[1] + fRec148 + fRec154 + fTemp83 - (fRec141[1] + fRec144[1] + fRec156[1] + fRec159[1] + fRec142 + fRec145 + fTemp77);
			float fTemp84 = fSlow288 * (fRec134[0] + fRec135[0]) + fSlow354 * fTemp70;
			fRec165[0] = fSlow358 + 0.999f * fRec165[1];
			fRec176[0] = fSlow367 * fRec173[1] + fSlow368 * fRec176[1];
			fVec85[IOTA0 & 32767] = 0.35355338f * fRec176[0] + 1e-20f;
			float fTemp85 = 0.6f * fRec174[1] + fVec85[(IOTA0 - iConst5) & 32767];
			fVec86[IOTA0 & 2047] = fTemp85;
			fRec174[0] = fVec86[(IOTA0 - iConst6) & 2047];
			float fRec175 = 0.0f - 0.6f * fTemp85;
			fRec179[0] = fSlow375 * fRec169[1] + fSlow376 * fRec179[1];
			fVec87[IOTA0 & 32767] = 0.35355338f * fRec179[0] + 1e-20f;
			float fTemp86 = 0.6f * fRec177[1] + fVec87[(IOTA0 - iConst31) & 32767];
			fVec88[IOTA0 & 4095] = fTemp86;
			fRec177[0] = fVec88[(IOTA0 - iConst32) & 4095];
			float fRec178 = 0.0f - 0.6f * fTemp86;
			fRec182[0] = fSlow383 * fRec171[1] + fSlow384 * fRec182[1];
			fVec89[IOTA0 & 16383] = 0.35355338f * fRec182[0] + 1e-20f;
			float fTemp87 = 0.6f * fRec180[1] + fVec89[(IOTA0 - iConst10) & 16383];
			fVec90[IOTA0 & 4095] = fTemp87;
			fRec180[0] = fVec90[(IOTA0 - iConst11) & 4095];
			float fRec181 = 0.0f - 0.6f * fTemp87;
			fRec185[0] = fSlow391 * fRec167[1] + fSlow392 * fRec185[1];
			fVec91[IOTA0 & 32767] = 0.35355338f * fRec185[0] + 1e-20f;
			float fTemp88 = 0.6f * fRec183[1] + fVec91[(IOTA0 - iConst15) & 32767];
			fVec92[IOTA0 & 4095] = fTemp88;
			fRec183[0] = fVec92[(IOTA0 - iConst16) & 4095];
			float fRec184 = 0.0f - 0.6f * fTemp88;
			fRec188[0] = fSlow399 * fRec172[1] + fSlow400 * fRec188[1];
			fVec93[IOTA0 & 16383] = 0.35355338f * fRec188[0] + 1e-20f;
			float fTemp89 = float(input5[i0]);
			fVec94[IOTA0 & 8191] = fTemp89;
			float fTemp90 = 0.3f * fVec94[(IOTA0 - iConst21) & 8191];
			float fTemp91 = fVec93[(IOTA0 - iConst36) & 16383] - (fTemp90 + 0.6f * fRec186[1]);
			fVec95[IOTA0 & 2047] = fTemp91;
			fRec186[0] = fVec95[(IOTA0 - iConst37) & 2047];
			float fRec187 = 0.6f * fTemp91;
			fRec191[0] = fSlow407 * fRec168[1] + fSlow408 * fRec191[1];
			fVec96[IOTA0 & 16383] = 0.35355338f * fRec191[0] + 1e-20f;
			float fTemp92 = fVec96[(IOTA0 - iConst20) & 16383] - (fTemp90 + 0.6f * fRec189[1]);
			fVec97[IOTA0 & 4095] = fTemp92;
			fRec189[0] = fVec97[(IOTA0 - iConst22) & 4095];
			float fRec190 = 0.6f * fTemp92;
			fRec194[0] = fSlow415 * fRec170[1] + fSlow416 * fRec194[1];
			fVec98[IOTA0 & 16383] = 0.35355338f * fRec194[0] + 1e-20f;
			float fTemp93 = fTemp90 + fVec98[(IOTA0 - iConst26) & 16383] - 0.6f * fRec192[1];
			fVec99[IOTA0 & 4095] = fTemp93;
			fRec192[0] = fVec99[(IOTA0 - iConst27) & 4095];
			float fRec193 = 0.6f * fTemp93;
			fRec197[0] = fSlow423 * fRec166[1] + fSlow424 * fRec197[1];
			fVec100[IOTA0 & 16383] = 0.35355338f * fRec197[0] + 1e-20f;
			float fTemp94 = fVec100[(IOTA0 - iConst41) & 16383] + fTemp90 - 0.6f * fRec195[1];
			fVec101[IOTA0 & 2047] = fTemp94;
			fRec195[0] = fVec101[(IOTA0 - iConst42) & 2047];
			float fRec196 = 0.6f * fTemp94;
			float fTemp95 = fRec196 + fRec193;
			float fTemp96 = fRec181 + fTemp95;
			fRec166[0] = fRec174[1] + fRec177[1] + fRec180[1] + fRec183[1] + fRec186[1] + fRec189[1] + fRec192[1] + fRec184 + fRec187 + fRec190 + fRec195[1] + fRec175 + fRec178 + fTemp96;
			fRec167[0] = fRec186[1] + fRec189[1] + fRec192[1] + fRec187 + fRec190 + fRec195[1] + fTemp95 - (fRec174[1] + fRec177[1] + fRec180[1] + fRec183[1] + fRec184 + fRec175 + fRec181 + fRec178);
			fRec168[0] = fRec180[1] + fRec183[1] + fRec192[1] + fRec184 + fRec195[1] + fTemp96 - (fRec174[1] + fRec177[1] + fRec186[1] + fRec189[1] + fRec187 + fRec190 + fRec178 + fRec175);
			fRec169[0] = fRec174[1] + fRec177[1] + fRec192[1] + fRec195[1] + fRec175 + fRec178 + fTemp95 - (fRec180[1] + fRec183[1] + fRec186[1] + fRec189[1] + fRec184 + fRec187 + fRec190 + fRec181);
			float fTemp97 = fRec193 + fRec181;
			fRec170[0] = fRec177[1] + fRec183[1] + fRec189[1] + fRec184 + fRec190 + fRec195[1] + fRec196 + fRec178 - (fRec174[1] + fRec180[1] + fRec186[1] + fRec192[1] + fRec187 + fRec175 + fTemp97);
			float fTemp98 = fRec196 + fRec181;
			fRec171[0] = fRec174[1] + fRec180[1] + fRec189[1] + fRec190 + fRec195[1] + fRec175 + fTemp98 - (fRec177[1] + fRec183[1] + fRec186[1] + fRec192[1] + fRec184 + fRec187 + fRec193 + fRec178);
			fRec172[0] = fRec174[1] + fRec183[1] + fRec186[1] + fRec184 + fRec187 + fRec195[1] + fRec196 + fRec175 - (fRec177[1] + fRec180[1] + fRec189[1] + fRec192[1] + fRec190 + fRec178 + fTemp97);
			fRec173[0] = fRec177[1] + fRec180[1] + fRec186[1] + fRec187 + fRec195[1] + fRec178 + fTemp98 - (fRec174[1] + fRec183[1] + fRec189[1] + fRec192[1] + fRec184 + fRec190 + fRec193 + fRec175);
			float fTemp99 = fSlow359 * (fRec167[0] + fRec168[0]) + fSlow425 * fTemp89;
			output0[i0] = FAUSTFLOAT(fRec0[0] * fTemp17 + fRec33[0] * fTemp35 + fRec66[0] * fTemp51 + fRec99[0] * fTemp66 + fRec132[0] * fTemp84 + fRec165[0] * fTemp99);
			fRec198[0] = fSlow426 + 0.999f * fRec198[1];
			fRec199[0] = fSlow427 + 0.999f * fRec199[1];
			fRec200[0] = fSlow428 + 0.999f * fRec200[1];
			fRec201[0] = fSlow429 + 0.999f * fRec201[1];
			fRec202[0] = fSlow430 + 0.999f * fRec202[1];
			fRec203[0] = fSlow431 + 0.999f * fRec203[1];
			output1[i0] = FAUSTFLOAT(fRec198[0] * fTemp35 + fRec199[0] * fTemp17 + fRec200[0] * fTemp51 + fRec201[0] * fTemp66 + fRec202[0] * fTemp84 + fRec203[0] * fTemp99);
			fRec204[0] = fSlow432 + 0.999f * fRec204[1];
			fRec205[0] = fSlow433 + 0.999f * fRec205[1];
			fRec206[0] = fSlow434 + 0.999f * fRec206[1];
			fRec207[0] = fSlow435 + 0.999f * fRec207[1];
			fRec208[0] = fSlow436 + 0.999f * fRec208[1];
			fRec209[0] = fSlow437 + 0.999f * fRec209[1];
			output2[i0] = FAUSTFLOAT(fRec204[0] * fTemp51 + fRec205[0] * fTemp17 + fRec206[0] * fTemp35 + fRec207[0] * fTemp66 + fRec208[0] * fTemp84 + fRec209[0] * fTemp99);
			fRec210[0] = fSlow438 + 0.999f * fRec210[1];
			fRec211[0] = fSlow439 + 0.999f * fRec211[1];
			fRec212[0] = fSlow440 + 0.999f * fRec212[1];
			fRec213[0] = fSlow441 + 0.999f * fRec213[1];
			fRec214[0] = fSlow442 + 0.999f * fRec214[1];
			fRec215[0] = fSlow443 + 0.999f * fRec215[1];
			output3[i0] = FAUSTFLOAT(fRec210[0] * fTemp66 + fRec211[0] * fTemp35 + fRec212[0] * fTemp51 + fRec213[0] * fTemp17 + fRec214[0] * fTemp84 + fRec215[0] * fTemp99);
			fRec216[0] = fSlow444 + 0.999f * fRec216[1];
			fRec217[0] = fSlow445 + 0.999f * fRec217[1];
			fRec218[0] = fSlow446 + 0.999f * fRec218[1];
			fRec219[0] = fSlow447 + 0.999f * fRec219[1];
			fRec220[0] = fSlow448 + 0.999f * fRec220[1];
			fRec221[0] = fSlow449 + 0.999f * fRec221[1];
			output4[i0] = FAUSTFLOAT(fRec216[0] * fTemp66 + fRec217[0] * fTemp35 + fRec218[0] * fTemp17 + fRec219[0] * fTemp51 + fRec220[0] * fTemp84 + fRec221[0] * fTemp99);
			fRec222[0] = fSlow450 + 0.999f * fRec222[1];
			fRec223[0] = fSlow451 + 0.999f * fRec223[1];
			fRec224[0] = fSlow452 + 0.999f * fRec224[1];
			fRec225[0] = fSlow453 + 0.999f * fRec225[1];
			fRec226[0] = fSlow454 + 0.999f * fRec226[1];
			fRec227[0] = fSlow455 + 0.999f * fRec227[1];
			output5[i0] = FAUSTFLOAT(fRec222[0] * fTemp51 + fRec223[0] * fTemp17 + fRec224[0] * fTemp35 + fRec225[0] * fTemp66 + fRec226[0] * fTemp84 + fRec227[0] * fTemp99);
			fRec228[0] = fSlow456 + 0.999f * fRec228[1];
			fRec229[0] = fSlow457 + 0.999f * fRec229[1];
			fRec230[0] = fSlow458 + 0.999f * fRec230[1];
			fRec231[0] = fSlow459 + 0.999f * fRec231[1];
			fRec232[0] = fSlow460 + 0.999f * fRec232[1];
			fRec233[0] = fSlow461 + 0.999f * fRec233[1];
			output6[i0] = FAUSTFLOAT(fRec228[0] * fTemp66 + fRec229[0] * fTemp35 + fRec230[0] * fTemp17 + fRec231[0] * fTemp51 + fRec232[0] * fTemp84 + fRec233[0] * fTemp99);
			fRec234[0] = fSlow462 + 0.999f * fRec234[1];
			fRec235[0] = fSlow463 + 0.999f * fRec235[1];
			fRec236[0] = fSlow464 + 0.999f * fRec236[1];
			fRec237[0] = fSlow465 + 0.999f * fRec237[1];
			fRec238[0] = fSlow466 + 0.999f * fRec238[1];
			fRec239[0] = fSlow467 + 0.999f * fRec239[1];
			output7[i0] = FAUSTFLOAT(fRec234[0] * fTemp51 + fRec235[0] * fTemp17 + fRec236[0] * fTemp35 + fRec237[0] * fTemp66 + fRec238[0] * fTemp84 + fRec239[0] * fTemp99);
			fRec0[1] = fRec0[0];
			fRec11[1] = fRec11[0];
			IOTA0 = IOTA0 + 1;
			fRec9[1] = fRec9[0];
			fRec14[1] = fRec14[0];
			fRec12[1] = fRec12[0];
			fRec17[1] = fRec17[0];
			fRec15[1] = fRec15[0];
			fRec20[1] = fRec20[0];
			fRec18[1] = fRec18[0];
			fRec23[1] = fRec23[0];
			fRec21[1] = fRec21[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			fRec29[1] = fRec29[0];
			fRec27[1] = fRec27[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec33[1] = fRec33[0];
			fRec44[1] = fRec44[0];
			fRec42[1] = fRec42[0];
			fRec47[1] = fRec47[0];
			fRec45[1] = fRec45[0];
			fRec50[1] = fRec50[0];
			fRec48[1] = fRec48[0];
			fRec53[1] = fRec53[0];
			fRec51[1] = fRec51[0];
			fRec56[1] = fRec56[0];
			fRec54[1] = fRec54[0];
			fRec59[1] = fRec59[0];
			fRec57[1] = fRec57[0];
			fRec62[1] = fRec62[0];
			fRec60[1] = fRec60[0];
			fRec65[1] = fRec65[0];
			fRec63[1] = fRec63[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec66[1] = fRec66[0];
			fRec77[1] = fRec77[0];
			fRec75[1] = fRec75[0];
			fRec80[1] = fRec80[0];
			fRec78[1] = fRec78[0];
			fRec83[1] = fRec83[0];
			fRec81[1] = fRec81[0];
			fRec86[1] = fRec86[0];
			fRec84[1] = fRec84[0];
			fRec89[1] = fRec89[0];
			fRec87[1] = fRec87[0];
			fRec92[1] = fRec92[0];
			fRec90[1] = fRec90[0];
			fRec95[1] = fRec95[0];
			fRec93[1] = fRec93[0];
			fRec98[1] = fRec98[0];
			fRec96[1] = fRec96[0];
			fRec67[1] = fRec67[0];
			fRec68[1] = fRec68[0];
			fRec69[1] = fRec69[0];
			fRec70[1] = fRec70[0];
			fRec71[1] = fRec71[0];
			fRec72[1] = fRec72[0];
			fRec73[1] = fRec73[0];
			fRec74[1] = fRec74[0];
			fRec99[1] = fRec99[0];
			fRec110[1] = fRec110[0];
			fRec108[1] = fRec108[0];
			fRec113[1] = fRec113[0];
			fRec111[1] = fRec111[0];
			fRec116[1] = fRec116[0];
			fRec114[1] = fRec114[0];
			fRec119[1] = fRec119[0];
			fRec117[1] = fRec117[0];
			fRec122[1] = fRec122[0];
			fRec120[1] = fRec120[0];
			fRec125[1] = fRec125[0];
			fRec123[1] = fRec123[0];
			fRec128[1] = fRec128[0];
			fRec126[1] = fRec126[0];
			fRec131[1] = fRec131[0];
			fRec129[1] = fRec129[0];
			fRec100[1] = fRec100[0];
			fRec101[1] = fRec101[0];
			fRec102[1] = fRec102[0];
			fRec103[1] = fRec103[0];
			fRec104[1] = fRec104[0];
			fRec105[1] = fRec105[0];
			fRec106[1] = fRec106[0];
			fRec107[1] = fRec107[0];
			fRec132[1] = fRec132[0];
			fRec143[1] = fRec143[0];
			fRec141[1] = fRec141[0];
			fRec146[1] = fRec146[0];
			fRec144[1] = fRec144[0];
			fRec149[1] = fRec149[0];
			fRec147[1] = fRec147[0];
			fRec152[1] = fRec152[0];
			fRec150[1] = fRec150[0];
			fRec155[1] = fRec155[0];
			fRec153[1] = fRec153[0];
			fRec158[1] = fRec158[0];
			fRec156[1] = fRec156[0];
			fRec161[1] = fRec161[0];
			fRec159[1] = fRec159[0];
			fRec164[1] = fRec164[0];
			fRec162[1] = fRec162[0];
			fRec133[1] = fRec133[0];
			fRec134[1] = fRec134[0];
			fRec135[1] = fRec135[0];
			fRec136[1] = fRec136[0];
			fRec137[1] = fRec137[0];
			fRec138[1] = fRec138[0];
			fRec139[1] = fRec139[0];
			fRec140[1] = fRec140[0];
			fRec165[1] = fRec165[0];
			fRec176[1] = fRec176[0];
			fRec174[1] = fRec174[0];
			fRec179[1] = fRec179[0];
			fRec177[1] = fRec177[0];
			fRec182[1] = fRec182[0];
			fRec180[1] = fRec180[0];
			fRec185[1] = fRec185[0];
			fRec183[1] = fRec183[0];
			fRec188[1] = fRec188[0];
			fRec186[1] = fRec186[0];
			fRec191[1] = fRec191[0];
			fRec189[1] = fRec189[0];
			fRec194[1] = fRec194[0];
			fRec192[1] = fRec192[0];
			fRec197[1] = fRec197[0];
			fRec195[1] = fRec195[0];
			fRec166[1] = fRec166[0];
			fRec167[1] = fRec167[0];
			fRec168[1] = fRec168[0];
			fRec169[1] = fRec169[0];
			fRec170[1] = fRec170[0];
			fRec171[1] = fRec171[0];
			fRec172[1] = fRec172[0];
			fRec173[1] = fRec173[0];
			fRec198[1] = fRec198[0];
			fRec199[1] = fRec199[0];
			fRec200[1] = fRec200[0];
			fRec201[1] = fRec201[0];
			fRec202[1] = fRec202[0];
			fRec203[1] = fRec203[0];
			fRec204[1] = fRec204[0];
			fRec205[1] = fRec205[0];
			fRec206[1] = fRec206[0];
			fRec207[1] = fRec207[0];
			fRec208[1] = fRec208[0];
			fRec209[1] = fRec209[0];
			fRec210[1] = fRec210[0];
			fRec211[1] = fRec211[0];
			fRec212[1] = fRec212[0];
			fRec213[1] = fRec213[0];
			fRec214[1] = fRec214[0];
			fRec215[1] = fRec215[0];
			fRec216[1] = fRec216[0];
			fRec217[1] = fRec217[0];
			fRec218[1] = fRec218[0];
			fRec219[1] = fRec219[0];
			fRec220[1] = fRec220[0];
			fRec221[1] = fRec221[0];
			fRec222[1] = fRec222[0];
			fRec223[1] = fRec223[0];
			fRec224[1] = fRec224[0];
			fRec225[1] = fRec225[0];
			fRec226[1] = fRec226[0];
			fRec227[1] = fRec227[0];
			fRec228[1] = fRec228[0];
			fRec229[1] = fRec229[0];
			fRec230[1] = fRec230[0];
			fRec231[1] = fRec231[0];
			fRec232[1] = fRec232[0];
			fRec233[1] = fRec233[0];
			fRec234[1] = fRec234[0];
			fRec235[1] = fRec235[0];
			fRec236[1] = fRec236[0];
			fRec237[1] = fRec237[0];
			fRec238[1] = fRec238[0];
			fRec239[1] = fRec239[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _spat_zita6x8_UI : public UI {
    static std::string name;
};

template <class T>
std::string _spat_zita6x8_UI<T>::name(sym(spat_zita6x8));

typedef _spat_zita6x8_UI<spat_zita6x8> spat_zita6x8_UI;

class faust_spat_zita6x8_tilde : public FaustExternal<spat_zita6x8, spat_zita6x8_UI> {
public:
    faust_spat_zita6x8_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
