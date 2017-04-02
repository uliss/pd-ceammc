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
#include "ceammc_message.h"
#include "ceammc_object.h"

#include <vector>

using namespace ceammc;

typedef std::vector<Message> MessageList;

static t_object* make_owner(const char* name)
{
    static t_class* test_pd_class = 0;

    if (test_pd_class == 0) {
        test_pd_class = class_new(gensym(name),
            reinterpret_cast<t_newmethod>(0), 0,
            sizeof(t_object), 0, A_NULL);
    }

    return (t_object*)pd_new(test_pd_class);
}

template <class T>
class TestExtension : public T {
    std::vector<MessageList> msg_;
    std::vector<long> msg_count_;

public:
    TestExtension(const char* name, const AtomList& args)
        : T(PdArgs(args, gensym(name), make_owner(name)))
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
    }

    void sendAny(const char* name, const AtomList& args = AtomList(), int inlet = 0)
    {
        if (inlet == 0)
            T::onAny(gensym(name), args);
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

#endif // BASE_EXTENSION_TEST_H
