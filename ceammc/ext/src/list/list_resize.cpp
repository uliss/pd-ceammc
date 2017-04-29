#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

static const size_t MAX_SIZE = 1024;

class ListResize : public BaseObject {
    SizeTProperty* size_;
    SymbolEnumProperty* method_;
    Atom pad_;
    t_symbol* gpad_;
    t_symbol* gclip_;
    t_symbol* gwrap_;
    t_symbol* gfold_;

public:
    ListResize(const PdArgs& a)
        : BaseObject(a)
        , size_(0)
        , method_(0)
        , pad_(0.f)
        , gpad_(gensym("pad"))
        , gclip_(gensym("clip"))
        , gwrap_(gensym("wrap"))
        , gfold_(gensym("fold"))
    {
        createInlet();
        createOutlet();

        initProperties();
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

        if (n > MAX_SIZE) {
            OBJ_ERR << "size is too big: " << n;
            return;
        }

        if (m == gpad_) {
            tmp.resizePad(n, pad_);
        } else if (m == gclip_) {
            tmp.resizeClip(n);
        } else if (m == gwrap_) {
            tmp.resizeWrap(n);
        } else if (m == gfold_) {
            tmp.resizeFold(n);
        }

        listTo(0, tmp);
    }

private:
    void initProperties()
    {
        size_ = new SizeTProperty("@size", size_t(positionalFloatArgument(0, 0)), false);
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

        // adding aliases
        createProperty(new SymbolEnumAlias("@pad", method_, gpad_));
        createProperty(new SymbolEnumAlias("@clip", method_, gclip_));
        createProperty(new SymbolEnumAlias("@wrap", method_, gwrap_));
        createProperty(new SymbolEnumAlias("@fold", method_, gfold_));

        createCbProperty("@pad", &ListResize::getPadValue, &ListResize::setPadValue);
    }

    AtomList getPadValue() const { return listFrom(pad_); }

    void setPadValue(const AtomList& l)
    {
        pad_ = atomlistToValue<Atom>(l, Atom(0.f));
        method_->setValue(gpad_);
    }
};

extern "C" void setup_list0x2eresize()
{
    ObjectFactory<ListResize> obj("list.resize");
}
