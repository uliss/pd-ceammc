#include "canvas_current.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"

CanvasCurrent::CanvasCurrent(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    {
        Property* p = createCbProperty("@name", &CanvasCurrent::p_name);
        p->info().setType(PropertyInfoType::SYMBOL);
    }

    {
        Property* p = createCbProperty("@dir", &CanvasCurrent::p_dir);
        p->info().setType(PropertyInfoType::SYMBOL);
    }

    {
        Property* p = createCbProperty("@root", &CanvasCurrent::p_root);
        p->info().setType(PropertyInfoType::BOOLEAN);
    }

    {
        Property* p = createCbProperty("@abstraction", &CanvasCurrent::p_abstraction);
        p->info().setType(PropertyInfoType::BOOLEAN);
    }

    createCbProperty("@args", &CanvasCurrent::p_args);

    {
        Property* p = createCbProperty("@font", &CanvasCurrent::p_font);
        p->info().setType(PropertyInfoType::INTEGER);
    }

    createCbProperty("@paths", &CanvasCurrent::p_paths);
    createCbProperty("@size", &CanvasCurrent::p_size);

    {
        Property* p = createCbProperty("@x", &CanvasCurrent::p_x);
        p->info().setType(PropertyInfoType::INTEGER);
    }

    {
        Property* p = createCbProperty("@y", &CanvasCurrent::p_y);
        p->info().setType(PropertyInfoType::INTEGER);
    }

    {
        Property* p = createCbProperty("@width", &CanvasCurrent::p_width);
        p->info().setType(PropertyInfoType::INTEGER);
    }

    {
        Property* p = createCbProperty("@height", &CanvasCurrent::p_height);
        p->info().setType(PropertyInfoType::INTEGER);
    }
}

void CanvasCurrent::onBang()
{
    DataTypeDict* dict = new DataTypeDict;

    dict->insert("name", p_name());
    dict->insert("dir", p_dir());
    dict->insert("root", p_root());
    dict->insert("abstraction", p_abstraction());
    dict->insert("args", p_args());
    dict->insert("font", p_font());
    dict->insert("paths", p_paths());
    dict->insert("size", p_size());
    dict->insert("x", p_x());
    dict->insert("y", p_y());
    dict->insert("width", p_width());
    dict->insert("height", p_height());

    dataTo(0, DataPtr(dict));
}

AtomList CanvasCurrent::p_name() const
{
    return Atom(canvas_info_name(canvas()));
}

AtomList CanvasCurrent::p_dir() const
{
    return Atom(canvas_info_dir(canvas()));
}

AtomList CanvasCurrent::p_root() const
{
    return Atom(canvas_info_is_root(canvas()) ? 1 : 0);
}

AtomList CanvasCurrent::p_abstraction() const
{
    return Atom(canvas_info_is_abstraction(canvas()) ? 1 : 0);
}

AtomList CanvasCurrent::p_args() const
{
    return canvas_info_args(canvas());
}

AtomList CanvasCurrent::p_font() const
{
    return Atom(canvas_info_font(canvas()));
}

AtomList CanvasCurrent::p_paths() const
{
    return canvas_info_paths(canvas());
}

AtomList CanvasCurrent::p_size() const
{
    t_rect bb = canvas_info_rect(canvas());
    return AtomList(Atom(bb.w), Atom(bb.h));
}

AtomList CanvasCurrent::p_width() const
{
    return Atom(canvas_info_rect(canvas()).w);
}

AtomList CanvasCurrent::p_height() const
{
    return Atom(canvas_info_rect(canvas()).h);
}

AtomList CanvasCurrent::p_x() const
{
    return Atom(canvas_info_rect(canvas()).x);
}

AtomList CanvasCurrent::p_y() const
{
    return Atom(canvas_info_rect(canvas()).y);
}

void setup_canvas_current()
{
    ObjectFactory<CanvasCurrent> obj("canvas.current");
}
