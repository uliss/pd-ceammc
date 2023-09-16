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
#include "ceammc_pd.h"
#include "lua_interp.h"

#include <algorithm>
#include <fstream>

extern "C" {
#include "luajit.h"
}

namespace {

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
}

LangLuaJit::LangLuaJit(const PdArgs& args)
    : LangLuaBase(args)
    , interp_(&outPipe(), subscriberId(), &quit())
    , nin_(nullptr)
    , nout_(nullptr)
{
    nin_ = new IntProperty("@in", 1, PropValueAccess::INITONLY);
    nin_->checkClosedRange(1, 16);
    nin_->setArgIndex(0);
    addProperty(nin_);

    nout_ = new IntProperty("@out", 1, PropValueAccess::INITONLY);
    nout_->checkClosedRange(1, 16);
    nout_->setArgIndex(1);
    addProperty(nout_);

    if (!runTask())
        OBJ_ERR << "can't start LUA event loop";
}

LangLuaJit::~LangLuaJit()
{
    finish();
}

void LangLuaJit::initDone()
{
    //    inlets_.reserve(nin_->value() - 1);
    for (int i = 1; i < nin_->value(); i++)
        inlets_.emplace_back(this, i);

    for (auto& i : inlets_)
        inlet_new(owner(), i.target(), nullptr, nullptr);

    for (int i = 0; i < nout_->value(); i++)
        createOutlet();
}

void LangLuaJit::onBang()
{
    using namespace lua;

    if (!inPipe().enqueue({ LUA_INTERP_BANG, LuaInt(0) })) {
        OBJ_ERR << "can't send command to LUA interpreter: bang";
        return;
    }

    notify_.notifyOne();
}

void LangLuaJit::onFloat(t_float f)
{
    using namespace lua;

    if (!inPipe().emplace(LUA_INTERP_FLOAT, LuaAtomList { LuaAtom(LuaInt(0)), LuaAtom(f) }))
        OBJ_ERR << "can't send command to LUA interpreter: float";

    notify_.notifyOne();
}

void LangLuaJit::onSymbol(t_symbol* s)
{
    using namespace lua;

    if (!inPipe().enqueue({ LUA_INTERP_SYMBOL, LuaAtomList { LuaAtom(LuaInt(0)), LuaAtom(s) } }))
        OBJ_ERR << "can't send command to LUA interpreter: float";

    notify_.notifyOne();
}

void LangLuaJit::onList(const AtomListView& lv)
{
    using namespace lua;

    LuaAtomList args;
    args.reserve(lv.size() + 1);
    args.emplace_back(LuaInt(0));
    for (size_t i = 0; i < lv.size(); i++) {
        const auto& a = lv[i];
        if (a.isFloat())
            args.emplace_back(LuaDouble(a.asT<t_float>()));
        else if (a.isSymbol())
            args.emplace_back(a.asT<t_symbol*>());
    }

    if (!inPipe().enqueue({ LUA_INTERP_LIST, args }))
        OBJ_ERR << "can't send command to LUA interpreter: list";

    notify_.notifyOne();
}

void LangLuaJit::onAny(t_symbol* sel, const AtomListView& lv)
{
    using namespace lua;

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

    if (!inPipe().enqueue({ LUA_INTERP_ANY, args }))
        OBJ_ERR << "can't send command to LUA interpreter: any";

    notify_.notifyOne();
}

void LangLuaJit::dump() const
{
    LangLuaBase::dump();

    Debug os(this);
    os << "source code: \n";
    for (auto& l : src_)
        os << l.view() << "\n";
}

LangLuaJit::Future LangLuaJit::createTask()
{
    setQuit(false);

    return std::async(
        std::launch::async,
        [this]() {
            lua::LuaCmd in_cmd;

            while (!quit()) {
                try {

                    while (inPipe().try_dequeue(in_cmd)) {
                        interp_.run(in_cmd);

                        if (quit())
                            return;
                    }

                    notify_.waitFor(100);

                } catch (std::exception& e) {
                    std::cerr << "lua thread exception: " << e.what();
                    return;
                }
            }

            return;
        });
}

void LangLuaJit::processTask(int)
{
    lua::LuaCmd msg;
    while (this->outPipe().try_dequeue(msg))
        processMessage(msg);
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
    case LUA_CMD_LOG:
        if (msg.args.size() == 1 && msg.args[0].isString())
            OBJ_LOG << msg.args[0].getString();

        break;
    case LUA_CMD_SEND_BANG: {
        const auto sel = (msg.args.size() < 1) ? LuaString("?") : msg.args[0].getString();
        auto sym = gensym(sel.c_str());
        if (!pd::send_bang(sym))
            OBJ_DBG << "send_bang() target not found: " << sym;
    } break;
    case LUA_CMD_SEND_FLOAT: {
        const auto sel = (msg.args.size() < 1) ? LuaString("?") : msg.args[0].getString();
        const auto val = (msg.args.size() < 2) ? 0 : msg.args[1].getDouble();

        auto sym = gensym(sel.c_str());
        if (!pd::send_float(sym, val))
            OBJ_DBG << "send_float() target not found: " << sym;
    } break;
    case LUA_CMD_SEND_SYMBOL: {
        const auto sel = (msg.args.size() < 1) ? LuaString("?") : msg.args[0].getString();
        const auto val = (msg.args.size() < 2) ? "" : msg.args[1].getString();

        auto sym = gensym(sel.c_str());
        if (!pd::send_symbol(sym, val.c_str()))
            OBJ_DBG << "send_symbol() target not found: " << sym;
    } break;
    case LUA_CMD_SEND_LIST: {
        const auto sel = (msg.args.size() < 1) ? LuaString("?") : msg.args[0].getString();

        auto sym = gensym(sel.c_str());
        if (sym->s_thing) {
            const auto N = msg.args.size();
            if (N < 1)
                return;

            AtomList res;
            res.reserve(N - 1);
            for (size_t i = 1; i < N; i++)
                res.append(msg.args[i].applyVisitor<AtomLuaVisitor>());

            pd::send_list(sym, res);
        } else
            OBJ_DBG << "send_list() target not found: " << sym;
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

    std::ifstream ifs(full_path.c_str());
    std::string line;
    src_.clear();
    constexpr int indent = 4;
    while (std::getline(ifs, line)) {
        // seems the only way to save formatting
        // replace
        auto start = line.find_first_not_of(' ');
        if (start != std::string::npos) {
            line.erase(0, start);
            line.insert(0, start / indent, '\t');
        }

        editorAddLine(&s_, AtomList(gensym(line.c_str())));
    }

    if (!inPipe().enqueue({ LUA_INTERP_LOAD, full_path }))
        METHOD_ERR(s) << "can't send command to LUA interpreter: load";

    notify_.notifyOne();
}

void LangLuaJit::m_eval(t_symbol* s, const AtomListView& lv)
{
    using namespace lua;

    if (!inPipe().enqueue({ LUA_INTERP_EVAL, to_string(lv) }))
        METHOD_ERR(s) << "can't send command to LUA interpreter: eval";

    notify_.notifyOne();
}

void LangLuaJit::m_call(t_symbol* s, const AtomListView& lv)
{
    using namespace lua;

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

    if (!inPipe().enqueue(cmd))
        METHOD_ERR(s) << "can't send command to LUA interpreter: call";

    notify_.notifyOne();
}

void LangLuaJit::onRestore(const AtomListView& lv)
{
    if (lv.empty()) {
        updateInterpSource();
    } else {
        src_.push_back({});
        auto& back = src_.back();

        for (auto& a : lv)
            back.push_back(a);
    }
}

void LangLuaJit::saveUser(t_binbuf* b)
{
    auto symA = gensym(sym_A);
    auto symR = gensym(sym_restore);

    for (auto& l : src_) {
        if (l.empty())
            continue;

        binbuf_addv(b, "ss", symA, symR);
        binbuf_add(b, l.size(), &l.front().atom());
        binbuf_addsemi(b);
    }

    binbuf_addv(b, "ss", symA, symR);
    binbuf_addsemi(b);
}

void LangLuaJit::editorClear()
{
    src_.clear();
}

void LangLuaJit::editorAddLine(t_symbol* sel, const AtomListView& lv)
{
    src_.push_back({});
    auto& b = src_.back();

    for (auto& a : lv)
        b.push_back(a);
}

EditorLineList LangLuaJit::getContentForEditor() const
{
    EditorLineList res;
    for (auto& l : src_) {
        if (l.empty())
            continue;

        auto str = EditorStringPool::pool().allocate();
        str->append(l.view());
        res.push_back(str);
    }

    return res;
}

void LangLuaJit::editorSync()
{
    LangLuaBase::editorSync();
    updateInterpSource();
}

void LangLuaJit::updateInterpSource()
{
    using namespace lua;
    try {
        if (!inPipe().enqueue(LUA_INTERP_EVAL_BEGIN))
            return;

        auto str = EditorStringPool::pool().allocate();

        for (auto& l : src_) {
            str->clear();
            str->append(l.view());
            if (!inPipe().enqueue({ LUA_INTERP_EVAL_APPEND, str->c_str() }))
                break;
        }

        if (!inPipe().enqueue(LUA_INTERP_EVAL_END))
            return;

        notify_.notifyOne();

    } catch (...) {
        OBJ_ERR << "error";
    }
}

void LangLuaJit::inletBang(int id)
{
    using namespace lua;

    if (!inPipe().enqueue({ LUA_INTERP_BANG, LuaAtomList { LuaAtom(LuaInt(id)) } }))
        OBJ_ERR << "can't send command to LUA interpreter: bang";

    notify_.notifyOne();
}

void LangLuaJit::inletFloat(int id, t_float f)
{
    using namespace lua;

    if (!inPipe().enqueue({ LUA_INTERP_FLOAT, LuaAtomList { LuaAtom(LuaInt(id)), LuaAtom(f) } }))
        OBJ_ERR << "can't send command to LUA interpreter: float";

    notify_.notifyOne();
}

void LangLuaJit::inletSymbol(int id, t_symbol* s)
{
    using namespace lua;

    if (!inPipe().enqueue({ LUA_INTERP_SYMBOL, LuaAtomList { LuaAtom(LuaInt(id)), LuaAtom(s) } }))
        OBJ_ERR << "can't send command to LUA interpreter: symbol";

    notify_.notifyOne();
}

void LangLuaJit::inletList(int id, const AtomListView& lv)
{
    using namespace lua;

    const auto N = lv.size();
    LuaAtomList args;
    args.reserve(N + 1);
    args.emplace_back(LuaInt(id));

    for (auto& a : lv) {
        if (a.isFloat())
            args.emplace_back(LuaDouble(a.asT<t_float>()));
        else if (a.isSymbol())
            args.emplace_back(a.asT<t_symbol*>());
    }

    if (!inPipe().enqueue({ LUA_INTERP_LIST, args }))
        OBJ_ERR << "can't send command to LUA interpreter: list";

    notify_.notifyOne();
}

void LangLuaJit::inletAny(int id, t_symbol* s, const AtomListView& lv)
{
    using namespace lua;

    const auto N = lv.size();
    LuaAtomList args;
    args.reserve(N + 2);
    args.emplace_back(LuaInt(id));
    args.emplace_back(s);

    for (auto& a : lv) {
        if (a.isFloat())
            args.emplace_back(LuaDouble(a.asT<t_float>()));
        else if (a.isSymbol())
            args.emplace_back(a.asT<t_symbol*>());
    }

    if (!inPipe().enqueue({ LUA_INTERP_ANY, args }))
        OBJ_ERR << "can't send command to LUA interpreter: any";

    notify_.notifyOne();
}

void setup_lang_luajit()
{
    LIB_DBG << LUAJIT_VERSION;

    Dispatcher::instance();
    ObjectFactory<LangLuaJit> obj("lang.lua");

    obj.addMethod("load", &LangLuaJit::m_load);
    obj.addMethod("eval", &LangLuaJit::m_eval);
    obj.addMethod("call", &LangLuaJit::m_call);
    obj.addMethod("quit", &LangLuaJit::m_quit);

    LangLuaJit::factoryEditorObjectInit(obj);
    LangLuaJit::factorySaveObjectInit(obj);

    InletProxy<LangLuaJit>::init();
    InletProxy<LangLuaJit>::set_bang_callback(&LangLuaJit::inletBang);
    InletProxy<LangLuaJit>::set_float_callback(&LangLuaJit::inletFloat);
    InletProxy<LangLuaJit>::set_symbol_callback(&LangLuaJit::inletSymbol);
    InletProxy<LangLuaJit>::set_list_callback(&LangLuaJit::inletList);
    InletProxy<LangLuaJit>::set_any_callback(&LangLuaJit::inletAny);
}
