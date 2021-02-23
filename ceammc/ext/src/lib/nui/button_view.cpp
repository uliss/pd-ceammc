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
#include "button_view.h"
#include "nui_button.tcl.h"

namespace ceammc {
namespace ui {

    ButtonView::ButtonView(ButtonModel* model, ButtonView::ViewImplPtr&& impl, const PointF& pos)
        : ModelView<ButtonData>(model, std::move(impl), pos)
    {
    }

    EventAcceptStatus ButtonView::onEvent(EventType t, const PointF& pos, const EventContext& ctx)
    {
        switch (t) {
        case EVENT_MOUSE_DOWN:
            data().setState(true);
            redraw();
            notifyOthers();
            return { nullptr, EVENT_STATUS_ACCEPT };
        case EVENT_MOUSE_UP:
        case EVENT_MOUSE_LEAVE:
            data().setState(false);
            redraw();
            notifyOthers();
            return { nullptr, EVENT_STATUS_ACCEPT };
        default:
            return { nullptr, EVENT_STATUS_IGNORE };
        }
    }

    void TclButtonImpl::create(const RectF& bbox, const ButtonData& data)
    {
        const SizeF min(SizeF(5, 5));
        const Rect rect = transform(bbox).clippedMin(min);

        sys_vgui("nui::button::create %lx %lx %lx"
                 " %d %d %d %d %d"
                 " #%6.6x #%6.6x"
                 " %d {%s} {%s}\n",
            winId(), widgetId(), this,
            rect.left(), rect.top(), rect.width(), rect.height(), data.state() ? 1 : 0,
            data.borderColor(), data.activeColor(), data.activeColor());
    }

    void TclButtonImpl::erase()
    {
    }

    void TclButtonImpl::update(const RectF& bbox, const ButtonData& data)
    {
    }

    void TclButtonImpl::updateCoords(const RectF& bbox)
    {
    }

    void tcl_button_init()
    {
        sys_gui(nui_button_tcl);
    }
}
}
