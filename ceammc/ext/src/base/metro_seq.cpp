#include "metro_seq.h"
#include "ceammc_factory.h"

static Atom toDigit(const Atom& a)
{
    return Atom(a.asFloat() == 0 ? 0.f : 1.f);
}

MetroSeq::MetroSeq(const PdArgs& a)
    : BaseObject(a)
    , clock_(this, &MetroSeq::clockTick)
    , interval_(0)
    , current_(0)
{
    createInlet(); // period
    createInlet(); // pattern
    createOutlet();

    interval_ = new FloatProperty("@interval", 0);
    interval_->setArgIndex(0);
    interval_->setUnitsMs();
    interval_->checkMinEq(0);
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
        [this](const AtomListView& lv) { return setPattern(lv); })
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

void MetroSeq::onInlet(size_t n, const AtomListView& lv)
{
    switch (n) {
    case 1:
        interval_->set(lv);
        break;
    case 2:
        setPattern(lv);
        break;
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

bool MetroSeq::setPattern(const AtomListView& lv)
{
    if (lv.empty()) {
        return false;
    } else {
        pattern_.clear();
        lv.map(toDigit, pattern_);

        if (current_ >= pattern_.size())
            current_ = 0;

        return true;
    }
}

void setup_metro_seq()
{
    ObjectFactory<MetroSeq> obj("metro.seq");

    obj.setDescription("metro sequencer");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "metro", "sequencer" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 5);
    obj.setXletsInfo({ "float: 1|0 to start or stop metro", "float: set new interval", "list: set new pattern" }, { "bang" });
}
