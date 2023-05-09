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
#include "ceammc_music_theory_keyboard.h"
#include "fmt/core.h"
#include "muParser.h"

using MuParserFn = double (*)(double);

MidiNotePass::MidiNotePass(const PdArgs& args)
    : BaseObject(args)
    , expr_(new mu::Parser)
{
    expr_->DefineNameChars("$0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    expr_->DefineVar("$n", &expr_note_);
    expr_->DefineVar("$v", &expr_vel_);
    expr_->DefineFun("is_black_key", static_cast<MuParserFn>([](double note) -> double {
        return music::keyboard::is_black_key<int>(note);
    }));

    prop_expr_ = new SymbolProperty("@expr", &s_);
    prop_expr_->setArgIndex(0);
    prop_expr_->setSymbolCheckFn([this](t_symbol* s) -> bool {
        try {
            expr_->SetExpr(prop_expr_->value()->s_name);
            return true;
        } catch (mu::Parser::exception_type& e) {
            OBJ_ERR << fmt::format("error '{}' in expression: '{}'", e.GetMsg(), e.GetExpr());
            return false;
        }
    });
    addProperty(prop_expr_);

    createOutlet();
}

void MidiNotePass::initDone()
{
    prop_expr_->set(prop_expr_->get());
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

void setup_midi_note_pass()
{
    ObjectFactory<MidiNotePass> obj("midi.note.pass");
    obj.addAlias("note.pass");
}
