#ifndef MLIST_IFACE_H_
#define MLIST_IFACE_H_

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "data_protocol.h"
#include "datatype_mlist.h"

using namespace ceammc;

template <class T>
class DataMListIFace : public ListIFace<T> {
    MListAtom mlist_;

public:
    DataMListIFace(const PdArgs& a)
        : ListIFace<T>(a)
    {
    }

    void dump() const override
    {
        BaseObject::dump();
        OBJ_DBG << "list: " << *mlist_;
    }

    void onBang() override
    {
        this->atomTo(0, mlist_);
    }

    void onList(const AtomList& lst) override
    {
        mlist_.detachData();
        mlist_->setRaw(lst);
        onBang();
    }

    void onAny(t_symbol* s, const AtomList& lst) override
    {
        Message msg(s, lst);
        if (s->s_name[0] != '\(') {
            OBJ_ERR << "opening parenthesis expected: " << to_string(msg);
            return;
        }

        mlist_.detachData();
        mlist_->setParsed(msg.anyValue());
    }

    void onDataT(const MListAtom& ml)
    {
        mlist_ = ml;
        onBang();
    }

    void proto_set(const AtomList& lst) override
    {
        mlist_.detachData();
        mlist_->setRaw(lst);
    }

    void proto_clear() override
    {
        mlist_.detachData();
        mlist_->clear();
    }

    void proto_append(const AtomList& lst) override
    {
        mlist_.detachData();
        mlist_->append(lst);
    }

    void proto_prepend(const AtomList& lst) override
    {
        mlist_.detachData();
        mlist_->prepend(lst);
    }

    bool proto_insert(size_t idx, const AtomList& lst) override
    {
        mlist_.detachData();
        return mlist_->insert(idx, lst);
    }

    bool proto_pop() override
    {
        auto n = mlist_->size();
        if (n < 1)
            return false;

        mlist_.detachData();
        return mlist_->remove(n - 1);
    }

    bool proto_removeAt(size_t pos) override
    {
        mlist_.detachData();
        return mlist_->remove(pos);
    }

    size_t proto_size() const override
    {
        return mlist_->size();
    }

    void proto_sort() override
    {
        mlist_.detachData();
        mlist_->sort();
    }

    void proto_reverse() override
    {
        mlist_.detachData();
        mlist_->reverse();
    }

    void proto_shuffle() override
    {
        mlist_.detachData();
        mlist_->shuffle();
    }

    void proto_fill(const Atom& v) override
    {
        mlist_.detachData();
        std::fill(mlist_->begin(), mlist_->end(), v);
    }

    const MListAtom& mlist() const { return mlist_; }
    MListAtom& mlist() { return mlist_; }
};

#endif
