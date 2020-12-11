#ifndef CEAMMC_CLOCK_H
#define CEAMMC_CLOCK_H

#include "m_pd.h"

#include <functional>

namespace ceammc {

class ClockLambdaFunction {
    std::function<void()> fn_;
    t_clock* clock_;
    bool is_active_ = { false };

    ClockLambdaFunction(const ClockLambdaFunction&) = delete;
    ClockLambdaFunction(ClockLambdaFunction&&) = delete;
    ClockLambdaFunction& operator=(const ClockLambdaFunction&) = delete;

public:
    ClockLambdaFunction(std::function<void()> fn)
        : fn_(fn)
        , clock_(nullptr)
    {
        clock_ = clock_new(static_cast<void*>(this), reinterpret_cast<t_method>(tick));
    }

    /** unset clock if active */
    void unset()
    {
        if (clock_) {
            clock_unset(clock_);
            is_active_ = false;
        }
    }

    /** schedule clock execution in future */
    void delay(double ms)
    {
        if (clock_) {
            clock_delay(clock_, ms);
            is_active_ = true;
        }
    }

    ~ClockLambdaFunction()
    {
        if (clock_)
            clock_free(clock_);
    }

    /** check if clock is active */
    bool isActive() const { return is_active_; }

    /** get clock callback */
    const std::function<void()>& callback() const { return fn_; }

    /** set clock callback */
    void setCallback(const std::function<void()>& f) { fn_ = f; }

    /** execure callback */
    void exec()
    {
        fn_();
        is_active_ = false;
    }

    static void tick(ClockLambdaFunction* fn) { fn->exec(); }
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
