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
#ifndef GLITCHXX_H
#define GLITCHXX_H

#include <string>

struct glitch;

class Glitch {
    glitch* glitch_;
    std::string expr_;

private:
    static size_t samplerate;

public:
    Glitch();
    ~Glitch();

    Glitch(const Glitch&) = delete;
    Glitch& operator=(const Glitch&) = delete;

    void reset();
    bool setExpr(const std::string& expr);
    void setSamplerate(size_t sr);
    bool fill(float* buf, size_t frames, size_t channels) const;

private:
    bool compile(const std::string& str);
};

#endif // GLITCHXX_H
