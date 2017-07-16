#include <boost/algorithm/cxx11/is_sorted.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/piecewise_linear_distribution.hpp>
#include <ctime>

#include "ceammc_factory.h"
#include "random_linseg.h"

static boost::random::mt19937 random_gen(std::time(0));

RandomLinSeg::RandomLinSeg(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    // the interval boundaries interleaved with weights
    createCbProperty("@segs", &RandomLinSeg::propSegs, &RandomLinSeg::propSetSegs);
    if (positionalArguments().size() > 1)
        set(positionalArguments());
}

void RandomLinSeg::onBang()
{
    if (bounds_.size() < 2) {
        OBJ_ERR << "to few values..." << segs_;
        return;
    }

    boost::random::piecewise_linear_distribution<t_float> dist(
        bounds_.begin(), bounds_.end(), weights_.begin());
    floatTo(0, dist(random_gen));
}

void RandomLinSeg::onList(const AtomList& w)
{
    if (!set(w))
        bangTo(0);
}

bool RandomLinSeg::set(const AtomList& data)
{
    if (data.size() % 2 != 0) {
        OBJ_ERR << "expected even number of arguments: boundary0, weight0, boundary1, weight1 etc...";
        return false;
    }

    if (data.size() < 4) {
        OBJ_ERR << "to few values..." << data;
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

    segs_ = data;
    bounds_ = b;
    weights_ = w;
    return true;
}

AtomList RandomLinSeg::propSegs() const
{
    return segs_;
}

void RandomLinSeg::propSetSegs(const AtomList& s)
{
    set(s);
}

extern "C" void setup_random0x2elinseg()
{
    ObjectFactory<RandomLinSeg> obj("random.linseg");
}
