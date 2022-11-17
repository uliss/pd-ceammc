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
#include "hw_apple_smc.h"
#include "ceammc_factory.h"
#include "ceammc_stub.h"

HwAppleSMC::HwAppleSMC(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    createCbListProperty("@keys", [this]() -> AtomList { return propKeys(); });

#ifndef WITH_SMC
    OBJ_ERR << "no system management control support for this device";
#endif
}

AtomList HwAppleSMC::propKeys() const
{
#ifdef WITH_SMC
    return smc_.keys();
#else
    return AtomList();
#endif
}

void HwAppleSMC::m_read(t_symbol* s, const AtomListView& l)
{
#ifdef WITH_SMC
    if (!checkArgs(l, ARG_SYMBOL, s))
        return;

    AtomList res(smc_.readKey(l.symbolAt(0, &s_)));
    if (res.empty()) {
        OBJ_ERR << "key is not found: " << l;
        return;
    }

    listTo(0, res);
#else
    OBJ_ERR << "no SMS support on this machine";
#endif
}

#if !defined(__arm64__) && defined(WITH_SMC)
extern "C" void setup_hw_apple_smc()
{
    ObjectFactory<HwAppleSMC> obj("hw.apple_smc");
}
#else
CONTROL_OBJECT_STUB(1, 1, "not supported on this platform");
OBJECT_STUB_SETUP("hw.apple_smc", hw_apple_smc);
#endif
