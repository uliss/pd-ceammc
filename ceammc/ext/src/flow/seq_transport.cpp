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
#include "seq_transport.h"
#include "ceammc_factory.h"
#include "ceammc_transport.h"
#include "transport_iface.h"

/*
 * 2*|3/8| 32*|4/4 b=32| |4/4 t=80| |5/4 t<90| 16*|3/4| |2/8 t>120@1|
 * 2*|3/8| 32*|4/4 c=1|  |4/4 p=80| |5/4 p@1| 16*|3/4| |2/8 p->!90@1|
 * |2/4 pan=30.. tempo=120| |3/4 pan0=120|
 */

SeqTransport::SeqTransport(const PdArgs& args)
    : BaseObject(args)
    , name_(nullptr)
    , clock_([this]() { tick(); })
    , transport_(nullptr)
    , beats_in_bar_(nullptr)
{
    name_ = new SymbolProperty("@name", gensym("default"), PropValueAccess::INITONLY);
    name_->setArgIndex(0);
    name_->setSuccessFn([this](Property*) { transport_ = &TransportStorage::instance().getOrCreate(name_->value()); });
    addProperty(name_);

    beats_in_bar_ = new IntProperty("@bar", 0);
    beats_in_bar_->checkNonNegative();
    addProperty(beats_in_bar_);
}

void SeqTransport::onFloat(t_float f)
{
    if (!transport_) {
        LIB_ERR << "logic error, NULL transport: " << name_->value();
        return;
    }

    if (f == 0) {
        transport_->stop();
        clock_.unset();
    } else {
        transport_->start();
        clock_.delay(500);
    }
}

void SeqTransport::tick()
{
    if (!transport_) {
        LIB_ERR << "logic error, NULL transport: " << name_->value();
        return;
    }

    transport_->nextBeat(beats_in_bar_->value());
}

void setup_seq_transport()
{
    ObjectFactory<SeqTransport> obj("seq.transport");
}
