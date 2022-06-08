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
#include "xmlnode.h"
#include "fmt/format.h"

namespace ceammc {
namespace touchosc {
    XmlNode::XmlNode(const std::string& name)
        : name_(name)
    {
    }

    bool XmlNode::hasAttribute(const std::string& key) const
    {
        return attrs_.find(key) != attrs_.end();
    }

    bool XmlNode::getAttribute(const std::string& key, std::string& value) const
    {
        auto it = attrs_.find(key);
        if (it == attrs_.end()) {
            return false;
        } else {
            value = it->second;
            return true;
        }
    }

    void XmlNode::setAttribute(const std::string& key, const std::string& value)
    {
        attrs_[key] = value;
    }

    void XmlNode::removeAttribute(const std::string& key)
    {
        attrs_.erase(key);
    }

    std::ostream& operator<<(std::ostream& os, const XmlNode& node)
    {
        os << fmt::format("<{}", node.name());
        // print attrs
        for (auto& a : node.attrs())
            os << fmt::format(" {}=\"{}\"", a.first, a.second);

        os << '>';

        os << fmt::format("</{}>", node.name());
        return os;
    }

}

}
