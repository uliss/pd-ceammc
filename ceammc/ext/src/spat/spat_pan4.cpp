#include "spat_pan4.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SpatPan4 : public faust_spat_pan4_tilde {
public:
    static t_symbol* SYM_RADIUS;
    static t_symbol* SYM_ANGLE;

public:
    SpatPan4(const PdArgs& args)
        : faust_spat_pan4_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_RADIUS, SYM_ANGLE });
    }

    void onList(const AtomList& l) override
    {
        static const Atom A_RADIUS(gensym("@radius"));
        static const Atom A_ANGLE(gensym("@angle"));

        if (!checkArgs(l, ARG_FLOAT, ARG_FLOAT))
            return;

        setProperty(SYM_RADIUS, l.view(0, 1));
        setProperty(SYM_ANGLE, l.view(1, 1));
    }
};

t_symbol* SpatPan4::SYM_ANGLE;
t_symbol* SpatPan4::SYM_RADIUS;

void setup_spat_pan4_tilde()
{
    SpatPan4::SYM_RADIUS = gensym("@radius");
    SpatPan4::SYM_ANGLE = gensym("@angle");

    SoundExternalFactory<SpatPan4> obj("spat.pan4~");
    obj.addAlias("pan4~");
    obj.setXletsInfo(
        { "signal: input\n"
          "list:   radius angle" },
        { "channel 1", "channel 2", "channel 3", "channel 4" });
}
