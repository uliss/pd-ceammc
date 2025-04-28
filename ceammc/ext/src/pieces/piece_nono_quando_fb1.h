/* ------------------------------------------------------------
name: "nono.quando.fb1"
Code generated with Faust 2.74.5. (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn piece_nono_quando_fb1 -scn piece_nono_quando_fb1_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0
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
        virtual piece_nono_quando_fb1_dsp* createDSPInstance() = 0;
    
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

/************************** END piece_nono_quando_fb1_dsp.h **************************/
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
 The base class of Meta handler to be used in piece_nono_quando_fb1_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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

static double piece_nono_quando_fb1_faustpower2_f(double value) {
	return value * value;
}
static double piece_nono_quando_fb1_faustpower4_f(double value) {
	return value * value * value * value;
}
static double piece_nono_quando_fb1_faustpower3_f(double value) {
	return value * value * value;
}

class piece_nono_quando_fb1 : public piece_nono_quando_fb1_dsp {
	
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
	double fConst25;
	double fConst26;
	double fConst27;
	double fConst28;
	double fConst29;
	double fRec2[3];
	double fConst30;
	double fConst31;
	double fConst32;
	double fConst33;
	double fConst34;
	double fConst35;
	double fConst36;
	double fConst37;
	double fRec1[5];
	double fConst38;
	double fConst39;
	double fConst40;
	double fConst41;
	double fConst42;
	double fConst43;
	double fRec0[5];
	double fConst44;
	double fConst45;
	double fConst46;
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
	double fConst63;
	double fConst64;
	double fConst65;
	double fRec5[3];
	double fConst66;
	double fConst67;
	double fConst68;
	double fConst69;
	double fConst70;
	double fConst71;
	double fConst72;
	double fRec4[5];
	double fConst73;
	double fConst74;
	double fConst75;
	double fConst76;
	double fConst77;
	double fRec3[5];
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
	double fConst91;
	double fConst92;
	double fConst93;
	double fConst94;
	double fConst95;
	double fConst96;
	double fConst97;
	double fConst98;
	double fConst99;
	double fConst100;
	double fRec8[3];
	double fConst101;
	double fConst102;
	double fConst103;
	double fConst104;
	double fConst105;
	double fConst106;
	double fConst107;
	double fRec7[5];
	double fConst108;
	double fConst109;
	double fConst110;
	double fConst111;
	double fConst112;
	double fRec6[5];
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
	double fConst131;
	double fConst132;
	double fConst133;
	double fConst134;
	double fRec11[3];
	double fConst135;
	double fConst136;
	double fConst137;
	double fConst138;
	double fConst139;
	double fConst140;
	double fConst141;
	double fRec10[5];
	double fConst142;
	double fConst143;
	double fConst144;
	double fConst145;
	double fConst146;
	double fRec9[5];
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
	double fRec14[3];
	double fConst169;
	double fConst170;
	double fConst171;
	double fConst172;
	double fConst173;
	double fConst174;
	double fConst175;
	double fRec13[5];
	double fConst176;
	double fConst177;
	double fConst178;
	double fConst179;
	double fConst180;
	double fRec12[5];
	
 public:
	piece_nono_quando_fb1() {
	}
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -ct 1 -cn piece_nono_quando_fb1 -scn piece_nono_quando_fb1_dsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0");
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
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "nono.quando.fb1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
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
		fConst1 = 1.0 / fConst0;
		fConst2 = piece_nono_quando_fb1_faustpower2_f(fConst1);
		fConst3 = std::tan(1196.946801017711 / fConst0);
		fConst4 = fConst0 * fConst3;
		fConst5 = piece_nono_quando_fb1_faustpower2_f(fConst0);
		fConst6 = std::sqrt(4.0 * fConst5 * std::tan(282.7433388230814 / fConst0) * fConst3);
		fConst7 = piece_nono_quando_fb1_faustpower2_f(fConst6);
		fConst8 = 2.0 * fConst4 - 0.5 * (fConst7 / fConst4);
		fConst9 = piece_nono_quando_fb1_faustpower2_f(fConst8);
		fConst10 = piece_nono_quando_fb1_faustpower4_f(fConst1);
		fConst11 = fConst10 * piece_nono_quando_fb1_faustpower4_f(fConst6);
		fConst12 = fConst11 + fConst2 * (4.0 * fConst9 + 8.0 * fConst7);
		fConst13 = 4.944271909999157 / fConst0;
		fConst14 = piece_nono_quando_fb1_faustpower3_f(fConst1);
		fConst15 = fConst14 * fConst7;
		fConst16 = fConst8 * (fConst13 + 1.2360679774997894 * fConst15);
		fConst17 = fConst12 + fConst16 + 16.0;
		fConst18 = fConst9 / fConst17;
		fConst19 = 12.944271909999157 / fConst0;
		fConst20 = fConst8 * (fConst19 + 3.2360679774997894 * fConst15);
		fConst21 = fConst20 + fConst12 + 16.0;
		fConst22 = fConst2 * fConst9 / fConst21;
		fConst23 = fConst2 * fConst7;
		fConst24 = 2.0 * (fConst8 / fConst0);
		fConst25 = fConst23 + fConst24 + 4.0;
		fConst26 = 2.0 * (fConst8 / (fConst0 * fConst25));
		fConst27 = 1.0 / fConst25;
		fConst28 = 2.0 * fConst23 + -8.0;
		fConst29 = fConst23 + (4.0 - fConst24);
		fConst30 = 1.0 / fConst21;
		fConst31 = 4.0 * fConst11;
		fConst32 = 6.472135954999579 * fConst15;
		fConst33 = 25.888543819998315 / fConst0;
		fConst34 = fConst31 + fConst8 * (fConst32 - fConst33) + -64.0;
		fConst35 = 6.0 * fConst11 + (96.0 - fConst2 * (8.0 * fConst9 + 16.0 * fConst7));
		fConst36 = fConst31 + fConst8 * (fConst33 - fConst32) + -64.0;
		fConst37 = fConst12 + (16.0 - fConst20);
		fConst38 = 1.0 / fConst17;
		fConst39 = 2.4721359549995787 * fConst15;
		fConst40 = 9.888543819998315 / fConst0;
		fConst41 = fConst31 + fConst8 * (fConst39 - fConst40) + -64.0;
		fConst42 = fConst31 + fConst8 * (fConst40 - fConst39) + -64.0;
		fConst43 = fConst12 + (16.0 - fConst16);
		fConst44 = std::tan(8523.140869189108 / fConst0);
		fConst45 = fConst0 * fConst44;
		fConst46 = std::sqrt(4.0 * fConst5 * std::tan(3194.9997287008196 / fConst0) * fConst44);
		fConst47 = piece_nono_quando_fb1_faustpower2_f(fConst46);
		fConst48 = 2.0 * fConst45 - 0.5 * (fConst47 / fConst45);
		fConst49 = piece_nono_quando_fb1_faustpower2_f(fConst48);
		fConst50 = fConst10 * piece_nono_quando_fb1_faustpower4_f(fConst46);
		fConst51 = fConst50 + fConst2 * (4.0 * fConst49 + 8.0 * fConst47);
		fConst52 = fConst14 * fConst47;
		fConst53 = fConst48 * (fConst13 + 1.2360679774997894 * fConst52);
		fConst54 = fConst51 + fConst53 + 16.0;
		fConst55 = fConst49 / fConst54;
		fConst56 = fConst48 * (fConst19 + 3.2360679774997894 * fConst52);
		fConst57 = fConst56 + fConst51 + 16.0;
		fConst58 = fConst2 * fConst49 / fConst57;
		fConst59 = fConst2 * fConst47;
		fConst60 = 2.0 * (fConst48 / fConst0);
		fConst61 = fConst59 + fConst60 + 4.0;
		fConst62 = 2.0 * (fConst48 / (fConst0 * fConst61));
		fConst63 = 1.0 / fConst61;
		fConst64 = 2.0 * fConst59 + -8.0;
		fConst65 = fConst59 + (4.0 - fConst60);
		fConst66 = 1.0 / fConst57;
		fConst67 = 4.0 * fConst50;
		fConst68 = 6.472135954999579 * fConst52;
		fConst69 = fConst67 + fConst48 * (fConst68 - fConst33) + -64.0;
		fConst70 = 6.0 * fConst50 + (96.0 - fConst2 * (8.0 * fConst49 + 16.0 * fConst47));
		fConst71 = fConst67 + fConst48 * (fConst33 - fConst68) + -64.0;
		fConst72 = fConst51 + (16.0 - fConst56);
		fConst73 = 1.0 / fConst54;
		fConst74 = 2.4721359549995787 * fConst52;
		fConst75 = fConst67 + fConst48 * (fConst74 - fConst40) + -64.0;
		fConst76 = fConst67 + fConst48 * (fConst40 - fConst74) + -64.0;
		fConst77 = fConst51 + (16.0 - fConst53);
		fConst78 = std::tan(10741.105282623503 / fConst0);
		fConst79 = fConst0 * fConst78;
		fConst80 = std::sqrt(4.0 * fConst5 * std::tan(10125.353122519904 / fConst0) * fConst78);
		fConst81 = piece_nono_quando_fb1_faustpower2_f(fConst80);
		fConst82 = 2.0 * fConst79 - 0.5 * (fConst81 / fConst79);
		fConst83 = piece_nono_quando_fb1_faustpower2_f(fConst82);
		fConst84 = fConst2 * (4.0 * fConst83 + 8.0 * fConst81);
		fConst85 = fConst10 * piece_nono_quando_fb1_faustpower4_f(fConst80);
		fConst86 = fConst14 * fConst81;
		fConst87 = fConst82 * (fConst13 + 1.2360679774997894 * fConst86);
		fConst88 = fConst84 + fConst85 + fConst87 + 16.0;
		fConst89 = fConst83 / fConst88;
		fConst90 = fConst82 * (fConst19 + 3.2360679774997894 * fConst86);
		fConst91 = fConst85 + fConst84;
		fConst92 = fConst90 + fConst91 + 16.0;
		fConst93 = fConst2 * fConst83 / fConst92;
		fConst94 = fConst2 * fConst81;
		fConst95 = 2.0 * (fConst82 / fConst0);
		fConst96 = fConst94 + fConst95 + 4.0;
		fConst97 = 2.0 * (fConst82 / (fConst0 * fConst96));
		fConst98 = 1.0 / fConst96;
		fConst99 = 2.0 * fConst94 + -8.0;
		fConst100 = fConst94 + (4.0 - fConst95);
		fConst101 = 1.0 / fConst92;
		fConst102 = 4.0 * fConst85;
		fConst103 = 6.472135954999579 * fConst86;
		fConst104 = fConst102 + fConst82 * (fConst103 - fConst33) + -64.0;
		fConst105 = 6.0 * fConst85 + (96.0 - fConst2 * (8.0 * fConst83 + 16.0 * fConst81));
		fConst106 = fConst102 + fConst82 * (fConst33 - fConst103) + -64.0;
		fConst107 = fConst91 + (16.0 - fConst90);
		fConst108 = 1.0 / fConst88;
		fConst109 = 2.4721359549995787 * fConst86;
		fConst110 = fConst102 + fConst82 * (fConst109 - fConst40) + -64.0;
		fConst111 = fConst102 + fConst82 * (fConst40 - fConst109) + -64.0;
		fConst112 = fConst91 + (16.0 - fConst87);
		fConst113 = std::tan(15189.60048010665 / fConst0);
		fConst114 = fConst0 * fConst113;
		fConst115 = std::sqrt(4.0 * fConst5 * std::tan(14344.512056290996 / fConst0) * fConst113);
		fConst116 = piece_nono_quando_fb1_faustpower2_f(fConst115);
		fConst117 = 2.0 * fConst114 - 0.5 * (fConst116 / fConst114);
		fConst118 = piece_nono_quando_fb1_faustpower2_f(fConst117);
		fConst119 = fConst10 * piece_nono_quando_fb1_faustpower4_f(fConst115);
		fConst120 = fConst119 + fConst2 * (4.0 * fConst118 + 8.0 * fConst116);
		fConst121 = fConst14 * fConst116;
		fConst122 = fConst117 * (fConst13 + 1.2360679774997894 * fConst121);
		fConst123 = fConst120 + fConst122 + 16.0;
		fConst124 = fConst118 / fConst123;
		fConst125 = fConst117 * (fConst19 + 3.2360679774997894 * fConst121);
		fConst126 = fConst125 + fConst120 + 16.0;
		fConst127 = fConst2 * fConst118 / fConst126;
		fConst128 = fConst2 * fConst116;
		fConst129 = 2.0 * (fConst117 / fConst0);
		fConst130 = fConst128 + fConst129 + 4.0;
		fConst131 = 2.0 * (fConst117 / (fConst0 * fConst130));
		fConst132 = 1.0 / fConst130;
		fConst133 = 2.0 * fConst128 + -8.0;
		fConst134 = fConst128 + (4.0 - fConst129);
		fConst135 = 1.0 / fConst126;
		fConst136 = 4.0 * fConst119;
		fConst137 = 6.472135954999579 * fConst121;
		fConst138 = fConst136 + fConst117 * (fConst137 - fConst33) + -64.0;
		fConst139 = 6.0 * fConst119 + (96.0 - fConst2 * (8.0 * fConst118 + 16.0 * fConst116));
		fConst140 = fConst136 + fConst117 * (fConst33 - fConst137) + -64.0;
		fConst141 = fConst120 + (16.0 - fConst125);
		fConst142 = 1.0 / fConst123;
		fConst143 = 2.4721359549995787 * fConst121;
		fConst144 = fConst136 + fConst117 * (fConst143 - fConst40) + -64.0;
		fConst145 = fConst136 + fConst117 * (fConst40 - fConst143) + -64.0;
		fConst146 = fConst120 + (16.0 - fConst122);
		fConst147 = std::tan(21482.210565247005 / fConst0);
		fConst148 = fConst0 * fConst147;
		fConst149 = std::sqrt(4.0 * fConst5 * std::tan(20288.405356882882 / fConst0) * fConst147);
		fConst150 = piece_nono_quando_fb1_faustpower2_f(fConst149);
		fConst151 = 2.0 * fConst148 - 0.5 * (fConst150 / fConst148);
		fConst152 = piece_nono_quando_fb1_faustpower2_f(fConst151);
		fConst153 = fConst10 * piece_nono_quando_fb1_faustpower4_f(fConst149);
		fConst154 = fConst153 + fConst2 * (4.0 * fConst152 + 8.0 * fConst150);
		fConst155 = fConst14 * fConst150;
		fConst156 = fConst151 * (fConst13 + 1.2360679774997894 * fConst155);
		fConst157 = fConst154 + fConst156 + 16.0;
		fConst158 = fConst152 / fConst157;
		fConst159 = fConst151 * (fConst19 + 3.2360679774997894 * fConst155);
		fConst160 = fConst159 + fConst154 + 16.0;
		fConst161 = fConst2 * fConst152 / fConst160;
		fConst162 = fConst2 * fConst150;
		fConst163 = 2.0 * (fConst151 / fConst0);
		fConst164 = fConst162 + fConst163 + 4.0;
		fConst165 = 2.0 * (fConst151 / (fConst0 * fConst164));
		fConst166 = 1.0 / fConst164;
		fConst167 = 2.0 * fConst162 + -8.0;
		fConst168 = fConst162 + (4.0 - fConst163);
		fConst169 = 1.0 / fConst160;
		fConst170 = 4.0 * fConst153;
		fConst171 = 6.472135954999579 * fConst155;
		fConst172 = fConst170 + fConst151 * (fConst171 - fConst33) + -64.0;
		fConst173 = 6.0 * fConst153 + (96.0 - fConst2 * (8.0 * fConst152 + 16.0 * fConst150));
		fConst174 = fConst170 + fConst151 * (fConst33 - fConst171) + -64.0;
		fConst175 = fConst154 + (16.0 - fConst159);
		fConst176 = 1.0 / fConst157;
		fConst177 = 2.4721359549995787 * fConst155;
		fConst178 = fConst170 + fConst151 * (fConst177 - fConst40) + -64.0;
		fConst179 = fConst170 + fConst151 * (fConst40 - fConst177) + -64.0;
		fConst180 = fConst154 + (16.0 - fConst156);
	}
	
	virtual void instanceResetUserInterface() {
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 3; l0 = l0 + 1) {
			fRec2[l0] = 0.0;
		}
		for (int l1 = 0; l1 < 5; l1 = l1 + 1) {
			fRec1[l1] = 0.0;
		}
		for (int l2 = 0; l2 < 5; l2 = l2 + 1) {
			fRec0[l2] = 0.0;
		}
		for (int l3 = 0; l3 < 3; l3 = l3 + 1) {
			fRec5[l3] = 0.0;
		}
		for (int l4 = 0; l4 < 5; l4 = l4 + 1) {
			fRec4[l4] = 0.0;
		}
		for (int l5 = 0; l5 < 5; l5 = l5 + 1) {
			fRec3[l5] = 0.0;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec8[l6] = 0.0;
		}
		for (int l7 = 0; l7 < 5; l7 = l7 + 1) {
			fRec7[l7] = 0.0;
		}
		for (int l8 = 0; l8 < 5; l8 = l8 + 1) {
			fRec6[l8] = 0.0;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec11[l9] = 0.0;
		}
		for (int l10 = 0; l10 < 5; l10 = l10 + 1) {
			fRec10[l10] = 0.0;
		}
		for (int l11 = 0; l11 < 5; l11 = l11 + 1) {
			fRec9[l11] = 0.0;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec14[l12] = 0.0;
		}
		for (int l13 = 0; l13 < 5; l13 = l13 + 1) {
			fRec13[l13] = 0.0;
		}
		for (int l14 = 0; l14 < 5; l14 = l14 + 1) {
			fRec12[l14] = 0.0;
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
			double fTemp0 = double(input0[i0]);
			fRec2[0] = fTemp0 - fConst27 * (fConst28 * fRec2[1] + fConst29 * fRec2[2]);
			fRec1[0] = fConst26 * (fRec2[0] - fRec2[2]) - fConst30 * (fConst34 * fRec1[1] + fConst35 * fRec1[2] + fConst36 * fRec1[3] + fConst37 * fRec1[4]);
			fRec0[0] = fConst22 * (4.0 * fRec1[0] - 8.0 * fRec1[2] + 4.0 * fRec1[4]) - fConst38 * (fConst41 * fRec0[1] + fConst35 * fRec0[2] + fConst42 * fRec0[3] + fConst43 * fRec0[4]);
			fRec5[0] = fTemp0 - fConst63 * (fConst64 * fRec5[1] + fConst65 * fRec5[2]);
			fRec4[0] = fConst62 * (fRec5[0] - fRec5[2]) - fConst66 * (fConst69 * fRec4[1] + fConst70 * fRec4[2] + fConst71 * fRec4[3] + fConst72 * fRec4[4]);
			fRec3[0] = fConst58 * (4.0 * fRec4[0] - 8.0 * fRec4[2] + 4.0 * fRec4[4]) - fConst73 * (fConst75 * fRec3[1] + fConst70 * fRec3[2] + fConst76 * fRec3[3] + fConst77 * fRec3[4]);
			fRec8[0] = fTemp0 - fConst98 * (fConst99 * fRec8[1] + fConst100 * fRec8[2]);
			fRec7[0] = fConst97 * (fRec8[0] - fRec8[2]) - fConst101 * (fConst104 * fRec7[1] + fConst105 * fRec7[2] + fConst106 * fRec7[3] + fConst107 * fRec7[4]);
			fRec6[0] = fConst93 * (4.0 * fRec7[0] - 8.0 * fRec7[2] + 4.0 * fRec7[4]) - fConst108 * (fConst110 * fRec6[1] + fConst105 * fRec6[2] + fConst111 * fRec6[3] + fConst112 * fRec6[4]);
			fRec11[0] = fTemp0 - fConst132 * (fConst133 * fRec11[1] + fConst134 * fRec11[2]);
			fRec10[0] = fConst131 * (fRec11[0] - fRec11[2]) - fConst135 * (fConst138 * fRec10[1] + fConst139 * fRec10[2] + fConst140 * fRec10[3] + fConst141 * fRec10[4]);
			fRec9[0] = fConst127 * (4.0 * fRec10[0] - 8.0 * fRec10[2] + 4.0 * fRec10[4]) - fConst142 * (fConst144 * fRec9[1] + fConst139 * fRec9[2] + fConst145 * fRec9[3] + fConst146 * fRec9[4]);
			fRec14[0] = fTemp0 - fConst166 * (fConst167 * fRec14[1] + fConst168 * fRec14[2]);
			fRec13[0] = fConst165 * (fRec14[0] - fRec14[2]) - fConst169 * (fConst172 * fRec13[1] + fConst173 * fRec13[2] + fConst174 * fRec13[3] + fConst175 * fRec13[4]);
			fRec12[0] = fConst161 * (4.0 * fRec13[0] - 8.0 * fRec13[2] + 4.0 * fRec13[4]) - fConst176 * (fConst178 * fRec12[1] + fConst173 * fRec12[2] + fConst179 * fRec12[3] + fConst180 * fRec12[4]);
			output0[i0] = FAUSTFLOAT(fConst2 * (fConst18 * (4.0 * (fRec0[0] + fRec0[4]) - 8.0 * fRec0[2]) + fConst55 * (4.0 * (fRec3[0] + fRec3[4]) - 8.0 * fRec3[2]) + fConst89 * (4.0 * (fRec6[0] + fRec6[4]) - 8.0 * fRec6[2]) + fConst124 * (4.0 * (fRec9[0] + fRec9[4]) - 8.0 * fRec9[2]) + fConst158 * (4.0 * (fRec12[0] + fRec12[4]) - 8.0 * fRec12[2])));
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
#endif

class faust_piece_nono_quando_fb1_tilde : public FaustExternal<piece_nono_quando_fb1> {
public:
    faust_piece_nono_quando_fb1_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(piece_nono_quando_fb1))
    {
    }
};

#endif
