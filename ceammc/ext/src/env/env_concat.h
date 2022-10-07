#ifndef ENV_CONCAT_H
#define ENV_CONCAT_H

#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

class EnvConcat : public BaseObject {
    EnvAtom res_env_;
    EnvAtom right_;

public:
    EnvConcat(const PdArgs& args);

    void onBang() override;
    void onDataT(const EnvAtom& env);
    void onInlet(size_t n, const AtomListView& lv) override;
};

void setup_env_concat();

#endif // ENV_CONCAT_H
