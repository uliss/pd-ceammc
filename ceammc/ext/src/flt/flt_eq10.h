/* ------------------------------------------------------------
name: "flt_eq10"
Code generated with Faust 2.8.5 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __flt_eq10_H__
#define  __flt_eq10_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

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

class UI;
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

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
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
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void init(int samplingRate) = 0;

        /**
         * Init instance state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceInit(int samplingRate) = 0;

        /**
         * Init instance constant state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int samplingRate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
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
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = 0):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int samplingRate) { fDSP->init(samplingRate); }
        virtual void instanceInit(int samplingRate) { fDSP->instanceInit(samplingRate); }
        virtual void instanceConstants(int samplingRate) { fDSP->instanceConstants(samplingRate); }
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
    
        virtual dsp* createDSPInstance() = 0;
    
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

class UI
{

    public:

        UI() {}

        virtual ~UI() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

#endif
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
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {};
};

#endif
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
#include <string.h>
#include <stdlib.h>
#include <cstdlib>


using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

inline int lsr(int x, int n)	{ return int(((unsigned int)x) >> n); }

inline int int2pow2(int x)		{ int r = 0; while ((1<<r) < x) r++; return r; }

inline long lopt(char* argv[], const char* name, long def)
{
	int	i;
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
	return def;
}

inline bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

inline const char* lopts(char* argv[], const char* name, const char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}

#endif


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
struct flt_eq10 : public dsp {
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

class flt_eq10 : public dsp {
	
 private:
	
	FAUSTFLOAT fVslider0;
	float fRec0[2];
	int fSamplingFreq;
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
	float fConst44;
	float fConst45;
	float fConst46;
	float fConst47;
	float fConst48;
	float fConst49;
	float fConst50;
	float fVec0[2];
	float fConst51;
	float fRec23[2];
	float fConst52;
	float fConst53;
	float fConst54;
	float fRec22[3];
	float fConst55;
	float fConst56;
	float fRec21[3];
	float fVec1[2];
	float fRec20[2];
	float fConst57;
	float fConst58;
	float fConst59;
	float fRec19[3];
	float fConst60;
	float fConst61;
	float fRec18[3];
	float fVec2[2];
	float fConst62;
	float fRec17[2];
	float fConst63;
	float fConst64;
	float fConst65;
	float fRec16[3];
	float fConst66;
	float fConst67;
	float fRec15[3];
	float fVec3[2];
	float fRec14[2];
	float fConst68;
	float fConst69;
	float fConst70;
	float fRec13[3];
	float fConst71;
	float fConst72;
	float fRec12[3];
	float fVec4[2];
	float fRec11[2];
	float fConst73;
	float fConst74;
	float fConst75;
	float fRec10[3];
	float fConst76;
	float fConst77;
	float fRec9[3];
	float fVec5[2];
	float fRec8[2];
	float fConst78;
	float fConst79;
	float fConst80;
	float fRec7[3];
	float fConst81;
	float fConst82;
	float fRec6[3];
	float fVec6[2];
	float fConst83;
	float fRec5[2];
	float fConst84;
	float fConst85;
	float fRec4[3];
	float fConst86;
	float fRec3[3];
	float fConst87;
	float fConst88;
	float fConst89;
	float fConst90;
	float fConst91;
	float fConst92;
	float fRec2[3];
	float fConst93;
	float fConst94;
	float fConst95;
	float fConst96;
	float fConst97;
	float fConst98;
	float fRec1[3];
	FAUSTFLOAT fVslider1;
	float fRec24[2];
	float fConst99;
	float fConst100;
	float fRec30[2];
	float fRec29[3];
	float fRec28[3];
	float fRec27[3];
	float fRec26[3];
	float fConst101;
	float fConst102;
	float fRec25[3];
	FAUSTFLOAT fVslider2;
	float fRec31[2];
	float fConst103;
	float fConst104;
	float fRec38[2];
	float fRec37[3];
	float fRec36[3];
	float fRec35[3];
	float fRec34[3];
	float fRec33[3];
	float fConst105;
	float fConst106;
	float fRec32[3];
	FAUSTFLOAT fVslider3;
	float fRec39[2];
	float fConst107;
	float fRec47[2];
	float fRec46[3];
	float fConst108;
	float fRec45[3];
	float fRec44[3];
	float fRec43[3];
	float fRec42[3];
	float fRec41[3];
	float fConst109;
	float fConst110;
	float fRec40[3];
	FAUSTFLOAT fVslider4;
	float fRec48[2];
	float fConst111;
	float fRec58[2];
	float fRec57[3];
	float fConst112;
	float fRec56[3];
	float fRec55[3];
	float fRec54[3];
	float fRec53[3];
	float fRec52[3];
	float fRec51[3];
	float fConst113;
	float fConst114;
	float fRec50[3];
	float fConst115;
	float fConst116;
	float fRec49[3];
	FAUSTFLOAT fVslider5;
	float fRec59[2];
	float fConst117;
	float fConst118;
	float fRec70[2];
	float fRec69[3];
	float fRec68[3];
	float fRec67[3];
	float fRec66[3];
	float fRec65[3];
	float fRec64[3];
	float fRec63[3];
	float fRec62[3];
	float fRec61[3];
	float fConst119;
	float fConst120;
	float fRec60[3];
	float fConst121;
	float fRec79[2];
	float fRec78[3];
	float fConst122;
	float fRec77[3];
	float fRec76[3];
	float fRec75[3];
	float fRec74[3];
	float fRec73[3];
	float fRec72[3];
	float fRec71[3];
	FAUSTFLOAT fVslider6;
	float fRec80[2];
	FAUSTFLOAT fVslider7;
	float fRec81[2];
	float fConst123;
	float fConst124;
	float fConst125;
	float fConst126;
	float fConst127;
	float fConst128;
	float fConst129;
	float fRec88[2];
	float fRec87[3];
	float fRec86[3];
	float fVec7[2];
	float fConst130;
	float fRec85[2];
	float fConst131;
	float fRec84[3];
	float fConst132;
	float fRec83[3];
	float fRec82[3];
	float fConst133;
	FAUSTFLOAT fVslider8;
	float fRec89[2];
	float fConst134;
	float fConst135;
	float fConst136;
	float fConst137;
	float fConst138;
	float fRec95[2];
	float fRec94[3];
	float fRec93[3];
	float fVec8[2];
	float fConst139;
	float fRec92[2];
	float fConst140;
	float fRec91[3];
	float fConst141;
	float fRec90[3];
	FAUSTFLOAT fVslider9;
	float fRec96[2];
	float fRec99[2];
	float fRec98[3];
	float fRec97[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "0.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("filename", "flt_eq10");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "flt_eq10");
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
		switch (channel) {
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
		switch (channel) {
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
	
	static void classInit(int samplingFreq) {
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = std::min(192000.0f, std::max(1.0f, float(fSamplingFreq)));
		fConst1 = std::tan((785.398193f / fConst0));
		fConst2 = (1.0f / fConst1);
		fConst3 = (((fConst2 + 0.618034005f) / fConst1) + 1.0f);
		fConst4 = (1.0f / fConst3);
		fConst5 = (1.0f / flt_eq10_faustpower2_f(fConst1));
		fConst6 = (2.0f * (0.0f - fConst5));
		fConst7 = (1.0f / (((fConst2 + 1.61803401f) / fConst1) + 1.0f));
		fConst8 = (fConst2 + 1.0f);
		fConst9 = (0.0f - ((1.0f - fConst2) / fConst8));
		fConst10 = std::tan((1570.79639f / fConst0));
		fConst11 = (1.0f / fConst10);
		fConst12 = (((fConst11 + 0.618034005f) / fConst10) + 1.0f);
		fConst13 = (1.0f / (fConst8 * fConst12));
		fConst14 = (1.0f / (((fConst11 + 1.61803401f) / fConst10) + 1.0f));
		fConst15 = (fConst11 + 1.0f);
		fConst16 = (0.0f - ((1.0f - fConst11) / fConst15));
		fConst17 = std::tan((3141.59277f / fConst0));
		fConst18 = (1.0f / fConst17);
		fConst19 = (((fConst18 + 0.618034005f) / fConst17) + 1.0f);
		fConst20 = (1.0f / (fConst15 * fConst19));
		fConst21 = (1.0f / (((fConst18 + 1.61803401f) / fConst17) + 1.0f));
		fConst22 = (fConst18 + 1.0f);
		fConst23 = (0.0f - ((1.0f - fConst18) / fConst22));
		fConst24 = std::tan((6283.18555f / fConst0));
		fConst25 = (1.0f / fConst24);
		fConst26 = (((fConst25 + 0.618034005f) / fConst24) + 1.0f);
		fConst27 = (1.0f / (fConst26 * fConst22));
		fConst28 = (1.0f / (((fConst25 + 1.61803401f) / fConst24) + 1.0f));
		fConst29 = (fConst25 + 1.0f);
		fConst30 = (0.0f - ((1.0f - fConst25) / fConst29));
		fConst31 = std::tan((12566.3711f / fConst0));
		fConst32 = (1.0f / fConst31);
		fConst33 = (((fConst32 + 0.618034005f) / fConst31) + 1.0f);
		fConst34 = (1.0f / (fConst29 * fConst33));
		fConst35 = (1.0f / (((fConst32 + 1.61803401f) / fConst31) + 1.0f));
		fConst36 = std::tan((25132.7422f / fConst0));
		fConst37 = (1.0f / fConst36);
		fConst38 = (((fConst37 + 0.618034005f) / fConst36) + 1.0f);
		fConst39 = (fConst32 + 1.0f);
		fConst40 = (1.0f / (fConst38 * fConst39));
		fConst41 = (1.0f / (((fConst37 + 1.61803401f) / fConst36) + 1.0f));
		fConst42 = (fConst37 + 1.0f);
		fConst43 = (0.0f - ((1.0f - fConst37) / fConst42));
		fConst44 = std::tan((50265.4844f / fConst0));
		fConst45 = (1.0f / fConst44);
		fConst46 = (((fConst45 + 0.618034005f) / fConst44) + 1.0f);
		fConst47 = (1.0f / (fConst42 * fConst46));
		fConst48 = (1.0f / (((fConst45 + 1.61803401f) / fConst44) + 1.0f));
		fConst49 = (fConst45 + 1.0f);
		fConst50 = (1.0f / fConst49);
		fConst51 = (0.0f - ((1.0f - fConst45) / fConst49));
		fConst52 = (((fConst45 + -1.61803401f) / fConst44) + 1.0f);
		fConst53 = (1.0f / flt_eq10_faustpower2_f(fConst44));
		fConst54 = (2.0f * (1.0f - fConst53));
		fConst55 = (1.0f / fConst46);
		fConst56 = (((fConst45 + -0.618034005f) / fConst44) + 1.0f);
		fConst57 = (((fConst37 + -1.61803401f) / fConst36) + 1.0f);
		fConst58 = (1.0f / flt_eq10_faustpower2_f(fConst36));
		fConst59 = (2.0f * (1.0f - fConst58));
		fConst60 = (1.0f / fConst38);
		fConst61 = (((fConst37 + -0.618034005f) / fConst36) + 1.0f);
		fConst62 = (0.0f - ((1.0f - fConst32) / fConst39));
		fConst63 = (((fConst32 + -1.61803401f) / fConst31) + 1.0f);
		fConst64 = (1.0f / flt_eq10_faustpower2_f(fConst31));
		fConst65 = (2.0f * (1.0f - fConst64));
		fConst66 = (1.0f / fConst33);
		fConst67 = (((fConst32 + -0.618034005f) / fConst31) + 1.0f);
		fConst68 = (((fConst25 + -1.61803401f) / fConst24) + 1.0f);
		fConst69 = (1.0f / flt_eq10_faustpower2_f(fConst24));
		fConst70 = (2.0f * (1.0f - fConst69));
		fConst71 = (1.0f / fConst26);
		fConst72 = (((fConst25 + -0.618034005f) / fConst24) + 1.0f);
		fConst73 = (((fConst18 + -1.61803401f) / fConst17) + 1.0f);
		fConst74 = (1.0f / flt_eq10_faustpower2_f(fConst17));
		fConst75 = (2.0f * (1.0f - fConst74));
		fConst76 = (1.0f / fConst19);
		fConst77 = (((fConst18 + -0.618034005f) / fConst17) + 1.0f);
		fConst78 = (((fConst11 + -1.61803401f) / fConst10) + 1.0f);
		fConst79 = (1.0f / flt_eq10_faustpower2_f(fConst10));
		fConst80 = (2.0f * (1.0f - fConst79));
		fConst81 = (1.0f / fConst12);
		fConst82 = (((fConst11 + -0.618034005f) / fConst10) + 1.0f);
		fConst83 = (0.0f - fConst2);
		fConst84 = (((fConst2 + -1.61803401f) / fConst1) + 1.0f);
		fConst85 = (2.0f * (1.0f - fConst5));
		fConst86 = (((fConst2 + -0.618034005f) / fConst1) + 1.0f);
		fConst87 = std::tan((196.349548f / fConst0));
		fConst88 = (1.0f / fConst87);
		fConst89 = (1.0f / (((fConst88 + 1.61803401f) / fConst87) + 1.0f));
		fConst90 = (1.0f / flt_eq10_faustpower2_f(fConst87));
		fConst91 = (2.0f * (1.0f - fConst90));
		fConst92 = (((fConst88 + -1.61803401f) / fConst87) + 1.0f);
		fConst93 = std::tan((392.699097f / fConst0));
		fConst94 = (1.0f / fConst93);
		fConst95 = (1.0f / (((fConst94 + 1.61803401f) / fConst93) + 1.0f));
		fConst96 = (((fConst94 + -1.61803401f) / fConst93) + 1.0f);
		fConst97 = (1.0f / flt_eq10_faustpower2_f(fConst93));
		fConst98 = (2.0f * (1.0f - fConst97));
		fConst99 = (2.0f * (0.0f - fConst79));
		fConst100 = (0.0f - fConst11);
		fConst101 = (1.0f / (((fConst2 + 1.61803401f) / fConst1) + 1.0f));
		fConst102 = (((fConst2 + -1.61803401f) / fConst1) + 1.0f);
		fConst103 = (2.0f * (0.0f - fConst74));
		fConst104 = (0.0f - fConst18);
		fConst105 = (1.0f / (((fConst11 + 1.61803401f) / fConst10) + 1.0f));
		fConst106 = (((fConst11 + -1.61803401f) / fConst10) + 1.0f);
		fConst107 = (0.0f - fConst25);
		fConst108 = (2.0f * (0.0f - fConst69));
		fConst109 = (1.0f / (((fConst18 + 1.61803401f) / fConst17) + 1.0f));
		fConst110 = (((fConst18 + -1.61803401f) / fConst17) + 1.0f);
		fConst111 = (0.0f - fConst37);
		fConst112 = (2.0f * (0.0f - fConst58));
		fConst113 = (1.0f / (((fConst25 + 1.61803401f) / fConst24) + 1.0f));
		fConst114 = (((fConst25 + -1.61803401f) / fConst24) + 1.0f);
		fConst115 = (1.0f / (((fConst32 + 1.61803401f) / fConst31) + 1.0f));
		fConst116 = (((fConst32 + -1.61803401f) / fConst31) + 1.0f);
		fConst117 = (2.0f * (0.0f - fConst53));
		fConst118 = (0.0f - fConst45);
		fConst119 = (1.0f / (((fConst37 + 1.61803401f) / fConst36) + 1.0f));
		fConst120 = (((fConst37 + -1.61803401f) / fConst36) + 1.0f);
		fConst121 = (0.0f - fConst32);
		fConst122 = (2.0f * (0.0f - fConst64));
		fConst123 = (((fConst94 + 0.618034005f) / fConst93) + 1.0f);
		fConst124 = (1.0f / fConst123);
		fConst125 = (2.0f * (0.0f - fConst97));
		fConst126 = (1.0f / (((fConst94 + 1.61803401f) / fConst93) + 1.0f));
		fConst127 = (fConst94 + 1.0f);
		fConst128 = (0.0f - ((1.0f - fConst94) / fConst127));
		fConst129 = (1.0f / (fConst3 * fConst127));
		fConst130 = (0.0f - fConst94);
		fConst131 = (((fConst94 + -1.61803401f) / fConst93) + 1.0f);
		fConst132 = (((fConst94 + -0.618034005f) / fConst93) + 1.0f);
		fConst133 = (1.0f / (((fConst88 + 0.618034005f) / fConst87) + 1.0f));
		fConst134 = (2.0f * (0.0f - fConst90));
		fConst135 = (1.0f / (((fConst88 + 1.61803401f) / fConst87) + 1.0f));
		fConst136 = (fConst88 + 1.0f);
		fConst137 = (0.0f - ((1.0f - fConst88) / fConst136));
		fConst138 = (1.0f / (fConst136 * fConst123));
		fConst139 = (0.0f - fConst88);
		fConst140 = (((fConst88 + -1.61803401f) / fConst87) + 1.0f);
		fConst141 = (((fConst88 + -0.618034005f) / fConst87) + 1.0f);
		
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(-10.0f);
		fVslider1 = FAUSTFLOAT(-10.0f);
		fVslider2 = FAUSTFLOAT(-10.0f);
		fVslider3 = FAUSTFLOAT(-10.0f);
		fVslider4 = FAUSTFLOAT(-10.0f);
		fVslider5 = FAUSTFLOAT(-10.0f);
		fVslider6 = FAUSTFLOAT(-10.0f);
		fVslider7 = FAUSTFLOAT(-10.0f);
		fVslider8 = FAUSTFLOAT(-10.0f);
		fVslider9 = FAUSTFLOAT(-10.0f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec0[l0] = 0.0f;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fVec0[l1] = 0.0f;
			
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec23[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 3); l3 = (l3 + 1)) {
			fRec22[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 3); l4 = (l4 + 1)) {
			fRec21[l4] = 0.0f;
			
		}
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec1[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec20[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 3); l7 = (l7 + 1)) {
			fRec19[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 3); l8 = (l8 + 1)) {
			fRec18[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fVec2[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec17[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec16[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec15[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fVec3[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec14[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec13[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec12[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fVec4[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec11[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec10[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec9[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fVec5[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fRec8[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec7[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 3); l24 = (l24 + 1)) {
			fRec6[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fVec6[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 2); l26 = (l26 + 1)) {
			fRec5[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec4[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec3[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 3); l29 = (l29 + 1)) {
			fRec2[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 3); l30 = (l30 + 1)) {
			fRec1[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec24[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 2); l32 = (l32 + 1)) {
			fRec30[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec29[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec28[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec27[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 3); l36 = (l36 + 1)) {
			fRec26[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 3); l37 = (l37 + 1)) {
			fRec25[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec31[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec38[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 3); l40 = (l40 + 1)) {
			fRec37[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 3); l41 = (l41 + 1)) {
			fRec36[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 3); l42 = (l42 + 1)) {
			fRec35[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec34[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec33[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec32[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec39[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 2); l47 = (l47 + 1)) {
			fRec47[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec46[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec45[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec44[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec43[l51] = 0.0f;
			
		}
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec42[l52] = 0.0f;
			
		}
		for (int l53 = 0; (l53 < 3); l53 = (l53 + 1)) {
			fRec41[l53] = 0.0f;
			
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec40[l54] = 0.0f;
			
		}
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec48[l55] = 0.0f;
			
		}
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec58[l56] = 0.0f;
			
		}
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec57[l57] = 0.0f;
			
		}
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec56[l58] = 0.0f;
			
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec55[l59] = 0.0f;
			
		}
		for (int l60 = 0; (l60 < 3); l60 = (l60 + 1)) {
			fRec54[l60] = 0.0f;
			
		}
		for (int l61 = 0; (l61 < 3); l61 = (l61 + 1)) {
			fRec53[l61] = 0.0f;
			
		}
		for (int l62 = 0; (l62 < 3); l62 = (l62 + 1)) {
			fRec52[l62] = 0.0f;
			
		}
		for (int l63 = 0; (l63 < 3); l63 = (l63 + 1)) {
			fRec51[l63] = 0.0f;
			
		}
		for (int l64 = 0; (l64 < 3); l64 = (l64 + 1)) {
			fRec50[l64] = 0.0f;
			
		}
		for (int l65 = 0; (l65 < 3); l65 = (l65 + 1)) {
			fRec49[l65] = 0.0f;
			
		}
		for (int l66 = 0; (l66 < 2); l66 = (l66 + 1)) {
			fRec59[l66] = 0.0f;
			
		}
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec70[l67] = 0.0f;
			
		}
		for (int l68 = 0; (l68 < 3); l68 = (l68 + 1)) {
			fRec69[l68] = 0.0f;
			
		}
		for (int l69 = 0; (l69 < 3); l69 = (l69 + 1)) {
			fRec68[l69] = 0.0f;
			
		}
		for (int l70 = 0; (l70 < 3); l70 = (l70 + 1)) {
			fRec67[l70] = 0.0f;
			
		}
		for (int l71 = 0; (l71 < 3); l71 = (l71 + 1)) {
			fRec66[l71] = 0.0f;
			
		}
		for (int l72 = 0; (l72 < 3); l72 = (l72 + 1)) {
			fRec65[l72] = 0.0f;
			
		}
		for (int l73 = 0; (l73 < 3); l73 = (l73 + 1)) {
			fRec64[l73] = 0.0f;
			
		}
		for (int l74 = 0; (l74 < 3); l74 = (l74 + 1)) {
			fRec63[l74] = 0.0f;
			
		}
		for (int l75 = 0; (l75 < 3); l75 = (l75 + 1)) {
			fRec62[l75] = 0.0f;
			
		}
		for (int l76 = 0; (l76 < 3); l76 = (l76 + 1)) {
			fRec61[l76] = 0.0f;
			
		}
		for (int l77 = 0; (l77 < 3); l77 = (l77 + 1)) {
			fRec60[l77] = 0.0f;
			
		}
		for (int l78 = 0; (l78 < 2); l78 = (l78 + 1)) {
			fRec79[l78] = 0.0f;
			
		}
		for (int l79 = 0; (l79 < 3); l79 = (l79 + 1)) {
			fRec78[l79] = 0.0f;
			
		}
		for (int l80 = 0; (l80 < 3); l80 = (l80 + 1)) {
			fRec77[l80] = 0.0f;
			
		}
		for (int l81 = 0; (l81 < 3); l81 = (l81 + 1)) {
			fRec76[l81] = 0.0f;
			
		}
		for (int l82 = 0; (l82 < 3); l82 = (l82 + 1)) {
			fRec75[l82] = 0.0f;
			
		}
		for (int l83 = 0; (l83 < 3); l83 = (l83 + 1)) {
			fRec74[l83] = 0.0f;
			
		}
		for (int l84 = 0; (l84 < 3); l84 = (l84 + 1)) {
			fRec73[l84] = 0.0f;
			
		}
		for (int l85 = 0; (l85 < 3); l85 = (l85 + 1)) {
			fRec72[l85] = 0.0f;
			
		}
		for (int l86 = 0; (l86 < 3); l86 = (l86 + 1)) {
			fRec71[l86] = 0.0f;
			
		}
		for (int l87 = 0; (l87 < 2); l87 = (l87 + 1)) {
			fRec80[l87] = 0.0f;
			
		}
		for (int l88 = 0; (l88 < 2); l88 = (l88 + 1)) {
			fRec81[l88] = 0.0f;
			
		}
		for (int l89 = 0; (l89 < 2); l89 = (l89 + 1)) {
			fRec88[l89] = 0.0f;
			
		}
		for (int l90 = 0; (l90 < 3); l90 = (l90 + 1)) {
			fRec87[l90] = 0.0f;
			
		}
		for (int l91 = 0; (l91 < 3); l91 = (l91 + 1)) {
			fRec86[l91] = 0.0f;
			
		}
		for (int l92 = 0; (l92 < 2); l92 = (l92 + 1)) {
			fVec7[l92] = 0.0f;
			
		}
		for (int l93 = 0; (l93 < 2); l93 = (l93 + 1)) {
			fRec85[l93] = 0.0f;
			
		}
		for (int l94 = 0; (l94 < 3); l94 = (l94 + 1)) {
			fRec84[l94] = 0.0f;
			
		}
		for (int l95 = 0; (l95 < 3); l95 = (l95 + 1)) {
			fRec83[l95] = 0.0f;
			
		}
		for (int l96 = 0; (l96 < 3); l96 = (l96 + 1)) {
			fRec82[l96] = 0.0f;
			
		}
		for (int l97 = 0; (l97 < 2); l97 = (l97 + 1)) {
			fRec89[l97] = 0.0f;
			
		}
		for (int l98 = 0; (l98 < 2); l98 = (l98 + 1)) {
			fRec95[l98] = 0.0f;
			
		}
		for (int l99 = 0; (l99 < 3); l99 = (l99 + 1)) {
			fRec94[l99] = 0.0f;
			
		}
		for (int l100 = 0; (l100 < 3); l100 = (l100 + 1)) {
			fRec93[l100] = 0.0f;
			
		}
		for (int l101 = 0; (l101 < 2); l101 = (l101 + 1)) {
			fVec8[l101] = 0.0f;
			
		}
		for (int l102 = 0; (l102 < 2); l102 = (l102 + 1)) {
			fRec92[l102] = 0.0f;
			
		}
		for (int l103 = 0; (l103 < 3); l103 = (l103 + 1)) {
			fRec91[l103] = 0.0f;
			
		}
		for (int l104 = 0; (l104 < 3); l104 = (l104 + 1)) {
			fRec90[l104] = 0.0f;
			
		}
		for (int l105 = 0; (l105 < 2); l105 = (l105 + 1)) {
			fRec96[l105] = 0.0f;
			
		}
		for (int l106 = 0; (l106 < 2); l106 = (l106 + 1)) {
			fRec99[l106] = 0.0f;
			
		}
		for (int l107 = 0; (l107 < 3); l107 = (l107 + 1)) {
			fRec98[l107] = 0.0f;
			
		}
		for (int l108 = 0; (l108 < 3); l108 = (l108 + 1)) {
			fRec97[l108] = 0.0f;
			
		}
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual flt_eq10* clone() {
		return new flt_eq10();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("flt_eq10");
		ui_interface->declare(&fVslider2, "unit", "dB");
		ui_interface->addVerticalSlider("f1000", &fVslider2, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider7, "unit", "dB");
		ui_interface->addVerticalSlider("f125", &fVslider7, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider5, "unit", "dB");
		ui_interface->addVerticalSlider("f16000", &fVslider5, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider3, "unit", "dB");
		ui_interface->addVerticalSlider("f2000", &fVslider3, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider0, "unit", "dB");
		ui_interface->addVerticalSlider("f250", &fVslider0, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider9, "unit", "dB");
		ui_interface->addVerticalSlider("f31", &fVslider9, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider6, "unit", "dB");
		ui_interface->addVerticalSlider("f4000", &fVslider6, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider1, "unit", "dB");
		ui_interface->addVerticalSlider("f500", &fVslider1, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider8, "unit", "dB");
		ui_interface->addVerticalSlider("f62", &fVslider8, -10.0f, -70.0f, 10.0f, 0.100000001f);
		ui_interface->declare(&fVslider4, "unit", "dB");
		ui_interface->addVerticalSlider("f8000", &fVslider4, -10.0f, -70.0f, 10.0f, 0.100000001f);
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
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec0[0] = (fSlow0 + (0.999000013f * fRec0[1]));
			float fTemp0 = float(input0[i]);
			fVec0[0] = fTemp0;
			fRec23[0] = ((fConst50 * (fTemp0 + fVec0[1])) + (fConst51 * fRec23[1]));
			fRec22[0] = (fRec23[0] - (fConst48 * ((fConst52 * fRec22[2]) + (fConst54 * fRec22[1]))));
			fRec21[0] = ((fConst48 * ((fRec22[2] + (2.0f * fRec22[1])) + fRec22[0])) - (fConst55 * ((fConst54 * fRec21[1]) + (fConst56 * fRec21[2]))));
			float fTemp1 = (2.0f * fRec21[1]);
			float fTemp2 = ((fRec21[2] + fTemp1) + fRec21[0]);
			fVec1[0] = fTemp2;
			fRec20[0] = ((fConst43 * fRec20[1]) + (fConst47 * (fRec21[0] + (fRec21[2] + (fVec1[1] + fTemp1)))));
			fRec19[0] = (fRec20[0] - (fConst41 * ((fConst57 * fRec19[2]) + (fConst59 * fRec19[1]))));
			fRec18[0] = ((fConst41 * (fRec19[0] + (fRec19[2] + (2.0f * fRec19[1])))) - (fConst60 * ((fConst59 * fRec18[1]) + (fConst61 * fRec18[2]))));
			float fTemp3 = (fRec18[0] + (fRec18[2] + (2.0f * fRec18[1])));
			fVec2[0] = fTemp3;
			fRec17[0] = ((fConst40 * (fTemp3 + fVec2[1])) + (fConst62 * fRec17[1]));
			fRec16[0] = (fRec17[0] - (fConst35 * ((fConst63 * fRec16[2]) + (fConst65 * fRec16[1]))));
			fRec15[0] = ((fConst35 * ((fRec16[2] + (2.0f * fRec16[1])) + fRec16[0])) - (fConst66 * ((fConst65 * fRec15[1]) + (fConst67 * fRec15[2]))));
			float fTemp4 = (2.0f * fRec15[1]);
			float fTemp5 = ((fRec15[2] + fTemp4) + fRec15[0]);
			fVec3[0] = fTemp5;
			fRec14[0] = ((fConst30 * fRec14[1]) + (fConst34 * (fRec15[0] + (fRec15[2] + (fVec3[1] + fTemp4)))));
			fRec13[0] = (fRec14[0] - (fConst28 * ((fConst68 * fRec13[2]) + (fConst70 * fRec13[1]))));
			fRec12[0] = ((fConst28 * (fRec13[0] + (fRec13[2] + (2.0f * fRec13[1])))) - (fConst71 * ((fConst70 * fRec12[1]) + (fConst72 * fRec12[2]))));
			float fTemp6 = (fRec12[0] + (fRec12[2] + (2.0f * fRec12[1])));
			fVec4[0] = fTemp6;
			fRec11[0] = ((fConst23 * fRec11[1]) + (fConst27 * (fTemp6 + fVec4[1])));
			fRec10[0] = (fRec11[0] - (fConst21 * ((fConst73 * fRec10[2]) + (fConst75 * fRec10[1]))));
			fRec9[0] = ((fConst21 * (fRec10[0] + (fRec10[2] + (2.0f * fRec10[1])))) - (fConst76 * ((fConst75 * fRec9[1]) + (fConst77 * fRec9[2]))));
			float fTemp7 = (fRec9[0] + (fRec9[2] + (2.0f * fRec9[1])));
			fVec5[0] = fTemp7;
			fRec8[0] = ((fConst16 * fRec8[1]) + (fConst20 * (fTemp7 + fVec5[1])));
			fRec7[0] = (fRec8[0] - (fConst14 * ((fConst78 * fRec7[2]) + (fConst80 * fRec7[1]))));
			fRec6[0] = ((fConst14 * (fRec7[0] + (fRec7[2] + (2.0f * fRec7[1])))) - (fConst81 * ((fConst80 * fRec6[1]) + (fConst82 * fRec6[2]))));
			float fTemp8 = (fRec6[0] + (fRec6[2] + (2.0f * fRec6[1])));
			fVec6[0] = fTemp8;
			fRec5[0] = ((fConst9 * fRec5[1]) + (fConst13 * ((fConst2 * fTemp8) + (fConst83 * fVec6[1]))));
			fRec4[0] = (fRec5[0] - (fConst7 * ((fConst84 * fRec4[2]) + (fConst85 * fRec4[1]))));
			fRec3[0] = ((fConst7 * ((fConst6 * fRec4[1]) + (fConst5 * (fRec4[2] + fRec4[0])))) - (fConst4 * ((fConst85 * fRec3[1]) + (fConst86 * fRec3[2]))));
			float fTemp9 = (fConst91 * fRec2[1]);
			fRec2[0] = ((fConst4 * ((fConst6 * fRec3[1]) + (fConst5 * (fRec3[2] + fRec3[0])))) - (fConst89 * (fTemp9 + (fConst92 * fRec2[2]))));
			float fTemp10 = (fConst98 * fRec1[1]);
			fRec1[0] = ((fRec2[2] + (fConst89 * (fTemp9 + (fConst92 * fRec2[0])))) - (fConst95 * ((fConst96 * fRec1[2]) + fTemp10)));
			fRec24[0] = (fSlow1 + (0.999000013f * fRec24[1]));
			fRec30[0] = ((fConst16 * fRec30[1]) + (fConst20 * ((fConst11 * fTemp7) + (fConst100 * fVec5[1]))));
			fRec29[0] = (fRec30[0] - (fConst14 * ((fConst78 * fRec29[2]) + (fConst80 * fRec29[1]))));
			fRec28[0] = ((fConst14 * ((fConst99 * fRec29[1]) + (fConst79 * (fRec29[2] + fRec29[0])))) - (fConst81 * ((fConst80 * fRec28[1]) + (fConst82 * fRec28[2]))));
			float fTemp11 = (fConst91 * fRec27[1]);
			fRec27[0] = ((fConst81 * ((fConst99 * fRec28[1]) + (fConst79 * (fRec28[2] + fRec28[0])))) - (fConst89 * (fTemp11 + (fConst92 * fRec27[2]))));
			float fTemp12 = (fConst98 * fRec26[1]);
			fRec26[0] = ((fRec27[2] + (fConst89 * (fTemp11 + (fConst92 * fRec27[0])))) - (fConst95 * ((fConst96 * fRec26[2]) + fTemp12)));
			float fTemp13 = (fConst85 * fRec25[1]);
			fRec25[0] = ((fRec26[2] + (fConst95 * (fTemp12 + (fConst96 * fRec26[0])))) - (fConst101 * ((fConst102 * fRec25[2]) + fTemp13)));
			fRec31[0] = (fSlow2 + (0.999000013f * fRec31[1]));
			fRec38[0] = ((fConst23 * fRec38[1]) + (fConst27 * ((fConst18 * fTemp6) + (fConst104 * fVec4[1]))));
			fRec37[0] = (fRec38[0] - (fConst21 * ((fConst73 * fRec37[2]) + (fConst75 * fRec37[1]))));
			fRec36[0] = ((fConst21 * ((fConst103 * fRec37[1]) + (fConst74 * (fRec37[2] + fRec37[0])))) - (fConst76 * ((fConst75 * fRec36[1]) + (fConst77 * fRec36[2]))));
			float fTemp14 = (fConst91 * fRec35[1]);
			fRec35[0] = ((fConst76 * ((fConst103 * fRec36[1]) + (fConst74 * (fRec36[2] + fRec36[0])))) - (fConst89 * (fTemp14 + (fConst92 * fRec35[2]))));
			float fTemp15 = (fConst98 * fRec34[1]);
			fRec34[0] = ((fRec35[2] + (fConst89 * (fTemp14 + (fConst92 * fRec35[0])))) - (fConst95 * ((fConst96 * fRec34[2]) + fTemp15)));
			float fTemp16 = (fConst85 * fRec33[1]);
			fRec33[0] = ((fRec34[2] + (fConst95 * (fTemp15 + (fConst96 * fRec34[0])))) - (fConst101 * ((fConst102 * fRec33[2]) + fTemp16)));
			float fTemp17 = (fConst80 * fRec32[1]);
			fRec32[0] = ((fRec33[2] + (fConst101 * (fTemp16 + (fConst102 * fRec33[0])))) - (fConst105 * ((fConst106 * fRec32[2]) + fTemp17)));
			fRec39[0] = (fSlow3 + (0.999000013f * fRec39[1]));
			fRec47[0] = ((fConst34 * ((fConst25 * fTemp5) + (fConst107 * fVec3[1]))) + (fConst30 * fRec47[1]));
			fRec46[0] = (fRec47[0] - (fConst28 * ((fConst68 * fRec46[2]) + (fConst70 * fRec46[1]))));
			fRec45[0] = ((fConst28 * ((fConst69 * (fRec46[2] + fRec46[0])) + (fConst108 * fRec46[1]))) - (fConst71 * ((fConst70 * fRec45[1]) + (fConst72 * fRec45[2]))));
			float fTemp18 = (fConst91 * fRec44[1]);
			fRec44[0] = ((fConst71 * ((fConst69 * (fRec45[2] + fRec45[0])) + (fConst108 * fRec45[1]))) - (fConst89 * (fTemp18 + (fConst92 * fRec44[2]))));
			float fTemp19 = (fConst98 * fRec43[1]);
			fRec43[0] = ((fRec44[2] + (fConst89 * (fTemp18 + (fConst92 * fRec44[0])))) - (fConst95 * ((fConst96 * fRec43[2]) + fTemp19)));
			float fTemp20 = (fConst85 * fRec42[1]);
			fRec42[0] = ((fRec43[2] + (fConst95 * (fTemp19 + (fConst96 * fRec43[0])))) - (fConst101 * ((fConst102 * fRec42[2]) + fTemp20)));
			float fTemp21 = (fConst80 * fRec41[1]);
			fRec41[0] = ((fRec42[2] + (fConst101 * (fTemp20 + (fConst102 * fRec42[0])))) - (fConst105 * ((fConst106 * fRec41[2]) + fTemp21)));
			float fTemp22 = (fConst75 * fRec40[1]);
			fRec40[0] = ((fRec41[2] + (fConst105 * (fTemp21 + (fConst106 * fRec41[0])))) - (fConst109 * (fTemp22 + (fConst110 * fRec40[2]))));
			fRec48[0] = (fSlow4 + (0.999000013f * fRec48[1]));
			fRec58[0] = ((fConst47 * ((fConst37 * fTemp2) + (fConst111 * fVec1[1]))) + (fConst43 * fRec58[1]));
			fRec57[0] = (fRec58[0] - (fConst41 * ((fConst57 * fRec57[2]) + (fConst59 * fRec57[1]))));
			fRec56[0] = ((fConst41 * ((fConst58 * (fRec57[2] + fRec57[0])) + (fConst112 * fRec57[1]))) - (fConst60 * ((fConst59 * fRec56[1]) + (fConst61 * fRec56[2]))));
			float fTemp23 = (fConst91 * fRec55[1]);
			fRec55[0] = ((fConst60 * ((fConst58 * (fRec56[2] + fRec56[0])) + (fConst112 * fRec56[1]))) - (fConst89 * (fTemp23 + (fConst92 * fRec55[2]))));
			float fTemp24 = (fConst98 * fRec54[1]);
			fRec54[0] = (((fConst89 * (fTemp23 + (fConst92 * fRec55[0]))) + fRec55[2]) - (fConst95 * ((fConst96 * fRec54[2]) + fTemp24)));
			float fTemp25 = (fConst85 * fRec53[1]);
			fRec53[0] = ((fRec54[2] + (fConst95 * (fTemp24 + (fConst96 * fRec54[0])))) - (fConst101 * ((fConst102 * fRec53[2]) + fTemp25)));
			float fTemp26 = (fConst80 * fRec52[1]);
			fRec52[0] = ((fRec53[2] + (fConst101 * (fTemp25 + (fConst102 * fRec53[0])))) - (fConst105 * ((fConst106 * fRec52[2]) + fTemp26)));
			float fTemp27 = (fConst75 * fRec51[1]);
			fRec51[0] = ((fRec52[2] + (fConst105 * (fTemp26 + (fConst106 * fRec52[0])))) - (fConst109 * (fTemp27 + (fConst110 * fRec51[2]))));
			float fTemp28 = (fConst70 * fRec50[1]);
			fRec50[0] = ((fRec51[2] + (fConst109 * (fTemp27 + (fConst110 * fRec51[0])))) - (fConst113 * (fTemp28 + (fConst114 * fRec50[2]))));
			float fTemp29 = (fConst65 * fRec49[1]);
			fRec49[0] = ((fRec50[2] + (fConst113 * (fTemp28 + (fConst114 * fRec50[0])))) - (fConst115 * (fTemp29 + (fConst116 * fRec49[2]))));
			fRec59[0] = (fSlow5 + (0.999000013f * fRec59[1]));
			fRec70[0] = ((fConst50 * ((fConst45 * fTemp0) + (fConst118 * fVec0[1]))) + (fConst51 * fRec70[1]));
			fRec69[0] = (fRec70[0] - (fConst48 * ((fConst52 * fRec69[2]) + (fConst54 * fRec69[1]))));
			fRec68[0] = ((fConst48 * ((fConst117 * fRec69[1]) + (fConst53 * (fRec69[2] + fRec69[0])))) - (fConst55 * ((fConst54 * fRec68[1]) + (fConst56 * fRec68[2]))));
			float fTemp30 = (fConst91 * fRec67[1]);
			fRec67[0] = ((fConst55 * ((fConst117 * fRec68[1]) + (fConst53 * (fRec68[2] + fRec68[0])))) - (fConst89 * (fTemp30 + (fConst92 * fRec67[2]))));
			float fTemp31 = (fConst98 * fRec66[1]);
			fRec66[0] = ((fRec67[2] + (fConst89 * (fTemp30 + (fConst92 * fRec67[0])))) - (fConst95 * ((fConst96 * fRec66[2]) + fTemp31)));
			float fTemp32 = (fConst85 * fRec65[1]);
			fRec65[0] = ((fRec66[2] + (fConst95 * (fTemp31 + (fConst96 * fRec66[0])))) - (fConst101 * ((fConst102 * fRec65[2]) + fTemp32)));
			float fTemp33 = (fConst80 * fRec64[1]);
			fRec64[0] = ((fRec65[2] + (fConst101 * (fTemp32 + (fConst102 * fRec65[0])))) - (fConst105 * ((fConst106 * fRec64[2]) + fTemp33)));
			float fTemp34 = (fConst75 * fRec63[1]);
			fRec63[0] = ((fRec64[2] + (fConst105 * (fTemp33 + (fConst106 * fRec64[0])))) - (fConst109 * (fTemp34 + (fConst110 * fRec63[2]))));
			float fTemp35 = (fConst70 * fRec62[1]);
			fRec62[0] = ((fRec63[2] + (fConst109 * (fTemp34 + (fConst110 * fRec63[0])))) - (fConst113 * (fTemp35 + (fConst114 * fRec62[2]))));
			float fTemp36 = (fConst65 * fRec61[1]);
			fRec61[0] = ((fRec62[2] + (fConst113 * (fTemp35 + (fConst114 * fRec62[0])))) - (fConst115 * (fTemp36 + (fConst116 * fRec61[2]))));
			float fTemp37 = (fConst59 * fRec60[1]);
			fRec60[0] = ((fRec61[2] + (fConst115 * (fTemp36 + (fConst116 * fRec61[0])))) - (fConst119 * (fTemp37 + (fConst120 * fRec60[2]))));
			fRec79[0] = ((fConst62 * fRec79[1]) + (fConst40 * ((fConst32 * fTemp3) + (fConst121 * fVec2[1]))));
			fRec78[0] = (fRec79[0] - (fConst35 * ((fConst63 * fRec78[2]) + (fConst65 * fRec78[1]))));
			fRec77[0] = ((fConst35 * ((fConst64 * (fRec78[2] + fRec78[0])) + (fConst122 * fRec78[1]))) - (fConst66 * ((fConst65 * fRec77[1]) + (fConst67 * fRec77[2]))));
			float fTemp38 = (fConst91 * fRec76[1]);
			fRec76[0] = ((fConst66 * ((fConst64 * (fRec77[2] + fRec77[0])) + (fConst122 * fRec77[1]))) - (fConst89 * (fTemp38 + (fConst92 * fRec76[2]))));
			float fTemp39 = (fConst98 * fRec75[1]);
			fRec75[0] = (((fConst89 * (fTemp38 + (fConst92 * fRec76[0]))) + fRec76[2]) - (fConst95 * ((fConst96 * fRec75[2]) + fTemp39)));
			float fTemp40 = (fConst85 * fRec74[1]);
			fRec74[0] = ((fRec75[2] + (fConst95 * (fTemp39 + (fConst96 * fRec75[0])))) - (fConst101 * ((fConst102 * fRec74[2]) + fTemp40)));
			float fTemp41 = (fConst80 * fRec73[1]);
			fRec73[0] = ((fRec74[2] + (fConst101 * (fTemp40 + (fConst102 * fRec74[0])))) - (fConst105 * ((fConst106 * fRec73[2]) + fTemp41)));
			float fTemp42 = (fConst75 * fRec72[1]);
			fRec72[0] = ((fRec73[2] + (fConst105 * (fTemp41 + (fConst106 * fRec73[0])))) - (fConst109 * (fTemp42 + (fConst110 * fRec72[2]))));
			float fTemp43 = (fConst70 * fRec71[1]);
			fRec71[0] = ((fRec72[2] + (fConst109 * (fTemp42 + (fConst110 * fRec72[0])))) - (fConst113 * (fTemp43 + (fConst114 * fRec71[2]))));
			fRec80[0] = (fSlow6 + (0.999000013f * fRec80[1]));
			fRec81[0] = (fSlow7 + (0.999000013f * fRec81[1]));
			fRec88[0] = ((fConst9 * fRec88[1]) + (fConst13 * (fTemp8 + fVec6[1])));
			fRec87[0] = (fRec88[0] - (fConst7 * ((fConst84 * fRec87[2]) + (fConst85 * fRec87[1]))));
			fRec86[0] = ((fConst7 * (fRec87[0] + (fRec87[2] + (2.0f * fRec87[1])))) - (fConst4 * ((fConst85 * fRec86[1]) + (fConst86 * fRec86[2]))));
			float fTemp44 = (fRec86[0] + (fRec86[2] + (2.0f * fRec86[1])));
			fVec7[0] = fTemp44;
			fRec85[0] = ((fConst128 * fRec85[1]) + (fConst129 * ((fConst94 * fTemp44) + (fConst130 * fVec7[1]))));
			fRec84[0] = (fRec85[0] - (fConst126 * ((fConst131 * fRec84[2]) + (fConst98 * fRec84[1]))));
			fRec83[0] = ((fConst126 * ((fConst125 * fRec84[1]) + (fConst97 * (fRec84[2] + fRec84[0])))) - (fConst124 * ((fConst98 * fRec83[1]) + (fConst132 * fRec83[2]))));
			float fTemp45 = (fConst91 * fRec82[1]);
			fRec82[0] = ((fConst124 * ((fConst125 * fRec83[1]) + (fConst97 * (fRec83[2] + fRec83[0])))) - (fConst89 * (fTemp45 + (fConst92 * fRec82[2]))));
			fRec89[0] = (fSlow8 + (0.999000013f * fRec89[1]));
			fRec95[0] = ((fConst128 * fRec95[1]) + (fConst129 * (fTemp44 + fVec7[1])));
			fRec94[0] = (fRec95[0] - (fConst126 * ((fConst131 * fRec94[2]) + (fConst98 * fRec94[1]))));
			fRec93[0] = ((fConst126 * (fRec94[0] + (fRec94[2] + (2.0f * fRec94[1])))) - (fConst124 * ((fConst98 * fRec93[1]) + (fConst132 * fRec93[2]))));
			float fTemp46 = (fRec93[0] + (fRec93[2] + (2.0f * fRec93[1])));
			fVec8[0] = fTemp46;
			fRec92[0] = ((fConst137 * fRec92[1]) + (fConst138 * ((fConst88 * fTemp46) + (fConst139 * fVec8[1]))));
			fRec91[0] = (fRec92[0] - (fConst135 * ((fConst140 * fRec91[2]) + (fConst91 * fRec91[1]))));
			fRec90[0] = ((fConst135 * ((fConst134 * fRec91[1]) + (fConst90 * (fRec91[2] + fRec91[0])))) - (fConst133 * ((fConst91 * fRec90[1]) + (fConst141 * fRec90[2]))));
			fRec96[0] = (fSlow9 + (0.999000013f * fRec96[1]));
			fRec99[0] = ((fConst137 * fRec99[1]) + (fConst138 * (fTemp46 + fVec8[1])));
			fRec98[0] = (fRec99[0] - (fConst135 * ((fConst140 * fRec98[2]) + (fConst91 * fRec98[1]))));
			fRec97[0] = ((fConst135 * (fRec98[0] + (fRec98[2] + (2.0f * fRec98[1])))) - (fConst133 * ((fConst91 * fRec97[1]) + (fConst141 * fRec97[2]))));
			output0[i] = FAUSTFLOAT((((std::pow(10.0f, (0.0500000007f * fRec0[0])) * (fRec1[2] + (fConst95 * (fTemp10 + (fConst96 * fRec1[0]))))) + ((std::pow(10.0f, (0.0500000007f * fRec24[0])) * (fRec25[2] + (fConst101 * (fTemp13 + (fConst102 * fRec25[0]))))) + ((std::pow(10.0f, (0.0500000007f * fRec31[0])) * (fRec32[2] + (fConst105 * (fTemp17 + (fConst106 * fRec32[0]))))) + ((std::pow(10.0f, (0.0500000007f * fRec39[0])) * (fRec40[2] + (fConst109 * (fTemp22 + (fConst110 * fRec40[0]))))) + ((std::pow(10.0f, (0.0500000007f * fRec48[0])) * (fRec49[2] + (fConst115 * (fTemp29 + (fConst116 * fRec49[0]))))) + ((std::pow(10.0f, (0.0500000007f * fRec59[0])) * (fRec60[2] + (fConst119 * (fTemp37 + (fConst120 * fRec60[0]))))) + ((((fConst113 * (fTemp43 + (fConst114 * fRec71[0]))) + fRec71[2]) * std::pow(10.0f, (0.0500000007f * fRec80[0]))) + (std::pow(10.0f, (0.0500000007f * fRec81[0])) * (fRec82[2] + (fConst89 * (fTemp45 + (fConst92 * fRec82[0])))))))))))) + (fConst133 * ((std::pow(10.0f, (0.0500000007f * fRec89[0])) * ((fConst134 * fRec90[1]) + (fConst90 * (fRec90[2] + fRec90[0])))) + (std::pow(10.0f, (0.0500000007f * fRec96[0])) * (fRec97[0] + (fRec97[2] + (2.0f * fRec97[1]))))))));
			fRec0[1] = fRec0[0];
			fVec0[1] = fVec0[0];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fVec1[1] = fVec1[0];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fVec2[1] = fVec2[0];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fVec3[1] = fVec3[0];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fVec4[1] = fVec4[0];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fVec5[1] = fVec5[0];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fVec6[1] = fVec6[0];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fRec24[1] = fRec24[0];
			fRec30[1] = fRec30[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec31[1] = fRec31[0];
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
			fRec39[1] = fRec39[0];
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
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fRec48[1] = fRec48[0];
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
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fRec51[2] = fRec51[1];
			fRec51[1] = fRec51[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec59[1] = fRec59[0];
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
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec76[2] = fRec76[1];
			fRec76[1] = fRec76[0];
			fRec75[2] = fRec75[1];
			fRec75[1] = fRec75[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec72[2] = fRec72[1];
			fRec72[1] = fRec72[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec80[1] = fRec80[0];
			fRec81[1] = fRec81[0];
			fRec88[1] = fRec88[0];
			fRec87[2] = fRec87[1];
			fRec87[1] = fRec87[0];
			fRec86[2] = fRec86[1];
			fRec86[1] = fRec86[0];
			fVec7[1] = fVec7[0];
			fRec85[1] = fRec85[0];
			fRec84[2] = fRec84[1];
			fRec84[1] = fRec84[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec89[1] = fRec89[0];
			fRec95[1] = fRec95[0];
			fRec94[2] = fRec94[1];
			fRec94[1] = fRec94[0];
			fRec93[2] = fRec93[1];
			fRec93[1] = fRec93[0];
			fVec8[1] = fVec8[0];
			fRec92[1] = fRec92[0];
			fRec91[2] = fRec91[1];
			fRec91[1] = fRec91[0];
			fRec90[2] = fRec90[1];
			fRec90[1] = fRec90[0];
			fRec96[1] = fRec96[0];
			fRec99[1] = fRec99[0];
			fRec98[2] = fRec98[1];
			fRec98[1] = fRec98[0];
			fRec97[2] = fRec97[1];
			fRec97[1] = fRec97[0];
			
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
