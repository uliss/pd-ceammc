#include "random_int.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

RandomInt::RandomInt(const PdArgs& a)
    : BaseObject(a)
    , min_(nullptr)
    , max_(nullptr)
    , seed_(nullptr)
{
    createOutlet();

    min_ = new IntProperty("@min", 0);
    max_ = new IntProperty("@max", 255);

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

void RandomInt::onBang()
{
    if (min_->value() > max_->value()) {
        LIB_ERR << fmt::format("@min should be less then @max, got: {} > {}",
            min_->value(), max_->value());
        return;
    }

    if (min_->value() == max_->value())
        return floatTo(0, min_->value());

    std::uniform_int_distribution<int> dist(min_->value(), max_->value());
    floatTo(0, dist(gen_.get()));
}

void setup_random_int()
{
    ObjectFactory<RandomInt> obj("random.int");
    obj.addAlias("random.i");

    obj.setDescription("uniform random integer generator in specified range");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "random", "int" });
    obj.setCategory("random");
    obj.setSinceVersion(0, 1);

    RandomInt::setInletsInfo(obj.classPointer(), { "bang" });
    RandomInt::setOutletsInfo(obj.classPointer(), { "int: random in \\[@min..@max\\] range" });
}
