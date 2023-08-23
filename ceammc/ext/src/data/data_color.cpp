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
#include "data_color.h"
#include "args/argcheck2.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

DataColor::DataColor(const PdArgs& args)
    : BaseTclObject(args, gensym("::ceammc::colorpanel::open"))
    , color_(nullptr)
{
    color_ = new DataPropertyT<DataTypeColor>("@value", DataTypeColor());
    color_->setArgIndex(0);
    addProperty(color_);

    createInlet();
    createOutlet();
}

void DataColor::onBang()
{
    atomTo(0, color_->asDataAtom());
}

void DataColor::onDataT(const ColorAtom& a)
{
    color_->setValue(*a);
    atomTo(0, a);
}

void DataColor::onInlet(size_t n, const AtomListView& lv)
{
    if (lv.empty()) {
        OBJ_ERR << "Color datatype expected";
        return;
    }

    color_->setAtom(lv.front());
}

void DataColor::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    if (shift)
        tclCall(color_->value().hex());
}

void DataColor::onTclResponse(t_symbol* s, const AtomListView& lv)
{
    if (!lv.isSymbol())
        return;

    auto color = lv.symbolAt(0, &s_);

    if (color != &s_) {
        color_->value().setHex(color->s_name);
        atomTo(0, color_->asDataAtom());
    }
}

void DataColor::m_brighten(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("f[-1,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().brighten(lv.floatAt(0, 0));
}

void DataColor::m_darken(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("f[-1,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().darken(lv.floatAt(0, 0));
}

void DataColor::m_grayscale(t_symbol* s, const AtomListView&)
{
    color_->value().grayscale();
}

void DataColor::m_saturate(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("AMOUNT:f[-1,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().saturate(lv.floatAt(0, 0));
}

void DataColor::m_desaturate(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("AMOUNT:f[-1,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().desaturate(lv.floatAt(0, 0));
}

void DataColor::m_invert(t_symbol* s, const AtomListView&)
{
    color_->value().invert();
}

void DataColor::m_rotate(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ANGLE:f[0,360]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().rotate(lv.floatAt(0, 0));
}

void DataColor::m_flip(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("f[0,360]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().flip(lv.floatAt(0, 0));
}

void DataColor::m_red(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("R:f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setRed(lv.floatAt(0, 0));
}

void DataColor::m_green(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("G:f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setGreen(lv.floatAt(0, 0));
}

void DataColor::m_blue(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("B:f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setBlue(lv.floatAt(0, 0));
}

void DataColor::m_alpha(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("A:f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setAlpha(lv.floatAt(0, 0));
}

void DataColor::m_hex(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("HEX:s");
    if (!chk.check(lv, this))
        return chk.usage(this);

    auto str = lv[0].asSymbol()->s_name;

    if (!color_->value().setHex(str)) {
        OBJ_ERR << fmt::format("can't parse color value: '{}'", str);
        return;
    }
}

void DataColor::m_rgb8(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("R:i[0,255] G:i[0,255] B:i[0,255] A:i[0,255]?");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setRgb8(lv.intAt(0, 0), lv.intAt(1, 0), lv.intAt(2, 0), lv.intAt(3, 255));
}

void DataColor::m_rgb(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("R:f[0,1] G:f[0,1] B:f[0,1] A:f[0,1]?");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setRgbf(lv.floatAt(0, 0), lv.floatAt(1, 0), lv.floatAt(2, 0), lv.floatAt(3, 1));
}

void DataColor::m_hsl(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("H:f[0,360] S:f[0,1] L:f[0,1] A:f[0,1]?");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setHsl(lv.floatAt(0, 0), lv.floatAt(1, 0), lv.floatAt(2, 0), lv.floatAt(3, 1));
}

void DataColor::m_hwb(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("H:f[0,360] W:f[0,1] B:f[0,1] A:f[0,1]?");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setHwb(lv.floatAt(0, 0), lv.floatAt(1, 0), lv.floatAt(2, 0), lv.floatAt(3, 1));
}

void DataColor::m_oklab(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("L:f[0,1] A:f[-1,1] B:f[-1,1] A:f[0,1]?");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_->value().setOkLab(lv.floatAt(0, 0), lv.floatAt(1, 0), lv.floatAt(2, 0), lv.floatAt(3, 1));
}

void DataColor::m_set(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "Color datatype expected";
        return;
    }

    color_->setAtom(lv.front());
}

void DataColor::m_mix(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("COLOR:a MIX:f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    DataTypeColor mix_color;
    if (DataTypeColor::parseFromList(lv.subView(0, 1), mix_color)) {
        auto new_color = color_->value().mix(mix_color, lv.floatAt(1, 0));
        color_->setValue(new_color);
    } else {
        METHOD_ERR(s) << "can't parse color: " << lv;
    }
}

void setup_data_color()
{
    ObjectFactory<DataColor> obj("data.color");
    obj.addAlias("color");
    obj.processData<DataTypeColor>();
    obj.setXletsInfo({ "data:Color\n"
                       "hex, rgb, rgb8 ...",
                         "data:Color" },
        { "data:Color" });

    obj.addMethod("brighten", &DataColor::m_brighten);
    obj.addMethod("darken", &DataColor::m_darken);
    obj.addMethod("grayscale", &DataColor::m_grayscale);
    obj.addMethod("saturate", &DataColor::m_saturate);
    obj.addMethod("desaturate", &DataColor::m_desaturate);
    obj.addMethod("rotate", &DataColor::m_rotate);
    obj.addMethod("invert", &DataColor::m_invert);
    obj.addMethod("flip", &DataColor::m_flip);

    obj.addMethod("hex", &DataColor::m_hex);
    obj.addMethod("rgb8", &DataColor::m_rgb8);
    obj.addMethod("rgb", &DataColor::m_rgb);
    obj.addMethod("hsl", &DataColor::m_hsl);
    obj.addMethod("hwb", &DataColor::m_hwb);
    obj.addMethod("oklab", &DataColor::m_oklab);

    obj.addMethod("red", &DataColor::m_red);
    obj.addMethod("green", &DataColor::m_green);
    obj.addMethod("blue", &DataColor::m_blue);
    obj.addMethod("alpha", &DataColor::m_alpha);

    obj.addMethod("set", &DataColor::m_set);
    obj.addMethod("mix", &DataColor::m_mix);

    obj.useClick();
    DataColor::initMethods(obj);
}
