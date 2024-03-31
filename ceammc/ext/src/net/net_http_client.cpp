#include "net_http_client.h"
#include "args/argcheck2.h"
#include "ceammc_crc32.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

NetHttpClient::NetHttpClient(const PdArgs& args)
    : NetHttpClientBase(args)
    , srv_(
          ceammc_http_client_new,
          ceammc_http_client_free,
          ceammc_http_client_process,
          ceammc_callback_notify { reinterpret_cast<size_t>(this), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } })
{
    srv_.setErrorCallback([this](const char* msg) { OBJ_ERR << msg; });
    srv_.setDebugCallback([this](const char* msg) { OBJ_DBG << msg; });
    srv_.setResultCallback([this](const ceammc_http_client_result& res) {
        atomTo(1, StringAtom(res.body));
        floatTo(0, res.status);
    });

    createOutlet();
    createOutlet();
}

void NetHttpClient::m_get(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL))
        return;

    auto url = lv.symbolAt(0, gensym("/"))->s_name;

    if (!ceammc_http_client_get(srv_.handle(), url, nullptr, ceammc_http_client_select_type::Html))
        OBJ_ERR << "can't make request";
}

void NetHttpClient::m_select(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CSS:s URL:s TYPE:s=@text|@html|@inner?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto css = lv.symbolAt(0, &s_)->s_name;
    auto url = lv.symbolAt(1, &s_)->s_name;
    auto stype = lv.symbolAt(2, gensym("@html"));
    auto sel_type = ceammc_http_client_select_type::Html;

    switch (crc32_hash(stype)) {
    case "@text"_hash:
        sel_type = ceammc_http_client_select_type::Text;
        break;
    case "@inner"_hash:
        sel_type = ceammc_http_client_select_type::InnerHtml;
        break;
    case "@html"_hash:
    default:
        sel_type = ceammc_http_client_select_type::Html;
        break;
    }

    if (!ceammc_http_client_get(srv_.handle(), url, css, sel_type))
        OBJ_ERR << "can't make request";
}

bool NetHttpClient::notify(int code)
{
    srv_.processResults();
    return true;
}

void setup_net_http_client()
{
    ObjectFactory<NetHttpClient> obj("net.http.client");
    obj.addAlias("http.client");
    obj.addMethod("get", &NetHttpClient::m_get);
    obj.addMethod("select", &NetHttpClient::m_select);

    obj.setXletsInfo({ "get" }, { "int: status code", "data:string: response body" });
}
