#include "patch_args.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"

PatchArgs::PatchArgs(const PdArgs& a)
    : BaseObject(a)
    , from_(nullptr)
{
    from_ = new IntProperty("@from", 0);
    from_->setArgIndex(0);
    from_->checkMinEq(0);
    addProperty(from_);

    createOutlet();
}

void PatchArgs::onBang()
{
    listTo(0, canvas_info_args(canvas()).subView(from_->value()));
}

void setup_patch_args()
{
    ObjectFactory<PatchArgs> obj("patch.args");

    obj.setDescription("Get patch or subpatch arguments");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "patch", "args", "arguments", "canvas" });
    obj.setCategory("patch");
    obj.setSinceVersion(0, 4);
}
