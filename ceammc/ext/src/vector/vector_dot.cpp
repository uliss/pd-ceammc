#include "ceammc_factory.h"
#include "ceammc_fn_vector.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class VectorDotProduct : public BaseObject {
    AtomList a_;
    AtomList b_;
    float dot_product_;

public:
    VectorDotProduct(const PdArgs& a)
        : BaseObject(a)
        , dot_product_(0.f)
    {
        createInlet();
        createOutlet();

        b_ = positionalArguments();
    }

    void onBang()
    {
        floatTo(0, dot_product_);
    }

    void onList(const AtomList& l)
    {
        a_ = l;
        dot_product_ = static_cast<t_float>(vector::dotProduct(a_, b_));
        onBang();
    }

    void onInlet(size_t, const AtomList& l)
    {
        b_ = l;
    }

    void dump() const
    {
        BaseObject::dump();
        OBJ_DBG << "first vector:  " << a_;
        OBJ_DBG << "second vector: " << b_;
        OBJ_DBG << "dot product:   " << dot_product_;
    }
};

extern "C" void setup_vector0x2edot()
{
    ObjectFactory<VectorDotProduct> obj("vector.dot");
}
