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
#ifndef TL_TIMELINE_H
#define TL_TIMELINE_H

#include "ceammc_object.h"
#include "tl_cmd_driver.h"
#include "tl_timelines.h"

using namespace ceammc;

class TlTimeLine : public BaseObject, tl::TlIFace {
    tl::TimeLineSingle<TlTimeLine> tl_;
    tl::TlCmdDriver cmd_parser_;

public:
    TlTimeLine(const PdArgs& args);

    void dump() const override;

    void onFloat(t_float v) override;

    void event(size_t n, t_float at);
    void eventStart();
    void eventEnd();

    void m_add(t_symbol* s, const AtomListView& lv);
    void m_remove(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);

    void m_start(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);
    void m_pause(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_to_event(t_symbol* s, const AtomListView& lv);
    void m_to_time(t_symbol* s, const AtomListView& lv);

    AtomList propNumEvents() const;
    AtomList propCurrentTime() const;
    AtomList propPhase() const;
    AtomList propEvents() const;

    tl::RunState state() const;

    // IFACE
public:
    bool addAbsEvent(const std::string& name, double time) override;
    bool addRelEvent(const std::string& name, double time, const std::string& relName) override;
    bool removeEvent(const std::string& name) override;
    bool removeEventAtTime(double time_ms) override;
    bool removeEventAtPos(int pos) override;

    bool moveToEvent(long idx) override;
    bool moveToEvent(const std::string& name) override;
    bool moveToTime(double time_ms) override;

    void clearAll() override;
    void error(const std::string& msg) override;

    // TESTS
public:
    const tl::EventList& events() const;
};

void setup_tl_timeline();

#endif // TL_TIMELINE_H
