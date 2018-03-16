#include "metro_seq.h"
#include "ceammc_factory.h"

static Atom toDigit(const Atom& l)
{
    return Atom(l.asFloat() == 0 ? 0.f : 1.f);
}

MetroSeq::MetroSeq(const PdArgs& a)
    : BaseObject(a)
    , clock_(this, &MetroSeq::clockTick)
    , pattern_(positionalArguments().slice(1).map(toDigit))
    , interval_(0)
    , current_(0)
{
    createOutlet();

    interval_ = new FloatProperty("@interval", positionalFloatArgument(0));
    createProperty(interval_);

    createCbProperty("@current", &MetroSeq::p_current, &MetroSeq::p_set_current);
    createCbProperty("@pattern", &MetroSeq::p_pattern, &MetroSeq::p_set_pattern);
}

void MetroSeq::onFloat(t_float f)
{
    if (f > 0) {
        if (interval_->value() < 1) {
            OBJ_ERR << "invalid metro interval: " << interval_->value();
            return;
        }

        clockTick();
    } else {
        clock_.unset();
    }
}

void MetroSeq::clockTick()
{
    if (current_ >= pattern_.size()) {
        OBJ_ERR << "invalid current index: " << current_;
        return;
    }

    if (pattern_[current_].asFloat() > 0)
        bangTo(0);

    clock_.delay(interval_->value());

    current_ = (current_ + 1) % pattern_.size();
}

AtomList MetroSeq::p_current() const
{
    return Atom(current_);
}

void MetroSeq::p_set_current(const AtomList& l)
{
    size_t n = l.asSizeT();
    if (n >= pattern_.size()) {
        OBJ_ERR << "invalid current value: " << n;
        return;
    }

    current_ = n;
}

AtomList MetroSeq::p_pattern() const
{
    return pattern_;
}

void MetroSeq::p_set_pattern(const AtomList& l)
{
    if (l.empty()) {
        OBJ_ERR << "empty pattern";
        return;
    }

    pattern_ = l.map(toDigit);
}

void setup_metro_seq()
{
    ObjectFactory<MetroSeq> obj("metro.seq");
}
