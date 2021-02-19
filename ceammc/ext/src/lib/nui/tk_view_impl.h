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

    class TclFrameImpl : public ViewImpl<FrameData> {
    public:
        void create(const RectF& bbox, const FrameData& data) final;
        void erase() final;
        void update(const RectF& bbox, const FrameData& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    class TclHSliderImpl : public ViewImpl<SliderData> {
    public:
        void create(const RectF& bbox, const SliderData& data) final;
        void erase() final;
        void update(const RectF& bbox, const SliderData& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    class TclLabelImpl : public ViewImpl<LabelData> {
    public:
        void create(const RectF& bbox, const LabelData& data) final;
        void erase() final;
        void update(const RectF& bbox, const LabelData& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    bool tcl_nui_init();

}
}

#endif // TK_VIEW_IMPL_H
