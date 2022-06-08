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
#ifndef XMLNODE_H
#define XMLNODE_H

#include <iostream>
#include <map>
#include <string>

namespace ceammc {
namespace touchosc {
    using XmlAttributes = std::map<std::string, std::string>;

    class XmlNode {
        const std::string name_;
        XmlAttributes attrs_;

    public:
        XmlNode(const std::string& name);

        const std::string& name() const { return name_; }
        bool hasAttribute(const std::string& key) const;
        bool getAttribute(const std::string& key, std::string& value) const;
        void setAttribute(const std::string& key, const std::string& value);
        void removeAttribute(const std::string& key);
        void setAttributes(const XmlAttributes& attrs) { attrs_ = attrs; }
        const XmlAttributes& attrs() const { return attrs_; }
    };

    std::ostream& operator<<(std::ostream& os, const XmlNode& node);
}
}

#endif // XMLNODE_H
