#ifndef PAN_SQRT_H
#define PAN_SQRT_H

#include "pan_base.h"

class PanSqrt : public PanBase {

public:
    PanSqrt(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_pan_sqrt();

#endif // PAN_SQRT_H
