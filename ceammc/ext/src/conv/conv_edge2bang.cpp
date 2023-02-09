/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "conv_edge2bang.h"
#include "ceammc_factory.h"

ConvEdgeToBang::ConvEdgeToBang(const PdArgs& args)
    : BaseObject(args)
    , mode_(nullptr)
{
    mode_ = new SymbolEnumProperty("@mode", { "up", "down", "both" });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@up", mode_, gensym("up")));
    addProperty(new SymbolEnumAlias("@down", mode_, gensym("down")));
    addProperty(new SymbolEnumAlias("@both", mode_, gensym("both")));

    createOutlet();
}

void ConvEdgeToBang::onFloat(t_float f)
{
    const auto diff = f - val_;

    switch (mode_->value()->s_name[0]) {
    case 'u':
        if (diff > 0)
            bangTo(0);
        break;
    case 'd':
        if (diff < 0)
            bangTo(0);

        break;
    case 'b': {
        if (diff > 0 && both_mode_dir_ <= 0) {
            bangTo(0);
            both_mode_dir_ = 1;
        } else if (diff < 0 && both_mode_dir_ >= 0) {
            bangTo(0);
            both_mode_dir_ = -1;
        }

    } break;
    default:
        return;
    }

    val_ = f;
}

void setup_conv_edge2bang()
{
    ObjectFactory<ConvEdgeToBang> obj("conv.edge2bang");
    obj.addAlias("edge->bang");

    obj.setDescription("edge detector for numeric stream");
    obj.setCategory("conv");
    obj.setKeywords({"conv", "edge", "detector"});
}
