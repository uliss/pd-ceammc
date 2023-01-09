/* ------------------------------------------------------------
name: "synth.voice_fofc"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn synth_voice_fofc -scn synth_voice_fofc_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_voice_fofc_H__
#define  __synth_voice_fofc_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_voice_fofc_dsp.h ********************************
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

class FAUST_API synth_voice_fofc_dsp {

    public:

        synth_voice_fofc_dsp() {}
        virtual ~synth_voice_fofc_dsp() {}

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
        virtual synth_voice_fofc_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_voice_fofc_dsp {

    protected:

        synth_voice_fofc_dsp* fDSP;

    public:

        decorator_dsp(synth_voice_fofc_dsp* synth_voice_fofc_dsp = nullptr):fDSP(synth_voice_fofc_dsp) {}
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
    
        virtual synth_voice_fofc_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_voice_fofc_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_voice_fofc_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_voice_fofc : public synth_voice_fofc_dsp {
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
#define FAUSTCLASS synth_voice_fofc
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

class synth_voice_fofcSIG0 {
	
  private:
	
	int iVec0[2];
	int iRec4[2];
	
  public:
	
	int getNumInputssynth_voice_fofcSIG0() {
		return 0;
	}
	int getNumOutputssynth_voice_fofcSIG0() {
		return 1;
	}
	
	void instanceInitsynth_voice_fofcSIG0(int sample_rate) {
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iVec0[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec4[l5] = 0;
		}
	}
	
	void fillsynth_voice_fofcSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec4[0] = (iVec0[1] + iRec4[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec4[0]));
			iVec0[1] = iVec0[0];
			iRec4[1] = iRec4[0];
		}
	}

};

static synth_voice_fofcSIG0* newsynth_voice_fofcSIG0() { return (synth_voice_fofcSIG0*)new synth_voice_fofcSIG0(); }
static void deletesynth_voice_fofcSIG0(synth_voice_fofcSIG0* dsp) { delete dsp; }

static float ftbl0synth_voice_fofcSIG0[65536];

class synth_voice_fofc : public synth_voice_fofc_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	int fSampleRate;
	float fConst1;
	float fRec0[2];
	FAUSTFLOAT fHslider2;
	float fConst2;
	FAUSTFLOAT fVslider0;
	float fConst3;
	float fRec2[2];
	FAUSTFLOAT fHslider3;
	float fRec3[2];
	float fRec1[2];
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fRec5[2];
	FAUSTFLOAT fHslider6;
	float fRec6[2];
	float fRec9[2];
	float fRec8[2];
	float fVec1[3];
	float fRec10[2];
	float fRec11[2];
	float fRec7[3];
	float fRec12[2];
	float fVec2[3];
	float fRec14[2];
	float fRec15[2];
	float fRec13[3];
	float fRec16[2];
	float fVec3[3];
	float fRec18[2];
	float fRec19[2];
	float fRec17[3];
	float fRec20[2];
	float fRec22[2];
	float fRec23[2];
	float fRec21[3];
	float fRec24[2];
	float fRec26[2];
	float fRec27[2];
	float fRec25[3];
	float fRec28[2];
	float fRec30[2];
	float fRec31[2];
	float fRec29[3];
	float fRec32[2];
	float fRec34[2];
	float fRec35[2];
	float fRec33[3];
	float fRec36[2];
	float fRec38[2];
	float fRec39[2];
	float fRec37[3];
	float fRec40[2];
	float fRec42[2];
	float fRec43[2];
	float fRec41[3];
	float fRec44[2];
	float fRec46[2];
	float fRec47[2];
	float fRec45[3];
	float fRec48[2];
	float fRec50[2];
	float fRec51[2];
	float fRec49[3];
	float fRec52[2];
	float fRec54[2];
	float fRec55[2];
	float fRec53[3];
	float fRec56[2];
	float fRec58[2];
	float fRec59[2];
	float fRec57[3];
	float fRec60[2];
	float fRec62[2];
	float fRec63[2];
	float fRec61[3];
	float fRec64[2];
	float fRec66[2];
	float fRec67[2];
	float fRec65[3];
	float fRec68[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/cycle:author", "Mike Olsen");
		m->declare("basics.lib/listInterp:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/pulse_countup_loop:author", "Vince");
		m->declare("basics.lib/resetCtr:author", "Mike Olsen");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn synth_voice_fofc -scn synth_voice_fofc_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("envelopes.lib/are:author", "Julius O. Smith III, revised by Stephane Letz");
		m->declare("envelopes.lib/are:licence", "STK-4.3");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.2");
		m->declare("filename", "synth_voice_fofc.dsp");
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
		m->declare("name", "synth.voice_fofc");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.4");
		m->declare("oscillators.lib/hs_oscsin:author", "Mike Olsen");
		m->declare("oscillators.lib/hs_phasor:author", "Mike Olsen, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.3");
		m->declare("physmodels.lib/SFFormantModel:author", "Mike Olsen");
		m->declare("physmodels.lib/SFFormantModelFofCycle:author", "Mike Olsen");
		m->declare("physmodels.lib/autobendFreq:author", "Mike Olsen");
		m->declare("physmodels.lib/fof:author", "Mike Olsen");
		m->declare("physmodels.lib/fofCycle:author", "Mike Olsen");
		m->declare("physmodels.lib/fofSH:author", "Mike Olsen");
		m->declare("physmodels.lib/formantFilterFofCycle:author", "Mike Olsen");
		m->declare("physmodels.lib/formantFilterbank:author", "Mike Olsen");
		m->declare("physmodels.lib/formantFilterbankFofCycle:author", "Mike Olsen");
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
		synth_voice_fofcSIG0* sig0 = newsynth_voice_fofcSIG0();
		sig0->instanceInitsynth_voice_fofcSIG0(sample_rate);
		sig0->fillsynth_voice_fofcSIG0(65536, ftbl0synth_voice_fofcSIG0);
		deletesynth_voice_fofcSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1e+01f);
		fHslider1 = FAUSTFLOAT(1e+01f);
		fHslider2 = FAUSTFLOAT(0.0f);
		fVslider0 = FAUSTFLOAT(4.4e+02f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.5f);
		fHslider5 = FAUSTFLOAT(6.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec2[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec3[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec1[l3] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec9[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fVec1[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec10[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec11[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = l13 + 1) {
			fRec7[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fVec2[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec13[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fVec3[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec18[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec19[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec17[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec20[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec22[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec23[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec21[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec24[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec26[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec27[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 3; l31 = l31 + 1) {
			fRec25[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec28[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec30[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec31[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fRec29[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec32[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec34[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec35[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 3; l39 = l39 + 1) {
			fRec33[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec36[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec38[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec39[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fRec37[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec40[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec42[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec43[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec41[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec44[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec46[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec47[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec45[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec48[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec50[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec51[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 3; l55 = l55 + 1) {
			fRec49[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec52[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec54[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec55[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec53[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec56[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec58[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec59[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fRec57[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec60[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec62[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec63[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 3; l67 = l67 + 1) {
			fRec61[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec64[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fRec66[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec67[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec65[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fRec68[l72] = 0.0f;
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
	
	virtual synth_voice_fofc* clone() {
		return new synth_voice_fofc();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.voice_fofc");
		ui_interface->addHorizontalSlider(".voice", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider(".vowel", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider1, "unit", "msec");
		ui_interface->addHorizontalSlider("attack", &fHslider1, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider0, "unit", "Hz");
		ui_interface->addVerticalSlider("freq", &fVslider0, FAUSTFLOAT(4.4e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(0.1f));
		ui_interface->addCheckButton("gate", &fCheckbox0);
		ui_interface->addHorizontalSlider("portamento", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4e+03f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider0, "unit", "msec");
		ui_interface->addHorizontalSlider("release", &fHslider0, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("vibr_freq", &fHslider5, FAUSTFLOAT(6.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("vibr_gain", &fHslider4, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = float(fCheckbox0) > 0.0f;
		float fSlow1 = 0.1447178f * ((iSlow0) ? 0.001f * float(fHslider1) : 0.001f * float(fHslider0));
		int iSlow2 = std::fabs(fSlow1) < 1.1920929e-07f;
		float fSlow3 = ((iSlow2) ? 0.0f : std::exp(0.0f - fConst1 / ((iSlow2) ? 1.0f : fSlow1)));
		float fSlow4 = (1.0f - fSlow3) * ((iSlow0) ? float(iSlow0) : 0.0f);
		int iSlow5 = int(float(fHslider2));
		int iSlow6 = ((iSlow5 == 0) ? 1 : ((iSlow5 == 3) ? 1 : 0));
		int iSlow7 = iSlow6 == 0;
		float fSlow8 = fConst2 * float(fVslider0);
		float fSlow9 = fConst2 * float(fHslider3);
		float fSlow10 = 0.1f * float(fHslider4);
		float fSlow11 = fConst1 * float(fHslider5);
		float fSlow12 = float(5 * iSlow5);
		float fSlow13 = fConst2 * float(fHslider6);
		float fSlow14 = float(5 * iSlow6);
		int iSlow15 = iSlow6 >= 1;
		float fSlow16 = ((iSlow15) ? 174.61f : 82.41f);
		float fSlow17 = ((iSlow15) ? 1046.5f : 523.25f);
		float fSlow18 = fSlow17 - fSlow16;
		int iSlow19 = iSlow5 != 2;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow4 + fSlow3 * fRec0[1];
			fRec2[0] = fSlow8 + fConst3 * fRec2[1];
			fRec3[0] = fSlow9 + fConst3 * fRec3[1];
			float fTemp0 = 0.001f * fRec3[0];
			int iTemp1 = std::fabs(fTemp0) < 1.1920929e-07f;
			float fTemp2 = ((iTemp1) ? 0.0f : std::exp(0.0f - fConst1 / ((iTemp1) ? 1.0f : fTemp0)));
			fRec1[0] = fRec2[0] * (1.0f - fTemp2) + fTemp2 * fRec1[1];
			fRec5[0] = fSlow11 + (fRec5[1] - std::floor(fSlow11 + fRec5[1]));
			float fTemp3 = fRec1[0] * (fSlow10 * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec5[0])] + 1.0f);
			float fTemp4 = 0.00084f * (1e+03f - fTemp3) + 0.8f;
			fRec6[0] = fSlow13 + fConst3 * fRec6[1];
			float fTemp5 = fSlow12 + fRec6[0];
			int iTemp6 = fTemp5 < 23.0f;
			int iTemp7 = fTemp5 < 24.0f;
			float fTemp8 = fSlow12 + fRec6[0] + -23.0f;
			int iTemp9 = fTemp5 < 22.0f;
			float fTemp10 = fSlow12 + fRec6[0] + -22.0f;
			int iTemp11 = fTemp5 < 21.0f;
			float fTemp12 = fSlow12 + fRec6[0] + -21.0f;
			int iTemp13 = fTemp5 < 2e+01f;
			float fTemp14 = fSlow12 + fRec6[0] + -2e+01f;
			int iTemp15 = fTemp5 < 19.0f;
			float fTemp16 = fSlow12 + fRec6[0] + -19.0f;
			int iTemp17 = fTemp5 < 18.0f;
			float fTemp18 = fSlow12 + fRec6[0] + -18.0f;
			int iTemp19 = fTemp5 < 17.0f;
			float fTemp20 = fSlow12 + fRec6[0] + -17.0f;
			float fTemp21 = ((iTemp17) ? 0.029314f * fTemp20 + 0.050119f : 0.079433f);
			int iTemp22 = fTemp5 < 16.0f;
			float fTemp23 = fSlow12 + fRec6[0] + -16.0f;
			int iTemp24 = fTemp5 < 15.0f;
			float fTemp25 = fSlow12 + fRec6[0] + -15.0f;
			int iTemp26 = fTemp5 < 14.0f;
			float fTemp27 = fSlow12 + fRec6[0] + -14.0f;
			int iTemp28 = fTemp5 < 13.0f;
			float fTemp29 = fSlow12 + fRec6[0] + -13.0f;
			int iTemp30 = fTemp5 < 12.0f;
			float fTemp31 = fSlow12 + fRec6[0] + -12.0f;
			int iTemp32 = fTemp5 < 11.0f;
			float fTemp33 = fSlow12 + fRec6[0] + -11.0f;
			float fTemp34 = ((iTemp30) ? 0.1f - 0.084151f * fTemp33 : 0.015849f);
			int iTemp35 = fTemp5 < 1e+01f;
			float fTemp36 = fSlow12 + fRec6[0] + -1e+01f;
			int iTemp37 = fTemp5 < 9.0f;
			float fTemp38 = fSlow12 + fRec6[0] + -9.0f;
			int iTemp39 = fTemp5 < 8.0f;
			float fTemp40 = fSlow12 + fRec6[0] + -8.0f;
			int iTemp41 = fTemp5 < 7.0f;
			float fTemp42 = fSlow12 + fRec6[0] + -7.0f;
			int iTemp43 = fTemp5 < 6.0f;
			float fTemp44 = fSlow12 + fRec6[0] + -6.0f;
			int iTemp45 = fTemp5 < 5.0f;
			float fTemp46 = fSlow12 + fRec6[0] + -5.0f;
			int iTemp47 = fTemp5 < 4.0f;
			float fTemp48 = fSlow12 + fRec6[0] + -4.0f;
			int iTemp49 = fTemp5 < 3.0f;
			float fTemp50 = fSlow12 + fRec6[0] + -3.0f;
			int iTemp51 = fTemp5 < 2.0f;
			float fTemp52 = fSlow12 + fRec6[0] + -2.0f;
			int iTemp53 = fTemp5 < 1.0f;
			float fTemp54 = fSlow12 + fRec6[0] + -1.0f;
			int iTemp55 = fTemp5 < 0.0f;
			float fTemp56 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 0.015849f : ((iTemp53) ? 0.001934f * fTemp5 + 0.015849f : 0.017783f)) : ((iTemp51) ? 0.017783f - 0.001934f * fTemp54 : 0.015849f)) : ((iTemp49) ? 0.023962f * fTemp52 + 0.015849f : 0.039811f)) : ((iTemp47) ? 0.039811f - 0.029811f * fTemp50 : 0.01f)) : ((iTemp45) ? 0.344813f * fTemp48 + 0.01f : 0.354813f)) : ((iTemp43) ? 0.354813f - 0.103624f * fTemp46 : 0.251189f)) : ((iTemp41) ? 0.251189f - 0.171756f * fTemp44 : 0.079433f)) : ((iTemp39) ? 0.020567f * fTemp42 + 0.079433f : 0.1f)) : ((iTemp37) ? 0.1f - 0.060189f * fTemp40 : 0.039811f)) : ((iTemp35) ? 0.023285f * fTemp38 + 0.039811f : 0.063096f)) : ((iTemp32) ? 0.036904f * fTemp36 + 0.063096f : 0.1f)) : fTemp34) : ((iTemp28) ? 0.063584f * fTemp31 + 0.015849f : 0.079433f)) : ((iTemp26) ? 0.079433f - 0.04781f * fTemp29 : 0.031623f)) : ((iTemp24) ? 0.068377f * fTemp27 + 0.031623f : 0.1f)) : ((iTemp22) ? 0.1f - 0.09f * fTemp25 : 0.01f)) : ((iTemp19) ? 0.040119f * fTemp23 + 0.01f : 0.050119f)) : fTemp21) : ((iTemp15) ? 0.079433f - 0.069433f * fTemp18 : 0.01f)) : ((iTemp13) ? 0.388107f * fTemp16 + 0.01f : 0.398107f)) : ((iTemp11) ? 0.398107f - 0.198581f * fTemp14 : 0.199526f)) : ((iTemp9) ? 0.199526f - 0.099526f * fTemp12 : 0.1f)) : ((iTemp6) ? 0.151189f * fTemp10 + 0.1f : 0.251189f)) : ((iTemp7) ? 0.251189f - 0.051663f * fTemp8 : 0.199526f));
			float fTemp57 = 0.0036666666f * (4e+02f - fTemp3) + 3.0f;
			float fTemp58 = fRec9[1] + fConst1 * fTemp3;
			fRec9[0] = fTemp58 - std::floor(fTemp58);
			float fTemp59 = float((fRec9[0] - fRec9[1]) < 0.0f);
			fRec8[0] = fTemp59 + fRec8[1] * float(fRec8[1] <= 2.0f);
			float fTemp60 = float(fRec8[0] == 1.0f) * fTemp59;
			fVec1[0] = fTemp60;
			int iTemp61 = int(fTemp60);
			float fTemp62 = fRec6[0] + fSlow14;
			int iTemp63 = fTemp62 >= 5.0f;
			int iTemp64 = fTemp62 >= 3.0f;
			int iTemp65 = fTemp62 >= 2.0f;
			int iTemp66 = fTemp62 >= 1.0f;
			int iTemp67 = fTemp62 >= 4.0f;
			int iTemp68 = fTemp62 >= 8.0f;
			int iTemp69 = fTemp62 >= 7.0f;
			int iTemp70 = fTemp62 >= 6.0f;
			float fTemp71 = ((iTemp63) ? ((iTemp68) ? 2.0f : ((iTemp69) ? 3.0f : ((iTemp70) ? 3.0f : 2.0f))) : ((iTemp64) ? ((iTemp67) ? 1.5f : 1.0f) : ((iTemp65) ? 1.25f : ((iTemp66) ? 1.25f : 1.0f))));
			float fTemp72 = fTemp71 + (((iTemp63) ? ((iTemp68) ? 12.0f : ((iTemp69) ? 12.0f : ((iTemp70) ? 12.0f : 15.0f))) : ((iTemp64) ? ((iTemp67) ? 4.0f : 1e+01f) : ((iTemp65) ? 2.5f : ((iTemp66) ? 2.5f : 1e+01f)))) - fTemp71) * ((fTemp3 <= fSlow16) ? 0.0f : ((fTemp3 >= fSlow17) ? 1.0f : (fTemp3 - fSlow16) / fSlow18));
			float fTemp73 = 1e+01f * fTemp10;
			float fTemp74 = 1e+01f * fTemp14;
			float fTemp75 = 5e+01f * fTemp16;
			float fTemp76 = 2e+01f * fTemp25;
			float fTemp77 = 1e+01f * fTemp36;
			float fTemp78 = 1e+01f * fTemp38;
			float fTemp79 = 6e+01f * fTemp48;
			float fTemp80 = 2e+01f * fTemp52;
			float fTemp81 = 2e+01f * fTemp5;
			float fTemp82 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 1.3e+02f : ((iTemp53) ? fTemp81 + 1.3e+02f : 1.5e+02f)) : 1.5e+02f) : ((iTemp49) ? 1.5e+02f - fTemp80 : 1.3e+02f)) : ((iTemp47) ? 5e+01f * fTemp50 + 1.3e+02f : 1.8e+02f)) : ((iTemp45) ? 1.8e+02f - fTemp79 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp35) ? fTemp78 + 1.2e+02f : 1.3e+02f)) : ((iTemp32) ? 1.3e+02f - fTemp77 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp24) ? 1e+01f * fTemp27 + 1.2e+02f : 1.3e+02f)) : ((iTemp22) ? fTemp76 + 1.3e+02f : 1.5e+02f)) : ((iTemp19) ? 1.5e+02f - 3e+01f * fTemp23 : 1.2e+02f)) : 1.2e+02f) : ((iTemp15) ? 6e+01f * fTemp18 + 1.2e+02f : 1.8e+02f)) : ((iTemp13) ? 1.8e+02f - fTemp75 : 1.3e+02f)) : ((iTemp11) ? 1.3e+02f - fTemp74 : 1.2e+02f)) : 1.2e+02f) : ((iTemp6) ? fTemp73 + 1.2e+02f : 1.3e+02f)) : ((iTemp7) ? 1.3e+02f - 1e+01f * fTemp8 : 1.2e+02f));
			float fTemp83 = fTemp72 * fTemp82;
			fRec10[0] = ((iTemp61) ? fTemp83 : fRec10[1]);
			float fTemp84 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec10[0]));
			fRec11[0] = ((iTemp61) ? fTemp82 : fRec11[1]);
			float fTemp85 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec11[0]));
			fRec7[0] = fVec1[2] - (fRec7[1] * (0.0f - (fTemp84 + fTemp85)) + fTemp84 * fTemp85 * fRec7[2]);
			int iTemp86 = int(fVec1[1]);
			float fTemp87 = 1e+02f * fTemp8;
			float fTemp88 = 3.5e+02f * fTemp46;
			float fTemp89 = fConst1 * ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 3.5e+03f : ((iTemp53) ? 3.5e+03f - 2e+02f * fTemp5 : 3.3e+03f)) : ((iTemp51) ? 4e+02f * fTemp54 + 3.3e+03f : 3.7e+03f)) : ((iTemp49) ? 3.7e+03f - 2e+02f * fTemp52 : 3.5e+03f)) : 3.5e+03f) : ((iTemp45) ? 3.5e+03f - 1.05e+03f * fTemp48 : 2.45e+03f)) : ((iTemp43) ? fTemp88 + 2.45e+03f : 2.8e+03f)) : ((iTemp41) ? 2.5e+02f * fTemp44 + 2.8e+03f : 3.05e+03f)) : ((iTemp39) ? 3.05e+03f - 4.5e+02f * fTemp42 : 2.6e+03f)) : ((iTemp37) ? 75.0f * fTemp40 + 2.6e+03f : 2675.0f)) : ((iTemp35) ? 325.0f * fTemp38 + 2675.0f : 3e+03f)) : 3e+03f) : ((iTemp30) ? 3.5e+02f * fTemp33 + 3e+03f : 3.35e+03f)) : ((iTemp28) ? 3.35e+03f - 3.5e+02f * fTemp31 : 3e+03f)) : 3e+03f) : ((iTemp24) ? 9e+02f * fTemp27 + 3e+03f : 3.9e+03f)) : ((iTemp22) ? 3.9e+03f - 3e+02f * fTemp25 : 3.6e+03f)) : ((iTemp19) ? 3e+02f * fTemp23 + 3.6e+03f : 3.9e+03f)) : ((iTemp17) ? 3.9e+03f - 1e+02f * fTemp20 : 3.8e+03f)) : 3.8e+03f) : ((iTemp13) ? 3.8e+03f - 9e+02f * fTemp16 : 2.9e+03f)) : ((iTemp11) ? 3e+02f * fTemp14 + 2.9e+03f : 3.2e+03f)) : ((iTemp9) ? 5e+01f * fTemp12 + 3.2e+03f : 3.25e+03f)) : ((iTemp6) ? 3.25e+03f - 4.5e+02f * fTemp10 : 2.8e+03f)) : ((iTemp7) ? fTemp87 + 2.8e+03f : 2.9e+03f));
			float fTemp90 = ((iTemp86) ? 0.0f : fRec12[1] + fTemp89);
			fRec12[0] = fTemp90 - std::floor(fTemp90);
			float fTemp91 = float(fRec8[0] == 2.0f) * fTemp59;
			fVec2[0] = fTemp91;
			int iTemp92 = int(fTemp91);
			fRec14[0] = ((iTemp92) ? fTemp83 : fRec14[1]);
			float fTemp93 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec14[0]));
			fRec15[0] = ((iTemp92) ? fTemp82 : fRec15[1]);
			float fTemp94 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec15[0]));
			fRec13[0] = fVec2[2] - (fRec13[1] * (0.0f - (fTemp93 + fTemp94)) + fTemp93 * fTemp94 * fRec13[2]);
			int iTemp95 = int(fVec2[1]);
			float fTemp96 = ((iTemp95) ? 0.0f : fTemp89 + fRec16[1]);
			fRec16[0] = fTemp96 - std::floor(fTemp96);
			float fTemp97 = float(fRec8[0] == 3.0f) * fTemp59;
			fVec3[0] = fTemp97;
			int iTemp98 = int(fTemp97);
			fRec18[0] = ((iTemp98) ? fTemp83 : fRec18[1]);
			float fTemp99 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec18[0]));
			fRec19[0] = ((iTemp98) ? fTemp82 : fRec19[1]);
			float fTemp100 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec19[0]));
			fRec17[0] = fVec3[2] - (fRec17[1] * (0.0f - (fTemp99 + fTemp100)) + fTemp99 * fTemp100 * fRec17[2]);
			int iTemp101 = int(fVec3[1]);
			float fTemp102 = ((iTemp101) ? 0.0f : fTemp89 + fRec20[1]);
			fRec20[0] = fTemp102 - std::floor(fTemp102);
			float fTemp103 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 0.1f : ((iTemp53) ? 0.1f - 0.068377f * fTemp5 : 0.031623f)) : 0.031623f) : ((iTemp49) ? 0.126866f * fTemp52 + 0.031623f : 0.158489f)) : ((iTemp47) ? 0.158489f - 0.126866f * fTemp50 : 0.031623f)) : ((iTemp45) ? 0.32319f * fTemp48 + 0.031623f : 0.354813f)) : 0.354813f) : ((iTemp41) ? 0.354813f - 0.196324f * fTemp44 : 0.158489f)) : ((iTemp39) ? 0.158489f - 0.069364f * fTemp42 : 0.089125f)) : ((iTemp37) ? 0.089125f - 0.064006f * fTemp40 : 0.025119f)) : ((iTemp35) ? 0.045676f * fTemp38 + 0.025119f : 0.070795f)) : ((iTemp32) ? 0.055098f * fTemp36 + 0.070795f : 0.125893f)) : ((iTemp30) ? 0.125893f - 0.062797f * fTemp33 : 0.063096f)) : ((iTemp28) ? 0.063096f - 0.012977f * fTemp31 : 0.050119f)) : ((iTemp26) ? 0.020676f * fTemp29 + 0.050119f : 0.070795f)) : ((iTemp24) ? 0.070795f - 0.045676f * fTemp27 : 0.025119f)) : ((iTemp22) ? 0.152709f * fTemp25 + 0.025119f : 0.177828f)) : ((iTemp19) ? 0.177828f - 0.127709f * fTemp23 : 0.050119f)) : fTemp21) : ((iTemp15) ? 0.079433f - 0.06165f * fTemp18 : 0.017783f)) : ((iTemp13) ? 0.428901f * fTemp16 + 0.017783f : 0.446684f)) : ((iTemp11) ? 0.446684f - 0.195495f * fTemp14 : 0.251189f)) : ((iTemp9) ? 0.251189f - 0.125296f * fTemp12 : 0.125893f)) : ((iTemp6) ? 0.125296f * fTemp10 + 0.125893f : 0.251189f)) : ((iTemp7) ? 0.251189f - 0.109935f * fTemp8 : 0.141254f));
			float fTemp104 = 2e+01f * fTemp14;
			float fTemp105 = 2e+01f * fTemp27;
			float fTemp106 = 2e+01f * fTemp36;
			float fTemp107 = 2e+01f * fTemp38;
			float fTemp108 = 1e+01f * fTemp46;
			float fTemp109 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? 1.2e+02f : ((iTemp49) ? 1.2e+02f - fTemp80 : 1e+02f)) : ((iTemp47) ? 7e+01f * fTemp50 + 1e+02f : 1.7e+02f)) : ((iTemp45) ? 1.7e+02f - fTemp79 : 1.1e+02f)) : ((iTemp43) ? 1.1e+02f - fTemp108 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp35) ? fTemp107 + 1e+02f : 1.2e+02f)) : ((iTemp32) ? 1.2e+02f - fTemp106 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp24) ? fTemp105 + 1e+02f : 1.2e+02f)) : 1.2e+02f) : ((iTemp19) ? 1.2e+02f - 2e+01f * fTemp23 : 1e+02f)) : 1e+02f) : ((iTemp15) ? 7e+01f * fTemp18 + 1e+02f : 1.7e+02f)) : ((iTemp13) ? 1.7e+02f - fTemp75 : 1.2e+02f)) : ((iTemp11) ? 1.2e+02f - fTemp104 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f);
			float fTemp110 = fTemp72 * fTemp109;
			fRec22[0] = ((iTemp61) ? fTemp110 : fRec22[1]);
			float fTemp111 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec22[0]));
			fRec23[0] = ((iTemp61) ? fTemp109 : fRec23[1]);
			float fTemp112 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec23[0]));
			fRec21[0] = fVec1[2] - (fRec21[1] * (0.0f - (fTemp111 + fTemp112)) + fTemp111 * fTemp112 * fRec21[2]);
			float fTemp113 = 5e+01f * fTemp36;
			float fTemp114 = fConst1 * ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 2.8e+03f : ((iTemp53) ? 2.8e+03f - 1e+02f * fTemp5 : 2.7e+03f)) : 2.7e+03f) : ((iTemp49) ? 1.3e+02f * fTemp52 + 2.7e+03f : 2.83e+03f)) : ((iTemp47) ? 2.83e+03f - 3e+02f * fTemp50 : 2.53e+03f)) : ((iTemp45) ? 2.53e+03f - 2.8e+02f * fTemp48 : 2.25e+03f)) : ((iTemp43) ? 1.5e+02f * fTemp46 + 2.25e+03f : 2.4e+03f)) : ((iTemp41) ? 2e+02f * fTemp44 + 2.4e+03f : 2.6e+03f)) : ((iTemp39) ? 2.6e+03f - 2e+02f * fTemp42 : 2.4e+03f)) : 2.4e+03f) : ((iTemp35) ? 3.5e+02f * fTemp38 + 2.4e+03f : 2.75e+03f)) : ((iTemp32) ? 2.75e+03f - fTemp113 : 2.7e+03f)) : ((iTemp30) ? 2e+02f * fTemp33 + 2.7e+03f : 2.9e+03f)) : ((iTemp28) ? 2.9e+03f - 2e+02f * fTemp31 : 2.7e+03f)) : ((iTemp26) ? 5e+01f * fTemp29 + 2.7e+03f : 2.75e+03f)) : ((iTemp24) ? 1.5e+02f * fTemp27 + 2.75e+03f : 2.9e+03f)) : ((iTemp22) ? 2.9e+03f - 1e+02f * fTemp25 : 2.8e+03f)) : ((iTemp19) ? 1.5e+02f * fTemp23 + 2.8e+03f : 2.95e+03f)) : ((iTemp17) ? 2.95e+03f - 1.2e+02f * fTemp20 : 2.83e+03f)) : ((iTemp15) ? 2.83e+03f - 1.3e+02f * fTemp18 : 2.7e+03f)) : ((iTemp13) ? 2.7e+03f - fTemp75 : 2.65e+03f)) : ((iTemp11) ? 2.65e+03f - 5e+01f * fTemp14 : 2.6e+03f)) : ((iTemp9) ? 2e+02f * fTemp12 + 2.6e+03f : 2.8e+03f)) : ((iTemp6) ? 2.8e+03f - 2e+02f * fTemp10 : 2.6e+03f)) : ((iTemp7) ? fTemp87 + 2.6e+03f : 2.7e+03f));
			float fTemp115 = ((iTemp86) ? 0.0f : fRec24[1] + fTemp114);
			fRec24[0] = fTemp115 - std::floor(fTemp115);
			fRec26[0] = ((iTemp92) ? fTemp110 : fRec26[1]);
			float fTemp116 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec26[0]));
			fRec27[0] = ((iTemp92) ? fTemp109 : fRec27[1]);
			float fTemp117 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec27[0]));
			fRec25[0] = fVec2[2] - (fRec25[1] * (0.0f - (fTemp116 + fTemp117)) + fTemp116 * fTemp117 * fRec25[2]);
			float fTemp118 = ((iTemp95) ? 0.0f : fTemp114 + fRec28[1]);
			fRec28[0] = fTemp118 - std::floor(fTemp118);
			fRec30[0] = ((iTemp98) ? fTemp110 : fRec30[1]);
			float fTemp119 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec30[0]));
			fRec31[0] = ((iTemp98) ? fTemp109 : fRec31[1]);
			float fTemp120 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec31[0]));
			fRec29[0] = fVec3[2] - (fRec29[1] * (0.0f - (fTemp119 + fTemp120)) + fTemp119 * fTemp120 * fRec29[2]);
			float fTemp121 = ((iTemp101) ? 0.0f : fTemp114 + fRec32[1]);
			fRec32[0] = fTemp121 - std::floor(fTemp121);
			float fTemp122 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 0.630957f : ((iTemp53) ? 0.630957f - 0.567861f * fTemp5 : 0.063096f)) : ((iTemp51) ? 0.036904f * fTemp54 + 0.063096f : 0.1f)) : ((iTemp49) ? 0.254813f * fTemp52 + 0.1f : 0.354813f)) : ((iTemp47) ? 0.354813f - 0.103624f * fTemp50 : 0.251189f)) : ((iTemp45) ? 0.195495f * fTemp48 + 0.251189f : 0.446684f)) : ((iTemp43) ? 0.446684f - 0.195495f * fTemp46 : 0.251189f)) : ((iTemp41) ? 0.251189f - 0.219566f * fTemp44 : 0.031623f)) : ((iTemp39) ? 0.250215f * fTemp42 + 0.031623f : 0.281838f)) : ((iTemp37) ? 0.281838f - 0.181838f * fTemp40 : 0.1f)) : ((iTemp35) ? 0.401187f * fTemp38 + 0.1f : 0.501187f)) : ((iTemp32) ? 0.501187f - 0.301661f * fTemp36 : 0.199526f)) : ((iTemp30) ? 0.199526f - 0.13643f * fTemp33 : 0.063096f)) : ((iTemp28) ? 0.253132f * fTemp31 + 0.063096f : 0.316228f)) : ((iTemp26) ? 0.316228f - 0.216228f * fTemp29 : 0.1f)) : ((iTemp24) ? 0.401187f * fTemp27 + 0.1f : 0.501187f)) : ((iTemp22) ? 0.501187f - 0.401187f * fTemp25 : 0.1f)) : ((iTemp19) ? 0.151189f * fTemp23 + 0.1f : 0.251189f)) : ((iTemp17) ? 0.030649f * fTemp20 + 0.251189f : 0.281838f)) : ((iTemp15) ? 0.281838f - 0.123349f * fTemp18 : 0.158489f)) : ((iTemp13) ? 0.342698f * fTemp16 + 0.158489f : 0.501187f)) : ((iTemp11) ? 0.501187f - 0.301661f * fTemp14 : 0.199526f)) : ((iTemp9) ? 0.199526f - 0.021698f * fTemp12 : 0.177828f)) : ((iTemp6) ? 0.1384f * fTemp10 + 0.177828f : 0.316228f)) : ((iTemp7) ? 0.316228f - 0.216228f * fTemp8 : 0.1f));
			float fTemp123 = 1e+01f * fTemp48;
			float fTemp124 = 2e+01f * fTemp50;
			float fTemp125 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 9e+01f : ((iTemp53) ? 9e+01f - 1e+01f * fTemp5 : 8e+01f)) : ((iTemp51) ? 2e+01f * fTemp54 + 8e+01f : 1e+02f)) : ((iTemp49) ? 1e+02f - fTemp80 : 8e+01f)) : ((iTemp47) ? 8e+01f - fTemp124 : 6e+01f)) : ((iTemp45) ? fTemp123 + 6e+01f : 7e+01f)) : ((iTemp43) ? fTemp108 + 7e+01f : 8e+01f)) : ((iTemp41) ? 1e+01f * fTemp44 + 8e+01f : 9e+01f)) : ((iTemp39) ? 9e+01f - 1e+01f * fTemp42 : 8e+01f)) : 8e+01f) : ((iTemp35) ? fTemp78 + 8e+01f : 9e+01f)) : ((iTemp32) ? 9e+01f - fTemp77 : 8e+01f)) : ((iTemp30) ? 1e+01f * fTemp33 + 8e+01f : 9e+01f)) : ((iTemp28) ? 9e+01f - 1e+01f * fTemp31 : 8e+01f)) : ((iTemp26) ? 8e+01f - 2e+01f * fTemp29 : 6e+01f)) : ((iTemp24) ? 3e+01f * fTemp27 + 6e+01f : 9e+01f)) : ((iTemp22) ? 1e+01f * fTemp25 + 9e+01f : 1e+02f)) : ((iTemp19) ? 1e+02f - 1e+01f * fTemp23 : 9e+01f)) : ((iTemp17) ? 9e+01f - 1e+01f * fTemp20 : 8e+01f)) : ((iTemp15) ? 8e+01f - 2e+01f * fTemp18 : 6e+01f)) : ((iTemp13) ? 3e+01f * fTemp16 + 6e+01f : 9e+01f)) : ((iTemp11) ? 9e+01f - fTemp74 : 8e+01f)) : ((iTemp9) ? 1e+01f * fTemp12 + 8e+01f : 9e+01f)) : ((iTemp6) ? 9e+01f - fTemp73 : 8e+01f)) : ((iTemp7) ? 8e+01f - 2e+01f * fTemp8 : 6e+01f));
			float fTemp126 = fTemp72 * fTemp125;
			fRec34[0] = ((iTemp92) ? fTemp126 : fRec34[1]);
			float fTemp127 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec34[0]));
			fRec35[0] = ((iTemp92) ? fTemp125 : fRec35[1]);
			float fTemp128 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec35[0]));
			fRec33[0] = fVec2[2] - (fRec33[1] * (0.0f - (fTemp127 + fTemp128)) + fTemp127 * fTemp128 * fRec33[2]);
			float fTemp129 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 1.15e+03f : ((iTemp53) ? 4.5e+02f * fTemp5 + 1.15e+03f : 1.6e+03f)) : ((iTemp51) ? 1e+02f * fTemp54 + 1.6e+03f : 1.7e+03f)) : ((iTemp49) ? 1.7e+03f - 9e+02f * fTemp52 : 8e+02f)) : ((iTemp47) ? 8e+02f - 1e+02f * fTemp50 : 7e+02f)) : ((iTemp45) ? 3.4e+02f * fTemp48 + 7e+02f : 1.04e+03f)) : ((iTemp43) ? 5.8e+02f * fTemp46 + 1.04e+03f : 1.62e+03f)) : ((iTemp41) ? 1.3e+02f * fTemp44 + 1.62e+03f : 1.75e+03f)) : ((iTemp39) ? 1.75e+03f - 1e+03f * fTemp42 : 7.5e+02f)) : ((iTemp37) ? 7.5e+02f - 1.5e+02f * fTemp40 : 6e+02f)) : ((iTemp35) ? 5.2e+02f * fTemp38 + 6e+02f : 1.12e+03f)) : ((iTemp32) ? 6.8e+02f * fTemp36 + 1.12e+03f : 1.8e+03f)) : ((iTemp30) ? 5e+01f * fTemp33 + 1.8e+03f : 1.85e+03f)) : ((iTemp28) ? 1.85e+03f - 1.03e+03f * fTemp31 : 8.2e+02f)) : ((iTemp26) ? 8.2e+02f - 1.9e+02f * fTemp29 : 6.3e+02f)) : ((iTemp24) ? 5.2e+02f * fTemp27 + 6.3e+02f : 1.15e+03f)) : ((iTemp22) ? 8.5e+02f * fTemp25 + 1.15e+03f : 2e+03f)) : ((iTemp19) ? 1.4e+02f * fTemp23 + 2e+03f : 2.14e+03f)) : ((iTemp17) ? 2.14e+03f - 1.34e+03f * fTemp20 : 8e+02f)) : ((iTemp15) ? 8e+02f - 1e+02f * fTemp18 : 7e+02f)) : ((iTemp13) ? 3.8e+02f * fTemp16 + 7e+02f : 1.08e+03f)) : ((iTemp11) ? 6.2e+02f * fTemp14 + 1.08e+03f : 1.7e+03f)) : ((iTemp9) ? 1.7e+02f * fTemp12 + 1.7e+03f : 1.87e+03f)) : ((iTemp6) ? 1.87e+03f - 1.07e+03f * fTemp10 : 8e+02f)) : ((iTemp7) ? 8e+02f - 2e+02f * fTemp8 : 6e+02f));
			float fTemp130 = 2.0f * fTemp3 + 3e+01f;
			float fTemp131 = fConst1 * ((iSlow19) ? (((fTemp129 >= 1.3e+03f) & (fTemp3 >= 2e+02f)) ? fTemp129 - 0.00095238094f * (fTemp3 + -2e+02f) * (fTemp129 + -1.3e+03f) : ((fTemp129 <= fTemp130) ? fTemp130 : fTemp129)) : fTemp129);
			float fTemp132 = ((iTemp95) ? 0.0f : fTemp131 + fRec36[1]);
			fRec36[0] = fTemp132 - std::floor(fTemp132);
			fRec38[0] = ((iTemp61) ? fTemp126 : fRec38[1]);
			float fTemp133 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec38[0]));
			fRec39[0] = ((iTemp61) ? fTemp125 : fRec39[1]);
			float fTemp134 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec39[0]));
			fRec37[0] = fVec1[2] - (fRec37[1] * (0.0f - (fTemp133 + fTemp134)) + fTemp133 * fTemp134 * fRec37[2]);
			float fTemp135 = ((iTemp86) ? 0.0f : fRec40[1] + fTemp131);
			fRec40[0] = fTemp135 - std::floor(fTemp135);
			fRec42[0] = ((iTemp98) ? fTemp126 : fRec42[1]);
			float fTemp136 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec42[0]));
			fRec43[0] = ((iTemp98) ? fTemp125 : fRec43[1]);
			float fTemp137 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec43[0]));
			fRec41[0] = fVec3[2] - (fRec41[1] * (0.0f - (fTemp136 + fTemp137)) + fTemp136 * fTemp137 * fRec41[2]);
			float fTemp138 = ((iTemp101) ? 0.0f : fTemp131 + fRec44[1]);
			fRec44[0] = fTemp138 - std::floor(fTemp138);
			float fTemp139 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 8e+01f : ((iTemp53) ? 8e+01f - fTemp81 : 6e+01f)) : ((iTemp51) ? 6e+01f - 1e+01f * fTemp54 : 5e+01f)) : ((iTemp49) ? fTemp80 + 5e+01f : 7e+01f)) : ((iTemp47) ? 7e+01f - fTemp124 : 5e+01f)) : ((iTemp45) ? fTemp123 + 5e+01f : 6e+01f)) : ((iTemp43) ? 6e+01f - 2e+01f * fTemp46 : 4e+01f)) : ((iTemp41) ? 2e+01f * fTemp44 + 4e+01f : 6e+01f)) : ((iTemp39) ? 6e+01f - 2e+01f * fTemp42 : 4e+01f)) : 4e+01f) : ((iTemp35) ? 4e+01f * (fTemp38 + 1.0f) : 8e+01f)) : ((iTemp32) ? 8e+01f - fTemp77 : 7e+01f)) : ((iTemp30) ? 7e+01f - 3e+01f * fTemp33 : 4e+01f)) : 4e+01f) : 4e+01f) : ((iTemp24) ? 4e+01f * (fTemp27 + 1.0f) : 8e+01f)) : ((iTemp22) ? 8e+01f - fTemp76 : 6e+01f)) : 6e+01f) : ((iTemp17) ? 6e+01f - 2e+01f * fTemp20 : 4e+01f)) : ((iTemp15) ? 1e+01f * fTemp18 + 4e+01f : 5e+01f)) : 5e+01f) : ((iTemp11) ? fTemp104 + 5e+01f : 7e+01f)) : ((iTemp9) ? 7e+01f - 3e+01f * fTemp12 : 4e+01f)) : ((iTemp6) ? 3e+01f * fTemp10 + 4e+01f : 7e+01f)) : ((iTemp7) ? 7e+01f - 3e+01f * fTemp8 : 4e+01f));
			float fTemp140 = fTemp72 * fTemp139;
			fRec46[0] = ((iTemp92) ? fTemp140 : fRec46[1]);
			float fTemp141 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec46[0]));
			fRec47[0] = ((iTemp92) ? fTemp139 : fRec47[1]);
			float fTemp142 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec47[0]));
			fRec45[0] = fVec2[2] - (fRec45[1] * (0.0f - (fTemp141 + fTemp142)) + fTemp141 * fTemp142 * fRec45[2]);
			float fTemp143 = 8e+01f * fTemp23;
			float fTemp144 = 5e+01f * fTemp40;
			float fTemp145 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 8e+02f : ((iTemp53) ? 8e+02f - 4e+02f * fTemp5 : 4e+02f)) : ((iTemp51) ? 4e+02f - 5e+01f * fTemp54 : 3.5e+02f)) : ((iTemp49) ? 1e+02f * fTemp52 + 3.5e+02f : 4.5e+02f)) : ((iTemp47) ? 4.5e+02f - 125.0f * fTemp50 : 325.0f)) : ((iTemp45) ? 275.0f * fTemp48 + 325.0f : 6e+02f)) : ((iTemp43) ? 6e+02f - 2e+02f * fTemp46 : 4e+02f)) : ((iTemp41) ? 4e+02f - 1.5e+02f * fTemp44 : 2.5e+02f)) : ((iTemp39) ? 1.5e+02f * fTemp42 + 2.5e+02f : 4e+02f)) : ((iTemp37) ? 4e+02f - fTemp144 : 3.5e+02f)) : ((iTemp35) ? 3.1e+02f * fTemp38 + 3.5e+02f : 6.6e+02f)) : ((iTemp32) ? 6.6e+02f - 2.2e+02f * fTemp36 : 4.4e+02f)) : ((iTemp30) ? 4.4e+02f - 1.7e+02f * fTemp33 : 2.7e+02f)) : ((iTemp28) ? 1.6e+02f * fTemp31 + 2.7e+02f : 4.3e+02f)) : ((iTemp26) ? 4.3e+02f - 6e+01f * fTemp29 : 3.7e+02f)) : ((iTemp24) ? 4.3e+02f * fTemp27 + 3.7e+02f : 8e+02f)) : ((iTemp22) ? 8e+02f - 4.5e+02f * fTemp25 : 3.5e+02f)) : ((iTemp19) ? 3.5e+02f - fTemp143 : 2.7e+02f)) : ((iTemp17) ? 1.8e+02f * fTemp20 + 2.7e+02f : 4.5e+02f)) : ((iTemp15) ? 4.5e+02f - 125.0f * fTemp18 : 325.0f)) : ((iTemp13) ? 325.0f * (fTemp16 + 1.0f) : 6.5e+02f)) : ((iTemp11) ? 6.5e+02f - 2.5e+02f * fTemp14 : 4e+02f)) : ((iTemp9) ? 4e+02f - 1.1e+02f * fTemp12 : 2.9e+02f)) : ((iTemp6) ? 1.1e+02f * fTemp10 + 2.9e+02f : 4e+02f)) : ((iTemp7) ? 4e+02f - 5e+01f * fTemp8 : 3.5e+02f));
			float fTemp146 = fConst1 * ((fTemp145 <= fTemp3) ? fTemp3 : fTemp145);
			float fTemp147 = ((iTemp95) ? 0.0f : fTemp146 + fRec48[1]);
			fRec48[0] = fTemp147 - std::floor(fTemp147);
			fRec50[0] = ((iTemp61) ? fTemp140 : fRec50[1]);
			float fTemp148 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec50[0]));
			fRec51[0] = ((iTemp61) ? fTemp139 : fRec51[1]);
			float fTemp149 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec51[0]));
			fRec49[0] = fVec1[2] - (fRec49[1] * (0.0f - (fTemp148 + fTemp149)) + fTemp148 * fTemp149 * fRec49[2]);
			float fTemp150 = ((iTemp86) ? 0.0f : fRec52[1] + fTemp146);
			fRec52[0] = fTemp150 - std::floor(fTemp150);
			fRec54[0] = ((iTemp98) ? fTemp140 : fRec54[1]);
			float fTemp151 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec54[0]));
			fRec55[0] = ((iTemp98) ? fTemp139 : fRec55[1]);
			float fTemp152 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec55[0]));
			fRec53[0] = fVec3[2] - (fRec53[1] * (0.0f - (fTemp151 + fTemp152)) + fTemp151 * fTemp152 * fRec53[2]);
			float fTemp153 = ((iTemp101) ? 0.0f : fTemp146 + fRec56[1]);
			fRec56[0] = fTemp153 - std::floor(fTemp153);
			float fTemp154 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? 0.001f : ((iTemp49) ? 0.000778f * fTemp52 + 0.001f : 0.001778f)) : ((iTemp47) ? 0.001778f - 0.001147f * fTemp50 : 0.000631f)) : ((iTemp45) ? 0.099369f * fTemp48 + 0.000631f : 0.1f)) : ((iTemp43) ? 0.025893f * fTemp46 + 0.1f : 0.125893f)) : ((iTemp41) ? 0.125893f - 0.086082f * fTemp44 : 0.039811f)) : ((iTemp39) ? 0.039811f - 0.029811f * fTemp42 : 0.01f)) : ((iTemp37) ? 0.005849f * fTemp40 + 0.01f : 0.015849f)) : ((iTemp35) ? 0.015849f - 0.00326f * fTemp38 : 0.012589f)) : ((iTemp32) ? 0.087411f * fTemp36 + 0.012589f : 0.1f)) : fTemp34) : ((iTemp28) ? 0.004104f * fTemp31 + 0.015849f : 0.019953f)) : 0.019953f) : ((iTemp24) ? 0.019953f - 0.016791f * fTemp27 : 0.003162f)) : ((iTemp22) ? 0.003162f - 0.001577f * fTemp25 : 0.001585f)) : ((iTemp19) ? 0.004725f * fTemp23 + 0.001585f : 0.00631f)) : ((iTemp17) ? 0.00631f - 0.003148f * fTemp20 : 0.003162f)) : ((iTemp15) ? 0.003162f - 0.002162f * fTemp18 : 0.001f)) : ((iTemp13) ? 0.078433f * fTemp16 + 0.001f : 0.079433f)) : ((iTemp11) ? 0.020567f * fTemp14 + 0.079433f : 0.1f)) : ((iTemp9) ? 0.1f - 0.068377f * fTemp12 : 0.031623f)) : ((iTemp6) ? 0.018496f * fTemp10 + 0.031623f : 0.050119f)) : 0.050119f);
			float fTemp155 = ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? ((iTemp49) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 1.4e+02f : ((iTemp53) ? 6e+01f * fTemp5 + 1.4e+02f : 2e+02f)) : 2e+02f) : ((iTemp49) ? 2e+02f - 65.0f * fTemp52 : 135.0f)) : ((iTemp47) ? 65.0f * fTemp50 + 135.0f : 2e+02f)) : ((iTemp45) ? 2e+02f - 7e+01f * fTemp48 : 1.3e+02f)) : ((iTemp43) ? 1.3e+02f - fTemp108 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp35) ? fTemp107 + 1.2e+02f : 1.4e+02f)) : ((iTemp32) ? 1.4e+02f - fTemp106 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp24) ? fTemp105 + 1.2e+02f : 1.4e+02f)) : ((iTemp22) ? 6e+01f * fTemp25 + 1.4e+02f : 2e+02f)) : ((iTemp19) ? 2e+02f - fTemp143 : 1.2e+02f)) : 1.2e+02f) : ((iTemp15) ? 8e+01f * fTemp18 + 1.2e+02f : 2e+02f)) : ((iTemp13) ? 2e+02f - 6e+01f * fTemp16 : 1.4e+02f)) : ((iTemp11) ? 1.4e+02f - fTemp104 : 1.2e+02f)) : 1.2e+02f) : ((iTemp6) ? 15.0f * fTemp10 + 1.2e+02f : 135.0f)) : ((iTemp7) ? 135.0f - 15.0f * fTemp8 : 1.2e+02f));
			float fTemp156 = fTemp72 * fTemp155;
			fRec58[0] = ((iTemp92) ? fTemp156 : fRec58[1]);
			float fTemp157 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec58[0]));
			fRec59[0] = ((iTemp92) ? fTemp155 : fRec59[1]);
			float fTemp158 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec59[0]));
			fRec57[0] = fVec2[2] - (fRec57[1] * (0.0f - (fTemp157 + fTemp158)) + fTemp157 * fTemp158 * fRec57[2]);
			float fTemp159 = fConst1 * ((iTemp6) ? ((iTemp9) ? ((iTemp11) ? ((iTemp13) ? ((iTemp15) ? ((iTemp17) ? ((iTemp19) ? ((iTemp22) ? ((iTemp24) ? ((iTemp26) ? ((iTemp28) ? ((iTemp30) ? ((iTemp32) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp43) ? ((iTemp45) ? ((iTemp47) ? 4.95e+03f : ((iTemp45) ? 4.95e+03f - 2.2e+03f * fTemp48 : 2.75e+03f)) : ((iTemp43) ? fTemp88 + 2.75e+03f : 3.1e+03f)) : ((iTemp41) ? 2.4e+02f * fTemp44 + 3.1e+03f : 3.34e+03f)) : ((iTemp39) ? 3.34e+03f - 4.4e+02f * fTemp42 : 2.9e+03f)) : ((iTemp37) ? fTemp144 + 2.9e+03f : 2.95e+03f)) : ((iTemp35) ? 4e+02f * fTemp38 + 2.95e+03f : 3.35e+03f)) : ((iTemp32) ? 3.35e+03f - fTemp113 : 3.3e+03f)) : ((iTemp30) ? 2.9e+02f * fTemp33 + 3.3e+03f : 3.59e+03f)) : ((iTemp28) ? 3.59e+03f - 2.9e+02f * fTemp31 : 3.3e+03f)) : ((iTemp26) ? 1e+02f * fTemp29 + 3.3e+03f : 3.4e+03f)) : ((iTemp24) ? 1.55e+03f * fTemp27 + 3.4e+03f : 4.95e+03f)) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : ((iTemp13) ? 4.95e+03f - 1.7e+03f * fTemp16 : 3.25e+03f)) : ((iTemp11) ? 3.3e+02f * fTemp14 + 3.25e+03f : 3.58e+03f)) : ((iTemp9) ? 3.58e+03f - 4e+01f * fTemp12 : 3.54e+03f)) : ((iTemp6) ? 3.54e+03f - 5.4e+02f * fTemp10 : 3e+03f)) : ((iTemp7) ? 3e+02f * fTemp8 + 3e+03f : 3.3e+03f));
			float fTemp160 = ((iTemp95) ? 0.0f : fTemp159 + fRec60[1]);
			fRec60[0] = fTemp160 - std::floor(fTemp160);
			fRec62[0] = ((iTemp61) ? fTemp156 : fRec62[1]);
			float fTemp161 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec62[0]));
			fRec63[0] = ((iTemp61) ? fTemp155 : fRec63[1]);
			float fTemp162 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec63[0]));
			fRec61[0] = fVec1[2] - (fRec61[1] * (0.0f - (fTemp161 + fTemp162)) + fTemp161 * fTemp162 * fRec61[2]);
			float fTemp163 = ((iTemp86) ? 0.0f : fRec64[1] + fTemp159);
			fRec64[0] = fTemp163 - std::floor(fTemp163);
			fRec66[0] = ((iTemp98) ? fTemp156 : fRec66[1]);
			float fTemp164 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec66[0]));
			fRec67[0] = ((iTemp98) ? fTemp155 : fRec67[1]);
			float fTemp165 = std::exp(fConst1 * (0.0f - 3.1415927f * fRec67[0]));
			fRec65[0] = fVec3[2] - (fRec65[1] * (0.0f - (fTemp164 + fTemp165)) + fTemp164 * fTemp165 * fRec65[2]);
			float fTemp166 = ((iTemp101) ? 0.0f : fTemp159 + fRec68[1]);
			fRec68[0] = fTemp166 - std::floor(fTemp166);
			output0[i0] = FAUSTFLOAT(75.0f * fRec0[0] * (((iSlow7) ? fTemp57 * fTemp56 : fTemp4 * fTemp56) * (fRec7[0] * (1.0f - (fTemp85 + fTemp84 * (1.0f - fTemp85))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec12[0])] + fRec13[0] * (1.0f - (fTemp94 + fTemp93 * (1.0f - fTemp94))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec16[0])] + fRec17[0] * (1.0f - (fTemp100 + fTemp99 * (1.0f - fTemp100))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec20[0])]) + ((iSlow7) ? fTemp57 * fTemp103 : fTemp4 * fTemp103) * (fRec21[0] * (1.0f - (fTemp112 + fTemp111 * (1.0f - fTemp112))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec24[0])] + fRec25[0] * (1.0f - (fTemp117 + fTemp116 * (1.0f - fTemp117))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec28[0])] + fRec29[0] * (1.0f - (fTemp120 + fTemp119 * (1.0f - fTemp120))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec32[0])]) + ((iSlow7) ? fTemp57 * fTemp122 : fTemp4 * fTemp122) * (fRec33[0] * (1.0f - (fTemp128 + fTemp127 * (1.0f - fTemp128))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec36[0])] + fRec37[0] * (fTemp133 * (fTemp134 + -1.0f) + (1.0f - fTemp134)) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec40[0])] + fRec41[0] * (1.0f - (fTemp137 + fTemp136 * (1.0f - fTemp137))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec44[0])]) + ((iSlow7) ? fTemp57 : fTemp4) * (fRec45[0] * (1.0f - (fTemp142 + fTemp141 * (1.0f - fTemp142))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec48[0])] + fRec49[0] * (1.0f - (fTemp149 + fTemp148 * (1.0f - fTemp149))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec52[0])] + fRec53[0] * (1.0f - (fTemp152 + fTemp151 * (1.0f - fTemp152))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec56[0])]) + ((iSlow7) ? fTemp57 * fTemp154 : fTemp4 * fTemp154) * (fRec57[0] * (1.0f - (fTemp158 + fTemp157 * (1.0f - fTemp158))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec60[0])] + fRec61[0] * (1.0f - (fTemp162 + fTemp161 * (1.0f - fTemp162))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec64[0])] + fRec65[0] * (1.0f - (fTemp165 + fTemp164 * (1.0f - fTemp165))) * ftbl0synth_voice_fofcSIG0[int(65536.0f * fRec68[0])])));
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec1[1] = fRec1[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			fVec1[2] = fVec1[1];
			fVec1[1] = fVec1[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec12[1] = fRec12[0];
			fVec2[2] = fVec2[1];
			fVec2[1] = fVec2[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec16[1] = fRec16[0];
			fVec3[2] = fVec3[1];
			fVec3[1] = fVec3[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec20[1] = fRec20[0];
			fRec22[1] = fRec22[0];
			fRec23[1] = fRec23[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec24[1] = fRec24[0];
			fRec26[1] = fRec26[0];
			fRec27[1] = fRec27[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec28[1] = fRec28[0];
			fRec30[1] = fRec30[0];
			fRec31[1] = fRec31[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec32[1] = fRec32[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec36[1] = fRec36[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec40[1] = fRec40[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec44[1] = fRec44[0];
			fRec46[1] = fRec46[0];
			fRec47[1] = fRec47[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec48[1] = fRec48[0];
			fRec50[1] = fRec50[0];
			fRec51[1] = fRec51[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec52[1] = fRec52[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec56[1] = fRec56[0];
			fRec58[1] = fRec58[0];
			fRec59[1] = fRec59[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec60[1] = fRec60[0];
			fRec62[1] = fRec62[0];
			fRec63[1] = fRec63[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec64[1] = fRec64[0];
			fRec66[1] = fRec66[0];
			fRec67[1] = fRec67[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec68[1] = fRec68[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _synth_voice_fofc_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_voice_fofc_UI<T>::name(sym(synth_voice_fofc));

typedef _synth_voice_fofc_UI<synth_voice_fofc> synth_voice_fofc_UI;

class faust_synth_voice_fofc_tilde : public FaustExternal<synth_voice_fofc, synth_voice_fofc_UI> {
public:
    faust_synth_voice_fofc_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
