#ifndef NET_HTTP_CLIENT_H
#define NET_HTTP_CLIENT_H

#include "ceammc_pollthread_spsc.h"
#include "net_rust.hpp"
using namespace ceammc;

struct HttpCliRequest {};

struct HttpCliResult {
    std::string content;
    std::uint16_t code;
};

using NetHttpClientBase = FixedSPSCObject<HttpCliRequest, HttpCliResult>;

class NetHttpClient : public NetHttpClientBase {
    ceammc_http_client* cli_ { nullptr };

public:
    NetHttpClient(const PdArgs& args);
    ~NetHttpClient();

    void m_get(t_symbol* s, const AtomListView& lv);
    void m_select(t_symbol* s, const AtomListView& lv);

    void processRequest(const HttpCliRequest& req, ResultCallback cb) final;
    void processResult(const HttpCliResult& res) final;

    Future createTask() override;

public:
};

void setup_net_http_client();

#endif // NET_HTTP_CLIENT_H
