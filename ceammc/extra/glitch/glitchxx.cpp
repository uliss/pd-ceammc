/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "glitchxx.h"
#include "glitch.h"

size_t Glitch::samplerate = 0;

Glitch::Glitch()
    : glitch_(nullptr)
{
    glitch_ = glitch_create();

    if (samplerate == 0)
        setSamplerate(44100);
}

Glitch::~Glitch()
{
    if (glitch_)
        glitch_destroy(glitch_);
}

void Glitch::reset()
{
    if (glitch_)
        glitch_reset(glitch_);
}

bool Glitch::setExpr(const std::string& expr)
{
    if (!compile(expr))
        return false;

    expr_ = expr;
    return true;
}

void Glitch::setSamplerate(size_t sr)
{
    if (samplerate != sr) {
        glitch_init(sr, 0);
        samplerate = sr;
    }
}

bool Glitch::compile(const std::string& str)
{
    if (glitch_) {
        auto err = glitch_compile(glitch_, str.c_str(), str.length());
        return !err;
    } else
        return false;
}

bool Glitch::fill(float* buf, size_t frames, size_t channels) const
{
    if (!glitch_)
        return false;

    if (expr_.empty()) {
        for (size_t i = 0; i < frames; i++) {
            for (size_t j = 0; j < channels; j++)
                buf[i * channels + j] = 0;
        }

        return true;
    }

    glitch_fill(glitch_, buf, frames, channels);
    return true;
}
