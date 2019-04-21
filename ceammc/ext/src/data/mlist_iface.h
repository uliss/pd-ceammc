#ifndef MLIST_IFACE_H_
#define MLIST_IFACE_H_

#include "ceammc_object.h"
#include "data_protocol.h"
#include "datatype_mlist.h"

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
        OBJ_DBG << mlist().toString();
    }

    void onBang() override
    {
        this->dataTo(0, mlist().clone());
    }

    void onList(const AtomList& lst) override
    {
        mlist().set(lst);
        onBang();
    }

    void onAny(t_symbol* s, const AtomList& lst) override
    {
        Message msg(s, lst);
        if (s->s_name[0] != '\(') {
            OBJ_ERR << "opening parenthesis expected: " << to_string(msg);
            return;
        }

        auto res = DataTypeMList::parse(msg.anyValue());

        if (res)
            mlist() = *res;
        else
            OBJ_ERR << "parse error: " << lst;
    }

    void onDataT(const DataTPtr<DataTypeMList>& dptr)
    {
        mlist() = *dptr;
        onBang();
    }

    void proto_set(const AtomList& lst) override
    {
        mlist().set(lst);
    }

    void proto_clear() override
    {
        mlist().clear();
    }

    void proto_append(const AtomList& lst) override
    {
        mlist().append(lst);
    }

    void proto_prepend(const AtomList& lst) override
    {
        mlist().prepend(lst);
    }

    bool proto_insert(size_t idx, const AtomList& lst) override
    {
        return mlist().insert(idx, lst);
    }

    bool proto_pop() override
    {
        size_t n = mlist().size();
        if (n < 1)
            return false;

        return mlist().remove(n - 1);
    }

    bool proto_removeAt(size_t pos) override
    {
        return mlist().remove(pos);
    }

    size_t proto_size() const override
    {
        return mlist().size();
    }

    void proto_sort() override
    {
        mlist().sort();
    }

    void proto_reverse() override
    {
        mlist().reverse();
    }

    void proto_shuffle() override
    {
        mlist().shuffle();
    }

    void proto_fill(const Atom& v) override
    {
        std::fill(mlist().begin(), mlist().end(), DataAtom(v));
    }

public:
    virtual DataTypeMList& mlist() = 0;
    virtual const DataTypeMList& mlist() const = 0;
};

#endif
