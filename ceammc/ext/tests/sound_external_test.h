#ifndef SOUND_EXTERNAL_TEST_H
#define SOUND_EXTERNAL_TEST_H

#include "base_extension_test.h"

struct ConstGen {
    double v;
    ConstGen(double val)
        : v(val)
    {
    }
    t_sample operator()(size_t) const { return t_sample(v); }
};

struct FRandGen {
    int f, t;
    FRandGen(int from, int to)
        : f(from)
        , t(to)
    {
    }
    t_sample operator()(size_t) const { return t_sample(rand() % (t - f) + f); }
};

template <size_t IN, size_t OUT>
struct TestSignal {
    t_sample buf_in[IN][64];
    t_sample buf_out[OUT][64];
    const t_sample* in_ptr[IN];
    t_sample* out_ptr[OUT];
    const t_sample** in;
    t_sample** out;

    TestSignal()
        : in(in_ptr)
        , out(out_ptr)
    {
        for (size_t i = 0; i < IN; i++)
            in_ptr[i] = buf_in[i];

        for (size_t i = 0; i < OUT; i++)
            out_ptr[i] = buf_out[i];
    }

    void fillInput(float v)
    {
        for (size_t c = 0; c < IN; c++)
            for (size_t s = 0; s < 64; s++)
                buf_in[c][s] = v;
    }

    void fillOutput(float v)
    {
        for (size_t c = 0; c < OUT; c++)
            for (size_t s = 0; s < 64; s++)
                buf_out[c][s] = v;
    }

    void fillInput(int v)
    {
        fillInput(float(v));
    }

    template <class Gen>
    void fillInput(Gen fn)
    {
        for (size_t c = 0; c < IN; c++)
            for (size_t s = 0; s < 64; s++)
                buf_in[c][s] = fn(s);
    }

    template <class Gen>
    void fillInputN(size_t n, Gen fn)
    {
        if (n >= IN)
            return;

        for (size_t s = 0; s < 64; s++)
            buf_in[n][s] = fn(s);
    }

    void fillInputN(size_t n, int v)
    {
        fillInputN(n, float(v));
    }

    void fillInputN(size_t n, float v)
    {
        if (n >= IN)
            return;

        for (size_t s = 0; s < 64; s++)
            buf_in[n][s] = v;
    }
};

#endif // SOUND_EXTERNAL_TEST_H
