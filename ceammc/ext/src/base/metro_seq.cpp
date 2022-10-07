#include "metro_seq.h"
#include "ceammc_factory.h"
#include "ceammc_property_callback.h"

static Atom toDigit(const Atom& l)
{
    return Atom(l.asFloat() == 0 ? 0.f : 1.f);
}

MetroSeq::MetroSeq(const PdArgs& a)
    : BaseObject(a)
    , clock_(this, &MetroSeq::clockTick)
    , interval_(0)
    , current_(0)
{
    createOutlet();

    interval_ = new FloatProperty("@interval", 0);
    interval_->setArgIndex(0);
    interval_->setUnitsMs();
    addProperty(interval_);

    createCbIntProperty(
        "@current",
        [this]() -> int { return current_; },
        [this](int v) -> bool {
            if (v >= pattern_.size())
                return false;
            else {
                current_ = v;
                return true;
            }
        })
        ->setIntCheck(PropValueConstraints::GREATER_EQUAL, 0);

    createCbListProperty(
        "@pattern",
        [this]() -> AtomList { return pattern_; },
        [this](const AtomListView& lv) {
            if (lv.empty())
                return false;
            else {
                pattern_.clear();
                lv.map(toDigit, pattern_);
                return true;
            }
        })
        ->setArgIndex(1);
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

void setup_metro_seq()
{
    ObjectFactory<MetroSeq> obj("metro.seq");

    obj.setDescription("metro sequencer");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "metro", "sequencer" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 5);
}
