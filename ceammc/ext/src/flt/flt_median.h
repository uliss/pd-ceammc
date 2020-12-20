/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef FLT_MEDIAN_H
#define FLT_MEDIAN_H

#include "ceammc_object.h"
#include <functional>
#include <queue>

using namespace ceammc;

class CircularBuffer {
    std::vector<t_float> vec_;
    size_t idx_;

public:
    CircularBuffer(size_t n);
    void push(t_float v);
    t_float back() const;
};

class FltMedian : public BaseObject {
    IntProperty* window_size_;
    std::vector<t_float> window_;
    std::vector<t_float> pool_;
    long window_idx_;

public:
    FltMedian(const PdArgs& args);

    void onFloat(t_float v) override;

public:
    void updateSize();
};

void setup_flt_median();

#endif // FLT_MEDIAN_H
