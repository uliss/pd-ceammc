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
#include "nui_help_box_tcl.h"

namespace ceammc {
namespace ui {

    namespace {
        std::pair<Point, const char*> anchorToPos(const Rect& rect, const AnchorPosition an)
        {
            switch (an) {
            case ANCHOR_CORNER_LEFT_TOP:
                return { rect.leftTop(), "nw" };
            case ANCHOR_SIDE_LEFT_CENTER:
                return { rect.leftCenter(), "w" };
            default:
                return { rect.leftTop(), "nw" };
            }
        }
    }

    void TclHelpboxImpl::create(const RectF& bbox, const HelpboxData& data)
    {
        Rect rect = transform(bbox);
        auto pt = rect.pt0();

        sys_vgui("nui::help_box::create %lx %lx %lx"
                 " %d %d"
                 " %s"
                 " %d"
                 " %d"
                 " {%s}\n",
            winId(), widgetId(), this,
            pt.x(), pt.y(),
            data.title().c_str(),
            data.textWidth(),
            data.isOpen(),
            "line1\nline2");
    }

    void TclHelpboxImpl::erase()
    {
        sys_vgui("nui::help_box::erase %lx %lx %lx\n", winId(), widgetId(), this);
    }

    void TclHelpboxImpl::update(const RectF& bbox, const HelpboxData& data)
    {
        Rect rect = transform(bbox);
        auto pt = rect.pt0();

        sys_vgui("nui::help_box::update %lx %lx %lx"
                 " %d %d"
                 " %s"
                 " %d"
                 " %d"
                 " {%s}\n",
            winId(), widgetId(), this,
            pt.x(), pt.y(),
            data.title().c_str(),
            data.textWidth(),
            data.isOpen(),
            "content\ncontent");
    }

    void TclHelpboxImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::help_box::move %lx %lx %lx %d %d\n",
            winId(), widgetId(), this, rect.left(), rect.top());
    }

    void tcl_help_box_init()
    {
        nui_help_box_tcl_output();
    }
}
}
