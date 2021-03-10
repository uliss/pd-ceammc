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
#include "ceammc_format.h"
#include "datatype_string.h"
#include "path_listdir.h"
#include "test_path_base.h"

PD_COMPLETE_TEST_SETUP(PathListDir, path, lsdir)

typedef std::vector<std::string> FileList;

static FileList dataToList(const Message& m)
{
    FileList res;
    const AtomList& items = m.listValue();
    for (size_t i = 0; i < items.size(); i++)
        res.push_back(to_string(items[i]));

    std::sort(res.begin(), res.end());
    return res;
}

TEST_CASE("path.lsdir", "[externals]")
{
    pd_test_init();

    FileList files;
    files.push_back("test_data0.mp3");
    files.push_back("test_data0_vbr.mp3");

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TObj t("path.ls", L());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @match, "");

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_SYMBOL_TO(0, t, TEST_DATA_DIR);
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage(0).isList());
            REQUIRE(t.lastMessage(0).listValue().size() > 0);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage(0).listValue().size() > 0);
        }

        SECTION("properties")
        {
            TExt t("path.ls", LA("@match", "*.mp3"));
            REQUIRE_PROPERTY(t, @match, A("*.mp3"));
            REQUIRE_PROPERTY(t, @path, "");

            t.sendBang();
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            t << TEST_DATA_DIR;
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage(0).isList());
            REQUIRE(dataToList(t.lastMessage(0)) == files);

            t << TEST_DATA_DIR "non-exists";
            REQUIRE_LIST_AT_OUTLET(0, t, L());
        }

        SECTION("properties")
        {
            TExt t("path.ls", LA(".", "@match", "*.mp3"));
            REQUIRE_PROPERTY(t, @match, A("*.mp3"));
            REQUIRE_PROPERTY(t, @path, ".");

            t.sendBang();
            REQUIRE_LIST_AT_OUTLET(0, t, L());

            t << TEST_DATA_DIR;
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage(0).isList());
            REQUIRE(dataToList(t.lastMessage(0)) == files);
        }

        SECTION("properties")
        {
            TObj t("path.ls", LA("\"a", "file.mp3\""));
            REQUIRE_PROPERTY(t, @path, "a file.mp3");
        }
    }

    SECTION("test errors")
    {
        TObj t("path.ls", L());

        WHEN_SEND_SYMBOL_TO(0, t, TEST_DATA_DIR "non-exists");
        REQUIRE_LIST_AT_OUTLET(0, t, L());

#ifdef __WIN32__
        WHEN_SEND_SYMBOL_TO(0, t, "C:/Windows");
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(t.lastMessage(0).listValue().size() > 0);
#else
        WHEN_SEND_SYMBOL_TO(0, t, "~");
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(t.lastMessage(0).listValue().size() > 0);
#endif
    }

    SECTION("set match")
    {
        TExt t("path.ls");
        REQUIRE_PROPERTY(t, @match, "");
        t.sendSymbolTo(SYM("*.mp4"), 1);
        REQUIRE_PROPERTY(t, @match, "*.mp4");
    }
}
