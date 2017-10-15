#include <boost/algorithm/cxx11/is_sorted.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/piecewise_linear_distribution.hpp>
#include <ctime>

#include "ceammc_factory.h"
#include "random_pwlin.h"

static boost::random::mt19937 random_gen(std::time(0));

RandomPwLinear::RandomPwLinear(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    // the interval boundaries interleaved with weights
    createCbProperty("@v", &RandomPwLinear::propValues, &RandomPwLinear::propSetValues);
    if (positionalArguments().size() > 1)
        set(positionalArguments());

    createCbProperty("@bounds", &RandomPwLinear::propBounds);
    createCbProperty("@weights", &RandomPwLinear::propWeights);
}

void RandomPwLinear::onBang()
{
    if (bounds_.size() < 2) {
        OBJ_ERR << "too few values..." << values_;
        return;
    }

    boost::random::piecewise_linear_distribution<t_float> dist(
        bounds_.begin(), bounds_.end(), weights_.begin());

    floatTo(0, dist(random_gen));
}

void RandomPwLinear::onList(const AtomList& w)
{
    if (set(w))
        onBang();
}

bool RandomPwLinear::set(const AtomList& data)
{
    if (data.size() % 2 != 0) {
        OBJ_ERR << "expected even number of arguments: boundary0, weight0, boundary1, weight1 etc...";
        return false;
    }

    if (data.size() < 4) {
        OBJ_ERR << "too few values..." << data;
        return false;
    }

    std::vector<t_float> b;
    std::vector<t_float> w;
    for (size_t i = 0; i < data.size(); i += 2) {
        b.push_back(data[i].asFloat());
        w.push_back(data[i + 1].asFloat());
    }

    if (!boost::algorithm::is_strictly_increasing(b.begin(), b.end())) {
        OBJ_ERR << "boundary values shoud be in strictly increasing order: " << b;
        return false;
    }

    if (std::count_if(w.begin(), w.end(), std::bind2nd(std::less<t_float>(), 0))) {
        OBJ_ERR << "negative weights are found: " << w;
        return false;
    }

    values_ = data;
    bounds_ = b;
    weights_ = w;
    return true;
}

AtomList RandomPwLinear::propValues() const
{
    return values_;
}

void RandomPwLinear::propSetValues(const AtomList& s)
{
    set(s);
}

static AtomList vector2list(const std::vector<t_float>& v)
{
    AtomList res;
    res.reserve(v.size());
    for (size_t i = 0; i < v.size(); i++)
        res.append(v[i]);

    return res;
}

AtomList RandomPwLinear::propBounds() const
{
    return vector2list(bounds_);
}

AtomList RandomPwLinear::propWeights() const
{
    return vector2list(weights_);
}

extern "C" void setup_random0x2epw_lin()
{
    ObjectFactory<RandomPwLinear> obj("random.pw_lin");
}
