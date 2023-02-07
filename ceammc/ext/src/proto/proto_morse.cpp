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
#include "proto_morse.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_string_split.h"

namespace {
#include "../conv/gperf_morse.h"
}

ProtoMorse::ProtoMorse(const PdArgs& args)
    : BaseObject(args)
    , dot_space_(nullptr)
    , word_space_(nullptr)
    , char_space_(nullptr)
    , ext_clock_(nullptr)
    , clock_([this]() {
        if (ext_clock_->value())
            return;

        if (popBit())
            clock_.delay(dot_space_->value());
    })
    , last_bit_(0)
{
    dot_space_ = new FloatProperty("@dot", 100);
    dot_space_->checkClosedRange(5, 1000);
    dot_space_->setArgIndex(0);
    addProperty(dot_space_);

    word_space_ = new IntProperty("@word", 5);
    word_space_->checkClosedRange(2, 20);
    addProperty(word_space_);

    char_space_ = new IntProperty("@letter", 3);
    char_space_->checkClosedRange(2, 20);
    addProperty(char_space_);

    ext_clock_ = new BoolProperty("@external", false);
    addProperty(ext_clock_);

    createInlet();
    createOutlet();
}

void ProtoMorse::onSymbol(t_symbol* s)
{
    processWord(s->s_name);
}

void ProtoMorse::onList(const AtomListView& lv)
{
    for (auto& a : lv) {
        if (a.isSymbol()) {
            processWord(a.asT<t_symbol*>()->s_name);
        } else {
            auto str = to_string(a);
            processWord(str.c_str());
        }
    }
}

void ProtoMorse::onInlet(size_t n, const AtomListView& lv)
{
    popBit();
}

void ProtoMorse::m_reset(t_symbol* s, const AtomListView& lv)
{
    clock_.unset();
    queue_.clear();
}

void ProtoMorse::processWord(const char* str)
{
    string::StringSplitResult chars;
    string::split_every_char(chars, str);

    std::vector<std::uint8_t> code;
    code.reserve(64);

    constexpr int HIGH = 1;
    constexpr int LOW = 0;

    for (size_t i = 0; i < chars.size(); i++) {
        auto c = chars[i];
        auto hash = MorsePerfectHash::in_word_set(c.data(), c.length());

        if (!hash)
            continue;

        // output char
        for (std::uint8_t j = 0; j < hash->code.length; j++) {
            auto c = hash->code.at(j);
            if (c == 1) {
                code.push_back(HIGH);
                code.push_back(HIGH);
                code.push_back(HIGH);
            } else if (c == 0)
                code.push_back(HIGH);
            else
                continue;

            // dot space
            if (j + 1 < hash->code.length)
                code.push_back(LOW);
        }

        // char space
        if (i + 1 < chars.size()) {
            for (int i = 0; i < char_space_->value(); i++)
                code.push_back(LOW);
        }
    }

    for (int i = 0; i < word_space_->value(); i++)
        code.push_back(LOW);

    if (queue_.empty() && !ext_clock_->value())
        clock_.delay(0);

    std::copy(code.begin(), code.end(), std::back_inserter(queue_));
}

bool ProtoMorse::popBit()
{
    if (queue_.empty()) {
        last_bit_ = 0;
        return false;
    }

    auto bit = queue_.front();
    queue_.pop_front();

    if (last_bit_ != bit) {
        floatTo(0, bit);
        last_bit_ = bit;
    }

    return true;
}

void setup_proto_morse()
{
    ObjectFactory<ProtoMorse> obj("proto.morse");
}
