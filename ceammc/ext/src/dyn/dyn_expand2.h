/* ------------------------------------------------------------
name: "dyn.expand2"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn dyn_expand2 -scn dyn_expand2_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __dyn_expand2_H__
#define  __dyn_expand2_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN dyn_expand2_dsp.h ********************************
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

class FAUST_API dyn_expand2_dsp {

    public:

        dyn_expand2_dsp() {}
        virtual ~dyn_expand2_dsp() {}

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
        virtual dyn_expand2_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public dyn_expand2_dsp {

    protected:

        dyn_expand2_dsp* fDSP;

    public:

        decorator_dsp(dyn_expand2_dsp* dyn_expand2_dsp = nullptr):fDSP(dyn_expand2_dsp) {}
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
    
        virtual dyn_expand2_dsp* createDSPInstance() = 0;
    
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

/************************** END dyn_expand2_dsp.h **************************/
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
 The base class of Meta handler to be used in dyn_expand2_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct dyn_expand2 : public dyn_expand2_dsp {
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
#define FAUSTCLASS dyn_expand2
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

static float dyn_expand2_faustpower2_f(float value) {
	return value * value;
}

class dyn_expand2 : public dyn_expand2_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	float fRec0[2];
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider1;
	float fConst2;
	float fRec2[2];
	FAUSTFLOAT fHslider2;
	float fVec0[2];
	float fVec1[3];
	float fVec2[7];
	float fVec3[15];
	int IOTA0;
	float fVec4[32];
	float fVec5[64];
	float fVec6[128];
	float fVec7[256];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fHslider6;
	float fConst3;
	float fRec1[2];
	FAUSTFLOAT fHslider7;
	float fVec8[2];
	float fVec9[3];
	float fVec10[7];
	float fVec11[15];
	float fVec12[32];
	float fVec13[64];
	float fVec14[128];
	float fVec15[256];
	float fRec3[2];
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
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn dyn_expand2 -scn dyn_expand2_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("compressors.lib/expanderSC_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/expanderSC_N_chan:license", "GPLv3");
		m->declare("compressors.lib/expander_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/expander_N_chan:license", "GPLv3");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_expansion_gain_N_chan_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_expansion_gain_N_chan_db:license", "GPLv3");
		m->declare("compressors.lib/version", "0.4");
		m->declare("filename", "dyn_expand2.dsp");
		m->declare("interpolators.lib/interpolate_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_linear:licence", "MIT");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "dyn.expand2");
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
		fConst3 = 1.0f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(-1.2e+02f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(128.0f);
		fHslider3 = FAUSTFLOAT(-48.0f);
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
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 7; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 15; l5 = l5 + 1) {
			fVec3[l5] = 0.0f;
		}
		IOTA0 = 0;
		for (int l6 = 0; l6 < 32; l6 = l6 + 1) {
			fVec4[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 64; l7 = l7 + 1) {
			fVec5[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 128; l8 = l8 + 1) {
			fVec6[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 256; l9 = l9 + 1) {
			fVec7[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec1[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fVec8[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fVec9[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 7; l13 = l13 + 1) {
			fVec10[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 15; l14 = l14 + 1) {
			fVec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 32; l15 = l15 + 1) {
			fVec12[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 64; l16 = l16 + 1) {
			fVec13[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 128; l17 = l17 + 1) {
			fVec14[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 256; l18 = l18 + 1) {
			fVec15[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec3[l19] = 0.0f;
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
	
	virtual dyn_expand2* clone() {
		return new dyn_expand2();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("dyn.expand2");
		ui_interface->declare(&fHslider5, "unit", "ms");
		ui_interface->addHorizontalSlider("attack", &fHslider5, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.1f));
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->declare(&fHslider2, "unit", "samp");
		ui_interface->addHorizontalSlider("hold", &fHslider2, FAUSTFLOAT(128.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(255.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider4, "unit", "db");
		ui_interface->addHorizontalSlider("knee", &fHslider4, FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHbargraph0, "unit", "db");
		ui_interface->addHorizontalBargraph("level", &fHbargraph0, FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f));
		ui_interface->declare(&fHbargraph1, "unit", "db");
		ui_interface->addHorizontalBargraph("level", &fHbargraph1, FAUSTFLOAT(0.0f), FAUSTFLOAT(9e+01f));
		ui_interface->addHorizontalSlider("link", &fHslider7, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider0, "unit", "db");
		ui_interface->addHorizontalSlider("range", &fHslider0, FAUSTFLOAT(-1.2e+02f), FAUSTFLOAT(-1.2e+02f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider6, "unit", "ms");
		ui_interface->addHorizontalSlider("release", &fHslider6, FAUSTFLOAT(5e+01f), FAUSTFLOAT(1.0f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("strength", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "unit", "db");
		ui_interface->addHorizontalSlider("threshold", &fHslider3, FAUSTFLOAT(-48.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(float(fCheckbox0) >= 1.0f);
		float fSlow1 = float(fHslider0);
		float fSlow2 = fConst1 * float(fHslider1);
		float fSlow3 = float(fHslider2);
		int iSlow4 = int(std::floor(fSlow3)) % 2;
		int iSlow5 = int(std::floor(0.5f * fSlow3)) % 2;
		int iSlow6 = int(std::floor(0.25f * fSlow3)) % 2;
		int iSlow7 = iSlow4 + 2 * iSlow5;
		int iSlow8 = int(std::floor(0.125f * fSlow3)) % 2;
		int iSlow9 = iSlow7 + 4 * iSlow6;
		int iSlow10 = int(std::floor(0.0625f * fSlow3)) % 2;
		int iSlow11 = iSlow9 + 8 * iSlow8;
		int iSlow12 = int(std::floor(0.03125f * fSlow3)) % 2;
		int iSlow13 = iSlow11 + 16 * iSlow10;
		int iSlow14 = int(std::floor(0.015625f * fSlow3)) % 2;
		int iSlow15 = iSlow13 + 32 * iSlow12;
		int iSlow16 = int(std::floor(0.0078125f * fSlow3)) % 2;
		int iSlow17 = iSlow15 + 64 * iSlow14;
		float fSlow18 = float(fHslider3);
		float fSlow19 = float(fHslider4);
		float fSlow20 = 0.5f * fSlow19;
		float fSlow21 = fSlow18 - fSlow20;
		float fSlow22 = fSlow18 + fSlow20;
		float fSlow23 = 1.0f / std::min<float>(1.1920929e-07f, 0.0f - 2.0f * fSlow19);
		float fSlow24 = 0.001f * float(fHslider5);
		float fSlow25 = 0.001f * float(fHslider6);
		float fSlow26 = float(fHslider7);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			float fTemp1 = fRec0[1] + 0.015625f;
			float fTemp2 = fRec0[1] + -0.015625f;
			fRec0[0] = ((fTemp1 < fSlow0) ? fTemp1 : ((fTemp2 > fSlow0) ? fTemp2 : fSlow0));
			float fTemp3 = 1.0f - fRec0[0];
			fRec2[0] = fSlow2 + fConst2 * fRec2[1];
			float fTemp4 = std::fabs(fRec2[0]);
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
			fVec7[IOTA0 & 255] = std::max<float>(fTemp11, fVec6[(IOTA0 - 64) & 127]);
			float fTemp12 = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(((iSlow4) ? fTemp5 : -3.4028235e+38f), ((iSlow5) ? fVec1[iSlow4] : -3.4028235e+38f)), ((iSlow6) ? fVec2[iSlow7] : -3.4028235e+38f)), ((iSlow8) ? fVec3[iSlow9] : -3.4028235e+38f)), ((iSlow10) ? fVec4[(IOTA0 - iSlow11) & 31] : -3.4028235e+38f)), ((iSlow12) ? fVec5[(IOTA0 - iSlow13) & 63] : -3.4028235e+38f)), ((iSlow14) ? fVec6[(IOTA0 - iSlow15) & 127] : -3.4028235e+38f)), ((iSlow16) ? fVec7[(IOTA0 - iSlow17) & 255] : -3.4028235e+38f))));
			int iTemp13 = (fTemp12 > fSlow21) + (fTemp12 > fSlow22);
			int iTemp14 = fRec2[0] > 0.0f;
			float fTemp15 = float(2 * iTemp14 + -1);
			float fTemp16 = std::max<float>(fSlow1, fTemp4 * ((iTemp13 == 0) ? fTemp12 - fSlow18 : ((iTemp13 == 1) ? fSlow23 * dyn_expand2_faustpower2_f(fTemp12 - fSlow22) : 0.0f))) * fTemp15;
			float fTemp17 = ((iTemp14) ? fSlow25 : fSlow24);
			int iTemp18 = std::fabs(fTemp17) < 1.1920929e-07f;
			float fTemp19 = ((iTemp18) ? 0.0f : std::exp(0.0f - fConst3 / ((iTemp18) ? 1.0f : fTemp17)));
			float fTemp20 = ((iTemp14) ? fSlow24 : fSlow25);
			int iTemp21 = std::fabs(fTemp20) < 1.1920929e-07f;
			float fTemp22 = ((iTemp21) ? 0.0f : std::exp(0.0f - fConst3 / ((iTemp21) ? 1.0f : fTemp20)));
			float fTemp23 = ((fTemp16 > fRec1[1]) ? fTemp22 : fTemp19);
			fRec1[0] = fTemp16 * (1.0f - fTemp23) + fRec1[1] * fTemp23;
			float fTemp24 = float(input1[i0]);
			float fTemp25 = std::fabs(fTemp24);
			fVec8[0] = fTemp25;
			float fTemp26 = std::max<float>(fTemp25, fVec8[1]);
			fVec9[0] = fTemp26;
			float fTemp27 = std::max<float>(fTemp26, fVec9[2]);
			fVec10[0] = fTemp27;
			float fTemp28 = std::max<float>(fTemp27, fVec10[4]);
			fVec11[0] = fTemp28;
			float fTemp29 = std::max<float>(fTemp28, fVec11[8]);
			fVec12[IOTA0 & 31] = fTemp29;
			float fTemp30 = std::max<float>(fTemp29, fVec12[(IOTA0 - 16) & 31]);
			fVec13[IOTA0 & 63] = fTemp30;
			float fTemp31 = std::max<float>(fTemp30, fVec13[(IOTA0 - 32) & 63]);
			fVec14[IOTA0 & 127] = fTemp31;
			fVec15[IOTA0 & 255] = std::max<float>(fTemp31, fVec14[(IOTA0 - 64) & 127]);
			float fTemp32 = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(std::max<float>(((iSlow4) ? fTemp25 : -3.4028235e+38f), ((iSlow5) ? fVec9[iSlow4] : -3.4028235e+38f)), ((iSlow6) ? fVec10[iSlow7] : -3.4028235e+38f)), ((iSlow8) ? fVec11[iSlow9] : -3.4028235e+38f)), ((iSlow10) ? fVec12[(IOTA0 - iSlow11) & 31] : -3.4028235e+38f)), ((iSlow12) ? fVec13[(IOTA0 - iSlow13) & 63] : -3.4028235e+38f)), ((iSlow14) ? fVec14[(IOTA0 - iSlow15) & 127] : -3.4028235e+38f)), ((iSlow16) ? fVec15[(IOTA0 - iSlow17) & 255] : -3.4028235e+38f))));
			int iTemp33 = (fTemp32 > fSlow21) + (fTemp32 > fSlow22);
			float fTemp34 = fTemp15 * std::max<float>(fSlow1, fTemp4 * ((iTemp33 == 0) ? fTemp32 - fSlow18 : ((iTemp33 == 1) ? fSlow23 * dyn_expand2_faustpower2_f(fTemp32 - fSlow22) : 0.0f)));
			float fTemp35 = ((fTemp34 > fRec3[1]) ? fTemp22 : fTemp19);
			fRec3[0] = fTemp34 * (1.0f - fTemp35) + fRec3[1] * fTemp35;
			float fTemp36 = std::max<float>(fRec1[0], fRec3[0]);
			float fTemp37 = fRec1[0] + fSlow26 * (fTemp36 - fRec1[0]);
			fHbargraph0 = FAUSTFLOAT(fTemp37);
			output0[i0] = FAUSTFLOAT(fTemp0 * (fRec0[0] + fTemp3 * std::pow(1e+01f, 0.05f * fTemp37)));
			float fTemp38 = fRec3[0] + fSlow26 * (fTemp36 - fRec3[0]);
			fHbargraph1 = FAUSTFLOAT(fTemp38);
			output1[i0] = FAUSTFLOAT(fTemp24 * (fRec0[0] + fTemp3 * std::pow(1e+01f, 0.05f * fTemp38)));
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
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
			fVec8[1] = fVec8[0];
			fVec9[2] = fVec9[1];
			fVec9[1] = fVec9[0];
			for (int j2 = 6; j2 > 0; j2 = j2 - 1) {
				fVec10[j2] = fVec10[j2 - 1];
			}
			for (int j3 = 14; j3 > 0; j3 = j3 - 1) {
				fVec11[j3] = fVec11[j3 - 1];
			}
			fRec3[1] = fRec3[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _dyn_expand2_UI : public UI {
    static std::string name;
};

template <class T>
std::string _dyn_expand2_UI<T>::name(sym(dyn_expand2));

typedef _dyn_expand2_UI<dyn_expand2> dyn_expand2_UI;

class faust_dyn_expand2_tilde : public FaustExternal<dyn_expand2, dyn_expand2_UI> {
public:
    faust_dyn_expand2_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
