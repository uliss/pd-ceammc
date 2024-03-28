#include "net_http_client.h"
#include "args/argcheck2.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

NetHttpClient::NetHttpClient(const PdArgs& args)
    : NetHttpClientBase(args)
{
    createOutlet();
    createOutlet();

#define THIS_AUTOCAST(user) auto this_ = static_cast<NetHttpClient*>(user);

    cli_ = ceammc_http_client_new(
        {
            this, [](void* user, const char* msg) {
                THIS_AUTOCAST(user);
                if (this_)
                    this_->workerThreadError(msg);
            } //
        },
        {
            this, [](void* user, std::uint16_t status, const char* body) {
                THIS_AUTOCAST(user);
                if (this_) {
                    this_->addReply(HttpCliResult { body, status });
                }
            } //
        },
        {
            this, [](void* user) -> bool {
                THIS_AUTOCAST(user);
                return this_ ? (bool)this_->quit() : false;
            } //
        });
}

NetHttpClient::~NetHttpClient()
{
    finish();
    ceammc_http_client_free(cli_);
}

void NetHttpClient::processRequest(const HttpCliRequest& req, ResultCallback cb)
{
}

void NetHttpClient::processResult(const HttpCliResult& res)
{
    atomTo(1, StringAtom(res.content));
    floatTo(0, res.code);
}

NetHttpClient::Future NetHttpClient::createTask()
{
    return std::async([this]() {
        if (!ceammc_http_client_runloop(cli_))
            workerThreadError("can't start runloop");
    });
}

void NetHttpClient::m_get(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL))
        return;

    auto url = lv.symbolAt(0, gensym("/"))->s_name;

    if (!ceammc_http_client_get(cli_, url, nullptr))
        OBJ_ERR << "can't make request";
}

void NetHttpClient::m_select(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CSS:s URL:s");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto css = lv.symbolAt(0, &s_)->s_name;
    auto url = lv.symbolAt(1, &s_)->s_name;

    if (!ceammc_http_client_get(cli_, url, css))
        OBJ_ERR << "can't make request";
}

void setup_net_http_client()
{
    ObjectFactory<NetHttpClient> obj("net.http.client");
    obj.addAlias("http.client");
    obj.addMethod("get", &NetHttpClient::m_get);
    obj.addMethod("select", &NetHttpClient::m_select);

    obj.setXletsInfo({ "get" }, { "int: status code", "data:string: respone body" });
}
