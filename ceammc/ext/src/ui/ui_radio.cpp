#include "ui_radio.h"
#include "ceammc_convert.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "ui_radio.tcl.h"

#include <cassert>
#include <chrono>
#include <random>

static const int MAX_ITEMS = 128;
static t_symbol* SYM_PROP_NITEMS;

void setup_ui_radio()
{
    UIRadio::setup();
}

UIRadio::UIRadio()
    : items_()
    , idx_(0)
    , prop_nitems_(8)
    , prop_checklist_mode_(0)
    , prop_color_active(hex_to_rgba(DEFAULT_ACTIVE_COLOR))
    , items_layer_(asEBox(), gensym("items_layer"))
{
    createOutlet();

    initPopupMenu("checklist",
        { { _("reset"), [this](const t_pt&) { if(prop_checklist_mode_) m_reset(); } },
            { _("flip"), [this](const t_pt&) { if(prop_checklist_mode_) m_flip(); } },
            { _("random"), [this](const t_pt&) { if(prop_checklist_mode_) m_random(); } } });
}

void UIRadio::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    static t_symbol* SYM_VRD = gensym("ui.vrd");
    static t_symbol* SYM_VRD_MULT = gensym("ui.vrd*");
    static t_symbol* SYM_HRD_MULT = gensym("ui.hrd*");
    static t_symbol* SYM_RADIO_MULT = gensym("ui.radio*");

    UIObject::init(name, args, usePresets);

    // check for vertical aliases and change orientation
    if (name == SYM_VRD || name == SYM_VRD_MULT)
        std::swap(asEBox()->b_rect.width, asEBox()->b_rect.height);

    // check checklist mode
    if (name == SYM_VRD_MULT || name == SYM_HRD_MULT || name == SYM_RADIO_MULT)
        setProperty(gensym("mode"), Atom(1));

    // has positional arguments
    int n = args.intAt(0, -1);
    if (n > 0) {
        prop_nitems_ = clip<int>(n, 2, MAX_ITEMS);
        const int dim1 = 15;
        const int dim2 = dim1 * prop_nitems_;
        if (isVertical()) {
            asEBox()->b_rect.width = dim1;
            asEBox()->b_rect.height = dim2;
        } else {
            asEBox()->b_rect.width = dim2;
            asEBox()->b_rect.height = dim1;
        }
    } else {
        const size_t N = args.size();
        for (size_t i = 0; i < N; i++) {
            const auto& a = args[i];
            if (!a.isProperty())
                continue;

            if (a.asSymbol() != SYM_PROP_NITEMS)
                continue;

            size_t inext = i + 1;
            if (inext >= N)
                break;

            prop_nitems_ = clip<int>(args[inext].asInt(0), 2, MAX_ITEMS);
            int h = 15;
            int w = h * prop_nitems_;
            if (isVertical())
                std::swap(h, w);

            asEBox()->b_rect.width = w;
            asEBox()->b_rect.height = h;
            break;
        }
    }
}

int UIRadio::singleValue() const
{
    return idx_;
}

void UIRadio::setSingleValue(int idx)
{
    idx_ = clip<int>(idx, 0, prop_nitems_ - 1);
}

void UIRadio::onBang()
{
    output();
}

void UIRadio::onFloat(t_float f)
{
    if (prop_checklist_mode_) {
        UI_ERR << "no floats expected on checklist mode: " << f;
        return;
    }

    setSingleValue(f);
    output();
    redrawItems();
}

void UIRadio::onList(const AtomListView& lv)
{
    if (!prop_checklist_mode_) {
        UI_ERR << "no lists expected in single mode: " << lv;
        return;
    }

    setListValue(lv);
    output();
    redrawItems();
}

int UIRadio::click2Cell(const t_pt& pt) const
{
    auto r = rect();
    return isVertical() ? (pt.y / r.height * prop_nitems_) : (pt.x / r.width * prop_nitems_);
}

void UIRadio::onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long mod)
{
    const int idx = click2Cell(pt);

    if (idx >= 0 && idx < prop_nitems_) {
        if (prop_checklist_mode_) {
            if (mod & EMOD_SHIFT)
                items_.set(idx, true);
            else if (mod & EMOD_ALT)
                items_.set(idx, false);
            else
                items_.flip(idx);
        } else {
            setSingleValue(idx);
            items_.reset();
            items_.set(idx, true);
        }

        output();
        redrawItems();
    }
}

void UIRadio::onMouseDrag(t_object* view, const t_pt& pt, long mod)
{
    if (!prop_checklist_mode_)
        return;

    const int idx = click2Cell(pt);

    if (0 <= idx && idx < prop_nitems_) {
        if (mod & EMOD_SHIFT)
            items_.set(idx, true);
        else if (mod & EMOD_ALT)
            items_.set(idx, false);
        else
            return;

        output();
        redrawItems();
    }
}

void UIRadio::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
    t_canvas* c = reinterpret_cast<t_canvas*>(view);
    if (c->gl_edit)
        resize(height() / zoom(), width() / zoom());
    else
        onMouseDown(view, pt, pt, modifiers);
}

t_float UIRadio::p_numItems() const
{
    return prop_nitems_;
}

void UIRadio::p_setNumItems(t_float n)
{
    int num = clip<int>(n, 2, MAX_ITEMS);
    if (num != prop_nitems_) {
        prop_nitems_ = num;
        items_.reset();
        updateSize();
    }
}

t_float UIRadio::p_mode() const
{
    return prop_checklist_mode_;
}

void UIRadio::p_setMode(t_float f)
{
    int mode = f ? 1 : 0;

    if (prop_checklist_mode_ != mode) {
        prop_checklist_mode_ = mode;
        idx_ = 0;
        items_.reset();
        redrawItems();
    }
}

AtomList UIRadio::p_value() const
{
    return prop_checklist_mode_ ? listValue() : Atom(singleValue());
}

void UIRadio::p_setValue(const AtomListView& lv)
{
    if (prop_checklist_mode_)
        setListValue(lv);
    else {
        if (lv.empty()) {
            UI_ERR << "radio index expected";
            return;
        }

        setSingleValue(lv[0].asFloat());
    }

    redrawItems();
}

void UIRadio::m_flip()
{
    if (!prop_checklist_mode_) {
        UI_ERR << "not in check list mode";
        return;
    }

    items_.flip();
    output();
    redrawItems();
}

void UIRadio::m_reset()
{
    if (!prop_checklist_mode_) {
        UI_ERR << "not in check list mode";
        return;
    }

    items_.reset();
    output();
    redrawItems();
}

void UIRadio::m_next()
{
    if (prop_checklist_mode_) {
        UI_ERR << "single check mode expected";
        return;
    }

    if (idx_ + 1 < prop_nitems_) {
        idx_++;
        output();
        redrawItems();
    }
}

void UIRadio::m_random()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);

    if (prop_checklist_mode_) {
        std::uniform_int_distribution<int> dist(0, 1);

        for (int i = 0; i < prop_nitems_; i++)
            items_.set(i, dist(gen));

        output();
        redrawItems();
    } else {
        std::uniform_int_distribution<int> dist(0, prop_nitems_ - 1);
        onFloat(dist(gen));
    }
}

void UIRadio::m_prev()
{
    if (prop_checklist_mode_) {
        UI_ERR << "single check mode expected";
        return;
    }

    if (idx_ > 0) {
        idx_--;
        output();
        redrawItems();
    }
}

void UIRadio::m_plus(t_float f)
{
    if (prop_checklist_mode_) {
        UI_ERR << "single check mode expected";
        return;
    }

    int v = idx_ + static_cast<int>(f);
    idx_ = (v >= 0) ? v % prop_nitems_ : prop_nitems_ - (abs(v) % prop_nitems_);
    output();
    redrawItems();
}

void UIRadio::m_minus(t_float f)
{
    if (prop_checklist_mode_) {
        UI_ERR << "single check mode expected";
        return;
    }

    int v = idx_ - int(f);
    idx_ = (v >= 0) ? v % prop_nitems_ : prop_nitems_ - (abs(v) % prop_nitems_);
    output();
    redrawItems();
}

void UIRadio::loadPreset(size_t idx)
{
    if (prop_checklist_mode_) {
        auto lv = PresetStorage::instance().listValueAt(presetId(), idx);
        onList(lv);
    } else {
        t_float f = PresetStorage::instance().floatValueAt(presetId(), idx);
        onFloat(f);
    }
}

void UIRadio::storePreset(size_t idx)
{
    if (prop_checklist_mode_)
        PresetStorage::instance().setListValueAt(presetId(), idx, listValue());
    else
        PresetStorage::instance().setFloatValueAt(presetId(), idx, singleValue());
}

void UIRadio::showPopup(const t_pt& pt, const t_pt& abs_pt)
{
    if (prop_checklist_mode_)
        showPopupMenu("checklist", pt, abs_pt);
}

AtomList UIRadio::listValue() const
{
    AtomList lst;

    lst.reserve(prop_nitems_);
    for (int i = 0; i < prop_nitems_; i++)
        lst.append(Atom(items_[i]));

    return lst;
}

void UIRadio::setListValue(const AtomListView& lv)
{
    size_t total = std::min<size_t>(lv.size(), prop_nitems_);
    for (size_t i = 0; i < total; i++)
        items_.set(i, lv[i].asFloat());
}

void UIRadio::output()
{
    if (!prop_checklist_mode_) {
        floatTo(0, idx_);
        send(idx_);
    } else {
        Atom res[prop_nitems_];
        for (int i = 0; i < prop_nitems_; i++)
            res[i] = Atom(items_[i]);

        auto lv = AtomListView(res, prop_nitems_);
        listTo(0, lv);
        send(lv);
    }
}

void UIRadio::paint()
{
    char states[prop_nitems_ + 1];
    for (int i = 0; i < prop_nitems_; i++) {
        if (prop_checklist_mode_)
            states[i] = items_[i] ? '1' : '0';
        else
            states[i] = (i == idx_) ? '1' : '0';
    }
    states[prop_nitems_] = '\0';

    sys_vgui("ui::radio_update %s %lx %d %d %d"
             " {%c} {%c} {%s}"
             " #%6.6x #%6.6x\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(), (int)zoom(),
        (isVertical() ? 'v' : 'h'), (prop_checklist_mode_ ? '*' : '.'), states,
        rgba_to_hex_int(prop_color_active), rgba_to_hex_int(prop_color_border));
}

void UIRadio::okSize(t_rect* newrect)
{
    assert(prop_nitems_ > 0);

    //    if (isPatchLoading()) {
    //        newrect->height = pd_clip_min(newrect->height, 8);
    //        newrect->width = pd_clip_min(newrect->width, 8);
    //    } else {
    if (isVertical()) {
        const int box_size = pd_clip_min(static_cast<int>(newrect->height / prop_nitems_), 8);
        newrect->height = prop_nitems_ * box_size;
        newrect->width = box_size;
    } else {
        const int box_size = pd_clip_min(static_cast<int>(newrect->width / prop_nitems_), 8);
        newrect->width = prop_nitems_ * box_size;
        newrect->height = box_size;
    }
    //    }
}

void UIRadio::redrawAll()
{
    bg_layer_.invalidate();
    items_layer_.invalidate();
    redraw();
}

void UIRadio::redrawItems()
{
    items_layer_.invalidate();
    redraw();
}

void UIRadio::onPropChange(t_symbol* prop_name)
{
    redrawAll();
}

void UIRadio::setup()
{
    sys_vgui(ui_radio_tcl);

    SYM_PROP_NITEMS = gensym("@nitems");

    UIObjectFactory<UIRadio> obj("ui.radio");
    obj.addAlias("ui.hrd");
    obj.addAlias("ui.vrd");
    obj.addAlias("ui.hrd*");
    obj.addAlias("ui.vrd*");
    obj.addAlias("ui.radio*");

    obj.useBang();
    obj.useFloat();
    obj.useList();
    obj.usePresets();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DBL_CLICK | UI_MOUSE_DRAG);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);
    obj.usePopup();

    obj.setDefaultSize(120, 15);
    obj.hideLabelInner();

    obj.addProperty("active_color", _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIRadio::prop_color_active);
    obj.addProperty("nitems", _("Number of Items"), 8, &UIRadio::prop_nitems_, _("Main"));
    obj.setPropertyRange("nitems", 2, MAX_ITEMS);
    obj.setPropertyAccessor("nitems", &UIRadio::p_numItems, &UIRadio::p_setNumItems);
    obj.addProperty("mode", _("Check List Mode"), false, &UIRadio::prop_checklist_mode_, _("Main"));
    obj.setPropertyAccessor("mode", &UIRadio::p_mode, &UIRadio::p_setMode);
    obj.addProperty("value", &UIRadio::p_value, &UIRadio::p_setValue);

    obj.addMethod("+", &UIRadio::m_plus);
    obj.addMethod("-", &UIRadio::m_minus);
    obj.addMethod("flip", &UIRadio::m_flip);
    obj.addMethod("next", &UIRadio::m_next);
    obj.addMethod("prev", &UIRadio::m_prev);
    obj.addMethod("random", &UIRadio::m_random);
    obj.addMethod("reset", &UIRadio::m_reset);
    obj.addMethod("set", &UIRadio::p_setValue);
}
