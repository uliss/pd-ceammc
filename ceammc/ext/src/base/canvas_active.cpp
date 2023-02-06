#include "canvas_active.h"
#include "ceammc_factory.h"
#include "parser_canvas_id.h"

extern "C" {
#include "g_canvas.h"
}

static t_canvas* canvas_get_root(t_canvas* x, bool abstractions = true)
{
    if (!x)
        return x;
    else if ((!x->gl_owner) || (abstractions && canvas_isabstraction(x)))
        return x;
    else
        return canvas_get_root(x->gl_owner, abstractions);
}

CanvasActive::CanvasActive(const PdArgs& args)
    : BaseObject(args)
    , abstractions_(nullptr)
{
    createOutlet();
    bindReceive(gensym("#canvas_active"));

    abstractions_ = new BoolProperty("@abs", false);
    addProperty(abstractions_);
}

void CanvasActive::onBang()
{
    sys_vgui("pdsend \"#canvas_active .active [focus]\"\n");
}

void CanvasActive::m_active(t_symbol* s, const AtomListView& lv)
{
    // METHOD_DBG(s) << lv;

    if (!lv.isSymbol())
        return;

    CanvasId given_id = 0;
    if (!try_parse_canvas_id(lv.asSymbol()->s_name, given_id))
        return boolTo(0, false);

    auto my_cnv_id = ptr_to_uint(currentCanvas());
    // METHOD_DBG(s) << std::hex << my_cnv_id;

    boolTo(0, given_id == my_cnv_id);
}

t_canvas* CanvasActive::currentCanvas()
{
    return canvas_get_root(canvas(), abstractions_->value());
}

void setup_base_canvas_active()
{
    ObjectFactory<CanvasActive> obj("canvas.active");
    obj.setXletsInfo({ "bang" }, { "int: 1 or 0" });
    obj.addMethod(".active", &CanvasActive::m_active);
}
