/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef BASE_EXTENSION_TEST_H
#define BASE_EXTENSION_TEST_H

#include "catch.hpp"
#include "ceammc_factory.h"
#include "ceammc_message.h"
#include "ceammc_object.h"

#include <cassert>
#include <vector>

using namespace ceammc;

typedef std::vector<Message> MessageList;
extern "C" void obj_init();

template <class T>
static t_object* make_owner(const char* name)
{
    static t_class* test_pd_class = 0;

    if (test_pd_class == 0) {
        obj_init();
        test_pd_class = class_new(gensym(name),
            reinterpret_cast<t_newmethod>(0), 0,
            sizeof(PdObject<T>), 0, A_NULL);
    }

    assert(test_pd_class != 0);

    return (t_object*)pd_new(test_pd_class);
}

template <class T>
class TestExtension : public T {
    std::vector<MessageList> msg_;
    std::vector<long> msg_count_;

public:
    TestExtension(const char* name, const AtomList& args)
        : T(PdArgs(args, gensym(name), make_owner<T>(name)))
    {
        msg_.assign(T::numOutlets(), MessageList());
        msg_count_.assign(T::numOutlets(), -1);
    }

    void sendBang()
    {
        T::onBang();
    }

    void sendFloat(float f, int inlet = 0)
    {
        if (inlet == 0)
            T::onFloat(f);
        else
            T::onInlet(inlet, AtomList(f));
    }

    void sendSymbol(t_symbol* s, int inlet = 0)
    {
        if (inlet == 0)
            T::onSymbol(s);
        else
            T::onInlet(inlet, AtomList(s));
    }

    void sendSymbol(const char* s, int inlet = 0)
    {
        sendSymbol(gensym(s), inlet);
    }

    void sendList(const AtomList& lst, int inlet = 0)
    {
        if (inlet == 0)
            T::onList(lst);
        else
            T::onInlet(inlet, lst);
    }

    void sendAny(const char* name, const AtomList& args = AtomList())
    {
        T::onAny(gensym(name), args);
    }

    void sendAny(const AtomList& args)
    {
        if (args.empty() || (!args[0].isSymbol()))
            return;

        Atom name = args[0];
        T::onAny(name.asSymbol(), args.slice(1));
    }

    virtual void bangTo(size_t n)
    {
        msg_[n].push_back(Message(&s_bang));
    }

    virtual void listTo(size_t n, const AtomList& lst)
    {
        msg_[n].push_back(Message(lst));
    }

    virtual void symbolTo(size_t n, t_symbol* s)
    {
        msg_[n].push_back(Message(s));
    }

    virtual void floatTo(size_t n, float f)
    {
        msg_[n].push_back(Message(f));
    }

    virtual void atomTo(size_t n, const Atom& a)
    {
        msg_[n].push_back(Message(a));
    }

    virtual void anyTo(size_t n, const AtomList& lst)
    {
        msg_[n].push_back(Message(lst.at(0).asSymbol(), lst.slice(1)));
    }

    virtual void anyTo(size_t n, t_symbol* sel, const AtomList& lst)
    {
        msg_[n].push_back(Message(sel, lst));
    }

public:
    void storeMessageCount(size_t outlet = 0)
    {
        msg_count_[outlet] = msg_[outlet].size();
    }

    void storeAllMessageCount()
    {
        for (size_t i = 0; i < msg_.size() && i < msg_count_.size(); i++)
            msg_count_[i] = msg_[i].size();
    }

    bool hasNewMessages(size_t outlet = 0)
    {
        return msg_count_[outlet] != msg_[outlet].size();
    }

    size_t messageCount(size_t outlet = 0) const
    {
        return msg_[outlet].size();
    }

    const Message& lastMessage(size_t outlet = 0) const
    {
        return msg_[outlet].back();
    }

    bool lastMessageIsBang(size_t outlet = 0) const
    {
        if (msg_[outlet].empty())
            return false;

        return msg_[outlet].back().isList();
    }

    void cleanMessages(size_t outlet = 0)
    {
        msg_[outlet].clear();
    }

    void cleanAllMessages()
    {
        for (size_t i = 0; i < msg_.size(); i++)
            cleanMessages(i);
    }
};

#define CALL(obj, method)                            \
    {                                                \
        obj.storeMessageCount();                     \
        obj.m_##method(gensym(#method), AtomList()); \
    }

#define CALL1(obj, method, arg1)                         \
    {                                                    \
        obj.storeMessageCount();                         \
        obj.m_##method(gensym(#method), AtomList(arg1)); \
    }

#define REQUIRE_LIST_MSG(obj, lst)                     \
    {                                                  \
        REQUIRE(obj.hasNewMessages());                 \
        REQUIRE(obj.lastMessage().isList());           \
        REQUIRE(obj.lastMessage().listValue() == lst); \
    }

#define REQUIRE_LIST_AT_OUTLET(outlet, obj, lst)             \
    {                                                        \
        REQUIRE(obj.hasNewMessages(outlet));                 \
        REQUIRE(obj.lastMessage(outlet).isList());           \
        REQUIRE(obj.lastMessage(outlet).listValue() == lst); \
    }

#define REQUIRE_FLOAT_AT_OUTLET(outlet, obj, v)                         \
    {                                                                   \
        REQUIRE(obj.hasNewMessages(outlet));                            \
        REQUIRE(obj.lastMessage(outlet).isFloat());                     \
        REQUIRE(obj.lastMessage(outlet).atomValue() == Atom(float(v))); \
    }

#define REQUIRE_SYMBOL_AT_OUTLET(outlet, obj, s)                         \
    {                                                                    \
        REQUIRE(obj.hasNewMessages(outlet));                             \
        REQUIRE(obj.lastMessage(outlet).isSymbol());                     \
        REQUIRE(obj.lastMessage(outlet).atomValue() == Atom(gensym(s))); \
    }

#define REQUIRE_ANY_AT_OUTLET(outlet, obj, anyLst)             \
    {                                                          \
        REQUIRE(obj.hasNewMessages(outlet));                   \
        REQUIRE(obj.lastMessage(outlet).isAny());              \
        REQUIRE(obj.lastMessage(outlet).anyValue() == anyLst); \
    }

#define REQUIRE_PROP(obj, name, val)                   \
    {                                                  \
        Property* p = obj.property(gensym("@" #name)); \
        REQUIRE(p != 0);                               \
        REQUIRE(p->get() == val);                      \
    }

#define REQUIRE_PROPERTY(obj, name, val)           \
    {                                              \
        Property* p = obj.property(gensym(#name)); \
        REQUIRE(p != 0);                           \
        REQUIRE(p->get() == val);                  \
    }

#define REQUIRE_PROPERTY_NONE(obj, name)           \
    {                                              \
        Property* p = obj.property(gensym(#name)); \
        REQUIRE(p != 0);                           \
        REQUIRE_FALSE(p->get().empty());           \
        REQUIRE(p->get()[0].isNone());             \
    }

#define REQUIRE_INDEX(obj, idx)                         \
    {                                                   \
        REQUIRE(obj.p_index() == AtomList(float(idx))); \
    }

#define REQUIRE_NO_MSG(obj) REQUIRE_FALSE(obj.hasNewMessages())

#define REQUIRE_NO_MESSAGES_AT_OUTLET(outlet, obj) REQUIRE_FALSE(obj.hasNewMessages(outlet))

#define REQUIRE_NEW_MESSAGES_AT_OUTLET(outlet, obj) REQUIRE(obj.hasNewMessages(outlet))

#define S(v) Atom(gensym(v))
#define F(v) Atom(float(v))
#define P(v) Atom(gensym(v))

// clang-format off
Atom test_atom_wrap(const char* v) { return Atom(gensym(v)); }
Atom test_atom_wrap(t_symbol* v) { return Atom(v); }
Atom test_atom_wrap(float v) { return Atom(v); }
Atom test_atom_wrap(const Atom& v) { return v; }

AtomList test_list_wrap(const Atom& a1) { return AtomList(a1); }
AtomList test_list_wrap(const Atom& a1, const Atom& a2) { return AtomList(a1, a2); }
AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3) {
    AtomList res(a1, a2); res.append(a3); return res;
}
AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4) {
    AtomList res(a1, a2); res.append(a3); res.append(a4); return res;
}
AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4,
                        const Atom& a5) {
    AtomList res(a1, a2); res.append(a3); res.append(a4); res.append(a5); return res;
}
AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4,
                        const Atom& a5, const Atom& a6) {
    AtomList res(a1, a2); res.append(a3); res.append(a4); res.append(a5); res.append(a6); return res;
}
AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4,
                        const Atom& a5, const Atom& a6, const Atom& a7) {
    AtomList res(a1, a2); res.append(a3); res.append(a4);
    res.append(a5); res.append(a6); res.append(a7); return res;
}
AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4,
                        const Atom& a5, const Atom& a6, const Atom& a7, const Atom& a8) {
    AtomList res(a1, a2); res.append(a3); res.append(a4);
    res.append(a5); res.append(a6); res.append(a7); res.append(a8); return res;
}
// clang-format on

#define L1(v) test_list_wrap(test_atom_wrap(v))
#define L2(v1, v2) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2))
#define L3(v1, v2, v3) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), test_atom_wrap(v3))
#define L4(v1, v2, v3, v4) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4))
#define L5(v1, v2, v3, v4, v5) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4), test_atom_wrap(v5))
#define L6(v1, v2, v3, v4, v5, v6) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4), test_atom_wrap(v5), test_atom_wrap(v6))
#define L7(v1, v2, v3, v4, v5, v6, v7) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4), test_atom_wrap(v5), test_atom_wrap(v6), test_atom_wrap(v7))
#define L8(v1, v2, v3, v4, v5, v6, v7, v8) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2),       \
    test_atom_wrap(v3), test_atom_wrap(v4), test_atom_wrap(v5), test_atom_wrap(v6), test_atom_wrap(v7), \
    test_atom_wrap(v8))

#define WHEN_CALL(obj, method)                       \
    {                                                \
        obj.storeAllMessageCount();                  \
        obj.m_##method(gensym(#method), AtomList()); \
    }

#define WHEN_CALL_1(obj, method, a1)             \
    {                                            \
        obj.storeAllMessageCount();              \
        obj.m_##method(gensym(#method), L1(a1)); \
    }

#define WHEN_CALL_2(obj, method, a1, a2)             \
    {                                                \
        obj.storeAllMessageCount();                  \
        obj.m_##method(gensym(#method), L2(a1, a2)); \
    }

#define WHEN_CALL_3(obj, method, a1, a2, a3)             \
    {                                                    \
        obj.storeAllMessageCount();                      \
        obj.m_##method(gensym(#method), L3(a1, a2, a3)); \
    }

#define WHEN_CALL_4(obj, method, a1, a2, a3, a4)             \
    {                                                        \
        obj.storeAllMessageCount();                          \
        obj.m_##method(gensym(#method), L4(a1, a2, a3, a4)); \
    }

#define WHEN_CALL_5(obj, method, a1, a2, a3, a4, a5)             \
    {                                                            \
        obj.storeAllMessageCount();                              \
        obj.m_##method(gensym(#method), L5(a1, a2, a3, a4, a5)); \
    }

#define WHEN_CALL_6(obj, method, a1, a2, a3, a4, a5, a6)             \
    {                                                                \
        obj.storeAllMessageCount();                                  \
        obj.m_##method(gensym(#method), L6(a1, a2, a3, a4, a5, a6)); \
    }

#define WHEN_CALL_7(obj, method, a1, a2, a3, a4, a5, a6, a7)             \
    {                                                                    \
        obj.storeAllMessageCount();                                      \
        obj.m_##method(gensym(#method), L7(a1, a2, a3, a4, a5, a6, a7)); \
    }

#define WHEN_CALL_8(obj, method, a1, a2, a3, a4, a5, a6, a7, a8)             \
    {                                                                        \
        obj.storeAllMessageCount();                                          \
        obj.m_##method(gensym(#method), L8(a1, a2, a3, a4, a5, a6, a7, a8)); \
    }

template <class T>
void WHEN_SEND_ANY_TO(T& obj, const AtomList& lst)
{
    obj.storeAllMessageCount();
    obj.sendAny(lst);
}

template <class T>
void WHEN_SEND_ANY_TO(T& obj, const char* name, const AtomList& lst)
{
    obj.storeAllMessageCount();
    obj.sendAny(name, lst);
}

template <class T>
void WHEN_SEND_LIST_TO(size_t inlet, T& obj, const AtomList& lst)
{
    obj.storeAllMessageCount();
    obj.sendList(lst, inlet);
}

template <class T>
void WHEN_SEND_FLOAT_TO(size_t inlet, T& obj, float v)
{
    obj.storeAllMessageCount();
    obj.sendFloat(v, inlet);
}

template <class T>
void WHEN_SEND_SYMBOL_TO(size_t inlet, T& obj, const char* sym)
{
    obj.storeAllMessageCount();
    obj.sendSymbol(gensym(sym), inlet);
}

template <class T>
void WHEN_SEND_BANG_TO(size_t inlet, T& obj)
{
    obj.storeAllMessageCount();
    obj.sendBang(inlet);
}

#endif // BASE_EXTENSION_TEST_H
