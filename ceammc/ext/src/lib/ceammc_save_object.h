/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_SAVE_OBJECT_H
#define CEAMMC_SAVE_OBJECT_H

#include "ceammc_factory.h"
#include "ceammc_object.h"

namespace ceammc {

template <typename T>
class SaveObject : public T {
public:
    SaveObject(const PdArgs& args)
        : T(args)
    {
        bindLoader();
    }

    void saveTextObject(t_binbuf* b)
    {
        auto x = this->owner();

        binbuf_addv(b, "ssii", &s__X, gensym("obj"),
            (int)x->te_xpix, (int)x->te_ypix);
        binbuf_addbinbuf(b, x->te_binbuf);
        binbuf_addsemi(b);
    }

    void saveTextObjectWidth(t_binbuf* b)
    {
        auto x = this->owner();
        obj_saveformat(x, b);
    }

    void saveUserRecord(t_binbuf* b, t_symbol* restoreSym, const AtomListView& lv)
    {
        binbuf_addv(b, "ss", gensym("#A"), restoreSym);
        binbuf_add(b, lv.size(), &lv.front().atom());
        binbuf_addsemi(b);
    }

    virtual void saveUser(t_binbuf* b) = 0;

    virtual void onSave(t_binbuf* b)
    {
        saveTextObject(b);
        saveUser(b);
        saveTextObjectWidth(b);
    }

    virtual void onRestore(const AtomListView& lv) = 0;

    void m_restore(t_symbol* /*s*/, const AtomListView& lv) { this->onRestore(lv); }

public:
    static constexpr const char* restoreSymbol = ".restore";

private:
    void bindLoader()
    {
        auto asym = gensym("#A");
        asym->s_thing = nullptr;
        pd_bind(&this->owner()->te_g.g_pd, asym);
    }
};

template <template <typename> class Factory, typename T>
class SaveObjectFactory : public Factory<T> {
public:
    using ObjectProxy = typename Factory<T>::ObjectProxy;

public:
    SaveObjectFactory(const char* name, uint32_t flags = OBJECT_FACTORY_DEFAULT)
        : Factory<T>(name, flags)
    {
        this->addMethod(T::restoreSymbol, &T::m_restore);
        class_setsavefn(this->classPointer(), (t_savefn)processSave);
    }

    static void processSave(ObjectProxy* x, t_binbuf* b)
    {
        x->impl->onSave(b);
    }
};

}

#endif // CEAMMC_SAVE_OBJECT_H
