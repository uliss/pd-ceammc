/* ------------------------------------------------------------
name: "nono.quando.fb1"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn piece_nono_quando_fb1 -scn piece_nono_quando_fb1_dsp -es 1 -mcd 16 -double -ftz 0
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
	double fConst2;
	double fConst12;
	double fConst18;
	double fConst20;
	double fConst24;
	double fConst28;
	double fConst29;
	double fConst30;
	double fConst31;
	double fRec2[3];
	double fConst32;
	double fConst33;
	double fConst37;
	double fConst38;
	double fConst39;
	double fConst40;
	double fRec1[5];
	double fConst43;
	double fConst44;
	double fConst45;
	double fRec0[5];
	double fConst53;
	double fConst58;
	double fConst60;
	double fConst63;
	double fConst67;
	double fConst68;
	double fConst69;
	double fConst70;
	double fRec5[3];
	double fConst71;
	double fConst72;
	double fConst73;
	double fConst76;
	double fConst77;
	double fConst78;
	double fRec4[5];
	double fConst80;
	double fConst81;
	double fConst82;
	double fRec3[5];
	double fConst90;
	double fConst95;
	double fConst97;
	double fConst100;
	double fConst104;
	double fConst105;
	double fConst106;
	double fConst107;
	double fRec8[3];
	double fConst108;
	double fConst109;
	double fConst112;
	double fConst113;
	double fConst114;
	double fConst115;
	double fRec7[5];
	double fConst117;
	double fConst118;
	double fConst119;
	double fRec6[5];
	double fConst126;
	double fConst131;
	double fConst133;
	double fConst137;
	double fConst141;
	double fConst142;
	double fConst143;
	double fConst144;
	double fRec11[3];
	double fConst145;
	double fConst146;
	double fConst149;
	double fConst150;
	double fConst151;
	double fConst152;
	double fRec10[5];
	double fConst154;
	double fConst155;
	double fConst156;
	double fRec9[5];
	double fConst164;
	double fConst169;
	double fConst171;
	double fConst174;
	double fConst178;
	double fConst179;
	double fConst180;
	double fConst181;
	double fRec14[3];
	double fConst182;
	double fConst183;
	double fConst186;
	double fConst187;
	double fConst188;
	double fConst189;
	double fRec13[5];
	double fConst191;
	double fConst192;
	double fConst193;
	double fRec12[5];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn piece_nono_quando_fb1 -scn piece_nono_quando_fb1_dsp -es 1 -mcd 16 -double -ftz 0");
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
		double fConst0 = std::min<double>(1.92e+05, std::max<double>(1.0, double(fSampleRate)));
		double fConst1 = 1.0 / fConst0;
		fConst2 = piece_nono_quando_fb1_faustpower2_f(fConst1);
		double fConst3 = piece_nono_quando_fb1_faustpower4_f(fConst1);
		double fConst4 = piece_nono_quando_fb1_faustpower2_f(fConst0);
		double fConst5 = std::tan(8523.140869189108 / fConst0);
		double fConst6 = std::sqrt(4.0 * fConst4 * std::tan(3194.9997287008196 / fConst0) * fConst5);
		double fConst7 = fConst3 * piece_nono_quando_fb1_faustpower4_f(fConst6);
		double fConst8 = fConst0 * fConst5;
		double fConst9 = piece_nono_quando_fb1_faustpower2_f(fConst6);
		double fConst10 = 2.0 * fConst8 - 0.5 * (fConst9 / fConst8);
		double fConst11 = piece_nono_quando_fb1_faustpower2_f(fConst10);
		fConst12 = 4.0 * fConst11;
		double fConst13 = fConst2 * (fConst12 + 8.0 * fConst9);
		double fConst14 = 4.944271909999157 / fConst0;
		double fConst15 = piece_nono_quando_fb1_faustpower3_f(fConst1);
		double fConst16 = fConst15 * fConst9;
		double fConst17 = fConst14 + 1.2360679774997894 * fConst16;
		fConst18 = 1.0 / (fConst7 + fConst13 + fConst10 * fConst17 + 16.0);
		double fConst19 = 8.0 * fConst11;
		fConst20 = 0.0 - fConst19;
		double fConst21 = 12.944271909999157 / fConst0;
		double fConst22 = fConst21 + 3.2360679774997894 * fConst16;
		double fConst23 = fConst7 + fConst13 + fConst10 * fConst22 + 16.0;
		fConst24 = fConst2 / fConst23;
		double fConst25 = fConst2 * fConst9;
		double fConst26 = 2.0 * (fConst10 / fConst0);
		double fConst27 = fConst25 + fConst26 + 4.0;
		fConst28 = 2.0 * (fConst10 / (fConst0 * fConst27));
		fConst29 = 1.0 / fConst27;
		fConst30 = 2.0 * fConst25 + -8.0;
		fConst31 = fConst25 + (4.0 - fConst26);
		fConst32 = 0.0 - fConst28;
		fConst33 = 1.0 / fConst23;
		double fConst34 = 4.0 * fConst7;
		double fConst35 = 6.472135954999579 * fConst16;
		double fConst36 = 25.888543819998315 / fConst0;
		fConst37 = fConst34 + fConst10 * (fConst35 - fConst36) + -64.0;
		fConst38 = 6.0 * fConst7 + fConst2 * (0.0 - (fConst19 + 16.0 * fConst9)) + 96.0;
		fConst39 = fConst34 + fConst10 * (fConst36 - fConst35) + -64.0;
		fConst40 = fConst7 + fConst13 + fConst10 * (0.0 - fConst22) + 16.0;
		double fConst41 = 2.4721359549995787 * fConst16;
		double fConst42 = 9.888543819998315 / fConst0;
		fConst43 = fConst34 + fConst10 * (fConst41 - fConst42) + -64.0;
		fConst44 = fConst34 + fConst10 * (fConst42 - fConst41) + -64.0;
		fConst45 = fConst7 + fConst13 + fConst10 * (0.0 - fConst17) + 16.0;
		double fConst46 = std::tan(1196.946801017711 / fConst0);
		double fConst47 = std::sqrt(4.0 * fConst4 * std::tan(282.7433388230814 / fConst0) * fConst46);
		double fConst48 = fConst3 * piece_nono_quando_fb1_faustpower4_f(fConst47);
		double fConst49 = fConst0 * fConst46;
		double fConst50 = piece_nono_quando_fb1_faustpower2_f(fConst47);
		double fConst51 = 2.0 * fConst49 - 0.5 * (fConst50 / fConst49);
		double fConst52 = piece_nono_quando_fb1_faustpower2_f(fConst51);
		fConst53 = 4.0 * fConst52;
		double fConst54 = fConst2 * (fConst53 + 8.0 * fConst50);
		double fConst55 = fConst48 + fConst54;
		double fConst56 = fConst15 * fConst50;
		double fConst57 = 1.2360679774997894 * fConst56 + fConst14;
		fConst58 = 1.0 / (fConst55 + fConst51 * fConst57 + 16.0);
		double fConst59 = 8.0 * fConst52;
		fConst60 = 0.0 - fConst59;
		double fConst61 = 3.2360679774997894 * fConst56 + fConst21;
		double fConst62 = fConst51 * fConst61 + fConst55 + 16.0;
		fConst63 = fConst2 / fConst62;
		double fConst64 = fConst2 * fConst50;
		double fConst65 = 2.0 * (fConst51 / fConst0);
		double fConst66 = fConst64 + fConst65 + 4.0;
		fConst67 = 2.0 * (fConst51 / (fConst0 * fConst66));
		fConst68 = 1.0 / fConst66;
		fConst69 = 2.0 * fConst64 + -8.0;
		fConst70 = fConst64 + (4.0 - fConst65);
		fConst71 = 0.0 - fConst67;
		fConst72 = 1.0 / fConst62;
		fConst73 = 6.0 * fConst48 + fConst2 * (0.0 - (fConst59 + 16.0 * fConst50)) + 96.0;
		double fConst74 = 4.0 * fConst48;
		double fConst75 = 6.472135954999579 * fConst56;
		fConst76 = fConst74 + fConst51 * (fConst75 - fConst36) + -64.0;
		fConst77 = fConst74 + fConst51 * (fConst36 - fConst75) + -64.0;
		fConst78 = fConst54 + fConst48 + fConst51 * (0.0 - fConst61) + 16.0;
		double fConst79 = 2.4721359549995787 * fConst56;
		fConst80 = fConst74 + fConst51 * (fConst79 - fConst42) + -64.0;
		fConst81 = fConst74 + fConst51 * (fConst42 - fConst79) + -64.0;
		fConst82 = fConst55 + fConst51 * (0.0 - fConst57) + 16.0;
		double fConst83 = std::tan(10741.105282623503 / fConst0);
		double fConst84 = std::sqrt(4.0 * fConst4 * std::tan(10125.353122519904 / fConst0) * fConst83);
		double fConst85 = fConst3 * piece_nono_quando_fb1_faustpower4_f(fConst84);
		double fConst86 = fConst0 * fConst83;
		double fConst87 = piece_nono_quando_fb1_faustpower2_f(fConst84);
		double fConst88 = 2.0 * fConst86 - 0.5 * (fConst87 / fConst86);
		double fConst89 = piece_nono_quando_fb1_faustpower2_f(fConst88);
		fConst90 = 4.0 * fConst89;
		double fConst91 = fConst2 * (fConst90 + 8.0 * fConst87);
		double fConst92 = fConst85 + fConst91;
		double fConst93 = fConst15 * fConst87;
		double fConst94 = fConst14 + 1.2360679774997894 * fConst93;
		fConst95 = 1.0 / (fConst92 + fConst88 * fConst94 + 16.0);
		double fConst96 = 8.0 * fConst89;
		fConst97 = 0.0 - fConst96;
		double fConst98 = fConst21 + 3.2360679774997894 * fConst93;
		double fConst99 = fConst85 + fConst91 + fConst88 * fConst98 + 16.0;
		fConst100 = fConst2 / fConst99;
		double fConst101 = fConst2 * fConst87;
		double fConst102 = 2.0 * (fConst88 / fConst0);
		double fConst103 = fConst101 + fConst102 + 4.0;
		fConst104 = 2.0 * (fConst88 / (fConst0 * fConst103));
		fConst105 = 1.0 / fConst103;
		fConst106 = 2.0 * fConst101 + -8.0;
		fConst107 = fConst101 + (4.0 - fConst102);
		fConst108 = 0.0 - fConst104;
		fConst109 = 1.0 / fConst99;
		double fConst110 = 4.0 * fConst85;
		double fConst111 = 6.472135954999579 * fConst93;
		fConst112 = fConst110 + fConst88 * (fConst111 - fConst36) + -64.0;
		fConst113 = 6.0 * fConst85 + fConst2 * (0.0 - (fConst96 + 16.0 * fConst87)) + 96.0;
		fConst114 = fConst110 + fConst88 * (fConst36 - fConst111) + -64.0;
		fConst115 = fConst85 + fConst91 + fConst88 * (0.0 - fConst98) + 16.0;
		double fConst116 = 2.4721359549995787 * fConst93;
		fConst117 = fConst110 + fConst88 * (fConst116 - fConst42) + -64.0;
		fConst118 = fConst110 + fConst88 * (fConst42 - fConst116) + -64.0;
		fConst119 = fConst92 + fConst88 * (0.0 - fConst94) + 16.0;
		double fConst120 = std::tan(15189.60048010665 / fConst0);
		double fConst121 = fConst0 * fConst120;
		double fConst122 = std::sqrt(4.0 * fConst4 * std::tan(14344.512056290996 / fConst0) * fConst120);
		double fConst123 = piece_nono_quando_fb1_faustpower2_f(fConst122);
		double fConst124 = 2.0 * fConst121 - 0.5 * (fConst123 / fConst121);
		double fConst125 = piece_nono_quando_fb1_faustpower2_f(fConst124);
		fConst126 = 4.0 * fConst125;
		double fConst127 = fConst2 * (fConst126 + 8.0 * fConst123);
		double fConst128 = fConst3 * piece_nono_quando_fb1_faustpower4_f(fConst122);
		double fConst129 = fConst15 * fConst123;
		double fConst130 = fConst14 + 1.2360679774997894 * fConst129;
		fConst131 = 1.0 / (fConst127 + fConst128 + fConst124 * fConst130 + 16.0);
		double fConst132 = 8.0 * fConst125;
		fConst133 = 0.0 - fConst132;
		double fConst134 = fConst21 + 3.2360679774997894 * fConst129;
		double fConst135 = fConst128 + fConst127;
		double fConst136 = fConst124 * fConst134 + fConst135 + 16.0;
		fConst137 = fConst2 / fConst136;
		double fConst138 = fConst2 * fConst123;
		double fConst139 = 2.0 * (fConst124 / fConst0);
		double fConst140 = fConst138 + fConst139 + 4.0;
		fConst141 = 2.0 * (fConst124 / (fConst0 * fConst140));
		fConst142 = 1.0 / fConst140;
		fConst143 = 2.0 * fConst138 + -8.0;
		fConst144 = fConst138 + (4.0 - fConst139);
		fConst145 = 0.0 - fConst141;
		fConst146 = 1.0 / fConst136;
		double fConst147 = 4.0 * fConst128;
		double fConst148 = 6.472135954999579 * fConst129;
		fConst149 = fConst147 + fConst124 * (fConst148 - fConst36) + -64.0;
		fConst150 = 6.0 * fConst128 + fConst2 * (0.0 - (fConst132 + 16.0 * fConst123)) + 96.0;
		fConst151 = fConst147 + fConst124 * (fConst36 - fConst148) + -64.0;
		fConst152 = fConst135 + fConst124 * (0.0 - fConst134) + 16.0;
		double fConst153 = 2.4721359549995787 * fConst129;
		fConst154 = fConst147 + fConst124 * (fConst153 - fConst42) + -64.0;
		fConst155 = fConst147 + fConst124 * (fConst42 - fConst153) + -64.0;
		fConst156 = fConst127 + fConst128 + fConst124 * (0.0 - fConst130) + 16.0;
		double fConst157 = std::tan(21482.210565247005 / fConst0);
		double fConst158 = std::sqrt(4.0 * fConst4 * std::tan(20288.405356882882 / fConst0) * fConst157);
		double fConst159 = fConst3 * piece_nono_quando_fb1_faustpower4_f(fConst158);
		double fConst160 = fConst0 * fConst157;
		double fConst161 = piece_nono_quando_fb1_faustpower2_f(fConst158);
		double fConst162 = 2.0 * fConst160 - 0.5 * (fConst161 / fConst160);
		double fConst163 = piece_nono_quando_fb1_faustpower2_f(fConst162);
		fConst164 = 4.0 * fConst163;
		double fConst165 = fConst2 * (fConst164 + 8.0 * fConst161);
		double fConst166 = fConst159 + fConst165;
		double fConst167 = fConst15 * fConst161;
		double fConst168 = fConst14 + 1.2360679774997894 * fConst167;
		fConst169 = 1.0 / (fConst166 + fConst162 * fConst168 + 16.0);
		double fConst170 = 8.0 * fConst163;
		fConst171 = 0.0 - fConst170;
		double fConst172 = fConst21 + 3.2360679774997894 * fConst167;
		double fConst173 = fConst159 + fConst165 + fConst162 * fConst172 + 16.0;
		fConst174 = fConst2 / fConst173;
		double fConst175 = fConst2 * fConst161;
		double fConst176 = 2.0 * (fConst162 / fConst0);
		double fConst177 = fConst175 + fConst176 + 4.0;
		fConst178 = 2.0 * (fConst162 / (fConst0 * fConst177));
		fConst179 = 1.0 / fConst177;
		fConst180 = 2.0 * fConst175 + -8.0;
		fConst181 = fConst175 + (4.0 - fConst176);
		fConst182 = 0.0 - fConst178;
		fConst183 = 1.0 / fConst173;
		double fConst184 = 4.0 * fConst159;
		double fConst185 = 6.472135954999579 * fConst167;
		fConst186 = fConst184 + fConst162 * (fConst185 - fConst36) + -64.0;
		fConst187 = 6.0 * fConst159 + fConst2 * (0.0 - (fConst170 + 16.0 * fConst161)) + 96.0;
		fConst188 = fConst184 + fConst162 * (fConst36 - fConst185) + -64.0;
		fConst189 = fConst159 + fConst165 + fConst162 * (0.0 - fConst172) + 16.0;
		double fConst190 = 2.4721359549995787 * fConst167;
		fConst191 = fConst184 + fConst162 * (fConst190 - fConst42) + -64.0;
		fConst192 = fConst184 + fConst162 * (fConst42 - fConst190) + -64.0;
		fConst193 = fConst166 + fConst162 * (0.0 - fConst168) + 16.0;
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
			fRec2[0] = fTemp0 - fConst29 * (fConst30 * fRec2[1] + fConst31 * fRec2[2]);
			fRec1[0] = fConst28 * fRec2[0] + fConst32 * fRec2[2] - fConst33 * (fConst37 * fRec1[1] + fConst38 * fRec1[2] + fConst39 * fRec1[3] + fConst40 * fRec1[4]);
			fRec0[0] = fConst24 * (fConst20 * fRec1[2] + fConst12 * fRec1[0] + fConst12 * fRec1[4]) - fConst18 * (fConst43 * fRec0[1] + fConst38 * fRec0[2] + fConst44 * fRec0[3] + fConst45 * fRec0[4]);
			fRec5[0] = fTemp0 - fConst68 * (fConst69 * fRec5[1] + fConst70 * fRec5[2]);
			fRec4[0] = fConst67 * fRec5[0] + fConst71 * fRec5[2] - fConst72 * (fConst73 * fRec4[2] + fConst76 * fRec4[1] + fConst77 * fRec4[3] + fConst78 * fRec4[4]);
			fRec3[0] = fConst63 * (fConst60 * fRec4[2] + fConst53 * fRec4[0] + fConst53 * fRec4[4]) - fConst58 * (fConst80 * fRec3[1] + fConst73 * fRec3[2] + fConst81 * fRec3[3] + fConst82 * fRec3[4]);
			fRec8[0] = fTemp0 - fConst105 * (fConst106 * fRec8[1] + fConst107 * fRec8[2]);
			fRec7[0] = fConst104 * fRec8[0] + fConst108 * fRec8[2] - fConst109 * (fConst112 * fRec7[1] + fConst113 * fRec7[2] + fConst114 * fRec7[3] + fConst115 * fRec7[4]);
			fRec6[0] = fConst100 * (fConst97 * fRec7[2] + fConst90 * fRec7[0] + fConst90 * fRec7[4]) - fConst95 * (fConst117 * fRec6[1] + fConst113 * fRec6[2] + fConst118 * fRec6[3] + fConst119 * fRec6[4]);
			fRec11[0] = fTemp0 - fConst142 * (fConst143 * fRec11[1] + fConst144 * fRec11[2]);
			fRec10[0] = fConst141 * fRec11[0] + fConst145 * fRec11[2] - fConst146 * (fConst149 * fRec10[1] + fConst150 * fRec10[2] + fConst151 * fRec10[3] + fConst152 * fRec10[4]);
			fRec9[0] = fConst137 * (fConst133 * fRec10[2] + fConst126 * fRec10[0] + fConst126 * fRec10[4]) - fConst131 * (fConst154 * fRec9[1] + fConst150 * fRec9[2] + fConst155 * fRec9[3] + fConst156 * fRec9[4]);
			fRec14[0] = fTemp0 - fConst179 * (fConst180 * fRec14[1] + fConst181 * fRec14[2]);
			fRec13[0] = fConst178 * fRec14[0] + fConst182 * fRec14[2] - fConst183 * (fConst186 * fRec13[1] + fConst187 * fRec13[2] + fConst188 * fRec13[3] + fConst189 * fRec13[4]);
			fRec12[0] = fConst174 * (fConst171 * fRec13[2] + fConst164 * fRec13[0] + fConst164 * fRec13[4]) - fConst169 * (fConst191 * fRec12[1] + fConst187 * fRec12[2] + fConst192 * fRec12[3] + fConst193 * fRec12[4]);
			output0[i0] = FAUSTFLOAT(fConst2 * (fConst18 * (fConst20 * fRec0[2] + fConst12 * fRec0[0] + fConst12 * fRec0[4]) + fConst58 * (fConst60 * fRec3[2] + fConst53 * fRec3[0] + fConst53 * fRec3[4]) + fConst95 * (fConst97 * fRec6[2] + fConst90 * fRec6[0] + fConst90 * fRec6[4]) + fConst131 * (fConst133 * fRec9[2] + fConst126 * fRec9[0] + fConst126 * fRec9[4]) + fConst169 * (fConst171 * fRec12[2] + fConst164 * fRec12[0] + fConst164 * fRec12[4])));
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
