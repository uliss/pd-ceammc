/* ------------------------------------------------------------
name: "nono.quando.fb2"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn piece_nono_quando_fb2 -scn piece_nono_quando_fb2_dsp -es 1 -mcd 16 -double -ftz 0
------------------------------------------------------------ */

#ifndef  __piece_nono_quando_fb2_H__
#define  __piece_nono_quando_fb2_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN piece_nono_quando_fb2_dsp.h ********************************
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

class FAUST_API piece_nono_quando_fb2_dsp {

    public:

        piece_nono_quando_fb2_dsp() {}
        virtual ~piece_nono_quando_fb2_dsp() {}

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
        virtual piece_nono_quando_fb2_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public piece_nono_quando_fb2_dsp {

    protected:

        piece_nono_quando_fb2_dsp* fDSP;

    public:

        decorator_dsp(piece_nono_quando_fb2_dsp* piece_nono_quando_fb2_dsp = nullptr):fDSP(piece_nono_quando_fb2_dsp) {}
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
    
        virtual piece_nono_quando_fb2_dsp* createDSPInstance() = 0;
    
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

/************************** END piece_nono_quando_fb2_dsp.h **************************/
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
 The base class of Meta handler to be used in piece_nono_quando_fb2_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct piece_nono_quando_fb2 : public piece_nono_quando_fb2_dsp {
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
#define FAUSTCLASS piece_nono_quando_fb2
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

static double piece_nono_quando_fb2_faustpower2_f(double value) {
	return value * value;
}
static double piece_nono_quando_fb2_faustpower4_f(double value) {
	return value * value * value * value;
}
static double piece_nono_quando_fb2_faustpower3_f(double value) {
	return value * value * value;
}

class piece_nono_quando_fb2 : public piece_nono_quando_fb2_dsp {
	
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
	double fConst57;
	double fConst59;
	double fConst62;
	double fConst66;
	double fConst67;
	double fConst68;
	double fConst69;
	double fRec5[3];
	double fConst70;
	double fConst71;
	double fConst74;
	double fConst75;
	double fConst76;
	double fConst77;
	double fRec4[5];
	double fConst79;
	double fConst80;
	double fConst81;
	double fRec3[5];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn piece_nono_quando_fb2 -scn piece_nono_quando_fb2_dsp -es 1 -mcd 16 -double -ftz 0");
		m->declare("filename", "piece_nono_quando_fb2.dsp");
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
		m->declare("name", "nono.quando.fb2");
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
		fConst2 = piece_nono_quando_fb2_faustpower2_f(fConst1);
		double fConst3 = piece_nono_quando_fb2_faustpower4_f(fConst1);
		double fConst4 = piece_nono_quando_fb2_faustpower2_f(fConst0);
		double fConst5 = std::tan(942.4777960769379 / fConst0);
		double fConst6 = std::sqrt(4.0 * fConst4 * std::tan(279.6017461694916 / fConst0) * fConst5);
		double fConst7 = fConst3 * piece_nono_quando_fb2_faustpower4_f(fConst6);
		double fConst8 = fConst0 * fConst5;
		double fConst9 = piece_nono_quando_fb2_faustpower2_f(fConst6);
		double fConst10 = 2.0 * fConst8 - 0.5 * (fConst9 / fConst8);
		double fConst11 = piece_nono_quando_fb2_faustpower2_f(fConst10);
		fConst12 = 4.0 * fConst11;
		double fConst13 = fConst2 * (fConst12 + 8.0 * fConst9);
		double fConst14 = 4.944271909999157 / fConst0;
		double fConst15 = piece_nono_quando_fb2_faustpower3_f(fConst1);
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
		double fConst46 = std::tan(3179.2917654328708 / fConst0);
		double fConst47 = std::sqrt(4.0 * fConst4 * std::tan(2120.5750411731105 / fConst0) * fConst46);
		double fConst48 = fConst3 * piece_nono_quando_fb2_faustpower4_f(fConst47);
		double fConst49 = fConst0 * fConst46;
		double fConst50 = piece_nono_quando_fb2_faustpower2_f(fConst47);
		double fConst51 = 2.0 * fConst49 - 0.5 * (fConst50 / fConst49);
		double fConst52 = piece_nono_quando_fb2_faustpower2_f(fConst51);
		fConst53 = 4.0 * fConst52;
		double fConst54 = fConst48 + fConst2 * (fConst53 + 8.0 * fConst50);
		double fConst55 = fConst15 * fConst50;
		double fConst56 = fConst14 + 1.2360679774997894 * fConst55;
		fConst57 = 1.0 / (fConst54 + fConst51 * fConst56 + 16.0);
		double fConst58 = 8.0 * fConst52;
		fConst59 = 0.0 - fConst58;
		double fConst60 = fConst21 + 3.2360679774997894 * fConst55;
		double fConst61 = fConst51 * fConst60 + fConst54 + 16.0;
		fConst62 = fConst2 / fConst61;
		double fConst63 = fConst2 * fConst50;
		double fConst64 = 2.0 * (fConst51 / fConst0);
		double fConst65 = fConst63 + fConst64 + 4.0;
		fConst66 = 2.0 * (fConst51 / (fConst0 * fConst65));
		fConst67 = 1.0 / fConst65;
		fConst68 = 2.0 * fConst63 + -8.0;
		fConst69 = fConst63 + (4.0 - fConst64);
		fConst70 = 0.0 - fConst66;
		fConst71 = 1.0 / fConst61;
		double fConst72 = 4.0 * fConst48;
		double fConst73 = 6.472135954999579 * fConst55;
		fConst74 = fConst72 + fConst51 * (fConst73 - fConst36) + -64.0;
		fConst75 = 6.0 * fConst48 + fConst2 * (0.0 - (fConst58 + 16.0 * fConst50)) + 96.0;
		fConst76 = fConst72 + fConst51 * (fConst36 - fConst73) + -64.0;
		fConst77 = fConst54 + fConst51 * (0.0 - fConst60) + 16.0;
		double fConst78 = 2.4721359549995787 * fConst55;
		fConst79 = fConst72 + fConst51 * (fConst78 - fConst42) + -64.0;
		fConst80 = fConst72 + fConst51 * (fConst42 - fConst78) + -64.0;
		fConst81 = fConst54 + fConst51 * (0.0 - fConst56) + 16.0;
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
	
	virtual piece_nono_quando_fb2* clone() {
		return new piece_nono_quando_fb2();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("nono.quando.fb2");
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
			fRec5[0] = fTemp0 - fConst67 * (fConst68 * fRec5[1] + fConst69 * fRec5[2]);
			fRec4[0] = fConst66 * fRec5[0] + fConst70 * fRec5[2] - fConst71 * (fConst74 * fRec4[1] + fConst75 * fRec4[2] + fConst76 * fRec4[3] + fConst77 * fRec4[4]);
			fRec3[0] = fConst62 * (fConst59 * fRec4[2] + fConst53 * fRec4[0] + fConst53 * fRec4[4]) - fConst57 * (fConst79 * fRec3[1] + fConst75 * fRec3[2] + fConst80 * fRec3[3] + fConst81 * fRec3[4]);
			output0[i0] = FAUSTFLOAT(fConst2 * (fConst18 * (fConst20 * fRec0[2] + fConst12 * fRec0[0] + fConst12 * fRec0[4]) + fConst57 * (fConst59 * fRec3[2] + fConst53 * fRec3[0] + fConst53 * fRec3[4])));
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
		}
	}

};
#endif

class faust_piece_nono_quando_fb2_tilde : public FaustExternal<piece_nono_quando_fb2> {
public:
    faust_piece_nono_quando_fb2_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(piece_nono_quando_fb2))
    {
    }
};

#endif
