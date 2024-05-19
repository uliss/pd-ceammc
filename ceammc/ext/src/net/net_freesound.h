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

#include "ceammc_editor_object.h"
#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "net_rust_struct.hpp"
using namespace ceammc;

using NetFreesoundBase = EditorObject<DispatchedObject<BaseObject>, EditorSyntax::NONE>;
using NetFreesoundImpl = net::NetService<ceammc_freesound_client, ceammc_freesound_init, ceammc_freesound_result_cb>;

struct OAuthAccess {
    std::string token;
    std::uint64_t expires;
};

class NetFreesound : public NetFreesoundBase {
    std::unique_ptr<NetFreesoundImpl> cli_;
    SymbolProperty* oauth_file_;
    SymbolProperty* oauth_id_;
    SymbolProperty* oauth_secret_;

public:
    NetFreesound(const PdArgs& args);

    void initDone() final;
    bool notify(int code) final;

    void editorClear() final;
    void editorAddLine(t_symbol* sel, const AtomListView& lv) final;
    EditorLineList getContentForEditor() const final;
    EditorTitleString editorTitle() const final { return "Freesound ID/SECRET"; }
    void editorSync() final;

    void m_access(t_symbol* s, const AtomListView& lv);
    void m_download(t_symbol* s, const AtomListView& lv);
    void m_load(t_symbol* s, const AtomListView& lv);
    void m_me(t_symbol* s, const AtomListView& lv);
    void m_search(t_symbol* s, const AtomListView& lv);

private:
    void processReplyAccess(const char* access_token, std::uint64_t expires);
    void processReplyDownload(const char* filename);
    void processReplyInfoMe(const ceammc_freesound_info_me& info);
    void processReplyLoad(ceammc_freesound_array_data* data, size_t len);
    void processReplyOAuth(const char* url);
    void processReplyOAuthFile(const char* id, const char* secret);

    bool checkToken(t_symbol* s) const;
    bool checkIdAndSecret() const;
};

void setup_net_freesound();

#endif // NET_FREESOUND_H
