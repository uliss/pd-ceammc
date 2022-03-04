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

class AtomLuaVisitor : public boost::static_visitor<Atom> {
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

using LL = ObjectFactory<LangLuaJit>::ObjectProxy;

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
    : LangLuaBase(args)
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

void LangLuaJit::onAny(t_symbol* sel, const AtomListView& lv)
{
    using namespace ceammc::lua;

    LuaAtomList args;
    args.reserve(lv.size() + 1);
    args.emplace_back(sel);
    for (size_t i = 0; i < lv.size(); i++) {
        const auto& a = lv[i];
        if (a.isFloat())
            args.emplace_back(LuaDouble(a.asT<t_float>()));
        else if (a.isSymbol())
            args.emplace_back(a.asT<t_symbol*>());
    }

    if (!inPipe().try_enqueue({ LUA_INTERP_ANY, args })) {
        OBJ_ERR << "can't send command to LUA interpreter: any";
        return;
    }
}

PollThreadTaskObject<int>::Future LangLuaJit::createTask()
{
    setQuit(false);

    return std::async(std::launch::async, startLuaEventLoop, this, &quit());
}

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
            res.append(a.applyVisitor<AtomLuaVisitor>());

        listTo(n, res.view(1));
    } break;
    case LUA_CMD_ANY_TO: {
        const int n = (msg.args.size() < 1) ? 0 : msg.args[0].getInt();
        const auto sel = (msg.args.size() < 2) ? LuaString("?") : msg.args[1].getString();

        AtomList res;
        res.reserve(msg.args.size());
        for (size_t i = 2; i < msg.args.size(); i++)
            res.append(msg.args[i].applyVisitor<AtomLuaVisitor>());

        anyTo(n, gensym(sel.c_str()), res);
        break;
    } break;
    case LUA_CMD_POST:
        if (msg.args.size() == 1 && msg.args[0].isString())
            OBJ_POST << msg.args[0].getString();

        break;
    case LUA_CMD_ERROR:
        if (msg.args.size() == 1 && msg.args[0].isString())
            OBJ_ERR << msg.args[0].getString();

        break;
    case LUA_CMD_SEND_BANG: {
        const auto sel = (msg.args.size() < 1) ? LuaString("?") : msg.args[0].getString();
        auto sym = gensym(sel.c_str());
        if (sym->s_thing)
            pd_bang(sym->s_thing);
        else
            OBJ_DBG << "send_bang() target not found: " << sym;
    } break;
    case LUA_CMD_SEND_FLOAT: {
        const auto sel = (msg.args.size() < 1) ? LuaString("?") : msg.args[0].getString();
        const auto val = (msg.args.size() < 2) ? 0 : msg.args[1].getDouble();

        auto sym = gensym(sel.c_str());
        if (sym->s_thing)
            pd_float(sym->s_thing, val);
        else
            OBJ_DBG << "send_float() target not found: " << sym;
    } break;
    case LUA_CMD_SEND_SYMBOL: {
        const auto sel = (msg.args.size() < 1) ? LuaString("?") : msg.args[0].getString();
        const auto val = (msg.args.size() < 2) ? "" : msg.args[1].getString();

        auto sym = gensym(sel.c_str());
        if (sym->s_thing)
            pd_symbol(sym->s_thing, gensym(val.c_str()));
        else
            OBJ_DBG << "send_symbol() target not found: " << sym;
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

void LangLuaJit::m_call(t_symbol* s, const AtomListView& lv)
{
    using namespace ceammc::lua;

    const bool ok = lv.size() > 0 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "usage: method args???";
        return;
    }

    lua::LuaCmd cmd(LUA_INTERP_CALL);
    for (auto& a : lv) {
        if (a.isFloat())
            cmd.appendArg(LuaAtom(a.asT<t_float>()));
        else if (a.isSymbol())
            cmd.appendArg(LuaAtom(a.asT<t_symbol*>()));
        else
            OBJ_ERR << "unknown atom type: " << (int)a.type();
    }

    if (!inPipe().enqueue(cmd)) {
        METHOD_ERR(s) << "can't send command to LUA interpreter: call";
        return;
    }
}

void LangLuaJit::onSave(t_binbuf* b)
{
    OBJ_ERR << "save";

    auto x = this->owner();
    //    binbuf_addv(b, "ssii", gensym("#X"), gensym("obj"),
    //        (int)x->te_xpix, (int)x->te_ypix);
    //    binbuf_addbinbuf(b, x->te_binbuf);
    //    if (x->te_width)
    //        binbuf_addv(b, ",si", gensym("f"), (int)x->te_width);
    //    binbuf_addv(b, ";");

    binbuf_addv(b, "ssii", &s__X, gensym("obj"),
        (int)x->te_xpix, (int)x->te_ypix);
    binbuf_addbinbuf(b, x->te_binbuf);
    binbuf_addsemi(b);
    binbuf_addv(b, "ss", gensym("#A"), gensym("set"));
    auto bb = binbuf_new();
    char txt[] = "1 2 3 4 5 ABC";
    //    auto n = sizeof(txt);
    binbuf_text(bb, txt, sizeof(txt) + 1);

    binbuf_addbinbuf(b, bb);
    binbuf_addsemi(b);
    obj_saveformat(x, b);

    binbuf_free(bb);
}

void LangLuaJit::editorClear()
{
    script_content_.clear();
}

void LangLuaJit::editorAddLine(t_symbol* sel, const AtomListView& lv)
{
    auto str = EditorStringPool::pool().allocate();
    str->append(lv);
    script_content_.push_back(str);
}

EditorLineList LangLuaJit::getContentForEditor() const
{
    return script_content_;
}

static void lua_menu_open(LL* o, t_symbol* name)
{
    LIB_ERR << "test";

    post("open {%s}\n", o->impl->className()->s_name);
}

template <typename T>
class SaveFactory : public ObjectFactory<T> {
public:
    SaveFactory(const char* name)
        : ObjectFactory<T>(name)
    {
    }

public:
    using ObjectProxy = typename ObjectFactory<T>::ObjectProxy;

    static void saveFunction(ObjectProxy* p, t_binbuf* b)
    {
        p->impl->onSave(b);
    }
};

void setup_lang_luajit()
{
    LIB_DBG << LUAJIT_VERSION;

    Dispatcher::instance();
    SaveFactory<LangLuaJit> obj("lang.lua");

    obj.addMethod("load", &LangLuaJit::m_load);
    obj.addMethod("eval", &LangLuaJit::m_eval);
    obj.addMethod("call", &LangLuaJit::m_call);
    obj.addMethod("quit", &LangLuaJit::m_quit);

    LangLuaJit::registerMethods(obj);

    class_addmethod(obj.classPointer(), (t_method)lua_menu_open, gensym("menu-open"), A_NULL);

    class_setsavefn(obj.classPointer(), (t_savefn)SaveFactory<LangLuaJit>::saveFunction);
}
