#include "list_resize.h"
#include "ceammc_factory.h"

static const size_t MAX_SIZE = 1024;

static t_symbol* SYM_PAD;
static t_symbol* SYM_CLIP;
static t_symbol* SYM_WRAP;
static t_symbol* SYM_FOLD;

ListResize::ListResize(const PdArgs& a)
    : BaseObject(a)
    , size_(nullptr)
    , method_(nullptr)
    , pad_(0.f)
{
    createInlet();
    createOutlet();
    initProperties();
}

void ListResize::onInlet(size_t n, const AtomListView& lv)
{
    if (n != 1)
        return;

    size_->set(lv);
}

void ListResize::onList(const AtomListView& lv)
{
    AtomList tmp(lv);
    const t_symbol* m = method_->value();
    const size_t n = size_->value();

    if (n > MAX_SIZE) {
        OBJ_ERR << "size is too big: " << n;
        return;
    }

    if (m == SYM_PAD) {
        tmp.resizePad(n, pad_);
    } else if (m == SYM_CLIP) {
        tmp.resizeClip(n);
    } else if (m == SYM_WRAP) {
        tmp.resizeWrap(n);
    } else if (m == SYM_FOLD) {
        tmp.resizeFold(n);
    }

    listTo(0, tmp);
}

void ListResize::initProperties()
{
    constexpr size_t DEFAULT_SIZE = 0;
    size_ = new SizeTProperty("@size", DEFAULT_SIZE);
    size_->setArgIndex(0);
    addProperty(size_);

    // resize methods:
    // @clip - pad with last element (by default)
    // @pad - pad with specified value (@pad_value property)
    // @wrap - pad with wrapped values
    // @fold - pad with fold values
    method_ = new SymbolEnumProperty("@method", { SYM_PAD, SYM_CLIP, SYM_WRAP, SYM_FOLD });
    addProperty(method_);

    // adding aliases
    addProperty(new SymbolEnumAlias("@pad", method_, SYM_PAD));
    addProperty(new SymbolEnumAlias("@clip", method_, SYM_CLIP));
    addProperty(new SymbolEnumAlias("@wrap", method_, SYM_WRAP));
    addProperty(new SymbolEnumAlias("@fold", method_, SYM_FOLD));

    createCbAtomProperty(
        "@pad",
        [this]() -> Atom { return pad_; },
        [this](const Atom& a) -> bool {
            pad_ = a;
            method_->setValue(SYM_PAD);
            return true; });
}

void setup_list_resize()
{
    SYM_PAD = gensym("pad");
    SYM_CLIP = gensym("clip");
    SYM_WRAP = gensym("wrap");
    SYM_FOLD = gensym("fold");

    ObjectFactory<ListResize> obj("list.resize");

    obj.setDescription("changes list size");
    obj.setCategory("list");
    obj.setKeywords({"list", "size", "resize"});
}
