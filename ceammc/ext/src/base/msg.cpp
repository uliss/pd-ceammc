#include "msg.h"

extern "C" void msg_setup()
{
    ObjectFactory<Msg> obj("msg");
    obj.addAlias("m");
}

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

void Msg::onFloat(float v)
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

void Msg::onAny(t_symbol* sel, const AtomList& l)
{
    data_ = prefix_;
    data_.append(sel);
    data_.append(l);
    output();
}

void Msg::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    setMethod(l);
}

bool Msg::processAnyProps(t_symbol* s, const AtomList&)
{
    return false;
}

void Msg::parseProperties()
{
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
