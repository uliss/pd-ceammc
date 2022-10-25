#include "net_http_send.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_string.h"
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
    addProperty(timeout_);

    Dispatcher::instance().subscribe(this, subscriberId());
}

void NetHttpSend::processMessage(const HttpResult& msg)
{
    atomTo(1, StringAtom(msg.content));
    floatTo(0, msg.code);
}

NetHttpSend::Future NetHttpSend::createTask()
{
    return std::async(
        std::launch::async, [this]() {
            try {
                HttpRequest msg;

                while (inPipe().try_dequeue(msg)) {
                    logger_.verbose(fmt::format("GET {}:{}@{}:{}{},", msg.user, msg.pass, msg.host, msg.port, msg.path));

                    httplib::Client cli(msg.host, msg.port);
                    cli.set_basic_auth(msg.user.c_str(), msg.pass.c_str());
                    cli.set_connection_timeout(msg.timeout);
                    cli.set_tcp_nodelay(true);
                    auto res = cli.Get(msg.path.c_str());
                    if (res) {
                        outPipe().enqueue({ res->body, res->status });
                    } else {
                        logger_.error(fmt::format("http request error: '{}'", to_string(res.error())));
                    }
                }

                Dispatcher::instance().send({ subscriberId(), NOTIFY_UPDATE });

            } catch (std::exception& e) {
                logger_.error(fmt::format("http request exception: '{}'", e.what()));
            }
        });
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

    if (!inPipe().try_enqueue(std::move(req))) {
        OBJ_ERR << "can't send command to worker";
        return;
    }

    if (!runTask())
        OBJ_ERR << "can't run worker";
}

void setup_net_http_send()
{
    ObjectFactory<NetHttpSend> obj("net.http.send");
    obj.addMethod("get", &NetHttpSend::m_get);
}
