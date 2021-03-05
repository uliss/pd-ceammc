/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef SPEECH_FLITE_TILDE_H
#define SPEECH_FLITE_TILDE_H

#include "ceammc_clock.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"

#include "readerwriterqueue/readerwriterqueue.h"

#include <atomic>
#include <future>
#include <list>
#include <queue>
#include <string>

using namespace ceammc;

using rw_queue = moodycamel::ReaderWriterQueue<float, 2048>;

class SpeechFilteTilde : public SoundExternal {
    SymbolEnumProperty* voice_name_;
    FloatProperty* speed_;
    FloatProperty* pitch_;
    std::future<int> running_;
    rw_queue queue_;
    std::atomic_bool run_;
    std::queue<std::string> sentences_;
    ClockLambdaFunction clock_;

public:
    SpeechFilteTilde(const PdArgs& args);
    ~SpeechFilteTilde();

    void onList(const AtomList& lst) override;

    void processBlock(const t_sample** in, t_sample** out) override;

    void m_stop(t_symbol* s, const AtomListView& lv);

private:
    void checkSynth();
};

void setup_misc_speech_filte_tilde();

#endif // SPEECH_FLITE_TILDE_H
