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
#ifndef NUI_TOGGLE_H
#define NUI_TOGGLE_H

#include "ceammc_clock.h"
#include "nui/toggle_model.h"
#include "nui/view.h"
#include "nui/widget.h"

namespace ceammc {
namespace ui {

class NUIToggleBase : public BaseObject {
public:
    NUIToggleBase(const PdArgs& args);
};

class NUIToggle : public ui::Widget<NUIToggleBase> {
    BoxModel box_model_;
    BoxView box_view_;
    ToggleModel model_;

public:
    NUIToggle(const PdArgs& args);

    void onFloat(t_float f) override;

    void onWidgetShow() override;
//    void onWidgetResize(const Size& sz) override;
    void onWidgetSelect(bool state) override;

    void onMouseDown(const Point& pt, const Point& abspt, uint32_t mod) override;
    void onWidgetResize(const Size& new_sz) override;
//    void onMouseDrag(const Point& pt, uint32_t mod) override;
//    void onMouseUp(const Point& pt, uint32_t mod) override;

protected:
//    void compile() override;
//    void createCustomUI() override;
};

}
}

void setup_nui_bang();

#endif // NUI_BANG_H
