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
	int IOTA;
	float fVec0[16384];
	FAUSTFLOAT fHslider0;
	float fRec1[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	float fConst4;
	float fConst5;
	float fConst6;
	float fRec13[2];
	float fRec12[2];
	float fVec1[65536];
	float fConst7;
	int iConst8;
	float fVec2[4096];
	int iConst9;
	float fRec10[2];
	float fConst10;
	float fConst11;
	float fRec17[2];
	float fRec16[2];
	float fVec3[32768];
	float fConst12;
	int iConst13;
	float fConst14;
	FAUSTFLOAT fVslider4;
	float fVec4[8192];
	int iConst15;
	float fRec14[2];
	float fConst16;
	float fConst17;
	float fRec21[2];
	float fRec20[2];
	float fVec5[65536];
	float fConst18;
	int iConst19;
	float fVec6[8192];
	int iConst20;
	float fRec18[2];
	float fConst21;
	float fConst22;
	float fRec25[2];
	float fRec24[2];
	float fVec7[32768];
	float fConst23;
	int iConst24;
	float fVec8[8192];
	int iConst25;
	float fRec22[2];
	float fConst26;
	float fConst27;
	float fRec29[2];
	float fRec28[2];
	float fVec9[65536];
	float fConst28;
	int iConst29;
	float fVec10[8192];
	int iConst30;
	float fRec26[2];
	float fConst31;
	float fConst32;
	float fRec33[2];
	float fRec32[2];
	float fVec11[32768];
	float fConst33;
	int iConst34;
	float fVec12[4096];
	int iConst35;
	float fRec30[2];
	float fConst36;
	float fConst37;
	float fRec37[2];
	float fRec36[2];
	float fVec13[32768];
	float fConst38;
	int iConst39;
	float fVec14[8192];
	int iConst40;
	float fRec34[2];
	float fConst41;
	float fConst42;
	float fRec41[2];
	float fRec40[2];
	float fVec15[32768];
	float fConst43;
	int iConst44;
	float fVec16[4096];
	int iConst45;
	float fRec38[2];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[3];
	float fRec6[3];
	float fRec7[3];
	float fRec8[3];
	float fRec9[3];
	FAUSTFLOAT fVslider5;
	FAUSTFLOAT fVslider6;
	float fRec42[2];
	float fVec17[16384];
	FAUSTFLOAT fHslider1;
	float fRec43[2];
	FAUSTFLOAT fVslider7;
	FAUSTFLOAT fVslider8;
	float fRec55[2];
	float fRec54[2];
	float fVec18[65536];
	float fVec19[4096];
	float fRec52[2];
	float fRec59[2];
	float fRec58[2];
	float fVec20[65536];
	float fVec21[8192];
	float fRec56[2];
	float fRec63[2];
	float fRec62[2];
	float fVec22[32768];
	float fVec23[8192];
	float fRec60[2];
	float fRec67[2];
	float fRec66[2];
	float fVec24[65536];
	float fVec25[8192];
	float fRec64[2];
	float fRec71[2];
	float fRec70[2];
	float fVec26[32768];
	FAUSTFLOAT fVslider9;
	float fVec27[4096];
	float fRec68[2];
	float fRec75[2];
	float fRec74[2];
	float fVec28[32768];
	float fVec29[8192];
	float fRec72[2];
	float fRec79[2];
	float fRec78[2];
	float fVec30[32768];
	float fVec31[8192];
	float fRec76[2];
	float fRec83[2];
	float fRec82[2];
	float fVec32[32768];
	float fVec33[4096];
	float fRec80[2];
	float fRec44[3];
	float fRec45[3];
	float fRec46[3];
	float fRec47[3];
	float fRec48[3];
	float fRec49[3];
	float fRec50[3];
	float fRec51[3];
	FAUSTFLOAT fVslider10;
	FAUSTFLOAT fVslider11;
	float fRec84[2];
	float fVec34[16384];
	FAUSTFLOAT fHslider2;
	float fRec85[2];
	FAUSTFLOAT fVslider12;
	FAUSTFLOAT fVslider13;
	float fRec97[2];
	float fRec96[2];
	float fVec35[65536];
	float fVec36[8192];
	float fRec94[2];
	float fRec101[2];
	float fRec100[2];
	float fVec37[32768];
	FAUSTFLOAT fVslider14;
	float fVec38[4096];
	float fRec98[2];
	float fRec105[2];
	float fRec104[2];
	float fVec39[32768];
	float fVec40[8192];
	float fRec102[2];
	float fRec109[2];
	float fRec108[2];
	float fVec41[32768];
	float fVec42[8192];
	float fRec106[2];
	float fRec113[2];
	float fRec112[2];
	float fVec43[65536];
	float fVec44[8192];
	float fRec110[2];
	float fRec117[2];
	float fRec116[2];
	float fVec45[32768];
	float fVec46[8192];
	float fRec114[2];
	float fRec121[2];
	float fRec120[2];
	float fVec47[32768];
	float fVec48[4096];
	float fRec118[2];
	float fRec125[2];
	float fRec124[2];
	float fVec49[65536];
	float fVec50[4096];
	float fRec122[2];
	float fRec86[3];
	float fRec87[3];
	float fRec88[3];
	float fRec89[3];
	float fRec90[3];
	float fRec91[3];
	float fRec92[3];
	float fRec93[3];
	FAUSTFLOAT fVslider15;
	FAUSTFLOAT fVslider16;
	float fRec126[2];
	float fVec51[16384];
	FAUSTFLOAT fHslider3;
	float fRec127[2];
	FAUSTFLOAT fVslider17;
	FAUSTFLOAT fVslider18;
	float fRec139[2];
	float fRec138[2];
	float fVec52[65536];
	float fVec53[8192];
	float fRec136[2];
	float fRec143[2];
	float fRec142[2];
	float fVec54[32768];
	FAUSTFLOAT fVslider19;
	float fVec55[4096];
	float fRec140[2];
	float fRec147[2];
	float fRec146[2];
	float fVec56[32768];
	float fVec57[8192];
	float fRec144[2];
	float fRec151[2];
	float fRec150[2];
	float fVec58[32768];
	float fVec59[8192];
	float fRec148[2];
	float fRec155[2];
	float fRec154[2];
	float fVec60[65536];
	float fVec61[4096];
	float fRec152[2];
	float fRec159[2];
	float fRec158[2];
	float fVec62[65536];
	float fVec63[8192];
	float fRec156[2];
	float fRec163[2];
	float fRec162[2];
	float fVec64[32768];
	float fVec65[8192];
	float fRec160[2];
	float fRec167[2];
	float fRec166[2];
	float fVec66[32768];
	float fVec67[4096];
	float fRec164[2];
	float fRec128[3];
	float fRec129[3];
	float fRec130[3];
	float fRec131[3];
	float fRec132[3];
	float fRec133[3];
	float fRec134[3];
	float fRec135[3];
	FAUSTFLOAT fVslider20;
	FAUSTFLOAT fVslider21;
	float fRec168[2];
	float fVec68[16384];
	FAUSTFLOAT fHslider4;
	float fRec169[2];
	FAUSTFLOAT fVslider22;
	FAUSTFLOAT fVslider23;
	float fRec181[2];
	float fRec180[2];
	float fVec69[65536];
	float fVec70[4096];
	float fRec178[2];
	float fRec185[2];
	float fRec184[2];
	float fVec71[32768];
	FAUSTFLOAT fVslider24;
	float fVec72[4096];
	float fRec182[2];
	float fRec189[2];
	float fRec188[2];
	float fVec73[65536];
	float fVec74[8192];
	float fRec186[2];
	float fRec193[2];
	float fRec192[2];
	float fVec75[32768];
	float fVec76[8192];
	float fRec190[2];
	float fRec197[2];
	float fRec196[2];
	float fVec77[65536];
	float fVec78[8192];
	float fRec194[2];
	float fRec201[2];
	float fRec200[2];
	float fVec79[32768];
	float fVec80[4096];
	float fRec198[2];
	float fRec205[2];
	float fRec204[2];
	float fVec81[32768];
	float fVec82[8192];
	float fRec202[2];
	float fRec209[2];
	float fRec208[2];
	float fVec83[32768];
	float fVec84[8192];
	float fRec206[2];
	float fRec170[3];
	float fRec171[3];
	float fRec172[3];
	float fRec173[3];
	float fRec174[3];
	float fRec175[3];
	float fRec176[3];
	float fRec177[3];
	FAUSTFLOAT fVslider25;
	FAUSTFLOAT fVslider26;
	float fRec210[2];
	float fVec85[16384];
	FAUSTFLOAT fHslider5;
	float fRec211[2];
	FAUSTFLOAT fVslider27;
	FAUSTFLOAT fVslider28;
	float fRec223[2];
	float fRec222[2];
	float fVec86[65536];
	float fVec87[4096];
	float fRec220[2];
	float fRec227[2];
	float fRec226[2];
	float fVec88[65536];
	float fVec89[8192];
	float fRec224[2];
	float fRec231[2];
	float fRec230[2];
	float fVec90[32768];
	float fVec91[8192];
	float fRec228[2];
	float fRec235[2];
	float fRec234[2];
	float fVec92[65536];
	float fVec93[8192];
	float fRec232[2];
	float fRec239[2];
	float fRec238[2];
	float fVec94[32768];
	FAUSTFLOAT fVslider29;
	float fVec95[4096];
	float fRec236[2];
	float fRec243[2];
	float fRec242[2];
	float fVec96[32768];
	float fVec97[8192];
	float fRec240[2];
	float fRec247[2];
	float fRec246[2];
	float fVec98[32768];
	float fVec99[8192];
	float fRec244[2];
	float fRec251[2];
	float fRec250[2];
	float fVec100[32768];
	float fVec101[4096];
	float fRec248[2];
	float fRec212[3];
	float fRec213[3];
	float fRec214[3];
	float fRec215[3];
	float fRec216[3];
	float fRec217[3];
	float fRec218[3];
	float fRec219[3];
	float fRec252[2];
	float fRec253[2];
	float fRec254[2];
	float fRec255[2];
	float fRec256[2];
	float fRec257[2];
	float fRec258[2];
	float fRec259[2];
	float fRec260[2];
	float fRec261[2];
	float fRec262[2];
	float fRec263[2];
	float fRec264[2];
	float fRec265[2];
	float fRec266[2];
	float fRec267[2];
	float fRec268[2];
	float fRec269[2];
	float fRec270[2];
	float fRec271[2];
	float fRec272[2];
	float fRec273[2];
	float fRec274[2];
	float fRec275[2];
	float fRec276[2];
	float fRec277[2];
	float fRec278[2];
	float fRec279[2];
	float fRec280[2];
	float fRec281[2];
	float fRec282[2];
	float fRec283[2];
	float fRec284[2];
	float fRec285[2];
	float fRec286[2];
	float fRec287[2];
	float fRec288[2];
	float fRec289[2];
	float fRec290[2];
	float fRec291[2];
	float fRec292[2];
	float fRec293[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("ceammc.lib/name", "Ceammc PureData misc utils");
		m->declare("ceammc.lib/version", "0.1.2");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("filename", "spat_zita6x8.dsp");
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
		fConst4 = (1.0f / std::tan((628.318542f / fConst0)));
		fConst5 = (1.0f / (fConst4 + 1.0f));
		fConst6 = (1.0f - fConst4);
		fConst7 = std::floor(((0.0191229992f * fConst0) + 0.5f));
		iConst8 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst2 - fConst7))));
		iConst9 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst7 + -1.0f))));
		fConst10 = std::floor(((0.127837002f * fConst0) + 0.5f));
		fConst11 = ((0.0f - (6.90775537f * fConst10)) / fConst0);
		fConst12 = std::floor(((0.0316039994f * fConst0) + 0.5f));
		iConst13 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst10 - fConst12))));
		fConst14 = (0.00100000005f * fConst0);
		iConst15 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst12 + -1.0f))));
		fConst16 = std::floor(((0.256891012f * fConst0) + 0.5f));
		fConst17 = ((0.0f - (6.90775537f * fConst16)) / fConst0);
		fConst18 = std::floor(((0.0273330007f * fConst0) + 0.5f));
		iConst19 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst16 - fConst18))));
		iConst20 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst18 + -1.0f))));
		fConst21 = std::floor(((0.192303002f * fConst0) + 0.5f));
		fConst22 = ((0.0f - (6.90775537f * fConst21)) / fConst0);
		fConst23 = std::floor(((0.0292910002f * fConst0) + 0.5f));
		iConst24 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst21 - fConst23))));
		iConst25 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst23 + -1.0f))));
		fConst26 = std::floor(((0.210389003f * fConst0) + 0.5f));
		fConst27 = ((0.0f - (6.90775537f * fConst26)) / fConst0);
		fConst28 = std::floor(((0.0244210009f * fConst0) + 0.5f));
		iConst29 = int(std::min<float>(32768.0f, std::max<float>(0.0f, (fConst26 - fConst28))));
		iConst30 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst28 + -1.0f))));
		fConst31 = std::floor(((0.125f * fConst0) + 0.5f));
		fConst32 = ((0.0f - (6.90775537f * fConst31)) / fConst0);
		fConst33 = std::floor(((0.0134579996f * fConst0) + 0.5f));
		iConst34 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst31 - fConst33))));
		iConst35 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst33 + -1.0f))));
		fConst36 = std::floor(((0.174713001f * fConst0) + 0.5f));
		fConst37 = ((0.0f - (6.90775537f * fConst36)) / fConst0);
		fConst38 = std::floor(((0.0229039993f * fConst0) + 0.5f));
		iConst39 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst36 - fConst38))));
		iConst40 = int(std::min<float>(4096.0f, std::max<float>(0.0f, (fConst38 + -1.0f))));
		fConst41 = std::floor(((0.153128996f * fConst0) + 0.5f));
		fConst42 = ((0.0f - (6.90775537f * fConst41)) / fConst0);
		fConst43 = std::floor(((0.0203460008f * fConst0) + 0.5f));
		iConst44 = int(std::min<float>(16384.0f, std::max<float>(0.0f, (fConst41 - fConst43))));
		iConst45 = int(std::min<float>(2048.0f, std::max<float>(0.0f, (fConst43 + -1.0f))));
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(1.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.5f);
		fVslider2 = FAUSTFLOAT(2.0f);
		fVslider3 = FAUSTFLOAT(3.0f);
		fVslider4 = FAUSTFLOAT(60.0f);
		fVslider5 = FAUSTFLOAT(1.0f);
		fVslider6 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fVslider7 = FAUSTFLOAT(2.0f);
		fVslider8 = FAUSTFLOAT(3.0f);
		fVslider9 = FAUSTFLOAT(60.0f);
		fVslider10 = FAUSTFLOAT(1.0f);
		fVslider11 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fVslider12 = FAUSTFLOAT(2.0f);
		fVslider13 = FAUSTFLOAT(3.0f);
		fVslider14 = FAUSTFLOAT(60.0f);
		fVslider15 = FAUSTFLOAT(1.0f);
		fVslider16 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(0.5f);
		fVslider17 = FAUSTFLOAT(2.0f);
		fVslider18 = FAUSTFLOAT(3.0f);
		fVslider19 = FAUSTFLOAT(60.0f);
		fVslider20 = FAUSTFLOAT(1.0f);
		fVslider21 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.5f);
		fVslider22 = FAUSTFLOAT(2.0f);
		fVslider23 = FAUSTFLOAT(3.0f);
		fVslider24 = FAUSTFLOAT(60.0f);
		fVslider25 = FAUSTFLOAT(1.0f);
		fVslider26 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.5f);
		fVslider27 = FAUSTFLOAT(2.0f);
		fVslider28 = FAUSTFLOAT(3.0f);
		fVslider29 = FAUSTFLOAT(60.0f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 16384); l1 = (l1 + 1)) {
			fVec0[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec1[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec13[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec12[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 65536); l5 = (l5 + 1)) {
			fVec1[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 4096); l6 = (l6 + 1)) {
			fVec2[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec10[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec17[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec16[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 32768); l10 = (l10 + 1)) {
			fVec3[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 8192); l11 = (l11 + 1)) {
			fVec4[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec14[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec21[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec20[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 65536); l15 = (l15 + 1)) {
			fVec5[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 8192); l16 = (l16 + 1)) {
			fVec6[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec18[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec25[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec24[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 32768); l20 = (l20 + 1)) {
			fVec7[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 8192); l21 = (l21 + 1)) {
			fVec8[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec22[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec29[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec28[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 65536); l25 = (l25 + 1)) {
			fVec9[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 8192); l26 = (l26 + 1)) {
			fVec10[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec26[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec33[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec32[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 32768); l30 = (l30 + 1)) {
			fVec11[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 4096); l31 = (l31 + 1)) {
			fVec12[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec30[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec37[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 2); l34 = (l34 + 1)) {
			fRec36[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 32768); l35 = (l35 + 1)) {
			fVec13[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 8192); l36 = (l36 + 1)) {
			fVec14[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fRec34[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec41[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec40[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 32768); l40 = (l40 + 1)) {
			fVec15[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 4096); l41 = (l41 + 1)) {
			fVec16[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec38[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec2[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec3[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec4[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec5[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec6[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec7[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec8[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec9[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec42[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 16384); l52 = (l52 + 1)) {
			fVec17[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 2); l53 = (l53 + 1)) {
			fRec43[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec55[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec54[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 65536); l56 = (l56 + 1)) {
			fVec18[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 4096); l57 = (l57 + 1)) {
			fVec19[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec52[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec59[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec58[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 65536); l61 = (l61 + 1)) {
			fVec20[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 8192); l62 = (l62 + 1)) {
			fVec21[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec56[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 2); l64 = (l64 + 1)) {
			fRec63[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 2); l65 = (l65 + 1)) {
			fRec62[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 32768); l66 = (l66 + 1)) {
			fVec22[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 8192); l67 = (l67 + 1)) {
			fVec23[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec60[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec67[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec66[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 65536); l71 = (l71 + 1)) {
			fVec24[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 8192); l72 = (l72 + 1)) {
			fVec25[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec64[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec71[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec70[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 32768); l76 = (l76 + 1)) {
			fVec26[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 4096); l77 = (l77 + 1)) {
			fVec27[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec68[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 2); l79 = (l79 + 1)) {
			fRec75[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 2); l80 = (l80 + 1)) {
			fRec74[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 32768); l81 = (l81 + 1)) {
			fVec28[l81] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 8192); l82 = (l82 + 1)) {
			fVec29[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 2); l83 = (l83 + 1)) {
			fRec72[l83] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 2); l84 = (l84 + 1)) {
			fRec79[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 2); l85 = (l85 + 1)) {
			fRec78[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 32768); l86 = (l86 + 1)) {
			fVec30[l86] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l87 = 0; (l87 < 8192); l87 = (l87 + 1)) {
			fVec31[l87] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec76[l88] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec83[l89] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l90 = 0; (l90 < 2); l90 = (l90 + 1)) {
			fRec82[l90] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l91 = 0; (l91 < 32768); l91 = (l91 + 1)) {
			fVec32[l91] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l92 = 0; (l92 < 4096); l92 = (l92 + 1)) {
			fVec33[l92] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec80[l93] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l94 = 0; (l94 < 3); l94 = (l94 + 1)) {
			fRec44[l94] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l95 = 0; (l95 < 3); l95 = (l95 + 1)) {
			fRec45[l95] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l96 = 0; (l96 < 3); l96 = (l96 + 1)) {
			fRec46[l96] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l97 = 0; (l97 < 3); l97 = (l97 + 1)) {
			fRec47[l97] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec48[l98] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec49[l99] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l100 = 0; (l100 < 3); l100 = (l100 + 1)) {
			fRec50[l100] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l101 = 0; (l101 < 3); l101 = (l101 + 1)) {
			fRec51[l101] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			fRec84[l102] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l103 = 0; (l103 < 16384); l103 = (l103 + 1)) {
			fVec34[l103] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec85[l104] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec97[l105] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec96[l106] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l107 = 0; (l107 < 65536); l107 = (l107 + 1)) {
			fVec35[l107] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l108 = 0; (l108 < 8192); l108 = (l108 + 1)) {
			fVec36[l108] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l109 = 0; (l109 < 2); l109 = (l109 + 1)) {
			fRec94[l109] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l110 = 0; (l110 < 2); l110 = (l110 + 1)) {
			fRec101[l110] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l111 = 0; (l111 < 2); l111 = (l111 + 1)) {
			fRec100[l111] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l112 = 0; (l112 < 32768); l112 = (l112 + 1)) {
			fVec37[l112] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l113 = 0; (l113 < 4096); l113 = (l113 + 1)) {
			fVec38[l113] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l114 = 0; (l114 < 2); l114 = (l114 + 1)) {
			fRec98[l114] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l115 = 0; (l115 < 2); l115 = (l115 + 1)) {
			fRec105[l115] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l116 = 0; (l116 < 2); l116 = (l116 + 1)) {
			fRec104[l116] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l117 = 0; (l117 < 32768); l117 = (l117 + 1)) {
			fVec39[l117] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l118 = 0; (l118 < 8192); l118 = (l118 + 1)) {
			fVec40[l118] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l119 = 0; (l119 < 2); l119 = (l119 + 1)) {
			fRec102[l119] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l120 = 0; (l120 < 2); l120 = (l120 + 1)) {
			fRec109[l120] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l121 = 0; (l121 < 2); l121 = (l121 + 1)) {
			fRec108[l121] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l122 = 0; (l122 < 32768); l122 = (l122 + 1)) {
			fVec41[l122] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l123 = 0; (l123 < 8192); l123 = (l123 + 1)) {
			fVec42[l123] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l124 = 0; (l124 < 2); l124 = (l124 + 1)) {
			fRec106[l124] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l125 = 0; (l125 < 2); l125 = (l125 + 1)) {
			fRec113[l125] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l126 = 0; (l126 < 2); l126 = (l126 + 1)) {
			fRec112[l126] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l127 = 0; (l127 < 65536); l127 = (l127 + 1)) {
			fVec43[l127] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l128 = 0; (l128 < 8192); l128 = (l128 + 1)) {
			fVec44[l128] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l129 = 0; (l129 < 2); l129 = (l129 + 1)) {
			fRec110[l129] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l130 = 0; (l130 < 2); l130 = (l130 + 1)) {
			fRec117[l130] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l131 = 0; (l131 < 2); l131 = (l131 + 1)) {
			fRec116[l131] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l132 = 0; (l132 < 32768); l132 = (l132 + 1)) {
			fVec45[l132] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l133 = 0; (l133 < 8192); l133 = (l133 + 1)) {
			fVec46[l133] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l134 = 0; (l134 < 2); l134 = (l134 + 1)) {
			fRec114[l134] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l135 = 0; (l135 < 2); l135 = (l135 + 1)) {
			fRec121[l135] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l136 = 0; (l136 < 2); l136 = (l136 + 1)) {
			fRec120[l136] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l137 = 0; (l137 < 32768); l137 = (l137 + 1)) {
			fVec47[l137] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l138 = 0; (l138 < 4096); l138 = (l138 + 1)) {
			fVec48[l138] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l139 = 0; (l139 < 2); l139 = (l139 + 1)) {
			fRec118[l139] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l140 = 0; (l140 < 2); l140 = (l140 + 1)) {
			fRec125[l140] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l141 = 0; (l141 < 2); l141 = (l141 + 1)) {
			fRec124[l141] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l142 = 0; (l142 < 65536); l142 = (l142 + 1)) {
			fVec49[l142] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l143 = 0; (l143 < 4096); l143 = (l143 + 1)) {
			fVec50[l143] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l144 = 0; (l144 < 2); l144 = (l144 + 1)) {
			fRec122[l144] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l145 = 0; (l145 < 3); l145 = (l145 + 1)) {
			fRec86[l145] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l146 = 0; (l146 < 3); l146 = (l146 + 1)) {
			fRec87[l146] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l147 = 0; (l147 < 3); l147 = (l147 + 1)) {
			fRec88[l147] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l148 = 0; (l148 < 3); l148 = (l148 + 1)) {
			fRec89[l148] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l149 = 0; (l149 < 3); l149 = (l149 + 1)) {
			fRec90[l149] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l150 = 0; (l150 < 3); l150 = (l150 + 1)) {
			fRec91[l150] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l151 = 0; (l151 < 3); l151 = (l151 + 1)) {
			fRec92[l151] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l152 = 0; (l152 < 3); l152 = (l152 + 1)) {
			fRec93[l152] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l153 = 0; (l153 < 2); l153 = (l153 + 1)) {
			fRec126[l153] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l154 = 0; (l154 < 16384); l154 = (l154 + 1)) {
			fVec51[l154] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l155 = 0; (l155 < 2); l155 = (l155 + 1)) {
			fRec127[l155] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l156 = 0; (l156 < 2); l156 = (l156 + 1)) {
			fRec139[l156] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l157 = 0; (l157 < 2); l157 = (l157 + 1)) {
			fRec138[l157] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l158 = 0; (l158 < 65536); l158 = (l158 + 1)) {
			fVec52[l158] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l159 = 0; (l159 < 8192); l159 = (l159 + 1)) {
			fVec53[l159] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l160 = 0; (l160 < 2); l160 = (l160 + 1)) {
			fRec136[l160] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l161 = 0; (l161 < 2); l161 = (l161 + 1)) {
			fRec143[l161] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l162 = 0; (l162 < 2); l162 = (l162 + 1)) {
			fRec142[l162] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l163 = 0; (l163 < 32768); l163 = (l163 + 1)) {
			fVec54[l163] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l164 = 0; (l164 < 4096); l164 = (l164 + 1)) {
			fVec55[l164] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l165 = 0; (l165 < 2); l165 = (l165 + 1)) {
			fRec140[l165] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l166 = 0; (l166 < 2); l166 = (l166 + 1)) {
			fRec147[l166] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l167 = 0; (l167 < 2); l167 = (l167 + 1)) {
			fRec146[l167] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l168 = 0; (l168 < 32768); l168 = (l168 + 1)) {
			fVec56[l168] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l169 = 0; (l169 < 8192); l169 = (l169 + 1)) {
			fVec57[l169] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l170 = 0; (l170 < 2); l170 = (l170 + 1)) {
			fRec144[l170] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l171 = 0; (l171 < 2); l171 = (l171 + 1)) {
			fRec151[l171] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l172 = 0; (l172 < 2); l172 = (l172 + 1)) {
			fRec150[l172] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l173 = 0; (l173 < 32768); l173 = (l173 + 1)) {
			fVec58[l173] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l174 = 0; (l174 < 8192); l174 = (l174 + 1)) {
			fVec59[l174] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l175 = 0; (l175 < 2); l175 = (l175 + 1)) {
			fRec148[l175] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l176 = 0; (l176 < 2); l176 = (l176 + 1)) {
			fRec155[l176] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l177 = 0; (l177 < 2); l177 = (l177 + 1)) {
			fRec154[l177] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l178 = 0; (l178 < 65536); l178 = (l178 + 1)) {
			fVec60[l178] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l179 = 0; (l179 < 4096); l179 = (l179 + 1)) {
			fVec61[l179] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l180 = 0; (l180 < 2); l180 = (l180 + 1)) {
			fRec152[l180] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l181 = 0; (l181 < 2); l181 = (l181 + 1)) {
			fRec159[l181] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l182 = 0; (l182 < 2); l182 = (l182 + 1)) {
			fRec158[l182] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l183 = 0; (l183 < 65536); l183 = (l183 + 1)) {
			fVec62[l183] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l184 = 0; (l184 < 8192); l184 = (l184 + 1)) {
			fVec63[l184] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l185 = 0; (l185 < 2); l185 = (l185 + 1)) {
			fRec156[l185] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l186 = 0; (l186 < 2); l186 = (l186 + 1)) {
			fRec163[l186] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l187 = 0; (l187 < 2); l187 = (l187 + 1)) {
			fRec162[l187] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l188 = 0; (l188 < 32768); l188 = (l188 + 1)) {
			fVec64[l188] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l189 = 0; (l189 < 8192); l189 = (l189 + 1)) {
			fVec65[l189] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l190 = 0; (l190 < 2); l190 = (l190 + 1)) {
			fRec160[l190] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l191 = 0; (l191 < 2); l191 = (l191 + 1)) {
			fRec167[l191] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l192 = 0; (l192 < 2); l192 = (l192 + 1)) {
			fRec166[l192] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l193 = 0; (l193 < 32768); l193 = (l193 + 1)) {
			fVec66[l193] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l194 = 0; (l194 < 4096); l194 = (l194 + 1)) {
			fVec67[l194] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l195 = 0; (l195 < 2); l195 = (l195 + 1)) {
			fRec164[l195] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l196 = 0; (l196 < 3); l196 = (l196 + 1)) {
			fRec128[l196] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l197 = 0; (l197 < 3); l197 = (l197 + 1)) {
			fRec129[l197] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l198 = 0; (l198 < 3); l198 = (l198 + 1)) {
			fRec130[l198] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l199 = 0; (l199 < 3); l199 = (l199 + 1)) {
			fRec131[l199] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l200 = 0; (l200 < 3); l200 = (l200 + 1)) {
			fRec132[l200] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l201 = 0; (l201 < 3); l201 = (l201 + 1)) {
			fRec133[l201] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l202 = 0; (l202 < 3); l202 = (l202 + 1)) {
			fRec134[l202] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l203 = 0; (l203 < 3); l203 = (l203 + 1)) {
			fRec135[l203] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l204 = 0; (l204 < 2); l204 = (l204 + 1)) {
			fRec168[l204] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l205 = 0; (l205 < 16384); l205 = (l205 + 1)) {
			fVec68[l205] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l206 = 0; (l206 < 2); l206 = (l206 + 1)) {
			fRec169[l206] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l207 = 0; (l207 < 2); l207 = (l207 + 1)) {
			fRec181[l207] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l208 = 0; (l208 < 2); l208 = (l208 + 1)) {
			fRec180[l208] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l209 = 0; (l209 < 65536); l209 = (l209 + 1)) {
			fVec69[l209] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l210 = 0; (l210 < 4096); l210 = (l210 + 1)) {
			fVec70[l210] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l211 = 0; (l211 < 2); l211 = (l211 + 1)) {
			fRec178[l211] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l212 = 0; (l212 < 2); l212 = (l212 + 1)) {
			fRec185[l212] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l213 = 0; (l213 < 2); l213 = (l213 + 1)) {
			fRec184[l213] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l214 = 0; (l214 < 32768); l214 = (l214 + 1)) {
			fVec71[l214] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l215 = 0; (l215 < 4096); l215 = (l215 + 1)) {
			fVec72[l215] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l216 = 0; (l216 < 2); l216 = (l216 + 1)) {
			fRec182[l216] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l217 = 0; (l217 < 2); l217 = (l217 + 1)) {
			fRec189[l217] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l218 = 0; (l218 < 2); l218 = (l218 + 1)) {
			fRec188[l218] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l219 = 0; (l219 < 65536); l219 = (l219 + 1)) {
			fVec73[l219] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l220 = 0; (l220 < 8192); l220 = (l220 + 1)) {
			fVec74[l220] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l221 = 0; (l221 < 2); l221 = (l221 + 1)) {
			fRec186[l221] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l222 = 0; (l222 < 2); l222 = (l222 + 1)) {
			fRec193[l222] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l223 = 0; (l223 < 2); l223 = (l223 + 1)) {
			fRec192[l223] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l224 = 0; (l224 < 32768); l224 = (l224 + 1)) {
			fVec75[l224] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l225 = 0; (l225 < 8192); l225 = (l225 + 1)) {
			fVec76[l225] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l226 = 0; (l226 < 2); l226 = (l226 + 1)) {
			fRec190[l226] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l227 = 0; (l227 < 2); l227 = (l227 + 1)) {
			fRec197[l227] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l228 = 0; (l228 < 2); l228 = (l228 + 1)) {
			fRec196[l228] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l229 = 0; (l229 < 65536); l229 = (l229 + 1)) {
			fVec77[l229] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l230 = 0; (l230 < 8192); l230 = (l230 + 1)) {
			fVec78[l230] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l231 = 0; (l231 < 2); l231 = (l231 + 1)) {
			fRec194[l231] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l232 = 0; (l232 < 2); l232 = (l232 + 1)) {
			fRec201[l232] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l233 = 0; (l233 < 2); l233 = (l233 + 1)) {
			fRec200[l233] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l234 = 0; (l234 < 32768); l234 = (l234 + 1)) {
			fVec79[l234] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l235 = 0; (l235 < 4096); l235 = (l235 + 1)) {
			fVec80[l235] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l236 = 0; (l236 < 2); l236 = (l236 + 1)) {
			fRec198[l236] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l237 = 0; (l237 < 2); l237 = (l237 + 1)) {
			fRec205[l237] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l238 = 0; (l238 < 2); l238 = (l238 + 1)) {
			fRec204[l238] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l239 = 0; (l239 < 32768); l239 = (l239 + 1)) {
			fVec81[l239] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l240 = 0; (l240 < 8192); l240 = (l240 + 1)) {
			fVec82[l240] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l241 = 0; (l241 < 2); l241 = (l241 + 1)) {
			fRec202[l241] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l242 = 0; (l242 < 2); l242 = (l242 + 1)) {
			fRec209[l242] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l243 = 0; (l243 < 2); l243 = (l243 + 1)) {
			fRec208[l243] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l244 = 0; (l244 < 32768); l244 = (l244 + 1)) {
			fVec83[l244] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l245 = 0; (l245 < 8192); l245 = (l245 + 1)) {
			fVec84[l245] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l246 = 0; (l246 < 2); l246 = (l246 + 1)) {
			fRec206[l246] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l247 = 0; (l247 < 3); l247 = (l247 + 1)) {
			fRec170[l247] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l248 = 0; (l248 < 3); l248 = (l248 + 1)) {
			fRec171[l248] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l249 = 0; (l249 < 3); l249 = (l249 + 1)) {
			fRec172[l249] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l250 = 0; (l250 < 3); l250 = (l250 + 1)) {
			fRec173[l250] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l251 = 0; (l251 < 3); l251 = (l251 + 1)) {
			fRec174[l251] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l252 = 0; (l252 < 3); l252 = (l252 + 1)) {
			fRec175[l252] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l253 = 0; (l253 < 3); l253 = (l253 + 1)) {
			fRec176[l253] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l254 = 0; (l254 < 3); l254 = (l254 + 1)) {
			fRec177[l254] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l255 = 0; (l255 < 2); l255 = (l255 + 1)) {
			fRec210[l255] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l256 = 0; (l256 < 16384); l256 = (l256 + 1)) {
			fVec85[l256] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l257 = 0; (l257 < 2); l257 = (l257 + 1)) {
			fRec211[l257] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l258 = 0; (l258 < 2); l258 = (l258 + 1)) {
			fRec223[l258] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l259 = 0; (l259 < 2); l259 = (l259 + 1)) {
			fRec222[l259] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l260 = 0; (l260 < 65536); l260 = (l260 + 1)) {
			fVec86[l260] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l261 = 0; (l261 < 4096); l261 = (l261 + 1)) {
			fVec87[l261] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l262 = 0; (l262 < 2); l262 = (l262 + 1)) {
			fRec220[l262] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l263 = 0; (l263 < 2); l263 = (l263 + 1)) {
			fRec227[l263] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l264 = 0; (l264 < 2); l264 = (l264 + 1)) {
			fRec226[l264] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l265 = 0; (l265 < 65536); l265 = (l265 + 1)) {
			fVec88[l265] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l266 = 0; (l266 < 8192); l266 = (l266 + 1)) {
			fVec89[l266] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l267 = 0; (l267 < 2); l267 = (l267 + 1)) {
			fRec224[l267] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l268 = 0; (l268 < 2); l268 = (l268 + 1)) {
			fRec231[l268] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l269 = 0; (l269 < 2); l269 = (l269 + 1)) {
			fRec230[l269] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l270 = 0; (l270 < 32768); l270 = (l270 + 1)) {
			fVec90[l270] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l271 = 0; (l271 < 8192); l271 = (l271 + 1)) {
			fVec91[l271] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l272 = 0; (l272 < 2); l272 = (l272 + 1)) {
			fRec228[l272] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l273 = 0; (l273 < 2); l273 = (l273 + 1)) {
			fRec235[l273] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l274 = 0; (l274 < 2); l274 = (l274 + 1)) {
			fRec234[l274] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l275 = 0; (l275 < 65536); l275 = (l275 + 1)) {
			fVec92[l275] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l276 = 0; (l276 < 8192); l276 = (l276 + 1)) {
			fVec93[l276] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l277 = 0; (l277 < 2); l277 = (l277 + 1)) {
			fRec232[l277] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l278 = 0; (l278 < 2); l278 = (l278 + 1)) {
			fRec239[l278] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l279 = 0; (l279 < 2); l279 = (l279 + 1)) {
			fRec238[l279] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l280 = 0; (l280 < 32768); l280 = (l280 + 1)) {
			fVec94[l280] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l281 = 0; (l281 < 4096); l281 = (l281 + 1)) {
			fVec95[l281] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l282 = 0; (l282 < 2); l282 = (l282 + 1)) {
			fRec236[l282] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l283 = 0; (l283 < 2); l283 = (l283 + 1)) {
			fRec243[l283] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l284 = 0; (l284 < 2); l284 = (l284 + 1)) {
			fRec242[l284] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l285 = 0; (l285 < 32768); l285 = (l285 + 1)) {
			fVec96[l285] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l286 = 0; (l286 < 8192); l286 = (l286 + 1)) {
			fVec97[l286] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l287 = 0; (l287 < 2); l287 = (l287 + 1)) {
			fRec240[l287] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l288 = 0; (l288 < 2); l288 = (l288 + 1)) {
			fRec247[l288] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l289 = 0; (l289 < 2); l289 = (l289 + 1)) {
			fRec246[l289] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l290 = 0; (l290 < 32768); l290 = (l290 + 1)) {
			fVec98[l290] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l291 = 0; (l291 < 8192); l291 = (l291 + 1)) {
			fVec99[l291] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l292 = 0; (l292 < 2); l292 = (l292 + 1)) {
			fRec244[l292] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l293 = 0; (l293 < 2); l293 = (l293 + 1)) {
			fRec251[l293] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l294 = 0; (l294 < 2); l294 = (l294 + 1)) {
			fRec250[l294] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l295 = 0; (l295 < 32768); l295 = (l295 + 1)) {
			fVec100[l295] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l296 = 0; (l296 < 4096); l296 = (l296 + 1)) {
			fVec101[l296] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l297 = 0; (l297 < 2); l297 = (l297 + 1)) {
			fRec248[l297] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l298 = 0; (l298 < 3); l298 = (l298 + 1)) {
			fRec212[l298] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l299 = 0; (l299 < 3); l299 = (l299 + 1)) {
			fRec213[l299] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l300 = 0; (l300 < 3); l300 = (l300 + 1)) {
			fRec214[l300] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l301 = 0; (l301 < 3); l301 = (l301 + 1)) {
			fRec215[l301] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l302 = 0; (l302 < 3); l302 = (l302 + 1)) {
			fRec216[l302] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l303 = 0; (l303 < 3); l303 = (l303 + 1)) {
			fRec217[l303] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l304 = 0; (l304 < 3); l304 = (l304 + 1)) {
			fRec218[l304] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l305 = 0; (l305 < 3); l305 = (l305 + 1)) {
			fRec219[l305] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l306 = 0; (l306 < 2); l306 = (l306 + 1)) {
			fRec252[l306] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l307 = 0; (l307 < 2); l307 = (l307 + 1)) {
			fRec253[l307] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l308 = 0; (l308 < 2); l308 = (l308 + 1)) {
			fRec254[l308] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l309 = 0; (l309 < 2); l309 = (l309 + 1)) {
			fRec255[l309] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l310 = 0; (l310 < 2); l310 = (l310 + 1)) {
			fRec256[l310] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l311 = 0; (l311 < 2); l311 = (l311 + 1)) {
			fRec257[l311] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l312 = 0; (l312 < 2); l312 = (l312 + 1)) {
			fRec258[l312] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l313 = 0; (l313 < 2); l313 = (l313 + 1)) {
			fRec259[l313] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l314 = 0; (l314 < 2); l314 = (l314 + 1)) {
			fRec260[l314] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l315 = 0; (l315 < 2); l315 = (l315 + 1)) {
			fRec261[l315] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l316 = 0; (l316 < 2); l316 = (l316 + 1)) {
			fRec262[l316] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l317 = 0; (l317 < 2); l317 = (l317 + 1)) {
			fRec263[l317] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l318 = 0; (l318 < 2); l318 = (l318 + 1)) {
			fRec264[l318] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l319 = 0; (l319 < 2); l319 = (l319 + 1)) {
			fRec265[l319] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l320 = 0; (l320 < 2); l320 = (l320 + 1)) {
			fRec266[l320] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l321 = 0; (l321 < 2); l321 = (l321 + 1)) {
			fRec267[l321] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l322 = 0; (l322 < 2); l322 = (l322 + 1)) {
			fRec268[l322] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l323 = 0; (l323 < 2); l323 = (l323 + 1)) {
			fRec269[l323] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l324 = 0; (l324 < 2); l324 = (l324 + 1)) {
			fRec270[l324] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l325 = 0; (l325 < 2); l325 = (l325 + 1)) {
			fRec271[l325] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l326 = 0; (l326 < 2); l326 = (l326 + 1)) {
			fRec272[l326] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l327 = 0; (l327 < 2); l327 = (l327 + 1)) {
			fRec273[l327] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l328 = 0; (l328 < 2); l328 = (l328 + 1)) {
			fRec274[l328] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l329 = 0; (l329 < 2); l329 = (l329 + 1)) {
			fRec275[l329] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l330 = 0; (l330 < 2); l330 = (l330 + 1)) {
			fRec276[l330] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l331 = 0; (l331 < 2); l331 = (l331 + 1)) {
			fRec277[l331] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l332 = 0; (l332 < 2); l332 = (l332 + 1)) {
			fRec278[l332] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l333 = 0; (l333 < 2); l333 = (l333 + 1)) {
			fRec279[l333] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l334 = 0; (l334 < 2); l334 = (l334 + 1)) {
			fRec280[l334] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l335 = 0; (l335 < 2); l335 = (l335 + 1)) {
			fRec281[l335] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l336 = 0; (l336 < 2); l336 = (l336 + 1)) {
			fRec282[l336] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l337 = 0; (l337 < 2); l337 = (l337 + 1)) {
			fRec283[l337] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l338 = 0; (l338 < 2); l338 = (l338 + 1)) {
			fRec284[l338] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l339 = 0; (l339 < 2); l339 = (l339 + 1)) {
			fRec285[l339] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l340 = 0; (l340 < 2); l340 = (l340 + 1)) {
			fRec286[l340] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l341 = 0; (l341 < 2); l341 = (l341 + 1)) {
			fRec287[l341] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l342 = 0; (l342 < 2); l342 = (l342 + 1)) {
			fRec288[l342] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l343 = 0; (l343 < 2); l343 = (l343 + 1)) {
			fRec289[l343] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l344 = 0; (l344 < 2); l344 = (l344 + 1)) {
			fRec290[l344] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l345 = 0; (l345 < 2); l345 = (l345 + 1)) {
			fRec291[l345] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l346 = 0; (l346 < 2); l346 = (l346 + 1)) {
			fRec292[l346] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l347 = 0; (l347 < 2); l347 = (l347 + 1)) {
			fRec293[l347] = 0.0f;
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
		ui_interface->addVerticalSlider("angle0", &fVslider11, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle1", &fVslider16, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle2", &fVslider6, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle3", &fVslider21, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle4", &fVslider1, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("angle5", &fVslider26, 0.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->declare(&fVslider13, "scale", "log");
		ui_interface->declare(&fVslider13, "style", "knob");
		ui_interface->declare(&fVslider13, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low0", &fVslider13, 3.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider18, "scale", "log");
		ui_interface->declare(&fVslider18, "style", "knob");
		ui_interface->declare(&fVslider18, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low1", &fVslider18, 3.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider8, "scale", "log");
		ui_interface->declare(&fVslider8, "style", "knob");
		ui_interface->declare(&fVslider8, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low2", &fVslider8, 3.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider23, "scale", "log");
		ui_interface->declare(&fVslider23, "style", "knob");
		ui_interface->declare(&fVslider23, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low3", &fVslider23, 3.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider3, "scale", "log");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low4", &fVslider3, 3.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider28, "scale", "log");
		ui_interface->declare(&fVslider28, "style", "knob");
		ui_interface->declare(&fVslider28, "unit", "sec");
		ui_interface->addVerticalSlider("decay_low5", &fVslider28, 3.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider12, "scale", "log");
		ui_interface->declare(&fVslider12, "style", "knob");
		ui_interface->declare(&fVslider12, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid0", &fVslider12, 2.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider17, "scale", "log");
		ui_interface->declare(&fVslider17, "style", "knob");
		ui_interface->declare(&fVslider17, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid1", &fVslider17, 2.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider7, "scale", "log");
		ui_interface->declare(&fVslider7, "style", "knob");
		ui_interface->declare(&fVslider7, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid2", &fVslider7, 2.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider22, "scale", "log");
		ui_interface->declare(&fVslider22, "style", "knob");
		ui_interface->declare(&fVslider22, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid3", &fVslider22, 2.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider2, "scale", "log");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid4", &fVslider2, 2.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider27, "scale", "log");
		ui_interface->declare(&fVslider27, "style", "knob");
		ui_interface->declare(&fVslider27, "unit", "sec");
		ui_interface->addVerticalSlider("decay_mid5", &fVslider27, 2.0f, 1.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider14, "style", "knob");
		ui_interface->declare(&fVslider14, "unit", "ms");
		ui_interface->addVerticalSlider("delay0", &fVslider14, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider19, "style", "knob");
		ui_interface->declare(&fVslider19, "unit", "ms");
		ui_interface->addVerticalSlider("delay1", &fVslider19, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider9, "style", "knob");
		ui_interface->declare(&fVslider9, "unit", "ms");
		ui_interface->addVerticalSlider("delay2", &fVslider9, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider24, "style", "knob");
		ui_interface->declare(&fVslider24, "unit", "ms");
		ui_interface->addVerticalSlider("delay3", &fVslider24, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "unit", "ms");
		ui_interface->addVerticalSlider("delay4", &fVslider4, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fVslider29, "style", "knob");
		ui_interface->declare(&fVslider29, "unit", "ms");
		ui_interface->addVerticalSlider("delay5", &fVslider29, 60.0f, 20.0f, 100.0f, 1.0f);
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("drywet0", &fHslider2, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("drywet1", &fHslider3, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->addHorizontalSlider("drywet2", &fHslider1, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->addHorizontalSlider("drywet3", &fHslider4, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider0, "style", "knob");
		ui_interface->addHorizontalSlider("drywet4", &fHslider0, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->addHorizontalSlider("drywet5", &fHslider5, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addVerticalSlider("radius0", &fVslider10, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius1", &fVslider15, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius2", &fVslider5, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius3", &fVslider20, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius4", &fVslider0, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
		ui_interface->addVerticalSlider("radius5", &fVslider25, 1.0f, 0.0f, 1.0f, 9.99999975e-05f);
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
		float fSlow3 = (4.99999987e-05f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow4 = (0.00100000005f * float(fHslider0));
		float fSlow5 = float(fVslider2);
		float fSlow6 = std::exp((fConst3 / fSlow5));
		float fSlow7 = spat_zita6x8_faustpower2_f(fSlow6);
		float fSlow8 = (1.0f - (fConst1 * fSlow7));
		float fSlow9 = (1.0f - fSlow7);
		float fSlow10 = (fSlow8 / fSlow9);
		float fSlow11 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow8) / spat_zita6x8_faustpower2_f(fSlow9)) + -1.0f)));
		float fSlow12 = (fSlow10 - fSlow11);
		float fSlow13 = (fSlow6 * (fSlow11 + (1.0f - fSlow10)));
		float fSlow14 = float(fVslider3);
		float fSlow15 = ((std::exp((fConst3 / fSlow14)) / fSlow6) + -1.0f);
		float fSlow16 = std::exp((fConst11 / fSlow5));
		float fSlow17 = spat_zita6x8_faustpower2_f(fSlow16);
		float fSlow18 = (1.0f - (fConst1 * fSlow17));
		float fSlow19 = (1.0f - fSlow17);
		float fSlow20 = (fSlow18 / fSlow19);
		float fSlow21 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow18) / spat_zita6x8_faustpower2_f(fSlow19)) + -1.0f)));
		float fSlow22 = (fSlow20 - fSlow21);
		float fSlow23 = (fSlow16 * (fSlow21 + (1.0f - fSlow20)));
		float fSlow24 = ((std::exp((fConst11 / fSlow14)) / fSlow16) + -1.0f);
		int iSlow25 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst14 * float(fVslider4)))));
		float fSlow26 = std::exp((fConst17 / fSlow5));
		float fSlow27 = spat_zita6x8_faustpower2_f(fSlow26);
		float fSlow28 = (1.0f - (fConst1 * fSlow27));
		float fSlow29 = (1.0f - fSlow27);
		float fSlow30 = (fSlow28 / fSlow29);
		float fSlow31 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow28) / spat_zita6x8_faustpower2_f(fSlow29)) + -1.0f)));
		float fSlow32 = (fSlow30 - fSlow31);
		float fSlow33 = (fSlow26 * (fSlow31 + (1.0f - fSlow30)));
		float fSlow34 = ((std::exp((fConst17 / fSlow14)) / fSlow26) + -1.0f);
		float fSlow35 = std::exp((fConst22 / fSlow5));
		float fSlow36 = spat_zita6x8_faustpower2_f(fSlow35);
		float fSlow37 = (1.0f - (fConst1 * fSlow36));
		float fSlow38 = (1.0f - fSlow36);
		float fSlow39 = (fSlow37 / fSlow38);
		float fSlow40 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow37) / spat_zita6x8_faustpower2_f(fSlow38)) + -1.0f)));
		float fSlow41 = (fSlow39 - fSlow40);
		float fSlow42 = (fSlow35 * (fSlow40 + (1.0f - fSlow39)));
		float fSlow43 = ((std::exp((fConst22 / fSlow14)) / fSlow35) + -1.0f);
		float fSlow44 = std::exp((fConst27 / fSlow5));
		float fSlow45 = spat_zita6x8_faustpower2_f(fSlow44);
		float fSlow46 = (1.0f - (fConst1 * fSlow45));
		float fSlow47 = (1.0f - fSlow45);
		float fSlow48 = (fSlow46 / fSlow47);
		float fSlow49 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow46) / spat_zita6x8_faustpower2_f(fSlow47)) + -1.0f)));
		float fSlow50 = (fSlow48 - fSlow49);
		float fSlow51 = (fSlow44 * (fSlow49 + (1.0f - fSlow48)));
		float fSlow52 = ((std::exp((fConst27 / fSlow14)) / fSlow44) + -1.0f);
		float fSlow53 = std::exp((fConst32 / fSlow5));
		float fSlow54 = spat_zita6x8_faustpower2_f(fSlow53);
		float fSlow55 = (1.0f - (fConst1 * fSlow54));
		float fSlow56 = (1.0f - fSlow54);
		float fSlow57 = (fSlow55 / fSlow56);
		float fSlow58 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow55) / spat_zita6x8_faustpower2_f(fSlow56)) + -1.0f)));
		float fSlow59 = (fSlow57 - fSlow58);
		float fSlow60 = (fSlow53 * (fSlow58 + (1.0f - fSlow57)));
		float fSlow61 = ((std::exp((fConst32 / fSlow14)) / fSlow53) + -1.0f);
		float fSlow62 = std::exp((fConst37 / fSlow5));
		float fSlow63 = spat_zita6x8_faustpower2_f(fSlow62);
		float fSlow64 = (1.0f - (fConst1 * fSlow63));
		float fSlow65 = (1.0f - fSlow63);
		float fSlow66 = (fSlow64 / fSlow65);
		float fSlow67 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow64) / spat_zita6x8_faustpower2_f(fSlow65)) + -1.0f)));
		float fSlow68 = (fSlow66 - fSlow67);
		float fSlow69 = (fSlow62 * (fSlow67 + (1.0f - fSlow66)));
		float fSlow70 = ((std::exp((fConst37 / fSlow14)) / fSlow62) + -1.0f);
		float fSlow71 = std::exp((fConst42 / fSlow5));
		float fSlow72 = spat_zita6x8_faustpower2_f(fSlow71);
		float fSlow73 = (1.0f - (fConst1 * fSlow72));
		float fSlow74 = (1.0f - fSlow72);
		float fSlow75 = (fSlow73 / fSlow74);
		float fSlow76 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow73) / spat_zita6x8_faustpower2_f(fSlow74)) + -1.0f)));
		float fSlow77 = (fSlow75 - fSlow76);
		float fSlow78 = (fSlow71 * (fSlow76 + (1.0f - fSlow75)));
		float fSlow79 = ((std::exp((fConst42 / fSlow14)) / fSlow71) + -1.0f);
		float fSlow80 = float(fVslider5);
		float fSlow81 = (fSlow80 + 1.0f);
		float fSlow82 = float(fVslider6);
		float fSlow83 = (4.99999987e-05f * (fSlow81 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow80 * std::fabs((std::fmod((fSlow82 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow84 = (0.00100000005f * float(fHslider1));
		float fSlow85 = float(fVslider7);
		float fSlow86 = std::exp((fConst3 / fSlow85));
		float fSlow87 = spat_zita6x8_faustpower2_f(fSlow86);
		float fSlow88 = (1.0f - (fConst1 * fSlow87));
		float fSlow89 = (1.0f - fSlow87);
		float fSlow90 = (fSlow88 / fSlow89);
		float fSlow91 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow88) / spat_zita6x8_faustpower2_f(fSlow89)) + -1.0f)));
		float fSlow92 = (fSlow90 - fSlow91);
		float fSlow93 = (fSlow86 * (fSlow91 + (1.0f - fSlow90)));
		float fSlow94 = float(fVslider8);
		float fSlow95 = ((std::exp((fConst3 / fSlow94)) / fSlow86) + -1.0f);
		float fSlow96 = std::exp((fConst17 / fSlow85));
		float fSlow97 = spat_zita6x8_faustpower2_f(fSlow96);
		float fSlow98 = (1.0f - (fConst1 * fSlow97));
		float fSlow99 = (1.0f - fSlow97);
		float fSlow100 = (fSlow98 / fSlow99);
		float fSlow101 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow98) / spat_zita6x8_faustpower2_f(fSlow99)) + -1.0f)));
		float fSlow102 = (fSlow100 - fSlow101);
		float fSlow103 = (fSlow96 * (fSlow101 + (1.0f - fSlow100)));
		float fSlow104 = ((std::exp((fConst17 / fSlow94)) / fSlow96) + -1.0f);
		float fSlow105 = std::exp((fConst22 / fSlow85));
		float fSlow106 = spat_zita6x8_faustpower2_f(fSlow105);
		float fSlow107 = (1.0f - (fConst1 * fSlow106));
		float fSlow108 = (1.0f - fSlow106);
		float fSlow109 = (fSlow107 / fSlow108);
		float fSlow110 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow107) / spat_zita6x8_faustpower2_f(fSlow108)) + -1.0f)));
		float fSlow111 = (fSlow109 - fSlow110);
		float fSlow112 = (fSlow105 * (fSlow110 + (1.0f - fSlow109)));
		float fSlow113 = ((std::exp((fConst22 / fSlow94)) / fSlow105) + -1.0f);
		float fSlow114 = std::exp((fConst27 / fSlow85));
		float fSlow115 = spat_zita6x8_faustpower2_f(fSlow114);
		float fSlow116 = (1.0f - (fConst1 * fSlow115));
		float fSlow117 = (1.0f - fSlow115);
		float fSlow118 = (fSlow116 / fSlow117);
		float fSlow119 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow116) / spat_zita6x8_faustpower2_f(fSlow117)) + -1.0f)));
		float fSlow120 = (fSlow118 - fSlow119);
		float fSlow121 = (fSlow114 * (fSlow119 + (1.0f - fSlow118)));
		float fSlow122 = ((std::exp((fConst27 / fSlow94)) / fSlow114) + -1.0f);
		float fSlow123 = std::exp((fConst32 / fSlow85));
		float fSlow124 = spat_zita6x8_faustpower2_f(fSlow123);
		float fSlow125 = (1.0f - (fConst1 * fSlow124));
		float fSlow126 = (1.0f - fSlow124);
		float fSlow127 = (fSlow125 / fSlow126);
		float fSlow128 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow125) / spat_zita6x8_faustpower2_f(fSlow126)) + -1.0f)));
		float fSlow129 = (fSlow127 - fSlow128);
		float fSlow130 = (fSlow123 * (fSlow128 + (1.0f - fSlow127)));
		float fSlow131 = ((std::exp((fConst32 / fSlow94)) / fSlow123) + -1.0f);
		int iSlow132 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst14 * float(fVslider9)))));
		float fSlow133 = std::exp((fConst11 / fSlow85));
		float fSlow134 = spat_zita6x8_faustpower2_f(fSlow133);
		float fSlow135 = (1.0f - (fConst1 * fSlow134));
		float fSlow136 = (1.0f - fSlow134);
		float fSlow137 = (fSlow135 / fSlow136);
		float fSlow138 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow135) / spat_zita6x8_faustpower2_f(fSlow136)) + -1.0f)));
		float fSlow139 = (fSlow137 - fSlow138);
		float fSlow140 = (fSlow133 * (fSlow138 + (1.0f - fSlow137)));
		float fSlow141 = ((std::exp((fConst11 / fSlow94)) / fSlow133) + -1.0f);
		float fSlow142 = std::exp((fConst37 / fSlow85));
		float fSlow143 = spat_zita6x8_faustpower2_f(fSlow142);
		float fSlow144 = (1.0f - (fConst1 * fSlow143));
		float fSlow145 = (1.0f - fSlow143);
		float fSlow146 = (fSlow144 / fSlow145);
		float fSlow147 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow144) / spat_zita6x8_faustpower2_f(fSlow145)) + -1.0f)));
		float fSlow148 = (fSlow146 - fSlow147);
		float fSlow149 = (fSlow142 * (fSlow147 + (1.0f - fSlow146)));
		float fSlow150 = ((std::exp((fConst37 / fSlow94)) / fSlow142) + -1.0f);
		float fSlow151 = std::exp((fConst42 / fSlow85));
		float fSlow152 = spat_zita6x8_faustpower2_f(fSlow151);
		float fSlow153 = (1.0f - (fConst1 * fSlow152));
		float fSlow154 = (1.0f - fSlow152);
		float fSlow155 = (fSlow153 / fSlow154);
		float fSlow156 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow153) / spat_zita6x8_faustpower2_f(fSlow154)) + -1.0f)));
		float fSlow157 = (fSlow155 - fSlow156);
		float fSlow158 = (fSlow151 * (fSlow156 + (1.0f - fSlow155)));
		float fSlow159 = ((std::exp((fConst42 / fSlow94)) / fSlow151) + -1.0f);
		float fSlow160 = float(fVslider10);
		float fSlow161 = (fSlow160 + 1.0f);
		float fSlow162 = float(fVslider11);
		float fSlow163 = (4.99999987e-05f * (fSlow161 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow160 * std::fabs((std::fmod((fSlow162 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow164 = (0.00100000005f * float(fHslider2));
		float fSlow165 = float(fVslider12);
		float fSlow166 = std::exp((fConst17 / fSlow165));
		float fSlow167 = spat_zita6x8_faustpower2_f(fSlow166);
		float fSlow168 = (1.0f - (fConst1 * fSlow167));
		float fSlow169 = (1.0f - fSlow167);
		float fSlow170 = (fSlow168 / fSlow169);
		float fSlow171 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow168) / spat_zita6x8_faustpower2_f(fSlow169)) + -1.0f)));
		float fSlow172 = (fSlow170 - fSlow171);
		float fSlow173 = (fSlow166 * (fSlow171 + (1.0f - fSlow170)));
		float fSlow174 = float(fVslider13);
		float fSlow175 = ((std::exp((fConst17 / fSlow174)) / fSlow166) + -1.0f);
		float fSlow176 = std::exp((fConst32 / fSlow165));
		float fSlow177 = spat_zita6x8_faustpower2_f(fSlow176);
		float fSlow178 = (1.0f - (fConst1 * fSlow177));
		float fSlow179 = (1.0f - fSlow177);
		float fSlow180 = (fSlow178 / fSlow179);
		float fSlow181 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow178) / spat_zita6x8_faustpower2_f(fSlow179)) + -1.0f)));
		float fSlow182 = (fSlow180 - fSlow181);
		float fSlow183 = (fSlow176 * (fSlow181 + (1.0f - fSlow180)));
		float fSlow184 = ((std::exp((fConst32 / fSlow174)) / fSlow176) + -1.0f);
		int iSlow185 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst14 * float(fVslider14)))));
		float fSlow186 = std::exp((fConst11 / fSlow165));
		float fSlow187 = spat_zita6x8_faustpower2_f(fSlow186);
		float fSlow188 = (1.0f - (fConst1 * fSlow187));
		float fSlow189 = (1.0f - fSlow187);
		float fSlow190 = (fSlow188 / fSlow189);
		float fSlow191 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow188) / spat_zita6x8_faustpower2_f(fSlow189)) + -1.0f)));
		float fSlow192 = (fSlow190 - fSlow191);
		float fSlow193 = (fSlow186 * (fSlow191 + (1.0f - fSlow190)));
		float fSlow194 = ((std::exp((fConst11 / fSlow174)) / fSlow186) + -1.0f);
		float fSlow195 = std::exp((fConst22 / fSlow165));
		float fSlow196 = spat_zita6x8_faustpower2_f(fSlow195);
		float fSlow197 = (1.0f - (fConst1 * fSlow196));
		float fSlow198 = (1.0f - fSlow196);
		float fSlow199 = (fSlow197 / fSlow198);
		float fSlow200 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow197) / spat_zita6x8_faustpower2_f(fSlow198)) + -1.0f)));
		float fSlow201 = (fSlow199 - fSlow200);
		float fSlow202 = (fSlow195 * (fSlow200 + (1.0f - fSlow199)));
		float fSlow203 = ((std::exp((fConst22 / fSlow174)) / fSlow195) + -1.0f);
		float fSlow204 = std::exp((fConst27 / fSlow165));
		float fSlow205 = spat_zita6x8_faustpower2_f(fSlow204);
		float fSlow206 = (1.0f - (fConst1 * fSlow205));
		float fSlow207 = (1.0f - fSlow205);
		float fSlow208 = (fSlow206 / fSlow207);
		float fSlow209 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow206) / spat_zita6x8_faustpower2_f(fSlow207)) + -1.0f)));
		float fSlow210 = (fSlow208 - fSlow209);
		float fSlow211 = (fSlow204 * (fSlow209 + (1.0f - fSlow208)));
		float fSlow212 = ((std::exp((fConst27 / fSlow174)) / fSlow204) + -1.0f);
		float fSlow213 = std::exp((fConst37 / fSlow165));
		float fSlow214 = spat_zita6x8_faustpower2_f(fSlow213);
		float fSlow215 = (1.0f - (fConst1 * fSlow214));
		float fSlow216 = (1.0f - fSlow214);
		float fSlow217 = (fSlow215 / fSlow216);
		float fSlow218 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow215) / spat_zita6x8_faustpower2_f(fSlow216)) + -1.0f)));
		float fSlow219 = (fSlow217 - fSlow218);
		float fSlow220 = (fSlow213 * (fSlow218 + (1.0f - fSlow217)));
		float fSlow221 = ((std::exp((fConst37 / fSlow174)) / fSlow213) + -1.0f);
		float fSlow222 = std::exp((fConst42 / fSlow165));
		float fSlow223 = spat_zita6x8_faustpower2_f(fSlow222);
		float fSlow224 = (1.0f - (fConst1 * fSlow223));
		float fSlow225 = (1.0f - fSlow223);
		float fSlow226 = (fSlow224 / fSlow225);
		float fSlow227 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow224) / spat_zita6x8_faustpower2_f(fSlow225)) + -1.0f)));
		float fSlow228 = (fSlow226 - fSlow227);
		float fSlow229 = (fSlow222 * (fSlow227 + (1.0f - fSlow226)));
		float fSlow230 = ((std::exp((fConst42 / fSlow174)) / fSlow222) + -1.0f);
		float fSlow231 = std::exp((fConst3 / fSlow165));
		float fSlow232 = spat_zita6x8_faustpower2_f(fSlow231);
		float fSlow233 = (1.0f - (fConst1 * fSlow232));
		float fSlow234 = (1.0f - fSlow232);
		float fSlow235 = (fSlow233 / fSlow234);
		float fSlow236 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow233) / spat_zita6x8_faustpower2_f(fSlow234)) + -1.0f)));
		float fSlow237 = (fSlow235 - fSlow236);
		float fSlow238 = (fSlow231 * (fSlow236 + (1.0f - fSlow235)));
		float fSlow239 = ((std::exp((fConst3 / fSlow174)) / fSlow231) + -1.0f);
		float fSlow240 = float(fVslider15);
		float fSlow241 = (fSlow240 + 1.0f);
		float fSlow242 = float(fVslider16);
		float fSlow243 = (4.99999987e-05f * (fSlow241 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow240 * std::fabs((std::fmod((fSlow242 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow244 = (0.00100000005f * float(fHslider3));
		float fSlow245 = float(fVslider17);
		float fSlow246 = std::exp((fConst27 / fSlow245));
		float fSlow247 = spat_zita6x8_faustpower2_f(fSlow246);
		float fSlow248 = (1.0f - (fConst1 * fSlow247));
		float fSlow249 = (1.0f - fSlow247);
		float fSlow250 = (fSlow248 / fSlow249);
		float fSlow251 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow248) / spat_zita6x8_faustpower2_f(fSlow249)) + -1.0f)));
		float fSlow252 = (fSlow250 - fSlow251);
		float fSlow253 = (fSlow246 * (fSlow251 + (1.0f - fSlow250)));
		float fSlow254 = float(fVslider18);
		float fSlow255 = ((std::exp((fConst27 / fSlow254)) / fSlow246) + -1.0f);
		float fSlow256 = std::exp((fConst32 / fSlow245));
		float fSlow257 = spat_zita6x8_faustpower2_f(fSlow256);
		float fSlow258 = (1.0f - (fConst1 * fSlow257));
		float fSlow259 = (1.0f - fSlow257);
		float fSlow260 = (fSlow258 / fSlow259);
		float fSlow261 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow258) / spat_zita6x8_faustpower2_f(fSlow259)) + -1.0f)));
		float fSlow262 = (fSlow260 - fSlow261);
		float fSlow263 = (fSlow256 * (fSlow261 + (1.0f - fSlow260)));
		float fSlow264 = ((std::exp((fConst32 / fSlow254)) / fSlow256) + -1.0f);
		int iSlow265 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst14 * float(fVslider19)))));
		float fSlow266 = std::exp((fConst11 / fSlow245));
		float fSlow267 = spat_zita6x8_faustpower2_f(fSlow266);
		float fSlow268 = (1.0f - (fConst1 * fSlow267));
		float fSlow269 = (1.0f - fSlow267);
		float fSlow270 = (fSlow268 / fSlow269);
		float fSlow271 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow268) / spat_zita6x8_faustpower2_f(fSlow269)) + -1.0f)));
		float fSlow272 = (fSlow270 - fSlow271);
		float fSlow273 = (fSlow266 * (fSlow271 + (1.0f - fSlow270)));
		float fSlow274 = ((std::exp((fConst11 / fSlow254)) / fSlow266) + -1.0f);
		float fSlow275 = std::exp((fConst37 / fSlow245));
		float fSlow276 = spat_zita6x8_faustpower2_f(fSlow275);
		float fSlow277 = (1.0f - (fConst1 * fSlow276));
		float fSlow278 = (1.0f - fSlow276);
		float fSlow279 = (fSlow277 / fSlow278);
		float fSlow280 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow277) / spat_zita6x8_faustpower2_f(fSlow278)) + -1.0f)));
		float fSlow281 = (fSlow279 - fSlow280);
		float fSlow282 = (fSlow275 * (fSlow280 + (1.0f - fSlow279)));
		float fSlow283 = ((std::exp((fConst37 / fSlow254)) / fSlow275) + -1.0f);
		float fSlow284 = std::exp((fConst3 / fSlow245));
		float fSlow285 = spat_zita6x8_faustpower2_f(fSlow284);
		float fSlow286 = (1.0f - (fConst1 * fSlow285));
		float fSlow287 = (1.0f - fSlow285);
		float fSlow288 = (fSlow286 / fSlow287);
		float fSlow289 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow286) / spat_zita6x8_faustpower2_f(fSlow287)) + -1.0f)));
		float fSlow290 = (fSlow288 - fSlow289);
		float fSlow291 = (fSlow284 * (fSlow289 + (1.0f - fSlow288)));
		float fSlow292 = ((std::exp((fConst3 / fSlow254)) / fSlow284) + -1.0f);
		float fSlow293 = std::exp((fConst17 / fSlow245));
		float fSlow294 = spat_zita6x8_faustpower2_f(fSlow293);
		float fSlow295 = (1.0f - (fConst1 * fSlow294));
		float fSlow296 = (1.0f - fSlow294);
		float fSlow297 = (fSlow295 / fSlow296);
		float fSlow298 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow295) / spat_zita6x8_faustpower2_f(fSlow296)) + -1.0f)));
		float fSlow299 = (fSlow297 - fSlow298);
		float fSlow300 = (fSlow293 * (fSlow298 + (1.0f - fSlow297)));
		float fSlow301 = ((std::exp((fConst17 / fSlow254)) / fSlow293) + -1.0f);
		float fSlow302 = std::exp((fConst22 / fSlow245));
		float fSlow303 = spat_zita6x8_faustpower2_f(fSlow302);
		float fSlow304 = (1.0f - (fConst1 * fSlow303));
		float fSlow305 = (1.0f - fSlow303);
		float fSlow306 = (fSlow304 / fSlow305);
		float fSlow307 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow304) / spat_zita6x8_faustpower2_f(fSlow305)) + -1.0f)));
		float fSlow308 = (fSlow306 - fSlow307);
		float fSlow309 = (fSlow302 * (fSlow307 + (1.0f - fSlow306)));
		float fSlow310 = ((std::exp((fConst22 / fSlow254)) / fSlow302) + -1.0f);
		float fSlow311 = std::exp((fConst42 / fSlow245));
		float fSlow312 = spat_zita6x8_faustpower2_f(fSlow311);
		float fSlow313 = (1.0f - (fConst1 * fSlow312));
		float fSlow314 = (1.0f - fSlow312);
		float fSlow315 = (fSlow313 / fSlow314);
		float fSlow316 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow313) / spat_zita6x8_faustpower2_f(fSlow314)) + -1.0f)));
		float fSlow317 = (fSlow315 - fSlow316);
		float fSlow318 = (fSlow311 * (fSlow316 + (1.0f - fSlow315)));
		float fSlow319 = ((std::exp((fConst42 / fSlow254)) / fSlow311) + -1.0f);
		float fSlow320 = float(fVslider20);
		float fSlow321 = (fSlow320 + 1.0f);
		float fSlow322 = float(fVslider21);
		float fSlow323 = (4.99999987e-05f * (fSlow321 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow320 * std::fabs((std::fmod((fSlow322 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow324 = (0.00100000005f * float(fHslider4));
		float fSlow325 = float(fVslider22);
		float fSlow326 = std::exp((fConst3 / fSlow325));
		float fSlow327 = spat_zita6x8_faustpower2_f(fSlow326);
		float fSlow328 = (1.0f - (fConst1 * fSlow327));
		float fSlow329 = (1.0f - fSlow327);
		float fSlow330 = (fSlow328 / fSlow329);
		float fSlow331 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow328) / spat_zita6x8_faustpower2_f(fSlow329)) + -1.0f)));
		float fSlow332 = (fSlow330 - fSlow331);
		float fSlow333 = (fSlow326 * (fSlow331 + (1.0f - fSlow330)));
		float fSlow334 = float(fVslider23);
		float fSlow335 = ((std::exp((fConst3 / fSlow334)) / fSlow326) + -1.0f);
		float fSlow336 = std::exp((fConst42 / fSlow325));
		float fSlow337 = spat_zita6x8_faustpower2_f(fSlow336);
		float fSlow338 = (1.0f - (fConst1 * fSlow337));
		float fSlow339 = (1.0f - fSlow337);
		float fSlow340 = (fSlow338 / fSlow339);
		float fSlow341 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow338) / spat_zita6x8_faustpower2_f(fSlow339)) + -1.0f)));
		float fSlow342 = (fSlow340 - fSlow341);
		float fSlow343 = (fSlow336 * (fSlow341 + (1.0f - fSlow340)));
		float fSlow344 = ((std::exp((fConst42 / fSlow334)) / fSlow336) + -1.0f);
		int iSlow345 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst14 * float(fVslider24)))));
		float fSlow346 = std::exp((fConst17 / fSlow325));
		float fSlow347 = spat_zita6x8_faustpower2_f(fSlow346);
		float fSlow348 = (1.0f - (fConst1 * fSlow347));
		float fSlow349 = (1.0f - fSlow347);
		float fSlow350 = (fSlow348 / fSlow349);
		float fSlow351 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow348) / spat_zita6x8_faustpower2_f(fSlow349)) + -1.0f)));
		float fSlow352 = (fSlow350 - fSlow351);
		float fSlow353 = (fSlow346 * (fSlow351 + (1.0f - fSlow350)));
		float fSlow354 = ((std::exp((fConst17 / fSlow334)) / fSlow346) + -1.0f);
		float fSlow355 = std::exp((fConst22 / fSlow325));
		float fSlow356 = spat_zita6x8_faustpower2_f(fSlow355);
		float fSlow357 = (1.0f - (fConst1 * fSlow356));
		float fSlow358 = (1.0f - fSlow356);
		float fSlow359 = (fSlow357 / fSlow358);
		float fSlow360 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow357) / spat_zita6x8_faustpower2_f(fSlow358)) + -1.0f)));
		float fSlow361 = (fSlow359 - fSlow360);
		float fSlow362 = (fSlow355 * (fSlow360 + (1.0f - fSlow359)));
		float fSlow363 = ((std::exp((fConst22 / fSlow334)) / fSlow355) + -1.0f);
		float fSlow364 = std::exp((fConst27 / fSlow325));
		float fSlow365 = spat_zita6x8_faustpower2_f(fSlow364);
		float fSlow366 = (1.0f - (fConst1 * fSlow365));
		float fSlow367 = (1.0f - fSlow365);
		float fSlow368 = (fSlow366 / fSlow367);
		float fSlow369 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow366) / spat_zita6x8_faustpower2_f(fSlow367)) + -1.0f)));
		float fSlow370 = (fSlow368 - fSlow369);
		float fSlow371 = (fSlow364 * (fSlow369 + (1.0f - fSlow368)));
		float fSlow372 = ((std::exp((fConst27 / fSlow334)) / fSlow364) + -1.0f);
		float fSlow373 = std::exp((fConst32 / fSlow325));
		float fSlow374 = spat_zita6x8_faustpower2_f(fSlow373);
		float fSlow375 = (1.0f - (fConst1 * fSlow374));
		float fSlow376 = (1.0f - fSlow374);
		float fSlow377 = (fSlow375 / fSlow376);
		float fSlow378 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow375) / spat_zita6x8_faustpower2_f(fSlow376)) + -1.0f)));
		float fSlow379 = (fSlow377 - fSlow378);
		float fSlow380 = (fSlow373 * (fSlow378 + (1.0f - fSlow377)));
		float fSlow381 = ((std::exp((fConst32 / fSlow334)) / fSlow373) + -1.0f);
		float fSlow382 = std::exp((fConst11 / fSlow325));
		float fSlow383 = spat_zita6x8_faustpower2_f(fSlow382);
		float fSlow384 = (1.0f - (fConst1 * fSlow383));
		float fSlow385 = (1.0f - fSlow383);
		float fSlow386 = (fSlow384 / fSlow385);
		float fSlow387 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow384) / spat_zita6x8_faustpower2_f(fSlow385)) + -1.0f)));
		float fSlow388 = (fSlow386 - fSlow387);
		float fSlow389 = (fSlow382 * (fSlow387 + (1.0f - fSlow386)));
		float fSlow390 = ((std::exp((fConst11 / fSlow334)) / fSlow382) + -1.0f);
		float fSlow391 = std::exp((fConst37 / fSlow325));
		float fSlow392 = spat_zita6x8_faustpower2_f(fSlow391);
		float fSlow393 = (1.0f - (fConst1 * fSlow392));
		float fSlow394 = (1.0f - fSlow392);
		float fSlow395 = (fSlow393 / fSlow394);
		float fSlow396 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow393) / spat_zita6x8_faustpower2_f(fSlow394)) + -1.0f)));
		float fSlow397 = (fSlow395 - fSlow396);
		float fSlow398 = (fSlow391 * (fSlow396 + (1.0f - fSlow395)));
		float fSlow399 = ((std::exp((fConst37 / fSlow334)) / fSlow391) + -1.0f);
		float fSlow400 = float(fVslider25);
		float fSlow401 = (fSlow400 + 1.0f);
		float fSlow402 = float(fVslider26);
		float fSlow403 = (4.99999987e-05f * (fSlow401 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow400 * std::fabs((std::fmod((fSlow402 + 1.5f), 1.0f) + -0.5f)))))))))));
		float fSlow404 = (0.00100000005f * float(fHslider5));
		float fSlow405 = float(fVslider27);
		float fSlow406 = std::exp((fConst3 / fSlow405));
		float fSlow407 = spat_zita6x8_faustpower2_f(fSlow406);
		float fSlow408 = (1.0f - (fConst1 * fSlow407));
		float fSlow409 = (1.0f - fSlow407);
		float fSlow410 = (fSlow408 / fSlow409);
		float fSlow411 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow408) / spat_zita6x8_faustpower2_f(fSlow409)) + -1.0f)));
		float fSlow412 = (fSlow410 - fSlow411);
		float fSlow413 = (fSlow406 * (fSlow411 + (1.0f - fSlow410)));
		float fSlow414 = float(fVslider28);
		float fSlow415 = ((std::exp((fConst3 / fSlow414)) / fSlow406) + -1.0f);
		float fSlow416 = std::exp((fConst17 / fSlow405));
		float fSlow417 = spat_zita6x8_faustpower2_f(fSlow416);
		float fSlow418 = (1.0f - (fConst1 * fSlow417));
		float fSlow419 = (1.0f - fSlow417);
		float fSlow420 = (fSlow418 / fSlow419);
		float fSlow421 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow418) / spat_zita6x8_faustpower2_f(fSlow419)) + -1.0f)));
		float fSlow422 = (fSlow420 - fSlow421);
		float fSlow423 = (fSlow416 * (fSlow421 + (1.0f - fSlow420)));
		float fSlow424 = ((std::exp((fConst17 / fSlow414)) / fSlow416) + -1.0f);
		float fSlow425 = std::exp((fConst22 / fSlow405));
		float fSlow426 = spat_zita6x8_faustpower2_f(fSlow425);
		float fSlow427 = (1.0f - (fConst1 * fSlow426));
		float fSlow428 = (1.0f - fSlow426);
		float fSlow429 = (fSlow427 / fSlow428);
		float fSlow430 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow427) / spat_zita6x8_faustpower2_f(fSlow428)) + -1.0f)));
		float fSlow431 = (fSlow429 - fSlow430);
		float fSlow432 = (fSlow425 * (fSlow430 + (1.0f - fSlow429)));
		float fSlow433 = ((std::exp((fConst22 / fSlow414)) / fSlow425) + -1.0f);
		float fSlow434 = std::exp((fConst27 / fSlow405));
		float fSlow435 = spat_zita6x8_faustpower2_f(fSlow434);
		float fSlow436 = (1.0f - (fConst1 * fSlow435));
		float fSlow437 = (1.0f - fSlow435);
		float fSlow438 = (fSlow436 / fSlow437);
		float fSlow439 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow436) / spat_zita6x8_faustpower2_f(fSlow437)) + -1.0f)));
		float fSlow440 = (fSlow438 - fSlow439);
		float fSlow441 = (fSlow434 * (fSlow439 + (1.0f - fSlow438)));
		float fSlow442 = ((std::exp((fConst27 / fSlow414)) / fSlow434) + -1.0f);
		float fSlow443 = std::exp((fConst32 / fSlow405));
		float fSlow444 = spat_zita6x8_faustpower2_f(fSlow443);
		float fSlow445 = (1.0f - (fConst1 * fSlow444));
		float fSlow446 = (1.0f - fSlow444);
		float fSlow447 = (fSlow445 / fSlow446);
		float fSlow448 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow445) / spat_zita6x8_faustpower2_f(fSlow446)) + -1.0f)));
		float fSlow449 = (fSlow447 - fSlow448);
		float fSlow450 = (fSlow443 * (fSlow448 + (1.0f - fSlow447)));
		float fSlow451 = ((std::exp((fConst32 / fSlow414)) / fSlow443) + -1.0f);
		int iSlow452 = int(std::min<float>(8192.0f, std::max<float>(0.0f, (fConst14 * float(fVslider29)))));
		float fSlow453 = std::exp((fConst11 / fSlow405));
		float fSlow454 = spat_zita6x8_faustpower2_f(fSlow453);
		float fSlow455 = (1.0f - (fConst1 * fSlow454));
		float fSlow456 = (1.0f - fSlow454);
		float fSlow457 = (fSlow455 / fSlow456);
		float fSlow458 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow455) / spat_zita6x8_faustpower2_f(fSlow456)) + -1.0f)));
		float fSlow459 = (fSlow457 - fSlow458);
		float fSlow460 = (fSlow453 * (fSlow458 + (1.0f - fSlow457)));
		float fSlow461 = ((std::exp((fConst11 / fSlow414)) / fSlow453) + -1.0f);
		float fSlow462 = std::exp((fConst37 / fSlow405));
		float fSlow463 = spat_zita6x8_faustpower2_f(fSlow462);
		float fSlow464 = (1.0f - (fConst1 * fSlow463));
		float fSlow465 = (1.0f - fSlow463);
		float fSlow466 = (fSlow464 / fSlow465);
		float fSlow467 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow464) / spat_zita6x8_faustpower2_f(fSlow465)) + -1.0f)));
		float fSlow468 = (fSlow466 - fSlow467);
		float fSlow469 = (fSlow462 * (fSlow467 + (1.0f - fSlow466)));
		float fSlow470 = ((std::exp((fConst37 / fSlow414)) / fSlow462) + -1.0f);
		float fSlow471 = std::exp((fConst42 / fSlow405));
		float fSlow472 = spat_zita6x8_faustpower2_f(fSlow471);
		float fSlow473 = (1.0f - (fConst1 * fSlow472));
		float fSlow474 = (1.0f - fSlow472);
		float fSlow475 = (fSlow473 / fSlow474);
		float fSlow476 = std::sqrt(std::max<float>(0.0f, ((spat_zita6x8_faustpower2_f(fSlow473) / spat_zita6x8_faustpower2_f(fSlow474)) + -1.0f)));
		float fSlow477 = (fSlow475 - fSlow476);
		float fSlow478 = (fSlow471 * (fSlow476 + (1.0f - fSlow475)));
		float fSlow479 = ((std::exp((fConst42 / fSlow414)) / fSlow471) + -1.0f);
		float fSlow480 = (4.99999987e-05f * (fSlow161 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow160 * std::fabs((std::fmod((fSlow162 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow481 = (4.99999987e-05f * (fSlow241 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow240 * std::fabs((std::fmod((fSlow242 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow482 = (4.99999987e-05f * (fSlow81 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow80 * std::fabs((std::fmod((fSlow82 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow483 = (4.99999987e-05f * (fSlow321 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow320 * std::fabs((std::fmod((fSlow322 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow484 = (4.99999987e-05f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow485 = (4.99999987e-05f * (fSlow401 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow400 * std::fabs((std::fmod((fSlow402 + 1.375f), 1.0f) + -0.5f)))))))))));
		float fSlow486 = (4.99999987e-05f * (fSlow241 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow240 * std::fabs((std::fmod((fSlow242 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow487 = (4.99999987e-05f * (fSlow161 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow160 * std::fabs((std::fmod((fSlow162 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow488 = (4.99999987e-05f * (fSlow81 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow80 * std::fabs((std::fmod((fSlow82 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow489 = (4.99999987e-05f * (fSlow321 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow320 * std::fabs((std::fmod((fSlow322 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow490 = (4.99999987e-05f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow491 = (4.99999987e-05f * (fSlow401 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow400 * std::fabs((std::fmod((fSlow402 + 1.25f), 1.0f) + -0.5f)))))))))));
		float fSlow492 = (4.99999987e-05f * (fSlow161 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow160 * std::fabs((std::fmod((fSlow162 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow493 = (4.99999987e-05f * (fSlow241 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow240 * std::fabs((std::fmod((fSlow242 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow494 = (4.99999987e-05f * (fSlow81 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow80 * std::fabs((std::fmod((fSlow82 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow495 = (4.99999987e-05f * (fSlow321 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow320 * std::fabs((std::fmod((fSlow322 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow496 = (4.99999987e-05f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow497 = (4.99999987e-05f * (fSlow401 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow400 * std::fabs((std::fmod((fSlow402 + 1.125f), 1.0f) + -0.5f)))))))))));
		float fSlow498 = (4.99999987e-05f * (fSlow161 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow160 * std::fabs((std::fmod((fSlow162 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow499 = (4.99999987e-05f * (fSlow241 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow240 * std::fabs((std::fmod((fSlow242 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow500 = (4.99999987e-05f * (fSlow81 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow80 * std::fabs((std::fmod((fSlow82 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow501 = (4.99999987e-05f * (fSlow321 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow320 * std::fabs((std::fmod((fSlow322 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow502 = (4.99999987e-05f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow503 = (4.99999987e-05f * (fSlow401 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow400 * std::fabs((std::fmod((fSlow402 + 1.0f), 1.0f) + -0.5f)))))))))));
		float fSlow504 = (4.99999987e-05f * (fSlow161 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow160 * std::fabs((std::fmod((fSlow162 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow505 = (4.99999987e-05f * (fSlow241 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow240 * std::fabs((std::fmod((fSlow242 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow506 = (4.99999987e-05f * (fSlow81 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow80 * std::fabs((std::fmod((fSlow82 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow507 = (4.99999987e-05f * (fSlow321 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow320 * std::fabs((std::fmod((fSlow322 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow508 = (4.99999987e-05f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow509 = (4.99999987e-05f * (fSlow401 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow400 * std::fabs((std::fmod((fSlow402 + 0.875f), 1.0f) + -0.5f)))))))))));
		float fSlow510 = (4.99999987e-05f * (fSlow161 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow160 * std::fabs((std::fmod((fSlow162 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow511 = (4.99999987e-05f * (fSlow241 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow240 * std::fabs((std::fmod((fSlow242 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow512 = (4.99999987e-05f * (fSlow81 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow80 * std::fabs((std::fmod((fSlow82 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow513 = (4.99999987e-05f * (fSlow321 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow320 * std::fabs((std::fmod((fSlow322 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow514 = (4.99999987e-05f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow515 = (4.99999987e-05f * (fSlow401 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow400 * std::fabs((std::fmod((fSlow402 + 0.75f), 1.0f) + -0.5f)))))))))));
		float fSlow516 = (4.99999987e-05f * (fSlow161 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow160 * std::fabs((std::fmod((fSlow162 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow517 = (4.99999987e-05f * (fSlow241 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow240 * std::fabs((std::fmod((fSlow242 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow518 = (4.99999987e-05f * (fSlow81 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow80 * std::fabs((std::fmod((fSlow82 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow519 = (4.99999987e-05f * (fSlow321 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow320 * std::fabs((std::fmod((fSlow322 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow520 = (4.99999987e-05f * (fSlow1 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow0 * std::fabs((std::fmod((fSlow2 + 0.625f), 1.0f) + -0.5f)))))))))));
		float fSlow521 = (4.99999987e-05f * (fSlow401 * float(q8_sqrt(float(std::max<float>(0.0f, (1.0f - (8.0f * (fSlow400 * std::fabs((std::fmod((fSlow402 + 0.625f), 1.0f) + -0.5f)))))))))));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec0[0] = (fSlow3 + (0.999899983f * fRec0[1]));
			float fTemp0 = float(input4[i]);
			fVec0[(IOTA & 16383)] = fTemp0;
			fRec1[0] = (fSlow4 + (0.999000013f * fRec1[1]));
			fRec13[0] = (0.0f - (fConst5 * ((fConst6 * fRec13[1]) - (fRec9[1] + fRec9[2]))));
			fRec12[0] = ((fSlow12 * fRec12[1]) + (fSlow13 * (fRec9[1] + (fSlow15 * fRec13[0]))));
			fVec1[(IOTA & 65535)] = ((0.353553385f * fRec12[0]) + 9.99999968e-21f);
			float fTemp1 = ((0.600000024f * fRec10[1]) + fVec1[((IOTA - iConst8) & 65535)]);
			fVec2[(IOTA & 4095)] = fTemp1;
			fRec10[0] = fVec2[((IOTA - iConst9) & 4095)];
			float fRec11 = (0.0f - (0.600000024f * fTemp1));
			fRec17[0] = (fConst5 * ((fRec4[1] + fRec4[2]) - (fConst6 * fRec17[1])));
			fRec16[0] = ((fSlow22 * fRec16[1]) + (fSlow23 * (fRec4[1] + (fSlow24 * fRec17[0]))));
			fVec3[(IOTA & 32767)] = ((0.353553385f * fRec16[0]) + 9.99999968e-21f);
			float fTemp2 = (0.300000012f * fVec0[((IOTA - iSlow25) & 16383)]);
			float fTemp3 = (fVec3[((IOTA - iConst13) & 32767)] - (fTemp2 + (0.600000024f * fRec14[1])));
			fVec4[(IOTA & 8191)] = fTemp3;
			fRec14[0] = fVec4[((IOTA - iConst15) & 8191)];
			float fRec15 = (0.600000024f * fTemp3);
			fRec21[0] = (0.0f - (fConst5 * ((fConst6 * fRec21[1]) - (fRec5[1] + fRec5[2]))));
			fRec20[0] = ((fSlow32 * fRec20[1]) + (fSlow33 * (fRec5[1] + (fSlow34 * fRec21[0]))));
			fVec5[(IOTA & 65535)] = ((0.353553385f * fRec20[0]) + 9.99999968e-21f);
			float fTemp4 = ((0.600000024f * fRec18[1]) + fVec5[((IOTA - iConst19) & 65535)]);
			fVec6[(IOTA & 8191)] = fTemp4;
			fRec18[0] = fVec6[((IOTA - iConst20) & 8191)];
			float fRec19 = (0.0f - (0.600000024f * fTemp4));
			fRec25[0] = (0.0f - (fConst5 * ((fConst6 * fRec25[1]) - (fRec7[1] + fRec7[2]))));
			fRec24[0] = ((fSlow41 * fRec24[1]) + (fSlow42 * (fRec7[1] + (fSlow43 * fRec25[0]))));
			fVec7[(IOTA & 32767)] = ((0.353553385f * fRec24[0]) + 9.99999968e-21f);
			float fTemp5 = ((0.600000024f * fRec22[1]) + fVec7[((IOTA - iConst24) & 32767)]);
			fVec8[(IOTA & 8191)] = fTemp5;
			fRec22[0] = fVec8[((IOTA - iConst25) & 8191)];
			float fRec23 = (0.0f - (0.600000024f * fTemp5));
			fRec29[0] = (0.0f - (fConst5 * ((fConst6 * fRec29[1]) - (fRec3[1] + fRec3[2]))));
			fRec28[0] = ((fSlow50 * fRec28[1]) + (fSlow51 * (fRec3[1] + (fSlow52 * fRec29[0]))));
			fVec9[(IOTA & 65535)] = ((0.353553385f * fRec28[0]) + 9.99999968e-21f);
			float fTemp6 = ((0.600000024f * fRec26[1]) + fVec9[((IOTA - iConst29) & 65535)]);
			fVec10[(IOTA & 8191)] = fTemp6;
			fRec26[0] = fVec10[((IOTA - iConst30) & 8191)];
			float fRec27 = (0.0f - (0.600000024f * fTemp6));
			fRec33[0] = (0.0f - (fConst5 * ((fConst6 * fRec33[1]) - (fRec8[1] + fRec8[2]))));
			fRec32[0] = ((fSlow59 * fRec32[1]) + (fSlow60 * (fRec8[1] + (fSlow61 * fRec33[0]))));
			fVec11[(IOTA & 32767)] = ((0.353553385f * fRec32[0]) + 9.99999968e-21f);
			float fTemp7 = (fVec11[((IOTA - iConst34) & 32767)] - (fTemp2 + (0.600000024f * fRec30[1])));
			fVec12[(IOTA & 4095)] = fTemp7;
			fRec30[0] = fVec12[((IOTA - iConst35) & 4095)];
			float fRec31 = (0.600000024f * fTemp7);
			fRec37[0] = (0.0f - (fConst5 * ((fConst6 * fRec37[1]) - (fRec6[1] + fRec6[2]))));
			fRec36[0] = ((fSlow68 * fRec36[1]) + (fSlow69 * (fRec6[1] + (fSlow70 * fRec37[0]))));
			fVec13[(IOTA & 32767)] = ((0.353553385f * fRec36[0]) + 9.99999968e-21f);
			float fTemp8 = ((fTemp2 + fVec13[((IOTA - iConst39) & 32767)]) - (0.600000024f * fRec34[1]));
			fVec14[(IOTA & 8191)] = fTemp8;
			fRec34[0] = fVec14[((IOTA - iConst40) & 8191)];
			float fRec35 = (0.600000024f * fTemp8);
			fRec41[0] = (0.0f - (fConst5 * ((fConst6 * fRec41[1]) - (fRec2[1] + fRec2[2]))));
			fRec40[0] = ((fSlow77 * fRec40[1]) + (fSlow78 * (fRec2[1] + (fSlow79 * fRec41[0]))));
			fVec15[(IOTA & 32767)] = ((0.353553385f * fRec40[0]) + 9.99999968e-21f);
			float fTemp9 = ((fVec15[((IOTA - iConst44) & 32767)] + fTemp2) - (0.600000024f * fRec38[1]));
			fVec16[(IOTA & 4095)] = fTemp9;
			fRec38[0] = fVec16[((IOTA - iConst45) & 4095)];
			float fRec39 = (0.600000024f * fTemp9);
			float fTemp10 = (fRec38[1] + fRec34[1]);
			float fTemp11 = (fTemp10 + fRec30[1]);
			fRec2[0] = (fRec10[1] + (fRec14[1] + (fRec11 + (fRec19 + (fRec23 + (fRec27 + (fRec31 + (fRec15 + (fRec35 + (fRec39 + (fRec18[1] + (fRec22[1] + (fTemp11 + fRec26[1])))))))))))));
			fRec3[0] = ((fRec14[1] + (fRec31 + (fRec15 + (fRec35 + (fRec39 + fTemp11))))) - (fRec10[1] + (fRec11 + (fRec19 + (fRec23 + (fRec27 + ((fRec26[1] + fRec22[1]) + fRec18[1])))))));
			fRec4[0] = ((fRec23 + (fRec27 + (fRec35 + (fRec39 + (fRec22[1] + (fTemp10 + fRec26[1])))))) - (fRec10[1] + (fRec14[1] + (fRec11 + (fRec19 + (fRec31 + (fRec15 + (fRec30[1] + fRec18[1]))))))));
			fRec5[0] = ((fRec10[1] + (fRec11 + (fRec19 + (fRec35 + (fRec39 + (fTemp10 + fRec18[1])))))) - (fRec14[1] + (fRec23 + (fRec27 + (fRec31 + (fRec15 + (fRec22[1] + (fRec30[1] + fRec26[1]))))))));
			float fTemp12 = (fRec34[1] + fRec30[1]);
			fRec6[0] = ((fRec14[1] + (fRec19 + (fRec27 + (fRec15 + (fRec39 + (fRec18[1] + (fRec38[1] + fRec26[1]))))))) - (fRec10[1] + (fRec11 + (fRec23 + (fRec31 + (fRec35 + (fRec22[1] + fTemp12)))))));
			fRec7[0] = ((fRec10[1] + (fRec14[1] + (fRec11 + (fRec23 + (fRec15 + (fRec39 + (fRec38[1] + fRec22[1]))))))) - (fRec19 + (fRec27 + (fRec31 + (fRec35 + (fRec18[1] + (fRec26[1] + fTemp12)))))));
			float fTemp13 = (fRec38[1] + fRec30[1]);
			fRec8[0] = ((fRec10[1] + (fRec11 + (fRec27 + (fRec31 + (fRec39 + (fRec26[1] + fTemp13)))))) - (fRec14[1] + (fRec19 + (fRec23 + (fRec15 + (fRec35 + (fRec18[1] + (fRec34[1] + fRec22[1]))))))));
			fRec9[0] = ((fRec19 + (fRec23 + (fRec31 + (fRec39 + (fRec18[1] + (fRec22[1] + fTemp13)))))) - (fRec10[1] + (fRec14[1] + (fRec11 + (fRec27 + (fRec15 + (fRec35 + (fRec34[1] + fRec26[1]))))))));
			float fTemp14 = ((fTemp0 * (1.0f - fRec1[0])) + (0.370000005f * (fRec1[0] * (fRec3[0] + fRec4[0]))));
			fRec42[0] = (fSlow83 + (0.999899983f * fRec42[1]));
			float fTemp15 = float(input2[i]);
			fVec17[(IOTA & 16383)] = fTemp15;
			fRec43[0] = (fSlow84 + (0.999000013f * fRec43[1]));
			fRec55[0] = (0.0f - (fConst5 * ((fConst6 * fRec55[1]) - (fRec51[1] + fRec51[2]))));
			fRec54[0] = ((fSlow92 * fRec54[1]) + (fSlow93 * (fRec51[1] + (fSlow95 * fRec55[0]))));
			fVec18[(IOTA & 65535)] = ((0.353553385f * fRec54[0]) + 9.99999968e-21f);
			float fTemp16 = ((0.600000024f * fRec52[1]) + fVec18[((IOTA - iConst8) & 65535)]);
			fVec19[(IOTA & 4095)] = fTemp16;
			fRec52[0] = fVec19[((IOTA - iConst9) & 4095)];
			float fRec53 = (0.0f - (0.600000024f * fTemp16));
			fRec59[0] = (0.0f - (fConst5 * ((fConst6 * fRec59[1]) - (fRec47[1] + fRec47[2]))));
			fRec58[0] = ((fSlow102 * fRec58[1]) + (fSlow103 * (fRec47[1] + (fSlow104 * fRec59[0]))));
			fVec20[(IOTA & 65535)] = ((0.353553385f * fRec58[0]) + 9.99999968e-21f);
			float fTemp17 = ((0.600000024f * fRec56[1]) + fVec20[((IOTA - iConst19) & 65535)]);
			fVec21[(IOTA & 8191)] = fTemp17;
			fRec56[0] = fVec21[((IOTA - iConst20) & 8191)];
			float fRec57 = (0.0f - (0.600000024f * fTemp17));
			fRec63[0] = (0.0f - (fConst5 * ((fConst6 * fRec63[1]) - (fRec49[1] + fRec49[2]))));
			fRec62[0] = ((fSlow111 * fRec62[1]) + (fSlow112 * (fRec49[1] + (fSlow113 * fRec63[0]))));
			fVec22[(IOTA & 32767)] = ((0.353553385f * fRec62[0]) + 9.99999968e-21f);
			float fTemp18 = ((0.600000024f * fRec60[1]) + fVec22[((IOTA - iConst24) & 32767)]);
			fVec23[(IOTA & 8191)] = fTemp18;
			fRec60[0] = fVec23[((IOTA - iConst25) & 8191)];
			float fRec61 = (0.0f - (0.600000024f * fTemp18));
			fRec67[0] = (0.0f - (fConst5 * ((fConst6 * fRec67[1]) - (fRec45[1] + fRec45[2]))));
			fRec66[0] = ((fSlow120 * fRec66[1]) + (fSlow121 * (fRec45[1] + (fSlow122 * fRec67[0]))));
			fVec24[(IOTA & 65535)] = ((0.353553385f * fRec66[0]) + 9.99999968e-21f);
			float fTemp19 = ((0.600000024f * fRec64[1]) + fVec24[((IOTA - iConst29) & 65535)]);
			fVec25[(IOTA & 8191)] = fTemp19;
			fRec64[0] = fVec25[((IOTA - iConst30) & 8191)];
			float fRec65 = (0.0f - (0.600000024f * fTemp19));
			fRec71[0] = (0.0f - (fConst5 * ((fConst6 * fRec71[1]) - (fRec50[1] + fRec50[2]))));
			fRec70[0] = ((fSlow129 * fRec70[1]) + (fSlow130 * (fRec50[1] + (fSlow131 * fRec71[0]))));
			fVec26[(IOTA & 32767)] = ((0.353553385f * fRec70[0]) + 9.99999968e-21f);
			float fTemp20 = (0.300000012f * fVec17[((IOTA - iSlow132) & 16383)]);
			float fTemp21 = (fVec26[((IOTA - iConst34) & 32767)] - (fTemp20 + (0.600000024f * fRec68[1])));
			fVec27[(IOTA & 4095)] = fTemp21;
			fRec68[0] = fVec27[((IOTA - iConst35) & 4095)];
			float fRec69 = (0.600000024f * fTemp21);
			fRec75[0] = (0.0f - (fConst5 * ((fConst6 * fRec75[1]) - (fRec46[1] + fRec46[2]))));
			fRec74[0] = ((fSlow139 * fRec74[1]) + (fSlow140 * (fRec46[1] + (fSlow141 * fRec75[0]))));
			fVec28[(IOTA & 32767)] = ((0.353553385f * fRec74[0]) + 9.99999968e-21f);
			float fTemp22 = (fVec28[((IOTA - iConst13) & 32767)] - (fTemp20 + (0.600000024f * fRec72[1])));
			fVec29[(IOTA & 8191)] = fTemp22;
			fRec72[0] = fVec29[((IOTA - iConst15) & 8191)];
			float fRec73 = (0.600000024f * fTemp22);
			fRec79[0] = (0.0f - (fConst5 * ((fConst6 * fRec79[1]) - (fRec48[1] + fRec48[2]))));
			fRec78[0] = ((fSlow148 * fRec78[1]) + (fSlow149 * (fRec48[1] + (fSlow150 * fRec79[0]))));
			fVec30[(IOTA & 32767)] = ((0.353553385f * fRec78[0]) + 9.99999968e-21f);
			float fTemp23 = ((fTemp20 + fVec30[((IOTA - iConst39) & 32767)]) - (0.600000024f * fRec76[1]));
			fVec31[(IOTA & 8191)] = fTemp23;
			fRec76[0] = fVec31[((IOTA - iConst40) & 8191)];
			float fRec77 = (0.600000024f * fTemp23);
			fRec83[0] = (0.0f - (fConst5 * ((fConst6 * fRec83[1]) - (fRec44[1] + fRec44[2]))));
			fRec82[0] = ((fSlow157 * fRec82[1]) + (fSlow158 * (fRec44[1] + (fSlow159 * fRec83[0]))));
			fVec32[(IOTA & 32767)] = ((0.353553385f * fRec82[0]) + 9.99999968e-21f);
			float fTemp24 = ((fVec32[((IOTA - iConst44) & 32767)] + fTemp20) - (0.600000024f * fRec80[1]));
			fVec33[(IOTA & 4095)] = fTemp24;
			fRec80[0] = fVec33[((IOTA - iConst45) & 4095)];
			float fRec81 = (0.600000024f * fTemp24);
			float fTemp25 = (fRec80[1] + fRec76[1]);
			float fTemp26 = (fRec68[1] + (fTemp25 + fRec72[1]));
			fRec44[0] = (fRec53 + (fRec57 + (fRec61 + (fRec65 + (fRec69 + (fRec73 + (fRec77 + (fRec81 + (fRec52[1] + (fRec56[1] + (fRec60[1] + (fTemp26 + fRec64[1]))))))))))));
			fRec45[0] = ((fRec69 + (fRec73 + (fRec77 + (fRec81 + fTemp26)))) - (fRec53 + (fRec57 + (fRec61 + (fRec65 + (fRec52[1] + ((fRec64[1] + fRec60[1]) + fRec56[1])))))));
			float fTemp27 = (fRec72[1] + fRec68[1]);
			fRec46[0] = ((fRec61 + (fRec65 + (fRec77 + (fRec81 + (fRec60[1] + (fTemp25 + fRec64[1])))))) - (fRec53 + (fRec57 + (fRec69 + (fRec73 + (fRec52[1] + (fTemp27 + fRec56[1])))))));
			fRec47[0] = ((fRec53 + (fRec57 + (fRec77 + (fRec81 + (fRec52[1] + (fTemp25 + fRec56[1])))))) - (fRec61 + (fRec65 + (fRec69 + (fRec73 + (fRec60[1] + (fTemp27 + fRec64[1])))))));
			float fTemp28 = (fRec80[1] + fRec72[1]);
			float fTemp29 = (fRec76[1] + fRec68[1]);
			fRec48[0] = ((fRec57 + (fRec65 + (fRec73 + (fRec81 + (fRec56[1] + (fRec64[1] + fTemp28)))))) - (fRec53 + (fRec61 + (fRec69 + (fRec77 + (fRec52[1] + (fRec60[1] + fTemp29)))))));
			fRec49[0] = ((fRec53 + (fRec61 + (fRec73 + (fRec81 + (fRec52[1] + (fRec60[1] + fTemp28)))))) - (fRec57 + (fRec65 + (fRec69 + (fRec77 + (fRec56[1] + (fRec64[1] + fTemp29)))))));
			float fTemp30 = (fRec80[1] + fRec68[1]);
			float fTemp31 = (fRec76[1] + fRec72[1]);
			fRec50[0] = ((fRec53 + (fRec65 + (fRec69 + (fRec81 + (fRec52[1] + (fRec64[1] + fTemp30)))))) - (fRec57 + (fRec61 + (fRec73 + (fRec77 + (fRec56[1] + (fRec60[1] + fTemp31)))))));
			fRec51[0] = ((fRec57 + (fRec61 + (fRec69 + (fRec81 + (fRec56[1] + (fRec60[1] + fTemp30)))))) - (fRec53 + (fRec65 + (fRec73 + (fRec77 + (fRec52[1] + (fRec64[1] + fTemp31)))))));
			float fTemp32 = ((fTemp15 * (1.0f - fRec43[0])) + (0.370000005f * (fRec43[0] * (fRec45[0] + fRec46[0]))));
			fRec84[0] = (fSlow163 + (0.999899983f * fRec84[1]));
			float fTemp33 = float(input0[i]);
			fVec34[(IOTA & 16383)] = fTemp33;
			fRec85[0] = (fSlow164 + (0.999000013f * fRec85[1]));
			fRec97[0] = (0.0f - (fConst5 * ((fConst6 * fRec97[1]) - (fRec89[1] + fRec89[2]))));
			fRec96[0] = ((fSlow172 * fRec96[1]) + (fSlow173 * (fRec89[1] + (fSlow175 * fRec97[0]))));
			fVec35[(IOTA & 65535)] = ((0.353553385f * fRec96[0]) + 9.99999968e-21f);
			float fTemp34 = ((0.600000024f * fRec94[1]) + fVec35[((IOTA - iConst19) & 65535)]);
			fVec36[(IOTA & 8191)] = fTemp34;
			fRec94[0] = fVec36[((IOTA - iConst20) & 8191)];
			float fRec95 = (0.0f - (0.600000024f * fTemp34));
			fRec101[0] = (0.0f - (fConst5 * ((fConst6 * fRec101[1]) - (fRec92[1] + fRec92[2]))));
			fRec100[0] = ((fSlow182 * fRec100[1]) + (fSlow183 * (fRec92[1] + (fSlow184 * fRec101[0]))));
			fVec37[(IOTA & 32767)] = ((0.353553385f * fRec100[0]) + 9.99999968e-21f);
			float fTemp35 = (0.300000012f * fVec34[((IOTA - iSlow185) & 16383)]);
			float fTemp36 = (fVec37[((IOTA - iConst34) & 32767)] - (fTemp35 + (0.600000024f * fRec98[1])));
			fVec38[(IOTA & 4095)] = fTemp36;
			fRec98[0] = fVec38[((IOTA - iConst35) & 4095)];
			float fRec99 = (0.600000024f * fTemp36);
			fRec105[0] = (0.0f - (fConst5 * ((fConst6 * fRec105[1]) - (fRec88[1] + fRec88[2]))));
			fRec104[0] = ((fSlow192 * fRec104[1]) + (fSlow193 * (fRec88[1] + (fSlow194 * fRec105[0]))));
			fVec39[(IOTA & 32767)] = ((0.353553385f * fRec104[0]) + 9.99999968e-21f);
			float fTemp37 = (fVec39[((IOTA - iConst13) & 32767)] - (fTemp35 + (0.600000024f * fRec102[1])));
			fVec40[(IOTA & 8191)] = fTemp37;
			fRec102[0] = fVec40[((IOTA - iConst15) & 8191)];
			float fRec103 = (0.600000024f * fTemp37);
			fRec109[0] = (fConst5 * ((fRec91[1] + fRec91[2]) - (fConst6 * fRec109[1])));
			fRec108[0] = ((fSlow201 * fRec108[1]) + (fSlow202 * (fRec91[1] + (fSlow203 * fRec109[0]))));
			fVec41[(IOTA & 32767)] = ((0.353553385f * fRec108[0]) + 9.99999968e-21f);
			float fTemp38 = ((0.600000024f * fRec106[1]) + fVec41[((IOTA - iConst24) & 32767)]);
			fVec42[(IOTA & 8191)] = fTemp38;
			fRec106[0] = fVec42[((IOTA - iConst25) & 8191)];
			float fRec107 = (0.0f - (0.600000024f * fTemp38));
			fRec113[0] = (0.0f - (fConst5 * ((fConst6 * fRec113[1]) - (fRec87[1] + fRec87[2]))));
			fRec112[0] = ((fSlow210 * fRec112[1]) + (fSlow211 * (fRec87[1] + (fSlow212 * fRec113[0]))));
			fVec43[(IOTA & 65535)] = ((0.353553385f * fRec112[0]) + 9.99999968e-21f);
			float fTemp39 = ((0.600000024f * fRec110[1]) + fVec43[((IOTA - iConst29) & 65535)]);
			fVec44[(IOTA & 8191)] = fTemp39;
			fRec110[0] = fVec44[((IOTA - iConst30) & 8191)];
			float fRec111 = (0.0f - (0.600000024f * fTemp39));
			fRec117[0] = (0.0f - (fConst5 * ((fConst6 * fRec117[1]) - (fRec90[1] + fRec90[2]))));
			fRec116[0] = ((fSlow219 * fRec116[1]) + (fSlow220 * (fRec90[1] + (fSlow221 * fRec117[0]))));
			fVec45[(IOTA & 32767)] = ((0.353553385f * fRec116[0]) + 9.99999968e-21f);
			float fTemp40 = ((fTemp35 + fVec45[((IOTA - iConst39) & 32767)]) - (0.600000024f * fRec114[1]));
			fVec46[(IOTA & 8191)] = fTemp40;
			fRec114[0] = fVec46[((IOTA - iConst40) & 8191)];
			float fRec115 = (0.600000024f * fTemp40);
			fRec121[0] = (0.0f - (fConst5 * ((fConst6 * fRec121[1]) - (fRec86[1] + fRec86[2]))));
			fRec120[0] = ((fSlow228 * fRec120[1]) + (fSlow229 * (fRec86[1] + (fSlow230 * fRec121[0]))));
			fVec47[(IOTA & 32767)] = ((0.353553385f * fRec120[0]) + 9.99999968e-21f);
			float fTemp41 = ((fVec47[((IOTA - iConst44) & 32767)] + fTemp35) - (0.600000024f * fRec118[1]));
			fVec48[(IOTA & 4095)] = fTemp41;
			fRec118[0] = fVec48[((IOTA - iConst45) & 4095)];
			float fRec119 = (0.600000024f * fTemp41);
			fRec125[0] = (0.0f - (fConst5 * ((fConst6 * fRec125[1]) - (fRec93[1] + fRec93[2]))));
			fRec124[0] = ((fSlow237 * fRec124[1]) + (fSlow238 * (fRec93[1] + (fSlow239 * fRec125[0]))));
			fVec49[(IOTA & 65535)] = ((0.353553385f * fRec124[0]) + 9.99999968e-21f);
			float fTemp42 = ((0.600000024f * fRec122[1]) + fVec49[((IOTA - iConst8) & 65535)]);
			fVec50[(IOTA & 4095)] = fTemp42;
			fRec122[0] = fVec50[((IOTA - iConst9) & 4095)];
			float fRec123 = (0.0f - (0.600000024f * fTemp42));
			float fTemp43 = (fRec119 + fRec115);
			float fTemp44 = (fRec103 + fTemp43);
			fRec86[0] = (fRec94[1] + (fRec98[1] + (fRec102[1] + (fRec106[1] + (fRec110[1] + (fRec114[1] + (fRec118[1] + (fRec123 + (fRec95 + (fRec107 + (fRec111 + (fRec99 + (fTemp44 + fRec122[1])))))))))))));
			fRec87[0] = ((fRec98[1] + (fRec102[1] + (fRec114[1] + (fRec118[1] + (fRec99 + fTemp44))))) - (fRec94[1] + (fRec106[1] + (fRec110[1] + (fRec123 + (fRec95 + (fRec107 + (fRec111 + fRec122[1]))))))));
			fRec88[0] = ((fRec106[1] + (fRec110[1] + (fRec114[1] + (fRec118[1] + (fRec107 + (fRec111 + fTemp43)))))) - (fRec94[1] + (fRec98[1] + (fRec102[1] + (fRec123 + (fRec95 + (fRec99 + (fRec103 + fRec122[1]))))))));
			fRec89[0] = ((fRec94[1] + (fRec114[1] + (fRec118[1] + (fRec123 + (fRec95 + (fTemp43 + fRec122[1])))))) - (fRec98[1] + (fRec102[1] + (fRec106[1] + (fRec110[1] + (fRec107 + (fRec111 + (fRec103 + fRec99))))))));
			float fTemp45 = (fRec119 + fRec103);
			fRec90[0] = ((fRec94[1] + (fRec102[1] + (fRec110[1] + (fRec118[1] + (fRec95 + (fRec111 + fTemp45)))))) - (fRec98[1] + (fRec106[1] + (fRec114[1] + (fRec123 + (fRec107 + (fRec99 + (fRec115 + fRec122[1]))))))));
			fRec91[0] = ((fRec102[1] + (fRec106[1] + (fRec118[1] + (fRec123 + (fRec107 + (fRec122[1] + fTemp45)))))) - (fRec94[1] + (fRec98[1] + (fRec110[1] + (fRec114[1] + (fRec95 + (fRec111 + (fRec115 + fRec99))))))));
			float fTemp46 = (fRec115 + fRec103);
			fRec92[0] = ((fRec98[1] + (fRec110[1] + (fRec118[1] + (fRec123 + (fRec111 + (fRec99 + (fRec119 + fRec122[1]))))))) - (fRec94[1] + (fRec102[1] + (fRec106[1] + (fRec114[1] + (fRec95 + (fRec107 + fTemp46)))))));
			fRec93[0] = ((fRec94[1] + (fRec98[1] + (fRec106[1] + (fRec118[1] + (fRec95 + (fRec107 + (fRec119 + fRec99))))))) - (fRec102[1] + (fRec110[1] + (fRec114[1] + (fRec123 + (fRec111 + (fRec122[1] + fTemp46)))))));
			float fTemp47 = ((fTemp33 * (1.0f - fRec85[0])) + (0.370000005f * (fRec85[0] * (fRec87[0] + fRec88[0]))));
			fRec126[0] = (fSlow243 + (0.999899983f * fRec126[1]));
			float fTemp48 = float(input1[i]);
			fVec51[(IOTA & 16383)] = fTemp48;
			fRec127[0] = (fSlow244 + (0.999000013f * fRec127[1]));
			fRec139[0] = (0.0f - (fConst5 * ((fConst6 * fRec139[1]) - (fRec129[1] + fRec129[2]))));
			fRec138[0] = ((fSlow252 * fRec138[1]) + (fSlow253 * (fRec129[1] + (fSlow255 * fRec139[0]))));
			fVec52[(IOTA & 65535)] = ((0.353553385f * fRec138[0]) + 9.99999968e-21f);
			float fTemp49 = ((0.600000024f * fRec136[1]) + fVec52[((IOTA - iConst29) & 65535)]);
			fVec53[(IOTA & 8191)] = fTemp49;
			fRec136[0] = fVec53[((IOTA - iConst30) & 8191)];
			float fRec137 = (0.0f - (0.600000024f * fTemp49));
			fRec143[0] = (0.0f - (fConst5 * ((fConst6 * fRec143[1]) - (fRec134[1] + fRec134[2]))));
			fRec142[0] = ((fSlow262 * fRec142[1]) + (fSlow263 * (fRec134[1] + (fSlow264 * fRec143[0]))));
			fVec54[(IOTA & 32767)] = ((0.353553385f * fRec142[0]) + 9.99999968e-21f);
			float fTemp50 = (0.300000012f * fVec51[((IOTA - iSlow265) & 16383)]);
			float fTemp51 = (fVec54[((IOTA - iConst34) & 32767)] - (fTemp50 + (0.600000024f * fRec140[1])));
			fVec55[(IOTA & 4095)] = fTemp51;
			fRec140[0] = fVec55[((IOTA - iConst35) & 4095)];
			float fRec141 = (0.600000024f * fTemp51);
			fRec147[0] = (0.0f - (fConst5 * ((fConst6 * fRec147[1]) - (fRec130[1] + fRec130[2]))));
			fRec146[0] = ((fSlow272 * fRec146[1]) + (fSlow273 * (fRec130[1] + (fSlow274 * fRec147[0]))));
			fVec56[(IOTA & 32767)] = ((0.353553385f * fRec146[0]) + 9.99999968e-21f);
			float fTemp52 = (fVec56[((IOTA - iConst13) & 32767)] - (fTemp50 + (0.600000024f * fRec144[1])));
			fVec57[(IOTA & 8191)] = fTemp52;
			fRec144[0] = fVec57[((IOTA - iConst15) & 8191)];
			float fRec145 = (0.600000024f * fTemp52);
			fRec151[0] = (0.0f - (fConst5 * ((fConst6 * fRec151[1]) - (fRec132[1] + fRec132[2]))));
			fRec150[0] = ((fSlow281 * fRec150[1]) + (fSlow282 * (fRec132[1] + (fSlow283 * fRec151[0]))));
			fVec58[(IOTA & 32767)] = ((0.353553385f * fRec150[0]) + 9.99999968e-21f);
			float fTemp53 = ((fTemp50 + fVec58[((IOTA - iConst39) & 32767)]) - (0.600000024f * fRec148[1]));
			fVec59[(IOTA & 8191)] = fTemp53;
			fRec148[0] = fVec59[((IOTA - iConst40) & 8191)];
			float fRec149 = (0.600000024f * fTemp53);
			fRec155[0] = (0.0f - (fConst5 * ((fConst6 * fRec155[1]) - (fRec135[1] + fRec135[2]))));
			fRec154[0] = ((fSlow290 * fRec154[1]) + (fSlow291 * (fRec135[1] + (fSlow292 * fRec155[0]))));
			fVec60[(IOTA & 65535)] = ((0.353553385f * fRec154[0]) + 9.99999968e-21f);
			float fTemp54 = ((0.600000024f * fRec152[1]) + fVec60[((IOTA - iConst8) & 65535)]);
			fVec61[(IOTA & 4095)] = fTemp54;
			fRec152[0] = fVec61[((IOTA - iConst9) & 4095)];
			float fRec153 = (0.0f - (0.600000024f * fTemp54));
			fRec159[0] = (0.0f - (fConst5 * ((fConst6 * fRec159[1]) - (fRec131[1] + fRec131[2]))));
			fRec158[0] = ((fSlow299 * fRec158[1]) + (fSlow300 * (fRec131[1] + (fSlow301 * fRec159[0]))));
			fVec62[(IOTA & 65535)] = ((0.353553385f * fRec158[0]) + 9.99999968e-21f);
			float fTemp55 = ((0.600000024f * fRec156[1]) + fVec62[((IOTA - iConst19) & 65535)]);
			fVec63[(IOTA & 8191)] = fTemp55;
			fRec156[0] = fVec63[((IOTA - iConst20) & 8191)];
			float fRec157 = (0.0f - (0.600000024f * fTemp55));
			fRec163[0] = (0.0f - (fConst5 * ((fConst6 * fRec163[1]) - (fRec133[1] + fRec133[2]))));
			fRec162[0] = ((fSlow308 * fRec162[1]) + (fSlow309 * (fRec133[1] + (fSlow310 * fRec163[0]))));
			fVec64[(IOTA & 32767)] = ((0.353553385f * fRec162[0]) + 9.99999968e-21f);
			float fTemp56 = ((0.600000024f * fRec160[1]) + fVec64[((IOTA - iConst24) & 32767)]);
			fVec65[(IOTA & 8191)] = fTemp56;
			fRec160[0] = fVec65[((IOTA - iConst25) & 8191)];
			float fRec161 = (0.0f - (0.600000024f * fTemp56));
			fRec167[0] = (0.0f - (fConst5 * ((fConst6 * fRec167[1]) - (fRec128[1] + fRec128[2]))));
			fRec166[0] = ((fSlow317 * fRec166[1]) + (fSlow318 * (fRec128[1] + (fSlow319 * fRec167[0]))));
			fVec66[(IOTA & 32767)] = ((0.353553385f * fRec166[0]) + 9.99999968e-21f);
			float fTemp57 = ((fVec66[((IOTA - iConst44) & 32767)] + fTemp50) - (0.600000024f * fRec164[1]));
			fVec67[(IOTA & 4095)] = fTemp57;
			fRec164[0] = fVec67[((IOTA - iConst45) & 4095)];
			float fRec165 = (0.600000024f * fTemp57);
			float fTemp58 = (fRec165 + fRec149);
			float fTemp59 = (fRec141 + fTemp58);
			fRec128[0] = (fRec136[1] + (fRec140[1] + (fRec144[1] + (fRec148[1] + (fRec152[1] + (fRec156[1] + (fRec160[1] + (fRec164[1] + (fRec161 + (fRec145 + (fRec153 + (fRec157 + (fRec137 + fTemp59)))))))))))));
			fRec129[0] = ((fRec140[1] + (fRec144[1] + (fRec148[1] + (fRec164[1] + (fRec145 + fTemp59))))) - (fRec136[1] + (fRec152[1] + (fRec156[1] + (fRec160[1] + (fRec161 + (fRec153 + (fRec137 + fRec157))))))));
			fRec130[0] = ((fRec136[1] + (fRec148[1] + (fRec160[1] + (fRec164[1] + (fRec161 + (fRec137 + fTemp58)))))) - (fRec140[1] + (fRec144[1] + (fRec152[1] + (fRec156[1] + (fRec145 + (fRec153 + (fRec141 + fRec157))))))));
			fRec131[0] = ((fRec148[1] + (fRec152[1] + (fRec156[1] + (fRec164[1] + (fRec153 + (fRec157 + fTemp58)))))) - (fRec136[1] + (fRec140[1] + (fRec144[1] + (fRec160[1] + (fRec161 + (fRec145 + (fRec141 + fRec137))))))));
			float fTemp60 = (fRec149 + fRec141);
			fRec132[0] = ((fRec136[1] + (fRec144[1] + (fRec156[1] + (fRec164[1] + (fRec145 + (fRec157 + (fRec165 + fRec137))))))) - (fRec140[1] + (fRec148[1] + (fRec152[1] + (fRec160[1] + (fRec161 + (fRec153 + fTemp60)))))));
			fRec133[0] = ((fRec144[1] + (fRec152[1] + (fRec160[1] + (fRec164[1] + (fRec161 + (fRec145 + (fRec165 + fRec153))))))) - (fRec136[1] + (fRec140[1] + (fRec148[1] + (fRec156[1] + (fRec157 + (fRec137 + fTemp60)))))));
			float fTemp61 = (fRec165 + fRec141);
			fRec134[0] = ((fRec136[1] + (fRec140[1] + (fRec152[1] + (fRec164[1] + (fRec153 + (fRec137 + fTemp61)))))) - (fRec144[1] + (fRec148[1] + (fRec156[1] + (fRec160[1] + (fRec161 + (fRec145 + (fRec149 + fRec157))))))));
			fRec135[0] = ((fRec140[1] + (fRec156[1] + (fRec160[1] + (fRec164[1] + (fRec161 + (fRec157 + fTemp61)))))) - (fRec136[1] + (fRec144[1] + (fRec148[1] + (fRec152[1] + (fRec145 + (fRec153 + (fRec149 + fRec137))))))));
			float fTemp62 = ((fTemp48 * (1.0f - fRec127[0])) + (0.370000005f * (fRec127[0] * (fRec129[0] + fRec130[0]))));
			fRec168[0] = (fSlow323 + (0.999899983f * fRec168[1]));
			float fTemp63 = float(input3[i]);
			fVec68[(IOTA & 16383)] = fTemp63;
			fRec169[0] = (fSlow324 + (0.999000013f * fRec169[1]));
			fRec181[0] = (0.0f - (fConst5 * ((fConst6 * fRec181[1]) - (fRec177[1] + fRec177[2]))));
			fRec180[0] = ((fSlow332 * fRec180[1]) + (fSlow333 * (fRec177[1] + (fSlow335 * fRec181[0]))));
			fVec69[(IOTA & 65535)] = ((0.353553385f * fRec180[0]) + 9.99999968e-21f);
			float fTemp64 = ((0.600000024f * fRec178[1]) + fVec69[((IOTA - iConst8) & 65535)]);
			fVec70[(IOTA & 4095)] = fTemp64;
			fRec178[0] = fVec70[((IOTA - iConst9) & 4095)];
			float fRec179 = (0.0f - (0.600000024f * fTemp64));
			fRec185[0] = (0.0f - (fConst5 * ((fConst6 * fRec185[1]) - (fRec170[1] + fRec170[2]))));
			fRec184[0] = ((fSlow342 * fRec184[1]) + (fSlow343 * (fRec170[1] + (fSlow344 * fRec185[0]))));
			fVec71[(IOTA & 32767)] = ((0.353553385f * fRec184[0]) + 9.99999968e-21f);
			float fTemp65 = (0.300000012f * fVec68[((IOTA - iSlow345) & 16383)]);
			float fTemp66 = ((fVec71[((IOTA - iConst44) & 32767)] + fTemp65) - (0.600000024f * fRec182[1]));
			fVec72[(IOTA & 4095)] = fTemp66;
			fRec182[0] = fVec72[((IOTA - iConst45) & 4095)];
			float fRec183 = (0.600000024f * fTemp66);
			fRec189[0] = (0.0f - (fConst5 * ((fConst6 * fRec189[1]) - (fRec173[1] + fRec173[2]))));
			fRec188[0] = ((fSlow352 * fRec188[1]) + (fSlow353 * (fRec173[1] + (fSlow354 * fRec189[0]))));
			fVec73[(IOTA & 65535)] = ((0.353553385f * fRec188[0]) + 9.99999968e-21f);
			float fTemp67 = ((0.600000024f * fRec186[1]) + fVec73[((IOTA - iConst19) & 65535)]);
			fVec74[(IOTA & 8191)] = fTemp67;
			fRec186[0] = fVec74[((IOTA - iConst20) & 8191)];
			float fRec187 = (0.0f - (0.600000024f * fTemp67));
			fRec193[0] = (0.0f - (fConst5 * ((fConst6 * fRec193[1]) - (fRec175[1] + fRec175[2]))));
			fRec192[0] = ((fSlow361 * fRec192[1]) + (fSlow362 * (fRec175[1] + (fSlow363 * fRec193[0]))));
			fVec75[(IOTA & 32767)] = ((0.353553385f * fRec192[0]) + 9.99999968e-21f);
			float fTemp68 = ((0.600000024f * fRec190[1]) + fVec75[((IOTA - iConst24) & 32767)]);
			fVec76[(IOTA & 8191)] = fTemp68;
			fRec190[0] = fVec76[((IOTA - iConst25) & 8191)];
			float fRec191 = (0.0f - (0.600000024f * fTemp68));
			fRec197[0] = (0.0f - (fConst5 * ((fConst6 * fRec197[1]) - (fRec171[1] + fRec171[2]))));
			fRec196[0] = ((fSlow370 * fRec196[1]) + (fSlow371 * (fRec171[1] + (fSlow372 * fRec197[0]))));
			fVec77[(IOTA & 65535)] = ((0.353553385f * fRec196[0]) + 9.99999968e-21f);
			float fTemp69 = ((0.600000024f * fRec194[1]) + fVec77[((IOTA - iConst29) & 65535)]);
			fVec78[(IOTA & 8191)] = fTemp69;
			fRec194[0] = fVec78[((IOTA - iConst30) & 8191)];
			float fRec195 = (0.0f - (0.600000024f * fTemp69));
			fRec201[0] = (0.0f - (fConst5 * ((fConst6 * fRec201[1]) - (fRec176[1] + fRec176[2]))));
			fRec200[0] = ((fSlow379 * fRec200[1]) + (fSlow380 * (fRec176[1] + (fSlow381 * fRec201[0]))));
			fVec79[(IOTA & 32767)] = ((0.353553385f * fRec200[0]) + 9.99999968e-21f);
			float fTemp70 = (fVec79[((IOTA - iConst34) & 32767)] - (fTemp65 + (0.600000024f * fRec198[1])));
			fVec80[(IOTA & 4095)] = fTemp70;
			fRec198[0] = fVec80[((IOTA - iConst35) & 4095)];
			float fRec199 = (0.600000024f * fTemp70);
			fRec205[0] = (0.0f - (fConst5 * ((fConst6 * fRec205[1]) - (fRec172[1] + fRec172[2]))));
			fRec204[0] = ((fSlow388 * fRec204[1]) + (fSlow389 * (fRec172[1] + (fSlow390 * fRec205[0]))));
			fVec81[(IOTA & 32767)] = ((0.353553385f * fRec204[0]) + 9.99999968e-21f);
			float fTemp71 = (fVec81[((IOTA - iConst13) & 32767)] - (fTemp65 + (0.600000024f * fRec202[1])));
			fVec82[(IOTA & 8191)] = fTemp71;
			fRec202[0] = fVec82[((IOTA - iConst15) & 8191)];
			float fRec203 = (0.600000024f * fTemp71);
			fRec209[0] = (0.0f - (fConst5 * ((fConst6 * fRec209[1]) - (fRec174[1] + fRec174[2]))));
			fRec208[0] = ((fSlow397 * fRec208[1]) + (fSlow398 * (fRec174[1] + (fSlow399 * fRec209[0]))));
			fVec83[(IOTA & 32767)] = ((0.353553385f * fRec208[0]) + 9.99999968e-21f);
			float fTemp72 = ((fTemp65 + fVec83[((IOTA - iConst39) & 32767)]) - (0.600000024f * fRec206[1]));
			fVec84[(IOTA & 8191)] = fTemp72;
			fRec206[0] = fVec84[((IOTA - iConst40) & 8191)];
			float fRec207 = (0.600000024f * fTemp72);
			float fTemp73 = (fRec206[1] + fRec202[1]);
			float fTemp74 = (fRec198[1] + fTemp73);
			fRec170[0] = (fRec178[1] + (fRec182[1] + (fRec186[1] + (fRec179 + (fRec187 + (fRec191 + (fRec195 + (fRec199 + (fRec203 + (fRec207 + (fRec183 + (fRec190[1] + (fTemp74 + fRec194[1])))))))))))));
			fRec171[0] = ((fRec182[1] + (fRec199 + (fRec203 + (fRec207 + (fRec183 + fTemp74))))) - (fRec178[1] + (fRec186[1] + (fRec179 + (fRec187 + (fRec191 + (fRec195 + (fRec194[1] + fRec190[1]))))))));
			float fTemp75 = (fRec202[1] + fRec198[1]);
			fRec172[0] = ((fRec182[1] + (fRec191 + (fRec195 + (fRec207 + (fRec183 + (fRec190[1] + (fRec206[1] + fRec194[1]))))))) - (fRec178[1] + (fRec186[1] + (fRec179 + (fRec187 + (fRec199 + (fRec203 + fTemp75)))))));
			fRec173[0] = ((fRec178[1] + (fRec182[1] + (fRec186[1] + (fRec179 + (fRec187 + (fRec207 + (fRec183 + fRec206[1]))))))) - (fRec191 + (fRec195 + (fRec199 + (fRec203 + (fRec190[1] + (fTemp75 + fRec194[1])))))));
			float fTemp76 = (fRec206[1] + fRec198[1]);
			fRec174[0] = ((fRec182[1] + (fRec186[1] + (fRec187 + (fRec195 + (fRec203 + (fRec183 + (fRec202[1] + fRec194[1]))))))) - (fRec178[1] + (fRec179 + (fRec191 + (fRec199 + (fRec207 + (fRec190[1] + fTemp76)))))));
			fRec175[0] = ((fRec178[1] + (fRec182[1] + (fRec179 + (fRec191 + (fRec203 + (fRec183 + (fRec202[1] + fRec190[1]))))))) - (fRec186[1] + (fRec187 + (fRec195 + (fRec199 + (fRec207 + (fRec194[1] + fTemp76)))))));
			fRec176[0] = ((fRec178[1] + (fRec182[1] + (fRec179 + (fRec195 + (fRec199 + (fRec183 + (fRec198[1] + fRec194[1]))))))) - (fRec186[1] + (fRec187 + (fRec191 + (fRec203 + (fRec207 + (fTemp73 + fRec190[1])))))));
			fRec177[0] = ((fRec182[1] + (fRec186[1] + (fRec187 + (fRec191 + (fRec199 + (fRec183 + (fRec198[1] + fRec190[1]))))))) - (fRec178[1] + (fRec179 + (fRec195 + (fRec203 + (fRec207 + (fTemp73 + fRec194[1])))))));
			float fTemp77 = ((fTemp63 * (1.0f - fRec169[0])) + (0.370000005f * (fRec169[0] * (fRec171[0] + fRec172[0]))));
			fRec210[0] = (fSlow403 + (0.999899983f * fRec210[1]));
			float fTemp78 = float(input5[i]);
			fVec85[(IOTA & 16383)] = fTemp78;
			fRec211[0] = (fSlow404 + (0.999000013f * fRec211[1]));
			fRec223[0] = (0.0f - (fConst5 * ((fConst6 * fRec223[1]) - (fRec219[1] + fRec219[2]))));
			fRec222[0] = ((fSlow412 * fRec222[1]) + (fSlow413 * (fRec219[1] + (fSlow415 * fRec223[0]))));
			fVec86[(IOTA & 65535)] = ((0.353553385f * fRec222[0]) + 9.99999968e-21f);
			float fTemp79 = ((0.600000024f * fRec220[1]) + fVec86[((IOTA - iConst8) & 65535)]);
			fVec87[(IOTA & 4095)] = fTemp79;
			fRec220[0] = fVec87[((IOTA - iConst9) & 4095)];
			float fRec221 = (0.0f - (0.600000024f * fTemp79));
			fRec227[0] = (0.0f - (fConst5 * ((fConst6 * fRec227[1]) - (fRec215[1] + fRec215[2]))));
			fRec226[0] = ((fSlow422 * fRec226[1]) + (fSlow423 * (fRec215[1] + (fSlow424 * fRec227[0]))));
			fVec88[(IOTA & 65535)] = ((0.353553385f * fRec226[0]) + 9.99999968e-21f);
			float fTemp80 = ((0.600000024f * fRec224[1]) + fVec88[((IOTA - iConst19) & 65535)]);
			fVec89[(IOTA & 8191)] = fTemp80;
			fRec224[0] = fVec89[((IOTA - iConst20) & 8191)];
			float fRec225 = (0.0f - (0.600000024f * fTemp80));
			fRec231[0] = (0.0f - (fConst5 * ((fConst6 * fRec231[1]) - (fRec217[1] + fRec217[2]))));
			fRec230[0] = ((fSlow431 * fRec230[1]) + (fSlow432 * (fRec217[1] + (fSlow433 * fRec231[0]))));
			fVec90[(IOTA & 32767)] = ((0.353553385f * fRec230[0]) + 9.99999968e-21f);
			float fTemp81 = ((0.600000024f * fRec228[1]) + fVec90[((IOTA - iConst24) & 32767)]);
			fVec91[(IOTA & 8191)] = fTemp81;
			fRec228[0] = fVec91[((IOTA - iConst25) & 8191)];
			float fRec229 = (0.0f - (0.600000024f * fTemp81));
			fRec235[0] = (0.0f - (fConst5 * ((fConst6 * fRec235[1]) - (fRec213[1] + fRec213[2]))));
			fRec234[0] = ((fSlow440 * fRec234[1]) + (fSlow441 * (fRec213[1] + (fSlow442 * fRec235[0]))));
			fVec92[(IOTA & 65535)] = ((0.353553385f * fRec234[0]) + 9.99999968e-21f);
			float fTemp82 = ((0.600000024f * fRec232[1]) + fVec92[((IOTA - iConst29) & 65535)]);
			fVec93[(IOTA & 8191)] = fTemp82;
			fRec232[0] = fVec93[((IOTA - iConst30) & 8191)];
			float fRec233 = (0.0f - (0.600000024f * fTemp82));
			fRec239[0] = (0.0f - (fConst5 * ((fConst6 * fRec239[1]) - (fRec218[1] + fRec218[2]))));
			fRec238[0] = ((fSlow449 * fRec238[1]) + (fSlow450 * (fRec218[1] + (fSlow451 * fRec239[0]))));
			fVec94[(IOTA & 32767)] = ((0.353553385f * fRec238[0]) + 9.99999968e-21f);
			float fTemp83 = (0.300000012f * fVec85[((IOTA - iSlow452) & 16383)]);
			float fTemp84 = (fVec94[((IOTA - iConst34) & 32767)] - (fTemp83 + (0.600000024f * fRec236[1])));
			fVec95[(IOTA & 4095)] = fTemp84;
			fRec236[0] = fVec95[((IOTA - iConst35) & 4095)];
			float fRec237 = (0.600000024f * fTemp84);
			fRec243[0] = (0.0f - (fConst5 * ((fConst6 * fRec243[1]) - (fRec214[1] + fRec214[2]))));
			fRec242[0] = ((fSlow459 * fRec242[1]) + (fSlow460 * (fRec214[1] + (fSlow461 * fRec243[0]))));
			fVec96[(IOTA & 32767)] = ((0.353553385f * fRec242[0]) + 9.99999968e-21f);
			float fTemp85 = (fVec96[((IOTA - iConst13) & 32767)] - (fTemp83 + (0.600000024f * fRec240[1])));
			fVec97[(IOTA & 8191)] = fTemp85;
			fRec240[0] = fVec97[((IOTA - iConst15) & 8191)];
			float fRec241 = (0.600000024f * fTemp85);
			fRec247[0] = (0.0f - (fConst5 * ((fConst6 * fRec247[1]) - (fRec216[1] + fRec216[2]))));
			fRec246[0] = ((fSlow468 * fRec246[1]) + (fSlow469 * (fRec216[1] + (fSlow470 * fRec247[0]))));
			fVec98[(IOTA & 32767)] = ((0.353553385f * fRec246[0]) + 9.99999968e-21f);
			float fTemp86 = ((fTemp83 + fVec98[((IOTA - iConst39) & 32767)]) - (0.600000024f * fRec244[1]));
			fVec99[(IOTA & 8191)] = fTemp86;
			fRec244[0] = fVec99[((IOTA - iConst40) & 8191)];
			float fRec245 = (0.600000024f * fTemp86);
			fRec251[0] = (0.0f - (fConst5 * ((fConst6 * fRec251[1]) - (fRec212[1] + fRec212[2]))));
			fRec250[0] = ((fSlow477 * fRec250[1]) + (fSlow478 * (fRec212[1] + (fSlow479 * fRec251[0]))));
			fVec100[(IOTA & 32767)] = ((0.353553385f * fRec250[0]) + 9.99999968e-21f);
			float fTemp87 = ((fVec100[((IOTA - iConst44) & 32767)] + fTemp83) - (0.600000024f * fRec248[1]));
			fVec101[(IOTA & 4095)] = fTemp87;
			fRec248[0] = fVec101[((IOTA - iConst45) & 4095)];
			float fRec249 = (0.600000024f * fTemp87);
			float fTemp88 = (fRec249 + fRec245);
			float fTemp89 = (fRec237 + (fRec241 + fTemp88));
			fRec212[0] = (fRec220[1] + (fRec224[1] + (fRec228[1] + (fRec232[1] + (fRec236[1] + (fRec240[1] + (fRec244[1] + (fRec248[1] + (fRec221 + (fRec225 + (fRec229 + (fRec233 + fTemp89))))))))))));
			fRec213[0] = ((fRec236[1] + (fRec240[1] + (fRec244[1] + (fRec248[1] + fTemp89)))) - (fRec220[1] + (fRec224[1] + (fRec228[1] + (fRec232[1] + (fRec221 + (fRec225 + (fRec233 + fRec229))))))));
			float fTemp90 = (fRec241 + fRec237);
			fRec214[0] = ((fRec228[1] + (fRec232[1] + (fRec244[1] + (fRec248[1] + (fRec229 + (fRec233 + fTemp88)))))) - (fRec220[1] + (fRec224[1] + (fRec236[1] + (fRec240[1] + (fRec221 + (fRec225 + fTemp90)))))));
			fRec215[0] = ((fRec220[1] + (fRec224[1] + (fRec244[1] + (fRec248[1] + (fRec221 + (fRec225 + fTemp88)))))) - (fRec228[1] + (fRec232[1] + (fRec236[1] + (fRec240[1] + (fRec229 + (fRec233 + fTemp90)))))));
			float fTemp91 = (fRec249 + fRec241);
			float fTemp92 = (fRec245 + fRec237);
			fRec216[0] = ((fRec224[1] + (fRec232[1] + (fRec240[1] + (fRec248[1] + (fRec225 + (fRec233 + fTemp91)))))) - (fRec220[1] + (fRec228[1] + (fRec236[1] + (fRec244[1] + (fRec221 + (fRec229 + fTemp92)))))));
			fRec217[0] = ((fRec220[1] + (fRec228[1] + (fRec240[1] + (fRec248[1] + (fRec221 + (fRec229 + fTemp91)))))) - (fRec224[1] + (fRec232[1] + (fRec236[1] + (fRec244[1] + (fRec225 + (fRec233 + fTemp92)))))));
			float fTemp93 = (fRec249 + fRec237);
			float fTemp94 = (fRec245 + fRec241);
			fRec218[0] = ((fRec220[1] + (fRec232[1] + (fRec236[1] + (fRec248[1] + (fRec221 + (fRec233 + fTemp93)))))) - (fRec224[1] + (fRec228[1] + (fRec240[1] + (fRec244[1] + (fRec225 + (fRec229 + fTemp94)))))));
			fRec219[0] = ((fRec224[1] + (fRec228[1] + (fRec236[1] + (fRec248[1] + (fRec225 + (fRec229 + fTemp93)))))) - (fRec220[1] + (fRec232[1] + (fRec240[1] + (fRec244[1] + (fRec221 + (fRec233 + fTemp94)))))));
			float fTemp95 = ((fTemp78 * (1.0f - fRec211[0])) + (0.370000005f * (fRec211[0] * (fRec213[0] + fRec214[0]))));
			output0[i] = FAUSTFLOAT(((fRec0[0] * fTemp14) + ((fRec42[0] * fTemp32) + (((fRec84[0] * fTemp47) + ((fRec126[0] * fTemp62) + (fRec168[0] * fTemp77))) + (fRec210[0] * fTemp95)))));
			fRec252[0] = (fSlow480 + (0.999899983f * fRec252[1]));
			fRec253[0] = (fSlow481 + (0.999899983f * fRec253[1]));
			fRec254[0] = (fSlow482 + (0.999899983f * fRec254[1]));
			fRec255[0] = (fSlow483 + (0.999899983f * fRec255[1]));
			fRec256[0] = (fSlow484 + (0.999899983f * fRec256[1]));
			fRec257[0] = (fSlow485 + (0.999899983f * fRec257[1]));
			output1[i] = FAUSTFLOAT(((((((fRec252[0] * fTemp47) + (fRec253[0] * fTemp62)) + (fRec254[0] * fTemp32)) + (fRec255[0] * fTemp77)) + (fRec256[0] * fTemp14)) + (fRec257[0] * fTemp95)));
			fRec258[0] = (fSlow486 + (0.999899983f * fRec258[1]));
			fRec259[0] = (fSlow487 + (0.999899983f * fRec259[1]));
			fRec260[0] = (fSlow488 + (0.999899983f * fRec260[1]));
			fRec261[0] = (fSlow489 + (0.999899983f * fRec261[1]));
			fRec262[0] = (fSlow490 + (0.999899983f * fRec262[1]));
			fRec263[0] = (fSlow491 + (0.999899983f * fRec263[1]));
			output2[i] = FAUSTFLOAT(((fRec258[0] * fTemp62) + ((fRec259[0] * fTemp47) + ((((fRec260[0] * fTemp32) + (fRec261[0] * fTemp77)) + (fRec262[0] * fTemp14)) + (fRec263[0] * fTemp95)))));
			fRec264[0] = (fSlow492 + (0.999899983f * fRec264[1]));
			fRec265[0] = (fSlow493 + (0.999899983f * fRec265[1]));
			fRec266[0] = (fSlow494 + (0.999899983f * fRec266[1]));
			fRec267[0] = (fSlow495 + (0.999899983f * fRec267[1]));
			fRec268[0] = (fSlow496 + (0.999899983f * fRec268[1]));
			fRec269[0] = (fSlow497 + (0.999899983f * fRec269[1]));
			output3[i] = FAUSTFLOAT(((((((fRec264[0] * fTemp47) + (fRec265[0] * fTemp62)) + (fRec266[0] * fTemp32)) + (fRec267[0] * fTemp77)) + (fRec268[0] * fTemp14)) + (fRec269[0] * fTemp95)));
			fRec270[0] = (fSlow498 + (0.999899983f * fRec270[1]));
			fRec271[0] = (fSlow499 + (0.999899983f * fRec271[1]));
			fRec272[0] = (fSlow500 + (0.999899983f * fRec272[1]));
			fRec273[0] = (fSlow501 + (0.999899983f * fRec273[1]));
			fRec274[0] = (fSlow502 + (0.999899983f * fRec274[1]));
			fRec275[0] = (fSlow503 + (0.999899983f * fRec275[1]));
			output4[i] = FAUSTFLOAT(((((((fRec270[0] * fTemp47) + (fRec271[0] * fTemp62)) + (fRec272[0] * fTemp32)) + (fRec273[0] * fTemp77)) + (fRec274[0] * fTemp14)) + (fRec275[0] * fTemp95)));
			fRec276[0] = (fSlow504 + (0.999899983f * fRec276[1]));
			fRec277[0] = (fSlow505 + (0.999899983f * fRec277[1]));
			fRec278[0] = (fSlow506 + (0.999899983f * fRec278[1]));
			fRec279[0] = (fSlow507 + (0.999899983f * fRec279[1]));
			fRec280[0] = (fSlow508 + (0.999899983f * fRec280[1]));
			fRec281[0] = (fSlow509 + (0.999899983f * fRec281[1]));
			output5[i] = FAUSTFLOAT(((((((fRec276[0] * fTemp47) + (fRec277[0] * fTemp62)) + (fRec278[0] * fTemp32)) + (fRec279[0] * fTemp77)) + (fRec280[0] * fTemp14)) + (fRec281[0] * fTemp95)));
			fRec282[0] = (fSlow510 + (0.999899983f * fRec282[1]));
			fRec283[0] = (fSlow511 + (0.999899983f * fRec283[1]));
			fRec284[0] = (fSlow512 + (0.999899983f * fRec284[1]));
			fRec285[0] = (fSlow513 + (0.999899983f * fRec285[1]));
			fRec286[0] = (fSlow514 + (0.999899983f * fRec286[1]));
			fRec287[0] = (fSlow515 + (0.999899983f * fRec287[1]));
			output6[i] = FAUSTFLOAT(((((((fRec282[0] * fTemp47) + (fRec283[0] * fTemp62)) + (fRec284[0] * fTemp32)) + (fRec285[0] * fTemp77)) + (fRec286[0] * fTemp14)) + (fRec287[0] * fTemp95)));
			fRec288[0] = (fSlow516 + (0.999899983f * fRec288[1]));
			fRec289[0] = (fSlow517 + (0.999899983f * fRec289[1]));
			fRec290[0] = (fSlow518 + (0.999899983f * fRec290[1]));
			fRec291[0] = (fSlow519 + (0.999899983f * fRec291[1]));
			fRec292[0] = (fSlow520 + (0.999899983f * fRec292[1]));
			fRec293[0] = (fSlow521 + (0.999899983f * fRec293[1]));
			output7[i] = FAUSTFLOAT(((((((fRec288[0] * fTemp47) + (fRec289[0] * fTemp62)) + (fRec290[0] * fTemp32)) + (fRec291[0] * fTemp77)) + (fRec292[0] * fTemp14)) + (fRec293[0] * fTemp95)));
			fRec0[1] = fRec0[0];
			IOTA = (IOTA + 1);
			fRec1[1] = fRec1[0];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fRec10[1] = fRec10[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec14[1] = fRec14[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fRec18[1] = fRec18[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fRec22[1] = fRec22[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fRec26[1] = fRec26[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fRec30[1] = fRec30[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec34[1] = fRec34[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fRec38[1] = fRec38[0];
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
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec52[1] = fRec52[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fRec56[1] = fRec56[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fRec60[1] = fRec60[0];
			fRec67[1] = fRec67[0];
			fRec66[1] = fRec66[0];
			fRec64[1] = fRec64[0];
			fRec71[1] = fRec71[0];
			fRec70[1] = fRec70[0];
			fRec68[1] = fRec68[0];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec72[1] = fRec72[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fRec76[1] = fRec76[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fRec80[1] = fRec80[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec51[2] = fRec51[1];
			fRec51[1] = fRec51[0];
			fRec84[1] = fRec84[0];
			fRec85[1] = fRec85[0];
			fRec97[1] = fRec97[0];
			fRec96[1] = fRec96[0];
			fRec94[1] = fRec94[0];
			fRec101[1] = fRec101[0];
			fRec100[1] = fRec100[0];
			fRec98[1] = fRec98[0];
			fRec105[1] = fRec105[0];
			fRec104[1] = fRec104[0];
			fRec102[1] = fRec102[0];
			fRec109[1] = fRec109[0];
			fRec108[1] = fRec108[0];
			fRec106[1] = fRec106[0];
			fRec113[1] = fRec113[0];
			fRec112[1] = fRec112[0];
			fRec110[1] = fRec110[0];
			fRec117[1] = fRec117[0];
			fRec116[1] = fRec116[0];
			fRec114[1] = fRec114[0];
			fRec121[1] = fRec121[0];
			fRec120[1] = fRec120[0];
			fRec118[1] = fRec118[0];
			fRec125[1] = fRec125[0];
			fRec124[1] = fRec124[0];
			fRec122[1] = fRec122[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec88[2] = fRec88[1];
			fRec88[1] = fRec88[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec126[1] = fRec126[0];
			fRec127[1] = fRec127[0];
			fRec139[1] = fRec139[0];
			fRec138[1] = fRec138[0];
			fRec136[1] = fRec136[0];
			fRec143[1] = fRec143[0];
			fRec142[1] = fRec142[0];
			fRec140[1] = fRec140[0];
			fRec147[1] = fRec147[0];
			fRec146[1] = fRec146[0];
			fRec144[1] = fRec144[0];
			fRec151[1] = fRec151[0];
			fRec150[1] = fRec150[0];
			fRec148[1] = fRec148[0];
			fRec155[1] = fRec155[0];
			fRec154[1] = fRec154[0];
			fRec152[1] = fRec152[0];
			fRec159[1] = fRec159[0];
			fRec158[1] = fRec158[0];
			fRec156[1] = fRec156[0];
			fRec163[1] = fRec163[0];
			fRec162[1] = fRec162[0];
			fRec160[1] = fRec160[0];
			fRec167[1] = fRec167[0];
			fRec166[1] = fRec166[0];
			fRec164[1] = fRec164[0];
			fRec128[2] = fRec128[1];
			fRec128[1] = fRec128[0];
			fRec129[2] = fRec129[1];
			fRec129[1] = fRec129[0];
			fRec130[2] = fRec130[1];
			fRec130[1] = fRec130[0];
			fRec131[2] = fRec131[1];
			fRec131[1] = fRec131[0];
			fRec132[2] = fRec132[1];
			fRec132[1] = fRec132[0];
			fRec133[2] = fRec133[1];
			fRec133[1] = fRec133[0];
			fRec134[2] = fRec134[1];
			fRec134[1] = fRec134[0];
			fRec135[2] = fRec135[1];
			fRec135[1] = fRec135[0];
			fRec168[1] = fRec168[0];
			fRec169[1] = fRec169[0];
			fRec181[1] = fRec181[0];
			fRec180[1] = fRec180[0];
			fRec178[1] = fRec178[0];
			fRec185[1] = fRec185[0];
			fRec184[1] = fRec184[0];
			fRec182[1] = fRec182[0];
			fRec189[1] = fRec189[0];
			fRec188[1] = fRec188[0];
			fRec186[1] = fRec186[0];
			fRec193[1] = fRec193[0];
			fRec192[1] = fRec192[0];
			fRec190[1] = fRec190[0];
			fRec197[1] = fRec197[0];
			fRec196[1] = fRec196[0];
			fRec194[1] = fRec194[0];
			fRec201[1] = fRec201[0];
			fRec200[1] = fRec200[0];
			fRec198[1] = fRec198[0];
			fRec205[1] = fRec205[0];
			fRec204[1] = fRec204[0];
			fRec202[1] = fRec202[0];
			fRec209[1] = fRec209[0];
			fRec208[1] = fRec208[0];
			fRec206[1] = fRec206[0];
			fRec170[2] = fRec170[1];
			fRec170[1] = fRec170[0];
			fRec171[2] = fRec171[1];
			fRec171[1] = fRec171[0];
			fRec172[2] = fRec172[1];
			fRec172[1] = fRec172[0];
			fRec173[2] = fRec173[1];
			fRec173[1] = fRec173[0];
			fRec174[2] = fRec174[1];
			fRec174[1] = fRec174[0];
			fRec175[2] = fRec175[1];
			fRec175[1] = fRec175[0];
			fRec176[2] = fRec176[1];
			fRec176[1] = fRec176[0];
			fRec177[2] = fRec177[1];
			fRec177[1] = fRec177[0];
			fRec210[1] = fRec210[0];
			fRec211[1] = fRec211[0];
			fRec223[1] = fRec223[0];
			fRec222[1] = fRec222[0];
			fRec220[1] = fRec220[0];
			fRec227[1] = fRec227[0];
			fRec226[1] = fRec226[0];
			fRec224[1] = fRec224[0];
			fRec231[1] = fRec231[0];
			fRec230[1] = fRec230[0];
			fRec228[1] = fRec228[0];
			fRec235[1] = fRec235[0];
			fRec234[1] = fRec234[0];
			fRec232[1] = fRec232[0];
			fRec239[1] = fRec239[0];
			fRec238[1] = fRec238[0];
			fRec236[1] = fRec236[0];
			fRec243[1] = fRec243[0];
			fRec242[1] = fRec242[0];
			fRec240[1] = fRec240[0];
			fRec247[1] = fRec247[0];
			fRec246[1] = fRec246[0];
			fRec244[1] = fRec244[0];
			fRec251[1] = fRec251[0];
			fRec250[1] = fRec250[0];
			fRec248[1] = fRec248[0];
			fRec212[2] = fRec212[1];
			fRec212[1] = fRec212[0];
			fRec213[2] = fRec213[1];
			fRec213[1] = fRec213[0];
			fRec214[2] = fRec214[1];
			fRec214[1] = fRec214[0];
			fRec215[2] = fRec215[1];
			fRec215[1] = fRec215[0];
			fRec216[2] = fRec216[1];
			fRec216[1] = fRec216[0];
			fRec217[2] = fRec217[1];
			fRec217[1] = fRec217[0];
			fRec218[2] = fRec218[1];
			fRec218[1] = fRec218[0];
			fRec219[2] = fRec219[1];
			fRec219[1] = fRec219[0];
			fRec252[1] = fRec252[0];
			fRec253[1] = fRec253[0];
			fRec254[1] = fRec254[0];
			fRec255[1] = fRec255[0];
			fRec256[1] = fRec256[0];
			fRec257[1] = fRec257[0];
			fRec258[1] = fRec258[0];
			fRec259[1] = fRec259[0];
			fRec260[1] = fRec260[0];
			fRec261[1] = fRec261[0];
			fRec262[1] = fRec262[0];
			fRec263[1] = fRec263[0];
			fRec264[1] = fRec264[0];
			fRec265[1] = fRec265[0];
			fRec266[1] = fRec266[0];
			fRec267[1] = fRec267[0];
			fRec268[1] = fRec268[0];
			fRec269[1] = fRec269[0];
			fRec270[1] = fRec270[0];
			fRec271[1] = fRec271[0];
			fRec272[1] = fRec272[0];
			fRec273[1] = fRec273[0];
			fRec274[1] = fRec274[0];
			fRec275[1] = fRec275[0];
			fRec276[1] = fRec276[0];
			fRec277[1] = fRec277[0];
			fRec278[1] = fRec278[0];
			fRec279[1] = fRec279[0];
			fRec280[1] = fRec280[0];
			fRec281[1] = fRec281[0];
			fRec282[1] = fRec282[0];
			fRec283[1] = fRec283[0];
			fRec284[1] = fRec284[0];
			fRec285[1] = fRec285[0];
			fRec286[1] = fRec286[0];
			fRec287[1] = fRec287[0];
			fRec288[1] = fRec288[0];
			fRec289[1] = fRec289[0];
			fRec290[1] = fRec290[0];
			fRec291[1] = fRec291[0];
			fRec292[1] = fRec292[0];
			fRec293[1] = fRec293[0];
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
