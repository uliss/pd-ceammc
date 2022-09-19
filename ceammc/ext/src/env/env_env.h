#ifndef ENV_ENV_H
#define ENV_ENV_H

#include "ceammc_data.h"
#include "ceammc_object.h"

using namespace ceammc;

class DataTypeEnv;
using EnvAtom = DataAtom<DataTypeEnv>;

class Envelope : public BaseObject {
    EnvAtom env_;

public:
    Envelope(const PdArgs& args);
    void dump() const override;

    const DataTypeEnv& envelope() const;

    void onBang() override;
    void onDataT(const EnvAtom& env);

    void m_addPoint(t_symbol* s, const AtomListView& lv);
    void m_removePoint(t_symbol* s, const AtomListView& lv);
    void m_setPointTime(t_symbol* s, const AtomListView& lv);
    void m_setPointValue(t_symbol* s, const AtomListView& lv);
    void m_setPoint(t_symbol* s, const AtomListView& lv);
    void m_setStopPoint(t_symbol* s, const AtomListView& lv);
    void m_appendSegment(t_symbol* s, const AtomListView& lv);
    void m_setSegmentType(t_symbol* s, const AtomListView& lv);
    void m_plus(t_symbol* s, const AtomListView& lv);
    void m_multiply(t_symbol* s, const AtomListView& lv);
    void m_shift(t_symbol* s, const AtomListView& lv);

    void m_AR(t_symbol* s, const AtomListView& lv);
    void m_ASR(t_symbol* s, const AtomListView& lv);
    void m_ADSR(t_symbol* s, const AtomListView& lv);

    void m_EADSR(t_symbol* s, const AtomListView& lv);
    void m_EASR(t_symbol* s, const AtomListView& lv);
    void m_EAR(t_symbol* s, const AtomListView& lv);

    void m_step(t_symbol* s, const AtomListView& lv);
    void m_line(t_symbol* s, const AtomListView& lv);
    void m_sin2(t_symbol* s, const AtomListView& lv);
    void m_exp(t_symbol* s, const AtomListView& lv);
    void m_sigmoid(t_symbol* s, const AtomListView& lv);

    void m_clear(t_symbol*, const AtomListView&);

    AtomList p_length() const;
    AtomList p_values() const;
    AtomList p_points() const;
    AtomList p_stops() const;
};

void setup_envelope_env();

#endif // ENV_ENV_H
