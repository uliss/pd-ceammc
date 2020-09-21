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
#ifndef ARRAY_GRAINER_H
#define ARRAY_GRAINER_H

#include "ceammc_array.h"
#include "ceammc_sound_external.h"
#include "grain.h"
using namespace ceammc;

#include <cstdint>
#include <iostream>
#include <random>

enum PlaybackType {
    ONESHOT_FORWARD,
    ONESHOT_BACKWARD,
    ONESHOT_PINGPONG,
    LOOP_FORWARD,
    LOOP_BACKWARD,
    LOOP_PINGPONG
};

class GrainPool {
    constexpr static size_t N = 2048;
    Grain grains_[N];
    std::bitset<N> map_;

    GrainPool()
    {
    }

    GrainPool(const GrainPool&) = delete;

public:
    static GrainPool& instance()
    {
        static GrainPool pool_;
        return pool_;
    }

    size_t capacity() const
    {
        return map_.size() - map_.count();
    }

    template <typename... Args>
    Grain* allocate(Args... args)
    {
        // optimise
        for (size_t i = 0; i < N; i++) {
            if (!map_[i]) {
                map_[i] = true;
                grains_[i] = Grain(args...);
                return &grains_[i];
            }
        }

        return nullptr;
    }

    void free(Grain* g)
    {
        ptrdiff_t offset = g - &grains_[0];
        if (offset < 0 || offset >= N)
            return;

        if (!map_.test(offset)) { // double free!!!!
            return;
        }

        map_[offset] = false;
    }
};

class GrainAction {
public:
    GrainAction(size_t when, Grain* g)
        : time_samp(when)
        , grain(g)
    {
    }

    size_t time_samp;
    Grain* grain;
};

class GrainCloud {
    std::vector<GrainAction> grains_;
    size_t first_grain_start = { 0 };
    size_t last_grain_end = { 0 };
    double play_pos_samp = { 0 };
    double play_speed = { 1 };
    std::vector<Grain*> grains_active_;
    ArrayIterator array_it_;
    size_t array_size_ = { 0 };
    size_t current_event_idx = { 0 };

public:
    GrainCloud()
    {
        grains_active_.reserve(100);
    }

    double speed() const { return play_speed; }
    /// cloud length in samples
    size_t length() const { return last_grain_end - first_grain_start; }
    /// cloud offset in samples
    size_t offset() const { return first_grain_start; }
    /// number of grains
    size_t size() const { return grains_.size(); }
    /// check if cloud is empty
    bool empty() const { return grains_.empty(); }
    /// list of grains
    const std::vector<GrainAction>& grains() const { return grains_; }
    /// clear all grains
    void clear()
    {
        grains_.clear();
        grains_active_.clear();
        first_grain_start = 0;
        last_grain_end = 0;
        play_pos_samp = 0;
    }
    /// set playback speed
    void setSpeed(double v) { play_speed = v; }

    void setArrayData(ArrayIterator data, size_t sz)
    {
        array_it_ = data;
        array_size_ = sz;
    }

    void addGrain(Grain* g)
    {
        assert(g);

        GrainAction ga_start = { g->startInSamples(), g };

        auto it0 = std::upper_bound(grains_.cbegin(), grains_.cend(), ga_start,
            [](const GrainAction& g0, const GrainAction& g1) {
                return g0.time_samp < g1.time_samp;
            });

        grains_.insert(it0, ga_start);

        // update cloud borders
        if (grains_.size() == 1) {
            first_grain_start = g->startInSamples();
            last_grain_end = g->endInSamples();
        } else {
            first_grain_start = std::min(first_grain_start, g->startInSamples());
            last_grain_end = std::max(last_grain_end, g->endInSamples());
        }
    }

    void movePlayPhase(size_t time_samp)
    {
        const double new_play_pos = play_pos_samp + play_speed * time_samp;
        if (new_play_pos >= last_grain_end) {
            play_pos_samp = 0;
            current_event_idx = 0;
            // unfinished grains
            for(auto g: grains_active_)
                g->done();

            grains_active_.clear();
        } else {
            play_pos_samp = new_play_pos;
        }
    }

    void playGrains(t_sample** buf, size_t bs)
    {
        std::fill(&buf[0][0], &buf[0][0] + bs, 0);
        std::fill(&buf[1][0], &buf[1][0] + bs, 0);

        if (grains_active_.size() > 0) {
            for (auto& g : grains_active_) {
                auto st = g->process(array_it_, array_size_, buf, bs);
                if (st == Grain::FINISHED) {
                    //                    LIB_DBG << "finished: " << *g;
                    g = nullptr; // mark to delete with nullptr
                }
            }

            // remove finished
            auto it = std::remove(grains_active_.begin(), grains_active_.end(), nullptr);
            grains_active_.erase(it, grains_active_.end());
        }

        movePlayPhase(bs);
    }

    void playBuffer(t_sample** buf, size_t bs)
    {
        // no grains - zero output
        if (grains_.empty()) {
            std::fill(&buf[0][0], &buf[0][0] + bs, 0);
            std::fill(&buf[1][0], &buf[1][0] + bs, 0);
            return;
        }

        if (current_event_idx >= grains_.size())
            return playGrains(buf, bs);

        // fast forward if needed
        while (grains_[current_event_idx].time_samp < (size_t)play_pos_samp) {
            current_event_idx++;
            if (current_event_idx >= grains_.size()) { // no more events
                return playGrains(buf, bs);
            }
        }

        assert(current_event_idx < grains_.size());
        auto* ga = &grains_[current_event_idx];
        assert(play_pos_samp <= ga->time_samp);
        const auto block_end_samp = play_pos_samp + bs * play_speed;

        // no events yet
        if (block_end_samp <= ga->time_samp)
            return playGrains(buf, bs);

        // have events in current block
        assert(play_pos_samp <= ga->time_samp);
        assert(ga->time_samp < block_end_samp);

        while (ga->time_samp < block_end_samp) {
            // start new grain
            ga->grain->start(play_pos_samp);
            grains_active_.push_back(ga->grain);
            //            LIB_DBG << "start: " << *ga->grain;

            // try next
            current_event_idx++;

            // no more events
            if (current_event_idx >= grains_.size())
                break;

            assert(current_event_idx < grains_.size());
            ga = &grains_[current_event_idx];
        }

        // play grains
        playGrains(buf, bs);
    }
};

class ArrayGrainer : public SoundExternal {
    SymbolProperty* array_name_;
    FloatProperty* speed_;
    Array array_;
    GrainCloud cloud_;
    std::mt19937 random_;

public:
    ArrayGrainer(const PdArgs& args);

    void setupDSP(t_signal** sp) override;
    void processBlock(const t_sample** in, t_sample** out) override;

    void dump() const override;

    void m_grain(t_symbol* s, const AtomListView& lv);
    void m_fill(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);

    const GrainCloud& cloud() const { return cloud_; }

private:
    void updateGrains();
};

void setup_array_grainer();

#endif // ARRAY_GRAINER_H
