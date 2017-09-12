#include "base_extension_test.h"
#include "catch.hpp"

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
        REQUIRE(array1->name() == "array1");
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
        float data[5] = { -1, 0, 1, 2, 3 };

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
}
