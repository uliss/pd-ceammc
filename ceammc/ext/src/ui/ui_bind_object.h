/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef UI_BIND_OBJECT_H
#define UI_BIND_OBJECT_H

#include "ceammc_proxy.h"
#include "ceammc_ui_object.h"
#include "lex/parser_ui_bind.h"

#include <boost/container/static_vector.hpp>
#include <functional>

namespace ceammc {

constexpr const char* UI_BIND_CTLIN = "#ctlin";
constexpr const char* UI_BIND_PGMIN = "#pgmin";
constexpr const char* UI_BIND_NOTEIN = "#notein";

using UIBindObjectFn = std::function<void()>;

template <size_t N = 1>
class UIBindObject : public UIObject {
    using MidiProxy = PdListProxy<UIBindObject>;
    using ProxyArray = boost::container::static_vector<MidiProxy, N>;

    AtomList values_[N];
    UIBindOptions opts_[N];
    UIBindObjectFn callbacks_[N];
    ProxyArray midi_proxy_;

protected:
    template <size_t IDX>
    AtomList getBind() const { return values_[IDX]; }

    template <size_t IDX>
    void setBind(const AtomListView& lv)
    {
        values_[IDX].clear();
        midi_proxy_[IDX].unbind();
        opts_[IDX].reset();

        for (auto& a : lv) {
            if (!a.isSymbol())
                continue;

            auto sym = a.asT<t_symbol*>();

            if (parser::parse_ui_bind(sym->s_name, opts_[IDX])) {
                values_[IDX].append(a);

                switch (opts_[IDX].type) {
                case ceammc::UI_BIND_MIDI_CC:
                    midi_proxy_[IDX].bind(UI_BIND_CTLIN);
                    break;
                case ceammc::UI_BIND_MIDI_PGM:
                    midi_proxy_[IDX].bind(UI_BIND_PGMIN);
                    break;
                case ceammc::UI_BIND_MIDI_NOTE:
                    midi_proxy_[IDX].bind(UI_BIND_NOTEIN);
                    break;
                default:
                    break;
                }
            }
        }
    }

    void call(size_t idx)
    {
        if (callbacks_[idx])
            callbacks_[idx]();
    }

    template <size_t IDX>
    void initMidiProxy()
    {
        if (IDX > 0)
            initMidiProxy<IDX - 1>();

        midi_proxy_.emplace_back(this, &UIBindObject::onMidiBind<IDX - 1>);
    }

    template <>
    void initMidiProxy<0>()
    {
    }

public:
    UIBindObject(std::initializer_list<UIBindObjectFn> cb)
    {
        std::copy(cb.begin(), cb.end(), callbacks_);
        initMidiProxy<N>();
    }

    template <size_t IDX>
    void onMidiBind(const AtomListView& lv)
    {
        switch (opts_[IDX].type) {
        case UI_BIND_MIDI_NOTE:
        case UI_BIND_MIDI_CC: {
            auto cc = lv.intAt(0, 0);
            auto val = lv.intAt(1, 0);
            auto chan = lv.intAt(2, 0);

            if (opts_[IDX].checkMidi(chan, cc, val))
                call(IDX);

        } break;
        case UI_BIND_MIDI_PGM: {
            auto prog = lv.intAt(0, 0);
            auto chan = lv.intAt(1, 0);

            if (opts_[IDX].checkMidi(chan, prog, 0))
                call(IDX);

        } break;
        default:
            break;
        }
    }
};
}

#endif // UI_BIND_OBJECT_H
