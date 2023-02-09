#include "fx_room.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxRoom : public faust_fx_room_tilde {
    Property* size_;

public:
    FxRoom(const PdArgs& args)
        : faust_fx_room_tilde(args)
        , size_(nullptr)
    {
        bindPositionalArgsToProps({ gensym("@size"), gensym("@fb") });
        createInlet();

        size_ = property("@size");
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        if (size_)
            size_->set(lv);
    }
};

void setup_fx_room_tilde()
{
    SoundExternalFactory<FxRoom> obj("fx.room~");
    obj.addMethod("reset", &FxRoom::m_reset);

    obj.setXletsInfo({ "signal: input", "float: set room size (0..3)" }, { "signal: output" });

    obj.setDescription("Gardners room emulation algorithms");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "room", "reverb"});
}
