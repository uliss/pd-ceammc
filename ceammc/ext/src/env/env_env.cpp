#include "env_env.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_env.h"

#include <boost/range.hpp>
#include <limits>

CEAMMC_DEFINE_SYM_HASH(step);
CEAMMC_DEFINE_SYM_HASH(line);
CEAMMC_DEFINE_SYM_HASH(exp);
CEAMMC_DEFINE_SYM_HASH(sin2);
CEAMMC_DEFINE_SYM_HASH(sigmoid);

CEAMMC_DEFINE_SYM_HASH(adsr);
CEAMMC_DEFINE_SYM_HASH(asr);
CEAMMC_DEFINE_SYM_HASH(ar);
CEAMMC_DEFINE_SYM_HASH(eadsr);
CEAMMC_DEFINE_SYM_HASH(easr);
CEAMMC_DEFINE_SYM_HASH(ear);
CEAMMC_DEFINE_SYM_HASH(empty);

Envelope::Envelope(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    t_symbol* method = parsedPosArgs().symbolAt(0, sym_empty());

    if (env_->isNamedEnvelope(method->s_name))
        env_->setNamedEnvelope(method->s_name, parsedPosArgs().slice(1));
    else if (method != sym_empty()) // wrong name
        OBJ_ERR << "unknown arguments: " << parsedPosArgs();

    {
        Property* p = createCbIntProperty("@npoints",
            [this]() -> int { return env_->numPoints(); });
        p->checkNonNegative();
    }

    {
        Property* p = createCbFloatProperty("@length",
            [this]() -> t_float { return env_->totalLength() / t_float(1000); });
        p->setUnitsMs();
        p->checkNonNegative();
    }

    createCbListProperty("@points", [this]() -> AtomList { return p_points(); });
    createCbListProperty("@values", [this]() -> AtomList { return p_values(); });
    createCbListProperty("@stops", [this]() -> AtomList { return p_stops(); });
}

void Envelope::dump() const
{
    BaseObject::dump();
    OBJ_DBG << env_->toString();
}

const DataTypeEnv& Envelope::envelope() const
{
    return *env_;
}

void Envelope::onBang()
{
    atomTo(0, env_);
}

void Envelope::onDataT(const EnvAtom& env)
{
    env_ = env;
    onBang();
}

void Envelope::m_addPoint(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT, s))
        return;

    int point_time = lv[0].asFloat() * 1000;
    if (point_time < 0) {
        OBJ_ERR << "invalid point time: " << point_time;
        return;
    }

    t_float point_value = lv[1].asFloat();
    bool stop_point = lv.boolAt(2, false);
    t_symbol* curve = lv.symbolAt(3, sym_line());
    t_float curve_data = lv.floatAt(4, 1);

    CurveType curve_type = CURVE_LINE;

    switch (crc32_hash(curve)) {
    case hash_step:
        curve_type = CURVE_STEP;
        break;
    case hash_exp:
        curve_type = CURVE_EXP;
        break;
    case hash_sin2:
        curve_type = CURVE_SIN2;
        break;
    case hash_sigmoid:
        curve_type = CURVE_SIGMOID;
        break;
    case hash_line:
    default:
        curve_type = CURVE_LINE;
        break;
    }

    EnvelopePoint pt(point_time, point_value, stop_point, curve_type);
    pt.data = curve_data;
    env_.detachData();
    env_->insertPoint(pt);
}

void Envelope::m_removePoint(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, s))
        return;

    size_t point_idx = lv[0].asSizeT();
    if (point_idx >= env_->numPoints()) {
        OBJ_ERR << "invalid point index: " << point_idx;
        return;
    }

    env_.detachData();
    if (!env_->removePoint(point_idx))
        OBJ_ERR << "can't remove point at: " << point_idx;
}

void Envelope::m_setPointTime(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, ARG_FLOAT, s))
        return;

    size_t point_idx = lv[0].asSizeT();
    if (point_idx >= env_->numPoints()) {
        OBJ_ERR << "invalid point index: " << point_idx;
        return;
    }

    env_.detachData();
    env_->pointAt(point_idx).utime = lv[1].asFloat() * 1000;
    env_->sort();
}

void Envelope::m_setPointValue(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, ARG_FLOAT, s))
        return;

    size_t point_idx = lv[0].asSizeT();
    if (point_idx >= env_->numPoints()) {
        OBJ_ERR << "invalid point index: " << point_idx;
        return;
    }

    env_.detachData();
    env_->pointAt(point_idx).value = lv[1].asFloat();
}

void Envelope::m_setPoint(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, s))
        return;

    size_t idx = lv[0].asSizeT();
    t_float time_ms = lv.floatAt(1, -1);
    t_float value = lv.floatAt(2, 0);
    int stop = lv.intAt(2, 0);

    if (idx >= env_->numPoints()) {
        OBJ_ERR << "invalid point index: " << idx;
        return;
    }

    if (time_ms < 0) {
        OBJ_ERR << "invalid points time: " << time_ms;
        return;
    }

    EnvelopePoint& p = env_->pointAt(idx);
    p.utime = time_ms * 1000;
    p.value = value;
    p.stop = stop;

    env_.detachData();
    env_->sort();
}

void Envelope::m_setStopPoint(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, ARG_BOOL, s))
        return;

    size_t point_idx = lv[0].asSizeT();
    if (point_idx >= env_->numPoints()) {
        OBJ_ERR << "invalid point index: " << point_idx;
        return;
    }

    env_.detachData();
    env_->pointAt(point_idx).stop = lv[1].asInt();
}

void Envelope::m_appendSegment(t_symbol* s, const AtomListView& lv)
{
    t_symbol* type = lv.symbolAt(0, NULL);
    int length_ms = lv.intAt(1, -1);
    t_float value = lv.floatAt(2, std::numeric_limits<t_float>::max());

    t_float curve_skew = lv.floatAt(3, 0);

    if (length_ms < 1
        || value == std::numeric_limits<t_float>::max()
        || !type) {

        OBJ_ERR << "Usage: " << s->s_name
                << " (step|line|exp|sin2|sigmoid) length_ms target_value [curve_degree=0]";
        return;
    }

    CurveType ctype;
    if (!symbol2curve(type, ctype)) {
        OBJ_ERR << "invalid segment type: " << type->s_name;
        OBJ_ERR << "   valid values are: step, line, exp, sin2, sigmoid";
        return;
    }

    env_.detachData();
    if (!env_->appendSegment(length_ms * 1000, value, ctype, curve_skew))
        OBJ_ERR << "can't append segment";
}

void Envelope::m_setSegmentType(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, ARG_SYMBOL, s))
        return;

    if (env_->numPoints() == 0)
        return;

    size_t seg_idx = lv[0].asSizeT();

    if (seg_idx >= (env_->numPoints() - 1)) {
        OBJ_ERR << "invalid segment index: " << seg_idx;
        return;
    }

    t_symbol* t = lv[1].asSymbol();
    CurveType seg_type = CURVE_LINE;
    if (!symbol2curve(t, seg_type)) {
        OBJ_ERR << "invalid segment type: " << t->s_name;
        return;
    }

    env_.detachData();
    env_->pointAt(seg_idx).type = seg_type;
}

void Envelope::m_plus(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    t_float v = lv[0].asFloat();

    env_.detachData();
    for (size_t i = 0; i < env_->numPoints(); i++)
        env_->pointAt(i).value += v;
}

void Envelope::m_multiply(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    t_float v = lv[0].asFloat();

    env_.detachData();
    for (size_t i = 0; i < env_->numPoints(); i++)
        env_->pointAt(i).value *= v;
}

void Envelope::m_shift(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    env_.detachData();
    long v = lv[0].asFloat() * 1000;
    env_->shiftTime(v);
}

void Envelope::m_AR(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    env_->setAR(lv);
}

void Envelope::m_ASR(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    env_->setASR(lv);
}

void Envelope::m_ADSR(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    env_->setADSR(lv);
}

void Envelope::m_EADSR(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    env_->setEADSR(lv);
}

void Envelope::m_EASR(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    env_->setEASR(lv);
}

void Envelope::m_EAR(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    env_->setEAR(lv);
}

void Envelope::m_step(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() % 2 != 1 || !lv.allOf(isFloat)) {
        OBJ_ERR << "Usage: step VAL LENGTH VAL [LENGTH VAL]...";
        return;
    }

    env_.detachData();
    if (!env_->setStep(lv))
        OBJ_ERR << "Can't set step envelope";
}

void Envelope::m_line(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() % 2 != 1 || !lv.allOf(isFloat)) {
        OBJ_ERR << "Usage: line VAL LENGTH VAL [LENGTH VAL]...";
        return;
    }

    env_.detachData();
    if (!env_->setLine(lv))
        OBJ_ERR << "Can't set line envelope";
}

void Envelope::m_sin2(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    if (!env_->setSin2(lv))
        OBJ_ERR << "Can't set sin2 envelope";
}

void Envelope::m_exp(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    if (!env_->setExponential(lv))
        OBJ_ERR << "Can't set exponential envelope";
}

void Envelope::m_sigmoid(t_symbol* s, const AtomListView& lv)
{
    env_.detachData();
    if (!env_->setSigmoid(lv))
        OBJ_ERR << "Can't set sigmoid envelope";
}

void Envelope::m_clear(t_symbol*, const AtomListView&)
{
    env_.detachData();
    env_->clear();
}

AtomList Envelope::p_length() const
{
    return AtomList(Atom(env_->totalLength() / 1000.f));
}

AtomList Envelope::p_values() const
{
    AtomList res;
    res.reserve(env_->numPoints());

    for (size_t i = 0; i < env_->numPoints(); i++)
        res.append(Atom(env_->pointAt(i).value));

    return res;
}

AtomList Envelope::p_points() const
{
    AtomList res;
    res.reserve(env_->numPoints());

    for (size_t i = 0; i < env_->numPoints(); i++)
        res.append(Atom(env_->pointAt(i).timeMs()));

    return res;
}

AtomList Envelope::p_stops() const
{
    AtomList res;
    for (size_t i = 0; i < env_->numPoints(); i++) {
        if (env_->pointAt(i).stop)
            res.append(Atom(i));
    }

    return res;
}

void setup_envelope_env()
{
    ObjectFactory<Envelope> obj("envelope");
    obj.addAlias("env");
    obj.processData<DataTypeEnv>();
    obj.addMethod("add_point", &Envelope::m_addPoint);
    obj.addMethod("remove_point", &Envelope::m_removePoint);
    obj.addMethod("set_point_value", &Envelope::m_setPointValue);
    obj.addMethod("set_point_time", &Envelope::m_setPointTime);
    obj.addMethod("set_stop_point", &Envelope::m_setStopPoint);
    obj.addMethod("set_point", &Envelope::m_setPoint);
    obj.addMethod("add_seg", &Envelope::m_appendSegment);
    obj.addMethod("set_seg_type", &Envelope::m_setSegmentType);
    obj.addMethod("+", &Envelope::m_plus);
    obj.addMethod("*", &Envelope::m_multiply);
    obj.addMethod(">>", &Envelope::m_shift);
    obj.addMethod(str_adsr, &Envelope::m_ADSR);
    obj.addMethod(str_asr, &Envelope::m_ASR);
    obj.addMethod(str_ar, &Envelope::m_AR);
    obj.addMethod(str_eadsr, &Envelope::m_EADSR);
    obj.addMethod(str_easr, &Envelope::m_EASR);
    obj.addMethod(str_ear, &Envelope::m_EAR);
    obj.addMethod(str_step, &Envelope::m_step);
    obj.addMethod(str_line, &Envelope::m_line);
    obj.addMethod(str_exp, &Envelope::m_exp);
    obj.addMethod(str_sin2, &Envelope::m_sin2);
    obj.addMethod(str_sigmoid, &Envelope::m_sigmoid);

    obj.addMethod("clear", &Envelope::m_clear);

    obj.setDescription("Envelope data type");
    obj.setCategory("env");
    obj.setKeywords({"envelope"});
}
