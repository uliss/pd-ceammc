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
#ifndef NUI_BANG_H
#define NUI_BANG_H

#include "ceammc_clock.h"
#include "nui/widget.h"
#include "nui/button_view.h"
#include "nui/button_model.h"

namespace ceammc {
namespace ui {

class NUIBangBase : public BaseObject {
public:
    NUIBangBase(const PdArgs& args) : BaseObject(args){}
};

class NUIBang : public ui::Widget<NUIBangBase> {
    ButtonModel model_;
    ButtonView view_;
    ClockLambdaFunction clock_;

public:
    NUIBang(const PdArgs& args);

    void buildUI() override;

    void onWidgetShow() override;
//    void onWidgetResize(const Size& sz) override;
//    void onWidgetSelect(bool state) override;

    void onMouseDown(const Point& pt, const Point& abspt, uint32_t mod) override;
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
