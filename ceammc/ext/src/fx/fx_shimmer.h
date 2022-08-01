/* ------------------------------------------------------------
author: "thedrgreenthumb"
license: "MIT"
name: "fx_shimmer"
Code generated with Faust 2.44.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_shimmer -scn fx_shimmer_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_shimmer_H__
#define  __fx_shimmer_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_shimmer_dsp.h ********************************
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

class FAUST_API fx_shimmer_dsp {

    public:

        fx_shimmer_dsp() {}
        virtual ~fx_shimmer_dsp() {}

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
        virtual fx_shimmer_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_shimmer_dsp {

    protected:

        fx_shimmer_dsp* fDSP;

    public:

        decorator_dsp(fx_shimmer_dsp* fx_shimmer_dsp = nullptr):fDSP(fx_shimmer_dsp) {}
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
    
        virtual fx_shimmer_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_shimmer_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_shimmer_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_shimmer : public fx_shimmer_dsp {
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
#define FAUSTCLASS fx_shimmer
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

class fx_shimmerSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec15[2];
	
  public:
	
	int getNumInputsfx_shimmerSIG0() {
		return 0;
	}
	int getNumOutputsfx_shimmerSIG0() {
		return 1;
	}
	
	void instanceInitfx_shimmerSIG0(int sample_rate) {
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iVec1[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			iRec15[l6] = 0;
		}
	}
	
	void fillfx_shimmerSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[0] = 1;
			iRec15[0] = (iVec1[1] + iRec15[1]) % 65536;
			table[i1] = std::sin(9.58738019e-05f * float(iRec15[0]));
			iVec1[1] = iVec1[0];
			iRec15[1] = iRec15[0];
		}
	}

};

static fx_shimmerSIG0* newfx_shimmerSIG0() { return (fx_shimmerSIG0*)new fx_shimmerSIG0(); }
static void deletefx_shimmerSIG0(fx_shimmerSIG0* dsp) { delete dsp; }

static float fx_shimmer_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0fx_shimmerSIG0[65536];

class fx_shimmer : public fx_shimmer_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fConst4;
	FAUSTFLOAT fHslider2;
	float fConst5;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fConst6;
	FAUSTFLOAT fHslider5;
	float fRec12[2];
	float fRec11[2];
	int IOTA0;
	float fVec0[131072];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHslider7;
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	FAUSTFLOAT fHslider10;
	float fConst7;
	float fRec14[2];
	FAUSTFLOAT fHslider11;
	float fRec16[2];
	float fRec13[2];
	float fVec2[32768];
	int iConst9;
	float fVec3[4096];
	int iConst10;
	float fRec9[2];
	float fConst12;
	float fRec20[2];
	float fRec19[2];
	float fVec4[131072];
	float fRec22[2];
	float fRec21[2];
	float fVec5[65536];
	int iConst14;
	float fVec6[4096];
	int iConst15;
	float fRec17[2];
	float fConst17;
	float fRec26[2];
	float fRec25[2];
	float fVec7[131072];
	float fRec28[2];
	float fRec27[2];
	float fVec8[65536];
	int iConst19;
	float fVec9[8192];
	int iConst20;
	float fRec23[2];
	float fConst22;
	float fRec32[2];
	float fRec31[2];
	float fVec10[131072];
	float fRec34[2];
	float fRec33[2];
	float fVec11[32768];
	int iConst24;
	float fVec12[8192];
	int iConst25;
	float fRec29[2];
	float fConst27;
	float fRec38[2];
	float fRec37[2];
	float fVec13[131072];
	float fRec40[2];
	float fRec39[2];
	float fVec14[65536];
	int iConst29;
	float fVec15[8192];
	int iConst30;
	float fRec35[2];
	float fConst32;
	float fRec44[2];
	float fRec43[2];
	float fVec16[131072];
	float fRec46[2];
	float fRec45[2];
	float fVec17[32768];
	int iConst34;
	float fVec18[4096];
	int iConst35;
	float fRec41[2];
	float fConst37;
	float fRec50[2];
	float fRec49[2];
	float fVec19[131072];
	float fRec52[2];
	float fRec51[2];
	float fVec20[32768];
	int iConst39;
	float fVec21[8192];
	int iConst40;
	float fRec47[2];
	float fConst42;
	float fRec56[2];
	float fRec55[2];
	float fVec22[131072];
	float fRec58[2];
	float fRec57[2];
	float fVec23[32768];
	int iConst44;
	float fVec24[8192];
	int iConst45;
	float fRec53[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.2");
		m->declare("author", "thedrgreenthumb");
		m->declare("basics.lib/bypass2:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_shimmer -scn fx_shimmer_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_shimmer.dsp");
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
		m->declare("id", "fx.shimmer");
		m->declare("license", "MIT");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "fx_shimmer");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.3");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
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
		fx_shimmerSIG0* sig0 = newfx_shimmerSIG0();
		sig0->instanceInitfx_shimmerSIG0(sample_rate);
		sig0->fillfx_shimmerSIG0(65536, ftbl0fx_shimmerSIG0);
		deletefx_shimmerSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.0999985f / fConst0;
		fConst2 = 1.0f - fConst1;
		float fConst3 = std::floor(0.153128996f * fConst0 + 0.5f);
		fConst4 = (0.0f - 6.90775537f * fConst3) / fConst0;
		fConst5 = 6.28318548f / fConst0;
		fConst6 = 3.14159274f / fConst0;
		fConst7 = 1.0f / fConst0;
		float fConst8 = std::floor(0.0203460008f * fConst0 + 0.5f);
		iConst9 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst3 - fConst8)));
		iConst10 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst8 + -1.0f)));
		float fConst11 = std::floor(0.219990999f * fConst0 + 0.5f);
		fConst12 = (0.0f - 6.90775537f * fConst11) / fConst0;
		float fConst13 = std::floor(0.0191229992f * fConst0 + 0.5f);
		iConst14 = int(std::min<float>(32768.0f, std::max<float>(0.0f, fConst11 - fConst13)));
		iConst15 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst13 + -1.0f)));
		float fConst16 = std::floor(0.256891012f * fConst0 + 0.5f);
		fConst17 = (0.0f - 6.90775537f * fConst16) / fConst0;
		float fConst18 = std::floor(0.0273330007f * fConst0 + 0.5f);
		iConst19 = int(std::min<float>(32768.0f, std::max<float>(0.0f, fConst16 - fConst18)));
		iConst20 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst18 + -1.0f)));
		float fConst21 = std::floor(0.192303002f * fConst0 + 0.5f);
		fConst22 = (0.0f - 6.90775537f * fConst21) / fConst0;
		float fConst23 = std::floor(0.0292910002f * fConst0 + 0.5f);
		iConst24 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst21 - fConst23)));
		iConst25 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst23 + -1.0f)));
		float fConst26 = std::floor(0.210389003f * fConst0 + 0.5f);
		fConst27 = (0.0f - 6.90775537f * fConst26) / fConst0;
		float fConst28 = std::floor(0.0244210009f * fConst0 + 0.5f);
		iConst29 = int(std::min<float>(32768.0f, std::max<float>(0.0f, fConst26 - fConst28)));
		iConst30 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst28 + -1.0f)));
		float fConst31 = std::floor(0.125f * fConst0 + 0.5f);
		fConst32 = (0.0f - 6.90775537f * fConst31) / fConst0;
		float fConst33 = std::floor(0.0134579996f * fConst0 + 0.5f);
		iConst34 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst31 - fConst33)));
		iConst35 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst33 + -1.0f)));
		float fConst36 = std::floor(0.127837002f * fConst0 + 0.5f);
		fConst37 = (0.0f - 6.90775537f * fConst36) / fConst0;
		float fConst38 = std::floor(0.0316039994f * fConst0 + 0.5f);
		iConst39 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst36 - fConst38)));
		iConst40 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst38 + -1.0f)));
		float fConst41 = std::floor(0.174713001f * fConst0 + 0.5f);
		fConst42 = (0.0f - 6.90775537f * fConst41) / fConst0;
		float fConst43 = std::floor(0.0229039993f * fConst0 + 0.5f);
		iConst44 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst41 - fConst43)));
		iConst45 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst43 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(2.0f);
		fHslider3 = FAUSTFLOAT(6000.0f);
		fHslider4 = FAUSTFLOAT(3.0f);
		fHslider5 = FAUSTFLOAT(200.0f);
		fHslider6 = FAUSTFLOAT(12.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.5f);
		fHslider10 = FAUSTFLOAT(1.0f);
		fHslider11 = FAUSTFLOAT(0.10000000000000001f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec12[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec11[l2] = 0.0f;
		}
		IOTA0 = 0;
		for (int l3 = 0; l3 < 131072; l3 = l3 + 1) {
			fVec0[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec14[l4] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec16[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec13[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 32768; l9 = l9 + 1) {
			fVec2[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 4096; l10 = l10 + 1) {
			fVec3[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec20[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec19[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 131072; l14 = l14 + 1) {
			fVec4[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec22[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec21[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 65536; l17 = l17 + 1) {
			fVec5[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 4096; l18 = l18 + 1) {
			fVec6[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec17[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec26[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec25[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 131072; l22 = l22 + 1) {
			fVec7[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec28[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec27[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 65536; l25 = l25 + 1) {
			fVec8[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 8192; l26 = l26 + 1) {
			fVec9[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec23[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec32[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec31[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 131072; l30 = l30 + 1) {
			fVec10[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec34[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec33[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 32768; l33 = l33 + 1) {
			fVec11[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 8192; l34 = l34 + 1) {
			fVec12[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec29[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec38[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec37[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 131072; l38 = l38 + 1) {
			fVec13[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec40[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec39[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 65536; l41 = l41 + 1) {
			fVec14[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 8192; l42 = l42 + 1) {
			fVec15[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec35[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec44[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec43[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 131072; l46 = l46 + 1) {
			fVec16[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec46[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec45[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 32768; l49 = l49 + 1) {
			fVec17[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 4096; l50 = l50 + 1) {
			fVec18[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec41[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec50[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec49[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 131072; l54 = l54 + 1) {
			fVec19[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec52[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec51[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 32768; l57 = l57 + 1) {
			fVec20[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 8192; l58 = l58 + 1) {
			fVec21[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec47[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec56[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec55[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 131072; l62 = l62 + 1) {
			fVec22[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec58[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec57[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 32768; l65 = l65 + 1) {
			fVec23[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 8192; l66 = l66 + 1) {
			fVec24[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec53[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 3; l68 = l68 + 1) {
			fRec1[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec2[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fRec3[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec4[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 3; l72 = l72 + 1) {
			fRec5[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 3; l73 = l73 + 1) {
			fRec6[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 3; l74 = l74 + 1) {
			fRec7[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec8[l75] = 0.0f;
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
	
	virtual fx_shimmer* clone() {
		return new fx_shimmer();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx_shimmer");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->addHorizontalSlider("control", &fHslider9, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0500000007f));
		ui_interface->declare(&fHslider3, "unit", "Hz");
		ui_interface->addHorizontalSlider("damp_hf", &fHslider3, FAUSTFLOAT(6000.0f), FAUSTFLOAT(1500.0f), FAUSTFLOAT(47040.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider4, "unit", "sec");
		ui_interface->addHorizontalSlider("decay_low", &fHslider4, FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->declare(&fHslider2, "unit", "sec");
		ui_interface->addHorizontalSlider("decay_mid", &fHslider2, FAUSTFLOAT(2.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addHorizontalSlider("depth", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0500000007f));
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->declare(&fHslider10, "unit", "sec");
		ui_interface->addHorizontalSlider("envelope", &fHslider10, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0500000007f));
		ui_interface->declare(&fHslider5, "unit", "Hz");
		ui_interface->addHorizontalSlider("freq_low", &fHslider5, FAUSTFLOAT(200.0f), FAUSTFLOAT(50.0f), FAUSTFLOAT(1000.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider7, "type", "int");
		ui_interface->addHorizontalSlider("mode", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(-3.0f), FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("ps_drywet", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0500000007f));
		ui_interface->declare(&fHslider6, "unit", "semitone");
		ui_interface->addHorizontalSlider("shift", &fHslider6, FAUSTFLOAT(12.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->declare(&fHslider11, "unit", "Hz");
		ui_interface->addHorizontalSlider("speed", &fHslider11, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(10.0f), FAUSTFLOAT(0.0500000007f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = fConst1 * float(fHslider0);
		float fSlow2 = float(fHslider1);
		float fSlow3 = 1.0f - fSlow2;
		float fSlow4 = float(fHslider2);
		float fSlow5 = std::exp(fConst4 / fSlow4);
		float fSlow6 = fx_shimmer_faustpower2_f(fSlow5);
		float fSlow7 = std::cos(fConst5 * float(fHslider3));
		float fSlow8 = 1.0f - fSlow6 * fSlow7;
		float fSlow9 = 1.0f - fSlow6;
		float fSlow10 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow8) / fx_shimmer_faustpower2_f(fSlow9) + -1.0f));
		float fSlow11 = fSlow8 / fSlow9;
		float fSlow12 = fSlow5 * (fSlow10 + 1.0f - fSlow11);
		float fSlow13 = float(fHslider4);
		float fSlow14 = std::exp(fConst4 / fSlow13) / fSlow5 + -1.0f;
		float fSlow15 = 1.0f / std::tan(fConst6 * float(fHslider5));
		float fSlow16 = 1.0f / (fSlow15 + 1.0f);
		float fSlow17 = 1.0f - fSlow15;
		float fSlow18 = fSlow11 - fSlow10;
		float fSlow19 = float(fHslider6);
		float fSlow20 = float(fHslider7);
		float fSlow21 = float(fSlow20 >= 0.0f);
		float fSlow22 = fSlow20 + 2.0f;
		float fSlow23 = fSlow21 * fSlow22;
		float fSlow24 = float(fSlow20 < 0.0f);
		float fSlow25 = 0.0833333358f * fSlow19 * (fSlow23 + fSlow24 * (0.400000006f * fSlow20 + 2.0f));
		float fSlow26 = float(fHslider8);
		float fSlow27 = float(fHslider9);
		float fSlow28 = 6.0f * (1.0f - fSlow27);
		float fSlow29 = float(fHslider10);
		int iSlow30 = std::fabs(fSlow29) < 1.1920929e-07f;
		float fThen2 = std::exp(0.0f - fConst7 / ((iSlow30) ? 1.0f : fSlow29));
		float fSlow31 = ((iSlow30) ? 0.0f : fThen2);
		float fSlow32 = 1.0f - fSlow31;
		float fSlow33 = 0.5f * fSlow27;
		float fSlow34 = fConst7 * float(fHslider11);
		float fSlow35 = std::exp(fConst12 / fSlow4);
		float fSlow36 = fx_shimmer_faustpower2_f(fSlow35);
		float fSlow37 = 1.0f - fSlow36 * fSlow7;
		float fSlow38 = 1.0f - fSlow36;
		float fSlow39 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow37) / fx_shimmer_faustpower2_f(fSlow38) + -1.0f));
		float fSlow40 = fSlow37 / fSlow38;
		float fSlow41 = fSlow35 * (fSlow39 + 1.0f - fSlow40);
		float fSlow42 = std::exp(fConst12 / fSlow13) / fSlow35 + -1.0f;
		float fSlow43 = fSlow40 - fSlow39;
		float fSlow44 = 0.666666687f * fSlow20;
		float fSlow45 = fSlow44 + 2.0f;
		float fSlow46 = fSlow21 * fSlow45;
		float fSlow47 = 0.0833333358f * fSlow19 * (fSlow46 + fSlow24 * fSlow22);
		float fSlow48 = std::exp(fConst17 / fSlow4);
		float fSlow49 = fx_shimmer_faustpower2_f(fSlow48);
		float fSlow50 = 1.0f - fSlow49 * fSlow7;
		float fSlow51 = 1.0f - fSlow49;
		float fSlow52 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow50) / fx_shimmer_faustpower2_f(fSlow51) + -1.0f));
		float fSlow53 = fSlow50 / fSlow51;
		float fSlow54 = fSlow48 * (fSlow52 + 1.0f - fSlow53);
		float fSlow55 = std::exp(fConst17 / fSlow13) / fSlow48 + -1.0f;
		float fSlow56 = fSlow53 - fSlow52;
		float fSlow57 = fSlow24 * (2.0f - fSlow20);
		float fSlow58 = 0.0833333358f * fSlow19 * (fSlow57 + fSlow21 * (0.5f * fSlow20 + 2.0f));
		float fSlow59 = std::exp(fConst22 / fSlow4);
		float fSlow60 = fx_shimmer_faustpower2_f(fSlow59);
		float fSlow61 = 1.0f - fSlow60 * fSlow7;
		float fSlow62 = 1.0f - fSlow60;
		float fSlow63 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow61) / fx_shimmer_faustpower2_f(fSlow62) + -1.0f));
		float fSlow64 = fSlow61 / fSlow62;
		float fSlow65 = fSlow59 * (fSlow63 + 1.0f - fSlow64);
		float fSlow66 = std::exp(fConst22 / fSlow13) / fSlow59 + -1.0f;
		float fSlow67 = fSlow64 - fSlow63;
		float fSlow68 = 0.0833333358f * fSlow19 * (fSlow21 * (0.25f * fSlow20 + 2.0f) + fSlow24 * (2.0f - fSlow44));
		float fSlow69 = std::exp(fConst27 / fSlow4);
		float fSlow70 = fx_shimmer_faustpower2_f(fSlow69);
		float fSlow71 = 1.0f - fSlow70 * fSlow7;
		float fSlow72 = 1.0f - fSlow70;
		float fSlow73 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow71) / fx_shimmer_faustpower2_f(fSlow72) + -1.0f));
		float fSlow74 = fSlow71 / fSlow72;
		float fSlow75 = fSlow69 * (fSlow73 + 1.0f - fSlow74);
		float fSlow76 = std::exp(fConst27 / fSlow13) / fSlow69 + -1.0f;
		float fSlow77 = fSlow74 - fSlow73;
		float fSlow78 = 0.0833333358f * fSlow19 * (fSlow46 + fSlow57);
		float fSlow79 = std::exp(fConst32 / fSlow4);
		float fSlow80 = fx_shimmer_faustpower2_f(fSlow79);
		float fSlow81 = 1.0f - fSlow80 * fSlow7;
		float fSlow82 = 1.0f - fSlow80;
		float fSlow83 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow81) / fx_shimmer_faustpower2_f(fSlow82) + -1.0f));
		float fSlow84 = fSlow81 / fSlow82;
		float fSlow85 = fSlow79 * (fSlow83 + 1.0f - fSlow84);
		float fSlow86 = std::exp(fConst32 / fSlow13) / fSlow79 + -1.0f;
		float fSlow87 = fSlow84 - fSlow83;
		float fSlow88 = 0.0833333358f * fSlow19 * (fSlow23 + fSlow24 * fSlow45);
		float fSlow89 = std::exp(fConst37 / fSlow4);
		float fSlow90 = fx_shimmer_faustpower2_f(fSlow89);
		float fSlow91 = 1.0f - fSlow90 * fSlow7;
		float fSlow92 = 1.0f - fSlow90;
		float fSlow93 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow91) / fx_shimmer_faustpower2_f(fSlow92) + -1.0f));
		float fSlow94 = fSlow91 / fSlow92;
		float fSlow95 = fSlow89 * (fSlow93 + 1.0f - fSlow94);
		float fSlow96 = std::exp(fConst37 / fSlow13) / fSlow89 + -1.0f;
		float fSlow97 = fSlow94 - fSlow93;
		float fSlow98 = std::exp(fConst42 / fSlow4);
		float fSlow99 = fx_shimmer_faustpower2_f(fSlow98);
		float fSlow100 = 1.0f - fSlow99 * fSlow7;
		float fSlow101 = 1.0f - fSlow99;
		float fSlow102 = std::sqrt(std::max<float>(0.0f, fx_shimmer_faustpower2_f(fSlow100) / fx_shimmer_faustpower2_f(fSlow101) + -1.0f));
		float fSlow103 = fSlow100 / fSlow101;
		float fSlow104 = fSlow98 * (fSlow102 + 1.0f - fSlow103);
		float fSlow105 = std::exp(fConst42 / fSlow13) / fSlow98 + -1.0f;
		float fSlow106 = fSlow103 - fSlow102;
		float fSlow107 = 0.0833333358f * fSlow19 * (fSlow23 + fSlow24 * (0.333333343f * fSlow20 + 2.0f));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow1 + fConst2 * fRec0[1];
			float fTemp0 = float(input0[i0]);
			float fTemp1 = ((iSlow0) ? 0.0f : fTemp0);
			fRec12[0] = 0.0f - fSlow16 * (fSlow17 * fRec12[1] - (fRec1[1] + fRec1[2]));
			fRec11[0] = fSlow12 * (fRec1[1] + fSlow14 * fRec12[0]) + fSlow18 * fRec11[1];
			float fTemp2 = 0.353553385f * fRec11[0] + 9.99999968e-21f;
			fVec0[IOTA0 & 131071] = fTemp2;
			float fTemp3 = std::fabs(fTemp2);
			fRec14[0] = std::max<float>(fTemp3, fRec14[1] * fSlow31 + fTemp3 * fSlow32);
			fRec16[0] = fSlow34 + fRec16[1] - std::floor(fSlow34 + fRec16[1]);
			float fTemp4 = fSlow33 * ftbl0fx_shimmerSIG0[int(65536.0f * fRec16[0])];
			fRec13[0] = std::fmod(fRec13[1] + 2049.0f - std::pow(2.0f, fSlow25 * (fSlow26 * (fSlow28 * fRec14[0] + fTemp4) + 0.5f)), 2048.0f);
			int iTemp5 = int(fRec13[0]);
			float fTemp6 = std::floor(fRec13[0]);
			float fTemp7 = std::min<float>(0.0009765625f * fRec13[0], 1.0f);
			float fTemp8 = fRec13[0] + 2048.0f;
			int iTemp9 = int(fTemp8);
			float fTemp10 = std::floor(fTemp8);
			fVec2[IOTA0 & 32767] = fSlow3 * fTemp2 + fSlow2 * ((fVec0[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp5))))) & 131071] * (fTemp6 + 1.0f - fRec13[0]) + (fRec13[0] - fTemp6) * fVec0[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp5 + 1))))) & 131071]) * fTemp7 + (fVec0[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp9))))) & 131071] * (fTemp10 + -2047.0f - fRec13[0]) + (fRec13[0] + 2048.0f - fTemp10) * fVec0[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp9 + 1))))) & 131071]) * (1.0f - fTemp7));
			float fTemp11 = (fTemp1 + fVec2[(IOTA0 - iConst9) & 32767]) - 0.600000024f * fRec9[1];
			fVec3[IOTA0 & 4095] = fTemp11;
			fRec9[0] = fVec3[(IOTA0 - iConst10) & 4095];
			float fRec10 = 0.600000024f * fTemp11;
			float fTemp12 = float(input1[i0]);
			float fTemp13 = ((iSlow0) ? 0.0f : fTemp12);
			fRec20[0] = 0.0f - fSlow16 * (fSlow17 * fRec20[1] - (fRec8[1] + fRec8[2]));
			fRec19[0] = fSlow41 * (fRec8[1] + fSlow42 * fRec20[0]) + fSlow43 * fRec19[1];
			float fTemp14 = 0.353553385f * fRec19[0] + 9.99999968e-21f;
			fVec4[IOTA0 & 131071] = fTemp14;
			float fTemp15 = std::fabs(fTemp14);
			fRec22[0] = std::max<float>(fTemp15, fSlow31 * fRec22[1] + fSlow32 * fTemp15);
			fRec21[0] = std::fmod(fRec21[1] + 2049.0f - std::pow(2.0f, fSlow47 * (fSlow26 * (fTemp4 + fSlow28 * fRec22[0]) + 0.5f)), 2048.0f);
			int iTemp16 = int(fRec21[0]);
			float fTemp17 = std::floor(fRec21[0]);
			float fTemp18 = std::min<float>(0.0009765625f * fRec21[0], 1.0f);
			float fTemp19 = fRec21[0] + 2048.0f;
			int iTemp20 = int(fTemp19);
			float fTemp21 = std::floor(fTemp19);
			fVec5[IOTA0 & 65535] = fSlow3 * fTemp14 + fSlow2 * ((fVec4[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp16))))) & 131071] * (fTemp17 + 1.0f - fRec21[0]) + (fRec21[0] - fTemp17) * fVec4[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp16 + 1))))) & 131071]) * fTemp18 + (fVec4[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp20))))) & 131071] * (fTemp21 + -2047.0f - fRec21[0]) + (fRec21[0] + 2048.0f - fTemp21) * fVec4[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp20 + 1))))) & 131071]) * (1.0f - fTemp18));
			float fTemp22 = fTemp13 + 0.600000024f * fRec17[1] + fVec5[(IOTA0 - iConst14) & 65535];
			fVec6[IOTA0 & 4095] = fTemp22;
			fRec17[0] = fVec6[(IOTA0 - iConst15) & 4095];
			float fRec18 = 0.0f - 0.600000024f * fTemp22;
			fRec26[0] = 0.0f - fSlow16 * (fSlow17 * fRec26[1] - (fRec4[1] + fRec4[2]));
			fRec25[0] = fSlow54 * (fRec4[1] + fSlow55 * fRec26[0]) + fSlow56 * fRec25[1];
			float fTemp23 = 0.353553385f * fRec25[0] + 9.99999968e-21f;
			fVec7[IOTA0 & 131071] = fTemp23;
			float fTemp24 = std::fabs(fTemp23);
			fRec28[0] = std::max<float>(fTemp24, fSlow31 * fRec28[1] + fSlow32 * fTemp24);
			fRec27[0] = std::fmod(fRec27[1] + 2049.0f - std::pow(2.0f, fSlow58 * (fSlow26 * (fTemp4 + fSlow28 * fRec28[0]) + 0.5f)), 2048.0f);
			int iTemp25 = int(fRec27[0]);
			float fTemp26 = std::floor(fRec27[0]);
			float fTemp27 = std::min<float>(0.0009765625f * fRec27[0], 1.0f);
			float fTemp28 = fRec27[0] + 2048.0f;
			int iTemp29 = int(fTemp28);
			float fTemp30 = std::floor(fTemp28);
			fVec8[IOTA0 & 65535] = fSlow3 * fTemp23 + fSlow2 * ((fVec7[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp25))))) & 131071] * (fTemp26 + 1.0f - fRec27[0]) + (fRec27[0] - fTemp26) * fVec7[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp25 + 1))))) & 131071]) * fTemp27 + (fVec7[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp29))))) & 131071] * (fTemp30 + -2047.0f - fRec27[0]) + (fRec27[0] + 2048.0f - fTemp30) * fVec7[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp29 + 1))))) & 131071]) * (1.0f - fTemp27));
			float fTemp31 = fTemp13 + 0.600000024f * fRec23[1] + fVec8[(IOTA0 - iConst19) & 65535];
			fVec9[IOTA0 & 8191] = fTemp31;
			fRec23[0] = fVec9[(IOTA0 - iConst20) & 8191];
			float fRec24 = 0.0f - 0.600000024f * fTemp31;
			fRec32[0] = fSlow16 * ((fRec6[1] + fRec6[2]) - fSlow17 * fRec32[1]);
			fRec31[0] = fSlow65 * (fRec6[1] + fSlow66 * fRec32[0]) + fSlow67 * fRec31[1];
			float fTemp32 = 0.353553385f * fRec31[0] + 9.99999968e-21f;
			fVec10[IOTA0 & 131071] = fTemp32;
			float fTemp33 = std::fabs(fTemp32);
			fRec34[0] = std::max<float>(fTemp33, fSlow31 * fRec34[1] + fSlow32 * fTemp33);
			fRec33[0] = std::fmod(fRec33[1] + 2049.0f - std::pow(2.0f, fSlow68 * (fSlow26 * (fTemp4 + fSlow28 * fRec34[0]) + 0.5f)), 2048.0f);
			int iTemp34 = int(fRec33[0]);
			float fTemp35 = std::floor(fRec33[0]);
			float fTemp36 = std::min<float>(0.0009765625f * fRec33[0], 1.0f);
			float fTemp37 = fRec33[0] + 2048.0f;
			int iTemp38 = int(fTemp37);
			float fTemp39 = std::floor(fTemp37);
			fVec11[IOTA0 & 32767] = fSlow3 * fTemp32 + fSlow2 * ((fVec10[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp34))))) & 131071] * (fTemp35 + 1.0f - fRec33[0]) + (fRec33[0] - fTemp35) * fVec10[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp34 + 1))))) & 131071]) * fTemp36 + (fVec10[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp38))))) & 131071] * (fTemp39 + -2047.0f - fRec33[0]) + (fRec33[0] + 2048.0f - fTemp39) * fVec10[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp38 + 1))))) & 131071]) * (1.0f - fTemp36));
			float fTemp40 = fTemp13 + 0.600000024f * fRec29[1] + fVec11[(IOTA0 - iConst24) & 32767];
			fVec12[IOTA0 & 8191] = fTemp40;
			fRec29[0] = fVec12[(IOTA0 - iConst25) & 8191];
			float fRec30 = 0.0f - 0.600000024f * fTemp40;
			fRec38[0] = 0.0f - fSlow16 * (fSlow17 * fRec38[1] - (fRec2[1] + fRec2[2]));
			fRec37[0] = fSlow75 * (fRec2[1] + fSlow76 * fRec38[0]) + fSlow77 * fRec37[1];
			float fTemp41 = 0.353553385f * fRec37[0] + 9.99999968e-21f;
			fVec13[IOTA0 & 131071] = fTemp41;
			float fTemp42 = std::fabs(fTemp41);
			fRec40[0] = std::max<float>(fTemp42, fSlow31 * fRec40[1] + fSlow32 * fTemp42);
			fRec39[0] = std::fmod(fRec39[1] + 2049.0f - std::pow(2.0f, fSlow78 * (fSlow26 * (fTemp4 + fSlow28 * fRec40[0]) + 0.5f)), 2048.0f);
			int iTemp43 = int(fRec39[0]);
			float fTemp44 = std::floor(fRec39[0]);
			float fTemp45 = std::min<float>(0.0009765625f * fRec39[0], 1.0f);
			float fTemp46 = fRec39[0] + 2048.0f;
			int iTemp47 = int(fTemp46);
			float fTemp48 = std::floor(fTemp46);
			fVec14[IOTA0 & 65535] = fSlow3 * fTemp41 + fSlow2 * ((fVec13[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp43))))) & 131071] * (fTemp44 + 1.0f - fRec39[0]) + (fRec39[0] - fTemp44) * fVec13[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp43 + 1))))) & 131071]) * fTemp45 + (fVec13[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp47))))) & 131071] * (fTemp48 + -2047.0f - fRec39[0]) + (fRec39[0] + 2048.0f - fTemp48) * fVec13[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp47 + 1))))) & 131071]) * (1.0f - fTemp45));
			float fTemp49 = fTemp13 + 0.600000024f * fRec35[1] + fVec14[(IOTA0 - iConst29) & 65535];
			fVec15[IOTA0 & 8191] = fTemp49;
			fRec35[0] = fVec15[(IOTA0 - iConst30) & 8191];
			float fRec36 = 0.0f - 0.600000024f * fTemp49;
			fRec44[0] = 0.0f - fSlow16 * (fSlow17 * fRec44[1] - (fRec7[1] + fRec7[2]));
			fRec43[0] = fSlow85 * (fRec7[1] + fSlow86 * fRec44[0]) + fSlow87 * fRec43[1];
			float fTemp50 = 0.353553385f * fRec43[0] + 9.99999968e-21f;
			fVec16[IOTA0 & 131071] = fTemp50;
			float fTemp51 = std::fabs(fTemp50);
			fRec46[0] = std::max<float>(fTemp51, fSlow31 * fRec46[1] + fSlow32 * fTemp51);
			fRec45[0] = std::fmod(fRec45[1] + 2049.0f - std::pow(2.0f, fSlow88 * (fSlow26 * (fTemp4 + fSlow28 * fRec46[0]) + 0.5f)), 2048.0f);
			int iTemp52 = int(fRec45[0]);
			float fTemp53 = std::floor(fRec45[0]);
			float fTemp54 = std::min<float>(0.0009765625f * fRec45[0], 1.0f);
			float fTemp55 = fRec45[0] + 2048.0f;
			int iTemp56 = int(fTemp55);
			float fTemp57 = std::floor(fTemp55);
			fVec17[IOTA0 & 32767] = fSlow3 * fTemp50 + fSlow2 * ((fVec16[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp52))))) & 131071] * (fTemp53 + 1.0f - fRec45[0]) + (fRec45[0] - fTemp53) * fVec16[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp52 + 1))))) & 131071]) * fTemp54 + (fVec16[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp56))))) & 131071] * (fTemp57 + -2047.0f - fRec45[0]) + (fRec45[0] + 2048.0f - fTemp57) * fVec16[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp56 + 1))))) & 131071]) * (1.0f - fTemp54));
			float fTemp58 = (fTemp1 + fVec17[(IOTA0 - iConst34) & 32767]) - 0.600000024f * fRec41[1];
			fVec18[IOTA0 & 4095] = fTemp58;
			fRec41[0] = fVec18[(IOTA0 - iConst35) & 4095];
			float fRec42 = 0.600000024f * fTemp58;
			fRec50[0] = 0.0f - fSlow16 * (fSlow17 * fRec50[1] - (fRec3[1] + fRec3[2]));
			fRec49[0] = fSlow95 * (fRec3[1] + fSlow96 * fRec50[0]) + fSlow97 * fRec49[1];
			float fTemp59 = 0.353553385f * fRec49[0] + 9.99999968e-21f;
			fVec19[IOTA0 & 131071] = fTemp59;
			float fTemp60 = std::fabs(fTemp59);
			fRec52[0] = std::max<float>(fTemp60, fSlow31 * fRec52[1] + fSlow32 * fTemp60);
			fRec51[0] = std::fmod(fRec51[1] + 2049.0f - std::pow(2.0f, fSlow88 * (fSlow26 * (fTemp4 + fSlow28 * fRec52[0]) + 0.5f)), 2048.0f);
			int iTemp61 = int(fRec51[0]);
			float fTemp62 = std::floor(fRec51[0]);
			float fTemp63 = std::min<float>(0.0009765625f * fRec51[0], 1.0f);
			float fTemp64 = fRec51[0] + 2048.0f;
			int iTemp65 = int(fTemp64);
			float fTemp66 = std::floor(fTemp64);
			fVec20[IOTA0 & 32767] = fSlow3 * fTemp59 + fSlow2 * ((fVec19[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp61))))) & 131071] * (fTemp62 + 1.0f - fRec51[0]) + (fRec51[0] - fTemp62) * fVec19[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp61 + 1))))) & 131071]) * fTemp63 + (fVec19[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp65))))) & 131071] * (fTemp66 + -2047.0f - fRec51[0]) + (fRec51[0] + 2048.0f - fTemp66) * fVec19[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp65 + 1))))) & 131071]) * (1.0f - fTemp63));
			float fTemp67 = (fTemp1 + fVec20[(IOTA0 - iConst39) & 32767]) - 0.600000024f * fRec47[1];
			fVec21[IOTA0 & 8191] = fTemp67;
			fRec47[0] = fVec21[(IOTA0 - iConst40) & 8191];
			float fRec48 = 0.600000024f * fTemp67;
			fRec56[0] = 0.0f - fSlow16 * (fSlow17 * fRec56[1] - (fRec5[1] + fRec5[2]));
			fRec55[0] = fSlow104 * (fRec5[1] + fSlow105 * fRec56[0]) + fSlow106 * fRec55[1];
			float fTemp68 = 0.353553385f * fRec55[0] + 9.99999968e-21f;
			fVec22[IOTA0 & 131071] = fTemp68;
			float fTemp69 = std::fabs(fTemp68);
			fRec58[0] = std::max<float>(fTemp69, fSlow31 * fRec58[1] + fSlow32 * fTemp69);
			fRec57[0] = std::fmod(fRec57[1] + 2049.0f - std::pow(2.0f, fSlow107 * (fSlow26 * (fTemp4 + fSlow28 * fRec58[0]) + 0.5f)), 2048.0f);
			int iTemp70 = int(fRec57[0]);
			float fTemp71 = std::floor(fRec57[0]);
			float fTemp72 = std::min<float>(0.0009765625f * fRec57[0], 1.0f);
			float fTemp73 = fRec57[0] + 2048.0f;
			int iTemp74 = int(fTemp73);
			float fTemp75 = std::floor(fTemp73);
			fVec23[IOTA0 & 32767] = fSlow3 * fTemp68 + fSlow2 * ((fVec22[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp70))))) & 131071] * (fTemp71 + 1.0f - fRec57[0]) + (fRec57[0] - fTemp71) * fVec22[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp70 + 1))))) & 131071]) * fTemp72 + (fVec22[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp74))))) & 131071] * (fTemp75 + -2047.0f - fRec57[0]) + (fRec57[0] + 2048.0f - fTemp75) * fVec22[(IOTA0 - std::min<int>(65537, int(std::max<int>(0, int(iTemp74 + 1))))) & 131071]) * (1.0f - fTemp72));
			float fTemp76 = (fTemp1 + fVec23[(IOTA0 - iConst44) & 32767]) - 0.600000024f * fRec53[1];
			fVec24[IOTA0 & 8191] = fTemp76;
			fRec53[0] = fVec24[(IOTA0 - iConst45) & 8191];
			float fRec54 = 0.600000024f * fTemp76;
			float fTemp77 = fRec24 + fRec18;
			float fTemp78 = fRec54 + fRec10 + fRec53[1] + fTemp77;
			fRec1[0] = fRec9[1] + fRec17[1] + fRec23[1] + fRec29[1] + fRec35[1] + fRec41[1] + fRec47[1] + fRec30 + fRec36 + fRec42 + fRec48 + fTemp78;
			float fTemp79 = fRec54 + fRec10 + fRec53[1];
			fRec2[0] = (fRec9[1] + fRec41[1] + fRec47[1] + fRec42 + fRec48 + fTemp79) - (fRec17[1] + fRec23[1] + fRec29[1] + fRec35[1] + fRec30 + fRec36 + fTemp77);
			fRec3[0] = (fRec9[1] + fRec29[1] + fRec35[1] + fRec30 + fRec36 + fTemp79) - (fRec17[1] + fRec23[1] + fRec41[1] + fRec47[1] + fRec42 + fRec48 + fTemp77);
			fRec4[0] = (fRec9[1] + fRec17[1] + fRec23[1] + fTemp78) - (fRec29[1] + fRec35[1] + fRec41[1] + fRec47[1] + fRec30 + fRec36 + fRec48 + fRec42);
			float fTemp80 = fRec10 + fRec24;
			float fTemp81 = fRec54 + fRec18 + fRec53[1];
			fRec5[0] = (fRec9[1] + fRec23[1] + fRec35[1] + fRec47[1] + fRec36 + fRec48 + fTemp80) - (fRec17[1] + fRec29[1] + fRec41[1] + fRec30 + fRec42 + fTemp81);
			float fTemp82 = fRec10 + fRec18;
			float fTemp83 = fRec54 + fRec24 + fRec53[1];
			fRec6[0] = (fRec9[1] + fRec17[1] + fRec29[1] + fRec47[1] + fRec30 + fRec48 + fTemp82) - (fRec23[1] + fRec35[1] + fRec41[1] + fRec36 + fRec42 + fTemp83);
			fRec7[0] = (fRec9[1] + fRec17[1] + fRec35[1] + fRec41[1] + fRec36 + fRec42 + fTemp82) - (fRec23[1] + fRec29[1] + fRec47[1] + fRec30 + fRec48 + fTemp83);
			fRec8[0] = (fRec9[1] + fRec23[1] + fRec29[1] + fRec41[1] + fRec30 + fRec42 + fTemp80) - (fRec17[1] + fRec35[1] + fRec47[1] + fRec36 + fRec48 + fTemp81);
			float fTemp84 = 1.0f - fRec0[0];
			float fThen4 = fRec0[0] * (fRec1[0] + fRec3[0] + fRec5[0] + fRec7[0]) + fTemp84 * fTemp1;
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp0 : fThen4));
			float fThen5 = fTemp84 * fTemp13 + fRec0[0] * (fRec2[0] + fRec4[0] + fRec6[0] + fRec8[0]);
			output1[i0] = FAUSTFLOAT(((iSlow0) ? fTemp12 : fThen5));
			fRec0[1] = fRec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			IOTA0 = IOTA0 + 1;
			fRec14[1] = fRec14[0];
			fRec16[1] = fRec16[0];
			fRec13[1] = fRec13[0];
			fRec9[1] = fRec9[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec17[1] = fRec17[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec23[1] = fRec23[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec29[1] = fRec29[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec35[1] = fRec35[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec41[1] = fRec41[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec47[1] = fRec47[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec58[1] = fRec58[0];
			fRec57[1] = fRec57[0];
			fRec53[1] = fRec53[0];
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
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_shimmer_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_shimmer_UI<T>::name(sym(fx_shimmer));

typedef _fx_shimmer_UI<fx_shimmer> fx_shimmer_UI;

class faust_fx_shimmer_tilde : public FaustExternal<fx_shimmer, fx_shimmer_UI> {
public:
    faust_fx_shimmer_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
