#include "list_resize.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

static const size_t MAX_SIZE = 1024;

CEAMMC_DEFINE_SYM(pad);
CEAMMC_DEFINE_SYM(clip);
CEAMMC_DEFINE_SYM(wrap);
CEAMMC_DEFINE_SYM(fold);

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

    if (m == sym_pad()) {
        tmp.resizePad(n, pad_);
    } else if (m == sym_clip()) {
        tmp.resizeClip(n);
    } else if (m == sym_wrap()) {
        tmp.resizeWrap(n);
    } else if (m == sym_fold()) {
        tmp.resizeFold(n);
    }

    listTo(0, tmp);
}

void ListResize::initProperties()
{
    constexpr size_t DEFAULT_SIZE = 0;
    size_ = new IntProperty("@size", DEFAULT_SIZE);
    size_->checkMinEq(0);
    size_->setArgIndex(0);
    addProperty(size_);

    // resize methods:
    // @clip - pad with last element (by default)
    // @pad - pad with specified value (@pad_value property)
    // @wrap - pad with wrapped values
    // @fold - pad with fold values
    method_ = new SymbolEnumProperty("@method", { sym_pad(), sym_clip(), sym_wrap(), sym_fold() });
    addProperty(method_);

    // adding aliases
    addProperty(new SymbolEnumAlias("@clip", method_, sym_clip()));
    addProperty(new SymbolEnumAlias("@wrap", method_, sym_wrap()));
    addProperty(new SymbolEnumAlias("@fold", method_, sym_fold()));

    createCbAtomProperty(
        "@pad",
        [this]() -> Atom { return pad_; },
        [this](const Atom& a) -> bool {
            pad_ = a;
            method_->setValue(sym_pad());
            return true; });
}

void setup_list_resize()
{
    ObjectFactory<ListResize> obj("list.resize");

    obj.setDescription("changes list size");
    obj.setCategory("list");
    obj.setKeywords({ "list", "size", "resize" });
}
