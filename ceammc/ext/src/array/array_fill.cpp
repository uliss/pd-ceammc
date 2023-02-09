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
#include "array_fill.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <chrono>
#include <cmath>
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

void ArrayFill::onFloat(t_float f)
{
    if (!checkArray())
        return;

    for (auto& i : array_)
        i = f;

    finish();
}

void ArrayFill::onList(const AtomListView& lv)
{
    m_fill(gensym("fill"), lv);
}

void ArrayFill::m_gauss(t_symbol* m, const AtomListView& lv)
{
    if (!checkArray())
        return;

    const t_float mean = lv.floatAt(0, 0);
    const t_float stddev = lv.floatAt(1, 1);

    if (stddev <= 0) {
        METHOD_ERR(m) << "standart deviation (sigma) should be > 0";
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

void ArrayFill::m_uniform(t_symbol* m, const AtomListView& lv)
{
    if (!checkArray())
        return;

    const t_float a = lv.floatAt(0, 0);
    const t_float b = lv.floatAt(1, 1);

    if (!(a < b)) {
        METHOD_ERR(m) << "a should be less then b";
        return;
    }

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_real_distribution<t_float> dist(a, b);

    std::generate(array_.begin(), array_.end(), [&gen, &dist]() {
        return dist(gen);
    });

    finish();
}

void ArrayFill::m_fill(t_symbol* m, const AtomListView& l)
{
    if (!checkArray())
        return;

    size_t from = 0;
    size_t to = 0;
    const AtomListView values = parseRange(l, &from, &to);

    if (values.empty()) {
        METHOD_ERR(m) << "usage: [@from N] [@to N] VALUES...";
        return;
    } else if (values.size() == 1)
        fillRange(from, to, values[0].asFloat());
    else
        fillRange(from, to, values);
}

void ArrayFill::m_sin(t_symbol* m, const AtomListView& l)
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

void ArrayFill::m_pulse(t_symbol* m, const AtomListView& l)
{
    if (!checkArray())
        return;

    if (l.empty()) {
        METHOD_ERR(m) << "usage: " << m << " PERIOD [AMPLITUDE] [PHASE]";
        return;
    }

    const t_float period = l.floatAt(0, 0);
    const t_float amp = l.floatAt(1, 1);
    const t_float duty = clip<t_float>(l.floatAt(2, 0.5), t_float(0.001), t_float(0.999));

    if (period <= 1) {
        METHOD_ERR(m) << "invalid period value: " << period;
        return;
    }

    int i = 0;
    for (auto& v : array_) {
        t_float n = (i++ / period);
        auto frac = std::fmod(n, 1);
        v = (frac < duty) ? amp : -amp;
    }

    finish();
}

void ArrayFill::m_saw(t_symbol* m, const AtomListView& l)
{
    if (!checkArray())
        return;

    if (l.empty()) {
        METHOD_ERR(m) << "usage: " << m << " PERIOD [AMPLITUDE] [PHASE]";
        return;
    }

    const t_float period = l.floatAt(0, 0);
    const t_float amp = l.floatAt(1, 1);

    if (period <= 1) {
        METHOD_ERR(m) << "invalid period value: " << period;
        return;
    }

    int i = 0;
    for (auto& v : array_) {
        t_float n = (i++ / period);
        auto frac = std::fmod(n, 1);
        v = convert::lin2lin<t_sample>(frac, 0, 1, -amp, amp);
    }

    finish();
}

void ArrayFill::m_tri(t_symbol* m, const AtomListView& l)
{
    if (!checkArray())
        return;

    if (l.empty()) {
        METHOD_ERR(m) << "usage: " << m << " PERIOD [AMPLITUDE] [PHASE]";
        return;
    }

    const t_float period = l.floatAt(0, 0);
    const t_float amp = l.floatAt(1, 1);

    if (period <= 1) {
        METHOD_ERR(m) << "invalid period value: " << period;
        return;
    }

    int i = 0;
    for (auto& v : array_) {
        t_float n = (i++ / period);
        auto frac = std::fmod(n + 0.25, 1);
        auto v1 = convert::lin2lin<t_sample>(frac, 0, 1, -2 * amp, 2 * amp);
        auto v2 = -v1;
        v = std::min(v1, v2) + amp;
    }

    finish();
}

void ArrayFill::fillRange(size_t from, size_t to, const AtomListView& lv)
{
    size_t step = lv.size();
    for (size_t i = from; i < to; i++)
        array_[i] = lv[i % step].asFloat();

    finish();
}

void ArrayFill::fillRange(size_t from, size_t to, t_float v)
{
    std::fill(array_.begin() + from, array_.begin() + to, v);
    finish();
}

void ArrayFill::finish()
{
    if (shouldRedraw())
        array_.redraw();

    bangTo(0);
}

AtomListView ArrayFill::parseRange(const AtomListView& args, size_t* from, size_t* to) const
{
    static t_symbol* PROP_FROM = gensym("@from");
    static t_symbol* PROP_TO = gensym("@to");

    AtomListView res;

    Atom p_from;
    Atom p_to;

    size_t num_props = 0;

    auto it = std::find(args.begin(), args.end(), PROP_FROM);
    auto end = args.end();

    if (it != end) {

        num_props++;
        ++it;

        if (it != end) {
            p_from = *it;
            num_props++;
        }
    }

    it = std::find(args.begin(), args.end(), PROP_TO);
    if (it != end) {

        num_props++;
        ++it;

        if (it != end) {
            p_to = *it;
            num_props++;
        }
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

    res = args.subView(num_props);

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

void setup_array_fill()
{
    ObjectFactory<ArrayFill> obj("array.fill");
    obj.addMethod("fill", &ArrayFill::m_fill);
    obj.addMethod("sin", &ArrayFill::m_sin);
    obj.addMethod("gauss", &ArrayFill::m_gauss);
    obj.addMethod("uniform", &ArrayFill::m_uniform);
    obj.addMethod("pulse", &ArrayFill::m_pulse);
    obj.addMethod("saw", &ArrayFill::m_saw);
    obj.addMethod("tri", &ArrayFill::m_tri);

    obj.setDescription("fill array with single value or pattern");
    obj.setCategory("array");
    obj.setKeywords({"array", "fill"});
}
