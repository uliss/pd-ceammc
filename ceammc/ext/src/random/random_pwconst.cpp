#include <boost/algorithm/cxx11/is_sorted.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/piecewise_constant_distribution.hpp>
#include <ctime>

#include "ceammc_factory.h"
#include "random_pwconst.h"

static boost::random::mt19937 random_gen(std::time(0));

RandomPWConst::RandomPWConst(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    // the interval boundaries interleaved with weights
    createCbProperty("@v", &RandomPWConst::propValues, &RandomPWConst::propSetValues);
    if (positionalArguments().size() > 4)
        set(positionalArguments());

    createCbProperty("@bounds", &RandomPWConst::propBounds);
    createCbProperty("@weights", &RandomPWConst::propWeights);
}

void RandomPWConst::onBang()
{
    if (bounds_.size() < 3) {
        OBJ_ERR << "too few values..." << values_;
        return;
    }

    boost::random::piecewise_constant_distribution<t_float> dist(
        bounds_.begin(), bounds_.end(), weights_.begin());

    floatTo(0, dist(random_gen));
}

void RandomPWConst::onList(const AtomList& v)
{
    if (set(v))
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

AtomList RandomPWConst::propBounds() const
{
    return vector2list(bounds_);
}

AtomList RandomPWConst::propWeights() const
{
    return vector2list(weights_);
}

AtomList RandomPWConst::propValues() const
{
    return values_;
}

void RandomPWConst::propSetValues(const AtomList& s)
{
    set(s);
}

bool RandomPWConst::set(const AtomList& data)
{
    if (data.size() % 2 == 0) {
        OBJ_ERR << "expected odd number of arguments: boundary0, weight0, boundary1 etc...";
        return false;
    }

    if (data.size() < 5) {
        OBJ_ERR << "too few values..." << data;
        return false;
    }

    std::vector<t_float> b;
    std::vector<t_float> w;
    for (size_t i = 0; i < data.size(); i++) {
        b.push_back(data[i].asFloat());
        if (++i < data.size()) {
            w.push_back(data[i].asFloat());
        }
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

extern "C" void setup_random0x2epw_const()
{
    ObjectFactory<RandomPWConst> obj("random.pw_const");
}
