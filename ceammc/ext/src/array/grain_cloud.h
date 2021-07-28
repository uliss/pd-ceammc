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

    void removeFinished();
    void removeById(int id);
    void removeByTag(t_symbol* tag);

    void alignGrain(Grain* g, const std::vector<size_t>& onsets);
    void alignAll(const std::vector<size_t>& onsets);
    void alignById(int id, const std::vector<size_t>& onsets);
    void alignByTag(t_symbol* tag, const std::vector<size_t>& onsets);
    void alignFinished(const std::vector<size_t>& onsets);

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

    void playBuffer(t_sample** buf, uint32_t bs, uint32_t sr);
};

}

#endif // GRAIN_CLOUD_H
