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
#include "color_scale.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "colorm/colorm.h"
#include "fmt/core.h"

CEAMMC_DEFINE_SYM_HASH(color);
CEAMMC_DEFINE_SYM_HASH(hex);
CEAMMC_DEFINE_SYM_HASH(rgb);
CEAMMC_DEFINE_SYM_HASH(none);
CEAMMC_DEFINE_SYM_HASH(short);
CEAMMC_DEFINE_SYM_HASH(long);
CEAMMC_DEFINE_SYM_HASH(inc);
CEAMMC_DEFINE_SYM_HASH(dec);

using ColorT = colorm::Oklch;
using ColorListV = std::vector<ColorT>;
using ColorScaleT = colorm::ColorScale<ColorT>;

struct ColorScaleImpl {
    ColorScaleT scale;
    ColorT color(float f) const { return scale(f); }

    void setColors(const std::vector<DataTypeColor>& cl, t_symbol* mode)
    {
        ColorListV colors;
        colors.reserve(cl.size());
        for (auto& c : cl) {
            colorm::Srgb rgb(c.red(), c.green(), c.blue(), c.alpha());
            colors.push_back(ColorT(rgb));
        }

        switch (crc32_hash(mode)) {
        case hash_short:
            scale = ColorScaleT(colors).shorter();
            break;
        case hash_long:
            scale = ColorScaleT(colors).longer();
            break;
        case hash_inc:
            scale = ColorScaleT(colors).increasing();
            break;
        case hash_dec:
            scale = ColorScaleT(colors).decreasing();
            break;
        case hash_none:
        default:
            scale = ColorScaleT(colors);
            break;
        }
    }
};

ColorScale::ColorScale(const PdArgs& args)
    : BaseObject(args)
    , colors_(nullptr)
    , mode_(nullptr)
    , interp_(nullptr)
    , impl_(new ColorScaleImpl)
{
    colors_ = new ColorListProperty("@scale", {});
    colors_->setArgIndex(0);
    colors_->setSuccessFn([this](Property* p) {
        if (!impl_)
            return;

        impl_->setColors(colors_->value(), interp_->value());
    });
    addProperty(colors_);

    mode_ = new SymbolEnumProperty("@mode", { sym_color(), sym_hex(), sym_rgb() });
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@color", mode_, sym_color()));
    addProperty(new SymbolEnumAlias("@hex", mode_, sym_hex()));
    addProperty(new SymbolEnumAlias("@rgb", mode_, sym_rgb()));

    interp_ = new SymbolEnumProperty("@interp", { str_none, str_short, str_long, str_inc, str_dec });
    interp_->setSuccessFn([this](Property*) {
        if (impl_)
            impl_->setColors(colors_->value(), interp_->value());
    });
    addProperty(interp_);

    createOutlet();
}

ColorScale::~ColorScale() = default;

void ColorScale::onFloat(t_float f)
{
    if (!impl_) {
        OBJ_ERR << "null impl";
        return;
    }

    auto color = colorm::Srgb(impl_->color(f));

    switch (crc32_hash(mode_->value())) {
    case hash_color: {
        atomTo(0, ColorAtom(DataTypeColor().setRgbf(color.red(), color.green(), color.blue())));
    } break;
    case hash_hex: {
        colorm::Rgb rgb(color);

        if (rgb.alpha8() == 0xFF) {
            char buf[8];
            fmt::format_to(buf, "#{:02X}{:02X}{:02X}\0", rgb.red8(), rgb.green8(), rgb.blue8());
            symbolTo(0, gensym(buf));
        } else {
            char buf[10];
            fmt::format_to(buf, "#{:02X}{:02X}{:02X}{:02X}\0", rgb.red8(), rgb.green8(), rgb.blue8(), rgb.alpha8());
            symbolTo(0, gensym(buf));
        }

    } break;
    case hash_rgb: {
        StaticAtomList<4> data {
            color.red(),
            color.green(),
            color.blue(),
            color.alpha()
        };
        listTo(0, data.view());
    } break;
    }
}

void setup_color_scale()
{
    ObjectFactory<ColorScale> obj("color.scale");
    obj.setXletsInfo({ "float\\[0..1\\]" }, { "data:color, symbol or list R G B A" });
}
