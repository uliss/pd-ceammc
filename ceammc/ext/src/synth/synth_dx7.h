//----------------------------------------------------------
// name: "synth.dx7"
//
// Code generated with Faust 2.44.1 (https://faust.grame.fr)
//----------------------------------------------------------

/* link with  */
#include <math.h>
#ifndef FAUSTPOWER
#define FAUSTPOWER
#include <cmath>
template <int N> inline int faustpower(int x)              { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline int faustpower<0>(int x)            { return 1; }
template <> 	 inline int faustpower<1>(int x)            { return x; }
template <> 	 inline int faustpower<2>(int x)            { return x*x; }
template <int N> inline float faustpower(float x)            { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline float faustpower<0>(float x)          { return 1; }
template <> 	 inline float faustpower<1>(float x)          { return x; }
template <> 	 inline float faustpower<2>(float x)          { return x*x; }
#endif
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
				iRec3[0] = (iVec0[1] + iRec3[1]) % 65536;
				output[i] = sinf(9.5873799242852573e-05f * float(iRec3[0]));
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
	float 	fTempPerm8;
	float 	fTempPerm9;
	float 	fTempPerm10;
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
	int 	iTempPerm11;
	int 	iTempPerm12;
	float 	fRec10[2];
	float 	fTempPerm13;
	float 	fTempPerm14;
	float 	fRec12[2];
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
	int 	iTempPerm15;
	int 	iTempPerm16;
	float 	fRec14[2];
	float 	fTempPerm17;
	float 	fTempPerm18;
	float 	fRec16[2];
	float 	fTempPerm19;
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
	int 	iTempPerm20;
	int 	iTempPerm21;
	float 	fRec19[2];
	float 	fTempPerm22;
	float 	fTempPerm23;
	float 	fRec21[2];
	float 	fTempPerm24;
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
	int 	iTempPerm25;
	int 	iTempPerm26;
	float 	fRec23[2];
	float 	fTempPerm27;
	float 	fTempPerm28;
	float 	fRec25[2];
	float 	fTempPerm29;
	float 	fTempPerm30;
	float 	fTempPerm31;
	float 	fRec26[2];
	float 	fTempPerm32;
	float 	fTempPerm33;
	float 	fTempPerm34;
	float 	fTempPerm35;
	float 	fTempPerm36;
	float 	fRec27[2];
	float 	fTempPerm37;
	float 	fTempPerm38;
	float 	fRec28[2];
	float 	fTempPerm39;
	float 	fRec29[2];
	float 	fTempPerm40;
	float 	fTempPerm41;
	float 	fTempPerm42;
	float 	fRec30[2];
	float 	fTempPerm43;
	float 	fTempPerm44;
	float 	fTempPerm45;
	float 	fRec31[2];
	float 	fTempPerm46;
	float 	fTempPerm47;
	float 	fTempPerm48;
	float 	fRec32[2];
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
                fConst0 = std::fmin(192000.0f, std::fmax(1.0f, (float)(fSampleRate)));
		iTempPerm0 = 0;
		iTempPerm1 = 0;
		fConst1 = (1.0f / fConst0);
		fTempPerm2 = 0;
		fTempPerm3 = 0;
		iTempPerm4 = 0;
		iTempPerm5 = 0;
		fTempPerm6 = 0;
		fTempPerm7 = 0;
		fTempPerm8 = 0;
		fTempPerm9 = 0;
		fTempPerm10 = 0;
		iTempPerm11 = 0;
		iTempPerm12 = 0;
		fTempPerm13 = 0;
		fTempPerm14 = 0;
		iTempPerm15 = 0;
		iTempPerm16 = 0;
		fTempPerm17 = 0;
		fTempPerm18 = 0;
		fTempPerm19 = 0;
		iTempPerm20 = 0;
		iTempPerm21 = 0;
		fTempPerm22 = 0;
		fTempPerm23 = 0;
		fTempPerm24 = 0;
		iTempPerm25 = 0;
		iTempPerm26 = 0;
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
		fentry2 = 90.0f;
		fslider0 = 0.80000000000000004f;
		fentry3 = 1.0f;
		fentry4 = 0.0f;
		fentry5 = 90.0f;
		fentry6 = 0.0f;
		fentry7 = 1.0f;
		fentry8 = 0.0f;
		fentry9 = 1.0f;
		fslider1 = 48.0f;
		fentry10 = 0.0f;
		fentry11 = 90.0f;
		fentry12 = 90.0f;
		fentry13 = 90.0f;
		fentry14 = 90.0f;
		fentry15 = 95.0f;
		fentry16 = 90.0f;
		fentry17 = 1.0f;
		fentry18 = 0.0f;
		fentry19 = 90.0f;
		fentry20 = 0.0f;
		fentry21 = 1.0f;
		fentry22 = 0.0f;
		fentry23 = 1.0f;
		fentry24 = 0.0f;
		fentry25 = 90.0f;
		fentry26 = 90.0f;
		fentry27 = 90.0f;
		fentry28 = 90.0f;
		fentry29 = 95.0f;
		fentry30 = 90.0f;
		fentry31 = 1.0f;
		fentry32 = 0.0f;
		fentry33 = 90.0f;
		fentry34 = 0.0f;
		fentry35 = 1.0f;
		fentry36 = 0.0f;
		fentry37 = 1.0f;
		fentry38 = 0.0f;
		fentry39 = 90.0f;
		fentry40 = 90.0f;
		fentry41 = 90.0f;
		fentry42 = 90.0f;
		fentry43 = 95.0f;
		fentry44 = 90.0f;
		fentry45 = 1.0f;
		fentry46 = 0.0f;
		fentry47 = 90.0f;
		fentry48 = 0.0f;
		fentry49 = 1.0f;
		fentry50 = 0.0f;
		fentry51 = 1.0f;
		fentry52 = 0.0f;
		fentry53 = 90.0f;
		fentry54 = 90.0f;
		fentry55 = 90.0f;
		fentry56 = 90.0f;
		fentry57 = 95.0f;
		fentry58 = 90.0f;
		fentry59 = 1.0f;
		fentry60 = 0.0f;
		fentry61 = 90.0f;
		fentry62 = 0.0f;
		fentry63 = 1.0f;
		fentry64 = 0.0f;
		fentry65 = 1.0f;
		fentry66 = 0.0f;
		fentry67 = 90.0f;
		fentry68 = 90.0f;
		fentry69 = 90.0f;
		fentry70 = 90.0f;
		fentry71 = 0.0f;
		fentry72 = 95.0f;
		fentry73 = 90.0f;
		fentry74 = 1.0f;
		fentry75 = 0.0f;
		fentry76 = 90.0f;
		fentry77 = 0.0f;
		fentry78 = 1.0f;
		fentry79 = 0.0f;
		fentry80 = 1.0f;
		fentry81 = 0.0f;
		fentry82 = 90.0f;
		fentry83 = 90.0f;
		fentry84 = 90.0f;
		fentry85 = 90.0f;
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
		ui_interface->addHorizontalSlider("gain", &fslider0, 0.80000000000000004f, 0.0f, 1.0f, 0.01f);
		ui_interface->addButton("gate", &fbutton0);
		ui_interface->addNumEntry("op0:detune", &fentry7, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL1", &fentry10, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL2", &fentry12, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL3", &fentry2, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egL4", &fentry4, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR1", &fentry11, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR2", &fentry13, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR3", &fentry14, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:egR4", &fentry5, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry9, "unit", "Hz");
		ui_interface->addNumEntry("op0:freq", &fentry9, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op0:key_vel_sens", &fentry3, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op0:level", &fentry1, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op0:mode", &fentry8, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op0:rate_scale", &fentry6, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op1:detune", &fentry21, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL1", &fentry24, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL2", &fentry26, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL3", &fentry16, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egL4", &fentry18, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR1", &fentry25, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR2", &fentry27, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR3", &fentry28, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:egR4", &fentry19, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry23, "unit", "Hz");
		ui_interface->addNumEntry("op1:freq", &fentry23, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op1:key_vel_sens", &fentry17, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op1:level", &fentry15, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op1:mode", &fentry22, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op1:rate_scale", &fentry20, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op2:detune", &fentry35, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL1", &fentry38, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL2", &fentry40, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL3", &fentry30, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egL4", &fentry32, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR1", &fentry39, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR2", &fentry41, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR3", &fentry42, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:egR4", &fentry33, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry37, "unit", "Hz");
		ui_interface->addNumEntry("op2:freq", &fentry37, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op2:key_vel_sens", &fentry31, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op2:level", &fentry29, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op2:mode", &fentry36, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op2:rate_scale", &fentry34, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op3:detune", &fentry78, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL1", &fentry81, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL2", &fentry83, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL3", &fentry73, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egL4", &fentry75, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR1", &fentry82, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR2", &fentry84, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR3", &fentry85, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:egR4", &fentry76, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry80, "unit", "Hz");
		ui_interface->addNumEntry("op3:freq", &fentry80, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op3:key_vel_sens", &fentry74, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op3:level", &fentry72, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op3:mode", &fentry79, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op3:rate_scale", &fentry77, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op4:detune", &fentry49, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL1", &fentry52, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL2", &fentry54, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL3", &fentry44, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egL4", &fentry46, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR1", &fentry53, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR2", &fentry55, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR3", &fentry56, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:egR4", &fentry47, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry51, "unit", "Hz");
		ui_interface->addNumEntry("op4:freq", &fentry51, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op4:key_vel_sens", &fentry45, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op4:level", &fentry43, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op4:mode", &fentry50, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op4:rate_scale", &fentry48, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op5:detune", &fentry63, 1.0f, -10.0f, 10.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL1", &fentry66, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL2", &fentry68, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL3", &fentry58, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egL4", &fentry60, 0.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR1", &fentry67, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR2", &fentry69, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR3", &fentry70, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:egR4", &fentry61, 90.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->declare(&fentry65, "unit", "Hz");
		ui_interface->addNumEntry("op5:freq", &fentry65, 1.0f, 0.0f, 2.0f, 0.01f);
		ui_interface->addNumEntry("op5:key_vel_sens", &fentry59, 1.0f, 0.0f, 8.0f, 1.0f);
		ui_interface->addNumEntry("op5:level", &fentry57, 95.0f, 0.0f, 99.0f, 1.0f);
		ui_interface->addNumEntry("op5:mode", &fentry64, 0.0f, 0.0f, 1.0f, 1.0f);
		ui_interface->addNumEntry("op5:rate_scale", &fentry62, 0.0f, 0.0f, 10.0f, 1.0f);
		ui_interface->addHorizontalSlider("pitch", &fslider1, 48.0f, 24.0f, 84.0f, 0.001f);
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		//zone1
		//zone2
		int 	iSlow0 = int(float(fentry0));
		int 	iSlow1 = float(iSlow0 == 12) != 0.0f;
		float 	fSlow2 = float(fbutton0);
		int 	iSlow3 = fSlow2 > 0.0f;
		int 	iSlow4 = int(iSlow3 > 0);
		float 	fSlow5 = float(fentry1);
		float 	fSlow6 = float(fentry2);
		float 	fSlow7 = float(fslider0);
		float 	fSlow8 = float(fentry3);
		float 	fSlow9 = 0.0012626262626262627f * fSlow7 * fSlow8 + 0.010101010101010102f * (1.0f - 0.125f * fSlow8);
		float 	fSlow10 = fSlow5 * fSlow6 * (fSlow9);
		float 	fSlow11 = float(fentry4);
		float 	fSlow12 = fSlow5 * fSlow11 * (fSlow9);
		int 	iSlow13 = int(fSlow10 <= fSlow12);
		float 	fSlow14 = float(fentry9);
		float 	fSlow15 = powf(2.0f,0.083333333333333329f * (float(fslider1) + -69.0f));
		float 	fSlow16 = 0.25f * float(fentry7) + ((int(float(fentry8)))?fSlow14:440.0f * fSlow15 * fSlow14);
		float 	fSlow17 = 0.057142857142857148f * float(fentry6) * (17.312340490667559f * logf(0.0022727272727272726f * (fSlow16)) + 48.0f);
                float 	fSlow18 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry5) + fSlow17));
                float 	fSlow19 = powf(0.014705882352941176f * float(fSlow10 >= 30.0f) * fmin(fSlow10 + -30.0f, (float)(68)),1.8f);
                float 	fSlow20 = powf(0.010101010101010102f * fmin((float)(99), fSlow10),0.69999999999999996f);
                float 	fSlow21 = powf(0.014705882352941176f * float(fSlow12 >= 30.0f) * fmin(fSlow12 + -30.0f, (float)(68)),1.8f);
                float 	fSlow22 = powf(0.010101010101010102f * fmin((float)(99), fSlow12),0.69999999999999996f);
                float 	fSlow23 = fmax(0.001f, fabsf(((iSlow13) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow18)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow18,12.6f))) * (((iSlow13)?fSlow19:fSlow20) - ((iSlow13)?fSlow21:fSlow22))));
		float 	fSlow24 = fConst0 * fSlow23;
		int 	iSlow25 = float(iSlow0 == 10) != 0.0f;
		int 	iSlow26 = float(iSlow0 == 7) != 0.0f;
		int 	iSlow27 = float(iSlow0 == 4) != 0.0f;
		int 	iSlow28 = float(iSlow0 == 2) != 0.0f;
		int 	iSlow29 = float(iSlow0 == 14) != 0.0f;
		int 	iSlow30 = float(iSlow0 == 9) != 0.0f;
		int 	iSlow31 = float(iSlow0 == 11) != 0.0f;
		int 	iSlow32 = float(iSlow0 == 8) != 0.0f;
		int 	iSlow33 = float(iSlow0 == 6) != 0.0f;
		int 	iSlow34 = float(iSlow0 == 5) != 0.0f;
		int 	iSlow35 = float(iSlow0 == 3) != 0.0f;
		int 	iSlow36 = float(iSlow0 == 1) != 0.0f;
		int 	iSlow37 = float(iSlow0 == 0) != 0.0f;
		int 	iSlow38 = float(iSlow0 == 13) != 0.0f;
		int 	iSlow39 = float(iSlow0 == 15) != 0.0f;
		int 	iSlow40 = int(iSlow3);
		float 	fSlow41 = float(iSlow3);
		int 	iSlow42 = int((fSlow2 == 0.0f) > 0);
		float 	fSlow43 = float(fentry10);
		float 	fSlow44 = fSlow5 * fSlow43 * (fSlow9);
		int 	iSlow45 = int(fSlow44 > fSlow12);
                float 	fSlow46 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry11) + fSlow17));
                float 	fSlow47 = powf(0.014705882352941176f * float(fSlow44 >= 30.0f) * fmin(fSlow44 + -30.0f, (float)(68)),1.8f);
                float 	fSlow48 = powf(0.010101010101010102f * fmin((float)(99), fSlow44),0.69999999999999996f);
                float 	fSlow49 = fmax(0.001f, fabsf(((iSlow45) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow46)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow46,12.6f))) * (((iSlow45)?fSlow21:fSlow22) - ((iSlow45)?fSlow47:fSlow48))));
		float 	fSlow50 = float(fentry12);
		float 	fSlow51 = fSlow5 * fSlow50 * (fSlow9);
		int 	iSlow52 = int(fSlow51 > fSlow44);
                float 	fSlow53 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry13) + fSlow17));
                float 	fSlow54 = powf(0.014705882352941176f * float(fSlow51 >= 30.0f) * fmin(fSlow51 + -30.0f, (float)(68)),1.8f);
                float 	fSlow55 = powf(0.010101010101010102f * fmin((float)(99), fSlow51),0.69999999999999996f);
                float 	fSlow56 = fSlow49 + fmax(0.001f, fabsf(((iSlow52) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow53)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow53,12.6f))) * (((iSlow52)?fSlow54:fSlow55) - ((iSlow52)?fSlow47:fSlow48))));
		int 	iSlow57 = int(fSlow10 > fSlow51);
                float 	fSlow58 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry14) + fSlow17));
                float 	fSlow59 = fSlow56 + fmax(0.001f, fabsf(((iSlow57) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow58)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow58,12.6f))) * (((iSlow57)?fSlow54:fSlow55) - ((iSlow57)?fSlow19:fSlow20))));
		float 	fSlow60 = fConst0 * (fSlow59);
		float 	fSlow61 = fConst0 * (fSlow56);
		int 	iTemp0 = iTempPerm0;
		float 	fSlow62 = fConst0 * fSlow49;
		int 	iTemp1 = iTempPerm1;
		float 	fSlow63 = fSlow5 * (fSlow9);
		float 	fSlow64 = fConst1 * (0 - fSlow63 * (fSlow11 - fSlow43));
		float 	fSlow65 = 0 - fSlow63 * (fSlow43 - fSlow50);
		float 	fSlow66 = 0 - fConst0 * (fSlow49 - (fSlow56));
		float 	fSlow67 = fSlow6 - fSlow50;
		float 	fSlow68 = 0 - fConst0 * ((fSlow56) - (fSlow59));
		float 	fTemp2 = fTempPerm2;
		float 	fSlow69 = fConst1 * (fSlow16);
		float 	fTemp3 = fTempPerm3;
		float 	fSlow70 = float(fentry15);
		float 	fSlow71 = float(fentry16);
		float 	fSlow72 = float(fentry17);
		float 	fSlow73 = 0.0012626262626262627f * fSlow72 * fSlow7 + 0.010101010101010102f * (1.0f - 0.125f * fSlow72);
		float 	fSlow74 = fSlow70 * fSlow71 * (fSlow73);
		float 	fSlow75 = float(fentry18);
		float 	fSlow76 = fSlow70 * fSlow75 * (fSlow73);
		int 	iSlow77 = int(fSlow74 <= fSlow76);
		float 	fSlow78 = float(fentry23);
		float 	fSlow79 = 0.25f * float(fentry21) + ((int(float(fentry22)))?fSlow78:440.0f * fSlow78 * fSlow15);
		float 	fSlow80 = 0.057142857142857148f * float(fentry20) * (17.312340490667559f * logf(0.0022727272727272726f * (fSlow79)) + 48.0f);
                float 	fSlow81 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry19) + fSlow80));
                float 	fSlow82 = powf(0.014705882352941176f * float(fSlow74 >= 30.0f) * fmin(fSlow74 + -30.0f, (float)(68)),1.8f);
                float 	fSlow83 = powf(0.010101010101010102f * fmin((float)(99), fSlow74),0.69999999999999996f);
                float 	fSlow84 = powf(0.014705882352941176f * float(fSlow76 >= 30.0f) * fmin(fSlow76 + -30.0f, (float)(68)),1.8f);
                float 	fSlow85 = powf(0.010101010101010102f * fmin((float)(99), fSlow76),0.69999999999999996f);
                float 	fSlow86 = fmax(0.001f, fabsf(((iSlow77) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow81)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow81,12.6f))) * (((iSlow77)?fSlow82:fSlow83) - ((iSlow77)?fSlow84:fSlow85))));
		float 	fSlow87 = fConst0 * fSlow86;
		float 	fSlow88 = float(fentry24);
		float 	fSlow89 = fSlow70 * fSlow88 * (fSlow73);
		int 	iSlow90 = int(fSlow89 > fSlow76);
                float 	fSlow91 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry25) + fSlow80));
                float 	fSlow92 = powf(0.014705882352941176f * float(fSlow89 >= 30.0f) * fmin(fSlow89 + -30.0f, (float)(68)),1.8f);
                float 	fSlow93 = powf(0.010101010101010102f * fmin((float)(99), fSlow89),0.69999999999999996f);
                float 	fSlow94 = fmax(0.001f, fabsf(((iSlow90) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow91)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow91,12.6f))) * (((iSlow90)?fSlow84:fSlow85) - ((iSlow90)?fSlow92:fSlow93))));
		float 	fSlow95 = float(fentry26);
		float 	fSlow96 = fSlow70 * fSlow95 * (fSlow73);
		int 	iSlow97 = int(fSlow96 > fSlow89);
                float 	fSlow98 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry27) + fSlow80));
                float 	fSlow99 = powf(0.014705882352941176f * float(fSlow96 >= 30.0f) * fmin(fSlow96 + -30.0f, (float)(68)),1.8f);
                float 	fSlow100 = powf(0.010101010101010102f * fmin((float)(99), fSlow96),0.69999999999999996f);
                float 	fSlow101 = fSlow94 + fmax(0.001f, fabsf(((iSlow97) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow98)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow98,12.6f))) * (((iSlow97)?fSlow99:fSlow100) - ((iSlow97)?fSlow92:fSlow93))));
		int 	iSlow102 = int(fSlow74 > fSlow96);
                float 	fSlow103 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry28) + fSlow80));
                float 	fSlow104 = fSlow101 + fmax(0.001f, fabsf(((iSlow102) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow103)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow103,12.6f))) * (((iSlow102)?fSlow99:fSlow100) - ((iSlow102)?fSlow82:fSlow83))));
		float 	fSlow105 = fConst0 * (fSlow104);
		float 	fSlow106 = fConst0 * (fSlow101);
		int 	iTemp4 = iTempPerm4;
		float 	fSlow107 = fConst0 * fSlow94;
		int 	iTemp5 = iTempPerm5;
		float 	fSlow108 = fSlow70 * (fSlow73);
		float 	fSlow109 = fConst1 * (0 - fSlow108 * (fSlow75 - fSlow88));
		float 	fSlow110 = 0 - fSlow108 * (fSlow88 - fSlow95);
		float 	fSlow111 = 0 - fConst0 * (fSlow94 - (fSlow101));
		float 	fSlow112 = fSlow71 - fSlow95;
		float 	fSlow113 = 0 - fConst0 * ((fSlow101) - (fSlow104));
		float 	fTemp6 = fTempPerm6;
		float 	fSlow114 = fConst1 * (fSlow79);
		float 	fTemp7 = fTempPerm7;
		float 	fTemp8 = fTempPerm8;
		float 	fTemp9 = fTempPerm9;
		float 	fTemp10 = fTempPerm10;
		float 	fSlow115 = float(fentry29);
		float 	fSlow116 = float(fentry30);
		float 	fSlow117 = float(fentry31);
		float 	fSlow118 = 0.0012626262626262627f * fSlow7 * fSlow117 + 0.010101010101010102f * (1.0f - 0.125f * fSlow117);
		float 	fSlow119 = fSlow115 * fSlow116 * (fSlow118);
		float 	fSlow120 = float(fentry32);
		float 	fSlow121 = fSlow115 * fSlow120 * (fSlow118);
		int 	iSlow122 = int(fSlow119 <= fSlow121);
		float 	fSlow123 = float(fentry37);
		float 	fSlow124 = 0.25f * float(fentry35) + ((int(float(fentry36)))?fSlow123:440.0f * fSlow15 * fSlow123);
		float 	fSlow125 = 0.057142857142857148f * float(fentry34) * (17.312340490667559f * logf(0.0022727272727272726f * (fSlow124)) + 48.0f);
                float 	fSlow126 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry33) + fSlow125));
                float 	fSlow127 = powf(0.014705882352941176f * float(fSlow119 >= 30.0f) * fmin(fSlow119 + -30.0f, (float)(68)),1.8f);
                float 	fSlow128 = powf(0.010101010101010102f * fmin((float)(99), fSlow119),0.69999999999999996f);
                float 	fSlow129 = powf(0.014705882352941176f * float(fSlow121 >= 30.0f) * fmin(fSlow121 + -30.0f, (float)(68)),1.8f);
                float 	fSlow130 = powf(0.010101010101010102f * fmin((float)(99), fSlow121),0.69999999999999996f);
                float 	fSlow131 = fmax(0.001f, fabsf(((iSlow122) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow126)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow126,12.6f))) * (((iSlow122)?fSlow127:fSlow128) - ((iSlow122)?fSlow129:fSlow130))));
		float 	fSlow132 = fConst0 * fSlow131;
		float 	fSlow133 = float(fentry38);
		float 	fSlow134 = fSlow115 * fSlow133 * (fSlow118);
		int 	iSlow135 = int(fSlow134 > fSlow121);
                float 	fSlow136 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry39) + fSlow125));
                float 	fSlow137 = powf(0.014705882352941176f * float(fSlow134 >= 30.0f) * fmin(fSlow134 + -30.0f, (float)(68)),1.8f);
                float 	fSlow138 = powf(0.010101010101010102f * fmin((float)(99), fSlow134),0.69999999999999996f);
                float 	fSlow139 = fmax(0.001f, fabsf(((iSlow135) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow136)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow136,12.6f))) * (((iSlow135)?fSlow129:fSlow130) - ((iSlow135)?fSlow137:fSlow138))));
		float 	fSlow140 = float(fentry40);
		float 	fSlow141 = fSlow115 * fSlow140 * (fSlow118);
		int 	iSlow142 = int(fSlow141 > fSlow134);
                float 	fSlow143 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry41) + fSlow125));
                float 	fSlow144 = powf(0.014705882352941176f * float(fSlow141 >= 30.0f) * fmin(fSlow141 + -30.0f, (float)(68)),1.8f);
                float 	fSlow145 = powf(0.010101010101010102f * fmin((float)(99), fSlow141),0.69999999999999996f);
                float 	fSlow146 = fSlow139 + fmax(0.001f, fabsf(((iSlow142) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow143)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow143,12.6f))) * (((iSlow142)?fSlow144:fSlow145) - ((iSlow142)?fSlow137:fSlow138))));
		int 	iSlow147 = int(fSlow119 > fSlow141);
                float 	fSlow148 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry42) + fSlow125));
                float 	fSlow149 = fSlow146 + fmax(0.001f, fabsf(((iSlow147) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow148)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow148,12.6f))) * (((iSlow147)?fSlow144:fSlow145) - ((iSlow147)?fSlow127:fSlow128))));
		float 	fSlow150 = fConst0 * (fSlow149);
		float 	fSlow151 = fConst0 * (fSlow146);
		int 	iTemp11 = iTempPerm11;
		float 	fSlow152 = fConst0 * fSlow139;
		int 	iTemp12 = iTempPerm12;
		float 	fSlow153 = fSlow115 * (fSlow118);
		float 	fSlow154 = fConst1 * (0 - fSlow153 * (fSlow120 - fSlow133));
		float 	fSlow155 = fSlow140 - fSlow133;
		float 	fSlow156 = 0 - fConst0 * (fSlow139 - (fSlow146));
		float 	fSlow157 = 0 - fSlow153 * (fSlow140 - fSlow116);
		float 	fSlow158 = 0 - fConst0 * ((fSlow146) - (fSlow149));
		float 	fTemp13 = fTempPerm13;
		float 	fSlow159 = fConst1 * (fSlow124);
		float 	fTemp14 = fTempPerm14;
		float 	fSlow160 = float(fentry43);
		float 	fSlow161 = float(fentry44);
		float 	fSlow162 = float(fentry45);
		float 	fSlow163 = 0.0012626262626262627f * fSlow7 * fSlow162 + 0.010101010101010102f * (1.0f - 0.125f * fSlow162);
		float 	fSlow164 = fSlow160 * fSlow161 * (fSlow163);
		float 	fSlow165 = float(fentry46);
		float 	fSlow166 = fSlow160 * fSlow165 * (fSlow163);
		int 	iSlow167 = int(fSlow164 <= fSlow166);
		float 	fSlow168 = float(fentry51);
		float 	fSlow169 = 0.25f * float(fentry49) + ((int(float(fentry50)))?fSlow168:440.0f * fSlow15 * fSlow168);
		float 	fSlow170 = 0.057142857142857148f * float(fentry48) * (17.312340490667559f * logf(0.0022727272727272726f * (fSlow169)) + 48.0f);
                float 	fSlow171 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry47) + fSlow170));
                float 	fSlow172 = powf(0.014705882352941176f * float(fSlow164 >= 30.0f) * fmin(fSlow164 + -30.0f, (float)(68)),1.8f);
                float 	fSlow173 = powf(0.010101010101010102f * fmin((float)(99), fSlow164),0.69999999999999996f);
                float 	fSlow174 = powf(0.014705882352941176f * float(fSlow166 >= 30.0f) * fmin(fSlow166 + -30.0f, (float)(68)),1.8f);
                float 	fSlow175 = powf(0.010101010101010102f * fmin((float)(99), fSlow166),0.69999999999999996f);
                float 	fSlow176 = fmax(0.001f, fabsf(((iSlow167) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow171)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow171,12.6f))) * (((iSlow167)?fSlow172:fSlow173) - ((iSlow167)?fSlow174:fSlow175))));
		float 	fSlow177 = fConst0 * fSlow176;
		float 	fSlow178 = float(fentry52);
		float 	fSlow179 = fSlow160 * fSlow178 * (fSlow163);
		int 	iSlow180 = int(fSlow179 > fSlow166);
                float 	fSlow181 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry53) + fSlow170));
                float 	fSlow182 = powf(0.014705882352941176f * float(fSlow179 >= 30.0f) * fmin(fSlow179 + -30.0f, (float)(68)),1.8f);
                float 	fSlow183 = powf(0.010101010101010102f * fmin((float)(99), fSlow179),0.69999999999999996f);
                float 	fSlow184 = fmax(0.001f, fabsf(((iSlow180) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow181)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow181,12.6f))) * (((iSlow180)?fSlow174:fSlow175) - ((iSlow180)?fSlow182:fSlow183))));
		float 	fSlow185 = float(fentry54);
		float 	fSlow186 = fSlow160 * fSlow185 * (fSlow163);
		int 	iSlow187 = int(fSlow186 > fSlow179);
                float 	fSlow188 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry55) + fSlow170));
                float 	fSlow189 = powf(0.014705882352941176f * float(fSlow186 >= 30.0f) * fmin(fSlow186 + -30.0f, (float)(68)),1.8f);
                float 	fSlow190 = powf(0.010101010101010102f * fmin((float)(99), fSlow186),0.69999999999999996f);
                float 	fSlow191 = fSlow184 + fmax(0.001f, fabsf(((iSlow187) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow188)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow188,12.6f))) * (((iSlow187)?fSlow189:fSlow190) - ((iSlow187)?fSlow182:fSlow183))));
		int 	iSlow192 = int(fSlow164 > fSlow186);
                float 	fSlow193 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry56) + fSlow170));
                float 	fSlow194 = fSlow191 + fmax(0.001f, fabsf(((iSlow192) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow193)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow193,12.6f))) * (((iSlow192)?fSlow189:fSlow190) - ((iSlow192)?fSlow172:fSlow173))));
		float 	fSlow195 = fConst0 * (fSlow194);
		float 	fSlow196 = fConst0 * (fSlow191);
		int 	iTemp15 = iTempPerm15;
		float 	fSlow197 = fConst0 * fSlow184;
		int 	iTemp16 = iTempPerm16;
		float 	fSlow198 = fSlow160 * (fSlow163);
		float 	fSlow199 = fConst1 * (0 - fSlow198 * (fSlow165 - fSlow178));
		float 	fSlow200 = 0 - fSlow198 * (fSlow178 - fSlow185);
		float 	fSlow201 = 0 - fConst0 * (fSlow184 - (fSlow191));
		float 	fSlow202 = fSlow161 - fSlow185;
		float 	fSlow203 = 0 - fConst0 * ((fSlow191) - (fSlow194));
		float 	fTemp17 = fTempPerm17;
		float 	fSlow204 = fConst1 * (fSlow169);
		float 	fTemp18 = fTempPerm18;
		float 	fTemp19 = fTempPerm19;
		float 	fSlow205 = float(fentry57);
		float 	fSlow206 = float(fentry58);
		float 	fSlow207 = float(fentry59);
		float 	fSlow208 = 0.0012626262626262627f * fSlow7 * fSlow207 + 0.010101010101010102f * (1.0f - 0.125f * fSlow207);
		float 	fSlow209 = fSlow205 * fSlow206 * (fSlow208);
		float 	fSlow210 = float(fentry60);
		float 	fSlow211 = fSlow205 * fSlow210 * (fSlow208);
		int 	iSlow212 = int(fSlow209 <= fSlow211);
		float 	fSlow213 = float(fentry65);
		float 	fSlow214 = 0.25f * float(fentry63) + ((int(float(fentry64)))?fSlow213:440.0f * fSlow15 * fSlow213);
		float 	fSlow215 = 0.057142857142857148f * float(fentry62) * (17.312340490667559f * logf(0.0022727272727272726f * (fSlow214)) + 48.0f);
                float 	fSlow216 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry61) + fSlow215));
                float 	fSlow217 = powf(0.014705882352941176f * float(fSlow209 >= 30.0f) * fmin(fSlow209 + -30.0f, (float)(68)),1.8f);
                float 	fSlow218 = powf(0.010101010101010102f * fmin((float)(99), fSlow209),0.69999999999999996f);
                float 	fSlow219 = powf(0.014705882352941176f * float(fSlow211 >= 30.0f) * fmin(fSlow211 + -30.0f, (float)(68)),1.8f);
                float 	fSlow220 = powf(0.010101010101010102f * fmin((float)(99), fSlow211),0.69999999999999996f);
                float 	fSlow221 = fmax(0.001f, fabsf(((iSlow212) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow216)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow216,12.6f))) * (((iSlow212)?fSlow217:fSlow218) - ((iSlow212)?fSlow219:fSlow220))));
		float 	fSlow222 = fConst0 * fSlow221;
		float 	fSlow223 = float(fentry66);
		float 	fSlow224 = fSlow205 * fSlow223 * (fSlow208);
		int 	iSlow225 = int(fSlow224 > fSlow211);
                float 	fSlow226 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry67) + fSlow215));
                float 	fSlow227 = powf(0.014705882352941176f * float(fSlow224 >= 30.0f) * fmin(fSlow224 + -30.0f, (float)(68)),1.8f);
                float 	fSlow228 = powf(0.010101010101010102f * fmin((float)(99), fSlow224),0.69999999999999996f);
                float 	fSlow229 = fmax(0.001f, fabsf(((iSlow225) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow226)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow226,12.6f))) * (((iSlow225)?fSlow219:fSlow220) - ((iSlow225)?fSlow227:fSlow228))));
		float 	fSlow230 = float(fentry68);
		float 	fSlow231 = fSlow205 * fSlow230 * (fSlow208);
		int 	iSlow232 = int(fSlow231 > fSlow224);
                float 	fSlow233 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry69) + fSlow215));
                float 	fSlow234 = powf(0.014705882352941176f * float(fSlow231 >= 30.0f) * fmin(fSlow231 + -30.0f, (float)(68)),1.8f);
                float 	fSlow235 = powf(0.010101010101010102f * fmin((float)(99), fSlow231),0.69999999999999996f);
                float 	fSlow236 = fSlow229 + fmax(0.001f, fabsf(((iSlow232) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow233)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow233,12.6f))) * (((iSlow232)?fSlow234:fSlow235) - ((iSlow232)?fSlow227:fSlow228))));
		int 	iSlow237 = int(fSlow209 > fSlow231);
                float 	fSlow238 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry70) + fSlow215));
                float 	fSlow239 = fSlow236 + fmax(0.001f, fabsf(((iSlow237) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow238)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow238,12.6f))) * (((iSlow237)?fSlow234:fSlow235) - ((iSlow237)?fSlow217:fSlow218))));
		float 	fSlow240 = fConst0 * (fSlow239);
		float 	fSlow241 = fConst0 * (fSlow236);
		int 	iTemp20 = iTempPerm20;
		float 	fSlow242 = fConst0 * fSlow229;
		int 	iTemp21 = iTempPerm21;
		float 	fSlow243 = fSlow205 * (fSlow208);
		float 	fSlow244 = fConst1 * (0 - fSlow243 * (fSlow210 - fSlow223));
		float 	fSlow245 = 0 - fSlow243 * (fSlow223 - fSlow230);
		float 	fSlow246 = 0 - fConst0 * (fSlow229 - (fSlow236));
		float 	fSlow247 = fSlow206 - fSlow230;
		float 	fSlow248 = 0 - fConst0 * ((fSlow236) - (fSlow239));
		float 	fTemp22 = fTempPerm22;
		float 	fSlow249 = fConst1 * (fSlow214);
		float 	fTemp23 = fTempPerm23;
		float 	fTemp24 = fTempPerm24;
		float 	fSlow250 = float(fentry71);
		float 	fSlow251 = 9126.5810566616474f * fSlow250;
		float 	fSlow252 = float(fentry72);
		float 	fSlow253 = float(fentry73);
		float 	fSlow254 = float(fentry74);
		float 	fSlow255 = 0.0012626262626262627f * fSlow7 * fSlow254 + 0.010101010101010102f * (1.0f - 0.125f * fSlow254);
		float 	fSlow256 = fSlow252 * fSlow253 * (fSlow255);
		float 	fSlow257 = float(fentry75);
		float 	fSlow258 = fSlow252 * fSlow257 * (fSlow255);
		int 	iSlow259 = int(fSlow256 <= fSlow258);
		float 	fSlow260 = float(fentry80);
		float 	fSlow261 = 0.25f * float(fentry78) + ((int(float(fentry79)))?fSlow260:440.0f * fSlow15 * fSlow260);
		float 	fSlow262 = 0.057142857142857148f * float(fentry77) * (17.312340490667559f * logf(0.0022727272727272726f * (fSlow261)) + 48.0f);
                float 	fSlow263 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry76) + fSlow262));
                float 	fSlow264 = powf(0.014705882352941176f * float(fSlow256 >= 30.0f) * fmin(fSlow256 + -30.0f, (float)(68)),1.8f);
                float 	fSlow265 = powf(0.010101010101010102f * fmin((float)(99), fSlow256),0.69999999999999996f);
                float 	fSlow266 = powf(0.014705882352941176f * float(fSlow258 >= 30.0f) * fmin(fSlow258 + -30.0f, (float)(68)),1.8f);
                float 	fSlow267 = powf(0.010101010101010102f * fmin((float)(99), fSlow258),0.69999999999999996f);
                float 	fSlow268 = fmax(0.001f, fabsf(((iSlow259) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow263)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow263,12.6f))) * (((iSlow259)?fSlow264:fSlow265) - ((iSlow259)?fSlow266:fSlow267))));
		float 	fSlow269 = fConst0 * fSlow268;
		float 	fSlow270 = float(fentry81);
		float 	fSlow271 = fSlow252 * fSlow270 * (fSlow255);
		int 	iSlow272 = int(fSlow271 > fSlow258);
                float 	fSlow273 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry82) + fSlow262));
                float 	fSlow274 = powf(0.014705882352941176f * float(fSlow271 >= 30.0f) * fmin(fSlow271 + -30.0f, (float)(68)),1.8f);
                float 	fSlow275 = powf(0.010101010101010102f * fmin((float)(99), fSlow271),0.69999999999999996f);
                float 	fSlow276 = fmax(0.001f, fabsf(((iSlow272) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow273)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow273,12.6f))) * (((iSlow272)?fSlow266:fSlow267) - ((iSlow272)?fSlow274:fSlow275))));
		float 	fSlow277 = float(fentry83);
		float 	fSlow278 = fSlow252 * fSlow277 * (fSlow255);
		int 	iSlow279 = int(fSlow278 > fSlow271);
                float 	fSlow280 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry84) + fSlow262));
                float 	fSlow281 = powf(0.014705882352941176f * float(fSlow278 >= 30.0f) * fmin(fSlow278 + -30.0f, (float)(68)),1.8f);
                float 	fSlow282 = powf(0.010101010101010102f * fmin((float)(99), fSlow278),0.69999999999999996f);
                float 	fSlow283 = fSlow276 + fmax(0.001f, fabsf(((iSlow279) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow280)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow280,12.6f))) * (((iSlow279)?fSlow281:fSlow282) - ((iSlow279)?fSlow274:fSlow275))));
		int 	iSlow284 = int(fSlow256 > fSlow278);
                float 	fSlow285 = 0.007874015748031496f * (127.0f - fmin((float)(99), float(fentry85) + fSlow262));
                float 	fSlow286 = fSlow283 + fmax(0.001f, fabsf(((iSlow284) ?fmax(0.0030000000000000001f, 38.0f * faustpower<12>(fSlow285)) :fmax(0.0080000000000000002f, 318.0f * powf(fSlow285,12.6f))) * (((iSlow284)?fSlow281:fSlow282) - ((iSlow284)?fSlow264:fSlow265))));
		float 	fSlow287 = fConst0 * (fSlow286);
		float 	fSlow288 = fConst0 * (fSlow283);
		int 	iTemp25 = iTempPerm25;
		float 	fSlow289 = fConst0 * fSlow276;
		int 	iTemp26 = iTempPerm26;
		float 	fSlow290 = fSlow252 * (fSlow255);
		float 	fSlow291 = fConst1 * (0 - fSlow290 * (fSlow257 - fSlow270));
		float 	fSlow292 = 0 - fSlow290 * (fSlow270 - fSlow277);
		float 	fSlow293 = 0 - fConst0 * (fSlow276 - (fSlow283));
		float 	fSlow294 = fSlow253 - fSlow277;
		float 	fSlow295 = 0 - fConst0 * ((fSlow283) - (fSlow286));
		float 	fTemp27 = fTempPerm27;
		float 	fSlow296 = fConst1 * (fSlow261);
		float 	fTemp28 = fTempPerm28;
		float 	fTemp29 = fTempPerm29;
		float 	fTemp30 = fTempPerm30;
		float 	fTemp31 = fTempPerm31;
		float 	fSlow297 = 1825.3162113323297f * fSlow250;
		float 	fTemp32 = fTempPerm32;
		float 	fTemp33 = fTempPerm33;
		float 	fTemp34 = fTempPerm34;
		float 	fTemp35 = fTempPerm35;
		float 	fTemp36 = fTempPerm36;
		float 	fTemp37 = fTempPerm37;
		float 	fTemp38 = fTempPerm38;
		float 	fTemp39 = fTempPerm39;
		float 	fSlow298 = 3650.6324226646593f * fSlow250;
		float 	fTemp40 = fTempPerm40;
		float 	fTemp41 = fTempPerm41;
		float 	fTemp42 = fTempPerm42;
		float 	fSlow299 = 912.65810566616483f * fSlow250;
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
                                fRec0[0] = ((iSlow4)?0.0f :fmin(fSlow24, fRec0[1] + 1.0f));
                                fRec2[0] = ((iSlow42)?0.0f :fmin(fSlow60, fRec2[1] + 1.0f));
				iTemp0 = int(fRec2[0] < fSlow61);
				iTemp1 = int(fRec2[0] < fSlow62);
				fRec1[0] = ((iSlow40)?fSlow41 * ((iTemp0)?((iTemp1)?((int(fRec2[0] < 0.0f))?fSlow12:((iTemp1)?fSlow12 + fSlow64 * (fRec2[0] / fSlow49):fSlow44)):((iTemp0)?fSlow44 + (fSlow65) * ((fRec2[0] - fSlow62) / (fSlow66)):fSlow51)):((int(fRec2[0] < fSlow60))?fSlow63 * (fSlow50 + (fSlow67) * ((fRec2[0] - fSlow61) / (fSlow68))):fSlow10)):fRec1[1]);
                                fTemp2 = faustpower<8>(0.01020408163265306f * fmin((float)(98), ((int(fRec0[0] < 0.0f))?fRec1[0]:((int(fRec0[0] < fSlow24))?fRec1[0] + fConst1 * ((fRec0[0] * (fSlow12 - fRec1[0])) / fSlow23):fSlow12))));
				fTemp3 = fRec4[1] + fSlow69;
				fRec4[0] = (fTemp3) - floorf(fTemp3);
                                fRec5[0] = ((iSlow4)?0.0f :fmin(fSlow87, fRec5[1] + 1.0f));
                                fRec7[0] = ((iSlow42)?0.0f :fmin(fSlow105, fRec7[1] + 1.0f));
				iTemp4 = int(fRec7[0] < fSlow106);
				iTemp5 = int(fRec7[0] < fSlow107);
				fRec6[0] = ((iSlow40)?fSlow41 * ((iTemp4)?((iTemp5)?((int(fRec7[0] < 0.0f))?fSlow76:((iTemp5)?fSlow76 + fSlow109 * (fRec7[0] / fSlow94):fSlow89)):((iTemp4)?fSlow89 + (fSlow110) * ((fRec7[0] - fSlow107) / (fSlow111)):fSlow96)):((int(fRec7[0] < fSlow105))?fSlow108 * (fSlow95 + (fSlow112) * ((fRec7[0] - fSlow106) / (fSlow113))):fSlow74)):fRec6[1]);
                                fTemp6 = faustpower<8>(0.01020408163265306f * fmin((float)(98), ((int(fRec5[0] < 0.0f))?fRec6[0]:((int(fRec5[0] < fSlow87))?fRec6[0] + fConst1 * ((fRec5[0] * (fSlow76 - fRec6[0])) / fSlow86):fSlow76))));
				fTemp7 = fSlow114 + fRec8[1];
				fRec8[0] = (fTemp7) - floorf(fTemp7);
			}
			if ((iSlow1 || (iSlow26 || (iSlow27 || (iSlow29 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))) {
				fTemp8 = 65536.0f * fRec8[0];
			}
			if ((iSlow1 || (iSlow26 || (iSlow27 || (iSlow33 || (iSlow34 || (iSlow37 || (iSlow38 || iSlow39)))))))) {
				fTemp9 = fTemp6 * ftbl0[(int(fTemp8) % 65536 + 65536) % 65536];
			}
			if ((iSlow1 || (iSlow26 || (iSlow27 || (iSlow33 || (iSlow34 || (iSlow37 || iSlow38))))))) {
				fTemp10 = fTemp2 * ftbl0[(int(65536.0f * (fRec4[0] + fTemp9)) % 65536 + 65536) % 65536];
			}
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
                                fRec9[0] = ((iSlow4)?0.0f :fmin(fSlow132, fRec9[1] + 1.0f));
                                fRec11[0] = ((iSlow42)?0.0f :fmin(fSlow150, fRec11[1] + 1.0f));
				iTemp11 = int(fRec11[0] < fSlow151);
				iTemp12 = int(fRec11[0] < fSlow152);
				fRec10[0] = ((iSlow40)?fSlow41 * ((iTemp11)?((iTemp12)?((int(fRec11[0] < 0.0f))?fSlow121:((iTemp12)?fSlow121 + fSlow154 * (fRec11[0] / fSlow139):fSlow134)):((iTemp11)?fSlow153 * (fSlow133 + (fSlow155) * ((fRec11[0] - fSlow152) / (fSlow156))):fSlow141)):((int(fRec11[0] < fSlow150))?fSlow141 + (fSlow157) * ((fRec11[0] - fSlow151) / (fSlow158)):fSlow119)):fRec10[1]);
                                fTemp13 = faustpower<8>(0.01020408163265306f * fmin((float)(98), ((int(fRec9[0] < 0.0f))?fRec10[0]:((int(fRec9[0] < fSlow132))?fRec10[0] + fConst1 * ((fRec9[0] * (fSlow121 - fRec10[0])) / fSlow131):fSlow121))));
				fTemp14 = fSlow159 + fRec12[1];
				fRec12[0] = (fTemp14) - floorf(fTemp14);
                                fRec13[0] = ((iSlow4)?0.0f :fmin(fSlow177, fRec13[1] + 1.0f));
                                fRec15[0] = ((iSlow42)?0.0f :fmin(fSlow195, fRec15[1] + 1.0f));
				iTemp15 = int(fRec15[0] < fSlow196);
				iTemp16 = int(fRec15[0] < fSlow197);
				fRec14[0] = ((iSlow40)?fSlow41 * ((iTemp15)?((iTemp16)?((int(fRec15[0] < 0.0f))?fSlow166:((iTemp16)?fSlow166 + fSlow199 * (fRec15[0] / fSlow184):fSlow179)):((iTemp15)?fSlow179 + (fSlow200) * ((fRec15[0] - fSlow197) / (fSlow201)):fSlow186)):((int(fRec15[0] < fSlow195))?fSlow198 * (fSlow185 + (fSlow202) * ((fRec15[0] - fSlow196) / (fSlow203))):fSlow164)):fRec14[1]);
                                fTemp17 = faustpower<8>(0.01020408163265306f * fmin((float)(98), ((int(fRec13[0] < 0.0f))?fRec14[0]:((int(fRec13[0] < fSlow177))?fRec14[0] + fConst1 * ((fRec13[0] * (fSlow166 - fRec14[0])) / fSlow176):fSlow166))));
				fTemp18 = fSlow204 + fRec16[1];
				fRec16[0] = (fTemp18) - floorf(fTemp18);
			}
			if ((iSlow1 || (iSlow25 || (iSlow29 || (iSlow30 || (iSlow31 || iSlow38)))))) {
				fTemp19 = fTemp17 * ftbl0[(int(65536.0f * fRec16[0]) % 65536 + 65536) % 65536];
			}
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
                                fRec18[0] = ((iSlow4)?0.0f :fmin(fSlow222, fRec18[1] + 1.0f));
                                fRec20[0] = ((iSlow42)?0.0f :fmin(fSlow240, fRec20[1] + 1.0f));
				iTemp20 = int(fRec20[0] < fSlow241);
				iTemp21 = int(fRec20[0] < fSlow242);
				fRec19[0] = ((iSlow40)?fSlow41 * ((iTemp20)?((iTemp21)?((int(fRec20[0] < 0.0f))?fSlow211:((iTemp21)?fSlow211 + fSlow244 * (fRec20[0] / fSlow229):fSlow224)):((iTemp20)?fSlow224 + (fSlow245) * ((fRec20[0] - fSlow242) / (fSlow246)):fSlow231)):((int(fRec20[0] < fSlow240))?fSlow243 * (fSlow230 + (fSlow247) * ((fRec20[0] - fSlow241) / (fSlow248))):fSlow209)):fRec19[1]);
                                fTemp22 = faustpower<8>(0.01020408163265306f * fmin((float)(98), ((int(fRec18[0] < 0.0f))?fRec19[0]:((int(fRec18[0] < fSlow222))?fRec19[0] + fConst1 * ((fRec18[0] * (fSlow211 - fRec19[0])) / fSlow221):fSlow211))));
				fTemp23 = fSlow249 + fRec21[1];
				fRec21[0] = (fTemp23) - floorf(fTemp23);
				fTemp24 = 65536.0f * fRec21[0];
			}
			if ((iSlow1 || (iSlow28 || (iSlow33 || (iSlow38 || iSlow39))))) {
				fRec17[0] = fTemp22 * ftbl0[(int(fTemp24 + fSlow251 * fRec17[1]) % 65536 + 65536) % 65536];
			}
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
                                fRec22[0] = ((iSlow4)?0.0f :fmin(fSlow269, fRec22[1] + 1.0f));
                                fRec24[0] = ((iSlow42)?0.0f :fmin(fSlow287, fRec24[1] + 1.0f));
				iTemp25 = int(fRec24[0] < fSlow288);
				iTemp26 = int(fRec24[0] < fSlow289);
				fRec23[0] = ((iSlow40)?fSlow41 * ((iTemp25)?((iTemp26)?((int(fRec24[0] < 0.0f))?fSlow258:((iTemp26)?fSlow258 + fSlow291 * (fRec24[0] / fSlow276):fSlow271)):((iTemp25)?fSlow271 + (fSlow292) * ((fRec24[0] - fSlow289) / (fSlow293)):fSlow278)):((int(fRec24[0] < fSlow287))?fSlow290 * (fSlow277 + (fSlow294) * ((fRec24[0] - fSlow288) / (fSlow295))):fSlow256)):fRec23[1]);
                                fTemp27 = faustpower<8>(0.01020408163265306f * fmin((float)(98), ((int(fRec22[0] < 0.0f))?fRec23[0]:((int(fRec22[0] < fSlow269))?fRec23[0] + fConst1 * ((fRec22[0] * (fSlow258 - fRec23[0])) / fSlow268):fSlow258))));
				fTemp28 = fSlow296 + fRec25[1];
				fRec25[0] = (fTemp28) - floorf(fTemp28);
			}
			if ((iSlow1 || (iSlow26 || (iSlow27 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || iSlow39)))))))) {
				fTemp29 = 65536.0f * fRec25[0];
			}
			if ((iSlow1 || (iSlow27 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || iSlow39))))))) {
				fTemp30 = fTemp27 * ftbl0[(int(fTemp29) % 65536 + 65536) % 65536];
			}
			if (iSlow1) {
				fTemp31 = 2.0879500000000002f * (fTemp10 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp19 + fRec17[0] + fTemp30)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow31 || iSlow36)) {
				fRec26[0] = fTemp6 * ftbl0[(int(fTemp8 + fSlow297 * fRec26[1]) % 65536 + 65536) % 65536];
				fTemp32 = fTemp2 * ftbl0[(int(65536.0f * (fRec4[0] + fRec26[0])) % 65536 + 65536) % 65536];
			}
			if ((iSlow26 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || iSlow36)))))) {
				fTemp33 = fTemp22 * ftbl0[(int(fTemp24) % 65536 + 65536) % 65536];
			}
			if (iSlow31) {
				fTemp34 = 2.0879500000000002f * (fTemp32 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp19 + fTemp33 + fTemp30)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow25 || (iSlow28 || (iSlow30 || iSlow35)))) {
				fTemp35 = 65536.0f * fRec12[0];
			}
			if ((iSlow25 || (iSlow28 || iSlow35))) {
				fTemp36 = fTemp2 * ftbl0[(int(65536.0f * (fRec4[0] + fTemp6 * ftbl0[(int(65536.0f * (fRec8[0] + fTemp13 * ftbl0[(int(fTemp35) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
			}
			if ((iSlow25 || iSlow37)) {
				fRec27[0] = fTemp22 * ftbl0[(int(fTemp24 + fSlow297 * fRec27[1]) % 65536 + 65536) % 65536];
			}
			if (iSlow25) {
				fTemp37 = 2.0879500000000002f * (fTemp36 + fTemp27 * ftbl0[(int(65536.0f * (fRec25[0] + fRec27[0] + fTemp19)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow29 || iSlow30)) {
				fTemp38 = fTemp27 * ftbl0[(int(65536.0f * (fRec25[0] + fTemp33 + fTemp19)) % 65536 + 65536) % 65536];
			}
			if (iSlow30) {
				fRec28[0] = fTemp13 * ftbl0[(int(fTemp35 + fSlow297 * fRec28[1]) % 65536 + 65536) % 65536];
				fTemp39 = 2.0879500000000002f * (fTemp38 + fTemp2 * ftbl0[(int(65536.0f * (fRec4[0] + fTemp6 * ftbl0[(int(65536.0f * (fRec8[0] + fRec28[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if ((iSlow29 || iSlow32)) {
				fRec29[0] = fTemp6 * ftbl0[(int(fTemp8 + fSlow298 * fRec29[1]) % 65536 + 65536) % 65536];
				fTemp40 = fTemp2 * ftbl0[(int(65536.0f * (fRec4[0] + fRec29[0])) % 65536 + 65536) % 65536];
			}
			if ((iSlow26 || (iSlow32 || iSlow36))) {
				fTemp41 = fTemp17 * ftbl0[(int(65536.0f * (fRec16[0] + fTemp33)) % 65536 + 65536) % 65536];
			}
			if (iSlow32) {
				fTemp42 = 2.0879500000000002f * (fTemp40 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp41 + fTemp30)) % 65536 + 65536) % 65536]);
			}
			if (iSlow26) {
				fRec30[0] = fTemp27 * ftbl0[(int(fTemp29 + fSlow299 * fRec30[1]) % 65536 + 65536) % 65536];
				fTemp43 = 2.0879500000000002f * (fTemp10 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fRec30[0] + fTemp41)) % 65536 + 65536) % 65536]);
			}
			if ((iSlow28 || (iSlow33 || iSlow39))) {
				fTemp44 = fTemp17 * ftbl0[(int(65536.0f * (fRec16[0] + fRec17[0])) % 65536 + 65536) % 65536];
			}
			if (iSlow33) {
				fTemp45 = 2.0879500000000002f * (fTemp10 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp44 + fTemp30)) % 65536 + 65536) % 65536]);
			}
			if (iSlow34) {
				fRec31[0] = 2.0879500000000002f * fTemp17 * ftbl0[(int(65536.0f * (fRec16[0] + fTemp22 * ftbl0[(int(fTemp24 + fSlow251 * fRec31[1]) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
			}
			if ((iSlow27 || (iSlow34 || iSlow39))) {
				fTemp46 = fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp30)) % 65536 + 65536) % 65536];
			}
			if ((iSlow27 || iSlow34)) {
				fTemp47 = fTemp10 + fTemp46;
			}
			if (iSlow34) {
				fTemp48 = fRec31[0] + 2.0879500000000002f * (fTemp47);
			}
			if (iSlow27) {
				fRec32[0] = fTemp22 * ftbl0[(int(fTemp24 + fSlow299 * fRec32[1]) % 65536 + 65536) % 65536];
				fTemp49 = 2.0879500000000002f * (fTemp47 + fTemp17 * ftbl0[(int(65536.0f * (fRec16[0] + fRec32[0])) % 65536 + 65536) % 65536]);
			}
			if (iSlow35) {
				fRec33[0] = 2.0879500000000002f * fTemp27 * ftbl0[(int(65536.0f * (fRec25[0] + fTemp17 * ftbl0[(int(65536.0f * (fRec16[0] + fTemp22 * ftbl0[(int(fTemp24 + fSlow251 * fRec33[1]) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536];
				fTemp50 = fRec33[0] + 2.0879500000000002f * fTemp36;
			}
			if (iSlow28) {
				fTemp51 = 2.0879500000000002f * (fTemp36 + fTemp27 * ftbl0[(int(65536.0f * (fRec25[0] + fTemp44)) % 65536 + 65536) % 65536]);
			}
			if (iSlow36) {
				fTemp52 = 2.0879500000000002f * (fTemp32 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp27 * ftbl0[(int(65536.0f * (fRec25[0] + fTemp41)) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow37) {
				fTemp53 = 2.0879500000000002f * (fTemp10 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp27 * ftbl0[(int(65536.0f * (fRec25[0] + fTemp17 * ftbl0[(int(65536.0f * (fRec16[0] + fRec27[0])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow38) {
				fTemp54 = 2.0879500000000002f * (fTemp10 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp27 * ftbl0[(int(65536.0f * (fRec25[0] + fRec17[0] + fTemp19)) % 65536 + 65536) % 65536])) % 65536 + 65536) % 65536]);
			}
			if (iSlow29) {
				fTemp55 = 2.0879500000000002f * (fTemp40 + fTemp13 * ftbl0[(int(65536.0f * (fRec12[0] + fTemp38)) % 65536 + 65536) % 65536]);
			}
			if (iSlow39) {
				fTemp56 = 2.0879500000000002f * fTemp2 * ftbl0[(int(65536.0f * (fRec4[0] + fTemp46 + fTemp9 + fTemp44)) % 65536 + 65536) % 65536];
			}
			output0[i] = (FAUSTFLOAT)(fTemp31 + fTemp34 + fTemp37 + fTemp39 + fTemp42 + fTemp43 + fTemp45 + fTemp48 + fTemp49 + fTemp50 + fTemp51 + fTemp52 + fTemp53 + fTemp54 + fTemp55 + fTemp56);
			// post processing
			if (iSlow35) {
				fRec33[1] = fRec33[0];
			}
			if (iSlow27) {
				fRec32[1] = fRec32[0];
			}
			if (iSlow34) {
				fRec31[1] = fRec31[0];
			}
			if (iSlow26) {
				fRec30[1] = fRec30[0];
			}
			if ((iSlow29 || iSlow32)) {
				fRec29[1] = fRec29[0];
			}
			if (iSlow30) {
				fRec28[1] = fRec28[0];
			}
			if ((iSlow25 || iSlow37)) {
				fRec27[1] = fRec27[0];
			}
			if ((iSlow31 || iSlow36)) {
				fRec26[1] = fRec26[0];
			}
			if ((iSlow1 || (iSlow25 || (iSlow26 || (iSlow27 || (iSlow28 || (iSlow29 || (iSlow30 || (iSlow31 || (iSlow32 || (iSlow33 || (iSlow34 || (iSlow35 || (iSlow36 || (iSlow37 || (iSlow38 || iSlow39)))))))))))))))) {
				fRec25[1] = fRec25[0];
				fRec23[1] = fRec23[0];
				fRec24[1] = fRec24[0];
				fRec22[1] = fRec22[0];
			}
			if ((iSlow1 || (iSlow28 || (iSlow33 || (iSlow38 || iSlow39))))) {
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
		fTempPerm8 = fTemp8;
		fTempPerm9 = fTemp9;
		fTempPerm10 = fTemp10;
		iTempPerm11 = iTemp11;
		iTempPerm12 = iTemp12;
		fTempPerm13 = fTemp13;
		fTempPerm14 = fTemp14;
		iTempPerm15 = iTemp15;
		iTempPerm16 = iTemp16;
		fTempPerm17 = fTemp17;
		fTempPerm18 = fTemp18;
		fTempPerm19 = fTemp19;
		iTempPerm20 = iTemp20;
		iTempPerm21 = iTemp21;
		fTempPerm22 = fTemp22;
		fTempPerm23 = fTemp23;
		fTempPerm24 = fTemp24;
		iTempPerm25 = iTemp25;
		iTempPerm26 = iTemp26;
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
