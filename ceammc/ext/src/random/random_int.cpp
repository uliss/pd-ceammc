#include <ctime>
#include <random>

#include "ceammc_factory.h"
#include "fmt/format.h"
#include "random_int.h"

static std::mt19937 random_gen(std::time(0));

RandomInt::RandomInt(const PdArgs& a)
    : BaseObject(a)
    , min_(nullptr)
    , max_(nullptr)
{
    createOutlet();

    min_ = new IntProperty("@min", 0);
    max_ = new IntProperty("@max", 255);

    addProperty(min_);
    addProperty(max_);

    switch (a.args.size()) {
    case 2:
        min_->setArgIndex(0);
        max_->setArgIndex(1);
        break;
    case 1:
        max_->setArgIndex(0);
        break;
    default:
        break;
    }
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
    floatTo(0, dist(random_gen));
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

    RandomInt::setInletsInfo(obj.classPointer(), {"bang: generate new randomly distributed integer"});
    RandomInt::setInletsInfo(obj.classPointer(), {"randomly distributed integer in @min @max range"});
}
