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
#include "conv_sec2str.h"
#include "ceammc_convert.h"

using namespace ceammc;

SecToStr::SecToStr(const PdArgs& a)
    : BaseObject(a)
    , ms_flag_(0)
{
    createOutlet();

    ms_flag_ = new FlagProperty("@ms");
    createProperty(ms_flag_);
}

void SecToStr::onFloat(t_float v)
{
    symbolTo(0, gensym(convert::time::sec2str(v, ms_flag_->value()).c_str()));
}

extern "C" void setup_conv0x2esec2str()
{
    ObjectFactory<SecToStr> obj("conv.sec2str");
    obj.addAlias("sec->str");
}
