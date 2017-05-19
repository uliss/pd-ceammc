/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef WINDOW_H
#define WINDOW_H

#include "ceammc_object.h"

using namespace ceammc;

typedef float (*WindowFuncPtr)(size_t, size_t);

class Window : public BaseObject {
    SizeTProperty* size_;
    t_symbol* type_;
    WindowFuncPtr fn_;

public:
    Window(const PdArgs& a);
    void onFloat(float v);
    void onList(const AtomList& l);
    void setWindowFunc(t_symbol* name);
    WindowFuncPtr windowFunc();

    void m_hann(t_symbol*, const AtomList&);
    void m_tri(t_symbol*, const AtomList&);
    void m_welch(t_symbol*, const AtomList&);

private:
    void pTypeSet(const AtomList& l);
    AtomList pTypeGet() const;
};

extern "C" void window_setup();

#endif // WINDOW_H
