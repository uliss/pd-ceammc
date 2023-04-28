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
#include "ceammc_platform.h"
#include "ceammc_log.h"
#include "config.h"

#include "g_canvas.h"
extern "C" {
#include "m_imp.h"
}

#include "memrss.h"
#include "memsize.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef __WIN32
#define NS(f) win_##f
#include "ceammc_platform_win.h"
#else
#define NS(f) unix_##f
#include "ceammc_platform_unix.h"
#endif

#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#error <dirent.h> is missing...
#endif

namespace ceammc {
namespace platform {

    bool is_path_relative(const char* path)
    {
        return NS(is_path_relative(path));
    }

    const char* platform_name()
    {
#ifdef __APPLE__
        return "macosx";
#elif defined(__WIN32)
        return "windows";
#elif defined(__ANDROID__)
        return "android";
#elif defined(__linux__)
        return "linux";
#elif defined(__FreeBSD__)
        return "freebsd";
#elif defined(__NetBSD__)
        return "netbsd";
#elif defined(__OpenBSD__)
        return "openbsd";
#else
        return "???";
#endif
    }

    size_t memory_size()
    {
        return getMemorySize();
    }

    size_t memory_current_rss()
    {
        return getCurrentRSS();
    }

    size_t memory_peak_rss()
    {
        return getPeakRSS();
    }

    std::string basename(const char* path)
    {
        return NS(basename(path));
    }

    std::string dirname(const char* path)
    {
        return NS(dirname(path));
    }

    static bool findEnvVar(const std::string& str, std::string::size_type from,
        std::string* varname, std::string::size_type* begin, std::string::size_type* end)
    {
        std::string::size_type at1 = str.find_first_of('%', from);
        if (std::string::npos == at1)
            return false;

        std::string::size_type at2 = str.find_first_of('%', at1 + 1);
        if (std::string::npos == at2)
            return false;

        std::string::size_type len = at2 - at1;
        if (len < 2)
            return false;

        *begin = at1;
        *end = at2;
        *varname = str.substr(at1 + 1, len - 1);
        return true;
    }

    std::string expandenv(const char* str)
    {
        std::string s(str);
        std::string::size_type pos = 0;

        std::string::size_type at1 = 0;
        std::string::size_type at2 = 0;
        std::string varname;
        std::string res;

        while (findEnvVar(s, pos, &varname, &at1, &at2)) {
            res += s.substr(pos, at1 - pos);

            char* var = getenv(varname.c_str());
            // if env var not found
            if (!var) {
                // append unmodified
                res += s.substr(at1, at2 - at1 + 1);
            } else {
                res += var;
            }

            pos = at2 + 1;
        }

        res += s.substr(pos, std::string::npos);
        return res;
    }

    std::string get_env(const char* varname)
    {
        std::string res;
        char* env = ::getenv(varname);
        if (env)
            res = env;

        return res;
    }

    void set_env(const char* varname, const char* val)
    {
#ifdef HAVE_SETENV
        ::setenv(varname, val, 1);
#else
        std::string str(varname);
        str += '=';
        str += val;
        ::putenv(str.c_str());
#endif
    }

    bool fnmatch(const char* pattern, const char* str)
    {
        return NS(fnmatch)(pattern, str);
    }

    bool path_exists(const char* path)
    {
        return NS(path_exists)(path);
    }

    bool mkdir(const char* path, int flags)
    {
        return NS(mkdir)(path, flags);
    }

    bool rmdir(const char* path)
    {
        return NS(rmdir)(path);
    }

    std::string home_directory()
    {
        return NS(home_directory)();
    }

    std::string expand_tilde_path(const std::string& path)
    {
        if (!is_tilde_path(path.c_str()))
            return path;

        std::string res(path);
        return res.replace(0, 1, home_directory());
    }

    bool is_tilde_path(const char* path)
    {
        return (path[0] == '~' && (path[1] == '\0' || path[1] == '/'));
    }

    std::string find_in_std_path(t_canvas* cnv, const char* path)
    {
        std::string fname = path;
        if (is_tilde_path(path))
            fname = expand_tilde_path(path);

        char dirname[MAXPDSTRING], *filename;
        int fd = canvas_open(cnv, fname.c_str(), "", dirname, &filename, MAXPDSTRING, 1);
        if (fd < 0)
            return std::string();

        close(fd);

        std::string full_path(dirname);
        full_path += '/';
        full_path += filename;
        return full_path;
    }

    std::string strip_extension(const std::string& name)
    {
        std::string::size_type p = name.rfind('.');
        if (p != std::string::npos) {
            if (p == 0 || (p + 1 == name.length()))
                return name;
        }

        return name.substr(0, p);
    }

    bool remove(const char* path)
    {
        if (is_dir(path))
            return rmdir(path);
        else
            return ::remove(path) == 0;
    }

    bool is_dir(const char* path)
    {
        return NS(is_dir(path));
    }

    DirList list_directory(const char* path, const char* pattern)
    {
        std::vector<std::string> res;

        DIR* dir = opendir(path);
        if (dir == NULL) {
            LIB_ERR << "can't read directory: '" << path << "'. Error: " << strerror(errno);
            return boost::none;
        }

        const bool match = (pattern != 0 && pattern[0] != '\0');

        struct dirent* p_dirent;
        while ((p_dirent = readdir(dir)) != NULL) {
            // skip hidden files on UNIX
            if (p_dirent->d_name[0] == '.')
                continue;

            if (match) {
                if (platform::fnmatch(pattern, p_dirent->d_name))
                    res.push_back(p_dirent->d_name);
            } else {
                res.push_back(p_dirent->d_name);
            }
        }

        closedir(dir);

        return res;
    }

    std::string find_in_exernal_dir(t_object* obj, const char* path)
    {
        if (!is_path_relative(path))
            return path;

        char dirname[MAXPDSTRING], *filename;
        int fd = open_via_path(obj->te_g.g_pd->c_externdir->s_name, path, "", dirname, &filename, MAXPDSTRING, 1);
        if (fd < 0)
            return std::string();

        close(fd);

        std::string full_path(dirname);
        full_path += '/';
        full_path += filename;
        return full_path;
    }

    std::string make_abs_filepath_with_canvas(const t_canvas* cnv, const std::string& path)
    {
        if (path.empty() || path == "~" || path == "~/")
            return {};

        auto p = expand_tilde_path(path);
        if (!is_path_relative(p.c_str()))
            return p;

        if (cnv) {
            if (cnv && (cnv->gl_owner || cnv->gl_env)) {
                auto patch_dir = canvas_getdir(cnv);
                if (patch_dir && patch_dir->s_name) {
                    std::string dir(patch_dir->s_name);
                    dir += '/';
                    dir += p;

                    if (dir[0] == '~')
                        return expand_tilde_path(dir);

                    return dir;
                }
            }
        }

        return pd_user_directory() + "/" + p;
    }

    std::string pd_user_directory()
    {
        return home_directory() + "/Documents/Pd";
    }

    Either<NetAddressList, PlatformError> hostnametoip(const char* name, NetAddressType type)
    {
        return NS(hostnametoip(name, type));
    }

    PlatformError::PlatformError(int c, const char* s)
        : code(c)
        , msg(s)
    {
    }

    Either<int, PlatformError> fd_set_non_blocking(int fd)
    {
        return NS(fd_set_non_blocking(fd));
    }

    Either<bool, PlatformError> init_pipe(int fd[])
    {
        return NS(init_pipe(fd));
    }

    std::string current_working_directory()
    {
        return NS(current_working_directory());
    }

    std::vector<std::string> net_ifaces_ip(NetAddressType type)
    {
    return NS(net_ifaces_ip(type));
    }

}
}
