#ifndef CEAMMC_CLOCK_H
#define CEAMMC_CLOCK_H

#include "m_pd.h"

#include <functional>

namespace ceammc {

class ClockLambdaFunction {
    std::function<void()> fn_;
    t_clock* clock_;

public:
    ClockLambdaFunction(std::function<void()> fn)
        : fn_(fn)
        , clock_(nullptr)
    {
        clock_ = clock_new(static_cast<void*>(this), reinterpret_cast<t_method>(tick));
    }

    void unset()
    {
        if (clock_)
            clock_unset(clock_);
    }

    void delay(double ms)
    {
        if (clock_)
            clock_delay(clock_, ms);
    }

    ~ClockLambdaFunction()
    {
        if (clock_)
            clock_free(clock_);
    }

    static void tick(ClockLambdaFunction* fn)
    {
        fn->fn_();
    }
};

template <class T>
class ClockMemberFunction {
public:
    typedef void (T::*MemberFunPtr)();

public:
    ClockMemberFunction(T* this__, MemberFunPtr fn)
        : clock_(nullptr)
        , this_(this__)
        , mem_fn_(fn)
    {
        clock_ = clock_new(static_cast<void*>(this),
            reinterpret_cast<t_method>(&ClockMemberFunction<T>::callback));
    }

    ~ClockMemberFunction()
    {
        if (clock_)
            clock_free(clock_);
    }

    void unset()
    {
        if (clock_)
            clock_unset(clock_);
    }

    void delay(double ms)
    {
        if (clock_)
            clock_delay(clock_, ms);
    }

private:
    static void callback(ClockMemberFunction<T>* arg)
    {
        T* obj = arg->this_;
        MemberFunPtr m = arg->mem_fn_;

        (obj->*m)();
    }

private:
    t_clock* clock_;
    T* this_;
    MemberFunPtr mem_fn_;
};
}

#endif // CEAMMC_CLOCK_H
