#ifndef WITH_FREESOUND
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetFreesound, 1, 1, "compiled without Freesound support");
OBJECT_STUB_SETUP(NetFreesound, net_freesound, "net.freesound");
#else

#include "args/argcheck2.h"
#include "ceammc_array.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_dict.h"
#include "fmt/core.h"
#include "net_freesound.h"

using AccessToken = SingletonMeyers<OAuthAccess>;

NetFreesound::NetFreesound(const PdArgs& args)
    : NetFreesoundBase(args)
{
    createOutlet();
    createOutlet();

    oauth_file_ = new SymbolProperty("@oauth_file", &s_);
    oauth_file_->setInitOnly();
    addProperty(oauth_file_);

    oauth_id_ = new SymbolProperty("@oauth_id", &s_);
    oauth_id_->setInitOnly();
    addProperty(oauth_id_);

    oauth_secret_ = new SymbolProperty("@oauth_secret", &s_);
    oauth_secret_->setInitOnly();
    addProperty(oauth_secret_);
}

void NetFreesound::initDone()
{
    std::string secret_path;
    if (oauth_file_->value() != &s_) {
        auto res = findInStdPaths(oauth_file_->value()->s_name);
        if (res.empty())
            OBJ_ERR << fmt::format("OAuth secret file not found: '{}'", oauth_file_->value()->s_name);
        else
            secret_path = res;
    }

    cli_.reset(new NetFreesoundImpl {
        {
            secret_path.c_str(),
            [](size_t n) -> ceammc_t_pd_rust_word* {
                // std::cerr << fmt::format("-> alloc {} elements", n) << std::endl;
                return static_cast<ceammc_t_pd_rust_word*>(getbytes(n * sizeof(t_word))); },
            [](ceammc_t_pd_rust_word* data, size_t n) {
                // std::cerr << fmt::format("-> free {} elements", n) << std::endl;
                freebytes(data, n);
            } //
        },
        ceammc_freesound_new,
        ceammc_freesound_free,
        ceammc_freesound_process,
        ceammc_freesound_result_cb {
            this,
            [](void* user, const char* url) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && url)
                    this_->processReplyOAuth(url);
            },
            [](void* user, const char* access_token, std::uint64_t expires) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && access_token)
                    this_->processReplyAccess(access_token, expires);
            },
            [](void* user, const char* id, const char* secret) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && id && secret)
                    this_->processReplyOAuthFile(id, secret);
            },
            [](void* user, const ceammc_freesound_info_me* info) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && info)
                    this_->processReplyInfoMe(*info);
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
            [](void* user, const char* filename) {
                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && filename) {
                    this_->processReplyDownload(filename);
                }
            },
            [](void* user, ceammc_freesound_array_data* data, size_t size) {
                static_assert(sizeof(t_word) == sizeof(ceammc_t_pd_rust_word), "");
                static_assert(offsetof(t_word, w_float) == offsetof(ceammc_t_pd_rust_word, w_float), "");

                auto this_ = static_cast<NetFreesound*>(user);
                if (this_ && data) {
                    this_->processReplyLoad(data, size);
                }
            },
            //
        },
        ceammc_callback_notify { reinterpret_cast<size_t>(this), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } } });

    cli_->setErrorCallback([this](const char* msg) { OBJ_ERR << msg; });
    cli_->setDebugCallback([this](const char* msg) { OBJ_DBG << msg; });
    cli_->setProgressCallback([this](int percent) {
        if (percent == 0)
            startpost("%s", ".");
        else if (percent == 100)
            endpost();
        else
            poststring(".");

        anyTo(1, gensym("progress"), Atom(percent));
    });
}

bool NetFreesound::notify(int code)
{
    if (cli_)
        ceammc_freesound_process(cli_->handle());

    return true;
}

void NetFreesound::m_download(t_symbol* s, const AtomListView& lv)
{
    if (!cli_ || !checkOAuth(s))
        return;

    static const args::ArgChecker chk("ID:i @PARAMS:a*");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto id = lv.intAt(0, 0);
    ceammc_freesound_download_file(cli_->handle(), id, AccessToken::instance().token.c_str(), canvasDir(CanvasType::TOPLEVEL)->s_name);
}

void NetFreesound::m_load(t_symbol* s, const AtomListView& lv)
{
    if (!cli_ || !checkOAuth(s))
        return;

    static const args::ArgChecker chk("ID:i>=0 @PARAMS:a*");
    static const args::ArgChecker chk_array("ARRAY:s CHAN:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    bool norm = false;
    std::vector<ceammc_freesound_array> arrays;
    list::foreachProperty(lv.properties(), [&norm, &arrays, this](t_symbol* s, const AtomListView& lv) {
        switch (crc32_hash(s)) {
        case "@arr"_hash:
        case "@array"_hash:
            if (!chk_array.check(lv, this))
                chk_array.usage(this);

            arrays.push_back({ lv.symbolAt(0, &s_)->s_name, static_cast<size_t>(lv.intGreaterEqualAt(1, 0, 0)) });
            break;
        case "@norm"_hash:
        case "@normalize"_hash:
            norm = true;
            break;
        default:
            break;
        }
    });

    auto file_id = lv.intAt(0, 0);

    ceammc_freesound_load_to_arrays(
        cli_->handle(),
        file_id,
        arrays.data(),
        arrays.size(),
        norm,
        AccessToken::instance().token.c_str());
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
    std::vector<const char*> fields, descriptors;

    ceammc_freesound_search_params params {
        query,
        "",
        "",
        fields.data(), fields.size(), descriptors.data(), descriptors.size(), 1, 32, false, false
    };

    list::foreachProperty(lv.subView(1), [&params, &fields, &descriptors](t_symbol* k, const AtomListView& args) {
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
        case "@desc"_hash:
        case "@descriptors"_hash:
            for (auto& a : args) {
                if (!a.isSymbol())
                    continue;

                auto sym = a.asSymbol();
                if (sym != &s_) {
                    descriptors.push_back(sym->s_name);
                    params.descriptors = descriptors.data(); // important!
                    params.num_descriptors++;
                }
            }
            break;
        default:
            break;
        }
    });

    ceammc_freesound_search(cli_->handle(), AccessToken::instance().token.c_str(), params);
}

void NetFreesound::m_access(t_symbol* s, const AtomListView& lv)
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

void NetFreesound::processReplyOAuth(const char* url)
{
    OBJ_DBG << "oauth url: " << url;
    pdgui_vmess("::pd_menucommands::menu_openfile", "s", url);
}

void NetFreesound::processReplyOAuthFile(const char* id, const char* secret)
{
    oauth_id_->setValue(gensym(id));
    oauth_secret_->setValue(gensym(secret));
    OBJ_DBG << "OAuth id/secret loaded from file: " << oauth_file_->value()->s_name;
}

void NetFreesound::processReplyAccess(const char* access_token, std::uint64_t expires)
{
    AccessToken::instance().token = access_token;
    AccessToken::instance().expires = expires;
    OBJ_DBG << "oauth success";
}

void NetFreesound::processReplyInfoMe(const ceammc_freesound_info_me& info)
{
    DictAtom data;

    data->insert("id", info.id);
    for (size_t i = 0; i < info.str_props_len; i++) {
        auto& prop = info.str_props[i];
        data->insert(prop.name, gensym(prop.value));
    }

    anyTo(0, gensym("me"), data);
}

void NetFreesound::processReplySearchInfo(uint64_t id, std::uint32_t prev, std::uint32_t next)
{
    AtomArray<4> data { gensym("info"), id, prev, next };
    anyTo(0, gensym("search"), data.view());
}

void NetFreesound::processReplySearch(uint64_t i, const ceammc_freesound_search_result& res)
{
    DictAtom da;
    da->insert("id", res.id);

    // str props
    for (size_t i = 0; res.str_props && i < res.str_props_len; i++) {
        auto& prop = res.str_props[i];
        da->insert(prop.name, gensym(prop.value));
    }

    // num props
    for (size_t i = 0; res.num_props && i < res.num_props_len; i++) {
        auto& prop = res.num_props[i];
        da->insert(prop.name, prop.value);
    }

    // obj props
    for (size_t i = 0; res.obj_props && i < res.obj_props_len; i++) {
        auto& prop = res.obj_props[i];

        DictAtom objs;
        for (size_t j = 0; j < prop.len; j++) {
            auto& obj = prop.data[j];
            objs->insert(obj.name, obj.value);
        }
        if (prop.len > 0)
            da->insert(prop.name, objs);
    }

    // tags
    AtomList tags;
    for (size_t i = 0; res.tags && i < res.tags_len; i++) {
        auto tag = res.tags[i];
        if (tag)
            tags.append(gensym(tag));
    }

    if (tags.size() > 0)
        da->insert("tags", tags);

    // output
    anyTo(0, gensym("search"), da);
}

void NetFreesound::processReplyDownload(const char* filename)
{
    OBJ_DBG << "file downloaded to: " << filename;
    anyTo(0, gensym("downloaded"), gensym(filename));
}

void NetFreesound::processReplyLoad(ceammc_freesound_array_data* data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        auto arr = ceammc_freesound_array_data_at(i, data, len);
        auto array_name = ceammc_freesound_array_data_name(arr);
        auto array_size = ceammc_freesound_array_data_size(arr);
        auto array_chan = ceammc_freesound_array_data_channel(arr);
        auto arr_data = static_cast<t_word*>(
            static_cast<void*>(
                const_cast<ceammc_t_pd_rust_word*>(ceammc_freesound_array_data_ptr(arr))));

        OBJ_DBG << fmt::format("array loaded: '{}' [channel: {}]", array_name, array_chan);

        auto aname = gensym(array_name);
        Array array(aname);
        if (!array.isValid()) {
            OBJ_ERR << fmt::format("array not found: '{}'", array_name);
            return;
        }

        if (!array.setData(arr_data, array_size)) {
            OBJ_ERR << fmt::format("can't set array data: {}", array_name);
            return;
        }

        // take ownership
        ceammc_freesound_array_data_retain(arr);
        array.redraw();
        OBJ_DBG << fmt::format("array '{}': set new data [{}]", array_name, array_size);
    }

    anyTo(0, gensym("loaded"), AtomListView {});
}

bool NetFreesound::checkOAuth(t_symbol* s) const
{
    if (AccessToken::instance().token.empty()) {
        METHOD_ERR(s) << "not logged with OAuth. "
                         "Use oauth auth to get authorization code in browser (valid 24 hours) and then: oauth code CODE";
        return false;
    } else
        return true;
}

void setup_net_freesound()
{
    ObjectFactory<NetFreesound> obj("net.freesound");

    obj.addMethod("access", &NetFreesound::m_access);
    obj.addMethod("download", &NetFreesound::m_download);
    obj.addMethod("load", &NetFreesound::m_load);
    obj.addMethod("me", &NetFreesound::m_me);
    obj.addMethod("search", &NetFreesound::m_search);
}

#endif
