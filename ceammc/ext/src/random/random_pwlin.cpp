#include <boost/algorithm/cxx11/is_sorted.hpp> // for is_strictly_increasing

#include "ceammc_factory.h"
#include "random_pwlin.h"

static AtomList vector2list(const std::vector<t_float>& v)
{
    AtomList res;
    res.reserve(v.size());
    for (auto& x : v)
        res.append(x);

    return res;
}

RandomPwLinear::RandomPwLinear(const PdArgs& a)
    : BaseObject(a)
    , seed_(nullptr)
{
    createOutlet();

    createCbListProperty(
        "@v",
        [this]() -> AtomList { return values_; },
        [this](const AtomListView& lv) -> bool { return set(lv); })
        ->setArgIndex(0);

    createCbListProperty("@bounds", [this]() { return vector2list(bounds_); });
    createCbListProperty("@weights", [this]() { return vector2list(weights_); });

    seed_ = new SizeTProperty("@seed", 0);
    seed_->setSuccessFn([this](Property* p) { gen_.setSeed(seed_->value()); });
    addProperty(seed_);
}

void RandomPwLinear::onBang()
{
    if (bounds_.size() < 2) {
        OBJ_ERR << "too few values..." << values_;
        return;
    }

    std::piecewise_linear_distribution<t_float> dist(
        bounds_.begin(), bounds_.end(), weights_.begin());

    floatTo(0, dist(gen_.get()));
}

void RandomPwLinear::onList(const AtomListView& w)
{
    if (set(w))
        onBang();
}

bool RandomPwLinear::set(const AtomListView& data)
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

void setup_random_pw_lin()
{
    ObjectFactory<RandomPwLinear> obj("random.pw_lin");

    obj.setDescription("piecewise linear random distribution");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "linear", "random", "piecewise" });
    obj.setCategory("random");
    obj.setSinceVersion(0, 4);

    RandomPwLinear::setInletsInfo(obj.classPointer(), { "bang: output new random\n"
                                                        "list: set new distribution values and output\n"
                                                        "args: b0 w0 b1 w1..." });
    RandomPwLinear::setOutletsInfo(obj.classPointer(), { "float: piecewise linear distributed random" });
}
