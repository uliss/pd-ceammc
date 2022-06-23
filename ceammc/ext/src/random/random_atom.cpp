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
#include "random_atom.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "fmt/format.h"

#include <cassert>
#include <limits>

RandomAtom::RandomAtom(const PdArgs& args)
    : BaseObject(args)
    , atoms_(nullptr)
    , seed_(nullptr)
    , wsum_(0)
    , last_idx_(std::numeric_limits<size_t>::max())
{
    atoms_ = new ListProperty("@a");
    atoms_->setArgIndex(0);
    addProperty(atoms_);

    createCbListProperty(
        "@w",
        [this]() -> AtomList {
            AtomList res;
            for (auto& w : weights_)
                res.append(w);

            return res;
        },
        [this](const AtomList& l) -> bool {
            if (l.size() > atoms_->value().size()) {
                OBJ_ERR << "number of weights exceed number of atoms";
                return false;
            }

            weights_.clear();
            weights_.assign(atoms_->value().size(), 0);

            assert(l.size() <= atoms_->value().size());

            int idx = 0;
            for (auto& a : l) {
                auto w = a.asFloat(-1);
                if (w < 0) {
                    OBJ_ERR << "invalid weight value: " << a << ", using 0 instead";
                    w = 0;
                }

                weights_[idx++] = w;
            }

            wsum_ = std::accumulate(weights_.begin(), weights_.end(), t_float(0));
            if (wsum_ == 0) {
                OBJ_ERR << "weights sum is equal 0";
                return false;
            }

            dist_ = std::discrete_distribution<size_t>(weights_.begin(), weights_.end());
            return true;
        });

    seed_ = new SizeTProperty("@seed", 0);
    seed_->setSuccessFn([this](Property* p) { gen_.setSeed(seed_->value()); });
    addProperty(seed_);

    nonrep_ = new BoolProperty("@nonrep", false);
    addProperty(nonrep_);

    createInlet();
    createOutlet();
}

void RandomAtom::onBang()
{
    const auto N = atoms_->value().size();
    const auto NONREP = nonrep_->value();

    size_t idx = 0;

    if (NONREP) {
        if (N < 2) {
            OBJ_ERR << fmt::format("not enough elements for non-repeating random reneration: {}", N);
            return;
        }

        std::uniform_int_distribution<size_t> dist(0, N - 1);
        auto idx = dist(gen_.get());
        int max_tries = 512;
        // generate new index not equal to previous
        while (last_idx_ == idx && max_tries-- > 0)
            idx = dist(gen_.get());

        // update last index
        last_idx_ = idx;

    } else {
        if (N == 0) {
            OBJ_ERR << "empty atom list";
            return;
        }

        if (wsum_ == 0) {
            std::uniform_int_distribution<size_t> dist(0, N - 1);
            auto s = sizeof(dist);
            idx = dist(gen_.get());
        } else
            idx = dist_(gen_.get());
    }

    atomTo(0, atoms_->value()[idx]);
}

void setup_random_atom()
{
    ObjectFactory<RandomAtom> obj("random.atom");
    obj.addAlias("random.a");
    obj.setXletsInfo({ "bang", "list: set list to choose from" }, { "atom: random element from list" });
}
