#ifndef SEQ_MARKOV_H
#define SEQ_MARKOV_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property_enum.h"

#include <memory>

struct ceammc_markov;

namespace ceammc {
class SeqMarkov : public DispatchedObject<BaseObject> {
    std::unique_ptr<ceammc_markov, void (*)(ceammc_markov*)> markov_;
    IntEnumProperty* order_;
    IntProperty* seed_;

public:
    explicit SeqMarkov(const PdArgs& args);

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;

    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_train(t_symbol* s, const AtomListView& lv);
};
}

void setup_seq_markov();

#endif // SEQ_MARKOV_H
