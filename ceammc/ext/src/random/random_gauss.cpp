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

    mu_ = new FloatProperty("@mu", 0);
    mu_->setArgIndex(0);
    sigma_ = new FloatProperty("@sigma", 0);
    sigma_->setArgIndexNext(mu_);
    sigma_->checkNonNegative();

    addProperty(mu_);
    addProperty(sigma_);
}

void RandomGauss::onBang()
{
    static boost::random::mt19937 random_gen(std::time(0));

    boost::random::normal_distribution<t_float> dist(mu_->value(), sigma_->value());
    floatTo(0, dist(random_gen));
}
