/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "load_msg.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_string.h"

extern "C" {
#include "g_canvas.h"
EXTERN int canvas_getdollarzero(void);
}

LoadMsg::LoadMsg(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    auto bb = binbuf_new();
    binbuf_restore(bb, args().size(), args().toPdData());
    raw_args_ = AtomList(binbuf_getnatom(bb), binbuf_getvec(bb));
    binbuf_free(bb);

    realizeDollars();
}

void LoadMsg::realizeDollars()
{
    auto cnv = canvas();
    for (auto& a : raw_args_) {
        if (a.isSymbol() || a.isDollarSym())
            a = canvas_realizedollar(cnv, a.asT<t_symbol*>());
        else if (a.isDollar()) {
            auto dollar_idx = a.atom().a_w.w_index;
            int argc = 0;
            t_atom* argv = nullptr;
            canvas_getargs(&argc, &argv);
            if (dollar_idx == 0)
                a = canvas_getdollarzero();
            else if (dollar_idx > 0 && dollar_idx <= argc)
                a = Atom(argv[dollar_idx - 1]);
            else {
                char buf[32];
                sprintf(buf, "$%d", dollar_idx);
                a = gensym(buf);
            }
        }
    }
}

void LoadMsg::output()
{
    enum State {
        NORMAL,
        ON_SEMI
    };

    if (raw_args_.empty())
        bangTo(0);
    else if (raw_args_.isFloat())
        floatTo(0, raw_args_[0].asT<t_float>());
    else {
        State cs = NORMAL;
        size_t msg_start = 0;

        for (size_t i = 0; i < raw_args_.size(); i++) {
            auto& a = raw_args_[i];
            switch (cs) {
            case NORMAL: {
                if (a.isComma()) {
                    const auto msg = raw_args_.view(msg_start, i - msg_start);
                    doOutput(msg);
                    msg_start = i + 1;
                } else if (a.isSemicolon()) {
                    const auto msg = raw_args_.view(msg_start, i - msg_start);
                    doOutput(msg);
                    msg_start = i + 1;
                    cs = ON_SEMI;
                }
            } break;
            case ON_SEMI: {
                if (a.isComma()) {
                    const auto msg = raw_args_.view(msg_start, i - msg_start);
                    doSendPd(msg);
                    msg_start = i + 1;
                    cs = NORMAL;
                } else if (a.isSemicolon()) {
                    const auto msg = raw_args_.view(msg_start, i - msg_start);
                    doSendPd(msg);
                    msg_start = i + 1;
                }
            } break;
            }
        }

        if (msg_start < raw_args_.size()) {
            switch (cs) {
            case NORMAL:
                doOutput(raw_args_.view(msg_start, raw_args_.size() - msg_start));
                break;
            case ON_SEMI:
                doSendPd(raw_args_.view(msg_start, raw_args_.size() - msg_start));
                break;
            }
        }
    }
}

void LoadMsg::onClick(t_floatarg /*xpos*/, t_floatarg /*ypos*/, t_floatarg /*shift*/, t_floatarg /*ctrl*/, t_floatarg /*alt*/)
{
    output();
}

void LoadMsg::onLoadBang()
{
    output();
}

void LoadMsg::doSendPd(const AtomListView& lv)
{
    if (lv.size() >= 1) {
        auto addr = atom_getsymbol(&lv[0].atom())->s_thing;
        if (addr) {
            auto msg = lv.subView(1);
            if (msg.empty())
                pd_bang(addr);
            else if (msg.isFloat())
                pd_float(addr, msg[0].asT<t_float>());
            else if (msg.size() > 1 && msg[0].isFloat())
                pd_list(addr, &s_list, msg.size(), msg.toPdData());
            else {
                auto args = msg.subView(1);
                pd_typedmess(addr, msg[0].asSymbol(), args.size(), args.toPdData());
            }

        } else
            OBJ_ERR << "unknown target: " << atom_getsymbol(&lv[0].atom())->s_name;
    } else
        OBJ_ERR << "empty global send message";
}

void LoadMsg::doOutput(const AtomListView& lv)
{
    if (lv.empty())
        bangTo(0);
    else if (lv.isFloat())
        floatTo(0, lv[0].asT<t_float>()); //
    else if (lv.size() > 1 && lv[0] == &s_float) // explicit float: float 123
        floatTo(0, lv[1].asFloat());
    else if (lv.size() > 1 && lv[0] == &s_symbol) // symbol A B C
        symbolTo(0, lv[1].asSymbol());
    else if (lv.size() > 1 && lv[0].isFloat()) // implicit list: 1 2 3
        listTo(0, lv);
    else if (lv.size() > 1 && lv[0] == &s_list) // explicit list: list 1 2 3
        listTo(0, lv.subView(1));
    else
        anyTo(0, lv);
}

void setup_load_msg()
{
    ObjectFactory<LoadMsg> obj("msg.onload", OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.addAlias("loadmsg");
    obj.useClick();
    obj.useLoadBang();
    obj.noPropsDispatch();
    obj.noArgsAndPropsParse();

    obj.setDescription("send message when patch loads");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "message", "loadbang", "onload" });
    obj.setCategory("msg");
    obj.setSinceVersion(0, 7);
}
