/* ------------------------------------------------------------
name: "spat.zita8"
Code generated with Faust 2.28.6 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __spat_zita6x8_H__
#define  __spat_zita6x8_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN spat_zita6x8_dsp.h **************************/
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

class spat_zita6x8_dsp {

    public:

        spat_zita6x8_dsp() {}
        virtual ~spat_zita6x8_dsp() {}

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
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual spat_zita6x8_dsp* clone() = 0;
    
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

class decorator_dsp : public spat_zita6x8_dsp {

    protected:

        spat_zita6x8_dsp* fDSP;

    public:

        decorator_dsp(spat_zita6x8_dsp* spat_zita6x8_dsp = nullptr):fDSP(spat_zita6x8_dsp) {}
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
 * DSP factory class.
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
    
        virtual spat_zita6x8_dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/**************************  END  spat_zita6x8_dsp.h **************************/
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
struct spat_zita6x8 : public spat_zita6x8_dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include "m_pd.h"
#include <algorithm>
#include <cmath>
#include <math.h>

static float spat_zita6x8_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS spat_zita6x8
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class spat_zita6x8 : public spat_zita6x8_dsp {
	
 private:
	
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fVslider1;
	float fRec0[2];
	FAUSTFLOAT fVslider2;
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fVslider3;
	float fRec11[2];
	int IOTA;
	float fVec0[32768];
	float fConst4;
	int iConst5;
	float fVec1[2048];
	int iConst6;
	float fRec9[2];
	float fConst7;
	float fConst8;
	float fRec14[2];
	float fVec2[32768];
	float fConst9;
	int iConst10;
	float fVec3[4096];
	int iConst11;
	float fRec12[2];
	float fConst12;
	float fConst13;
	float fRec17[2];
	float fVec4[16384];
	float fConst14;
	int iConst15;
	float fVec5[16384];
	float fConst16;
	FAUSTFLOAT fVslider4;
	float fVec6[4096];
	int iConst17;
	float fRec15[2];
	float fConst18;
	float fConst19;
	float fRec20[2];
	float fVec7[16384];
	float fConst20;
	int iConst21;
	float fVec8[4096];
	int iConst22;
	float fRec18[2];
	float fConst23;
	float fConst24;
	float fRec23[2];
	float fVec9[16384];
	float fConst25;
	int iConst26;
	float fVec10[2048];
	int iConst27;
	float fRec21[2];
	float fConst28;
	float fConst29;
	float fRec26[2];
	float fVec11[16384];
	float fConst30;
	int iConst31;
	float fVec12[4096];
	int iConst32;
	float fRec24[2];
	float fConst33;
	float fConst34;
	float fRec29[2];
	float fVec13[32768];
	float fConst35;
	int iConst36;
	float fVec14[4096];
	int iConst37;
	float fRec27[2];
	float fConst38;
	float fConst39;
	float fRec32[2];
	float fVec15[16384];
	float fConst40;
	int iConst41;
	float fVec16[2048];
	int iConst42;
	float fRec30[2];
	float fRec1[2];
	float fRec2[2];
	float fRec3[2];
	float fRec4[2];
	float fRec5[2];
	float fRec6[2];
	float fRec7[2];
	float fRec8[2];
	FAUSTFLOAT fVslider5;
	FAUSTFLOAT fVslider6;
	FAUSTFLOAT fVslider7;
	float fRec33[2];
	FAUSTFLOAT fVslider8;
	FAUSTFLOAT fVslider9;
	float fRec44[2];
	float fVec17[32768];
	float fVec18[4096];
	float fRec42[2];
	float fRec47[2];
	float fVec19[16384];
	float fVec20[4096];
	float fRec45[2];
	float fRec50[2];
	float fVec21[32768];
	float fVec22[4096];
	float fRec48[2];
	float fRec53[2];
	float fVec23[16384];
	float fVec24[16384];
	FAUSTFLOAT fVslider10;
	float fVec25[2048];
	float fRec51[2];
	float fRec56[2];
	float fVec26[32768];
	float fVec27[2048];
	float fRec54[2];
	float fRec59[2];
	float fVec28[16384];
	float fVec29[2048];
	float fRec57[2];
	float fRec62[2];
	float fVec30[16384];
	float fVec31[4096];
	float fRec60[2];
	float fRec65[2];
	float fVec32[16384];
	float fVec33[4096];
	float fRec63[2];
	float fRec34[2];
	float fRec35[2];
	float fRec36[2];
	float fRec37[2];
	float fRec38[2];
	float fRec39[2];
	float fRec40[2];
	float fRec41[2];
	FAUSTFLOAT fVslider11;
	FAUSTFLOAT fVslider12;
	FAUSTFLOAT fVslider13;
	float fRec66[2];
	FAUSTFLOAT fVslider14;
	float fVec34[16384];
	FAUSTFLOAT fVslider15;
	FAUSTFLOAT fVslider16;
	float fRec77[2];
	float fVec35[16384];
	float fVec36[4096];
	float fRec75[2];
	float fRec80[2];
	float fVec37[16384];
	float fVec38[2048];
	float fRec78[2];
	float fRec83[2];
	float fVec39[32768];
	float fVec40[4096];
	float fRec81[2];
	float fRec86[2];
	float fVec41[16384];
	float fVec42[4096];
	float fRec84[2];
	float fRec89[2];
	float fVec43[32768];
	float fVec44[4096];
	float fRec87[2];
	float fRec92[2];
	float fVec45[16384];
	float fVec46[2048];
	float fRec90[2];
	float fRec95[2];
	float fVec47[16384];
	float fVec48[4096];
	float fRec93[2];
	float fRec98[2];
	float fVec49[32768];
	float fVec50[2048];
	float fRec96[2];
	float fRec67[2];
	float fRec68[2];
	float fRec69[2];
	float fRec70[2];
	float fRec71[2];
	float fRec72[2];
	float fRec73[2];
	float fRec74[2];
	FAUSTFLOAT fVslider17;
	FAUSTFLOAT fVslider18;
	FAUSTFLOAT fVslider19;
	float fRec99[2];
	FAUSTFLOAT fVslider20;
	FAUSTFLOAT fVslider21;
	float fRec110[2];
	float fVec51[16384];
	float fVec52[16384];
	FAUSTFLOAT fVslider22;
	float fVec53[2048];
	float fRec108[2];
	float fRec113[2];
	float fVec54[16384];
	float fVec55[4096];
	float fRec111[2];
	float fRec116[2];
	float fVec56[16384];
	float fVec57[4096];
	float fRec114[2];
	float fRec119[2];
	float fVec58[32768];
	float fVec59[2048];
	float fRec117[2];
	float fRec122[2];
	float fVec60[32768];
	float fVec61[4096];
	float fRec120[2];
	float fRec125[2];
	float fVec62[16384];
	float fVec63[4096];
	float fRec123[2];
	float fRec128[2];
	float fVec64[32768];
	float fVec65[4096];
	float fRec126[2];
	float fRec131[2];
	float fVec66[16384];
	float fVec67[2048];
	float fRec129[2];
	float fRec100[2];
	float fRec101[2];
	float fRec102[2];
	float fRec103[2];
	float fRec104[2];
	float fRec105[2];
	float fRec106[2];
	float fRec107[2];
	FAUSTFLOAT fVslider23;
	FAUSTFLOAT fVslider24;
	FAUSTFLOAT fVslider25;
	float fRec132[2];
	FAUSTFLOAT fVslider26;
	FAUSTFLOAT fVslider27;
	float fRec143[2];
	float fVec68[32768];
	float fVec69[4096];
	float fRec141[2];
	float fRec146[2];
	float fVec70[16384];
	float fVec71[16384];
	FAUSTFLOAT fVslider28;
	float fVec72[2048];
	float fRec144[2];
	float fRec149[2];
	float fVec73[32768];
	float fVec74[2048];
	float fRec147[2];
	float fRec152[2];
	float fVec75[32768];
	float fVec76[4096];
	float fRec150[2];
	float fRec155[2];
	float fVec77[16384];
	float fVec78[4096];
	float fRec153[2];
	float fRec158[2];
	float fVec79[16384];
	float fVec80[2048];
	float fRec156[2];
	float fRec161[2];
	float fVec81[16384];
	float fVec82[4096];
	float fRec159[2];
	float fRec164[2];
	float fVec83[16384];
	float fVec84[4096];
	float fRec162[2];
	float fRec133[2];
	float fRec134[2];
	float fRec135[2];
	float fRec136[2];
	float fRec137[2];
	float fRec138[2];
	float fRec139[2];
	float fRec140[2];
	FAUSTFLOAT fVslider29;
	FAUSTFLOAT fVslider30;
	FAUSTFLOAT fVslider31;
	float fRec165[2];
	FAUSTFLOAT fVslider32;
	FAUSTFLOAT fVslider33;
	float fRec176[2];
	float fVec85[32768];
	float fVec86[2048];
	float fRec174[2];
	float fRec179[2];
	float fVec87[16384];
	float fVec88[4096];
	float fRec177[2];
	float fRec182[2];
	float fVec89[32768];
	float fVec90[4096];
	float fRec180[2];
	float fRec185[2];
	float fVec91[32768];
	float fVec92[4096];
	float fRec183[2];
	float fRec188[2];
	float fVec93[16384];
	float fVec94[16384];
	FAUSTFLOAT fVslider34;
	float fVec95[2048];
	float fRec186[2];
	float fRec191[2];
	float fVec96[16384];
	float fVec97[4096];
	float fRec189[2];
	float fRec194[2];
	float fVec98[16384];
	float fVec99[4096];
	float fRec192[2];
	float fRec197[2];
	float fVec100[16384];
	float fVec101[2048];
	float fRec195[2];
	float fRec166[2];
	float fRec167[2];
	float fRec168[2];
	float fRec169[2];
	float fRec170[2];
	float fRec171[2];
	float fRec172[2];
	float fRec173[2];
	FAUSTFLOAT fVslider35;
	float fRec198[2];
	float fRec199[2];
	float fRec200[2];
	float fRec201[2];
	float fRec202[2];
	float fRec203[2];
	float fRec204[2];
	float fRec205[2];
	float fRec206[2];
	float fRec207[2];
	float fRec208[2];
	float fRec209[2];
	float fRec210[2];
	float fRec211[2];
	float fRec212[2];
	float fRec213[2];
	float fRec214[2];
	float fRec215[2];
	float fRec216[2];
	float fRec217[2];
	float fRec218[2];
	float fRec219[2];
	float fRec220[2];
	float fRec221[2];
	float fRec222[2];
	float fRec223[2];
	float fRec224[2];
	float fRec225[2];
	float fRec226[2];
	float fRec227[2];
	float fRec228[2];
	float fRec229[2];
	float fRec230[2];
	float fRec231[2];
	float fRec232[2];
	float fRec233[2];
	float fRec234[2];
	float fRec235[2];
	float fRec236[2];
	float fRec237[2];
	float fRec238[2];
	float fRec239[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "spat_zita6x8.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
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
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "spat.zita8");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 6;
	}
	virtual int getNumOutputs() {
		return 8;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			case 2: {
				rate = 1;
				break;
			}
			case 3: {
				rate = 1;
				break;
			}
			case 4: {
				rate = 1;
				break;
			}
			case 5: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			case 2: {
				rate = 1;
				break;
			}
			case 3: {
				rate = 1;
				break;
			}
			case 4: {
				rate = 1;
				break;
			}
			case 5: {
				rate = 1;
				break;
			}
			case 6: {
				rate = 1;
				break;
			}
			case 7: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::cos((37699.1133f / fConst0));
		fConst2 = std::floor(((0.219990999f * fConst0) + 0.5f));
		fConst3 = ((0.0f - (6.90775537f * fConst2)) / fConst0);
		fConst4 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst5 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst2 - fConst4))));
		iConst6 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst4 + -1.0f))));
		fConst7 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst8 = ((0.0f - (6.90775537f * fConst7)) / fConst0);
		fConst9 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst10 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst7 - fConst9))));
		iConst11 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst9 + -1.0f))));
		fConst12 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst13 = ((0.0f - (6.90775537f * fConst12)) / fConst0);
		fConst14 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst15 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst12 - fConst14))));
		fConst16 = (0.00100000005f * fConst0);
		iConst17 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst14 + -1.0f))));
		fConst18 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst19 = ((0.0f - (6.90775537f * fConst18)) / fConst0);
		fConst20 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst21 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst18 - fConst20))));
		iConst22 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst20 + -1.0f))));
		fConst23 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst24 = ((0.0f - (6.90775537f * fConst23)) / fConst0);
		fConst25 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst26 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst23 - fConst25))));
		iConst27 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst25 + -1.0f))));
		fConst28 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst29 = ((0.0f - (6.90775537f * fConst28)) / fConst0);
		fConst30 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst31 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst28 - fConst30))));
		iConst32 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst30 + -1.0f))));
		fConst33 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst34 = ((0.0f - (6.90775537f * fConst33)) / fConst0);
		fConst35 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst36 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst33 - fConst35))));
		iConst37 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst35 + -1.0f))));
		fConst38 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst39 = ((0.0f - (6.90775537f * fConst38)) / fConst0);
		fConst40 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst41 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst38 - fConst40))));
		iConst42 = int(std::min<float>(1024.0f, std::max<float>(0.0f, (fConst40 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(1.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fVslider2 = FAUSTFLOAT(-6.0f);
		fVslider3 = FAUSTFLOAT(2.0f);
		fVslider4 = FAUSTFLOAT(60.0f);
		fVslider5 = FAUSTFLOAT(-3.0f);
		fVslider6 = FAUSTFLOAT(1.0f);
		fVslider7 = FAUSTFLOAT(0.0f);
		fVslider8 = FAUSTFLOAT(-6.0f);
		fVslider9 = FAUSTFLOAT(2.0f);
		fVslider10 = FAUSTFLOAT(60.0f);
		fVslider11 = FAUSTFLOAT(-3.0f);
		fVslider12 = FAUSTFLOAT(1.0f);
		fVslider13 = FAUSTFLOAT(0.0f);
		fVslider14 = FAUSTFLOAT(-6.0f);
		fVslider15 = FAUSTFLOAT(60.0f);
		fVslider16 = FAUSTFLOAT(2.0f);
		fVslider17 = FAUSTFLOAT(-3.0f);
		fVslider18 = FAUSTFLOAT(1.0f);
		fVslider19 = FAUSTFLOAT(0.0f);
		fVslider20 = FAUSTFLOAT(-6.0f);
		fVslider21 = FAUSTFLOAT(2.0f);
		fVslider22 = FAUSTFLOAT(60.0f);
		fVslider23 = FAUSTFLOAT(-3.0f);
		fVslider24 = FAUSTFLOAT(1.0f);
		fVslider25 = FAUSTFLOAT(0.0f);
		fVslider26 = FAUSTFLOAT(-6.0f);
		fVslider27 = FAUSTFLOAT(2.0f);
		fVslider28 = FAUSTFLOAT(60.0f);
		fVslider29 = FAUSTFLOAT(-3.0f);
		fVslider30 = FAUSTFLOAT(1.0f);
		fVslider31 = FAUSTFLOAT(0.0f);
		fVslider32 = FAUSTFLOAT(-6.0f);
		fVslider33 = FAUSTFLOAT(2.0f);
		fVslider34 = FAUSTFLOAT(60.0f);
		fVslider35 = FAUSTFLOAT(-3.0f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec11[l1] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 32768); l2 = (l2 + 1)) {
			fVec0[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2048); l3 = (l3 + 1)) {
			fVec1[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec9[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec14[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 32768); l6 = (l6 + 1)) {
			fVec2[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 4096); l7 = (l7 + 1)) {
			fVec3[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec12[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec17[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 16384); l10 = (l10 + 1)) {
			fVec4[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 16384); l11 = (l11 + 1)) {
			fVec5[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 4096); l12 = (l12 + 1)) {
			fVec6[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec15[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec20[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 16384); l15 = (l15 + 1)) {
			fVec7[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 4096); l16 = (l16 + 1)) {
			fVec8[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec18[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec23[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 16384); l19 = (l19 + 1)) {
			fVec9[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2048); l20 = (l20 + 1)) {
			fVec10[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec21[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec26[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 16384); l23 = (l23 + 1)) {
			fVec11[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 4096); l24 = (l24 + 1)) {
			fVec12[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fRec24[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec29[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 32768); l27 = (l27 + 1)) {
			fVec13[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 4096); l28 = (l28 + 1)) {
			fVec14[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec27[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 2); l30 = (l30 + 1)) {
			fRec32[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 16384); l31 = (l31 + 1)) {
			fVec15[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2048); l32 = (l32 + 1)) {
			fVec16[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec30[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec1[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec2[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec3[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec4[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec5[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec6[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec7[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec8[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec33[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec44[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 32768); l44 = (l44 + 1)) {
			fVec17[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 4096); l45 = (l45 + 1)) {
			fVec18[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec42[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec47[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 16384); l48 = (l48 + 1)) {
			fVec19[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 4096); l49 = (l49 + 1)) {
			fVec20[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec45[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec50[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 32768); l52 = (l52 + 1)) {
			fVec21[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 4096); l53 = (l53 + 1)) {
			fVec22[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec48[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec53[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 16384); l56 = (l56 + 1)) {
			fVec23[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 16384); l57 = (l57 + 1)) {
			fVec24[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2048); l58 = (l58 + 1)) {
			fVec25[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec51[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec56[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 32768); l61 = (l61 + 1)) {
			fVec26[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 2048); l62 = (l62 + 1)) {
			fVec27[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec54[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec59[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 16384); l65 = (l65 + 1)) {
			fVec28[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 2048); l66 = (l66 + 1)) {
			fVec29[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec57[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec62[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 16384); l69 = (l69 + 1)) {
			fVec30[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 4096); l70 = (l70 + 1)) {
			fVec31[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec60[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 2); l72 = (l72 + 1)) {
			fRec65[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 16384); l73 = (l73 + 1)) {
			fVec32[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 4096); l74 = (l74 + 1)) {
			fVec33[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec63[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec34[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec35[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec36[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec37[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec38[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fRec39[l81] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec40[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec41[l83] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec66[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 16384); l85 = (l85 + 1)) {
			fVec34[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 2); l86 = (l86 + 1)) {
			fRec77[l86] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l87 = 0; (l87 < 16384); l87 = (l87 + 1)) {
			fVec35[l87] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l88 = 0; (l88 < 4096); l88 = (l88 + 1)) {
			fVec36[l88] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec75[l89] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec80[l90] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l91 = 0; (l91 < 16384); l91 = (l91 + 1)) {
			fVec37[l91] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l92 = 0; (l92 < 2048); l92 = (l92 + 1)) {
			fVec38[l92] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec78[l93] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l94 = 0; (l94 < 2); l94 = (l94 + 1)) {
			fRec83[l94] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l95 = 0; (l95 < 32768); l95 = (l95 + 1)) {
			fVec39[l95] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l96 = 0; (l96 < 4096); l96 = (l96 + 1)) {
			fVec40[l96] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec81[l97] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec86[l98] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l99 = 0; (l99 < 16384); l99 = (l99 + 1)) {
			fVec41[l99] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l100 = 0; (l100 < 4096); l100 = (l100 + 1)) {
			fVec42[l100] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec84[l101] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			fRec89[l102] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l103 = 0; (l103 < 32768); l103 = (l103 + 1)) {
			fVec43[l103] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l104 = 0; (l104 < 4096); l104 = (l104 + 1)) {
			fVec44[l104] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec87[l105] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec92[l106] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l107 = 0; (l107 < 16384); l107 = (l107 + 1)) {
			fVec45[l107] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l108 = 0; (l108 < 2048); l108 = (l108 + 1)) {
			fVec46[l108] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec90[l109] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			fRec95[l110] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l111 = 0; (l111 < 16384); l111 = (l111 + 1)) {
			fVec47[l111] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l112 = 0; (l112 < 4096); l112 = (l112 + 1)) {
			fVec48[l112] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l113 = 0; (l113 < 2); l113 = (l113 + 1)) {
			fRec93[l113] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec98[l114] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l115 = 0; (l115 < 32768); l115 = (l115 + 1)) {
			fVec49[l115] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l116 = 0; (l116 < 2048); l116 = (l116 + 1)) {
			fVec50[l116] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l117 = 0; (l117 < 2); l117 = (l117 + 1)) {
			fRec96[l117] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l118 = 0; (l118 < 2); l118 = (l118 + 1)) {
			fRec67[l118] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec68[l119] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec69[l120] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec70[l121] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l122 = 0; (l122 < 2); l122 = (l122 + 1)) {
			fRec71[l122] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l123 = 0; (l123 < 2); l123 = (l123 + 1)) {
			fRec72[l123] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			fRec73[l124] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			fRec74[l125] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			fRec99[l126] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l127 = 0; (l127 < 2); l127 = (l127 + 1)) {
			fRec110[l127] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l128 = 0; (l128 < 16384); l128 = (l128 + 1)) {
			fVec51[l128] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l129 = 0; (l129 < 16384); l129 = (l129 + 1)) {
			fVec52[l129] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l130 = 0; (l130 < 2048); l130 = (l130 + 1)) {
			fVec53[l130] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			fRec108[l131] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l132 = 0; (l132 < 2); l132 = (l132 + 1)) {
			fRec113[l132] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l133 = 0; (l133 < 16384); l133 = (l133 + 1)) {
			fVec54[l133] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l134 = 0; (l134 < 4096); l134 = (l134 + 1)) {
			fVec55[l134] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l135 = 0; (l135 < 2); l135 = (l135 + 1)) {
			fRec111[l135] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l136 = 0; (l136 < 2); l136 = (l136 + 1)) {
			fRec116[l136] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l137 = 0; (l137 < 16384); l137 = (l137 + 1)) {
			fVec56[l137] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l138 = 0; (l138 < 4096); l138 = (l138 + 1)) {
			fVec57[l138] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l139 = 0; (l139 < 2); l139 = (l139 + 1)) {
			fRec114[l139] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l140 = 0; (l140 < 2); l140 = (l140 + 1)) {
			fRec119[l140] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l141 = 0; (l141 < 32768); l141 = (l141 + 1)) {
			fVec58[l141] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l142 = 0; (l142 < 2048); l142 = (l142 + 1)) {
			fVec59[l142] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l143 = 0; (l143 < 2); l143 = (l143 + 1)) {
			fRec117[l143] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l144 = 0; (l144 < 2); l144 = (l144 + 1)) {
			fRec122[l144] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l145 = 0; (l145 < 32768); l145 = (l145 + 1)) {
			fVec60[l145] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l146 = 0; (l146 < 4096); l146 = (l146 + 1)) {
			fVec61[l146] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l147 = 0; (l147 < 2); l147 = (l147 + 1)) {
			fRec120[l147] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l148 = 0; (l148 < 2); l148 = (l148 + 1)) {
			fRec125[l148] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l149 = 0; (l149 < 16384); l149 = (l149 + 1)) {
			fVec62[l149] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l150 = 0; (l150 < 4096); l150 = (l150 + 1)) {
			fVec63[l150] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l151 = 0; (l151 < 2); l151 = (l151 + 1)) {
			fRec123[l151] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l152 = 0; (l152 < 2); l152 = (l152 + 1)) {
			fRec128[l152] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l153 = 0; (l153 < 32768); l153 = (l153 + 1)) {
			fVec64[l153] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l154 = 0; (l154 < 4096); l154 = (l154 + 1)) {
			fVec65[l154] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fRec126[l155] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l156 = 0; (l156 < 2); l156 = (l156 + 1)) {
			fRec131[l156] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l157 = 0; (l157 < 16384); l157 = (l157 + 1)) {
			fVec66[l157] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l158 = 0; (l158 < 2048); l158 = (l158 + 1)) {
			fVec67[l158] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l159 = 0; (l159 < 2); l159 = (l159 + 1)) {
			fRec129[l159] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l160 = 0; (l160 < 2); l160 = (l160 + 1)) {
			fRec100[l160] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l161 = 0; (l161 < 2); l161 = (l161 + 1)) {
			fRec101[l161] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			fRec102[l162] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l163 = 0; (l163 < 2); l163 = (l163 + 1)) {
			fRec103[l163] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l164 = 0; (l164 < 2); l164 = (l164 + 1)) {
			fRec104[l164] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l165 = 0; (l165 < 2); l165 = (l165 + 1)) {
			fRec105[l165] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l166 = 0; (l166 < 2); l166 = (l166 + 1)) {
			fRec106[l166] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l167 = 0; (l167 < 2); l167 = (l167 + 1)) {
			fRec107[l167] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l168 = 0; (l168 < 2); l168 = (l168 + 1)) {
			fRec132[l168] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l169 = 0; (l169 < 2); l169 = (l169 + 1)) {
			fRec143[l169] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l170 = 0; (l170 < 32768); l170 = (l170 + 1)) {
			fVec68[l170] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l171 = 0; (l171 < 4096); l171 = (l171 + 1)) {
			fVec69[l171] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l172 = 0; (l172 < 2); l172 = (l172 + 1)) {
			fRec141[l172] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l173 = 0; (l173 < 2); l173 = (l173 + 1)) {
			fRec146[l173] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l174 = 0; (l174 < 16384); l174 = (l174 + 1)) {
			fVec70[l174] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l175 = 0; (l175 < 16384); l175 = (l175 + 1)) {
			fVec71[l175] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l176 = 0; (l176 < 2048); l176 = (l176 + 1)) {
			fVec72[l176] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fRec144[l177] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l178 = 0; (l178 < 2); l178 = (l178 + 1)) {
			fRec149[l178] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l179 = 0; (l179 < 32768); l179 = (l179 + 1)) {
			fVec73[l179] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l180 = 0; (l180 < 2048); l180 = (l180 + 1)) {
			fVec74[l180] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l181 = 0; (l181 < 2); l181 = (l181 + 1)) {
			fRec147[l181] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l182 = 0; (l182 < 2); l182 = (l182 + 1)) {
			fRec152[l182] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l183 = 0; (l183 < 32768); l183 = (l183 + 1)) {
			fVec75[l183] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l184 = 0; (l184 < 4096); l184 = (l184 + 1)) {
			fVec76[l184] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l185 = 0; (l185 < 2); l185 = (l185 + 1)) {
			fRec150[l185] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l186 = 0; (l186 < 2); l186 = (l186 + 1)) {
			fRec155[l186] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l187 = 0; (l187 < 16384); l187 = (l187 + 1)) {
			fVec77[l187] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l188 = 0; (l188 < 4096); l188 = (l188 + 1)) {
			fVec78[l188] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l189 = 0; (l189 < 2); l189 = (l189 + 1)) {
			fRec153[l189] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l190 = 0; (l190 < 2); l190 = (l190 + 1)) {
			fRec158[l190] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l191 = 0; (l191 < 16384); l191 = (l191 + 1)) {
			fVec79[l191] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l192 = 0; (l192 < 2048); l192 = (l192 + 1)) {
			fVec80[l192] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l193 = 0; (l193 < 2); l193 = (l193 + 1)) {
			fRec156[l193] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l194 = 0; (l194 < 2); l194 = (l194 + 1)) {
			fRec161[l194] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l195 = 0; (l195 < 16384); l195 = (l195 + 1)) {
			fVec81[l195] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l196 = 0; (l196 < 4096); l196 = (l196 + 1)) {
			fVec82[l196] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l197 = 0; (l197 < 2); l197 = (l197 + 1)) {
			fRec159[l197] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l198 = 0; (l198 < 2); l198 = (l198 + 1)) {
			fRec164[l198] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l199 = 0; (l199 < 16384); l199 = (l199 + 1)) {
			fVec83[l199] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l200 = 0; (l200 < 4096); l200 = (l200 + 1)) {
			fVec84[l200] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l201 = 0; (l201 < 2); l201 = (l201 + 1)) {
			fRec162[l201] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l202 = 0; (l202 < 2); l202 = (l202 + 1)) {
			fRec133[l202] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l203 = 0; (l203 < 2); l203 = (l203 + 1)) {
			fRec134[l203] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fRec135[l204] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l205 = 0; (l205 < 2); l205 = (l205 + 1)) {
			fRec136[l205] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l206 = 0; (l206 < 2); l206 = (l206 + 1)) {
			fRec137[l206] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l207 = 0; (l207 < 2); l207 = (l207 + 1)) {
			fRec138[l207] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l208 = 0; (l208 < 2); l208 = (l208 + 1)) {
			fRec139[l208] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l209 = 0; (l209 < 2); l209 = (l209 + 1)) {
			fRec140[l209] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l210 = 0; (l210 < 2); l210 = (l210 + 1)) {
			fRec165[l210] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			fRec176[l211] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l212 = 0; (l212 < 32768); l212 = (l212 + 1)) {
			fVec85[l212] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l213 = 0; (l213 < 2048); l213 = (l213 + 1)) {
			fVec86[l213] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l214 = 0; (l214 < 2); l214 = (l214 + 1)) {
			fRec174[l214] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l215 = 0; (l215 < 2); l215 = (l215 + 1)) {
			fRec179[l215] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l216 = 0; (l216 < 16384); l216 = (l216 + 1)) {
			fVec87[l216] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l217 = 0; (l217 < 4096); l217 = (l217 + 1)) {
			fVec88[l217] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l218 = 0; (l218 < 2); l218 = (l218 + 1)) {
			fRec177[l218] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l219 = 0; (l219 < 2); l219 = (l219 + 1)) {
			fRec182[l219] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l220 = 0; (l220 < 32768); l220 = (l220 + 1)) {
			fVec89[l220] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l221 = 0; (l221 < 4096); l221 = (l221 + 1)) {
			fVec90[l221] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l222 = 0; (l222 < 2); l222 = (l222 + 1)) {
			fRec180[l222] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l223 = 0; (l223 < 2); l223 = (l223 + 1)) {
			fRec185[l223] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l224 = 0; (l224 < 32768); l224 = (l224 + 1)) {
			fVec91[l224] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l225 = 0; (l225 < 4096); l225 = (l225 + 1)) {
			fVec92[l225] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l226 = 0; (l226 < 2); l226 = (l226 + 1)) {
			fRec183[l226] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l227 = 0; (l227 < 2); l227 = (l227 + 1)) {
			fRec188[l227] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l228 = 0; (l228 < 16384); l228 = (l228 + 1)) {
			fVec93[l228] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l229 = 0; (l229 < 16384); l229 = (l229 + 1)) {
			fVec94[l229] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l230 = 0; (l230 < 2048); l230 = (l230 + 1)) {
			fVec95[l230] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l231 = 0; (l231 < 2); l231 = (l231 + 1)) {
			fRec186[l231] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l232 = 0; (l232 < 2); l232 = (l232 + 1)) {
			fRec191[l232] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l233 = 0; (l233 < 16384); l233 = (l233 + 1)) {
			fVec96[l233] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l234 = 0; (l234 < 4096); l234 = (l234 + 1)) {
			fVec97[l234] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l235 = 0; (l235 < 2); l235 = (l235 + 1)) {
			fRec189[l235] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l236 = 0; (l236 < 2); l236 = (l236 + 1)) {
			fRec194[l236] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l237 = 0; (l237 < 16384); l237 = (l237 + 1)) {
			fVec98[l237] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l238 = 0; (l238 < 4096); l238 = (l238 + 1)) {
			fVec99[l238] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l239 = 0; (l239 < 2); l239 = (l239 + 1)) {
			fRec192[l239] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l240 = 0; (l240 < 2); l240 = (l240 + 1)) {
			fRec197[l240] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l241 = 0; (l241 < 16384); l241 = (l241 + 1)) {
			fVec100[l241] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l242 = 0; (l242 < 2048); l242 = (l242 + 1)) {
			fVec101[l242] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l243 = 0; (l243 < 2); l243 = (l243 + 1)) {
			fRec195[l243] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l244 = 0; (l244 < 2); l244 = (l244 + 1)) {
			fRec166[l244] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l245 = 0; (l245 < 2); l245 = (l245 + 1)) {
			fRec167[l245] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l246 = 0; (l246 < 2); l246 = (l246 + 1)) {
			fRec168[l246] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l247 = 0; (l247 < 2); l247 = (l247 + 1)) {
			fRec169[l247] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l248 = 0; (l248 < 2); l248 = (l248 + 1)) {
			fRec170[l248] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l249 = 0; (l249 < 2); l249 = (l249 + 1)) {
			fRec171[l249] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l250 = 0; (l250 < 2); l250 = (l250 + 1)) {
			fRec172[l250] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l251 = 0; (l251 < 2); l251 = (l251 + 1)) {
			fRec173[l251] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l252 = 0; (l252 < 2); l252 = (l252 + 1)) {
			fRec198[l252] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l253 = 0; (l253 < 2); l253 = (l253 + 1)) {
			fRec199[l253] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l254 = 0; (l254 < 2); l254 = (l254 + 1)) {
			fRec200[l254] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l255 = 0; (l255 < 2); l255 = (l255 + 1)) {
			fRec201[l255] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l256 = 0; (l256 < 2); l256 = (l256 + 1)) {
			fRec202[l256] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l257 = 0; (l257 < 2); l257 = (l257 + 1)) {
			fRec203[l257] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l258 = 0; (l258 < 2); l258 = (l258 + 1)) {
			fRec204[l258] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l259 = 0; (l259 < 2); l259 = (l259 + 1)) {
			fRec205[l259] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l260 = 0; (l260 < 2); l260 = (l260 + 1)) {
			fRec206[l260] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l261 = 0; (l261 < 2); l261 = (l261 + 1)) {
			fRec207[l261] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l262 = 0; (l262 < 2); l262 = (l262 + 1)) {
			fRec208[l262] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l263 = 0; (l263 < 2); l263 = (l263 + 1)) {
			fRec209[l263] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l264 = 0; (l264 < 2); l264 = (l264 + 1)) {
			fRec210[l264] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l265 = 0; (l265 < 2); l265 = (l265 + 1)) {
			fRec211[l265] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l266 = 0; (l266 < 2); l266 = (l266 + 1)) {
			fRec212[l266] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l267 = 0; (l267 < 2); l267 = (l267 + 1)) {
			fRec213[l267] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l268 = 0; (l268 < 2); l268 = (l268 + 1)) {
			fRec214[l268] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l269 = 0; (l269 < 2); l269 = (l269 + 1)) {
			fRec215[l269] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l270 = 0; (l270 < 2); l270 = (l270 + 1)) {
			fRec216[l270] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l271 = 0; (l271 < 2); l271 = (l271 + 1)) {
			fRec217[l271] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l272 = 0; (l272 < 2); l272 = (l272 + 1)) {
			fRec218[l272] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l273 = 0; (l273 < 2); l273 = (l273 + 1)) {
			fRec219[l273] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l274 = 0; (l274 < 2); l274 = (l274 + 1)) {
			fRec220[l274] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l275 = 0; (l275 < 2); l275 = (l275 + 1)) {
			fRec221[l275] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l276 = 0; (l276 < 2); l276 = (l276 + 1)) {
			fRec222[l276] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l277 = 0; (l277 < 2); l277 = (l277 + 1)) {
			fRec223[l277] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l278 = 0; (l278 < 2); l278 = (l278 + 1)) {
			fRec224[l278] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l279 = 0; (l279 < 2); l279 = (l279 + 1)) {
			fRec225[l279] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l280 = 0; (l280 < 2); l280 = (l280 + 1)) {
			fRec226[l280] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l281 = 0; (l281 < 2); l281 = (l281 + 1)) {
			fRec227[l281] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l282 = 0; (l282 < 2); l282 = (l282 + 1)) {
			fRec228[l282] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l283 = 0; (l283 < 2); l283 = (l283 + 1)) {
			fRec229[l283] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l284 = 0; (l284 < 2); l284 = (l284 + 1)) {
			fRec230[l284] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l285 = 0; (l285 < 2); l285 = (l285 + 1)) {
			fRec231[l285] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l286 = 0; (l286 < 2); l286 = (l286 + 1)) {
			fRec232[l286] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l287 = 0; (l287 < 2); l287 = (l287 + 1)) {
			fRec233[l287] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l288 = 0; (l288 < 2); l288 = (l288 + 1)) {
			fRec234[l288] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l289 = 0; (l289 < 2); l289 = (l289 + 1)) {
			fRec235[l289] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l290 = 0; (l290 < 2); l290 = (l290 + 1)) {
			fRec236[l290] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l291 = 0; (l291 < 2); l291 = (l291 + 1)) {
			fRec237[l291] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l292 = 0; (l292 < 2); l292 = (l292 + 1)) {
			fRec238[l292] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l293 = 0; (l293 < 2); l293 = (l293 + 1)) {
			fRec239[l293] = 0.0f;
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
	
	virtual spat_zita6x8* clone() {
		return new spat_zita6x8();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("spat.zita8");
		ui_interface->addVerticalSlider("angle0", &fVslider19, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle1", &fVslider13, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle2", &fVslider7, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle3", &fVslider1, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle4", &fVslider25, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle5", &fVslider31, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->declare(&fVslider21, "scale", "log");
		ui_interface->declare(&fVslider21, "style", "knob");
		ui_interface->declare(&fVslider21, "unit", "sec");
		ui_interface->addVerticalSlider("decay0", &fVslider21, 2.0f, 0.0f, 90.0f, 0.100000001f);
		ui_interface->declare(&fVslider16, "scale", "log");
		ui_interface->declare(&fVslider16, "style", "knob");
		ui_interface->declare(&fVslider16, "unit", "sec");
		ui_interface->addVerticalSlider("decay1", &fVslider16, 2.0f, 0.0f, 90.0f, 0.100000001f);
		ui_interface->declare(&fVslider9, "scale", "log");
		ui_interface->declare(&fVslider9, "style", "knob");
		ui_interface->declare(&fVslider9, "unit", "sec");
		ui_interface->addVerticalSlider("decay2", &fVslider9, 2.0f, 0.0f, 90.0f, 0.100000001f);
		ui_interface->declare(&fVslider3, "scale", "log");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "unit", "sec");
		ui_interface->addVerticalSlider("decay3", &fVslider3, 2.0f, 0.0f, 90.0f, 0.100000001f);
		ui_interface->declare(&fVslider27, "scale", "log");
		ui_interface->declare(&fVslider27, "style", "knob");
		ui_interface->declare(&fVslider27, "unit", "sec");
		ui_interface->addVerticalSlider("decay4", &fVslider27, 2.0f, 0.0f, 90.0f, 0.100000001f);
		ui_interface->declare(&fVslider33, "scale", "log");
		ui_interface->declare(&fVslider33, "style", "knob");
		ui_interface->declare(&fVslider33, "unit", "sec");
		ui_interface->addVerticalSlider("decay5", &fVslider33, 2.0f, 0.0f, 90.0f, 0.100000001f);
		ui_interface->declare(&fVslider22, "style", "knob");
		ui_interface->declare(&fVslider22, "unit", "ms");
		ui_interface->addVerticalSlider("delay0", &fVslider22, 60.0f, 0.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider15, "style", "knob");
		ui_interface->declare(&fVslider15, "unit", "ms");
		ui_interface->addVerticalSlider("delay1", &fVslider15, 60.0f, 0.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider10, "style", "knob");
		ui_interface->declare(&fVslider10, "unit", "ms");
		ui_interface->addVerticalSlider("delay2", &fVslider10, 60.0f, 0.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "unit", "ms");
		ui_interface->addVerticalSlider("delay3", &fVslider4, 60.0f, 0.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider28, "style", "knob");
		ui_interface->declare(&fVslider28, "unit", "ms");
		ui_interface->addVerticalSlider("delay4", &fVslider28, 60.0f, 0.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider34, "style", "knob");
		ui_interface->declare(&fVslider34, "unit", "ms");
		ui_interface->addVerticalSlider("delay5", &fVslider34, 60.0f, 0.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider23, "scale", "log");
		ui_interface->declare(&fVslider23, "style", "knob");
		ui_interface->declare(&fVslider23, "unit", "db");
		ui_interface->addVerticalSlider("direct0", &fVslider23, -3.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider17, "scale", "log");
		ui_interface->declare(&fVslider17, "style", "knob");
		ui_interface->declare(&fVslider17, "unit", "db");
		ui_interface->addVerticalSlider("direct1", &fVslider17, -3.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider11, "scale", "log");
		ui_interface->declare(&fVslider11, "style", "knob");
		ui_interface->declare(&fVslider11, "unit", "db");
		ui_interface->addVerticalSlider("direct2", &fVslider11, -3.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider5, "scale", "log");
		ui_interface->declare(&fVslider5, "style", "knob");
		ui_interface->declare(&fVslider5, "unit", "db");
		ui_interface->addVerticalSlider("direct3", &fVslider5, -3.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider29, "scale", "log");
		ui_interface->declare(&fVslider29, "style", "knob");
		ui_interface->declare(&fVslider29, "unit", "db");
		ui_interface->addVerticalSlider("direct4", &fVslider29, -3.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider35, "scale", "log");
		ui_interface->declare(&fVslider35, "style", "knob");
		ui_interface->declare(&fVslider35, "unit", "db");
		ui_interface->addVerticalSlider("direct5", &fVslider35, -3.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->addVerticalSlider("radius0", &fVslider18, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius1", &fVslider12, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius2", &fVslider6, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius3", &fVslider0, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius4", &fVslider24, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius5", &fVslider30, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->declare(&fVslider20, "scale", "log");
		ui_interface->declare(&fVslider20, "style", "knob");
		ui_interface->declare(&fVslider20, "unit", "db");
		ui_interface->addVerticalSlider("reverb0", &fVslider20, -6.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider14, "scale", "log");
		ui_interface->declare(&fVslider14, "style", "knob");
		ui_interface->declare(&fVslider14, "unit", "db");
		ui_interface->addVerticalSlider("reverb1", &fVslider14, -6.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider8, "scale", "log");
		ui_interface->declare(&fVslider8, "style", "knob");
		ui_interface->declare(&fVslider8, "unit", "db");
		ui_interface->addVerticalSlider("reverb2", &fVslider8, -6.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "db");
		ui_interface->addVerticalSlider("reverb3", &fVslider2, -6.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider26, "scale", "log");
		ui_interface->declare(&fVslider26, "style", "knob");
		ui_interface->declare(&fVslider26, "unit", "db");
		ui_interface->addVerticalSlider("reverb4", &fVslider26, -6.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->declare(&fVslider32, "scale", "log");
		ui_interface->declare(&fVslider32, "style", "knob");
		ui_interface->declare(&fVslider32, "unit", "db");
		ui_interface->addVerticalSlider("reverb5", &fVslider32, -6.0f, -60.0f, 0.0f, 0.100000001f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* input2 = inputs[2];
		FAUSTFLOAT* input3 = inputs[3];
		FAUSTFLOAT* input4 = inputs[4];
		FAUSTFLOAT* input5 = inputs[5];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		FAUSTFLOAT* output2 = outputs[2];
		FAUSTFLOAT* output3 = outputs[3];
		FAUSTFLOAT* output4 = outputs[4];
		FAUSTFLOAT* output5 = outputs[5];
		FAUSTFLOAT* output6 = outputs[6];
		FAUSTFLOAT* output7 = outputs[7];
		float fSlow0 = float(fVslider0);
		float fSlow1 = (fSlow0 + 1.0f);
		float fSlow2 = float(fVslider1);
		float fSlow3 = (0.000500000024f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow4 = (0.370000005f * std::pow(10.0f, (0.0500000007f * float(fVslider2))));
		float fSlow5 = float(fVslider3);
		float fSlow6 = std::exp((fConst3 / fSlow5));
		float fSlow7 = spat_zita6x8_faustpower2_f(fSlow6);
		float fSlow8 = (1.0f - (fConst1 * fSlow7));
		float fSlow9 = (1.0f - fSlow7);
		float fSlow10 = (fSlow8 / fSlow9);
		float fSlow11 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow8) / spat_zita6x8_faustpower2_f(fSlow9)) + -1.0f)));
		float fSlow12 = (fSlow10 - fSlow11);
		float fSlow13 = (fSlow6 * (fSlow11 + (1.0f - fSlow10)));
		float fSlow14 = std::exp((fConst8 / fSlow5));
		float fSlow15 = spat_zita6x8_faustpower2_f(fSlow14);
		float fSlow16 = (1.0f - (fConst1 * fSlow15));
		float fSlow17 = (1.0f - fSlow15);
		float fSlow18 = (fSlow16 / fSlow17);
		float fSlow19 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow16) / spat_zita6x8_faustpower2_f(fSlow17)) + -1.0f)));
		float fSlow20 = (fSlow18 - fSlow19);
		float fSlow21 = (fSlow14 * (fSlow19 + (1.0f - fSlow18)));
		float fSlow22 = std::exp((fConst13 / fSlow5));
		float fSlow23 = spat_zita6x8_faustpower2_f(fSlow22);
		float fSlow24 = (1.0f - (fConst1 * fSlow23));
		float fSlow25 = (1.0f - fSlow23);
		float fSlow26 = (fSlow24 / fSlow25);
		float fSlow27 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow24) / spat_zita6x8_faustpower2_f(fSlow25)) + -1.0f)));
		float fSlow28 = (fSlow26 - fSlow27);
		float fSlow29 = (fSlow22 * (fSlow27 + (1.0f - fSlow26)));
		int iSlow30 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst16 * float(fVslider4)))));
		float fSlow31 = std::exp((fConst19 / fSlow5));
		float fSlow32 = spat_zita6x8_faustpower2_f(fSlow31);
		float fSlow33 = (1.0f - (fConst1 * fSlow32));
		float fSlow34 = (1.0f - fSlow32);
		float fSlow35 = (fSlow33 / fSlow34);
		float fSlow36 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow33) / spat_zita6x8_faustpower2_f(fSlow34)) + -1.0f)));
		float fSlow37 = (fSlow35 - fSlow36);
		float fSlow38 = (fSlow31 * (fSlow36 + (1.0f - fSlow35)));
		float fSlow39 = std::exp((fConst24 / fSlow5));
		float fSlow40 = spat_zita6x8_faustpower2_f(fSlow39);
		float fSlow41 = (1.0f - (fConst1 * fSlow40));
		float fSlow42 = (1.0f - fSlow40);
		float fSlow43 = (fSlow41 / fSlow42);
		float fSlow44 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow41) / spat_zita6x8_faustpower2_f(fSlow42)) + -1.0f)));
		float fSlow45 = (fSlow43 - fSlow44);
		float fSlow46 = (fSlow39 * (fSlow44 + (1.0f - fSlow43)));
		float fSlow47 = std::exp((fConst29 / fSlow5));
		float fSlow48 = spat_zita6x8_faustpower2_f(fSlow47);
		float fSlow49 = (1.0f - (fConst1 * fSlow48));
		float fSlow50 = (1.0f - fSlow48);
		float fSlow51 = (fSlow49 / fSlow50);
		float fSlow52 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow49) / spat_zita6x8_faustpower2_f(fSlow50)) + -1.0f)));
		float fSlow53 = (fSlow51 - fSlow52);
		float fSlow54 = (fSlow47 * (fSlow52 + (1.0f - fSlow51)));
		float fSlow55 = std::exp((fConst34 / fSlow5));
		float fSlow56 = spat_zita6x8_faustpower2_f(fSlow55);
		float fSlow57 = (1.0f - (fConst1 * fSlow56));
		float fSlow58 = (1.0f - fSlow56);
		float fSlow59 = (fSlow57 / fSlow58);
		float fSlow60 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow57) / spat_zita6x8_faustpower2_f(fSlow58)) + -1.0f)));
		float fSlow61 = (fSlow59 - fSlow60);
		float fSlow62 = (fSlow55 * (fSlow60 + (1.0f - fSlow59)));
		float fSlow63 = std::exp((fConst39 / fSlow5));
		float fSlow64 = spat_zita6x8_faustpower2_f(fSlow63);
		float fSlow65 = (1.0f - (fConst1 * fSlow64));
		float fSlow66 = (1.0f - fSlow64);
		float fSlow67 = (fSlow65 / fSlow66);
		float fSlow68 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow65) / spat_zita6x8_faustpower2_f(fSlow66)) + -1.0f)));
		float fSlow69 = (fSlow67 - fSlow68);
		float fSlow70 = (fSlow63 * (fSlow68 + (1.0f - fSlow67)));
		float fSlow71 = std::pow(10.0f, (0.0500000007f * float(fVslider5)));
		float fSlow72 = float(fVslider6);
		float fSlow73 = (fSlow72 + 1.0f);
		float fSlow74 = float(fVslider7);
		float fSlow75 = (0.000500000024f * (fSlow73 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow72 * std::fabs((std::fmod((fSlow74 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow76 = (0.370000005f * std::pow(10.0f, (0.0500000007f * float(fVslider8))));
		float fSlow77 = float(fVslider9);
		float fSlow78 = std::exp((fConst8 / fSlow77));
		float fSlow79 = spat_zita6x8_faustpower2_f(fSlow78);
		float fSlow80 = (1.0f - (fConst1 * fSlow79));
		float fSlow81 = (1.0f - fSlow79);
		float fSlow82 = (fSlow80 / fSlow81);
		float fSlow83 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow80) / spat_zita6x8_faustpower2_f(fSlow81)) + -1.0f)));
		float fSlow84 = (fSlow82 - fSlow83);
		float fSlow85 = (fSlow78 * (fSlow83 + (1.0f - fSlow82)));
		float fSlow86 = std::exp((fConst29 / fSlow77));
		float fSlow87 = spat_zita6x8_faustpower2_f(fSlow86);
		float fSlow88 = (1.0f - (fConst1 * fSlow87));
		float fSlow89 = (1.0f - fSlow87);
		float fSlow90 = (fSlow88 / fSlow89);
		float fSlow91 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow88) / spat_zita6x8_faustpower2_f(fSlow89)) + -1.0f)));
		float fSlow92 = (fSlow90 - fSlow91);
		float fSlow93 = (fSlow86 * (fSlow91 + (1.0f - fSlow90)));
		float fSlow94 = std::exp((fConst34 / fSlow77));
		float fSlow95 = spat_zita6x8_faustpower2_f(fSlow94);
		float fSlow96 = (1.0f - (fConst1 * fSlow95));
		float fSlow97 = (1.0f - fSlow95);
		float fSlow98 = (fSlow96 / fSlow97);
		float fSlow99 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow96) / spat_zita6x8_faustpower2_f(fSlow97)) + -1.0f)));
		float fSlow100 = (fSlow98 - fSlow99);
		float fSlow101 = (fSlow94 * (fSlow99 + (1.0f - fSlow98)));
		float fSlow102 = std::exp((fConst24 / fSlow77));
		float fSlow103 = spat_zita6x8_faustpower2_f(fSlow102);
		float fSlow104 = (1.0f - (fConst1 * fSlow103));
		float fSlow105 = (1.0f - fSlow103);
		float fSlow106 = (fSlow104 / fSlow105);
		float fSlow107 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow104) / spat_zita6x8_faustpower2_f(fSlow105)) + -1.0f)));
		float fSlow108 = (fSlow106 - fSlow107);
		float fSlow109 = (fSlow102 * (fSlow107 + (1.0f - fSlow106)));
		int iSlow110 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst16 * float(fVslider10)))));
		float fSlow111 = std::exp((fConst3 / fSlow77));
		float fSlow112 = spat_zita6x8_faustpower2_f(fSlow111);
		float fSlow113 = (1.0f - (fConst1 * fSlow112));
		float fSlow114 = (1.0f - fSlow112);
		float fSlow115 = (fSlow113 / fSlow114);
		float fSlow116 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow113) / spat_zita6x8_faustpower2_f(fSlow114)) + -1.0f)));
		float fSlow117 = (fSlow115 - fSlow116);
		float fSlow118 = (fSlow111 * (fSlow116 + (1.0f - fSlow115)));
		float fSlow119 = std::exp((fConst39 / fSlow77));
		float fSlow120 = spat_zita6x8_faustpower2_f(fSlow119);
		float fSlow121 = (1.0f - (fConst1 * fSlow120));
		float fSlow122 = (1.0f - fSlow120);
		float fSlow123 = (fSlow121 / fSlow122);
		float fSlow124 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow121) / spat_zita6x8_faustpower2_f(fSlow122)) + -1.0f)));
		float fSlow125 = (fSlow123 - fSlow124);
		float fSlow126 = (fSlow119 * (fSlow124 + (1.0f - fSlow123)));
		float fSlow127 = std::exp((fConst13 / fSlow77));
		float fSlow128 = spat_zita6x8_faustpower2_f(fSlow127);
		float fSlow129 = (1.0f - (fConst1 * fSlow128));
		float fSlow130 = (1.0f - fSlow128);
		float fSlow131 = (fSlow129 / fSlow130);
		float fSlow132 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow129) / spat_zita6x8_faustpower2_f(fSlow130)) + -1.0f)));
		float fSlow133 = (fSlow131 - fSlow132);
		float fSlow134 = (fSlow127 * (fSlow132 + (1.0f - fSlow131)));
		float fSlow135 = std::exp((fConst19 / fSlow77));
		float fSlow136 = spat_zita6x8_faustpower2_f(fSlow135);
		float fSlow137 = (1.0f - (fConst1 * fSlow136));
		float fSlow138 = (1.0f - fSlow136);
		float fSlow139 = (fSlow137 / fSlow138);
		float fSlow140 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow137) / spat_zita6x8_faustpower2_f(fSlow138)) + -1.0f)));
		float fSlow141 = (fSlow139 - fSlow140);
		float fSlow142 = (fSlow135 * (fSlow140 + (1.0f - fSlow139)));
		float fSlow143 = std::pow(10.0f, (0.0500000007f * float(fVslider11)));
		float fSlow144 = float(fVslider12);
		float fSlow145 = (fSlow144 + 1.0f);
		float fSlow146 = float(fVslider13);
		float fSlow147 = (0.000500000024f * (fSlow145 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow144 * std::fabs((std::fmod((fSlow146 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow148 = (0.370000005f * std::pow(10.0f, (0.0500000007f * float(fVslider14))));
		int iSlow149 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst16 * float(fVslider15)))));
		float fSlow150 = float(fVslider16);
		float fSlow151 = std::exp((fConst19 / fSlow150));
		float fSlow152 = spat_zita6x8_faustpower2_f(fSlow151);
		float fSlow153 = (1.0f - (fConst1 * fSlow152));
		float fSlow154 = (1.0f - fSlow152);
		float fSlow155 = (fSlow153 / fSlow154);
		float fSlow156 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow153) / spat_zita6x8_faustpower2_f(fSlow154)) + -1.0f)));
		float fSlow157 = (fSlow155 - fSlow156);
		float fSlow158 = (fSlow151 * (fSlow156 + (1.0f - fSlow155)));
		float fSlow159 = std::exp((fConst24 / fSlow150));
		float fSlow160 = spat_zita6x8_faustpower2_f(fSlow159);
		float fSlow161 = (1.0f - (fConst1 * fSlow160));
		float fSlow162 = (1.0f - fSlow160);
		float fSlow163 = (fSlow161 / fSlow162);
		float fSlow164 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow161) / spat_zita6x8_faustpower2_f(fSlow162)) + -1.0f)));
		float fSlow165 = (fSlow163 - fSlow164);
		float fSlow166 = (fSlow159 * (fSlow164 + (1.0f - fSlow163)));
		float fSlow167 = std::exp((fConst8 / fSlow150));
		float fSlow168 = spat_zita6x8_faustpower2_f(fSlow167);
		float fSlow169 = (1.0f - (fConst1 * fSlow168));
		float fSlow170 = (1.0f - fSlow168);
		float fSlow171 = (fSlow169 / fSlow170);
		float fSlow172 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow169) / spat_zita6x8_faustpower2_f(fSlow170)) + -1.0f)));
		float fSlow173 = (fSlow171 - fSlow172);
		float fSlow174 = (fSlow167 * (fSlow172 + (1.0f - fSlow171)));
		float fSlow175 = std::exp((fConst29 / fSlow150));
		float fSlow176 = spat_zita6x8_faustpower2_f(fSlow175);
		float fSlow177 = (1.0f - (fConst1 * fSlow176));
		float fSlow178 = (1.0f - fSlow176);
		float fSlow179 = (fSlow177 / fSlow178);
		float fSlow180 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow177) / spat_zita6x8_faustpower2_f(fSlow178)) + -1.0f)));
		float fSlow181 = (fSlow179 - fSlow180);
		float fSlow182 = (fSlow175 * (fSlow180 + (1.0f - fSlow179)));
		float fSlow183 = std::exp((fConst34 / fSlow150));
		float fSlow184 = spat_zita6x8_faustpower2_f(fSlow183);
		float fSlow185 = (1.0f - (fConst1 * fSlow184));
		float fSlow186 = (1.0f - fSlow184);
		float fSlow187 = (fSlow185 / fSlow186);
		float fSlow188 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow185) / spat_zita6x8_faustpower2_f(fSlow186)) + -1.0f)));
		float fSlow189 = (fSlow187 - fSlow188);
		float fSlow190 = (fSlow183 * (fSlow188 + (1.0f - fSlow187)));
		float fSlow191 = std::exp((fConst39 / fSlow150));
		float fSlow192 = spat_zita6x8_faustpower2_f(fSlow191);
		float fSlow193 = (1.0f - (fConst1 * fSlow192));
		float fSlow194 = (1.0f - fSlow192);
		float fSlow195 = (fSlow193 / fSlow194);
		float fSlow196 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow193) / spat_zita6x8_faustpower2_f(fSlow194)) + -1.0f)));
		float fSlow197 = (fSlow195 - fSlow196);
		float fSlow198 = (fSlow191 * (fSlow196 + (1.0f - fSlow195)));
		float fSlow199 = std::exp((fConst13 / fSlow150));
		float fSlow200 = spat_zita6x8_faustpower2_f(fSlow199);
		float fSlow201 = (1.0f - (fConst1 * fSlow200));
		float fSlow202 = (1.0f - fSlow200);
		float fSlow203 = (fSlow201 / fSlow202);
		float fSlow204 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow201) / spat_zita6x8_faustpower2_f(fSlow202)) + -1.0f)));
		float fSlow205 = (fSlow203 - fSlow204);
		float fSlow206 = (fSlow199 * (fSlow204 + (1.0f - fSlow203)));
		float fSlow207 = std::exp((fConst3 / fSlow150));
		float fSlow208 = spat_zita6x8_faustpower2_f(fSlow207);
		float fSlow209 = (1.0f - (fConst1 * fSlow208));
		float fSlow210 = (1.0f - fSlow208);
		float fSlow211 = (fSlow209 / fSlow210);
		float fSlow212 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow209) / spat_zita6x8_faustpower2_f(fSlow210)) + -1.0f)));
		float fSlow213 = (fSlow211 - fSlow212);
		float fSlow214 = (fSlow207 * (fSlow212 + (1.0f - fSlow211)));
		float fSlow215 = std::pow(10.0f, (0.0500000007f * float(fVslider17)));
		float fSlow216 = float(fVslider18);
		float fSlow217 = (fSlow216 + 1.0f);
		float fSlow218 = float(fVslider19);
		float fSlow219 = (0.000500000024f * (fSlow217 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow216 * std::fabs((std::fmod((fSlow218 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow220 = (0.370000005f * std::pow(10.0f, (0.0500000007f * float(fVslider20))));
		float fSlow221 = float(fVslider21);
		float fSlow222 = std::exp((fConst39 / fSlow221));
		float fSlow223 = spat_zita6x8_faustpower2_f(fSlow222);
		float fSlow224 = (1.0f - (fConst1 * fSlow223));
		float fSlow225 = (1.0f - fSlow223);
		float fSlow226 = (fSlow224 / fSlow225);
		float fSlow227 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow224) / spat_zita6x8_faustpower2_f(fSlow225)) + -1.0f)));
		float fSlow228 = (fSlow226 - fSlow227);
		float fSlow229 = (fSlow222 * (fSlow227 + (1.0f - fSlow226)));
		int iSlow230 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst16 * float(fVslider22)))));
		float fSlow231 = std::exp((fConst13 / fSlow221));
		float fSlow232 = spat_zita6x8_faustpower2_f(fSlow231);
		float fSlow233 = (1.0f - (fConst1 * fSlow232));
		float fSlow234 = (1.0f - fSlow232);
		float fSlow235 = (fSlow233 / fSlow234);
		float fSlow236 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow233) / spat_zita6x8_faustpower2_f(fSlow234)) + -1.0f)));
		float fSlow237 = (fSlow235 - fSlow236);
		float fSlow238 = (fSlow231 * (fSlow236 + (1.0f - fSlow235)));
		float fSlow239 = std::exp((fConst19 / fSlow221));
		float fSlow240 = spat_zita6x8_faustpower2_f(fSlow239);
		float fSlow241 = (1.0f - (fConst1 * fSlow240));
		float fSlow242 = (1.0f - fSlow240);
		float fSlow243 = (fSlow241 / fSlow242);
		float fSlow244 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow241) / spat_zita6x8_faustpower2_f(fSlow242)) + -1.0f)));
		float fSlow245 = (fSlow243 - fSlow244);
		float fSlow246 = (fSlow239 * (fSlow244 + (1.0f - fSlow243)));
		float fSlow247 = std::exp((fConst3 / fSlow221));
		float fSlow248 = spat_zita6x8_faustpower2_f(fSlow247);
		float fSlow249 = (1.0f - (fConst1 * fSlow248));
		float fSlow250 = (1.0f - fSlow248);
		float fSlow251 = (fSlow249 / fSlow250);
		float fSlow252 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow249) / spat_zita6x8_faustpower2_f(fSlow250)) + -1.0f)));
		float fSlow253 = (fSlow251 - fSlow252);
		float fSlow254 = (fSlow247 * (fSlow252 + (1.0f - fSlow251)));
		float fSlow255 = std::exp((fConst8 / fSlow221));
		float fSlow256 = spat_zita6x8_faustpower2_f(fSlow255);
		float fSlow257 = (1.0f - (fConst1 * fSlow256));
		float fSlow258 = (1.0f - fSlow256);
		float fSlow259 = (fSlow257 / fSlow258);
		float fSlow260 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow257) / spat_zita6x8_faustpower2_f(fSlow258)) + -1.0f)));
		float fSlow261 = (fSlow259 - fSlow260);
		float fSlow262 = (fSlow255 * (fSlow260 + (1.0f - fSlow259)));
		float fSlow263 = std::exp((fConst29 / fSlow221));
		float fSlow264 = spat_zita6x8_faustpower2_f(fSlow263);
		float fSlow265 = (1.0f - (fConst1 * fSlow264));
		float fSlow266 = (1.0f - fSlow264);
		float fSlow267 = (fSlow265 / fSlow266);
		float fSlow268 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow265) / spat_zita6x8_faustpower2_f(fSlow266)) + -1.0f)));
		float fSlow269 = (fSlow267 - fSlow268);
		float fSlow270 = (fSlow263 * (fSlow268 + (1.0f - fSlow267)));
		float fSlow271 = std::exp((fConst34 / fSlow221));
		float fSlow272 = spat_zita6x8_faustpower2_f(fSlow271);
		float fSlow273 = (1.0f - (fConst1 * fSlow272));
		float fSlow274 = (1.0f - fSlow272);
		float fSlow275 = (fSlow273 / fSlow274);
		float fSlow276 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow273) / spat_zita6x8_faustpower2_f(fSlow274)) + -1.0f)));
		float fSlow277 = (fSlow275 - fSlow276);
		float fSlow278 = (fSlow271 * (fSlow276 + (1.0f - fSlow275)));
		float fSlow279 = std::exp((fConst24 / fSlow221));
		float fSlow280 = spat_zita6x8_faustpower2_f(fSlow279);
		float fSlow281 = (1.0f - (fConst1 * fSlow280));
		float fSlow282 = (1.0f - fSlow280);
		float fSlow283 = (fSlow281 / fSlow282);
		float fSlow284 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow281) / spat_zita6x8_faustpower2_f(fSlow282)) + -1.0f)));
		float fSlow285 = (fSlow283 - fSlow284);
		float fSlow286 = (fSlow279 * (fSlow284 + (1.0f - fSlow283)));
		float fSlow287 = std::pow(10.0f, (0.0500000007f * float(fVslider23)));
		float fSlow288 = float(fVslider24);
		float fSlow289 = (fSlow288 + 1.0f);
		float fSlow290 = float(fVslider25);
		float fSlow291 = (0.000500000024f * (fSlow289 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow288 * std::fabs((std::fmod((fSlow290 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow292 = (0.370000005f * std::pow(10.0f, (0.0500000007f * float(fVslider26))));
		float fSlow293 = float(fVslider27);
		float fSlow294 = std::exp((fConst34 / fSlow293));
		float fSlow295 = spat_zita6x8_faustpower2_f(fSlow294);
		float fSlow296 = (1.0f - (fConst1 * fSlow295));
		float fSlow297 = (1.0f - fSlow295);
		float fSlow298 = (fSlow296 / fSlow297);
		float fSlow299 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow296) / spat_zita6x8_faustpower2_f(fSlow297)) + -1.0f)));
		float fSlow300 = (fSlow298 - fSlow299);
		float fSlow301 = (fSlow294 * (fSlow299 + (1.0f - fSlow298)));
		float fSlow302 = std::exp((fConst39 / fSlow293));
		float fSlow303 = spat_zita6x8_faustpower2_f(fSlow302);
		float fSlow304 = (1.0f - (fConst1 * fSlow303));
		float fSlow305 = (1.0f - fSlow303);
		float fSlow306 = (fSlow304 / fSlow305);
		float fSlow307 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow304) / spat_zita6x8_faustpower2_f(fSlow305)) + -1.0f)));
		float fSlow308 = (fSlow306 - fSlow307);
		float fSlow309 = (fSlow302 * (fSlow307 + (1.0f - fSlow306)));
		int iSlow310 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst16 * float(fVslider28)))));
		float fSlow311 = std::exp((fConst3 / fSlow293));
		float fSlow312 = spat_zita6x8_faustpower2_f(fSlow311);
		float fSlow313 = (1.0f - (fConst1 * fSlow312));
		float fSlow314 = (1.0f - fSlow312);
		float fSlow315 = (fSlow313 / fSlow314);
		float fSlow316 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow313) / spat_zita6x8_faustpower2_f(fSlow314)) + -1.0f)));
		float fSlow317 = (fSlow315 - fSlow316);
		float fSlow318 = (fSlow311 * (fSlow316 + (1.0f - fSlow315)));
		float fSlow319 = std::exp((fConst8 / fSlow293));
		float fSlow320 = spat_zita6x8_faustpower2_f(fSlow319);
		float fSlow321 = (1.0f - (fConst1 * fSlow320));
		float fSlow322 = (1.0f - fSlow320);
		float fSlow323 = (fSlow321 / fSlow322);
		float fSlow324 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow321) / spat_zita6x8_faustpower2_f(fSlow322)) + -1.0f)));
		float fSlow325 = (fSlow323 - fSlow324);
		float fSlow326 = (fSlow319 * (fSlow324 + (1.0f - fSlow323)));
		float fSlow327 = std::exp((fConst29 / fSlow293));
		float fSlow328 = spat_zita6x8_faustpower2_f(fSlow327);
		float fSlow329 = (1.0f - (fConst1 * fSlow328));
		float fSlow330 = (1.0f - fSlow328);
		float fSlow331 = (fSlow329 / fSlow330);
		float fSlow332 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow329) / spat_zita6x8_faustpower2_f(fSlow330)) + -1.0f)));
		float fSlow333 = (fSlow331 - fSlow332);
		float fSlow334 = (fSlow327 * (fSlow332 + (1.0f - fSlow331)));
		float fSlow335 = std::exp((fConst24 / fSlow293));
		float fSlow336 = spat_zita6x8_faustpower2_f(fSlow335);
		float fSlow337 = (1.0f - (fConst1 * fSlow336));
		float fSlow338 = (1.0f - fSlow336);
		float fSlow339 = (fSlow337 / fSlow338);
		float fSlow340 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow337) / spat_zita6x8_faustpower2_f(fSlow338)) + -1.0f)));
		float fSlow341 = (fSlow339 - fSlow340);
		float fSlow342 = (fSlow335 * (fSlow340 + (1.0f - fSlow339)));
		float fSlow343 = std::exp((fConst13 / fSlow293));
		float fSlow344 = spat_zita6x8_faustpower2_f(fSlow343);
		float fSlow345 = (1.0f - (fConst1 * fSlow344));
		float fSlow346 = (1.0f - fSlow344);
		float fSlow347 = (fSlow345 / fSlow346);
		float fSlow348 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow345) / spat_zita6x8_faustpower2_f(fSlow346)) + -1.0f)));
		float fSlow349 = (fSlow347 - fSlow348);
		float fSlow350 = (fSlow343 * (fSlow348 + (1.0f - fSlow347)));
		float fSlow351 = std::exp((fConst19 / fSlow293));
		float fSlow352 = spat_zita6x8_faustpower2_f(fSlow351);
		float fSlow353 = (1.0f - (fConst1 * fSlow352));
		float fSlow354 = (1.0f - fSlow352);
		float fSlow355 = (fSlow353 / fSlow354);
		float fSlow356 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow353) / spat_zita6x8_faustpower2_f(fSlow354)) + -1.0f)));
		float fSlow357 = (fSlow355 - fSlow356);
		float fSlow358 = (fSlow351 * (fSlow356 + (1.0f - fSlow355)));
		float fSlow359 = std::pow(10.0f, (0.0500000007f * float(fVslider29)));
		float fSlow360 = float(fVslider30);
		float fSlow361 = (fSlow360 + 1.0f);
		float fSlow362 = float(fVslider31);
		float fSlow363 = (0.000500000024f * (fSlow361 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow360 * std::fabs((std::fmod((fSlow362 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow364 = (0.370000005f * std::pow(10.0f, (0.0500000007f * float(fVslider32))));
		float fSlow365 = float(fVslider33);
		float fSlow366 = std::exp((fConst3 / fSlow365));
		float fSlow367 = spat_zita6x8_faustpower2_f(fSlow366);
		float fSlow368 = (1.0f - (fConst1 * fSlow367));
		float fSlow369 = (1.0f - fSlow367);
		float fSlow370 = (fSlow368 / fSlow369);
		float fSlow371 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow368) / spat_zita6x8_faustpower2_f(fSlow369)) + -1.0f)));
		float fSlow372 = (fSlow370 - fSlow371);
		float fSlow373 = (fSlow366 * (fSlow371 + (1.0f - fSlow370)));
		float fSlow374 = std::exp((fConst29 / fSlow365));
		float fSlow375 = spat_zita6x8_faustpower2_f(fSlow374);
		float fSlow376 = (1.0f - (fConst1 * fSlow375));
		float fSlow377 = (1.0f - fSlow375);
		float fSlow378 = (fSlow376 / fSlow377);
		float fSlow379 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow376) / spat_zita6x8_faustpower2_f(fSlow377)) + -1.0f)));
		float fSlow380 = (fSlow378 - fSlow379);
		float fSlow381 = (fSlow374 * (fSlow379 + (1.0f - fSlow378)));
		float fSlow382 = std::exp((fConst8 / fSlow365));
		float fSlow383 = spat_zita6x8_faustpower2_f(fSlow382);
		float fSlow384 = (1.0f - (fConst1 * fSlow383));
		float fSlow385 = (1.0f - fSlow383);
		float fSlow386 = (fSlow384 / fSlow385);
		float fSlow387 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow384) / spat_zita6x8_faustpower2_f(fSlow385)) + -1.0f)));
		float fSlow388 = (fSlow386 - fSlow387);
		float fSlow389 = (fSlow382 * (fSlow387 + (1.0f - fSlow386)));
		float fSlow390 = std::exp((fConst34 / fSlow365));
		float fSlow391 = spat_zita6x8_faustpower2_f(fSlow390);
		float fSlow392 = (1.0f - (fConst1 * fSlow391));
		float fSlow393 = (1.0f - fSlow391);
		float fSlow394 = (fSlow392 / fSlow393);
		float fSlow395 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow392) / spat_zita6x8_faustpower2_f(fSlow393)) + -1.0f)));
		float fSlow396 = (fSlow394 - fSlow395);
		float fSlow397 = (fSlow390 * (fSlow395 + (1.0f - fSlow394)));
		float fSlow398 = std::exp((fConst39 / fSlow365));
		float fSlow399 = spat_zita6x8_faustpower2_f(fSlow398);
		float fSlow400 = (1.0f - (fConst1 * fSlow399));
		float fSlow401 = (1.0f - fSlow399);
		float fSlow402 = (fSlow400 / fSlow401);
		float fSlow403 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow400) / spat_zita6x8_faustpower2_f(fSlow401)) + -1.0f)));
		float fSlow404 = (fSlow402 - fSlow403);
		float fSlow405 = (fSlow398 * (fSlow403 + (1.0f - fSlow402)));
		int iSlow406 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst16 * float(fVslider34)))));
		float fSlow407 = std::exp((fConst13 / fSlow365));
		float fSlow408 = spat_zita6x8_faustpower2_f(fSlow407);
		float fSlow409 = (1.0f - (fConst1 * fSlow408));
		float fSlow410 = (1.0f - fSlow408);
		float fSlow411 = (fSlow409 / fSlow410);
		float fSlow412 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow409) / spat_zita6x8_faustpower2_f(fSlow410)) + -1.0f)));
		float fSlow413 = (fSlow411 - fSlow412);
		float fSlow414 = (fSlow407 * (fSlow412 + (1.0f - fSlow411)));
		float fSlow415 = std::exp((fConst19 / fSlow365));
		float fSlow416 = spat_zita6x8_faustpower2_f(fSlow415);
		float fSlow417 = (1.0f - (fConst1 * fSlow416));
		float fSlow418 = (1.0f - fSlow416);
		float fSlow419 = (fSlow417 / fSlow418);
		float fSlow420 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow417) / spat_zita6x8_faustpower2_f(fSlow418)) + -1.0f)));
		float fSlow421 = (fSlow419 - fSlow420);
		float fSlow422 = (fSlow415 * (fSlow420 + (1.0f - fSlow419)));
		float fSlow423 = std::exp((fConst24 / fSlow365));
		float fSlow424 = spat_zita6x8_faustpower2_f(fSlow423);
		float fSlow425 = (1.0f - (fConst1 * fSlow424));
		float fSlow426 = (1.0f - fSlow424);
		float fSlow427 = (fSlow425 / fSlow426);
		float fSlow428 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow425) / spat_zita6x8_faustpower2_f(fSlow426)) + -1.0f)));
		float fSlow429 = (fSlow427 - fSlow428);
		float fSlow430 = (fSlow423 * (fSlow428 + (1.0f - fSlow427)));
		float fSlow431 = std::pow(10.0f, (0.0500000007f * float(fVslider35)));
		float fSlow432 = (0.000500000024f * (fSlow217 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow216 * std::fabs((std::fmod((fSlow218 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow433 = (0.000500000024f * (fSlow145 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow144 * std::fabs((std::fmod((fSlow146 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow434 = (0.000500000024f * (fSlow73 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow72 * std::fabs((std::fmod((fSlow74 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow435 = (0.000500000024f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow436 = (0.000500000024f * (fSlow289 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow288 * std::fabs((std::fmod((fSlow290 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow437 = (0.000500000024f * (fSlow361 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow360 * std::fabs((std::fmod((fSlow362 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow438 = (0.000500000024f * (fSlow217 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow216 * std::fabs((std::fmod((fSlow218 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow439 = (0.000500000024f * (fSlow145 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow144 * std::fabs((std::fmod((fSlow146 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow440 = (0.000500000024f * (fSlow73 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow72 * std::fabs((std::fmod((fSlow74 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow441 = (0.000500000024f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow442 = (0.000500000024f * (fSlow289 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow288 * std::fabs((std::fmod((fSlow290 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow443 = (0.000500000024f * (fSlow361 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow360 * std::fabs((std::fmod((fSlow362 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow444 = (0.000500000024f * (fSlow217 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow216 * std::fabs((std::fmod((fSlow218 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow445 = (0.000500000024f * (fSlow145 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow144 * std::fabs((std::fmod((fSlow146 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow446 = (0.000500000024f * (fSlow73 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow72 * std::fabs((std::fmod((fSlow74 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow447 = (0.000500000024f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow448 = (0.000500000024f * (fSlow289 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow288 * std::fabs((std::fmod((fSlow290 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow449 = (0.000500000024f * (fSlow361 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow360 * std::fabs((std::fmod((fSlow362 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow450 = (0.000500000024f * (fSlow217 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow216 * std::fabs((std::fmod((fSlow218 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow451 = (0.000500000024f * (fSlow145 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow144 * std::fabs((std::fmod((fSlow146 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow452 = (0.000500000024f * (fSlow73 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow72 * std::fabs((std::fmod((fSlow74 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow453 = (0.000500000024f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow454 = (0.000500000024f * (fSlow289 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow288 * std::fabs((std::fmod((fSlow290 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow455 = (0.000500000024f * (fSlow361 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow360 * std::fabs((std::fmod((fSlow362 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow456 = (0.000500000024f * (fSlow217 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow216 * std::fabs((std::fmod((fSlow218 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow457 = (0.000500000024f * (fSlow145 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow144 * std::fabs((std::fmod((fSlow146 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow458 = (0.000500000024f * (fSlow73 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow72 * std::fabs((std::fmod((fSlow74 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow459 = (0.000500000024f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow460 = (0.000500000024f * (fSlow289 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow288 * std::fabs((std::fmod((fSlow290 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow461 = (0.000500000024f * (fSlow361 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow360 * std::fabs((std::fmod((fSlow362 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow462 = (0.000500000024f * (fSlow217 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow216 * std::fabs((std::fmod((fSlow218 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow463 = (0.000500000024f * (fSlow145 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow144 * std::fabs((std::fmod((fSlow146 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow464 = (0.000500000024f * (fSlow73 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow72 * std::fabs((std::fmod((fSlow74 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow465 = (0.000500000024f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow466 = (0.000500000024f * (fSlow289 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow288 * std::fabs((std::fmod((fSlow290 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow467 = (0.000500000024f * (fSlow361 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow360 * std::fabs((std::fmod((fSlow362 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow468 = (0.000500000024f * (fSlow217 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow216 * std::fabs((std::fmod((fSlow218 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow469 = (0.000500000024f * (fSlow145 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow144 * std::fabs((std::fmod((fSlow146 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow470 = (0.000500000024f * (fSlow73 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow72 * std::fabs((std::fmod((fSlow74 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow471 = (0.000500000024f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow472 = (0.000500000024f * (fSlow289 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow288 * std::fabs((std::fmod((fSlow290 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow473 = (0.000500000024f * (fSlow361 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow360 * std::fabs((std::fmod((fSlow362 + 0.625f), 1.0f) + -0.5f)))))))))));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec0[0] = (fSlow3 + (0.999000013f * fRec0[1]));
			fRec11[0] = ((fSlow12 * fRec11[1]) + (fSlow13 * fRec8[1]));
			fVec0[(IOTA & 32767)] = ((0.353553385f * fRec11[0]) + 9.99999968e-21f);
			float fTemp0 = ((0.600000024f * fRec9[1]) + fVec0[((IOTA - iConst5) & 32767)]);
			fVec1[(IOTA & 2047)] = fTemp0;
			fRec9[0] = fVec1[((IOTA - iConst6) & 2047)];
			float fRec10 = (0.0f - (0.600000024f * fTemp0));
			fRec14[0] = ((fSlow20 * fRec14[1]) + (fSlow21 * fRec4[1]));
			fVec2[(IOTA & 32767)] = ((0.353553385f * fRec14[0]) + 9.99999968e-21f);
			float fTemp1 = ((0.600000024f * fRec12[1]) + fVec2[((IOTA - iConst10) & 32767)]);
			fVec3[(IOTA & 4095)] = fTemp1;
			fRec12[0] = fVec3[((IOTA - iConst11) & 4095)];
			float fRec13 = (0.0f - (0.600000024f * fTemp1));
			fRec17[0] = ((fSlow28 * fRec17[1]) + (fSlow29 * fRec3[1]));
			fVec4[(IOTA & 16383)] = ((0.353553385f * fRec17[0]) + 9.99999968e-21f);
			float fTemp2 = float(input3[i]);
			fVec5[(IOTA & 16383)] = fTemp2;
			float fTemp3 = (0.300000012f * fVec5[((IOTA - iSlow30) & 16383)]);
			float fTemp4 = (fVec4[((IOTA - iConst15) & 16383)] - (fTemp3 + (0.600000024f * fRec15[1])));
			fVec6[(IOTA & 4095)] = fTemp4;
			fRec15[0] = fVec6[((IOTA - iConst17) & 4095)];
			float fRec16 = (0.600000024f * fTemp4);
			fRec20[0] = ((fSlow37 * fRec20[1]) + (fSlow38 * fRec5[1]));
			fVec7[(IOTA & 16383)] = ((0.353553385f * fRec20[0]) + 9.99999968e-21f);
			float fTemp5 = ((fTemp3 + fVec7[((IOTA - iConst21) & 16383)]) - (0.600000024f * fRec18[1]));
			fVec8[(IOTA & 4095)] = fTemp5;
			fRec18[0] = fVec8[((IOTA - iConst22) & 4095)];
			float fRec19 = (0.600000024f * fTemp5);
			fRec23[0] = ((fSlow45 * fRec23[1]) + (fSlow46 * fRec1[1]));
			fVec9[(IOTA & 16383)] = ((0.353553385f * fRec23[0]) + 9.99999968e-21f);
			float fTemp6 = ((fVec9[((IOTA - iConst26) & 16383)] + fTemp3) - (0.600000024f * fRec21[1]));
			fVec10[(IOTA & 2047)] = fTemp6;
			fRec21[0] = fVec10[((IOTA - iConst27) & 2047)];
			float fRec22 = (0.600000024f * fTemp6);
			fRec26[0] = ((fSlow53 * fRec26[1]) + (fSlow54 * fRec6[1]));
			fVec11[(IOTA & 16383)] = ((0.353553385f * fRec26[0]) + 9.99999968e-21f);
			float fTemp7 = ((0.600000024f * fRec24[1]) + fVec11[((IOTA - iConst31) & 16383)]);
			fVec12[(IOTA & 4095)] = fTemp7;
			fRec24[0] = fVec12[((IOTA - iConst32) & 4095)];
			float fRec25 = (0.0f - (0.600000024f * fTemp7));
			fRec29[0] = ((fSlow61 * fRec29[1]) + (fSlow62 * fRec2[1]));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec29[0]) + 9.99999968e-21f);
			float fTemp8 = ((0.600000024f * fRec27[1]) + fVec13[((IOTA - iConst36) & 32767)]);
			fVec14[(IOTA & 4095)] = fTemp8;
			fRec27[0] = fVec14[((IOTA - iConst37) & 4095)];
			float fRec28 = (0.0f - (0.600000024f * fTemp8));
			fRec32[0] = ((fSlow69 * fRec32[1]) + (fSlow70 * fRec7[1]));
			fVec15[(IOTA & 16383)] = ((0.353553385f * fRec32[0]) + 9.99999968e-21f);
			float fTemp9 = (fVec15[((IOTA - iConst41) & 16383)] - (fTemp3 + (0.600000024f * fRec30[1])));
			fVec16[(IOTA & 2047)] = fTemp9;
			fRec30[0] = fVec16[((IOTA - iConst42) & 2047)];
			float fRec31 = (0.600000024f * fTemp9);
			float fTemp10 = (fRec22 + fRec19);
			float fTemp11 = (fRec31 + (fRec16 + fTemp10));
			fRec1[0] = (fRec9[1] + (fRec12[1] + (fRec15[1] + (fRec18[1] + (fRec21[1] + (fRec24[1] + (fRec27[1] + (fRec30[1] + (fRec10 + (fRec13 + (fRec25 + (fRec28 + fTemp11))))))))))));
			fRec2[0] = ((fRec15[1] + (fRec18[1] + (fRec21[1] + (fRec30[1] + fTemp11)))) - (fRec9[1] + (fRec12[1] + (fRec24[1] + (fRec27[1] + (fRec10 + (fRec13 + (fRec28 + fRec25))))))));
			float fTemp12 = (fRec16 + fRec31);
			fRec3[0] = ((fRec18[1] + (fRec21[1] + (fRec24[1] + (fRec27[1] + (fRec25 + (fRec28 + fTemp10)))))) - (fRec9[1] + (fRec12[1] + (fRec15[1] + (fRec30[1] + (fRec10 + (fRec13 + fTemp12)))))));
			fRec4[0] = ((fRec9[1] + (fRec12[1] + (fRec18[1] + (fRec21[1] + (fRec10 + (fRec13 + fTemp10)))))) - (fRec15[1] + (fRec24[1] + (fRec27[1] + (fRec30[1] + (fRec25 + (fRec28 + fTemp12)))))));
			float fTemp13 = (fRec22 + fRec16);
			float fTemp14 = (fRec19 + fRec31);
			fRec5[0] = ((fRec12[1] + (fRec15[1] + (fRec21[1] + (fRec27[1] + (fRec13 + (fRec28 + fTemp13)))))) - (fRec9[1] + (fRec18[1] + (fRec24[1] + (fRec30[1] + (fRec10 + (fRec25 + fTemp14)))))));
			fRec6[0] = ((fRec9[1] + (fRec15[1] + (fRec21[1] + (fRec24[1] + (fRec10 + (fRec25 + fTemp13)))))) - (fRec12[1] + (fRec18[1] + (fRec27[1] + (fRec30[1] + (fRec13 + (fRec28 + fTemp14)))))));
			float fTemp15 = (fRec22 + fRec31);
			float fTemp16 = (fRec19 + fRec16);
			fRec7[0] = ((fRec9[1] + (fRec21[1] + (fRec27[1] + (fRec30[1] + (fRec10 + (fRec28 + fTemp15)))))) - (fRec12[1] + (fRec15[1] + (fRec18[1] + (fRec24[1] + (fRec13 + (fRec25 + fTemp16)))))));
			fRec8[0] = ((fRec12[1] + (fRec21[1] + (fRec24[1] + (fRec30[1] + (fRec13 + (fRec25 + fTemp15)))))) - (fRec9[1] + (fRec15[1] + (fRec18[1] + (fRec27[1] + (fRec10 + (fRec28 + fTemp16)))))));
			float fTemp17 = ((fSlow4 * (fRec2[0] + fRec3[0])) + (fSlow71 * fTemp2));
			fRec33[0] = (fSlow75 + (0.999000013f * fRec33[1]));
			fRec44[0] = ((fSlow84 * fRec44[1]) + (fSlow85 * fRec37[1]));
			fVec17[(IOTA & 32767)] = ((0.353553385f * fRec44[0]) + 9.99999968e-21f);
			float fTemp18 = ((0.600000024f * fRec42[1]) + fVec17[((IOTA - iConst10) & 32767)]);
			fVec18[(IOTA & 4095)] = fTemp18;
			fRec42[0] = fVec18[((IOTA - iConst11) & 4095)];
			float fRec43 = (0.0f - (0.600000024f * fTemp18));
			fRec47[0] = ((fSlow92 * fRec47[1]) + (fSlow93 * fRec39[1]));
			fVec19[(IOTA & 16383)] = ((0.353553385f * fRec47[0]) + 9.99999968e-21f);
			float fTemp19 = ((0.600000024f * fRec45[1]) + fVec19[((IOTA - iConst31) & 16383)]);
			fVec20[(IOTA & 4095)] = fTemp19;
			fRec45[0] = fVec20[((IOTA - iConst32) & 4095)];
			float fRec46 = (0.0f - (0.600000024f * fTemp19));
			fRec50[0] = ((fSlow100 * fRec50[1]) + (fSlow101 * fRec35[1]));
			fVec21[(IOTA & 32767)] = ((0.353553385f * fRec50[0]) + 9.99999968e-21f);
			float fTemp20 = ((0.600000024f * fRec48[1]) + fVec21[((IOTA - iConst36) & 32767)]);
			fVec22[(IOTA & 4095)] = fTemp20;
			fRec48[0] = fVec22[((IOTA - iConst37) & 4095)];
			float fRec49 = (0.0f - (0.600000024f * fTemp20));
			fRec53[0] = ((fSlow108 * fRec53[1]) + (fSlow109 * fRec34[1]));
			fVec23[(IOTA & 16383)] = ((0.353553385f * fRec53[0]) + 9.99999968e-21f);
			float fTemp21 = float(input2[i]);
			fVec24[(IOTA & 16383)] = fTemp21;
			float fTemp22 = (0.300000012f * fVec24[((IOTA - iSlow110) & 16383)]);
			float fTemp23 = ((fVec23[((IOTA - iConst26) & 16383)] + fTemp22) - (0.600000024f * fRec51[1]));
			fVec25[(IOTA & 2047)] = fTemp23;
			fRec51[0] = fVec25[((IOTA - iConst27) & 2047)];
			float fRec52 = (0.600000024f * fTemp23);
			fRec56[0] = ((fSlow117 * fRec56[1]) + (fSlow118 * fRec41[1]));
			fVec26[(IOTA & 32767)] = ((0.353553385f * fRec56[0]) + 9.99999968e-21f);
			float fTemp24 = ((0.600000024f * fRec54[1]) + fVec26[((IOTA - iConst5) & 32767)]);
			fVec27[(IOTA & 2047)] = fTemp24;
			fRec54[0] = fVec27[((IOTA - iConst6) & 2047)];
			float fRec55 = (0.0f - (0.600000024f * fTemp24));
			fRec59[0] = ((fSlow125 * fRec59[1]) + (fSlow126 * fRec40[1]));
			fVec28[(IOTA & 16383)] = ((0.353553385f * fRec59[0]) + 9.99999968e-21f);
			float fTemp25 = (fVec28[((IOTA - iConst41) & 16383)] - (fTemp22 + (0.600000024f * fRec57[1])));
			fVec29[(IOTA & 2047)] = fTemp25;
			fRec57[0] = fVec29[((IOTA - iConst42) & 2047)];
			float fRec58 = (0.600000024f * fTemp25);
			fRec62[0] = ((fSlow133 * fRec62[1]) + (fSlow134 * fRec36[1]));
			fVec30[(IOTA & 16383)] = ((0.353553385f * fRec62[0]) + 9.99999968e-21f);
			float fTemp26 = (fVec30[((IOTA - iConst15) & 16383)] - (fTemp22 + (0.600000024f * fRec60[1])));
			fVec31[(IOTA & 4095)] = fTemp26;
			fRec60[0] = fVec31[((IOTA - iConst17) & 4095)];
			float fRec61 = (0.600000024f * fTemp26);
			fRec65[0] = ((fSlow141 * fRec65[1]) + (fSlow142 * fRec38[1]));
			fVec32[(IOTA & 16383)] = ((0.353553385f * fRec65[0]) + 9.99999968e-21f);
			float fTemp27 = ((fTemp22 + fVec32[((IOTA - iConst21) & 16383)]) - (0.600000024f * fRec63[1]));
			fVec33[(IOTA & 4095)] = fTemp27;
			fRec63[0] = fVec33[((IOTA - iConst22) & 4095)];
			float fRec64 = (0.600000024f * fTemp27);
			float fTemp28 = (fRec52 + fRec64);
			float fTemp29 = (fRec61 + fTemp28);
			fRec34[0] = (fRec42[1] + (fRec45[1] + (fRec48[1] + (fRec51[1] + (fRec54[1] + (fRec57[1] + (fRec60[1] + (fRec63[1] + (fRec55 + (fRec43 + (fRec58 + (fRec46 + (fRec49 + fTemp29)))))))))))));
			fRec35[0] = ((fRec51[1] + (fRec57[1] + (fRec60[1] + (fRec63[1] + (fRec58 + fTemp29))))) - (fRec42[1] + (fRec45[1] + (fRec48[1] + (fRec54[1] + (fRec55 + (fRec43 + (fRec49 + fRec46))))))));
			fRec36[0] = ((fRec45[1] + (fRec48[1] + (fRec51[1] + (fRec63[1] + (fRec46 + (fRec49 + fTemp28)))))) - (fRec42[1] + (fRec54[1] + (fRec57[1] + (fRec60[1] + (fRec55 + (fRec43 + (fRec61 + fRec58))))))));
			fRec37[0] = ((fRec42[1] + (fRec51[1] + (fRec54[1] + (fRec63[1] + (fRec55 + (fRec43 + fTemp28)))))) - (fRec45[1] + (fRec48[1] + (fRec57[1] + (fRec60[1] + (fRec58 + (fRec46 + (fRec61 + fRec49))))))));
			float fTemp30 = (fRec52 + fRec61);
			fRec38[0] = ((fRec42[1] + (fRec48[1] + (fRec51[1] + (fRec60[1] + (fRec43 + (fRec49 + fTemp30)))))) - (fRec45[1] + (fRec54[1] + (fRec57[1] + (fRec63[1] + (fRec55 + (fRec58 + (fRec64 + fRec46))))))));
			fRec39[0] = ((fRec45[1] + (fRec51[1] + (fRec54[1] + (fRec60[1] + (fRec55 + (fRec46 + fTemp30)))))) - (fRec42[1] + (fRec48[1] + (fRec57[1] + (fRec63[1] + (fRec43 + (fRec58 + (fRec64 + fRec49))))))));
			float fTemp31 = (fRec64 + fRec61);
			fRec40[0] = ((fRec48[1] + (fRec51[1] + (fRec54[1] + (fRec57[1] + (fRec55 + (fRec58 + (fRec52 + fRec49))))))) - (fRec42[1] + (fRec45[1] + (fRec60[1] + (fRec63[1] + (fRec43 + (fRec46 + fTemp31)))))));
			fRec41[0] = ((fRec42[1] + (fRec45[1] + (fRec51[1] + (fRec57[1] + (fRec43 + (fRec58 + (fRec52 + fRec46))))))) - (fRec48[1] + (fRec54[1] + (fRec60[1] + (fRec63[1] + (fRec55 + (fRec49 + fTemp31)))))));
			float fTemp32 = ((fSlow76 * (fRec35[0] + fRec36[0])) + (fSlow143 * fTemp21));
			fRec66[0] = (fSlow147 + (0.999000013f * fRec66[1]));
			float fTemp33 = float(input1[i]);
			fVec34[(IOTA & 16383)] = fTemp33;
			float fTemp34 = (0.300000012f * fVec34[((IOTA - iSlow149) & 16383)]);
			fRec77[0] = ((fSlow157 * fRec77[1]) + (fSlow158 * fRec71[1]));
			fVec35[(IOTA & 16383)] = ((0.353553385f * fRec77[0]) + 9.99999968e-21f);
			float fTemp35 = ((fTemp34 + fVec35[((IOTA - iConst21) & 16383)]) - (0.600000024f * fRec75[1]));
			fVec36[(IOTA & 4095)] = fTemp35;
			fRec75[0] = fVec36[((IOTA - iConst22) & 4095)];
			float fRec76 = (0.600000024f * fTemp35);
			fRec80[0] = ((fSlow165 * fRec80[1]) + (fSlow166 * fRec67[1]));
			fVec37[(IOTA & 16383)] = ((0.353553385f * fRec80[0]) + 9.99999968e-21f);
			float fTemp36 = ((fVec37[((IOTA - iConst26) & 16383)] + fTemp34) - (0.600000024f * fRec78[1]));
			fVec38[(IOTA & 2047)] = fTemp36;
			fRec78[0] = fVec38[((IOTA - iConst27) & 2047)];
			float fRec79 = (0.600000024f * fTemp36);
			fRec83[0] = ((fSlow173 * fRec83[1]) + (fSlow174 * fRec70[1]));
			fVec39[(IOTA & 32767)] = ((0.353553385f * fRec83[0]) + 9.99999968e-21f);
			float fTemp37 = ((0.600000024f * fRec81[1]) + fVec39[((IOTA - iConst10) & 32767)]);
			fVec40[(IOTA & 4095)] = fTemp37;
			fRec81[0] = fVec40[((IOTA - iConst11) & 4095)];
			float fRec82 = (0.0f - (0.600000024f * fTemp37));
			fRec86[0] = ((fSlow181 * fRec86[1]) + (fSlow182 * fRec72[1]));
			fVec41[(IOTA & 16383)] = ((0.353553385f * fRec86[0]) + 9.99999968e-21f);
			float fTemp38 = ((0.600000024f * fRec84[1]) + fVec41[((IOTA - iConst31) & 16383)]);
			fVec42[(IOTA & 4095)] = fTemp38;
			fRec84[0] = fVec42[((IOTA - iConst32) & 4095)];
			float fRec85 = (0.0f - (0.600000024f * fTemp38));
			fRec89[0] = ((fSlow189 * fRec89[1]) + (fSlow190 * fRec68[1]));
			fVec43[(IOTA & 32767)] = ((0.353553385f * fRec89[0]) + 9.99999968e-21f);
			float fTemp39 = ((0.600000024f * fRec87[1]) + fVec43[((IOTA - iConst36) & 32767)]);
			fVec44[(IOTA & 4095)] = fTemp39;
			fRec87[0] = fVec44[((IOTA - iConst37) & 4095)];
			float fRec88 = (0.0f - (0.600000024f * fTemp39));
			fRec92[0] = ((fSlow197 * fRec92[1]) + (fSlow198 * fRec73[1]));
			fVec45[(IOTA & 16383)] = ((0.353553385f * fRec92[0]) + 9.99999968e-21f);
			float fTemp40 = (fVec45[((IOTA - iConst41) & 16383)] - (fTemp34 + (0.600000024f * fRec90[1])));
			fVec46[(IOTA & 2047)] = fTemp40;
			fRec90[0] = fVec46[((IOTA - iConst42) & 2047)];
			float fRec91 = (0.600000024f * fTemp40);
			fRec95[0] = ((fSlow205 * fRec95[1]) + (fSlow206 * fRec69[1]));
			fVec47[(IOTA & 16383)] = ((0.353553385f * fRec95[0]) + 9.99999968e-21f);
			float fTemp41 = (fVec47[((IOTA - iConst15) & 16383)] - (fTemp34 + (0.600000024f * fRec93[1])));
			fVec48[(IOTA & 4095)] = fTemp41;
			fRec93[0] = fVec48[((IOTA - iConst17) & 4095)];
			float fRec94 = (0.600000024f * fTemp41);
			fRec98[0] = ((fSlow213 * fRec98[1]) + (fSlow214 * fRec74[1]));
			fVec49[(IOTA & 32767)] = ((0.353553385f * fRec98[0]) + 9.99999968e-21f);
			float fTemp42 = ((0.600000024f * fRec96[1]) + fVec49[((IOTA - iConst5) & 32767)]);
			fVec50[(IOTA & 2047)] = fTemp42;
			fRec96[0] = fVec50[((IOTA - iConst6) & 2047)];
			float fRec97 = (0.0f - (0.600000024f * fTemp42));
			float fTemp43 = (fRec91 + fRec88);
			float fTemp44 = (fRec85 + fTemp43);
			fRec67[0] = (fRec75[1] + (fRec78[1] + (fRec81[1] + (fRec84[1] + (fRec87[1] + (fRec90[1] + (fRec93[1] + (fRec96[1] + (fRec94 + (fRec76 + (fRec79 + (fRec97 + (fRec82 + fTemp44)))))))))))));
			float fTemp45 = (fRec88 + fRec85);
			fRec68[0] = ((fRec75[1] + (fRec78[1] + (fRec90[1] + (fRec93[1] + (fRec94 + (fRec76 + (fRec79 + fRec91))))))) - (fRec81[1] + (fRec84[1] + (fRec87[1] + (fRec96[1] + (fRec97 + (fRec82 + fTemp45)))))));
			fRec69[0] = ((fRec75[1] + (fRec78[1] + (fRec84[1] + (fRec87[1] + (fRec76 + (fRec79 + fTemp45)))))) - (fRec81[1] + (fRec90[1] + (fRec93[1] + (fRec96[1] + (fRec94 + (fRec97 + (fRec91 + fRec82))))))));
			fRec70[0] = ((fRec75[1] + (fRec78[1] + (fRec81[1] + (fRec96[1] + (fRec76 + (fRec79 + (fRec82 + fRec97))))))) - (fRec84[1] + (fRec87[1] + (fRec90[1] + (fRec93[1] + (fRec94 + fTemp44))))));
			float fTemp46 = (fRec91 + fRec85);
			fRec71[0] = ((fRec78[1] + (fRec81[1] + (fRec87[1] + (fRec93[1] + (fRec94 + (fRec79 + (fRec88 + fRec82))))))) - (fRec75[1] + (fRec84[1] + (fRec90[1] + (fRec96[1] + (fRec76 + (fRec97 + fTemp46)))))));
			fRec72[0] = ((fRec78[1] + (fRec84[1] + (fRec93[1] + (fRec96[1] + (fRec94 + (fRec79 + (fRec85 + fRec97))))))) - (fRec75[1] + (fRec81[1] + (fRec87[1] + (fRec90[1] + (fRec76 + (fRec82 + fTemp43)))))));
			fRec73[0] = ((fRec78[1] + (fRec87[1] + (fRec90[1] + (fRec96[1] + (fRec79 + (fRec97 + fTemp43)))))) - (fRec75[1] + (fRec81[1] + (fRec84[1] + (fRec93[1] + (fRec94 + (fRec76 + (fRec85 + fRec82))))))));
			fRec74[0] = ((fRec78[1] + (fRec81[1] + (fRec84[1] + (fRec90[1] + (fRec79 + (fRec82 + fTemp46)))))) - (fRec75[1] + (fRec87[1] + (fRec93[1] + (fRec96[1] + (fRec94 + (fRec76 + (fRec88 + fRec97))))))));
			float fTemp47 = ((fSlow148 * (fRec68[0] + fRec69[0])) + (fSlow215 * fTemp33));
			fRec99[0] = (fSlow219 + (0.999000013f * fRec99[1]));
			fRec110[0] = ((fSlow228 * fRec110[1]) + (fSlow229 * fRec106[1]));
			fVec51[(IOTA & 16383)] = ((0.353553385f * fRec110[0]) + 9.99999968e-21f);
			float fTemp48 = float(input0[i]);
			fVec52[(IOTA & 16383)] = fTemp48;
			float fTemp49 = (0.300000012f * fVec52[((IOTA - iSlow230) & 16383)]);
			float fTemp50 = (fVec51[((IOTA - iConst41) & 16383)] - (fTemp49 + (0.600000024f * fRec108[1])));
			fVec53[(IOTA & 2047)] = fTemp50;
			fRec108[0] = fVec53[((IOTA - iConst42) & 2047)];
			float fRec109 = (0.600000024f * fTemp50);
			fRec113[0] = ((fSlow237 * fRec113[1]) + (fSlow238 * fRec102[1]));
			fVec54[(IOTA & 16383)] = ((0.353553385f * fRec113[0]) + 9.99999968e-21f);
			float fTemp51 = (fVec54[((IOTA - iConst15) & 16383)] - (fTemp49 + (0.600000024f * fRec111[1])));
			fVec55[(IOTA & 4095)] = fTemp51;
			fRec111[0] = fVec55[((IOTA - iConst17) & 4095)];
			float fRec112 = (0.600000024f * fTemp51);
			fRec116[0] = ((fSlow245 * fRec116[1]) + (fSlow246 * fRec104[1]));
			fVec56[(IOTA & 16383)] = ((0.353553385f * fRec116[0]) + 9.99999968e-21f);
			float fTemp52 = ((fTemp49 + fVec56[((IOTA - iConst21) & 16383)]) - (0.600000024f * fRec114[1]));
			fVec57[(IOTA & 4095)] = fTemp52;
			fRec114[0] = fVec57[((IOTA - iConst22) & 4095)];
			float fRec115 = (0.600000024f * fTemp52);
			fRec119[0] = ((fSlow253 * fRec119[1]) + (fSlow254 * fRec107[1]));
			fVec58[(IOTA & 32767)] = ((0.353553385f * fRec119[0]) + 9.99999968e-21f);
			float fTemp53 = ((0.600000024f * fRec117[1]) + fVec58[((IOTA - iConst5) & 32767)]);
			fVec59[(IOTA & 2047)] = fTemp53;
			fRec117[0] = fVec59[((IOTA - iConst6) & 2047)];
			float fRec118 = (0.0f - (0.600000024f * fTemp53));
			fRec122[0] = ((fSlow261 * fRec122[1]) + (fSlow262 * fRec103[1]));
			fVec60[(IOTA & 32767)] = ((0.353553385f * fRec122[0]) + 9.99999968e-21f);
			float fTemp54 = ((0.600000024f * fRec120[1]) + fVec60[((IOTA - iConst10) & 32767)]);
			fVec61[(IOTA & 4095)] = fTemp54;
			fRec120[0] = fVec61[((IOTA - iConst11) & 4095)];
			float fRec121 = (0.0f - (0.600000024f * fTemp54));
			fRec125[0] = ((fSlow269 * fRec125[1]) + (fSlow270 * fRec105[1]));
			fVec62[(IOTA & 16383)] = ((0.353553385f * fRec125[0]) + 9.99999968e-21f);
			float fTemp55 = ((0.600000024f * fRec123[1]) + fVec62[((IOTA - iConst31) & 16383)]);
			fVec63[(IOTA & 4095)] = fTemp55;
			fRec123[0] = fVec63[((IOTA - iConst32) & 4095)];
			float fRec124 = (0.0f - (0.600000024f * fTemp55));
			fRec128[0] = ((fSlow277 * fRec128[1]) + (fSlow278 * fRec101[1]));
			fVec64[(IOTA & 32767)] = ((0.353553385f * fRec128[0]) + 9.99999968e-21f);
			float fTemp56 = ((0.600000024f * fRec126[1]) + fVec64[((IOTA - iConst36) & 32767)]);
			fVec65[(IOTA & 4095)] = fTemp56;
			fRec126[0] = fVec65[((IOTA - iConst37) & 4095)];
			float fRec127 = (0.0f - (0.600000024f * fTemp56));
			fRec131[0] = ((fSlow285 * fRec131[1]) + (fSlow286 * fRec100[1]));
			fVec66[(IOTA & 16383)] = ((0.353553385f * fRec131[0]) + 9.99999968e-21f);
			float fTemp57 = ((fVec66[((IOTA - iConst26) & 16383)] + fTemp49) - (0.600000024f * fRec129[1]));
			fVec67[(IOTA & 2047)] = fTemp57;
			fRec129[0] = fVec67[((IOTA - iConst27) & 2047)];
			float fRec130 = (0.600000024f * fTemp57);
			float fTemp58 = (fRec130 + fRec115);
			float fTemp59 = (fRec109 + (fRec112 + fTemp58));
			fRec100[0] = (fRec108[1] + (fRec111[1] + (fRec114[1] + (fRec117[1] + (fRec120[1] + (fRec123[1] + (fRec126[1] + (fRec129[1] + (fRec118 + (fRec121 + (fRec124 + (fRec127 + fTemp59))))))))))));
			fRec101[0] = ((fRec108[1] + (fRec111[1] + (fRec114[1] + (fRec129[1] + fTemp59)))) - (fRec117[1] + (fRec120[1] + (fRec123[1] + (fRec126[1] + (fRec118 + (fRec121 + (fRec127 + fRec124))))))));
			float fTemp60 = (fRec112 + fRec109);
			fRec102[0] = ((fRec114[1] + (fRec123[1] + (fRec126[1] + (fRec129[1] + (fRec124 + (fRec127 + fTemp58)))))) - (fRec108[1] + (fRec111[1] + (fRec117[1] + (fRec120[1] + (fRec118 + (fRec121 + fTemp60)))))));
			fRec103[0] = ((fRec114[1] + (fRec117[1] + (fRec120[1] + (fRec129[1] + (fRec118 + (fRec121 + fTemp58)))))) - (fRec108[1] + (fRec111[1] + (fRec123[1] + (fRec126[1] + (fRec124 + (fRec127 + fTemp60)))))));
			float fTemp61 = (fRec130 + fRec112);
			float fTemp62 = (fRec115 + fRec109);
			fRec104[0] = ((fRec111[1] + (fRec120[1] + (fRec126[1] + (fRec129[1] + (fRec121 + (fRec127 + fTemp61)))))) - (fRec108[1] + (fRec114[1] + (fRec117[1] + (fRec123[1] + (fRec118 + (fRec124 + fTemp62)))))));
			fRec105[0] = ((fRec111[1] + (fRec117[1] + (fRec123[1] + (fRec129[1] + (fRec118 + (fRec124 + fTemp61)))))) - (fRec108[1] + (fRec114[1] + (fRec120[1] + (fRec126[1] + (fRec121 + (fRec127 + fTemp62)))))));
			float fTemp63 = (fRec130 + fRec109);
			float fTemp64 = (fRec115 + fRec112);
			fRec106[0] = ((fRec108[1] + (fRec117[1] + (fRec126[1] + (fRec129[1] + (fRec118 + (fRec127 + fTemp63)))))) - (fRec111[1] + (fRec114[1] + (fRec120[1] + (fRec123[1] + (fRec121 + (fRec124 + fTemp64)))))));
			fRec107[0] = ((fRec108[1] + (fRec120[1] + (fRec123[1] + (fRec129[1] + (fRec121 + (fRec124 + fTemp63)))))) - (fRec111[1] + (fRec114[1] + (fRec117[1] + (fRec126[1] + (fRec118 + (fRec127 + fTemp64)))))));
			float fTemp65 = ((fSlow220 * (fRec101[0] + fRec102[0])) + (fSlow287 * fTemp48));
			fRec132[0] = (fSlow291 + (0.999000013f * fRec132[1]));
			fRec143[0] = ((fSlow300 * fRec143[1]) + (fSlow301 * fRec134[1]));
			fVec68[(IOTA & 32767)] = ((0.353553385f * fRec143[0]) + 9.99999968e-21f);
			float fTemp66 = ((0.600000024f * fRec141[1]) + fVec68[((IOTA - iConst36) & 32767)]);
			fVec69[(IOTA & 4095)] = fTemp66;
			fRec141[0] = fVec69[((IOTA - iConst37) & 4095)];
			float fRec142 = (0.0f - (0.600000024f * fTemp66));
			fRec146[0] = ((fSlow308 * fRec146[1]) + (fSlow309 * fRec139[1]));
			fVec70[(IOTA & 16383)] = ((0.353553385f * fRec146[0]) + 9.99999968e-21f);
			float fTemp67 = float(input4[i]);
			fVec71[(IOTA & 16383)] = fTemp67;
			float fTemp68 = (0.300000012f * fVec71[((IOTA - iSlow310) & 16383)]);
			float fTemp69 = (fVec70[((IOTA - iConst41) & 16383)] - (fTemp68 + (0.600000024f * fRec144[1])));
			fVec72[(IOTA & 2047)] = fTemp69;
			fRec144[0] = fVec72[((IOTA - iConst42) & 2047)];
			float fRec145 = (0.600000024f * fTemp69);
			fRec149[0] = ((fSlow317 * fRec149[1]) + (fSlow318 * fRec140[1]));
			fVec73[(IOTA & 32767)] = ((0.353553385f * fRec149[0]) + 9.99999968e-21f);
			float fTemp70 = ((0.600000024f * fRec147[1]) + fVec73[((IOTA - iConst5) & 32767)]);
			fVec74[(IOTA & 2047)] = fTemp70;
			fRec147[0] = fVec74[((IOTA - iConst6) & 2047)];
			float fRec148 = (0.0f - (0.600000024f * fTemp70));
			fRec152[0] = ((fSlow325 * fRec152[1]) + (fSlow326 * fRec136[1]));
			fVec75[(IOTA & 32767)] = ((0.353553385f * fRec152[0]) + 9.99999968e-21f);
			float fTemp71 = ((0.600000024f * fRec150[1]) + fVec75[((IOTA - iConst10) & 32767)]);
			fVec76[(IOTA & 4095)] = fTemp71;
			fRec150[0] = fVec76[((IOTA - iConst11) & 4095)];
			float fRec151 = (0.0f - (0.600000024f * fTemp71));
			fRec155[0] = ((fSlow333 * fRec155[1]) + (fSlow334 * fRec138[1]));
			fVec77[(IOTA & 16383)] = ((0.353553385f * fRec155[0]) + 9.99999968e-21f);
			float fTemp72 = ((0.600000024f * fRec153[1]) + fVec77[((IOTA - iConst31) & 16383)]);
			fVec78[(IOTA & 4095)] = fTemp72;
			fRec153[0] = fVec78[((IOTA - iConst32) & 4095)];
			float fRec154 = (0.0f - (0.600000024f * fTemp72));
			fRec158[0] = ((fSlow341 * fRec158[1]) + (fSlow342 * fRec133[1]));
			fVec79[(IOTA & 16383)] = ((0.353553385f * fRec158[0]) + 9.99999968e-21f);
			float fTemp73 = ((fVec79[((IOTA - iConst26) & 16383)] + fTemp68) - (0.600000024f * fRec156[1]));
			fVec80[(IOTA & 2047)] = fTemp73;
			fRec156[0] = fVec80[((IOTA - iConst27) & 2047)];
			float fRec157 = (0.600000024f * fTemp73);
			fRec161[0] = ((fSlow349 * fRec161[1]) + (fSlow350 * fRec135[1]));
			fVec81[(IOTA & 16383)] = ((0.353553385f * fRec161[0]) + 9.99999968e-21f);
			float fTemp74 = (fVec81[((IOTA - iConst15) & 16383)] - (fTemp68 + (0.600000024f * fRec159[1])));
			fVec82[(IOTA & 4095)] = fTemp74;
			fRec159[0] = fVec82[((IOTA - iConst17) & 4095)];
			float fRec160 = (0.600000024f * fTemp74);
			fRec164[0] = ((fSlow357 * fRec164[1]) + (fSlow358 * fRec137[1]));
			fVec83[(IOTA & 16383)] = ((0.353553385f * fRec164[0]) + 9.99999968e-21f);
			float fTemp75 = ((fTemp68 + fVec83[((IOTA - iConst21) & 16383)]) - (0.600000024f * fRec162[1]));
			fVec84[(IOTA & 4095)] = fTemp75;
			fRec162[0] = fVec84[((IOTA - iConst22) & 4095)];
			float fRec163 = (0.600000024f * fTemp75);
			float fTemp76 = (fRec157 + fRec163);
			float fTemp77 = (fRec145 + fTemp76);
			fRec133[0] = (fRec141[1] + (fRec144[1] + (fRec147[1] + (fRec150[1] + (fRec153[1] + (fRec156[1] + (fRec159[1] + (fRec162[1] + (fRec160 + (fRec148 + (fRec151 + (fRec154 + (fRec142 + fTemp77)))))))))))));
			fRec134[0] = ((fRec144[1] + (fRec156[1] + (fRec159[1] + (fRec162[1] + (fRec160 + fTemp77))))) - (fRec141[1] + (fRec147[1] + (fRec150[1] + (fRec153[1] + (fRec148 + (fRec151 + (fRec142 + fRec154))))))));
			fRec135[0] = ((fRec141[1] + (fRec153[1] + (fRec156[1] + (fRec162[1] + (fRec154 + (fRec142 + fTemp76)))))) - (fRec144[1] + (fRec147[1] + (fRec150[1] + (fRec159[1] + (fRec160 + (fRec148 + (fRec145 + fRec151))))))));
			fRec136[0] = ((fRec147[1] + (fRec150[1] + (fRec156[1] + (fRec162[1] + (fRec148 + (fRec151 + fTemp76)))))) - (fRec141[1] + (fRec144[1] + (fRec153[1] + (fRec159[1] + (fRec160 + (fRec154 + (fRec145 + fRec142))))))));
			float fTemp78 = (fRec163 + fRec145);
			fRec137[0] = ((fRec141[1] + (fRec150[1] + (fRec156[1] + (fRec159[1] + (fRec160 + (fRec151 + (fRec157 + fRec142))))))) - (fRec144[1] + (fRec147[1] + (fRec153[1] + (fRec162[1] + (fRec148 + (fRec154 + fTemp78)))))));
			fRec138[0] = ((fRec147[1] + (fRec153[1] + (fRec156[1] + (fRec159[1] + (fRec160 + (fRec148 + (fRec157 + fRec154))))))) - (fRec141[1] + (fRec144[1] + (fRec150[1] + (fRec162[1] + (fRec151 + (fRec142 + fTemp78)))))));
			float fTemp79 = (fRec157 + fRec145);
			fRec139[0] = ((fRec141[1] + (fRec144[1] + (fRec147[1] + (fRec156[1] + (fRec148 + (fRec142 + fTemp79)))))) - (fRec150[1] + (fRec153[1] + (fRec159[1] + (fRec162[1] + (fRec160 + (fRec151 + (fRec163 + fRec154))))))));
			fRec140[0] = ((fRec144[1] + (fRec150[1] + (fRec153[1] + (fRec156[1] + (fRec151 + (fRec154 + fTemp79)))))) - (fRec141[1] + (fRec147[1] + (fRec159[1] + (fRec162[1] + (fRec160 + (fRec148 + (fRec163 + fRec142))))))));
			float fTemp80 = ((fSlow292 * (fRec134[0] + fRec135[0])) + (fSlow359 * fTemp67));
			fRec165[0] = (fSlow363 + (0.999000013f * fRec165[1]));
			fRec176[0] = ((fSlow372 * fRec176[1]) + (fSlow373 * fRec173[1]));
			fVec85[(IOTA & 32767)] = ((0.353553385f * fRec176[0]) + 9.99999968e-21f);
			float fTemp81 = ((0.600000024f * fRec174[1]) + fVec85[((IOTA - iConst5) & 32767)]);
			fVec86[(IOTA & 2047)] = fTemp81;
			fRec174[0] = fVec86[((IOTA - iConst6) & 2047)];
			float fRec175 = (0.0f - (0.600000024f * fTemp81));
			fRec179[0] = ((fSlow380 * fRec179[1]) + (fSlow381 * fRec171[1]));
			fVec87[(IOTA & 16383)] = ((0.353553385f * fRec179[0]) + 9.99999968e-21f);
			float fTemp82 = ((0.600000024f * fRec177[1]) + fVec87[((IOTA - iConst31) & 16383)]);
			fVec88[(IOTA & 4095)] = fTemp82;
			fRec177[0] = fVec88[((IOTA - iConst32) & 4095)];
			float fRec178 = (0.0f - (0.600000024f * fTemp82));
			fRec182[0] = ((fSlow388 * fRec182[1]) + (fSlow389 * fRec169[1]));
			fVec89[(IOTA & 32767)] = ((0.353553385f * fRec182[0]) + 9.99999968e-21f);
			float fTemp83 = ((0.600000024f * fRec180[1]) + fVec89[((IOTA - iConst10) & 32767)]);
			fVec90[(IOTA & 4095)] = fTemp83;
			fRec180[0] = fVec90[((IOTA - iConst11) & 4095)];
			float fRec181 = (0.0f - (0.600000024f * fTemp83));
			fRec185[0] = ((fSlow396 * fRec185[1]) + (fSlow397 * fRec167[1]));
			fVec91[(IOTA & 32767)] = ((0.353553385f * fRec185[0]) + 9.99999968e-21f);
			float fTemp84 = ((0.600000024f * fRec183[1]) + fVec91[((IOTA - iConst36) & 32767)]);
			fVec92[(IOTA & 4095)] = fTemp84;
			fRec183[0] = fVec92[((IOTA - iConst37) & 4095)];
			float fRec184 = (0.0f - (0.600000024f * fTemp84));
			fRec188[0] = ((fSlow404 * fRec188[1]) + (fSlow405 * fRec172[1]));
			fVec93[(IOTA & 16383)] = ((0.353553385f * fRec188[0]) + 9.99999968e-21f);
			float fTemp85 = float(input5[i]);
			fVec94[(IOTA & 16383)] = fTemp85;
			float fTemp86 = (0.300000012f * fVec94[((IOTA - iSlow406) & 16383)]);
			float fTemp87 = (fVec93[((IOTA - iConst41) & 16383)] - (fTemp86 + (0.600000024f * fRec186[1])));
			fVec95[(IOTA & 2047)] = fTemp87;
			fRec186[0] = fVec95[((IOTA - iConst42) & 2047)];
			float fRec187 = (0.600000024f * fTemp87);
			fRec191[0] = ((fSlow413 * fRec191[1]) + (fSlow414 * fRec168[1]));
			fVec96[(IOTA & 16383)] = ((0.353553385f * fRec191[0]) + 9.99999968e-21f);
			float fTemp88 = (fVec96[((IOTA - iConst15) & 16383)] - (fTemp86 + (0.600000024f * fRec189[1])));
			fVec97[(IOTA & 4095)] = fTemp88;
			fRec189[0] = fVec97[((IOTA - iConst17) & 4095)];
			float fRec190 = (0.600000024f * fTemp88);
			fRec194[0] = ((fSlow421 * fRec194[1]) + (fSlow422 * fRec170[1]));
			fVec98[(IOTA & 16383)] = ((0.353553385f * fRec194[0]) + 9.99999968e-21f);
			float fTemp89 = ((fTemp86 + fVec98[((IOTA - iConst21) & 16383)]) - (0.600000024f * fRec192[1]));
			fVec99[(IOTA & 4095)] = fTemp89;
			fRec192[0] = fVec99[((IOTA - iConst22) & 4095)];
			float fRec193 = (0.600000024f * fTemp89);
			fRec197[0] = ((fSlow429 * fRec197[1]) + (fSlow430 * fRec166[1]));
			fVec100[(IOTA & 16383)] = ((0.353553385f * fRec197[0]) + 9.99999968e-21f);
			float fTemp90 = ((fVec100[((IOTA - iConst26) & 16383)] + fTemp86) - (0.600000024f * fRec195[1]));
			fVec101[(IOTA & 2047)] = fTemp90;
			fRec195[0] = fVec101[((IOTA - iConst27) & 2047)];
			float fRec196 = (0.600000024f * fTemp90);
			float fTemp91 = (fRec196 + fRec178);
			float fTemp92 = (fRec181 + fTemp91);
			fRec166[0] = (fRec174[1] + (fRec177[1] + (fRec180[1] + (fRec183[1] + (fRec186[1] + (fRec189[1] + (fRec192[1] + (fRec195[1] + (fRec175 + (fRec187 + (fRec184 + (fRec190 + (fRec193 + fTemp92)))))))))))));
			float fTemp93 = (fRec178 + fRec181);
			fRec167[0] = ((fRec186[1] + (fRec189[1] + (fRec192[1] + (fRec195[1] + (fRec187 + (fRec190 + (fRec196 + fRec193))))))) - (fRec174[1] + (fRec177[1] + (fRec180[1] + (fRec183[1] + (fRec175 + (fRec184 + fTemp93)))))));
			fRec168[0] = ((fRec177[1] + (fRec183[1] + (fRec192[1] + (fRec195[1] + (fRec184 + (fRec193 + fTemp91)))))) - (fRec174[1] + (fRec180[1] + (fRec186[1] + (fRec189[1] + (fRec175 + (fRec187 + (fRec190 + fRec181))))))));
			float fTemp94 = (fRec196 + fRec181);
			fRec169[0] = ((fRec174[1] + (fRec180[1] + (fRec192[1] + (fRec195[1] + (fRec175 + (fRec193 + fTemp94)))))) - (fRec177[1] + (fRec183[1] + (fRec186[1] + (fRec189[1] + (fRec187 + (fRec184 + (fRec190 + fRec178))))))));
			fRec170[0] = ((fRec180[1] + (fRec183[1] + (fRec189[1] + (fRec195[1] + (fRec184 + (fRec190 + fTemp94)))))) - (fRec174[1] + (fRec177[1] + (fRec186[1] + (fRec192[1] + (fRec175 + (fRec187 + (fRec193 + fRec178))))))));
			fRec171[0] = ((fRec174[1] + (fRec177[1] + (fRec189[1] + (fRec195[1] + (fRec175 + (fRec190 + fTemp91)))))) - (fRec180[1] + (fRec183[1] + (fRec186[1] + (fRec192[1] + (fRec187 + (fRec184 + (fRec193 + fRec181))))))));
			fRec172[0] = ((fRec174[1] + (fRec183[1] + (fRec186[1] + (fRec195[1] + (fRec175 + (fRec187 + (fRec196 + fRec184))))))) - (fRec177[1] + (fRec180[1] + (fRec189[1] + (fRec192[1] + (fRec190 + (fRec193 + fTemp93)))))));
			fRec173[0] = ((fRec177[1] + (fRec180[1] + (fRec186[1] + (fRec195[1] + (fRec187 + fTemp92))))) - (fRec174[1] + (fRec183[1] + (fRec189[1] + (fRec192[1] + (fRec175 + (fRec184 + (fRec193 + fRec190))))))));
			float fTemp95 = ((fSlow364 * (fRec167[0] + fRec168[0])) + (fSlow431 * fTemp85));
			output0[i] = FAUSTFLOAT((((fRec0[0] * fTemp17) + ((fRec33[0] * fTemp32) + ((fRec66[0] * fTemp47) + ((fRec99[0] * fTemp65) + (fRec132[0] * fTemp80))))) + (fRec165[0] * fTemp95)));
			fRec198[0] = (fSlow432 + (0.999000013f * fRec198[1]));
			fRec199[0] = (fSlow433 + (0.999000013f * fRec199[1]));
			fRec200[0] = (fSlow434 + (0.999000013f * fRec200[1]));
			fRec201[0] = (fSlow435 + (0.999000013f * fRec201[1]));
			fRec202[0] = (fSlow436 + (0.999000013f * fRec202[1]));
			fRec203[0] = (fSlow437 + (0.999000013f * fRec203[1]));
			output1[i] = FAUSTFLOAT(((((((fRec198[0] * fTemp65) + (fRec199[0] * fTemp47)) + (fRec200[0] * fTemp32)) + (fRec201[0] * fTemp17)) + (fRec202[0] * fTemp80)) + (fRec203[0] * fTemp95)));
			fRec204[0] = (fSlow438 + (0.999000013f * fRec204[1]));
			fRec205[0] = (fSlow439 + (0.999000013f * fRec205[1]));
			fRec206[0] = (fSlow440 + (0.999000013f * fRec206[1]));
			fRec207[0] = (fSlow441 + (0.999000013f * fRec207[1]));
			fRec208[0] = (fSlow442 + (0.999000013f * fRec208[1]));
			fRec209[0] = (fSlow443 + (0.999000013f * fRec209[1]));
			output2[i] = FAUSTFLOAT(((((((fRec204[0] * fTemp65) + (fRec205[0] * fTemp47)) + (fRec206[0] * fTemp32)) + (fRec207[0] * fTemp17)) + (fRec208[0] * fTemp80)) + (fRec209[0] * fTemp95)));
			fRec210[0] = (fSlow444 + (0.999000013f * fRec210[1]));
			fRec211[0] = (fSlow445 + (0.999000013f * fRec211[1]));
			fRec212[0] = (fSlow446 + (0.999000013f * fRec212[1]));
			fRec213[0] = (fSlow447 + (0.999000013f * fRec213[1]));
			fRec214[0] = (fSlow448 + (0.999000013f * fRec214[1]));
			fRec215[0] = (fSlow449 + (0.999000013f * fRec215[1]));
			output3[i] = FAUSTFLOAT(((((((fRec210[0] * fTemp65) + (fRec211[0] * fTemp47)) + (fRec212[0] * fTemp32)) + (fRec213[0] * fTemp17)) + (fRec214[0] * fTemp80)) + (fRec215[0] * fTemp95)));
			fRec216[0] = (fSlow450 + (0.999000013f * fRec216[1]));
			fRec217[0] = (fSlow451 + (0.999000013f * fRec217[1]));
			fRec218[0] = (fSlow452 + (0.999000013f * fRec218[1]));
			fRec219[0] = (fSlow453 + (0.999000013f * fRec219[1]));
			fRec220[0] = (fSlow454 + (0.999000013f * fRec220[1]));
			fRec221[0] = (fSlow455 + (0.999000013f * fRec221[1]));
			output4[i] = FAUSTFLOAT(((fRec216[0] * fTemp65) + (((((fRec217[0] * fTemp47) + (fRec218[0] * fTemp32)) + (fRec219[0] * fTemp17)) + (fRec220[0] * fTemp80)) + (fRec221[0] * fTemp95))));
			fRec222[0] = (fSlow456 + (0.999000013f * fRec222[1]));
			fRec223[0] = (fSlow457 + (0.999000013f * fRec223[1]));
			fRec224[0] = (fSlow458 + (0.999000013f * fRec224[1]));
			fRec225[0] = (fSlow459 + (0.999000013f * fRec225[1]));
			fRec226[0] = (fSlow460 + (0.999000013f * fRec226[1]));
			fRec227[0] = (fSlow461 + (0.999000013f * fRec227[1]));
			output5[i] = FAUSTFLOAT(((((((fRec222[0] * fTemp65) + (fRec223[0] * fTemp47)) + (fRec224[0] * fTemp32)) + (fRec225[0] * fTemp17)) + (fRec226[0] * fTemp80)) + (fRec227[0] * fTemp95)));
			fRec228[0] = (fSlow462 + (0.999000013f * fRec228[1]));
			fRec229[0] = (fSlow463 + (0.999000013f * fRec229[1]));
			fRec230[0] = (fSlow464 + (0.999000013f * fRec230[1]));
			fRec231[0] = (fSlow465 + (0.999000013f * fRec231[1]));
			fRec232[0] = (fSlow466 + (0.999000013f * fRec232[1]));
			fRec233[0] = (fSlow467 + (0.999000013f * fRec233[1]));
			output6[i] = FAUSTFLOAT(((((((fRec228[0] * fTemp65) + (fRec229[0] * fTemp47)) + (fRec230[0] * fTemp32)) + (fRec231[0] * fTemp17)) + (fRec232[0] * fTemp80)) + (fRec233[0] * fTemp95)));
			fRec234[0] = (fSlow468 + (0.999000013f * fRec234[1]));
			fRec235[0] = (fSlow469 + (0.999000013f * fRec235[1]));
			fRec236[0] = (fSlow470 + (0.999000013f * fRec236[1]));
			fRec237[0] = (fSlow471 + (0.999000013f * fRec237[1]));
			fRec238[0] = (fSlow472 + (0.999000013f * fRec238[1]));
			fRec239[0] = (fSlow473 + (0.999000013f * fRec239[1]));
			output7[i] = FAUSTFLOAT(((((((fRec234[0] * fTemp65) + (fRec235[0] * fTemp47)) + (fRec236[0] * fTemp32)) + (fRec237[0] * fTemp17)) + (fRec238[0] * fTemp80)) + (fRec239[0] * fTemp95)));
			fRec0[1] = fRec0[0];
			fRec11[1] = fRec11[0];
			IOTA = (IOTA + 1);
			fRec9[1] = fRec9[0];
			fRec14[1] = fRec14[0];
			fRec12[1] = fRec12[0];
			fRec17[1] = fRec17[0];
			fRec15[1] = fRec15[0];
			fRec20[1] = fRec20[0];
			fRec18[1] = fRec18[0];
			fRec23[1] = fRec23[0];
			fRec21[1] = fRec21[0];
			fRec26[1] = fRec26[0];
			fRec24[1] = fRec24[0];
			fRec29[1] = fRec29[0];
			fRec27[1] = fRec27[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec33[1] = fRec33[0];
			fRec44[1] = fRec44[0];
			fRec42[1] = fRec42[0];
			fRec47[1] = fRec47[0];
			fRec45[1] = fRec45[0];
			fRec50[1] = fRec50[0];
			fRec48[1] = fRec48[0];
			fRec53[1] = fRec53[0];
			fRec51[1] = fRec51[0];
			fRec56[1] = fRec56[0];
			fRec54[1] = fRec54[0];
			fRec59[1] = fRec59[0];
			fRec57[1] = fRec57[0];
			fRec62[1] = fRec62[0];
			fRec60[1] = fRec60[0];
			fRec65[1] = fRec65[0];
			fRec63[1] = fRec63[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec37[1] = fRec37[0];
			fRec38[1] = fRec38[0];
			fRec39[1] = fRec39[0];
			fRec40[1] = fRec40[0];
			fRec41[1] = fRec41[0];
			fRec66[1] = fRec66[0];
			fRec77[1] = fRec77[0];
			fRec75[1] = fRec75[0];
			fRec80[1] = fRec80[0];
			fRec78[1] = fRec78[0];
			fRec83[1] = fRec83[0];
			fRec81[1] = fRec81[0];
			fRec86[1] = fRec86[0];
			fRec84[1] = fRec84[0];
			fRec89[1] = fRec89[0];
			fRec87[1] = fRec87[0];
			fRec92[1] = fRec92[0];
			fRec90[1] = fRec90[0];
			fRec95[1] = fRec95[0];
			fRec93[1] = fRec93[0];
			fRec98[1] = fRec98[0];
			fRec96[1] = fRec96[0];
			fRec67[1] = fRec67[0];
			fRec68[1] = fRec68[0];
			fRec69[1] = fRec69[0];
			fRec70[1] = fRec70[0];
			fRec71[1] = fRec71[0];
			fRec72[1] = fRec72[0];
			fRec73[1] = fRec73[0];
			fRec74[1] = fRec74[0];
			fRec99[1] = fRec99[0];
			fRec110[1] = fRec110[0];
			fRec108[1] = fRec108[0];
			fRec113[1] = fRec113[0];
			fRec111[1] = fRec111[0];
			fRec116[1] = fRec116[0];
			fRec114[1] = fRec114[0];
			fRec119[1] = fRec119[0];
			fRec117[1] = fRec117[0];
			fRec122[1] = fRec122[0];
			fRec120[1] = fRec120[0];
			fRec125[1] = fRec125[0];
			fRec123[1] = fRec123[0];
			fRec128[1] = fRec128[0];
			fRec126[1] = fRec126[0];
			fRec131[1] = fRec131[0];
			fRec129[1] = fRec129[0];
			fRec100[1] = fRec100[0];
			fRec101[1] = fRec101[0];
			fRec102[1] = fRec102[0];
			fRec103[1] = fRec103[0];
			fRec104[1] = fRec104[0];
			fRec105[1] = fRec105[0];
			fRec106[1] = fRec106[0];
			fRec107[1] = fRec107[0];
			fRec132[1] = fRec132[0];
			fRec143[1] = fRec143[0];
			fRec141[1] = fRec141[0];
			fRec146[1] = fRec146[0];
			fRec144[1] = fRec144[0];
			fRec149[1] = fRec149[0];
			fRec147[1] = fRec147[0];
			fRec152[1] = fRec152[0];
			fRec150[1] = fRec150[0];
			fRec155[1] = fRec155[0];
			fRec153[1] = fRec153[0];
			fRec158[1] = fRec158[0];
			fRec156[1] = fRec156[0];
			fRec161[1] = fRec161[0];
			fRec159[1] = fRec159[0];
			fRec164[1] = fRec164[0];
			fRec162[1] = fRec162[0];
			fRec133[1] = fRec133[0];
			fRec134[1] = fRec134[0];
			fRec135[1] = fRec135[0];
			fRec136[1] = fRec136[0];
			fRec137[1] = fRec137[0];
			fRec138[1] = fRec138[0];
			fRec139[1] = fRec139[0];
			fRec140[1] = fRec140[0];
			fRec165[1] = fRec165[0];
			fRec176[1] = fRec176[0];
			fRec174[1] = fRec174[0];
			fRec179[1] = fRec179[0];
			fRec177[1] = fRec177[0];
			fRec182[1] = fRec182[0];
			fRec180[1] = fRec180[0];
			fRec185[1] = fRec185[0];
			fRec183[1] = fRec183[0];
			fRec188[1] = fRec188[0];
			fRec186[1] = fRec186[0];
			fRec191[1] = fRec191[0];
			fRec189[1] = fRec189[0];
			fRec194[1] = fRec194[0];
			fRec192[1] = fRec192[0];
			fRec197[1] = fRec197[0];
			fRec195[1] = fRec195[0];
			fRec166[1] = fRec166[0];
			fRec167[1] = fRec167[0];
			fRec168[1] = fRec168[0];
			fRec169[1] = fRec169[0];
			fRec170[1] = fRec170[0];
			fRec171[1] = fRec171[0];
			fRec172[1] = fRec172[0];
			fRec173[1] = fRec173[0];
			fRec198[1] = fRec198[0];
			fRec199[1] = fRec199[0];
			fRec200[1] = fRec200[0];
			fRec201[1] = fRec201[0];
			fRec202[1] = fRec202[0];
			fRec203[1] = fRec203[0];
			fRec204[1] = fRec204[0];
			fRec205[1] = fRec205[0];
			fRec206[1] = fRec206[0];
			fRec207[1] = fRec207[0];
			fRec208[1] = fRec208[0];
			fRec209[1] = fRec209[0];
			fRec210[1] = fRec210[0];
			fRec211[1] = fRec211[0];
			fRec212[1] = fRec212[0];
			fRec213[1] = fRec213[0];
			fRec214[1] = fRec214[0];
			fRec215[1] = fRec215[0];
			fRec216[1] = fRec216[0];
			fRec217[1] = fRec217[0];
			fRec218[1] = fRec218[0];
			fRec219[1] = fRec219[0];
			fRec220[1] = fRec220[0];
			fRec221[1] = fRec221[0];
			fRec222[1] = fRec222[0];
			fRec223[1] = fRec223[0];
			fRec224[1] = fRec224[0];
			fRec225[1] = fRec225[0];
			fRec226[1] = fRec226[0];
			fRec227[1] = fRec227[0];
			fRec228[1] = fRec228[0];
			fRec229[1] = fRec229[0];
			fRec230[1] = fRec230[0];
			fRec231[1] = fRec231[0];
			fRec232[1] = fRec232[0];
			fRec233[1] = fRec233[0];
			fRec234[1] = fRec234[0];
			fRec235[1] = fRec235[0];
			fRec236[1] = fRec236[0];
			fRec237[1] = fRec237[0];
			fRec238[1] = fRec238[0];
			fRec239[1] = fRec239[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _spat_zita6x8_UI : public UI {
    static std::string name;
};

template <class T>
std::string _spat_zita6x8_UI<T>::name(sym(spat_zita6x8));

typedef _spat_zita6x8_UI<spat_zita6x8> spat_zita6x8_UI;

class faust_spat_zita6x8_tilde : public FaustExternal<spat_zita6x8, spat_zita6x8_UI> {
public:
    faust_spat_zita6x8_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
