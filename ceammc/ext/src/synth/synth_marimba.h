/* ------------------------------------------------------------
name: "synth.marimba"
Code generated with Faust 2.28.6 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __synth_marimba_H__
#define  __synth_marimba_H__

// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

/************************** BEGIN synth_marimba_dsp.h **************************/
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

class synth_marimba_dsp {

    public:

        synth_marimba_dsp() {}
        virtual ~synth_marimba_dsp() {}

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
        virtual synth_marimba_dsp* clone() = 0;
    
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

class decorator_dsp : public synth_marimba_dsp {

    protected:

        synth_marimba_dsp* fDSP;

    public:

        decorator_dsp(synth_marimba_dsp* synth_marimba_dsp = nullptr):fDSP(synth_marimba_dsp) {}
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
    
        virtual synth_marimba_dsp* createDSPInstance() = 0;
    
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
/**************************  END  synth_marimba_dsp.h **************************/
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
struct synth_marimba : public synth_marimba_dsp {
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

const static float fsynth_marimbaSIG0Wave0[250] = {1.0f,0.776724994f,0.625723004f,0.855223f,0.760159016f,0.69837302f,0.768010974f,0.64112699f,0.244034007f,0.707754016f,0.634012997f,0.247527003f,0.660848975f,0.450396001f,0.567782998f,0.106361002f,0.716813982f,0.663919985f,0.291207999f,0.310598999f,0.801495016f,0.635291994f,0.307435006f,0.874123991f,0.497667998f,0.487087995f,0.459114999f,0.733455002f,0.541818023f,0.441318005f,0.313919991f,0.40309f,0.685352981f,0.603139997f,0.400552005f,0.453511f,0.634386003f,0.291547f,0.131604999f,0.368506998f,0.839906991f,0.602159977f,0.288296014f,0.579670012f,0.0242493004f,0.262746006f,0.368588001f,0.890284002f,0.408962995f,0.556071997f,0.884427011f,0.832109988f,0.612015009f,0.757175982f,0.919476986f,1.0f,0.827962995f,0.89240998f,0.0357408002f,0.480789006f,0.75287199f,0.0546301007f,0.235936999f,0.362937987f,0.444471985f,0.101751f,0.703418016f,0.453135997f,0.316628993f,0.490393996f,0.982508004f,0.551621974f,0.602008998f,0.666957021f,0.776830018f,0.905662f,0.0987197012f,0.402967989f,0.829451978f,0.307644993f,0.640479982f,0.983971f,0.584204972f,0.650364995f,0.334446996f,0.583570004f,0.540190995f,0.672533989f,0.245711997f,0.687298f,0.883058012f,0.792949975f,0.600619018f,0.572682023f,0.122612f,0.388247997f,0.290657997f,0.380255014f,0.290966988f,0.567818999f,0.0737721026f,0.42098999f,0.0786577985f,0.393994987f,0.268983006f,0.260614008f,0.494085997f,0.238025993f,0.0987823978f,0.277879f,0.440562993f,0.0770211965f,0.450590998f,0.128137007f,0.0368275009f,0.128699005f,0.329605013f,0.374511987f,0.363590002f,0.272594005f,0.379052013f,0.305240989f,0.0741128996f,0.34572801f,0.299349993f,0.221284002f,0.0261390992f,0.293202013f,0.361885011f,0.114330001f,0.239004999f,0.434156001f,0.329582989f,0.219459996f,0.284175009f,0.198554993f,0.431975991f,0.302985013f,1.0f,0.146220997f,0.140700996f,0.264243007f,0.185996994f,0.426322013f,0.304780006f,0.343989998f,0.195429996f,0.386954993f,0.187600002f,0.172812f,0.0434115008f,0.303761005f,0.0694539994f,0.453943014f,0.832450986f,0.317817003f,0.940600991f,1.0f,0.180657998f,0.737921f,0.832297027f,0.402352005f,0.126785994f,0.594398022f,0.485455006f,0.324470013f,0.365101993f,0.777921975f,0.588271976f,0.401353002f,0.610734999f,0.158693001f,0.0746072009f,0.825098991f,0.925459027f,0.65377003f,0.260791987f,0.719384015f,0.559907973f,0.372590005f,0.360035002f,0.622938991f,0.210271001f,0.444595009f,0.311286002f,0.464309007f,0.557231009f,0.524079978f,0.0701055974f,0.320749015f,0.194460005f,0.727608979f,0.522062004f,0.394003987f,0.235035002f,0.395646006f,0.494796008f,0.51731801f,0.109751999f,0.69284898f,0.00632009003f,0.0207582992f,0.00306107011f,0.0637191013f,0.0816610008f,0.0351100005f,0.127813995f,0.202294007f,0.0764145032f,0.263126999f,0.400198996f,0.267277986f,0.633385003f,1.0f,0.73990202f,0.413762987f,0.418110013f,0.612715006f,0.67237401f,0.339673996f,0.211720005f,0.459645003f,0.102499999f,0.325890005f,0.148154005f,0.265442014f,0.0974304974f,0.286437988f,0.275213003f,0.109111004f,0.575088978f,0.370283008f,0.29411f,0.259826005f,0.0648718998f,0.583418012f,0.282662988f,0.182004005f,0.117421001f,0.417726994f,0.169650003f,0.24853f,0.122818999f,0.185486004f,0.0433618017f,0.373849005f,0.25276801f,0.195103005f,0.0927835032f,0.166543007f};
class synth_marimbaSIG0 {
	
  private:
	
	int fsynth_marimbaSIG0Wave0_idx;
	
  public:
	
	int getNumInputssynth_marimbaSIG0() {
		return 0;
	}
	int getNumOutputssynth_marimbaSIG0() {
		return 1;
	}
	int getInputRatesynth_marimbaSIG0(int channel) {
		int rate;
		switch ((channel)) {
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	int getOutputRatesynth_marimbaSIG0(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 0;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	void instanceInitsynth_marimbaSIG0(int sample_rate) {
		fsynth_marimbaSIG0Wave0_idx = 0;
	}
	
	void fillsynth_marimbaSIG0(int count, float* table) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			table[i] = fsynth_marimbaSIG0Wave0[fsynth_marimbaSIG0Wave0_idx];
			fsynth_marimbaSIG0Wave0_idx = ((1 + fsynth_marimbaSIG0Wave0_idx) % 250);
		}
	}

};

static synth_marimbaSIG0* newsynth_marimbaSIG0() { return (synth_marimbaSIG0*)new synth_marimbaSIG0(); }
static void deletesynth_marimbaSIG0(synth_marimbaSIG0* dsp) { delete dsp; }

static float synth_marimba_faustpower2_f(float value) {
	return (value * value);
}
static float ftbl0synth_marimbaSIG0[250];

#ifndef FAUSTCLASS 
#define FAUSTCLASS synth_marimba
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class synth_marimba : public synth_marimba_dsp {
	
 private:
	
	int iRec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fRec4[2];
	int IOTA;
	float fVec0[2048];
	float fConst2;
	float fRec2[2];
	float fRec5[2];
	FAUSTFLOAT fHslider1;
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	int iRec9[2];
	float fConst9;
	float fConst10;
	float fRec8[3];
	float fConst11;
	float fRec7[3];
	float fConst12;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fButton0;
	float fVec1[2];
	int iRec10[2];
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fRec6[3];
	float fConst17;
	FAUSTFLOAT fHslider4;
	float fConst18;
	float fConst19;
	float fConst20;
	float fConst21;
	float fRec11[3];
	float fConst22;
	float fConst23;
	float fConst24;
	float fConst25;
	float fRec12[3];
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fRec13[3];
	float fConst30;
	float fConst31;
	float fConst32;
	float fConst33;
	float fRec14[3];
	float fConst34;
	float fConst35;
	float fConst36;
	float fConst37;
	float fRec15[3];
	float fConst38;
	float fConst39;
	float fConst40;
	float fConst41;
	float fRec16[3];
	float fConst42;
	float fConst43;
	float fConst44;
	float fConst45;
	float fRec17[3];
	float fConst46;
	float fConst47;
	float fConst48;
	float fConst49;
	float fRec18[3];
	float fConst50;
	float fConst51;
	float fConst52;
	float fConst53;
	float fRec19[3];
	float fConst54;
	float fConst55;
	float fConst56;
	float fConst57;
	float fRec20[3];
	float fConst58;
	float fConst59;
	float fConst60;
	float fConst61;
	float fRec21[3];
	float fConst62;
	float fConst63;
	float fConst64;
	float fConst65;
	float fRec22[3];
	float fConst66;
	float fConst67;
	float fConst68;
	float fConst69;
	float fRec23[3];
	float fConst70;
	float fConst71;
	float fConst72;
	float fConst73;
	float fRec24[3];
	float fConst74;
	float fConst75;
	float fConst76;
	float fConst77;
	float fRec25[3];
	float fConst78;
	float fConst79;
	float fConst80;
	float fConst81;
	float fRec26[3];
	float fConst82;
	float fConst83;
	float fConst84;
	float fConst85;
	float fRec27[3];
	float fConst86;
	float fConst87;
	float fConst88;
	float fConst89;
	float fRec28[3];
	float fConst90;
	float fConst91;
	float fConst92;
	float fConst93;
	float fRec29[3];
	float fConst94;
	float fConst95;
	float fConst96;
	float fConst97;
	float fRec30[3];
	float fConst98;
	float fConst99;
	float fConst100;
	float fConst101;
	float fRec31[3];
	float fConst102;
	float fConst103;
	float fConst104;
	float fConst105;
	float fRec32[3];
	float fConst106;
	float fConst107;
	float fConst108;
	float fConst109;
	float fRec33[3];
	float fConst110;
	float fConst111;
	float fConst112;
	float fConst113;
	float fRec34[3];
	float fConst114;
	float fConst115;
	float fConst116;
	float fConst117;
	float fRec35[3];
	float fConst118;
	float fConst119;
	float fConst120;
	float fConst121;
	float fRec36[3];
	float fConst122;
	float fConst123;
	float fConst124;
	float fConst125;
	float fRec37[3];
	float fConst126;
	float fConst127;
	float fConst128;
	float fConst129;
	float fRec38[3];
	float fConst130;
	float fConst131;
	float fConst132;
	float fConst133;
	float fRec39[3];
	float fConst134;
	float fConst135;
	float fConst136;
	float fConst137;
	float fRec40[3];
	float fConst138;
	float fConst139;
	float fConst140;
	float fConst141;
	float fRec41[3];
	float fConst142;
	float fConst143;
	float fConst144;
	float fConst145;
	float fRec42[3];
	float fConst146;
	float fConst147;
	float fConst148;
	float fConst149;
	float fRec43[3];
	float fConst150;
	float fConst151;
	float fConst152;
	float fConst153;
	float fRec44[3];
	float fConst154;
	float fConst155;
	float fConst156;
	float fConst157;
	float fRec45[3];
	float fConst158;
	float fConst159;
	float fConst160;
	float fConst161;
	float fRec46[3];
	float fConst162;
	float fConst163;
	float fConst164;
	float fConst165;
	float fRec47[3];
	float fConst166;
	float fConst167;
	float fConst168;
	float fConst169;
	float fRec48[3];
	float fConst170;
	float fConst171;
	float fConst172;
	float fConst173;
	float fRec49[3];
	float fConst174;
	float fConst175;
	float fConst176;
	float fConst177;
	float fRec50[3];
	float fConst178;
	float fConst179;
	float fConst180;
	float fConst181;
	float fRec51[3];
	float fConst182;
	float fConst183;
	float fConst184;
	float fConst185;
	float fRec52[3];
	float fConst186;
	float fConst187;
	float fConst188;
	float fConst189;
	float fRec53[3];
	float fConst190;
	float fConst191;
	float fConst192;
	float fConst193;
	float fRec54[3];
	float fConst194;
	float fConst195;
	float fConst196;
	float fConst197;
	float fRec55[3];
	float fConst198;
	float fConst199;
	float fConst200;
	float fConst201;
	float fRec56[3];
	float fConst202;
	float fConst203;
	float fConst204;
	float fConst205;
	float fRec57[3];
	float fConst206;
	float fConst207;
	float fConst208;
	float fConst209;
	float fRec58[3];
	float fConst210;
	float fConst211;
	float fConst212;
	float fConst213;
	float fRec59[3];
	float fVec2[2];
	float fVec3[2048];
	float fRec3[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.1");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "synth_marimba.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
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
		m->declare("name", "synth.marimba");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("spn.lib/name", "Standart Pitch Notation constants");
		m->declare("spn.lib/version", "0.1");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
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
		synth_marimbaSIG0* sig0 = newsynth_marimbaSIG0();
		sig0->instanceInitsynth_marimbaSIG0(sample_rate);
		sig0->fillsynth_marimbaSIG0(250, ftbl0synth_marimbaSIG0);
		deletesynth_marimbaSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (0.00147058826f * fConst0);
		fConst2 = (0.00882352982f * fConst0);
		fConst3 = (3.14159274f / fConst0);
		fConst4 = std::tan((31.415926f / fConst0));
		fConst5 = (1.0f / fConst4);
		fConst6 = (1.0f / (((fConst5 + 1.41421354f) / fConst4) + 1.0f));
		fConst7 = synth_marimba_faustpower2_f(fConst4);
		fConst8 = (1.0f / fConst7);
		fConst9 = (((fConst5 + -1.41421354f) / fConst4) + 1.0f);
		fConst10 = (2.0f * (1.0f - fConst8));
		fConst11 = (0.0f - (2.0f / fConst7));
		fConst12 = (0.00200000009f * fConst0);
		fConst13 = std::pow(0.00100000005f, (4549764.5f / fConst0));
		fConst14 = (0.0f - (2.0f * fConst13));
		fConst15 = (501725.406f / fConst0);
		fConst16 = synth_marimba_faustpower2_f(fConst13);
		fConst17 = ((0.5f * fConst0) + -1.0f);
		fConst18 = std::pow(0.00100000005f, (17675.4805f / fConst0));
		fConst19 = (0.0f - (2.0f * fConst18));
		fConst20 = (420310.656f / fConst0);
		fConst21 = synth_marimba_faustpower2_f(fConst18);
		fConst22 = std::pow(0.00100000005f, (13265.4883f / fConst0));
		fConst23 = (0.0f - (2.0f * fConst22));
		fConst24 = (413136.531f / fConst0);
		fConst25 = synth_marimba_faustpower2_f(fConst22);
		fConst26 = std::pow(0.00100000005f, (8048.45801f / fConst0));
		fConst27 = (0.0f - (2.0f * fConst26));
		fConst28 = (399620.375f / fConst0);
		fConst29 = synth_marimba_faustpower2_f(fConst26);
		fConst30 = std::pow(0.00100000005f, (3034.91235f / fConst0));
		fConst31 = (0.0f - (2.0f * fConst30));
		fConst32 = (369010.719f / fConst0);
		fConst33 = synth_marimba_faustpower2_f(fConst30);
		fConst34 = std::pow(0.00100000005f, (2755.63037f / fConst0));
		fConst35 = (0.0f - (2.0f * fConst34));
		fConst36 = (365643.438f / fConst0);
		fConst37 = synth_marimba_faustpower2_f(fConst34);
		fConst38 = std::pow(0.00100000005f, (2311.80176f / fConst0));
		fConst39 = (0.0f - (2.0f * fConst38));
		fConst40 = (359348.438f / fConst0);
		fConst41 = synth_marimba_faustpower2_f(fConst38);
		fConst42 = std::pow(0.00100000005f, (1859.39001f / fConst0));
		fConst43 = (0.0f - (2.0f * fConst42));
		fConst44 = (351228.812f / fConst0);
		fConst45 = synth_marimba_faustpower2_f(fConst42);
		fConst46 = std::pow(0.00100000005f, (1095.63586f / fConst0));
		fConst47 = (0.0f - (2.0f * fConst46));
		fConst48 = (329971.781f / fConst0);
		fConst49 = synth_marimba_faustpower2_f(fConst46);
		fConst50 = std::pow(0.00100000005f, (249.361649f / fConst0));
		fConst51 = (0.0f - (2.0f * fConst50));
		fConst52 = (257014.219f / fConst0);
		fConst53 = synth_marimba_faustpower2_f(fConst50);
		fConst54 = std::pow(0.00100000005f, (216.134277f / fConst0));
		fConst55 = (0.0f - (2.0f * fConst54));
		fConst56 = (248753.312f / fConst0);
		fConst57 = synth_marimba_faustpower2_f(fConst54);
		fConst58 = std::pow(0.00100000005f, (142.914078f / fConst0));
		fConst59 = (0.0f - (2.0f * fConst58));
		fConst60 = (223483.484f / fConst0);
		fConst61 = synth_marimba_faustpower2_f(fConst58);
		fConst62 = std::pow(0.00100000005f, (130.309692f / fConst0));
		fConst63 = (0.0f - (2.0f * fConst62));
		fConst64 = (217552.031f / fConst0);
		fConst65 = synth_marimba_faustpower2_f(fConst62);
		fConst66 = std::pow(0.00100000005f, (109.456856f / fConst0));
		fConst67 = (0.0f - (2.0f * fConst66));
		fConst68 = (206046.031f / fConst0);
		fConst69 = synth_marimba_faustpower2_f(fConst66);
		fConst70 = std::pow(0.00100000005f, (80.8901215f / fConst0));
		fConst71 = (0.0f - (2.0f * fConst70));
		fConst72 = (185114.125f / fConst0);
		fConst73 = synth_marimba_faustpower2_f(fConst70);
		fConst74 = std::pow(0.00100000005f, (2024133.5f / fConst0));
		fConst75 = (0.0f - (2.0f * fConst74));
		fConst76 = (494689.5f / fConst0);
		fConst77 = synth_marimba_faustpower2_f(fConst74);
		fConst78 = std::pow(0.00100000005f, (630229.375f / fConst0));
		fConst79 = (0.0f - (2.0f * fConst78));
		fConst80 = (482323.438f / fConst0);
		fConst81 = synth_marimba_faustpower2_f(fConst78);
		fConst82 = std::pow(0.00100000005f, (355973.344f / fConst0));
		fConst83 = (0.0f - (2.0f * fConst82));
		fConst84 = (475132.719f / fConst0);
		fConst85 = synth_marimba_faustpower2_f(fConst82);
		fConst86 = std::pow(0.00100000005f, (172447.047f / fConst0));
		fConst87 = (0.0f - (2.0f * fConst86));
		fConst88 = (464743.344f / fConst0);
		fConst89 = synth_marimba_faustpower2_f(fConst86);
		fConst90 = std::pow(0.00100000005f, (37577.8438f / fConst0));
		fConst91 = (0.0f - (2.0f * fConst90));
		fConst92 = (437312.969f / fConst0);
		fConst93 = synth_marimba_faustpower2_f(fConst90);
		fConst94 = std::pow(0.00100000005f, (773.212891f / fConst0));
		fConst95 = (0.0f - (2.0f * fConst94));
		fConst96 = (314683.531f / fConst0);
		fConst97 = synth_marimba_faustpower2_f(fConst94);
		fConst98 = std::pow(0.00100000005f, (695.55957f / fConst0));
		fConst99 = (0.0f - (2.0f * fConst98));
		fConst100 = (309826.125f / fConst0);
		fConst101 = synth_marimba_faustpower2_f(fConst98);
		fConst102 = std::pow(0.00100000005f, (638.477783f / fConst0));
		fConst103 = (0.0f - (2.0f * fConst102));
		fConst104 = (305820.219f / fConst0);
		fConst105 = synth_marimba_faustpower2_f(fConst102);
		fConst106 = std::pow(0.00100000005f, (312.009094f / fConst0));
		fConst107 = (0.0f - (2.0f * fConst106));
		fConst108 = (269495.281f / fConst0);
		fConst109 = synth_marimba_faustpower2_f(fConst106);
		fConst110 = std::pow(0.00100000005f, (278.139954f / fConst0));
		fConst111 = (0.0f - (2.0f * fConst110));
		fConst112 = (263166.281f / fConst0);
		fConst113 = synth_marimba_faustpower2_f(fConst110);
		fConst114 = std::pow(0.00100000005f, (11.0386343f / fConst0));
		fConst115 = (0.0f - (2.0f * fConst114));
		fConst116 = (10601.3896f / fConst0);
		fConst117 = synth_marimba_faustpower2_f(fConst114);
		fConst118 = std::pow(0.00100000005f, (10.8901329f / fConst0));
		fConst119 = (0.0f - (2.0f * fConst118));
		fConst120 = (9160.67285f / fConst0);
		fConst121 = synth_marimba_faustpower2_f(fConst118);
		fConst122 = std::pow(0.00100000005f, (10.2591143f / fConst0));
		fConst123 = (0.0f - (2.0f * fConst122));
		fConst124 = (2764.60156f / fConst0);
		fConst125 = synth_marimba_faustpower2_f(fConst122);
		fConst126 = std::pow(0.00100000005f, (12.3379154f / fConst0));
		fConst127 = (0.0f - (2.0f * fConst126));
		fConst128 = (22291.3418f / fConst0);
		fConst129 = synth_marimba_faustpower2_f(fConst126);
		fConst130 = std::pow(0.00100000005f, (12.8027573f / fConst0));
		fConst131 = (0.0f - (2.0f * fConst130));
		fConst132 = (26119.3477f / fConst0);
		fConst133 = synth_marimba_faustpower2_f(fConst130);
		fConst134 = std::pow(0.00100000005f, (14.5330687f / fConst0));
		fConst135 = (0.0f - (2.0f * fConst134));
		fConst136 = (39027.6016f / fConst0);
		fConst137 = synth_marimba_faustpower2_f(fConst134);
		fConst138 = std::pow(0.00100000005f, (16.3652077f / fConst0));
		fConst139 = (0.0f - (2.0f * fConst138));
		fConst140 = (50824.4336f / fConst0);
		fConst141 = synth_marimba_faustpower2_f(fConst138);
		fConst142 = std::pow(0.00100000005f, (17.6309834f / fConst0));
		fConst143 = (0.0f - (2.0f * fConst142));
		fConst144 = (58084.832f / fConst0);
		fConst145 = synth_marimba_faustpower2_f(fConst142);
		fConst146 = std::pow(0.00100000005f, (20.4824104f / fConst0));
		fConst147 = (0.0f - (2.0f * fConst146));
		fConst148 = (72370.3594f / fConst0);
		fConst149 = synth_marimba_faustpower2_f(fConst146);
		fConst150 = std::pow(0.00100000005f, (22.2695656f / fConst0));
		fConst151 = (0.0f - (2.0f * fConst150));
		fConst152 = (80157.9609f / fConst0);
		fConst153 = synth_marimba_faustpower2_f(fConst150);
		fConst154 = std::pow(0.00100000005f, (28.5368156f / fConst0));
		fConst155 = (0.0f - (2.0f * fConst154));
		fConst156 = (102491.523f / fConst0);
		fConst157 = synth_marimba_faustpower2_f(fConst154);
		fConst158 = std::pow(0.00100000005f, (29.2639236f / fConst0));
		fConst159 = (0.0f - (2.0f * fConst158));
		fConst160 = (104696.289f / fConst0);
		fConst161 = synth_marimba_faustpower2_f(fConst158);
		fConst162 = std::pow(0.00100000005f, (31.3811016f / fConst0));
		fConst163 = (0.0f - (2.0f * fConst162));
		fConst164 = (110759.336f / fConst0);
		fConst165 = synth_marimba_faustpower2_f(fConst162);
		fConst166 = std::pow(0.00100000005f, (40.2636795f / fConst0));
		fConst167 = (0.0f - (2.0f * fConst166));
		fConst168 = (131716.406f / fConst0);
		fConst169 = synth_marimba_faustpower2_f(fConst166);
		fConst170 = std::pow(0.00100000005f, (45.1752319f / fConst0));
		fConst171 = (0.0f - (2.0f * fConst170));
		fConst172 = (141047.203f / fConst0);
		fConst173 = synth_marimba_faustpower2_f(fConst170);
		fConst174 = std::pow(0.00100000005f, (47.4399223f / fConst0));
		fConst175 = (0.0f - (2.0f * fConst174));
		fConst176 = (144948.062f / fConst0);
		fConst177 = synth_marimba_faustpower2_f(fConst174);
		fConst178 = std::pow(0.00100000005f, (58.4253006f / fConst0));
		fConst179 = (0.0f - (2.0f * fConst178));
		fConst180 = (161137.562f / fConst0);
		fConst181 = synth_marimba_faustpower2_f(fConst178);
		fConst182 = std::pow(0.00100000005f, (70.9964905f / fConst0));
		fConst183 = (0.0f - (2.0f * fConst182));
		fConst184 = (175686.562f / fConst0);
		fConst185 = synth_marimba_faustpower2_f(fConst182);
		fConst186 = std::pow(0.00100000005f, (76.0668716f / fConst0));
		fConst187 = (0.0f - (2.0f * fConst186));
		fConst188 = (180702.094f / fConst0);
		fConst189 = synth_marimba_faustpower2_f(fConst186);
		fConst190 = std::pow(0.00100000005f, (20142.8574f / fConst0));
		fConst191 = (0.0f - (2.0f * fConst190));
		fConst192 = (423442.969f / fConst0);
		fConst193 = synth_marimba_faustpower2_f(fConst190);
		fConst194 = std::pow(0.00100000005f, (26985.6934f / fConst0));
		fConst195 = (0.0f - (2.0f * fConst194));
		fConst196 = (430163.719f / fConst0);
		fConst197 = synth_marimba_faustpower2_f(fConst194);
		fConst198 = std::pow(0.00100000005f, (22033280.0f / fConst0));
		fConst199 = (0.0f - (2.0f * fConst198));
		fConst200 = (512551.594f / fConst0);
		fConst201 = synth_marimba_faustpower2_f(fConst198);
		fConst202 = std::pow(0.00100000005f, (531223264.0f / fConst0));
		fConst203 = (0.0f - (2.0f * fConst202));
		fConst204 = (526294.438f / fConst0);
		fConst205 = synth_marimba_faustpower2_f(fConst202);
		fConst206 = std::pow(0.00100000005f, (3.8190633e+09f / fConst0));
		fConst207 = (0.0f - (2.0f * fConst206));
		fConst208 = (531328.75f / fConst0);
		fConst209 = synth_marimba_faustpower2_f(fConst206);
		fConst210 = std::pow(0.00100000005f, (1.56573838e+14f / fConst0));
		fConst211 = (0.0f - (2.0f * fConst210));
		fConst212 = (540493.438f / fConst0);
		fConst213 = synth_marimba_faustpower2_f(fConst210);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(48.0f);
		fHslider1 = FAUSTFLOAT(1.0f);
		fHslider2 = FAUSTFLOAT(7000.0f);
		fHslider3 = FAUSTFLOAT(0.25f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iRec0[l0] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec4[l1] = 0.0f;
		}
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2048); l2 = (l2 + 1)) {
			fVec0[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec2[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec5[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iRec9[l5] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 3); l6 = (l6 + 1)) {
			fRec8[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 3); l7 = (l7 + 1)) {
			fRec7[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec1[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			iRec10[l9] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec6[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec11[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec12[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec13[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec14[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec15[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec16[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 3); l17 = (l17 + 1)) {
			fRec17[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec18[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec19[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec20[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec21[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec22[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec23[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 3); l24 = (l24 + 1)) {
			fRec24[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 3); l25 = (l25 + 1)) {
			fRec25[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec26[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec27[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec28[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 3); l29 = (l29 + 1)) {
			fRec29[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 3); l30 = (l30 + 1)) {
			fRec30[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 3); l31 = (l31 + 1)) {
			fRec31[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 3); l32 = (l32 + 1)) {
			fRec32[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec33[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec34[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec35[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 3); l36 = (l36 + 1)) {
			fRec36[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 3); l37 = (l37 + 1)) {
			fRec37[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 3); l38 = (l38 + 1)) {
			fRec38[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 3); l39 = (l39 + 1)) {
			fRec39[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 3); l40 = (l40 + 1)) {
			fRec40[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 3); l41 = (l41 + 1)) {
			fRec41[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 3); l42 = (l42 + 1)) {
			fRec42[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec43[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec44[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec45[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec46[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec47[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec48[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec49[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec50[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec51[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec52[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 3); l53 = (l53 + 1)) {
			fRec53[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec54[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec55[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 3); l56 = (l56 + 1)) {
			fRec56[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec57[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec58[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec59[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fVec2[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2048); l61 = (l61 + 1)) {
			fVec3[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec3[l62] = 0.0f;
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
	
	virtual synth_marimba* clone() {
		return new synth_marimba();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("synth.marimba");
		ui_interface->addHorizontalSlider("cutoff", &fHslider2, 7000.0f, 1000.0f, 10000.0f, 1.0f);
		ui_interface->addHorizontalSlider("gain", &fHslider1, 1.0f, 0.0f, 1.0f, 0.00100000005f);
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("pitch", &fHslider0, 48.0f, 36.0f, 84.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("pos", &fHslider4, 0.0f, 0.0f, 4.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("sharp", &fHslider3, 0.25f, 0.00999999978f, 1.0f, 0.00100000005f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = std::pow(2.0f, (0.0833333358f * (float(fHslider0) + -69.0f)));
		float fSlow1 = (fConst1 * ((0.772727251f / fSlow0) + -0.0399999991f));
		float fSlow2 = (fSlow1 + -1.49999499f);
		float fSlow3 = std::floor(fSlow2);
		float fSlow4 = (fSlow1 + (-1.0f - fSlow3));
		float fSlow5 = (fSlow1 + (-2.0f - fSlow3));
		float fSlow6 = (fSlow1 + (-3.0f - fSlow3));
		float fSlow7 = (fSlow1 + (-4.0f - fSlow3));
		float fSlow8 = ((((0.0f - fSlow4) * (0.0f - (0.5f * fSlow5))) * (0.0f - (0.333333343f * fSlow6))) * (0.0f - (0.25f * fSlow7)));
		int iSlow9 = int(fSlow2);
		int iSlow10 = int(std::min<float>(fConst2, float(std::max<int>(0, iSlow9))));
		float fSlow11 = (fSlow1 - fSlow3);
		float fSlow12 = (((0.0f - fSlow5) * (0.0f - (0.5f * fSlow6))) * (0.0f - (0.333333343f * fSlow7)));
		int iSlow13 = int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow9 + 1)))));
		float fSlow14 = (0.5f * ((fSlow4 * (0.0f - fSlow6)) * (0.0f - (0.5f * fSlow7))));
		int iSlow15 = int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow9 + 2)))));
		float fSlow16 = (fSlow4 * fSlow5);
		float fSlow17 = (0.166666672f * (fSlow16 * (0.0f - fSlow7)));
		int iSlow18 = int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow9 + 3)))));
		float fSlow19 = (0.0416666679f * (fSlow16 * fSlow6));
		int iSlow20 = int(std::min<float>(fConst2, float(std::max<int>(0, (iSlow9 + 4)))));
		float fSlow21 = std::tan((fConst3 * float(fHslider2)));
		float fSlow22 = (1.0f / fSlow21);
		float fSlow23 = (((fSlow22 + 1.41421354f) / fSlow21) + 1.0f);
		float fSlow24 = (float(fHslider1) / fSlow23);
		float fSlow25 = (1.0f / fSlow23);
		float fSlow26 = (((fSlow22 + -1.41421354f) / fSlow21) + 1.0f);
		float fSlow27 = (2.0f * (1.0f - (1.0f / synth_marimba_faustpower2_f(fSlow21))));
		float fSlow28 = (1.0f / std::max<float>(1.0f, (fConst12 * float(fHslider3))));
		float fSlow29 = float(fButton0);
		float fSlow30 = (fConst14 * std::cos((fConst15 * fSlow0)));
		int iSlow31 = (50 * int(float(fHslider4)));
		float fSlow32 = (((79852.0781f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 45)] : 0.0f);
		float fSlow33 = (fConst19 * std::cos((fConst20 * fSlow0)));
		float fSlow34 = (((66894.5234f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 37)] : 0.0f);
		float fSlow35 = (fConst23 * std::cos((fConst24 * fSlow0)));
		float fSlow36 = (((65752.7188f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 36)] : 0.0f);
		float fSlow37 = (fConst27 * std::cos((fConst28 * fSlow0)));
		float fSlow38 = (((63601.5586f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 35)] : 0.0f);
		float fSlow39 = (fConst31 * std::cos((fConst32 * fSlow0)));
		float fSlow40 = (((58729.8789f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 34)] : 0.0f);
		float fSlow41 = (fConst35 * std::cos((fConst36 * fSlow0)));
		float fSlow42 = (((58193.9609f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 33)] : 0.0f);
		float fSlow43 = (fConst39 * std::cos((fConst40 * fSlow0)));
		float fSlow44 = (((57192.0781f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 32)] : 0.0f);
		float fSlow45 = (fConst43 * std::cos((fConst44 * fSlow0)));
		float fSlow46 = (((55899.8008f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 31)] : 0.0f);
		float fSlow47 = (fConst47 * std::cos((fConst48 * fSlow0)));
		float fSlow48 = (((52516.6406f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 30)] : 0.0f);
		float fSlow49 = (fConst51 * std::cos((fConst52 * fSlow0)));
		float fSlow50 = (((40905.0859f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 24)] : 0.0f);
		float fSlow51 = (fConst55 * std::cos((fConst56 * fSlow0)));
		float fSlow52 = (((39590.3203f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 23)] : 0.0f);
		float fSlow53 = (fConst59 * std::cos((fConst60 * fSlow0)));
		float fSlow54 = (((35568.5f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 22)] : 0.0f);
		float fSlow55 = (fConst63 * std::cos((fConst64 * fSlow0)));
		float fSlow56 = (((34624.4805f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 21)] : 0.0f);
		float fSlow57 = (fConst67 * std::cos((fConst68 * fSlow0)));
		float fSlow58 = (((32793.2422f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 20)] : 0.0f);
		float fSlow59 = (fConst71 * std::cos((fConst72 * fSlow0)));
		float fSlow60 = (((29461.8281f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 19)] : 0.0f);
		float fSlow61 = (fConst75 * std::cos((fConst76 * fSlow0)));
		float fSlow62 = (((78732.2812f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 44)] : 0.0f);
		float fSlow63 = (fConst79 * std::cos((fConst80 * fSlow0)));
		float fSlow64 = (((76764.1562f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 43)] : 0.0f);
		float fSlow65 = (fConst83 * std::cos((fConst84 * fSlow0)));
		float fSlow66 = (((75619.7188f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 42)] : 0.0f);
		float fSlow67 = (fConst87 * std::cos((fConst88 * fSlow0)));
		float fSlow68 = (((73966.2031f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 41)] : 0.0f);
		float fSlow69 = (fConst91 * std::cos((fConst92 * fSlow0)));
		float fSlow70 = (((69600.5234f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 40)] : 0.0f);
		float fSlow71 = (fConst95 * std::cos((fConst96 * fSlow0)));
		float fSlow72 = (((50083.4414f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 29)] : 0.0f);
		float fSlow73 = (fConst99 * std::cos((fConst100 * fSlow0)));
		float fSlow74 = (((49310.3594f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 28)] : 0.0f);
		float fSlow75 = (fConst103 * std::cos((fConst104 * fSlow0)));
		float fSlow76 = (((48672.8008f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 27)] : 0.0f);
		float fSlow77 = (fConst107 * std::cos((fConst108 * fSlow0)));
		float fSlow78 = (((42891.5078f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 26)] : 0.0f);
		float fSlow79 = (fConst111 * std::cos((fConst112 * fSlow0)));
		float fSlow80 = (((41884.2148f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 25)] : 0.0f);
		float fSlow81 = (fConst115 * std::cos((fConst116 * fSlow0)));
		float fSlow82 = (((1687.26355f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 2)] : 0.0f);
		float fSlow83 = (fConst119 * std::cos((fConst120 * fSlow0)));
		float fSlow84 = (((1457.96643f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 1)] : 0.0f);
		float fSlow85 = (fConst123 * std::cos((fConst124 * fSlow0)));
		float fSlow86 = (((440.0f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[iSlow31] : 0.0f);
		float fSlow87 = (fConst127 * std::cos((fConst128 * fSlow0)));
		float fSlow88 = (((3547.7771f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 3)] : 0.0f);
		float fSlow89 = (fConst131 * std::cos((fConst132 * fSlow0)));
		float fSlow90 = (((4157.02344f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 4)] : 0.0f);
		float fSlow91 = (fConst135 * std::cos((fConst136 * fSlow0)));
		float fSlow92 = (((6211.43604f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 5)] : 0.0f);
		float fSlow93 = (fConst139 * std::cos((fConst140 * fSlow0)));
		float fSlow94 = (((8088.95996f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 6)] : 0.0f);
		float fSlow95 = (fConst143 * std::cos((fConst144 * fSlow0)));
		float fSlow96 = (((9244.48828f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 7)] : 0.0f);
		float fSlow97 = (fConst147 * std::cos((fConst148 * fSlow0)));
		float fSlow98 = (((11518.0996f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 8)] : 0.0f);
		float fSlow99 = (fConst151 * std::cos((fConst152 * fSlow0)));
		float fSlow100 = (((12757.5361f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 9)] : 0.0f);
		float fSlow101 = (fConst155 * std::cos((fConst156 * fSlow0)));
		float fSlow102 = (((16312.0322f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 10)] : 0.0f);
		float fSlow103 = (fConst159 * std::cos((fConst160 * fSlow0)));
		float fSlow104 = (((16662.9316f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 11)] : 0.0f);
		float fSlow105 = (fConst163 * std::cos((fConst164 * fSlow0)));
		float fSlow106 = (((17627.8965f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 12)] : 0.0f);
		float fSlow107 = (fConst167 * std::cos((fConst168 * fSlow0)));
		float fSlow108 = (((20963.3164f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 13)] : 0.0f);
		float fSlow109 = (fConst171 * std::cos((fConst172 * fSlow0)));
		float fSlow110 = (((22448.3594f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 14)] : 0.0f);
		float fSlow111 = (fConst175 * std::cos((fConst176 * fSlow0)));
		float fSlow112 = (((23069.1992f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 15)] : 0.0f);
		float fSlow113 = (fConst179 * std::cos((fConst180 * fSlow0)));
		float fSlow114 = (((25645.8398f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 16)] : 0.0f);
		float fSlow115 = (fConst183 * std::cos((fConst184 * fSlow0)));
		float fSlow116 = (((27961.3848f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 17)] : 0.0f);
		float fSlow117 = (fConst187 * std::cos((fConst188 * fSlow0)));
		float fSlow118 = (((28759.6328f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 18)] : 0.0f);
		float fSlow119 = (fConst191 * std::cos((fConst192 * fSlow0)));
		float fSlow120 = (((67393.0391f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 38)] : 0.0f);
		float fSlow121 = (fConst195 * std::cos((fConst196 * fSlow0)));
		float fSlow122 = (((68462.6797f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 39)] : 0.0f);
		float fSlow123 = (fConst199 * std::cos((fConst200 * fSlow0)));
		float fSlow124 = (((81575.1172f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 46)] : 0.0f);
		float fSlow125 = (fConst203 * std::cos((fConst204 * fSlow0)));
		float fSlow126 = (((83762.3594f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 47)] : 0.0f);
		float fSlow127 = (fConst207 * std::cos((fConst208 * fSlow0)));
		float fSlow128 = (((84563.6016f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 48)] : 0.0f);
		float fSlow129 = (fConst211 * std::cos((fConst212 * fSlow0)));
		float fSlow130 = (((86022.2031f * fSlow0) < fConst17) ? ftbl0synth_marimbaSIG0[(iSlow31 + 49)] : 0.0f);
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec0[0] = 0;
			fRec4[0] = ((0.949999988f * fRec4[1]) + (0.0500000007f * fRec3[1]));
			float fTemp0 = ((0.99000001f * fRec4[0]) + float(iRec0[1]));
			fVec0[(IOTA & 2047)] = fTemp0;
			fRec2[0] = ((fSlow8 * fVec0[((IOTA - iSlow10) & 2047)]) + (fSlow11 * ((((fSlow12 * fVec0[((IOTA - iSlow13) & 2047)]) + (fSlow14 * fVec0[((IOTA - iSlow15) & 2047)])) + (fSlow17 * fVec0[((IOTA - iSlow18) & 2047)])) + (fSlow19 * fVec0[((IOTA - iSlow20) & 2047)]))));
			fRec5[0] = ((0.949999988f * fRec5[1]) + (0.0500000007f * fRec2[1]));
			iRec9[0] = ((1103515245 * iRec9[1]) + 12345);
			fRec8[0] = ((4.65661287e-10f * float(iRec9[0])) - (fConst6 * ((fConst9 * fRec8[2]) + (fConst10 * fRec8[1]))));
			fRec7[0] = ((fConst6 * (((fConst8 * fRec8[0]) + (fConst11 * fRec8[1])) + (fConst8 * fRec8[2]))) - (fSlow25 * ((fSlow26 * fRec7[2]) + (fSlow27 * fRec7[1]))));
			fVec1[0] = fSlow29;
			iRec10[0] = (((iRec10[1] + (iRec10[1] > 0)) * (fSlow29 <= fVec1[1])) + (fSlow29 > fVec1[1]));
			float fTemp1 = (fSlow28 * float(iRec10[0]));
			float fTemp2 = (fSlow24 * ((fRec7[2] + (fRec7[0] + (2.0f * fRec7[1]))) * std::max<float>(0.0f, std::min<float>(fTemp1, (2.0f - fTemp1)))));
			fRec6[0] = (fTemp2 - ((fSlow30 * fRec6[1]) + (fConst16 * fRec6[2])));
			fRec11[0] = (fTemp2 - ((fSlow33 * fRec11[1]) + (fConst21 * fRec11[2])));
			fRec12[0] = (fTemp2 - ((fSlow35 * fRec12[1]) + (fConst25 * fRec12[2])));
			fRec13[0] = (fTemp2 - ((fSlow37 * fRec13[1]) + (fConst29 * fRec13[2])));
			fRec14[0] = (fTemp2 - ((fSlow39 * fRec14[1]) + (fConst33 * fRec14[2])));
			fRec15[0] = (fTemp2 - ((fSlow41 * fRec15[1]) + (fConst37 * fRec15[2])));
			fRec16[0] = (fTemp2 - ((fSlow43 * fRec16[1]) + (fConst41 * fRec16[2])));
			fRec17[0] = (fTemp2 - ((fSlow45 * fRec17[1]) + (fConst45 * fRec17[2])));
			fRec18[0] = (fTemp2 - ((fSlow47 * fRec18[1]) + (fConst49 * fRec18[2])));
			fRec19[0] = (fTemp2 - ((fSlow49 * fRec19[1]) + (fConst53 * fRec19[2])));
			fRec20[0] = (fTemp2 - ((fSlow51 * fRec20[1]) + (fConst57 * fRec20[2])));
			fRec21[0] = (fTemp2 - ((fSlow53 * fRec21[1]) + (fConst61 * fRec21[2])));
			fRec22[0] = (fTemp2 - ((fSlow55 * fRec22[1]) + (fConst65 * fRec22[2])));
			fRec23[0] = (fTemp2 - ((fSlow57 * fRec23[1]) + (fConst69 * fRec23[2])));
			fRec24[0] = (fTemp2 - ((fSlow59 * fRec24[1]) + (fConst73 * fRec24[2])));
			fRec25[0] = (fTemp2 - ((fSlow61 * fRec25[1]) + (fConst77 * fRec25[2])));
			fRec26[0] = (fTemp2 - ((fSlow63 * fRec26[1]) + (fConst81 * fRec26[2])));
			fRec27[0] = (fTemp2 - ((fSlow65 * fRec27[1]) + (fConst85 * fRec27[2])));
			fRec28[0] = (fTemp2 - ((fSlow67 * fRec28[1]) + (fConst89 * fRec28[2])));
			fRec29[0] = (fTemp2 - ((fSlow69 * fRec29[1]) + (fConst93 * fRec29[2])));
			fRec30[0] = (fTemp2 - ((fSlow71 * fRec30[1]) + (fConst97 * fRec30[2])));
			fRec31[0] = (fTemp2 - ((fSlow73 * fRec31[1]) + (fConst101 * fRec31[2])));
			fRec32[0] = (fTemp2 - ((fSlow75 * fRec32[1]) + (fConst105 * fRec32[2])));
			fRec33[0] = (fTemp2 - ((fSlow77 * fRec33[1]) + (fConst109 * fRec33[2])));
			fRec34[0] = (fTemp2 - ((fSlow79 * fRec34[1]) + (fConst113 * fRec34[2])));
			fRec35[0] = (fTemp2 - ((fSlow81 * fRec35[1]) + (fConst117 * fRec35[2])));
			fRec36[0] = (fTemp2 - ((fSlow83 * fRec36[1]) + (fConst121 * fRec36[2])));
			fRec37[0] = (fTemp2 - ((fSlow85 * fRec37[1]) + (fConst125 * fRec37[2])));
			fRec38[0] = (fTemp2 - ((fSlow87 * fRec38[1]) + (fConst129 * fRec38[2])));
			fRec39[0] = (fTemp2 - ((fSlow89 * fRec39[1]) + (fConst133 * fRec39[2])));
			fRec40[0] = (fTemp2 - ((fSlow91 * fRec40[1]) + (fConst137 * fRec40[2])));
			fRec41[0] = (fTemp2 - ((fSlow93 * fRec41[1]) + (fConst141 * fRec41[2])));
			fRec42[0] = (fTemp2 - ((fSlow95 * fRec42[1]) + (fConst145 * fRec42[2])));
			fRec43[0] = (fTemp2 - ((fSlow97 * fRec43[1]) + (fConst149 * fRec43[2])));
			fRec44[0] = (fTemp2 - ((fSlow99 * fRec44[1]) + (fConst153 * fRec44[2])));
			fRec45[0] = (fTemp2 - ((fSlow101 * fRec45[1]) + (fConst157 * fRec45[2])));
			fRec46[0] = (fTemp2 - ((fSlow103 * fRec46[1]) + (fConst161 * fRec46[2])));
			fRec47[0] = (fTemp2 - ((fSlow105 * fRec47[1]) + (fConst165 * fRec47[2])));
			fRec48[0] = (fTemp2 - ((fSlow107 * fRec48[1]) + (fConst169 * fRec48[2])));
			fRec49[0] = (fTemp2 - ((fSlow109 * fRec49[1]) + (fConst173 * fRec49[2])));
			fRec50[0] = (fTemp2 - ((fSlow111 * fRec50[1]) + (fConst177 * fRec50[2])));
			fRec51[0] = (fTemp2 - ((fSlow113 * fRec51[1]) + (fConst181 * fRec51[2])));
			fRec52[0] = (fTemp2 - ((fSlow115 * fRec52[1]) + (fConst185 * fRec52[2])));
			fRec53[0] = (fTemp2 - ((fSlow117 * fRec53[1]) + (fConst189 * fRec53[2])));
			fRec54[0] = (fTemp2 - ((fSlow119 * fRec54[1]) + (fConst193 * fRec54[2])));
			fRec55[0] = (fTemp2 - ((fSlow121 * fRec55[1]) + (fConst197 * fRec55[2])));
			fRec56[0] = (fTemp2 - ((fSlow123 * fRec56[1]) + (fConst201 * fRec56[2])));
			fRec57[0] = (fTemp2 - ((fSlow125 * fRec57[1]) + (fConst205 * fRec57[2])));
			fRec58[0] = (fTemp2 - ((fSlow127 * fRec58[1]) + (fConst209 * fRec58[2])));
			fRec59[0] = (fTemp2 - ((fSlow129 * fRec59[1]) + (fConst213 * fRec59[2])));
			fVec2[0] = (((fRec6[0] - fRec6[2]) * fSlow32) + ((((((fRec11[0] - fRec11[2]) * fSlow34) + (((fRec12[0] - fRec12[2]) * fSlow36) + (((fRec13[0] - fRec13[2]) * fSlow38) + (((fRec14[0] - fRec14[2]) * fSlow40) + (((fRec15[0] - fRec15[2]) * fSlow42) + (((fRec16[0] - fRec16[2]) * fSlow44) + (((fRec17[0] - fRec17[2]) * fSlow46) + (((fRec18[0] - fRec18[2]) * fSlow48) + (((fRec19[0] - fRec19[2]) * fSlow50) + (((fRec20[0] - fRec20[2]) * fSlow52) + (((fRec21[0] - fRec21[2]) * fSlow54) + (((fRec22[0] - fRec22[2]) * fSlow56) + (((fRec23[0] - fRec23[2]) * fSlow58) + (((fRec24[0] - fRec24[2]) * fSlow60) + (((fRec25[0] - fRec25[2]) * fSlow62) + (((fRec26[0] - fRec26[2]) * fSlow64) + (((fRec27[0] - fRec27[2]) * fSlow66) + (((fRec28[0] - fRec28[2]) * fSlow68) + (((fRec29[0] - fRec29[2]) * fSlow70) + (((fRec30[0] - fRec30[2]) * fSlow72) + (((fRec31[0] - fRec31[2]) * fSlow74) + (((fRec32[0] - fRec32[2]) * fSlow76) + (((fRec33[0] - fRec33[2]) * fSlow78) + (((fRec34[0] - fRec34[2]) * fSlow80) + (((fRec35[0] - fRec35[2]) * fSlow82) + (((fRec36[0] - fRec36[2]) * fSlow84) + (((fRec37[0] - fRec37[2]) * fSlow86) + ((((((((((((((((((((fRec38[0] - fRec38[2]) * fSlow88) + ((fRec39[0] - fRec39[2]) * fSlow90)) + ((fRec40[0] - fRec40[2]) * fSlow92)) + ((fRec41[0] - fRec41[2]) * fSlow94)) + ((fRec42[0] - fRec42[2]) * fSlow96)) + ((fRec43[0] - fRec43[2]) * fSlow98)) + ((fRec44[0] - fRec44[2]) * fSlow100)) + ((fRec45[0] - fRec45[2]) * fSlow102)) + ((fRec46[0] - fRec46[2]) * fSlow104)) + ((fRec47[0] - fRec47[2]) * fSlow106)) + ((fRec48[0] - fRec48[2]) * fSlow108)) + ((fRec49[0] - fRec49[2]) * fSlow110)) + ((fRec50[0] - fRec50[2]) * fSlow112)) + ((fRec51[0] - fRec51[2]) * fSlow114)) + ((fRec52[0] - fRec52[2]) * fSlow116)) + ((fRec53[0] - fRec53[2]) * fSlow118)) + ((fRec54[0] - fRec54[2]) * fSlow120)) + ((fRec55[0] - fRec55[2]) * fSlow122)) + ((fRec56[0] - fRec56[2]) * fSlow124))))))))))))))))))))))))))))) + ((fRec57[0] - fRec57[2]) * fSlow126)) + ((fRec58[0] - fRec58[2]) * fSlow128)) + ((fRec59[0] - fRec59[2]) * fSlow130)));
			float fTemp3 = ((0.99000001f * fRec5[0]) + (0.0199999996f * fVec2[1]));
			fVec3[(IOTA & 2047)] = fTemp3;
			fRec3[0] = ((fSlow8 * fVec3[((IOTA - iSlow10) & 2047)]) + (fSlow11 * ((((fSlow12 * fVec3[((IOTA - iSlow13) & 2047)]) + (fSlow14 * fVec3[((IOTA - iSlow15) & 2047)])) + (fSlow17 * fVec3[((IOTA - iSlow18) & 2047)])) + (fSlow19 * fVec3[((IOTA - iSlow20) & 2047)]))));
			float fRec1 = fRec3[0];
			output0[i] = FAUSTFLOAT(fRec1);
			iRec0[1] = iRec0[0];
			fRec4[1] = fRec4[0];
			IOTA = (IOTA + 1);
			fRec2[1] = fRec2[0];
			fRec5[1] = fRec5[0];
			iRec9[1] = iRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fVec1[1] = fVec1[0];
			iRec10[1] = iRec10[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fRec33[2] = fRec33[1];
			fRec33[1] = fRec33[0];
			fRec34[2] = fRec34[1];
			fRec34[1] = fRec34[0];
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
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
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fVec2[1] = fVec2[0];
			fRec3[1] = fRec3[0];
		}
	}

};
// clang-format on
#endif

    template <class T>
    struct _synth_marimba_UI : public UI {
    static std::string name;
};

template <class T>
std::string _synth_marimba_UI<T>::name(sym(synth_marimba));

typedef _synth_marimba_UI<synth_marimba> synth_marimba_UI;

class faust_synth_marimba_tilde : public FaustExternal<synth_marimba, synth_marimba_UI> {
public:
    faust_synth_marimba_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

#endif
