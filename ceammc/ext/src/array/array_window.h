/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef ARRAY_WINDOW_H
#define ARRAY_WINDOW_H

#include "array_base.h"

typedef float (*WindowFuncPtr)(size_t, size_t);

class ArrayWindow : public ArrayMod {
    SymbolEnumProperty* type_;
    WindowFuncPtr gen_fn_;

public:
    ArrayWindow(const PdArgs& args);

    void onBang() override;
    void onAny(t_symbol* s, const AtomList& lst) override;

    void updateGenFn();

    void m_resize(t_symbol* s, const AtomList& l);
    void m_fit(t_symbol* s, const AtomList& l);

private:
    void fill();
};

void setup_array_window();

#endif // ARRAY_WINDOW_H
