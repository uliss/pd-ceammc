#ifndef ENV_MIX_H
#define ENV_MIX_H

#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

class EnvMix : public BaseObject {
    DataTypeEnv env0_, env1_;

public:
    EnvMix(const PdArgs& args);
    void onFloat(t_float v);
    void onInlet(size_t n, const AtomList& lst);

    void dump() const;
};

void setup_env_mix();

#endif // ENV_MIX_H
