/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "base_clone.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

constexpr const char* CONTAINTER_NAME = "/CONTAINER/";
constexpr const char* PATTERN_NAME = "/clone:/";
constexpr const char* SYM_CANVAS_RESTORE = "#Z";

extern "C" {
#include "g_canvas.h"
}

namespace {

void clone_pop_canvas(t_canvas* x, bool show)
{
    if (show)
        canvas_vis(x, 1);

    pd_popsym(&x->gl_pd);
    canvas_resortinlets(x);
    canvas_resortoutlets(x);
    x->gl_loading = 0;
}

t_canvas* clone_create_container(t_canvas* owner)
{
    t_atom a[5];
    t_symbol* s = gensym(CONTAINTER_NAME);
    SETFLOAT(a, 0);
    SETFLOAT(a + 1, 22);
    SETFLOAT(a + 2, 700);
    SETFLOAT(a + 3, 500);
    SETFLOAT(a + 4, 12);
    //        SETSYMBOL(a + 4, s);
    //        SETFLOAT(a + 5, 1);
    auto* c = canvas_new(0, 0, 5, a);
    c->gl_owner = owner;
    clone_pop_canvas(c, false);
    return c;
}

t_canvas* clone_create_pattern(t_canvas* owner)
{
    t_atom a[6];
    t_symbol* s = gensym(PATTERN_NAME);
    SETFLOAT(a, 0);
    SETFLOAT(a + 1, 22);
    SETFLOAT(a + 2, 700);
    SETFLOAT(a + 3, 500);
    SETSYMBOL(a + 4, s);
    SETFLOAT(a + 5, 1);
    auto c = canvas_new(0, 0, 6, a);
    c->gl_owner = owner;
    clone_pop_canvas(c, true);
    glist_add(owner, &c->gl_obj.te_g);
    return c;
}

void clone_bind_restore(t_object* owner)
{
    auto z = gensym(SYM_CANVAS_RESTORE);
    z->s_thing = nullptr;
    pd_bind(&owner->te_g.g_pd, z);
}

}

BaseClone::BaseClone(const PdArgs& args)
    : SoundExternal(args)
    , num_(nullptr)
    , patch_(nullptr)
    , args_(nullptr)
    , canvas_cont_(nullptr)
    , clone_pattern_(nullptr)
{
    args_ = new ListProperty("@args");
    args_->setInitOnly();
    args_->setArgIndex(2);
    addProperty(args_);

    patch_ = new SymbolProperty("@patch", &s_);
    patch_->setInitOnly();
    patch_->setArgIndex(1);
    addProperty(patch_);

    num_ = new IntProperty("@n", 0);
    num_->setInitOnly();
    num_->checkMinEq(0);
    num_->setArgIndex(0);
    addProperty(num_);

    // pattern container
    canvas_cont_ = clone_create_container(canvas());

    if (!isPatchLoading()) {
        clone_pattern_ = clone_create_pattern(canvas_cont_);
    } else {
        clone_bind_restore(owner());
    }
}

BaseClone::~BaseClone()
{
    if (canvas_cont_) {
        canvas_free(canvas_cont_);
        canvas_cont_ = nullptr;
    }
}

void BaseClone::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    if (!canvas_cont_) {
        OBJ_ERR << "NULL canvas pattern container";
        return;
    }

    if (!clone_pattern_) {
        OBJ_ERR << "NULL canvas pattern";
        return;
    }

    OBJ_LOG << __FUNCTION__;

    canvas_vis(clone_pattern_, 1);
}

void BaseClone::initDone()
{
    try {
        if (patch_->value() == &s_)
            throw std::invalid_argument("@patch property required");

        if (num_->value() < 1)
            throw std::invalid_argument("number of copies required");

        if (!initInstances(patch_->value(), args_->value().view()))
            throw std::runtime_error(fmt::format("can't load the patch {0}.pd", patch_->value()->s_name));

        //        allocSignals();
        //        allocInlets();
        //        allocOutlets();
    } catch (std::exception& e) {
        if (!args().empty())
            OBJ_ERR << e.what();
        else
            OBJ_LOG << e.what(); // object without args - used in help
    }

    // call loadbang in 5 ticks
    // clock_.delay(5);
}

bool BaseClone::initInstances(t_symbol* name, const AtomListView& patch_args)
{
    const size_t NINSTANCE = num_->value();
    //    instances_.assign(NINSTANCE, ProcessInstance());

    //    AtomList load_args;
    //    load_args.append(Atom(SYM_2D));
    //    load_args.append(Atom(SYM_PLANEWAVES));
    //    load_args.append(Atom(NINSTANCE)); // number of channels
    //    load_args.append(Atom()); // channel index
    //    load_args.append(Atom()); // channel index
    //    load_args.append(patch_args);

    //    for (size_t i = 0; i < NINSTANCE; i++) {
    //        load_args[3].setFloat(i, true);
    //        load_args[4].setFloat(i, true);

    //        if (!instances_[i].init(name, load_args)) {
    //            instances_.clear();
    //            return false;
    //        }
    //    }

    return true;
}

void BaseClone::processBlock(const t_sample** in, t_sample** out)
{
}

void BaseClone::setupDSP(t_signal** sp)
{
}

void BaseClone::onRestore(const AtomListView& lv)
{
    OBJ_LOG << __FUNCTION__;

    auto cnv = gensym("#X");
    if (cnv->s_thing == nullptr) {
        OBJ_ERR << "null canvas";
        return;
    }

    // current canvas binding
    auto z = cnv->s_thing;
    if (!z || *z != canvas_class) {
        OBJ_ERR << "not a canvas";
        return;
    }

    // current canvas pointer
    auto x = (t_canvas*)z;

    clone_pop_canvas(x, false);
    x->gl_owner = canvas_cont_;

    auto t = &x->gl_obj;

    t->te_width = 0; /* don't know it yet. */
    t->te_type = T_OBJECT;
    t->te_binbuf = binbuf_new();
    t->te_xpix = atom_getfloatarg(0, lv.size(), lv.toPdData());
    t->te_ypix = atom_getfloatarg(1, lv.size(), lv.toPdData());

    if (lv.size() > 2)
        binbuf_restore(t->te_binbuf, lv.size() - 2, lv.toPdData() + 2);

    glist_add(canvas_cont_, &t->te_g);

    if (clone_pattern_) {
        canvas_free(clone_pattern_);
        clone_pattern_ = nullptr;
    }

    clone_pattern_ = x;
}

void BaseClone::onSave(t_binbuf* b)
{
    OBJ_LOG << __FUNCTION__;

    auto obj = owner();

    // save object
    binbuf_addv(b, "ssii", gensym("#X"), gensym("obj"),
        (int)obj->te_xpix, (int)obj->te_ypix);
    binbuf_addbinbuf(b, obj->te_binbuf);
    if (obj->te_width)
        binbuf_addv(b, ",si", gensym("f"), (int)obj->te_width);
    binbuf_addv(b, ";");

    if (canvas_cont_ && clone_pattern_) {
        auto x = clone_pattern_;

        /* have to go to original binbuf to find out how we were named. */
        binbuf_addv(b, "ssiiiii;", gensym("#N"), gensym("canvas"),
            (int)(x->gl_screenx1),
            (int)(x->gl_screeny1),
            (int)(x->gl_screenx2 - x->gl_screenx1),
            (int)(x->gl_screeny2 - x->gl_screeny1),
            12);

        // save objects
        for (auto y = x->gl_list; y != nullptr; y = y->g_next)
            gobj_save(y, b);

        // save connections
        t_linetraverser t;
        linetraverser_start(&t, x);
        while (linetraverser_next(&t)) {
            int srcno = canvas_getindex(x, &t.tr_ob->ob_g);
            int sinkno = canvas_getindex(x, &t.tr_ob2->ob_g);
            binbuf_addv(b, "ssiiii;", gensym("#X"), gensym("connect"),
                srcno, t.tr_outno, sinkno, t.tr_inno);
        }

        binbuf_addv(b, "ssiis", gensym("#Z"), gensym("restore"),
            (int)obj->te_xpix, (int)obj->te_ypix, gensym(PATTERN_NAME));
        //    binbuf_addbinbuf(b, obj->te_binbuf);
        binbuf_addv(b, ";");
    }

    //    if (obj->te_width)
    //        binbuf_addv(b, "ssi;",
    //            gensym("#X"), gensym("f"), (int)obj->te_width);
}

class BaseCloneFactory : public SoundExternalFactory<BaseClone> {
public:
    BaseCloneFactory(const char* name, int flags)
        : SoundExternalFactory<BaseClone>(name, flags)
    {
        class_addmethod(this->classPointer(), (t_method)restore_fn, gensym("restore"), A_GIMME, 0);
        class_setsavefn(this->classPointer(), (t_savefn)save_fn);
    }

    static void save_fn(ObjectProxy* x, t_binbuf* b)
    {
        x->impl->onSave(b);
    }

    static void restore_fn(ObjectProxy* x, t_symbol* /*sel*/, int argc, t_atom* argv)
    {
        x->impl->onRestore(AtomListView(argv, argc));
    }
};

void setup_base_clone()
{
    BaseCloneFactory obj("clone:", OBJECT_FACTORY_NO_DEFAULT_INLET);

    obj.useClick();

    //    obj.addAlias("clone:>");

    //    class_addmethod(obj.classPointer(), (t_method)canvas_setbounds,
    //        gensym("setbounds"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);

    //    class_addmethod(obj.classPointer(), (t_method)canvas_rename_method,
    //        gensym("rename"), A_GIMME, 0);
}
