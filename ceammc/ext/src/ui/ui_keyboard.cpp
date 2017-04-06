//
//  ui_keyboard.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 17/12/16.
//
//

#include "ceammc_gui.h"

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

struct ui_keyboard : public ceammc_gui::BaseGuiObject {
    t_outlet* out1;
    t_atom out_list[2];

    int keys;
    int shift;

    int _pitch;
    int _vel;

public:
    t_float realPitch() const { return _pitch + shift; }

    void output()
    {
        atom_setfloat(&out_list[0], realPitch());
        atom_setfloat(&out_list[1], _vel);

        outlet_list(out1, &s_list, 2, out_list);

        send(2, out_list);
    }
};

namespace ceammc_gui {

static t_symbol* COLOR_KEY_WHITE = gensym("#F0F0F0");
static t_symbol* COLOR_KEY_BLACK = gensym("#505050");
static t_symbol* COLOR_KEY_BORDER = gensym("#C0C0C0");

struct kRect {
    float x, y, w, h;
    bool is_black;
};

static size_t white_keys(size_t num_keys)
{
    size_t n_octave = num_keys / 12;
    size_t n_number = num_keys % 12;
    size_t wk = (n_number + (n_number < 6 ? 1 : 2)) >> 1;
    return n_octave * 7 + wk;
}

static float key_width(float keyboard_width, size_t num_keys)
{
    return keyboard_width / (white_keys(num_keys) * 2.f);
}

static kRect get_black_key_r(int offset, float kWidth, float kHeight)
{
    kRect ret;

    int n_number = offset % 12;
    int n_octave = offset / 12;

    ret.x = (offset + (n_number > 4 ? 1 : 0) + 2 * n_octave) * kWidth + 0.5f * kWidth;
    ret.y = 0;
    ret.w = kWidth;
    ret.h = kHeight * 0.6f;
    ret.is_black = true;

    return ret;
}

static kRect get_white_key_r(int offset, float kWidth, float kHeight)
{
    kRect ret;

    int n_number = offset % 12;
    int n_octave = offset / 12;

    ret.x = (offset + (n_number > 4 ? 1 : 0) + 2 * n_octave) * kWidth;
    ret.y = 0;
    ret.w = kWidth * 2;
    ret.h = kHeight;
    ret.is_black = false;

    return ret;
}

static kRect get_key_r(int number, float kWidth, float kHeight)
{
    int n_number = number % 12;
    bool is_black = ((n_number - (n_number > 4 ? 1 : 0)) % 2) == 1;

    return is_black ? get_black_key_r(number, kWidth, kHeight) : get_white_key_r(number, kWidth, kHeight);
}

static bool mouse_in_rect(int x, int y, const kRect& kr)
{
    return (x > kr.x) && x < (kr.x + kr.w) && y > (kr.y) && y < (kr.y + kr.h);
}

UI_fun(ui_keyboard)::wx_paint(ui_keyboard* zx, t_object* /*view*/)
{
    t_symbol* bgl = BG_LAYER;

    t_rect rect;
    zx->getRect(&rect);

    t_elayer* g = ebox_start_layer(asBox(zx), bgl, rect.width, rect.height);

    if (g) {
        if (zx->keys < 3)
            zx->keys = 3; // rapper's piano meme
        if (zx->keys > 127)
            zx->keys = 127;

        float kWidth = key_width(rect.width, zx->keys);

        // two pass draw
        // white keys first
        for (int i = 0; i < zx->keys; i++) {
            kRect k = get_key_r(i, kWidth, rect.height);

            if (!k.is_black) {
                bool hover = (i == zx->_pitch);
                bool click = zx->mouse_dn;

                egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                egraphics_set_color_hex(g, (hover) ? (click ? COLOR_ACTIVE : COLOR_KEY_WHITE) : COLOR_KEY_WHITE);
                egraphics_fill(g);

                egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                egraphics_set_color_hex(g, (hover) ? COLOR_ACTIVE : COLOR_KEY_BORDER);
                egraphics_stroke(g);

                if ((i + zx->shift) == 60) { // middle C
                    egraphics_set_line_width(g, 2);
                    float w = k.w * 0.75f;
                    egraphics_line(g, k.x, k.y, k.x + w / 2, k.y + w / 2);
                    egraphics_line(g, k.x + w, k.y, k.x + w / 2, k.y + w / 2);
                    egraphics_line(g, k.x, k.y, k.x + w, k.y);
                    egraphics_set_color_hex(g, COLOR_ACTIVE);
                    egraphics_stroke(g);
                    egraphics_set_line_width(g, 1);
                }
            }
        }

        for (int i = 0; i < zx->keys; i++) {
            kRect k = get_key_r(i, kWidth, rect.height);

            if (k.is_black) {
                bool hover = (i == int(zx->_pitch));
                bool click = zx->mouse_dn;

                egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                egraphics_set_color_hex(g, (hover) ? (click ? COLOR_ACTIVE : COLOR_KEY_BLACK) : COLOR_KEY_BLACK);
                egraphics_fill(g);

                egraphics_rectangle(g, k.x, k.y, k.w, k.h);
                egraphics_set_color_hex(g, (hover) ? COLOR_ACTIVE : COLOR_KEY_BORDER);
                egraphics_stroke(g);
            }
        }

        ebox_end_layer(asBox(zx), bgl);
    }

    ebox_paint_layer(asBox(zx), bgl, 0., 0.);
}

UI_fun(ui_keyboard)::wx_mousemove_ext(ui_keyboard* zx, t_object* /*view*/, t_pt /*pt*/, long /*modifiers*/)
{
    t_rect rect;
    ebox_get_rect_for_view(asBox(zx), &rect);

    float kWidth = key_width(rect.width, zx->keys);

    for (int i = 0; i < zx->keys; i++) {
        kRect k = get_key_r(i, kWidth, rect.height);

        if (!k.is_black) {
            if (mouse_in_rect(zx->mouse_x, zx->mouse_y, k)) {
                zx->_pitch = i;
            }
        }
    }

    for (int i = 0; i < zx->keys; i++) {
        kRect k = get_key_r(i, kWidth, rect.height);

        if (k.is_black) {
            if (mouse_in_rect(zx->mouse_x, zx->mouse_y, k)) {
                zx->_pitch = i;
            }
        }
    }
}

UI_fun(ui_keyboard)::wx_mousedown_ext(ui_keyboard* zx, t_object* /*view*/, t_pt pt, long /*modifiers*/)
{
    t_rect rect;
    ebox_get_rect_for_view(asBox(zx), &rect);

    // calc velocity
    zx->_vel = int(pt.y / rect.height * 127.f);

    zx->output();
    ws_redraw(zx);
}

UI_fun(ui_keyboard)::wx_mouseup_ext(ui_keyboard* zx, t_object* /*view*/, t_pt /*pt*/, long /*modifiers*/)
{
    zx->_vel = 0;

    zx->output();
    ws_redraw(zx);
}

UI_fun(ui_keyboard)::wx_mousedrag_ext(ui_keyboard* zx, t_object* view, t_pt pt, long modifiers)
{
    int prev_pitch = zx->_pitch;

    wx_mousemove_ext(zx, view, pt, modifiers);

    if (prev_pitch != zx->_pitch) {
        int tmp_vel = zx->_vel;
        int tmp_pitch = zx->_pitch;

        // release previous note
        zx->_vel = 0;
        zx->_pitch = prev_pitch;
        zx->output();

        // press new note
        zx->_vel = tmp_vel;
        zx->_pitch = tmp_pitch;
        zx->output();

        ws_redraw(zx);
    }
}

UI_fun(ui_keyboard)::wx_oksize(ui_keyboard* zx, t_rect* newrect)
{
    float min_width = white_keys(zx->keys) * 8;
    newrect->width = pd_clip_min(newrect->width, min_width);
    newrect->height = pd_clip_min(newrect->height, 40.);
}

UI_fun(ui_keyboard)::wx_mouseleave_ext(ui_keyboard* zx, t_object* /*view*/, t_pt /*pt*/, long /*modifiers*/)
{
    zx->_pitch = -1;
    ws_redraw(zx);
}

static void ui_k_getdrawparams(ui_keyboard* x, t_object* /*patcherview*/, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
}

UI_fun(ui_keyboard)::wx_attr_changed_ext(ui_keyboard* z, t_symbol* attr)
{
    if (attr == gensym("keys"))
        ws_redraw(z);
}

UI_fun(ui_keyboard)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_INVISIBLE            (z, PROP_BACKGROUND_COLOR, 0);
    CLASS_ATTR_DEFAULT              (z, "size", 0, "433. 60.");

    CLASS_ATTR_INT                  (z, "shift", 0, ui_keyboard, shift);
    CLASS_ATTR_DEFAULT              (z, "shift", 0, "36");
    CLASS_ATTR_LABEL                (z, "shift", 0, _("Leftmost MIDI note"));
    CLASS_ATTR_STYLE                (z, "shift", 0, "number");
    CLASS_ATTR_FILTER_CLIP          (z, "shift", 0, 127);
    CLASS_ATTR_STEP                 (z, "shift", 1);
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "shift", 0, "36");

    CLASS_ATTR_INT                  (z, "keys", 0, ui_keyboard, keys);
    CLASS_ATTR_DEFAULT              (z, "keys", 0, "61");
    CLASS_ATTR_LABEL                (z, "keys", 0, _("Keys"));
    CLASS_ATTR_STYLE                (z, "keys", 0, "number");
    CLASS_ATTR_FILTER_CLIP          (z, "keys", 5, 88);
    CLASS_ATTR_STEP                 (z, "keys", 1);
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "keys", 0, "61");
    // clang-format on

    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_k_getdrawparams), "getdrawparams", A_NULL, 0);
}

UI_fun(ui_keyboard)::new_ext(ui_keyboard* zx, t_symbol* /*s*/, int /*argcl*/, t_atom* /*argv*/)
{
    zx->out1 = create_outlet(zx);
}
}

extern "C" void setup_ui0x2ekeyboard()
{
    ceammc_gui::GuiFactory<ui_keyboard> class1;
    class1.setup("ui.keyboard");
}
