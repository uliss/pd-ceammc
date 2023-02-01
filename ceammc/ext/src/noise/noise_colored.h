/* ------------------------------------------------------------
name: "noise.colored"
Code generated with Faust 2.53.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn noise_colored -scn noise_colored_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __noise_colored_H__
#define  __noise_colored_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN noise_colored_dsp.h ********************************
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

class FAUST_API noise_colored_dsp {

    public:

        noise_colored_dsp() {}
        virtual ~noise_colored_dsp() {}

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
        virtual noise_colored_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public noise_colored_dsp {

    protected:

        noise_colored_dsp* fDSP;

    public:

        decorator_dsp(noise_colored_dsp* noise_colored_dsp = nullptr):fDSP(noise_colored_dsp) {}
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
    
        virtual noise_colored_dsp* createDSPInstance() = 0;
    
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

/************************** END noise_colored_dsp.h **************************/
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
 The base class of Meta handler to be used in noise_colored_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct noise_colored : public noise_colored_dsp {
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
#define FAUSTCLASS noise_colored
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


class noise_colored : public noise_colored_dsp {
	
 private:
	
	int fSampleRate;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fConst3;
	float fConst4;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst10;
	float fConst11;
	float fConst13;
	float fConst14;
	float fConst16;
	float fConst17;
	float fConst19;
	float fConst20;
	float fConst22;
	float fConst23;
	float fConst25;
	float fConst26;
	float fConst27;
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst37;
	float fConst38;
	float fConst39;
	float fConst41;
	float fConst42;
	float fConst43;
	float fConst44;
	int iRec13[2];
	float fVec0[2];
	float fRec12[2];
	float fRec11[2];
	float fVec1[2];
	float fRec10[2];
	float fVec2[2];
	float fRec9[2];
	float fVec3[2];
	float fRec8[2];
	float fVec4[2];
	float fRec7[2];
	float fVec5[2];
	float fRec6[2];
	float fVec6[2];
	float fConst45;
	float fRec5[2];
	float fVec7[2];
	float fConst46;
	float fRec4[2];
	float fVec8[2];
	float fConst47;
	float fRec3[2];
	float fVec9[2];
	float fConst48;
	float fRec2[2];
	float fVec10[2];
	float fConst49;
	float fRec1[2];
	float fVec11[2];
	float fConst50;
	float fRec0[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn noise_colored -scn noise_colored_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "noise_colored.dsp");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
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
		m->declare("filters.lib/version", "0.3");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "noise.colored");
		m->declare("noises.lib/colored_noise:author", "Constantinos Odysseas Economou");
		m->declare("noises.lib/colored_noise:copyright", "Copyright (C) 2022 Constantinos Odysseas Economou <c.economou@sirenfx.io>");
		m->declare("noises.lib/colored_noise:license", "MIT-style STK-4.3 license");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.4");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::tan(62831.85f / fConst0);
		fConst2 = 62.831852f / fConst0;
		fConst3 = 1.0f / std::tan(0.5f / fConst0);
		fConst4 = std::tan(fConst2);
		float fConst5 = 125.663704f * (fConst1 / fConst4);
		fConst6 = 1.0f / (fConst3 + fConst5);
		fConst7 = std::tan(33531.47f / fConst0);
		fConst8 = 125.663704f / fConst4;
		float fConst9 = 125.663704f * (fConst7 / fConst4);
		fConst10 = 1.0f / (fConst3 + fConst9);
		fConst11 = std::tan(17894.736f / fConst0);
		float fConst12 = 125.663704f * (fConst11 / fConst4);
		fConst13 = 1.0f / (fConst3 + fConst12);
		fConst14 = std::tan(9549.883f / fConst0);
		float fConst15 = 125.663704f * (fConst14 / fConst4);
		fConst16 = 1.0f / (fConst3 + fConst15);
		fConst17 = std::tan(5096.4854f / fConst0);
		float fConst18 = 125.663704f * (fConst17 / fConst4);
		fConst19 = 1.0f / (fConst3 + fConst18);
		fConst20 = std::tan(2719.8408f / fConst0);
		float fConst21 = 125.663704f * (fConst20 / fConst4);
		fConst22 = 1.0f / (fConst3 + fConst21);
		fConst23 = std::tan(1451.4973f / fConst0);
		float fConst24 = 125.663704f * (fConst23 / fConst4);
		fConst25 = 1.0f / (fConst3 + fConst24);
		fConst26 = fConst24 - fConst3;
		fConst27 = std::tan(774.6204f / fConst0);
		float fConst28 = 125.663704f * (fConst27 / fConst4);
		fConst29 = 1.0f / (fConst3 + fConst28);
		fConst30 = fConst28 - fConst3;
		fConst31 = std::tan(413.39163f / fConst0);
		float fConst32 = 125.663704f * (fConst31 / fConst4);
		fConst33 = 1.0f / (fConst3 + fConst32);
		fConst34 = fConst32 - fConst3;
		fConst35 = std::tan(220.61469f / fConst0);
		float fConst36 = 125.663704f * (fConst35 / fConst4);
		fConst37 = 1.0f / (fConst3 + fConst36);
		fConst38 = fConst36 - fConst3;
		fConst39 = std::tan(117.73542f / fConst0);
		float fConst40 = 125.663704f * (fConst39 / fConst4);
		fConst41 = 1.0f / (fConst3 + fConst40);
		fConst42 = fConst40 - fConst3;
		fConst43 = 1.0f / (fConst3 + 125.663704f);
		fConst44 = 125.663704f - fConst3;
		fConst45 = fConst21 - fConst3;
		fConst46 = fConst18 - fConst3;
		fConst47 = fConst15 - fConst3;
		fConst48 = fConst12 - fConst3;
		fConst49 = fConst9 - fConst3;
		fConst50 = fConst5 - fConst3;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iRec13[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fVec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec12[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec11[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec10[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fVec2[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec9[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fVec3[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fVec4[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec7[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fVec5[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec6[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec6[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec5[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fVec7[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fRec4[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fVec8[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec3[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fVec9[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec2[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec10[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec1[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fVec11[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec0[l25] = 0.0f;
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
	
	virtual noise_colored* clone() {
		return new noise_colored();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("noise.colored");
		ui_interface->addHorizontalSlider("alpha", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::tan(fConst2 * std::pow(1.8738174f, 11.0f - fSlow0));
		float fSlow2 = fConst1 / fSlow1;
		float fSlow3 = fConst8 * fSlow1;
		float fSlow4 = std::tan(fConst2 * std::pow(1.8738174f, 1e+01f - fSlow0));
		float fSlow5 = fConst7 * ((fConst3 + fSlow3) / fSlow4);
		float fSlow6 = fConst8 * fSlow4;
		float fSlow7 = std::tan(fConst2 * std::pow(1.8738174f, 9.0f - fSlow0));
		float fSlow8 = fConst11 * ((fConst3 + fSlow6) / fSlow7);
		float fSlow9 = fConst8 * fSlow7;
		float fSlow10 = std::tan(fConst2 * std::pow(1.8738174f, 8.0f - fSlow0));
		float fSlow11 = fConst14 * ((fConst3 + fSlow9) / fSlow10);
		float fSlow12 = fConst8 * fSlow10;
		float fSlow13 = std::tan(fConst2 * std::pow(1.8738174f, 7.0f - fSlow0));
		float fSlow14 = fConst17 * ((fConst3 + fSlow12) / fSlow13);
		float fSlow15 = fConst8 * fSlow13;
		float fSlow16 = std::tan(fConst2 * std::pow(1.8738174f, 6.0f - fSlow0));
		float fSlow17 = fConst20 * ((fConst3 + fSlow15) / fSlow16);
		float fSlow18 = fConst8 * fSlow16;
		float fSlow19 = std::tan(fConst2 * std::pow(1.8738174f, 5.0f - fSlow0));
		float fSlow20 = fConst23 * ((fConst3 + fSlow18) / fSlow19);
		float fSlow21 = fConst8 * fSlow19;
		float fSlow22 = std::tan(fConst2 * std::pow(1.8738174f, 4.0f - fSlow0));
		float fSlow23 = fConst27 * ((fConst3 + fSlow21) / fSlow22);
		float fSlow24 = fConst8 * fSlow22;
		float fSlow25 = std::tan(fConst2 * std::pow(1.8738174f, 3.0f - fSlow0));
		float fSlow26 = fConst31 * ((fConst3 + fSlow24) / fSlow25);
		float fSlow27 = fConst8 * fSlow25;
		float fSlow28 = std::tan(fConst2 * std::pow(1.8738174f, 2.0f - fSlow0));
		float fSlow29 = fConst35 * ((fConst3 + fSlow27) / fSlow28);
		float fSlow30 = fConst8 * fSlow28;
		float fSlow31 = std::tan(fConst2 * std::pow(1.8738174f, 1.0f - fSlow0));
		float fSlow32 = fConst39 * ((fConst3 + fSlow30) / fSlow31);
		float fSlow33 = fConst8 * fSlow31;
		float fSlow34 = std::tan(fConst2 * std::pow(1.8738174f, -1.0f * fSlow0));
		float fSlow35 = fConst4 * ((fConst3 + fSlow33) / fSlow34);
		float fSlow36 = fConst8 * fSlow34;
		float fSlow37 = fConst3 + fSlow36;
		float fSlow38 = fSlow36 - fConst3;
		float fSlow39 = fSlow33 - fConst3;
		float fSlow40 = fConst4 / fSlow34;
		float fSlow41 = fSlow30 - fConst3;
		float fSlow42 = fConst39 / fSlow31;
		float fSlow43 = fSlow27 - fConst3;
		float fSlow44 = fConst35 / fSlow28;
		float fSlow45 = fSlow24 - fConst3;
		float fSlow46 = fConst31 / fSlow25;
		float fSlow47 = fSlow21 - fConst3;
		float fSlow48 = fConst27 / fSlow22;
		float fSlow49 = fSlow18 - fConst3;
		float fSlow50 = fConst23 / fSlow19;
		float fSlow51 = fSlow15 - fConst3;
		float fSlow52 = fConst20 / fSlow16;
		float fSlow53 = fSlow12 - fConst3;
		float fSlow54 = fConst17 / fSlow13;
		float fSlow55 = fSlow9 - fConst3;
		float fSlow56 = fConst14 / fSlow10;
		float fSlow57 = fSlow6 - fConst3;
		float fSlow58 = fConst11 / fSlow7;
		float fSlow59 = fSlow3 - fConst3;
		float fSlow60 = fConst7 / fSlow4;
		float fSlow61 = 2.0f * fSlow0;
		int iSlow62 = ((fSlow61 > 0.0f) - (fSlow61 < 0.0f)) > 0;
		float fSlow63 = std::pow(((iSlow62) ? 1.0f : 0.8016f) * std::exp(0.0f - fSlow61 * ((iSlow62) ? -4.28f : -2.633f)) + ((iSlow62) ? 0.0f : 0.1984f) * std::exp(0.0f - fSlow61 * ((iSlow62) ? 0.0f : -0.7196f)), -1.0f);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iRec13[0] = 1103515245 * iRec13[1] + 12345;
			float fTemp0 = float(iRec13[0]);
			fVec0[0] = fTemp0;
			fRec12[0] = 0.995f * fRec12[1] + 4.656613e-10f * (fTemp0 - fVec0[1]);
			fRec11[0] = 0.0f - fConst43 * (fConst44 * fRec11[1] - (fSlow37 * fRec12[0] + fSlow38 * fRec12[1]));
			fVec1[0] = fSlow40 * fRec11[0];
			fRec10[0] = 0.0f - fConst41 * (fConst42 * fRec10[1] - (fSlow35 * fRec11[0] + fSlow39 * fVec1[1]));
			fVec2[0] = fSlow42 * fRec10[0];
			fRec9[0] = 0.0f - fConst37 * (fConst38 * fRec9[1] - (fSlow32 * fRec10[0] + fSlow41 * fVec2[1]));
			fVec3[0] = fSlow44 * fRec9[0];
			fRec8[0] = 0.0f - fConst33 * (fConst34 * fRec8[1] - (fSlow29 * fRec9[0] + fSlow43 * fVec3[1]));
			fVec4[0] = fSlow46 * fRec8[0];
			fRec7[0] = 0.0f - fConst29 * (fConst30 * fRec7[1] - (fSlow26 * fRec8[0] + fSlow45 * fVec4[1]));
			fVec5[0] = fSlow48 * fRec7[0];
			fRec6[0] = 0.0f - fConst25 * (fConst26 * fRec6[1] - (fSlow23 * fRec7[0] + fSlow47 * fVec5[1]));
			fVec6[0] = fSlow50 * fRec6[0];
			fRec5[0] = fConst22 * (fSlow20 * fRec6[0] + fSlow49 * fVec6[1] - fConst45 * fRec5[1]);
			fVec7[0] = fSlow52 * fRec5[0];
			fRec4[0] = fConst19 * (fSlow17 * fRec5[0] + fSlow51 * fVec7[1] - fConst46 * fRec4[1]);
			fVec8[0] = fSlow54 * fRec4[0];
			fRec3[0] = fConst16 * (fSlow14 * fRec4[0] + fSlow53 * fVec8[1] - fConst47 * fRec3[1]);
			fVec9[0] = fSlow56 * fRec3[0];
			fRec2[0] = fConst13 * (fSlow11 * fRec3[0] + fSlow55 * fVec9[1] - fConst48 * fRec2[1]);
			fVec10[0] = fSlow58 * fRec2[0];
			fRec1[0] = fConst10 * (fSlow8 * fRec2[0] + fSlow57 * fVec10[1] - fConst49 * fRec1[1]);
			fVec11[0] = fSlow60 * fRec1[0];
			fRec0[0] = fConst6 * (fSlow5 * fRec1[0] + fSlow59 * fVec11[1] - fConst50 * fRec0[1]);
			output0[i0] = FAUSTFLOAT(std::min<float>(1.0f, std::max<float>(-1.0f, fSlow2 * fRec0[0] * fSlow63)));
			iRec13[1] = iRec13[0];
			fVec0[1] = fVec0[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fVec1[1] = fVec1[0];
			fRec10[1] = fRec10[0];
			fVec2[1] = fVec2[0];
			fRec9[1] = fRec9[0];
			fVec3[1] = fVec3[0];
			fRec8[1] = fRec8[0];
			fVec4[1] = fVec4[0];
			fRec7[1] = fRec7[0];
			fVec5[1] = fVec5[0];
			fRec6[1] = fRec6[0];
			fVec6[1] = fVec6[0];
			fRec5[1] = fRec5[0];
			fVec7[1] = fVec7[0];
			fRec4[1] = fRec4[0];
			fVec8[1] = fVec8[0];
			fRec3[1] = fRec3[0];
			fVec9[1] = fVec9[0];
			fRec2[1] = fRec2[0];
			fVec10[1] = fVec10[0];
			fRec1[1] = fRec1[0];
			fVec11[1] = fVec11[0];
			fRec0[1] = fRec0[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _noise_colored_UI : public UI {
    static std::string name;
};

template <class T>
std::string _noise_colored_UI<T>::name(sym(noise_colored));

typedef _noise_colored_UI<noise_colored> noise_colored_UI;

class faust_noise_colored_tilde : public FaustExternal<noise_colored, noise_colored_UI> {
public:
    faust_noise_colored_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
