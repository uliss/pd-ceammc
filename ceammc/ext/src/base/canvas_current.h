#ifndef CANVAS_CURRENT_H
#define CANVAS_CURRENT_H

#include "ceammc_object.h"

using namespace ceammc;

class CanvasCurrent : public BaseObject {
public:
    CanvasCurrent(const PdArgs& a);

    void onBang() override;
    AtomList p_size() const;
};

void setup_canvas_current();

#endif // CANVAS_CURRENT_H
