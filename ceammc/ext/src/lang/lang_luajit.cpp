/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "lang_luajit.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "lua_interp.h"

#include <algorithm>

extern "C" {
#include "luajit.h"
}

//
//t_text_define *x = (t_text_define *)z;
//binbuf_addv(bb, "ssff", &s__X, gensym("obj"),
//    (float)x->x_ob.te_xpix, (float)x->x_ob.te_ypix);
//binbuf_addbinbuf(bb, x->x_ob.ob_binbuf);
//binbuf_addsemi(bb);
//if (x->x_keep)
//{
//    binbuf_addv(bb, "ss", gensym("#A"), gensym("set"));
//    binbuf_addbinbuf(bb, x->x_binbuf);
//    binbuf_addsemi(bb);
//}
//obj_saveformat(&x->x_ob, bb);

using LL = ObjectFactory<LangLuaJit>::ObjectProxy;

static void text_define_save(LL* x, t_binbuf* bb)
{
    //    t_text_define *x = (t_text_define *)z;
    binbuf_addv(bb, "ssff", &s__X, gensym("obj"),
        (float)x->pd_obj.te_xpix, (float)x->pd_obj.te_ypix);
    binbuf_addbinbuf(bb, x->pd_obj.te_binbuf);
    binbuf_addsemi(bb);
    //        if (x->x_keep)
    //        {
    //            binbuf_addv(bb, "ss", gensym("#A"), gensym("set"));
    //            binbuf_addbinbuf(bb, x->x_binbuf);
    //            binbuf_addsemi(bb);
    //        }
    obj_saveformat(&x->pd_obj, bb);
}

static void textbuf_senditup(LL* x)
{
    //    if (!x->impl->guiconnect)
    return;

    //    binbuf_gettext(x->b_binbuf, &txt, &ntxt);
    sys_vgui("pdtk_textwindow_clear .x%lx\n", x);
    //    for (auto& l : x->impl->lines_) {
    //        char* j = strchr(txt + i, '\n');
    //        if (!j)
    //            j = txt + ntxt;
    //        sys_vgui("pdtk_textwindow_append .x%lx {%s\n}\n",
    //            x, l.c_str());
    //        i = (int)((j - txt) + 1);
    //    }
    //    sys_vgui("pdtk_textwindow_setdirty .x%lx 0\n", x);
    //    t_freebytes(txt, ntxt);
}

static void textbuf_clear(LL* x)
{
    //    x->impl->lines_.clear();
    textbuf_senditup(x);
}

static void textbuf_addline(LL* b, t_symbol* s, int argc, t_atom* argv)
{
    AtomListView lst(argv, argc);
    //    b->impl->lines_.push_back(to_string(lst));
}

static void textbuf_open(LL* x)
{
    //    if (x->impl->guiconnect) {
    //    sys_vgui("wm deiconify .x%lx\n", x);
    //    sys_vgui("raise .x%lx\n", x);
    //    sys_vgui("focus .x%lx.text\n", x);
    //    } else {
    //        char buf[40];
    //        sys_vgui("pdtk_textwindow_open .x%lx %dx%d {%s} %d\n",
    //            x, 600, 340, "LUA",
    //            sys_hostfontsize(glist_getfont(x->impl->canvas()),
    //                glist_getzoom(x->impl->canvas())));
    //        sprintf(buf, ".x%lx", (unsigned long)x);
    //        x->impl->guiconnect = guiconnect_new(&x->pd_obj.te_g.g_pd, gensym(buf));
    //        textbuf_senditup(x);
    //    }
}

static void textbuf_close(LL* x)
{
    //    sys_vgui("pdtk_textwindow_doclose .x%lx\n", x);
    //    if (x->impl->guiconnect) {
    //        guiconnect_notarget(x->impl->guiconnect, 1000);
    //        x->impl->guiconnect = 0;
    //    }
}

static void startLuaEventLoop(LangLuaJit* x, const bool* quit)
{
    while (!*quit) {
        lua::LuaCmd in_cmd;
        if (x->inPipe().try_dequeue(in_cmd)) {
            x->interp().run(in_cmd);
        }

        if (*quit)
            break;
    }

    return;
}

LangLuaJit::LangLuaJit(const PdArgs& args)
    : PollThreadQueueObject<lua::LuaCmd>(args)
    , interp_(&outPipe(), subscriberId(), &quit())
{
    createOutlet();

    Dispatcher::instance().subscribe(this, subscriberId());

    if (!runTask())
        OBJ_ERR << "can't start LUA event loop";
}

LangLuaJit::~LangLuaJit()
{
    finish();
}

void LangLuaJit::onBang()
{
    using namespace ceammc::lua;

    if (!inPipe().try_enqueue({ LUA_INTERP_BANG, LuaInt(0) })) {
        OBJ_ERR << "can't send command to LUA interpreter: bang";
        return;
    }
}

void LangLuaJit::onFloat(t_float f)
{
    using namespace ceammc::lua;

    try {

        if (!inPipe().try_emplace(LUA_INTERP_FLOAT, LuaAtomList { LuaAtom(LuaInt(0)), LuaAtom(f) })) {
            OBJ_ERR << "can't send command to LUA interpreter: float";
            return;
        }
    } catch (...) {
        OBJ_ERR << "error";
    }
}

void LangLuaJit::onSymbol(t_symbol* s)
{
    using namespace ceammc::lua;

    if (!inPipe().try_enqueue({ LUA_INTERP_SYMBOL, LuaAtomList { LuaAtom(LuaInt(0)), LuaAtom(s) } })) {
        OBJ_ERR << "can't send command to LUA interpreter: float";
        return;
    }
}

void LangLuaJit::onList(const AtomList& lst)
{
    using namespace ceammc::lua;
    LuaAtomList args;
    args.reserve(lst.size() + 1);
    args.emplace_back(LuaInt(0));
    for (size_t i = 0; i < lst.size(); i++) {
        const auto& a = lst[i];
        if (a.isFloat())
            args.emplace_back(LuaDouble(a.asT<t_float>()));
        else if (a.isSymbol())
            args.emplace_back(a.asT<t_symbol*>());
    }

    if (!inPipe().try_enqueue({ LUA_INTERP_LIST, args })) {
        OBJ_ERR << "can't send command to LUA interpreter: list";
        return;
    }
}

PollThreadTaskObject<int>::Future LangLuaJit::createTask()
{
    setQuit(false);

    return std::async(std::launch::async, startLuaEventLoop, this, &quit());
}

class my_visitor : public boost::static_visitor<Atom> {
public:
    Atom operator()(lua::LuaInt i) const
    {
        return i;
    }

    Atom operator()(lua::LuaDouble f) const
    {
        return f;
    }

    Atom operator()(const lua::LuaString& str) const
    {
        return gensym(str.c_str());
    }
};

void LangLuaJit::processMessage(const lua::LuaCmd& msg)
{
    using namespace lua;

    switch (msg.cmd) {
    case LUA_CMD_BANG_TO: {
        const int n = msg.args.empty() ? 0 : msg.args[0].getInt();
        bangTo(n);
    } break;
    case LUA_CMD_FLOAT_TO: {
        const int n = (msg.args.size() < 1) ? 0 : msg.args[0].getInt();
        const t_float f = (msg.args.size() < 2) ? 0 : msg.args[1].getDouble();
        floatTo(n, f);
    } break;
    case LUA_CMD_SYMBOL_TO: {
        const int n = (msg.args.size() < 1) ? 0 : msg.args[0].getInt();
        const auto str = (msg.args.size() < 2) ? LuaString() : msg.args[1].getString();
        symbolTo(n, gensym(str.c_str()));
    } break;
    case LUA_CMD_LIST_TO: {
        const int n = (msg.args.size() < 1) ? 0 : msg.args[0].getInt();
        AtomList res;
        res.reserve(msg.args.size());

        for (auto& a : msg.args)
            res.append(a.applyVisitor<my_visitor>());

        listTo(n, res.view(1));
    } break;
    case LUA_CMD_POST:
        if (msg.args.size() == 1 && msg.args[0].isString())
            OBJ_POST << msg.args[0].getString();

        break;
    case LUA_CMD_ERROR:
        if (msg.args.size() == 1 && msg.args[0].isString())
            OBJ_ERR << msg.args[0].getString();

        break;
    case LUA_CMD_SEND: {
        AtomList res;
        for (auto& a : msg.args)
            res.append(a.applyVisitor<my_visitor>());

        if (res.size() >= 1 && res[0].isSymbol()) {
            auto dest = res[0].asT<t_symbol*>();
            if (dest->s_thing)
                pd_list(dest->s_thing, dest, res.size() - 1, res.view(1).toPdData());
        } else
            LIB_ERR << "invalid command format: " << res;
    } break;
    case LUA_CMD_MESSAGE: {
        if (msg.args.size() == 0) {
            OBJ_ERR << "invalid arguments";
            break;
        } else {
            AtomList res;
            for (auto& a : msg.args)
                res.append(a.applyVisitor<my_visitor>());

            const int idx = res.intAt(0, 0);
            t_symbol* sel = res.symbolAt(1, &s_);
            if (sel != &s_)
                anyTo(idx, sel, res.view(2));
            else
                OBJ_ERR << "invalid message args: " << res;
        }
        break;
    } break;
    default:
        OBJ_ERR << "unknown command code: " << msg.cmd;
        break;
    }
}

void LangLuaJit::m_load(t_symbol* s, const AtomListView& lv)
{
    std::string path = to_string(lv);
    const auto full_path = findInStdPaths(path.c_str());
    if (full_path.empty()) {
        METHOD_ERR(s) << "can't find file: '" << path << "'";
        return;
    }

    using namespace lua;
    if (!inPipe().enqueue({ LUA_INTERP_LOAD, full_path })) {
        METHOD_ERR(s) << "can't send command to LUA interpreter: load";
        return;
    }
}

void LangLuaJit::m_eval(t_symbol* s, const AtomListView& lv)
{
    using namespace ceammc::lua;

    if (!inPipe().enqueue({ LUA_INTERP_EVAL, to_string(lv) })) {
        METHOD_ERR(s) << "can't send command to LUA interpreter: eval";
        return;
    }
}

static void lua_menu_open(LL* o, t_symbol* name)
{
    LIB_ERR << "test";

    post("open {%s}\n", o->impl->className()->s_name);
}

void setup_lang_luajit()
{
    LIB_DBG << LUAJIT_VERSION;

    Dispatcher::instance();
    ObjectFactory<LangLuaJit> obj("lang.lua");
    obj.addMethod("load", &LangLuaJit::m_load);
    obj.addMethod("eval", &LangLuaJit::m_eval);

    obj.addMethod("quit", &LangLuaJit::m_quit);

    class_addmethod(obj.classPointer(), (t_method)textbuf_open,
        gensym("click"), A_NULL);
    //    class_addmethod(obj.classPointer(), (t_method)textbuf_close,
    //        gensym("close"), A_NULL);
    class_addmethod(obj.classPointer(), (t_method)textbuf_clear,
        gensym("clear"), A_NULL);
    class_addmethod(obj.classPointer(), (t_method)textbuf_addline,
        gensym("addline"), A_NULL);
    class_addmethod(obj.classPointer(), (t_method)textbuf_senditup,
        gensym("notify"), A_NULL, A_NULL);

    class_addmethod(obj.classPointer(), (t_method)lua_menu_open, gensym("menu-open"), A_NULL);

    class_setsavefn(obj.classPointer(), (t_savefn)text_define_save);
}
