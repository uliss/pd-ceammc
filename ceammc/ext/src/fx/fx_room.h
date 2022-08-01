/* ------------------------------------------------------------
author: "thedrgreenthumb"
license: "MIT"
name: "fx_room"
Code generated with Faust 2.44.1 (https://faust.grame.fr)
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
	float fConst6;
	float fConst9;
	float fConst11;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	float fRec19[2];
	float fConst12;
	float fConst13;
	float fRec18[3];
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fRec17[3];
	float fConst20;
	float fConst21;
	float fConst22;
	FAUSTFLOAT fHslider4;
	float fConst23;
	float fConst24;
	float fRec21[3];
	float fConst25;
	float fRec20[3];
	float fVec1[2048];
	int iConst26;
	float fRec15[2];
	float fVec2[4096];
	int iConst27;
	float fRec13[2];
	float fVec3[4096];
	int iConst28;
	int iConst29;
	float fVec4[16384];
	int iConst30;
	float fVec5[16384];
	int iConst31;
	float fRec11[2];
	float fRec9[2];
	float fVec6[8192];
	int iConst32;
	int iConst33;
	float fVec7[16384];
	int iConst34;
	float fVec8[16384];
	int iConst35;
	float fVec9[8192];
	int iConst36;
	float fRec7[2];
	float fRec5[2];
	float fRec3[2];
	float fRec1[2];
	float fRec38[3];
	float fRec37[3];
	float fRec36[2];
	float fConst39;
	float fConst40;
	float fConst41;
	float fConst42;
	float fRec40[3];
	float fConst43;
	float fRec39[3];
	float fVec10[8192];
	int iConst44;
	float fVec11[2048];
	int iConst45;
	float fVec12[8192];
	int iConst46;
	float fRec34[2];
	float fRec32[2];
	float fRec30[2];
	float fVec13[1024];
	int iConst47;
	float fVec14[8192];
	float fRec28[2];
	float fVec15[16384];
	int iConst48;
	int iConst49;
	float fVec16[8192];
	int iConst50;
	float fVec17[2048];
	int iConst51;
	float fRec26[2];
	float fRec24[2];
	float fRec22[2];
	float fConst54;
	float fConst57;
	float fConst59;
	float fConst60;
	float fConst61;
	float fRec55[3];
	float fConst62;
	float fConst63;
	float fConst64;
	float fRec54[3];
	float fRec53[2];
	float fVec18[8192];
	int iConst65;
	float fVec19[8192];
	float fVec20[8192];
	float fVec21[2048];
	float fRec51[2];
	float fRec49[2];
	float fRec47[2];
	float fVec22[16384];
	int iConst66;
	float fVec23[8192];
	float fRec45[2];
	float fRec43[2];
	float fRec41[2];
	
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
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
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
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.0999985f / fConst0;
		fConst2 = 1.0f - fConst1;
		float fConst3 = std::tan(3141.59277f / fConst0);
		float fConst4 = 1.0f / fConst3;
		float fConst5 = (fConst4 + 1.41421354f) / fConst3 + 1.0f;
		fConst6 = 1.29900002f / fConst5;
		float fConst7 = std::tan(1570.79639f / fConst0);
		float fConst8 = 1.0f / fConst7;
		fConst9 = 1.0f / ((fConst8 + 1.41421354f) / fConst7 + 1.0f);
		float fConst10 = fx_room_faustpower2_f(fConst7);
		fConst11 = 1.0f / fConst10;
		fConst12 = (fConst8 + -1.41421354f) / fConst7 + 1.0f;
		fConst13 = 2.0f * (1.0f - fConst11);
		fConst14 = 0.0f - 2.0f / fConst10;
		fConst15 = 1.0f / fConst5;
		fConst16 = (fConst4 + -1.41421354f) / fConst3 + 1.0f;
		fConst17 = 2.0f * (1.0f - 1.0f / fx_room_faustpower2_f(fConst3));
		float fConst18 = std::tan(12566.3711f / fConst0);
		float fConst19 = 1.0f / fConst18;
		fConst20 = 1.0f / ((fConst19 + 0.765366852f) / fConst18 + 1.0f);
		fConst21 = 1.0f / ((fConst19 + 1.84775901f) / fConst18 + 1.0f);
		fConst22 = 0.00100000005f * fConst0;
		fConst23 = (fConst19 + -1.84775901f) / fConst18 + 1.0f;
		fConst24 = 2.0f * (1.0f - 1.0f / fx_room_faustpower2_f(fConst18));
		fConst25 = (fConst19 + -0.765366852f) / fConst18 + 1.0f;
		iConst26 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.00800000038f * fConst0 + -1.0f)));
		iConst27 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0120000001f * fConst0 + -1.0f)));
		iConst28 = int(0.00400000019f * fConst0);
		iConst29 = iConst28 + int(0.0170000009f * fConst0);
		iConst30 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0869999975f * fConst0 + -1.0f)));
		iConst31 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.061999999f * fConst0 + -1.0f)));
		iConst32 = int(0.0309999995f * fConst0);
		iConst33 = iConst32 + int(0.00300000003f * fConst0);
		iConst34 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.119999997f * fConst0 + -1.0f)));
		iConst35 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0759999976f * fConst0 + -1.0f)));
		iConst36 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0299999993f * fConst0 + -1.0f)));
		float fConst37 = std::tan(18849.5566f / fConst0);
		float fConst38 = 1.0f / fConst37;
		fConst39 = 1.0f / ((fConst38 + 0.765366852f) / fConst37 + 1.0f);
		fConst40 = 1.0f / ((fConst38 + 1.84775901f) / fConst37 + 1.0f);
		fConst41 = (fConst38 + -1.84775901f) / fConst37 + 1.0f;
		fConst42 = 2.0f * (1.0f - 1.0f / fx_room_faustpower2_f(fConst37));
		fConst43 = (fConst38 + -0.765366852f) / fConst37 + 1.0f;
		iConst44 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0350000001f * fConst0 + -1.0f)));
		iConst45 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.00829999987f * fConst0 + -1.0f)));
		iConst46 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0219999999f * fConst0 + -1.0f)));
		iConst47 = int(0.00499999989f * fConst0);
		iConst48 = int(0.0670000017f * fConst0);
		iConst49 = iConst48 + int(0.0149999997f * fConst0);
		iConst50 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0390000008f * fConst0 + -1.0f)));
		iConst51 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.00980000012f * fConst0 + -1.0f)));
		float fConst52 = std::tan(5026.54834f / fConst0);
		float fConst53 = 1.0f / fConst52;
		fConst54 = 1.0f / ((fConst53 + 1.41421354f) / fConst52 + 1.0f);
		float fConst55 = std::tan(2513.27417f / fConst0);
		float fConst56 = 1.0f / fConst55;
		fConst57 = 1.0f / ((fConst56 + 1.41421354f) / fConst55 + 1.0f);
		float fConst58 = fx_room_faustpower2_f(fConst55);
		fConst59 = 1.0f / fConst58;
		fConst60 = (fConst56 + -1.41421354f) / fConst55 + 1.0f;
		fConst61 = 2.0f * (1.0f - fConst59);
		fConst62 = 0.0f - 2.0f / fConst58;
		fConst63 = (fConst53 + -1.41421354f) / fConst52 + 1.0f;
		fConst64 = 2.0f * (1.0f - 1.0f / fx_room_faustpower2_f(fConst52));
		iConst65 = int(0.0240000002f * fConst0);
		iConst66 = int(std::min<float>(8192.0f, std::max<float>(0.0f, 0.0659999996f * fConst0 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(1.0f);
		fHslider2 = FAUSTFLOAT(1.0f);
		fHslider3 = FAUSTFLOAT(0.29999999999999999f);
		fHslider4 = FAUSTFLOAT(20.0f);
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
			fRec19[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3; l3 = l3 + 1) {
			fRec18[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = l4 + 1) {
			fRec17[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = l5 + 1) {
			fRec21[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec20[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2048; l7 = l7 + 1) {
			fVec1[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec15[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 4096; l9 = l9 + 1) {
			fVec2[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec13[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4096; l11 = l11 + 1) {
			fVec3[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 16384; l12 = l12 + 1) {
			fVec4[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 16384; l13 = l13 + 1) {
			fVec5[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec9[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 8192; l16 = l16 + 1) {
			fVec6[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 16384; l17 = l17 + 1) {
			fVec7[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 16384; l18 = l18 + 1) {
			fVec8[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 8192; l19 = l19 + 1) {
			fVec9[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec7[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec5[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec3[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec1[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec38[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = l25 + 1) {
			fRec37[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec36[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec40[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 3; l28 = l28 + 1) {
			fRec39[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 8192; l29 = l29 + 1) {
			fVec10[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2048; l30 = l30 + 1) {
			fVec11[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 8192; l31 = l31 + 1) {
			fVec12[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec34[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec32[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec30[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 1024; l35 = l35 + 1) {
			fVec13[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 8192; l36 = l36 + 1) {
			fVec14[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec28[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 16384; l38 = l38 + 1) {
			fVec15[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 8192; l39 = l39 + 1) {
			fVec16[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2048; l40 = l40 + 1) {
			fVec17[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec26[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec24[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec22[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec55[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec54[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec53[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 8192; l47 = l47 + 1) {
			fVec18[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 8192; l48 = l48 + 1) {
			fVec19[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 8192; l49 = l49 + 1) {
			fVec20[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2048; l50 = l50 + 1) {
			fVec21[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec51[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec49[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec47[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 16384; l54 = l54 + 1) {
			fVec22[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 8192; l55 = l55 + 1) {
			fVec23[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec45[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec43[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec41[l58] = 0.0f;
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
		ui_interface->addHorizontalSlider("drywet", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("fb", &fHslider3, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0500000007f));
		ui_interface->addHorizontalSlider("hfdamp", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0500000007f));
		ui_interface->declare(&fHslider4, "unit", "ms");
		ui_interface->addHorizontalSlider("predelay", &fHslider4, FAUSTFLOAT(20.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(200.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("size", &fHslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0500000007f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		int iSlow0 = int(float(fCheckbox0));
		float fSlow1 = fConst1 * float(fHslider0);
		float fSlow2 = float(fHslider1);
		float fSlow3 = std::floor(fSlow2);
		float fSlow4 = fSlow3 + 1.0f - fSlow2;
		int iSlow5 = int(fSlow2);
		int iSlow6 = iSlow5 == 0;
		int iSlow7 = iSlow5 == 1;
		float fSlow8 = 1.0f - float(fHslider2);
		float fSlow9 = float(fHslider3);
		float fSlow10 = 0.5f * fSlow9;
		float fSlow11 = fConst22 * float(fHslider4);
		float fSlow12 = std::floor(fSlow11);
		float fSlow13 = fSlow12 + 1.0f - fSlow11;
		int iSlow14 = int(fSlow11);
		int iSlow15 = std::min<int>(65537, std::max<int>(0, iSlow14));
		float fSlow16 = fSlow11 - fSlow12;
		int iSlow17 = std::min<int>(65537, std::max<int>(0, iSlow14 + 1));
		float fSlow18 = 0.400000006f * fSlow9;
		float fSlow19 = 1.29900002f * fSlow9;
		float fSlow20 = fConst54 * fSlow9;
		float fSlow21 = fSlow2 - fSlow3;
		int iSlow22 = int(fSlow2 + 1.0f);
		int iSlow23 = iSlow22 == 0;
		int iSlow24 = iSlow22 == 1;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow1 + fConst2 * fRec0[1];
			float fTemp0 = float(input0[i0]);
			float fTemp1 = ((iSlow0) ? 0.0f : fTemp0);
			fVec0[IOTA0 & 65535] = fTemp1;
			fRec19[0] = fSlow8 * fRec19[1] + fSlow10 * fRec1[1];
			fRec18[0] = fRec19[0] - fConst9 * (fConst12 * fRec18[2] + fConst13 * fRec18[1]);
			fRec17[0] = fConst9 * (fConst11 * fRec18[0] + fConst14 * fRec18[1] + fConst11 * fRec18[2]) - fConst15 * (fConst16 * fRec17[2] + fConst17 * fRec17[1]);
			float fTemp2 = fSlow13 * fVec0[(IOTA0 - iSlow15) & 65535];
			float fTemp3 = fSlow16 * fVec0[(IOTA0 - iSlow17) & 65535];
			float fTemp4 = fTemp2 + fTemp3;
			fRec21[0] = fTemp4 - fConst21 * (fConst23 * fRec21[2] + fConst24 * fRec21[1]);
			fRec20[0] = fConst21 * (fRec21[2] + fRec21[0] + 2.0f * fRec21[1]) - fConst20 * (fConst25 * fRec20[2] + fConst24 * fRec20[1]);
			float fTemp5 = (fConst6 * (fRec17[2] + fRec17[0] + 2.0f * fRec17[1]) + fConst20 * (fRec20[2] + fRec20[0] + 2.0f * fRec20[1])) - 0.300000012f * fRec15[1];
			fVec1[IOTA0 & 2047] = fTemp5;
			fRec15[0] = fVec1[(IOTA0 - iConst26) & 2047];
			float fRec16 = 0.300000012f * fTemp5;
			float fTemp6 = (fRec16 + fRec15[1]) - 0.300000012f * fRec13[1];
			fVec2[IOTA0 & 4095] = fTemp6;
			fRec13[0] = fVec2[(IOTA0 - iConst27) & 4095];
			float fRec14 = 0.300000012f * fTemp6;
			float fTemp7 = fRec14 + fRec13[1];
			fVec3[IOTA0 & 4095] = fTemp7;
			float fTemp8 = fVec3[(IOTA0 - iConst29) & 4095] - 0.5f * fRec9[1];
			fVec4[IOTA0 & 16383] = fTemp8;
			float fTemp9 = fVec4[(IOTA0 - iConst30) & 16383] - 0.25f * fRec11[1];
			fVec5[IOTA0 & 16383] = fTemp9;
			fRec11[0] = fVec5[(IOTA0 - iConst31) & 16383];
			float fRec12 = 0.25f * fTemp9;
			fRec9[0] = fRec12 + fRec11[1];
			float fRec10 = 0.5f * fTemp8;
			float fTemp10 = fRec10 + fRec9[1];
			fVec6[IOTA0 & 8191] = fTemp10;
			float fTemp11 = fVec6[(IOTA0 - iConst33) & 8191] - 0.5f * fRec3[1];
			fVec7[IOTA0 & 16383] = fTemp11;
			float fTemp12 = fVec7[(IOTA0 - iConst34) & 16383] - 0.25f * fRec5[1];
			fVec8[IOTA0 & 16383] = fTemp12;
			float fTemp13 = fVec8[(IOTA0 - iConst35) & 16383] - 0.25f * fRec7[1];
			fVec9[IOTA0 & 8191] = fTemp13;
			fRec7[0] = fVec9[(IOTA0 - iConst36) & 8191];
			float fRec8 = 0.25f * fTemp13;
			fRec5[0] = fRec8 + fRec7[1];
			float fRec6 = 0.25f * fTemp12;
			fRec3[0] = fRec6 + fRec5[1];
			float fRec4 = 0.5f * fTemp11;
			fRec1[0] = fRec4 + fRec3[1];
			float fRec2 = 0.800000012f * fVec6[(IOTA0 - iConst32) & 8191] + 1.5f * fVec3[(IOTA0 - iConst28) & 4095];
			float fTemp14 = fRec1[0] + 0.800000012f * fRec2;
			fRec38[0] = fRec22[1] - fConst9 * (fConst12 * fRec38[2] + fConst13 * fRec38[1]);
			fRec37[0] = fConst9 * (fConst11 * fRec38[0] + fConst14 * fRec38[1] + fConst11 * fRec38[2]) - fConst15 * (fConst16 * fRec37[2] + fConst17 * fRec37[1]);
			fRec36[0] = fSlow8 * fRec36[1] + fConst15 * (fRec37[2] + fRec37[0] + 2.0f * fRec37[1]);
			fRec40[0] = fTemp4 - fConst40 * (fConst41 * fRec40[2] + fConst42 * fRec40[1]);
			fRec39[0] = fConst40 * (fRec40[2] + fRec40[0] + 2.0f * fRec40[1]) - fConst39 * (fConst43 * fRec39[2] + fConst42 * fRec39[1]);
			float fTemp15 = fConst39 * (fRec39[2] + fRec39[0] + 2.0f * fRec39[1]);
			float fTemp16 = (fSlow19 * fRec36[0] + fTemp15) - 0.25f * fRec30[1];
			fVec10[IOTA0 & 8191] = fTemp16;
			float fTemp17 = fVec10[(IOTA0 - iConst44) & 8191] - 0.349999994f * fRec32[1];
			fVec11[IOTA0 & 2047] = fTemp17;
			float fTemp18 = fVec11[(IOTA0 - iConst45) & 2047] - 0.449999988f * fRec34[1];
			fVec12[IOTA0 & 8191] = fTemp18;
			fRec34[0] = fVec12[(IOTA0 - iConst46) & 8191];
			float fRec35 = 0.449999988f * fTemp18;
			fRec32[0] = fRec35 + fRec34[1];
			float fRec33 = 0.349999994f * fTemp17;
			fRec30[0] = fRec33 + fRec32[1];
			float fRec31 = 0.25f * fTemp16;
			float fTemp19 = fRec31 + fRec30[1];
			fVec13[IOTA0 & 1023] = fTemp19;
			float fTemp20 = fVec13[(IOTA0 - iConst47) & 1023] - 0.449999988f * fRec28[1];
			fVec14[IOTA0 & 8191] = fTemp20;
			fRec28[0] = fVec14[(IOTA0 - iConst36) & 8191];
			float fRec29 = 0.449999988f * fTemp20;
			float fTemp21 = fRec29 + fRec28[1];
			fVec15[IOTA0 & 16383] = fTemp21;
			float fTemp22 = (fTemp3 + fTemp2 + fSlow18 * fVec15[(IOTA0 - iConst49) & 16383]) - 0.25f * fRec24[1];
			fVec16[IOTA0 & 8191] = fTemp22;
			float fTemp23 = fVec16[(IOTA0 - iConst50) & 8191] - 0.349999994f * fRec26[1];
			fVec17[IOTA0 & 2047] = fTemp23;
			fRec26[0] = fVec17[(IOTA0 - iConst51) & 2047];
			float fRec27 = 0.349999994f * fTemp23;
			fRec24[0] = fRec27 + fRec26[1];
			float fRec25 = 0.25f * fTemp22;
			fRec22[0] = fRec25 + fRec24[1];
			float fRec23 = 0.5f * (fTemp19 + fVec15[(IOTA0 - iConst48) & 16383]);
			float fTemp24 = fRec23 + 0.5f * fRec22[0];
			fRec55[0] = 0.99000001f * fRec41[1] - fConst57 * (fConst60 * fRec55[2] + fConst61 * fRec55[1]);
			fRec54[0] = fConst57 * (fConst59 * fRec55[0] + fConst62 * fRec55[1] + fConst59 * fRec55[2]) - fConst54 * (fConst63 * fRec54[2] + fConst64 * fRec54[1]);
			fRec53[0] = fSlow8 * fRec53[1] + fSlow20 * (fRec54[2] + fRec54[0] + 2.0f * fRec54[1]);
			fVec18[IOTA0 & 8191] = fRec53[0] + fTemp15;
			float fTemp25 = fVec18[(IOTA0 - iConst65) & 8191] - 0.150000006f * fRec47[1];
			fVec19[IOTA0 & 8191] = fTemp25;
			float fTemp26 = fVec19[(IOTA0 - iConst44) & 8191] - 0.25f * fRec49[1];
			fVec20[IOTA0 & 8191] = fTemp26;
			float fTemp27 = fVec20[(IOTA0 - iConst46) & 8191] - 0.300000012f * fRec51[1];
			fVec21[IOTA0 & 2047] = fTemp27;
			fRec51[0] = fVec21[(IOTA0 - iConst45) & 2047];
			float fRec52 = 0.300000012f * fTemp27;
			fRec49[0] = fRec52 + fRec51[1];
			float fRec50 = 0.25f * fTemp26;
			fRec47[0] = fRec50 + fRec49[1];
			float fRec48 = 0.150000006f * fTemp25;
			float fTemp28 = fRec48 + fRec47[1];
			float fTemp29 = fTemp28 - 0.0799999982f * fRec43[1];
			fVec22[IOTA0 & 16383] = fTemp29;
			float fTemp30 = fVec22[(IOTA0 - iConst66) & 16383] - 0.300000012f * fRec45[1];
			fVec23[IOTA0 & 8191] = fTemp30;
			fRec45[0] = fVec23[(IOTA0 - iConst36) & 8191];
			float fRec46 = 0.300000012f * fTemp30;
			fRec43[0] = fRec46 + fRec45[1];
			float fRec44 = 0.0799999982f * fTemp29;
			fRec41[0] = fRec44 + fRec43[1];
			float fRec42 = fTemp28;
			float fTemp31 = 0.5f * (fRec41[0] + fRec42);
			float fThen2 = ((iSlow7) ? fTemp24 : fTemp14);
			float fThen4 = ((iSlow24) ? fTemp24 : fTemp14);
			float fThen5 = (1.0f - fRec0[0]) * fTemp1 + fRec0[0] * (fSlow4 * ((iSlow6) ? fTemp31 : fThen2) + fSlow21 * ((iSlow23) ? fTemp31 : fThen4));
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp0 : fThen5));
			fRec0[1] = fRec0[0];
			IOTA0 = IOTA0 + 1;
			fRec19[1] = fRec19[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec15[1] = fRec15[0];
			fRec13[1] = fRec13[0];
			fRec11[1] = fRec11[0];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec5[1] = fRec5[0];
			fRec3[1] = fRec3[0];
			fRec1[1] = fRec1[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec34[1] = fRec34[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			fRec28[1] = fRec28[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec53[1] = fRec53[0];
			fRec51[1] = fRec51[0];
			fRec49[1] = fRec49[0];
			fRec47[1] = fRec47[0];
			fRec45[1] = fRec45[0];
			fRec43[1] = fRec43[0];
			fRec41[1] = fRec41[0];
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
