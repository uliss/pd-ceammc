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
CEAMMC_DEFINE_SYM_HASH(shorter);
CEAMMC_DEFINE_SYM_HASH(longer);
CEAMMC_DEFINE_SYM_HASH(increasing);
CEAMMC_DEFINE_SYM_HASH(decreasing);

using DefaultColorScale = colorm::ColorScale<colorm::Oklab>;

struct ColorScaleImpl {
    DefaultColorScale scale;
};

ColorScale::ColorScale(const PdArgs& args)
    : BaseObject(args)
    , scale_(nullptr)
    , mode_(nullptr)
    , interp_(nullptr)
    , impl_(new ColorScaleImpl)
{
    scale_ = new DataPropertyListT<DataTypeColor>("@scale", {});
    scale_->setArgIndex(0);
    scale_->setSuccessFn([this](Property* p) {
        if (!impl_)
            return;

        std::vector<colorm::Oklab> colors;
        for (auto& c : scale_->value())
            colors.push_back(colorm::Oklab(colorm::Srgb(c.red(), c.green(), c.blue(), c.alpha())));

        switch (crc32_hash(interp_->value())) {
        case hash_none:
            impl_->scale = DefaultColorScale(colors);
            break;
        case hash_shorter:
            impl_->scale = DefaultColorScale(colors).shorter();
            break;
        case hash_longer:
            impl_->scale = DefaultColorScale(colors).longer();
            break;
        case hash_increasing:
            impl_->scale = DefaultColorScale(colors).increasing();
            break;
        case hash_decreasing:
            impl_->scale = DefaultColorScale(colors).decreasing();
            break;
        }
    });
    addProperty(scale_);

    mode_ = new SymbolEnumProperty("@mode", { sym_color(), sym_hex(), sym_rgb() });
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@color", mode_, sym_color()));
    addProperty(new SymbolEnumAlias("@hex", mode_, sym_hex()));
    addProperty(new SymbolEnumAlias("@rgb", mode_, sym_rgb()));

    interp_ = new SymbolEnumProperty("@interp", { str_none, str_shorter, str_longer, str_increasing, str_decreasing });
    addProperty(interp_);

    createOutlet();
}

ColorScale::~ColorScale() = default;

void ColorScale::onFloat(t_float f)
{
    if (!impl_)
        return;

    auto color = colorm::Srgb(impl_->scale(f));

    switch (crc32_hash(mode_->value())) {
    case hash_color:
        break;
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
}
