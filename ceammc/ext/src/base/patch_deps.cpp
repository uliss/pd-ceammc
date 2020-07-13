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
#include "patch_deps.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "datatype_dict.h"

PatchDeps::PatchDeps(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

namespace {
using namespace ceammc::pd;

void print_cnv(CanvasTree* tree, int level)
{
    if (tree == nullptr)
        return;

    std::string ind(2 * level, ' ');
    LIB_POST << ind << "canvas: " << tree->cnv_name;
    LIB_POST << ind << "dir:  " << tree->cnv_dir;
    for (auto& obj : tree->objects) {
        switch (obj.obj_type) {
        case CanvasTree::ABSTRACTION:
            LIB_POST << ind << "[" << obj.obj_dir->s_name << "/" << obj.obj_name->s_name << "]";
            print_cnv(obj.obj_tree.get(), level + 1);
            break;
        case CanvasTree::SUBPATCH:
            LIB_POST << ind << "[pd " << obj.obj_name->s_name << "]";
            print_cnv(obj.obj_tree.get(), level + 1);
            break;
        case CanvasTree::OBJECT:
            if (obj.obj_dir == &s_ || obj.obj_dir == nullptr) // core objects
                LIB_POST << ind << "[" << obj.obj_name->s_name << "]";
            else
                LIB_POST << ind << "[" << obj.obj_name->s_name << "] " << platform::basename(obj.obj_dir->s_name);
        default:
            break;
        }
    }
}

DataTypeDict tree_to_dict(CanvasTree* tree)
{
    DataTypeDict dict;

    if (!tree)
        return dict;

    for (auto& obj : tree->objects) {
        switch (obj.obj_type) {
        case CanvasTree::SUBPATCH: {
            for (auto& kv : tree_to_dict(obj.obj_tree.get())) {
                if (!dict.contains(kv.first)) {
                    dict.innerData().insert(kv);
                } else {
                    auto& lst = dict.at(kv.first);
                    for (auto& a : kv.second) {
                        if (!lst.contains(a))
                            lst.append(a);
                    }
                }
            }
        } break;
        case CanvasTree::ABSTRACTION: {
            t_symbol* abs_fname = gensym((std::string(obj.obj_dir->s_name) + '/' + obj.obj_name->s_name).c_str());
            if (!dict.contains(abs_fname))
                dict.insert(abs_fname, Atom(obj.obj_name));

            for (auto& kv : tree_to_dict(obj.obj_tree.get())) {
                if (!dict.contains(kv.first)) {
                    dict.innerData().insert(kv);
                } else {
                    auto& lst = dict.at(kv.first);
                    for (auto& a : kv.second) {
                        if (!lst.contains(a))
                            lst.append(a);
                    }
                }
            }
        } break;
        case CanvasTree::OBJECT: { // external -> obj list
        default:
            if (!dict.contains(obj.obj_dir))
                dict.insert(obj.obj_dir, Atom(obj.obj_name));
            else {
                auto& lst = dict.at(obj.obj_dir);
                if (!lst.contains(obj.obj_name))
                    lst.append(obj.obj_name);
            }
        } break;
        }
    }

    return dict;
}
}

void PatchDeps::onBang()
{
    auto t = canvas_info_tree(canvas());
    atomTo(0, DictAtom(tree_to_dict(t.get())));
}

void PatchDeps::dump() const
{
    BaseObject::dump();
    auto t = canvas_info_tree(canvas());
    print_cnv(t.get(), 0);
}

void setup_patch_deps()
{
    ObjectFactory<PatchDeps> obj("patch.deps");
    obj.setDescription("patch dependencies: used objects and abstractions");
    obj.addAuthor("Serge Poltavski");
    obj.setKeywords({ "patch", "canvas", "deps" });
    obj.setCategory("patch");
    obj.setSinceVersion(0, 9);

    PatchDeps::setInletsInfo(obj.classPointer(), { "on bang: output patch dependencies" });
    PatchDeps::setOutletsInfo(obj.classPointer(), { "data: Dict" });
}
