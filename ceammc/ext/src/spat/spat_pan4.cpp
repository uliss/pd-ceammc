#include "spat_pan4.h"
#include "ceammc_factory.h"

using namespace ceammc;

constexpr const char* STR_RADIUS = "@radius";
constexpr const char* STR_ANGLE = "@angle";

class SpatPan4 : public faust_spat_pan4_tilde {
public:
    SpatPan4(const PdArgs& args)
        : faust_spat_pan4_tilde(args)
    {
        bindPositionalArgsToProps({ gensym(STR_RADIUS), gensym(STR_ANGLE) });
    }

    void onList(const AtomListView& lv) override
    {
        if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT))
            return;

        setProperty(gensym(STR_RADIUS), lv.subView(0, 1));
        setProperty(gensym(STR_ANGLE), lv.subView(1, 1));
    }
};

void setup_spat_pan4_tilde()
{
    SoundExternalFactory<SpatPan4> obj("spat.pan4~");
    obj.addAlias("pan4~");
    obj.setXletsInfo(
        { "signal: input\n"
          "list:   radius angle" },
        { "channel 1", "channel 2", "channel 3", "channel 4" });
}
