/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "conv_color2rgba.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

CEAMMC_DEFINE_SYM_HASH(int);
CEAMMC_DEFINE_SYM_HASH(float);
CEAMMC_DEFINE_SYM_HASH(pd);

ConvColor2RGBA::ConvColor2RGBA(const PdArgs& args)
    : BaseObject(args)
    , mode_(nullptr)
{
    createOutlet();
    createOutlet();
    createOutlet();
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { str_int, str_float, str_pd });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@int", mode_, sym_int()));
    addProperty(new SymbolEnumAlias("@float", mode_, sym_float()));
}

void ConvColor2RGBA::onSymbol(t_symbol* s)
{
    parser_.reset();
    if (!parser_.parse(s->s_name)) {
        OBJ_ERR << fmt::format("can't parse color: '{}'", s->s_name);
        return;
    }

    switch (crc32_hash(mode_->value())) {
    case hash_int:
        floatTo(3, parser_.alpha());
        floatTo(2, parser_.blue());
        floatTo(1, parser_.green());
        floatTo(0, parser_.red());
        break;
    case hash_float:
        floatTo(3, parser_.norm_alpha());
        floatTo(2, parser_.norm_blue());
        floatTo(1, parser_.norm_green());
        floatTo(0, parser_.norm_red());
        break;
    case hash_pd:
        //        floatTo(0, parser_.asPdColor());
        break;
    default:
        OBJ_ERR << fmt::format("invalid mode: '{}'", mode_->value()->s_name);
        break;
    }
}

void ConvColor2RGBA::onAny(t_symbol* s, const AtomListView&)
{
    onSymbol(s);
}

void setup_conv_color2rgba()
{
    ObjectFactory<ConvColor2RGBA> obj("conv.color2rgba");
    obj.addAlias("color->rgba");

    obj.setXletsInfo({ "symbol: named or hex RGBA color" }, { "red", "green", "blue", "alpha" });
    obj.setCategory("conv");
    obj.setKeywords({ "color", "rgb", "rgba", "hex", "hsv" });
    obj.setSinceVersion(0, 9);
    obj.setDescription("convert named or hex RGB/RGBA colors to separate RGBA values");
}
