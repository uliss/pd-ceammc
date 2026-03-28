/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#ifndef MISC_WHISPER_TILDE_H
#define MISC_WHISPER_TILDE_H

#include "ceammc_property_enum.h"
#include "ceammc_rs_msg_object.h"
#include "ceammc_sound_external.h"
#include "whisper_rust.hpp"

#include <memory>

namespace ceammc {
class MiscWhisperTilde : public RustMessageObject<SoundExternal> {
    using WhisperPtr = std::unique_ptr<ceammc_misc_whisper, typeof(&ceammc_misc_whisper_free)>;
    WhisperPtr w_;
    bool do_record_ { false };
    SymbolProperty* model_ { nullptr };
    SymbolEnumProperty* lang_ { nullptr };
    SymbolEnumProperty* output_mode_ { nullptr };
    IntProperty* num_threads_ { nullptr };
    FloatProperty* max_length_ { nullptr };
    size_t recorded_samples_ = 0;

public:
    explicit MiscWhisperTilde(const PdArgs& args);
    void initDone() override;
    bool notify(int code) final;
    void processBlock(const t_sample** in, t_sample** out) final;

    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_recognize(t_symbol* s, const AtomListView& lv);
    void m_record(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);
};

void setup_misc_whisper_tilde();
} // namespace ceammc

#endif // MISC_WHISPER_TILDE_H
