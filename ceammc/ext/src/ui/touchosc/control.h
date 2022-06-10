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
#ifndef CONTROL_H
#define CONTROL_H

#include "xmlnode.h"

namespace ceammc {
namespace touchosc {

    class Control : public XmlNode {
        int x_, y_, w_, h_;
        Color color_;
        OscAttributes osc_;
        const std::string type_;
        const std::string& name_;

    public:
        Control(const std::string& name, const std::string& type, int w, int h, int x, int y);

        OscAttributes& osc() { return osc_; }
        const OscAttributes& osc() const { return osc_; }

        int x() const { return x_; }
        int y() const { return y_; }
        int width() const { return w_; }
        int height() const { return h_; }

        void setX(int x);
        void setY(int y);
        void setWidth(int w);
        void setHeight(int h);
        void setPos(int x, int y);
        void setSize(int w, int h);

        Color color() const { return color_; }
        void setColor(Color c);
        void setColor(float r, float g, float b);

        const std::string& type() const { return type_; }
        const std::string& name() const { return name_; }
    };
}
}

#endif // CONTROL_H
