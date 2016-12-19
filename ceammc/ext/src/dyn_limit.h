//----------------------------------------------------------
//
// Code generated with Faust 0.9.92 (http://faust.grame.fr)
//----------------------------------------------------------

/* link with  */
#include <math.h>
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
#include <stdlib.h>
#include <string>

/************************************************************************
    IMPORTANT NOTE : this file contains two clearly delimited sections :
    the ARCHITECTURE section (in two parts) and the USER section. Each section
    is governed by its own copyright and license. Please check individually
    each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
    Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
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

 ************************************************************************
 ************************************************************************/
 
/******************************************************************************
*******************************************************************************

								FAUST DSP

*******************************************************************************
*******************************************************************************/

#ifndef __dsp__
#define __dsp__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;
struct Meta;

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
         * Trigger the UI* parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the UI* user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /** Global init, calls the following methods :
         * - static class 'classInit' : static table initialisation
         * - 'instanceInit' : constants and instance table initialisation
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void init(int samplingRate) = 0;
    
        /** Init instance state
         *
         * @param samplingRate - the sampling rate in Herz
         */
        virtual void instanceInit(int samplingRate) = 0;
    
        /** Init instance constant state
         *
         * @param samplingRate - the sampling rate in Herz
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
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value metadata).
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with sucessive in/out audio buffers.
         *
         * @param count - the nomber of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation : alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the nomber of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, doucbe or quad)
         *
         */
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
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
        virtual void metadata(Meta* m) { return fDSP->metadata(m); }
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
       
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
    Copyright (C) 2003-2016 GRAME, Centre National de Creation Musicale
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


 ************************************************************************
 ************************************************************************/
 
#ifndef FAUST_UI_H
#define FAUST_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust User Interface
 * This abstract class contains only the method that the faust compiler can
 * generate to describe a DSP interface.
 ******************************************************************************/

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

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

//----------------------------------------------------------------
//  Generic decorator
//----------------------------------------------------------------

class DecoratorUI : public UI
{
    protected:
    
        UI* fUI;

    public:
    
        DecoratorUI(UI* ui = 0):fUI(ui)
        {}

        virtual ~DecoratorUI() { delete fUI; }

        // -- widget's layouts
        virtual void openTabBox(const char* label)          { fUI->openTabBox(label); }
        virtual void openHorizontalBox(const char* label)   { fUI->openHorizontalBox(label); }
        virtual void openVerticalBox(const char* label)     { fUI->openVerticalBox(label); }
        virtual void closeBox()                             { fUI->closeBox(); }

        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone)         { fUI->addButton(label, zone); }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)    { fUI->addCheckButton(label, zone); }
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
            { fUI->addVerticalSlider(label, zone, init, min, max, step); }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) 	
            { fUI->addHorizontalSlider(label, zone, init, min, max, step); }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) 			
            { fUI->addNumEntry(label, zone, init, min, max, step); }

        // -- passive widgets	
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) 
            { fUI->addHorizontalBargraph(label, zone, min, max); }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
            { fUI->addVerticalBargraph(label, zone, min, max); }

        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) { fUI->declare(zone, key, val); }

};

#endif
/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
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

 ************************************************************************
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
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
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

 ************************************************************************
 ************************************************************************/
 
#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <string.h>
#include <stdlib.h>


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
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
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


/******************************************************************************
*******************************************************************************

                                   VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/


/***************************************************************************
   Pd UI interface
 ***************************************************************************/

enum ui_elem_type_t {
    UI_BUTTON,
    UI_CHECK_BUTTON,
    UI_V_SLIDER,
    UI_H_SLIDER,
    UI_NUM_ENTRY,
    UI_V_BARGRAPH,
    UI_H_BARGRAPH,
    UI_END_GROUP,
    UI_V_GROUP,
    UI_H_GROUP,
    UI_T_GROUP
};

struct ui_elem_t {
    ui_elem_type_t type;
    char* label;
    float* zone;
    float init, min, max, step;
};

class PdUI : public UI {
public:
    const char* name;
    int nelems, level;
    ui_elem_t* elems;

    PdUI();
    PdUI(const char* nm, const char* s);
    virtual ~PdUI();

protected:
    std::string path;
    void add_elem(ui_elem_type_t type, const char* label = NULL);
    void add_elem(ui_elem_type_t type, const char* label, float* zone);
    void add_elem(ui_elem_type_t type, const char* label, float* zone,
        float init, float min, float max, float step);
    void add_elem(ui_elem_type_t type, const char* label, float* zone,
        float min, float max);

public:
    virtual void addButton(const char* label, float* zone);
    virtual void addCheckButton(const char* label, float* zone);
    virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step);
    virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step);
    virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step);

    virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max);
    virtual void addVerticalBargraph(const char* label, float* zone, float min, float max);

    virtual void openTabBox(const char* label);
    virtual void openHorizontalBox(const char* label);
    virtual void openVerticalBox(const char* label);
    virtual void closeBox();

    virtual void run();
public:
    ui_elem_t* findElementByLabel(const char* label);
    void setElementValue(const char* label, float v);
};

static std::string mangle(const char* name, int level, const char* s)
{
    const char* s0 = s;
    std::string t = "";
    if (!s)
        return t;
    // Get rid of bogus "0x00" labels in recent Faust revisions. Also, for
    // backward compatibility with old Faust versions, make sure that default
    // toplevel groups and explicit toplevel groups with an empty label are
    // treated alike (these both return "0x00" labels in the latest Faust, but
    // would be treated inconsistently in earlier versions).
    if (!*s || strcmp(s, "0x00") == 0) {
        if (level == 0)
            // toplevel group with empty label, map to dsp name
            s = name;
        else
            // empty label
            s = "";
    }
    while (*s)
        if (isalnum(*s))
            t += *(s++);
        else {
            const char* s1 = s;
            while (*s && !isalnum(*s))
                ++s;
            if (s1 != s0 && *s)
                t += "-";
        }
    return t;
}

static std::string normpath(std::string path)
{
    path = std::string("/") + path;
    int pos = path.find("//");
    while (pos >= 0) {
        path.erase(pos, 1);
        pos = path.find("//");
    }
    size_t len = path.length();
    if (len > 1 && path[len - 1] == '/')
        path.erase(len - 1, 1);
    return path;
}

static std::string pathcat(std::string path, std::string label)
{
    if (path.empty())
        return normpath(label);
    else if (label.empty())
        return normpath(path);
    else
        return normpath(path + "/" + label);
}

PdUI::PdUI()
{
    nelems = level = 0;
    elems = NULL;
    name = "";
    path = "";
}

PdUI::PdUI(const char* nm, const char* s)
{
    nelems = level = 0;
    elems = NULL;
    name = nm ? nm : "";
    path = s ? s : "";
}

PdUI::~PdUI()
{
    if (elems) {
        for (int i = 0; i < nelems; i++)
            if (elems[i].label)
                free(elems[i].label);
        free(elems);
    }
}

inline void PdUI::add_elem(ui_elem_type_t type, const char* label)
{
    ui_elem_t* elems1 = (ui_elem_t*)realloc(elems, (nelems + 1) * sizeof(ui_elem_t));
    if (elems1)
        elems = elems1;
    else
        return;
    std::string s = pathcat(path, mangle(name, level, label));
    elems[nelems].type = type;
    elems[nelems].label = strdup(s.c_str());
    elems[nelems].zone = NULL;
    elems[nelems].init = 0.0;
    elems[nelems].min = 0.0;
    elems[nelems].max = 0.0;
    elems[nelems].step = 0.0;
    nelems++;
}

inline void PdUI::add_elem(ui_elem_type_t type, const char* label, float* zone)
{
    ui_elem_t* elems1 = (ui_elem_t*)realloc(elems, (nelems + 1) * sizeof(ui_elem_t));
    if (elems1)
        elems = elems1;
    else
        return;
    std::string s = pathcat(path, mangle(name, level, label));
    elems[nelems].type = type;
    elems[nelems].label = strdup(s.c_str());
    elems[nelems].zone = zone;
    elems[nelems].init = 0.0;
    elems[nelems].min = 0.0;
    elems[nelems].max = 1.0;
    elems[nelems].step = 1.0;
    nelems++;
}

inline void PdUI::add_elem(ui_elem_type_t type, const char* label, float* zone,
    float init, float min, float max, float step)
{
    ui_elem_t* elems1 = (ui_elem_t*)realloc(elems, (nelems + 1) * sizeof(ui_elem_t));
    if (elems1)
        elems = elems1;
    else
        return;
    std::string s = pathcat(path, mangle(name, level, label));
    elems[nelems].type = type;
    elems[nelems].label = strdup(s.c_str());
    elems[nelems].zone = zone;
    elems[nelems].init = init;
    elems[nelems].min = min;
    elems[nelems].max = max;
    elems[nelems].step = step;
    nelems++;
}

inline void PdUI::add_elem(ui_elem_type_t type, const char* label, float* zone,
    float min, float max)
{
    ui_elem_t* elems1 = (ui_elem_t*)realloc(elems, (nelems + 1) * sizeof(ui_elem_t));
    if (elems1)
        elems = elems1;
    else
        return;
    std::string s = pathcat(path, mangle(name, level, label));
    elems[nelems].type = type;
    elems[nelems].label = strdup(s.c_str());
    elems[nelems].zone = zone;
    elems[nelems].init = 0.0;
    elems[nelems].min = min;
    elems[nelems].max = max;
    elems[nelems].step = 0.0;
    nelems++;
}

void PdUI::addButton(const char* label, float* zone)
{
    add_elem(UI_BUTTON, label, zone);
}
void PdUI::addCheckButton(const char* label, float* zone)
{
    add_elem(UI_CHECK_BUTTON, label, zone);
}
void PdUI::addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step)
{
    add_elem(UI_V_SLIDER, label, zone, init, min, max, step);
}
void PdUI::addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step)
{
    add_elem(UI_H_SLIDER, label, zone, init, min, max, step);
}
void PdUI::addNumEntry(const char* label, float* zone, float init, float min, float max, float step)
{
    add_elem(UI_NUM_ENTRY, label, zone, init, min, max, step);
}

void PdUI::addHorizontalBargraph(const char* label, float* zone, float min, float max)
{
    add_elem(UI_H_BARGRAPH, label, zone, min, max);
}
void PdUI::addVerticalBargraph(const char* label, float* zone, float min, float max)
{
    add_elem(UI_V_BARGRAPH, label, zone, min, max);
}

void PdUI::openTabBox(const char* label)
{
    if (!path.empty())
        path += "/";
    path += mangle(name, level, label);
    level++;
}
void PdUI::openHorizontalBox(const char* label)
{
    if (!path.empty())
        path += "/";
    path += mangle(name, level, label);
    level++;
}
void PdUI::openVerticalBox(const char* label)
{
    if (!path.empty())
        path += "/";
    path += mangle(name, level, label);
    level++;
}
void PdUI::closeBox()
{
    int pos = path.rfind("/");
    if (pos < 0)
        pos = 0;
    path.erase(pos);
    level--;
}

void PdUI::run() {}

static int pathcmp(const char* s, const char* t);
ui_elem_t* PdUI::findElementByLabel(const char* label)
{
    if (!elems)
        return NULL;

    for (int i = 0; i < nelems; i++) {
        if(pathcmp(elems[i].label, label) == 0)
            return &elems[i];
    }

    return NULL;
}

void PdUI::setElementValue(const char* label, float v)
{
    ui_elem_t* el = findElementByLabel(label);
    if(!el) return;

    if(el->min <= v && v <= el->max)
        *el->zone = v;
}

/******************************************************************************
*******************************************************************************

                FAUST DSP

*******************************************************************************
*******************************************************************************/

//----------------------------------------------------------------------------
//  FAUST generated signal processor
//----------------------------------------------------------------------------

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  


#ifndef FAUSTCLASS 
#define FAUSTCLASS limit
#endif

class limit : public dsp {
  private:
	float 	fConst0;
	float 	fConst1;
	float 	fConst2;
	float 	fConst3;
	float 	fConst4;
	float 	fRec2[2];
	float 	fRec1[2];
	float 	fRec0[2];
	int fSamplingFreq;

  public:
	virtual void metadata(Meta* m) { 
		m->declare("compressor.lib/name", "Faust Compressor Effect Library");
		m->declare("compressor.lib/version", "0.0");
		m->declare("basic.lib/name", "Faust Basic Element Library");
		m->declare("basic.lib/version", "0.0");
		m->declare("math.lib/name", "Faust Math Library");
		m->declare("math.lib/version", "2.0");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/license", "LGPL with exception");
		m->declare("analyzer.lib/name", "Faust Analyzer Library");
		m->declare("analyzer.lib/version", "0.0");
		m->declare("signal.lib/name", "Faust Signal Routing Library");
		m->declare("signal.lib/version", "0.0");
	}

	virtual int getNumInputs() { return 1; }
	virtual int getNumOutputs() { return 1; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = min(1.92e+05f, max(1.0f, (float)fSamplingFreq));
		fConst1 = expf((0 - (2.5e+03f / fConst0)));
		fConst2 = (1.0f - fConst1);
		fConst3 = expf((0 - (2.0f / fConst0)));
		fConst4 = expf((0 - (1.25e+03f / fConst0)));
	}
	virtual void instanceResetUserInterface() {
	}
	virtual void instanceClear() {
		for (int i=0; i<2; i++) fRec2[i] = 0;
		for (int i=0; i<2; i++) fRec1[i] = 0;
		for (int i=0; i<2; i++) fRec0[i] = 0;
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
	virtual limit* clone() {
		return new limit();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
	}
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("0x00");
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* output0 = output[0];
		for (int i=0; i<count; i++) {
			float fTemp0 = (float)input0[i];
			float fTemp1 = fabsf(fTemp0);
			float fTemp2 = ((int((fRec1[1] > fTemp1)))?fConst3:fConst4);
			fRec2[0] = ((fTemp2 * fRec2[1]) + (fTemp1 * (1.0f - fTemp2)));
			fRec1[0] = fRec2[0];
			fRec0[0] = ((fConst1 * fRec0[1]) + (fConst2 * (0 - (0.75f * max(((20 * log10f(fRec1[0])) + 6), 0.0f)))));
			output0[i] = (FAUSTFLOAT)(fTemp0 * powf(10,(0.05f * fRec0[0])));
			// post processing
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
		}
	}
};



#include "m_pd.h"
#include <stdio.h>
#include <string>

#define sym(name) xsym(name)
#define xsym(name) #name
#define faust_setup(name) xfaust_setup(name)
#define xfaust_setup(name) name##_tilde_setup(void)

// time for "active" toggle xfades in secs
#define XFADE_TIME 0.1f

static t_class* faust_class;

struct t_faust {
    t_object x_obj;
#ifdef __MINGW32__
    /* This seems to be necessary as some as yet undetermined Pd routine seems
     to write past the end of x_obj on Windows. */
    int fence; /* dummy field (not used) */
#endif
    limit* dsp;
    PdUI* ui;
    std::string* label;
    int active, xfade, n_xfade, rate, n_in, n_out;
    t_sample **inputs, **outputs, **buf;
    t_outlet* out;
    t_sample f;
};

static t_symbol *s_button, *s_checkbox, *s_vslider, *s_hslider, *s_nentry,
    *s_vbargraph, *s_hbargraph;

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
    t_faust* x = (t_faust*)(w[1]);
    int n = (int)(w[2]);
    if (!x->dsp || !x->buf)
        return (w + 3);
    AVOIDDENORMALS;
    if (x->xfade > 0) {
        float d = 1.0f / x->n_xfade, f = (x->xfade--) * d;
        d = d / n;
        x->dsp->compute(n, x->inputs, x->buf);
        if (x->active)
            if (x->n_in == x->n_out)
                /* xfade inputs -> buf */
                for (int j = 0; j < n; j++, f -= d)
                    for (int i = 0; i < x->n_out; i++)
                        x->outputs[i][j] = f * x->inputs[i][j] + (1.0f - f) * x->buf[i][j];
            else
                /* xfade 0 -> buf */
                for (int j = 0; j < n; j++, f -= d)
                    for (int i = 0; i < x->n_out; i++)
                        x->outputs[i][j] = (1.0f - f) * x->buf[i][j];
        else if (x->n_in == x->n_out)
            /* xfade buf -> inputs */
            for (int j = 0; j < n; j++, f -= d)
                for (int i = 0; i < x->n_out; i++)
                    x->outputs[i][j] = f * x->buf[i][j] + (1.0f - f) * x->inputs[i][j];
        else
            /* xfade buf -> 0 */
            for (int j = 0; j < n; j++, f -= d)
                for (int i = 0; i < x->n_out; i++)
                    x->outputs[i][j] = f * x->buf[i][j];
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

static void faust_dsp(t_faust* x, t_signal** sp)
{
    int n = sp[0]->s_n, sr = (int)sp[0]->s_sr;
    if (x->rate <= 0) {
        /* default sample rate is whatever Pd tells us */
        PdUI* ui = x->ui;
        float* z = NULL;
        if (ui->nelems > 0 && (z = (float*)malloc(ui->nelems * sizeof(float)))) {
            /* save the current control values */
            for (int i = 0; i < ui->nelems; i++)
                if (ui->elems[i].zone)
                    z[i] = *ui->elems[i].zone;
        }
        /* set the proper sample rate; this requires reinitializing the dsp */
        x->rate = sr;
        x->dsp->init(sr);
        if (z) {
            /* restore previous control values */
            for (int i = 0; i < ui->nelems; i++)
                if (ui->elems[i].zone)
                    *ui->elems[i].zone = z[i];
            free(z);
        }
    }
    if (n > 0)
        x->n_xfade = static_cast<int>(x->rate * XFADE_TIME / n);

    dsp_add(faust_perform, 2, x, n);

    for (int i = 0; i < x->n_in; i++)
        x->inputs[i] = sp[i]->s_vec;

    for (int i = 0; i < x->n_out; i++)
        x->outputs[i] = sp[x->n_in + i]->s_vec;

    if (x->buf != NULL)
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

static int pathcmp(const char* s, const char* t)
{
    size_t n = strlen(s), m = strlen(t);
    if (n == 0 || m == 0)
        return 0;
    else if (t[0] == '/')
        return strcmp(s, t);
    else if (n <= m || s[n - m - 1] != '/')
        return strcmp(s + 1, t);
    else
        return strcmp(s + n - m, t);
}


static void faust_any(t_faust* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!x->dsp)
        return;
    PdUI* ui = x->ui;
    if (s == &s_bang) {
        for (int i = 0; i < ui->nelems; i++)
            if (ui->elems[i].label && ui->elems[i].zone) {
                t_atom args[6];
                t_symbol* _s;
                switch (ui->elems[i].type) {
                case UI_BUTTON:
                    _s = s_button;
                    break;
                case UI_CHECK_BUTTON:
                    _s = s_checkbox;
                    break;
                case UI_V_SLIDER:
                    _s = s_vslider;
                    break;
                case UI_H_SLIDER:
                    _s = s_hslider;
                    break;
                case UI_NUM_ENTRY:
                    _s = s_nentry;
                    break;
                case UI_V_BARGRAPH:
                    _s = s_vbargraph;
                    break;
                case UI_H_BARGRAPH:
                    _s = s_hbargraph;
                    break;
                default:
                    continue;
                }
                SETSYMBOL(&args[0], gensym(ui->elems[i].label));
                SETFLOAT(&args[1], *ui->elems[i].zone);
                SETFLOAT(&args[2], ui->elems[i].init);
                SETFLOAT(&args[3], ui->elems[i].min);
                SETFLOAT(&args[4], ui->elems[i].max);
                SETFLOAT(&args[5], ui->elems[i].step);
                if(x->out) {
                    outlet_anything(x->out, _s, 6, args);
                }
            }
    } else {
        const char* label = s->s_name;
        int count = 0;
        for (int i = 0; i < ui->nelems; i++)
            if (ui->elems[i].label && pathcmp(ui->elems[i].label, label) == 0) {
                if (argc == 0) {
                    if (ui->elems[i].zone) {
                        t_atom arg;
                        SETFLOAT(&arg, *ui->elems[i].zone);
                        if(x->out) {
                            outlet_anything(x->out, gensym(ui->elems[i].label), 1, &arg);
                        }
                    }
                    ++count;
                } else if (argc == 1 && (argv[0].a_type == A_FLOAT || argv[0].a_type == A_DEFFLOAT) && ui->elems[i].zone) {
                    float f = atom_getfloat(argv);
                    ui_elem_t* el = &ui->elems[i];
                    if(el->min <= f && f <= el->max) {
                        *el->zone = f;
                    }
                    ++count;
                } else
                    pd_error(x, "[ceammc] %s: bad control argument: %s",
                        x->label->c_str(), label);
            }
        if (count == 0 && strcmp(label, "active") == 0) {
            if (argc == 0) {
                t_atom arg;
                SETFLOAT(&arg, (float)x->active);
                if(x->out) {
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

static void faust_free_label(t_faust* x) { delete x->label; x->label = NULL; }
static void faust_free_dsp(t_faust* x) { delete x->dsp; x->dsp = NULL; }
static void faust_free_ui(t_faust* x) { delete x->ui; x->ui = NULL; }

static void faust_free_inputs(t_faust* x) {
    if(x->inputs) free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust* x) {
    if(x->outputs) free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust* x) {
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i]) free(x->buf[i]);
        }

        free(x->buf);
    }
}


static void faust_free(t_faust* x)
{
    faust_free_label(x);
    faust_free_dsp(x);
    faust_free_ui(x);
    faust_free_inputs(x);
    faust_free_outputs(x);
    faust_free_buf(x);
}

static bool faust_init_inputs(t_faust* x) {
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if(x->inputs == NULL) {
            error("[ceammc] faust_init_inputs failed");
            return false;
        }
    }

    // creating sound inlets (except the first one!)
    for (int i = 0; i < (x->n_in - 1); i++) {
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    return true;
}

static bool faust_init_outputs(t_faust* x, bool info_outlet) {
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if(x->outputs == NULL) {
            error("[ceammc] faust_init_outputs failed");
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if(x->buf == NULL) {
            error("[ceammc] faust_init_outputs failed");
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
    if(info_outlet)
        x->out = outlet_new(&x->x_obj, 0);
    else
        x->out = 0;

    return true;
}

static void faust_init_label(t_faust* x, const char* obj_id) {
    x->label = new std::string(sym(limit) "~");

    // label settings
    if (obj_id) {
        *x->label += " ";
        *x->label += obj_id;
    }
}

static bool faust_new_internal(t_faust* x, const char* obj_id = NULL, bool info_outlet = true) {
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new limit();
    x->ui = new PdUI(sym(limit), obj_id);

    faust_init_label(x, obj_id);

    if(!faust_init_inputs(x)) {
        faust_free(x);
        return false;
    }

    if(!faust_init_outputs(x, info_outlet)) {
        faust_free(x);
        return false;
    }

    x->dsp->init(sr);
    x->dsp->buildUserInterface(x->ui);

    return true;
}

/**
 * find nth element that satisfies given predicate
 * @first - first element of sequence
 * @last - pointer behind last element of sequence
 * @Nth - searched element index
 * @pred - predicate
 * @return pointer to found element or pointer to @bold last, if not found
 */
template<class InputIterator, class NthOccurence, class UnaryPredicate>
InputIterator find_nth_if(InputIterator first, InputIterator last, NthOccurence Nth, UnaryPredicate pred)
{
    if (Nth > 0)
        while (first != last) {
            if (pred(*first))
                if (!--Nth)
                    return first;
            ++first;
        }
    return last;
}

/**
 * @return true if given atom is a float
 */
static bool atom_is_float(const t_atom& a) {
    switch(a.a_type) {
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
static bool atom_is_symbol(const t_atom& a) {
    switch(a.a_type) {
        case A_DEFSYMBOL:
        case A_SYMBOL:
            return true;
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
static bool get_nth_float_arg(int argc, t_atom* argv, int nth, t_float* dest) {
    t_atom* last = argv + argc;
    t_atom* res = find_nth_if(argv, last, nth, atom_is_float);
    if(last == res) return false;

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
static bool get_nth_symbol_arg(int argc, t_atom* argv, int nth, const char** dest) {
    t_atom* last = argv + argc;
    t_atom* res = find_nth_if(argv, last, nth, atom_is_symbol);
    if(last == res) return false;

    t_symbol* s = atom_getsymbol(res);
    *dest = s->s_name;
    return true;
}

class PdArgParser {
    t_faust* x_;
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
    PdArgParser(t_faust* x, int argc, t_atom* argv, bool info_outlet = true)
        : x_(x)
        , argc_(argc)
        , argv_(argv)
        , control_outlet_(info_outlet)
    {
        const char* id = NULL;
        get_nth_symbol_arg(argc_, argv_, 1, &id);

        // init error
        if (!faust_new_internal(x, id, control_outlet_)) {
            this->x_ = NULL;
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
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &v))
            this->x_->ui->setElementValue(name, v);
    }

    /**
     * @brief send creation argument to first signal inlet
     * @param name argument name
     * @param pos argument position among of @bold float(!) arguments. Position starts from @bold 1(!).
     * to select first argument - pass 1.
     */
    void signalFloatArg(const char* name, int pos)
    {
        // object was not created
        if (!this->x_)
            return;

        t_float arg = 0;
        if(get_nth_float_arg(this->argc_, this->argv_, pos, &arg))
            pd_float(reinterpret_cast<t_pd*>(this->x_), arg);
    }

    t_faust* pd_obj()
    {
        return this->x_;
    }
};


static void* faust_new(t_symbol* s, int argc, t_atom* argv);

static void internal_setup(t_symbol* s)
{
    faust_class = class_new(s, reinterpret_cast<t_newmethod>(faust_new),
        reinterpret_cast<t_method>(faust_free),
        sizeof(t_faust),
        CLASS_DEFAULT,
        A_GIMME, A_NULL);
    class_addmethod(faust_class, nullfn, &s_signal, A_NULL);
    class_addmethod(faust_class, reinterpret_cast<t_method>(faust_dsp), gensym("dsp"), A_NULL);
    CLASS_MAINSIGNALIN(faust_class, t_faust, f);
    class_addanything(faust_class, faust_any);

    s_button = gensym("button");
    s_checkbox = gensym("checkbox");
    s_vslider = gensym("vslider");
    s_hslider = gensym("hslider");
    s_nentry = gensym("nentry");
    s_vbargraph = gensym("vbargraph");
    s_hbargraph = gensym("hbargraph");
}


