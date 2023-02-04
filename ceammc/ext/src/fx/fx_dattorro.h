/* ------------------------------------------------------------
name: "fx.dattorro"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_dattorro -scn fx_dattorro_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_dattorro_H__
#define  __fx_dattorro_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_dattorro_dsp.h ********************************
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

class FAUST_API fx_dattorro_dsp {

    public:

        fx_dattorro_dsp() {}
        virtual ~fx_dattorro_dsp() {}

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
        virtual fx_dattorro_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_dattorro_dsp {

    protected:

        fx_dattorro_dsp* fDSP;

    public:

        decorator_dsp(fx_dattorro_dsp* fx_dattorro_dsp = nullptr):fDSP(fx_dattorro_dsp) {}
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
    
        virtual fx_dattorro_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_dattorro_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_dattorro_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_dattorro : public fx_dattorro_dsp {
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
#define FAUSTCLASS fx_dattorro
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


class fx_dattorro : public fx_dattorro_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec0[2];
	FAUSTFLOAT fVslider0;
	float fRec12[2];
	int IOTA0;
	float fVec0[32768];
	float fConst3;
	float fConst4;
	FAUSTFLOAT fVslider1;
	float fRec13[2];
	float fRec11[3];
	FAUSTFLOAT fVslider2;
	float fRec14[2];
	float fVec1[256];
	float fRec9[2];
	float fVec2[128];
	float fRec7[2];
	FAUSTFLOAT fVslider3;
	float fRec15[2];
	float fVec3[512];
	float fRec5[2];
	float fVec4[512];
	float fRec3[2];
	FAUSTFLOAT fVslider4;
	float fRec16[2];
	FAUSTFLOAT fVslider5;
	float fRec20[2];
	FAUSTFLOAT fVslider6;
	float fRec23[2];
	float fVec5[1024];
	float fRec21[2];
	float fVec6[8192];
	float fRec19[2];
	FAUSTFLOAT fVslider7;
	float fRec24[2];
	float fVec7[4096];
	float fRec17[2];
	float fVec8[4096];
	float fRec1[2];
	float fVec9[1024];
	float fRec28[2];
	float fVec10[8192];
	float fRec27[2];
	float fVec11[2048];
	float fRec25[2];
	float fVec12[2048];
	float fRec2[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/bypass2:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_dattorro -scn fx_dattorro_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_dattorro.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "fx.dattorro");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 0.1f * fConst0 + 1.0f;
		fConst4 = 0.0441f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.33f);
		fVslider0 = FAUSTFLOAT(0.9995f);
		fVslider1 = FAUSTFLOAT(4e+01f);
		fVslider2 = FAUSTFLOAT(0.625f);
		fVslider3 = FAUSTFLOAT(0.75f);
		fVslider4 = FAUSTFLOAT(0.5f);
		fVslider5 = FAUSTFLOAT(0.0005f);
		fVslider6 = FAUSTFLOAT(0.5f);
		fVslider7 = FAUSTFLOAT(0.7f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec12[l1] = 0.0f;
		}
		IOTA0 = 0;
		for (int l2 = 0; l2 < 32768; l2 = l2 + 1) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec13[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = l4 + 1) {
			fRec11[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec14[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 256; l6 = l6 + 1) {
			fVec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec9[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 128; l8 = l8 + 1) {
			fVec2[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec7[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec15[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 512; l11 = l11 + 1) {
			fVec3[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec5[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 512; l13 = l13 + 1) {
			fVec4[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec3[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec16[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec20[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec23[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 1024; l18 = l18 + 1) {
			fVec5[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec21[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 8192; l20 = l20 + 1) {
			fVec6[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec19[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec24[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 4096; l23 = l23 + 1) {
			fVec7[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec17[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 4096; l25 = l25 + 1) {
			fVec8[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec1[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 1024; l27 = l27 + 1) {
			fVec9[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec28[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 8192; l29 = l29 + 1) {
			fVec10[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec27[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2048; l31 = l31 + 1) {
			fVec11[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec25[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2048; l33 = l33 + 1) {
			fVec12[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec2[l34] = 0.0f;
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
	
	virtual fx_dattorro* clone() {
		return new fx_dattorro();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.dattorro");
		ui_interface->addVerticalSlider("bw", &fVslider0, FAUSTFLOAT(0.9995f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addVerticalSlider("damp", &fVslider5, FAUSTFLOAT(0.0005f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addVerticalSlider("ddiff1", &fVslider6, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addVerticalSlider("ddiff2", &fVslider7, FAUSTFLOAT(0.7f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addVerticalSlider("decay", &fVslider4, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, FAUSTFLOAT(0.33f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addVerticalSlider("idiff1", &fVslider2, FAUSTFLOAT(0.625f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addVerticalSlider("idiff2", &fVslider3, FAUSTFLOAT(0.75f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "ms");
		ui_interface->addVerticalSlider("predelay", &fVslider1, FAUSTFLOAT(4e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = fConst1 * float(fHslider0);
		float fSlow2 = fConst1 * float(fVslider0);
		float fSlow3 = fConst4 * float(fVslider1);
		float fSlow4 = fConst1 * float(fVslider2);
		float fSlow5 = fConst1 * float(fVslider3);
		float fSlow6 = fConst1 * float(fVslider4);
		float fSlow7 = fConst1 * float(fVslider5);
		float fSlow8 = fConst1 * float(fVslider6);
		float fSlow9 = fConst1 * float(fVslider7);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			float fTemp1 = ((iSlow0) ? 0.0f : fTemp0);
			fRec0[0] = fSlow1 + fConst2 * fRec0[1];
			float fTemp2 = 1.0f - fRec0[0];
			fRec12[0] = fSlow2 + fConst2 * fRec12[1];
			float fTemp3 = float(input1[i0]);
			float fTemp4 = ((iSlow0) ? 0.0f : fTemp3);
			float fTemp5 = fTemp1 + fTemp4;
			fVec0[IOTA0 & 32767] = fTemp5;
			fRec13[0] = fSlow3 + fConst2 * fRec13[1];
			float fTemp6 = fConst0 * fRec13[0];
			int iTemp7 = int(fTemp6);
			float fTemp8 = std::floor(fTemp6);
			fRec11[0] = (1.0f - fRec12[0]) * fRec11[2] + 0.5f * fRec12[0] * (fVec0[(IOTA0 - int(std::min<float>(fConst3, float(std::max<int>(0, iTemp7))))) & 32767] * (fTemp8 + (1.0f - fTemp6)) + (fTemp6 - fTemp8) * fVec0[(IOTA0 - int(std::min<float>(fConst3, float(std::max<int>(0, iTemp7 + 1))))) & 32767]);
			fRec14[0] = fSlow4 + fConst2 * fRec14[1];
			float fTemp9 = fRec11[0] - fRec14[0] * fRec9[1];
			fVec1[IOTA0 & 255] = fTemp9;
			fRec9[0] = fVec1[(IOTA0 - 142) & 255];
			float fRec10 = fRec14[0] * fTemp9;
			float fTemp10 = fRec10 + fRec9[1] - fRec14[0] * fRec7[1];
			fVec2[IOTA0 & 127] = fTemp10;
			fRec7[0] = fVec2[(IOTA0 - 107) & 127];
			float fRec8 = fRec14[0] * fTemp10;
			fRec15[0] = fSlow5 + fConst2 * fRec15[1];
			float fTemp11 = fRec8 + fRec7[1] - fRec15[0] * fRec5[1];
			fVec3[IOTA0 & 511] = fTemp11;
			fRec5[0] = fVec3[(IOTA0 - 379) & 511];
			float fRec6 = fRec15[0] * fTemp11;
			float fTemp12 = fRec6 + fRec5[1] - fRec15[0] * fRec3[1];
			fVec4[IOTA0 & 511] = fTemp12;
			fRec3[0] = fVec4[(IOTA0 - 277) & 511];
			float fRec4 = fRec15[0] * fTemp12;
			fRec16[0] = fSlow6 + fConst2 * fRec16[1];
			fRec20[0] = fSlow7 + fConst2 * fRec20[1];
			float fTemp13 = 1.0f - fRec20[0];
			fRec23[0] = fSlow8 + fConst2 * fRec23[1];
			float fTemp14 = fRec23[0] * fRec21[1] + fRec2[1];
			fVec5[IOTA0 & 1023] = fTemp14;
			fRec21[0] = fVec5[(IOTA0 - 908) & 1023];
			float fRec22 = 0.0f - fRec23[0] * fTemp14;
			fVec6[IOTA0 & 8191] = fRec22 + fRec21[1];
			fRec19[0] = fRec20[0] * fRec19[1] + fTemp13 * fVec6[(IOTA0 - 4217) & 8191];
			fRec24[0] = fSlow9 + fConst2 * fRec24[1];
			float fTemp15 = fRec19[0] * fRec16[0] - fRec24[0] * fRec17[1];
			fVec7[IOTA0 & 4095] = fTemp15;
			fRec17[0] = fVec7[(IOTA0 - 2656) & 4095];
			float fRec18 = fRec24[0] * fTemp15;
			fVec8[IOTA0 & 4095] = fRec18 + fRec17[1];
			fRec1[0] = fRec4 + fRec16[0] * fVec8[(IOTA0 - 2656) & 4095] + fRec3[1];
			float fTemp16 = fRec23[0] * fRec28[1] + fRec1[1];
			fVec9[IOTA0 & 1023] = fTemp16;
			fRec28[0] = fVec9[(IOTA0 - 672) & 1023];
			float fRec29 = 0.0f - fRec23[0] * fTemp16;
			fVec10[IOTA0 & 8191] = fRec29 + fRec28[1];
			fRec27[0] = fRec20[0] * fRec27[1] + fTemp13 * fVec10[(IOTA0 - 4453) & 8191];
			float fTemp17 = fRec16[0] * fRec27[0] - fRec24[0] * fRec25[1];
			fVec11[IOTA0 & 2047] = fTemp17;
			fRec25[0] = fVec11[(IOTA0 - 1800) & 2047];
			float fRec26 = fRec24[0] * fTemp17;
			fVec12[IOTA0 & 2047] = fRec26 + fRec25[1];
			fRec2[0] = fRec4 + fRec3[1] + fRec16[0] * fVec12[(IOTA0 - 1800) & 2047];
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp0 : fTemp1 * fTemp2 + fRec0[0] * fRec1[0]));
			output1[i0] = FAUSTFLOAT(((iSlow0) ? fTemp3 : fTemp2 * fTemp4 + fRec0[0] * fRec2[0]));
			fRec0[1] = fRec0[0];
			fRec12[1] = fRec12[0];
			IOTA0 = IOTA0 + 1;
			fRec13[1] = fRec13[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec14[1] = fRec14[0];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec15[1] = fRec15[0];
			fRec5[1] = fRec5[0];
			fRec3[1] = fRec3[0];
			fRec16[1] = fRec16[0];
			fRec20[1] = fRec20[0];
			fRec23[1] = fRec23[0];
			fRec21[1] = fRec21[0];
			fRec19[1] = fRec19[0];
			fRec24[1] = fRec24[0];
			fRec17[1] = fRec17[0];
			fRec1[1] = fRec1[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec25[1] = fRec25[0];
			fRec2[1] = fRec2[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_dattorro_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_dattorro_UI<T>::name(sym(fx_dattorro));

typedef _fx_dattorro_UI<fx_dattorro> fx_dattorro_UI;

class faust_fx_dattorro_tilde : public FaustExternal<fx_dattorro, fx_dattorro_UI> {
public:
    faust_fx_dattorro_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
