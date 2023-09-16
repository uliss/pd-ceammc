#ifndef CANVAS_TOP_H
#define CANVAS_TOP_H

#include "ceammc_object.h"

using namespace ceammc;

class CanvasTop : public BaseObject {
    BoolProperty* abs_ { nullptr };

public:
    CanvasTop(const PdArgs& a);
    void onBang() override;

    void m_postscript(t_symbol* s, const AtomListView& lv);

private:
    const t_canvas* topCanvas() const;
};

void setup_canvas_top();

#endif // CANVAS_TOP_H
