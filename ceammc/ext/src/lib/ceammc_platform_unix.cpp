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
#include "ceammc_platform_unix.h"

#include <cstdlib>
#include <cstring>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <ifaddrs.h>
#include <iostream>
#include <libgen.h>
#include <limits.h>
#include <netdb.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace ceammc {
namespace platform {

    bool unix_is_path_relative(const char* path)
    {
        return path[0] != '/';
    }

    std::string unix_basename(const char* path)
    {
        char* p = strdup(path);
        char* bn = ::basename(p);
        if (!bn)
            return "";

        std::string res(bn);
        free(p);
        return res;
    }

    std::string unix_dirname(const char* path)
    {
        char* p = strdup(path);
        char* dir = ::dirname(p);
        if (!dir)
            return "";

        std::string res(dir);
        free(p);
        return res;
    }

    bool unix_fnmatch(const char* pattern, const char* str)
    {
        return ::fnmatch(pattern, str, 0) == 0;
    }

    bool unix_path_exists(const char* path)
    {
        return ::access(path, R_OK) == 0;
    }

    bool unix_mkdir(const char* path, int flags)
    {
        if (flags < 0)
            flags = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;

        if (::mkdir(path, static_cast<mode_t>(flags)) == 0)
            return true;

        std::cerr << "[unix_mkdir] error: " << strerror(errno) << "\n";
        return false;
    }

    bool unix_rmdir(const char* path)
    {
        return ::rmdir(path) == 0;
    }

    std::string unix_home_directory()
    {
        const char* homedir = 0;

        if ((homedir = ::getenv("HOME")) == NULL) {
            homedir = getpwuid(getuid())->pw_dir;
        }

        return homedir ? homedir : "";
    }

    bool unix_is_dir(const char* path)
    {
        struct stat statbuf;
        if (::stat(path, &statbuf) != -1) {
            if (S_ISDIR(statbuf.st_mode))
                return true;
            else
                return false;
        }
        return false;
    }

    Either<NetAddressList, PlatformError> unix_hostnametoip(const char* name, NetAddressType type)
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

    Either<int, PlatformError> unix_fd_set_non_blocking(int fd)
    {
        int rc = fcntl(fd, F_SETFL, O_NONBLOCK);
        if (rc == -1)
            return PlatformError(errno, strerror(errno));

        return rc;
    }

    Either<bool, PlatformError> unix_init_pipe(int fd[])
    {
        int rc = ::pipe(fd);
        if (rc == -1)
            return PlatformError(errno, strerror(errno));
        else
            return true;
    }

    std::string unix_current_working_directory()
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            return cwd;

        perror("getcwd() error");
        return std::string();
    }

    bool unix_is_file(const char* path)
    {
        struct stat statbuf;
        if (::stat(path, &statbuf) != -1) {
            if (S_ISREG(statbuf.st_mode))
                return true;
            else
                return false;
        }
        return false;
    }

    std::vector<std::string> unix_net_ifaces_ip(NetAddressType type)
    {
        std::vector<std::string> res;
        ifaddrs* ifaddr;

        if (getifaddrs(&ifaddr) == -1)
            return res;

        char host[INET6_ADDRSTRLEN];

        for (auto ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr)
                continue;

            auto addr = ifa->ifa_addr;
            auto family = addr->sa_family;

            switch (family) {
            case AF_INET:
                if (type != ADDR_IPV4 && type != ADDR_IPANY)
                    continue;

                if (inet_ntop(family, &((struct sockaddr_in*)addr)->sin_addr, host, INET6_ADDRSTRLEN))
                    res.push_back(host);
                break;
            case AF_INET6:
                if (type != ADDR_IPV6 && type != ADDR_IPANY)
                    continue;

                if (inet_ntop(family, &((struct sockaddr_in6*)addr)->sin6_addr, host, INET6_ADDRSTRLEN))
                    res.push_back(host);
                break;
            default:
                continue;
            }
        }

        freeifaddrs(ifaddr);

        return res;
    }

    bool unix_net_socket_addr(int fd, std::string& res)
    {
        struct sockaddr_storage addr = { 0 };
        socklen_t len = sizeof(addr);

        if (getsockname(fd, (struct sockaddr*)(&addr), &len) < 0) {
            pd_error(nullptr, "getsockname: %s", strerror(errno));
            return false;
        }

        char host[INET6_ADDRSTRLEN];

        switch (addr.ss_family) {
        case AF_INET:
            if (inet_ntop(addr.ss_family, &((struct sockaddr_in*)&addr)->sin_addr, host, INET6_ADDRSTRLEN)) {
                res = host;
                return true;
            }

            break;
        case AF_INET6:
            if (inet_ntop(addr.ss_family, &((struct sockaddr_in6*)&addr)->sin6_addr, host, INET6_ADDRSTRLEN)) {
                res = host;
                return true;
            }
            break;
        default:
            break;
        }

        return false;
    }
}
}
