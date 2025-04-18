/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_PROPERTIES_OBJECT_H
#define CEAMMC_PROPERTIES_OBJECT_H

#include "ceammc_fn_list.h"
#include "ceammc_object.h"
#include "m_pd.h"

namespace ceammc {

class TclPropDialogGenerator {
    const BaseObject* obj_ { nullptr };

public:
    enum EntryType {
        Widget,
        Button,
        Reset,
        Default,
        SetProc,
        SetProcName,
    };

    enum class VarType {
        Current,
        Init,
        Default,
    };

public:
    explicit TclPropDialogGenerator(const BaseObject* obj);

    void prepare() const;

    std::string generate() const;
    std::string callProc() const;
    std::string procBody() const;
    std::string procBodyInit() const;
    std::string buttons(int row) const;
    std::string setPropProcs() const;

    std::string makeClassName() const;
    void foreachProperty(const std::function<void(const char* name, int row, const Property* p)>& cb) const;

    static std::string procName(const char* className);
    static std::string okProcName(const char* className);
    static std::string validateProcName(const char* className);
    static std::string propVarName(int propId);

    static std::string propGetValue(const char* propName, VarType vtype);
    static std::string propSetValue(const char* propName, const char* value, VarType vtype);

    static std::string normPropName(const char* propName);
    static std::string propSetProcName(const char* propName, const char* className);
    static std::string propSetProcBody(int propId, const PropertyInfo& info);
    static std::string propResetCall(int propId, const char* propName, const char* className);
    static std::string propDefaultCall(int propId, const char* propName, const char* className);

    static std::string actionButtonId(int propId);
    static std::string resetButton(int propId, const char* propName, const char* className);
    static std::string resetButtonId(int propId);
    static std::string defaultButton(int propId, const char* propName, const char* className);
    static std::string defaultButtonId(int propId);

    static bool hasResetButton(const PropertyInfo& info);
    static bool hasDefaultButton(const PropertyInfo& info);

    static std::string entryBool(int propId, const PropertyInfo& info);
    static std::string entryFloat(int propId, const PropertyInfo& info);
    static std::string entryInt(int propId, const PropertyInfo& info);
    static std::string entrySymbol(int propId, const PropertyInfo& info);
    static std::string entryAtom(int propId, const PropertyInfo& info);

    static std::string checkbox(int propId, const PropertyInfo& info);
    static std::string colorentry(int propId, const PropertyInfo& info);
    static std::string combobox(int propId, const PropertyInfo& info);
    static std::string spinbox(int propId, const PropertyInfo& info);
    static std::string textentry(int propId, const PropertyInfo& info);
    static std::string pathentry(int propId, const PropertyInfo& info);

    static std::string propsDictVar(VarType vt);
    static std::string widgetId(int propId);
    static std::string widgetState(int propId, PropValueAccess state);
    static std::string grid(int row, int col, const std::string& widget, const char* sticky);

    static std::string propLabel(int propId, const std::string& text);
    static std::string unitsLabel(int propId, const PropertyInfo& info);
    static std::string propType(const PropertyInfo& info);

    static std::string list2tcl(const AtomListView& lv);
    static std::string atom2tcl(const Atom& a);

    static bool setList(Property* p, const AtomListView& lv);
};

template <typename T>
class PropertiesObject : public T {
    using PropertiesObjectT = PropertiesObject<T>;

    TclPropDialogGenerator dialog_gen_;

public:
    explicit PropertiesObject(const PdArgs& args)
        : T(args)
        , dialog_gen_(this)
    {
    }

    ~PropertiesObject()
    {
        gfxstub_deleteforkey(this);
    }

    virtual void onPropDialog(t_glist* glist)
    {
        sys_vgui("%s\n", dialog_gen_.generate().c_str());
        gfxstub_new(this->asPd(), this, callDialogProc().c_str());
    }

    std::string generateDialogProc() const
    {
        return dialog_gen_.generate();
    }

    std::string callDialogProc() const
    {
        return dialog_gen_.callProc();
    }

    void m_prop_set(t_symbol* s, const AtomListView& lv)
    {
        list::foreachProperty(lv, [this, s](t_symbol* key, const AtomListView& lv) {
            Property* prop = this->property(key);
            if (!prop) {
                METHOD_ERR(s) << "property not found: " << key;
            } else {
                dialog_gen_.setList(prop, lv);
            }
        });
    }

    void m_prop_validate(t_symbol* s, const AtomListView& lv)
    {
        if (lv.size() < 3) {
            METHOD_ERR(s) << "invalid arguments";
            return;
        }

        auto id = lv.symbolAt(0, &s_);
        auto wid = lv.symbolAt(1, &s_);
        auto prop = lv.symbolAt(2, &s_);
        auto args = lv.subView(3);

        Property* p = this->property(prop);
        if (!p) {
            METHOD_ERR(s) << "property not found: " << prop->s_name;
            return;
        }

        auto old_value = p->get();
        if (p->set(args)) {
            // restore
            p->set(old_value);
            METHOD_DBG(s) << "OK";
        } else {
            METHOD_ERR(s) << "invalid args: " << args;
            // sys_vgui("dict set %s %s %s", wid->s_name, prop->s_name, atom_string());
        }
    }

public:
    template <typename Factory>
    static void factoryPropertiesObjectInit(Factory& f)
    {
        class_setpropertiesfn(f.classPointer(), &processPropDialog<Factory>);

        f.addMethod(".prop_set", &PropertiesObject<T>::m_prop_set);
        f.addMethod(".prop_validate", &PropertiesObject<T>::m_prop_validate);
    }

    template <typename Factory>
    static void processPropDialog(t_gobj* x, t_glist* glist)
    {
        using ObjectProxy = typename Factory::ObjectProxy;
        auto proxy = reinterpret_cast<ObjectProxy*>(x);
        proxy->impl->onPropDialog(glist);
    }
};

} // namespace ceammc

#endif // CEAMMC_PROPERTIES_OBJECT_H
