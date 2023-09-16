/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef UI_TOUCHOSC_H
#define UI_TOUCHOSC_H

#include "ceammc_object.h"

#include <memory>

namespace ceammc {

namespace touchosc {
    class Layout;
}

class UiTouchOsc : public BaseObject {
    BoolProperty* server_;
    std::string xml_content_;
    std::unique_ptr<touchosc::Layout> layout_;

public:
    UiTouchOsc(const PdArgs& args);
    ~UiTouchOsc();

    void m_auto(t_symbol* s, const AtomListView& lv);
    void m_save(t_symbol* s, const AtomListView& lv);
};

}

void setup_ui_touchosc();

#endif // UI_TOUCHOSC_H
