#ifndef NET_HTTP_SEND_H
#define NET_HTTP_SEND_H

#include "ceammc_pollthread_spsc.h"
using namespace ceammc;

struct HttpRequest {
    std::string host;
    std::string user;
    std::string pass;
    std::string path;
    int port;
    int timeout;

    HttpRequest() = default;
    HttpRequest(const HttpRequest&) = default;
    HttpRequest(HttpRequest&&) = default;
    HttpRequest& operator=(HttpRequest&&) = default;
};

struct HttpResult {
    std::string content;
    int code;
};

using NetHttpSendBase = FixedSPSCObject<HttpRequest, HttpResult>;

class NetHttpSend : public NetHttpSendBase {
    SymbolProperty* host_;
    IntProperty* port_;
    SymbolProperty* user_;
    AtomProperty* pass_;
    ThreadPdLogger logger_;
    IntProperty* timeout_;

public:
    NetHttpSend(const PdArgs& args);
    void m_get(t_symbol* s, const AtomListView& lv);

    Either<HttpResult> processRequest(const HttpRequest& req) final;
    void processResult(const HttpResult& res) final;

public:
    static std::string makePath(t_symbol* path, const AtomListView& lv);
};

void setup_net_http_send();

#endif // NET_HTTP_SEND_H
