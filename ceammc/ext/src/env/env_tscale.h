#ifndef ENV_TSCALE_H
#define ENV_TSCALE_H

#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

class EnvTimeScale : public BaseObject {
    FloatPropertyMin* scale_;

public:
    EnvTimeScale(const PdArgs& a);
    void onDataT(const DataTypeEnv& env);
};

void setup_env_tscale();

#endif // ENV_TSCALE_H
