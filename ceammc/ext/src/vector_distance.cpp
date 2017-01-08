#include "ceammc_factory.h"
#include "ceammc_fn_vector.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class VectorDistance : public BaseObject {
    AtomList a_;
    AtomList b_;
    float distance_;

public:
    VectorDistance(const PdArgs& a)
        : BaseObject(a)
        , distance_(0.f)
    {
        createInlet();
        createOutlet();
    }

    void onBang()
    {
        floatTo(0, distance_);
    }

    void onList(const AtomList& l)
    {
        a_ = l;
        distance_ = static_cast<t_float>(vector::distance(a_, b_));
        onBang();
    }

    void onInlet(size_t, const AtomList& l)
    {
        b_ = l;
    }

    void dump() const
    {
        BaseObject::dump();
        OBJ_DBG << "first point:  " << a_;
        OBJ_DBG << "second point: " << b_;
        OBJ_DBG << "distance:     " << distance_;
    }
};

extern "C" void setup_vector0x2edistance()
{
    ObjectFactory<VectorDistance> obj("vector.distance");
}
