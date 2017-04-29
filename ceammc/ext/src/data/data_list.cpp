#include "data_list.h"
#include "ceammc_factory.h"

DataList::DataList(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createOutlet();

    createCbProperty("@size", &DataList::getSize);

    list_ = positionalArguments();
}

void DataList::onFloat(float f)
{
    if (list_.empty())
        return;

    if (f < 0)
        return;

    size_t idx = static_cast<size_t>(f);
    if (idx >= list_.size())
        return;

    atomTo(0, list_[idx]);
}

void DataList::onList(const AtomList& l)
{
    list_ = l;
    onBang();
}

void DataList::onBang()
{
    listTo(0, list_);
}

void DataList::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    list_ = l;
}

void DataList::m_clear(t_symbol*, const AtomList&) { list_.clear(); }

void DataList::m_append(t_symbol*, const AtomList& l) { list_.append(l); }

void DataList::m_set(t_symbol*, const AtomList& l) { list_ = l; }

void DataList::m_remove(t_symbol* s, const AtomList& l)
{
    if (!checkArgs(l, ARG_NATURAL, s))
        return;

    if (!list_.remove(l[0].asSizeT(size_t(-1))))
        METHOD_ERR(s) << "invalid index: " << l;
}

void DataList::m_pop(t_symbol* s, const AtomList&)
{
    if (list_.empty()) {
        METHOD_ERR(s) << "empty list";
        return;
    }

    list_.remove(list_.size() - 1);
}

void DataList::m_fill(t_symbol*, const AtomList& l)
{
    if (!l.empty())
        list_.fill(l[0]);
}

void DataList::m_insert(t_symbol* s, const AtomList& l)
{
    if (l.size() < 2) {
        METHOD_ERR(s) << "invalid arguments. Insert position and value expected";
        return;
    }

    size_t pos = l[0].asSizeT(0);
    if (!list_.insert(pos, l.slice(1)))
        METHOD_ERR(s) << "invalid index: " << pos;
}

void DataList::m_flush(t_symbol* s, const AtomList& l)
{
    onBang();
    m_clear(s, l);
}

void DataList::dump() const
{
    BaseObject::dump();
    OBJ_DBG << list_;
}

AtomList DataList::getSize() const
{
    return listFrom(list_.size());
}

extern "C" void setup_data0x2elist()
{
    ObjectFactory<DataList> obj("data.list");
    obj.addMethod("clear", &DataList::m_clear);
    obj.addMethod("flush", &DataList::m_flush);
    obj.addMethod("append", &DataList::m_append);
    obj.addMethod("insert", &DataList::m_insert);
    obj.addMethod("set", &DataList::m_set);
    obj.addMethod("fill", &DataList::m_fill);
    obj.addMethod("remove", &DataList::m_remove);
    obj.addMethod("pop", &DataList::m_pop);
}
