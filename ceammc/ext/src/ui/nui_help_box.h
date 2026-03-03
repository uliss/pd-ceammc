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
#ifndef NUI_HELP_BOX_H
#define NUI_HELP_BOX_H

#include "ceammc_object.h"
#include "nui/help_box_model.h"
#include "nui/simple_widget.h"

namespace ceammc {
namespace ui {

    class NUIHelpBoxBase : public BaseObject {
    public:
        explicit NUIHelpBoxBase(const PdArgs& args);
    };

    class NUIHelpBox : public SimpleTclWidget<NUIHelpBoxBase> {
        HelpboxModel model_;

    public:
        explicit NUIHelpBox(const PdArgs& args);

        void onMouseDown(const Point& pt, const Point& abspt, uint32_t mod) override;
    };

} // namespace ui
} // namespace ceammc

void setup_nui_help_box();

#endif // NUI_HELP_BOX_H
