/* ------------------------------------------------------------
author: "thedrgreenthumb"
license: "MIT"
name: "fx_room"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_room -scn fx_room_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_room_H__
#define  __fx_room_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_room_dsp.h ********************************
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

class FAUST_API fx_room_dsp {

    public:

        fx_room_dsp() {}
        virtual ~fx_room_dsp() {}

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
        virtual fx_room_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_room_dsp {

    protected:

        fx_room_dsp* fDSP;

    public:

        decorator_dsp(fx_room_dsp* fx_room_dsp = nullptr):fDSP(fx_room_dsp) {}
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
    
        virtual fx_room_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_room_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_room_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_room : public fx_room_dsp {
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
#define FAUSTCLASS fx_room
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

static float fx_room_faustpower2_f(float value) {
	return value * value;
}

class fx_room : public fx_room_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec0[2];
	int IOTA0;
	float fVec0[65536];
	FAUSTFLOAT fHslider1;
	float fRec1[2];
	float fConst3;
	float fRec2[2];
	float fRec3[2];
	FAUSTFLOAT fHslider2;
	float fConst6;
	FAUSTFLOAT fHslider3;
	float fConst9;
	float fConst11;
	float fConst12;
	float fConst13;
	float fRec18[3];
	float fConst14;
	float fConst15;
	float fConst16;
	float fRec17[3];
	float fRec16[2];
	float fConst19;
	float fConst20;
	float fConst21;
	FAUSTFLOAT fHslider4;
	float fConst22;
	float fConst23;
	float fRec20[3];
	float fConst24;
	float fRec19[3];
	float fVec1[8192];
	int iConst25;
	float fVec2[8192];
	int iConst26;
	float fVec3[8192];
	int iConst27;
	float fVec4[2048];
	int iConst28;
	float fRec14[2];
	float fRec12[2];
	float fRec10[2];
	float fVec5[16384];
	int iConst29;
	float fVec6[8192];
	int iConst30;
	float fRec8[2];
	float fRec6[2];
	float fRec4[2];
	float fRec21[2];
	float fConst34;
	float fConst37;
	float fConst39;
	float fConst40;
	float fConst41;
	float fRec38[3];
	float fConst42;
	float fConst43;
	float fConst44;
	float fRec37[3];
	float fRec36[2];
	float fVec7[8192];
	float fVec8[2048];
	float fVec9[8192];
	float fRec34[2];
	float fRec32[2];
	float fRec30[2];
	float fVec10[1024];
	int iConst45;
	float fVec11[8192];
	float fRec28[2];
	float fVec12[16384];
	int iConst46;
	int iConst47;
	float fVec13[8192];
	int iConst48;
	float fVec14[2048];
	int iConst49;
	float fRec26[2];
	float fRec24[2];
	float fRec22[2];
	float fConst50;
	float fRec57[2];
	float fRec56[3];
	float fRec55[3];
	float fConst53;
	float fConst54;
	float fConst55;
	float fConst56;
	float fRec59[3];
	float fConst57;
	float fRec58[3];
	float fVec15[2048];
	int iConst58;
	float fRec53[2];
	float fVec16[4096];
	int iConst59;
	float fRec51[2];
	float fVec17[4096];
	int iConst60;
	int iConst61;
	float fVec18[16384];
	int iConst62;
	float fVec19[16384];
	int iConst63;
	float fRec49[2];
	float fRec47[2];
	float fVec20[8192];
	int iConst64;
	int iConst65;
	float fVec21[16384];
	int iConst66;
	float fVec22[16384];
	int iConst67;
	float fVec23[8192];
	float fRec45[2];
	float fRec43[2];
	float fRec41[2];
	float fRec39[2];
	float fRec60[2];
	float fRec61[2];
	float fRec62[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "thedrgreenthumb");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_room -scn fx_room_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_room.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("id", "fx.room");
		m->declare("license", "MIT");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "fx_room");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 2e+01f / fConst0;
		float fConst4 = std::tan(5026.5483f / fConst0);
		float fConst5 = 1.0f / fConst4;
		fConst6 = 1.0f / ((fConst5 + 1.4142135f) / fConst4 + 1.0f);
		float fConst7 = std::tan(2513.2742f / fConst0);
		float fConst8 = 1.0f / fConst7;
		fConst9 = 1.0f / ((fConst8 + 1.4142135f) / fConst7 + 1.0f);
		float fConst10 = fx_room_faustpower2_f(fConst7);
		fConst11 = 1.0f / fConst10;
		fConst12 = (fConst8 + -1.4142135f) / fConst7 + 1.0f;
		fConst13 = 2.0f * (1.0f - fConst11);
		fConst14 = 0.0f - 2.0f / fConst10;
		fConst15 = (fConst5 + -1.4142135f) / fConst4 + 1.0f;
		fConst16 = 2.0f * (1.0f - 1.0f / fx_room_faustpower2_f(fConst4));
		float fConst17 = std::tan(18849.557f / fConst0);
		float fConst18 = 1.0f / fConst17;
		fConst19 = 1.0f / ((fConst18 + 0.76536685f) / fConst17 + 1.0f);
		fConst20 = 1.0f / ((fConst18 + 1.847759f) / fConst17 + 1.0f);
		fConst21 = 0.001f * fConst0;
		fConst22 = (fConst18 + -1.847759f) / fConst17 + 1.0f;
		fConst23 = 2.0f * (1.0f - 1.0f / fx_room_faustpower2_f(fConst17));
		fConst24 = (fConst18 + -0.76536685f) / fConst17 + 1.0f;
		iConst25 = int(0.024f * fConst0);
		iConst26 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.035f * fConst0 + -1.0f)));
		iConst27 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.022f * fConst0 + -1.0f)));
		iConst28 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0083f * fConst0 + -1.0f)));
		iConst29 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.066f * fConst0 + -1.0f)));
		iConst30 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.03f * fConst0 + -1.0f)));
		float fConst31 = std::tan(3141.5928f / fConst0);
		float fConst32 = 1.0f / fConst31;
		float fConst33 = (fConst32 + 1.4142135f) / fConst31 + 1.0f;
		fConst34 = 1.0f / fConst33;
		float fConst35 = std::tan(1570.7964f / fConst0);
		float fConst36 = 1.0f / fConst35;
		fConst37 = 1.0f / ((fConst36 + 1.4142135f) / fConst35 + 1.0f);
		float fConst38 = fx_room_faustpower2_f(fConst35);
		fConst39 = 1.0f / fConst38;
		fConst40 = (fConst36 + -1.4142135f) / fConst35 + 1.0f;
		fConst41 = 2.0f * (1.0f - fConst39);
		fConst42 = 0.0f - 2.0f / fConst38;
		fConst43 = (fConst32 + -1.4142135f) / fConst31 + 1.0f;
		fConst44 = 2.0f * (1.0f - 1.0f / fx_room_faustpower2_f(fConst31));
		iConst45 = int(0.005f * fConst0);
		iConst46 = int(0.067f * fConst0);
		iConst47 = iConst46 + int(0.015f * fConst0);
		iConst48 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.039f * fConst0 + -1.0f)));
		iConst49 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0098f * fConst0 + -1.0f)));
		fConst50 = 1.299f / fConst33;
		float fConst51 = std::tan(12566.371f / fConst0);
		float fConst52 = 1.0f / fConst51;
		fConst53 = 1.0f / ((fConst52 + 0.76536685f) / fConst51 + 1.0f);
		fConst54 = 1.0f / ((fConst52 + 1.847759f) / fConst51 + 1.0f);
		fConst55 = (fConst52 + -1.847759f) / fConst51 + 1.0f;
		fConst56 = 2.0f * (1.0f - 1.0f / fx_room_faustpower2_f(fConst51));
		fConst57 = (fConst52 + -0.76536685f) / fConst51 + 1.0f;
		iConst58 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.008f * fConst0 + -1.0f)));
		iConst59 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.012f * fConst0 + -1.0f)));
		iConst60 = int(0.004f * fConst0);
		iConst61 = iConst60 + int(0.017f * fConst0);
		iConst62 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.087f * fConst0 + -1.0f)));
		iConst63 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.062f * fConst0 + -1.0f)));
		iConst64 = int(0.031f * fConst0);
		iConst65 = iConst64 + int(0.003f * fConst0);
		iConst66 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.12f * fConst0 + -1.0f)));
		iConst67 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.076f * fConst0 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(1.0f);
		fHslider2 = FAUSTFLOAT(1.0f);
		fHslider3 = FAUSTFLOAT(0.3f);
		fHslider4 = FAUSTFLOAT(2e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		IOTA0 = 0;
		for (int l1 = 0; l1 < 65536; l1 = l1 + 1) {
			fVec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = l5 + 1) {
			fRec18[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec17[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec16[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec20[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec19[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 8192; l10 = l10 + 1) {
			fVec1[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 8192; l11 = l11 + 1) {
			fVec2[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 8192; l12 = l12 + 1) {
			fVec3[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2048; l13 = l13 + 1) {
			fVec4[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec14[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec12[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec10[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 16384; l17 = l17 + 1) {
			fVec5[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 8192; l18 = l18 + 1) {
			fVec6[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec8[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec6[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec4[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec21[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec38[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec37[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec36[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 8192; l26 = l26 + 1) {
			fVec7[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2048; l27 = l27 + 1) {
			fVec8[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 8192; l28 = l28 + 1) {
			fVec9[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec34[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec32[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec30[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 1024; l32 = l32 + 1) {
			fVec10[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 8192; l33 = l33 + 1) {
			fVec11[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec28[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 16384; l35 = l35 + 1) {
			fVec12[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 8192; l36 = l36 + 1) {
			fVec13[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2048; l37 = l37 + 1) {
			fVec14[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec26[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec24[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec22[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec57[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec56[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fRec55[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec59[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec58[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2048; l46 = l46 + 1) {
			fVec15[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec53[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 4096; l48 = l48 + 1) {
			fVec16[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec51[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 4096; l50 = l50 + 1) {
			fVec17[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 16384; l51 = l51 + 1) {
			fVec18[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 16384; l52 = l52 + 1) {
			fVec19[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec49[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec47[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 8192; l55 = l55 + 1) {
			fVec20[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 16384; l56 = l56 + 1) {
			fVec21[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 16384; l57 = l57 + 1) {
			fVec22[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 8192; l58 = l58 + 1) {
			fVec23[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec45[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec43[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec41[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec39[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fRec60[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec61[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec62[l65] = 0.0f;
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
	
	virtual fx_room* clone() {
		return new fx_room();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx_room");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("fb", &fHslider3, FAUSTFLOAT(0.3f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.05f));
		ui_interface->addHorizontalSlider("hfdamp", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.05f));
		ui_interface->declare(&fHslider4, "unit", "ms");
		ui_interface->addHorizontalSlider("predelay", &fHslider4, FAUSTFLOAT(2e+01f), FAUSTFLOAT(1.0f), FAUSTFLOAT(2e+02f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("size", &fHslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3.0f), FAUSTFLOAT(0.05f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = fConst1 * float(fHslider0);
		float fSlow2 = float(fHslider1);
		float fSlow3 = fConst1 * (fSlow2 - std::floor(fSlow2));
		float fSlow4 = float(fSlow2 >= 1.0f);
		float fSlow5 = float(fSlow2 >= 2.0f);
		float fSlow6 = 1.0f - float(fHslider2);
		float fSlow7 = float(fHslider3);
		float fSlow8 = fConst6 * fSlow7;
		float fSlow9 = fConst21 * float(fHslider4);
		float fSlow10 = std::floor(fSlow9);
		float fSlow11 = fSlow10 + (1.0f - fSlow9);
		int iSlow12 = int(fSlow9);
		int iSlow13 = std::min<int>(65537, std::max<int>(0, iSlow12));
		float fSlow14 = fSlow9 - fSlow10;
		int iSlow15 = std::min<int>(65537, std::max<int>(0, iSlow12 + 1));
		float fSlow16 = float(fSlow2 >= 3.0f);
		float fSlow17 = 0.4f * fSlow7;
		float fSlow18 = 1.299f * fSlow7;
		float fSlow19 = 0.5f * fSlow7;
		float fSlow20 = fSlow2 + 1.0f;
		float fSlow21 = float(fSlow20 >= 1.0f);
		float fSlow22 = float(fSlow20 >= 2.0f);
		float fSlow23 = float(fSlow20 >= 3.0f);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow1 + fConst2 * fRec0[1];
			float fTemp0 = float(input0[i0]);
			float fTemp1 = ((iSlow0) ? 0.0f : fTemp0);
			fVec0[IOTA0 & 65535] = fTemp1;
			fRec1[0] = fSlow3 + fConst2 * fRec1[1];
			float fTemp2 = fConst3 + fRec2[1];
			float fTemp3 = fRec2[1] - fConst3;
			fRec2[0] = ((fTemp2 < fSlow4) ? fTemp2 : ((fTemp3 > fSlow4) ? fTemp3 : fSlow4));
			float fTemp4 = fConst3 + fRec3[1];
			float fTemp5 = fRec3[1] - fConst3;
			fRec3[0] = ((fTemp4 < fSlow5) ? fTemp4 : ((fTemp5 > fSlow5) ? fTemp5 : fSlow5));
			fRec18[0] = 0.99f * fRec4[1] - fConst9 * (fConst12 * fRec18[2] + fConst13 * fRec18[1]);
			fRec17[0] = fConst9 * (fConst11 * fRec18[0] + fConst14 * fRec18[1] + fConst11 * fRec18[2]) - fConst6 * (fConst15 * fRec17[2] + fConst16 * fRec17[1]);
			fRec16[0] = fSlow6 * fRec16[1] + fSlow8 * (fRec17[2] + fRec17[0] + 2.0f * fRec17[1]);
			float fTemp6 = fSlow11 * fVec0[(IOTA0 - iSlow13) & 65535] + fSlow14 * fVec0[(IOTA0 - iSlow15) & 65535];
			fRec20[0] = fTemp6 - fConst20 * (fConst22 * fRec20[2] + fConst23 * fRec20[1]);
			fRec19[0] = fConst20 * (fRec20[2] + fRec20[0] + 2.0f * fRec20[1]) - fConst19 * (fConst24 * fRec19[2] + fConst23 * fRec19[1]);
			float fTemp7 = fConst19 * (fRec19[2] + fRec19[0] + 2.0f * fRec19[1]);
			fVec1[IOTA0 & 8191] = fRec16[0] + fTemp7;
			float fTemp8 = fVec1[(IOTA0 - iConst25) & 8191] - 0.15f * fRec10[1];
			fVec2[IOTA0 & 8191] = fTemp8;
			float fTemp9 = fVec2[(IOTA0 - iConst26) & 8191] - 0.25f * fRec12[1];
			fVec3[IOTA0 & 8191] = fTemp9;
			float fTemp10 = fVec3[(IOTA0 - iConst27) & 8191] - 0.3f * fRec14[1];
			fVec4[IOTA0 & 2047] = fTemp10;
			fRec14[0] = fVec4[(IOTA0 - iConst28) & 2047];
			float fRec15 = 0.3f * fTemp10;
			fRec12[0] = fRec15 + fRec14[1];
			float fRec13 = 0.25f * fTemp9;
			fRec10[0] = fRec13 + fRec12[1];
			float fRec11 = 0.15f * fTemp8;
			float fTemp11 = fRec11 + fRec10[1];
			float fTemp12 = fTemp11 - 0.08f * fRec6[1];
			fVec5[IOTA0 & 16383] = fTemp12;
			float fTemp13 = fVec5[(IOTA0 - iConst29) & 16383] - 0.3f * fRec8[1];
			fVec6[IOTA0 & 8191] = fTemp13;
			fRec8[0] = fVec6[(IOTA0 - iConst30) & 8191];
			float fRec9 = 0.3f * fTemp13;
			fRec6[0] = fRec9 + fRec8[1];
			float fRec7 = 0.08f * fTemp12;
			fRec4[0] = fRec7 + fRec6[1];
			float fRec5 = fTemp11;
			float fTemp14 = fRec4[0] + fRec5;
			float fTemp15 = fConst3 + fRec21[1];
			float fTemp16 = fRec21[1] - fConst3;
			fRec21[0] = ((fTemp15 < fSlow16) ? fTemp15 : ((fTemp16 > fSlow16) ? fTemp16 : fSlow16));
			fRec38[0] = fRec22[1] - fConst37 * (fConst40 * fRec38[2] + fConst41 * fRec38[1]);
			fRec37[0] = fConst37 * (fConst39 * fRec38[0] + fConst42 * fRec38[1] + fConst39 * fRec38[2]) - fConst34 * (fConst43 * fRec37[2] + fConst44 * fRec37[1]);
			fRec36[0] = fSlow6 * fRec36[1] + fConst34 * (fRec37[2] + fRec37[0] + 2.0f * fRec37[1]);
			float fTemp17 = fTemp7 + fSlow18 * fRec36[0] - 0.25f * fRec30[1];
			fVec7[IOTA0 & 8191] = fTemp17;
			float fTemp18 = fVec7[(IOTA0 - iConst26) & 8191] - 0.35f * fRec32[1];
			fVec8[IOTA0 & 2047] = fTemp18;
			float fTemp19 = fVec8[(IOTA0 - iConst28) & 2047] - 0.45f * fRec34[1];
			fVec9[IOTA0 & 8191] = fTemp19;
			fRec34[0] = fVec9[(IOTA0 - iConst27) & 8191];
			float fRec35 = 0.45f * fTemp19;
			fRec32[0] = fRec35 + fRec34[1];
			float fRec33 = 0.35f * fTemp18;
			fRec30[0] = fRec33 + fRec32[1];
			float fRec31 = 0.25f * fTemp17;
			float fTemp20 = fRec31 + fRec30[1];
			fVec10[IOTA0 & 1023] = fTemp20;
			float fTemp21 = fVec10[(IOTA0 - iConst45) & 1023] - 0.45f * fRec28[1];
			fVec11[IOTA0 & 8191] = fTemp21;
			fRec28[0] = fVec11[(IOTA0 - iConst30) & 8191];
			float fRec29 = 0.45f * fTemp21;
			float fTemp22 = fRec29 + fRec28[1];
			fVec12[IOTA0 & 16383] = fTemp22;
			float fTemp23 = fTemp6 + fSlow17 * fVec12[(IOTA0 - iConst47) & 16383] - 0.25f * fRec24[1];
			fVec13[IOTA0 & 8191] = fTemp23;
			float fTemp24 = fVec13[(IOTA0 - iConst48) & 8191] - 0.35f * fRec26[1];
			fVec14[IOTA0 & 2047] = fTemp24;
			fRec26[0] = fVec14[(IOTA0 - iConst49) & 2047];
			float fRec27 = 0.35f * fTemp24;
			fRec24[0] = fRec27 + fRec26[1];
			float fRec25 = 0.25f * fTemp23;
			fRec22[0] = fRec25 + fRec24[1];
			float fRec23 = 0.5f * (fTemp20 + fVec12[(IOTA0 - iConst46) & 16383]);
			float fTemp25 = fRec23 + 0.5f * fRec22[0];
			fRec57[0] = fSlow6 * fRec57[1] + fSlow19 * fRec39[1];
			fRec56[0] = fRec57[0] - fConst37 * (fConst40 * fRec56[2] + fConst41 * fRec56[1]);
			fRec55[0] = fConst37 * (fConst39 * fRec56[0] + fConst42 * fRec56[1] + fConst39 * fRec56[2]) - fConst34 * (fConst43 * fRec55[2] + fConst44 * fRec55[1]);
			fRec59[0] = fTemp6 - fConst54 * (fConst55 * fRec59[2] + fConst56 * fRec59[1]);
			fRec58[0] = fConst54 * (fRec59[2] + fRec59[0] + 2.0f * fRec59[1]) - fConst53 * (fConst57 * fRec58[2] + fConst56 * fRec58[1]);
			float fTemp26 = fConst50 * (fRec55[2] + fRec55[0] + 2.0f * fRec55[1]) + fConst53 * (fRec58[2] + fRec58[0] + 2.0f * fRec58[1]) - 0.3f * fRec53[1];
			fVec15[IOTA0 & 2047] = fTemp26;
			fRec53[0] = fVec15[(IOTA0 - iConst58) & 2047];
			float fRec54 = 0.3f * fTemp26;
			float fTemp27 = fRec54 + fRec53[1] - 0.3f * fRec51[1];
			fVec16[IOTA0 & 4095] = fTemp27;
			fRec51[0] = fVec16[(IOTA0 - iConst59) & 4095];
			float fRec52 = 0.3f * fTemp27;
			float fTemp28 = fRec52 + fRec51[1];
			fVec17[IOTA0 & 4095] = fTemp28;
			float fTemp29 = fVec17[(IOTA0 - iConst61) & 4095] - 0.5f * fRec47[1];
			fVec18[IOTA0 & 16383] = fTemp29;
			float fTemp30 = fVec18[(IOTA0 - iConst62) & 16383] - 0.25f * fRec49[1];
			fVec19[IOTA0 & 16383] = fTemp30;
			fRec49[0] = fVec19[(IOTA0 - iConst63) & 16383];
			float fRec50 = 0.25f * fTemp30;
			fRec47[0] = fRec50 + fRec49[1];
			float fRec48 = 0.5f * fTemp29;
			float fTemp31 = fRec48 + fRec47[1];
			fVec20[IOTA0 & 8191] = fTemp31;
			float fTemp32 = fVec20[(IOTA0 - iConst65) & 8191] - 0.5f * fRec41[1];
			fVec21[IOTA0 & 16383] = fTemp32;
			float fTemp33 = fVec21[(IOTA0 - iConst66) & 16383] - 0.25f * fRec43[1];
			fVec22[IOTA0 & 16383] = fTemp33;
			float fTemp34 = fVec22[(IOTA0 - iConst67) & 16383] - 0.25f * fRec45[1];
			fVec23[IOTA0 & 8191] = fTemp34;
			fRec45[0] = fVec23[(IOTA0 - iConst30) & 8191];
			float fRec46 = 0.25f * fTemp34;
			fRec43[0] = fRec46 + fRec45[1];
			float fRec44 = 0.25f * fTemp33;
			fRec41[0] = fRec44 + fRec43[1];
			float fRec42 = 0.5f * fTemp32;
			fRec39[0] = fRec42 + fRec41[1];
			float fRec40 = 0.8f * fVec20[(IOTA0 - iConst64) & 8191] + 1.5f * fVec17[(IOTA0 - iConst60) & 4095];
			float fTemp35 = fRec39[0] + 0.8f * fRec40;
			float fTemp36 = fConst3 + fRec60[1];
			float fTemp37 = fRec60[1] - fConst3;
			fRec60[0] = ((fTemp36 < fSlow21) ? fTemp36 : ((fTemp37 > fSlow21) ? fTemp37 : fSlow21));
			float fTemp38 = fConst3 + fRec61[1];
			float fTemp39 = fRec61[1] - fConst3;
			fRec61[0] = ((fTemp38 < fSlow22) ? fTemp38 : ((fTemp39 > fSlow22) ? fTemp39 : fSlow22));
			float fTemp40 = fConst3 + fRec62[1];
			float fTemp41 = fRec62[1] - fConst3;
			fRec62[0] = ((fTemp40 < fSlow23) ? fTemp40 : ((fTemp41 > fSlow23) ? fTemp41 : fSlow23));
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp0 : (1.0f - fRec0[0]) * fTemp1 + fRec0[0] * ((1.0f - fRec1[0]) * (0.5f * fRec2[0] * (1.0f - fRec3[0]) * fTemp14 + fRec3[0] * ((1.0f - fRec21[0]) * fTemp25 + fRec21[0] * fTemp35)) + fRec1[0] * (0.5f * fRec60[0] * (1.0f - fRec61[0]) * fTemp14 + fRec61[0] * ((1.0f - fRec62[0]) * fTemp25 + fRec62[0] * fTemp35)))));
			fRec0[1] = fRec0[0];
			IOTA0 = IOTA0 + 1;
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec14[1] = fRec14[0];
			fRec12[1] = fRec12[0];
			fRec10[1] = fRec10[0];
			fRec8[1] = fRec8[0];
			fRec6[1] = fRec6[0];
			fRec4[1] = fRec4[0];
			fRec21[1] = fRec21[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec34[1] = fRec34[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			fRec28[1] = fRec28[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec53[1] = fRec53[0];
			fRec51[1] = fRec51[0];
			fRec49[1] = fRec49[0];
			fRec47[1] = fRec47[0];
			fRec45[1] = fRec45[0];
			fRec43[1] = fRec43[0];
			fRec41[1] = fRec41[0];
			fRec39[1] = fRec39[0];
			fRec60[1] = fRec60[0];
			fRec61[1] = fRec61[0];
			fRec62[1] = fRec62[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_room_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_room_UI<T>::name(sym(fx_room));

typedef _fx_room_UI<fx_room> fx_room_UI;

class faust_fx_room_tilde : public FaustExternal<fx_room, fx_room_UI> {
public:
    faust_fx_room_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
