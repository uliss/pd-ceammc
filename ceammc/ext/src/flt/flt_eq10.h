/* ------------------------------------------------------------
name: "flt.eq10"
Code generated with Faust 2.28.6 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __flt_eq10_H__
#define  __flt_eq10_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN flt_eq10_dsp.h **************************/
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

class flt_eq10_dsp {

    public:

        flt_eq10_dsp() {}
        virtual ~flt_eq10_dsp() {}

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
        virtual flt_eq10_dsp* clone() = 0;
    
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

class decorator_dsp : public flt_eq10_dsp {

    protected:

        flt_eq10_dsp* fDSP;

    public:

        decorator_dsp(flt_eq10_dsp* flt_eq10_dsp = nullptr):fDSP(flt_eq10_dsp) {}
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
    
        virtual flt_eq10_dsp* createDSPInstance() = 0;
    
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
/**************************  END  flt_eq10_dsp.h **************************/
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
struct flt_eq10 : public flt_eq10_dsp {
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
#include <math.h>

static float flt_eq10_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS flt_eq10
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class flt_eq10 : public flt_eq10_dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	float fConst25;
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fConst38;
	float fConst39;
	float fConst40;
	float fConst41;
	float fConst42;
	float fConst43;
	float fVec0[2];
	float fRec20[2];
	float fConst44;
	float fConst45;
	float fConst46;
	float fConst47;
	float fRec19[3];
	float fConst48;
	float fRec18[3];
	float fVec1[2];
	float fRec17[2];
	float fConst49;
	float fConst50;
	float fConst51;
	float fConst52;
	float fRec16[3];
	float fConst53;
	float fRec15[3];
	float fVec2[2];
	float fRec14[2];
	float fConst54;
	float fConst55;
	float fConst56;
	float fConst57;
	float fRec13[3];
	float fConst58;
	float fRec12[3];
	float fVec3[2];
	float fRec11[2];
	float fConst59;
	float fConst60;
	float fConst61;
	float fConst62;
	float fRec10[3];
	float fConst63;
	float fRec9[3];
	float fVec4[2];
	float fRec8[2];
	float fConst64;
	float fConst65;
	float fConst66;
	float fConst67;
	float fRec7[3];
	float fConst68;
	float fRec6[3];
	float fVec5[2];
	float fConst69;
	float fConst70;
	float fConst71;
	float fRec5[2];
	float fConst72;
	float fConst73;
	float fRec4[3];
	float fConst74;
	float fConst75;
	float fRec3[3];
	float fConst76;
	float fConst77;
	float fConst78;
	float fConst79;
	float fConst80;
	float fConst81;
	float fConst82;
	float fRec2[3];
	float fConst83;
	float fConst84;
	float fConst85;
	float fConst86;
	float fConst87;
	float fConst88;
	float fConst89;
	float fRec1[3];
	float fConst90;
	float fConst91;
	float fConst92;
	float fConst93;
	float fConst94;
	float fConst95;
	float fConst96;
	float fRec0[3];
	FAUSTFLOAT fVslider0;
	float fRec21[2];
	float fConst97;
	float fConst98;
	float fConst99;
	float fRec28[2];
	float fRec27[3];
	float fConst100;
	float fRec26[3];
	float fRec25[3];
	float fRec24[3];
	float fRec23[3];
	float fConst101;
	float fConst102;
	float fRec22[3];
	FAUSTFLOAT fVslider1;
	float fRec29[2];
	float fConst103;
	float fRec40[2];
	float fRec39[3];
	float fConst104;
	float fRec38[3];
	float fRec37[3];
	float fRec36[3];
	float fRec35[3];
	float fRec34[3];
	float fConst105;
	float fConst106;
	float fRec33[3];
	float fConst107;
	float fConst108;
	float fRec32[3];
	float fConst109;
	float fConst110;
	float fRec31[3];
	float fConst111;
	float fConst112;
	float fRec30[3];
	FAUSTFLOAT fVslider2;
	float fRec41[2];
	float fConst113;
	float fConst114;
	float fConst115;
	float fRec51[2];
	float fRec50[3];
	float fConst116;
	float fRec49[3];
	float fRec48[3];
	float fRec47[3];
	float fRec46[3];
	float fRec45[3];
	float fRec44[3];
	float fRec43[3];
	float fRec42[3];
	FAUSTFLOAT fVslider3;
	float fRec52[2];
	float fConst117;
	float fConst118;
	float fConst119;
	float fRec61[2];
	float fRec60[3];
	float fConst120;
	float fRec59[3];
	float fRec58[3];
	float fRec57[3];
	float fRec56[3];
	float fRec55[3];
	float fRec54[3];
	float fRec53[3];
	FAUSTFLOAT fVslider4;
	float fRec62[2];
	float fConst121;
	float fConst122;
	float fConst123;
	float fRec70[2];
	float fRec69[3];
	float fConst124;
	float fRec68[3];
	float fRec67[3];
	float fRec66[3];
	float fRec65[3];
	float fRec64[3];
	float fRec63[3];
	FAUSTFLOAT fVslider5;
	float fRec71[2];
	float fConst125;
	float fConst126;
	float fConst127;
	float fConst128;
	float fConst129;
	float fRec79[2];
	float fRec78[3];
	float fRec77[3];
	float fVec6[2];
	float fConst130;
	float fConst131;
	float fConst132;
	float fRec76[2];
	float fConst133;
	float fRec75[3];
	float fConst134;
	float fConst135;
	float fRec74[3];
	float fRec73[3];
	float fRec72[3];
	FAUSTFLOAT fVslider6;
	float fRec80[2];
	float fConst136;
	float fConst137;
	float fConst138;
	float fConst139;
	float fConst140;
	float fRec87[2];
	float fRec86[3];
	float fRec85[3];
	float fVec7[2];
	float fConst141;
	float fConst142;
	float fConst143;
	float fRec84[2];
	float fConst144;
	float fRec83[3];
	float fConst145;
	float fConst146;
	float fRec82[3];
	float fRec81[3];
	FAUSTFLOAT fVslider7;
	float fRec88[2];
	float fConst147;
	float fConst148;
	float fConst149;
	float fConst150;
	float fConst151;
	float fRec94[2];
	float fRec93[3];
	float fRec92[3];
	float fVec8[2];
	float fConst152;
	float fConst153;
	float fConst154;
	float fRec91[2];
	float fConst155;
	float fRec90[3];
	float fConst156;
	float fConst157;
	float fRec89[3];
	FAUSTFLOAT fVslider8;
	float fRec95[2];
	float fConst158;
	float fRec98[2];
	float fRec97[3];
	float fRec96[3];
	FAUSTFLOAT fVslider9;
	float fRec99[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.1");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("filename", "flt_eq10.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass_plus_lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass_plus_lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass_plus_lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/mth_octave_filterbank5:author", "Julius O. Smith III");
		m->declare("filters.lib/mth_octave_filterbank5:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/mth_octave_filterbank5:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/mth_octave_filterbank:author", "Julius O. Smith III");
		m->declare("filters.lib/mth_octave_filterbank:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/mth_octave_filterbank:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "flt.eq10");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
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
		fConst1 = std::tan((1570.79639f / fConst0));
		fConst2 = (1.0f / fConst1);
		fConst3 = (1.0f / (((fConst2 + 0.618034005f) / fConst1) + 1.0f));
		fConst4 = flt_eq10_faustpower2_f(fConst1);
		fConst5 = (1.0f / fConst4);
		fConst6 = (1.0f / (((fConst2 + 1.61803401f) / fConst1) + 1.0f));
		fConst7 = std::tan((3141.59277f / fConst0));
		fConst8 = (1.0f / fConst7);
		fConst9 = (1.0f / (((fConst8 + 0.618034005f) / fConst7) + 1.0f));
		fConst10 = (fConst2 + 1.0f);
		fConst11 = (1.0f / (fConst1 * fConst10));
		fConst12 = (1.0f / (((fConst8 + 1.61803401f) / fConst7) + 1.0f));
		fConst13 = (fConst8 + 1.0f);
		fConst14 = (1.0f / fConst13);
		fConst15 = (1.0f - fConst8);
		fConst16 = std::tan((6283.18555f / fConst0));
		fConst17 = (1.0f / fConst16);
		fConst18 = (1.0f / (((fConst17 + 0.618034005f) / fConst16) + 1.0f));
		fConst19 = (1.0f / (((fConst17 + 1.61803401f) / fConst16) + 1.0f));
		fConst20 = (fConst17 + 1.0f);
		fConst21 = (1.0f / fConst20);
		fConst22 = (1.0f - fConst17);
		fConst23 = std::tan((12566.3711f / fConst0));
		fConst24 = (1.0f / fConst23);
		fConst25 = (1.0f / (((fConst24 + 0.618034005f) / fConst23) + 1.0f));
		fConst26 = (1.0f / (((fConst24 + 1.61803401f) / fConst23) + 1.0f));
		fConst27 = (fConst24 + 1.0f);
		fConst28 = (1.0f / fConst27);
		fConst29 = (1.0f - fConst24);
		fConst30 = std::tan((25132.7422f / fConst0));
		fConst31 = (1.0f / fConst30);
		fConst32 = (1.0f / (((fConst31 + 0.618034005f) / fConst30) + 1.0f));
		fConst33 = (1.0f / (((fConst31 + 1.61803401f) / fConst30) + 1.0f));
		fConst34 = (fConst31 + 1.0f);
		fConst35 = (1.0f / fConst34);
		fConst36 = (1.0f - fConst31);
		fConst37 = std::tan((50265.4844f / fConst0));
		fConst38 = (1.0f / fConst37);
		fConst39 = (1.0f / (((fConst38 + 0.618034005f) / fConst37) + 1.0f));
		fConst40 = (1.0f / (((fConst38 + 1.61803401f) / fConst37) + 1.0f));
		fConst41 = (fConst38 + 1.0f);
		fConst42 = (1.0f / fConst41);
		fConst43 = (1.0f - fConst38);
		fConst44 = (((fConst38 + -1.61803401f) / fConst37) + 1.0f);
		fConst45 = flt_eq10_faustpower2_f(fConst37);
		fConst46 = (1.0f / fConst45);
		fConst47 = (2.0f * (1.0f - fConst46));
		fConst48 = (((fConst38 + -0.618034005f) / fConst37) + 1.0f);
		fConst49 = (((fConst31 + -1.61803401f) / fConst30) + 1.0f);
		fConst50 = flt_eq10_faustpower2_f(fConst30);
		fConst51 = (1.0f / fConst50);
		fConst52 = (2.0f * (1.0f - fConst51));
		fConst53 = (((fConst31 + -0.618034005f) / fConst30) + 1.0f);
		fConst54 = (((fConst24 + -1.61803401f) / fConst23) + 1.0f);
		fConst55 = flt_eq10_faustpower2_f(fConst23);
		fConst56 = (1.0f / fConst55);
		fConst57 = (2.0f * (1.0f - fConst56));
		fConst58 = (((fConst24 + -0.618034005f) / fConst23) + 1.0f);
		fConst59 = (((fConst17 + -1.61803401f) / fConst16) + 1.0f);
		fConst60 = flt_eq10_faustpower2_f(fConst16);
		fConst61 = (1.0f / fConst60);
		fConst62 = (2.0f * (1.0f - fConst61));
		fConst63 = (((fConst17 + -0.618034005f) / fConst16) + 1.0f);
		fConst64 = (((fConst8 + -1.61803401f) / fConst7) + 1.0f);
		fConst65 = flt_eq10_faustpower2_f(fConst7);
		fConst66 = (1.0f / fConst65);
		fConst67 = (2.0f * (1.0f - fConst66));
		fConst68 = (((fConst8 + -0.618034005f) / fConst7) + 1.0f);
		fConst69 = (0.0f - fConst11);
		fConst70 = (1.0f - fConst2);
		fConst71 = (fConst70 / fConst10);
		fConst72 = (((fConst2 + -1.61803401f) / fConst1) + 1.0f);
		fConst73 = (2.0f * (1.0f - fConst5));
		fConst74 = (0.0f - (2.0f / fConst4));
		fConst75 = (((fConst2 + -0.618034005f) / fConst1) + 1.0f);
		fConst76 = std::tan((196.349548f / fConst0));
		fConst77 = (1.0f / fConst76);
		fConst78 = (1.0f / (((fConst77 + 1.61803401f) / fConst76) + 1.0f));
		fConst79 = (((fConst77 + -1.61803401f) / fConst76) + 1.0f);
		fConst80 = flt_eq10_faustpower2_f(fConst76);
		fConst81 = (1.0f / fConst80);
		fConst82 = (2.0f * (1.0f - fConst81));
		fConst83 = std::tan((392.699097f / fConst0));
		fConst84 = (1.0f / fConst83);
		fConst85 = (1.0f / (((fConst84 + 1.61803401f) / fConst83) + 1.0f));
		fConst86 = (((fConst84 + -1.61803401f) / fConst83) + 1.0f);
		fConst87 = flt_eq10_faustpower2_f(fConst83);
		fConst88 = (1.0f / fConst87);
		fConst89 = (2.0f * (1.0f - fConst88));
		fConst90 = std::tan((785.398193f / fConst0));
		fConst91 = (1.0f / fConst90);
		fConst92 = (1.0f / (((fConst91 + 1.61803401f) / fConst90) + 1.0f));
		fConst93 = (((fConst91 + -1.61803401f) / fConst90) + 1.0f);
		fConst94 = flt_eq10_faustpower2_f(fConst90);
		fConst95 = (1.0f / fConst94);
		fConst96 = (2.0f * (1.0f - fConst95));
		fConst97 = (1.0f / (fConst7 * fConst13));
		fConst98 = (0.0f - fConst97);
		fConst99 = (fConst15 / fConst13);
		fConst100 = (0.0f - (2.0f / fConst65));
		fConst101 = (1.0f / (((fConst2 + 1.61803401f) / fConst1) + 1.0f));
		fConst102 = (((fConst2 + -1.61803401f) / fConst1) + 1.0f);
		fConst103 = (0.0f - (1.0f / (fConst37 * fConst41)));
		fConst104 = (0.0f - (2.0f / fConst45));
		fConst105 = (1.0f / (((fConst8 + 1.61803401f) / fConst7) + 1.0f));
		fConst106 = (((fConst8 + -1.61803401f) / fConst7) + 1.0f);
		fConst107 = (1.0f / (((fConst17 + 1.61803401f) / fConst16) + 1.0f));
		fConst108 = (((fConst17 + -1.61803401f) / fConst16) + 1.0f);
		fConst109 = (1.0f / (((fConst24 + 1.61803401f) / fConst23) + 1.0f));
		fConst110 = (((fConst24 + -1.61803401f) / fConst23) + 1.0f);
		fConst111 = (1.0f / (((fConst31 + 1.61803401f) / fConst30) + 1.0f));
		fConst112 = (((fConst31 + -1.61803401f) / fConst30) + 1.0f);
		fConst113 = (1.0f / (fConst30 * fConst34));
		fConst114 = (0.0f - fConst113);
		fConst115 = (fConst36 / fConst34);
		fConst116 = (0.0f - (2.0f / fConst50));
		fConst117 = (1.0f / (fConst23 * fConst27));
		fConst118 = (0.0f - fConst117);
		fConst119 = (fConst29 / fConst27);
		fConst120 = (0.0f - (2.0f / fConst55));
		fConst121 = (1.0f / (fConst16 * fConst20));
		fConst122 = (0.0f - fConst121);
		fConst123 = (fConst22 / fConst20);
		fConst124 = (0.0f - (2.0f / fConst60));
		fConst125 = (1.0f / (((fConst91 + 0.618034005f) / fConst90) + 1.0f));
		fConst126 = (1.0f / (((fConst91 + 1.61803401f) / fConst90) + 1.0f));
		fConst127 = (fConst91 + 1.0f);
		fConst128 = (1.0f / (fConst90 * fConst127));
		fConst129 = (1.0f / fConst10);
		fConst130 = (0.0f - fConst128);
		fConst131 = (1.0f - fConst91);
		fConst132 = (fConst131 / fConst127);
		fConst133 = (((fConst91 + -1.61803401f) / fConst90) + 1.0f);
		fConst134 = (0.0f - (2.0f / fConst94));
		fConst135 = (((fConst91 + -0.618034005f) / fConst90) + 1.0f);
		fConst136 = (1.0f / (((fConst84 + 0.618034005f) / fConst83) + 1.0f));
		fConst137 = (1.0f / (((fConst84 + 1.61803401f) / fConst83) + 1.0f));
		fConst138 = (fConst84 + 1.0f);
		fConst139 = (1.0f / (fConst83 * fConst138));
		fConst140 = (1.0f / fConst127);
		fConst141 = (0.0f - fConst139);
		fConst142 = (1.0f - fConst84);
		fConst143 = (fConst142 / fConst138);
		fConst144 = (((fConst84 + -1.61803401f) / fConst83) + 1.0f);
		fConst145 = (0.0f - (2.0f / fConst87));
		fConst146 = (((fConst84 + -0.618034005f) / fConst83) + 1.0f);
		fConst147 = (1.0f / (((fConst77 + 0.618034005f) / fConst76) + 1.0f));
		fConst148 = (1.0f / (((fConst77 + 1.61803401f) / fConst76) + 1.0f));
		fConst149 = (fConst77 + 1.0f);
		fConst150 = (1.0f / (fConst76 * fConst149));
		fConst151 = (1.0f / fConst138);
		fConst152 = (0.0f - fConst150);
		fConst153 = (1.0f - fConst77);
		fConst154 = (fConst153 / fConst149);
		fConst155 = (((fConst77 + -1.61803401f) / fConst76) + 1.0f);
		fConst156 = (0.0f - (2.0f / fConst80));
		fConst157 = (((fConst77 + -0.618034005f) / fConst76) + 1.0f);
		fConst158 = (1.0f / fConst149);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(0.0f);
		fVslider2 = FAUSTFLOAT(0.0f);
		fVslider3 = FAUSTFLOAT(0.0f);
		fVslider4 = FAUSTFLOAT(0.0f);
		fVslider5 = FAUSTFLOAT(0.0f);
		fVslider6 = FAUSTFLOAT(0.0f);
		fVslider7 = FAUSTFLOAT(0.0f);
		fVslider8 = FAUSTFLOAT(0.0f);
		fVslider9 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec20[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
			fRec19[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 3); l3 = (l3 + 1)) {
			fRec18[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fVec1[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fRec17[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 3); l6 = (l6 + 1)) {
			fRec16[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 3); l7 = (l7 + 1)) {
			fRec15[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec2[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec14[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec13[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec12[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fVec3[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec11[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec10[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec9[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fVec4[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec8[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec7[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec6[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fVec5[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec5[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec4[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec3[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 3); l24 = (l24 + 1)) {
			fRec2[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 3); l25 = (l25 + 1)) {
			fRec1[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec0[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec21[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec28[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 3); l29 = (l29 + 1)) {
			fRec27[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 3); l30 = (l30 + 1)) {
			fRec26[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 3); l31 = (l31 + 1)) {
			fRec25[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 3); l32 = (l32 + 1)) {
			fRec24[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec23[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec22[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 2); l35 = (l35 + 1)) {
			fRec29[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 2); l36 = (l36 + 1)) {
			fRec40[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 3); l37 = (l37 + 1)) {
			fRec39[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 3); l38 = (l38 + 1)) {
			fRec38[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 3); l39 = (l39 + 1)) {
			fRec37[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 3); l40 = (l40 + 1)) {
			fRec36[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 3); l41 = (l41 + 1)) {
			fRec35[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 3); l42 = (l42 + 1)) {
			fRec34[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec33[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec32[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec31[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec30[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec41[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec51[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec50[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec49[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec48[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec47[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 3); l53 = (l53 + 1)) {
			fRec46[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec45[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec44[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 3); l56 = (l56 + 1)) {
			fRec43[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec42[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec52[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 2); l59 = (l59 + 1)) {
			fRec61[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec60[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 3); l61 = (l61 + 1)) {
			fRec59[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 3); l62 = (l62 + 1)) {
			fRec58[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 3); l63 = (l63 + 1)) {
			fRec57[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 3); l64 = (l64 + 1)) {
			fRec56[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 3); l65 = (l65 + 1)) {
			fRec55[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 3); l66 = (l66 + 1)) {
			fRec54[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 3); l67 = (l67 + 1)) {
			fRec53[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec62[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec70[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec69[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec68[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec67[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec66[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec65[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec64[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 3); l76 = (l76 + 1)) {
			fRec63[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 2); l77 = (l77 + 1)) {
			fRec71[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec79[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 3); l79 = (l79 + 1)) {
			fRec78[l79] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 3); l80 = (l80 + 1)) {
			fRec77[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 2); l81 = (l81 + 1)) {
			fVec6[l81] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 2); l82 = (l82 + 1)) {
			fRec76[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec75[l83] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec74[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec73[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec72[l86] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec80[l87] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec87[l88] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l89 = 0; (l89 < 3); l89 = (l89 + 1)) {
			fRec86[l89] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l90 = 0; (l90 < 3); l90 = (l90 + 1)) {
			fRec85[l90] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l91 = 0; (l91 < 2); l91 = (l91 + 1)) {
			fVec7[l91] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fRec84[l92] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l93 = 0; (l93 < 3); l93 = (l93 + 1)) {
			fRec83[l93] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l94 = 0; (l94 < 3); l94 = (l94 + 1)) {
			fRec82[l94] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l95 = 0; (l95 < 3); l95 = (l95 + 1)) {
			fRec81[l95] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l96 = 0; (l96 < 2); l96 = (l96 + 1)) {
			fRec88[l96] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec94[l97] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l98 = 0; (l98 < 3); l98 = (l98 + 1)) {
			fRec93[l98] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec92[l99] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l100 = 0; (l100 < 2); l100 = (l100 + 1)) {
			fVec8[l100] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fRec91[l101] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l102 = 0; (l102 < 3); l102 = (l102 + 1)) {
			fRec90[l102] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec89[l103] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l104 = 0; (l104 < 2); l104 = (l104 + 1)) {
			fRec95[l104] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec98[l105] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l106 = 0; (l106 < 3); l106 = (l106 + 1)) {
			fRec97[l106] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec96[l107] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l108 = 0; (l108 < 2); l108 = (l108 + 1)) {
			fRec99[l108] = 0.0f;
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
	
	virtual flt_eq10* clone() {
		return new flt_eq10();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("flt.eq10");
		ui_interface->declare(&fVslider1, "unit", "db");
		ui_interface->addVerticalSlider("f1000", &fVslider1, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider7, "unit", "db");
		ui_interface->addVerticalSlider("f125", &fVslider7, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider2, "unit", "db");
		ui_interface->addVerticalSlider("f16000", &fVslider2, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider5, "unit", "db");
		ui_interface->addVerticalSlider("f2000", &fVslider5, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider6, "unit", "db");
		ui_interface->addVerticalSlider("f250", &fVslider6, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider9, "unit", "db");
		ui_interface->addVerticalSlider("f31", &fVslider9, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider4, "unit", "db");
		ui_interface->addVerticalSlider("f4000", &fVslider4, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "unit", "db");
		ui_interface->addVerticalSlider("f500", &fVslider0, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider8, "unit", "db");
		ui_interface->addVerticalSlider("f62", &fVslider8, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider3, "unit", "db");
		ui_interface->addVerticalSlider("f8000", &fVslider3, 0.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = (0.00100000005f * float(fVslider0));
		float fSlow1 = (0.00100000005f * float(fVslider1));
		float fSlow2 = (0.00100000005f * float(fVslider2));
		float fSlow3 = (0.00100000005f * float(fVslider3));
		float fSlow4 = (0.00100000005f * float(fVslider4));
		float fSlow5 = (0.00100000005f * float(fVslider5));
		float fSlow6 = (0.00100000005f * float(fVslider6));
		float fSlow7 = (0.00100000005f * float(fVslider7));
		float fSlow8 = (0.00100000005f * float(fVslider8));
		float fSlow9 = (0.00100000005f * float(fVslider9));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			float fTemp0 = float(input0[i]);
			fVec0[0] = fTemp0;
			fRec20[0] = (0.0f - (fConst42 * ((fConst43 * fRec20[1]) - (fTemp0 + fVec0[1]))));
			fRec19[0] = (fRec20[0] - (fConst40 * ((fConst44 * fRec19[2]) + (fConst47 * fRec19[1]))));
			fRec18[0] = ((fConst40 * (fRec19[2] + (fRec19[0] + (2.0f * fRec19[1])))) - (fConst39 * ((fConst48 * fRec18[2]) + (fConst47 * fRec18[1]))));
			float fTemp1 = (fRec18[2] + (fRec18[0] + (2.0f * fRec18[1])));
			fVec1[0] = fTemp1;
			fRec17[0] = (0.0f - (fConst35 * ((fConst36 * fRec17[1]) - (fConst39 * (fTemp1 + fVec1[1])))));
			fRec16[0] = (fRec17[0] - (fConst33 * ((fConst49 * fRec16[2]) + (fConst52 * fRec16[1]))));
			fRec15[0] = ((fConst33 * (fRec16[2] + (fRec16[0] + (2.0f * fRec16[1])))) - (fConst32 * ((fConst53 * fRec15[2]) + (fConst52 * fRec15[1]))));
			float fTemp2 = (fRec15[2] + (fRec15[0] + (2.0f * fRec15[1])));
			fVec2[0] = fTemp2;
			fRec14[0] = (0.0f - (fConst28 * ((fConst29 * fRec14[1]) - (fConst32 * (fTemp2 + fVec2[1])))));
			fRec13[0] = (fRec14[0] - (fConst26 * ((fConst54 * fRec13[2]) + (fConst57 * fRec13[1]))));
			fRec12[0] = ((fConst26 * (fRec13[2] + (fRec13[0] + (2.0f * fRec13[1])))) - (fConst25 * ((fConst58 * fRec12[2]) + (fConst57 * fRec12[1]))));
			float fTemp3 = (fRec12[2] + (fRec12[0] + (2.0f * fRec12[1])));
			fVec3[0] = fTemp3;
			fRec11[0] = (0.0f - (fConst21 * ((fConst22 * fRec11[1]) - (fConst25 * (fTemp3 + fVec3[1])))));
			fRec10[0] = (fRec11[0] - (fConst19 * ((fConst59 * fRec10[2]) + (fConst62 * fRec10[1]))));
			fRec9[0] = ((fConst19 * (fRec10[2] + (fRec10[0] + (2.0f * fRec10[1])))) - (fConst18 * ((fConst63 * fRec9[2]) + (fConst62 * fRec9[1]))));
			float fTemp4 = (fRec9[2] + (fRec9[0] + (2.0f * fRec9[1])));
			fVec4[0] = fTemp4;
			fRec8[0] = (0.0f - (fConst14 * ((fConst15 * fRec8[1]) - (fConst18 * (fTemp4 + fVec4[1])))));
			fRec7[0] = (fRec8[0] - (fConst12 * ((fConst64 * fRec7[2]) + (fConst67 * fRec7[1]))));
			fRec6[0] = ((fConst12 * (fRec7[2] + (fRec7[0] + (2.0f * fRec7[1])))) - (fConst9 * ((fConst68 * fRec6[2]) + (fConst67 * fRec6[1]))));
			float fTemp5 = (fRec6[2] + (fRec6[0] + (2.0f * fRec6[1])));
			fVec5[0] = fTemp5;
			fRec5[0] = ((fConst9 * ((fConst11 * fTemp5) + (fConst69 * fVec5[1]))) - (fConst71 * fRec5[1]));
			fRec4[0] = (fRec5[0] - (fConst6 * ((fConst72 * fRec4[2]) + (fConst73 * fRec4[1]))));
			fRec3[0] = ((fConst6 * (((fConst5 * fRec4[0]) + (fConst74 * fRec4[1])) + (fConst5 * fRec4[2]))) - (fConst3 * ((fConst75 * fRec3[2]) + (fConst73 * fRec3[1]))));
			float fTemp6 = (fConst82 * fRec2[1]);
			fRec2[0] = ((fConst3 * (((fConst5 * fRec3[0]) + (fConst74 * fRec3[1])) + (fConst5 * fRec3[2]))) - (fConst78 * ((fConst79 * fRec2[2]) + fTemp6)));
			float fTemp7 = (fConst89 * fRec1[1]);
			fRec1[0] = ((fRec2[2] + (fConst78 * (fTemp6 + (fConst79 * fRec2[0])))) - (fConst85 * ((fConst86 * fRec1[2]) + fTemp7)));
			float fTemp8 = (fConst96 * fRec0[1]);
			fRec0[0] = ((fRec1[2] + (fConst85 * (fTemp7 + (fConst86 * fRec1[0])))) - (fConst92 * ((fConst93 * fRec0[2]) + fTemp8)));
			fRec21[0] = (fSlow0 + (0.999000013f * fRec21[1]));
			fRec28[0] = ((fConst18 * ((fConst97 * fTemp4) + (fConst98 * fVec4[1]))) - (fConst99 * fRec28[1]));
			fRec27[0] = (fRec28[0] - (fConst12 * ((fConst64 * fRec27[2]) + (fConst67 * fRec27[1]))));
			fRec26[0] = ((fConst12 * (((fConst66 * fRec27[0]) + (fConst100 * fRec27[1])) + (fConst66 * fRec27[2]))) - (fConst9 * ((fConst68 * fRec26[2]) + (fConst67 * fRec26[1]))));
			float fTemp9 = (fConst82 * fRec25[1]);
			fRec25[0] = ((fConst9 * (((fConst66 * fRec26[0]) + (fConst100 * fRec26[1])) + (fConst66 * fRec26[2]))) - (fConst78 * ((fConst79 * fRec25[2]) + fTemp9)));
			float fTemp10 = (fConst89 * fRec24[1]);
			fRec24[0] = ((fRec25[2] + (fConst78 * (fTemp9 + (fConst79 * fRec25[0])))) - (fConst85 * ((fConst86 * fRec24[2]) + fTemp10)));
			float fTemp11 = (fConst96 * fRec23[1]);
			fRec23[0] = ((fRec24[2] + (fConst85 * (fTemp10 + (fConst86 * fRec24[0])))) - (fConst92 * ((fConst93 * fRec23[2]) + fTemp11)));
			float fTemp12 = (fConst73 * fRec22[1]);
			fRec22[0] = ((fRec23[2] + (fConst92 * (fTemp11 + (fConst93 * fRec23[0])))) - (fConst101 * ((fConst102 * fRec22[2]) + fTemp12)));
			fRec29[0] = (fSlow1 + (0.999000013f * fRec29[1]));
			fRec40[0] = ((fConst103 * fVec0[1]) - (fConst42 * ((fConst43 * fRec40[1]) - (fConst38 * fTemp0))));
			fRec39[0] = (fRec40[0] - (fConst40 * ((fConst44 * fRec39[2]) + (fConst47 * fRec39[1]))));
			fRec38[0] = ((fConst40 * (((fConst46 * fRec39[0]) + (fConst104 * fRec39[1])) + (fConst46 * fRec39[2]))) - (fConst39 * ((fConst48 * fRec38[2]) + (fConst47 * fRec38[1]))));
			float fTemp13 = (fConst82 * fRec37[1]);
			fRec37[0] = ((fConst39 * (((fConst46 * fRec38[0]) + (fConst104 * fRec38[1])) + (fConst46 * fRec38[2]))) - (fConst78 * ((fConst79 * fRec37[2]) + fTemp13)));
			float fTemp14 = (fConst89 * fRec36[1]);
			fRec36[0] = ((fRec37[2] + (fConst78 * (fTemp13 + (fConst79 * fRec37[0])))) - (fConst85 * ((fConst86 * fRec36[2]) + fTemp14)));
			float fTemp15 = (fConst96 * fRec35[1]);
			fRec35[0] = ((fRec36[2] + (fConst85 * (fTemp14 + (fConst86 * fRec36[0])))) - (fConst92 * ((fConst93 * fRec35[2]) + fTemp15)));
			float fTemp16 = (fConst73 * fRec34[1]);
			fRec34[0] = ((fRec35[2] + (fConst92 * (fTemp15 + (fConst93 * fRec35[0])))) - (fConst101 * ((fConst102 * fRec34[2]) + fTemp16)));
			float fTemp17 = (fConst67 * fRec33[1]);
			fRec33[0] = ((fRec34[2] + (fConst101 * (fTemp16 + (fConst102 * fRec34[0])))) - (fConst105 * ((fConst106 * fRec33[2]) + fTemp17)));
			float fTemp18 = (fConst62 * fRec32[1]);
			fRec32[0] = ((fRec33[2] + (fConst105 * (fTemp17 + (fConst106 * fRec33[0])))) - (fConst107 * ((fConst108 * fRec32[2]) + fTemp18)));
			float fTemp19 = (fConst57 * fRec31[1]);
			fRec31[0] = ((fRec32[2] + (fConst107 * (fTemp18 + (fConst108 * fRec32[0])))) - (fConst109 * ((fConst110 * fRec31[2]) + fTemp19)));
			float fTemp20 = (fConst52 * fRec30[1]);
			fRec30[0] = ((fRec31[2] + (fConst109 * (fTemp19 + (fConst110 * fRec31[0])))) - (fConst111 * ((fConst112 * fRec30[2]) + fTemp20)));
			fRec41[0] = (fSlow2 + (0.999000013f * fRec41[1]));
			fRec51[0] = ((fConst39 * ((fConst113 * fTemp1) + (fConst114 * fVec1[1]))) - (fConst115 * fRec51[1]));
			fRec50[0] = (fRec51[0] - (fConst33 * ((fConst49 * fRec50[2]) + (fConst52 * fRec50[1]))));
			fRec49[0] = ((fConst33 * (((fConst51 * fRec50[0]) + (fConst116 * fRec50[1])) + (fConst51 * fRec50[2]))) - (fConst32 * ((fConst53 * fRec49[2]) + (fConst52 * fRec49[1]))));
			float fTemp21 = (fConst82 * fRec48[1]);
			fRec48[0] = ((fConst32 * (((fConst51 * fRec49[0]) + (fConst116 * fRec49[1])) + (fConst51 * fRec49[2]))) - (fConst78 * ((fConst79 * fRec48[2]) + fTemp21)));
			float fTemp22 = (fConst89 * fRec47[1]);
			fRec47[0] = ((fRec48[2] + (fConst78 * (fTemp21 + (fConst79 * fRec48[0])))) - (fConst85 * ((fConst86 * fRec47[2]) + fTemp22)));
			float fTemp23 = (fConst96 * fRec46[1]);
			fRec46[0] = ((fRec47[2] + (fConst85 * (fTemp22 + (fConst86 * fRec47[0])))) - (fConst92 * ((fConst93 * fRec46[2]) + fTemp23)));
			float fTemp24 = (fConst73 * fRec45[1]);
			fRec45[0] = ((fRec46[2] + (fConst92 * (fTemp23 + (fConst93 * fRec46[0])))) - (fConst101 * ((fConst102 * fRec45[2]) + fTemp24)));
			float fTemp25 = (fConst67 * fRec44[1]);
			fRec44[0] = ((fRec45[2] + (fConst101 * (fTemp24 + (fConst102 * fRec45[0])))) - (fConst105 * ((fConst106 * fRec44[2]) + fTemp25)));
			float fTemp26 = (fConst62 * fRec43[1]);
			fRec43[0] = ((fRec44[2] + (fConst105 * (fTemp25 + (fConst106 * fRec44[0])))) - (fConst107 * ((fConst108 * fRec43[2]) + fTemp26)));
			float fTemp27 = (fConst57 * fRec42[1]);
			fRec42[0] = ((fRec43[2] + (fConst107 * (fTemp26 + (fConst108 * fRec43[0])))) - (fConst109 * ((fConst110 * fRec42[2]) + fTemp27)));
			fRec52[0] = (fSlow3 + (0.999000013f * fRec52[1]));
			fRec61[0] = ((fConst32 * ((fConst117 * fTemp2) + (fConst118 * fVec2[1]))) - (fConst119 * fRec61[1]));
			fRec60[0] = (fRec61[0] - (fConst26 * ((fConst54 * fRec60[2]) + (fConst57 * fRec60[1]))));
			fRec59[0] = ((fConst26 * (((fConst56 * fRec60[0]) + (fConst120 * fRec60[1])) + (fConst56 * fRec60[2]))) - (fConst25 * ((fConst58 * fRec59[2]) + (fConst57 * fRec59[1]))));
			float fTemp28 = (fConst82 * fRec58[1]);
			fRec58[0] = ((fConst25 * (((fConst56 * fRec59[0]) + (fConst120 * fRec59[1])) + (fConst56 * fRec59[2]))) - (fConst78 * ((fConst79 * fRec58[2]) + fTemp28)));
			float fTemp29 = (fConst89 * fRec57[1]);
			fRec57[0] = ((fRec58[2] + (fConst78 * (fTemp28 + (fConst79 * fRec58[0])))) - (fConst85 * ((fConst86 * fRec57[2]) + fTemp29)));
			float fTemp30 = (fConst96 * fRec56[1]);
			fRec56[0] = ((fRec57[2] + (fConst85 * (fTemp29 + (fConst86 * fRec57[0])))) - (fConst92 * ((fConst93 * fRec56[2]) + fTemp30)));
			float fTemp31 = (fConst73 * fRec55[1]);
			fRec55[0] = ((fRec56[2] + (fConst92 * (fTemp30 + (fConst93 * fRec56[0])))) - (fConst101 * ((fConst102 * fRec55[2]) + fTemp31)));
			float fTemp32 = (fConst67 * fRec54[1]);
			fRec54[0] = ((fRec55[2] + (fConst101 * (fTemp31 + (fConst102 * fRec55[0])))) - (fConst105 * ((fConst106 * fRec54[2]) + fTemp32)));
			float fTemp33 = (fConst62 * fRec53[1]);
			fRec53[0] = ((fRec54[2] + (fConst105 * (fTemp32 + (fConst106 * fRec54[0])))) - (fConst107 * ((fConst108 * fRec53[2]) + fTemp33)));
			fRec62[0] = (fSlow4 + (0.999000013f * fRec62[1]));
			fRec70[0] = ((fConst25 * ((fConst121 * fTemp3) + (fConst122 * fVec3[1]))) - (fConst123 * fRec70[1]));
			fRec69[0] = (fRec70[0] - (fConst19 * ((fConst59 * fRec69[2]) + (fConst62 * fRec69[1]))));
			fRec68[0] = ((fConst19 * (((fConst61 * fRec69[0]) + (fConst124 * fRec69[1])) + (fConst61 * fRec69[2]))) - (fConst18 * ((fConst63 * fRec68[2]) + (fConst62 * fRec68[1]))));
			float fTemp34 = (fConst82 * fRec67[1]);
			fRec67[0] = ((fConst18 * (((fConst61 * fRec68[0]) + (fConst124 * fRec68[1])) + (fConst61 * fRec68[2]))) - (fConst78 * ((fConst79 * fRec67[2]) + fTemp34)));
			float fTemp35 = (fConst89 * fRec66[1]);
			fRec66[0] = ((fRec67[2] + (fConst78 * (fTemp34 + (fConst79 * fRec67[0])))) - (fConst85 * ((fConst86 * fRec66[2]) + fTemp35)));
			float fTemp36 = (fConst96 * fRec65[1]);
			fRec65[0] = ((fRec66[2] + (fConst85 * (fTemp35 + (fConst86 * fRec66[0])))) - (fConst92 * ((fConst93 * fRec65[2]) + fTemp36)));
			float fTemp37 = (fConst73 * fRec64[1]);
			fRec64[0] = ((fRec65[2] + (fConst92 * (fTemp36 + (fConst93 * fRec65[0])))) - (fConst101 * ((fConst102 * fRec64[2]) + fTemp37)));
			float fTemp38 = (fConst67 * fRec63[1]);
			fRec63[0] = ((fRec64[2] + (fConst101 * (fTemp37 + (fConst102 * fRec64[0])))) - (fConst105 * ((fConst106 * fRec63[2]) + fTemp38)));
			fRec71[0] = (fSlow5 + (0.999000013f * fRec71[1]));
			fRec79[0] = (fConst129 * ((fConst9 * (fTemp5 + fVec5[1])) - (fConst70 * fRec79[1])));
			fRec78[0] = (fRec79[0] - (fConst6 * ((fConst72 * fRec78[2]) + (fConst73 * fRec78[1]))));
			fRec77[0] = ((fConst6 * (fRec78[2] + (fRec78[0] + (2.0f * fRec78[1])))) - (fConst3 * ((fConst75 * fRec77[2]) + (fConst73 * fRec77[1]))));
			float fTemp39 = (fRec77[2] + (fRec77[0] + (2.0f * fRec77[1])));
			fVec6[0] = fTemp39;
			fRec76[0] = ((fConst3 * ((fConst128 * fTemp39) + (fConst130 * fVec6[1]))) - (fConst132 * fRec76[1]));
			fRec75[0] = (fRec76[0] - (fConst126 * ((fConst133 * fRec75[2]) + (fConst96 * fRec75[1]))));
			fRec74[0] = ((fConst126 * (((fConst95 * fRec75[0]) + (fConst134 * fRec75[1])) + (fConst95 * fRec75[2]))) - (fConst125 * ((fConst135 * fRec74[2]) + (fConst96 * fRec74[1]))));
			float fTemp40 = (fConst82 * fRec73[1]);
			fRec73[0] = ((fConst125 * (((fConst95 * fRec74[0]) + (fConst134 * fRec74[1])) + (fConst95 * fRec74[2]))) - (fConst78 * ((fConst79 * fRec73[2]) + fTemp40)));
			float fTemp41 = (fConst89 * fRec72[1]);
			fRec72[0] = ((fRec73[2] + (fConst78 * (fTemp40 + (fConst79 * fRec73[0])))) - (fConst85 * ((fConst86 * fRec72[2]) + fTemp41)));
			fRec80[0] = (fSlow6 + (0.999000013f * fRec80[1]));
			fRec87[0] = (0.0f - (fConst140 * ((fConst131 * fRec87[1]) - (fConst3 * (fTemp39 + fVec6[1])))));
			fRec86[0] = (fRec87[0] - (fConst126 * ((fConst133 * fRec86[2]) + (fConst96 * fRec86[1]))));
			fRec85[0] = ((fConst126 * (fRec86[2] + (fRec86[0] + (2.0f * fRec86[1])))) - (fConst125 * ((fConst135 * fRec85[2]) + (fConst96 * fRec85[1]))));
			float fTemp42 = (fRec85[2] + (fRec85[0] + (2.0f * fRec85[1])));
			fVec7[0] = fTemp42;
			fRec84[0] = ((fConst125 * ((fConst139 * fTemp42) + (fConst141 * fVec7[1]))) - (fConst143 * fRec84[1]));
			fRec83[0] = (fRec84[0] - (fConst137 * ((fConst144 * fRec83[2]) + (fConst89 * fRec83[1]))));
			fRec82[0] = ((fConst137 * (((fConst88 * fRec83[0]) + (fConst145 * fRec83[1])) + (fConst88 * fRec83[2]))) - (fConst136 * ((fConst146 * fRec82[2]) + (fConst89 * fRec82[1]))));
			float fTemp43 = (fConst82 * fRec81[1]);
			fRec81[0] = ((fConst136 * (((fConst88 * fRec82[0]) + (fConst145 * fRec82[1])) + (fConst88 * fRec82[2]))) - (fConst78 * ((fConst79 * fRec81[2]) + fTemp43)));
			fRec88[0] = (fSlow7 + (0.999000013f * fRec88[1]));
			fRec94[0] = (0.0f - (fConst151 * ((fConst142 * fRec94[1]) - (fConst125 * (fTemp42 + fVec7[1])))));
			fRec93[0] = (fRec94[0] - (fConst137 * ((fConst144 * fRec93[2]) + (fConst89 * fRec93[1]))));
			fRec92[0] = ((fConst137 * (fRec93[2] + (fRec93[0] + (2.0f * fRec93[1])))) - (fConst136 * ((fConst146 * fRec92[2]) + (fConst89 * fRec92[1]))));
			float fTemp44 = (fRec92[2] + (fRec92[0] + (2.0f * fRec92[1])));
			fVec8[0] = fTemp44;
			fRec91[0] = ((fConst136 * ((fConst150 * fTemp44) + (fConst152 * fVec8[1]))) - (fConst154 * fRec91[1]));
			fRec90[0] = (fRec91[0] - (fConst148 * ((fConst155 * fRec90[2]) + (fConst82 * fRec90[1]))));
			fRec89[0] = ((fConst148 * (((fConst81 * fRec90[0]) + (fConst156 * fRec90[1])) + (fConst81 * fRec90[2]))) - (fConst147 * ((fConst157 * fRec89[2]) + (fConst82 * fRec89[1]))));
			fRec95[0] = (fSlow8 + (0.999000013f * fRec95[1]));
			fRec98[0] = (0.0f - (fConst158 * ((fConst153 * fRec98[1]) - (fConst136 * (fTemp44 + fVec8[1])))));
			fRec97[0] = (fRec98[0] - (fConst148 * ((fConst155 * fRec97[2]) + (fConst82 * fRec97[1]))));
			fRec96[0] = ((fConst148 * (fRec97[2] + (fRec97[0] + (2.0f * fRec97[1])))) - (fConst147 * ((fConst157 * fRec96[2]) + (fConst82 * fRec96[1]))));
			fRec99[0] = (fSlow9 + (0.999000013f * fRec99[1]));
			output0[i] = FAUSTFLOAT(((((fRec0[2] + (fConst92 * (fTemp8 + (fConst93 * fRec0[0])))) * std::pow(10.0f, (0.0500000007f * fRec21[0]))) + (((fRec22[2] + (fConst101 * (fTemp12 + (fConst102 * fRec22[0])))) * std::pow(10.0f, (0.0500000007f * fRec29[0]))) + (((((((fRec30[2] + (fConst111 * (fTemp20 + (fConst112 * fRec30[0])))) * std::pow(10.0f, (0.0500000007f * fRec41[0]))) + ((fRec42[2] + (fConst109 * (fTemp27 + (fConst110 * fRec42[0])))) * std::pow(10.0f, (0.0500000007f * fRec52[0])))) + ((fRec53[2] + (fConst107 * (fTemp33 + (fConst108 * fRec53[0])))) * std::pow(10.0f, (0.0500000007f * fRec62[0])))) + ((fRec63[2] + (fConst105 * (fTemp38 + (fConst106 * fRec63[0])))) * std::pow(10.0f, (0.0500000007f * fRec71[0])))) + ((fRec72[2] + (fConst85 * (fTemp41 + (fConst86 * fRec72[0])))) * std::pow(10.0f, (0.0500000007f * fRec80[0])))) + ((fRec81[2] + (fConst78 * (fTemp43 + (fConst79 * fRec81[0])))) * std::pow(10.0f, (0.0500000007f * fRec88[0])))))) + (fConst147 * (((((fConst81 * fRec89[0]) + (fConst156 * fRec89[1])) + (fConst81 * fRec89[2])) * std::pow(10.0f, (0.0500000007f * fRec95[0]))) + ((fRec96[2] + (fRec96[0] + (2.0f * fRec96[1]))) * std::pow(10.0f, (0.0500000007f * fRec99[0])))))));
			fVec0[1] = fVec0[0];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fVec1[1] = fVec1[0];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fVec2[1] = fVec2[0];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fVec3[1] = fVec3[0];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fVec4[1] = fVec4[0];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fVec5[1] = fVec5[0];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fRec21[1] = fRec21[0];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec29[1] = fRec29[0];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fRec41[1] = fRec41[0];
			fRec51[1] = fRec51[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec44[2] = fRec44[1];
			fRec44[1] = fRec44[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec52[1] = fRec52[0];
			fRec61[1] = fRec61[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec62[1] = fRec62[0];
			fRec70[1] = fRec70[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec68[2] = fRec68[1];
			fRec68[1] = fRec68[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec71[1] = fRec71[0];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fVec6[1] = fVec6[0];
			fRec76[1] = fRec76[0];
			fRec75[2] = fRec75[1];
			fRec75[1] = fRec75[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec72[2] = fRec72[1];
			fRec72[1] = fRec72[0];
			fRec80[1] = fRec80[0];
			fRec87[1] = fRec87[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fRec85[2] = fRec85[1];
			fRec85[1] = fRec85[0];
			fVec7[1] = fVec7[0];
			fRec84[1] = fRec84[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec88[1] = fRec88[0];
			fRec94[1] = fRec94[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fRec92[2] = fRec92[1];
			fRec92[1] = fRec92[0];
			fVec8[1] = fVec8[0];
			fRec91[1] = fRec91[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec89[2] = fRec89[1];
			fRec89[1] = fRec89[0];
			fRec95[1] = fRec95[0];
			fRec98[1] = fRec98[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			fRec96[2] = fRec96[1];
			fRec96[1] = fRec96[0];
			fRec99[1] = fRec99[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _flt_eq10_UI : public UI {
    static std::string name;
};

template <class T>
std::string _flt_eq10_UI<T>::name(sym(flt_eq10));

typedef _flt_eq10_UI<flt_eq10> flt_eq10_UI;

class faust_flt_eq10_tilde : public FaustExternal<flt_eq10, flt_eq10_UI> {
public:
    faust_flt_eq10_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
