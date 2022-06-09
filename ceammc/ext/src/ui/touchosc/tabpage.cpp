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
}
}
