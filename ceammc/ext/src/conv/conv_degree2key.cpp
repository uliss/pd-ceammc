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
#include "conv_degree2key.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_music_scale.h"

#include <ctime>
#include <random>

CEAMMC_DEFINE_HASH(major)

constexpr const char* DEFAULT_SCALE = str_major;

ConvDegree2Key::ConvDegree2Key(const PdArgs& args)
    : BaseObject(args)
    , scale_(nullptr)
    , scale_ptr_(music::ScaleLibrary::instance().findByHash(hash_major))
{
    createOutlet();

    createCbListProperty("@all", []() {
        AtomList res;
        auto& l = music::ScaleLibrary::instance().all();
        for (auto s : l)
            res.append(gensym(s->name().c_str()));

        return res;
    });

    scale_ = new SymbolEnumProperty("@scale", gensym(DEFAULT_SCALE));
    auto& l = music::ScaleLibrary::instance().all();
    for (auto s : l) {
        if (s->name() != DEFAULT_SCALE)
            scale_->appendEnum(gensym(s->name().c_str()));
    }

    scale_->setArgIndex(0);
    scale_->setSuccessFn([this](Property*) {
        auto s = scale_->value();
        scale_ptr_ = music::ScaleLibrary::instance().find(s);
    });
    addProperty(scale_);
}

void ConvDegree2Key::onFloat(t_float f)
{
    if (scale_ptr_)
        floatTo(0, scale_ptr_->degreeToKey(f));
}

void ConvDegree2Key::onList(const AtomList& l)
{
    if (scale_ptr_)
        listTo(0, l.mapFloat([this](t_float f) { return scale_ptr_->degreeToKey(f); }));
}

void ConvDegree2Key::m_random(t_symbol* s, const AtomListView& v)
{
    static std::mt19937 gen;

    if (!scale_ptr_) {
        METHOD_ERR(s) << "no scale";
        return;
    }

    std::uniform_int_distribution<int> dist(0, scale_ptr_->size() - 1);
    auto k = scale_ptr_->degreeToKey(dist(gen));
    floatTo(0, k);
}

void setup_conv_degree2key()
{
    ObjectFactory<ConvDegree2Key> obj("conv.degree2key");
    obj.addAlias("degree->key");
    obj.addMethod("random", &ConvDegree2Key::m_random);
}
