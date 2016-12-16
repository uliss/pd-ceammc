//
//  ceammc_draw_wrappers.h
//  pd_ext
//
//  Created by Alex on 15/12/16.
//
//

#ifndef ceammc_draw_wrappers_h
#define ceammc_draw_wrappers_h

#include <g_canvas.h>

t_canvas *_gui_canvas;
t_object *_gui_obj;


void gui_set_canvas(t_glist *glist)
{
    //    printf("--set c-\n");
    _gui_canvas = glist;
}

void gui_set_object(t_object *obj)
{
    //    printf("--set obj-\n");
    _gui_obj = obj;
}

void gui_rect (std::string obj, int x, int y, int w, int h, std::string stroke_color, std::string fill_color, float line_width)
{
    //    printf ("rect\n");
    char s1[] = ".x%lx.c create rectangle %d %d %d %d -width %d -outline %s -fill %s -tags %lx%s\n";
    sys_vgui(s1,
             _gui_canvas, x, y,x+w, y+h,
             int(line_width),
             stroke_color.c_str(),
             fill_color.c_str(), _gui_obj, obj.c_str());
}

void gui_text (std::string obj, int x, int y, std::string text, std::string text_color)
{
    //    printf ("text\n");
    
    int font_size = 12;//*(this->ui_property_get("font_size")).get();
    char s1[] = ".x%lx.c create text %d %d -text {%s} -anchor w -font {{%s} -%d %s} -fill %s -tags [list %lx%s label text]\n";
    sys_vgui(s1,
             _gui_canvas, int(x),
             int(y),
             text.c_str(),"Monaco",
             font_size, "normal",
             text_color.c_str(), _gui_obj, obj.c_str());
    
    //xx+x->x_gui.x_ldx * zoomlabel
    //yy+x->x_gui.x_ldy * zoomlabel
}

void gui_move (std::string obj, int x, int y, int w, int h)
{
    char s1[] = ".x%lx.c coords %lx%s %d %d %d %d\n";
    sys_vgui(s1,
             _gui_canvas, _gui_obj, obj.c_str(), x, y, x+w, y+h);
}

void gui_set_width (std::string obj, int w)
{
    char s1[] = ".x%lx.c itemconfigure %lx%s -width %d\n";
    sys_vgui(s1, _gui_canvas, _gui_obj ,obj.c_str(), w);
}

void gui_delete(std::string obj)
{
    char s1[] = ".x%lx.c delete %lx%s\n";
    sys_vgui(s1, _gui_canvas, _gui_obj, obj.c_str());
}

#endif /* ceammc_draw_wrappers_h */
