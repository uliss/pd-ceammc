#ifndef ENV_ENV_H
#define ENV_ENV_H

#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

class Envelope : public BaseObject {
    DataTypeEnv env_;

public:
    Envelope(const PdArgs& args);
    void dump() const;

    const DataTypeEnv& envelope() const;

    void onBang();
    void onDataT(const DataTypeEnv& env);

    void m_addPoint(t_symbol* s, const AtomList& lst);
    void m_removePoint(t_symbol* s, const AtomList& lst);
    void m_setPointTime(t_symbol* s, const AtomList& lst);
    void m_setPointValue(t_symbol* s, const AtomList& lst);
    void m_setPoint(t_symbol* s, const AtomList& lst);
    void m_setStopPoint(t_symbol* s, const AtomList& lst);
    void m_appendSegment(t_symbol* s, const AtomList& lst);
    void m_setSegmentType(t_symbol* s, const AtomList& lst);
    void m_plus(t_symbol* s, const AtomList& lst);
    void m_multiply(t_symbol* s, const AtomList& lst);
    void m_shift(t_symbol* s, const AtomList& lst);

    void m_AR(t_symbol* s, const AtomList& lst);
    void m_ASR(t_symbol* s, const AtomList& lst);
    void m_ADSR(t_symbol* s, const AtomList& lst);

    void m_EADSR(t_symbol* s, const AtomList& lst);
    void m_EASR(t_symbol* s, const AtomList& lst);
    void m_EAR(t_symbol* s, const AtomList& lst);

    void m_step(t_symbol* s, const AtomList& lst);
    void m_line(t_symbol* s, const AtomList& lst);
    void m_sin2(t_symbol* s, const AtomList& lst);
    void m_exp(t_symbol* s, const AtomList& lst);
    void m_sigmoid(t_symbol* s, const AtomList& lst);

    void m_clear(t_symbol*, const AtomList&);

    AtomList p_npoints() const;
    AtomList p_length() const;
    AtomList p_values() const;
    AtomList p_points() const;
    AtomList p_stops() const;
};

void setup_envelope();

#endif // ENV_ENV_H
