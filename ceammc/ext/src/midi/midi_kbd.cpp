/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "midi_kbd.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"
#include "kbd_names.h"
#include "kbd_querty.h"

#include <limits>

#include <codecvt>
#include <locale>

constexpr auto KEY_NOT_FOUND = std::numeric_limits<int>::max();

static t_symbol* SYM_KEYUP;
static t_symbol* SYM_KEYDOWN;
static t_symbol* SYM_QWERTY;
static t_symbol* SYM_NAMES;
static t_symbol* SYM_CUSTOM;

static inline std::string int2utf8(int v)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv1;
    return conv1.to_bytes(static_cast<uint32_t>(v));
}

MidiKbd::MidiKbd(const PdArgs& args)
    : BaseObject(args)
    , keypress_(this, &MidiKbd::onKeyPress)
    , keyrelease_(this, &MidiKbd::onKeyRelease)
    , octave_(nullptr)
    , vel_(nullptr)
    , layout_(nullptr)
    , kbd_(&kbd_querty)
{
    keypress_.bind(SYM_KEYDOWN);
    keyrelease_.bind(SYM_KEYUP);

    octave_ = new IntProperty("@octave", 4);
    octave_->checkClosedRange(0, 8);
    addProperty(octave_);

    vel_ = new FloatProperty("@vel", 100);
    vel_->checkClosedRange(0, 127);
    addProperty(vel_);

    layout_ = new SymbolEnumProperty("@layout", { SYM_QWERTY, SYM_NAMES, SYM_CUSTOM });
    layout_->setSuccessFn([this](Property*) {
        const auto l = layout_->value();
        if (l == SYM_QWERTY)
            kbd_ = &kbd_querty;
        else if (l == SYM_NAMES)
            kbd_ = &kbd_names;
        else if (l == SYM_CUSTOM)
            kbd_ = &custom_;
        else
            OBJ_ERR << "unknown layout type: " << l->s_name;
    });
    addProperty(layout_);

    createCbAtomProperty(
        "@custom",
        [this]() -> Atom {
            DictAtom dict;

            for (auto& kv : custom_) {
                auto k = kv.first;

                static_assert(std::is_unsigned<decltype(k)>::value, "");

                t_symbol* skey;
                if (k < 127) {
                    char buf[2] = { (char)k, 0 };
                    skey = gensym(buf);
                } else {
                    skey = gensym(int2utf8(k).c_str());
                }

                dict->insert(skey, kv.second);
            }

            return std::move(dict);
        },
        [this](const Atom& a) -> bool {
            auto pdict = a.asDataT<DataTypeDict>();
            if (!pdict) {
                OBJ_ERR << "Dict expected";
                return false;
            } else {
                custom_.clear();
                custom_.reserve(pdict->size());

                for (auto& kv : *pdict) {
                    auto s = kv.first->s_name;
                    if (s[0] != 0 && s[1] == 0 && kv.second.isInteger()) { // single ascii char
                        custom_[s[0]] = kv.second.asT<int>();
                    }
                }

                return true;
            }
        });

    createOutlet();
}

void MidiKbd::dump() const
{
    BaseObject::dump();

    const size_t N = kbd_->size();
    if (N < 1)
        return;

    using KV = std::pair<uint32_t, int8_t>;
    KV keys[N];
    int i = 0;
    for (auto& kv : *kbd_)
        keys[i++] = kv;

    std::sort(keys, keys + N, [](const KV& a, const KV& b) { return a.second < b.second; });
    std::string keyname;
    for (auto kv : keys) {
        auto k = kv.first;
        if (k < 127)
            keyname = char(k);
        else
            keyname = int2utf8(k);

        OBJ_POST << '"' << keyname << "\" -> " << midiNote(kv.second);
    }
}

int MidiKbd::findKey(int key) const
{
    if (!kbd_)
        return KEY_NOT_FOUND;

    auto it = kbd_->find(key);
    return (it == kbd_->end()) ? KEY_NOT_FOUND : it->second;
}

void MidiKbd::onKeyPress(t_float key)
{
    auto note = findKey(key);
    if (note == KEY_NOT_FOUND)
        return;

    Atom res[2] = { midiNote(note), vel_->value() };
    listTo(0, AtomListView(res, 2));
}

void MidiKbd::onKeyRelease(t_float key)
{
    auto note = findKey(key);
    if (note == KEY_NOT_FOUND)
        return;

    Atom res[2] = { midiNote(note), 0. };
    listTo(0, AtomListView(res, 2));
}

void setup_midi_kbd()
{
    SYM_KEYUP = gensym("#keyup");
    SYM_KEYDOWN = gensym("#key");

    SYM_QWERTY = gensym("qwerty");
    SYM_NAMES = gensym("names");
    SYM_CUSTOM = gensym("custom");

    ObjectFactory<MidiKbd> obj("midi.kbd");
}
