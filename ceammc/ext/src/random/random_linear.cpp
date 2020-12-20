#include "random_linear.h"
#include "ceammc_factory.h"

RandomLinear::RandomLinear(const PdArgs& a)
    : BaseObject(a)
    , v0_(0)
    , v1_(0)
    , p0_(0)
    , p1_(0)
    , seed_(nullptr)
{
    createOutlet();

    v0_ = new FloatProperty("@v0", 0);
    v1_ = new FloatProperty("@v1", 1);
    p0_ = new FloatProperty("@p0", 1);
    p1_ = new FloatProperty("@p1", 0);

    addProperty(v0_);
    addProperty(v1_);
    addProperty(p0_);
    addProperty(p1_);

    seed_ = new SizeTProperty("@seed", 0);
    seed_->setSuccessFn([this](Property* p) { gen_.setSeed(seed_->value()); });
    addProperty(seed_);
}

void RandomLinear::onBang()
{
    const t_float v0 = v0_->value();
    const t_float v1 = v1_->value();

    if (v0 >= v1) {
        OBJ_ERR << "invalid value range: " << v0 << "-" << v1;
        return;
    }

    t_float intervals[] = { v0, v1 };
    t_float weights[] = { p0_->value(), p1_->value() };

    std::piecewise_linear_distribution<t_float> dist(intervals, intervals + 2, weights);
    floatTo(0, dist(gen_.get()));
}

void setup_random_linear()
{
    ObjectFactory<RandomLinear> obj("random.linear");

    obj.setDescription("random linear distribution");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "linear", "random" });
    obj.setCategory("random");
    obj.setSinceVersion(0, 4);

    RandomLinear::setInletsInfo(obj.classPointer(), { "bang" });
    RandomLinear::setOutletsInfo(obj.classPointer(), { "float: random in \\[@v0..@v1) range\n"
                                                       "       with linear probs in \\[@p0..@p1)" });
}
