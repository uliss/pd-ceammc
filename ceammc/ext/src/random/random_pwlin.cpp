#include <boost/algorithm/cxx11/is_sorted.hpp> // for is_strictly_increasing
#include <ctime>
#include <random>

#include "ceammc_factory.h"
#include "random_pwlin.h"

static std::mt19937 random_gen(std::time(0));

RandomPwLinear::RandomPwLinear(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    createCbListProperty(
        "@v",
        [this]() -> AtomList { return values_; },
        [this](const AtomList& l) -> bool { return set(l); })
        ->setArgIndex(0);

    createCbProperty("@bounds", &RandomPwLinear::propBounds);
    createCbProperty("@weights", &RandomPwLinear::propWeights);
}

void RandomPwLinear::onBang()
{
    if (bounds_.size() < 2) {
        OBJ_ERR << "too few values..." << values_;
        return;
    }

    std::piecewise_linear_distribution<t_float> dist(
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

    if (std::count_if(w.begin(), w.end(), [](t_float f) { return f < 0; })) {
        OBJ_ERR << "negative weights are found: " << w;
        return false;
    }

    values_ = data;
    bounds_ = b;
    weights_ = w;
    return true;
}

static AtomList vector2list(const std::vector<t_float>& v)
{
    AtomList res;
    res.reserve(v.size());
    for (auto& x : v)
        res.append(x);

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

void setup_random_pw_lin()
{
    ObjectFactory<RandomPwLinear> obj("random.pw_lin");
}
