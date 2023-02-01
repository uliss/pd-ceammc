/*
 * The MIT License (MIT)
 *
 * Copyright (C) 2018-2019 Alex Nadzharov, Serge Poltavsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// converts AtomList <-> tuple

#ifndef WRAPPER_TUPLE_H
#define WRAPPER_TUPLE_H

#include "data_iface.h"
#include "wrapper_datatype.h"
#include "wrapper_fn_traits.h"
#include "wrapper_invocations.h"
#include "wrapper_sequence.h"

#include "ceammc_atomlist.h"
#include "ceammc_data.h"
#include "ceammc_log.h"

#include <boost/variant.hpp>
#include <string>
#include <tuple>

namespace wrapper {

namespace tuple_utils {
    template <int index, typename Callback, typename Args>
    struct iterate_tuple {
        static void next(Args& t, Callback&& callback)
        {
            iterate_tuple<index - 1, Callback, Args>::next(t, std::forward<Callback>(callback));
            callback(index, std::get<index>(t));
        }
    };

    template <typename Callback, typename Args>
    struct iterate_tuple<0, Callback, Args> {
        static void next(Args& t, Callback&& callback)
        {
            callback(0, std::get<0>(t));
        }
    };

    template <typename Callback, typename Args>
    struct iterate_tuple<-1, Callback, Args> {
        static void next(Args& t, Callback&& callback)
        {
        }
    };

    // conditional iteration
    template <int index, typename Pred, typename Args>
    struct iterate_pred {
        static int next(Args& t, Pred&& pred)
        {
            int res = iterate_pred<index - 1, Pred, Args>::next(t, std::forward<Pred>(pred));

            if (res >= 0)
                return res;
            else if (pred(index, std::get<index>(t)))
                return index;
            else
                return -1;
        }
    };

    template <typename Pred, typename Args>
    struct iterate_pred<0, Pred, Args> {
        static int next(Args& t, Pred&& pred)
        {
            if (pred(0, std::get<0>(t)))
                return 0;

            return -1;
        }
    };

    template <typename Pred, typename Args>
    struct iterate_pred<-1, Pred, Args> {
        static int next(Args& t, Pred&& pred)
        {
            return -1;
        }
    };

    // conditional iteration
    template <int index, typename Pred, typename Args>
    struct riterate_pred {
        static int next(Args& t, Pred&& pred)
        {
            if (pred(index, std::get<index>(t)))
                return index;
            else
                return riterate_pred<index - 1, Pred, Args>::next(t, std::forward<Pred>(pred));
        }
    };

    template <typename Pred, typename Args>
    struct riterate_pred<0, Pred, Args> {
        static int next(Args& t, Pred&& pred)
        {
            if (pred(0, std::get<0>(t)))
                return 0;

            return -1;
        }
    };

    template <typename Pred, typename Args>
    struct riterate_pred<-1, Pred, Args> {
        static int next(Args& t, Pred&& pred)
        {
            return -1;
        }
    };

    template <int index, typename Pred, typename Tuple0, typename Tuple1>
    struct iterate_pred2 {
        static int next(Tuple0& t0, Tuple1& t1, Pred&& pred)
        {
            int idx = iterate_pred2<index - 1, Pred, Tuple0, Tuple1>::next(t0, t1,
                std::forward<Pred>(pred));

            if (idx >= 0)
                return idx;
            else if (pred(index, std::get<index>(t0), std::get<index>(t1)))
                return index;
            else
                return -1;
        }
    };

    template <typename Pred, typename Tuple0, typename Tuple1>
    struct iterate_pred2<0, Pred, Tuple0, Tuple1> {
        static int next(Tuple0& t0, Tuple1& t1, Pred&& pred)
        {
            return pred(0, std::get<0>(t0), std::get<0>(t1)) ? 0 : -1;
        }
    };

    template <typename Pred, typename Tuple0, typename Tuple1>
    struct iterate_pred2<-1, Pred, Tuple0, Tuple1> {
        static int next(Tuple0&, Tuple1&, Pred&&)
        {
            return -1;
        }
    };

    // expected callback
    // struct callback
    // {
    //     template<typename T>
    //     void operator()(int index, T&& t)
    //     {
    //         std::cout << index << " = " << t << std::endl;
    //     }
    // };
    template <typename Callback, typename Args>
    void for_each(Args& t, Callback&& callback)
    {
        int const N = std::tuple_size<Args>::value;
        iterate_tuple<N - 1, Callback, Args>::next(t, std::forward<Callback>(callback));
    }

    /**
     * @param t - tuple to process
     * @param pred - should return true if element is found
     * @return index of found element or -1 if not found
     */
    template <typename Args, typename Pred>
    int find_first(Args& t, Pred&& pred)
    {
        int const N = std::tuple_size<Args>::value;
        return iterate_pred<N - 1, Pred, Args>::next(t, std::forward<Pred>(pred));
    }

    /**
     * @param t - tuple to process
     * @param pred - should return true if element is found
     * @return index of found element or -1 if not found
     */
    template <typename Args, typename Pred>
    int find_last(Args& t, Pred&& pred)
    {
        int const N = std::tuple_size<Args>::value;
        return riterate_pred<N - 1, Pred, Args>::next(t, std::forward<Pred>(pred));
    }

    // reverse iterate
    template <int index, typename Callback, typename Args>
    struct riterate_tuple {
        static void next(Args& t, Callback&& callback)
        {
            callback(index, std::get<index>(t));
            riterate_tuple<index - 1, Callback, Args>::next(t, std::forward<Callback>(callback));
        }
    };

    template <typename Callback, typename Args>
    struct riterate_tuple<0, Callback, Args> {
        static void next(Args& t, Callback&& callback)
        {
            callback(0, std::get<0>(t));
        }
    };

    template <typename Callback, typename Args>
    struct riterate_tuple<-1, Callback, Args> {
        static void next(Args& t, Callback&& callback)
        {
        }
    };

    // expected callback
    // struct callback
    // {
    //     template<typename T>
    //     void operator()(int index, T&& t)
    //     {
    //         std::cout << index << " = " << t << std::endl;
    //     }
    // };
    template <typename Callback, typename Args>
    void rfor_each(Args& t, Callback&& callback)
    {
        int const N = std::tuple_size<Args>::value;
        riterate_tuple<N - 1, Callback, Args>::next(t, std::forward<Callback>(callback));
    }

    template <typename Pred, typename Tuple0, typename Tuple1>
    int find_first(Tuple0& t0, Tuple1& t1, Pred&& pred)
    {
        constexpr int const N0 = std::tuple_size<Tuple0>::value;
        constexpr int const N1 = std::tuple_size<Tuple1>::value;
        static_assert(N0 == N1, "different tuple sizes");
        return iterate_pred2<N0 - 1, Pred, Tuple0, Tuple1>::next(t0, t1,
            std::forward<Pred>(pred));
    }

    template <typename T, typename Tuple>
    struct has_type;

    template <typename T>
    struct has_type<T, std::tuple<>> : std::false_type {
    };

    template <typename T, typename U, typename... Ts>
    struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {
    };

    template <typename T, typename... Ts>
    struct has_type<T, std::tuple<T, Ts...>> : std::true_type {
    };

    template <typename... Types>
    struct tail;

    template <typename Head>
    struct tail<Head> {
        using type = std::tuple<>;
    };

    template <typename Head, typename... Types>
    struct tail<std::tuple<Head, Types...>> {
        using type = std::tuple<Types...>;
    };

    template <typename Tuple>
    struct head {
        using type = typename std::tuple_element<0, Tuple>::type;
    };

    template <typename Head, int... Ns, typename... Types>
    std::tuple<Types...> tail_impl(IntSequence<Ns...>, const std::tuple<Head, Types...>& t)
    {
        return std::make_tuple(std::get<Ns + 1>(t)...);
    }

    template <typename Head, typename... Types>
    std::tuple<Types...> get_tail(const std::tuple<Head, Types...>& t)
    {
        return tail_impl(typename GenSequence<sizeof...(Types)>::type(), t);
    }
}

template <size_t IDX, size_t nArgs, typename Args, typename FromAtomListConverter>
class TupleFromAtomlistT {
    Args& output_;
    TupleFromAtomlistT<IDX - 1, nArgs, Args, FromAtomListConverter> tail_;

private:
    // converts n elements to T; erases them, returns false if l.size is zero.
    template <typename U>
    size_t process(U& v, const AtomList& l, size_t idx)
    {
        if (l.empty())
            return 0;

        return FromAtomListConverter::fromAtomList(v, l, idx);
    }

public:
    explicit TupleFromAtomlistT(Args& o)
        : output_(o)
        , tail_(output_)
    {
    }

    void operator()(const AtomList& src, size_t pos)
    {
        auto& element = std::get<nArgs - IDX>(output_);

        size_t next_pos = pos;
        if ((next_pos = process(element, src, pos)) > 0) {
            tail_(src, next_pos);
        }
    }
};

template <size_t nArgs, typename Args, typename FromAtomListConverter>
class TupleFromAtomlistT<0, nArgs, Args, FromAtomListConverter> {
public:
    TupleFromAtomlistT(Args&) {}
    void operator()(const AtomListView&, size_t) {}
};

class ArgumentTypePrinter {
    std::ostream& os_;

public:
    ArgumentTypePrinter(const char* msg, std::ostream& os)
        : os_(os)
    {
        os_ << msg;
    }

    void operator()(int, float&)
    {
        os_ << "float ";
    }

    void operator()(int, double&)
    {
        os_ << "float ";
    }

    void operator()(int, int&)
    {
        os_ << "int ";
    }

    void operator()(int, unsigned int&)
    {
        os_ << "int ";
    }
};

class ErrorMsg : boost::variant<bool, std::string> {
private:
    ErrorMsg()
        : boost::variant<bool, std::string>(true)
    {
    }

    ErrorMsg(const std::string& errMsg)
        : boost::variant<bool, std::string>(errMsg)
    {
    }

public:
    static ErrorMsg ok()
    {
        return ErrorMsg();
    }

    static ErrorMsg err(const std::string& errMsg)
    {
        return ErrorMsg(errMsg);
    }

    operator bool()
    {
        return which() == 1;
    }

    const std::string& msg() const
    {
        static std::string empty;

        if (which() == 0)
            return empty;
        else
            return boost::get<std::string>(*this);
    }
};

struct Converter {
    static size_t fromAtomList(float& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx || !l[idx].isFloat())
            return 0;

        out = l[idx].asFloat();
        return idx + 1;
    }

    static size_t fromAtomList(bool& out, const AtomListView& l, size_t idx)
    {
        static t_symbol* SYM_TRUE = gensym("true");
        static t_symbol* SYM_FALSE = gensym("false");

        if (l.size() <= idx)
            return 0;

        if (l[idx].isFloat()) {
            auto f = l[idx].asFloat();
            if (f == 1)
                out = true;
            else if (f == 0)
                out = false;
            else {
                LIB_DBG << "warning! not boolean value: " << f;
                out = (f != 0);
            }

            return idx + 1;
        } else if (l[idx].isSymbol()) {
            t_symbol* s = l[idx].asSymbol();
            if (s == SYM_TRUE) {
                out = true;
                return idx + 1;
            } else if (s == SYM_FALSE) {
                out = false;
                return idx + 1;
            } else {
                LIB_DBG << "warning! not a boolean value: " << s;
            }
        }
        return 0;
    }

    static size_t fromAtomList(double& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx || !l[idx].isFloat())
            return 0;

        out = l[idx].asFloat();
        return idx + 1;
    }

    static size_t fromAtomList(int& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx || !l[idx].isFloat())
            return 0;

        out = (int)l[idx].asFloat();
        return idx + 1;
    }

    static size_t fromAtomList(unsigned int& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx || !l[idx].isFloat())
            return 0;

        auto f = l[idx].asFloat();
        if (f < 0)
            throw std::runtime_error("negative value is not allowed");

        out = (unsigned int)f;
        return idx + 1;
    }

    static size_t fromAtomList(long& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx || !l[idx].isFloat())
            return 0;

        out = (long)l[idx].asFloat();
        return idx + 1;
    }

    static size_t fromAtomList(unsigned long& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx || !l[idx].isFloat())
            return 0;

        auto f = l[idx].asFloat();
        if (f < 0)
            throw std::runtime_error("negative value is not allowed");

        out = (unsigned long)f;
        return idx + 1;
    }

    static size_t fromAtomList(std::vector<float>& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        out.clear();
        for (int i = idx; i < l.size(); i++)
            out.push_back(l[idx + i].asFloat());

        return l.size() - idx;
    }

    static size_t fromAtomList(std::vector<double>& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        out.clear();
        for (int i = idx; i < l.size(); i++)
            out.push_back(l[idx + i].asFloat());

        return l.size() - idx;
    }

    static size_t fromAtomList(std::vector<int>& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        out.clear();
        for (int i = idx; i < l.size(); i++)
            out.push_back(l[idx + i].asFloat());

        return l.size() - idx;
    }

    static size_t fromAtomList(std::vector<std::string>& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        out.clear();
        for (int i = idx; i < l.size(); i++)
            out.push_back(ceammc::to_string(l[idx + i]));

        return l.size() - idx;
    }

    static size_t fromAtomList(std::string& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        if (l[idx].isSymbol())
            out = l[idx].asSymbol()->s_name;
        else
            out = ceammc::to_string(l[idx]);

        return idx + 1;
    }

    static size_t fromAtomList(AtomList& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        out = l.subView(idx);
        return l.size() - idx;
    }

    static size_t fromAtomList(Atom& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        out = l[idx];
        return idx + 1;
    }

    static size_t fromAtomList(t_symbol*& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        if (!l[idx].isSymbol())
            return 0;

        out = l[idx].asSymbol();
        return idx + 1;
    }

    template <typename T>
    static size_t fromAtomList(T& out, const AtomListView& l, size_t idx)
    {
        if (l.size() <= idx)
            return 0;

        if (l[idx].isData()) {
            using WrappedData = AbstractDataWrapper<T>;

            if (l[idx].isA<WrappedData>()) {
                out = l[idx].asD<WrappedData>()->value();
                return idx + 1;
            } else {
                LIB_ERR << "unsupported data type: " << l[idx].asData()->typeName();
                return 0;
            }
        } else {
            Result r = out.setFromPd(l.subView(idx));

            std::string msg;
            if (r.error(&msg)) {
                LIB_ERR << msg;
                return 0;
            }

            return l.size() - idx;
        }
    }
};

template <size_t N, typename Args>
class InletArgFromAtomList {
    Args& args_;

public:
    InletArgFromAtomList(Args& args)
        : args_(args)
    {
    }

    ErrorMsg setNthArg(size_t n, const AtomListView& lv)
    {
        try {
            if (n < N) {
                InletArgFromAtomList<N - 1, Args> tail = InletArgFromAtomList<N - 1, Args>(args_);
                return tail.setNthArg(n, lv);
            } else if (n > N) {
                std::ostringstream s;
                s << "invalid inlet number: " << n;
                return ErrorMsg::err(s.str());
            }

            auto& nth_arg = std::get<N - 1>(args_);
            using ArgType = typename std::remove_const<decltype(nth_arg)>::type;
            ErrorMsg err = inletArgFromAtomList((ArgType&)nth_arg, lv);

            if (err)
                return err;
            else
                return ErrorMsg::ok();

        } catch (std::exception& e) {
            return ErrorMsg::err(e.what());
        }
    }

public:
    static ErrorMsg inletArgFromAtomList(float& f, const AtomListView& lv)
    {
        if (!lv.isFloat())
            return ErrorMsg::err("float value expected");

        f = lv.asFloat();
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(double& f, const AtomListView& lv)
    {
        if (!lv.isFloat())
            return ErrorMsg::err("float value expected");

        f = lv.asFloat();
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(bool& b, const AtomListView& lv)
    {
        static t_symbol* SYM_TRUE = gensym("true");
        static t_symbol* SYM_FALSE = gensym("false");

        if (!lv.isFloat()) {
            if (lv.isSymbol()) {
                t_symbol* s = lv[0].asSymbol();
                if (s == SYM_TRUE)
                    b = true;
                else if (s == SYM_FALSE)
                    b = false;

                return ErrorMsg::err("boolean value expected");
            }
        }

        t_float f = lv[0].asFloat();
        if (f == 0)
            b = false;
        else if (f == 1)
            b = true;
        else {
            LIB_DBG << "warning! non boolean value: " << f;
            b = (f != 0);
        }

        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(int& i, const AtomListView& lv)
    {
        if (!lv.isFloat())
            return ErrorMsg::err("integer value expected");

        i = lv.asInt();
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(std::vector<int>& v, const AtomListView& lv)
    {
        v.clear();

        for (auto& e : lv)
            v.push_back(e.asFloat());

        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(std::vector<float>& v, const AtomListView& lv)
    {
        v.clear();

        for (auto& e : lv)
            v.push_back(e.asFloat());

        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(std::vector<double>& v, const AtomListView& lv)
    {
        v.clear();

        for (auto& e : lv)
            v.push_back(e.asFloat());

        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(std::vector<std::string>& v, const AtomListView& lv)
    {
        v.clear();

        for (auto& e : lv)
            v.push_back(ceammc::to_string(e));

        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(unsigned int& i, const AtomListView& lv)
    {
        if (!lv.isFloat())
            return ErrorMsg::err("integer value expected");

        if (lv[0].asFloat() < 0) {
            std::ostringstream ss;
            ss << "non-negative value expected: " << lv[0].asFloat();
            return ErrorMsg::err(ss.str());
        }

        i = lv.asInt();
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(long& i, const AtomListView& lv)
    {
        if (!lv.isFloat())
            return ErrorMsg::err("integer value expected");

        i = lv.asInt();
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(unsigned long& i, const AtomListView& lv)
    {
        if (!lv.isFloat())
            return ErrorMsg::err("integer value expected");

        if (lv[0].asFloat() < 0) {
            std::ostringstream ss;
            ss << "non-negative value expected: " << lv[0].asFloat();
            return ErrorMsg::err(ss.str());
        }

        i = lv.asInt();
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(std::string& str, const AtomListView& lv)
    {
        str = ceammc::to_string(lv, " ");
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(Atom& a, const AtomListView& lv)
    {
        if (lv.size() != 1) {
            std::ostringstream ss;
            ss << "single atom expected: " << lv;
            return ErrorMsg::err(ss.str());
        }

        a = lv[0];
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(AtomList& l, const AtomListView& lv)
    {
        l = lv;
        return ErrorMsg::ok();
    }

    static ErrorMsg inletArgFromAtomList(t_symbol*& s, const AtomListView& lv)
    {
        if (lv.size() != 1 || !lv[0].isSymbol()) {
            std::ostringstream ss;
            ss << "symbol expected: " << lv;
            return ErrorMsg::err(ss.str());
        }

        s = lv[0].asSymbol();
        return ErrorMsg::ok();
    }

    template <typename T>
    static ErrorMsg inletArgFromAtomList(T& t, const AtomListView& lv)
    {
        // list initializers
        if (!lv.isData()) {
            Result res = t.setFromPd(lv);
            return ErrorMsg::ok();
        }

        // data atom initializer
        Atom datom(lv.at(0));
        if (!datom.isData())
            return ErrorMsg::err("invalid data pointer");

        // not a wrapper type
        if (!datom.isA<AbstractDataWrapper<T>>()) {
            std::ostringstream s;
            s << "datatype expected: " << T::typeName();
            return ErrorMsg::err(s.str());
        }

        const auto* dptr = datom.asD<AbstractDataWrapper<T>>();
        t = dptr->value();
        return ErrorMsg::ok();
    }
};

template <typename Arguments>
class ArgumentInitializer {
    Arguments& args_;

public:
    explicit ArgumentInitializer(Arguments& args)
        : args_(args)
    {
    }

    void parse(const AtomListView& src, size_t from)
    {
        constexpr size_t N = std::tuple_size<Arguments>::value;
        TupleFromAtomlistT<N, N, Arguments, Converter> proc(args_);
        proc(src, from);
    }
};

template <typename F>
static bool atomListToArguments(const AtomListView& lv, typename FunctionTraits<F>::arguments& args)
{
    ArgumentInitializer<typename FunctionTraits<F>::arguments> arg_parser(args);
    arg_parser.parse(lv, 0);
    return true;
}

template <typename Args>
class InletArgFromAtomList<0, Args> {
public:
    InletArgFromAtomList(Args&) {}
    ErrorMsg setNthArg(size_t n, const AtomListView&) { return ErrorMsg::ok(); }
};

struct ArgumentMatchAndSet {
    const AtomList& l_;

    ArgumentMatchAndSet(const AtomList& lst)
        : l_(lst)
    {
    }

    template <typename TupleElement>
    bool operator()(int index, TupleElement&& t)
    {
        return setArgs(index, t);
    }

    bool setArgs(int idx, std::tuple<>&)
    {
        return true;
    }

    bool setArgs(int idx, std::tuple<float>& f)
    {
        if (!l_.isFloat())
            return false;

        // set float value
        std::get<0>(f) = l_[0].asFloat();
        return true;
    }

    bool setArgs(int idx, std::tuple<double>& f)
    {
        if (!l_.isFloat())
            return false;

        // set float value
        std::get<0>(f) = static_cast<double>(l_[0].asFloat());
        return true;
    }

    bool setArgs(int idx, std::tuple<bool>& f)
    {
        static t_symbol* SYM_TRUE = gensym("true");
        static t_symbol* SYM_FALSE = gensym("false");

        // check for true and false symbols
        if (!l_.isFloat()) {
            if (l_.isSymbol()) {
                t_symbol* s = l_[0].asSymbol();
                return (s == SYM_TRUE || s == SYM_FALSE);
            }
            return false;
        }

        // set float value
        std::get<0>(f) = (l_[0].asFloat() != 0);
        return true;
    }

    bool setArgs(int idx, std::tuple<int>& i)
    {
        if (!l_.isFloat())
            return false;

        // set float value
        std::get<0>(i) = static_cast<int>(l_[0].asFloat());
        return true;
    }

    bool setArgs(int idx, std::tuple<long>& i)
    {
        if (!l_.isFloat())
            return false;

        // set float value
        std::get<0>(i) = static_cast<long>(l_[0].asFloat());
        return true;
    }

    bool setArgs(int idx, std::tuple<unsigned int>& i)
    {
        if (!l_.isFloat())
            return false;

        t_float f = l_[0].asFloat();
        if (f < 0)
            return false;

        // set float value
        std::get<0>(i) = static_cast<unsigned int>(f);
        return true;
    }

    bool setArgs(int idx, std::tuple<unsigned long>& i)
    {
        if (!l_.isFloat())
            return false;

        t_float f = l_[0].asFloat();
        if (f < 0)
            return false;

        // set float value
        std::get<0>(i) = static_cast<unsigned long>(f);
        return true;
    }

    template <class T>
    bool setArgs(int idx, std::tuple<T>& t)
    {
        if (!l_.isData()) {
            Result r = std::get<0>(t).setFromPd(l_);
            return r.isOk();
        }

        using ArgumentWrapper = AbstractDataWrapper<T>;

        DataAtom<ArgumentWrapper> in_ptr(l_[0]);
        if (!in_ptr.isData())
            return false;

        // set type value
        std::get<0>(t) = in_ptr->value();

        return true;
    }
};
}

#endif
