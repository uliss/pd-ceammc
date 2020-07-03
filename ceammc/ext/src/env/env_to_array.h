#ifndef ENV_TO_ARRAY_H
#define ENV_TO_ARRAY_H

#include "../array/array_base.h"
#include "ceammc_array.h"
#include "ceammc_data.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "datatype_env.h"

using namespace ceammc;

class Env2Array : public ArrayMod {
    SymbolEnumProperty* mode_;

public:
    Env2Array(const PdArgs& args);

    void onDataT(const DataAtom<DataTypeEnv>& env);
    void render(const DataTypeEnv& env);
};

void setup_env_to_array();

#endif // ENV_TO_ARRAY_H
