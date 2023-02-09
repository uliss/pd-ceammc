#include "env_to_vline.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cassert>

Env2VLine::Env2VLine(const PdArgs& args)
    : BaseObject(args)
    , stop_point_index_(-1)
    , stop_offset_us_(0)
    , state_(STATE_INIT)
    , data_sync_(0)
{
    data_sync_ = new BoolProperty("@sync", false);
    addProperty(data_sync_);

    createOutlet();
}

void Env2VLine::onBang()
{
    outputFixed();
}

void Env2VLine::onFloat(t_float f)
{
    size_t n_stops = env_->numStopPoints();

    if (n_stops == 0) {
        OBJ_DBG << "envelope contains no stop points" << env_->toString();
        return outputFixed();
    } else if (n_stops > 1) {
        OBJ_ERR << "envelope contains multiple stop points: " << n_stops;
        return;
    }

    if (f != 0.f)
        outputStart();
    else
        outputRelease();
}

void Env2VLine::onDataT(const EnvAtom& env)
{
    env_ = env;
    stop_point_index_ = 0;
    stop_offset_us_ = 0;
    state_ = STATE_INIT;

    if (data_sync_->value())
        outputFixed();
}

void Env2VLine::outputSegment(const EnvelopePoint& pt0, const EnvelopePoint& pt1, long offset_us)
{
    AtomList res;
    res.fill(Atom(0.f), 3);

    switch (pt0.type) {
    case CURVE_STEP:
        res[0].setFloat(pt1.value);
        res[1].setFloat(0);
        res[2].setFloat((pt1.utime - stop_offset_us_) / 1000.0);
        listTo(0, res);
        return;
    case CURVE_LINE:
        res[0].setFloat(pt1.value);
        res[1].setFloat(intervalMs(pt1, pt0));
        res[2].setFloat((pt0.utime - stop_offset_us_) / 1000.0);
        listTo(0, res);
        return;
    case CURVE_EXP:
        interpSegment(pt0, pt1, &Env2VLine::interpExp, res);
        return;
    case CURVE_SIN2:
        interpSegment(pt0, pt1, &Env2VLine::interpSin2, res);
        return;
    case CURVE_SIGMOID:
        interpSegment(pt0, pt1, &Env2VLine::interpSigmoid, res);
        return;
    default: {
        OBJ_ERR << "unsupported curve type: " << pt0.type;
        break;
    }
    }
}

void Env2VLine::outputFixed()
{
    if (env_->empty()) {
        OBJ_ERR << "empty envelope";
        return;
    }

    AtomList lst;
    lst.fill(Atom(0.f), 3);
    lst[0] = env_->pointAt(0).value;

    // output first points
    listTo(0, lst);

    stop_offset_us_ = 0;

    const size_t n_pt = env_->numPoints();
    for (size_t i = 1; i < n_pt; i++) {
        EnvelopePoint& pt0 = env_->pointAt(i - 1);
        EnvelopePoint& pt1 = env_->pointAt(i);

        outputSegment(pt0, pt1, pt0.utime);
    }
}

void Env2VLine::outputStart()
{
    if (env_->empty()) {
        OBJ_ERR << "empty envelope";
        return;
    }

    // output first point
    AtomList lst;
    lst.fill(Atom(0.f), 3);
    lst[0] = env_->pointAt(0).value;
    listTo(0, lst);

    // find first stop point
    stop_point_index_ = env_->nextStopIdx(0);

    // stop point is not found
    if (stop_point_index_ < 0) {
        OBJ_ERR << "fixed time enevelope";
        return;
    }

    assert(!env_->empty());
    assert(stop_point_index_ < (env_->numPoints() - 1));
    stop_offset_us_ = 0;

    // output segment until stop point
    for (long i = 1; i <= stop_point_index_; i++) {
        EnvelopePoint& pt0 = env_->pointAt(i - 1);
        EnvelopePoint& pt1 = env_->pointAt(i);

        outputSegment(pt0, pt1, pt0.utime);
    }

    state_ = STATE_ACTIVE;
    stop_offset_us_ = env_->pointAt(stop_point_index_).utime;
}

void Env2VLine::outputRelease()
{
    if (state_ != STATE_ACTIVE) {
        OBJ_ERR << "envelope was not activated...";
        return;
    }

    long offset_us = (stop_point_index_ < env_->numPoints()) ? env_->pointAt(stop_point_index_).utime : 0;

    // output from stop point till the end
    for (long i = stop_point_index_ + 1; i < env_->numPoints(); i++) {
        EnvelopePoint& pt0 = env_->pointAt(i - 1);
        EnvelopePoint& pt1 = env_->pointAt(i);

        outputSegment(pt0, pt1, pt0.utime - offset_us);
    }

    stop_point_index_ = -1;
    state_ = STATE_FINISHED;
    stop_offset_us_ = 0;
}

void Env2VLine::outputNextStop()
{
    if (env_->empty()) {
        OBJ_ERR << "empty envelope";
        return;
    }

    if (state_ == STATE_FINISHED) {
        OBJ_ERR << "envelope was is finished. Reset to execute it again";
        return;
    }

    if (state_ == STATE_INIT) {
        // output first point
        AtomList lst;
        lst.fill(Atom(0.f), 3);
        lst[0] = env_->pointAt(0).value;
        listTo(0, lst);
        state_ = STATE_ACTIVE;
    }

    size_t begin = stop_point_index_;
    stop_point_index_ = env_->nextStopIdx(stop_point_index_);

    if (stop_point_index_ < 0) {
        OBJ_DBG << "finished...";
        state_ = STATE_FINISHED;
        return;
    }

    assert(stop_point_index_ < env_->numPoints());
    long offset_us = env_->pointAt(begin).utime;

    for (long i = begin; i < stop_point_index_; i++) {
        EnvelopePoint& pt0 = env_->pointAt(i);
        EnvelopePoint& pt1 = env_->pointAt(i + 1);

        outputSegment(pt0, pt1, pt0.utime - offset_us);
    }

    stop_offset_us_ = env_->pointAt(stop_point_index_).utime;
}

void Env2VLine::interpSegment(const EnvelopePoint& pt0, const EnvelopePoint& pt1, Env2VLine::InterpMethod m, AtomList& res)
{
    static const int MAX_STEPS = 50;

    double duration_ms = intervalMs(pt1, pt0);
    int step_num = ((int(duration_ms) + 9) / 10);
    if (step_num == 0)
        step_num = 1;

    if (step_num > MAX_STEPS)
        step_num = MAX_STEPS;

    float step_len_ms = duration_ms / step_num;

    // output interpolated values
    for (int i = 1; i < step_num; i++)
        (this->*m)(i, step_len_ms, pt0, pt1, res);

    // output last point
    res[0].setFloat(pt1.value);
    res[1].setFloat(step_len_ms);
    res[2].setFloat((pt1.utime - stop_offset_us_) / 1000.0 - step_len_ms);
    listTo(0, res);
}

void Env2VLine::interpSin2(size_t step_idx, double step_ms,
    const EnvelopePoint& pt0, const EnvelopePoint& pt1, AtomList& lst)
{
    auto last_dur = pt0.timeMs() + step_idx * step_ms;
    auto v = convert::lin2sin2(last_dur, pt0.timeMs(), pt1.timeMs(), pt0.value, pt1.value);

    lst[0].setFloat(v);
    lst[1].setFloat(step_ms);
    lst[2].setFloat(last_dur - step_ms - (stop_offset_us_ / 1000.0));
    listTo(0, lst);
}

void Env2VLine::interpSigmoid(size_t step_idx, double step_ms,
    const EnvelopePoint& pt0, const EnvelopePoint& pt1, AtomList& lst)
{
    double last_dur = pt0.timeMs() + step_idx * step_ms;
    double v = convert::lin2sigmoid(last_dur,
        pt0.timeMs(), pt1.timeMs(), pt0.value, pt1.value, pt0.sigmoid_skew);

    lst[0].setFloat(v);
    lst[1].setFloat(step_ms);
    lst[2].setFloat(last_dur - step_ms - (stop_offset_us_ / 1000.0));
    listTo(0, lst);
}

void Env2VLine::interpExp(size_t step_idx, double step_ms,
    const EnvelopePoint& pt0, const EnvelopePoint& pt1,
    AtomList& lst)
{
    double last_dur = pt0.timeMs() + step_idx * step_ms;
    auto v = convert::lin2curve(last_dur,
        pt0.timeMs(), pt1.timeMs(), (double)pt0.value, (double)pt1.value, (double)pt0.data);

    lst[0].setFloat(v);
    lst[1].setFloat(step_ms);
    lst[2].setFloat(last_dur - step_ms - (stop_offset_us_ / 1000.0));
    listTo(0, lst);
}

void Env2VLine::m_next(t_symbol*, const AtomListView&)
{
    outputNextStop();
}

void Env2VLine::m_reset(t_symbol*, const AtomListView&)
{
    stop_point_index_ = 0;
    stop_offset_us_ = 0;
    state_ = STATE_INIT;
}

void setup_env_to_vline()
{
    ObjectFactory<Env2VLine> obj("env2vline");
    obj.addAlias("env->vline");
    obj.processData<DataTypeEnv>();

    obj.addMethod("next", &Env2VLine::m_next);
    obj.addMethod("reset", &Env2VLine::m_reset);

    obj.setDescription("Converts envelope to bunch of vline messages");
    obj.setCategory("env");
    obj.setKeywords({"envelope", "vline"});
}
