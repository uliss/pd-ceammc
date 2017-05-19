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
typedef t_object* (*t_newgimme)(t_symbol* s, int argc, t_atom* argv);

using namespace ceammc;

pd::External::External(const char* name, const AtomList& lst)
    : obj_(0)
{
    t_symbol* OBJ_NAME = gensym(name);
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
            }
        }
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        obj_ = 0;
    }
}

pd::External::~External()
{
    if (!obj_) {
        pd_free(&obj_->te_g.g_pd);
    }
}

bool pd::External::isNull() const
{
    return obj_ == 0;
}

bool pd::External::connectTo(int outn, t_object* dest, int inln)
{
    if (!obj_)
        return false;

    if (obj_ninlets(obj_) < 1)
        return false;

    obj_connect(obj_, outn, dest, inln);
    return true;
}

bool pd::External::connectTo(int outn, pd::External& ext, int inln)
{
    return connectTo(outn, ext.object(), inln);
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

void pd::External::sendFloat(t_float v)
{
    if (!obj_)
        return;

    pd_float(&obj_->te_g.g_pd, v);
}

PureData::PureData()
{
    pd_init();
}

CanvasPtr PureData::createTopCanvas(const char* name)
{
    CanvasPtr ptr;

    CanvasMap::iterator it = canvas_map_.find(name);
    if (it != canvas_map_.end()) {
        std::cerr << "Canvas exist's: " << name << "\n";
        return it->second;
    }

    AtomList l(0.f, 0.f); // x, y
    l.append(600); // width
    l.append(400); // height
    l.append(10); // font size

    t_canvas* cnv = canvas_new(0, gensym(name), l.size(), l.toPdData());

    if (!cnv)
        return ptr;

    ptr.reset(new Canvas(cnv));
    canvas_map_[name] = ptr;
    return ptr;
}

PureData& PureData::instance()
{
    static PureData pd;
    return pd;
}
