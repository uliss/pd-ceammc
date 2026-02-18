/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#ifndef PARSER_PATH_TEMPLATE_H
#define PARSER_PATH_TEMPLATE_H

#include <cstring>
#include <functional>
#include <string>

namespace ceammc {
namespace parser {
    enum DirectoryTemplate {
        None,
        Home,
        Audio,
        Video,
        Picture,
        Desktop,
        Document,
        Download,
        Tmp,
        Cwd,
        Unknown,
    };

    /**
     * finds directory template in the given path and returns it position
     * @param path - input path. Not NULL
     * @param begin - pointer to write begining of found template.
     * @param end - pointer to write end of found template.
     * @return detected template type
     */
    DirectoryTemplate path_get_dir_template(const char* path, const char** begin = nullptr, const char** end = nullptr);

    using path_dir_template_subst_cb = std::function<const char*(DirectoryTemplate t, const char* begin, const char* end)>;

    inline std::string path_dir_template_subst(const char* path, const path_dir_template_subst_cb& fn)
    {
        const char* begin = nullptr;
        const char* end = nullptr;
        auto type = path_get_dir_template(path, &begin, &end);
        if (type != DirectoryTemplate::None) {
            std::string res;
            res.reserve(std::strlen(path) + 64);
            res.assign(path, begin);
            res.append(fn(type, begin, end));
            res.append(end);
            return res;
        } else {
            return path;
        }
    }
} // namespace parser
} // namespace ceammc

#endif // PARSER_PATH_TEMPLATE_H
