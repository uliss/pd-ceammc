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
#include "tk_view_impl.h"
#include "tcl_nui.tcl.h"

#include "ceammc_convert.h"

#include "m_pd.h"

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

    void TclFrameImpl::create(const RectF& bbox, const FrameData& data)
    {
        const SizeF min(SizeF(data.padding() * 2, data.padding() * 2));
        const Rect rect = transform(bbox).clippedMin(min);

        sys_vgui("nui::frame::create %lx %lx %lx"
                 " %d %d %d %d"
                 " #%6.6x {} %d\n",
            winId(), widgetId(), this,
            rect.left(), rect.top(), rect.width(), rect.height(),
            data.borderColor(), (int)scale());
    }

    void TclFrameImpl::erase()
    {
        sys_vgui("nui::frame::erase %lx %lx %lx\n", winId(), widgetId(), this);
    }

    void TclFrameImpl::update(const RectF& bbox, const FrameData& data)
    {
        const SizeF min(SizeF(data.padding() * 2, data.padding() * 2));
        const Rect rect = transform(bbox).clippedMin(min);

        sys_vgui("nui::frame::update %lx %lx %lx"
                 " %d %d"
                 " #%6.6x {}\n",
            winId(), widgetId(), this,
            rect.width(), rect.height(),
            data.selected() ? data.selectColor() : data.borderColor() /*, data.fillColor()*/);
    }

    void TclFrameImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::frame::move %lx %lx %lx %d %d\n",
            winId(), widgetId(), this, rect.left(), rect.top());
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
            data.color(), data.text()->s_name);
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
            data.color(), data.text()->s_name);
    }

    void TclLabelImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::label::move %lx %lx %lx %d %d\n",
            winId(), widgetId(), this, rect.left(), rect.top());
    }

    bool tcl_nui_init()
    {
        sys_gui(tcl_nui_tcl);
        return true;
    }

    void TclBoxImpl::create(const RectF& bbox, const BoxData& data)
    {
        const SizeF min(SizeF(5, 5));
        const Rect rect = transform(bbox).clippedMin(min);

        sys_vgui("nui::box::create %lx %lx %lx"
                 " %d %d %d %d %d"
                 " #%6.6x #%6.6x"
                 " #%6.6x #%6.6x"
                 " %d {%s} {%s}\n",
            winId(), widgetId(), this,
            rect.left(), rect.top(), rect.width(), rect.height(), (int)scale(),
            data.borderColor(), data.fillColor(),
            data.xletControlColor(), data.xletSignalColor(),
            (int)scale(), data.inlets().asString(), data.outlets().asString());
    }

    void TclBoxImpl::update(const RectF& bbox, const BoxData& data)
    {
        const SizeF min(SizeF(5, 5));
        const Rect rect = transform(bbox).clippedMin(min);

        sys_vgui("nui::box::update %lx %lx %lx"
                 " %d %d %d"
                 " #%6.6x #%6.6x"
                 " #%6.6x #%6.6x"
                 " %d {%s} {%s}\n",
            winId(), widgetId(), this,
            rect.width(), rect.height(), (int)scale(),
            data.borderColor(), data.fillColor(),
            data.xletControlColor(), data.xletSignalColor(),
            (int)scale(), data.inlets().asString(), data.outlets().asString());
    }

    void TclBoxImpl::updateCoords(const RectF& bbox)
    {
    }

    void TclBoxImpl::erase()
    {
    }

    void TclToggleImpl::create(const RectF& bbox, const ToggleData& data)
    {
        Rect rect = transform(bbox);
        auto value = clip01<int>(data.value() > 0);

        sys_vgui("nui::toggle::create %lx %lx %lx"
                 " %d %d %d %d %d"
                 " %d"
                 " #%6.6x #%6.6x #%6.6x\n",
            winId(), widgetId(), this,
            rect.left(), rect.top(), rect.width(), rect.height(), (int)scale(),
            value,
            data.borderColor(), data.fillColor(), data.knobColor());
    }

    void TclToggleImpl::erase()
    {
    }

    void TclToggleImpl::update(const RectF& bbox, const ToggleData& data)
    {
        auto value = clip01<int>(data.value() > 0);
        sys_vgui("nui::toggle::update %lx %lx %lx"
                 " %d %d"
                 " #%6.6x #%6.6x #%6.6x\n",
            winId(), widgetId(), this,
            (int)scale(), value,
            data.borderColor(), data.fillColor(), data.knobColor());
    }

    void TclToggleImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::toggle::move %lx %lx %lx %d %d\n",
            winId(), widgetId(), this, rect.left(), rect.top());
    }

    void TclBarImpl::create(const RectF& bbox, const BarData& data)
    {
        // cnv model id x y w h pos fill_color
        Rect rect = transform(bbox);
        const float pos = (data.value() - data.min()) / (data.max() - data.min());

        sys_vgui("nui::bar::hcreate %lx %lx %lx"
                 " %d %d %d %d"
                 " %g"
                 " #%6.6x\n",
            winId(), widgetId(), this,
            rect.left(), rect.top(), rect.width(), rect.height(),
            pos, data.fillColor());
    }

    void TclBarImpl::erase()
    {
    }

    void TclBarImpl::update(const RectF& bbox, const BarData& data)
    {
        // cnv model id w h pos fill_color
        Rect rect = transform(bbox);
        const float pos = (data.value() - data.min()) / (data.max() - data.min());

        sys_vgui("nui::bar::hupdate %lx %lx %lx"
                 " %d %d %g"
                 " #%6.6x\n",
            winId(), widgetId(), this,
            rect.width(), rect.height(), pos, data.fillColor());
    }

    void TclBarImpl::updateCoords(const RectF& bbox)
    {
    }
}
}
