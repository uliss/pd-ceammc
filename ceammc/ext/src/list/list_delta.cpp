#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

CEAMMC_DEFINE_SYM_HASH(min);
CEAMMC_DEFINE_SYM_HASH(padz);
CEAMMC_DEFINE_SYM_HASH(clip);
CEAMMC_DEFINE_SYM_HASH(wrap);
CEAMMC_DEFINE_SYM_HASH(fold);

class ListDelta : public BaseObject {
    AtomList prev_list_;
    AtomList delta_list_;
    SymbolEnumProperty* wrap_method_;

public:
    ListDelta(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
        wrap_method_ = new SymbolEnumProperty("@oversize", { str_padz, str_min, str_clip, str_fold, str_wrap });
        addProperty(wrap_method_);

        addProperty(new SymbolEnumAlias("@min", wrap_method_, sym_min()));
        addProperty(new SymbolEnumAlias("@padz", wrap_method_, sym_padz()));
        addProperty(new SymbolEnumAlias("@clip", wrap_method_, sym_clip()));
        addProperty(new SymbolEnumAlias("@wrap", wrap_method_, sym_wrap()));
        addProperty(new SymbolEnumAlias("@fold", wrap_method_, sym_fold()));
    }

    AtomList::NonEqualLengthBehaivor symbolToWrap(t_symbol* s)
    {
        switch (crc32_hash(s)) {
        case hash_min:
            return AtomList::MINSIZE;
        case hash_padz:
            return AtomList::PADZERO;
        case hash_clip:
            return AtomList::CLIP;
        case hash_wrap:
            return AtomList::WRAP;
        case hash_fold:
            return AtomList::FOLD;
        default:
            return AtomList::MINSIZE;
        }
    }

    void onBang()
    {
        listTo(0, delta_list_);
    }

    void onList(const AtomListView& lv)
    {
        delta_list_ = AtomList::sub(lv, prev_list_, symbolToWrap(wrap_method_->value()));
        prev_list_ = lv;

        listTo(0, delta_list_);
    }

    void m_clear(t_symbol*, const AtomListView&)
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

void setup_list_delta()
{
    ObjectFactory<ListDelta> obj("list.delta");
    obj.addMethod("clear", &ListDelta::m_clear);
}
