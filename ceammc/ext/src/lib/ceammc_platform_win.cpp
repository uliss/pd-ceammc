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
#include "ceammc_platform_win.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

#include <Shlwapi.h>
#include <Stringapiset.h>
#include <Userenv.h>
#include <Winsock2.h>
#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <ws2tcpip.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "config.h"

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

namespace ceammc {
namespace platform {

    bool win_is_path_relative(const char* path)
    {
        return PathIsRelative(path) == 1;
    }

    std::string win_basename(const char* path)
    {
#ifdef HAVE_LIBGEN_H
        char* p = strdup(path);
        char* bn = ::basename(p);
        if (!bn)
            return "";

        std::string res(bn);
        free(p);
        return res;
#else
        char* buf = strdup(path);
        PathStripPath(buf);
        std::string res(buf);
        free(buf);
        return res;
#endif
    }

    std::string win_dirname(const char* path)
    {
#ifdef HAVE_LIBGEN_H
        char* p = strdup(path);
        char* dir = ::dirname(p);
        if (!dir)
            return "";

        std::string res(dir);
        free(p);
        return res;
#else
        char* buf = strdup(path);
        PathRemoveFileSpec(buf);
        std::string res(buf);
        free(buf);
        return res;
#endif
    }

    bool mb_to_wch(const char* str, wchar_t** res)
    {
        int str_len = strlen(str) + 1;
        int wstr_len = MultiByteToWideChar(CP_UTF8, 0, str, str_len, 0, 0);
        if (wstr_len == 0)
            return false;

        wchar_t* wstr = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * wstr_len));
        int rc = MultiByteToWideChar(CP_UTF8, 0, str, str_len, wstr, wstr_len);
        if (rc == 0) {
            free(wstr);
            return false;
        } else {
            *res = wstr;
            return true;
        }
    }

    bool win_fnmatch(const char* pattern, const char* str)
    {
        wchar_t* wstr = 0;
        wchar_t* wpattern = 0;

        if (!mb_to_wch(str, &wstr) || !mb_to_wch(pattern, &wpattern))
            return false;

        BOOL res = PathMatchSpecW(wstr, wpattern);
        free(wstr);
        free(wpattern);
        return res;
    }

    bool win_path_exists(const char* path)
    {
        wchar_t* wpath = 0;
        if (!mb_to_wch(path, &wpath))
            return false;

        BOOL res = PathFileExistsW(wpath);
        free(wpath);
        return res;
    }

    bool win_mkdir(const char* path, int flags)
    {
        wchar_t* wpath = 0;
        if (!mb_to_wch(path, &wpath))
            return false;

        int err = _wmkdir(wpath);
        free(wpath);
        return err == 0;
    }

    bool win_rmdir(const char* path)
    {
        wchar_t* wpath = 0;
        if (!mb_to_wch(path, &wpath))
            return false;

        int err = _wrmdir(wpath);
        free(wpath);
        return err == 0;
    }

    bool win_is_dir(const char* path)
    {
        auto attrs = GetFileAttributesA(path);
        if (attrs == INVALID_FILE_ATTRIBUTES)
            return false;

        return attrs & FILE_ATTRIBUTE_DIRECTORY;
    }

    std::string win_home_directory()
    {
        WCHAR buf[MAX_PATH] = { 0 };

        // We need a process with query permission set
        HANDLE token = 0;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
            return "";

        // Returns a path like C:/Documents and Settings/nibu if my user name is nibu
        DWORD buf_size = MAX_PATH;
        GetUserProfileDirectoryW(token, buf, &buf_size);
        CloseHandle(token);

        char* path;
        if (!wch_to_mb(buf, &path))
            return "";

        std::string home(path);
        free(path);
        return home;
    }

    bool wch_to_mb(const wchar_t* wstr, char** res)
    {
        const int wstr_len = wcslen(wstr) + 1;
        int str_len = WideCharToMultiByte(CP_ACP, 0, wstr, wstr_len, 0, 0, 0, 0);
        if (str_len == 0)
            return false;

        char* str = static_cast<char*>(malloc(str_len));
        int rc = WideCharToMultiByte(CP_ACP, 0, wstr, wstr_len, str, str_len, 0, 0);
        if (!rc) {
            free(str);
            return false;
        } else {
            *res = str;
            return true;
        }
    }

    void win_sleep_ms(unsigned int ms)
    {
        Sleep(ms);
    }

    std::string win_current_working_directory()
    {
        char* buffer;

        // Get the current working directory:
        if ((buffer = _getcwd(NULL, 0)) == NULL) {
            perror("_getcwd error");
            return std::string();
        } else {
            std::string res(buffer);
            free(buffer);
            return res;
        }
    }

    Either<NetAddressList, PlatformError> win_hostnametoip(const char* name, NetAddressType type)
    {
        struct addrinfo* result = NULL;
        struct addrinfo hints;
        char address[INET6_ADDRSTRLEN];

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = (type == ADDR_IPV4) ? PF_INET : PF_INET6;
        hints.ai_protocol = IPPROTO_TCP;

        int error = getaddrinfo(name, NULL, &hints, &result);

        if (error)
            return PlatformError(error, gai_strerror(error));

        NetAddressList res;

        for (auto ptr = result; ptr != NULL; ptr = ptr->ai_next) {
            int error = getnameinfo(ptr->ai_addr,
                ptr->ai_addrlen,
                address, sizeof(address), NULL, 0, NI_NUMERICHOST);

            if (error)
                return PlatformError(error, gai_strerror(error));

            res.push_back(address);
        }

        freeaddrinfo(result);
        return res;
    }

    Either<int, PlatformError> win_fd_set_non_blocking(int fd)
    {
        return 0;

        u_long arg = 1;
        int rc = ioctlsocket(fd, FIONBIO, &arg);
        if (rc != NO_ERROR)
            return PlatformError(rc, "fcntl failed");

        return rc;
    }

    Either<bool, PlatformError> win_init_pipe(int fd[])
    {
        int rc = _pipe(fd, 256, O_BINARY);
        if (rc == -1)
            return PlatformError(rc, "asd");

        return true;
    }

    std::vector<std::string> win_net_ifaces_ip(NetAddressType type)
    {
        return {};
    }

    bool win_net_socket_addr(int fd, std::string& addr)
    {
        return false;
    }

}
}
