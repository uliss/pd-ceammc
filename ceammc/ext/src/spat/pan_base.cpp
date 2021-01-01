#include "pan_base.h"
#include <cassert>

PanBase::PanBase(const PdArgs& args)
    : SoundExternal(args)
    , smooth_(0)
    , pos_(0)
{
    smooth_ = new IntProperty("@smooth", 20);
    smooth_->setInitOnly();
    smooth_->checkClosedRange(0, 100);
    smooth_->setSuccessFn(
        [this](Property*) { smooth_pos_.setSmoothTime(smooth_->value(), samplerate(), 8); });
    addProperty(smooth_);

    pos_ = new FloatProperty("@pos", 0);
    pos_->checkClosedRange(-1, 1);
    pos_->setArgIndex(0);
    addProperty(pos_);

    createInlet();
    createSignalOutlet();
    createSignalOutlet();
}

void PanBase::onInlet(size_t n, const AtomListView& l)
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
