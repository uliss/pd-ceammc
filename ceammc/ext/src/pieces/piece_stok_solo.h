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
	float fRec2[2];
	float fConst5;
	float fConst7;
	float fConst10;
	float fConst12;
	float fConst13;
	float fConst14;
	FAUSTFLOAT fHslider1;
	float fRec7[2];
	float fVec0[2];
	float fRec6[2];
	float fConst15;
	float fConst16;
	float fRec5[3];
	float fVec1[2];
	float fConst17;
	float fConst18;
	float fRec4[2];
	float fConst19;
	float fConst20;
	float fRec3[3];
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	float fConst25;
	float fRec9[2];
	float fRec8[2];
	int IOTA0;
	float fVec2[8388608];
	FAUSTFLOAT fCheckbox2;
	float fRec10[2];
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fCheckbox3;
	float fRec11[2];
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fCheckbox4;
	float fRec12[2];
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fCheckbox5;
	float fRec13[2];
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fCheckbox6;
	float fRec14[2];
	FAUSTFLOAT fHslider6;
	float fRec0[2];
	FAUSTFLOAT fHslider7;
	float fRec15[2];
	FAUSTFLOAT fCheckbox7;
	float fRec19[2];
	FAUSTFLOAT fHslider8;
	float fRec24[2];
	float fVec3[2];
	float fRec23[2];
	float fRec22[3];
	float fVec4[2];
	float fRec21[2];
	float fRec20[3];
	float fRec26[2];
	float fRec25[2];
	float fVec5[8388608];
	float fRec18[2];
	FAUSTFLOAT fHslider9;
	float fRec27[2];
	float fRec17[2];
	float fRec16[2];
	
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
		fConst15 = (fConst9 + -1.0f) / fConst8 + 1.0f;
		fConst16 = 2.0f * (1.0f - 1.0f / piece_stok_solo_faustpower2_f(fConst8));
		fConst17 = 0.0f - fConst12;
		fConst18 = (1.0f - fConst4) / fConst11;
		fConst19 = (fConst4 + -1.0f) / fConst3 + 1.0f;
		fConst20 = 2.0f * (1.0f - fConst7);
		fConst21 = 0.0f - 2.0f / fConst6;
		fConst22 = std::exp(0.0f - 2.5e+03f / fConst0);
		fConst23 = 1.0f - fConst22;
		fConst24 = std::exp(0.0f - 2.0f / fConst0);
		fConst25 = std::exp(0.0f - 1.25e+03f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fCheckbox2 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.0f);
		fCheckbox3 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fCheckbox4 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fCheckbox5 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fCheckbox6 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fCheckbox7 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.0f);
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
			fVec0[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec6[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = l5 + 1) {
			fRec5[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fVec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec4[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec3[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec9[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec8[l10] = 0.0f;
		}
		IOTA0 = 0;
		for (int l11 = 0; l11 < 8388608; l11 = l11 + 1) {
			fVec2[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec10[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec0[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec15[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec19[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec24[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fVec3[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec23[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec22[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fVec4[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec21[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec20[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec26[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec25[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 8388608; l29 = l29 + 1) {
			fVec5[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec18[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec27[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec17[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec16[l33] = 0.0f;
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
		ui_interface->addCheckButton("cycle0", &fCheckbox2);
		ui_interface->addCheckButton("cycle1", &fCheckbox0);
		ui_interface->addCheckButton("cycle2", &fCheckbox3);
		ui_interface->addCheckButton("cycle3", &fCheckbox4);
		ui_interface->addCheckButton("cycle4", &fCheckbox5);
		ui_interface->addCheckButton("cycle5", &fCheckbox6);
		ui_interface->declare(&fHslider2, "unit", "sec");
		ui_interface->addHorizontalSlider("delay0", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider0, "unit", "sec");
		ui_interface->addHorizontalSlider("delay1", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider3, "unit", "sec");
		ui_interface->addHorizontalSlider("delay2", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider4, "unit", "sec");
		ui_interface->addHorizontalSlider("delay3", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider5, "unit", "sec");
		ui_interface->addHorizontalSlider("delay4", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider6, "unit", "sec");
		ui_interface->addHorizontalSlider("delay5", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4377599.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("fb1.gain", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("fb2.gain", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addCheckButton("in1", &fCheckbox1);
		ui_interface->addCheckButton("in2", &fCheckbox7);
		ui_interface->addHorizontalSlider("out1.gain", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("out2.gain", &fHslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
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
		float fSlow4 = fConst1 * float(fCheckbox1);
		float fSlow5 = fConst1 * float(fHslider1);
		int iSlow6 = int(fSlow1);
		int iSlow7 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow6))));
		float fSlow8 = fSlow1 - fSlow2;
		int iSlow9 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow6 + 1))));
		float fSlow10 = fConst1 * float(fCheckbox2);
		float fSlow11 = fConst0 * float(fHslider2);
		float fSlow12 = std::floor(fSlow11);
		float fSlow13 = fSlow12 + (1.0f - fSlow11);
		int iSlow14 = int(fSlow11);
		int iSlow15 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow14))));
		float fSlow16 = fSlow11 - fSlow12;
		int iSlow17 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow14 + 1))));
		float fSlow18 = fConst1 * float(fCheckbox3);
		float fSlow19 = fConst0 * float(fHslider3);
		float fSlow20 = std::floor(fSlow19);
		float fSlow21 = fSlow20 + (1.0f - fSlow19);
		int iSlow22 = int(fSlow19);
		int iSlow23 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow22))));
		float fSlow24 = fSlow19 - fSlow20;
		int iSlow25 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow22 + 1))));
		float fSlow26 = fConst1 * float(fCheckbox4);
		float fSlow27 = fConst0 * float(fHslider4);
		float fSlow28 = std::floor(fSlow27);
		float fSlow29 = fSlow28 + (1.0f - fSlow27);
		int iSlow30 = int(fSlow27);
		int iSlow31 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow30))));
		float fSlow32 = fSlow27 - fSlow28;
		int iSlow33 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow30 + 1))));
		float fSlow34 = fConst1 * float(fCheckbox5);
		float fSlow35 = fConst0 * float(fHslider5);
		float fSlow36 = std::floor(fSlow35);
		float fSlow37 = fSlow36 + (1.0f - fSlow35);
		int iSlow38 = int(fSlow35);
		int iSlow39 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow38))));
		float fSlow40 = fSlow35 - fSlow36;
		int iSlow41 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow38 + 1))));
		float fSlow42 = fConst1 * float(fCheckbox6);
		float fSlow43 = fConst0 * float(fHslider6);
		float fSlow44 = std::floor(fSlow43);
		float fSlow45 = fSlow44 + (1.0f - fSlow43);
		int iSlow46 = int(fSlow43);
		int iSlow47 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow46))));
		float fSlow48 = fSlow43 - fSlow44;
		int iSlow49 = int(std::min<float>(4377601.0f, float(std::max<int>(0, iSlow46 + 1))));
		float fSlow50 = fConst1 * float(fHslider7);
		float fSlow51 = fConst1 * float(fCheckbox7);
		float fSlow52 = fConst1 * float(fHslider8);
		float fSlow53 = fConst1 * float(fHslider9);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec1[0] = fSlow0 + fConst2 * fRec1[1];
			fRec2[0] = fSlow4 + fConst2 * fRec2[1];
			fRec7[0] = fSlow5 + fConst2 * fRec7[1];
			float fTemp0 = fRec7[0] * fRec0[1];
			fVec0[0] = fTemp0;
			fRec6[0] = 0.0f - fConst13 * (fConst14 * fRec6[1] - (fTemp0 + fVec0[1]));
			fRec5[0] = fRec6[0] - fConst10 * (fConst15 * fRec5[2] + fConst16 * fRec5[1]);
			float fTemp1 = fRec5[2] + fRec5[0] + 2.0f * fRec5[1];
			fVec1[0] = fTemp1;
			fRec4[0] = fConst10 * (fConst12 * fTemp1 + fConst17 * fVec1[1]) - fConst18 * fRec4[1];
			fRec3[0] = fRec4[0] - fConst5 * (fConst19 * fRec3[2] + fConst20 * fRec3[1]);
			float fTemp2 = fConst7 * fRec3[0] + fConst21 * fRec3[1] + fConst7 * fRec3[2];
			float fTemp3 = std::fabs(fConst5 * fTemp2);
			float fTemp4 = ((fTemp3 > fRec9[1]) ? fConst25 : fConst24);
			fRec9[0] = fTemp3 * (1.0f - fTemp4) + fRec9[1] * fTemp4;
			fRec8[0] = fConst23 * (0.0f - 0.75f * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec9[0])) + 6.0f, 0.0f)) + fConst22 * fRec8[1];
			float fTemp5 = fRec2[0] * (float(input0[i0]) + fConst5 * fTemp2 * std::pow(1e+01f, 0.05f * fRec8[0]));
			fVec2[IOTA0 & 8388607] = fTemp5;
			fRec10[0] = fSlow10 + fConst2 * fRec10[1];
			fRec11[0] = fSlow18 + fConst2 * fRec11[1];
			fRec12[0] = fSlow26 + fConst2 * fRec12[1];
			fRec13[0] = fSlow34 + fConst2 * fRec13[1];
			fRec14[0] = fSlow42 + fConst2 * fRec14[1];
			fRec0[0] = fRec1[0] * (fSlow3 * fVec2[(IOTA0 - iSlow7) & 8388607] + fSlow8 * fVec2[(IOTA0 - iSlow9) & 8388607]) + fRec10[0] * (fSlow13 * fVec2[(IOTA0 - iSlow15) & 8388607] + fSlow16 * fVec2[(IOTA0 - iSlow17) & 8388607]) + fRec11[0] * (fSlow21 * fVec2[(IOTA0 - iSlow23) & 8388607] + fSlow24 * fVec2[(IOTA0 - iSlow25) & 8388607]) + fRec12[0] * (fSlow29 * fVec2[(IOTA0 - iSlow31) & 8388607] + fSlow32 * fVec2[(IOTA0 - iSlow33) & 8388607]) + fRec13[0] * (fSlow37 * fVec2[(IOTA0 - iSlow39) & 8388607] + fSlow40 * fVec2[(IOTA0 - iSlow41) & 8388607]) + fRec14[0] * (fSlow45 * fVec2[(IOTA0 - iSlow47) & 8388607] + fSlow48 * fVec2[(IOTA0 - iSlow49) & 8388607]);
			fRec15[0] = fSlow50 + fConst2 * fRec15[1];
			float fTemp6 = fRec0[0] * fRec15[0];
			fRec19[0] = fSlow51 + fConst2 * fRec19[1];
			fRec24[0] = fSlow52 + fConst2 * fRec24[1];
			float fTemp7 = fRec24[0] * fRec18[1];
			fVec3[0] = fTemp7;
			fRec23[0] = 0.0f - fConst13 * (fConst14 * fRec23[1] - (fTemp7 + fVec3[1]));
			fRec22[0] = fRec23[0] - fConst10 * (fConst15 * fRec22[2] + fConst16 * fRec22[1]);
			float fTemp8 = fRec22[2] + fRec22[0] + 2.0f * fRec22[1];
			fVec4[0] = fTemp8;
			fRec21[0] = fConst10 * (fConst12 * fTemp8 + fConst17 * fVec4[1]) - fConst18 * fRec21[1];
			fRec20[0] = fRec21[0] - fConst5 * (fConst19 * fRec20[2] + fConst20 * fRec20[1]);
			float fTemp9 = fConst7 * fRec20[0] + fConst21 * fRec20[1] + fConst7 * fRec20[2];
			float fTemp10 = std::fabs(fConst5 * fTemp9);
			float fTemp11 = ((fTemp10 > fRec26[1]) ? fConst25 : fConst24);
			fRec26[0] = fTemp10 * (1.0f - fTemp11) + fRec26[1] * fTemp11;
			fRec25[0] = fConst23 * (0.0f - 0.75f * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec26[0])) + 6.0f, 0.0f)) + fConst22 * fRec25[1];
			float fTemp12 = fRec19[0] * (float(input1[i0]) + fConst5 * fTemp9 * std::pow(1e+01f, 0.05f * fRec25[0]));
			fVec5[IOTA0 & 8388607] = fTemp12;
			fRec18[0] = fRec13[0] * (fSlow37 * fVec5[(IOTA0 - iSlow39) & 8388607] + fSlow40 * fVec5[(IOTA0 - iSlow41) & 8388607]) + fRec12[0] * (fSlow29 * fVec5[(IOTA0 - iSlow31) & 8388607] + fSlow32 * fVec5[(IOTA0 - iSlow33) & 8388607]) + fRec11[0] * (fSlow21 * fVec5[(IOTA0 - iSlow23) & 8388607] + fSlow24 * fVec5[(IOTA0 - iSlow25) & 8388607]) + fRec1[0] * (fSlow3 * fVec5[(IOTA0 - iSlow7) & 8388607] + fSlow8 * fVec5[(IOTA0 - iSlow9) & 8388607]) + fRec10[0] * (fSlow13 * fVec5[(IOTA0 - iSlow15) & 8388607] + fSlow16 * fVec5[(IOTA0 - iSlow17) & 8388607]) + fRec14[0] * (fSlow45 * fVec5[(IOTA0 - iSlow47) & 8388607] + fSlow48 * fVec5[(IOTA0 - iSlow49) & 8388607]);
			fRec27[0] = fSlow53 + fConst2 * fRec27[1];
			float fTemp13 = fRec18[0] * fRec27[0];
			float fTemp14 = std::fabs(std::fabs(fTemp6) + std::fabs(fTemp13));
			float fTemp15 = ((fTemp14 > fRec17[1]) ? fConst25 : fConst24);
			fRec17[0] = fTemp14 * (1.0f - fTemp15) + fRec17[1] * fTemp15;
			fRec16[0] = fConst23 * (0.0f - 0.75f * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec17[0])) + 6.0f, 0.0f)) + fConst22 * fRec16[1];
			float fTemp16 = std::pow(1e+01f, 0.05f * fRec16[0]);
			output0[i0] = FAUSTFLOAT(fTemp6 * fTemp16);
			output1[i0] = FAUSTFLOAT(fTemp13 * fTemp16);
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec7[1] = fRec7[0];
			fVec0[1] = fVec0[0];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec1[1] = fVec1[0];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			IOTA0 = IOTA0 + 1;
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec0[1] = fRec0[0];
			fRec15[1] = fRec15[0];
			fRec19[1] = fRec19[0];
			fRec24[1] = fRec24[0];
			fVec3[1] = fVec3[0];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fVec4[1] = fVec4[0];
			fRec21[1] = fRec21[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec26[1] = fRec26[0];
			fRec25[1] = fRec25[0];
			fRec18[1] = fRec18[0];
			fRec27[1] = fRec27[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
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
