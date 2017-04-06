#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class Msg : public BaseObject {
    t_symbol* sel_;
    AtomList prefix_;
    AtomList data_;

public:
    Msg(const PdArgs& a)
        : BaseObject(a)
        , sel_(&s_anything)
    {
        createInlet();
        createOutlet();

        parseArguments();
        setMethod(args());
    }

    void onBang()
    {
        anyTo(0, sel_, data_);
    }

    void onList(const AtomList& l)
    {
        data_ = prefix_;
        data_.append(l);
        onBang();
    }

    void onAny(t_symbol* sel, const AtomList& l)
    {
        data_ = AtomList(sel);
        data_.append(l);
        onBang();
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n != 1)
            return;

        setMethod(l);
    }

    bool processAnyProps(t_symbol*, const AtomList&)
    {
        return false;
    }

private:
    void setMethod(const AtomList& l)
    {
        if (l.empty())
            return;

        if (!l[0].getSymbol(&sel_)) {
            OBJ_ERR << "first argument is not a symbol: " << l[0];
        } else {
            prefix_ = l.slice(1);
        }
    }
};

extern "C" void msg_setup()
{
    ObjectFactory<Msg> obj("msg");
    obj.addAlias("m");
    obj.mapFloatToList();
    obj.mapSymbolToList();
}
