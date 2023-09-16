/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "fx_dattorro.h"

#include "ceammc_factory.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxDattorro : public faust_fx_dattorro_tilde {
public:
    FxDattorro(const PdArgs& args)
        : faust_fx_dattorro_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@decay"), gensym("@drywet") });
    }
};

void setup_fx_dattorro_tilde()
{
    FaustFactory<FxDattorro> obj("fx.dattorro~");

    obj.setDescription("dattorro reverb");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "reverb" });
}
