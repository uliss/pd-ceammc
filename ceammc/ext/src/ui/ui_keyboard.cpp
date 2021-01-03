//
//  ui_keyboard.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 17/12/16.
//
//

#include "ui_keyboard.h"
#include "ceammc_music_theory_keyboard.h"
#include "ceammc_ui.h"

static t_rgba RGBA_WHITE = hex_to_rgba("#F0F0F0");
static t_rgba RGBA_BLACK = hex_to_rgba("#505050");

size_t keyboard_num_white_keys(size_t num_keys)
{
    size_t n_octave = num_keys / 12;
    size_t n_number = num_keys % 12;
    size_t wk = (n_number + (n_number < 6 ? 1 : 2)) >> 1;
    return n_octave * 7 + wk;
}

static t_rect black_key_rect(int offset, float black_key_w, float key_h)
{
    t_rect ret;

    int n_number = offset % 12;
    int n_octave = offset / 12;

    ret.x = (offset + (n_number > 4 ? 1 : 0) + 2 * n_octave) * black_key_w + 0.5f * black_key_w - 1;
    ret.y = -1;
    ret.width = black_key_w;
    ret.height = key_h * 0.6f;

    return ret;
}

static t_rect white_key_rect(int offset, float black_key_w, float key_h)
{
    t_rect ret;

    int n_number = offset % 12;
    int n_octave = offset / 12;

    ret.x = (offset + (n_number > 4 ? 1 : 0) + 2 * n_octave) * black_key_w - 1;
    ret.y = -1;
    ret.width = black_key_w * 2;
    ret.height = key_h + 1;

    return ret;
}

std::vector<t_pt> white_key_poly(int offset, float black_key_w, float key_h)
{
    enum side_t {
        SIDE_LEFT = 0,
        SIDE_RIGHT = 1
    };

    enum offset_t {
        NO_OFF = 0,
        LEFT_OFF = 1,
        RIGHT_OFF = -1
    };

    struct PointTemplate {
        size_t side;
        offset_t off;
        float height;
    };

    static const float Y_TOP = 0;
    static const float Y_BLACK = 0.6f;
    static const float Y_BOTTOM = 1;

    static std::vector<PointTemplate> key_templates[] = {
        // white key template: C
        // .__.
        // |  |
        // |  |
        // |  |_.
        // |    |
        // |____|

        { { SIDE_LEFT, NO_OFF, Y_TOP },
            { SIDE_LEFT, NO_OFF, Y_BOTTOM },
            { SIDE_RIGHT, NO_OFF, Y_BOTTOM },
            { SIDE_RIGHT, NO_OFF, Y_BLACK },
            { SIDE_RIGHT, RIGHT_OFF, Y_BLACK },
            { SIDE_RIGHT, RIGHT_OFF, Y_TOP } },

        // white key template: D
        //  .__.
        //  |  |
        //  |  |
        //  |  |
        // ..  ..
        // |    |
        // |____|
        {
            { SIDE_LEFT, LEFT_OFF, Y_TOP },
            { SIDE_LEFT, LEFT_OFF, Y_BLACK },
            { SIDE_LEFT, NO_OFF, Y_BLACK },
            { SIDE_LEFT, NO_OFF, Y_BOTTOM },
            { SIDE_RIGHT, NO_OFF, Y_BOTTOM },
            { SIDE_RIGHT, NO_OFF, Y_BLACK },
            { SIDE_RIGHT, RIGHT_OFF, Y_BLACK },
            { SIDE_RIGHT, RIGHT_OFF, Y_TOP } },

        // white key template: E
        //   .__.
        //   |  |
        //   |  |
        //   |  |
        // ._|  |
        // |    |
        // |____|
        {
            { SIDE_LEFT, LEFT_OFF, Y_TOP },
            { SIDE_LEFT, LEFT_OFF, Y_BLACK },
            { SIDE_LEFT, NO_OFF, Y_BLACK },
            { SIDE_LEFT, NO_OFF, Y_BOTTOM },
            { SIDE_RIGHT, NO_OFF, Y_BOTTOM },
            { SIDE_RIGHT, NO_OFF, Y_TOP } }
    };

    const int n_number = offset % 12;
    const int n_octave = offset / 12;

    int template_idx = 0;
    switch (n_number) {
    case 0:
    case 5:
        template_idx = 0;
        break;
    case 2:
    case 7:
    case 9:
        template_idx = 1;
        break;
    case 4:
    case 11:
        template_idx = 2;
        break;
    }

    const std::vector<PointTemplate>& ktmpl = key_templates[template_idx];
    std::vector<t_pt> res;
    res.reserve(ktmpl.size());

    const float white_key_w = black_key_w * 2;

    for (const PointTemplate& ptmp : ktmpl) {
        t_pt pt;
        pt.x = (offset + (n_number > 4 ? 1 : 0) + 2 * n_octave) * black_key_w
            + white_key_w * ptmp.side + black_key_w * ptmp.off * 0.5;
        pt.y = ptmp.height * key_h;
        res.push_back(pt);
    }

    return res;
}

void setup_ui_keyboard()
{
    UIKeyboard::setup();
}

UIKeyboard::UIKeyboard()
    : keys_(61)
    , shift_(36)
    , current_key_(0)
    , velocity_(0)
    , mouse_pressed_(false)
    , prop_color_active_(rgba_black)
    , key_layer_(asEBox(), gensym("keys_layer"))
{
    appendToLayerList(&key_layer_);
    createOutlet();

    initPopupMenu("keyboard",
        { { _("release all"), [this](const t_pt&) { resetAllNotes(); } },
            PopupMenuCallbacks::sep(),
            { _("maj"),
                [this](const t_pt&) {
                  releaseAllNotes();
                  playChord({ 0, 4, 7 });
                  redrawLayer(key_layer_); } },
            { _("min"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 3, 7 });
                 redrawLayer(key_layer_);
             } },
            { _("aug"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 4, 8 });
                 redrawLayer(key_layer_);
             } },
            { _("dim"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 3, 6 });
                 redrawLayer(key_layer_);
             } },
            { _("7"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 4, 7, 10 });
                 redrawLayer(key_layer_);
             } },
            { _("maj7"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 4, 7, 11 });
                 redrawLayer(key_layer_);
             } },
            { _("min7"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 3, 7, 10 });
                 redrawLayer(key_layer_);
             } },
            { _("aug7"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 4, 8, 10 });
                 redrawLayer(key_layer_);
             } } });
}

bool UIKeyboard::okSize(t_rect* newrect)
{
    float min_width = keyboard_num_white_keys(keys_) * 8;
    newrect->width = pd_clip_min(newrect->width, min_width);
    newrect->height = pd_clip_min(newrect->height, 40.);
    return true;
}

void UIKeyboard::paint()
{
    if (keyboard_num_white_keys(keys_) < 1)
        return;

    drawBackground();
    drawActive();
}

void UIKeyboard::releaseAllNotes()
{
    for (int k : sustained_keys_) {
        Atom res[2];
        res[0] = shift_ + k;
        res[1] = 0.f;
        listTo(0, AtomListView(&res->atom(), 2));
    }

    sustained_keys_.clear();
}

void UIKeyboard::resetAllNotes()
{
    releaseAllNotes();

    velocity_ = 0;
    current_key_ = -1;

    redrawLayer(key_layer_);
}

void UIKeyboard::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    // release all notes
    if (modifiers & EMOD_ALT)
        return resetAllNotes();

    // calc velocity
    velocity_ = std::min<int>(127, int(pt.y / height() * 100.f) + 27);
    mouse_pressed_ = true;
    current_key_ = findPressedKey(pt);
    if (current_key_ < 0)
        return;

    // sustain mode
    if (modifiers & EMOD_SHIFT) {
        // add new sustaind note
        if (sustained_keys_.find(current_key_) == sustained_keys_.end()) {
            sustained_keys_.insert(current_key_);
            output();
            current_key_ = -1;
        } else { // remove sustained note
            sustained_keys_.erase(current_key_);
            velocity_ = 0;
            output();
            current_key_ = -1;
        }
    } else {
        output();
    }

    redrawLayer(key_layer_);
}

void UIKeyboard::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    if (current_key_ == -1)
        return;

    velocity_ = 0;
    mouse_pressed_ = false;
    output();
    current_key_ = -1;

    redrawLayer(key_layer_);
}

void UIKeyboard::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    current_key_ = findPressedKey(pt);
}

void UIKeyboard::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    current_key_ = -1;
    mouse_pressed_ = false;

    redrawLayer(key_layer_);
}

void UIKeyboard::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    int prev_pitch = current_key_;
    onMouseMove(view, pt, modifiers);

    if (prev_pitch != current_key_) {
        int new_velocity = velocity_;
        int new_pitch = current_key_;

        // release previous note
        velocity_ = 0;
        current_key_ = prev_pitch;
        output();

        // press new note
        velocity_ = new_velocity;
        current_key_ = new_pitch;
        output();

        redrawLayer(key_layer_);
    }
}

void UIKeyboard::showPopup(const t_pt& pt, const t_pt& abs_pt)
{
    UIPopupMenu menu(asEObj(), popup_menu_list_.front(), abs_pt, pt);

    velocity_ = std::min<int>(127, int(pt.y / height() * 100.f) + 27);
    current_key_ = findPressedKey(pt);
}

void UIKeyboard::playChord(const std::unordered_set<int>& keys)
{
    if (current_key_ == -1)
        current_key_ = 60 - shift_;

    for (int k : keys)
        sustained_keys_.insert(current_key_ + k);

    output();
}

int UIKeyboard::findPressedKey(const t_pt& pt) const
{
    int res = -1;

    const float black_key_w = width() / (keyboard_num_white_keys(keys_) * 2.f);
    const float white_key_w = black_key_w * 2;
    const int key = music::keyboard::wkey_to_key<int>(round(pt.x / white_key_w));

    // check nearest white keys
    for (int i = -2; i < 2; i++) {
        int idx = key + i;

        if (music::keyboard::is_white_key(idx)) {
            if (contains_point(white_key_rect(idx, black_key_w, height()), pt)) {
                res = idx;
            }
        }
    }

    // check nearest black keys
    for (int i = -2; i < 2; i++) {
        int idx = key + i;

        if (music::keyboard::is_black_key(idx)) {
            if (contains_point(black_key_rect(idx, black_key_w, height()), pt)) {
                res = idx;
            }
        }
    }

    if (res >= keys_)
        return keys_ - 1;

    return res;
}

const char* UIKeyboard::annotateOutlet(int n) const
{
    return "list: pitch velocity";
}

void UIKeyboard::setup()
{
    UIObjectFactory<UIKeyboard> obj("ui.keyboard");

    obj.hideLabelInner();
    obj.useAnnotations();
    obj.useBang();
    obj.usePopup();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG
        | UI_MOUSE_MOVE | UI_MOUSE_LEAVE
        | UI_MOUSE_WHEEL | UI_MOUSE_UP);

    obj.addProperty("active_color", _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIKeyboard::prop_color_active_);
    obj.hideProperty(PROP_BACKGROUND_COLOR);

    obj.addProperty("keys", _("Keys"), 61, &UIKeyboard::keys_, _("Main"));
    obj.setDefaultSize(433, 60);
    obj.setPropertyRange("keys", 5, 88);

    obj.addProperty("shift", _("Leftmost MIDI note"), 36, &UIKeyboard::shift_, _("Main"));
    obj.setPropertyRange("shift", 6, 88);
}

int UIKeyboard::realPitch() const
{
    return (shift_ + current_key_);
}

void UIKeyboard::output()
{
    if (current_key_ == -1)
        return;

    Atom res[2];
    res[0] = realPitch();
    res[1] = velocity_;
    listTo(0, AtomListView(&res->atom(), 2));
}

void UIKeyboard::drawBackground()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);
    const float black_key_w = width() / (keyboard_num_white_keys(keys_) * 2.f);
    const float key_h = height();

    if (p) {
        p.preAllocObjects(keys_);
        p.preAllocPoints(keys_ * 3);

        // two pass draw
        // draw white keys first
        p.setColor(prop_color_border);
        for (int i = 0; i < keys_; i++) {
            if (music::keyboard::is_black_key(i))
                continue;

            t_rect key_rect = white_key_rect(i, black_key_w, key_h);
            p.drawLine(key_rect.x, 0, key_rect.x, key_h);
        }
        p.stroke();

        // draw black keys
        for (int i = 0; i < keys_; i++) {
            if (music::keyboard::is_white_key(i))
                continue;

            p.preAllocPoints(4);
            t_rect key_r = black_key_rect(i, black_key_w, key_h);

            p.drawRect(key_r);
            p.setColor(RGBA_BLACK);
            p.fillPreserve();

            p.setColor(prop_color_border);
            p.stroke();
        }
    }
}

void UIKeyboard::drawActive()
{
    const t_rect r = rect();
    const float black_key_w = width() / (keyboard_num_white_keys(keys_) * 2.f);
    const float key_h = height();

    UIPainter kp = key_layer_.painter(r);
    if (kp) {
        if (current_key_ >= 0 && mouse_pressed_) {
            if (music::keyboard::is_black_key(current_key_)) {
                t_rect key_r = black_key_rect(current_key_, black_key_w, key_h);

                kp.drawRect(key_r);
                kp.setColor(prop_color_active_);
                kp.fill();
            } else {
                const bool is_C = (current_key_ % 12) == 0;
                const bool is_F = (current_key_ % 12) == 5;
                // last white full key (C or F)
                const bool full = ((current_key_ + 1) == keys_) && (is_C || is_F);

                if (full) {
                    t_rect key_r = white_key_rect(current_key_, black_key_w, key_h);
                    kp.drawRect(key_r);
                    kp.setColor(prop_color_active_);
                    kp.fill();
                } else {
                    auto vec = white_key_poly(current_key_, black_key_w, key_h);
                    kp.drawPoly(vec);
                    kp.setColor(prop_color_active_);
                    kp.fill();
                }
            }
        }

        for (int key : sustained_keys_) {
            if (music::keyboard::is_black_key(key)) {
                t_rect key_r = black_key_rect(key, black_key_w, key_h);

                kp.drawRect(key_r);
                kp.setColor(prop_color_active_);
                kp.fill();
            } else {
                const bool is_C = (key % 12) == 0;
                const bool is_F = (key % 12) == 5;
                // last white full key (C or F)
                const bool full = ((key + 1) == keys_) && (is_C || is_F);

                if (full) {
                    t_rect key_r = white_key_rect(key, black_key_w, key_h);
                    kp.drawRect(key_r);
                    kp.setColor(prop_color_active_);
                    kp.fill();
                } else {
                    auto vec = white_key_poly(key, black_key_w, key_h);
                    kp.drawPoly(vec);
                    kp.setColor(prop_color_active_);
                    kp.fill();
                }
            }
        }
    }
}
