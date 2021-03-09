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
#ifndef FAUST_LANG_STUB_TILDE_H
#define FAUST_LANG_STUB_TILDE_H

#include "ceammc_sound_external.h"

using namespace ceammc;

class LangFaustTilde : public SoundExternal {

public:
    LangFaustTilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out);
};

class LangFaustUITilde : public LangFaustTilde {
public:
    LangFaustUITilde(const PdArgs& args);
};

void setup_lang_faust_tilde();
void setup_lang_faust_ui_tilde();

#endif // FAUST_LANG_STUB_TILDE_H
