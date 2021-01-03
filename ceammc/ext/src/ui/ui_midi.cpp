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

static t_rgba CHANNEL_BACKGROUND = hex_to_rgba("#A0E000");

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
    , prop_channel_bgcolor(CHANNEL_BACKGROUND)
    , prop_hex(0)
{
    createOutlet();
}

void UIMidi::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    // note: here, not in constructor!
    ctlin_.bind(SYM_CTLIN);
    note_.bind(SYM_NOTEIN);
    polyt_.bind(SYM_POLYTOUCH);
    bend_.bind(SYM_BENDIN);
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
    p.setColor(prop_channel_bgcolor);
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
    if (lv.size() != 3)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[2].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "NOTE: %02X %02X", lv[0].asT<int>(), lv[1].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "NOTE: %3d %3d", lv[0].asT<int>(), lv[1].asT<int>());

    bg_layer_.invalidate();
    redraw();
}

void UIMidi::onCtlin(const AtomListView& lv)
{
    if (lv.size() != 3)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[2].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "CC: %02X %02X", lv[0].asT<int>(), lv[1].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "CC: %3d %3d", lv[0].asT<int>(), lv[1].asT<int>());

    bg_layer_.invalidate();
    redraw();
}

void UIMidi::onPgmin(const AtomListView& lv)
{
    if (lv.size() != 2)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[1].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "PGM: %02X", lv[0].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "PGM: %3d", lv[0].asT<int>());

    bg_layer_.invalidate();
    redraw();
}

void UIMidi::onSysex(const AtomListView& lv)
{
}

void UIMidi::onBendin(const AtomListView& lv)
{
    if (lv.size() != 2)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[1].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "BEND: %04X", lv[0].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "BEND: %6d", lv[0].asT<int>() - 8192);

    bg_layer_.invalidate();
    redraw();
}

void UIMidi::onTouch(const AtomListView& lv)
{
}

void UIMidi::onPolyTouch(const AtomListView& lv)
{
    if (lv.size() != 3)
        return;

    snprintf(msg_type_, sizeof(msg_type_) - 1, "MIDI[%02d]", lv[2].asT<int>());

    if (prop_hex)
        snprintf(msg_body_, sizeof(msg_body_) - 1, "POLYTCH: %02X %02X", lv[0].asT<int>(), lv[1].asT<int>());
    else
        snprintf(msg_body_, sizeof(msg_body_) - 1, "POLYTCH: %3d %3d", lv[0].asT<int>(), lv[1].asT<int>());

    bg_layer_.invalidate();
    redraw();
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
    obj.setDefaultSize(140, 15);
    obj.hideLabel();

    obj.addProperty(PROP_TEXT_COLOR, _("Text Color"), DEFAULT_TEXT_COLOR, &UIMidi::prop_text_color);
    obj.addProperty("midi_label_color", _("Label Color"), "0.6274509 0.87843 0 1", &UIMidi::prop_channel_bgcolor);
    obj.addBoolProperty("hex", _("Hex"), false, &UIMidi::prop_hex, _("Main"));
}

void setup_ui_midi()
{
    UIMidi::setup();
}
