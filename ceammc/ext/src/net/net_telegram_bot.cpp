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
#include "fmt/core.h"
#ifndef WITH_MQTT
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetTelegramBot, 1, 1, "compiled without telegram support");
OBJECT_STUB_SETUP(NetTelegramBot, net_telegram_bot, "net.telegram.bot");
#else
#include "net_telegram_bot.h"

#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

NetTelegramBot::NetTelegramBot(const PdArgs& args)
    : BaseTelegramBot(args)
{
    createOutlet();
    createOutlet();
}

void NetTelegramBot::m_connect(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("TOKEN:s");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_telegram_bot_init params { lv.symbolAt(0, &s_)->s_name };

    cli_.reset(new TelegramBot(
        params,
        ceammc_telegram_bot_new,
        ceammc_telegram_bot_free,
        ceammc_telegram_bot_process,
        ceammc_telegram_bot_result_cb {
            this,
            [](void* user, std::uint64_t id, const char* name) {
                auto this_ = static_cast<NetTelegramBot*>(user);
                if (this_) {
                    AtomArray<2> data { gensym(name), id };
                    this_->anyTo(1, gensym("/connected"), data.view());
                }
            },
            [](void* user, std::int64_t chat_id, std::int32_t msg_id, const char* msg) {
                auto this_ = static_cast<NetTelegramBot*>(user);
                if (this_)
                    this_->processText(msg, msg_id, chat_id);
            },
            [](void* user, std::int64_t chat_id, double latitude, double longitude) {
                auto this_ = static_cast<NetTelegramBot*>(user);
                if (this_)
                    this_->processLocation(chat_id, latitude, longitude);
            } },
        ceammc_callback_notify { reinterpret_cast<size_t>(this), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } })
        //
    );

    cli_->setErrorCallback([this](const char* msg) { OBJ_ERR << msg; });
    cli_->setDebugCallback([this](const char* msg) { OBJ_DBG << msg; });
}

void NetTelegramBot::m_send_text(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAT_ID:s MSG:a+");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    auto schat_id = lv.symbolAt(0, &s_)->s_name;
    if (schat_id[0] != '#') {
        METHOD_ERR(s) << fmt::format("chat_id expected to start with #, got: '{}'", schat_id);
        return;
    }
    auto chat_id = std::stoull(schat_id + 1, nullptr, 16);

    if (cli_)
        ceammc_telegram_bot_send_message(cli_->handle(), chat_id, -1, to_string(lv.subView(1)).c_str());
}

void NetTelegramBot::m_whoami(t_symbol* s, const AtomListView& lv)
{
    if (cli_)
        ceammc_telegram_bot_whoami(cli_->handle());
}

bool NetTelegramBot::notify(int code)
{
    if (cli_)
        cli_->processResults();

    return true;
}

void NetTelegramBot::processText(const char* msg, int32_t msg_id, int64_t chat_id)
{
    auto sym_chat_id = gensym(fmt::format("#{:X}", chat_id).c_str());
    if (msg && msg[0] == '/') {
        AtomArray<2> data { sym_chat_id, msg_id };
        anyTo(1, gensym(msg), data.view());
    } else {
        AtomList res { sym_chat_id };
        res.append(AtomList::parseString(msg));
        anyTo(0, gensym("text"), res);
    }
}

void NetTelegramBot::processLocation(int64_t chat_id, double latitude, double longitude)
{
    auto sym_chat_id = gensym(fmt::format("#{:X}", chat_id).c_str());
    AtomArray<3> data { sym_chat_id, latitude, longitude };
    anyTo(0, gensym("location"), data.view());
}

void setup_net_telegram_bot()
{
    ObjectFactory<NetTelegramBot> obj("net.telegram.bot");
    obj.addMethod("connect", &NetTelegramBot::m_connect);
    obj.addMethod("send_text", &NetTelegramBot::m_send_text);
    obj.addMethod("whoami", &NetTelegramBot::m_whoami);
}

#endif
