/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include <utility>
#include <vector>

#include "tabpage.h"
#include "xmlnode.h"

namespace ceammc {
namespace touchosc {

    using TabPagePtr = std::unique_ptr<TabPage>;
    using TabVec = std::vector<TabPagePtr>;

    enum LayoutMode {
        LAYOUT_IPHONE = 0,
        LAYOUT_IPAD,
        LAYOUT_IPHONE5,
        LAYOUT_IPHONE6S,
        LAYOUT_IPHONE6PLUS,
        LAYOUT_IPADPRO,
        LAYOUT_CUSTOM,
    };

    enum LayoutOrientation {
        LAYOUT_VERTICAL,
        LAYOUT_HORIZONTAL
    };

    class Layout : public XmlNode {
        TabVec tabs_;
        LayoutMode mode_;
        int width_, height_;
        LayoutOrientation orient_;

    public:
        Layout();

        void append(TabPagePtr&& ptr);

        LayoutMode mode() const { return mode_; }
        void setLayoutMode(LayoutMode mode, int w = 0, int h = 0);

        LayoutOrientation orientation() const { return orient_; }
        void setOrientation(LayoutOrientation orient);

        int width() const;
        int height() const;
        std::pair<int, int> size() const { return { width(), height() }; }

        const TabVec& tabs() const { return tabs_; }

        std::ostream& printContent(std::ostream& os) const;
    };

    std::ostream& operator<<(std::ostream& os, const Layout& l);
    std::string to_string(const Layout& l);
}
}

#endif // LAYOUT_H
