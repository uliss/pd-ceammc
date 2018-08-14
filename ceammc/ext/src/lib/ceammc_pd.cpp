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
#include "ceammc_pd.h"
#include "ceammc_atomlist.h"
#include "ceammc_platform.h"

#include "m_pd.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
void pd_init();
}

#include <exception>
#include <iostream>

typedef t_object* (*t_newempty)();
typedef t_object* (*t_newfloat)(t_float);
typedef t_object* (*t_newsymbol)(t_symbol*);
typedef t_object* (*t_newgimme)(t_symbol* s, int argc, t_atom* argv);

using namespace ceammc;

pd::External::External(const char* name, const AtomList& lst)
    : obj_(0)
{
    t_symbol* OBJ_NAME = gensym(name);

    //    pd_typedmess(&pd_objectmaker, OBJ_NAME, lst.size(), lst.toPdData());

    t_methodentry* m = pd_objectmaker->c_methods;
    try {
        for (int i = 0; i < pd_objectmaker->c_nmethod; i++) {
            if (m[i].me_name == OBJ_NAME) {
                if (m[i].me_arg[0] == A_GIMME) {
                    t_newgimme new_fn = (t_newgimme)m[i].me_fun;
                    t_atom* al = lst.toPdData();
                    obj_ = (*new_fn)(OBJ_NAME, lst.size(), al);
                    break;
                }

                if (lst.size() > 5) {
                    break;
                }

                if (m[i].me_arg[0] == A_NULL) {
                    t_newempty new_fn = (t_newempty)m[i].me_fun;
                    obj_ = (*new_fn)();
                    break;
                }

                if (m[i].me_arg[0] == A_DEFFLOAT) {
                    t_newfloat new_fn = (t_newfloat)m[i].me_fun;
                    t_float f = lst.empty() ? 0 : lst[0].asFloat(0);
                    obj_ = (*new_fn)(f);
                    break;
                }

                if (m[i].me_arg[0] == A_DEFSYMBOL) {
                    t_newsymbol new_fn = (t_newsymbol)m[i].me_fun;
                    t_symbol* s = lst.empty() ? 0 : lst[0].asSymbol();
                    obj_ = (*new_fn)(s);
                    break;
                }

                printf("unknown construct method: %d...\n", m[i].me_arg[0]);
            }
        }
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        obj_ = 0;
    }
}

pd::External::~External()
{
    if (obj_) {
        pd_free(&obj_->te_g.g_pd);
    }
}

bool pd::External::isNull() const
{
    return obj_ == 0;
}

bool pd::External::connectTo(int outn, t_object* dest, int inln)
{
    if (!obj_) {
        printf("[connectTo] NULL object\n");
        return false;
    }

#define OBJ_NAME(obj) obj->te_g.g_pd->c_name->s_name

    if (obj_ == dest) {
        printf("[%s: connectTo] self-connect\n", OBJ_NAME(obj_));
        return false;
    }

    if (!dest) {
        printf("[%s: connectTo] NULL destination\n", OBJ_NAME(obj_));
        return false;
    }

    if (inln >= obj_ninlets(dest)) {
        printf("[%s: connectTo %s] invalid destination inlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(dest), inln);
        return false;
    }

    if (outn >= numOutlets()) {
        printf("[%s: connectTo %s] invalid source outlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(dest), outn);
        return false;
    }

#undef OBJ_NAME

    return obj_connect(obj_, outn, dest, inln) != 0;
}

bool pd::External::connectTo(int outn, pd::External& ext, int inln)
{
    return connectTo(outn, ext.object(), inln);
}

bool pd::External::connectFrom(int outn, t_object* src, int inln)
{
    if (!obj_) {
        printf("[connectFrom] NULL object\n");
        return false;
    }

#define OBJ_NAME(obj) obj->te_g.g_pd->c_name->s_name

    if (obj_ == src) {
        printf("[%s: connectFrom] self-connect\n", OBJ_NAME(obj_));
        return false;
    }

    if (!src) {
        printf("[%s: connectFrom] NULL source\n", OBJ_NAME(obj_));
        return false;
    }

    if (inln >= numInlets()) {
        printf("[%s: connectFrom %s] invalid destination inlet: %dn",
            OBJ_NAME(obj_), OBJ_NAME(src), inln);
        return false;
    }

    if (outn >= obj_noutlets(src)) {
        printf("[%s: connectFrom %s] invalid source outlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(src), outn);
        return false;
    }

#undef OBJ_NAME

    return obj_connect(src, outn, obj_, inln) != 0;
}

bool pd::External::connectFrom(int outn, pd::External& ext, int inln)
{
    return connectFrom(outn, ext.object(), inln);
}

t_object* pd::External::object()
{
    return obj_;
}

void pd::External::bang()
{
    if (!obj_)
        return;

    pd_bang(&obj_->te_g.g_pd);
}

void pd::External::sendBang()
{
    bang();
}

void pd::External::sendFloat(t_float v)
{
    if (!obj_)
        return;

    pd_float(&obj_->te_g.g_pd, v);
}

void pd::External::sendSymbol(t_symbol* s)
{
    if (!obj_)
        return;

    pd_symbol(&obj_->te_g.g_pd, s);
}

void pd::External::sendList(const AtomList& l)
{
    if (!obj_)
        return;

    pd_list(&obj_->te_g.g_pd, &s_list, l.size(), l.toPdData());
}

void pd::External::sendMessage(t_symbol* msg, const AtomList& args)
{
    if (!obj_)
        return;

    pd_typedmess(&obj_->te_g.g_pd, msg, args.size(), args.toPdData());
}

int pd::External::numOutlets() const
{
    if (!obj_)
        return 0;

    return obj_noutlets(obj_);
}

int pd::External::numInlets() const
{
    if (!obj_)
        return 0;

    return obj_ninlets(obj_);
}

PureData::PureData()
{
    pd_init();
}

CanvasPtr PureData::createTopCanvas(const char* name)
{
    CanvasPtr ptr;

    CanvasMap::iterator it = canvas_map_.find(name);
    if (it != canvas_map_.end())
        return it->second;

    AtomList l(0.f, 0.f); // x, y
    l.append(600); // width
    l.append(400); // height
    l.append(10); // font size

    canvas_setcurrent(0);

    if (platform::is_path_relative(name)) {
        glob_setfilename(0, gensym(name), &s_);
    } else {
        std::string dir = platform::dirname(name);
        std::string fname = platform::basename(name);
        glob_setfilename(0, gensym(fname.c_str()), gensym(dir.c_str()));
    }

    t_canvas* cnv = canvas_new(0, gensym(name), l.size(), l.toPdData());

    if (!cnv)
        return ptr;

    ptr.reset(new Canvas(cnv));
    canvas_map_[name] = ptr;
    return ptr;
}

CanvasPtr PureData::createSubpatch(_glist* parent, const char* name)
{
    t_canvas* cnv = canvas_new(0, gensym(name), 0, NULL);
    if (!cnv)
        return CanvasPtr();

    cnv->gl_name = gensym(name);
    cnv->gl_owner = parent;

    return CanvasPtr(new Canvas(cnv));
}

PureData& PureData::instance()
{
    static PureData pd;
    return pd;
}
