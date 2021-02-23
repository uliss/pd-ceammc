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
#ifndef BUTTON_VIEW_H
#define BUTTON_VIEW_H

#include "nui/button_model.h"
#include "nui/view.h"

namespace ceammc {
namespace ui {

    class ButtonView : public ModelView<ButtonData> {
    public:
        ButtonView(ButtonModel* model, ViewImplPtr&& impl, const PointF& pos);

        EventAcceptStatus onEvent(EventType t, const PointF& pos, const EventContext& ctx) override;
    };

    class TclButtonImpl : public ViewImpl<ButtonData> {
    public:
        void create(const RectF& bbox, const ButtonData& data) final;
        void erase() final;
        void update(const RectF& bbox, const ButtonData& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    void tcl_button_init();

}
}

#endif // BUTTON_VIEW_H
