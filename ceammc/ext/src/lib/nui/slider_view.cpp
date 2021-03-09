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
#include "slider_view.h"
#include "ceammc_convert.h"

#include "nui_slider_tcl.h"

namespace ceammc {
namespace ui {

    HSliderView::HSliderView(SliderModel* model, ModelView::ViewImplPtr&& impl, const PointF& pos)
        : ModelView<SliderData>(model, std::move(impl), pos)
    {
    }

    EventAcceptStatus HSliderView::onEvent(EventType t, const PointF& pos, const EventContext& ctx)
    {
        switch (t) {
        case EVENT_MOUSE_DOWN: {
            auto vpos = toViewCoords(pos);
            auto value = convert::lin2lin<t_float>(vpos.x(), 0, size().width(), this->data().min(), this->data().max());
            data().setValue(value);
            redraw();
            notifyOthers();
            return { this, EVENT_STATUS_ACCEPT };
        }
        case EVENT_MOUSE_DRAG: {
            auto vpos = toViewCoords(pos);
            auto value = convert::lin2lin_clip<t_float>(vpos.x(), 0, size().width(), this->data().min(), this->data().max());
            data().setValue(value);
            redraw();
            notifyOthers();
            return { this, EVENT_STATUS_ACCEPT };
        }
        case EVENT_MOUSE_UP:
            return { nullptr, EVENT_STATUS_ACCEPT };
        default:
            return { this, EVENT_STATUS_IGNORE };
        }
    }

    void TclHSliderImpl::create(const RectF& bbox, const SliderData& data)
    {
        Rect rect = transform(bbox);

        const float pos = (data.value() - data.min()) / (data.max() - data.min());
        sys_vgui("nui::slider::hcreate %lx %lx %lx"
                 " %d %d %d %d %d"
                 " %f %g %g %g"
                 " #%6.6x #%6.6x #%6.6x\n",
            winId(), widgetId(), this,
            rect.left(), rect.top(), rect.width(), rect.height(), (int)scale(),
            pos, data.value(), data.min(), data.max(),
            data.borderColor(), data.fillColor(), data.knobColor());
    }

    void TclHSliderImpl::erase()
    {
        sys_vgui("nui::slider::erase %lx %lx %lx \n", winId(), widgetId(), this);
    }

    void TclHSliderImpl::update(const RectF& bbox, const SliderData& data)
    {
        const float pos = (data.value() - data.min()) / (data.max() - data.min());

        sys_vgui("nui::slider::hupdate %lx %lx %lx %d"
                 " %f %g %g %g"
                 " #%6.6x #%6.6x #%6.6x\n",
            winId(), widgetId(), this, (int)scale(),
            pos, data.value(), data.min(), data.max(),
            data.borderColor(), data.fillColor(), data.knobColor());
    }

    void TclHSliderImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::slider::move %lx %lx %lx %d %d\n",
            winId(), widgetId(), this, rect.left(), rect.top());
    }

    void tcl_hslider_init()
    {
        nui_slider_tcl_output();
    }
}
}
