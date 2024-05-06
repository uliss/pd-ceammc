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
#ifndef WITH_SYSTEM_CMD
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(SystemCommand, 1, 2, "compiled without system cmd support");
OBJECT_STUB_SETUP(SystemCommand, system_command, "system.command", "system.cmd");
#else
#include "system_command.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"

CEAMMC_DEFINE_SYM_HASH(str)
CEAMMC_DEFINE_SYM_HASH(bytes)
CEAMMC_DEFINE_SYM_HASH(lines)
CEAMMC_DEFINE_SYM_HASH(pd)

constexpr size_t OUT_STDOUT = 1;
constexpr size_t OUT_EXITCODE = 0;

static ceammc_system_process_mode prop2mode(const char* mode)
{
    switch (crc32_hash(mode)) {
    case hash_str:
        return ceammc_system_process_mode::String;
    case hash_bytes:
        return ceammc_system_process_mode::Bytes;
    case hash_lines:
    case hash_pd:
    default:
        return ceammc_system_process_mode::Lines;
    }
}

SystemCommand::SystemCommand(const PdArgs& args)
    : BaseObject(args)
    , proc_check_([this]() {
        int32_t rc = 0;
        switch (ceammc_system_process_results(proc_, &rc)) {
        case ceammc_system_process_state::Running:
            proc_check_.delay(timeout_->value());
            break;
        case ceammc_system_process_state::Ready:
        case ceammc_system_process_state::Error:
        default:
            floatTo(OUT_EXITCODE, rc);
            return;
        }
    })
{
    createOutlet();
    createOutlet();

    pargs_ = new ListProperty("@args");
    pargs_->setArgIndex(0);
    pargs_->setListCheckFn([this](const AtomListView& lv) -> bool {
        return lv.empty() || parseCommand(lv);
    });
    addProperty(pargs_);

    stdout_ = new BoolProperty("@stdout", true);
    addProperty(stdout_);

    stderr_ = new BoolProperty("@stderr", true);
    addProperty(stderr_);

    pwd_ = new SymbolProperty("@pwd", &s_);
    addProperty(pwd_);

    mode_ = new SymbolEnumProperty("@mode", { sym_pd(), sym_lines(), sym_bytes(), sym_str() });
    addProperty(mode_);

    timeout_ = new IntProperty("@t", 100);
    timeout_->setUnitsMs();
    timeout_->checkClosedRange(5, 500);
    addProperty(timeout_);
}

SystemCommand::~SystemCommand()
{
    ceammc_system_process_free(proc_);
}

void SystemCommand::onBang()
{
    if (args_.empty()) {
        OBJ_ERR << "empty command list";
        return;
    }

    exec();
}

void SystemCommand::onAny(t_symbol* s, const AtomListView& lv)
{
    AtomList32 lst;
    lst.push_back(s);
    lst.insert_back(lv.begin(), lv.end());

    if (pargs_->set(lst.view()))
        exec();
}

void SystemCommand::m_terminate(t_symbol* s, const AtomListView& lv)
{
    ceammc_system_process_terminate(proc_);
}

void SystemCommand::m_stdin(t_symbol* s, const AtomListView& lv)
{
    exec(to_string(lv).c_str());
}

void SystemCommand::exec(const char* input)
{
    if (proc_)
        ceammc_system_process_free(proc_);

    std::vector<ceammc_system_process_cmd> cmd_args;
    cmd_args.reserve(args_.size());

    for (auto& c : args_)
        cmd_args.push_back(ceammc_system_process_cmd { c.name, c.args.data(), c.args.size() });

    proc_ = ceammc_system_process_new(
        cmd_args.data(),
        cmd_args.size(),
        prop2mode(mode_->value()->s_name),
        stdout_->value(),
        stderr_->value(),
        pwd_->value()->s_name,
        input,
        this,
        [](void* user, const char* msg) {
            auto this_ = static_cast<SystemCommand*>(user);
            if (this_)
                Error(this_) << msg;
        },
        [](void* user, const std::uint8_t* data, size_t len) {
            auto this_ = static_cast<SystemCommand*>(user);
            if (this_)
                this_->output(OUT_STDOUT, data, len);
        },
        [](void* user, const std::uint8_t* data, size_t len) {
            auto this_ = static_cast<SystemCommand*>(user);
            if (this_) {
                Error(this_) << std::string((const char*)data, len);
            }
        });

    if (proc_ && ceammc_system_process_exec(proc_))
        proc_check_.delay(20);
}

bool SystemCommand::parseCommand(const AtomListView& lv)
{
    enum State {
        ON_COMMAND,
        ON_ARGS,
        ON_PIPE,
    };

    auto state = ON_COMMAND;

    auto is_pipe = [](t_symbol* sym) { return sym->s_name[0] == '|' && sym->s_name[1] == '\0'; };

    args_.clear();

    for (auto& a : lv) {
        t_symbol* sym = &s_;
        if (a.isSymbol())
            sym = a.asT<t_symbol*>();
        else
            sym = gensym(to_string(a).c_str());

        const auto sym_pipe = is_pipe(sym);

        switch (state) {
        case ON_COMMAND: {
            if (sym_pipe) {
                OBJ_ERR << "unexpected pipe symbol";
                return false;
            } else {
                args_.push_back({ sym->s_name, {} });
                state = ON_ARGS;
            }
        } break;
        case ON_ARGS: {
            if (sym_pipe)
                state = ON_PIPE;
            else
                args_.back().args.push_back(sym->s_name);
        } break;
        case ON_PIPE: {
            if (sym_pipe) {
                OBJ_ERR << "unexpected pipe symbol";
                return false;
            } else {
                args_.push_back({ sym->s_name, {} });
                state = ON_ARGS;
            }
        } break;
        }
    }

    return true;
}

void SystemCommand::output(size_t outlet, const uint8_t* data, size_t len)
{
    switch (crc32_hash(mode_->value())) {
    case hash_str: {
        atomTo(outlet, StringAtom(std::string((const char*)data, len)));
    } break;
    case hash_bytes: {
        for (size_t i = 0; i < len; i++)
            floatTo(outlet, data[i]);
    } break;
    case hash_lines: {
        atomTo(outlet, StringAtom(std::string((const char*)data, len)));
    } break;
    case hash_pd:
    default: {
        t_binbuf* bbuf = binbuf_new();
        binbuf_text(bbuf, (const char*)data, len);
        listTo(outlet, AtomListView(bbuf));
        binbuf_free(bbuf);
    } break;
    }
}

void setup_system_command()
{
    ObjectFactory<SystemCommand> obj("system.command");
    obj.addAlias("system.cmd");
    obj.addMethod("terminate", &SystemCommand::m_terminate);
    obj.addMethod("stdin", &SystemCommand::m_stdin);
}
#endif
