#include "list_gen.h"
#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_log.h"

static const int MIN_COUNT = 1;
static const int MAX_COUNT = 1024;

ListGenerate::ListGenerate(const PdArgs& a)
    : BaseObject(a)
    , count_(nullptr)
    , in_process_(false)
{
    createInlet();
    createOutlet();
    createOutlet();

    count_ = new IntPropertyClosedRange("@count", positionalFloatArgument(0, 1), MIN_COUNT, MAX_COUNT);
    createProperty(count_);
}

bool ListGenerate::loopbackDetected()
{
    if (in_process_)
        OBJ_ERR << "loopback detected... rejected";

    return in_process_;
}

void ListGenerate::onBang()
{
    if (loopbackDetected())
        return;

    in_process_ = true;
    gen_values_.clear();

    for (int i = 0; i < count_->value(); i++)
        bangTo(1);

    listTo(0, gen_values_.toList());
    in_process_ = false;
}

void ListGenerate::onFloat(float v)
{
    if (loopbackDetected())
        return;

    if (!setCount(v))
        return;

    onBang();
}

void ListGenerate::onInlet(size_t n, const AtomList& l)
{
    if (n == 1) {
        gen_values_.append(l);
        return;
    }
}

bool ListGenerate::setCount(float v)
{
    if (!math::is_natural(v)) {
        OBJ_ERR << "invalid argument! natural number expected: " << v;
        return false;
    }

    int c = static_cast<int>(v);
    if (c > MAX_COUNT) {
        OBJ_ERR << "count should be less then " << MAX_COUNT << ". Setting to " << MAX_COUNT;
    }

    count_->setValue(std::min(c, MAX_COUNT));
    return true;
}

void setup_list_gen()
{
    ObjectFactory<ListGenerate> obj("list.gen");
}
