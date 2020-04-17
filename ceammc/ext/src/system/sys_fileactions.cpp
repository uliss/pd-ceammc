/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "sys_fileactions.h"

#include <cstring>

namespace ceammc {
namespace sys {

    FileActions::FileActions()
    {
        int rc = posix_spawn_file_actions_init(&fa);
        if (rc != 0)
            throw std::runtime_error(strerror(rc));
    }

    FileActions::~FileActions() noexcept
    {
        posix_spawn_file_actions_destroy(&fa);
    }

    void FileActions::addClose(int fd)
    {
        int rc = posix_spawn_file_actions_addclose(&fa, fd);
        if (rc != 0)
            throw std::runtime_error(strerror(rc));
    }

    void FileActions::addDup2(int in, int out)
    {
        int rc = posix_spawn_file_actions_adddup2(&fa, in, out);
        if (rc != 0)
            throw std::runtime_error(strerror(rc));
    }
}
}
