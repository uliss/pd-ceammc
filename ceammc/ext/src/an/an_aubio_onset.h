/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef AN_AUBIO_ONSET_H
#define AN_AUBIO_ONSET_H

#include "aubio_base.h"
#include "ceammc_array.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class AubioOnset : public BaseObject {
public:
    AubioOnset(const PdArgs& args);

    bool setArray(t_symbol* s);
    bool checkArray();

    void onBang() final;
    void initDone() final;

private:
    AtomList propArray() const;
    void propSetArray(const AtomList& l);
    void resetAubioOnset(uint_t sr);
    void saveSteadyProperties();
    void restoreSteadyProperties();

private:
    t_symbol* array_name_;
    FVecPtr in_, out_;
    OnsetPtr onset_;

    IntProperty* buffer_size_;
    HopSizeProperty* hop_size_;
    OnsetMethodProperty* method_;
    OnsetFloatProperty* threshold_;
    OnsetFloatProperty* silence_threshold_;
    OnsetFloatProperty* speedlim_;

protected:
    Array array_;
};

void setup_an_onset();

#endif // AN_AUBIO_ONSET_H
