#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

static t_symbol* SYM_MIN;
static t_symbol* SYM_PADZ;
static t_symbol* SYM_CLIP;
static t_symbol* SYM_WRAP;
static t_symbol* SYM_FOLD;

class ListDelta : public BaseObject {
    AtomList prev_list_;
    AtomList delta_list_;
    SymbolEnumProperty* wrap_method_;

public:
    ListDelta(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
        wrap_method_ = new SymbolEnumProperty("@oversize", SYM_PADZ);
        wrap_method_->appendEnum(SYM_MIN);
        wrap_method_->appendEnum(SYM_CLIP);
        wrap_method_->appendEnum(SYM_FOLD);
        wrap_method_->appendEnum(SYM_WRAP);
        createProperty(wrap_method_);

        createProperty(new SymbolEnumAlias("@min", wrap_method_, SYM_MIN));
        createProperty(new SymbolEnumAlias("@padz", wrap_method_, SYM_PADZ));
        createProperty(new SymbolEnumAlias("@clip", wrap_method_, SYM_CLIP));
        createProperty(new SymbolEnumAlias("@wrap", wrap_method_, SYM_WRAP));
        createProperty(new SymbolEnumAlias("@fold", wrap_method_, SYM_FOLD));
    }

    AtomList::NonEqualLengthBehaivor symbolToWrap(t_symbol* s)
    {
        if (s == SYM_MIN)
            return AtomList::MINSIZE;
        else if (s == SYM_PADZ)
            return AtomList::PADZERO;
        else if (s == SYM_CLIP)
            return AtomList::CLIP;
        else if (s == SYM_WRAP)
            return AtomList::WRAP;
        else if (s == SYM_FOLD)
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
    SYM_MIN = gensym("min");
    SYM_PADZ = gensym("padz");
    SYM_CLIP = gensym("clip");
    SYM_WRAP = gensym("wrap");
    SYM_FOLD = gensym("fold");

    ObjectFactory<ListDelta> obj("list.delta");
    obj.addMethod("clear", &ListDelta::m_clear);
}
