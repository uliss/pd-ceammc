/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "patch_tree.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "datatype_dict.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

PatchTree::PatchTree(const PdArgs& a)
    : BaseObject(a)
    , mode_(nullptr)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { "deps", "abs", "obj" });
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@deps", mode_, gensym("deps")));
    addProperty(new SymbolEnumAlias("@abs", mode_, gensym("abs")));
    addProperty(new SymbolEnumAlias("@obj", mode_, gensym("obj")));
}

static DataTypeDict canvas_tree(t_gobj* x)
{
    DataTypeDict t;

    for (; x != nullptr; x = x->g_next) {
        auto pdobj = x->g_pd;
        if (!pd_checkobject(&pdobj))
            continue;

        if (pd_class(&pdobj) == canvas_class) {

            t_canvas* c = reinterpret_cast<t_canvas*>(x);

            if (canvas_isabstraction(c)) {
                std::string dir = canvas_info_dir(c)->s_name;
                std::string name = canvas_info_name(c)->s_name;
                t_symbol* full_path = gensym((dir + "/" + name).c_str());
                if (!t.contains(full_path)) {
                    DataTypeDict d;
                    d.insert("n", 1);
                    d.insert("obj", full_path);
                    t.insert(full_path, DictAtom(d));
                } else {
                    auto& kv = t.at(full_path);
                    auto d = const_cast<DataTypeDict*>(kv[0].asD<DataTypeDict>());
                    d->at("n")[0].applyFloat([](t_float f) { return f + 1; });
                }
            }

            for (auto& kv : canvas_tree(c->gl_list)) {
                if (!t.contains(kv.first))
                    t.innerData().insert(kv);
                else {
                    auto& info = t.at(kv.first);
                    auto d = const_cast<DataTypeDict*>(info[0].asD<DataTypeDict>());
                    auto& n = d->at("n")[0];
                    auto& objlist_src = kv.second[0].asD<DataTypeDict>()->at("obj");
                    auto& objlist_dest = d->at("obj");
                    for (auto& a : objlist_src) {
                        n.applyFloat([](t_float f) { return f + 1; });
                        if (!objlist_dest.contains(a)) {
                            objlist_dest.append(a);
                        }
                    }
                }
            }

            continue;
        }

        auto name = pdobj->c_name;
        if (pdobj->c_externdir) {
            if (!t.contains(pdobj->c_externdir)) {
                DataTypeDict d;
                d.insert("n", 1);
                d.insert("obj", name);
                t.insert(pdobj->c_externdir, DictAtom(d));
            } else {
                auto& kv = t.at(pdobj->c_externdir);
                auto d = const_cast<DataTypeDict*>(kv[0].asD<DataTypeDict>());
                d->at("n")[0].applyFloat([](t_float f) { return f + 1; });
                auto& objlist = d->at("obj");
                if (!objlist.contains(name))
                    objlist.append(Atom(name));
            }
        }
        //        t.insert(pdobj->c_externdir, 1);
        //        if (pdobj->c_helpname)
        //            info->insert("help", pdobj->c_helpname);

        //        t.insert(name, info);
        //        LIB_DBG << "insert: " << name;
    }

    return t;
}

void PatchTree::onBang()
{
//    if (mode_->value() == SYM_DEPS) {
        atomTo(0, DictAtom(canvas_tree(canvas()->gl_list)));
//    }
}

void setup_patch_tree()
{
    ObjectFactory<PatchTree> obj("patch.tree");
}
