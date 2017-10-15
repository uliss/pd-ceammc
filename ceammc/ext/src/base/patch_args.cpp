#include "patch_args.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"

PatchArgs::PatchArgs(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void PatchArgs::onBang()
{
    listTo(0, canvas_info_args(canvas()));
}

void setup_patch_args()
{
    ObjectFactory<PatchArgs> obj("patch.args");
}
