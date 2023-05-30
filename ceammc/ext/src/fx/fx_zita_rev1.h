/* ------------------------------------------------------------
name: "fx.zita_rev1"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn fx_zita_rev1 -scn fx_zita_rev1_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_zita_rev1_H__
#define  __fx_zita_rev1_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_zita_rev1_dsp.h ********************************
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

class FAUST_API fx_zita_rev1_dsp {

    public:

        fx_zita_rev1_dsp() {}
        virtual ~fx_zita_rev1_dsp() {}

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
        virtual fx_zita_rev1_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_zita_rev1_dsp {

    protected:

        fx_zita_rev1_dsp* fDSP;

    public:

        decorator_dsp(fx_zita_rev1_dsp* fx_zita_rev1_dsp = nullptr):fDSP(fx_zita_rev1_dsp) {}
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
    
        virtual fx_zita_rev1_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_zita_rev1_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_zita_rev1_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_zita_rev1 : public fx_zita_rev1_dsp {
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
#define FAUSTCLASS fx_zita_rev1
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

static float fx_zita_rev1_faustpower2_f(float value) {
	return value * value;
}

class fx_zita_rev1 : public fx_zita_rev1_dsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	float fRec0[2];
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec1[2];
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider1;
	float fConst3;
	float fRec12[2];
	int IOTA0;
	float fVec0[16384];
	float fConst4;
	FAUSTFLOAT fVslider0;
	float fRec13[2];
	float fConst6;
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fHslider2;
	float fConst7;
	FAUSTFLOAT fVslider2;
	float fConst8;
	FAUSTFLOAT fVslider3;
	float fRec15[2];
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fHslider3;
	float fRec14[2];
	float fVec1[32768];
	int iConst10;
	float fVec2[8192];
	int iConst11;
	float fRec10[2];
	float fConst13;
	float fRec19[2];
	float fRec18[2];
	float fVec3[32768];
	int iConst15;
	float fVec4[4096];
	int iConst16;
	float fRec16[2];
	float fConst18;
	float fRec23[2];
	float fRec22[2];
	float fVec5[32768];
	int iConst20;
	float fVec6[4096];
	int iConst21;
	float fRec20[2];
	float fConst23;
	float fRec27[2];
	float fRec26[2];
	float fVec7[32768];
	int iConst25;
	float fVec8[8192];
	int iConst26;
	float fRec24[2];
	float fConst28;
	float fRec31[2];
	float fRec30[2];
	float fVec9[32768];
	int iConst30;
	float fVec10[16384];
	float fVec11[8192];
	int iConst31;
	float fRec28[2];
	float fConst33;
	float fRec35[2];
	float fRec34[2];
	float fVec12[65536];
	int iConst35;
	float fVec13[8192];
	int iConst36;
	float fRec32[2];
	float fConst38;
	float fRec39[2];
	float fRec38[2];
	float fVec14[65536];
	int iConst40;
	float fVec15[4096];
	int iConst41;
	float fRec36[2];
	float fConst43;
	float fRec43[2];
	float fRec42[2];
	float fVec16[65536];
	int iConst45;
	float fVec17[8192];
	int iConst46;
	float fRec40[2];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	float fRec9[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.4");
		m->declare("ceammc_ui.lib/name", "CEAMMC faust default UI elements");
		m->declare("ceammc_ui.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/faust_arch_ceammc.cpp -lang cpp -i -cn fx_zita_rev1 -scn fx_zita_rev1_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "fx_zita_rev1.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
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
		m->declare("filters.lib/version", "0.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "fx.zita_rev1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
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
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / fConst0;
		fConst4 = 0.001f * fConst0;
		float fConst5 = std::floor(0.174713f * fConst0 + 0.5f);
		fConst6 = (0.0f - 6.9077554f * fConst5) / fConst0;
		fConst7 = 6.2831855f / fConst0;
		fConst8 = 3.1415927f / fConst0;
		float fConst9 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst10 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst5 - fConst9)));
		iConst11 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst9 + -1.0f)));
		float fConst12 = std::floor(0.153129f * fConst0 + 0.5f);
		fConst13 = (0.0f - 6.9077554f * fConst12) / fConst0;
		float fConst14 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst15 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst12 - fConst14)));
		iConst16 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst14 + -1.0f)));
		float fConst17 = std::floor(0.125f * fConst0 + 0.5f);
		fConst18 = (0.0f - 6.9077554f * fConst17) / fConst0;
		float fConst19 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst20 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst17 - fConst19)));
		iConst21 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst19 + -1.0f)));
		float fConst22 = std::floor(0.127837f * fConst0 + 0.5f);
		fConst23 = (0.0f - 6.9077554f * fConst22) / fConst0;
		float fConst24 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst25 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst22 - fConst24)));
		iConst26 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst24 + -1.0f)));
		float fConst27 = std::floor(0.192303f * fConst0 + 0.5f);
		fConst28 = (0.0f - 6.9077554f * fConst27) / fConst0;
		float fConst29 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst30 = int(std::min<float>(16384.0f, std::max<float>(0.0f, fConst27 - fConst29)));
		iConst31 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst29 + -1.0f)));
		float fConst32 = std::floor(0.210389f * fConst0 + 0.5f);
		fConst33 = (0.0f - 6.9077554f * fConst32) / fConst0;
		float fConst34 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst35 = int(std::min<float>(32768.0f, std::max<float>(0.0f, fConst32 - fConst34)));
		iConst36 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst34 + -1.0f)));
		float fConst37 = std::floor(0.219991f * fConst0 + 0.5f);
		fConst38 = (0.0f - 6.9077554f * fConst37) / fConst0;
		float fConst39 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst40 = int(std::min<float>(32768.0f, std::max<float>(0.0f, fConst37 - fConst39)));
		iConst41 = int(std::min<float>(2048.0f, std::max<float>(0.0f, fConst39 + -1.0f)));
		float fConst42 = std::floor(0.256891f * fConst0 + 0.5f);
		fConst43 = (0.0f - 6.9077554f * fConst42) / fConst0;
		float fConst44 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst45 = int(std::min<float>(32768.0f, std::max<float>(0.0f, fConst42 - fConst44)));
		iConst46 = int(std::min<float>(4096.0f, std::max<float>(0.0f, fConst44 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(1.0f);
		fCheckbox1 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(1e+02f);
		fVslider0 = FAUSTFLOAT(4e+01f);
		fVslider1 = FAUSTFLOAT(2.0f);
		fHslider2 = FAUSTFLOAT(4e+01f);
		fVslider2 = FAUSTFLOAT(6e+03f);
		fVslider3 = FAUSTFLOAT(2e+02f);
		fVslider4 = FAUSTFLOAT(3.0f);
		fHslider3 = FAUSTFLOAT(4e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec12[l2] = 0.0f;
		}
		IOTA0 = 0;
		for (int l3 = 0; l3 < 16384; l3 = l3 + 1) {
			fVec0[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec13[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec15[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec14[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 32768; l7 = l7 + 1) {
			fVec1[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 8192; l8 = l8 + 1) {
			fVec2[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec10[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec19[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec18[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 32768; l12 = l12 + 1) {
			fVec3[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 4096; l13 = l13 + 1) {
			fVec4[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec16[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec23[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec22[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 32768; l17 = l17 + 1) {
			fVec5[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 4096; l18 = l18 + 1) {
			fVec6[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec20[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec27[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec26[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 32768; l22 = l22 + 1) {
			fVec7[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 8192; l23 = l23 + 1) {
			fVec8[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec24[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec31[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec30[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 32768; l27 = l27 + 1) {
			fVec9[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 16384; l28 = l28 + 1) {
			fVec10[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 8192; l29 = l29 + 1) {
			fVec11[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec28[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec35[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec34[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 65536; l33 = l33 + 1) {
			fVec12[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 8192; l34 = l34 + 1) {
			fVec13[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec32[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec39[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec38[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 65536; l38 = l38 + 1) {
			fVec14[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 4096; l39 = l39 + 1) {
			fVec15[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec36[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec43[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec42[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 65536; l43 = l43 + 1) {
			fVec16[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 8192; l44 = l44 + 1) {
			fVec17[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec40[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 3; l46 = l46 + 1) {
			fRec2[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec3[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fRec4[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = l49 + 1) {
			fRec5[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = l50 + 1) {
			fRec6[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec7[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 3; l52 = l52 + 1) {
			fRec8[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fRec9[l53] = 0.0f;
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
	
	virtual fx_zita_rev1* clone() {
		return new fx_zita_rev1();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.zita_rev1");
		ui_interface->addCheckButton("bypass", &fCheckbox0);
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "Hz");
		ui_interface->addVerticalSlider("damp_hf", &fVslider2, FAUSTFLOAT(6e+03f), FAUSTFLOAT(1.5e+03f), FAUSTFLOAT(4.704e+04f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fVslider4, "scale", "log");
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low", &fVslider4, FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(6e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fVslider1, "scale", "log");
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid", &fVslider1, FAUSTFLOAT(2.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(6e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->declare(&fHslider3, "unit", "sec");
		ui_interface->addHorizontalSlider("fr_ldecay", &fHslider3, FAUSTFLOAT(4e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(6e+01f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->declare(&fHslider2, "unit", "sec");
		ui_interface->addHorizontalSlider("fr_mdecay", &fHslider2, FAUSTFLOAT(4e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(6e+01f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "ms");
		ui_interface->addHorizontalSlider("fr_time", &fHslider1, FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fCheckbox1, "type", "bool");
		ui_interface->addCheckButton("freeze", &fCheckbox1);
		ui_interface->declare(&fVslider3, "scale", "log");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "unit", "Hz");
		ui_interface->addVerticalSlider("freq_low", &fVslider3, FAUSTFLOAT(2e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "unit", "ms");
		ui_interface->addVerticalSlider("predelay", &fVslider0, FAUSTFLOAT(4e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(float(fCheckbox0) >= 1.0f);
		float fSlow1 = fConst1 * float(fHslider0);
		int iSlow2 = int(float(fCheckbox1));
		float fSlow3 = 0.001f * float(fHslider1);
		int iSlow4 = std::fabs(fSlow3) < 1.1920929e-07f;
		float fSlow5 = ((iSlow4) ? 0.0f : std::exp(0.0f - fConst3 / ((iSlow4) ? 1.0f : fSlow3)));
		float fSlow6 = float(1 - iSlow2) * (1.0f - fSlow5);
		float fSlow7 = fConst1 * float(fVslider0);
		float fSlow8 = ((iSlow2) ? float(fHslider2) : float(fVslider1));
		float fSlow9 = std::exp(fConst6 / fSlow8);
		float fSlow10 = std::cos(fConst7 * float(fVslider2));
		float fSlow11 = fx_zita_rev1_faustpower2_f(fSlow9);
		float fSlow12 = 1.0f - fSlow10 * fSlow11;
		float fSlow13 = 1.0f - fSlow11;
		float fSlow14 = std::sqrt(std::max<float>(0.0f, fx_zita_rev1_faustpower2_f(fSlow12) / fx_zita_rev1_faustpower2_f(fSlow13) + -1.0f));
		float fSlow15 = fSlow12 / fSlow13;
		float fSlow16 = fSlow9 * (fSlow14 + (1.0f - fSlow15));
		float fSlow17 = 1.0f / std::tan(fConst8 * float(fVslider3));
		float fSlow18 = 1.0f / (fSlow17 + 1.0f);
		float fSlow19 = 1.0f - fSlow17;
		float fSlow20 = ((iSlow2) ? float(fHslider3) : float(fVslider4));
		float fSlow21 = std::exp(fConst6 / fSlow20) / fSlow9 + -1.0f;
		float fSlow22 = fSlow15 - fSlow14;
		float fSlow23 = std::exp(fConst13 / fSlow8);
		float fSlow24 = fx_zita_rev1_faustpower2_f(fSlow23);
		float fSlow25 = 1.0f - fSlow10 * fSlow24;
		float fSlow26 = 1.0f - fSlow24;
		float fSlow27 = std::sqrt(std::max<float>(0.0f, fx_zita_rev1_faustpower2_f(fSlow25) / fx_zita_rev1_faustpower2_f(fSlow26) + -1.0f));
		float fSlow28 = fSlow25 / fSlow26;
		float fSlow29 = fSlow23 * (fSlow27 + (1.0f - fSlow28));
		float fSlow30 = std::exp(fConst13 / fSlow20) / fSlow23 + -1.0f;
		float fSlow31 = fSlow28 - fSlow27;
		float fSlow32 = std::exp(fConst18 / fSlow8);
		float fSlow33 = fx_zita_rev1_faustpower2_f(fSlow32);
		float fSlow34 = 1.0f - fSlow10 * fSlow33;
		float fSlow35 = 1.0f - fSlow33;
		float fSlow36 = std::sqrt(std::max<float>(0.0f, fx_zita_rev1_faustpower2_f(fSlow34) / fx_zita_rev1_faustpower2_f(fSlow35) + -1.0f));
		float fSlow37 = fSlow34 / fSlow35;
		float fSlow38 = fSlow32 * (fSlow36 + (1.0f - fSlow37));
		float fSlow39 = std::exp(fConst18 / fSlow20) / fSlow32 + -1.0f;
		float fSlow40 = fSlow37 - fSlow36;
		float fSlow41 = std::exp(fConst23 / fSlow8);
		float fSlow42 = fx_zita_rev1_faustpower2_f(fSlow41);
		float fSlow43 = 1.0f - fSlow10 * fSlow42;
		float fSlow44 = 1.0f - fSlow42;
		float fSlow45 = std::sqrt(std::max<float>(0.0f, fx_zita_rev1_faustpower2_f(fSlow43) / fx_zita_rev1_faustpower2_f(fSlow44) + -1.0f));
		float fSlow46 = fSlow43 / fSlow44;
		float fSlow47 = fSlow41 * (fSlow45 + (1.0f - fSlow46));
		float fSlow48 = std::exp(fConst23 / fSlow20) / fSlow41 + -1.0f;
		float fSlow49 = fSlow46 - fSlow45;
		float fSlow50 = std::exp(fConst28 / fSlow8);
		float fSlow51 = fx_zita_rev1_faustpower2_f(fSlow50);
		float fSlow52 = 1.0f - fSlow10 * fSlow51;
		float fSlow53 = 1.0f - fSlow51;
		float fSlow54 = std::sqrt(std::max<float>(0.0f, fx_zita_rev1_faustpower2_f(fSlow52) / fx_zita_rev1_faustpower2_f(fSlow53) + -1.0f));
		float fSlow55 = fSlow52 / fSlow53;
		float fSlow56 = fSlow50 * (fSlow54 + (1.0f - fSlow55));
		float fSlow57 = std::exp(fConst28 / fSlow20) / fSlow50 + -1.0f;
		float fSlow58 = fSlow55 - fSlow54;
		float fSlow59 = std::exp(fConst33 / fSlow8);
		float fSlow60 = fx_zita_rev1_faustpower2_f(fSlow59);
		float fSlow61 = 1.0f - fSlow10 * fSlow60;
		float fSlow62 = 1.0f - fSlow60;
		float fSlow63 = std::sqrt(std::max<float>(0.0f, fx_zita_rev1_faustpower2_f(fSlow61) / fx_zita_rev1_faustpower2_f(fSlow62) + -1.0f));
		float fSlow64 = fSlow61 / fSlow62;
		float fSlow65 = fSlow59 * (fSlow63 + (1.0f - fSlow64));
		float fSlow66 = std::exp(fConst33 / fSlow20) / fSlow59 + -1.0f;
		float fSlow67 = fSlow64 - fSlow63;
		float fSlow68 = std::exp(fConst38 / fSlow8);
		float fSlow69 = fx_zita_rev1_faustpower2_f(fSlow68);
		float fSlow70 = 1.0f - fSlow10 * fSlow69;
		float fSlow71 = 1.0f - fSlow69;
		float fSlow72 = std::sqrt(std::max<float>(0.0f, fx_zita_rev1_faustpower2_f(fSlow70) / fx_zita_rev1_faustpower2_f(fSlow71) + -1.0f));
		float fSlow73 = fSlow70 / fSlow71;
		float fSlow74 = fSlow68 * (fSlow72 + (1.0f - fSlow73));
		float fSlow75 = std::exp(fConst38 / fSlow20) / fSlow68 + -1.0f;
		float fSlow76 = fSlow73 - fSlow72;
		float fSlow77 = std::exp(fConst43 / fSlow8);
		float fSlow78 = fx_zita_rev1_faustpower2_f(fSlow77);
		float fSlow79 = 1.0f - fSlow10 * fSlow78;
		float fSlow80 = 1.0f - fSlow78;
		float fSlow81 = std::sqrt(std::max<float>(0.0f, fx_zita_rev1_faustpower2_f(fSlow79) / fx_zita_rev1_faustpower2_f(fSlow80) + -1.0f));
		float fSlow82 = fSlow79 / fSlow80;
		float fSlow83 = fSlow77 * (fSlow81 + (1.0f - fSlow82));
		float fSlow84 = std::exp(fConst43 / fSlow20) / fSlow77 + -1.0f;
		float fSlow85 = fSlow82 - fSlow81;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = fRec0[1] + 0.0078125f;
			float fTemp1 = fRec0[1] + -0.0078125f;
			fRec0[0] = ((fTemp0 < fSlow0) ? fTemp0 : ((fTemp1 > fSlow0) ? fTemp1 : fSlow0));
			float fTemp2 = 1.0f - fRec0[0];
			float fTemp3 = float(input0[i0]);
			fRec1[0] = fSlow1 + fConst2 * fRec1[1];
			float fTemp4 = 1.0f - fRec1[0];
			fRec12[0] = fSlow6 + fSlow5 * fRec12[1];
			fVec0[IOTA0 & 16383] = fTemp3 * fRec12[0];
			fRec13[0] = fSlow7 + fConst2 * fRec13[1];
			int iTemp5 = int(std::min<float>(8192.0f, std::max<float>(0.0f, fConst4 * fRec13[0])));
			float fTemp6 = 0.3f * fVec0[(IOTA0 - iTemp5) & 16383];
			fRec15[0] = 0.0f - fSlow18 * (fSlow19 * fRec15[1] - (fRec6[1] + fRec6[2]));
			fRec14[0] = fSlow16 * (fRec6[1] + fRec15[0] * fSlow21) + fSlow22 * fRec14[1];
			fVec1[IOTA0 & 32767] = 0.35355338f * fRec14[0] + 1e-20f;
			float fTemp7 = fTemp6 + fVec1[(IOTA0 - iConst10) & 32767] - 0.6f * fRec10[1];
			fVec2[IOTA0 & 8191] = fTemp7;
			fRec10[0] = fVec2[(IOTA0 - iConst11) & 8191];
			float fRec11 = 0.6f * fTemp7;
			fRec19[0] = 0.0f - fSlow18 * (fSlow19 * fRec19[1] - (fRec2[1] + fRec2[2]));
			fRec18[0] = fSlow29 * (fRec2[1] + fRec19[0] * fSlow30) + fSlow31 * fRec18[1];
			fVec3[IOTA0 & 32767] = 0.35355338f * fRec18[0] + 1e-20f;
			float fTemp8 = fVec3[(IOTA0 - iConst15) & 32767] + fTemp6 - 0.6f * fRec16[1];
			fVec4[IOTA0 & 4095] = fTemp8;
			fRec16[0] = fVec4[(IOTA0 - iConst16) & 4095];
			float fRec17 = 0.6f * fTemp8;
			fRec23[0] = 0.0f - fSlow18 * (fSlow19 * fRec23[1] - (fRec8[1] + fRec8[2]));
			fRec22[0] = fSlow38 * (fRec8[1] + fRec23[0] * fSlow39) + fSlow40 * fRec22[1];
			fVec5[IOTA0 & 32767] = 0.35355338f * fRec22[0] + 1e-20f;
			float fTemp9 = fVec5[(IOTA0 - iConst20) & 32767] - (fTemp6 + 0.6f * fRec20[1]);
			fVec6[IOTA0 & 4095] = fTemp9;
			fRec20[0] = fVec6[(IOTA0 - iConst21) & 4095];
			float fRec21 = 0.6f * fTemp9;
			fRec27[0] = 0.0f - fSlow18 * (fSlow19 * fRec27[1] - (fRec4[1] + fRec4[2]));
			fRec26[0] = fSlow47 * (fRec4[1] + fRec27[0] * fSlow48) + fSlow49 * fRec26[1];
			fVec7[IOTA0 & 32767] = 0.35355338f * fRec26[0] + 1e-20f;
			float fTemp10 = fVec7[(IOTA0 - iConst25) & 32767] - (fTemp6 + 0.6f * fRec24[1]);
			fVec8[IOTA0 & 8191] = fTemp10;
			fRec24[0] = fVec8[(IOTA0 - iConst26) & 8191];
			float fRec25 = 0.6f * fTemp10;
			fRec31[0] = 0.0f - fSlow18 * (fSlow19 * fRec31[1] - (fRec7[1] + fRec7[2]));
			fRec30[0] = fSlow56 * (fRec7[1] + fRec31[0] * fSlow57) + fSlow58 * fRec30[1];
			fVec9[IOTA0 & 32767] = 0.35355338f * fRec30[0] + 1e-20f;
			float fTemp11 = float(input1[i0]);
			fVec10[IOTA0 & 16383] = fTemp11 * fRec12[0];
			float fTemp12 = 0.3f * fVec10[(IOTA0 - iTemp5) & 16383];
			float fTemp13 = fVec9[(IOTA0 - iConst30) & 32767] + fTemp12 + 0.6f * fRec28[1];
			fVec11[IOTA0 & 8191] = fTemp13;
			fRec28[0] = fVec11[(IOTA0 - iConst31) & 8191];
			float fRec29 = 0.0f - 0.6f * fTemp13;
			fRec35[0] = 0.0f - fSlow18 * (fSlow19 * fRec35[1] - (fRec3[1] + fRec3[2]));
			fRec34[0] = fSlow65 * (fRec3[1] + fRec35[0] * fSlow66) + fSlow67 * fRec34[1];
			fVec12[IOTA0 & 65535] = 0.35355338f * fRec34[0] + 1e-20f;
			float fTemp14 = fTemp12 + 0.6f * fRec32[1] + fVec12[(IOTA0 - iConst35) & 65535];
			fVec13[IOTA0 & 8191] = fTemp14;
			fRec32[0] = fVec13[(IOTA0 - iConst36) & 8191];
			float fRec33 = 0.0f - 0.6f * fTemp14;
			fRec39[0] = 0.0f - fSlow18 * (fSlow19 * fRec39[1] - (fRec9[1] + fRec9[2]));
			fRec38[0] = fSlow74 * (fRec9[1] + fRec39[0] * fSlow75) + fSlow76 * fRec38[1];
			fVec14[IOTA0 & 65535] = 0.35355338f * fRec38[0] + 1e-20f;
			float fTemp15 = 0.6f * fRec36[1] + fVec14[(IOTA0 - iConst40) & 65535] - fTemp12;
			fVec15[IOTA0 & 4095] = fTemp15;
			fRec36[0] = fVec15[(IOTA0 - iConst41) & 4095];
			float fRec37 = 0.0f - 0.6f * fTemp15;
			fRec43[0] = 0.0f - fSlow18 * (fSlow19 * fRec43[1] - (fRec5[1] + fRec5[2]));
			fRec42[0] = fSlow83 * (fRec5[1] + fRec43[0] * fSlow84) + fSlow85 * fRec42[1];
			fVec16[IOTA0 & 65535] = 0.35355338f * fRec42[0] + 1e-20f;
			float fTemp16 = 0.6f * fRec40[1] + fVec16[(IOTA0 - iConst45) & 65535] - fTemp12;
			fVec17[IOTA0 & 8191] = fTemp16;
			fRec40[0] = fVec17[(IOTA0 - iConst46) & 8191];
			float fRec41 = 0.0f - 0.6f * fTemp16;
			float fTemp17 = fRec41 + fRec37;
			float fTemp18 = fRec21 + fRec25 + fTemp17;
			fRec2[0] = fRec10[1] + fRec16[1] + fRec20[1] + fRec24[1] + fRec28[1] + fRec32[1] + fRec36[1] + fRec40[1] + fRec11 + fRec17 + fRec29 + fRec33 + fTemp18;
			float fTemp19 = fRec25 + fRec21;
			fRec3[0] = fRec10[1] + fRec16[1] + fRec20[1] + fRec24[1] + fRec11 + fRec17 + fTemp19 - (fRec28[1] + fRec32[1] + fRec36[1] + fRec40[1] + fRec29 + fRec33 + fTemp17);
			fRec4[0] = fRec10[1] + fRec16[1] + fRec28[1] + fRec32[1] + fRec11 + fRec17 + fRec33 + fRec29 - (fRec20[1] + fRec24[1] + fRec36[1] + fRec40[1] + fTemp18);
			fRec5[0] = fRec10[1] + fRec16[1] + fRec36[1] + fRec40[1] + fRec11 + fRec17 + fTemp17 - (fRec20[1] + fRec24[1] + fRec28[1] + fRec32[1] + fRec29 + fRec33 + fTemp19);
			float fTemp20 = fRec25 + fRec41;
			float fTemp21 = fRec21 + fRec37;
			fRec6[0] = fRec16[1] + fRec24[1] + fRec32[1] + fRec40[1] + fRec17 + fRec33 + fTemp20 - (fRec10[1] + fRec20[1] + fRec28[1] + fRec36[1] + fRec11 + fRec29 + fTemp21);
			float fTemp22 = fRec25 + fRec37;
			float fTemp23 = fRec21 + fRec41;
			fRec7[0] = fRec16[1] + fRec24[1] + fRec28[1] + fRec36[1] + fRec17 + fRec29 + fTemp22 - (fRec10[1] + fRec20[1] + fRec32[1] + fRec40[1] + fRec11 + fRec33 + fTemp23);
			fRec8[0] = fRec16[1] + fRec20[1] + fRec32[1] + fRec36[1] + fRec17 + fRec33 + fTemp21 - (fRec10[1] + fRec24[1] + fRec28[1] + fRec40[1] + fRec11 + fRec29 + fTemp20);
			fRec9[0] = fRec16[1] + fRec20[1] + fRec28[1] + fRec40[1] + fRec17 + fRec29 + fTemp23 - (fRec10[1] + fRec24[1] + fRec32[1] + fRec36[1] + fRec11 + fRec33 + fTemp22);
			output0[i0] = FAUSTFLOAT(fTemp2 * (fTemp3 * fTemp4 + 0.37f * fRec1[0] * (fRec3[0] + fRec4[0])) + fTemp3 * fRec0[0]);
			output1[i0] = FAUSTFLOAT(fTemp2 * (fTemp11 * fTemp4 + 0.37f * fRec1[0] * (fRec3[0] - fRec4[0])) + fTemp11 * fRec0[0]);
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec12[1] = fRec12[0];
			IOTA0 = IOTA0 + 1;
			fRec13[1] = fRec13[0];
			fRec15[1] = fRec15[0];
			fRec14[1] = fRec14[0];
			fRec10[1] = fRec10[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec16[1] = fRec16[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fRec20[1] = fRec20[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec28[1] = fRec28[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fRec32[1] = fRec32[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fRec36[1] = fRec36[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec40[1] = fRec40[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
		}
	}

};
#endif

class faust_fx_zita_rev1_tilde : public FaustExternal<fx_zita_rev1> {
public:
    faust_fx_zita_rev1_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(fx_zita_rev1))
    {
    }
};

#endif
