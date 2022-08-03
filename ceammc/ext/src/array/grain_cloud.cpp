/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "grain_cloud.h"
#include "ceammc_convert.h"
#include "fmt/core.h"
#include "grain_random.h"

#include <algorithm>
#include <boost/container/small_vector.hpp>

namespace ceammc {

GrainCloud::GrainCloud(size_t n)
    : pool_(n)
{
}

void GrainCloud::clear()
{
    for (auto* g : grains_)
        pool_.destroy(g);

    grains_.clear();
}

void GrainCloud::removeFinished()
{
    auto from = std::remove_if(grains_.begin(), grains_.end(),
        [this](Grain* g) {
            if (g->playStatus() == GRAIN_FINISHED) {
                pool_.destroy(g);
                return true;
            } else
                return false;
        });

    grains_.erase(from, grains_.end());
}

void GrainCloud::removeById(int id)
{
    auto from = std::remove_if(grains_.begin(), grains_.end(),
        [this, id](Grain* g) {
            if (g->id() == id) {
                pool_.destroy(g);
                return true;
            } else
                return false;
        });

    grains_.erase(from, grains_.cend());
}

void GrainCloud::removeByTag(t_symbol* tag)
{
    auto from = std::remove_if(grains_.begin(), grains_.end(),
        [this, tag](Grain* g) {
            if (g->tag() == tag) {
                pool_.destroy(g);
                return true;
            } else
                return false;
        });

    grains_.erase(from, grains_.end());
}

void GrainCloud::pauseAll(bool value)
{
    const auto st = value ? GRAIN_PAUSE : GRAIN_PLAYING;
    for (auto* g : grains_)
        g->setPlayStatus(st);
}

void GrainCloud::pauseById(int id, bool value)
{
    const auto st = value ? GRAIN_PAUSE : GRAIN_PLAYING;

    for (auto* g : grains_) {
        if (g->id() == id) {
            g->setPlayStatus(st);
            break;
        }
    }
}

void GrainCloud::pauseByTag(t_symbol* tag, bool value)
{
    const auto st = value ? GRAIN_PAUSE : GRAIN_PLAYING;

    for (auto* g : grains_) {
        if (g->tag() == tag)
            g->setPlayStatus(st);
    }
}

void GrainCloud::alignAll(const std::vector<size_t>& onsets)
{
    for (auto* g : grains_)
        alignGrain(g, onsets);
}

void GrainCloud::alignById(int id, const std::vector<size_t>& onsets)
{
    for (auto* g : grains_) {
        if (g->id() == id) {
            alignGrain(g, onsets);
            break;
        }
    }
}

void GrainCloud::alignByTag(t_symbol* tag, const std::vector<size_t>& onsets)
{
    for (auto* g : grains_) {
        if (g->tag() == tag)
            alignGrain(g, onsets);
    }
}

void GrainCloud::alignFinished(const std::vector<size_t>& onsets)
{
    for (auto* g : grains_) {
        if (g->playStatus() == GRAIN_FINISHED)
            alignGrain(g, onsets);
    }
}

bool GrainCloud::spreadGrains(SpreadMode mode, uint32_t len_samp, t_symbol* tag, bool sync)
{
    using GrainIdxVec = boost::container::small_vector<std::uint8_t, 64>;

    // count target grains
    const auto N = (!tag || tag == &s_)
        ? grains_.size()
        : std::count_if(grains_.begin(), grains_.end(), [tag](const Grain* g) { return g->tag() == tag; });

    switch (mode) {
    case SPREAD_EQUAL: {
        for (size_t i = 0; i < N; i++) {
            auto g = grains_[i];

            if (tag == &s_ || tag == g->tag()) {
                const double glen = g->lengthInSamples();
                const double tlen = len_samp;
                const double gpos = (i / double(N)) * tlen;

                g->resetFirstTime();
                g->start(0);
                g->setTimeBefore(gpos);
                g->setTimeAfter(clip_min<double, 0>(tlen - glen));
            }
        }
    } break;
    case SPREAD_SHUFFLE: {
        GrainIdxVec gidxvec;
        gidxvec.reserve(N);
        for (size_t i = 0; i < N; i++)
            gidxvec.push_back(i);

        for (size_t i = 0; i < N; i++) {
            auto g = grains_[i];

            if (tag == &s_ || tag == g->tag()) {
                const double glen = g->lengthInSamples();
                const double tlen = len_samp;
                std::shuffle(gidxvec.begin(), gidxvec.end(), GrainRandom::instance().gen());
                double gk = gidxvec[i] / double(N);
                double gpos = gk * tlen;
                g->resetFirstTime();
                g->start(0);
                g->setTimeBefore(gpos);
                g->setTimeAfter(clip_min<double, 0>(tlen - glen));
            }
        }
    } break;
    default:
        return false;
    }

    return true;
}

void GrainCloud::popGrain()
{
    if (grains_.empty())
        return;

    auto g = grains_.back();
    pool_.destroy(g);
    grains_.pop_back();
}

void GrainCloud::playBuffer(t_sample** buf, uint32_t bs, uint32_t sr)
{
    std::fill(&buf[0][0], &buf[0][0] + bs, 0);
    std::fill(&buf[1][0], &buf[1][0] + bs, 0);

    if (grains_.empty())
        return;

    for (auto* g : grains_) {
        if (!g)
            continue;

        if (g->canBePlayed()) {
            if (g->playStatus() == GRAIN_FINISHED) { // grain retriggering
                switch (sync_) {
                case SYNC_INTERNAL:
                    sync_counter_ += bs;

                    if (sync_counter_ > (sr * 0.001 * sync_interval_)) {
                        sync_counter_ = 0;

                        bool start = true;
                        if (sync_prob_ < 1)
                            start = GrainRandom::instance().urandf(0, 1) < sync_prob_;

                        if (start) {
                            g->start(0);
                            g->process(array_it_, array_size_, buf, bs, sr);
                        }
                    }

                    break;
                case SYNC_EXTERNAL:
                    if (sync_ext_) {
                        sync_ext_ = false;

                        bool start = true;
                        if (sync_prob_ < 1)
                            start = GrainRandom::instance().urandf(0, 1) < sync_prob_;

                        if (start) {
                            g->start(0);
                            g->process(array_it_, array_size_, buf, bs, sr);
                        }
                    }
                    break;
                case SYNC_NONE:
                default:
                    g->start(0);

                    uint32_t done_samp = 0;
                    uint32_t offset = 0;
                    auto state = g->process(array_it_, array_size_, buf, bs, sr, offset, &done_samp);

                    while (state == GRAIN_FINISHED && g->canBePlayed() && done_samp > 0 && offset < bs) {
                        offset += done_samp;
                        done_samp = 0;
                        g->start(0);
                        state = g->process(array_it_, array_size_, buf, bs, sr, offset, &done_samp);
                    }
                    break;
                }
            } else { // process without retriggering
                switch (sync_) {
                case SYNC_INTERNAL:
                case SYNC_EXTERNAL:
                    // simple process
                    g->process(array_it_, array_size_, buf, bs, sr);
                    break;
                case SYNC_NONE:
                default:
                    uint32_t done_samp = 0;
                    uint32_t offset = 0;
                    auto state = g->process(array_it_, array_size_, buf, bs, sr, offset, &done_samp);

                    while (state == GRAIN_FINISHED && g->canBePlayed() && done_samp > 0 && offset < bs) {
                        offset += done_samp;
                        done_samp = 0;
                        g->start(0);
                        state = g->process(array_it_, array_size_, buf, bs, sr, offset, &done_samp);
                    }
                    break;
                }
            }
        }
    }
}

void GrainCloud::alignGrain(Grain* g, const std::vector<size_t>& onsets)
{
    const long pos = g->arrayPosInSamples();
    auto next = std::upper_bound(onsets.begin(), onsets.end(), pos);

    if (next == onsets.end()) { // position after last onset
        g->setArrayPosInSamples(onsets.back());
    } else if (next == onsets.begin()) { // position before first onset
        g->setArrayPosInSamples(onsets.front());
    } else {
        const size_t next_onset = *next;
        const size_t prev_onset = *(next - 1);
        const size_t d0 = pos - prev_onset;
        const size_t d1 = next_onset - pos;

        if (d0 < d1)
            g->setArrayPosInSamples(prev_onset);
        else
            g->setArrayPosInSamples(next_onset);
    }
}

}
