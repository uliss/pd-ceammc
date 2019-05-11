/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_thisprocess.h"
#include "ceammc_factory.h"
#include "hoa_process_instance.h"

HoaThisProcess::HoaThisProcess(const PdArgs& args)
    : BaseObject(args)
    , process_(nullptr)
{
    createOutlet();
    createOutlet();
    createOutlet();
    createOutlet();
    createOutlet();
}

void HoaThisProcess::onBang()
{
    if (!process_) {
        OBJ_ERR << "not in process instance";
        return;
    }

    const AtomList& args = process_->args();

    for (auto& p : args.properties())
        anyTo(3, p);

    listTo(2, args.slice(5));
    listTo(1, { args[0], args[1] });
    listTo(0, { args[2], args[3], args[4] });
}

void HoaThisProcess::parseProperties()
{
    for (auto& p : args().properties())
        props_[p[0].asSymbol()] = p.slice(1);
}

HoaThisProcess* HoaThisProcess::fromObject(void* obj)
{
    if (!isA(obj))
        return nullptr;

    return reinterpret_cast<ObjectFactory<HoaThisProcess>::ObjectProxy*>(obj)->impl;
}

bool HoaThisProcess::isA(void* obj)
{
    return obj != nullptr
        && ((t_object*)obj)->te_g.g_pd == ObjectFactory<HoaThisProcess>::classPointer();
}

void setup_spat_hoa_thisprocess()
{
    ObjectFactory<HoaThisProcess> obj("hoa.thisprocess~");
}
