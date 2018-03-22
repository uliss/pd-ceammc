#include "ceammc_ui_object.h"
#include "ceammc_log.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

#include <boost/foreach.hpp>

namespace ceammc {

t_symbol* UIObject::BG_LAYER = gensym("background_layer");

#ifdef __APPLE__
t_symbol* UIObject::FONT_FAMILY = gensym("Helvetica");
const int UIObject::FONT_SIZE = 12;
const int UIObject::FONT_SIZE_SMALL = 8;
#elif _WIN32
t_symbol* UIObject::FONT_FAMILY = gensym("DejaVu Sans Mono");
const int UIObject::FONT_SIZE = 9;
const int UIObject::FONT_SIZE_SMALL = 6;
#else
t_symbol* UIObject::FONT_FAMILY = gensym("DejaVu Sans Mono");
const int UIObject::FONT_SIZE = 9;
const int UIObject::FONT_SIZE_SMALL = 6;
#endif

t_symbol* UIObject::FONT_STYLE = gensym("roman");
t_symbol* UIObject::FONT_WEIGHT = gensym("normal");
t_symbol* UIObject::COLOR_ACTIVE = gensym("#00C0FF");

UIObject::PresetNameMap UIObject::presets_;

UIObject::UIObject()
    : name_(&s_)
    , bg_layer_(asEBox(), BG_LAYER)
    , old_preset_id_(s_null)
    , cursor_(ECURSOR_LEFT_PTR)
    , use_presets_(false)
    , prop_color_background(rgba_white)
    , prop_color_border(rgba_black)
{
}

UIObject::~UIObject()
{
    for (size_t i = 0; i < outlets_.size(); i++)
        outlet_free(outlets_[i]);

    unbindPreset(presetId());
    unbindAll();
}

t_ebox* UIObject::asEBox() const { return const_cast<UIObject*>(this); }

t_object* UIObject::asPdObject() const
{
    return &(asEBox()->b_obj.o_obj);
}

t_pd* UIObject::asPd() const
{
    return &asPdObject()->te_g.g_pd;
}

t_outlet* UIObject::createOutlet()
{
    t_outlet* out = outlet_new(asPdObject(), &s_list);
    outlets_.push_back(out);
    return out;
}

t_canvas* UIObject::canvas() const
{
    return eobj_getcanvas((void*)this);
}

bool UIObject::isPatchLoading() const
{
    t_canvas* c = canvas();
    return c ? c->gl_loading : false;
}

bool UIObject::isPatchEdited() const
{
    t_canvas* c = canvas();
    return c ? c->gl_edit : false;
}

void UIObject::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    name_ = name;
    args_ = args;
    use_presets_ = usePresets;

    if (use_presets_)
        presetInit();
}

t_symbol* UIObject::name() const
{
    return name_;
}

t_symbol* UIObject::presetId()
{
    return b_objpreset_id;
}

void UIObject::paint(t_object* view)
{
}

void UIObject::redraw()
{
    ebox_redraw(asEBox());
}

void UIObject::redrawBGLayer()
{
    bg_layer_.invalidate();
    redraw();
}

void UIObject::updateSize()
{
    ebox_notify(asEBox(), s_size, &s_, NULL, NULL);
}

void UIObject::resize(int w, int h)
{
    b_rect.width = w;
    b_rect.height = h;
    updateSize();
}

void UIObject::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObject::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObject::onMouseDown(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObject::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObject::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObject::onMouseEnter(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObject::onMouseWheel(t_object* view, const t_pt& pt, long modifiers, double delta)
{
}

void UIObject::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
}

t_pd_err UIObject::notify(t_symbol* /*attr_name*/, t_symbol* msg)
{
    if (msg == s_attr_modified) {
        redrawBGLayer();
    }

    return 0;
}

void UIObject::okSize(t_rect* newrect)
{
}

void UIObject::setDrawParams(t_object*, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 1;
    params->d_bordercolor = prop_color_border;
    params->d_boxfillcolor = prop_color_background;
}

void UIObject::m_custom(t_symbol* sel, const AtomList& lst)
{
}

void UIObject::onBang()
{
}

void UIObject::onFloat(t_float f)
{
}

void UIObject::onSymbol(t_symbol* s)
{
}

void UIObject::onList(const AtomList& lst)
{
}

void UIObject::onAny(t_symbol* s, const AtomList& lst)
{
    LIB_ERR << "unknown message: " << s->s_name;
}

void UIObject::onKey(int k, long modifiers)
{
}

void UIObject::onKeyFilter(int k, long modifiers)
{
}

void UIObject::onData(const DataPtr& ptr)
{
}

void UIObject::onProperty(t_symbol* s, const AtomList& lst)
{
}

void UIObject::loadPreset(size_t idx)
{
}

void UIObject::storePreset(size_t idx)
{
}

void UIObject::clearPreset(size_t idx)
{
    PresetStorage::instance().clearValueAt(presetId(), idx);
}

void UIObject::bangTo(size_t n)
{
    if (n >= outlets_.size())
        return;

    outlet_bang(outlets_[n]);
}

void UIObject::floatTo(size_t n, t_float f)
{
    if (n >= outlets_.size())
        return;

    outlet_float(outlets_[n], f);
}

void UIObject::symbolTo(size_t n, t_symbol* s)
{
    if (n >= outlets_.size())
        return;

    outlet_symbol(outlets_[n], s);
}

void UIObject::listTo(size_t n, const AtomList& lst)
{
    if (n >= outlets_.size())
        return;

    outlet_list(outlets_[n], &s_list, lst.size(), lst.toPdData());
}

void UIObject::anyTo(size_t n, t_symbol* s, const AtomList& args)
{
    if (n >= outlets_.size())
        return;

    outlet_anything(outlets_[n], s, args.size(), args.toPdData());
}

void UIObject::anyTo(size_t n, const AtomList& msg)
{
    if (n >= outlets_.size())
        return;

    if (msg.empty())
        return;

    if (!msg[0].isSymbol())
        return;

    outlet_anything(outlets_[n], msg[0].asSymbol(), msg.size() - 1, msg.toPdData() + 1);
}

void UIObject::dataTo(size_t n, const DataPtr& ptr)
{
    if (n >= outlets_.size())
        return;

    if (ptr.isNull())
        return;

    ptr.asAtom().output(outlets_[n]);
}

void UIObject::sendBang()
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_bang(send);
}

void UIObject::send(t_float f)
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_float(send, f);
}

void UIObject::send(t_symbol* s)
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_symbol(send, s);
}

void UIObject::send(const AtomList& lst)
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_list(send, &s_list, lst.size(), lst.toPdData());
}

void UIObject::send(t_symbol* s, const AtomList& lst)
{
    t_pd* send = ebox_getsender(asEBox());
    if (send)
        pd_typedmess(send, s, lst.size(), lst.toPdData());
}

const t_rect& UIObject::rect() const { return asEBox()->b_rect; }

float UIObject::x() const { return asEBox()->b_rect.x; }

float UIObject::y() const { return asEBox()->b_rect.y; }

float UIObject::width() const { return asEBox()->b_rect.width; }

float UIObject::height() const { return asEBox()->b_rect.height; }

float UIObject::zoom() const
{
    return ebox_getzoom(asEBox());
}

t_cursor UIObject::cursor() const
{
    return cursor_;
}

void UIObject::setCursor(t_cursor c)
{
    if (cursor_ != c) {
        ebox_set_cursor(asEBox(), c);
        cursor_ = c;
    }
}

void UIObject::presetInit()
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

void UIObject::bindPreset(t_symbol* name)
{
    if (!name || name == s_null)
        return;

#ifdef CEAMMC_PRESET_DEBUG
    post("bind preset: %s", name->s_name);
#endif

    pd_bind(asPd(), Preset::SYM_PRESET_ALL);
    PresetStorage::instance().bindPreset(name);
    acquirePresetName(name);
}

void UIObject::unbindPreset(t_symbol* name)
{
    if (!name || name == s_null)
        return;

#ifdef CEAMMC_PRESET_DEBUG
    post("unbind preset: %s", name->s_name);
#endif

    pd_unbind(asPd(), Preset::SYM_PRESET_ALL);
    PresetStorage::instance().unbindPreset(name);
    releasePresetName(name);
}

void UIObject::rebindPreset(t_symbol* from, t_symbol* to)
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

void UIObject::handlePresetNameChange()
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

size_t UIObject::numInlets() const
{
    return obj_ninlets(asPdObject());
}

size_t UIObject::numOutlets() const
{
    return outlets_.size();
}

bool UIObject::hasProperty(t_symbol* name) const
{
    t_eclass* c = (t_eclass*)b_obj.o_obj.te_g.g_pd;

    for (int i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == name)
            return true;
    }

    return false;
}

bool UIObject::getProperty(t_symbol* name, t_float& f) const
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

bool UIObject::getProperty(t_symbol* name, AtomList& lst) const
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

void UIObject::setProperty(t_symbol* name, const AtomList& lst)
{
    eclass_attr_setter(asPdObject(), name, lst.size(), lst.toPdData());
}

void UIObject::bindTo(t_symbol* s)
{
    if (binded_signals_.find(s) == binded_signals_.end()) {
        pd_bind(asPd(), s);
        binded_signals_.insert(s);
    }
}

void UIObject::unbindFrom(t_symbol* s)
{
    if (binded_signals_.find(s) != binded_signals_.end()) {
        pd_unbind(asPd(), s);
        binded_signals_.erase(s);
    }
}

void UIObject::unbindAll()
{
    BOOST_FOREACH (t_symbol* s, binded_signals_) {
        pd_unbind(asPd(), s);
    }
}

float UIObject::fontSize() const
{
    return ebox_getfontsize(asEBox());
}

float UIObject::fontSizeZoomed() const
{
    return ebox_getzoomfontsize(asEBox());
}

t_symbol* UIObject::genPresetName(t_symbol* prefix)
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

void UIObject::releasePresetName(t_symbol* s)
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

void UIObject::acquirePresetName(t_symbol* s)
{
    presets_[s] += 1;

#ifdef CEAMMC_PRESET_DEBUG
    post("preset aquire: %s +%d", s->s_name, presets_[s]);
#endif
}

UIError::UIError(const UIObject* obj)
    : obj_(obj)
{
}

static void pdDebug(const char* name, const std::string& s)
{
    // strlen '[%s] '
    const size_t N = MAXPDSTRING - (strlen(name) + 10);

    if (s.size() < N) {
        post("[%s] %s", name, s.c_str());
    } else {
        for (size_t i = 0; i < s.size(); i += N) {
            post("[%s] %s", name,
                s.substr(i, std::min<size_t>(N, s.size() - i)).c_str());
        }
    }
}

static void pdError(void* obj, const char* name, const std::string& s)
{
    // strlen '[%s] '
    const size_t N = MAXPDSTRING - (strlen(name) + 10);

    if (s.size() < N) {
        pd_error(obj, "[%s] %s", name, s.c_str());
    } else {
        for (size_t i = 0; i < s.size(); i += N) {
            pd_error(obj, "[%s] %s", name,
                s.substr(i, std::min<size_t>(N, s.size() - i)).c_str());
        }
    }
}

UIError::~UIError()
{
    if (obj_ != 0)
        pdError(obj_->asPd(), obj_->name()->s_name, str());
    else
        pdError(0, "ceammc", str());
}

UIDebug::UIDebug(const UIObject* obj)
    : obj_(obj)
{
}

UIDebug::~UIDebug()
{
    if (obj_ != 0)
        pdDebug(obj_->name()->s_name, str());
    else
        pdDebug("ceammc", str());
}
}
