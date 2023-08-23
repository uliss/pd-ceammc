/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "system_colorpanel.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"
#include "system_colorpanel.tcl.h"

#include <cinttypes>

CEAMMC_DEFINE_SYM_HASH(float)
CEAMMC_DEFINE_SYM_HASH(int)
CEAMMC_DEFINE_SYM_HASH(hex)
CEAMMC_DEFINE_SYM_HASH(data)

SystemColorpanel::SystemColorpanel(const PdArgs& args)
    : BaseTclObject(args, gensym("::ceammc::colorpanel::open"))
{
    createOutlet();

    color_ = new ColorProperty("@color", DataTypeColor {});
    addProperty(color_);

    mode_ = new SymbolEnumProperty("@mode", { sym_float(), sym_int(), sym_hex(), sym_data() });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@f", mode_, sym_float()));
    addProperty(new SymbolEnumAlias("@i", mode_, sym_int()));
    addProperty(new SymbolEnumAlias("@h", mode_, sym_hex()));
    addProperty(new SymbolEnumAlias("@d", mode_, sym_data()));

    createCbListProperty(
        "@float",
        [this]() -> AtomList {
            return color_->value().asRgbFList().view();
        },
        [this](const AtomListView& lv) -> bool {
            if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT))
                return false;

            color_->value().setRgbf(lv.floatAt(0, 0), lv.floatAt(1, 0), lv.floatAt(2, 0));
            return true;
        });

    createCbListProperty(
        "@int",
        [this]() -> AtomList {
            return color_->value().asRgb8List().view();
        },
        [this](const AtomListView& lv) -> bool {
            if (!(checkArgs(lv, ARG_INT, ARG_INT, ARG_INT)
                    && lv.allOf([](const Atom& a) { return a.isFloat() && a.asFloat() >= 0 && a.asFloat() <= 255; }))) {
                OBJ_ERR << "list of RGB int values expected in 0-255 range: " << lv;
                return false;
            }

            color_->value().setRgb8(lv.intAt(0, 0), lv.intAt(1, 0), lv.intAt(2, 0));
            return true;
        });

    createCbSymbolProperty(
        "@hex",
        [this]() -> t_symbol* { return color_->value().hex(); },
        [this](t_symbol* s) -> bool { return color_->value().setHex(s->s_name); });
}

void SystemColorpanel::onBang()
{
    tclCall(color_->value().hex());
}

void SystemColorpanel::onSymbol(t_symbol* s)
{
    DataTypeColor c;
    if (!DataTypeColor::parseFromList({ s }, c)) {
        OBJ_ERR << fmt::format("invalid color: {}", s->s_name);
        return;
    }

    color_->setValue(c);
}

void SystemColorpanel::onDataT(const DataAtom<DataTypeColor>& c)
{
    color_->setAtom(c);
}

void SystemColorpanel::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    if (shift)
        onBang();
}

void SystemColorpanel::onTclResponse(t_symbol* s, const AtomListView& args)
{
    if (!args.isSymbol())
        return;

    auto color = atomlistToValue<t_symbol*>(args, &s_);

    if (color != &s_) {
        if (!color_->value().setHex(color->s_name))
            return;

        switch (crc32_hash(mode_->value())) {
        case hash_float:
            return listTo(0, color_->value().asRgbFList().view());
        case hash_int:
            return listTo(0, color_->value().asRgb8List().view());
        case hash_hex:
            return symbolTo(0, color_->value().hex());
        case hash_data:
            return atomTo(0, color_->asDataAtom());
        default:
            OBJ_ERR << "unknown output mode: " << mode_->value();
            break;
        }
    }
}

void setup_system_colorpanel()
{
    ObjectFactory<SystemColorpanel> obj("system.colorpanel");
    obj.processData<DataTypeColor>();

    obj.useClick();
    SystemColorpanel::initMethods(obj, system_colorpanel_tcl);
}
