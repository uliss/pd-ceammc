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
    void m_get_file(t_symbol* s, const AtomListView& lv);
    void m_send_audio(t_symbol* s, const AtomListView& lv);
    void m_send_text(t_symbol* s, const AtomListView& lv);
    void m_send_voice(t_symbol* s, const AtomListView& lv);
    void m_whoami(t_symbol* s, const AtomListView& lv);

    bool notify(int code) final;

private:
    void processText(const char* msg, int32_t msg_id, int64_t chat_id);
    void processLocation(int64_t chat_id, double latitude, double longitude);
    void processSticker(int64_t chat_id, const char* file_id, const char* emoji);
    void processVoice(std::int64_t chat_id,
        const char* file_id,
        const char* file_unique_id,
        const char* mime,
        std::uint32_t duration,
        std::uint64_t file_size);
    void processAudio(std::int64_t chat_id,
        const char* file_id,
        const char* file_unique_id,
        const char* mime,
        const char* filename,
        std::uint32_t duration,
        std::uint64_t file_size,
        const char* title);
};

void setup_net_telegram_bot();

#endif // NET_TELEGRAM_BOT_H
