#include "fx_drive.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxDrive : public faust_fx_drive_tilde {
public:
    FxDrive(const PdArgs& args)
        : faust_fx_drive_tilde(args)
    {
        auto SYM_PROP_DRIVE = gensym("@drive");

        bindPositionalArgsToProps({ SYM_PROP_DRIVE });
    }
};

void setup_fx_drive_tilde()
{
    FaustFactory<FxDrive> obj("fx.drive~");

    obj.setDescription("Guitarix drive effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "drive" });
}
