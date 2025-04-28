/* ------------------------------------------------------------
name: "nono.quando.vocoder"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn piece_nono_quando_vocoder -scn piece_nono_quando_vocoder_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0
------------------------------------------------------------ */

#ifndef  __piece_nono_quando_vocoder_H__
#define  __piece_nono_quando_vocoder_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN piece_nono_quando_vocoder_dsp.h ********************************
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
#include <cstdint>

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

// Version as a global string
#define FAUSTVERSION "2.74.3"

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 74
#define FAUSTPATCHVERSION 3

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

class FAUST_API piece_nono_quando_vocoder_dsp {

    public:

        piece_nono_quando_vocoder_dsp() {}
        virtual ~piece_nono_quando_vocoder_dsp() {}

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
        virtual piece_nono_quando_vocoder_dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * Read all controllers (buttons, sliders..etc), and update the DSP state to be used by 'frame' or 'compute'.
         * This method will be filled with the -ec (--external-control) option.
         */
        virtual void control() {}
    
        /**
         * DSP instance computation to process one single frame.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write frame(inputs, inputs).
         * The -inpl option can be used for that, but only in scalar mode for now.
         * This method will be filled with the -os (--one-sample) option.
         *
         * @param inputs - the input audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         */
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) {}
        
        /**
         * DSP instance computation to be called with successive in/out audio buffers.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write compute(count, inputs, inputs).
         * The -inpl compilation option can be used for that, but only in scalar mode for now.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * Alternative DSP instance computation method for use by subclasses, incorporating an additional `date_usec` parameter,
         * which specifies the timestamp of the first sample in the audio buffers.
         *
         * @param date_usec - the timestamp in microsec given by audio driver. By convention timestamp of -1 means 'no timestamp conversion',
         * events already have a timestamp expressed in frames.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class FAUST_API decorator_dsp : public piece_nono_quando_vocoder_dsp {

    protected:

        piece_nono_quando_vocoder_dsp* fDSP;

    public:

        decorator_dsp(piece_nono_quando_vocoder_dsp* piece_nono_quando_vocoder_dsp = nullptr):fDSP(piece_nono_quando_vocoder_dsp) {}
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
        virtual void control() { fDSP->control(); }
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) { fDSP->frame(inputs, outputs); }
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
    
        /* Return factory name */
        virtual std::string getName() = 0;
    
        /* Return factory SHA key */
        virtual std::string getSHAKey() = 0;
    
        /* Return factory expanded DSP code */
        virtual std::string getDSPCode() = 0;
    
        /* Return factory compile options */
        virtual std::string getCompileOptions() = 0;
    
        /* Get the Faust DSP factory list of library dependancies */
        virtual std::vector<std::string> getLibraryList() = 0;
    
        /* Get the list of all used includes */
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        /* Get warning messages list for a given compilation */
        virtual std::vector<std::string> getWarningMessages() = 0;
    
        /* Create a new DSP instance, to be deleted with C++ 'delete' */
        virtual piece_nono_quando_vocoder_dsp* createDSPInstance() = 0;
    
        /* Static tables initialization, possibly implemened in sub-classes*/
        virtual void classInit(int sample_rate) {};
    
        /* Set a custom memory manager to be used when creating instances */
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
    
        /* Return the currently set custom memory manager */
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

/************************** END piece_nono_quando_vocoder_dsp.h **************************/
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
 The base class of Meta handler to be used in piece_nono_quando_vocoder_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct piece_nono_quando_vocoder : public piece_nono_quando_vocoder_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS piece_nono_quando_vocoder
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

static double piece_nono_quando_vocoder_faustpower2_f(double value) {
	return value * value;
}

class piece_nono_quando_vocoder : public piece_nono_quando_vocoder_dsp {
	
 private:
	
	int fSampleRate;
	double fConst0;
	double fConst1;
	double fConst2;
	double fConst3;
	double fConst4;
	double fConst5;
	double fConst6;
	double fConst7;
	double fConst8;
	double fConst9;
	double fConst10;
	double fConst11;
	double fConst12;
	double fConst13;
	double fConst14;
	double fConst15;
	double fConst16;
	double fConst17;
	double fConst18;
	double fConst19;
	double fConst20;
	double fConst21;
	double fConst22;
	double fConst23;
	double fConst24;
	double fRec13[3];
	double fConst25;
	double fConst26;
	double fConst27;
	double fRec12[3];
	double fConst28;
	double fConst29;
	double fConst30;
	double fRec11[3];
	double fConst31;
	double fConst32;
	double fConst33;
	double fRec10[3];
	double fConst34;
	double fConst35;
	double fConst36;
	double fRec9[3];
	double fConst37;
	double fConst38;
	double fConst39;
	double fRec8[3];
	double fConst40;
	double fRec7[3];
	double fRec6[3];
	double fRec5[3];
	double fRec4[3];
	double fRec3[3];
	double fRec2[3];
	double fConst41;
	FAUSTFLOAT fHslider0;
	double fConst42;
	double fConst43;
	double fRec14[2];
	double fConst44;
	FAUSTFLOAT fHslider1;
	double fRec15[2];
	double fRec1[2];
	FAUSTFLOAT fHslider2;
	double fRec28[2];
	double fConst45;
	double fConst46;
	FAUSTFLOAT fHslider3;
	double fConst47;
	double fConst48;
	double fConst49;
	double fConst50;
	double fConst51;
	double fConst52;
	double fConst53;
	double fConst54;
	double fConst55;
	double fConst56;
	double fConst57;
	double fConst58;
	double fConst59;
	double fConst60;
	double fConst61;
	double fConst62;
	int iRec35[2];
	double fVec0[2];
	double fRec34[2];
	double fRec33[2];
	double fVec1[2];
	double fConst63;
	double fRec32[2];
	double fVec2[2];
	double fConst64;
	double fRec31[2];
	double fVec3[2];
	double fConst65;
	double fRec30[2];
	double fVec4[2];
	double fConst66;
	double fRec29[2];
	double fRec27[3];
	double fRec26[3];
	double fRec25[3];
	double fRec24[3];
	double fRec23[3];
	double fRec22[3];
	double fRec21[3];
	double fRec20[3];
	double fRec19[3];
	double fRec18[3];
	double fRec17[3];
	double fRec16[3];
	double fConst67;
	double fConst68;
	double fConst69;
	double fConst70;
	double fConst71;
	double fConst72;
	double fConst73;
	double fConst74;
	double fConst75;
	double fConst76;
	double fConst77;
	double fConst78;
	double fConst79;
	double fConst80;
	double fConst81;
	double fConst82;
	double fConst83;
	double fConst84;
	double fConst85;
	double fConst86;
	double fConst87;
	double fConst88;
	double fConst89;
	double fConst90;
	double fRec48[3];
	double fConst91;
	double fConst92;
	double fConst93;
	double fRec47[3];
	double fConst94;
	double fConst95;
	double fConst96;
	double fRec46[3];
	double fConst97;
	double fConst98;
	double fConst99;
	double fRec45[3];
	double fConst100;
	double fConst101;
	double fConst102;
	double fRec44[3];
	double fConst103;
	double fConst104;
	double fConst105;
	double fRec43[3];
	double fConst106;
	double fRec42[3];
	double fRec41[3];
	double fRec40[3];
	double fRec39[3];
	double fRec38[3];
	double fRec37[3];
	double fRec36[2];
	double fRec60[3];
	double fRec59[3];
	double fRec58[3];
	double fRec57[3];
	double fRec56[3];
	double fRec55[3];
	double fRec54[3];
	double fRec53[3];
	double fRec52[3];
	double fRec51[3];
	double fRec50[3];
	double fRec49[3];
	double fConst107;
	double fConst108;
	double fConst109;
	double fConst110;
	double fConst111;
	double fConst112;
	double fConst113;
	double fConst114;
	double fConst115;
	double fConst116;
	double fConst117;
	double fConst118;
	double fConst119;
	double fConst120;
	double fConst121;
	double fConst122;
	double fConst123;
	double fConst124;
	double fConst125;
	double fConst126;
	double fConst127;
	double fConst128;
	double fConst129;
	double fConst130;
	double fRec73[3];
	double fConst131;
	double fConst132;
	double fConst133;
	double fRec72[3];
	double fConst134;
	double fConst135;
	double fConst136;
	double fRec71[3];
	double fConst137;
	double fConst138;
	double fConst139;
	double fRec70[3];
	double fConst140;
	double fConst141;
	double fConst142;
	double fRec69[3];
	double fConst143;
	double fConst144;
	double fConst145;
	double fRec68[3];
	double fConst146;
	double fRec67[3];
	double fRec66[3];
	double fRec65[3];
	double fRec64[3];
	double fRec63[3];
	double fRec62[3];
	double fRec61[2];
	double fRec85[3];
	double fRec84[3];
	double fRec83[3];
	double fRec82[3];
	double fRec81[3];
	double fRec80[3];
	double fRec79[3];
	double fRec78[3];
	double fRec77[3];
	double fRec76[3];
	double fRec75[3];
	double fRec74[3];
	double fConst147;
	double fConst148;
	double fConst149;
	double fConst150;
	double fConst151;
	double fConst152;
	double fConst153;
	double fConst154;
	double fConst155;
	double fConst156;
	double fConst157;
	double fConst158;
	double fConst159;
	double fConst160;
	double fConst161;
	double fConst162;
	double fConst163;
	double fConst164;
	double fConst165;
	double fConst166;
	double fConst167;
	double fConst168;
	double fConst169;
	double fConst170;
	double fRec98[3];
	double fConst171;
	double fConst172;
	double fConst173;
	double fRec97[3];
	double fConst174;
	double fConst175;
	double fConst176;
	double fRec96[3];
	double fConst177;
	double fConst178;
	double fConst179;
	double fRec95[3];
	double fConst180;
	double fConst181;
	double fConst182;
	double fRec94[3];
	double fConst183;
	double fConst184;
	double fConst185;
	double fRec93[3];
	double fConst186;
	double fRec92[3];
	double fRec91[3];
	double fRec90[3];
	double fRec89[3];
	double fRec88[3];
	double fRec87[3];
	double fRec86[2];
	double fRec110[3];
	double fRec109[3];
	double fRec108[3];
	double fRec107[3];
	double fRec106[3];
	double fRec105[3];
	double fRec104[3];
	double fRec103[3];
	double fRec102[3];
	double fRec101[3];
	double fRec100[3];
	double fRec99[3];
	double fConst187;
	double fConst188;
	double fConst189;
	double fConst190;
	double fConst191;
	double fConst192;
	double fConst193;
	double fConst194;
	double fConst195;
	double fConst196;
	double fConst197;
	double fConst198;
	double fConst199;
	double fConst200;
	double fConst201;
	double fConst202;
	double fConst203;
	double fConst204;
	double fConst205;
	double fConst206;
	double fConst207;
	double fConst208;
	double fConst209;
	double fConst210;
	double fRec123[3];
	double fConst211;
	double fConst212;
	double fConst213;
	double fRec122[3];
	double fConst214;
	double fConst215;
	double fConst216;
	double fRec121[3];
	double fConst217;
	double fConst218;
	double fConst219;
	double fRec120[3];
	double fConst220;
	double fConst221;
	double fConst222;
	double fRec119[3];
	double fConst223;
	double fConst224;
	double fConst225;
	double fRec118[3];
	double fConst226;
	double fRec117[3];
	double fRec116[3];
	double fRec115[3];
	double fRec114[3];
	double fRec113[3];
	double fRec112[3];
	double fRec111[2];
	double fRec135[3];
	double fRec134[3];
	double fRec133[3];
	double fRec132[3];
	double fRec131[3];
	double fRec130[3];
	double fRec129[3];
	double fRec128[3];
	double fRec127[3];
	double fRec126[3];
	double fRec125[3];
	double fRec124[3];
	double fConst227;
	double fConst228;
	double fConst229;
	double fConst230;
	double fConst231;
	double fConst232;
	double fConst233;
	double fConst234;
	double fConst235;
	double fConst236;
	double fConst237;
	double fConst238;
	double fConst239;
	double fConst240;
	double fConst241;
	double fConst242;
	double fConst243;
	double fConst244;
	double fConst245;
	double fConst246;
	double fConst247;
	double fConst248;
	double fConst249;
	double fConst250;
	double fRec148[3];
	double fConst251;
	double fConst252;
	double fConst253;
	double fRec147[3];
	double fConst254;
	double fConst255;
	double fConst256;
	double fRec146[3];
	double fConst257;
	double fConst258;
	double fConst259;
	double fRec145[3];
	double fConst260;
	double fConst261;
	double fConst262;
	double fRec144[3];
	double fConst263;
	double fConst264;
	double fConst265;
	double fRec143[3];
	double fConst266;
	double fRec142[3];
	double fRec141[3];
	double fRec140[3];
	double fRec139[3];
	double fRec138[3];
	double fRec137[3];
	double fRec136[2];
	double fRec160[3];
	double fRec159[3];
	double fRec158[3];
	double fRec157[3];
	double fRec156[3];
	double fRec155[3];
	double fRec154[3];
	double fRec153[3];
	double fRec152[3];
	double fRec151[3];
	double fRec150[3];
	double fRec149[3];
	double fConst267;
	double fConst268;
	double fConst269;
	double fConst270;
	double fConst271;
	double fConst272;
	double fConst273;
	double fConst274;
	double fConst275;
	double fConst276;
	double fConst277;
	double fConst278;
	double fConst279;
	double fConst280;
	double fConst281;
	double fConst282;
	double fConst283;
	double fConst284;
	double fConst285;
	double fConst286;
	double fConst287;
	double fConst288;
	double fConst289;
	double fConst290;
	double fRec173[3];
	double fConst291;
	double fConst292;
	double fConst293;
	double fRec172[3];
	double fConst294;
	double fConst295;
	double fConst296;
	double fRec171[3];
	double fConst297;
	double fConst298;
	double fConst299;
	double fRec170[3];
	double fConst300;
	double fConst301;
	double fConst302;
	double fRec169[3];
	double fConst303;
	double fConst304;
	double fConst305;
	double fRec168[3];
	double fConst306;
	double fRec167[3];
	double fRec166[3];
	double fRec165[3];
	double fRec164[3];
	double fRec163[3];
	double fRec162[3];
	double fRec161[2];
	double fRec185[3];
	double fRec184[3];
	double fRec183[3];
	double fRec182[3];
	double fRec181[3];
	double fRec180[3];
	double fRec179[3];
	double fRec178[3];
	double fRec177[3];
	double fRec176[3];
	double fRec175[3];
	double fRec174[3];
	double fConst307;
	double fConst308;
	double fConst309;
	double fConst310;
	double fConst311;
	double fConst312;
	double fConst313;
	double fConst314;
	double fConst315;
	double fConst316;
	double fConst317;
	double fConst318;
	double fConst319;
	double fConst320;
	double fConst321;
	double fConst322;
	double fConst323;
	double fConst324;
	double fConst325;
	double fConst326;
	double fConst327;
	double fConst328;
	double fConst329;
	double fConst330;
	double fRec198[3];
	double fConst331;
	double fConst332;
	double fConst333;
	double fRec197[3];
	double fConst334;
	double fConst335;
	double fConst336;
	double fRec196[3];
	double fConst337;
	double fConst338;
	double fConst339;
	double fRec195[3];
	double fConst340;
	double fConst341;
	double fConst342;
	double fRec194[3];
	double fConst343;
	double fConst344;
	double fConst345;
	double fRec193[3];
	double fConst346;
	double fRec192[3];
	double fRec191[3];
	double fRec190[3];
	double fRec189[3];
	double fRec188[3];
	double fRec187[3];
	double fRec186[2];
	double fRec210[3];
	double fRec209[3];
	double fRec208[3];
	double fRec207[3];
	double fRec206[3];
	double fRec205[3];
	double fRec204[3];
	double fRec203[3];
	double fRec202[3];
	double fRec201[3];
	double fRec200[3];
	double fRec199[3];
	double fConst347;
	double fConst348;
	double fConst349;
	double fConst350;
	double fConst351;
	double fConst352;
	double fConst353;
	double fConst354;
	double fConst355;
	double fConst356;
	double fConst357;
	double fConst358;
	double fConst359;
	double fConst360;
	double fConst361;
	double fConst362;
	double fConst363;
	double fConst364;
	double fConst365;
	double fConst366;
	double fConst367;
	double fConst368;
	double fConst369;
	double fConst370;
	double fRec223[3];
	double fConst371;
	double fConst372;
	double fConst373;
	double fRec222[3];
	double fConst374;
	double fConst375;
	double fConst376;
	double fRec221[3];
	double fConst377;
	double fConst378;
	double fConst379;
	double fRec220[3];
	double fConst380;
	double fConst381;
	double fConst382;
	double fRec219[3];
	double fConst383;
	double fConst384;
	double fConst385;
	double fRec218[3];
	double fConst386;
	double fRec217[3];
	double fRec216[3];
	double fRec215[3];
	double fRec214[3];
	double fRec213[3];
	double fRec212[3];
	double fRec211[2];
	double fRec235[3];
	double fRec234[3];
	double fRec233[3];
	double fRec232[3];
	double fRec231[3];
	double fRec230[3];
	double fRec229[3];
	double fRec228[3];
	double fRec227[3];
	double fRec226[3];
	double fRec225[3];
	double fRec224[3];
	double fConst387;
	double fConst388;
	double fConst389;
	double fConst390;
	double fConst391;
	double fConst392;
	double fConst393;
	double fConst394;
	double fConst395;
	double fConst396;
	double fConst397;
	double fConst398;
	double fConst399;
	double fConst400;
	double fConst401;
	double fConst402;
	double fConst403;
	double fConst404;
	double fConst405;
	double fConst406;
	double fConst407;
	double fConst408;
	double fConst409;
	double fConst410;
	double fRec248[3];
	double fConst411;
	double fConst412;
	double fConst413;
	double fRec247[3];
	double fConst414;
	double fConst415;
	double fConst416;
	double fRec246[3];
	double fConst417;
	double fConst418;
	double fConst419;
	double fRec245[3];
	double fConst420;
	double fConst421;
	double fConst422;
	double fRec244[3];
	double fConst423;
	double fConst424;
	double fConst425;
	double fRec243[3];
	double fConst426;
	double fRec242[3];
	double fRec241[3];
	double fRec240[3];
	double fRec239[3];
	double fRec238[3];
	double fRec237[3];
	double fRec236[2];
	double fRec260[3];
	double fRec259[3];
	double fRec258[3];
	double fRec257[3];
	double fRec256[3];
	double fRec255[3];
	double fRec254[3];
	double fRec253[3];
	double fRec252[3];
	double fRec251[3];
	double fRec250[3];
	double fRec249[3];
	double fConst427;
	double fConst428;
	double fConst429;
	double fConst430;
	double fConst431;
	double fConst432;
	double fConst433;
	double fConst434;
	double fConst435;
	double fConst436;
	double fConst437;
	double fConst438;
	double fConst439;
	double fConst440;
	double fConst441;
	double fConst442;
	double fConst443;
	double fConst444;
	double fConst445;
	double fConst446;
	double fConst447;
	double fConst448;
	double fConst449;
	double fConst450;
	double fRec273[3];
	double fConst451;
	double fConst452;
	double fConst453;
	double fRec272[3];
	double fConst454;
	double fConst455;
	double fConst456;
	double fRec271[3];
	double fConst457;
	double fConst458;
	double fConst459;
	double fRec270[3];
	double fConst460;
	double fConst461;
	double fConst462;
	double fRec269[3];
	double fConst463;
	double fConst464;
	double fConst465;
	double fRec268[3];
	double fConst466;
	double fRec267[3];
	double fRec266[3];
	double fRec265[3];
	double fRec264[3];
	double fRec263[3];
	double fRec262[3];
	double fRec261[2];
	double fRec285[3];
	double fRec284[3];
	double fRec283[3];
	double fRec282[3];
	double fRec281[3];
	double fRec280[3];
	double fRec279[3];
	double fRec278[3];
	double fRec277[3];
	double fRec276[3];
	double fRec275[3];
	double fRec274[3];
	double fConst467;
	double fConst468;
	double fConst469;
	double fConst470;
	double fConst471;
	double fConst472;
	double fConst473;
	double fConst474;
	double fConst475;
	double fConst476;
	double fConst477;
	double fConst478;
	double fConst479;
	double fConst480;
	double fConst481;
	double fConst482;
	double fConst483;
	double fConst484;
	double fConst485;
	double fConst486;
	double fConst487;
	double fConst488;
	double fConst489;
	double fConst490;
	double fRec298[3];
	double fConst491;
	double fConst492;
	double fConst493;
	double fRec297[3];
	double fConst494;
	double fConst495;
	double fConst496;
	double fRec296[3];
	double fConst497;
	double fConst498;
	double fConst499;
	double fRec295[3];
	double fConst500;
	double fConst501;
	double fConst502;
	double fRec294[3];
	double fConst503;
	double fConst504;
	double fConst505;
	double fRec293[3];
	double fConst506;
	double fRec292[3];
	double fRec291[3];
	double fRec290[3];
	double fRec289[3];
	double fRec288[3];
	double fRec287[3];
	double fRec286[2];
	double fRec310[3];
	double fRec309[3];
	double fRec308[3];
	double fRec307[3];
	double fRec306[3];
	double fRec305[3];
	double fRec304[3];
	double fRec303[3];
	double fRec302[3];
	double fRec301[3];
	double fRec300[3];
	double fRec299[3];
	double fConst507;
	double fConst508;
	double fConst509;
	double fConst510;
	double fConst511;
	double fConst512;
	double fConst513;
	double fConst514;
	double fConst515;
	double fConst516;
	double fConst517;
	double fConst518;
	double fConst519;
	double fConst520;
	double fConst521;
	double fConst522;
	double fConst523;
	double fConst524;
	double fConst525;
	double fConst526;
	double fConst527;
	double fConst528;
	double fConst529;
	double fConst530;
	double fRec323[3];
	double fConst531;
	double fConst532;
	double fConst533;
	double fRec322[3];
	double fConst534;
	double fConst535;
	double fConst536;
	double fRec321[3];
	double fConst537;
	double fConst538;
	double fConst539;
	double fRec320[3];
	double fConst540;
	double fConst541;
	double fConst542;
	double fRec319[3];
	double fConst543;
	double fConst544;
	double fConst545;
	double fRec318[3];
	double fConst546;
	double fRec317[3];
	double fRec316[3];
	double fRec315[3];
	double fRec314[3];
	double fRec313[3];
	double fRec312[3];
	double fRec311[2];
	double fRec335[3];
	double fRec334[3];
	double fRec333[3];
	double fRec332[3];
	double fRec331[3];
	double fRec330[3];
	double fRec329[3];
	double fRec328[3];
	double fRec327[3];
	double fRec326[3];
	double fRec325[3];
	double fRec324[3];
	double fConst547;
	double fConst548;
	double fConst549;
	double fConst550;
	double fConst551;
	double fConst552;
	double fConst553;
	double fConst554;
	double fConst555;
	double fConst556;
	double fConst557;
	double fConst558;
	double fConst559;
	double fConst560;
	double fConst561;
	double fConst562;
	double fConst563;
	double fConst564;
	double fConst565;
	double fConst566;
	double fConst567;
	double fConst568;
	double fConst569;
	double fConst570;
	double fRec348[3];
	double fConst571;
	double fConst572;
	double fConst573;
	double fRec347[3];
	double fConst574;
	double fConst575;
	double fConst576;
	double fRec346[3];
	double fConst577;
	double fConst578;
	double fConst579;
	double fRec345[3];
	double fConst580;
	double fConst581;
	double fConst582;
	double fRec344[3];
	double fConst583;
	double fConst584;
	double fConst585;
	double fRec343[3];
	double fConst586;
	double fRec342[3];
	double fRec341[3];
	double fRec340[3];
	double fRec339[3];
	double fRec338[3];
	double fRec337[3];
	double fRec336[2];
	double fRec360[3];
	double fRec359[3];
	double fRec358[3];
	double fRec357[3];
	double fRec356[3];
	double fRec355[3];
	double fRec354[3];
	double fRec353[3];
	double fRec352[3];
	double fRec351[3];
	double fRec350[3];
	double fRec349[3];
	double fConst587;
	double fConst588;
	double fConst589;
	double fConst590;
	double fConst591;
	double fConst592;
	double fConst593;
	double fConst594;
	double fConst595;
	double fConst596;
	double fConst597;
	double fConst598;
	double fConst599;
	double fConst600;
	double fConst601;
	double fConst602;
	double fConst603;
	double fConst604;
	double fConst605;
	double fConst606;
	double fConst607;
	double fConst608;
	double fConst609;
	double fConst610;
	double fRec373[3];
	double fConst611;
	double fConst612;
	double fConst613;
	double fRec372[3];
	double fConst614;
	double fConst615;
	double fConst616;
	double fRec371[3];
	double fConst617;
	double fConst618;
	double fConst619;
	double fRec370[3];
	double fConst620;
	double fConst621;
	double fConst622;
	double fRec369[3];
	double fConst623;
	double fConst624;
	double fConst625;
	double fRec368[3];
	double fConst626;
	double fRec367[3];
	double fRec366[3];
	double fRec365[3];
	double fRec364[3];
	double fRec363[3];
	double fRec362[3];
	double fRec361[2];
	double fRec385[3];
	double fRec384[3];
	double fRec383[3];
	double fRec382[3];
	double fRec381[3];
	double fRec380[3];
	double fRec379[3];
	double fRec378[3];
	double fRec377[3];
	double fRec376[3];
	double fRec375[3];
	double fRec374[3];
	double fConst627;
	double fConst628;
	double fConst629;
	double fConst630;
	double fConst631;
	double fConst632;
	double fConst633;
	double fConst634;
	double fConst635;
	double fConst636;
	double fConst637;
	double fConst638;
	double fConst639;
	double fConst640;
	double fConst641;
	double fConst642;
	double fConst643;
	double fConst644;
	double fConst645;
	double fConst646;
	double fConst647;
	double fConst648;
	double fConst649;
	double fConst650;
	double fRec398[3];
	double fConst651;
	double fConst652;
	double fConst653;
	double fRec397[3];
	double fConst654;
	double fConst655;
	double fConst656;
	double fRec396[3];
	double fConst657;
	double fConst658;
	double fConst659;
	double fRec395[3];
	double fConst660;
	double fConst661;
	double fConst662;
	double fRec394[3];
	double fConst663;
	double fConst664;
	double fConst665;
	double fRec393[3];
	double fConst666;
	double fRec392[3];
	double fRec391[3];
	double fRec390[3];
	double fRec389[3];
	double fRec388[3];
	double fRec387[3];
	double fRec386[2];
	double fRec410[3];
	double fRec409[3];
	double fRec408[3];
	double fRec407[3];
	double fRec406[3];
	double fRec405[3];
	double fRec404[3];
	double fRec403[3];
	double fRec402[3];
	double fRec401[3];
	double fRec400[3];
	double fRec399[3];
	double fConst667;
	double fConst668;
	double fConst669;
	double fConst670;
	double fConst671;
	double fConst672;
	double fConst673;
	double fConst674;
	double fConst675;
	double fConst676;
	double fConst677;
	double fConst678;
	double fConst679;
	double fConst680;
	double fConst681;
	double fConst682;
	double fConst683;
	double fConst684;
	double fConst685;
	double fConst686;
	double fConst687;
	double fConst688;
	double fConst689;
	double fConst690;
	double fRec423[3];
	double fConst691;
	double fConst692;
	double fConst693;
	double fRec422[3];
	double fConst694;
	double fConst695;
	double fConst696;
	double fRec421[3];
	double fConst697;
	double fConst698;
	double fConst699;
	double fRec420[3];
	double fConst700;
	double fConst701;
	double fConst702;
	double fRec419[3];
	double fConst703;
	double fConst704;
	double fConst705;
	double fRec418[3];
	double fConst706;
	double fRec417[3];
	double fRec416[3];
	double fRec415[3];
	double fRec414[3];
	double fRec413[3];
	double fRec412[3];
	double fRec411[2];
	double fRec435[3];
	double fRec434[3];
	double fRec433[3];
	double fRec432[3];
	double fRec431[3];
	double fRec430[3];
	double fRec429[3];
	double fRec428[3];
	double fRec427[3];
	double fRec426[3];
	double fRec425[3];
	double fRec424[3];
	double fConst707;
	double fConst708;
	double fConst709;
	double fConst710;
	double fConst711;
	double fConst712;
	double fConst713;
	double fConst714;
	double fConst715;
	double fConst716;
	double fConst717;
	double fConst718;
	double fConst719;
	double fConst720;
	double fConst721;
	double fConst722;
	double fConst723;
	double fConst724;
	double fConst725;
	double fConst726;
	double fConst727;
	double fConst728;
	double fConst729;
	double fConst730;
	double fRec448[3];
	double fConst731;
	double fConst732;
	double fConst733;
	double fRec447[3];
	double fConst734;
	double fConst735;
	double fConst736;
	double fRec446[3];
	double fConst737;
	double fConst738;
	double fConst739;
	double fRec445[3];
	double fConst740;
	double fConst741;
	double fConst742;
	double fRec444[3];
	double fConst743;
	double fConst744;
	double fConst745;
	double fRec443[3];
	double fConst746;
	double fRec442[3];
	double fRec441[3];
	double fRec440[3];
	double fRec439[3];
	double fRec438[3];
	double fRec437[3];
	double fRec436[2];
	double fRec460[3];
	double fRec459[3];
	double fRec458[3];
	double fRec457[3];
	double fRec456[3];
	double fRec455[3];
	double fRec454[3];
	double fRec453[3];
	double fRec452[3];
	double fRec451[3];
	double fRec450[3];
	double fRec449[3];
	double fConst747;
	double fConst748;
	double fConst749;
	double fConst750;
	double fConst751;
	double fConst752;
	double fConst753;
	double fConst754;
	double fConst755;
	double fConst756;
	double fConst757;
	double fConst758;
	double fConst759;
	double fConst760;
	double fConst761;
	double fConst762;
	double fConst763;
	double fConst764;
	double fConst765;
	double fConst766;
	double fConst767;
	double fConst768;
	double fConst769;
	double fConst770;
	double fRec473[3];
	double fConst771;
	double fConst772;
	double fConst773;
	double fRec472[3];
	double fConst774;
	double fConst775;
	double fConst776;
	double fRec471[3];
	double fConst777;
	double fConst778;
	double fConst779;
	double fRec470[3];
	double fConst780;
	double fConst781;
	double fConst782;
	double fRec469[3];
	double fConst783;
	double fConst784;
	double fConst785;
	double fRec468[3];
	double fConst786;
	double fRec467[3];
	double fRec466[3];
	double fRec465[3];
	double fRec464[3];
	double fRec463[3];
	double fRec462[3];
	double fRec461[2];
	double fRec485[3];
	double fRec484[3];
	double fRec483[3];
	double fRec482[3];
	double fRec481[3];
	double fRec480[3];
	double fRec479[3];
	double fRec478[3];
	double fRec477[3];
	double fRec476[3];
	double fRec475[3];
	double fRec474[3];
	double fConst787;
	double fConst788;
	double fConst789;
	double fConst790;
	double fConst791;
	double fConst792;
	double fConst793;
	double fConst794;
	double fConst795;
	double fConst796;
	double fConst797;
	double fConst798;
	double fConst799;
	double fConst800;
	double fConst801;
	double fConst802;
	double fConst803;
	double fConst804;
	double fConst805;
	double fConst806;
	double fConst807;
	double fConst808;
	double fConst809;
	double fConst810;
	double fRec498[3];
	double fConst811;
	double fConst812;
	double fConst813;
	double fRec497[3];
	double fConst814;
	double fConst815;
	double fConst816;
	double fRec496[3];
	double fConst817;
	double fConst818;
	double fConst819;
	double fRec495[3];
	double fConst820;
	double fConst821;
	double fConst822;
	double fRec494[3];
	double fConst823;
	double fConst824;
	double fConst825;
	double fRec493[3];
	double fConst826;
	double fRec492[3];
	double fRec491[3];
	double fRec490[3];
	double fRec489[3];
	double fRec488[3];
	double fRec487[3];
	double fRec486[2];
	double fRec510[3];
	double fRec509[3];
	double fRec508[3];
	double fRec507[3];
	double fRec506[3];
	double fRec505[3];
	double fRec504[3];
	double fRec503[3];
	double fRec502[3];
	double fRec501[3];
	double fRec500[3];
	double fRec499[3];
	double fConst827;
	double fConst828;
	double fConst829;
	double fConst830;
	double fConst831;
	double fConst832;
	double fConst833;
	double fConst834;
	double fConst835;
	double fConst836;
	double fConst837;
	double fConst838;
	double fConst839;
	double fConst840;
	double fConst841;
	double fConst842;
	double fConst843;
	double fConst844;
	double fConst845;
	double fConst846;
	double fConst847;
	double fConst848;
	double fConst849;
	double fConst850;
	double fRec523[3];
	double fConst851;
	double fConst852;
	double fConst853;
	double fRec522[3];
	double fConst854;
	double fConst855;
	double fConst856;
	double fRec521[3];
	double fConst857;
	double fConst858;
	double fConst859;
	double fRec520[3];
	double fConst860;
	double fConst861;
	double fConst862;
	double fRec519[3];
	double fConst863;
	double fConst864;
	double fConst865;
	double fRec518[3];
	double fConst866;
	double fRec517[3];
	double fRec516[3];
	double fRec515[3];
	double fRec514[3];
	double fRec513[3];
	double fRec512[3];
	double fRec511[2];
	double fRec535[3];
	double fRec534[3];
	double fRec533[3];
	double fRec532[3];
	double fRec531[3];
	double fRec530[3];
	double fRec529[3];
	double fRec528[3];
	double fRec527[3];
	double fRec526[3];
	double fRec525[3];
	double fRec524[3];
	double fConst867;
	double fConst868;
	double fConst869;
	double fConst870;
	double fConst871;
	double fConst872;
	double fConst873;
	double fConst874;
	double fConst875;
	double fConst876;
	double fConst877;
	double fConst878;
	double fConst879;
	double fConst880;
	double fConst881;
	double fConst882;
	double fConst883;
	double fConst884;
	double fConst885;
	double fConst886;
	double fConst887;
	double fConst888;
	double fConst889;
	double fConst890;
	double fRec548[3];
	double fConst891;
	double fConst892;
	double fConst893;
	double fRec547[3];
	double fConst894;
	double fConst895;
	double fConst896;
	double fRec546[3];
	double fConst897;
	double fConst898;
	double fConst899;
	double fRec545[3];
	double fConst900;
	double fConst901;
	double fConst902;
	double fRec544[3];
	double fConst903;
	double fConst904;
	double fConst905;
	double fRec543[3];
	double fConst906;
	double fRec542[3];
	double fRec541[3];
	double fRec540[3];
	double fRec539[3];
	double fRec538[3];
	double fRec537[3];
	double fRec536[2];
	double fRec560[3];
	double fRec559[3];
	double fRec558[3];
	double fRec557[3];
	double fRec556[3];
	double fRec555[3];
	double fRec554[3];
	double fRec553[3];
	double fRec552[3];
	double fRec551[3];
	double fRec550[3];
	double fRec549[3];
	double fConst907;
	double fConst908;
	double fConst909;
	double fConst910;
	double fConst911;
	double fConst912;
	double fConst913;
	double fConst914;
	double fConst915;
	double fConst916;
	double fConst917;
	double fConst918;
	double fConst919;
	double fConst920;
	double fConst921;
	double fConst922;
	double fConst923;
	double fConst924;
	double fConst925;
	double fConst926;
	double fConst927;
	double fConst928;
	double fConst929;
	double fConst930;
	double fRec573[3];
	double fConst931;
	double fConst932;
	double fConst933;
	double fRec572[3];
	double fConst934;
	double fConst935;
	double fConst936;
	double fRec571[3];
	double fConst937;
	double fConst938;
	double fConst939;
	double fRec570[3];
	double fConst940;
	double fConst941;
	double fConst942;
	double fRec569[3];
	double fConst943;
	double fConst944;
	double fConst945;
	double fRec568[3];
	double fConst946;
	double fRec567[3];
	double fRec566[3];
	double fRec565[3];
	double fRec564[3];
	double fRec563[3];
	double fRec562[3];
	double fRec561[2];
	double fRec585[3];
	double fRec584[3];
	double fRec583[3];
	double fRec582[3];
	double fRec581[3];
	double fRec580[3];
	double fRec579[3];
	double fRec578[3];
	double fRec577[3];
	double fRec576[3];
	double fRec575[3];
	double fRec574[3];
	double fConst947;
	double fConst948;
	double fConst949;
	double fConst950;
	double fConst951;
	double fConst952;
	double fConst953;
	double fConst954;
	double fConst955;
	double fConst956;
	double fConst957;
	double fConst958;
	double fConst959;
	double fConst960;
	double fConst961;
	double fConst962;
	double fConst963;
	double fConst964;
	double fConst965;
	double fConst966;
	double fConst967;
	double fConst968;
	double fConst969;
	double fConst970;
	double fRec598[3];
	double fConst971;
	double fConst972;
	double fConst973;
	double fRec597[3];
	double fConst974;
	double fConst975;
	double fConst976;
	double fRec596[3];
	double fConst977;
	double fConst978;
	double fConst979;
	double fRec595[3];
	double fConst980;
	double fConst981;
	double fConst982;
	double fRec594[3];
	double fConst983;
	double fConst984;
	double fConst985;
	double fRec593[3];
	double fConst986;
	double fRec592[3];
	double fRec591[3];
	double fRec590[3];
	double fRec589[3];
	double fRec588[3];
	double fRec587[3];
	double fRec586[2];
	double fRec610[3];
	double fRec609[3];
	double fRec608[3];
	double fRec607[3];
	double fRec606[3];
	double fRec605[3];
	double fRec604[3];
	double fRec603[3];
	double fRec602[3];
	double fRec601[3];
	double fRec600[3];
	double fRec599[3];
	double fConst987;
	double fConst988;
	double fConst989;
	double fConst990;
	double fConst991;
	double fConst992;
	double fConst993;
	double fConst994;
	double fConst995;
	double fConst996;
	double fConst997;
	double fConst998;
	double fConst999;
	double fConst1000;
	double fConst1001;
	double fConst1002;
	double fConst1003;
	double fConst1004;
	double fConst1005;
	double fConst1006;
	double fConst1007;
	double fConst1008;
	double fConst1009;
	double fConst1010;
	double fRec623[3];
	double fConst1011;
	double fConst1012;
	double fConst1013;
	double fRec622[3];
	double fConst1014;
	double fConst1015;
	double fConst1016;
	double fRec621[3];
	double fConst1017;
	double fConst1018;
	double fConst1019;
	double fRec620[3];
	double fConst1020;
	double fConst1021;
	double fConst1022;
	double fRec619[3];
	double fConst1023;
	double fConst1024;
	double fConst1025;
	double fRec618[3];
	double fConst1026;
	double fRec617[3];
	double fRec616[3];
	double fRec615[3];
	double fRec614[3];
	double fRec613[3];
	double fRec612[3];
	double fRec611[2];
	double fRec635[3];
	double fRec634[3];
	double fRec633[3];
	double fRec632[3];
	double fRec631[3];
	double fRec630[3];
	double fRec629[3];
	double fRec628[3];
	double fRec627[3];
	double fRec626[3];
	double fRec625[3];
	double fRec624[3];
	double fConst1027;
	double fConst1028;
	double fConst1029;
	double fConst1030;
	double fConst1031;
	double fConst1032;
	double fConst1033;
	double fConst1034;
	double fConst1035;
	double fConst1036;
	double fConst1037;
	double fConst1038;
	double fConst1039;
	double fConst1040;
	double fConst1041;
	double fConst1042;
	double fConst1043;
	double fConst1044;
	double fConst1045;
	double fConst1046;
	double fConst1047;
	double fConst1048;
	double fConst1049;
	double fConst1050;
	double fRec648[3];
	double fConst1051;
	double fConst1052;
	double fConst1053;
	double fRec647[3];
	double fConst1054;
	double fConst1055;
	double fConst1056;
	double fRec646[3];
	double fConst1057;
	double fConst1058;
	double fConst1059;
	double fRec645[3];
	double fConst1060;
	double fConst1061;
	double fConst1062;
	double fRec644[3];
	double fConst1063;
	double fConst1064;
	double fConst1065;
	double fRec643[3];
	double fConst1066;
	double fRec642[3];
	double fRec641[3];
	double fRec640[3];
	double fRec639[3];
	double fRec638[3];
	double fRec637[3];
	double fRec636[2];
	double fRec660[3];
	double fRec659[3];
	double fRec658[3];
	double fRec657[3];
	double fRec656[3];
	double fRec655[3];
	double fRec654[3];
	double fRec653[3];
	double fRec652[3];
	double fRec651[3];
	double fRec650[3];
	double fRec649[3];
	double fConst1067;
	double fConst1068;
	double fConst1069;
	double fConst1070;
	double fConst1071;
	double fConst1072;
	double fConst1073;
	double fConst1074;
	double fConst1075;
	double fConst1076;
	double fConst1077;
	double fConst1078;
	double fConst1079;
	double fConst1080;
	double fConst1081;
	double fConst1082;
	double fConst1083;
	double fConst1084;
	double fConst1085;
	double fConst1086;
	double fConst1087;
	double fConst1088;
	double fConst1089;
	double fConst1090;
	double fRec673[3];
	double fConst1091;
	double fConst1092;
	double fConst1093;
	double fRec672[3];
	double fConst1094;
	double fConst1095;
	double fConst1096;
	double fRec671[3];
	double fConst1097;
	double fConst1098;
	double fConst1099;
	double fRec670[3];
	double fConst1100;
	double fConst1101;
	double fConst1102;
	double fRec669[3];
	double fConst1103;
	double fConst1104;
	double fConst1105;
	double fRec668[3];
	double fConst1106;
	double fRec667[3];
	double fRec666[3];
	double fRec665[3];
	double fRec664[3];
	double fRec663[3];
	double fRec662[3];
	double fRec661[2];
	double fRec685[3];
	double fRec684[3];
	double fRec683[3];
	double fRec682[3];
	double fRec681[3];
	double fRec680[3];
	double fRec679[3];
	double fRec678[3];
	double fRec677[3];
	double fRec676[3];
	double fRec675[3];
	double fRec674[3];
	double fConst1107;
	double fConst1108;
	double fConst1109;
	double fConst1110;
	double fConst1111;
	double fConst1112;
	double fConst1113;
	double fConst1114;
	double fConst1115;
	double fConst1116;
	double fConst1117;
	double fConst1118;
	double fConst1119;
	double fConst1120;
	double fConst1121;
	double fConst1122;
	double fConst1123;
	double fConst1124;
	double fConst1125;
	double fConst1126;
	double fConst1127;
	double fConst1128;
	double fConst1129;
	double fConst1130;
	double fRec698[3];
	double fConst1131;
	double fConst1132;
	double fConst1133;
	double fRec697[3];
	double fConst1134;
	double fConst1135;
	double fConst1136;
	double fRec696[3];
	double fConst1137;
	double fConst1138;
	double fConst1139;
	double fRec695[3];
	double fConst1140;
	double fConst1141;
	double fConst1142;
	double fRec694[3];
	double fConst1143;
	double fConst1144;
	double fConst1145;
	double fRec693[3];
	double fConst1146;
	double fRec692[3];
	double fRec691[3];
	double fRec690[3];
	double fRec689[3];
	double fRec688[3];
	double fRec687[3];
	double fRec686[2];
	double fRec710[3];
	double fRec709[3];
	double fRec708[3];
	double fRec707[3];
	double fRec706[3];
	double fRec705[3];
	double fRec704[3];
	double fRec703[3];
	double fRec702[3];
	double fRec701[3];
	double fRec700[3];
	double fRec699[3];
	double fConst1147;
	double fConst1148;
	double fConst1149;
	double fConst1150;
	double fConst1151;
	double fConst1152;
	double fConst1153;
	double fConst1154;
	double fConst1155;
	double fConst1156;
	double fConst1157;
	double fConst1158;
	double fConst1159;
	double fConst1160;
	double fConst1161;
	double fConst1162;
	double fConst1163;
	double fConst1164;
	double fConst1165;
	double fConst1166;
	double fConst1167;
	double fConst1168;
	double fConst1169;
	double fConst1170;
	double fRec723[3];
	double fConst1171;
	double fConst1172;
	double fConst1173;
	double fRec722[3];
	double fConst1174;
	double fConst1175;
	double fConst1176;
	double fRec721[3];
	double fConst1177;
	double fConst1178;
	double fConst1179;
	double fRec720[3];
	double fConst1180;
	double fConst1181;
	double fConst1182;
	double fRec719[3];
	double fConst1183;
	double fConst1184;
	double fConst1185;
	double fRec718[3];
	double fConst1186;
	double fRec717[3];
	double fRec716[3];
	double fRec715[3];
	double fRec714[3];
	double fRec713[3];
	double fRec712[3];
	double fRec711[2];
	double fRec735[3];
	double fRec734[3];
	double fRec733[3];
	double fRec732[3];
	double fRec731[3];
	double fRec730[3];
	double fRec729[3];
	double fRec728[3];
	double fRec727[3];
	double fRec726[3];
	double fRec725[3];
	double fRec724[3];
	double fConst1187;
	double fConst1188;
	double fConst1189;
	double fConst1190;
	double fConst1191;
	double fConst1192;
	double fConst1193;
	double fConst1194;
	double fConst1195;
	double fConst1196;
	double fConst1197;
	double fConst1198;
	double fConst1199;
	double fConst1200;
	double fConst1201;
	double fConst1202;
	double fConst1203;
	double fConst1204;
	double fConst1205;
	double fConst1206;
	double fConst1207;
	double fConst1208;
	double fConst1209;
	double fConst1210;
	double fRec748[3];
	double fConst1211;
	double fConst1212;
	double fConst1213;
	double fRec747[3];
	double fConst1214;
	double fConst1215;
	double fConst1216;
	double fRec746[3];
	double fConst1217;
	double fConst1218;
	double fConst1219;
	double fRec745[3];
	double fConst1220;
	double fConst1221;
	double fConst1222;
	double fRec744[3];
	double fConst1223;
	double fConst1224;
	double fConst1225;
	double fRec743[3];
	double fConst1226;
	double fRec742[3];
	double fRec741[3];
	double fRec740[3];
	double fRec739[3];
	double fRec738[3];
	double fRec737[3];
	double fRec736[2];
	double fRec760[3];
	double fRec759[3];
	double fRec758[3];
	double fRec757[3];
	double fRec756[3];
	double fRec755[3];
	double fRec754[3];
	double fRec753[3];
	double fRec752[3];
	double fRec751[3];
	double fRec750[3];
	double fRec749[3];
	double fConst1227;
	double fConst1228;
	double fConst1229;
	double fConst1230;
	double fConst1231;
	double fConst1232;
	double fConst1233;
	double fConst1234;
	double fConst1235;
	double fConst1236;
	double fConst1237;
	double fConst1238;
	double fConst1239;
	double fConst1240;
	double fConst1241;
	double fConst1242;
	double fConst1243;
	double fConst1244;
	double fConst1245;
	double fConst1246;
	double fConst1247;
	double fConst1248;
	double fConst1249;
	double fConst1250;
	double fRec773[3];
	double fConst1251;
	double fConst1252;
	double fConst1253;
	double fRec772[3];
	double fConst1254;
	double fConst1255;
	double fConst1256;
	double fRec771[3];
	double fConst1257;
	double fConst1258;
	double fConst1259;
	double fRec770[3];
	double fConst1260;
	double fConst1261;
	double fConst1262;
	double fRec769[3];
	double fConst1263;
	double fConst1264;
	double fConst1265;
	double fRec768[3];
	double fConst1266;
	double fRec767[3];
	double fRec766[3];
	double fRec765[3];
	double fRec764[3];
	double fRec763[3];
	double fRec762[3];
	double fRec761[2];
	double fRec785[3];
	double fRec784[3];
	double fRec783[3];
	double fRec782[3];
	double fRec781[3];
	double fRec780[3];
	double fRec779[3];
	double fRec778[3];
	double fRec777[3];
	double fRec776[3];
	double fRec775[3];
	double fRec774[3];
	double fConst1267;
	double fConst1268;
	double fConst1269;
	double fConst1270;
	double fConst1271;
	double fConst1272;
	double fConst1273;
	double fConst1274;
	double fConst1275;
	double fConst1276;
	double fConst1277;
	double fConst1278;
	double fConst1279;
	double fConst1280;
	double fConst1281;
	double fConst1282;
	double fConst1283;
	double fConst1284;
	double fConst1285;
	double fConst1286;
	double fConst1287;
	double fConst1288;
	double fConst1289;
	double fConst1290;
	double fRec798[3];
	double fConst1291;
	double fConst1292;
	double fConst1293;
	double fRec797[3];
	double fConst1294;
	double fConst1295;
	double fConst1296;
	double fRec796[3];
	double fConst1297;
	double fConst1298;
	double fConst1299;
	double fRec795[3];
	double fConst1300;
	double fConst1301;
	double fConst1302;
	double fRec794[3];
	double fConst1303;
	double fConst1304;
	double fConst1305;
	double fRec793[3];
	double fConst1306;
	double fRec792[3];
	double fRec791[3];
	double fRec790[3];
	double fRec789[3];
	double fRec788[3];
	double fRec787[3];
	double fRec786[2];
	double fRec810[3];
	double fRec809[3];
	double fRec808[3];
	double fRec807[3];
	double fRec806[3];
	double fRec805[3];
	double fRec804[3];
	double fRec803[3];
	double fRec802[3];
	double fRec801[3];
	double fRec800[3];
	double fRec799[3];
	double fConst1307;
	double fConst1308;
	double fConst1309;
	double fConst1310;
	double fConst1311;
	double fConst1312;
	double fConst1313;
	double fConst1314;
	double fConst1315;
	double fConst1316;
	double fConst1317;
	double fConst1318;
	double fConst1319;
	double fConst1320;
	double fConst1321;
	double fConst1322;
	double fConst1323;
	double fConst1324;
	double fConst1325;
	double fConst1326;
	double fConst1327;
	double fConst1328;
	double fConst1329;
	double fConst1330;
	double fRec823[3];
	double fConst1331;
	double fConst1332;
	double fConst1333;
	double fRec822[3];
	double fConst1334;
	double fConst1335;
	double fConst1336;
	double fRec821[3];
	double fConst1337;
	double fConst1338;
	double fConst1339;
	double fRec820[3];
	double fConst1340;
	double fConst1341;
	double fConst1342;
	double fRec819[3];
	double fConst1343;
	double fConst1344;
	double fConst1345;
	double fRec818[3];
	double fConst1346;
	double fRec817[3];
	double fRec816[3];
	double fRec815[3];
	double fRec814[3];
	double fRec813[3];
	double fRec812[3];
	double fRec811[2];
	double fRec835[3];
	double fRec834[3];
	double fRec833[3];
	double fRec832[3];
	double fRec831[3];
	double fRec830[3];
	double fRec829[3];
	double fRec828[3];
	double fRec827[3];
	double fRec826[3];
	double fRec825[3];
	double fRec824[3];
	double fConst1347;
	double fConst1348;
	double fConst1349;
	double fConst1350;
	double fConst1351;
	double fConst1352;
	double fConst1353;
	double fConst1354;
	double fConst1355;
	double fConst1356;
	double fConst1357;
	double fConst1358;
	double fConst1359;
	double fConst1360;
	double fConst1361;
	double fConst1362;
	double fConst1363;
	double fConst1364;
	double fConst1365;
	double fConst1366;
	double fConst1367;
	double fConst1368;
	double fConst1369;
	double fConst1370;
	double fRec848[3];
	double fConst1371;
	double fConst1372;
	double fConst1373;
	double fRec847[3];
	double fConst1374;
	double fConst1375;
	double fConst1376;
	double fRec846[3];
	double fConst1377;
	double fConst1378;
	double fConst1379;
	double fRec845[3];
	double fConst1380;
	double fConst1381;
	double fConst1382;
	double fRec844[3];
	double fConst1383;
	double fConst1384;
	double fConst1385;
	double fRec843[3];
	double fConst1386;
	double fRec842[3];
	double fRec841[3];
	double fRec840[3];
	double fRec839[3];
	double fRec838[3];
	double fRec837[3];
	double fRec836[2];
	double fRec860[3];
	double fRec859[3];
	double fRec858[3];
	double fRec857[3];
	double fRec856[3];
	double fRec855[3];
	double fRec854[3];
	double fRec853[3];
	double fRec852[3];
	double fRec851[3];
	double fRec850[3];
	double fRec849[3];
	double fConst1387;
	double fConst1388;
	double fConst1389;
	double fConst1390;
	double fConst1391;
	double fConst1392;
	double fConst1393;
	double fConst1394;
	double fConst1395;
	double fConst1396;
	double fConst1397;
	double fConst1398;
	double fConst1399;
	double fConst1400;
	double fConst1401;
	double fConst1402;
	double fConst1403;
	double fConst1404;
	double fConst1405;
	double fConst1406;
	double fConst1407;
	double fConst1408;
	double fConst1409;
	double fConst1410;
	double fRec873[3];
	double fConst1411;
	double fConst1412;
	double fConst1413;
	double fRec872[3];
	double fConst1414;
	double fConst1415;
	double fConst1416;
	double fRec871[3];
	double fConst1417;
	double fConst1418;
	double fConst1419;
	double fRec870[3];
	double fConst1420;
	double fConst1421;
	double fConst1422;
	double fRec869[3];
	double fConst1423;
	double fConst1424;
	double fConst1425;
	double fRec868[3];
	double fConst1426;
	double fRec867[3];
	double fRec866[3];
	double fRec865[3];
	double fRec864[3];
	double fRec863[3];
	double fRec862[3];
	double fRec861[2];
	double fRec885[3];
	double fRec884[3];
	double fRec883[3];
	double fRec882[3];
	double fRec881[3];
	double fRec880[3];
	double fRec879[3];
	double fRec878[3];
	double fRec877[3];
	double fRec876[3];
	double fRec875[3];
	double fRec874[3];
	double fConst1427;
	double fConst1428;
	double fConst1429;
	double fConst1430;
	double fConst1431;
	double fConst1432;
	double fConst1433;
	double fConst1434;
	double fConst1435;
	double fConst1436;
	double fConst1437;
	double fConst1438;
	double fConst1439;
	double fConst1440;
	double fConst1441;
	double fConst1442;
	double fConst1443;
	double fConst1444;
	double fConst1445;
	double fConst1446;
	double fConst1447;
	double fConst1448;
	double fConst1449;
	double fConst1450;
	double fRec898[3];
	double fConst1451;
	double fConst1452;
	double fConst1453;
	double fRec897[3];
	double fConst1454;
	double fConst1455;
	double fConst1456;
	double fRec896[3];
	double fConst1457;
	double fConst1458;
	double fConst1459;
	double fRec895[3];
	double fConst1460;
	double fConst1461;
	double fConst1462;
	double fRec894[3];
	double fConst1463;
	double fConst1464;
	double fConst1465;
	double fRec893[3];
	double fConst1466;
	double fRec892[3];
	double fRec891[3];
	double fRec890[3];
	double fRec889[3];
	double fRec888[3];
	double fRec887[3];
	double fRec886[2];
	double fRec910[3];
	double fRec909[3];
	double fRec908[3];
	double fRec907[3];
	double fRec906[3];
	double fRec905[3];
	double fRec904[3];
	double fRec903[3];
	double fRec902[3];
	double fRec901[3];
	double fRec900[3];
	double fRec899[3];
	double fConst1467;
	double fConst1468;
	double fRec0[2];
	
 public:
	piece_nono_quando_vocoder() {
	}
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/amp_follower_ar:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.2.0");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.16.0");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn piece_nono_quando_vocoder -scn piece_nono_quando_vocoder_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0");
		m->declare("compressors.lib/FFcompressor_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/FFcompressor_N_chan:license", "GPLv3");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_compression_gain_N_chan_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_compression_gain_N_chan_db:license", "GPLv3");
		m->declare("compressors.lib/peak_compression_gain_mono_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_compression_gain_mono_db:license", "GPLv3");
		m->declare("compressors.lib/version", "1.6.0");
		m->declare("filename", "piece_nono_quando_vocoder.dsp");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass6e:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass6e:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass6e:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass6e:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass6e:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass6e:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/spectral_tilt:author", "Julius O. Smith III");
		m->declare("filters.lib/spectral_tilt:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/spectral_tilt:license", "MIT-style STK-4.3 license");
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
		m->declare("filters.lib/version", "1.3.0");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "nono.quando.vocoder");
		m->declare("noises.lib/colored_noise:author", "Constantinos Odysseas Economou");
		m->declare("noises.lib/colored_noise:copyright", "Copyright (C) 2022 Constantinos Odysseas Economou <c.economou@sirenfx.io>");
		m->declare("noises.lib/colored_noise:license", "MIT-style STK-4.3 license");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
		m->declare("signals.lib/version", "1.5.0");
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
		fConst0 = std::min<double>(1.92e+05, std::max<double>(1.0, double(fSampleRate)));
		fConst1 = std::tan(205.48023848490934 / fConst0);
		fConst2 = 1.0 / fConst1;
		fConst3 = (fConst2 + 0.168404871113589) / fConst1 + 1.069358407707312;
		fConst4 = 1.0 / fConst3;
		fConst5 = 3.0 / fConst3;
		fConst6 = piece_nono_quando_vocoder_faustpower2_f(fConst1);
		fConst7 = 1.0 / fConst6;
		fConst8 = fConst7 + 53.53615295455673;
		fConst9 = 1.0 / ((fConst2 + 0.512478641889141) / fConst1 + 0.689621364484675);
		fConst10 = fConst7 + 7.621731298870603;
		fConst11 = 1.0 / ((fConst2 + 0.782413046821645) / fConst1 + 0.24529150870616);
		fConst12 = 9.9999997055e-05 / fConst6;
		fConst13 = fConst12 + 0.000433227200555;
		fConst14 = 1.0 / ((fConst2 + 0.157482159302087) / fConst1 + 0.9351401670315425);
		fConst15 = 50.063807016150385 / fConst6;
		fConst16 = fConst15 + 0.9351401670315425;
		fConst17 = 1.0 / ((fConst2 + 0.7431304601070396) / fConst1 + 1.450071084655647);
		fConst18 = 11.052052171507189 / fConst6;
		fConst19 = fConst18 + 1.450071084655647;
		fConst20 = 1.0 / ((fConst2 + 3.1897274020965583) / fConst1 + 4.076781969643807);
		fConst21 = 0.0017661728399818856 / fConst6;
		fConst22 = fConst21 + 0.00040767818495825777;
		fConst23 = (fConst2 + -3.1897274020965583) / fConst1 + 4.076781969643807;
		fConst24 = 2.0 * (4.076781969643807 - fConst7);
		fConst25 = 2.0 * (0.00040767818495825777 - fConst21);
		fConst26 = (fConst2 + -0.7431304601070396) / fConst1 + 1.450071084655647;
		fConst27 = 2.0 * (1.450071084655647 - fConst7);
		fConst28 = 2.0 * (1.450071084655647 - fConst18);
		fConst29 = (fConst2 + -0.157482159302087) / fConst1 + 0.9351401670315425;
		fConst30 = 2.0 * (0.9351401670315425 - fConst7);
		fConst31 = 2.0 * (0.9351401670315425 - fConst15);
		fConst32 = (fConst2 + -0.782413046821645) / fConst1 + 0.24529150870616;
		fConst33 = 2.0 * (0.24529150870616 - fConst7);
		fConst34 = 2.0 * (0.000433227200555 - fConst12);
		fConst35 = (fConst2 + -0.512478641889141) / fConst1 + 0.689621364484675;
		fConst36 = 2.0 * (0.689621364484675 - fConst7);
		fConst37 = 2.0 * (7.621731298870603 - fConst7);
		fConst38 = (fConst2 + -0.168404871113589) / fConst1 + 1.069358407707312;
		fConst39 = 2.0 * (1.069358407707312 - fConst7);
		fConst40 = 2.0 * (53.53615295455673 - fConst7);
		fConst41 = 0.0441 / fConst0;
		fConst42 = 44.1 / fConst0;
		fConst43 = 1.0 - fConst42;
		fConst44 = 1.0 / fConst0;
		fConst45 = std::tan(62831.85307179588 / fConst0);
		fConst46 = 62.83185307179586 / fConst0;
		fConst47 = 1.0 / std::tan(0.5 / fConst0);
		fConst48 = std::tan(fConst46);
		fConst49 = 125.66370614359172 * (fConst45 / fConst48);
		fConst50 = 1.0 / (fConst47 + fConst49);
		fConst51 = std::tan(11173.259061216544 / fConst0);
		fConst52 = 125.66370614359172 / fConst48;
		fConst53 = 125.66370614359172 * (fConst51 / fConst48);
		fConst54 = 1.0 / (fConst47 + fConst53);
		fConst55 = std::tan(1986.9176531592204 / fConst0);
		fConst56 = 125.66370614359172 * (fConst55 / fConst48);
		fConst57 = 1.0 / (fConst47 + fConst56);
		fConst58 = std::tan(353.3294752055899 / fConst0);
		fConst59 = 125.66370614359172 * (fConst58 / fConst48);
		fConst60 = 1.0 / (fConst47 + fConst59);
		fConst61 = 1.0 / (fConst47 + 125.66370614359172);
		fConst62 = 125.66370614359172 - fConst47;
		fConst63 = fConst59 - fConst47;
		fConst64 = fConst56 - fConst47;
		fConst65 = fConst53 - fConst47;
		fConst66 = fConst49 - fConst47;
		fConst67 = std::tan(230.6437693768698 / fConst0);
		fConst68 = 1.0 / fConst67;
		fConst69 = (fConst68 + 0.168404871113589) / fConst67 + 1.069358407707312;
		fConst70 = 1.0 / fConst69;
		fConst71 = 3.0 / fConst69;
		fConst72 = piece_nono_quando_vocoder_faustpower2_f(fConst67);
		fConst73 = 1.0 / fConst72;
		fConst74 = fConst73 + 53.53615295455673;
		fConst75 = 1.0 / ((fConst68 + 0.512478641889141) / fConst67 + 0.689621364484675);
		fConst76 = fConst73 + 7.621731298870603;
		fConst77 = 1.0 / ((fConst68 + 0.782413046821645) / fConst67 + 0.24529150870616);
		fConst78 = 9.9999997055e-05 / fConst72;
		fConst79 = fConst78 + 0.000433227200555;
		fConst80 = 1.0 / ((fConst68 + 0.157482159302087) / fConst67 + 0.9351401670315425);
		fConst81 = 50.063807016150385 / fConst72;
		fConst82 = fConst81 + 0.9351401670315425;
		fConst83 = 1.0 / ((fConst68 + 0.7431304601070396) / fConst67 + 1.450071084655647);
		fConst84 = 11.052052171507189 / fConst72;
		fConst85 = fConst84 + 1.450071084655647;
		fConst86 = 1.0 / ((fConst68 + 3.1897274020965583) / fConst67 + 4.076781969643807);
		fConst87 = 0.0017661728399818856 / fConst72;
		fConst88 = fConst87 + 0.00040767818495825777;
		fConst89 = (fConst68 + -3.1897274020965583) / fConst67 + 4.076781969643807;
		fConst90 = 2.0 * (4.076781969643807 - fConst73);
		fConst91 = 2.0 * (0.00040767818495825777 - fConst87);
		fConst92 = (fConst68 + -0.7431304601070396) / fConst67 + 1.450071084655647;
		fConst93 = 2.0 * (1.450071084655647 - fConst73);
		fConst94 = 2.0 * (1.450071084655647 - fConst84);
		fConst95 = (fConst68 + -0.157482159302087) / fConst67 + 0.9351401670315425;
		fConst96 = 2.0 * (0.9351401670315425 - fConst73);
		fConst97 = 2.0 * (0.9351401670315425 - fConst81);
		fConst98 = (fConst68 + -0.782413046821645) / fConst67 + 0.24529150870616;
		fConst99 = 2.0 * (0.24529150870616 - fConst73);
		fConst100 = 2.0 * (0.000433227200555 - fConst78);
		fConst101 = (fConst68 + -0.512478641889141) / fConst67 + 0.689621364484675;
		fConst102 = 2.0 * (0.689621364484675 - fConst73);
		fConst103 = 2.0 * (7.621731298870603 - fConst73);
		fConst104 = (fConst68 + -0.168404871113589) / fConst67 + 1.069358407707312;
		fConst105 = 2.0 * (1.069358407707312 - fConst73);
		fConst106 = 2.0 * (53.53615295455673 - fConst73);
		fConst107 = std::tan(258.8888778045559 / fConst0);
		fConst108 = 1.0 / fConst107;
		fConst109 = (fConst108 + 0.168404871113589) / fConst107 + 1.069358407707312;
		fConst110 = 1.0 / fConst109;
		fConst111 = 3.0 / fConst109;
		fConst112 = piece_nono_quando_vocoder_faustpower2_f(fConst107);
		fConst113 = 1.0 / fConst112;
		fConst114 = fConst113 + 53.53615295455673;
		fConst115 = 1.0 / ((fConst108 + 0.512478641889141) / fConst107 + 0.689621364484675);
		fConst116 = fConst113 + 7.621731298870603;
		fConst117 = 1.0 / ((fConst108 + 0.782413046821645) / fConst107 + 0.24529150870616);
		fConst118 = 9.9999997055e-05 / fConst112;
		fConst119 = fConst118 + 0.000433227200555;
		fConst120 = 1.0 / ((fConst108 + 0.157482159302087) / fConst107 + 0.9351401670315425);
		fConst121 = 50.063807016150385 / fConst112;
		fConst122 = fConst121 + 0.9351401670315425;
		fConst123 = 1.0 / ((fConst108 + 0.7431304601070396) / fConst107 + 1.450071084655647);
		fConst124 = 11.052052171507189 / fConst112;
		fConst125 = fConst124 + 1.450071084655647;
		fConst126 = 1.0 / ((fConst108 + 3.1897274020965583) / fConst107 + 4.076781969643807);
		fConst127 = 0.0017661728399818856 / fConst112;
		fConst128 = fConst127 + 0.00040767818495825777;
		fConst129 = (fConst108 + -3.1897274020965583) / fConst107 + 4.076781969643807;
		fConst130 = 2.0 * (4.076781969643807 - fConst113);
		fConst131 = 2.0 * (0.00040767818495825777 - fConst127);
		fConst132 = (fConst108 + -0.7431304601070396) / fConst107 + 1.450071084655647;
		fConst133 = 2.0 * (1.450071084655647 - fConst113);
		fConst134 = 2.0 * (1.450071084655647 - fConst124);
		fConst135 = (fConst108 + -0.157482159302087) / fConst107 + 0.9351401670315425;
		fConst136 = 2.0 * (0.9351401670315425 - fConst113);
		fConst137 = 2.0 * (0.9351401670315425 - fConst121);
		fConst138 = (fConst108 + -0.782413046821645) / fConst107 + 0.24529150870616;
		fConst139 = 2.0 * (0.24529150870616 - fConst113);
		fConst140 = 2.0 * (0.000433227200555 - fConst118);
		fConst141 = (fConst108 + -0.512478641889141) / fConst107 + 0.689621364484675;
		fConst142 = 2.0 * (0.689621364484675 - fConst113);
		fConst143 = 2.0 * (7.621731298870603 - fConst113);
		fConst144 = (fConst108 + -0.168404871113589) / fConst107 + 1.069358407707312;
		fConst145 = 2.0 * (1.069358407707312 - fConst113);
		fConst146 = 2.0 * (53.53615295455673 - fConst113);
		fConst147 = std::tan(290.59294006501676 / fConst0);
		fConst148 = 1.0 / fConst147;
		fConst149 = (fConst148 + 0.168404871113589) / fConst147 + 1.069358407707312;
		fConst150 = 1.0 / fConst149;
		fConst151 = 3.0 / fConst149;
		fConst152 = piece_nono_quando_vocoder_faustpower2_f(fConst147);
		fConst153 = 1.0 / fConst152;
		fConst154 = fConst153 + 53.53615295455673;
		fConst155 = 1.0 / ((fConst148 + 0.512478641889141) / fConst147 + 0.689621364484675);
		fConst156 = fConst153 + 7.621731298870603;
		fConst157 = 1.0 / ((fConst148 + 0.782413046821645) / fConst147 + 0.24529150870616);
		fConst158 = 9.9999997055e-05 / fConst152;
		fConst159 = fConst158 + 0.000433227200555;
		fConst160 = 1.0 / ((fConst148 + 0.157482159302087) / fConst147 + 0.9351401670315425);
		fConst161 = 50.063807016150385 / fConst152;
		fConst162 = fConst161 + 0.9351401670315425;
		fConst163 = 1.0 / ((fConst148 + 0.7431304601070396) / fConst147 + 1.450071084655647);
		fConst164 = 11.052052171507189 / fConst152;
		fConst165 = fConst164 + 1.450071084655647;
		fConst166 = 1.0 / ((fConst148 + 3.1897274020965583) / fConst147 + 4.076781969643807);
		fConst167 = 0.0017661728399818856 / fConst152;
		fConst168 = fConst167 + 0.00040767818495825777;
		fConst169 = (fConst148 + -3.1897274020965583) / fConst147 + 4.076781969643807;
		fConst170 = 2.0 * (4.076781969643807 - fConst153);
		fConst171 = 2.0 * (0.00040767818495825777 - fConst167);
		fConst172 = (fConst148 + -0.7431304601070396) / fConst147 + 1.450071084655647;
		fConst173 = 2.0 * (1.450071084655647 - fConst153);
		fConst174 = 2.0 * (1.450071084655647 - fConst164);
		fConst175 = (fConst148 + -0.157482159302087) / fConst147 + 0.9351401670315425;
		fConst176 = 2.0 * (0.9351401670315425 - fConst153);
		fConst177 = 2.0 * (0.9351401670315425 - fConst161);
		fConst178 = (fConst148 + -0.782413046821645) / fConst147 + 0.24529150870616;
		fConst179 = 2.0 * (0.24529150870616 - fConst153);
		fConst180 = 2.0 * (0.000433227200555 - fConst158);
		fConst181 = (fConst148 + -0.512478641889141) / fConst147 + 0.689621364484675;
		fConst182 = 2.0 * (0.689621364484675 - fConst153);
		fConst183 = 2.0 * (7.621731298870603 - fConst153);
		fConst184 = (fConst148 + -0.168404871113589) / fConst147 + 1.069358407707312;
		fConst185 = 2.0 * (1.069358407707312 - fConst153);
		fConst186 = 2.0 * (53.53615295455673 - fConst153);
		fConst187 = std::tan(326.17954672962156 / fConst0);
		fConst188 = 1.0 / fConst187;
		fConst189 = (fConst188 + 0.168404871113589) / fConst187 + 1.069358407707312;
		fConst190 = 1.0 / fConst189;
		fConst191 = 3.0 / fConst189;
		fConst192 = piece_nono_quando_vocoder_faustpower2_f(fConst187);
		fConst193 = 1.0 / fConst192;
		fConst194 = fConst193 + 53.53615295455673;
		fConst195 = 1.0 / ((fConst188 + 0.512478641889141) / fConst187 + 0.689621364484675);
		fConst196 = fConst193 + 7.621731298870603;
		fConst197 = 1.0 / ((fConst188 + 0.782413046821645) / fConst187 + 0.24529150870616);
		fConst198 = 9.9999997055e-05 / fConst192;
		fConst199 = fConst198 + 0.000433227200555;
		fConst200 = 1.0 / ((fConst188 + 0.157482159302087) / fConst187 + 0.9351401670315425);
		fConst201 = 50.063807016150385 / fConst192;
		fConst202 = fConst201 + 0.9351401670315425;
		fConst203 = 1.0 / ((fConst188 + 0.7431304601070396) / fConst187 + 1.450071084655647);
		fConst204 = 11.052052171507189 / fConst192;
		fConst205 = fConst204 + 1.450071084655647;
		fConst206 = 1.0 / ((fConst188 + 3.1897274020965583) / fConst187 + 4.076781969643807);
		fConst207 = 0.0017661728399818856 / fConst192;
		fConst208 = fConst207 + 0.00040767818495825777;
		fConst209 = (fConst188 + -3.1897274020965583) / fConst187 + 4.076781969643807;
		fConst210 = 2.0 * (4.076781969643807 - fConst193);
		fConst211 = 2.0 * (0.00040767818495825777 - fConst207);
		fConst212 = (fConst188 + -0.7431304601070396) / fConst187 + 1.450071084655647;
		fConst213 = 2.0 * (1.450071084655647 - fConst193);
		fConst214 = 2.0 * (1.450071084655647 - fConst204);
		fConst215 = (fConst188 + -0.157482159302087) / fConst187 + 0.9351401670315425;
		fConst216 = 2.0 * (0.9351401670315425 - fConst193);
		fConst217 = 2.0 * (0.9351401670315425 - fConst201);
		fConst218 = (fConst188 + -0.782413046821645) / fConst187 + 0.24529150870616;
		fConst219 = 2.0 * (0.24529150870616 - fConst193);
		fConst220 = 2.0 * (0.000433227200555 - fConst198);
		fConst221 = (fConst188 + -0.512478641889141) / fConst187 + 0.689621364484675;
		fConst222 = 2.0 * (0.689621364484675 - fConst193);
		fConst223 = 2.0 * (7.621731298870603 - fConst193);
		fConst224 = (fConst188 + -0.168404871113589) / fConst187 + 1.069358407707312;
		fConst225 = 2.0 * (1.069358407707312 - fConst193);
		fConst226 = 2.0 * (53.53615295455673 - fConst193);
		fConst227 = std::tan(366.1241621387539 / fConst0);
		fConst228 = 1.0 / fConst227;
		fConst229 = (fConst228 + 0.168404871113589) / fConst227 + 1.069358407707312;
		fConst230 = 1.0 / fConst229;
		fConst231 = 3.0 / fConst229;
		fConst232 = piece_nono_quando_vocoder_faustpower2_f(fConst227);
		fConst233 = 1.0 / fConst232;
		fConst234 = fConst233 + 53.53615295455673;
		fConst235 = 1.0 / ((fConst228 + 0.512478641889141) / fConst227 + 0.689621364484675);
		fConst236 = fConst233 + 7.621731298870603;
		fConst237 = 1.0 / ((fConst228 + 0.782413046821645) / fConst227 + 0.24529150870616);
		fConst238 = 9.9999997055e-05 / fConst232;
		fConst239 = fConst238 + 0.000433227200555;
		fConst240 = 1.0 / ((fConst228 + 0.157482159302087) / fConst227 + 0.9351401670315425);
		fConst241 = 50.063807016150385 / fConst232;
		fConst242 = fConst241 + 0.9351401670315425;
		fConst243 = 1.0 / ((fConst228 + 0.7431304601070396) / fConst227 + 1.450071084655647);
		fConst244 = 11.052052171507189 / fConst232;
		fConst245 = fConst244 + 1.450071084655647;
		fConst246 = 1.0 / ((fConst228 + 3.1897274020965583) / fConst227 + 4.076781969643807);
		fConst247 = 0.0017661728399818856 / fConst232;
		fConst248 = fConst247 + 0.00040767818495825777;
		fConst249 = (fConst228 + -3.1897274020965583) / fConst227 + 4.076781969643807;
		fConst250 = 2.0 * (4.076781969643807 - fConst233);
		fConst251 = 2.0 * (0.00040767818495825777 - fConst247);
		fConst252 = (fConst228 + -0.7431304601070396) / fConst227 + 1.450071084655647;
		fConst253 = 2.0 * (1.450071084655647 - fConst233);
		fConst254 = 2.0 * (1.450071084655647 - fConst244);
		fConst255 = (fConst228 + -0.157482159302087) / fConst227 + 0.9351401670315425;
		fConst256 = 2.0 * (0.9351401670315425 - fConst233);
		fConst257 = 2.0 * (0.9351401670315425 - fConst241);
		fConst258 = (fConst228 + -0.782413046821645) / fConst227 + 0.24529150870616;
		fConst259 = 2.0 * (0.24529150870616 - fConst233);
		fConst260 = 2.0 * (0.000433227200555 - fConst238);
		fConst261 = (fConst228 + -0.512478641889141) / fConst227 + 0.689621364484675;
		fConst262 = 2.0 * (0.689621364484675 - fConst233);
		fConst263 = 2.0 * (7.621731298870603 - fConst233);
		fConst264 = (fConst228 + -0.168404871113589) / fConst227 + 1.069358407707312;
		fConst265 = 2.0 * (1.069358407707312 - fConst233);
		fConst266 = 2.0 * (53.53615295455673 - fConst233);
		fConst267 = std::tan(410.96047696981867 / fConst0);
		fConst268 = 1.0 / fConst267;
		fConst269 = (fConst268 + 0.168404871113589) / fConst267 + 1.069358407707312;
		fConst270 = 1.0 / fConst269;
		fConst271 = 3.0 / fConst269;
		fConst272 = piece_nono_quando_vocoder_faustpower2_f(fConst267);
		fConst273 = 1.0 / fConst272;
		fConst274 = fConst273 + 53.53615295455673;
		fConst275 = 1.0 / ((fConst268 + 0.512478641889141) / fConst267 + 0.689621364484675);
		fConst276 = fConst273 + 7.621731298870603;
		fConst277 = 1.0 / ((fConst268 + 0.782413046821645) / fConst267 + 0.24529150870616);
		fConst278 = 9.9999997055e-05 / fConst272;
		fConst279 = fConst278 + 0.000433227200555;
		fConst280 = 1.0 / ((fConst268 + 0.157482159302087) / fConst267 + 0.9351401670315425);
		fConst281 = 50.063807016150385 / fConst272;
		fConst282 = fConst281 + 0.9351401670315425;
		fConst283 = 1.0 / ((fConst268 + 0.7431304601070396) / fConst267 + 1.450071084655647);
		fConst284 = 11.052052171507189 / fConst272;
		fConst285 = fConst284 + 1.450071084655647;
		fConst286 = 1.0 / ((fConst268 + 3.1897274020965583) / fConst267 + 4.076781969643807);
		fConst287 = 0.0017661728399818856 / fConst272;
		fConst288 = fConst287 + 0.00040767818495825777;
		fConst289 = (fConst268 + -3.1897274020965583) / fConst267 + 4.076781969643807;
		fConst290 = 2.0 * (4.076781969643807 - fConst273);
		fConst291 = 2.0 * (0.00040767818495825777 - fConst287);
		fConst292 = (fConst268 + -0.7431304601070396) / fConst267 + 1.450071084655647;
		fConst293 = 2.0 * (1.450071084655647 - fConst273);
		fConst294 = 2.0 * (1.450071084655647 - fConst284);
		fConst295 = (fConst268 + -0.157482159302087) / fConst267 + 0.9351401670315425;
		fConst296 = 2.0 * (0.9351401670315425 - fConst273);
		fConst297 = 2.0 * (0.9351401670315425 - fConst281);
		fConst298 = (fConst268 + -0.782413046821645) / fConst267 + 0.24529150870616;
		fConst299 = 2.0 * (0.24529150870616 - fConst273);
		fConst300 = 2.0 * (0.000433227200555 - fConst278);
		fConst301 = (fConst268 + -0.512478641889141) / fConst267 + 0.689621364484675;
		fConst302 = 2.0 * (0.689621364484675 - fConst273);
		fConst303 = 2.0 * (7.621731298870603 - fConst273);
		fConst304 = (fConst268 + -0.168404871113589) / fConst267 + 1.069358407707312;
		fConst305 = 2.0 * (1.069358407707312 - fConst273);
		fConst306 = 2.0 * (53.53615295455673 - fConst273);
		fConst307 = std::tan(461.2875387537396 / fConst0);
		fConst308 = 1.0 / fConst307;
		fConst309 = (fConst308 + 0.168404871113589) / fConst307 + 1.069358407707312;
		fConst310 = 1.0 / fConst309;
		fConst311 = 3.0 / fConst309;
		fConst312 = piece_nono_quando_vocoder_faustpower2_f(fConst307);
		fConst313 = 1.0 / fConst312;
		fConst314 = fConst313 + 53.53615295455673;
		fConst315 = 1.0 / ((fConst308 + 0.512478641889141) / fConst307 + 0.689621364484675);
		fConst316 = fConst313 + 7.621731298870603;
		fConst317 = 1.0 / ((fConst308 + 0.782413046821645) / fConst307 + 0.24529150870616);
		fConst318 = 9.9999997055e-05 / fConst312;
		fConst319 = fConst318 + 0.000433227200555;
		fConst320 = 1.0 / ((fConst308 + 0.157482159302087) / fConst307 + 0.9351401670315425);
		fConst321 = 50.063807016150385 / fConst312;
		fConst322 = fConst321 + 0.9351401670315425;
		fConst323 = 1.0 / ((fConst308 + 0.7431304601070396) / fConst307 + 1.450071084655647);
		fConst324 = 11.052052171507189 / fConst312;
		fConst325 = fConst324 + 1.450071084655647;
		fConst326 = 1.0 / ((fConst308 + 3.1897274020965583) / fConst307 + 4.076781969643807);
		fConst327 = 0.0017661728399818856 / fConst312;
		fConst328 = fConst327 + 0.00040767818495825777;
		fConst329 = (fConst308 + -3.1897274020965583) / fConst307 + 4.076781969643807;
		fConst330 = 2.0 * (4.076781969643807 - fConst313);
		fConst331 = 2.0 * (0.00040767818495825777 - fConst327);
		fConst332 = (fConst308 + -0.7431304601070396) / fConst307 + 1.450071084655647;
		fConst333 = 2.0 * (1.450071084655647 - fConst313);
		fConst334 = 2.0 * (1.450071084655647 - fConst324);
		fConst335 = (fConst308 + -0.157482159302087) / fConst307 + 0.9351401670315425;
		fConst336 = 2.0 * (0.9351401670315425 - fConst313);
		fConst337 = 2.0 * (0.9351401670315425 - fConst321);
		fConst338 = (fConst308 + -0.782413046821645) / fConst307 + 0.24529150870616;
		fConst339 = 2.0 * (0.24529150870616 - fConst313);
		fConst340 = 2.0 * (0.000433227200555 - fConst318);
		fConst341 = (fConst308 + -0.512478641889141) / fConst307 + 0.689621364484675;
		fConst342 = 2.0 * (0.689621364484675 - fConst313);
		fConst343 = 2.0 * (7.621731298870603 - fConst313);
		fConst344 = (fConst308 + -0.168404871113589) / fConst307 + 1.069358407707312;
		fConst345 = 2.0 * (1.069358407707312 - fConst313);
		fConst346 = 2.0 * (53.53615295455673 - fConst313);
		fConst347 = std::tan(517.7777556091118 / fConst0);
		fConst348 = 1.0 / fConst347;
		fConst349 = (fConst348 + 0.168404871113589) / fConst347 + 1.069358407707312;
		fConst350 = 1.0 / fConst349;
		fConst351 = 3.0 / fConst349;
		fConst352 = piece_nono_quando_vocoder_faustpower2_f(fConst347);
		fConst353 = 1.0 / fConst352;
		fConst354 = fConst353 + 53.53615295455673;
		fConst355 = 1.0 / ((fConst348 + 0.512478641889141) / fConst347 + 0.689621364484675);
		fConst356 = fConst353 + 7.621731298870603;
		fConst357 = 1.0 / ((fConst348 + 0.782413046821645) / fConst347 + 0.24529150870616);
		fConst358 = 9.9999997055e-05 / fConst352;
		fConst359 = fConst358 + 0.000433227200555;
		fConst360 = 1.0 / ((fConst348 + 0.157482159302087) / fConst347 + 0.9351401670315425);
		fConst361 = 50.063807016150385 / fConst352;
		fConst362 = fConst361 + 0.9351401670315425;
		fConst363 = 1.0 / ((fConst348 + 0.7431304601070396) / fConst347 + 1.450071084655647);
		fConst364 = 11.052052171507189 / fConst352;
		fConst365 = fConst364 + 1.450071084655647;
		fConst366 = 1.0 / ((fConst348 + 3.1897274020965583) / fConst347 + 4.076781969643807);
		fConst367 = 0.0017661728399818856 / fConst352;
		fConst368 = fConst367 + 0.00040767818495825777;
		fConst369 = (fConst348 + -3.1897274020965583) / fConst347 + 4.076781969643807;
		fConst370 = 2.0 * (4.076781969643807 - fConst353);
		fConst371 = 2.0 * (0.00040767818495825777 - fConst367);
		fConst372 = (fConst348 + -0.7431304601070396) / fConst347 + 1.450071084655647;
		fConst373 = 2.0 * (1.450071084655647 - fConst353);
		fConst374 = 2.0 * (1.450071084655647 - fConst364);
		fConst375 = (fConst348 + -0.157482159302087) / fConst347 + 0.9351401670315425;
		fConst376 = 2.0 * (0.9351401670315425 - fConst353);
		fConst377 = 2.0 * (0.9351401670315425 - fConst361);
		fConst378 = (fConst348 + -0.782413046821645) / fConst347 + 0.24529150870616;
		fConst379 = 2.0 * (0.24529150870616 - fConst353);
		fConst380 = 2.0 * (0.000433227200555 - fConst358);
		fConst381 = (fConst348 + -0.512478641889141) / fConst347 + 0.689621364484675;
		fConst382 = 2.0 * (0.689621364484675 - fConst353);
		fConst383 = 2.0 * (7.621731298870603 - fConst353);
		fConst384 = (fConst348 + -0.168404871113589) / fConst347 + 1.069358407707312;
		fConst385 = 2.0 * (1.069358407707312 - fConst353);
		fConst386 = 2.0 * (53.53615295455673 - fConst353);
		fConst387 = std::tan(581.1858801300335 / fConst0);
		fConst388 = 1.0 / fConst387;
		fConst389 = (fConst388 + 0.168404871113589) / fConst387 + 1.069358407707312;
		fConst390 = 1.0 / fConst389;
		fConst391 = 3.0 / fConst389;
		fConst392 = piece_nono_quando_vocoder_faustpower2_f(fConst387);
		fConst393 = 1.0 / fConst392;
		fConst394 = fConst393 + 53.53615295455673;
		fConst395 = 1.0 / ((fConst388 + 0.512478641889141) / fConst387 + 0.689621364484675);
		fConst396 = fConst393 + 7.621731298870603;
		fConst397 = 1.0 / ((fConst388 + 0.782413046821645) / fConst387 + 0.24529150870616);
		fConst398 = 9.9999997055e-05 / fConst392;
		fConst399 = fConst398 + 0.000433227200555;
		fConst400 = 1.0 / ((fConst388 + 0.157482159302087) / fConst387 + 0.9351401670315425);
		fConst401 = 50.063807016150385 / fConst392;
		fConst402 = fConst401 + 0.9351401670315425;
		fConst403 = 1.0 / ((fConst388 + 0.7431304601070396) / fConst387 + 1.450071084655647);
		fConst404 = 11.052052171507189 / fConst392;
		fConst405 = fConst404 + 1.450071084655647;
		fConst406 = 1.0 / ((fConst388 + 3.1897274020965583) / fConst387 + 4.076781969643807);
		fConst407 = 0.0017661728399818856 / fConst392;
		fConst408 = fConst407 + 0.00040767818495825777;
		fConst409 = (fConst388 + -3.1897274020965583) / fConst387 + 4.076781969643807;
		fConst410 = 2.0 * (4.076781969643807 - fConst393);
		fConst411 = 2.0 * (0.00040767818495825777 - fConst407);
		fConst412 = (fConst388 + -0.7431304601070396) / fConst387 + 1.450071084655647;
		fConst413 = 2.0 * (1.450071084655647 - fConst393);
		fConst414 = 2.0 * (1.450071084655647 - fConst404);
		fConst415 = (fConst388 + -0.157482159302087) / fConst387 + 0.9351401670315425;
		fConst416 = 2.0 * (0.9351401670315425 - fConst393);
		fConst417 = 2.0 * (0.9351401670315425 - fConst401);
		fConst418 = (fConst388 + -0.782413046821645) / fConst387 + 0.24529150870616;
		fConst419 = 2.0 * (0.24529150870616 - fConst393);
		fConst420 = 2.0 * (0.000433227200555 - fConst398);
		fConst421 = (fConst388 + -0.512478641889141) / fConst387 + 0.689621364484675;
		fConst422 = 2.0 * (0.689621364484675 - fConst393);
		fConst423 = 2.0 * (7.621731298870603 - fConst393);
		fConst424 = (fConst388 + -0.168404871113589) / fConst387 + 1.069358407707312;
		fConst425 = 2.0 * (1.069358407707312 - fConst393);
		fConst426 = 2.0 * (53.53615295455673 - fConst393);
		fConst427 = std::tan(652.3590934592431 / fConst0);
		fConst428 = 1.0 / fConst427;
		fConst429 = (fConst428 + 0.168404871113589) / fConst427 + 1.069358407707312;
		fConst430 = 1.0 / fConst429;
		fConst431 = 3.0 / fConst429;
		fConst432 = piece_nono_quando_vocoder_faustpower2_f(fConst427);
		fConst433 = 1.0 / fConst432;
		fConst434 = fConst433 + 53.53615295455673;
		fConst435 = 1.0 / ((fConst428 + 0.512478641889141) / fConst427 + 0.689621364484675);
		fConst436 = fConst433 + 7.621731298870603;
		fConst437 = 1.0 / ((fConst428 + 0.782413046821645) / fConst427 + 0.24529150870616);
		fConst438 = 9.9999997055e-05 / fConst432;
		fConst439 = fConst438 + 0.000433227200555;
		fConst440 = 1.0 / ((fConst428 + 0.157482159302087) / fConst427 + 0.9351401670315425);
		fConst441 = 50.063807016150385 / fConst432;
		fConst442 = fConst441 + 0.9351401670315425;
		fConst443 = 1.0 / ((fConst428 + 0.7431304601070396) / fConst427 + 1.450071084655647);
		fConst444 = 11.052052171507189 / fConst432;
		fConst445 = fConst444 + 1.450071084655647;
		fConst446 = 1.0 / ((fConst428 + 3.1897274020965583) / fConst427 + 4.076781969643807);
		fConst447 = 0.0017661728399818856 / fConst432;
		fConst448 = fConst447 + 0.00040767818495825777;
		fConst449 = (fConst428 + -3.1897274020965583) / fConst427 + 4.076781969643807;
		fConst450 = 2.0 * (4.076781969643807 - fConst433);
		fConst451 = 2.0 * (0.00040767818495825777 - fConst447);
		fConst452 = (fConst428 + -0.7431304601070396) / fConst427 + 1.450071084655647;
		fConst453 = 2.0 * (1.450071084655647 - fConst433);
		fConst454 = 2.0 * (1.450071084655647 - fConst444);
		fConst455 = (fConst428 + -0.157482159302087) / fConst427 + 0.9351401670315425;
		fConst456 = 2.0 * (0.9351401670315425 - fConst433);
		fConst457 = 2.0 * (0.9351401670315425 - fConst441);
		fConst458 = (fConst428 + -0.782413046821645) / fConst427 + 0.24529150870616;
		fConst459 = 2.0 * (0.24529150870616 - fConst433);
		fConst460 = 2.0 * (0.000433227200555 - fConst438);
		fConst461 = (fConst428 + -0.512478641889141) / fConst427 + 0.689621364484675;
		fConst462 = 2.0 * (0.689621364484675 - fConst433);
		fConst463 = 2.0 * (7.621731298870603 - fConst433);
		fConst464 = (fConst428 + -0.168404871113589) / fConst427 + 1.069358407707312;
		fConst465 = 2.0 * (1.069358407707312 - fConst433);
		fConst466 = 2.0 * (53.53615295455673 - fConst433);
		fConst467 = std::tan(732.2483242775078 / fConst0);
		fConst468 = 1.0 / fConst467;
		fConst469 = (fConst468 + 0.168404871113589) / fConst467 + 1.069358407707312;
		fConst470 = 1.0 / fConst469;
		fConst471 = 3.0 / fConst469;
		fConst472 = piece_nono_quando_vocoder_faustpower2_f(fConst467);
		fConst473 = 1.0 / fConst472;
		fConst474 = fConst473 + 53.53615295455673;
		fConst475 = 1.0 / ((fConst468 + 0.512478641889141) / fConst467 + 0.689621364484675);
		fConst476 = fConst473 + 7.621731298870603;
		fConst477 = 1.0 / ((fConst468 + 0.782413046821645) / fConst467 + 0.24529150870616);
		fConst478 = 9.9999997055e-05 / fConst472;
		fConst479 = fConst478 + 0.000433227200555;
		fConst480 = 1.0 / ((fConst468 + 0.157482159302087) / fConst467 + 0.9351401670315425);
		fConst481 = 50.063807016150385 / fConst472;
		fConst482 = fConst481 + 0.9351401670315425;
		fConst483 = 1.0 / ((fConst468 + 0.7431304601070396) / fConst467 + 1.450071084655647);
		fConst484 = 11.052052171507189 / fConst472;
		fConst485 = fConst484 + 1.450071084655647;
		fConst486 = 1.0 / ((fConst468 + 3.1897274020965583) / fConst467 + 4.076781969643807);
		fConst487 = 0.0017661728399818856 / fConst472;
		fConst488 = fConst487 + 0.00040767818495825777;
		fConst489 = (fConst468 + -3.1897274020965583) / fConst467 + 4.076781969643807;
		fConst490 = 2.0 * (4.076781969643807 - fConst473);
		fConst491 = 2.0 * (0.00040767818495825777 - fConst487);
		fConst492 = (fConst468 + -0.7431304601070396) / fConst467 + 1.450071084655647;
		fConst493 = 2.0 * (1.450071084655647 - fConst473);
		fConst494 = 2.0 * (1.450071084655647 - fConst484);
		fConst495 = (fConst468 + -0.157482159302087) / fConst467 + 0.9351401670315425;
		fConst496 = 2.0 * (0.9351401670315425 - fConst473);
		fConst497 = 2.0 * (0.9351401670315425 - fConst481);
		fConst498 = (fConst468 + -0.782413046821645) / fConst467 + 0.24529150870616;
		fConst499 = 2.0 * (0.24529150870616 - fConst473);
		fConst500 = 2.0 * (0.000433227200555 - fConst478);
		fConst501 = (fConst468 + -0.512478641889141) / fConst467 + 0.689621364484675;
		fConst502 = 2.0 * (0.689621364484675 - fConst473);
		fConst503 = 2.0 * (7.621731298870603 - fConst473);
		fConst504 = (fConst468 + -0.168404871113589) / fConst467 + 1.069358407707312;
		fConst505 = 2.0 * (1.069358407707312 - fConst473);
		fConst506 = 2.0 * (53.53615295455673 - fConst473);
		fConst507 = std::tan(821.9209539396373 / fConst0);
		fConst508 = 1.0 / fConst507;
		fConst509 = (fConst508 + 0.168404871113589) / fConst507 + 1.069358407707312;
		fConst510 = 1.0 / fConst509;
		fConst511 = 3.0 / fConst509;
		fConst512 = piece_nono_quando_vocoder_faustpower2_f(fConst507);
		fConst513 = 1.0 / fConst512;
		fConst514 = fConst513 + 53.53615295455673;
		fConst515 = 1.0 / ((fConst508 + 0.512478641889141) / fConst507 + 0.689621364484675);
		fConst516 = fConst513 + 7.621731298870603;
		fConst517 = 1.0 / ((fConst508 + 0.782413046821645) / fConst507 + 0.24529150870616);
		fConst518 = 9.9999997055e-05 / fConst512;
		fConst519 = fConst518 + 0.000433227200555;
		fConst520 = 1.0 / ((fConst508 + 0.157482159302087) / fConst507 + 0.9351401670315425);
		fConst521 = 50.063807016150385 / fConst512;
		fConst522 = fConst521 + 0.9351401670315425;
		fConst523 = 1.0 / ((fConst508 + 0.7431304601070396) / fConst507 + 1.450071084655647);
		fConst524 = 11.052052171507189 / fConst512;
		fConst525 = fConst524 + 1.450071084655647;
		fConst526 = 1.0 / ((fConst508 + 3.1897274020965583) / fConst507 + 4.076781969643807);
		fConst527 = 0.0017661728399818856 / fConst512;
		fConst528 = fConst527 + 0.00040767818495825777;
		fConst529 = (fConst508 + -3.1897274020965583) / fConst507 + 4.076781969643807;
		fConst530 = 2.0 * (4.076781969643807 - fConst513);
		fConst531 = 2.0 * (0.00040767818495825777 - fConst527);
		fConst532 = (fConst508 + -0.7431304601070396) / fConst507 + 1.450071084655647;
		fConst533 = 2.0 * (1.450071084655647 - fConst513);
		fConst534 = 2.0 * (1.450071084655647 - fConst524);
		fConst535 = (fConst508 + -0.157482159302087) / fConst507 + 0.9351401670315425;
		fConst536 = 2.0 * (0.9351401670315425 - fConst513);
		fConst537 = 2.0 * (0.9351401670315425 - fConst521);
		fConst538 = (fConst508 + -0.782413046821645) / fConst507 + 0.24529150870616;
		fConst539 = 2.0 * (0.24529150870616 - fConst513);
		fConst540 = 2.0 * (0.000433227200555 - fConst518);
		fConst541 = (fConst508 + -0.512478641889141) / fConst507 + 0.689621364484675;
		fConst542 = 2.0 * (0.689621364484675 - fConst513);
		fConst543 = 2.0 * (7.621731298870603 - fConst513);
		fConst544 = (fConst508 + -0.168404871113589) / fConst507 + 1.069358407707312;
		fConst545 = 2.0 * (1.069358407707312 - fConst513);
		fConst546 = 2.0 * (53.53615295455673 - fConst513);
		fConst547 = std::tan(922.575077507479 / fConst0);
		fConst548 = 1.0 / fConst547;
		fConst549 = (fConst548 + 0.168404871113589) / fConst547 + 1.069358407707312;
		fConst550 = 1.0 / fConst549;
		fConst551 = 3.0 / fConst549;
		fConst552 = piece_nono_quando_vocoder_faustpower2_f(fConst547);
		fConst553 = 1.0 / fConst552;
		fConst554 = fConst553 + 53.53615295455673;
		fConst555 = 1.0 / ((fConst548 + 0.512478641889141) / fConst547 + 0.689621364484675);
		fConst556 = fConst553 + 7.621731298870603;
		fConst557 = 1.0 / ((fConst548 + 0.782413046821645) / fConst547 + 0.24529150870616);
		fConst558 = 9.9999997055e-05 / fConst552;
		fConst559 = fConst558 + 0.000433227200555;
		fConst560 = 1.0 / ((fConst548 + 0.157482159302087) / fConst547 + 0.9351401670315425);
		fConst561 = 50.063807016150385 / fConst552;
		fConst562 = fConst561 + 0.9351401670315425;
		fConst563 = 1.0 / ((fConst548 + 0.7431304601070396) / fConst547 + 1.450071084655647);
		fConst564 = 11.052052171507189 / fConst552;
		fConst565 = fConst564 + 1.450071084655647;
		fConst566 = 1.0 / ((fConst548 + 3.1897274020965583) / fConst547 + 4.076781969643807);
		fConst567 = 0.0017661728399818856 / fConst552;
		fConst568 = fConst567 + 0.00040767818495825777;
		fConst569 = (fConst548 + -3.1897274020965583) / fConst547 + 4.076781969643807;
		fConst570 = 2.0 * (4.076781969643807 - fConst553);
		fConst571 = 2.0 * (0.00040767818495825777 - fConst567);
		fConst572 = (fConst548 + -0.7431304601070396) / fConst547 + 1.450071084655647;
		fConst573 = 2.0 * (1.450071084655647 - fConst553);
		fConst574 = 2.0 * (1.450071084655647 - fConst564);
		fConst575 = (fConst548 + -0.157482159302087) / fConst547 + 0.9351401670315425;
		fConst576 = 2.0 * (0.9351401670315425 - fConst553);
		fConst577 = 2.0 * (0.9351401670315425 - fConst561);
		fConst578 = (fConst548 + -0.782413046821645) / fConst547 + 0.24529150870616;
		fConst579 = 2.0 * (0.24529150870616 - fConst553);
		fConst580 = 2.0 * (0.000433227200555 - fConst558);
		fConst581 = (fConst548 + -0.512478641889141) / fConst547 + 0.689621364484675;
		fConst582 = 2.0 * (0.689621364484675 - fConst553);
		fConst583 = 2.0 * (7.621731298870603 - fConst553);
		fConst584 = (fConst548 + -0.168404871113589) / fConst547 + 1.069358407707312;
		fConst585 = 2.0 * (1.069358407707312 - fConst553);
		fConst586 = 2.0 * (53.53615295455673 - fConst553);
		fConst587 = std::tan(1035.5555112182235 / fConst0);
		fConst588 = 1.0 / fConst587;
		fConst589 = (fConst588 + 0.168404871113589) / fConst587 + 1.069358407707312;
		fConst590 = 1.0 / fConst589;
		fConst591 = 3.0 / fConst589;
		fConst592 = piece_nono_quando_vocoder_faustpower2_f(fConst587);
		fConst593 = 1.0 / fConst592;
		fConst594 = fConst593 + 53.53615295455673;
		fConst595 = 1.0 / ((fConst588 + 0.512478641889141) / fConst587 + 0.689621364484675);
		fConst596 = fConst593 + 7.621731298870603;
		fConst597 = 1.0 / ((fConst588 + 0.782413046821645) / fConst587 + 0.24529150870616);
		fConst598 = 9.9999997055e-05 / fConst592;
		fConst599 = fConst598 + 0.000433227200555;
		fConst600 = 1.0 / ((fConst588 + 0.157482159302087) / fConst587 + 0.9351401670315425);
		fConst601 = 50.063807016150385 / fConst592;
		fConst602 = fConst601 + 0.9351401670315425;
		fConst603 = 1.0 / ((fConst588 + 0.7431304601070396) / fConst587 + 1.450071084655647);
		fConst604 = 11.052052171507189 / fConst592;
		fConst605 = fConst604 + 1.450071084655647;
		fConst606 = 1.0 / ((fConst588 + 3.1897274020965583) / fConst587 + 4.076781969643807);
		fConst607 = 0.0017661728399818856 / fConst592;
		fConst608 = fConst607 + 0.00040767818495825777;
		fConst609 = (fConst588 + -3.1897274020965583) / fConst587 + 4.076781969643807;
		fConst610 = 2.0 * (4.076781969643807 - fConst593);
		fConst611 = 2.0 * (0.00040767818495825777 - fConst607);
		fConst612 = (fConst588 + -0.7431304601070396) / fConst587 + 1.450071084655647;
		fConst613 = 2.0 * (1.450071084655647 - fConst593);
		fConst614 = 2.0 * (1.450071084655647 - fConst604);
		fConst615 = (fConst588 + -0.157482159302087) / fConst587 + 0.9351401670315425;
		fConst616 = 2.0 * (0.9351401670315425 - fConst593);
		fConst617 = 2.0 * (0.9351401670315425 - fConst601);
		fConst618 = (fConst588 + -0.782413046821645) / fConst587 + 0.24529150870616;
		fConst619 = 2.0 * (0.24529150870616 - fConst593);
		fConst620 = 2.0 * (0.000433227200555 - fConst598);
		fConst621 = (fConst588 + -0.512478641889141) / fConst587 + 0.689621364484675;
		fConst622 = 2.0 * (0.689621364484675 - fConst593);
		fConst623 = 2.0 * (7.621731298870603 - fConst593);
		fConst624 = (fConst588 + -0.168404871113589) / fConst587 + 1.069358407707312;
		fConst625 = 2.0 * (1.069358407707312 - fConst593);
		fConst626 = 2.0 * (53.53615295455673 - fConst593);
		fConst627 = std::tan(1162.371760260067 / fConst0);
		fConst628 = 1.0 / fConst627;
		fConst629 = (fConst628 + 0.168404871113589) / fConst627 + 1.069358407707312;
		fConst630 = 1.0 / fConst629;
		fConst631 = 3.0 / fConst629;
		fConst632 = piece_nono_quando_vocoder_faustpower2_f(fConst627);
		fConst633 = 1.0 / fConst632;
		fConst634 = fConst633 + 53.53615295455673;
		fConst635 = 1.0 / ((fConst628 + 0.512478641889141) / fConst627 + 0.689621364484675);
		fConst636 = fConst633 + 7.621731298870603;
		fConst637 = 1.0 / ((fConst628 + 0.782413046821645) / fConst627 + 0.24529150870616);
		fConst638 = 9.9999997055e-05 / fConst632;
		fConst639 = fConst638 + 0.000433227200555;
		fConst640 = 1.0 / ((fConst628 + 0.157482159302087) / fConst627 + 0.9351401670315425);
		fConst641 = 50.063807016150385 / fConst632;
		fConst642 = fConst641 + 0.9351401670315425;
		fConst643 = 1.0 / ((fConst628 + 0.7431304601070396) / fConst627 + 1.450071084655647);
		fConst644 = 11.052052171507189 / fConst632;
		fConst645 = fConst644 + 1.450071084655647;
		fConst646 = 1.0 / ((fConst628 + 3.1897274020965583) / fConst627 + 4.076781969643807);
		fConst647 = 0.0017661728399818856 / fConst632;
		fConst648 = fConst647 + 0.00040767818495825777;
		fConst649 = (fConst628 + -3.1897274020965583) / fConst627 + 4.076781969643807;
		fConst650 = 2.0 * (4.076781969643807 - fConst633);
		fConst651 = 2.0 * (0.00040767818495825777 - fConst647);
		fConst652 = (fConst628 + -0.7431304601070396) / fConst627 + 1.450071084655647;
		fConst653 = 2.0 * (1.450071084655647 - fConst633);
		fConst654 = 2.0 * (1.450071084655647 - fConst644);
		fConst655 = (fConst628 + -0.157482159302087) / fConst627 + 0.9351401670315425;
		fConst656 = 2.0 * (0.9351401670315425 - fConst633);
		fConst657 = 2.0 * (0.9351401670315425 - fConst641);
		fConst658 = (fConst628 + -0.782413046821645) / fConst627 + 0.24529150870616;
		fConst659 = 2.0 * (0.24529150870616 - fConst633);
		fConst660 = 2.0 * (0.000433227200555 - fConst638);
		fConst661 = (fConst628 + -0.512478641889141) / fConst627 + 0.689621364484675;
		fConst662 = 2.0 * (0.689621364484675 - fConst633);
		fConst663 = 2.0 * (7.621731298870603 - fConst633);
		fConst664 = (fConst628 + -0.168404871113589) / fConst627 + 1.069358407707312;
		fConst665 = 2.0 * (1.069358407707312 - fConst633);
		fConst666 = 2.0 * (53.53615295455673 - fConst633);
		fConst667 = std::tan(1304.7181869184863 / fConst0);
		fConst668 = 1.0 / fConst667;
		fConst669 = (fConst668 + 0.168404871113589) / fConst667 + 1.069358407707312;
		fConst670 = 1.0 / fConst669;
		fConst671 = 3.0 / fConst669;
		fConst672 = piece_nono_quando_vocoder_faustpower2_f(fConst667);
		fConst673 = 1.0 / fConst672;
		fConst674 = fConst673 + 53.53615295455673;
		fConst675 = 1.0 / ((fConst668 + 0.512478641889141) / fConst667 + 0.689621364484675);
		fConst676 = fConst673 + 7.621731298870603;
		fConst677 = 1.0 / ((fConst668 + 0.782413046821645) / fConst667 + 0.24529150870616);
		fConst678 = 9.9999997055e-05 / fConst672;
		fConst679 = fConst678 + 0.000433227200555;
		fConst680 = 1.0 / ((fConst668 + 0.157482159302087) / fConst667 + 0.9351401670315425);
		fConst681 = 50.063807016150385 / fConst672;
		fConst682 = fConst681 + 0.9351401670315425;
		fConst683 = 1.0 / ((fConst668 + 0.7431304601070396) / fConst667 + 1.450071084655647);
		fConst684 = 11.052052171507189 / fConst672;
		fConst685 = fConst684 + 1.450071084655647;
		fConst686 = 1.0 / ((fConst668 + 3.1897274020965583) / fConst667 + 4.076781969643807);
		fConst687 = 0.0017661728399818856 / fConst672;
		fConst688 = fConst687 + 0.00040767818495825777;
		fConst689 = (fConst668 + -3.1897274020965583) / fConst667 + 4.076781969643807;
		fConst690 = 2.0 * (4.076781969643807 - fConst673);
		fConst691 = 2.0 * (0.00040767818495825777 - fConst687);
		fConst692 = (fConst668 + -0.7431304601070396) / fConst667 + 1.450071084655647;
		fConst693 = 2.0 * (1.450071084655647 - fConst673);
		fConst694 = 2.0 * (1.450071084655647 - fConst684);
		fConst695 = (fConst668 + -0.157482159302087) / fConst667 + 0.9351401670315425;
		fConst696 = 2.0 * (0.9351401670315425 - fConst673);
		fConst697 = 2.0 * (0.9351401670315425 - fConst681);
		fConst698 = (fConst668 + -0.782413046821645) / fConst667 + 0.24529150870616;
		fConst699 = 2.0 * (0.24529150870616 - fConst673);
		fConst700 = 2.0 * (0.000433227200555 - fConst678);
		fConst701 = (fConst668 + -0.512478641889141) / fConst667 + 0.689621364484675;
		fConst702 = 2.0 * (0.689621364484675 - fConst673);
		fConst703 = 2.0 * (7.621731298870603 - fConst673);
		fConst704 = (fConst668 + -0.168404871113589) / fConst667 + 1.069358407707312;
		fConst705 = 2.0 * (1.069358407707312 - fConst673);
		fConst706 = 2.0 * (53.53615295455673 - fConst673);
		fConst707 = std::tan(1464.4966485550156 / fConst0);
		fConst708 = 1.0 / fConst707;
		fConst709 = (fConst708 + 0.168404871113589) / fConst707 + 1.069358407707312;
		fConst710 = 1.0 / fConst709;
		fConst711 = 3.0 / fConst709;
		fConst712 = piece_nono_quando_vocoder_faustpower2_f(fConst707);
		fConst713 = 1.0 / fConst712;
		fConst714 = fConst713 + 53.53615295455673;
		fConst715 = 1.0 / ((fConst708 + 0.512478641889141) / fConst707 + 0.689621364484675);
		fConst716 = fConst713 + 7.621731298870603;
		fConst717 = 1.0 / ((fConst708 + 0.782413046821645) / fConst707 + 0.24529150870616);
		fConst718 = 9.9999997055e-05 / fConst712;
		fConst719 = fConst718 + 0.000433227200555;
		fConst720 = 1.0 / ((fConst708 + 0.157482159302087) / fConst707 + 0.9351401670315425);
		fConst721 = 50.063807016150385 / fConst712;
		fConst722 = fConst721 + 0.9351401670315425;
		fConst723 = 1.0 / ((fConst708 + 0.7431304601070396) / fConst707 + 1.450071084655647);
		fConst724 = 11.052052171507189 / fConst712;
		fConst725 = fConst724 + 1.450071084655647;
		fConst726 = 1.0 / ((fConst708 + 3.1897274020965583) / fConst707 + 4.076781969643807);
		fConst727 = 0.0017661728399818856 / fConst712;
		fConst728 = fConst727 + 0.00040767818495825777;
		fConst729 = (fConst708 + -3.1897274020965583) / fConst707 + 4.076781969643807;
		fConst730 = 2.0 * (4.076781969643807 - fConst713);
		fConst731 = 2.0 * (0.00040767818495825777 - fConst727);
		fConst732 = (fConst708 + -0.7431304601070396) / fConst707 + 1.450071084655647;
		fConst733 = 2.0 * (1.450071084655647 - fConst713);
		fConst734 = 2.0 * (1.450071084655647 - fConst724);
		fConst735 = (fConst708 + -0.157482159302087) / fConst707 + 0.9351401670315425;
		fConst736 = 2.0 * (0.9351401670315425 - fConst713);
		fConst737 = 2.0 * (0.9351401670315425 - fConst721);
		fConst738 = (fConst708 + -0.782413046821645) / fConst707 + 0.24529150870616;
		fConst739 = 2.0 * (0.24529150870616 - fConst713);
		fConst740 = 2.0 * (0.000433227200555 - fConst718);
		fConst741 = (fConst708 + -0.512478641889141) / fConst707 + 0.689621364484675;
		fConst742 = 2.0 * (0.689621364484675 - fConst713);
		fConst743 = 2.0 * (7.621731298870603 - fConst713);
		fConst744 = (fConst708 + -0.168404871113589) / fConst707 + 1.069358407707312;
		fConst745 = 2.0 * (1.069358407707312 - fConst713);
		fConst746 = 2.0 * (53.53615295455673 - fConst713);
		fConst747 = std::tan(1643.8419078792747 / fConst0);
		fConst748 = 1.0 / fConst747;
		fConst749 = (fConst748 + 0.168404871113589) / fConst747 + 1.069358407707312;
		fConst750 = 1.0 / fConst749;
		fConst751 = 3.0 / fConst749;
		fConst752 = piece_nono_quando_vocoder_faustpower2_f(fConst747);
		fConst753 = 1.0 / fConst752;
		fConst754 = fConst753 + 53.53615295455673;
		fConst755 = 1.0 / ((fConst748 + 0.512478641889141) / fConst747 + 0.689621364484675);
		fConst756 = fConst753 + 7.621731298870603;
		fConst757 = 1.0 / ((fConst748 + 0.782413046821645) / fConst747 + 0.24529150870616);
		fConst758 = 9.9999997055e-05 / fConst752;
		fConst759 = fConst758 + 0.000433227200555;
		fConst760 = 1.0 / ((fConst748 + 0.157482159302087) / fConst747 + 0.9351401670315425);
		fConst761 = 50.063807016150385 / fConst752;
		fConst762 = fConst761 + 0.9351401670315425;
		fConst763 = 1.0 / ((fConst748 + 0.7431304601070396) / fConst747 + 1.450071084655647);
		fConst764 = 11.052052171507189 / fConst752;
		fConst765 = fConst764 + 1.450071084655647;
		fConst766 = 1.0 / ((fConst748 + 3.1897274020965583) / fConst747 + 4.076781969643807);
		fConst767 = 0.0017661728399818856 / fConst752;
		fConst768 = fConst767 + 0.00040767818495825777;
		fConst769 = (fConst748 + -3.1897274020965583) / fConst747 + 4.076781969643807;
		fConst770 = 2.0 * (4.076781969643807 - fConst753);
		fConst771 = 2.0 * (0.00040767818495825777 - fConst767);
		fConst772 = (fConst748 + -0.7431304601070396) / fConst747 + 1.450071084655647;
		fConst773 = 2.0 * (1.450071084655647 - fConst753);
		fConst774 = 2.0 * (1.450071084655647 - fConst764);
		fConst775 = (fConst748 + -0.157482159302087) / fConst747 + 0.9351401670315425;
		fConst776 = 2.0 * (0.9351401670315425 - fConst753);
		fConst777 = 2.0 * (0.9351401670315425 - fConst761);
		fConst778 = (fConst748 + -0.782413046821645) / fConst747 + 0.24529150870616;
		fConst779 = 2.0 * (0.24529150870616 - fConst753);
		fConst780 = 2.0 * (0.000433227200555 - fConst758);
		fConst781 = (fConst748 + -0.512478641889141) / fConst747 + 0.689621364484675;
		fConst782 = 2.0 * (0.689621364484675 - fConst753);
		fConst783 = 2.0 * (7.621731298870603 - fConst753);
		fConst784 = (fConst748 + -0.168404871113589) / fConst747 + 1.069358407707312;
		fConst785 = 2.0 * (1.069358407707312 - fConst753);
		fConst786 = 2.0 * (53.53615295455673 - fConst753);
		fConst787 = std::tan(1845.150155014958 / fConst0);
		fConst788 = 1.0 / fConst787;
		fConst789 = (fConst788 + 0.168404871113589) / fConst787 + 1.069358407707312;
		fConst790 = 1.0 / fConst789;
		fConst791 = 3.0 / fConst789;
		fConst792 = piece_nono_quando_vocoder_faustpower2_f(fConst787);
		fConst793 = 1.0 / fConst792;
		fConst794 = fConst793 + 53.53615295455673;
		fConst795 = 1.0 / ((fConst788 + 0.512478641889141) / fConst787 + 0.689621364484675);
		fConst796 = fConst793 + 7.621731298870603;
		fConst797 = 1.0 / ((fConst788 + 0.782413046821645) / fConst787 + 0.24529150870616);
		fConst798 = 9.9999997055e-05 / fConst792;
		fConst799 = fConst798 + 0.000433227200555;
		fConst800 = 1.0 / ((fConst788 + 0.157482159302087) / fConst787 + 0.9351401670315425);
		fConst801 = 50.063807016150385 / fConst792;
		fConst802 = fConst801 + 0.9351401670315425;
		fConst803 = 1.0 / ((fConst788 + 0.7431304601070396) / fConst787 + 1.450071084655647);
		fConst804 = 11.052052171507189 / fConst792;
		fConst805 = fConst804 + 1.450071084655647;
		fConst806 = 1.0 / ((fConst788 + 3.1897274020965583) / fConst787 + 4.076781969643807);
		fConst807 = 0.0017661728399818856 / fConst792;
		fConst808 = fConst807 + 0.00040767818495825777;
		fConst809 = (fConst788 + -3.1897274020965583) / fConst787 + 4.076781969643807;
		fConst810 = 2.0 * (4.076781969643807 - fConst793);
		fConst811 = 2.0 * (0.00040767818495825777 - fConst807);
		fConst812 = (fConst788 + -0.7431304601070396) / fConst787 + 1.450071084655647;
		fConst813 = 2.0 * (1.450071084655647 - fConst793);
		fConst814 = 2.0 * (1.450071084655647 - fConst804);
		fConst815 = (fConst788 + -0.157482159302087) / fConst787 + 0.9351401670315425;
		fConst816 = 2.0 * (0.9351401670315425 - fConst793);
		fConst817 = 2.0 * (0.9351401670315425 - fConst801);
		fConst818 = (fConst788 + -0.782413046821645) / fConst787 + 0.24529150870616;
		fConst819 = 2.0 * (0.24529150870616 - fConst793);
		fConst820 = 2.0 * (0.000433227200555 - fConst798);
		fConst821 = (fConst788 + -0.512478641889141) / fConst787 + 0.689621364484675;
		fConst822 = 2.0 * (0.689621364484675 - fConst793);
		fConst823 = 2.0 * (7.621731298870603 - fConst793);
		fConst824 = (fConst788 + -0.168404871113589) / fConst787 + 1.069358407707312;
		fConst825 = 2.0 * (1.069358407707312 - fConst793);
		fConst826 = 2.0 * (53.53615295455673 - fConst793);
		fConst827 = std::tan(2071.111022436447 / fConst0);
		fConst828 = 1.0 / fConst827;
		fConst829 = (fConst828 + 0.168404871113589) / fConst827 + 1.069358407707312;
		fConst830 = 1.0 / fConst829;
		fConst831 = 3.0 / fConst829;
		fConst832 = piece_nono_quando_vocoder_faustpower2_f(fConst827);
		fConst833 = 1.0 / fConst832;
		fConst834 = fConst833 + 53.53615295455673;
		fConst835 = 1.0 / ((fConst828 + 0.512478641889141) / fConst827 + 0.689621364484675);
		fConst836 = fConst833 + 7.621731298870603;
		fConst837 = 1.0 / ((fConst828 + 0.782413046821645) / fConst827 + 0.24529150870616);
		fConst838 = 9.9999997055e-05 / fConst832;
		fConst839 = fConst838 + 0.000433227200555;
		fConst840 = 1.0 / ((fConst828 + 0.157482159302087) / fConst827 + 0.9351401670315425);
		fConst841 = 50.063807016150385 / fConst832;
		fConst842 = fConst841 + 0.9351401670315425;
		fConst843 = 1.0 / ((fConst828 + 0.7431304601070396) / fConst827 + 1.450071084655647);
		fConst844 = 11.052052171507189 / fConst832;
		fConst845 = fConst844 + 1.450071084655647;
		fConst846 = 1.0 / ((fConst828 + 3.1897274020965583) / fConst827 + 4.076781969643807);
		fConst847 = 0.0017661728399818856 / fConst832;
		fConst848 = fConst847 + 0.00040767818495825777;
		fConst849 = (fConst828 + -3.1897274020965583) / fConst827 + 4.076781969643807;
		fConst850 = 2.0 * (4.076781969643807 - fConst833);
		fConst851 = 2.0 * (0.00040767818495825777 - fConst847);
		fConst852 = (fConst828 + -0.7431304601070396) / fConst827 + 1.450071084655647;
		fConst853 = 2.0 * (1.450071084655647 - fConst833);
		fConst854 = 2.0 * (1.450071084655647 - fConst844);
		fConst855 = (fConst828 + -0.157482159302087) / fConst827 + 0.9351401670315425;
		fConst856 = 2.0 * (0.9351401670315425 - fConst833);
		fConst857 = 2.0 * (0.9351401670315425 - fConst841);
		fConst858 = (fConst828 + -0.782413046821645) / fConst827 + 0.24529150870616;
		fConst859 = 2.0 * (0.24529150870616 - fConst833);
		fConst860 = 2.0 * (0.000433227200555 - fConst838);
		fConst861 = (fConst828 + -0.512478641889141) / fConst827 + 0.689621364484675;
		fConst862 = 2.0 * (0.689621364484675 - fConst833);
		fConst863 = 2.0 * (7.621731298870603 - fConst833);
		fConst864 = (fConst828 + -0.168404871113589) / fConst827 + 1.069358407707312;
		fConst865 = 2.0 * (1.069358407707312 - fConst833);
		fConst866 = 2.0 * (53.53615295455673 - fConst833);
		fConst867 = std::tan(2324.743520520134 / fConst0);
		fConst868 = 1.0 / fConst867;
		fConst869 = (fConst868 + 0.168404871113589) / fConst867 + 1.069358407707312;
		fConst870 = 1.0 / fConst869;
		fConst871 = 3.0 / fConst869;
		fConst872 = piece_nono_quando_vocoder_faustpower2_f(fConst867);
		fConst873 = 1.0 / fConst872;
		fConst874 = fConst873 + 53.53615295455673;
		fConst875 = 1.0 / ((fConst868 + 0.512478641889141) / fConst867 + 0.689621364484675);
		fConst876 = fConst873 + 7.621731298870603;
		fConst877 = 1.0 / ((fConst868 + 0.782413046821645) / fConst867 + 0.24529150870616);
		fConst878 = 9.9999997055e-05 / fConst872;
		fConst879 = fConst878 + 0.000433227200555;
		fConst880 = 1.0 / ((fConst868 + 0.157482159302087) / fConst867 + 0.9351401670315425);
		fConst881 = 50.063807016150385 / fConst872;
		fConst882 = fConst881 + 0.9351401670315425;
		fConst883 = 1.0 / ((fConst868 + 0.7431304601070396) / fConst867 + 1.450071084655647);
		fConst884 = 11.052052171507189 / fConst872;
		fConst885 = fConst884 + 1.450071084655647;
		fConst886 = 1.0 / ((fConst868 + 3.1897274020965583) / fConst867 + 4.076781969643807);
		fConst887 = 0.0017661728399818856 / fConst872;
		fConst888 = fConst887 + 0.00040767818495825777;
		fConst889 = (fConst868 + -3.1897274020965583) / fConst867 + 4.076781969643807;
		fConst890 = 2.0 * (4.076781969643807 - fConst873);
		fConst891 = 2.0 * (0.00040767818495825777 - fConst887);
		fConst892 = (fConst868 + -0.7431304601070396) / fConst867 + 1.450071084655647;
		fConst893 = 2.0 * (1.450071084655647 - fConst873);
		fConst894 = 2.0 * (1.450071084655647 - fConst884);
		fConst895 = (fConst868 + -0.157482159302087) / fConst867 + 0.9351401670315425;
		fConst896 = 2.0 * (0.9351401670315425 - fConst873);
		fConst897 = 2.0 * (0.9351401670315425 - fConst881);
		fConst898 = (fConst868 + -0.782413046821645) / fConst867 + 0.24529150870616;
		fConst899 = 2.0 * (0.24529150870616 - fConst873);
		fConst900 = 2.0 * (0.000433227200555 - fConst878);
		fConst901 = (fConst868 + -0.512478641889141) / fConst867 + 0.689621364484675;
		fConst902 = 2.0 * (0.689621364484675 - fConst873);
		fConst903 = 2.0 * (7.621731298870603 - fConst873);
		fConst904 = (fConst868 + -0.168404871113589) / fConst867 + 1.069358407707312;
		fConst905 = 2.0 * (1.069358407707312 - fConst873);
		fConst906 = 2.0 * (53.53615295455673 - fConst873);
		fConst907 = std::tan(2609.4363738369725 / fConst0);
		fConst908 = 1.0 / fConst907;
		fConst909 = (fConst908 + 0.168404871113589) / fConst907 + 1.069358407707312;
		fConst910 = 1.0 / fConst909;
		fConst911 = 3.0 / fConst909;
		fConst912 = piece_nono_quando_vocoder_faustpower2_f(fConst907);
		fConst913 = 1.0 / fConst912;
		fConst914 = fConst913 + 53.53615295455673;
		fConst915 = 1.0 / ((fConst908 + 0.512478641889141) / fConst907 + 0.689621364484675);
		fConst916 = fConst913 + 7.621731298870603;
		fConst917 = 1.0 / ((fConst908 + 0.782413046821645) / fConst907 + 0.24529150870616);
		fConst918 = 9.9999997055e-05 / fConst912;
		fConst919 = fConst918 + 0.000433227200555;
		fConst920 = 1.0 / ((fConst908 + 0.157482159302087) / fConst907 + 0.9351401670315425);
		fConst921 = 50.063807016150385 / fConst912;
		fConst922 = fConst921 + 0.9351401670315425;
		fConst923 = 1.0 / ((fConst908 + 0.7431304601070396) / fConst907 + 1.450071084655647);
		fConst924 = 11.052052171507189 / fConst912;
		fConst925 = fConst924 + 1.450071084655647;
		fConst926 = 1.0 / ((fConst908 + 3.1897274020965583) / fConst907 + 4.076781969643807);
		fConst927 = 0.0017661728399818856 / fConst912;
		fConst928 = fConst927 + 0.00040767818495825777;
		fConst929 = (fConst908 + -3.1897274020965583) / fConst907 + 4.076781969643807;
		fConst930 = 2.0 * (4.076781969643807 - fConst913);
		fConst931 = 2.0 * (0.00040767818495825777 - fConst927);
		fConst932 = (fConst908 + -0.7431304601070396) / fConst907 + 1.450071084655647;
		fConst933 = 2.0 * (1.450071084655647 - fConst913);
		fConst934 = 2.0 * (1.450071084655647 - fConst924);
		fConst935 = (fConst908 + -0.157482159302087) / fConst907 + 0.9351401670315425;
		fConst936 = 2.0 * (0.9351401670315425 - fConst913);
		fConst937 = 2.0 * (0.9351401670315425 - fConst921);
		fConst938 = (fConst908 + -0.782413046821645) / fConst907 + 0.24529150870616;
		fConst939 = 2.0 * (0.24529150870616 - fConst913);
		fConst940 = 2.0 * (0.000433227200555 - fConst918);
		fConst941 = (fConst908 + -0.512478641889141) / fConst907 + 0.689621364484675;
		fConst942 = 2.0 * (0.689621364484675 - fConst913);
		fConst943 = 2.0 * (7.621731298870603 - fConst913);
		fConst944 = (fConst908 + -0.168404871113589) / fConst907 + 1.069358407707312;
		fConst945 = 2.0 * (1.069358407707312 - fConst913);
		fConst946 = 2.0 * (53.53615295455673 - fConst913);
		fConst947 = std::tan(2928.993297110031 / fConst0);
		fConst948 = 1.0 / fConst947;
		fConst949 = (fConst948 + 0.168404871113589) / fConst947 + 1.069358407707312;
		fConst950 = 1.0 / fConst949;
		fConst951 = 3.0 / fConst949;
		fConst952 = piece_nono_quando_vocoder_faustpower2_f(fConst947);
		fConst953 = 1.0 / fConst952;
		fConst954 = fConst953 + 53.53615295455673;
		fConst955 = 1.0 / ((fConst948 + 0.512478641889141) / fConst947 + 0.689621364484675);
		fConst956 = fConst953 + 7.621731298870603;
		fConst957 = 1.0 / ((fConst948 + 0.782413046821645) / fConst947 + 0.24529150870616);
		fConst958 = 9.9999997055e-05 / fConst952;
		fConst959 = fConst958 + 0.000433227200555;
		fConst960 = 1.0 / ((fConst948 + 0.157482159302087) / fConst947 + 0.9351401670315425);
		fConst961 = 50.063807016150385 / fConst952;
		fConst962 = fConst961 + 0.9351401670315425;
		fConst963 = 1.0 / ((fConst948 + 0.7431304601070396) / fConst947 + 1.450071084655647);
		fConst964 = 11.052052171507189 / fConst952;
		fConst965 = fConst964 + 1.450071084655647;
		fConst966 = 1.0 / ((fConst948 + 3.1897274020965583) / fConst947 + 4.076781969643807);
		fConst967 = 0.0017661728399818856 / fConst952;
		fConst968 = fConst967 + 0.00040767818495825777;
		fConst969 = (fConst948 + -3.1897274020965583) / fConst947 + 4.076781969643807;
		fConst970 = 2.0 * (4.076781969643807 - fConst953);
		fConst971 = 2.0 * (0.00040767818495825777 - fConst967);
		fConst972 = (fConst948 + -0.7431304601070396) / fConst947 + 1.450071084655647;
		fConst973 = 2.0 * (1.450071084655647 - fConst953);
		fConst974 = 2.0 * (1.450071084655647 - fConst964);
		fConst975 = (fConst948 + -0.157482159302087) / fConst947 + 0.9351401670315425;
		fConst976 = 2.0 * (0.9351401670315425 - fConst953);
		fConst977 = 2.0 * (0.9351401670315425 - fConst961);
		fConst978 = (fConst948 + -0.782413046821645) / fConst947 + 0.24529150870616;
		fConst979 = 2.0 * (0.24529150870616 - fConst953);
		fConst980 = 2.0 * (0.000433227200555 - fConst958);
		fConst981 = (fConst948 + -0.512478641889141) / fConst947 + 0.689621364484675;
		fConst982 = 2.0 * (0.689621364484675 - fConst953);
		fConst983 = 2.0 * (7.621731298870603 - fConst953);
		fConst984 = (fConst948 + -0.168404871113589) / fConst947 + 1.069358407707312;
		fConst985 = 2.0 * (1.069358407707312 - fConst953);
		fConst986 = 2.0 * (53.53615295455673 - fConst953);
		fConst987 = std::tan(3287.6838157585494 / fConst0);
		fConst988 = 1.0 / fConst987;
		fConst989 = (fConst988 + 0.168404871113589) / fConst987 + 1.069358407707312;
		fConst990 = 1.0 / fConst989;
		fConst991 = 3.0 / fConst989;
		fConst992 = piece_nono_quando_vocoder_faustpower2_f(fConst987);
		fConst993 = 1.0 / fConst992;
		fConst994 = fConst993 + 53.53615295455673;
		fConst995 = 1.0 / ((fConst988 + 0.512478641889141) / fConst987 + 0.689621364484675);
		fConst996 = fConst993 + 7.621731298870603;
		fConst997 = 1.0 / ((fConst988 + 0.782413046821645) / fConst987 + 0.24529150870616);
		fConst998 = 9.9999997055e-05 / fConst992;
		fConst999 = fConst998 + 0.000433227200555;
		fConst1000 = 1.0 / ((fConst988 + 0.157482159302087) / fConst987 + 0.9351401670315425);
		fConst1001 = 50.063807016150385 / fConst992;
		fConst1002 = fConst1001 + 0.9351401670315425;
		fConst1003 = 1.0 / ((fConst988 + 0.7431304601070396) / fConst987 + 1.450071084655647);
		fConst1004 = 11.052052171507189 / fConst992;
		fConst1005 = fConst1004 + 1.450071084655647;
		fConst1006 = 1.0 / ((fConst988 + 3.1897274020965583) / fConst987 + 4.076781969643807);
		fConst1007 = 0.0017661728399818856 / fConst992;
		fConst1008 = fConst1007 + 0.00040767818495825777;
		fConst1009 = (fConst988 + -3.1897274020965583) / fConst987 + 4.076781969643807;
		fConst1010 = 2.0 * (4.076781969643807 - fConst993);
		fConst1011 = 2.0 * (0.00040767818495825777 - fConst1007);
		fConst1012 = (fConst988 + -0.7431304601070396) / fConst987 + 1.450071084655647;
		fConst1013 = 2.0 * (1.450071084655647 - fConst993);
		fConst1014 = 2.0 * (1.450071084655647 - fConst1004);
		fConst1015 = (fConst988 + -0.157482159302087) / fConst987 + 0.9351401670315425;
		fConst1016 = 2.0 * (0.9351401670315425 - fConst993);
		fConst1017 = 2.0 * (0.9351401670315425 - fConst1001);
		fConst1018 = (fConst988 + -0.782413046821645) / fConst987 + 0.24529150870616;
		fConst1019 = 2.0 * (0.24529150870616 - fConst993);
		fConst1020 = 2.0 * (0.000433227200555 - fConst998);
		fConst1021 = (fConst988 + -0.512478641889141) / fConst987 + 0.689621364484675;
		fConst1022 = 2.0 * (0.689621364484675 - fConst993);
		fConst1023 = 2.0 * (7.621731298870603 - fConst993);
		fConst1024 = (fConst988 + -0.168404871113589) / fConst987 + 1.069358407707312;
		fConst1025 = 2.0 * (1.069358407707312 - fConst993);
		fConst1026 = 2.0 * (53.53615295455673 - fConst993);
		fConst1027 = std::tan(3690.3003100299175 / fConst0);
		fConst1028 = 1.0 / fConst1027;
		fConst1029 = (fConst1028 + 0.168404871113589) / fConst1027 + 1.069358407707312;
		fConst1030 = 1.0 / fConst1029;
		fConst1031 = 3.0 / fConst1029;
		fConst1032 = piece_nono_quando_vocoder_faustpower2_f(fConst1027);
		fConst1033 = 1.0 / fConst1032;
		fConst1034 = fConst1033 + 53.53615295455673;
		fConst1035 = 1.0 / ((fConst1028 + 0.512478641889141) / fConst1027 + 0.689621364484675);
		fConst1036 = fConst1033 + 7.621731298870603;
		fConst1037 = 1.0 / ((fConst1028 + 0.782413046821645) / fConst1027 + 0.24529150870616);
		fConst1038 = 9.9999997055e-05 / fConst1032;
		fConst1039 = fConst1038 + 0.000433227200555;
		fConst1040 = 1.0 / ((fConst1028 + 0.157482159302087) / fConst1027 + 0.9351401670315425);
		fConst1041 = 50.063807016150385 / fConst1032;
		fConst1042 = fConst1041 + 0.9351401670315425;
		fConst1043 = 1.0 / ((fConst1028 + 0.7431304601070396) / fConst1027 + 1.450071084655647);
		fConst1044 = 11.052052171507189 / fConst1032;
		fConst1045 = fConst1044 + 1.450071084655647;
		fConst1046 = 1.0 / ((fConst1028 + 3.1897274020965583) / fConst1027 + 4.076781969643807);
		fConst1047 = 0.0017661728399818856 / fConst1032;
		fConst1048 = fConst1047 + 0.00040767818495825777;
		fConst1049 = (fConst1028 + -3.1897274020965583) / fConst1027 + 4.076781969643807;
		fConst1050 = 2.0 * (4.076781969643807 - fConst1033);
		fConst1051 = 2.0 * (0.00040767818495825777 - fConst1047);
		fConst1052 = (fConst1028 + -0.7431304601070396) / fConst1027 + 1.450071084655647;
		fConst1053 = 2.0 * (1.450071084655647 - fConst1033);
		fConst1054 = 2.0 * (1.450071084655647 - fConst1044);
		fConst1055 = (fConst1028 + -0.157482159302087) / fConst1027 + 0.9351401670315425;
		fConst1056 = 2.0 * (0.9351401670315425 - fConst1033);
		fConst1057 = 2.0 * (0.9351401670315425 - fConst1041);
		fConst1058 = (fConst1028 + -0.782413046821645) / fConst1027 + 0.24529150870616;
		fConst1059 = 2.0 * (0.24529150870616 - fConst1033);
		fConst1060 = 2.0 * (0.000433227200555 - fConst1038);
		fConst1061 = (fConst1028 + -0.512478641889141) / fConst1027 + 0.689621364484675;
		fConst1062 = 2.0 * (0.689621364484675 - fConst1033);
		fConst1063 = 2.0 * (7.621731298870603 - fConst1033);
		fConst1064 = (fConst1028 + -0.168404871113589) / fConst1027 + 1.069358407707312;
		fConst1065 = 2.0 * (1.069358407707312 - fConst1033);
		fConst1066 = 2.0 * (53.53615295455673 - fConst1033);
		fConst1067 = std::tan(4142.222044872894 / fConst0);
		fConst1068 = 1.0 / fConst1067;
		fConst1069 = (fConst1068 + 0.168404871113589) / fConst1067 + 1.069358407707312;
		fConst1070 = 1.0 / fConst1069;
		fConst1071 = 3.0 / fConst1069;
		fConst1072 = piece_nono_quando_vocoder_faustpower2_f(fConst1067);
		fConst1073 = 1.0 / fConst1072;
		fConst1074 = fConst1073 + 53.53615295455673;
		fConst1075 = 1.0 / ((fConst1068 + 0.512478641889141) / fConst1067 + 0.689621364484675);
		fConst1076 = fConst1073 + 7.621731298870603;
		fConst1077 = 1.0 / ((fConst1068 + 0.782413046821645) / fConst1067 + 0.24529150870616);
		fConst1078 = 9.9999997055e-05 / fConst1072;
		fConst1079 = fConst1078 + 0.000433227200555;
		fConst1080 = 1.0 / ((fConst1068 + 0.157482159302087) / fConst1067 + 0.9351401670315425);
		fConst1081 = 50.063807016150385 / fConst1072;
		fConst1082 = fConst1081 + 0.9351401670315425;
		fConst1083 = 1.0 / ((fConst1068 + 0.7431304601070396) / fConst1067 + 1.450071084655647);
		fConst1084 = 11.052052171507189 / fConst1072;
		fConst1085 = fConst1084 + 1.450071084655647;
		fConst1086 = 1.0 / ((fConst1068 + 3.1897274020965583) / fConst1067 + 4.076781969643807);
		fConst1087 = 0.0017661728399818856 / fConst1072;
		fConst1088 = fConst1087 + 0.00040767818495825777;
		fConst1089 = (fConst1068 + -3.1897274020965583) / fConst1067 + 4.076781969643807;
		fConst1090 = 2.0 * (4.076781969643807 - fConst1073);
		fConst1091 = 2.0 * (0.00040767818495825777 - fConst1087);
		fConst1092 = (fConst1068 + -0.7431304601070396) / fConst1067 + 1.450071084655647;
		fConst1093 = 2.0 * (1.450071084655647 - fConst1073);
		fConst1094 = 2.0 * (1.450071084655647 - fConst1084);
		fConst1095 = (fConst1068 + -0.157482159302087) / fConst1067 + 0.9351401670315425;
		fConst1096 = 2.0 * (0.9351401670315425 - fConst1073);
		fConst1097 = 2.0 * (0.9351401670315425 - fConst1081);
		fConst1098 = (fConst1068 + -0.782413046821645) / fConst1067 + 0.24529150870616;
		fConst1099 = 2.0 * (0.24529150870616 - fConst1073);
		fConst1100 = 2.0 * (0.000433227200555 - fConst1078);
		fConst1101 = (fConst1068 + -0.512478641889141) / fConst1067 + 0.689621364484675;
		fConst1102 = 2.0 * (0.689621364484675 - fConst1073);
		fConst1103 = 2.0 * (7.621731298870603 - fConst1073);
		fConst1104 = (fConst1068 + -0.168404871113589) / fConst1067 + 1.069358407707312;
		fConst1105 = 2.0 * (1.069358407707312 - fConst1073);
		fConst1106 = 2.0 * (53.53615295455673 - fConst1073);
		fConst1107 = std::tan(4649.487041040268 / fConst0);
		fConst1108 = 1.0 / fConst1107;
		fConst1109 = (fConst1108 + 0.168404871113589) / fConst1107 + 1.069358407707312;
		fConst1110 = 1.0 / fConst1109;
		fConst1111 = 3.0 / fConst1109;
		fConst1112 = piece_nono_quando_vocoder_faustpower2_f(fConst1107);
		fConst1113 = 1.0 / fConst1112;
		fConst1114 = fConst1113 + 53.53615295455673;
		fConst1115 = 1.0 / ((fConst1108 + 0.512478641889141) / fConst1107 + 0.689621364484675);
		fConst1116 = fConst1113 + 7.621731298870603;
		fConst1117 = 1.0 / ((fConst1108 + 0.782413046821645) / fConst1107 + 0.24529150870616);
		fConst1118 = 9.9999997055e-05 / fConst1112;
		fConst1119 = fConst1118 + 0.000433227200555;
		fConst1120 = 1.0 / ((fConst1108 + 0.157482159302087) / fConst1107 + 0.9351401670315425);
		fConst1121 = 50.063807016150385 / fConst1112;
		fConst1122 = fConst1121 + 0.9351401670315425;
		fConst1123 = 1.0 / ((fConst1108 + 0.7431304601070396) / fConst1107 + 1.450071084655647);
		fConst1124 = 11.052052171507189 / fConst1112;
		fConst1125 = fConst1124 + 1.450071084655647;
		fConst1126 = 1.0 / ((fConst1108 + 3.1897274020965583) / fConst1107 + 4.076781969643807);
		fConst1127 = 0.0017661728399818856 / fConst1112;
		fConst1128 = fConst1127 + 0.00040767818495825777;
		fConst1129 = (fConst1108 + -3.1897274020965583) / fConst1107 + 4.076781969643807;
		fConst1130 = 2.0 * (4.076781969643807 - fConst1113);
		fConst1131 = 2.0 * (0.00040767818495825777 - fConst1127);
		fConst1132 = (fConst1108 + -0.7431304601070396) / fConst1107 + 1.450071084655647;
		fConst1133 = 2.0 * (1.450071084655647 - fConst1113);
		fConst1134 = 2.0 * (1.450071084655647 - fConst1124);
		fConst1135 = (fConst1108 + -0.157482159302087) / fConst1107 + 0.9351401670315425;
		fConst1136 = 2.0 * (0.9351401670315425 - fConst1113);
		fConst1137 = 2.0 * (0.9351401670315425 - fConst1121);
		fConst1138 = (fConst1108 + -0.782413046821645) / fConst1107 + 0.24529150870616;
		fConst1139 = 2.0 * (0.24529150870616 - fConst1113);
		fConst1140 = 2.0 * (0.000433227200555 - fConst1118);
		fConst1141 = (fConst1108 + -0.512478641889141) / fConst1107 + 0.689621364484675;
		fConst1142 = 2.0 * (0.689621364484675 - fConst1113);
		fConst1143 = 2.0 * (7.621731298870603 - fConst1113);
		fConst1144 = (fConst1108 + -0.168404871113589) / fConst1107 + 1.069358407707312;
		fConst1145 = 2.0 * (1.069358407707312 - fConst1113);
		fConst1146 = 2.0 * (53.53615295455673 - fConst1113);
		fConst1147 = std::tan(5218.872747673946 / fConst0);
		fConst1148 = 1.0 / fConst1147;
		fConst1149 = (fConst1148 + 0.168404871113589) / fConst1147 + 1.069358407707312;
		fConst1150 = 1.0 / fConst1149;
		fConst1151 = 3.0 / fConst1149;
		fConst1152 = piece_nono_quando_vocoder_faustpower2_f(fConst1147);
		fConst1153 = 1.0 / fConst1152;
		fConst1154 = fConst1153 + 53.53615295455673;
		fConst1155 = 1.0 / ((fConst1148 + 0.512478641889141) / fConst1147 + 0.689621364484675);
		fConst1156 = fConst1153 + 7.621731298870603;
		fConst1157 = 1.0 / ((fConst1148 + 0.782413046821645) / fConst1147 + 0.24529150870616);
		fConst1158 = 9.9999997055e-05 / fConst1152;
		fConst1159 = fConst1158 + 0.000433227200555;
		fConst1160 = 1.0 / ((fConst1148 + 0.157482159302087) / fConst1147 + 0.9351401670315425);
		fConst1161 = 50.063807016150385 / fConst1152;
		fConst1162 = fConst1161 + 0.9351401670315425;
		fConst1163 = 1.0 / ((fConst1148 + 0.7431304601070396) / fConst1147 + 1.450071084655647);
		fConst1164 = 11.052052171507189 / fConst1152;
		fConst1165 = fConst1164 + 1.450071084655647;
		fConst1166 = 1.0 / ((fConst1148 + 3.1897274020965583) / fConst1147 + 4.076781969643807);
		fConst1167 = 0.0017661728399818856 / fConst1152;
		fConst1168 = fConst1167 + 0.00040767818495825777;
		fConst1169 = (fConst1148 + -3.1897274020965583) / fConst1147 + 4.076781969643807;
		fConst1170 = 2.0 * (4.076781969643807 - fConst1153);
		fConst1171 = 2.0 * (0.00040767818495825777 - fConst1167);
		fConst1172 = (fConst1148 + -0.7431304601070396) / fConst1147 + 1.450071084655647;
		fConst1173 = 2.0 * (1.450071084655647 - fConst1153);
		fConst1174 = 2.0 * (1.450071084655647 - fConst1164);
		fConst1175 = (fConst1148 + -0.157482159302087) / fConst1147 + 0.9351401670315425;
		fConst1176 = 2.0 * (0.9351401670315425 - fConst1153);
		fConst1177 = 2.0 * (0.9351401670315425 - fConst1161);
		fConst1178 = (fConst1148 + -0.782413046821645) / fConst1147 + 0.24529150870616;
		fConst1179 = 2.0 * (0.24529150870616 - fConst1153);
		fConst1180 = 2.0 * (0.000433227200555 - fConst1158);
		fConst1181 = (fConst1148 + -0.512478641889141) / fConst1147 + 0.689621364484675;
		fConst1182 = 2.0 * (0.689621364484675 - fConst1153);
		fConst1183 = 2.0 * (7.621731298870603 - fConst1153);
		fConst1184 = (fConst1148 + -0.168404871113589) / fConst1147 + 1.069358407707312;
		fConst1185 = 2.0 * (1.069358407707312 - fConst1153);
		fConst1186 = 2.0 * (53.53615295455673 - fConst1153);
		fConst1187 = std::tan(5857.986594220061 / fConst0);
		fConst1188 = 1.0 / fConst1187;
		fConst1189 = (fConst1188 + 0.168404871113589) / fConst1187 + 1.069358407707312;
		fConst1190 = 1.0 / fConst1189;
		fConst1191 = 3.0 / fConst1189;
		fConst1192 = piece_nono_quando_vocoder_faustpower2_f(fConst1187);
		fConst1193 = 1.0 / fConst1192;
		fConst1194 = fConst1193 + 53.53615295455673;
		fConst1195 = 1.0 / ((fConst1188 + 0.512478641889141) / fConst1187 + 0.689621364484675);
		fConst1196 = fConst1193 + 7.621731298870603;
		fConst1197 = 1.0 / ((fConst1188 + 0.782413046821645) / fConst1187 + 0.24529150870616);
		fConst1198 = 9.9999997055e-05 / fConst1192;
		fConst1199 = fConst1198 + 0.000433227200555;
		fConst1200 = 1.0 / ((fConst1188 + 0.157482159302087) / fConst1187 + 0.9351401670315425);
		fConst1201 = 50.063807016150385 / fConst1192;
		fConst1202 = fConst1201 + 0.9351401670315425;
		fConst1203 = 1.0 / ((fConst1188 + 0.7431304601070396) / fConst1187 + 1.450071084655647);
		fConst1204 = 11.052052171507189 / fConst1192;
		fConst1205 = fConst1204 + 1.450071084655647;
		fConst1206 = 1.0 / ((fConst1188 + 3.1897274020965583) / fConst1187 + 4.076781969643807);
		fConst1207 = 0.0017661728399818856 / fConst1192;
		fConst1208 = fConst1207 + 0.00040767818495825777;
		fConst1209 = (fConst1188 + -3.1897274020965583) / fConst1187 + 4.076781969643807;
		fConst1210 = 2.0 * (4.076781969643807 - fConst1193);
		fConst1211 = 2.0 * (0.00040767818495825777 - fConst1207);
		fConst1212 = (fConst1188 + -0.7431304601070396) / fConst1187 + 1.450071084655647;
		fConst1213 = 2.0 * (1.450071084655647 - fConst1193);
		fConst1214 = 2.0 * (1.450071084655647 - fConst1204);
		fConst1215 = (fConst1188 + -0.157482159302087) / fConst1187 + 0.9351401670315425;
		fConst1216 = 2.0 * (0.9351401670315425 - fConst1193);
		fConst1217 = 2.0 * (0.9351401670315425 - fConst1201);
		fConst1218 = (fConst1188 + -0.782413046821645) / fConst1187 + 0.24529150870616;
		fConst1219 = 2.0 * (0.24529150870616 - fConst1193);
		fConst1220 = 2.0 * (0.000433227200555 - fConst1198);
		fConst1221 = (fConst1188 + -0.512478641889141) / fConst1187 + 0.689621364484675;
		fConst1222 = 2.0 * (0.689621364484675 - fConst1193);
		fConst1223 = 2.0 * (7.621731298870603 - fConst1193);
		fConst1224 = (fConst1188 + -0.168404871113589) / fConst1187 + 1.069358407707312;
		fConst1225 = 2.0 * (1.069358407707312 - fConst1193);
		fConst1226 = 2.0 * (53.53615295455673 - fConst1193);
		fConst1227 = std::tan(6575.367631517099 / fConst0);
		fConst1228 = 1.0 / fConst1227;
		fConst1229 = (fConst1228 + 0.168404871113589) / fConst1227 + 1.069358407707312;
		fConst1230 = 1.0 / fConst1229;
		fConst1231 = 3.0 / fConst1229;
		fConst1232 = piece_nono_quando_vocoder_faustpower2_f(fConst1227);
		fConst1233 = 1.0 / fConst1232;
		fConst1234 = fConst1233 + 53.53615295455673;
		fConst1235 = 1.0 / ((fConst1228 + 0.512478641889141) / fConst1227 + 0.689621364484675);
		fConst1236 = fConst1233 + 7.621731298870603;
		fConst1237 = 1.0 / ((fConst1228 + 0.782413046821645) / fConst1227 + 0.24529150870616);
		fConst1238 = 9.9999997055e-05 / fConst1232;
		fConst1239 = fConst1238 + 0.000433227200555;
		fConst1240 = 1.0 / ((fConst1228 + 0.157482159302087) / fConst1227 + 0.9351401670315425);
		fConst1241 = 50.063807016150385 / fConst1232;
		fConst1242 = fConst1241 + 0.9351401670315425;
		fConst1243 = 1.0 / ((fConst1228 + 0.7431304601070396) / fConst1227 + 1.450071084655647);
		fConst1244 = 11.052052171507189 / fConst1232;
		fConst1245 = fConst1244 + 1.450071084655647;
		fConst1246 = 1.0 / ((fConst1228 + 3.1897274020965583) / fConst1227 + 4.076781969643807);
		fConst1247 = 0.0017661728399818856 / fConst1232;
		fConst1248 = fConst1247 + 0.00040767818495825777;
		fConst1249 = (fConst1228 + -3.1897274020965583) / fConst1227 + 4.076781969643807;
		fConst1250 = 2.0 * (4.076781969643807 - fConst1233);
		fConst1251 = 2.0 * (0.00040767818495825777 - fConst1247);
		fConst1252 = (fConst1228 + -0.7431304601070396) / fConst1227 + 1.450071084655647;
		fConst1253 = 2.0 * (1.450071084655647 - fConst1233);
		fConst1254 = 2.0 * (1.450071084655647 - fConst1244);
		fConst1255 = (fConst1228 + -0.157482159302087) / fConst1227 + 0.9351401670315425;
		fConst1256 = 2.0 * (0.9351401670315425 - fConst1233);
		fConst1257 = 2.0 * (0.9351401670315425 - fConst1241);
		fConst1258 = (fConst1228 + -0.782413046821645) / fConst1227 + 0.24529150870616;
		fConst1259 = 2.0 * (0.24529150870616 - fConst1233);
		fConst1260 = 2.0 * (0.000433227200555 - fConst1238);
		fConst1261 = (fConst1228 + -0.512478641889141) / fConst1227 + 0.689621364484675;
		fConst1262 = 2.0 * (0.689621364484675 - fConst1233);
		fConst1263 = 2.0 * (7.621731298870603 - fConst1233);
		fConst1264 = (fConst1228 + -0.168404871113589) / fConst1227 + 1.069358407707312;
		fConst1265 = 2.0 * (1.069358407707312 - fConst1233);
		fConst1266 = 2.0 * (53.53615295455673 - fConst1233);
		fConst1267 = std::tan(7380.600620059835 / fConst0);
		fConst1268 = 1.0 / fConst1267;
		fConst1269 = (fConst1268 + 0.168404871113589) / fConst1267 + 1.069358407707312;
		fConst1270 = 1.0 / fConst1269;
		fConst1271 = 3.0 / fConst1269;
		fConst1272 = piece_nono_quando_vocoder_faustpower2_f(fConst1267);
		fConst1273 = 1.0 / fConst1272;
		fConst1274 = fConst1273 + 53.53615295455673;
		fConst1275 = 1.0 / ((fConst1268 + 0.512478641889141) / fConst1267 + 0.689621364484675);
		fConst1276 = fConst1273 + 7.621731298870603;
		fConst1277 = 1.0 / ((fConst1268 + 0.782413046821645) / fConst1267 + 0.24529150870616);
		fConst1278 = 9.9999997055e-05 / fConst1272;
		fConst1279 = fConst1278 + 0.000433227200555;
		fConst1280 = 1.0 / ((fConst1268 + 0.157482159302087) / fConst1267 + 0.9351401670315425);
		fConst1281 = 50.063807016150385 / fConst1272;
		fConst1282 = fConst1281 + 0.9351401670315425;
		fConst1283 = 1.0 / ((fConst1268 + 0.7431304601070396) / fConst1267 + 1.450071084655647);
		fConst1284 = 11.052052171507189 / fConst1272;
		fConst1285 = fConst1284 + 1.450071084655647;
		fConst1286 = 1.0 / ((fConst1268 + 3.1897274020965583) / fConst1267 + 4.076781969643807);
		fConst1287 = 0.0017661728399818856 / fConst1272;
		fConst1288 = fConst1287 + 0.00040767818495825777;
		fConst1289 = (fConst1268 + -3.1897274020965583) / fConst1267 + 4.076781969643807;
		fConst1290 = 2.0 * (4.076781969643807 - fConst1273);
		fConst1291 = 2.0 * (0.00040767818495825777 - fConst1287);
		fConst1292 = (fConst1268 + -0.7431304601070396) / fConst1267 + 1.450071084655647;
		fConst1293 = 2.0 * (1.450071084655647 - fConst1273);
		fConst1294 = 2.0 * (1.450071084655647 - fConst1284);
		fConst1295 = (fConst1268 + -0.157482159302087) / fConst1267 + 0.9351401670315425;
		fConst1296 = 2.0 * (0.9351401670315425 - fConst1273);
		fConst1297 = 2.0 * (0.9351401670315425 - fConst1281);
		fConst1298 = (fConst1268 + -0.782413046821645) / fConst1267 + 0.24529150870616;
		fConst1299 = 2.0 * (0.24529150870616 - fConst1273);
		fConst1300 = 2.0 * (0.000433227200555 - fConst1278);
		fConst1301 = (fConst1268 + -0.512478641889141) / fConst1267 + 0.689621364484675;
		fConst1302 = 2.0 * (0.689621364484675 - fConst1273);
		fConst1303 = 2.0 * (7.621731298870603 - fConst1273);
		fConst1304 = (fConst1268 + -0.168404871113589) / fConst1267 + 1.069358407707312;
		fConst1305 = 2.0 * (1.069358407707312 - fConst1273);
		fConst1306 = 2.0 * (53.53615295455673 - fConst1273);
		fConst1307 = std::tan(8284.444089745788 / fConst0);
		fConst1308 = 1.0 / fConst1307;
		fConst1309 = (fConst1308 + 0.168404871113589) / fConst1307 + 1.069358407707312;
		fConst1310 = 1.0 / fConst1309;
		fConst1311 = 3.0 / fConst1309;
		fConst1312 = piece_nono_quando_vocoder_faustpower2_f(fConst1307);
		fConst1313 = 1.0 / fConst1312;
		fConst1314 = fConst1313 + 53.53615295455673;
		fConst1315 = 1.0 / ((fConst1308 + 0.512478641889141) / fConst1307 + 0.689621364484675);
		fConst1316 = fConst1313 + 7.621731298870603;
		fConst1317 = 1.0 / ((fConst1308 + 0.782413046821645) / fConst1307 + 0.24529150870616);
		fConst1318 = 9.9999997055e-05 / fConst1312;
		fConst1319 = fConst1318 + 0.000433227200555;
		fConst1320 = 1.0 / ((fConst1308 + 0.157482159302087) / fConst1307 + 0.9351401670315425);
		fConst1321 = 50.063807016150385 / fConst1312;
		fConst1322 = fConst1321 + 0.9351401670315425;
		fConst1323 = 1.0 / ((fConst1308 + 0.7431304601070396) / fConst1307 + 1.450071084655647);
		fConst1324 = 11.052052171507189 / fConst1312;
		fConst1325 = fConst1324 + 1.450071084655647;
		fConst1326 = 1.0 / ((fConst1308 + 3.1897274020965583) / fConst1307 + 4.076781969643807);
		fConst1327 = 0.0017661728399818856 / fConst1312;
		fConst1328 = fConst1327 + 0.00040767818495825777;
		fConst1329 = (fConst1308 + -3.1897274020965583) / fConst1307 + 4.076781969643807;
		fConst1330 = 2.0 * (4.076781969643807 - fConst1313);
		fConst1331 = 2.0 * (0.00040767818495825777 - fConst1327);
		fConst1332 = (fConst1308 + -0.7431304601070396) / fConst1307 + 1.450071084655647;
		fConst1333 = 2.0 * (1.450071084655647 - fConst1313);
		fConst1334 = 2.0 * (1.450071084655647 - fConst1324);
		fConst1335 = (fConst1308 + -0.157482159302087) / fConst1307 + 0.9351401670315425;
		fConst1336 = 2.0 * (0.9351401670315425 - fConst1313);
		fConst1337 = 2.0 * (0.9351401670315425 - fConst1321);
		fConst1338 = (fConst1308 + -0.782413046821645) / fConst1307 + 0.24529150870616;
		fConst1339 = 2.0 * (0.24529150870616 - fConst1313);
		fConst1340 = 2.0 * (0.000433227200555 - fConst1318);
		fConst1341 = (fConst1308 + -0.512478641889141) / fConst1307 + 0.689621364484675;
		fConst1342 = 2.0 * (0.689621364484675 - fConst1313);
		fConst1343 = 2.0 * (7.621731298870603 - fConst1313);
		fConst1344 = (fConst1308 + -0.168404871113589) / fConst1307 + 1.069358407707312;
		fConst1345 = 2.0 * (1.069358407707312 - fConst1313);
		fConst1346 = 2.0 * (53.53615295455673 - fConst1313);
		fConst1347 = std::tan(9298.974082080536 / fConst0);
		fConst1348 = 1.0 / fConst1347;
		fConst1349 = (fConst1348 + 0.168404871113589) / fConst1347 + 1.069358407707312;
		fConst1350 = 1.0 / fConst1349;
		fConst1351 = 3.0 / fConst1349;
		fConst1352 = piece_nono_quando_vocoder_faustpower2_f(fConst1347);
		fConst1353 = 1.0 / fConst1352;
		fConst1354 = fConst1353 + 53.53615295455673;
		fConst1355 = 1.0 / ((fConst1348 + 0.512478641889141) / fConst1347 + 0.689621364484675);
		fConst1356 = fConst1353 + 7.621731298870603;
		fConst1357 = 1.0 / ((fConst1348 + 0.782413046821645) / fConst1347 + 0.24529150870616);
		fConst1358 = 9.9999997055e-05 / fConst1352;
		fConst1359 = fConst1358 + 0.000433227200555;
		fConst1360 = 1.0 / ((fConst1348 + 0.157482159302087) / fConst1347 + 0.9351401670315425);
		fConst1361 = 50.063807016150385 / fConst1352;
		fConst1362 = fConst1361 + 0.9351401670315425;
		fConst1363 = 1.0 / ((fConst1348 + 0.7431304601070396) / fConst1347 + 1.450071084655647);
		fConst1364 = 11.052052171507189 / fConst1352;
		fConst1365 = fConst1364 + 1.450071084655647;
		fConst1366 = 1.0 / ((fConst1348 + 3.1897274020965583) / fConst1347 + 4.076781969643807);
		fConst1367 = 0.0017661728399818856 / fConst1352;
		fConst1368 = fConst1367 + 0.00040767818495825777;
		fConst1369 = (fConst1348 + -3.1897274020965583) / fConst1347 + 4.076781969643807;
		fConst1370 = 2.0 * (4.076781969643807 - fConst1353);
		fConst1371 = 2.0 * (0.00040767818495825777 - fConst1367);
		fConst1372 = (fConst1348 + -0.7431304601070396) / fConst1347 + 1.450071084655647;
		fConst1373 = 2.0 * (1.450071084655647 - fConst1353);
		fConst1374 = 2.0 * (1.450071084655647 - fConst1364);
		fConst1375 = (fConst1348 + -0.157482159302087) / fConst1347 + 0.9351401670315425;
		fConst1376 = 2.0 * (0.9351401670315425 - fConst1353);
		fConst1377 = 2.0 * (0.9351401670315425 - fConst1361);
		fConst1378 = (fConst1348 + -0.782413046821645) / fConst1347 + 0.24529150870616;
		fConst1379 = 2.0 * (0.24529150870616 - fConst1353);
		fConst1380 = 2.0 * (0.000433227200555 - fConst1358);
		fConst1381 = (fConst1348 + -0.512478641889141) / fConst1347 + 0.689621364484675;
		fConst1382 = 2.0 * (0.689621364484675 - fConst1353);
		fConst1383 = 2.0 * (7.621731298870603 - fConst1353);
		fConst1384 = (fConst1348 + -0.168404871113589) / fConst1347 + 1.069358407707312;
		fConst1385 = 2.0 * (1.069358407707312 - fConst1353);
		fConst1386 = 2.0 * (53.53615295455673 - fConst1353);
		fConst1387 = std::tan(10437.745495347892 / fConst0);
		fConst1388 = 1.0 / fConst1387;
		fConst1389 = (fConst1388 + 0.168404871113589) / fConst1387 + 1.069358407707312;
		fConst1390 = 1.0 / fConst1389;
		fConst1391 = 3.0 / fConst1389;
		fConst1392 = piece_nono_quando_vocoder_faustpower2_f(fConst1387);
		fConst1393 = 1.0 / fConst1392;
		fConst1394 = fConst1393 + 53.53615295455673;
		fConst1395 = 1.0 / ((fConst1388 + 0.512478641889141) / fConst1387 + 0.689621364484675);
		fConst1396 = fConst1393 + 7.621731298870603;
		fConst1397 = 1.0 / ((fConst1388 + 0.782413046821645) / fConst1387 + 0.24529150870616);
		fConst1398 = 9.9999997055e-05 / fConst1392;
		fConst1399 = fConst1398 + 0.000433227200555;
		fConst1400 = 1.0 / ((fConst1388 + 0.157482159302087) / fConst1387 + 0.9351401670315425);
		fConst1401 = 50.063807016150385 / fConst1392;
		fConst1402 = fConst1401 + 0.9351401670315425;
		fConst1403 = 1.0 / ((fConst1388 + 0.7431304601070396) / fConst1387 + 1.450071084655647);
		fConst1404 = 11.052052171507189 / fConst1392;
		fConst1405 = fConst1404 + 1.450071084655647;
		fConst1406 = 1.0 / ((fConst1388 + 3.1897274020965583) / fConst1387 + 4.076781969643807);
		fConst1407 = 0.0017661728399818856 / fConst1392;
		fConst1408 = fConst1407 + 0.00040767818495825777;
		fConst1409 = (fConst1388 + -3.1897274020965583) / fConst1387 + 4.076781969643807;
		fConst1410 = 2.0 * (4.076781969643807 - fConst1393);
		fConst1411 = 2.0 * (0.00040767818495825777 - fConst1407);
		fConst1412 = (fConst1388 + -0.7431304601070396) / fConst1387 + 1.450071084655647;
		fConst1413 = 2.0 * (1.450071084655647 - fConst1393);
		fConst1414 = 2.0 * (1.450071084655647 - fConst1404);
		fConst1415 = (fConst1388 + -0.157482159302087) / fConst1387 + 0.9351401670315425;
		fConst1416 = 2.0 * (0.9351401670315425 - fConst1393);
		fConst1417 = 2.0 * (0.9351401670315425 - fConst1401);
		fConst1418 = (fConst1388 + -0.782413046821645) / fConst1387 + 0.24529150870616;
		fConst1419 = 2.0 * (0.24529150870616 - fConst1393);
		fConst1420 = 2.0 * (0.000433227200555 - fConst1398);
		fConst1421 = (fConst1388 + -0.512478641889141) / fConst1387 + 0.689621364484675;
		fConst1422 = 2.0 * (0.689621364484675 - fConst1393);
		fConst1423 = 2.0 * (7.621731298870603 - fConst1393);
		fConst1424 = (fConst1388 + -0.168404871113589) / fConst1387 + 1.069358407707312;
		fConst1425 = 2.0 * (1.069358407707312 - fConst1393);
		fConst1426 = 2.0 * (53.53615295455673 - fConst1393);
		fConst1427 = std::tan(11715.973188440123 / fConst0);
		fConst1428 = 1.0 / fConst1427;
		fConst1429 = (fConst1428 + 0.168404871113589) / fConst1427 + 1.069358407707312;
		fConst1430 = 1.0 / fConst1429;
		fConst1431 = 3.0 / fConst1429;
		fConst1432 = piece_nono_quando_vocoder_faustpower2_f(fConst1427);
		fConst1433 = 1.0 / fConst1432;
		fConst1434 = fConst1433 + 53.53615295455673;
		fConst1435 = 1.0 / ((fConst1428 + 0.512478641889141) / fConst1427 + 0.689621364484675);
		fConst1436 = fConst1433 + 7.621731298870603;
		fConst1437 = 1.0 / ((fConst1428 + 0.782413046821645) / fConst1427 + 0.24529150870616);
		fConst1438 = 9.9999997055e-05 / fConst1432;
		fConst1439 = fConst1438 + 0.000433227200555;
		fConst1440 = 1.0 / ((fConst1428 + 0.157482159302087) / fConst1427 + 0.9351401670315425);
		fConst1441 = 50.063807016150385 / fConst1432;
		fConst1442 = fConst1441 + 0.9351401670315425;
		fConst1443 = 1.0 / ((fConst1428 + 0.7431304601070396) / fConst1427 + 1.450071084655647);
		fConst1444 = 11.052052171507189 / fConst1432;
		fConst1445 = fConst1444 + 1.450071084655647;
		fConst1446 = 1.0 / ((fConst1428 + 3.1897274020965583) / fConst1427 + 4.076781969643807);
		fConst1447 = 0.0017661728399818856 / fConst1432;
		fConst1448 = fConst1447 + 0.00040767818495825777;
		fConst1449 = (fConst1428 + -3.1897274020965583) / fConst1427 + 4.076781969643807;
		fConst1450 = 2.0 * (4.076781969643807 - fConst1433);
		fConst1451 = 2.0 * (0.00040767818495825777 - fConst1447);
		fConst1452 = (fConst1428 + -0.7431304601070396) / fConst1427 + 1.450071084655647;
		fConst1453 = 2.0 * (1.450071084655647 - fConst1433);
		fConst1454 = 2.0 * (1.450071084655647 - fConst1444);
		fConst1455 = (fConst1428 + -0.157482159302087) / fConst1427 + 0.9351401670315425;
		fConst1456 = 2.0 * (0.9351401670315425 - fConst1433);
		fConst1457 = 2.0 * (0.9351401670315425 - fConst1441);
		fConst1458 = (fConst1428 + -0.782413046821645) / fConst1427 + 0.24529150870616;
		fConst1459 = 2.0 * (0.24529150870616 - fConst1433);
		fConst1460 = 2.0 * (0.000433227200555 - fConst1438);
		fConst1461 = (fConst1428 + -0.512478641889141) / fConst1427 + 0.689621364484675;
		fConst1462 = 2.0 * (0.689621364484675 - fConst1433);
		fConst1463 = 2.0 * (7.621731298870603 - fConst1433);
		fConst1464 = (fConst1428 + -0.168404871113589) / fConst1427 + 1.069358407707312;
		fConst1465 = 2.0 * (1.069358407707312 - fConst1433);
		fConst1466 = 2.0 * (53.53615295455673 - fConst1433);
		fConst1467 = std::exp(-(1e+03 / fConst0));
		fConst1468 = std::exp(-(1e+02 / fConst0));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1e+02);
		fHslider1 = FAUSTFLOAT(1e+01);
		fHslider2 = FAUSTFLOAT(0.0);
		fHslider3 = FAUSTFLOAT(-0.5);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 3; l0 = l0 + 1) {
			fRec13[l0] = 0.0;
		}
		for (int l1 = 0; l1 < 3; l1 = l1 + 1) {
			fRec12[l1] = 0.0;
		}
		for (int l2 = 0; l2 < 3; l2 = l2 + 1) {
			fRec11[l2] = 0.0;
		}
		for (int l3 = 0; l3 < 3; l3 = l3 + 1) {
			fRec10[l3] = 0.0;
		}
		for (int l4 = 0; l4 < 3; l4 = l4 + 1) {
			fRec9[l4] = 0.0;
		}
		for (int l5 = 0; l5 < 3; l5 = l5 + 1) {
			fRec8[l5] = 0.0;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec7[l6] = 0.0;
		}
		for (int l7 = 0; l7 < 3; l7 = l7 + 1) {
			fRec6[l7] = 0.0;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec5[l8] = 0.0;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec4[l9] = 0.0;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec3[l10] = 0.0;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec2[l11] = 0.0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec14[l12] = 0.0;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec15[l13] = 0.0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec1[l14] = 0.0;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec28[l15] = 0.0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			iRec35[l16] = 0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fVec0[l17] = 0.0;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec34[l18] = 0.0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec33[l19] = 0.0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fVec1[l20] = 0.0;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec32[l21] = 0.0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec2[l22] = 0.0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec31[l23] = 0.0;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fVec3[l24] = 0.0;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec30[l25] = 0.0;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fVec4[l26] = 0.0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec29[l27] = 0.0;
		}
		for (int l28 = 0; l28 < 3; l28 = l28 + 1) {
			fRec27[l28] = 0.0;
		}
		for (int l29 = 0; l29 < 3; l29 = l29 + 1) {
			fRec26[l29] = 0.0;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec25[l30] = 0.0;
		}
		for (int l31 = 0; l31 < 3; l31 = l31 + 1) {
			fRec24[l31] = 0.0;
		}
		for (int l32 = 0; l32 < 3; l32 = l32 + 1) {
			fRec23[l32] = 0.0;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fRec22[l33] = 0.0;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fRec21[l34] = 0.0;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fRec20[l35] = 0.0;
		}
		for (int l36 = 0; l36 < 3; l36 = l36 + 1) {
			fRec19[l36] = 0.0;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fRec18[l37] = 0.0;
		}
		for (int l38 = 0; l38 < 3; l38 = l38 + 1) {
			fRec17[l38] = 0.0;
		}
		for (int l39 = 0; l39 < 3; l39 = l39 + 1) {
			fRec16[l39] = 0.0;
		}
		for (int l40 = 0; l40 < 3; l40 = l40 + 1) {
			fRec48[l40] = 0.0;
		}
		for (int l41 = 0; l41 < 3; l41 = l41 + 1) {
			fRec47[l41] = 0.0;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec46[l42] = 0.0;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fRec45[l43] = 0.0;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec44[l44] = 0.0;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec43[l45] = 0.0;
		}
		for (int l46 = 0; l46 < 3; l46 = l46 + 1) {
			fRec42[l46] = 0.0;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec41[l47] = 0.0;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fRec40[l48] = 0.0;
		}
		for (int l49 = 0; l49 < 3; l49 = l49 + 1) {
			fRec39[l49] = 0.0;
		}
		for (int l50 = 0; l50 < 3; l50 = l50 + 1) {
			fRec38[l50] = 0.0;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec37[l51] = 0.0;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec36[l52] = 0.0;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fRec60[l53] = 0.0;
		}
		for (int l54 = 0; l54 < 3; l54 = l54 + 1) {
			fRec59[l54] = 0.0;
		}
		for (int l55 = 0; l55 < 3; l55 = l55 + 1) {
			fRec58[l55] = 0.0;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fRec57[l56] = 0.0;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fRec56[l57] = 0.0;
		}
		for (int l58 = 0; l58 < 3; l58 = l58 + 1) {
			fRec55[l58] = 0.0;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec54[l59] = 0.0;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec53[l60] = 0.0;
		}
		for (int l61 = 0; l61 < 3; l61 = l61 + 1) {
			fRec52[l61] = 0.0;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec51[l62] = 0.0;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fRec50[l63] = 0.0;
		}
		for (int l64 = 0; l64 < 3; l64 = l64 + 1) {
			fRec49[l64] = 0.0;
		}
		for (int l65 = 0; l65 < 3; l65 = l65 + 1) {
			fRec73[l65] = 0.0;
		}
		for (int l66 = 0; l66 < 3; l66 = l66 + 1) {
			fRec72[l66] = 0.0;
		}
		for (int l67 = 0; l67 < 3; l67 = l67 + 1) {
			fRec71[l67] = 0.0;
		}
		for (int l68 = 0; l68 < 3; l68 = l68 + 1) {
			fRec70[l68] = 0.0;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec69[l69] = 0.0;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fRec68[l70] = 0.0;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec67[l71] = 0.0;
		}
		for (int l72 = 0; l72 < 3; l72 = l72 + 1) {
			fRec66[l72] = 0.0;
		}
		for (int l73 = 0; l73 < 3; l73 = l73 + 1) {
			fRec65[l73] = 0.0;
		}
		for (int l74 = 0; l74 < 3; l74 = l74 + 1) {
			fRec64[l74] = 0.0;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec63[l75] = 0.0;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fRec62[l76] = 0.0;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fRec61[l77] = 0.0;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fRec85[l78] = 0.0;
		}
		for (int l79 = 0; l79 < 3; l79 = l79 + 1) {
			fRec84[l79] = 0.0;
		}
		for (int l80 = 0; l80 < 3; l80 = l80 + 1) {
			fRec83[l80] = 0.0;
		}
		for (int l81 = 0; l81 < 3; l81 = l81 + 1) {
			fRec82[l81] = 0.0;
		}
		for (int l82 = 0; l82 < 3; l82 = l82 + 1) {
			fRec81[l82] = 0.0;
		}
		for (int l83 = 0; l83 < 3; l83 = l83 + 1) {
			fRec80[l83] = 0.0;
		}
		for (int l84 = 0; l84 < 3; l84 = l84 + 1) {
			fRec79[l84] = 0.0;
		}
		for (int l85 = 0; l85 < 3; l85 = l85 + 1) {
			fRec78[l85] = 0.0;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec77[l86] = 0.0;
		}
		for (int l87 = 0; l87 < 3; l87 = l87 + 1) {
			fRec76[l87] = 0.0;
		}
		for (int l88 = 0; l88 < 3; l88 = l88 + 1) {
			fRec75[l88] = 0.0;
		}
		for (int l89 = 0; l89 < 3; l89 = l89 + 1) {
			fRec74[l89] = 0.0;
		}
		for (int l90 = 0; l90 < 3; l90 = l90 + 1) {
			fRec98[l90] = 0.0;
		}
		for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
			fRec97[l91] = 0.0;
		}
		for (int l92 = 0; l92 < 3; l92 = l92 + 1) {
			fRec96[l92] = 0.0;
		}
		for (int l93 = 0; l93 < 3; l93 = l93 + 1) {
			fRec95[l93] = 0.0;
		}
		for (int l94 = 0; l94 < 3; l94 = l94 + 1) {
			fRec94[l94] = 0.0;
		}
		for (int l95 = 0; l95 < 3; l95 = l95 + 1) {
			fRec93[l95] = 0.0;
		}
		for (int l96 = 0; l96 < 3; l96 = l96 + 1) {
			fRec92[l96] = 0.0;
		}
		for (int l97 = 0; l97 < 3; l97 = l97 + 1) {
			fRec91[l97] = 0.0;
		}
		for (int l98 = 0; l98 < 3; l98 = l98 + 1) {
			fRec90[l98] = 0.0;
		}
		for (int l99 = 0; l99 < 3; l99 = l99 + 1) {
			fRec89[l99] = 0.0;
		}
		for (int l100 = 0; l100 < 3; l100 = l100 + 1) {
			fRec88[l100] = 0.0;
		}
		for (int l101 = 0; l101 < 3; l101 = l101 + 1) {
			fRec87[l101] = 0.0;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fRec86[l102] = 0.0;
		}
		for (int l103 = 0; l103 < 3; l103 = l103 + 1) {
			fRec110[l103] = 0.0;
		}
		for (int l104 = 0; l104 < 3; l104 = l104 + 1) {
			fRec109[l104] = 0.0;
		}
		for (int l105 = 0; l105 < 3; l105 = l105 + 1) {
			fRec108[l105] = 0.0;
		}
		for (int l106 = 0; l106 < 3; l106 = l106 + 1) {
			fRec107[l106] = 0.0;
		}
		for (int l107 = 0; l107 < 3; l107 = l107 + 1) {
			fRec106[l107] = 0.0;
		}
		for (int l108 = 0; l108 < 3; l108 = l108 + 1) {
			fRec105[l108] = 0.0;
		}
		for (int l109 = 0; l109 < 3; l109 = l109 + 1) {
			fRec104[l109] = 0.0;
		}
		for (int l110 = 0; l110 < 3; l110 = l110 + 1) {
			fRec103[l110] = 0.0;
		}
		for (int l111 = 0; l111 < 3; l111 = l111 + 1) {
			fRec102[l111] = 0.0;
		}
		for (int l112 = 0; l112 < 3; l112 = l112 + 1) {
			fRec101[l112] = 0.0;
		}
		for (int l113 = 0; l113 < 3; l113 = l113 + 1) {
			fRec100[l113] = 0.0;
		}
		for (int l114 = 0; l114 < 3; l114 = l114 + 1) {
			fRec99[l114] = 0.0;
		}
		for (int l115 = 0; l115 < 3; l115 = l115 + 1) {
			fRec123[l115] = 0.0;
		}
		for (int l116 = 0; l116 < 3; l116 = l116 + 1) {
			fRec122[l116] = 0.0;
		}
		for (int l117 = 0; l117 < 3; l117 = l117 + 1) {
			fRec121[l117] = 0.0;
		}
		for (int l118 = 0; l118 < 3; l118 = l118 + 1) {
			fRec120[l118] = 0.0;
		}
		for (int l119 = 0; l119 < 3; l119 = l119 + 1) {
			fRec119[l119] = 0.0;
		}
		for (int l120 = 0; l120 < 3; l120 = l120 + 1) {
			fRec118[l120] = 0.0;
		}
		for (int l121 = 0; l121 < 3; l121 = l121 + 1) {
			fRec117[l121] = 0.0;
		}
		for (int l122 = 0; l122 < 3; l122 = l122 + 1) {
			fRec116[l122] = 0.0;
		}
		for (int l123 = 0; l123 < 3; l123 = l123 + 1) {
			fRec115[l123] = 0.0;
		}
		for (int l124 = 0; l124 < 3; l124 = l124 + 1) {
			fRec114[l124] = 0.0;
		}
		for (int l125 = 0; l125 < 3; l125 = l125 + 1) {
			fRec113[l125] = 0.0;
		}
		for (int l126 = 0; l126 < 3; l126 = l126 + 1) {
			fRec112[l126] = 0.0;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec111[l127] = 0.0;
		}
		for (int l128 = 0; l128 < 3; l128 = l128 + 1) {
			fRec135[l128] = 0.0;
		}
		for (int l129 = 0; l129 < 3; l129 = l129 + 1) {
			fRec134[l129] = 0.0;
		}
		for (int l130 = 0; l130 < 3; l130 = l130 + 1) {
			fRec133[l130] = 0.0;
		}
		for (int l131 = 0; l131 < 3; l131 = l131 + 1) {
			fRec132[l131] = 0.0;
		}
		for (int l132 = 0; l132 < 3; l132 = l132 + 1) {
			fRec131[l132] = 0.0;
		}
		for (int l133 = 0; l133 < 3; l133 = l133 + 1) {
			fRec130[l133] = 0.0;
		}
		for (int l134 = 0; l134 < 3; l134 = l134 + 1) {
			fRec129[l134] = 0.0;
		}
		for (int l135 = 0; l135 < 3; l135 = l135 + 1) {
			fRec128[l135] = 0.0;
		}
		for (int l136 = 0; l136 < 3; l136 = l136 + 1) {
			fRec127[l136] = 0.0;
		}
		for (int l137 = 0; l137 < 3; l137 = l137 + 1) {
			fRec126[l137] = 0.0;
		}
		for (int l138 = 0; l138 < 3; l138 = l138 + 1) {
			fRec125[l138] = 0.0;
		}
		for (int l139 = 0; l139 < 3; l139 = l139 + 1) {
			fRec124[l139] = 0.0;
		}
		for (int l140 = 0; l140 < 3; l140 = l140 + 1) {
			fRec148[l140] = 0.0;
		}
		for (int l141 = 0; l141 < 3; l141 = l141 + 1) {
			fRec147[l141] = 0.0;
		}
		for (int l142 = 0; l142 < 3; l142 = l142 + 1) {
			fRec146[l142] = 0.0;
		}
		for (int l143 = 0; l143 < 3; l143 = l143 + 1) {
			fRec145[l143] = 0.0;
		}
		for (int l144 = 0; l144 < 3; l144 = l144 + 1) {
			fRec144[l144] = 0.0;
		}
		for (int l145 = 0; l145 < 3; l145 = l145 + 1) {
			fRec143[l145] = 0.0;
		}
		for (int l146 = 0; l146 < 3; l146 = l146 + 1) {
			fRec142[l146] = 0.0;
		}
		for (int l147 = 0; l147 < 3; l147 = l147 + 1) {
			fRec141[l147] = 0.0;
		}
		for (int l148 = 0; l148 < 3; l148 = l148 + 1) {
			fRec140[l148] = 0.0;
		}
		for (int l149 = 0; l149 < 3; l149 = l149 + 1) {
			fRec139[l149] = 0.0;
		}
		for (int l150 = 0; l150 < 3; l150 = l150 + 1) {
			fRec138[l150] = 0.0;
		}
		for (int l151 = 0; l151 < 3; l151 = l151 + 1) {
			fRec137[l151] = 0.0;
		}
		for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
			fRec136[l152] = 0.0;
		}
		for (int l153 = 0; l153 < 3; l153 = l153 + 1) {
			fRec160[l153] = 0.0;
		}
		for (int l154 = 0; l154 < 3; l154 = l154 + 1) {
			fRec159[l154] = 0.0;
		}
		for (int l155 = 0; l155 < 3; l155 = l155 + 1) {
			fRec158[l155] = 0.0;
		}
		for (int l156 = 0; l156 < 3; l156 = l156 + 1) {
			fRec157[l156] = 0.0;
		}
		for (int l157 = 0; l157 < 3; l157 = l157 + 1) {
			fRec156[l157] = 0.0;
		}
		for (int l158 = 0; l158 < 3; l158 = l158 + 1) {
			fRec155[l158] = 0.0;
		}
		for (int l159 = 0; l159 < 3; l159 = l159 + 1) {
			fRec154[l159] = 0.0;
		}
		for (int l160 = 0; l160 < 3; l160 = l160 + 1) {
			fRec153[l160] = 0.0;
		}
		for (int l161 = 0; l161 < 3; l161 = l161 + 1) {
			fRec152[l161] = 0.0;
		}
		for (int l162 = 0; l162 < 3; l162 = l162 + 1) {
			fRec151[l162] = 0.0;
		}
		for (int l163 = 0; l163 < 3; l163 = l163 + 1) {
			fRec150[l163] = 0.0;
		}
		for (int l164 = 0; l164 < 3; l164 = l164 + 1) {
			fRec149[l164] = 0.0;
		}
		for (int l165 = 0; l165 < 3; l165 = l165 + 1) {
			fRec173[l165] = 0.0;
		}
		for (int l166 = 0; l166 < 3; l166 = l166 + 1) {
			fRec172[l166] = 0.0;
		}
		for (int l167 = 0; l167 < 3; l167 = l167 + 1) {
			fRec171[l167] = 0.0;
		}
		for (int l168 = 0; l168 < 3; l168 = l168 + 1) {
			fRec170[l168] = 0.0;
		}
		for (int l169 = 0; l169 < 3; l169 = l169 + 1) {
			fRec169[l169] = 0.0;
		}
		for (int l170 = 0; l170 < 3; l170 = l170 + 1) {
			fRec168[l170] = 0.0;
		}
		for (int l171 = 0; l171 < 3; l171 = l171 + 1) {
			fRec167[l171] = 0.0;
		}
		for (int l172 = 0; l172 < 3; l172 = l172 + 1) {
			fRec166[l172] = 0.0;
		}
		for (int l173 = 0; l173 < 3; l173 = l173 + 1) {
			fRec165[l173] = 0.0;
		}
		for (int l174 = 0; l174 < 3; l174 = l174 + 1) {
			fRec164[l174] = 0.0;
		}
		for (int l175 = 0; l175 < 3; l175 = l175 + 1) {
			fRec163[l175] = 0.0;
		}
		for (int l176 = 0; l176 < 3; l176 = l176 + 1) {
			fRec162[l176] = 0.0;
		}
		for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
			fRec161[l177] = 0.0;
		}
		for (int l178 = 0; l178 < 3; l178 = l178 + 1) {
			fRec185[l178] = 0.0;
		}
		for (int l179 = 0; l179 < 3; l179 = l179 + 1) {
			fRec184[l179] = 0.0;
		}
		for (int l180 = 0; l180 < 3; l180 = l180 + 1) {
			fRec183[l180] = 0.0;
		}
		for (int l181 = 0; l181 < 3; l181 = l181 + 1) {
			fRec182[l181] = 0.0;
		}
		for (int l182 = 0; l182 < 3; l182 = l182 + 1) {
			fRec181[l182] = 0.0;
		}
		for (int l183 = 0; l183 < 3; l183 = l183 + 1) {
			fRec180[l183] = 0.0;
		}
		for (int l184 = 0; l184 < 3; l184 = l184 + 1) {
			fRec179[l184] = 0.0;
		}
		for (int l185 = 0; l185 < 3; l185 = l185 + 1) {
			fRec178[l185] = 0.0;
		}
		for (int l186 = 0; l186 < 3; l186 = l186 + 1) {
			fRec177[l186] = 0.0;
		}
		for (int l187 = 0; l187 < 3; l187 = l187 + 1) {
			fRec176[l187] = 0.0;
		}
		for (int l188 = 0; l188 < 3; l188 = l188 + 1) {
			fRec175[l188] = 0.0;
		}
		for (int l189 = 0; l189 < 3; l189 = l189 + 1) {
			fRec174[l189] = 0.0;
		}
		for (int l190 = 0; l190 < 3; l190 = l190 + 1) {
			fRec198[l190] = 0.0;
		}
		for (int l191 = 0; l191 < 3; l191 = l191 + 1) {
			fRec197[l191] = 0.0;
		}
		for (int l192 = 0; l192 < 3; l192 = l192 + 1) {
			fRec196[l192] = 0.0;
		}
		for (int l193 = 0; l193 < 3; l193 = l193 + 1) {
			fRec195[l193] = 0.0;
		}
		for (int l194 = 0; l194 < 3; l194 = l194 + 1) {
			fRec194[l194] = 0.0;
		}
		for (int l195 = 0; l195 < 3; l195 = l195 + 1) {
			fRec193[l195] = 0.0;
		}
		for (int l196 = 0; l196 < 3; l196 = l196 + 1) {
			fRec192[l196] = 0.0;
		}
		for (int l197 = 0; l197 < 3; l197 = l197 + 1) {
			fRec191[l197] = 0.0;
		}
		for (int l198 = 0; l198 < 3; l198 = l198 + 1) {
			fRec190[l198] = 0.0;
		}
		for (int l199 = 0; l199 < 3; l199 = l199 + 1) {
			fRec189[l199] = 0.0;
		}
		for (int l200 = 0; l200 < 3; l200 = l200 + 1) {
			fRec188[l200] = 0.0;
		}
		for (int l201 = 0; l201 < 3; l201 = l201 + 1) {
			fRec187[l201] = 0.0;
		}
		for (int l202 = 0; l202 < 2; l202 = l202 + 1) {
			fRec186[l202] = 0.0;
		}
		for (int l203 = 0; l203 < 3; l203 = l203 + 1) {
			fRec210[l203] = 0.0;
		}
		for (int l204 = 0; l204 < 3; l204 = l204 + 1) {
			fRec209[l204] = 0.0;
		}
		for (int l205 = 0; l205 < 3; l205 = l205 + 1) {
			fRec208[l205] = 0.0;
		}
		for (int l206 = 0; l206 < 3; l206 = l206 + 1) {
			fRec207[l206] = 0.0;
		}
		for (int l207 = 0; l207 < 3; l207 = l207 + 1) {
			fRec206[l207] = 0.0;
		}
		for (int l208 = 0; l208 < 3; l208 = l208 + 1) {
			fRec205[l208] = 0.0;
		}
		for (int l209 = 0; l209 < 3; l209 = l209 + 1) {
			fRec204[l209] = 0.0;
		}
		for (int l210 = 0; l210 < 3; l210 = l210 + 1) {
			fRec203[l210] = 0.0;
		}
		for (int l211 = 0; l211 < 3; l211 = l211 + 1) {
			fRec202[l211] = 0.0;
		}
		for (int l212 = 0; l212 < 3; l212 = l212 + 1) {
			fRec201[l212] = 0.0;
		}
		for (int l213 = 0; l213 < 3; l213 = l213 + 1) {
			fRec200[l213] = 0.0;
		}
		for (int l214 = 0; l214 < 3; l214 = l214 + 1) {
			fRec199[l214] = 0.0;
		}
		for (int l215 = 0; l215 < 3; l215 = l215 + 1) {
			fRec223[l215] = 0.0;
		}
		for (int l216 = 0; l216 < 3; l216 = l216 + 1) {
			fRec222[l216] = 0.0;
		}
		for (int l217 = 0; l217 < 3; l217 = l217 + 1) {
			fRec221[l217] = 0.0;
		}
		for (int l218 = 0; l218 < 3; l218 = l218 + 1) {
			fRec220[l218] = 0.0;
		}
		for (int l219 = 0; l219 < 3; l219 = l219 + 1) {
			fRec219[l219] = 0.0;
		}
		for (int l220 = 0; l220 < 3; l220 = l220 + 1) {
			fRec218[l220] = 0.0;
		}
		for (int l221 = 0; l221 < 3; l221 = l221 + 1) {
			fRec217[l221] = 0.0;
		}
		for (int l222 = 0; l222 < 3; l222 = l222 + 1) {
			fRec216[l222] = 0.0;
		}
		for (int l223 = 0; l223 < 3; l223 = l223 + 1) {
			fRec215[l223] = 0.0;
		}
		for (int l224 = 0; l224 < 3; l224 = l224 + 1) {
			fRec214[l224] = 0.0;
		}
		for (int l225 = 0; l225 < 3; l225 = l225 + 1) {
			fRec213[l225] = 0.0;
		}
		for (int l226 = 0; l226 < 3; l226 = l226 + 1) {
			fRec212[l226] = 0.0;
		}
		for (int l227 = 0; l227 < 2; l227 = l227 + 1) {
			fRec211[l227] = 0.0;
		}
		for (int l228 = 0; l228 < 3; l228 = l228 + 1) {
			fRec235[l228] = 0.0;
		}
		for (int l229 = 0; l229 < 3; l229 = l229 + 1) {
			fRec234[l229] = 0.0;
		}
		for (int l230 = 0; l230 < 3; l230 = l230 + 1) {
			fRec233[l230] = 0.0;
		}
		for (int l231 = 0; l231 < 3; l231 = l231 + 1) {
			fRec232[l231] = 0.0;
		}
		for (int l232 = 0; l232 < 3; l232 = l232 + 1) {
			fRec231[l232] = 0.0;
		}
		for (int l233 = 0; l233 < 3; l233 = l233 + 1) {
			fRec230[l233] = 0.0;
		}
		for (int l234 = 0; l234 < 3; l234 = l234 + 1) {
			fRec229[l234] = 0.0;
		}
		for (int l235 = 0; l235 < 3; l235 = l235 + 1) {
			fRec228[l235] = 0.0;
		}
		for (int l236 = 0; l236 < 3; l236 = l236 + 1) {
			fRec227[l236] = 0.0;
		}
		for (int l237 = 0; l237 < 3; l237 = l237 + 1) {
			fRec226[l237] = 0.0;
		}
		for (int l238 = 0; l238 < 3; l238 = l238 + 1) {
			fRec225[l238] = 0.0;
		}
		for (int l239 = 0; l239 < 3; l239 = l239 + 1) {
			fRec224[l239] = 0.0;
		}
		for (int l240 = 0; l240 < 3; l240 = l240 + 1) {
			fRec248[l240] = 0.0;
		}
		for (int l241 = 0; l241 < 3; l241 = l241 + 1) {
			fRec247[l241] = 0.0;
		}
		for (int l242 = 0; l242 < 3; l242 = l242 + 1) {
			fRec246[l242] = 0.0;
		}
		for (int l243 = 0; l243 < 3; l243 = l243 + 1) {
			fRec245[l243] = 0.0;
		}
		for (int l244 = 0; l244 < 3; l244 = l244 + 1) {
			fRec244[l244] = 0.0;
		}
		for (int l245 = 0; l245 < 3; l245 = l245 + 1) {
			fRec243[l245] = 0.0;
		}
		for (int l246 = 0; l246 < 3; l246 = l246 + 1) {
			fRec242[l246] = 0.0;
		}
		for (int l247 = 0; l247 < 3; l247 = l247 + 1) {
			fRec241[l247] = 0.0;
		}
		for (int l248 = 0; l248 < 3; l248 = l248 + 1) {
			fRec240[l248] = 0.0;
		}
		for (int l249 = 0; l249 < 3; l249 = l249 + 1) {
			fRec239[l249] = 0.0;
		}
		for (int l250 = 0; l250 < 3; l250 = l250 + 1) {
			fRec238[l250] = 0.0;
		}
		for (int l251 = 0; l251 < 3; l251 = l251 + 1) {
			fRec237[l251] = 0.0;
		}
		for (int l252 = 0; l252 < 2; l252 = l252 + 1) {
			fRec236[l252] = 0.0;
		}
		for (int l253 = 0; l253 < 3; l253 = l253 + 1) {
			fRec260[l253] = 0.0;
		}
		for (int l254 = 0; l254 < 3; l254 = l254 + 1) {
			fRec259[l254] = 0.0;
		}
		for (int l255 = 0; l255 < 3; l255 = l255 + 1) {
			fRec258[l255] = 0.0;
		}
		for (int l256 = 0; l256 < 3; l256 = l256 + 1) {
			fRec257[l256] = 0.0;
		}
		for (int l257 = 0; l257 < 3; l257 = l257 + 1) {
			fRec256[l257] = 0.0;
		}
		for (int l258 = 0; l258 < 3; l258 = l258 + 1) {
			fRec255[l258] = 0.0;
		}
		for (int l259 = 0; l259 < 3; l259 = l259 + 1) {
			fRec254[l259] = 0.0;
		}
		for (int l260 = 0; l260 < 3; l260 = l260 + 1) {
			fRec253[l260] = 0.0;
		}
		for (int l261 = 0; l261 < 3; l261 = l261 + 1) {
			fRec252[l261] = 0.0;
		}
		for (int l262 = 0; l262 < 3; l262 = l262 + 1) {
			fRec251[l262] = 0.0;
		}
		for (int l263 = 0; l263 < 3; l263 = l263 + 1) {
			fRec250[l263] = 0.0;
		}
		for (int l264 = 0; l264 < 3; l264 = l264 + 1) {
			fRec249[l264] = 0.0;
		}
		for (int l265 = 0; l265 < 3; l265 = l265 + 1) {
			fRec273[l265] = 0.0;
		}
		for (int l266 = 0; l266 < 3; l266 = l266 + 1) {
			fRec272[l266] = 0.0;
		}
		for (int l267 = 0; l267 < 3; l267 = l267 + 1) {
			fRec271[l267] = 0.0;
		}
		for (int l268 = 0; l268 < 3; l268 = l268 + 1) {
			fRec270[l268] = 0.0;
		}
		for (int l269 = 0; l269 < 3; l269 = l269 + 1) {
			fRec269[l269] = 0.0;
		}
		for (int l270 = 0; l270 < 3; l270 = l270 + 1) {
			fRec268[l270] = 0.0;
		}
		for (int l271 = 0; l271 < 3; l271 = l271 + 1) {
			fRec267[l271] = 0.0;
		}
		for (int l272 = 0; l272 < 3; l272 = l272 + 1) {
			fRec266[l272] = 0.0;
		}
		for (int l273 = 0; l273 < 3; l273 = l273 + 1) {
			fRec265[l273] = 0.0;
		}
		for (int l274 = 0; l274 < 3; l274 = l274 + 1) {
			fRec264[l274] = 0.0;
		}
		for (int l275 = 0; l275 < 3; l275 = l275 + 1) {
			fRec263[l275] = 0.0;
		}
		for (int l276 = 0; l276 < 3; l276 = l276 + 1) {
			fRec262[l276] = 0.0;
		}
		for (int l277 = 0; l277 < 2; l277 = l277 + 1) {
			fRec261[l277] = 0.0;
		}
		for (int l278 = 0; l278 < 3; l278 = l278 + 1) {
			fRec285[l278] = 0.0;
		}
		for (int l279 = 0; l279 < 3; l279 = l279 + 1) {
			fRec284[l279] = 0.0;
		}
		for (int l280 = 0; l280 < 3; l280 = l280 + 1) {
			fRec283[l280] = 0.0;
		}
		for (int l281 = 0; l281 < 3; l281 = l281 + 1) {
			fRec282[l281] = 0.0;
		}
		for (int l282 = 0; l282 < 3; l282 = l282 + 1) {
			fRec281[l282] = 0.0;
		}
		for (int l283 = 0; l283 < 3; l283 = l283 + 1) {
			fRec280[l283] = 0.0;
		}
		for (int l284 = 0; l284 < 3; l284 = l284 + 1) {
			fRec279[l284] = 0.0;
		}
		for (int l285 = 0; l285 < 3; l285 = l285 + 1) {
			fRec278[l285] = 0.0;
		}
		for (int l286 = 0; l286 < 3; l286 = l286 + 1) {
			fRec277[l286] = 0.0;
		}
		for (int l287 = 0; l287 < 3; l287 = l287 + 1) {
			fRec276[l287] = 0.0;
		}
		for (int l288 = 0; l288 < 3; l288 = l288 + 1) {
			fRec275[l288] = 0.0;
		}
		for (int l289 = 0; l289 < 3; l289 = l289 + 1) {
			fRec274[l289] = 0.0;
		}
		for (int l290 = 0; l290 < 3; l290 = l290 + 1) {
			fRec298[l290] = 0.0;
		}
		for (int l291 = 0; l291 < 3; l291 = l291 + 1) {
			fRec297[l291] = 0.0;
		}
		for (int l292 = 0; l292 < 3; l292 = l292 + 1) {
			fRec296[l292] = 0.0;
		}
		for (int l293 = 0; l293 < 3; l293 = l293 + 1) {
			fRec295[l293] = 0.0;
		}
		for (int l294 = 0; l294 < 3; l294 = l294 + 1) {
			fRec294[l294] = 0.0;
		}
		for (int l295 = 0; l295 < 3; l295 = l295 + 1) {
			fRec293[l295] = 0.0;
		}
		for (int l296 = 0; l296 < 3; l296 = l296 + 1) {
			fRec292[l296] = 0.0;
		}
		for (int l297 = 0; l297 < 3; l297 = l297 + 1) {
			fRec291[l297] = 0.0;
		}
		for (int l298 = 0; l298 < 3; l298 = l298 + 1) {
			fRec290[l298] = 0.0;
		}
		for (int l299 = 0; l299 < 3; l299 = l299 + 1) {
			fRec289[l299] = 0.0;
		}
		for (int l300 = 0; l300 < 3; l300 = l300 + 1) {
			fRec288[l300] = 0.0;
		}
		for (int l301 = 0; l301 < 3; l301 = l301 + 1) {
			fRec287[l301] = 0.0;
		}
		for (int l302 = 0; l302 < 2; l302 = l302 + 1) {
			fRec286[l302] = 0.0;
		}
		for (int l303 = 0; l303 < 3; l303 = l303 + 1) {
			fRec310[l303] = 0.0;
		}
		for (int l304 = 0; l304 < 3; l304 = l304 + 1) {
			fRec309[l304] = 0.0;
		}
		for (int l305 = 0; l305 < 3; l305 = l305 + 1) {
			fRec308[l305] = 0.0;
		}
		for (int l306 = 0; l306 < 3; l306 = l306 + 1) {
			fRec307[l306] = 0.0;
		}
		for (int l307 = 0; l307 < 3; l307 = l307 + 1) {
			fRec306[l307] = 0.0;
		}
		for (int l308 = 0; l308 < 3; l308 = l308 + 1) {
			fRec305[l308] = 0.0;
		}
		for (int l309 = 0; l309 < 3; l309 = l309 + 1) {
			fRec304[l309] = 0.0;
		}
		for (int l310 = 0; l310 < 3; l310 = l310 + 1) {
			fRec303[l310] = 0.0;
		}
		for (int l311 = 0; l311 < 3; l311 = l311 + 1) {
			fRec302[l311] = 0.0;
		}
		for (int l312 = 0; l312 < 3; l312 = l312 + 1) {
			fRec301[l312] = 0.0;
		}
		for (int l313 = 0; l313 < 3; l313 = l313 + 1) {
			fRec300[l313] = 0.0;
		}
		for (int l314 = 0; l314 < 3; l314 = l314 + 1) {
			fRec299[l314] = 0.0;
		}
		for (int l315 = 0; l315 < 3; l315 = l315 + 1) {
			fRec323[l315] = 0.0;
		}
		for (int l316 = 0; l316 < 3; l316 = l316 + 1) {
			fRec322[l316] = 0.0;
		}
		for (int l317 = 0; l317 < 3; l317 = l317 + 1) {
			fRec321[l317] = 0.0;
		}
		for (int l318 = 0; l318 < 3; l318 = l318 + 1) {
			fRec320[l318] = 0.0;
		}
		for (int l319 = 0; l319 < 3; l319 = l319 + 1) {
			fRec319[l319] = 0.0;
		}
		for (int l320 = 0; l320 < 3; l320 = l320 + 1) {
			fRec318[l320] = 0.0;
		}
		for (int l321 = 0; l321 < 3; l321 = l321 + 1) {
			fRec317[l321] = 0.0;
		}
		for (int l322 = 0; l322 < 3; l322 = l322 + 1) {
			fRec316[l322] = 0.0;
		}
		for (int l323 = 0; l323 < 3; l323 = l323 + 1) {
			fRec315[l323] = 0.0;
		}
		for (int l324 = 0; l324 < 3; l324 = l324 + 1) {
			fRec314[l324] = 0.0;
		}
		for (int l325 = 0; l325 < 3; l325 = l325 + 1) {
			fRec313[l325] = 0.0;
		}
		for (int l326 = 0; l326 < 3; l326 = l326 + 1) {
			fRec312[l326] = 0.0;
		}
		for (int l327 = 0; l327 < 2; l327 = l327 + 1) {
			fRec311[l327] = 0.0;
		}
		for (int l328 = 0; l328 < 3; l328 = l328 + 1) {
			fRec335[l328] = 0.0;
		}
		for (int l329 = 0; l329 < 3; l329 = l329 + 1) {
			fRec334[l329] = 0.0;
		}
		for (int l330 = 0; l330 < 3; l330 = l330 + 1) {
			fRec333[l330] = 0.0;
		}
		for (int l331 = 0; l331 < 3; l331 = l331 + 1) {
			fRec332[l331] = 0.0;
		}
		for (int l332 = 0; l332 < 3; l332 = l332 + 1) {
			fRec331[l332] = 0.0;
		}
		for (int l333 = 0; l333 < 3; l333 = l333 + 1) {
			fRec330[l333] = 0.0;
		}
		for (int l334 = 0; l334 < 3; l334 = l334 + 1) {
			fRec329[l334] = 0.0;
		}
		for (int l335 = 0; l335 < 3; l335 = l335 + 1) {
			fRec328[l335] = 0.0;
		}
		for (int l336 = 0; l336 < 3; l336 = l336 + 1) {
			fRec327[l336] = 0.0;
		}
		for (int l337 = 0; l337 < 3; l337 = l337 + 1) {
			fRec326[l337] = 0.0;
		}
		for (int l338 = 0; l338 < 3; l338 = l338 + 1) {
			fRec325[l338] = 0.0;
		}
		for (int l339 = 0; l339 < 3; l339 = l339 + 1) {
			fRec324[l339] = 0.0;
		}
		for (int l340 = 0; l340 < 3; l340 = l340 + 1) {
			fRec348[l340] = 0.0;
		}
		for (int l341 = 0; l341 < 3; l341 = l341 + 1) {
			fRec347[l341] = 0.0;
		}
		for (int l342 = 0; l342 < 3; l342 = l342 + 1) {
			fRec346[l342] = 0.0;
		}
		for (int l343 = 0; l343 < 3; l343 = l343 + 1) {
			fRec345[l343] = 0.0;
		}
		for (int l344 = 0; l344 < 3; l344 = l344 + 1) {
			fRec344[l344] = 0.0;
		}
		for (int l345 = 0; l345 < 3; l345 = l345 + 1) {
			fRec343[l345] = 0.0;
		}
		for (int l346 = 0; l346 < 3; l346 = l346 + 1) {
			fRec342[l346] = 0.0;
		}
		for (int l347 = 0; l347 < 3; l347 = l347 + 1) {
			fRec341[l347] = 0.0;
		}
		for (int l348 = 0; l348 < 3; l348 = l348 + 1) {
			fRec340[l348] = 0.0;
		}
		for (int l349 = 0; l349 < 3; l349 = l349 + 1) {
			fRec339[l349] = 0.0;
		}
		for (int l350 = 0; l350 < 3; l350 = l350 + 1) {
			fRec338[l350] = 0.0;
		}
		for (int l351 = 0; l351 < 3; l351 = l351 + 1) {
			fRec337[l351] = 0.0;
		}
		for (int l352 = 0; l352 < 2; l352 = l352 + 1) {
			fRec336[l352] = 0.0;
		}
		for (int l353 = 0; l353 < 3; l353 = l353 + 1) {
			fRec360[l353] = 0.0;
		}
		for (int l354 = 0; l354 < 3; l354 = l354 + 1) {
			fRec359[l354] = 0.0;
		}
		for (int l355 = 0; l355 < 3; l355 = l355 + 1) {
			fRec358[l355] = 0.0;
		}
		for (int l356 = 0; l356 < 3; l356 = l356 + 1) {
			fRec357[l356] = 0.0;
		}
		for (int l357 = 0; l357 < 3; l357 = l357 + 1) {
			fRec356[l357] = 0.0;
		}
		for (int l358 = 0; l358 < 3; l358 = l358 + 1) {
			fRec355[l358] = 0.0;
		}
		for (int l359 = 0; l359 < 3; l359 = l359 + 1) {
			fRec354[l359] = 0.0;
		}
		for (int l360 = 0; l360 < 3; l360 = l360 + 1) {
			fRec353[l360] = 0.0;
		}
		for (int l361 = 0; l361 < 3; l361 = l361 + 1) {
			fRec352[l361] = 0.0;
		}
		for (int l362 = 0; l362 < 3; l362 = l362 + 1) {
			fRec351[l362] = 0.0;
		}
		for (int l363 = 0; l363 < 3; l363 = l363 + 1) {
			fRec350[l363] = 0.0;
		}
		for (int l364 = 0; l364 < 3; l364 = l364 + 1) {
			fRec349[l364] = 0.0;
		}
		for (int l365 = 0; l365 < 3; l365 = l365 + 1) {
			fRec373[l365] = 0.0;
		}
		for (int l366 = 0; l366 < 3; l366 = l366 + 1) {
			fRec372[l366] = 0.0;
		}
		for (int l367 = 0; l367 < 3; l367 = l367 + 1) {
			fRec371[l367] = 0.0;
		}
		for (int l368 = 0; l368 < 3; l368 = l368 + 1) {
			fRec370[l368] = 0.0;
		}
		for (int l369 = 0; l369 < 3; l369 = l369 + 1) {
			fRec369[l369] = 0.0;
		}
		for (int l370 = 0; l370 < 3; l370 = l370 + 1) {
			fRec368[l370] = 0.0;
		}
		for (int l371 = 0; l371 < 3; l371 = l371 + 1) {
			fRec367[l371] = 0.0;
		}
		for (int l372 = 0; l372 < 3; l372 = l372 + 1) {
			fRec366[l372] = 0.0;
		}
		for (int l373 = 0; l373 < 3; l373 = l373 + 1) {
			fRec365[l373] = 0.0;
		}
		for (int l374 = 0; l374 < 3; l374 = l374 + 1) {
			fRec364[l374] = 0.0;
		}
		for (int l375 = 0; l375 < 3; l375 = l375 + 1) {
			fRec363[l375] = 0.0;
		}
		for (int l376 = 0; l376 < 3; l376 = l376 + 1) {
			fRec362[l376] = 0.0;
		}
		for (int l377 = 0; l377 < 2; l377 = l377 + 1) {
			fRec361[l377] = 0.0;
		}
		for (int l378 = 0; l378 < 3; l378 = l378 + 1) {
			fRec385[l378] = 0.0;
		}
		for (int l379 = 0; l379 < 3; l379 = l379 + 1) {
			fRec384[l379] = 0.0;
		}
		for (int l380 = 0; l380 < 3; l380 = l380 + 1) {
			fRec383[l380] = 0.0;
		}
		for (int l381 = 0; l381 < 3; l381 = l381 + 1) {
			fRec382[l381] = 0.0;
		}
		for (int l382 = 0; l382 < 3; l382 = l382 + 1) {
			fRec381[l382] = 0.0;
		}
		for (int l383 = 0; l383 < 3; l383 = l383 + 1) {
			fRec380[l383] = 0.0;
		}
		for (int l384 = 0; l384 < 3; l384 = l384 + 1) {
			fRec379[l384] = 0.0;
		}
		for (int l385 = 0; l385 < 3; l385 = l385 + 1) {
			fRec378[l385] = 0.0;
		}
		for (int l386 = 0; l386 < 3; l386 = l386 + 1) {
			fRec377[l386] = 0.0;
		}
		for (int l387 = 0; l387 < 3; l387 = l387 + 1) {
			fRec376[l387] = 0.0;
		}
		for (int l388 = 0; l388 < 3; l388 = l388 + 1) {
			fRec375[l388] = 0.0;
		}
		for (int l389 = 0; l389 < 3; l389 = l389 + 1) {
			fRec374[l389] = 0.0;
		}
		for (int l390 = 0; l390 < 3; l390 = l390 + 1) {
			fRec398[l390] = 0.0;
		}
		for (int l391 = 0; l391 < 3; l391 = l391 + 1) {
			fRec397[l391] = 0.0;
		}
		for (int l392 = 0; l392 < 3; l392 = l392 + 1) {
			fRec396[l392] = 0.0;
		}
		for (int l393 = 0; l393 < 3; l393 = l393 + 1) {
			fRec395[l393] = 0.0;
		}
		for (int l394 = 0; l394 < 3; l394 = l394 + 1) {
			fRec394[l394] = 0.0;
		}
		for (int l395 = 0; l395 < 3; l395 = l395 + 1) {
			fRec393[l395] = 0.0;
		}
		for (int l396 = 0; l396 < 3; l396 = l396 + 1) {
			fRec392[l396] = 0.0;
		}
		for (int l397 = 0; l397 < 3; l397 = l397 + 1) {
			fRec391[l397] = 0.0;
		}
		for (int l398 = 0; l398 < 3; l398 = l398 + 1) {
			fRec390[l398] = 0.0;
		}
		for (int l399 = 0; l399 < 3; l399 = l399 + 1) {
			fRec389[l399] = 0.0;
		}
		for (int l400 = 0; l400 < 3; l400 = l400 + 1) {
			fRec388[l400] = 0.0;
		}
		for (int l401 = 0; l401 < 3; l401 = l401 + 1) {
			fRec387[l401] = 0.0;
		}
		for (int l402 = 0; l402 < 2; l402 = l402 + 1) {
			fRec386[l402] = 0.0;
		}
		for (int l403 = 0; l403 < 3; l403 = l403 + 1) {
			fRec410[l403] = 0.0;
		}
		for (int l404 = 0; l404 < 3; l404 = l404 + 1) {
			fRec409[l404] = 0.0;
		}
		for (int l405 = 0; l405 < 3; l405 = l405 + 1) {
			fRec408[l405] = 0.0;
		}
		for (int l406 = 0; l406 < 3; l406 = l406 + 1) {
			fRec407[l406] = 0.0;
		}
		for (int l407 = 0; l407 < 3; l407 = l407 + 1) {
			fRec406[l407] = 0.0;
		}
		for (int l408 = 0; l408 < 3; l408 = l408 + 1) {
			fRec405[l408] = 0.0;
		}
		for (int l409 = 0; l409 < 3; l409 = l409 + 1) {
			fRec404[l409] = 0.0;
		}
		for (int l410 = 0; l410 < 3; l410 = l410 + 1) {
			fRec403[l410] = 0.0;
		}
		for (int l411 = 0; l411 < 3; l411 = l411 + 1) {
			fRec402[l411] = 0.0;
		}
		for (int l412 = 0; l412 < 3; l412 = l412 + 1) {
			fRec401[l412] = 0.0;
		}
		for (int l413 = 0; l413 < 3; l413 = l413 + 1) {
			fRec400[l413] = 0.0;
		}
		for (int l414 = 0; l414 < 3; l414 = l414 + 1) {
			fRec399[l414] = 0.0;
		}
		for (int l415 = 0; l415 < 3; l415 = l415 + 1) {
			fRec423[l415] = 0.0;
		}
		for (int l416 = 0; l416 < 3; l416 = l416 + 1) {
			fRec422[l416] = 0.0;
		}
		for (int l417 = 0; l417 < 3; l417 = l417 + 1) {
			fRec421[l417] = 0.0;
		}
		for (int l418 = 0; l418 < 3; l418 = l418 + 1) {
			fRec420[l418] = 0.0;
		}
		for (int l419 = 0; l419 < 3; l419 = l419 + 1) {
			fRec419[l419] = 0.0;
		}
		for (int l420 = 0; l420 < 3; l420 = l420 + 1) {
			fRec418[l420] = 0.0;
		}
		for (int l421 = 0; l421 < 3; l421 = l421 + 1) {
			fRec417[l421] = 0.0;
		}
		for (int l422 = 0; l422 < 3; l422 = l422 + 1) {
			fRec416[l422] = 0.0;
		}
		for (int l423 = 0; l423 < 3; l423 = l423 + 1) {
			fRec415[l423] = 0.0;
		}
		for (int l424 = 0; l424 < 3; l424 = l424 + 1) {
			fRec414[l424] = 0.0;
		}
		for (int l425 = 0; l425 < 3; l425 = l425 + 1) {
			fRec413[l425] = 0.0;
		}
		for (int l426 = 0; l426 < 3; l426 = l426 + 1) {
			fRec412[l426] = 0.0;
		}
		for (int l427 = 0; l427 < 2; l427 = l427 + 1) {
			fRec411[l427] = 0.0;
		}
		for (int l428 = 0; l428 < 3; l428 = l428 + 1) {
			fRec435[l428] = 0.0;
		}
		for (int l429 = 0; l429 < 3; l429 = l429 + 1) {
			fRec434[l429] = 0.0;
		}
		for (int l430 = 0; l430 < 3; l430 = l430 + 1) {
			fRec433[l430] = 0.0;
		}
		for (int l431 = 0; l431 < 3; l431 = l431 + 1) {
			fRec432[l431] = 0.0;
		}
		for (int l432 = 0; l432 < 3; l432 = l432 + 1) {
			fRec431[l432] = 0.0;
		}
		for (int l433 = 0; l433 < 3; l433 = l433 + 1) {
			fRec430[l433] = 0.0;
		}
		for (int l434 = 0; l434 < 3; l434 = l434 + 1) {
			fRec429[l434] = 0.0;
		}
		for (int l435 = 0; l435 < 3; l435 = l435 + 1) {
			fRec428[l435] = 0.0;
		}
		for (int l436 = 0; l436 < 3; l436 = l436 + 1) {
			fRec427[l436] = 0.0;
		}
		for (int l437 = 0; l437 < 3; l437 = l437 + 1) {
			fRec426[l437] = 0.0;
		}
		for (int l438 = 0; l438 < 3; l438 = l438 + 1) {
			fRec425[l438] = 0.0;
		}
		for (int l439 = 0; l439 < 3; l439 = l439 + 1) {
			fRec424[l439] = 0.0;
		}
		for (int l440 = 0; l440 < 3; l440 = l440 + 1) {
			fRec448[l440] = 0.0;
		}
		for (int l441 = 0; l441 < 3; l441 = l441 + 1) {
			fRec447[l441] = 0.0;
		}
		for (int l442 = 0; l442 < 3; l442 = l442 + 1) {
			fRec446[l442] = 0.0;
		}
		for (int l443 = 0; l443 < 3; l443 = l443 + 1) {
			fRec445[l443] = 0.0;
		}
		for (int l444 = 0; l444 < 3; l444 = l444 + 1) {
			fRec444[l444] = 0.0;
		}
		for (int l445 = 0; l445 < 3; l445 = l445 + 1) {
			fRec443[l445] = 0.0;
		}
		for (int l446 = 0; l446 < 3; l446 = l446 + 1) {
			fRec442[l446] = 0.0;
		}
		for (int l447 = 0; l447 < 3; l447 = l447 + 1) {
			fRec441[l447] = 0.0;
		}
		for (int l448 = 0; l448 < 3; l448 = l448 + 1) {
			fRec440[l448] = 0.0;
		}
		for (int l449 = 0; l449 < 3; l449 = l449 + 1) {
			fRec439[l449] = 0.0;
		}
		for (int l450 = 0; l450 < 3; l450 = l450 + 1) {
			fRec438[l450] = 0.0;
		}
		for (int l451 = 0; l451 < 3; l451 = l451 + 1) {
			fRec437[l451] = 0.0;
		}
		for (int l452 = 0; l452 < 2; l452 = l452 + 1) {
			fRec436[l452] = 0.0;
		}
		for (int l453 = 0; l453 < 3; l453 = l453 + 1) {
			fRec460[l453] = 0.0;
		}
		for (int l454 = 0; l454 < 3; l454 = l454 + 1) {
			fRec459[l454] = 0.0;
		}
		for (int l455 = 0; l455 < 3; l455 = l455 + 1) {
			fRec458[l455] = 0.0;
		}
		for (int l456 = 0; l456 < 3; l456 = l456 + 1) {
			fRec457[l456] = 0.0;
		}
		for (int l457 = 0; l457 < 3; l457 = l457 + 1) {
			fRec456[l457] = 0.0;
		}
		for (int l458 = 0; l458 < 3; l458 = l458 + 1) {
			fRec455[l458] = 0.0;
		}
		for (int l459 = 0; l459 < 3; l459 = l459 + 1) {
			fRec454[l459] = 0.0;
		}
		for (int l460 = 0; l460 < 3; l460 = l460 + 1) {
			fRec453[l460] = 0.0;
		}
		for (int l461 = 0; l461 < 3; l461 = l461 + 1) {
			fRec452[l461] = 0.0;
		}
		for (int l462 = 0; l462 < 3; l462 = l462 + 1) {
			fRec451[l462] = 0.0;
		}
		for (int l463 = 0; l463 < 3; l463 = l463 + 1) {
			fRec450[l463] = 0.0;
		}
		for (int l464 = 0; l464 < 3; l464 = l464 + 1) {
			fRec449[l464] = 0.0;
		}
		for (int l465 = 0; l465 < 3; l465 = l465 + 1) {
			fRec473[l465] = 0.0;
		}
		for (int l466 = 0; l466 < 3; l466 = l466 + 1) {
			fRec472[l466] = 0.0;
		}
		for (int l467 = 0; l467 < 3; l467 = l467 + 1) {
			fRec471[l467] = 0.0;
		}
		for (int l468 = 0; l468 < 3; l468 = l468 + 1) {
			fRec470[l468] = 0.0;
		}
		for (int l469 = 0; l469 < 3; l469 = l469 + 1) {
			fRec469[l469] = 0.0;
		}
		for (int l470 = 0; l470 < 3; l470 = l470 + 1) {
			fRec468[l470] = 0.0;
		}
		for (int l471 = 0; l471 < 3; l471 = l471 + 1) {
			fRec467[l471] = 0.0;
		}
		for (int l472 = 0; l472 < 3; l472 = l472 + 1) {
			fRec466[l472] = 0.0;
		}
		for (int l473 = 0; l473 < 3; l473 = l473 + 1) {
			fRec465[l473] = 0.0;
		}
		for (int l474 = 0; l474 < 3; l474 = l474 + 1) {
			fRec464[l474] = 0.0;
		}
		for (int l475 = 0; l475 < 3; l475 = l475 + 1) {
			fRec463[l475] = 0.0;
		}
		for (int l476 = 0; l476 < 3; l476 = l476 + 1) {
			fRec462[l476] = 0.0;
		}
		for (int l477 = 0; l477 < 2; l477 = l477 + 1) {
			fRec461[l477] = 0.0;
		}
		for (int l478 = 0; l478 < 3; l478 = l478 + 1) {
			fRec485[l478] = 0.0;
		}
		for (int l479 = 0; l479 < 3; l479 = l479 + 1) {
			fRec484[l479] = 0.0;
		}
		for (int l480 = 0; l480 < 3; l480 = l480 + 1) {
			fRec483[l480] = 0.0;
		}
		for (int l481 = 0; l481 < 3; l481 = l481 + 1) {
			fRec482[l481] = 0.0;
		}
		for (int l482 = 0; l482 < 3; l482 = l482 + 1) {
			fRec481[l482] = 0.0;
		}
		for (int l483 = 0; l483 < 3; l483 = l483 + 1) {
			fRec480[l483] = 0.0;
		}
		for (int l484 = 0; l484 < 3; l484 = l484 + 1) {
			fRec479[l484] = 0.0;
		}
		for (int l485 = 0; l485 < 3; l485 = l485 + 1) {
			fRec478[l485] = 0.0;
		}
		for (int l486 = 0; l486 < 3; l486 = l486 + 1) {
			fRec477[l486] = 0.0;
		}
		for (int l487 = 0; l487 < 3; l487 = l487 + 1) {
			fRec476[l487] = 0.0;
		}
		for (int l488 = 0; l488 < 3; l488 = l488 + 1) {
			fRec475[l488] = 0.0;
		}
		for (int l489 = 0; l489 < 3; l489 = l489 + 1) {
			fRec474[l489] = 0.0;
		}
		for (int l490 = 0; l490 < 3; l490 = l490 + 1) {
			fRec498[l490] = 0.0;
		}
		for (int l491 = 0; l491 < 3; l491 = l491 + 1) {
			fRec497[l491] = 0.0;
		}
		for (int l492 = 0; l492 < 3; l492 = l492 + 1) {
			fRec496[l492] = 0.0;
		}
		for (int l493 = 0; l493 < 3; l493 = l493 + 1) {
			fRec495[l493] = 0.0;
		}
		for (int l494 = 0; l494 < 3; l494 = l494 + 1) {
			fRec494[l494] = 0.0;
		}
		for (int l495 = 0; l495 < 3; l495 = l495 + 1) {
			fRec493[l495] = 0.0;
		}
		for (int l496 = 0; l496 < 3; l496 = l496 + 1) {
			fRec492[l496] = 0.0;
		}
		for (int l497 = 0; l497 < 3; l497 = l497 + 1) {
			fRec491[l497] = 0.0;
		}
		for (int l498 = 0; l498 < 3; l498 = l498 + 1) {
			fRec490[l498] = 0.0;
		}
		for (int l499 = 0; l499 < 3; l499 = l499 + 1) {
			fRec489[l499] = 0.0;
		}
		for (int l500 = 0; l500 < 3; l500 = l500 + 1) {
			fRec488[l500] = 0.0;
		}
		for (int l501 = 0; l501 < 3; l501 = l501 + 1) {
			fRec487[l501] = 0.0;
		}
		for (int l502 = 0; l502 < 2; l502 = l502 + 1) {
			fRec486[l502] = 0.0;
		}
		for (int l503 = 0; l503 < 3; l503 = l503 + 1) {
			fRec510[l503] = 0.0;
		}
		for (int l504 = 0; l504 < 3; l504 = l504 + 1) {
			fRec509[l504] = 0.0;
		}
		for (int l505 = 0; l505 < 3; l505 = l505 + 1) {
			fRec508[l505] = 0.0;
		}
		for (int l506 = 0; l506 < 3; l506 = l506 + 1) {
			fRec507[l506] = 0.0;
		}
		for (int l507 = 0; l507 < 3; l507 = l507 + 1) {
			fRec506[l507] = 0.0;
		}
		for (int l508 = 0; l508 < 3; l508 = l508 + 1) {
			fRec505[l508] = 0.0;
		}
		for (int l509 = 0; l509 < 3; l509 = l509 + 1) {
			fRec504[l509] = 0.0;
		}
		for (int l510 = 0; l510 < 3; l510 = l510 + 1) {
			fRec503[l510] = 0.0;
		}
		for (int l511 = 0; l511 < 3; l511 = l511 + 1) {
			fRec502[l511] = 0.0;
		}
		for (int l512 = 0; l512 < 3; l512 = l512 + 1) {
			fRec501[l512] = 0.0;
		}
		for (int l513 = 0; l513 < 3; l513 = l513 + 1) {
			fRec500[l513] = 0.0;
		}
		for (int l514 = 0; l514 < 3; l514 = l514 + 1) {
			fRec499[l514] = 0.0;
		}
		for (int l515 = 0; l515 < 3; l515 = l515 + 1) {
			fRec523[l515] = 0.0;
		}
		for (int l516 = 0; l516 < 3; l516 = l516 + 1) {
			fRec522[l516] = 0.0;
		}
		for (int l517 = 0; l517 < 3; l517 = l517 + 1) {
			fRec521[l517] = 0.0;
		}
		for (int l518 = 0; l518 < 3; l518 = l518 + 1) {
			fRec520[l518] = 0.0;
		}
		for (int l519 = 0; l519 < 3; l519 = l519 + 1) {
			fRec519[l519] = 0.0;
		}
		for (int l520 = 0; l520 < 3; l520 = l520 + 1) {
			fRec518[l520] = 0.0;
		}
		for (int l521 = 0; l521 < 3; l521 = l521 + 1) {
			fRec517[l521] = 0.0;
		}
		for (int l522 = 0; l522 < 3; l522 = l522 + 1) {
			fRec516[l522] = 0.0;
		}
		for (int l523 = 0; l523 < 3; l523 = l523 + 1) {
			fRec515[l523] = 0.0;
		}
		for (int l524 = 0; l524 < 3; l524 = l524 + 1) {
			fRec514[l524] = 0.0;
		}
		for (int l525 = 0; l525 < 3; l525 = l525 + 1) {
			fRec513[l525] = 0.0;
		}
		for (int l526 = 0; l526 < 3; l526 = l526 + 1) {
			fRec512[l526] = 0.0;
		}
		for (int l527 = 0; l527 < 2; l527 = l527 + 1) {
			fRec511[l527] = 0.0;
		}
		for (int l528 = 0; l528 < 3; l528 = l528 + 1) {
			fRec535[l528] = 0.0;
		}
		for (int l529 = 0; l529 < 3; l529 = l529 + 1) {
			fRec534[l529] = 0.0;
		}
		for (int l530 = 0; l530 < 3; l530 = l530 + 1) {
			fRec533[l530] = 0.0;
		}
		for (int l531 = 0; l531 < 3; l531 = l531 + 1) {
			fRec532[l531] = 0.0;
		}
		for (int l532 = 0; l532 < 3; l532 = l532 + 1) {
			fRec531[l532] = 0.0;
		}
		for (int l533 = 0; l533 < 3; l533 = l533 + 1) {
			fRec530[l533] = 0.0;
		}
		for (int l534 = 0; l534 < 3; l534 = l534 + 1) {
			fRec529[l534] = 0.0;
		}
		for (int l535 = 0; l535 < 3; l535 = l535 + 1) {
			fRec528[l535] = 0.0;
		}
		for (int l536 = 0; l536 < 3; l536 = l536 + 1) {
			fRec527[l536] = 0.0;
		}
		for (int l537 = 0; l537 < 3; l537 = l537 + 1) {
			fRec526[l537] = 0.0;
		}
		for (int l538 = 0; l538 < 3; l538 = l538 + 1) {
			fRec525[l538] = 0.0;
		}
		for (int l539 = 0; l539 < 3; l539 = l539 + 1) {
			fRec524[l539] = 0.0;
		}
		for (int l540 = 0; l540 < 3; l540 = l540 + 1) {
			fRec548[l540] = 0.0;
		}
		for (int l541 = 0; l541 < 3; l541 = l541 + 1) {
			fRec547[l541] = 0.0;
		}
		for (int l542 = 0; l542 < 3; l542 = l542 + 1) {
			fRec546[l542] = 0.0;
		}
		for (int l543 = 0; l543 < 3; l543 = l543 + 1) {
			fRec545[l543] = 0.0;
		}
		for (int l544 = 0; l544 < 3; l544 = l544 + 1) {
			fRec544[l544] = 0.0;
		}
		for (int l545 = 0; l545 < 3; l545 = l545 + 1) {
			fRec543[l545] = 0.0;
		}
		for (int l546 = 0; l546 < 3; l546 = l546 + 1) {
			fRec542[l546] = 0.0;
		}
		for (int l547 = 0; l547 < 3; l547 = l547 + 1) {
			fRec541[l547] = 0.0;
		}
		for (int l548 = 0; l548 < 3; l548 = l548 + 1) {
			fRec540[l548] = 0.0;
		}
		for (int l549 = 0; l549 < 3; l549 = l549 + 1) {
			fRec539[l549] = 0.0;
		}
		for (int l550 = 0; l550 < 3; l550 = l550 + 1) {
			fRec538[l550] = 0.0;
		}
		for (int l551 = 0; l551 < 3; l551 = l551 + 1) {
			fRec537[l551] = 0.0;
		}
		for (int l552 = 0; l552 < 2; l552 = l552 + 1) {
			fRec536[l552] = 0.0;
		}
		for (int l553 = 0; l553 < 3; l553 = l553 + 1) {
			fRec560[l553] = 0.0;
		}
		for (int l554 = 0; l554 < 3; l554 = l554 + 1) {
			fRec559[l554] = 0.0;
		}
		for (int l555 = 0; l555 < 3; l555 = l555 + 1) {
			fRec558[l555] = 0.0;
		}
		for (int l556 = 0; l556 < 3; l556 = l556 + 1) {
			fRec557[l556] = 0.0;
		}
		for (int l557 = 0; l557 < 3; l557 = l557 + 1) {
			fRec556[l557] = 0.0;
		}
		for (int l558 = 0; l558 < 3; l558 = l558 + 1) {
			fRec555[l558] = 0.0;
		}
		for (int l559 = 0; l559 < 3; l559 = l559 + 1) {
			fRec554[l559] = 0.0;
		}
		for (int l560 = 0; l560 < 3; l560 = l560 + 1) {
			fRec553[l560] = 0.0;
		}
		for (int l561 = 0; l561 < 3; l561 = l561 + 1) {
			fRec552[l561] = 0.0;
		}
		for (int l562 = 0; l562 < 3; l562 = l562 + 1) {
			fRec551[l562] = 0.0;
		}
		for (int l563 = 0; l563 < 3; l563 = l563 + 1) {
			fRec550[l563] = 0.0;
		}
		for (int l564 = 0; l564 < 3; l564 = l564 + 1) {
			fRec549[l564] = 0.0;
		}
		for (int l565 = 0; l565 < 3; l565 = l565 + 1) {
			fRec573[l565] = 0.0;
		}
		for (int l566 = 0; l566 < 3; l566 = l566 + 1) {
			fRec572[l566] = 0.0;
		}
		for (int l567 = 0; l567 < 3; l567 = l567 + 1) {
			fRec571[l567] = 0.0;
		}
		for (int l568 = 0; l568 < 3; l568 = l568 + 1) {
			fRec570[l568] = 0.0;
		}
		for (int l569 = 0; l569 < 3; l569 = l569 + 1) {
			fRec569[l569] = 0.0;
		}
		for (int l570 = 0; l570 < 3; l570 = l570 + 1) {
			fRec568[l570] = 0.0;
		}
		for (int l571 = 0; l571 < 3; l571 = l571 + 1) {
			fRec567[l571] = 0.0;
		}
		for (int l572 = 0; l572 < 3; l572 = l572 + 1) {
			fRec566[l572] = 0.0;
		}
		for (int l573 = 0; l573 < 3; l573 = l573 + 1) {
			fRec565[l573] = 0.0;
		}
		for (int l574 = 0; l574 < 3; l574 = l574 + 1) {
			fRec564[l574] = 0.0;
		}
		for (int l575 = 0; l575 < 3; l575 = l575 + 1) {
			fRec563[l575] = 0.0;
		}
		for (int l576 = 0; l576 < 3; l576 = l576 + 1) {
			fRec562[l576] = 0.0;
		}
		for (int l577 = 0; l577 < 2; l577 = l577 + 1) {
			fRec561[l577] = 0.0;
		}
		for (int l578 = 0; l578 < 3; l578 = l578 + 1) {
			fRec585[l578] = 0.0;
		}
		for (int l579 = 0; l579 < 3; l579 = l579 + 1) {
			fRec584[l579] = 0.0;
		}
		for (int l580 = 0; l580 < 3; l580 = l580 + 1) {
			fRec583[l580] = 0.0;
		}
		for (int l581 = 0; l581 < 3; l581 = l581 + 1) {
			fRec582[l581] = 0.0;
		}
		for (int l582 = 0; l582 < 3; l582 = l582 + 1) {
			fRec581[l582] = 0.0;
		}
		for (int l583 = 0; l583 < 3; l583 = l583 + 1) {
			fRec580[l583] = 0.0;
		}
		for (int l584 = 0; l584 < 3; l584 = l584 + 1) {
			fRec579[l584] = 0.0;
		}
		for (int l585 = 0; l585 < 3; l585 = l585 + 1) {
			fRec578[l585] = 0.0;
		}
		for (int l586 = 0; l586 < 3; l586 = l586 + 1) {
			fRec577[l586] = 0.0;
		}
		for (int l587 = 0; l587 < 3; l587 = l587 + 1) {
			fRec576[l587] = 0.0;
		}
		for (int l588 = 0; l588 < 3; l588 = l588 + 1) {
			fRec575[l588] = 0.0;
		}
		for (int l589 = 0; l589 < 3; l589 = l589 + 1) {
			fRec574[l589] = 0.0;
		}
		for (int l590 = 0; l590 < 3; l590 = l590 + 1) {
			fRec598[l590] = 0.0;
		}
		for (int l591 = 0; l591 < 3; l591 = l591 + 1) {
			fRec597[l591] = 0.0;
		}
		for (int l592 = 0; l592 < 3; l592 = l592 + 1) {
			fRec596[l592] = 0.0;
		}
		for (int l593 = 0; l593 < 3; l593 = l593 + 1) {
			fRec595[l593] = 0.0;
		}
		for (int l594 = 0; l594 < 3; l594 = l594 + 1) {
			fRec594[l594] = 0.0;
		}
		for (int l595 = 0; l595 < 3; l595 = l595 + 1) {
			fRec593[l595] = 0.0;
		}
		for (int l596 = 0; l596 < 3; l596 = l596 + 1) {
			fRec592[l596] = 0.0;
		}
		for (int l597 = 0; l597 < 3; l597 = l597 + 1) {
			fRec591[l597] = 0.0;
		}
		for (int l598 = 0; l598 < 3; l598 = l598 + 1) {
			fRec590[l598] = 0.0;
		}
		for (int l599 = 0; l599 < 3; l599 = l599 + 1) {
			fRec589[l599] = 0.0;
		}
		for (int l600 = 0; l600 < 3; l600 = l600 + 1) {
			fRec588[l600] = 0.0;
		}
		for (int l601 = 0; l601 < 3; l601 = l601 + 1) {
			fRec587[l601] = 0.0;
		}
		for (int l602 = 0; l602 < 2; l602 = l602 + 1) {
			fRec586[l602] = 0.0;
		}
		for (int l603 = 0; l603 < 3; l603 = l603 + 1) {
			fRec610[l603] = 0.0;
		}
		for (int l604 = 0; l604 < 3; l604 = l604 + 1) {
			fRec609[l604] = 0.0;
		}
		for (int l605 = 0; l605 < 3; l605 = l605 + 1) {
			fRec608[l605] = 0.0;
		}
		for (int l606 = 0; l606 < 3; l606 = l606 + 1) {
			fRec607[l606] = 0.0;
		}
		for (int l607 = 0; l607 < 3; l607 = l607 + 1) {
			fRec606[l607] = 0.0;
		}
		for (int l608 = 0; l608 < 3; l608 = l608 + 1) {
			fRec605[l608] = 0.0;
		}
		for (int l609 = 0; l609 < 3; l609 = l609 + 1) {
			fRec604[l609] = 0.0;
		}
		for (int l610 = 0; l610 < 3; l610 = l610 + 1) {
			fRec603[l610] = 0.0;
		}
		for (int l611 = 0; l611 < 3; l611 = l611 + 1) {
			fRec602[l611] = 0.0;
		}
		for (int l612 = 0; l612 < 3; l612 = l612 + 1) {
			fRec601[l612] = 0.0;
		}
		for (int l613 = 0; l613 < 3; l613 = l613 + 1) {
			fRec600[l613] = 0.0;
		}
		for (int l614 = 0; l614 < 3; l614 = l614 + 1) {
			fRec599[l614] = 0.0;
		}
		for (int l615 = 0; l615 < 3; l615 = l615 + 1) {
			fRec623[l615] = 0.0;
		}
		for (int l616 = 0; l616 < 3; l616 = l616 + 1) {
			fRec622[l616] = 0.0;
		}
		for (int l617 = 0; l617 < 3; l617 = l617 + 1) {
			fRec621[l617] = 0.0;
		}
		for (int l618 = 0; l618 < 3; l618 = l618 + 1) {
			fRec620[l618] = 0.0;
		}
		for (int l619 = 0; l619 < 3; l619 = l619 + 1) {
			fRec619[l619] = 0.0;
		}
		for (int l620 = 0; l620 < 3; l620 = l620 + 1) {
			fRec618[l620] = 0.0;
		}
		for (int l621 = 0; l621 < 3; l621 = l621 + 1) {
			fRec617[l621] = 0.0;
		}
		for (int l622 = 0; l622 < 3; l622 = l622 + 1) {
			fRec616[l622] = 0.0;
		}
		for (int l623 = 0; l623 < 3; l623 = l623 + 1) {
			fRec615[l623] = 0.0;
		}
		for (int l624 = 0; l624 < 3; l624 = l624 + 1) {
			fRec614[l624] = 0.0;
		}
		for (int l625 = 0; l625 < 3; l625 = l625 + 1) {
			fRec613[l625] = 0.0;
		}
		for (int l626 = 0; l626 < 3; l626 = l626 + 1) {
			fRec612[l626] = 0.0;
		}
		for (int l627 = 0; l627 < 2; l627 = l627 + 1) {
			fRec611[l627] = 0.0;
		}
		for (int l628 = 0; l628 < 3; l628 = l628 + 1) {
			fRec635[l628] = 0.0;
		}
		for (int l629 = 0; l629 < 3; l629 = l629 + 1) {
			fRec634[l629] = 0.0;
		}
		for (int l630 = 0; l630 < 3; l630 = l630 + 1) {
			fRec633[l630] = 0.0;
		}
		for (int l631 = 0; l631 < 3; l631 = l631 + 1) {
			fRec632[l631] = 0.0;
		}
		for (int l632 = 0; l632 < 3; l632 = l632 + 1) {
			fRec631[l632] = 0.0;
		}
		for (int l633 = 0; l633 < 3; l633 = l633 + 1) {
			fRec630[l633] = 0.0;
		}
		for (int l634 = 0; l634 < 3; l634 = l634 + 1) {
			fRec629[l634] = 0.0;
		}
		for (int l635 = 0; l635 < 3; l635 = l635 + 1) {
			fRec628[l635] = 0.0;
		}
		for (int l636 = 0; l636 < 3; l636 = l636 + 1) {
			fRec627[l636] = 0.0;
		}
		for (int l637 = 0; l637 < 3; l637 = l637 + 1) {
			fRec626[l637] = 0.0;
		}
		for (int l638 = 0; l638 < 3; l638 = l638 + 1) {
			fRec625[l638] = 0.0;
		}
		for (int l639 = 0; l639 < 3; l639 = l639 + 1) {
			fRec624[l639] = 0.0;
		}
		for (int l640 = 0; l640 < 3; l640 = l640 + 1) {
			fRec648[l640] = 0.0;
		}
		for (int l641 = 0; l641 < 3; l641 = l641 + 1) {
			fRec647[l641] = 0.0;
		}
		for (int l642 = 0; l642 < 3; l642 = l642 + 1) {
			fRec646[l642] = 0.0;
		}
		for (int l643 = 0; l643 < 3; l643 = l643 + 1) {
			fRec645[l643] = 0.0;
		}
		for (int l644 = 0; l644 < 3; l644 = l644 + 1) {
			fRec644[l644] = 0.0;
		}
		for (int l645 = 0; l645 < 3; l645 = l645 + 1) {
			fRec643[l645] = 0.0;
		}
		for (int l646 = 0; l646 < 3; l646 = l646 + 1) {
			fRec642[l646] = 0.0;
		}
		for (int l647 = 0; l647 < 3; l647 = l647 + 1) {
			fRec641[l647] = 0.0;
		}
		for (int l648 = 0; l648 < 3; l648 = l648 + 1) {
			fRec640[l648] = 0.0;
		}
		for (int l649 = 0; l649 < 3; l649 = l649 + 1) {
			fRec639[l649] = 0.0;
		}
		for (int l650 = 0; l650 < 3; l650 = l650 + 1) {
			fRec638[l650] = 0.0;
		}
		for (int l651 = 0; l651 < 3; l651 = l651 + 1) {
			fRec637[l651] = 0.0;
		}
		for (int l652 = 0; l652 < 2; l652 = l652 + 1) {
			fRec636[l652] = 0.0;
		}
		for (int l653 = 0; l653 < 3; l653 = l653 + 1) {
			fRec660[l653] = 0.0;
		}
		for (int l654 = 0; l654 < 3; l654 = l654 + 1) {
			fRec659[l654] = 0.0;
		}
		for (int l655 = 0; l655 < 3; l655 = l655 + 1) {
			fRec658[l655] = 0.0;
		}
		for (int l656 = 0; l656 < 3; l656 = l656 + 1) {
			fRec657[l656] = 0.0;
		}
		for (int l657 = 0; l657 < 3; l657 = l657 + 1) {
			fRec656[l657] = 0.0;
		}
		for (int l658 = 0; l658 < 3; l658 = l658 + 1) {
			fRec655[l658] = 0.0;
		}
		for (int l659 = 0; l659 < 3; l659 = l659 + 1) {
			fRec654[l659] = 0.0;
		}
		for (int l660 = 0; l660 < 3; l660 = l660 + 1) {
			fRec653[l660] = 0.0;
		}
		for (int l661 = 0; l661 < 3; l661 = l661 + 1) {
			fRec652[l661] = 0.0;
		}
		for (int l662 = 0; l662 < 3; l662 = l662 + 1) {
			fRec651[l662] = 0.0;
		}
		for (int l663 = 0; l663 < 3; l663 = l663 + 1) {
			fRec650[l663] = 0.0;
		}
		for (int l664 = 0; l664 < 3; l664 = l664 + 1) {
			fRec649[l664] = 0.0;
		}
		for (int l665 = 0; l665 < 3; l665 = l665 + 1) {
			fRec673[l665] = 0.0;
		}
		for (int l666 = 0; l666 < 3; l666 = l666 + 1) {
			fRec672[l666] = 0.0;
		}
		for (int l667 = 0; l667 < 3; l667 = l667 + 1) {
			fRec671[l667] = 0.0;
		}
		for (int l668 = 0; l668 < 3; l668 = l668 + 1) {
			fRec670[l668] = 0.0;
		}
		for (int l669 = 0; l669 < 3; l669 = l669 + 1) {
			fRec669[l669] = 0.0;
		}
		for (int l670 = 0; l670 < 3; l670 = l670 + 1) {
			fRec668[l670] = 0.0;
		}
		for (int l671 = 0; l671 < 3; l671 = l671 + 1) {
			fRec667[l671] = 0.0;
		}
		for (int l672 = 0; l672 < 3; l672 = l672 + 1) {
			fRec666[l672] = 0.0;
		}
		for (int l673 = 0; l673 < 3; l673 = l673 + 1) {
			fRec665[l673] = 0.0;
		}
		for (int l674 = 0; l674 < 3; l674 = l674 + 1) {
			fRec664[l674] = 0.0;
		}
		for (int l675 = 0; l675 < 3; l675 = l675 + 1) {
			fRec663[l675] = 0.0;
		}
		for (int l676 = 0; l676 < 3; l676 = l676 + 1) {
			fRec662[l676] = 0.0;
		}
		for (int l677 = 0; l677 < 2; l677 = l677 + 1) {
			fRec661[l677] = 0.0;
		}
		for (int l678 = 0; l678 < 3; l678 = l678 + 1) {
			fRec685[l678] = 0.0;
		}
		for (int l679 = 0; l679 < 3; l679 = l679 + 1) {
			fRec684[l679] = 0.0;
		}
		for (int l680 = 0; l680 < 3; l680 = l680 + 1) {
			fRec683[l680] = 0.0;
		}
		for (int l681 = 0; l681 < 3; l681 = l681 + 1) {
			fRec682[l681] = 0.0;
		}
		for (int l682 = 0; l682 < 3; l682 = l682 + 1) {
			fRec681[l682] = 0.0;
		}
		for (int l683 = 0; l683 < 3; l683 = l683 + 1) {
			fRec680[l683] = 0.0;
		}
		for (int l684 = 0; l684 < 3; l684 = l684 + 1) {
			fRec679[l684] = 0.0;
		}
		for (int l685 = 0; l685 < 3; l685 = l685 + 1) {
			fRec678[l685] = 0.0;
		}
		for (int l686 = 0; l686 < 3; l686 = l686 + 1) {
			fRec677[l686] = 0.0;
		}
		for (int l687 = 0; l687 < 3; l687 = l687 + 1) {
			fRec676[l687] = 0.0;
		}
		for (int l688 = 0; l688 < 3; l688 = l688 + 1) {
			fRec675[l688] = 0.0;
		}
		for (int l689 = 0; l689 < 3; l689 = l689 + 1) {
			fRec674[l689] = 0.0;
		}
		for (int l690 = 0; l690 < 3; l690 = l690 + 1) {
			fRec698[l690] = 0.0;
		}
		for (int l691 = 0; l691 < 3; l691 = l691 + 1) {
			fRec697[l691] = 0.0;
		}
		for (int l692 = 0; l692 < 3; l692 = l692 + 1) {
			fRec696[l692] = 0.0;
		}
		for (int l693 = 0; l693 < 3; l693 = l693 + 1) {
			fRec695[l693] = 0.0;
		}
		for (int l694 = 0; l694 < 3; l694 = l694 + 1) {
			fRec694[l694] = 0.0;
		}
		for (int l695 = 0; l695 < 3; l695 = l695 + 1) {
			fRec693[l695] = 0.0;
		}
		for (int l696 = 0; l696 < 3; l696 = l696 + 1) {
			fRec692[l696] = 0.0;
		}
		for (int l697 = 0; l697 < 3; l697 = l697 + 1) {
			fRec691[l697] = 0.0;
		}
		for (int l698 = 0; l698 < 3; l698 = l698 + 1) {
			fRec690[l698] = 0.0;
		}
		for (int l699 = 0; l699 < 3; l699 = l699 + 1) {
			fRec689[l699] = 0.0;
		}
		for (int l700 = 0; l700 < 3; l700 = l700 + 1) {
			fRec688[l700] = 0.0;
		}
		for (int l701 = 0; l701 < 3; l701 = l701 + 1) {
			fRec687[l701] = 0.0;
		}
		for (int l702 = 0; l702 < 2; l702 = l702 + 1) {
			fRec686[l702] = 0.0;
		}
		for (int l703 = 0; l703 < 3; l703 = l703 + 1) {
			fRec710[l703] = 0.0;
		}
		for (int l704 = 0; l704 < 3; l704 = l704 + 1) {
			fRec709[l704] = 0.0;
		}
		for (int l705 = 0; l705 < 3; l705 = l705 + 1) {
			fRec708[l705] = 0.0;
		}
		for (int l706 = 0; l706 < 3; l706 = l706 + 1) {
			fRec707[l706] = 0.0;
		}
		for (int l707 = 0; l707 < 3; l707 = l707 + 1) {
			fRec706[l707] = 0.0;
		}
		for (int l708 = 0; l708 < 3; l708 = l708 + 1) {
			fRec705[l708] = 0.0;
		}
		for (int l709 = 0; l709 < 3; l709 = l709 + 1) {
			fRec704[l709] = 0.0;
		}
		for (int l710 = 0; l710 < 3; l710 = l710 + 1) {
			fRec703[l710] = 0.0;
		}
		for (int l711 = 0; l711 < 3; l711 = l711 + 1) {
			fRec702[l711] = 0.0;
		}
		for (int l712 = 0; l712 < 3; l712 = l712 + 1) {
			fRec701[l712] = 0.0;
		}
		for (int l713 = 0; l713 < 3; l713 = l713 + 1) {
			fRec700[l713] = 0.0;
		}
		for (int l714 = 0; l714 < 3; l714 = l714 + 1) {
			fRec699[l714] = 0.0;
		}
		for (int l715 = 0; l715 < 3; l715 = l715 + 1) {
			fRec723[l715] = 0.0;
		}
		for (int l716 = 0; l716 < 3; l716 = l716 + 1) {
			fRec722[l716] = 0.0;
		}
		for (int l717 = 0; l717 < 3; l717 = l717 + 1) {
			fRec721[l717] = 0.0;
		}
		for (int l718 = 0; l718 < 3; l718 = l718 + 1) {
			fRec720[l718] = 0.0;
		}
		for (int l719 = 0; l719 < 3; l719 = l719 + 1) {
			fRec719[l719] = 0.0;
		}
		for (int l720 = 0; l720 < 3; l720 = l720 + 1) {
			fRec718[l720] = 0.0;
		}
		for (int l721 = 0; l721 < 3; l721 = l721 + 1) {
			fRec717[l721] = 0.0;
		}
		for (int l722 = 0; l722 < 3; l722 = l722 + 1) {
			fRec716[l722] = 0.0;
		}
		for (int l723 = 0; l723 < 3; l723 = l723 + 1) {
			fRec715[l723] = 0.0;
		}
		for (int l724 = 0; l724 < 3; l724 = l724 + 1) {
			fRec714[l724] = 0.0;
		}
		for (int l725 = 0; l725 < 3; l725 = l725 + 1) {
			fRec713[l725] = 0.0;
		}
		for (int l726 = 0; l726 < 3; l726 = l726 + 1) {
			fRec712[l726] = 0.0;
		}
		for (int l727 = 0; l727 < 2; l727 = l727 + 1) {
			fRec711[l727] = 0.0;
		}
		for (int l728 = 0; l728 < 3; l728 = l728 + 1) {
			fRec735[l728] = 0.0;
		}
		for (int l729 = 0; l729 < 3; l729 = l729 + 1) {
			fRec734[l729] = 0.0;
		}
		for (int l730 = 0; l730 < 3; l730 = l730 + 1) {
			fRec733[l730] = 0.0;
		}
		for (int l731 = 0; l731 < 3; l731 = l731 + 1) {
			fRec732[l731] = 0.0;
		}
		for (int l732 = 0; l732 < 3; l732 = l732 + 1) {
			fRec731[l732] = 0.0;
		}
		for (int l733 = 0; l733 < 3; l733 = l733 + 1) {
			fRec730[l733] = 0.0;
		}
		for (int l734 = 0; l734 < 3; l734 = l734 + 1) {
			fRec729[l734] = 0.0;
		}
		for (int l735 = 0; l735 < 3; l735 = l735 + 1) {
			fRec728[l735] = 0.0;
		}
		for (int l736 = 0; l736 < 3; l736 = l736 + 1) {
			fRec727[l736] = 0.0;
		}
		for (int l737 = 0; l737 < 3; l737 = l737 + 1) {
			fRec726[l737] = 0.0;
		}
		for (int l738 = 0; l738 < 3; l738 = l738 + 1) {
			fRec725[l738] = 0.0;
		}
		for (int l739 = 0; l739 < 3; l739 = l739 + 1) {
			fRec724[l739] = 0.0;
		}
		for (int l740 = 0; l740 < 3; l740 = l740 + 1) {
			fRec748[l740] = 0.0;
		}
		for (int l741 = 0; l741 < 3; l741 = l741 + 1) {
			fRec747[l741] = 0.0;
		}
		for (int l742 = 0; l742 < 3; l742 = l742 + 1) {
			fRec746[l742] = 0.0;
		}
		for (int l743 = 0; l743 < 3; l743 = l743 + 1) {
			fRec745[l743] = 0.0;
		}
		for (int l744 = 0; l744 < 3; l744 = l744 + 1) {
			fRec744[l744] = 0.0;
		}
		for (int l745 = 0; l745 < 3; l745 = l745 + 1) {
			fRec743[l745] = 0.0;
		}
		for (int l746 = 0; l746 < 3; l746 = l746 + 1) {
			fRec742[l746] = 0.0;
		}
		for (int l747 = 0; l747 < 3; l747 = l747 + 1) {
			fRec741[l747] = 0.0;
		}
		for (int l748 = 0; l748 < 3; l748 = l748 + 1) {
			fRec740[l748] = 0.0;
		}
		for (int l749 = 0; l749 < 3; l749 = l749 + 1) {
			fRec739[l749] = 0.0;
		}
		for (int l750 = 0; l750 < 3; l750 = l750 + 1) {
			fRec738[l750] = 0.0;
		}
		for (int l751 = 0; l751 < 3; l751 = l751 + 1) {
			fRec737[l751] = 0.0;
		}
		for (int l752 = 0; l752 < 2; l752 = l752 + 1) {
			fRec736[l752] = 0.0;
		}
		for (int l753 = 0; l753 < 3; l753 = l753 + 1) {
			fRec760[l753] = 0.0;
		}
		for (int l754 = 0; l754 < 3; l754 = l754 + 1) {
			fRec759[l754] = 0.0;
		}
		for (int l755 = 0; l755 < 3; l755 = l755 + 1) {
			fRec758[l755] = 0.0;
		}
		for (int l756 = 0; l756 < 3; l756 = l756 + 1) {
			fRec757[l756] = 0.0;
		}
		for (int l757 = 0; l757 < 3; l757 = l757 + 1) {
			fRec756[l757] = 0.0;
		}
		for (int l758 = 0; l758 < 3; l758 = l758 + 1) {
			fRec755[l758] = 0.0;
		}
		for (int l759 = 0; l759 < 3; l759 = l759 + 1) {
			fRec754[l759] = 0.0;
		}
		for (int l760 = 0; l760 < 3; l760 = l760 + 1) {
			fRec753[l760] = 0.0;
		}
		for (int l761 = 0; l761 < 3; l761 = l761 + 1) {
			fRec752[l761] = 0.0;
		}
		for (int l762 = 0; l762 < 3; l762 = l762 + 1) {
			fRec751[l762] = 0.0;
		}
		for (int l763 = 0; l763 < 3; l763 = l763 + 1) {
			fRec750[l763] = 0.0;
		}
		for (int l764 = 0; l764 < 3; l764 = l764 + 1) {
			fRec749[l764] = 0.0;
		}
		for (int l765 = 0; l765 < 3; l765 = l765 + 1) {
			fRec773[l765] = 0.0;
		}
		for (int l766 = 0; l766 < 3; l766 = l766 + 1) {
			fRec772[l766] = 0.0;
		}
		for (int l767 = 0; l767 < 3; l767 = l767 + 1) {
			fRec771[l767] = 0.0;
		}
		for (int l768 = 0; l768 < 3; l768 = l768 + 1) {
			fRec770[l768] = 0.0;
		}
		for (int l769 = 0; l769 < 3; l769 = l769 + 1) {
			fRec769[l769] = 0.0;
		}
		for (int l770 = 0; l770 < 3; l770 = l770 + 1) {
			fRec768[l770] = 0.0;
		}
		for (int l771 = 0; l771 < 3; l771 = l771 + 1) {
			fRec767[l771] = 0.0;
		}
		for (int l772 = 0; l772 < 3; l772 = l772 + 1) {
			fRec766[l772] = 0.0;
		}
		for (int l773 = 0; l773 < 3; l773 = l773 + 1) {
			fRec765[l773] = 0.0;
		}
		for (int l774 = 0; l774 < 3; l774 = l774 + 1) {
			fRec764[l774] = 0.0;
		}
		for (int l775 = 0; l775 < 3; l775 = l775 + 1) {
			fRec763[l775] = 0.0;
		}
		for (int l776 = 0; l776 < 3; l776 = l776 + 1) {
			fRec762[l776] = 0.0;
		}
		for (int l777 = 0; l777 < 2; l777 = l777 + 1) {
			fRec761[l777] = 0.0;
		}
		for (int l778 = 0; l778 < 3; l778 = l778 + 1) {
			fRec785[l778] = 0.0;
		}
		for (int l779 = 0; l779 < 3; l779 = l779 + 1) {
			fRec784[l779] = 0.0;
		}
		for (int l780 = 0; l780 < 3; l780 = l780 + 1) {
			fRec783[l780] = 0.0;
		}
		for (int l781 = 0; l781 < 3; l781 = l781 + 1) {
			fRec782[l781] = 0.0;
		}
		for (int l782 = 0; l782 < 3; l782 = l782 + 1) {
			fRec781[l782] = 0.0;
		}
		for (int l783 = 0; l783 < 3; l783 = l783 + 1) {
			fRec780[l783] = 0.0;
		}
		for (int l784 = 0; l784 < 3; l784 = l784 + 1) {
			fRec779[l784] = 0.0;
		}
		for (int l785 = 0; l785 < 3; l785 = l785 + 1) {
			fRec778[l785] = 0.0;
		}
		for (int l786 = 0; l786 < 3; l786 = l786 + 1) {
			fRec777[l786] = 0.0;
		}
		for (int l787 = 0; l787 < 3; l787 = l787 + 1) {
			fRec776[l787] = 0.0;
		}
		for (int l788 = 0; l788 < 3; l788 = l788 + 1) {
			fRec775[l788] = 0.0;
		}
		for (int l789 = 0; l789 < 3; l789 = l789 + 1) {
			fRec774[l789] = 0.0;
		}
		for (int l790 = 0; l790 < 3; l790 = l790 + 1) {
			fRec798[l790] = 0.0;
		}
		for (int l791 = 0; l791 < 3; l791 = l791 + 1) {
			fRec797[l791] = 0.0;
		}
		for (int l792 = 0; l792 < 3; l792 = l792 + 1) {
			fRec796[l792] = 0.0;
		}
		for (int l793 = 0; l793 < 3; l793 = l793 + 1) {
			fRec795[l793] = 0.0;
		}
		for (int l794 = 0; l794 < 3; l794 = l794 + 1) {
			fRec794[l794] = 0.0;
		}
		for (int l795 = 0; l795 < 3; l795 = l795 + 1) {
			fRec793[l795] = 0.0;
		}
		for (int l796 = 0; l796 < 3; l796 = l796 + 1) {
			fRec792[l796] = 0.0;
		}
		for (int l797 = 0; l797 < 3; l797 = l797 + 1) {
			fRec791[l797] = 0.0;
		}
		for (int l798 = 0; l798 < 3; l798 = l798 + 1) {
			fRec790[l798] = 0.0;
		}
		for (int l799 = 0; l799 < 3; l799 = l799 + 1) {
			fRec789[l799] = 0.0;
		}
		for (int l800 = 0; l800 < 3; l800 = l800 + 1) {
			fRec788[l800] = 0.0;
		}
		for (int l801 = 0; l801 < 3; l801 = l801 + 1) {
			fRec787[l801] = 0.0;
		}
		for (int l802 = 0; l802 < 2; l802 = l802 + 1) {
			fRec786[l802] = 0.0;
		}
		for (int l803 = 0; l803 < 3; l803 = l803 + 1) {
			fRec810[l803] = 0.0;
		}
		for (int l804 = 0; l804 < 3; l804 = l804 + 1) {
			fRec809[l804] = 0.0;
		}
		for (int l805 = 0; l805 < 3; l805 = l805 + 1) {
			fRec808[l805] = 0.0;
		}
		for (int l806 = 0; l806 < 3; l806 = l806 + 1) {
			fRec807[l806] = 0.0;
		}
		for (int l807 = 0; l807 < 3; l807 = l807 + 1) {
			fRec806[l807] = 0.0;
		}
		for (int l808 = 0; l808 < 3; l808 = l808 + 1) {
			fRec805[l808] = 0.0;
		}
		for (int l809 = 0; l809 < 3; l809 = l809 + 1) {
			fRec804[l809] = 0.0;
		}
		for (int l810 = 0; l810 < 3; l810 = l810 + 1) {
			fRec803[l810] = 0.0;
		}
		for (int l811 = 0; l811 < 3; l811 = l811 + 1) {
			fRec802[l811] = 0.0;
		}
		for (int l812 = 0; l812 < 3; l812 = l812 + 1) {
			fRec801[l812] = 0.0;
		}
		for (int l813 = 0; l813 < 3; l813 = l813 + 1) {
			fRec800[l813] = 0.0;
		}
		for (int l814 = 0; l814 < 3; l814 = l814 + 1) {
			fRec799[l814] = 0.0;
		}
		for (int l815 = 0; l815 < 3; l815 = l815 + 1) {
			fRec823[l815] = 0.0;
		}
		for (int l816 = 0; l816 < 3; l816 = l816 + 1) {
			fRec822[l816] = 0.0;
		}
		for (int l817 = 0; l817 < 3; l817 = l817 + 1) {
			fRec821[l817] = 0.0;
		}
		for (int l818 = 0; l818 < 3; l818 = l818 + 1) {
			fRec820[l818] = 0.0;
		}
		for (int l819 = 0; l819 < 3; l819 = l819 + 1) {
			fRec819[l819] = 0.0;
		}
		for (int l820 = 0; l820 < 3; l820 = l820 + 1) {
			fRec818[l820] = 0.0;
		}
		for (int l821 = 0; l821 < 3; l821 = l821 + 1) {
			fRec817[l821] = 0.0;
		}
		for (int l822 = 0; l822 < 3; l822 = l822 + 1) {
			fRec816[l822] = 0.0;
		}
		for (int l823 = 0; l823 < 3; l823 = l823 + 1) {
			fRec815[l823] = 0.0;
		}
		for (int l824 = 0; l824 < 3; l824 = l824 + 1) {
			fRec814[l824] = 0.0;
		}
		for (int l825 = 0; l825 < 3; l825 = l825 + 1) {
			fRec813[l825] = 0.0;
		}
		for (int l826 = 0; l826 < 3; l826 = l826 + 1) {
			fRec812[l826] = 0.0;
		}
		for (int l827 = 0; l827 < 2; l827 = l827 + 1) {
			fRec811[l827] = 0.0;
		}
		for (int l828 = 0; l828 < 3; l828 = l828 + 1) {
			fRec835[l828] = 0.0;
		}
		for (int l829 = 0; l829 < 3; l829 = l829 + 1) {
			fRec834[l829] = 0.0;
		}
		for (int l830 = 0; l830 < 3; l830 = l830 + 1) {
			fRec833[l830] = 0.0;
		}
		for (int l831 = 0; l831 < 3; l831 = l831 + 1) {
			fRec832[l831] = 0.0;
		}
		for (int l832 = 0; l832 < 3; l832 = l832 + 1) {
			fRec831[l832] = 0.0;
		}
		for (int l833 = 0; l833 < 3; l833 = l833 + 1) {
			fRec830[l833] = 0.0;
		}
		for (int l834 = 0; l834 < 3; l834 = l834 + 1) {
			fRec829[l834] = 0.0;
		}
		for (int l835 = 0; l835 < 3; l835 = l835 + 1) {
			fRec828[l835] = 0.0;
		}
		for (int l836 = 0; l836 < 3; l836 = l836 + 1) {
			fRec827[l836] = 0.0;
		}
		for (int l837 = 0; l837 < 3; l837 = l837 + 1) {
			fRec826[l837] = 0.0;
		}
		for (int l838 = 0; l838 < 3; l838 = l838 + 1) {
			fRec825[l838] = 0.0;
		}
		for (int l839 = 0; l839 < 3; l839 = l839 + 1) {
			fRec824[l839] = 0.0;
		}
		for (int l840 = 0; l840 < 3; l840 = l840 + 1) {
			fRec848[l840] = 0.0;
		}
		for (int l841 = 0; l841 < 3; l841 = l841 + 1) {
			fRec847[l841] = 0.0;
		}
		for (int l842 = 0; l842 < 3; l842 = l842 + 1) {
			fRec846[l842] = 0.0;
		}
		for (int l843 = 0; l843 < 3; l843 = l843 + 1) {
			fRec845[l843] = 0.0;
		}
		for (int l844 = 0; l844 < 3; l844 = l844 + 1) {
			fRec844[l844] = 0.0;
		}
		for (int l845 = 0; l845 < 3; l845 = l845 + 1) {
			fRec843[l845] = 0.0;
		}
		for (int l846 = 0; l846 < 3; l846 = l846 + 1) {
			fRec842[l846] = 0.0;
		}
		for (int l847 = 0; l847 < 3; l847 = l847 + 1) {
			fRec841[l847] = 0.0;
		}
		for (int l848 = 0; l848 < 3; l848 = l848 + 1) {
			fRec840[l848] = 0.0;
		}
		for (int l849 = 0; l849 < 3; l849 = l849 + 1) {
			fRec839[l849] = 0.0;
		}
		for (int l850 = 0; l850 < 3; l850 = l850 + 1) {
			fRec838[l850] = 0.0;
		}
		for (int l851 = 0; l851 < 3; l851 = l851 + 1) {
			fRec837[l851] = 0.0;
		}
		for (int l852 = 0; l852 < 2; l852 = l852 + 1) {
			fRec836[l852] = 0.0;
		}
		for (int l853 = 0; l853 < 3; l853 = l853 + 1) {
			fRec860[l853] = 0.0;
		}
		for (int l854 = 0; l854 < 3; l854 = l854 + 1) {
			fRec859[l854] = 0.0;
		}
		for (int l855 = 0; l855 < 3; l855 = l855 + 1) {
			fRec858[l855] = 0.0;
		}
		for (int l856 = 0; l856 < 3; l856 = l856 + 1) {
			fRec857[l856] = 0.0;
		}
		for (int l857 = 0; l857 < 3; l857 = l857 + 1) {
			fRec856[l857] = 0.0;
		}
		for (int l858 = 0; l858 < 3; l858 = l858 + 1) {
			fRec855[l858] = 0.0;
		}
		for (int l859 = 0; l859 < 3; l859 = l859 + 1) {
			fRec854[l859] = 0.0;
		}
		for (int l860 = 0; l860 < 3; l860 = l860 + 1) {
			fRec853[l860] = 0.0;
		}
		for (int l861 = 0; l861 < 3; l861 = l861 + 1) {
			fRec852[l861] = 0.0;
		}
		for (int l862 = 0; l862 < 3; l862 = l862 + 1) {
			fRec851[l862] = 0.0;
		}
		for (int l863 = 0; l863 < 3; l863 = l863 + 1) {
			fRec850[l863] = 0.0;
		}
		for (int l864 = 0; l864 < 3; l864 = l864 + 1) {
			fRec849[l864] = 0.0;
		}
		for (int l865 = 0; l865 < 3; l865 = l865 + 1) {
			fRec873[l865] = 0.0;
		}
		for (int l866 = 0; l866 < 3; l866 = l866 + 1) {
			fRec872[l866] = 0.0;
		}
		for (int l867 = 0; l867 < 3; l867 = l867 + 1) {
			fRec871[l867] = 0.0;
		}
		for (int l868 = 0; l868 < 3; l868 = l868 + 1) {
			fRec870[l868] = 0.0;
		}
		for (int l869 = 0; l869 < 3; l869 = l869 + 1) {
			fRec869[l869] = 0.0;
		}
		for (int l870 = 0; l870 < 3; l870 = l870 + 1) {
			fRec868[l870] = 0.0;
		}
		for (int l871 = 0; l871 < 3; l871 = l871 + 1) {
			fRec867[l871] = 0.0;
		}
		for (int l872 = 0; l872 < 3; l872 = l872 + 1) {
			fRec866[l872] = 0.0;
		}
		for (int l873 = 0; l873 < 3; l873 = l873 + 1) {
			fRec865[l873] = 0.0;
		}
		for (int l874 = 0; l874 < 3; l874 = l874 + 1) {
			fRec864[l874] = 0.0;
		}
		for (int l875 = 0; l875 < 3; l875 = l875 + 1) {
			fRec863[l875] = 0.0;
		}
		for (int l876 = 0; l876 < 3; l876 = l876 + 1) {
			fRec862[l876] = 0.0;
		}
		for (int l877 = 0; l877 < 2; l877 = l877 + 1) {
			fRec861[l877] = 0.0;
		}
		for (int l878 = 0; l878 < 3; l878 = l878 + 1) {
			fRec885[l878] = 0.0;
		}
		for (int l879 = 0; l879 < 3; l879 = l879 + 1) {
			fRec884[l879] = 0.0;
		}
		for (int l880 = 0; l880 < 3; l880 = l880 + 1) {
			fRec883[l880] = 0.0;
		}
		for (int l881 = 0; l881 < 3; l881 = l881 + 1) {
			fRec882[l881] = 0.0;
		}
		for (int l882 = 0; l882 < 3; l882 = l882 + 1) {
			fRec881[l882] = 0.0;
		}
		for (int l883 = 0; l883 < 3; l883 = l883 + 1) {
			fRec880[l883] = 0.0;
		}
		for (int l884 = 0; l884 < 3; l884 = l884 + 1) {
			fRec879[l884] = 0.0;
		}
		for (int l885 = 0; l885 < 3; l885 = l885 + 1) {
			fRec878[l885] = 0.0;
		}
		for (int l886 = 0; l886 < 3; l886 = l886 + 1) {
			fRec877[l886] = 0.0;
		}
		for (int l887 = 0; l887 < 3; l887 = l887 + 1) {
			fRec876[l887] = 0.0;
		}
		for (int l888 = 0; l888 < 3; l888 = l888 + 1) {
			fRec875[l888] = 0.0;
		}
		for (int l889 = 0; l889 < 3; l889 = l889 + 1) {
			fRec874[l889] = 0.0;
		}
		for (int l890 = 0; l890 < 3; l890 = l890 + 1) {
			fRec898[l890] = 0.0;
		}
		for (int l891 = 0; l891 < 3; l891 = l891 + 1) {
			fRec897[l891] = 0.0;
		}
		for (int l892 = 0; l892 < 3; l892 = l892 + 1) {
			fRec896[l892] = 0.0;
		}
		for (int l893 = 0; l893 < 3; l893 = l893 + 1) {
			fRec895[l893] = 0.0;
		}
		for (int l894 = 0; l894 < 3; l894 = l894 + 1) {
			fRec894[l894] = 0.0;
		}
		for (int l895 = 0; l895 < 3; l895 = l895 + 1) {
			fRec893[l895] = 0.0;
		}
		for (int l896 = 0; l896 < 3; l896 = l896 + 1) {
			fRec892[l896] = 0.0;
		}
		for (int l897 = 0; l897 < 3; l897 = l897 + 1) {
			fRec891[l897] = 0.0;
		}
		for (int l898 = 0; l898 < 3; l898 = l898 + 1) {
			fRec890[l898] = 0.0;
		}
		for (int l899 = 0; l899 < 3; l899 = l899 + 1) {
			fRec889[l899] = 0.0;
		}
		for (int l900 = 0; l900 < 3; l900 = l900 + 1) {
			fRec888[l900] = 0.0;
		}
		for (int l901 = 0; l901 < 3; l901 = l901 + 1) {
			fRec887[l901] = 0.0;
		}
		for (int l902 = 0; l902 < 2; l902 = l902 + 1) {
			fRec886[l902] = 0.0;
		}
		for (int l903 = 0; l903 < 3; l903 = l903 + 1) {
			fRec910[l903] = 0.0;
		}
		for (int l904 = 0; l904 < 3; l904 = l904 + 1) {
			fRec909[l904] = 0.0;
		}
		for (int l905 = 0; l905 < 3; l905 = l905 + 1) {
			fRec908[l905] = 0.0;
		}
		for (int l906 = 0; l906 < 3; l906 = l906 + 1) {
			fRec907[l906] = 0.0;
		}
		for (int l907 = 0; l907 < 3; l907 = l907 + 1) {
			fRec906[l907] = 0.0;
		}
		for (int l908 = 0; l908 < 3; l908 = l908 + 1) {
			fRec905[l908] = 0.0;
		}
		for (int l909 = 0; l909 < 3; l909 = l909 + 1) {
			fRec904[l909] = 0.0;
		}
		for (int l910 = 0; l910 < 3; l910 = l910 + 1) {
			fRec903[l910] = 0.0;
		}
		for (int l911 = 0; l911 < 3; l911 = l911 + 1) {
			fRec902[l911] = 0.0;
		}
		for (int l912 = 0; l912 < 3; l912 = l912 + 1) {
			fRec901[l912] = 0.0;
		}
		for (int l913 = 0; l913 < 3; l913 = l913 + 1) {
			fRec900[l913] = 0.0;
		}
		for (int l914 = 0; l914 < 3; l914 = l914 + 1) {
			fRec899[l914] = 0.0;
		}
		for (int l915 = 0; l915 < 2; l915 = l915 + 1) {
			fRec0[l915] = 0.0;
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
	
	virtual piece_nono_quando_vocoder* clone() {
		return new piece_nono_quando_vocoder();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("nono.quando.vocoder");
		ui_interface->addHorizontalSlider("attack", &fHslider1, FAUSTFLOAT(1e+01), FAUSTFLOAT(0.0), FAUSTFLOAT(5e+02), FAUSTFLOAT(0.001));
		ui_interface->addHorizontalSlider("color", &fHslider3, FAUSTFLOAT(-0.5), FAUSTFLOAT(-1.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.001));
		ui_interface->addHorizontalSlider("gain", &fHslider2, FAUSTFLOAT(0.0), FAUSTFLOAT(-6e+01), FAUSTFLOAT(6.0), FAUSTFLOAT(0.001));
		ui_interface->addHorizontalSlider("release", &fHslider0, FAUSTFLOAT(1e+02), FAUSTFLOAT(0.0), FAUSTFLOAT(1e+03), FAUSTFLOAT(0.001));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		double fSlow0 = fConst41 * double(fHslider0);
		double fSlow1 = fConst41 * double(fHslider1);
		double fSlow2 = fConst42 * std::pow(1e+01, 0.05 * double(fHslider2));
		double fSlow3 = double(fHslider3);
		double fSlow4 = std::tan(fConst46 * std::pow(5.623413251903491, 4.0 - fSlow3));
		double fSlow5 = fConst45 / fSlow4;
		double fSlow6 = fConst52 * fSlow4;
		double fSlow7 = std::tan(fConst46 * std::pow(5.623413251903491, 3.0 - fSlow3));
		double fSlow8 = fConst51 * ((fConst47 + fSlow6) / fSlow7);
		double fSlow9 = fConst52 * fSlow7;
		double fSlow10 = std::tan(fConst46 * std::pow(5.623413251903491, 2.0 - fSlow3));
		double fSlow11 = fConst55 * ((fConst47 + fSlow9) / fSlow10);
		double fSlow12 = fConst52 * fSlow10;
		double fSlow13 = std::tan(fConst46 * std::pow(5.623413251903491, 1.0 - fSlow3));
		double fSlow14 = fConst58 * ((fConst47 + fSlow12) / fSlow13);
		double fSlow15 = fConst52 * fSlow13;
		double fSlow16 = std::tan(fConst46 * std::pow(5.623413251903491, -fSlow3));
		double fSlow17 = fConst48 * ((fConst47 + fSlow15) / fSlow16);
		double fSlow18 = fConst52 * fSlow16;
		double fSlow19 = fConst47 + fSlow18;
		double fSlow20 = fSlow18 - fConst47;
		double fSlow21 = fSlow15 - fConst47;
		double fSlow22 = fConst48 / fSlow16;
		double fSlow23 = fSlow12 - fConst47;
		double fSlow24 = fConst58 / fSlow13;
		double fSlow25 = fSlow9 - fConst47;
		double fSlow26 = fConst55 / fSlow10;
		double fSlow27 = fSlow6 - fConst47;
		double fSlow28 = fConst51 / fSlow7;
		double fSlow29 = 2.0 * fSlow3;
		int iSlow30 = ((fSlow29 > 0.0) - (fSlow29 < 0.0)) > 0;
		double fSlow31 = std::pow(((iSlow30) ? 1.0 : 0.8016) * std::exp(-(fSlow29 * ((iSlow30) ? -4.28 : -2.633))) + ((iSlow30) ? 0.0 : 0.1984) * std::exp(-(fSlow29 * ((iSlow30) ? 0.0 : -0.7196))), -1.0);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			double fTemp0 = double(input0[i0]);
			fRec13[0] = fTemp0 - fConst20 * (fConst23 * fRec13[2] + fConst24 * fRec13[1]);
			fRec12[0] = fConst20 * (fConst22 * fRec13[0] + fConst25 * fRec13[1] + fConst22 * fRec13[2]) - fConst17 * (fConst26 * fRec12[2] + fConst27 * fRec12[1]);
			fRec11[0] = fConst17 * (fConst19 * fRec12[0] + fConst28 * fRec12[1] + fConst19 * fRec12[2]) - fConst14 * (fConst29 * fRec11[2] + fConst30 * fRec11[1]);
			fRec10[0] = fConst14 * (fConst16 * fRec11[0] + fConst31 * fRec11[1] + fConst16 * fRec11[2]) - fConst11 * (fConst32 * fRec10[2] + fConst33 * fRec10[1]);
			fRec9[0] = fConst11 * (fConst13 * fRec10[0] + fConst34 * fRec10[1] + fConst13 * fRec10[2]) - fConst9 * (fConst35 * fRec9[2] + fConst36 * fRec9[1]);
			fRec8[0] = fConst9 * (fConst10 * fRec9[0] + fConst37 * fRec9[1] + fConst10 * fRec9[2]) - fConst4 * (fConst38 * fRec8[2] + fConst39 * fRec8[1]);
			fRec7[0] = fConst5 * (fConst8 * fRec8[0] + fConst40 * fRec8[1] + fConst8 * fRec8[2]) - fConst20 * (fConst23 * fRec7[2] + fConst24 * fRec7[1]);
			fRec6[0] = fConst20 * (fConst22 * fRec7[0] + fConst25 * fRec7[1] + fConst22 * fRec7[2]) - fConst17 * (fConst26 * fRec6[2] + fConst27 * fRec6[1]);
			fRec5[0] = fConst17 * (fConst19 * fRec6[0] + fConst28 * fRec6[1] + fConst19 * fRec6[2]) - fConst14 * (fConst29 * fRec5[2] + fConst30 * fRec5[1]);
			fRec4[0] = fConst14 * (fConst16 * fRec5[0] + fConst31 * fRec5[1] + fConst16 * fRec5[2]) - fConst11 * (fConst32 * fRec4[2] + fConst33 * fRec4[1]);
			fRec3[0] = fConst11 * (fConst13 * fRec4[0] + fConst34 * fRec4[1] + fConst13 * fRec4[2]) - fConst9 * (fConst35 * fRec3[2] + fConst36 * fRec3[1]);
			fRec2[0] = fConst9 * (fConst10 * fRec3[0] + fConst37 * fRec3[1] + fConst10 * fRec3[2]) - fConst4 * (fConst38 * fRec2[2] + fConst39 * fRec2[1]);
			double fTemp1 = std::fabs(fConst5 * (fConst8 * fRec2[0] + fConst40 * fRec2[1] + fConst8 * fRec2[2]));
			fRec14[0] = fSlow0 + fConst43 * fRec14[1];
			int iTemp2 = std::fabs(fRec14[0]) < 2.220446049250313e-16;
			double fTemp3 = ((iTemp2) ? 0.0 : std::exp(-(fConst44 / ((iTemp2) ? 1.0 : fRec14[0]))));
			fRec15[0] = fSlow1 + fConst43 * fRec15[1];
			int iTemp4 = std::fabs(fRec15[0]) < 2.220446049250313e-16;
			double fTemp5 = ((iTemp4) ? 0.0 : std::exp(-(fConst44 / ((iTemp4) ? 1.0 : fRec15[0]))));
			double fTemp6 = ((fTemp1 > fRec1[1]) ? fTemp5 : fTemp3);
			fRec1[0] = fTemp1 * (1.0 - fTemp6) + fRec1[1] * fTemp6;
			double fTemp7 = tanh(fRec1[0]);
			fRec28[0] = fSlow2 + fConst43 * fRec28[1];
			iRec35[0] = 1103515245 * iRec35[1] + 12345;
			double fTemp8 = double(iRec35[0]);
			fVec0[0] = fTemp8;
			fRec34[0] = 0.995 * fRec34[1] + 4.656612875245797e-10 * (fTemp8 - fVec0[1]);
			fRec33[0] = -(fConst61 * (fConst62 * fRec33[1] - (fSlow19 * fRec34[0] + fSlow20 * fRec34[1])));
			fVec1[0] = fSlow22 * fRec33[0];
			fRec32[0] = fConst60 * (fSlow17 * fRec33[0] + fSlow21 * fVec1[1] - fConst63 * fRec32[1]);
			fVec2[0] = fSlow24 * fRec32[0];
			fRec31[0] = fConst57 * (fSlow14 * fRec32[0] + fSlow23 * fVec2[1] - fConst64 * fRec31[1]);
			fVec3[0] = fSlow26 * fRec31[0];
			fRec30[0] = fConst54 * (fSlow11 * fRec31[0] + fSlow25 * fVec3[1] - fConst65 * fRec30[1]);
			fVec4[0] = fSlow28 * fRec30[0];
			fRec29[0] = fConst50 * (fSlow8 * fRec30[0] + fSlow27 * fVec4[1] - fConst66 * fRec29[1]);
			double fTemp9 = fRec28[0] * std::min<double>(1.0, std::max<double>(-1.0, fSlow5 * fRec29[0] * fSlow31));
			fRec27[0] = fTemp9 - fConst20 * (fConst23 * fRec27[2] + fConst24 * fRec27[1]);
			fRec26[0] = fConst20 * (fConst22 * fRec27[0] + fConst25 * fRec27[1] + fConst22 * fRec27[2]) - fConst17 * (fConst26 * fRec26[2] + fConst27 * fRec26[1]);
			fRec25[0] = fConst17 * (fConst19 * fRec26[0] + fConst28 * fRec26[1] + fConst19 * fRec26[2]) - fConst14 * (fConst29 * fRec25[2] + fConst30 * fRec25[1]);
			fRec24[0] = fConst14 * (fConst16 * fRec25[0] + fConst31 * fRec25[1] + fConst16 * fRec25[2]) - fConst11 * (fConst32 * fRec24[2] + fConst33 * fRec24[1]);
			fRec23[0] = fConst11 * (fConst13 * fRec24[0] + fConst34 * fRec24[1] + fConst13 * fRec24[2]) - fConst9 * (fConst35 * fRec23[2] + fConst36 * fRec23[1]);
			fRec22[0] = fConst9 * (fConst10 * fRec23[0] + fConst37 * fRec23[1] + fConst10 * fRec23[2]) - fConst4 * (fConst38 * fRec22[2] + fConst39 * fRec22[1]);
			fRec21[0] = fConst5 * fTemp7 * (fConst8 * fRec22[0] + fConst40 * fRec22[1] + fConst8 * fRec22[2]) - fConst20 * (fConst23 * fRec21[2] + fConst24 * fRec21[1]);
			fRec20[0] = fConst20 * (fConst22 * fRec21[0] + fConst25 * fRec21[1] + fConst22 * fRec21[2]) - fConst17 * (fConst26 * fRec20[2] + fConst27 * fRec20[1]);
			fRec19[0] = fConst17 * (fConst19 * fRec20[0] + fConst28 * fRec20[1] + fConst19 * fRec20[2]) - fConst14 * (fConst29 * fRec19[2] + fConst30 * fRec19[1]);
			fRec18[0] = fConst14 * (fConst16 * fRec19[0] + fConst31 * fRec19[1] + fConst16 * fRec19[2]) - fConst11 * (fConst32 * fRec18[2] + fConst33 * fRec18[1]);
			fRec17[0] = fConst11 * (fConst13 * fRec18[0] + fConst34 * fRec18[1] + fConst13 * fRec18[2]) - fConst9 * (fConst35 * fRec17[2] + fConst36 * fRec17[1]);
			fRec16[0] = fConst9 * (fConst10 * fRec17[0] + fConst37 * fRec17[1] + fConst10 * fRec17[2]) - fConst4 * (fConst38 * fRec16[2] + fConst39 * fRec16[1]);
			double fTemp10 = fConst40 * fRec16[1];
			fRec48[0] = fTemp0 - fConst86 * (fConst89 * fRec48[2] + fConst90 * fRec48[1]);
			fRec47[0] = fConst86 * (fConst88 * fRec48[0] + fConst91 * fRec48[1] + fConst88 * fRec48[2]) - fConst83 * (fConst92 * fRec47[2] + fConst93 * fRec47[1]);
			fRec46[0] = fConst83 * (fConst85 * fRec47[0] + fConst94 * fRec47[1] + fConst85 * fRec47[2]) - fConst80 * (fConst95 * fRec46[2] + fConst96 * fRec46[1]);
			fRec45[0] = fConst80 * (fConst82 * fRec46[0] + fConst97 * fRec46[1] + fConst82 * fRec46[2]) - fConst77 * (fConst98 * fRec45[2] + fConst99 * fRec45[1]);
			fRec44[0] = fConst77 * (fConst79 * fRec45[0] + fConst100 * fRec45[1] + fConst79 * fRec45[2]) - fConst75 * (fConst101 * fRec44[2] + fConst102 * fRec44[1]);
			fRec43[0] = fConst75 * (fConst76 * fRec44[0] + fConst103 * fRec44[1] + fConst76 * fRec44[2]) - fConst70 * (fConst104 * fRec43[2] + fConst105 * fRec43[1]);
			fRec42[0] = fConst71 * (fConst74 * fRec43[0] + fConst106 * fRec43[1] + fConst74 * fRec43[2]) - fConst86 * (fConst89 * fRec42[2] + fConst90 * fRec42[1]);
			fRec41[0] = fConst86 * (fConst88 * fRec42[0] + fConst91 * fRec42[1] + fConst88 * fRec42[2]) - fConst83 * (fConst92 * fRec41[2] + fConst93 * fRec41[1]);
			fRec40[0] = fConst83 * (fConst85 * fRec41[0] + fConst94 * fRec41[1] + fConst85 * fRec41[2]) - fConst80 * (fConst95 * fRec40[2] + fConst96 * fRec40[1]);
			fRec39[0] = fConst80 * (fConst82 * fRec40[0] + fConst97 * fRec40[1] + fConst82 * fRec40[2]) - fConst77 * (fConst98 * fRec39[2] + fConst99 * fRec39[1]);
			fRec38[0] = fConst77 * (fConst79 * fRec39[0] + fConst100 * fRec39[1] + fConst79 * fRec39[2]) - fConst75 * (fConst101 * fRec38[2] + fConst102 * fRec38[1]);
			fRec37[0] = fConst75 * (fConst76 * fRec38[0] + fConst103 * fRec38[1] + fConst76 * fRec38[2]) - fConst70 * (fConst104 * fRec37[2] + fConst105 * fRec37[1]);
			double fTemp11 = std::fabs(fConst71 * (fConst74 * fRec37[0] + fConst106 * fRec37[1] + fConst74 * fRec37[2]));
			double fTemp12 = ((fTemp11 > fRec36[1]) ? fTemp5 : fTemp3);
			fRec36[0] = fTemp11 * (1.0 - fTemp12) + fRec36[1] * fTemp12;
			double fTemp13 = tanh(fRec36[0]);
			fRec60[0] = fTemp9 - fConst86 * (fConst89 * fRec60[2] + fConst90 * fRec60[1]);
			fRec59[0] = fConst86 * (fConst88 * fRec60[0] + fConst91 * fRec60[1] + fConst88 * fRec60[2]) - fConst83 * (fConst92 * fRec59[2] + fConst93 * fRec59[1]);
			fRec58[0] = fConst83 * (fConst85 * fRec59[0] + fConst94 * fRec59[1] + fConst85 * fRec59[2]) - fConst80 * (fConst95 * fRec58[2] + fConst96 * fRec58[1]);
			fRec57[0] = fConst80 * (fConst82 * fRec58[0] + fConst97 * fRec58[1] + fConst82 * fRec58[2]) - fConst77 * (fConst98 * fRec57[2] + fConst99 * fRec57[1]);
			fRec56[0] = fConst77 * (fConst79 * fRec57[0] + fConst100 * fRec57[1] + fConst79 * fRec57[2]) - fConst75 * (fConst101 * fRec56[2] + fConst102 * fRec56[1]);
			fRec55[0] = fConst75 * (fConst76 * fRec56[0] + fConst103 * fRec56[1] + fConst76 * fRec56[2]) - fConst70 * (fConst104 * fRec55[2] + fConst105 * fRec55[1]);
			fRec54[0] = fConst71 * fTemp13 * (fConst74 * fRec55[0] + fConst106 * fRec55[1] + fConst74 * fRec55[2]) - fConst86 * (fConst89 * fRec54[2] + fConst90 * fRec54[1]);
			fRec53[0] = fConst86 * (fConst88 * fRec54[0] + fConst91 * fRec54[1] + fConst88 * fRec54[2]) - fConst83 * (fConst92 * fRec53[2] + fConst93 * fRec53[1]);
			fRec52[0] = fConst83 * (fConst85 * fRec53[0] + fConst94 * fRec53[1] + fConst85 * fRec53[2]) - fConst80 * (fConst95 * fRec52[2] + fConst96 * fRec52[1]);
			fRec51[0] = fConst80 * (fConst82 * fRec52[0] + fConst97 * fRec52[1] + fConst82 * fRec52[2]) - fConst77 * (fConst98 * fRec51[2] + fConst99 * fRec51[1]);
			fRec50[0] = fConst77 * (fConst79 * fRec51[0] + fConst100 * fRec51[1] + fConst79 * fRec51[2]) - fConst75 * (fConst101 * fRec50[2] + fConst102 * fRec50[1]);
			fRec49[0] = fConst75 * (fConst76 * fRec50[0] + fConst103 * fRec50[1] + fConst76 * fRec50[2]) - fConst70 * (fConst104 * fRec49[2] + fConst105 * fRec49[1]);
			double fTemp14 = fConst106 * fRec49[1];
			fRec73[0] = fTemp0 - fConst126 * (fConst129 * fRec73[2] + fConst130 * fRec73[1]);
			fRec72[0] = fConst126 * (fConst128 * fRec73[0] + fConst131 * fRec73[1] + fConst128 * fRec73[2]) - fConst123 * (fConst132 * fRec72[2] + fConst133 * fRec72[1]);
			fRec71[0] = fConst123 * (fConst125 * fRec72[0] + fConst134 * fRec72[1] + fConst125 * fRec72[2]) - fConst120 * (fConst135 * fRec71[2] + fConst136 * fRec71[1]);
			fRec70[0] = fConst120 * (fConst122 * fRec71[0] + fConst137 * fRec71[1] + fConst122 * fRec71[2]) - fConst117 * (fConst138 * fRec70[2] + fConst139 * fRec70[1]);
			fRec69[0] = fConst117 * (fConst119 * fRec70[0] + fConst140 * fRec70[1] + fConst119 * fRec70[2]) - fConst115 * (fConst141 * fRec69[2] + fConst142 * fRec69[1]);
			fRec68[0] = fConst115 * (fConst116 * fRec69[0] + fConst143 * fRec69[1] + fConst116 * fRec69[2]) - fConst110 * (fConst144 * fRec68[2] + fConst145 * fRec68[1]);
			fRec67[0] = fConst111 * (fConst114 * fRec68[0] + fConst146 * fRec68[1] + fConst114 * fRec68[2]) - fConst126 * (fConst129 * fRec67[2] + fConst130 * fRec67[1]);
			fRec66[0] = fConst126 * (fConst128 * fRec67[0] + fConst131 * fRec67[1] + fConst128 * fRec67[2]) - fConst123 * (fConst132 * fRec66[2] + fConst133 * fRec66[1]);
			fRec65[0] = fConst123 * (fConst125 * fRec66[0] + fConst134 * fRec66[1] + fConst125 * fRec66[2]) - fConst120 * (fConst135 * fRec65[2] + fConst136 * fRec65[1]);
			fRec64[0] = fConst120 * (fConst122 * fRec65[0] + fConst137 * fRec65[1] + fConst122 * fRec65[2]) - fConst117 * (fConst138 * fRec64[2] + fConst139 * fRec64[1]);
			fRec63[0] = fConst117 * (fConst119 * fRec64[0] + fConst140 * fRec64[1] + fConst119 * fRec64[2]) - fConst115 * (fConst141 * fRec63[2] + fConst142 * fRec63[1]);
			fRec62[0] = fConst115 * (fConst116 * fRec63[0] + fConst143 * fRec63[1] + fConst116 * fRec63[2]) - fConst110 * (fConst144 * fRec62[2] + fConst145 * fRec62[1]);
			double fTemp15 = std::fabs(fConst111 * (fConst114 * fRec62[0] + fConst146 * fRec62[1] + fConst114 * fRec62[2]));
			double fTemp16 = ((fTemp15 > fRec61[1]) ? fTemp5 : fTemp3);
			fRec61[0] = fTemp15 * (1.0 - fTemp16) + fRec61[1] * fTemp16;
			double fTemp17 = tanh(fRec61[0]);
			fRec85[0] = fTemp9 - fConst126 * (fConst129 * fRec85[2] + fConst130 * fRec85[1]);
			fRec84[0] = fConst126 * (fConst128 * fRec85[0] + fConst131 * fRec85[1] + fConst128 * fRec85[2]) - fConst123 * (fConst132 * fRec84[2] + fConst133 * fRec84[1]);
			fRec83[0] = fConst123 * (fConst125 * fRec84[0] + fConst134 * fRec84[1] + fConst125 * fRec84[2]) - fConst120 * (fConst135 * fRec83[2] + fConst136 * fRec83[1]);
			fRec82[0] = fConst120 * (fConst122 * fRec83[0] + fConst137 * fRec83[1] + fConst122 * fRec83[2]) - fConst117 * (fConst138 * fRec82[2] + fConst139 * fRec82[1]);
			fRec81[0] = fConst117 * (fConst119 * fRec82[0] + fConst140 * fRec82[1] + fConst119 * fRec82[2]) - fConst115 * (fConst141 * fRec81[2] + fConst142 * fRec81[1]);
			fRec80[0] = fConst115 * (fConst116 * fRec81[0] + fConst143 * fRec81[1] + fConst116 * fRec81[2]) - fConst110 * (fConst144 * fRec80[2] + fConst145 * fRec80[1]);
			fRec79[0] = fConst111 * fTemp17 * (fConst114 * fRec80[0] + fConst146 * fRec80[1] + fConst114 * fRec80[2]) - fConst126 * (fConst129 * fRec79[2] + fConst130 * fRec79[1]);
			fRec78[0] = fConst126 * (fConst128 * fRec79[0] + fConst131 * fRec79[1] + fConst128 * fRec79[2]) - fConst123 * (fConst132 * fRec78[2] + fConst133 * fRec78[1]);
			fRec77[0] = fConst123 * (fConst125 * fRec78[0] + fConst134 * fRec78[1] + fConst125 * fRec78[2]) - fConst120 * (fConst135 * fRec77[2] + fConst136 * fRec77[1]);
			fRec76[0] = fConst120 * (fConst122 * fRec77[0] + fConst137 * fRec77[1] + fConst122 * fRec77[2]) - fConst117 * (fConst138 * fRec76[2] + fConst139 * fRec76[1]);
			fRec75[0] = fConst117 * (fConst119 * fRec76[0] + fConst140 * fRec76[1] + fConst119 * fRec76[2]) - fConst115 * (fConst141 * fRec75[2] + fConst142 * fRec75[1]);
			fRec74[0] = fConst115 * (fConst116 * fRec75[0] + fConst143 * fRec75[1] + fConst116 * fRec75[2]) - fConst110 * (fConst144 * fRec74[2] + fConst145 * fRec74[1]);
			double fTemp18 = fConst146 * fRec74[1];
			fRec98[0] = fTemp0 - fConst166 * (fConst169 * fRec98[2] + fConst170 * fRec98[1]);
			fRec97[0] = fConst166 * (fConst168 * fRec98[0] + fConst171 * fRec98[1] + fConst168 * fRec98[2]) - fConst163 * (fConst172 * fRec97[2] + fConst173 * fRec97[1]);
			fRec96[0] = fConst163 * (fConst165 * fRec97[0] + fConst174 * fRec97[1] + fConst165 * fRec97[2]) - fConst160 * (fConst175 * fRec96[2] + fConst176 * fRec96[1]);
			fRec95[0] = fConst160 * (fConst162 * fRec96[0] + fConst177 * fRec96[1] + fConst162 * fRec96[2]) - fConst157 * (fConst178 * fRec95[2] + fConst179 * fRec95[1]);
			fRec94[0] = fConst157 * (fConst159 * fRec95[0] + fConst180 * fRec95[1] + fConst159 * fRec95[2]) - fConst155 * (fConst181 * fRec94[2] + fConst182 * fRec94[1]);
			fRec93[0] = fConst155 * (fConst156 * fRec94[0] + fConst183 * fRec94[1] + fConst156 * fRec94[2]) - fConst150 * (fConst184 * fRec93[2] + fConst185 * fRec93[1]);
			fRec92[0] = fConst151 * (fConst154 * fRec93[0] + fConst186 * fRec93[1] + fConst154 * fRec93[2]) - fConst166 * (fConst169 * fRec92[2] + fConst170 * fRec92[1]);
			fRec91[0] = fConst166 * (fConst168 * fRec92[0] + fConst171 * fRec92[1] + fConst168 * fRec92[2]) - fConst163 * (fConst172 * fRec91[2] + fConst173 * fRec91[1]);
			fRec90[0] = fConst163 * (fConst165 * fRec91[0] + fConst174 * fRec91[1] + fConst165 * fRec91[2]) - fConst160 * (fConst175 * fRec90[2] + fConst176 * fRec90[1]);
			fRec89[0] = fConst160 * (fConst162 * fRec90[0] + fConst177 * fRec90[1] + fConst162 * fRec90[2]) - fConst157 * (fConst178 * fRec89[2] + fConst179 * fRec89[1]);
			fRec88[0] = fConst157 * (fConst159 * fRec89[0] + fConst180 * fRec89[1] + fConst159 * fRec89[2]) - fConst155 * (fConst181 * fRec88[2] + fConst182 * fRec88[1]);
			fRec87[0] = fConst155 * (fConst156 * fRec88[0] + fConst183 * fRec88[1] + fConst156 * fRec88[2]) - fConst150 * (fConst184 * fRec87[2] + fConst185 * fRec87[1]);
			double fTemp19 = std::fabs(fConst151 * (fConst154 * fRec87[0] + fConst186 * fRec87[1] + fConst154 * fRec87[2]));
			double fTemp20 = ((fTemp19 > fRec86[1]) ? fTemp5 : fTemp3);
			fRec86[0] = fTemp19 * (1.0 - fTemp20) + fRec86[1] * fTemp20;
			double fTemp21 = tanh(fRec86[0]);
			fRec110[0] = fTemp9 - fConst166 * (fConst169 * fRec110[2] + fConst170 * fRec110[1]);
			fRec109[0] = fConst166 * (fConst168 * fRec110[0] + fConst171 * fRec110[1] + fConst168 * fRec110[2]) - fConst163 * (fConst172 * fRec109[2] + fConst173 * fRec109[1]);
			fRec108[0] = fConst163 * (fConst165 * fRec109[0] + fConst174 * fRec109[1] + fConst165 * fRec109[2]) - fConst160 * (fConst175 * fRec108[2] + fConst176 * fRec108[1]);
			fRec107[0] = fConst160 * (fConst162 * fRec108[0] + fConst177 * fRec108[1] + fConst162 * fRec108[2]) - fConst157 * (fConst178 * fRec107[2] + fConst179 * fRec107[1]);
			fRec106[0] = fConst157 * (fConst159 * fRec107[0] + fConst180 * fRec107[1] + fConst159 * fRec107[2]) - fConst155 * (fConst181 * fRec106[2] + fConst182 * fRec106[1]);
			fRec105[0] = fConst155 * (fConst156 * fRec106[0] + fConst183 * fRec106[1] + fConst156 * fRec106[2]) - fConst150 * (fConst184 * fRec105[2] + fConst185 * fRec105[1]);
			fRec104[0] = fConst151 * fTemp21 * (fConst154 * fRec105[0] + fConst186 * fRec105[1] + fConst154 * fRec105[2]) - fConst166 * (fConst169 * fRec104[2] + fConst170 * fRec104[1]);
			fRec103[0] = fConst166 * (fConst168 * fRec104[0] + fConst171 * fRec104[1] + fConst168 * fRec104[2]) - fConst163 * (fConst172 * fRec103[2] + fConst173 * fRec103[1]);
			fRec102[0] = fConst163 * (fConst165 * fRec103[0] + fConst174 * fRec103[1] + fConst165 * fRec103[2]) - fConst160 * (fConst175 * fRec102[2] + fConst176 * fRec102[1]);
			fRec101[0] = fConst160 * (fConst162 * fRec102[0] + fConst177 * fRec102[1] + fConst162 * fRec102[2]) - fConst157 * (fConst178 * fRec101[2] + fConst179 * fRec101[1]);
			fRec100[0] = fConst157 * (fConst159 * fRec101[0] + fConst180 * fRec101[1] + fConst159 * fRec101[2]) - fConst155 * (fConst181 * fRec100[2] + fConst182 * fRec100[1]);
			fRec99[0] = fConst155 * (fConst156 * fRec100[0] + fConst183 * fRec100[1] + fConst156 * fRec100[2]) - fConst150 * (fConst184 * fRec99[2] + fConst185 * fRec99[1]);
			double fTemp22 = fConst186 * fRec99[1];
			fRec123[0] = fTemp0 - fConst206 * (fConst209 * fRec123[2] + fConst210 * fRec123[1]);
			fRec122[0] = fConst206 * (fConst208 * fRec123[0] + fConst211 * fRec123[1] + fConst208 * fRec123[2]) - fConst203 * (fConst212 * fRec122[2] + fConst213 * fRec122[1]);
			fRec121[0] = fConst203 * (fConst205 * fRec122[0] + fConst214 * fRec122[1] + fConst205 * fRec122[2]) - fConst200 * (fConst215 * fRec121[2] + fConst216 * fRec121[1]);
			fRec120[0] = fConst200 * (fConst202 * fRec121[0] + fConst217 * fRec121[1] + fConst202 * fRec121[2]) - fConst197 * (fConst218 * fRec120[2] + fConst219 * fRec120[1]);
			fRec119[0] = fConst197 * (fConst199 * fRec120[0] + fConst220 * fRec120[1] + fConst199 * fRec120[2]) - fConst195 * (fConst221 * fRec119[2] + fConst222 * fRec119[1]);
			fRec118[0] = fConst195 * (fConst196 * fRec119[0] + fConst223 * fRec119[1] + fConst196 * fRec119[2]) - fConst190 * (fConst224 * fRec118[2] + fConst225 * fRec118[1]);
			fRec117[0] = fConst191 * (fConst194 * fRec118[0] + fConst226 * fRec118[1] + fConst194 * fRec118[2]) - fConst206 * (fConst209 * fRec117[2] + fConst210 * fRec117[1]);
			fRec116[0] = fConst206 * (fConst208 * fRec117[0] + fConst211 * fRec117[1] + fConst208 * fRec117[2]) - fConst203 * (fConst212 * fRec116[2] + fConst213 * fRec116[1]);
			fRec115[0] = fConst203 * (fConst205 * fRec116[0] + fConst214 * fRec116[1] + fConst205 * fRec116[2]) - fConst200 * (fConst215 * fRec115[2] + fConst216 * fRec115[1]);
			fRec114[0] = fConst200 * (fConst202 * fRec115[0] + fConst217 * fRec115[1] + fConst202 * fRec115[2]) - fConst197 * (fConst218 * fRec114[2] + fConst219 * fRec114[1]);
			fRec113[0] = fConst197 * (fConst199 * fRec114[0] + fConst220 * fRec114[1] + fConst199 * fRec114[2]) - fConst195 * (fConst221 * fRec113[2] + fConst222 * fRec113[1]);
			fRec112[0] = fConst195 * (fConst196 * fRec113[0] + fConst223 * fRec113[1] + fConst196 * fRec113[2]) - fConst190 * (fConst224 * fRec112[2] + fConst225 * fRec112[1]);
			double fTemp23 = std::fabs(fConst191 * (fConst194 * fRec112[0] + fConst226 * fRec112[1] + fConst194 * fRec112[2]));
			double fTemp24 = ((fTemp23 > fRec111[1]) ? fTemp5 : fTemp3);
			fRec111[0] = fTemp23 * (1.0 - fTemp24) + fRec111[1] * fTemp24;
			double fTemp25 = tanh(fRec111[0]);
			fRec135[0] = fTemp9 - fConst206 * (fConst209 * fRec135[2] + fConst210 * fRec135[1]);
			fRec134[0] = fConst206 * (fConst208 * fRec135[0] + fConst211 * fRec135[1] + fConst208 * fRec135[2]) - fConst203 * (fConst212 * fRec134[2] + fConst213 * fRec134[1]);
			fRec133[0] = fConst203 * (fConst205 * fRec134[0] + fConst214 * fRec134[1] + fConst205 * fRec134[2]) - fConst200 * (fConst215 * fRec133[2] + fConst216 * fRec133[1]);
			fRec132[0] = fConst200 * (fConst202 * fRec133[0] + fConst217 * fRec133[1] + fConst202 * fRec133[2]) - fConst197 * (fConst218 * fRec132[2] + fConst219 * fRec132[1]);
			fRec131[0] = fConst197 * (fConst199 * fRec132[0] + fConst220 * fRec132[1] + fConst199 * fRec132[2]) - fConst195 * (fConst221 * fRec131[2] + fConst222 * fRec131[1]);
			fRec130[0] = fConst195 * (fConst196 * fRec131[0] + fConst223 * fRec131[1] + fConst196 * fRec131[2]) - fConst190 * (fConst224 * fRec130[2] + fConst225 * fRec130[1]);
			fRec129[0] = fConst191 * fTemp25 * (fConst194 * fRec130[0] + fConst226 * fRec130[1] + fConst194 * fRec130[2]) - fConst206 * (fConst209 * fRec129[2] + fConst210 * fRec129[1]);
			fRec128[0] = fConst206 * (fConst208 * fRec129[0] + fConst211 * fRec129[1] + fConst208 * fRec129[2]) - fConst203 * (fConst212 * fRec128[2] + fConst213 * fRec128[1]);
			fRec127[0] = fConst203 * (fConst205 * fRec128[0] + fConst214 * fRec128[1] + fConst205 * fRec128[2]) - fConst200 * (fConst215 * fRec127[2] + fConst216 * fRec127[1]);
			fRec126[0] = fConst200 * (fConst202 * fRec127[0] + fConst217 * fRec127[1] + fConst202 * fRec127[2]) - fConst197 * (fConst218 * fRec126[2] + fConst219 * fRec126[1]);
			fRec125[0] = fConst197 * (fConst199 * fRec126[0] + fConst220 * fRec126[1] + fConst199 * fRec126[2]) - fConst195 * (fConst221 * fRec125[2] + fConst222 * fRec125[1]);
			fRec124[0] = fConst195 * (fConst196 * fRec125[0] + fConst223 * fRec125[1] + fConst196 * fRec125[2]) - fConst190 * (fConst224 * fRec124[2] + fConst225 * fRec124[1]);
			double fTemp26 = fConst226 * fRec124[1];
			fRec148[0] = fTemp0 - fConst246 * (fConst249 * fRec148[2] + fConst250 * fRec148[1]);
			fRec147[0] = fConst246 * (fConst248 * fRec148[0] + fConst251 * fRec148[1] + fConst248 * fRec148[2]) - fConst243 * (fConst252 * fRec147[2] + fConst253 * fRec147[1]);
			fRec146[0] = fConst243 * (fConst245 * fRec147[0] + fConst254 * fRec147[1] + fConst245 * fRec147[2]) - fConst240 * (fConst255 * fRec146[2] + fConst256 * fRec146[1]);
			fRec145[0] = fConst240 * (fConst242 * fRec146[0] + fConst257 * fRec146[1] + fConst242 * fRec146[2]) - fConst237 * (fConst258 * fRec145[2] + fConst259 * fRec145[1]);
			fRec144[0] = fConst237 * (fConst239 * fRec145[0] + fConst260 * fRec145[1] + fConst239 * fRec145[2]) - fConst235 * (fConst261 * fRec144[2] + fConst262 * fRec144[1]);
			fRec143[0] = fConst235 * (fConst236 * fRec144[0] + fConst263 * fRec144[1] + fConst236 * fRec144[2]) - fConst230 * (fConst264 * fRec143[2] + fConst265 * fRec143[1]);
			fRec142[0] = fConst231 * (fConst234 * fRec143[0] + fConst266 * fRec143[1] + fConst234 * fRec143[2]) - fConst246 * (fConst249 * fRec142[2] + fConst250 * fRec142[1]);
			fRec141[0] = fConst246 * (fConst248 * fRec142[0] + fConst251 * fRec142[1] + fConst248 * fRec142[2]) - fConst243 * (fConst252 * fRec141[2] + fConst253 * fRec141[1]);
			fRec140[0] = fConst243 * (fConst245 * fRec141[0] + fConst254 * fRec141[1] + fConst245 * fRec141[2]) - fConst240 * (fConst255 * fRec140[2] + fConst256 * fRec140[1]);
			fRec139[0] = fConst240 * (fConst242 * fRec140[0] + fConst257 * fRec140[1] + fConst242 * fRec140[2]) - fConst237 * (fConst258 * fRec139[2] + fConst259 * fRec139[1]);
			fRec138[0] = fConst237 * (fConst239 * fRec139[0] + fConst260 * fRec139[1] + fConst239 * fRec139[2]) - fConst235 * (fConst261 * fRec138[2] + fConst262 * fRec138[1]);
			fRec137[0] = fConst235 * (fConst236 * fRec138[0] + fConst263 * fRec138[1] + fConst236 * fRec138[2]) - fConst230 * (fConst264 * fRec137[2] + fConst265 * fRec137[1]);
			double fTemp27 = std::fabs(fConst231 * (fConst234 * fRec137[0] + fConst266 * fRec137[1] + fConst234 * fRec137[2]));
			double fTemp28 = ((fTemp27 > fRec136[1]) ? fTemp5 : fTemp3);
			fRec136[0] = fTemp27 * (1.0 - fTemp28) + fRec136[1] * fTemp28;
			double fTemp29 = tanh(fRec136[0]);
			fRec160[0] = fTemp9 - fConst246 * (fConst249 * fRec160[2] + fConst250 * fRec160[1]);
			fRec159[0] = fConst246 * (fConst248 * fRec160[0] + fConst251 * fRec160[1] + fConst248 * fRec160[2]) - fConst243 * (fConst252 * fRec159[2] + fConst253 * fRec159[1]);
			fRec158[0] = fConst243 * (fConst245 * fRec159[0] + fConst254 * fRec159[1] + fConst245 * fRec159[2]) - fConst240 * (fConst255 * fRec158[2] + fConst256 * fRec158[1]);
			fRec157[0] = fConst240 * (fConst242 * fRec158[0] + fConst257 * fRec158[1] + fConst242 * fRec158[2]) - fConst237 * (fConst258 * fRec157[2] + fConst259 * fRec157[1]);
			fRec156[0] = fConst237 * (fConst239 * fRec157[0] + fConst260 * fRec157[1] + fConst239 * fRec157[2]) - fConst235 * (fConst261 * fRec156[2] + fConst262 * fRec156[1]);
			fRec155[0] = fConst235 * (fConst236 * fRec156[0] + fConst263 * fRec156[1] + fConst236 * fRec156[2]) - fConst230 * (fConst264 * fRec155[2] + fConst265 * fRec155[1]);
			fRec154[0] = fConst231 * fTemp29 * (fConst234 * fRec155[0] + fConst266 * fRec155[1] + fConst234 * fRec155[2]) - fConst246 * (fConst249 * fRec154[2] + fConst250 * fRec154[1]);
			fRec153[0] = fConst246 * (fConst248 * fRec154[0] + fConst251 * fRec154[1] + fConst248 * fRec154[2]) - fConst243 * (fConst252 * fRec153[2] + fConst253 * fRec153[1]);
			fRec152[0] = fConst243 * (fConst245 * fRec153[0] + fConst254 * fRec153[1] + fConst245 * fRec153[2]) - fConst240 * (fConst255 * fRec152[2] + fConst256 * fRec152[1]);
			fRec151[0] = fConst240 * (fConst242 * fRec152[0] + fConst257 * fRec152[1] + fConst242 * fRec152[2]) - fConst237 * (fConst258 * fRec151[2] + fConst259 * fRec151[1]);
			fRec150[0] = fConst237 * (fConst239 * fRec151[0] + fConst260 * fRec151[1] + fConst239 * fRec151[2]) - fConst235 * (fConst261 * fRec150[2] + fConst262 * fRec150[1]);
			fRec149[0] = fConst235 * (fConst236 * fRec150[0] + fConst263 * fRec150[1] + fConst236 * fRec150[2]) - fConst230 * (fConst264 * fRec149[2] + fConst265 * fRec149[1]);
			double fTemp30 = fConst266 * fRec149[1];
			fRec173[0] = fTemp0 - fConst286 * (fConst289 * fRec173[2] + fConst290 * fRec173[1]);
			fRec172[0] = fConst286 * (fConst288 * fRec173[0] + fConst291 * fRec173[1] + fConst288 * fRec173[2]) - fConst283 * (fConst292 * fRec172[2] + fConst293 * fRec172[1]);
			fRec171[0] = fConst283 * (fConst285 * fRec172[0] + fConst294 * fRec172[1] + fConst285 * fRec172[2]) - fConst280 * (fConst295 * fRec171[2] + fConst296 * fRec171[1]);
			fRec170[0] = fConst280 * (fConst282 * fRec171[0] + fConst297 * fRec171[1] + fConst282 * fRec171[2]) - fConst277 * (fConst298 * fRec170[2] + fConst299 * fRec170[1]);
			fRec169[0] = fConst277 * (fConst279 * fRec170[0] + fConst300 * fRec170[1] + fConst279 * fRec170[2]) - fConst275 * (fConst301 * fRec169[2] + fConst302 * fRec169[1]);
			fRec168[0] = fConst275 * (fConst276 * fRec169[0] + fConst303 * fRec169[1] + fConst276 * fRec169[2]) - fConst270 * (fConst304 * fRec168[2] + fConst305 * fRec168[1]);
			fRec167[0] = fConst271 * (fConst274 * fRec168[0] + fConst306 * fRec168[1] + fConst274 * fRec168[2]) - fConst286 * (fConst289 * fRec167[2] + fConst290 * fRec167[1]);
			fRec166[0] = fConst286 * (fConst288 * fRec167[0] + fConst291 * fRec167[1] + fConst288 * fRec167[2]) - fConst283 * (fConst292 * fRec166[2] + fConst293 * fRec166[1]);
			fRec165[0] = fConst283 * (fConst285 * fRec166[0] + fConst294 * fRec166[1] + fConst285 * fRec166[2]) - fConst280 * (fConst295 * fRec165[2] + fConst296 * fRec165[1]);
			fRec164[0] = fConst280 * (fConst282 * fRec165[0] + fConst297 * fRec165[1] + fConst282 * fRec165[2]) - fConst277 * (fConst298 * fRec164[2] + fConst299 * fRec164[1]);
			fRec163[0] = fConst277 * (fConst279 * fRec164[0] + fConst300 * fRec164[1] + fConst279 * fRec164[2]) - fConst275 * (fConst301 * fRec163[2] + fConst302 * fRec163[1]);
			fRec162[0] = fConst275 * (fConst276 * fRec163[0] + fConst303 * fRec163[1] + fConst276 * fRec163[2]) - fConst270 * (fConst304 * fRec162[2] + fConst305 * fRec162[1]);
			double fTemp31 = std::fabs(fConst271 * (fConst274 * fRec162[0] + fConst306 * fRec162[1] + fConst274 * fRec162[2]));
			double fTemp32 = ((fTemp31 > fRec161[1]) ? fTemp5 : fTemp3);
			fRec161[0] = fTemp31 * (1.0 - fTemp32) + fRec161[1] * fTemp32;
			double fTemp33 = tanh(fRec161[0]);
			fRec185[0] = fTemp9 - fConst286 * (fConst289 * fRec185[2] + fConst290 * fRec185[1]);
			fRec184[0] = fConst286 * (fConst288 * fRec185[0] + fConst291 * fRec185[1] + fConst288 * fRec185[2]) - fConst283 * (fConst292 * fRec184[2] + fConst293 * fRec184[1]);
			fRec183[0] = fConst283 * (fConst285 * fRec184[0] + fConst294 * fRec184[1] + fConst285 * fRec184[2]) - fConst280 * (fConst295 * fRec183[2] + fConst296 * fRec183[1]);
			fRec182[0] = fConst280 * (fConst282 * fRec183[0] + fConst297 * fRec183[1] + fConst282 * fRec183[2]) - fConst277 * (fConst298 * fRec182[2] + fConst299 * fRec182[1]);
			fRec181[0] = fConst277 * (fConst279 * fRec182[0] + fConst300 * fRec182[1] + fConst279 * fRec182[2]) - fConst275 * (fConst301 * fRec181[2] + fConst302 * fRec181[1]);
			fRec180[0] = fConst275 * (fConst276 * fRec181[0] + fConst303 * fRec181[1] + fConst276 * fRec181[2]) - fConst270 * (fConst304 * fRec180[2] + fConst305 * fRec180[1]);
			fRec179[0] = fConst271 * fTemp33 * (fConst274 * fRec180[0] + fConst306 * fRec180[1] + fConst274 * fRec180[2]) - fConst286 * (fConst289 * fRec179[2] + fConst290 * fRec179[1]);
			fRec178[0] = fConst286 * (fConst288 * fRec179[0] + fConst291 * fRec179[1] + fConst288 * fRec179[2]) - fConst283 * (fConst292 * fRec178[2] + fConst293 * fRec178[1]);
			fRec177[0] = fConst283 * (fConst285 * fRec178[0] + fConst294 * fRec178[1] + fConst285 * fRec178[2]) - fConst280 * (fConst295 * fRec177[2] + fConst296 * fRec177[1]);
			fRec176[0] = fConst280 * (fConst282 * fRec177[0] + fConst297 * fRec177[1] + fConst282 * fRec177[2]) - fConst277 * (fConst298 * fRec176[2] + fConst299 * fRec176[1]);
			fRec175[0] = fConst277 * (fConst279 * fRec176[0] + fConst300 * fRec176[1] + fConst279 * fRec176[2]) - fConst275 * (fConst301 * fRec175[2] + fConst302 * fRec175[1]);
			fRec174[0] = fConst275 * (fConst276 * fRec175[0] + fConst303 * fRec175[1] + fConst276 * fRec175[2]) - fConst270 * (fConst304 * fRec174[2] + fConst305 * fRec174[1]);
			double fTemp34 = fConst306 * fRec174[1];
			fRec198[0] = fTemp0 - fConst326 * (fConst329 * fRec198[2] + fConst330 * fRec198[1]);
			fRec197[0] = fConst326 * (fConst328 * fRec198[0] + fConst331 * fRec198[1] + fConst328 * fRec198[2]) - fConst323 * (fConst332 * fRec197[2] + fConst333 * fRec197[1]);
			fRec196[0] = fConst323 * (fConst325 * fRec197[0] + fConst334 * fRec197[1] + fConst325 * fRec197[2]) - fConst320 * (fConst335 * fRec196[2] + fConst336 * fRec196[1]);
			fRec195[0] = fConst320 * (fConst322 * fRec196[0] + fConst337 * fRec196[1] + fConst322 * fRec196[2]) - fConst317 * (fConst338 * fRec195[2] + fConst339 * fRec195[1]);
			fRec194[0] = fConst317 * (fConst319 * fRec195[0] + fConst340 * fRec195[1] + fConst319 * fRec195[2]) - fConst315 * (fConst341 * fRec194[2] + fConst342 * fRec194[1]);
			fRec193[0] = fConst315 * (fConst316 * fRec194[0] + fConst343 * fRec194[1] + fConst316 * fRec194[2]) - fConst310 * (fConst344 * fRec193[2] + fConst345 * fRec193[1]);
			fRec192[0] = fConst311 * (fConst314 * fRec193[0] + fConst346 * fRec193[1] + fConst314 * fRec193[2]) - fConst326 * (fConst329 * fRec192[2] + fConst330 * fRec192[1]);
			fRec191[0] = fConst326 * (fConst328 * fRec192[0] + fConst331 * fRec192[1] + fConst328 * fRec192[2]) - fConst323 * (fConst332 * fRec191[2] + fConst333 * fRec191[1]);
			fRec190[0] = fConst323 * (fConst325 * fRec191[0] + fConst334 * fRec191[1] + fConst325 * fRec191[2]) - fConst320 * (fConst335 * fRec190[2] + fConst336 * fRec190[1]);
			fRec189[0] = fConst320 * (fConst322 * fRec190[0] + fConst337 * fRec190[1] + fConst322 * fRec190[2]) - fConst317 * (fConst338 * fRec189[2] + fConst339 * fRec189[1]);
			fRec188[0] = fConst317 * (fConst319 * fRec189[0] + fConst340 * fRec189[1] + fConst319 * fRec189[2]) - fConst315 * (fConst341 * fRec188[2] + fConst342 * fRec188[1]);
			fRec187[0] = fConst315 * (fConst316 * fRec188[0] + fConst343 * fRec188[1] + fConst316 * fRec188[2]) - fConst310 * (fConst344 * fRec187[2] + fConst345 * fRec187[1]);
			double fTemp35 = std::fabs(fConst311 * (fConst314 * fRec187[0] + fConst346 * fRec187[1] + fConst314 * fRec187[2]));
			double fTemp36 = ((fTemp35 > fRec186[1]) ? fTemp5 : fTemp3);
			fRec186[0] = fTemp35 * (1.0 - fTemp36) + fRec186[1] * fTemp36;
			double fTemp37 = tanh(fRec186[0]);
			fRec210[0] = fTemp9 - fConst326 * (fConst329 * fRec210[2] + fConst330 * fRec210[1]);
			fRec209[0] = fConst326 * (fConst328 * fRec210[0] + fConst331 * fRec210[1] + fConst328 * fRec210[2]) - fConst323 * (fConst332 * fRec209[2] + fConst333 * fRec209[1]);
			fRec208[0] = fConst323 * (fConst325 * fRec209[0] + fConst334 * fRec209[1] + fConst325 * fRec209[2]) - fConst320 * (fConst335 * fRec208[2] + fConst336 * fRec208[1]);
			fRec207[0] = fConst320 * (fConst322 * fRec208[0] + fConst337 * fRec208[1] + fConst322 * fRec208[2]) - fConst317 * (fConst338 * fRec207[2] + fConst339 * fRec207[1]);
			fRec206[0] = fConst317 * (fConst319 * fRec207[0] + fConst340 * fRec207[1] + fConst319 * fRec207[2]) - fConst315 * (fConst341 * fRec206[2] + fConst342 * fRec206[1]);
			fRec205[0] = fConst315 * (fConst316 * fRec206[0] + fConst343 * fRec206[1] + fConst316 * fRec206[2]) - fConst310 * (fConst344 * fRec205[2] + fConst345 * fRec205[1]);
			fRec204[0] = fConst311 * fTemp37 * (fConst314 * fRec205[0] + fConst346 * fRec205[1] + fConst314 * fRec205[2]) - fConst326 * (fConst329 * fRec204[2] + fConst330 * fRec204[1]);
			fRec203[0] = fConst326 * (fConst328 * fRec204[0] + fConst331 * fRec204[1] + fConst328 * fRec204[2]) - fConst323 * (fConst332 * fRec203[2] + fConst333 * fRec203[1]);
			fRec202[0] = fConst323 * (fConst325 * fRec203[0] + fConst334 * fRec203[1] + fConst325 * fRec203[2]) - fConst320 * (fConst335 * fRec202[2] + fConst336 * fRec202[1]);
			fRec201[0] = fConst320 * (fConst322 * fRec202[0] + fConst337 * fRec202[1] + fConst322 * fRec202[2]) - fConst317 * (fConst338 * fRec201[2] + fConst339 * fRec201[1]);
			fRec200[0] = fConst317 * (fConst319 * fRec201[0] + fConst340 * fRec201[1] + fConst319 * fRec201[2]) - fConst315 * (fConst341 * fRec200[2] + fConst342 * fRec200[1]);
			fRec199[0] = fConst315 * (fConst316 * fRec200[0] + fConst343 * fRec200[1] + fConst316 * fRec200[2]) - fConst310 * (fConst344 * fRec199[2] + fConst345 * fRec199[1]);
			double fTemp38 = fConst346 * fRec199[1];
			fRec223[0] = fTemp0 - fConst366 * (fConst369 * fRec223[2] + fConst370 * fRec223[1]);
			fRec222[0] = fConst366 * (fConst368 * fRec223[0] + fConst371 * fRec223[1] + fConst368 * fRec223[2]) - fConst363 * (fConst372 * fRec222[2] + fConst373 * fRec222[1]);
			fRec221[0] = fConst363 * (fConst365 * fRec222[0] + fConst374 * fRec222[1] + fConst365 * fRec222[2]) - fConst360 * (fConst375 * fRec221[2] + fConst376 * fRec221[1]);
			fRec220[0] = fConst360 * (fConst362 * fRec221[0] + fConst377 * fRec221[1] + fConst362 * fRec221[2]) - fConst357 * (fConst378 * fRec220[2] + fConst379 * fRec220[1]);
			fRec219[0] = fConst357 * (fConst359 * fRec220[0] + fConst380 * fRec220[1] + fConst359 * fRec220[2]) - fConst355 * (fConst381 * fRec219[2] + fConst382 * fRec219[1]);
			fRec218[0] = fConst355 * (fConst356 * fRec219[0] + fConst383 * fRec219[1] + fConst356 * fRec219[2]) - fConst350 * (fConst384 * fRec218[2] + fConst385 * fRec218[1]);
			fRec217[0] = fConst351 * (fConst354 * fRec218[0] + fConst386 * fRec218[1] + fConst354 * fRec218[2]) - fConst366 * (fConst369 * fRec217[2] + fConst370 * fRec217[1]);
			fRec216[0] = fConst366 * (fConst368 * fRec217[0] + fConst371 * fRec217[1] + fConst368 * fRec217[2]) - fConst363 * (fConst372 * fRec216[2] + fConst373 * fRec216[1]);
			fRec215[0] = fConst363 * (fConst365 * fRec216[0] + fConst374 * fRec216[1] + fConst365 * fRec216[2]) - fConst360 * (fConst375 * fRec215[2] + fConst376 * fRec215[1]);
			fRec214[0] = fConst360 * (fConst362 * fRec215[0] + fConst377 * fRec215[1] + fConst362 * fRec215[2]) - fConst357 * (fConst378 * fRec214[2] + fConst379 * fRec214[1]);
			fRec213[0] = fConst357 * (fConst359 * fRec214[0] + fConst380 * fRec214[1] + fConst359 * fRec214[2]) - fConst355 * (fConst381 * fRec213[2] + fConst382 * fRec213[1]);
			fRec212[0] = fConst355 * (fConst356 * fRec213[0] + fConst383 * fRec213[1] + fConst356 * fRec213[2]) - fConst350 * (fConst384 * fRec212[2] + fConst385 * fRec212[1]);
			double fTemp39 = std::fabs(fConst351 * (fConst354 * fRec212[0] + fConst386 * fRec212[1] + fConst354 * fRec212[2]));
			double fTemp40 = ((fTemp39 > fRec211[1]) ? fTemp5 : fTemp3);
			fRec211[0] = fTemp39 * (1.0 - fTemp40) + fRec211[1] * fTemp40;
			double fTemp41 = tanh(fRec211[0]);
			fRec235[0] = fTemp9 - fConst366 * (fConst369 * fRec235[2] + fConst370 * fRec235[1]);
			fRec234[0] = fConst366 * (fConst368 * fRec235[0] + fConst371 * fRec235[1] + fConst368 * fRec235[2]) - fConst363 * (fConst372 * fRec234[2] + fConst373 * fRec234[1]);
			fRec233[0] = fConst363 * (fConst365 * fRec234[0] + fConst374 * fRec234[1] + fConst365 * fRec234[2]) - fConst360 * (fConst375 * fRec233[2] + fConst376 * fRec233[1]);
			fRec232[0] = fConst360 * (fConst362 * fRec233[0] + fConst377 * fRec233[1] + fConst362 * fRec233[2]) - fConst357 * (fConst378 * fRec232[2] + fConst379 * fRec232[1]);
			fRec231[0] = fConst357 * (fConst359 * fRec232[0] + fConst380 * fRec232[1] + fConst359 * fRec232[2]) - fConst355 * (fConst381 * fRec231[2] + fConst382 * fRec231[1]);
			fRec230[0] = fConst355 * (fConst356 * fRec231[0] + fConst383 * fRec231[1] + fConst356 * fRec231[2]) - fConst350 * (fConst384 * fRec230[2] + fConst385 * fRec230[1]);
			fRec229[0] = fConst351 * fTemp41 * (fConst354 * fRec230[0] + fConst386 * fRec230[1] + fConst354 * fRec230[2]) - fConst366 * (fConst369 * fRec229[2] + fConst370 * fRec229[1]);
			fRec228[0] = fConst366 * (fConst368 * fRec229[0] + fConst371 * fRec229[1] + fConst368 * fRec229[2]) - fConst363 * (fConst372 * fRec228[2] + fConst373 * fRec228[1]);
			fRec227[0] = fConst363 * (fConst365 * fRec228[0] + fConst374 * fRec228[1] + fConst365 * fRec228[2]) - fConst360 * (fConst375 * fRec227[2] + fConst376 * fRec227[1]);
			fRec226[0] = fConst360 * (fConst362 * fRec227[0] + fConst377 * fRec227[1] + fConst362 * fRec227[2]) - fConst357 * (fConst378 * fRec226[2] + fConst379 * fRec226[1]);
			fRec225[0] = fConst357 * (fConst359 * fRec226[0] + fConst380 * fRec226[1] + fConst359 * fRec226[2]) - fConst355 * (fConst381 * fRec225[2] + fConst382 * fRec225[1]);
			fRec224[0] = fConst355 * (fConst356 * fRec225[0] + fConst383 * fRec225[1] + fConst356 * fRec225[2]) - fConst350 * (fConst384 * fRec224[2] + fConst385 * fRec224[1]);
			double fTemp42 = fConst386 * fRec224[1];
			fRec248[0] = fTemp0 - fConst406 * (fConst409 * fRec248[2] + fConst410 * fRec248[1]);
			fRec247[0] = fConst406 * (fConst408 * fRec248[0] + fConst411 * fRec248[1] + fConst408 * fRec248[2]) - fConst403 * (fConst412 * fRec247[2] + fConst413 * fRec247[1]);
			fRec246[0] = fConst403 * (fConst405 * fRec247[0] + fConst414 * fRec247[1] + fConst405 * fRec247[2]) - fConst400 * (fConst415 * fRec246[2] + fConst416 * fRec246[1]);
			fRec245[0] = fConst400 * (fConst402 * fRec246[0] + fConst417 * fRec246[1] + fConst402 * fRec246[2]) - fConst397 * (fConst418 * fRec245[2] + fConst419 * fRec245[1]);
			fRec244[0] = fConst397 * (fConst399 * fRec245[0] + fConst420 * fRec245[1] + fConst399 * fRec245[2]) - fConst395 * (fConst421 * fRec244[2] + fConst422 * fRec244[1]);
			fRec243[0] = fConst395 * (fConst396 * fRec244[0] + fConst423 * fRec244[1] + fConst396 * fRec244[2]) - fConst390 * (fConst424 * fRec243[2] + fConst425 * fRec243[1]);
			fRec242[0] = fConst391 * (fConst394 * fRec243[0] + fConst426 * fRec243[1] + fConst394 * fRec243[2]) - fConst406 * (fConst409 * fRec242[2] + fConst410 * fRec242[1]);
			fRec241[0] = fConst406 * (fConst408 * fRec242[0] + fConst411 * fRec242[1] + fConst408 * fRec242[2]) - fConst403 * (fConst412 * fRec241[2] + fConst413 * fRec241[1]);
			fRec240[0] = fConst403 * (fConst405 * fRec241[0] + fConst414 * fRec241[1] + fConst405 * fRec241[2]) - fConst400 * (fConst415 * fRec240[2] + fConst416 * fRec240[1]);
			fRec239[0] = fConst400 * (fConst402 * fRec240[0] + fConst417 * fRec240[1] + fConst402 * fRec240[2]) - fConst397 * (fConst418 * fRec239[2] + fConst419 * fRec239[1]);
			fRec238[0] = fConst397 * (fConst399 * fRec239[0] + fConst420 * fRec239[1] + fConst399 * fRec239[2]) - fConst395 * (fConst421 * fRec238[2] + fConst422 * fRec238[1]);
			fRec237[0] = fConst395 * (fConst396 * fRec238[0] + fConst423 * fRec238[1] + fConst396 * fRec238[2]) - fConst390 * (fConst424 * fRec237[2] + fConst425 * fRec237[1]);
			double fTemp43 = std::fabs(fConst391 * (fConst394 * fRec237[0] + fConst426 * fRec237[1] + fConst394 * fRec237[2]));
			double fTemp44 = ((fTemp43 > fRec236[1]) ? fTemp5 : fTemp3);
			fRec236[0] = fTemp43 * (1.0 - fTemp44) + fRec236[1] * fTemp44;
			double fTemp45 = tanh(fRec236[0]);
			fRec260[0] = fTemp9 - fConst406 * (fConst409 * fRec260[2] + fConst410 * fRec260[1]);
			fRec259[0] = fConst406 * (fConst408 * fRec260[0] + fConst411 * fRec260[1] + fConst408 * fRec260[2]) - fConst403 * (fConst412 * fRec259[2] + fConst413 * fRec259[1]);
			fRec258[0] = fConst403 * (fConst405 * fRec259[0] + fConst414 * fRec259[1] + fConst405 * fRec259[2]) - fConst400 * (fConst415 * fRec258[2] + fConst416 * fRec258[1]);
			fRec257[0] = fConst400 * (fConst402 * fRec258[0] + fConst417 * fRec258[1] + fConst402 * fRec258[2]) - fConst397 * (fConst418 * fRec257[2] + fConst419 * fRec257[1]);
			fRec256[0] = fConst397 * (fConst399 * fRec257[0] + fConst420 * fRec257[1] + fConst399 * fRec257[2]) - fConst395 * (fConst421 * fRec256[2] + fConst422 * fRec256[1]);
			fRec255[0] = fConst395 * (fConst396 * fRec256[0] + fConst423 * fRec256[1] + fConst396 * fRec256[2]) - fConst390 * (fConst424 * fRec255[2] + fConst425 * fRec255[1]);
			fRec254[0] = fConst391 * fTemp45 * (fConst394 * fRec255[0] + fConst426 * fRec255[1] + fConst394 * fRec255[2]) - fConst406 * (fConst409 * fRec254[2] + fConst410 * fRec254[1]);
			fRec253[0] = fConst406 * (fConst408 * fRec254[0] + fConst411 * fRec254[1] + fConst408 * fRec254[2]) - fConst403 * (fConst412 * fRec253[2] + fConst413 * fRec253[1]);
			fRec252[0] = fConst403 * (fConst405 * fRec253[0] + fConst414 * fRec253[1] + fConst405 * fRec253[2]) - fConst400 * (fConst415 * fRec252[2] + fConst416 * fRec252[1]);
			fRec251[0] = fConst400 * (fConst402 * fRec252[0] + fConst417 * fRec252[1] + fConst402 * fRec252[2]) - fConst397 * (fConst418 * fRec251[2] + fConst419 * fRec251[1]);
			fRec250[0] = fConst397 * (fConst399 * fRec251[0] + fConst420 * fRec251[1] + fConst399 * fRec251[2]) - fConst395 * (fConst421 * fRec250[2] + fConst422 * fRec250[1]);
			fRec249[0] = fConst395 * (fConst396 * fRec250[0] + fConst423 * fRec250[1] + fConst396 * fRec250[2]) - fConst390 * (fConst424 * fRec249[2] + fConst425 * fRec249[1]);
			double fTemp46 = fConst426 * fRec249[1];
			fRec273[0] = fTemp0 - fConst446 * (fConst449 * fRec273[2] + fConst450 * fRec273[1]);
			fRec272[0] = fConst446 * (fConst448 * fRec273[0] + fConst451 * fRec273[1] + fConst448 * fRec273[2]) - fConst443 * (fConst452 * fRec272[2] + fConst453 * fRec272[1]);
			fRec271[0] = fConst443 * (fConst445 * fRec272[0] + fConst454 * fRec272[1] + fConst445 * fRec272[2]) - fConst440 * (fConst455 * fRec271[2] + fConst456 * fRec271[1]);
			fRec270[0] = fConst440 * (fConst442 * fRec271[0] + fConst457 * fRec271[1] + fConst442 * fRec271[2]) - fConst437 * (fConst458 * fRec270[2] + fConst459 * fRec270[1]);
			fRec269[0] = fConst437 * (fConst439 * fRec270[0] + fConst460 * fRec270[1] + fConst439 * fRec270[2]) - fConst435 * (fConst461 * fRec269[2] + fConst462 * fRec269[1]);
			fRec268[0] = fConst435 * (fConst436 * fRec269[0] + fConst463 * fRec269[1] + fConst436 * fRec269[2]) - fConst430 * (fConst464 * fRec268[2] + fConst465 * fRec268[1]);
			fRec267[0] = fConst431 * (fConst434 * fRec268[0] + fConst466 * fRec268[1] + fConst434 * fRec268[2]) - fConst446 * (fConst449 * fRec267[2] + fConst450 * fRec267[1]);
			fRec266[0] = fConst446 * (fConst448 * fRec267[0] + fConst451 * fRec267[1] + fConst448 * fRec267[2]) - fConst443 * (fConst452 * fRec266[2] + fConst453 * fRec266[1]);
			fRec265[0] = fConst443 * (fConst445 * fRec266[0] + fConst454 * fRec266[1] + fConst445 * fRec266[2]) - fConst440 * (fConst455 * fRec265[2] + fConst456 * fRec265[1]);
			fRec264[0] = fConst440 * (fConst442 * fRec265[0] + fConst457 * fRec265[1] + fConst442 * fRec265[2]) - fConst437 * (fConst458 * fRec264[2] + fConst459 * fRec264[1]);
			fRec263[0] = fConst437 * (fConst439 * fRec264[0] + fConst460 * fRec264[1] + fConst439 * fRec264[2]) - fConst435 * (fConst461 * fRec263[2] + fConst462 * fRec263[1]);
			fRec262[0] = fConst435 * (fConst436 * fRec263[0] + fConst463 * fRec263[1] + fConst436 * fRec263[2]) - fConst430 * (fConst464 * fRec262[2] + fConst465 * fRec262[1]);
			double fTemp47 = std::fabs(fConst431 * (fConst434 * fRec262[0] + fConst466 * fRec262[1] + fConst434 * fRec262[2]));
			double fTemp48 = ((fTemp47 > fRec261[1]) ? fTemp5 : fTemp3);
			fRec261[0] = fTemp47 * (1.0 - fTemp48) + fRec261[1] * fTemp48;
			double fTemp49 = tanh(fRec261[0]);
			fRec285[0] = fTemp9 - fConst446 * (fConst449 * fRec285[2] + fConst450 * fRec285[1]);
			fRec284[0] = fConst446 * (fConst448 * fRec285[0] + fConst451 * fRec285[1] + fConst448 * fRec285[2]) - fConst443 * (fConst452 * fRec284[2] + fConst453 * fRec284[1]);
			fRec283[0] = fConst443 * (fConst445 * fRec284[0] + fConst454 * fRec284[1] + fConst445 * fRec284[2]) - fConst440 * (fConst455 * fRec283[2] + fConst456 * fRec283[1]);
			fRec282[0] = fConst440 * (fConst442 * fRec283[0] + fConst457 * fRec283[1] + fConst442 * fRec283[2]) - fConst437 * (fConst458 * fRec282[2] + fConst459 * fRec282[1]);
			fRec281[0] = fConst437 * (fConst439 * fRec282[0] + fConst460 * fRec282[1] + fConst439 * fRec282[2]) - fConst435 * (fConst461 * fRec281[2] + fConst462 * fRec281[1]);
			fRec280[0] = fConst435 * (fConst436 * fRec281[0] + fConst463 * fRec281[1] + fConst436 * fRec281[2]) - fConst430 * (fConst464 * fRec280[2] + fConst465 * fRec280[1]);
			fRec279[0] = fConst431 * fTemp49 * (fConst434 * fRec280[0] + fConst466 * fRec280[1] + fConst434 * fRec280[2]) - fConst446 * (fConst449 * fRec279[2] + fConst450 * fRec279[1]);
			fRec278[0] = fConst446 * (fConst448 * fRec279[0] + fConst451 * fRec279[1] + fConst448 * fRec279[2]) - fConst443 * (fConst452 * fRec278[2] + fConst453 * fRec278[1]);
			fRec277[0] = fConst443 * (fConst445 * fRec278[0] + fConst454 * fRec278[1] + fConst445 * fRec278[2]) - fConst440 * (fConst455 * fRec277[2] + fConst456 * fRec277[1]);
			fRec276[0] = fConst440 * (fConst442 * fRec277[0] + fConst457 * fRec277[1] + fConst442 * fRec277[2]) - fConst437 * (fConst458 * fRec276[2] + fConst459 * fRec276[1]);
			fRec275[0] = fConst437 * (fConst439 * fRec276[0] + fConst460 * fRec276[1] + fConst439 * fRec276[2]) - fConst435 * (fConst461 * fRec275[2] + fConst462 * fRec275[1]);
			fRec274[0] = fConst435 * (fConst436 * fRec275[0] + fConst463 * fRec275[1] + fConst436 * fRec275[2]) - fConst430 * (fConst464 * fRec274[2] + fConst465 * fRec274[1]);
			double fTemp50 = fConst466 * fRec274[1];
			fRec298[0] = fTemp0 - fConst486 * (fConst489 * fRec298[2] + fConst490 * fRec298[1]);
			fRec297[0] = fConst486 * (fConst488 * fRec298[0] + fConst491 * fRec298[1] + fConst488 * fRec298[2]) - fConst483 * (fConst492 * fRec297[2] + fConst493 * fRec297[1]);
			fRec296[0] = fConst483 * (fConst485 * fRec297[0] + fConst494 * fRec297[1] + fConst485 * fRec297[2]) - fConst480 * (fConst495 * fRec296[2] + fConst496 * fRec296[1]);
			fRec295[0] = fConst480 * (fConst482 * fRec296[0] + fConst497 * fRec296[1] + fConst482 * fRec296[2]) - fConst477 * (fConst498 * fRec295[2] + fConst499 * fRec295[1]);
			fRec294[0] = fConst477 * (fConst479 * fRec295[0] + fConst500 * fRec295[1] + fConst479 * fRec295[2]) - fConst475 * (fConst501 * fRec294[2] + fConst502 * fRec294[1]);
			fRec293[0] = fConst475 * (fConst476 * fRec294[0] + fConst503 * fRec294[1] + fConst476 * fRec294[2]) - fConst470 * (fConst504 * fRec293[2] + fConst505 * fRec293[1]);
			fRec292[0] = fConst471 * (fConst474 * fRec293[0] + fConst506 * fRec293[1] + fConst474 * fRec293[2]) - fConst486 * (fConst489 * fRec292[2] + fConst490 * fRec292[1]);
			fRec291[0] = fConst486 * (fConst488 * fRec292[0] + fConst491 * fRec292[1] + fConst488 * fRec292[2]) - fConst483 * (fConst492 * fRec291[2] + fConst493 * fRec291[1]);
			fRec290[0] = fConst483 * (fConst485 * fRec291[0] + fConst494 * fRec291[1] + fConst485 * fRec291[2]) - fConst480 * (fConst495 * fRec290[2] + fConst496 * fRec290[1]);
			fRec289[0] = fConst480 * (fConst482 * fRec290[0] + fConst497 * fRec290[1] + fConst482 * fRec290[2]) - fConst477 * (fConst498 * fRec289[2] + fConst499 * fRec289[1]);
			fRec288[0] = fConst477 * (fConst479 * fRec289[0] + fConst500 * fRec289[1] + fConst479 * fRec289[2]) - fConst475 * (fConst501 * fRec288[2] + fConst502 * fRec288[1]);
			fRec287[0] = fConst475 * (fConst476 * fRec288[0] + fConst503 * fRec288[1] + fConst476 * fRec288[2]) - fConst470 * (fConst504 * fRec287[2] + fConst505 * fRec287[1]);
			double fTemp51 = std::fabs(fConst471 * (fConst474 * fRec287[0] + fConst506 * fRec287[1] + fConst474 * fRec287[2]));
			double fTemp52 = ((fTemp51 > fRec286[1]) ? fTemp5 : fTemp3);
			fRec286[0] = fTemp51 * (1.0 - fTemp52) + fRec286[1] * fTemp52;
			double fTemp53 = tanh(fRec286[0]);
			fRec310[0] = fTemp9 - fConst486 * (fConst489 * fRec310[2] + fConst490 * fRec310[1]);
			fRec309[0] = fConst486 * (fConst488 * fRec310[0] + fConst491 * fRec310[1] + fConst488 * fRec310[2]) - fConst483 * (fConst492 * fRec309[2] + fConst493 * fRec309[1]);
			fRec308[0] = fConst483 * (fConst485 * fRec309[0] + fConst494 * fRec309[1] + fConst485 * fRec309[2]) - fConst480 * (fConst495 * fRec308[2] + fConst496 * fRec308[1]);
			fRec307[0] = fConst480 * (fConst482 * fRec308[0] + fConst497 * fRec308[1] + fConst482 * fRec308[2]) - fConst477 * (fConst498 * fRec307[2] + fConst499 * fRec307[1]);
			fRec306[0] = fConst477 * (fConst479 * fRec307[0] + fConst500 * fRec307[1] + fConst479 * fRec307[2]) - fConst475 * (fConst501 * fRec306[2] + fConst502 * fRec306[1]);
			fRec305[0] = fConst475 * (fConst476 * fRec306[0] + fConst503 * fRec306[1] + fConst476 * fRec306[2]) - fConst470 * (fConst504 * fRec305[2] + fConst505 * fRec305[1]);
			fRec304[0] = fConst471 * fTemp53 * (fConst474 * fRec305[0] + fConst506 * fRec305[1] + fConst474 * fRec305[2]) - fConst486 * (fConst489 * fRec304[2] + fConst490 * fRec304[1]);
			fRec303[0] = fConst486 * (fConst488 * fRec304[0] + fConst491 * fRec304[1] + fConst488 * fRec304[2]) - fConst483 * (fConst492 * fRec303[2] + fConst493 * fRec303[1]);
			fRec302[0] = fConst483 * (fConst485 * fRec303[0] + fConst494 * fRec303[1] + fConst485 * fRec303[2]) - fConst480 * (fConst495 * fRec302[2] + fConst496 * fRec302[1]);
			fRec301[0] = fConst480 * (fConst482 * fRec302[0] + fConst497 * fRec302[1] + fConst482 * fRec302[2]) - fConst477 * (fConst498 * fRec301[2] + fConst499 * fRec301[1]);
			fRec300[0] = fConst477 * (fConst479 * fRec301[0] + fConst500 * fRec301[1] + fConst479 * fRec301[2]) - fConst475 * (fConst501 * fRec300[2] + fConst502 * fRec300[1]);
			fRec299[0] = fConst475 * (fConst476 * fRec300[0] + fConst503 * fRec300[1] + fConst476 * fRec300[2]) - fConst470 * (fConst504 * fRec299[2] + fConst505 * fRec299[1]);
			double fTemp54 = fConst506 * fRec299[1];
			fRec323[0] = fTemp0 - fConst526 * (fConst529 * fRec323[2] + fConst530 * fRec323[1]);
			fRec322[0] = fConst526 * (fConst528 * fRec323[0] + fConst531 * fRec323[1] + fConst528 * fRec323[2]) - fConst523 * (fConst532 * fRec322[2] + fConst533 * fRec322[1]);
			fRec321[0] = fConst523 * (fConst525 * fRec322[0] + fConst534 * fRec322[1] + fConst525 * fRec322[2]) - fConst520 * (fConst535 * fRec321[2] + fConst536 * fRec321[1]);
			fRec320[0] = fConst520 * (fConst522 * fRec321[0] + fConst537 * fRec321[1] + fConst522 * fRec321[2]) - fConst517 * (fConst538 * fRec320[2] + fConst539 * fRec320[1]);
			fRec319[0] = fConst517 * (fConst519 * fRec320[0] + fConst540 * fRec320[1] + fConst519 * fRec320[2]) - fConst515 * (fConst541 * fRec319[2] + fConst542 * fRec319[1]);
			fRec318[0] = fConst515 * (fConst516 * fRec319[0] + fConst543 * fRec319[1] + fConst516 * fRec319[2]) - fConst510 * (fConst544 * fRec318[2] + fConst545 * fRec318[1]);
			fRec317[0] = fConst511 * (fConst514 * fRec318[0] + fConst546 * fRec318[1] + fConst514 * fRec318[2]) - fConst526 * (fConst529 * fRec317[2] + fConst530 * fRec317[1]);
			fRec316[0] = fConst526 * (fConst528 * fRec317[0] + fConst531 * fRec317[1] + fConst528 * fRec317[2]) - fConst523 * (fConst532 * fRec316[2] + fConst533 * fRec316[1]);
			fRec315[0] = fConst523 * (fConst525 * fRec316[0] + fConst534 * fRec316[1] + fConst525 * fRec316[2]) - fConst520 * (fConst535 * fRec315[2] + fConst536 * fRec315[1]);
			fRec314[0] = fConst520 * (fConst522 * fRec315[0] + fConst537 * fRec315[1] + fConst522 * fRec315[2]) - fConst517 * (fConst538 * fRec314[2] + fConst539 * fRec314[1]);
			fRec313[0] = fConst517 * (fConst519 * fRec314[0] + fConst540 * fRec314[1] + fConst519 * fRec314[2]) - fConst515 * (fConst541 * fRec313[2] + fConst542 * fRec313[1]);
			fRec312[0] = fConst515 * (fConst516 * fRec313[0] + fConst543 * fRec313[1] + fConst516 * fRec313[2]) - fConst510 * (fConst544 * fRec312[2] + fConst545 * fRec312[1]);
			double fTemp55 = std::fabs(fConst511 * (fConst514 * fRec312[0] + fConst546 * fRec312[1] + fConst514 * fRec312[2]));
			double fTemp56 = ((fTemp55 > fRec311[1]) ? fTemp5 : fTemp3);
			fRec311[0] = fTemp55 * (1.0 - fTemp56) + fRec311[1] * fTemp56;
			double fTemp57 = tanh(fRec311[0]);
			fRec335[0] = fTemp9 - fConst526 * (fConst529 * fRec335[2] + fConst530 * fRec335[1]);
			fRec334[0] = fConst526 * (fConst528 * fRec335[0] + fConst531 * fRec335[1] + fConst528 * fRec335[2]) - fConst523 * (fConst532 * fRec334[2] + fConst533 * fRec334[1]);
			fRec333[0] = fConst523 * (fConst525 * fRec334[0] + fConst534 * fRec334[1] + fConst525 * fRec334[2]) - fConst520 * (fConst535 * fRec333[2] + fConst536 * fRec333[1]);
			fRec332[0] = fConst520 * (fConst522 * fRec333[0] + fConst537 * fRec333[1] + fConst522 * fRec333[2]) - fConst517 * (fConst538 * fRec332[2] + fConst539 * fRec332[1]);
			fRec331[0] = fConst517 * (fConst519 * fRec332[0] + fConst540 * fRec332[1] + fConst519 * fRec332[2]) - fConst515 * (fConst541 * fRec331[2] + fConst542 * fRec331[1]);
			fRec330[0] = fConst515 * (fConst516 * fRec331[0] + fConst543 * fRec331[1] + fConst516 * fRec331[2]) - fConst510 * (fConst544 * fRec330[2] + fConst545 * fRec330[1]);
			fRec329[0] = fConst511 * fTemp57 * (fConst514 * fRec330[0] + fConst546 * fRec330[1] + fConst514 * fRec330[2]) - fConst526 * (fConst529 * fRec329[2] + fConst530 * fRec329[1]);
			fRec328[0] = fConst526 * (fConst528 * fRec329[0] + fConst531 * fRec329[1] + fConst528 * fRec329[2]) - fConst523 * (fConst532 * fRec328[2] + fConst533 * fRec328[1]);
			fRec327[0] = fConst523 * (fConst525 * fRec328[0] + fConst534 * fRec328[1] + fConst525 * fRec328[2]) - fConst520 * (fConst535 * fRec327[2] + fConst536 * fRec327[1]);
			fRec326[0] = fConst520 * (fConst522 * fRec327[0] + fConst537 * fRec327[1] + fConst522 * fRec327[2]) - fConst517 * (fConst538 * fRec326[2] + fConst539 * fRec326[1]);
			fRec325[0] = fConst517 * (fConst519 * fRec326[0] + fConst540 * fRec326[1] + fConst519 * fRec326[2]) - fConst515 * (fConst541 * fRec325[2] + fConst542 * fRec325[1]);
			fRec324[0] = fConst515 * (fConst516 * fRec325[0] + fConst543 * fRec325[1] + fConst516 * fRec325[2]) - fConst510 * (fConst544 * fRec324[2] + fConst545 * fRec324[1]);
			double fTemp58 = fConst546 * fRec324[1];
			fRec348[0] = fTemp0 - fConst566 * (fConst569 * fRec348[2] + fConst570 * fRec348[1]);
			fRec347[0] = fConst566 * (fConst568 * fRec348[0] + fConst571 * fRec348[1] + fConst568 * fRec348[2]) - fConst563 * (fConst572 * fRec347[2] + fConst573 * fRec347[1]);
			fRec346[0] = fConst563 * (fConst565 * fRec347[0] + fConst574 * fRec347[1] + fConst565 * fRec347[2]) - fConst560 * (fConst575 * fRec346[2] + fConst576 * fRec346[1]);
			fRec345[0] = fConst560 * (fConst562 * fRec346[0] + fConst577 * fRec346[1] + fConst562 * fRec346[2]) - fConst557 * (fConst578 * fRec345[2] + fConst579 * fRec345[1]);
			fRec344[0] = fConst557 * (fConst559 * fRec345[0] + fConst580 * fRec345[1] + fConst559 * fRec345[2]) - fConst555 * (fConst581 * fRec344[2] + fConst582 * fRec344[1]);
			fRec343[0] = fConst555 * (fConst556 * fRec344[0] + fConst583 * fRec344[1] + fConst556 * fRec344[2]) - fConst550 * (fConst584 * fRec343[2] + fConst585 * fRec343[1]);
			fRec342[0] = fConst551 * (fConst554 * fRec343[0] + fConst586 * fRec343[1] + fConst554 * fRec343[2]) - fConst566 * (fConst569 * fRec342[2] + fConst570 * fRec342[1]);
			fRec341[0] = fConst566 * (fConst568 * fRec342[0] + fConst571 * fRec342[1] + fConst568 * fRec342[2]) - fConst563 * (fConst572 * fRec341[2] + fConst573 * fRec341[1]);
			fRec340[0] = fConst563 * (fConst565 * fRec341[0] + fConst574 * fRec341[1] + fConst565 * fRec341[2]) - fConst560 * (fConst575 * fRec340[2] + fConst576 * fRec340[1]);
			fRec339[0] = fConst560 * (fConst562 * fRec340[0] + fConst577 * fRec340[1] + fConst562 * fRec340[2]) - fConst557 * (fConst578 * fRec339[2] + fConst579 * fRec339[1]);
			fRec338[0] = fConst557 * (fConst559 * fRec339[0] + fConst580 * fRec339[1] + fConst559 * fRec339[2]) - fConst555 * (fConst581 * fRec338[2] + fConst582 * fRec338[1]);
			fRec337[0] = fConst555 * (fConst556 * fRec338[0] + fConst583 * fRec338[1] + fConst556 * fRec338[2]) - fConst550 * (fConst584 * fRec337[2] + fConst585 * fRec337[1]);
			double fTemp59 = std::fabs(fConst551 * (fConst554 * fRec337[0] + fConst586 * fRec337[1] + fConst554 * fRec337[2]));
			double fTemp60 = ((fTemp59 > fRec336[1]) ? fTemp5 : fTemp3);
			fRec336[0] = fTemp59 * (1.0 - fTemp60) + fRec336[1] * fTemp60;
			double fTemp61 = tanh(fRec336[0]);
			fRec360[0] = fTemp9 - fConst566 * (fConst569 * fRec360[2] + fConst570 * fRec360[1]);
			fRec359[0] = fConst566 * (fConst568 * fRec360[0] + fConst571 * fRec360[1] + fConst568 * fRec360[2]) - fConst563 * (fConst572 * fRec359[2] + fConst573 * fRec359[1]);
			fRec358[0] = fConst563 * (fConst565 * fRec359[0] + fConst574 * fRec359[1] + fConst565 * fRec359[2]) - fConst560 * (fConst575 * fRec358[2] + fConst576 * fRec358[1]);
			fRec357[0] = fConst560 * (fConst562 * fRec358[0] + fConst577 * fRec358[1] + fConst562 * fRec358[2]) - fConst557 * (fConst578 * fRec357[2] + fConst579 * fRec357[1]);
			fRec356[0] = fConst557 * (fConst559 * fRec357[0] + fConst580 * fRec357[1] + fConst559 * fRec357[2]) - fConst555 * (fConst581 * fRec356[2] + fConst582 * fRec356[1]);
			fRec355[0] = fConst555 * (fConst556 * fRec356[0] + fConst583 * fRec356[1] + fConst556 * fRec356[2]) - fConst550 * (fConst584 * fRec355[2] + fConst585 * fRec355[1]);
			fRec354[0] = fConst551 * fTemp61 * (fConst554 * fRec355[0] + fConst586 * fRec355[1] + fConst554 * fRec355[2]) - fConst566 * (fConst569 * fRec354[2] + fConst570 * fRec354[1]);
			fRec353[0] = fConst566 * (fConst568 * fRec354[0] + fConst571 * fRec354[1] + fConst568 * fRec354[2]) - fConst563 * (fConst572 * fRec353[2] + fConst573 * fRec353[1]);
			fRec352[0] = fConst563 * (fConst565 * fRec353[0] + fConst574 * fRec353[1] + fConst565 * fRec353[2]) - fConst560 * (fConst575 * fRec352[2] + fConst576 * fRec352[1]);
			fRec351[0] = fConst560 * (fConst562 * fRec352[0] + fConst577 * fRec352[1] + fConst562 * fRec352[2]) - fConst557 * (fConst578 * fRec351[2] + fConst579 * fRec351[1]);
			fRec350[0] = fConst557 * (fConst559 * fRec351[0] + fConst580 * fRec351[1] + fConst559 * fRec351[2]) - fConst555 * (fConst581 * fRec350[2] + fConst582 * fRec350[1]);
			fRec349[0] = fConst555 * (fConst556 * fRec350[0] + fConst583 * fRec350[1] + fConst556 * fRec350[2]) - fConst550 * (fConst584 * fRec349[2] + fConst585 * fRec349[1]);
			double fTemp62 = fConst586 * fRec349[1];
			fRec373[0] = fTemp0 - fConst606 * (fConst609 * fRec373[2] + fConst610 * fRec373[1]);
			fRec372[0] = fConst606 * (fConst608 * fRec373[0] + fConst611 * fRec373[1] + fConst608 * fRec373[2]) - fConst603 * (fConst612 * fRec372[2] + fConst613 * fRec372[1]);
			fRec371[0] = fConst603 * (fConst605 * fRec372[0] + fConst614 * fRec372[1] + fConst605 * fRec372[2]) - fConst600 * (fConst615 * fRec371[2] + fConst616 * fRec371[1]);
			fRec370[0] = fConst600 * (fConst602 * fRec371[0] + fConst617 * fRec371[1] + fConst602 * fRec371[2]) - fConst597 * (fConst618 * fRec370[2] + fConst619 * fRec370[1]);
			fRec369[0] = fConst597 * (fConst599 * fRec370[0] + fConst620 * fRec370[1] + fConst599 * fRec370[2]) - fConst595 * (fConst621 * fRec369[2] + fConst622 * fRec369[1]);
			fRec368[0] = fConst595 * (fConst596 * fRec369[0] + fConst623 * fRec369[1] + fConst596 * fRec369[2]) - fConst590 * (fConst624 * fRec368[2] + fConst625 * fRec368[1]);
			fRec367[0] = fConst591 * (fConst594 * fRec368[0] + fConst626 * fRec368[1] + fConst594 * fRec368[2]) - fConst606 * (fConst609 * fRec367[2] + fConst610 * fRec367[1]);
			fRec366[0] = fConst606 * (fConst608 * fRec367[0] + fConst611 * fRec367[1] + fConst608 * fRec367[2]) - fConst603 * (fConst612 * fRec366[2] + fConst613 * fRec366[1]);
			fRec365[0] = fConst603 * (fConst605 * fRec366[0] + fConst614 * fRec366[1] + fConst605 * fRec366[2]) - fConst600 * (fConst615 * fRec365[2] + fConst616 * fRec365[1]);
			fRec364[0] = fConst600 * (fConst602 * fRec365[0] + fConst617 * fRec365[1] + fConst602 * fRec365[2]) - fConst597 * (fConst618 * fRec364[2] + fConst619 * fRec364[1]);
			fRec363[0] = fConst597 * (fConst599 * fRec364[0] + fConst620 * fRec364[1] + fConst599 * fRec364[2]) - fConst595 * (fConst621 * fRec363[2] + fConst622 * fRec363[1]);
			fRec362[0] = fConst595 * (fConst596 * fRec363[0] + fConst623 * fRec363[1] + fConst596 * fRec363[2]) - fConst590 * (fConst624 * fRec362[2] + fConst625 * fRec362[1]);
			double fTemp63 = std::fabs(fConst591 * (fConst594 * fRec362[0] + fConst626 * fRec362[1] + fConst594 * fRec362[2]));
			double fTemp64 = ((fTemp63 > fRec361[1]) ? fTemp5 : fTemp3);
			fRec361[0] = fTemp63 * (1.0 - fTemp64) + fRec361[1] * fTemp64;
			double fTemp65 = tanh(fRec361[0]);
			fRec385[0] = fTemp9 - fConst606 * (fConst609 * fRec385[2] + fConst610 * fRec385[1]);
			fRec384[0] = fConst606 * (fConst608 * fRec385[0] + fConst611 * fRec385[1] + fConst608 * fRec385[2]) - fConst603 * (fConst612 * fRec384[2] + fConst613 * fRec384[1]);
			fRec383[0] = fConst603 * (fConst605 * fRec384[0] + fConst614 * fRec384[1] + fConst605 * fRec384[2]) - fConst600 * (fConst615 * fRec383[2] + fConst616 * fRec383[1]);
			fRec382[0] = fConst600 * (fConst602 * fRec383[0] + fConst617 * fRec383[1] + fConst602 * fRec383[2]) - fConst597 * (fConst618 * fRec382[2] + fConst619 * fRec382[1]);
			fRec381[0] = fConst597 * (fConst599 * fRec382[0] + fConst620 * fRec382[1] + fConst599 * fRec382[2]) - fConst595 * (fConst621 * fRec381[2] + fConst622 * fRec381[1]);
			fRec380[0] = fConst595 * (fConst596 * fRec381[0] + fConst623 * fRec381[1] + fConst596 * fRec381[2]) - fConst590 * (fConst624 * fRec380[2] + fConst625 * fRec380[1]);
			fRec379[0] = fConst591 * fTemp65 * (fConst594 * fRec380[0] + fConst626 * fRec380[1] + fConst594 * fRec380[2]) - fConst606 * (fConst609 * fRec379[2] + fConst610 * fRec379[1]);
			fRec378[0] = fConst606 * (fConst608 * fRec379[0] + fConst611 * fRec379[1] + fConst608 * fRec379[2]) - fConst603 * (fConst612 * fRec378[2] + fConst613 * fRec378[1]);
			fRec377[0] = fConst603 * (fConst605 * fRec378[0] + fConst614 * fRec378[1] + fConst605 * fRec378[2]) - fConst600 * (fConst615 * fRec377[2] + fConst616 * fRec377[1]);
			fRec376[0] = fConst600 * (fConst602 * fRec377[0] + fConst617 * fRec377[1] + fConst602 * fRec377[2]) - fConst597 * (fConst618 * fRec376[2] + fConst619 * fRec376[1]);
			fRec375[0] = fConst597 * (fConst599 * fRec376[0] + fConst620 * fRec376[1] + fConst599 * fRec376[2]) - fConst595 * (fConst621 * fRec375[2] + fConst622 * fRec375[1]);
			fRec374[0] = fConst595 * (fConst596 * fRec375[0] + fConst623 * fRec375[1] + fConst596 * fRec375[2]) - fConst590 * (fConst624 * fRec374[2] + fConst625 * fRec374[1]);
			double fTemp66 = fConst626 * fRec374[1];
			fRec398[0] = fTemp0 - fConst646 * (fConst649 * fRec398[2] + fConst650 * fRec398[1]);
			fRec397[0] = fConst646 * (fConst648 * fRec398[0] + fConst651 * fRec398[1] + fConst648 * fRec398[2]) - fConst643 * (fConst652 * fRec397[2] + fConst653 * fRec397[1]);
			fRec396[0] = fConst643 * (fConst645 * fRec397[0] + fConst654 * fRec397[1] + fConst645 * fRec397[2]) - fConst640 * (fConst655 * fRec396[2] + fConst656 * fRec396[1]);
			fRec395[0] = fConst640 * (fConst642 * fRec396[0] + fConst657 * fRec396[1] + fConst642 * fRec396[2]) - fConst637 * (fConst658 * fRec395[2] + fConst659 * fRec395[1]);
			fRec394[0] = fConst637 * (fConst639 * fRec395[0] + fConst660 * fRec395[1] + fConst639 * fRec395[2]) - fConst635 * (fConst661 * fRec394[2] + fConst662 * fRec394[1]);
			fRec393[0] = fConst635 * (fConst636 * fRec394[0] + fConst663 * fRec394[1] + fConst636 * fRec394[2]) - fConst630 * (fConst664 * fRec393[2] + fConst665 * fRec393[1]);
			fRec392[0] = fConst631 * (fConst634 * fRec393[0] + fConst666 * fRec393[1] + fConst634 * fRec393[2]) - fConst646 * (fConst649 * fRec392[2] + fConst650 * fRec392[1]);
			fRec391[0] = fConst646 * (fConst648 * fRec392[0] + fConst651 * fRec392[1] + fConst648 * fRec392[2]) - fConst643 * (fConst652 * fRec391[2] + fConst653 * fRec391[1]);
			fRec390[0] = fConst643 * (fConst645 * fRec391[0] + fConst654 * fRec391[1] + fConst645 * fRec391[2]) - fConst640 * (fConst655 * fRec390[2] + fConst656 * fRec390[1]);
			fRec389[0] = fConst640 * (fConst642 * fRec390[0] + fConst657 * fRec390[1] + fConst642 * fRec390[2]) - fConst637 * (fConst658 * fRec389[2] + fConst659 * fRec389[1]);
			fRec388[0] = fConst637 * (fConst639 * fRec389[0] + fConst660 * fRec389[1] + fConst639 * fRec389[2]) - fConst635 * (fConst661 * fRec388[2] + fConst662 * fRec388[1]);
			fRec387[0] = fConst635 * (fConst636 * fRec388[0] + fConst663 * fRec388[1] + fConst636 * fRec388[2]) - fConst630 * (fConst664 * fRec387[2] + fConst665 * fRec387[1]);
			double fTemp67 = std::fabs(fConst631 * (fConst634 * fRec387[0] + fConst666 * fRec387[1] + fConst634 * fRec387[2]));
			double fTemp68 = ((fTemp67 > fRec386[1]) ? fTemp5 : fTemp3);
			fRec386[0] = fTemp67 * (1.0 - fTemp68) + fRec386[1] * fTemp68;
			double fTemp69 = tanh(fRec386[0]);
			fRec410[0] = fTemp9 - fConst646 * (fConst649 * fRec410[2] + fConst650 * fRec410[1]);
			fRec409[0] = fConst646 * (fConst648 * fRec410[0] + fConst651 * fRec410[1] + fConst648 * fRec410[2]) - fConst643 * (fConst652 * fRec409[2] + fConst653 * fRec409[1]);
			fRec408[0] = fConst643 * (fConst645 * fRec409[0] + fConst654 * fRec409[1] + fConst645 * fRec409[2]) - fConst640 * (fConst655 * fRec408[2] + fConst656 * fRec408[1]);
			fRec407[0] = fConst640 * (fConst642 * fRec408[0] + fConst657 * fRec408[1] + fConst642 * fRec408[2]) - fConst637 * (fConst658 * fRec407[2] + fConst659 * fRec407[1]);
			fRec406[0] = fConst637 * (fConst639 * fRec407[0] + fConst660 * fRec407[1] + fConst639 * fRec407[2]) - fConst635 * (fConst661 * fRec406[2] + fConst662 * fRec406[1]);
			fRec405[0] = fConst635 * (fConst636 * fRec406[0] + fConst663 * fRec406[1] + fConst636 * fRec406[2]) - fConst630 * (fConst664 * fRec405[2] + fConst665 * fRec405[1]);
			fRec404[0] = fConst631 * fTemp69 * (fConst634 * fRec405[0] + fConst666 * fRec405[1] + fConst634 * fRec405[2]) - fConst646 * (fConst649 * fRec404[2] + fConst650 * fRec404[1]);
			fRec403[0] = fConst646 * (fConst648 * fRec404[0] + fConst651 * fRec404[1] + fConst648 * fRec404[2]) - fConst643 * (fConst652 * fRec403[2] + fConst653 * fRec403[1]);
			fRec402[0] = fConst643 * (fConst645 * fRec403[0] + fConst654 * fRec403[1] + fConst645 * fRec403[2]) - fConst640 * (fConst655 * fRec402[2] + fConst656 * fRec402[1]);
			fRec401[0] = fConst640 * (fConst642 * fRec402[0] + fConst657 * fRec402[1] + fConst642 * fRec402[2]) - fConst637 * (fConst658 * fRec401[2] + fConst659 * fRec401[1]);
			fRec400[0] = fConst637 * (fConst639 * fRec401[0] + fConst660 * fRec401[1] + fConst639 * fRec401[2]) - fConst635 * (fConst661 * fRec400[2] + fConst662 * fRec400[1]);
			fRec399[0] = fConst635 * (fConst636 * fRec400[0] + fConst663 * fRec400[1] + fConst636 * fRec400[2]) - fConst630 * (fConst664 * fRec399[2] + fConst665 * fRec399[1]);
			double fTemp70 = fConst666 * fRec399[1];
			fRec423[0] = fTemp0 - fConst686 * (fConst689 * fRec423[2] + fConst690 * fRec423[1]);
			fRec422[0] = fConst686 * (fConst688 * fRec423[0] + fConst691 * fRec423[1] + fConst688 * fRec423[2]) - fConst683 * (fConst692 * fRec422[2] + fConst693 * fRec422[1]);
			fRec421[0] = fConst683 * (fConst685 * fRec422[0] + fConst694 * fRec422[1] + fConst685 * fRec422[2]) - fConst680 * (fConst695 * fRec421[2] + fConst696 * fRec421[1]);
			fRec420[0] = fConst680 * (fConst682 * fRec421[0] + fConst697 * fRec421[1] + fConst682 * fRec421[2]) - fConst677 * (fConst698 * fRec420[2] + fConst699 * fRec420[1]);
			fRec419[0] = fConst677 * (fConst679 * fRec420[0] + fConst700 * fRec420[1] + fConst679 * fRec420[2]) - fConst675 * (fConst701 * fRec419[2] + fConst702 * fRec419[1]);
			fRec418[0] = fConst675 * (fConst676 * fRec419[0] + fConst703 * fRec419[1] + fConst676 * fRec419[2]) - fConst670 * (fConst704 * fRec418[2] + fConst705 * fRec418[1]);
			fRec417[0] = fConst671 * (fConst674 * fRec418[0] + fConst706 * fRec418[1] + fConst674 * fRec418[2]) - fConst686 * (fConst689 * fRec417[2] + fConst690 * fRec417[1]);
			fRec416[0] = fConst686 * (fConst688 * fRec417[0] + fConst691 * fRec417[1] + fConst688 * fRec417[2]) - fConst683 * (fConst692 * fRec416[2] + fConst693 * fRec416[1]);
			fRec415[0] = fConst683 * (fConst685 * fRec416[0] + fConst694 * fRec416[1] + fConst685 * fRec416[2]) - fConst680 * (fConst695 * fRec415[2] + fConst696 * fRec415[1]);
			fRec414[0] = fConst680 * (fConst682 * fRec415[0] + fConst697 * fRec415[1] + fConst682 * fRec415[2]) - fConst677 * (fConst698 * fRec414[2] + fConst699 * fRec414[1]);
			fRec413[0] = fConst677 * (fConst679 * fRec414[0] + fConst700 * fRec414[1] + fConst679 * fRec414[2]) - fConst675 * (fConst701 * fRec413[2] + fConst702 * fRec413[1]);
			fRec412[0] = fConst675 * (fConst676 * fRec413[0] + fConst703 * fRec413[1] + fConst676 * fRec413[2]) - fConst670 * (fConst704 * fRec412[2] + fConst705 * fRec412[1]);
			double fTemp71 = std::fabs(fConst671 * (fConst674 * fRec412[0] + fConst706 * fRec412[1] + fConst674 * fRec412[2]));
			double fTemp72 = ((fTemp71 > fRec411[1]) ? fTemp5 : fTemp3);
			fRec411[0] = fTemp71 * (1.0 - fTemp72) + fRec411[1] * fTemp72;
			double fTemp73 = tanh(fRec411[0]);
			fRec435[0] = fTemp9 - fConst686 * (fConst689 * fRec435[2] + fConst690 * fRec435[1]);
			fRec434[0] = fConst686 * (fConst688 * fRec435[0] + fConst691 * fRec435[1] + fConst688 * fRec435[2]) - fConst683 * (fConst692 * fRec434[2] + fConst693 * fRec434[1]);
			fRec433[0] = fConst683 * (fConst685 * fRec434[0] + fConst694 * fRec434[1] + fConst685 * fRec434[2]) - fConst680 * (fConst695 * fRec433[2] + fConst696 * fRec433[1]);
			fRec432[0] = fConst680 * (fConst682 * fRec433[0] + fConst697 * fRec433[1] + fConst682 * fRec433[2]) - fConst677 * (fConst698 * fRec432[2] + fConst699 * fRec432[1]);
			fRec431[0] = fConst677 * (fConst679 * fRec432[0] + fConst700 * fRec432[1] + fConst679 * fRec432[2]) - fConst675 * (fConst701 * fRec431[2] + fConst702 * fRec431[1]);
			fRec430[0] = fConst675 * (fConst676 * fRec431[0] + fConst703 * fRec431[1] + fConst676 * fRec431[2]) - fConst670 * (fConst704 * fRec430[2] + fConst705 * fRec430[1]);
			fRec429[0] = fConst671 * fTemp73 * (fConst674 * fRec430[0] + fConst706 * fRec430[1] + fConst674 * fRec430[2]) - fConst686 * (fConst689 * fRec429[2] + fConst690 * fRec429[1]);
			fRec428[0] = fConst686 * (fConst688 * fRec429[0] + fConst691 * fRec429[1] + fConst688 * fRec429[2]) - fConst683 * (fConst692 * fRec428[2] + fConst693 * fRec428[1]);
			fRec427[0] = fConst683 * (fConst685 * fRec428[0] + fConst694 * fRec428[1] + fConst685 * fRec428[2]) - fConst680 * (fConst695 * fRec427[2] + fConst696 * fRec427[1]);
			fRec426[0] = fConst680 * (fConst682 * fRec427[0] + fConst697 * fRec427[1] + fConst682 * fRec427[2]) - fConst677 * (fConst698 * fRec426[2] + fConst699 * fRec426[1]);
			fRec425[0] = fConst677 * (fConst679 * fRec426[0] + fConst700 * fRec426[1] + fConst679 * fRec426[2]) - fConst675 * (fConst701 * fRec425[2] + fConst702 * fRec425[1]);
			fRec424[0] = fConst675 * (fConst676 * fRec425[0] + fConst703 * fRec425[1] + fConst676 * fRec425[2]) - fConst670 * (fConst704 * fRec424[2] + fConst705 * fRec424[1]);
			double fTemp74 = fConst706 * fRec424[1];
			fRec448[0] = fTemp0 - fConst726 * (fConst729 * fRec448[2] + fConst730 * fRec448[1]);
			fRec447[0] = fConst726 * (fConst728 * fRec448[0] + fConst731 * fRec448[1] + fConst728 * fRec448[2]) - fConst723 * (fConst732 * fRec447[2] + fConst733 * fRec447[1]);
			fRec446[0] = fConst723 * (fConst725 * fRec447[0] + fConst734 * fRec447[1] + fConst725 * fRec447[2]) - fConst720 * (fConst735 * fRec446[2] + fConst736 * fRec446[1]);
			fRec445[0] = fConst720 * (fConst722 * fRec446[0] + fConst737 * fRec446[1] + fConst722 * fRec446[2]) - fConst717 * (fConst738 * fRec445[2] + fConst739 * fRec445[1]);
			fRec444[0] = fConst717 * (fConst719 * fRec445[0] + fConst740 * fRec445[1] + fConst719 * fRec445[2]) - fConst715 * (fConst741 * fRec444[2] + fConst742 * fRec444[1]);
			fRec443[0] = fConst715 * (fConst716 * fRec444[0] + fConst743 * fRec444[1] + fConst716 * fRec444[2]) - fConst710 * (fConst744 * fRec443[2] + fConst745 * fRec443[1]);
			fRec442[0] = fConst711 * (fConst714 * fRec443[0] + fConst746 * fRec443[1] + fConst714 * fRec443[2]) - fConst726 * (fConst729 * fRec442[2] + fConst730 * fRec442[1]);
			fRec441[0] = fConst726 * (fConst728 * fRec442[0] + fConst731 * fRec442[1] + fConst728 * fRec442[2]) - fConst723 * (fConst732 * fRec441[2] + fConst733 * fRec441[1]);
			fRec440[0] = fConst723 * (fConst725 * fRec441[0] + fConst734 * fRec441[1] + fConst725 * fRec441[2]) - fConst720 * (fConst735 * fRec440[2] + fConst736 * fRec440[1]);
			fRec439[0] = fConst720 * (fConst722 * fRec440[0] + fConst737 * fRec440[1] + fConst722 * fRec440[2]) - fConst717 * (fConst738 * fRec439[2] + fConst739 * fRec439[1]);
			fRec438[0] = fConst717 * (fConst719 * fRec439[0] + fConst740 * fRec439[1] + fConst719 * fRec439[2]) - fConst715 * (fConst741 * fRec438[2] + fConst742 * fRec438[1]);
			fRec437[0] = fConst715 * (fConst716 * fRec438[0] + fConst743 * fRec438[1] + fConst716 * fRec438[2]) - fConst710 * (fConst744 * fRec437[2] + fConst745 * fRec437[1]);
			double fTemp75 = std::fabs(fConst711 * (fConst714 * fRec437[0] + fConst746 * fRec437[1] + fConst714 * fRec437[2]));
			double fTemp76 = ((fTemp75 > fRec436[1]) ? fTemp5 : fTemp3);
			fRec436[0] = fTemp75 * (1.0 - fTemp76) + fRec436[1] * fTemp76;
			double fTemp77 = tanh(fRec436[0]);
			fRec460[0] = fTemp9 - fConst726 * (fConst729 * fRec460[2] + fConst730 * fRec460[1]);
			fRec459[0] = fConst726 * (fConst728 * fRec460[0] + fConst731 * fRec460[1] + fConst728 * fRec460[2]) - fConst723 * (fConst732 * fRec459[2] + fConst733 * fRec459[1]);
			fRec458[0] = fConst723 * (fConst725 * fRec459[0] + fConst734 * fRec459[1] + fConst725 * fRec459[2]) - fConst720 * (fConst735 * fRec458[2] + fConst736 * fRec458[1]);
			fRec457[0] = fConst720 * (fConst722 * fRec458[0] + fConst737 * fRec458[1] + fConst722 * fRec458[2]) - fConst717 * (fConst738 * fRec457[2] + fConst739 * fRec457[1]);
			fRec456[0] = fConst717 * (fConst719 * fRec457[0] + fConst740 * fRec457[1] + fConst719 * fRec457[2]) - fConst715 * (fConst741 * fRec456[2] + fConst742 * fRec456[1]);
			fRec455[0] = fConst715 * (fConst716 * fRec456[0] + fConst743 * fRec456[1] + fConst716 * fRec456[2]) - fConst710 * (fConst744 * fRec455[2] + fConst745 * fRec455[1]);
			fRec454[0] = fConst711 * fTemp77 * (fConst714 * fRec455[0] + fConst746 * fRec455[1] + fConst714 * fRec455[2]) - fConst726 * (fConst729 * fRec454[2] + fConst730 * fRec454[1]);
			fRec453[0] = fConst726 * (fConst728 * fRec454[0] + fConst731 * fRec454[1] + fConst728 * fRec454[2]) - fConst723 * (fConst732 * fRec453[2] + fConst733 * fRec453[1]);
			fRec452[0] = fConst723 * (fConst725 * fRec453[0] + fConst734 * fRec453[1] + fConst725 * fRec453[2]) - fConst720 * (fConst735 * fRec452[2] + fConst736 * fRec452[1]);
			fRec451[0] = fConst720 * (fConst722 * fRec452[0] + fConst737 * fRec452[1] + fConst722 * fRec452[2]) - fConst717 * (fConst738 * fRec451[2] + fConst739 * fRec451[1]);
			fRec450[0] = fConst717 * (fConst719 * fRec451[0] + fConst740 * fRec451[1] + fConst719 * fRec451[2]) - fConst715 * (fConst741 * fRec450[2] + fConst742 * fRec450[1]);
			fRec449[0] = fConst715 * (fConst716 * fRec450[0] + fConst743 * fRec450[1] + fConst716 * fRec450[2]) - fConst710 * (fConst744 * fRec449[2] + fConst745 * fRec449[1]);
			double fTemp78 = fConst746 * fRec449[1];
			fRec473[0] = fTemp0 - fConst766 * (fConst769 * fRec473[2] + fConst770 * fRec473[1]);
			fRec472[0] = fConst766 * (fConst768 * fRec473[0] + fConst771 * fRec473[1] + fConst768 * fRec473[2]) - fConst763 * (fConst772 * fRec472[2] + fConst773 * fRec472[1]);
			fRec471[0] = fConst763 * (fConst765 * fRec472[0] + fConst774 * fRec472[1] + fConst765 * fRec472[2]) - fConst760 * (fConst775 * fRec471[2] + fConst776 * fRec471[1]);
			fRec470[0] = fConst760 * (fConst762 * fRec471[0] + fConst777 * fRec471[1] + fConst762 * fRec471[2]) - fConst757 * (fConst778 * fRec470[2] + fConst779 * fRec470[1]);
			fRec469[0] = fConst757 * (fConst759 * fRec470[0] + fConst780 * fRec470[1] + fConst759 * fRec470[2]) - fConst755 * (fConst781 * fRec469[2] + fConst782 * fRec469[1]);
			fRec468[0] = fConst755 * (fConst756 * fRec469[0] + fConst783 * fRec469[1] + fConst756 * fRec469[2]) - fConst750 * (fConst784 * fRec468[2] + fConst785 * fRec468[1]);
			fRec467[0] = fConst751 * (fConst754 * fRec468[0] + fConst786 * fRec468[1] + fConst754 * fRec468[2]) - fConst766 * (fConst769 * fRec467[2] + fConst770 * fRec467[1]);
			fRec466[0] = fConst766 * (fConst768 * fRec467[0] + fConst771 * fRec467[1] + fConst768 * fRec467[2]) - fConst763 * (fConst772 * fRec466[2] + fConst773 * fRec466[1]);
			fRec465[0] = fConst763 * (fConst765 * fRec466[0] + fConst774 * fRec466[1] + fConst765 * fRec466[2]) - fConst760 * (fConst775 * fRec465[2] + fConst776 * fRec465[1]);
			fRec464[0] = fConst760 * (fConst762 * fRec465[0] + fConst777 * fRec465[1] + fConst762 * fRec465[2]) - fConst757 * (fConst778 * fRec464[2] + fConst779 * fRec464[1]);
			fRec463[0] = fConst757 * (fConst759 * fRec464[0] + fConst780 * fRec464[1] + fConst759 * fRec464[2]) - fConst755 * (fConst781 * fRec463[2] + fConst782 * fRec463[1]);
			fRec462[0] = fConst755 * (fConst756 * fRec463[0] + fConst783 * fRec463[1] + fConst756 * fRec463[2]) - fConst750 * (fConst784 * fRec462[2] + fConst785 * fRec462[1]);
			double fTemp79 = std::fabs(fConst751 * (fConst754 * fRec462[0] + fConst786 * fRec462[1] + fConst754 * fRec462[2]));
			double fTemp80 = ((fTemp79 > fRec461[1]) ? fTemp5 : fTemp3);
			fRec461[0] = fTemp79 * (1.0 - fTemp80) + fRec461[1] * fTemp80;
			double fTemp81 = tanh(fRec461[0]);
			fRec485[0] = fTemp9 - fConst766 * (fConst769 * fRec485[2] + fConst770 * fRec485[1]);
			fRec484[0] = fConst766 * (fConst768 * fRec485[0] + fConst771 * fRec485[1] + fConst768 * fRec485[2]) - fConst763 * (fConst772 * fRec484[2] + fConst773 * fRec484[1]);
			fRec483[0] = fConst763 * (fConst765 * fRec484[0] + fConst774 * fRec484[1] + fConst765 * fRec484[2]) - fConst760 * (fConst775 * fRec483[2] + fConst776 * fRec483[1]);
			fRec482[0] = fConst760 * (fConst762 * fRec483[0] + fConst777 * fRec483[1] + fConst762 * fRec483[2]) - fConst757 * (fConst778 * fRec482[2] + fConst779 * fRec482[1]);
			fRec481[0] = fConst757 * (fConst759 * fRec482[0] + fConst780 * fRec482[1] + fConst759 * fRec482[2]) - fConst755 * (fConst781 * fRec481[2] + fConst782 * fRec481[1]);
			fRec480[0] = fConst755 * (fConst756 * fRec481[0] + fConst783 * fRec481[1] + fConst756 * fRec481[2]) - fConst750 * (fConst784 * fRec480[2] + fConst785 * fRec480[1]);
			fRec479[0] = fConst751 * fTemp81 * (fConst754 * fRec480[0] + fConst786 * fRec480[1] + fConst754 * fRec480[2]) - fConst766 * (fConst769 * fRec479[2] + fConst770 * fRec479[1]);
			fRec478[0] = fConst766 * (fConst768 * fRec479[0] + fConst771 * fRec479[1] + fConst768 * fRec479[2]) - fConst763 * (fConst772 * fRec478[2] + fConst773 * fRec478[1]);
			fRec477[0] = fConst763 * (fConst765 * fRec478[0] + fConst774 * fRec478[1] + fConst765 * fRec478[2]) - fConst760 * (fConst775 * fRec477[2] + fConst776 * fRec477[1]);
			fRec476[0] = fConst760 * (fConst762 * fRec477[0] + fConst777 * fRec477[1] + fConst762 * fRec477[2]) - fConst757 * (fConst778 * fRec476[2] + fConst779 * fRec476[1]);
			fRec475[0] = fConst757 * (fConst759 * fRec476[0] + fConst780 * fRec476[1] + fConst759 * fRec476[2]) - fConst755 * (fConst781 * fRec475[2] + fConst782 * fRec475[1]);
			fRec474[0] = fConst755 * (fConst756 * fRec475[0] + fConst783 * fRec475[1] + fConst756 * fRec475[2]) - fConst750 * (fConst784 * fRec474[2] + fConst785 * fRec474[1]);
			double fTemp82 = fConst786 * fRec474[1];
			fRec498[0] = fTemp0 - fConst806 * (fConst809 * fRec498[2] + fConst810 * fRec498[1]);
			fRec497[0] = fConst806 * (fConst808 * fRec498[0] + fConst811 * fRec498[1] + fConst808 * fRec498[2]) - fConst803 * (fConst812 * fRec497[2] + fConst813 * fRec497[1]);
			fRec496[0] = fConst803 * (fConst805 * fRec497[0] + fConst814 * fRec497[1] + fConst805 * fRec497[2]) - fConst800 * (fConst815 * fRec496[2] + fConst816 * fRec496[1]);
			fRec495[0] = fConst800 * (fConst802 * fRec496[0] + fConst817 * fRec496[1] + fConst802 * fRec496[2]) - fConst797 * (fConst818 * fRec495[2] + fConst819 * fRec495[1]);
			fRec494[0] = fConst797 * (fConst799 * fRec495[0] + fConst820 * fRec495[1] + fConst799 * fRec495[2]) - fConst795 * (fConst821 * fRec494[2] + fConst822 * fRec494[1]);
			fRec493[0] = fConst795 * (fConst796 * fRec494[0] + fConst823 * fRec494[1] + fConst796 * fRec494[2]) - fConst790 * (fConst824 * fRec493[2] + fConst825 * fRec493[1]);
			fRec492[0] = fConst791 * (fConst794 * fRec493[0] + fConst826 * fRec493[1] + fConst794 * fRec493[2]) - fConst806 * (fConst809 * fRec492[2] + fConst810 * fRec492[1]);
			fRec491[0] = fConst806 * (fConst808 * fRec492[0] + fConst811 * fRec492[1] + fConst808 * fRec492[2]) - fConst803 * (fConst812 * fRec491[2] + fConst813 * fRec491[1]);
			fRec490[0] = fConst803 * (fConst805 * fRec491[0] + fConst814 * fRec491[1] + fConst805 * fRec491[2]) - fConst800 * (fConst815 * fRec490[2] + fConst816 * fRec490[1]);
			fRec489[0] = fConst800 * (fConst802 * fRec490[0] + fConst817 * fRec490[1] + fConst802 * fRec490[2]) - fConst797 * (fConst818 * fRec489[2] + fConst819 * fRec489[1]);
			fRec488[0] = fConst797 * (fConst799 * fRec489[0] + fConst820 * fRec489[1] + fConst799 * fRec489[2]) - fConst795 * (fConst821 * fRec488[2] + fConst822 * fRec488[1]);
			fRec487[0] = fConst795 * (fConst796 * fRec488[0] + fConst823 * fRec488[1] + fConst796 * fRec488[2]) - fConst790 * (fConst824 * fRec487[2] + fConst825 * fRec487[1]);
			double fTemp83 = std::fabs(fConst791 * (fConst794 * fRec487[0] + fConst826 * fRec487[1] + fConst794 * fRec487[2]));
			double fTemp84 = ((fTemp83 > fRec486[1]) ? fTemp5 : fTemp3);
			fRec486[0] = fTemp83 * (1.0 - fTemp84) + fRec486[1] * fTemp84;
			double fTemp85 = tanh(fRec486[0]);
			fRec510[0] = fTemp9 - fConst806 * (fConst809 * fRec510[2] + fConst810 * fRec510[1]);
			fRec509[0] = fConst806 * (fConst808 * fRec510[0] + fConst811 * fRec510[1] + fConst808 * fRec510[2]) - fConst803 * (fConst812 * fRec509[2] + fConst813 * fRec509[1]);
			fRec508[0] = fConst803 * (fConst805 * fRec509[0] + fConst814 * fRec509[1] + fConst805 * fRec509[2]) - fConst800 * (fConst815 * fRec508[2] + fConst816 * fRec508[1]);
			fRec507[0] = fConst800 * (fConst802 * fRec508[0] + fConst817 * fRec508[1] + fConst802 * fRec508[2]) - fConst797 * (fConst818 * fRec507[2] + fConst819 * fRec507[1]);
			fRec506[0] = fConst797 * (fConst799 * fRec507[0] + fConst820 * fRec507[1] + fConst799 * fRec507[2]) - fConst795 * (fConst821 * fRec506[2] + fConst822 * fRec506[1]);
			fRec505[0] = fConst795 * (fConst796 * fRec506[0] + fConst823 * fRec506[1] + fConst796 * fRec506[2]) - fConst790 * (fConst824 * fRec505[2] + fConst825 * fRec505[1]);
			fRec504[0] = fConst791 * fTemp85 * (fConst794 * fRec505[0] + fConst826 * fRec505[1] + fConst794 * fRec505[2]) - fConst806 * (fConst809 * fRec504[2] + fConst810 * fRec504[1]);
			fRec503[0] = fConst806 * (fConst808 * fRec504[0] + fConst811 * fRec504[1] + fConst808 * fRec504[2]) - fConst803 * (fConst812 * fRec503[2] + fConst813 * fRec503[1]);
			fRec502[0] = fConst803 * (fConst805 * fRec503[0] + fConst814 * fRec503[1] + fConst805 * fRec503[2]) - fConst800 * (fConst815 * fRec502[2] + fConst816 * fRec502[1]);
			fRec501[0] = fConst800 * (fConst802 * fRec502[0] + fConst817 * fRec502[1] + fConst802 * fRec502[2]) - fConst797 * (fConst818 * fRec501[2] + fConst819 * fRec501[1]);
			fRec500[0] = fConst797 * (fConst799 * fRec501[0] + fConst820 * fRec501[1] + fConst799 * fRec501[2]) - fConst795 * (fConst821 * fRec500[2] + fConst822 * fRec500[1]);
			fRec499[0] = fConst795 * (fConst796 * fRec500[0] + fConst823 * fRec500[1] + fConst796 * fRec500[2]) - fConst790 * (fConst824 * fRec499[2] + fConst825 * fRec499[1]);
			double fTemp86 = fConst826 * fRec499[1];
			fRec523[0] = fTemp0 - fConst846 * (fConst849 * fRec523[2] + fConst850 * fRec523[1]);
			fRec522[0] = fConst846 * (fConst848 * fRec523[0] + fConst851 * fRec523[1] + fConst848 * fRec523[2]) - fConst843 * (fConst852 * fRec522[2] + fConst853 * fRec522[1]);
			fRec521[0] = fConst843 * (fConst845 * fRec522[0] + fConst854 * fRec522[1] + fConst845 * fRec522[2]) - fConst840 * (fConst855 * fRec521[2] + fConst856 * fRec521[1]);
			fRec520[0] = fConst840 * (fConst842 * fRec521[0] + fConst857 * fRec521[1] + fConst842 * fRec521[2]) - fConst837 * (fConst858 * fRec520[2] + fConst859 * fRec520[1]);
			fRec519[0] = fConst837 * (fConst839 * fRec520[0] + fConst860 * fRec520[1] + fConst839 * fRec520[2]) - fConst835 * (fConst861 * fRec519[2] + fConst862 * fRec519[1]);
			fRec518[0] = fConst835 * (fConst836 * fRec519[0] + fConst863 * fRec519[1] + fConst836 * fRec519[2]) - fConst830 * (fConst864 * fRec518[2] + fConst865 * fRec518[1]);
			fRec517[0] = fConst831 * (fConst834 * fRec518[0] + fConst866 * fRec518[1] + fConst834 * fRec518[2]) - fConst846 * (fConst849 * fRec517[2] + fConst850 * fRec517[1]);
			fRec516[0] = fConst846 * (fConst848 * fRec517[0] + fConst851 * fRec517[1] + fConst848 * fRec517[2]) - fConst843 * (fConst852 * fRec516[2] + fConst853 * fRec516[1]);
			fRec515[0] = fConst843 * (fConst845 * fRec516[0] + fConst854 * fRec516[1] + fConst845 * fRec516[2]) - fConst840 * (fConst855 * fRec515[2] + fConst856 * fRec515[1]);
			fRec514[0] = fConst840 * (fConst842 * fRec515[0] + fConst857 * fRec515[1] + fConst842 * fRec515[2]) - fConst837 * (fConst858 * fRec514[2] + fConst859 * fRec514[1]);
			fRec513[0] = fConst837 * (fConst839 * fRec514[0] + fConst860 * fRec514[1] + fConst839 * fRec514[2]) - fConst835 * (fConst861 * fRec513[2] + fConst862 * fRec513[1]);
			fRec512[0] = fConst835 * (fConst836 * fRec513[0] + fConst863 * fRec513[1] + fConst836 * fRec513[2]) - fConst830 * (fConst864 * fRec512[2] + fConst865 * fRec512[1]);
			double fTemp87 = std::fabs(fConst831 * (fConst834 * fRec512[0] + fConst866 * fRec512[1] + fConst834 * fRec512[2]));
			double fTemp88 = ((fTemp87 > fRec511[1]) ? fTemp5 : fTemp3);
			fRec511[0] = fTemp87 * (1.0 - fTemp88) + fRec511[1] * fTemp88;
			double fTemp89 = tanh(fRec511[0]);
			fRec535[0] = fTemp9 - fConst846 * (fConst849 * fRec535[2] + fConst850 * fRec535[1]);
			fRec534[0] = fConst846 * (fConst848 * fRec535[0] + fConst851 * fRec535[1] + fConst848 * fRec535[2]) - fConst843 * (fConst852 * fRec534[2] + fConst853 * fRec534[1]);
			fRec533[0] = fConst843 * (fConst845 * fRec534[0] + fConst854 * fRec534[1] + fConst845 * fRec534[2]) - fConst840 * (fConst855 * fRec533[2] + fConst856 * fRec533[1]);
			fRec532[0] = fConst840 * (fConst842 * fRec533[0] + fConst857 * fRec533[1] + fConst842 * fRec533[2]) - fConst837 * (fConst858 * fRec532[2] + fConst859 * fRec532[1]);
			fRec531[0] = fConst837 * (fConst839 * fRec532[0] + fConst860 * fRec532[1] + fConst839 * fRec532[2]) - fConst835 * (fConst861 * fRec531[2] + fConst862 * fRec531[1]);
			fRec530[0] = fConst835 * (fConst836 * fRec531[0] + fConst863 * fRec531[1] + fConst836 * fRec531[2]) - fConst830 * (fConst864 * fRec530[2] + fConst865 * fRec530[1]);
			fRec529[0] = fConst831 * fTemp89 * (fConst834 * fRec530[0] + fConst866 * fRec530[1] + fConst834 * fRec530[2]) - fConst846 * (fConst849 * fRec529[2] + fConst850 * fRec529[1]);
			fRec528[0] = fConst846 * (fConst848 * fRec529[0] + fConst851 * fRec529[1] + fConst848 * fRec529[2]) - fConst843 * (fConst852 * fRec528[2] + fConst853 * fRec528[1]);
			fRec527[0] = fConst843 * (fConst845 * fRec528[0] + fConst854 * fRec528[1] + fConst845 * fRec528[2]) - fConst840 * (fConst855 * fRec527[2] + fConst856 * fRec527[1]);
			fRec526[0] = fConst840 * (fConst842 * fRec527[0] + fConst857 * fRec527[1] + fConst842 * fRec527[2]) - fConst837 * (fConst858 * fRec526[2] + fConst859 * fRec526[1]);
			fRec525[0] = fConst837 * (fConst839 * fRec526[0] + fConst860 * fRec526[1] + fConst839 * fRec526[2]) - fConst835 * (fConst861 * fRec525[2] + fConst862 * fRec525[1]);
			fRec524[0] = fConst835 * (fConst836 * fRec525[0] + fConst863 * fRec525[1] + fConst836 * fRec525[2]) - fConst830 * (fConst864 * fRec524[2] + fConst865 * fRec524[1]);
			double fTemp90 = fConst866 * fRec524[1];
			fRec548[0] = fTemp0 - fConst886 * (fConst889 * fRec548[2] + fConst890 * fRec548[1]);
			fRec547[0] = fConst886 * (fConst888 * fRec548[0] + fConst891 * fRec548[1] + fConst888 * fRec548[2]) - fConst883 * (fConst892 * fRec547[2] + fConst893 * fRec547[1]);
			fRec546[0] = fConst883 * (fConst885 * fRec547[0] + fConst894 * fRec547[1] + fConst885 * fRec547[2]) - fConst880 * (fConst895 * fRec546[2] + fConst896 * fRec546[1]);
			fRec545[0] = fConst880 * (fConst882 * fRec546[0] + fConst897 * fRec546[1] + fConst882 * fRec546[2]) - fConst877 * (fConst898 * fRec545[2] + fConst899 * fRec545[1]);
			fRec544[0] = fConst877 * (fConst879 * fRec545[0] + fConst900 * fRec545[1] + fConst879 * fRec545[2]) - fConst875 * (fConst901 * fRec544[2] + fConst902 * fRec544[1]);
			fRec543[0] = fConst875 * (fConst876 * fRec544[0] + fConst903 * fRec544[1] + fConst876 * fRec544[2]) - fConst870 * (fConst904 * fRec543[2] + fConst905 * fRec543[1]);
			fRec542[0] = fConst871 * (fConst874 * fRec543[0] + fConst906 * fRec543[1] + fConst874 * fRec543[2]) - fConst886 * (fConst889 * fRec542[2] + fConst890 * fRec542[1]);
			fRec541[0] = fConst886 * (fConst888 * fRec542[0] + fConst891 * fRec542[1] + fConst888 * fRec542[2]) - fConst883 * (fConst892 * fRec541[2] + fConst893 * fRec541[1]);
			fRec540[0] = fConst883 * (fConst885 * fRec541[0] + fConst894 * fRec541[1] + fConst885 * fRec541[2]) - fConst880 * (fConst895 * fRec540[2] + fConst896 * fRec540[1]);
			fRec539[0] = fConst880 * (fConst882 * fRec540[0] + fConst897 * fRec540[1] + fConst882 * fRec540[2]) - fConst877 * (fConst898 * fRec539[2] + fConst899 * fRec539[1]);
			fRec538[0] = fConst877 * (fConst879 * fRec539[0] + fConst900 * fRec539[1] + fConst879 * fRec539[2]) - fConst875 * (fConst901 * fRec538[2] + fConst902 * fRec538[1]);
			fRec537[0] = fConst875 * (fConst876 * fRec538[0] + fConst903 * fRec538[1] + fConst876 * fRec538[2]) - fConst870 * (fConst904 * fRec537[2] + fConst905 * fRec537[1]);
			double fTemp91 = std::fabs(fConst871 * (fConst874 * fRec537[0] + fConst906 * fRec537[1] + fConst874 * fRec537[2]));
			double fTemp92 = ((fTemp91 > fRec536[1]) ? fTemp5 : fTemp3);
			fRec536[0] = fTemp91 * (1.0 - fTemp92) + fRec536[1] * fTemp92;
			double fTemp93 = tanh(fRec536[0]);
			fRec560[0] = fTemp9 - fConst886 * (fConst889 * fRec560[2] + fConst890 * fRec560[1]);
			fRec559[0] = fConst886 * (fConst888 * fRec560[0] + fConst891 * fRec560[1] + fConst888 * fRec560[2]) - fConst883 * (fConst892 * fRec559[2] + fConst893 * fRec559[1]);
			fRec558[0] = fConst883 * (fConst885 * fRec559[0] + fConst894 * fRec559[1] + fConst885 * fRec559[2]) - fConst880 * (fConst895 * fRec558[2] + fConst896 * fRec558[1]);
			fRec557[0] = fConst880 * (fConst882 * fRec558[0] + fConst897 * fRec558[1] + fConst882 * fRec558[2]) - fConst877 * (fConst898 * fRec557[2] + fConst899 * fRec557[1]);
			fRec556[0] = fConst877 * (fConst879 * fRec557[0] + fConst900 * fRec557[1] + fConst879 * fRec557[2]) - fConst875 * (fConst901 * fRec556[2] + fConst902 * fRec556[1]);
			fRec555[0] = fConst875 * (fConst876 * fRec556[0] + fConst903 * fRec556[1] + fConst876 * fRec556[2]) - fConst870 * (fConst904 * fRec555[2] + fConst905 * fRec555[1]);
			fRec554[0] = fConst871 * fTemp93 * (fConst874 * fRec555[0] + fConst906 * fRec555[1] + fConst874 * fRec555[2]) - fConst886 * (fConst889 * fRec554[2] + fConst890 * fRec554[1]);
			fRec553[0] = fConst886 * (fConst888 * fRec554[0] + fConst891 * fRec554[1] + fConst888 * fRec554[2]) - fConst883 * (fConst892 * fRec553[2] + fConst893 * fRec553[1]);
			fRec552[0] = fConst883 * (fConst885 * fRec553[0] + fConst894 * fRec553[1] + fConst885 * fRec553[2]) - fConst880 * (fConst895 * fRec552[2] + fConst896 * fRec552[1]);
			fRec551[0] = fConst880 * (fConst882 * fRec552[0] + fConst897 * fRec552[1] + fConst882 * fRec552[2]) - fConst877 * (fConst898 * fRec551[2] + fConst899 * fRec551[1]);
			fRec550[0] = fConst877 * (fConst879 * fRec551[0] + fConst900 * fRec551[1] + fConst879 * fRec551[2]) - fConst875 * (fConst901 * fRec550[2] + fConst902 * fRec550[1]);
			fRec549[0] = fConst875 * (fConst876 * fRec550[0] + fConst903 * fRec550[1] + fConst876 * fRec550[2]) - fConst870 * (fConst904 * fRec549[2] + fConst905 * fRec549[1]);
			double fTemp94 = fConst906 * fRec549[1];
			fRec573[0] = fTemp0 - fConst926 * (fConst929 * fRec573[2] + fConst930 * fRec573[1]);
			fRec572[0] = fConst926 * (fConst928 * fRec573[0] + fConst931 * fRec573[1] + fConst928 * fRec573[2]) - fConst923 * (fConst932 * fRec572[2] + fConst933 * fRec572[1]);
			fRec571[0] = fConst923 * (fConst925 * fRec572[0] + fConst934 * fRec572[1] + fConst925 * fRec572[2]) - fConst920 * (fConst935 * fRec571[2] + fConst936 * fRec571[1]);
			fRec570[0] = fConst920 * (fConst922 * fRec571[0] + fConst937 * fRec571[1] + fConst922 * fRec571[2]) - fConst917 * (fConst938 * fRec570[2] + fConst939 * fRec570[1]);
			fRec569[0] = fConst917 * (fConst919 * fRec570[0] + fConst940 * fRec570[1] + fConst919 * fRec570[2]) - fConst915 * (fConst941 * fRec569[2] + fConst942 * fRec569[1]);
			fRec568[0] = fConst915 * (fConst916 * fRec569[0] + fConst943 * fRec569[1] + fConst916 * fRec569[2]) - fConst910 * (fConst944 * fRec568[2] + fConst945 * fRec568[1]);
			fRec567[0] = fConst911 * (fConst914 * fRec568[0] + fConst946 * fRec568[1] + fConst914 * fRec568[2]) - fConst926 * (fConst929 * fRec567[2] + fConst930 * fRec567[1]);
			fRec566[0] = fConst926 * (fConst928 * fRec567[0] + fConst931 * fRec567[1] + fConst928 * fRec567[2]) - fConst923 * (fConst932 * fRec566[2] + fConst933 * fRec566[1]);
			fRec565[0] = fConst923 * (fConst925 * fRec566[0] + fConst934 * fRec566[1] + fConst925 * fRec566[2]) - fConst920 * (fConst935 * fRec565[2] + fConst936 * fRec565[1]);
			fRec564[0] = fConst920 * (fConst922 * fRec565[0] + fConst937 * fRec565[1] + fConst922 * fRec565[2]) - fConst917 * (fConst938 * fRec564[2] + fConst939 * fRec564[1]);
			fRec563[0] = fConst917 * (fConst919 * fRec564[0] + fConst940 * fRec564[1] + fConst919 * fRec564[2]) - fConst915 * (fConst941 * fRec563[2] + fConst942 * fRec563[1]);
			fRec562[0] = fConst915 * (fConst916 * fRec563[0] + fConst943 * fRec563[1] + fConst916 * fRec563[2]) - fConst910 * (fConst944 * fRec562[2] + fConst945 * fRec562[1]);
			double fTemp95 = std::fabs(fConst911 * (fConst914 * fRec562[0] + fConst946 * fRec562[1] + fConst914 * fRec562[2]));
			double fTemp96 = ((fTemp95 > fRec561[1]) ? fTemp5 : fTemp3);
			fRec561[0] = fTemp95 * (1.0 - fTemp96) + fRec561[1] * fTemp96;
			double fTemp97 = tanh(fRec561[0]);
			fRec585[0] = fTemp9 - fConst926 * (fConst929 * fRec585[2] + fConst930 * fRec585[1]);
			fRec584[0] = fConst926 * (fConst928 * fRec585[0] + fConst931 * fRec585[1] + fConst928 * fRec585[2]) - fConst923 * (fConst932 * fRec584[2] + fConst933 * fRec584[1]);
			fRec583[0] = fConst923 * (fConst925 * fRec584[0] + fConst934 * fRec584[1] + fConst925 * fRec584[2]) - fConst920 * (fConst935 * fRec583[2] + fConst936 * fRec583[1]);
			fRec582[0] = fConst920 * (fConst922 * fRec583[0] + fConst937 * fRec583[1] + fConst922 * fRec583[2]) - fConst917 * (fConst938 * fRec582[2] + fConst939 * fRec582[1]);
			fRec581[0] = fConst917 * (fConst919 * fRec582[0] + fConst940 * fRec582[1] + fConst919 * fRec582[2]) - fConst915 * (fConst941 * fRec581[2] + fConst942 * fRec581[1]);
			fRec580[0] = fConst915 * (fConst916 * fRec581[0] + fConst943 * fRec581[1] + fConst916 * fRec581[2]) - fConst910 * (fConst944 * fRec580[2] + fConst945 * fRec580[1]);
			fRec579[0] = fConst911 * fTemp97 * (fConst914 * fRec580[0] + fConst946 * fRec580[1] + fConst914 * fRec580[2]) - fConst926 * (fConst929 * fRec579[2] + fConst930 * fRec579[1]);
			fRec578[0] = fConst926 * (fConst928 * fRec579[0] + fConst931 * fRec579[1] + fConst928 * fRec579[2]) - fConst923 * (fConst932 * fRec578[2] + fConst933 * fRec578[1]);
			fRec577[0] = fConst923 * (fConst925 * fRec578[0] + fConst934 * fRec578[1] + fConst925 * fRec578[2]) - fConst920 * (fConst935 * fRec577[2] + fConst936 * fRec577[1]);
			fRec576[0] = fConst920 * (fConst922 * fRec577[0] + fConst937 * fRec577[1] + fConst922 * fRec577[2]) - fConst917 * (fConst938 * fRec576[2] + fConst939 * fRec576[1]);
			fRec575[0] = fConst917 * (fConst919 * fRec576[0] + fConst940 * fRec576[1] + fConst919 * fRec576[2]) - fConst915 * (fConst941 * fRec575[2] + fConst942 * fRec575[1]);
			fRec574[0] = fConst915 * (fConst916 * fRec575[0] + fConst943 * fRec575[1] + fConst916 * fRec575[2]) - fConst910 * (fConst944 * fRec574[2] + fConst945 * fRec574[1]);
			double fTemp98 = fConst946 * fRec574[1];
			fRec598[0] = fTemp0 - fConst966 * (fConst969 * fRec598[2] + fConst970 * fRec598[1]);
			fRec597[0] = fConst966 * (fConst968 * fRec598[0] + fConst971 * fRec598[1] + fConst968 * fRec598[2]) - fConst963 * (fConst972 * fRec597[2] + fConst973 * fRec597[1]);
			fRec596[0] = fConst963 * (fConst965 * fRec597[0] + fConst974 * fRec597[1] + fConst965 * fRec597[2]) - fConst960 * (fConst975 * fRec596[2] + fConst976 * fRec596[1]);
			fRec595[0] = fConst960 * (fConst962 * fRec596[0] + fConst977 * fRec596[1] + fConst962 * fRec596[2]) - fConst957 * (fConst978 * fRec595[2] + fConst979 * fRec595[1]);
			fRec594[0] = fConst957 * (fConst959 * fRec595[0] + fConst980 * fRec595[1] + fConst959 * fRec595[2]) - fConst955 * (fConst981 * fRec594[2] + fConst982 * fRec594[1]);
			fRec593[0] = fConst955 * (fConst956 * fRec594[0] + fConst983 * fRec594[1] + fConst956 * fRec594[2]) - fConst950 * (fConst984 * fRec593[2] + fConst985 * fRec593[1]);
			fRec592[0] = fConst951 * (fConst954 * fRec593[0] + fConst986 * fRec593[1] + fConst954 * fRec593[2]) - fConst966 * (fConst969 * fRec592[2] + fConst970 * fRec592[1]);
			fRec591[0] = fConst966 * (fConst968 * fRec592[0] + fConst971 * fRec592[1] + fConst968 * fRec592[2]) - fConst963 * (fConst972 * fRec591[2] + fConst973 * fRec591[1]);
			fRec590[0] = fConst963 * (fConst965 * fRec591[0] + fConst974 * fRec591[1] + fConst965 * fRec591[2]) - fConst960 * (fConst975 * fRec590[2] + fConst976 * fRec590[1]);
			fRec589[0] = fConst960 * (fConst962 * fRec590[0] + fConst977 * fRec590[1] + fConst962 * fRec590[2]) - fConst957 * (fConst978 * fRec589[2] + fConst979 * fRec589[1]);
			fRec588[0] = fConst957 * (fConst959 * fRec589[0] + fConst980 * fRec589[1] + fConst959 * fRec589[2]) - fConst955 * (fConst981 * fRec588[2] + fConst982 * fRec588[1]);
			fRec587[0] = fConst955 * (fConst956 * fRec588[0] + fConst983 * fRec588[1] + fConst956 * fRec588[2]) - fConst950 * (fConst984 * fRec587[2] + fConst985 * fRec587[1]);
			double fTemp99 = std::fabs(fConst951 * (fConst954 * fRec587[0] + fConst986 * fRec587[1] + fConst954 * fRec587[2]));
			double fTemp100 = ((fTemp99 > fRec586[1]) ? fTemp5 : fTemp3);
			fRec586[0] = fTemp99 * (1.0 - fTemp100) + fRec586[1] * fTemp100;
			double fTemp101 = tanh(fRec586[0]);
			fRec610[0] = fTemp9 - fConst966 * (fConst969 * fRec610[2] + fConst970 * fRec610[1]);
			fRec609[0] = fConst966 * (fConst968 * fRec610[0] + fConst971 * fRec610[1] + fConst968 * fRec610[2]) - fConst963 * (fConst972 * fRec609[2] + fConst973 * fRec609[1]);
			fRec608[0] = fConst963 * (fConst965 * fRec609[0] + fConst974 * fRec609[1] + fConst965 * fRec609[2]) - fConst960 * (fConst975 * fRec608[2] + fConst976 * fRec608[1]);
			fRec607[0] = fConst960 * (fConst962 * fRec608[0] + fConst977 * fRec608[1] + fConst962 * fRec608[2]) - fConst957 * (fConst978 * fRec607[2] + fConst979 * fRec607[1]);
			fRec606[0] = fConst957 * (fConst959 * fRec607[0] + fConst980 * fRec607[1] + fConst959 * fRec607[2]) - fConst955 * (fConst981 * fRec606[2] + fConst982 * fRec606[1]);
			fRec605[0] = fConst955 * (fConst956 * fRec606[0] + fConst983 * fRec606[1] + fConst956 * fRec606[2]) - fConst950 * (fConst984 * fRec605[2] + fConst985 * fRec605[1]);
			fRec604[0] = fConst951 * fTemp101 * (fConst954 * fRec605[0] + fConst986 * fRec605[1] + fConst954 * fRec605[2]) - fConst966 * (fConst969 * fRec604[2] + fConst970 * fRec604[1]);
			fRec603[0] = fConst966 * (fConst968 * fRec604[0] + fConst971 * fRec604[1] + fConst968 * fRec604[2]) - fConst963 * (fConst972 * fRec603[2] + fConst973 * fRec603[1]);
			fRec602[0] = fConst963 * (fConst965 * fRec603[0] + fConst974 * fRec603[1] + fConst965 * fRec603[2]) - fConst960 * (fConst975 * fRec602[2] + fConst976 * fRec602[1]);
			fRec601[0] = fConst960 * (fConst962 * fRec602[0] + fConst977 * fRec602[1] + fConst962 * fRec602[2]) - fConst957 * (fConst978 * fRec601[2] + fConst979 * fRec601[1]);
			fRec600[0] = fConst957 * (fConst959 * fRec601[0] + fConst980 * fRec601[1] + fConst959 * fRec601[2]) - fConst955 * (fConst981 * fRec600[2] + fConst982 * fRec600[1]);
			fRec599[0] = fConst955 * (fConst956 * fRec600[0] + fConst983 * fRec600[1] + fConst956 * fRec600[2]) - fConst950 * (fConst984 * fRec599[2] + fConst985 * fRec599[1]);
			double fTemp102 = fConst986 * fRec599[1];
			fRec623[0] = fTemp0 - fConst1006 * (fConst1009 * fRec623[2] + fConst1010 * fRec623[1]);
			fRec622[0] = fConst1006 * (fConst1008 * fRec623[0] + fConst1011 * fRec623[1] + fConst1008 * fRec623[2]) - fConst1003 * (fConst1012 * fRec622[2] + fConst1013 * fRec622[1]);
			fRec621[0] = fConst1003 * (fConst1005 * fRec622[0] + fConst1014 * fRec622[1] + fConst1005 * fRec622[2]) - fConst1000 * (fConst1015 * fRec621[2] + fConst1016 * fRec621[1]);
			fRec620[0] = fConst1000 * (fConst1002 * fRec621[0] + fConst1017 * fRec621[1] + fConst1002 * fRec621[2]) - fConst997 * (fConst1018 * fRec620[2] + fConst1019 * fRec620[1]);
			fRec619[0] = fConst997 * (fConst999 * fRec620[0] + fConst1020 * fRec620[1] + fConst999 * fRec620[2]) - fConst995 * (fConst1021 * fRec619[2] + fConst1022 * fRec619[1]);
			fRec618[0] = fConst995 * (fConst996 * fRec619[0] + fConst1023 * fRec619[1] + fConst996 * fRec619[2]) - fConst990 * (fConst1024 * fRec618[2] + fConst1025 * fRec618[1]);
			fRec617[0] = fConst991 * (fConst994 * fRec618[0] + fConst1026 * fRec618[1] + fConst994 * fRec618[2]) - fConst1006 * (fConst1009 * fRec617[2] + fConst1010 * fRec617[1]);
			fRec616[0] = fConst1006 * (fConst1008 * fRec617[0] + fConst1011 * fRec617[1] + fConst1008 * fRec617[2]) - fConst1003 * (fConst1012 * fRec616[2] + fConst1013 * fRec616[1]);
			fRec615[0] = fConst1003 * (fConst1005 * fRec616[0] + fConst1014 * fRec616[1] + fConst1005 * fRec616[2]) - fConst1000 * (fConst1015 * fRec615[2] + fConst1016 * fRec615[1]);
			fRec614[0] = fConst1000 * (fConst1002 * fRec615[0] + fConst1017 * fRec615[1] + fConst1002 * fRec615[2]) - fConst997 * (fConst1018 * fRec614[2] + fConst1019 * fRec614[1]);
			fRec613[0] = fConst997 * (fConst999 * fRec614[0] + fConst1020 * fRec614[1] + fConst999 * fRec614[2]) - fConst995 * (fConst1021 * fRec613[2] + fConst1022 * fRec613[1]);
			fRec612[0] = fConst995 * (fConst996 * fRec613[0] + fConst1023 * fRec613[1] + fConst996 * fRec613[2]) - fConst990 * (fConst1024 * fRec612[2] + fConst1025 * fRec612[1]);
			double fTemp103 = std::fabs(fConst991 * (fConst994 * fRec612[0] + fConst1026 * fRec612[1] + fConst994 * fRec612[2]));
			double fTemp104 = ((fTemp103 > fRec611[1]) ? fTemp5 : fTemp3);
			fRec611[0] = fTemp103 * (1.0 - fTemp104) + fRec611[1] * fTemp104;
			double fTemp105 = tanh(fRec611[0]);
			fRec635[0] = fTemp9 - fConst1006 * (fConst1009 * fRec635[2] + fConst1010 * fRec635[1]);
			fRec634[0] = fConst1006 * (fConst1008 * fRec635[0] + fConst1011 * fRec635[1] + fConst1008 * fRec635[2]) - fConst1003 * (fConst1012 * fRec634[2] + fConst1013 * fRec634[1]);
			fRec633[0] = fConst1003 * (fConst1005 * fRec634[0] + fConst1014 * fRec634[1] + fConst1005 * fRec634[2]) - fConst1000 * (fConst1015 * fRec633[2] + fConst1016 * fRec633[1]);
			fRec632[0] = fConst1000 * (fConst1002 * fRec633[0] + fConst1017 * fRec633[1] + fConst1002 * fRec633[2]) - fConst997 * (fConst1018 * fRec632[2] + fConst1019 * fRec632[1]);
			fRec631[0] = fConst997 * (fConst999 * fRec632[0] + fConst1020 * fRec632[1] + fConst999 * fRec632[2]) - fConst995 * (fConst1021 * fRec631[2] + fConst1022 * fRec631[1]);
			fRec630[0] = fConst995 * (fConst996 * fRec631[0] + fConst1023 * fRec631[1] + fConst996 * fRec631[2]) - fConst990 * (fConst1024 * fRec630[2] + fConst1025 * fRec630[1]);
			fRec629[0] = fConst991 * fTemp105 * (fConst994 * fRec630[0] + fConst1026 * fRec630[1] + fConst994 * fRec630[2]) - fConst1006 * (fConst1009 * fRec629[2] + fConst1010 * fRec629[1]);
			fRec628[0] = fConst1006 * (fConst1008 * fRec629[0] + fConst1011 * fRec629[1] + fConst1008 * fRec629[2]) - fConst1003 * (fConst1012 * fRec628[2] + fConst1013 * fRec628[1]);
			fRec627[0] = fConst1003 * (fConst1005 * fRec628[0] + fConst1014 * fRec628[1] + fConst1005 * fRec628[2]) - fConst1000 * (fConst1015 * fRec627[2] + fConst1016 * fRec627[1]);
			fRec626[0] = fConst1000 * (fConst1002 * fRec627[0] + fConst1017 * fRec627[1] + fConst1002 * fRec627[2]) - fConst997 * (fConst1018 * fRec626[2] + fConst1019 * fRec626[1]);
			fRec625[0] = fConst997 * (fConst999 * fRec626[0] + fConst1020 * fRec626[1] + fConst999 * fRec626[2]) - fConst995 * (fConst1021 * fRec625[2] + fConst1022 * fRec625[1]);
			fRec624[0] = fConst995 * (fConst996 * fRec625[0] + fConst1023 * fRec625[1] + fConst996 * fRec625[2]) - fConst990 * (fConst1024 * fRec624[2] + fConst1025 * fRec624[1]);
			double fTemp106 = fConst1026 * fRec624[1];
			fRec648[0] = fTemp0 - fConst1046 * (fConst1049 * fRec648[2] + fConst1050 * fRec648[1]);
			fRec647[0] = fConst1046 * (fConst1048 * fRec648[0] + fConst1051 * fRec648[1] + fConst1048 * fRec648[2]) - fConst1043 * (fConst1052 * fRec647[2] + fConst1053 * fRec647[1]);
			fRec646[0] = fConst1043 * (fConst1045 * fRec647[0] + fConst1054 * fRec647[1] + fConst1045 * fRec647[2]) - fConst1040 * (fConst1055 * fRec646[2] + fConst1056 * fRec646[1]);
			fRec645[0] = fConst1040 * (fConst1042 * fRec646[0] + fConst1057 * fRec646[1] + fConst1042 * fRec646[2]) - fConst1037 * (fConst1058 * fRec645[2] + fConst1059 * fRec645[1]);
			fRec644[0] = fConst1037 * (fConst1039 * fRec645[0] + fConst1060 * fRec645[1] + fConst1039 * fRec645[2]) - fConst1035 * (fConst1061 * fRec644[2] + fConst1062 * fRec644[1]);
			fRec643[0] = fConst1035 * (fConst1036 * fRec644[0] + fConst1063 * fRec644[1] + fConst1036 * fRec644[2]) - fConst1030 * (fConst1064 * fRec643[2] + fConst1065 * fRec643[1]);
			fRec642[0] = fConst1031 * (fConst1034 * fRec643[0] + fConst1066 * fRec643[1] + fConst1034 * fRec643[2]) - fConst1046 * (fConst1049 * fRec642[2] + fConst1050 * fRec642[1]);
			fRec641[0] = fConst1046 * (fConst1048 * fRec642[0] + fConst1051 * fRec642[1] + fConst1048 * fRec642[2]) - fConst1043 * (fConst1052 * fRec641[2] + fConst1053 * fRec641[1]);
			fRec640[0] = fConst1043 * (fConst1045 * fRec641[0] + fConst1054 * fRec641[1] + fConst1045 * fRec641[2]) - fConst1040 * (fConst1055 * fRec640[2] + fConst1056 * fRec640[1]);
			fRec639[0] = fConst1040 * (fConst1042 * fRec640[0] + fConst1057 * fRec640[1] + fConst1042 * fRec640[2]) - fConst1037 * (fConst1058 * fRec639[2] + fConst1059 * fRec639[1]);
			fRec638[0] = fConst1037 * (fConst1039 * fRec639[0] + fConst1060 * fRec639[1] + fConst1039 * fRec639[2]) - fConst1035 * (fConst1061 * fRec638[2] + fConst1062 * fRec638[1]);
			fRec637[0] = fConst1035 * (fConst1036 * fRec638[0] + fConst1063 * fRec638[1] + fConst1036 * fRec638[2]) - fConst1030 * (fConst1064 * fRec637[2] + fConst1065 * fRec637[1]);
			double fTemp107 = std::fabs(fConst1031 * (fConst1034 * fRec637[0] + fConst1066 * fRec637[1] + fConst1034 * fRec637[2]));
			double fTemp108 = ((fTemp107 > fRec636[1]) ? fTemp5 : fTemp3);
			fRec636[0] = fTemp107 * (1.0 - fTemp108) + fRec636[1] * fTemp108;
			double fTemp109 = tanh(fRec636[0]);
			fRec660[0] = fTemp9 - fConst1046 * (fConst1049 * fRec660[2] + fConst1050 * fRec660[1]);
			fRec659[0] = fConst1046 * (fConst1048 * fRec660[0] + fConst1051 * fRec660[1] + fConst1048 * fRec660[2]) - fConst1043 * (fConst1052 * fRec659[2] + fConst1053 * fRec659[1]);
			fRec658[0] = fConst1043 * (fConst1045 * fRec659[0] + fConst1054 * fRec659[1] + fConst1045 * fRec659[2]) - fConst1040 * (fConst1055 * fRec658[2] + fConst1056 * fRec658[1]);
			fRec657[0] = fConst1040 * (fConst1042 * fRec658[0] + fConst1057 * fRec658[1] + fConst1042 * fRec658[2]) - fConst1037 * (fConst1058 * fRec657[2] + fConst1059 * fRec657[1]);
			fRec656[0] = fConst1037 * (fConst1039 * fRec657[0] + fConst1060 * fRec657[1] + fConst1039 * fRec657[2]) - fConst1035 * (fConst1061 * fRec656[2] + fConst1062 * fRec656[1]);
			fRec655[0] = fConst1035 * (fConst1036 * fRec656[0] + fConst1063 * fRec656[1] + fConst1036 * fRec656[2]) - fConst1030 * (fConst1064 * fRec655[2] + fConst1065 * fRec655[1]);
			fRec654[0] = fConst1031 * fTemp109 * (fConst1034 * fRec655[0] + fConst1066 * fRec655[1] + fConst1034 * fRec655[2]) - fConst1046 * (fConst1049 * fRec654[2] + fConst1050 * fRec654[1]);
			fRec653[0] = fConst1046 * (fConst1048 * fRec654[0] + fConst1051 * fRec654[1] + fConst1048 * fRec654[2]) - fConst1043 * (fConst1052 * fRec653[2] + fConst1053 * fRec653[1]);
			fRec652[0] = fConst1043 * (fConst1045 * fRec653[0] + fConst1054 * fRec653[1] + fConst1045 * fRec653[2]) - fConst1040 * (fConst1055 * fRec652[2] + fConst1056 * fRec652[1]);
			fRec651[0] = fConst1040 * (fConst1042 * fRec652[0] + fConst1057 * fRec652[1] + fConst1042 * fRec652[2]) - fConst1037 * (fConst1058 * fRec651[2] + fConst1059 * fRec651[1]);
			fRec650[0] = fConst1037 * (fConst1039 * fRec651[0] + fConst1060 * fRec651[1] + fConst1039 * fRec651[2]) - fConst1035 * (fConst1061 * fRec650[2] + fConst1062 * fRec650[1]);
			fRec649[0] = fConst1035 * (fConst1036 * fRec650[0] + fConst1063 * fRec650[1] + fConst1036 * fRec650[2]) - fConst1030 * (fConst1064 * fRec649[2] + fConst1065 * fRec649[1]);
			double fTemp110 = fConst1066 * fRec649[1];
			fRec673[0] = fTemp0 - fConst1086 * (fConst1089 * fRec673[2] + fConst1090 * fRec673[1]);
			fRec672[0] = fConst1086 * (fConst1088 * fRec673[0] + fConst1091 * fRec673[1] + fConst1088 * fRec673[2]) - fConst1083 * (fConst1092 * fRec672[2] + fConst1093 * fRec672[1]);
			fRec671[0] = fConst1083 * (fConst1085 * fRec672[0] + fConst1094 * fRec672[1] + fConst1085 * fRec672[2]) - fConst1080 * (fConst1095 * fRec671[2] + fConst1096 * fRec671[1]);
			fRec670[0] = fConst1080 * (fConst1082 * fRec671[0] + fConst1097 * fRec671[1] + fConst1082 * fRec671[2]) - fConst1077 * (fConst1098 * fRec670[2] + fConst1099 * fRec670[1]);
			fRec669[0] = fConst1077 * (fConst1079 * fRec670[0] + fConst1100 * fRec670[1] + fConst1079 * fRec670[2]) - fConst1075 * (fConst1101 * fRec669[2] + fConst1102 * fRec669[1]);
			fRec668[0] = fConst1075 * (fConst1076 * fRec669[0] + fConst1103 * fRec669[1] + fConst1076 * fRec669[2]) - fConst1070 * (fConst1104 * fRec668[2] + fConst1105 * fRec668[1]);
			fRec667[0] = fConst1071 * (fConst1074 * fRec668[0] + fConst1106 * fRec668[1] + fConst1074 * fRec668[2]) - fConst1086 * (fConst1089 * fRec667[2] + fConst1090 * fRec667[1]);
			fRec666[0] = fConst1086 * (fConst1088 * fRec667[0] + fConst1091 * fRec667[1] + fConst1088 * fRec667[2]) - fConst1083 * (fConst1092 * fRec666[2] + fConst1093 * fRec666[1]);
			fRec665[0] = fConst1083 * (fConst1085 * fRec666[0] + fConst1094 * fRec666[1] + fConst1085 * fRec666[2]) - fConst1080 * (fConst1095 * fRec665[2] + fConst1096 * fRec665[1]);
			fRec664[0] = fConst1080 * (fConst1082 * fRec665[0] + fConst1097 * fRec665[1] + fConst1082 * fRec665[2]) - fConst1077 * (fConst1098 * fRec664[2] + fConst1099 * fRec664[1]);
			fRec663[0] = fConst1077 * (fConst1079 * fRec664[0] + fConst1100 * fRec664[1] + fConst1079 * fRec664[2]) - fConst1075 * (fConst1101 * fRec663[2] + fConst1102 * fRec663[1]);
			fRec662[0] = fConst1075 * (fConst1076 * fRec663[0] + fConst1103 * fRec663[1] + fConst1076 * fRec663[2]) - fConst1070 * (fConst1104 * fRec662[2] + fConst1105 * fRec662[1]);
			double fTemp111 = std::fabs(fConst1071 * (fConst1074 * fRec662[0] + fConst1106 * fRec662[1] + fConst1074 * fRec662[2]));
			double fTemp112 = ((fTemp111 > fRec661[1]) ? fTemp5 : fTemp3);
			fRec661[0] = fTemp111 * (1.0 - fTemp112) + fRec661[1] * fTemp112;
			double fTemp113 = tanh(fRec661[0]);
			fRec685[0] = fTemp9 - fConst1086 * (fConst1089 * fRec685[2] + fConst1090 * fRec685[1]);
			fRec684[0] = fConst1086 * (fConst1088 * fRec685[0] + fConst1091 * fRec685[1] + fConst1088 * fRec685[2]) - fConst1083 * (fConst1092 * fRec684[2] + fConst1093 * fRec684[1]);
			fRec683[0] = fConst1083 * (fConst1085 * fRec684[0] + fConst1094 * fRec684[1] + fConst1085 * fRec684[2]) - fConst1080 * (fConst1095 * fRec683[2] + fConst1096 * fRec683[1]);
			fRec682[0] = fConst1080 * (fConst1082 * fRec683[0] + fConst1097 * fRec683[1] + fConst1082 * fRec683[2]) - fConst1077 * (fConst1098 * fRec682[2] + fConst1099 * fRec682[1]);
			fRec681[0] = fConst1077 * (fConst1079 * fRec682[0] + fConst1100 * fRec682[1] + fConst1079 * fRec682[2]) - fConst1075 * (fConst1101 * fRec681[2] + fConst1102 * fRec681[1]);
			fRec680[0] = fConst1075 * (fConst1076 * fRec681[0] + fConst1103 * fRec681[1] + fConst1076 * fRec681[2]) - fConst1070 * (fConst1104 * fRec680[2] + fConst1105 * fRec680[1]);
			fRec679[0] = fConst1071 * fTemp113 * (fConst1074 * fRec680[0] + fConst1106 * fRec680[1] + fConst1074 * fRec680[2]) - fConst1086 * (fConst1089 * fRec679[2] + fConst1090 * fRec679[1]);
			fRec678[0] = fConst1086 * (fConst1088 * fRec679[0] + fConst1091 * fRec679[1] + fConst1088 * fRec679[2]) - fConst1083 * (fConst1092 * fRec678[2] + fConst1093 * fRec678[1]);
			fRec677[0] = fConst1083 * (fConst1085 * fRec678[0] + fConst1094 * fRec678[1] + fConst1085 * fRec678[2]) - fConst1080 * (fConst1095 * fRec677[2] + fConst1096 * fRec677[1]);
			fRec676[0] = fConst1080 * (fConst1082 * fRec677[0] + fConst1097 * fRec677[1] + fConst1082 * fRec677[2]) - fConst1077 * (fConst1098 * fRec676[2] + fConst1099 * fRec676[1]);
			fRec675[0] = fConst1077 * (fConst1079 * fRec676[0] + fConst1100 * fRec676[1] + fConst1079 * fRec676[2]) - fConst1075 * (fConst1101 * fRec675[2] + fConst1102 * fRec675[1]);
			fRec674[0] = fConst1075 * (fConst1076 * fRec675[0] + fConst1103 * fRec675[1] + fConst1076 * fRec675[2]) - fConst1070 * (fConst1104 * fRec674[2] + fConst1105 * fRec674[1]);
			double fTemp114 = fConst1106 * fRec674[1];
			fRec698[0] = fTemp0 - fConst1126 * (fConst1129 * fRec698[2] + fConst1130 * fRec698[1]);
			fRec697[0] = fConst1126 * (fConst1128 * fRec698[0] + fConst1131 * fRec698[1] + fConst1128 * fRec698[2]) - fConst1123 * (fConst1132 * fRec697[2] + fConst1133 * fRec697[1]);
			fRec696[0] = fConst1123 * (fConst1125 * fRec697[0] + fConst1134 * fRec697[1] + fConst1125 * fRec697[2]) - fConst1120 * (fConst1135 * fRec696[2] + fConst1136 * fRec696[1]);
			fRec695[0] = fConst1120 * (fConst1122 * fRec696[0] + fConst1137 * fRec696[1] + fConst1122 * fRec696[2]) - fConst1117 * (fConst1138 * fRec695[2] + fConst1139 * fRec695[1]);
			fRec694[0] = fConst1117 * (fConst1119 * fRec695[0] + fConst1140 * fRec695[1] + fConst1119 * fRec695[2]) - fConst1115 * (fConst1141 * fRec694[2] + fConst1142 * fRec694[1]);
			fRec693[0] = fConst1115 * (fConst1116 * fRec694[0] + fConst1143 * fRec694[1] + fConst1116 * fRec694[2]) - fConst1110 * (fConst1144 * fRec693[2] + fConst1145 * fRec693[1]);
			fRec692[0] = fConst1111 * (fConst1114 * fRec693[0] + fConst1146 * fRec693[1] + fConst1114 * fRec693[2]) - fConst1126 * (fConst1129 * fRec692[2] + fConst1130 * fRec692[1]);
			fRec691[0] = fConst1126 * (fConst1128 * fRec692[0] + fConst1131 * fRec692[1] + fConst1128 * fRec692[2]) - fConst1123 * (fConst1132 * fRec691[2] + fConst1133 * fRec691[1]);
			fRec690[0] = fConst1123 * (fConst1125 * fRec691[0] + fConst1134 * fRec691[1] + fConst1125 * fRec691[2]) - fConst1120 * (fConst1135 * fRec690[2] + fConst1136 * fRec690[1]);
			fRec689[0] = fConst1120 * (fConst1122 * fRec690[0] + fConst1137 * fRec690[1] + fConst1122 * fRec690[2]) - fConst1117 * (fConst1138 * fRec689[2] + fConst1139 * fRec689[1]);
			fRec688[0] = fConst1117 * (fConst1119 * fRec689[0] + fConst1140 * fRec689[1] + fConst1119 * fRec689[2]) - fConst1115 * (fConst1141 * fRec688[2] + fConst1142 * fRec688[1]);
			fRec687[0] = fConst1115 * (fConst1116 * fRec688[0] + fConst1143 * fRec688[1] + fConst1116 * fRec688[2]) - fConst1110 * (fConst1144 * fRec687[2] + fConst1145 * fRec687[1]);
			double fTemp115 = std::fabs(fConst1111 * (fConst1114 * fRec687[0] + fConst1146 * fRec687[1] + fConst1114 * fRec687[2]));
			double fTemp116 = ((fTemp115 > fRec686[1]) ? fTemp5 : fTemp3);
			fRec686[0] = fTemp115 * (1.0 - fTemp116) + fRec686[1] * fTemp116;
			double fTemp117 = tanh(fRec686[0]);
			fRec710[0] = fTemp9 - fConst1126 * (fConst1129 * fRec710[2] + fConst1130 * fRec710[1]);
			fRec709[0] = fConst1126 * (fConst1128 * fRec710[0] + fConst1131 * fRec710[1] + fConst1128 * fRec710[2]) - fConst1123 * (fConst1132 * fRec709[2] + fConst1133 * fRec709[1]);
			fRec708[0] = fConst1123 * (fConst1125 * fRec709[0] + fConst1134 * fRec709[1] + fConst1125 * fRec709[2]) - fConst1120 * (fConst1135 * fRec708[2] + fConst1136 * fRec708[1]);
			fRec707[0] = fConst1120 * (fConst1122 * fRec708[0] + fConst1137 * fRec708[1] + fConst1122 * fRec708[2]) - fConst1117 * (fConst1138 * fRec707[2] + fConst1139 * fRec707[1]);
			fRec706[0] = fConst1117 * (fConst1119 * fRec707[0] + fConst1140 * fRec707[1] + fConst1119 * fRec707[2]) - fConst1115 * (fConst1141 * fRec706[2] + fConst1142 * fRec706[1]);
			fRec705[0] = fConst1115 * (fConst1116 * fRec706[0] + fConst1143 * fRec706[1] + fConst1116 * fRec706[2]) - fConst1110 * (fConst1144 * fRec705[2] + fConst1145 * fRec705[1]);
			fRec704[0] = fConst1111 * fTemp117 * (fConst1114 * fRec705[0] + fConst1146 * fRec705[1] + fConst1114 * fRec705[2]) - fConst1126 * (fConst1129 * fRec704[2] + fConst1130 * fRec704[1]);
			fRec703[0] = fConst1126 * (fConst1128 * fRec704[0] + fConst1131 * fRec704[1] + fConst1128 * fRec704[2]) - fConst1123 * (fConst1132 * fRec703[2] + fConst1133 * fRec703[1]);
			fRec702[0] = fConst1123 * (fConst1125 * fRec703[0] + fConst1134 * fRec703[1] + fConst1125 * fRec703[2]) - fConst1120 * (fConst1135 * fRec702[2] + fConst1136 * fRec702[1]);
			fRec701[0] = fConst1120 * (fConst1122 * fRec702[0] + fConst1137 * fRec702[1] + fConst1122 * fRec702[2]) - fConst1117 * (fConst1138 * fRec701[2] + fConst1139 * fRec701[1]);
			fRec700[0] = fConst1117 * (fConst1119 * fRec701[0] + fConst1140 * fRec701[1] + fConst1119 * fRec701[2]) - fConst1115 * (fConst1141 * fRec700[2] + fConst1142 * fRec700[1]);
			fRec699[0] = fConst1115 * (fConst1116 * fRec700[0] + fConst1143 * fRec700[1] + fConst1116 * fRec700[2]) - fConst1110 * (fConst1144 * fRec699[2] + fConst1145 * fRec699[1]);
			double fTemp118 = fConst1146 * fRec699[1];
			fRec723[0] = fTemp0 - fConst1166 * (fConst1169 * fRec723[2] + fConst1170 * fRec723[1]);
			fRec722[0] = fConst1166 * (fConst1168 * fRec723[0] + fConst1171 * fRec723[1] + fConst1168 * fRec723[2]) - fConst1163 * (fConst1172 * fRec722[2] + fConst1173 * fRec722[1]);
			fRec721[0] = fConst1163 * (fConst1165 * fRec722[0] + fConst1174 * fRec722[1] + fConst1165 * fRec722[2]) - fConst1160 * (fConst1175 * fRec721[2] + fConst1176 * fRec721[1]);
			fRec720[0] = fConst1160 * (fConst1162 * fRec721[0] + fConst1177 * fRec721[1] + fConst1162 * fRec721[2]) - fConst1157 * (fConst1178 * fRec720[2] + fConst1179 * fRec720[1]);
			fRec719[0] = fConst1157 * (fConst1159 * fRec720[0] + fConst1180 * fRec720[1] + fConst1159 * fRec720[2]) - fConst1155 * (fConst1181 * fRec719[2] + fConst1182 * fRec719[1]);
			fRec718[0] = fConst1155 * (fConst1156 * fRec719[0] + fConst1183 * fRec719[1] + fConst1156 * fRec719[2]) - fConst1150 * (fConst1184 * fRec718[2] + fConst1185 * fRec718[1]);
			fRec717[0] = fConst1151 * (fConst1154 * fRec718[0] + fConst1186 * fRec718[1] + fConst1154 * fRec718[2]) - fConst1166 * (fConst1169 * fRec717[2] + fConst1170 * fRec717[1]);
			fRec716[0] = fConst1166 * (fConst1168 * fRec717[0] + fConst1171 * fRec717[1] + fConst1168 * fRec717[2]) - fConst1163 * (fConst1172 * fRec716[2] + fConst1173 * fRec716[1]);
			fRec715[0] = fConst1163 * (fConst1165 * fRec716[0] + fConst1174 * fRec716[1] + fConst1165 * fRec716[2]) - fConst1160 * (fConst1175 * fRec715[2] + fConst1176 * fRec715[1]);
			fRec714[0] = fConst1160 * (fConst1162 * fRec715[0] + fConst1177 * fRec715[1] + fConst1162 * fRec715[2]) - fConst1157 * (fConst1178 * fRec714[2] + fConst1179 * fRec714[1]);
			fRec713[0] = fConst1157 * (fConst1159 * fRec714[0] + fConst1180 * fRec714[1] + fConst1159 * fRec714[2]) - fConst1155 * (fConst1181 * fRec713[2] + fConst1182 * fRec713[1]);
			fRec712[0] = fConst1155 * (fConst1156 * fRec713[0] + fConst1183 * fRec713[1] + fConst1156 * fRec713[2]) - fConst1150 * (fConst1184 * fRec712[2] + fConst1185 * fRec712[1]);
			double fTemp119 = std::fabs(fConst1151 * (fConst1154 * fRec712[0] + fConst1186 * fRec712[1] + fConst1154 * fRec712[2]));
			double fTemp120 = ((fTemp119 > fRec711[1]) ? fTemp5 : fTemp3);
			fRec711[0] = fTemp119 * (1.0 - fTemp120) + fRec711[1] * fTemp120;
			double fTemp121 = tanh(fRec711[0]);
			fRec735[0] = fTemp9 - fConst1166 * (fConst1169 * fRec735[2] + fConst1170 * fRec735[1]);
			fRec734[0] = fConst1166 * (fConst1168 * fRec735[0] + fConst1171 * fRec735[1] + fConst1168 * fRec735[2]) - fConst1163 * (fConst1172 * fRec734[2] + fConst1173 * fRec734[1]);
			fRec733[0] = fConst1163 * (fConst1165 * fRec734[0] + fConst1174 * fRec734[1] + fConst1165 * fRec734[2]) - fConst1160 * (fConst1175 * fRec733[2] + fConst1176 * fRec733[1]);
			fRec732[0] = fConst1160 * (fConst1162 * fRec733[0] + fConst1177 * fRec733[1] + fConst1162 * fRec733[2]) - fConst1157 * (fConst1178 * fRec732[2] + fConst1179 * fRec732[1]);
			fRec731[0] = fConst1157 * (fConst1159 * fRec732[0] + fConst1180 * fRec732[1] + fConst1159 * fRec732[2]) - fConst1155 * (fConst1181 * fRec731[2] + fConst1182 * fRec731[1]);
			fRec730[0] = fConst1155 * (fConst1156 * fRec731[0] + fConst1183 * fRec731[1] + fConst1156 * fRec731[2]) - fConst1150 * (fConst1184 * fRec730[2] + fConst1185 * fRec730[1]);
			fRec729[0] = fConst1151 * fTemp121 * (fConst1154 * fRec730[0] + fConst1186 * fRec730[1] + fConst1154 * fRec730[2]) - fConst1166 * (fConst1169 * fRec729[2] + fConst1170 * fRec729[1]);
			fRec728[0] = fConst1166 * (fConst1168 * fRec729[0] + fConst1171 * fRec729[1] + fConst1168 * fRec729[2]) - fConst1163 * (fConst1172 * fRec728[2] + fConst1173 * fRec728[1]);
			fRec727[0] = fConst1163 * (fConst1165 * fRec728[0] + fConst1174 * fRec728[1] + fConst1165 * fRec728[2]) - fConst1160 * (fConst1175 * fRec727[2] + fConst1176 * fRec727[1]);
			fRec726[0] = fConst1160 * (fConst1162 * fRec727[0] + fConst1177 * fRec727[1] + fConst1162 * fRec727[2]) - fConst1157 * (fConst1178 * fRec726[2] + fConst1179 * fRec726[1]);
			fRec725[0] = fConst1157 * (fConst1159 * fRec726[0] + fConst1180 * fRec726[1] + fConst1159 * fRec726[2]) - fConst1155 * (fConst1181 * fRec725[2] + fConst1182 * fRec725[1]);
			fRec724[0] = fConst1155 * (fConst1156 * fRec725[0] + fConst1183 * fRec725[1] + fConst1156 * fRec725[2]) - fConst1150 * (fConst1184 * fRec724[2] + fConst1185 * fRec724[1]);
			double fTemp122 = fConst1186 * fRec724[1];
			fRec748[0] = fTemp0 - fConst1206 * (fConst1209 * fRec748[2] + fConst1210 * fRec748[1]);
			fRec747[0] = fConst1206 * (fConst1208 * fRec748[0] + fConst1211 * fRec748[1] + fConst1208 * fRec748[2]) - fConst1203 * (fConst1212 * fRec747[2] + fConst1213 * fRec747[1]);
			fRec746[0] = fConst1203 * (fConst1205 * fRec747[0] + fConst1214 * fRec747[1] + fConst1205 * fRec747[2]) - fConst1200 * (fConst1215 * fRec746[2] + fConst1216 * fRec746[1]);
			fRec745[0] = fConst1200 * (fConst1202 * fRec746[0] + fConst1217 * fRec746[1] + fConst1202 * fRec746[2]) - fConst1197 * (fConst1218 * fRec745[2] + fConst1219 * fRec745[1]);
			fRec744[0] = fConst1197 * (fConst1199 * fRec745[0] + fConst1220 * fRec745[1] + fConst1199 * fRec745[2]) - fConst1195 * (fConst1221 * fRec744[2] + fConst1222 * fRec744[1]);
			fRec743[0] = fConst1195 * (fConst1196 * fRec744[0] + fConst1223 * fRec744[1] + fConst1196 * fRec744[2]) - fConst1190 * (fConst1224 * fRec743[2] + fConst1225 * fRec743[1]);
			fRec742[0] = fConst1191 * (fConst1194 * fRec743[0] + fConst1226 * fRec743[1] + fConst1194 * fRec743[2]) - fConst1206 * (fConst1209 * fRec742[2] + fConst1210 * fRec742[1]);
			fRec741[0] = fConst1206 * (fConst1208 * fRec742[0] + fConst1211 * fRec742[1] + fConst1208 * fRec742[2]) - fConst1203 * (fConst1212 * fRec741[2] + fConst1213 * fRec741[1]);
			fRec740[0] = fConst1203 * (fConst1205 * fRec741[0] + fConst1214 * fRec741[1] + fConst1205 * fRec741[2]) - fConst1200 * (fConst1215 * fRec740[2] + fConst1216 * fRec740[1]);
			fRec739[0] = fConst1200 * (fConst1202 * fRec740[0] + fConst1217 * fRec740[1] + fConst1202 * fRec740[2]) - fConst1197 * (fConst1218 * fRec739[2] + fConst1219 * fRec739[1]);
			fRec738[0] = fConst1197 * (fConst1199 * fRec739[0] + fConst1220 * fRec739[1] + fConst1199 * fRec739[2]) - fConst1195 * (fConst1221 * fRec738[2] + fConst1222 * fRec738[1]);
			fRec737[0] = fConst1195 * (fConst1196 * fRec738[0] + fConst1223 * fRec738[1] + fConst1196 * fRec738[2]) - fConst1190 * (fConst1224 * fRec737[2] + fConst1225 * fRec737[1]);
			double fTemp123 = std::fabs(fConst1191 * (fConst1194 * fRec737[0] + fConst1226 * fRec737[1] + fConst1194 * fRec737[2]));
			double fTemp124 = ((fTemp123 > fRec736[1]) ? fTemp5 : fTemp3);
			fRec736[0] = fTemp123 * (1.0 - fTemp124) + fRec736[1] * fTemp124;
			double fTemp125 = tanh(fRec736[0]);
			fRec760[0] = fTemp9 - fConst1206 * (fConst1209 * fRec760[2] + fConst1210 * fRec760[1]);
			fRec759[0] = fConst1206 * (fConst1208 * fRec760[0] + fConst1211 * fRec760[1] + fConst1208 * fRec760[2]) - fConst1203 * (fConst1212 * fRec759[2] + fConst1213 * fRec759[1]);
			fRec758[0] = fConst1203 * (fConst1205 * fRec759[0] + fConst1214 * fRec759[1] + fConst1205 * fRec759[2]) - fConst1200 * (fConst1215 * fRec758[2] + fConst1216 * fRec758[1]);
			fRec757[0] = fConst1200 * (fConst1202 * fRec758[0] + fConst1217 * fRec758[1] + fConst1202 * fRec758[2]) - fConst1197 * (fConst1218 * fRec757[2] + fConst1219 * fRec757[1]);
			fRec756[0] = fConst1197 * (fConst1199 * fRec757[0] + fConst1220 * fRec757[1] + fConst1199 * fRec757[2]) - fConst1195 * (fConst1221 * fRec756[2] + fConst1222 * fRec756[1]);
			fRec755[0] = fConst1195 * (fConst1196 * fRec756[0] + fConst1223 * fRec756[1] + fConst1196 * fRec756[2]) - fConst1190 * (fConst1224 * fRec755[2] + fConst1225 * fRec755[1]);
			fRec754[0] = fConst1191 * fTemp125 * (fConst1194 * fRec755[0] + fConst1226 * fRec755[1] + fConst1194 * fRec755[2]) - fConst1206 * (fConst1209 * fRec754[2] + fConst1210 * fRec754[1]);
			fRec753[0] = fConst1206 * (fConst1208 * fRec754[0] + fConst1211 * fRec754[1] + fConst1208 * fRec754[2]) - fConst1203 * (fConst1212 * fRec753[2] + fConst1213 * fRec753[1]);
			fRec752[0] = fConst1203 * (fConst1205 * fRec753[0] + fConst1214 * fRec753[1] + fConst1205 * fRec753[2]) - fConst1200 * (fConst1215 * fRec752[2] + fConst1216 * fRec752[1]);
			fRec751[0] = fConst1200 * (fConst1202 * fRec752[0] + fConst1217 * fRec752[1] + fConst1202 * fRec752[2]) - fConst1197 * (fConst1218 * fRec751[2] + fConst1219 * fRec751[1]);
			fRec750[0] = fConst1197 * (fConst1199 * fRec751[0] + fConst1220 * fRec751[1] + fConst1199 * fRec751[2]) - fConst1195 * (fConst1221 * fRec750[2] + fConst1222 * fRec750[1]);
			fRec749[0] = fConst1195 * (fConst1196 * fRec750[0] + fConst1223 * fRec750[1] + fConst1196 * fRec750[2]) - fConst1190 * (fConst1224 * fRec749[2] + fConst1225 * fRec749[1]);
			double fTemp126 = fConst1226 * fRec749[1];
			fRec773[0] = fTemp0 - fConst1246 * (fConst1249 * fRec773[2] + fConst1250 * fRec773[1]);
			fRec772[0] = fConst1246 * (fConst1248 * fRec773[0] + fConst1251 * fRec773[1] + fConst1248 * fRec773[2]) - fConst1243 * (fConst1252 * fRec772[2] + fConst1253 * fRec772[1]);
			fRec771[0] = fConst1243 * (fConst1245 * fRec772[0] + fConst1254 * fRec772[1] + fConst1245 * fRec772[2]) - fConst1240 * (fConst1255 * fRec771[2] + fConst1256 * fRec771[1]);
			fRec770[0] = fConst1240 * (fConst1242 * fRec771[0] + fConst1257 * fRec771[1] + fConst1242 * fRec771[2]) - fConst1237 * (fConst1258 * fRec770[2] + fConst1259 * fRec770[1]);
			fRec769[0] = fConst1237 * (fConst1239 * fRec770[0] + fConst1260 * fRec770[1] + fConst1239 * fRec770[2]) - fConst1235 * (fConst1261 * fRec769[2] + fConst1262 * fRec769[1]);
			fRec768[0] = fConst1235 * (fConst1236 * fRec769[0] + fConst1263 * fRec769[1] + fConst1236 * fRec769[2]) - fConst1230 * (fConst1264 * fRec768[2] + fConst1265 * fRec768[1]);
			fRec767[0] = fConst1231 * (fConst1234 * fRec768[0] + fConst1266 * fRec768[1] + fConst1234 * fRec768[2]) - fConst1246 * (fConst1249 * fRec767[2] + fConst1250 * fRec767[1]);
			fRec766[0] = fConst1246 * (fConst1248 * fRec767[0] + fConst1251 * fRec767[1] + fConst1248 * fRec767[2]) - fConst1243 * (fConst1252 * fRec766[2] + fConst1253 * fRec766[1]);
			fRec765[0] = fConst1243 * (fConst1245 * fRec766[0] + fConst1254 * fRec766[1] + fConst1245 * fRec766[2]) - fConst1240 * (fConst1255 * fRec765[2] + fConst1256 * fRec765[1]);
			fRec764[0] = fConst1240 * (fConst1242 * fRec765[0] + fConst1257 * fRec765[1] + fConst1242 * fRec765[2]) - fConst1237 * (fConst1258 * fRec764[2] + fConst1259 * fRec764[1]);
			fRec763[0] = fConst1237 * (fConst1239 * fRec764[0] + fConst1260 * fRec764[1] + fConst1239 * fRec764[2]) - fConst1235 * (fConst1261 * fRec763[2] + fConst1262 * fRec763[1]);
			fRec762[0] = fConst1235 * (fConst1236 * fRec763[0] + fConst1263 * fRec763[1] + fConst1236 * fRec763[2]) - fConst1230 * (fConst1264 * fRec762[2] + fConst1265 * fRec762[1]);
			double fTemp127 = std::fabs(fConst1231 * (fConst1234 * fRec762[0] + fConst1266 * fRec762[1] + fConst1234 * fRec762[2]));
			double fTemp128 = ((fTemp127 > fRec761[1]) ? fTemp5 : fTemp3);
			fRec761[0] = fTemp127 * (1.0 - fTemp128) + fRec761[1] * fTemp128;
			double fTemp129 = tanh(fRec761[0]);
			fRec785[0] = fTemp9 - fConst1246 * (fConst1249 * fRec785[2] + fConst1250 * fRec785[1]);
			fRec784[0] = fConst1246 * (fConst1248 * fRec785[0] + fConst1251 * fRec785[1] + fConst1248 * fRec785[2]) - fConst1243 * (fConst1252 * fRec784[2] + fConst1253 * fRec784[1]);
			fRec783[0] = fConst1243 * (fConst1245 * fRec784[0] + fConst1254 * fRec784[1] + fConst1245 * fRec784[2]) - fConst1240 * (fConst1255 * fRec783[2] + fConst1256 * fRec783[1]);
			fRec782[0] = fConst1240 * (fConst1242 * fRec783[0] + fConst1257 * fRec783[1] + fConst1242 * fRec783[2]) - fConst1237 * (fConst1258 * fRec782[2] + fConst1259 * fRec782[1]);
			fRec781[0] = fConst1237 * (fConst1239 * fRec782[0] + fConst1260 * fRec782[1] + fConst1239 * fRec782[2]) - fConst1235 * (fConst1261 * fRec781[2] + fConst1262 * fRec781[1]);
			fRec780[0] = fConst1235 * (fConst1236 * fRec781[0] + fConst1263 * fRec781[1] + fConst1236 * fRec781[2]) - fConst1230 * (fConst1264 * fRec780[2] + fConst1265 * fRec780[1]);
			fRec779[0] = fConst1231 * fTemp129 * (fConst1234 * fRec780[0] + fConst1266 * fRec780[1] + fConst1234 * fRec780[2]) - fConst1246 * (fConst1249 * fRec779[2] + fConst1250 * fRec779[1]);
			fRec778[0] = fConst1246 * (fConst1248 * fRec779[0] + fConst1251 * fRec779[1] + fConst1248 * fRec779[2]) - fConst1243 * (fConst1252 * fRec778[2] + fConst1253 * fRec778[1]);
			fRec777[0] = fConst1243 * (fConst1245 * fRec778[0] + fConst1254 * fRec778[1] + fConst1245 * fRec778[2]) - fConst1240 * (fConst1255 * fRec777[2] + fConst1256 * fRec777[1]);
			fRec776[0] = fConst1240 * (fConst1242 * fRec777[0] + fConst1257 * fRec777[1] + fConst1242 * fRec777[2]) - fConst1237 * (fConst1258 * fRec776[2] + fConst1259 * fRec776[1]);
			fRec775[0] = fConst1237 * (fConst1239 * fRec776[0] + fConst1260 * fRec776[1] + fConst1239 * fRec776[2]) - fConst1235 * (fConst1261 * fRec775[2] + fConst1262 * fRec775[1]);
			fRec774[0] = fConst1235 * (fConst1236 * fRec775[0] + fConst1263 * fRec775[1] + fConst1236 * fRec775[2]) - fConst1230 * (fConst1264 * fRec774[2] + fConst1265 * fRec774[1]);
			double fTemp130 = fConst1266 * fRec774[1];
			fRec798[0] = fTemp0 - fConst1286 * (fConst1289 * fRec798[2] + fConst1290 * fRec798[1]);
			fRec797[0] = fConst1286 * (fConst1288 * fRec798[0] + fConst1291 * fRec798[1] + fConst1288 * fRec798[2]) - fConst1283 * (fConst1292 * fRec797[2] + fConst1293 * fRec797[1]);
			fRec796[0] = fConst1283 * (fConst1285 * fRec797[0] + fConst1294 * fRec797[1] + fConst1285 * fRec797[2]) - fConst1280 * (fConst1295 * fRec796[2] + fConst1296 * fRec796[1]);
			fRec795[0] = fConst1280 * (fConst1282 * fRec796[0] + fConst1297 * fRec796[1] + fConst1282 * fRec796[2]) - fConst1277 * (fConst1298 * fRec795[2] + fConst1299 * fRec795[1]);
			fRec794[0] = fConst1277 * (fConst1279 * fRec795[0] + fConst1300 * fRec795[1] + fConst1279 * fRec795[2]) - fConst1275 * (fConst1301 * fRec794[2] + fConst1302 * fRec794[1]);
			fRec793[0] = fConst1275 * (fConst1276 * fRec794[0] + fConst1303 * fRec794[1] + fConst1276 * fRec794[2]) - fConst1270 * (fConst1304 * fRec793[2] + fConst1305 * fRec793[1]);
			fRec792[0] = fConst1271 * (fConst1274 * fRec793[0] + fConst1306 * fRec793[1] + fConst1274 * fRec793[2]) - fConst1286 * (fConst1289 * fRec792[2] + fConst1290 * fRec792[1]);
			fRec791[0] = fConst1286 * (fConst1288 * fRec792[0] + fConst1291 * fRec792[1] + fConst1288 * fRec792[2]) - fConst1283 * (fConst1292 * fRec791[2] + fConst1293 * fRec791[1]);
			fRec790[0] = fConst1283 * (fConst1285 * fRec791[0] + fConst1294 * fRec791[1] + fConst1285 * fRec791[2]) - fConst1280 * (fConst1295 * fRec790[2] + fConst1296 * fRec790[1]);
			fRec789[0] = fConst1280 * (fConst1282 * fRec790[0] + fConst1297 * fRec790[1] + fConst1282 * fRec790[2]) - fConst1277 * (fConst1298 * fRec789[2] + fConst1299 * fRec789[1]);
			fRec788[0] = fConst1277 * (fConst1279 * fRec789[0] + fConst1300 * fRec789[1] + fConst1279 * fRec789[2]) - fConst1275 * (fConst1301 * fRec788[2] + fConst1302 * fRec788[1]);
			fRec787[0] = fConst1275 * (fConst1276 * fRec788[0] + fConst1303 * fRec788[1] + fConst1276 * fRec788[2]) - fConst1270 * (fConst1304 * fRec787[2] + fConst1305 * fRec787[1]);
			double fTemp131 = std::fabs(fConst1271 * (fConst1274 * fRec787[0] + fConst1306 * fRec787[1] + fConst1274 * fRec787[2]));
			double fTemp132 = ((fTemp131 > fRec786[1]) ? fTemp5 : fTemp3);
			fRec786[0] = fTemp131 * (1.0 - fTemp132) + fRec786[1] * fTemp132;
			double fTemp133 = tanh(fRec786[0]);
			fRec810[0] = fTemp9 - fConst1286 * (fConst1289 * fRec810[2] + fConst1290 * fRec810[1]);
			fRec809[0] = fConst1286 * (fConst1288 * fRec810[0] + fConst1291 * fRec810[1] + fConst1288 * fRec810[2]) - fConst1283 * (fConst1292 * fRec809[2] + fConst1293 * fRec809[1]);
			fRec808[0] = fConst1283 * (fConst1285 * fRec809[0] + fConst1294 * fRec809[1] + fConst1285 * fRec809[2]) - fConst1280 * (fConst1295 * fRec808[2] + fConst1296 * fRec808[1]);
			fRec807[0] = fConst1280 * (fConst1282 * fRec808[0] + fConst1297 * fRec808[1] + fConst1282 * fRec808[2]) - fConst1277 * (fConst1298 * fRec807[2] + fConst1299 * fRec807[1]);
			fRec806[0] = fConst1277 * (fConst1279 * fRec807[0] + fConst1300 * fRec807[1] + fConst1279 * fRec807[2]) - fConst1275 * (fConst1301 * fRec806[2] + fConst1302 * fRec806[1]);
			fRec805[0] = fConst1275 * (fConst1276 * fRec806[0] + fConst1303 * fRec806[1] + fConst1276 * fRec806[2]) - fConst1270 * (fConst1304 * fRec805[2] + fConst1305 * fRec805[1]);
			fRec804[0] = fConst1271 * fTemp133 * (fConst1274 * fRec805[0] + fConst1306 * fRec805[1] + fConst1274 * fRec805[2]) - fConst1286 * (fConst1289 * fRec804[2] + fConst1290 * fRec804[1]);
			fRec803[0] = fConst1286 * (fConst1288 * fRec804[0] + fConst1291 * fRec804[1] + fConst1288 * fRec804[2]) - fConst1283 * (fConst1292 * fRec803[2] + fConst1293 * fRec803[1]);
			fRec802[0] = fConst1283 * (fConst1285 * fRec803[0] + fConst1294 * fRec803[1] + fConst1285 * fRec803[2]) - fConst1280 * (fConst1295 * fRec802[2] + fConst1296 * fRec802[1]);
			fRec801[0] = fConst1280 * (fConst1282 * fRec802[0] + fConst1297 * fRec802[1] + fConst1282 * fRec802[2]) - fConst1277 * (fConst1298 * fRec801[2] + fConst1299 * fRec801[1]);
			fRec800[0] = fConst1277 * (fConst1279 * fRec801[0] + fConst1300 * fRec801[1] + fConst1279 * fRec801[2]) - fConst1275 * (fConst1301 * fRec800[2] + fConst1302 * fRec800[1]);
			fRec799[0] = fConst1275 * (fConst1276 * fRec800[0] + fConst1303 * fRec800[1] + fConst1276 * fRec800[2]) - fConst1270 * (fConst1304 * fRec799[2] + fConst1305 * fRec799[1]);
			double fTemp134 = fConst1306 * fRec799[1];
			fRec823[0] = fTemp0 - fConst1326 * (fConst1329 * fRec823[2] + fConst1330 * fRec823[1]);
			fRec822[0] = fConst1326 * (fConst1328 * fRec823[0] + fConst1331 * fRec823[1] + fConst1328 * fRec823[2]) - fConst1323 * (fConst1332 * fRec822[2] + fConst1333 * fRec822[1]);
			fRec821[0] = fConst1323 * (fConst1325 * fRec822[0] + fConst1334 * fRec822[1] + fConst1325 * fRec822[2]) - fConst1320 * (fConst1335 * fRec821[2] + fConst1336 * fRec821[1]);
			fRec820[0] = fConst1320 * (fConst1322 * fRec821[0] + fConst1337 * fRec821[1] + fConst1322 * fRec821[2]) - fConst1317 * (fConst1338 * fRec820[2] + fConst1339 * fRec820[1]);
			fRec819[0] = fConst1317 * (fConst1319 * fRec820[0] + fConst1340 * fRec820[1] + fConst1319 * fRec820[2]) - fConst1315 * (fConst1341 * fRec819[2] + fConst1342 * fRec819[1]);
			fRec818[0] = fConst1315 * (fConst1316 * fRec819[0] + fConst1343 * fRec819[1] + fConst1316 * fRec819[2]) - fConst1310 * (fConst1344 * fRec818[2] + fConst1345 * fRec818[1]);
			fRec817[0] = fConst1311 * (fConst1314 * fRec818[0] + fConst1346 * fRec818[1] + fConst1314 * fRec818[2]) - fConst1326 * (fConst1329 * fRec817[2] + fConst1330 * fRec817[1]);
			fRec816[0] = fConst1326 * (fConst1328 * fRec817[0] + fConst1331 * fRec817[1] + fConst1328 * fRec817[2]) - fConst1323 * (fConst1332 * fRec816[2] + fConst1333 * fRec816[1]);
			fRec815[0] = fConst1323 * (fConst1325 * fRec816[0] + fConst1334 * fRec816[1] + fConst1325 * fRec816[2]) - fConst1320 * (fConst1335 * fRec815[2] + fConst1336 * fRec815[1]);
			fRec814[0] = fConst1320 * (fConst1322 * fRec815[0] + fConst1337 * fRec815[1] + fConst1322 * fRec815[2]) - fConst1317 * (fConst1338 * fRec814[2] + fConst1339 * fRec814[1]);
			fRec813[0] = fConst1317 * (fConst1319 * fRec814[0] + fConst1340 * fRec814[1] + fConst1319 * fRec814[2]) - fConst1315 * (fConst1341 * fRec813[2] + fConst1342 * fRec813[1]);
			fRec812[0] = fConst1315 * (fConst1316 * fRec813[0] + fConst1343 * fRec813[1] + fConst1316 * fRec813[2]) - fConst1310 * (fConst1344 * fRec812[2] + fConst1345 * fRec812[1]);
			double fTemp135 = std::fabs(fConst1311 * (fConst1314 * fRec812[0] + fConst1346 * fRec812[1] + fConst1314 * fRec812[2]));
			double fTemp136 = ((fTemp135 > fRec811[1]) ? fTemp5 : fTemp3);
			fRec811[0] = fTemp135 * (1.0 - fTemp136) + fRec811[1] * fTemp136;
			double fTemp137 = tanh(fRec811[0]);
			fRec835[0] = fTemp9 - fConst1326 * (fConst1329 * fRec835[2] + fConst1330 * fRec835[1]);
			fRec834[0] = fConst1326 * (fConst1328 * fRec835[0] + fConst1331 * fRec835[1] + fConst1328 * fRec835[2]) - fConst1323 * (fConst1332 * fRec834[2] + fConst1333 * fRec834[1]);
			fRec833[0] = fConst1323 * (fConst1325 * fRec834[0] + fConst1334 * fRec834[1] + fConst1325 * fRec834[2]) - fConst1320 * (fConst1335 * fRec833[2] + fConst1336 * fRec833[1]);
			fRec832[0] = fConst1320 * (fConst1322 * fRec833[0] + fConst1337 * fRec833[1] + fConst1322 * fRec833[2]) - fConst1317 * (fConst1338 * fRec832[2] + fConst1339 * fRec832[1]);
			fRec831[0] = fConst1317 * (fConst1319 * fRec832[0] + fConst1340 * fRec832[1] + fConst1319 * fRec832[2]) - fConst1315 * (fConst1341 * fRec831[2] + fConst1342 * fRec831[1]);
			fRec830[0] = fConst1315 * (fConst1316 * fRec831[0] + fConst1343 * fRec831[1] + fConst1316 * fRec831[2]) - fConst1310 * (fConst1344 * fRec830[2] + fConst1345 * fRec830[1]);
			fRec829[0] = fConst1311 * fTemp137 * (fConst1314 * fRec830[0] + fConst1346 * fRec830[1] + fConst1314 * fRec830[2]) - fConst1326 * (fConst1329 * fRec829[2] + fConst1330 * fRec829[1]);
			fRec828[0] = fConst1326 * (fConst1328 * fRec829[0] + fConst1331 * fRec829[1] + fConst1328 * fRec829[2]) - fConst1323 * (fConst1332 * fRec828[2] + fConst1333 * fRec828[1]);
			fRec827[0] = fConst1323 * (fConst1325 * fRec828[0] + fConst1334 * fRec828[1] + fConst1325 * fRec828[2]) - fConst1320 * (fConst1335 * fRec827[2] + fConst1336 * fRec827[1]);
			fRec826[0] = fConst1320 * (fConst1322 * fRec827[0] + fConst1337 * fRec827[1] + fConst1322 * fRec827[2]) - fConst1317 * (fConst1338 * fRec826[2] + fConst1339 * fRec826[1]);
			fRec825[0] = fConst1317 * (fConst1319 * fRec826[0] + fConst1340 * fRec826[1] + fConst1319 * fRec826[2]) - fConst1315 * (fConst1341 * fRec825[2] + fConst1342 * fRec825[1]);
			fRec824[0] = fConst1315 * (fConst1316 * fRec825[0] + fConst1343 * fRec825[1] + fConst1316 * fRec825[2]) - fConst1310 * (fConst1344 * fRec824[2] + fConst1345 * fRec824[1]);
			double fTemp138 = fConst1346 * fRec824[1];
			fRec848[0] = fTemp0 - fConst1366 * (fConst1369 * fRec848[2] + fConst1370 * fRec848[1]);
			fRec847[0] = fConst1366 * (fConst1368 * fRec848[0] + fConst1371 * fRec848[1] + fConst1368 * fRec848[2]) - fConst1363 * (fConst1372 * fRec847[2] + fConst1373 * fRec847[1]);
			fRec846[0] = fConst1363 * (fConst1365 * fRec847[0] + fConst1374 * fRec847[1] + fConst1365 * fRec847[2]) - fConst1360 * (fConst1375 * fRec846[2] + fConst1376 * fRec846[1]);
			fRec845[0] = fConst1360 * (fConst1362 * fRec846[0] + fConst1377 * fRec846[1] + fConst1362 * fRec846[2]) - fConst1357 * (fConst1378 * fRec845[2] + fConst1379 * fRec845[1]);
			fRec844[0] = fConst1357 * (fConst1359 * fRec845[0] + fConst1380 * fRec845[1] + fConst1359 * fRec845[2]) - fConst1355 * (fConst1381 * fRec844[2] + fConst1382 * fRec844[1]);
			fRec843[0] = fConst1355 * (fConst1356 * fRec844[0] + fConst1383 * fRec844[1] + fConst1356 * fRec844[2]) - fConst1350 * (fConst1384 * fRec843[2] + fConst1385 * fRec843[1]);
			fRec842[0] = fConst1351 * (fConst1354 * fRec843[0] + fConst1386 * fRec843[1] + fConst1354 * fRec843[2]) - fConst1366 * (fConst1369 * fRec842[2] + fConst1370 * fRec842[1]);
			fRec841[0] = fConst1366 * (fConst1368 * fRec842[0] + fConst1371 * fRec842[1] + fConst1368 * fRec842[2]) - fConst1363 * (fConst1372 * fRec841[2] + fConst1373 * fRec841[1]);
			fRec840[0] = fConst1363 * (fConst1365 * fRec841[0] + fConst1374 * fRec841[1] + fConst1365 * fRec841[2]) - fConst1360 * (fConst1375 * fRec840[2] + fConst1376 * fRec840[1]);
			fRec839[0] = fConst1360 * (fConst1362 * fRec840[0] + fConst1377 * fRec840[1] + fConst1362 * fRec840[2]) - fConst1357 * (fConst1378 * fRec839[2] + fConst1379 * fRec839[1]);
			fRec838[0] = fConst1357 * (fConst1359 * fRec839[0] + fConst1380 * fRec839[1] + fConst1359 * fRec839[2]) - fConst1355 * (fConst1381 * fRec838[2] + fConst1382 * fRec838[1]);
			fRec837[0] = fConst1355 * (fConst1356 * fRec838[0] + fConst1383 * fRec838[1] + fConst1356 * fRec838[2]) - fConst1350 * (fConst1384 * fRec837[2] + fConst1385 * fRec837[1]);
			double fTemp139 = std::fabs(fConst1351 * (fConst1354 * fRec837[0] + fConst1386 * fRec837[1] + fConst1354 * fRec837[2]));
			double fTemp140 = ((fTemp139 > fRec836[1]) ? fTemp5 : fTemp3);
			fRec836[0] = fTemp139 * (1.0 - fTemp140) + fRec836[1] * fTemp140;
			double fTemp141 = tanh(fRec836[0]);
			fRec860[0] = fTemp9 - fConst1366 * (fConst1369 * fRec860[2] + fConst1370 * fRec860[1]);
			fRec859[0] = fConst1366 * (fConst1368 * fRec860[0] + fConst1371 * fRec860[1] + fConst1368 * fRec860[2]) - fConst1363 * (fConst1372 * fRec859[2] + fConst1373 * fRec859[1]);
			fRec858[0] = fConst1363 * (fConst1365 * fRec859[0] + fConst1374 * fRec859[1] + fConst1365 * fRec859[2]) - fConst1360 * (fConst1375 * fRec858[2] + fConst1376 * fRec858[1]);
			fRec857[0] = fConst1360 * (fConst1362 * fRec858[0] + fConst1377 * fRec858[1] + fConst1362 * fRec858[2]) - fConst1357 * (fConst1378 * fRec857[2] + fConst1379 * fRec857[1]);
			fRec856[0] = fConst1357 * (fConst1359 * fRec857[0] + fConst1380 * fRec857[1] + fConst1359 * fRec857[2]) - fConst1355 * (fConst1381 * fRec856[2] + fConst1382 * fRec856[1]);
			fRec855[0] = fConst1355 * (fConst1356 * fRec856[0] + fConst1383 * fRec856[1] + fConst1356 * fRec856[2]) - fConst1350 * (fConst1384 * fRec855[2] + fConst1385 * fRec855[1]);
			fRec854[0] = fConst1351 * fTemp141 * (fConst1354 * fRec855[0] + fConst1386 * fRec855[1] + fConst1354 * fRec855[2]) - fConst1366 * (fConst1369 * fRec854[2] + fConst1370 * fRec854[1]);
			fRec853[0] = fConst1366 * (fConst1368 * fRec854[0] + fConst1371 * fRec854[1] + fConst1368 * fRec854[2]) - fConst1363 * (fConst1372 * fRec853[2] + fConst1373 * fRec853[1]);
			fRec852[0] = fConst1363 * (fConst1365 * fRec853[0] + fConst1374 * fRec853[1] + fConst1365 * fRec853[2]) - fConst1360 * (fConst1375 * fRec852[2] + fConst1376 * fRec852[1]);
			fRec851[0] = fConst1360 * (fConst1362 * fRec852[0] + fConst1377 * fRec852[1] + fConst1362 * fRec852[2]) - fConst1357 * (fConst1378 * fRec851[2] + fConst1379 * fRec851[1]);
			fRec850[0] = fConst1357 * (fConst1359 * fRec851[0] + fConst1380 * fRec851[1] + fConst1359 * fRec851[2]) - fConst1355 * (fConst1381 * fRec850[2] + fConst1382 * fRec850[1]);
			fRec849[0] = fConst1355 * (fConst1356 * fRec850[0] + fConst1383 * fRec850[1] + fConst1356 * fRec850[2]) - fConst1350 * (fConst1384 * fRec849[2] + fConst1385 * fRec849[1]);
			double fTemp142 = fConst1386 * fRec849[1];
			fRec873[0] = fTemp0 - fConst1406 * (fConst1409 * fRec873[2] + fConst1410 * fRec873[1]);
			fRec872[0] = fConst1406 * (fConst1408 * fRec873[0] + fConst1411 * fRec873[1] + fConst1408 * fRec873[2]) - fConst1403 * (fConst1412 * fRec872[2] + fConst1413 * fRec872[1]);
			fRec871[0] = fConst1403 * (fConst1405 * fRec872[0] + fConst1414 * fRec872[1] + fConst1405 * fRec872[2]) - fConst1400 * (fConst1415 * fRec871[2] + fConst1416 * fRec871[1]);
			fRec870[0] = fConst1400 * (fConst1402 * fRec871[0] + fConst1417 * fRec871[1] + fConst1402 * fRec871[2]) - fConst1397 * (fConst1418 * fRec870[2] + fConst1419 * fRec870[1]);
			fRec869[0] = fConst1397 * (fConst1399 * fRec870[0] + fConst1420 * fRec870[1] + fConst1399 * fRec870[2]) - fConst1395 * (fConst1421 * fRec869[2] + fConst1422 * fRec869[1]);
			fRec868[0] = fConst1395 * (fConst1396 * fRec869[0] + fConst1423 * fRec869[1] + fConst1396 * fRec869[2]) - fConst1390 * (fConst1424 * fRec868[2] + fConst1425 * fRec868[1]);
			fRec867[0] = fConst1391 * (fConst1394 * fRec868[0] + fConst1426 * fRec868[1] + fConst1394 * fRec868[2]) - fConst1406 * (fConst1409 * fRec867[2] + fConst1410 * fRec867[1]);
			fRec866[0] = fConst1406 * (fConst1408 * fRec867[0] + fConst1411 * fRec867[1] + fConst1408 * fRec867[2]) - fConst1403 * (fConst1412 * fRec866[2] + fConst1413 * fRec866[1]);
			fRec865[0] = fConst1403 * (fConst1405 * fRec866[0] + fConst1414 * fRec866[1] + fConst1405 * fRec866[2]) - fConst1400 * (fConst1415 * fRec865[2] + fConst1416 * fRec865[1]);
			fRec864[0] = fConst1400 * (fConst1402 * fRec865[0] + fConst1417 * fRec865[1] + fConst1402 * fRec865[2]) - fConst1397 * (fConst1418 * fRec864[2] + fConst1419 * fRec864[1]);
			fRec863[0] = fConst1397 * (fConst1399 * fRec864[0] + fConst1420 * fRec864[1] + fConst1399 * fRec864[2]) - fConst1395 * (fConst1421 * fRec863[2] + fConst1422 * fRec863[1]);
			fRec862[0] = fConst1395 * (fConst1396 * fRec863[0] + fConst1423 * fRec863[1] + fConst1396 * fRec863[2]) - fConst1390 * (fConst1424 * fRec862[2] + fConst1425 * fRec862[1]);
			double fTemp143 = std::fabs(fConst1391 * (fConst1394 * fRec862[0] + fConst1426 * fRec862[1] + fConst1394 * fRec862[2]));
			double fTemp144 = ((fTemp143 > fRec861[1]) ? fTemp5 : fTemp3);
			fRec861[0] = fTemp143 * (1.0 - fTemp144) + fRec861[1] * fTemp144;
			double fTemp145 = tanh(fRec861[0]);
			fRec885[0] = fTemp9 - fConst1406 * (fConst1409 * fRec885[2] + fConst1410 * fRec885[1]);
			fRec884[0] = fConst1406 * (fConst1408 * fRec885[0] + fConst1411 * fRec885[1] + fConst1408 * fRec885[2]) - fConst1403 * (fConst1412 * fRec884[2] + fConst1413 * fRec884[1]);
			fRec883[0] = fConst1403 * (fConst1405 * fRec884[0] + fConst1414 * fRec884[1] + fConst1405 * fRec884[2]) - fConst1400 * (fConst1415 * fRec883[2] + fConst1416 * fRec883[1]);
			fRec882[0] = fConst1400 * (fConst1402 * fRec883[0] + fConst1417 * fRec883[1] + fConst1402 * fRec883[2]) - fConst1397 * (fConst1418 * fRec882[2] + fConst1419 * fRec882[1]);
			fRec881[0] = fConst1397 * (fConst1399 * fRec882[0] + fConst1420 * fRec882[1] + fConst1399 * fRec882[2]) - fConst1395 * (fConst1421 * fRec881[2] + fConst1422 * fRec881[1]);
			fRec880[0] = fConst1395 * (fConst1396 * fRec881[0] + fConst1423 * fRec881[1] + fConst1396 * fRec881[2]) - fConst1390 * (fConst1424 * fRec880[2] + fConst1425 * fRec880[1]);
			fRec879[0] = fConst1391 * fTemp145 * (fConst1394 * fRec880[0] + fConst1426 * fRec880[1] + fConst1394 * fRec880[2]) - fConst1406 * (fConst1409 * fRec879[2] + fConst1410 * fRec879[1]);
			fRec878[0] = fConst1406 * (fConst1408 * fRec879[0] + fConst1411 * fRec879[1] + fConst1408 * fRec879[2]) - fConst1403 * (fConst1412 * fRec878[2] + fConst1413 * fRec878[1]);
			fRec877[0] = fConst1403 * (fConst1405 * fRec878[0] + fConst1414 * fRec878[1] + fConst1405 * fRec878[2]) - fConst1400 * (fConst1415 * fRec877[2] + fConst1416 * fRec877[1]);
			fRec876[0] = fConst1400 * (fConst1402 * fRec877[0] + fConst1417 * fRec877[1] + fConst1402 * fRec877[2]) - fConst1397 * (fConst1418 * fRec876[2] + fConst1419 * fRec876[1]);
			fRec875[0] = fConst1397 * (fConst1399 * fRec876[0] + fConst1420 * fRec876[1] + fConst1399 * fRec876[2]) - fConst1395 * (fConst1421 * fRec875[2] + fConst1422 * fRec875[1]);
			fRec874[0] = fConst1395 * (fConst1396 * fRec875[0] + fConst1423 * fRec875[1] + fConst1396 * fRec875[2]) - fConst1390 * (fConst1424 * fRec874[2] + fConst1425 * fRec874[1]);
			double fTemp146 = fConst1426 * fRec874[1];
			fRec898[0] = fTemp0 - fConst1446 * (fConst1449 * fRec898[2] + fConst1450 * fRec898[1]);
			fRec897[0] = fConst1446 * (fConst1448 * fRec898[0] + fConst1451 * fRec898[1] + fConst1448 * fRec898[2]) - fConst1443 * (fConst1452 * fRec897[2] + fConst1453 * fRec897[1]);
			fRec896[0] = fConst1443 * (fConst1445 * fRec897[0] + fConst1454 * fRec897[1] + fConst1445 * fRec897[2]) - fConst1440 * (fConst1455 * fRec896[2] + fConst1456 * fRec896[1]);
			fRec895[0] = fConst1440 * (fConst1442 * fRec896[0] + fConst1457 * fRec896[1] + fConst1442 * fRec896[2]) - fConst1437 * (fConst1458 * fRec895[2] + fConst1459 * fRec895[1]);
			fRec894[0] = fConst1437 * (fConst1439 * fRec895[0] + fConst1460 * fRec895[1] + fConst1439 * fRec895[2]) - fConst1435 * (fConst1461 * fRec894[2] + fConst1462 * fRec894[1]);
			fRec893[0] = fConst1435 * (fConst1436 * fRec894[0] + fConst1463 * fRec894[1] + fConst1436 * fRec894[2]) - fConst1430 * (fConst1464 * fRec893[2] + fConst1465 * fRec893[1]);
			fRec892[0] = fConst1431 * (fConst1434 * fRec893[0] + fConst1466 * fRec893[1] + fConst1434 * fRec893[2]) - fConst1446 * (fConst1449 * fRec892[2] + fConst1450 * fRec892[1]);
			fRec891[0] = fConst1446 * (fConst1448 * fRec892[0] + fConst1451 * fRec892[1] + fConst1448 * fRec892[2]) - fConst1443 * (fConst1452 * fRec891[2] + fConst1453 * fRec891[1]);
			fRec890[0] = fConst1443 * (fConst1445 * fRec891[0] + fConst1454 * fRec891[1] + fConst1445 * fRec891[2]) - fConst1440 * (fConst1455 * fRec890[2] + fConst1456 * fRec890[1]);
			fRec889[0] = fConst1440 * (fConst1442 * fRec890[0] + fConst1457 * fRec890[1] + fConst1442 * fRec890[2]) - fConst1437 * (fConst1458 * fRec889[2] + fConst1459 * fRec889[1]);
			fRec888[0] = fConst1437 * (fConst1439 * fRec889[0] + fConst1460 * fRec889[1] + fConst1439 * fRec889[2]) - fConst1435 * (fConst1461 * fRec888[2] + fConst1462 * fRec888[1]);
			fRec887[0] = fConst1435 * (fConst1436 * fRec888[0] + fConst1463 * fRec888[1] + fConst1436 * fRec888[2]) - fConst1430 * (fConst1464 * fRec887[2] + fConst1465 * fRec887[1]);
			double fTemp147 = std::fabs(fConst1431 * (fConst1434 * fRec887[0] + fConst1466 * fRec887[1] + fConst1434 * fRec887[2]));
			double fTemp148 = ((fTemp147 > fRec886[1]) ? fTemp5 : fTemp3);
			fRec886[0] = fTemp147 * (1.0 - fTemp148) + fRec886[1] * fTemp148;
			double fTemp149 = tanh(fRec886[0]);
			fRec910[0] = fTemp9 - fConst1446 * (fConst1449 * fRec910[2] + fConst1450 * fRec910[1]);
			fRec909[0] = fConst1446 * (fConst1448 * fRec910[0] + fConst1451 * fRec910[1] + fConst1448 * fRec910[2]) - fConst1443 * (fConst1452 * fRec909[2] + fConst1453 * fRec909[1]);
			fRec908[0] = fConst1443 * (fConst1445 * fRec909[0] + fConst1454 * fRec909[1] + fConst1445 * fRec909[2]) - fConst1440 * (fConst1455 * fRec908[2] + fConst1456 * fRec908[1]);
			fRec907[0] = fConst1440 * (fConst1442 * fRec908[0] + fConst1457 * fRec908[1] + fConst1442 * fRec908[2]) - fConst1437 * (fConst1458 * fRec907[2] + fConst1459 * fRec907[1]);
			fRec906[0] = fConst1437 * (fConst1439 * fRec907[0] + fConst1460 * fRec907[1] + fConst1439 * fRec907[2]) - fConst1435 * (fConst1461 * fRec906[2] + fConst1462 * fRec906[1]);
			fRec905[0] = fConst1435 * (fConst1436 * fRec906[0] + fConst1463 * fRec906[1] + fConst1436 * fRec906[2]) - fConst1430 * (fConst1464 * fRec905[2] + fConst1465 * fRec905[1]);
			fRec904[0] = fConst1431 * fTemp149 * (fConst1434 * fRec905[0] + fConst1466 * fRec905[1] + fConst1434 * fRec905[2]) - fConst1446 * (fConst1449 * fRec904[2] + fConst1450 * fRec904[1]);
			fRec903[0] = fConst1446 * (fConst1448 * fRec904[0] + fConst1451 * fRec904[1] + fConst1448 * fRec904[2]) - fConst1443 * (fConst1452 * fRec903[2] + fConst1453 * fRec903[1]);
			fRec902[0] = fConst1443 * (fConst1445 * fRec903[0] + fConst1454 * fRec903[1] + fConst1445 * fRec903[2]) - fConst1440 * (fConst1455 * fRec902[2] + fConst1456 * fRec902[1]);
			fRec901[0] = fConst1440 * (fConst1442 * fRec902[0] + fConst1457 * fRec902[1] + fConst1442 * fRec902[2]) - fConst1437 * (fConst1458 * fRec901[2] + fConst1459 * fRec901[1]);
			fRec900[0] = fConst1437 * (fConst1439 * fRec901[0] + fConst1460 * fRec901[1] + fConst1439 * fRec901[2]) - fConst1435 * (fConst1461 * fRec900[2] + fConst1462 * fRec900[1]);
			fRec899[0] = fConst1435 * (fConst1436 * fRec900[0] + fConst1463 * fRec900[1] + fConst1436 * fRec900[2]) - fConst1430 * (fConst1464 * fRec899[2] + fConst1465 * fRec899[1]);
			double fTemp150 = fConst1466 * fRec899[1];
			double fTemp151 = 2e+01 * std::log10(std::max<double>(2.2250738585072014e-308, std::fabs(3.0 * (fConst4 * fTemp7 * (fConst8 * fRec16[0] + fTemp10 + fConst8 * fRec16[2]) + fConst70 * fTemp13 * (fConst74 * fRec49[0] + fTemp14 + fConst74 * fRec49[2]) + fConst110 * fTemp17 * (fConst114 * fRec74[0] + fTemp18 + fConst114 * fRec74[2]) + fConst150 * fTemp21 * (fConst154 * fRec99[0] + fTemp22 + fConst154 * fRec99[2]) + fConst190 * fTemp25 * (fConst194 * fRec124[0] + fTemp26 + fConst194 * fRec124[2]) + fConst230 * fTemp29 * (fConst234 * fRec149[0] + fTemp30 + fConst234 * fRec149[2]) + fConst270 * fTemp33 * (fConst274 * fRec174[0] + fTemp34 + fConst274 * fRec174[2]) + fConst310 * fTemp37 * (fConst314 * fRec199[0] + fTemp38 + fConst314 * fRec199[2]) + fConst350 * fTemp41 * (fConst354 * fRec224[0] + fTemp42 + fConst354 * fRec224[2]) + fConst390 * fTemp45 * (fConst394 * fRec249[0] + fTemp46 + fConst394 * fRec249[2]) + fConst430 * fTemp49 * (fConst434 * fRec274[0] + fTemp50 + fConst434 * fRec274[2]) + fConst470 * fTemp53 * (fConst474 * fRec299[0] + fTemp54 + fConst474 * fRec299[2]) + fConst510 * fTemp57 * (fConst514 * fRec324[0] + fTemp58 + fConst514 * fRec324[2]) + fConst550 * fTemp61 * (fConst554 * fRec349[0] + fTemp62 + fConst554 * fRec349[2]) + fConst590 * fTemp65 * (fConst594 * fRec374[0] + fTemp66 + fConst594 * fRec374[2]) + fConst630 * fTemp69 * (fConst634 * fRec399[0] + fTemp70 + fConst634 * fRec399[2]) + fConst670 * fTemp73 * (fConst674 * fRec424[0] + fTemp74 + fConst674 * fRec424[2]) + fConst710 * fTemp77 * (fConst714 * fRec449[0] + fTemp78 + fConst714 * fRec449[2]) + fConst750 * fTemp81 * (fConst754 * fRec474[0] + fTemp82 + fConst754 * fRec474[2]) + fConst790 * fTemp85 * (fConst794 * fRec499[0] + fTemp86 + fConst794 * fRec499[2]) + fConst830 * fTemp89 * (fConst834 * fRec524[0] + fTemp90 + fConst834 * fRec524[2]) + fConst870 * fTemp93 * (fConst874 * fRec549[0] + fTemp94 + fConst874 * fRec549[2]) + fConst910 * fTemp97 * (fConst914 * fRec574[0] + fTemp98 + fConst914 * fRec574[2]) + fConst950 * fTemp101 * (fConst954 * fRec599[0] + fTemp102 + fConst954 * fRec599[2]) + fConst990 * fTemp105 * (fConst994 * fRec624[0] + fTemp106 + fConst994 * fRec624[2]) + fConst1030 * fTemp109 * (fConst1034 * fRec649[0] + fTemp110 + fConst1034 * fRec649[2]) + fConst1070 * fTemp113 * (fConst1074 * fRec674[0] + fTemp114 + fConst1074 * fRec674[2]) + fConst1110 * fTemp117 * (fConst1114 * fRec699[0] + fTemp118 + fConst1114 * fRec699[2]) + fConst1150 * fTemp121 * (fConst1154 * fRec724[0] + fTemp122 + fConst1154 * fRec724[2]) + fConst1190 * fTemp125 * (fConst1194 * fRec749[0] + fTemp126 + fConst1194 * fRec749[2]) + fConst1230 * fTemp129 * (fConst1234 * fRec774[0] + fTemp130 + fConst1234 * fRec774[2]) + fConst1270 * fTemp133 * (fConst1274 * fRec799[0] + fTemp134 + fConst1274 * fRec799[2]) + fConst1310 * fTemp137 * (fConst1314 * fRec824[0] + fTemp138 + fConst1314 * fRec824[2]) + fConst1350 * fTemp141 * (fConst1354 * fRec849[0] + fTemp142 + fConst1354 * fRec849[2]) + fConst1390 * fTemp145 * (fConst1394 * fRec874[0] + fTemp146 + fConst1394 * fRec874[2]) + fConst1430 * fTemp149 * (fConst1434 * fRec899[0] + fTemp150 + fConst1434 * fRec899[2])))));
			int iTemp152 = (fTemp151 > -3.5) + (fTemp151 > -2.5);
			double fTemp153 = std::max<double>(0.0, ((iTemp152 == 0) ? 0.0 : ((iTemp152 == 1) ? 0.5 * piece_nono_quando_vocoder_faustpower2_f(fTemp151 + 3.5) : fTemp151 + 3.0)));
			double fTemp154 = ((-(0.8 * fTemp153) > fRec0[1]) ? fConst1468 : fConst1467);
			fRec0[0] = fRec0[1] * fTemp154 - 0.8 * fTemp153 * (1.0 - fTemp154);
			output0[i0] = FAUSTFLOAT(tanh(3.0 * std::pow(1e+01, 0.05 * fRec0[0]) * (fConst750 * fTemp81 * (fTemp82 + fConst754 * (fRec474[0] + fRec474[2])) + fConst710 * fTemp77 * (fTemp78 + fConst714 * (fRec449[0] + fRec449[2])) + fConst670 * fTemp73 * (fTemp74 + fConst674 * (fRec424[0] + fRec424[2])) + fConst630 * fTemp69 * (fTemp70 + fConst634 * (fRec399[0] + fRec399[2])) + fConst590 * fTemp65 * (fTemp66 + fConst594 * (fRec374[0] + fRec374[2])) + fConst550 * fTemp61 * (fTemp62 + fConst554 * (fRec349[0] + fRec349[2])) + fConst510 * fTemp57 * (fTemp58 + fConst514 * (fRec324[0] + fRec324[2])) + fConst470 * fTemp53 * (fTemp54 + fConst474 * (fRec299[0] + fRec299[2])) + fConst430 * fTemp49 * (fTemp50 + fConst434 * (fRec274[0] + fRec274[2])) + fConst390 * fTemp45 * (fTemp46 + fConst394 * (fRec249[0] + fRec249[2])) + fConst350 * fTemp41 * (fTemp42 + fConst354 * (fRec224[0] + fRec224[2])) + fConst310 * fTemp37 * (fTemp38 + fConst314 * (fRec199[0] + fRec199[2])) + fConst270 * fTemp33 * (fTemp34 + fConst274 * (fRec174[0] + fRec174[2])) + fConst230 * fTemp29 * (fTemp30 + fConst234 * (fRec149[0] + fRec149[2])) + fConst190 * fTemp25 * (fTemp26 + fConst194 * (fRec124[0] + fRec124[2])) + fConst150 * fTemp21 * (fTemp22 + fConst154 * (fRec99[0] + fRec99[2])) + fConst110 * fTemp17 * (fTemp18 + fConst114 * (fRec74[0] + fRec74[2])) + fConst70 * fTemp13 * (fTemp14 + fConst74 * (fRec49[0] + fRec49[2])) + fConst4 * fTemp7 * (fTemp10 + fConst8 * (fRec16[0] + fRec16[2])) + fConst1270 * fTemp133 * (fTemp134 + fConst1274 * (fRec799[0] + fRec799[2])) + fConst1230 * fTemp129 * (fTemp130 + fConst1234 * (fRec774[0] + fRec774[2])) + fConst1190 * fTemp125 * (fTemp126 + fConst1194 * (fRec749[0] + fRec749[2])) + fConst1150 * fTemp121 * (fTemp122 + fConst1154 * (fRec724[0] + fRec724[2])) + fConst950 * fTemp101 * (fTemp102 + fConst954 * (fRec599[0] + fRec599[2])) + fConst910 * fTemp97 * (fTemp98 + fConst914 * (fRec574[0] + fRec574[2])) + fConst870 * fTemp93 * (fTemp94 + fConst874 * (fRec549[0] + fRec549[2])) + fConst830 * fTemp89 * (fTemp90 + fConst834 * (fRec524[0] + fRec524[2])) + fConst790 * fTemp85 * (fTemp86 + fConst794 * (fRec499[0] + fRec499[2])) + fConst990 * fTemp105 * (fTemp106 + fConst994 * (fRec624[0] + fRec624[2])) + fConst1030 * fTemp109 * (fTemp110 + fConst1034 * (fRec649[0] + fRec649[2])) + fConst1070 * fTemp113 * (fTemp114 + fConst1074 * (fRec674[0] + fRec674[2])) + fConst1110 * fTemp117 * (fTemp118 + fConst1114 * (fRec699[0] + fRec699[2])) + fConst1310 * fTemp137 * (fTemp138 + fConst1314 * (fRec824[0] + fRec824[2])) + fConst1350 * fTemp141 * (fTemp142 + fConst1354 * (fRec849[0] + fRec849[2])) + fConst1390 * fTemp145 * (fTemp146 + fConst1394 * (fRec874[0] + fRec874[2])) + fConst1430 * fTemp149 * (fTemp150 + fConst1434 * (fRec899[0] + fRec899[2])))));
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
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
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec1[1] = fRec1[0];
			fRec28[1] = fRec28[0];
			iRec35[1] = iRec35[0];
			fVec0[1] = fVec0[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fVec1[1] = fVec1[0];
			fRec32[1] = fRec32[0];
			fVec2[1] = fVec2[0];
			fRec31[1] = fRec31[0];
			fVec3[1] = fVec3[0];
			fRec30[1] = fRec30[0];
			fVec4[1] = fVec4[0];
			fRec29[1] = fRec29[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[2] = fRec24[1];
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
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec60[2] = fRec60[1];
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
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fRec51[2] = fRec51[1];
			fRec51[1] = fRec51[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec72[2] = fRec72[1];
			fRec72[1] = fRec72[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec70[2] = fRec70[1];
			fRec70[1] = fRec70[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec68[2] = fRec68[1];
			fRec68[1] = fRec68[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec61[1] = fRec61[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fRec84[2] = fRec84[1];
			fRec84[1] = fRec84[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec76[2] = fRec76[1];
			fRec76[1] = fRec76[0];
			fRec75[2] = fRec75[1];
			fRec75[1] = fRec75[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec98[2] = fRec98[1];
			fRec98[1] = fRec98[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec96[2] = fRec96[1];
			fRec96[1] = fRec96[0];
			fRec95[2] = fRec95[1];
			fRec95[1] = fRec95[0];
			fRec94[2] = fRec94[1];
			fRec94[1] = fRec94[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec86[1] = fRec86[0];
			fRec110[2] = fRec110[1];
			fRec110[1] = fRec110[0];
			fRec109[2] = fRec109[1];
			fRec109[1] = fRec109[0];
			fRec108[2] = fRec108[1];
			fRec108[1] = fRec108[0];
			fRec107[2] = fRec107[1];
			fRec107[1] = fRec107[0];
			fRec106[2] = fRec106[1];
			fRec106[1] = fRec106[0];
			fRec105[2] = fRec105[1];
			fRec105[1] = fRec105[0];
			fRec104[2] = fRec104[1];
			fRec104[1] = fRec104[0];
			fRec103[2] = fRec103[1];
			fRec103[1] = fRec103[0];
			fRec102[2] = fRec102[1];
			fRec102[1] = fRec102[0];
			fRec101[2] = fRec101[1];
			fRec101[1] = fRec101[0];
			fRec100[2] = fRec100[1];
			fRec100[1] = fRec100[0];
			fRec99[2] = fRec99[1];
			fRec99[1] = fRec99[0];
			fRec123[2] = fRec123[1];
			fRec123[1] = fRec123[0];
			fRec122[2] = fRec122[1];
			fRec122[1] = fRec122[0];
			fRec121[2] = fRec121[1];
			fRec121[1] = fRec121[0];
			fRec120[2] = fRec120[1];
			fRec120[1] = fRec120[0];
			fRec119[2] = fRec119[1];
			fRec119[1] = fRec119[0];
			fRec118[2] = fRec118[1];
			fRec118[1] = fRec118[0];
			fRec117[2] = fRec117[1];
			fRec117[1] = fRec117[0];
			fRec116[2] = fRec116[1];
			fRec116[1] = fRec116[0];
			fRec115[2] = fRec115[1];
			fRec115[1] = fRec115[0];
			fRec114[2] = fRec114[1];
			fRec114[1] = fRec114[0];
			fRec113[2] = fRec113[1];
			fRec113[1] = fRec113[0];
			fRec112[2] = fRec112[1];
			fRec112[1] = fRec112[0];
			fRec111[1] = fRec111[0];
			fRec135[2] = fRec135[1];
			fRec135[1] = fRec135[0];
			fRec134[2] = fRec134[1];
			fRec134[1] = fRec134[0];
			fRec133[2] = fRec133[1];
			fRec133[1] = fRec133[0];
			fRec132[2] = fRec132[1];
			fRec132[1] = fRec132[0];
			fRec131[2] = fRec131[1];
			fRec131[1] = fRec131[0];
			fRec130[2] = fRec130[1];
			fRec130[1] = fRec130[0];
			fRec129[2] = fRec129[1];
			fRec129[1] = fRec129[0];
			fRec128[2] = fRec128[1];
			fRec128[1] = fRec128[0];
			fRec127[2] = fRec127[1];
			fRec127[1] = fRec127[0];
			fRec126[2] = fRec126[1];
			fRec126[1] = fRec126[0];
			fRec125[2] = fRec125[1];
			fRec125[1] = fRec125[0];
			fRec124[2] = fRec124[1];
			fRec124[1] = fRec124[0];
			fRec148[2] = fRec148[1];
			fRec148[1] = fRec148[0];
			fRec147[2] = fRec147[1];
			fRec147[1] = fRec147[0];
			fRec146[2] = fRec146[1];
			fRec146[1] = fRec146[0];
			fRec145[2] = fRec145[1];
			fRec145[1] = fRec145[0];
			fRec144[2] = fRec144[1];
			fRec144[1] = fRec144[0];
			fRec143[2] = fRec143[1];
			fRec143[1] = fRec143[0];
			fRec142[2] = fRec142[1];
			fRec142[1] = fRec142[0];
			fRec141[2] = fRec141[1];
			fRec141[1] = fRec141[0];
			fRec140[2] = fRec140[1];
			fRec140[1] = fRec140[0];
			fRec139[2] = fRec139[1];
			fRec139[1] = fRec139[0];
			fRec138[2] = fRec138[1];
			fRec138[1] = fRec138[0];
			fRec137[2] = fRec137[1];
			fRec137[1] = fRec137[0];
			fRec136[1] = fRec136[0];
			fRec160[2] = fRec160[1];
			fRec160[1] = fRec160[0];
			fRec159[2] = fRec159[1];
			fRec159[1] = fRec159[0];
			fRec158[2] = fRec158[1];
			fRec158[1] = fRec158[0];
			fRec157[2] = fRec157[1];
			fRec157[1] = fRec157[0];
			fRec156[2] = fRec156[1];
			fRec156[1] = fRec156[0];
			fRec155[2] = fRec155[1];
			fRec155[1] = fRec155[0];
			fRec154[2] = fRec154[1];
			fRec154[1] = fRec154[0];
			fRec153[2] = fRec153[1];
			fRec153[1] = fRec153[0];
			fRec152[2] = fRec152[1];
			fRec152[1] = fRec152[0];
			fRec151[2] = fRec151[1];
			fRec151[1] = fRec151[0];
			fRec150[2] = fRec150[1];
			fRec150[1] = fRec150[0];
			fRec149[2] = fRec149[1];
			fRec149[1] = fRec149[0];
			fRec173[2] = fRec173[1];
			fRec173[1] = fRec173[0];
			fRec172[2] = fRec172[1];
			fRec172[1] = fRec172[0];
			fRec171[2] = fRec171[1];
			fRec171[1] = fRec171[0];
			fRec170[2] = fRec170[1];
			fRec170[1] = fRec170[0];
			fRec169[2] = fRec169[1];
			fRec169[1] = fRec169[0];
			fRec168[2] = fRec168[1];
			fRec168[1] = fRec168[0];
			fRec167[2] = fRec167[1];
			fRec167[1] = fRec167[0];
			fRec166[2] = fRec166[1];
			fRec166[1] = fRec166[0];
			fRec165[2] = fRec165[1];
			fRec165[1] = fRec165[0];
			fRec164[2] = fRec164[1];
			fRec164[1] = fRec164[0];
			fRec163[2] = fRec163[1];
			fRec163[1] = fRec163[0];
			fRec162[2] = fRec162[1];
			fRec162[1] = fRec162[0];
			fRec161[1] = fRec161[0];
			fRec185[2] = fRec185[1];
			fRec185[1] = fRec185[0];
			fRec184[2] = fRec184[1];
			fRec184[1] = fRec184[0];
			fRec183[2] = fRec183[1];
			fRec183[1] = fRec183[0];
			fRec182[2] = fRec182[1];
			fRec182[1] = fRec182[0];
			fRec181[2] = fRec181[1];
			fRec181[1] = fRec181[0];
			fRec180[2] = fRec180[1];
			fRec180[1] = fRec180[0];
			fRec179[2] = fRec179[1];
			fRec179[1] = fRec179[0];
			fRec178[2] = fRec178[1];
			fRec178[1] = fRec178[0];
			fRec177[2] = fRec177[1];
			fRec177[1] = fRec177[0];
			fRec176[2] = fRec176[1];
			fRec176[1] = fRec176[0];
			fRec175[2] = fRec175[1];
			fRec175[1] = fRec175[0];
			fRec174[2] = fRec174[1];
			fRec174[1] = fRec174[0];
			fRec198[2] = fRec198[1];
			fRec198[1] = fRec198[0];
			fRec197[2] = fRec197[1];
			fRec197[1] = fRec197[0];
			fRec196[2] = fRec196[1];
			fRec196[1] = fRec196[0];
			fRec195[2] = fRec195[1];
			fRec195[1] = fRec195[0];
			fRec194[2] = fRec194[1];
			fRec194[1] = fRec194[0];
			fRec193[2] = fRec193[1];
			fRec193[1] = fRec193[0];
			fRec192[2] = fRec192[1];
			fRec192[1] = fRec192[0];
			fRec191[2] = fRec191[1];
			fRec191[1] = fRec191[0];
			fRec190[2] = fRec190[1];
			fRec190[1] = fRec190[0];
			fRec189[2] = fRec189[1];
			fRec189[1] = fRec189[0];
			fRec188[2] = fRec188[1];
			fRec188[1] = fRec188[0];
			fRec187[2] = fRec187[1];
			fRec187[1] = fRec187[0];
			fRec186[1] = fRec186[0];
			fRec210[2] = fRec210[1];
			fRec210[1] = fRec210[0];
			fRec209[2] = fRec209[1];
			fRec209[1] = fRec209[0];
			fRec208[2] = fRec208[1];
			fRec208[1] = fRec208[0];
			fRec207[2] = fRec207[1];
			fRec207[1] = fRec207[0];
			fRec206[2] = fRec206[1];
			fRec206[1] = fRec206[0];
			fRec205[2] = fRec205[1];
			fRec205[1] = fRec205[0];
			fRec204[2] = fRec204[1];
			fRec204[1] = fRec204[0];
			fRec203[2] = fRec203[1];
			fRec203[1] = fRec203[0];
			fRec202[2] = fRec202[1];
			fRec202[1] = fRec202[0];
			fRec201[2] = fRec201[1];
			fRec201[1] = fRec201[0];
			fRec200[2] = fRec200[1];
			fRec200[1] = fRec200[0];
			fRec199[2] = fRec199[1];
			fRec199[1] = fRec199[0];
			fRec223[2] = fRec223[1];
			fRec223[1] = fRec223[0];
			fRec222[2] = fRec222[1];
			fRec222[1] = fRec222[0];
			fRec221[2] = fRec221[1];
			fRec221[1] = fRec221[0];
			fRec220[2] = fRec220[1];
			fRec220[1] = fRec220[0];
			fRec219[2] = fRec219[1];
			fRec219[1] = fRec219[0];
			fRec218[2] = fRec218[1];
			fRec218[1] = fRec218[0];
			fRec217[2] = fRec217[1];
			fRec217[1] = fRec217[0];
			fRec216[2] = fRec216[1];
			fRec216[1] = fRec216[0];
			fRec215[2] = fRec215[1];
			fRec215[1] = fRec215[0];
			fRec214[2] = fRec214[1];
			fRec214[1] = fRec214[0];
			fRec213[2] = fRec213[1];
			fRec213[1] = fRec213[0];
			fRec212[2] = fRec212[1];
			fRec212[1] = fRec212[0];
			fRec211[1] = fRec211[0];
			fRec235[2] = fRec235[1];
			fRec235[1] = fRec235[0];
			fRec234[2] = fRec234[1];
			fRec234[1] = fRec234[0];
			fRec233[2] = fRec233[1];
			fRec233[1] = fRec233[0];
			fRec232[2] = fRec232[1];
			fRec232[1] = fRec232[0];
			fRec231[2] = fRec231[1];
			fRec231[1] = fRec231[0];
			fRec230[2] = fRec230[1];
			fRec230[1] = fRec230[0];
			fRec229[2] = fRec229[1];
			fRec229[1] = fRec229[0];
			fRec228[2] = fRec228[1];
			fRec228[1] = fRec228[0];
			fRec227[2] = fRec227[1];
			fRec227[1] = fRec227[0];
			fRec226[2] = fRec226[1];
			fRec226[1] = fRec226[0];
			fRec225[2] = fRec225[1];
			fRec225[1] = fRec225[0];
			fRec224[2] = fRec224[1];
			fRec224[1] = fRec224[0];
			fRec248[2] = fRec248[1];
			fRec248[1] = fRec248[0];
			fRec247[2] = fRec247[1];
			fRec247[1] = fRec247[0];
			fRec246[2] = fRec246[1];
			fRec246[1] = fRec246[0];
			fRec245[2] = fRec245[1];
			fRec245[1] = fRec245[0];
			fRec244[2] = fRec244[1];
			fRec244[1] = fRec244[0];
			fRec243[2] = fRec243[1];
			fRec243[1] = fRec243[0];
			fRec242[2] = fRec242[1];
			fRec242[1] = fRec242[0];
			fRec241[2] = fRec241[1];
			fRec241[1] = fRec241[0];
			fRec240[2] = fRec240[1];
			fRec240[1] = fRec240[0];
			fRec239[2] = fRec239[1];
			fRec239[1] = fRec239[0];
			fRec238[2] = fRec238[1];
			fRec238[1] = fRec238[0];
			fRec237[2] = fRec237[1];
			fRec237[1] = fRec237[0];
			fRec236[1] = fRec236[0];
			fRec260[2] = fRec260[1];
			fRec260[1] = fRec260[0];
			fRec259[2] = fRec259[1];
			fRec259[1] = fRec259[0];
			fRec258[2] = fRec258[1];
			fRec258[1] = fRec258[0];
			fRec257[2] = fRec257[1];
			fRec257[1] = fRec257[0];
			fRec256[2] = fRec256[1];
			fRec256[1] = fRec256[0];
			fRec255[2] = fRec255[1];
			fRec255[1] = fRec255[0];
			fRec254[2] = fRec254[1];
			fRec254[1] = fRec254[0];
			fRec253[2] = fRec253[1];
			fRec253[1] = fRec253[0];
			fRec252[2] = fRec252[1];
			fRec252[1] = fRec252[0];
			fRec251[2] = fRec251[1];
			fRec251[1] = fRec251[0];
			fRec250[2] = fRec250[1];
			fRec250[1] = fRec250[0];
			fRec249[2] = fRec249[1];
			fRec249[1] = fRec249[0];
			fRec273[2] = fRec273[1];
			fRec273[1] = fRec273[0];
			fRec272[2] = fRec272[1];
			fRec272[1] = fRec272[0];
			fRec271[2] = fRec271[1];
			fRec271[1] = fRec271[0];
			fRec270[2] = fRec270[1];
			fRec270[1] = fRec270[0];
			fRec269[2] = fRec269[1];
			fRec269[1] = fRec269[0];
			fRec268[2] = fRec268[1];
			fRec268[1] = fRec268[0];
			fRec267[2] = fRec267[1];
			fRec267[1] = fRec267[0];
			fRec266[2] = fRec266[1];
			fRec266[1] = fRec266[0];
			fRec265[2] = fRec265[1];
			fRec265[1] = fRec265[0];
			fRec264[2] = fRec264[1];
			fRec264[1] = fRec264[0];
			fRec263[2] = fRec263[1];
			fRec263[1] = fRec263[0];
			fRec262[2] = fRec262[1];
			fRec262[1] = fRec262[0];
			fRec261[1] = fRec261[0];
			fRec285[2] = fRec285[1];
			fRec285[1] = fRec285[0];
			fRec284[2] = fRec284[1];
			fRec284[1] = fRec284[0];
			fRec283[2] = fRec283[1];
			fRec283[1] = fRec283[0];
			fRec282[2] = fRec282[1];
			fRec282[1] = fRec282[0];
			fRec281[2] = fRec281[1];
			fRec281[1] = fRec281[0];
			fRec280[2] = fRec280[1];
			fRec280[1] = fRec280[0];
			fRec279[2] = fRec279[1];
			fRec279[1] = fRec279[0];
			fRec278[2] = fRec278[1];
			fRec278[1] = fRec278[0];
			fRec277[2] = fRec277[1];
			fRec277[1] = fRec277[0];
			fRec276[2] = fRec276[1];
			fRec276[1] = fRec276[0];
			fRec275[2] = fRec275[1];
			fRec275[1] = fRec275[0];
			fRec274[2] = fRec274[1];
			fRec274[1] = fRec274[0];
			fRec298[2] = fRec298[1];
			fRec298[1] = fRec298[0];
			fRec297[2] = fRec297[1];
			fRec297[1] = fRec297[0];
			fRec296[2] = fRec296[1];
			fRec296[1] = fRec296[0];
			fRec295[2] = fRec295[1];
			fRec295[1] = fRec295[0];
			fRec294[2] = fRec294[1];
			fRec294[1] = fRec294[0];
			fRec293[2] = fRec293[1];
			fRec293[1] = fRec293[0];
			fRec292[2] = fRec292[1];
			fRec292[1] = fRec292[0];
			fRec291[2] = fRec291[1];
			fRec291[1] = fRec291[0];
			fRec290[2] = fRec290[1];
			fRec290[1] = fRec290[0];
			fRec289[2] = fRec289[1];
			fRec289[1] = fRec289[0];
			fRec288[2] = fRec288[1];
			fRec288[1] = fRec288[0];
			fRec287[2] = fRec287[1];
			fRec287[1] = fRec287[0];
			fRec286[1] = fRec286[0];
			fRec310[2] = fRec310[1];
			fRec310[1] = fRec310[0];
			fRec309[2] = fRec309[1];
			fRec309[1] = fRec309[0];
			fRec308[2] = fRec308[1];
			fRec308[1] = fRec308[0];
			fRec307[2] = fRec307[1];
			fRec307[1] = fRec307[0];
			fRec306[2] = fRec306[1];
			fRec306[1] = fRec306[0];
			fRec305[2] = fRec305[1];
			fRec305[1] = fRec305[0];
			fRec304[2] = fRec304[1];
			fRec304[1] = fRec304[0];
			fRec303[2] = fRec303[1];
			fRec303[1] = fRec303[0];
			fRec302[2] = fRec302[1];
			fRec302[1] = fRec302[0];
			fRec301[2] = fRec301[1];
			fRec301[1] = fRec301[0];
			fRec300[2] = fRec300[1];
			fRec300[1] = fRec300[0];
			fRec299[2] = fRec299[1];
			fRec299[1] = fRec299[0];
			fRec323[2] = fRec323[1];
			fRec323[1] = fRec323[0];
			fRec322[2] = fRec322[1];
			fRec322[1] = fRec322[0];
			fRec321[2] = fRec321[1];
			fRec321[1] = fRec321[0];
			fRec320[2] = fRec320[1];
			fRec320[1] = fRec320[0];
			fRec319[2] = fRec319[1];
			fRec319[1] = fRec319[0];
			fRec318[2] = fRec318[1];
			fRec318[1] = fRec318[0];
			fRec317[2] = fRec317[1];
			fRec317[1] = fRec317[0];
			fRec316[2] = fRec316[1];
			fRec316[1] = fRec316[0];
			fRec315[2] = fRec315[1];
			fRec315[1] = fRec315[0];
			fRec314[2] = fRec314[1];
			fRec314[1] = fRec314[0];
			fRec313[2] = fRec313[1];
			fRec313[1] = fRec313[0];
			fRec312[2] = fRec312[1];
			fRec312[1] = fRec312[0];
			fRec311[1] = fRec311[0];
			fRec335[2] = fRec335[1];
			fRec335[1] = fRec335[0];
			fRec334[2] = fRec334[1];
			fRec334[1] = fRec334[0];
			fRec333[2] = fRec333[1];
			fRec333[1] = fRec333[0];
			fRec332[2] = fRec332[1];
			fRec332[1] = fRec332[0];
			fRec331[2] = fRec331[1];
			fRec331[1] = fRec331[0];
			fRec330[2] = fRec330[1];
			fRec330[1] = fRec330[0];
			fRec329[2] = fRec329[1];
			fRec329[1] = fRec329[0];
			fRec328[2] = fRec328[1];
			fRec328[1] = fRec328[0];
			fRec327[2] = fRec327[1];
			fRec327[1] = fRec327[0];
			fRec326[2] = fRec326[1];
			fRec326[1] = fRec326[0];
			fRec325[2] = fRec325[1];
			fRec325[1] = fRec325[0];
			fRec324[2] = fRec324[1];
			fRec324[1] = fRec324[0];
			fRec348[2] = fRec348[1];
			fRec348[1] = fRec348[0];
			fRec347[2] = fRec347[1];
			fRec347[1] = fRec347[0];
			fRec346[2] = fRec346[1];
			fRec346[1] = fRec346[0];
			fRec345[2] = fRec345[1];
			fRec345[1] = fRec345[0];
			fRec344[2] = fRec344[1];
			fRec344[1] = fRec344[0];
			fRec343[2] = fRec343[1];
			fRec343[1] = fRec343[0];
			fRec342[2] = fRec342[1];
			fRec342[1] = fRec342[0];
			fRec341[2] = fRec341[1];
			fRec341[1] = fRec341[0];
			fRec340[2] = fRec340[1];
			fRec340[1] = fRec340[0];
			fRec339[2] = fRec339[1];
			fRec339[1] = fRec339[0];
			fRec338[2] = fRec338[1];
			fRec338[1] = fRec338[0];
			fRec337[2] = fRec337[1];
			fRec337[1] = fRec337[0];
			fRec336[1] = fRec336[0];
			fRec360[2] = fRec360[1];
			fRec360[1] = fRec360[0];
			fRec359[2] = fRec359[1];
			fRec359[1] = fRec359[0];
			fRec358[2] = fRec358[1];
			fRec358[1] = fRec358[0];
			fRec357[2] = fRec357[1];
			fRec357[1] = fRec357[0];
			fRec356[2] = fRec356[1];
			fRec356[1] = fRec356[0];
			fRec355[2] = fRec355[1];
			fRec355[1] = fRec355[0];
			fRec354[2] = fRec354[1];
			fRec354[1] = fRec354[0];
			fRec353[2] = fRec353[1];
			fRec353[1] = fRec353[0];
			fRec352[2] = fRec352[1];
			fRec352[1] = fRec352[0];
			fRec351[2] = fRec351[1];
			fRec351[1] = fRec351[0];
			fRec350[2] = fRec350[1];
			fRec350[1] = fRec350[0];
			fRec349[2] = fRec349[1];
			fRec349[1] = fRec349[0];
			fRec373[2] = fRec373[1];
			fRec373[1] = fRec373[0];
			fRec372[2] = fRec372[1];
			fRec372[1] = fRec372[0];
			fRec371[2] = fRec371[1];
			fRec371[1] = fRec371[0];
			fRec370[2] = fRec370[1];
			fRec370[1] = fRec370[0];
			fRec369[2] = fRec369[1];
			fRec369[1] = fRec369[0];
			fRec368[2] = fRec368[1];
			fRec368[1] = fRec368[0];
			fRec367[2] = fRec367[1];
			fRec367[1] = fRec367[0];
			fRec366[2] = fRec366[1];
			fRec366[1] = fRec366[0];
			fRec365[2] = fRec365[1];
			fRec365[1] = fRec365[0];
			fRec364[2] = fRec364[1];
			fRec364[1] = fRec364[0];
			fRec363[2] = fRec363[1];
			fRec363[1] = fRec363[0];
			fRec362[2] = fRec362[1];
			fRec362[1] = fRec362[0];
			fRec361[1] = fRec361[0];
			fRec385[2] = fRec385[1];
			fRec385[1] = fRec385[0];
			fRec384[2] = fRec384[1];
			fRec384[1] = fRec384[0];
			fRec383[2] = fRec383[1];
			fRec383[1] = fRec383[0];
			fRec382[2] = fRec382[1];
			fRec382[1] = fRec382[0];
			fRec381[2] = fRec381[1];
			fRec381[1] = fRec381[0];
			fRec380[2] = fRec380[1];
			fRec380[1] = fRec380[0];
			fRec379[2] = fRec379[1];
			fRec379[1] = fRec379[0];
			fRec378[2] = fRec378[1];
			fRec378[1] = fRec378[0];
			fRec377[2] = fRec377[1];
			fRec377[1] = fRec377[0];
			fRec376[2] = fRec376[1];
			fRec376[1] = fRec376[0];
			fRec375[2] = fRec375[1];
			fRec375[1] = fRec375[0];
			fRec374[2] = fRec374[1];
			fRec374[1] = fRec374[0];
			fRec398[2] = fRec398[1];
			fRec398[1] = fRec398[0];
			fRec397[2] = fRec397[1];
			fRec397[1] = fRec397[0];
			fRec396[2] = fRec396[1];
			fRec396[1] = fRec396[0];
			fRec395[2] = fRec395[1];
			fRec395[1] = fRec395[0];
			fRec394[2] = fRec394[1];
			fRec394[1] = fRec394[0];
			fRec393[2] = fRec393[1];
			fRec393[1] = fRec393[0];
			fRec392[2] = fRec392[1];
			fRec392[1] = fRec392[0];
			fRec391[2] = fRec391[1];
			fRec391[1] = fRec391[0];
			fRec390[2] = fRec390[1];
			fRec390[1] = fRec390[0];
			fRec389[2] = fRec389[1];
			fRec389[1] = fRec389[0];
			fRec388[2] = fRec388[1];
			fRec388[1] = fRec388[0];
			fRec387[2] = fRec387[1];
			fRec387[1] = fRec387[0];
			fRec386[1] = fRec386[0];
			fRec410[2] = fRec410[1];
			fRec410[1] = fRec410[0];
			fRec409[2] = fRec409[1];
			fRec409[1] = fRec409[0];
			fRec408[2] = fRec408[1];
			fRec408[1] = fRec408[0];
			fRec407[2] = fRec407[1];
			fRec407[1] = fRec407[0];
			fRec406[2] = fRec406[1];
			fRec406[1] = fRec406[0];
			fRec405[2] = fRec405[1];
			fRec405[1] = fRec405[0];
			fRec404[2] = fRec404[1];
			fRec404[1] = fRec404[0];
			fRec403[2] = fRec403[1];
			fRec403[1] = fRec403[0];
			fRec402[2] = fRec402[1];
			fRec402[1] = fRec402[0];
			fRec401[2] = fRec401[1];
			fRec401[1] = fRec401[0];
			fRec400[2] = fRec400[1];
			fRec400[1] = fRec400[0];
			fRec399[2] = fRec399[1];
			fRec399[1] = fRec399[0];
			fRec423[2] = fRec423[1];
			fRec423[1] = fRec423[0];
			fRec422[2] = fRec422[1];
			fRec422[1] = fRec422[0];
			fRec421[2] = fRec421[1];
			fRec421[1] = fRec421[0];
			fRec420[2] = fRec420[1];
			fRec420[1] = fRec420[0];
			fRec419[2] = fRec419[1];
			fRec419[1] = fRec419[0];
			fRec418[2] = fRec418[1];
			fRec418[1] = fRec418[0];
			fRec417[2] = fRec417[1];
			fRec417[1] = fRec417[0];
			fRec416[2] = fRec416[1];
			fRec416[1] = fRec416[0];
			fRec415[2] = fRec415[1];
			fRec415[1] = fRec415[0];
			fRec414[2] = fRec414[1];
			fRec414[1] = fRec414[0];
			fRec413[2] = fRec413[1];
			fRec413[1] = fRec413[0];
			fRec412[2] = fRec412[1];
			fRec412[1] = fRec412[0];
			fRec411[1] = fRec411[0];
			fRec435[2] = fRec435[1];
			fRec435[1] = fRec435[0];
			fRec434[2] = fRec434[1];
			fRec434[1] = fRec434[0];
			fRec433[2] = fRec433[1];
			fRec433[1] = fRec433[0];
			fRec432[2] = fRec432[1];
			fRec432[1] = fRec432[0];
			fRec431[2] = fRec431[1];
			fRec431[1] = fRec431[0];
			fRec430[2] = fRec430[1];
			fRec430[1] = fRec430[0];
			fRec429[2] = fRec429[1];
			fRec429[1] = fRec429[0];
			fRec428[2] = fRec428[1];
			fRec428[1] = fRec428[0];
			fRec427[2] = fRec427[1];
			fRec427[1] = fRec427[0];
			fRec426[2] = fRec426[1];
			fRec426[1] = fRec426[0];
			fRec425[2] = fRec425[1];
			fRec425[1] = fRec425[0];
			fRec424[2] = fRec424[1];
			fRec424[1] = fRec424[0];
			fRec448[2] = fRec448[1];
			fRec448[1] = fRec448[0];
			fRec447[2] = fRec447[1];
			fRec447[1] = fRec447[0];
			fRec446[2] = fRec446[1];
			fRec446[1] = fRec446[0];
			fRec445[2] = fRec445[1];
			fRec445[1] = fRec445[0];
			fRec444[2] = fRec444[1];
			fRec444[1] = fRec444[0];
			fRec443[2] = fRec443[1];
			fRec443[1] = fRec443[0];
			fRec442[2] = fRec442[1];
			fRec442[1] = fRec442[0];
			fRec441[2] = fRec441[1];
			fRec441[1] = fRec441[0];
			fRec440[2] = fRec440[1];
			fRec440[1] = fRec440[0];
			fRec439[2] = fRec439[1];
			fRec439[1] = fRec439[0];
			fRec438[2] = fRec438[1];
			fRec438[1] = fRec438[0];
			fRec437[2] = fRec437[1];
			fRec437[1] = fRec437[0];
			fRec436[1] = fRec436[0];
			fRec460[2] = fRec460[1];
			fRec460[1] = fRec460[0];
			fRec459[2] = fRec459[1];
			fRec459[1] = fRec459[0];
			fRec458[2] = fRec458[1];
			fRec458[1] = fRec458[0];
			fRec457[2] = fRec457[1];
			fRec457[1] = fRec457[0];
			fRec456[2] = fRec456[1];
			fRec456[1] = fRec456[0];
			fRec455[2] = fRec455[1];
			fRec455[1] = fRec455[0];
			fRec454[2] = fRec454[1];
			fRec454[1] = fRec454[0];
			fRec453[2] = fRec453[1];
			fRec453[1] = fRec453[0];
			fRec452[2] = fRec452[1];
			fRec452[1] = fRec452[0];
			fRec451[2] = fRec451[1];
			fRec451[1] = fRec451[0];
			fRec450[2] = fRec450[1];
			fRec450[1] = fRec450[0];
			fRec449[2] = fRec449[1];
			fRec449[1] = fRec449[0];
			fRec473[2] = fRec473[1];
			fRec473[1] = fRec473[0];
			fRec472[2] = fRec472[1];
			fRec472[1] = fRec472[0];
			fRec471[2] = fRec471[1];
			fRec471[1] = fRec471[0];
			fRec470[2] = fRec470[1];
			fRec470[1] = fRec470[0];
			fRec469[2] = fRec469[1];
			fRec469[1] = fRec469[0];
			fRec468[2] = fRec468[1];
			fRec468[1] = fRec468[0];
			fRec467[2] = fRec467[1];
			fRec467[1] = fRec467[0];
			fRec466[2] = fRec466[1];
			fRec466[1] = fRec466[0];
			fRec465[2] = fRec465[1];
			fRec465[1] = fRec465[0];
			fRec464[2] = fRec464[1];
			fRec464[1] = fRec464[0];
			fRec463[2] = fRec463[1];
			fRec463[1] = fRec463[0];
			fRec462[2] = fRec462[1];
			fRec462[1] = fRec462[0];
			fRec461[1] = fRec461[0];
			fRec485[2] = fRec485[1];
			fRec485[1] = fRec485[0];
			fRec484[2] = fRec484[1];
			fRec484[1] = fRec484[0];
			fRec483[2] = fRec483[1];
			fRec483[1] = fRec483[0];
			fRec482[2] = fRec482[1];
			fRec482[1] = fRec482[0];
			fRec481[2] = fRec481[1];
			fRec481[1] = fRec481[0];
			fRec480[2] = fRec480[1];
			fRec480[1] = fRec480[0];
			fRec479[2] = fRec479[1];
			fRec479[1] = fRec479[0];
			fRec478[2] = fRec478[1];
			fRec478[1] = fRec478[0];
			fRec477[2] = fRec477[1];
			fRec477[1] = fRec477[0];
			fRec476[2] = fRec476[1];
			fRec476[1] = fRec476[0];
			fRec475[2] = fRec475[1];
			fRec475[1] = fRec475[0];
			fRec474[2] = fRec474[1];
			fRec474[1] = fRec474[0];
			fRec498[2] = fRec498[1];
			fRec498[1] = fRec498[0];
			fRec497[2] = fRec497[1];
			fRec497[1] = fRec497[0];
			fRec496[2] = fRec496[1];
			fRec496[1] = fRec496[0];
			fRec495[2] = fRec495[1];
			fRec495[1] = fRec495[0];
			fRec494[2] = fRec494[1];
			fRec494[1] = fRec494[0];
			fRec493[2] = fRec493[1];
			fRec493[1] = fRec493[0];
			fRec492[2] = fRec492[1];
			fRec492[1] = fRec492[0];
			fRec491[2] = fRec491[1];
			fRec491[1] = fRec491[0];
			fRec490[2] = fRec490[1];
			fRec490[1] = fRec490[0];
			fRec489[2] = fRec489[1];
			fRec489[1] = fRec489[0];
			fRec488[2] = fRec488[1];
			fRec488[1] = fRec488[0];
			fRec487[2] = fRec487[1];
			fRec487[1] = fRec487[0];
			fRec486[1] = fRec486[0];
			fRec510[2] = fRec510[1];
			fRec510[1] = fRec510[0];
			fRec509[2] = fRec509[1];
			fRec509[1] = fRec509[0];
			fRec508[2] = fRec508[1];
			fRec508[1] = fRec508[0];
			fRec507[2] = fRec507[1];
			fRec507[1] = fRec507[0];
			fRec506[2] = fRec506[1];
			fRec506[1] = fRec506[0];
			fRec505[2] = fRec505[1];
			fRec505[1] = fRec505[0];
			fRec504[2] = fRec504[1];
			fRec504[1] = fRec504[0];
			fRec503[2] = fRec503[1];
			fRec503[1] = fRec503[0];
			fRec502[2] = fRec502[1];
			fRec502[1] = fRec502[0];
			fRec501[2] = fRec501[1];
			fRec501[1] = fRec501[0];
			fRec500[2] = fRec500[1];
			fRec500[1] = fRec500[0];
			fRec499[2] = fRec499[1];
			fRec499[1] = fRec499[0];
			fRec523[2] = fRec523[1];
			fRec523[1] = fRec523[0];
			fRec522[2] = fRec522[1];
			fRec522[1] = fRec522[0];
			fRec521[2] = fRec521[1];
			fRec521[1] = fRec521[0];
			fRec520[2] = fRec520[1];
			fRec520[1] = fRec520[0];
			fRec519[2] = fRec519[1];
			fRec519[1] = fRec519[0];
			fRec518[2] = fRec518[1];
			fRec518[1] = fRec518[0];
			fRec517[2] = fRec517[1];
			fRec517[1] = fRec517[0];
			fRec516[2] = fRec516[1];
			fRec516[1] = fRec516[0];
			fRec515[2] = fRec515[1];
			fRec515[1] = fRec515[0];
			fRec514[2] = fRec514[1];
			fRec514[1] = fRec514[0];
			fRec513[2] = fRec513[1];
			fRec513[1] = fRec513[0];
			fRec512[2] = fRec512[1];
			fRec512[1] = fRec512[0];
			fRec511[1] = fRec511[0];
			fRec535[2] = fRec535[1];
			fRec535[1] = fRec535[0];
			fRec534[2] = fRec534[1];
			fRec534[1] = fRec534[0];
			fRec533[2] = fRec533[1];
			fRec533[1] = fRec533[0];
			fRec532[2] = fRec532[1];
			fRec532[1] = fRec532[0];
			fRec531[2] = fRec531[1];
			fRec531[1] = fRec531[0];
			fRec530[2] = fRec530[1];
			fRec530[1] = fRec530[0];
			fRec529[2] = fRec529[1];
			fRec529[1] = fRec529[0];
			fRec528[2] = fRec528[1];
			fRec528[1] = fRec528[0];
			fRec527[2] = fRec527[1];
			fRec527[1] = fRec527[0];
			fRec526[2] = fRec526[1];
			fRec526[1] = fRec526[0];
			fRec525[2] = fRec525[1];
			fRec525[1] = fRec525[0];
			fRec524[2] = fRec524[1];
			fRec524[1] = fRec524[0];
			fRec548[2] = fRec548[1];
			fRec548[1] = fRec548[0];
			fRec547[2] = fRec547[1];
			fRec547[1] = fRec547[0];
			fRec546[2] = fRec546[1];
			fRec546[1] = fRec546[0];
			fRec545[2] = fRec545[1];
			fRec545[1] = fRec545[0];
			fRec544[2] = fRec544[1];
			fRec544[1] = fRec544[0];
			fRec543[2] = fRec543[1];
			fRec543[1] = fRec543[0];
			fRec542[2] = fRec542[1];
			fRec542[1] = fRec542[0];
			fRec541[2] = fRec541[1];
			fRec541[1] = fRec541[0];
			fRec540[2] = fRec540[1];
			fRec540[1] = fRec540[0];
			fRec539[2] = fRec539[1];
			fRec539[1] = fRec539[0];
			fRec538[2] = fRec538[1];
			fRec538[1] = fRec538[0];
			fRec537[2] = fRec537[1];
			fRec537[1] = fRec537[0];
			fRec536[1] = fRec536[0];
			fRec560[2] = fRec560[1];
			fRec560[1] = fRec560[0];
			fRec559[2] = fRec559[1];
			fRec559[1] = fRec559[0];
			fRec558[2] = fRec558[1];
			fRec558[1] = fRec558[0];
			fRec557[2] = fRec557[1];
			fRec557[1] = fRec557[0];
			fRec556[2] = fRec556[1];
			fRec556[1] = fRec556[0];
			fRec555[2] = fRec555[1];
			fRec555[1] = fRec555[0];
			fRec554[2] = fRec554[1];
			fRec554[1] = fRec554[0];
			fRec553[2] = fRec553[1];
			fRec553[1] = fRec553[0];
			fRec552[2] = fRec552[1];
			fRec552[1] = fRec552[0];
			fRec551[2] = fRec551[1];
			fRec551[1] = fRec551[0];
			fRec550[2] = fRec550[1];
			fRec550[1] = fRec550[0];
			fRec549[2] = fRec549[1];
			fRec549[1] = fRec549[0];
			fRec573[2] = fRec573[1];
			fRec573[1] = fRec573[0];
			fRec572[2] = fRec572[1];
			fRec572[1] = fRec572[0];
			fRec571[2] = fRec571[1];
			fRec571[1] = fRec571[0];
			fRec570[2] = fRec570[1];
			fRec570[1] = fRec570[0];
			fRec569[2] = fRec569[1];
			fRec569[1] = fRec569[0];
			fRec568[2] = fRec568[1];
			fRec568[1] = fRec568[0];
			fRec567[2] = fRec567[1];
			fRec567[1] = fRec567[0];
			fRec566[2] = fRec566[1];
			fRec566[1] = fRec566[0];
			fRec565[2] = fRec565[1];
			fRec565[1] = fRec565[0];
			fRec564[2] = fRec564[1];
			fRec564[1] = fRec564[0];
			fRec563[2] = fRec563[1];
			fRec563[1] = fRec563[0];
			fRec562[2] = fRec562[1];
			fRec562[1] = fRec562[0];
			fRec561[1] = fRec561[0];
			fRec585[2] = fRec585[1];
			fRec585[1] = fRec585[0];
			fRec584[2] = fRec584[1];
			fRec584[1] = fRec584[0];
			fRec583[2] = fRec583[1];
			fRec583[1] = fRec583[0];
			fRec582[2] = fRec582[1];
			fRec582[1] = fRec582[0];
			fRec581[2] = fRec581[1];
			fRec581[1] = fRec581[0];
			fRec580[2] = fRec580[1];
			fRec580[1] = fRec580[0];
			fRec579[2] = fRec579[1];
			fRec579[1] = fRec579[0];
			fRec578[2] = fRec578[1];
			fRec578[1] = fRec578[0];
			fRec577[2] = fRec577[1];
			fRec577[1] = fRec577[0];
			fRec576[2] = fRec576[1];
			fRec576[1] = fRec576[0];
			fRec575[2] = fRec575[1];
			fRec575[1] = fRec575[0];
			fRec574[2] = fRec574[1];
			fRec574[1] = fRec574[0];
			fRec598[2] = fRec598[1];
			fRec598[1] = fRec598[0];
			fRec597[2] = fRec597[1];
			fRec597[1] = fRec597[0];
			fRec596[2] = fRec596[1];
			fRec596[1] = fRec596[0];
			fRec595[2] = fRec595[1];
			fRec595[1] = fRec595[0];
			fRec594[2] = fRec594[1];
			fRec594[1] = fRec594[0];
			fRec593[2] = fRec593[1];
			fRec593[1] = fRec593[0];
			fRec592[2] = fRec592[1];
			fRec592[1] = fRec592[0];
			fRec591[2] = fRec591[1];
			fRec591[1] = fRec591[0];
			fRec590[2] = fRec590[1];
			fRec590[1] = fRec590[0];
			fRec589[2] = fRec589[1];
			fRec589[1] = fRec589[0];
			fRec588[2] = fRec588[1];
			fRec588[1] = fRec588[0];
			fRec587[2] = fRec587[1];
			fRec587[1] = fRec587[0];
			fRec586[1] = fRec586[0];
			fRec610[2] = fRec610[1];
			fRec610[1] = fRec610[0];
			fRec609[2] = fRec609[1];
			fRec609[1] = fRec609[0];
			fRec608[2] = fRec608[1];
			fRec608[1] = fRec608[0];
			fRec607[2] = fRec607[1];
			fRec607[1] = fRec607[0];
			fRec606[2] = fRec606[1];
			fRec606[1] = fRec606[0];
			fRec605[2] = fRec605[1];
			fRec605[1] = fRec605[0];
			fRec604[2] = fRec604[1];
			fRec604[1] = fRec604[0];
			fRec603[2] = fRec603[1];
			fRec603[1] = fRec603[0];
			fRec602[2] = fRec602[1];
			fRec602[1] = fRec602[0];
			fRec601[2] = fRec601[1];
			fRec601[1] = fRec601[0];
			fRec600[2] = fRec600[1];
			fRec600[1] = fRec600[0];
			fRec599[2] = fRec599[1];
			fRec599[1] = fRec599[0];
			fRec623[2] = fRec623[1];
			fRec623[1] = fRec623[0];
			fRec622[2] = fRec622[1];
			fRec622[1] = fRec622[0];
			fRec621[2] = fRec621[1];
			fRec621[1] = fRec621[0];
			fRec620[2] = fRec620[1];
			fRec620[1] = fRec620[0];
			fRec619[2] = fRec619[1];
			fRec619[1] = fRec619[0];
			fRec618[2] = fRec618[1];
			fRec618[1] = fRec618[0];
			fRec617[2] = fRec617[1];
			fRec617[1] = fRec617[0];
			fRec616[2] = fRec616[1];
			fRec616[1] = fRec616[0];
			fRec615[2] = fRec615[1];
			fRec615[1] = fRec615[0];
			fRec614[2] = fRec614[1];
			fRec614[1] = fRec614[0];
			fRec613[2] = fRec613[1];
			fRec613[1] = fRec613[0];
			fRec612[2] = fRec612[1];
			fRec612[1] = fRec612[0];
			fRec611[1] = fRec611[0];
			fRec635[2] = fRec635[1];
			fRec635[1] = fRec635[0];
			fRec634[2] = fRec634[1];
			fRec634[1] = fRec634[0];
			fRec633[2] = fRec633[1];
			fRec633[1] = fRec633[0];
			fRec632[2] = fRec632[1];
			fRec632[1] = fRec632[0];
			fRec631[2] = fRec631[1];
			fRec631[1] = fRec631[0];
			fRec630[2] = fRec630[1];
			fRec630[1] = fRec630[0];
			fRec629[2] = fRec629[1];
			fRec629[1] = fRec629[0];
			fRec628[2] = fRec628[1];
			fRec628[1] = fRec628[0];
			fRec627[2] = fRec627[1];
			fRec627[1] = fRec627[0];
			fRec626[2] = fRec626[1];
			fRec626[1] = fRec626[0];
			fRec625[2] = fRec625[1];
			fRec625[1] = fRec625[0];
			fRec624[2] = fRec624[1];
			fRec624[1] = fRec624[0];
			fRec648[2] = fRec648[1];
			fRec648[1] = fRec648[0];
			fRec647[2] = fRec647[1];
			fRec647[1] = fRec647[0];
			fRec646[2] = fRec646[1];
			fRec646[1] = fRec646[0];
			fRec645[2] = fRec645[1];
			fRec645[1] = fRec645[0];
			fRec644[2] = fRec644[1];
			fRec644[1] = fRec644[0];
			fRec643[2] = fRec643[1];
			fRec643[1] = fRec643[0];
			fRec642[2] = fRec642[1];
			fRec642[1] = fRec642[0];
			fRec641[2] = fRec641[1];
			fRec641[1] = fRec641[0];
			fRec640[2] = fRec640[1];
			fRec640[1] = fRec640[0];
			fRec639[2] = fRec639[1];
			fRec639[1] = fRec639[0];
			fRec638[2] = fRec638[1];
			fRec638[1] = fRec638[0];
			fRec637[2] = fRec637[1];
			fRec637[1] = fRec637[0];
			fRec636[1] = fRec636[0];
			fRec660[2] = fRec660[1];
			fRec660[1] = fRec660[0];
			fRec659[2] = fRec659[1];
			fRec659[1] = fRec659[0];
			fRec658[2] = fRec658[1];
			fRec658[1] = fRec658[0];
			fRec657[2] = fRec657[1];
			fRec657[1] = fRec657[0];
			fRec656[2] = fRec656[1];
			fRec656[1] = fRec656[0];
			fRec655[2] = fRec655[1];
			fRec655[1] = fRec655[0];
			fRec654[2] = fRec654[1];
			fRec654[1] = fRec654[0];
			fRec653[2] = fRec653[1];
			fRec653[1] = fRec653[0];
			fRec652[2] = fRec652[1];
			fRec652[1] = fRec652[0];
			fRec651[2] = fRec651[1];
			fRec651[1] = fRec651[0];
			fRec650[2] = fRec650[1];
			fRec650[1] = fRec650[0];
			fRec649[2] = fRec649[1];
			fRec649[1] = fRec649[0];
			fRec673[2] = fRec673[1];
			fRec673[1] = fRec673[0];
			fRec672[2] = fRec672[1];
			fRec672[1] = fRec672[0];
			fRec671[2] = fRec671[1];
			fRec671[1] = fRec671[0];
			fRec670[2] = fRec670[1];
			fRec670[1] = fRec670[0];
			fRec669[2] = fRec669[1];
			fRec669[1] = fRec669[0];
			fRec668[2] = fRec668[1];
			fRec668[1] = fRec668[0];
			fRec667[2] = fRec667[1];
			fRec667[1] = fRec667[0];
			fRec666[2] = fRec666[1];
			fRec666[1] = fRec666[0];
			fRec665[2] = fRec665[1];
			fRec665[1] = fRec665[0];
			fRec664[2] = fRec664[1];
			fRec664[1] = fRec664[0];
			fRec663[2] = fRec663[1];
			fRec663[1] = fRec663[0];
			fRec662[2] = fRec662[1];
			fRec662[1] = fRec662[0];
			fRec661[1] = fRec661[0];
			fRec685[2] = fRec685[1];
			fRec685[1] = fRec685[0];
			fRec684[2] = fRec684[1];
			fRec684[1] = fRec684[0];
			fRec683[2] = fRec683[1];
			fRec683[1] = fRec683[0];
			fRec682[2] = fRec682[1];
			fRec682[1] = fRec682[0];
			fRec681[2] = fRec681[1];
			fRec681[1] = fRec681[0];
			fRec680[2] = fRec680[1];
			fRec680[1] = fRec680[0];
			fRec679[2] = fRec679[1];
			fRec679[1] = fRec679[0];
			fRec678[2] = fRec678[1];
			fRec678[1] = fRec678[0];
			fRec677[2] = fRec677[1];
			fRec677[1] = fRec677[0];
			fRec676[2] = fRec676[1];
			fRec676[1] = fRec676[0];
			fRec675[2] = fRec675[1];
			fRec675[1] = fRec675[0];
			fRec674[2] = fRec674[1];
			fRec674[1] = fRec674[0];
			fRec698[2] = fRec698[1];
			fRec698[1] = fRec698[0];
			fRec697[2] = fRec697[1];
			fRec697[1] = fRec697[0];
			fRec696[2] = fRec696[1];
			fRec696[1] = fRec696[0];
			fRec695[2] = fRec695[1];
			fRec695[1] = fRec695[0];
			fRec694[2] = fRec694[1];
			fRec694[1] = fRec694[0];
			fRec693[2] = fRec693[1];
			fRec693[1] = fRec693[0];
			fRec692[2] = fRec692[1];
			fRec692[1] = fRec692[0];
			fRec691[2] = fRec691[1];
			fRec691[1] = fRec691[0];
			fRec690[2] = fRec690[1];
			fRec690[1] = fRec690[0];
			fRec689[2] = fRec689[1];
			fRec689[1] = fRec689[0];
			fRec688[2] = fRec688[1];
			fRec688[1] = fRec688[0];
			fRec687[2] = fRec687[1];
			fRec687[1] = fRec687[0];
			fRec686[1] = fRec686[0];
			fRec710[2] = fRec710[1];
			fRec710[1] = fRec710[0];
			fRec709[2] = fRec709[1];
			fRec709[1] = fRec709[0];
			fRec708[2] = fRec708[1];
			fRec708[1] = fRec708[0];
			fRec707[2] = fRec707[1];
			fRec707[1] = fRec707[0];
			fRec706[2] = fRec706[1];
			fRec706[1] = fRec706[0];
			fRec705[2] = fRec705[1];
			fRec705[1] = fRec705[0];
			fRec704[2] = fRec704[1];
			fRec704[1] = fRec704[0];
			fRec703[2] = fRec703[1];
			fRec703[1] = fRec703[0];
			fRec702[2] = fRec702[1];
			fRec702[1] = fRec702[0];
			fRec701[2] = fRec701[1];
			fRec701[1] = fRec701[0];
			fRec700[2] = fRec700[1];
			fRec700[1] = fRec700[0];
			fRec699[2] = fRec699[1];
			fRec699[1] = fRec699[0];
			fRec723[2] = fRec723[1];
			fRec723[1] = fRec723[0];
			fRec722[2] = fRec722[1];
			fRec722[1] = fRec722[0];
			fRec721[2] = fRec721[1];
			fRec721[1] = fRec721[0];
			fRec720[2] = fRec720[1];
			fRec720[1] = fRec720[0];
			fRec719[2] = fRec719[1];
			fRec719[1] = fRec719[0];
			fRec718[2] = fRec718[1];
			fRec718[1] = fRec718[0];
			fRec717[2] = fRec717[1];
			fRec717[1] = fRec717[0];
			fRec716[2] = fRec716[1];
			fRec716[1] = fRec716[0];
			fRec715[2] = fRec715[1];
			fRec715[1] = fRec715[0];
			fRec714[2] = fRec714[1];
			fRec714[1] = fRec714[0];
			fRec713[2] = fRec713[1];
			fRec713[1] = fRec713[0];
			fRec712[2] = fRec712[1];
			fRec712[1] = fRec712[0];
			fRec711[1] = fRec711[0];
			fRec735[2] = fRec735[1];
			fRec735[1] = fRec735[0];
			fRec734[2] = fRec734[1];
			fRec734[1] = fRec734[0];
			fRec733[2] = fRec733[1];
			fRec733[1] = fRec733[0];
			fRec732[2] = fRec732[1];
			fRec732[1] = fRec732[0];
			fRec731[2] = fRec731[1];
			fRec731[1] = fRec731[0];
			fRec730[2] = fRec730[1];
			fRec730[1] = fRec730[0];
			fRec729[2] = fRec729[1];
			fRec729[1] = fRec729[0];
			fRec728[2] = fRec728[1];
			fRec728[1] = fRec728[0];
			fRec727[2] = fRec727[1];
			fRec727[1] = fRec727[0];
			fRec726[2] = fRec726[1];
			fRec726[1] = fRec726[0];
			fRec725[2] = fRec725[1];
			fRec725[1] = fRec725[0];
			fRec724[2] = fRec724[1];
			fRec724[1] = fRec724[0];
			fRec748[2] = fRec748[1];
			fRec748[1] = fRec748[0];
			fRec747[2] = fRec747[1];
			fRec747[1] = fRec747[0];
			fRec746[2] = fRec746[1];
			fRec746[1] = fRec746[0];
			fRec745[2] = fRec745[1];
			fRec745[1] = fRec745[0];
			fRec744[2] = fRec744[1];
			fRec744[1] = fRec744[0];
			fRec743[2] = fRec743[1];
			fRec743[1] = fRec743[0];
			fRec742[2] = fRec742[1];
			fRec742[1] = fRec742[0];
			fRec741[2] = fRec741[1];
			fRec741[1] = fRec741[0];
			fRec740[2] = fRec740[1];
			fRec740[1] = fRec740[0];
			fRec739[2] = fRec739[1];
			fRec739[1] = fRec739[0];
			fRec738[2] = fRec738[1];
			fRec738[1] = fRec738[0];
			fRec737[2] = fRec737[1];
			fRec737[1] = fRec737[0];
			fRec736[1] = fRec736[0];
			fRec760[2] = fRec760[1];
			fRec760[1] = fRec760[0];
			fRec759[2] = fRec759[1];
			fRec759[1] = fRec759[0];
			fRec758[2] = fRec758[1];
			fRec758[1] = fRec758[0];
			fRec757[2] = fRec757[1];
			fRec757[1] = fRec757[0];
			fRec756[2] = fRec756[1];
			fRec756[1] = fRec756[0];
			fRec755[2] = fRec755[1];
			fRec755[1] = fRec755[0];
			fRec754[2] = fRec754[1];
			fRec754[1] = fRec754[0];
			fRec753[2] = fRec753[1];
			fRec753[1] = fRec753[0];
			fRec752[2] = fRec752[1];
			fRec752[1] = fRec752[0];
			fRec751[2] = fRec751[1];
			fRec751[1] = fRec751[0];
			fRec750[2] = fRec750[1];
			fRec750[1] = fRec750[0];
			fRec749[2] = fRec749[1];
			fRec749[1] = fRec749[0];
			fRec773[2] = fRec773[1];
			fRec773[1] = fRec773[0];
			fRec772[2] = fRec772[1];
			fRec772[1] = fRec772[0];
			fRec771[2] = fRec771[1];
			fRec771[1] = fRec771[0];
			fRec770[2] = fRec770[1];
			fRec770[1] = fRec770[0];
			fRec769[2] = fRec769[1];
			fRec769[1] = fRec769[0];
			fRec768[2] = fRec768[1];
			fRec768[1] = fRec768[0];
			fRec767[2] = fRec767[1];
			fRec767[1] = fRec767[0];
			fRec766[2] = fRec766[1];
			fRec766[1] = fRec766[0];
			fRec765[2] = fRec765[1];
			fRec765[1] = fRec765[0];
			fRec764[2] = fRec764[1];
			fRec764[1] = fRec764[0];
			fRec763[2] = fRec763[1];
			fRec763[1] = fRec763[0];
			fRec762[2] = fRec762[1];
			fRec762[1] = fRec762[0];
			fRec761[1] = fRec761[0];
			fRec785[2] = fRec785[1];
			fRec785[1] = fRec785[0];
			fRec784[2] = fRec784[1];
			fRec784[1] = fRec784[0];
			fRec783[2] = fRec783[1];
			fRec783[1] = fRec783[0];
			fRec782[2] = fRec782[1];
			fRec782[1] = fRec782[0];
			fRec781[2] = fRec781[1];
			fRec781[1] = fRec781[0];
			fRec780[2] = fRec780[1];
			fRec780[1] = fRec780[0];
			fRec779[2] = fRec779[1];
			fRec779[1] = fRec779[0];
			fRec778[2] = fRec778[1];
			fRec778[1] = fRec778[0];
			fRec777[2] = fRec777[1];
			fRec777[1] = fRec777[0];
			fRec776[2] = fRec776[1];
			fRec776[1] = fRec776[0];
			fRec775[2] = fRec775[1];
			fRec775[1] = fRec775[0];
			fRec774[2] = fRec774[1];
			fRec774[1] = fRec774[0];
			fRec798[2] = fRec798[1];
			fRec798[1] = fRec798[0];
			fRec797[2] = fRec797[1];
			fRec797[1] = fRec797[0];
			fRec796[2] = fRec796[1];
			fRec796[1] = fRec796[0];
			fRec795[2] = fRec795[1];
			fRec795[1] = fRec795[0];
			fRec794[2] = fRec794[1];
			fRec794[1] = fRec794[0];
			fRec793[2] = fRec793[1];
			fRec793[1] = fRec793[0];
			fRec792[2] = fRec792[1];
			fRec792[1] = fRec792[0];
			fRec791[2] = fRec791[1];
			fRec791[1] = fRec791[0];
			fRec790[2] = fRec790[1];
			fRec790[1] = fRec790[0];
			fRec789[2] = fRec789[1];
			fRec789[1] = fRec789[0];
			fRec788[2] = fRec788[1];
			fRec788[1] = fRec788[0];
			fRec787[2] = fRec787[1];
			fRec787[1] = fRec787[0];
			fRec786[1] = fRec786[0];
			fRec810[2] = fRec810[1];
			fRec810[1] = fRec810[0];
			fRec809[2] = fRec809[1];
			fRec809[1] = fRec809[0];
			fRec808[2] = fRec808[1];
			fRec808[1] = fRec808[0];
			fRec807[2] = fRec807[1];
			fRec807[1] = fRec807[0];
			fRec806[2] = fRec806[1];
			fRec806[1] = fRec806[0];
			fRec805[2] = fRec805[1];
			fRec805[1] = fRec805[0];
			fRec804[2] = fRec804[1];
			fRec804[1] = fRec804[0];
			fRec803[2] = fRec803[1];
			fRec803[1] = fRec803[0];
			fRec802[2] = fRec802[1];
			fRec802[1] = fRec802[0];
			fRec801[2] = fRec801[1];
			fRec801[1] = fRec801[0];
			fRec800[2] = fRec800[1];
			fRec800[1] = fRec800[0];
			fRec799[2] = fRec799[1];
			fRec799[1] = fRec799[0];
			fRec823[2] = fRec823[1];
			fRec823[1] = fRec823[0];
			fRec822[2] = fRec822[1];
			fRec822[1] = fRec822[0];
			fRec821[2] = fRec821[1];
			fRec821[1] = fRec821[0];
			fRec820[2] = fRec820[1];
			fRec820[1] = fRec820[0];
			fRec819[2] = fRec819[1];
			fRec819[1] = fRec819[0];
			fRec818[2] = fRec818[1];
			fRec818[1] = fRec818[0];
			fRec817[2] = fRec817[1];
			fRec817[1] = fRec817[0];
			fRec816[2] = fRec816[1];
			fRec816[1] = fRec816[0];
			fRec815[2] = fRec815[1];
			fRec815[1] = fRec815[0];
			fRec814[2] = fRec814[1];
			fRec814[1] = fRec814[0];
			fRec813[2] = fRec813[1];
			fRec813[1] = fRec813[0];
			fRec812[2] = fRec812[1];
			fRec812[1] = fRec812[0];
			fRec811[1] = fRec811[0];
			fRec835[2] = fRec835[1];
			fRec835[1] = fRec835[0];
			fRec834[2] = fRec834[1];
			fRec834[1] = fRec834[0];
			fRec833[2] = fRec833[1];
			fRec833[1] = fRec833[0];
			fRec832[2] = fRec832[1];
			fRec832[1] = fRec832[0];
			fRec831[2] = fRec831[1];
			fRec831[1] = fRec831[0];
			fRec830[2] = fRec830[1];
			fRec830[1] = fRec830[0];
			fRec829[2] = fRec829[1];
			fRec829[1] = fRec829[0];
			fRec828[2] = fRec828[1];
			fRec828[1] = fRec828[0];
			fRec827[2] = fRec827[1];
			fRec827[1] = fRec827[0];
			fRec826[2] = fRec826[1];
			fRec826[1] = fRec826[0];
			fRec825[2] = fRec825[1];
			fRec825[1] = fRec825[0];
			fRec824[2] = fRec824[1];
			fRec824[1] = fRec824[0];
			fRec848[2] = fRec848[1];
			fRec848[1] = fRec848[0];
			fRec847[2] = fRec847[1];
			fRec847[1] = fRec847[0];
			fRec846[2] = fRec846[1];
			fRec846[1] = fRec846[0];
			fRec845[2] = fRec845[1];
			fRec845[1] = fRec845[0];
			fRec844[2] = fRec844[1];
			fRec844[1] = fRec844[0];
			fRec843[2] = fRec843[1];
			fRec843[1] = fRec843[0];
			fRec842[2] = fRec842[1];
			fRec842[1] = fRec842[0];
			fRec841[2] = fRec841[1];
			fRec841[1] = fRec841[0];
			fRec840[2] = fRec840[1];
			fRec840[1] = fRec840[0];
			fRec839[2] = fRec839[1];
			fRec839[1] = fRec839[0];
			fRec838[2] = fRec838[1];
			fRec838[1] = fRec838[0];
			fRec837[2] = fRec837[1];
			fRec837[1] = fRec837[0];
			fRec836[1] = fRec836[0];
			fRec860[2] = fRec860[1];
			fRec860[1] = fRec860[0];
			fRec859[2] = fRec859[1];
			fRec859[1] = fRec859[0];
			fRec858[2] = fRec858[1];
			fRec858[1] = fRec858[0];
			fRec857[2] = fRec857[1];
			fRec857[1] = fRec857[0];
			fRec856[2] = fRec856[1];
			fRec856[1] = fRec856[0];
			fRec855[2] = fRec855[1];
			fRec855[1] = fRec855[0];
			fRec854[2] = fRec854[1];
			fRec854[1] = fRec854[0];
			fRec853[2] = fRec853[1];
			fRec853[1] = fRec853[0];
			fRec852[2] = fRec852[1];
			fRec852[1] = fRec852[0];
			fRec851[2] = fRec851[1];
			fRec851[1] = fRec851[0];
			fRec850[2] = fRec850[1];
			fRec850[1] = fRec850[0];
			fRec849[2] = fRec849[1];
			fRec849[1] = fRec849[0];
			fRec873[2] = fRec873[1];
			fRec873[1] = fRec873[0];
			fRec872[2] = fRec872[1];
			fRec872[1] = fRec872[0];
			fRec871[2] = fRec871[1];
			fRec871[1] = fRec871[0];
			fRec870[2] = fRec870[1];
			fRec870[1] = fRec870[0];
			fRec869[2] = fRec869[1];
			fRec869[1] = fRec869[0];
			fRec868[2] = fRec868[1];
			fRec868[1] = fRec868[0];
			fRec867[2] = fRec867[1];
			fRec867[1] = fRec867[0];
			fRec866[2] = fRec866[1];
			fRec866[1] = fRec866[0];
			fRec865[2] = fRec865[1];
			fRec865[1] = fRec865[0];
			fRec864[2] = fRec864[1];
			fRec864[1] = fRec864[0];
			fRec863[2] = fRec863[1];
			fRec863[1] = fRec863[0];
			fRec862[2] = fRec862[1];
			fRec862[1] = fRec862[0];
			fRec861[1] = fRec861[0];
			fRec885[2] = fRec885[1];
			fRec885[1] = fRec885[0];
			fRec884[2] = fRec884[1];
			fRec884[1] = fRec884[0];
			fRec883[2] = fRec883[1];
			fRec883[1] = fRec883[0];
			fRec882[2] = fRec882[1];
			fRec882[1] = fRec882[0];
			fRec881[2] = fRec881[1];
			fRec881[1] = fRec881[0];
			fRec880[2] = fRec880[1];
			fRec880[1] = fRec880[0];
			fRec879[2] = fRec879[1];
			fRec879[1] = fRec879[0];
			fRec878[2] = fRec878[1];
			fRec878[1] = fRec878[0];
			fRec877[2] = fRec877[1];
			fRec877[1] = fRec877[0];
			fRec876[2] = fRec876[1];
			fRec876[1] = fRec876[0];
			fRec875[2] = fRec875[1];
			fRec875[1] = fRec875[0];
			fRec874[2] = fRec874[1];
			fRec874[1] = fRec874[0];
			fRec898[2] = fRec898[1];
			fRec898[1] = fRec898[0];
			fRec897[2] = fRec897[1];
			fRec897[1] = fRec897[0];
			fRec896[2] = fRec896[1];
			fRec896[1] = fRec896[0];
			fRec895[2] = fRec895[1];
			fRec895[1] = fRec895[0];
			fRec894[2] = fRec894[1];
			fRec894[1] = fRec894[0];
			fRec893[2] = fRec893[1];
			fRec893[1] = fRec893[0];
			fRec892[2] = fRec892[1];
			fRec892[1] = fRec892[0];
			fRec891[2] = fRec891[1];
			fRec891[1] = fRec891[0];
			fRec890[2] = fRec890[1];
			fRec890[1] = fRec890[0];
			fRec889[2] = fRec889[1];
			fRec889[1] = fRec889[0];
			fRec888[2] = fRec888[1];
			fRec888[1] = fRec888[0];
			fRec887[2] = fRec887[1];
			fRec887[1] = fRec887[0];
			fRec886[1] = fRec886[0];
			fRec910[2] = fRec910[1];
			fRec910[1] = fRec910[0];
			fRec909[2] = fRec909[1];
			fRec909[1] = fRec909[0];
			fRec908[2] = fRec908[1];
			fRec908[1] = fRec908[0];
			fRec907[2] = fRec907[1];
			fRec907[1] = fRec907[0];
			fRec906[2] = fRec906[1];
			fRec906[1] = fRec906[0];
			fRec905[2] = fRec905[1];
			fRec905[1] = fRec905[0];
			fRec904[2] = fRec904[1];
			fRec904[1] = fRec904[0];
			fRec903[2] = fRec903[1];
			fRec903[1] = fRec903[0];
			fRec902[2] = fRec902[1];
			fRec902[1] = fRec902[0];
			fRec901[2] = fRec901[1];
			fRec901[1] = fRec901[0];
			fRec900[2] = fRec900[1];
			fRec900[1] = fRec900[0];
			fRec899[2] = fRec899[1];
			fRec899[1] = fRec899[0];
			fRec0[1] = fRec0[0];
		}
	}

};
#endif

class faust_piece_nono_quando_vocoder_tilde : public FaustExternal<piece_nono_quando_vocoder> {
public:
    faust_piece_nono_quando_vocoder_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(piece_nono_quando_vocoder))
    {
    }
};

#endif
