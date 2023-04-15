/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "fx_overdrive.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxOverdrive : public faust_fx_overdrive_tilde {
    UIProperty* gain_;

public:
    FxOverdrive(const PdArgs& args)
        : faust_fx_overdrive_tilde(args)
        , gain_(static_cast<UIProperty*>(property("@gain")))
    {
        createInlet();

        if (gain_)
            gain_->setArgIndex(0);
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        if (gain_)
            gain_->set(lv);
    }
};

void setup_fx_overdrive_tilde()
{
    FaustFactory<FxOverdrive> obj("fx.overdrive~");
    obj.setXletsInfo({ "signal: input", "float(1-5): gain" }, { "signal: output" });

    obj.setDescription("overdrive effect");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "overdrive"});
}
