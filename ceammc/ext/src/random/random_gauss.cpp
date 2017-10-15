#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <ctime>

#include "ceammc_factory.h"
#include "random_gauss.h"

extern "C" void setup_random0x2egauss()
{
    ObjectFactory<RandomGauss> obj("random.gauss");
}

RandomGauss::RandomGauss(const PdArgs& a)
    : BaseObject(a)
    , mu_(0)
    , sigma_(0)
{
    createOutlet();

    mu_ = new FloatProperty("@mu", positionalFloatArgument(0, 0));
    sigma_ = new FloatPropertyMinEq("@sigma", positionalFloatArgument(1, 0), 0);

    createProperty(mu_);
    createProperty(sigma_);
}

void RandomGauss::onBang()
{
    static boost::random::mt19937 random_gen(std::time(0));

    boost::random::normal_distribution<t_float> dist(mu_->value(), sigma_->value());
    floatTo(0, dist(random_gen));
}
