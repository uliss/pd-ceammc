#ifndef PAN_COSINE_H
#define PAN_COSINE_H

#include "pan_base.h"

class PanCosine : public PanBase {

public:
    PanCosine(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_pan_cosine();

#endif // PAN_COSINE_H
