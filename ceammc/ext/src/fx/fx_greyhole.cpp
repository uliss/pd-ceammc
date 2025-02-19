/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "fx_greyhole.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxGreyhole : public faust_fx_greyhole_tilde {
public:
    FxGreyhole(const PdArgs& args)
        : faust_fx_greyhole_tilde(args)
    {
    }
};

void setup_fx_greyhole_tilde()
{
    FaustFactory<FxGreyhole> obj("fx.greyhole~");

    obj.setDescription("A complex echo-like effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "reverb", "echo" });
}
