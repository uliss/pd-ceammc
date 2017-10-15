/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef DATA_LIST_H
#define DATA_LIST_H

#include "ceammc_object.h"

using namespace ceammc;

class DataList : public BaseObject {
    AtomList list_;

public:
    DataList(const PdArgs& a);

    /** pd */
    void onFloat(float f);
    void onList(const AtomList& l);
    void onBang();
    void onInlet(size_t n, const AtomList& l);

    /** methods */
    void m_clear(t_symbol*, const AtomList&);
    void m_append(t_symbol*, const AtomList& l);
    void m_set(t_symbol*, const AtomList& l);
    void m_remove(t_symbol* s, const AtomList& l);
    void m_pop(t_symbol* s, const AtomList&);
    void m_fill(t_symbol*, const AtomList& l);
    void m_insert(t_symbol* s, const AtomList& l);
    void m_flush(t_symbol* s, const AtomList& l);

    void dump() const;

private:
    AtomList getSize() const;
};

#endif // DATA_LIST_H
