/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef STOK_SOLO_DATA_H
#define STOK_SOLO_DATA_H

#include "ceammc_log.h"
#include "fmt/core.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

namespace solo {

constexpr int NO_PERFORATION = -1;
using namespace ceammc;

struct Event {
    enum Type : std::int8_t {
        EVENT_OFF,
        EVENT_ON,
        EVENT_CRESC,
        EVENT_DIM
    };
    float rel_pos { 0 }, rel_length { 0 }, from { 0 }, to { 0 };
    Type type { EVENT_OFF };

    Event(Type t, float rpos = 0)
        : type(t)
        , rel_pos(rpos)
    {
    }
};

struct Period {
    std::vector<Event> events;

    Period& addOff()
    {
        events.emplace_back(Event::EVENT_OFF);
        return *this;
    }

    Period& addOn(int perforate = 0, float pos = 0, float length = 0)
    {
        events.emplace_back(Event::EVENT_ON, pos);
        events.back().rel_length = length;

        std::vector<int> rnd;
        std::uniform_int_distribution<int> dist(4, 1024);
        float sum = 0;
        std::default_random_engine gen(0);
        for (int i = 0; i < perforate; i++) {
            auto x = dist(gen);
            rnd.push_back(x);
            sum += x;
        }

        for (auto v : rnd) {
            auto pos = float(v) / sum;
            events.emplace_back(Event::EVENT_OFF, pos);
            events.emplace_back(Event::EVENT_ON, std::max<float>(1, pos + 0.125));
        }

        return *this;
    }

    Period& addCresc(float pos, float length, float from = 0, float to = 1)
    {
        events.emplace_back(Event::EVENT_CRESC, pos);
        events.back().rel_length = length;
        events.back().from = from;
        events.back().to = to;
        return *this;
    }

    std::string toString() const
    {
        std::string res;
        res.reserve(events.size());

        if (events.size() == 1) {
            if (events.front().type == Event::EVENT_OFF)
                res += ' ';
            else if (events.front().type == Event::EVENT_ON)
                res += '+';
        } else {
            auto n = std::count_if(events.begin(), events.end(), [](const Event& e) { return e.type == Event::EVENT_ON; });
            res += '0' + ((n > 0) ? n - 1 : 0);
        }

        return res;
    }
};

using PeriodList = std::vector<Period>;

struct Cycle {
    PeriodList mic1, mic2, fb1, fb2, out1, out2;
    float abs_period_length; // seconds

    void set(size_t n, float periodLengthSec)
    {
        mic1.assign(n, {});
        mic2.assign(n, {});
        fb1.assign(n, {});
        fb2.assign(n, {});
        out1.assign(n, {});
        out2.assign(n, {});
        abs_period_length = periodLengthSec;
    }

    void clear()
    {
        mic1.clear();
        mic2.clear();
        fb1.clear();
        fb2.clear();
        out1.clear();
        out2.clear();
    }

    static void addPeriodSimpleEvents(PeriodList& periods, std::initializer_list<int> init)
    {
        if (init.size() != periods.size())
            LIB_DBG << fmt::format("warning: period number mismatch: {} != {}", periods.size(), init.size());

        const auto N = std::min<size_t>(init.size(), periods.size());
        for (size_t i = 0; i < N; i++) {
            auto& p = periods[i];
            auto x = *(init.begin() + i);
            if (x == 0)
                p.addOff();
            else
                p.addOn(x == NO_PERFORATION ? 0 : x); // negative no perf
        }
    }

    void addSimpleEvents(
        std::initializer_list<int> m1, std::initializer_list<int> m2,
        std::initializer_list<int> f1, std::initializer_list<int> f2,
        std::initializer_list<int> o1, std::initializer_list<int> o2)
    {
        addPeriodSimpleEvents(mic1, m1);
        addPeriodSimpleEvents(mic2, m2);
        addPeriodSimpleEvents(fb1, f1);
        addPeriodSimpleEvents(fb2, f2);
        addPeriodSimpleEvents(out1, o1);
        addPeriodSimpleEvents(out2, o2);
    }

    float length() const
    {
        return periodCount() * abs_period_length;
    }

    size_t periodCount() const
    {
        return mic1.size();
    }

    std::string toString(size_t n) const
    {
        std::string res;
        switch (n) {
        case 0:
            for (auto& x : mic1)
                res += x.toString();
            break;
        case 1:
            for (auto& x : mic2)
                res += x.toString();
        case 2:
            for (auto& x : fb1)
                res += x.toString();
        case 3:
            for (auto& x : fb2)
                res += x.toString();
        case 4:
            for (auto& x : out1)
                res += x.toString();
        case 5:
            for (auto& x : out2)
                res += x.toString();
        default:
            break;
        }

        res += '|';

        return res;
    }
};

struct Scheme {
    Cycle a;
    Cycle b;
    Cycle c;
    Cycle d;
    Cycle e;
    Cycle f;
    const int E = NO_PERFORATION;

    Scheme() { }

    Scheme(int i)
    {
        set(i);
    }

    bool set(int i)
    {
        switch (i) {
        case 1:
            setScheme1();
            break;
        case 2:
            setScheme2();
            break;
        default:
            return false;
        }

        return true;
    }

    void setScheme1()
    {
        a.set(11, 6);
        b.set(8, 14.2);
        c.set(7, 19);
        d.set(6, 25.3);
        e.set(9, 10.6);
        f.set(10, 8);
    }

    void setScheme2()
    {
        a.set(9, 12);
        b.set(7, 24);
        c.set(11, 6);
        d.set(10, 8.5);
        e.set(6, 30.4);
        f.set(8, 17.1);

        a.addSimpleEvents(
            { 0, 3, 2, 0, 2, 3, 0, 2, 0 },
            { 2, 0, 0, 3, 0, 0, 2, 0, 2 },
            { 0, 0, 2, 0, 0, 3, 0, 0, 2 },
            { 0, 2, 0, 0, 2, 0, 0, 2, 1 },
            { 0, 0, E, E, 0, E, E, 0, E },
            { 0, E, E, 0, E, E, E, E, E } //
        );
        b.addSimpleEvents(
            { 4, 1, 3, 1, 2, 0, 1 },
            { 1, 4, 1, 2, 1, 3, 0 },
            { 0, 2, 1, 1, 1, 1, E },
            { 0, 1, 1, 1, 1, 1, E },
            { 0, E, 0, E, 0, E, 0 },
            { 0, 0, E, 0, E, 0, E } //
        );
        c.addSimpleEvents(
            { 2, 0, 1, 0, 1, 1, 0, 2, 0, E, 0 },
            { 0, 1, 0, 2, 0, 0, 1, 0, 2, 0, E },
            { 0, 1, 1, 1, 1, 1, 0, 0, 1, E, E },
            { 0, 0, 1, 1, 1, 1, 1, 0, 0, E, E },
            { 0, E, E, E, E, E, E, 0, E, E, E },
            { 0, 0, E, E, E, E, 0, E, 0, E, E } //
        );
        d.addSimpleEvents(
            { 0, 2, 1, 0, 1, 1, 0, 2, 1, 0 },
            { 2, 0, 0, 1, 0, 0, 0, 0, 0, E },
            { 0, 0, 1, E, 1, 2, 0, 0, 1, E },
            { 0, 1, E, 1, E, 1, 1, E, 1, E },
            { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1 },
            { 0, 1, 1, 0, 1, 0, 1, 1, 0, 1 } //
        );
        e.addSimpleEvents(
            { 3, 0, 0, 0, 0, 8 },
            { 0, 5, E, E, 3, 0 },
            { 0, 1, 2, 2, 1, 1 },
            { 0, 0, E, E, 3, E },
            { 0, 1, 1, 1, 1, 1 },
            { 0, 0, 1, 0, 0, 1 } //
        );
        f.addSimpleEvents(
            { 0, 0, 3, 0, 0, 0, 6, 0 },
            { 4, 0, 0, 0, 5, 0, 0, 0 },
            { 1, 1, 0, 2, 0, 0, 0, 0 },
            { 0, 2, 0, 0, 0, 3, 0, 0 },
            { 0, 0, 0, 1, 1, 0, 0, 1 },
            { 0, 1, 1, 0, 0, 1, 1, 0 } //
        );
    }

    float length() const
    {
        return a.length() + b.length() + c.length() + d.length() + e.length() + f.length();
    }

    std::string toString(size_t part) const
    {
        return '|'
            + a.toString(part)
            + b.toString(part)
            + c.toString(part)
            + d.toString(part)
            + e.toString(part)
            + f.toString(part);
    }
};

}

#endif // STOK_SOLO_DATA_H
