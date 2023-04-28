/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_PLATFORM_WIN_H
#define CEAMMC_PLATFORM_WIN_H

#include <string>

#include "ceammc_platform.h"

namespace ceammc {
namespace platform {
    bool win_is_path_relative(const char* path);
    std::string win_basename(const char* path);
    std::string win_dirname(const char* path);
    bool win_fnmatch(const char* pattern, const char* str);
    bool win_path_exists(const char* path);
    bool mb_to_wch(const char* str, wchar_t** res);
    bool wch_to_mb(const wchar_t* str, char** res);
    bool win_mkdir(const char* path, int flags = -1);
    bool win_rmdir(const char* path);
    std::string win_home_directory();
    void win_sleep_ms(unsigned int ms);
    bool win_is_dir(const char* path);
    std::string win_current_working_directory();
    Either<NetAddressList, PlatformError> win_hostnametoip(const char* name, NetAddressType type);
    Either<int, PlatformError> win_fd_set_non_blocking(int fd);
    Either<bool, PlatformError> win_init_pipe(int fd[]);
    std::vector<std::string> win_net_ifaces_ip(NetAddressType type);
    bool win_net_socket_addr(int fd, std::string& addr);
}
}

#endif // CEAMMC_PLATFORM_WIN_H
