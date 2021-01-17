#ifndef PATCH_ARGS_H
#define PATCH_ARGS_H

#include "ceammc_object.h"

using namespace ceammc;

class PatchArgs : public BaseObject {
    IntProperty* from_;

public:
    PatchArgs(const PdArgs& a);

    void onBang() override;
};

void setup_patch_args();

#endif // PATCH_ARGS_H
