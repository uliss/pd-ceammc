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

#include "m_pd.h"

namespace ceammc {
namespace ui {

    TclHSliderImpl::TclHSliderImpl(const ViewId& view_id, PropId prop_id)
        : TclViewImpl<SliderProps>(view_id, prop_id)
    {
    }

    void TclHSliderImpl::create(const RectF& bbox, const SliderProps& data)
    {
        Rect rect = transform(bbox);

        const float pos = (data.value - data.min) / (data.max - data.min);
        sys_vgui("nui::slider::hcreate %lx %lx"
                 " %d %d %d %d %f %f"
                 " #%6.6x #%6.6x #%6.6x\n",
            winId(), tclId(),
            rect.left(), rect.top(), rect.right(), rect.bottom(),
            pos, data.value,
            data.bg_color, data.bd_color, data.kn_color);
    }

    void TclHSliderImpl::erase()
    {
        sys_vgui("nui::slider::erase %lx %lx\n", winId(), tclId());
    }

    void TclHSliderImpl::update(const RectF& bbox, const SliderProps& data)
    {
        const float pos = (data.value - data.min) / (data.max - data.min);

        sys_vgui("nui::slider::hupdate %lx %lx"
                 " %f %f"
                 " #%6.6x #%6.6x #%6.6x\n",
            winId(), tclId(),
            pos, data.value,
            data.bg_color, data.bd_color, data.kn_color);
    }

    void TclHSliderImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::slider::move %lx %lx %d %d\n",
            winId(), tclId(), rect.left(), rect.top());
    }

    TclFrameImpl::TclFrameImpl(const ViewId& view_id, PropId prop_id)
        : TclViewImpl<FrameProps>(view_id, prop_id)
    {
    }

    void TclFrameImpl::create(const RectF& bbox, const FrameProps& data)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::frame::create %lx %lx"
                 " %d %d %d %d {} #%6.6x 1\n",
            winId(), tclId(),
            rect.left(), rect.top(), rect.width(), rect.height(),
            data.selected ? data.sel_color : data.bd_color);
    }

    void TclFrameImpl::erase()
    {
        sys_vgui("nui::frame::erase %lx %lx\n", winId(), tclId());
    }

    void TclFrameImpl::update(const RectF& bbox, const FrameProps& data)
    {
        sys_vgui("nui::frame::update_outline %lx %lx"
                 " {} #%6.6x 1\n",
            winId(), tclId(),
            data.selected ? data.sel_color : data.bd_color);
    }

    void TclFrameImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::frame::move %lx %lx %d %d\n",
            winId(), tclId(), rect.left(), rect.top());
    }

    TclLabelImpl::TclLabelImpl(const ViewId& view_id, PropId prop_id)
        : TclViewImpl<LabelProps>(view_id, prop_id)
    {
    }

    void TclLabelImpl::create(const RectF& bbox, const LabelProps& data)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::label::create %lx %lx %d %d"
                 " 0 nw {{%s} %d} #%6.6x {%s}\n",
            winId(), tclId(), rect.left(), rect.top(),
            data.font.family(), int(data.font.size() * scale()),
            data.txt_color, data.text);
    }

    void TclLabelImpl::erase()
    {
        sys_vgui("nui::label::erase %lx %lx\n", winId(), tclId());
    }

    void TclLabelImpl::update(const RectF& bbox, const LabelProps& data)
    {
    }

    void TclLabelImpl::updateCoords(const RectF& bbox)
    {
        Rect rect = transform(bbox);

        sys_vgui("nui::label::move %lx %lx %d %d\n",
            winId(), tclId(), rect.left(), rect.top());
    }
}
}
