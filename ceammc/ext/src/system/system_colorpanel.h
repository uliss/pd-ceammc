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
#ifndef SYSTEM_COLORPANEL_H
#define SYSTEM_COLORPANEL_H

#include <cstdint>

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class SystemColorpanel : public BaseObject {
    uint8_t r_, g_, b_;
    char hex_[8];

    SymbolEnumProperty* mode_;

public:
    SystemColorpanel(const PdArgs& args);

    void onBang() override;
    void onSymbol(t_symbol* s) override;
    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override;

    void m_callback(t_symbol* s, const AtomListView& args);

private:
    bool setHex(t_symbol* s);
    AtomList propInt() const { return AtomList({ t_float(r_), t_float(g_), t_float(b_) }); }
    AtomList propFloat() const { return AtomList({ r_ / t_float(255), g_ / t_float(255), b_ / t_float(255) }); }

    void propSetFloat(const AtomList& v);
    void propSetInt(const AtomList& v);
};

void setup_system_colorpanel();

#endif // SYSTEM_COLORPANEL_H
