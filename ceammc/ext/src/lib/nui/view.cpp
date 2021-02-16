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
#include "view.h"

namespace ceammc {
namespace ui {

    EmptyViewImpl::EmptyViewImpl()
        : ViewImpl<EmptyData>({ 0, 0 }, 0)
    {
    }

    void EmptyViewImpl::create(const RectF& /*bbox*/, const EmptyData& /*data*/) { }

    void EmptyViewImpl::erase() { }

    void EmptyViewImpl::update(const RectF& /*bbox*/, const EmptyData& /*data*/) { }

    void EmptyViewImpl::updateCoords(const RectF& /*bbox*/) { }

    ViewBase::ViewBase(const PointF& pos, const SizeF& sz)
        : pos_(pos)
        , size_(sz)
        , parent_(nullptr)
    {
    }

    ViewBase::~ViewBase() { }

    void ViewBase::layout() { }

}
}
