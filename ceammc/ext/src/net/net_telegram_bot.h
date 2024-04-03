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
#ifndef NET_TELEGRAM_BOT_H
#define NET_TELEGRAM_BOT_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "net_rust_struct.hpp"
using namespace ceammc;

using BaseTelegramBot = DispatchedObject<BaseObject>;
using TelegramBot = net::NetService<ceammc_telegram_bot_client, ceammc_telegram_bot_init, ceammc_telegram_bot_result_cb>;

class NetTelegramBot : public BaseTelegramBot {
    std::unique_ptr<TelegramBot> cli_;

public:
    NetTelegramBot(const PdArgs& args);

    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_send_text(t_symbol* s, const AtomListView& lv);
    void m_whoami(t_symbol* s, const AtomListView& lv);
    //    void m_subscribe(t_symbol* s, const AtomListView& lv);
    //    void m_unsubscribe(t_symbol* s, const AtomListView& lv);
    //    void m_publish(t_symbol* s, const AtomListView& lv);

    bool notify(int code) final;

private:
    void processText(const char* msg, int32_t msg_id, int64_t chat_id);
    void processLocation(int64_t chat_id, double latitude, double longitude);
};

void setup_net_telegram_bot();

#endif // NET_TELEGRAM_BOT_H
