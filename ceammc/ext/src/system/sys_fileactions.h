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
#ifndef SYS_FILEACTIONS_H
#define SYS_FILEACTIONS_H

#include <spawn.h>
#include <stdexcept>

namespace ceammc {
namespace sys {
    // RAII
    struct FileActions {
        posix_spawn_file_actions_t fa;

        /**
         * Creates empty file actions object
         * @throw std::runtime_error on error
         */
        FileActions();

        ~FileActions() noexcept;

        /**
         * Add close action for specified fd in spawned process
         * @param fd - file descriptor
         * @throw std::runtime_error on error
         */
        void addClose(int fd);

        /**
         *  duplicate an existing file descriptor in spawned process
         */
        void addDup2(int in, int out);
    };
}
}

#endif // SYS_FILEACTIONS_H
