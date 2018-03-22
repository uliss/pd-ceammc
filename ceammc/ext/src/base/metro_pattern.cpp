#include "metro_pattern.h"
#include "ceammc_factory.h"

static bool validTime(const Atom& a)
{
    return a.asFloat() > 0;
}

MetroPattern::MetroPattern(const PdArgs& args)
    : BaseObject(args)
    , clock_(this, &MetroPattern::tick)
    , pattern_(positionalArguments().filtered(validTime))
    , current_(0)
{
    createOutlet();

    current_ = new SizeTProperty("@current", 0);
    createProperty(current_);

    createCbProperty("@pattern", &MetroPattern::p_pattern, &MetroPattern::p_set_pattern);
}

void MetroPattern::onFloat(t_float on)
{
    if (on > 0)
        tick();
    else
        clock_.unset();
}

AtomList MetroPattern::p_pattern() const
{
    return pattern_;
}

void MetroPattern::p_set_pattern(const AtomList& l)
{
    if (l.empty() || (!l.allOf(validTime))) {
        OBJ_ERR << "invalid pattern: " << l;
        return;
    }

    pattern_ = l;

    if (current_->value() >= l.size())
        current_->setValue(0);
}

void MetroPattern::tick()
{
    if (next())
        bangTo(0);
}

t_float MetroPattern::currentDelay() const
{
    const size_t current = current_->value();

    if (pattern_.empty()) {
        OBJ_ERR << "empty metro pattern";
        return 0;
    }

    Atom delay;

    if (current >= pattern_.size()) {
        OBJ_ERR << "invalid current index: " << current << " for pattern: " << pattern_;
        delay = pattern_.at(0);
    } else
        delay = pattern_.at(current);

    t_float res = delay.asFloat();
    return res < 1 ? 0 : res;
}

bool MetroPattern::next()
{
    if (pattern_.empty()) {
        OBJ_ERR << "empty pattern";
        return false;
    }

    current_->setValue((current_->value() + 1) % pattern_.size());

    t_float tm = currentDelay();

    if (tm > 0) {
        clock_.delay(tm);
        return true;
    } else
        return false;
}

void setup_metro_pattern()
{
    ObjectFactory<MetroPattern> obj("metro.pattern");
}
