#include <boost/random/discrete_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <ctime>

#include "ceammc_factory.h"
#include "random_discrete.h"

static boost::random::mt19937 random_gen(std::time(0));

RandomDiscrete::RandomDiscrete(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    // the interval boundaries interleaved with weights
    createCbProperty("@weights", &RandomDiscrete::propWeights, &RandomDiscrete::setPropWeights);
    if (!positionalArguments().empty())
        set(positionalArguments());
}

void RandomDiscrete::onBang()
{
    if (weights_.size() < 1) {
        OBJ_ERR << "no elements to output";
        return;
    }

    boost::random::discrete_distribution<size_t> dist(weights_.begin(), weights_.end());
    floatTo(0, dist(random_gen));
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

extern "C" void setup_random0x2ediscrete()
{
    ObjectFactory<RandomDiscrete> obj("random.discrete");
}
