#include "catch.hpp"
#include "m_pd.h"

#include "ceammc_canvas.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "config.h"

#include <cstdlib>
#include <ctime>
#include <cwchar>
#include <fstream>
#include <string>

#ifdef __WIN32
#include "ceammc_platform_win.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

#define THIS_FILE PROJECT_SOURCE_DIR "/ceammc/ext/tests/cells/test_platform.cpp"

using namespace ceammc::platform;

#define basename_(str) ceammc::platform::basename(str)
#define dirname_(str) ceammc::platform::dirname(str)

TEST_CASE("ceammc::platform", "[ceammc::lib]")
{
    // strange link fix: do not remove!
    t_symbol* t = gensym("test");

    SECTION("platform_name")
    {
#ifdef __APPLE__
        REQUIRE(std::string(platform_name()) == "macosx");
#endif

#ifdef __WIN32
        REQUIRE(std::string(platform_name()) == "windows");
#endif

#ifdef __linux__
        REQUIRE(std::string(platform_name()) == "linux");
#endif

#ifdef __FreeBSD__
        REQUIRE(std::string(platform_name()) == "freebsd");
#endif

#ifdef __NetBSD__
        REQUIRE(std::string(platform_name()) == "netbsd");
#endif

#ifdef __OpenBSD__
        REQUIRE(std::string(platform_name()) == "openbsd");
#endif
    }

    SECTION("platform_name")
    {
#ifdef __WIN32
        REQUIRE_FALSE(is_path_relative("C:\\test.exe"));
        REQUIRE_FALSE(is_path_relative("\\test.exe"));
        REQUIRE_FALSE(is_path_relative("C:/test.exe"));
        REQUIRE(is_path_relative("/test.exe")); // NB(!)
        REQUIRE(is_path_relative("test.exe"));
        REQUIRE(is_path_relative("./test.exe"));
        REQUIRE(is_path_relative("../test.exe"));
#else
        REQUIRE_FALSE(is_path_relative("/"));
        REQUIRE_FALSE(is_path_relative("/test"));
        REQUIRE(is_path_relative("."));
        REQUIRE(is_path_relative(".."));
        REQUIRE(is_path_relative("test.txt"));
        REQUIRE(is_path_relative("./test"));
#endif
    }

    SECTION("basename")
    {
        REQUIRE(basename_("test.pd") == "test.pd");
        REQUIRE(basename_("/test.pd") == "test.pd");
        REQUIRE(basename_("./test.pd") == "test.pd");
        REQUIRE(basename_("../test.pd") == "test.pd");
        REQUIRE(basename_("lib/test.pd") == "test.pd");
        REQUIRE(basename_("/lib/test.pd") == "test.pd");

        REQUIRE(basename_("//dir") == "dir");
        REQUIRE(basename_("..") == "..");
        REQUIRE(basename_(".") == ".");

#ifdef __WIN32
        REQUIRE(basename_("c:\\dir\\file.txt") == "file.txt");
        REQUIRE(basename_("c:/dir/file.txt") == "file.txt");
        REQUIRE(basename_("c:/dir1/dir2") == "dir2");
        REQUIRE(basename_("c:\\dir1\\dir2") == "dir2");
#if defined(__MINGW32__)
        REQUIRE(basename_("C:/dir/") == "dir");
        REQUIRE(basename_("C:\\") == "/");
        REQUIRE(basename_("C:/") == "/");
        REQUIRE(basename_("A:") == ".");
#else
        REQUIRE(basename_("C:/dir/") == "dir/");
        REQUIRE(basename_("C:\\") == "C:\\");
        REQUIRE(basename_("C:/") == "C:/");
        REQUIRE(basename_("A:") == "A:");
#endif
#else
        REQUIRE(basename_("/////") == "/");
        REQUIRE(basename_("/dir/") == "dir");
        REQUIRE(basename_("") == ".");
#endif
    }

    SECTION("dirname")
    {
        REQUIRE(dirname_("lib/test.pd") == "lib");
        REQUIRE(dirname_("/lib/test.pd") == "/lib");
        REQUIRE(dirname_("") == ".");
        REQUIRE(dirname_("/") == "/");
        REQUIRE(dirname_("/dir") == "/");
        REQUIRE(dirname_("/dir/") == "/");
        REQUIRE(dirname_("/dir////") == "/");

#if defined(__WIN32) && !defined(__MINGW32__)
        REQUIRE(dirname_("test.pd") == "");
        REQUIRE(dirname_("/test.pd") == "");
        REQUIRE(dirname_("\\test.pd") == "\\");
#else
        REQUIRE(dirname_("test.pd") == ".");
        REQUIRE(dirname_("/test.pd") == "/");
        REQUIRE(dirname_("./test.pd") == ".");
        REQUIRE(dirname_("../test.pd") == "..");
#endif
    }

    SECTION("expandenv")
    {
        REQUIRE(expandenv("test") == "test");
        REQUIRE(expandenv("%test") == "%test");
        REQUIRE(expandenv("test%") == "test%");
        REQUIRE(expandenv("%test%") == "%test%");
        REQUIRE(expandenv("%TEST%/file.txt") == "%TEST%/file.txt");
        REQUIRE(expandenv("%TEST%%VAR%") == "%TEST%%VAR%");
        REQUIRE(expandenv("%%") == "%%");
        REQUIRE(expandenv("%A%") == "%A%");

        ceammc::platform::set_env("TEST", "/some/path");
        REQUIRE(expandenv("%TEST%/file.txt") == "/some/path/file.txt");
        REQUIRE(expandenv("%TEST%%VAR%") == "/some/path%VAR%");
        REQUIRE(expandenv("%TEST%%") == "/some/path%");
        REQUIRE(expandenv("%TEST% ...") == "/some/path ...");
        REQUIRE(expandenv(".. %TEST% ...") == ".. /some/path ...");

        ceammc::platform::set_env("VAR", "!!!!");
        REQUIRE(expandenv("%TEST%%VAR%") == "/some/path!!!!");
        REQUIRE(expandenv(",, %TEST% - %VAR% ...") == ",, /some/path - !!!! ...");
    }

    SECTION("fnmatch")
    {
        using namespace ceammc::platform;

        REQUIRE(fnmatch("*", "test.txt"));
        REQUIRE(fnmatch("*.txt", "test.txt"));
        REQUIRE(fnmatch("*", ".."));
        REQUIRE(fnmatch("*.wav", "test.wav"));
        REQUIRE_FALSE(fnmatch("*.wav", "a.mp3"));

        REQUIRE(fnmatch("????.wav", "test.wav"));
        REQUIRE(fnmatch("????.wav", "fest.wav"));
        REQUIRE_FALSE(fnmatch("???.wav", "test.wav"));

#ifndef __WIN32
        REQUIRE_FALSE(fnmatch("[abcd].wav", "a.mp3"));
        REQUIRE(fnmatch("[abc].mp3", "a.mp3"));
        REQUIRE(fnmatch("[abc].mp3", "b.mp3"));
        REQUIRE(fnmatch("[abc].mp3", "c.mp3"));
        REQUIRE(fnmatch("[!d].mp3", "a.mp3"));
        REQUIRE_FALSE(fnmatch("[abc].mp3", "d.mp3"));
        REQUIRE_FALSE(fnmatch("[!d].mp3", "d.mp3"));
#endif
    }

    SECTION("mb_to_wch")
    {
#ifdef __WIN32
        wchar_t* wstr = 0;
        // ANSI
        REQUIRE(mb_to_wch("test", &wstr));
        REQUIRE(wstr != 0);
        REQUIRE(wcscmp(wstr, L"test") == 0);
        free(wstr);
        // UTF-8
        wstr = 0;
        REQUIRE(mb_to_wch("тест", &wstr));
        REQUIRE(wstr != 0);
        REQUIRE(wcscmp(wstr, L"тест") == 0);
        free(wstr);
#endif
    }

    SECTION("path_exists")
    {
        using namespace ceammc::platform;
        REQUIRE(path_exists(THIS_FILE));
        REQUIRE(path_exists(TEST_DATA_DIR));
        REQUIRE_FALSE(path_exists(TEST_DATA_DIR "not-exists"));

#ifdef __APPLE__
        REQUIRE_FALSE(path_exists("/var/root"));
#endif

#ifdef __WIN32
        ceammc::platform::mkdir("platform_test");
        REQUIRE(path_exists("platform_test"));
        ceammc::platform::rmdir("platform_test");

        // UTF-8 test
        ceammc::platform::mkdir("тест");
        REQUIRE(path_exists("тест"));
        ceammc::platform::rmdir("тест");
#endif
    }

    SECTION("home_directory")
    {
#ifdef __APPLE__
        REQUIRE(ceammc::platform::home_directory().substr(0, 6) == "/Users");

        unsetenv("HOME");
        REQUIRE(ceammc::platform::home_directory().substr(0, 6) == "/Users");
#endif

#ifdef __WIN32
        REQUIRE(ceammc::platform::home_directory().substr(0, 3) == "C:\\");
#endif
    }

    SECTION("expand_tilde_path")
    {
        REQUIRE(ceammc::platform::expand_tilde_path("") == "");
        REQUIRE(ceammc::platform::expand_tilde_path("/full/path") == "/full/path");
        REQUIRE(ceammc::platform::expand_tilde_path("relative path") == "relative path");
        REQUIRE(ceammc::platform::expand_tilde_path("./~") == "./~");

#ifdef __APPLE__
        REQUIRE(ceammc::platform::expand_tilde_path("~/").substr(0, 7) == "/Users/");
        REQUIRE(ceammc::platform::expand_tilde_path("~").substr(0, 7) == "/Users/");
        REQUIRE(ceammc::platform::expand_tilde_path("~1") == "~1");
        REQUIRE(ceammc::platform::expand_tilde_path("~ABC") == "~ABC");
        REQUIRE(ceammc::platform::expand_tilde_path("~/ABC") == home_directory() + "/ABC");
#endif

#ifdef __WIN32
        REQUIRE(ceammc::platform::expand_tilde_path("~/").substr(0, 3) == "C:\\");
#endif
    }

    SECTION("find_in_std_path")
    {
        REQUIRE(ceammc::platform::find_in_std_path(0, "test").empty());
        ceammc::CanvasPtr cnv = ceammc::PureData::instance().createTopCanvas(TEST_DATA_DIR "/empty");
        REQUIRE(cnv);
        REQUIRE(cnv->pd_canvas());
        REQUIRE(ceammc::platform::find_in_std_path(cnv->pd_canvas(), "test").empty());
        REQUIRE(ceammc::platform::find_in_std_path(cnv->pd_canvas(), "snd_mono_48k.wav")
            == TEST_DATA_DIR "/snd_mono_48k.wav");

        // full path
        REQUIRE(ceammc::platform::find_in_std_path(cnv->pd_canvas(), TEST_DATA_DIR "/snd_mono_48k.wav") == TEST_DATA_DIR "/snd_mono_48k.wav");
    }

    SECTION("strip extension")
    {
        REQUIRE(ceammc::platform::strip_extension("") == "");
        REQUIRE(ceammc::platform::strip_extension(".") == ".");
        REQUIRE(ceammc::platform::strip_extension(".a") == ".a");
        REQUIRE(ceammc::platform::strip_extension(".atest") == ".atest");
        REQUIRE(ceammc::platform::strip_extension(".file.pd") == ".file");
        REQUIRE(ceammc::platform::strip_extension("file.pd") == "file");
        REQUIRE(ceammc::platform::strip_extension("file.1.2.3.4.pd") == "file.1.2.3.4");
    }

    SECTION("mkdir/rmdir")
    {
        const char* DIR = "./123";
        REQUIRE(ceammc::platform::mkdir(DIR));
        REQUIRE(ceammc::platform::path_exists(DIR));
        REQUIRE(ceammc::platform::is_dir(DIR));

        // double creation
        REQUIRE_FALSE(ceammc::platform::mkdir(DIR));

        // remove
        REQUIRE(ceammc::platform::rmdir(DIR));
        REQUIRE_FALSE(ceammc::platform::rmdir("./dir-not-exists"));
        REQUIRE_FALSE(ceammc::platform::is_dir(DIR));
    }

    SECTION("remove")
    {
        REQUIRE(ceammc::platform::mkdir("./example_path"));
        REQUIRE(ceammc::platform::remove("./example_path"));
        REQUIRE_FALSE(ceammc::platform::remove("./example_path"));

        // create file
        {
            std::ofstream f("test.file");
        }

        REQUIRE(ceammc::platform::path_exists("test.file"));
        REQUIRE(ceammc::platform::remove("test.file"));
    }

    SECTION("make_abs_path_with_canvas")
    {
        REQUIRE(make_abs_filepath_with_canvas(nullptr, "") == "");
        REQUIRE(make_abs_filepath_with_canvas(nullptr, "abc.json") == home_directory() + "/Documents/Pd/abc.json");
        REQUIRE(make_abs_filepath_with_canvas(nullptr, "~/1.json") == home_directory() + "/1.json");
        REQUIRE(make_abs_filepath_with_canvas(nullptr, "~") == "");
        REQUIRE(make_abs_filepath_with_canvas(nullptr, "~/") == "");

        auto cnv = ceammc::PureData::instance().createTopCanvas(TEST_DATA_DIR "/canvas");
        REQUIRE(cnv);
        REQUIRE(cnv->pd_canvas());

#ifndef __WIN32
        REQUIRE(make_abs_filepath_with_canvas(cnv->pd_canvas(), "/abc") == "/abc");
#else
        REQUIRE(make_abs_filepath_with_canvas(cnv->pd_canvas(), "C:/abc") == "C:/abc");
        REQUIRE(make_abs_filepath_with_canvas(cnv->pd_canvas(), "\\abc") == "\\abc");
#endif
        REQUIRE(make_abs_filepath_with_canvas(cnv->pd_canvas(), "") == "");
        REQUIRE(make_abs_filepath_with_canvas(cnv->pd_canvas(), "~") == "");
        REQUIRE(make_abs_filepath_with_canvas(cnv->pd_canvas(), "~/") == "");
        REQUIRE(make_abs_filepath_with_canvas(cnv->pd_canvas(), "abc.txt") == TEST_DATA_DIR "/abc.txt");
        REQUIRE(make_abs_filepath_with_canvas(cnv->pd_canvas(), "./abc.txt") == TEST_DATA_DIR "/./abc.txt");

        auto sub = ceammc::PureData::instance().createSubpatch(cnv->pd_canvas(), "sp");
        REQUIRE(make_abs_filepath_with_canvas(sub->pd_canvas(), "abc.txt") == TEST_DATA_DIR "/abc.txt");
    }

    SECTION("list ifaces")
    {
        using namespace ceammc;

#ifdef __MACH__
        REQUIRE(platform::net_ifaces_ip().size() > 0);
        REQUIRE(platform::net_ifaces_ip(ADDR_IPV4).size() > 0);
        REQUIRE(platform::net_ifaces_ip(ADDR_IPV6).size() > 0);
        REQUIRE(platform::net_ifaces_ip(ADDR_IPANY).size() > 0);
#endif
    }

    SECTION("file_mime_type")
    {
#ifdef __WIN32
        return;
#endif

        using namespace ceammc;
        REQUIRE(platform::file_mime_type("") == "");
        REQUIRE(platform::file_mime_type("???") == "");
        REQUIRE(platform::file_mime_type(__FILE__) == "text/x-c");
        REQUIRE(platform::file_mime_type(TEST_DATA_DIR) == "inode/directory");
        REQUIRE(platform::file_mime_type(TEST_DATA_DIR "/llvm_cov_gen.sh") == "text/x-shellscript");

#if defined(__linux__) || defined(__apple__)
        REQUIRE(platform::file_mime_type(TEST_DATA_DIR "/gen_test_wav.py") == "text/x-script.python");
#endif
        REQUIRE(platform::file_mime_type(TEST_DATA_DIR "/snd_mono_44k.ogg") == "audio/ogg");
        REQUIRE(platform::file_mime_type(TEST_DATA_DIR "/snd_mono_48k.flac") == "audio/flac");
        REQUIRE(platform::file_mime_type(TEST_DATA_DIR "/snd_mono_48k.wav") == "audio/x-wav");
        REQUIRE(platform::file_mime_type(TEST_DATA_DIR "/test_data0.mp3") == "audio/mpeg");

        REQUIRE(platform::file_mime_type(TEST_DATA_DIR "/test0.lua") == "text/plain");
        REQUIRE(platform::file_mime_type(TEST_DATA_DIR "/test_canvas_01.pd") == "text/plain");
    }
}
