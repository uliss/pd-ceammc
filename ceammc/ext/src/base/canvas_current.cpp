#include "canvas_current.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_property_callback.h"
#include "datatype_dict.h"

CanvasCurrent::CanvasCurrent(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    createCbSymbolProperty("@name", [this]() -> t_symbol* { return canvas_info_name(canvas()); });
    createCbSymbolProperty("@dir", [this]() -> t_symbol* { return canvas_info_dir(canvas()); });
    createCbBoolProperty("@root", [this]() -> bool { return canvas_info_is_root(canvas()); });
    createCbBoolProperty("@abstraction", [this]() -> bool { return canvas_info_is_abstraction(canvas()); });
    createCbListProperty("@args", [this]() -> AtomList { return canvas_info_args(canvas()); });
    createCbIntProperty("@font", [this]() -> int { return canvas_info_font(canvas()); });
    createCbListProperty("@paths", [this]() -> AtomList { return canvas_info_paths(canvas()); });
    createCbListProperty("@size", [this]() -> AtomList { return p_size(); });

    createCbIntProperty("@x", [this]() -> int { return canvas_info_rect(canvas()).x; });
    createCbIntProperty("@y", [this]() -> int { return canvas_info_rect(canvas()).y; });
    createCbIntProperty("@width", [this]() -> int { return canvas_info_rect(canvas()).w; });
    createCbIntProperty("@height", [this]() -> int { return canvas_info_rect(canvas()).h; });
}

void CanvasCurrent::onBang()
{
    DataTypeDict* dict = new DataTypeDict;

    for (auto& c : { "name", "dir", "root", "abstraction", "args", "font", "paths", "size", "x", "y", "width", "height" }) {
        dict->insert(c, property(c)->get());
    }

    atomTo(0, Atom(dict));
}

AtomList CanvasCurrent::p_size() const
{
    t_rect bb = canvas_info_rect(canvas());
    return AtomList(Atom(bb.w), Atom(bb.h));
}

void setup_canvas_current()
{
    ObjectFactory<CanvasCurrent> obj("canvas.current");

    obj.setDescription("verbose information about current canvas");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"canvas"});
    obj.setCategory("patch");
    obj.setSinceVersion(0, 4);
}
