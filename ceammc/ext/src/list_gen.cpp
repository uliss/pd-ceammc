#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

static const size_t MAX_COUNT = 1024;

class ListGenerate : public BaseObject {
    AtomList gen_values_;
    size_t count_;

public:
    ListGenerate(const PdArgs& a)
        : BaseObject(a)
        , count_(0)
    {
        createInlet();
        createInlet();
        createOutlet();
        createOutlet();

        createProperty(new PointerProperty<size_t>("@count", &count_, true));

        setCount(atomlistToValue<float>(args(), 0.f));
    }

    void onBang()
    {
        gen_values_.clear();

        for (size_t i = 0; i < count_; i++)
            bangTo(1);

        listTo(0, gen_values_);
    }

    void onFloat(float v)
    {
        if (!setCount(v))
            return;

        onBang();
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n == 1) {
            setCount(atomlistToValue<float>(l, 0));
            return;
        }

        else if (n == 2) {
            gen_values_.append(l);
            return;
        }
    }

private:
    bool setCount(float v)
    {
        if (!math::is_natural(v)) {
            ERR << "invalid argument! natural number expected: " << v;
            return false;
        }

        size_t c = static_cast<size_t>(v);
        if (c > MAX_COUNT) {
            ERR << "count should be less then " << MAX_COUNT << ". Setting to " << MAX_COUNT;
        }

        count_ = std::min(c, MAX_COUNT);
        return true;
    }
};

extern "C" void setup_list0x2egen()
{
    ObjectFactory<ListGenerate> obj("list.gen");
}
