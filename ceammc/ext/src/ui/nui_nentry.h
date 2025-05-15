/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#ifndef NUI_NENTRY_H
#define NUI_NENTRY_H

#include "ceammc_object.h"
#include "nui/nentry_model.h"
#include "nui/simple_widget.h"
namespace ceammc {
namespace ui {

    class NUINumEntryBase : public BaseObject {
    public:
        explicit NUINumEntryBase(const PdArgs& args);
    };

    class NUINumEntry : public SimpleTclWidget<NUINumEntryBase> {
        NumentryModel model_;

    public:
        explicit NUINumEntry(const PdArgs& args);

        void onBang() override;
        void onFloat(t_float f) override;
        void onMouseDown(const Point& pt, const Point& abspt, uint32_t mod) override;

    private:
        void set(t_float f);
    };

} // namespace ui
} // namespace ceammc

#endif // NUI_NENTRY_H
