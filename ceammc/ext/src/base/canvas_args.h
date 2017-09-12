#ifndef CANVAS_ARGS_H
#define CANVAS_ARGS_H

#include "ceammc_object.h"

using namespace ceammc;

class CanvasArgs : public BaseObject {
public:
    CanvasArgs(const PdArgs& a);

    void onBang();
};

void setup_canvas_args();

#endif // CANVAS_ARGS_H
