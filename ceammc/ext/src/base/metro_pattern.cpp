#include "metro_pattern.h"
#include "ceammc_factory.h"
#include "ceammc_property_callback.h"

static bool validTime(const Atom& a)
{
    return a.asFloat() > 0;
}

MetroPattern::MetroPattern(const PdArgs& args)
    : BaseObject(args)
    , clock_(this, &MetroPattern::tick)
    , current_(nullptr)
    , sync_(nullptr)
    , sync_update_(false)
{
    createOutlet();
    createOutlet();

    current_ = new SizeTProperty("@current", 0);
    addProperty(current_);

    sync_ = new BoolProperty("@sync", false);
    addProperty(sync_);

    createCbListProperty(
        "@pattern", [this]() -> AtomList { return pattern_; },
        [this](const AtomList& lst) -> bool { return p_set_pattern(lst); })
        ->setArgIndex(0);
}

void MetroPattern::onFloat(t_float on)
{
    if (on > 0) {
        clock_.delay(currentDelay());
        output(true);
    } else
        clock_.unset();
}

bool MetroPattern::p_set_pattern(const AtomList& l)
{
    if (l.empty() || (!l.allOf(validTime))) {
        OBJ_ERR << "invalid pattern: " << l;
        return false;
    }

    if (sync_->value()) {
        // defer pattern update
        sync_update_ = true;
        // save to pattern to temp list
        new_pattern_ = l;

        // only set pattern if empty main pattern list
        if (pattern_.empty())
            pattern_ = l;
    } else {
        pattern_ = l;
        // keep in sync - to handle @sync property change while running
        new_pattern_ = l;
    }

    return true;
}

void MetroPattern::tick()
{
    if (pattern_.empty()) {
        OBJ_ERR << "empty pattern";
        return;
    }

    // on last pattern value
    if (current_->value() == pattern_.size() - 1) {
        // update to saved new pattern
        if (sync_->value() && sync_update_) {
            sync_update_ = false;
            pattern_ = new_pattern_;
        }
    }

    next();

    auto tm = currentDelay();
    if (tm > 0)
        clock_.delay(currentDelay());

    output(false);
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

size_t MetroPattern::currentIndex() const
{
    return current_->value();
}

bool MetroPattern::next()
{
    if (pattern_.empty())
        return false;

    current_->setValue((current_->value() + 1) % pattern_.size());
    return true;
}

void MetroPattern::output(bool on_start)
{
    // end of cycle output
    if (!on_start && current_->value() == 0)
        bangTo(1);

    floatTo(1, current_->value());
    bangTo(0);
}

void setup_metro_pattern()
{
    ObjectFactory<MetroPattern> obj("metro.pattern");

    obj.setDescription("metro with rhythmic patterns");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "metro", "pattern", "rhythm" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 5);
}
