/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "fx_tremolo.h"
#include "args/argcheck2.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

constexpr const char* PROP_FREQ = "@freq";

class FxTremolo : public faust_fx_tremolo_tilde {
    UIProperty* freq_;

public:
    FxTremolo(const PdArgs& args)
        : faust_fx_tremolo_tilde(args)
        , freq_(findUIProperty(PROP_FREQ))
    {
        bindPositionalArgsToProps({ gensym(PROP_FREQ), gensym("@depth") });

        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        static const args::ArgChecker chk("FREQ:f");
        if (!chk.check(lv, this))
            return chk.usage(this);

        if (freq_)
            freq_->setValue(lv.asT<t_float>(), true);
    }
};

void setup_fx_tremolo_tilde()
{
    FaustFactory<FxTremolo> obj("fx.tremolo~");

    obj.setDescription("tremolo effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "tremolo" });
    obj.setXletsInfo({ "signal: input", "float: freq" }, { "signal: fx output" });
}
