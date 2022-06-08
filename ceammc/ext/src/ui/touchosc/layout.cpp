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
#include "layout.h"

namespace ceammc {
namespace touchosc {
    Layout::Layout()
        : XmlNode("layout")
    {
        setAttribute("version", "17");
        setLayoutMode(LAYOUT_IPHONE);
        setOrientation(LAYOUT_VERTICAL);
    }

    void Layout::append(TabPagePtr&& ptr)
    {
        tabs_.push_back(std::move(ptr));
    }

    void Layout::setLayoutMode(LayoutMode mode, int w, int h)
    {
        removeAttribute("w");
        removeAttribute("h");

        switch (mode) {
        case LAYOUT_IPHONE:
            width_ = 320;
            height_ = 480;
            setAttribute("mode", "0");
            break;
        case LAYOUT_IPAD:
            width_ = 768;
            height_ = 1024;
            setAttribute("mode", "1");
            break;
        case LAYOUT_IPHONE5:
            width_ = 320;
            height_ = 568;
            setAttribute("mode", "2");
            break;
        case LAYOUT_IPHONE6S:
            width_ = 375;
            height_ = 667;
            setAttribute("mode", "3");
            setAttribute("w", std::to_string(width_));
            setAttribute("h", std::to_string(height_));
            break;
        case LAYOUT_IPHONE6PLUS:
            width_ = 414;
            height_ = 736;
            setAttribute("mode", "3");
            setAttribute("w", std::to_string(width_));
            setAttribute("h", std::to_string(height_));
            break;
        case LAYOUT_IPADPRO:
            width_ = 1024;
            height_ = 1366;
            setAttribute("mode", "3");
            setAttribute("w", std::to_string(width_));
            setAttribute("h", std::to_string(height_));
            break;
        case LAYOUT_CUSTOM:
            width_ = w;
            height_ = h;
            setAttribute("mode", "3");
            setAttribute("w", std::to_string(width_));
            setAttribute("h", std::to_string(height_));
            break;
        default:
            return;
        }

        mode_ = mode;
    }

    void Layout::setOrientation(LayoutOrientation orient)
    {
        orient_ = orient;
        setAttribute("orientation", orient_ == LAYOUT_VERTICAL ? "horizontal" : "vertical");
    }
}
}
