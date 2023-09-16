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
#include "flow_sync_pack.h"
#include "ceammc_factory.h"

FlowSyncPack::FlowSyncPack(const PdArgs& args)
    : FlowPack(args)
{
}

void FlowSyncPack::output(size_t inlet_idx)
{
    listTo(0, msg_->value());
}

void setup_flow_sync_pack()
{
    ObjectFactory<FlowSyncPack> obj("flow.sync_pack");
    obj.addAlias("flow.pack'");

    obj.setDescription("flow pack with all hot inlets");
    obj.setCategory("flow");
    obj.setKeywords({"flow", "pack", "hot"});
}
