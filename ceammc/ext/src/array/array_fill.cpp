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

#define _USE_MATH_DEFINES
#include <cmath>

#include "array_fill.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <chrono>
#include <random>

ArrayFill::ArrayFill(const PdArgs& a)
    : ArrayMod(a)
{
    createOutlet();
}

void ArrayFill::onSymbol(t_symbol* s)
{
    setArray(s);
}

void ArrayFill::onFloat(float f)
{
    if (!checkArray())
        return;

    for (size_t i = 0; i < array_.size(); i++)
        array_[i] = f;

    finish();
}

void ArrayFill::onList(const AtomList& l)
{
    m_fill(gensym("fill"), l);
}

void ArrayFill::m_gauss(t_symbol* m, const AtomList& l)
{
    if (!checkArray())
        return;

    const t_float mean = l.floatAt(0, 0);
    const t_float stddev = l.floatAt(1, 1);

    if (stddev <= 0) {
        OBJ_ERR << "standart deviation (sigma) should be > 0";
        return;
    }

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::normal_distribution<t_float> dist(mean, stddev);

    std::generate(array_.begin(), array_.end(), [&gen, &dist]() {
        return dist(gen);
    });

    finish();
}

void ArrayFill::m_fill(t_symbol* m, const AtomList& l)
{
    if (!checkArray())
        return;

    size_t from = 0;
    size_t to = 0;
    AtomList values = parseRange(l, &from, &to);

    if (values.empty()) {
        METHOD_ERR(m) << "usage: [@from N] [@to N] VALUES...";
        return;
    }

    fillRange(from, to, values);
}

void ArrayFill::m_sin(t_symbol* m, const AtomList& l)
{
    if (!checkArray())
        return;

    if (l.empty()) {
        METHOD_ERR(m) << "usage: " << m << " PERIOD [AMPLITUDE] [PHASE]";
        return;
    }

    const t_float p = l.floatAt(0, 0);
    const t_float amp = l.floatAt(1, 1);
    const t_float phase = l.floatAt(2, 0) * M_PI;

    if (p <= 0) {
        METHOD_ERR(m) << "invalid period value: " << p;
        return;
    }

    const t_float w = 2 * M_PI / p;

    int i = 0;
    for (auto& v : array_)
        v = amp * std::sin(w * i++ + phase);

    finish();
}

void ArrayFill::fillRange(size_t from, size_t to, const AtomList& l)
{
    size_t step = l.size();
    for (size_t i = from; i < to; i++)
        array_[i] = l[i % step].asFloat();

    finish();
}

void ArrayFill::finish()
{
    if (shouldRedraw())
        array_.redraw();

    bangTo(0);
}

AtomList ArrayFill::parseRange(const AtomList& args, size_t* from, size_t* to) const
{
    AtomList res;

    Atom p_from;
    Atom p_to;

    size_t num_props = 0;

    if (args.hasProperty("@from")) {

        num_props++;

        if (args.property("@from", &p_from))
            num_props++;
    }

    if (args.hasProperty("@to")) {

        num_props++;

        if (args.property("@to", &p_to))
            num_props++;
    }

    int n_from = p_from.asInt(0);
    int n_to = p_to.asInt(array_.size());

    if (n_from < 0) {
        OBJ_ERR << "@from should be >= 0";
        return res;
    }

    if (n_from >= array_.size()) {
        OBJ_ERR << "@from should be < " << array_.size();
        return res;
    }

    if (n_to < 0) {
        n_to += array_.size();
        if (n_to < 0) {
            OBJ_ERR << "invalid @to range";
            return res;
        }
    }

    if (n_to > array_.size()) {
        OBJ_ERR << "invalid @to range: " << n_to;
        return res;
    }

    if (n_from >= n_to) {
        OBJ_ERR << "invalid range: " << n_from << '-' << n_to;
        return res;
    }

    res = args.slice(num_props);

    if (res.empty()) {
        OBJ_ERR << "fill values are required";
        return res;
    }

    if (from)
        *from = size_t(n_from);

    if (to)
        *to = size_t(n_to);

    return res;
}

extern "C" void setup_array0x2efill()
{
    ObjectFactory<ArrayFill> obj("array.fill");
    obj.addMethod("fill", &ArrayFill::m_fill);
    obj.addMethod("sin", &ArrayFill::m_sin);
    obj.addMethod("gauss", &ArrayFill::m_gauss);
}
