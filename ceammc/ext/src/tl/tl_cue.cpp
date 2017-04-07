////
////  tl_cue.cpp
////  pd_ext
////
////  Created by Alex Nadzharov on 10/12/16.
////
////
//

//  CICM Wrapper version
#include "ceammc_gui.h"
#include "ceammc_timeline.h"

using namespace ceammc_gui;
using namespace ceammc::tl;

namespace ceammc_gui {

static const int LINE_WIDTH = 2;
static const int LINE_BOTTOM_MARGIN = 5;

struct tl_cue : public BaseGuiObject {
    t_canvas* canvas;

    t_etext* txt;
    t_efont* fnt;

    CueData* data;

    t_object* asObj() { return &b_box.b_obj.o_obj; }

    bool updatePos()
    {
        if (data->xPos() != b_box.b_rect.x) {
            data->setXPos(b_box.b_rect.x);
            CueStorage::sort(canvas);
            CueStorage::enumerate(canvas);
            return true;
        }
        return false;
    }
};

static inline tl_cue* asCue(t_ebox* x)
{
    return reinterpret_cast<tl_cue*>(x);
}

static inline int canvas_height(t_ebox* x)
{
    return x->b_obj.o_canvas->gl_screeny2 - x->b_obj.o_canvas->gl_screeny1 - LINE_BOTTOM_MARGIN;
}

static inline void vline_draw(t_ebox* x)
{
    int height = canvas_height(x);
    int xx = x->b_rect.x - 1;
    sys_vgui("%s create line %d %d %d %d -width %d -fill %s -tags .x%lxVLINE\n",
        x->b_canvas_id->s_name, xx, 0, xx, height, LINE_WIDTH,
        rgba_to_hex(asCue(x)->b_color_border), x);
}

static inline void vline_delete(t_ebox* x)
{
    sys_vgui("%s delete .x%lxVLINE\n", x->b_canvas_id->s_name, x);
}

static inline void vline_move(t_ebox* x)
{
    int height = canvas_height(x);
    int xx = x->b_rect.x - 1;
    sys_vgui("%s coords .x%lxVLINE %d %d %d %d\n", x->b_canvas_id->s_name, x, xx, 0, xx, height);
}

static void tl_cue_getdrawparams(tl_cue* x, t_object* view, t_edrawparams* params)
{
    params->d_borderthickness = 2;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

static void tl_cue_ebox_move(t_ebox* x)
{
    if (glist_isvisible(x->b_obj.o_canvas)) {
        int xx = (int)(x->b_rect.x - x->b_boxparameters.d_borderthickness);
        int yy = (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness);
        sys_vgui("%s coords %s %d %d\n",
            x->b_canvas_id->s_name,
            x->b_window_id->s_name,
            xx, yy);

        vline_move(x);
    }
}

static void redraw_canvas_cues(t_canvas* c, tl_cue* except = 0)
{
    CueList* lst = CueStorage::cueList(c);
    if (lst == 0)
        return;

    for (size_t i = 0; i < lst->size(); i++) {
        void* obj = lst->at(i)->object();
        if (obj != except) {
            GuiFactory<tl_cue>::ws_redraw(reinterpret_cast<tl_cue*>(obj));
        }
    }
}

void tl_cue_displace(t_gobj* z, t_glist* glist, int dx, int /*dy*/)
{
//todo set arg - see ui.display

#ifdef _WINDOWS
    t_ebox* x = (t_ebox*)z;
    if (x->b_selected_box) {
        x->b_rect.x += dx;
        x->b_rect.y = 2;
        x->b_obj.o_obj.te_xpix += dx;
        x->b_obj.o_obj.te_ypix = 0;
        tl_cue_ebox_move(x);
    }

#else
    t_ebox* x = (t_ebox*)z;

    x->b_rect.x += dx;
    x->b_rect.y = 2;
    x->b_obj.o_obj.te_xpix += dx;
    x->b_obj.o_obj.te_ypix = 2;
    tl_cue_ebox_move(x);
#endif

    tl_cue* zx = asCue(x);
    if (zx->updatePos()) {
        CueStorage::sort(zx->canvas);
        CueStorage::enumerate(zx->canvas);
        redraw_canvas_cues(zx->canvas);
    }
}

UI_fun(tl_cue)::new_ext(tl_cue* zx, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    zx->txt = etext_layout_create();
    zx->fnt = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE);
    zx->canvas = canvas_getcurrent();

    zx->data = new CueData(zx->canvas, zx->asObj());

    CueStorage::add(zx->data);
}

UI_fun(tl_cue)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT              (z, "size", 0, "45. 15.");
    CLASS_ATTR_INVISIBLE            (z, "size", 0);
    CLASS_ATTR_INVISIBLE            (z, "send", 0);
    CLASS_ATTR_INVISIBLE            (z, "receive", 0);

    // change default border color
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, PROP_BORDER_COLOR, 0, DEFAULT_ACTIVE_COLOR);
    // clang-format on

    z->c_widget.w_displacefn = tl_cue_displace;
    eclass_addmethod(z, reinterpret_cast<method>(tl_cue_getdrawparams), "getdrawparams", A_NULL, 0);
}

UI_fun(tl_cue)::free_ext(tl_cue* zx)
{
    vline_delete(asBox(zx));
    CueStorage::remove(zx->data);
    delete zx->data;
    etext_layout_destroy(zx->txt);
    efont_destroy(zx->fnt);
}

UI_fun(tl_cue)::wx_oksize(tl_cue* z, t_rect* newrect)
{
    const float zoom = ebox_getzoom(asBox(z));
    newrect->width = 50 * zoom;
    newrect->height = ebox_getzoomfontsize(asBox(z)) + 3 * zoom;
}

UI_fun(tl_cue)::wx_attr_changed_ext(tl_cue* z, t_symbol* attr)
{
    ws_redraw(z);
}

UI_fun(tl_cue)::wx_paint(tl_cue* zx, t_object* /*view*/)
{
    t_rect rect;
    zx->getRect(&rect);

    t_elayer* g = ebox_start_layer(asBox(zx), BG_LAYER, rect.width, rect.height);
    if (g) {
        const float xoff = 2 * ebox_getzoom(asBox(zx));
        const float yoff = (FONT_SIZE + 2) * ebox_getzoom(asBox(zx));
        bool pos_changed = zx->updatePos();

        etext_layout_set(zx->txt, zx->data->name().c_str(), zx->fnt, xoff, yoff, rect.width, rect.height / 2,
            ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);

        etext_layout_draw(zx->txt, g);
        ebox_end_layer(asBox(zx), BG_LAYER);

        if (pos_changed)
            redraw_canvas_cues(zx->canvas, zx);

        vline_delete(asBox(zx));
        vline_draw(asBox(zx));
    }

    ebox_paint_layer(asBox(zx), BG_LAYER, 0., 0.);
}
}

extern "C" void setup_tl0x2ecue()
{
    GuiFactory<tl_cue> class1;
    class1.setup_noin("tl.cue");
}
