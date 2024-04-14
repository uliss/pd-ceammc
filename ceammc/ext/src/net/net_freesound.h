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
#ifndef NET_FREESOUND_H
#define NET_FREESOUND_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "net_rust_struct.hpp"
using namespace ceammc;

using NetFreesoundBase = DispatchedObject<BaseObject>;
using NetFreesoundImpl = net::NetService<ceammc_freesound_client, ceammc_freesound_init, ceammc_freesound_result_cb>;


struct OAuthAccess {
    std::string token;
    std::uint64_t expires;
};

class NetFreesound : public NetFreesoundBase {
    std::unique_ptr<NetFreesoundImpl> cli_;
    SymbolProperty* token_;
    SymbolProperty* oauth_id_;
    SymbolProperty* oauth_secret_;
    OAuthAccess access_;

public:
    NetFreesound(const PdArgs& args);

    void initDone() final;
    bool notify(int code) final;

    void m_me(t_symbol* s, const AtomListView& lv);
    void m_search(t_symbol* s, const AtomListView& lv);
    void m_oauth2(t_symbol* s, const AtomListView& lv);

private:
    //    bool processParams(const AtomListView& lv, std::vector<ceammc_http_client_param>& params) const;
    void processReplyOAuth2(const char* url);
    void processReplyAccess(const char* access_token, std::uint64_t expires);
    void processReplyInfoMe(uint64_t id, const char* username, const char* email, const char* homepage, const char* url, const char* sounds, const char* packs);
};

void setup_net_freesound();

#endif // NET_FREESOUND_H
