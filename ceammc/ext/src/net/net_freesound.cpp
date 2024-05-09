#ifndef WITH_FREESOUND
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetFreesound, 1, 1, "compiled without Freesound support");
OBJECT_STUB_SETUP(NetFreesound, net_freesound, "net.freesound");
#else

#include "args/argcheck.h"
#include "ceammc_array.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "fmt/core.h"
#include "net_freesound.h"

CEAMMC_DEFINE_SYM(downloaded)
CEAMMC_DEFINE_SYM(loaded)
CEAMMC_DEFINE_SYM(progress)
CEAMMC_DEFINE_SYM(results)
CEAMMC_DEFINE_SYM(user)

using AccessToken = SingletonMeyers<OAuthAccess>;

NetFreesound::NetFreesound(const PdArgs& args)
    : NetFreesoundBase(args)
{
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
            [](size_t n) -> ceammc_t_pd_rust_word* { //
                return static_cast<ceammc_t_pd_rust_word*>(getbytes(n * sizeof(t_word)));
            },
            [](ceammc_t_pd_rust_word* data, size_t n) {
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
            [](void* user, const ceammc_freesound_search_result* res) -> void* {
                auto this_ = static_cast<NetFreesound*>(user);
                if (!this_ || !res)
                    return nullptr;

                return new DictAtom(DataTypeDict { //
                    DataTypeDict::DictEntry { "prev", res->prev },
                    DataTypeDict::DictEntry { "next", res->next },
                    DataTypeDict::DictEntry { "count", res->count } });
            },
            [](uint64_t id) -> void* {
                return new DataTypeDict({ DataTypeDict::DictEntry { "id", id } });
            },
            [](void* data, const char* tag) {
                if (!data)
                    return;

                auto dict = static_cast<DataTypeDict*>(data);

                auto key_tags = gensym("tags");
                if (dict->contains(key_tags)) {
                    dict->at(key_tags).append(gensym(tag));
                } else {
                    dict->insert(key_tags, gensym(tag));
                }
            },
            [](void* data, const char* key, double value) {
                if (!data)
                    return;

                auto dict = static_cast<DataTypeDict*>(data);

                auto sym_key = gensym(key);
                if (dict->contains(sym_key)) {
                    dict->at(sym_key).append(t_float(value));
                } else {
                    dict->insert(sym_key, t_float(value));
                }
            },
            [](void* data, const char* key, const char* value) {
                if (!data)
                    return;

                auto dict = static_cast<DataTypeDict*>(data);

                auto sym_key = gensym(key);
                if (dict->contains(sym_key)) {
                    dict->at(sym_key).append(gensym(value));
                } else {
                    dict->insert(sym_key, gensym(value));
                }
            },
            [](void* data, const char* key1, const char* key2, const double* floats, size_t num_floats) {
                if (!data)
                    return;

                auto dict = static_cast<DataTypeDict*>(data);

                auto sym_key = gensym(fmt::format("{}.{}", key1, key2).c_str());
                if (dict->contains(sym_key)) {
                    auto& ref = dict->at(sym_key);
                    ref.reserve(num_floats);
                    for (size_t i = 0; i < num_floats; i++)
                        ref.append(floats[i]);
                } else {
                    AtomList value;
                    value.reserve(num_floats);
                    for (size_t i = 0; i < num_floats; i++)
                        value.append(floats[i]);

                    dict->insert(sym_key, value);
                }
            },
            [](void* data0, void* data1) {
                if (!data0 || !data1)
                    return;

                auto dict0 = static_cast<DictAtom*>(data0);
                auto dict1 = static_cast<DataTypeDict*>(data1);

                auto key_res = gensym("results");
                if ((*dict0)->contains(key_res)) {
                    (*dict0)->at(key_res).append(DictAtom(std::move(*dict1)));
                } else {
                    (*dict0)->insert(key_res, DictAtom(std::move(*dict1)));
                }

                delete dict1;
            },
            [](void* data0, void* data1) {
                if (!data0 || !data1)
                    return;

                auto this_ = static_cast<NetFreesound*>(data0);
                auto dict = static_cast<DictAtom*>(data1);

                this_->anyTo(0, sym_results(), *dict);
                delete dict;
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

        anyTo(0, sym_progress(), Atom(percent));
    });
}

bool NetFreesound::notify(int code)
{
    if (cli_)
        ceammc_freesound_process(cli_->handle());

    return true;
}

void NetFreesound::editorClear()
{
    oauth_id_->setValue(&s_);
    oauth_secret_->setValue(&s_);
}

void NetFreesound::editorAddLine(t_symbol* sel, const AtomListView& lv)
{
    static const args::ArgChecker chk("KEY:s='@oauth_id'|'@oauth_secret' VALUE:s");
    if (!chk.check(lv, this))
        return chk.usage(this);

    if (lv[0] == "@oauth_id")
        oauth_id_->setValue(lv.symbolAt(1, &s_));

    if (lv[0] == "@oauth_secret")
        oauth_secret_->setValue(lv.symbolAt(1, &s_));
}

EditorLineList NetFreesound::getContentForEditor() const
{
    EditorLineList res;
    res.reserve(2);

    auto id = EditorStringPool::pool().allocate();
    id->append("@oauth_id ");
    id->append(oauth_id_->value()->s_name);
    auto secret = EditorStringPool::pool().allocate();
    secret->append("@oauth_secret ");
    secret->append(oauth_secret_->value()->s_name);

    return EditorLineList { id, secret };
}

void NetFreesound::editorSync()
{
    NetFreesoundBase::editorSync();
    OBJ_DBG << "id/secret updated";
}

void NetFreesound::m_download(t_symbol* s, const AtomListView& lv)
{
    if (!cli_ || !checkToken(s))
        return;

    static const args::ArgChecker chk("ID:i");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto id = lv.intAt(0, 0);
    ceammc_freesound_download_file(cli_->handle(), id, AccessToken::instance().token.c_str(), canvasDir(CanvasType::TOPLEVEL)->s_name);
}

void NetFreesound::m_load(t_symbol* s, const AtomListView& lv)
{
    if (!cli_ || !checkToken(s))
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
        AccessToken::instance().token.c_str(),
        static_cast<float>(sys_getsr()),
        std::is_same<double, t_float>::value);
}

void NetFreesound::m_me(t_symbol* s, const AtomListView& lv)
{
    if (!cli_ || !checkToken(s))
        return;

    ceammc_freesound_me(cli_->handle(), AccessToken::instance().token.c_str());
}

void NetFreesound::m_search(t_symbol* s, const AtomListView& lv)
{
    if (!cli_ || !checkToken(s))
        return;

    static const args::ArgChecker chk("QUERY:s @PARAMS:a*");
    static const args::ArgChecker chk_filters("FIELD:s VALUES:a+");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto query = lv.symbolAt(0, &s_)->s_name;
    std::vector<const char*> fields, descriptors;
    std::vector<ceammc_freesound_search_filter> filters;

    ceammc_freesound_search_params params {
        query,
        "",
        filters.data(),
        filters.size(),
        fields.data(), fields.size(), descriptors.data(), descriptors.size(), 1, 32, false, false
    };

    list::foreachProperty(lv.subView(1), [&params, &fields, &filters, &descriptors, this](t_symbol* k, const AtomListView& args) {
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
        case "@sort"_hash:
            params.sort = args.symbolAt(0, &s_)->s_name;
            break;
        case "@filter"_hash: {
            if (!chk_filters.check(args, this))
                return chk_filters.usage(this);

            filters.push_back({
                args.symbolAt(0, &s_)->s_name,
                gensym(to_string(args.subView(1)).c_str())->s_name,
            });

            params.filters = filters.data();
            params.num_filters = filters.size();
        } break;
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

        if (action == "auth") {
            if (!checkIdAndSecret())
                return;

            ceammc_freesound_oauth_get_code(cli_->handle(), oauth_id_->value()->s_name, oauth_secret_->value()->s_name);
        } else if (action == "code") {
            if (!checkIdAndSecret())
                return;

            ceammc_freesound_oauth_get_access(
                cli_->handle(),
                oauth_id_->value()->s_name,
                oauth_secret_->value()->s_name,
                lv.symbolAt(1, &s_)->s_name);
        } else if (action == "store")
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

    anyTo(0, sym_user(), data);
}

void NetFreesound::processReplyDownload(const char* filename)
{
    OBJ_DBG << "file downloaded to: " << filename;
    anyTo(0, sym_downloaded(), gensym(filename));
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
        OBJ_DBG << fmt::format("loaded {} samples into '{}' from channel [{}]", array_size, array_name, array_chan);
    }

    anyTo(0, sym_loaded(), AtomListView {});
}

bool NetFreesound::checkToken(t_symbol* s) const
{
    if (AccessToken::instance().token.empty()) {
        METHOD_ERR(s) << "not logged with OAuth. "
                         "Use [access auth( to get authorization code in browser (valid 24 hours)"
                         " and then: [access code CODE(";
        return false;
    } else
        return true;
}

bool NetFreesound::checkIdAndSecret() const
{
    if (oauth_id_->value() == &s_ || oauth_secret_->value() == &s_) {
        OBJ_ERR << fmt::format("@oauth_id or @oauth_secret are empty");
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

    NetFreesound::factoryEditorObjectInit(obj);
}

#endif
