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

#include "../class-wrapper/src/wrapper_fn_traits.h"
#include "../class-wrapper/src/wrapper_tuple.h"
#include "catch.hpp"

struct CallbackMultiply {
    int times_;
    std::vector<int> idx_;

    CallbackMultiply(int times = 1)
        : times_(times)
    {
    }

    void operator()(int idx, float& f)
    {
        idx_.push_back(idx);
        f *= times_;
    }

    void operator()(int idx, int& i)
    {
        idx_.push_back(idx);
        i *= times_;
    }

    void operator()(int idx, std::string& s)
    {
        idx_.push_back(idx);

        std::string res;
        for (int i = 0; i < times_; i++)
            res += s;

        s = res;
    }
};

TEST_CASE("wrapper template", "[class-wrapper]")
{
    SECTION("tuple_utils")
    {
        using namespace wrapper::tuple_utils;

        SECTION("for_each")
        {
            using Arg = std::tuple<float, int, std::string>;
            Arg t(1.5f, -100, "test");

            // rvalue
            for_each(t, CallbackMultiply(2));
            REQUIRE(std::get<0>(t) == 3.f);
            REQUIRE(std::get<1>(t) == -200);
            REQUIRE(std::get<2>(t) == "testtest");

            // lvalue
            CallbackMultiply mul4(3);
            for_each(t, mul4);
            REQUIRE(std::get<0>(t) == 9.f);
            REQUIRE(std::get<1>(t) == -600);
            REQUIRE(std::get<2>(t) == "testtesttesttesttesttest");

            REQUIRE(mul4.idx_ == std::vector<int>({ 0, 1, 2 }));
        }

        SECTION("rfor_each")
        {
            using Arg = std::tuple<float, int, std::string>;
            Arg t(1.5f, -100, "test");

            // rvalue
            rfor_each(t, CallbackMultiply(2));
            REQUIRE(std::get<0>(t) == 3.f);
            REQUIRE(std::get<1>(t) == -200);
            REQUIRE(std::get<2>(t) == "testtest");

            // lvalue
            CallbackMultiply mul4(3);
            rfor_each(t, mul4);
            REQUIRE(std::get<0>(t) == 9.f);
            REQUIRE(std::get<1>(t) == -600);
            REQUIRE(std::get<2>(t) == "testtesttesttesttesttest");

            REQUIRE(mul4.idx_ == std::vector<int>({ 2, 1, 0 }));
        }

        SECTION("find_first/last")
        {
            struct IsFloat {
                bool operator()(int idx, float& f)
                {
                    return true;
                }

                bool operator()(int idx, bool b)
                {
                    return false;
                }
            };

            SECTION("empty")
            {
                auto t = std::tuple<>();
                REQUIRE(find_first(t, IsFloat()) == -1);
                REQUIRE(find_last(t, IsFloat()) == -1);
            }

            SECTION("single")
            {
                auto t = std::tuple<float>(1);
                REQUIRE(find_first(t, IsFloat()) == 0);
                REQUIRE(find_last(t, IsFloat()) == 0);
            }

            SECTION("two")
            {
                auto t = std::tuple<bool, float>(true, 1);
                REQUIRE(find_first(t, IsFloat()) == 1);
                REQUIRE(std::get<1>(t) == 1);
                REQUIRE(find_last(t, IsFloat()) == 1);
            }

            SECTION("many")
            {
                auto t = std::tuple<bool, bool, float, bool, float>(true, true, 1.1, false, 2.2);
                REQUIRE(find_first(t, IsFloat()) == 2);
                REQUIRE(std::get<2>(t) == Approx(1.1));
                REQUIRE(find_last(t, IsFloat()) == 4);
            }

            struct Less3 {
                float value;
                int idx;

                Less3()
                    : value(0)
                    , idx(-1)
                {
                }

                bool operator()(int i, float f)
                {
                    bool res = (f < 3);

                    if (res) {
                        idx = i;
                        value = f;
                    }

                    return res;
                }
            };

            std::tuple<float, float, float, float> t(1, 2, 3, 4);
            REQUIRE(find_first(t, Less3()) == 0);
            t = std::make_tuple(4.f, 3.f, 2.f, 1.f);
            REQUIRE(find_first(t, Less3()) == 2);
            t = std::make_tuple(3.f, 4.f, 5.f, 6.f);
            REQUIRE(find_first(t, Less3()) == -1);
        }

        SECTION("2 tuples")
        {
            using Tuple = std::tuple<int, bool, float>;
            Tuple t0(3, true, 1.5);
            Tuple t1(4, false, 4);

            struct FloatMul {
                int call_count;

                FloatMul()
                    : call_count(0)
                {
                }

                bool operator()(int i, float& f0, float& f1)
                {
                    call_count++;
                    f0 *= f1;
                    return true;
                }

                bool operator()(int i, int&, int&)
                {
                    call_count++;
                    return false;
                }

                bool operator()(int i, bool&, bool&)
                {
                    call_count++;
                    return false;
                }
            };

            struct IntMul {
                bool operator()(int i, float&, float&)
                {
                    return false;
                }

                bool operator()(int i, int& i0, int& i1)
                {
                    i0 *= i1;
                    return true;
                }

                bool operator()(int i, bool&, bool&)
                {
                    return false;
                }
            };

            REQUIRE(find_first(t0, t1, FloatMul()) == 2);
            REQUIRE(std::get<2>(t0) == 6);
            REQUIRE(find_first(t0, t1, FloatMul()) == 2);
            REQUIRE(std::get<2>(t0) == 24);

            REQUIRE(find_first(t0, t1, IntMul()) == 0);
            REQUIRE(std::get<0>(t0) == 12);
            REQUIRE(find_first(t0, t1, IntMul()) == 0);
            REQUIRE(std::get<0>(t0) == 48);

            SECTION("call_count")
            {
                struct IsBool {
                    int count;

                    IsBool()
                        : count(0)
                    {
                    }

                    bool operator()(int i, float&, float&)
                    {
                        count++;
                        return false;
                    }

                    bool operator()(int i, int& i0, int& i1)
                    {
                        count++;
                        return false;
                    }

                    bool operator()(int i, bool&, bool&)
                    {
                        count++;
                        return true;
                    }
                };

                struct IsInt {
                    int count;

                    IsInt()
                        : count(0)
                    {
                    }

                    bool operator()(int i, float&, float&)
                    {
                        count++;
                        return false;
                    }

                    bool operator()(int i, int& i0, int& i1)
                    {
                        count++;
                        return true;
                    }

                    bool operator()(int i, bool&, bool&)
                    {
                        count++;
                        return false;
                    }
                };

                IsBool is_bool;
                REQUIRE(find_first(t0, t1, is_bool) == 1);
                REQUIRE(is_bool.count == 2);

                IsInt is_int;
                REQUIRE(find_first(t0, t1, is_int) == 0);
                REQUIRE(is_int.count == 1);
            }
        }

        SECTION("has_type")
        {
            REQUIRE_FALSE((has_type<float, std::tuple<>>::value));
            REQUIRE((has_type<float, std::tuple<float>>::value));
            REQUIRE((has_type<float, std::tuple<int, float>>::value));
            REQUIRE_FALSE((has_type<float, std::tuple<bool>>::value));
            REQUIRE((has_type<float, std::tuple<bool, int, long, float>>::value));
        }

        SECTION("method arguments")
        {
            typedef bool (*FnType0)(float);
            typedef bool (*FnType1)(float&);
            typedef void (*FnType2)(long, long);
            typedef void (*FnType3)(long, long&);
            typedef void (*FnType4)(long, const long&);
            typedef void (*FnType5)();
            typedef std::vector<int> (*FnType6)(const std::string&);

            using Args = wrapper::MethodArgList<
                std::tuple<FnType0, FnType1, FnType2, FnType3, FnType4, FnType5, FnType6>>::type;

            auto check0 = std::is_same<std::tuple_element<0, Args>::type, std::tuple<float>>::value;
            REQUIRE(check0);
            auto check1 = std::is_same<std::tuple_element<2, Args>::type, std::tuple<long, long>>::value;
            REQUIRE(check1);
            auto check = std::is_same<std::tuple_element<3, Args>::type, std::tuple<long, long>>::value;
            REQUIRE(check);
            check = std::is_same<std::tuple_element<4, Args>::type, std::tuple<long, long>>::value;
            REQUIRE(check);
            check = std::is_same<std::tuple_element<5, Args>::type, std::tuple<>>::value;
            REQUIRE(check);
            check = std::is_same<std::tuple_element<1, Args>::type, std::tuple<float>>::value;
            REQUIRE(check1);
        }

        SECTION("function_traits")
        {
            using namespace wrapper;

            SECTION("plain functions")
            {
                typedef void (*FnType0)();
                typedef void (*FnType1)(float);
                typedef bool (*FnType2)();
                typedef float (*FnType3)(int, long&);
                typedef float (*FnType4)(const int, const long&);

                // type0
                auto v = std::is_same<FunctionTraits<FnType0>::return_type, void>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType0>::arguments, std::tuple<>>::value;
                REQUIRE(v);

                auto nargs = FunctionTraits<FnType0>::nargs;
                auto nouts = FunctionTraits<FnType0>::nouts;
                REQUIRE(nargs == 0);
                REQUIRE(nouts == 0);

                // type1
                nargs = FunctionTraits<FnType1>::nargs;
                nouts = FunctionTraits<FnType1>::nouts;
                REQUIRE(nargs == 1);
                REQUIRE(nouts == 0);
                v = std::is_same<FunctionTraits<FnType1>::return_type, void>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType1>::arguments, std::tuple<float>>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType1>::argument<0>::type, float>::value;
                REQUIRE(v);

                // type2
                nargs = FunctionTraits<FnType2>::nargs;
                nouts = FunctionTraits<FnType2>::nouts;
                REQUIRE(nargs == 0);
                REQUIRE(nouts == 1);
                v = std::is_same<FunctionTraits<FnType2>::return_type, bool>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType2>::arguments, std::tuple<>>::value;
                REQUIRE(v);

                // type3
                nargs = FunctionTraits<FnType3>::nargs;
                nouts = FunctionTraits<FnType3>::nouts;
                REQUIRE(nargs == 2);
                REQUIRE(nouts == 1);
                v = std::is_same<FunctionTraits<FnType3>::return_type, float>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType3>::arguments, std::tuple<int, long>>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType3>::argument<0>::type, int>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType3>::argument<1>::type, long&>::value;
                REQUIRE(v);

                // type4
                nargs = FunctionTraits<FnType4>::nargs;
                nouts = FunctionTraits<FnType4>::nouts;
                REQUIRE(nargs == 2);
                REQUIRE(nouts == 1);
                v = std::is_same<FunctionTraits<FnType4>::return_type, float>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType4>::arguments, std::tuple<int, long>>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType4>::argument<0>::type, int>::value;
                REQUIRE(v);
                v = std::is_same<FunctionTraits<FnType4>::argument<1>::type, const long&>::value;
                REQUIRE(v);
            }

            SECTION("tail")
            {
                static_assert(std::is_same<tail<std::tuple<>>::type, std::tuple<>>::value, "");
                static_assert(std::is_same<tail<std::tuple<float>>::type, std::tuple<>>::value, "");

                static_assert(std::is_same<tail<std::tuple<float, int>>::type, std::tuple<int>>::value, "");
                static_assert(std::is_same<tail<std::tuple<float, int, bool>>::type, std::tuple<int, bool>>::value, "");
            }

            SECTION("get_tail")
            {

                REQUIRE((get_tail(std::tuple<int, int, bool>(1, 2, true)) == std::tuple<int, bool>(2, true)));
                REQUIRE((get_tail(std::tuple<int, bool>(2, true)) == std::tuple<bool>(true)));
                REQUIRE((get_tail(std::tuple<bool>(true)) == std::tuple<>()));
            }
        }
    }
}
