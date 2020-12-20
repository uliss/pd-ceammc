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
#ifndef RND_GEN_H
#define RND_GEN_H

#include <memory>
#include <random>

namespace ceammc {

using RandomGenT = std::mt19937;

class RandomGen {
    std::shared_ptr<RandomGenT> gen_;

public:
    RandomGen();
    void setSeed(uint_fast32_t v);
    RandomGenT& get() { return *gen_; }
};
}

#endif // RND_GEN_H
