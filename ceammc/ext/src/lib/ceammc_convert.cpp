/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "ceammc_convert.h"

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

std::string ceammc::convert::time::sec2str(float sec, bool ms)
{
    float isec = 0;
    int fraq = abs(static_cast<int>(modff(sec, &isec) * 1000));
    int psec = abs(static_cast<int>(sec));

    const int BUFSIZE = 32;
    char buf[BUFSIZE];
    char ms_buf[16];

    char* buf_ptr = buf;
    if (sec < 0) {
        buf_ptr[0] = '-';
        buf_ptr++;
    }

    int h = (psec % (3600 * 24)) / 3600;
    int m = (psec % 3600) / 60;
    int s = psec % 60;
    if (ms) {
        snprintf(ms_buf, sizeof(ms_buf)-1, ".%03d", fraq);
    }

    snprintf(buf_ptr, BUFSIZE - 2, "%02d:%02d:%02d%s", h, m, s, ms ? ms_buf : "");
    return buf;
}

class TimeFSM {
    std::vector<int> stack_;
    int digits_;
    int sign_;
    bool has_ms_;

public:
    enum state_t {
        INIT = 0,
        DIGIT,
        DOT,
        SEMICOLON,
        MINUS
    };

    TimeFSM()
        : digits_(2)
        , sign_(1)
        , has_ms_(false)
        , current_state_(INIT)
    {
        stack_.push_back(0);
    }

    void start_num(int digits)
    {
        digits_ = digits;
    }

    void end_num()
    {
        stack_.push_back(0);
    }

    bool add_digit(int ch)
    {
        if (digits_ < 1)
            return false;

        stack_.back() = stack_.back() * 10 + (ch - '0');
        digits_--;
        return true;
    }

    bool read(int ch)
    {
        if (stack_.size() > (has_ms_ ? 4 : 3))
            return false;

        state_t next_state;
        if (::isdigit(ch))
            next_state = DIGIT;
        else if (ch == '.')
            next_state = DOT;
        else if (ch == ':')
            next_state = SEMICOLON;
        else if (ch == '-')
            next_state = MINUS;
        else
            return false;

        if (table_[current_state_][next_state] == 0)
            return false;

        switch (next_state) {
        case DIGIT:
            if (!add_digit(ch))
                return false;
            break;
        case DOT:
            end_num();
            start_num(3);
            has_ms_ = true;
            break;
        case SEMICOLON:
            end_num();
            start_num(2);
            break;
        case MINUS:
            sign_ = -1;
            break;
        case INIT:
        default:
            break;
        }

        current_state_ = next_state;
        return true;
    }

    float get_time() const
    {
        int h = 0, m = 0, s = 0, ms = 0;
        long idx = stack_.size();
        if (has_ms_ && --idx >= 0)
            ms = stack_[idx];

        if (--idx >= 0)
            s = stack_[idx];

        if (--idx >= 0)
            m = stack_[idx];

        if (--idx >= 0)
            h = stack_[idx];

        return sign_ * (h * 3600 + m * 60 + s + ms / 1000.f);
    }

private:
    static char table_[5][5];
    state_t current_state_;
};

char TimeFSM::table_[5][5] = {
    //        I  D  .  :  -
    /* I */ { 0, 1, 0, 0, 1 },
    /* D */ { 0, 1, 1, 1, 0 },
    /* . */ { 0, 1, 0, 0, 0 },
    /* : */ { 0, 1, 0, 0, 0 },
    /* - */ { 0, 1, 0, 0, 0 }
};

float ceammc::convert::time::str2sec(const std::string& str, float def)
{
    TimeFSM fsm;
    for (size_t i = 0; i < str.size(); i++) {
        if (!fsm.read(str[i]))
            return def;
    }

    return fsm.get_time();
}

float ceammc::convert::lin2exp(float x, float x0, float x1, float y0, float y1)
{
    return float(lin2exp(double(x), double(x0), double(x1), double(y0), double(y1)));
}

double ceammc::convert::lin2exp(double x, double x0, double x1, double y0, double y1)
{
    return y0 * pow(y1 / y0, (x - x0) / (x1 - x0));
}

float ceammc::convert::exp2lin(float x, float x0, float x1, float y0, float y1)
{
    return float(exp2lin(double(x), double(x0), double(x1), double(y0), double(y1)));
}

double ceammc::convert::exp2lin(double x, double x0, double x1, double y0, double y1)
{
    return y0 + (y1 - y0) * log(x / x0) / log(x1 / x0);
}

float ceammc::convert::lin2curve(float x, float x0, float x1, float y0, float y1, float curve)
{
    return float(lin2curve(double(x), double(x0), double(x1), double(y0), double(y1), double(curve)));
}

double ceammc::convert::lin2curve(double x, double x0, double x1, double y0, double y1, double curve)
{
    // code from SuperCollider
    if (fabs(curve) < 0.001) {
        // If the value should be clipped, it has already been clipped (above).
        // If we got this far, then linlin does not need to do any clipping.
        // Inlining the formula here makes it even faster.
        return (x - x0) / (x1 - x0) * (y1 - y0) + y0;
    };

    const double grow = exp(curve);
    const double a = (y1 - y0) / (1.0 - grow);
    const double b = y0 + a;
    const double scaled = (x - x0) / (x1 - x0);

    return b - (a * pow(grow, scaled));
}

double ceammc::convert::lin2sin2(double x, double x0, double x1, double y0, double y1)
{
    double v = sin(M_PI_2 * (x - x0) / (x1 - x0));
    return (v * v) * (y1 - y0) + y0;
}

double ceammc::convert::lin2sigmoid(double x, double x0, double x1, double y0, double y1, double skew)
{
    double v = 1 / (1 + pow(M_E, (-skew) * ((x - x0) / (x1 - x0) - 0.5)));
    return v * (y1 - y0) + y0;
}

float ceammc::convert::dbfs2amp(float db)
{
    return powf(10.f, db / 20.f);
}

double ceammc::convert::dbfs2amp(double db)
{
    return pow(10.0, db / 20.0);
}

float ceammc::convert::amp2dbfs(float amp)
{
    return 20 * log10f(amp);
}

double ceammc::convert::amp2dbfs(double amp)
{
    return 20 * log10(amp);
}

float ceammc::convert::midi2freq(float note, float a_freq)
{
    return a_freq * powf(2, (note - 69) / 12);
}

double ceammc::convert::midi2freq(double note, double a_freq)
{
    return a_freq * pow(2, (note - 69) / 12);
}

float ceammc::convert::freq2midi(float freq, float a_freq)
{
    if (freq <= 0 || a_freq <= 0)
        return -1;

    return (12 * log2f(freq / a_freq)) + 69;
}

double ceammc::convert::freq2midi(double freq, double a_freq)
{
    if (freq <= 0 || a_freq <= 0)
        return -1;

    return (12 * log2(freq / a_freq)) + 69;
}
