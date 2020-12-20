/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "ceammc_ui_object.h"

using namespace ceammc;

class UILabel : public UIObject {
    UITextLayout text_;
    std::string text_str_;

private:
    t_rgba prop_text_color;
    int prop_margin_top;
    int prop_margin_left;
    int prop_margin_bottom;
    int prop_margin_right;
    t_symbol* prop_align;
    AtomList prop_text;

public:
    UILabel();

    void okSize(t_rect* newrect);
    void paint();
    void setDrawParams(t_edrawparams* params);
    void init(t_symbol* name, const AtomList& args, bool usePresets);

    void onBang();
    void onList(const AtomList& lst);
    void onAny(t_symbol* s, const AtomList& lst);
    void onData(const Atom& data);

    void m_clear(const AtomList&);
    void m_append(const AtomList& lst);
    void m_prepend(const AtomList& lst);

    AtomList propGetText() const;
    void propSetText(const AtomList& lst);

    const std::string& text() const;

public:
    static void setup();
};

void setup_ui_label();

#endif // UI_LABEL_H
