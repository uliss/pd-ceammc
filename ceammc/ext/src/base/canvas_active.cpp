#include "canvas_active.h"
#include "ceammc_factory.h"
#include "parser_canvas_id.h"

extern "C" {
#include "g_canvas.h"
}

static inline std::uintptr_t ptr_to_uint(void* x)
{
    return reinterpret_cast<std::uintptr_t>(x);
}

CanvasActive::CanvasActive(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    bindReceive(gensym("#canvas_active"));
}

void CanvasActive::onBang()
{
    sys_vgui("pdsend \"#canvas_active .active [focus]\"\n");
}

void CanvasActive::m_active(t_symbol* s, const AtomListView& lv)
{
    if (!lv.isSymbol())
        return;

    CanvasId given_id = 0;
    if (!try_parse_canvas_id(lv.asSymbol()->s_name, given_id))
        return boolTo(0, false);

    auto my_cnv_id = ptr_to_uint(rootCanvas());

    boolTo(0, given_id == my_cnv_id);
}

void setup_base_canvas_active()
{
    ObjectFactory<CanvasActive> obj("canvas.active");
    obj.setXletsInfo({ "bang" }, { "int: 1 or 0" });
    obj.addMethod(".active", &CanvasActive::m_active);
}
