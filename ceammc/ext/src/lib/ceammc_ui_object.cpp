#include "ceammc_ui_object.h"
#include "ceammc_log.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

namespace ceammc {

const char* UIObject::BG_LAYER = "background_layer";

#ifdef __APPLE__
const char* UIObject::FONT_FAMILY = "Helvetica";
const int UIObject::FONT_SIZE = 12;
const int UIObject::FONT_SIZE_SMALL = 8;
#elif _WIN32
const char* UIObject::FONT_FAMILY = "DejaVu Sans Mono";
const int UIObject::FONT_SIZE = 9;
const int UIObject::FONT_SIZE_SMALL = 6;
#else
const char* UIObject::FONT_FAMILY = "DejaVu Sans Mono";
const int UIObject::FONT_SIZE = 9;
const int UIObject::FONT_SIZE_SMALL = 6;
#endif

const char* UIObject::FONT_STYLE = "roman";
const char* UIObject::FONT_WEIGHT = "normal";
const char* UIObject::COLOR_ACTIVE = "#00C0FF";

UIObject::PresetNameMap UIObject::presets_;

UIObject::UIObject()
    : name_(&s_)
    , bg_layer_(asEBox(), gensym(BG_LAYER))
    , old_preset_id_(s_null)
    , use_presets_(false)
    , prop_color_background(rgba_white)
    , prop_color_border(rgba_black)
    , prop_color_label(rgba_black)
{
    appendToLayerList(&bg_layer_);
}

UIObject::~UIObject()
{
    for (size_t i = 0; i < outlets_.size(); i++)
        outlet_free(outlets_[i]);

    unbindPreset(presetId());
    unbindAll();
}

void UIObject::appendToLayerList(UILayer* l)
{
    layer_stack_.push_back(l);
}

void UIObject::prependToLayerList(UILayer* l)
{
    layer_stack_.insert(layer_stack_.begin(), l);
}

void UIObject::invalidateLayer(UILayer* l)
{
    bool found = false;
    for (UILayer* x : layer_stack_) {
        if (x == l)
            found = true;

        if (found)
            x->invalidate();
    }
}

void UIObject::invalidateBox()
{
    ebox_invalidate_layer(asEBox(), s_eboxbd);
    ebox_invalidate_layer(asEBox(), s_eboxio);
}

t_ebox* UIObject::asEBox() const { return const_cast<UIObject*>(this); }

t_eobj* UIObject::asEObj() const
{
    return &const_cast<UIObject*>(this)->b_obj;
}

t_object* UIObject::asPdObject() const
{
    return &(asEBox()->b_obj.o_obj);
}

t_gobj* UIObject::asGObj() const
{
    return &(asEBox()->b_obj.o_obj.te_g);
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

bool UIObject::isVisible() const
{
    return asEBox() && ebox_isvisible(asEBox());
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

void UIObject::paint()
{
}

void UIObject::create()
{
}

void UIObject::erase()
{
}

void UIObject::redraw()
{
    ebox_redraw(asEBox());
}

void UIObject::redrawInnerArea()
{
    ebox_redraw_inner(asEBox());
}

void UIObject::redrawBGLayer()
{
    bg_layer_.invalidate();
    redraw();
}

void UIObject::updateSize()
{
    if (asEBox())
        ebox_notify(asEBox(), s_size);
}

void UIObject::resize(int w, int h)
{
    b_rect.width = w;
    b_rect.height = h;
    updateSize();
}

void UIObject::redrawLayer(UILayer& l)
{
    invalidateLayer(&l);
    redrawInnerArea();
}

void UIObject::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObject::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObject::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
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

void UIObject::okSize(t_rect* newrect)
{
}

void UIObject::setDrawParams(t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_bordercolor = prop_color_border;
    params->d_boxfillcolor = prop_color_background;
    params->d_labelcolor = prop_color_label;
}

void UIObject::onZoom(t_float z)
{
}

void UIObject::onPopup(t_symbol* menu_name, long item_idx)
{
}

void UIObject::onPropChange(t_symbol* name)
{
    redrawLayer(bg_layer_);
}

void UIObject::write(const std::string& fname)
{
}

void UIObject::read(const std::string& fname)
{
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

t_rect UIObject::rect() const
{
    auto z = asEBox()->b_zoom;
    auto r = asEBox()->b_rect;
    r.width *= z;
    r.height *= z;
    return r;
}

float UIObject::x() const { return asEBox()->b_rect.x; }

float UIObject::y() const { return asEBox()->b_rect.y; }

float UIObject::width() const { return asEBox()->b_rect.width * asEBox()->b_zoom; }

float UIObject::height() const { return asEBox()->b_rect.height * asEBox()->b_zoom; }

float UIObject::zoom() const
{
    return ebox_getzoom(asEBox());
}

void UIObject::setCursor(t_cursor c)
{
    ebox_set_cursor(asEBox(), c);
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

    pd_bind(asPd(), gensym(Preset::SYM_PRESET_ALL));
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

    pd_unbind(asPd(), gensym(Preset::SYM_PRESET_ALL));
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

static AtomList sym_to_list(t_symbol* sym)
{
    t_binbuf* bb = binbuf_new();
    binbuf_text(bb, sym->s_name, strlen(sym->s_name));
    int argc = binbuf_getnatom(bb);
    t_atom* argv = binbuf_getvec(bb);
    AtomList res(argc, argv);
    binbuf_free(bb);

    return res;
}

static void set_constrains(PropertyInfo& info, t_eattr* a)
{
    if (a->step)
        info.setStep(a->step);

    if (a->clipped & 0x1)
        info.setMin(a->minimum);

    if (a->clipped & 0x2)
        info.setMax(a->maximum);

    if (a->itemssize > 0) {
        info.setView(PropertyInfoView::MENU);

        for (int i = 0; i < a->itemssize; i++)
            info.addEnum(a->itemslist[i]);
    }
}

static PropertyInfo attr_to_prop(t_eattr* a)
{
    static t_symbol* SYM_CHECKBOX = gensym("checkbutton");
    static t_symbol* SYM_FLOAT = &s_float;
    static t_symbol* SYM_DOUBLE = gensym("double");
    static t_symbol* SYM_INT = gensym("int");
    static t_symbol* SYM_LONG = gensym("long");
    static t_symbol* SYM_SYMBOL = &s_symbol;
    static t_symbol* SYM_COLOR = gensym("color");
    static t_symbol* SYM_ATOM = gensym("atom");

    PropertyInfo res(std::string("@") + a->name->s_name, PropertyInfoType::VARIANT);

    if (a->type == SYM_FLOAT || a->type == SYM_DOUBLE) {
        if (a->size == 1) {
            res.setType(PropertyInfoType::FLOAT);
            set_constrains(res, a);

            if (a->defvals)
                res.setDefault((float)strtof(a->defvals->s_name, NULL));

        } else if (a->size > 1) {
            res.setType(PropertyInfoType::LIST);

            if (a->style == SYM_COLOR)
                res.setView(PropertyInfoView::COLOR);

            if (a->defvals)
                res.setDefault(sym_to_list(a->defvals));

        } else {
            std::cerr << "invalid float property size: " << a->size << "\n";
        }
    } else if (a->type == SYM_INT || a->type == SYM_LONG) {
        if (a->size == 1) {
            if (a->style == SYM_CHECKBOX) {
                res.setType(PropertyInfoType::BOOLEAN);
                res.setView(PropertyInfoView::TOGGLE);
                res.setRange(0, 1);

                if (a->defvals)
                    res.setDefault(a->defvals->s_name[0] == '1');

            } else {
                res.setType(PropertyInfoType::INTEGER);
                set_constrains(res, a);

                if (a->defvals)
                    res.setDefault((int)strtol(a->defvals->s_name, NULL, 10));
            }
        } else if (a->size > 1) {
            res.setType(PropertyInfoType::LIST);
            if (a->defvals)
                res.setDefault(sym_to_list(a->defvals));
        } else {
            std::cerr << "invalid int property size: " << a->size << "\n";
        }
    } else if (a->type == SYM_SYMBOL) {
        if (a->size == 1) {
            res.setType(PropertyInfoType::SYMBOL);
            set_constrains(res, a);

            if (a->defvals)
                res.setDefault(a->defvals);
        } else if (a->size > 1) {
            res.setType(PropertyInfoType::LIST);
            if (a->defvals)
                res.setDefault(sym_to_list(a->defvals));
        } else {
            std::cerr << "invalid int property size: " << a->size << "\n";
        }
    } else if (a->type == SYM_ATOM) {
        if (a->size == 1) {
            res.setType(PropertyInfoType::VARIANT);
            set_constrains(res, a);

            if (a->defvals)
                res.setDefault(Atom(a->defvals));
        } else if (a->size > 1) {
            res.setType(PropertyInfoType::LIST);
            if (a->defvals)
                res.setDefault(sym_to_list(a->defvals));
        } else {
            std::cerr << "invalid atom property size: " << a->size << "\n";
        }
    }

    if (a->getter != 0)
        res.setReadonly(a->setter == 0);

    return res;
}

std::vector<PropertyInfo> UIObject::propsInfo() const
{
    const t_eclass* c = reinterpret_cast<const t_eclass*>(asEBox()->b_obj.o_obj.te_g.g_pd);

    std::vector<PropertyInfo> res;
    res.reserve(c->c_nattr);

    for (int i = 0; i < c->c_nattr; i++)
        res.push_back(attr_to_prop(c->c_attr[i]));

    return res;
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
    for (t_symbol* s : binded_signals_) {
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
