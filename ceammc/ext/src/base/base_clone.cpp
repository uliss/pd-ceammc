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
#include "ceammc_inlet.h"
#include "ceammc_outlet.h"
#include "fmt/format.h"

constexpr const char* CONTAINTER_NAME = "/CONTAINER/";
constexpr const char* PATTERN_NAME = "/PATTERN/";
constexpr const char* SYM_CANVAS_RESTORE = "#Z";

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
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

t_binbuf* canvas_docopy(const t_canvas* z)
{
    auto x = (t_canvas*)z;

    t_binbuf* b = binbuf_new();
    for (t_gobj* y = x->gl_list; y; y = y->g_next)
        gobj_save(y, b);

    t_linetraverser t;
    linetraverser_start(&t, x);

    while (linetraverser_next(&t)) {
        int srcno = canvas_getindex(x, &t.tr_ob->ob_g);
        int sinkno = canvas_getindex(x, &t.tr_ob2->ob_g);

        binbuf_addv(b, "ssiiii;", gensym("#X"), gensym("connect"),
            srcno, t.tr_outno,
            sinkno, t.tr_inno);
    }
    return (b);
}

t_canvas* clone_create_container(t_canvas* owner)
{
    t_atom a[6];
    t_symbol* s = gensym(CONTAINTER_NAME);
    SETFLOAT(a, 0);
    SETFLOAT(a + 1, 22);
    SETFLOAT(a + 2, 700);
    SETFLOAT(a + 3, 500);
    SETSYMBOL(a + 4, s);
    SETFLOAT(a + 5, 1);
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
    SETFLOAT(a + 5, 0);
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

void canvas_dopaste(t_canvas* x, const t_binbuf* b)
{
    int dspstate = canvas_suspend_dsp();
    t_symbol* asym = gensym("#A");
    /* save and clear bindings to symbols #A, #N, #X; restore when done */
    t_pd *boundx = s__X.s_thing, *bounda = asym->s_thing,
         *boundn = s__N.s_thing;
    asym->s_thing = 0;
    s__X.s_thing = &x->gl_pd;
    s__N.s_thing = &pd_canvasmaker;

    binbuf_eval(b, 0, 0, 0);
    canvas_resume_dsp(dspstate);
    //    if (!sys_noloadbang)
    //        glist_donewloadbangs(x);
    asym->s_thing = bounda;
    s__X.s_thing = boundx;
    s__N.s_thing = boundn;
}

}

CloneInstance::CloneInstance(size_t idx, t_canvas* owner, const AtomListView& args)
    : idx_(idx)
    , canvas_(nullptr)
    , args_(args)
{
    t_atom a[6];
    t_symbol* s = gensym(fmt::format("instance({})", idx).c_str());
    SETFLOAT(a, 0);
    SETFLOAT(a + 1, 22);
    SETFLOAT(a + 2, 700);
    SETFLOAT(a + 3, 500);
    SETSYMBOL(a + 4, s);
    SETFLOAT(a + 5, 0);
    canvas_ = canvas_new(0, 0, 6, a);
    canvas_->gl_owner = owner;
    clone_pop_canvas(canvas_, false);
}

CloneInstance::CloneInstance(CloneInstance&& ci)
    : idx_(ci.idx_)
    , canvas_(std::move(ci.canvas_))
    , args_(std::move(ci.args_))
{
}

CloneInstance::~CloneInstance()
{
    if (canvas_) {
        canvas_free(canvas_);
        canvas_ = nullptr;
    }
}

void CloneInstance::fillWithPattern(const t_binbuf* pattern)
{
    if (!canvas_)
        return;

    clear();
    canvas_dopaste(canvas_, pattern);
}

void CloneInstance::loadbang()
{
    if (canvas_)
        canvas_loadbang(canvas_);
}

void CloneInstance::clear()
{
    if (!canvas_)
        return;

    t_gobj* y = nullptr;
    while ((y = canvas_->gl_list))
        glist_delete(canvas_, y);
}

void CloneInstance::open()
{
    if (canvas_)
        canvas_vis(canvas_, 1);
}

BaseClone::BaseClone(const PdArgs& args)
    : SoundExternal(args)
    , num_(nullptr)
    , args_(nullptr)
    , wrapper_(nullptr)
    , pattern_(nullptr)
{
    args_ = new ListProperty("@args");
    args_->setInitOnly();
    args_->setArgIndex(1);
    addProperty(args_);

    num_ = new IntProperty("@n", 0);
    num_->setInitOnly();
    num_->checkMinEq(0);
    num_->setArgIndex(0);
    addProperty(num_);

    // pattern container
    wrapper_ = clone_create_container(canvas());

    if (!isPatchLoading()) {
        pattern_ = clone_create_pattern(wrapper_);
    } else {
        clone_bind_restore(owner());
    }
}

BaseClone::~BaseClone()
{
    if (wrapper_) {
        canvas_free(wrapper_);
        wrapper_ = nullptr;
        // clone_pattern_ should be freed automatically
    }
}

void BaseClone::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    if (!wrapper_) {
        OBJ_ERR << "NULL canvas pattern wrapper";
        return;
    }

    if (!pattern_) {
        OBJ_ERR << "NULL canvas pattern";
        return;
    }

    OBJ_LOG << __FUNCTION__;

    canvas_vis(pattern_, 1);
}

void BaseClone::initDone()
{
    try {
        if (num_->value() < 1)
            throw std::invalid_argument("number of copies required");

        if (!initInstances(args_->value().view()))
            throw std::runtime_error("can't init instances");

        //        allocSignals();
        //        allocInlets();
        //        allocOutlets();
    } catch (std::exception& e) {
        if (!args().empty())
            OBJ_ERR << e.what();
        else
            OBJ_LOG << e.what(); // object without args - used in help
    }
}

bool BaseClone::initInstances(const AtomListView& patch_args)
{
    const size_t NINSTANCE = num_->value();
    instances_.reserve(NINSTANCE);

    for (size_t i = 0; i < NINSTANCE; i++) {
        if (!initInstance(i, patch_args))
            return false;
    }

    return true;
}

bool BaseClone::initInstance(size_t idx, const AtomListView& args)
{
    OBJ_LOG << fmt::format("instance {}", idx);
    instances_.emplace_back(idx, wrapper_, args);
    return true;
}

void BaseClone::updateInstances()
{
    if (pattern_) {
        auto bb = canvas_docopy(pattern_);

        for (auto& i : instances_) {
            i.fillWithPattern(bb);
            i.loadbang();
        }

        binbuf_free(bb);

        gobj_vis(&owner()->te_g, canvas(), 0);
        updateInlets();
        updateOutlets();
        gobj_vis(&owner()->te_g, canvas(), 1);

        canvas_fixlinesfor(canvas(), owner());
    }
}

void BaseClone::updateInlets()
{
    const size_t NUM_PATTERN_INLETS = pattern_ ? obj_ninlets(&pattern_->gl_obj) : 0;
    const size_t NUM_ISTANCES = num_->value();
    const auto new_count = NUM_PATTERN_INLETS * NUM_ISTANCES;
    const auto prev_count = inlets().size();

    // add/remove inlets if needed
    if (prev_count < new_count) {
        // add new
        for (size_t i = prev_count; i < new_count; i++) {
            auto in = inlet_new(owner(), nullptr, nullptr, nullptr);
            appendInlet(in);
        }
    } else if (prev_count > new_count) {
        // remove extra
        while (inlets().size() > new_count) {
            if (!popInlet())
                break;
        }
    }

    // update inlet destinations
    for (size_t n = 0; n < NUM_ISTANCES; n++) {
        // instance canvas
        auto ci = instances_[n].canvas();
        auto x = &ci->gl_obj;

        // inlet index
        size_t idx = n * NUM_PATTERN_INLETS;

        // iterate instance inlets
        InletIterator it(x);
        while (it) {
            auto inl = inlets()[idx];
            util::inlet_set_dest(inl, it.asObject());
            util::inlet_set_signal(inl, it.isSignal());
            it.next();
            idx++;
        }
    }
}

void BaseClone::updateOutlets()
{
    const size_t NUM_PATTERN_OUTLETS = pattern_ ? obj_noutlets(&pattern_->gl_obj) : 0;
    const size_t NUM_ISTANCES = num_->value();
    const auto new_count = NUM_PATTERN_OUTLETS * NUM_ISTANCES;
    const auto prev_count = outlets().size();

    // add/remove outlets if needed
    if (prev_count < new_count) {
        // add new
        for (size_t i = prev_count; i < new_count; i++) {
            auto out = outlet_new(owner(), &s_list);
            appendOutlet(out);
        }
    } else if (prev_count > new_count) {
        // remove extra
        while (outlets().size() > new_count) {
            if (!popOutlet())
                break;
        }
    }

    proxy_.clear();
    proxy_.reserve(outlets().size());

    // divizion by zero check: in next loop
    if (outlets().empty() || NUM_PATTERN_OUTLETS == 0)
        return;

    for (size_t i = 0; i < outlets().size(); i++) {
        const auto inst_idx = i / NUM_PATTERN_OUTLETS;
        const auto src_idx = i % NUM_PATTERN_OUTLETS;

        proxy_.emplace_back(this, (int)i);

        auto ci = instances_[inst_idx].canvas();
        auto x = &ci->gl_obj;
        obj_connect(x, src_idx, proxy_.back().object(), 0);
        util::outlet_set_signal(outletAt(i), obj_issignaloutlet(x, src_idx));
    }
}

void BaseClone::processBlock(const t_sample** in, t_sample** out)
{
}

void BaseClone::setupDSP(t_signal** sp)
{
}

void BaseClone::m_open(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, s))
        return;

    int n = lv.intAt(0, 0);
    if (n < 0 || n > instances_.size()) {
        OBJ_ERR << fmt::format("invalid index: {}", n);
        return;
    }

    instances_[n].open();
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
    x->gl_owner = wrapper_;

    auto t = &x->gl_obj;

    t->te_width = 0; /* don't know it yet. */
    t->te_type = T_OBJECT;
    t->te_binbuf = binbuf_new();
    t->te_xpix = atom_getfloatarg(0, lv.size(), lv.toPdData());
    t->te_ypix = atom_getfloatarg(1, lv.size(), lv.toPdData());

    if (lv.size() > 2)
        binbuf_restore(t->te_binbuf, lv.size() - 2, lv.toPdData() + 2);

    glist_add(wrapper_, &t->te_g);

    if (pattern_) {
        canvas_free(pattern_);
        pattern_ = nullptr;
    }

    pattern_ = x;

    updateInstances();
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

    if (wrapper_ && pattern_) {
        auto x = pattern_;

        /* have to go to original binbuf to find out how we were named. */
        binbuf_addv(b, "ssiiiisi;", gensym("#N"), gensym("canvas"),
            (int)(x->gl_screenx1),
            (int)(x->gl_screeny1),
            (int)(x->gl_screenx2 - x->gl_screenx1),
            (int)(x->gl_screeny2 - x->gl_screeny1),
            gensym(PATTERN_NAME),
            0);

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
        binbuf_addv(b, ";");
    }

    updateInstances();

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
    BaseCloneFactory obj("clone:", OBJECT_FACTORY_DEFAULT);
    obj.useClick();
    obj.addMethod("open", &BaseClone::m_open);
}
