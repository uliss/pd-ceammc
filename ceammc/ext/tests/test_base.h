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

#include "ceammc_dataatom.h"
#include "ceammc_dataatomlist.h"
#include "ceammc_factory.h"
#include "ceammc_message.h"
#include "ceammc_object.h"

#include "test_common.h"

#include <boost/shared_ptr.hpp>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

extern "C" {
#include "m_imp.h"
}

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
class TestExternal : public T {
    std::vector<MessageList> msg_;
    std::vector<long> msg_count_;
    std::vector<DataPtrList> data_;

public:
    TestExternal(const char* name, const AtomList& args = AtomList(), bool mainSignalInlet = false);

    /** send functions */
    void sendBang(int inlet = 0);
    void sendFloat(float f, int inlet = 0);
    void sendSymbol(t_symbol* s, int inlet = 0);
    void sendSymbol(const char* s, int inlet = 0);
    void sendList(const AtomList& lst, int inlet = 0);
    void sendAny(const char* name, const AtomList& args = AtomList());
    void sendAny(const AtomList& args);
    void sendData(const DataPtr& d, int inlet = 0);

    template <class DataT>
    void sendTData(const DataT& d, int inlet = 0);

    /** overloaded */
    virtual void bangTo(size_t n);
    virtual void listTo(size_t n, const AtomList& lst);
    virtual void symbolTo(size_t n, t_symbol* s);
    virtual void floatTo(size_t n, float f);
    virtual void atomTo(size_t n, const Atom& a);
    virtual void anyTo(size_t n, const AtomList& lst);
    virtual void anyTo(size_t n, t_symbol* sel, const AtomList& lst);
    virtual void messageTo(size_t n, const Message& m);
    virtual void dataTo(size_t n, const DataPtr& d);

    /** messages methods */
public:
    void storeMessageCount(size_t outlet = 0);
    void storeAllMessageCount();
    bool hasNewMessages(size_t outlet = 0);
    size_t messageCount(size_t outlet = 0) const;
    const Message& lastMessage(size_t outlet = 0) const;
    const Message& messageAt(size_t idx, size_t outlet) const;
    DataPtr dataAt(size_t idx, size_t outlet) const;
    const DataPtr& lastData(size_t outlet = 0) const;
    bool lastMessageIsBang(size_t outlet = 0) const;
    void cleanMessages(size_t outlet = 0);
    void cleanAllMessages();

    template <class DataT>
    const DataT* typedDataAt(size_t idx, size_t outlet);

    template <class DataT>
    const DataT* typedLastDataAt(size_t outlet);

public:
    typedef void (*sendAtomCallback)(TestExternal* obj, size_t outn, const Atom& a);
    void setSendAtomCallback(sendAtomCallback cb) { atom_cb_ = cb; }

private:
    sendAtomCallback atom_cb_;
};

void setTestSampleRate(size_t sr = 44100);

template <class T>
class TestSoundExternal : public TestExternal<T> {
public:
    TestSoundExternal(const char* name, const AtomList& args = AtomList(), bool mainSignalInlet = false)
        : TestExternal<T>(name, args, mainSignalInlet)
    {
        T::setBlockSize(64);
        T::setSamplerate(48000);
        setTestSampleRate(48000);
    }
};

#define CALL(obj, method)                            \
    {                                                \
        obj.storeMessageCount();                     \
        obj.m_##method(gensym(#method), AtomList()); \
    }

#define CALL_N(obj, method, ...)                          \
    {                                                     \
        obj.storeMessageCount();                          \
        obj.m_##method(gensym(#method), LA(__VA_ARGS__)); \
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

#define REQUIRE_BANG_AT_OUTLET(outlet, obj)        \
    {                                              \
        REQUIRE(obj.hasNewMessages(outlet));       \
        REQUIRE(obj.lastMessage(outlet).isBang()); \
    }

#define REQUIRE_FLOAT_AT_OUTLET(outlet, obj, v)                                      \
    {                                                                                \
        REQUIRE(obj.hasNewMessages(outlet));                                         \
        REQUIRE(obj.lastMessage(outlet).isFloat());                                  \
        REQUIRE(double(obj.lastMessage(outlet).atomValue().asFloat()) == Approx(v)); \
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

#define REQUIRE_DATA_AT_OUTLET(outlet, obj, data)             \
    {                                                         \
        REQUIRE(obj.hasNewMessages(outlet));                  \
        REQUIRE(obj.lastMessage(outlet).isData());            \
        REQUIRE(obj.lastMessage(outlet).dataValue() == data); \
    }

#define REQUIRE_PROPERTY(obj, name, val)           \
    {                                              \
        Property* p = obj.property(gensym(#name)); \
        REQUIRE(p != 0);                           \
        REQUIRE(p->get() == test_atom_wrap(val));  \
    }

#define REQUIRE_PROPERTY_FLOAT(obj, name, val)            \
    {                                                     \
        Property* p = obj.property(gensym(#name));        \
        REQUIRE(p != 0);                                  \
        REQUIRE_FALSE(p->get().empty());                  \
        REQUIRE(p->get().at(0).asFloat() == Approx(val)); \
    }

#define REQUIRE_PROPERTY_LIST(obj, name, lst)      \
    {                                              \
        Property* p = obj.property(gensym(#name)); \
        REQUIRE(p != 0);                           \
        REQUIRE(p->get() == lst);                  \
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

#define REQUIRE_NEW_DATA_AT_OUTLET(outlet, obj)    \
    {                                              \
        REQUIRE(obj.hasNewMessages(outlet));       \
        REQUIRE(obj.lastMessage(outlet).isData()); \
    }

#define WHEN_CALL(obj, method)                       \
    {                                                \
        obj.storeAllMessageCount();                  \
        obj.m_##method(gensym(#method), AtomList()); \
    }

#define WHEN_CALL_N(obj, method, ...)                     \
    {                                                     \
        obj.storeAllMessageCount();                       \
        obj.m_##method(gensym(#method), LA(__VA_ARGS__)); \
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

template <class T, class D>
void WHEN_SEND_DATA_TO(size_t inlet, T& obj, const D& d)
{
    obj.storeAllMessageCount();
    obj.sendData(DataPtr(d.clone()), inlet);
}

template <class T>
void WHEN_SEND_DATA_TO(size_t inlet, T& obj, const DataPtr& d)
{
    obj.storeAllMessageCount();
    obj.sendData(d, inlet);
}

template <class T, class D>
void WHEN_SEND_TDATA_TO(size_t inlet, T& obj, const D& d)
{
    obj.storeAllMessageCount();
    obj.sendTData(d, inlet);
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

static PdArgs mainSignalArgs(const PdArgs& args)
{
    PdArgs res(args);
    res.mainSignalInlet = true;
    return res;
}

template <class T>
TestExternal<T>::TestExternal(const char* name, const AtomList& args, bool mainSignalInlet)
    : T(mainSignalInlet
            ? mainSignalArgs(PdArgs(args, gensym(name), make_owner<T>(name), gensym(name)))
            : PdArgs(args, gensym(name), make_owner<T>(name), gensym(name)))
    , atom_cb_(0)
{
    const size_t N = T::numOutlets();
    msg_.assign(N, MessageList());
    msg_count_.assign(N, -1);
    data_.assign(N, DataPtrList());
    T::parseProperties();
    // fix CLASS_NOINLET flag
    if (T::owner() && ObjectFactory<T>::classPointer()) {
        T::owner()->te_g.g_pd->c_firstin = ObjectFactory<T>::classPointer()->c_firstin;
    }
}

template <class T>
void TestExternal<T>::sendBang(int inlet)
{
    if (inlet == 0)
        T::onBang();
    else
        T::onInlet(inlet, AtomList());
}

template <class T>
void TestExternal<T>::sendFloat(float f, int inlet)
{
    if (inlet == 0)
        T::onFloat(f);
    else
        T::onInlet(inlet, AtomList(f));
}

template <class T>
void TestExternal<T>::sendSymbol(t_symbol* s, int inlet)
{
    if (inlet == 0)
        T::onSymbol(s);
    else
        T::onInlet(inlet, AtomList(s));
}

template <class T>
void TestExternal<T>::sendSymbol(const char* s, int inlet)
{
    sendSymbol(gensym(s), inlet);
}

template <class T>
void TestExternal<T>::sendList(const AtomList& lst, int inlet)
{
    if (inlet == 0)
        T::onList(lst);
    else
        T::onInlet(inlet, lst);
}

template <class T>
void TestExternal<T>::sendAny(const char* name, const AtomList& args)
{
    T::onAny(gensym(name), args);
}

template <class T>
void TestExternal<T>::sendAny(const AtomList& args)
{
    if (args.empty() || (!args[0].isSymbol()))
        return;

    Atom name = args[0];
    T::onAny(name.asSymbol(), args.slice(1));
}

template <class T>
void TestExternal<T>::sendData(const DataPtr& d, int inlet)
{
    if (inlet == 0)
        T::onData(d);
    else
        T::onInlet(inlet, d.asAtom());
}

template <class T>
template <class DataT>
void TestExternal<T>::sendTData(const DataT& d, int inlet)
{
    T::onDataT(DataTPtr<DataT>(d));
}

template <class T>
void TestExternal<T>::bangTo(size_t n)
{
    msg_[n].push_back(Message(&s_bang));
}

template <class T>
void TestExternal<T>::listTo(size_t n, const AtomList& lst)
{
    msg_[n].push_back(Message(lst));
}

template <class T>
void TestExternal<T>::symbolTo(size_t n, t_symbol* s)
{
    msg_[n].push_back(Message(s));
}

template <class T>
void TestExternal<T>::floatTo(size_t n, float f)
{
    msg_[n].push_back(Message(f));
}

template <class T>
void TestExternal<T>::atomTo(size_t n, const Atom& a)
{
    msg_[n].push_back(Message(a));

    if (atom_cb_)
        atom_cb_(this, n, a);
}

template <class T>
void TestExternal<T>::anyTo(size_t n, const AtomList& lst)
{
    msg_[n].push_back(Message(lst.at(0).asSymbol(), lst.slice(1)));
}

template <class T>
void TestExternal<T>::anyTo(size_t n, t_symbol* sel, const AtomList& lst)
{
    msg_[n].push_back(Message(sel, lst));
}

template <class T>
void TestExternal<T>::messageTo(size_t n, const Message& m)
{
    msg_[n].push_back(m);
}

template <class T>
void TestExternal<T>::dataTo(size_t n, const DataPtr& d)
{
    data_[n].push_back(d);
    msg_[n].push_back(d.asAtom());
}

template <class T>
void TestExternal<T>::storeMessageCount(size_t outlet)
{
    msg_count_[outlet] = msg_[outlet].size();
}

template <class T>
void TestExternal<T>::storeAllMessageCount()
{
    for (size_t i = 0; i < msg_.size() && i < msg_count_.size(); i++)
        msg_count_[i] = msg_[i].size();
}

template <class T>
bool TestExternal<T>::hasNewMessages(size_t outlet)
{
    REQUIRE(outlet < msg_.size());
    return msg_count_[outlet] != msg_[outlet].size();
}

template <class T>
size_t TestExternal<T>::messageCount(size_t outlet) const
{
    return msg_[outlet].size();
}

template <class T>
const Message& TestExternal<T>::lastMessage(size_t outlet) const
{
    return msg_[outlet].back();
}

template <class T>
const Message& TestExternal<T>::messageAt(size_t idx, size_t outlet) const
{
    return msg_[outlet].at(idx);
}

template <class T>
DataPtr TestExternal<T>::dataAt(size_t idx, size_t outlet) const
{
    return data_[outlet].at(idx);
}

template <class T>
const DataPtr& TestExternal<T>::lastData(size_t outlet) const
{
    return data_[outlet].back();
}

template <class T>
bool TestExternal<T>::lastMessageIsBang(size_t outlet) const
{
    if (msg_[outlet].empty())
        return false;

    return msg_[outlet].back().isList();
}

template <class T>
void TestExternal<T>::cleanMessages(size_t outlet)
{
    msg_[outlet].clear();
    data_[outlet].clear();
}

template <class T>
void TestExternal<T>::cleanAllMessages()
{
    for (size_t i = 0; i < msg_.size(); i++)
        cleanMessages(i);
}

template <class T>
template <class DataT>
const DataT* TestExternal<T>::typedDataAt(size_t idx, size_t outlet)
{
    DataPtr p = dataAt(idx, outlet);
    if (p && p->type() == DataT::dataType) {
        return static_cast<DataT*>(p.data());
    } else
        return 0;
}

template <class T>
template <class DataT>
const DataT* TestExternal<T>::typedLastDataAt(size_t outlet)
{
    DataPtr p = lastData(outlet);
    if (p.isValid() && p->type() == DataT::dataType) {
        return static_cast<const DataT*>(p.data());
    } else
        return 0;
}

#endif // BASE_EXTENSION_TEST_H
