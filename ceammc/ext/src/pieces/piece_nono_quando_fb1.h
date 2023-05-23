/* ------------------------------------------------------------
name: "nono.quando.fb1"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn piece_nono_quando_fb1 -scn piece_nono_quando_fb1_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __piece_nono_quando_fb1_H__
#define  __piece_nono_quando_fb1_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN piece_nono_quando_fb1_dsp.h ********************************
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

class FAUST_API piece_nono_quando_fb1_dsp {

    public:

        piece_nono_quando_fb1_dsp() {}
        virtual ~piece_nono_quando_fb1_dsp() {}

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
        virtual piece_nono_quando_fb1_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public piece_nono_quando_fb1_dsp {

    protected:

        piece_nono_quando_fb1_dsp* fDSP;

    public:

        decorator_dsp(piece_nono_quando_fb1_dsp* piece_nono_quando_fb1_dsp = nullptr):fDSP(piece_nono_quando_fb1_dsp) {}
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
    
        virtual piece_nono_quando_fb1_dsp* createDSPInstance() = 0;
    
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

/************************** END piece_nono_quando_fb1_dsp.h **************************/
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
 The base class of Meta handler to be used in piece_nono_quando_fb1_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct piece_nono_quando_fb1 : public piece_nono_quando_fb1_dsp {
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
#define FAUSTCLASS piece_nono_quando_fb1
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

static float piece_nono_quando_fb1_faustpower2_f(float value) {
	return value * value;
}
static float piece_nono_quando_fb1_faustpower4_f(float value) {
	return value * value * value * value;
}
static float piece_nono_quando_fb1_faustpower3_f(float value) {
	return value * value * value;
}

class piece_nono_quando_fb1 : public piece_nono_quando_fb1_dsp {
	
 private:
	
	int fSampleRate;
	float fConst2;
	float fConst10;
	float fConst18;
	float fConst20;
	float fConst24;
	float fConst28;
	float fConst29;
	float fConst30;
	float fConst31;
	float fRec2[3];
	float fConst32;
	float fConst33;
	float fConst37;
	float fConst38;
	float fConst39;
	float fConst40;
	float fRec1[5];
	float fConst43;
	float fConst44;
	float fConst45;
	float fRec0[5];
	float fConst53;
	float fConst57;
	float fConst59;
	float fConst62;
	float fConst66;
	float fConst67;
	float fConst68;
	float fConst69;
	float fRec5[3];
	float fConst70;
	float fConst71;
	float fConst74;
	float fConst75;
	float fConst76;
	float fConst77;
	float fRec4[5];
	float fConst79;
	float fConst80;
	float fConst81;
	float fRec3[5];
	float fConst88;
	float fConst93;
	float fConst95;
	float fConst99;
	float fConst103;
	float fConst104;
	float fConst105;
	float fConst106;
	float fRec8[3];
	float fConst107;
	float fConst108;
	float fConst111;
	float fConst112;
	float fConst113;
	float fConst114;
	float fRec7[5];
	float fConst116;
	float fConst117;
	float fConst118;
	float fRec6[5];
	float fConst126;
	float fConst130;
	float fConst132;
	float fConst135;
	float fConst139;
	float fConst140;
	float fConst141;
	float fConst142;
	float fRec11[3];
	float fConst143;
	float fConst144;
	float fConst147;
	float fConst148;
	float fConst149;
	float fConst150;
	float fRec10[5];
	float fConst152;
	float fConst153;
	float fConst154;
	float fRec9[5];
	float fConst162;
	float fConst166;
	float fConst168;
	float fConst171;
	float fConst175;
	float fConst176;
	float fConst177;
	float fConst178;
	float fRec14[3];
	float fConst179;
	float fConst180;
	float fConst183;
	float fConst184;
	float fConst185;
	float fConst186;
	float fRec13[5];
	float fConst188;
	float fConst189;
	float fConst190;
	float fRec12[5];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn piece_nono_quando_fb1 -scn piece_nono_quando_fb1_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "piece_nono_quando_fb1.dsp");
		m->declare("filters.lib/bandpass0_bandstop1:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass0_bandstop1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass0_bandstop1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1sb:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1sb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1sb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2sb:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2sb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2sb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "nono.quando.fb1");
		m->declare("piece_nono_quando.dsp/name", "nono.quando");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
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
		float fConst1 = 1.0f / fConst0;
		fConst2 = piece_nono_quando_fb1_faustpower2_f(fConst1);
		float fConst3 = std::tan(8523.141f / fConst0);
		float fConst4 = fConst0 * fConst3;
		float fConst5 = piece_nono_quando_fb1_faustpower2_f(fConst0);
		float fConst6 = std::sqrt(4.0f * fConst5 * std::tan(3194.9998f / fConst0) * fConst3);
		float fConst7 = piece_nono_quando_fb1_faustpower2_f(fConst6);
		float fConst8 = 2.0f * fConst4 - 0.5f * (fConst7 / fConst4);
		float fConst9 = piece_nono_quando_fb1_faustpower2_f(fConst8);
		fConst10 = 4.0f * fConst9;
		float fConst11 = fConst2 * (fConst10 + 8.0f * fConst7);
		float fConst12 = piece_nono_quando_fb1_faustpower4_f(fConst1);
		float fConst13 = fConst12 * piece_nono_quando_fb1_faustpower4_f(fConst6);
		float fConst14 = 4.944272f / fConst0;
		float fConst15 = piece_nono_quando_fb1_faustpower3_f(fConst1);
		float fConst16 = fConst15 * fConst7;
		float fConst17 = fConst14 + 1.236068f * fConst16;
		fConst18 = 1.0f / (fConst11 + fConst13 + fConst8 * fConst17 + 16.0f);
		float fConst19 = 8.0f * fConst9;
		fConst20 = 0.0f - fConst19;
		float fConst21 = 12.944272f / fConst0;
		float fConst22 = fConst21 + 3.236068f * fConst16;
		float fConst23 = fConst11 + fConst13 + fConst8 * fConst22 + 16.0f;
		fConst24 = fConst2 / fConst23;
		float fConst25 = fConst2 * fConst7;
		float fConst26 = 2.0f * (fConst8 / fConst0);
		float fConst27 = fConst25 + fConst26 + 4.0f;
		fConst28 = 2.0f * (fConst8 / (fConst0 * fConst27));
		fConst29 = 1.0f / fConst27;
		fConst30 = 2.0f * fConst25 + -8.0f;
		fConst31 = fConst25 + (4.0f - fConst26);
		fConst32 = 0.0f - fConst28;
		fConst33 = 1.0f / fConst23;
		float fConst34 = 4.0f * fConst13;
		float fConst35 = 6.472136f * fConst16;
		float fConst36 = 25.888544f / fConst0;
		fConst37 = fConst34 + fConst8 * (fConst35 - fConst36) + -64.0f;
		fConst38 = 6.0f * fConst13 + fConst2 * (0.0f - (fConst19 + 16.0f * fConst7)) + 96.0f;
		fConst39 = fConst34 + fConst8 * (fConst36 - fConst35) + -64.0f;
		fConst40 = fConst11 + fConst13 + fConst8 * (0.0f - fConst22) + 16.0f;
		float fConst41 = 2.472136f * fConst16;
		float fConst42 = 9.888544f / fConst0;
		fConst43 = fConst34 + fConst8 * (fConst41 - fConst42) + -64.0f;
		fConst44 = fConst34 + fConst8 * (fConst42 - fConst41) + -64.0f;
		fConst45 = fConst11 + fConst13 + fConst8 * (0.0f - fConst17) + 16.0f;
		float fConst46 = std::tan(1196.9468f / fConst0);
		float fConst47 = std::sqrt(4.0f * fConst5 * std::tan(282.74335f / fConst0) * fConst46);
		float fConst48 = fConst12 * piece_nono_quando_fb1_faustpower4_f(fConst47);
		float fConst49 = fConst0 * fConst46;
		float fConst50 = piece_nono_quando_fb1_faustpower2_f(fConst47);
		float fConst51 = 2.0f * fConst49 - 0.5f * (fConst50 / fConst49);
		float fConst52 = piece_nono_quando_fb1_faustpower2_f(fConst51);
		fConst53 = 4.0f * fConst52;
		float fConst54 = fConst48 + fConst2 * (fConst53 + 8.0f * fConst50);
		float fConst55 = fConst15 * fConst50;
		float fConst56 = 1.236068f * fConst55 + fConst14;
		fConst57 = 1.0f / (fConst54 + fConst51 * fConst56 + 16.0f);
		float fConst58 = 8.0f * fConst52;
		fConst59 = 0.0f - fConst58;
		float fConst60 = 3.236068f * fConst55 + fConst21;
		float fConst61 = fConst51 * fConst60 + fConst54 + 16.0f;
		fConst62 = fConst2 / fConst61;
		float fConst63 = fConst2 * fConst50;
		float fConst64 = 2.0f * (fConst51 / fConst0);
		float fConst65 = fConst63 + fConst64 + 4.0f;
		fConst66 = 2.0f * (fConst51 / (fConst0 * fConst65));
		fConst67 = 1.0f / fConst65;
		fConst68 = 2.0f * fConst63 + -8.0f;
		fConst69 = fConst63 + (4.0f - fConst64);
		fConst70 = 0.0f - fConst66;
		fConst71 = 1.0f / fConst61;
		float fConst72 = 4.0f * fConst48;
		float fConst73 = 6.472136f * fConst55;
		fConst74 = fConst72 + fConst51 * (fConst73 - fConst36) + -64.0f;
		fConst75 = 6.0f * fConst48 + fConst2 * (0.0f - (fConst58 + 16.0f * fConst50)) + 96.0f;
		fConst76 = fConst72 + fConst51 * (fConst36 - fConst73) + -64.0f;
		fConst77 = fConst54 + fConst51 * (0.0f - fConst60) + 16.0f;
		float fConst78 = 2.472136f * fConst55;
		fConst79 = fConst72 + fConst51 * (fConst78 - fConst42) + -64.0f;
		fConst80 = fConst72 + fConst51 * (fConst42 - fConst78) + -64.0f;
		fConst81 = fConst54 + fConst51 * (0.0f - fConst56) + 16.0f;
		float fConst82 = std::tan(10741.105f / fConst0);
		float fConst83 = fConst0 * fConst82;
		float fConst84 = std::sqrt(4.0f * fConst5 * std::tan(10125.354f / fConst0) * fConst82);
		float fConst85 = piece_nono_quando_fb1_faustpower2_f(fConst84);
		float fConst86 = 2.0f * fConst83 - 0.5f * (fConst85 / fConst83);
		float fConst87 = piece_nono_quando_fb1_faustpower2_f(fConst86);
		fConst88 = 4.0f * fConst87;
		float fConst89 = fConst2 * (fConst88 + 8.0f * fConst85);
		float fConst90 = fConst12 * piece_nono_quando_fb1_faustpower4_f(fConst84);
		float fConst91 = fConst15 * fConst85;
		float fConst92 = fConst14 + 1.236068f * fConst91;
		fConst93 = 1.0f / (fConst89 + fConst90 + fConst86 * fConst92 + 16.0f);
		float fConst94 = 8.0f * fConst87;
		fConst95 = 0.0f - fConst94;
		float fConst96 = fConst21 + 3.236068f * fConst91;
		float fConst97 = fConst90 + fConst89;
		float fConst98 = fConst86 * fConst96 + fConst97 + 16.0f;
		fConst99 = fConst2 / fConst98;
		float fConst100 = fConst2 * fConst85;
		float fConst101 = 2.0f * (fConst86 / fConst0);
		float fConst102 = fConst100 + fConst101 + 4.0f;
		fConst103 = 2.0f * (fConst86 / (fConst0 * fConst102));
		fConst104 = 1.0f / fConst102;
		fConst105 = 2.0f * fConst100 + -8.0f;
		fConst106 = fConst100 + (4.0f - fConst101);
		fConst107 = 0.0f - fConst103;
		fConst108 = 1.0f / fConst98;
		float fConst109 = 4.0f * fConst90;
		float fConst110 = 6.472136f * fConst91;
		fConst111 = fConst109 + fConst86 * (fConst110 - fConst36) + -64.0f;
		fConst112 = 6.0f * fConst90 + fConst2 * (0.0f - (fConst94 + 16.0f * fConst85)) + 96.0f;
		fConst113 = fConst109 + fConst86 * (fConst36 - fConst110) + -64.0f;
		fConst114 = fConst97 + fConst86 * (0.0f - fConst96) + 16.0f;
		float fConst115 = 2.472136f * fConst91;
		fConst116 = fConst109 + fConst86 * (fConst115 - fConst42) + -64.0f;
		fConst117 = fConst109 + fConst86 * (fConst42 - fConst115) + -64.0f;
		fConst118 = fConst89 + fConst90 + fConst86 * (0.0f - fConst92) + 16.0f;
		float fConst119 = std::tan(15189.601f / fConst0);
		float fConst120 = std::sqrt(4.0f * fConst5 * std::tan(14344.512f / fConst0) * fConst119);
		float fConst121 = fConst12 * piece_nono_quando_fb1_faustpower4_f(fConst120);
		float fConst122 = fConst0 * fConst119;
		float fConst123 = piece_nono_quando_fb1_faustpower2_f(fConst120);
		float fConst124 = 2.0f * fConst122 - 0.5f * (fConst123 / fConst122);
		float fConst125 = piece_nono_quando_fb1_faustpower2_f(fConst124);
		fConst126 = 4.0f * fConst125;
		float fConst127 = fConst121 + fConst2 * (fConst126 + 8.0f * fConst123);
		float fConst128 = fConst15 * fConst123;
		float fConst129 = fConst14 + 1.236068f * fConst128;
		fConst130 = 1.0f / (fConst127 + fConst124 * fConst129 + 16.0f);
		float fConst131 = 8.0f * fConst125;
		fConst132 = 0.0f - fConst131;
		float fConst133 = fConst21 + 3.236068f * fConst128;
		float fConst134 = fConst124 * fConst133 + fConst127 + 16.0f;
		fConst135 = fConst2 / fConst134;
		float fConst136 = fConst2 * fConst123;
		float fConst137 = 2.0f * (fConst124 / fConst0);
		float fConst138 = fConst136 + fConst137 + 4.0f;
		fConst139 = 2.0f * (fConst124 / (fConst0 * fConst138));
		fConst140 = 1.0f / fConst138;
		fConst141 = 2.0f * fConst136 + -8.0f;
		fConst142 = fConst136 + (4.0f - fConst137);
		fConst143 = 0.0f - fConst139;
		fConst144 = 1.0f / fConst134;
		float fConst145 = 4.0f * fConst121;
		float fConst146 = 6.472136f * fConst128;
		fConst147 = fConst145 + fConst124 * (fConst146 - fConst36) + -64.0f;
		fConst148 = 6.0f * fConst121 + fConst2 * (0.0f - (fConst131 + 16.0f * fConst123)) + 96.0f;
		fConst149 = fConst145 + fConst124 * (fConst36 - fConst146) + -64.0f;
		fConst150 = fConst127 + fConst124 * (0.0f - fConst133) + 16.0f;
		float fConst151 = 2.472136f * fConst128;
		fConst152 = fConst145 + fConst124 * (fConst151 - fConst42) + -64.0f;
		fConst153 = fConst145 + fConst124 * (fConst42 - fConst151) + -64.0f;
		fConst154 = fConst127 + fConst124 * (0.0f - fConst129) + 16.0f;
		float fConst155 = std::tan(21482.21f / fConst0);
		float fConst156 = std::sqrt(4.0f * fConst5 * std::tan(20288.406f / fConst0) * fConst155);
		float fConst157 = fConst12 * piece_nono_quando_fb1_faustpower4_f(fConst156);
		float fConst158 = fConst0 * fConst155;
		float fConst159 = piece_nono_quando_fb1_faustpower2_f(fConst156);
		float fConst160 = 2.0f * fConst158 - 0.5f * (fConst159 / fConst158);
		float fConst161 = piece_nono_quando_fb1_faustpower2_f(fConst160);
		fConst162 = 4.0f * fConst161;
		float fConst163 = fConst157 + fConst2 * (fConst162 + 8.0f * fConst159);
		float fConst164 = fConst15 * fConst159;
		float fConst165 = fConst14 + 1.236068f * fConst164;
		fConst166 = 1.0f / (fConst163 + fConst160 * fConst165 + 16.0f);
		float fConst167 = 8.0f * fConst161;
		fConst168 = 0.0f - fConst167;
		float fConst169 = fConst21 + 3.236068f * fConst164;
		float fConst170 = fConst160 * fConst169 + fConst163 + 16.0f;
		fConst171 = fConst2 / fConst170;
		float fConst172 = fConst2 * fConst159;
		float fConst173 = 2.0f * (fConst160 / fConst0);
		float fConst174 = fConst172 + fConst173 + 4.0f;
		fConst175 = 2.0f * (fConst160 / (fConst0 * fConst174));
		fConst176 = 1.0f / fConst174;
		fConst177 = 2.0f * fConst172 + -8.0f;
		fConst178 = fConst172 + (4.0f - fConst173);
		fConst179 = 0.0f - fConst175;
		fConst180 = 1.0f / fConst170;
		float fConst181 = 4.0f * fConst157;
		float fConst182 = 6.472136f * fConst164;
		fConst183 = fConst181 + fConst160 * (fConst182 - fConst36) + -64.0f;
		fConst184 = 6.0f * fConst157 + fConst2 * (0.0f - (fConst167 + 16.0f * fConst159)) + 96.0f;
		fConst185 = fConst181 + fConst160 * (fConst36 - fConst182) + -64.0f;
		fConst186 = fConst163 + fConst160 * (0.0f - fConst169) + 16.0f;
		float fConst187 = 2.472136f * fConst164;
		fConst188 = fConst181 + fConst160 * (fConst187 - fConst42) + -64.0f;
		fConst189 = fConst181 + fConst160 * (fConst42 - fConst187) + -64.0f;
		fConst190 = fConst163 + fConst160 * (0.0f - fConst165) + 16.0f;
	}
	
	virtual void instanceResetUserInterface() {
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 3; l0 = l0 + 1) {
			fRec2[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 5; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 5; l2 = l2 + 1) {
			fRec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3; l3 = l3 + 1) {
			fRec5[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 5; l4 = l4 + 1) {
			fRec4[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 5; l5 = l5 + 1) {
			fRec3[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec8[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 5; l7 = l7 + 1) {
			fRec7[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 5; l8 = l8 + 1) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec11[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 5; l10 = l10 + 1) {
			fRec10[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 5; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec14[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 5; l13 = l13 + 1) {
			fRec13[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 5; l14 = l14 + 1) {
			fRec12[l14] = 0.0f;
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
	
	virtual piece_nono_quando_fb1* clone() {
		return new piece_nono_quando_fb1();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("nono.quando.fb1");
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			fRec2[0] = fTemp0 - fConst29 * (fConst30 * fRec2[1] + fConst31 * fRec2[2]);
			fRec1[0] = fConst28 * fRec2[0] + fConst32 * fRec2[2] - fConst33 * (fConst37 * fRec1[1] + fConst38 * fRec1[2] + fConst39 * fRec1[3] + fConst40 * fRec1[4]);
			fRec0[0] = fConst24 * (fConst20 * fRec1[2] + fConst10 * fRec1[0] + fConst10 * fRec1[4]) - fConst18 * (fConst43 * fRec0[1] + fConst38 * fRec0[2] + fConst44 * fRec0[3] + fConst45 * fRec0[4]);
			fRec5[0] = fTemp0 - fConst67 * (fConst68 * fRec5[1] + fConst69 * fRec5[2]);
			fRec4[0] = fConst66 * fRec5[0] + fConst70 * fRec5[2] - fConst71 * (fConst74 * fRec4[1] + fConst75 * fRec4[2] + fConst76 * fRec4[3] + fConst77 * fRec4[4]);
			fRec3[0] = fConst62 * (fConst59 * fRec4[2] + fConst53 * fRec4[0] + fConst53 * fRec4[4]) - fConst57 * (fConst79 * fRec3[1] + fConst75 * fRec3[2] + fConst80 * fRec3[3] + fConst81 * fRec3[4]);
			fRec8[0] = fTemp0 - fConst104 * (fConst105 * fRec8[1] + fConst106 * fRec8[2]);
			fRec7[0] = fConst103 * fRec8[0] + fConst107 * fRec8[2] - fConst108 * (fConst111 * fRec7[1] + fConst112 * fRec7[2] + fConst113 * fRec7[3] + fConst114 * fRec7[4]);
			fRec6[0] = fConst99 * (fConst95 * fRec7[2] + fConst88 * fRec7[0] + fConst88 * fRec7[4]) - fConst93 * (fConst116 * fRec6[1] + fConst112 * fRec6[2] + fConst117 * fRec6[3] + fConst118 * fRec6[4]);
			fRec11[0] = fTemp0 - fConst140 * (fConst141 * fRec11[1] + fConst142 * fRec11[2]);
			fRec10[0] = fConst139 * fRec11[0] + fConst143 * fRec11[2] - fConst144 * (fConst147 * fRec10[1] + fConst148 * fRec10[2] + fConst149 * fRec10[3] + fConst150 * fRec10[4]);
			fRec9[0] = fConst135 * (fConst132 * fRec10[2] + fConst126 * fRec10[0] + fConst126 * fRec10[4]) - fConst130 * (fConst152 * fRec9[1] + fConst148 * fRec9[2] + fConst153 * fRec9[3] + fConst154 * fRec9[4]);
			fRec14[0] = fTemp0 - fConst176 * (fConst177 * fRec14[1] + fConst178 * fRec14[2]);
			fRec13[0] = fConst175 * fRec14[0] + fConst179 * fRec14[2] - fConst180 * (fConst183 * fRec13[1] + fConst184 * fRec13[2] + fConst185 * fRec13[3] + fConst186 * fRec13[4]);
			fRec12[0] = fConst171 * (fConst168 * fRec13[2] + fConst162 * fRec13[0] + fConst162 * fRec13[4]) - fConst166 * (fConst188 * fRec12[1] + fConst184 * fRec12[2] + fConst189 * fRec12[3] + fConst190 * fRec12[4]);
			output0[i0] = FAUSTFLOAT(fConst2 * (fConst18 * (fConst20 * fRec0[2] + fConst10 * fRec0[0] + fConst10 * fRec0[4]) + fConst57 * (fConst59 * fRec3[2] + fConst53 * fRec3[0] + fConst53 * fRec3[4]) + fConst93 * (fConst95 * fRec6[2] + fConst88 * fRec6[0] + fConst88 * fRec6[4]) + fConst130 * (fConst132 * fRec9[2] + fConst126 * fRec9[0] + fConst126 * fRec9[4]) + fConst166 * (fConst168 * fRec12[2] + fConst162 * fRec12[0] + fConst162 * fRec12[4])));
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			for (int j0 = 4; j0 > 0; j0 = j0 - 1) {
				fRec1[j0] = fRec1[j0 - 1];
			}
			for (int j1 = 4; j1 > 0; j1 = j1 - 1) {
				fRec0[j1] = fRec0[j1 - 1];
			}
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			for (int j2 = 4; j2 > 0; j2 = j2 - 1) {
				fRec4[j2] = fRec4[j2 - 1];
			}
			for (int j3 = 4; j3 > 0; j3 = j3 - 1) {
				fRec3[j3] = fRec3[j3 - 1];
			}
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			for (int j4 = 4; j4 > 0; j4 = j4 - 1) {
				fRec7[j4] = fRec7[j4 - 1];
			}
			for (int j5 = 4; j5 > 0; j5 = j5 - 1) {
				fRec6[j5] = fRec6[j5 - 1];
			}
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			for (int j6 = 4; j6 > 0; j6 = j6 - 1) {
				fRec10[j6] = fRec10[j6 - 1];
			}
			for (int j7 = 4; j7 > 0; j7 = j7 - 1) {
				fRec9[j7] = fRec9[j7 - 1];
			}
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			for (int j8 = 4; j8 > 0; j8 = j8 - 1) {
				fRec13[j8] = fRec13[j8 - 1];
			}
			for (int j9 = 4; j9 > 0; j9 = j9 - 1) {
				fRec12[j9] = fRec12[j9 - 1];
			}
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _piece_nono_quando_fb1_UI : public UI {
    static std::string name;
};

template <class T>
std::string _piece_nono_quando_fb1_UI<T>::name(sym(piece_nono_quando_fb1));

typedef _piece_nono_quando_fb1_UI<piece_nono_quando_fb1> piece_nono_quando_fb1_UI;

class faust_piece_nono_quando_fb1_tilde : public FaustExternal<piece_nono_quando_fb1, piece_nono_quando_fb1_UI> {
public:
    faust_piece_nono_quando_fb1_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
