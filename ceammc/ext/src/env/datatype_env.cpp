#include "datatype_env.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_datastorage.h"
#include "ceammc_log.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "fmt/format.h"

#include <algorithm>
#include <boost/range.hpp>
#include <unordered_map>

constexpr const char* TYPE_NAME = "Env";

CEAMMC_DEFINE_HASH(adsr);
CEAMMC_DEFINE_HASH(asr);
CEAMMC_DEFINE_HASH(ar);
CEAMMC_DEFINE_HASH(eadsr);
CEAMMC_DEFINE_HASH(easr);
CEAMMC_DEFINE_HASH(ear);
CEAMMC_DEFINE_HASH(step)
CEAMMC_DEFINE_HASH(line)
CEAMMC_DEFINE_HASH(exp)
CEAMMC_DEFINE_HASH(sin2)
CEAMMC_DEFINE_HASH(sigmoid)

CEAMMC_DEFINE_HASH(all)
CEAMMC_DEFINE_HASH(none)
CEAMMC_DEFINE_HASH(time)
CEAMMC_DEFINE_HASH(value)

namespace {

const char* CURVE_TYPES[] = {
    str_step, str_line, str_exp, str_sin2, str_sigmoid
};

const char* FIX_TYPES[] = {
    str_none, str_all, str_time, str_value
};

struct find_by_time {
    size_t t;
    find_by_time(size_t time_us)
        : t(time_us)
    {
    }

    bool operator()(const EnvelopePoint& n) { return n.utime == t; }
};

bool compareByTime(const EnvelopePoint& n0, const EnvelopePoint& n1)
{
    return n0.utime < n1.utime;
}

bool compareByValue(const EnvelopePoint& n0, const EnvelopePoint& n1)
{
    return n0.value < n1.value;
}

bool has_stop(const EnvelopePoint& n)
{
    return n.stop;
}

bool isValidAttack(const Atom& p)
{
    if (!p.isFloat() || p.asFloat() < 0) {
        LIB_ERR << "invalid attack time: " << p;
        return false;
    }

    return true;
}

bool isValidDecay(const Atom& p)
{
    if (!p.isFloat() || p.asFloat() < 0) {
        LIB_ERR << "invalid decay time: " << p;
        return false;
    }

    return true;
}

bool isValidRelease(const Atom& p)
{
    if (!p.isFloat() || p.asFloat() < 0) {
        LIB_ERR << "invalid release time: " << p;
        return false;
    }

    return true;
}

bool isValidSustain(const Atom& p)
{
    if (!p.isFloat() || p.asFloat() < 0 || p.asFloat() > 100) {
        LIB_ERR << "invalid sustain level value: " << p;
        return false;
    }

    return true;
}

#define VALIDATE_ATTACK(lst, idx, str)  \
    {                                   \
        if (!isValidAttack(lst[idx])) { \
            LIB_ERR << str;             \
            return false;               \
        }                               \
    }

#define VALIDATE_ATTACK_CURVE(lst, idx, str)                \
    {                                                       \
        const Atom& p = lst[idx];                           \
        if (!p.isFloat()) {                                 \
            LIB_ERR << "invalid attack curve value: " << p; \
            LIB_ERR << str;                                 \
            return false;                                   \
        }                                                   \
    }

#define VALIDATE_DECAY(lst, idx, str)  \
    {                                  \
        if (!isValidDecay(lst[idx])) { \
            LIB_ERR << str;            \
            return false;              \
        }                              \
    }

#define VALIDATE_DECAY_CURVE(lst, idx, str)                \
    {                                                      \
        const Atom& p = lst[idx];                          \
        if (!p.isFloat()) {                                \
            LIB_ERR << "invalid decay curve value: " << p; \
            LIB_ERR << str;                                \
            return false;                                  \
        }                                                  \
    }

#define VALIDATE_SUSTAIN(lst, idx, str)  \
    {                                    \
        if (!isValidSustain(lst[idx])) { \
            LIB_ERR << str;              \
            return false;                \
        }                                \
    }

#define VALIDATE_RELEASE(lst, idx, str)  \
    {                                    \
        if (!isValidRelease(lst[idx])) { \
            LIB_ERR << str;              \
            return false;                \
        }                                \
    }

#define VALIDATE_RELEASE_CURVE(lst, idx, str)                \
    {                                                        \
        const Atom& p = lst[idx];                            \
        if (!p.isFloat()) {                                  \
            LIB_ERR << "invalid release curve value: " << p; \
            LIB_ERR << str;                                  \
            return false;                                    \
        }                                                    \
    }

const char* fixToStr(uint8_t f)
{
    switch (f) {
    case EnvelopePoint::FIX_TIME:
        return str_time;
    case EnvelopePoint::FIX_VALUE:
        return str_value;
    case EnvelopePoint::FIX_BOTH:
        return str_all;
    case EnvelopePoint::FIX_NONE:
    default:
        return str_none;
    }
}

void symbolToFix(t_symbol* s, std::uint8_t& fix)
{
    switch (crc32_hash(s)) {
    case hash_time:
        fix = EnvelopePoint::FIX_TIME;
        break;
    case hash_value:
        fix = EnvelopePoint::FIX_VALUE;
        break;
    case hash_all:
        fix = EnvelopePoint::FIX_BOTH;
        break;
    case hash_none:
    default:
        fix = EnvelopePoint::FIX_NONE;
        break;
    }
}

void exportPoint(const EnvelopePoint& pt, std::string& res)
{
    auto str = std::back_inserter(res);
    fmt::format_to(str, "    [time: {0:.7g} value: {1:.7g} type: {2}",
        pt.timeMs(), pt.value, CURVE_TYPES[pt.type]);
    if (pt.type != CURVE_LINE || pt.type != CURVE_STEP)
        fmt::format_to(str, " curve: {}",
            (pt.type == CURVE_SIGMOID) ? pt.sigmoid_skew : pt.data);

    if (pt.fix_pos != EnvelopePoint::FIX_NONE)
        fmt::format_to(str, " fix: {}", fixToStr(pt.fix_pos));
    fmt::format_to(str, " stop: {}]", pt.stop ? 1 : 0);
}

EnvelopePoint pointFromDict(const DataTypeDict& dict)
{
    EnvelopePoint pt { 0, 0 };

    for (auto& kv : dict) {
        const auto key = kv.first;
        const auto& val = kv.second;
        switch (crc32_hash(key)) {
        case "time"_hash:
            pt.utime = val.floatAt(0, 0) * 1000;
            break;
        case "value"_hash:
            pt.value = val.floatAt(0, 0);
            break;
        case "type"_hash:
            symbol2curve(val.symbolAt(0, gensym(str_line)), pt.type);
            break;
        case "stop"_hash:
            pt.stop = val.boolAt(0, false);
            break;
        case "curve"_hash:
            pt.data = val.floatAt(0, 0);
            break;
        case "skew"_hash:
            pt.sigmoid_skew = val.floatAt(0, 0);
            break;
        case "fix"_hash:
            symbolToFix(val.symbolAt(0, gensym(str_none)), pt.fix_pos);
            break;
        default:
            LIB_ERR << fmt::format("[{}] invalid key '{}'", TYPE_NAME, key->s_name);
            break;
        }
    }

    return pt;
}

Atom createEnv(const DictAtom& dict)
{
    EnvAtom res;

    for (auto& kv : *dict) {
        const auto key = kv.first;
        const auto& val = kv.second;

        switch (crc32_hash(key)) {
        case hash_adsr:
            res->setADSR(val);
            return res;
        case hash_asr:
            res->setASR(val);
            return res;
        case hash_ar:
            res->setAR(val);
            return res;
        case hash_step:
            res->setStep(val);
            return res;
        case hash_line:
            res->setLine(val);
            return res;
        case hash_ear:
            res->setEAR(val);
            return res;
        case "points"_hash: {
            for (auto& a : val) {
                auto dict = a.asDataT<DataTypeDict>();
                if (dict)
                    res->insertPoint(pointFromDict(*dict));
            }
        } break;
        default:
            LIB_ERR << fmt::format("invalid key: '{}'", key->s_name);
            break;
        }
    }

    return res;
}
}

DataTypeId DataTypeEnv::dataType = DataStorage::instance().registerNewType(TYPE_NAME, nullptr, createEnv);

DataTypeEnv::DataTypeEnv()
{
}

DataTypeEnv::DataTypeEnv(const DataTypeEnv& env)
    : points_(env.points_)
{
}

DataTypeEnv::DataTypeEnv(DataTypeEnv&& env)
    : points_(std::move(env.points_))
{
}

DataTypeId DataTypeEnv::type() const noexcept
{
    return dataType;
}

static const char* to_string(CurveType t)
{
    if (t >= 0 && t < boost::size(CURVE_TYPES))
        return CURVE_TYPES[t];

    return "?";
}

const DataTypeEnv::NamedMethodList DataTypeEnv::named_methods = {
    NamedMethod { hash_adsr, &DataTypeEnv::setADSR },
    NamedMethod { hash_ar, &DataTypeEnv::setAR },
    NamedMethod { hash_asr, &DataTypeEnv::setASR },
    NamedMethod { hash_eadsr, &DataTypeEnv::setEADSR },
    NamedMethod { hash_ear, &DataTypeEnv::setEAR },
    NamedMethod { hash_easr, &DataTypeEnv::setEASR },
    NamedMethod { hash_exp, &DataTypeEnv::setExponential },
    NamedMethod { hash_line, &DataTypeEnv::setLine },
    NamedMethod { hash_sigmoid, &DataTypeEnv::setSigmoid },
    NamedMethod { hash_sin2, &DataTypeEnv::setSin2 },
    NamedMethod { hash_step, &DataTypeEnv::setStep },
};

bool DataTypeEnv::isEqual(const AbstractData* d) const noexcept
{
    if (d->type() != dataType)
        return false;

    const DataTypeEnv* env = d->as<DataTypeEnv>();
    return this->operator==(*env);
}

bool DataTypeEnv::empty() const
{
    return points_.empty();
}

const EnvelopePoint& DataTypeEnv::front() const
{
    return points_.front();
}

const EnvelopePoint& DataTypeEnv::back() const
{
    return points_.back();
}

EnvelopePoint& DataTypeEnv::front()
{
    return points_.front();
}

EnvelopePoint& DataTypeEnv::back()
{
    return points_.back();
}

size_t DataTypeEnv::totalLength() const
{
    if (points_.empty())
        return 0;

    return points_.back().utime - points_.front().utime;
}

DataTypeEnv::iterator DataTypeEnv::begin()
{
    return points_.begin();
}

DataTypeEnv::iterator DataTypeEnv::end()
{
    return points_.end();
}

size_t DataTypeEnv::numPoints() const
{
    return points_.size();
}

EnvelopePoint& DataTypeEnv::pointAt(size_t idx)
{
    return points_.at(idx);
}

const EnvelopePoint& DataTypeEnv::pointAt(size_t idx) const
{
    return points_.at(idx);
}

double DataTypeEnv::valueAtTime(size_t time_us) const
{
    if (points_.empty() || time_us > points_.back().utime)
        return 0;

    long upper_idx = -1;
    for (size_t i = 0; i < points_.size(); i++) {
        const EnvelopePoint& n = points_[i];
        if (n.utime == time_us) {
            return n.value;
        } else if (n.utime > time_us) {
            upper_idx = i;
            break;
        }
    }

    if (upper_idx < 1)
        return 0;

    const EnvelopePoint& l = points_[upper_idx - 1];
    const EnvelopePoint& u = points_[upper_idx];

    switch (l.type) {
    case CURVE_STEP:
        return l.value;
    case CURVE_LINE:
        return convert::lin2lin<double>(time_us, l.utime, u.utime, l.value, u.value);
    case CURVE_EXP:
        return convert::lin2curve(double(time_us),
            double(l.utime), double(u.utime),
            double(l.value), double(u.value), double(l.data));
    case CURVE_SIN2:
        return convert::lin2sin2(time_us, l.utime, u.utime, l.value, u.value);
    case CURVE_SIGMOID:
        return convert::lin2sigmoid(time_us, l.utime, u.utime, l.value, u.value, l.sigmoid_skew);
    default:
        return 0;
    }
}

void DataTypeEnv::resizeTime(size_t time_us)
{
    if (totalLength() < 1)
        return;

    double factor = double(time_us) / double(totalLength());

    for (size_t i = 0; i < points_.size(); i++)
        points_[i].utime = round(factor * points_[i].utime);
}

void DataTypeEnv::shiftTime(long time_us)
{
    if (empty())
        return;

    if (time_us < 0 && (points_[0].utime < -time_us)) {
        LIB_ERR << "invalid envelope shift value: " << time_us;
        return;
    }

    for (size_t i = 0; i < points_.size(); i++)
        points_[i].utime += time_us;
}

void DataTypeEnv::scaleTime(float factor)
{
    if (factor < 0)
        return;

    for (size_t i = 0; i < points_.size(); i++)
        points_[i].utime = round(factor * points_[i].utime);
}

void DataTypeEnv::scaleValue(float factor)
{
    for (size_t i = 0; i < points_.size(); i++)
        points_[i].value *= factor;
}

DataTypeEnv DataTypeEnv::normalize() const
{
    std::pair<float, float> yr = valueRange();
    float vrange = yr.second - yr.first;
    if (vrange == 0 || totalLength() == 0)
        return DataTypeEnv();

    DataTypeEnv res(*this);

    // shift
    if (points_[0].utime > 0)
        res.shiftTime(-points_[0].utime);

    // value scale
    for (size_t i = 0; i < res.points_.size(); i++)
        res.points_[i].value = (res.points_[i].value - yr.first) / vrange;

    return res;
}

std::pair<float, float> DataTypeEnv::valueRange() const
{
    std::pair<float, float> res;
    if (points_.empty())
        return res;

    auto it = std::minmax_element(points_.begin(), points_.end(), compareByValue);

    res.first = it.first->value;
    res.second = it.second->value;
    return res;
}

DataTypeEnv::iterator DataTypeEnv::nextStopPoint(DataTypeEnv::iterator pos)
{
    if (pos == points_.end())
        return points_.end();

    for (++pos; pos != points_.end(); ++pos) {
        if (pos->stop)
            return pos;
    }

    return points_.end();
}

long DataTypeEnv::nextStopIdx(size_t current_stop)
{
    if (points_.empty() || (current_stop + 1) >= points_.size())
        return -1;

    PointList::iterator it = nextStopPoint(points_.begin() + current_stop);
    return (it == points_.end()) ? points_.size() - 1 : std::distance(points_.begin(), it);
}

bool DataTypeEnv::hasPointAtTime(size_t time_us) const
{
    return std::find_if(points_.begin(), points_.end(), find_by_time(time_us)) != points_.end();
}

EnvelopePoint* DataTypeEnv::pointAtTime(size_t time_us)
{
    PointList::iterator it = std::find_if(points_.begin(), points_.end(), find_by_time(time_us));

    return (it == points_.end()) ? 0 : &(*it);
}

const EnvelopePoint* DataTypeEnv::pointAtTime(size_t time_us) const
{
    PointList::const_iterator it = std::find_if(points_.begin(), points_.end(), find_by_time(time_us));

    return (it == points_.end()) ? 0 : &(*it);
}

size_t DataTypeEnv::insertPoint(const EnvelopePoint& n)
{
    PointList::iterator it = std::find_if(points_.begin(), points_.end(), find_by_time(n.utime));
    if (it != points_.end()) {
        *it = n;
        return std::distance(points_.begin(), it);
    }

    // then you can keep it ordered at each insertion
    it = std::upper_bound(points_.begin(), points_.end(), n, compareByTime);
    PointList::iterator node_it = points_.insert(it, n);
    return std::distance(points_.begin(), node_it);
}

bool DataTypeEnv::removePointByTime(size_t time_us)
{
    PointList::iterator it = std::find_if(points_.begin(), points_.end(),
        find_by_time(time_us));

    if (it == points_.end())
        return false;

    points_.erase(it);
    return true;
}

bool DataTypeEnv::removePoint(size_t idx)
{
    if (idx >= points_.size())
        return false;

    points_.erase(points_.begin() + idx);
    return true;
}

bool DataTypeEnv::appendSegment(size_t time_us, double end_value, CurveType t, double curve_skew)
{
    if (time_us == 0)
        return false;

    if (points_.empty())
        points_.push_back(EnvelopePoint(0, 0));

    points_.back().sigmoid_skew = curve_skew;
    points_.back().data = curve_skew;
    points_.back().type = t;
    points_.push_back(EnvelopePoint(points_.back().utime + time_us, end_value));

    return true;
}

void DataTypeEnv::clear()
{
    points_.clear();
}

void DataTypeEnv::setAR(size_t attack_us, size_t release_us, float value)
{
    clear();
    points_.push_back(EnvelopePoint(0, 0, false, CURVE_LINE));
    points_.push_back(EnvelopePoint(attack_us, value, false, CURVE_LINE));
    points_.push_back(EnvelopePoint(attack_us + release_us, 0));
    points_.front().setFixBoth();
    points_.back().setFixValue();
}

void DataTypeEnv::setEAR(size_t attack_us, float attack_curve, size_t release_us, float release_curve)
{
    clear();
    points_.push_back(EnvelopePoint(0, 0, false, CURVE_EXP, attack_curve));
    points_.push_back(EnvelopePoint(attack_us, 1, false, CURVE_EXP, release_curve));
    points_.push_back(EnvelopePoint(attack_us + release_us, 0));
    points_.front().setFixBoth();
    points_.back().setFixValue();
}

void DataTypeEnv::setASR(size_t attack_us, size_t release_us, double value)
{
    clear();
    points_.push_back(EnvelopePoint(0, 0, false, CURVE_LINE));
    points_.push_back(EnvelopePoint(attack_us, value, true, CURVE_LINE));
    points_.push_back(EnvelopePoint(attack_us + release_us, 0));
    points_.front().setFixBoth();
    points_.back().setFixValue();
}

void DataTypeEnv::setEASR(size_t attack_us, float attack_curve, size_t release_us, float release_curve)
{
    clear();
    points_.push_back(EnvelopePoint(0, 0, false, CURVE_EXP, attack_curve));
    points_.push_back(EnvelopePoint(attack_us, 1, true, CURVE_EXP, release_curve));
    points_.push_back(EnvelopePoint(attack_us + release_us, 0));
    points_.front().setFixBoth();
    points_.back().setFixValue();
}

void DataTypeEnv::setADSR(size_t attack_us, size_t decay_us, double sustain_level, size_t release_us)
{
    clear();
    points_.push_back(EnvelopePoint(0, 0, false, CURVE_LINE));
    points_.push_back(EnvelopePoint(attack_us, 1, false, CURVE_LINE));
    points_.push_back(EnvelopePoint(attack_us + decay_us, sustain_level, true, CURVE_LINE));
    points_.push_back(EnvelopePoint(attack_us + decay_us + release_us, 0, false, CURVE_LINE));
    points_.front().setFixBoth();
    points_[1].setFixValue();
    points_.back().setFixValue();
}

void DataTypeEnv::setEADSR(size_t attack_us, float attack_curve, size_t decay_us, float decay_curve, double sustain_level, size_t release_us, float release_curve)
{
    clear();
    points_.push_back(EnvelopePoint(0, 0, false, CURVE_EXP, attack_curve));
    points_.push_back(EnvelopePoint(attack_us, 1, false, CURVE_EXP, decay_curve));
    points_.push_back(EnvelopePoint(attack_us + decay_us, sustain_level, true, CURVE_EXP, release_curve));
    points_.push_back(EnvelopePoint(attack_us + decay_us + release_us, 0, false, CURVE_LINE));
    points_.front().setFixBoth();
    points_.back().setFixValue();
}

bool DataTypeEnv::setAR(const AtomListView& lv)
{
    const char* usage = "Usage: AR attack(ms) release(ms)";

    if (lv.size() != 2) {
        LIB_ERR << usage;
        return false;
    }

    VALIDATE_ATTACK(lv, 0, usage);
    VALIDATE_RELEASE(lv, 1, usage);

    setAR(lv[0].asFloat() * 1000, lv[1].asFloat() * 1000);

    return true;
}

bool DataTypeEnv::setASR(const AtomListView& lv)
{
    const char* usage = "Usage: ASR attack(ms) release(ms)";

    if (lv.size() != 2) {
        LIB_ERR << usage;
        return false;
    }

    VALIDATE_ATTACK(lv, 0, usage);
    VALIDATE_RELEASE(lv, 1, usage);

    setASR(lv[0].asFloat() * 1000, lv[1].asFloat() * 1000);

    return true;
}

bool DataTypeEnv::setADSR(const AtomListView& lv)
{
    static const char* usage = "Usage: ADSR attack(ms) decay(ms) sustain(%) release(ms)";

    if (lv.size() != 4) {
        LIB_ERR << usage;
        return false;
    }

    VALIDATE_ATTACK(lv, 0, usage);
    VALIDATE_DECAY(lv, 1, usage);
    VALIDATE_SUSTAIN(lv, 2, usage);
    VALIDATE_RELEASE(lv, 3, usage);

    setADSR(lv[0].asFloat() * 1000,
        lv[1].asFloat() * 1000,
        lv[2].asFloat() / 100,
        lv[3].asFloat() * 1000);

    return true;
}

bool DataTypeEnv::setEAR(const AtomListView& lv)
{
    static const char* usage = "Usage: EAR attack(ms) attack_curve release(ms) release_curve";

    if (lv.size() != 4) {
        LIB_ERR << usage;
        return false;
    }

    VALIDATE_ATTACK(lv, 0, usage);
    VALIDATE_ATTACK_CURVE(lv, 1, usage);
    VALIDATE_RELEASE(lv, 2, usage);
    VALIDATE_RELEASE_CURVE(lv, 3, usage);

    setEAR(lv[0].asFloat() * 1000, lv[1].asFloat(), lv[2].asFloat() * 1000, lv[3].asFloat());

    return true;
}

bool DataTypeEnv::setEASR(const AtomListView& lv)
{
    static const char* usage = "Usage: EASR attack(ms) attack_curve release(ms) release_curve";

    if (lv.size() != 4) {
        LIB_ERR << usage;
        return false;
    }

    VALIDATE_ATTACK(lv, 0, usage);
    VALIDATE_ATTACK_CURVE(lv, 1, usage);
    VALIDATE_RELEASE(lv, 2, usage);
    VALIDATE_RELEASE_CURVE(lv, 3, usage);

    setEASR(lv[0].asFloat() * 1000, lv[1].asFloat(), lv[2].asFloat() * 1000, lv[3].asFloat());

    return true;
}

bool DataTypeEnv::setEADSR(const AtomListView& lv)
{
    static const char* usage = "Usage: EADSR attack(ms) attack_curve "
                               "decay(ms) decay_curve sustain_level "
                               "release(ms) release_curve";

    if (lv.size() != 7) {
        LIB_ERR << usage;
        return false;
    }

    VALIDATE_ATTACK(lv, 0, usage);
    VALIDATE_ATTACK_CURVE(lv, 1, usage);
    VALIDATE_DECAY(lv, 2, usage);
    VALIDATE_DECAY_CURVE(lv, 3, usage);
    VALIDATE_SUSTAIN(lv, 4, usage);
    VALIDATE_RELEASE(lv, 5, usage);
    VALIDATE_RELEASE_CURVE(lv, 6, usage);

    setEADSR(lv[0].asFloat() * 1000, lv[1].asFloat(),
        lv[2].asFloat() * 1000, lv[3].asFloat(),
        lv[4].asFloat() / 100,
        lv[5].asFloat() * 1000, lv[6].asFloat());

    return true;
}

bool DataTypeEnv::setStep(const AtomListView& lv)
{
    // check args
    if (lv.size() % 2 != 1) {
        LIB_ERR << "Usage: step VAL_0 LEN_0 VAL_1 [LEN_1 VAL_2] ...";
        return false;
    }

    // clear envelope
    points_.clear();

    // insert start point
    points_.push_back(EnvelopePoint(0, lv[0].asFloat(), false, CURVE_STEP));

    t_float offset_ms = 0;

    // inserts remaining
    for (size_t i = 1; i < (lv.size() - 1); i += 2) {
        t_float time_ms = lv[i].asFloat() * 1000;

        if (time_ms < 1) {
            LIB_ERR << "invalid segment, skipping...";
            return false;
        }

        offset_ms += time_ms;
        points_.push_back(EnvelopePoint(offset_ms, lv[i + 1].asFloat(), false, CURVE_STEP));
    }

    return true;
}

bool DataTypeEnv::setLine(const AtomListView& lv)
{
    // check args
    if (lv.size() % 2 != 1) {
        LIB_ERR << "Usage: line VAL_0 LEN_0 VAL_1 [LEN_1 VAL_2] ...";
        return false;
    }

    // clear envelope
    points_.clear();

    // insert start point
    points_.push_back(EnvelopePoint(0, lv[0].asFloat(), false, CURVE_LINE));

    t_float offset_us = 0;

    // inserts remaining
    for (size_t i = 1; i < (lv.size() - 1); i += 2) {
        t_float time_us = lv[i].asFloat() * 1000;

        if (time_us < 1) {
            LIB_ERR << "invalid segment, skipping...";
            return false;
        }

        offset_us += time_us;
        points_.push_back(EnvelopePoint(offset_us, lv[i + 1].asFloat(), false, CURVE_LINE));
    }

    return true;
}

bool DataTypeEnv::setExponential(const AtomListView& lv)
{
    const size_t N = lv.size();

    // check args
    if (!(N > 3 && (N % 3 == 1))) {
        LIB_ERR << "Usage: exp VAL_0 LEN_0 CURVE_0 VAL_1 [LEN_1 CURVE VAL_2] ...";
        return false;
    }

    // clear envelope
    points_.clear();

    t_float offset_us = 0;

    for (size_t i = 0; i < (N - 2); i += 3) {
        t_float value = lv[i].asFloat();
        t_float segm_len_us = lv[i + 1].asFloat() * 1000;
        t_float curve = lv[i + 2].asFloat();

        if (segm_len_us < 1) {
            LIB_ERR << "invalid segment, skipping...";
            return false;
        }

        points_.push_back(EnvelopePoint(offset_us, value, false, CURVE_EXP, curve));
        offset_us += segm_len_us;
    }

    points_.push_back(EnvelopePoint(offset_us, lv.back().asFloat(), false, CURVE_EXP, 0));

    return true;
}

bool DataTypeEnv::setSin2(const AtomListView& lv)
{
    // check args
    if (lv.size() % 2 != 1) {
        LIB_ERR << "Usage: sin2 VAL_0 LEN_0 VAL_1 [LEN_1 VAL_2] ...";
        return false;
    }

    // clear envelope
    points_.clear();

    // insert start point
    points_.push_back(EnvelopePoint(0, lv[0].asFloat(), false, CURVE_SIN2));

    t_float offset_us = 0;

    // inserts remaining
    for (size_t i = 1; i < (lv.size() - 1); i += 2) {
        t_float time_us = lv[i].asFloat() * 1000;

        if (time_us < 1) {
            LIB_ERR << "invalid segment, skipping...";
            return false;
        }

        offset_us += time_us;
        points_.push_back(EnvelopePoint(offset_us, lv[i + 1].asFloat(), false, CURVE_SIN2));
    }

    return true;
}

bool DataTypeEnv::setSigmoid(const AtomListView& lv)
{
    const size_t N = lv.size();

    // check args
    if (!(N > 3 && (N % 3 == 1))) {
        LIB_ERR << "Usage: sigmoid VAL_0 LEN_0 CURVE_0 VAL_1 [LEN_1 CURVE VAL_2] ...";
        return false;
    }

    // clear envelope
    points_.clear();

    t_float offset_us = 0;

    for (size_t i = 0; i < (N - 2); i += 3) {
        t_float value = lv[i].asFloat();
        t_float segm_len_us = lv[i + 1].asFloat() * 1000;
        t_float curve = lv[i + 2].asFloat();

        if (segm_len_us < 1) {
            LIB_ERR << "invalid segment, skipping...";
            return false;
        }

        points_.push_back(EnvelopePoint(offset_us, value, false, CURVE_SIGMOID, 0));
        points_.back().sigmoid_skew = curve;
        offset_us += segm_len_us;
    }

    points_.push_back(EnvelopePoint(offset_us, lv.back().asFloat(0), false, CURVE_SIGMOID, 0));

    return true;
}

bool DataTypeEnv::setNamedEnvelope(const char* name, const AtomListView& args)
{
    const auto hash = crc32_hash(name);

    for (const auto& n : named_methods) {
        if (n.crc32_hash == hash)
            return (this->*n.m)(args);
    }

    return false;
}

bool DataTypeEnv::isNamedEnvelope(const char* name) const
{
    const auto hash = crc32_hash(name);

    return std::find_if(
               named_methods.begin(),
               named_methods.end(),
               [hash](const NamedMethod& m) { return m.crc32_hash == hash; })
        != named_methods.end();
}

bool DataTypeEnv::hasStopPoints() const
{
    return std::find_if(points_.begin(), points_.end(), has_stop) != points_.end();
}

size_t DataTypeEnv::numStopPoints() const
{
    return std::count_if(points_.begin(), points_.end(), has_stop);
}

DataTypeEnv* DataTypeEnv::clone() const
{
    return new DataTypeEnv(*this);
}

void DataTypeEnv::render(std::vector<float>::iterator first, std::vector<float>::iterator last)
{
    render<std::vector<float>::iterator>(first, last);
}

bool DataTypeEnv::operator==(const DataTypeEnv& e) const
{
    return e.points_ == points_;
}

DataTypeEnv& DataTypeEnv::operator+=(const DataTypeEnv& env)
{
    if (env.empty())
        return *this;

    size_t len_us = points_.empty() ? 0 : points_.back().utime;

    if (!points_.empty()) {
        EnvelopePoint pt(env.front());
        pt.utime += len_us;
        points_.back() = pt;

    } else {
        points_.push_back(env.front());
    }

    const size_t n = env.points_.size();
    for (size_t i = 1; i < n; i++) {
        EnvelopePoint pt(env.points_[i]);
        pt.utime += len_us;
        points_.push_back(pt);
    }

    return *this;
}

DataTypeEnv& DataTypeEnv::operator=(const DataTypeEnv& env)
{
    if (&env != this)
        points_ = env.points_;

    return *this;
}

DataTypeEnv& DataTypeEnv::operator=(DataTypeEnv&& env)
{
    if (&env != this)
        points_ = std::move(env.points_);

    return *this;
}

void DataTypeEnv::sort()
{
    std::stable_sort(points_.begin(), points_.end(), compareByTime);
}

bool DataTypeEnv::interpolated(const DataTypeEnv& env0, const DataTypeEnv& env1, float v, DataTypeEnv& dest)
{
    if (v < 0 || v > 1) {
        LIB_ERR << "invalid interpolation coefficent: " << v << ". Should be in range";
        return false;
    }

    if (env0.points_.size() != env1.points_.size()) {
        LIB_ERR << "envelope should contain equal point numbers";
        return false;
    }

    DataTypeEnv res(env0);

    for (size_t i = 0; i < env0.points_.size(); i++) {
        const EnvelopePoint& p0 = env0.points_[i];
        const EnvelopePoint& p1 = env1.points_[i];
        EnvelopePoint& out = res.points_[i];

        if (p0.type != p1.type) {
            LIB_ERR << "different segment type: " << i;
            return false;
        }

        out.utime = convert::lin2lin<double>(v, 0, 1, p0.utime, p1.utime);
        out.value = convert::lin2lin<double>(v, 0, 1, p0.value, p1.value);
    }

    res.sort();
    dest = res;
    return true;
}

bool DataTypeEnv::isAR(bool checkVal) const
{
    if (points_.size() != 3)
        return false;

    if (!(points_[0].type == CURVE_LINE
            && points_[1].type == CURVE_LINE))
        return false;

    if (checkVal) {
        if (points_[0].value != 0
            || points_[1].value != 1
            || points_[2].value != 0)
            return false;
    }

    return true;
}

bool DataTypeEnv::isADSR(bool checkVal) const
{
    if (points_.size() != 4)
        return false;

    if (!(points_[0].type == CURVE_LINE
            && points_[1].type == CURVE_LINE
            && points_[2].type == CURVE_LINE))
        return false;

    if (checkVal) {
        if (points_[0].value != 0
            || points_[1].value != 1
            || points_[3].value != 0)
            return false;
    }

    return true;
}

bool DataTypeEnv::checkAR() const
{
    return points_.size() == 3
        && (points_[0].utime == 0
            && points_[0].value == 0
            && points_[0].stop == false
            && points_[0].type == CURVE_LINE
            && points_[0].fix_pos == EnvelopePoint::FIX_BOTH)
        && (points_[1].value == 1
            && points_[1].stop == false
            && points_[1].type == CURVE_LINE)
        && (points_[2].value == 0
            && points_[2].stop == false
            && points_[2].type == CURVE_LINE
            && points_[2].fix_pos == EnvelopePoint::FIX_VALUE);
}

bool DataTypeEnv::checkASR() const
{
    return points_.size() == 3
        && (points_[0].utime == 0
            && points_[0].value == 0
            && points_[0].stop == false
            && points_[0].type == CURVE_LINE
            && points_[0].fix_pos == EnvelopePoint::FIX_BOTH)
        && (points_[1].value == 1
            && points_[1].stop == true
            && points_[1].type == CURVE_LINE)
        && (points_[2].value == 0
            && points_[2].stop == false
            && points_[2].type == CURVE_LINE
            && points_[2].fix_pos == EnvelopePoint::FIX_VALUE);
}

bool DataTypeEnv::checkADSR() const
{
    return points_.size() == 4
        && (points_[0].utime == 0
            && points_[0].value == 0
            && points_[0].stop == false
            && points_[0].type == CURVE_LINE
            && points_[0].fix_pos == EnvelopePoint::FIX_BOTH)
        && (points_[1].value == 1
            && points_[1].stop == false
            && points_[1].type == CURVE_LINE)
        && (points_[2].stop == true
            && points_[2].type == CURVE_LINE)
        && (points_[3].value == 0
            && points_[3].stop == false
            && points_[3].type == CURVE_LINE
            && points_[3].fix_pos == EnvelopePoint::FIX_VALUE);
}

std::string DataTypeEnv::toString() const noexcept
{
    return toDictString();
}

std::string DataTypeEnv::toListStringContent() const noexcept
{
    return " ";
}

std::string DataTypeEnv::toDictStringContent() const noexcept
{
    std::string res;

    try {
        if (checkAR()) {
            const auto A = points_[1].timeMs();
            const auto R = points_[2].timeMs() - A;
            res = fmt::format("ar: {:.3g} {:.3g}", A, R);
        } else if (checkASR()) {
            const auto A = points_[1].timeMs();
            const auto R = points_[2].timeMs() - A;
            res = fmt::format("asr: {:.3g} {:.3g}", A, R);
        } else if (checkADSR()) {
            const auto A = points_[1].timeMs();
            const auto D = points_[2].timeMs() - A;
            const auto S = points_[2].value * 100;
            const auto R = points_[3].timeMs() - (A + D);
            res = fmt::format("adsr: {:.3g} {:.3g} {:.3g} {:.3g}", A, D, S, R);
        } else if (!points_.empty()) {
            res = fmt::format("points:\n");
            for (auto& pt : points_) {
                exportPoint(pt, res);
                res.push_back('\n');
            }

        } else
            res = " ";
    } catch (std::exception& e) {
        LIB_ERR << e.what();
    }

    return res;
}

bool DataTypeEnv::set(const AbstractData* d) noexcept
{
    return setDataT<DataTypeEnv>(d);
}

bool operator==(const EnvelopePoint& p0, const EnvelopePoint& p1)
{
    return p0.utime == p1.utime
        && p0.value == p1.value
        && p0.data == p1.data
        && p0.sigmoid_skew == p1.sigmoid_skew
        && p0.type == p1.type
        && p0.stop == p1.stop;
}

std::ostream& operator<<(std::ostream& os, const DataTypeEnv& env)
{
    os << env.toString();
    return os;
}

bool symbol2curve(t_symbol* s, CurveType& t)
{
    const size_t n = boost::size(CURVE_TYPES);
    for (size_t i = 0; i < n; i++) {
        if (gensym(CURVE_TYPES[i]) == s) {
            t = static_cast<CurveType>(i);
            return true;
        }
    }

    return false;
}

t_symbol* curve2symbol(CurveType t)
{
    return (t >= 0 && t < boost::size(CURVE_TYPES)) ? gensym(CURVE_TYPES[t]) : nullptr;
}

bool isValidCurve(t_symbol* s)
{
    CurveType t;
    return symbol2curve(s, t);
}

static bool canMergePoint(const DataTypeEnv& env0, const DataTypeEnv& env1)
{
    if (env0.empty() || env1.empty())
        return false;

    return env0.back().value == env1.front().value;
}

DataTypeEnv operator+(const DataTypeEnv& env0, const DataTypeEnv& env1)
{
    DataTypeEnv res(env0);

    long d0 = env0.points_.empty() ? 0 : env0.back().utime;
    long d1 = env1.points_.empty() ? 0 : env1.front().utime;

    if (canMergePoint(env0, env1))
        res.points_.pop_back();

    for (size_t i = 0; i < env1.numPoints(); i++) {
        EnvelopePoint pt = env1.points_[i];
        pt.utime += (d0 - d1);
        res.points_.push_back(pt);
    }

    return res;
}
