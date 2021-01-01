#include "ui_dsp.h"
#include "ceammc_ui.h"

UIDsp::UIDsp()
    : state_(false)
    , init_(false)
    , prop_color_active(rgba_black)
{
    initPopupMenu("main", { { _("Audio Settings"), [this](const t_pt&) { openSoundSettingsDialog(); } } });
}

void UIDsp::init(t_symbol* name, const AtomList& args, bool)
{
    UIObject::init(name, args, false);
    bindTo(gensym("pd"));
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
        state_ = canvas_dspstate;
        init_ = true;
    }

    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (p) {
        p.setColor(state_ ? prop_color_active : prop_color_border);
        const float center = roundf(r.width * 0.5f - 0.5f);

        p.drawCircle(center, center, roundf(r.width * 0.15f - 0.5f));
        p.fill();

        p.setLineWidth(2);
        p.drawCircleArc(center, center, roundf(r.width * 0.25f - 0.5f), 0, EPD_PI);
        p.drawCircleArc(center, center, roundf(r.width * 0.35f - 0.5f), 0, EPD_PI);
    }
}

void UIDsp::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    if (canvas_dspstate)
        m_stop(AtomList());
    else
        m_start(AtomList());
}

void UIDsp::onAny(t_symbol* s, const AtomListView& lst)
{
    if (s == gensym("dsp") && lst.size() > 0 && lst[0].isFloat()) {
        state_ = lst[0].asInt(0);
        redrawAll();
    }
}

void UIDsp::m_start(const AtomListView&)
{
    t_symbol* SYM_PD = gensym("pd");
    t_atom av;
    atom_setfloat(&av, 1);
    pd_typedmess((t_pd*)SYM_PD->s_thing, gensym("dsp"), 1, &av);
    state_ = true;
    redrawAll();
}

void UIDsp::m_stop(const AtomListView&)
{
    t_symbol* SYM_PD = gensym("pd");
    t_atom av;
    atom_setfloat(&av, 0);
    pd_typedmess((t_pd*)SYM_PD->s_thing, gensym("dsp"), 1, &av);
    state_ = false;
    redrawAll();
}

void UIDsp::m_settings(const AtomListView&)
{
    openSoundSettingsDialog();
}

void UIDsp::setup()
{
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

void UIDsp::openSoundSettingsDialog()
{
    sys_gui("pdsend \"pd audio-properties\"\n");
}

void UIDsp::redrawAll()
{
    bg_layer_.invalidate();
    redrawInnerArea();
}

void setup_ui_dsp()
{
    UIDsp::setup();
}
