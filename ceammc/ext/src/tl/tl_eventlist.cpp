/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "tl_eventlist.h"
#include "ceammc_log.h"

#include <algorithm>

using namespace ceammc;
using namespace ceammc::tl;

#define TL_ERR ceammc::LIB_ERR << "[tl] "
#define TL_DBG ceammc::LIB_DBG << "[tl] "

Event::Event(Event&& ev)
    : abs_time(ev.abs_time)
    , next_time(ev.next_time)
    , rel_time(ev.rel_time)
    , name(ev.name)
    , rel_name(ev.rel_name)
    , data(std::move(ev.data))
    , num_refs(ev.num_refs)
    , mode(ev.mode)
    , type(ev.type)
{
}

Event& Event::operator=(Event&& ev)
{
    if (&ev == this)
        return *this;

    abs_time = ev.abs_time;
    next_time = ev.next_time;
    rel_time = ev.rel_time;
    name = ev.name;
    rel_name = ev.rel_name;
    data = std::move(ev.data);
    num_refs = ev.num_refs;
    mode = ev.mode;
    type = ev.type;
    return *this;
}

Event& Event::operator=(const Event& ev)
{
    if (&ev == this)
        return *this;

    abs_time = ev.abs_time;
    next_time = ev.next_time;
    rel_time = ev.rel_time;
    name = ev.name;
    rel_name = ev.rel_name;
    num_refs = ev.num_refs;
    mode = ev.mode;
    type = ev.type;

    if (ev.data)
        data.reset(new EventData(*ev.data));
    else
        data.reset();

    return *this;
}

Event::Event(double absTime, t_symbol* name)
    : abs_time(absTime)
    , next_time(absTime)
    , rel_time(0)
    , name(name)
    , rel_name(&s_)
    , num_refs(0)
    , mode(EVENT_ABSOLUTE)
{
}

Event::Event(const Event& ev)
    : abs_time(ev.abs_time)
    , next_time(ev.next_time)
    , rel_time(ev.rel_time)
    , name(ev.name)
    , rel_name(ev.rel_name)
    , num_refs(ev.num_refs)
    , mode(ev.mode)
    , type(ev.type)
{
    if (ev.data)
        data.reset(new EventData(*ev.data));
}

Event Event::relEvent(t_symbol* name, double time, t_symbol* target)
{
    Event res(0, name);
    res.mode = EVENT_RELATIVE;
    res.rel_time = time;
    res.rel_name = target;
    return res;
}

EventList::EventList()
{
}

EventList::pointer EventList::at(long relPos)
{
    const int N = vec_.size();
    if (relPos < -N || relPos >= N)
        return nullptr;

    auto pos = (relPos + N) % N;
    return &vec_[pos];
}

EventList::const_pointer EventList::at(long relPos) const
{
    const int N = vec_.size();
    if (relPos < -N || relPos >= N)
        return nullptr;

    auto pos = (relPos + N) % N;
    return &vec_[pos];
}

void EventList::clear(double time)
{
    static const size_t DEF_SIZE = 32;
    static t_symbol* SYM_END = gensym("end");

    vec_.clear();

    addAbsEvent(Event(time, SYM_END));
    vec_.reserve(DEF_SIZE);
}

bool EventList::removeAtPos(long pos)
{
    // protection for last item (do not delete)
    const int N = vec_.size() - 1;
    if (pos < -N || pos >= N)
        return false;

    pos = (pos + N) % N;

    Event& ev = vec_[pos];

    // remove references
    if (ev.num_refs > 0) {
        int nrefs = ev.num_refs;
        t_symbol* name = ev.name;

        while (nrefs-- > 0) {
            auto it = findByRelName(name);

            // not found, should never happend
            if (it == vec_.end())
                break;

            vec_.erase(it);
        }

        // update positions because some element could be remove while
        // removing references
        pos = findPosByName(name);
        if (pos < 0) {
            TL_ERR << "removeAtPos bug";
            return false;
        }
    }

    // fix ref count
    if (ev.mode == EVENT_RELATIVE) {
        auto it = findByName(ev.rel_name);
        if (it != vec_.end())
            it->num_refs--;
    }

    vec_.erase(vec_.begin() + pos);

    calcNextEvents();

    return true;
}

bool EventList::exists(const Event& ev) const
{
    return std::binary_search(vec_.begin(), vec_.end(), ev,
        [](const Event& e0, const Event& e1) { return e0.abs_time < e1.abs_time; });
}

long EventList::addAbsEvent(const Event& ev)
{
    auto it = std::lower_bound(vec_.begin(), vec_.end(), ev);
    auto pos = std::distance(vec_.begin(), vec_.insert(it, ev));

    calcNextEvents();
    return pos;
}

long EventList::addRelEvent(const Event& ev)
{
    if (ev.name == ev.rel_name) {
        std::cerr << "same source and target names: " << ev.name;
        return -1;
    }

    // find target event
    auto it = findByName(ev.rel_name);

    if (it == vec_.end()) {
        std::cerr << "target event is not found: " << ev.rel_name;
        return -1;
    }

    if (it->mode != EVENT_ABSOLUTE) {
        std::cerr << "only asbolute events allowed as target";
        return -1;
    }

    // calc and check absolute time
    double event_abs_time = it->abs_time + ev.rel_time;
    if (event_abs_time < 0 || event_abs_time >= vec_.back().abs_time) {
        std::cerr << "invalid relative offset: "
                  << ev.rel_time << "ms " << ((ev.rel_time < 0) ? "before " : "after ") << ev.rel_name;
        return -1;
    }

    // insert new event
    Event rel_event(ev);
    rel_event.abs_time = event_abs_time;
    it->num_refs++;
    return addAbsEvent(rel_event);
}

void EventList::calcNextEvents()
{
    const auto N = vec_.size();
    if (N == 0)
        return;

    vec_[0].next_time = vec_[0].abs_time;
    for (size_t i = 1; i < N; i++)
        vec_[i].next_time = vec_[i].abs_time - vec_[i - 1].abs_time;
}

void EventList::calcRelEvents()
{
    // calc rel times
    for (auto& ev : vec_) {
        if (ev.mode == EVENT_RELATIVE) {
            t_symbol* owner_name = ev.rel_name;

            // find owner
            auto it = findByName(owner_name);

            if (it == vec_.end()) {
                std::cerr << "orphan event found: " << ev.name << " at " << ev.abs_time;
                break;
            }

            if (it->mode != EVENT_ABSOLUTE) {
                std::cerr << "not absolute event as owner for: " << ev.name << " at " << ev.abs_time;
                break;
            }

            ev.abs_time = it->abs_time + ev.rel_time;
        }
    }
}

EventList::iterator EventList::findByName(t_symbol* name)
{
    return std::find_if(vec_.begin(), vec_.end(),
        [name](const Event& ev) { return ev.name == name; });
}

EventList::iterator EventList::findByRelName(t_symbol* name)
{
    return std::find_if(vec_.begin(), vec_.end(),
        [name](const Event& ev) { return ev.rel_name == name; });
}

long EventList::findPosByAbsTime(double time_ms) const
{
    auto it = std::lower_bound(vec_.begin(), vec_.end(), Event(time_ms));
    if (it == vec_.end() || it->abs_time != time_ms)
        return -1;

    return std::distance(vec_.begin(), it);
}

EventList::const_iterator EventList::findByName(t_symbol* name) const
{
    return std::find_if(vec_.begin(), vec_.end(),
        [name](const Event& ev) { return ev.name == name; });
}

EventList::const_iterator EventList::findByRelName(t_symbol* name) const
{
    return std::find_if(vec_.begin(), vec_.end(),
        [name](const Event& ev) { return ev.rel_name == name; });
}

long EventList::findPosByName(t_symbol* name) const
{
    auto it = findByName(name);
    if (it == vec_.end())
        return -1;

    return std::distance(vec_.begin(), it);
}

std::ostream& ceammc::tl::operator<<(std::ostream& os, const Event& ev)
{
    os << "Event at: " << ev.abs_time << "\n"
       << "    next: " << ev.next_time << "\n"
       << "     rel: " << ev.rel_time << "\n"
       << "    name: " << ev.name << "\n"
       << " relname: " << ev.rel_name << "\n"
       << "    mode: " << ev.mode << "\n"
       << "   nrefs: " << ev.num_refs << "\n";

    return os;
}

std::ostream& ceammc::tl::operator<<(std::ostream& os, const EventList& evlist)
{
    for (auto& ev : evlist)
        os << ev << "\n";

    return os;
}
