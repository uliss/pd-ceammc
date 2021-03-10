#include "canvas_top.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "ceammc_property_callback.h"
#include "datatype_dict.h"

CanvasTop::CanvasTop(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    createCbSymbolProperty("@name", [this]() -> t_symbol* { return canvas_info_name(rootCanvas()); });
    createCbSymbolProperty("@dir", [this]() -> t_symbol* { return canvas_info_dir(rootCanvas()); });
    createCbIntProperty("@font", [this]() -> int { return canvas_info_font(rootCanvas()); });
    createCbListProperty("@paths", [this]() -> AtomList { return canvas_info_paths(rootCanvas()); });
    createCbIntProperty("@x", [this]() -> int { return canvas_info_rect(rootCanvas()).x; });
    createCbIntProperty("@y", [this]() -> int { return canvas_info_rect(rootCanvas()).y; });
    createCbIntProperty("@width", [this]() -> int { return canvas_info_rect(rootCanvas()).w; });
    createCbIntProperty("@height", [this]() -> int { return canvas_info_rect(rootCanvas()).h; });

    createCbListProperty("@size", [this]() -> AtomList {
        t_rect bb = canvas_info_rect(rootCanvas());
        return { Atom(bb.w), Atom(bb.h) };
    });
}

void CanvasTop::onBang()
{
    DictAtom dict;

    dict->insert("name", property("@name")->get());
    dict->insert("dir", property("@dir")->get());
    dict->insert("font", property("@font")->get());
    dict->insert("paths", property("@paths")->get());
    dict->insert("size", property("@size")->get());
    dict->insert("x", property("@x")->get());
    dict->insert("y", property("@y")->get());
    dict->insert("width", property("@width")->get());
    dict->insert("height", property("@height")->get());

    atomTo(0, dict);
}

void CanvasTop::m_postscript(t_symbol* s, const AtomListView& lv)
{
    std::string filename;

    t_symbol* sname = lv.symbolAt(0, &s_);
    if (sname == &s_) {
        filename = platform::strip_extension(canvas_info_name(rootCanvas())->s_name);
        filename += ".ps";
    } else {
        filename = sname->s_name;
    }

    if (filename.empty()) {
        METHOD_ERR(s) << "empty filename";
        return;
    }

    OBJ_DBG << "saving to postscript file: " << filename;

    sys_vgui(".x%lx.c postscript -file {%s}\n", rootCanvas(), filename.c_str());
}

void setup_canvas_top()
{
    ObjectFactory<CanvasTop> obj("canvas.top");
    obj.addMethod("postscript", &CanvasTop::m_postscript);

    obj.setDescription("verbose information about top-level canvas");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "canvas" });
    obj.setCategory("patch");
    obj.setSinceVersion(0, 4);
}
