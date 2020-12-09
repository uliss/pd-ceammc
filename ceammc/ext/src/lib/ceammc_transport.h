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
#ifndef CEAMMC_TRANSPORT_H
#define CEAMMC_TRANSPORT_H

#include "ceammc_object.h"

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <vector>

namespace ceammc {

class Transport {
public:
    enum PlayEvent {
        EVENT_START,
        EVENT_STOP,
        EVENT_PAUSE,
        EVENT_RESUME,
        EVENT_SEEK
    };

    enum PlayState { // note: keep in sync with fsm_
        STATE_PLAY,
        STATE_STOP,
        STATE_PAUSE,
        STATE_TOTAL
    };

    enum BarEvent {
        BAR_BEGIN,
        BAR_END
    };

    using NotifyPlaybackFn = std::function<void(PlayEvent)>;
    using NotifyBarFn = std::function<void(BarEvent, size_t)>;
    using NotifyBpmFn = std::function<void(float)>;
    using NotifyBeatFn = std::function<void(size_t)>;

    using PlaybackSubsciption = std::pair<BaseObject*, NotifyPlaybackFn>;
    using BarSubsciption = std::pair<BaseObject*, NotifyBarFn>;
    using BpmSubsciption = std::pair<BaseObject*, NotifyBpmFn>;
    using BeatSubsciption = std::pair<BaseObject*, NotifyBeatFn>;

private:
    using TransitionFn = std::function<void(PlayEvent)>;
    using PlayStateTransition = std::pair<PlayState, TransitionFn>;
    using PlayFSM = std::array<PlayStateTransition, STATE_TOTAL>;

    std::vector<PlaybackSubsciption> playbacks_;
    std::vector<BarSubsciption> bars_;
    std::vector<BpmSubsciption> bpms_;
    std::vector<BeatSubsciption> beats_;
    PlayFSM play_fsm_;

    PlayState play_state_ = { STATE_STOP };
    size_t bar_ = { 0 };
    size_t beat_ = { 0 };
    float bpm_ = { 60 };

    Transport(const Transport&) = delete;
    Transport& operator=(const Transport&) = delete;

public:
    Transport(float bpm = 60);

    void subscribePlayback(BaseObject* x, NotifyPlaybackFn fn);
    void subscribeBar(BaseObject* x, NotifyBarFn fn);
    void subscribeBpm(BaseObject* x, NotifyBpmFn fn);
    void subscribeBeat(BaseObject* x, NotifyBeatFn fn);

    void unsubsribeAll(BaseObject* x);
    void unsubsribePlayback(BaseObject* x);
    void unsubsribeBar(BaseObject* x);
    void unsubsribeBpm(BaseObject* x);
    void unsubsribeBeat(BaseObject* x);

    void notifyPlayback(PlayEvent ev);
    void notifyBar(BarEvent ev, size_t bar);
    void notifyBeat(size_t beat);
    void notifyBpm(float bpm);

    float bpm() const { return bpm_; }
    bool setBpm(t_float bpm);
    size_t bar() const { return bar_; }
    size_t beat() const { return beat_; }
    PlayState playState() const { return play_state_; }

    void start();
    void stop();
    void pause();
    void resume();

    void nextBar();
    void nextBeat(size_t beatsInBar = 0);

private:
    void doStart();
    void doStop();
    void doPause();
    void doResume();
};

class TransportStorage {
    using TMap = std::map<t_symbol*, Transport>;
    TMap tmap_;

    TransportStorage();

public:
    static TransportStorage& instance();

    Transport& getOrCreate(t_symbol* name);
};

}

#endif // CEAMMC_TRANSPORT_H
