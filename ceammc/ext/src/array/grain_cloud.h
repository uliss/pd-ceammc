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
#ifndef GRAIN_CLOUD_H
#define GRAIN_CLOUD_H

#include <boost/pool/object_pool.hpp>

#include <cstdint>
#include <functional>
#include <iostream>
#include <vector>

#include "ceammc_array.h"
#include "grain.h"

namespace ceammc {

class GrainCloud {
public:
    enum SyncMode {
        SYNC_NONE,
        SYNC_INTERNAL,
        SYNC_EXTERNAL,
    };

private:
    std::vector<Grain*> grains_;
    ArrayIterator array_it_;
    size_t array_size_ = { 0 };

    using Pool = boost::object_pool<Grain>;
    Pool pool_;

    SyncMode sync_ = { SYNC_NONE };
    uint32_t sync_counter_ = { 0 };
    float sync_interval_ = { 50 };
    bool sync_ext_ = { false };
    float sync_prob_ = { 1 };

public:
    GrainCloud(size_t chunk_size = 32);

    /// number of grains
    size_t size() const { return grains_.size(); }
    /// check if cloud is empty
    bool empty() const { return grains_.empty(); }
    /// list of grains
    const std::vector<Grain*>& grains() const { return grains_; }
    /// clear all grains
    void clear();

    SyncMode syncMode() const { return sync_; }
    void setSyncMode(SyncMode m) { sync_ = m; }

    float syncInterval() const { return sync_interval_; }
    void setSyncInterval(float ms) { sync_interval_ = ms; }

    float syncProbability() const { return sync_prob_; }
    void setSyncProbability(float p) { sync_prob_ = p; }

    void externalSyncTick() { sync_ext_ = true; }

    void removeFinished();
    void removeById(int id);
    void removeByTag(t_symbol* tag);

    void pauseAll(bool value);
    void pauseById(int id, bool value);
    void pauseByTag(t_symbol* tag, bool value);

    void alignGrain(Grain* g, const std::vector<size_t>& onsets);
    void alignAll(const std::vector<size_t>& onsets);
    void alignById(int id, const std::vector<size_t>& onsets);
    void alignByTag(t_symbol* tag, const std::vector<size_t>& onsets);
    void alignFinished(const std::vector<size_t>& onsets);

    /**
     * uniformly spread grains
     */
    bool spread(uint32_t len_samp, t_symbol* tag = &s_);

    bool shuffle(t_symbol* tag = &s_);

    bool reverse(t_symbol* tag = &s_);

    bool permutate(int n, t_symbol* tag = &s_);

    void setArrayData(ArrayIterator data, size_t sz)
    {
        array_it_ = data;
        array_size_ = sz;
    }

    template <typename... Args>
    Grain* appendGrain(Args... args)
    {
        auto res = pool_.construct(args...);
        if (res)
            grains_.push_back(res);

        return res;
    }

    void popGrain();

    /**
     * play active grains and add their values to given stereo buffer
     * @param buf - output buffer
     * @param bs - buffer size
     * @param sr - samplerate
     * @return number of done grains while playing this buffer
     */
    int playBuffer(t_sample** buf, uint32_t bs, uint32_t sr);
};

}

#endif // GRAIN_CLOUD_H
