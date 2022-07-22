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

#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace ceammc {
namespace touchosc {

    std::string base64_encode(const std::string& str)
    {
        using namespace boost::archive::iterators;
        using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;

        auto tmp = std::string(It(std::begin(str)), It(std::end(str)));
        return tmp.append((3 - str.size() % 3) % 3, '=');
    }

    XmlNode::XmlNode(const std::string& tag)
        : tag_(tag)
    {
    }

    XmlNode::~XmlNode()
    {
    }

    std::ostream& XmlNode::printOpenTag(std::ostream& os) const
    {
        os << fmt::format("<{}", tag_);
        printAttributes(os) << '>';
        return os;
    }

    std::ostream& XmlNode::printContent(std::ostream& os) const
    {
        return os;
    }

    std::ostream& XmlNode::printCloseTag(std::ostream& os) const
    {
        os << fmt::format("</{}>", tag_);
        return os;
    }

    std::ostream& XmlNode::printAttributes(std::ostream& os) const
    {
        for (auto& a : attrs_)
            os << fmt::format(" {}=\"{}\"", a.first, a.second);

        return os;
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
        node.printOpenTag(os);
        node.printContent(os);
        node.printCloseTag(os);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const OscAttributes& attr)
    {
        os << fmt::format("osc_cs=\"{}\" scalef=\"{:f}\" scalet=\"{:f}\"",
            base64_encode(attr.path()), attr.from(), attr.to());
        return os;
    }

    OscAttributes::OscAttributes(const std::string& path, float from, float to)
        : path_(path)
        , from_(from)
        , to_(to)
    {
    }

}

}
