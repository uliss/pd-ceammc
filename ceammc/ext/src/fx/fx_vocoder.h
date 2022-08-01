/* ------------------------------------------------------------
name: "fx.vocoder"
Code generated with Faust 2.44.1 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_vocoder -scn fx_vocoder_dsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_vocoder_H__
#define  __fx_vocoder_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_vocoder_dsp.h ********************************
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

class FAUST_API fx_vocoder_dsp {

    public:

        fx_vocoder_dsp() {}
        virtual ~fx_vocoder_dsp() {}

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
        virtual fx_vocoder_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public fx_vocoder_dsp {

    protected:

        fx_vocoder_dsp* fDSP;

    public:

        decorator_dsp(fx_vocoder_dsp* fx_vocoder_dsp = nullptr):fDSP(fx_vocoder_dsp) {}
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
    
        virtual fx_vocoder_dsp* createDSPInstance() = 0;
    
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

/************************** END fx_vocoder_dsp.h **************************/
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
 The base class of Meta handler to be used in fx_vocoder_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct fx_vocoder : public fx_vocoder_dsp {
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
#define FAUSTCLASS fx_vocoder
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

static float fx_vocoder_faustpower2_f(float value) {
	return value * value;
}

class fx_vocoder : public fx_vocoder_dsp {
	
 private:
	
	int fSampleRate;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fConst3;
	float fRec0[3];
	float fRec2[3];
	float fConst4;
	FAUSTFLOAT fHslider1;
	float fConst5;
	float fRec3[2];
	float fConst6;
	FAUSTFLOAT fHslider2;
	float fRec4[2];
	float fRec1[2];
	float fConst8;
	float fConst9;
	float fRec5[3];
	float fRec7[3];
	float fRec6[2];
	float fConst11;
	float fConst12;
	float fRec8[3];
	float fRec10[3];
	float fRec9[2];
	float fConst14;
	float fConst15;
	float fRec11[3];
	float fRec13[3];
	float fRec12[2];
	float fConst17;
	float fConst18;
	float fRec14[3];
	float fRec16[3];
	float fRec15[2];
	float fConst20;
	float fConst21;
	float fRec17[3];
	float fRec19[3];
	float fRec18[2];
	float fConst23;
	float fConst24;
	float fRec20[3];
	float fRec22[3];
	float fRec21[2];
	float fConst26;
	float fConst27;
	float fRec23[3];
	float fRec25[3];
	float fRec24[2];
	float fConst29;
	float fConst30;
	float fRec26[3];
	float fRec28[3];
	float fRec27[2];
	float fConst32;
	float fConst33;
	float fRec29[3];
	float fRec31[3];
	float fRec30[2];
	float fConst35;
	float fConst36;
	float fRec32[3];
	float fRec34[3];
	float fRec33[2];
	float fConst38;
	float fConst39;
	float fRec35[3];
	float fRec37[3];
	float fRec36[2];
	float fConst41;
	float fConst42;
	float fRec38[3];
	float fRec40[3];
	float fRec39[2];
	float fConst44;
	float fConst45;
	float fRec41[3];
	float fRec43[3];
	float fRec42[2];
	float fConst47;
	float fConst48;
	float fRec44[3];
	float fRec46[3];
	float fRec45[2];
	float fConst50;
	float fConst51;
	float fRec47[3];
	float fRec49[3];
	float fRec48[2];
	float fConst53;
	float fConst54;
	float fRec50[3];
	float fRec52[3];
	float fRec51[2];
	float fConst56;
	float fConst57;
	float fRec53[3];
	float fRec55[3];
	float fRec54[2];
	float fConst59;
	float fConst60;
	float fRec56[3];
	float fRec58[3];
	float fRec57[2];
	float fConst62;
	float fConst63;
	float fRec59[3];
	float fRec61[3];
	float fRec60[2];
	float fConst65;
	float fConst66;
	float fRec62[3];
	float fRec64[3];
	float fRec63[2];
	float fConst68;
	float fConst69;
	float fRec65[3];
	float fRec67[3];
	float fRec66[2];
	float fConst71;
	float fConst72;
	float fRec68[3];
	float fRec70[3];
	float fRec69[2];
	float fConst74;
	float fConst75;
	float fRec71[3];
	float fRec73[3];
	float fRec72[2];
	float fConst77;
	float fConst78;
	float fRec74[3];
	float fRec76[3];
	float fRec75[2];
	float fConst80;
	float fConst81;
	float fRec77[3];
	float fRec79[3];
	float fRec78[2];
	float fConst83;
	float fConst84;
	float fRec80[3];
	float fRec82[3];
	float fRec81[2];
	float fConst86;
	float fConst87;
	float fRec83[3];
	float fRec85[3];
	float fRec84[2];
	float fConst89;
	float fConst90;
	float fRec86[3];
	float fRec88[3];
	float fRec87[2];
	float fConst92;
	float fConst93;
	float fRec89[3];
	float fRec91[3];
	float fRec90[2];
	float fConst95;
	float fConst96;
	float fRec92[3];
	float fRec94[3];
	float fRec93[2];
	float fConst98;
	float fConst99;
	float fRec95[3];
	float fRec97[3];
	float fRec96[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/amp_follower_ar:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.2");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.8");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -i -cn fx_vocoder -scn fx_vocoder_dsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "fx_vocoder.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/resonbp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonbp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonbp:license", "MIT-style STK-4.3 license");
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
		m->declare("name", "fx.vocoder");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
		m->declare("signals.lib/version", "0.3");
		m->declare("vaeffects.lib/name", "Faust Virtual Analog Filter Effect Library");
		m->declare("vaeffects.lib/oneVocoderBand:author", "Romain Michon");
		m->declare("vaeffects.lib/version", "0.2");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		float fConst1 = std::tan(27228.9238f / fConst0);
		fConst2 = 1.0f / fConst1;
		fConst3 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst1));
		fConst4 = 0.0441000015f / fConst0;
		fConst5 = 1.0f - 44.0999985f / fConst0;
		fConst6 = 1.0f / fConst0;
		float fConst7 = std::tan(22406.0742f / fConst0);
		fConst8 = 1.0f / fConst7;
		fConst9 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst7));
		float fConst10 = std::tan(18437.4609f / fConst0);
		fConst11 = 1.0f / fConst10;
		fConst12 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst10));
		float fConst13 = std::tan(15171.7754f / fConst0);
		fConst14 = 1.0f / fConst13;
		fConst15 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst13));
		float fConst16 = std::tan(12484.5166f / fConst0);
		fConst17 = 1.0f / fConst16;
		fConst18 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst16));
		float fConst19 = std::tan(10273.2305f / fConst0);
		fConst20 = 1.0f / fConst19;
		fConst21 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst19));
		float fConst22 = std::tan(1777.1532f / fConst0);
		fConst23 = 1.0f / fConst22;
		fConst24 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst22));
		float fConst25 = std::tan(1462.37976f / fConst0);
		fConst26 = 1.0f / fConst25;
		fConst27 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst25));
		float fConst28 = std::tan(1203.35974f / fConst0);
		fConst29 = 1.0f / fConst28;
		fConst30 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst28));
		float fConst31 = std::tan(990.217957f / fConst0);
		fConst32 = 1.0f / fConst31;
		fConst33 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst31));
		float fConst34 = std::tan(814.828308f / fConst0);
		fConst35 = 1.0f / fConst34;
		fConst36 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst34));
		float fConst37 = std::tan(670.504089f / fConst0);
		fConst38 = 1.0f / fConst37;
		fConst39 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst37));
		float fConst40 = std::tan(551.742859f / fConst0);
		fConst41 = 1.0f / fConst40;
		fConst42 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst40));
		float fConst43 = std::tan(454.016937f / fConst0);
		fConst44 = 1.0f / fConst43;
		fConst45 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst43));
		float fConst46 = std::tan(373.600433f / fConst0);
		fConst47 = 1.0f / fConst46;
		fConst48 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst46));
		float fConst49 = std::tan(40212.3867f / fConst0);
		fConst50 = 1.0f / fConst49;
		fConst51 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst49));
		float fConst52 = std::tan(33089.8789f / fConst0);
		fConst53 = 1.0f / fConst52;
		fConst54 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst52));
		float fConst55 = std::tan(8453.61328f / fConst0);
		fConst56 = 1.0f / fConst55;
		fConst57 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst55));
		float fConst58 = std::tan(6956.28955f / fConst0);
		fConst59 = 1.0f / fConst58;
		fConst60 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst58));
		float fConst61 = std::tan(5724.17578f / fConst0);
		fConst62 = 1.0f / fConst61;
		fConst63 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst61));
		float fConst64 = std::tan(4710.29688f / fConst0);
		fConst65 = 1.0f / fConst64;
		fConst66 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst64));
		float fConst67 = std::tan(3875.99854f / fConst0);
		fConst68 = 1.0f / fConst67;
		fConst69 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst67));
		float fConst70 = std::tan(3189.4729f / fConst0);
		fConst71 = 1.0f / fConst70;
		fConst72 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst70));
		float fConst73 = std::tan(2624.54639f / fConst0);
		fConst74 = 1.0f / fConst73;
		fConst75 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst73));
		float fConst76 = std::tan(2159.68066f / fConst0);
		fConst77 = 1.0f / fConst76;
		fConst78 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst76));
		float fConst79 = std::tan(307.42749f / fConst0);
		fConst80 = 1.0f / fConst79;
		fConst81 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst79));
		float fConst82 = std::tan(252.975235f / fConst0);
		fConst83 = 1.0f / fConst82;
		fConst84 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst82));
		float fConst85 = std::tan(208.167679f / fConst0);
		fConst86 = 1.0f / fConst85;
		fConst87 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst85));
		float fConst88 = std::tan(171.296555f / fConst0);
		fConst89 = 1.0f / fConst88;
		fConst90 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst88));
		float fConst91 = std::tan(140.956116f / fConst0);
		fConst92 = 1.0f / fConst91;
		fConst93 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst91));
		float fConst94 = std::tan(115.989655f / fConst0);
		fConst95 = 1.0f / fConst94;
		fConst96 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst94));
		float fConst97 = std::tan(95.4453049f / fConst0);
		fConst98 = 1.0f / fConst97;
		fConst99 = 2.0f * (1.0f - 1.0f / fx_vocoder_faustpower2_f(fConst97));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(5.0f);
		fHslider2 = FAUSTFLOAT(5.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 3; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 3; l1 = l1 + 1) {
			fRec2[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec3[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = l5 + 1) {
			fRec5[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec7[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec6[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec8[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec10[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec11[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec13[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec12[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec14[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fRec16[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec15[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec17[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec19[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec18[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fRec20[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec22[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec21[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec23[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec25[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec24[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec26[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec28[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec27[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 3; l29 = l29 + 1) {
			fRec29[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec31[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec30[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 3; l32 = l32 + 1) {
			fRec32[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fRec34[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec33[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fRec35[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 3; l36 = l36 + 1) {
			fRec37[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec36[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 3; l38 = l38 + 1) {
			fRec38[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 3; l39 = l39 + 1) {
			fRec40[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec39[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 3; l41 = l41 + 1) {
			fRec41[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec43[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec42[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec44[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec46[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec45[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec47[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fRec49[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec48[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = l50 + 1) {
			fRec50[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec52[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec51[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fRec53[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 3; l54 = l54 + 1) {
			fRec55[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec54[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fRec56[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fRec58[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec57[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec59[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec61[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec60[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec62[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fRec64[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec63[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 3; l65 = l65 + 1) {
			fRec65[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 3; l66 = l66 + 1) {
			fRec67[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec66[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 3; l68 = l68 + 1) {
			fRec68[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec70[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec69[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec71[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 3; l72 = l72 + 1) {
			fRec73[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec72[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 3; l74 = l74 + 1) {
			fRec74[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec76[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
			fRec75[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 3; l77 = l77 + 1) {
			fRec77[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fRec79[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec78[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 3; l80 = l80 + 1) {
			fRec80[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 3; l81 = l81 + 1) {
			fRec82[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec81[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 3; l83 = l83 + 1) {
			fRec83[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 3; l84 = l84 + 1) {
			fRec85[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec84[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec86[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 3; l87 = l87 + 1) {
			fRec88[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec87[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 3; l89 = l89 + 1) {
			fRec89[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 3; l90 = l90 + 1) {
			fRec91[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			fRec90[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 3; l92 = l92 + 1) {
			fRec92[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 3; l93 = l93 + 1) {
			fRec94[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec93[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 3; l95 = l95 + 1) {
			fRec95[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 3; l96 = l96 + 1) {
			fRec97[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec96[l97] = 0.0f;
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
	
	virtual fx_vocoder* clone() {
		return new fx_vocoder();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("fx.vocoder");
		ui_interface->declare(&fHslider2, "unit", "ms");
		ui_interface->addHorizontalSlider("attack", &fHslider2, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(100.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("bwratio", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->declare(&fHslider1, "unit", "ms");
		ui_interface->addHorizontalSlider("release", &fHslider1, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(100.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = 0.177122265f * fSlow0;
		float fSlow2 = fConst2 * (fConst2 + fSlow1) + 1.0f;
		float fSlow3 = 1.0f / fSlow2;
		float fSlow4 = fConst2 * (fConst2 - fSlow1) + 1.0f;
		float fSlow5 = fConst2 / fSlow2;
		float fSlow6 = 0.0f - fSlow5;
		float fSlow7 = fConst4 * float(fHslider1);
		float fSlow8 = fConst4 * float(fHslider2);
		float fSlow9 = 0.177122265f * fSlow0;
		float fSlow10 = fConst8 * (fConst8 + fSlow9) + 1.0f;
		float fSlow11 = 1.0f / fSlow10;
		float fSlow12 = fConst8 * (fConst8 - fSlow9) + 1.0f;
		float fSlow13 = fConst8 / fSlow10;
		float fSlow14 = 0.0f - fSlow13;
		float fSlow15 = 0.177122265f * fSlow0;
		float fSlow16 = fConst11 * (fConst11 + fSlow15) + 1.0f;
		float fSlow17 = 1.0f / fSlow16;
		float fSlow18 = fConst11 * (fConst11 - fSlow15) + 1.0f;
		float fSlow19 = fConst11 / fSlow16;
		float fSlow20 = 0.0f - fSlow19;
		float fSlow21 = 0.177122265f * fSlow0;
		float fSlow22 = fConst14 * (fConst14 + fSlow21) + 1.0f;
		float fSlow23 = 1.0f / fSlow22;
		float fSlow24 = fConst14 * (fConst14 - fSlow21) + 1.0f;
		float fSlow25 = fConst14 / fSlow22;
		float fSlow26 = 0.0f - fSlow25;
		float fSlow27 = fConst17 * (fConst17 + fSlow9) + 1.0f;
		float fSlow28 = 1.0f / fSlow27;
		float fSlow29 = fConst17 * (fConst17 - fSlow9) + 1.0f;
		float fSlow30 = fConst17 / fSlow27;
		float fSlow31 = 0.0f - fSlow30;
		float fSlow32 = 0.177122265f * fSlow0;
		float fSlow33 = fConst20 * (fConst20 + fSlow32) + 1.0f;
		float fSlow34 = 1.0f / fSlow33;
		float fSlow35 = fConst20 * (fConst20 - fSlow32) + 1.0f;
		float fSlow36 = fConst20 / fSlow33;
		float fSlow37 = 0.0f - fSlow36;
		float fSlow38 = fConst23 * (fConst23 + fSlow32) + 1.0f;
		float fSlow39 = 1.0f / fSlow38;
		float fSlow40 = fConst23 * (fConst23 - fSlow32) + 1.0f;
		float fSlow41 = fConst23 / fSlow38;
		float fSlow42 = 0.0f - fSlow41;
		float fSlow43 = fConst26 * (fConst26 + fSlow32) + 1.0f;
		float fSlow44 = 1.0f / fSlow43;
		float fSlow45 = fConst26 * (fConst26 - fSlow32) + 1.0f;
		float fSlow46 = fConst26 / fSlow43;
		float fSlow47 = 0.0f - fSlow46;
		float fSlow48 = fConst29 * (fConst29 + fSlow1) + 1.0f;
		float fSlow49 = 1.0f / fSlow48;
		float fSlow50 = fConst29 * (fConst29 - fSlow1) + 1.0f;
		float fSlow51 = fConst29 / fSlow48;
		float fSlow52 = 0.0f - fSlow51;
		float fSlow53 = fConst32 * (fConst32 + fSlow21) + 1.0f;
		float fSlow54 = 1.0f / fSlow53;
		float fSlow55 = fConst32 * (fConst32 - fSlow21) + 1.0f;
		float fSlow56 = fConst32 / fSlow53;
		float fSlow57 = 0.0f - fSlow56;
		float fSlow58 = 0.177122265f * fSlow0;
		float fSlow59 = fConst35 * (fConst35 + fSlow58) + 1.0f;
		float fSlow60 = 1.0f / fSlow59;
		float fSlow61 = fConst35 * (fConst35 - fSlow58) + 1.0f;
		float fSlow62 = fConst35 / fSlow59;
		float fSlow63 = 0.0f - fSlow62;
		float fSlow64 = fConst38 * (fConst38 + fSlow21) + 1.0f;
		float fSlow65 = 1.0f / fSlow64;
		float fSlow66 = fConst38 * (fConst38 - fSlow21) + 1.0f;
		float fSlow67 = fConst38 / fSlow64;
		float fSlow68 = 0.0f - fSlow67;
		float fSlow69 = 0.177122265f * fSlow0;
		float fSlow70 = fConst41 * (fConst41 + fSlow69) + 1.0f;
		float fSlow71 = 1.0f / fSlow70;
		float fSlow72 = fConst41 * (fConst41 - fSlow69) + 1.0f;
		float fSlow73 = fConst41 / fSlow70;
		float fSlow74 = 0.0f - fSlow73;
		float fSlow75 = 0.177122265f * fSlow0;
		float fSlow76 = fConst44 * (fConst44 + fSlow75) + 1.0f;
		float fSlow77 = 1.0f / fSlow76;
		float fSlow78 = fConst44 * (fConst44 - fSlow75) + 1.0f;
		float fSlow79 = fConst44 / fSlow76;
		float fSlow80 = 0.0f - fSlow79;
		float fSlow81 = fConst47 * (fConst47 + fSlow9) + 1.0f;
		float fSlow82 = 1.0f / fSlow81;
		float fSlow83 = fConst47 * (fConst47 - fSlow9) + 1.0f;
		float fSlow84 = fConst47 / fSlow81;
		float fSlow85 = 0.0f - fSlow84;
		float fSlow86 = 0.177122265f * fSlow0;
		float fSlow87 = fConst50 * (fConst50 + fSlow86) + 1.0f;
		float fSlow88 = 1.0f / fSlow87;
		float fSlow89 = fConst50 * (fConst50 - fSlow86) + 1.0f;
		float fSlow90 = fConst50 / fSlow87;
		float fSlow91 = 0.0f - fSlow90;
		float fSlow92 = 0.177122265f * fSlow0;
		float fSlow93 = fConst53 * (fConst53 + fSlow92) + 1.0f;
		float fSlow94 = 1.0f / fSlow93;
		float fSlow95 = fConst53 * (fConst53 - fSlow92) + 1.0f;
		float fSlow96 = fConst53 / fSlow93;
		float fSlow97 = 0.0f - fSlow96;
		float fSlow98 = fConst56 * (fConst56 + fSlow86) + 1.0f;
		float fSlow99 = 1.0f / fSlow98;
		float fSlow100 = fConst56 * (fConst56 - fSlow86) + 1.0f;
		float fSlow101 = fConst56 / fSlow98;
		float fSlow102 = 0.0f - fSlow101;
		float fSlow103 = 0.177122265f * fSlow0;
		float fSlow104 = fConst59 * (fConst59 + fSlow103) + 1.0f;
		float fSlow105 = 1.0f / fSlow104;
		float fSlow106 = fConst59 * (fConst59 - fSlow103) + 1.0f;
		float fSlow107 = fConst59 / fSlow104;
		float fSlow108 = 0.0f - fSlow107;
		float fSlow109 = fConst62 * (fConst62 + fSlow1) + 1.0f;
		float fSlow110 = 1.0f / fSlow109;
		float fSlow111 = fConst62 * (fConst62 - fSlow1) + 1.0f;
		float fSlow112 = fConst62 / fSlow109;
		float fSlow113 = 0.0f - fSlow112;
		float fSlow114 = fConst65 * (fConst65 + fSlow103) + 1.0f;
		float fSlow115 = 1.0f / fSlow114;
		float fSlow116 = fConst65 * (fConst65 - fSlow103) + 1.0f;
		float fSlow117 = fConst65 / fSlow114;
		float fSlow118 = 0.0f - fSlow117;
		float fSlow119 = fConst68 * (fConst68 + fSlow1) + 1.0f;
		float fSlow120 = 1.0f / fSlow119;
		float fSlow121 = fConst68 * (fConst68 - fSlow1) + 1.0f;
		float fSlow122 = fConst68 / fSlow119;
		float fSlow123 = 0.0f - fSlow122;
		float fSlow124 = fConst71 * (fConst71 + fSlow75) + 1.0f;
		float fSlow125 = 1.0f / fSlow124;
		float fSlow126 = fConst71 * (fConst71 - fSlow75) + 1.0f;
		float fSlow127 = fConst71 / fSlow124;
		float fSlow128 = 0.0f - fSlow127;
		float fSlow129 = 0.177122265f * fSlow0;
		float fSlow130 = fConst74 * (fConst74 + fSlow129) + 1.0f;
		float fSlow131 = 1.0f / fSlow130;
		float fSlow132 = fConst74 * (fConst74 - fSlow129) + 1.0f;
		float fSlow133 = fConst74 / fSlow130;
		float fSlow134 = 0.0f - fSlow133;
		float fSlow135 = fConst77 * (fConst77 + fSlow69) + 1.0f;
		float fSlow136 = 1.0f / fSlow135;
		float fSlow137 = fConst77 * (fConst77 - fSlow69) + 1.0f;
		float fSlow138 = fConst77 / fSlow135;
		float fSlow139 = 0.0f - fSlow138;
		float fSlow140 = fConst80 * (fConst80 + fSlow103) + 1.0f;
		float fSlow141 = 1.0f / fSlow140;
		float fSlow142 = fConst80 * (fConst80 - fSlow103) + 1.0f;
		float fSlow143 = fConst80 / fSlow140;
		float fSlow144 = 0.0f - fSlow143;
		float fSlow145 = 0.177122265f * fSlow0;
		float fSlow146 = fConst83 * (fConst83 + fSlow145) + 1.0f;
		float fSlow147 = 1.0f / fSlow146;
		float fSlow148 = fConst83 * (fConst83 - fSlow145) + 1.0f;
		float fSlow149 = fConst83 / fSlow146;
		float fSlow150 = 0.0f - fSlow149;
		float fSlow151 = fConst86 * (fConst86 + fSlow75) + 1.0f;
		float fSlow152 = 1.0f / fSlow151;
		float fSlow153 = fConst86 * (fConst86 - fSlow75) + 1.0f;
		float fSlow154 = fConst86 / fSlow151;
		float fSlow155 = 0.0f - fSlow154;
		float fSlow156 = fConst89 * (fConst89 + fSlow32) + 1.0f;
		float fSlow157 = 1.0f / fSlow156;
		float fSlow158 = fConst89 * (fConst89 - fSlow32) + 1.0f;
		float fSlow159 = fConst89 / fSlow156;
		float fSlow160 = 0.0f - fSlow159;
		float fSlow161 = fConst92 * (fConst92 + fSlow9) + 1.0f;
		float fSlow162 = 1.0f / fSlow161;
		float fSlow163 = fConst92 * (fConst92 - fSlow9) + 1.0f;
		float fSlow164 = fConst92 / fSlow161;
		float fSlow165 = 0.0f - fSlow164;
		float fSlow166 = fConst95 * (fConst95 + fSlow69) + 1.0f;
		float fSlow167 = 1.0f / fSlow166;
		float fSlow168 = fConst95 * (fConst95 - fSlow69) + 1.0f;
		float fSlow169 = fConst95 / fSlow166;
		float fSlow170 = 0.0f - fSlow169;
		float fSlow171 = fConst98 * (fConst98 + fSlow9) + 1.0f;
		float fSlow172 = fConst98 / fSlow171;
		float fSlow173 = 1.0f / fSlow171;
		float fSlow174 = fConst98 * (fConst98 - fSlow9) + 1.0f;
		float fSlow175 = 0.0f - fSlow172;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input1[i0]);
			fRec0[0] = fTemp0 - fSlow3 * (fSlow4 * fRec0[2] + fConst3 * fRec0[1]);
			float fTemp1 = float(input0[i0]);
			fRec2[0] = fTemp1 - fSlow3 * (fSlow4 * fRec2[2] + fConst3 * fRec2[1]);
			float fTemp2 = std::fabs(fSlow5 * fRec2[0] + fSlow6 * fRec2[2]);
			fRec3[0] = fSlow7 + fConst5 * fRec3[1];
			int iTemp3 = std::fabs(fRec3[0]) < 1.1920929e-07f;
			float fThen1 = std::exp(0.0f - fConst6 / ((iTemp3) ? 1.0f : fRec3[0]));
			float fTemp4 = ((iTemp3) ? 0.0f : fThen1);
			fRec4[0] = fSlow8 + fConst5 * fRec4[1];
			int iTemp5 = std::fabs(fRec4[0]) < 1.1920929e-07f;
			float fThen3 = std::exp(0.0f - fConst6 / ((iTemp5) ? 1.0f : fRec4[0]));
			float fTemp6 = ((iTemp5) ? 0.0f : fThen3);
			float fTemp7 = ((fTemp2 > fRec1[1]) ? fTemp6 : fTemp4);
			fRec1[0] = fTemp2 * (1.0f - fTemp7) + fRec1[1] * fTemp7;
			fRec5[0] = fTemp0 - fSlow11 * (fSlow12 * fRec5[2] + fConst9 * fRec5[1]);
			fRec7[0] = fTemp1 - fSlow11 * (fSlow12 * fRec7[2] + fConst9 * fRec7[1]);
			float fTemp8 = std::fabs(fSlow13 * fRec7[0] + fSlow14 * fRec7[2]);
			float fTemp9 = ((fTemp8 > fRec6[1]) ? fTemp6 : fTemp4);
			fRec6[0] = fTemp8 * (1.0f - fTemp9) + fRec6[1] * fTemp9;
			fRec8[0] = fTemp0 - fSlow17 * (fSlow18 * fRec8[2] + fConst12 * fRec8[1]);
			fRec10[0] = fTemp1 - fSlow17 * (fSlow18 * fRec10[2] + fConst12 * fRec10[1]);
			float fTemp10 = std::fabs(fSlow19 * fRec10[0] + fSlow20 * fRec10[2]);
			float fTemp11 = ((fTemp10 > fRec9[1]) ? fTemp6 : fTemp4);
			fRec9[0] = fTemp10 * (1.0f - fTemp11) + fRec9[1] * fTemp11;
			fRec11[0] = fTemp0 - fSlow23 * (fSlow24 * fRec11[2] + fConst15 * fRec11[1]);
			fRec13[0] = fTemp1 - fSlow23 * (fSlow24 * fRec13[2] + fConst15 * fRec13[1]);
			float fTemp12 = std::fabs(fSlow25 * fRec13[0] + fSlow26 * fRec13[2]);
			float fTemp13 = ((fTemp12 > fRec12[1]) ? fTemp6 : fTemp4);
			fRec12[0] = fTemp12 * (1.0f - fTemp13) + fRec12[1] * fTemp13;
			fRec14[0] = fTemp0 - fSlow28 * (fSlow29 * fRec14[2] + fConst18 * fRec14[1]);
			fRec16[0] = fTemp1 - fSlow28 * (fSlow29 * fRec16[2] + fConst18 * fRec16[1]);
			float fTemp14 = std::fabs(fSlow30 * fRec16[0] + fSlow31 * fRec16[2]);
			float fTemp15 = ((fTemp14 > fRec15[1]) ? fTemp6 : fTemp4);
			fRec15[0] = fTemp14 * (1.0f - fTemp15) + fRec15[1] * fTemp15;
			fRec17[0] = fTemp0 - fSlow34 * (fSlow35 * fRec17[2] + fConst21 * fRec17[1]);
			fRec19[0] = fTemp1 - fSlow34 * (fSlow35 * fRec19[2] + fConst21 * fRec19[1]);
			float fTemp16 = std::fabs(fSlow36 * fRec19[0] + fSlow37 * fRec19[2]);
			float fTemp17 = ((fTemp16 > fRec18[1]) ? fTemp6 : fTemp4);
			fRec18[0] = fTemp16 * (1.0f - fTemp17) + fRec18[1] * fTemp17;
			fRec20[0] = fTemp0 - fSlow39 * (fSlow40 * fRec20[2] + fConst24 * fRec20[1]);
			fRec22[0] = fTemp1 - fSlow39 * (fSlow40 * fRec22[2] + fConst24 * fRec22[1]);
			float fTemp18 = std::fabs(fSlow41 * fRec22[0] + fSlow42 * fRec22[2]);
			float fTemp19 = ((fTemp18 > fRec21[1]) ? fTemp6 : fTemp4);
			fRec21[0] = fTemp18 * (1.0f - fTemp19) + fRec21[1] * fTemp19;
			fRec23[0] = fTemp0 - fSlow44 * (fSlow45 * fRec23[2] + fConst27 * fRec23[1]);
			fRec25[0] = fTemp1 - fSlow44 * (fSlow45 * fRec25[2] + fConst27 * fRec25[1]);
			float fTemp20 = std::fabs(fSlow46 * fRec25[0] + fSlow47 * fRec25[2]);
			float fTemp21 = ((fTemp20 > fRec24[1]) ? fTemp6 : fTemp4);
			fRec24[0] = fTemp20 * (1.0f - fTemp21) + fRec24[1] * fTemp21;
			fRec26[0] = fTemp0 - fSlow49 * (fSlow50 * fRec26[2] + fConst30 * fRec26[1]);
			fRec28[0] = fTemp1 - fSlow49 * (fSlow50 * fRec28[2] + fConst30 * fRec28[1]);
			float fTemp22 = std::fabs(fSlow51 * fRec28[0] + fSlow52 * fRec28[2]);
			float fTemp23 = ((fTemp22 > fRec27[1]) ? fTemp6 : fTemp4);
			fRec27[0] = fTemp22 * (1.0f - fTemp23) + fRec27[1] * fTemp23;
			fRec29[0] = fTemp0 - fSlow54 * (fSlow55 * fRec29[2] + fConst33 * fRec29[1]);
			fRec31[0] = fTemp1 - fSlow54 * (fSlow55 * fRec31[2] + fConst33 * fRec31[1]);
			float fTemp24 = std::fabs(fSlow56 * fRec31[0] + fSlow57 * fRec31[2]);
			float fTemp25 = ((fTemp24 > fRec30[1]) ? fTemp6 : fTemp4);
			fRec30[0] = fTemp24 * (1.0f - fTemp25) + fRec30[1] * fTemp25;
			fRec32[0] = fTemp0 - fSlow60 * (fSlow61 * fRec32[2] + fConst36 * fRec32[1]);
			fRec34[0] = fTemp1 - fSlow60 * (fSlow61 * fRec34[2] + fConst36 * fRec34[1]);
			float fTemp26 = std::fabs(fSlow62 * fRec34[0] + fSlow63 * fRec34[2]);
			float fTemp27 = ((fTemp26 > fRec33[1]) ? fTemp6 : fTemp4);
			fRec33[0] = fTemp26 * (1.0f - fTemp27) + fRec33[1] * fTemp27;
			fRec35[0] = fTemp0 - fSlow65 * (fSlow66 * fRec35[2] + fConst39 * fRec35[1]);
			fRec37[0] = fTemp1 - fSlow65 * (fSlow66 * fRec37[2] + fConst39 * fRec37[1]);
			float fTemp28 = std::fabs(fSlow67 * fRec37[0] + fSlow68 * fRec37[2]);
			float fTemp29 = ((fTemp28 > fRec36[1]) ? fTemp6 : fTemp4);
			fRec36[0] = fTemp28 * (1.0f - fTemp29) + fRec36[1] * fTemp29;
			fRec38[0] = fTemp0 - fSlow71 * (fSlow72 * fRec38[2] + fConst42 * fRec38[1]);
			fRec40[0] = fTemp1 - fSlow71 * (fSlow72 * fRec40[2] + fConst42 * fRec40[1]);
			float fTemp30 = std::fabs(fSlow73 * fRec40[0] + fSlow74 * fRec40[2]);
			float fTemp31 = ((fTemp30 > fRec39[1]) ? fTemp6 : fTemp4);
			fRec39[0] = fTemp30 * (1.0f - fTemp31) + fRec39[1] * fTemp31;
			fRec41[0] = fTemp0 - fSlow77 * (fSlow78 * fRec41[2] + fConst45 * fRec41[1]);
			fRec43[0] = fTemp1 - fSlow77 * (fSlow78 * fRec43[2] + fConst45 * fRec43[1]);
			float fTemp32 = std::fabs(fSlow79 * fRec43[0] + fSlow80 * fRec43[2]);
			float fTemp33 = ((fTemp32 > fRec42[1]) ? fTemp6 : fTemp4);
			fRec42[0] = fTemp32 * (1.0f - fTemp33) + fRec42[1] * fTemp33;
			fRec44[0] = fTemp0 - fSlow82 * (fSlow83 * fRec44[2] + fConst48 * fRec44[1]);
			fRec46[0] = fTemp1 - fSlow82 * (fSlow83 * fRec46[2] + fConst48 * fRec46[1]);
			float fTemp34 = std::fabs(fSlow84 * fRec46[0] + fSlow85 * fRec46[2]);
			float fTemp35 = ((fTemp34 > fRec45[1]) ? fTemp6 : fTemp4);
			fRec45[0] = fTemp34 * (1.0f - fTemp35) + fRec45[1] * fTemp35;
			fRec47[0] = fTemp0 - fSlow88 * (fSlow89 * fRec47[2] + fConst51 * fRec47[1]);
			fRec49[0] = fTemp1 - fSlow88 * (fSlow89 * fRec49[2] + fConst51 * fRec49[1]);
			float fTemp36 = std::fabs(fSlow90 * fRec49[0] + fSlow91 * fRec49[2]);
			float fTemp37 = ((fTemp36 > fRec48[1]) ? fTemp6 : fTemp4);
			fRec48[0] = fTemp36 * (1.0f - fTemp37) + fRec48[1] * fTemp37;
			fRec50[0] = fTemp0 - fSlow94 * (fSlow95 * fRec50[2] + fConst54 * fRec50[1]);
			fRec52[0] = fTemp1 - fSlow94 * (fSlow95 * fRec52[2] + fConst54 * fRec52[1]);
			float fTemp38 = std::fabs(fSlow96 * fRec52[0] + fSlow97 * fRec52[2]);
			float fTemp39 = ((fTemp38 > fRec51[1]) ? fTemp6 : fTemp4);
			fRec51[0] = fTemp38 * (1.0f - fTemp39) + fRec51[1] * fTemp39;
			fRec53[0] = fTemp0 - fSlow99 * (fSlow100 * fRec53[2] + fConst57 * fRec53[1]);
			fRec55[0] = fTemp1 - fSlow99 * (fSlow100 * fRec55[2] + fConst57 * fRec55[1]);
			float fTemp40 = std::fabs(fSlow101 * fRec55[0] + fSlow102 * fRec55[2]);
			float fTemp41 = ((fTemp40 > fRec54[1]) ? fTemp6 : fTemp4);
			fRec54[0] = fTemp40 * (1.0f - fTemp41) + fRec54[1] * fTemp41;
			fRec56[0] = fTemp0 - fSlow105 * (fSlow106 * fRec56[2] + fConst60 * fRec56[1]);
			fRec58[0] = fTemp1 - fSlow105 * (fSlow106 * fRec58[2] + fConst60 * fRec58[1]);
			float fTemp42 = std::fabs(fSlow107 * fRec58[0] + fSlow108 * fRec58[2]);
			float fTemp43 = ((fTemp42 > fRec57[1]) ? fTemp6 : fTemp4);
			fRec57[0] = fTemp42 * (1.0f - fTemp43) + fRec57[1] * fTemp43;
			fRec59[0] = fTemp0 - fSlow110 * (fSlow111 * fRec59[2] + fConst63 * fRec59[1]);
			fRec61[0] = fTemp1 - fSlow110 * (fSlow111 * fRec61[2] + fConst63 * fRec61[1]);
			float fTemp44 = std::fabs(fSlow112 * fRec61[0] + fSlow113 * fRec61[2]);
			float fTemp45 = ((fTemp44 > fRec60[1]) ? fTemp6 : fTemp4);
			fRec60[0] = fTemp44 * (1.0f - fTemp45) + fRec60[1] * fTemp45;
			fRec62[0] = fTemp0 - fSlow115 * (fSlow116 * fRec62[2] + fConst66 * fRec62[1]);
			fRec64[0] = fTemp1 - fSlow115 * (fSlow116 * fRec64[2] + fConst66 * fRec64[1]);
			float fTemp46 = std::fabs(fSlow117 * fRec64[0] + fSlow118 * fRec64[2]);
			float fTemp47 = ((fTemp46 > fRec63[1]) ? fTemp6 : fTemp4);
			fRec63[0] = fTemp46 * (1.0f - fTemp47) + fRec63[1] * fTemp47;
			fRec65[0] = fTemp0 - fSlow120 * (fSlow121 * fRec65[2] + fConst69 * fRec65[1]);
			fRec67[0] = fTemp1 - fSlow120 * (fSlow121 * fRec67[2] + fConst69 * fRec67[1]);
			float fTemp48 = std::fabs(fSlow122 * fRec67[0] + fSlow123 * fRec67[2]);
			float fTemp49 = ((fTemp48 > fRec66[1]) ? fTemp6 : fTemp4);
			fRec66[0] = fTemp48 * (1.0f - fTemp49) + fRec66[1] * fTemp49;
			fRec68[0] = fTemp0 - fSlow125 * (fSlow126 * fRec68[2] + fConst72 * fRec68[1]);
			fRec70[0] = fTemp1 - fSlow125 * (fSlow126 * fRec70[2] + fConst72 * fRec70[1]);
			float fTemp50 = std::fabs(fSlow127 * fRec70[0] + fSlow128 * fRec70[2]);
			float fTemp51 = ((fTemp50 > fRec69[1]) ? fTemp6 : fTemp4);
			fRec69[0] = fTemp50 * (1.0f - fTemp51) + fRec69[1] * fTemp51;
			fRec71[0] = fTemp0 - fSlow131 * (fSlow132 * fRec71[2] + fConst75 * fRec71[1]);
			fRec73[0] = fTemp1 - fSlow131 * (fSlow132 * fRec73[2] + fConst75 * fRec73[1]);
			float fTemp52 = std::fabs(fSlow133 * fRec73[0] + fSlow134 * fRec73[2]);
			float fTemp53 = ((fTemp52 > fRec72[1]) ? fTemp6 : fTemp4);
			fRec72[0] = fTemp52 * (1.0f - fTemp53) + fRec72[1] * fTemp53;
			fRec74[0] = fTemp0 - fSlow136 * (fSlow137 * fRec74[2] + fConst78 * fRec74[1]);
			fRec76[0] = fTemp1 - fSlow136 * (fSlow137 * fRec76[2] + fConst78 * fRec76[1]);
			float fTemp54 = std::fabs(fSlow138 * fRec76[0] + fSlow139 * fRec76[2]);
			float fTemp55 = ((fTemp54 > fRec75[1]) ? fTemp6 : fTemp4);
			fRec75[0] = fTemp54 * (1.0f - fTemp55) + fRec75[1] * fTemp55;
			fRec77[0] = fTemp0 - fSlow141 * (fSlow142 * fRec77[2] + fConst81 * fRec77[1]);
			fRec79[0] = fTemp1 - fSlow141 * (fSlow142 * fRec79[2] + fConst81 * fRec79[1]);
			float fTemp56 = std::fabs(fSlow143 * fRec79[0] + fSlow144 * fRec79[2]);
			float fTemp57 = ((fTemp56 > fRec78[1]) ? fTemp6 : fTemp4);
			fRec78[0] = fTemp56 * (1.0f - fTemp57) + fRec78[1] * fTemp57;
			fRec80[0] = fTemp0 - fSlow147 * (fSlow148 * fRec80[2] + fConst84 * fRec80[1]);
			fRec82[0] = fTemp1 - fSlow147 * (fSlow148 * fRec82[2] + fConst84 * fRec82[1]);
			float fTemp58 = std::fabs(fSlow149 * fRec82[0] + fSlow150 * fRec82[2]);
			float fTemp59 = ((fTemp58 > fRec81[1]) ? fTemp6 : fTemp4);
			fRec81[0] = fTemp58 * (1.0f - fTemp59) + fRec81[1] * fTemp59;
			fRec83[0] = fTemp0 - fSlow152 * (fSlow153 * fRec83[2] + fConst87 * fRec83[1]);
			fRec85[0] = fTemp1 - fSlow152 * (fSlow153 * fRec85[2] + fConst87 * fRec85[1]);
			float fTemp60 = std::fabs(fSlow154 * fRec85[0] + fSlow155 * fRec85[2]);
			float fTemp61 = ((fTemp60 > fRec84[1]) ? fTemp6 : fTemp4);
			fRec84[0] = fTemp60 * (1.0f - fTemp61) + fRec84[1] * fTemp61;
			fRec86[0] = fTemp0 - fSlow157 * (fSlow158 * fRec86[2] + fConst90 * fRec86[1]);
			fRec88[0] = fTemp1 - fSlow157 * (fSlow158 * fRec88[2] + fConst90 * fRec88[1]);
			float fTemp62 = std::fabs(fSlow159 * fRec88[0] + fSlow160 * fRec88[2]);
			float fTemp63 = ((fTemp62 > fRec87[1]) ? fTemp6 : fTemp4);
			fRec87[0] = fTemp62 * (1.0f - fTemp63) + fRec87[1] * fTemp63;
			fRec89[0] = fTemp0 - fSlow162 * (fSlow163 * fRec89[2] + fConst93 * fRec89[1]);
			fRec91[0] = fTemp1 - fSlow162 * (fSlow163 * fRec91[2] + fConst93 * fRec91[1]);
			float fTemp64 = std::fabs(fSlow164 * fRec91[0] + fSlow165 * fRec91[2]);
			float fTemp65 = ((fTemp64 > fRec90[1]) ? fTemp6 : fTemp4);
			fRec90[0] = fTemp64 * (1.0f - fTemp65) + fRec90[1] * fTemp65;
			fRec92[0] = fTemp0 - fSlow167 * (fSlow168 * fRec92[2] + fConst96 * fRec92[1]);
			fRec94[0] = fTemp1 - fSlow167 * (fSlow168 * fRec94[2] + fConst96 * fRec94[1]);
			float fTemp66 = std::fabs(fSlow169 * fRec94[0] + fSlow170 * fRec94[2]);
			float fTemp67 = ((fTemp66 > fRec93[1]) ? fTemp6 : fTemp4);
			fRec93[0] = fTemp66 * (1.0f - fTemp67) + fRec93[1] * fTemp67;
			fRec95[0] = fTemp0 - fSlow173 * (fSlow174 * fRec95[2] + fConst99 * fRec95[1]);
			fRec97[0] = fTemp1 - fSlow173 * (fSlow174 * fRec97[2] + fConst99 * fRec97[1]);
			float fTemp68 = std::fabs(fSlow172 * fRec97[0] + fSlow175 * fRec97[2]);
			float fTemp69 = ((fTemp68 > fRec96[1]) ? fTemp6 : fTemp4);
			fRec96[0] = fTemp68 * (1.0f - fTemp69) + fRec96[1] * fTemp69;
			output0[i0] = FAUSTFLOAT(fRec0[2] * (0.0f - fSlow5 * fRec1[0]) + fSlow5 * fRec0[0] * fRec1[0] + fRec5[2] * (0.0f - fSlow13 * fRec6[0]) + fSlow13 * fRec5[0] * fRec6[0] + fRec8[2] * (0.0f - fSlow19 * fRec9[0]) + fSlow19 * fRec8[0] * fRec9[0] + fRec11[2] * (0.0f - fSlow25 * fRec12[0]) + fSlow25 * fRec11[0] * fRec12[0] + fRec14[2] * (0.0f - fSlow30 * fRec15[0]) + fSlow30 * fRec14[0] * fRec15[0] + fRec17[2] * (0.0f - fSlow36 * fRec18[0]) + fSlow36 * fRec17[0] * fRec18[0] + fRec20[2] * (0.0f - fSlow41 * fRec21[0]) + fSlow41 * fRec20[0] * fRec21[0] + fRec23[2] * (0.0f - fSlow46 * fRec24[0]) + fSlow46 * fRec23[0] * fRec24[0] + fRec26[2] * (0.0f - fSlow51 * fRec27[0]) + fSlow51 * fRec26[0] * fRec27[0] + fRec29[2] * (0.0f - fSlow56 * fRec30[0]) + fSlow56 * fRec29[0] * fRec30[0] + fRec32[2] * (0.0f - fSlow62 * fRec33[0]) + fSlow62 * fRec32[0] * fRec33[0] + fRec35[2] * (0.0f - fSlow67 * fRec36[0]) + fSlow67 * fRec35[0] * fRec36[0] + fRec38[2] * (0.0f - fSlow73 * fRec39[0]) + fSlow73 * fRec38[0] * fRec39[0] + fRec41[2] * (0.0f - fSlow79 * fRec42[0]) + fSlow79 * fRec41[0] * fRec42[0] + fRec44[2] * (0.0f - fSlow84 * fRec45[0]) + fSlow84 * fRec44[0] * fRec45[0] + fRec47[2] * (0.0f - fSlow90 * fRec48[0]) + fRec50[2] * (0.0f - fSlow96 * fRec51[0]) + fRec53[2] * (0.0f - fSlow101 * fRec54[0]) + fRec56[2] * (0.0f - fSlow107 * fRec57[0]) + fRec59[2] * (0.0f - fSlow112 * fRec60[0]) + fRec62[2] * (0.0f - fSlow117 * fRec63[0]) + fRec65[2] * (0.0f - fSlow122 * fRec66[0]) + fRec68[2] * (0.0f - fSlow127 * fRec69[0]) + fRec71[2] * (0.0f - fSlow133 * fRec72[0]) + fRec74[2] * (0.0f - fSlow138 * fRec75[0]) + fRec77[2] * (0.0f - fSlow143 * fRec78[0]) + fRec80[2] * (0.0f - fSlow149 * fRec81[0]) + fRec83[2] * (0.0f - fSlow154 * fRec84[0]) + fRec86[2] * (0.0f - fSlow159 * fRec87[0]) + fRec89[2] * (0.0f - fSlow164 * fRec90[0]) + fRec92[2] * (0.0f - fSlow169 * fRec93[0]) + fSlow172 * fRec95[0] * fRec96[0] + fRec95[2] * (0.0f - fSlow172 * fRec96[0]) + fSlow169 * fRec92[0] * fRec93[0] + fSlow164 * fRec89[0] * fRec90[0] + fSlow159 * fRec86[0] * fRec87[0] + fSlow154 * fRec83[0] * fRec84[0] + fSlow149 * fRec80[0] * fRec81[0] + fSlow143 * fRec77[0] * fRec78[0] + fSlow138 * fRec74[0] * fRec75[0] + fSlow133 * fRec71[0] * fRec72[0] + fSlow127 * fRec68[0] * fRec69[0] + fSlow122 * fRec65[0] * fRec66[0] + fSlow117 * fRec62[0] * fRec63[0] + fSlow112 * fRec59[0] * fRec60[0] + fSlow107 * fRec56[0] * fRec57[0] + fSlow101 * fRec53[0] * fRec54[0] + fSlow96 * fRec50[0] * fRec51[0] + fSlow90 * fRec47[0] * fRec48[0]);
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec1[1] = fRec1[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[1] = fRec6[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[1] = fRec9[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[1] = fRec15[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec21[1] = fRec21[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec45[1] = fRec45[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec57[1] = fRec57[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fRec63[1] = fRec63[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			fRec68[2] = fRec68[1];
			fRec68[1] = fRec68[0];
			fRec70[2] = fRec70[1];
			fRec70[1] = fRec70[0];
			fRec69[1] = fRec69[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec76[2] = fRec76[1];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec81[1] = fRec81[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fRec84[1] = fRec84[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec87[1] = fRec87[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fRec90[1] = fRec90[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec94[2] = fRec94[1];
			fRec94[1] = fRec94[0];
			fRec93[1] = fRec93[0];
			fRec95[2] = fRec95[1];
			fRec95[1] = fRec95[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec96[1] = fRec96[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _fx_vocoder_UI : public UI {
    static std::string name;
};

template <class T>
std::string _fx_vocoder_UI<T>::name(sym(fx_vocoder));

typedef _fx_vocoder_UI<fx_vocoder> fx_vocoder_UI;

class faust_fx_vocoder_tilde : public FaustExternal<fx_vocoder, fx_vocoder_UI> {
public:
    faust_fx_vocoder_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
