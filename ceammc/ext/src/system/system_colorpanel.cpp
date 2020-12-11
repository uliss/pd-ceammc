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
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "system_colorpanel.tcl.h"

#include <cinttypes>

static t_symbol* SYM_FLOAT;
static t_symbol* SYM_INT;
static t_symbol* SYM_HEX;

SystemColorpanel::SystemColorpanel(const PdArgs& args)
    : BaseObject(args)
    , r_(255)
    , g_(255)
    , b_(255)
    , hex_ { "#ffffff" }
    , mode_(nullptr)
{
    char buf[MAXPDSTRING];
    sprintf(buf, "#%" PRIxPTR, reinterpret_cast<uintptr_t>(this));
    bindReceive(gensym(buf));

    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { SYM_FLOAT, SYM_INT, SYM_HEX });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@f", mode_, SYM_FLOAT));
    addProperty(new SymbolEnumAlias("@i", mode_, SYM_INT));
    addProperty(new SymbolEnumAlias("@h", mode_, SYM_HEX));

    createCbProperty("@float", &SystemColorpanel::propFloat, &SystemColorpanel::propSetFloat);
    createCbProperty("@int", &SystemColorpanel::propInt, &SystemColorpanel::propSetInt);

    createCbSymbolProperty(
        "@hex",
        [this]() -> t_symbol* { return gensym(hex_); },
        [this](t_symbol* s) -> bool { return setHex(s); });
}

void SystemColorpanel::onBang()
{
    sys_vgui("after idle [list after 100 ::ceammc::colorpanel::open %s %s]\n",
        receive()->s_name, hex_);
}

void SystemColorpanel::onSymbol(t_symbol* s)
{
    if (setHex(s))
        onBang();
}

void SystemColorpanel::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    if (shift)
        onBang();
}

void SystemColorpanel::m_callback(t_symbol* s, const AtomListView& args)
{
    if (!args.isSymbol())
        return;

    t_symbol* color = atomlistToValue<t_symbol*>(args, &s_);

    if (color != &s_) {
        if (!setHex(color))
            return;

        if (mode_->value() == SYM_FLOAT)
            listTo(0, propFloat());
        else if (mode_->value() == SYM_INT)
            listTo(0, propInt());
        else if (mode_->value() == SYM_HEX)
            symbolTo(0, gensym(hex_));
        else
            OBJ_ERR << "unknown output mode: " << mode_->value();
    }
}

bool SystemColorpanel::setHex(t_symbol* s)
{
    unsigned int red, green, blue;
    auto n = sscanf(s->s_name, "#%02x%02x%02x", &red, &green, &blue);
    if (n != 3) {
        OBJ_ERR << "invalid color value: " << s;
        return false;
    }

    r_ = red;
    g_ = green;
    b_ = blue;

    snprintf(hex_, sizeof(hex_), "#%02x%02x%02x", r_, g_, b_);

    return true;
}

void SystemColorpanel::propSetFloat(const AtomList& v)
{
    if (!checkArgs(v, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT)) {
        OBJ_ERR << "list of RGB float values expected in 0-1 range: " << v;
        return;
    }

    r_ = convert::lin2lin_clip<t_float, 0, 1>(v[0].asFloat(), 0, 255);
    g_ = convert::lin2lin_clip<t_float, 0, 1>(v[1].asFloat(), 0, 255);
    b_ = convert::lin2lin_clip<t_float, 0, 1>(v[2].asFloat(), 0, 255);
}

void SystemColorpanel::propSetInt(const AtomList& v)
{
    if (!(checkArgs(v, ARG_INT, ARG_INT, ARG_INT)
            && v.allOf([](const Atom& a) { return a.isFloat() && a.asFloat() >= 0 && a.asFloat() <= 255; }))) {
        OBJ_ERR << "list of RGB int values expected in 0-255 range: " << v;
        return;
    }

    r_ = v[0].asFloat();
    g_ = v[1].asFloat();
    b_ = v[2].asFloat();
}

void setup_system_colorpanel()
{
    SYM_FLOAT = &s_float;
    SYM_INT = gensym("int");
    SYM_HEX = gensym("hex");

    sys_gui(system_colorpanel_tcl);

    ObjectFactory<SystemColorpanel> obj("system.colorpanel");

    obj.useClick();
    obj.addMethod(".callback", &SystemColorpanel::m_callback);
}
