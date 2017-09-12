#include "canvas_args.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"

CanvasArgs::CanvasArgs(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void CanvasArgs::onBang()
{
    listTo(0, canvas_info_args(canvas()));
}

void setup_canvas_args()
{
    ObjectFactory<CanvasArgs> obj("canvas.args");
}
