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

PatchTree::PatchTree(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

namespace {
using namespace ceammc::pd;

DataTypeDict canvas_tree(CanvasTree* tree)
{
    DataTypeDict t;
    if (tree == nullptr)
        return t;

    for (auto& obj : tree->objects) {
        switch (obj.obj_type) {
        case CanvasTree::ABSTRACTION: {
            t_symbol* k = gensym((std::string(obj.obj_name->s_name) + ".pd").c_str());
            if (!t.contains(k))
                t.insert(k, AtomList());

            t.at(k).append(DictAtom(canvas_tree(obj.obj_tree.get())));
        } break;
        case CanvasTree::SUBPATCH: {
            t_symbol* k = gensym((std::string("pd ") + obj.obj_name->s_name).c_str());
            if (!t.contains(k))
                t.insert(k, AtomList());

            t.at(k).append(DictAtom(canvas_tree(obj.obj_tree.get())));
        } break;
        case CanvasTree::OBJECT:
        default: {
            if (!t.contains("obj"))
                t.insert("obj", AtomList());

            t.at("obj").append(Atom(obj.obj_name));
        }
        }
    }

    return t;
}
}

void PatchTree::onBang()
{
    auto t = canvas_info_tree(canvas());
    atomTo(0, DictAtom(canvas_tree(t.get())));
}

void setup_patch_tree()
{
    ObjectFactory<PatchTree> obj("patch.tree");
    obj.setDescription("patch tree");
    obj.addAuthor("Serge Poltavski");
    obj.setKeywords({ "patch", "tree" });
    obj.setCategory("patch");
    obj.setSinceVersion(0, 9);

    PatchTree::setInletsInfo(obj.classPointer(), { "on bang: output patch tree" });
    PatchTree::setOutletsInfo(obj.classPointer(), { "data: Dict" });
}
