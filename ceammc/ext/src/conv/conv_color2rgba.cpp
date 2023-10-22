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
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

CEAMMC_DEFINE_SYM_HASH(int);
CEAMMC_DEFINE_SYM_HASH(float);

ConvColor2RGBA::ConvColor2RGBA(const PdArgs& args)
    : BaseObject(args)
{
    mode_ = new SymbolEnumProperty("@mode", { str_int, str_float });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@int", mode_, sym_int()));
    addProperty(new SymbolEnumAlias("@float", mode_, sym_float()));

    pack_ = new FlagProperty("@pack");
    addProperty(pack_);
}

void ConvColor2RGBA::initDone()
{
    if (!pack_->value()) {
        createOutlet();
        createOutlet();
        createOutlet();
        createOutlet();
    } else {
        createOutlet();
    }
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
        if (!pack_->value()) {
            floatTo(3, parser_.alpha());
            floatTo(2, parser_.blue());
            floatTo(1, parser_.green());
            floatTo(0, parser_.red());
        } else {
            AtomArray<4> data;
            data[0] = parser_.red();
            data[1] = parser_.green();
            data[2] = parser_.blue();
            data[3] = parser_.alpha();

            listTo(0, data.view());
        }
        break;
    case hash_float:
        if (!pack_->value()) {
            floatTo(3, parser_.norm_alpha());
            floatTo(2, parser_.norm_blue());
            floatTo(1, parser_.norm_green());
            floatTo(0, parser_.norm_red());
        } else {
            AtomArray<4> data;
            data[0] = parser_.norm_red();
            data[1] = parser_.norm_green();
            data[2] = parser_.norm_blue();
            data[3] = parser_.norm_alpha();

            listTo(0, data.view());
        }
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

void ConvColor2RGBA::onDataT(const DataAtom<DataTypeColor>& data)
{
    auto c = *data;
    switch (crc32_hash(mode_->value())) {
    case hash_int:
        if (!pack_->value()) {
            floatTo(3, c.alpha8());
            floatTo(2, c.blue8());
            floatTo(1, c.green8());
            floatTo(0, c.red8());
        } else {
            AtomArray<4> data;
            data[0] = c.red8();
            data[1] = c.green8();
            data[2] = c.blue8();
            data[3] = c.alpha8();

            listTo(0, data.view());
        }
        break;
    case hash_float:
        if (!pack_->value()) {
            floatTo(3, c.alpha());
            floatTo(2, c.blue());
            floatTo(1, c.green());
            floatTo(0, c.red());
        } else {
            AtomArray<4> data;
            data[0] = c.red();
            data[1] = c.green();
            data[2] = c.blue();
            data[3] = c.alpha();

            listTo(0, data.view());
        }
        break;
    default:
        OBJ_ERR << fmt::format("invalid mode: '{}'", mode_->value()->s_name);
        break;
    }
}

const char* ConvColor2RGBA::annotateOutlet(size_t n) const
{
#define INT_PREFIX "int\\[0-255\\]: "
#define FLOAT_PREFIX "float\\[0-1\\]: "

    switch (crc32_hash(mode_->value())) {
    case hash_int:
        if (pack_->value())
            return "list: R G B A \\[0-255\\]";

        switch (n) {
        case 0:
            return INT_PREFIX "red";
        case 1:
            return INT_PREFIX "green";
        case 2:
            return INT_PREFIX "blue";
        case 3:
        default:
            return INT_PREFIX "alpha";
        }
        break;
    case hash_float:
        if (pack_->value())
            return "list: R G B A \\[0-1\\]";

        switch (n) {
        case 0:
            return FLOAT_PREFIX "red";
        case 1:
            return FLOAT_PREFIX "green";
        case 2:
            return FLOAT_PREFIX "blue";
        case 3:
        default:
            return FLOAT_PREFIX "alpha";
        }
        break;
    default:
        return "?";
    }
}

void setup_conv_color2rgba()
{
    ObjectFactory<ConvColor2RGBA> obj("conv.color2rgba");
    obj.addAlias("color->rgba");
    obj.processData<DataTypeColor>();

    obj.setXletsInfo({ "symbol: named or hex RGBA color" }, { "red", "green", "blue", "alpha" });
    obj.setCategory("conv");
    obj.setKeywords({ "color", "rgb", "rgba", "hex", "hsv" });
    obj.setSinceVersion(0, 9);
    obj.setDescription("convert named or hex RGB/RGBA colors to separate RGBA values");
}
