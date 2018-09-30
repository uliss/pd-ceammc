#include "fx_drive.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_DRIVE = gensym("@drive");

class FxDrive : public faust_fx_drive_tilde {
public:
    FxDrive(const PdArgs& args)
        : faust_fx_drive_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_DRIVE });
    }
};

void setup_fx_drive_tilde()
{
    SoundExternalFactory<FxDrive> obj("fx.drive~");
}
