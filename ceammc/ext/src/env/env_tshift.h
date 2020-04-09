#ifndef ENV_TSHIFT_H
#define ENV_TSHIFT_H

#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

class EnvTimeShift : public BaseObject {
    FloatProperty* shift_;

public:
    EnvTimeShift(const PdArgs& args);
    void onDataT(const EnvAtom& env);
};

void setup_env_tshift();

#endif // ENV_TSHIFT_H
