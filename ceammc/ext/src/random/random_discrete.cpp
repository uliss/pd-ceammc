#include "random_discrete.h"
#include "ceammc_factory.h"

RandomDiscrete::RandomDiscrete(const PdArgs& a)
    : BaseObject(a)
    , seed_(nullptr)
{
    createOutlet();

    // the interval boundaries interleaved with weights
    createCbListProperty(
        "@weights",
        [this]() -> AtomList { return propWeights(); },
        [this](const AtomList& l) -> bool { setPropWeights(l); return true; })
        ->setArgIndex(0);

    seed_ = new SizeTProperty("@seed", 0);
    seed_->setSuccessFn([this](Property* p) { gen_.setSeed(seed_->value()); });
    addProperty(seed_);
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

void RandomDiscrete::onList(const AtomList& l)
{
    if (set(l))
        onBang();
}

static AtomList vector2list(const std::vector<t_float>& v)
{
    AtomList res;
    res.reserve(v.size());
    for (size_t i = 0; i < v.size(); i++)
        res.append(v[i]);

    return res;
}

AtomList RandomDiscrete::propWeights() const
{
    return vector2list(weights_);
}

void RandomDiscrete::setPropWeights(const AtomList& l)
{
    set(l);
}

static bool invalidWeight(const Atom& a)
{
    return !a.isFloat() || a.asFloat() < 0;
}

bool RandomDiscrete::set(const AtomList& l)
{
    if (l.empty()) {
        OBJ_ERR << "empty list";
        return false;
    }

    if (l.anyOf(invalidWeight)) {
        OBJ_ERR << "invalid weight values: " << l;
        return false;
    }

    std::vector<t_float> w;
    for (size_t i = 0; i < l.size(); i++) {
        w.push_back(l[i].asFloat());
    }

    weights_ = w;
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
