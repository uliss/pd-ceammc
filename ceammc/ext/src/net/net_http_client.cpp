#ifndef WITH_HTTP
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetHttpClient, 1, 2, "compiled without http support");
OBJECT_STUB_SETUP(NetHttpClient, net_http_client, "net.http.client", "http.client");
#else

#include "net_http_client.h"
#include "args/argcheck2.h"
#include "ceammc_crc32.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_string.h"

NetHttpClient::NetHttpClient(const PdArgs& args)
    : NetHttpClientBase(args)
    , srv_(
          ceammc_http_client_init {},
          ceammc_http_client_new,
          ceammc_http_client_free,
          ceammc_http_client_process,
          ceammc_http_client_result_cb {
              this,
              [](void* user, const ceammc_http_client_result* res) {
                  auto this_ = static_cast<NetHttpClient*>(user);
                  if (this_ && res) {
                      if (res->data && *res->data)
                          this_->atomTo(1, StringAtom(res->data));

                      this_->floatTo(0, res->status);
                  }
              } },
          ceammc_callback_notify { reinterpret_cast<size_t>(this), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } })
{
    srv_.setErrorCallback([this](const char* msg) { OBJ_ERR << msg; });
    srv_.setDebugCallback([this](const char* msg) { OBJ_DBG << msg; });
    srv_.setProgressCallback([this](int percent) {
        anyTo(1, gensym("progress"), Atom(percent));
    });

    createOutlet();
    createOutlet();
}

void NetHttpClient::m_download(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("URL:s FILE:s? @PARAMS:s*");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto url = lv.symbolAt(0, &s_)->s_name;
    auto file = lv.symbolAt(1, &s_)->s_name;
    auto dir = canvasDir(CanvasType::PARENT)->s_name;

    std::vector<ceammc_http_client_param> params;
    processParams(lv.subView(1), params);

    if (!ceammc_http_client_download(srv_.handle(), url, file, dir, params.data(), params.size()))
        OBJ_ERR << "can't make request";
}

void NetHttpClient::m_get(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("URL:s @PARAMS:s*");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto url = lv.symbolAt(0, &s_)->s_name;

    std::vector<ceammc_http_client_param> params;
    if (!processParams(lv.subView(1), params))
        return;

    if (!ceammc_http_client_get(srv_.handle(), url, params.data(), params.size()))
        OBJ_ERR << "can't make request";
}

void NetHttpClient::m_post(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("URL:s @PARAMS:s*");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto url = lv.symbolAt(0, &s_)->s_name;

    std::vector<ceammc_http_client_param> params;
    processParams(lv.subView(1), params);

    if (!ceammc_http_client_post(srv_.handle(),
            url,
            params.data(),
            params.size()))
        OBJ_ERR << "can't make request";
}

void NetHttpClient::m_upload(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("URL:s FILE:s FILE_KEY:s @PARAMS:s*");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto url = lv.symbolAt(0, &s_)->s_name;
    auto file = lv.symbolAt(1, &s_)->s_name;
    auto file_key = lv.symbolAt(2, &s_)->s_name;

    std::vector<ceammc_http_client_param> params;
    processParams(lv.subView(3), params);

    if (!ceammc_http_client_upload(srv_.handle(),
            url,
            file,
            file_key,
            params.data(), params.size()))
        OBJ_ERR << "error while making upload file POST request";
}

bool NetHttpClient::notify(int code)
{
    srv_.processResults();
    return true;
}

bool NetHttpClient::processParams(const AtomListView& lv, std::vector<ceammc_http_client_param>& params) const
{
    bool no_err = true;

    list::foreachProperty(lv, [&params, &no_err, this](t_symbol* key, const AtomListView& lv) {
        switch (crc32_hash(key)) {
        case "@form"_hash: {
            auto k = lv.symbolAt(0, &s_);
            auto v = lv.symbolAt(1, &s_);
            if (k == &s_) {
                OBJ_ERR << "invalid form key: " << lv.atomAt(0, &s_);
                no_err = false;
                return;
            }

            if (v == &s_) {
                OBJ_ERR << "invalid form key value: " << lv.atomAt(1, &s_);
                no_err = false;
                return;
            }

            params.push_back({ k->s_name, v->s_name, ceammc_http_client_param_type::Form });
        } break;
        case "@mime"_hash: {
            auto mime = lv.symbolAt(0, &s_);
            if (mime == &s_) {
                OBJ_ERR << "invalid mime type: " << lv.atomAt(0, &s_);
                no_err = false;
                return;
            }

            params.push_back({ mime->s_name, "", ceammc_http_client_param_type::Mime });
        } break;
        case "@multipart"_hash: {
            auto k = lv.symbolAt(0, &s_);
            auto v = lv.symbolAt(1, &s_);
            if (k == &s_) {
                OBJ_ERR << "invalid multipart form key: " << lv.atomAt(0, &s_);
                no_err = false;
                return;
            }

            if (v == &s_) {
                OBJ_ERR << "invalid multipart form key value: " << lv.atomAt(1, &s_);
                no_err = false;
                return;
            }

            params.push_back({ k->s_name, v->s_name, ceammc_http_client_param_type::MultiPart });
        } break;
        case "@header"_hash: {
            auto k = lv.symbolAt(0, &s_);
            auto v = lv.symbolAt(1, &s_);
            if (k == &s_) {
                OBJ_ERR << "invalid header key: " << lv.atomAt(0, &s_);
                no_err = false;
                return;
            }

            if (v == &s_) {
                OBJ_ERR << "invalid header value: " << lv.atomAt(1, &s_);
                no_err = false;
                return;
            }

            params.push_back({ k->s_name, v->s_name, ceammc_http_client_param_type::Header });
        } break;
        case "@auth"_hash: {
            auto name = lv.symbolAt(0, &s_);
            auto pass = lv.symbolAt(1, &s_);
            if (name == &s_) {
                OBJ_ERR << "invalid username: " << lv.atomAt(0, &s_);
                no_err = false;
                return;
            }

            if (pass == &s_) {
                OBJ_ERR << "invalid password: " << lv.atomAt(1, &s_);
                no_err = false;
                return;
            }

            params.push_back({ name->s_name, pass->s_name, ceammc_http_client_param_type::BasicAuth });
        } break;
        case "@css"_hash: {
            auto css = lv.symbolAt(0, &s_);
            auto output = lv.symbolAt(1, &s_);
            if (css == &s_) {
                OBJ_ERR << "invalid CSS selector name: " << lv.atomAt(0, &s_);
                no_err = false;
                return;
            }

            switch (crc32_hash(output)) {
            case "inner"_hash:
            case "html"_hash:
            case "text"_hash:
            case "href"_hash:
            case "none"_hash:
                break;
            default:
                OBJ_ERR << "invalid output value: " << lv.atomAt(1, &s_) << ", expected: html|inner|text|none";
                no_err = false;
                return;
            }

            params.push_back({ css->s_name, output->s_name, ceammc_http_client_param_type::Selector });
        } break;
        case "@strip_ws"_hash: {
            params.push_back({ "", "", ceammc_http_client_param_type::StripWhiteSpaces });
        } break;
        default:
            break;
        }
    });

    return no_err;
}

void setup_net_http_client()
{
    ObjectFactory<NetHttpClient> obj("net.http.client");
    obj.addAlias("http.client");

    obj.addMethod("get", &NetHttpClient::m_get);
    obj.addMethod("post", &NetHttpClient::m_post);
    obj.addMethod("upload", &NetHttpClient::m_upload);
    obj.addMethod("download", &NetHttpClient::m_download);

    obj.setXletsInfo({ "get, post, upload, select" }, { "int: status code", "data:string: response body" });
}

#endif
