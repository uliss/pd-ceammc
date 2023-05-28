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
#ifndef DATA_BIMAP_H
#define DATA_BIMAP_H

#include "ceammc_editor_object.h"
#include "ceammc_object.h"
#include "data_protocol.h"

#include <boost/bimap.hpp>

using namespace ceammc;

using BiMap = boost::bimap<Atom, Atom>;
using DataBiMapFs = EditorObject<FilesystemIFace<BaseObject>>; // read/write methods

class DataBiMap : public DataBiMapFs {
    BiMap bimap_;
    ListProperty* values_ { 0 };

public:
    DataBiMap(const PdArgs& args);

    void onFloat(t_float v) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomListView& lv) final;
    void onInlet(size_t idx, const AtomListView& lv) final;

    bool proto_write(const std::string& path) const final;
    bool proto_read(const std::string& path) final;

    // editor
    void editorAddLine(t_symbol* sel, const AtomListView& lv) final;
    void editorClear() final;
    int calcEditorLines() const final;
    int calcEditorChars() const final;
    EditorTitleString editorTitle() const final { return "DATA.BIMAP"; }
    EditorLineList getContentForEditor() const final;

    void m_append(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);

public:
    template <template <typename T> class Factory, typename T>
    static void registerMethods(Factory<T>& obj)
    {
        EditorObject::registerMethods(obj);
        obj.addMethod("append", &T::m_append);
        obj.addMethod("clear", &T::m_clear);

        protocol::ReaderWriter<Factory, T> rw(obj);
    }

private:
    bool check(const AtomListView& lv) const;
    void sync(bool printErr = true);
};

void setup_data_bimap();

#endif // DATA_BIMAP_H
