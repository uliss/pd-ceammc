#include "canvas_top.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "datatype_dict.h"

CanvasTop::CanvasTop(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    abs_ = new BoolProperty("@abs", true);
    addProperty(abs_);

    createCbSymbolProperty("@name", [this]() -> t_symbol* { return canvas_info_name(topCanvas()); });
    createCbSymbolProperty("@dir", [this]() -> t_symbol* { return canvas_info_dir(topCanvas()); });
    createCbIntProperty("@font", [this]() -> int { return canvas_info_font(topCanvas()); });
    createCbListProperty("@paths", [this]() -> AtomList { return canvas_info_paths(topCanvas()); });
    createCbIntProperty("@x", [this]() -> int { return canvas_info_rect(topCanvas()).x; });
    createCbIntProperty("@y", [this]() -> int { return canvas_info_rect(topCanvas()).y; });
    createCbIntProperty("@width", [this]() -> int { return canvas_info_rect(topCanvas()).w; });
    createCbIntProperty("@height", [this]() -> int { return canvas_info_rect(topCanvas()).h; });

    createCbListProperty("@size", [this]() -> AtomList {
        t_rect bb = canvas_info_rect(topCanvas());
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
        filename = platform::strip_extension(canvas_info_name(topCanvas())->s_name);
        filename += ".ps";
    } else {
        filename = sname->s_name;
    }

    if (filename.empty()) {
        METHOD_ERR(s) << "empty filename";
        return;
    }

    OBJ_DBG << "saving to postscript file: " << filename;

    sys_vgui(".x%lx.c postscript -file {%s}\n", topCanvas(), filename.c_str());
}

const t_canvas* CanvasTop::topCanvas() const
{
    return canvas(abs_->value() ? CanvasType::ROOT : CanvasType::TOPLEVEL);
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
