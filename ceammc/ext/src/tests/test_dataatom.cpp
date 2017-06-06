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
#include "base_extension_test.h"
#include "ceammc_dataatom.h"

#include "catch.hpp"

using namespace ceammc;

class IntData : public AbstractData {
    int v_;

public:
    IntData(int v)
        : v_(v)
    {
    }

    int get() const { return v_; }
    void set(int v) { v_ = v; }
    IntData* clone() const { return new IntData(v_); }
    DataType type() const { return dataType; }
    bool isEqual(const AbstractData* d) const
    {
        if (d->type() != dataType)
            return false;

        return d->as<IntData>()->v_ == v_;
    }

    std::string toString() const
    {
        std::ostringstream os;
        os << v_;
        return os.str();
    }

public:
    static const DataType dataType = 33;
};

TEST_CASE("DataAtom", "[ceammc::DataAtom]")
{
    SECTION("create")
    {
        DataAtom a(1.2f);
        REQUIRE(a.isAtom());
        REQUIRE(!a.isData());

        DataAtom b(1.2f);
        REQUIRE(a == b);
        b.set(S("ABC"));
        REQUIRE_FALSE(a == b);

        Data int_data(new IntData(123));
        b.set(int_data.toAtom());

        REQUIRE(b.isData());
        REQUIRE(b.data() != int_data.data());
        REQUIRE(b.data()->type() == int_data.data()->type());
        REQUIRE(b.data()->toString() == int_data.data()->toString());
        REQUIRE(b.data()->isEqual(int_data.data()));
        REQUIRE(int_data.data()->isEqual(b.data()));

        Atom aa = int_data.toAtom();
        Atom bb = b.toAtom();

        REQUIRE(aa.dataType() == bb.dataType());
        REQUIRE(aa != bb);

        Data* aptr = DataStorage::instance().get(b.dataPtr()->desc());
        Data* bptr = DataStorage::instance().get(int_data.desc());

        REQUIRE(aptr != bptr);

        DataAtom c(b);
        REQUIRE(c.isData());
        REQUIRE(c.data()->toString() == "123");
        REQUIRE(((IntData*)c.data())->get() == 123);
        //        REQUIRE(b == c);
        //            REQUIRE(c == b);
    }
}
