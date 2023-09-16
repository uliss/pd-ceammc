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
#include "conv_hsl2rgb.h"
#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "colorm/colorm.h"

CEAMMC_DEFINE_SYM_HASH(int);
CEAMMC_DEFINE_SYM_HASH(float);

ConvHsl2Rgb::ConvHsl2Rgb(const PdArgs& args)
    : BaseObject(args)
    , h_(0)
    , s_(0)
    , l_(0)
{

    mode_ = new SymbolEnumProperty("@mode", { str_int, str_float });
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@int", mode_, sym_int()));
    addProperty(new SymbolEnumAlias("@float", mode_, sym_float()));

    sync_ = new FlagProperty("@sync");
    addProperty(sync_);

    pack_ = new FlagProperty("@pack");
    addProperty(pack_);
}

void ConvHsl2Rgb::initDone()
{
    createInlet();
    createInlet();

    if (pack_->value()) {
        createOutlet();
    } else {
        createOutlet();
        createOutlet();
        createOutlet();
    }
}

void ConvHsl2Rgb::onBang()
{
    outputRGB();
}

void ConvHsl2Rgb::onFloat(t_float f)
{
    h_ = clip<t_float, 0, 360>(f);
    outputRGB();
}

void ConvHsl2Rgb::onList(const AtomListView& lv)
{
    static const args::ArgChecker chk("H:f[0,360] S:f[0,1] L:f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    h_ = lv.floatAt(0, 0);
    s_ = lv.floatAt(1, 0);
    l_ = lv.floatAt(2, 0);

    outputRGB();
}

void ConvHsl2Rgb::onInlet(size_t n, const AtomListView& lv)
{
    if (!lv.isFloat()) {
        OBJ_ERR << "float expected, got: " << lv;
        return;
    }

    if (n == 1) {
        s_ = clip01<t_float>(lv.asT<t_float>());
        if (sync_->value())
            outputRGB();
    } else if (n == 2) {
        l_ = clip01<t_float>(lv.asT<t_float>());
        if (sync_->value())
            outputRGB();
    }
}

const char* ConvHsl2Rgb::annotateOutlet(size_t n) const
{

#define INT_PREFIX "int\\[0-255\\]: "
#define FLOAT_PREFIX "float\\[0-1\\]: "

    switch (crc32_hash(mode_->value())) {
    case hash_float: {
        if (pack_->value())
            return "list\\[0-1\\]: R G B";

        switch (n) {
        case 0:
            return FLOAT_PREFIX "red";
        case 1:
            return FLOAT_PREFIX "green";
        case 2:
        default:
            return FLOAT_PREFIX "blue";
        }
    } break;
    case hash_int: {
        if (pack_->value())
            return "list\\[0-255\\]: R G B";

        switch (n) {
        case 0:
            return INT_PREFIX "red";
        case 1:
            return INT_PREFIX "green";
        case 2:
        default:
            return INT_PREFIX "blue";
        }
    } break;
    default:
        return "";
    }
}

void ConvHsl2Rgb::outputRGB()
{
    colorm::Hsl hsl(h_, s_ * 100, l_ * 100);
    colorm::Rgb rgb(hsl);

    switch (crc32_hash(mode_->value())) {
    case hash_float: {
        if (pack_->value()) {

            AtomArray<3> data;
            data[0] = rgb.red() / 255;
            data[1] = rgb.green() / 255;
            data[2] = rgb.blue() / 255;
            listTo(0, data.view());

        } else {
            floatTo(2, rgb.blue() / 255);
            floatTo(1, rgb.green() / 255);
            floatTo(0, rgb.red() / 255);
        }
    } break;
    case hash_int: {
        if (pack_->value()) {
            AtomArray<3> data;
            data[0] = rgb.red8();
            data[1] = rgb.green8();
            data[2] = rgb.blue8();
            listTo(0, data.view());
        } else {
            floatTo(2, rgb.blue8());
            floatTo(1, rgb.green8());
            floatTo(0, rgb.red8());
        }
    } break;
    }
}

void setup_conv_hsl2rgb()
{
    ObjectFactory<ConvHsl2Rgb> obj("conv.hsl2rgb");
    obj.addAlias("hsl->rgb");

    obj.setXletsInfo({
                         "float\\[0-360\\]: hue",
                         "float\\[0-1\\]: saturation",
                         "float\\[0-1\\]: lightness",
                     },
        {});
}
