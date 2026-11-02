#ifndef WITH_MARKOV
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(SeqMarkov, 1, 1, "compiled without Markov chains support");
OBJECT_STUB_SETUP(SeqMarkov, setup_seq_markov, "seq.markov");
#else

#include "seq_markov.h"

#include "ceammc_args.h"
#include "ceammc_factory.h"
#include "lex/parser_music.h"
#include "rust_markov.hpp"

#include "fmt/core.h"

using namespace ceammc;

SeqMarkov::SeqMarkov(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
    , markov_(nullptr, &ceammc_markov_free)
{
    createOutlet();
    createOutlet();

    order_ = new IntEnumProperty("@order", { 1, 2, 0 });
    order_->setInitOnly();
    order_->setArgIndex(0);
    addProperty(order_);

    seed_ = new IntProperty("@seed", 0);
    seed_->setInitOnly();
    addProperty(seed_);
}

void SeqMarkov::initDone()
{
    auto order = ceammc_markov_order::Order1;
    switch(order_->value()) {
    case 0:
        order = ceammc_markov_order::Order0;
    case 1:
        order =  ceammc_markov_order::Order1;
    case 2:
        order = ceammc_markov_order::Order2;
    default:
        OBJ_ERR << fmt::format("invalid markov chain order: {}", order_->value());
        break;
    }

    markov_.reset(ceammc_markov_new(order, seed_->value(), //
        { subscriberId(), [](size_t id) { Dispatcher::instance().send(NotifyMessage { id, 0 }); } }, //
        {
            this, [](void* user, ceammc_msg_level level, const char* msg) {
             auto this_ = static_cast<SeqMarkov*>(user);

             if (this_) {
                 switch (level) {
                 case ceammc_msg_level::Error:
                     Error(this_) << msg;
                     break;
                 case ceammc_msg_level::Post:
                     Post(this_) << msg;
                     break;
                 case ceammc_msg_level::Debug:
                     Debug(this_) << msg;
                     break;
                 case ceammc_msg_level::Log:
                     Log(this_) << msg;
                     break;
                 default:
                     break;
                 } } } //
        }, //
        {
            this, [](void* user, ceammc_markov_note note) {
                auto* obj = static_cast<SeqMarkov*>(user);
                if (!obj)
                    return;

                obj->floatTo(1, static_cast<std::uint8_t>(note.dur_bin));
                obj->floatTo(0, note.pitch);
            } } //
        ));
}

bool SeqMarkov::notify(int code)
{
    return ceammc_markov_process(markov_.get());
}

void SeqMarkov::onBang()
{
    ceammc_markov_bang(markov_.get());
}

void SeqMarkov::m_clear(t_symbol *s, const AtomListView &lv)
{
    ceammc_markov_clear(markov_.get());
}

void SeqMarkov::m_reset(t_symbol *s, const AtomListView &lv)
{
    ceammc_markov_reset(markov_.get());
}

void SeqMarkov::m_train(t_symbol* s, const AtomListView& lv)
{
    args::ArgChecker chk("PITCH1:i[0,127] DUR1:a?");
    if (!chk.check(lv, this, s))
        return;

    std::uint8_t pitch = lv.intAt(0, -1);
    auto d = lv.atomAt(1, Atom());

    music::Duration dur;
    if(!parser::parse_duration(lv.atomAt(1, d), dur)) {
        METHOD_ERR(s) << "invalid duration: " << d;
        return;
    }

    auto ffi_dur = ceammc_markov_duration::D1;
    if(dur == music::Duration(1, 1)) {
        ffi_dur = ceammc_markov_duration::D1;
    } else if(dur == music::Duration(1, 2)) {
        ffi_dur = ceammc_markov_duration::D2;
    } else if (dur == music::Duration(1, 4)) {
        ffi_dur = ceammc_markov_duration::D4;
    }

    ceammc_markov_train(markov_.get(), ceammc_markov_note { pitch, ffi_dur });
}

void setup_seq_markov()
{
    ObjectFactory<SeqMarkov> obj("seq.markov");

    obj.addMethod("clear", &SeqMarkov::m_clear);
    obj.addMethod("reset", &SeqMarkov::m_reset);
    obj.addMethod("train", &SeqMarkov::m_train);
}

#endif
