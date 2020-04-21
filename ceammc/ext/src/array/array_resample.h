#ifndef ARRAY_RESAMPLE_H
#define ARRAY_RESAMPLE_H

#include <memory>

#include "array_base.h"
#include "ceammc_property_enum.h"

class ArrayResample : public BaseObject {
    Array src_array_;
    Array dest_array_;

    SymbolProperty* src_name_;
    SymbolProperty* dest_name_;

    FloatProperty* ratio_;
    SymbolEnumProperty* quality_;

public:
    ArrayResample(const PdArgs& a);
    void onBang() override;

private:
    void resampleCopy();
    void resampleSingle();
};

void setup_array_resample();

#endif // ARRAY_RESAMPLE_H
