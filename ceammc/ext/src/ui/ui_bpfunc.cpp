//
//  ui_bpfunc.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 20/12/16.
//
//

#include <stdio.h>

#include "ceammc_gui.h"

#include "ceammc_atomlist.h"

#include <algorithm>
#include <map>

#include <vector>

#include "ceammc_bpf.h"

#include "ceammc_globaldata.h"

//using namespace ceammc;

using namespace std;
using namespace ceammc;
using namespace ceammc_gui;

// for local object name
#include <sstream>

template <class T>
inline std::string to_string(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

namespace ceammc_gui {

typedef std::vector<t_bpt> bpf_points;

struct ui_bpfunc : public BaseGuiObject {
    t_ebox x_gui;

    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;

    t_outlet* out1;
    t_atom* out_list;
    int out_list_count;

    BPF* x_bpf; //later add globaldata / pointer
    GlobalData<BPF>* g_bpf;

    float _px;
    float _py;

    int addidx;
    int delidx;

    float addpos;
    float addpos_y;
    bool del_mod;

    int select_idx;

    bool auto_send;

    t_etext* txt_min;
    t_etext* txt_max;
    t_etext* txt_val; //stub

    t_efont* txt_font;
};

#pragma mark -

UI_fun(ui_bpfunc)::wx_paint(ui_bpfunc* z, t_object* view)
{
    t_symbol* bgl = gensym("background_layer");
    //float size;
    t_rect rect;
    z->getRect(&rect);

    t_elayer* g = ebox_start_layer((t_ebox*)z, bgl, rect.width, rect.height);

    ui_bpfunc* zx = (ui_bpfunc*)z;

    if (g) {
        //egraphics_set_color_hex(g, gensym("#E0E0E0"));
        egraphics_set_color_rgba(g, &zx->b_color_background);

        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
        egraphics_fill(g);

        egraphics_set_color_hex(g, gensym("#C0C0C0"));

        egraphics_line(g, 0, .25 * rect.height, rect.width, .25 * rect.height);
        egraphics_line(g, 0, .5 * rect.height, rect.width, .5 * rect.height);
        egraphics_line(g, 0, .75 * rect.height, rect.width, .75 * rect.height);

        egraphics_stroke(g);

        egraphics_set_color_hex(g, gensym("#505050"));

        char c_val[16];
        sprintf(c_val, "—");

        if (zx->addpos > 0) {
            egraphics_set_color_hex(g, gensym("#00C0FF"));

            egraphics_line(g, zx->addpos, 0, zx->addpos, rect.height);
            egraphics_line(g, 0, zx->addpos_y, rect.width, zx->addpos_y);

            sprintf(c_val, "%.2f : %.2f", zx->addpos / rect.width * zx->x_bpf->range_x + zx->x_bpf->shift_x, (1 - zx->addpos_y / rect.height) * zx->x_bpf->range_y + zx->x_bpf->shift_y);

            egraphics_stroke(g);
        }

        if (zx->select_idx == 0) {
            egraphics_set_color_hex(g, gensym("#00C0FF"));

            egraphics_rectangle(g, zx->x_bpf->getPointRawX(0) * rect.width - 8, (1 - zx->x_bpf->getPointRawY(0)) * rect.height - 8, 15, 15);
            egraphics_stroke(g);
        }

        int i = 0;
        {

            int pw1 = 6 - 4 * zx->x_bpf->getPointLockX(i);
            int ph1 = 6 - 4 * zx->x_bpf->getPointLockY(i);

            egraphics_rectangle(g, zx->x_bpf->getPointRawX(i) * rect.width - .5 * pw1, (1 - zx->x_bpf->getPointRawY(i)) * rect.height - .5 * ph1, pw1, ph1);
            egraphics_fill(g);

            egraphics_set_color_hex(g, gensym("#505050"));

            float px = zx->x_bpf->getPointRawX(i) * rect.width;
            float py = (1 - zx->x_bpf->getPointRawY(i)) * rect.height;

            if (zx->x_bpf->getPointDist(i) < .1) {
                egraphics_rectangle(g, zx->x_bpf->getPointRawX(i) * rect.width - 6, (1 - zx->x_bpf->getPointRawY(i)) * rect.height - 6, 11, 11);
                egraphics_stroke(g);

                sprintf(c_val, "%.2f : %.2f", zx->x_bpf->getPointRawX(i) * zx->x_bpf->range_x + zx->x_bpf->shift_x, zx->x_bpf->getPointRawY(i) * zx->x_bpf->range_y + zx->x_bpf->shift_y);
            }

            //yet disabled
            //            float lw = (it->ldist<.4);// && (it->ldist>.2);
            //            if (lw<0) lw = 0;
            //            if (lw>1) lw=1;

            egraphics_set_line_width(g, 1); //yet disabled

            for (i = 1; i < zx->x_bpf->size(); i++) {
                egraphics_set_color_hex(g, gensym("#505050"));

                egraphics_set_line_width(g, 1);
                egraphics_line(g, px, py, zx->x_bpf->getPointRawX(i) * rect.width, (1 - zx->x_bpf->getPointRawY(i)) * rect.height);
                egraphics_stroke(g);

                px = zx->x_bpf->getPointRawX(i) * rect.width;
                py = (1 - zx->x_bpf->getPointRawY(i)) * rect.height;

                //yet disabled
                //                lw = (it->ldist<.4);
                //                if (lw<0) lw = 0;
                //                if (lw>1) lw=1;

                egraphics_set_line_width(g, 1);
                int pw1 = 6 - 4 * zx->x_bpf->getPointLockX(i);
                int ph1 = 6 - 4 * zx->x_bpf->getPointLockY(i);
                egraphics_rectangle(g, zx->x_bpf->getPointRawX(i) * rect.width - .5 * pw1, (1 - zx->x_bpf->getPointRawY(i)) * rect.height - .5 * ph1, pw1, ph1);
                egraphics_fill(g);

                if (zx->x_bpf->getPointDist(i) < .1) {
                    egraphics_set_color_hex(g, gensym((zx->del_mod) ? "#FF0000" : "#505050"));

                    egraphics_rectangle(g, zx->x_bpf->getPointRawX(i) * rect.width - 6, (1 - zx->x_bpf->getPointRawY(i)) * rect.height - 6, 11, 11);
                    egraphics_stroke(g);

                    sprintf(c_val, "%.2f : %.2f", zx->x_bpf->getPointRawX(i) * zx->x_bpf->range_x + zx->x_bpf->shift_x, zx->x_bpf->getPointRawY(i) * zx->x_bpf->range_y + zx->x_bpf->shift_y);
                }

                if (zx->select_idx == i) {
                    egraphics_set_color_hex(g, gensym("#00C0FF"));

                    egraphics_rectangle(g, zx->x_bpf->getPointRawX(i) * rect.width - 8, (1 - zx->x_bpf->getPointRawY(i)) * rect.height - 8, 15, 15);
                    egraphics_stroke(g);
                }

                if (zx->x_bpf->getEndSeg(i)) {
                    for (int sy = 0; sy < rect.height; sy += 5) {
                        egraphics_line(g, zx->x_bpf->getPointRawX(i) * rect.width, sy, zx->x_bpf->getPointRawX(i) * rect.width, sy + 2.5);
                    }
                    egraphics_stroke(g);
                }

                //i++;
            }
        }

        char c_min[10];
        sprintf(c_min, "%.2f", zx->x_bpf->shift_y);

        char c_max[10];
        sprintf(c_max, "%.2f", zx->x_bpf->range_y + zx->x_bpf->shift_y);

        etext_layout_set(zx->txt_min, c_min, zx->txt_font, 3, rect.height - 12, rect.width * 2, rect.height / 2, ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_min, g);

        etext_layout_set(zx->txt_max, c_max, zx->txt_font, 3, 12, rect.width * 2, rect.height / 2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_max, g);

        etext_layout_set(zx->txt_val, c_val, zx->txt_font, rect.width * .45, 12, rect.width * .1, rect.height / 2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        etext_layout_draw(zx->txt_val, g);
    }

    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox*)z, bgl, 0., 0.);
}

#pragma mark -
#pragma mark mouse

UI_fun(ui_bpfunc)::wx_mousemove_ext(ui_bpfunc* zx, t_object* view, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view(asBox(zx), &rect);

    float nx = pt.x / rect.width;
    float ny = pt.y / rect.height;

    //        float nlen = sqrtf(nx*nx+ny*ny);
    //        float nnx = (nlen!=0) ? nx/ nlen : 0;
    //        float nny = (nlen!=0) ? ny/ nlen : 0;

    for (int i = 0; i < zx->x_bpf->size(); i++) //it = zx->points->begin(); it != zx->points->end(); ++it
    {

        int j = ((i - 1) == zx->x_bpf->size()) ? i + 1 : i;

        float dx = nx - zx->x_bpf->getPointRawX(i);
        float dy = (ny - (1 - zx->x_bpf->getPointRawY(i)));

        float dist = sqrtf(dx * dx + dy * dy);
        zx->x_bpf->setPointDist(i, dist);

        bool sel = dist < .1;
        zx->x_bpf->setPointSel(i, sel);

        //todo ldist

        if ((zx->x_bpf->getPointRawX(i) < (pt.x / rect.width)) && ((pt.x / rect.width) < zx->x_bpf->getPointRawX(j))) {
            zx->addidx = i;
        } else {
            zx->addidx = (int)zx->x_bpf->size();
        }

        if (modifiers == EMOD_SHIFT) {
            zx->addpos = pt.x;
            zx->addpos_y = pt.y;
        } else {
            zx->addpos = -1;
            zx->del_mod = false;
        }

        if (sel)
            zx->delidx = i;

        if (modifiers == EMOD_ALT) {
            zx->del_mod = true;
        }
    }

    ws_redraw(zx);
}

UI_fun(ui_bpfunc)::wx_mouseleave_ext(ui_bpfunc* zx, t_object* view, t_pt pt, long modifiers)
{
    zx->addpos = -1;
}

UI_fun(ui_bpfunc)::wx_mousedown_ext(ui_bpfunc* zx, t_object* view, t_pt pt, long modifiers)
{
    zx->_px = pt.x;
    zx->_py = pt.y;

    t_rect rect;
    ebox_get_rect_for_view(asBox(zx), &rect);

    if (modifiers == EMOD_SHIFT) {
        zx->x_bpf->addPointRaw(zx->addidx, pt.x / rect.width, 1 - (pt.y / rect.height));
        ws_redraw(zx);
    }

    if (modifiers == EMOD_ALT) {
        zx->x_bpf->deletePoint(zx->delidx);
        ws_redraw(zx);
    }

    if (modifiers == EMOD_CTRL) {
        zx->x_bpf->setSplit(zx->delidx);
        ws_redraw(zx);
    }
}

UI_fun(ui_bpfunc)::wx_mouseup_ext(ui_bpfunc* zx, t_object* view, t_pt pt, long modifiers)
{
    zx->addpos = -1;
}

#pragma mark -
#pragma mark messages

//get list; scaled
UI_fun(ui_bpfunc)::m_bang(ui_bpfunc* z)
{

    ui_bpfunc* zx = (ui_bpfunc*)z;

    AtomList list = zx->x_bpf->getBpfList();
    list.output(zx->out1);
}

void bpf_m_raw(t_object* z, t_symbol* s, int argc, t_atom* argv)
{

    ui_bpfunc* zx = (ui_bpfunc*)z;

    AtomList list = zx->x_bpf->getBpfListRaw();
    list.output(zx->out1);
}

void bpf_m_range_x(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_bpfunc* zx = (ui_bpfunc*)z;
    zx->x_bpf->range_x = argv[0].a_w.w_float;
}

void bpf_m_range_y(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_bpfunc* zx = (ui_bpfunc*)z;
    zx->x_bpf->range_y = argv[0].a_w.w_float;
}

void bpf_m_shift_x(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_bpfunc* zx = (ui_bpfunc*)z;
    zx->x_bpf->shift_x = argv[0].a_w.w_float;
}

void bpf_m_shift_y(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_bpfunc* zx = (ui_bpfunc*)z;
    zx->x_bpf->shift_y = argv[0].a_w.w_float;
}

static void bpf_m_select(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    if (argv->a_type == A_FLOAT) {
        zx->select_idx = (int)(argv[0].a_w.w_float);
    }

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

static void bpf_m_drag_limit(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    if (argv->a_type == A_FLOAT) {
        zx->x_bpf->drag_limit = (int)(argv[0].a_w.w_float != 0);
    }

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_get(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;
    if (argv[0].a_type != A_FLOAT)
        return;

    int i = int(argv[0].a_w.w_float);
    if (i > zx->x_bpf->size() - 1)
        return; //i = (int)zx->x_bpf->size()-1;
    if (i < 0)
        return; //i=0;

    AtomList list = zx->x_bpf->getPoint(i);
    list.output(zx->out1);
}

void bpf_m_getraw(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 0)
        return;
    if (argv[0].a_type != A_FLOAT)
        return;

    int i = int(argv[0].a_w.w_float);
    if (i > zx->x_bpf->size() - 1)
        return; //i = (int)zx->x_bpf->size()-1;
    if (i < 0)
        return; //i=0;

    AtomList list = zx->x_bpf->getPointRaw(i);
    list.output(zx->out1);
}

void bpf_m_set(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 3)
        return;
    if (argv[0].a_type != A_FLOAT)
        return;

    int i = int(argv[0].a_w.w_float);
    if (i > zx->x_bpf->size() - 1)
        return; //i = (int)zx->x_bpf->size()-1;
    if (i < 0)
        return; //i=0;

    float x = argv[1].a_w.w_float;
    float y = argv[2].a_w.w_float;

    zx->x_bpf->setPoint(i, x, y);

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_set_raw(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 3)
        return;
    if (argv[0].a_type != A_FLOAT)
        return;

    int i = int(argv[0].a_w.w_float);
    if (i > zx->x_bpf->size() - 1)
        return; //i = (int)zx->x_bpf->size()-1;
    if (i < 0)
        return; //i=0;

    float x = argv[1].a_w.w_float;
    float y = argv[2].a_w.w_float;

    zx->x_bpf->setPointRaw(i, x, y);

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_end_seg(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;
    if (argv[0].a_type != A_FLOAT)
        return;

    int idx = int(argv[0].a_w.w_float);
    if (idx > zx->x_bpf->size() - 1)
        return; //idx = (int)zx->x_bpf->size()-1;
    if (idx < 0)
        return; //idx=0;

    bool b = argv[1].a_w.w_float != 0;

    zx->x_bpf->setPointSeg(idx, b);

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_seg_count(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (zx->out_list) {
        free(zx->out_list);
    }
    zx->out_list = (t_atom*)malloc(sizeof(t_atom));

    zx->out_list[0].a_type = A_FLOAT;
    zx->out_list[0].a_w.w_float = zx->x_bpf->getSegCount();

    outlet_list(zx->out1, &s_list, 1, zx->out_list);
}

void bpf_m_lock_x(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    if (argv[0].a_type == A_FLOAT) {
        int idx = (int)(argv[0].a_w.w_float);

        zx->x_bpf->lockX(idx, (argv[1].a_w.w_float != 0));
    }

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_lock_y(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    if (argv[0].a_type == A_FLOAT) {
        int idx = (int)(argv[0].a_w.w_float);
        zx->x_bpf->lockY(idx, (argv[1].a_w.w_float != 0));
    }

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_join_next(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    if (argv[0].a_type == A_FLOAT) {
        int idx = (int)(argv[0].a_w.w_float);
        zx->x_bpf->joinNext(idx, (argv[1].a_w.w_float != 0));
    }

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_clear(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    zx->x_bpf->clear();

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_add(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;
    if ((argv[0].a_type != A_FLOAT) && (argv[1].a_type != A_FLOAT))
        return;

    float xx = argv[0].a_w.w_float;
    float yy = argv[1].a_w.w_float;

    t_rect rect;
    zx->getRect(&rect);

    zx->x_bpf->addPoint(zx->addidx, xx, yy);

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_add_raw(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;
    if ((argv[0].a_type != A_FLOAT) && (argv[1].a_type != A_FLOAT))
        return;

    float xx = argv[0].a_w.w_float;
    float yy = argv[1].a_w.w_float;

    t_rect rect;
    zx->getRect(&rect);

    zx->x_bpf->addPointRaw(zx->addidx, xx, yy);

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_del(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;
    if (argv[0].a_type != A_FLOAT)
        return;

    int del_i = (int)argv[0].a_w.w_float;

    zx->x_bpf->deletePoint(del_i);

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_vline(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    vector<AtomList> output = zx->x_bpf->getVline();

    for (int i = 0; i < output.size(); i++) {
        output.at(i).output(zx->out1); //lol
    }
}

void bpf_m_vline_seg(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;
    if (argv[0].a_type != A_FLOAT) {
        return;
    }

    int seg_number = int(argv[0].a_w.w_float);

    if (seg_number < 0) {
        error("segment number should be greater than zero");
        return;
    }
    if (seg_number > (zx->x_bpf->getSegCount() - 1)) {
        error("segment number out of range");
        return;
    }

    vector<AtomList> output = zx->x_bpf->getVlineSegment(seg_number);

    for (int i = 0; i < output.size(); i++) {
        output.at(i).output(zx->out1); //lol
    }
}

void bpf_m_vline_tgl(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;
    if (argv[0].a_type != A_FLOAT) {
        return;
    }

    int seg_number = int(argv[0].a_w.w_float);

    if (seg_number < 0) {
        error("segment number should be 0 or 1");
        return;
    }
    if (seg_number > (1)) {
        error("segment number should be 0 or 1");
        return;
    }

    t_atom tgl;
    tgl.a_type = A_FLOAT;
    tgl.a_w.w_float = 1 - seg_number;

    bpf_m_vline_seg(zx, s, 1, &tgl);
}

void bpf_m_auto_send(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    if (argv->a_type == A_FLOAT) {
        zx->auto_send = (argv[0].a_w.w_float > 0);
    }
}

void bpf_m_setbpf(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    AtomList list = AtomList(argc, argv);

    zx->x_bpf->setBPFList(list);

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_setrawbpf(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    AtomList list = AtomList(argc, argv);

    zx->x_bpf->setBPFListRaw(list);

    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

void bpf_m_set_object(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    //        ui_bpfunc *zx = (ui_bpfunc*)z;
    //
    //        std::string obj_name = Atom(argv[0]).asString();
    //
    //        zx->g_bpf = new GlobalData<BPF>(obj_name);
    //        zx->x_bpf = &zx->g_bpf->ref();
    //
    //        GuiFactory<BaseGuiObject>::ws_redraw(z);
}

#pragma mark env

//list sending defines
#define e_list_clear list.clear();
#define e_list_addf(x) \
    a = Atom(x);       \
    list.append(a);

void bpf_m_env(ui_bpfunc* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1) {
        error("bad argument");
        return;
    }
    if (argv->a_type != A_SYMBOL) {
        error("bad argument");
        return;
    }

    if (Atom(argv[0]).asString() == "adsr") {
        //            ;
        //            bpf1 clear;
        //            bpf1 set_raw 1 0.25 1;
        //            bpf1 set_raw 2 0.5 0.6;
        //            bpf1 add_raw 1 0;
        //            bpf1 lock_y 0 1;
        //            bpf1 lock_y 1 1;
        //            bpf1 lock_y 3 1;
        //            bpf1 end_seg 2 1;
        //            bpf1 auto_send 1;
        //            bpf1 drag_limit 1

        AtomList list;
        Atom a;

        //todo cleanup

        bpf_m_clear(zx, 0, 0, 0);

        e_list_clear e_list_addf(1) e_list_addf(.25) e_list_addf(1)
            bpf_m_set_raw(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(2) e_list_addf(.5) e_list_addf(.6)
            bpf_m_set_raw(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(1) e_list_addf(0.)
            bpf_m_add_raw(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(0.) e_list_addf(1)
            bpf_m_lock_y(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(1.) e_list_addf(1)
            bpf_m_lock_y(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(3.) e_list_addf(1)
            bpf_m_lock_y(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(2) e_list_addf(1)
            bpf_m_end_seg(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(1)
            bpf_m_auto_send(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(1)
            bpf_m_drag_limit(zx, 0, (int)list.size(), list.toPdData());

    } else if (Atom(argv[0]).asString() == "ahr") {
        //            ;
        //            bpf2 clear;
        //            bpf2 set_raw 1 0.25 0.7;
        //            bpf2 set_raw 2 0.5 0.7;
        //            bpf2 add_raw 1 0;
        //            bpf2 lock_y 0 1;
        //            bpf2 lock_y 2 1;
        //            bpf2 join_next 1 1;
        //            bpf2 lock_y 3 1;
        //            bpf2 auto_send 1;
        //            bpf2 drag_limit 1

        AtomList list;
        Atom a;

        //todo cleanup
        bpf_m_clear(zx, 0, 0, 0);

        e_list_clear e_list_addf(1) e_list_addf(.25) e_list_addf(.7)
            bpf_m_set_raw(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(2) e_list_addf(.5) e_list_addf(.7)
            bpf_m_set_raw(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(1) e_list_addf(0.)
            bpf_m_add_raw(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(0.) e_list_addf(1)
            bpf_m_lock_y(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(2.) e_list_addf(1)
            bpf_m_lock_y(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(3.) e_list_addf(1)
            bpf_m_lock_y(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(1.) e_list_addf(1)
            bpf_m_join_next(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(1)
            bpf_m_auto_send(zx, 0, (int)list.size(), list.toPdData());
        e_list_clear e_list_addf(1)
            bpf_m_drag_limit(zx, 0, (int)list.size(), list.toPdData());
    } else {
        error("bad envelope type specified: %s", ceammc::Atom(argv[0]).asString().c_str());
    }

    //        if (argv->a_type == A_FLOAT)
    //        {
    //            ui_bpfunc *zx = (ui_bpfunc*)z;
    //            zx->auto_send = (argv[0].a_w.w_float>0);
    //        }
}

#pragma mark -
#pragma mark mouse+
UI_fun(ui_bpfunc)::wx_mousedrag_ext(ui_bpfunc* zx, t_object* view, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view(asBox(zx), &rect);

    for (int i = 0; i < zx->x_bpf->size(); i++) {
        if (zx->x_bpf->getPointSel(i)) {
            int prev = i - 1;
            int next = i + 1;

            float prev_x = -1.;
            if (prev >= 0)
                prev_x = zx->x_bpf->getPointRawX(prev);

            float next_x = 2.;
            if (next < zx->x_bpf->size())
                next_x = zx->x_bpf->getPointRawX(next);

            float dx = (pt.x - zx->_px) / rect.width;

            if (!((zx->x_bpf->drag_limit) && ((prev_x > (zx->x_bpf->getPointRawX(i) + dx)) || (next_x < (zx->x_bpf->getPointRawX(i) + dx))))) {
                float x = zx->x_bpf->getPointRawX(i);
                float y = zx->x_bpf->getPointRawY(i);
                float px = x;
                float py = y;

                if (!zx->x_bpf->getPointLockX(i))
                    x += dx;
                if (!zx->x_bpf->getPointLockY(i))
                    y += ((1 - pt.y) - (1 - zx->_py)) / rect.height;

                if (zx->x_bpf->getPointRawX(i) > 1)
                    x = 1;
                if (zx->x_bpf->getPointRawX(i) < 0)
                    x = 0;

                if (zx->x_bpf->getPointRawY(i) > 1)
                    y = 1;
                if (zx->x_bpf->getPointRawY(i) < 0)
                    y = 0;

                if (px != x || py != y) {
                    zx->x_bpf->setPointRaw(i, x, y);
                }

                if (zx->x_bpf->getJoinNext(i)) {
                    zx->x_bpf->setPointRaw(next, zx->x_bpf->getPointRawX(next), y);
                }
            }
        }
    }

    zx->_px = pt.x;
    zx->_py = pt.y;

    zx->x_bpf->_Sort();

    ws_redraw(zx);

    if (zx->auto_send) {
        m_bang(zx);
    }
}

#pragma mark -

UI_fun(ui_bpfunc)::new_ext(ui_bpfunc* zx, t_symbol* s, int argcl, t_atom* argv)
{
    //        zx->g_bpf = new GlobalData<BPF>(to_string((unsigned long) z));
    //        zx->x_bpf = &zx->g_bpf->ref();

    zx->x_bpf = new BPF;

    //zx->x_bpf->clear();

    zx->_px = 0;
    zx->_py = 0;

    zx->out1 = create_outlet(zx, &s_list);

    zx->x_bpf->range_x = 1000;
    zx->x_bpf->range_y = 1;
    zx->x_bpf->shift_x = 0;
    zx->x_bpf->shift_y = 0;

    zx->select_idx = -1;

    zx->auto_send = 0;

    zx->out_list = (t_atom*)malloc(sizeof(t_atom));

    zx->txt_max = etext_layout_create();
    zx->txt_min = etext_layout_create();
    zx->txt_val = etext_layout_create();

    zx->txt_font = efont_create(gensym("Helvetica"), gensym("light"), gensym("normal"), 8);
}

UI_fun(ui_bpfunc)::free_ext(ui_bpfunc* zx)
{
    //?
    zx->g_bpf = new GlobalData<BPF>(to_string((unsigned long)zx));
    delete zx->g_bpf;
}

static void ui_bpf_getdrawparams(ui_bpfunc* x, t_object* patcherview, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

UI_fun(ui_bpfunc)::init_ext(t_eclass* z)
{
    CLASS_ATTR_DEFAULT(z, "size", 0, "200. 150.");

    //TODO

    //        CLASS_ATTR_FLOAT(z, "shift_x", 0, ui_bpfunc, shift_x);
    //        CLASS_ATTR_DEFAULT(z, "shift_x", 0, "0");
    //        CLASS_ATTR_LABEL(z, "shift_x", 0, "shift_x");
    //        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift_x", 0, "0");
    //
    //        CLASS_ATTR_FLOAT(z, "range_x", 0, ui_bpfunc, range_x);
    //        CLASS_ATTR_DEFAULT(z, "range_x", 0, "1");
    //        CLASS_ATTR_LABEL(z, "range_x", 0, "range_x");
    //        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "range_x", 0, "1");
    //
    //        CLASS_ATTR_FLOAT(z, "shift_y", 0, ui_bpfunc, shift_y);
    //        CLASS_ATTR_DEFAULT(z, "shift_y", 0, "0");
    //        CLASS_ATTR_LABEL(z, "shift_y", 0, "shift_y");
    //        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift_y", 0, "0");
    //
    //        CLASS_ATTR_FLOAT(z, "range_y", 0, ui_bpfunc, range_y);
    //        CLASS_ATTR_DEFAULT(z, "range_y", 0, "1");
    //        CLASS_ATTR_LABEL(z, "range_y", 0, "range_y");
    //        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "range_y", 0, "1");
    //
    //        CLASS_ATTR_INT(z, "drag_limit", 0, ui_bpfunc, drag_limit);
    //        CLASS_ATTR_DEFAULT(z, "drag_limit", 0, "1");
    //        CLASS_ATTR_LABEL(z, "drag_limit", 0, "drag_limit");
    //        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "drag_limit", 0, "0");
    //        CLASS_ATTR_STYLE(z, "drag_limit", 0, "onoff");

    CLASS_ATTR_INT(z, "auto_send", 0, ui_bpfunc, auto_send);
    CLASS_ATTR_DEFAULT(z, "auto_send", 0, "1");
    CLASS_ATTR_LABEL(z, "auto_send", 0, _("Auto send"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "auto_send", 0, "0");
    CLASS_ATTR_STYLE(z, "auto_send", 0, "onoff");

    //todo readonly or disable?
    //        CLASS_ATTR_INT(z, "seg_count", 0, ui_bpfunc, seg_count);
    //        CLASS_ATTR_DEFAULT(z, "seg_count", 0, "1");
    //        CLASS_ATTR_LABEL(z, "seg_count", 0, "seg_count");
    //        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "seg_count", 0, "1");

    // TODO
    //eclass_addmethod(z, (method)(bpf_m_set_object), ("set_object"), A_GIMME,0);

    eclass_addmethod(z, (method)(bpf_m_range_x), ("range_x"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_range_y), ("range_y"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_shift_x), ("shift_x"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_shift_y), ("shift_y"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_select), ("sel"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_set), ("set"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_set_raw), ("set_raw"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_get), ("get"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_getraw), ("getraw"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_add), ("add"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_add_raw), ("add_raw"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_del), ("del"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_drag_limit), ("drag_limit"), A_GIMME, 0);

    //stubs

    //eclass_addmethod(z, (method)(bpf_m_set_seg), ("set_seg"), A_GIMME,0);
    eclass_addmethod(z, (method)(bpf_m_setbpf), ("set_bpf"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_setrawbpf), ("set_rawbpf"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_env), ("env"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_seg_count), ("seg_count"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_end_seg), ("end_seg"), A_GIMME, 0);

    //bang for getting the whole envelope
    eclass_addmethod(z, (method)(bpf_m_raw), ("raw"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_auto_send), ("auto_send"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_lock_x), ("lock_x"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_lock_y), ("lock_y"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_join_next), ("join_next"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_clear), ("clear"), A_GIMME, 0);

    eclass_addmethod(z, (method)(bpf_m_vline), ("vline"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_vline_seg), ("vline_seg"), A_GIMME, 0);
    eclass_addmethod(z, (method)(bpf_m_vline_tgl), ("vline_tgl"), A_GIMME, 0);

    eclass_addmethod(z, (method)(ui_bpf_getdrawparams), "getdrawparams", A_NULL, 0);
}
}

extern "C" void setup_ui0x2ebpfunc()
{
    GuiFactory<ui_bpfunc> class1;
    class1.setup("ui.bpfunc");
}
