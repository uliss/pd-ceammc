//
//  ui_keyboard.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 17/12/16.
//
//

#include "ui_keyboard.h"
#include "ceammc_convert.h"
#include "ceammc_music_theory_keyboard.h"
#include "ceammc_ui.h"
#include "ui_keyboard.tcl.h"

#include <cinttypes>

constexpr int NO_KEY = -1;
constexpr int MIN_KEYS = 5;
constexpr int DEFAULT_SHIFT = 36;
constexpr int DEFAULT_HEIGHT = 60;
constexpr float MIN_WKEY_WIDTH = 8;
constexpr float DEFAULT_WKEY_WIDTH = 12;
constexpr uint32_t RGBA_WHITE = 0xF0F0F0;
constexpr uint32_t RGBA_BLACK = 0x505050;

static t_symbol* SYM_VKEYBOARD;
static t_symbol* SYM_VKEYBOARD2;
static t_symbol* SYM_PROP_KEYS;

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

UIKeyboard::UIKeyboard()
    : prop_keys(61)
    , prop_shift(36)
    , current_key_(NO_KEY)
    , velocity_(0)
    , mouse_pressed_(false)
    , prop_color_active_(rgba_black)
{
    createOutlet();

    initPopupMenu("keyboard",
        { { _("release all"), [this](const t_pt&) { resetAllNotes(); } },
            PopupMenuCallbacks::sep(),
            { _("maj"),
                [this](const t_pt&) {
                  releaseAllNotes();
                  playChord({ 0, 4, 7 });
                  redraw(); } },
            { _("min"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 3, 7 });
                 redraw();
             } },
            { _("aug"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 4, 8 });
                 redraw();
             } },
            { _("dim"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 3, 6 });
                 redraw();
             } },
            { _("7"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 4, 7, 10 });
                 redraw();
             } },
            { _("maj7"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 4, 7, 11 });
                 redraw();
             } },
            { _("min7"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 3, 7, 10 });
                 redraw();
             } },
            { _("aug7"), [this](const t_pt&) {
                 releaseAllNotes();
                 playChord({ 0, 4, 8, 10 });
                 redraw();
             } } });
}

bool UIKeyboard::okSize(t_rect* newrect)
{
    const float min_length = keyboard_num_white_keys(prop_keys) * MIN_WKEY_WIDTH;

    if (prop_vertical) {
        newrect->width = pd_clip_min(newrect->width, 40.);
        newrect->height = pd_clip_min(newrect->height, min_length);
    } else {
        newrect->width = pd_clip_min(newrect->width, min_length);
        newrect->height = pd_clip_min(newrect->height, 40.);
    }
    return true;
}

void UIKeyboard::paint()
{
    const int nw = keyboard_num_white_keys(prop_keys);
    if (nw < 1)
        return;

    // fill string with 1 and 0
    char bits[MAX_KEYS + 1];
    for (size_t i = 0; i < active_keys_.size(); i++)
        bits[i] = active_keys_.test(i) ? '1' : '0';

    if (current_key_ >= 0 && current_key_ < active_keys_.size())
        bits[current_key_] = '1';
    bits[MAX_KEYS] = '\0';

    sys_vgui("ui::keyboard_delete_keys #%x %s\n",
        asEBox(), asEBox()->b_drawing_id->s_name);

    if (prop_vertical) {
        sys_vgui("ui::keyboard_create_vkeys #%x %s "
                 "%d %d "
                 "#%6.6x #%6.6x #%6.6x #%6.6x "
                 "%.2f %.2f %s\n",
            asEBox(), asEBox()->b_drawing_id->s_name,
            prop_keys, nw,
            RGBA_WHITE,
            RGBA_BLACK,
            rgba_to_hex_int(prop_color_active_),
            rgba_to_hex_int(prop_color_border),
            width(), height(), bits);
    } else {
        sys_vgui("ui::keyboard_create_hkeys #%x %s %d "
                 "#%6.6x #%6.6x #%6.6x #%6.6x "
                 "%.2f %.2f %s\n",
            asEBox(),
            asEBox()->b_drawing_id->s_name,
            prop_keys,
            RGBA_WHITE,
            RGBA_BLACK,
            rgba_to_hex_int(prop_color_active_),
            rgba_to_hex_int(prop_color_border),
            width() / nw, height(), bits);
    }
}

void UIKeyboard::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    if (name == SYM_VKEYBOARD || name == SYM_VKEYBOARD2)
        prop_vertical = 1;

    // number of keys
    const int nkeys = args.intAt(0, -1);

    // no positional arguments or invalid
    if (nkeys <= MIN_KEYS) {
        // set prop_keys
        const size_t N = args.size();
        for (size_t i = 0; i < N; i++) {
            const auto& a = args[i];
            if (a != SYM_PROP_KEYS)
                continue;

            if (i + 1 >= N)
                break;

            prop_keys = clip<int>(args[i + 1].asInt(0), MIN_KEYS, MAX_KEYS);
            break;
        }
    } else {
        prop_keys = clip<int>(nkeys, MIN_KEYS, MAX_KEYS);
    }

    // set dimensions
    const int dim1 = DEFAULT_HEIGHT;
    const int dim2 = keyboard_num_white_keys(prop_keys) * DEFAULT_WKEY_WIDTH;
    if (prop_vertical) {
        asEBox()->b_rect.width = dim1;
        asEBox()->b_rect.height = dim2;
    } else {
        asEBox()->b_rect.width = dim2;
        asEBox()->b_rect.height = dim1;
    }

    // check if first argument is not property
    if (args.size() > 2 && args[0].isFloat()) {
        // key shift
        prop_shift = args.intAt(1, DEFAULT_SHIFT);
    }
}

void UIKeyboard::releaseAllNotes()
{
    for (size_t i = 0; i < active_keys_.size(); i++) {
        if (active_keys_.test(i)) {
            Atom res[2];
            res[0] = prop_shift + i;
            res[1] = 0.f;
            listTo(0, AtomListView(res, 2));
        }
    }

    active_keys_.reset();
}

void UIKeyboard::resetAllNotes()
{
    releaseAllNotes();

    velocity_ = 0;
    current_key_ = NO_KEY;

    redraw();
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
        if (active_keys_.test(current_key_)) { // remove sustained note
            active_keys_.set(current_key_, false);
            velocity_ = 0;
            outputCurrentKey();
            current_key_ = NO_KEY;
        } else { // add new sustaind note
            active_keys_.set(current_key_, true);
            outputCurrentKey();
            current_key_ = NO_KEY;
        }
    } else {
        outputCurrentKey();
    }

    redraw();
}

void UIKeyboard::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    if (current_key_ == NO_KEY)
        return;

    velocity_ = 0;
    mouse_pressed_ = false;
    outputCurrentKey();
    current_key_ = NO_KEY;

    redraw();
}

void UIKeyboard::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    current_key_ = findPressedKey(pt);
}

void UIKeyboard::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    current_key_ = NO_KEY;
    mouse_pressed_ = false;

    redraw();
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
        outputCurrentKey();

        // press new note
        velocity_ = new_velocity;
        current_key_ = new_pitch;
        outputCurrentKey();

        redraw();
    }
}

void UIKeyboard::showPopup(const t_pt& pt, const t_pt& abs_pt)
{
    UIPopupMenu menu(asEObj(), popup_menu_list_.front(), abs_pt, pt);

    velocity_ = std::min<int>(127, int(pt.y / height() * 100.f) + 27);
    current_key_ = findPressedKey(pt);
}

void UIKeyboard::playChord(const std::initializer_list<uint8_t>& keys)
{
    if (current_key_ == NO_KEY)
        current_key_ = 60 - prop_shift;

    for (auto k : keys) {
        auto kk = current_key_ + k;
        if (k < MAX_KEYS)
            active_keys_.set(kk);
    }

    redraw();
    for (auto k : keys) {
        t_float kk = current_key_ + k + prop_shift;
        Atom res[2] = { kk, velocity_ };
        listTo(0, AtomListView(res, 2));
    }
}

int UIKeyboard::findPressedKey(const t_pt& pt) const
{
    int res = -1;

    if (prop_vertical) {
        const auto nwk = keyboard_num_white_keys(prop_keys);
        const float wkh = height() / nwk;
        const float pty = height() - pt.y;

        const int wki = std::floor(pty / wkh);
        const int key = music::keyboard::wkey_to_key<int>(wki);

        const float bkw = 0.6 * width();

        if (pt.x < bkw) {
            const float kstep = 0.5 * wkh;
            const float koff0 = 0.2 * wkh;
            const float koff1 = 0.6 * wkh;

            // check nearest black keys
            for (int i = -2; i < 2; i++) {
                int idx = key + i;

                if (music::keyboard::is_black_key(idx)) {
                    int kn = idx % 12;
                    int koct = idx / 12;
                    float y0 = (idx * kstep + ((kn > 4 ? 1 : 0) + 2 * koct) * kstep + koff0);
                    float y1 = y0 + koff1;

                    if (pty >= y0 && pty < y1)
                        return idx;
                }
            }
        }

        return key;

    } else {

        const float black_key_w = width() / (keyboard_num_white_keys(prop_keys) * 2.f);
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

        if (res >= prop_keys)
            return prop_keys - 1;
    }

    return res;
}

void UIKeyboard::onList(const AtomListView& lv)
{
    const bool ok = lv.size() == 2
        && lv[0].isInteger()
        && lv[1].isFloat();

    if (!ok) {
        UI_ERR << "usage: NOTE VEL";
        return;
    }

    auto note = lv[0].asInt();
    auto vel = lv[1].asInt();

    if (note < 0 || note > 127) {
        UI_ERR << "note in 0..127 range expected, got: " << lv[0];
        return;
    }

    if (vel < 0 || vel > 127) {
        UI_ERR << "velocity in 0..127 range expected, got: " << lv[1];
        return;
    }

    auto key = note - prop_shift;
    if (key >= 0 && key < MAX_KEYS)
        active_keys_.set(key, vel > 0);

    current_key_ = NO_KEY;
    redraw();
    listTo(0, lv);
}

const char* UIKeyboard::annotateInlet(int n) const
{
    return "list: pitch velocity";
}

const char* UIKeyboard::annotateOutlet(int n) const
{
    return "list: pitch velocity";
}

int UIKeyboard::realPitch() const
{
    return (prop_shift + current_key_);
}

void UIKeyboard::outputCurrentKey()
{
    if (current_key_ == NO_KEY)
        return;

    Atom res[2];
    res[0] = realPitch();
    res[1] = velocity_;
    listTo(0, AtomListView(res, 2));
}

void UIKeyboard::setup()
{
    sys_gui(ui_keyboard_tcl);

    SYM_VKEYBOARD = gensym("ui.vkeyboard");
    SYM_VKEYBOARD2 = gensym("ui.vk");
    SYM_PROP_KEYS = gensym("@keys");

    UIObjectFactory<UIKeyboard> obj("ui.keyboard");
    obj.addAlias("ui.hk");
    obj.addAlias(SYM_VKEYBOARD->s_name);
    obj.addAlias(SYM_VKEYBOARD2->s_name);

    obj.hideLabelInner();
    obj.useAnnotations();
    obj.useBang();
    obj.useList();
    obj.usePopup();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG
        | UI_MOUSE_MOVE | UI_MOUSE_LEAVE
        | UI_MOUSE_WHEEL | UI_MOUSE_UP);

    obj.addProperty("active_color", _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIKeyboard::prop_color_active_);
    obj.hideProperty(PROP_BACKGROUND_COLOR);

    obj.addProperty("keys", _("Keys"), 61, &UIKeyboard::prop_keys, _("Main"));
    obj.setDefaultSize(433, DEFAULT_HEIGHT);
    obj.setPropertyRange("keys", MIN_KEYS, MAX_KEYS);

    obj.addProperty("shift", _("Leftmost MIDI note"), 36, &UIKeyboard::prop_shift, _("Main"));
    obj.setPropertyRange("shift", 6, MAX_KEYS);
}

void setup_ui_keyboard()
{
    UIKeyboard::setup();
}
