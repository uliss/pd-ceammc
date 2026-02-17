#include "proto_vlc.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "parser_vlc.h"
#include "proto_rust.hpp"

namespace {

ceammc_rust_atom to_rust(const Atom& a)
{
    ceammc_rust_value val;
    if (a.isFloat()) {
        val.float_val = a.asT<t_float>();
        return ceammc_rust_atom { val, ceammc_rust_atom_type::Float };
    } else if (a.isSymbol()) {
        val.str_val = a.asT<t_symbol*>()->s_name;
        return ceammc_rust_atom { val, ceammc_rust_atom_type::String };
    } else {
        val.float_val = 0;
        return ceammc_rust_atom { val, ceammc_rust_atom_type::Null };
    }
}

} // namespace

ProtoVlc::ProtoVlc(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
    , host_(nullptr)
    , port_(nullptr)
    , pass_(nullptr)
    , vlc_ { nullptr, &ceammc_vlc_free }
{
    createOutlet();
    createOutlet();

    host_ = new SymbolProperty("@host", gensym("localhost"));
    host_->setArgIndex(0);
    addProperty(host_);

    port_ = new IntProperty("@port", 8080);
    port_->checkClosedRange(1, 1024 * 32);
    addProperty(port_);

    pass_ = new AtomProperty("@pass", Atom(&s_));
    addProperty(pass_);
}

void ProtoVlc::initDone()
{
    auto pass = to_string(pass_->value());
    vlc_.reset(ceammc_vlc_create( //
        host_->cstr(),
        port_->value(),
        pass.c_str(),
        { this, [](void* user, const char* msg) {} },
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }));
}

void ProtoVlc::m_play(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("ID:i?");
    if (!chk.check(lv, this, s))
        chk.usage(this, s);

    ceammc_vlc_play(vlc_.get(), to_rust(lv.atomAt(0, {})));
}

void ProtoVlc::m_stop(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_stop(vlc_.get());
}

void ProtoVlc::m_pause(t_symbol* s, const AtomListView& lv)
{
    proto_rust::Option<bool> value;
    if (lv.isBool())
        value.set(lv.asBool(true));

    ceammc_vlc_pause(vlc_.get(), value.get());
}

void ProtoVlc::m_fullscreen(t_symbol* s, const AtomListView& lv)
{
    proto_rust::Option<bool> value;
    if (lv.isBool())
        value.set(lv.asBool(true));

    ceammc_vlc_fullscreen(vlc_.get(), value.get());
}

void ProtoVlc::m_seek(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "TIME expected";
        return;
    }

    if (!parser::check_vlc_seek(lv[0])) {
        METHOD_ERR(s) << "invalid TIME: " << lv;
        return;
    }

    ceammc_vlc_seek(vlc_.get(), to_rust(lv[0]));
}

void ProtoVlc::m_clear(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_clear(vlc_.get());
}

void ProtoVlc::m_next(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_next(vlc_.get());
}

void ProtoVlc::m_prev(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_prev(vlc_.get());
}

void ProtoVlc::m_sort(t_symbol* s, const AtomListView& lv)
{
    enum ceammc_vlc_sort_order order = ceammc_vlc_sort_order::Normal;
    AtomListView sort_order;
    if (lv.getProperty(gensym("@order"), sort_order)) {
        if (sort_order.asBool(false))
            order = ceammc_vlc_sort_order::Reversed;
    }

    auto sort_by_sym = &s_;
    AtomListView sort_by;
    if (lv.getProperty(gensym("@by"), sort_by)) {
        sort_by_sym = sort_by.asSymbol(&s_);
    }

    ceammc_vlc_sort(vlc_.get(), sort_by_sym->s_name, order);
}

void ProtoVlc::m_loop(t_symbol* s, const AtomListView& lv)
{
    proto_rust::Option<bool> value;
    if (lv.isBool())
        value.set(lv.asBool(true));

    ceammc_vlc_loop(vlc_.get(), value.get());
}

void ProtoVlc::m_repeat(t_symbol* s, const AtomListView& lv)
{
    proto_rust::Option<bool> value;
    if (lv.isBool())
        value.set(lv.asBool(true));

    ceammc_vlc_repeat(vlc_.get(), value.get());
}

void ProtoVlc::m_volume(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_volume(vlc_.get(), to_rust(lv.atomAt(0, {})), to_rust(lv.atomAt(1, {})));
}

void ProtoVlc::m_speed(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("SPEED:f[0.25,4]", lv, this, &s_))
        return;

    ceammc_vlc_playback_rate(vlc_.get(), lv[0].asFloat());
}

void ProtoVlc::m_delete(t_symbol* s, const AtomListView& lv)
{
    AtomListView name, pos, id;
    if (lv.getProperty(gensym("@name"), name)) {
        auto str_name = to_string(name);
        ceammc_vlc_delete_by_name(vlc_.get(), to_rust(Atom(gensym(str_name.c_str()))));
    } else if (lv.getProperty(gensym("@id"), id)) {
        ceammc_vlc_delete_by_id(vlc_.get(), id.asInt());
    } else if (lv.getProperty(gensym("@pos"), pos)) {
        ceammc_vlc_delete_at_pos(vlc_.get(), pos.asInt());
    } else {
        OBJ_ERR << "item info expected: @name, @id or @pos";
    }
}

void ProtoVlc::m_add_uri(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("URI:s PLAY:b?");
    if (!chk.check(lv, this, &s_))
        return;

    ceammc_vlc_add_uri(vlc_.get(), lv[0].asSymbol()->s_name, lv.boolAt(1, false));
}

void ProtoVlc::m_add_dir(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("DIR:s GLOB:s");
    if (!chk.check(lv, this, &s_))
        return;

    ceammc_vlc_add_dir_files(vlc_.get(), lv.symbolAt(0, &s_)->s_name, lv.symbolAt(1, &s_)->s_name);
}

void ProtoVlc::m_get_current(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_get_current(vlc_.get());
}

void ProtoVlc::m_get_status(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_get_status(vlc_.get());
}

void ProtoVlc::m_get_playlist(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_get_playlist(vlc_.get());
}

void ProtoVlc::m_browse(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "URI expected";
        return;
    }

    const char* filter_type = nullptr;
    const char* match_glob = nullptr;

    AtomListView _lv;
    if (lv.getProperty(gensym("@dir"), _lv)) {
        filter_type = "dir";
    } else if (lv.getProperty(gensym("@file"), _lv)) {
        filter_type = "file";
    }

    if (lv.getProperty(gensym("@glob"), _lv)) {
        match_glob = _lv.asSymbol()->s_name;
    }

    ceammc_vlc_browse(vlc_.get(), lv.asSymbol()->s_name, filter_type, match_glob);
}

bool ProtoVlc::notify(int code)
{
    return ceammc_vlc_poll(vlc_.get(),
        { this, [](void* user, const char* msg) {
             auto obj = static_cast<ProtoVlc*>(user);
             Error(obj) << msg;
         } },
        { this, [](void* user, const ceammc_vlc_status* stat) {
             auto obj = static_cast<ProtoVlc*>(user);
             if (!obj)
                 return;

             DictAtom da;

             da->insert("apiversion", stat->apiversion);
             da->insert("loop", stat->has_loop);
             da->insert("repeat", stat->repeat);
             da->insert("random", stat->random);
             da->insert("fullscreen", stat->fullscreen);
             da->insert("position", stat->position);
             da->insert("volume", stat->volume);
             da->insert("time", stat->time);
             da->insert("length", stat->length);
             da->insert("rate", stat->rate);
             da->insert("current_id", stat->currentplid);

             obj->anyTo(1, gensym("status"), da);
         } },
        {
            this,
            [](void* user, const ceammc_vlc_playlist* playlist) {
                auto obj = static_cast<ProtoVlc*>(user);

                MListAtom pl;
                ceammc_vlc_playlist_iter(playlist->items,
                    playlist->size,
                    {
                        &pl,
                        [](void* user, const ceammc_vlc_playlist_item* item) {
                            auto& pl = *static_cast<MListAtom*>(user);
                            DictAtom da;
                            da->insert("id", item->id);
                            da->insert("name", gensym(item->name));
                            da->insert("uri", gensym(item->uri));
                            da->insert("duration", item->duration);
                            da->insert("current", item->current);
                            pl->append(da);
                        },
                    });

                obj->anyTo(1, gensym("playlist"), pl);
            },
        },
        {
            this,
            [](void* user, const ceammc_vlc_playlist_item* item) {
                auto obj = static_cast<ProtoVlc*>(user);
                DictAtom da;
                da->insert("id", item->id);
                da->insert("name", gensym(item->name));
                da->insert("uri", gensym(item->uri));
                da->insert("duration", item->duration);
                da->insert("current", item->current);
                obj->anyTo(1, gensym("current"), da);
            },
        },
        {
            this,
            [](void* user, const ceammc_vlc_filelist* filelist) {
                auto obj = static_cast<ProtoVlc*>(user);
                MListAtom pl;

                ceammc_vlc_filelist_iter(filelist->files, filelist->size,
                    ceammc_vlc_fileinfo_cb {
                        &pl,
                        [](void* user, const ceammc_vlc_fileinfo* info) {
                            auto& pl = *static_cast<MListAtom*>(user);
                            DictAtom da;
                            da->insert("name", gensym(info->name));
                            da->insert("uri", gensym(info->uri));
                            da->insert("path", gensym(info->path));
                            da->insert("size", info->size);
                            da->insert("type", (int)info->type_);
                            pl->append(da);
                        },
                    });

                obj->anyTo(1, gensym("browse"), pl);
            },
        });
}

void setup_proto_vlc()
{
    ObjectFactory<ProtoVlc> obj("proto.vlc");

    obj.addMethod("add", &ProtoVlc::m_add_uri);
    obj.addMethod("add_dir", &ProtoVlc::m_add_dir);
    obj.addMethod("browse", &ProtoVlc::m_browse);
    obj.addMethod("clear", &ProtoVlc::m_clear);
    obj.addMethod("delete", &ProtoVlc::m_delete);
    obj.addMethod("fs", &ProtoVlc::m_fullscreen);
    obj.addMethod("get_current", &ProtoVlc::m_get_current);
    obj.addMethod("get_playlist", &ProtoVlc::m_get_playlist);
    obj.addMethod("get_status", &ProtoVlc::m_get_status);
    obj.addMethod("loop", &ProtoVlc::m_loop);
    obj.addMethod("next", &ProtoVlc::m_next);
    obj.addMethod("pause", &ProtoVlc::m_pause);
    obj.addMethod("play", &ProtoVlc::m_play);
    obj.addMethod("prev", &ProtoVlc::m_prev);
    obj.addMethod("repeat", &ProtoVlc::m_repeat);
    obj.addMethod("seek", &ProtoVlc::m_seek);
    obj.addMethod("sort", &ProtoVlc::m_sort);
    obj.addMethod("speed", &ProtoVlc::m_speed);
    obj.addMethod("stop", &ProtoVlc::m_stop);
    obj.addMethod("volume", &ProtoVlc::m_volume);
}
