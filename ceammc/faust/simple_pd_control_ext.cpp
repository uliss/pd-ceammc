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

#include "faust/dsp/dsp.h"
#include "faust/gui/UI.h"
#include "faust/gui/meta.h"
#include "faust/misc.h"

#include "ceammc_atomlist.h"
#include <m_pd.h>

/******************************************************************************
*******************************************************************************

                                   VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/

#ifdef FAUST_MACRO
// clang-format off
<<includeIntrinsic>>
// clang-format on
#endif

/***************************************************************************
   Pd UI interface
***************************************************************************/

#ifndef FAUST_MACRO
    struct mydsp : public dsp {
};
#endif

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

static t_symbol *s_button, *s_checkbox, *s_vslider, *s_hslider, *s_nentry, *s_vbargraph, *s_hbargraph;

class ui_elem_t {
public:
    char* label;
    t_symbol* property;
    t_symbol* get_property;
    float* zone;
    float init, min, max, step;
    ui_elem_type_t type;

    t_symbol* typeSymbol();
    void initProperty(const char* name);
    float value(float def = 0.f) const;
    void outputProperty(t_outlet* out);
};

t_symbol* ui_elem_t::typeSymbol()
{
    switch (type) {
    case UI_BUTTON:
        return s_button;
    case UI_CHECK_BUTTON:
        return s_checkbox;
    case UI_V_SLIDER:
        return s_vslider;
    case UI_H_SLIDER:
        return s_hslider;
    case UI_NUM_ENTRY:
        return s_nentry;
    case UI_V_BARGRAPH:
        return s_vbargraph;
    case UI_H_BARGRAPH:
        return s_hbargraph;
    default:
        return 0;
    }
}

void ui_elem_t::initProperty(const char* name)
{
    if (name == NULL) {
        property = gensym("?");
        get_property = gensym("?");
        return;
    }

    char buf[MAXPDSTRING];
    sprintf(buf, "@%s", name);
    property = gensym(buf);
    sprintf(buf, "@%s?", name);
    get_property = gensym(buf);
}

float ui_elem_t::value(float def) const
{
    if (!zone)
        return def;

    return *zone;
}

void ui_elem_t::outputProperty(t_outlet* out)
{
    ceammc::Atom a;

    if (zone)
        a.setFloat(*zone, true);
    else
        a.setSymbol(gensym("?"), true);

    a.outputAsAny(out, property);
}

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
    void dumpUI(t_outlet* out);
    void outputAllProperties(t_outlet* out);
    void outputProperty(t_symbol* s, t_outlet* out);
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
    elems[nelems].initProperty(label);
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
    elems[nelems].initProperty(label);
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
    elems[nelems].initProperty(label);
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
    elems[nelems].initProperty(label);
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
        if (pathcmp(elems[i].label, label) == 0)
            return &elems[i];
    }

    return NULL;
}

void PdUI::setElementValue(const char* label, float v)
{
    ui_elem_t* el = findElementByLabel(label);
    if (!el)
        return;

    if (el->min <= v && v <= el->max)
        *el->zone = v;
}

void PdUI::dumpUI(t_outlet* out)
{
    for (int i = 0; i < nelems; i++) {
        if (elems[i].label && elems[i].zone) {
            t_atom args[6];
            t_symbol* _s = elems[i].typeSymbol();
            if (!_s)
                continue;

            SETSYMBOL(&args[0], gensym(elems[i].label));
            SETFLOAT(&args[1], *elems[i].zone);
            SETFLOAT(&args[2], elems[i].init);
            SETFLOAT(&args[3], elems[i].min);
            SETFLOAT(&args[4], elems[i].max);
            SETFLOAT(&args[5], elems[i].step);

            if (out) {
                outlet_anything(out, _s, 6, args);
            }
        }
    }
}

void PdUI::outputAllProperties(t_outlet* out)
{
    ceammc::AtomList l;
    for (int i = 0; i < nelems; i++)
        l.append(elems[i].property);

    l.output(out);
}

void PdUI::outputProperty(t_symbol* s, t_outlet* out)
{
    for (int i = 0; i < nelems; i++) {
        if (elems[i].get_property == s)
            elems[i].outputProperty(out);
    }
}

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
<<includeclass>>
// clang-format on
#endif

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
    mydsp* dsp;
    PdUI* ui;
    std::string* label;
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
    t_faust* x = (t_faust*)(w[1]);
    int n = (int)(w[2]);
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

static void faust_dsp(t_faust* x, t_signal** sp)
{
    int n = sp[0]->s_n, sr = (int)sp[0]->s_sr;
    if (x->rate <= 0) {
        /* default sample rate is whatever Pd tells us */
        PdUI* ui = x->ui;
        float* z = NULL;
        if (ui->nelems > 0 && (z = (float*)malloc(ui->nelems * sizeof(float)))) {
            /* save the current control values */
            for (int i = 0; i < ui->nelems; i++) {
                if (ui->elems[i].zone)
                    z[i] = *ui->elems[i].zone;
            }
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

static bool isGetAllProperties(t_symbol* s)
{
    size_t len = strlen(s->s_name);
    if (len < 2)
        return false;

    return s->s_name[0] == '@' && s->s_name[1] == '*';
}

static bool isGetProperty(t_symbol* s)
{
    size_t len = strlen(s->s_name);
    if (len < 1)
        return false;

    if (s->s_name[0] != '@')
        return false;

    return s->s_name[len - 1] == '?';
}

static void faust_any(t_faust* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!x->dsp)
        return;

    PdUI* ui = x->ui;
    if (s == &s_bang) {
        ui->dumpUI(x->out);
    } else if (isGetAllProperties(s)) {
        ui->outputAllProperties(x->out);
    } else if (isGetProperty(s)) {
        ui->outputProperty(s, x->out);
    } else {
        const char* label = s->s_name;
        int count = 0;
        for (int i = 0; i < ui->nelems; i++) {
            if (ui->elems[i].label && pathcmp(ui->elems[i].label, label) == 0) {
                if (argc == 0) {
                    if (ui->elems[i].zone) {
                        t_atom arg;
                        SETFLOAT(&arg, *ui->elems[i].zone);
                        if (x->out) {
                            outlet_anything(x->out, gensym(ui->elems[i].label), 1, &arg);
                        }
                    }
                    ++count;
                } else if (argc == 1 && (argv[0].a_type == A_FLOAT || argv[0].a_type == A_DEFFLOAT) && ui->elems[i].zone) {
                    float f = atom_getfloat(argv);
                    ui_elem_t* el = &ui->elems[i];
                    if (el->min <= f && f <= el->max) {
                        *el->zone = f;
                    }
                    ++count;
                } else
                    pd_error(x, "[ceammc] %s: bad control argument: %s",
                        x->label->c_str(), label);
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

static void faust_free_label(t_faust* x)
{
    delete x->label;
    x->label = NULL;
}

static void faust_free_dsp(t_faust* x)
{
    delete x->dsp;
    x->dsp = NULL;
}

static void faust_free_ui(t_faust* x)
{
    delete x->ui;
    x->ui = NULL;
}

static void faust_free_inputs(t_faust* x)
{
    if (x->inputs)
        free(x->inputs);
    x->inputs = NULL;
}

static void faust_free_outputs(t_faust* x)
{
    if (x->outputs)
        free(x->outputs);
    x->outputs = NULL;
}

static void faust_free_buf(t_faust* x)
{
    if (x->buf) {
        for (int i = 0; i < x->n_out; i++) {
            if (x->buf[i])
                free(x->buf[i]);
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

static bool faust_init_inputs(t_faust* x)
{
    x->inputs = NULL;
    x->n_in = x->dsp->getNumInputs();

    if (x->n_in > 0) {
        x->inputs = static_cast<t_sample**>(calloc(x->n_in, sizeof(t_sample*)));

        if (x->inputs == NULL) {
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

static bool faust_init_outputs(t_faust* x, bool info_outlet)
{
    x->outputs = NULL;
    x->buf = NULL;

    x->n_out = x->dsp->getNumOutputs();

    if (x->n_out > 0) {
        x->outputs = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->outputs == NULL) {
            error("[ceammc] faust_init_outputs failed");
            return false;
        }

        x->buf = static_cast<t_sample**>(calloc(x->n_out, sizeof(t_sample*)));
        if (x->buf == NULL) {
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
    if (info_outlet)
        x->out = outlet_new(&x->x_obj, 0);
    else
        x->out = 0;

    return true;
}

static void faust_init_label(t_faust* x, const char* obj_id)
{
    x->label = new std::string(sym(mydsp) "~");

    // label settings
    if (obj_id) {
        *x->label += " ";
        *x->label += obj_id;
    }
}

static bool faust_new_internal(t_faust* x, const char* obj_id = NULL, bool info_outlet = true)
{
    int sr = 44100;
    x->active = 1;
    x->xfade = 0;
    x->rate = sr;
    x->n_xfade = static_cast<int>(sr * XFADE_TIME / 64);

    x->dsp = new mydsp();
    x->ui = new PdUI(sym(mydsp), obj_id);

    faust_init_label(x, obj_id);

    if (!faust_init_inputs(x)) {
        faust_free(x);
        return false;
    }

    if (!faust_init_outputs(x, info_outlet)) {
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
        if (get_nth_float_arg(this->argc_, this->argv_, pos, &arg))
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
