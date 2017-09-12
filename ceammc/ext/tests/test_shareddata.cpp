/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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

#include "catch.hpp"
#include "ceammc_globaldata.h"

class TestRef {
public:
    static int count;
    int value;
    TestRef(int v)
        : value(v)
    {
        count++;
    }
    ~TestRef() { count--; }
};

using namespace ceammc;
typedef GlobalData<int> GlobalInt;
typedef NamedDataDict<TestRef> NamedTestDict;
typedef NamedDataDict<int> NamedIntDict;

int TestRef::count = 0;

TEST_CASE("NamedDataDict", "[ceammc::NamedDataDict]")
{
    NamedTestDict dict;
    REQUIRE_FALSE(dict.contains("test"));
    REQUIRE(dict.refCount("test") == 0);
    REQUIRE(dict.acquire("test") == 0);
    REQUIRE_FALSE(dict.release("test"));

    REQUIRE(TestRef::count == 0);
    TestRef* t = new TestRef(0xBEEF);
    REQUIRE(TestRef::count == 1);
    REQUIRE(dict.create("test", t));
    REQUIRE(dict.create("test2", new TestRef(0xDAD)));
    REQUIRE(TestRef::count == 2);
    // second creation
    REQUIRE_FALSE(dict.create("test", t));

    REQUIRE(dict.contains("test"));
    REQUIRE(dict.refCount("test") == 0);
    // +
    REQUIRE(dict.acquire("test")->value == 0xBEEF);
    REQUIRE(dict.refCount("test") == 1);
    // -
    REQUIRE(dict.release("test"));
    REQUIRE(dict.refCount("test") == 0);
    // failed -
    REQUIRE_FALSE(dict.release("test"));
    REQUIRE(dict.refCount("test") == 0);
    REQUIRE_FALSE(dict.contains("test"));

    // not acquired data
    REQUIRE(TestRef::count == 1);
    // release not acquired
    REQUIRE(dict.release("test2"));
    REQUIRE(TestRef::count == 0);
}

TEST_CASE("SharedData", "[ceammc::SharedData]")
{
    GlobalInt int1("id1");
    // default constructor
    REQUIRE(int1.ref() == 0);
    int1.ref() = 200;
    // Same ID
    GlobalInt int2("id1");
    REQUIRE(int2.ref() == 200);
    // same pointer
    REQUIRE(&int1.ref() == &int2.ref());

    // change data
    int2.ref() = 300;
    REQUIRE(int1.ref() == 300);
}
