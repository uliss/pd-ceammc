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
#include "control.h"

namespace ceammc {
namespace touchosc {
    Control::Control(const std::string& type, int w, int h, int x, int y)
        : XmlNode("control")
        , osc_("")
        , type_(type)
    {
        setX(x);
        setY(y);
        setWidth(w);
        setHeight(h);
        setColor(Color::GREY);

        setAttribute("type", type_);
    }

    void Control::setX(int x)
    {
        x_ = x;
        setAttribute("x", std::to_string(x_));
    }

    void Control::setY(int y)
    {
        y_ = y;
        setAttribute("x", std::to_string(y_));
    }

    void Control::setWidth(int w)
    {
        w_ = w;
        setAttribute("w", std::to_string(w_));
    }

    void Control::setHeight(int h)
    {
        h_ = h;
        setAttribute("h", std::to_string(h_));
    }

    void Control::setColor(Color c)
    {
        static const char* colors[] = { "red", "green", "blue", "yellow", "purple", "grey", "orange", "brown", "pink" };

        color_ = c;
        setAttribute("color", colors[static_cast<int>(color_)]);
    }
}
}
