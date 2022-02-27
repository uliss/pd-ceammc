#ifndef MLIST_IFACE_H_
#define MLIST_IFACE_H_

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "data_protocol.h"
#include "datatype_mlist.h"

#include <ctime>
#include <random>

using namespace ceammc;

template <class T>
class DataMListIFace : public ListIFace<T> {
public:
    DataMListIFace(const PdArgs& a)
        : ListIFace<T>(a)
    {
    }

    void dump() const override
    {
        BaseObject::dump();
        OBJ_DBG << "list: " << mlist();
    }

    void onBang() override
    {
        this->atomTo(0, mlist());
    }

    void onFloat(t_float f) override
    {
        mlist()->setRaw({ f });
        onBang();
    }

    void onSymbol(t_symbol* s) override
    {
        mlist()->setRaw({ s });
        onBang();
    }

    void onList(const AtomList& lst) override
    {
        mlist()->setRaw(lst);
        onBang();
    }

    void onAny(t_symbol* s, const AtomListView& lst) override
    {
        Message msg(s, lst);
        if (s->s_name[0] != '\(') {
            OBJ_ERR << "opening parenthesis expected: " << to_string(msg);
            return;
        }

        mlist()->setParsed(msg.anyValue());
    }

    void onDataT(const MListAtom& ml)
    {
        mlist() = ml;
        // this is important detach
        mlist().detachData();
        onBang();
    }

    bool proto_front(Atom& res) const override
    {
        if (mlist()->size() > 0) {
            res = mlist()->at(0);
            return true;
        } else {
            return false;
        }
    }

    bool proto_back(Atom& res) const override
    {
        const auto size = mlist()->size();

        if (size > 0) {
            res = mlist()->at(size - 1);
            return true;
        } else {
            return false;
        }
    }

    void proto_set(const AtomListView& lst) override
    {
        mlist()->setRaw(lst);
    }

    void proto_clear() override
    {
        mlist()->clear();
    }

    void proto_append(const AtomListView& lst) override
    {
        mlist()->append(lst);
    }

    void proto_prepend(const AtomListView& lst) override
    {
        mlist()->prepend(lst);
    }

    bool proto_insert(size_t idx, const AtomListView& lst) override
    {
        return mlist()->insert(idx, lst);
    }

    bool proto_pop() override
    {
        auto n = mlist()->size();
        if (n < 1)
            return false;

        return mlist()->remove(n - 1);
    }

    bool proto_removeAt(size_t pos) override
    {
        return mlist()->remove(pos);
    }

    size_t proto_size() const override
    {
        return mlist()->size();
    }

    void proto_sort() override
    {
        mlist()->sort();
    }

    void proto_reverse() override
    {
        mlist()->reverse();
    }

    void proto_shuffle() override
    {
        mlist()->shuffle();
    }

    void proto_choose() override
    {
        using RandomGenT = std::mt19937;
        static RandomGenT gen(time(0));

        auto N = mlist()->size();
        if (N < 1)
            return;

        auto idx = std::uniform_int_distribution<size_t>(0, N - 1)(gen);
        this->atomTo(0, mlist()->at(idx));
    }

    void proto_fill(const Atom& v) override
    {
        std::fill(mlist()->begin(), mlist()->end(), v);
    }

    virtual const MListAtom& mlist() const = 0;
    virtual MListAtom& mlist() = 0;
};

#endif
