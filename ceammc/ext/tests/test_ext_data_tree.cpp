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
#include "../data/data_tree.h"
#include "ceammc_platform.h"
#include "datatype_tree_imp.h"
#include "test_external.h"

#define REQUIRE_SIZE(obj, n) REQUIRE_PROPERTY(t, @size, float(n));

#define REQUIRE_DATA_EQUAL_AT_OUTLET(outlet, obj, data)       \
    {                                                         \
        REQUIRE(obj.hasNewMessages(outlet));                  \
        REQUIRE(obj.lastMessage(outlet).isData());            \
        REQUIRE(obj.lastMessage(outlet).dataValue() == data); \
    }

#define REQUIRE_OUTPUT_DICT(obj, data)                   \
    {                                                    \
        REQUIRE(obj.hasNewMessages(0));                  \
        REQUIRE(obj.lastMessage(0).isData());            \
        REQUIRE(obj.lastMessage(0).dataValue() == data); \
    }

#define REQUIRE_CONTAINS_ATOM(obj, key, value)                                           \
    {                                                                                    \
        REQUIRE(obj.dict().contains(test_atom_wrap(key)));                               \
        REQUIRE(*obj.dict().valueT<Atom>(test_atom_wrap(key)) == test_atom_wrap(value)); \
    }

#define REQUIRE_CONTAINS_LIST(obj, key, lst)                               \
    {                                                                      \
        REQUIRE(obj.dict().contains(test_atom_wrap(key)));                 \
        REQUIRE(*obj.dict().valueT<AtomList>(test_atom_wrap(key)) == lst); \
    }

PD_COMPLETE_TEST_SETUP(DataTree, data, tree)

TEST_CASE("data.tree", "[externals]")
{
    pd_test_init();

    SECTION("escape")
    {
        REQUIRE(DataTypeTreeImpl::treeToJson("") == "");
        REQUIRE(DataTypeTreeImpl::treeToJson("()") == "{}");
        REQUIRE(DataTypeTreeImpl::treeToJson(R"(abc)") == "abc");
        REQUIRE(DataTypeTreeImpl::treeToJson(R"("abc")") == "\"abc\"");
        REQUIRE(DataTypeTreeImpl::treeToJson("\"((())\"()") == "\"((())\"{}");
        REQUIRE(DataTypeTreeImpl::treeToJson("(\"\\\"\")") == R"({"\""})");

#define REQUIRE_ESC_TREE(str) REQUIRE(DataTypeTreeImpl::jsonToTree(DataTypeTreeImpl::treeToJson(str)) == str)
#define REQUIRE_ESC_JSON(str) REQUIRE(DataTypeTreeImpl::treeToJson(DataTypeTreeImpl::jsonToTree(str).c_str()) == str)

        REQUIRE_ESC_TREE("");
        REQUIRE_ESC_TREE("[]");
        REQUIRE_ESC_TREE("1 2 3");
        REQUIRE_ESC_TREE("[1,2,3]");
        REQUIRE_ESC_TREE("[\"a\",\"b\"]");
        REQUIRE_ESC_TREE("()");
        REQUIRE_ESC_TREE("123 123");
        REQUIRE_ESC_TREE("spaces     spaces");
        REQUIRE_ESC_TREE("abc");
        REQUIRE_ESC_TREE("\"{}}}}}\"");

        REQUIRE_ESC_JSON("");
        REQUIRE_ESC_JSON("[]");
        REQUIRE_ESC_JSON("1 2 3");
        REQUIRE_ESC_JSON("[1,2,3]");
        REQUIRE_ESC_JSON("[\"a\",\"b\"]");
        REQUIRE_ESC_JSON("\"()\"");
        REQUIRE_ESC_JSON("123 123");
        REQUIRE_ESC_JSON("spaces     spaces");
        REQUIRE_ESC_JSON("abc");
        REQUIRE_ESC_JSON("\"{}}}}}\"");

#undef REQUIRE_ESC_TREE
#undef REQUIRE_ESC_JSON
    }
}
