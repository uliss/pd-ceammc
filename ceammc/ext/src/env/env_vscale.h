#ifndef ENV_VSCALE_H
#define ENV_VSCALE_H

#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

class EnvValueScale : public BaseObject {
    FloatProperty* scale_;

public:
    EnvValueScale(const PdArgs& args);
    void onDataT(const EnvAtom& env);
};

void setup_env_vscale();

#endif // ENV_VSCALE_H
