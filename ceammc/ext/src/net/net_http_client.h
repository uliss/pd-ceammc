#ifndef NET_HTTP_CLIENT_H
#define NET_HTTP_CLIENT_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "net_rust_struct.hpp"
using namespace ceammc;

using NetHttpClientBase = DispatchedObject<BaseObject>;

class NetHttpClient : public NetHttpClientBase {
    net::NetService<ceammc_http_client, ceammc_http_client_result> srv_;

public:
    NetHttpClient(const PdArgs& args);

    void m_get(t_symbol* s, const AtomListView& lv);
    void m_post(t_symbol* s, const AtomListView& lv);
    void m_select(t_symbol* s, const AtomListView& lv);

    bool notify(int code) final;
};

void setup_net_http_client();

#endif // NET_HTTP_CLIENT_H
