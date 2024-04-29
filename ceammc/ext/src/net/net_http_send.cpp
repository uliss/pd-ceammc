#ifndef WITH_HTTP
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetHttpSend, 1, 2, "compiled without http support");
OBJECT_STUB_SETUP(NetHttpSend, net_http_send, "net.http.send", "http.send");
#else

#include "net_http_send.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"

#include "fmt/core.h"
#include "httplib.h"

NetHttpSend::NetHttpSend(const PdArgs& args)
    : NetHttpSendBase(args)
    , host_(nullptr)
    , port_(nullptr)
    , user_(nullptr)
    , pass_(nullptr)
    , timeout_(nullptr)
{
    createOutlet();
    createOutlet();

    host_ = new SymbolProperty("@host", gensym("localhost"));
    host_->setArgIndex(0);
    addProperty(host_);

    port_ = new IntProperty("@port", 80);
    port_->checkClosedRange(1, 32 * 1024);
    port_->setArgIndex(1);
    addProperty(port_);

    user_ = new SymbolProperty("@user", &s_);
    addProperty(user_);

    pass_ = new AtomProperty("@pass", Atom(&s_));
    addProperty(pass_);

    timeout_ = new IntProperty("@timeout", 1);
    timeout_->checkClosedRange(1, 10);
    timeout_->setUnits(PropValueUnits::SEC);
    addProperty(timeout_);
}

void NetHttpSend::processRequest(const HttpRequest& req, ResultCallback cb)
{
    logger_.verbose(fmt::format("GET {}:{}@{}:{}{}", req.user, req.pass, req.host, req.port, req.path));

    httplib::Client cli(req.host, req.port);
    cli.set_basic_auth(req.user.c_str(), req.pass.c_str());
    cli.set_connection_timeout(req.timeout);
    cli.set_tcp_nodelay(true);
    auto res = cli.Get(req.path.c_str());
    if (res)
        cb(HttpResult { res->body, res->status });
    else
        workerThreadError(fmt::format("http request error: '{}'", to_string(res.error())));
}

void NetHttpSend::processResult(const HttpResult& res)
{
    atomTo(1, StringAtom(res.content));
    floatTo(0, res.code);
}

std::string NetHttpSend::makePath(t_symbol* path, const AtomListView& lv)
{
    std::string res = path->s_name;

    if (lv.size() > 1 && lv.size() % 2 == 0) {
        res += '?';
        for (size_t i = 0; i < lv.size(); i += 2) {
            res += to_string(lv[i]);
            res += '=';
            res += httplib::detail::encode_query_param(to_string(lv[i + 1]));
            res += '&';
        }
    }

    if (!res.empty() && res.back() == '&')
        res.pop_back();

    return res;
}

void NetHttpSend::m_get(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL))
        return;

    HttpRequest req;
    req.host = host_->str();
    req.user = user_->str();
    req.pass = to_string(pass_->value());
    req.path = makePath(lv.symbolAt(0, gensym("/")), lv.subView(1));
    req.port = port_->value();
    req.timeout = timeout_->value();

    if (!addRequest(req)) {
        OBJ_ERR << "can't send command to worker";
        return;
    }

    if (!isRunning() && !runTask())
        OBJ_ERR << "can't run worker";
}

void setup_net_http_send()
{
    // LIB_DBG << "httplib-cpp version: " << CPPHTTPLIB_VERSION;

    ObjectFactory<NetHttpSend> obj("net.http.send");
    obj.addAlias("http.send");
    obj.addMethod("get", &NetHttpSend::m_get);
    obj.setDeprecated();
    obj.setUseInstead("net.http.client");

    obj.setXletsInfo({ "get" }, { "int: status code", "data:string: respone body" });
}
#endif
