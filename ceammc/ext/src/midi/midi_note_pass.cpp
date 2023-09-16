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
#include "midi_note_pass.h"
#include "args/argcheck2.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_music_chord.h"
#include "ceammc_music_scale.h"
#include "ceammc_music_theory_keyboard.h"
#include "fmt/core.h"
#include "lex/parser_music.h"
#include "lex/ragel_music.h"
#include "muParser.h"

#include <cctype>

using MuParserFn = double (*)(double);
using MuParserFn2 = double (*)(double, double);

MidiNotePass::MidiNotePass(const PdArgs& args)
    : BaseObject(args)
    , expr_(new mu::Parser)
{
    try {
        expr_->DefineNameChars("#$0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ:");
        expr_->DefineVar("$N", &expr_note_);
        expr_->DefineVar("$V", &expr_vel_);

        expr_->DefineConst("Cb", 11);
        expr_->DefineConst("C", 0);
        expr_->DefineConst("C#", 1);
        expr_->DefineConst("Db", 1);
        expr_->DefineConst("D", 2);
        expr_->DefineConst("D#", 3);
        expr_->DefineConst("Eb", 3);
        expr_->DefineConst("E", 4);
        expr_->DefineConst("E#", 5);
        expr_->DefineConst("Fb", 4);
        expr_->DefineConst("F", 5);
        expr_->DefineConst("F#", 6);
        expr_->DefineConst("Gb", 6);
        expr_->DefineConst("G", 7);
        expr_->DefineConst("G#", 8);
        expr_->DefineConst("Ab", 8);
        expr_->DefineConst("A", 9);
        expr_->DefineConst("A#", 10);
        expr_->DefineConst("Bb", 10);
        expr_->DefineConst("B", 11);
        expr_->DefineConst("B#", 12);

        for (auto& s : music::ScaleLibrary::instance().all())
            expr_->DefineConst(s->name(), crc32_hash(s->name()));

        expr_->DefineFun("black", static_cast<MuParserFn>([](double note) -> double {
            return music::keyboard::is_black_key<int>(note);
        }));
        expr_->DefineFun("white", static_cast<MuParserFn>([](double note) -> double {
            return music::keyboard::is_white_key<int>(note);
        }));
        expr_->DefineFun("oct", static_cast<MuParserFn>([](double note) -> double {
            return (int(note) / 12) - 1;
        }));
        expr_->DefineFun("oct", static_cast<MuParserFn>([](double note) -> double {
            return (int(note) / 12) - 1;
        }));
        expr_->DefineFun("print", static_cast<MuParserFn>([](double value) -> double {
            LIB_POST << fmt::format("[midi.note.pass] print: {}", value);
            return 1;
        }));
        expr_->DefineFunUserData(
            "scale", [](void* data, double base, double scale_hash) -> double {
                const MidiNotePass* x = static_cast<const MidiNotePass*>(data);
                if (!x)
                    return 0;

                auto scale = music::ScaleLibrary::instance().findByHash(scale_hash);
                if (!scale) {
                    LIB_ERR << "scale not found";
                    return 0;
                } else {
                    auto degree = static_cast<music::DegreeType>(24 + x->currentNote() - base) % 12;
                    return scale->find(degree);
                }
            },
            this, false);
        expr_->DefineFunUserData(
            "chord", [](void* data, const char* chord) -> double {
                const MidiNotePass* x = static_cast<const MidiNotePass*>(data);
                if (!x)
                    return 0;

                music::ChordClass ch;
                if (!parser::parse_chord_class(chord, ch)) {
                    LIB_ERR << fmt::format("invaldi chord: '{}'", chord);
                    return 0;
                }

                auto degree = static_cast<music::DegreeType>(24 + x->currentNote() - ch.basePitch().asMidi()) % 12;
                return ch.type().contains(degree);
            },
            this, false);

    } catch (mu::ParserError& e) {
        OBJ_ERR << e.GetMsg();
    }

    prop_expr_ = new ListProperty("@expr");
    prop_expr_->setArgIndex(0);
    prop_expr_->setListCheckFn([this](const AtomListView& lv) -> bool {
        try {
            expr_->SetExpr(to_string(lv, ""));
            return true;
        } catch (mu::Parser::exception_type& e) {
            OBJ_ERR << fmt::format("error '{}' in expression: '{}'", e.GetMsg(), e.GetExpr());
            return false;
        }
    });
    addProperty(prop_expr_);

    createInlet();
    createOutlet();
}

void MidiNotePass::onList(const AtomListView& lv)
{
    static const args::ArgChecker chk("NOTE:i[0,127] VEL:i[0,127] DUR:f>0?");

    if (!chk.check(lv, this))
        return chk.usage(this);

    expr_note_ = lv.intAt(0, 0);
    expr_vel_ = lv.intAt(1, 0);

    try {
        const auto res = expr_->Eval();
        if (res != 0)
            listTo(0, lv);
    } catch (mu::Parser::exception_type& e) {
        OBJ_ERR << fmt::format("error '{}' in expression: '{}'", e.GetMsg(), e.GetExpr());
    }
}

void MidiNotePass::onInlet(size_t n, const AtomListView& lv)
{
    prop_expr_->set(lv);
}

void MidiNotePass::dump() const
{
    BaseObject::dump();

    Post post(this);
    post << "const:\n";

    // notes
    for (auto& kv : expr_->GetConst()) {
        auto& name = kv.first;
        auto ch = name[0];
        if (!::isupper(ch))
            break;

        post << kv.first << ' ';
    }
    post.flush();

    // scales
    post << "scales:\n";
    {
        int cnt = 1;
        for (auto& kv : expr_->GetConst()) {
            auto& name = kv.first;
            auto ch = name[0];
            if (!::islower(ch))
                continue;

            post << kv.first << ' ';
            if ((cnt++ % 4) == 0)
                post.flush();
        }
    }

    post.flush();

    // chords
    post << "chords: ";
    {
        int cnt = 1;
        for (auto c : ragel::chord_suffix_list()) {
            post << 'C' << c << ' ';
            if ((cnt++ % 4) == 0)
                post.flush();
        }
    }

    post.flush();
    post << "vars: ";
    for (auto& kv : expr_->GetVar())
        post << kv.first << ' ';

    post.flush();
    {
        post << "functions:\n";
        int cnt = 1;
        for (auto& fn : expr_->GetFunDef()) {
            post << fn.first << "()";
            post << ((cnt++ % 8) ? ' ' : '\n');
        }
    }

    post.flush();
}

void setup_midi_note_pass()
{
    LIB_DBG << fmt::format("muparser: {}", mu::Parser().GetVersion(mu::pviBRIEF));

    ObjectFactory<MidiNotePass> obj("midi.note.pass");
    obj.parseArgsMode(PdArgs::PARSE_COPY);
    obj.parsePropsMode(PdArgs::PARSE_COPY);
    obj.addAlias("note.pass");

    obj.setXletsInfo({ "list: NOTE VEL DUR?", "list: set new expression" }, { "list: NOTE VEL DUR?" });
}
