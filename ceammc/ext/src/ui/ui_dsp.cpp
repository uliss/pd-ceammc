#include "ui_dsp.h"
#include "ceammc_ui.h"
#include "ui_dsp.tcl.h"

static t_symbol* SYM_PD;
static t_symbol* SYM_DSP;

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
    bindTo(SYM_PD);
}

bool UIDsp::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 30);
    newrect->height = pd_clip_min(newrect->height, 30);
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
    if (s == SYM_DSP && lst.size() > 0 && lst[0].isFloat()) {
        state_ = lst[0].asInt(0);
        redrawAll();
    }
}

void UIDsp::m_start(const AtomListView&)
{
    t_atom av;
    atom_setfloat(&av, 1);
    pd_typedmess((t_pd*)SYM_PD->s_thing, SYM_DSP, 1, &av);
    state_ = true;
    redrawAll();
}

void UIDsp::m_stop(const AtomListView&)
{
    t_atom av;
    atom_setfloat(&av, 0);
    pd_typedmess((t_pd*)SYM_PD->s_thing, SYM_DSP, 1, &av);
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

    SYM_DSP = gensym("dsp");
    SYM_PD = gensym("pd");

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
