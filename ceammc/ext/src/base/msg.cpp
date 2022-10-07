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

void Msg::onList(const AtomListView& lv)
{
    data_ = prefix_;
    data_.append(lv);
    output();
}

void Msg::onAny(t_symbol* sel, const AtomListView& lv)
{
    data_ = prefix_;
    data_.append(sel);
    data_.append(lv);
    output();
}

void Msg::onInlet(size_t n, const AtomListView& lv)
{
    if (n != 1)
        return;

    setMethod(lv);
}

void Msg::setMethod(const AtomListView& lv)
{
    if (lv.empty())
        return;

    prefix_ = lv;
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
    obj.noArgsAndPropsParse();

    obj.setDescription("message constructor");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "msg", "message" });
    obj.setCategory("msg");
    obj.setSinceVersion(0, 1);
    obj.setXletsInfo({ "any: append all atoms to message and output\n"
                       "list: append list to message and output\n"
                       "float: append float to message and output\n"
                       "symbol: append symbol to message and output",
                         "symbol: set message prefix (selector)\n"
                         "list: set message prefix" },
        { "any: message output" });
}
