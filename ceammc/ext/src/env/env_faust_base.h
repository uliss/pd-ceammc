/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef ENV_FAUST_BASE_H
#define ENV_FAUST_BASE_H

#include "ceammc_clock.h"
#include "ceammc_faust.h"
#include "ceammc_sound_external.h"

using namespace ceammc;
using namespace ceammc::faust;

static t_symbol* SYM_PROP_ATTACK = gensym("@attack");
static t_symbol* SYM_PROP_DECAY = gensym("@decay");
static t_symbol* SYM_PROP_SUSTAIN = gensym("@sustain");
static t_symbol* SYM_PROP_RELEASE = gensym("@release");
static t_symbol* SYM_PROP_GATE = gensym("@gate");

template <class T>
class EnvFaustBase : public T {
    ClockMemberFunction<EnvFaustBase<T>> auto_release_;
    ClockMemberFunction<EnvFaustBase<T>> done_;

protected:
    UIProperty* prop_attack_;
    UIProperty* prop_release_;
    UIProperty* prop_gate_;

public:
    EnvFaustBase(const PdArgs& args)
        : T(args)
        , auto_release_(this, &EnvFaustBase<T>::release)
        , done_(this, &EnvFaustBase<T>::done)
        , prop_attack_((UIProperty*)this->property(SYM_PROP_ATTACK))
        , prop_release_((UIProperty*)this->property(SYM_PROP_RELEASE))
        , prop_gate_((UIProperty*)this->property(SYM_PROP_GATE))
    {
        this->createCbProperty("@length", &EnvFaustBase<T>::propLength);
        this->createOutlet();
    }

    virtual t_float length() const
    {
        return prop_attack_->value() + prop_release_->value();
    }

    void onBang() override
    {
        done_.unset();
        prop_gate_->setValue(1);
        auto_release_.delay(10);
        done_.delay(length());
    }

    void m_reset(t_symbol*, const AtomList&)
    {
        this->dsp_->instanceClear();
        auto_release_.unset();
    }

    void m_click(t_symbol*, const AtomList& l)
    {
        onBang();
    }

    AtomList propLength() const
    {
        return Atom(length());
    }

    void gate(bool on)
    {
        prop_gate_->setValue(on ? 1 : 0);
    }

private:
    void release()
    {
        prop_gate_->setValue(0);
    }

    void done()
    {
        this->bangTo(1);
    }
};

#endif // ENV_FAUST_BASE_H
