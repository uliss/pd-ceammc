#include "random_float.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

RandomFloat::RandomFloat(const PdArgs& a)
    : BaseObject(a)
    , min_(nullptr)
    , max_(nullptr)
    , seed_(nullptr)
{
    createOutlet();

    min_ = new FloatProperty("@min", 0);
    max_ = new FloatProperty("@max", 1);

    addProperty(min_);
    addProperty(max_);

    switch (parsedPosArgs().size()) {
    case 2:
        min_->setArgIndex(0);
        max_->setArgIndex(1);
        break;
    case 1:
        max_->setArgIndex(0);
        break;
    case 0:
        break;
    default:
        OBJ_ERR << "expecting MIN MAX or MAX args";
        break;
    }

    seed_ = new SizeTProperty("@seed", 0);
    seed_->setSuccessFn([this](Property* p) { gen_.setSeed(seed_->value()); });
    addProperty(seed_);
}

void RandomFloat::onBang()
{
    if (min_->value() > max_->value()) {
        LIB_ERR << fmt::format("@min should be less then @max, got: {} > {}",
            min_->value(), max_->value());
        return;
    }

    if (min_->value() == max_->value())
        return floatTo(0, min_->value());

    std::uniform_real_distribution<t_float> dist(min_->value(), max_->value());
    floatTo(0, dist(gen_.get()));
}

void setup_random_float()
{
    ObjectFactory<RandomFloat> obj("random.float");
    obj.addAlias("random.f");

    obj.setDescription("uniform random float generator in specified range");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "random", "float" });
    obj.setCategory("random");
    obj.setSinceVersion(0, 1);

    RandomFloat::setInletsInfo(obj.classPointer(), { "bang" });
    RandomFloat::setOutletsInfo(obj.classPointer(), { "float: random within \\[@min..@max) range" });
}
