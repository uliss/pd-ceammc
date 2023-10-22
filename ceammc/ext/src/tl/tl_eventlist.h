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
#ifndef TL_EVENTLIST_H
#define TL_EVENTLIST_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "m_pd.h"

namespace ceammc {
namespace tl {

    enum EventTimeMode : std::int8_t {
        EVENT_ABSOLUTE = 0,
        EVENT_RELATIVE
    };

    enum EventType : std::int8_t {
        EVENT_TYPE_OUT_MSG = 0,
        EVENT_TYPE_OUT_MOD_START,
        EVENT_TYPE_OUT_MOD_STOP = 0,
        EVENT_TYPE_SEND_MSG,
        EVENT_TYPE_SEND_MOD_START,
        EVENT_TYPE_SEND_MOD_STOP,
        EVENT_TYPE_PRESET_SET,
        EVENT_TYPE_PRESET_INTERP_START,
        EVENT_TYPE_PRESET_INTERP_STOP,
    };

    struct EventData {
        t_symbol* out_target;
        t_symbol* send_target;
    };

    struct Event {
        double abs_time;
        double next_time;
        double rel_time;
        t_symbol* name;
        t_symbol* rel_name;
        std::unique_ptr<EventData> data;
        int num_refs;
        EventTimeMode mode;
        EventType type;

        Event(double absTime, t_symbol* name = &s_);
        Event(const Event& ev);
        Event(Event&&);
        Event& operator=(Event&& ev);
        Event& operator=(const Event& ev);

        bool operator<(const Event& e0) const
        {
            return abs_time < e0.abs_time;
        }

        static Event relEvent(t_symbol* name, double time, t_symbol* target);
    };

    class EventList {
        typedef std::vector<Event> EventVector;
        EventVector vec_;

    public:
        typedef EventVector::value_type value_type;
        typedef EventVector::reference reference;
        typedef EventVector::const_reference const_reference;
        typedef EventVector::iterator iterator;
        typedef EventVector::const_iterator const_iterator;
        typedef EventVector::pointer pointer;
        typedef EventVector::const_pointer const_pointer;

    public:
        EventList();

        iterator begin() { return vec_.begin(); }
        const_iterator begin() const { return vec_.begin(); }
        iterator end() { return vec_.end(); }
        const_iterator end() const { return vec_.end(); }

        reference operator[](size_t n) { return vec_[n]; }
        const_reference operator[](size_t n) const { return vec_[n]; }
        pointer at(long relPos);
        const_pointer at(long relPos) const;

        reference back() { return vec_.back(); }
        const_reference back() const { return vec_.back(); }
        const_reference front() const { return vec_.front(); }

        bool empty() const { return vec_.empty(); }
        size_t size() const { return vec_.size(); }

        void clear(double time);
        bool removeAtPos(long pos);

        bool exists(const Event& ev) const;
        long addAbsEvent(const Event& ev);
        long addRelEvent(const Event& ev);
        void calcNextEvents();
        void calcRelEvents();

        iterator findByName(t_symbol* name);
        iterator findByRelName(t_symbol* name);

        const_iterator findByName(t_symbol* name) const;
        const_iterator findByRelName(t_symbol* name) const;

        long findPosByName(t_symbol* name) const;
        long findPosByAbsTime(double time_ms) const;
    };

    std::ostream& operator<<(std::ostream& os, const Event& ev);
    std::ostream& operator<<(std::ostream& os, const EventList& evlist);
}
}

#endif // TL_EVENTLIST_H
