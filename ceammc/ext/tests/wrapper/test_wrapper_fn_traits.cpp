/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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

#include "wrapper_fn_traits.h"
#include "test_wrapper_base.h"

using namespace wrapper;

struct TestClass {
    static bool static_method0();
    static void static_method1();
    static bool static_method2(int, float);
    static void static_method3(int);
    static void static_method4(int&);
    static void static_method5(const int);
    static void static_method6(const int&);

    void method0();
    void method1() const;
    bool method2() const;
    bool method3(double) const;
    std::tuple<int, bool> method4(const int&, const std::string&, std::tuple<int, float>) const;
};

template <typename T, typename... Args>
struct equal_args : std::is_same<std::tuple<Args...>, typename FunctionTraits<T>::arguments>::type {
};

template <typename T, typename Return>
struct equal_return : std::is_same<Return, typename FunctionTraits<T>::return_type>::type {
};

TEST_CASE("wrapper fn_traits", "[class-wrapper]")
{
    SECTION("class static")
    {
        using T0 = decltype(&TestClass::static_method0);
        REQUIRE((FunctionTraits<T0>::nargs == 0));
        REQUIRE((equal_return<T0, bool>::value));
        REQUIRE((equal_args<T0>::value));

        using T1 = decltype(&TestClass::static_method1);
        REQUIRE((FunctionTraits<T1>::nargs == 0));
        REQUIRE((equal_return<T1, void>::value));
        REQUIRE((equal_args<T1>::value));

        using T2 = decltype(&TestClass::static_method2);
        REQUIRE((FunctionTraits<T2>::nargs == 2));
        REQUIRE((equal_return<T2, bool>::value));
        REQUIRE((equal_args<T2, int, float>::value));

        using T3 = decltype(&TestClass::static_method3);
        REQUIRE((equal_args<T3, int>::value));

        using T4 = std::remove_pointer<decltype(&TestClass::static_method4)>::type;
        REQUIRE((equal_args<T4, int>::value));

        using T5 = decltype(&TestClass::static_method5);
        REQUIRE((equal_args<T5, int>::value));

        using T6 = decltype(&TestClass::static_method6);
        REQUIRE((equal_args<T6, int>::value));
    }

    SECTION("class member")
    {
        using T0 = decltype(&TestClass::method0);
        REQUIRE((FunctionTraits<T0>::nargs == 0));
        REQUIRE((equal_return<T0, void>::value));
        REQUIRE((equal_args<T0>::value));

        using T1 = decltype(&TestClass::method1);
        REQUIRE((FunctionTraits<T1>::nargs == 0));
        REQUIRE((equal_return<T0, void>::value));
        REQUIRE((equal_args<T1>::value));

        {
            using T = decltype(&TestClass::method2);
            REQUIRE((FunctionTraits<T>::nargs == 0));
            REQUIRE((equal_return<T, bool>::value));
            REQUIRE((equal_args<T>::value));
        }

        {
            using T = decltype(&TestClass::method3);
            REQUIRE((FunctionTraits<T>::nargs == 1));
            REQUIRE((equal_return<T, bool>::value));
            REQUIRE((equal_args<T, double>::value));
        }

        {
            using T = decltype(&TestClass::method4);
            REQUIRE((FunctionTraits<T>::nargs == 3));
            REQUIRE((equal_return<T, std::tuple<int, bool>>::value));
            REQUIRE((equal_args<T, int, std::string, std::tuple<int, float>>::value));
        }
    }
}
