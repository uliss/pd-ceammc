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

static void testTask(SubscriberId id, lua::LuaCmd cmd, lua::LuaPipe& pipe, const bool* quit)
{
    lua::LuaInterp interp(&pipe, id, quit);
    interp.run(cmd);

    std::atomic_thread_fence(std::memory_order_acquire);
    Dispatcher::instance().send({ id, NOTIFY_UPDATE });
    return;
}

LangLuaJit::LangLuaJit(const PdArgs& args)
    : PollThreadQueueObject<lua::LuaCmd>(args)
    , interp_cmd_ { lua::LUA_CMD_NOP }
{
    //    guiconnect = nullptr;
    createOutlet();

    Dispatcher::instance().subscribe(this, subscriberId());
}

void LangLuaJit::onBang()
{
    interp_cmd_ = { lua::LUA_CMD_NOP };

    if (!runTask()) {
        OBJ_ERR << "can't run task";
        return;
    }
}

void LangLuaJit::onFloat(t_float f)
{
    interp_cmd_ = { lua::LUA_CMD_NOP, f };

    if (!runTask()) {
        OBJ_ERR << "can't run task";
        return;
    }
}

void LangLuaJit::onList(const AtomList& lst)
{
    interp_cmd_ = { lua::LUA_INTERP_EVAL, to_string(lst) };

    if (!runTask()) {
        OBJ_ERR << "can't run task";
        return;
    }
}

PollThreadTaskObject<int>::Future LangLuaJit::createTask()
{
    quit_ = false;
    return std::async(std::launch::async, testTask, subscriberId(), interp_cmd_, std::ref(result_), &quit_);
}

class my_visitor : public boost::static_visitor<Atom> {
public:
    Atom operator()(int64_t i) const
    {
        return i;
    }

    Atom operator()(double f) const
    {
        return f;
    }

    Atom operator()(const std::string& str) const
    {
        return gensym(str.c_str());
    }
};

void LangLuaJit::processMessage(const lua::LuaCmd& msg)
{
    using namespace lua;

    switch (msg.cmd) {
    case LUA_CMD_OUTPUT:
        if (msg.args.size() == 0) {
            OBJ_ERR << "invalid arguments";
            break;
        } else {
            AtomList res;
            for (auto& a : msg.args)
                res.append(boost::apply_visitor(my_visitor(), a));

            const int idx = res.intAt(0, 0);
            if (res.size() == 1)
                bangTo(idx);
            else if (res.size() == 2)
                atomTo(idx, res[1]);
            else
                listTo(idx, res.view(1));
        }
        break;
    case LUA_CMD_POST:
        if (msg.args.size() == 1 && msg.args[0].type() == typeid(LuaString))
            OBJ_POST << boost::get<std::string>(msg.args[0]);

        break;
    case LUA_CMD_ERROR:
        if (msg.args.size() == 1 && msg.args[0].type() == typeid(LuaString))
            OBJ_ERR << boost::get<std::string>(msg.args[0]);

        break;
    default:
        OBJ_ERR << "unknown command code: " << msg.cmd;
        break;
    }
}

void LangLuaJit::m_file(t_symbol* s, const AtomListView& lv)
{
    std::string path = to_string(lv);
    const auto full_path = findInStdPaths(path.c_str());
    if (full_path.empty()) {
        METHOD_ERR(s) << "can't find file: '" << path << "'";
        return;
    }

    interp_cmd_ = { lua::LUA_INTERP_LOAD, full_path };

    if (!runTask()) {
        OBJ_ERR << "can't run task";
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
    Dispatcher::instance();
    ObjectFactory<LangLuaJit> obj("lang.lua");
    obj.addMethod("load", &LangLuaJit::m_file);
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
