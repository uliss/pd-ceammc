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
#include "miniz.h"

#include <sstream>

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

    void Layout::clear()
    {
        tabs_.clear();
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

    int Layout::width() const
    {
        return (orient_ == LAYOUT_VERTICAL) ? width_ : height_;
    }

    int Layout::height() const
    {
        return (orient_ == LAYOUT_VERTICAL) ? height_ : width_;
    }

    std::ostream& Layout::printContent(std::ostream& os) const
    {
        for (auto& t : tabs_)
            os << *t;

        return os;
    }

    bool Layout::saveToFile(const std::string& path)
    {
        //        LIB_LOG << "miniz version: " << MZ_VERSION;

        std::ostringstream ss;
        ss << *this;
        const auto str = ss.str();
        const char* comment = "TouchOSC interface";

        // Add a new file to the archive. Note this is an IN-PLACE operation, so if it fails your archive is probably hosed (its central directory may not be complete) but it should be recoverable using zip -F or -FF. So use caution with this guy.
        // A more robust way to add a file to an archive would be to read it into memory, perform the operation, then write a new archive out to a temp file and then delete/rename the files.
        // Or, write a new archive to disk to a temp file, then delete/rename the files. For this test this API is fine.
        auto status = mz_zip_add_mem_to_archive_file_in_place(path.c_str(),
            "index.xml",
            str.data(), str.size(),
            comment, strlen(comment), MZ_BEST_SPEED);

        if (!status) {
            printf("mz_zip_add_mem_to_archive_file_in_place failed!\n");
            return false;
        }

        return true;
    }

    std::ostream& operator<<(std::ostream& os, const Layout& l)
    {
        os << R"(<?xml version="1.0" encoding="UTF-8"?>)";
        os << '\n';
        os << static_cast<const XmlNode&>(l);
        os << '\n';
        return os;
    }

    std::string to_string(const Layout& l)
    {
        std::ostringstream str;
        str << l;
        return str.str();
    }

}
}
