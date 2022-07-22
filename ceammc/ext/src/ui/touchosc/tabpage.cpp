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
#include "tabpage.h"
#include "fmt/format.h"

#include <cstdlib>

namespace ceammc {
namespace touchosc {

    TabPage::TabPage(const std::string& name)
        : XmlNode("tabpage")
        , name_(name)
        , osc_(fmt::format("/{}", name_))
    {
        setName(name);
    }

    void TabPage::setName(const std::string& name)
    {
        name_ = name;
        setAttribute("name", base64_encode(name));
    }

    void TabPage::setLabel(const std::string& name)
    {
        name_ = name;
        setAttribute("la_t", base64_encode(name));
    }

    std::ostream& TabPage::printAttributes(std::ostream& os) const
    {
        XmlNode::printAttributes(os);
        os << ' ' << osc_;
        return os;
    }

    std::ostream& TabPage::printContent(std::ostream& os) const
    {
        for (auto& c : controls_)
            os << *c;

        return os;
    }

    void TabPage::append(ControlPtr&& ctl)
    {
        controls_.push_back(std::move(ctl));
    }

    void TabPage::layout()
    {
        int min_x = std::numeric_limits<int>::max();
        int min_y = std::numeric_limits<int>::max();
        int min_w = std::numeric_limits<int>::max();
        int min_h = std::numeric_limits<int>::max();

        // find padding
        for (auto& c : controls_) {
            if (min_x > c->x())
                min_x = c->x();

            if (min_y > c->y())
                min_y = c->y();

            if (min_h > c->height())
                min_h = c->height();

            if (min_w < c->width())
                min_w = c->width();
        }

        // remove extra padding
        for (auto& c : controls_) {
            c->setX(c->x() - min_x);
            c->setY(c->y() - min_y);
        }
    }
}
}
