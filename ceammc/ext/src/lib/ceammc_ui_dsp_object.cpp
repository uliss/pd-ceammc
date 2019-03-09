/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "ceammc_ui_dsp_object.h"
#include "ceammc_dsp_ui.h"
#include "ceammc_preset.h"

#include "ceammc_cicm.h"
#include "m_pd.h"

#include <boost/foreach.hpp>

using namespace ceammc;

UIDspError::UIDspError(const UIDspObject* obj)
    : obj_(obj)
{
}

UIDspError::~UIDspError()
{
    if (obj_ != 0)
        pd_error(static_cast<void*>(obj_->asPd()), "[%s] %s", obj_->name()->s_name, str().c_str());
    else
        pd_error(0, "[ceammc] %s", str().c_str());
}

UIDspDebug::UIDspDebug(const UIDspObject* obj)
    : obj_(obj)
{
}

UIDspDebug::~UIDspDebug()
{
    if (obj_ != 0)
        post("[%s] %s", obj_->name()->s_name, str().c_str());
    else
        post("[ceammc] %s", str().c_str());
}

const char* UIDspObject::BG_LAYER = "background_layer";

#ifdef __APPLE__
const char* UIDspObject::FONT_FAMILY = "Helvetica";
const int UIDspObject::FONT_SIZE = 12;
const int UIDspObject::FONT_SIZE_SMALL = 8;
#elif _WIN32
const char* UIDspObject::FONT_FAMILY = "DejaVu Sans Mono";
const int UIDspObject::FONT_SIZE = 9;
const int UIDspObject::FONT_SIZE_SMALL = 6;
#else
const char* UIDspObject::FONT_FAMILY = "DejaVu Sans Mono";
const int UIDspObject::FONT_SIZE = 9;
const int UIDspObject::FONT_SIZE_SMALL = 6;
#endif

const char* UIDspObject::FONT_STYLE = "roman";
const char* UIDspObject::FONT_WEIGHT = "normal";
const char* UIDspObject::COLOR_ACTIVE = "#00C0FF";

UIDspObject::PresetNameMap UIDspObject::presets_;

UIDspObject::UIDspObject()
    : name_(&s_)
    , bg_layer_(asEBox(), gensym(BG_LAYER))
    , old_preset_id_(s_null)
    , cursor_(ECURSOR_LEFT_PTR)
    , samplerate_(44100)
    , blocksize_(0)
    , use_presets_(false)
    , prop_color_background(rgba_white)
    , prop_color_border(rgba_black)
{
}

UIDspObject::~UIDspObject()
{
    unbindPreset(presetId());
    unbindAll();
}

t_ebox* UIDspObject::asEBox() const { return reinterpret_cast<t_ebox*>(const_cast<UIDspObject*>(this)); }

t_edspbox* UIDspObject::asDspBox() const
{
    return const_cast<UIDspObject*>(this);
}

t_object* UIDspObject::asPdObject() const
{
    return &(asEBox()->b_obj.o_obj);
}

t_pd* UIDspObject::asPd() const
{
    return &asPdObject()->te_g.g_pd;
}

t_outlet* UIDspObject::createOutlet()
{
    outlets_.push_back(outlet_new(asPdObject(), &s_list));
    return outlets_.back();
}

t_canvas* UIDspObject::canvas() const
{
    return eobj_getcanvas((void*)this);
}

bool UIDspObject::isPatchLoading() const
{
    t_canvas* c = canvas();
    return c ? c->gl_loading : false;
}

bool UIDspObject::isPatchEdited() const
{
    t_canvas* c = canvas();
    return c ? c->gl_edit : false;
}

void UIDspObject::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    name_ = name;
    args_ = args;
    use_presets_ = usePresets;

    if (use_presets_)
        presetInit();
}

t_symbol* UIDspObject::name() const
{
    return name_;
}

t_symbol* UIDspObject::presetId()
{
    return b_objpreset_id;
}

void UIDspObject::paint(t_object* view)
{
}

void UIDspObject::redraw()
{
    ebox_redraw(asEBox());
}

void UIDspObject::redrawBGLayer()
{
    bg_layer_.invalidate();
    redraw();
}

void UIDspObject::updateSize()
{
    ebox_notify(asEBox(), s_size, &s_, NULL, NULL);
}

void UIDspObject::resize(int w, int h)
{
    b_rect.width = w;
    b_rect.height = h;
    updateSize();
}

void UIDspObject::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIDspObject::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIDspObject::onMouseDown(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIDspObject::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIDspObject::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIDspObject::onMouseEnter(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIDspObject::onMouseWheel(t_object* view, const t_pt& pt, long modifiers, double delta)
{
}

void UIDspObject::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
}

t_pd_err UIDspObject::notify(t_symbol* /*attr_name*/, t_symbol* msg)
{
    if (msg == s_attr_modified) {
        redrawBGLayer();
    }

    return 0;
}

void UIDspObject::okSize(t_rect* newrect)
{
}

void UIDspObject::setDrawParams(t_object*, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 1;
    params->d_bordercolor = prop_color_border;
    params->d_boxfillcolor = prop_color_background;
}

void UIDspObject::onBang()
{
}

void UIDspObject::onFloat(t_float f)
{
}

void UIDspObject::onSymbol(t_symbol* s)
{
}

void UIDspObject::onList(const AtomList& lst)
{
}

void UIDspObject::onAny(t_symbol* s, const AtomList& lst)
{
    LIB_ERR << "unknown message: " << s->s_name;
}

void UIDspObject::onKey(int k)
{
}

void UIDspObject::onData(const DataPtr& ptr)
{
}

void UIDspObject::onProperty(t_symbol* s, const AtomList& lst)
{
}

void UIDspObject::loadPreset(size_t idx)
{
}

void UIDspObject::storePreset(size_t idx)
{
}

void UIDspObject::clearPreset(size_t idx)
{
    PresetStorage::instance().clearValueAt(presetId(), idx);
}

void UIDspObject::bangTo(size_t n)
{
    if (n >= outlets_.size())
        return;

    outlet_bang(outlets_[n]);
}

void UIDspObject::floatTo(size_t n, t_float f)
{
    if (n >= outlets_.size())
        return;

    outlet_float(outlets_[n], f);
}

void UIDspObject::symbolTo(size_t n, t_symbol* s)
{
    if (n >= outlets_.size())
        return;

    outlet_symbol(outlets_[n], s);
}

void UIDspObject::listTo(size_t n, const AtomList& lst)
{
    if (n >= outlets_.size())
        return;

    outlet_list(outlets_[n], &s_list, lst.size(), lst.toPdData());
}

void UIDspObject::anyTo(size_t n, t_symbol* s, const AtomList& args)
{
    if (n >= outlets_.size())
        return;

    outlet_anything(outlets_[n], s, args.size(), args.toPdData());
}

void UIDspObject::anyTo(size_t n, const AtomList& msg)
{
    if (n >= outlets_.size())
        return;

    if (msg.empty())
        return;

    if (!msg[0].isSymbol())
        return;

    outlet_anything(outlets_[n], msg[0].asSymbol(), msg.size() - 1, msg.toPdData() + 1);
}

void UIDspObject::dataTo(size_t n, const DataPtr& ptr)
{
    if (n >= outlets_.size())
        return;

    if (ptr.isNull())
        return;

    ptr.asAtom().output(outlets_[n]);
}

void UIDspObject::send(t_float f)
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_float(send, f);
}

void UIDspObject::send(t_symbol* s)
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_symbol(send, s);
}

void UIDspObject::send(const AtomList& lst)
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_list(send, &s_list, lst.size(), lst.toPdData());
}

void UIDspObject::send(t_symbol* s, const AtomList& lst)
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_typedmess(send, s, lst.size(), lst.toPdData());
}

const t_rect& UIDspObject::rect() const { return asEBox()->b_rect; }

float UIDspObject::x() const { return asEBox()->b_rect.x; }

float UIDspObject::y() const { return asEBox()->b_rect.y; }

float UIDspObject::width() const { return asEBox()->b_rect.width; }

float UIDspObject::height() const { return asEBox()->b_rect.height; }

float UIDspObject::zoom() const
{
    return ebox_getzoom(asEBox());
}

t_cursor UIDspObject::cursor() const
{
    return cursor_;
}

void UIDspObject::setCursor(t_cursor c)
{
    if (cursor_ != c) {
        ebox_set_cursor(asEBox(), c);
        cursor_ = c;
    }
}

void UIDspObject::presetInit()
{
    old_preset_id_ = s_null;
    if ((!presetId() || presetId() == s_null) && !isPatchLoading()) {
        t_symbol* name = genPresetName(name_);
        b_objpreset_id = name;
        bindPreset(b_objpreset_id);
    } else if (isPatchEdited() && !isPatchLoading()) {
        PresetNameMap::iterator it = presets_.find(b_objpreset_id);
        if (it != presets_.end() && it->second > 1) {
            t_symbol* name = genPresetName(name_);
            rebindPreset(b_objpreset_id, name);
            b_objpreset_id = name;
        }
    }

    // sync
    old_preset_id_ = presetId();
}

void UIDspObject::bindPreset(t_symbol* name)
{
    if (!name || name == s_null)
        return;

#ifdef CEAMMC_PRESET_DEBUG
    post("bind preset: %s", name->s_name);
#endif

    pd_bind(asPd(), gensym(Preset::SYM_PRESET_ALL));
    PresetStorage::instance().bindPreset(name);
    acquirePresetName(name);
}

void UIDspObject::unbindPreset(t_symbol* name)
{
    if (!name || name == s_null)
        return;

#ifdef CEAMMC_PRESET_DEBUG
    post("unbind preset: %s", name->s_name);
#endif

    pd_unbind(asPd(), gensym(Preset::SYM_PRESET_ALL));
    PresetStorage::instance().unbindPreset(name);
    releasePresetName(name);
}

void UIDspObject::rebindPreset(t_symbol* from, t_symbol* to)
{
    if (from == to)
        return;

    if (!from || !to)
        return;

#ifdef CEAMMC_PRESET_DEBUG
    post("rebind preset: %s -> %s", from->s_name, to->s_name);
#endif

    ceammc::PresetStorage::instance().unbindPreset(from);
    releasePresetName(from);
    ceammc::PresetStorage::instance().bindPreset(to);
    acquirePresetName(to);
}

void UIDspObject::handlePresetNameChange()
{
    if (old_preset_id_ != presetId()) {
        if (old_preset_id_ == s_null)
            bindPreset(presetId());
        else if (presetId() == s_null)
            unbindPreset(old_preset_id_);
        else
            rebindPreset(old_preset_id_, presetId());

        // sync
        old_preset_id_ = b_objpreset_id;
    }
}

size_t UIDspObject::numInlets() const
{
    return obj_ninlets(asPdObject());
}

size_t UIDspObject::numOutlets() const
{
    return outlets_.size();
}

bool UIDspObject::hasProperty(t_symbol* name) const
{
    t_eclass* c = (t_eclass*)b_obj.o_obj.te_g.g_pd;

    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == name)
            return true;
    }

    return false;
}

bool UIDspObject::getProperty(t_symbol* name, t_float& f) const
{
    int argc = 0;
    t_atom* argv = 0;
    eclass_attr_getter(asPdObject(), name, &argc, &argv);

    if (argc && argv) {
        if (argc != 1) {
            freebytes(argv, argc);
            return false;
        }

        if (atom_gettype(argv) != A_FLOAT) {
            freebytes(argv, argc);
            return false;
        }

        f = atom_getfloat(argv);
        freebytes(argv, argc);
        return true;
    }

    return false;
}

bool UIDspObject::getProperty(t_symbol* name, AtomList& lst) const
{
    int argc = 0;
    t_atom* argv = 0;
    eclass_attr_getter(asPdObject(), name, &argc, &argv);

    if (argc && argv) {
        lst = AtomList(argc, argv);
        freebytes(argv, argc);
        return true;
    }

    return false;
}

void UIDspObject::setProperty(t_symbol* name, const AtomList& lst)
{
    eclass_attr_setter(asPdObject(), name, lst.size(), lst.toPdData());
}

void UIDspObject::bindTo(t_symbol* s)
{
    if (binded_signals_.find(s) == binded_signals_.end()) {
        pd_bind(asPd(), s);
        binded_signals_.insert(s);
    }
}

void UIDspObject::unbindFrom(t_symbol* s)
{
    if (binded_signals_.find(s) != binded_signals_.end()) {
        pd_unbind(asPd(), s);
        binded_signals_.erase(s);
    }
}

void UIDspObject::unbindAll()
{
    BOOST_FOREACH (t_symbol* s, binded_signals_) {
        pd_unbind(asPd(), s);
    }
}

void UIDspObject::dspInit()
{
}

void UIDspObject::dspSetup(size_t n_in, size_t n_out)
{
    std::vector<t_outlet*> outs(n_out, 0);

    eobj_dspsetup(asEBox(), n_in, n_out, 0, outs.data());
    std::copy(outs.begin(), outs.end(), std::back_inserter(outlets_));
}

void UIDspObject::dspOn(double samplerate, long blocksize)
{
    samplerate_ = samplerate;
    blocksize_ = blocksize;
}

void UIDspObject::dspProcess(t_sample** ins, long n_ins,
    t_sample** outs, long n_outs,
    long sampleframes)
{
}

float UIDspObject::fontSize() const
{
    return ebox_getfontsize(asEBox());
}

float UIDspObject::fontSizeZoomed() const
{
    return ebox_getzoomfontsize(asEBox());
}

t_symbol* UIDspObject::genPresetName(t_symbol* prefix)
{
    char buf[64];

    for (int i = 0; i < 1024; i++) {
        snprintf(buf, 60, "%s.%d", prefix->s_name, i);
        t_symbol* s = gensym(buf);
        if (presets_.find(s) == presets_.end()) {
            presets_[s] = 0;
            return s;
        }
    }

    return s_null;
}

void UIDspObject::releasePresetName(t_symbol* s)
{
    PresetNameMap::iterator it = presets_.find(s);
    if (it != presets_.end()) {
        if (it->second > 1) {
            it->second--;

#ifdef CEAMMC_PRESET_DEBUG
            post("preset release: %s +%d", s->s_name, it->second);
#endif

        } else {
            presets_.erase(it);

#ifdef CEAMMC_PRESET_DEBUG
            post("preset release: %s erase...", s->s_name);
#endif
        }
    }
}

void UIDspObject::acquirePresetName(t_symbol* s)
{
    presets_[s] += 1;

#ifdef CEAMMC_PRESET_DEBUG
    post("preset acquire: %s +%d", s->s_name, presets_[s]);
#endif
}
