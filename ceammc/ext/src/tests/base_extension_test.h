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
#include "ceammc_datastorage.h"
#include "ceammc_factory.h"
#include "ceammc_message.h"
#include "ceammc_object.h"

#include <boost/shared_ptr.hpp>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

using namespace ceammc;

typedef std::vector<Message> MessageList;
typedef std::vector<SharedDataPtr> DataPtrList;

extern "C" void obj_init();
extern "C" void pd_init();

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
    std::vector<DataPtrList> data_;

public:
    TestExtension(const char* name, const AtomList& args = AtomList());

    /** send functions */
    void sendBang(int inlet = 0);
    void sendFloat(float f, int inlet = 0);
    void sendSymbol(t_symbol* s, int inlet = 0);
    void sendSymbol(const char* s, int inlet = 0);
    void sendList(const AtomList& lst, int inlet = 0);
    void sendAny(const char* name, const AtomList& args = AtomList());
    void sendAny(const AtomList& args);
    void sendData(const AbstractData* d, int inlet = 0);

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
    virtual void dataTo(size_t n, const Data& d);

    /** messages methods */
public:
    void storeMessageCount(size_t outlet = 0);
    void storeAllMessageCount();
    bool hasNewMessages(size_t outlet = 0);
    size_t messageCount(size_t outlet = 0) const;
    const Message& lastMessage(size_t outlet = 0) const;
    const Message& messageAt(size_t idx, size_t outlet) const;
    SharedDataPtr dataAt(size_t idx, size_t outlet) const;
    const SharedDataPtr& lastData(size_t outlet = 0) const;
    bool lastMessageIsBang(size_t outlet = 0) const;
    void cleanMessages(size_t outlet = 0);
    void cleanAllMessages();

    template <class DataT>
    DataT* typedDataAt(size_t idx, size_t outlet);

    template <class DataT>
    DataT* typedLastDataAt(size_t outlet);

public:
    typedef void (*sendAtomCallback)(TestExtension* obj, size_t outn, const Atom& a);
    void setSendAtomCallback(sendAtomCallback cb) { atom_cb_ = cb; }

private:
    sendAtomCallback atom_cb_;
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
        REQUIRE(obj.lastMessage(outlet).atomValue() == data); \
    }

#define REQUIRE_PROPERTY(obj, name, val)           \
    {                                              \
        Property* p = obj.property(gensym(#name)); \
        REQUIRE(p != 0);                           \
        REQUIRE(p->get() == test_atom_wrap(val));  \
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

#define S(v) Atom(gensym(v))
#define F(v) Atom(float(v))
#define P(v) Atom(gensym(v))

// clang-format off
Atom test_atom_wrap(const char* v) { return Atom(gensym(v)); }
Atom test_atom_wrap(t_symbol* v) { return Atom(v); }
Atom test_atom_wrap(float v) { return Atom(v); }
Atom test_atom_wrap(const Atom& v) { return v; }
Atom test_atom_wrap(const Data& d) { return d.toAtom(); }

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

DataAtomList test_datalist_wrap(const Atom& a1) { return DataAtomList(a1); }

DataAtomList test_datalist_wrap(const Atom& a1, const Atom& a2) {
    DataAtomList res(a1);
    res.append(a2);
    return res;
}

DataAtomList test_datalist_wrap(const Atom& a1, const Atom& a2, const Atom& a3) {
    DataAtomList res(a1);
    res.append(a2);
    res.append(a3);
    return res;
}

DataAtomList test_datalist_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4) {
    DataAtomList res(a1);
    res.append(a2);
    res.append(a3);
    res.append(a4);
    return res;
}
// clang-format on

#define D1(v) test_datalist_wrap(test_atom_wrap(v))
#define D2(v1, v2) test_datalist_wrap(test_atom_wrap(v1), test_atom_wrap(v2))
#define D3(v1, v2, v3) test_datalist_wrap(test_atom_wrap(v1), test_atom_wrap(v2), test_atom_wrap(v3))
#define D4(v1, v2, v3, v4) test_datalist_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4))

#define A(v) test_atom_wrap(v)
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

static Atom D(DataType t, DataId id)
{
    return Atom(DataDesc(t, id));
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
void WHEN_SEND_DATA_TO(size_t inlet, T& obj, const D* d)
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

template <class T, class D>
void WHEN_SEND_DATA_TO(size_t inlet, T& obj, const D& d)
{
    WHEN_SEND_DATA_TO(inlet, obj, &d);
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

template <class T>
TestExtension<T>::TestExtension(const char* name, const AtomList& args)
    : T(PdArgs(args, gensym(name), make_owner<T>(name)))
    , atom_cb_(0)
{
    const size_t N = T::numOutlets();
    msg_.assign(N, MessageList());
    msg_count_.assign(N, -1);
    data_.assign(N, DataPtrList());
    T::parseProperties();
}

template <class T>
void TestExtension<T>::sendBang(int inlet)
{
    if (inlet == 0)
        T::onBang();
    else
        T::onInlet(inlet, AtomList());
}

template <class T>
void TestExtension<T>::sendFloat(float f, int inlet)
{
    if (inlet == 0)
        T::onFloat(f);
    else
        T::onInlet(inlet, AtomList(f));
}

template <class T>
void TestExtension<T>::sendSymbol(t_symbol* s, int inlet)
{
    if (inlet == 0)
        T::onSymbol(s);
    else
        T::onInlet(inlet, AtomList(s));
}

template <class T>
void TestExtension<T>::sendSymbol(const char* s, int inlet)
{
    sendSymbol(gensym(s), inlet);
}

template <class T>
void TestExtension<T>::sendList(const AtomList& lst, int inlet)
{
    if (inlet == 0)
        T::onList(lst);
    else
        T::onInlet(inlet, lst);
}

template <class T>
void TestExtension<T>::sendAny(const char* name, const AtomList& args)
{
    T::onAny(gensym(name), args);
}

template <class T>
void TestExtension<T>::sendAny(const AtomList& args)
{
    if (args.empty() || (!args[0].isSymbol()))
        return;

    Atom name = args[0];
    T::onAny(name.asSymbol(), args.slice(1));
}

template <class T>
void TestExtension<T>::sendData(const AbstractData* d, int inlet)
{
    T::onData(d);
}

template <class T>
template <class DataT>
void TestExtension<T>::sendTData(const DataT& d, int inlet)
{
    T::onDataT(d);
}

template <class T>
void TestExtension<T>::bangTo(size_t n)
{
    msg_[n].push_back(Message(&s_bang));
}

template <class T>
void TestExtension<T>::listTo(size_t n, const AtomList& lst)
{
    msg_[n].push_back(Message(lst));
}

template <class T>
void TestExtension<T>::symbolTo(size_t n, t_symbol* s)
{
    msg_[n].push_back(Message(s));
}

template <class T>
void TestExtension<T>::floatTo(size_t n, float f)
{
    msg_[n].push_back(Message(f));
}

template <class T>
void TestExtension<T>::atomTo(size_t n, const Atom& a)
{
    msg_[n].push_back(Message(a));

    if (atom_cb_)
        atom_cb_(this, n, a);
}

template <class T>
void TestExtension<T>::anyTo(size_t n, const AtomList& lst)
{
    msg_[n].push_back(Message(lst.at(0).asSymbol(), lst.slice(1)));
}

template <class T>
void TestExtension<T>::anyTo(size_t n, t_symbol* sel, const AtomList& lst)
{
    msg_[n].push_back(Message(sel, lst));
}

template <class T>
void TestExtension<T>::messageTo(size_t n, const Message& m)
{
    msg_[n].push_back(m);
}

template <class T>
void TestExtension<T>::dataTo(size_t n, const Data& d)
{
    SharedDataPtr p(d.clone());
    data_[n].push_back(p);
    msg_[n].push_back(p->toAtom());
}

template <class T>
void TestExtension<T>::storeMessageCount(size_t outlet)
{
    msg_count_[outlet] = msg_[outlet].size();
}

template <class T>
void TestExtension<T>::storeAllMessageCount()
{
    for (size_t i = 0; i < msg_.size() && i < msg_count_.size(); i++)
        msg_count_[i] = msg_[i].size();
}

template <class T>
bool TestExtension<T>::hasNewMessages(size_t outlet)
{
    return msg_count_[outlet] != msg_[outlet].size();
}

template <class T>
size_t TestExtension<T>::messageCount(size_t outlet) const
{
    return msg_[outlet].size();
}

template <class T>
const Message& TestExtension<T>::lastMessage(size_t outlet) const
{
    return msg_[outlet].back();
}

template <class T>
const Message& TestExtension<T>::messageAt(size_t idx, size_t outlet) const
{
    return msg_[outlet].at(idx);
}

template <class T>
SharedDataPtr TestExtension<T>::dataAt(size_t idx, size_t outlet) const
{
    return data_[outlet].at(idx);
}

template <class T>
const SharedDataPtr& TestExtension<T>::lastData(size_t outlet) const
{
    return data_[outlet].back();
}

template <class T>
bool TestExtension<T>::lastMessageIsBang(size_t outlet) const
{
    if (msg_[outlet].empty())
        return false;

    return msg_[outlet].back().isList();
}

template <class T>
void TestExtension<T>::cleanMessages(size_t outlet)
{
    msg_[outlet].clear();
    data_[outlet].clear();
}

template <class T>
void TestExtension<T>::cleanAllMessages()
{
    for (size_t i = 0; i < msg_.size(); i++)
        cleanMessages(i);
}

template <class T>
template <class DataT>
DataT* TestExtension<T>::typedDataAt(size_t idx, size_t outlet)
{
    SharedDataPtr p = dataAt(idx, outlet);
    if (p && p->type() == DataT::dataType) {
        return static_cast<DataT*>(p.get());
    } else
        return 0;
}

template <class T>
template <class DataT>
DataT* TestExtension<T>::typedLastDataAt(size_t outlet)
{
    SharedDataPtr p = lastData(outlet);
    if (p && p->type() == DataT::dataType) {
        return static_cast<DataT*>(p->data());
    } else
        return 0;
}

class IntData : public AbstractData {
    int v_;

public:
    IntData(int v)
        : v_(v)
    {
        constructor_called++;
    }

    ~IntData()
    {
        destructor_called++;
    }

    int value() const { return v_; }
    void setValue(int v) { v_ = v; }
    bool isEqual(const AbstractData* d) const
    {
        const IntData* dt = d->as<IntData>();
        if (!dt)
            return false;

        return v_ == dt->v_;
    }

    std::string toString() const
    {
        std::ostringstream buf;
        buf << v_;
        return buf.str();
    }

    DataType type() const { return dataType; }
    IntData* clone() const { return new IntData(v_); }

public:
    static DataType dataType;
    static int constructor_called;
    static int destructor_called;
};

DataType IntData::dataType = 1001;
int IntData::constructor_called = 0;
int IntData::destructor_called = 0;

class StrData : public AbstractData {
    std::string v_;

public:
    StrData(const std::string& v)
        : v_(v)
    {
        constructor_called++;
    }

    ~StrData()
    {
        destructor_called++;
    }

    const std::string& get() const { return v_; }
    void setValue(const std::string& v) { v_ = v; }
    bool isEqual(const AbstractData* d) const
    {
        const StrData* dt = d->as<StrData>();
        if (!dt)
            return false;

        return v_ == dt->v_;
    }

    std::string toString() const
    {
        return v_;
    }

    DataType type() const { return dataType; }
    StrData* clone() const { return new StrData(v_); }

public:
    static DataType dataType;
    static int constructor_called;
    static int destructor_called;
};

DataType StrData::dataType = 1002;
int StrData::constructor_called = 0;
int StrData::destructor_called = 0;

#endif // BASE_EXTENSION_TEST_H
