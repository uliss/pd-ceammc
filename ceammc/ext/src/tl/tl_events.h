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
#ifndef TL_EVENTS_H
#define TL_EVENTS_H

#include "ceammc_containers.h"
#include "ceammc_editor_object.h"
#include "ceammc_save_object.h"
#include "tl_baseobject.h"

#include "lex/parser_timeline.h"

using TlEventsBase = EditorObject<SaveObject<TlBaseObject>>;

class TlEvents : public TlEventsBase {
    using FixedAtomList = SmallAtomListN<8>;
    using FixedEditorList = boost::container::small_vector<FixedAtomList, 48>;
    FixedEditorList src_;
    parser::TimeLine tl_;

public:
    TlEvents(const PdArgs& args);
    void tick();

    // tl fn
    void onCueEnter() final;
    void onCueLeave() final;

    // save fn
    void saveUser(t_binbuf* b) final;
    void onRestore(const AtomListView& lv) final;

    // editor fn
    void editorClear() final;
    void editorAddLine(t_symbol* sel, const AtomListView& lv) final;
    EditorLineList getContentForEditor() const final;
    void editorSync() final;

private:
    void update();
};

void setup_tl_events();

#endif // TL_EVENTS_H
