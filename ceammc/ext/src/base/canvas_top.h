#ifndef CANVAS_TOP_H
#define CANVAS_TOP_H

#include "ceammc_object.h"

using namespace ceammc;

class CanvasTop : public BaseObject {
public:
    CanvasTop(const PdArgs& a);
    void onBang() override;
};

void setup_canvas_top();

#endif // CANVAS_TOP_H
