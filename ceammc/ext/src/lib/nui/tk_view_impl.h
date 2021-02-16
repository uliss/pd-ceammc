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
#ifndef TK_VIEW_IMPL_H
#define TK_VIEW_IMPL_H

#include "nui/view.h"

namespace ceammc {
namespace ui {

    template <typename Props>
    class TclViewImpl : public ViewImpl<Props> {
    public:
        TclViewImpl(const ViewId& view_id, PropId prop_id)
            : ViewImpl<Props>(view_id, prop_id)
        {
        }

        uint64_t tclId() const { return this->modelId() + this->propId(); }
    };

    class TclFrameImpl : public TclViewImpl<FrameProps> {
    public:
        TclFrameImpl(const ViewId& view_id, PropId prop_id);

        void create(const RectF& bbox, const FrameProps& data) final;
        void erase() final;
        void update(const RectF& bbox, const FrameProps& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    class TclHSliderImpl : public TclViewImpl<SliderProps> {
    public:
        TclHSliderImpl(const ViewId& view_id, PropId prop_id);

        void create(const RectF& bbox, const SliderProps& data) final;
        void erase() final;
        void update(const RectF& bbox, const SliderProps& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    class TclLabelImpl : public TclViewImpl<LabelProps> {
    public:
        TclLabelImpl(const ViewId& view_id, PropId prop_id);

        void create(const RectF& bbox, const LabelProps& data) final;
        void erase() final;
        void update(const RectF& bbox, const LabelProps& data) final;
        void updateCoords(const RectF& bbox) final;
    };

}
}

#endif // TK_VIEW_IMPL_H
