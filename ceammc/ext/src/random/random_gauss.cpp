#include <ctime>
#include <random>

#include "ceammc_factory.h"
#include "random_gauss.h"

RandomGauss::RandomGauss(const PdArgs& a)
    : BaseObject(a)
    , mu_(nullptr)
    , sigma_(nullptr)
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
    static std::mt19937 random_gen(std::time(0));

    std::normal_distribution<t_float> dist(mu_->value(), sigma_->value());
    floatTo(0, dist(random_gen));
}

void setup_random_gauss()
{
    ObjectFactory<RandomGauss> obj("random.gauss");
}
