#include "proto_vlc.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "parser_vlc.h"

using opt_i16 = ceammc::proto_rust::Option<std::int16_t>;

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
    opt_i16 id;
    if (lv.isInteger())
        id.set(lv[0].asInt());

    ceammc_vlc_play(vlc_.get(), id.get());
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
    } else if (lv.getProperty(gensym("@pos"), pos)) {
    } else {
        OBJ_ERR << "item info expected: @name, @id or @pos";
    }
}

void ProtoVlc::m_add(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("URI:s PLAY:b?", lv, this, &s_))
        return;

    ceammc_vlc_add_uri(vlc_.get(), lv[0].asSymbol()->s_name, lv.boolAt(1, false));
}

void ProtoVlc::m_status(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_get_status(vlc_.get());
}

void ProtoVlc::m_playlist(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_get_playlist(vlc_.get());
}

void ProtoVlc::m_browse(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "URI expected";
        return;
    }

    // VlcCommand cmd;
    // cmd.code = VLC_CMD_BROWSE;
    // cmd.data = to_string(lv);
    // sendCommand(s, cmd);
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

             obj->atomTo(1, da);
         } },
        { this, [](void* user, const ceammc_vlc_playlist* playlist) {
             auto obj = static_cast<ProtoVlc*>(user);
             Error(obj) << "count: " << playlist->size;
         } });
}

void setup_proto_vlc()
{
    ObjectFactory<ProtoVlc> obj("proto.vlc");

    obj.addMethod("add", &ProtoVlc::m_add);
    obj.addMethod("clear", &ProtoVlc::m_clear);
    obj.addMethod("delete", &ProtoVlc::m_delete);
    obj.addMethod("fs", &ProtoVlc::m_fullscreen);
    obj.addMethod("loop", &ProtoVlc::m_loop);
    obj.addMethod("next", &ProtoVlc::m_next);
    obj.addMethod("pause", &ProtoVlc::m_pause);
    obj.addMethod("play", &ProtoVlc::m_play);
    obj.addMethod("prev", &ProtoVlc::m_prev);
    obj.addMethod("repeat", &ProtoVlc::m_repeat);
    obj.addMethod("seek", &ProtoVlc::m_seek);
    obj.addMethod("sort", &ProtoVlc::m_sort);
    obj.addMethod("speed", &ProtoVlc::m_speed);
    obj.addMethod("status", &ProtoVlc::m_status);
    obj.addMethod("stop", &ProtoVlc::m_stop);
    obj.addMethod("volume", &ProtoVlc::m_volume);

    obj.addMethod("browse", &ProtoVlc::m_browse);

    obj.addMethod("playlist", &ProtoVlc::m_playlist);
}
