//
//  ceammc_gui.cpp
//  pd_ext
//
//  Created by Alex on 12/12/16.
//
//

#include "ceammc_gui.hpp"

#pragma mark init

t_object * ceammc_gui::ceammc_gui_pd_object = NULL;
t_class* ceammc_gui::ceammc_gui_pd_class;

t_widgetbehavior w_proxy::w_widget;
t_object* w_proxy::ceammc_gui_pd_object = NULL;
t_object* w_proxy::ceammc_gui_pd_instance = NULL;

t_canvas *draw_wrappers::w_canvas;
t_object *draw_wrappers::w_obj;

gui_properties *ceammc_gui::default_properties;

t_class* ceammc_gui::get_pd_class(){return ceammc_gui::ceammc_gui_pd_class;}
void ceammc_gui::set_pd_class(t_class* c1){ceammc_gui::ceammc_gui_pd_class=c1;}


#pragma mark properties

t_atom ceammc_gui_object::ui_property_get(std::string name)
{
    t_atom ret = (*this->ui_properties)[name];
    
    return ret;
}

t_float ceammc_gui_object::ui_property_get_float(std::string name)
{
    //printf("property get float\n");
    
    if ((*this->ui_properties)[name].a_type == A_FLOAT)
    {
        t_float ret = (*this->ui_properties)[name].a_w.w_float;
        
        return ret;
    }
    else
    {
        ceammc_gui::e_error("bad float value");
        return 0;
    }
}

char* ceammc_gui_object::ui_property_get_c_str(std::string name)
{
    //printf("property get cstr %s\n", name.c_str());
    
    if ((*this->ui_properties)[name].a_type == A_SYMBOL)
    {
        char* ret = (*this->ui_properties)[name].a_w.w_symbol->s_name;
        
        return ret;
    }
    else
    {
        ceammc_gui::e_error("bad symbol value");
        printf ("property get cstr err\n");
        return "";
    }
}

void ceammc_gui_object::ui_property_set(std::string name, t_atom value)
{
    (*this->ui_properties)[name] = value;
}

void ceammc_gui_object::ui_property_set(std::string name, std::string value)
{
    t_atom a;
    a.a_w.w_symbol = gensym(value.c_str());
    a.a_type = A_SYMBOL;
    (*this->ui_properties)[name] = a;
}

void ceammc_gui_object::ui_property_set(std::string name, float value)
{
    t_atom a;
    a.a_w.w_float = value;
    a.a_type = A_FLOAT;
    (*this->ui_properties)[name] = a;
}

bool ceammc_gui_object::ui_property_load(t_atom *values)
{
    printf("** property load [%lu]\n", (long)this);
    int count = (int)(*this->ui_properties).size();
    int i=0;
    
    printf("count: %i\n", count);
    ceammc_gui::e_atoms(values, count);
    
    std::map<std::string, t_atom>::iterator it;
    
    for (it = (*this->ui_properties).begin(); it != (*this->ui_properties).end(); ++it)
    {
        if (i==count) break;
        
        t_atom a = values[i];
        if (a.a_type != it->second.a_type)
        {
            ceammc_gui::e_error("broken object data");
            
            return false;
        }
        else
        {
            it->second = a;
        }
        
        i++;
        
    }
    
    printf( "** loaded %i\n",i);
    
    printf("********** load\n");
    ceammc_gui::e_properties((t_object*)this);
    
    return true;
}

void ceammc_gui_object::ui_property_init()
{
    printf("**property init [%lu]\n", (long)this);
    
    if (this->ui_properties)
    {
        printf("p del\n");
        delete this->ui_properties;
    }
    this->ui_properties = new std::map<std::string,t_atom>;
    
    this->ui_property_set("object_name", "ceammc.gui_object");
    
    this->ui_property_set("x", 0);
    this->ui_property_set("y", 0);
    this->ui_property_set("width", 45);
    this->ui_property_set("height", 15);
    
    this->ui_property_set("font", "Helvetica");
    this->ui_property_set("font_size", 14.);
    
    this->ui_property_set("color_bg", "#FFFFFF");
    this->ui_property_set("color_text", "#000000");
    this->ui_property_set("color_line", "#00C0FF");
    
    this->ui_property_set("send_id", "no_send");
    this->ui_property_set("receive_id", "no_receive");
    
    this->ui_property_set("label", "CEAMMC_gui_template");
    
    this->ui_property_set("label_x", 7);
    this->ui_property_set("label_y", 7);
    
    this->ui_property_set("_selected", 0);
    
    printf( "** done\n");
    
    printf("********** init\n");
    ceammc_gui::e_properties((t_object*)this);
    
}
void ceammc_gui_object::ui_property_copy()
{
    if (this->ui_properties)
    {
        printf("p del\n");
        delete this->ui_properties;
    }
    this->ui_properties = new std::map<std::string,t_atom>;
    
    *this->ui_properties = *ceammc_gui::default_properties;
    
    printf("********** copy\n");
    ceammc_gui::e_properties((t_object*)this);
    
}


#pragma mark -
#pragma mark draw wrappers

//TODO names

void draw_wrappers::dw_set_canvas(t_glist *glist)
{
    //    printf("--set c-\n");
    draw_wrappers::w_canvas = glist;
}

void draw_wrappers::dw_set_object(t_object *obj)
{
    //    printf("--set obj-\n");
    draw_wrappers::w_obj = obj;
}

void draw_wrappers::dw_rect (std::string obj, int x, int y, int w, int h, std::string stroke_color, std::string fill_color, float line_width)
{
    //    printf ("rect\n");
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -width %d -outline %s -fill %s -tags %lx%s\n",
             draw_wrappers::w_canvas, x, y,x+w, y+h,
             int(line_width),
             stroke_color.c_str(),
             fill_color.c_str(), draw_wrappers::w_obj, obj.c_str());
}

void draw_wrappers::dw_text (std::string obj, int x, int y, std::string text, std::string text_color)
{
    //    printf ("text\n");
    
    int font_size = 12;//*(this->ui_property_get("font_size")).get();
    sys_vgui(".x%lx.c create text %d %d -text {%s} -anchor w -font {{%s} -%d %s} -fill %s -tags [list %lx%s label text]\n",
             draw_wrappers::w_canvas, int(x),
             int(y),
             text.c_str(),"Helvetica",
             font_size, sys_fontweight,
             text_color.c_str(), draw_wrappers::w_obj, obj.c_str());
    
    //xx+x->x_gui.x_ldx * zoomlabel
    //yy+x->x_gui.x_ldy * zoomlabel
}

void draw_wrappers::dw_move (std::string obj, int x, int y, int w, int h)
{
    sys_vgui(".x%lx.c coords %lx%s %d %d %d %d\n",
             draw_wrappers::w_canvas, draw_wrappers::w_obj, obj.c_str(), x, y, x+w, y+h);
}

void draw_wrappers::dw_set_width (std::string obj, int w)
{
    sys_vgui(".x%lx.c itemconfigure %lx%s -width %d\n", draw_wrappers::w_canvas, draw_wrappers::w_obj ,obj.c_str(), w);
}

void draw_wrappers::dw_delete(std::string obj)
{
    sys_vgui(".x%lx.c delete %lx%s\n", draw_wrappers::w_canvas, draw_wrappers::w_obj, obj.c_str());
}

#pragma mark -
#pragma mark widget


t_widgetbehavior *w_proxy::w_create()
{
    t_widgetbehavior *ret = new t_widgetbehavior;
    
    ret->w_activatefn = NULL;   //TEMP
    ret->w_clickfn = w_proxy::pw_click;
    ret->w_deletefn = w_proxy::pw_delete;
    ret->w_displacefn = w_proxy::pw_displace;
    ret->w_getrectfn = w_proxy::pw_getrect;
    ret->w_selectfn = w_proxy::pw_select;
    ret->w_visfn = w_proxy::pw_vis;
    
    return ret;
}

//

#pragma mark proto ceammc_widget_class

void v_widget::w_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2)
{
    printf("w displace prototype\n");
}


int v_widget::w_click(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    return 1;
    
}

void v_widget::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    printf("w displace prototype\n");
}

void v_widget::w_select(t_gobj *z, t_glist *glist, int selected)
{
    printf("w sel prototype\n");
}

void v_widget::w_delete(t_gobj *z, t_glist *glist)
{
    printf("w del prototype \n");
}

void v_widget::w_draw(t_gobj *z, t_glist *glist)
{
    printf("w draw prototype\n");
}

void v_widget::w_erase(t_gobj *z, t_glist *glist)
{
    printf("w erase prototype\n");
}

#pragma mark ceammc_widget_class

void ceammc_gui_object::w_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2)
{
    ceammc_gui_object *x = (ceammc_gui_object *)z;
    
    *x1 = x->ui_property_get_float("x");
    *y1 = x->ui_property_get_float("y");
    *x2 = *x1 + x->ui_property_get_float("width");
    *y2 = *y1 + x->ui_property_get_float("height");
    
}


int ceammc_gui_object::w_click(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    return 1;
    
}

void ceammc_gui_object::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    
    ceammc_gui_object *x = (ceammc_gui_object *)z;
    
    x->ui_property_set("x", x->ui_property_get_float("x") + dx);
    x->ui_property_set("y", x->ui_property_get_float("y") + dy);
    
}

void ceammc_gui_object::w_select(t_gobj *z, t_glist *glist, int selected)
{
    printf("sel prototype\n");
}

void ceammc_gui_object::w_delete(t_gobj *z, t_glist *glist)
{
    printf("del prototype \n");
}

void ceammc_gui_object::w_draw(t_gobj *z, t_glist *glist)
{
    printf("draw prototype\n");
}

void ceammc_gui_object::w_erase(t_gobj *z, t_glist *glist)
{
    printf("erase prototype\n");
}

#pragma mark -

#pragma mark widget proxy

void w_proxy::pw_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2)
{
    printf("widget proxy 1 %lu\n", (long)z);
//    if (z)
//        ((ceammc_gui_object*)z) -> w_getrect(z,glist,x1,y1,x2,y2);
}
int w_proxy::pw_click(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    printf("widget proxy 2 %lu\n", (long)z);
//    if (z)
//    return ((v_widget*)z) -> w_click(z, glist, xpix, ypix, shift, alt, dbl, doit);
//    else
        return 0;
}
void w_proxy::pw_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    printf("widget proxy 3 %lu\n", (long)z);
//    if (z)
//    ((v_widget*)z) -> w_displace(z, glist, dx, dy);
}
void w_proxy::pw_select(t_gobj *z, t_glist *glist, int selected)
{
    printf("widget proxy 4 %lu\n", (long)z);
//    if (z)
//    ((v_widget*)z) -> w_select(z, glist, selected);
}
void w_proxy::pw_delete(t_gobj *z, t_glist *glist)
{
    printf("widget proxy 5 %lu\n", (long)z);
//    if (z)
//    ((v_widget*)z) -> w_delete(z, glist);
}
void w_proxy::pw_vis(t_gobj *z, t_glist *glist, int vis)
{
    printf("widget proxy 6 %lu\n", (long)z);
//    if (z)
//        if (vis)
//            ((v_widget*)z) -> w_draw(z, glist);
//        else
//            ((v_widget*)z) -> w_erase(z, glist);
    
}

void w_proxy::pd_instance_init(t_object *obj)
{
    printf(">>> w proxy instance init base\n");
    //((v_widget*)w_proxy::ceammc_gui_pd_object) -> pd_instance_init(obj);
}

#pragma mark log

void ceammc_gui::e_error(std::string err)
{
    post("ceammc_gui_error: %s", err.c_str());
}

void ceammc_gui::e_properties(t_object *obj)
{
    gui_properties *prop = ((ceammc_gui_object *) obj)->ui_properties;
    
    gui_properties::iterator it;
    
    for (it = prop->begin(); it != prop->end(); ++it)
    {
        (it->second.a_type == A_SYMBOL)?
        printf("[ %s ] : %s \n", it->first.c_str(), it->second.a_w.w_symbol->s_name )
        :
        printf("[ %s ] : %f \n", it->first.c_str(), it->second.a_w.w_float )
        ;
    }
    
    printf("\n");
}

void ceammc_gui::e_atoms(t_atom *obj, int count)
{
    //gui_properties *prop = ((ceammc_gui_object *) obj)->ui_properties;
    
   // gui_properties::iterator it;
    
    for (int i = 0; i<count; i++)
    {
        (obj[i].a_type == A_SYMBOL)?
        printf("[ %d ] : %s \n", i, obj[i].a_w.w_symbol->s_name )
        :
        printf("[ %d ] : %f \n", i, obj[i].a_w.w_float )
        ;
    }
    
    printf("\n");
}

#pragma mark -

void ceammc_gui_object::pd_instance_init(t_object *obj)
{
    printf(">>> instance init base\n");
}

//void v_widget::pd_instance_init(t_object *obj)
//{
//    printf(">>> w instance init base\n");
//}



#pragma mark -

void *ceammc_gui_object::pd_init_v(t_symbol *s, int argc, t_atom *argv)
{
    ceammc_gui_object* x = reinterpret_cast<ceammc_gui_object*>(pd_new(x->proto_class));
    
    
    
    return static_cast<void*>(x);
}
void ceammc_gui_object::pd_init_v(ceammc_gui_object *x)
{

}

#pragma mark +

t_newmethod ceammc_gui_object::get_pd_class_new()
{
    return (t_newmethod)(ceammc_gui::pd_class_new1);
}

void *ceammc_gui::pd_class_new1(t_symbol *s, int argc, t_atom *argv)
{
    printf("\n*base ceammc_gui::pd_class_new1\n");
    return ceammc_gui::pd_class_new2((t_class*)ceammc_gui::ceammc_gui_pd_class,s,argc,argv);
}

void *ceammc_gui::pd_class_new2(t_class *c1, t_symbol *s, int argc, t_atom *argv)
{
    printf("gui class new\n");
    
    //t_class *c1;// = //((ceammc_gui*)NULL)->get_pd_class();
    
    printf("class ptr: %lu\n",(long)c1);
    
    ceammc_gui_object* x = reinterpret_cast<ceammc_gui_object*>(pd_new(c1));
    
    
    x->ui_property_copy();
    
    //w_proxy::pd_instance_init((t_object*)x);
    
    //remove name
    if ((argc)==(*x->ui_properties).size())
    {
        x->ui_property_load(argv);
        //x->ui_property_init();
    }
    else
    {
        printf("no args to load: argc %i prop: %lu\n", argc, (*x->ui_properties).size());
        printf("args count: %i\n", argc );
        //ceammc_gui::e_atoms(argv, argc);
    }
    
    
    printf("pd instance ptr: %lu\n", (long)x);
    printf("pd class ptr: %lu\n", (long)c1);
    
    return static_cast<void*>(x);
}

void ceammc_gui::pd_class_free(ceammc_gui_object *x)
{
    
    //TODO
    //w_proxy::pd_instance_free((t_object*)x);
}

void ceammc_gui::pd_class_save(t_gobj *z, t_binbuf *b)
{
    ceammc_gui_object *x = (ceammc_gui_object *)z;
    
    binbuf_addv(b, "s", gensym("#X obj"));
    
    binbuf_addv(b, "i", int(((*x->ui_properties)["x"]).a_w.w_float));
    binbuf_addv(b, "i", int(((*x->ui_properties)["y"]).a_w.w_float));
    
    //TODO wrap in quotes
    binbuf_addv(b, "s", ((*x->ui_properties)["object_name"]).a_w.w_symbol);
    
    std::map<std::string,t_atom>::iterator it;
    for (it = (*x->ui_properties).begin(); it!= (*x->ui_properties).end(); ++it)
    {
        binbuf_add(b, 1, &it->second);
    }
    
    binbuf_addv(b, ";");
    
    printf("********** save\n");
    ceammc_gui::e_properties((t_object*)x);
    
    
    
}

#pragma mark -

t_class* ceammc_gui::pd_setup(t_object *gui_class, std::string class_name, t_class *pd_class)
{
    //ceammc_gui_get_pd_class_p =
    printf("lib pd_setup %lu | %lu\n", gui_class, ceammc_gui::ceammc_gui_pd_object);
    
    //((ceammc_gui*)NULL)->set_pd_class(pd_class);
    
    std::string name = class_name;//((ceammc_gui_object*)gui_class)->ui_property_get_c_str("object_name");
    printf("* name %s",name.c_str());
    if (name.length()==0) {ceammc_gui::e_error("bad object name");return NULL;}
    
    
    //remove
    ceammc_gui::default_properties = ((ceammc_gui_object*)gui_class)->ui_properties;
    
    pd_class = class_new(gensym(name.c_str()),
                                                reinterpret_cast<t_newmethod>(
                                                                              ((ceammc_gui_object*)gui_class)->get_pd_class_new()
                                                                              ),
                                                reinterpret_cast<t_method>(pd_class_free),
                                                ((ceammc_gui_object*)gui_class)->size(), CLASS_PATCHABLE, A_GIMME,0);   //sizeof(gui_class)*16
    //((ceammc_gui_object*)gui_class)->size()
    
    printf("new ptr %lu\n", pd_class);
    
    w_proxy pr1;
    w_proxy::w_widget = *pr1.w_create();
    //pr1.ceammc_gui_pd_object = gui_class;
    
    //((ceammc_gui*)NULL)->set_pd_class(pd_class);
    
    class_setwidget(pd_class, &w_proxy::w_widget);      //ceammc_gui::ceammc_gui_
    class_setsavefn(pd_class, pd_class_save);           //ceammc_gui::ceammc_gui_
    
    return pd_class;
}



////////////////////
// prototype

//extern "C" {void ___setup_ceammc_gui_template()
//{
//    ceammc_gui *gui = new ceammc_gui();
//
//    ceammc_gui_object *obj1 = new ceammc_gui_object();
//
//    gui->pd_setup((t_object*)obj1);
//
//    
//}
//}




