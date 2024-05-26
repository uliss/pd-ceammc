/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "ui_midi.h"
#include "ceammc_ui.h"
#include "cicm/Sources/egraphics.h"
#include "fmt/core.h"

#define DECLARE_SYM_MIDI(name) \
    inline t_symbol* sym_##name() { return gensym("#" #name); }

DECLARE_SYM_MIDI(notein)
DECLARE_SYM_MIDI(ctlin)
DECLARE_SYM_MIDI(pgmin)
DECLARE_SYM_MIDI(sysexin)
DECLARE_SYM_MIDI(bendin)
DECLARE_SYM_MIDI(touchin)
DECLARE_SYM_MIDI(polytouchin)

UIMidi::UIMidi()
    : note_(this, &UIMidi::onNote)
    , ctlin_(this, &UIMidi::onCtlin)
    , sysex_(this, &UIMidi::onSysex)
    , pgm_(this, &UIMidi::onPgmin)
    , bend_(this, &UIMidi::onBendin)
    , touch_(this, &UIMidi::onTouch)
    , polyt_(this, &UIMidi::onPolyTouch)
    , txt_type_(&asEBox()->b_font, ColorRGBA::black(), ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , txt_body_(&asEBox()->b_font, ColorRGBA::black(), ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , prop_text_color(rgba_black)
    , prop_hex(0)
{
    initPopupMenu("main", { { _("Midi Settings"), [](const t_pt&) { openMidiSettingsDialog(); } } });

    createOutlet();
}

void UIMidi::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    sysex_buffer_.reserve(256);

    // note: here, not in constructor!
    ctlin_.bind(sym_ctlin());
    note_.bind(sym_notein());
    touch_.bind(sym_touchin());
    polyt_.bind(sym_polytouchin());
    bend_.bind(sym_bendin());
    sysex_.bind(sym_sysexin());
    pgm_.bind(sym_pgmin());
}

void UIMidi::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, 100);
    newrect->h = pd_clip_min(newrect->h, 10);
}

void UIMidi::paint()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    float lbl_wd = 46;
#ifdef __APPLE__
    lbl_wd += 0;
#endif

#ifdef __WIN32__
    lbl_wd += 12;
#endif

    p.drawRect(-1, -1, lbl_wd, r.h + 1);
    p.setColor(prop_active_color);
    p.fillPreserve();
    p.setColor(prop_color_border);
    p.setLineWidth(1);
    p.stroke();

    txt_type_.set(msg_type_,
        2, r.h / 2,
        r.w, r.h);
    txt_type_.setColor(prop_text_color);
    p.drawText(txt_type_);

    txt_body_.set(msg_body_,
        lbl_wd + 2, r.h / 2,
        r.w, r.h);
    txt_body_.setColor(prop_text_color);
    p.drawText(txt_body_);
}

void UIMidi::printType(int type)
{
    *fmt::format_to(msg_type_, "MIDI[{:02d}]", type) = '\0';
}

void UIMidi::onNote(const AtomListView& lv)
{
    if (!prop_show_note || lv.size() != 3)
        return;

    printType(lv[2].asT<t_int>());

    if (prop_hex)
        *fmt::format_to(msg_body_, "Note {:02X} {:02X}", lv[0].asT<t_int>(), lv[1].asT<t_int>()) = '\0';
    else
        *fmt::format_to(msg_body_, "Note {:3d} {:3d}", lv[0].asT<t_int>(), lv[1].asT<t_int>()) = '\0';

    bg_layer_.invalidate();
    redraw();

    anyTo(0, sym_notein(), lv);
}

void UIMidi::onCtlin(const AtomListView& lv)
{
    if (!prop_show_cc || lv.size() != 3)
        return;

    printType(lv[2].asT<t_int>());

    if (prop_hex)
        *fmt::format_to(msg_body_, "Ctl {:02X} {:02X}", lv[0].asT<t_int>(), lv[1].asT<t_int>()) = '\0';
    else
        *fmt::format_to(msg_body_, "Ctl {:3d} {:3d}", lv[0].asT<t_int>(), lv[1].asT<t_int>()) = '\0';

    bg_layer_.invalidate();
    redraw();

    anyTo(0, sym_ctlin(), lv);
}

void UIMidi::onPgmin(const AtomListView& lv)
{
    if (!prop_show_pgm || lv.size() != 2)
        return;

    printType(lv[1].asT<t_int>());

    if (prop_hex)
        *fmt::format_to(msg_body_, "Program: {:02X}", lv[0].asT<t_int>()) = '\0';
    else
        *fmt::format_to(msg_body_, "Program: {:3d}", lv[0].asT<t_int>()) = '\0';

    bg_layer_.invalidate();
    redraw();

    anyTo(0, sym_pgmin(), lv);
}

void UIMidi::onSysex(const AtomListView& lv)
{
    if (!prop_show_sysex || lv.size() != 2)
        return;

    constexpr int SYSEX_BEGIN = 0xF0;
    constexpr int SYSEX_END = 0xF7;

    const auto byte = lv[0].asT<t_int>();
    const auto ch = lv[1].asT<t_int>();

    if (byte == SYSEX_BEGIN)
        sysex_buffer_.clear();

    sysex_buffer_.push_back(byte);

    if (byte == SYSEX_END) {
        Atom out_msg[sysex_buffer_.size()];

        printType(ch);

        const int LEN = sizeof(msg_body_) - 1;
        auto pos = snprintf(msg_body_, LEN, "SysEx:");
        for (size_t i = 0; i < sysex_buffer_.size(); i++) {
            const auto v = sysex_buffer_[i];
            out_msg[i] = v;

            if ((LEN - pos) < 1)
                continue;

            if (prop_hex)
                pos += snprintf(msg_body_ + pos, LEN - pos, " %02X", (int)v);
            else
                pos += snprintf(msg_body_ + pos, LEN - pos, " %d", (int)v);
        }

        bg_layer_.invalidate();
        redraw();

        anyTo(0, sym_sysexin(), AtomListView(out_msg, sysex_buffer_.size()));
        sysex_buffer_.clear();
    }
}

void UIMidi::onBendin(const AtomListView& lv)
{
    if (!prop_show_bend || lv.size() != 2)
        return;

    printType(lv[1].asT<t_int>());

    if (prop_hex)
        *fmt::format_to(msg_body_, "Pitch Wheel: {:04X}", lv[0].asT<t_int>()) = '\0';
    else
        *fmt::format_to(msg_body_, "Pitch Wheel: {:6d}", lv[0].asT<t_int>() - 8192) = '\0';

    bg_layer_.invalidate();
    redraw();

    anyTo(0, sym_bendin(), lv);
}

void UIMidi::onTouch(const AtomListView& lv)
{
    if (!prop_show_touch || lv.size() != 2)
        return;

    printType(lv[1].asT<t_int>());

    if (prop_hex)
        *fmt::format_to(msg_body_, "Aftertouch: {:02X}", lv[0].asT<t_int>()) = '\0';
    else
        *fmt::format_to(msg_body_, "Aftertouch: {:3d}", lv[0].asT<t_int>()) = '\0';

    bg_layer_.invalidate();
    redraw();

    anyTo(0, sym_touchin(), lv);
}

void UIMidi::onPolyTouch(const AtomListView& lv)
{
    if (!prop_show_touch || lv.size() != 3)
        return;

    printType(lv[2].asT<t_int>());

    if (prop_hex)
        *fmt::format_to(msg_body_, "Aftertouch(Poly): {:02X} {:02X}", lv[0].asT<t_int>(), lv[1].asT<t_int>()) = '\0';
    else
        *fmt::format_to(msg_body_, "Aftertouch(Poly): {:3d} {:3d}", lv[0].asT<t_int>(), lv[1].asT<t_int>()) = '\0';

    bg_layer_.invalidate();
    redraw();

    anyTo(0, sym_polytouchin(), lv);
}

void UIMidi::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
    openMidiSettingsDialog();
}

void UIMidi::openMidiSettingsDialog()
{
    sys_gui("pdsend \"pd midi-properties\"\n");
}

void UIMidi::setup()
{
    UIObjectFactory<UIMidi> obj("ui.midi", EBOX_GROWINDI);
    obj.useList();
    obj.setDefaultSize(180, 15);
    obj.hideLabel();
    obj.usePopup();
    obj.hideFontProps();
    obj.useMouseEvents(UI_MOUSE_DBL_CLICK);

    obj.addProperty(sym::props::name_text_color, _("Text Color"), DEFAULT_TEXT_COLOR, &UIMidi::prop_text_color);
    obj.addProperty(sym::props::name_active_color, _("Active Color"), "0.63 0.88 0 1", &UIMidi::prop_active_color);

    obj.addBoolProperty("hex", _("Show in hex"), false, &UIMidi::prop_hex, _("Main"));
    obj.addBoolProperty("notes", _("Show Note On/Off"), true, &UIMidi::prop_show_note, _("Main"));
    obj.addBoolProperty("cc", _("Show Control"), true, &UIMidi::prop_show_cc, _("Main"));
    obj.addBoolProperty("program", _("Show Program"), true, &UIMidi::prop_show_pgm, _("Main"));
    obj.addBoolProperty("touch", _("Show Aftertouch (poly)"), true, &UIMidi::prop_show_touch, _("Main"));
    obj.addBoolProperty("pitchwheel", _("Show Pitch Wheel"), true, &UIMidi::prop_show_bend, _("Main"));
    obj.addBoolProperty("sysex", _("Show SysEx"), true, &UIMidi::prop_show_sysex, _("Main"));
}

void setup_ui_midi()
{
    UIMidi::setup();
}
