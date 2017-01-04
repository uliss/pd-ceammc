#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListResize : public BaseObject {
    SizeTProperty* size_;
    SymbolEnumProperty* method_;
    AtomProperty* pad_value_;
    t_symbol* gpad_;
    t_symbol* gclip_;
    t_symbol* gwrap_;
    t_symbol* gfold_;

public:
    ListResize(const PdArgs& a)
        : BaseObject(a)
        , size_(0)
        , method_(0)
        , pad_value_(0)
        , gpad_(0)
        , gclip_(0)
        , gwrap_(0)
        , gfold_(0)
    {
        createInlet();
        createOutlet();

        size_ = new SizeTProperty("@size", 0, false);
        createProperty(size_);

        // resize methods:
        // @clip - pad with last element (by default)
        // @pad - pad with specified value (@pad_value property)
        // @wrap - pad with wrapped values
        // @fold - pad with fold values
        method_ = new SymbolEnumProperty("@method", "pad");
        method_->appendEnum("clip");
        method_->appendEnum("wrap");
        method_->appendEnum("fold");
        createProperty(method_);

        // by default pad with zero
        pad_value_ = new AtomProperty("@pad_value", Atom(0.f));
        createProperty(pad_value_);

        // init t_symbol* pointer members for fast compare
        gpad_ = gensym("pad");
        gclip_ = gensym("clip");
        gwrap_ = gensym("wrap");
        gfold_ = gensym("fold");

        // adding aliases
        createProperty(new SymbolEnumAlias("@pad", method_, gpad_));
        createProperty(new SymbolEnumAlias("@clip", method_, gclip_));
        createProperty(new SymbolEnumAlias("@wrap", method_, gwrap_));
        createProperty(new SymbolEnumAlias("@fold", method_, gfold_));

        // parse creation arguments and properties
        parseArguments();

        // use first non property argument for new size, if not set before
        if (size_->value() == 0 && args().size() > 0)
            size_->setValue(args().first()->asSizeT(0));
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n != 1)
            return;

        size_->set(l);
    }

    void onList(const AtomList& l)
    {
        AtomList tmp(l);
        const t_symbol* m = method_->value();
        const size_t n = size_->value();

        if (m == gpad_) {
            tmp.resizePad(n, pad_value_->value());
        } else if (m == gclip_) {
            tmp.resizeClip(n);
        } else if (m == gwrap_) {
            tmp.resizeWrap(n);
        } else if (m == gfold_) {
            tmp.resizeFold(n);
        }

        listTo(0, tmp);
    }
};

extern "C" void setup_list0x2eresize()
{
    ObjectFactory<ListResize> obj("list.resize");
}
