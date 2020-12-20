#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

static t_symbol* SYM_MIN;
static t_symbol* SYM_PADZ;
static t_symbol* SYM_CLIP;
static t_symbol* SYM_WRAP;
static t_symbol* SYM_FOLD;

class ListIntegrator : public BaseObject {
    AtomList integral_list_;
    SymbolEnumProperty* wrap_method_;

public:
    ListIntegrator(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
        wrap_method_ = new SymbolEnumProperty("@oversize", { SYM_PADZ, SYM_MIN, SYM_CLIP, SYM_FOLD, SYM_WRAP });
        addProperty(wrap_method_);

        addProperty(new SymbolEnumAlias("@min", wrap_method_, SYM_MIN));
        addProperty(new SymbolEnumAlias("@padz", wrap_method_, SYM_PADZ));
        addProperty(new SymbolEnumAlias("@clip", wrap_method_, SYM_CLIP));
        addProperty(new SymbolEnumAlias("@wrap", wrap_method_, SYM_WRAP));
        addProperty(new SymbolEnumAlias("@fold", wrap_method_, SYM_FOLD));
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
        listTo(0, integral_list_);
    }

    void onList(const AtomList& l)
    {
        integral_list_ = AtomList::add(l, integral_list_, symbolToWrap(wrap_method_->value()));
        listTo(0, integral_list_);
    }

    void m_clear(t_symbol*, const AtomListView&)
    {
        integral_list_.clear();
    }

    void dump() const
    {
        BaseObject::dump();
        OBJ_DBG << "integral sum:  " << integral_list_;
    }
};

void setup_list_integrator()
{
    SYM_MIN = gensym("min");
    SYM_PADZ = gensym("padz");
    SYM_CLIP = gensym("clip");
    SYM_WRAP = gensym("wrap");
    SYM_FOLD = gensym("fold");

    ObjectFactory<ListIntegrator> obj("list.integrator");
    obj.addMethod("clear", &ListIntegrator::m_clear);
}
