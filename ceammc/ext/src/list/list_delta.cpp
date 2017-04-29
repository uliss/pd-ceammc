#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListDelta : public BaseObject {
    AtomList prev_list_;
    AtomList delta_list_;
    SymbolEnumProperty* wrap_method_;
    t_symbol* gmin_;
    t_symbol* gpadz_;
    t_symbol* gclip_;
    t_symbol* gwrap_;
    t_symbol* gfold_;

public:
    ListDelta(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
        wrap_method_ = new SymbolEnumProperty("@oversize", "padz");
        wrap_method_->appendEnum("min");
        wrap_method_->appendEnum("clip");
        wrap_method_->appendEnum("fold");
        wrap_method_->appendEnum("wrap");
        createProperty(wrap_method_);

        gmin_ = gensym("min");
        gpadz_ = gensym("padz");
        gclip_ = gensym("clip");
        gwrap_ = gensym("wrap");
        gfold_ = gensym("fold");

        createProperty(new SymbolEnumAlias("@min", wrap_method_, gmin_));
        createProperty(new SymbolEnumAlias("@padz", wrap_method_, gpadz_));
        createProperty(new SymbolEnumAlias("@clip", wrap_method_, gclip_));
        createProperty(new SymbolEnumAlias("@wrap", wrap_method_, gwrap_));
        createProperty(new SymbolEnumAlias("@fold", wrap_method_, gfold_));
    }

    AtomList::NonEqualLengthBehaivor symbolToWrap(t_symbol* s)
    {
        if (s == gmin_)
            return AtomList::MINSIZE;
        else if (s == gpadz_)
            return AtomList::PADZERO;
        else if (s == gclip_)
            return AtomList::CLIP;
        else if (s == gwrap_)
            return AtomList::WRAP;
        else if (s == gfold_)
            return AtomList::FOLD;
        else
            return AtomList::MINSIZE;
    }

    void onBang()
    {
        listTo(0, delta_list_);
    }

    void onList(const AtomList& l)
    {
        delta_list_ = AtomList::sub(l, prev_list_, symbolToWrap(wrap_method_->value()));
        prev_list_ = l;

        listTo(0, delta_list_);
    }

    void m_clear(t_symbol*, const AtomList&)
    {
        prev_list_.clear();
        delta_list_.clear();
    }

    void dump() const
    {
        BaseObject::dump();
        OBJ_DBG << "current value: " << prev_list_;
        OBJ_DBG << "delta:         " << delta_list_;
    }
};

extern "C" void setup_list0x2edelta()
{
    ObjectFactory<ListDelta> obj("list.delta");
    obj.addMethod("clear", &ListDelta::m_clear);
}
