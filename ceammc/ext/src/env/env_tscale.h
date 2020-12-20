#ifndef ENV_TSCALE_H
#define ENV_TSCALE_H

#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

class EnvTimeScale : public BaseObject {
    FloatProperty* scale_;

public:
    EnvTimeScale(const PdArgs& a);
    void onDataT(const EnvAtom& env);
};

void setup_env_tscale();

#endif // ENV_TSCALE_H
