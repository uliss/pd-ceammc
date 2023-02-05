#ifndef CANVAS_ACTIVE_H
#define CANVAS_ACTIVE_H

#include "ceammc_object.h"

#include <cstdint>

using namespace ceammc;

class CanvasActive : public BaseObject {
    BoolProperty* abstractions_;

public:
    CanvasActive(const PdArgs& args);
    void onBang() final;

    void m_active(t_symbol* s, const AtomListView& lv);
    t_canvas* currentCanvas();

public:
    static inline std::uint32_t ptr_to_uint(const void* x)
    {
        return reinterpret_cast<std::uintptr_t>(x) & std::numeric_limits<std::uint32_t>::max();
    }
};

void setup_base_canvas_active();

#endif // CANVAS_ACTIVE_H
