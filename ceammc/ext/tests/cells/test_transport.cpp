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
#include "ceammc_transport.h"
#include "test_common.h"

TEST_CASE("transport", "[ceammc::Transport]")
{
    SECTION("create")
    {
        Transport t;
        REQUIRE(t.bpm() == 60);
        REQUIRE(t.bar() == 0);
        REQUIRE(t.beat() == 0);
        REQUIRE(t.playState() == Transport::STATE_STOP);
    }

    SECTION("fsm")
    {
        Transport t(120);

        using T = Transport;

        T::PlayEvent vpe;
        T::BarEvent vbe;
        long vbar = -1;
        long vbeat = -1;
        float vbpm = 0;
        long i = 0;

        auto lp = [&vpe, &i](T::PlayEvent e) { vpe = e; i++; };
        auto lbpm = [&vbpm, &i](float bpm) { vbpm = bpm; i++; };
        auto lbar = [&vbe, &vbar, &i](T::BarEvent e, size_t bar) { vbe = e; vbar = bar; i++; };
        auto lbeat = [&vbeat, &i](size_t beat) { vbeat = beat; i++; };

        auto reset = [&vpe, &vbar, &i, &vbe, &vbeat]() { i = 0; vbar = -1; vpe = T::EVENT_SEEK; vbe = T::BAR_END; vbeat = -1; };

        t.subscribePlayback(0, lp);
        t.subscribeBar(0, lbar);
        t.subscribeBpm(0, lbpm);
        t.subscribeBeat(0, lbeat);

        REQUIRE(t.bpm() == 120);

        t.start();
        REQUIRE(t.playState() == T::STATE_PLAY);
        REQUIRE(vbar == 0);
        REQUIRE(vbpm == 120);
        REQUIRE(vbeat == 0);
        REQUIRE(vbe == T::BAR_BEGIN);
        REQUIRE(vpe == T::EVENT_START);
        REQUIRE(i == 4);

        t.start();
        REQUIRE(i == 4);
        t.resume();
        REQUIRE(i == 4);

        t.pause();
        REQUIRE(t.playState() == T::STATE_PAUSE);
        REQUIRE(vbar == 0);
        REQUIRE(vbpm == 120);
        REQUIRE(vbe == T::BAR_BEGIN);
        REQUIRE(vpe == T::EVENT_PAUSE);
        REQUIRE(i == 5);

        t.pause();
        REQUIRE(i == 5);

        t.stop();
        REQUIRE(t.playState() == T::STATE_STOP);
        REQUIRE(vbar == 0);
        REQUIRE(vbe == T::BAR_END);
        REQUIRE(vpe == T::EVENT_STOP);
        REQUIRE(i == 7);

        t.pause();
        REQUIRE(i == 7);
        t.stop();
        REQUIRE(i == 7);

        reset();
        t.resume();
        REQUIRE(t.playState() == T::STATE_PLAY);
        REQUIRE(vbar == -1);
        REQUIRE(vbeat == 0);
        REQUIRE(vpe == T::EVENT_RESUME);
        REQUIRE(i == 3);

        reset();
        t.pause();
        REQUIRE(t.playState() == T::STATE_PAUSE);
        REQUIRE(vbar == -1);
        REQUIRE(vpe == T::EVENT_PAUSE);
        REQUIRE(i == 1);
        REQUIRE(vbeat == -1);
    }

    SECTION("bars")
    {
        Transport t(120);

        using T = Transport;

        T::PlayEvent vpe;
        T::BarEvent vbe;
        long vbar;
        long i;

        auto lp = [&vpe, &i](T::PlayEvent e) { vpe = e; i++; };
        auto lbar = [&vbe, &vbar, &i](T::BarEvent e, size_t bar) { vbe = e; vbar = bar; i++; };

        auto reset = [&vpe, &vbar, &i, &vbe]() { i = 0; vbar = -1; vpe = T::EVENT_SEEK; vbe = T::BAR_END; };

        reset();
        t.subscribePlayback(0, lp);
        t.subscribeBar(0, lbar);

        t.start();
        REQUIRE(vbar == 0);

        reset();
        t.nextBar();
        REQUIRE(vbar == 1);
        REQUIRE(vbe == T::BAR_BEGIN);
        REQUIRE(i == 2);

        reset();
        t.nextBar();
        REQUIRE(vbar == 2);
        REQUIRE(vbe == T::BAR_BEGIN);
        REQUIRE(i == 2);

        reset();
        t.pause();
        REQUIRE(vpe == T::EVENT_PAUSE);
        REQUIRE(vbar == -1);
        REQUIRE(i == 1);

        reset();
        t.nextBar();
        REQUIRE(i == 0);

        reset();
        t.resume();
        REQUIRE(vpe == T::EVENT_RESUME);
        REQUIRE(vbar == -1);
        REQUIRE(i == 1);

        reset();
        t.nextBar();
        REQUIRE(vbar == 3);
        REQUIRE(i == 2);

        reset();
        t.nextBeat(0);
        REQUIRE(t.beat() == 1);

        reset();
        t.nextBar();
        REQUIRE(vbar == 4);
        REQUIRE(i == 2);
        REQUIRE(t.beat() == 0);

        i = 0;
        t.stop();
        REQUIRE(t.playState() == T::STATE_STOP);
        REQUIRE(vpe == T::EVENT_STOP);
        REQUIRE(vbe == T::BAR_END);
        REQUIRE(vbar == 4);
        REQUIRE(t.beat() == 0);
        REQUIRE(t.bar() == 0);
        REQUIRE(i == 2);

        t.unsubsribeAll(0);

        reset();
        t.start();
        REQUIRE(i == 0);
    }

    SECTION("storage")
    {
        auto& t0 = TransportStorage::instance().getOrCreate(&s_list);
        auto& t1 = TransportStorage::instance().getOrCreate(&s_list);

        REQUIRE(&t0 == &t1);
        REQUIRE(t0.bpm() == 60);
        REQUIRE(t1.bpm() == 60);

        t0.setBpm(120);
        REQUIRE(t1.bpm() == 120);
    }
}
