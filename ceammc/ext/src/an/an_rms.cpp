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
#include "an_rms.h"
#include "ceammc_factory.h"

using namespace ceammc;

class AnRms : public faust_an_rms_tilde {
public:
    AnRms(const PdArgs& args)
        : faust_an_rms_tilde(args)
    {
        auto SYM_PROP_PERIOD = gensym("@period");

        bindPositionalArgsToProps({ SYM_PROP_PERIOD });
    }
};

void setup_an_rms_tilde()
{
    SoundExternalFactory<AnRms> obj("an.rms~");
    obj.addMethod("reset", &AnRms::m_reset);

    obj.setDescription("root mean square with moving-average algorithm.");
    obj.setCategory("an");
    obj.setKeywords({"rms", "root", "mean", "square"});
}
