//----------------------------------------------------------
// name: "synth.dx7"
//
// Code generated with Faust 2.53.1 (https://faust.grame.fr)
//----------------------------------------------------------

/* link with  */
#include <math.h>
// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_dx7_dsp.h ********************************
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

class FAUST_API synth_dx7_dsp {

    public:

        synth_dx7_dsp() {}
        virtual ~synth_dx7_dsp() {}

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
        virtual synth_dx7_dsp* clone() = 0;
    
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

class FAUST_API decorator_dsp : public synth_dx7_dsp {

    protected:

        synth_dx7_dsp* fDSP;

    public:

        decorator_dsp(synth_dx7_dsp* synth_dx7_dsp = nullptr):fDSP(synth_dx7_dsp) {}
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
    
        virtual synth_dx7_dsp* createDSPInstance() = 0;
    
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

/************************** END synth_dx7_dsp.h **************************/
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
 The base class of Meta handler to be used in synth_dx7_dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
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
struct synth_dx7 : public synth_dx7_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 


#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_dx7
#endif

class synth_dx7 : public synth_dx7_dsp {
  private:
	class SIG0 {
	  private:
		int fSampleRate;
		int 	iVec0[2];
		int 	iRec3[2];
	  public:
		int getNumInputs() { return 0; }
		int getNumOutputs() { return 1; }
		void init(int sample_rate) {
			fSampleRate = sample_rate;
			for (int i=0; i<2; i++) iVec0[i] = 0;
			for (int i=0; i<2; i++) iRec3[i] = 0;
		}
		void fill(int count, float output[]) {
			for (int i=0; i<count; i++) {
				iVec0[0] = 1;
				iRec3[0] = (((iVec0[1] + iRec3[1])) % 65536);
				output[i] = sinf((9.58738e-05f * float(iRec3[0])));
				// post processing
				iRec3[1] = iRec3[0];
				iVec0[1] = iVec0[0];
			}
		}
	};


	FAUSTFLOAT 	fentry0;
	FAUSTFLOAT 	fbutton0;
	float 	fConst0;
	FAUSTFLOAT 	fentry1;
	FAUSTFLOAT 	fentry2;
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT 	fentry3;
	FAUSTFLOAT 	fentry4;
	FAUSTFLOAT 	fentry5;
	FAUSTFLOAT 	fentry6;
	FAUSTFLOAT 	fentry7;
	FAUSTFLOAT 	fentry8;
	FAUSTFLOAT 	fentry9;
	FAUSTFLOAT 	fslider1;
	float 	fRec0[2];
	FAUSTFLOAT 	fentry10;
	FAUSTFLOAT 	fentry11;
	FAUSTFLOAT 	fentry12;
	FAUSTFLOAT 	fentry13;
	FAUSTFLOAT 	fentry14;
	float 	fRec2[2];
	int 	iTempPerm0;
	int 	iTempPerm1;
	float 	fConst1;
	float 	fRec1[2];
	float 	fTempPerm2;
	static float 	ftbl0[65536];
	float 	fTempPerm3;
	float 	fRec4[2];
	FAUSTFLOAT 	fentry15;
	FAUSTFLOAT 	fentry16;
	FAUSTFLOAT 	fentry17;
	FAUSTFLOAT 	fentry18;
	FAUSTFLOAT 	fentry19;
	FAUSTFLOAT 	fentry20;
	FAUSTFLOAT 	fentry21;
	FAUSTFLOAT 	fentry22;
	FAUSTFLOAT 	fentry23;
	float 	fRec5[2];
	FAUSTFLOAT 	fentry24;
	FAUSTFLOAT 	fentry25;
	FAUSTFLOAT 	fentry26;
	FAUSTFLOAT 	fentry27;
	FAUSTFLOAT 	fentry28;
	float 	fRec7[2];
	int 	iTempPerm4;
	int 	iTempPerm5;
	float 	fRec6[2];
	float 	fTempPerm6;
	float 	fTempPerm7;
	float 	fRec8[2];
	FAUSTFLOAT 	fentry29;
	FAUSTFLOAT 	fentry30;
	FAUSTFLOAT 	fentry31;
	FAUSTFLOAT 	fentry32;
	FAUSTFLOAT 	fentry33;
	FAUSTFLOAT 	fentry34;
	FAUSTFLOAT 	fentry35;
	FAUSTFLOAT 	fentry36;
	FAUSTFLOAT 	fentry37;
	float 	fRec9[2];
	FAUSTFLOAT 	fentry38;
	FAUSTFLOAT 	fentry39;
	FAUSTFLOAT 	fentry40;
	FAUSTFLOAT 	fentry41;
	FAUSTFLOAT 	fentry42;
	float 	fRec11[2];
	int 	iTempPerm8;
	int 	iTempPerm9;
	float 	fRec10[2];
	float 	fTempPerm10;
	float 	fTempPerm11;
	float 	fRec12[2];
	float 	fTempPerm12;
	float 	fTempPerm13;
	FAUSTFLOAT 	fentry43;
	FAUSTFLOAT 	fentry44;
	FAUSTFLOAT 	fentry45;
	FAUSTFLOAT 	fentry46;
	FAUSTFLOAT 	fentry47;
	FAUSTFLOAT 	fentry48;
	FAUSTFLOAT 	fentry49;
	FAUSTFLOAT 	fentry50;
	FAUSTFLOAT 	fentry51;
	float 	fRec13[2];
	FAUSTFLOAT 	fentry52;
	FAUSTFLOAT 	fentry53;
	FAUSTFLOAT 	fentry54;
	FAUSTFLOAT 	fentry55;
	FAUSTFLOAT 	fentry56;
	float 	fRec15[2];
	int 	iTempPerm14;
	int 	iTempPerm15;
	float 	fRec14[2];
	float 	fTempPerm16;
	float 	fTempPerm17;
	float 	fRec16[2];
	FAUSTFLOAT 	fentry57;
	FAUSTFLOAT 	fentry58;
	FAUSTFLOAT 	fentry59;
	FAUSTFLOAT 	fentry60;
	FAUSTFLOAT 	fentry61;
	FAUSTFLOAT 	fentry62;
	FAUSTFLOAT 	fentry63;
	FAUSTFLOAT 	fentry64;
	FAUSTFLOAT 	fentry65;
	float 	fRec18[2];
	FAUSTFLOAT 	fentry66;
	FAUSTFLOAT 	fentry67;
	FAUSTFLOAT 	fentry68;
	FAUSTFLOAT 	fentry69;
	FAUSTFLOAT 	fentry70;
	float 	fRec20[2];
	int 	iTempPerm18;
	int 	iTempPerm19;
	float 	fRec19[2];
	float 	fTempPerm20;
	float 	fTempPerm21;
	float 	fRec21[2];
	float 	fTempPerm22;
	FAUSTFLOAT 	fentry71;
	float 	fRec17[2];
	FAUSTFLOAT 	fentry72;
	FAUSTFLOAT 	fentry73;
	FAUSTFLOAT 	fentry74;
	FAUSTFLOAT 	fentry75;
	FAUSTFLOAT 	fentry76;
	FAUSTFLOAT 	fentry77;
	FAUSTFLOAT 	fentry78;
	FAUSTFLOAT 	fentry79;
	FAUSTFLOAT 	fentry80;
	float 	fRec22[2];
	FAUSTFLOAT 	fentry81;
	FAUSTFLOAT 	fentry82;
	FAUSTFLOAT 	fentry83;
	FAUSTFLOAT 	fentry84;
	FAUSTFLOAT 	fentry85;
	float 	fRec24[2];
	int 	iTempPerm23;
	int 	iTempPerm24;
	float 	fRec23[2];
	float 	fTempPerm25;
	float 	fTempPerm26;
	float 	fRec25[2];
	float 	fTempPerm27;
	float 	fTempPerm28;
	float 	fTempPerm29;
	float 	fRec26[2];
	float 	fTempPerm30;
	float 	fTempPerm31;
	float 	fTempPerm32;
	float 	fTempPerm33;
	float 	fRec27[2];
	float 	fTempPerm34;
	float 	fTempPerm35;
	float 	fTempPerm36;
	float 	fTempPerm37;
	float 	fTempPerm38;
	float 	fTempPerm39;
	float 	fTempPerm40;
	float 	fRec28[2];
	float 	fTempPerm41;
	float 	fRec29[2];
	float 	fTempPerm42;
	float 	fTempPerm43;
	float 	fRec30[2];
	float 	fTempPerm44;
	float 	fTempPerm45;
	float 	fTempPerm46;
	float 	fRec31[2];
	float 	fTempPerm47;
	float 	fRec32[2];
	float 	fTempPerm48;
	float 	fTempPerm49;
	float 	fRec33[2];
	float 	fTempPerm50;
	float 	fTempPerm51;
	float 	fTempPerm52;
	float 	fTempPerm53;
	float 	fTempPerm54;
	float 	fTempPerm55;
	float 	fTempPerm56;
	int fSampleRate;

  public:
	virtual void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "0.8");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/dx7envelope:author", "Romain Michon");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.2");
		m->declare("filename", "synth_dx7.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "synth.dx7");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.3");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("spn.lib/name", "Standart Pitch Notation constants");
		m->declare("spn.lib/version", "0.2");
	}

	virtual int getNumInputs() { return 0; }
	virtual int getNumOutputs() { return 1; }
	static void classInit(int sample_rate) {
		SIG0 sig0;
		sig0.init(sample_rate);
		sig0.fill(65536,ftbl0);
	}
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = min(1.92e+05f, max(1.0f, float(fSampleRate)));
		iTempPerm0 = 0;
		iTempPerm1 = 0;
		fConst1 = (1.0f / fConst0);
		fTempPerm2 = 0;
		fTempPerm3 = 0;
		iTempPerm4 = 0;
		iTempPerm5 = 0;
		fTempPerm6 = 0;
		fTempPerm7 = 0;
		iTempPerm8 = 0;
		iTempPerm9 = 0;
		fTempPerm10 = 0;
		fTempPerm11 = 0;
		fTempPerm12 = 0;
		fTempPerm13 = 0;
		iTempPerm14 = 0;
		iTempPerm15 = 0;
		fTempPerm16 = 0;
		fTempPerm17 = 0;
		iTempPerm18 = 0;
		iTempPerm19 = 0;
		fTempPerm20 = 0;
		fTempPerm21 = 0;
		fTempPerm22 = 0;
		iTempPerm23 = 0;
		iTempPerm24 = 0;
		fTempPerm25 = 0;
		fTempPerm26 = 0;
		fTempPerm27 = 0;
		fTempPerm28 = 0;
		fTempPerm29 = 0;
		fTempPerm30 = 0;
		fTempPerm31 = 0;
		fTempPerm32 = 0;
		fTempPerm33 = 0;
		fTempPerm34 = 0;
		fTempPerm35 = 0;
		fTempPerm36 = 0;
		fTempPerm37 = 0;
		fTempPerm38 = 0;
		fTempPerm39 = 0;
		fTempPerm40 = 0;
		fTempPerm41 = 0;
		fTempPerm42 = 0;
		fTempPerm43 = 0;
		fTempPerm44 = 0;
		fTempPerm45 = 0;
		fTempPerm46 = 0;
		fTempPerm47 = 0;
		fTempPerm48 = 0;
		fTempPerm49 = 0;
		fTempPerm50 = 0;
		fTempPerm51 = 0;
		fTempPerm52 = 0;
		fTempPerm53 = 0;
		fTempPerm54 = 0;
		fTempPerm55 = 0;
		fTempPerm56 = 0;
	}
	virtual void instanceResetUserInterface() {
		fentry0 = 0.0f;
		fbutton0 = 0.0;
		fentry1 = 95.0f;
		fentry2 = 9e+01f;
		fslider0 = 0.8f;
		fentry3 = 1.0f;
		fentry4 = 0.0f;
		fentry5 = 9e+01f;
		fentry6 = 0.0f;
		fentry7 = 1.0f;
		fentry8 = 0.0f;
		fentry9 = 1.0f;
		fslider1 = 48.0f;
		fentry10 = 0.0f;
		fentry11 = 9e+01f;
		fentry12 = 9e+01f;
		fentry13 = 9e+01f;
		fentry14 = 9e+01f;
		fentry15 = 95.0f;
		fentry16 = 9e+01f;
		fentry17 = 1.0f;
		fentry18 = 0.0f;
		fentry19 = 9e+01f;
		fentry20 = 0.0f;
		fentry21 = 1.0f;
		fentry22 = 0.0f;
		fentry23 = 1.0f;
		fentry24 = 0.0f;
		fentry25 = 9e+01f;
		fentry26 = 9e+01f;
		fentry27 = 9e+01f;
		fentry28 = 9e+01f;
		fentry29 = 95.0f;
		fentry30 = 9e+01f;
		fentry31 = 1.0f;
		fentry32 = 0.0f;
		fentry33 = 9e+01f;
		fentry34 = 0.0f;
		fentry35 = 1.0f;
		fentry36 = 0.0f;
		fentry37 = 1.0f;
		fentry38 = 0.0f;
		fentry39 = 9e+01f;
		fentry40 = 9e+01f;
		fentry41 = 9e+01f;
		fentry42 = 9e+01f;
		fentry43 = 95.0f;
		fentry44 = 9e+01f;
		fentry45 = 1.0f;
		fentry46 = 0.0f;
		fentry47 = 9e+01f;
		fentry48 = 0.0f;
		fentry49 = 1.0f;
		fentry50 = 0.0f;
		fentry51 = 1.0f;
		fentry52 = 0.0f;
		fentry53 = 9e+01f;
		fentry54 = 9e+01f;
		fentry55 = 9e+01f;
		fentry56 = 9e+01f;
		fentry57 = 95.0f;
		fentry58 = 9e+01f;
		fentry59 = 1.0f;
		fentry60 = 0.0f;
		fentry61 = 9e+01f;
		fentry62 = 0.0f;
		fentry63 = 1.0f;
		fentry64 = 0.0f;
		fentry65 = 1.0f;
		fentry66 = 0.0f;
		fentry67 = 9e+01f;
		fentry68 = 9e+01f;
		fentry69 = 9e+01f;
		fentry70 = 9e+01f;
		fentry71 = 0.0f;
		fentry72 = 95.0f;
		fentry73 = 9e+01f;
		fentry74 = 1.0f;
		fentry75 = 0.0f;
		fentry76 = 9e+01f;
		fentry77 = 0.0f;
		fentry78 = 1.0f;
		fentry79 = 0.0f;
		fentry80 = 1.0f;
		fentry81 = 0.0f;
		fentry82 = 9e+01f;
		fentry83 = 9e+01f;
		fentry84 = 9e+01f;
		fentry85 = 9e+01f;
	}
	virtual void instanceClear() {
		for (int i=0; i<2; i++) fRec0[i] = 0;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		for (int i=0; i<2; i++) fRec1[i] = 0;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		for (int i=0; i<2; i++) fRec5[i] = 0;
		for (int i=0; i<2; i++) fRec7[i] = 0;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		for (int i=0; i<2; i++) fRec8[i] = 0;
		for (int i=0; i<2; i++) fRec9[i] = 0;
		for (int i=0; i<2; i++) fRec11[i] = 0;
		for (int i=0; i<2; i++) fRec10[i] = 0;
		for (int i=0; i<2; i++) fRec12[i] = 0;
		for (int i=0; i<2; i++) fRec13[i] = 0;
		for (int i=0; i<2; i++) fRec15[i] = 0;
		for (int i=0; i<2; i++) fRec14[i] = 0;
		for (int i=0; i<2; i++) fRec16[i] = 0;
		for (int i=0; i<2; i++) fRec18[i] = 0;
		for (int i=0; i<2; i++) fRec20[i] = 0;
		for (int i=0; i<2; i++) fRec19[i] = 0;
		for (int i=0; i<2; i++) fRec21[i] = 0;
		for (int i=0; i<2; i++) fRec17[i] = 0;
		for (int i=0; i<2; i++) fRec22[i] = 0;
		for (int i=0; i<2; i++) fRec24[i] = 0;
		for (int i=0; i<2; i++) fRec23[i] = 0;
		for (int i=0; i<2; i++) fRec25[i] = 0;
		for (int i=0; i<2; i++) fRec26[i] = 0;
		for (int i=0; i<2; i++) fRec27[i] = 0;
		for (int i=0; i<2; i++) fRec28[i] = 0;
		for (int i=0; i<2; i++) fRec29[i] = 0;
		for (int i=0; i<2; i++) fRec30[i] = 0;
		for (int i=0; i<2; i++) fRec31[i] = 0;
		for (int i=0; i<2; i++) fRec32[i] = 0;
		for (int i=0; i<2; i++) fRec33[i] = 0;
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
	virtual synth_dx7* clone() {
		return new synth_dx7();
	}
	virtual int getSampleRate() {
		return fSampleRate;
	}
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.dx7");
		ui_interface->declare(&fentry0, "type", "int");
		ui_interface->addNumEntry("algorithm", &fentry0, 0.0f, 0.0f, 15.0f, 1.0f);
		ui_interface->addNumEntry("feedback", &fentry71, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addHorizontalSlider("gain", &fslider0, 0.8f, 0.0f, 1.0f, 0.01f);
		ui_interface->addButton("gate", &fbutton0);
		ui_interface->addNumEntry("op0:detune", &fentry7, 1.0f, -1e+01f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op0:egL1", &fentry10, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL2", &fentry12, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL3", &fentry2, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL4", &fentry4, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR1", &fentry11, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR2", &fentry13, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR3", &fentry14, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR4", &fentry5, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry9, "unit", "Hz");
		ui_interface->addNumEntry("op0:freq", &fentry9, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op0:key_vel_sens", &fentry3, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op0:level", &fentry1, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:mode", &fentry8, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op0:rate_scale", &fentry6, 0.0f, 0.0f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op1:detune", &fentry21, 1.0f, -1e+01f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op1:egL1", &fentry24, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL2", &fentry26, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL3", &fentry16, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL4", &fentry18, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR1", &fentry25, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR2", &fentry27, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR3", &fentry28, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR4", &fentry19, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry23, "unit", "Hz");
		ui_interface->addNumEntry("op1:freq", &fentry23, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op1:key_vel_sens", &fentry17, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op1:level", &fentry15, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:mode", &fentry22, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op1:rate_scale", &fentry20, 0.0f, 0.0f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op2:detune", &fentry35, 1.0f, -1e+01f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op2:egL1", &fentry38, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL2", &fentry40, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL3", &fentry30, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL4", &fentry32, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR1", &fentry39, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR2", &fentry41, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR3", &fentry42, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR4", &fentry33, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry37, "unit", "Hz");
		ui_interface->addNumEntry("op2:freq", &fentry37, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op2:key_vel_sens", &fentry31, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op2:level", &fentry29, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:mode", &fentry36, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op2:rate_scale", &fentry34, 0.0f, 0.0f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op3:detune", &fentry49, 1.0f, -1e+01f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op3:egL1", &fentry52, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL2", &fentry54, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL3", &fentry44, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL4", &fentry46, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR1", &fentry53, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR2", &fentry55, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR3", &fentry56, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR4", &fentry47, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry51, "unit", "Hz");
		ui_interface->addNumEntry("op3:freq", &fentry51, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op3:key_vel_sens", &fentry45, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op3:level", &fentry43, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:mode", &fentry50, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op3:rate_scale", &fentry48, 0.0f, 0.0f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op4:detune", &fentry78, 1.0f, -1e+01f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op4:egL1", &fentry81, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL2", &fentry83, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL3", &fentry73, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL4", &fentry75, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR1", &fentry82, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR2", &fentry84, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR3", &fentry85, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR4", &fentry76, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry80, "unit", "Hz");
		ui_interface->addNumEntry("op4:freq", &fentry80, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op4:key_vel_sens", &fentry74, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op4:level", &fentry72, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:mode", &fentry79, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op4:rate_scale", &fentry77, 0.0f, 0.0f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op5:detune", &fentry63, 1.0f, -1e+01f, 1e+01f, 1.0f);
		ui_interface->addNumEntry("op5:egL1", &fentry66, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL2", &fentry68, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL3", &fentry58, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL4", &fentry60, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR1", &fentry67, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR2", &fentry69, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR3", &fentry70, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR4", &fentry61, 9e+01f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry65, "unit", "Hz");
		ui_interface->addNumEntry("op5:freq", &fentry65, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op5:key_vel_sens", &fentry59, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op5:level", &fentry57, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:mode", &fentry64, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op5:rate_scale", &fentry62, 0.0f, 0.0f, 1e+01f, 1.0f);
		ui_interface->addHorizontalSlider("pitch", &fslider1, 48.0f, 24.0f, 84.0f, 0.001f);
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		//zone1
		//zone2
		int 	iSlow0 = int(float(fentry0));
		int 	iSlow1 = (float((iSlow0 == 10)) != 0.0f);
		float 	fSlow2 = float(fbutton0);
		int 	iSlow3 = (fSlow2 > 0.0f);
		int 	iSlow4 = (iSlow3 > 0);
		float 	fSlow5 = float(fentry1);
		float 	fSlow6 = float(fentry2);
		float 	fSlow7 = float(fslider0);
		float 	fSlow8 = float(fentry3);
		float 	fSlow9 = ((0.0012626263f * (fSlow7 * fSlow8)) + (0.01010101f * ((1.0f - (0.125f * fSlow8)))));
		float 	fSlow10 = ((fSlow5 * fSlow6) * (fSlow9));
		float 	fSlow11 = float(fentry4);
		float 	fSlow12 = ((fSlow5 * fSlow11) * (fSlow9));
		int 	iSlow13 = (fSlow10 <= fSlow12);
		float 	fSlow14 = float(fentry9);
		float 	fSlow15 = powf(2.0f,(0.083333336f * ((float(fslider1) + -69.0f))));
		float 	fSlow16 = ((0.25f * float(fentry7)) + ((int(float(fentry8)))?fSlow14:(4.4e+02f * (fSlow15 * fSlow14))));
		float 	fSlow17 = ((0.057142857f * float(fentry6)) * (((17.31234f * logf((0.0022727272f * (fSlow16)))) + 48.0f)));
		float 	fSlow18 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry5) + fSlow17)))));
		float 	fSlow19 = powf((0.014705882f * (float((fSlow10 >= 3e+01f)) * min((fSlow10 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow20 = powf((0.01010101f * min(99.0f, fSlow10)),0.7f);
		float 	fSlow21 = powf((0.014705882f * (float((fSlow12 >= 3e+01f)) * min((fSlow12 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow22 = powf((0.01010101f * min(99.0f, fSlow12)),0.7f);
		float 	fSlow23 = max(0.001f, fabsf((((iSlow13)?max(0.003f, (38.0f * powf(fSlow18,12.0f))):max(0.008f, (318.0f * powf(fSlow18,12.6f)))) * ((((iSlow13)?fSlow19:fSlow20) - ((iSlow13)?fSlow21:fSlow22))))));
		float 	fSlow24 = (fConst0 * fSlow23);
		int 	iSlow25 = (float((iSlow0 == 9)) != 0.0f);
		int 	iSlow26 = (float((iSlow0 == 7)) != 0.0f);
		int 	iSlow27 = (float((iSlow0 == 6)) != 0.0f);
		int 	iSlow28 = (float((iSlow0 == 11)) != 0.0f);
		int 	iSlow29 = (float((iSlow0 == 12)) != 0.0f);
		int 	iSlow30 = (float((iSlow0 == 13)) != 0.0f);
		int 	iSlow31 = (float((iSlow0 == 14)) != 0.0f);
		int 	iSlow32 = (float((iSlow0 == 8)) != 0.0f);
		int 	iSlow33 = (float((iSlow0 == 5)) != 0.0f);
		int 	iSlow34 = (float((iSlow0 == 4)) != 0.0f);
		int 	iSlow35 = (float((iSlow0 == 3)) != 0.0f);
		int 	iSlow36 = (float((iSlow0 == 2)) != 0.0f);
		int 	iSlow37 = (float((iSlow0 == 1)) != 0.0f);
		int 	iSlow38 = (float((iSlow0 == 0)) != 0.0f);
		int 	iSlow39 = (float((iSlow0 == 15)) != 0.0f);
		float 	fSlow40 = float(iSlow3);
		int 	iSlow41 = (((fSlow2 == 0.0f)) > 0);
		float 	fSlow42 = float(fentry10);
		float 	fSlow43 = ((fSlow5 * fSlow42) * (fSlow9));
		int 	iSlow44 = (fSlow43 > fSlow12);
		float 	fSlow45 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry11) + fSlow17)))));
		float 	fSlow46 = powf((0.014705882f * (float((fSlow43 >= 3e+01f)) * min((fSlow43 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow47 = powf((0.01010101f * min(99.0f, fSlow43)),0.7f);
		float 	fSlow48 = max(0.001f, fabsf((((iSlow44)?max(0.003f, (38.0f * powf(fSlow45,12.0f))):max(0.008f, (318.0f * powf(fSlow45,12.6f)))) * ((((iSlow44)?fSlow21:fSlow22) - ((iSlow44)?fSlow46:fSlow47))))));
		float 	fSlow49 = float(fentry12);
		float 	fSlow50 = ((fSlow5 * fSlow49) * (fSlow9));
		int 	iSlow51 = (fSlow50 > fSlow43);
		float 	fSlow52 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry13) + fSlow17)))));
		float 	fSlow53 = powf((0.014705882f * (float((fSlow50 >= 3e+01f)) * min((fSlow50 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow54 = powf((0.01010101f * min(99.0f, fSlow50)),0.7f);
		float 	fSlow55 = (fSlow48 + max(0.001f, fabsf((((iSlow51)?max(0.003f, (38.0f * powf(fSlow52,12.0f))):max(0.008f, (318.0f * powf(fSlow52,12.6f)))) * ((((iSlow51)?fSlow53:fSlow54) - ((iSlow51)?fSlow46:fSlow47)))))));
		int 	iSlow56 = (fSlow10 > fSlow50);
		float 	fSlow57 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry14) + fSlow17)))));
		float 	fSlow58 = (fSlow55 + max(0.001f, fabsf((((iSlow56)?max(0.003f, (38.0f * powf(fSlow57,12.0f))):max(0.008f, (318.0f * powf(fSlow57,12.6f)))) * ((((iSlow56)?fSlow53:fSlow54) - ((iSlow56)?fSlow19:fSlow20)))))));
		float 	fSlow59 = (fConst0 * (fSlow58));
		float 	fSlow60 = (fConst0 * (fSlow55));
		int 	iTemp0 = iTempPerm0;
		float 	fSlow61 = (fConst0 * fSlow48);
		int 	iTemp1 = iTempPerm1;
		float 	fSlow62 = (fSlow5 * (fSlow9));
		float 	fSlow63 = (fConst1 * ((fSlow42 - fSlow11)));
		float 	fSlow64 = (0.0f - (fSlow62 * ((fSlow42 - fSlow49))));
		float 	fSlow65 = (0.0f - (fConst0 * ((fSlow48 - fSlow55))));
		float 	fSlow66 = (fSlow6 - fSlow49);
		float 	fSlow67 = (0.0f - (fConst0 * ((fSlow55 - fSlow58))));
		float 	fTemp2 = fTempPerm2;
		float 	fSlow68 = (fConst1 * (fSlow16));
		float 	fTemp3 = fTempPerm3;
		float 	fSlow69 = float(fentry15);
		float 	fSlow70 = float(fentry16);
		float 	fSlow71 = float(fentry17);
		float 	fSlow72 = ((0.0012626263f * (fSlow71 * fSlow7)) + (0.01010101f * ((1.0f - (0.125f * fSlow71)))));
		float 	fSlow73 = ((fSlow69 * fSlow70) * (fSlow72));
		float 	fSlow74 = float(fentry18);
		float 	fSlow75 = ((fSlow69 * fSlow74) * (fSlow72));
		int 	iSlow76 = (fSlow73 <= fSlow75);
		float 	fSlow77 = float(fentry23);
		float 	fSlow78 = ((0.25f * float(fentry21)) + ((int(float(fentry22)))?fSlow77:(4.4e+02f * (fSlow77 * fSlow15))));
		float 	fSlow79 = ((0.057142857f * float(fentry20)) * (((17.31234f * logf((0.0022727272f * (fSlow78)))) + 48.0f)));
		float 	fSlow80 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry19) + fSlow79)))));
		float 	fSlow81 = powf((0.014705882f * (float((fSlow73 >= 3e+01f)) * min((fSlow73 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow82 = powf((0.01010101f * min(99.0f, fSlow73)),0.7f);
		float 	fSlow83 = powf((0.014705882f * (float((fSlow75 >= 3e+01f)) * min((fSlow75 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow84 = powf((0.01010101f * min(99.0f, fSlow75)),0.7f);
		float 	fSlow85 = max(0.001f, fabsf((((iSlow76)?max(0.003f, (38.0f * powf(fSlow80,12.0f))):max(0.008f, (318.0f * powf(fSlow80,12.6f)))) * ((((iSlow76)?fSlow81:fSlow82) - ((iSlow76)?fSlow83:fSlow84))))));
		float 	fSlow86 = (fConst0 * fSlow85);
		float 	fSlow87 = float(fentry24);
		float 	fSlow88 = ((fSlow69 * fSlow87) * (fSlow72));
		int 	iSlow89 = (fSlow88 > fSlow75);
		float 	fSlow90 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry25) + fSlow79)))));
		float 	fSlow91 = powf((0.014705882f * (float((fSlow88 >= 3e+01f)) * min((fSlow88 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow92 = powf((0.01010101f * min(99.0f, fSlow88)),0.7f);
		float 	fSlow93 = max(0.001f, fabsf((((iSlow89)?max(0.003f, (38.0f * powf(fSlow90,12.0f))):max(0.008f, (318.0f * powf(fSlow90,12.6f)))) * ((((iSlow89)?fSlow83:fSlow84) - ((iSlow89)?fSlow91:fSlow92))))));
		float 	fSlow94 = float(fentry26);
		float 	fSlow95 = ((fSlow69 * fSlow94) * (fSlow72));
		int 	iSlow96 = (fSlow95 > fSlow88);
		float 	fSlow97 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry27) + fSlow79)))));
		float 	fSlow98 = powf((0.014705882f * (float((fSlow95 >= 3e+01f)) * min((fSlow95 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow99 = powf((0.01010101f * min(99.0f, fSlow95)),0.7f);
		float 	fSlow100 = (fSlow93 + max(0.001f, fabsf((((iSlow96)?max(0.003f, (38.0f * powf(fSlow97,12.0f))):max(0.008f, (318.0f * powf(fSlow97,12.6f)))) * ((((iSlow96)?fSlow98:fSlow99) - ((iSlow96)?fSlow91:fSlow92)))))));
		int 	iSlow101 = (fSlow73 > fSlow95);
		float 	fSlow102 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry28) + fSlow79)))));
		float 	fSlow103 = (fSlow100 + max(0.001f, fabsf((((iSlow101)?max(0.003f, (38.0f * powf(fSlow102,12.0f))):max(0.008f, (318.0f * powf(fSlow102,12.6f)))) * ((((iSlow101)?fSlow98:fSlow99) - ((iSlow101)?fSlow81:fSlow82)))))));
		float 	fSlow104 = (fConst0 * (fSlow103));
		float 	fSlow105 = (fConst0 * (fSlow100));
		int 	iTemp4 = iTempPerm4;
		float 	fSlow106 = (fConst0 * fSlow93);
		int 	iTemp5 = iTempPerm5;
		float 	fSlow107 = (fSlow69 * (fSlow72));
		float 	fSlow108 = (fConst1 * ((0.0f - (fSlow107 * ((fSlow74 - fSlow87))))));
		float 	fSlow109 = (0.0f - (fSlow107 * ((fSlow87 - fSlow94))));
		float 	fSlow110 = (0.0f - (fConst0 * ((fSlow93 - fSlow100))));
		float 	fSlow111 = (fConst1 * ((fSlow70 - fSlow94)));
		float 	fSlow112 = (fSlow103 - fSlow100);
		float 	fTemp6 = fTempPerm6;
		float 	fSlow113 = (fConst1 * (fSlow78));
		float 	fTemp7 = fTempPerm7;
		float 	fSlow114 = float(fentry29);
		float 	fSlow115 = float(fentry30);
		float 	fSlow116 = float(fentry31);
		float 	fSlow117 = ((0.0012626263f * (fSlow7 * fSlow116)) + (0.01010101f * ((1.0f - (0.125f * fSlow116)))));
		float 	fSlow118 = ((fSlow114 * fSlow115) * (fSlow117));
		float 	fSlow119 = float(fentry32);
		float 	fSlow120 = ((fSlow114 * fSlow119) * (fSlow117));
		int 	iSlow121 = (fSlow118 <= fSlow120);
		float 	fSlow122 = float(fentry37);
		float 	fSlow123 = ((0.25f * float(fentry35)) + ((int(float(fentry36)))?fSlow122:(4.4e+02f * (fSlow15 * fSlow122))));
		float 	fSlow124 = ((0.057142857f * float(fentry34)) * (((17.31234f * logf((0.0022727272f * (fSlow123)))) + 48.0f)));
		float 	fSlow125 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry33) + fSlow124)))));
		float 	fSlow126 = powf((0.014705882f * (float((fSlow120 >= 3e+01f)) * min((fSlow120 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow127 = powf((0.01010101f * min(99.0f, fSlow120)),0.7f);
		float 	fSlow128 = powf((0.014705882f * (float((fSlow118 >= 3e+01f)) * min((fSlow118 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow129 = powf((0.01010101f * min(99.0f, fSlow118)),0.7f);
		float 	fSlow130 = max(0.001f, fabsf((0.0f - (((iSlow121)?max(0.003f, (38.0f * powf(fSlow125,12.0f))):max(0.008f, (318.0f * powf(fSlow125,12.6f)))) * ((((iSlow121)?fSlow126:fSlow127) - ((iSlow121)?fSlow128:fSlow129)))))));
		float 	fSlow131 = (fConst0 * fSlow130);
		float 	fSlow132 = float(fentry38);
		float 	fSlow133 = ((fSlow114 * fSlow132) * (fSlow117));
		int 	iSlow134 = (fSlow133 > fSlow120);
		float 	fSlow135 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry39) + fSlow124)))));
		float 	fSlow136 = powf((0.014705882f * (float((fSlow133 >= 3e+01f)) * min((fSlow133 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow137 = powf((0.01010101f * min(99.0f, fSlow133)),0.7f);
		float 	fSlow138 = max(0.001f, fabsf((0.0f - (((iSlow134)?max(0.003f, (38.0f * powf(fSlow135,12.0f))):max(0.008f, (318.0f * powf(fSlow135,12.6f)))) * ((((iSlow134)?fSlow136:fSlow137) - ((iSlow134)?fSlow126:fSlow127)))))));
		float 	fSlow139 = float(fentry40);
		float 	fSlow140 = ((fSlow114 * fSlow139) * (fSlow117));
		int 	iSlow141 = (fSlow140 > fSlow133);
		float 	fSlow142 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry41) + fSlow124)))));
		float 	fSlow143 = powf((0.014705882f * (float((fSlow140 >= 3e+01f)) * min((fSlow140 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow144 = powf((0.01010101f * min(99.0f, fSlow140)),0.7f);
		float 	fSlow145 = (fSlow138 + max(0.001f, fabsf((((iSlow141)?max(0.003f, (38.0f * powf(fSlow142,12.0f))):max(0.008f, (318.0f * powf(fSlow142,12.6f)))) * ((((iSlow141)?fSlow143:fSlow144) - ((iSlow141)?fSlow136:fSlow137)))))));
		int 	iSlow146 = (fSlow118 > fSlow140);
		float 	fSlow147 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry42) + fSlow124)))));
		float 	fSlow148 = (fSlow145 + max(0.001f, fabsf((((iSlow146)?max(0.003f, (38.0f * powf(fSlow147,12.0f))):max(0.008f, (318.0f * powf(fSlow147,12.6f)))) * ((((iSlow146)?fSlow143:fSlow144) - ((iSlow146)?fSlow128:fSlow129)))))));
		float 	fSlow149 = (fConst0 * (fSlow148));
		float 	fSlow150 = (fConst0 * (fSlow145));
		int 	iTemp8 = iTempPerm8;
		float 	fSlow151 = (fConst0 * fSlow138);
		int 	iTemp9 = iTempPerm9;
		float 	fSlow152 = (fSlow114 * (fSlow117));
		float 	fSlow153 = (fConst1 * ((0.0f - (fSlow152 * ((fSlow119 - fSlow132))))));
		float 	fSlow154 = (fSlow139 - fSlow132);
		float 	fSlow155 = (0.0f - (fConst0 * ((fSlow138 - fSlow145))));
		float 	fSlow156 = (fSlow115 - fSlow139);
		float 	fSlow157 = (0.0f - (fConst0 * ((fSlow145 - fSlow148))));
		float 	fTemp10 = fTempPerm10;
		float 	fSlow158 = (fConst1 * (fSlow123));
		float 	fTemp11 = fTempPerm11;
		float 	fTemp12 = fTempPerm12;
		float 	fTemp13 = fTempPerm13;
		float 	fSlow159 = float(fentry43);
		float 	fSlow160 = float(fentry44);
		float 	fSlow161 = float(fentry45);
		float 	fSlow162 = ((0.0012626263f * (fSlow7 * fSlow161)) + (0.01010101f * ((1.0f - (0.125f * fSlow161)))));
		float 	fSlow163 = ((fSlow159 * fSlow160) * (fSlow162));
		float 	fSlow164 = float(fentry46);
		float 	fSlow165 = ((fSlow159 * fSlow164) * (fSlow162));
		int 	iSlow166 = (fSlow163 <= fSlow165);
		float 	fSlow167 = float(fentry51);
		float 	fSlow168 = ((0.25f * float(fentry49)) + ((int(float(fentry50)))?fSlow167:(4.4e+02f * (fSlow15 * fSlow167))));
		float 	fSlow169 = ((0.057142857f * float(fentry48)) * (((17.31234f * logf((0.0022727272f * (fSlow168)))) + 48.0f)));
		float 	fSlow170 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry47) + fSlow169)))));
		float 	fSlow171 = powf((0.014705882f * (float((fSlow165 >= 3e+01f)) * min((fSlow165 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow172 = powf((0.01010101f * min(99.0f, fSlow165)),0.7f);
		float 	fSlow173 = powf((0.014705882f * (float((fSlow163 >= 3e+01f)) * min((fSlow163 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow174 = powf((0.01010101f * min(99.0f, fSlow163)),0.7f);
		float 	fSlow175 = max(0.001f, fabsf((0.0f - (((iSlow166)?max(0.003f, (38.0f * powf(fSlow170,12.0f))):max(0.008f, (318.0f * powf(fSlow170,12.6f)))) * ((((iSlow166)?fSlow171:fSlow172) - ((iSlow166)?fSlow173:fSlow174)))))));
		float 	fSlow176 = (fConst0 * fSlow175);
		float 	fSlow177 = float(fentry52);
		float 	fSlow178 = ((fSlow159 * fSlow177) * (fSlow162));
		int 	iSlow179 = (fSlow178 > fSlow165);
		float 	fSlow180 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry53) + fSlow169)))));
		float 	fSlow181 = powf((0.014705882f * (float((fSlow178 >= 3e+01f)) * min((fSlow178 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow182 = powf((0.01010101f * min(99.0f, fSlow178)),0.7f);
		float 	fSlow183 = max(0.001f, fabsf((((iSlow179)?max(0.003f, (38.0f * powf(fSlow180,12.0f))):max(0.008f, (318.0f * powf(fSlow180,12.6f)))) * ((((iSlow179)?fSlow171:fSlow172) - ((iSlow179)?fSlow181:fSlow182))))));
		float 	fSlow184 = float(fentry54);
		float 	fSlow185 = ((fSlow159 * fSlow184) * (fSlow162));
		int 	iSlow186 = (fSlow185 > fSlow178);
		float 	fSlow187 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry55) + fSlow169)))));
		float 	fSlow188 = powf((0.014705882f * (float((fSlow185 >= 3e+01f)) * min((fSlow185 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow189 = powf((0.01010101f * min(99.0f, fSlow185)),0.7f);
		float 	fSlow190 = (fSlow183 + max(0.001f, fabsf((((iSlow186)?max(0.003f, (38.0f * powf(fSlow187,12.0f))):max(0.008f, (318.0f * powf(fSlow187,12.6f)))) * ((((iSlow186)?fSlow188:fSlow189) - ((iSlow186)?fSlow181:fSlow182)))))));
		int 	iSlow191 = (fSlow163 > fSlow185);
		float 	fSlow192 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry56) + fSlow169)))));
		float 	fSlow193 = (fSlow190 + max(0.001f, fabsf((((iSlow191)?max(0.003f, (38.0f * powf(fSlow192,12.0f))):max(0.008f, (318.0f * powf(fSlow192,12.6f)))) * ((((iSlow191)?fSlow188:fSlow189) - ((iSlow191)?fSlow173:fSlow174)))))));
		float 	fSlow194 = (fConst0 * (fSlow193));
		float 	fSlow195 = (fConst0 * (fSlow190));
		int 	iTemp14 = iTempPerm14;
		float 	fSlow196 = (fConst0 * fSlow183);
		int 	iTemp15 = iTempPerm15;
		float 	fSlow197 = (fSlow159 * (fSlow162));
		float 	fSlow198 = (fConst1 * ((0.0f - (fSlow197 * ((fSlow164 - fSlow177))))));
		float 	fSlow199 = (fConst1 * ((0.0f - (fSlow197 * ((fSlow177 - fSlow184))))));
		float 	fSlow200 = (fSlow190 - fSlow183);
		float 	fSlow201 = (fSlow160 - fSlow184);
		float 	fSlow202 = (0.0f - (fConst0 * ((fSlow190 - fSlow193))));
		float 	fTemp16 = fTempPerm16;
		float 	fSlow203 = (fConst1 * (fSlow168));
		float 	fTemp17 = fTempPerm17;
		float 	fSlow204 = float(fentry57);
		float 	fSlow205 = float(fentry58);
		float 	fSlow206 = float(fentry59);
		float 	fSlow207 = ((0.0012626263f * (fSlow7 * fSlow206)) + (0.01010101f * ((1.0f - (0.125f * fSlow206)))));
		float 	fSlow208 = ((fSlow204 * fSlow205) * (fSlow207));
		float 	fSlow209 = float(fentry60);
		float 	fSlow210 = ((fSlow204 * fSlow209) * (fSlow207));
		int 	iSlow211 = (fSlow208 <= fSlow210);
		float 	fSlow212 = float(fentry65);
		float 	fSlow213 = ((0.25f * float(fentry63)) + ((int(float(fentry64)))?fSlow212:(4.4e+02f * (fSlow15 * fSlow212))));
		float 	fSlow214 = ((0.057142857f * float(fentry62)) * (((17.31234f * logf((0.0022727272f * (fSlow213)))) + 48.0f)));
		float 	fSlow215 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry61) + fSlow214)))));
		float 	fSlow216 = powf((0.014705882f * (float((fSlow208 >= 3e+01f)) * min((fSlow208 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow217 = powf((0.01010101f * min(99.0f, fSlow208)),0.7f);
		float 	fSlow218 = powf((0.014705882f * (float((fSlow210 >= 3e+01f)) * min((fSlow210 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow219 = powf((0.01010101f * min(99.0f, fSlow210)),0.7f);
		float 	fSlow220 = max(0.001f, fabsf((((iSlow211)?max(0.003f, (38.0f * powf(fSlow215,12.0f))):max(0.008f, (318.0f * powf(fSlow215,12.6f)))) * ((((iSlow211)?fSlow216:fSlow217) - ((iSlow211)?fSlow218:fSlow219))))));
		float 	fSlow221 = (fConst0 * fSlow220);
		float 	fSlow222 = float(fentry66);
		float 	fSlow223 = ((fSlow204 * fSlow222) * (fSlow207));
		int 	iSlow224 = (fSlow223 > fSlow210);
		float 	fSlow225 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry67) + fSlow214)))));
		float 	fSlow226 = powf((0.014705882f * (float((fSlow223 >= 3e+01f)) * min((fSlow223 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow227 = powf((0.01010101f * min(99.0f, fSlow223)),0.7f);
		float 	fSlow228 = max(0.001f, fabsf((((iSlow224)?max(0.003f, (38.0f * powf(fSlow225,12.0f))):max(0.008f, (318.0f * powf(fSlow225,12.6f)))) * ((((iSlow224)?fSlow218:fSlow219) - ((iSlow224)?fSlow226:fSlow227))))));
		float 	fSlow229 = float(fentry68);
		float 	fSlow230 = ((fSlow204 * fSlow229) * (fSlow207));
		int 	iSlow231 = (fSlow230 > fSlow223);
		float 	fSlow232 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry69) + fSlow214)))));
		float 	fSlow233 = powf((0.014705882f * (float((fSlow230 >= 3e+01f)) * min((fSlow230 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow234 = powf((0.01010101f * min(99.0f, fSlow230)),0.7f);
		float 	fSlow235 = (fSlow228 + max(0.001f, fabsf((((iSlow231)?max(0.003f, (38.0f * powf(fSlow232,12.0f))):max(0.008f, (318.0f * powf(fSlow232,12.6f)))) * ((((iSlow231)?fSlow233:fSlow234) - ((iSlow231)?fSlow226:fSlow227)))))));
		int 	iSlow236 = (fSlow208 > fSlow230);
		float 	fSlow237 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry70) + fSlow214)))));
		float 	fSlow238 = (fSlow235 + max(0.001f, fabsf((((iSlow236)?max(0.003f, (38.0f * powf(fSlow237,12.0f))):max(0.008f, (318.0f * powf(fSlow237,12.6f)))) * ((((iSlow236)?fSlow233:fSlow234) - ((iSlow236)?fSlow216:fSlow217)))))));
		float 	fSlow239 = (fConst0 * (fSlow238));
		float 	fSlow240 = (fConst0 * (fSlow235));
		int 	iTemp18 = iTempPerm18;
		float 	fSlow241 = (fConst0 * fSlow228);
		int 	iTemp19 = iTempPerm19;
		float 	fSlow242 = (fSlow204 * (fSlow207));
		float 	fSlow243 = (fConst1 * ((0.0f - (fSlow242 * ((fSlow209 - fSlow222))))));
		float 	fSlow244 = (0.0f - (fSlow242 * ((fSlow222 - fSlow229))));
		float 	fSlow245 = (0.0f - (fConst0 * ((fSlow228 - fSlow235))));
		float 	fSlow246 = (fSlow205 - fSlow229);
		float 	fSlow247 = (0.0f - (fConst0 * ((fSlow235 - fSlow238))));
		float 	fTemp20 = fTempPerm20;
		float 	fSlow248 = (fConst1 * (fSlow213));
		float 	fTemp21 = fTempPerm21;
		float 	fTemp22 = fTempPerm22;
		float 	fSlow249 = float(fentry71);
		float 	fSlow250 = (1825.3162f * fSlow249);
		float 	fSlow251 = float(fentry72);
		float 	fSlow252 = float(fentry73);
		float 	fSlow253 = float(fentry74);
		float 	fSlow254 = ((0.0012626263f * (fSlow7 * fSlow253)) + (0.01010101f * ((1.0f - (0.125f * fSlow253)))));
		float 	fSlow255 = ((fSlow251 * fSlow252) * (fSlow254));
		float 	fSlow256 = float(fentry75);
		float 	fSlow257 = ((fSlow251 * fSlow256) * (fSlow254));
		int 	iSlow258 = (fSlow255 <= fSlow257);
		float 	fSlow259 = float(fentry80);
		float 	fSlow260 = ((0.25f * float(fentry78)) + ((int(float(fentry79)))?fSlow259:(4.4e+02f * (fSlow15 * fSlow259))));
		float 	fSlow261 = ((0.057142857f * float(fentry77)) * (((17.31234f * logf((0.0022727272f * (fSlow260)))) + 48.0f)));
		float 	fSlow262 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry76) + fSlow261)))));
		float 	fSlow263 = powf((0.014705882f * (float((fSlow255 >= 3e+01f)) * min((fSlow255 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow264 = powf((0.01010101f * min(99.0f, fSlow255)),0.7f);
		float 	fSlow265 = powf((0.014705882f * (float((fSlow257 >= 3e+01f)) * min((fSlow257 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow266 = powf((0.01010101f * min(99.0f, fSlow257)),0.7f);
		float 	fSlow267 = max(0.001f, fabsf((((iSlow258)?max(0.003f, (38.0f * powf(fSlow262,12.0f))):max(0.008f, (318.0f * powf(fSlow262,12.6f)))) * ((((iSlow258)?fSlow263:fSlow264) - ((iSlow258)?fSlow265:fSlow266))))));
		float 	fSlow268 = (fConst0 * fSlow267);
		float 	fSlow269 = float(fentry81);
		float 	fSlow270 = ((fSlow251 * fSlow269) * (fSlow254));
		int 	iSlow271 = (fSlow270 > fSlow257);
		float 	fSlow272 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry82) + fSlow261)))));
		float 	fSlow273 = powf((0.014705882f * (float((fSlow270 >= 3e+01f)) * min((fSlow270 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow274 = powf((0.01010101f * min(99.0f, fSlow270)),0.7f);
		float 	fSlow275 = max(0.001f, fabsf((((iSlow271)?max(0.003f, (38.0f * powf(fSlow272,12.0f))):max(0.008f, (318.0f * powf(fSlow272,12.6f)))) * ((((iSlow271)?fSlow265:fSlow266) - ((iSlow271)?fSlow273:fSlow274))))));
		float 	fSlow276 = float(fentry83);
		float 	fSlow277 = ((fSlow251 * fSlow276) * (fSlow254));
		int 	iSlow278 = (fSlow277 > fSlow270);
		float 	fSlow279 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry84) + fSlow261)))));
		float 	fSlow280 = powf((0.014705882f * (float((fSlow277 >= 3e+01f)) * min((fSlow277 + -3e+01f), 68.0f))),1.8f);
		float 	fSlow281 = powf((0.01010101f * min(99.0f, fSlow277)),0.7f);
		float 	fSlow282 = (fSlow275 + max(0.001f, fabsf((((iSlow278)?max(0.003f, (38.0f * powf(fSlow279,12.0f))):max(0.008f, (318.0f * powf(fSlow279,12.6f)))) * ((((iSlow278)?fSlow280:fSlow281) - ((iSlow278)?fSlow273:fSlow274)))))));
		int 	iSlow283 = (fSlow255 > fSlow277);
		float 	fSlow284 = (0.007874016f * ((127.0f - min(99.0f, (float(fentry85) + fSlow261)))));
		float 	fSlow285 = (fSlow282 + max(0.001f, fabsf((((iSlow283)?max(0.003f, (38.0f * powf(fSlow284,12.0f))):max(0.008f, (318.0f * powf(fSlow284,12.6f)))) * ((((iSlow283)?fSlow280:fSlow281) - ((iSlow283)?fSlow263:fSlow264)))))));
		float 	fSlow286 = (fConst0 * (fSlow285));
		float 	fSlow287 = (fConst0 * (fSlow282));
		int 	iTemp23 = iTempPerm23;
		float 	fSlow288 = (fConst0 * fSlow275);
		int 	iTemp24 = iTempPerm24;
		float 	fSlow289 = (fSlow251 * (fSlow254));
		float 	fSlow290 = (fConst1 * ((fSlow269 - fSlow256)));
		float 	fSlow291 = (0.0f - (fSlow289 * ((fSlow269 - fSlow276))));
		float 	fSlow292 = (0.0f - (fConst0 * ((fSlow275 - fSlow282))));
		float 	fSlow293 = (0.0f - (fSlow289 * ((fSlow276 - fSlow252))));
		float 	fSlow294 = (0.0f - (fConst0 * ((fSlow282 - fSlow285))));
		float 	fTemp25 = fTempPerm25;
		float 	fSlow295 = (fConst1 * (fSlow260));
		float 	fTemp26 = fTempPerm26;
		float 	fTemp27 = fTempPerm27;
		float 	fTemp28 = fTempPerm28;
		float 	fTemp29 = fTempPerm29;
		float 	fSlow296 = (3650.6323f * fSlow249);
		float 	fTemp30 = fTempPerm30;
		float 	fTemp31 = fTempPerm31;
		float 	fTemp32 = fTempPerm32;
		float 	fTemp33 = fTempPerm33;
		float 	fTemp34 = fTempPerm34;
		float 	fTemp35 = fTempPerm35;
		float 	fTemp36 = fTempPerm36;
		float 	fTemp37 = fTempPerm37;
		float 	fTemp38 = fTempPerm38;
		float 	fTemp39 = fTempPerm39;
		float 	fTemp40 = fTempPerm40;
		float 	fSlow297 = (912.6581f * fSlow249);
		float 	fTemp41 = fTempPerm41;
		float 	fSlow298 = (9126.581f * fSlow249);
		float 	fTemp42 = fTempPerm42;
		float 	fTemp43 = fTempPerm43;
		float 	fTemp44 = fTempPerm44;
		float 	fTemp45 = fTempPerm45;
		float 	fTemp46 = fTempPerm46;
		float 	fTemp47 = fTempPerm47;
		float 	fTemp48 = fTempPerm48;
		float 	fTemp49 = fTempPerm49;
		float 	fTemp50 = fTempPerm50;
		float 	fTemp51 = fTempPerm51;
		float 	fTemp52 = fTempPerm52;
		float 	fTemp53 = fTempPerm53;
		float 	fTemp54 = fTempPerm54;
		float 	fTemp55 = fTempPerm55;
		float 	fTemp56 = fTempPerm56;
		//zone2b
		//zone3
		FAUSTFLOAT* output0 = output[0];
		//LoopGraphScalar
		for (int i=0; i<count; i++) {
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
				fRec0[0] = ((iSlow4)?0.0f:min(fSlow24, (fRec0[1] + 1.0f)));
				fRec2[0] = ((iSlow41)?0.0f:min(fSlow59, (fRec2[1] + 1.0f)));
				iTemp0 = (fRec2[0] < fSlow60);
				iTemp1 = (fRec2[0] < fSlow61);
				fRec1[0] = ((iSlow3)?(fSlow40 * ((iTemp0)?((iTemp1)?(((fRec2[0] < 0.0f))?fSlow12:((iTemp1)?(fSlow62 * ((fSlow11 + (fSlow63 * (fRec2[0] / fSlow48))))):fSlow43)):((iTemp0)?(fSlow43 + ((fSlow64) * (((fRec2[0] - fSlow61)) / (fSlow65)))):fSlow50)):(((fRec2[0] < fSlow59))?(fSlow62 * ((fSlow49 + ((fSlow66) * (((fRec2[0] - fSlow60)) / (fSlow67)))))):fSlow10))):fRec1[1]);
				fTemp2 = powf((0.010204081f * min(98.0f, (((fRec0[0] < 0.0f))?fRec1[0]:(((fRec0[0] < fSlow24))?(fRec1[0] + (fConst1 * ((fRec0[0] * ((fSlow12 - fRec1[0]))) / fSlow23))):fSlow12)))),8.0f);
				fTemp3 = (fRec4[1] + fSlow68);
				fRec4[0] = (fTemp3 - floorf(fTemp3));
				fRec5[0] = ((iSlow4)?0.0f:min(fSlow86, (fRec5[1] + 1.0f)));
				fRec7[0] = ((iSlow41)?0.0f:min(fSlow104, (fRec7[1] + 1.0f)));
				iTemp4 = (fRec7[0] < fSlow105);
				iTemp5 = (fRec7[0] < fSlow106);
				fRec6[0] = ((iSlow3)?(fSlow40 * ((iTemp4)?((iTemp5)?(((fRec7[0] < 0.0f))?fSlow75:((iTemp5)?(fSlow75 + (fSlow108 * (fRec7[0] / fSlow93))):fSlow88)):((iTemp4)?(fSlow88 + ((fSlow109) * (((fRec7[0] - fSlow106)) / (fSlow110)))):fSlow95)):(((fRec7[0] < fSlow104))?(fSlow107 * ((fSlow94 + (fSlow111 * (((fRec7[0] - fSlow105)) / (fSlow112)))))):fSlow73))):fRec6[1]);
				fTemp6 = powf((0.010204081f * min(98.0f, (((fRec5[0] < 0.0f))?fRec6[0]:(((fRec5[0] < fSlow86))?(fRec6[0] + (fConst1 * ((fRec5[0] * ((fSlow75 - fRec6[0]))) / fSlow85))):fSlow75)))),8.0f);
				fTemp7 = (fRec8[1] + fSlow113);
				fRec8[0] = (fTemp7 - floorf(fTemp7));
				fRec9[0] = ((iSlow4)?0.0f:min(fSlow131, (fRec9[1] + 1.0f)));
				fRec11[0] = ((iSlow41)?0.0f:min(fSlow149, (fRec11[1] + 1.0f)));
				iTemp8 = (fRec11[0] < fSlow150);
				iTemp9 = (fRec11[0] < fSlow151);
				fRec10[0] = ((iSlow3)?(fSlow40 * ((iTemp8)?((iTemp9)?(((fRec11[0] < 0.0f))?fSlow120:((iTemp9)?(fSlow120 + (fSlow153 * (fRec11[0] / fSlow138))):fSlow133)):((iTemp8)?(fSlow152 * ((fSlow132 + ((fSlow154) * (((fRec11[0] - fSlow151)) / (fSlow155)))))):fSlow140)):(((fRec11[0] < fSlow149))?(fSlow152 * ((fSlow139 + ((fSlow156) * (((fRec11[0] - fSlow150)) / (fSlow157)))))):fSlow118))):fRec10[1]);
				fTemp10 = powf((0.010204081f * min(98.0f, (((fRec9[0] < 0.0f))?fRec10[0]:(((fRec9[0] < fSlow131))?(fRec10[0] + (fConst1 * ((fRec9[0] * ((fSlow120 - fRec10[0]))) / fSlow130))):fSlow120)))),8.0f);
				fTemp11 = (fRec12[1] + fSlow158);
				fRec12[0] = (fTemp11 - floorf(fTemp11));
			}
			if ((iSlow1 || (iSlow25 || (iSlow35 || iSlow36)))) {
				fTemp12 = (65536.0f * fRec12[0]);
			}
			if ((iSlow1 || (iSlow35 || iSlow36))) {
				fTemp13 = (fTemp2 * ftbl0[((((int((65536.0f * ((fRec4[0] + (fTemp6 * ftbl0[((((int((65536.0f * ((fRec8[0] + (fTemp10 * ftbl0[((((int(fTemp12) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]);
			}
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
				fRec13[0] = ((iSlow4)?0.0f:min(fSlow176, (fRec13[1] + 1.0f)));
				fRec15[0] = ((iSlow41)?0.0f:min(fSlow194, (fRec15[1] + 1.0f)));
				iTemp14 = (fRec15[0] < fSlow195);
				iTemp15 = (fRec15[0] < fSlow196);
				fRec14[0] = ((iSlow3)?(fSlow40 * ((iTemp14)?((iTemp15)?(((fRec15[0] < 0.0f))?fSlow165:((iTemp15)?(fSlow165 + (fSlow198 * (fRec15[0] / fSlow183))):fSlow178)):((iTemp14)?(fSlow178 + (fSlow199 * (((fRec15[0] - fSlow196)) / (fSlow200)))):fSlow185)):(((fRec15[0] < fSlow194))?(fSlow197 * ((fSlow184 + ((fSlow201) * (((fRec15[0] - fSlow195)) / (fSlow202)))))):fSlow163))):fRec14[1]);
				fTemp16 = powf((0.010204081f * min(98.0f, (((fRec13[0] < 0.0f))?fRec14[0]:(((fRec13[0] < fSlow176))?(fRec14[0] + (fConst1 * ((fRec13[0] * ((fSlow165 - fRec14[0]))) / fSlow175))):fSlow165)))),8.0f);
				fTemp17 = (fRec16[1] + fSlow203);
				fRec16[0] = (fTemp17 - floorf(fTemp17));
				fRec18[0] = ((iSlow4)?0.0f:min(fSlow221, (fRec18[1] + 1.0f)));
				fRec20[0] = ((iSlow41)?0.0f:min(fSlow239, (fRec20[1] + 1.0f)));
				iTemp18 = (fRec20[0] < fSlow240);
				iTemp19 = (fRec20[0] < fSlow241);
				fRec19[0] = ((iSlow3)?(fSlow40 * ((iTemp18)?((iTemp19)?(((fRec20[0] < 0.0f))?fSlow210:((iTemp19)?(fSlow210 + (fSlow243 * (fRec20[0] / fSlow228))):fSlow223)):((iTemp18)?(fSlow223 + ((fSlow244) * (((fRec20[0] - fSlow241)) / (fSlow245)))):fSlow230)):(((fRec20[0] < fSlow239))?(fSlow242 * ((fSlow229 + ((fSlow246) * (((fRec20[0] - fSlow240)) / (fSlow247)))))):fSlow208))):fRec19[1]);
				fTemp20 = powf((0.010204081f * min(98.0f, (((fRec18[0] < 0.0f))?fRec19[0]:(((fRec18[0] < fSlow221))?(fRec19[0] + (fConst1 * ((fRec18[0] * ((fSlow210 - fRec19[0]))) / fSlow220))):fSlow210)))),8.0f);
				fTemp21 = (fRec21[1] + fSlow248);
				fRec21[0] = (fTemp21 - floorf(fTemp21));
				fTemp22 = (65536.0f * fRec21[0]);
			}
			if ((iSlow1 || iSlow38)) {
				fRec17[0] = (fTemp20 * ftbl0[((((int((fTemp22 + (fSlow250 * fRec17[1]))) % 65536) + 65536)) % 65536)]);
			}
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
				fRec22[0] = ((iSlow4)?0.0f:min(fSlow268, (fRec22[1] + 1.0f)));
				fRec24[0] = ((iSlow41)?0.0f:min(fSlow286, (fRec24[1] + 1.0f)));
				iTemp23 = (fRec24[0] < fSlow287);
				iTemp24 = (fRec24[0] < fSlow288);
				fRec23[0] = ((iSlow3)?(fSlow40 * ((iTemp23)?((iTemp24)?(((fRec24[0] < 0.0f))?fSlow257:((iTemp24)?(fSlow289 * ((fSlow256 + (fSlow290 * (fRec24[0] / fSlow275))))):fSlow270)):((iTemp23)?(fSlow270 + ((fSlow291) * (((fRec24[0] - fSlow288)) / (fSlow292)))):fSlow277)):(((fRec24[0] < fSlow286))?(fSlow277 + ((fSlow293) * (((fRec24[0] - fSlow287)) / (fSlow294)))):fSlow255))):fRec23[1]);
				fTemp25 = powf((0.010204081f * min(98.0f, (((fRec22[0] < 0.0f))?fRec23[0]:(((fRec22[0] < fSlow268))?(fRec23[0] + (fConst1 * ((fRec22[0] * ((fSlow257 - fRec23[0]))) / fSlow267))):fSlow257)))),8.0f);
				fTemp26 = (fRec25[1] + fSlow295);
				fRec25[0] = (fTemp26 - floorf(fTemp26));
			}
			if ((iSlow1 || (iSlow25 || (iSlow28 || (iSlow29 || (iSlow30 || iSlow31)))))) {
				fTemp27 = (fTemp25 * ftbl0[((((int((65536.0f * fRec25[0])) % 65536) + 65536)) % 65536)]);
			}
			if (iSlow1) {
				fTemp28 = (2.08795f * ((fTemp13 + (fTemp16 * ftbl0[((((int((65536.0f * ((fRec16[0] + (fRec17[0] + fTemp27))))) % 65536) + 65536)) % 65536)]))));
			}
			if ((iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow37 || (iSlow38 || iSlow39)))))))))))) {
				fTemp29 = (65536.0f * fRec8[0]);
			}
			if ((iSlow31 || iSlow32)) {
				fRec26[0] = (fTemp6 * ftbl0[((((int((fTemp29 + (fSlow296 * fRec26[1]))) % 65536) + 65536)) % 65536)]);
				fTemp30 = (fTemp2 * ftbl0[((((int((65536.0f * ((fRec4[0] + fRec26[0])))) % 65536) + 65536)) % 65536)]);
			}
			if ((iSlow25 || (iSlow26 || (iSlow28 || (iSlow31 || (iSlow32 || iSlow37)))))) {
				fTemp31 = (fTemp20 * ftbl0[((((int(fTemp22) % 65536) + 65536)) % 65536)]);
			}
			if ((iSlow25 || iSlow31)) {
				fTemp32 = (fTemp16 * ftbl0[((((int((65536.0f * ((fRec16[0] + (fTemp31 + fTemp27))))) % 65536) + 65536)) % 65536)]);
			}
			if (iSlow31) {
				fTemp33 = (2.08795f * ((fTemp30 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + fTemp32)))) % 65536) + 65536)) % 65536)]))));
			}
			if (iSlow25) {
				fRec27[0] = (fTemp10 * ftbl0[((((int((fTemp12 + (fSlow250 * fRec27[1]))) % 65536) + 65536)) % 65536)]);
				fTemp34 = (2.08795f * ((fTemp32 + (fTemp2 * ftbl0[((((int((65536.0f * ((fRec4[0] + (fTemp6 * ftbl0[((((int((65536.0f * ((fRec8[0] + fRec27[0])))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]))));
			}
			if ((iSlow26 || (iSlow32 || iSlow37))) {
				fTemp35 = (fTemp25 * ftbl0[((((int((65536.0f * ((fRec25[0] + fTemp31)))) % 65536) + 65536)) % 65536)]);
			}
			if ((iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow32 || (iSlow33 || (iSlow34 || iSlow39)))))))) {
				fTemp36 = (65536.0f * fRec16[0]);
			}
			if ((iSlow27 || (iSlow28 || (iSlow29 || (iSlow32 || (iSlow33 || (iSlow34 || iSlow39))))))) {
				fTemp37 = (fTemp16 * ftbl0[((((int(fTemp36) % 65536) + 65536)) % 65536)]);
			}
			if (iSlow32) {
				fTemp38 = (2.08795f * ((fTemp30 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + (fTemp35 + fTemp37))))) % 65536) + 65536)) % 65536)]))));
			}
			if ((iSlow26 || (iSlow27 || (iSlow29 || (iSlow30 || (iSlow33 || (iSlow34 || (iSlow38 || iSlow39)))))))) {
				fTemp39 = (fTemp6 * ftbl0[((((int(fTemp29) % 65536) + 65536)) % 65536)]);
			}
			if ((iSlow26 || (iSlow27 || (iSlow29 || (iSlow30 || (iSlow33 || (iSlow34 || iSlow38))))))) {
				fTemp40 = (fTemp2 * ftbl0[((((int((65536.0f * ((fRec4[0] + fTemp39)))) % 65536) + 65536)) % 65536)]);
			}
			if (iSlow26) {
				fRec28[0] = (fTemp16 * ftbl0[((((int((fTemp36 + (fSlow297 * fRec28[1]))) % 65536) + 65536)) % 65536)]);
				fTemp41 = (2.08795f * ((fTemp40 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + (fRec28[0] + fTemp35))))) % 65536) + 65536)) % 65536)]))));
			}
			if ((iSlow27 || (iSlow29 || (iSlow30 || (iSlow36 || iSlow39))))) {
				fRec29[0] = (fTemp20 * ftbl0[((((int((fTemp22 + (fSlow298 * fRec29[1]))) % 65536) + 65536)) % 65536)]);
			}
			if ((iSlow27 || (iSlow36 || iSlow39))) {
				fTemp42 = (fTemp25 * ftbl0[((((int((65536.0f * ((fRec25[0] + fRec29[0])))) % 65536) + 65536)) % 65536)]);
			}
			if (iSlow27) {
				fTemp43 = (2.08795f * ((fTemp40 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + (fTemp42 + fTemp37))))) % 65536) + 65536)) % 65536)]))));
			}
			if (iSlow33) {
				fRec30[0] = (2.08795f * (fTemp25 * ftbl0[((((int((65536.0f * ((fRec25[0] + (fTemp20 * ftbl0[((((int((fTemp22 + (fSlow298 * fRec30[1]))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]));
			}
			if ((iSlow33 || (iSlow34 || iSlow39))) {
				fTemp44 = (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + fTemp37)))) % 65536) + 65536)) % 65536)]);
			}
			if ((iSlow33 || iSlow34)) {
				fTemp45 = (fTemp40 + fTemp44);
			}
			if (iSlow33) {
				fTemp46 = (fRec30[0] + (2.08795f * (fTemp45)));
			}
			if (iSlow34) {
				fRec31[0] = (fTemp20 * ftbl0[((((int((fTemp22 + (fSlow297 * fRec31[1]))) % 65536) + 65536)) % 65536)]);
				fTemp47 = (2.08795f * ((fTemp45 + (fTemp25 * ftbl0[((((int((65536.0f * ((fRec25[0] + fRec31[0])))) % 65536) + 65536)) % 65536)]))));
			}
			if (iSlow35) {
				fRec32[0] = (2.08795f * (fTemp16 * ftbl0[((((int((65536.0f * ((fRec16[0] + (fTemp25 * ftbl0[((((int((65536.0f * ((fRec25[0] + (fTemp20 * ftbl0[((((int((fTemp22 + (fSlow298 * fRec32[1]))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]));
				fTemp48 = (fRec32[0] + (2.08795f * fTemp13));
			}
			if (iSlow36) {
				fTemp49 = (2.08795f * ((fTemp13 + (fTemp16 * ftbl0[((((int((65536.0f * ((fRec16[0] + fTemp42)))) % 65536) + 65536)) % 65536)]))));
			}
			if ((iSlow28 || iSlow37)) {
				fRec33[0] = (fTemp6 * ftbl0[((((int((fTemp29 + (fSlow250 * fRec33[1]))) % 65536) + 65536)) % 65536)]);
				fTemp50 = (fTemp2 * ftbl0[((((int((65536.0f * ((fRec4[0] + fRec33[0])))) % 65536) + 65536)) % 65536)]);
			}
			if (iSlow37) {
				fTemp51 = (2.08795f * ((fTemp50 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + (fTemp16 * ftbl0[((((int((65536.0f * ((fRec16[0] + fTemp35)))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]))));
			}
			if (iSlow38) {
				fTemp52 = (2.08795f * ((fTemp40 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + (fTemp16 * ftbl0[((((int((65536.0f * ((fRec16[0] + (fTemp25 * ftbl0[((((int((65536.0f * ((fRec25[0] + fRec17[0])))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]))));
			}
			if (iSlow28) {
				fTemp53 = (2.08795f * ((fTemp50 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + (fTemp27 + (fTemp31 + fTemp37)))))) % 65536) + 65536)) % 65536)]))));
			}
			if (iSlow29) {
				fTemp54 = (2.08795f * ((fTemp40 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + (fTemp27 + (fRec29[0] + fTemp37)))))) % 65536) + 65536)) % 65536)]))));
			}
			if (iSlow30) {
				fTemp55 = (2.08795f * ((fTemp40 + (fTemp10 * ftbl0[((((int((65536.0f * ((fRec12[0] + (fTemp16 * ftbl0[((((int((65536.0f * ((fRec16[0] + (fRec29[0] + fTemp27))))) % 65536) + 65536)) % 65536)]))))) % 65536) + 65536)) % 65536)]))));
			}
			if (iSlow39) {
				fTemp56 = (2.08795f * (fTemp2 * ftbl0[((((int((65536.0f * ((fRec4[0] + (fTemp39 + (fTemp42 + fTemp44)))))) % 65536) + 65536)) % 65536)]));
			}
			output0[i] = (FAUSTFLOAT)((fTemp28 + (fTemp33 + ((fTemp34 + (fTemp38 + (fTemp41 + (fTemp43 + (fTemp46 + (fTemp47 + (fTemp48 + (fTemp49 + (fTemp51 + (fTemp52 + ((fTemp53 + fTemp54) + fTemp55))))))))))) + fTemp56))));
			// post processing
			if ((iSlow28 || iSlow37)) {
				fRec33[1] = fRec33[0];
			}
			if (iSlow35) {
				fRec32[1] = fRec32[0];
			}
			if (iSlow34) {
				fRec31[1] = fRec31[0];
			}
			if (iSlow33) {
				fRec30[1] = fRec30[0];
			}
			if ((iSlow27 || (iSlow29 || (iSlow30 || (iSlow36 || iSlow39))))) {
				fRec29[1] = fRec29[0];
			}
			if (iSlow26) {
				fRec28[1] = fRec28[0];
			}
			if (iSlow25) {
				fRec27[1] = fRec27[0];
			}
			if ((iSlow31 || iSlow32)) {
				fRec26[1] = fRec26[0];
			}
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
				fRec25[1] = fRec25[0];
				fRec23[1] = fRec23[0];
				fRec24[1] = fRec24[0];
				fRec22[1] = fRec22[0];
			}
			if ((iSlow1 || iSlow38)) {
				fRec17[1] = fRec17[0];
			}
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
				fRec21[1] = fRec21[0];
				fRec19[1] = fRec19[0];
				fRec20[1] = fRec20[0];
				fRec18[1] = fRec18[0];
				fRec16[1] = fRec16[0];
				fRec14[1] = fRec14[0];
				fRec15[1] = fRec15[0];
				fRec13[1] = fRec13[0];
				fRec12[1] = fRec12[0];
				fRec10[1] = fRec10[0];
				fRec11[1] = fRec11[0];
				fRec9[1] = fRec9[0];
				fRec8[1] = fRec8[0];
				fRec6[1] = fRec6[0];
				fRec7[1] = fRec7[0];
				fRec5[1] = fRec5[0];
				fRec4[1] = fRec4[0];
				fRec1[1] = fRec1[0];
				fRec2[1] = fRec2[0];
				fRec0[1] = fRec0[0];
			}
		}
		iTempPerm0 = iTemp0;
		iTempPerm1 = iTemp1;
		fTempPerm2 = fTemp2;
		fTempPerm3 = fTemp3;
		iTempPerm4 = iTemp4;
		iTempPerm5 = iTemp5;
		fTempPerm6 = fTemp6;
		fTempPerm7 = fTemp7;
		iTempPerm8 = iTemp8;
		iTempPerm9 = iTemp9;
		fTempPerm10 = fTemp10;
		fTempPerm11 = fTemp11;
		fTempPerm12 = fTemp12;
		fTempPerm13 = fTemp13;
		iTempPerm14 = iTemp14;
		iTempPerm15 = iTemp15;
		fTempPerm16 = fTemp16;
		fTempPerm17 = fTemp17;
		iTempPerm18 = iTemp18;
		iTempPerm19 = iTemp19;
		fTempPerm20 = fTemp20;
		fTempPerm21 = fTemp21;
		fTempPerm22 = fTemp22;
		iTempPerm23 = iTemp23;
		iTempPerm24 = iTemp24;
		fTempPerm25 = fTemp25;
		fTempPerm26 = fTemp26;
		fTempPerm27 = fTemp27;
		fTempPerm28 = fTemp28;
		fTempPerm29 = fTemp29;
		fTempPerm30 = fTemp30;
		fTempPerm31 = fTemp31;
		fTempPerm32 = fTemp32;
		fTempPerm33 = fTemp33;
		fTempPerm34 = fTemp34;
		fTempPerm35 = fTemp35;
		fTempPerm36 = fTemp36;
		fTempPerm37 = fTemp37;
		fTempPerm38 = fTemp38;
		fTempPerm39 = fTemp39;
		fTempPerm40 = fTemp40;
		fTempPerm41 = fTemp41;
		fTempPerm42 = fTemp42;
		fTempPerm43 = fTemp43;
		fTempPerm44 = fTemp44;
		fTempPerm45 = fTemp45;
		fTempPerm46 = fTemp46;
		fTempPerm47 = fTemp47;
		fTempPerm48 = fTemp48;
		fTempPerm49 = fTemp49;
		fTempPerm50 = fTemp50;
		fTempPerm51 = fTemp51;
		fTempPerm52 = fTemp52;
		fTempPerm53 = fTemp53;
		fTempPerm54 = fTemp54;
		fTempPerm55 = fTemp55;
		fTempPerm56 = fTemp56;
	}
};


float 	synth_dx7::ftbl0[65536];
// clang-format on
#endif

    template <class T>
    struct _synth_dx7_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_dx7_UI<T>::name(sym(synth_dx7));

typedef _synth_dx7_UI<synth_dx7> synth_dx7_UI;

class faust_synth_dx7_tilde : public FaustExternal<synth_dx7, synth_dx7_UI> {
public:
    faust_synth_dx7_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};
