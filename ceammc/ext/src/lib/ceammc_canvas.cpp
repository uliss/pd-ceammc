/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "ceammc_canvas.h"
#include "ceammc_object.h"

#include "m_pd.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
#include "s_stuff.h"
void pd_init();
}

// Note! keep in sync with PD declaration
struct _canvasenvironment {
    t_symbol* ce_dir; /* directory patch lives in */
    int ce_argc; /* number of "$" arguments */
    t_atom* ce_argv; /* array of "$" arguments */
    int ce_dollarzero; /* value of "$0" */
    t_namelist* ce_path; /* search path */
};

using namespace ceammc;

AtomList ceammc::canvas_info_paths(const t_canvas* c)
{
    AtomList res;

    if (!c || !c->gl_env)
        return res;

    t_canvasenvironment* e = canvas_getenv(const_cast<t_canvas*>(c));

    t_namelist* path_list = e->ce_path;
    while (path_list) {
        res.append(Atom(gensym(path_list->nl_string)));
        path_list = path_list->nl_next;
    }

    return res;
}

t_symbol* ceammc::canvas_info_name(const t_canvas* c)
{
    return c ? c->gl_name : &s_;
}

t_symbol* ceammc::canvas_info_dir(const t_canvas* c)
{
    if (!c || !c->gl_env)
        return &s_;

    return canvas_getdir(const_cast<t_canvas*>(c));
}

int ceammc::canvas_info_font(const t_canvas* c)
{
    return c ? c->gl_font : 0;
}

bool ceammc::canvas_info_is_root(const t_canvas* c)
{
    return c ? (c->gl_owner == 0) : false;
}

bool ceammc::canvas_info_is_abstraction(const t_canvas* c)
{
    return c ? canvas_isabstraction(const_cast<t_canvas*>(c)) : false;
}

t_rect ceammc::canvas_info_rect(const t_canvas* c)
{
    if (!c)
        return t_rect(0, 0, 0, 0);

    if (canvas_info_is_root(c)) {
        return t_rect(c->gl_screenx1,
            c->gl_screeny1,
            c->gl_screenx2 - c->gl_screenx1,
            c->gl_screeny2 - c->gl_screeny1);
    } else {
        return t_rect(c->gl_xmargin,
            c->gl_ymargin,
            c->gl_pixwidth,
            c->gl_pixheight);
    }
}

Canvas::Canvas(t_canvas* c)
    : canvas_(c)
{
}

ArrayPtr Canvas::createArray(const char* name, size_t n)
{
    ArrayPtr ptr;
    if (!canvas_)
        return ptr;

    t_symbol* sname = gensym(name);
    t_garray* a = graph_array(canvas_, sname, &s_float, n, 0);
    if (!a)
        return ptr;

    ptr.reset(new Array(name));
    return ptr;
}

bool Canvas::connect(t_object* src, size_t nout, t_object* dest, size_t ninl)
{
    if (!src || !dest)
        return false;

    t_outconnect* c = obj_connect(src, nout, dest, ninl);
    return c != 0;
}

bool Canvas::connect(const BaseObject& src, size_t nout, BaseObject& dest, size_t ninl)
{
    return connect(src.owner(), nout, dest.owner(), ninl);
}

_glist* Canvas::owner()
{
    return canvas_->gl_owner;
}

t_symbol* Canvas::name()
{
    return canvas_->gl_name;
}

void Canvas::setName(const char* str)
{
    canvas_->gl_name = gensym(str);
}

std::string Canvas::parentName() const
{
    return canvas_->gl_owner ? canvas_->gl_owner->gl_name->s_name : "";
}

_glist* Canvas::current()
{
    return canvas_getcurrent();
}

void Canvas::setCurrent(_glist* c)
{
    canvas_setcurrent(c);
}

AtomList ceammc::canvas_info_args(const _glist* c)
{
    AtomList res;
    if (!c)
        return res;

    t_binbuf* b = c->gl_obj.te_binbuf;
    if (b) {
        int argc = binbuf_getnatom(b);
        t_atom* argv = binbuf_getvec(b);

        for (int i = 1; i < argc; i++)
            res.append(Atom(argv[i]));
    }

    return res;
}
