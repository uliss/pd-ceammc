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

#include "ceammc_object.h"
#include "m_pd.h"

namespace ceammc {

class TclPropDialogGenerator {
    const BaseObject* obj_ { nullptr };

public:
    explicit TclPropDialogGenerator(const BaseObject* obj);

    std::string generate() const;
    std::string procName() const;
    std::string procArgs() const;
    std::string procBody() const;
    std::string procBodyInit() const;
    std::string callProc() const;
    std::string buttons(int row) const;

    static std::string entryInt(int row, t_int value, const PropertyInfo& info);
    static std::string entryFloat(int row, t_float value, const PropertyInfo& info);
    std::string entryBool(int row, bool value, const PropertyInfo& info) const;
    std::string propVar(int row, t_symbol* name) const;
    std::string checkbox(int row) const;
    static std::string spinbox(int row, const PropertyInfo& info);
    static std::string propLabel(int row, const std::string& text);
    static std::string widgetId(int row);
    static std::string widgetState(int row, PropValueAccess state);
    static std::string grid(int row, int col, const std::string& widget, const char* sticky);
    static std::string unitsLabel(int row, const PropertyInfo& info);

private:
    std::string propVarName(int row) const;
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

public:
    template <typename Factory>
    static void factoryPropertiesObjectInit(Factory& f)
    {
        class_setpropertiesfn(f.classPointer(), &processPropDialog<Factory>);
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
