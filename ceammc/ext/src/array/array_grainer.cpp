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
#include "array_grainer.h"
#include "args/argcheck.h"
#include "aubio.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_units.h"
#include "fmt/core.h"
#include "grain_expr_parser.h"

#include <limits>
#include <memory>
#include <random>

constexpr const char* CHAR_ALL = "*";
constexpr const char* CHAR_FINISHED = ".";

using namespace ceammc;

using Factory = SoundExternalFactory<ArrayGrainer>;
using Proxy = Factory::ObjectProxy;

namespace {

struct FVecDeleter {
    void operator()(fvec_t* v) { del_fvec(v); }
};

struct OnsetDeleter {
    void operator()(aubio_onset_t* o)
    {
        if (o)
            del_aubio_onset(o);
    }
};

using FVecPtr = std::unique_ptr<fvec_t, FVecDeleter>;
using OnsetPtr = std::unique_ptr<aubio_onset_t, OnsetDeleter>;

uint64_t parseTimeUnit(const AtomListView& lv, double sr, uint64_t def)
{
    units::TimeValue tm(0, units::TimeValue::SAMPLE, sr);
    auto res = units::TimeValue::parse(lv);
    if (res.matchValue(tm))
        return tm.value() <= 0 ? def : tm.toSamples();
    else
        return def;
}

}

ArrayGrainer::ArrayGrainer(const PdArgs& args)
    : ArraySoundBase(args)
    , sync_(nullptr)
    , sync_interval_(nullptr)
    , sync_prob_(nullptr)
    , defer_([this]() {
        for (auto& m : defer_msg_) {
            if (m.count > 0)
                m.count--;

            if (m.count == 0) {
                auto sel = m.msg[0].asSymbol();
                dispatchMethod(sel, m.msg.view().subView(1));
            }
        }

        auto end = std::remove_if(defer_msg_.begin(), defer_msg_.end(), [](const DeferMessage& m) { return m.count == 0; });
        defer_msg_.erase(end, defer_msg_.end());
    })
{
    createSignalOutlet();
    createSignalOutlet();

    sync_ = new SymbolEnumProperty("@sync", { "none", "int", "ext" });
    sync_->setSuccessFn([this](Property*) {
        const auto v = sync_->value();
        if (v == gensym("ext"))
            cloud_.setSyncMode(GrainCloud::SYNC_EXTERNAL);
        else if (v == gensym("int"))
            cloud_.setSyncMode(GrainCloud::SYNC_INTERNAL);
        else
            cloud_.setSyncMode(GrainCloud::SYNC_NONE);
    });
    addProperty(sync_);

    sync_interval_ = new FloatProperty("@tsync", 50);
    sync_interval_->checkMinEq(1);
    sync_interval_->setUnitsMs();
    sync_interval_->setSuccessFn([this](Property*) { cloud_.setSyncInterval(sync_interval_->value()); });
    addProperty(sync_interval_);

    sync_prob_ = new FloatProperty("@prob", 1);
    sync_prob_->checkClosedRange(0, 1);
    sync_prob_->setSuccessFn([this](Property*) { cloud_.setSyncProbability(sync_prob_->value()); });
    addProperty(sync_prob_);

    defer_msg_.reserve(4);
}

void ArrayGrainer::setupDSP(t_signal** sp)
{
    ArraySoundBase::setupDSP(sp);

    if (array_.isValid())
        cloud_.setArrayData(array_.begin(), array_.size());
}

void ArrayGrainer::processBlock(const t_sample** /*in*/, t_sample** out)
{
    int done = cloud_.playBuffer(out, blockSize(), samplerate());
    if (done > 0 && defer_msg_.size() > 0)
        defer_.delay(0);
}

void ArrayGrainer::onBang()
{
    cloud_.externalSyncTick();
}

void ArrayGrainer::dump() const
{
    OBJ_POST << "cloud:\n"
                "    num grains: "
             << cloud_.size() << "\n";

    OBJ_POST << "grains: ";
    for (auto g : cloud_.grains())
        OBJ_POST << *g;

    OBJ_POST << "onsets: ";
    for (auto x : onsets_)
        OBJ_POST << x;

    SoundExternal::dump();
}

void ArrayGrainer::m_align(t_symbol* s, const AtomListView& lv)
{
    if (onsets_.empty()) {
        METHOD_DBG(s) << "no onset information, can't align";
        return;
    }

    if (lv.empty() || lv[0] == gensym(CHAR_ALL)) { /// align all
        cloud_.alignAll(onsets_);
    } else if (lv[0].isFloat()) { /// remove by #ID
        const int id = lv[0].asInt();
        if (id < 0) {
            METHOD_ERR(s) << "non-negative grain id expected, got: " << id;
            return;
        }

        cloud_.alignById(id, onsets_);
    } else if (lv[0] == gensym(CHAR_FINISHED)) { /// remove finished
        cloud_.alignFinished(onsets_);
    } else if (lv[0].isSymbol()) { /// remove by tag
        t_symbol* tag = lv[0].asT<t_symbol*>();
        cloud_.alignByTag(tag, onsets_);
    }
}

void ArrayGrainer::m_append(t_symbol* s, const AtomListView& lv)
{
    if (!checkArray(true))
        return;

    if (lv.size() < 1) {
        METHOD_ERR(s) << "NUM_GRAINS PROPS... expected";
        return;
    }

    appendGrains(lv.intAt(0, 1), lv.subView(1));
}

void ArrayGrainer::m_grain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArray(true))
        return;

    auto id = cloud_.size();
    auto grain = cloud_.appendGrain();
    if (!grain) {
        METHOD_ERR(s) << "too many grains, can't insert";
        return;
    }

    grain->setId(id);
    grain->setArraySizeInSamples(array_.size());
    GrainExprParser parser(grain);

    if (!parser.parse(lv)) {
        LIB_ERR << "parse error";
        cloud_.popGrain();
        return;
    }
}

void ArrayGrainer::m_set(t_symbol* s, const AtomListView& lv)
{
    if (!checkArray(true))
        return;

    const bool ok = lv.size() > 1;
    if (!ok) {
        METHOD_ERR(s) << "id, tag or 'all' is required";
        return;
    }

    const auto& subj = lv[0];

    if (subj.isFloat()) { /// set by ID
        const int id = lv[0].asFloat();

        if (id < 0) {
            METHOD_ERR(s) << "non-negative grain id expected, got: " << id;
            return;
        }

        for (auto g : cloud_.grains()) {
            if (g->id() == id) {
                GrainExprParser p(g);
                p.parse(lv.subView(1));
                return;
            }
        }

        METHOD_ERR(s) << "grain #" << id << " not found";
    } else if (subj == gensym(CHAR_ALL)) { /// set all grains
        GrainExprParser p(nullptr);

        for (auto g : cloud_.grains()) {
            p.setGrain(g);
            p.parse(lv.subView(1));
        }

    } else if (subj.isSymbol()) { /// set by tag
        t_symbol* tag = subj.asT<t_symbol*>();

        GrainExprParser p(nullptr);
        int num_matches = 0;

        for (auto g : cloud_.grains()) {
            if (g->equalTag(tag)) {
                p.setGrain(g);
                p.parse(lv.subView(1));
                num_matches++;
            }
        }

        if (num_matches == 0) {
            METHOD_DBG(s) << "no matches for tag '" << tag->s_name << "' found";
            return;
        }

    } else
        METHOD_ERR(s) << "ID or tag expected";
}

void ArrayGrainer::m_pause(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() >= 1 && lv[0] == gensym(CHAR_ALL)) { /// pause all
        cloud_.pauseAll(lv.boolAt(1, true));
    } else if (lv.size() >= 1 && lv[0].isFloat()) { /// pause by #ID
        auto id = lv[0].asInt();
        if (id < 0) {
            METHOD_ERR(s) << "non-negative grain id expected, got: " << id;
            return;
        }

        cloud_.pauseById(id, lv.boolAt(1, true));
    } else if (lv.size() >= 1 && lv[0].isSymbol()) { /// pause by tag
        auto tag = lv[0].asT<t_symbol*>();
        cloud_.pauseByTag(tag, lv.boolAt(1, true));
    }
}

void ArrayGrainer::appendGrains(int n, const AtomListView& args)
{
    if (n < 1) {
        OBJ_ERR << "invalid number of grains: " << n;
        return;
    }

    GrainExprParser parser(nullptr);

    for (int i = 0; i < n; i++) {
        auto id = cloud_.size();
        auto grain = cloud_.appendGrain();
        if (!grain) {
            OBJ_ERR << "memory error, can't insert";
            break;
        }

        grain->setId(id);

        grain->setArraySizeInSamples(array_.size());
        parser.setGrain(grain);

        if (!parser.parse(args)) {
            OBJ_ERR << "parse error: " << args;
            cloud_.popGrain();
            return;
        }

        OBJ_DBG << "added: " << *grain;
    }
}

void ArrayGrainer::m_fill(t_symbol* s, const AtomListView& lv)
{
    if (!checkArray(true))
        return;

    if (lv.size() < 1) {
        METHOD_ERR(s) << "NUM_GRAINS PROPS... expected";
        return;
    }

    cloud_.clear();
    appendGrains(lv.intAt(0, 1), lv.subView(1));
}

void ArrayGrainer::m_clear(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty() || lv[0] == gensym(CHAR_ALL)) { /// remove all
        cloud_.clear();
    } else if (lv[0].isFloat()) { /// remove by #ID
        const int id = lv[0].asInt();
        if (id < 0) {
            METHOD_ERR(s) << "non-negative grain id expected, got: " << id;
            return;
        }

        cloud_.removeById(id);
    } else if (lv[0] == gensym(CHAR_FINISHED)) { /// remove finished
        cloud_.removeFinished();
    } else if (lv[0].isSymbol()) { /// remove by tag
        t_symbol* tag = lv[0].asT<t_symbol*>();
        cloud_.removeByTag(tag);
    }
}

void ArrayGrainer::m_onsets(t_symbol* s, const AtomListView& lv)
{
    constexpr const char* DEFAULT_METHOD = "default";
    constexpr int DEFAULT_BUFFER_SIZE = 1024;

    if (!checkArray(true))
        return;

    const size_t BS = DEFAULT_BUFFER_SIZE; // buffer size
    const size_t HS = BS / 2; // hop size
    const size_t AS = array_.size();
    const char* METHOD = (lv.size() > 0 && lv[0].isSymbol()) ? lv[0].asT<t_symbol*>()->s_name : DEFAULT_METHOD;

    OnsetPtr detector(new_aubio_onset(METHOD, BS, HS, sys_getsr()));

    if (!detector) {
        METHOD_ERR(s) << "can't create onset detector";
        return;
    }

    aubio_onset_set_minioi_ms(detector.get(), clip_min<t_float, 64>(lv.floatAt(1, 250)));
    aubio_onset_set_silence(detector.get(), clip<t_float, -80, 0>(lv.floatAt(2, -40)));

    FVecPtr in(new_fvec(HS));
    FVecPtr out(new_fvec(1));

    onsets_.clear();

    size_t hop_idx = 0;
    while (true) {
        const auto hop_offset = hop_idx * HS;

        if (hop_offset >= AS)
            break;

        for (size_t i = 0; i < HS; i++) {
            const auto hop_pos = hop_offset + i;
            if (hop_pos < AS)
                fvec_set_sample(in.get(), array_[hop_pos], i);
            else
                fvec_set_sample(in.get(), 0.f, i);
        }

        aubio_onset_do(detector.get(), in.get(), out.get());
        if (fvec_get_sample(out.get(), 0) > 0)
            onsets_.push_back(aubio_onset_get_last(detector.get()));

        hop_idx++;
    }
}

void ArrayGrainer::m_slice(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("N:i[1,64] "
                                "LEN:t? "
                                "GRAIN:a*");

    args::ArgMatchList m;
    if (!chk.check(lv, this, &m)) {
        chk.usage(this, s);
        return;
    }

    if (!checkArray(true))
        return;

    const auto N = m[0].asInt();
    const int64_t alen = parseTimeUnit(m[1], samplerate(), array_.size());
    const auto grain_props = m[2];
    const double glen = alen / double(N);
    const int64_t ilen = std::round(glen);

    GrainExprParser parser(nullptr);

    for (int i = 0; i < N; i++) {
        auto id = cloud_.size();
        auto grain = cloud_.appendGrain();
        if (!grain) {
            OBJ_ERR << "memory error, can't add grain";
            break;
        }

        grain->setId(id);
        grain->setArraySizeInSamples(alen);
        parser.setGrain(grain);

        if (grain_props.size() > 0 && !parser.parse(grain_props)) {
            OBJ_ERR << "invalid grain props: " << grain_props;
            cloud_.popGrain();
            return;
        }

        const int64_t pos = std::round(i * glen);
        grain->setLengthInSamples(ilen);
        grain->setArrayPosInSamples(pos);
        grain->setTimeBefore(pos);
        grain->setTimeAfter(clip_min<int64_t, 0>(alen - grain->durationInSamples()));
    }
}

void ArrayGrainer::m_shuffle(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("TAG:s?");

    if (!chk.check(lv, this)) {
        chk.usage(this, s);
        return;
    }

    if (!checkArray(true))
        return;

    cloud_.shuffle(lv.symbolAt(0, &s_));
}

void ArrayGrainer::m_reverse(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("TAG:s?");

    if (!chk.check(lv, this)) {
        chk.usage(this, s);
        return;
    }

    if (!checkArray(true))
        return;

    cloud_.reverse(lv.symbolAt(0, &s_));
}

void ArrayGrainer::m_spread(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("DUR:t? "
                                "TAG:s?");

    args::ArgMatchList m;
    if (!chk.check(lv, this, &m)) {
        chk.usage(this, s);
        return;
    }

    if (!checkArray(true))
        return;

    const int64_t gdur = parseTimeUnit(m[0], samplerate(), array_.size());
    auto tag = m[1].symbolAt(0, &s_);

    cloud_.spread(gdur, tag);
}

void ArrayGrainer::m_permutate(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("N:i[-16,16]? "
                                "TAG:s?");

    args::ArgMatchList m;
    if (!chk.check(lv, this, &m)) {
        chk.usage(this, s);
        return;
    }

    if (!checkArray(true))
        return;

    const int n = m[0].intAt(0, 1);
    auto tag = m[1].symbolAt(0, &s_);

    cloud_.permutate(n, tag);
}

void ArrayGrainer::m_defer(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("COUNT:i[1,255]? "
                                "MSG:s "
                                "ARG:a*");

    args::ArgMatchList m;
    if (!chk.check(lv, this, &m)) {
        chk.usage(this, s);
        return;
    }

    DeferMessage def;
    def.msg.reserve(m[1].size() + m[2].size());
    def.msg.insert_back(m[1]);
    def.msg.insert_back(m[2]);
    def.count = m[0].intAt(0, 1);
    defer_msg_.push_back(std::move(def));
}

void ArrayGrainer::dispatchMethod(t_symbol* m, const AtomListView& args)
{
    auto p = (Proxy*)owner();
    Factory::defaultListMethod(p, m, args.size(), args.toPdData());
}

void setup_array_grainer()
{
    Factory obj("array.grainer~", OBJECT_FACTORY_DEFAULT);

    obj.addMethod("align", &ArrayGrainer::m_align);
    obj.addMethod("append", &ArrayGrainer::m_append);
    obj.addMethod("clear", &ArrayGrainer::m_clear);
    obj.addMethod("fill", &ArrayGrainer::m_fill);
    obj.addMethod("grain", &ArrayGrainer::m_grain);
    obj.addMethod("onsets", &ArrayGrainer::m_onsets);
    obj.addMethod("pause", &ArrayGrainer::m_pause);
    obj.addMethod("set", &ArrayGrainer::m_set);
    obj.addMethod("slice", &ArrayGrainer::m_slice);
    obj.addMethod("spread", &ArrayGrainer::m_spread);
    obj.addMethod("shuffle", &ArrayGrainer::m_shuffle);
    obj.addMethod("defer", &ArrayGrainer::m_defer);
    obj.addMethod("reverse", &ArrayGrainer::m_reverse);
    obj.addMethod("permutate", &ArrayGrainer::m_permutate);

    obj.setDescription("array granulator");
    obj.setCategory("array");
    obj.setKeywords({"array", "grain", "particle", "granular"});
}
