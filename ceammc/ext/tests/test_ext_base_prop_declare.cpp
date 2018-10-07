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
#include "../base/prop_declare.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(PropDeclare, prop, declare);

static const t_float FMIN = std::numeric_limits<t_float>::lowest();
static const t_float FMAX = std::numeric_limits<t_float>::max();
static const long IMIN = std::numeric_limits<long>::lowest();
static const long IMAX = std::numeric_limits<long>::max();

TEST_CASE("radio", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError(true);

    SECTION("construct")
    {
        t_symbol* fn = &s_;

        {
            // default name
            TestExtPropDeclare t("prop.declare");
            REQUIRE(t.object());
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t.numOutlets() == 0);
            REQUIRE(t->isFloat());
            REQUIRE(t->name() == SYM("@default"));
            fn = t->fullName();

            REQUIRE(PropertyStorage::storage().contains(fn->s_name));
        }

        REQUIRE_FALSE(PropertyStorage::storage().contains(fn->s_name));

        {
            // invalid name
            TestExtPropDeclare t("prop.declare", LX(1, 2, 3));
            REQUIRE(t.object());
            REQUIRE(t->name() == SYM("@default"));
        }

        {
            // valid name
            TestExtPropDeclare t("prop.declare", LA("a", "b", "c"));
            REQUIRE(t.object());
            REQUIRE(t->name() == SYM("@a"));
            REQUIRE(t->isFloat());
        }

        {
            // same name
            pd::External t0("prop.declare", LA("a"));
            pd::External t1("prop.declare", LA("a"));

            REQUIRE(t0.object());
            REQUIRE(!t1.object());
        }

        {
            // float type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "float"));
            REQUIRE(t.object());
            REQUIRE(t->isFloat());
        }

        {
            // float type
            TestExtPropDeclare t("prop.declare", LA("a", "@f"));
            REQUIRE(t.object());
            REQUIRE(t->isFloat());
        }

        {
            // int type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "int"));
            REQUIRE(t.object());
            REQUIRE(t->isInt());
        }

        {
            // int type
            TestExtPropDeclare t("prop.declare", LA("a", "@i"));
            REQUIRE(t.object());
            REQUIRE(t->isInt());
        }

        {
            // bool type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "bool"));
            REQUIRE(t.object());
            REQUIRE(t->isBool());
        }

        {
            // bool type
            TestExtPropDeclare t("prop.declare", LA("a", "@b"));
            REQUIRE(t.object());
            REQUIRE(t->isBool());
        }

        {
            // symbol type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "symbol"));
            REQUIRE(t.object());
            REQUIRE(t->isSymbol());
        }

        {
            // symbol type
            TestExtPropDeclare t("prop.declare", LA("a", "@s"));
            REQUIRE(t.object());
            REQUIRE(t->isSymbol());
        }

        {
            // list type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "list"));
            REQUIRE(t.object());
            REQUIRE(t->isList());
        }

        {
            // list type
            TestExtPropDeclare t("prop.declare", LA("a", "@type", "list"));
            REQUIRE(t.object());
            REQUIRE(t->isList());
        }
    }

    SECTION("enum")
    {
        // symbol type
        TestExtPropDeclare t("prop.declare", LA("s", "@s", "@enum", "A", "B", "C"));
        auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
        REQUIRE(p->enumValues() == LA("", "A", "B", "C"));
        PropertyStorage::storage().release(t->fullName()->s_name);
    }

    SECTION("min/max float")
    {
        {
            // float default
            TestExtPropDeclare t("prop.declare", LA("f"));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            REQUIRE(p->floatRange() == std::make_pair(FMIN, FMAX));
            PropertyStorage::storage().release(t->fullName()->s_name);
        }

        {
            // float min
            TestExtPropDeclare t("prop.declare", LA("f", "@min", 2.1));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            auto r = p->floatRange();
            REQUIRE(r.first == Approx(2.1));
            REQUIRE(r.second == FMAX);
            PropertyStorage::storage().release(t->fullName()->s_name);
        }

        {
            // float max
            TestExtPropDeclare t("prop.declare", LA("f", "@max", 16.1));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            auto r = p->floatRange();
            REQUIRE(r.second == Approx(16.1));
            REQUIRE(r.first == FMIN);
            PropertyStorage::storage().release(t->fullName()->s_name);
        }

        {
            // float both
            TestExtPropDeclare t("prop.declare", LA("f", "@max", 16.1, "@min", 1.1));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            auto r = p->floatRange();
            REQUIRE(r.second == Approx(16.1));
            REQUIRE(r.first == Approx(1.1));
            PropertyStorage::storage().release(t->fullName()->s_name);
        }

        {
            // float both invalid
            TestExtPropDeclare t("prop.declare", LA("f", "@min", 16.1, "@max", 1.1));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            auto r = p->floatRange();
            REQUIRE(r.second == Approx(16.1));
            REQUIRE(r.first == Approx(1.1));
            PropertyStorage::storage().release(t->fullName()->s_name);
        }
    }

    SECTION("min/max int")
    {
        {
            // int default
            TestExtPropDeclare t("prop.declare", LA("i", "@i"));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            REQUIRE(p->intRange() == std::make_pair(IMIN, IMAX));
            PropertyStorage::storage().release(t->fullName()->s_name);
        }

        {
            // int min
            TestExtPropDeclare t("prop.declare", LA("i", "@i", "@min", 2));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            REQUIRE(p->intRange() == std::make_pair(long(2), IMAX));
            PropertyStorage::storage().release(t->fullName()->s_name);
        }

        {
            // int max
            TestExtPropDeclare t("prop.declare", LA("i", "@i", "@max", 20));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            REQUIRE(p->intRange() == std::make_pair(IMIN, long(20)));
            PropertyStorage::storage().release(t->fullName()->s_name);
        }

        {
            // int both
            TestExtPropDeclare t("prop.declare", LA("i", "@i", "@max", 20, "@min", -2));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            REQUIRE(p->intRange() == std::make_pair(long(-2), long(20)));
            PropertyStorage::storage().release(t->fullName()->s_name);
        }

        {
            // int both invalid order
            TestExtPropDeclare t("prop.declare", LA("i", "@i", "@max", -2, "@min", 20));
            auto p = PropertyStorage::storage().acquire(t->fullName()->s_name);
            REQUIRE(p->intRange() == std::make_pair(long(-2), long(20)));
            PropertyStorage::storage().release(t->fullName()->s_name);
        }
    }
}
