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
#include "ceammc_crc32.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"
#include "kbd_names.h"
#include "kbd_querty.h"

#include <boost/container/small_vector.hpp>

#include <codecvt>
#include <limits>
#include <locale>

constexpr auto KEY_NOT_FOUND = std::numeric_limits<int>::max();

CEAMMC_DEFINE_SYM_HASH(qwerty);
CEAMMC_DEFINE_SYM_HASH(names);
CEAMMC_DEFINE_SYM_HASH(custom);

constexpr const char* STR_KEYUP = "#keyup";
constexpr const char* STR_KEYDOWN = "#key";

static inline std::string int_to_utf8(char32_t v)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(v);
}

static inline char32_t utf8_to_int(const char* key)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    auto res = convert.from_bytes(key);
    return res.empty() ? 0 : res[0];
}

MidiKbd::MidiKbd(const PdArgs& args)
    : BaseObject(args)
    , keypress_(this, &MidiKbd::onKeyPress)
    , keyrelease_(this, &MidiKbd::onKeyRelease)
    , octave_(nullptr)
    , vel_(nullptr)
    , layout_(nullptr)
    , on_(nullptr)
    , kbd_(&kbd_querty)
{
    keypress_.bind(gensym(STR_KEYDOWN));
    keyrelease_.bind(gensym(STR_KEYUP));

    octave_ = new IntProperty("@octave", 4);
    octave_->checkClosedRange(0, 8);
    addProperty(octave_);

    vel_ = new FloatProperty("@vel", 100);
    vel_->checkClosedRange(0, 127);
    addProperty(vel_);

    layout_ = new SymbolEnumProperty("@layout", { sym_qwerty(), sym_names(), sym_custom() });
    layout_->setSuccessFn([this](Property*) {
        switch (crc32_hash(layout_->value())) {
        case hash_qwerty:
            kbd_ = &kbd_querty;
            break;
        case hash_names:
            kbd_ = &kbd_names;
            break;
        case hash_custom:
            kbd_ = &custom_;
            break;
        default:
            OBJ_ERR << "unknown layout type: " << layout_->value()->s_name;
        }
    });
    layout_->setArgIndex(0);
    addProperty(layout_);

    on_ = new BoolProperty("@on", true);
    on_->setArgIndex(1);
    addProperty(on_);

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
                    skey = gensym(int_to_utf8(k).c_str());
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
                    if (kv.second.isInteger()) {
                        auto note = kv.second[0].asT<int>();
                        custom_[utf8_to_int(kv.first->s_name)] = note;
                    }
                }

                return true;
            }
        });

    createInlet();
    createOutlet();
}

void MidiKbd::dump() const
{
    BaseObject::dump();

    const size_t N = kbd_->size();
    if (N < 1)
        return;

    using KV = std::pair<uint32_t, int8_t>;
    boost::container::small_vector<KV, 32> keys;
    keys.reserve(N);

    for (auto& kv : *kbd_)
        keys.push_back(kv);

    std::sort(keys.begin(), keys.end(), [](const KV& a, const KV& b) { return a.second < b.second; });
    std::string keyname;
    for (auto& kv : keys) {
        auto k = kv.first;
        if (k < 127)
            keyname = char(k);
        else
            keyname = int_to_utf8(k);

        OBJ_POST << '"' << keyname << "\" -> " << midiNote(kv.second);
    }
}

void MidiKbd::onInlet(size_t n, const AtomListView& lv)
{
    on_->set(lv);
}

void MidiKbd::m_custom(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, s))
        return;

    custom_.clear();
    auto sym = lv[0].asT<t_symbol*>();
    if (sym) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        int i = 0;
        for (auto c : convert.from_bytes(sym->s_name))
            custom_[c] = i++;
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
    if (!on_->value())
        return;

    auto note = findKey(key);
    if (note == KEY_NOT_FOUND)
        return;

    Atom res[2] = { midiNote(note), vel_->value() };
    listTo(0, AtomListView(res, 2));
}

void MidiKbd::onKeyRelease(t_float key)
{
    if (!on_->value())
        return;

    auto note = findKey(key);
    if (note == KEY_NOT_FOUND)
        return;

    Atom res[2] = { midiNote(note), 0. };
    listTo(0, AtomListView(res, 2));
}

void setup_midi_kbd()
{
    ObjectFactory<MidiKbd> obj("midi.kbd");
    obj.addMethod("custom", &MidiKbd::m_custom);

    obj.setXletsInfo({ "", "int: 1/0 - on/off" }, { "list: NOTE VEL" });

    obj.setDescription("computer keyboard midi");
    obj.setCategory("midi");
    obj.setKeywords({"midi", "keyboard"});
}
