/* ------------------------------------------------------------
name: "fx.vocoder"
Code generated with Faust 2.37.3 (https://faust.grame.fr)
Compilation options: -a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -es 1 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __fx_vocoder_H__
#define  __fx_vocoder_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN fx_vocoder_dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class fx_vocoder_dsp {

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

class decorator_dsp : public fx_vocoder_dsp {

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

class dsp_factory {
    
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

class ScopedNoDenormals
{
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
/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

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
struct UIReal
{
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

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
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
struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN misc.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <string>


using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

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

static float fx_vocoder_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS fx_vocoder
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class fx_vocoder : public fx_vocoder_dsp {
	
 private:
	
	int fSampleRate;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fConst3;
	float fRec0[3];
	float fRec3[3];
	float fConst4;
	FAUSTFLOAT fHslider1;
	float fConst5;
	float fRec4[2];
	float fConst6;
	FAUSTFLOAT fHslider2;
	float fRec5[2];
	float fRec2[2];
	float fRec1[2];
	float fConst8;
	float fConst9;
	float fRec6[3];
	float fRec9[3];
	float fRec8[2];
	float fRec7[2];
	float fConst11;
	float fConst12;
	float fRec10[3];
	float fRec13[3];
	float fRec12[2];
	float fRec11[2];
	float fConst14;
	float fConst15;
	float fRec14[3];
	float fRec17[3];
	float fRec16[2];
	float fRec15[2];
	float fConst17;
	float fConst18;
	float fRec18[3];
	float fRec21[3];
	float fRec20[2];
	float fRec19[2];
	float fConst20;
	float fConst21;
	float fRec22[3];
	float fRec25[3];
	float fRec24[2];
	float fRec23[2];
	float fConst23;
	float fConst24;
	float fRec26[3];
	float fRec29[3];
	float fRec28[2];
	float fRec27[2];
	float fConst26;
	float fConst27;
	float fRec30[3];
	float fRec33[3];
	float fRec32[2];
	float fRec31[2];
	float fConst29;
	float fConst30;
	float fRec34[3];
	float fRec37[3];
	float fRec36[2];
	float fRec35[2];
	float fConst32;
	float fConst33;
	float fRec38[3];
	float fRec41[3];
	float fRec40[2];
	float fRec39[2];
	float fConst35;
	float fConst36;
	float fRec42[3];
	float fRec45[3];
	float fRec44[2];
	float fRec43[2];
	float fConst38;
	float fConst39;
	float fRec46[3];
	float fRec49[3];
	float fRec48[2];
	float fRec47[2];
	float fConst41;
	float fConst42;
	float fRec50[3];
	float fRec53[3];
	float fRec52[2];
	float fRec51[2];
	float fConst44;
	float fConst45;
	float fRec54[3];
	float fRec57[3];
	float fRec56[2];
	float fRec55[2];
	float fConst47;
	float fConst48;
	float fRec58[3];
	float fRec61[3];
	float fRec60[2];
	float fRec59[2];
	float fConst50;
	float fConst51;
	float fRec62[3];
	float fRec65[3];
	float fRec64[2];
	float fRec63[2];
	float fConst53;
	float fConst54;
	float fRec66[3];
	float fRec69[3];
	float fRec68[2];
	float fRec67[2];
	float fConst56;
	float fConst57;
	float fRec70[3];
	float fRec73[3];
	float fRec72[2];
	float fRec71[2];
	float fConst59;
	float fConst60;
	float fRec74[3];
	float fRec77[3];
	float fRec76[2];
	float fRec75[2];
	float fConst62;
	float fConst63;
	float fRec78[3];
	float fRec81[3];
	float fRec80[2];
	float fRec79[2];
	float fConst65;
	float fConst66;
	float fRec82[3];
	float fRec85[3];
	float fRec84[2];
	float fRec83[2];
	float fConst68;
	float fConst69;
	float fRec86[3];
	float fRec89[3];
	float fRec88[2];
	float fRec87[2];
	float fConst71;
	float fConst72;
	float fRec90[3];
	float fRec93[3];
	float fRec92[2];
	float fRec91[2];
	float fConst74;
	float fConst75;
	float fRec94[3];
	float fRec97[3];
	float fRec96[2];
	float fRec95[2];
	float fConst77;
	float fConst78;
	float fRec98[3];
	float fRec101[3];
	float fRec100[2];
	float fRec99[2];
	float fConst80;
	float fConst81;
	float fRec102[3];
	float fRec105[3];
	float fRec104[2];
	float fRec103[2];
	float fConst83;
	float fConst84;
	float fRec106[3];
	float fRec109[3];
	float fRec108[2];
	float fRec107[2];
	float fConst86;
	float fConst87;
	float fRec110[3];
	float fRec113[3];
	float fRec112[2];
	float fRec111[2];
	float fConst89;
	float fConst90;
	float fRec114[3];
	float fRec117[3];
	float fRec116[2];
	float fRec115[2];
	float fConst92;
	float fConst93;
	float fRec118[3];
	float fRec121[3];
	float fRec120[2];
	float fRec119[2];
	float fConst95;
	float fConst96;
	float fRec122[3];
	float fRec125[3];
	float fRec124[2];
	float fRec123[2];
	float fConst98;
	float fConst99;
	float fRec126[3];
	float fRec129[3];
	float fRec128[2];
	float fRec127[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.1");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("compile_options", "-a /Users/serge/work/music/pure-data/ceammc/faust/ceammc_dsp_ext.cpp -lang cpp -es 1 -single -ftz 0");
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
		m->declare("signals.lib/version", "0.1");
		m->declare("vaeffects.lib/name", "Faust Virtual Analog Filter Effect Library");
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
		float fConst1 = std::tan((40212.3867f / fConst0));
		fConst2 = (1.0f / fConst1);
		fConst3 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst1))));
		fConst4 = (0.0441000015f / fConst0);
		fConst5 = (1.0f - (44.0999985f / fConst0));
		fConst6 = (1.0f / fConst0);
		float fConst7 = std::tan((33089.8789f / fConst0));
		fConst8 = (1.0f / fConst7);
		fConst9 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst7))));
		float fConst10 = std::tan((27228.9238f / fConst0));
		fConst11 = (1.0f / fConst10);
		fConst12 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst10))));
		float fConst13 = std::tan((18437.4609f / fConst0));
		fConst14 = (1.0f / fConst13);
		fConst15 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst13))));
		float fConst16 = std::tan((15171.7754f / fConst0));
		fConst17 = (1.0f / fConst16);
		fConst18 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst16))));
		float fConst19 = std::tan((12484.5166f / fConst0));
		fConst20 = (1.0f / fConst19);
		fConst21 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst19))));
		float fConst22 = std::tan((10273.2305f / fConst0));
		fConst23 = (1.0f / fConst22);
		fConst24 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst22))));
		float fConst25 = std::tan((8453.61328f / fConst0));
		fConst26 = (1.0f / fConst25);
		fConst27 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst25))));
		float fConst28 = std::tan((6956.28955f / fConst0));
		fConst29 = (1.0f / fConst28);
		fConst30 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst28))));
		float fConst31 = std::tan((4710.29688f / fConst0));
		fConst32 = (1.0f / fConst31);
		fConst33 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst31))));
		float fConst34 = std::tan((3875.99854f / fConst0));
		fConst35 = (1.0f / fConst34);
		fConst36 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst34))));
		float fConst37 = std::tan((3189.4729f / fConst0));
		fConst38 = (1.0f / fConst37);
		fConst39 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst37))));
		float fConst40 = std::tan((2624.54639f / fConst0));
		fConst41 = (1.0f / fConst40);
		fConst42 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst40))));
		float fConst43 = std::tan((2159.68066f / fConst0));
		fConst44 = (1.0f / fConst43);
		fConst45 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst43))));
		float fConst46 = std::tan((1777.1532f / fConst0));
		fConst47 = (1.0f / fConst46);
		fConst48 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst46))));
		float fConst49 = std::tan((1462.37976f / fConst0));
		fConst50 = (1.0f / fConst49);
		fConst51 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst49))));
		float fConst52 = std::tan((171.296555f / fConst0));
		fConst53 = (1.0f / fConst52);
		fConst54 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst52))));
		float fConst55 = std::tan((140.956116f / fConst0));
		fConst56 = (1.0f / fConst55);
		fConst57 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst55))));
		float fConst58 = std::tan((115.989655f / fConst0));
		fConst59 = (1.0f / fConst58);
		fConst60 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst58))));
		float fConst61 = std::tan((95.4453049f / fConst0));
		fConst62 = (1.0f / fConst61);
		fConst63 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst61))));
		float fConst64 = std::tan((22406.0742f / fConst0));
		fConst65 = (1.0f / fConst64);
		fConst66 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst64))));
		float fConst67 = std::tan((5724.17578f / fConst0));
		fConst68 = (1.0f / fConst67);
		fConst69 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst67))));
		float fConst70 = std::tan((1203.35974f / fConst0));
		fConst71 = (1.0f / fConst70);
		fConst72 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst70))));
		float fConst73 = std::tan((990.217957f / fConst0));
		fConst74 = (1.0f / fConst73);
		fConst75 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst73))));
		float fConst76 = std::tan((814.828308f / fConst0));
		fConst77 = (1.0f / fConst76);
		fConst78 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst76))));
		float fConst79 = std::tan((670.504089f / fConst0));
		fConst80 = (1.0f / fConst79);
		fConst81 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst79))));
		float fConst82 = std::tan((551.742859f / fConst0));
		fConst83 = (1.0f / fConst82);
		fConst84 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst82))));
		float fConst85 = std::tan((454.016937f / fConst0));
		fConst86 = (1.0f / fConst85);
		fConst87 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst85))));
		float fConst88 = std::tan((373.600433f / fConst0));
		fConst89 = (1.0f / fConst88);
		fConst90 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst88))));
		float fConst91 = std::tan((307.42749f / fConst0));
		fConst92 = (1.0f / fConst91);
		fConst93 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst91))));
		float fConst94 = std::tan((252.975235f / fConst0));
		fConst95 = (1.0f / fConst94);
		fConst96 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst94))));
		float fConst97 = std::tan((208.167679f / fConst0));
		fConst98 = (1.0f / fConst97);
		fConst99 = (2.0f * (1.0f - (1.0f / fx_vocoder_faustpower2_f(fConst97))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(5.0f);
		fHslider2 = FAUSTFLOAT(5.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
			fRec3[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec4[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec5[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec2[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec1[l5] = 0.0f;
		}
		for (int l6 = 0; (l6 < 3); l6 = (l6 + 1)) {
			fRec6[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 3); l7 = (l7 + 1)) {
			fRec9[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec8[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec7[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec10[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec13[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec12[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec14[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec17[l15] = 0.0f;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec16[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec15[l17] = 0.0f;
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec18[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec21[l19] = 0.0f;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec20[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec19[l21] = 0.0f;
		}
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec22[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec25[l23] = 0.0f;
		}
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec24[l24] = 0.0f;
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec23[l25] = 0.0f;
		}
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec26[l26] = 0.0f;
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec29[l27] = 0.0f;
		}
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec28[l28] = 0.0f;
		}
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec27[l29] = 0.0f;
		}
		for (int l30 = 0; (l30 < 3); l30 = (l30 + 1)) {
			fRec30[l30] = 0.0f;
		}
		for (int l31 = 0; (l31 < 3); l31 = (l31 + 1)) {
			fRec33[l31] = 0.0f;
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec32[l32] = 0.0f;
		}
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec31[l33] = 0.0f;
		}
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec34[l34] = 0.0f;
		}
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec37[l35] = 0.0f;
		}
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec36[l36] = 0.0f;
		}
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec35[l37] = 0.0f;
		}
		for (int l38 = 0; (l38 < 3); l38 = (l38 + 1)) {
			fRec38[l38] = 0.0f;
		}
		for (int l39 = 0; (l39 < 3); l39 = (l39 + 1)) {
			fRec41[l39] = 0.0f;
		}
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec40[l40] = 0.0f;
		}
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec39[l41] = 0.0f;
		}
		for (int l42 = 0; (l42 < 3); l42 = (l42 + 1)) {
			fRec42[l42] = 0.0f;
		}
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec45[l43] = 0.0f;
		}
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec44[l44] = 0.0f;
		}
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec43[l45] = 0.0f;
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec46[l46] = 0.0f;
		}
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec49[l47] = 0.0f;
		}
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec48[l48] = 0.0f;
		}
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec47[l49] = 0.0f;
		}
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec50[l50] = 0.0f;
		}
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec53[l51] = 0.0f;
		}
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec52[l52] = 0.0f;
		}
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec51[l53] = 0.0f;
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec54[l54] = 0.0f;
		}
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec57[l55] = 0.0f;
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec56[l56] = 0.0f;
		}
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec55[l57] = 0.0f;
		}
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec58[l58] = 0.0f;
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec61[l59] = 0.0f;
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec60[l60] = 0.0f;
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec59[l61] = 0.0f;
		}
		for (int l62 = 0; (l62 < 3); l62 = (l62 + 1)) {
			fRec62[l62] = 0.0f;
		}
		for (int l63 = 0; (l63 < 3); l63 = (l63 + 1)) {
			fRec65[l63] = 0.0f;
		}
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec64[l64] = 0.0f;
		}
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec63[l65] = 0.0f;
		}
		for (int l66 = 0; (l66 < 3); l66 = (l66 + 1)) {
			fRec66[l66] = 0.0f;
		}
		for (int l67 = 0; (l67 < 3); l67 = (l67 + 1)) {
			fRec69[l67] = 0.0f;
		}
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec68[l68] = 0.0f;
		}
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec67[l69] = 0.0f;
		}
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec70[l70] = 0.0f;
		}
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec73[l71] = 0.0f;
		}
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec72[l72] = 0.0f;
		}
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec71[l73] = 0.0f;
		}
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec74[l74] = 0.0f;
		}
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec77[l75] = 0.0f;
		}
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec76[l76] = 0.0f;
		}
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec75[l77] = 0.0f;
		}
		for (int l78 = 0; (l78 < 3); l78 = (l78 + 1)) {
			fRec78[l78] = 0.0f;
		}
		for (int l79 = 0; (l79 < 3); l79 = (l79 + 1)) {
			fRec81[l79] = 0.0f;
		}
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec80[l80] = 0.0f;
		}
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec79[l81] = 0.0f;
		}
		for (int l82 = 0; (l82 < 3); l82 = (l82 + 1)) {
			fRec82[l82] = 0.0f;
		}
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec85[l83] = 0.0f;
		}
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec84[l84] = 0.0f;
		}
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fRec83[l85] = 0.0f;
		}
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec86[l86] = 0.0f;
		}
		for (int l87 = 0; (l87 < 3); l87 = (l87 + 1)) {
			fRec89[l87] = 0.0f;
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec88[l88] = 0.0f;
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec87[l89] = 0.0f;
		}
		for (int l90 = 0; (l90 < 3); l90 = (l90 + 1)) {
			fRec90[l90] = 0.0f;
		}
		for (int l91 = 0; (l91 < 3); l91 = (l91 + 1)) {
			fRec93[l91] = 0.0f;
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec92[l92] = 0.0f;
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec91[l93] = 0.0f;
		}
		for (int l94 = 0; (l94 < 3); l94 = (l94 + 1)) {
			fRec94[l94] = 0.0f;
		}
		for (int l95 = 0; (l95 < 3); l95 = (l95 + 1)) {
			fRec97[l95] = 0.0f;
		}
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec96[l96] = 0.0f;
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec95[l97] = 0.0f;
		}
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec98[l98] = 0.0f;
		}
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec101[l99] = 0.0f;
		}
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			fRec100[l100] = 0.0f;
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec99[l101] = 0.0f;
		}
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec102[l102] = 0.0f;
		}
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec105[l103] = 0.0f;
		}
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec104[l104] = 0.0f;
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec103[l105] = 0.0f;
		}
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec106[l106] = 0.0f;
		}
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec109[l107] = 0.0f;
		}
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			fRec108[l108] = 0.0f;
		}
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec107[l109] = 0.0f;
		}
		for (int l110 = 0; (l110 < 3); l110 = (l110 + 1)) {
			fRec110[l110] = 0.0f;
		}
		for (int l111 = 0; (l111 < 3); l111 = (l111 + 1)) {
			fRec113[l111] = 0.0f;
		}
		for (int l112 = 0; (l112 < 2); l112 = (l112 + 1)) {
			fRec112[l112] = 0.0f;
		}
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fRec111[l113] = 0.0f;
		}
		for (int l114 = 0; (l114 < 3); l114 = (l114 + 1)) {
			fRec114[l114] = 0.0f;
		}
		for (int l115 = 0; (l115 < 3); l115 = (l115 + 1)) {
			fRec117[l115] = 0.0f;
		}
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			fRec116[l116] = 0.0f;
		}
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fRec115[l117] = 0.0f;
		}
		for (int l118 = 0; (l118 < 3); l118 = (l118 + 1)) {
			fRec118[l118] = 0.0f;
		}
		for (int l119 = 0; (l119 < 3); l119 = (l119 + 1)) {
			fRec121[l119] = 0.0f;
		}
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec120[l120] = 0.0f;
		}
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec119[l121] = 0.0f;
		}
		for (int l122 = 0; (l122 < 3); l122 = (l122 + 1)) {
			fRec122[l122] = 0.0f;
		}
		for (int l123 = 0; (l123 < 3); l123 = (l123 + 1)) {
			fRec125[l123] = 0.0f;
		}
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			fRec124[l124] = 0.0f;
		}
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			fRec123[l125] = 0.0f;
		}
		for (int l126 = 0; (l126 < 3); l126 = (l126 + 1)) {
			fRec126[l126] = 0.0f;
		}
		for (int l127 = 0; (l127 < 3); l127 = (l127 + 1)) {
			fRec129[l127] = 0.0f;
		}
		for (int l128 = 0; (l128 < 2); l128 = (l128 + 1)) {
			fRec128[l128] = 0.0f;
		}
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			fRec127[l129] = 0.0f;
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
		ui_interface->declare(&fHslider1, "unit", "ms");
		ui_interface->addHorizontalSlider("attack", &fHslider1, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(100.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("bwratio", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->declare(&fHslider2, "unit", "ms");
		ui_interface->addHorizontalSlider("release", &fHslider2, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.100000001f), FAUSTFLOAT(100.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = (0.177122265f * fSlow0);
		float fSlow2 = ((fConst2 * (fConst2 + fSlow1)) + 1.0f);
		float fSlow3 = (1.0f / fSlow2);
		float fSlow4 = ((fConst2 * (fConst2 - fSlow1)) + 1.0f);
		float fSlow5 = (fConst2 / fSlow2);
		float fSlow6 = (0.0f - fSlow5);
		float fSlow7 = (fConst4 * float(fHslider1));
		float fSlow8 = (fConst4 * float(fHslider2));
		float fSlow9 = (0.177122265f * fSlow0);
		float fSlow10 = ((fConst8 * (fConst8 + fSlow9)) + 1.0f);
		float fSlow11 = (1.0f / fSlow10);
		float fSlow12 = ((fConst8 * (fConst8 - fSlow9)) + 1.0f);
		float fSlow13 = (fConst8 / fSlow10);
		float fSlow14 = (0.0f - fSlow13);
		float fSlow15 = (0.177122265f * fSlow0);
		float fSlow16 = ((fConst11 * (fConst11 + fSlow15)) + 1.0f);
		float fSlow17 = (1.0f / fSlow16);
		float fSlow18 = ((fConst11 * (fConst11 - fSlow15)) + 1.0f);
		float fSlow19 = (fConst11 / fSlow16);
		float fSlow20 = (0.0f - fSlow19);
		float fSlow21 = (0.177122265f * fSlow0);
		float fSlow22 = ((fConst14 * (fConst14 + fSlow21)) + 1.0f);
		float fSlow23 = (1.0f / fSlow22);
		float fSlow24 = ((fConst14 * (fConst14 - fSlow21)) + 1.0f);
		float fSlow25 = (fConst14 / fSlow22);
		float fSlow26 = (0.0f - fSlow25);
		float fSlow27 = (0.177122265f * fSlow0);
		float fSlow28 = ((fConst17 * (fConst17 + fSlow27)) + 1.0f);
		float fSlow29 = (1.0f / fSlow28);
		float fSlow30 = ((fConst17 * (fConst17 - fSlow27)) + 1.0f);
		float fSlow31 = (fConst17 / fSlow28);
		float fSlow32 = (0.0f - fSlow31);
		float fSlow33 = (0.177122265f * fSlow0);
		float fSlow34 = ((fConst20 * (fConst20 + fSlow33)) + 1.0f);
		float fSlow35 = (1.0f / fSlow34);
		float fSlow36 = ((fConst20 * (fConst20 - fSlow33)) + 1.0f);
		float fSlow37 = (fConst20 / fSlow34);
		float fSlow38 = (0.0f - fSlow37);
		float fSlow39 = (0.177122265f * fSlow0);
		float fSlow40 = ((fConst23 * (fConst23 + fSlow39)) + 1.0f);
		float fSlow41 = (1.0f / fSlow40);
		float fSlow42 = ((fConst23 * (fConst23 - fSlow39)) + 1.0f);
		float fSlow43 = (fConst23 / fSlow40);
		float fSlow44 = (0.0f - fSlow43);
		float fSlow45 = ((fConst26 * (fConst26 + fSlow1)) + 1.0f);
		float fSlow46 = (1.0f / fSlow45);
		float fSlow47 = ((fConst26 * (fConst26 - fSlow1)) + 1.0f);
		float fSlow48 = (fConst26 / fSlow45);
		float fSlow49 = (0.0f - fSlow48);
		float fSlow50 = (0.177122265f * fSlow0);
		float fSlow51 = ((fConst29 * (fConst29 + fSlow50)) + 1.0f);
		float fSlow52 = (1.0f / fSlow51);
		float fSlow53 = ((fConst29 * (fConst29 - fSlow50)) + 1.0f);
		float fSlow54 = (fConst29 / fSlow51);
		float fSlow55 = (0.0f - fSlow54);
		float fSlow56 = ((fConst32 * (fConst32 + fSlow50)) + 1.0f);
		float fSlow57 = (1.0f / fSlow56);
		float fSlow58 = ((fConst32 * (fConst32 - fSlow50)) + 1.0f);
		float fSlow59 = (fConst32 / fSlow56);
		float fSlow60 = (0.0f - fSlow59);
		float fSlow61 = ((fConst35 * (fConst35 + fSlow15)) + 1.0f);
		float fSlow62 = (1.0f / fSlow61);
		float fSlow63 = ((fConst35 * (fConst35 - fSlow15)) + 1.0f);
		float fSlow64 = (fConst35 / fSlow61);
		float fSlow65 = (0.0f - fSlow64);
		float fSlow66 = (0.177122265f * fSlow0);
		float fSlow67 = ((fConst38 * (fConst38 + fSlow66)) + 1.0f);
		float fSlow68 = (1.0f / fSlow67);
		float fSlow69 = ((fConst38 * (fConst38 - fSlow66)) + 1.0f);
		float fSlow70 = (fConst38 / fSlow67);
		float fSlow71 = (0.0f - fSlow70);
		float fSlow72 = (0.177122265f * fSlow0);
		float fSlow73 = ((fConst41 * (fConst41 + fSlow72)) + 1.0f);
		float fSlow74 = (1.0f / fSlow73);
		float fSlow75 = ((fConst41 * (fConst41 - fSlow72)) + 1.0f);
		float fSlow76 = (fConst41 / fSlow73);
		float fSlow77 = (0.0f - fSlow76);
		float fSlow78 = (0.177122265f * fSlow0);
		float fSlow79 = ((fConst44 * (fConst44 + fSlow78)) + 1.0f);
		float fSlow80 = (1.0f / fSlow79);
		float fSlow81 = ((fConst44 * (fConst44 - fSlow78)) + 1.0f);
		float fSlow82 = (fConst44 / fSlow79);
		float fSlow83 = (0.0f - fSlow82);
		float fSlow84 = ((fConst47 * (fConst47 + fSlow39)) + 1.0f);
		float fSlow85 = (1.0f / fSlow84);
		float fSlow86 = ((fConst47 * (fConst47 - fSlow39)) + 1.0f);
		float fSlow87 = (fConst47 / fSlow84);
		float fSlow88 = (0.0f - fSlow87);
		float fSlow89 = ((fConst50 * (fConst50 + fSlow39)) + 1.0f);
		float fSlow90 = (1.0f / fSlow89);
		float fSlow91 = ((fConst50 * (fConst50 - fSlow39)) + 1.0f);
		float fSlow92 = (fConst50 / fSlow89);
		float fSlow93 = (0.0f - fSlow92);
		float fSlow94 = ((fConst53 * (fConst53 + fSlow39)) + 1.0f);
		float fSlow95 = (1.0f / fSlow94);
		float fSlow96 = ((fConst53 * (fConst53 - fSlow39)) + 1.0f);
		float fSlow97 = (fConst53 / fSlow94);
		float fSlow98 = (0.0f - fSlow97);
		float fSlow99 = ((fConst56 * (fConst56 + fSlow33)) + 1.0f);
		float fSlow100 = (1.0f / fSlow99);
		float fSlow101 = ((fConst56 * (fConst56 - fSlow33)) + 1.0f);
		float fSlow102 = (fConst56 / fSlow99);
		float fSlow103 = (0.0f - fSlow102);
		float fSlow104 = ((fConst59 * (fConst59 + fSlow78)) + 1.0f);
		float fSlow105 = (1.0f / fSlow104);
		float fSlow106 = ((fConst59 * (fConst59 - fSlow78)) + 1.0f);
		float fSlow107 = (fConst59 / fSlow104);
		float fSlow108 = (0.0f - fSlow107);
		float fSlow109 = ((fConst62 * (fConst62 + fSlow33)) + 1.0f);
		float fSlow110 = (1.0f / fSlow109);
		float fSlow111 = ((fConst62 * (fConst62 - fSlow33)) + 1.0f);
		float fSlow112 = (fConst62 / fSlow109);
		float fSlow113 = (0.0f - fSlow112);
		float fSlow114 = ((fConst65 * (fConst65 + fSlow33)) + 1.0f);
		float fSlow115 = (1.0f / fSlow114);
		float fSlow116 = ((fConst65 * (fConst65 - fSlow33)) + 1.0f);
		float fSlow117 = (fConst65 / fSlow114);
		float fSlow118 = (0.0f - fSlow117);
		float fSlow119 = ((fConst68 * (fConst68 + fSlow15)) + 1.0f);
		float fSlow120 = (1.0f / fSlow119);
		float fSlow121 = ((fConst68 * (fConst68 - fSlow15)) + 1.0f);
		float fSlow122 = (fConst68 / fSlow119);
		float fSlow123 = (0.0f - fSlow122);
		float fSlow124 = ((fConst71 * (fConst71 + fSlow15)) + 1.0f);
		float fSlow125 = (1.0f / fSlow124);
		float fSlow126 = ((fConst71 * (fConst71 - fSlow15)) + 1.0f);
		float fSlow127 = (fConst71 / fSlow124);
		float fSlow128 = (0.0f - fSlow127);
		float fSlow129 = ((fConst74 * (fConst74 + fSlow27)) + 1.0f);
		float fSlow130 = (1.0f / fSlow129);
		float fSlow131 = ((fConst74 * (fConst74 - fSlow27)) + 1.0f);
		float fSlow132 = (fConst74 / fSlow129);
		float fSlow133 = (0.0f - fSlow132);
		float fSlow134 = (0.177122265f * fSlow0);
		float fSlow135 = ((fConst77 * (fConst77 + fSlow134)) + 1.0f);
		float fSlow136 = (1.0f / fSlow135);
		float fSlow137 = ((fConst77 * (fConst77 - fSlow134)) + 1.0f);
		float fSlow138 = (fConst77 / fSlow135);
		float fSlow139 = (0.0f - fSlow138);
		float fSlow140 = ((fConst80 * (fConst80 + fSlow27)) + 1.0f);
		float fSlow141 = (1.0f / fSlow140);
		float fSlow142 = ((fConst80 * (fConst80 - fSlow27)) + 1.0f);
		float fSlow143 = (fConst80 / fSlow140);
		float fSlow144 = (0.0f - fSlow143);
		float fSlow145 = ((fConst83 * (fConst83 + fSlow78)) + 1.0f);
		float fSlow146 = (1.0f / fSlow145);
		float fSlow147 = ((fConst83 * (fConst83 - fSlow78)) + 1.0f);
		float fSlow148 = (fConst83 / fSlow145);
		float fSlow149 = (0.0f - fSlow148);
		float fSlow150 = ((fConst86 * (fConst86 + fSlow66)) + 1.0f);
		float fSlow151 = (1.0f / fSlow150);
		float fSlow152 = ((fConst86 * (fConst86 - fSlow66)) + 1.0f);
		float fSlow153 = (fConst86 / fSlow150);
		float fSlow154 = (0.0f - fSlow153);
		float fSlow155 = ((fConst89 * (fConst89 + fSlow33)) + 1.0f);
		float fSlow156 = (1.0f / fSlow155);
		float fSlow157 = ((fConst89 * (fConst89 - fSlow33)) + 1.0f);
		float fSlow158 = (fConst89 / fSlow155);
		float fSlow159 = (0.0f - fSlow158);
		float fSlow160 = ((fConst92 * (fConst92 + fSlow50)) + 1.0f);
		float fSlow161 = (1.0f / fSlow160);
		float fSlow162 = ((fConst92 * (fConst92 - fSlow50)) + 1.0f);
		float fSlow163 = (fConst92 / fSlow160);
		float fSlow164 = (0.0f - fSlow163);
		float fSlow165 = (0.177122265f * fSlow0);
		float fSlow166 = ((fConst95 * (fConst95 + fSlow165)) + 1.0f);
		float fSlow167 = (1.0f / fSlow166);
		float fSlow168 = ((fConst95 * (fConst95 - fSlow165)) + 1.0f);
		float fSlow169 = (fConst95 / fSlow166);
		float fSlow170 = (0.0f - fSlow169);
		float fSlow171 = ((fConst98 * (fConst98 + fSlow66)) + 1.0f);
		float fSlow172 = (fConst98 / fSlow171);
		float fSlow173 = (1.0f / fSlow171);
		float fSlow174 = ((fConst98 * (fConst98 - fSlow66)) + 1.0f);
		float fSlow175 = (0.0f - fSlow172);
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = float(input1[i0]);
			fRec0[0] = (fTemp0 - (fSlow3 * ((fSlow4 * fRec0[2]) + (fConst3 * fRec0[1]))));
			float fTemp1 = float(input0[i0]);
			fRec3[0] = (fTemp1 - (fSlow3 * ((fSlow4 * fRec3[2]) + (fConst3 * fRec3[1]))));
			float fTemp2 = std::fabs(((fSlow5 * fRec3[0]) + (fSlow6 * fRec3[2])));
			fRec4[0] = (fSlow7 + (fConst5 * fRec4[1]));
			int iTemp3 = (std::fabs(fRec4[0]) < 1.1920929e-07f);
			float fThen1 = std::exp((0.0f - (fConst6 / (iTemp3 ? 1.0f : fRec4[0]))));
			float fTemp4 = (iTemp3 ? 0.0f : fThen1);
			fRec5[0] = (fSlow8 + (fConst5 * fRec5[1]));
			int iTemp5 = (std::fabs(fRec5[0]) < 1.1920929e-07f);
			float fThen3 = std::exp((0.0f - (fConst6 / (iTemp5 ? 1.0f : fRec5[0]))));
			float fTemp6 = (iTemp5 ? 0.0f : fThen3);
			float fTemp7 = ((fRec1[1] > fTemp2) ? fTemp6 : fTemp4);
			fRec2[0] = ((fRec2[1] * fTemp7) + (fTemp2 * (1.0f - fTemp7)));
			fRec1[0] = fRec2[0];
			fRec6[0] = (fTemp0 - (fSlow11 * ((fSlow12 * fRec6[2]) + (fConst9 * fRec6[1]))));
			fRec9[0] = (fTemp1 - (fSlow11 * ((fSlow12 * fRec9[2]) + (fConst9 * fRec9[1]))));
			float fTemp8 = std::fabs(((fSlow13 * fRec9[0]) + (fSlow14 * fRec9[2])));
			float fTemp9 = ((fRec7[1] > fTemp8) ? fTemp6 : fTemp4);
			fRec8[0] = ((fRec8[1] * fTemp9) + (fTemp8 * (1.0f - fTemp9)));
			fRec7[0] = fRec8[0];
			fRec10[0] = (fTemp0 - (fSlow17 * ((fSlow18 * fRec10[2]) + (fConst12 * fRec10[1]))));
			fRec13[0] = (fTemp1 - (fSlow17 * ((fSlow18 * fRec13[2]) + (fConst12 * fRec13[1]))));
			float fTemp10 = std::fabs(((fSlow19 * fRec13[0]) + (fSlow20 * fRec13[2])));
			float fTemp11 = ((fRec11[1] > fTemp10) ? fTemp6 : fTemp4);
			fRec12[0] = ((fRec12[1] * fTemp11) + (fTemp10 * (1.0f - fTemp11)));
			fRec11[0] = fRec12[0];
			fRec14[0] = (fTemp0 - (fSlow23 * ((fSlow24 * fRec14[2]) + (fConst15 * fRec14[1]))));
			fRec17[0] = (fTemp1 - (fSlow23 * ((fSlow24 * fRec17[2]) + (fConst15 * fRec17[1]))));
			float fTemp12 = std::fabs(((fSlow25 * fRec17[0]) + (fSlow26 * fRec17[2])));
			float fTemp13 = ((fRec15[1] > fTemp12) ? fTemp6 : fTemp4);
			fRec16[0] = ((fRec16[1] * fTemp13) + (fTemp12 * (1.0f - fTemp13)));
			fRec15[0] = fRec16[0];
			fRec18[0] = (fTemp0 - (fSlow29 * ((fSlow30 * fRec18[2]) + (fConst18 * fRec18[1]))));
			fRec21[0] = (fTemp1 - (fSlow29 * ((fSlow30 * fRec21[2]) + (fConst18 * fRec21[1]))));
			float fTemp14 = std::fabs(((fSlow31 * fRec21[0]) + (fSlow32 * fRec21[2])));
			float fTemp15 = ((fRec19[1] > fTemp14) ? fTemp6 : fTemp4);
			fRec20[0] = ((fRec20[1] * fTemp15) + (fTemp14 * (1.0f - fTemp15)));
			fRec19[0] = fRec20[0];
			fRec22[0] = (fTemp0 - (fSlow35 * ((fSlow36 * fRec22[2]) + (fConst21 * fRec22[1]))));
			fRec25[0] = (fTemp1 - (fSlow35 * ((fSlow36 * fRec25[2]) + (fConst21 * fRec25[1]))));
			float fTemp16 = std::fabs(((fSlow37 * fRec25[0]) + (fSlow38 * fRec25[2])));
			float fTemp17 = ((fRec23[1] > fTemp16) ? fTemp6 : fTemp4);
			fRec24[0] = ((fRec24[1] * fTemp17) + (fTemp16 * (1.0f - fTemp17)));
			fRec23[0] = fRec24[0];
			fRec26[0] = (fTemp0 - (fSlow41 * ((fSlow42 * fRec26[2]) + (fConst24 * fRec26[1]))));
			fRec29[0] = (fTemp1 - (fSlow41 * ((fSlow42 * fRec29[2]) + (fConst24 * fRec29[1]))));
			float fTemp18 = std::fabs(((fSlow43 * fRec29[0]) + (fSlow44 * fRec29[2])));
			float fTemp19 = ((fRec27[1] > fTemp18) ? fTemp6 : fTemp4);
			fRec28[0] = ((fRec28[1] * fTemp19) + (fTemp18 * (1.0f - fTemp19)));
			fRec27[0] = fRec28[0];
			fRec30[0] = (fTemp0 - (fSlow46 * ((fSlow47 * fRec30[2]) + (fConst27 * fRec30[1]))));
			fRec33[0] = (fTemp1 - (fSlow46 * ((fSlow47 * fRec33[2]) + (fConst27 * fRec33[1]))));
			float fTemp20 = std::fabs(((fSlow48 * fRec33[0]) + (fSlow49 * fRec33[2])));
			float fTemp21 = ((fRec31[1] > fTemp20) ? fTemp6 : fTemp4);
			fRec32[0] = ((fRec32[1] * fTemp21) + (fTemp20 * (1.0f - fTemp21)));
			fRec31[0] = fRec32[0];
			fRec34[0] = (fTemp0 - (fSlow52 * ((fSlow53 * fRec34[2]) + (fConst30 * fRec34[1]))));
			fRec37[0] = (fTemp1 - (fSlow52 * ((fSlow53 * fRec37[2]) + (fConst30 * fRec37[1]))));
			float fTemp22 = std::fabs(((fSlow54 * fRec37[0]) + (fSlow55 * fRec37[2])));
			float fTemp23 = ((fRec35[1] > fTemp22) ? fTemp6 : fTemp4);
			fRec36[0] = ((fRec36[1] * fTemp23) + (fTemp22 * (1.0f - fTemp23)));
			fRec35[0] = fRec36[0];
			fRec38[0] = (fTemp0 - (fSlow57 * ((fSlow58 * fRec38[2]) + (fConst33 * fRec38[1]))));
			fRec41[0] = (fTemp1 - (fSlow57 * ((fSlow58 * fRec41[2]) + (fConst33 * fRec41[1]))));
			float fTemp24 = std::fabs(((fSlow59 * fRec41[0]) + (fSlow60 * fRec41[2])));
			float fTemp25 = ((fRec39[1] > fTemp24) ? fTemp6 : fTemp4);
			fRec40[0] = ((fRec40[1] * fTemp25) + (fTemp24 * (1.0f - fTemp25)));
			fRec39[0] = fRec40[0];
			fRec42[0] = (fTemp0 - (fSlow62 * ((fSlow63 * fRec42[2]) + (fConst36 * fRec42[1]))));
			fRec45[0] = (fTemp1 - (fSlow62 * ((fSlow63 * fRec45[2]) + (fConst36 * fRec45[1]))));
			float fTemp26 = std::fabs(((fSlow64 * fRec45[0]) + (fSlow65 * fRec45[2])));
			float fTemp27 = ((fRec43[1] > fTemp26) ? fTemp6 : fTemp4);
			fRec44[0] = ((fRec44[1] * fTemp27) + (fTemp26 * (1.0f - fTemp27)));
			fRec43[0] = fRec44[0];
			fRec46[0] = (fTemp0 - (fSlow68 * ((fSlow69 * fRec46[2]) + (fConst39 * fRec46[1]))));
			fRec49[0] = (fTemp1 - (fSlow68 * ((fSlow69 * fRec49[2]) + (fConst39 * fRec49[1]))));
			float fTemp28 = std::fabs(((fSlow70 * fRec49[0]) + (fSlow71 * fRec49[2])));
			float fTemp29 = ((fRec47[1] > fTemp28) ? fTemp6 : fTemp4);
			fRec48[0] = ((fRec48[1] * fTemp29) + (fTemp28 * (1.0f - fTemp29)));
			fRec47[0] = fRec48[0];
			fRec50[0] = (fTemp0 - (fSlow74 * ((fSlow75 * fRec50[2]) + (fConst42 * fRec50[1]))));
			fRec53[0] = (fTemp1 - (fSlow74 * ((fSlow75 * fRec53[2]) + (fConst42 * fRec53[1]))));
			float fTemp30 = std::fabs(((fSlow76 * fRec53[0]) + (fSlow77 * fRec53[2])));
			float fTemp31 = ((fRec51[1] > fTemp30) ? fTemp6 : fTemp4);
			fRec52[0] = ((fRec52[1] * fTemp31) + (fTemp30 * (1.0f - fTemp31)));
			fRec51[0] = fRec52[0];
			fRec54[0] = (fTemp0 - (fSlow80 * ((fSlow81 * fRec54[2]) + (fConst45 * fRec54[1]))));
			fRec57[0] = (fTemp1 - (fSlow80 * ((fSlow81 * fRec57[2]) + (fConst45 * fRec57[1]))));
			float fTemp32 = std::fabs(((fSlow82 * fRec57[0]) + (fSlow83 * fRec57[2])));
			float fTemp33 = ((fRec55[1] > fTemp32) ? fTemp6 : fTemp4);
			fRec56[0] = ((fRec56[1] * fTemp33) + (fTemp32 * (1.0f - fTemp33)));
			fRec55[0] = fRec56[0];
			fRec58[0] = (fTemp0 - (fSlow85 * ((fSlow86 * fRec58[2]) + (fConst48 * fRec58[1]))));
			fRec61[0] = (fTemp1 - (fSlow85 * ((fSlow86 * fRec61[2]) + (fConst48 * fRec61[1]))));
			float fTemp34 = std::fabs(((fSlow87 * fRec61[0]) + (fSlow88 * fRec61[2])));
			float fTemp35 = ((fRec59[1] > fTemp34) ? fTemp6 : fTemp4);
			fRec60[0] = ((fRec60[1] * fTemp35) + (fTemp34 * (1.0f - fTemp35)));
			fRec59[0] = fRec60[0];
			fRec62[0] = (fTemp0 - (fSlow90 * ((fSlow91 * fRec62[2]) + (fConst51 * fRec62[1]))));
			fRec65[0] = (fTemp1 - (fSlow90 * ((fSlow91 * fRec65[2]) + (fConst51 * fRec65[1]))));
			float fTemp36 = std::fabs(((fSlow92 * fRec65[0]) + (fSlow93 * fRec65[2])));
			float fTemp37 = ((fRec63[1] > fTemp36) ? fTemp6 : fTemp4);
			fRec64[0] = ((fRec64[1] * fTemp37) + (fTemp36 * (1.0f - fTemp37)));
			fRec63[0] = fRec64[0];
			fRec66[0] = (fTemp0 - (fSlow95 * ((fSlow96 * fRec66[2]) + (fConst54 * fRec66[1]))));
			fRec69[0] = (fTemp1 - (fSlow95 * ((fSlow96 * fRec69[2]) + (fConst54 * fRec69[1]))));
			float fTemp38 = std::fabs(((fSlow97 * fRec69[0]) + (fSlow98 * fRec69[2])));
			float fTemp39 = ((fRec67[1] > fTemp38) ? fTemp6 : fTemp4);
			fRec68[0] = ((fRec68[1] * fTemp39) + (fTemp38 * (1.0f - fTemp39)));
			fRec67[0] = fRec68[0];
			fRec70[0] = (fTemp0 - (fSlow100 * ((fSlow101 * fRec70[2]) + (fConst57 * fRec70[1]))));
			fRec73[0] = (fTemp1 - (fSlow100 * ((fSlow101 * fRec73[2]) + (fConst57 * fRec73[1]))));
			float fTemp40 = std::fabs(((fSlow102 * fRec73[0]) + (fSlow103 * fRec73[2])));
			float fTemp41 = ((fRec71[1] > fTemp40) ? fTemp6 : fTemp4);
			fRec72[0] = ((fRec72[1] * fTemp41) + (fTemp40 * (1.0f - fTemp41)));
			fRec71[0] = fRec72[0];
			fRec74[0] = (fTemp0 - (fSlow105 * ((fSlow106 * fRec74[2]) + (fConst60 * fRec74[1]))));
			fRec77[0] = (fTemp1 - (fSlow105 * ((fSlow106 * fRec77[2]) + (fConst60 * fRec77[1]))));
			float fTemp42 = std::fabs(((fSlow107 * fRec77[0]) + (fSlow108 * fRec77[2])));
			float fTemp43 = ((fRec75[1] > fTemp42) ? fTemp6 : fTemp4);
			fRec76[0] = ((fRec76[1] * fTemp43) + (fTemp42 * (1.0f - fTemp43)));
			fRec75[0] = fRec76[0];
			fRec78[0] = (fTemp0 - (fSlow110 * ((fSlow111 * fRec78[2]) + (fConst63 * fRec78[1]))));
			fRec81[0] = (fTemp1 - (fSlow110 * ((fSlow111 * fRec81[2]) + (fConst63 * fRec81[1]))));
			float fTemp44 = std::fabs(((fSlow112 * fRec81[0]) + (fSlow113 * fRec81[2])));
			float fTemp45 = ((fRec79[1] > fTemp44) ? fTemp6 : fTemp4);
			fRec80[0] = ((fRec80[1] * fTemp45) + (fTemp44 * (1.0f - fTemp45)));
			fRec79[0] = fRec80[0];
			fRec82[0] = (fTemp0 - (fSlow115 * ((fSlow116 * fRec82[2]) + (fConst66 * fRec82[1]))));
			fRec85[0] = (fTemp1 - (fSlow115 * ((fSlow116 * fRec85[2]) + (fConst66 * fRec85[1]))));
			float fTemp46 = std::fabs(((fSlow117 * fRec85[0]) + (fSlow118 * fRec85[2])));
			float fTemp47 = ((fRec83[1] > fTemp46) ? fTemp6 : fTemp4);
			fRec84[0] = ((fRec84[1] * fTemp47) + (fTemp46 * (1.0f - fTemp47)));
			fRec83[0] = fRec84[0];
			fRec86[0] = (fTemp0 - (fSlow120 * ((fSlow121 * fRec86[2]) + (fConst69 * fRec86[1]))));
			fRec89[0] = (fTemp1 - (fSlow120 * ((fSlow121 * fRec89[2]) + (fConst69 * fRec89[1]))));
			float fTemp48 = std::fabs(((fSlow122 * fRec89[0]) + (fSlow123 * fRec89[2])));
			float fTemp49 = ((fRec87[1] > fTemp48) ? fTemp6 : fTemp4);
			fRec88[0] = ((fRec88[1] * fTemp49) + (fTemp48 * (1.0f - fTemp49)));
			fRec87[0] = fRec88[0];
			fRec90[0] = (fTemp0 - (fSlow125 * ((fSlow126 * fRec90[2]) + (fConst72 * fRec90[1]))));
			fRec93[0] = (fTemp1 - (fSlow125 * ((fSlow126 * fRec93[2]) + (fConst72 * fRec93[1]))));
			float fTemp50 = std::fabs(((fSlow127 * fRec93[0]) + (fSlow128 * fRec93[2])));
			float fTemp51 = ((fRec91[1] > fTemp50) ? fTemp6 : fTemp4);
			fRec92[0] = ((fRec92[1] * fTemp51) + (fTemp50 * (1.0f - fTemp51)));
			fRec91[0] = fRec92[0];
			fRec94[0] = (fTemp0 - (fSlow130 * ((fSlow131 * fRec94[2]) + (fConst75 * fRec94[1]))));
			fRec97[0] = (fTemp1 - (fSlow130 * ((fSlow131 * fRec97[2]) + (fConst75 * fRec97[1]))));
			float fTemp52 = std::fabs(((fSlow132 * fRec97[0]) + (fSlow133 * fRec97[2])));
			float fTemp53 = ((fRec95[1] > fTemp52) ? fTemp6 : fTemp4);
			fRec96[0] = ((fRec96[1] * fTemp53) + (fTemp52 * (1.0f - fTemp53)));
			fRec95[0] = fRec96[0];
			fRec98[0] = (fTemp0 - (fSlow136 * ((fSlow137 * fRec98[2]) + (fConst78 * fRec98[1]))));
			fRec101[0] = (fTemp1 - (fSlow136 * ((fSlow137 * fRec101[2]) + (fConst78 * fRec101[1]))));
			float fTemp54 = std::fabs(((fSlow138 * fRec101[0]) + (fSlow139 * fRec101[2])));
			float fTemp55 = ((fRec99[1] > fTemp54) ? fTemp6 : fTemp4);
			fRec100[0] = ((fRec100[1] * fTemp55) + (fTemp54 * (1.0f - fTemp55)));
			fRec99[0] = fRec100[0];
			fRec102[0] = (fTemp0 - (fSlow141 * ((fSlow142 * fRec102[2]) + (fConst81 * fRec102[1]))));
			fRec105[0] = (fTemp1 - (fSlow141 * ((fSlow142 * fRec105[2]) + (fConst81 * fRec105[1]))));
			float fTemp56 = std::fabs(((fSlow143 * fRec105[0]) + (fSlow144 * fRec105[2])));
			float fTemp57 = ((fRec103[1] > fTemp56) ? fTemp6 : fTemp4);
			fRec104[0] = ((fRec104[1] * fTemp57) + (fTemp56 * (1.0f - fTemp57)));
			fRec103[0] = fRec104[0];
			fRec106[0] = (fTemp0 - (fSlow146 * ((fSlow147 * fRec106[2]) + (fConst84 * fRec106[1]))));
			fRec109[0] = (fTemp1 - (fSlow146 * ((fSlow147 * fRec109[2]) + (fConst84 * fRec109[1]))));
			float fTemp58 = std::fabs(((fSlow148 * fRec109[0]) + (fSlow149 * fRec109[2])));
			float fTemp59 = ((fRec107[1] > fTemp58) ? fTemp6 : fTemp4);
			fRec108[0] = ((fRec108[1] * fTemp59) + (fTemp58 * (1.0f - fTemp59)));
			fRec107[0] = fRec108[0];
			fRec110[0] = (fTemp0 - (fSlow151 * ((fSlow152 * fRec110[2]) + (fConst87 * fRec110[1]))));
			fRec113[0] = (fTemp1 - (fSlow151 * ((fSlow152 * fRec113[2]) + (fConst87 * fRec113[1]))));
			float fTemp60 = std::fabs(((fSlow153 * fRec113[0]) + (fSlow154 * fRec113[2])));
			float fTemp61 = ((fRec111[1] > fTemp60) ? fTemp6 : fTemp4);
			fRec112[0] = ((fRec112[1] * fTemp61) + (fTemp60 * (1.0f - fTemp61)));
			fRec111[0] = fRec112[0];
			fRec114[0] = (fTemp0 - (fSlow156 * ((fSlow157 * fRec114[2]) + (fConst90 * fRec114[1]))));
			fRec117[0] = (fTemp1 - (fSlow156 * ((fSlow157 * fRec117[2]) + (fConst90 * fRec117[1]))));
			float fTemp62 = std::fabs(((fSlow158 * fRec117[0]) + (fSlow159 * fRec117[2])));
			float fTemp63 = ((fRec115[1] > fTemp62) ? fTemp6 : fTemp4);
			fRec116[0] = ((fRec116[1] * fTemp63) + (fTemp62 * (1.0f - fTemp63)));
			fRec115[0] = fRec116[0];
			fRec118[0] = (fTemp0 - (fSlow161 * ((fSlow162 * fRec118[2]) + (fConst93 * fRec118[1]))));
			fRec121[0] = (fTemp1 - (fSlow161 * ((fSlow162 * fRec121[2]) + (fConst93 * fRec121[1]))));
			float fTemp64 = std::fabs(((fSlow163 * fRec121[0]) + (fSlow164 * fRec121[2])));
			float fTemp65 = ((fRec119[1] > fTemp64) ? fTemp6 : fTemp4);
			fRec120[0] = ((fRec120[1] * fTemp65) + (fTemp64 * (1.0f - fTemp65)));
			fRec119[0] = fRec120[0];
			fRec122[0] = (fTemp0 - (fSlow167 * ((fSlow168 * fRec122[2]) + (fConst96 * fRec122[1]))));
			fRec125[0] = (fTemp1 - (fSlow167 * ((fSlow168 * fRec125[2]) + (fConst96 * fRec125[1]))));
			float fTemp66 = std::fabs(((fSlow169 * fRec125[0]) + (fSlow170 * fRec125[2])));
			float fTemp67 = ((fRec123[1] > fTemp66) ? fTemp6 : fTemp4);
			fRec124[0] = ((fRec124[1] * fTemp67) + (fTemp66 * (1.0f - fTemp67)));
			fRec123[0] = fRec124[0];
			fRec126[0] = (fTemp0 - (fSlow173 * ((fSlow174 * fRec126[2]) + (fConst99 * fRec126[1]))));
			fRec129[0] = (fTemp1 - (fSlow173 * ((fSlow174 * fRec129[2]) + (fConst99 * fRec129[1]))));
			float fTemp68 = std::fabs(((fSlow172 * fRec129[0]) + (fSlow175 * fRec129[2])));
			float fTemp69 = ((fRec127[1] > fTemp68) ? fTemp6 : fTemp4);
			fRec128[0] = ((fRec128[1] * fTemp69) + (fTemp68 * (1.0f - fTemp69)));
			fRec127[0] = fRec128[0];
			output0[i0] = FAUSTFLOAT(((fRec0[2] * (0.0f - (fSlow5 * fRec1[0]))) + ((fRec6[2] * (0.0f - (fSlow13 * fRec7[0]))) + ((fSlow13 * (fRec6[0] * fRec7[0])) + ((fRec10[2] * (0.0f - (fSlow19 * fRec11[0]))) + ((fSlow19 * (fRec10[0] * fRec11[0])) + ((fRec14[2] * (0.0f - (fSlow25 * fRec15[0]))) + ((fSlow25 * (fRec14[0] * fRec15[0])) + ((fRec18[2] * (0.0f - (fSlow31 * fRec19[0]))) + ((fSlow31 * (fRec18[0] * fRec19[0])) + ((fRec22[2] * (0.0f - (fSlow37 * fRec23[0]))) + ((fSlow37 * (fRec22[0] * fRec23[0])) + ((fRec26[2] * (0.0f - (fSlow43 * fRec27[0]))) + ((fSlow43 * (fRec26[0] * fRec27[0])) + ((fRec30[2] * (0.0f - (fSlow48 * fRec31[0]))) + ((fSlow48 * (fRec30[0] * fRec31[0])) + ((fRec34[2] * (0.0f - (fSlow54 * fRec35[0]))) + ((fSlow54 * (fRec34[0] * fRec35[0])) + ((fRec38[2] * (0.0f - (fSlow59 * fRec39[0]))) + ((fSlow59 * (fRec38[0] * fRec39[0])) + ((fRec42[2] * (0.0f - (fSlow64 * fRec43[0]))) + ((fSlow64 * (fRec42[0] * fRec43[0])) + ((fRec46[2] * (0.0f - (fSlow70 * fRec47[0]))) + ((fSlow70 * (fRec46[0] * fRec47[0])) + ((fRec50[2] * (0.0f - (fSlow76 * fRec51[0]))) + ((fSlow76 * (fRec50[0] * fRec51[0])) + ((fRec54[2] * (0.0f - (fSlow82 * fRec55[0]))) + ((fSlow82 * (fRec54[0] * fRec55[0])) + ((fRec58[2] * (0.0f - (fSlow87 * fRec59[0]))) + ((fSlow87 * (fRec58[0] * fRec59[0])) + ((fRec62[2] * (0.0f - (fSlow92 * fRec63[0]))) + ((fSlow92 * (fRec62[0] * fRec63[0])) + ((fRec66[2] * (0.0f - (fSlow97 * fRec67[0]))) + ((fSlow97 * (fRec66[0] * fRec67[0])) + ((fRec70[2] * (0.0f - (fSlow102 * fRec71[0]))) + ((fSlow102 * (fRec70[0] * fRec71[0])) + ((fRec74[2] * (0.0f - (fSlow107 * fRec75[0]))) + ((fSlow107 * (fRec74[0] * fRec75[0])) + ((fRec78[2] * (0.0f - (fSlow112 * fRec79[0]))) + ((fSlow112 * (fRec78[0] * fRec79[0])) + (((fRec82[2] * (0.0f - (fSlow117 * fRec83[0]))) + (((fRec86[2] * (0.0f - (fSlow122 * fRec87[0]))) + (((fRec90[2] * (0.0f - (fSlow127 * fRec91[0]))) + (((fRec94[2] * (0.0f - (fSlow132 * fRec95[0]))) + (((fRec98[2] * (0.0f - (fSlow138 * fRec99[0]))) + (((fRec102[2] * (0.0f - (fSlow143 * fRec103[0]))) + (((fRec106[2] * (0.0f - (fSlow148 * fRec107[0]))) + (((fRec110[2] * (0.0f - (fSlow153 * fRec111[0]))) + (((fRec114[2] * (0.0f - (fSlow158 * fRec115[0]))) + (((fRec118[2] * (0.0f - (fSlow163 * fRec119[0]))) + (((fRec122[2] * (0.0f - (fSlow169 * fRec123[0]))) + (((fSlow172 * (fRec126[0] * fRec127[0])) + (fRec126[2] * (0.0f - (fSlow172 * fRec127[0])))) + (fSlow169 * (fRec122[0] * fRec123[0])))) + (fSlow163 * (fRec118[0] * fRec119[0])))) + (fSlow158 * (fRec114[0] * fRec115[0])))) + (fSlow153 * (fRec110[0] * fRec111[0])))) + (fSlow148 * (fRec106[0] * fRec107[0])))) + (fSlow143 * (fRec102[0] * fRec103[0])))) + (fSlow138 * (fRec98[0] * fRec99[0])))) + (fSlow132 * (fRec94[0] * fRec95[0])))) + (fSlow127 * (fRec90[0] * fRec91[0])))) + (fSlow122 * (fRec86[0] * fRec87[0])))) + (fSlow117 * (fRec82[0] * fRec83[0])))) + (fSlow5 * (fRec0[0] * fRec1[0])))))))))))))))))))))))))))))))))))))))))));
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec2[1] = fRec2[0];
			fRec1[1] = fRec1[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			fRec7[1] = fRec7[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec15[1] = fRec15[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec19[1] = fRec19[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec23[1] = fRec23[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec27[1] = fRec27[0];
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec39[1] = fRec39[0];
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fRec47[1] = fRec47[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec51[1] = fRec51[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fRec55[1] = fRec55[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fRec59[1] = fRec59[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec64[1] = fRec64[0];
			fRec63[1] = fRec63[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec68[1] = fRec68[0];
			fRec67[1] = fRec67[0];
			fRec70[2] = fRec70[1];
			fRec70[1] = fRec70[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec72[1] = fRec72[0];
			fRec71[1] = fRec71[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fRec75[1] = fRec75[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec80[1] = fRec80[0];
			fRec79[1] = fRec79[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fRec84[1] = fRec84[0];
			fRec83[1] = fRec83[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec88[1] = fRec88[0];
			fRec87[1] = fRec87[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec92[1] = fRec92[0];
			fRec91[1] = fRec91[0];
			fRec94[2] = fRec94[1];
			fRec94[1] = fRec94[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec96[1] = fRec96[0];
			fRec95[1] = fRec95[0];
			fRec98[2] = fRec98[1];
			fRec98[1] = fRec98[0];
			fRec101[2] = fRec101[1];
			fRec101[1] = fRec101[0];
			fRec100[1] = fRec100[0];
			fRec99[1] = fRec99[0];
			fRec102[2] = fRec102[1];
			fRec102[1] = fRec102[0];
			fRec105[2] = fRec105[1];
			fRec105[1] = fRec105[0];
			fRec104[1] = fRec104[0];
			fRec103[1] = fRec103[0];
			fRec106[2] = fRec106[1];
			fRec106[1] = fRec106[0];
			fRec109[2] = fRec109[1];
			fRec109[1] = fRec109[0];
			fRec108[1] = fRec108[0];
			fRec107[1] = fRec107[0];
			fRec110[2] = fRec110[1];
			fRec110[1] = fRec110[0];
			fRec113[2] = fRec113[1];
			fRec113[1] = fRec113[0];
			fRec112[1] = fRec112[0];
			fRec111[1] = fRec111[0];
			fRec114[2] = fRec114[1];
			fRec114[1] = fRec114[0];
			fRec117[2] = fRec117[1];
			fRec117[1] = fRec117[0];
			fRec116[1] = fRec116[0];
			fRec115[1] = fRec115[0];
			fRec118[2] = fRec118[1];
			fRec118[1] = fRec118[0];
			fRec121[2] = fRec121[1];
			fRec121[1] = fRec121[0];
			fRec120[1] = fRec120[0];
			fRec119[1] = fRec119[0];
			fRec122[2] = fRec122[1];
			fRec122[1] = fRec122[0];
			fRec125[2] = fRec125[1];
			fRec125[1] = fRec125[0];
			fRec124[1] = fRec124[0];
			fRec123[1] = fRec123[0];
			fRec126[2] = fRec126[1];
			fRec126[1] = fRec126[0];
			fRec129[2] = fRec129[1];
			fRec129[1] = fRec129[0];
			fRec128[1] = fRec128[0];
			fRec127[1] = fRec127[0];
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
