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
    static inline std::uintptr_t ptr_to_uint(const void* x)
    {
#ifdef __WIN32__
        using CanvasIntPtr = std::uint32_t;
#else
        using CanvasIntPtr = std::uint64_t;
#endif
        return reinterpret_cast<std::uintptr_t>(x) & std::numeric_limits<CanvasIntPtr>::max();
    }
};

void setup_base_canvas_active();

#endif // CANVAS_ACTIVE_H
