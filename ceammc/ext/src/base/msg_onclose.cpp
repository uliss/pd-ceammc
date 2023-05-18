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
#include "msg_onclose.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"

#include "g_canvas.h"

constexpr const char* MENUCLOSE = "menuclose";
constexpr const char* ORIG_MENUCLOSE = "menuclose [msg.onclose]";

static void msg_onclose_menuclose(t_canvas* x, t_floatarg f)
{
    canvas_foreach(x, [](t_gobj* x, const t_class* c) {
        auto obj = ObjectFactory<MsgOnClose>::objectCast(x);
        if (obj)
            obj->onCloseBang();
    });

    /* pass the menuclose message on to the real canvasmethod*/
    pd::message_to((t_pd*)x, gensym(ORIG_MENUCLOSE), Atom(f));
}

static bool is_comma(const Atom& a)
{
    return a.isComma()
        || (a.isSymbol()
            && a.asT<t_symbol*>()->s_name[0] == ','
            && a.asT<t_symbol*>()->s_name[1] == '\0');
}

MsgOnClose::MsgOnClose(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    bindReceive(gensym(MENUCLOSE));

    auto cnv = canvas();
    auto& bargs = this->args();
    AtomList m;

    for (size_t i = 0; i < bargs.size(); i++) {
        auto& a = bargs[i];
        if (is_comma(a)) {
            msg_.push_back(Message::makeTyped(m));
            m.clear();
        } else {
            m.append(a.expandDollarArgs(cnv));
        }
    }

    if (!m.empty())
        msg_.push_back(Message::makeTyped(m));
}

void MsgOnClose::onCloseBang()
{
    for (auto& m : msg_)
        messageTo(0, m);
}

void setup_msg_on_close()
{
    ObjectFactory<MsgOnClose> obj("msg.onclose");
    obj.useLoadBang();

    auto orgfun = zgetfn(&canvas_class, gensym(MENUCLOSE));
    if ((t_gotfn)msg_onclose_menuclose != orgfun) {
        class_addmethod(canvas_class, (t_method)msg_onclose_menuclose, gensym(MENUCLOSE), A_DEFFLOAT, 0);
        class_addmethod(canvas_class, (t_method)orgfun, gensym(ORIG_MENUCLOSE), A_DEFFLOAT, 0);
    }
}
