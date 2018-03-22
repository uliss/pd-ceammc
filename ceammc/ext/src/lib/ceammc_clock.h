#ifndef CEAMMC_CLOCK_H
#define CEAMMC_CLOCK_H

#include "m_pd.h"

namespace ceammc {

template <class T>
class ClockFunction {
    t_clock* clock_;

public:
    typedef void (*FunPtr)(T*);

protected:
    FunPtr fn_;
    T* arg_;

public:
    ClockFunction(FunPtr fn, T* arg)
        : clock_(0)
        , fn_(fn)
        , arg_(arg)
    {
        clock_ = clock_new(static_cast<void*>(arg_), reinterpret_cast<t_method>(fn_));
    }

    void unset()
    {
        clock_unset(clock_);
    }

    void delay(double ms)
    {
        clock_delay(clock_, ms);
    }

    ~ClockFunction()
    {
        clock_free(clock_);
    }
};

template <class T>
class ClockMemberFunction {
public:
    typedef void (T::*MemberFunPtr)();

public:
    ClockMemberFunction(T* this__, MemberFunPtr fn)
        : clock_(0)
        , this_(this__)
        , mem_fn_(fn)
    {
        clock_ = clock_new(static_cast<void*>(this),
            reinterpret_cast<t_method>(&ClockMemberFunction<T>::callback));
    }

    ~ClockMemberFunction()
    {
        clock_free(clock_);
    }

    void unset()
    {
        clock_unset(clock_);
    }

    void delay(double ms)
    {
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
    MemberFunPtr mem_fn_;
    T* this_;
    t_clock* clock_;
};
}

#endif // CEAMMC_CLOCK_H
