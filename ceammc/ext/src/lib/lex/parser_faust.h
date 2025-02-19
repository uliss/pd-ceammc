/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef PARSER_FAUST_H
#define PARSER_FAUST_H

#include <functional>
#include <string>

namespace ceammc {
namespace parser {

    using FaustStyleMenuCb = std::function<void(const std::string&, int)>;
    bool parse_faust_style_menu(const char* str, FaustStyleMenuCb cb);

}
}

#endif // PARSER_FAUST_H
