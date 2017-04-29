//
//  list_deinterleave.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 23/12/16.
//
//

#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

#include <cassert>

using namespace ceammc;

static const size_t MIN_OUTLET = 2;
static const size_t MAX_OUTLET = 20;

class ListDeinterleave : public BaseObject {
    const size_t out_count_;
    AtomList in_list_;
    std::vector<AtomList> out_lists_;
    Atom pad_;
    SymbolEnumProperty* method_;
    t_symbol* gmin_;
    t_symbol* gpad_;

public:
    ListDeinterleave(const PdArgs& a)
        : BaseObject(a)
        , out_count_(math::clip(atomlistToValue<size_t>(a.args, MIN_OUTLET), MIN_OUTLET, MAX_OUTLET))
        , pad_(0.f)
        , method_(0)
        , gmin_(gensym("min"))
        , gpad_(gensym("pad"))
    {
        initOutlets();
        initProperties();
    }

    void onBang()
    {
        for (size_t i = out_lists_.size(); i > 0; i--)
            listTo(i - 1, out_lists_[i - 1]);
    }

    void onList(const AtomList& l)
    {
        clearOutputList();

        if (method_->value() == gmin_)
            list::deinterleaveMinLength(l, out_lists_);
        else if (method_->value() == gpad_)
            list::deinterleavePadWith(l, pad_, out_lists_);

        onBang();
    }

private:
    void clearOutputList()
    {
        for (size_t i = 0; i < out_lists_.size(); i++)
            out_lists_[i].clear();
    }

    void initOutlets()
    {
        for (size_t i = 0; i < out_count_; i++) {
            out_lists_.push_back(AtomList());
            createOutlet();
        }
    }

    void initProperties()
    {
        method_ = new SymbolEnumProperty("@method", "min");
        method_->appendEnum("pad");
        createProperty(method_);
        createProperty(new SymbolEnumAlias("@min", method_, gmin_));
        createCbProperty("@pad", &ListDeinterleave::getPadValue, &ListDeinterleave::setPadValue);
    }

    AtomList getPadValue() const
    {
        return listFrom(pad_);
    }

    void setPadValue(const AtomList& l)
    {
        pad_ = atomlistToValue<Atom>(l, Atom(0.f));
        method_->setValue(gpad_);
    }
};

extern "C" void setup_list0x2edeinterleave()
{
    ObjectFactory<ListDeinterleave> obj("list.deinterleave");
}
