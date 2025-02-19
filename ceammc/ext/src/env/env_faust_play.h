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
#ifndef ENV_FAUST_PLAY_H
#define ENV_FAUST_PLAY_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_pd.h"

namespace ceammc {

template <class T>
class EnvAutoplay : public T {
    ClockLambdaFunction auto_play_clock_;

public:
    EnvAutoplay(const PdArgs& args)
        : T(args)
        , auto_play_clock_([this]() { sendGate(0); })
    {
    }

    void m_play(t_symbol* sel, const AtomListView& dur)
    {
        bool ok = !dur.empty() && dur[0].isFloat() && dur[0].asFloat() > 0;
        if (!ok) {
            METHOD_ERR(sel) << "DURATION(ms) expected: " << dur;
            return;
        }

        // reset envelope
        sendReset();
        // activate timer
        auto_play_clock_.delay(dur[0].asFloat());
        // gate on
        sendGate(1);
    }

protected:
    void sendReset()
    {
        pd::message_to(this->asPd(), gensym("reset"), {});
    }

    void sendGate(t_float val)
    {
        Atom gate(val);
        pd::message_to(this->asPd(), gensym("@gate"), gate);
    }
};
}

#endif // ENV_FAUST_PLAY_H
