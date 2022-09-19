#include "random_float.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

RandomFloat::RandomFloat(const PdArgs& a)
    : BaseObject(a)
    , min_(nullptr)
    , max_(nullptr)
{
    createInlet();
    createInlet();
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

    addProperty(new random::SeedProperty(gen_));
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

void RandomFloat::m_gen(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, s))
        return;

    const auto n = lv[0].asT<int>();

    AtomList res;
    res.reserve(n);

    std::uniform_real_distribution<t_float> dist(min_->value(), max_->value());

    for (int i = 0; i < n; i++)
        res.append(dist(gen_.get()));

    listTo(0, res);
}

void RandomFloat::onInlet(size_t n, const AtomListView& lv)
{
    switch (n) {
    case 1:

        if (checkArgs(lv, ARG_FLOAT, ARG_FLOAT)) {
            min_->setFloat(lv[0].asT<t_float>());
            max_->setFloat(lv[1].asT<t_float>());
        } else
            min_->setList(lv);

        break;
    case 2:
        max_->setList(lv);
        break;
    }
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

    obj.addMethod("gen", &RandomFloat::m_gen);

    RandomFloat::setInletsInfo(obj.classPointer(), { "bang", "float: set min value", "float: set max value" });
    RandomFloat::setOutletsInfo(obj.classPointer(), { "float: random within \\[@min..@max) range" });
}
