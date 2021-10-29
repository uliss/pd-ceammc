/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "ceammc_factory.h"
#include "ceammc_property_enum.h"
#include "prop_random.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(PropRandom, prop, random)

class TestPropsRandom : public BaseObject {
public:
    FloatProperty* fp0_;
    FloatProperty* fp1_;
    FloatProperty* fp2_;
    FloatProperty* fp3_;
    FloatProperty* fp4_;
    FloatProperty* fp5_;
    FloatProperty* fp6_;
    IntProperty* ip0_;
    IntProperty* ip1_;
    IntProperty* ip2_;
    IntProperty* ip3_;
    BoolProperty* bp0_;
    SymbolEnumProperty* senum0_;

public:
    TestPropsRandom(const PdArgs& args)
        : BaseObject(args)
    {
        fp0_ = new FloatProperty("@fp0", 0);
        fp0_->setReadOnly();
        addProperty(fp0_);

        fp1_ = new FloatProperty("@fp1", 0);
        fp1_->setInitOnly();
        addProperty(fp1_);

        fp2_ = new FloatProperty("@fp2", 0);
        fp2_->setHidden();
        addProperty(fp2_);

        fp3_ = new FloatProperty("@fp3", 0);
        addProperty(fp3_);

        fp4_ = new FloatProperty("@fp4", 0);
        fp4_->checkMin(-10);
        addProperty(fp4_);

        fp5_ = new FloatProperty("@fp5", 0);
        fp5_->checkMax(-10);
        addProperty(fp5_);

        fp6_ = new FloatProperty("@fp6", 15);
        fp6_->checkClosedRange(10, 20);
        addProperty(fp6_);

        ip0_ = new IntProperty("@ip0", 0);
        addProperty(ip0_);

        ip1_ = new IntProperty("@ip1", 0);
        ip1_->checkMinEq(-10);
        addProperty(ip1_);

        ip2_ = new IntProperty("@ip2", 0);
        ip2_->checkMax(1);
        addProperty(ip2_);

        ip3_ = new IntProperty("@ip3", 25);
        ip3_->checkClosedRange(20, 30);
        addProperty(ip3_);

        bp0_ = new BoolProperty("@bp0", true);
        addProperty(bp0_);

        senum0_ = new SymbolEnumProperty("@senum0", { "a", "b", "c", "d", "e" });
        addProperty(senum0_);
    }
};

using TProp = TestPdExternal<TestPropsRandom>;

static void setup_test_props_random()
{
    ObjectFactory<TestPropsRandom> obj("test.rp");
}

TEST_CASE("prop.random", "[externals]")
{
    pd_test_init();
    setup_test_props_random();
    test::pdPrintToStdError();

    SECTION("construct")
    {
        External t("prop.random", LA("@db"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        t.sendBang();
    }

    SECTION("readonly prop")
    {
        External t("prop.random", LA("@fp0"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));

        t.sendBang();
    }

    SECTION("initonly prop")
    {
        External t("prop.random", LA("@fp1"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
    }

    SECTION("hidden prop")
    {
        External t("prop.random", LA("@fp2"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
    }

    SECTION("float not bounded prop")
    {
        External t("prop.random", LA("@fp3"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
    }

    SECTION("flaot min bounded prop")
    {
        External t("prop.random", LA("@fp4"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
    }

    SECTION("float max bounded prop")
    {
        External t("prop.random", LA("@fp5"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
    }

    SECTION("float prop ok")
    {
        TExt t("prop.random", LA("@fp6"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
        REQUIRE(t.hasOutputAt(1));
        const auto out = t.outputFloatAt(1);
        REQUIRE(out >= 10);
        REQUIRE(out <= 20);
        REQUIRE(tp->fp6_->value() == out);

        t.sendBang();
        REQUIRE(tp->fp6_->value() != out);
    }

    SECTION("int not bounded prop")
    {
        External t("prop.random", LA("@ip0"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
    }

    SECTION("int min bounded prop")
    {
        External t("prop.random", LA("@ip1"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
    }

    SECTION("int max bounded prop")
    {
        External t("prop.random", LA("@ip2"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
    }

    SECTION("int prop ok")
    {
        TExt t("prop.random", LA("@ip3"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
        REQUIRE(t.hasOutputAt(1));
        const auto out = t.outputFloatAt(1);
        REQUIRE(out >= 10);
        REQUIRE(out <= 30);
        REQUIRE(tp->ip3_->value() == out);

        t.sendBang();
        REQUIRE(tp->ip3_->value() != out);
    }

    SECTION("bool prop ok")
    {
        TExt t("prop.random", LA("@bp0"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
        REQUIRE(t.hasOutputAt(1));
        const auto out = t.outputFloatAt(1);
        REQUIRE(tp->bp0_->value() == out);
    }

    SECTION("symbol enum prop ok")
    {
        TExt t("prop.random", LA("@senum0"));
        TProp tp("test.rp");
        REQUIRE(t.connectTo(0, tp, 0));
        t.sendBang();
        REQUIRE(t.hasOutputAt(1));
        const auto out = t.outputSymbolAt(1);
        REQUIRE(tp->senum0_->value() == out);
    }
}
