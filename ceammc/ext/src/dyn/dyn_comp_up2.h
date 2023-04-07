/* ------------------------------------------------------------
name: "dyn.comp_up2"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn dyn_comp_up2 -scn dyn_comp_up2_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __dyn_comp_up2_H__
#define  __dyn_comp_up2_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN dyn_comp_up2_dsp.h ********************************
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

class FAUST_API dyn_comp_up2_dsp {

    public:

        dyn_comp_up2_dsp() {}
        virtual ~dyn_comp_up2_dsp() {}

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
        virtual dyn_comp_up2_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public dyn_comp_up2_dsp {

    protected:

        dyn_comp_up2_dsp* fDSP;

    public:

        decorator_dsp(dyn_comp_up2_dsp* dyn_comp_up2_dsp = nullptr):fDSP(dyn_comp_up2_dsp) {}
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
    
        virtual dyn_comp_up2_dsp* createDSPInstance() = 0;
    
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

/************************** END dyn_comp_up2_dsp.h **************************/
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
 The base class of Meta handler to be used in dyn_comp_up2_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct dyn_comp_up2 : public dyn_comp_up2_dsp {
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
#define FAUSTCLASS dyn_comp_up2
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

static float dyn_comp_up2_faustpower2_f(float value) {
	return value * value;
}

class dyn_comp_up2 : public dyn_comp_up2_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	float fRec0[2];
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec2[2];
	FAUSTFLOAT fHslider1;
	float fRec3[2];
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fVec0[2];
	float fConst4;
	float fVec1[3];
	float fConst5;
	float fVec2[7];
	float fConst6;
	float fVec3[15];
	float fConst7;
	int IOTA0;
	float fVec4[32];
	float fConst8;
	float fVec5[64];
	float fConst9;
	float fVec6[128];
	float fConst10;
	float fVec7[256];
	float fConst11;
	float fVec8[512];
	float fConst12;
	float fVec9[1024];
	float fConst13;
	float fVec10[2048];
	float fConst14;
	float fVec11[4096];
	float fConst15;
	float fVec12[8192];
	float fConst16;
	float fVec13[16384];
	float fConst17;
	float fVec14[32768];
	float fConst18;
	float fVec15[65536];
	float fConst19;
	float fVec16[131072];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fHslider6;
	float fConst20;
	float fRec1[2];
	FAUSTFLOAT fHslider7;
	float fVec17[2];
	float fVec18[3];
	float fVec19[7];
	float fVec20[15];
	float fVec21[32];
	float fVec22[64];
	float fVec23[128];
	float fVec24[256];
	float fVec25[512];
	float fVec26[1024];
	float fVec27[2048];
	float fVec28[4096];
	float fVec29[8192];
	float fVec30[16384];
	float fVec31[32768];
	float fVec32[65536];
	float fVec33[131072];
	float fRec4[2];
	FAUSTFLOAT fHbargraph0;
	FAUSTFLOAT fHbargraph1;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/parallelMax:author", "Bart Brouns");
		m->declare("basics.lib/parallelMax:copyright", "Copyright (c) 2020 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/parallelMax:licence", "GPL-3.0");
		m->declare("basics.lib/parallelOp:author", "Bart Brouns");
		m->declare("basics.lib/parallelOp:copyright", "Copyright (c) 2020 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/parallelOp:licence", "GPL-3.0");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.3");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn dyn_comp_up2 -scn dyn_comp_up2_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("compressors.lib/expanderSC_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/expanderSC_N_chan:license", "GPLv3");
		m->declare("compressors.lib/expander_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/expander_N_chan:license", "GPLv3");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_expansion_gain_N_chan_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_expansion_gain_N_chan_db:license", "GPLv3");
		m->declare("compressors.lib/version", "0.4");
		m->declare("filename", "dyn_comp_up2.dsp");
		m->declare("interpolators.lib/interpolate_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_linear:licence", "MIT");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "dyn.comp_up2");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
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
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 0.001f * fConst0;
		fConst4 = 0.0005f * fConst0;
		fConst5 = 0.00025f * fConst0;
		fConst6 = 0.000125f * fConst0;
		fConst7 = 6.25e-05f * fConst0;
		fConst8 = 3.125e-05f * fConst0;
		fConst9 = 1.5625e-05f * fConst0;
		fConst10 = 7.8125e-06f * fConst0;
		fConst11 = 3.90625e-06f * fConst0;
		fConst12 = 1.953125e-06f * fConst0;
		fConst13 = 9.765625e-07f * fConst0;
		fConst14 = 4.882813e-07f * fConst0;
		fConst15 = 2.4414064e-07f * fConst0;
		fConst16 = 1.2207032e-07f * fConst0;
		fConst17 = 6.103516e-08f * fConst0;
		fConst18 = 3.051758e-08f * fConst0;
		fConst19 = 1.525879e-08f * fConst0;
		fConst20 = 1.0f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(12.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(1e+02f);
		fHslider3 = FAUSTFLOAT(-32.0f);
		fHslider4 = FAUSTFLOAT(3.0f);
		fHslider5 = FAUSTFLOAT(1e+01f);
		fHslider6 = FAUSTFLOAT(5e+01f);
		fHslider7 = FAUSTFLOAT(1.0f);
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
			fVec0[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 7; l5 = l5 + 1) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 15; l6 = l6 + 1) {
			fVec3[l6] = 0.0f;
		}
		IOTA0 = 0;
		for (int l7 = 0; l7 < 32; l7 = l7 + 1) {
			fVec4[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 64; l8 = l8 + 1) {
			fVec5[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 128; l9 = l9 + 1) {
			fVec6[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 256; l10 = l10 + 1) {
			fVec7[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 512; l11 = l11 + 1) {
			fVec8[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 1024; l12 = l12 + 1) {
			fVec9[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2048; l13 = l13 + 1) {
			fVec10[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 4096; l14 = l14 + 1) {
			fVec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 8192; l15 = l15 + 1) {
			fVec12[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 16384; l16 = l16 + 1) {
			fVec13[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 32768; l17 = l17 + 1) {
			fVec14[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 65536; l18 = l18 + 1) {
			fVec15[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 131072; l19 = l19 + 1) {
			fVec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec1[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fVec17[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fVec18[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 7; l23 = l23 + 1) {
			fVec19[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 15; l24 = l24 + 1) {
			fVec20[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 32; l25 = l25 + 1) {
			fVec21[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 64; l26 = l26 + 1) {
			fVec22[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 128; l27 = l27 + 1) {
			fVec23[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 256; l28 = l28 + 1) {
			fVec24[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 512; l29 = l29 + 1) {
			fVec25[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 1024; l30 = l30 + 1) {
			fVec26[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2048; l31 = l31 + 1) {
			fVec27[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 4096; l32 = l32 + 1) {
			fVec28[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 8192; l33 = l33 + 1) {
			fVec29[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 16384; l34 = l34 + 1) {
			fVec30[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 32768; l35 = l35 + 1) {
			fVec31[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 65536; l36 = l36 + 1) {
			fVec32[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 131072; l37 = l37 + 1) {
			fVec33[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec4[l38] = 0.0f;
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
	
	virtual dyn_comp_up2* clone() {
		return new dyn_comp_up2();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("dyn.comp_up2");
		ui_interface->declare(&fHslider5, "unit", "ms");
		ui_interface->addHorizontalSlider("attack", &fHslider5, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.1f));
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->declare(&fHslider2, "unit", "ms");
		ui_interface->addHorizontalSlider("hold", &fHslider2, FAUSTFLOAT(1e+02f), FAUSTFLOAT(1.0f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider4, "unit", "db");
		ui_interface->addHorizontalSlider("knee", &fHslider4, FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHbargraph0, "unit", "db");
		ui_interface->addHorizontalBargraph("level", &fHbargraph0, FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f));
		ui_interface->declare(&fHbargraph1, "unit", "db");
		ui_interface->addHorizontalBargraph("level", &fHbargraph1, FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f));
		ui_interface->addHorizontalSlider("link", &fHslider7, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider0, "unit", "db");
		ui_interface->addHorizontalSlider("range", &fHslider0, FAUSTFLOAT(12.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(32.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider6, "unit", "ms");
		ui_interface->addHorizontalSlider("release", &fHslider6, FAUSTFLOAT(5e+01f), FAUSTFLOAT(1.0f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("strength", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "unit", "db");
		ui_interface->addHorizontalSlider("threshold", &fHslider3, FAUSTFLOAT(-32.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(float(fCheckbox0) >= 1.0f);
		float fSlow1 = fConst1 * (0.0f - float(fHslider0));
		float fSlow2 = fConst1 * (0.0f - float(fHslider1));
		float fSlow3 = float(fHslider2);
		int iSlow4 = int(std::floor(fConst3 * fSlow3)) % 2;
		int iSlow5 = int(std::floor(fConst4 * fSlow3)) % 2;
		int iSlow6 = int(std::floor(fConst5 * fSlow3)) % 2;
		int iSlow7 = 2 * iSlow5;
		int iSlow8 = iSlow4 + iSlow7;
		int iSlow9 = int(std::floor(fConst6 * fSlow3)) % 2;
		int iSlow10 = 4 * iSlow6;
		int iSlow11 = iSlow8 + iSlow10;
		int iSlow12 = int(std::floor(fConst7 * fSlow3)) % 2;
		int iSlow13 = 8 * iSlow9;
		int iSlow14 = iSlow11 + iSlow13;
		int iSlow15 = int(std::floor(fConst8 * fSlow3)) % 2;
		int iSlow16 = 16 * iSlow12;
		int iSlow17 = iSlow14 + iSlow16;
		int iSlow18 = int(std::floor(fConst9 * fSlow3)) % 2;
		int iSlow19 = 32 * iSlow15;
		int iSlow20 = iSlow17 + iSlow19;
		int iSlow21 = int(std::floor(fConst10 * fSlow3)) % 2;
		int iSlow22 = 64 * iSlow18;
		int iSlow23 = iSlow20 + iSlow22;
		int iSlow24 = int(std::floor(fConst11 * fSlow3)) % 2;
		int iSlow25 = 128 * iSlow21;
		int iSlow26 = iSlow23 + iSlow25;
		int iSlow27 = int(std::floor(fConst12 * fSlow3)) % 2;
		int iSlow28 = 256 * iSlow24;
		int iSlow29 = iSlow25 + iSlow22 + iSlow19 + iSlow16 + iSlow13 + iSlow10 + iSlow7 + iSlow4 + iSlow28;
		int iSlow30 = int(std::floor(fConst13 * fSlow3)) % 2;
		int iSlow31 = iSlow28 + 512 * iSlow27;
		int iSlow32 = iSlow25 + iSlow22 + iSlow19 + iSlow16 + iSlow13 + iSlow10 + iSlow7 + iSlow4 + iSlow31;
		int iSlow33 = int(std::floor(fConst14 * fSlow3)) % 2;
		int iSlow34 = iSlow31 + 1024 * iSlow30;
		int iSlow35 = iSlow25 + iSlow22 + iSlow19 + iSlow16 + iSlow13 + iSlow10 + iSlow7 + iSlow4 + iSlow34;
		int iSlow36 = int(std::floor(fConst15 * fSlow3)) % 2;
		int iSlow37 = iSlow34 + 2048 * iSlow33;
		int iSlow38 = iSlow25 + iSlow22 + iSlow19 + iSlow16 + iSlow13 + iSlow10 + iSlow7 + iSlow4 + iSlow37;
		int iSlow39 = int(std::floor(fConst16 * fSlow3)) % 2;
		int iSlow40 = iSlow37 + 4096 * iSlow36;
		int iSlow41 = iSlow25 + iSlow22 + iSlow19 + iSlow16 + iSlow13 + iSlow10 + iSlow7 + iSlow4 + iSlow40;
		int iSlow42 = int(std::floor(fConst17 * fSlow3)) % 2;
		int iSlow43 = iSlow40 + 8192 * iSlow39;
		int iSlow44 = iSlow25 + iSlow22 + iSlow19 + iSlow16 + iSlow13 + iSlow10 + iSlow7 + iSlow4 + iSlow43;
		int iSlow45 = int(std::floor(fConst18 * fSlow3)) % 2;
		int iSlow46 = iSlow43 + 16384 * iSlow42;
		int iSlow47 = iSlow25 + iSlow22 + iSlow19 + iSlow16 + iSlow13 + iSlow10 + iSlow7 + iSlow4 + iSlow46;
		int iSlow48 = int(std::floor(fConst19 * fSlow3)) % 2;
		int iSlow49 = iSlow25 + iSlow22 + iSlow19 + iSlow16 + iSlow13 + iSlow10 + iSlow7 + iSlow4 + iSlow46 + 32768 * iSlow45;
		float fSlow50 = float(fHslider3);
		float fSlow51 = float(fHslider4);
		float fSlow52 = 0.5f * fSlow51;
		float fSlow53 = fSlow50 - fSlow52;
		float fSlow54 = fSlow50 + fSlow52;
		float fSlow55 = 1.0f / std::min<float>(1.1920929e-07f, 0.0f - 2.0f * fSlow51);
		float fSlow56 = 0.001f * float(fHslider5);
		float fSlow57 = 0.001f * float(fHslider6);
		float fSlow58 = float(fHslider7);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			float fTemp1 = fRec0[1] + 0.015625f;
			float fTemp2 = fRec0[1] + -0.015625f;
			fRec0[0] = ((fTemp1 < fSlow0) ? fTemp1 : ((fTemp2 > fSlow0) ? fTemp2 : fSlow0));
			float fTemp3 = 1.0f - fRec0[0];
			fRec2[0] = fSlow1 + fConst2 * fRec2[1];
			fRec3[0] = fSlow2 + fConst2 * fRec3[1];
			float fTemp4 = std::fabs(fRec3[0]);
			float fTemp5 = std::fabs(fTemp0);
			fVec0[0] = fTemp5;
			float fTemp6 = std::max<float>(fTemp5, fVec0[1]);
			fVec1[0] = fTemp6;
			float fTemp7 = std::max<float>(fTemp6, fVec1[2]);
			fVec2[0] = fTemp7;
			float fTemp8 = std::max<float>(fTemp7, fVec2[4]);
			fVec3[0] = fTemp8;
			float fTemp9 = std::max<float>(fTemp8, fVec3[8]);
			fVec4[IOTA0 & 31] = fTemp9;
			float fTemp10 = std::max<float>(fTemp9, fVec4[(IOTA0 - 16) & 31]);
			fVec5[IOTA0 & 63] = fTemp10;
			float fTemp11 = std::max<float>(fTemp10, fVec5[(IOTA0 - 32) & 63]);
			fVec6[IOTA0 & 127] = fTemp11;
			float fTemp12 = std::max<float>(fTemp11, fVec6[(IOTA0 - 64) & 127]);
			fVec7[IOTA0 & 255] = fTemp12;
			float fTemp13 = std::max<float>(fTemp12, fVec7[(IOTA0 - 128) & 255]);
			fVec8[IOTA0 & 511] = fTemp13;
			float fTemp14 = std::max<float>(fTemp13, fVec8[(IOTA0 - 256) & 511]);
			fVec9[IOTA0 & 1023] = fTemp14;
			float fTemp15 = std::max<float>(fTemp14, fVec9[(IOTA0 - 512) & 1023]);
			fVec10[IOTA0 & 2047] = fTemp15;
			float fTemp16 = std::max<float>(fTemp15, fVec10[(IOTA0 - 1024) & 2047]);
			fVec11[IOTA0 & 4095] = fTemp16;
			float fTemp17 = std::max<float>(fTemp16, fVec11[(IOTA0 - 2048) & 4095]);
			fVec12[IOTA0 & 8191] = fTemp17;
			float fTemp18 = std::max<float>(fTemp17, fVec12[(IOTA0 - 4096) & 8191]);
			fVec13[IOTA0 & 16383] = fTemp18;
			float fTemp19 = std::max<float>(fTemp18, fVec13[(IOTA0 - 8192) & 16383]);
			fVec14[IOTA0 & 32767] = fTemp19;
			float fTemp20 = std::max<float>(fTemp19, fVec14[(IOTA0 - 16384) & 32767]);
			fVec15[IOTA0 & 65535] = fTemp20;
			fVec16[IOTA0 & 131071] = std::max<float>(fTemp20, fVec15[(IOTA0 - 32768) & 65535]);
			float fTemp21 = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(((iSlow4) ? fTemp5 : -3.4028235e+38f), ((iSlow5) ? fVec1[iSlow4] : -3.4028235e+38f)), ((iSlow6) ? fVec2[iSlow8] : -3.4028235e+38f)), ((iSlow9) ? fVec3[iSlow11] : -3.4028235e+38f)), ((iSlow12) ? fVec4[(IOTA0 - iSlow14) & 31] : -3.4028235e+38f)), ((iSlow15) ? fVec5[(IOTA0 - iSlow17) & 63] : -3.4028235e+38f)), ((iSlow18) ? fVec6[(IOTA0 - iSlow20) & 127] : -3.4028235e+38f)), ((iSlow21) ? fVec7[(IOTA0 - iSlow23) & 255] : -3.4028235e+38f)), ((iSlow24) ? fVec8[(IOTA0 - iSlow26) & 511] : -3.4028235e+38f)), ((iSlow27) ? fVec9[(IOTA0 - iSlow29) & 1023] : -3.4028235e+38f)), ((iSlow30) ? fVec10[(IOTA0 - iSlow32) & 2047] : -3.4028235e+38f)), ((iSlow33) ? fVec11[(IOTA0 - iSlow35) & 4095] : -3.4028235e+38f)), ((iSlow36) ? fVec12[(IOTA0 - iSlow38) & 8191] : -3.4028235e+38f)), ((iSlow39) ? fVec13[(IOTA0 - iSlow41) & 16383] : -3.4028235e+38f)), ((iSlow42) ? fVec14[(IOTA0 - iSlow44) & 32767] : -3.4028235e+38f)), ((iSlow45) ? fVec15[(IOTA0 - iSlow47) & 65535] : -3.4028235e+38f)), ((iSlow48) ? fVec16[(IOTA0 - iSlow49) & 131071] : -3.4028235e+38f))));
			int iTemp22 = (fTemp21 > fSlow53) + (fTemp21 > fSlow54);
			int iTemp23 = fRec3[0] > 0.0f;
			float fTemp24 = float(2 * iTemp23 + -1);
			float fTemp25 = std::max<float>(fRec2[0], fTemp4 * ((iTemp22 == 0) ? fTemp21 - fSlow50 : ((iTemp22 == 1) ? fSlow55 * dyn_comp_up2_faustpower2_f(fTemp21 - fSlow54) : 0.0f))) * fTemp24;
			float fTemp26 = ((iTemp23) ? fSlow57 : fSlow56);
			int iTemp27 = std::fabs(fTemp26) < 1.1920929e-07f;
			float fTemp28 = ((iTemp27) ? 0.0f : std::exp(0.0f - fConst20 / ((iTemp27) ? 1.0f : fTemp26)));
			float fTemp29 = ((iTemp23) ? fSlow56 : fSlow57);
			int iTemp30 = std::fabs(fTemp29) < 1.1920929e-07f;
			float fTemp31 = ((iTemp30) ? 0.0f : std::exp(0.0f - fConst20 / ((iTemp30) ? 1.0f : fTemp29)));
			float fTemp32 = ((fTemp25 > fRec1[1]) ? fTemp31 : fTemp28);
			fRec1[0] = fTemp25 * (1.0f - fTemp32) + fRec1[1] * fTemp32;
			float fTemp33 = float(input1[i0]);
			float fTemp34 = std::fabs(fTemp33);
			fVec17[0] = fTemp34;
			float fTemp35 = std::max<float>(fTemp34, fVec17[1]);
			fVec18[0] = fTemp35;
			float fTemp36 = std::max<float>(fTemp35, fVec18[2]);
			fVec19[0] = fTemp36;
			float fTemp37 = std::max<float>(fTemp36, fVec19[4]);
			fVec20[0] = fTemp37;
			float fTemp38 = std::max<float>(fTemp37, fVec20[8]);
			fVec21[IOTA0 & 31] = fTemp38;
			float fTemp39 = std::max<float>(fTemp38, fVec21[(IOTA0 - 16) & 31]);
			fVec22[IOTA0 & 63] = fTemp39;
			float fTemp40 = std::max<float>(fTemp39, fVec22[(IOTA0 - 32) & 63]);
			fVec23[IOTA0 & 127] = fTemp40;
			float fTemp41 = std::max<float>(fTemp40, fVec23[(IOTA0 - 64) & 127]);
			fVec24[IOTA0 & 255] = fTemp41;
			float fTemp42 = std::max<float>(fTemp41, fVec24[(IOTA0 - 128) & 255]);
			fVec25[IOTA0 & 511] = fTemp42;
			float fTemp43 = std::max<float>(fTemp42, fVec25[(IOTA0 - 256) & 511]);
			fVec26[IOTA0 & 1023] = fTemp43;
			float fTemp44 = std::max<float>(fTemp43, fVec26[(IOTA0 - 512) & 1023]);
			fVec27[IOTA0 & 2047] = fTemp44;
			float fTemp45 = std::max<float>(fTemp44, fVec27[(IOTA0 - 1024) & 2047]);
			fVec28[IOTA0 & 4095] = fTemp45;
			float fTemp46 = std::max<float>(fTemp45, fVec28[(IOTA0 - 2048) & 4095]);
			fVec29[IOTA0 & 8191] = fTemp46;
			float fTemp47 = std::max<float>(fTemp46, fVec29[(IOTA0 - 4096) & 8191]);
			fVec30[IOTA0 & 16383] = fTemp47;
			float fTemp48 = std::max<float>(fTemp47, fVec30[(IOTA0 - 8192) & 16383]);
			fVec31[IOTA0 & 32767] = fTemp48;
			float fTemp49 = std::max<float>(fTemp48, fVec31[(IOTA0 - 16384) & 32767]);
			fVec32[IOTA0 & 65535] = fTemp49;
			fVec33[IOTA0 & 131071] = std::max<float>(fTemp49, fVec32[(IOTA0 - 32768) & 65535]);
			float fTemp50 = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(((iSlow4) ? fTemp34 : -3.4028235e+38f), ((iSlow5) ? fVec18[iSlow4] : -3.4028235e+38f)), ((iSlow6) ? fVec19[iSlow8] : -3.4028235e+38f)), ((iSlow9) ? fVec20[iSlow11] : -3.4028235e+38f)), ((iSlow12) ? fVec21[(IOTA0 - iSlow14) & 31] : -3.4028235e+38f)), ((iSlow15) ? fVec22[(IOTA0 - iSlow17) & 63] : -3.4028235e+38f)), ((iSlow18) ? fVec23[(IOTA0 - iSlow20) & 127] : -3.4028235e+38f)), ((iSlow21) ? fVec24[(IOTA0 - iSlow23) & 255] : -3.4028235e+38f)), ((iSlow24) ? fVec25[(IOTA0 - iSlow26) & 511] : -3.4028235e+38f)), ((iSlow27) ? fVec26[(IOTA0 - iSlow29) & 1023] : -3.4028235e+38f)), ((iSlow30) ? fVec27[(IOTA0 - iSlow32) & 2047] : -3.4028235e+38f)), ((iSlow33) ? fVec28[(IOTA0 - iSlow35) & 4095] : -3.4028235e+38f)), ((iSlow36) ? fVec29[(IOTA0 - iSlow38) & 8191] : -3.4028235e+38f)), ((iSlow39) ? fVec30[(IOTA0 - iSlow41) & 16383] : -3.4028235e+38f)), ((iSlow42) ? fVec31[(IOTA0 - iSlow44) & 32767] : -3.4028235e+38f)), ((iSlow45) ? fVec32[(IOTA0 - iSlow47) & 65535] : -3.4028235e+38f)), ((iSlow48) ? fVec33[(IOTA0 - iSlow49) & 131071] : -3.4028235e+38f))));
			int iTemp51 = (fTemp50 > fSlow53) + (fTemp50 > fSlow54);
			float fTemp52 = fTemp24 * std::max<float>(fRec2[0], fTemp4 * ((iTemp51 == 0) ? fTemp50 - fSlow50 : ((iTemp51 == 1) ? fSlow55 * dyn_comp_up2_faustpower2_f(fTemp50 - fSlow54) : 0.0f)));
			float fTemp53 = ((fTemp52 > fRec4[1]) ? fTemp31 : fTemp28);
			fRec4[0] = fTemp52 * (1.0f - fTemp53) + fRec4[1] * fTemp53;
			float fTemp54 = std::max<float>(fRec1[0], fRec4[0]);
			float fTemp55 = fRec1[0] + fSlow58 * (fTemp54 - fRec1[0]);
			fHbargraph0 = FAUSTFLOAT(fTemp55);
			output0[i0] = FAUSTFLOAT(fTemp0 * (fRec0[0] + fTemp3 * std::pow(1e+01f, 0.05f * fTemp55)));
			float fTemp56 = fRec4[0] + fSlow58 * (fTemp54 - fRec4[0]);
			fHbargraph1 = FAUSTFLOAT(fTemp56);
			output1[i0] = FAUSTFLOAT(fTemp33 * (fRec0[0] + fTemp3 * std::pow(1e+01f, 0.05f * fTemp56)));
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fVec0[1] = fVec0[0];
			fVec1[2] = fVec1[1];
			fVec1[1] = fVec1[0];
			for (int j0 = 6; j0 > 0; j0 = j0 - 1) {
				fVec2[j0] = fVec2[j0 - 1];
			}
			for (int j1 = 14; j1 > 0; j1 = j1 - 1) {
				fVec3[j1] = fVec3[j1 - 1];
			}
			IOTA0 = IOTA0 + 1;
			fRec1[1] = fRec1[0];
			fVec17[1] = fVec17[0];
			fVec18[2] = fVec18[1];
			fVec18[1] = fVec18[0];
			for (int j2 = 6; j2 > 0; j2 = j2 - 1) {
				fVec19[j2] = fVec19[j2 - 1];
			}
			for (int j3 = 14; j3 > 0; j3 = j3 - 1) {
				fVec20[j3] = fVec20[j3 - 1];
			}
			fRec4[1] = fRec4[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _dyn_comp_up2_UI : public UI {
    static std::string name;
};

template <class T>
std::string _dyn_comp_up2_UI<T>::name(sym(dyn_comp_up2));

typedef _dyn_comp_up2_UI<dyn_comp_up2> dyn_comp_up2_UI;

class faust_dyn_comp_up2_tilde : public FaustExternal<dyn_comp_up2, dyn_comp_up2_UI> {
public:
    faust_dyn_comp_up2_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
