#ifndef ENV_TO_VLINE_H
#define ENV_TO_VLINE_H

#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

enum EnvelopeState {
    STATE_INIT,
    STATE_ACTIVE,
    STATE_FINISHED
};

class Env2VLine : public BaseObject {
    EnvAtom env_;
    long stop_point_index_;
    size_t stop_offset_us_;
    EnvelopeState state_;
    BoolProperty* data_sync_;

public:
    typedef void (Env2VLine::*InterpMethod)(size_t, double,
        const EnvelopePoint&, const EnvelopePoint&, AtomList&);

public:
    Env2VLine(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onDataT(const EnvAtom& env);

    void outputSegment(const EnvelopePoint& pt0, const EnvelopePoint& pt1, long offset_us);
    void outputFixed();
    void outputStart();
    void outputRelease();
    void outputNextStop();

    void interpSegment(const EnvelopePoint& pt0, const EnvelopePoint& pt1, InterpMethod m, AtomList& res);
    void interpSin2(size_t step_idx, double step_ms, const EnvelopePoint& pt0, const EnvelopePoint& pt1, AtomList& lst);
    void interpSigmoid(size_t step_idx, double step_ms, const EnvelopePoint& pt0, const EnvelopePoint& pt1, AtomList& lst);
    void interpExp(size_t step_idx, double step_ms, const EnvelopePoint& pt0, const EnvelopePoint& pt1, AtomList& lst);

    void m_next(t_symbol*, const AtomListView&);
    void m_reset(t_symbol*, const AtomListView&);
};

void setup_env_to_vline();

#endif // ENV_TO_VLINE_H
