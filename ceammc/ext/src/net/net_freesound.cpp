#ifndef WITH_FREESOUND
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetFreesound, 1, 1, "compiled without Freesound support");
OBJECT_STUB_SETUP(NetFreesound, net_freesound, "net.freesound");
#else

#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_dict.h"
#include "net_freesound.h"

using AccessToken = SingletonMeyers<OAuthAccess>;

NetFreesound::NetFreesound(const PdArgs& args)
    : NetFreesoundBase(args)
{
    createOutlet();
    createOutlet();

    token_ = new SymbolProperty("@token", &s_);
    token_->setInitOnly();
    addProperty(token_);

    oauth_id_ = new SymbolProperty("@oauth_id", &s_);
    oauth_id_->setInitOnly();
    addProperty(oauth_id_);

    oauth_secret_ = new SymbolProperty("@oauth_secret", &s_);
    oauth_secret_->setInitOnly();
    addProperty(oauth_secret_);
}

void NetFreesound::initDone()
{
    cli_.reset(new NetFreesoundImpl {
        ceammc_freesound_init { token_->value()->s_name },
        ceammc_freesound_new,
        ceammc_freesound_free,
        ceammc_freesound_process,
        ceammc_freesound_result_cb {
            this,
            [](void* user, const char* url) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && url)
                    this_->processReplyOAuth2(url);
            },
            [](void* user, const char* access_token, std::uint64_t expires) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && access_token)
                    this_->processReplyAccess(access_token, expires);
            },
            [](void* user, uint64_t id, const char* username, const char* email, const char* homepage, const char* url, const char* sounds, const char* packs) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_)
                    this_->processReplyInfoMe(id, username, email, homepage, url, sounds, packs);
            },
            [](void* user, uint64_t count, std::uint32_t prev, std::uint32_t next) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_)
                    this_->processReplySearchInfo(count, prev, next);
            },
            [](void* user, size_t i, const ceammc_freesound_search_result* res) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && res)
                    this_->processReplySearch(i, *res);
            },
            //
        },
        ceammc_callback_notify { reinterpret_cast<size_t>(this), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } } });

    cli_->setErrorCallback([this](const char* msg) { OBJ_ERR << msg; });
    cli_->setDebugCallback([this](const char* msg) { OBJ_DBG << msg; });
    cli_->setProgressCallback([this](int percent) {
        anyTo(1, gensym("progress"), Atom(percent));
    });
}

bool NetFreesound::notify(int code)
{
    if (cli_)
        ceammc_freesound_process(cli_->handle());

    return true;
}

void NetFreesound::m_download(t_symbol *s, const AtomListView &lv)
{
    if (!cli_ || !checkOAuth(s))
        return;

    static const args::ArgChecker chk("ID:i @PARAMS:a*");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto id = lv.intAt(0, 0);
    ceammc_freesound_download_file(cli_->handle(), id, AccessToken::instance().token.c_str(), canvasDir(CanvasType::TOPLEVEL)->s_name);
}

void NetFreesound::m_me(t_symbol* s, const AtomListView& lv)
{
    if (!cli_ || !checkOAuth(s))
        return;

    ceammc_freesound_me(cli_->handle(), AccessToken::instance().token.c_str());
}

void NetFreesound::m_search(t_symbol* s, const AtomListView& lv)
{
    if (!cli_ || !checkOAuth(s))
        return;

    static const args::ArgChecker chk("QUERY:s @PARAMS:a*");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto query = lv.symbolAt(0, &s_)->s_name;
    std::vector<const char*> fields;

    ceammc_freesound_search_params params {
        query,
        "",
        "",
        fields.data(), fields.size(), nullptr, 0, 1, 32, false, false
    };

    list::foreachProperty(lv.subView(1), [&params, &fields](t_symbol* k, const AtomListView& args) {
        switch (crc32_hash(k)) {
        case "@p"_hash:
            params.page = std::max<int>(1, args.intAt(0, 1));
            break;
        case "@n"_hash:
            params.page_size = std::max<int>(1, args.intAt(0, 32));
            break;
        case "@norm"_hash:
            params.normalized = true;
            break;
        case "@group_by_pack"_hash:
            params.group_by_pack = true;
            break;
        case "@fields"_hash:
            for (auto& a : args) {
                if (!a.isSymbol())
                    continue;

                auto sym = a.asSymbol();
                if (sym != &s_) {
                    fields.push_back(sym->s_name);
                    params.fields = fields.data(); // important!
                    params.num_fields++;
                }
            }
            break;
        default:
            break;
        }
    });

    ceammc_freesound_search(cli_->handle(), AccessToken::instance().token.c_str(), params);
}

void NetFreesound::m_oauth2(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("s=auth|code|load|store CODE:s?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (cli_) {
        auto action = lv[0];

        if (action == "auth")
            ceammc_freesound_oauth_get_code(cli_->handle(), oauth_id_->value()->s_name, oauth_secret_->value()->s_name);
        else if (action == "code")
            ceammc_freesound_oauth_get_access(
                cli_->handle(),
                oauth_id_->value()->s_name,
                oauth_secret_->value()->s_name,
                lv.symbolAt(1, &s_)->s_name);
        else if (action == "store")
            ceammc_freesound_oauth_store_access_token(cli_->handle(),
                AccessToken::instance().token.c_str(),
                canvasDir(CanvasType::TOPLEVEL)->s_name,
                true);
        else if (action == "load")
            ceammc_freesound_oauth_load_access_token(cli_->handle(), canvasDir(CanvasType::TOPLEVEL)->s_name);
    }
}

void NetFreesound::processReplyOAuth2(const char* url)
{
    OBJ_DBG << "oauth2 url: " << url;
    pdgui_vmess("::pd_menucommands::menu_openfile", "s", url);
}

void NetFreesound::processReplyAccess(const char* access_token, std::uint64_t expires)
{
    AccessToken::instance().token = access_token;
    AccessToken::instance().expires = expires;
    OBJ_DBG << "OAuth2 success";
}

void NetFreesound::processReplyInfoMe(uint64_t id, const char* username, const char* email, const char* homepage, const char* url, const char* sounds, const char* packs)
{
    DictAtom data;

    data->insert("id", id);
    data->insert("username", gensym(username));
    data->insert("email", gensym(email));
    data->insert("homepage", gensym(homepage));
    data->insert("url", gensym(url));
    data->insert("sounds", gensym(sounds));
    data->insert("packs", gensym(packs));

    anyTo(0, gensym("me"), data);
}

void NetFreesound::processReplySearchInfo(uint64_t id, std::uint32_t prev, std::uint32_t next)
{
    AtomArray<4> data { gensym("info"), id, prev, next };
    anyTo(0, gensym("search"), data.view());
}

void NetFreesound::processReplySearch(uint64_t i, const ceammc_freesound_search_result& res)
{
    AtomArray<6> data { gensym("result"), i, res.id, gensym(res.name), res.channels, gensym(res.file_type) };
    anyTo(0, gensym("search"), data.view());
}

bool NetFreesound::checkOAuth(t_symbol* s) const
{
    if (AccessToken::instance().token.empty()) {
        METHOD_ERR(s) << "not logged with OAuth2. "
                         "Use oauth2 auth to get authorization code in browser(valid 24 hours) and then: oauth2 code CODE";
        return false;
    } else
        return true;
}

void setup_net_freesound()
{
    ObjectFactory<NetFreesound> obj("net.freesound");

    obj.addMethod("download", &NetFreesound::m_download);
    obj.addMethod("me", &NetFreesound::m_me);
    obj.addMethod("search", &NetFreesound::m_search);
    obj.addMethod("oauth2", &NetFreesound::m_oauth2);
}

#endif
