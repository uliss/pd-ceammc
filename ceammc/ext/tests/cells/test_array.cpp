#include "catch.hpp"
#include "test_base.h"

#include "ceammc_array.h"
#include "ceammc_canvas.h"
#include "ceammc_pd.h"

static t_float generateLin(size_t n) { return n; }

#define REQUIRE_ARRAY_EQUAL(a, b)             \
    {                                         \
        REQUIRE(a.size() == b.size());        \
        for (size_t i = 0; i < a.size(); i++) \
            REQUIRE(a[i] == b[i]);            \
    }

TEST_CASE("Array", "[ceammc::Atom]")
{
    using namespace ceammc;

    SECTION("base")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas("array_test");
        ArrayPtr array1 = cnv->createArray("array1", 10);
        REQUIRE(array1->isValid());
        REQUIRE(array1->size() == 10);
        REQUIRE(array1->name() == SYM("array1"));
        for (size_t i = 0; i < 10; i++) {
            REQUIRE(array1->at(i) == 0.f);
        }

        Array a("not-exists");
        REQUIRE(a.begin() == a.end());
        REQUIRE_FALSE(a.isValid());
        REQUIRE_FALSE(a.open("not-exists2"));
        REQUIRE_FALSE(a.isValid());

        REQUIRE(a.open("array1"));
        REQUIRE(a.size() == 10);

        a.fillWith(11.f);

        REQUIRE_ARRAY_EQUAL(a, (*array1));

        for (size_t i = 0; i < 10; i++) {
            REQUIRE(a.at(i) == 11.f);
        }

        a.fillWith(generateLin);
        REQUIRE_ARRAY_EQUAL(a, (*array1));
        for (size_t i = 0; i < 10; i++) {
            REQUIRE(a.at(i) == float(i));
        }
    }

    SECTION("iterators")
    {
        Array a("array1");
        REQUIRE(a.isValid());
        a.fillWith(generateLin);

        REQUIRE(a.begin() != a.end());
        REQUIRE(a.begin() < a.end());
        REQUIRE(a.begin() <= a.end());
        REQUIRE(a.end() > a.begin());
        REQUIRE(a.end() >= a.begin());
        REQUIRE(a.begin() == a.begin());
        REQUIRE(a.end() == a.end());

        ArrayIterator it = a.begin();
        REQUIRE(*it == 0.f);
        REQUIRE(*(++it) == 1.f);
        REQUIRE(*(it++) == 1.f);
        REQUIRE(*it == 2.f);
        REQUIRE(*(--it) == 1.f);
        REQUIRE(*(it--) == 1.f);
        REQUIRE(*it == 0.f);

        REQUIRE(*(it + 0) == 0.f);
        REQUIRE(*it == 0.f);
        REQUIRE(*(it + 1) == 1.f);
        REQUIRE(*it == 0.f);
        REQUIRE(*(it + 2) == 2.f);
        REQUIRE(*it == 0.f);
        REQUIRE(*(it + 8) == 8.f);
        REQUIRE(*it == 0.f);

        REQUIRE(*(it += 0) == 0.f);
        REQUIRE(*it == 0.f);
        REQUIRE(*(it += 2) == 2.f);
        REQUIRE(*it == 2.f);
        REQUIRE(*(it += 2) == 4.f);
        REQUIRE(*it == 4.f);
        REQUIRE(*(it += 2) == 6.f);
        REQUIRE(*it == 6.f);
        REQUIRE(*(it += 2) == 8.f);
        REQUIRE(*it == 8.f);

        REQUIRE(*(it += 1) == 9.f);
        REQUIRE((it += 1) == a.end());

        REQUIRE(*(it - 1) == 9.f);
        REQUIRE(*(it - 2) == 8.f);
        REQUIRE(*(it - 3) == 7.f);
        REQUIRE(it == a.end());

        REQUIRE(*(it -= 1) == 9.f);
        REQUIRE(*(it -= 2) == 7.f);
        REQUIRE(*(it -= 3) == 4.f);
        REQUIRE(*it == 4.f);

        REQUIRE(a.end() - a.begin() == 10);
        it = a.begin();
        REQUIRE(it[0] == 0.f);
        REQUIRE(it[1] == 1.f);
        REQUIRE(it[2] == 2.f);
        REQUIRE(it[3] == 3.f);
        REQUIRE(it[4] == 4.f);
        REQUIRE(it[5] == 5.f);
        REQUIRE(it[6] == 6.f);
        REQUIRE(it[7] == 7.f);
        REQUIRE(it[8] == 8.f);
        REQUIRE(it[9] == 9.f);

        it[0] = 20;
        REQUIRE(a[0] == 20.f);
    }

    SECTION("resize")
    {
        Array a("array1");
        REQUIRE(a.size() == 10);
        REQUIRE(a.resize(15));
        REQUIRE(a.size() == 15);

        Array b("not-exists");
        REQUIRE_FALSE(b.resize(10000));
        REQUIRE(b.size() == 0);

        Array c("array1");
        REQUIRE(a.size() == 15);

        REQUIRE(c[0] == 20.f);
        REQUIRE(c[1] == 1.f);
        REQUIRE(c[2] == 2.f);
        REQUIRE(c[3] == 3.f);
        REQUIRE(c[4] == 4.f);
        REQUIRE(c[5] == 5.f);
        REQUIRE(c[6] == 6.f);
        REQUIRE(c[7] == 7.f);
        REQUIRE(c[8] == 8.f);
        REQUIRE(c[9] == 9.f);
        REQUIRE(c[10] == 0.f);
        REQUIRE(c[11] == 0.f);
        REQUIRE(c[12] == 0.f);
        REQUIRE(c[13] == 0.f);
        REQUIRE(c[14] == 0.f);
    }

    SECTION("copyFrom")
    {
        t_sample data[5] = { -1, 0, 1, 2, 3 };

        Array b("array1");
        b.copyFrom(data, 5);

        REQUIRE(b[0] == -1.f);
        REQUIRE(b[1] == 0.f);
        REQUIRE(b[2] == 1.f);
        REQUIRE(b[3] == 2.f);
        REQUIRE(b[4] == 3.f);
        REQUIRE(b[5] == 5.f);
        REQUIRE(b[6] == 6.f);
        REQUIRE(b[7] == 7.f);
        REQUIRE(b[8] == 8.f);
        REQUIRE(b[9] == 9.f);
    }

    SECTION("copyTo")
    {
        float data[5] = { 0 };
        Array b("array1");
        b.fillWith(generateLin);
        //        b.copyTo(data, 5);
        // strange fail on GCC coverage
        std::copy(b.begin(), b.begin() + 5, data);

        REQUIRE(data[0] == 0.f);
        REQUIRE(data[1] == 1.f);
        REQUIRE(data[2] == 2.f);
        REQUIRE(data[3] == 3.f);
        REQUIRE(data[4] == 4.f);
    }

    SECTION("readSafe0")
    {
        Array a("array1", { 1, 2, 3, 2, 1 });
        REQUIRE(a.isValid());

        REQUIRE(a.readSafe0(0) == 1);
        REQUIRE(a.readSafe0(1) == 2);
        REQUIRE(a.readSafe0(2) == 3);
        REQUIRE(a.readSafe0(3) == 2);
        REQUIRE(a.readSafe0(4) == 1);

        // fractional
        REQUIRE(a.readSafe0(0.1) == 1);
        REQUIRE(a.readSafe0(0.999) == 1);
        REQUIRE(a.readSafe0(1) == 2);
        REQUIRE(a.readSafe0(1.1) == 2);
        REQUIRE(a.readSafe0(4) == 1);

        // invalid index
        REQUIRE(a.readSafe0(-0.001) == 0);
        REQUIRE(a.readSafe0(-10.001) == 0);
        REQUIRE(a.readSafe0(-10000.001) == 0);
        REQUIRE(a.readSafe0(4) == 1);
        REQUIRE(a.readSafe0(4.01) == 1);
        REQUIRE(a.readSafe0(4.999) == 1);
        REQUIRE(a.readSafe0(5) == 0);
    }

    SECTION("readSafe1")
    {
        Array a("array1", { 1, 2, 3, 2, 1 });
        REQUIRE(a.isValid());

        // fractional
        REQUIRE(a.readSafe1(0) == 1);
        REQUIRE(a.readSafe1(0.25) == 1.25);
        REQUIRE(a.readSafe1(0.5) == 1.5);
        REQUIRE(a.readSafe1(0.75) == 1.75);
        REQUIRE(a.readSafe1(1) == 2);
        REQUIRE(a.readSafe1(1.25) == 2.25);
        REQUIRE(a.readSafe1(1.5) == 2.5);
        REQUIRE(a.readSafe1(1.75) == 2.75);
        REQUIRE(a.readSafe1(2) == 3);
        REQUIRE(a.readSafe1(2.25) == 2.75);
        REQUIRE(a.readSafe1(2.5) == 2.5);
        REQUIRE(a.readSafe1(2.75) == 2.25);
        REQUIRE(a.readSafe1(3) == 2);
        REQUIRE(a.readSafe1(3.25) == 1.75);
        REQUIRE(a.readSafe1(3.5) == 1.5);
        REQUIRE(a.readSafe1(3.75) == 1.25);
        REQUIRE(a.readSafe1(4) == 1);
        REQUIRE(a.readSafe1(4.25) == 1);
        REQUIRE(a.readSafe1(4.5) == 1);
        REQUIRE(a.readSafe1(4.75) == 1);
        REQUIRE(a.readSafe1(4.999) == 1);
        REQUIRE(a.readSafe1(5) == 0);

        // out of range
        REQUIRE(a.readSafe1(-0.01) == 0);
        REQUIRE(a.readSafe1(-100000.01) == 0);
        REQUIRE(a.readSafe1(5) == 0);
        REQUIRE(a.readSafe1(5.1) == 0);
        REQUIRE(a.readSafe1(50000) == 0);
    }

    SECTION("readSafe3")
    {
        Array a("array1", { 1, 2, 3, 2, 1 });
        REQUIRE(a.isValid());

        // fractional
        REQUIRE(a.readSafe3(0) == 1);
        REQUIRE(a.readSafe3(0.25) == Approx(1.17969));
        REQUIRE(a.readSafe3(0.5) == Approx(1.4375));
        REQUIRE(a.readSafe3(0.75) == Approx(1.72656));
        REQUIRE(a.readSafe3(1) == 2);
        REQUIRE(a.readSafe3(1.25) == Approx(2.29688));
        REQUIRE(a.readSafe3(1.5) == Approx(2.625));
        REQUIRE(a.readSafe3(1.75) == Approx(2.89062));
        REQUIRE(a.readSafe3(2) == 3);
        REQUIRE(a.readSafe3(2.25) == Approx(2.89062));
        REQUIRE(a.readSafe3(2.5) == Approx(2.625));
        REQUIRE(a.readSafe3(2.75) == Approx(2.29688));
        REQUIRE(a.readSafe3(3) == 2);
        REQUIRE(a.readSafe3(3.25) == Approx(1.72656));
        REQUIRE(a.readSafe3(3.5) == Approx(1.4375));
        REQUIRE(a.readSafe3(3.75) == Approx(1.17969));
        REQUIRE(a.readSafe3(4) == 1);
        REQUIRE(a.readSafe3(4.25) == Approx(0.92969));
        REQUIRE(a.readSafe3(4.5) == Approx(0.9375));
        REQUIRE(a.readSafe3(4.75) == Approx(0.97656));
        REQUIRE(a.readSafe3(5) == 0);

        // out of range
        REQUIRE(a.readSafe3(-0.01) == 0);
        REQUIRE(a.readSafe3(-100000.01) == 0);
        REQUIRE(a.readSafe3(5) == 0);
        REQUIRE(a.readSafe3(5.1) == 0);
        REQUIRE(a.readSafe3(50000) == 0);
    }

    SECTION("readUnsafe0")
    {
        Array a("array1", { 1, 2, 3, 2, 1 });
        REQUIRE(a.isValid());

        REQUIRE(a.readUnsafe0(-0.9) == 1);
        REQUIRE(a.readUnsafe0(-0.001) == 1);
        REQUIRE(a.readUnsafe0(0) == 1);
        REQUIRE(a.readUnsafe0(0.1) == 1);
        REQUIRE(a.readUnsafe0(0.99) == 1);
        REQUIRE(a.readUnsafe0(1) == 2);
        REQUIRE(a.readUnsafe0(1.1) == 2);
        REQUIRE(a.readUnsafe0(1.99) == 2);
        REQUIRE(a.readUnsafe0(2) == 3);
        REQUIRE(a.readUnsafe0(2.1) == 3);
        REQUIRE(a.readUnsafe0(2.99) == 3);
        REQUIRE(a.readUnsafe0(3) == 2);
        REQUIRE(a.readUnsafe0(3.1) == 2);
        REQUIRE(a.readUnsafe0(3.99) == 2);
        REQUIRE(a.readUnsafe0(4) == 1);
        REQUIRE(a.readUnsafe0(4.01) == 1);
        REQUIRE(a.readUnsafe0(4.99) == 1);
    }

    SECTION("readUnsafe1")
    {
        Array a("array1", { 1, 2, 3, 2, 1 });
        REQUIRE(a.isValid());

        REQUIRE(a.readUnsafe1(-0.25) == 0.75);
        REQUIRE(a.readUnsafe1(0) == 1);
        REQUIRE(a.readUnsafe1(0.25) == 1.25);
        REQUIRE(a.readUnsafe1(1) == 2);
        REQUIRE(a.readUnsafe1(1.75) == 2.75);
        REQUIRE(a.readUnsafe1(2) == 3);
        REQUIRE(a.readUnsafe1(2.75) == 2.25);
        REQUIRE(a.readUnsafe1(3) == 2);
        REQUIRE(a.readUnsafe1(3.25) == 1.75);
        REQUIRE(a.readUnsafe1(3.5) == 1.5);
        REQUIRE(a.readUnsafe1(3.75) == 1.25);
    }

    SECTION("readUnsafe3")
    {
        Array a("array1", { 1, 2, 3, 2, 1 });
        REQUIRE(a.isValid());

        REQUIRE(a.readSafe3(1) == 2);
        REQUIRE(a.readSafe3(1.25) == Approx(2.29688));
        REQUIRE(a.readSafe3(1.5) == Approx(2.625));
        REQUIRE(a.readSafe3(1.75) == Approx(2.89062));
        REQUIRE(a.readSafe3(2) == 3);
        REQUIRE(a.readSafe3(2.25) == Approx(2.89062));
        REQUIRE(a.readSafe3(2.5) == Approx(2.625));
        REQUIRE(a.readSafe3(2.75) == Approx(2.29688));
    }

    SECTION("ringPushBack")
    {
        Array a("array1", { 1, 2, 3 });
        REQUIRE(a.isValid());

        a.ringPushBack(4);
        REQUIRE(a.size() == 3);
        REQUIRE(a[0] == 2);
        REQUIRE(a[1] == 3);
        REQUIRE(a[2] == 4);

        a.ringPushBack(5);
        REQUIRE(a.size() == 3);
        REQUIRE(a[0] == 3);
        REQUIRE(a[1] == 4);
        REQUIRE(a[2] == 5);
    }

    SECTION("ringPushFront")
    {
        Array a("array1", { 1, 2, 3 });
        REQUIRE(a.isValid());

        a.ringPushFront(0);
        REQUIRE(a.size() == 3);
        REQUIRE(a[0] == 0);
        REQUIRE(a[1] == 1);
        REQUIRE(a[2] == 2);

        a.ringPushFront(-0.5);
        REQUIRE(a.size() == 3);
        REQUIRE(a[0] == -0.5);
        REQUIRE(a[1] == 0);
        REQUIRE(a[2] == 1);
    }
}
