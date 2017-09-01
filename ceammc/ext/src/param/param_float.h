#ifndef PARAM_FLOAT_H
#define PARAM_FLOAT_H

#include "ceammc_object.h"

#include <vector>

using namespace ceammc;

class ParamFloat : public BaseObject {
    SizeTProperty* max_;
    FloatProperty* init_;
    SymbolProperty* name_;
    std::vector<t_float> params_;

public:
    ParamFloat(const PdArgs& args);

    void onFloat(float f);

    void m_clear(t_symbol*, const AtomList&);
};

void setup_param_float();

#endif // PARAM_FLOAT_H
