#ifndef CANVAS_TOP_H
#define CANVAS_TOP_H

#include "ceammc_object.h"

using namespace ceammc;

class CanvasTop : public BaseObject {
public:
    CanvasTop(const PdArgs& a);

    AtomList p_name() const;
    AtomList p_dir() const;
    AtomList p_font() const;
    AtomList p_paths() const;
    AtomList p_size() const;
    AtomList p_x() const;
    AtomList p_y() const;
    AtomList p_width() const;
    AtomList p_height() const;
};

void setup_canvas_top();

#endif // CANVAS_TOP_H
