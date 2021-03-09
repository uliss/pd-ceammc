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
#ifndef UI_MIDI_H
#define UI_MIDI_H

#include "ceammc_proxy.h"
#include "ceammc_ui_object.h"

#include <cstdint>

using namespace ceammc;

class UIMidi : public UIObject {
    using Proxy = PdListProxy<UIMidi>;
    Proxy note_, ctlin_, sysex_, pgm_, bend_, touch_, polyt_;
    char msg_type_[16];
    char msg_body_[240];
    UITextLayout txt_type_, txt_body_;
    std::vector<uint8_t> sysex_buffer_;
    // props
    t_rgba prop_text_color;
    t_rgba prop_active_color;
    int prop_hex;
    int prop_show_note,
        prop_show_cc,
        prop_show_sysex,
        prop_show_pgm,
        prop_show_bend,
        prop_show_touch;

public:
    UIMidi();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void onNote(const AtomListView& lv);
    void onCtlin(const AtomListView& lv);
    void onPgmin(const AtomListView& lv);
    void onSysex(const AtomListView& lv);
    void onBendin(const AtomListView& lv);
    void onTouch(const AtomListView& lv);
    void onPolyTouch(const AtomListView& lv);

public:
    static void setup();

private:
    static void openMidiSettingsDialog();
};

void setup_ui_midi();

#endif // UI_MIDI_H
