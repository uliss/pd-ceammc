/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef SYSTEM_COMMAND_H
#define SYSTEM_COMMAND_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "system_rust.hpp"

using namespace ceammc;
class SystemCommand : public BaseObject {
    struct Command {
        const char* name;
        std::vector<const char*> args;
    };

    std::vector<Command> args_;
    ceammc_system_process* proc_ { nullptr };
    ClockLambdaFunction proc_check_;

    ListProperty* pargs_ { nullptr };
    IntProperty* timeout_ { nullptr };
    BoolProperty* stdout_ { nullptr };
    BoolProperty* stderr_ { nullptr };
    SymbolProperty* pwd_ { nullptr };

public:
    SystemCommand(const PdArgs& args);
    ~SystemCommand();

    void onBang() final;
    void onAny(t_symbol* s, const AtomListView& lv) final;

    void m_terminate(t_symbol* s, const AtomListView& lv);
    void m_stdin(t_symbol* s, const AtomListView& lv);

private:
    void exec(const char* input = nullptr);
    bool parseCommand(const AtomListView& lv);
};

void setup_system_command();

#endif // SYSTEM_COMMAND_H
