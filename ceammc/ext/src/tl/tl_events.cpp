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
#include "tl_events.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "lex/parser_timeline.h"

TlEvents::TlEvents(const PdArgs& args)
    : TlEventsBase(args)
{
}

void TlEvents::onCueEnter()
{
}

void TlEvents::onCueLeave()
{
}

void TlEvents::saveUser(t_binbuf* b)
{
    auto symA = gensym(sym_A);
    auto symR = gensym(sym_restore);

    for (auto& l : src_) {
        if (l.empty())
            continue;

        binbuf_addv(b, "ss", symA, symR);
        binbuf_add(b, l.size(), &l.front().atom());
        binbuf_addsemi(b);
    }

    binbuf_addv(b, "ss", symA, symR);
    binbuf_addsemi(b);
}

void TlEvents::onRestore(const AtomListView& lv)
{
    if (lv.empty()) {
        update();
    } else {
        src_.push_back({});
        auto& back = src_.back();

        for (auto& a : lv)
            back.push_back(a);
    }
}

void TlEvents::editorClear()
{
    src_.clear();
}

void TlEvents::editorAddLine(t_symbol* sel, const AtomListView& lv)
{
    src_.push_back({});
    auto& b = src_.back();

    for (auto& a : lv)
        b.push_back(a);
}

EditorLineList TlEvents::getContentForEditor() const
{
    EditorLineList res;
    for (auto& l : src_) {
        if (l.empty())
            continue;

        auto str = EditorStringPool::pool().allocate();
        str->append(l.view());
        res.push_back(str);
    }

    return res;
}

void TlEvents::editorSync()
{
    TlEventsBase::editorSync();
    update();
}

void TlEvents::update()
{
    std::string txt(80, ' ');
    tl_.clear();

    for (auto& line : src_) {
        txt.clear();
        for (auto& a : line) {
            txt += to_string(a);
            txt += ' ';
        }

        if (txt.size() > 0 && txt.back() == ' ')
            txt.pop_back();

        if (!parser::parse_timelime(txt.c_str(), tl_)) {
            OBJ_ERR << "can't parse timeline: \"" << txt << '"';
            tl_.clear();
            return;
        }
    }

    tl_.dump();
    for (auto& ev : tl_.events) {
    }
}

void setup_tl_events()
{
    ObjectFactory<TlEvents> obj("tl.events");
    TlEvents::setDisplaceFn(obj.classPointer());

    TlEvents::factoryEditorObjectInit(obj);
    TlEvents::factorySaveObjectInit(obj);
}
