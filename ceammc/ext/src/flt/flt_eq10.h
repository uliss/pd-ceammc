/* ------------------------------------------------------------
name: "flt.eq10"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn flt_eq10 -scn flt_eq10_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __flt_eq10_H__
#define  __flt_eq10_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN flt_eq10_dsp.h ********************************
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

class FAUST_API flt_eq10_dsp {

    public:

        flt_eq10_dsp() {}
        virtual ~flt_eq10_dsp() {}

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
        virtual flt_eq10_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public flt_eq10_dsp {

    protected:

        flt_eq10_dsp* fDSP;

    public:

        decorator_dsp(flt_eq10_dsp* flt_eq10_dsp = nullptr):fDSP(flt_eq10_dsp) {}
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
    
        virtual flt_eq10_dsp* createDSPInstance() = 0;
    
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

/************************** END flt_eq10_dsp.h **************************/
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
 The base class of Meta handler to be used in flt_eq10_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct FAUST_API Meta {
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/

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
struct flt_eq10 : public flt_eq10_dsp {
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
#define FAUSTCLASS flt_eq10
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

static float flt_eq10_faustpower2_f(float value) {
	return value * value;
}

class flt_eq10 : public flt_eq10_dsp {
	
 private:
	
	int fSampleRate;
	float fConst3;
	float fConst5;
	float fConst6;
	float fConst8;
	float fConst9;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst15;
	float fConst16;
	float fVec0[2];
	float fRec12[2];
	float fConst17;
	float fConst19;
	float fConst20;
	float fRec11[3];
	float fConst21;
	float fRec10[3];
	float fVec1[2];
	float fConst22;
	float fConst23;
	float fRec9[2];
	float fConst24;
	float fConst25;
	float fRec8[3];
	float fConst26;
	float fConst27;
	float fRec7[3];
	float fConst30;
	float fConst31;
	float fConst33;
	float fConst34;
	float fRec6[3];
	float fConst37;
	float fConst38;
	float fConst40;
	float fConst41;
	float fRec5[3];
	float fConst44;
	float fConst45;
	float fConst47;
	float fConst48;
	float fRec4[3];
	float fConst51;
	float fConst52;
	float fConst54;
	float fConst55;
	float fRec3[3];
	float fConst58;
	float fConst59;
	float fConst61;
	float fConst62;
	float fRec2[3];
	float fConst65;
	float fConst66;
	float fConst68;
	float fConst69;
	float fRec1[3];
	float fConst72;
	float fConst73;
	float fConst75;
	float fConst76;
	float fRec0[3];
	float fConst77;
	FAUSTFLOAT fVslider0;
	float fConst78;
	float fRec13[2];
	float fConst79;
	float fRec24[2];
	float fRec23[3];
	float fConst80;
	float fRec22[3];
	float fRec21[3];
	float fRec20[3];
	float fRec19[3];
	float fRec18[3];
	float fRec17[3];
	float fRec16[3];
	float fRec15[3];
	float fConst81;
	float fConst82;
	float fRec14[3];
	FAUSTFLOAT fVslider1;
	float fRec25[2];
	float fConst83;
	float fConst84;
	float fConst85;
	float fConst87;
	float fConst88;
	float fConst90;
	float fConst91;
	float fConst92;
	float fConst93;
	float fConst95;
	float fConst96;
	float fConst97;
	float fRec41[2];
	float fRec40[3];
	float fRec39[3];
	float fVec2[2];
	float fRec38[2];
	float fConst98;
	float fRec37[3];
	float fConst99;
	float fRec36[3];
	float fVec3[2];
	float fRec35[2];
	float fConst100;
	float fRec34[3];
	float fConst101;
	float fRec33[3];
	float fVec4[2];
	float fConst102;
	float fConst103;
	float fConst104;
	float fRec32[2];
	float fConst105;
	float fRec31[3];
	float fConst106;
	float fConst107;
	float fRec30[3];
	float fRec29[3];
	float fRec28[3];
	float fRec27[3];
	float fRec26[3];
	FAUSTFLOAT fVslider2;
	float fRec42[2];
	float fConst108;
	float fConst109;
	float fConst110;
	float fRec51[2];
	float fRec50[3];
	float fConst111;
	float fRec49[3];
	float fRec48[3];
	float fRec47[3];
	float fRec46[3];
	float fRec45[3];
	float fRec44[3];
	float fRec43[3];
	FAUSTFLOAT fVslider3;
	float fRec52[2];
	float fConst112;
	float fConst113;
	float fConst114;
	float fRec60[2];
	float fRec59[3];
	float fConst115;
	float fRec58[3];
	float fRec57[3];
	float fRec56[3];
	float fRec55[3];
	float fRec54[3];
	float fRec53[3];
	FAUSTFLOAT fVslider4;
	float fRec61[2];
	float fConst116;
	float fConst117;
	float fConst118;
	float fConst120;
	float fConst121;
	float fConst123;
	float fConst124;
	float fConst125;
	float fRec72[2];
	float fRec71[3];
	float fRec70[3];
	float fVec5[2];
	float fRec69[2];
	float fConst126;
	float fRec68[3];
	float fConst127;
	float fRec67[3];
	float fVec6[2];
	float fConst128;
	float fConst129;
	float fConst130;
	float fRec66[2];
	float fConst131;
	float fRec65[3];
	float fConst132;
	float fConst133;
	float fRec64[3];
	float fRec63[3];
	float fRec62[3];
	FAUSTFLOAT fVslider5;
	float fRec73[2];
	float fConst134;
	float fConst135;
	float fConst137;
	float fConst138;
	float fRec80[2];
	float fRec79[3];
	float fRec78[3];
	float fVec7[2];
	float fConst139;
	float fConst140;
	float fConst141;
	float fRec77[2];
	float fConst142;
	float fRec76[3];
	float fConst143;
	float fConst144;
	float fRec75[3];
	float fRec74[3];
	FAUSTFLOAT fVslider6;
	float fRec81[2];
	float fConst145;
	float fConst146;
	float fConst147;
	float fRec87[2];
	float fRec86[3];
	float fConst148;
	float fRec85[3];
	float fRec84[3];
	float fRec83[3];
	float fRec82[3];
	FAUSTFLOAT fVslider7;
	float fRec88[2];
	float fConst149;
	float fConst150;
	float fConst152;
	float fConst153;
	float fRec94[2];
	float fRec93[3];
	float fRec92[3];
	float fVec8[2];
	float fConst154;
	float fConst155;
	float fConst156;
	float fRec91[2];
	float fConst157;
	float fRec90[3];
	float fConst158;
	float fConst159;
	float fRec89[3];
	FAUSTFLOAT fVslider8;
	float fRec95[2];
	float fConst160;
	float fRec98[2];
	float fRec97[3];
	float fRec96[3];
	FAUSTFLOAT fVslider9;
	float fRec99[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.2");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn flt_eq10 -scn flt_eq10_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "flt_eq10.dsp");
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
		m->declare("filters.lib/mth_octave_filterbank5:author", "Julius O. Smith III");
		m->declare("filters.lib/mth_octave_filterbank5:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/mth_octave_filterbank5:license", "MIT-style STK-4.3 license");
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
		m->declare("name", "flt.eq10");
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
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::tan(25132.742f / fConst0);
		float fConst2 = 1.0f / fConst1;
		fConst3 = 1.0f / ((fConst2 + 0.618034f) / fConst1 + 1.0f);
		float fConst4 = flt_eq10_faustpower2_f(fConst1);
		fConst5 = 1.0f / fConst4;
		fConst6 = 1.0f / ((fConst2 + 1.618034f) / fConst1 + 1.0f);
		float fConst7 = std::tan(50265.484f / fConst0);
		fConst8 = 1.0f / fConst7;
		fConst9 = 1.0f / ((fConst8 + 0.618034f) / fConst7 + 1.0f);
		float fConst10 = fConst2 + 1.0f;
		fConst11 = 1.0f / (fConst1 * fConst10);
		fConst12 = 0.0f - fConst11;
		fConst13 = 1.0f / ((fConst8 + 1.618034f) / fConst7 + 1.0f);
		float fConst14 = fConst8 + 1.0f;
		fConst15 = 1.0f / fConst14;
		fConst16 = 1.0f - fConst8;
		fConst17 = (fConst8 + -1.618034f) / fConst7 + 1.0f;
		float fConst18 = flt_eq10_faustpower2_f(fConst7);
		fConst19 = 1.0f / fConst18;
		fConst20 = 2.0f * (1.0f - fConst19);
		fConst21 = (fConst8 + -0.618034f) / fConst7 + 1.0f;
		fConst22 = 1.0f - fConst2;
		fConst23 = fConst22 / fConst10;
		fConst24 = (fConst2 + -1.618034f) / fConst1 + 1.0f;
		fConst25 = 2.0f * (1.0f - fConst5);
		fConst26 = 0.0f - 2.0f / fConst4;
		fConst27 = (fConst2 + -0.618034f) / fConst1 + 1.0f;
		float fConst28 = std::tan(196.34955f / fConst0);
		float fConst29 = 1.0f / fConst28;
		fConst30 = 1.0f / ((fConst29 + 1.618034f) / fConst28 + 1.0f);
		fConst31 = (fConst29 + -1.618034f) / fConst28 + 1.0f;
		float fConst32 = flt_eq10_faustpower2_f(fConst28);
		fConst33 = 1.0f / fConst32;
		fConst34 = 2.0f * (1.0f - fConst33);
		float fConst35 = std::tan(392.6991f / fConst0);
		float fConst36 = 1.0f / fConst35;
		fConst37 = 1.0f / ((fConst36 + 1.618034f) / fConst35 + 1.0f);
		fConst38 = (fConst36 + -1.618034f) / fConst35 + 1.0f;
		float fConst39 = flt_eq10_faustpower2_f(fConst35);
		fConst40 = 1.0f / fConst39;
		fConst41 = 2.0f * (1.0f - fConst40);
		float fConst42 = std::tan(785.3982f / fConst0);
		float fConst43 = 1.0f / fConst42;
		fConst44 = 1.0f / ((fConst43 + 1.618034f) / fConst42 + 1.0f);
		fConst45 = (fConst43 + -1.618034f) / fConst42 + 1.0f;
		float fConst46 = flt_eq10_faustpower2_f(fConst42);
		fConst47 = 1.0f / fConst46;
		fConst48 = 2.0f * (1.0f - fConst47);
		float fConst49 = std::tan(1570.7964f / fConst0);
		float fConst50 = 1.0f / fConst49;
		fConst51 = 1.0f / ((fConst50 + 1.618034f) / fConst49 + 1.0f);
		fConst52 = (fConst50 + -1.618034f) / fConst49 + 1.0f;
		float fConst53 = flt_eq10_faustpower2_f(fConst49);
		fConst54 = 1.0f / fConst53;
		fConst55 = 2.0f * (1.0f - fConst54);
		float fConst56 = std::tan(3141.5928f / fConst0);
		float fConst57 = 1.0f / fConst56;
		fConst58 = 1.0f / ((fConst57 + 1.618034f) / fConst56 + 1.0f);
		fConst59 = (fConst57 + -1.618034f) / fConst56 + 1.0f;
		float fConst60 = flt_eq10_faustpower2_f(fConst56);
		fConst61 = 1.0f / fConst60;
		fConst62 = 2.0f * (1.0f - fConst61);
		float fConst63 = std::tan(6283.1855f / fConst0);
		float fConst64 = 1.0f / fConst63;
		fConst65 = 1.0f / ((fConst64 + 1.618034f) / fConst63 + 1.0f);
		fConst66 = (fConst64 + -1.618034f) / fConst63 + 1.0f;
		float fConst67 = flt_eq10_faustpower2_f(fConst63);
		fConst68 = 1.0f / fConst67;
		fConst69 = 2.0f * (1.0f - fConst68);
		float fConst70 = std::tan(12566.371f / fConst0);
		float fConst71 = 1.0f / fConst70;
		fConst72 = 1.0f / ((fConst71 + 1.618034f) / fConst70 + 1.0f);
		fConst73 = (fConst71 + -1.618034f) / fConst70 + 1.0f;
		float fConst74 = flt_eq10_faustpower2_f(fConst70);
		fConst75 = 1.0f / fConst74;
		fConst76 = 2.0f * (1.0f - fConst75);
		fConst77 = 44.1f / fConst0;
		fConst78 = 1.0f - fConst77;
		fConst79 = 0.0f - 1.0f / (fConst7 * fConst14);
		fConst80 = 0.0f - 2.0f / fConst18;
		fConst81 = 1.0f / ((fConst2 + 1.618034f) / fConst1 + 1.0f);
		fConst82 = (fConst2 + -1.618034f) / fConst1 + 1.0f;
		fConst83 = 1.0f / ((fConst57 + 0.618034f) / fConst56 + 1.0f);
		fConst84 = 1.0f / ((fConst57 + 1.618034f) / fConst56 + 1.0f);
		fConst85 = 1.0f / ((fConst64 + 0.618034f) / fConst63 + 1.0f);
		float fConst86 = fConst57 + 1.0f;
		fConst87 = 1.0f / (fConst56 * fConst86);
		fConst88 = 1.0f / ((fConst64 + 1.618034f) / fConst63 + 1.0f);
		float fConst89 = fConst64 + 1.0f;
		fConst90 = 1.0f / fConst89;
		fConst91 = 1.0f - fConst64;
		fConst92 = 1.0f / ((fConst71 + 0.618034f) / fConst70 + 1.0f);
		fConst93 = 1.0f / ((fConst71 + 1.618034f) / fConst70 + 1.0f);
		float fConst94 = fConst71 + 1.0f;
		fConst95 = 1.0f / fConst94;
		fConst96 = 1.0f - fConst71;
		fConst97 = 1.0f / fConst10;
		fConst98 = (fConst71 + -1.618034f) / fConst70 + 1.0f;
		fConst99 = (fConst71 + -0.618034f) / fConst70 + 1.0f;
		fConst100 = (fConst64 + -1.618034f) / fConst63 + 1.0f;
		fConst101 = (fConst64 + -0.618034f) / fConst63 + 1.0f;
		fConst102 = 0.0f - fConst87;
		fConst103 = 1.0f - fConst57;
		fConst104 = fConst103 / fConst86;
		fConst105 = (fConst57 + -1.618034f) / fConst56 + 1.0f;
		fConst106 = 0.0f - 2.0f / fConst60;
		fConst107 = (fConst57 + -0.618034f) / fConst56 + 1.0f;
		fConst108 = 1.0f / (fConst70 * fConst94);
		fConst109 = 0.0f - fConst108;
		fConst110 = fConst96 / fConst94;
		fConst111 = 0.0f - 2.0f / fConst74;
		fConst112 = 1.0f / (fConst63 * fConst89);
		fConst113 = 0.0f - fConst112;
		fConst114 = fConst91 / fConst89;
		fConst115 = 0.0f - 2.0f / fConst67;
		fConst116 = 1.0f / ((fConst43 + 0.618034f) / fConst42 + 1.0f);
		fConst117 = 1.0f / ((fConst43 + 1.618034f) / fConst42 + 1.0f);
		fConst118 = 1.0f / ((fConst50 + 0.618034f) / fConst49 + 1.0f);
		float fConst119 = fConst43 + 1.0f;
		fConst120 = 1.0f / (fConst42 * fConst119);
		fConst121 = 1.0f / ((fConst50 + 1.618034f) / fConst49 + 1.0f);
		float fConst122 = fConst50 + 1.0f;
		fConst123 = 1.0f / fConst122;
		fConst124 = 1.0f - fConst50;
		fConst125 = 1.0f / fConst86;
		fConst126 = (fConst50 + -1.618034f) / fConst49 + 1.0f;
		fConst127 = (fConst50 + -0.618034f) / fConst49 + 1.0f;
		fConst128 = 0.0f - fConst120;
		fConst129 = 1.0f - fConst43;
		fConst130 = fConst129 / fConst119;
		fConst131 = (fConst43 + -1.618034f) / fConst42 + 1.0f;
		fConst132 = 0.0f - 2.0f / fConst46;
		fConst133 = (fConst43 + -0.618034f) / fConst42 + 1.0f;
		fConst134 = 1.0f / ((fConst36 + 0.618034f) / fConst35 + 1.0f);
		fConst135 = 1.0f / ((fConst36 + 1.618034f) / fConst35 + 1.0f);
		float fConst136 = fConst36 + 1.0f;
		fConst137 = 1.0f / (fConst35 * fConst136);
		fConst138 = 1.0f / fConst119;
		fConst139 = 0.0f - fConst137;
		fConst140 = 1.0f - fConst36;
		fConst141 = fConst140 / fConst136;
		fConst142 = (fConst36 + -1.618034f) / fConst35 + 1.0f;
		fConst143 = 0.0f - 2.0f / fConst39;
		fConst144 = (fConst36 + -0.618034f) / fConst35 + 1.0f;
		fConst145 = 1.0f / (fConst49 * fConst122);
		fConst146 = 0.0f - fConst145;
		fConst147 = fConst124 / fConst122;
		fConst148 = 0.0f - 2.0f / fConst53;
		fConst149 = 1.0f / ((fConst29 + 0.618034f) / fConst28 + 1.0f);
		fConst150 = 1.0f / ((fConst29 + 1.618034f) / fConst28 + 1.0f);
		float fConst151 = fConst29 + 1.0f;
		fConst152 = 1.0f / (fConst28 * fConst151);
		fConst153 = 1.0f / fConst136;
		fConst154 = 0.0f - fConst152;
		fConst155 = 1.0f - fConst29;
		fConst156 = fConst155 / fConst151;
		fConst157 = (fConst29 + -1.618034f) / fConst28 + 1.0f;
		fConst158 = 0.0f - 2.0f / fConst32;
		fConst159 = (fConst29 + -0.618034f) / fConst28 + 1.0f;
		fConst160 = 1.0f / fConst151;
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fVslider2 = FAUSTFLOAT(0.0f);
		fVslider3 = FAUSTFLOAT(0.0f);
		fVslider4 = FAUSTFLOAT(0.0f);
		fVslider5 = FAUSTFLOAT(0.0f);
		fVslider6 = FAUSTFLOAT(0.0f);
		fVslider7 = FAUSTFLOAT(0.0f);
		fVslider8 = FAUSTFLOAT(0.0f);
		fVslider9 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec12[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = l2 + 1) {
			fRec11[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3; l3 = l3 + 1) {
			fRec10[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec9[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec8[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 3; l7 = l7 + 1) {
			fRec7[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec5[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec4[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec3[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec2[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = l13 + 1) {
			fRec1[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec0[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec13[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec24[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec23[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec22[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = l19 + 1) {
			fRec21[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fRec20[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec19[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec18[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec17[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec16[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = l25 + 1) {
			fRec15[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec14[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec25[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec41[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 3; l29 = l29 + 1) {
			fRec40[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec39[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fVec2[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec38[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fRec37[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fRec36[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fVec3[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec35[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fRec34[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 3; l38 = l38 + 1) {
			fRec33[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fVec4[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec32[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 3; l41 = l41 + 1) {
			fRec31[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec30[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fRec29[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec28[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec27[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 3; l46 = l46 + 1) {
			fRec26[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec42[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec51[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = l49 + 1) {
			fRec50[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = l50 + 1) {
			fRec49[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec48[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 3; l52 = l52 + 1) {
			fRec47[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fRec46[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 3; l54 = l54 + 1) {
			fRec45[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 3; l55 = l55 + 1) {
			fRec44[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fRec43[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec52[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec60[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec59[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec58[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 3; l61 = l61 + 1) {
			fRec57[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec56[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fRec55[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 3; l64 = l64 + 1) {
			fRec54[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 3; l65 = l65 + 1) {
			fRec53[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			fRec61[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec72[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 3; l68 = l68 + 1) {
			fRec71[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec70[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fVec5[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
			fRec69[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 3; l72 = l72 + 1) {
			fRec68[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 3; l73 = l73 + 1) {
			fRec67[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fVec6[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = l75 + 1) {
			fRec66[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fRec65[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 3; l77 = l77 + 1) {
			fRec64[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fRec63[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 3; l79 = l79 + 1) {
			fRec62[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec73[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fRec80[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 3; l82 = l82 + 1) {
			fRec79[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 3; l83 = l83 + 1) {
			fRec78[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fVec7[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec77[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec76[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 3; l87 = l87 + 1) {
			fRec75[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 3; l88 = l88 + 1) {
			fRec74[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec81[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec87[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
			fRec86[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 3; l92 = l92 + 1) {
			fRec85[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 3; l93 = l93 + 1) {
			fRec84[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 3; l94 = l94 + 1) {
			fRec83[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 3; l95 = l95 + 1) {
			fRec82[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fRec88[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec94[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 3; l98 = l98 + 1) {
			fRec93[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 3; l99 = l99 + 1) {
			fRec92[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			fVec8[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec91[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 3; l102 = l102 + 1) {
			fRec90[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 3; l103 = l103 + 1) {
			fRec89[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			fRec95[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fRec98[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 3; l106 = l106 + 1) {
			fRec97[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 3; l107 = l107 + 1) {
			fRec96[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fRec99[l108] = 0.0f;
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
	
	virtual flt_eq10* clone() {
		return new flt_eq10();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("flt.eq10");
		ui_interface->declare(&fVslider2, "unit", "db");
		ui_interface->addVerticalSlider("f1000", &fVslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider6, "unit", "db");
		ui_interface->addVerticalSlider("f125", &fVslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider1, "unit", "db");
		ui_interface->addVerticalSlider("f16000", &fVslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider4, "unit", "db");
		ui_interface->addVerticalSlider("f2000", &fVslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider5, "unit", "db");
		ui_interface->addVerticalSlider("f250", &fVslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider9, "unit", "db");
		ui_interface->addVerticalSlider("f31", &fVslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider3, "unit", "db");
		ui_interface->addVerticalSlider("f4000", &fVslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider7, "unit", "db");
		ui_interface->addVerticalSlider("f500", &fVslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider8, "unit", "db");
		ui_interface->addVerticalSlider("f62", &fVslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider0, "unit", "db");
		ui_interface->addVerticalSlider("f8000", &fVslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-7e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = fConst77 * float(fVslider0);
		float fSlow1 = fConst77 * float(fVslider1);
		float fSlow2 = fConst77 * float(fVslider2);
		float fSlow3 = fConst77 * float(fVslider3);
		float fSlow4 = fConst77 * float(fVslider4);
		float fSlow5 = fConst77 * float(fVslider5);
		float fSlow6 = fConst77 * float(fVslider6);
		float fSlow7 = fConst77 * float(fVslider7);
		float fSlow8 = fConst77 * float(fVslider8);
		float fSlow9 = fConst77 * float(fVslider9);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			fVec0[0] = fTemp0;
			fRec12[0] = 0.0f - fConst15 * (fConst16 * fRec12[1] - (fTemp0 + fVec0[1]));
			fRec11[0] = fRec12[0] - fConst13 * (fConst17 * fRec11[2] + fConst20 * fRec11[1]);
			fRec10[0] = fConst13 * (fRec11[2] + fRec11[0] + 2.0f * fRec11[1]) - fConst9 * (fConst21 * fRec10[2] + fConst20 * fRec10[1]);
			float fTemp1 = fRec10[2] + fRec10[0] + 2.0f * fRec10[1];
			fVec1[0] = fTemp1;
			fRec9[0] = fConst9 * (fConst12 * fVec1[1] + fConst11 * fTemp1) - fConst23 * fRec9[1];
			fRec8[0] = fRec9[0] - fConst6 * (fConst24 * fRec8[2] + fConst25 * fRec8[1]);
			fRec7[0] = fConst6 * (fConst5 * fRec8[0] + fConst26 * fRec8[1] + fConst5 * fRec8[2]) - fConst3 * (fConst27 * fRec7[2] + fConst25 * fRec7[1]);
			float fTemp2 = fConst34 * fRec6[1];
			fRec6[0] = fConst3 * (fConst5 * fRec7[0] + fConst26 * fRec7[1] + fConst5 * fRec7[2]) - fConst30 * (fConst31 * fRec6[2] + fTemp2);
			float fTemp3 = fConst41 * fRec5[1];
			fRec5[0] = fRec6[2] + fConst30 * (fTemp2 + fConst31 * fRec6[0]) - fConst37 * (fConst38 * fRec5[2] + fTemp3);
			float fTemp4 = fConst48 * fRec4[1];
			fRec4[0] = fRec5[2] + fConst37 * (fTemp3 + fConst38 * fRec5[0]) - fConst44 * (fConst45 * fRec4[2] + fTemp4);
			float fTemp5 = fConst55 * fRec3[1];
			fRec3[0] = fRec4[2] + fConst44 * (fTemp4 + fConst45 * fRec4[0]) - fConst51 * (fConst52 * fRec3[2] + fTemp5);
			float fTemp6 = fConst62 * fRec2[1];
			fRec2[0] = fRec3[2] + fConst51 * (fTemp5 + fConst52 * fRec3[0]) - fConst58 * (fConst59 * fRec2[2] + fTemp6);
			float fTemp7 = fConst69 * fRec1[1];
			fRec1[0] = fRec2[2] + fConst58 * (fTemp6 + fConst59 * fRec2[0]) - fConst65 * (fConst66 * fRec1[2] + fTemp7);
			float fTemp8 = fConst76 * fRec0[1];
			fRec0[0] = fRec1[2] + fConst65 * (fTemp7 + fConst66 * fRec1[0]) - fConst72 * (fConst73 * fRec0[2] + fTemp8);
			fRec13[0] = fSlow0 + fConst78 * fRec13[1];
			fRec24[0] = fConst79 * fVec0[1] - fConst15 * (fConst16 * fRec24[1] - fConst8 * fTemp0);
			fRec23[0] = fRec24[0] - fConst13 * (fConst17 * fRec23[2] + fConst20 * fRec23[1]);
			fRec22[0] = fConst13 * (fConst19 * fRec23[0] + fConst80 * fRec23[1] + fConst19 * fRec23[2]) - fConst9 * (fConst21 * fRec22[2] + fConst20 * fRec22[1]);
			float fTemp9 = fConst34 * fRec21[1];
			fRec21[0] = fConst9 * (fConst19 * fRec22[0] + fConst80 * fRec22[1] + fConst19 * fRec22[2]) - fConst30 * (fConst31 * fRec21[2] + fTemp9);
			float fTemp10 = fConst41 * fRec20[1];
			fRec20[0] = fRec21[2] + fConst30 * (fTemp9 + fConst31 * fRec21[0]) - fConst37 * (fConst38 * fRec20[2] + fTemp10);
			float fTemp11 = fConst48 * fRec19[1];
			fRec19[0] = fRec20[2] + fConst37 * (fTemp10 + fConst38 * fRec20[0]) - fConst44 * (fConst45 * fRec19[2] + fTemp11);
			float fTemp12 = fConst55 * fRec18[1];
			fRec18[0] = fRec19[2] + fConst44 * (fTemp11 + fConst45 * fRec19[0]) - fConst51 * (fConst52 * fRec18[2] + fTemp12);
			float fTemp13 = fConst62 * fRec17[1];
			fRec17[0] = fRec18[2] + fConst51 * (fTemp12 + fConst52 * fRec18[0]) - fConst58 * (fConst59 * fRec17[2] + fTemp13);
			float fTemp14 = fConst69 * fRec16[1];
			fRec16[0] = fRec17[2] + fConst58 * (fTemp13 + fConst59 * fRec17[0]) - fConst65 * (fConst66 * fRec16[2] + fTemp14);
			float fTemp15 = fConst76 * fRec15[1];
			fRec15[0] = fRec16[2] + fConst65 * (fTemp14 + fConst66 * fRec16[0]) - fConst72 * (fConst73 * fRec15[2] + fTemp15);
			float fTemp16 = fConst25 * fRec14[1];
			fRec14[0] = fRec15[2] + fConst72 * (fTemp15 + fConst73 * fRec15[0]) - fConst81 * (fConst82 * fRec14[2] + fTemp16);
			fRec25[0] = fSlow1 + fConst78 * fRec25[1];
			fRec41[0] = 0.0f - fConst97 * (fConst22 * fRec41[1] - fConst9 * (fTemp1 + fVec1[1]));
			fRec40[0] = fRec41[0] - fConst6 * (fConst24 * fRec40[2] + fConst25 * fRec40[1]);
			fRec39[0] = fConst6 * (fRec40[2] + fRec40[0] + 2.0f * fRec40[1]) - fConst3 * (fConst27 * fRec39[2] + fConst25 * fRec39[1]);
			float fTemp17 = fRec39[2] + fRec39[0] + 2.0f * fRec39[1];
			fVec2[0] = fTemp17;
			fRec38[0] = 0.0f - fConst95 * (fConst96 * fRec38[1] - fConst3 * (fTemp17 + fVec2[1]));
			fRec37[0] = fRec38[0] - fConst93 * (fConst98 * fRec37[2] + fConst76 * fRec37[1]);
			fRec36[0] = fConst93 * (fRec37[2] + fRec37[0] + 2.0f * fRec37[1]) - fConst92 * (fConst99 * fRec36[2] + fConst76 * fRec36[1]);
			float fTemp18 = fRec36[2] + fRec36[0] + 2.0f * fRec36[1];
			fVec3[0] = fTemp18;
			fRec35[0] = 0.0f - fConst90 * (fConst91 * fRec35[1] - fConst92 * (fTemp18 + fVec3[1]));
			fRec34[0] = fRec35[0] - fConst88 * (fConst100 * fRec34[2] + fConst69 * fRec34[1]);
			fRec33[0] = fConst88 * (fRec34[2] + fRec34[0] + 2.0f * fRec34[1]) - fConst85 * (fConst101 * fRec33[2] + fConst69 * fRec33[1]);
			float fTemp19 = fRec33[2] + fRec33[0] + 2.0f * fRec33[1];
			fVec4[0] = fTemp19;
			fRec32[0] = fConst85 * (fConst87 * fTemp19 + fConst102 * fVec4[1]) - fConst104 * fRec32[1];
			fRec31[0] = fRec32[0] - fConst84 * (fConst105 * fRec31[2] + fConst62 * fRec31[1]);
			fRec30[0] = fConst84 * (fConst61 * fRec31[0] + fConst106 * fRec31[1] + fConst61 * fRec31[2]) - fConst83 * (fConst107 * fRec30[2] + fConst62 * fRec30[1]);
			float fTemp20 = fConst34 * fRec29[1];
			fRec29[0] = fConst83 * (fConst61 * fRec30[0] + fConst106 * fRec30[1] + fConst61 * fRec30[2]) - fConst30 * (fConst31 * fRec29[2] + fTemp20);
			float fTemp21 = fConst41 * fRec28[1];
			fRec28[0] = fRec29[2] + fConst30 * (fTemp20 + fConst31 * fRec29[0]) - fConst37 * (fConst38 * fRec28[2] + fTemp21);
			float fTemp22 = fConst48 * fRec27[1];
			fRec27[0] = fRec28[2] + fConst37 * (fTemp21 + fConst38 * fRec28[0]) - fConst44 * (fConst45 * fRec27[2] + fTemp22);
			float fTemp23 = fConst55 * fRec26[1];
			fRec26[0] = fRec27[2] + fConst44 * (fTemp22 + fConst45 * fRec27[0]) - fConst51 * (fConst52 * fRec26[2] + fTemp23);
			fRec42[0] = fSlow2 + fConst78 * fRec42[1];
			fRec51[0] = fConst3 * (fConst108 * fTemp17 + fConst109 * fVec2[1]) - fConst110 * fRec51[1];
			fRec50[0] = fRec51[0] - fConst93 * (fConst98 * fRec50[2] + fConst76 * fRec50[1]);
			fRec49[0] = fConst93 * (fConst75 * fRec50[0] + fConst111 * fRec50[1] + fConst75 * fRec50[2]) - fConst92 * (fConst99 * fRec49[2] + fConst76 * fRec49[1]);
			float fTemp24 = fConst34 * fRec48[1];
			fRec48[0] = fConst92 * (fConst75 * fRec49[0] + fConst111 * fRec49[1] + fConst75 * fRec49[2]) - fConst30 * (fConst31 * fRec48[2] + fTemp24);
			float fTemp25 = fConst41 * fRec47[1];
			fRec47[0] = fRec48[2] + fConst30 * (fTemp24 + fConst31 * fRec48[0]) - fConst37 * (fConst38 * fRec47[2] + fTemp25);
			float fTemp26 = fConst48 * fRec46[1];
			fRec46[0] = fRec47[2] + fConst37 * (fTemp25 + fConst38 * fRec47[0]) - fConst44 * (fConst45 * fRec46[2] + fTemp26);
			float fTemp27 = fConst55 * fRec45[1];
			fRec45[0] = fRec46[2] + fConst44 * (fTemp26 + fConst45 * fRec46[0]) - fConst51 * (fConst52 * fRec45[2] + fTemp27);
			float fTemp28 = fConst62 * fRec44[1];
			fRec44[0] = fRec45[2] + fConst51 * (fTemp27 + fConst52 * fRec45[0]) - fConst58 * (fConst59 * fRec44[2] + fTemp28);
			float fTemp29 = fConst69 * fRec43[1];
			fRec43[0] = fRec44[2] + fConst58 * (fTemp28 + fConst59 * fRec44[0]) - fConst65 * (fConst66 * fRec43[2] + fTemp29);
			fRec52[0] = fSlow3 + fConst78 * fRec52[1];
			fRec60[0] = fConst92 * (fConst112 * fTemp18 + fConst113 * fVec3[1]) - fConst114 * fRec60[1];
			fRec59[0] = fRec60[0] - fConst88 * (fConst100 * fRec59[2] + fConst69 * fRec59[1]);
			fRec58[0] = fConst88 * (fConst68 * fRec59[0] + fConst115 * fRec59[1] + fConst68 * fRec59[2]) - fConst85 * (fConst101 * fRec58[2] + fConst69 * fRec58[1]);
			float fTemp30 = fConst34 * fRec57[1];
			fRec57[0] = fConst85 * (fConst68 * fRec58[0] + fConst115 * fRec58[1] + fConst68 * fRec58[2]) - fConst30 * (fConst31 * fRec57[2] + fTemp30);
			float fTemp31 = fConst41 * fRec56[1];
			fRec56[0] = fRec57[2] + fConst30 * (fTemp30 + fConst31 * fRec57[0]) - fConst37 * (fConst38 * fRec56[2] + fTemp31);
			float fTemp32 = fConst48 * fRec55[1];
			fRec55[0] = fRec56[2] + fConst37 * (fTemp31 + fConst38 * fRec56[0]) - fConst44 * (fConst45 * fRec55[2] + fTemp32);
			float fTemp33 = fConst55 * fRec54[1];
			fRec54[0] = fRec55[2] + fConst44 * (fTemp32 + fConst45 * fRec55[0]) - fConst51 * (fConst52 * fRec54[2] + fTemp33);
			float fTemp34 = fConst62 * fRec53[1];
			fRec53[0] = fRec54[2] + fConst51 * (fTemp33 + fConst52 * fRec54[0]) - fConst58 * (fConst59 * fRec53[2] + fTemp34);
			fRec61[0] = fSlow4 + fConst78 * fRec61[1];
			fRec72[0] = 0.0f - fConst125 * (fConst103 * fRec72[1] - fConst85 * (fTemp19 + fVec4[1]));
			fRec71[0] = fRec72[0] - fConst84 * (fConst105 * fRec71[2] + fConst62 * fRec71[1]);
			fRec70[0] = fConst84 * (fRec71[2] + fRec71[0] + 2.0f * fRec71[1]) - fConst83 * (fConst107 * fRec70[2] + fConst62 * fRec70[1]);
			float fTemp35 = fRec70[2] + fRec70[0] + 2.0f * fRec70[1];
			fVec5[0] = fTemp35;
			fRec69[0] = 0.0f - fConst123 * (fConst124 * fRec69[1] - fConst83 * (fTemp35 + fVec5[1]));
			fRec68[0] = fRec69[0] - fConst121 * (fConst126 * fRec68[2] + fConst55 * fRec68[1]);
			fRec67[0] = fConst121 * (fRec68[2] + fRec68[0] + 2.0f * fRec68[1]) - fConst118 * (fConst127 * fRec67[2] + fConst55 * fRec67[1]);
			float fTemp36 = fRec67[2] + fRec67[0] + 2.0f * fRec67[1];
			fVec6[0] = fTemp36;
			fRec66[0] = fConst118 * (fConst120 * fTemp36 + fConst128 * fVec6[1]) - fConst130 * fRec66[1];
			fRec65[0] = fRec66[0] - fConst117 * (fConst131 * fRec65[2] + fConst48 * fRec65[1]);
			fRec64[0] = fConst117 * (fConst47 * fRec65[0] + fConst132 * fRec65[1] + fConst47 * fRec65[2]) - fConst116 * (fConst133 * fRec64[2] + fConst48 * fRec64[1]);
			float fTemp37 = fConst34 * fRec63[1];
			fRec63[0] = fConst116 * (fConst47 * fRec64[0] + fConst132 * fRec64[1] + fConst47 * fRec64[2]) - fConst30 * (fConst31 * fRec63[2] + fTemp37);
			float fTemp38 = fConst41 * fRec62[1];
			fRec62[0] = fRec63[2] + fConst30 * (fTemp37 + fConst31 * fRec63[0]) - fConst37 * (fConst38 * fRec62[2] + fTemp38);
			fRec73[0] = fSlow5 + fConst78 * fRec73[1];
			fRec80[0] = 0.0f - fConst138 * (fConst129 * fRec80[1] - fConst118 * (fTemp36 + fVec6[1]));
			fRec79[0] = fRec80[0] - fConst117 * (fConst131 * fRec79[2] + fConst48 * fRec79[1]);
			fRec78[0] = fConst117 * (fRec79[2] + fRec79[0] + 2.0f * fRec79[1]) - fConst116 * (fConst133 * fRec78[2] + fConst48 * fRec78[1]);
			float fTemp39 = fRec78[2] + fRec78[0] + 2.0f * fRec78[1];
			fVec7[0] = fTemp39;
			fRec77[0] = fConst116 * (fConst137 * fTemp39 + fConst139 * fVec7[1]) - fConst141 * fRec77[1];
			fRec76[0] = fRec77[0] - fConst135 * (fConst142 * fRec76[2] + fConst41 * fRec76[1]);
			fRec75[0] = fConst135 * (fConst40 * fRec76[0] + fConst143 * fRec76[1] + fConst40 * fRec76[2]) - fConst134 * (fConst144 * fRec75[2] + fConst41 * fRec75[1]);
			float fTemp40 = fConst34 * fRec74[1];
			fRec74[0] = fConst134 * (fConst40 * fRec75[0] + fConst143 * fRec75[1] + fConst40 * fRec75[2]) - fConst30 * (fConst31 * fRec74[2] + fTemp40);
			fRec81[0] = fSlow6 + fConst78 * fRec81[1];
			fRec87[0] = fConst83 * (fConst145 * fTemp35 + fConst146 * fVec5[1]) - fConst147 * fRec87[1];
			fRec86[0] = fRec87[0] - fConst121 * (fConst126 * fRec86[2] + fConst55 * fRec86[1]);
			fRec85[0] = fConst121 * (fConst54 * fRec86[0] + fConst148 * fRec86[1] + fConst54 * fRec86[2]) - fConst118 * (fConst127 * fRec85[2] + fConst55 * fRec85[1]);
			float fTemp41 = fConst34 * fRec84[1];
			fRec84[0] = fConst118 * (fConst54 * fRec85[0] + fConst148 * fRec85[1] + fConst54 * fRec85[2]) - fConst30 * (fConst31 * fRec84[2] + fTemp41);
			float fTemp42 = fConst41 * fRec83[1];
			fRec83[0] = fRec84[2] + fConst30 * (fTemp41 + fConst31 * fRec84[0]) - fConst37 * (fConst38 * fRec83[2] + fTemp42);
			float fTemp43 = fConst48 * fRec82[1];
			fRec82[0] = fRec83[2] + fConst37 * (fTemp42 + fConst38 * fRec83[0]) - fConst44 * (fConst45 * fRec82[2] + fTemp43);
			fRec88[0] = fSlow7 + fConst78 * fRec88[1];
			fRec94[0] = 0.0f - fConst153 * (fConst140 * fRec94[1] - fConst116 * (fTemp39 + fVec7[1]));
			fRec93[0] = fRec94[0] - fConst135 * (fConst142 * fRec93[2] + fConst41 * fRec93[1]);
			fRec92[0] = fConst135 * (fRec93[2] + fRec93[0] + 2.0f * fRec93[1]) - fConst134 * (fConst144 * fRec92[2] + fConst41 * fRec92[1]);
			float fTemp44 = fRec92[2] + fRec92[0] + 2.0f * fRec92[1];
			fVec8[0] = fTemp44;
			fRec91[0] = fConst134 * (fConst152 * fTemp44 + fConst154 * fVec8[1]) - fConst156 * fRec91[1];
			fRec90[0] = fRec91[0] - fConst150 * (fConst157 * fRec90[2] + fConst34 * fRec90[1]);
			fRec89[0] = fConst150 * (fConst33 * fRec90[0] + fConst158 * fRec90[1] + fConst33 * fRec90[2]) - fConst149 * (fConst159 * fRec89[2] + fConst34 * fRec89[1]);
			fRec95[0] = fSlow8 + fConst78 * fRec95[1];
			fRec98[0] = 0.0f - fConst160 * (fConst155 * fRec98[1] - fConst134 * (fTemp44 + fVec8[1]));
			fRec97[0] = fRec98[0] - fConst150 * (fConst157 * fRec97[2] + fConst34 * fRec97[1]);
			fRec96[0] = fConst150 * (fRec97[2] + fRec97[0] + 2.0f * fRec97[1]) - fConst149 * (fConst159 * fRec96[2] + fConst34 * fRec96[1]);
			fRec99[0] = fSlow9 + fConst78 * fRec99[1];
			output0[i0] = FAUSTFLOAT((fRec0[2] + fConst72 * (fTemp8 + fConst73 * fRec0[0])) * std::pow(1e+01f, 0.05f * fRec13[0]) + (fRec14[2] + fConst81 * (fTemp16 + fConst82 * fRec14[0])) * std::pow(1e+01f, 0.05f * fRec25[0]) + (fRec26[2] + fConst51 * (fTemp23 + fConst52 * fRec26[0])) * std::pow(1e+01f, 0.05f * fRec42[0]) + (fRec43[2] + fConst65 * (fTemp29 + fConst66 * fRec43[0])) * std::pow(1e+01f, 0.05f * fRec52[0]) + (fRec53[2] + fConst58 * (fTemp34 + fConst59 * fRec53[0])) * std::pow(1e+01f, 0.05f * fRec61[0]) + (fRec62[2] + fConst37 * (fTemp38 + fConst38 * fRec62[0])) * std::pow(1e+01f, 0.05f * fRec73[0]) + (fRec74[2] + fConst30 * (fTemp40 + fConst31 * fRec74[0])) * std::pow(1e+01f, 0.05f * fRec81[0]) + (fRec82[2] + fConst44 * (fTemp43 + fConst45 * fRec82[0])) * std::pow(1e+01f, 0.05f * fRec88[0]) + fConst149 * ((fConst33 * fRec89[0] + fConst158 * fRec89[1] + fConst33 * fRec89[2]) * std::pow(1e+01f, 0.05f * fRec95[0]) + (fRec96[2] + fRec96[0] + 2.0f * fRec96[1]) * std::pow(1e+01f, 0.05f * fRec99[0])));
			fVec0[1] = fVec0[0];
			fRec12[1] = fRec12[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fVec1[1] = fVec1[0];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec13[1] = fRec13[0];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec25[1] = fRec25[0];
			fRec41[1] = fRec41[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fVec2[1] = fVec2[0];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fVec3[1] = fVec3[0];
			fRec35[1] = fRec35[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fVec4[1] = fVec4[0];
			fRec32[1] = fRec32[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec42[1] = fRec42[0];
			fRec51[1] = fRec51[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec52[1] = fRec52[0];
			fRec60[1] = fRec60[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec61[1] = fRec61[0];
			fRec72[1] = fRec72[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec70[2] = fRec70[1];
			fRec70[1] = fRec70[0];
			fVec5[1] = fVec5[0];
			fRec69[1] = fRec69[0];
			fRec68[2] = fRec68[1];
			fRec68[1] = fRec68[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fVec6[1] = fVec6[0];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec73[1] = fRec73[0];
			fRec80[1] = fRec80[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fVec7[1] = fVec7[0];
			fRec77[1] = fRec77[0];
			fRec76[2] = fRec76[1];
			fRec76[1] = fRec76[0];
			fRec75[2] = fRec75[1];
			fRec75[1] = fRec75[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec81[1] = fRec81[0];
			fRec87[1] = fRec87[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fRec84[2] = fRec84[1];
			fRec84[1] = fRec84[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec88[1] = fRec88[0];
			fRec94[1] = fRec94[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fVec8[1] = fVec8[0];
			fRec91[1] = fRec91[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec95[1] = fRec95[0];
			fRec98[1] = fRec98[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec96[2] = fRec96[1];
			fRec96[1] = fRec96[0];
			fRec99[1] = fRec99[0];
		}
	}

};
#endif

class faust_flt_eq10_tilde : public FaustExternal<flt_eq10> {
public:
    faust_flt_eq10_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(flt_eq10))
    {
    }
};

#endif
