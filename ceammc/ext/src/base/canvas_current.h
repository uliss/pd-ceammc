#ifndef CANVAS_CURRENT_H
#define CANVAS_CURRENT_H

#include "ceammc_object.h"

using namespace ceammc;

class CanvasCurrent : public BaseObject {
public:
    CanvasCurrent(const PdArgs& a);

    AtomList p_name() const;
    AtomList p_dir() const;
    AtomList p_root() const;
    AtomList p_abstraction() const;
    AtomList p_args() const;
    AtomList p_font() const;
    AtomList p_paths() const;
    AtomList p_size() const;
    AtomList p_width() const;
    AtomList p_height() const;
    AtomList p_x() const;
    AtomList p_y() const;
};

void setup_canvas_current();

#endif // CANVAS_CURRENT_H
