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
#include "nui/view.h"
#include "test_nui.h"

using namespace ceammc::ui;

using TestFrameViewImpl = EmptyViewImplT<FrameProps>;
using TestFrameViewImplPtr = ViewImplPtr<FrameProps>;

TEST_CASE("nui", "[nui]")
{
    SECTION("init")
    {
        FrameView fv0(0, TestFrameViewImplPtr(new TestFrameViewImpl), 0, PointF(10, 20), SizeF(40, 50));

        REQUIRE(fv0.parent() == nullptr);
        REQUIRE(fv0.pos() == PointF(10, 20));
    }
}
