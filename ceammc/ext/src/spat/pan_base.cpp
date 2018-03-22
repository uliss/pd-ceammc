#include "pan_base.h"
#include <cassert>

PanBase::PanBase(const PdArgs& args)
    : SoundExternal(args)
    , smooth_(0)
    , pos_(0)
{
    smooth_ = new InitIntPropertyClosedRange(new IntPropertyClosedRange("@smooth", 20, 0, 100));
    createProperty(smooth_);

    pos_ = new FloatPropertyClosedRange("@pos", 0, -1, 1);
    setPropertyFromPositionalArg(pos_, 0);
    createProperty(pos_);

    createInlet();
    createSignalOutlet();
    createSignalOutlet();

    parseProperties();
    // after parse
    smooth_pos_.setSmoothTime(smooth_->value(), samplerate(), 8);
}

void PanBase::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    pos_->set(l);
}

void PanBase::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    smooth_pos_.setSmoothTime(smooth_->value(), samplerate(), 8);
}
