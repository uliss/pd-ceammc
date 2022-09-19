#include "random_int.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

RandomInt::RandomInt(const PdArgs& a)
    : BaseObject(a)
    , min_(nullptr)
    , max_(nullptr)
{
    createInlet();
    createInlet();
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

    addProperty(new random::SeedProperty(gen_));
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

void RandomInt::onInlet(size_t n, const AtomListView& lv)
{
    switch (n) {
    case 1:

        if (checkArgs(lv, ARG_FLOAT, ARG_FLOAT)) {
            min_->setInt(lv[0].asT<int>());
            max_->setInt(lv[1].asT<int>());
        } else
            min_->setList(lv);

        break;
    case 2:
        max_->setList(lv);
        break;
    }
}

void RandomInt::m_gen(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, s))
        return;

    const auto n = lv[0].asT<int>();

    AtomList res;
    res.reserve(n);

    std::uniform_int_distribution<int> dist(min_->value(), max_->value());

    for (int i = 0; i < n; i++)
        res.append(dist(gen_.get()));

    listTo(0, res);
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

    obj.addMethod("gen", &RandomInt::m_gen);

    RandomInt::setInletsInfo(obj.classPointer(), { "bang", "int: set min value", "int: set max value" });
    RandomInt::setOutletsInfo(obj.classPointer(), { "int: random in \\[@min..@max\\] range" });
}
