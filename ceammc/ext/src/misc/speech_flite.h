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
#ifndef SPEECH_FLITE_H
#define SPEECH_FLITE_H

#include "datatype_string.h"
#include "ceammc_array.h"
#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class FliteThread;

class SpeechFlite : public BaseObject {
    Array array_;
    t_symbol* name_;
    SymbolProperty* voice_name_;
    FloatPropertyClosedRange* speed_;
    FloatProperty* pitch_;
    FliteThread* render_;
    ClockMemberFunction<SpeechFlite> clock_;

public:
    SpeechFlite(const PdArgs& args);
    ~SpeechFlite();

    void onFloat(t_float v);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& lst);
    void onDataT(const DataTPtr<DataTypeString>& str);

private:
    bool synth(const char* str);
    void clockTick();
};

void setup_misc_speech_flite();

#endif // SPEECH_FLITE_H
