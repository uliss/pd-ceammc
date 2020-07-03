#ifndef PAN_LINEAR_H
#define PAN_LINEAR_H

#include "pan_base.h"

class PanLinear : public PanBase {
public:
    PanLinear(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_pan_linear();

#endif // PAN_LINEAR_H
