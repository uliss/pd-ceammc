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

static t_symbol* SYM_NOTEIN;
static t_symbol* SYM_CTLIN;
static t_symbol* SYM_PGMIN;
static t_symbol* SYM_SYSEX;
static t_symbol* SYM_BENDIN;
static t_symbol* SYM_TOUCHIN;
static t_symbol* SYM_POLYTOUCH;

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
    createOutlet();
}

void UIMidi::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    sysex_buffer_.reserve(256);

    // note: here, not in constructor!
    ctlin_.bind(SYM_CTLIN);
    note_.bind(SYM_NOTEIN);
    polyt_.bind(SYM_POLYTOUCH);
    bend_.bind(SYM_BENDIN);
    sysex_.bind(SYM_SYSEX);
}

void UIMidi::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 100);
    newrect->height = pd_clip_min(newrect->height, 10);
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

    p.drawRect(-1, -1, lbl_wd, r.height + 1);
    p.setColor(prop_active_color);
    p.fillPreserve();
    p.setColor(prop_color_border);
    p.setLineWidth(1);
    p.stroke();

    txt_type_.set(msg_type_,
        2, r.height / 2,
        r.width, r.height);
    txt_type_.setColor(prop_text_color);
    p.drawText(txt_type_);

    txt_body_.set(msg_body_,
        lbl_wd + 2, r.height / 2,
        r.width, r.height);
    txt_body_.setColor(prop_text_color);
    p.drawText(txt_body_);
}

void UIMidi::onNote(const AtomListView& lv)
{
    if (!prop_show_note || lv.size() != 3)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[2].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Note: %02X %02X", lv[0].asT<int>(), lv[1].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Note: %3d %3d", lv[0].asT<int>(), lv[1].asT<int>());

    bg_layer_.invalidate();
    redraw();

    anyTo(0, SYM_NOTEIN, lv);
}

void UIMidi::onCtlin(const AtomListView& lv)
{
    if (!prop_show_cc || lv.size() != 3)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[2].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Ctl: %02X %02X", lv[0].asT<int>(), lv[1].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Ctl: %3d %3d", lv[0].asT<int>(), lv[1].asT<int>());

    bg_layer_.invalidate();
    redraw();

    anyTo(0, SYM_CTLIN, lv);
}

void UIMidi::onPgmin(const AtomListView& lv)
{
    if (!prop_show_pgm || lv.size() != 2)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[1].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Program: %02X", lv[0].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Program: %3d", lv[0].asT<int>());

    bg_layer_.invalidate();
    redraw();

    anyTo(0, SYM_PGMIN, lv);
}

void UIMidi::onSysex(const AtomListView& lv)
{
    if (!prop_show_sysex || lv.size() != 2)
        return;

    constexpr int SYSEX_BEGIN = 0xF0;
    constexpr int SYSEX_END = 0xF7;

    const auto byte = lv[0].asT<int>();
    const auto ch = lv[1].asT<int>();

    if (byte == SYSEX_BEGIN)
        sysex_buffer_.clear();

    sysex_buffer_.push_back(byte);

    if (byte == SYSEX_END) {
        Atom out_msg[sysex_buffer_.size()];

        snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", ch);

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

        anyTo(0, SYM_SYSEX, AtomListView(out_msg, sysex_buffer_.size()));
        sysex_buffer_.clear();
    }
}

void UIMidi::onBendin(const AtomListView& lv)
{
    if (!prop_show_bend || lv.size() != 2)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[1].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Pitch Wheel: %04X", lv[0].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Pitch Wheel: %6d", lv[0].asT<int>() - 8192);

    bg_layer_.invalidate();
    redraw();

    anyTo(0, SYM_BENDIN, lv);
}

void UIMidi::onTouch(const AtomListView& lv)
{
    if (!prop_show_touch || lv.size() != 2)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[1].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Aftertouch: %02X", lv[0].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Aftertouch: %3d", lv[0].asT<int>());

    bg_layer_.invalidate();
    redraw();

    anyTo(0, SYM_TOUCHIN, lv);
}

void UIMidi::onPolyTouch(const AtomListView& lv)
{
    if (!prop_show_touch || lv.size() != 3)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[2].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Aftertouch(Poly): %02X %02X", lv[0].asT<int>(), lv[1].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "Aftertouch(Poly): %3d %3d", lv[0].asT<int>(), lv[1].asT<int>());

    bg_layer_.invalidate();
    redraw();

    anyTo(0, SYM_POLYTOUCH, lv);
}

void UIMidi::setup()
{
    SYM_NOTEIN = gensym("#notein");
    SYM_CTLIN = gensym("#ctlin");
    SYM_PGMIN = gensym("#pgmin");
    SYM_SYSEX = gensym("#sysexin");
    SYM_BENDIN = gensym("#bendin");
    SYM_TOUCHIN = gensym("#touchin");
    SYM_POLYTOUCH = gensym("#polytouchin");

    UIObjectFactory<UIMidi> obj("ui.midi", EBOX_GROWINDI);
    obj.useList();
    obj.setDefaultSize(180, 15);
    obj.hideLabel();
    obj.hideFontProps();

    obj.addProperty(PROP_TEXT_COLOR, _("Text Color"), DEFAULT_TEXT_COLOR, &UIMidi::prop_text_color);
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), "0.63 0.88 0 1", &UIMidi::prop_active_color);

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
