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
#include "ceammc_object.h"
#include "nui/button_model.h"
#include "nui/simple_widget.h"

namespace ceammc {
namespace ui {

    class NUIBangBase : public BaseObject {
    public:
        NUIBangBase(const PdArgs& args);
    };

    class NUIBang : public SimpleTclWidget<NUIBangBase> {
        ButtonModel model_;
        ClockLambdaFunction clock_;

    public:
        NUIBang(const PdArgs& args);

        void onBang() override;
        void onMouseDown(const Point& pt, const Point& abspt, uint32_t mod) override;
    };

}
}

void setup_nui_bang();

#endif // NUI_BANG_H
