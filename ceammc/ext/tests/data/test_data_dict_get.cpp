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
#include "dict_get.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DictGet, dict, get)

using MA = std::vector<Message>;
static std::vector<Message> msort(const std::vector<Message>& m)
{
    auto res = m;
    std::sort(res.begin(), res.end(),
        [](const Message& m1, const ceammc::Message& m2) {
            //
            if (m1.type() == m2.type()) {
                if (m1.isFloat() || m1.isSymbol() || m1.isAny()) {
                    return m1.atomValue() < m2.atomValue();
                } else if (m1.isList()) {
                    auto& l1 = m1.listValue();
                    auto& l2 = m2.listValue();
                    for (size_t i = 0; i < std::min<size_t>(l1.size(), l2.size()); i++) {
                        if (l1[i] < l2[i])
                            return true;
                    }

                    return l1.size() > l2.size();
                } else {
                    if (m1.isData())
                        return m1.atomValue().dataType() < m2.atomValue().dataType();
                    else
                        return false;
                }
            } else
                return m1.type() < m2.type();
        });
    return res;
}

#define REQUIRE_MESSAGES(obj, ...)                               \
    {                                                            \
        REQUIRE(msort(obj.messagesAt(0)) == MA { __VA_ARGS__ }); \
    }

TEST_CASE("dict.get", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("dict.get");
            REQUIRE_PROPERTY(t, @keys);
            // clang-format off
            REQUIRE_PROPERTY(t, @default, Atom());
            // clang-format on
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
        }

        SECTION("invalid keys")
        {
            TObj t("dict.get", LA(1, 2, 3));
            REQUIRE_PROPERTY(t, @keys);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
        }

        SECTION("keys")
        {
            TObj t("dict.get", LA("A", "B", "\"@C\""));
            REQUIRE_PROPERTY(t, @keys, LA("A", "B", "@C"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 3);
        }

        SECTION("too many keys")
        {
            AtomList args;
            TObj t("dict.get", L().filled(A("A"), 33));
            REQUIRE_PROPERTY(t, @keys);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
        }

        SECTION("@props")
        {
            TObj t("dict.get", LA("\"@a\"", "\"@b\""));
            REQUIRE_PROPERTY(t, @keys, LA("@a", "@b"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("do")
    {
        TExt t("dict.get", "A", "\"@a\"", "b");
        REQUIRE_PROPERTY(t, @keys, "A", "@a", "b");

        REQUIRE(t.object());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);

        t.send(DictA());
        REQUIRE_FALSE(t.hasOutput());

        t.send(DictA("[c: d]"));
        REQUIRE_FALSE(t.hasOutput());

        t.send(DictA("[@a: 100]"));
        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(!t.hasOutputAt(2));
        REQUIRE(t.outputFloatAt(1) == 100);

        t.send(DictA("[b: 1 2 3 4]"));
        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(t.hasOutputAt(2));
        REQUIRE(t.outputListAt(2) == LX(1, 2, 3, 4));

        t.send(DictA("[A: ABC]"));
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(!t.hasOutputAt(2));
        REQUIRE(t.outputSymbolAt(0) == gensym("ABC"));
    }

    SECTION("any")
    {
        TExt t("dict.get", "b");
        t <<= LA("[a:", 120, "b:", 130, "]");
        REQUIRE(floatAt(t) == 130);
    }

    SECTION("@default")
    {
        TExt t("dict.get", "b", "c", "\"@default\"", "@default", -1);

        // clang-format off
        REQUIRE_PROPERTY(t, @keys, "b", "c", "@default");
        REQUIRE_PROPERTY(t, @default, -1);
        // clang-format on

        t <<= AtomList::parseString("[a: 1 b: 2]");
        REQUIRE(floatAt(t, 0_out) == 2);
        REQUIRE(floatAt(t, 1_out) == -1);
        REQUIRE(floatAt(t, 2_out) == -1);

        t <<= AtomList::parseString("[c: 1 @default: 2]");
        REQUIRE(floatAt(t, 0_out) == -1);
        REQUIRE(floatAt(t, 1_out) == 1);
        REQUIRE(floatAt(t, 2_out) == 2);
    }

    SECTION("matches")
    {
        SECTION("/b")
        {
            TExt t("dict.get", "/foo");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: foo: c: d:]");
            REQUIRE(listAt(t) == L());
            t << DictAtom("[a: 1 foo: 120]");
            REQUIRE(floatAt(t) == 120);
            t << DictAtom("[a: 1 foo: ABC]");
            REQUIRE(symbolAt(t) == "ABC");
            t << DictAtom("[a: 1 foo: 1 2 3]");
            REQUIRE(listAt(t) == LF(1, 2, 3));
            t << DictAtom("[a: 1 foo: [obj:]]");
            REQUIRE(dataAt(t) == DictAtom("[obj:]"));
            t << DictAtom("[a: 1 foo: (1 2 3)]");
            REQUIRE(dataAt(t) == MListAtom(1, 2, 3));
            t << DictAtom("[d: e: f:]");
            REQUIRE(!t.hasOutputAt(0));
        }

        SECTION("/*")
        {
            TExt t("dict.get", "/*");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a:]");
            REQUIRE(listAt(t) == L());
            t << DictAtom("[a: b:]");
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(0).at(0) == L());
            REQUIRE(t.messagesAt(0).at(1) == L());

            t << DictAtom("[a: 1 b: TEXT]");
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE_MESSAGES(t, LF(1), LA("TEXT"));

            t << DictAtom("[a: 1 b: TEXT c: 1 2 3]");
            REQUIRE(t.messagesAt(0).size() == 3);
            REQUIRE_MESSAGES(t, LF(1), LA("TEXT"), LF(1, 2, 3));

            t << DictAtom("[a: 1 b: [f:]]");
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE_MESSAGES(t, LF(1), DictAtom("[f:]"));
        }

        SECTION("/1")
        {
            TExt t("dict.get", "/a/1");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a:]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: -1]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: 1 2 3]");
            REQUIRE(floatAt(t) == 2);
            t << DictAtom("[a: (10 20 30)]");
            REQUIRE(floatAt(t) == 20);
            t << DictAtom("[a: (10)]");
            REQUIRE(t.messagesAt(0) == MA {});
            t << DictAtom("[a: ()]");
            REQUIRE(t.messagesAt(0) == MA {});
        }

        SECTION("/a/-1")
        {
            TExt t("dict.get", "/a/-1");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a:]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: -1]");
            REQUIRE(floatAt(t) == -1);
            t << DictAtom("[a: 1 2 3]");
            REQUIRE(floatAt(t) == 3);
            t << DictAtom("[a: (10 20 30)]");
            REQUIRE(floatAt(t) == 30);
            t << DictAtom("[a: (10 20)]");
            REQUIRE(floatAt(t) == 20);
            t << DictAtom("[a: (10)]");
            REQUIRE(floatAt(t) == 10);
            t << DictAtom("[a: ()]");
            REQUIRE(t.messagesAt(0) == MA {});
        }

        SECTION("/*")
        {
            TExt t("dict.get", "/*");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));

            t << DictAtom("[a:]");
            REQUIRE(listAt(t) == L());
            t << DictAtom("[a: 1]");
            REQUIRE(floatAt(t) == 1);
            t << DictAtom("[a: 1 2 3]");
            REQUIRE(listAt(t) == LF(1, 2, 3));
            t << DictAtom("[a: b:]");
            REQUIRE_MESSAGES(t, L(), L());
            t << DictAtom("[a: b: 1 c: DEF d: 1 2 3]");
            REQUIRE_MESSAGES(t, LF(1), LA("DEF"), LF(1, 2, 3), L());
            t << DictAtom("[a: b: 1 c: DEF d: 1 2 3 e: ()]");
            REQUIRE_MESSAGES(t, LF(1), LA("DEF"), LF(1, 2, 3), L(), MListAtom());
            t << DictAtom("[a: b: 1 c: DEF d: 1 2 3 e: () f: []]");
            REQUIRE_MESSAGES(t, LF(1), LA("DEF"), LF(1, 2, 3), L(), DictAtom(), MListAtom());
        }

        SECTION("/*/foo")
        {
            TExt t("dict.get", "/*/foo");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: 1 2 3 b: [foo:]]");
            REQUIRE_MESSAGES(t, L());
            t << DictAtom("[a: 1 2 3 b: [foo: ()]]");
            REQUIRE_MESSAGES(t, MListAtom());
            t << DictAtom("[a: 1 2 3 b: [foo: 1 2 3]]");
            REQUIRE_MESSAGES(t, LF(1, 2, 3));
            t << DictAtom("[a: 1 2 3 b: [foo: ABC] c: [foo: 4]]");
            REQUIRE_MESSAGES(t, LF(4), LA("ABC"));
            t << DictAtom("[a: 1 2 3 b: [foo: ABC] c: [foo: 4] d: [foo: [bar:]]]");
            REQUIRE_MESSAGES(t, LF(4), LA("ABC"), DictAtom("[bar:]"));
        }

        SECTION("/*/foo/*")
        {
            TExt t("dict.get", "/*/foo/*");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[b: [foo:]]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[b: [foo: 1]]");
            REQUIRE(floatAt(t) == 1);
            t << DictAtom("[b: [foo: 1 2]]");
            REQUIRE_MESSAGES(t, LF(1), LF(2));
            t << DictAtom("[b: [foo: 1 2] c: [foo: ABC]]");
            REQUIRE_MESSAGES(t, LF(1), LF(2), LA("ABC"));
        }

        SECTION("/foo/*/foo")
        {
            TExt t("dict.get", "/foo/*/foo");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[foo: [a:]]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[foo: [a: 1 b: 2]]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[foo: [a: 1 b: 2 c: [foo: 32]]]");
            REQUIRE(floatAt(t) == 32);
            t << DictAtom("[foo: [a: 1 b: 2 c: [foo: 32] d: [foo: 33 34 35]]]");
            REQUIRE_MESSAGES(t, LF(32), LF(33, 34, 35));
        }

        SECTION("/*/0")
        {
            TExt t("dict.get", "/*/0");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a:]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: -1]");
            REQUIRE(floatAt(t) == -1);
            t << DictAtom("[a: 1 2 3]");
            REQUIRE(floatAt(t) == 1);
            t << DictAtom("[a: ()]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: (10)]");
            REQUIRE(floatAt(t) == 10);
            t << DictAtom("[a: (10 20)]");
            REQUIRE(floatAt(t) == 10);
        }

        SECTION("/*/1:")
        {
            TExt t("dict.get", "/*/1:");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a:]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: -1]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: 1 2 3]");
            REQUIRE(listAt(t) == LF(2, 3));
            t << DictAtom("[a: ()]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: (10)]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: (10 20)]");
            REQUIRE(floatAt(t) == 20);
            t << DictAtom("[a: (10 20 30)]");
            REQUIRE(listAt(t) == LF(20, 30));
        }

        SECTION("/*/1:3")
        {
            TExt t("dict.get", "/*/1:3");

            t << DictAtom("[]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a:]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: -1]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: 1 2 3]");
            REQUIRE(listAt(t) == LF(2, 3));
            t << DictAtom("[a: 1 2 3 4]");
            REQUIRE(listAt(t) == LF(2, 3, 4));
            t << DictAtom("[a: 1 2 3 4 5]");
            REQUIRE(listAt(t) == LF(2, 3, 4));
            t << DictAtom("[a: ()]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: (10)]");
            REQUIRE(!t.hasOutputAt(0));
            t << DictAtom("[a: (10 20)]");
            REQUIRE(floatAt(t) == 20);
            t << DictAtom("[a: (10 20 30)]");
            REQUIRE(listAt(t) == LF(20, 30));
            t << DictAtom("[a: (10 20 30)]");
            REQUIRE(listAt(t) == LF(20, 30));
        }
    }
}
