#include "ui_dsp.h"
#include "ceammc_pd.h"
#include "ceammc_ui.h"
#include "ui_dsp.tcl.h"

CEAMMC_DEFINE_SYM(pd)

UIDsp::UIDsp()
    : prop_color_active(rgba_black)
    , state_(false)
    , init_(false)
{
    initPopupMenu("main", { { _("Audio Settings"), [this](const t_pt&) { openSoundSettingsDialog(); } } });
}

void UIDsp::init(t_symbol* name, const AtomListView& args, bool)
{
    UIObject::init(name, args, false);
    bindTo(sym_pd());
}

bool UIDsp::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, 30);
    newrect->h = pd_clip_min(newrect->h, 30);
    return true;
}

void UIDsp::paint()
{
    // first time only to draw in right state
    if (!init_) {
        state_ = pd_getdspstate();
        init_ = true;
    }

    sys_vgui("ui::dsp_update %s %lx %d %d %d #%6.6x\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(), (int)zoom(),
        rgba_to_hex_int(state_ ? prop_color_active : prop_color_border));
}

void UIDsp::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    if (pd_getdspstate())
        m_stop(AtomList());
    else
        m_start(AtomList());
}

void UIDsp::onAny(t_symbol* s, const AtomListView& lst)
{
    if (s == sym_dsp() && lst.size() > 0 && lst[0].isFloat()) {
        state_ = lst[0].asInt(0);
        redrawAll();
    }
}

void UIDsp::m_start(const AtomListView&)
{
    pd::send_message(sym_pd(), sym_dsp(), Atom(1));
    state_ = true;
    redrawAll();
}

void UIDsp::m_stop(const AtomListView&)
{
    pd::send_message(sym_pd(), sym_dsp(), Atom(0.));
    state_ = false;
    redrawAll();
}

void UIDsp::m_settings(const AtomListView&)
{
    openSoundSettingsDialog();
}

void UIDsp::openSoundSettingsDialog()
{
    sys_gui("pdsend \"pd audio-properties\"\n");
}

void UIDsp::redrawAll()
{
    bg_layer_.invalidate();
    redraw();
}

void UIDsp::setup()
{
    sys_gui(ui_dsp_tcl);

    UIObjectFactory<UIDsp> obj("ui.dsp~", EBOX_GROWLINK);
    obj.hideLabelInner();

    obj.useAny();
    obj.usePopup();
    obj.setDefaultSize(30, 30);
    obj.useMouseEvents(UI_MOUSE_DOWN);
    obj.addProperty("active_color", _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIDsp::prop_color_active);

    obj.addMethod("start", &UIDsp::m_start);
    obj.addMethod("stop", &UIDsp::m_stop);
    obj.addMethod("settings", &UIDsp::m_settings);
}

void setup_ui_dsp()
{
    UIDsp::setup();
}
