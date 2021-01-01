#include "msg.h"

Msg::Msg(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createOutlet();

    setMethod(a.args);
}

void Msg::onBang()
{
    data_ = prefix_;
    output();
}

void Msg::onFloat(t_float v)
{
    data_ = prefix_;
    data_.append(v);
    output();
}

void Msg::onSymbol(t_symbol* s)
{
    data_ = prefix_;
    data_.append(s);
    output();
}

void Msg::onList(const AtomList& l)
{
    data_ = prefix_;
    data_.append(l);
    output();
}

void Msg::onAny(t_symbol* sel, const AtomListView& l)
{
    data_ = prefix_;
    data_.append(sel);
    data_.append(l);
    output();
}

void Msg::onInlet(size_t n, const AtomListView& l)
{
    if (n != 1)
        return;

    setMethod(l);
}

void Msg::setMethod(const AtomList& l)
{
    if (l.empty())
        return;

    prefix_ = l;
}

void Msg::output()
{
    if (data_.empty())
        return;

    if (data_.first()->isSymbol())
        anyTo(0, data_);
    else
        listTo(0, data_);
}

void setup_base_msg()
{
    ObjectFactory<Msg> obj("msg");
    obj.addAlias("m");
    obj.addAlias("prepend");
    obj.noPropsDispatch();
    obj.noArgsDataParsing();

    obj.setDescription("message constructor");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"msg", "message"});
    obj.setCategory("msg");
    obj.setSinceVersion(0, 1);
}
