#include "tl_bang.h"
#include "ceammc_factory.h"
#include "ceammc_timeline.h"

using namespace ceammc::tl;

TlBang::TlBang(const PdArgs& args)
    : TlBaseObject(args)
    , delay_(nullptr)
    , clock_(this, &TlBang::tick)
{
    delay_ = new FloatProperty("@delay", 0);
    delay_->checkNonNegative();
    delay_->setArgIndex(0);
    delay_->setUnitsMs();
    addProperty(delay_);
}

void TlBang::tick()
{
    bangTo(0);
}

void TlBang::onCueEnter()
{
    clock_.delay(delay_->value());
}

void TlBang::onCueLeave()
{
    clock_.unset();
}

void setup_tl_bang()
{
    ObjectFactory<TlBang> obj("tl.bang");
    obj.addAlias("tl.b");
    TlBang::setDisplaceFn(obj.classPointer());
}
