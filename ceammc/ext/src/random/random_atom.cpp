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
    , last_idx_(std::numeric_limits<uint32_t>::max())
    , is_uniform_(true)
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
        [this](const AtomListView& lv) -> bool {
            const auto NA = atoms_->value().size();
            const auto NW = lv.size();
            if (NW > NA) {
                OBJ_ERR << fmt::format("number of weights is greater then number of atoms: {}>{}", NW, NA);
                return false;
            }

            weights_.clear();
            weights_.assign(NA, 0);

            // empty weights: use uniform distribution
            if (NW == 0) {
                is_uniform_ = true;
                return true;
            }

            int idx = 0;
            for (auto& a : lv) {
                auto w = a.asFloat(-1);
                if (w < 0) {
                    OBJ_ERR << "invalid weight value: " << a << ", using 0 instead";
                    w = 0;
                }

                weights_[idx++] = w;
            }

            if (std::accumulate(weights_.begin(), weights_.end(), t_float(0)) == 0) {
                OBJ_ERR << "weights sum is equal 0";
                return false;
            }

            discrete_ = DiscreteDist(weights_.begin(), weights_.end());
            is_uniform_ = false;
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

        updateUniformDistrib();
        idx = genIndex();
        int max_tries = 512;
        // generate new index not equal to previous
        while (last_idx_ == idx && max_tries-- > 0)
            idx = genIndex();

        // update last index
        last_idx_ = idx;

    } else {
        if (N == 0) {
            OBJ_ERR << "empty atom list";
            return;
        }

        updateUniformDistrib();
        idx = genIndex();
    }

    atomTo(0, atoms_->value()[idx]);
}

void RandomAtom::onInlet(size_t n, const AtomListView& lv)
{
    if (atoms_->set(lv)) {
        if (!is_uniform_) {
            OBJ_DBG << "clearing atoms weights, using uniform distribution";
            is_uniform_ = true;
            weights_.clear();
        }
    }
}

uint32_t RandomAtom::genIndex()
{
    if (is_uniform_)
        return uniform_(gen_.get());
    else
        return discrete_(gen_.get());
}

void RandomAtom::updateUniformDistrib()
{
    uniform_ = UniformDist(0, atoms_->value().size() - 1);
}

void setup_random_atom()
{
    ObjectFactory<RandomAtom> obj("random.atom");
    obj.addAlias("random.a");
    obj.setXletsInfo({ "bang", "list: set list to choose from" }, { "atom: random element from list" });
}
