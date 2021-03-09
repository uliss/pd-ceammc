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
#ifndef LAYOUT_H
#define LAYOUT_H

#include <memory>
#include <vector>

namespace ceammc {
namespace ui {

    class ModelViewBase;
    using ViewPtr = std::unique_ptr<ModelViewBase>;
    using ViewList = std::vector<ViewPtr>;

    class LayoutBase {
    public:
        virtual void doLayout(ViewList&) { }
    };

    class HLayout : public LayoutBase {
        float space_ { 0 };

    public:
        HLayout(float space = 5);
        virtual void doLayout(ViewList& items) override;

        float space() const { return space_; }
        void setSpace(float space) { space_ = space; }
    };

    class VLayout : public LayoutBase {
        float space_ { 0 };

    public:
        VLayout(float space = 5);
        virtual void doLayout(ViewList& items) override;
        float space() const { return space_; }
        void setSpace(float space) { space_ = space; }
    };

    using LayoutPtr = std::unique_ptr<LayoutBase>;
}
}

#endif // LAYOUT_H
