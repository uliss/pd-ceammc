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

    enum class Color {
        RED,
        GREEN,
        BLUE,
        YELLOW,
        PURPLE,
        GREY,
        ORANGE,
        BROWN,
        PINK,
    };

    class OscAttributes {
        std::string path_;
        float from_, to_;

    public:
        OscAttributes(const std::string& path, float from = 0, float to = 1);

        float from() const { return from_; }
        float to() const { return to_; }
        const std::string& path() const { return path_; }
        void setPath(const std::string& str) { path_ = str; }
    };

    class XmlNode {
        const std::string tag_;
        XmlAttributes attrs_;

    public:
        XmlNode(const std::string& tag);

        const std::string& tag() const { return tag_; }
        const XmlAttributes& attrs() const { return attrs_; }

    protected:
        bool hasAttribute(const std::string& key) const;
        bool getAttribute(const std::string& key, std::string& value) const;
        void setAttribute(const std::string& key, const std::string& value);
        void removeAttribute(const std::string& key);
        void setAttributes(const XmlAttributes& attrs) { attrs_ = attrs; }
    };

    std::string base64_encode(const std::string& str);
    std::ostream& operator<<(std::ostream& os, const XmlNode& node);
    std::ostream& operator<<(std::ostream& os, const OscAttributes& osc);
}
}

#endif // XMLNODE_H
