/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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

#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class SystemScreenSize : public BaseObject {
public:
    SystemScreenSize(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();

        char buf[MAXPDSTRING];
        sprintf(buf, "#%p", static_cast<void*>(owner()));
        bindReceive(gensym(buf));
    }

    void onBang()
    {
        if (receive()) {
            sys_vgui("pdsend \"%s .screensize [winfo screenwidth .] [winfo screenheight .]\"\n",
                receive()->s_name);
        }
    }

    void screenSize(t_symbol*, const AtomListView& data)
    {
        listTo(0, data);
    }
};

void setup_system_screen_size()
{
    ObjectFactory<SystemScreenSize> obj("system.screen_size");
    obj.addMethod(".screensize", &SystemScreenSize::screenSize);
}
