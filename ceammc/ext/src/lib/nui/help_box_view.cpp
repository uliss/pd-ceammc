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
#include "help_box_view.h"
#include "ceammc_log.h"
#include "cpp_tcl_helpbox_view.h"
#include "nui_help_box_tcl.h"

namespace ceammc {
namespace ui {

    void TclHelpboxImpl::create(const RectF& bbox, const HelpboxData& data)
    {
        nui::helpbox::tcl_create(winId(), widgetId(), this, transform(bbox), data);
    }

    void TclHelpboxImpl::erase()
    {
        nui::helpbox::tcl_erase(winId(), widgetId(), this);
    }

    void TclHelpboxImpl::update(const RectF& bbox, const HelpboxData& data)
    {
        nui::helpbox::tcl_update(winId(), widgetId(), this, transform(bbox), data);
    }

    void TclHelpboxImpl::updateCoords(const RectF& bbox)
    {
        nui::helpbox::tcl_move(winId(), widgetId(), this, transform(bbox).leftTop());
    }

    void tcl_help_box_init()
    {
        nui_help_box_tcl_output();
    }

    HelpboxView::HelpboxView(ModelBase<HelpboxData>* model, ViewImplPtr&& impl, const PointF& pos)
        : ModelView<HelpboxData>(model, std::move(impl), pos)
    {
    }

    EventAcceptStatus HelpboxView::onEvent(EventType t, const PointF& pos, const EventContext& ctx)
    {
        switch (t) {
        case EVENT_MOUSE_DOWN:
            data().setIsOpen(!data().isOpen());
            redraw();
            notifyOthers();

            LIB_DBG << "state: " << data().isOpen();

            return { nullptr, EVENT_STATUS_ACCEPT };
        // case EVENT_MOUSE_LEAVE:
        //     data().setState(false);
        //     redraw();
        //     notifyOthers();
        //     return { nullptr, EVENT_STATUS_ACCEPT };
        default:
            return { nullptr, EVENT_STATUS_IGNORE };
        }
    }

}
}
