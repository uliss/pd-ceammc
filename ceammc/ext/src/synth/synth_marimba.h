/* ------------------------------------------------------------
name: "marimba"
Code generated with Faust 2.8.5 (https://faust.grame.fr)
Compilation options: cpp, -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __marimba_H__
#define  __marimba_H__

/************************************************************************
 ************************************************************************
    FAUST Architecture File
    Copyright (C) 2006-2011 Albert Graef <Dr.Graef@t-online.de>
    ---------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2.1 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with the GNU C Library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA.
 ************************************************************************
 ************************************************************************/

/* Pd architecture file, written by Albert Graef <Dr.Graef@t-online.de>.
   This was derived from minimal.cpp included in the Faust distribution.
   Please note that this is to be compiled as a shared library, which is
   then loaded dynamically by Pd as an external. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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


#include "ceammc_atomlist.h"
#include "ceammc_externals.h"
#include "m_pd.h"

/******************************************************************************
*******************************************************************************

                                   VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/

#ifdef FAUST_MACRO
// clang-format off
// clang-format on
#endif

#define sym(name) xsym(name)
#define xsym(name) #name

/***************************************************************************
   Pd UI interface
***************************************************************************/

// clang-format off
#ifndef FAUST_MACRO
struct marimba : public dsp {
};
#endif
// clang-format on

#include "ceammc_faust.h"
using namespace ceammc::faust;

/******************************************************************************
*******************************************************************************

                FAUST DSP

*******************************************************************************
*******************************************************************************/

//----------------------------------------------------------------------------
//  FAUST generated signal processor
//----------------------------------------------------------------------------

#ifdef FAUST_MACRO
// clang-format off
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <math.h>

static float fmarimbaSIG0Wave0[250] = {1.0f,0.776724994f,0.625723004f,0.855223f,0.760159016f,0.69837302f,0.768010974f,0.64112699f,0.244034007f,0.707754016f,0.634012997f,0.247527003f,0.660848975f,0.450396001f,0.567782998f,0.106361002f,0.716813982f,0.663919985f,0.291207999f,0.310598999f,0.801495016f,0.635291994f,0.307435006f,0.874123991f,0.497667998f,0.487087995f,0.459114999f,0.733455002f,0.541818023f,0.441318005f,0.313919991f,0.40309f,0.685352981f,0.603139997f,0.400552005f,0.453511f,0.634386003f,0.291547f,0.131604999f,0.368506998f,0.839906991f,0.602159977f,0.288296014f,0.579670012f,0.0242493004f,0.262746006f,0.368588001f,0.890284002f,0.408962995f,0.556071997f,0.884427011f,0.832109988f,0.612015009f,0.757175982f,0.919476986f,1.0f,0.827962995f,0.89240998f,0.0357408002f,0.480789006f,0.75287199f,0.0546301007f,0.235936999f,0.362937987f,0.444471985f,0.101751f,0.703418016f,0.453135997f,0.316628993f,0.490393996f,0.982508004f,0.551621974f,0.602008998f,0.666957021f,0.776830018f,0.905662f,0.0987197012f,0.402967989f,0.829451978f,0.307644993f,0.640479982f,0.983971f,0.584204972f,0.650364995f,0.334446996f,0.583570004f,0.540190995f,0.672533989f,0.245711997f,0.687298f,0.883058012f,0.792949975f,0.600619018f,0.572682023f,0.122612f,0.388247997f,0.290657997f,0.380255014f,0.290966988f,0.567818999f,0.0737721026f,0.42098999f,0.0786577985f,0.393994987f,0.268983006f,0.260614008f,0.494085997f,0.238025993f,0.0987823978f,0.277879f,0.440562993f,0.0770211965f,0.450590998f,0.128137007f,0.0368275009f,0.128699005f,0.329605013f,0.374511987f,0.363590002f,0.272594005f,0.379052013f,0.305240989f,0.0741128996f,0.34572801f,0.299349993f,0.221284002f,0.0261390992f,0.293202013f,0.361885011f,0.114330001f,0.239004999f,0.434156001f,0.329582989f,0.219459996f,0.284175009f,0.198554993f,0.431975991f,0.302985013f,1.0f,0.146220997f,0.140700996f,0.264243007f,0.185996994f,0.426322013f,0.304780006f,0.343989998f,0.195429996f,0.386954993f,0.187600002f,0.172812f,0.0434115008f,0.303761005f,0.0694539994f,0.453943014f,0.832450986f,0.317817003f,0.940600991f,1.0f,0.180657998f,0.737921f,0.832297027f,0.402352005f,0.126785994f,0.594398022f,0.485455006f,0.324470013f,0.365101993f,0.777921975f,0.588271976f,0.401353002f,0.610734999f,0.158693001f,0.0746072009f,0.825098991f,0.925459027f,0.65377003f,0.260791987f,0.719384015f,0.559907973f,0.372590005f,0.360035002f,0.622938991f,0.210271001f,0.444595009f,0.311286002f,0.464309007f,0.557231009f,0.524079978f,0.0701055974f,0.320749015f,0.194460005f,0.727608979f,0.522062004f,0.394003987f,0.235035002f,0.395646006f,0.494796008f,0.51731801f,0.109751999f,0.69284898f,0.00632009003f,0.0207582992f,0.00306107011f,0.0637191013f,0.0816610008f,0.0351100005f,0.127813995f,0.202294007f,0.0764145032f,0.263126999f,0.400198996f,0.267277986f,0.633385003f,1.0f,0.73990202f,0.413762987f,0.418110013f,0.612715006f,0.67237401f,0.339673996f,0.211720005f,0.459645003f,0.102499999f,0.325890005f,0.148154005f,0.265442014f,0.0974304974f,0.286437988f,0.275213003f,0.109111004f,0.575088978f,0.370283008f,0.29411f,0.259826005f,0.0648718998f,0.583418012f,0.282662988f,0.182004005f,0.117421001f,0.417726994f,0.169650003f,0.24853f,0.122818999f,0.185486004f,0.0433618017f,0.373849005f,0.25276801f,0.195103005f,0.0927835032f,0.166543007f};

class marimbaSIG0 {
	
  private:
	
	int fmarimbaSIG0Wave0_idx;
	
  public:
	
	int getNumInputsmarimbaSIG0() {
		return 0;
		
	}
	int getNumOutputsmarimbaSIG0() {
		return 1;
		
	}
	int getInputRatemarimbaSIG0(int channel) {
		int rate;
		switch (channel) {
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	int getOutputRatemarimbaSIG0(int channel) {
		int rate;
		switch (channel) {
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
	
	void instanceInitmarimbaSIG0(int samplingFreq) {
		fmarimbaSIG0Wave0_idx = 0;
		
	}
	
	void fillmarimbaSIG0(int count, float* output) {
		for (int i = 0; (i < count); i = (i + 1)) {
			output[i] = fmarimbaSIG0Wave0[fmarimbaSIG0Wave0_idx];
			fmarimbaSIG0Wave0_idx = ((1 + fmarimbaSIG0Wave0_idx) % 250);
			
		}
		
	}
};

marimbaSIG0* newmarimbaSIG0() { return (marimbaSIG0*)new marimbaSIG0(); }
void deletemarimbaSIG0(marimbaSIG0* dsp) { delete dsp; }

static float ftbl0marimbaSIG0[250];
static float marimba_faustpower2_f(float value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS marimba
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class marimba : public dsp {
	
 private:
	
	int iRec0[2];
	int fSamplingFreq;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fRec4[2];
	int IOTA;
	float fVec0[2048];
	float fConst2;
	float fRec2[2];
	float fConst3;
	FAUSTFLOAT fHslider1;
	float fConst4;
	FAUSTFLOAT fHslider2;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	int iRec8[2];
	float fConst9;
	float fConst10;
	float fRec7[3];
	float fConst11;
	float fRec6[3];
	FAUSTFLOAT fButton0;
	float fVec1[2];
	float fConst12;
	FAUSTFLOAT fHslider3;
	float fVec2[2];
	float fRec9[2];
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fRec5[3];
	float fConst19;
	float fConst20;
	float fConst21;
	float fConst22;
	float fRec10[3];
	float fConst23;
	float fConst24;
	float fConst25;
	float fConst26;
	float fRec11[3];
	float fConst27;
	float fConst28;
	float fConst29;
	float fConst30;
	float fRec12[3];
	float fConst31;
	float fConst32;
	float fConst33;
	float fConst34;
	float fRec13[3];
	float fConst35;
	float fConst36;
	float fConst37;
	float fConst38;
	float fRec14[3];
	float fConst39;
	float fConst40;
	float fConst41;
	float fConst42;
	float fRec15[3];
	float fConst43;
	float fConst44;
	float fConst45;
	float fConst46;
	float fRec16[3];
	float fConst47;
	float fConst48;
	float fConst49;
	float fConst50;
	float fRec17[3];
	float fConst51;
	float fConst52;
	float fConst53;
	float fConst54;
	float fRec18[3];
	float fConst55;
	float fConst56;
	float fConst57;
	float fConst58;
	float fRec19[3];
	float fConst59;
	float fConst60;
	float fConst61;
	float fConst62;
	float fRec20[3];
	float fConst63;
	float fConst64;
	float fConst65;
	float fConst66;
	float fRec21[3];
	float fConst67;
	float fConst68;
	float fConst69;
	float fConst70;
	float fRec22[3];
	float fConst71;
	float fConst72;
	float fConst73;
	float fConst74;
	float fRec23[3];
	float fConst75;
	float fConst76;
	float fConst77;
	float fConst78;
	float fRec24[3];
	float fConst79;
	float fConst80;
	float fConst81;
	float fConst82;
	float fRec25[3];
	float fConst83;
	float fConst84;
	float fConst85;
	float fConst86;
	float fRec26[3];
	float fConst87;
	float fConst88;
	float fConst89;
	float fConst90;
	float fRec27[3];
	float fConst91;
	float fConst92;
	float fConst93;
	float fConst94;
	float fRec28[3];
	float fConst95;
	float fConst96;
	float fConst97;
	float fConst98;
	float fRec29[3];
	float fConst99;
	float fConst100;
	float fConst101;
	float fConst102;
	float fRec30[3];
	float fConst103;
	float fConst104;
	float fConst105;
	float fConst106;
	float fRec31[3];
	float fConst107;
	float fConst108;
	float fConst109;
	float fConst110;
	float fRec32[3];
	float fConst111;
	float fConst112;
	float fConst113;
	float fConst114;
	float fRec33[3];
	float fConst115;
	float fConst116;
	float fConst117;
	float fConst118;
	float fRec34[3];
	float fConst119;
	float fConst120;
	float fConst121;
	float fConst122;
	float fRec35[3];
	float fConst123;
	float fConst124;
	float fConst125;
	float fConst126;
	float fRec36[3];
	float fConst127;
	float fConst128;
	float fConst129;
	float fConst130;
	float fRec37[3];
	float fConst131;
	float fConst132;
	float fConst133;
	float fConst134;
	float fRec38[3];
	float fConst135;
	float fConst136;
	float fConst137;
	float fConst138;
	float fRec39[3];
	float fConst139;
	float fConst140;
	float fConst141;
	float fConst142;
	float fRec40[3];
	float fConst143;
	float fConst144;
	float fConst145;
	float fConst146;
	float fRec41[3];
	float fConst147;
	float fConst148;
	float fConst149;
	float fConst150;
	float fRec42[3];
	float fConst151;
	float fConst152;
	float fConst153;
	float fConst154;
	float fRec43[3];
	float fConst155;
	float fConst156;
	float fConst157;
	float fConst158;
	float fRec44[3];
	float fConst159;
	float fConst160;
	float fConst161;
	float fConst162;
	float fRec45[3];
	float fConst163;
	float fConst164;
	float fConst165;
	float fConst166;
	float fRec46[3];
	float fConst167;
	float fConst168;
	float fConst169;
	float fConst170;
	float fRec47[3];
	float fConst171;
	float fConst172;
	float fConst173;
	float fConst174;
	float fRec48[3];
	float fConst175;
	float fConst176;
	float fConst177;
	float fConst178;
	float fRec49[3];
	float fConst179;
	float fConst180;
	float fConst181;
	float fConst182;
	float fRec50[3];
	float fConst183;
	float fConst184;
	float fConst185;
	float fConst186;
	float fRec51[3];
	float fConst187;
	float fConst188;
	float fConst189;
	float fConst190;
	float fRec52[3];
	float fConst191;
	float fConst192;
	float fConst193;
	float fConst194;
	float fRec53[3];
	float fConst195;
	float fConst196;
	float fConst197;
	float fConst198;
	float fRec54[3];
	float fConst199;
	float fConst200;
	float fConst201;
	float fConst202;
	float fRec55[3];
	float fConst203;
	float fConst204;
	float fConst205;
	float fConst206;
	float fRec56[3];
	float fConst207;
	float fConst208;
	float fConst209;
	float fConst210;
	float fRec57[3];
	float fConst211;
	float fConst212;
	float fConst213;
	float fConst214;
	float fRec58[3];
	float fVec3[2];
	float fRec59[2];
	float fVec4[2048];
	float fRec3[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "0.0");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.0");
		m->declare("filename", "synth_marimba");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "0.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "marimba");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "0.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.0");
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
		switch (channel) {
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
		marimbaSIG0* sig0 = newmarimbaSIG0();
		sig0->instanceInitmarimbaSIG0(samplingFreq);
		sig0->fillmarimbaSIG0(250, ftbl0marimbaSIG0);
		deletemarimbaSIG0(sig0);
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = std::min(192000.0f, std::max(1.0f, float(fSamplingFreq)));
		fConst1 = (0.00147058826f * fConst0);
		fConst2 = (0.00882352982f * fConst0);
		fConst3 = ((0.5f * fConst0) + -1.0f);
		fConst4 = (3.14159274f / fConst0);
		fConst5 = std::tan((31.415926f / fConst0));
		fConst6 = (1.0f / fConst5);
		fConst7 = (1.0f / (((fConst6 + 1.41421354f) / fConst5) + 1.0f));
		fConst8 = (1.0f / marimba_faustpower2_f(fConst5));
		fConst9 = (((fConst6 + -1.41421354f) / fConst5) + 1.0f);
		fConst10 = (2.0f * (1.0f - fConst8));
		fConst11 = (2.0f * (0.0f - fConst8));
		fConst12 = (0.00400000019f * fConst0);
		fConst13 = (0.00200000009f * fConst0);
		fConst14 = (500.0f / fConst0);
		fConst15 = std::pow(0.00100000005f, (1.0f / (2.19791588e-07f * fConst0)));
		fConst16 = (0.0f - (2.0f * fConst15));
		fConst17 = (501725.406f / fConst0);
		fConst18 = marimba_faustpower2_f(fConst15);
		fConst19 = std::pow(0.00100000005f, (1.0f / (4.94038545e-07f * fConst0)));
		fConst20 = (0.0f - (2.0f * fConst19));
		fConst21 = (494689.5f / fConst0);
		fConst22 = marimba_faustpower2_f(fConst19);
		fConst23 = std::pow(0.00100000005f, (1.0f / (1.5867239e-06f * fConst0)));
		fConst24 = (0.0f - (2.0f * fConst23));
		fConst25 = (482323.438f / fConst0);
		fConst26 = marimba_faustpower2_f(fConst23);
		fConst27 = std::pow(0.00100000005f, (1.0f / (2.80919903e-06f * fConst0)));
		fConst28 = (0.0f - (2.0f * fConst27));
		fConst29 = (475132.719f / fConst0);
		fConst30 = marimba_faustpower2_f(fConst27);
		fConst31 = std::pow(0.00100000005f, (1.0f / (5.79888183e-06f * fConst0)));
		fConst32 = (0.0f - (2.0f * fConst31));
		fConst33 = (464743.344f / fConst0);
		fConst34 = marimba_faustpower2_f(fConst31);
		fConst35 = std::pow(0.00100000005f, (1.0f / (2.66114275e-05f * fConst0)));
		fConst36 = (0.0f - (2.0f * fConst35));
		fConst37 = (437312.969f / fConst0);
		fConst38 = marimba_faustpower2_f(fConst35);
		fConst39 = std::pow(0.00100000005f, (1.0f / (3.7056674e-05f * fConst0)));
		fConst40 = (0.0f - (2.0f * fConst39));
		fConst41 = (430163.719f / fConst0);
		fConst42 = marimba_faustpower2_f(fConst39);
		fConst43 = std::pow(0.00100000005f, (1.0f / (4.96453904e-05f * fConst0)));
		fConst44 = (0.0f - (2.0f * fConst43));
		fConst45 = (423442.969f / fConst0);
		fConst46 = marimba_faustpower2_f(fConst43);
		fConst47 = std::pow(0.00100000005f, (1.0f / (5.65755508e-05f * fConst0)));
		fConst48 = (0.0f - (2.0f * fConst47));
		fConst49 = (420310.656f / fConst0);
		fConst50 = marimba_faustpower2_f(fConst47);
		fConst51 = std::pow(0.00100000005f, (1.0f / (7.53835775e-05f * fConst0)));
		fConst52 = (0.0f - (2.0f * fConst51));
		fConst53 = (413136.531f / fConst0);
		fConst54 = marimba_faustpower2_f(fConst51);
		fConst55 = std::pow(0.00100000005f, (1.0f / (0.000124247395f * fConst0)));
		fConst56 = (0.0f - (2.0f * fConst55));
		fConst57 = (399620.375f / fConst0);
		fConst58 = marimba_faustpower2_f(fConst55);
		fConst59 = std::pow(0.00100000005f, (1.0f / (2.61844324e-10f * fConst0)));
		fConst60 = (0.0f - (2.0f * fConst59));
		fConst61 = (531328.75f / fConst0);
		fConst62 = marimba_faustpower2_f(fConst59);
		fConst63 = std::pow(0.00100000005f, (1.0f / (1.88244775e-09f * fConst0)));
		fConst64 = (0.0f - (2.0f * fConst63));
		fConst65 = (526294.438f / fConst0);
		fConst66 = marimba_faustpower2_f(fConst63);
		fConst67 = std::pow(0.00100000005f, (1.0f / (0.000329498813f * fConst0)));
		fConst68 = (0.0f - (2.0f * fConst67));
		fConst69 = (369010.719f / fConst0);
		fConst70 = marimba_faustpower2_f(fConst67);
		fConst71 = std::pow(0.00100000005f, (1.0f / (0.000362893392f * fConst0)));
		fConst72 = (0.0f - (2.0f * fConst71));
		fConst73 = (365643.438f / fConst0);
		fConst74 = marimba_faustpower2_f(fConst71);
		fConst75 = std::pow(0.00100000005f, (1.0f / (0.000432563043f * fConst0)));
		fConst76 = (0.0f - (2.0f * fConst75));
		fConst77 = (359348.438f / fConst0);
		fConst78 = marimba_faustpower2_f(fConst75);
		fConst79 = std::pow(0.00100000005f, (1.0f / (0.000537810789f * fConst0)));
		fConst80 = (0.0f - (2.0f * fConst79));
		fConst81 = (351228.812f / fConst0);
		fConst82 = marimba_faustpower2_f(fConst79);
		fConst83 = std::pow(0.00100000005f, (1.0f / (0.000912711956f * fConst0)));
		fConst84 = (0.0f - (2.0f * fConst83));
		fConst85 = (329971.781f / fConst0);
		fConst86 = marimba_faustpower2_f(fConst83);
		fConst87 = std::pow(0.00100000005f, (1.0f / (0.00129330496f * fConst0)));
		fConst88 = (0.0f - (2.0f * fConst87));
		fConst89 = (314683.531f / fConst0);
		fConst90 = marimba_faustpower2_f(fConst87);
		fConst91 = std::pow(0.00100000005f, (1.0f / (0.00143769139f * fConst0)));
		fConst92 = (0.0f - (2.0f * fConst91));
		fConst93 = (309826.125f / fConst0);
		fConst94 = marimba_faustpower2_f(fConst91);
		fConst95 = std::pow(0.00100000005f, (1.0f / (0.0015662252f * fConst0)));
		fConst96 = (0.0f - (2.0f * fConst95));
		fConst97 = (305820.219f / fConst0);
		fConst98 = marimba_faustpower2_f(fConst95);
		fConst99 = std::pow(0.00100000005f, (1.0f / (0.00320503488f * fConst0)));
		fConst100 = (0.0f - (2.0f * fConst99));
		fConst101 = (269495.281f / fConst0);
		fConst102 = marimba_faustpower2_f(fConst99);
		fConst103 = std::pow(0.00100000005f, (1.0f / (0.00359531236f * fConst0)));
		fConst104 = (0.0f - (2.0f * fConst103));
		fConst105 = (263166.281f / fConst0);
		fConst106 = marimba_faustpower2_f(fConst103);
		fConst107 = std::pow(0.00100000005f, (1.0f / (0.00401024008f * fConst0)));
		fConst108 = (0.0f - (2.0f * fConst107));
		fConst109 = (257014.219f / fConst0);
		fConst110 = marimba_faustpower2_f(fConst107);
		fConst111 = std::pow(0.00100000005f, (1.0f / (0.00462675327f * fConst0)));
		fConst112 = (0.0f - (2.0f * fConst111));
		fConst113 = (248753.312f / fConst0);
		fConst114 = marimba_faustpower2_f(fConst111);
		fConst115 = std::pow(0.00100000005f, (1.0f / (0.00699721137f * fConst0)));
		fConst116 = (0.0f - (2.0f * fConst115));
		fConst117 = (223483.484f / fConst0);
		fConst118 = marimba_faustpower2_f(fConst115);
		fConst119 = std::pow(0.00100000005f, (1.0f / (0.00767402584f * fConst0)));
		fConst120 = (0.0f - (2.0f * fConst119));
		fConst121 = (217552.031f / fConst0);
		fConst122 = marimba_faustpower2_f(fConst119);
		fConst123 = std::pow(0.00100000005f, (1.0f / (0.00913601927f * fConst0)));
		fConst124 = (0.0f - (2.0f * fConst123));
		fConst125 = (206046.031f / fConst0);
		fConst126 = marimba_faustpower2_f(fConst123);
		fConst127 = std::pow(0.00100000005f, (1.0f / (0.0123624494f * fConst0)));
		fConst128 = (0.0f - (2.0f * fConst127));
		fConst129 = (185114.125f / fConst0);
		fConst130 = marimba_faustpower2_f(fConst127);
		fConst131 = std::pow(0.00100000005f, (1.0f / (0.0131463278f * fConst0)));
		fConst132 = (0.0f - (2.0f * fConst131));
		fConst133 = (180702.094f / fConst0);
		fConst134 = marimba_faustpower2_f(fConst131);
		fConst135 = std::pow(0.00100000005f, (1.0f / (0.0140852043f * fConst0)));
		fConst136 = (0.0f - (2.0f * fConst135));
		fConst137 = (175686.562f / fConst0);
		fConst138 = marimba_faustpower2_f(fConst135);
		fConst139 = std::pow(0.00100000005f, (1.0f / (0.0171158724f * fConst0)));
		fConst140 = (0.0f - (2.0f * fConst139));
		fConst141 = (161137.562f / fConst0);
		fConst142 = marimba_faustpower2_f(fConst139);
		fConst143 = std::pow(0.00100000005f, (1.0f / (0.0210792925f * fConst0)));
		fConst144 = (0.0f - (2.0f * fConst143));
		fConst145 = (144948.062f / fConst0);
		fConst146 = marimba_faustpower2_f(fConst143);
		fConst147 = std::pow(0.00100000005f, (1.0f / (0.0221360233f * fConst0)));
		fConst148 = (0.0f - (2.0f * fConst147));
		fConst149 = (141047.203f / fConst0);
		fConst150 = marimba_faustpower2_f(fConst147);
		fConst151 = std::pow(0.00100000005f, (1.0f / (0.0248362795f * fConst0)));
		fConst152 = (0.0f - (2.0f * fConst151));
		fConst153 = (131716.406f / fConst0);
		fConst154 = marimba_faustpower2_f(fConst151);
		fConst155 = std::pow(0.00100000005f, (1.0f / (0.0688085929f * fConst0)));
		fConst156 = (0.0f - (2.0f * fConst155));
		fConst157 = (39027.6016f / fConst0);
		fConst158 = marimba_faustpower2_f(fConst155);
		fConst159 = std::pow(0.00100000005f, (1.0f / (0.0781081766f * fConst0)));
		fConst160 = (0.0f - (2.0f * fConst159));
		fConst161 = (26119.3477f / fConst0);
		fConst162 = marimba_faustpower2_f(fConst159);
		fConst163 = std::pow(0.00100000005f, (1.0f / (0.0810509697f * fConst0)));
		fConst164 = (0.0f - (2.0f * fConst163));
		fConst165 = (22291.3418f / fConst0);
		fConst166 = marimba_faustpower2_f(fConst163);
		fConst167 = std::pow(0.00100000005f, (1.0f / (0.0905909166f * fConst0)));
		fConst168 = (0.0f - (2.0f * fConst167));
		fConst169 = (10601.3896f / fConst0);
		fConst170 = marimba_faustpower2_f(fConst167);
		fConst171 = std::pow(0.00100000005f, (1.0f / (0.0974743068f * fConst0)));
		fConst172 = (0.0f - (2.0f * fConst171));
		fConst173 = (2764.60156f / fConst0);
		fConst174 = marimba_faustpower2_f(fConst171);
		fConst175 = std::pow(0.00100000005f, (1.0f / (0.0918262452f * fConst0)));
		fConst176 = (0.0f - (2.0f * fConst175));
		fConst177 = (9160.67285f / fConst0);
		fConst178 = marimba_faustpower2_f(fConst175);
		fConst179 = std::pow(0.00100000005f, (1.0f / (0.0611052401f * fConst0)));
		fConst180 = (0.0f - (2.0f * fConst179));
		fConst181 = (50824.4336f / fConst0);
		fConst182 = marimba_faustpower2_f(fConst179);
		fConst183 = std::pow(0.00100000005f, (1.0f / (0.0567183308f * fConst0)));
		fConst184 = (0.0f - (2.0f * fConst183));
		fConst185 = (58084.832f / fConst0);
		fConst186 = marimba_faustpower2_f(fConst183);
		fConst187 = std::pow(0.00100000005f, (1.0f / (0.0488223769f * fConst0)));
		fConst188 = (0.0f - (2.0f * fConst187));
		fConst189 = (72370.3594f / fConst0);
		fConst190 = marimba_faustpower2_f(fConst187);
		fConst191 = std::pow(0.00100000005f, (1.0f / (0.0449043326f * fConst0)));
		fConst192 = (0.0f - (2.0f * fConst191));
		fConst193 = (80157.9609f / fConst0);
		fConst194 = marimba_faustpower2_f(fConst191);
		fConst195 = std::pow(0.00100000005f, (1.0f / (0.0350424498f * fConst0)));
		fConst196 = (0.0f - (2.0f * fConst195));
		fConst197 = (102491.523f / fConst0);
		fConst198 = marimba_faustpower2_f(fConst195);
		fConst199 = std::pow(0.00100000005f, (1.0f / (0.0341717675f * fConst0)));
		fConst200 = (0.0f - (2.0f * fConst199));
		fConst201 = (104696.289f / fConst0);
		fConst202 = marimba_faustpower2_f(fConst199);
		fConst203 = std::pow(0.00100000005f, (1.0f / (0.031866312f * fConst0)));
		fConst204 = (0.0f - (2.0f * fConst203));
		fConst205 = (110759.336f / fConst0);
		fConst206 = marimba_faustpower2_f(fConst203);
		fConst207 = std::pow(0.00100000005f, (1.0f / (6.3867631e-15f * fConst0)));
		fConst208 = (0.0f - (2.0f * fConst207));
		fConst209 = (540493.438f / fConst0);
		fConst210 = marimba_faustpower2_f(fConst207);
		fConst211 = std::pow(0.00100000005f, (1.0f / (4.53858888e-08f * fConst0)));
		fConst212 = (0.0f - (2.0f * fConst211));
		fConst213 = (512551.594f / fConst0);
		fConst214 = marimba_faustpower2_f(fConst211);
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(48.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(7000.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(0.25f);
		
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			iRec0[l0] = 0;
			
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec4[l1] = 0.0f;
			
		}
		IOTA = 0;
		for (int l2 = 0; (l2 < 2048); l2 = (l2 + 1)) {
			fVec0[l2] = 0.0f;
			
		}
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec2[l3] = 0.0f;
			
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec8[l4] = 0;
			
		}
		for (int l5 = 0; (l5 < 3); l5 = (l5 + 1)) {
			fRec7[l5] = 0.0f;
			
		}
		for (int l6 = 0; (l6 < 3); l6 = (l6 + 1)) {
			fRec6[l6] = 0.0f;
			
		}
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fVec1[l7] = 0.0f;
			
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fVec2[l8] = 0.0f;
			
		}
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec9[l9] = 0.0f;
			
		}
		for (int l10 = 0; (l10 < 3); l10 = (l10 + 1)) {
			fRec5[l10] = 0.0f;
			
		}
		for (int l11 = 0; (l11 < 3); l11 = (l11 + 1)) {
			fRec10[l11] = 0.0f;
			
		}
		for (int l12 = 0; (l12 < 3); l12 = (l12 + 1)) {
			fRec11[l12] = 0.0f;
			
		}
		for (int l13 = 0; (l13 < 3); l13 = (l13 + 1)) {
			fRec12[l13] = 0.0f;
			
		}
		for (int l14 = 0; (l14 < 3); l14 = (l14 + 1)) {
			fRec13[l14] = 0.0f;
			
		}
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec14[l15] = 0.0f;
			
		}
		for (int l16 = 0; (l16 < 3); l16 = (l16 + 1)) {
			fRec15[l16] = 0.0f;
			
		}
		for (int l17 = 0; (l17 < 3); l17 = (l17 + 1)) {
			fRec16[l17] = 0.0f;
			
		}
		for (int l18 = 0; (l18 < 3); l18 = (l18 + 1)) {
			fRec17[l18] = 0.0f;
			
		}
		for (int l19 = 0; (l19 < 3); l19 = (l19 + 1)) {
			fRec18[l19] = 0.0f;
			
		}
		for (int l20 = 0; (l20 < 3); l20 = (l20 + 1)) {
			fRec19[l20] = 0.0f;
			
		}
		for (int l21 = 0; (l21 < 3); l21 = (l21 + 1)) {
			fRec20[l21] = 0.0f;
			
		}
		for (int l22 = 0; (l22 < 3); l22 = (l22 + 1)) {
			fRec21[l22] = 0.0f;
			
		}
		for (int l23 = 0; (l23 < 3); l23 = (l23 + 1)) {
			fRec22[l23] = 0.0f;
			
		}
		for (int l24 = 0; (l24 < 3); l24 = (l24 + 1)) {
			fRec23[l24] = 0.0f;
			
		}
		for (int l25 = 0; (l25 < 3); l25 = (l25 + 1)) {
			fRec24[l25] = 0.0f;
			
		}
		for (int l26 = 0; (l26 < 3); l26 = (l26 + 1)) {
			fRec25[l26] = 0.0f;
			
		}
		for (int l27 = 0; (l27 < 3); l27 = (l27 + 1)) {
			fRec26[l27] = 0.0f;
			
		}
		for (int l28 = 0; (l28 < 3); l28 = (l28 + 1)) {
			fRec27[l28] = 0.0f;
			
		}
		for (int l29 = 0; (l29 < 3); l29 = (l29 + 1)) {
			fRec28[l29] = 0.0f;
			
		}
		for (int l30 = 0; (l30 < 3); l30 = (l30 + 1)) {
			fRec29[l30] = 0.0f;
			
		}
		for (int l31 = 0; (l31 < 3); l31 = (l31 + 1)) {
			fRec30[l31] = 0.0f;
			
		}
		for (int l32 = 0; (l32 < 3); l32 = (l32 + 1)) {
			fRec31[l32] = 0.0f;
			
		}
		for (int l33 = 0; (l33 < 3); l33 = (l33 + 1)) {
			fRec32[l33] = 0.0f;
			
		}
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec33[l34] = 0.0f;
			
		}
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec34[l35] = 0.0f;
			
		}
		for (int l36 = 0; (l36 < 3); l36 = (l36 + 1)) {
			fRec35[l36] = 0.0f;
			
		}
		for (int l37 = 0; (l37 < 3); l37 = (l37 + 1)) {
			fRec36[l37] = 0.0f;
			
		}
		for (int l38 = 0; (l38 < 3); l38 = (l38 + 1)) {
			fRec37[l38] = 0.0f;
			
		}
		for (int l39 = 0; (l39 < 3); l39 = (l39 + 1)) {
			fRec38[l39] = 0.0f;
			
		}
		for (int l40 = 0; (l40 < 3); l40 = (l40 + 1)) {
			fRec39[l40] = 0.0f;
			
		}
		for (int l41 = 0; (l41 < 3); l41 = (l41 + 1)) {
			fRec40[l41] = 0.0f;
			
		}
		for (int l42 = 0; (l42 < 3); l42 = (l42 + 1)) {
			fRec41[l42] = 0.0f;
			
		}
		for (int l43 = 0; (l43 < 3); l43 = (l43 + 1)) {
			fRec42[l43] = 0.0f;
			
		}
		for (int l44 = 0; (l44 < 3); l44 = (l44 + 1)) {
			fRec43[l44] = 0.0f;
			
		}
		for (int l45 = 0; (l45 < 3); l45 = (l45 + 1)) {
			fRec44[l45] = 0.0f;
			
		}
		for (int l46 = 0; (l46 < 3); l46 = (l46 + 1)) {
			fRec45[l46] = 0.0f;
			
		}
		for (int l47 = 0; (l47 < 3); l47 = (l47 + 1)) {
			fRec46[l47] = 0.0f;
			
		}
		for (int l48 = 0; (l48 < 3); l48 = (l48 + 1)) {
			fRec47[l48] = 0.0f;
			
		}
		for (int l49 = 0; (l49 < 3); l49 = (l49 + 1)) {
			fRec48[l49] = 0.0f;
			
		}
		for (int l50 = 0; (l50 < 3); l50 = (l50 + 1)) {
			fRec49[l50] = 0.0f;
			
		}
		for (int l51 = 0; (l51 < 3); l51 = (l51 + 1)) {
			fRec50[l51] = 0.0f;
			
		}
		for (int l52 = 0; (l52 < 3); l52 = (l52 + 1)) {
			fRec51[l52] = 0.0f;
			
		}
		for (int l53 = 0; (l53 < 3); l53 = (l53 + 1)) {
			fRec52[l53] = 0.0f;
			
		}
		for (int l54 = 0; (l54 < 3); l54 = (l54 + 1)) {
			fRec53[l54] = 0.0f;
			
		}
		for (int l55 = 0; (l55 < 3); l55 = (l55 + 1)) {
			fRec54[l55] = 0.0f;
			
		}
		for (int l56 = 0; (l56 < 3); l56 = (l56 + 1)) {
			fRec55[l56] = 0.0f;
			
		}
		for (int l57 = 0; (l57 < 3); l57 = (l57 + 1)) {
			fRec56[l57] = 0.0f;
			
		}
		for (int l58 = 0; (l58 < 3); l58 = (l58 + 1)) {
			fRec57[l58] = 0.0f;
			
		}
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec58[l59] = 0.0f;
			
		}
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fVec3[l60] = 0.0f;
			
		}
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec59[l61] = 0.0f;
			
		}
		for (int l62 = 0; (l62 < 2048); l62 = (l62 + 1)) {
			fVec4[l62] = 0.0f;
			
		}
		for (int l63 = 0; (l63 < 2); l63 = (l63 + 1)) {
			fRec3[l63] = 0.0f;
			
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
	
	virtual marimba* clone() {
		return new marimba();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("marimba");
		ui_interface->addHorizontalSlider("cutoff", &fHslider2, 7000.0f, 1000.0f, 10000.0f, 1.0f);
		ui_interface->addButton("gate", &fButton0);
		ui_interface->addHorizontalSlider("pitch", &fHslider0, 48.0f, 36.0f, 84.0f, 0.00100000005f);
		ui_interface->addHorizontalSlider("pos", &fHslider1, 0.0f, 0.0f, 4.0f, 0.00999999978f);
		ui_interface->addHorizontalSlider("sharp", &fHslider3, 0.25f, 0.00999999978f, 1.0f, 0.00100000005f);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = std::pow(2.0f, (0.0833333358f * (float(fHslider0) + -69.0f)));
		float fSlow1 = (fConst1 * ((0.772727251f / fSlow0) + -0.0399999991f));
		float fSlow2 = (fSlow1 + -1.49999499f);
		float fSlow3 = std::floor(fSlow2);
		float fSlow4 = (fSlow1 + (-2.0f - fSlow3));
		float fSlow5 = (fSlow1 + (-3.0f - fSlow3));
		float fSlow6 = (fSlow1 + (-4.0f - fSlow3));
		float fSlow7 = ((((0.0f - (0.5f * fSlow4)) * (fSlow3 + (1.0f - fSlow1))) * (0.0f - (0.333333343f * fSlow5))) * (0.0f - (0.25f * fSlow6)));
		int iSlow8 = int(fSlow2);
		int iSlow9 = int(std::min(fConst2, float(std::max(0, iSlow8))));
		float fSlow10 = (fSlow1 - fSlow3);
		float fSlow11 = (((0.0f - (0.5f * fSlow5)) * (fSlow3 + (2.0f - fSlow1))) * (0.0f - (0.333333343f * fSlow6)));
		int iSlow12 = int(std::min(fConst2, float(std::max(0, (iSlow8 + 1)))));
		float fSlow13 = (fSlow1 + (-1.0f - fSlow3));
		float fSlow14 = (0.5f * ((0.0f - (0.5f * fSlow6)) * (fSlow3 + (3.0f - fSlow1))));
		int iSlow15 = int(std::min(fConst2, float(std::max(0, (iSlow8 + 2)))));
		float fSlow16 = (fSlow3 + (4.0f - fSlow1));
		float fSlow17 = (0.166666672f * fSlow16);
		int iSlow18 = int(std::min(fConst2, float(std::max(0, (iSlow8 + 3)))));
		float fSlow19 = (0.0416666679f * fSlow5);
		int iSlow20 = int(std::min(fConst2, float(std::max(0, (iSlow8 + 4)))));
		int iSlow21 = (50 * int(float(fHslider1)));
		float fSlow22 = (((79852.0781f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 45)]:0.0f);
		float fSlow23 = std::tan((fConst4 * float(fHslider2)));
		float fSlow24 = (1.0f / fSlow23);
		float fSlow25 = (1.0f / (((fSlow24 + 1.41421354f) / fSlow23) + 1.0f));
		float fSlow26 = (((fSlow24 + -1.41421354f) / fSlow23) + 1.0f);
		float fSlow27 = (2.0f * (1.0f - (1.0f / marimba_faustpower2_f(fSlow23))));
		float fSlow28 = float(fButton0);
		float fSlow29 = float(fHslider3);
		float fSlow30 = (fConst12 * fSlow29);
		float fSlow31 = (fConst13 * fSlow29);
		float fSlow32 = (fConst14 / fSlow29);
		float fSlow33 = (fConst16 * std::cos((fConst17 * fSlow0)));
		float fSlow34 = (((78732.2812f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 44)]:0.0f);
		float fSlow35 = (fConst20 * std::cos((fConst21 * fSlow0)));
		float fSlow36 = (((76764.1562f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 43)]:0.0f);
		float fSlow37 = (fConst24 * std::cos((fConst25 * fSlow0)));
		float fSlow38 = (((75619.7188f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 42)]:0.0f);
		float fSlow39 = (fConst28 * std::cos((fConst29 * fSlow0)));
		float fSlow40 = (((73966.2031f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 41)]:0.0f);
		float fSlow41 = (fConst32 * std::cos((fConst33 * fSlow0)));
		float fSlow42 = (((69600.5234f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 40)]:0.0f);
		float fSlow43 = (fConst36 * std::cos((fConst37 * fSlow0)));
		float fSlow44 = (((68462.6797f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 39)]:0.0f);
		float fSlow45 = (fConst40 * std::cos((fConst41 * fSlow0)));
		float fSlow46 = (((67393.0391f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 38)]:0.0f);
		float fSlow47 = (fConst44 * std::cos((fConst45 * fSlow0)));
		float fSlow48 = (((66894.5234f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 37)]:0.0f);
		float fSlow49 = (fConst48 * std::cos((fConst49 * fSlow0)));
		float fSlow50 = (((65752.7188f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 36)]:0.0f);
		float fSlow51 = (fConst52 * std::cos((fConst53 * fSlow0)));
		float fSlow52 = (((63601.5586f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 35)]:0.0f);
		float fSlow53 = (fConst56 * std::cos((fConst57 * fSlow0)));
		float fSlow54 = (((84563.6016f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 48)]:0.0f);
		float fSlow55 = (fConst60 * std::cos((fConst61 * fSlow0)));
		float fSlow56 = (((83762.3594f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 47)]:0.0f);
		float fSlow57 = (fConst64 * std::cos((fConst65 * fSlow0)));
		float fSlow58 = (((58729.8789f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 34)]:0.0f);
		float fSlow59 = (fConst68 * std::cos((fConst69 * fSlow0)));
		float fSlow60 = (((58193.9609f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 33)]:0.0f);
		float fSlow61 = (fConst72 * std::cos((fConst73 * fSlow0)));
		float fSlow62 = (((57192.0781f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 32)]:0.0f);
		float fSlow63 = (fConst76 * std::cos((fConst77 * fSlow0)));
		float fSlow64 = (((55899.8008f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 31)]:0.0f);
		float fSlow65 = (fConst80 * std::cos((fConst81 * fSlow0)));
		float fSlow66 = (((52516.6406f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 30)]:0.0f);
		float fSlow67 = (fConst84 * std::cos((fConst85 * fSlow0)));
		float fSlow68 = (((50083.4414f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 29)]:0.0f);
		float fSlow69 = (fConst88 * std::cos((fConst89 * fSlow0)));
		float fSlow70 = (((49310.3594f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 28)]:0.0f);
		float fSlow71 = (fConst92 * std::cos((fConst93 * fSlow0)));
		float fSlow72 = (((48672.8008f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 27)]:0.0f);
		float fSlow73 = (fConst96 * std::cos((fConst97 * fSlow0)));
		float fSlow74 = (((42891.5078f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 26)]:0.0f);
		float fSlow75 = (fConst100 * std::cos((fConst101 * fSlow0)));
		float fSlow76 = (((41884.2148f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 25)]:0.0f);
		float fSlow77 = (fConst104 * std::cos((fConst105 * fSlow0)));
		float fSlow78 = (((40905.0859f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 24)]:0.0f);
		float fSlow79 = (fConst108 * std::cos((fConst109 * fSlow0)));
		float fSlow80 = (((39590.3203f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 23)]:0.0f);
		float fSlow81 = (fConst112 * std::cos((fConst113 * fSlow0)));
		float fSlow82 = (((35568.5f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 22)]:0.0f);
		float fSlow83 = (fConst116 * std::cos((fConst117 * fSlow0)));
		float fSlow84 = (((34624.4805f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 21)]:0.0f);
		float fSlow85 = (fConst120 * std::cos((fConst121 * fSlow0)));
		float fSlow86 = (((32793.2422f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 20)]:0.0f);
		float fSlow87 = (fConst124 * std::cos((fConst125 * fSlow0)));
		float fSlow88 = (((29461.8281f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 19)]:0.0f);
		float fSlow89 = (fConst128 * std::cos((fConst129 * fSlow0)));
		float fSlow90 = (((28759.6328f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 18)]:0.0f);
		float fSlow91 = (fConst132 * std::cos((fConst133 * fSlow0)));
		float fSlow92 = (((27961.3848f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 17)]:0.0f);
		float fSlow93 = (fConst136 * std::cos((fConst137 * fSlow0)));
		float fSlow94 = (((25645.8398f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 16)]:0.0f);
		float fSlow95 = (fConst140 * std::cos((fConst141 * fSlow0)));
		float fSlow96 = (((23069.1992f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 15)]:0.0f);
		float fSlow97 = (fConst144 * std::cos((fConst145 * fSlow0)));
		float fSlow98 = (((22448.3594f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 14)]:0.0f);
		float fSlow99 = (fConst148 * std::cos((fConst149 * fSlow0)));
		float fSlow100 = (((20963.3164f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 13)]:0.0f);
		float fSlow101 = (fConst152 * std::cos((fConst153 * fSlow0)));
		float fSlow102 = (((6211.43604f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 5)]:0.0f);
		float fSlow103 = (fConst156 * std::cos((fConst157 * fSlow0)));
		float fSlow104 = (((4157.02344f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 4)]:0.0f);
		float fSlow105 = (fConst160 * std::cos((fConst161 * fSlow0)));
		float fSlow106 = (((3547.7771f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 3)]:0.0f);
		float fSlow107 = (fConst164 * std::cos((fConst165 * fSlow0)));
		float fSlow108 = (((1687.26355f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 2)]:0.0f);
		float fSlow109 = (fConst168 * std::cos((fConst169 * fSlow0)));
		float fSlow110 = (((440.0f * fSlow0) < fConst3)?ftbl0marimbaSIG0[iSlow21]:0.0f);
		float fSlow111 = (fConst172 * std::cos((fConst173 * fSlow0)));
		float fSlow112 = (((1457.96643f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 1)]:0.0f);
		float fSlow113 = (fConst176 * std::cos((fConst177 * fSlow0)));
		float fSlow114 = (((8088.95996f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 6)]:0.0f);
		float fSlow115 = (fConst180 * std::cos((fConst181 * fSlow0)));
		float fSlow116 = (((9244.48828f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 7)]:0.0f);
		float fSlow117 = (fConst184 * std::cos((fConst185 * fSlow0)));
		float fSlow118 = (((11518.0996f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 8)]:0.0f);
		float fSlow119 = (fConst188 * std::cos((fConst189 * fSlow0)));
		float fSlow120 = (((12757.5361f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 9)]:0.0f);
		float fSlow121 = (fConst192 * std::cos((fConst193 * fSlow0)));
		float fSlow122 = (((16312.0322f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 10)]:0.0f);
		float fSlow123 = (fConst196 * std::cos((fConst197 * fSlow0)));
		float fSlow124 = (((16662.9316f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 11)]:0.0f);
		float fSlow125 = (fConst200 * std::cos((fConst201 * fSlow0)));
		float fSlow126 = (((17627.8965f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 12)]:0.0f);
		float fSlow127 = (fConst204 * std::cos((fConst205 * fSlow0)));
		float fSlow128 = (((86022.2031f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 49)]:0.0f);
		float fSlow129 = (fConst208 * std::cos((fConst209 * fSlow0)));
		float fSlow130 = (((81575.1172f * fSlow0) < fConst3)?ftbl0marimbaSIG0[(iSlow21 + 46)]:0.0f);
		float fSlow131 = (fConst212 * std::cos((fConst213 * fSlow0)));
		for (int i = 0; (i < count); i = (i + 1)) {
			iRec0[0] = 0;
			fRec4[0] = ((0.0500000007f * fRec3[1]) + (0.949999988f * fRec4[1]));
			float fTemp0 = (float(iRec0[1]) + (0.99000001f * fRec4[0]));
			fVec0[(IOTA & 2047)] = fTemp0;
			fRec2[0] = ((fSlow7 * fVec0[((IOTA - iSlow9) & 2047)]) + (fSlow10 * ((fSlow11 * fVec0[((IOTA - iSlow12) & 2047)]) + (fSlow13 * ((fSlow14 * fVec0[((IOTA - iSlow15) & 2047)]) + (fSlow4 * ((fSlow17 * fVec0[((IOTA - iSlow18) & 2047)]) + (fSlow19 * fVec0[((IOTA - iSlow20) & 2047)]))))))));
			iRec8[0] = ((1103515245 * iRec8[1]) + 12345);
			fRec7[0] = ((4.65661287e-10f * float(iRec8[0])) - (fConst7 * ((fConst9 * fRec7[2]) + (fConst10 * fRec7[1]))));
			fRec6[0] = ((fConst7 * ((fConst8 * (fRec7[2] + fRec7[0])) + (fConst11 * fRec7[1]))) - (fSlow25 * ((fSlow26 * fRec6[2]) + (fSlow27 * fRec6[1]))));
			fVec1[0] = fSlow28;
			fVec2[0] = fSlow29;
			fRec9[0] = ((((fSlow28 - fVec1[1]) > 0.0f) > 0)?0.0f:std::min(fSlow30, (((fConst12 * (fSlow29 - fVec2[1])) + fRec9[1]) + 1.0f)));
			int iTemp1 = (fRec9[0] < fSlow31);
			float fTemp2 = (fSlow25 * (((fRec6[2] + (2.0f * fRec6[1])) + fRec6[0]) * (iTemp1?((fRec9[0] < 0.0f)?0.0f:(iTemp1?(fSlow32 * fRec9[0]):1.0f)):((fRec9[0] < fSlow30)?((fSlow32 * (fSlow31 - fRec9[0])) + 1.0f):0.0f))));
			fRec5[0] = (fTemp2 - ((fSlow33 * fRec5[1]) + (fConst18 * fRec5[2])));
			fRec10[0] = (fTemp2 - ((fSlow35 * fRec10[1]) + (fConst22 * fRec10[2])));
			fRec11[0] = (fTemp2 - ((fSlow37 * fRec11[1]) + (fConst26 * fRec11[2])));
			fRec12[0] = (fTemp2 - ((fSlow39 * fRec12[1]) + (fConst30 * fRec12[2])));
			fRec13[0] = (fTemp2 - ((fSlow41 * fRec13[1]) + (fConst34 * fRec13[2])));
			fRec14[0] = (fTemp2 - ((fSlow43 * fRec14[1]) + (fConst38 * fRec14[2])));
			fRec15[0] = (fTemp2 - ((fSlow45 * fRec15[1]) + (fConst42 * fRec15[2])));
			fRec16[0] = (fTemp2 - ((fSlow47 * fRec16[1]) + (fConst46 * fRec16[2])));
			fRec17[0] = (fTemp2 - ((fSlow49 * fRec17[1]) + (fConst50 * fRec17[2])));
			fRec18[0] = (fTemp2 - ((fSlow51 * fRec18[1]) + (fConst54 * fRec18[2])));
			fRec19[0] = (fTemp2 - ((fSlow53 * fRec19[1]) + (fConst58 * fRec19[2])));
			fRec20[0] = (fTemp2 - ((fSlow55 * fRec20[1]) + (fConst62 * fRec20[2])));
			fRec21[0] = (fTemp2 - ((fSlow57 * fRec21[1]) + (fConst66 * fRec21[2])));
			fRec22[0] = (fTemp2 - ((fSlow59 * fRec22[1]) + (fConst70 * fRec22[2])));
			fRec23[0] = (fTemp2 - ((fSlow61 * fRec23[1]) + (fConst74 * fRec23[2])));
			fRec24[0] = (fTemp2 - ((fSlow63 * fRec24[1]) + (fConst78 * fRec24[2])));
			fRec25[0] = (fTemp2 - ((fSlow65 * fRec25[1]) + (fConst82 * fRec25[2])));
			fRec26[0] = (fTemp2 - ((fSlow67 * fRec26[1]) + (fConst86 * fRec26[2])));
			fRec27[0] = (fTemp2 - ((fSlow69 * fRec27[1]) + (fConst90 * fRec27[2])));
			fRec28[0] = (fTemp2 - ((fSlow71 * fRec28[1]) + (fConst94 * fRec28[2])));
			fRec29[0] = (fTemp2 - ((fSlow73 * fRec29[1]) + (fConst98 * fRec29[2])));
			fRec30[0] = (fTemp2 - ((fSlow75 * fRec30[1]) + (fConst102 * fRec30[2])));
			fRec31[0] = (fTemp2 - ((fSlow77 * fRec31[1]) + (fConst106 * fRec31[2])));
			fRec32[0] = (fTemp2 - ((fSlow79 * fRec32[1]) + (fConst110 * fRec32[2])));
			fRec33[0] = (fTemp2 - ((fSlow81 * fRec33[1]) + (fConst114 * fRec33[2])));
			fRec34[0] = (fTemp2 - ((fSlow83 * fRec34[1]) + (fConst118 * fRec34[2])));
			fRec35[0] = (fTemp2 - ((fSlow85 * fRec35[1]) + (fConst122 * fRec35[2])));
			fRec36[0] = (fTemp2 - ((fSlow87 * fRec36[1]) + (fConst126 * fRec36[2])));
			fRec37[0] = (fTemp2 - ((fSlow89 * fRec37[1]) + (fConst130 * fRec37[2])));
			fRec38[0] = (fTemp2 - ((fSlow91 * fRec38[1]) + (fConst134 * fRec38[2])));
			fRec39[0] = (fTemp2 - ((fSlow93 * fRec39[1]) + (fConst138 * fRec39[2])));
			fRec40[0] = (fTemp2 - ((fSlow95 * fRec40[1]) + (fConst142 * fRec40[2])));
			fRec41[0] = (fTemp2 - ((fSlow97 * fRec41[1]) + (fConst146 * fRec41[2])));
			fRec42[0] = (fTemp2 - ((fSlow99 * fRec42[1]) + (fConst150 * fRec42[2])));
			fRec43[0] = (fTemp2 - ((fSlow101 * fRec43[1]) + (fConst154 * fRec43[2])));
			fRec44[0] = (fTemp2 - ((fSlow103 * fRec44[1]) + (fConst158 * fRec44[2])));
			fRec45[0] = (fTemp2 - ((fSlow105 * fRec45[1]) + (fConst162 * fRec45[2])));
			fRec46[0] = (fTemp2 - ((fSlow107 * fRec46[1]) + (fConst166 * fRec46[2])));
			fRec47[0] = (fTemp2 - ((fSlow109 * fRec47[1]) + (fConst170 * fRec47[2])));
			fRec48[0] = (fTemp2 - ((fSlow111 * fRec48[1]) + (fConst174 * fRec48[2])));
			fRec49[0] = (fTemp2 - ((fSlow113 * fRec49[1]) + (fConst178 * fRec49[2])));
			fRec50[0] = (fTemp2 - ((fSlow115 * fRec50[1]) + (fConst182 * fRec50[2])));
			fRec51[0] = (fTemp2 - ((fSlow117 * fRec51[1]) + (fConst186 * fRec51[2])));
			fRec52[0] = (fTemp2 - ((fSlow119 * fRec52[1]) + (fConst190 * fRec52[2])));
			fRec53[0] = (fTemp2 - ((fSlow121 * fRec53[1]) + (fConst194 * fRec53[2])));
			fRec54[0] = (fTemp2 - ((fSlow123 * fRec54[1]) + (fConst198 * fRec54[2])));
			fRec55[0] = (fTemp2 - ((fSlow125 * fRec55[1]) + (fConst202 * fRec55[2])));
			fRec56[0] = (fTemp2 - ((fSlow127 * fRec56[1]) + (fConst206 * fRec56[2])));
			fRec57[0] = (fTemp2 - ((fSlow129 * fRec57[1]) + (fConst210 * fRec57[2])));
			fRec58[0] = (fTemp2 - ((fSlow131 * fRec58[1]) + (fConst214 * fRec58[2])));
			fVec3[0] = ((((((((((((((((((((((((((((((((((((((((((((((((((fSlow22 * (fRec5[0] - fRec5[2])) + (fSlow34 * (fRec10[0] - fRec10[2]))) + (fSlow36 * (fRec11[0] - fRec11[2]))) + (fSlow38 * (fRec12[0] - fRec12[2]))) + (fSlow40 * (fRec13[0] - fRec13[2]))) + (fSlow42 * (fRec14[0] - fRec14[2]))) + (fSlow44 * (fRec15[0] - fRec15[2]))) + (fSlow46 * (fRec16[0] - fRec16[2]))) + (fSlow48 * (fRec17[0] - fRec17[2]))) + (fSlow50 * (fRec18[0] - fRec18[2]))) + (fSlow52 * (fRec19[0] - fRec19[2]))) + (fSlow54 * (fRec20[0] - fRec20[2]))) + (fSlow56 * (fRec21[0] - fRec21[2]))) + (fSlow58 * (fRec22[0] - fRec22[2]))) + (fSlow60 * (fRec23[0] - fRec23[2]))) + (fSlow62 * (fRec24[0] - fRec24[2]))) + (fSlow64 * (fRec25[0] - fRec25[2]))) + (fSlow66 * (fRec26[0] - fRec26[2]))) + (fSlow68 * (fRec27[0] - fRec27[2]))) + (fSlow70 * (fRec28[0] - fRec28[2]))) + (fSlow72 * (fRec29[0] - fRec29[2]))) + (fSlow74 * (fRec30[0] - fRec30[2]))) + (fSlow76 * (fRec31[0] - fRec31[2]))) + (fSlow78 * (fRec32[0] - fRec32[2]))) + (fSlow80 * (fRec33[0] - fRec33[2]))) + (fSlow82 * (fRec34[0] - fRec34[2]))) + (fSlow84 * (fRec35[0] - fRec35[2]))) + (fSlow86 * (fRec36[0] - fRec36[2]))) + (fSlow88 * (fRec37[0] - fRec37[2]))) + (fSlow90 * (fRec38[0] - fRec38[2]))) + (fSlow92 * (fRec39[0] - fRec39[2]))) + (fSlow94 * (fRec40[0] - fRec40[2]))) + (fSlow96 * (fRec41[0] - fRec41[2]))) + (fSlow98 * (fRec42[0] - fRec42[2]))) + (fSlow100 * (fRec43[0] - fRec43[2]))) + (fSlow102 * (fRec44[0] - fRec44[2]))) + (fSlow104 * (fRec45[0] - fRec45[2]))) + (fSlow106 * (fRec46[0] - fRec46[2]))) + (fSlow108 * (fRec47[0] - fRec47[2]))) + (fSlow110 * (fRec48[0] - fRec48[2]))) + (fSlow112 * (fRec49[0] - fRec49[2]))) + (fSlow114 * (fRec50[0] - fRec50[2]))) + (fSlow116 * (fRec51[0] - fRec51[2]))) + (fSlow118 * (fRec52[0] - fRec52[2]))) + (fSlow120 * (fRec53[0] - fRec53[2]))) + (fSlow122 * (fRec54[0] - fRec54[2]))) + (fSlow124 * (fRec55[0] - fRec55[2]))) + (fSlow126 * (fRec56[0] - fRec56[2]))) + (fSlow128 * (fRec57[0] - fRec57[2]))) + (fSlow130 * (fRec58[0] - fRec58[2])));
			float fTemp3 = (0.0199999996f * fVec3[1]);
			fRec59[0] = ((0.0500000007f * fRec2[1]) + (0.949999988f * fRec59[1]));
			float fTemp4 = (fTemp3 + (0.99000001f * fRec59[0]));
			fVec4[(IOTA & 2047)] = fTemp4;
			fRec3[0] = ((fSlow7 * fVec4[((IOTA - iSlow9) & 2047)]) + (fSlow10 * ((fSlow11 * fVec4[((IOTA - iSlow12) & 2047)]) + (fSlow13 * ((fSlow14 * fVec4[((IOTA - iSlow15) & 2047)]) + (fSlow4 * ((fSlow17 * fVec4[((IOTA - iSlow18) & 2047)]) + (fSlow19 * fVec4[((IOTA - iSlow20) & 2047)]))))))));
			float fRec1 = fRec3[0];
			output0[i] = FAUSTFLOAT(fRec1);
			iRec0[1] = iRec0[0];
			fRec4[1] = fRec4[0];
			IOTA = (IOTA + 1);
			fRec2[1] = fRec2[0];
			iRec8[1] = iRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fRec9[1] = fRec9[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
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
			fVec3[1] = fVec3[0];
			fRec59[1] = fRec59[0];
			fRec3[1] = fRec3[0];
			
		}
		
	}

	
};
// clang-format on
#endif

// clang-format off
#define faust_setup(name) xfaust_setup(name)
#define xfaust_setup(name) name##_tilde_setup(void)
// time for "active" toggle xfades in secs
#define XFADE_TIME 0.1f
static t_class* marimba_faust_class;
#define FAUST_EXT t_faust_marimba
#define FAUST_EXT_CLASS marimba_faust_class
// clang-format on

template <class T>
class _marimba_UI : public UI {
};
typedef _marimba_UI<marimba> marimba_UI;

struct t_faust_marimba {
    t_object x_obj;
#ifdef __MINGW32__
    /* This seems to be necessary as some as yet undetermined Pd routine seems
     to write past the end of x_obj on Windows. */
    int fence; /* dummy field (not used) */
#endif
    marimba* dsp;
    PdUI<marimba_UI>* ui;
    int active, xfade, n_xfade, rate, n_in, n_out;
    t_sample **inputs, **outputs, **buf;
    t_outlet* out;
    t_sample f;
};

static inline void zero_samples(int k, int n, t_sample** out)
{
    for (int i = 0; i < k; i++)
#ifdef __STDC_IEC_559__
        /* IEC 559 a.k.a. IEEE 754 floats can be initialized faster like this */
        memset(out[i], 0, n * sizeof(t_sample));
#else
        for (int j = 0; j < n; j++)
            out[i][j] = 0.0f;
#endif
}

static inline void copy_samples(int k, int n, t_sample** out, t_sample** in)
{
    for (int i = 0; i < k; i++)
        memcpy(out[i], in[i], n * sizeof(t_sample));
}

static t_int* faust_perform(t_int* w)
{
    t_faust_marimba* x = reinterpret_cast<t_faust_marimba*>(w[1]);
    int n = static_cast<int>(w[2]);
    if (!x->dsp || !x->buf)
        return (w + 3);

    AVOIDDENORMALS;
    if (x->xfade > 0) {
        float d = 1.0f / x->n_xfade, f = (x->xfade--) * d;
        d = d / n;
        x->dsp->compute(n, x->inputs, x->buf);
        if (x->active) {
            if (x->n_in == x->n_out) {
                /* xfade inputs -> buf */
                for (int j = 0; j < n; j++, f -= d) {
                    for (int i = 0; i < x->n_out; i++)
                        x->outputs[i][j] = f * x->inputs[i][j] + (1.0f - f) * x->buf[i][j];
                }
            } else {
                /* xfade 0 -> buf */
                for (int j = 0; j < n; j++, f -= d) {
                    for (int i = 0; i < x->n_out; i++)
                        x->outputs[i][j] = (1.0f - f) * x->buf[i][j];
                }
            }
        } else if (x->n_in == x->n_out) {
            /* xfade buf -> inputs */
            for (int j = 0; j < n; j++, f -= d) {
                for (int i = 0; i < x->n_out; i++)
                    x->outputs[i][j] = f * x->buf[i][j] + (1.0f - f) * x->inputs[i][j];
            }
        } else {
            /* xfade buf -> 0 */
            for (int j = 0; j < n; j++, f -= d) {
                for (int i = 0; i < x->n_out; i++)
                    x->outputs[i][j] = f * x->buf[i][j];
            }
        }
    } else if (x->active) {
        x->dsp->compute(n, x->inputs, x->buf);
        copy_samples(x->n_out, n, x->outputs, x->buf);
    } else if (x->n_in == x->n_out) {
        copy_samples(x->n_out, n, x->buf, x->inputs);
        copy_samples(x->n_out, n, x->outputs, x->buf);
    } else
        zero_samples(x->n_out, n, x->outputs);

    return (w + 3);
}

static void marimba_faust_dsp(t_faust_marimba* x, t_signal** sp)
{
    const int n = sp[0]->s_n;
    const int sr = static_cast<int>(sp[0]->s_sr);

    if (x->rate <= 0) {
        /* default sample rate is whatever Pd tells us */
        PdUI<marimba_UI>* ui = x->ui;
        std::vector<FAUSTFLOAT> z = ui->uiValues();
        /* set the proper sample rate; this requires reinitializing the dsp */
        x->rate = sr;
        x->dsp->init(sr);
        ui->setUIValues(z);
    }
    if (n > 0)
        x->n_xfade = static_cast<int>(x->rate * XFADE_TIME / n);

    dsp_add(faust_perform, 2, x, n);

    for (int i = 0; i < x->n_in; i++)
        x->inputs[i] = sp[i]->s_vec;

    for (int i = 0; i < x->n_out; i++)
        x->outputs[i] = sp[x->n_in + i]->s_vec;

    if (x->buf != NULL) {
        for (int i = 0; i < x->n_out; i++) {
            x->buf[i] = static_cast<t_sample*>(malloc(n * sizeof(t_sample)));
            if (x->buf[i] == NULL) {
                for (int j = 0; j < i; j++)
                    free(x->buf[j]);
                free(x->buf);
                x->buf = NULL;
                break;
            }
        }
    }
}

static void marimba_dump_to_console(t_faust_marimba* x)
{
    t_object* xobj = &x->x_obj;
    t_class* xc = xobj->te_pd;
    const char* name = class_getname(xc);

    // print xlets
    post("[%s] inlets: %i", name, x->dsp->getNumInputs());
    int info_outlet = (x->out == 0) ? 0 : 1;
    post("[%s] outlets: %i", name, x->dsp->getNumOutputs() + info_outlet);

    // print properties
    for (size_t i = 0; i < x->ui->uiCount(); i++) {
        UIElement* el = x->ui->uiAt(i);
        post("[%s] property: %s = %g", name, el->setPropertySym()->s_name, static_cast<double>(el->value()));
    }
}

static void marimba_faust_any(t_faust_marimba* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!x->dsp)
        return;

    PdUI<marimba_UI>* ui = x->ui;
    if (s == &s_bang) {
        ui->dumpUI(x->out);
    } else if (isGetAllProperties(s)) {
        ui->outputAllProperties(x->out);
    } else if (isGetProperty(s)) {
        ui->outputProperty(s, x->out);
    } else if (isSetProperty(s)) {
        ui->setProperty(s, argc, argv);
    } else {
        const char* label = s->s_name;
        int count = 0;
        for (size_t i = 0; i < ui->uiCount(); i++) {
            if (ui->uiAt(i)->pathcmp(label)) {
                if (argc == 0) {
                    ui->uiAt(i)->outputValue(x->out);
                    ++count;
                } else if (argc == 1 && (argv[0].a_type == A_FLOAT || argv[0].a_type == A_DEFFLOAT)) {
                    float f = atom_getfloat(argv);
                    UIElement* el = ui->uiAt(i);
                    el->setValue(f);
                    ++count;
                } else
                    pd_error(x, "[ceammc] %s: bad control argument: %s", ui->fullName().c_str(), label);
            }
        }

        if (count == 0 && strcmp(label, "active") == 0) {
            if (argc == 0) {
                t_atom arg;
                SETFLOAT(&arg, (float)x->active);
                if (x->out) {
                    outlet_anything(x->out, gensym("active"), 1, &arg);
                }
            } else if (argc == 1 && (argv[0].a_type == A_FLOAT || argv[0].a_type == A_DEFFLOAT)) {
                float f = atom_getfloat(argv);
                x->active = (int)f;
                x->xfade = x->n_xfade;
            }
        }
    }
}

static void faust_free_dsp(t_faust_marimba* x)
{
    delete x->dsp;
    x->dsp = NULL;
}

static void faust_free_ui(t_faust_marimba* x)
{
    delete x->ui;
    x->ui = NULL;
}

static void faust_free_inputs(t_faust_marimba* x)
{
    if (x->inputs)
        free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust_marimba* x)
{
    if (x->outputs)
        free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust_marimba* x)
{
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i])
                free(x->buf[i]);
        }

        free(x->buf);
    }
}

static void marimba_faust_free(t_faust_marimba* x)
{
    faust_free_dsp(x);
    faust_free_ui(x);
    faust_free_inputs(x);
    faust_free_outputs(x);
    faust_free_buf(x);
}

static bool faust_init_inputs(t_faust_marimba* x)
{
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if (x->inputs == NULL) {
            error("[%s] faust_init_inputs failed", sym(marimba));
            return false;
        }
    }

    // creating sound inlets (except the first one!)
    for (int i = 0; i < (x->n_in - 1); i++) {
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    return true;
}

static bool faust_init_outputs(t_faust_marimba* x, bool info_outlet)
{
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->outputs == NULL) {
            error("[%s] faust_init_outputs failed", sym(marimba));
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->buf == NULL) {
            error("[%s] faust_init_outputs failed", sym(marimba));
            faust_free_outputs(x);
            return false;
        }

        for (int i = 0; i < x->n_out; i++)
            x->buf[i] = NULL;
    }

    // creating sound outlets
    for (int i = 0; i < x->n_out; i++) {
        outlet_new(&x->x_obj, &s_signal);
    }

    // control outlet
    if (info_outlet)
        x->out = outlet_new(&x->x_obj, 0);
    else
        x->out = 0;

    return true;
}

static bool faust_new_internal(t_faust_marimba* x, const std::string& objId = "", bool info_outlet = true)
{
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new marimba();
    x->ui = new PdUI<marimba_UI>(sym(marimba), objId);

    if (!faust_init_inputs(x)) {
        marimba_faust_free(x);
        return false;
    }

    if (!faust_init_outputs(x, info_outlet)) {
        marimba_faust_free(x);
        return false;
    }

    x->dsp->init(sr);
    x->dsp->buildUserInterface(x->ui);

    return true;
}

/**
 * find nth element that satisfies given predicate
 * @param first - first element of sequence
 * @param last - pointer behind last element of sequence
 * @param Nth - searched element index
 * @param pred - predicate
 * @return pointer to found element or pointer to last, if not found
 */
template <class InputIterator, class NthOccurence, class UnaryPredicate>
InputIterator find_nth_if(InputIterator first, InputIterator last, NthOccurence Nth, UnaryPredicate pred)
{
    if (Nth > 0) {
        while (first != last) {
            if (pred(*first))
                if (!--Nth)
                    return first;
            ++first;
        }
    }
    return last;
}

/**
 * @return true if given atom is a float
 */
static bool atom_is_float(const t_atom& a)
{
    switch (a.a_type) {
    case A_FLOAT:
    case A_DEFFLOAT:
        return true;
    default:
        return false;
    }
}

/**
 * @return true if given atom is a symbol
 */
static bool atom_is_symbol(const t_atom& a)
{
    switch (a.a_type) {
    case A_DEFSYMBOL:
    case A_SYMBOL:
        return true;
    default:
        return false;
    }
}

/**
 * @return true if given atom is a property
 */
static bool atom_is_property(const t_atom& a)
{
    switch (a.a_type) {
    case A_DEFSYMBOL:
    case A_SYMBOL:
        return a.a_w.w_symbol->s_name[0] == '@';
    default:
        return false;
    }
}

/**
 * @brief find nth float in argument list. (arguments can be mixed)
 * @param argc argument count
 * @param argv pointer to argument vector
 * @param nth find position. nth should be > 0!
 * @param dest destination to write value
 * @return true if argument at given position was found, otherwise false
 */
static bool get_nth_float_arg(int argc, t_atom* argv, int nth, t_float* dest)
{
    t_atom* last = argv + argc;
    t_atom* res = find_nth_if(argv, last, nth, atom_is_float);
    if (last == res)
        return false;

    *dest = atom_getfloat(res);
    return true;
}

/**
 * @brief find nth symbol in argument list. (arguments can be mixed)
 * @param argc argument count
 * @param argv pointer to argument vector
 * @param nth find position. nth should be > 0!
 * @param dest destination to write found argument value
 * @return true if argument at given position was found, otherwise false
 */
static bool get_nth_symbol_arg(int argc, t_atom* argv, int nth, const char** dest)
{
    t_atom* last = argv + argc;
    t_atom* res = find_nth_if(argv, last, nth, atom_is_symbol);
    if (last == res)
        return false;

    t_symbol* s = atom_getsymbol(res);
    *dest = s->s_name;
    return true;
}

class PdArgParser {
    t_faust_marimba* x_;
    int argc_;
    t_atom* argv_;
    bool control_outlet_;

public:
    /**
     * @brief FaustArgParser
     * @param x pointer to faust class
     * @param argc arguments count
     * @param argv pointer to argument vector
     */
    PdArgParser(t_faust_marimba* x, int argc, t_atom* argv, bool info_outlet = true)
        : x_(x)
        , argc_(0)
        , argv_(argv)
        , control_outlet_(info_outlet)
    {
        const char* id = NULL;
        std::string objId;

        int first_prop_idx = argc;
        for (int i = 0; i < argc; i++) {
            if (atom_is_property(argv[i]))
                first_prop_idx = i;
        }

        // store argument count (without properties)
        argc_ = first_prop_idx;

        if (get_nth_symbol_arg(argc_, argv_, 1, &id))
            objId = id;

        // init error
        if (!faust_new_internal(x, objId, control_outlet_)) {
            this->x_ = NULL;
        }

        // process properties
        std::deque<ceammc::AtomList> props = ceammc::AtomList(argc, argv).properties();
        for (size_t i = 0; i < props.size(); i++) {
            ceammc::AtomList& p = props[i];
            // skip empty property
            if (p.size() < 2)
                continue;

            t_atom* data = p.toPdData() + 1;
            this->x_->ui->setProperty(p[0].asSymbol(), p.size() - 1, data);
        }
    }

    /**
     * @brief initFloatArg
     * @param name argument name
     * @param pos argument position among of @bold float(!) arguments. Position starts from @bold 1(!).
     * to select first argument - pass 1.
     */
    void initFloatArg(const char* name, int pos)
    {
        // object was not created
        if (!this->x_)
            return;

        t_float v = 0.0;
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &v)) {
            UIElement* el = this->x_->ui->findElementByLabel(name);
            if (!el) {
                post("invalid UI element name: %s", name);
                return;
            }

            el->setValue(v, true);
        }
    }

    /**
     * @brief send creation argument to first signal inlet
     * @param name argument name
     * @param pos argument position among of @bold float(!) arguments. Position starts from @bold 1(!).
     * to select first argument - pass 1.
     */
    void signalFloatArg(const char* /*name*/, int pos)
    {
        // object was not created
        if (!this->x_)
            return;

        t_float arg = 0;
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &arg))
            pd_float(reinterpret_cast<t_pd*>(this->x_), arg);
    }

    t_faust_marimba* pd_obj()
    {
        return this->x_;
    }
};

static void* marimba_faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s, bool soundIn = true)
{
    marimba_faust_class = class_new(s, reinterpret_cast<t_newmethod>(marimba_faust_new),
        reinterpret_cast<t_method>(marimba_faust_free),
        sizeof(t_faust_marimba),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);

    if (soundIn) {
        class_addmethod(marimba_faust_class, nullfn, &s_signal, A_NULL);
        CLASS_MAINSIGNALIN(marimba_faust_class, t_faust_marimba, f);
    }

    class_addmethod(marimba_faust_class, reinterpret_cast<t_method>(marimba_faust_dsp), gensym("dsp"), A_NULL);
    class_addmethod(marimba_faust_class, reinterpret_cast<t_method>(marimba_dump_to_console), gensym("dump"), A_NULL);
    class_addanything(marimba_faust_class, marimba_faust_any);
    ceammc::register_faust_external(marimba_faust_class);
}

#define EXTERNAL_NEW void* marimba_faust_new(t_symbol*, int argc, t_atom* argv)

#define EXTERNAL_SIMPLE_NEW()                                                           \
    static void* marimba_faust_new(t_symbol*, int argc, t_atom* argv)                     \
    {                                                                                   \
        t_faust_marimba* x = reinterpret_cast<t_faust_marimba*>(pd_new(marimba_faust_class)); \
        PdArgParser p(x, argc, argv, false);                                            \
        return p.pd_obj();                                                              \
    }

#define EXTERNAL_SETUP(MOD)                        \
    extern "C" void setup_##MOD##0x2emarimba_tilde() \
    {                                              \
        internal_setup(gensym(#MOD ".marimba~"));    \
    }

#define EXTERNAL_SETUP_NO_IN(MOD)                      \
    extern "C" void setup_##MOD##0x2emarimba_tilde()     \
    {                                                  \
        internal_setup(gensym(#MOD ".marimba~"), false); \
    }

#define SIMPLE_EXTERNAL(MOD) \
    EXTERNAL_SIMPLE_NEW();   \
    EXTERNAL_SETUP(MOD);

#endif
