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

#include <algorithm>

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
            if (g->playStatus() == GRAIN_FINISHED)
                g->start(0);

            g->process(array_it_, array_size_, buf, bs, sr);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const GrainCloud& cloud)
{
    return os;
}

}
