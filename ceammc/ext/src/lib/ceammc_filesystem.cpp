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
#include "ceammc_filesystem.h"

namespace ceammc {
namespace fs {
    Either<std::string> readFileContent(const char* path)
    {
        if (!platform::path_exists(path))
            return RuntimeError(fmt::format("file not exists: '{}'", path));

        std::ifstream ifs(path);
        if (!ifs)
            return RuntimeError(fmt::format("can't open file: '{}'", path));

        std::string str;

        // reserve size for string
        ifs.seekg(0, std::ios::end);
        str.reserve(ifs.tellg());
        ifs.seekg(0, std::ios::beg);

        // read to string
        str.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

        return str;
    }
}
}
