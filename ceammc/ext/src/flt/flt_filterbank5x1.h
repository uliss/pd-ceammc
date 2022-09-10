/* ------------------------------------------------------------
name: "flt.fbank5x1"
Code generated with Faust 2.44.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn flt_filterbank5x1 -scn flt_filterbank5x1_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __flt_filterbank5x1_H__
#define  __flt_filterbank5x1_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN flt_filterbank5x1_dsp.h ********************************
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

class FAUST_API flt_filterbank5x1_dsp {

    public:

        flt_filterbank5x1_dsp() {}
        virtual ~flt_filterbank5x1_dsp() {}

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
        virtual flt_filterbank5x1_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public flt_filterbank5x1_dsp {

    protected:

        flt_filterbank5x1_dsp* fDSP;

    public:

        decorator_dsp(flt_filterbank5x1_dsp* flt_filterbank5x1_dsp = nullptr):fDSP(flt_filterbank5x1_dsp) {}
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
    
        virtual flt_filterbank5x1_dsp* createDSPInstance() = 0;
    
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

/************************** END flt_filterbank5x1_dsp.h **************************/
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
 The base class of Meta handler to be used in flt_filterbank5x1_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct flt_filterbank5x1 : public flt_filterbank5x1_dsp {
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
#define FAUSTCLASS flt_filterbank5x1
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

static float flt_filterbank5x1_faustpower2_f(float value) {
	return value * value;
}

class flt_filterbank5x1 : public flt_filterbank5x1_dsp {
	
 private:
	
	int fSampleRate;
	float fConst3;
	float fConst4;
	FAUSTFLOAT fVslider0;
	float fConst5;
	float fRec0[2];
	float fConst7;
	float fConst10;
	float fConst12;
	float fConst13;
	float fConst16;
	float fConst18;
	float fConst19;
	float fConst21;
	float fConst22;
	float fConst24;
	float fConst25;
	float fVec0[2];
	float fRec8[2];
	float fConst26;
	float fConst28;
	float fConst29;
	float fRec7[3];
	float fVec1[2];
	float fRec6[2];
	float fConst30;
	float fConst32;
	float fConst33;
	float fRec5[3];
	float fVec2[2];
	float fRec4[2];
	float fConst34;
	float fConst36;
	float fConst37;
	float fRec3[3];
	float fVec3[2];
	float fConst38;
	float fRec2[2];
	float fConst39;
	float fConst41;
	float fConst42;
	float fRec1[3];
	FAUSTFLOAT fVslider1;
	float fRec9[2];
	float fConst43;
	float fConst44;
	float fConst45;
	float fRec11[2];
	float fRec10[3];
	float fConst46;
	FAUSTFLOAT fVslider2;
	float fRec12[2];
	float fConst47;
	float fConst48;
	float fConst49;
	float fRec15[2];
	float fRec14[3];
	float fConst50;
	float fConst51;
	float fConst52;
	float fRec13[3];
	FAUSTFLOAT fVslider3;
	float fRec16[2];
	float fConst53;
	float fConst54;
	float fConst55;
	float fRec20[2];
	float fRec19[3];
	float fConst56;
	float fRec18[3];
	float fConst57;
	float fConst58;
	float fRec17[3];
	FAUSTFLOAT fVslider4;
	float fRec21[2];
	float fConst59;
	float fRec26[2];
	float fRec25[3];
	float fConst60;
	float fRec24[3];
	float fRec23[3];
	float fConst61;
	float fConst62;
	float fRec22[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.2");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn flt_filterbank5x1 -scn flt_filterbank5x1_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "flt_filterbank5x1.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass_plus_lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass_plus_lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass_plus_lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/mth_octave_filterbank:author", "Julius O. Smith III");
		m->declare("filters.lib/mth_octave_filterbank:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/mth_octave_filterbank:license", "MIT-style STK-4.3 license");
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
		m->declare("name", "flt.fbank5x1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 5;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::tan(1570.79639f / fConst0);
		float fConst2 = 1.0f / fConst1;
		fConst3 = 1.0f / ((fConst2 + 1.0f) / fConst1 + 1.0f);
		fConst4 = 44.0999985f / fConst0;
		fConst5 = 1.0f - fConst4;
		float fConst6 = fConst2 + 1.0f;
		fConst7 = 1.0f / fConst6;
		float fConst8 = std::tan(3141.59277f / fConst0);
		float fConst9 = 1.0f / fConst8;
		fConst10 = 1.0f / ((fConst9 + 1.0f) / fConst8 + 1.0f);
		float fConst11 = fConst9 + 1.0f;
		fConst12 = 1.0f / fConst11;
		fConst13 = 1.0f - fConst9;
		float fConst14 = std::tan(6283.18555f / fConst0);
		float fConst15 = 1.0f / fConst14;
		fConst16 = 1.0f / ((fConst15 + 1.0f) / fConst14 + 1.0f);
		float fConst17 = fConst15 + 1.0f;
		fConst18 = 1.0f / fConst17;
		fConst19 = 1.0f - fConst15;
		float fConst20 = std::tan(12566.3711f / fConst0);
		fConst21 = 1.0f / fConst20;
		fConst22 = 1.0f / ((fConst21 + 1.0f) / fConst20 + 1.0f);
		float fConst23 = fConst21 + 1.0f;
		fConst24 = 1.0f / fConst23;
		fConst25 = 1.0f - fConst21;
		fConst26 = (fConst21 + -1.0f) / fConst20 + 1.0f;
		float fConst27 = flt_filterbank5x1_faustpower2_f(fConst20);
		fConst28 = 1.0f / fConst27;
		fConst29 = 2.0f * (1.0f - fConst28);
		fConst30 = (fConst15 + -1.0f) / fConst14 + 1.0f;
		float fConst31 = flt_filterbank5x1_faustpower2_f(fConst14);
		fConst32 = 1.0f / fConst31;
		fConst33 = 2.0f * (1.0f - fConst32);
		fConst34 = (fConst9 + -1.0f) / fConst8 + 1.0f;
		float fConst35 = flt_filterbank5x1_faustpower2_f(fConst8);
		fConst36 = 1.0f / fConst35;
		fConst37 = 2.0f * (1.0f - fConst36);
		fConst38 = 1.0f - fConst2;
		fConst39 = (fConst2 + -1.0f) / fConst1 + 1.0f;
		float fConst40 = flt_filterbank5x1_faustpower2_f(fConst1);
		fConst41 = 1.0f / fConst40;
		fConst42 = 2.0f * (1.0f - fConst41);
		fConst43 = 1.0f / (fConst1 * fConst6);
		fConst44 = 0.0f - fConst43;
		fConst45 = fConst38 / fConst6;
		fConst46 = 0.0f - 2.0f / fConst40;
		fConst47 = 1.0f / (fConst8 * fConst11);
		fConst48 = 0.0f - fConst47;
		fConst49 = fConst13 / fConst11;
		fConst50 = 0.0f - 2.0f / fConst35;
		fConst51 = 1.0f / (fConst6 / fConst1 + 1.0f);
		fConst52 = 1.0f - fConst38 / fConst1;
		fConst53 = 1.0f / (fConst14 * fConst17);
		fConst54 = 0.0f - fConst53;
		fConst55 = fConst19 / fConst17;
		fConst56 = 0.0f - 2.0f / fConst31;
		fConst57 = 1.0f / (fConst11 / fConst8 + 1.0f);
		fConst58 = 1.0f - fConst13 / fConst8;
		fConst59 = 0.0f - 1.0f / (fConst20 * fConst23);
		fConst60 = 0.0f - 2.0f / fConst27;
		fConst61 = 1.0f / (fConst17 / fConst14 + 1.0f);
		fConst62 = 1.0f - fConst19 / fConst14;
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fVslider2 = FAUSTFLOAT(0.0f);
		fVslider3 = FAUSTFLOAT(0.0f);
		fVslider4 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fVec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec8[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3; l3 = l3 + 1) {
			fRec7[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec6[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec2[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec3[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fVec3[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec2[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec1[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec9[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fRec10[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec12[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec14[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = l19 + 1) {
			fRec13[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec16[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec20[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec19[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec18[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec17[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec21[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec26[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec25[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 3; l28 = l28 + 1) {
			fRec24[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 3; l29 = l29 + 1) {
			fRec23[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec22[l30] = 0.0f;
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
	
	virtual flt_filterbank5x1* clone() {
		return new flt_filterbank5x1();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("flt.fbank5x1");
		ui_interface->declare(&fVslider2, "unit", "db");
		ui_interface->addVerticalSlider("f1000", &fVslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->declare(&fVslider3, "unit", "db");
		ui_interface->addVerticalSlider("f2000", &fVslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->declare(&fVslider0, "unit", "db");
		ui_interface->addVerticalSlider("f250", &fVslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->declare(&fVslider4, "unit", "db");
		ui_interface->addVerticalSlider("f4000", &fVslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->declare(&fVslider1, "unit", "db");
		ui_interface->addVerticalSlider("f500", &fVslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		FAUSTFLOAT* output2 = outputs[2];
		FAUSTFLOAT* output3 = outputs[3];
		FAUSTFLOAT* output4 = outputs[4];
		float fSlow0 = fConst4 * std::pow(10.0f, 0.0500000007f * float(fVslider0));
		float fSlow1 = fConst4 * std::pow(10.0f, 0.0500000007f * float(fVslider1));
		float fSlow2 = fConst4 * std::pow(10.0f, 0.0500000007f * float(fVslider2));
		float fSlow3 = fConst4 * std::pow(10.0f, 0.0500000007f * float(fVslider3));
		float fSlow4 = fConst4 * std::pow(10.0f, 0.0500000007f * float(fVslider4));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec0[0] = fSlow0 + fConst5 * fRec0[1];
			float fTemp0 = float(input0[i0]);
			fVec0[0] = fTemp0;
			fRec8[0] = 0.0f - fConst24 * (fConst25 * fRec8[1] - (fTemp0 + fVec0[1]));
			fRec7[0] = fRec8[0] - fConst22 * (fConst26 * fRec7[2] + fConst29 * fRec7[1]);
			float fTemp1 = fRec7[2] + fRec7[0] + 2.0f * fRec7[1];
			fVec1[0] = fTemp1;
			fRec6[0] = 0.0f - fConst18 * (fConst19 * fRec6[1] - fConst22 * (fTemp1 + fVec1[1]));
			fRec5[0] = fRec6[0] - fConst16 * (fConst30 * fRec5[2] + fConst33 * fRec5[1]);
			float fTemp2 = fRec5[2] + fRec5[0] + 2.0f * fRec5[1];
			fVec2[0] = fTemp2;
			fRec4[0] = 0.0f - fConst12 * (fConst13 * fRec4[1] - fConst16 * (fTemp2 + fVec2[1]));
			fRec3[0] = fRec4[0] - fConst10 * (fConst34 * fRec3[2] + fConst37 * fRec3[1]);
			float fTemp3 = fRec3[2] + fRec3[0] + 2.0f * fRec3[1];
			fVec3[0] = fTemp3;
			fRec2[0] = fConst7 * (fConst10 * (fTemp3 + fVec3[1]) - fConst38 * fRec2[1]);
			fRec1[0] = fRec2[0] - fConst3 * (fConst39 * fRec1[2] + fConst42 * fRec1[1]);
			output0[i0] = FAUSTFLOAT(fConst3 * fRec0[0] * (fRec1[2] + fRec1[0] + 2.0f * fRec1[1]));
			fRec9[0] = fSlow1 + fConst5 * fRec9[1];
			fRec11[0] = fConst10 * (fConst43 * fTemp3 + fConst44 * fVec3[1]) - fConst45 * fRec11[1];
			fRec10[0] = fRec11[0] - fConst3 * (fConst39 * fRec10[2] + fConst42 * fRec10[1]);
			output1[i0] = FAUSTFLOAT(fConst3 * fRec9[0] * (fConst41 * fRec10[0] + fConst46 * fRec10[1] + fConst41 * fRec10[2]));
			fRec12[0] = fSlow2 + fConst5 * fRec12[1];
			fRec15[0] = fConst16 * (fConst47 * fTemp2 + fConst48 * fVec2[1]) - fConst49 * fRec15[1];
			fRec14[0] = fRec15[0] - fConst10 * (fConst34 * fRec14[2] + fConst37 * fRec14[1]);
			float fTemp4 = fConst42 * fRec13[1];
			fRec13[0] = fConst10 * (fConst36 * fRec14[0] + fConst50 * fRec14[1] + fConst36 * fRec14[2]) - fConst51 * (fConst52 * fRec13[2] + fTemp4);
			output2[i0] = FAUSTFLOAT(fRec12[0] * (fRec13[2] + fConst51 * (fTemp4 + fConst52 * fRec13[0])));
			fRec16[0] = fSlow3 + fConst5 * fRec16[1];
			fRec20[0] = fConst22 * (fConst53 * fTemp1 + fConst54 * fVec1[1]) - fConst55 * fRec20[1];
			fRec19[0] = fRec20[0] - fConst16 * (fConst30 * fRec19[2] + fConst33 * fRec19[1]);
			float fTemp5 = fConst42 * fRec18[1];
			fRec18[0] = fConst16 * (fConst32 * fRec19[0] + fConst56 * fRec19[1] + fConst32 * fRec19[2]) - fConst51 * (fConst52 * fRec18[2] + fTemp5);
			float fTemp6 = fConst37 * fRec17[1];
			fRec17[0] = (fRec18[2] + fConst51 * (fTemp5 + fConst52 * fRec18[0])) - fConst57 * (fConst58 * fRec17[2] + fTemp6);
			output3[i0] = FAUSTFLOAT(fRec16[0] * (fRec17[2] + fConst57 * (fTemp6 + fConst58 * fRec17[0])));
			fRec21[0] = fSlow4 + fConst5 * fRec21[1];
			fRec26[0] = fConst59 * fVec0[1] - fConst24 * (fConst25 * fRec26[1] - fConst21 * fTemp0);
			fRec25[0] = fRec26[0] - fConst22 * (fConst26 * fRec25[2] + fConst29 * fRec25[1]);
			float fTemp7 = fConst42 * fRec24[1];
			fRec24[0] = fConst22 * (fConst28 * fRec25[0] + fConst60 * fRec25[1] + fConst28 * fRec25[2]) - fConst51 * (fConst52 * fRec24[2] + fTemp7);
			float fTemp8 = fConst37 * fRec23[1];
			fRec23[0] = (fRec24[2] + fConst51 * (fTemp7 + fConst52 * fRec24[0])) - fConst57 * (fConst58 * fRec23[2] + fTemp8);
			float fTemp9 = fConst33 * fRec22[1];
			fRec22[0] = (fRec23[2] + fConst57 * (fTemp8 + fConst58 * fRec23[0])) - fConst61 * (fConst62 * fRec22[2] + fTemp9);
			output4[i0] = FAUSTFLOAT(fRec21[0] * (fRec22[2] + fConst61 * (fTemp9 + fConst62 * fRec22[0])));
			fRec0[1] = fRec0[0];
			fVec0[1] = fVec0[0];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fVec1[1] = fVec1[0];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec2[1] = fVec2[0];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fVec3[1] = fVec3[0];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec9[1] = fRec9[0];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec12[1] = fRec12[0];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec16[1] = fRec16[0];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec21[1] = fRec21[0];
			fRec26[1] = fRec26[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _flt_filterbank5x1_UI : public UI {
    static std::string name;
};

template <class T>
std::string _flt_filterbank5x1_UI<T>::name(sym(flt_filterbank5x1));

typedef _flt_filterbank5x1_UI<flt_filterbank5x1> flt_filterbank5x1_UI;

class faust_flt_filterbank5x1_tilde : public FaustExternal<flt_filterbank5x1, flt_filterbank5x1_UI> {
public:
    faust_flt_filterbank5x1_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
