/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "synth.risset_arp"
version: "0.1"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn synth_risset_arp -scn synth_risset_arp_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_risset_arp_H__
#define  __synth_risset_arp_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_risset_arp_dsp.h ********************************
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

class FAUST_API synth_risset_arp_dsp {

    public:

        synth_risset_arp_dsp() {}
        virtual ~synth_risset_arp_dsp() {}

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
        virtual synth_risset_arp_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_risset_arp_dsp {

    protected:

        synth_risset_arp_dsp* fDSP;

    public:

        decorator_dsp(synth_risset_arp_dsp* synth_risset_arp_dsp = nullptr):fDSP(synth_risset_arp_dsp) {}
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
    
        virtual synth_risset_arp_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_risset_arp_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_risset_arp_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_risset_arp : public synth_risset_arp_dsp {
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
#define FAUSTCLASS synth_risset_arp
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

class synth_risset_arpSIG0 {
	
  private:
	
	int iVec0[2];
	int iRec0[2];
	
  public:
	
	int getNumInputssynth_risset_arpSIG0() {
		return 0;
	}
	int getNumOutputssynth_risset_arpSIG0() {
		return 1;
	}
	
	void instanceInitsynth_risset_arpSIG0(int sample_rate) {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iRec0[l1] = 0;
		}
	}
	
	void fillsynth_risset_arpSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec0[0] = (iVec0[1] + iRec0[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec0[0]));
			iVec0[1] = iVec0[0];
			iRec0[1] = iRec0[0];
		}
	}

};

static synth_risset_arpSIG0* newsynth_risset_arpSIG0() { return (synth_risset_arpSIG0*)new synth_risset_arpSIG0(); }
static void deletesynth_risset_arpSIG0(synth_risset_arpSIG0* dsp) { delete dsp; }

static float ftbl0synth_risset_arpSIG0[65536];
static float synth_risset_arp_faustpower2_f(float value) {
	return value * value;
}

class synth_risset_arp : public synth_risset_arp_dsp {
	
 private:
	
	int fSampleRate;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fConst3;
	float fRec2[2];
	float fConst4;
	FAUSTFLOAT fHslider1;
	float fRec3[2];
	float fRec1[2];
	FAUSTFLOAT fHslider2;
	float fRec4[2];
	FAUSTFLOAT fHslider3;
	float fRec5[2];
	FAUSTFLOAT fHslider4;
	float fRec6[2];
	FAUSTFLOAT fHslider5;
	float fRec7[2];
	FAUSTFLOAT fHslider6;
	float fRec8[2];
	FAUSTFLOAT fHslider7;
	float fRec9[2];
	FAUSTFLOAT fHslider8;
	float fRec10[2];
	FAUSTFLOAT fHslider9;
	float fRec11[2];
	FAUSTFLOAT fHslider10;
	float fRec12[2];
	float fRec13[2];
	float fRec14[2];
	float fRec15[2];
	float fRec16[2];
	float fRec17[2];
	float fRec18[2];
	float fRec19[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn synth_risset_arp -scn synth_risset_arp_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("copyright", "Oliver Larkin");
		m->declare("description", "Jean Claude Risset's Harmonic Arpeggio Effect");
		m->declare("filename", "synth_risset_arp.dsp");
		m->declare("licence", "GPL");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "synth.risset_arp");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.3");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
		m->declare("version", "0.1");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		synth_risset_arpSIG0* sig0 = newsynth_risset_arpSIG0();
		sig0->instanceInitsynth_risset_arpSIG0(sample_rate);
		sig0->fillsynth_risset_arpSIG0(65536, ftbl0synth_risset_arpSIG0);
		deletesynth_risset_arpSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 0.441f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1e+02f);
		fHslider1 = FAUSTFLOAT(5.0f);
		fHslider2 = FAUSTFLOAT(1.0f);
		fHslider3 = FAUSTFLOAT(1.0f);
		fHslider4 = FAUSTFLOAT(1.0f);
		fHslider5 = FAUSTFLOAT(1.0f);
		fHslider6 = FAUSTFLOAT(1.0f);
		fHslider7 = FAUSTFLOAT(1.0f);
		fHslider8 = FAUSTFLOAT(1.0f);
		fHslider9 = FAUSTFLOAT(1.0f);
		fHslider10 = FAUSTFLOAT(1.0f);
	}
	
	virtual void instanceClear() {
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec10[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec11[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec12[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec13[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec14[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec15[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec19[l20] = 0.0f;
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
	
	virtual synth_risset_arp* clone() {
		return new synth_risset_arp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.risset_arp");
		ui_interface->addHorizontalSlider("detune", &fHslider1, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("freq", &fHslider0, FAUSTFLOAT(1e+02f), FAUSTFLOAT(4e+01f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("harmonic1", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic2", &fHslider6, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic3", &fHslider3, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic4", &fHslider7, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic5", &fHslider4, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic6", &fHslider8, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic7", &fHslider5, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("harmonic8", &fHslider9, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("spread", &fHslider10, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = fConst2 * float(fHslider0);
		float fSlow1 = fConst4 * float(fHslider1);
		float fSlow2 = fConst2 * float(fHslider2);
		float fSlow3 = fConst2 * float(fHslider3);
		float fSlow4 = fConst2 * float(fHslider4);
		float fSlow5 = fConst2 * float(fHslider5);
		float fSlow6 = fConst2 * float(fHslider6);
		float fSlow7 = fConst2 * float(fHslider7);
		float fSlow8 = fConst2 * float(fHslider8);
		float fSlow9 = fConst2 * float(fHslider9);
		float fSlow10 = fConst2 * float(fHslider10);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec2[0] = fSlow0 + fConst3 * fRec2[1];
			fRec3[0] = fSlow1 + fConst3 * fRec3[1];
			float fTemp0 = fRec1[1] + fConst1 * (fRec2[0] + fRec3[0]);
			fRec1[0] = fTemp0 - std::floor(fTemp0);
			float fTemp1 = ftbl0synth_risset_arpSIG0[int(65536.0f * fRec1[0])];
			fRec4[0] = fSlow2 + fConst3 * fRec4[1];
			fRec5[0] = fSlow3 + fConst3 * fRec5[1];
			float fTemp2 = synth_risset_arp_faustpower2_f(fTemp1);
			float fTemp3 = 2.0f * fTemp2 + -1.0f;
			float fTemp4 = 2.0f * fTemp3 + -1.0f;
			fRec6[0] = fSlow4 + fConst3 * fRec6[1];
			float fTemp5 = 1.0f - fTemp4;
			float fTemp6 = 1.0f - 2.0f * fTemp2 * fTemp5;
			float fTemp7 = 2.0f * fTemp6 - fTemp4;
			fRec7[0] = fSlow5 + fConst3 * fRec7[1];
			float fTemp8 = fTemp5 + fTemp7;
			float fTemp9 = 2.0f * fTemp2 * fTemp8 + -1.0f;
			float fTemp10 = 2.0f * fTemp9 - fTemp7;
			fRec8[0] = fSlow6 + fConst3 * fRec8[1];
			fRec9[0] = fSlow7 + fConst3 * fRec9[1];
			fRec10[0] = fSlow8 + fConst3 * fRec10[1];
			fRec11[0] = fSlow9 + fConst3 * fRec11[1];
			float fTemp11 = fTemp1 * (fRec4[0] + fRec5[0] * fTemp4 + fRec6[0] * fTemp7 + fRec7[0] * fTemp10) + fRec8[0] * fTemp3 + fRec9[0] * fTemp6 + fRec10[0] * fTemp9 + fRec11[0] * (1.0f - 2.0f * fTemp2 * (fTemp8 - fTemp10)) + 1.0f;
			fRec12[0] = fSlow10 + fConst3 * fRec12[1];
			float fTemp12 = 0.21428572f * fRec12[0];
			float fTemp13 = fRec13[1] + fConst1 * fRec2[0];
			fRec13[0] = fTemp13 - std::floor(fTemp13);
			float fTemp14 = ftbl0synth_risset_arpSIG0[int(65536.0f * fRec13[0])];
			float fTemp15 = synth_risset_arp_faustpower2_f(fTemp14);
			float fTemp16 = 2.0f * fTemp15 + -1.0f;
			float fTemp17 = 2.0f * fTemp16 + -1.0f;
			float fTemp18 = 1.0f - fTemp17;
			float fTemp19 = 1.0f - 2.0f * fTemp15 * fTemp18;
			float fTemp20 = 2.0f * fTemp19 - fTemp17;
			float fTemp21 = fTemp18 + fTemp20;
			float fTemp22 = 2.0f * fTemp15 * fTemp21 + -1.0f;
			float fTemp23 = 2.0f * fTemp22 - fTemp20;
			float fTemp24 = fTemp14 * (fRec4[0] + fRec5[0] * fTemp17 + fRec6[0] * fTemp20 + fRec7[0] * fTemp23) + fRec8[0] * fTemp16 + fRec9[0] * fTemp19 + fRec10[0] * fTemp22 + fRec11[0] * (1.0f - 2.0f * fTemp15 * (fTemp21 - fTemp23)) + 1.0f;
			float fTemp25 = 0.071428575f * fRec12[0];
			float fTemp26 = fRec14[1] + fConst1 * (fRec2[0] - fRec3[0]);
			fRec14[0] = fTemp26 - std::floor(fTemp26);
			float fTemp27 = ftbl0synth_risset_arpSIG0[int(65536.0f * fRec14[0])];
			float fTemp28 = synth_risset_arp_faustpower2_f(fTemp27);
			float fTemp29 = 2.0f * fTemp28 + -1.0f;
			float fTemp30 = 2.0f * fTemp29 + -1.0f;
			float fTemp31 = 1.0f - fTemp30;
			float fTemp32 = 1.0f - 2.0f * fTemp28 * fTemp31;
			float fTemp33 = 2.0f * fTemp32 - fTemp30;
			float fTemp34 = fTemp31 + fTemp33;
			float fTemp35 = 2.0f * fTemp28 * fTemp34 + -1.0f;
			float fTemp36 = 2.0f * fTemp35 - fTemp33;
			float fTemp37 = fTemp27 * (fRec4[0] + fRec5[0] * fTemp30 + fRec6[0] * fTemp33 + fRec7[0] * fTemp36) + fRec8[0] * fTemp29 + fRec9[0] * fTemp32 + fRec10[0] * fTemp35 + fRec11[0] * (1.0f - 2.0f * fTemp28 * (fTemp34 - fTemp36)) + 1.0f;
			float fTemp38 = 0.071428575f * fRec12[0];
			float fTemp39 = 3.0f * fRec3[0];
			float fTemp40 = fRec15[1] + fConst1 * (fRec2[0] - fTemp39);
			fRec15[0] = fTemp40 - std::floor(fTemp40);
			float fTemp41 = ftbl0synth_risset_arpSIG0[int(65536.0f * fRec15[0])];
			float fTemp42 = synth_risset_arp_faustpower2_f(fTemp41);
			float fTemp43 = 2.0f * fTemp42 + -1.0f;
			float fTemp44 = 2.0f * fTemp43 + -1.0f;
			float fTemp45 = 1.0f - fTemp44;
			float fTemp46 = 1.0f - 2.0f * fTemp42 * fTemp45;
			float fTemp47 = 2.0f * fTemp46 - fTemp44;
			float fTemp48 = fTemp45 + fTemp47;
			float fTemp49 = 2.0f * fTemp42 * fTemp48 + -1.0f;
			float fTemp50 = 2.0f * fTemp49 - fTemp47;
			float fTemp51 = fRec10[0] * fTemp49 + fRec9[0] * fTemp46 + fRec8[0] * fTemp43 + fTemp41 * (fRec4[0] + fRec5[0] * fTemp44 + fRec6[0] * fTemp47 + fRec7[0] * fTemp50) + fRec11[0] * (2.0f * fTemp42 * (fTemp50 - fTemp48) + 1.0f) + 1.0f;
			float fTemp52 = 0.35714287f * fRec12[0];
			float fTemp53 = fRec16[1] + fConst1 * (fRec2[0] - 4.0f * fRec3[0]);
			fRec16[0] = fTemp53 - std::floor(fTemp53);
			float fTemp54 = ftbl0synth_risset_arpSIG0[int(65536.0f * fRec16[0])];
			float fTemp55 = synth_risset_arp_faustpower2_f(fTemp54);
			float fTemp56 = 2.0f * fTemp55 + -1.0f;
			float fTemp57 = 2.0f * fTemp56 + -1.0f;
			float fTemp58 = fTemp57 + -1.0f;
			float fTemp59 = 2.0f * fTemp55 * fTemp58 + 1.0f;
			float fTemp60 = 2.0f * fTemp59 - fTemp57;
			float fTemp61 = fTemp58 - fTemp60;
			float fTemp62 = -1.0f - 2.0f * fTemp55 * fTemp61;
			float fTemp63 = 2.0f * fTemp62 - fTemp60;
			float fTemp64 = fRec10[0] * fTemp62 + fRec8[0] * fTemp56 + fRec9[0] * fTemp59 + fTemp54 * (fRec4[0] + fRec5[0] * fTemp57 + fRec6[0] * fTemp60 + fRec7[0] * fTemp63) + fRec11[0] * (2.0f * fTemp55 * (fTemp61 + fTemp63) + 1.0f) + 1.0f;
			float fTemp65 = 1.0f - fRec12[0];
			float fTemp66 = fRec17[1] + fConst1 * (fRec2[0] + fTemp39);
			fRec17[0] = fTemp66 - std::floor(fTemp66);
			float fTemp67 = ftbl0synth_risset_arpSIG0[int(65536.0f * fRec17[0])];
			float fTemp68 = synth_risset_arp_faustpower2_f(fTemp67);
			float fTemp69 = 2.0f * fTemp68 + -1.0f;
			float fTemp70 = 2.0f * fTemp69 + -1.0f;
			float fTemp71 = 1.0f - fTemp70;
			float fTemp72 = 1.0f - 2.0f * fTemp68 * fTemp71;
			float fTemp73 = 2.0f * fTemp72 - fTemp70;
			float fTemp74 = fTemp71 + fTemp73;
			float fTemp75 = 2.0f * fTemp68 * fTemp74 + -1.0f;
			float fTemp76 = 2.0f * fTemp75 - fTemp73;
			float fTemp77 = fTemp67 * (fRec4[0] + fRec5[0] * fTemp70 + fRec6[0] * fTemp73 + fRec7[0] * fTemp76) + fRec8[0] * fTemp69 + fRec9[0] * fTemp72 + fRec10[0] * fTemp75 + fRec11[0] * (1.0f - 2.0f * fTemp68 * (fTemp74 - fTemp76)) + 1.0f;
			float fTemp78 = fRec12[0] + 1.0f;
			float fTemp79 = 2.0f * fRec3[0];
			float fTemp80 = fRec18[1] + fConst1 * (fRec2[0] - fTemp79);
			fRec18[0] = fTemp80 - std::floor(fTemp80);
			float fTemp81 = ftbl0synth_risset_arpSIG0[int(65536.0f * fRec18[0])];
			float fTemp82 = synth_risset_arp_faustpower2_f(fTemp81);
			float fTemp83 = 2.0f * fTemp82 + -1.0f;
			float fTemp84 = 2.0f * fTemp83 + -1.0f;
			float fTemp85 = 1.0f - fTemp84;
			float fTemp86 = 1.0f - 2.0f * fTemp82 * fTemp85;
			float fTemp87 = 2.0f * fTemp86 - fTemp84;
			float fTemp88 = fTemp85 + fTemp87;
			float fTemp89 = 2.0f * fTemp82 * fTemp88 + -1.0f;
			float fTemp90 = 2.0f * fTemp89 - fTemp87;
			float fTemp91 = fRec10[0] * fTemp89 + fRec9[0] * fTemp86 + fRec8[0] * fTemp83 + fTemp81 * (fRec4[0] + fRec5[0] * fTemp84 + fRec6[0] * fTemp87 + fRec7[0] * fTemp90) + fRec11[0] * (2.0f * fTemp82 * (fTemp90 - fTemp88) + 1.0f) + 1.0f;
			float fTemp92 = 0.21428572f * fRec12[0];
			float fTemp93 = fRec19[1] + fConst1 * (fRec2[0] + fTemp79);
			fRec19[0] = fTemp93 - std::floor(fTemp93);
			float fTemp94 = ftbl0synth_risset_arpSIG0[int(65536.0f * fRec19[0])];
			float fTemp95 = synth_risset_arp_faustpower2_f(fTemp94);
			float fTemp96 = 2.0f * fTemp95 + -1.0f;
			float fTemp97 = 2.0f * fTemp96 + -1.0f;
			float fTemp98 = fTemp97 + -1.0f;
			float fTemp99 = 2.0f * fTemp95 * fTemp98 + 1.0f;
			float fTemp100 = 2.0f * fTemp99 - fTemp97;
			float fTemp101 = fTemp98 - fTemp100;
			float fTemp102 = -1.0f - 2.0f * fTemp95 * fTemp101;
			float fTemp103 = 2.0f * fTemp102 - fTemp100;
			float fTemp104 = fRec8[0] * fTemp96 + fTemp94 * (fRec4[0] + fRec5[0] * fTemp97 + fRec6[0] * fTemp100 + fRec7[0] * fTemp103) + fRec10[0] * fTemp102 + fRec9[0] * fTemp99 + fRec11[0] * (2.0f * fTemp95 * (fTemp101 + fTemp103) + 1.0f) + 1.0f;
			float fTemp105 = 0.35714287f * fRec12[0];
			output0[i0] = FAUSTFLOAT(0.05f * (fTemp11 * (fTemp12 + 0.5f) + fTemp24 * (fTemp25 + 0.5f) + fTemp37 * (0.5f - fTemp38) + fTemp51 * (0.5f - fTemp52) + 0.5f * (fTemp64 * fTemp65 + fTemp77 * fTemp78) + fTemp91 * (0.5f - fTemp92) + fTemp104 * (fTemp105 + 0.5f)));
			output1[i0] = FAUSTFLOAT(0.05f * (fTemp64 * (1.0f - 0.5f * fTemp65) + fTemp51 * (fTemp52 + 0.5f) + fTemp91 * (fTemp92 + 0.5f) + fTemp37 * (fTemp38 + 0.5f) + fTemp24 * (0.5f - fTemp25) + fTemp11 * (0.5f - fTemp12) + fTemp104 * (0.5f - fTemp105) + fTemp77 * (1.0f - 0.5f * fTemp78)));
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec1[1] = fRec1[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _synth_risset_arp_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_risset_arp_UI<T>::name(sym(synth_risset_arp));

typedef _synth_risset_arp_UI<synth_risset_arp> synth_risset_arp_UI;

class faust_synth_risset_arp_tilde : public FaustExternal<synth_risset_arp, synth_risset_arp_UI> {
public:
    faust_synth_risset_arp_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
