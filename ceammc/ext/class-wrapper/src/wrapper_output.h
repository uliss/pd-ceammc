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
#ifndef WRAPPER_OUTPUT_H
#define WRAPPER_OUTPUT_H

#include <string>
#include <tuple>
#include <vector>

#include "ceammc_data.h"
#include "ceammc_log.h"

#include "wrapper_tuple.h"

namespace wrapper {

/**
 * @brief Dumps values to Pd window
 */
class PdDump {
    ceammc::LogBaseObject& os_;

public:
    PdDump(ceammc::LogBaseObject& stream)
        : os_(stream)
    {
    }

    void operator()(int, const bool& b)
    {
        os_ << ' ' << b;
    }

    void operator()(int, const float& f)
    {
        os_ << ' ' << f;
    }

    void operator()(int, const double& f)
    {
        os_ << ' ' << f;
    }

    void operator()(int, const int& i)
    {
        os_ << ' ' << i;
    }

    void operator()(int, const long& i)
    {
        os_ << ' ' << i;
    }

    void operator()(int, const unsigned int& i)
    {
        os_ << ' ' << i;
    }

    void operator()(int, const unsigned long& i)
    {
        os_ << ' ' << i;
    }

    void operator()(int, const std::vector<int>& v)
    {
        for (auto& i : v)
            os_ << ' ' << i;
    }

    void operator()(int, const std::vector<float>& v)
    {
        for (auto& i : v)
            os_ << ' ' << i;
    }

    void operator()(int, const std::vector<double>& v)
    {
        for (auto& i : v)
            os_ << ' ' << i;
    }

    void operator()(int, const std::vector<std::string>& v)
    {
        for (auto& i : v)
            os_ << ' ' << i;
    }

    void operator()(int, const std::string& str)
    {
        os_ << ' ' << str;
    }

    void operator()(int, const AtomList& lst)
    {
        os_ << ' ' << lst;
    }

    void operator()(int, const Atom& a)
    {
        os_ << ' ' << a;
    }

    void operator()(int, t_symbol* s)
    {
        os_ << ' ' << s;
    }

    template <typename U>
    void operator()(int, const U& u)
    {
        os_ << ' ' << u.toString();
    }
};

template <typename T, typename V>
struct PdOutput {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const V& v)
    {
        t_->atomTo(n, DataAtom<AbstractDataWrapper<V>>(v));
    }
};

template <typename T>
struct PdOutput<T, bool> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const bool& v)
    {
        t_->floatTo(n, v ? 1 : 0);
    }
};

template <typename T>
struct PdOutput<T, float> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const float& v)
    {
        t_->floatTo(n, v);
    }
};

template <typename T>
struct PdOutput<T, double> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const double& v)
    {
        t_->floatTo(n, v);
    }
};

template <typename T>
struct PdOutput<T, int> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const int& v)
    {
        t_->floatTo(n, v);
    }
};

template <typename T>
struct PdOutput<T, unsigned int> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const unsigned int& v)
    {
        t_->floatTo(n, v);
    }
};

template <typename T>
struct PdOutput<T, long> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const long& v)
    {
        t_->floatTo(n, v);
    }
};

template <typename T>
struct PdOutput<T, unsigned long> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const unsigned long& v)
    {
        t_->floatTo(n, v);
    }
};

template <typename T>
struct PdOutput<T, std::string> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const std::string& v)
    {
        t_->symbolTo(n, gensym(v.c_str()));
    }
};

template <typename T>
struct PdOutput<T, std::vector<float>> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const std::vector<float>& v)
    {
        AtomList res;
        res.reserve(v.size());

        for (auto f : v)
            res.append(Atom(f));

        t_->listTo(n, res);
    }
};

template <typename T>
struct PdOutput<T, std::vector<double>> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const std::vector<double>& v)
    {
        AtomList res;
        res.reserve(v.size());

        for (auto f : v)
            res.append(Atom(f));

        t_->listTo(n, res);
    }
};

template <typename T>
struct PdOutput<T, std::vector<int>> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const std::vector<int>& v)
    {
        AtomList res;
        res.reserve(v.size());

        for (auto f : v)
            res.append(Atom(f));

        t_->listTo(n, res);
    }
};

template <typename T>
struct PdOutput<T, AtomList> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const AtomList& l)
    {
        t_->listTo(n, l);
    }
};

template <typename T>
struct PdOutput<T, Atom> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const Atom& a)
    {
        t_->atomTo(n, a);
    }
};

template <typename T>
struct PdOutput<T, t_symbol*> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, t_symbol* s)
    {
        t_->symbolTo(n, s);
    }
};

template <typename T>
struct PdOutput<T, std::vector<std::string>> {
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const std::vector<std::string>& v)
    {
        AtomList res;
        res.reserve(v.size());

        for (auto s : v)
            res.append(Atom(gensym(s.c_str())));

        t_->listTo(n, res);
    }
};

template <typename T, typename... Args>
struct PdOutput<T, std::tuple<Args...>> {
private:
    class OutputCallback {
        T* t_;
        size_t n_;

    public:
        OutputCallback(T* t, size_t n)
            : t_(t)
            , n_(n)
        {
        }

        void operator()(int index, bool v)
        {
            t_->floatTo(n_ + index, v ? 1 : 0);
        }

        void operator()(int index, float f)
        {
            t_->floatTo(n_ + index, f);
        }

        void operator()(int index, double f)
        {
            t_->floatTo(n_ + index, f);
        }

        void operator()(int index, int i)
        {
            t_->floatTo(n_ + index, i);
        }

        void operator()(int index, unsigned int i)
        {
            t_->floatTo(n_ + index, i);
        }

        void operator()(int index, long i)
        {
            t_->floatTo(n_ + index, i);
        }

        void operator()(int index, t_symbol* s)
        {
            t_->symbolTo(n_ + index, s);
        }

        void operator()(int index, const char* s)
        {
            t_->symbolTo(n_ + index, gensym(s));
        }

        void operator()(int index, const std::string& s)
        {
            t_->symbolTo(n_ + index, gensym(s.c_str()));
        }

        void operator()(int index, const std::vector<float>& v)
        {
            AtomList res;
            for (auto& f : v)
                res.append(f);

            t_->listTo(n_ + index, res);
        }

        void operator()(int index, const std::vector<double>& v)
        {
            AtomList res;
            for (auto& f : v)
                res.append(f);

            t_->listTo(n_ + index, res);
        }

        void operator()(int index, const std::vector<int>& v)
        {
            AtomList res;
            for (auto& i : v)
                res.append(i);

            t_->listTo(n_ + index, res);
        }

        void operator()(int index, const AtomList& l)
        {
            t_->listTo(n_ + index, l);
        }

        void operator()(int index, const Atom& a)
        {
            t_->atomTo(n_ + index, a);
        }
    };

public:
    T* t_;
    PdOutput(T* t)
        : t_(t)
    {
    }

    void output(size_t n, const std::tuple<Args...>& v)
    {
        tuple_utils::rfor_each(v, OutputCallback(t_, n));
    }
};

}

#endif // WRAPPER_OUTPUT_H
