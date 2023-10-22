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
#include "base_z_tilde.h"
#include "ceammc_factory.h"

BaseZTilde::BaseZTilde(const PdArgs& args)
    : SoundExternal(args)
    , delay_(0)
    , z_(nullptr)
{
    z_ = new IntProperty("@z", 1);
    z_->checkClosedRange(0, BASE_Z_MAX_SIZE);
    z_->setSuccessFn([this](Property*) {
        if (!delay_.setDelay(z_->value()))
            OBJ_ERR << "delay error";
    });
    z_->setArgIndex(0);
    z_->setUnits(PropValueUnits::SAMP);
    addProperty(z_);

    createInlet();
    createSignalOutlet();
}

void BaseZTilde::onInlet(size_t n, const AtomListView& lv)
{
    z_->set(lv);
}

void BaseZTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto bs = blockSize();
    for (size_t i = 0; i < bs; i++)
        out[0][i] = delay_.push(in[0][i]);
}

void setup_base_z_tilde()
{
    SoundExternalFactory<BaseZTilde> obj("z~");
    obj.setXletsInfo({ "signal: input", "int: delay in samples" }, { "signal: output" });

    obj.setDescription("sample delay");
    obj.setCategory("base");
    obj.setKeywords({ "delay", "z" });
}
