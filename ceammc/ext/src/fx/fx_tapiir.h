/* ------------------------------------------------------------
author: "Grame"
copyright: "(c)GRAME 2006"
license: "BSD"
name: "fx.tapiir"
version: "1.0"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_tapiir -scn fx_tapiir_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_tapiir_H__
#define  __fx_tapiir_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_tapiir_dsp.h ********************************
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

class FAUST_API fx_tapiir_dsp {

    public:

        fx_tapiir_dsp() {}
        virtual ~fx_tapiir_dsp() {}

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
        virtual fx_tapiir_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_tapiir_dsp {

    protected:

        fx_tapiir_dsp* fDSP;

    public:

        decorator_dsp(fx_tapiir_dsp* fx_tapiir_dsp = nullptr):fDSP(fx_tapiir_dsp) {}
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
    
        virtual fx_tapiir_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_tapiir_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_tapiir_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_tapiir : public fx_tapiir_dsp {
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
#define FAUSTCLASS fx_tapiir
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


class fx_tapiir : public fx_tapiir_dsp {
	
 private:
	
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fVslider1;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fVslider5;
	FAUSTFLOAT fVslider6;
	FAUSTFLOAT fVslider7;
	FAUSTFLOAT fVslider8;
	FAUSTFLOAT fVslider9;
	FAUSTFLOAT fVslider10;
	FAUSTFLOAT fVslider11;
	int IOTA0;
	float fVec0[1048576];
	float fConst2;
	float fRec0[2];
	FAUSTFLOAT fVslider12;
	FAUSTFLOAT fVslider13;
	FAUSTFLOAT fVslider14;
	FAUSTFLOAT fVslider15;
	FAUSTFLOAT fVslider16;
	FAUSTFLOAT fVslider17;
	FAUSTFLOAT fVslider18;
	FAUSTFLOAT fVslider19;
	FAUSTFLOAT fVslider20;
	FAUSTFLOAT fVslider21;
	float fVec1[1048576];
	float fRec1[2];
	FAUSTFLOAT fVslider22;
	FAUSTFLOAT fVslider23;
	FAUSTFLOAT fVslider24;
	FAUSTFLOAT fVslider25;
	FAUSTFLOAT fVslider26;
	FAUSTFLOAT fVslider27;
	FAUSTFLOAT fVslider28;
	FAUSTFLOAT fVslider29;
	FAUSTFLOAT fVslider30;
	FAUSTFLOAT fVslider31;
	float fVec2[1048576];
	float fRec2[2];
	FAUSTFLOAT fVslider32;
	FAUSTFLOAT fVslider33;
	FAUSTFLOAT fVslider34;
	FAUSTFLOAT fVslider35;
	FAUSTFLOAT fVslider36;
	FAUSTFLOAT fVslider37;
	FAUSTFLOAT fVslider38;
	FAUSTFLOAT fVslider39;
	FAUSTFLOAT fVslider40;
	FAUSTFLOAT fVslider41;
	float fVec3[1048576];
	float fRec3[2];
	FAUSTFLOAT fVslider42;
	FAUSTFLOAT fVslider43;
	FAUSTFLOAT fVslider44;
	FAUSTFLOAT fVslider45;
	FAUSTFLOAT fVslider46;
	FAUSTFLOAT fVslider47;
	FAUSTFLOAT fVslider48;
	FAUSTFLOAT fVslider49;
	FAUSTFLOAT fVslider50;
	FAUSTFLOAT fVslider51;
	float fVec4[1048576];
	float fRec4[2];
	FAUSTFLOAT fVslider52;
	FAUSTFLOAT fVslider53;
	FAUSTFLOAT fVslider54;
	FAUSTFLOAT fVslider55;
	FAUSTFLOAT fVslider56;
	FAUSTFLOAT fVslider57;
	FAUSTFLOAT fVslider58;
	FAUSTFLOAT fVslider59;
	FAUSTFLOAT fVslider60;
	FAUSTFLOAT fVslider61;
	float fVec5[1048576];
	float fRec5[2];
	FAUSTFLOAT fVslider62;
	FAUSTFLOAT fVslider63;
	FAUSTFLOAT fVslider64;
	FAUSTFLOAT fVslider65;
	FAUSTFLOAT fVslider66;
	FAUSTFLOAT fVslider67;
	FAUSTFLOAT fVslider68;
	FAUSTFLOAT fVslider69;
	FAUSTFLOAT fVslider70;
	FAUSTFLOAT fVslider71;
	FAUSTFLOAT fVslider72;
	FAUSTFLOAT fVslider73;
	FAUSTFLOAT fVslider74;
	FAUSTFLOAT fVslider75;
	FAUSTFLOAT fVslider76;
	FAUSTFLOAT fVslider77;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Grame");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_tapiir -scn fx_tapiir_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("copyright", "(c)GRAME 2006");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_tapiir.dsp");
		m->declare("license", "BSD");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "fx.tapiir");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 0.001f * fConst0;
		fConst2 = 5.0f * fConst0 + 1.0f;
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fVslider2 = FAUSTFLOAT(0.0f);
		fVslider3 = FAUSTFLOAT(0.0f);
		fVslider4 = FAUSTFLOAT(0.0f);
		fVslider5 = FAUSTFLOAT(0.0f);
		fVslider6 = FAUSTFLOAT(0.0f);
		fVslider7 = FAUSTFLOAT(0.0f);
		fVslider8 = FAUSTFLOAT(0.0f);
		fVslider9 = FAUSTFLOAT(0.0f);
		fVslider10 = FAUSTFLOAT(1.0f);
		fVslider11 = FAUSTFLOAT(1.0f);
		fVslider12 = FAUSTFLOAT(0.0f);
		fVslider13 = FAUSTFLOAT(0.0f);
		fVslider14 = FAUSTFLOAT(0.0f);
		fVslider15 = FAUSTFLOAT(0.0f);
		fVslider16 = FAUSTFLOAT(0.0f);
		fVslider17 = FAUSTFLOAT(0.0f);
		fVslider18 = FAUSTFLOAT(0.0f);
		fVslider19 = FAUSTFLOAT(0.0f);
		fVslider20 = FAUSTFLOAT(1.0f);
		fVslider21 = FAUSTFLOAT(1.0f);
		fVslider22 = FAUSTFLOAT(0.0f);
		fVslider23 = FAUSTFLOAT(0.0f);
		fVslider24 = FAUSTFLOAT(0.0f);
		fVslider25 = FAUSTFLOAT(0.0f);
		fVslider26 = FAUSTFLOAT(0.0f);
		fVslider27 = FAUSTFLOAT(0.0f);
		fVslider28 = FAUSTFLOAT(0.0f);
		fVslider29 = FAUSTFLOAT(0.0f);
		fVslider30 = FAUSTFLOAT(1.0f);
		fVslider31 = FAUSTFLOAT(1.0f);
		fVslider32 = FAUSTFLOAT(0.0f);
		fVslider33 = FAUSTFLOAT(0.0f);
		fVslider34 = FAUSTFLOAT(0.0f);
		fVslider35 = FAUSTFLOAT(0.0f);
		fVslider36 = FAUSTFLOAT(0.0f);
		fVslider37 = FAUSTFLOAT(0.0f);
		fVslider38 = FAUSTFLOAT(0.0f);
		fVslider39 = FAUSTFLOAT(0.0f);
		fVslider40 = FAUSTFLOAT(1.0f);
		fVslider41 = FAUSTFLOAT(1.0f);
		fVslider42 = FAUSTFLOAT(0.0f);
		fVslider43 = FAUSTFLOAT(0.0f);
		fVslider44 = FAUSTFLOAT(0.0f);
		fVslider45 = FAUSTFLOAT(0.0f);
		fVslider46 = FAUSTFLOAT(0.0f);
		fVslider47 = FAUSTFLOAT(0.0f);
		fVslider48 = FAUSTFLOAT(0.0f);
		fVslider49 = FAUSTFLOAT(0.0f);
		fVslider50 = FAUSTFLOAT(1.0f);
		fVslider51 = FAUSTFLOAT(1.0f);
		fVslider52 = FAUSTFLOAT(0.0f);
		fVslider53 = FAUSTFLOAT(0.0f);
		fVslider54 = FAUSTFLOAT(1.0f);
		fVslider55 = FAUSTFLOAT(0.0f);
		fVslider56 = FAUSTFLOAT(0.0f);
		fVslider57 = FAUSTFLOAT(0.0f);
		fVslider58 = FAUSTFLOAT(0.0f);
		fVslider59 = FAUSTFLOAT(0.0f);
		fVslider60 = FAUSTFLOAT(0.0f);
		fVslider61 = FAUSTFLOAT(1.0f);
		fVslider62 = FAUSTFLOAT(0.0f);
		fVslider63 = FAUSTFLOAT(0.0f);
		fVslider64 = FAUSTFLOAT(0.0f);
		fVslider65 = FAUSTFLOAT(0.0f);
		fVslider66 = FAUSTFLOAT(0.0f);
		fVslider67 = FAUSTFLOAT(0.0f);
		fVslider68 = FAUSTFLOAT(0.0f);
		fVslider69 = FAUSTFLOAT(0.0f);
		fVslider70 = FAUSTFLOAT(0.0f);
		fVslider71 = FAUSTFLOAT(0.0f);
		fVslider72 = FAUSTFLOAT(0.0f);
		fVslider73 = FAUSTFLOAT(0.0f);
		fVslider74 = FAUSTFLOAT(0.0f);
		fVslider75 = FAUSTFLOAT(0.0f);
		fVslider76 = FAUSTFLOAT(0.0f);
		fVslider77 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 1048576; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 1048576; l2 = l2 + 1) {
			fVec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 1048576; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 1048576; l6 = l6 + 1) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec3[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 1048576; l8 = l8 + 1) {
			fVec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec4[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 1048576; l10 = l10 + 1) {
			fVec5[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec5[l11] = 0.0f;
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
	
	virtual fx_tapiir* clone() {
		return new fx_tapiir();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.tapiir");
		ui_interface->openTabBox("0x00");
		ui_interface->declare(&fVslider2, "unit", "ms");
		ui_interface->addVerticalSlider("tap0.delay", &fVslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("tap0.fb0", &fVslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap0.fb1", &fVslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap0.fb2", &fVslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap0.fb3", &fVslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap0.fb4", &fVslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap0.fb5", &fVslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider3, "unit", "db");
		ui_interface->addVerticalSlider("tap0.gain", &fVslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap0.in0", &fVslider10, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap0.in1", &fVslider11, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider12, "unit", "ms");
		ui_interface->addVerticalSlider("tap1.delay", &fVslider12, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("tap1.fb0", &fVslider18, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap1.fb1", &fVslider17, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap1.fb2", &fVslider16, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap1.fb3", &fVslider15, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap1.fb4", &fVslider14, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap1.fb5", &fVslider19, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider13, "unit", "db");
		ui_interface->addVerticalSlider("tap1.gain", &fVslider13, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap1.in0", &fVslider20, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap1.in1", &fVslider21, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider22, "unit", "ms");
		ui_interface->addVerticalSlider("tap2.delay", &fVslider22, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("tap2.fb0", &fVslider24, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap2.fb1", &fVslider25, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap2.fb2", &fVslider26, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap2.fb3", &fVslider27, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap2.fb4", &fVslider28, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap2.fb5", &fVslider29, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider23, "unit", "db");
		ui_interface->addVerticalSlider("tap2.gain", &fVslider23, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap2.in0", &fVslider30, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap2.in1", &fVslider31, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider32, "unit", "ms");
		ui_interface->addVerticalSlider("tap3.delay", &fVslider32, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("tap3.fb0", &fVslider34, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap3.fb1", &fVslider35, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap3.fb2", &fVslider36, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap3.fb3", &fVslider37, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap3.fb4", &fVslider38, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap3.fb5", &fVslider39, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider33, "unit", "db");
		ui_interface->addVerticalSlider("tap3.gain", &fVslider33, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap3.in0", &fVslider40, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap3.in1", &fVslider41, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider42, "unit", "ms");
		ui_interface->addVerticalSlider("tap4.delay", &fVslider42, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("tap4.fb0", &fVslider49, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap4.fb1", &fVslider48, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap4.fb2", &fVslider47, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap4.fb3", &fVslider46, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap4.fb4", &fVslider45, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap4.fb5", &fVslider44, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider43, "unit", "db");
		ui_interface->addVerticalSlider("tap4.gain", &fVslider43, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap4.in0", &fVslider50, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap4.in1", &fVslider51, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider52, "unit", "ms");
		ui_interface->addVerticalSlider("tap5.delay", &fVslider52, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("tap5.fb0", &fVslider56, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap5.fb1", &fVslider57, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap5.fb2", &fVslider58, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap5.fb3", &fVslider59, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap5.fb4", &fVslider60, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap5.fb5", &fVslider55, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider53, "unit", "db");
		ui_interface->addVerticalSlider("tap5.gain", &fVslider53, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap5.in0", &fVslider54, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("tap5.in1", &fVslider61, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
		ui_interface->declare(&fVslider0, "unit", "db");
		ui_interface->addVerticalSlider("out0.gain", &fVslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out0.in0", &fVslider67, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out0.in1", &fVslider68, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out0.tap0", &fVslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out0.tap1", &fVslider62, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out0.tap2", &fVslider63, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out0.tap3", &fVslider64, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out0.tap4", &fVslider65, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out0.tap5", &fVslider66, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider69, "unit", "db");
		ui_interface->addVerticalSlider("out1.gain", &fVslider69, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out1.in0", &fVslider76, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out1.in1", &fVslider77, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out1.tap0", &fVslider70, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out1.tap1", &fVslider71, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out1.tap2", &fVslider72, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out1.tap3", &fVslider73, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out1.tap4", &fVslider74, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->addVerticalSlider("out1.tap5", &fVslider75, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = std::pow(1e+01f, 0.05f * float(fVslider0));
		float fSlow1 = float(fVslider1);
		float fSlow2 = fConst1 * float(fVslider2);
		float fSlow3 = std::floor(fSlow2);
		float fSlow4 = fSlow3 + (1.0f - fSlow2);
		float fSlow5 = std::pow(1e+01f, 0.05f * float(fVslider3));
		float fSlow6 = float(fVslider4);
		float fSlow7 = float(fVslider5);
		float fSlow8 = float(fVslider6);
		float fSlow9 = float(fVslider7);
		float fSlow10 = float(fVslider8);
		float fSlow11 = float(fVslider9);
		float fSlow12 = float(fVslider10);
		float fSlow13 = float(fVslider11);
		int iSlow14 = int(fSlow2);
		int iSlow15 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow14))));
		float fSlow16 = fSlow2 - fSlow3;
		int iSlow17 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow14 + 1))));
		float fSlow18 = fConst1 * float(fVslider12);
		float fSlow19 = std::floor(fSlow18);
		float fSlow20 = fSlow19 + (1.0f - fSlow18);
		float fSlow21 = std::pow(1e+01f, 0.05f * float(fVslider13));
		float fSlow22 = float(fVslider14);
		float fSlow23 = float(fVslider15);
		float fSlow24 = float(fVslider16);
		float fSlow25 = float(fVslider17);
		float fSlow26 = float(fVslider18);
		float fSlow27 = float(fVslider19);
		float fSlow28 = float(fVslider20);
		float fSlow29 = float(fVslider21);
		int iSlow30 = int(fSlow18);
		int iSlow31 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow30))));
		float fSlow32 = fSlow18 - fSlow19;
		int iSlow33 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow30 + 1))));
		float fSlow34 = fConst1 * float(fVslider22);
		float fSlow35 = std::floor(fSlow34);
		float fSlow36 = fSlow35 + (1.0f - fSlow34);
		float fSlow37 = std::pow(1e+01f, 0.05f * float(fVslider23));
		float fSlow38 = float(fVslider24);
		float fSlow39 = float(fVslider25);
		float fSlow40 = float(fVslider26);
		float fSlow41 = float(fVslider27);
		float fSlow42 = float(fVslider28);
		float fSlow43 = float(fVslider29);
		float fSlow44 = float(fVslider30);
		float fSlow45 = float(fVslider31);
		int iSlow46 = int(fSlow34);
		int iSlow47 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow46))));
		float fSlow48 = fSlow34 - fSlow35;
		int iSlow49 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow46 + 1))));
		float fSlow50 = fConst1 * float(fVslider32);
		float fSlow51 = std::floor(fSlow50);
		float fSlow52 = fSlow51 + (1.0f - fSlow50);
		float fSlow53 = std::pow(1e+01f, 0.05f * float(fVslider33));
		float fSlow54 = float(fVslider34);
		float fSlow55 = float(fVslider35);
		float fSlow56 = float(fVslider36);
		float fSlow57 = float(fVslider37);
		float fSlow58 = float(fVslider38);
		float fSlow59 = float(fVslider39);
		float fSlow60 = float(fVslider40);
		float fSlow61 = float(fVslider41);
		int iSlow62 = int(fSlow50);
		int iSlow63 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow62))));
		float fSlow64 = fSlow50 - fSlow51;
		int iSlow65 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow62 + 1))));
		float fSlow66 = fConst1 * float(fVslider42);
		float fSlow67 = std::floor(fSlow66);
		float fSlow68 = fSlow67 + (1.0f - fSlow66);
		float fSlow69 = std::pow(1e+01f, 0.05f * float(fVslider43));
		float fSlow70 = float(fVslider44);
		float fSlow71 = float(fVslider45);
		float fSlow72 = float(fVslider46);
		float fSlow73 = float(fVslider47);
		float fSlow74 = float(fVslider48);
		float fSlow75 = float(fVslider49);
		float fSlow76 = float(fVslider50);
		float fSlow77 = float(fVslider51);
		int iSlow78 = int(fSlow66);
		int iSlow79 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow78))));
		float fSlow80 = fSlow66 - fSlow67;
		int iSlow81 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow78 + 1))));
		float fSlow82 = fConst1 * float(fVslider52);
		float fSlow83 = std::floor(fSlow82);
		float fSlow84 = fSlow83 + (1.0f - fSlow82);
		float fSlow85 = std::pow(1e+01f, 0.05f * float(fVslider53));
		float fSlow86 = float(fVslider54);
		float fSlow87 = float(fVslider55);
		float fSlow88 = float(fVslider56);
		float fSlow89 = float(fVslider57);
		float fSlow90 = float(fVslider58);
		float fSlow91 = float(fVslider59);
		float fSlow92 = float(fVslider60);
		float fSlow93 = float(fVslider61);
		int iSlow94 = int(fSlow82);
		int iSlow95 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow94))));
		float fSlow96 = fSlow82 - fSlow83;
		int iSlow97 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow94 + 1))));
		float fSlow98 = float(fVslider62);
		float fSlow99 = float(fVslider63);
		float fSlow100 = float(fVslider64);
		float fSlow101 = float(fVslider65);
		float fSlow102 = float(fVslider66);
		float fSlow103 = float(fVslider67);
		float fSlow104 = float(fVslider68);
		float fSlow105 = std::pow(1e+01f, 0.05f * float(fVslider69));
		float fSlow106 = float(fVslider70);
		float fSlow107 = float(fVslider71);
		float fSlow108 = float(fVslider72);
		float fSlow109 = float(fVslider73);
		float fSlow110 = float(fVslider74);
		float fSlow111 = float(fVslider75);
		float fSlow112 = float(fVslider76);
		float fSlow113 = float(fVslider77);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			float fTemp1 = float(input1[i0]);
			float fTemp2 = fSlow5 * (fSlow6 * fRec5[1] + fSlow7 * fRec4[1] + fSlow8 * fRec3[1] + fSlow9 * fRec2[1] + fSlow10 * fRec1[1] + fSlow11 * fRec0[1] + fSlow12 * fTemp0 + fSlow13 * fTemp1);
			fVec0[IOTA0 & 1048575] = fTemp2;
			fRec0[0] = fSlow4 * fVec0[(IOTA0 - iSlow15) & 1048575] + fSlow16 * fVec0[(IOTA0 - iSlow17) & 1048575];
			float fTemp3 = fSlow21 * (fSlow22 * fRec4[1] + fSlow23 * fRec3[1] + fSlow24 * fRec2[1] + fSlow25 * fRec1[1] + fSlow26 * fRec0[1] + fSlow27 * fRec5[1] + fSlow28 * fTemp0 + fSlow29 * fTemp1);
			fVec1[IOTA0 & 1048575] = fTemp3;
			fRec1[0] = fSlow20 * fVec1[(IOTA0 - iSlow31) & 1048575] + fSlow32 * fVec1[(IOTA0 - iSlow33) & 1048575];
			float fTemp4 = fSlow37 * (fSlow38 * fRec0[1] + fSlow39 * fRec1[1] + fSlow40 * fRec2[1] + fSlow41 * fRec3[1] + fSlow42 * fRec4[1] + fSlow43 * fRec5[1] + fSlow44 * fTemp0 + fSlow45 * fTemp1);
			fVec2[IOTA0 & 1048575] = fTemp4;
			fRec2[0] = fSlow36 * fVec2[(IOTA0 - iSlow47) & 1048575] + fSlow48 * fVec2[(IOTA0 - iSlow49) & 1048575];
			float fTemp5 = fSlow53 * (fSlow54 * fRec0[1] + fSlow55 * fRec1[1] + fSlow56 * fRec2[1] + fSlow57 * fRec3[1] + fSlow58 * fRec4[1] + fSlow59 * fRec5[1] + fSlow60 * fTemp0 + fSlow61 * fTemp1);
			fVec3[IOTA0 & 1048575] = fTemp5;
			fRec3[0] = fSlow52 * fVec3[(IOTA0 - iSlow63) & 1048575] + fSlow64 * fVec3[(IOTA0 - iSlow65) & 1048575];
			float fTemp6 = fSlow69 * (fSlow70 * fRec5[1] + fSlow71 * fRec4[1] + fSlow72 * fRec3[1] + fSlow73 * fRec2[1] + fSlow74 * fRec1[1] + fSlow75 * fRec0[1] + fSlow76 * fTemp0 + fSlow77 * fTemp1);
			fVec4[IOTA0 & 1048575] = fTemp6;
			fRec4[0] = fSlow68 * fVec4[(IOTA0 - iSlow79) & 1048575] + fSlow80 * fVec4[(IOTA0 - iSlow81) & 1048575];
			float fTemp7 = fSlow85 * (fSlow86 * fTemp0 + fSlow87 * fRec5[1] + fSlow88 * fRec0[1] + fSlow89 * fRec1[1] + fSlow90 * fRec2[1] + fSlow91 * fRec3[1] + fSlow92 * fRec4[1] + fSlow93 * fTemp1);
			fVec5[IOTA0 & 1048575] = fTemp7;
			fRec5[0] = fSlow84 * fVec5[(IOTA0 - iSlow95) & 1048575] + fSlow96 * fVec5[(IOTA0 - iSlow97) & 1048575];
			output0[i0] = FAUSTFLOAT(fSlow0 * (fSlow1 * fRec0[0] + fSlow98 * fRec1[0] + fSlow99 * fRec2[0] + fSlow100 * fRec3[0] + fSlow101 * fRec4[0] + fSlow102 * fRec5[0] + fSlow103 * fTemp0 + fSlow104 * fTemp1));
			output1[i0] = FAUSTFLOAT(fSlow105 * (fSlow106 * fRec0[0] + fSlow107 * fRec1[0] + fSlow108 * fRec2[0] + fSlow109 * fRec3[0] + fSlow110 * fRec4[0] + fSlow111 * fRec5[0] + fSlow112 * fTemp0 + fSlow113 * fTemp1));
			IOTA0 = IOTA0 + 1;
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_tapiir_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_tapiir_UI<T>::name(sym(fx_tapiir));

typedef _fx_tapiir_UI<fx_tapiir> fx_tapiir_UI;

class faust_fx_tapiir_tilde : public FaustExternal<fx_tapiir, fx_tapiir_UI> {
public:
    faust_fx_tapiir_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
