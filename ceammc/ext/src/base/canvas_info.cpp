#include "canvas_info.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"

CanvasCurrent::CanvasCurrent(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    createCbProperty("@name", &CanvasCurrent::p_name);
    createCbProperty("@dir", &CanvasCurrent::p_dir);
    createCbProperty("@root", &CanvasCurrent::p_root);
    createCbProperty("@abstraction", &CanvasCurrent::p_abstraction);
    createCbProperty("@args", &CanvasCurrent::p_args);
    createCbProperty("@font", &CanvasCurrent::p_font);
    createCbProperty("@paths", &CanvasCurrent::p_paths);
    createCbProperty("@size", &CanvasCurrent::p_size);
    createCbProperty("@x", &CanvasCurrent::p_x);
    createCbProperty("@y", &CanvasCurrent::p_y);
    createCbProperty("@width", &CanvasCurrent::p_width);
    createCbProperty("@height", &CanvasCurrent::p_height);
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
