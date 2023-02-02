#ifndef CANVAS_ACTIVE_H
#define CANVAS_ACTIVE_H

#include "ceammc_object.h"
using namespace ceammc;

class CanvasActive : public BaseObject {
    BoolProperty* abstractions_;

public:
    CanvasActive(const PdArgs& args);
    void onBang() final;

    void m_active(t_symbol* s, const AtomListView& lv);
    t_canvas* currentCanvas();
};

void setup_base_canvas_active();

#endif // CANVAS_ACTIVE_H
