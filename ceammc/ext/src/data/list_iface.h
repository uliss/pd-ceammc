#ifndef LIST_IFACE_H_
#define LIST_IFACE_H_

#include "ceammc_convert.h"
#include "ceammc_data.h"
#include "ceammc_object.h"
#include "data_protocol.h"
#include "datatype_mlist.h"

#include <ctime>
#include <random>

using namespace ceammc;

template <class T>
class DataListIFace : public ListIFace<T> {
public:
    DataListIFace(const PdArgs& a)
        : ListIFace<T>(a)
    {
    }

    void onBang() override
    {
        this->listTo(0, list());
    }

    void onFloat(t_float f) override
    {
        const size_t N = list().size();
        if (!N)
            return;

        auto idx = relativeIndex<int>(f, N);
        if (idx < 0) {
            OBJ_ERR << "invalid index: " << f;
            return;
        }

        this->atomTo(0, list()[idx]);
    }

    void onSymbol(t_symbol* s) override
    {
        onList(AtomList(s));
    }

    void onList(const AtomList& l) override
    {
        list() = l;
        onBang();
    }

    void onInlet(size_t n, const AtomListView& l) override
    {
        if (n != 1)
            return;

        list() = l;
    }

    void onDataT(const MListAtom& ml)
    {
        onList(ml->data());
    }

    bool proto_front(Atom& res) const override
    {
        auto a = list().first();

        if (a) {
            res = *a;
            return true;
        } else
            return false;
    }

    bool proto_back(Atom& res) const override
    {
        auto a = list().last();

        if (a) {
            res = *a;
            return true;
        } else
            return false;
    }

    void proto_set(const AtomListView& lst) override
    {
        list() = lst;
    }

    void proto_clear() override
    {
        list().clear();
    }

    void proto_append(const AtomListView& lst) override
    {
        list().append(lst);
    }

    void proto_prepend(const AtomListView& lst) override
    {
        list().insert(0, lst);
    }

    bool proto_insert(size_t idx, const AtomListView& lst) override
    {
        return list().insert(idx, lst);
    }

    bool proto_pop() override
    {
        size_t n = list().size();
        if (n < 1)
            return false;

        return list().remove(n - 1);
    }

    bool proto_removeAt(size_t pos) override
    {
        return list().remove(pos);
    }

    size_t proto_size() const override
    {
        return list().size();
    }

    void proto_sort() override
    {
        list().sort();
    }

    void proto_reverse() override
    {
        list().reverse();
    }

    void proto_shuffle() override
    {
        list().shuffle();
    }

    void proto_fill(const Atom& v) override
    {
        list().fill(v);
    }

    bool proto_choose(Atom& res) const override
    {
        using RandomGenT = std::mt19937;
        static RandomGenT gen(time(0));

        auto N = list().size();
        if (N < 1)
            return false;

        auto idx = std::uniform_int_distribution<size_t>(0, N - 1)(gen);
        res = list().at(idx);
        return true;
    }

    void dump() const override
    {
        T::dump();
        OBJ_DBG << list();
    }

public:
    virtual AtomList& list() = 0;
    virtual const AtomList& list() const = 0;
};

#endif
