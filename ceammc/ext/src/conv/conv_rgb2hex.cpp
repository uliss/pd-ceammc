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
#include "conv_rgb2hex.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/core.h"

CEAMMC_DEFINE_SYM_HASH(int);
CEAMMC_DEFINE_SYM_HASH(float);

ConvRgb2Hex::ConvRgb2Hex(const PdArgs& args)
    : BaseObject(args)
    , sync_(nullptr)
    , mode_(nullptr)
{
    createInlet();
    createInlet();
    createOutlet();

    sync_ = new FlagProperty("@sync");
    addProperty(sync_);

    mode_ = new SymbolEnumProperty("@mode", { str_int, str_float });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@int", mode_, sym_int()));
    addProperty(new SymbolEnumAlias("@float", mode_, sym_float()));
}

void ConvRgb2Hex::onFloat(t_float f)
{
    switch (crc32_hash(mode_->value())) {
    case hash_int:
        r_ = clip<t_float, 0, 255>(std::round(f));
        outputInt();
        break;
    case hash_float:
        r_ = clip01(f);
        outputFloat();
        break;
    default:
        OBJ_ERR << fmt::format("invalid mode: '{}'", mode_->value()->s_name);
        return;
    }
}

void ConvRgb2Hex::onList(const AtomListView& lv)
{
    if (lv.size() == 0 || lv.size() > 3) {
        OBJ_ERR << fmt::format("R G B list expected, got: {}", to_string(lv));
        return;
    }

    switch (crc32_hash(mode_->value())) {
    case hash_int:

        r_ = clip<t_float, 0, 255>(std::round(lv.floatAt(0, 0)));
        if (lv.size() > 1)
            g_ = clip<t_float, 0, 255>(std::round(lv.floatAt(1, 0)));
        if (lv.size() > 2)
            b_ = clip<t_float, 0, 255>(std::round(lv.floatAt(2, 0)));

        outputInt();
        break;
    case hash_float:

        r_ = clip01(lv.floatAt(0, 0));
        if (lv.size() > 1)
            g_ = clip01(lv.floatAt(1, 0));
        if (lv.size() > 2)
            b_ = clip01(lv.floatAt(2, 0));

        outputFloat();
        break;
    default:
        OBJ_ERR << fmt::format("invalid mode: '{}'", mode_->value()->s_name);
        return;
    }
}

void ConvRgb2Hex::onInlet(size_t n, const AtomListView& lv)
{
    if (!lv.isFloat()) {
        OBJ_ERR << fmt::format("float or int value expected, got: {}", to_string(lv));
        return;
    }

    switch (crc32_hash(mode_->value())) {
    case hash_int:

        if (n == 1)
            g_ = clip<t_float, 0, 255>(std::round(lv.floatAt(0, 0)));
        else if (n == 2)
            b_ = clip<t_float, 0, 255>(std::round(lv.floatAt(0, 0)));

        if (sync_->value())
            outputInt();

        break;
    case hash_float:
        if (n == 1)
            g_ = clip01(lv.floatAt(0, 0));
        else if (n == 2)
            b_ = clip01(lv.floatAt(0, 0));

        if (sync_->value())
            outputFloat();

        break;
    default:
        OBJ_ERR << fmt::format("invalid mode: '{}'", mode_->value()->s_name);
        return;
    }
}

void ConvRgb2Hex::outputFloat()
{
    std::uint8_t R = clip<int, 0, 255>(std::round(r_ * 255));
    std::uint8_t G = clip<int, 0, 255>(std::round(g_ * 255));
    std::uint8_t B = clip<int, 0, 255>(std::round(b_ * 255));

    outputRGB(R, G, B);
}

void ConvRgb2Hex::outputRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    char buf[8];
    fmt::format_to(buf, "#{:02X}{:02X}{:02X}", r, g, b);
    symbolTo(0, gensym(buf));
}

void ConvRgb2Hex::outputInt()
{
    outputRGB(clip<int, 0, 255>(r_), clip<int, 0, 255>(g_), clip<int, 0, 255>(b_));
}

void setup_conv_rgb2hex()
{
    ObjectFactory<ConvRgb2Hex> obj("conv.rgb2hex");
    obj.addAlias("rgb->hex");
    obj.setXletsInfo({ "int or float: red", "int or float: green", "int or float: blue" }, { "symbol: #RRGGBB" });
}
