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

size_t keyboard_white_keys(size_t num_keys)
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
{
    createOutlet();
}

bool UIKeyboard::okSize(t_rect* newrect)
{
    float min_width = keyboard_white_keys(keys_) * 8;
    newrect->width = pd_clip_min(newrect->width, min_width);
    newrect->height = pd_clip_min(newrect->height, 40.);
    return true;
}

void UIKeyboard::paint(t_object* view)
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (p) {
        const float black_key_w = width() / (keyboard_white_keys(keys_) * 2.f);
        const float key_h = height();

        // two pass draw
        // draw white keys first
        for (int i = 0; i < keys_; i++) {
            if (music::keyboard::is_black_key(i))
                continue;

            t_rect key_rect = white_key_rect(i, black_key_w, key_h);

            // current note or sustained
            const bool hover = ((i == current_key_) && mouse_pressed_)
                || (sustained_keys_.find(i) != sustained_keys_.end());

            p.drawRect(key_rect);
            p.setColor(hover ? prop_color_active_ : RGBA_WHITE);
            p.fillPreserve();

            p.setColor(hover ? prop_color_active_ : prop_color_border);
            p.stroke();

            if (i + shift_ == 60) { // middle C indicator
                p.setLineWidth(2);
                p.setColor(prop_color_active_);

                const float w = black_key_w * 1.5;

                p.drawLine(key_rect.x, key_rect.y, key_rect.x + w / 2, key_rect.y + w / 2);
                p.drawLine(key_rect.x + w, key_rect.y, key_rect.x + w / 2, key_rect.y + w / 2);
                p.drawLine(key_rect.x, key_rect.y, key_rect.x + w, key_rect.y);

                p.setLineWidth(1);
            }
        }

        // draw black keys
        for (int i = 0; i < keys_; i++) {
            if (music::keyboard::is_white_key(i))
                continue;

            t_rect key_r = black_key_rect(i, black_key_w, key_h);

            // current note or sustained
            const bool hover = ((i == current_key_) && mouse_pressed_)
                || (sustained_keys_.find(i) != sustained_keys_.end());

            p.drawRect(key_r);
            p.setColor(hover ? prop_color_active_ : RGBA_BLACK);
            p.fillPreserve();

            p.setColor(hover ? prop_color_active_ : prop_color_border);
            p.stroke();
        }
    }
}

void UIKeyboard::onMouseDown(t_object* view, const t_pt& pt, long modifiers)
{
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

    redrawBGLayer();
}

void UIKeyboard::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    if (current_key_ == -1)
        return;

    velocity_ = 0;
    mouse_pressed_ = false;
    output();
    redrawBGLayer();
    current_key_ = -1;
}

void UIKeyboard::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    current_key_ = findPressedKey(pt);
}

void UIKeyboard::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    current_key_ = -1;
    mouse_pressed_ = false;
    redrawBGLayer();
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

        redrawBGLayer();
    }
}

int UIKeyboard::findPressedKey(const t_pt& pt) const
{
    int res = -1;

    const float black_key_w = width() / (keyboard_white_keys(keys_) * 2.f);
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

    return res;
}

void UIKeyboard::setup()
{
    UIObjectFactory<UIKeyboard> obj("ui.keyboard");

    obj.useBang();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG
        | UI_MOUSE_MOVE | UI_MOUSE_LEAVE
        | UI_MOUSE_WHEEL | UI_MOUSE_UP);

    obj.addProperty("active_color", _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIKeyboard::prop_color_active_);
    obj.hideProperty(PROP_BACKGROUND_COLOR);

    obj.addProperty("keys", _("Keys"), 61, &UIKeyboard::keys_);
    obj.setDefaultSize(433, 60);
    obj.setPropertyRange("keys", 5, 88);

    obj.addProperty("shift", _("Leftmost MIDI note"), 36, &UIKeyboard::shift_);
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

    listTo(0, AtomList(realPitch(), velocity_));
}
