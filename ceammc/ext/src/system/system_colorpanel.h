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

#include "ceammc_object.h"
#include "ceammc_property_data.h"
#include "ceammc_property_enum.h"
#include "ceammc_tcl.h"
#include "datatype_color.h"
using namespace ceammc;

using ColorProperty = DataPropertyT<DataTypeColor>;

class SystemColorpanel : public BaseTclObject<> {
    ColorProperty* color_ { nullptr };
    SymbolEnumProperty* mode_ { nullptr };

public:
    SystemColorpanel(const PdArgs& args);

    void onBang() final;
    void onSymbol(t_symbol* s) final;
    void onDataT(const DataAtom<DataTypeColor>& c);
    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override;

    void onTclResponse(t_symbol* s, const AtomListView& args) final;
};

void setup_system_colorpanel();

#endif // SYSTEM_COLORPANEL_H
