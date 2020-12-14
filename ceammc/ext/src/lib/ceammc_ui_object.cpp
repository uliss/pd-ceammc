#include "ceammc_ui_object.h"
#include "ceammc_log.h"
#include "ceammc_output.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

#include <algorithm>

namespace ceammc {

const char* UIObjectImpl::BG_LAYER = "background_layer";

#ifdef __APPLE__
const char* UIObjectImpl::FONT_FAMILY = "Helvetica";
const int UIObjectImpl::FONT_SIZE = 12;
const int UIObjectImpl::FONT_SIZE_SMALL = 8;
#elif _WIN32
const char* UIObjectImpl::FONT_FAMILY = "DejaVu Sans Mono";
const int UIObjectImpl::FONT_SIZE = 9;
const int UIObjectImpl::FONT_SIZE_SMALL = 6;
#else
const char* UIObjectImpl::FONT_FAMILY = "DejaVu Sans Mono";
const int UIObjectImpl::FONT_SIZE = 9;
const int UIObjectImpl::FONT_SIZE_SMALL = 6;
#endif

const char* UIObjectImpl::FONT_STYLE = "roman";
const char* UIObjectImpl::FONT_WEIGHT = "normal";
const char* UIObjectImpl::COLOR_ACTIVE = "#00C0FF";

UIObjectImpl::PresetNameMap UIObjectImpl::presets_;

UIObjectImpl::UIObjectImpl(t_ebox* x)
    : box_(x)
    , name_(&s_)
    , bg_layer_(box_, gensym(BG_LAYER))
    , old_preset_id_(s_null)
    , use_presets_(false)
    , prop_color_background(rgba_white)
    , prop_color_border(rgba_black)
    , prop_color_label(rgba_black)
    , prop_mouse_events(0)
{
    appendToLayerList(&bg_layer_);
}

UIObjectImpl::~UIObjectImpl()
{
    for (size_t i = 0; i < outlets_.size(); i++)
        outlet_free(outlets_[i]);

    unbindPreset(presetId());
    unbindAll();
}

void UIObjectImpl::appendToLayerList(UILayer* l)
{
    layer_stack_.push_back(l);
}

void UIObjectImpl::prependToLayerList(UILayer* l)
{
    layer_stack_.insert(layer_stack_.begin(), l);
}

void UIObjectImpl::invalidateLayer(UILayer* l)
{
    bool found = false;
    for (UILayer* x : layer_stack_) {
        if (x == l)
            found = true;

        if (found)
            x->invalidate();
    }
}

void UIObjectImpl::invalidateBox()
{
    ebox_invalidate_border(box_);
    ebox_invalidate_io(box_);
}

void UIObjectImpl::invalidateXlets()
{
    ebox_invalidate_io(box_);
}

void UIObjectImpl::invalidateBorder()
{
    ebox_invalidate_border(box_);
}

void UIObjectImpl::initPopupMenu(const std::string& n, std::initializer_list<PopupMenuCallbacks::Entry> args)
{
    auto it = std::find_if(popup_menu_list_.begin(), popup_menu_list_.end(), [&n](const PopupMenuCallbacks& c) { return c.name() == n; });
    if (it != popup_menu_list_.end()) {
        UI_ERR << "menu already exists: " << n;
        return;
    }

    popup_menu_list_.emplace_back(n, args);
}

void UIObjectImpl::showDefaultPopupMenu(const t_pt& pt, const t_pt& abs_pt)
{
    if (popup_menu_list_.empty()) {
        UI_ERR << "no default popup menu exists";
        return;
    }

    UIPopupMenu menu(asEObj(), popup_menu_list_.front(), abs_pt, pt);
}

void UIObjectImpl::showPopupMenu(const std::string& n, const t_pt& pt, const t_pt& abs_pt)
{
    auto it = std::find_if(popup_menu_list_.begin(), popup_menu_list_.end(), [&n](const PopupMenuCallbacks& c) { return c.name() == n; });
    if (it == popup_menu_list_.end()) {
        UI_ERR << "menu not found: " << n;
        return;
    }

    UIPopupMenu menu(asEObj(), *it, abs_pt, pt);
}

t_eobj* UIObjectImpl::asEObj() const
{
    return &(box_->b_obj);
}

t_object* UIObjectImpl::asPdObject() const
{
    return &(box_->b_obj.o_obj);
}

t_gobj* UIObjectImpl::asGObj() const
{
    return &(box_->b_obj.o_obj.te_g);
}

t_pd* UIObjectImpl::asPd() const
{
    return &(asPdObject()->te_g.g_pd);
}

t_outlet* UIObjectImpl::createOutlet()
{
    t_outlet* out = outlet_new(asPdObject(), &s_list);
    outlets_.push_back(out);
    return out;
}

const char* UIObjectImpl::annotateInlet(int n) const
{
    return nullptr;
}

const char* UIObjectImpl::annotateOutlet(int n) const
{
    return nullptr;
}

t_canvas* UIObjectImpl::canvas() const
{
    return asEObj()->o_canvas;
}

bool UIObjectImpl::isPatchLoading() const
{
    t_canvas* c = canvas();
    return c ? c->gl_loading : false;
}

bool UIObjectImpl::isPatchEdited() const
{
    t_canvas* c = canvas();
    return c ? c->gl_edit : false;
}

bool UIObjectImpl::isVisible() const
{
    return box_ && ebox_isvisible(box_);
}

void UIObjectImpl::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    name_ = name;
    args_ = args;
    use_presets_ = usePresets;

    if (use_presets_)
        presetInit();
}

t_symbol* UIObjectImpl::name() const
{
    return name_;
}

t_symbol* UIObjectImpl::presetId()
{
    return box_->b_objpreset_id;
}

void UIObjectImpl::paint()
{
}

void UIObjectImpl::create()
{
}

void UIObjectImpl::erase()
{
}

void UIObjectImpl::redraw()
{
    ebox_redraw(box_);
}

void UIObjectImpl::redrawInnerArea()
{
    ebox_redraw_inner(box_);
}

void UIObjectImpl::redrawBGLayer()
{
    bg_layer_.invalidate();
    redraw();
}

void UIObjectImpl::updateSize()
{
    if (box_)
        ebox_notify(box_, s_size);
}

void UIObjectImpl::resize(int w, int h)
{
    box_->b_rect.width = w;
    box_->b_rect.height = h;
    updateSize();
}

void UIObjectImpl::redrawLayer(UILayer& l)
{
    invalidateLayer(&l);
    redrawInnerArea();
}

void UIObjectImpl::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObjectImpl::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObjectImpl::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
}

void UIObjectImpl::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObjectImpl::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObjectImpl::onMouseEnter(t_object* view, const t_pt& pt, long modifiers)
{
}

void UIObjectImpl::onMouseWheel(const t_pt& pt, long modifiers, double delta)
{
}

void UIObjectImpl::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
}

bool UIObjectImpl::outputMouseEvents() const
{
    return prop_mouse_events;
}

void UIObjectImpl::okSize(t_rect* newrect)
{
}

void UIObjectImpl::setDrawParams(t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_bordercolor = prop_color_border;
    params->d_boxfillcolor = prop_color_background;
    params->d_labelcolor = prop_color_label;
}

void UIObjectImpl::onZoom(t_float z)
{
}

void UIObjectImpl::onPopup(t_symbol* menu_name, long item_idx, const t_pt& pt)
{
    auto it = std::find_if(popup_menu_list_.begin(), popup_menu_list_.end(), [menu_name](const PopupMenuCallbacks& cb) { return cb.name() == menu_name->s_name; });
    if (it == popup_menu_list_.end()) {
        UI_ERR << "popup menu not found: " << menu_name->s_name;
        return;
    }

    if (!it->process(menu_name, item_idx, pt)) {
        UI_ERR << "popup menu item processing error: " << item_idx;
        return;
    }
}

void UIObjectImpl::showPopup(const t_pt& pt, const t_pt& abs_pt)
{
    showDefaultPopupMenu(pt, abs_pt);
}

void UIObjectImpl::onPropChange(t_symbol* name)
{
    redrawLayer(bg_layer_);
}

void UIObjectImpl::write(const std::string& fname)
{
}

void UIObjectImpl::read(const std::string& fname)
{
}

void UIObjectImpl::m_custom(t_symbol* sel, const AtomList& lst)
{
}

void UIObjectImpl::onBang()
{
}

void UIObjectImpl::onFloat(t_float f)
{
}

void UIObjectImpl::onSymbol(t_symbol* s)
{
}

void UIObjectImpl::onList(const AtomList& lst)
{
}

void UIObjectImpl::onAny(t_symbol* s, const AtomListView& lst)
{
    LIB_ERR << "unknown message: " << s->s_name;
}

void UIObjectImpl::onKey(int k, long modifiers)
{
}

void UIObjectImpl::onKeyFilter(int k, long modifiers)
{
}

void UIObjectImpl::onData(const AbstractData* ptr)
{
}

void UIObjectImpl::onProperty(t_symbol* s, const AtomList& lst)
{
}

void UIObjectImpl::loadPreset(size_t idx)
{
}

void UIObjectImpl::storePreset(size_t idx)
{
}

void UIObjectImpl::clearPreset(size_t idx)
{
    PresetStorage::instance().clearValueAt(presetId(), idx);
}

void UIObjectImpl::bangTo(size_t n)
{
    if (n >= outlets_.size())
        return;

    outlet_bang(outlets_[n]);
}

void UIObjectImpl::floatTo(size_t n, t_float f)
{
    if (n >= outlets_.size())
        return;

    outlet_float(outlets_[n], f);
}

void UIObjectImpl::symbolTo(size_t n, t_symbol* s)
{
    if (n >= outlets_.size())
        return;

    outlet_symbol(outlets_[n], s);
}

void UIObjectImpl::atomTo(size_t n, const Atom& a)
{
    if (n >= outlets_.size())
        return;

    outlet_list(outlets_[n], &s_list, 1, const_cast<t_atom*>(&a.atom()));
}

void UIObjectImpl::listTo(size_t n, const AtomList& lst)
{
    if (n >= outlets_.size())
        return;

    outlet_list(outlets_[n], &s_list, lst.size(), lst.toPdData());
}

void UIObjectImpl::anyTo(size_t n, t_symbol* s, const AtomList& args)
{
    if (n >= outlets_.size())
        return;

    outlet_anything(outlets_[n], s, args.size(), args.toPdData());
}

void UIObjectImpl::anyTo(size_t n, const AtomList& msg)
{
    if (n >= outlets_.size())
        return;

    if (msg.empty())
        return;

    if (!msg[0].isSymbol())
        return;

    outlet_anything(outlets_[n], msg[0].asSymbol(), msg.size() - 1, msg.toPdData() + 1);
}

void UIObjectImpl::sendBang()
{
    t_pd* send = ebox_getsender(box_);
    if (send)
        pd_bang(send);
}

void UIObjectImpl::send(t_float f)
{
    t_pd* send = ebox_getsender(box_);
    if (send)
        pd_float(send, f);
}

void UIObjectImpl::send(t_symbol* s)
{
    t_pd* send = ebox_getsender(box_);
    if (send)
        pd_symbol(send, s);
}

void UIObjectImpl::send(const AtomList& lst)
{
    t_pd* send = ebox_getsender(box_);
    if (send)
        pd_list(send, &s_list, lst.size(), lst.toPdData());
}

void UIObjectImpl::send(t_symbol* s, const AtomList& lst)
{
    t_pd* send = ebox_getsender(box_);
    if (send)
        pd_typedmess(send, s, lst.size(), lst.toPdData());
}

t_rect UIObjectImpl::rect() const
{
    auto z = box_->b_zoom;
    auto r = box_->b_rect;
    r.width *= z;
    r.height *= z;
    return r;
}

float UIObjectImpl::x() const { return box_->b_rect.x; }

float UIObjectImpl::y() const { return box_->b_rect.y; }

float UIObjectImpl::width() const { return box_->b_rect.width * box_->b_zoom; }

float UIObjectImpl::height() const { return box_->b_rect.height * box_->b_zoom; }

float UIObjectImpl::zoom() const
{
    return ebox_getzoom(box_);
}

void UIObjectImpl::setCursor(t_cursor c)
{
    ebox_set_cursor(box_, c);
}

void UIObjectImpl::presetInit()
{
    old_preset_id_ = s_null;
    if ((!presetId() || presetId() == s_null) && !isPatchLoading()) {
        t_symbol* name = genPresetName(name_);
        box_->b_objpreset_id = name;
        bindPreset(box_->b_objpreset_id);
    } else if (isPatchEdited() && !isPatchLoading()) {
        PresetNameMap::iterator it = presets_.find(box_->b_objpreset_id);
        if (it != presets_.end() && it->second > 1) {
            t_symbol* name = genPresetName(name_);
            rebindPreset(box_->b_objpreset_id, name);
            box_->b_objpreset_id = name;
        }
    }

    // sync
    old_preset_id_ = presetId();
}

void UIObjectImpl::bindPreset(t_symbol* name)
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

void UIObjectImpl::unbindPreset(t_symbol* name)
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

void UIObjectImpl::rebindPreset(t_symbol* from, t_symbol* to)
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

void UIObjectImpl::handlePresetNameChange()
{
    if (old_preset_id_ != presetId()) {
        if (old_preset_id_ == s_null)
            bindPreset(presetId());
        else if (presetId() == s_null)
            unbindPreset(old_preset_id_);
        else
            rebindPreset(old_preset_id_, presetId());

        // sync
        old_preset_id_ = box_->b_objpreset_id;
    }
}

size_t UIObjectImpl::numInlets() const
{
    return obj_ninlets(asPdObject());
}

size_t UIObjectImpl::numOutlets() const
{
    return outlets_.size();
}

bool UIObjectImpl::hasProperty(t_symbol* name) const
{
    t_eclass* c = (t_eclass*)asPdObject()->te_g.g_pd;

    for (size_t i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->name == name)
            return true;
    }

    return false;
}

bool UIObjectImpl::getProperty(t_symbol* name, t_float& f) const
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

bool UIObjectImpl::getProperty(t_symbol* name, AtomList& lst) const
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

void UIObjectImpl::setProperty(t_symbol* name, const AtomList& lst)
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

static void set_constraints(PropertyInfo& info, t_eattr* a)
{
    if (a->step)
        info.setStep(a->step);

    if (a->clipped & E_CLIP_MIN) {
        if (info.isNumeric()) {
            info.setConstraints(PropValueConstraints::GREATER_EQUAL);

            if (info.isFloat()) {
                if (!info.setMinFloat(a->minimum))
                    LIB_ERR << "can't set min float";
            } else if (info.isInt())
                if (!info.setMinInt(a->minimum))
                    LIB_ERR << "can't set min int";
        }
    }

    if (a->clipped & E_CLIP_MAX) {
        if (info.isNumeric()) {
            if (info.hasConstraintsMin())
                info.setConstraints(PropValueConstraints::CLOSED_RANGE);
            else
                info.setConstraints(PropValueConstraints::LESS_EQUAL);

            if (info.isFloat()) {
                if (!info.setMaxFloat(a->maximum))
                    LIB_ERR << "can't set max float";
            } else if (!info.setMaxInt(a->maximum))
                LIB_ERR << "can't set max int";
        }
    }

    if (a->itemssize > 0) {
        info.setView(PropValueView::MENU);
        info.setConstraints(PropValueConstraints::ENUM);

        for (size_t i = 0; i < a->itemssize; i++) {
            if (!info.addEnum(a->itemslist[i]))
                LIB_ERR << "can't append enum";
        }
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
    static t_symbol* SYM_UNIT_DB = gensym("db");
    static t_symbol* SYM_UNIT_MSEC = gensym("msec");
    static t_symbol* SYM_UNIT_SEC = gensym("sec");
    static t_symbol* SYM_UNIT_SAMP = gensym("samp");
    static t_symbol* SYM_UNIT_DEG = gensym("deg");
    static t_symbol* SYM_UNIT_RAD = gensym("rad");
    static t_symbol* SYM_UNIT_HZ = gensym("hz");

    PropertyInfo res(std::string("@") + a->name->s_name, PropValueType::ATOM);

    if (a->type == SYM_FLOAT || a->type == SYM_DOUBLE) {
        if (a->size == 1) {
            res.setType(PropValueType::FLOAT);
            set_constraints(res, a);

            if (a->defvals)
                res.setDefault((t_float)strtod(a->defvals->s_name, NULL));
        } else if (a->size > 1) {
            res.setType(PropValueType::LIST);

            if (a->style == SYM_COLOR)
                res.setView(PropValueView::COLOR);

            if (a->defvals)
                res.setDefault(sym_to_list(a->defvals));

        } else {
            std::cerr << "invalid float property size: " << a->size << "\n";
        }
    } else if (a->type == SYM_INT || a->type == SYM_LONG) {
        if (a->size == 1) {
            if (a->style == SYM_CHECKBOX) {
                res.setType(PropValueType::BOOLEAN);
                res.setView(PropValueView::TOGGLE);

                if (a->defvals)
                    res.setDefault(a->defvals->s_name[0] == '1');

            } else {
                res.setType(PropValueType::INTEGER);
                set_constraints(res, a);

                if (a->defvals)
                    res.setDefault((int)strtol(a->defvals->s_name, NULL, 10));
            }
        } else if (a->size > 1) {
            res.setType(PropValueType::LIST);
            if (a->defvals)
                res.setDefault(sym_to_list(a->defvals));
        } else {
            std::cerr << "invalid int property size: " << a->size << "\n";
        }
    } else if (a->type == SYM_SYMBOL) {
        if (a->size == 1) {
            res.setType(PropValueType::SYMBOL);
            set_constraints(res, a);

            if (a->defvals)
                res.setDefault(a->defvals);
        } else if (a->size > 1) {
            res.setType(PropValueType::LIST);
            if (a->defvals)
                res.setDefault(sym_to_list(a->defvals));
        } else {
            std::cerr << "invalid int property size: " << a->size << "\n";
        }
    } else if (a->type == SYM_ATOM) {
        if (a->size == 1) {
            res.setType(PropValueType::ATOM);
            set_constraints(res, a);

            if (a->defvals)
                res.setDefault(Atom(a->defvals));
        } else if (a->size > 1) {
            res.setType(PropValueType::LIST);
            if (a->defvals)
                res.setDefault(sym_to_list(a->defvals));
        } else {
            std::cerr << "invalid atom property size: " << a->size << "\n";
        }
    }

    if (a->units != &s_) {
        if (a->units == SYM_UNIT_DB)
            res.setUnits(PropValueUnits::DB);
        else if (a->units == SYM_UNIT_MSEC)
            res.setUnits(PropValueUnits::MSEC);
        else if (a->units == SYM_UNIT_SEC)
            res.setUnits(PropValueUnits::SEC);
        else if (a->units == SYM_UNIT_SAMP)
            res.setUnits(PropValueUnits::SAMP);
        else if (a->units == SYM_UNIT_DEG)
            res.setUnits(PropValueUnits::DEG);
        else if (a->units == SYM_UNIT_RAD)
            res.setUnits(PropValueUnits::RAD);
        else if (a->units == SYM_UNIT_HZ)
            res.setUnits(PropValueUnits::HZ);
        else
            std::cerr << "unknown unit: " << a->units->s_name << "\n";
    }

    //
    if (a->getter != 0 && a->setter == 0)
        res.setAccess(PropValueAccess::READONLY);

    return res;
}

std::vector<PropertyInfo> UIObjectImpl::propsInfo() const
{
    const t_eclass* c = reinterpret_cast<const t_eclass*>(box_->b_obj.o_obj.te_g.g_pd);

    std::vector<PropertyInfo> res;
    res.reserve(c->c_nattr);

    for (size_t i = 0; i < c->c_nattr; i++)
        res.push_back(attr_to_prop(c->c_attr[i]));

    return res;
}

void UIObjectImpl::bindTo(t_symbol* s)
{
    if (binded_signals_.find(s) == binded_signals_.end()) {
        pd_bind(asPd(), s);
        binded_signals_.insert(s);
    }
}

void UIObjectImpl::unbindFrom(t_symbol* s)
{
    if (binded_signals_.find(s) != binded_signals_.end()) {
        pd_unbind(asPd(), s);
        binded_signals_.erase(s);
    }
}

void UIObjectImpl::unbindAll()
{
    for (t_symbol* s : binded_signals_) {
        pd_unbind(asPd(), s);
    }
}

float UIObjectImpl::fontSize() const
{
    return ebox_getfontsize(box_);
}

float UIObjectImpl::fontSizeZoomed() const
{
    return ebox_getzoomfontsize(box_);
}

t_symbol* UIObjectImpl::genPresetName(t_symbol* prefix)
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

void UIObjectImpl::releasePresetName(t_symbol* s)
{
    auto it = presets_.find(s);
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

void UIObjectImpl::acquirePresetName(t_symbol* s)
{
    presets_[s] += 1;

#ifdef CEAMMC_PRESET_DEBUG
    post("preset aquire: %s +%d", s->s_name, presets_[s]);
#endif
}

UIError::UIError(const UIObjectImpl* obj)
    : obj_(obj)
{
}

UIError::~UIError()
{
    pdError(obj_->asPd(), str());
}

UIDebug::UIDebug(const UIObjectImpl* obj)
    : obj_(obj)
{
}

UIDebug::~UIDebug()
{
    pdDebug(obj_->asPd(), str());
}

UIObject::UIObject()
    : UIObjectImpl(this)
{
}

UIDspObject::UIDspObject()
    : UIObjectImpl(&e_box)
    , samplerate_(44100)
    , blocksize_(0)
{
}

void UIDspObject::dspInit()
{
}

void UIDspObject::dspSetup(size_t n_in, size_t n_out)
{
    std::vector<t_outlet*> outs(n_out, 0);

    eobj_dspsetup(asEObj(), n_in, n_out, nullptr, outs.data());
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
}
