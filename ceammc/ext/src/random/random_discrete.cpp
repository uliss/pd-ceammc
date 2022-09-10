#include "random_discrete.h"
#include "ceammc_factory.h"

static AtomList vector2list(const std::vector<t_float>& v)
{
    AtomList res;
    res.reserve(v.size());
    for (size_t i = 0; i < v.size(); i++)
        res.append(v[i]);

    return res;
}

RandomDiscrete::RandomDiscrete(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
    weights_.reserve(16);

    // the interval boundaries interleaved with weights
    createCbListProperty(
        "@weights",
        [this]() -> AtomList { return vector2list(weights_); },
        [this](const AtomListView& lv) -> bool { return set(lv); })
        ->setArgIndex(0);

    addProperty(new random::SeedProperty(gen_));
}

void RandomDiscrete::onBang()
{
    if (weights_.size() < 1) {
        OBJ_ERR << "no elements to output";
        return;
    }

    std::discrete_distribution<size_t> dist(weights_.begin(), weights_.end());
    floatTo(0, dist(gen_.get()));
}

void RandomDiscrete::onList(const AtomListView& lv)
{
    if (set(lv))
        onBang();
}

bool RandomDiscrete::set(const AtomListView& lv)
{
    if (lv.empty()) {
        OBJ_ERR << "empty list";
        return false;
    }

    if (lv.anyOf([](const Atom& a) { return !a.isFloat() || a.asT<t_float>() < 0; })) {
        OBJ_ERR << "invalid weight values: " << lv;
        return false;
    }

    weights_.clear();
    for (size_t i = 0; i < lv.size(); i++) {
        weights_.push_back(lv[i].asT<t_float>());
    }

    return true;
}

void setup_random_discrete()
{
    ObjectFactory<RandomDiscrete> obj("random.discrete");

    obj.setDescription("random weighted integers on interval [0, n)");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "linear", "random", "discrete" });
    obj.setCategory("random");
    obj.setSinceVersion(0, 4);

    RandomDiscrete::setInletsInfo(obj.classPointer(), { "bang: output new random\n"
                                                        "list: set new weights and output\n"
                                                        "args: w0 w1 w2..." });
    RandomDiscrete::setOutletsInfo(obj.classPointer(), { "int: random weighted integer on interval \\[0, n)" });
}
