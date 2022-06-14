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

#include <algorithm>
#include <cstdint>

namespace ceammc {
namespace touchosc {
    Control::Control(const std::string& name, const std::string& type, int w, int h, int x, int y)
        : XmlNode("control")
        , osc_("")
        , type_(type)
        , name_(name)
    {
        setX(x);
        setY(y);
        setWidth(w);
        setHeight(h);
        setColor(COLOR_GREY);

        setAttribute("type", type_);
        setAttribute("name", base64_encode(name_));
    }

    void Control::setX(int x)
    {
        x_ = x;
        setAttribute("x", std::to_string(x_));
    }

    void Control::setY(int y)
    {
        y_ = y;
        setAttribute("y", std::to_string(y_));
    }

    void Control::setPos(int x, int y)
    {
        setX(x);
        setY(y);
    }

    void Control::setSize(int w, int h)
    {
        setWidth(w);
        setHeight(h);
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

    void Control::setColor(float r, float g, float b)
    {
        struct rgb {
            uint8_t r, g, b;
        };

        const static rgb colors[COLOR_MAX] {
            { 109, 22, 20 },
            { 25, 89, 33 },
            { 15, 45, 40 },
            { 56, 56, 20 },
            { 56, 38, 61 },
            { 51, 51, 51 },
            { 117, 58, 20 },
            { 56, 40, 2 },
            { 102, 2, 94 },
        };

        auto color_distance = [](const rgb c0, const rgb c1) -> int {
            auto dr = c0.r - c1.r;
            auto dg = c0.g - c1.g;
            auto db = c0.b - c1.b;
            return (dr * dr) + (dg * dg) + (db * db);
        };

        const uint8_t ur = r * 256;
        const uint8_t ug = g * 256;
        const uint8_t ub = b * 256;

        int min_dist = std::numeric_limits<int>::max();
        int color_idx = -1;

        for (int i = 0; i < COLOR_MAX; i++) {
            const auto dist = color_distance({ ur, ug, ub }, colors[i]);
            if (min_dist > dist) {
                min_dist = dist;
                color_idx = i;
            }
        }

        if (color_idx >= 0 && color_idx < COLOR_MAX)
            setColor(static_cast<Color>(color_idx));
    }
}
}
