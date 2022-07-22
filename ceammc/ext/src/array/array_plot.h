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
#ifndef ARRAY_PLOT_H
#define ARRAY_PLOT_H

#include "ceammc_array.h"
#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class ArrayPlot : public BaseObject {
    t_symbol* array_name_;
    Array array_;
    FloatProperty *ymin_, *ymax_;
    BoolProperty* yauto_;
    FloatProperty* nan_;
    size_t phase_;
    size_t total_;
    t_float min_, max_;
    bool running_;

public:
    ArrayPlot(const PdArgs& a);

    void onFloat(t_float sample) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& l) override;

    bool setArray(t_symbol* s);
    bool checkArray(bool log);

private:
    void updateScale();
    void done();
    void reset();
    bool resizeArray(size_t n);
    t_float processSample(t_float sample, t_float ymin, t_float max, bool auto_range);
    void plotSample(t_float sample);
};

void setup_array_plot();

#endif // ARRAY_PLOT_H
