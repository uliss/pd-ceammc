/* ------------------------------------------------------------
name: "shtokhausen.solo"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn piece_stok_solo -scn piece_stok_solo_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __piece_stok_solo_H__
#define  __piece_stok_solo_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN piece_stok_solo_dsp.h ********************************
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

class FAUST_API piece_stok_solo_dsp {

    public:

        piece_stok_solo_dsp() {}
        virtual ~piece_stok_solo_dsp() {}

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
        virtual piece_stok_solo_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public piece_stok_solo_dsp {

    protected:

        piece_stok_solo_dsp* fDSP;

    public:

        decorator_dsp(piece_stok_solo_dsp* piece_stok_solo_dsp = nullptr):fDSP(piece_stok_solo_dsp) {}
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
    
        virtual piece_stok_solo_dsp* createDSPInstance() = 0;
    
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

/************************** END piece_stok_solo_dsp.h **************************/
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
 The base class of Meta handler to be used in piece_stok_solo_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct piece_stok_solo : public piece_stok_solo_dsp {
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
#define FAUSTCLASS piece_stok_solo
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

static float piece_stok_solo_faustpower2_f(float value) {
	return value * value;
}

class piece_stok_solo : public piece_stok_solo_dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fCheckbox0;
	float fConst2;
	float fRec1[2];
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider1;
	float fRec2[2];
	float fConst5;
	float fConst7;
	float fConst10;
	float fConst12;
	float fConst13;
	float fConst14;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	float fRec7[2];
	float fConst15;
	float fConst16;
	float fRec9[2];
	FAUSTFLOAT fHslider4;
	int iVec0[2];
	int iConst17;
	int iRec10[2];
	float fConst18;
	float fRec8[2];
	float fVec1[2];
	float fRec6[2];
	float fConst19;
	float fConst20;
	float fRec5[3];
	float fVec2[2];
	float fConst21;
	float fConst22;
	float fRec4[2];
	float fConst23;
	float fConst24;
	float fRec3[3];
	float fConst25;
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fRec12[2];
	float fRec11[2];
	int IOTA0;
	float fVec3[8388608];
	FAUSTFLOAT fCheckbox2;
	float fRec13[2];
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fCheckbox3;
	float fRec14[2];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fCheckbox4;
	float fRec15[2];
	FAUSTFLOAT fHslider7;
	FAUSTFLOAT fCheckbox5;
	float fRec16[2];
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fCheckbox6;
	float fRec17[2];
	FAUSTFLOAT fHslider9;
	float fRec0[2];
	FAUSTFLOAT fHslider10;
	float fRec18[2];
	FAUSTFLOAT fCheckbox7;
	float fRec22[2];
	FAUSTFLOAT fHslider11;
	float fRec27[2];
	float fRec29[2];
	int iVec4[2];
	int iRec30[2];
	float fRec28[2];
	float fVec5[2];
	float fRec26[2];
	float fRec25[3];
	float fVec6[2];
	float fRec24[2];
	float fRec23[3];
	float fRec32[2];
	float fRec31[2];
	float fVec7[8388608];
	float fRec21[2];
	FAUSTFLOAT fHslider12;
	float fRec33[2];
	float fRec20[2];
	float fRec19[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/amp_follower_ar:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.2");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn piece_stok_solo -scn piece_stok_solo_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("compressors.lib/compression_gain_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compression_gain_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compression_gain_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_lad_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_lad_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_lad_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_stereo:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_stereo:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_stereo:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/limiter_1176_R4_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/limiter_1176_R4_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/limiter_1176_R4_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/limiter_1176_R4_stereo:author", "Julius O. Smith III");
		m->declare("compressors.lib/limiter_1176_R4_stereo:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/limiter_1176_R4_stereo:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/version", "0.4");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "piece_stok_solo.dsp");
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
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.0");
		m->declare("name", "shtokhausen.solo");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		float fConst3 = std::tan(314.15927f / fConst0);
		float fConst4 = 1.0f / fConst3;
		fConst5 = 1.0f / ((fConst4 + 1.0f) / fConst3 + 1.0f);
		float fConst6 = piece_stok_solo_faustpower2_f(fConst3);
		fConst7 = 1.0f / fConst6;
		float fConst8 = std::tan(31415.926f / fConst0);
		float fConst9 = 1.0f / fConst8;
		fConst10 = 1.0f / ((fConst9 + 1.0f) / fConst8 + 1.0f);
		float fConst11 = fConst4 + 1.0f;
		fConst12 = 1.0f / (fConst3 * fConst11);
		fConst13 = 1.0f / (fConst9 + 1.0f);
		fConst14 = 1.0f - fConst9;
		fConst15 = std::exp(0.0f - 1e+04f / fConst0);
		fConst16 = 1.0f - fConst15;
		iConst17 = int(0.2f * fConst0);
		fConst18 = std::exp(0.0f - 1e+01f / fConst0);
		fConst19 = (fConst9 + -1.0f) / fConst8 + 1.0f;
		fConst20 = 2.0f * (1.0f - 1.0f / piece_stok_solo_faustpower2_f(fConst8));
		fConst21 = 0.0f - fConst12;
		fConst22 = (1.0f - fConst4) / fConst11;
		fConst23 = (fConst4 + -1.0f) / fConst3 + 1.0f;
		fConst24 = 2.0f * (1.0f - fConst7);
		fConst25 = 0.0f - 2.0f / fConst6;
		fConst26 = std::exp(0.0f - 2.5e+03f / fConst0);
		fConst27 = 1.0f - fConst26;
		fConst28 = std::exp(0.0f - 2.0f / fConst0);
		fConst29 = std::exp(0.0f - 1.25e+03f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(1e+01f);
		fHslider2 = FAUSTFLOAT(0.999f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(-4e+01f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fCheckbox4 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fCheckbox5 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fCheckbox6 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.0f);
		fHslider10 = FAUSTFLOAT(0.0f);
		fCheckbox7 = FAUSTFLOAT(0.0f);
		fHslider11 = FAUSTFLOAT(0.0f);
		fHslider12 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec1[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec2[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec7[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec9[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iVec0[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec10[l5] = 0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec8[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec1[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec5[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fVec2[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec4[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec3[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec12[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec11[l14] = 0.0f;
		}
		IOTA0 = 0;
		for (int l15 = 0; l15 < 8388608; l15 = l15 + 1) {
			fVec3[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec13[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec14[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec15[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec0[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec18[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec22[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec27[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec29[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			iVec4[l26] = 0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			iRec30[l27] = 0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec28[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fVec5[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec26[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 3; l31 = l31 + 1) {
			fRec25[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fVec6[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec24[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fRec23[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec32[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec31[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 8388608; l37 = l37 + 1) {
			fVec7[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec21[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec33[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec20[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec19[l41] = 0.0f;
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
	
	virtual piece_stok_solo* clone() {
		return new piece_stok_solo();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("shtokhausen.solo");
		ui_interface->addCheckButton("cycle0", &fCheckbox0);
		ui_interface->addCheckButton("cycle1", &fCheckbox2);
		ui_interface->addCheckButton("cycle2", &fCheckbox3);
		ui_interface->addCheckButton("cycle3", &fCheckbox4);
		ui_interface->addCheckButton("cycle4", &fCheckbox5);
		ui_interface->addCheckButton("cycle5", &fCheckbox6);
		ui_interface->declare(&fHslider0, "unit", "sec");
		ui_interface->addHorizontalSlider("delay0", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider5, "unit", "sec");
		ui_interface->addHorizontalSlider("delay1", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider6, "unit", "sec");
		ui_interface->addHorizontalSlider("delay2", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider7, "unit", "sec");
		ui_interface->addHorizontalSlider("delay3", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider8, "unit", "sec");
		ui_interface->addHorizontalSlider("delay4", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider9, "unit", "sec");
		ui_interface->addHorizontalSlider("delay5", &fHslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("fb.amp", &fHslider2, FAUSTFLOAT(0.999f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("fb1.gain", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("fb2.gain", &fHslider11, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addCheckButton("in1", &fCheckbox1);
		ui_interface->addCheckButton("in2", &fCheckbox7);
		ui_interface->addHorizontalSlider("out1.gain", &fHslider10, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("out2.gain", &fHslider12, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("smooth", &fHslider1, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider4, "unit", "db");
		ui_interface->addHorizontalSlider("threshold", &fHslider4, FAUSTFLOAT(-4e+01f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = fConst1 * float(fCheckbox0);
		float fSlow1 = fConst0 * float(fHslider0);
		float fSlow2 = std::floor(fSlow1);
		float fSlow3 = fSlow2 + (1.0f - fSlow1);
		float fSlow4 = 0.00999f * float(fHslider1);
		float fSlow5 = 1.0f - fSlow4;
		float fSlow6 = float(fCheckbox1) * fSlow5;
		float fSlow7 = float(fHslider2);
		float fSlow8 = float(fHslider3) * fSlow5;
		float fSlow9 = std::pow(1e+01f, 0.05f * float(fHslider4));
		int iSlow10 = int(fSlow1);
		int iSlow11 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow10))));
		float fSlow12 = fSlow1 - fSlow2;
		int iSlow13 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow10 + 1))));
		float fSlow14 = fConst1 * float(fCheckbox2);
		float fSlow15 = fConst0 * float(fHslider5);
		float fSlow16 = std::floor(fSlow15);
		float fSlow17 = fSlow16 + (1.0f - fSlow15);
		int iSlow18 = int(fSlow15);
		int iSlow19 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow18))));
		float fSlow20 = fSlow15 - fSlow16;
		int iSlow21 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow18 + 1))));
		float fSlow22 = fConst1 * float(fCheckbox3);
		float fSlow23 = fConst0 * float(fHslider6);
		float fSlow24 = std::floor(fSlow23);
		float fSlow25 = fSlow24 + (1.0f - fSlow23);
		int iSlow26 = int(fSlow23);
		int iSlow27 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow26))));
		float fSlow28 = fSlow23 - fSlow24;
		int iSlow29 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow26 + 1))));
		float fSlow30 = fConst1 * float(fCheckbox4);
		float fSlow31 = fConst0 * float(fHslider7);
		float fSlow32 = std::floor(fSlow31);
		float fSlow33 = fSlow32 + (1.0f - fSlow31);
		int iSlow34 = int(fSlow31);
		int iSlow35 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow34))));
		float fSlow36 = fSlow31 - fSlow32;
		int iSlow37 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow34 + 1))));
		float fSlow38 = fConst1 * float(fCheckbox5);
		float fSlow39 = fConst0 * float(fHslider8);
		float fSlow40 = std::floor(fSlow39);
		float fSlow41 = fSlow40 + (1.0f - fSlow39);
		int iSlow42 = int(fSlow39);
		int iSlow43 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow42))));
		float fSlow44 = fSlow39 - fSlow40;
		int iSlow45 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow42 + 1))));
		float fSlow46 = fConst1 * float(fCheckbox6);
		float fSlow47 = fConst0 * float(fHslider9);
		float fSlow48 = std::floor(fSlow47);
		float fSlow49 = fSlow48 + (1.0f - fSlow47);
		int iSlow50 = int(fSlow47);
		int iSlow51 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow50))));
		float fSlow52 = fSlow47 - fSlow48;
		int iSlow53 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow50 + 1))));
		float fSlow54 = float(fHslider10) * fSlow5;
		float fSlow55 = float(fCheckbox7) * fSlow5;
		float fSlow56 = float(fHslider11) * fSlow5;
		float fSlow57 = float(fHslider12) * fSlow5;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec1[0] = fSlow0 + fConst2 * fRec1[1];
			fRec2[0] = fSlow6 + fSlow4 * fRec2[1];
			fRec7[0] = fSlow8 + fSlow4 * fRec7[1];
			fRec9[0] = fConst16 * std::fabs(fSlow7 * fRec7[0] * fRec0[1]) + fConst15 * fRec9[1];
			int iTemp0 = fRec9[0] > fSlow9;
			iVec0[0] = iTemp0;
			iRec10[0] = std::max<int>(iConst17 * (iTemp0 < iVec0[1]), iRec10[1] + -1);
			float fTemp1 = std::fabs(std::max<float>(float(iTemp0), float(iRec10[0] > 0)));
			float fTemp2 = ((fTemp1 > fRec8[1]) ? fConst15 : fConst18);
			fRec8[0] = fTemp1 * (1.0f - fTemp2) + fRec8[1] * fTemp2;
			float fTemp3 = fSlow7 * fRec7[0] * fRec8[0] * fRec0[1];
			fVec1[0] = fTemp3;
			fRec6[0] = 0.0f - fConst13 * (fConst14 * fRec6[1] - (fTemp3 + fVec1[1]));
			fRec5[0] = fRec6[0] - fConst10 * (fConst19 * fRec5[2] + fConst20 * fRec5[1]);
			float fTemp4 = fRec5[2] + fRec5[0] + 2.0f * fRec5[1];
			fVec2[0] = fTemp4;
			fRec4[0] = fConst10 * (fConst12 * fTemp4 + fConst21 * fVec2[1]) - fConst22 * fRec4[1];
			fRec3[0] = fRec4[0] - fConst5 * (fConst23 * fRec3[2] + fConst24 * fRec3[1]);
			float fTemp5 = fConst7 * fRec3[0] + fConst25 * fRec3[1] + fConst7 * fRec3[2];
			float fTemp6 = std::fabs(fConst5 * fTemp5);
			float fTemp7 = ((fTemp6 > fRec12[1]) ? fConst29 : fConst28);
			fRec12[0] = fTemp6 * (1.0f - fTemp7) + fRec12[1] * fTemp7;
			fRec11[0] = fConst27 * (0.0f - 0.75f * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec12[0])) + 6.0f, 0.0f)) + fConst26 * fRec11[1];
			float fTemp8 = fRec2[0] * (float(input0[i0]) + fConst5 * fTemp5 * std::pow(1e+01f, 0.05f * fRec11[0]));
			fVec3[IOTA0 & 8388607] = fTemp8;
			fRec13[0] = fSlow14 + fConst2 * fRec13[1];
			fRec14[0] = fSlow22 + fConst2 * fRec14[1];
			fRec15[0] = fSlow30 + fConst2 * fRec15[1];
			fRec16[0] = fSlow38 + fConst2 * fRec16[1];
			fRec17[0] = fSlow46 + fConst2 * fRec17[1];
			fRec0[0] = fRec1[0] * (fSlow3 * fVec3[(IOTA0 - iSlow11) & 8388607] + fSlow12 * fVec3[(IOTA0 - iSlow13) & 8388607]) + fRec13[0] * (fSlow17 * fVec3[(IOTA0 - iSlow19) & 8388607] + fSlow20 * fVec3[(IOTA0 - iSlow21) & 8388607]) + fRec14[0] * (fSlow25 * fVec3[(IOTA0 - iSlow27) & 8388607] + fSlow28 * fVec3[(IOTA0 - iSlow29) & 8388607]) + fRec15[0] * (fSlow33 * fVec3[(IOTA0 - iSlow35) & 8388607] + fSlow36 * fVec3[(IOTA0 - iSlow37) & 8388607]) + fRec16[0] * (fSlow41 * fVec3[(IOTA0 - iSlow43) & 8388607] + fSlow44 * fVec3[(IOTA0 - iSlow45) & 8388607]) + fRec17[0] * (fSlow49 * fVec3[(IOTA0 - iSlow51) & 8388607] + fSlow52 * fVec3[(IOTA0 - iSlow53) & 8388607]);
			fRec18[0] = fSlow54 + fSlow4 * fRec18[1];
			float fTemp9 = fRec0[0] * fRec18[0];
			fRec22[0] = fSlow55 + fSlow4 * fRec22[1];
			fRec27[0] = fSlow56 + fSlow4 * fRec27[1];
			fRec29[0] = fConst16 * std::fabs(fSlow7 * fRec27[0] * fRec21[1]) + fConst15 * fRec29[1];
			int iTemp10 = fRec29[0] > fSlow9;
			iVec4[0] = iTemp10;
			iRec30[0] = std::max<int>(iConst17 * (iTemp10 < iVec4[1]), iRec30[1] + -1);
			float fTemp11 = std::fabs(std::max<float>(float(iTemp10), float(iRec30[0] > 0)));
			float fTemp12 = ((fTemp11 > fRec28[1]) ? fConst15 : fConst18);
			fRec28[0] = fTemp11 * (1.0f - fTemp12) + fRec28[1] * fTemp12;
			float fTemp13 = fSlow7 * fRec27[0] * fRec28[0] * fRec21[1];
			fVec5[0] = fTemp13;
			fRec26[0] = fConst13 * (fTemp13 + fVec5[1] - fConst14 * fRec26[1]);
			fRec25[0] = fRec26[0] - fConst10 * (fConst19 * fRec25[2] + fConst20 * fRec25[1]);
			float fTemp14 = fRec25[2] + fRec25[0] + 2.0f * fRec25[1];
			fVec6[0] = fTemp14;
			fRec24[0] = fConst10 * (fConst12 * fTemp14 + fConst21 * fVec6[1]) - fConst22 * fRec24[1];
			fRec23[0] = fRec24[0] - fConst5 * (fConst23 * fRec23[2] + fConst24 * fRec23[1]);
			float fTemp15 = fConst7 * fRec23[0] + fConst25 * fRec23[1] + fConst7 * fRec23[2];
			float fTemp16 = std::fabs(fConst5 * fTemp15);
			float fTemp17 = ((fTemp16 > fRec32[1]) ? fConst29 : fConst28);
			fRec32[0] = fTemp16 * (1.0f - fTemp17) + fRec32[1] * fTemp17;
			fRec31[0] = fConst27 * (0.0f - 0.75f * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec32[0])) + 6.0f, 0.0f)) + fConst26 * fRec31[1];
			float fTemp18 = fRec22[0] * (float(input1[i0]) + fConst5 * fTemp15 * std::pow(1e+01f, 0.05f * fRec31[0]));
			fVec7[IOTA0 & 8388607] = fTemp18;
			fRec21[0] = fRec1[0] * (fSlow3 * fVec7[(IOTA0 - iSlow11) & 8388607] + fSlow12 * fVec7[(IOTA0 - iSlow13) & 8388607]) + fRec13[0] * (fSlow17 * fVec7[(IOTA0 - iSlow19) & 8388607] + fSlow20 * fVec7[(IOTA0 - iSlow21) & 8388607]) + fRec14[0] * (fSlow25 * fVec7[(IOTA0 - iSlow27) & 8388607] + fSlow28 * fVec7[(IOTA0 - iSlow29) & 8388607]) + fRec15[0] * (fSlow33 * fVec7[(IOTA0 - iSlow35) & 8388607] + fSlow36 * fVec7[(IOTA0 - iSlow37) & 8388607]) + fRec16[0] * (fSlow41 * fVec7[(IOTA0 - iSlow43) & 8388607] + fSlow44 * fVec7[(IOTA0 - iSlow45) & 8388607]) + fRec17[0] * (fSlow49 * fVec7[(IOTA0 - iSlow51) & 8388607] + fSlow52 * fVec7[(IOTA0 - iSlow53) & 8388607]);
			fRec33[0] = fSlow57 + fSlow4 * fRec33[1];
			float fTemp19 = fRec21[0] * fRec33[0];
			float fTemp20 = std::fabs(std::fabs(fTemp9) + std::fabs(fTemp19));
			float fTemp21 = ((fTemp20 > fRec20[1]) ? fConst29 : fConst28);
			fRec20[0] = fTemp20 * (1.0f - fTemp21) + fRec20[1] * fTemp21;
			fRec19[0] = fConst27 * (0.0f - 0.75f * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec20[0])) + 6.0f, 0.0f)) + fConst26 * fRec19[1];
			float fTemp22 = std::pow(1e+01f, 0.05f * fRec19[0]);
			output0[i0] = FAUSTFLOAT(fTemp9 * fTemp22);
			output1[i0] = FAUSTFLOAT(fTemp19 * fTemp22);
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec7[1] = fRec7[0];
			fRec9[1] = fRec9[0];
			iVec0[1] = iVec0[0];
			iRec10[1] = iRec10[0];
			fRec8[1] = fRec8[0];
			fVec1[1] = fVec1[0];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec2[1] = fVec2[0];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			IOTA0 = IOTA0 + 1;
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec0[1] = fRec0[0];
			fRec18[1] = fRec18[0];
			fRec22[1] = fRec22[0];
			fRec27[1] = fRec27[0];
			fRec29[1] = fRec29[0];
			iVec4[1] = iVec4[0];
			iRec30[1] = iRec30[0];
			fRec28[1] = fRec28[0];
			fVec5[1] = fVec5[0];
			fRec26[1] = fRec26[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fVec6[1] = fVec6[0];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec21[1] = fRec21[0];
			fRec33[1] = fRec33[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _piece_stok_solo_UI : public UI {
    static std::string name;
};

template <class T>
std::string _piece_stok_solo_UI<T>::name(sym(piece_stok_solo));

typedef _piece_stok_solo_UI<piece_stok_solo> piece_stok_solo_UI;

class faust_piece_stok_solo_tilde : public FaustExternal<piece_stok_solo, piece_stok_solo_UI> {
public:
    faust_piece_stok_solo_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
