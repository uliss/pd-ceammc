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
#include "label_view.h"
#include "nui_label_tcl.h"

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

    void TclLabelImpl::create(const RectF& bbox, const LabelData& data)
    {
        Rect rect = transform(bbox);
        auto anchor = anchorToPos(rect, data.anchor());
        Point pt = anchor.first;

        sys_vgui("nui::label::create %lx %lx %lx"
                 " %d %d"
                 " %d %s {{%s} %d} #%6.6x {%s}\n",
            winId(), widgetId(), this,
            pt.x(), pt.y(),
            data.textWidth(), anchor.second,
            data.font().family(), int(data.font().size() * scale()),
            data.color(), data.text().c_str());
    }

    void TclLabelImpl::erase()
    {
        sys_vgui("nui::label::erase %lx %lx %lx\n", winId(), widgetId(), this);
    }

    void TclLabelImpl::update(const RectF& bbox, const LabelData& data)
    {
        Rect rect = transform(bbox);
        auto anchor = anchorToPos(rect, data.anchor());
        Point pt = anchor.first;

        sys_vgui("nui::label::update %lx %lx %lx"
                 " %d %d"
                 " %d %s {{%s} %d} #%6.6x {%s}\n",
            winId(), widgetId(), this,
            pt.x(), pt.y(),
            data.textWidth(), anchor.second,
            data.font().family(), int(data.font().size() * scale()),
            data.color(), data.text().c_str());
    }

    void TclLabelImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::label::move %lx %lx %lx %d %d\n",
            winId(), widgetId(), this, rect.left(), rect.top());
    }

    void tcl_label_init()
    {
        nui_label_tcl_output();
    }
}
}
