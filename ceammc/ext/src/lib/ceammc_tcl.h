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
#ifndef CEAMMC_TCL_H
#define CEAMMC_TCL_H

#include "ceammc_object.h"

namespace ceammc {

/**
 * @brief The BaseTclObject class
 */
template <typename T = BaseObject>
class BaseTclObject : public T {
    t_symbol* tcl_bind_ = &s_;
    t_symbol* proc_name_ = &s_;

public:
    BaseTclObject(const PdArgs& args, t_symbol* procName)
        : T(args)
        , proc_name_(procName)
    {
        char buf[64];
        sprintf(buf, "#%p", static_cast<void*>(this));
        tcl_bind_ = gensym(buf);
        pd_bind(&this->owner()->te_g.g_pd, tcl_bind_);
    }

    virtual ~BaseTclObject()
    {
        pd_unbind(&this->owner()->te_g.g_pd, tcl_bind_);
    }

    void tclCall() const
    {
        pdgui_vmess(proc_name_->s_name, "s", tcl_bind_->s_name);
    }

    void tclCall(t_float f) const
    {
        pdgui_vmess(proc_name_->s_name, "sf", tcl_bind_->s_name, f);
    }

    void tclCall(t_symbol* s) const
    {
        pdgui_vmess(proc_name_->s_name, "ss", tcl_bind_->s_name, s->s_name);
    }

    void tclCall(const AtomListView& lv) const
    {
        pdgui_vmess(proc_name_->s_name, "sA", tcl_bind_->s_name, static_cast<int>(lv.size()), lv.toPdData());
    }

    void tcl_callback(t_symbol* s, const AtomListView& lv)
    {
        this->onTclResponse(s, lv);
    }

    template <typename Factory>
    static void initMethods(Factory& f, const char* tclProc = "")
    {
        if (tclProc && tclProc[0])
            sys_gui(tclProc);

        f.addMethod(".tcl_callback", &BaseTclObject::tcl_callback);
    }

    t_symbol* tclBindName() const { return tcl_bind_; }

    virtual void onTclResponse(t_symbol* s, const AtomListView& lv) = 0;
};

}

#endif // CEAMMC_TCL_H
