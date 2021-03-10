/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_EXTERNALS_H
#define CEAMMC_EXTERNALS_H

#include "ceammc_property_info.h"
#include "m_pd.h"

#include <boost/optional.hpp>
#include <vector>

namespace ceammc {
class BaseObject;
class UIObject;

void register_flext_external(t_class* c);
void register_ui_external(t_class* c);

bool is_ceammc(t_object* x);
bool is_ceammc_base(t_object* x);
bool is_ceammc_ui(t_object* x);
bool is_ceammc_flext(t_object* x);
bool is_ceammc_abstraction(t_object* x);

const BaseObject* ceammc_to_base_object(t_object* x, bool check = true);
const UIObject* ceammc_to_ui_object(t_object* x, bool check = true);
std::vector<PropertyInfo> ceammc_base_properties(t_object* x);
std::vector<PropertyInfo> ceammc_ui_properties(t_object* x);
std::vector<PropertyInfo> ceammc_abstraction_properties(t_object* x);

enum class PropertySetState {
    OK,
    OTHER_TYPE,
    ERROR_NOT_FOUND,
    ERROR_INVALID_TYPE,
    ERROR_NO_RANGE,
    ERROR_SET_VALUE,
    ERROR_ACCESS
};

PropertySetState ceammc_base_property_set_cc(t_object* x, t_symbol* key, t_float val, bool check = true);
PropertySetState ceammc_ui_property_set_cc(t_object* x, t_symbol* key, t_float val, bool check = true);
PropertySetState ceammc_abstraction_property_set_cc(t_object* x, t_symbol* key, t_float val, bool check = true);

PropertySetState ceammc_property_set_cc(t_object* x, t_symbol* key, t_float val);
const char* ceammc_property_set_cc_str_state(PropertySetState st);
}

#endif // CEAMMC_EXTERNALS_H
