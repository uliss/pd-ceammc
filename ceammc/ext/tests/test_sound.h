#ifndef SOUND_EXTERNAL_TEST_H
#define SOUND_EXTERNAL_TEST_H

#include "test_base.h"

#include <algorithm>
#include <initializer_list>
#include <iosfwd>

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
    static const size_t BS = 64;

    t_sample buf_in[IN][BS];
    t_sample buf_out[OUT][BS];
    const t_sample* in_ptr[IN];
    t_sample* out_ptr[OUT];
    const t_sample** in;
    t_sample** out;

    TestSignal(t_sample in = 0, t_sample out = 0)
        : in(in_ptr)
        , out(out_ptr)
    {
        for (size_t i = 0; i < IN; i++)
            in_ptr[i] = buf_in[i];

        for (size_t i = 0; i < OUT; i++)
            out_ptr[i] = buf_out[i];

        fillInput(in);
        fillOutput(out);
    }

    void fillInput(float v)
    {
        for (size_t c = 0; c < IN; c++)
            for (size_t s = 0; s < BS; s++)
                buf_in[c][s] = v;
    }

    void fillInput(double v)
    {
        for (size_t c = 0; c < IN; c++)
            for (size_t s = 0; s < BS; s++)
                buf_in[c][s] = v;
    }

    void fillOutput(t_sample v)
    {
        for (size_t c = 0; c < OUT; c++)
            for (size_t s = 0; s < BS; s++)
                buf_out[c][s] = v;
    }

    std::ostream& dumpOutput(std::ostream& os, size_t idx)
    {
        if (idx >= OUT) {
            os << "invalid output: " << idx;
        } else {
            for (size_t i = 0; i < BS; i++)
                os << buf_out[idx][i] << ' ';
        }

        return os;
    }

    void fillInput(int v)
    {
        fillInput(t_sample(v));
    }

    void setInput(size_t idx, t_sample v)
    {
        if (idx >= BS)
            return;

        buf_in[0][idx] = v;
    }

    void setInput(size_t input, std::initializer_list<t_sample> values)
    {
        if (input >= IN)
            return;

        for (size_t i = 0; i < std::min(blocksize(), values.size()); i++)
            buf_in[input][i] = *(values.begin() + i);
    }

    t_sample* lastOut(size_t output)
    {
        if (output >= OUT)
            return nullptr;

        return buf_out[output] + blocksize() - 1;
    }

    t_sample* lastIn(size_t input)
    {
        if (input >= IN)
            return nullptr;

        return buf_in[input] + blocksize() - 1;
    }

    bool outputStartsWith(size_t output, std::initializer_list<t_sample> values)
    {
        if (output >= OUT)
            return false;

        for (size_t i = 0; i < std::min(blocksize(), values.size()); i++) {
            if (buf_out[output][i] != *(values.begin() + i))
                return false;
        }

        return true;
    }

    bool isOutputZero(bool output)
    {
        if (output >= OUT)
            return false;

        return std::all_of(buf_out[output], buf_out[output] + blocksize(), [](t_sample s) { return s == 0; });
    }

    template <class B, class E>
    void fillInputSeq(B begin, E end)
    {
        auto it = begin;
        if (it == end)
            return;

        for (size_t c = 0; c < IN; c++) {
            for (size_t s = 0; s < BS; s++) {
                buf_in[c][s] = *it;

                if (it != end)
                    ++it;
            }
        }
    }

    template <class Gen>
    void fillInput(Gen fn)
    {
        for (size_t c = 0; c < IN; c++)
            for (size_t s = 0; s < BS; s++)
                buf_in[c][s] = fn(s);
    }

    template <class Gen>
    void fillInputN(size_t n, Gen fn)
    {
        if (n >= IN)
            return;

        for (size_t s = 0; s < BS; s++)
            buf_in[n][s] = fn(s);
    }

    void fillInputN(size_t n, int v)
    {
        fillInputN(n, t_float(v));
    }

    void fillInputN(size_t n, float v)
    {
        if (n >= IN)
            return;

        for (size_t s = 0; s < BS; s++)
            buf_in[n][s] = v;
    }

    void fillInputN(size_t n, double v)
    {
        if (n >= IN)
            return;

        for (size_t s = 0; s < BS; s++)
            buf_in[n][s] = v;
    }

    t_sample* beginOut(size_t i) { return &buf_out[i][0]; }
    t_sample* endOut(size_t i) { return &buf_out[i][BS]; }

    size_t blocksize() const { return BS; }
};

template <class T, class E>
class DSP {
    T& sig_;
    E& ext_;

public:
    const size_t NI;
    const size_t NO;
    const size_t SR;
    const size_t BS;

private:
    std::vector<t_signal*> arr_;

public:
    DSP(T& sig, E& ext)
        : sig_(sig)
        , ext_(ext)
        , NI(ext.numInputChannels())
        , NO(ext.numOutputChannels())
        , SR(ext.samplerate())
        , BS(ext.blockSize())
    {
        canvas_resume_dsp(1);

        for (size_t i = 0; i < NI; i++) {
            t_signal* a = new t_signal;
            a->s_vec = (t_sample*)sig_.in[i];
            a->s_n = BS;
            a->s_sr = SR;
            arr_.push_back(a);
        }

        for (size_t i = 0; i < NO; i++) {
            t_signal* a = new t_signal;
            a->s_vec = (t_sample*)sig_.out[i];
            a->s_n = BS;
            a->s_sr = SR;
            arr_.push_back(a);
        }

        ext_.setupDSP(arr_.data());
    }

    ~DSP()
    {
        for (size_t i = 0; i < arr_.size(); i++)
            delete arr_[i];

        arr_.clear();
    }

    void processBlock(size_t n = 1)
    {
        for (size_t i = 0; i < n; i++)
            ext_.processBlock(sig_.in, sig_.out);
    }

    t_sample out(size_t ch, size_t sample) const
    {
        return sig_.buf_out[ch][sample];
    }
};

#endif // SOUND_EXTERNAL_TEST_H
